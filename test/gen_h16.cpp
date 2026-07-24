/*
 * Copyright 2026 Tadashi G. Takaoka
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cctype>

#include "dis_h16.h"
#include "gen_driver.h"
#include "tokenizer.h"

using namespace libasm::h16;
using namespace libasm::gen;

namespace {

// H16 scaled-index / scaled-base operand.  Two forms collapse here:
//   index "Rn.W"/"Rn.L" with optional "*1/2/4/8" (Xm is global R0-R15), and
//   base  "Rn"/"CRn"/"PRn" with a required "*1/2/4/8" scale (@(Rn*Sf)).
// A bare register (no size, no scale) is left to H16Reg.
struct H16IndexTokenizer : Tokenizer {
    const char *tokenize(const char *p, std::string &out) const override {
        const char *q = p;
        if (toupper(q[0]) == 'C' || toupper(q[0]) == 'P')
            q++;  // optional current/previous-bank prefix of a CRn/PRn base
        if (toupper(*q) != 'R' || !isdigit(q[1]))
            return nullptr;
        q += 2;
        if (isdigit(*q))
            q++;  // Rn 10-15
        bool sized = false;
        if (*q == '.') {
            const auto sz = toupper(q[1]);
            if (sz != 'W' && sz != 'L')
                return nullptr;
            q += 2;
            sized = true;
        }
        bool scaled = false;
        if (*q == '*' && (q[1] == '1' || q[1] == '2' || q[1] == '4' || q[1] == '8')) {
            q += 2;
            scaled = true;
        }
        if (!sized && !scaled)
            return nullptr;
        out.append("Xn");
        return q;
    }
};

// H16 register with an optional current/previous-bank prefix: Rn, CRn or PRn.
// Collapse the bank class to one register token so the src x dst bank cross
// product does not explode the generated set.
struct H16RegTokenizer : Tokenizer {
    const char *tokenize(const char *p, std::string &out) const override {
        const char *q = p;
        if (toupper(q[0]) == 'C' || toupper(q[0]) == 'P')
            q++;  // optional current/previous-bank prefix
        if (toupper(*q) != 'R' || !isdigit(q[1]))
            return nullptr;
        q += 2;
        if (isdigit(*q))
            q++;  // Rn 10-15
        out.append("Rn");
        return q;
    }
};

// H16 register list (STM/LDM/PGBN): a comma-separated sequence of Rn or Rn-Rm
// elements.  Collapse a multi-element list containing a range to one token so
// these instructions do not emit a separate test per list shape.  A single
// range is left to H16List; all-singleton lists collapse by count via H16Reg.
struct H16RegListTokenizer : Tokenizer {
    const char *tokenize(const char *p, std::string &out) const override {
        const char *q = p;
        int elements = 0;
        bool hasRange = false;
        for (;;) {
            if (toupper(q[0]) != 'R' || !isdigit(q[1]))
                break;
            q += 2;
            if (isdigit(*q))
                q++;  // Rn 10-15
            if (q[0] == '-' && toupper(q[1]) == 'R' && isdigit(q[2])) {
                q += 3;
                if (isdigit(*q))
                    q++;  // -Rm 10-15
                hasRange = true;
            }
            elements++;
            if (q[0] == ',' && q[1] == ' ' && toupper(q[2]) == 'R' && isdigit(q[3])) {
                q += 2;  // skip ", " before the next element
                continue;
            }
            break;
        }
        if (elements < 2 || !hasRange)
            return nullptr;  // not a range-bearing multi-element list
        out.append("RL");
        return q;
    }
};

// Drop a forced ":N" size suffix (H16's :8/:16/:32 displacement/immediate/
// absolute width) so a forced-width value conflates with its natural-width form.
// ColonDisp consolidates the sign of a sized base displacement so it conflates
// regardless of sign.  H16's sole ":number" operand syntax is this size suffix.
struct H16SizeSuffixTokenizer : Tokenizer {
    const char *tokenize(const char *p, std::string &out) const override {
        const char *tmp;
        if (p[0] == ':' && matchNum<CstyleNumber>(p + 1, tmp))
            return tmp;  // consume ":N", emit nothing
        return nullptr;
    }
};

const H16RegListTokenizer REG_LIST;
const H16IndexTokenizer INDEX;
const H16RegTokenizer REG;
const H16SizeSuffixTokenizer SIZE_SUFFIX;
// Consolidate index/base displacement sign: "@(n, Rn)" (comma+space inside parens)
// and the size-suffixed base displacement "@(n:16, Rn)" (SIZE_SUFFIX drops ":16").
const IndexDispTokenizer<CstyleNumber, CommaIndex> INDEX_DISP;
const IndexDispTokenizer<CstyleNumber, ColonDisp> SIZED_DISP;

TokenizerList tokenizers(char loc) {
    TokenizerList list = standardTokenizers<CstyleNumber>(loc, {&INDEX_DISP, &SIZED_DISP});
    list.push_back(&REG_LIST);
    list.push_back(&INDEX);
    list.push_back(&REG);
    list.push_back(&SIZE_SUFFIX);
    return list;
}

}  // namespace

int main(int argc, const char **argv) {
    DisH16 dish16;
    GenDriver driver(dish16);
    if (driver.main(argc, argv))
        return 1;

    // C-style numbers + relative branch targets so the test generator can
    // tokenize operand values and collapse the address/branch/EA variants
    // (Motorola $xxxx and absolute targets are not recognized by the
    // tokenizer, so without this every EA variant is kept and the output
    // explodes -- e.g. ADD:G alone fills the file).
    dish16.setOption("c-style", "enable");
    dish16.setOption("relative", "enable");
    dish16.setOption("origin-char", "*");

    // The H16 exception vector table occupies 0x0000-0x03FC at reset (Manual
    // Table 4-13).  Place test code at 0x1000 so PC-relative deltas produce
    // targets in a sensible code-area range rather than the wrap-around
    // negative range that ORG=0 would yield.  ignoreSizeVariation() collapses
    // the disp8/disp16/disp32 length variants of one addressing mode (the
    // widths are size variations of the same mode, cf. gen_ns32000).  The H16
    // tokenizers add the EA/register/list collapses plus a ":N" size-suffix
    // collapse (so a forced :8/:16/:32 width dedups with its natural form).
    const auto toks = tokenizers(dish16.curSym());
    TestGenerator generator(driver, dish16, 0x1000, toks);
    generator.ignoreSizeVariation().generate();

    return driver.close();
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

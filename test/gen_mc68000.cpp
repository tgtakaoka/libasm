/*
 * Copyright 2020 Tadashi G. Takaoka
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

#include "dis_mc68000.h"
#include "gen_driver.h"
#include "tokenizer.h"

using namespace libasm::mc68000;
using namespace libasm::gen;

namespace {

// Reduce index variants "[AD][0-7].[WL]" with optional "*2/4/8" scale to "Xn".
struct Mc68kIndexTokenizer : Tokenizer {
    const char *tokenize(const char *p, std::string &out) const override {
        const auto x = toupper(*p);
        const auto n = *++p;
        if ((x == 'A' || x == 'D') && (n >= '0' && n < '8') && *++p == '.') {
            const auto sz = toupper(*++p);
            if (sz == 'W' || sz == 'L') {
                if (*++p != '*') {
                    out.append("Xn");
                    return p;
                }
                const auto sc = p[1];
                if (sc == '2' || sc == '4' || sc == '8') {
                    out.append("Xn");
                    return p + 2;
                }
            }
        }
        return nullptr;
    }
};

// Reduce MOVEM register-list elements "[AD][0-7][-/]" or "FP[0-7][-/]" to "Rn/".
struct Mc68kListTokenizer : Tokenizer {
    const char *tokenize(const char *p, std::string &out) const override {
        const auto x = toupper(*p++);
        const auto rn = *p++;
        if ((x == 'A' || x == 'D') && (rn >= '0' && rn < '8') && (*p == '/' || *p == '-')) {
            out.append("Rn/");
            return p + 1;
        }
        const auto fpn = *p++;
        if (x == 'F' && toupper(rn) == 'P' && (fpn >= '0' && fpn < '8') &&
                (*p == '/' || *p == '-')) {
            out.append("Rn/");
            return p + 1;
        }
        return nullptr;
    }
};

const Mc68kIndexTokenizer INDEX;
const Mc68kListTokenizer LIST;
// Register files D0-D7, A0-A7, FP0-FP7.  INDEX/LIST run BEFORE these so an index
// "d1.l" collapses to "Xn" and a MOVEM element to "Rn/" rather than "Dn.l"/"Dn/".
const RegisterTokenizer REG_FP("FP", 7, "FPn");
const RegisterTokenizer REG_D("D", 7, "Dn");
const RegisterTokenizer REG_A("A", 7, "An");

// |Hex| = MotorolaNumber (native $..) or CstyleNumber (gnu-as 0x..).  Consolidate
// index/base displacement sign: "(n,An)", "(n,An,Dn.sz)".
template <class Hex>
TokenizerList tokenizers(char loc) {
    static const IndexDispTokenizer<Hex, CommaIndex> indexDisp;
    return standardTokenizers<Hex>(
            loc, {&indexDisp, &INDEX, &LIST, &REG_FP, &REG_D, &REG_A});
}

}  // namespace

int main(int argc, const char **argv) {
    DisMc68000 dis68000;
    GenDriver driver(dis68000);
    if (driver.main(argc, argv))
        return 1;

    dis68000.setOption("relative", "enable");
    if (driver.generateGas())
        dis68000.setOption("gnu-as", "enable");

    const auto sym = dis68000.curSym();
    const auto toks = driver.generateGas() ? tokenizers<CstyleNumber>(sym)
                                           : tokenizers<MotorolaNumber>(sym);
    TestGenerator generator(driver, dis68000, 0x10000, toks);
    generator.generate();

    return driver.close();
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

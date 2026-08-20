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

#include "dis_am29000.h"
#include "gen_driver.h"
#include "tokenizer.h"

using namespace libasm::am29000;
using namespace libasm::gen;

namespace {

// Match a branch target symbol "A_XXXXXXXX" (8 hex digits) emitted by
// DisAm29000 with the extern-symbol option (gas path).  Collapse to "a" so the
// generator dedups across targets instead of keeping one test per address.
struct Am29000ExternSymTokenizer : Tokenizer {
    const char *tokenize(const char *p, std::string &out) const override {
        if (p[0] != 'A' || p[1] != '_')
            return nullptr;
        for (int i = 2; i < 10; i++) {
            if (!isxdigit(p[i]))
                return nullptr;
        }
        out.push_back('a');
        return p + 10;
    }
};

const Am29000ExternSymTokenizer EXTERN_SYM;

TokenizerList tokenizers(char loc) {
    auto list = standardTokenizers<CstyleNumber>(loc);
    list.push_back(&EXTERN_SYM);
    return list;
}

}  // namespace

int main(int argc, const char **argv) {
    DisAm29000 dis29000;
    GenDriver driver(dis29000);
    if (driver.main(argc, argv))
        return 1;

    // GNU as picks the encoding of a branch from the way its target is written:
    // an expression on the location counter gives the Program-Counter relative
    // form, so write that one relatively.  Its assembler rejects an absolute
    // target over 0x1FFFC, so name that one and leave it to the linker; the
    // scraper turns each name into a --defsym.  The location counter is "."
    // there, "$" natively.
    if (driver.generateGas()) {
        dis29000.setOption("extern-symbol", "enable");
        dis29000.setOption("relative", "enable");
        dis29000.setOption("origin-char", ".");
    }

    // Numbers are C style for both this assembler and ASL, and a register
    // number is collapsed by the number matcher, so the standard tokenizers
    // need no register or index helper.  There is no indexed addressing and no
    // negative displacement to consolidate.
    // A Program-Counter relative branch reaches 0x20000 bytes back, so generate
    // above that: from a lower origin the most negative displacements would
    // wrap past zero, and neither this assembler nor GNU as encodes a branch
    // which wraps.  0x40000 is also just past the largest absolute target, so
    // the two encodings of a branch stay clearly apart.
    TestGenerator generator(driver, dis29000, 0x40000, tokenizers(dis29000.curSym()));
    // An instruction is one 32-bit word, so scan it a byte at a time to give
    // the generator the child chain its drop heuristic needs, see scanByByte.
    generator.scanByByte().generate();

    return driver.close();
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

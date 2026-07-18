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

#include "dis_ns32000.h"
#include "gen_driver.h"
#include "tokenizer.h"

using namespace libasm::ns32000;
using namespace libasm::gen;

namespace {

bool isNs32kScale(const char *p) {
    if (p[1] == ']') {
        const auto c = toupper(*p);
        return c == 'B' || c == 'W' || c == 'D' || c == 'Q';
    }
    return false;
}

// Reduce displacement variants "+(-n)" to "+n" (drops the surrounding parens).
struct Ns32kDispTokenizer : Tokenizer {
    const char *tokenize(const char *p, std::string &out) const override {
        const char *tmp;
        if (p[0] == '+' && p[1] == '(' && p[2] == '-' && matchNum<CstyleNumber>(p + 3, tmp) &&
                *tmp == ')') {
            out.append("+n");
            return tmp + 1;
        }
        return nullptr;
    }
};

// Reduce index size variants "[Rn:B]", "[Rn:W]", "[Rn:D]", "[Rn:Q]" to ":s".
struct Ns32kScaleTokenizer : Tokenizer {
    const char *tokenize(const char *p, std::string &out) const override {
        if (p[0] == ':' && isNs32kScale(p + 1)) {
            out.push_back(':');
            out.push_back('s');
            out.push_back(p[2]);
            return p + 3;
        }
        return nullptr;
    }
};

const Ns32kDispTokenizer DISP;
const Ns32kScaleTokenizer SCALE;
// Consolidate index/base displacement sign: "n(Rn)", nested "n(n(Rn))".
const IndexDispTokenizer<CstyleNumber, ParenIndex> INDEX_DISP;
// Register files: R0-R7, F0-F7 (FPU); config regs PTB0/1, BPR0/1.
const RegisterTokenizer REG_PTB("PTB", 1, "PTBn");
const RegisterTokenizer REG_BPR("BPR", 1, "BPRn");
const RegisterTokenizer REG_R("R", 7, "Rn");
const RegisterTokenizer REG_F("F", 7, "Fn");

TokenizerList tokenizers(char loc) {
    TokenizerList list = standardTokenizers<CstyleNumber>(
            loc, {&INDEX_DISP, &REG_PTB, &REG_BPR, &REG_R, &REG_F});
    list.push_back(&DISP);
    list.push_back(&SCALE);
    return list;
}

}  // namespace

int main(int argc, const char **argv) {
    DisNs32000 dis32000;
    GenDriver driver(dis32000);
    if (driver.main(argc, argv))
        return 1;

    dis32000.setOption("relative", "enable");
    dis32000.setOption("c-style", "enable");
    dis32000.setOption("origin-char", "*");
    if (driver.generateGas()) {
        dis32000.setOption("gnu-as", "on");
    }
    const auto toks = tokenizers(dis32000.curSym());
    TestGenerator generator(driver, dis32000, 0x10000, toks);
    generator.ignoreSizeVariation().generate();

    return driver.close();
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

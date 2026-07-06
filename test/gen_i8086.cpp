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

#include "dis_i8086.h"
#include "gen_driver.h"
#include "tokenizer.h"

using namespace libasm::i8086;
using namespace libasm::gen;

namespace {

bool isX86Reg8(const char *p) {
    const auto c1 = toupper(p[0]);
    const auto c2 = toupper(p[1]);
    return (c1 >= 'A' && c1 <= 'D') && (c2 == 'L' || c2 == 'H');
}

bool isX86Reg16(const char *p) {
    const auto c1 = toupper(p[0]);
    const auto c2 = toupper(p[1]);
    if (c2 == 'X')
        return c1 >= 'B' && c1 <= 'D';  // BX/CX/DX
    if (c2 == 'P')
        return c1 == 'B' || c1 == 'S';  // BP/SP
    if (c2 == 'I')
        return c1 == 'D' || c1 == 'S';  // DI/SI
    return false;
}

bool isX86Reg32(const char *p) {
    return toupper(p[0]) == 'E' && isX86Reg16(p + 1);
}

bool isX86Seg(const char *p) {
    const auto s = toupper(p[1]);
    if (s == 'S' && p[2] == ':') {
        const auto c = toupper(*p);
        return (c >= 'C' && c <= 'G') || c == 'S';  // CS/DS/ES/FS/GS/SS
    }
    return false;
}

// Reduce 16-bit register indexing "[BX/.../SI+n]" to "<reg>+n".
struct X86Reg16IndexTokenizer : Tokenizer {
    const char *tokenize(const char *p, std::string &out) const override {
        const char *tmp;
        if (isX86Reg16(p) && (p[2] == '+' || p[2] == '-') && isNumber(p + 3, tmp) &&
                *tmp == ']') {
            out.push_back(p[1]);
            out.push_back('+');
            out.push_back('n');
            return tmp + 1;
        }
        return nullptr;
    }
};

// Reduce 32-bit register indexing "[EBX/.../ESI+n]" to "<reg>+n".
struct X86Reg32IndexTokenizer : Tokenizer {
    const char *tokenize(const char *p, std::string &out) const override {
        const char *tmp;
        if (isX86Reg32(p) && (p[3] == '+' || p[3] == '-') && isNumber(p + 4, tmp) &&
                *tmp == ']') {
            out.push_back(p[2]);
            out.push_back('+');
            out.push_back('n');
            return tmp + 1;
        }
        return nullptr;
    }
};

// Reduce index scaling "*2/4/8" to "*s".
struct X86ScaleTokenizer : Tokenizer {
    const char *tokenize(const char *p, std::string &out) const override {
        if (p[0] == '*' && (p[1] == '2' || p[1] == '4' || p[1] == '8')) {
            out.append("*s");
            return p + 2;
        }
        return nullptr;
    }
};

// Reduce 8-bit register variants "AL/BL/CL/DL, AH/BH/CH/DH" by their L/H.
struct X86Reg8Tokenizer : Tokenizer {
    const char *tokenize(const char *p, std::string &out) const override {
        if (isX86Reg8(p)) {
            out.push_back(p[1]);
            return p + 2;
        }
        return nullptr;
    }
};

// Reduce 16-bit register variants "BX/CX/DX, BP/SP, DI/SI" by their 2nd char.
struct X86Reg16Tokenizer : Tokenizer {
    const char *tokenize(const char *p, std::string &out) const override {
        if (isX86Reg16(p)) {
            out.push_back(p[1]);
            return p + 2;
        }
        return nullptr;
    }
};

// Reduce 32-bit register variants "EBX/ECX/EDX, EBP/ESP, EDI/ESI" by their 3rd char.
struct X86Reg32Tokenizer : Tokenizer {
    const char *tokenize(const char *p, std::string &out) const override {
        if (isX86Reg32(p)) {
            out.push_back(p[2]);
            return p + 3;
        }
        return nullptr;
    }
};

// Reduce segment override "ES:/CS:/SS:/DS:/FS:/GS:" to "so:".
struct X86SegTokenizer : Tokenizer {
    const char *tokenize(const char *p, std::string &out) const override {
        if (isX86Seg(p)) {
            out.append("so:");
            return p + 3;
        }
        return nullptr;
    }
};

const X86Reg16IndexTokenizer REG16_INDEX;
const X86Reg32IndexTokenizer REG32_INDEX;
const X86ScaleTokenizer SCALE;
const X86Reg8Tokenizer REG8;
const X86Reg16Tokenizer REG16;
const X86Reg32Tokenizer REG32;
const X86SegTokenizer SEG;

TokenizerList tokenizers(char loc) {
    TokenizerList list = standardTokenizers(loc);
    list.push_back(&REG16_INDEX);
    list.push_back(&REG32_INDEX);
    list.push_back(&SCALE);
    list.push_back(&REG8);
    list.push_back(&REG16);
    list.push_back(&REG32);
    list.push_back(&SEG);
    return list;
}

}  // namespace

int main(int argc, const char **argv) {
    DisI8086 dis8086;
    GenDriver driver(dis8086);
    if (driver.main(argc, argv))
        return 1;

    dis8086.setOption("relative", "enable");
    dis8086.setOption("string-insn", "enable");
    dis8086.setOption("segment-insn", "enable");
    if (driver.generateGas()) {
        dis8086.setOption("gnu-as", "on");
        dis8086.setOption("origin-char", "$");
        // i80186 may be used with i80C187
        dis8086.setOption("fpu", "on");
    } else {
        dis8086.setOption("lockInsn", "on");
    }

    const auto toks = tokenizers(dis8086.curSym());
    TestGenerator generator(driver, dis8086, 0x0000, toks);
    generator.generate().generate(0x9B);

    return driver.close();
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

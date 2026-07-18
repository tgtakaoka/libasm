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
#include <cstring>

#include "dis_tlcs900.h"
#include "gen_driver.h"
#include "tokenizer.h"

using namespace libasm::tlcs900;
using namespace libasm::gen;

namespace {

// TLCS900 absolute-bank registers are composed at output time (reg_tlcs900.cpp
// outAbsReg) as <basename><bank-digit>: XWA0, A0, QW0, DMAS0, ...  The bank digit
// selects one of the register banks (0-7 in min mode; the disassembler currently
// emits banks 0-3) and, like a register-file index (R0-R15 -> Rn), is collapsed
// to 'n' so every bank of one register dedups to a single representative.  The
// base names are the bank-relative registers plus the DMA channel registers; the
// current-bank (no suffix) and previous-bank (') forms stay distinct, matching
// the disassembler's distinct encodings.
struct Tlcs900AbsRegTokenizer : Tokenizer {
    const char *tokenize(const char *p, std::string &out) const override {
        if (!out.empty() && isalnum(static_cast<unsigned char>(out.back())))
            return nullptr;
        static const char *const BASES[] = {
                "XWA", "XBC", "XDE", "XHL",                          // 32-bit
                "QWA", "QBC", "QDE", "QHL", "WA", "BC", "DE", "HL",  // 16-bit
                "QA", "QB", "QC", "QD", "QE", "QH", "QL", "QW",      // 8-bit Q
                "A", "B", "C", "D", "E", "H", "L", "W",              // 8-bit
                "DMAC", "DMAD", "DMAM", "DMAS",                      // DMA channels
        };
        for (const char *const base : BASES) {
            const auto len = std::strlen(base);
            if (std::strncmp(p, base, len) != 0)
                continue;
            const auto digit = p[len];
            if (digit < '0' || digit > '7')
                continue;  // not a bank digit
            if (isalnum(static_cast<unsigned char>(p[len + 1])))
                continue;  // the bank is a single digit; require a boundary
            out.append(base);
            out.push_back('n');
            return p + len + 1;
        }
        return nullptr;
    }
};

const Tlcs900AbsRegTokenizer ABS_REG;
const RegDispTokenizer<IntelNumber, ParenReg> INDEX_TOK;

}  // namespace

int main(int argc, const char **argv) {
    DisTlcs900 dis900;
    dis900.setOption("relative", "enable");
    GenDriver driver(dis900);
    if (driver.main(argc, argv))
        return 1;

    TestGenerator generator(driver, dis900, 0x0100,
            standardTokenizers<IntelNumber>(dis900.curSym(), {&ABS_REG, &INDEX_TOK}));
    generator.generate();

    return driver.close();
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

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

#ifndef __REG_MC68000_H__
#define __REG_MC68000_H__

#include "reg_base.h"

#include "entry_mc68000.h"

namespace libasm {
namespace mc68000 {

enum RegName : int8_t {
    REG_UNDEF = -1,
    // Data registers.
    REG_D0 = 0 + 0,
    REG_D1 = 1 + 0,
    REG_D2 = 2 + 0,
    REG_D3 = 3 + 0,
    REG_D4 = 4 + 0,
    REG_D5 = 5 + 0,
    REG_D6 = 6 + 0,
    REG_D7 = 7 + 0,
    // Address registers.
    REG_A0 = 0 + 8,
    REG_A1 = 1 + 8,
    REG_A2 = 2 + 8,
    REG_A3 = 3 + 8,
    REG_A4 = 4 + 8,
    REG_A5 = 5 + 8,
    REG_A6 = 6 + 8,
    REG_A7 = 7 + 8,
    // Other registers.
    REG_CCR = 0 + 16,
    REG_PC = 1 + 16,
    REG_SR = 2 + 16,
    REG_USP = 3 + 16,
};

class RegMc68000 : public RegBase {
public:
    static RegName parseRegName(StrScanner &scan);
    StrBuffer &outRegName(StrBuffer &out, RegName name);
    static bool isDataReg(RegName name);
    static bool isAddrReg(RegName name);
    static bool isGeneralReg(RegName name);
    static Config::opcode_t encodeGeneralRegNo(RegName name);
    static uint8_t encodeGeneralRegPos(RegName name);
    static RegName decodeGeneralReg(uint8_t regno);
    static RegName decodeDataReg(uint8_t regno);
    static RegName decodeAddrReg(uint8_t regno);

    static OprSize parseSize(StrScanner &scan);
    static uint8_t sizeNameLen(OprSize size);
    char sizeSuffix(OprSize size) const;
};

struct EaMc68000 {
    EaMc68000(OprSize size, uint8_t mode, uint8_t regno);

    static Config::opcode_t encodeMode(AddrMode mode);
    static Config::opcode_t encodeRegNo(AddrMode mode, RegName reg);

    OprSize size;
    AddrMode mode;
    RegName reg;
};

// Brief extension word.
struct BriefExt {
    uint16_t word;

    OprSize indexSize() const;
    RegName index() const;
    uint8_t disp() const;
};

}  // namespace mc68000
}  // namespace libasm

#endif  // __REG_MC68000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

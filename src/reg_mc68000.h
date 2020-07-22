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

#include "entry_mc68000.h"
#include "reg_base.h"

namespace libasm {
namespace mc68000 {

enum RegName : char {
    REG_UNDEF = 0,
    REG_D0 = '0',
    REG_D1 = '1',
    REG_D2 = '2',
    REG_D3 = '3',
    REG_D4 = '4',
    REG_D5 = '5',
    REG_D6 = '6',
    REG_D7 = '7',
    REG_A0 = 'a',
    REG_A1 = 'b',
    REG_A2 = 'c',
    REG_A3 = 'd',
    REG_A4 = 'e',
    REG_A5 = 'f',
    REG_A6 = 'g',
    REG_A7 = 'h',
    REG_CCR = 'C',
    REG_PC = 'P',
    REG_SR = 'S',
    REG_USP = 'U',
};

class RegMc68000 : public RegBase {
public:
    static uint8_t regNameLen(RegName);
    static RegName parseRegName(const char *line);
    char *outRegName(char *out, RegName regName) const;
    char *outOprSize(char *out, OprSize size) const;
    static bool isDreg(RegName reg);
    static bool isAreg(RegName reg);
    static bool isADreg(RegName reg);
    static Config::opcode_t encodeRegNo(RegName reg);
    static uint8_t encodeRegPos(RegName reg);
    static RegName decodeDataReg(uint8_t regno);
    static RegName decodeAddrReg(uint8_t regno);
};

struct EaMc68000 {
    EaMc68000(OprSize size, uint8_t mode, uint8_t regno);

    static Config::opcode_t encodeMode(AddrMode mode);
    static Config::opcode_t encodeRegNo(AddrMode mode, RegName regName);

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

} // namespace mc68000
} // namespace libasm

#endif // __REG_MC68000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

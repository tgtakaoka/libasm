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

// Effective Address Size
enum EaSize {
    SZ_BYTE     = 0,
    SZ_WORD     = 1,
    SZ_LONG     = 2,
    SZ_INVALID  = 3,
    SZ_NONE     = 4,
};

class RegMc68000 : public RegBase {
public:
    static uint8_t regNameLen(RegName);
    static RegName parseRegName(const char *line);
    char *outRegName(char *out, RegName regName) const;
    char *outEaSize(char *out, EaSize size) const;
    static bool isDreg(RegName reg);
    static bool isAreg(RegName reg);
    static bool isADreg(RegName reg);
    static Config::opcode_t encodeRegNo(RegName reg);
    static uint8_t encodeRegPos(RegName reg);
    static RegName decodeDataReg(uint8_t regno);
    static RegName decodeAddrReg(uint8_t regno);
};

// Effective Address Mode
enum EaMode {
    M_DREG      = 0,    // D__A: Dn: Data Register Direct
    M_AREG      = 1,    // ___A: An: Address Register Direct
    M_AIND      = 2,    // DMCA: (An): Address Register Indirect
    M_PINC      = 3,    // DM_A: (An)+: Address Register Indirect with Postincrement
    M_PDEC      = 4,    // DM_A: -(An): Address Register Indirect with Predecrement
    M_DISP      = 5,    // DMCA: (d16,An): Address Register Indirect with Displacement
    M_INDX      = 6,    // DMCA: (d8,An,Xn): Address Register Indirect with Index
    M_ABS_SHORT = 8+0,  // DMCA: (xxx).W: Absolute Short Addressing
    M_ABS_LONG  = 8+1,  // DMCA: (xxx).L: Absolute Long Addressing
    M_PC_DISP   = 8+2,  // DMC_: (d16,PC): Program Counter Indirect with Displacement
    M_PC_INDX   = 8+3,  // DMC_: (d8,PC,Xn): Program Counter Indirect with Index
    M_IMM_DATA  = 8+4,  // DM__: #imm: Immediate Data
    M_ILLEGAL   = 31,

    // for assembler operand parsing
    M_NONE      = 16,   // no operand
    M_MULT_REGS = 17,   // MOVEM register list
    M_LABEL     = 18,   // label
};

// Effective Address Category
class EaCat final {
public:
    enum Type : uint8_t {
        NONE      = 1 << 0,
        DATA      = 1 << 1,
        MEMORY    = 1 << 2,
        CONTROL   = 1 << 3,
        ALTERABLE = 1 << 4,
    };

    constexpr EaCat(const uint8_t val) noexcept
        : _val(static_cast<Type>(val))
    {}

    constexpr operator uint8_t() const { return _val; }

private:
    Type _val;
};

struct EaMc68000 {
    EaMc68000(Config::opcode_t opCode);
    EaMc68000(EaSize size, uint8_t mode, uint8_t regno);
    EaMc68000(EaSize size, EaMode mode, uint8_t regno);

    bool satisfy(EaCat categories) const {
        return satisfy(mode, categories);
    }
    static bool satisfy(EaMode mode, EaCat categories);
    static Config::opcode_t encodeMode(EaMode mode);
    static Config::opcode_t encodeRegNo(EaMode mode, RegName regName);
    static const char *eaCategory(EaMode mode);

    EaSize size;
    EaMode mode;
    RegName reg;
};

// Brief extension word.
struct BriefExt {
    uint16_t word;

    EaSize indexSize() const;
    RegName index() const;
    uint8_t disp() const;
};

static const char *eaSize(EaSize size) __attribute__((unused));
static const char *eaSize(EaSize size) {
    switch (size) {
    case SZ_BYTE: return ".B";
    case SZ_WORD: return ".W";
    case SZ_LONG: return ".L";
    case SZ_NONE: return "__";
    default: return "SZ_unkn";
    }
}

static const char *eaMode(EaMode mode) __attribute__((unused));
static const char *eaMode(EaMode mode) {
    switch (mode) {
    case M_DREG: return "Dn";
    case M_AREG: return "An";
    case M_AIND: return "(An)";
    case M_PINC: return "(An)+";
    case M_PDEC: return "-(An)";
    case M_DISP: return "(d16,An)";
    case M_INDX: return "(d8,An,Xn)";
    case M_ABS_SHORT: return "(xxx).W";
    case M_ABS_LONG:  return "(xxx).L";
    case M_PC_DISP: return "(d16,PC)";
    case M_PC_INDX: return "(d8,PC,Xn)";
    case M_IMM_DATA: return "#imm";
    case M_NONE:     return "____";
    case M_MULT_REGS: return "Dx/Ax";
    case M_LABEL:     return "label";
    default: return "M_unkn";
    }
}

} // namespace mc68000
} // namespace libasm

#endif // __REG_MC68000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

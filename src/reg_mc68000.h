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

#ifndef __REGISTER_MC68000_H__
#define __REGISTER_MC68000_H__

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

#include "reg_base.h"

// Effective Address Size
enum EaSize : host::uint_t {
    SZ_BYTE = 0,
    SZ_WORD = 1,
    SZ_LONG = 2,
    SZ_INVALID = 3,
    SZ_NONE = 4,
};

class RegMc68000 : public RegBase {
public:
    static host::uint_t regNameLen(RegName);
    static RegName parseRegName(const char *line);
    char *outRegName(char *out, RegName regName) const;
    char *outEaSize(char *out, EaSize size) const;
    static bool isDreg(RegName reg);
    static bool isAreg(RegName reg);
    static bool isADreg(RegName reg);
    static target::insn_t encodeRegNo(RegName reg);
    static host::uint_t encodeRegPos(RegName reg);
    static RegName decodeDataReg(host::uint_t regno);
    static RegName decodeAddrReg(host::uint_t regno);
};

// Effective Address Mode
enum EaMode : host::uint_t {
    M_DREG   = 0,       // D__A: Dn: Data Register Direct
    M_AREG   = 1,       // ___A: An: Address Register Direct
    M_AIND   = 2,       // DMCA: (An): Address Register Indirect
    M_PINC   = 3,       // DM_A: (An)+: Address Register Indirect with Postincrement
    M_PDEC   = 4,       // DM_A: -(An): Address Register Indirect with Predecrement
    M_DISP   = 5,       // DMCA: (d16,An): Address Register Indirect with Displacement
    M_INDX   = 6,       // DMCA: (d8,An,Xn): Address Register Indirect with Index
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
#define CAT_NONE      0
#define CAT_DATA      1
#define CAT_MEMORY    2
#define CAT_CONTROL   4
#define CAT_ALTERABLE 8

struct EaMc68000 {
    EaMc68000(target::insn_t insnCode);
    EaMc68000(EaSize size, host::uint_t mode, host::uint_t regno);
    EaMc68000(EaSize size, EaMode mode, host::uint_t regno);

    bool satisfy(host::uint_t categories) const {
        return satisfy(mode, categories);
    }
    static bool satisfy(EaMode mode, host::uint_t categories);
    static target::insn_t encodeMode(EaMode mode);
    static target::insn_t encodeRegNo(EaMode mode, RegName regName);
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

#endif // __REGISTER_MC68000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

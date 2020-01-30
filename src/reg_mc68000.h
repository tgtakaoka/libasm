/* -*- mode: c++; -*- */
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
    char *outRegName(char *out, RegName regName) const;
    char *outEaSize(char *out, EaSize size) const;
    static bool isDreg(RegName reg);
    static bool isAreg(RegName reg);
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
    M_ABS_SHORT = 8,    // DMCA: (xxx).W: Absolute Short Addressing
    M_ABS_LONG  = 9,    // DMCA: (xxx).L: Absolute Long Addressing
    M_PC_DISP   = 10,   // DMC_: (d16,PC): Program Counter Indirect with Displacement
    M_PC_INDX   = 11,   // DMC_: (d8,PC,Xn): Program Counter Indirect with Index
    M_IMM_DATA  = 12,   // DM__: #imm: Immediate Data
    M_ILLEGAL   = 13,
};

// Effective Address Category
#define CAT_ILLEGAL   0
#define CAT_DATA      1
#define CAT_MEMORY    2
#define CAT_CONTROL   4
#define CAT_ALTERABLE 8

class EaMc68000 {
public:
    EaMc68000(target::insn_t insnCode);
    EaMc68000(EaSize size, host::uint_t mode, host::uint_t regno);
    EaMc68000(EaSize size, EaMode mode, host::uint_t regno);

    EaSize size() const { return _size; }
    EaMode mode() const { return _mode; }
    RegName reg() const;

    bool satisfy(host::uint_t categories) const;

private:
    EaSize _size;
    EaMode _mode;
    host::uint_t _regno;
    host::uint_t _categories;
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
    default: return "M_unkn";
    }
}

#endif // __REGISTER_MC68000_H__

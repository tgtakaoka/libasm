#include "config_mc68000.h"
#include "reg_mc68000.h"

#include <ctype.h>

static void toUppercase(char *p) {
    while (*p) {
        *p = toupper(*p);
        p++;
    }
}

char *RegMc68000::outRegName(char *out, RegName regName) const {
    char *p = out;
    if (isDreg(regName)) {
        *out++ = 'd';
        *out++ = char(regName) - char(REG_D0) + '0';
    } else if (isAreg(regName)) {
        *out++ = 'a';
        *out++ = char(regName) - char(REG_A0) + '0';
    } else if (regName == REG_CCR) {
        *out++ = 'c';
        *out++ = 'c';
        *out++ = 'r';
    } else if (regName == REG_PC) {
        *out++ = 'p';
        *out++ = 'c';
    } else if (regName == REG_SR) {
        *out++ = 's';
        *out++ = 'r';
    } else if (regName == REG_USP) {
        *out++ = 'u';
        *out++ = 's';
        *out++ = 'p';
    }
    *out = 0;
    if (_uppercase) toUppercase(p);
    return out;
}

char *RegMc68000::outEaSize(char *out, EaSize size) const {
    char *p = out;
    *out++ = '.';
    switch (size) {
    case SZ_BYTE: *out++ = 'b'; break;
    case SZ_WORD: *out++ = 'w'; break;
    case SZ_LONG: *out++ = 'l'; break;
    default: out--; break;
    }
    *out = 0;
    if (_uppercase) toUppercase(p);
    return out;
}

bool RegMc68000::isDreg(RegName reg) {
    const char r = char(reg);
    return r >= char(REG_D0) && r <= char(REG_D7);
}

bool RegMc68000::isAreg(RegName reg) {
    const char r = char(reg);
    return r >= char(REG_A0) && r <= char(REG_A7);
}

RegName RegMc68000::decodeDataReg(host::uint_t regno) {
    return RegName(char(REG_D0) + (regno & 7));
}

RegName RegMc68000::decodeAddrReg(host::uint_t regno) {
    return RegName(char(REG_A0) + (regno & 7));
}

static EaMode parseEaMode(host::uint_t mode, host::uint_t regno) {
    if (mode == 7) {
        switch (regno) {
        case 0: return M_ABS_SHORT;
        case 1: return M_ABS_LONG;
        case 2: return M_PC_DISP;
        case 3: return M_PC_INDX;
        case 4: return M_IMM_DATA;
        default: return M_ILLEGAL;
        }
    }
    return EaMode(mode);
}

static host::uint_t getCategories(EaMode mode) {
    switch (mode) {
    case M_DREG:
        return CAT_DATA | CAT_ALTERABLE;
    case M_AREG:
        return CAT_ALTERABLE;
    case M_AIND:
    case M_DISP:
    case M_INDX:
    case M_ABS_SHORT:
    case M_ABS_LONG:
        return CAT_DATA | CAT_MEMORY | CAT_CONTROL | CAT_ALTERABLE;
    case M_PINC:
    case M_PDEC:
        return CAT_DATA | CAT_MEMORY | CAT_ALTERABLE;
    case M_PC_DISP:
    case M_PC_INDX:
        return CAT_DATA | CAT_MEMORY | CAT_CONTROL;
    case M_IMM_DATA:
        return CAT_DATA;
    default:
        return CAT_ILLEGAL;
    }
}

static RegName encodeRegName(EaMode mode, host::uint_t regno) {
    switch (mode) {
    case M_DREG:
        return RegMc68000::decodeDataReg(regno);
    case M_ABS_SHORT:
    case M_ABS_LONG:
    case M_PC_DISP:
    case M_IMM_DATA:
    case M_ILLEGAL:
        return REG_UNDEF;
    default:
        return RegMc68000::decodeAddrReg(regno);
    }
}

EaMc68000::EaMc68000(target::insn_t insnCode) {
    const host::uint_t regno = insnCode & 7;
    size = EaSize((insnCode >> 6) & 3);
    mode = parseEaMode((insnCode >> 3) & 7, regno);
    reg = encodeRegName(mode, regno);
}

EaMc68000::EaMc68000(EaSize size_, host::uint_t raw_mode, host::uint_t regno) {
    regno &= 7;
    size = size_;
    mode = parseEaMode(raw_mode & 7, regno);
    reg = encodeRegName(mode, regno);
}

EaMc68000::EaMc68000(EaSize size_, EaMode mode_, host::uint_t regno) {
    size = size_;
    mode = mode_;
    reg = encodeRegName(mode, regno);
}

bool EaMc68000::satisfy(EaMode mode, host::uint_t categories) {
    return (getCategories(mode) & categories) == categories;
}

EaSize BriefExt::indexSize() const {
    return (word & 0x800) ? SZ_LONG : SZ_WORD;
}

RegName BriefExt::index() const {
    const RegName base = (word & 0x8000) ? REG_A0 : REG_D0;
    const uint8_t regno = (word >> 12) & 7;
    return RegName(char(base) + regno);
}

uint8_t BriefExt::disp() const {
    return static_cast<uint8_t>(word);
}

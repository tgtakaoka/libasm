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

#include "reg_mc68000.h"
#include "reg_base.h"
#include "str_buffer.h"
#include "str_scanner.h"
#include "text_mc68000.h"
#include "value_parser.h"

using namespace libasm::reg;
using namespace libasm::text::mc68000;

namespace libasm {
namespace mc68000 {
namespace reg {

namespace {
// clang-format off

constexpr NameEntry REG_ENTRIES[] PROGMEM = {
    { TEXT_REG_A0,    REG_A0    },
    { TEXT_REG_A1,    REG_A1    },
    { TEXT_REG_A2,    REG_A2    },
    { TEXT_REG_A3,    REG_A3    },
    { TEXT_REG_A4,    REG_A4    },
    { TEXT_REG_A5,    REG_A5    },
    { TEXT_REG_A6,    REG_A6    },
    { TEXT_REG_A7,    REG_A7    },
    { TEXT_REG_CCR,   REG_CCR   },
    { TEXT_REG_D0,    REG_D0    },
    { TEXT_REG_D1,    REG_D1    },
    { TEXT_REG_D2,    REG_D2    },
    { TEXT_REG_D3,    REG_D3    },
    { TEXT_REG_D4,    REG_D4    },
    { TEXT_REG_D5,    REG_D5    },
    { TEXT_REG_D6,    REG_D6    },
    { TEXT_REG_D7,    REG_D7    },
    { TEXT_REG_FP0,   REG_FP0   },
    { TEXT_REG_FP1,   REG_FP1   },
    { TEXT_REG_FP2,   REG_FP2   },
    { TEXT_REG_FP3,   REG_FP3   },
    { TEXT_REG_FP4,   REG_FP4   },
    { TEXT_REG_FP5,   REG_FP5   },
    { TEXT_REG_FP6,   REG_FP6   },
    { TEXT_REG_FP7,   REG_FP7   },
    { TEXT_REG_FPCR,  REG_FPCR  },
    { TEXT_REG_FPIAR, REG_FPIAR },
    { TEXT_REG_FPSR,  REG_FPSR  },
    { TEXT_REG_PC,    REG_PC    },
    { TEXT_REG_SR,    REG_SR    },
};

PROGMEM constexpr NameTable TABLE{ARRAY_RANGE(REG_ENTRIES)};

constexpr NameEntry CREG_ENTRIES[] PROGMEM = {
    { TEXT_REG_CAAR,  CREG_CAAR  },
    { TEXT_REG_CACR,  CREG_CACR  },
    { TEXT_REG_DACR0, CREG_DACR0 },
    { TEXT_REG_DACR1, CREG_DACR1 },
    { TEXT_REG_DFC,   CREG_DFC   },
    { TEXT_REG_DTT0,  CREG_DTT0  },
    { TEXT_REG_DTT1,  CREG_DTT1  },
    { TEXT_REG_IACR0, CREG_IACR0 },
    { TEXT_REG_IACR1, CREG_IACR1 },
    { TEXT_REG_ISP,   CREG_ISP   },
    { TEXT_REG_ITT0,  CREG_ITT0  },
    { TEXT_REG_ITT1,  CREG_ITT1  },
    { TEXT_REG_MMUSR, CREG_MMUSR },
    { TEXT_REG_MSP,   CREG_MSP   },
    { TEXT_REG_SFC,   CREG_SFC   },
    { TEXT_REG_SRP,   CREG_SRP   },
    { TEXT_REG_TC,    CREG_TC    },
    { TEXT_REG_URP,   CREG_URP   },
    { TEXT_REG_USP,   CREG_USP   },
    { TEXT_REG_VBR,   CREG_VBR   },
};

PROGMEM constexpr NameTable CREG_TABLE{ARRAY_RANGE(CREG_ENTRIES)};

#if !defined(LIBASM_MC68000_NOPMMU)
constexpr NameEntry PREG_ENTRIES[] PROGMEM = {
    { TEXT_REG_AC,    PREG_AC    },
    { TEXT_REG_AC0,   PREG_AC0   },
    { TEXT_REG_AC1,   PREG_AC1   },
    { TEXT_REG_ACUSR, PREG_ACUSR },
    { TEXT_REG_CAL,   PREG_CAL   },
    { TEXT_REG_CRP,   PREG_CRP   },
    { TEXT_REG_DRP,   PREG_DRP   },
    { TEXT_REG_PCSR,  PREG_PCSR  },
    { TEXT_REG_PSR,   PREG_PSR   },
    { TEXT_REG_SCC,   PREG_SCC   },
    { TEXT_REG_SRP,   PREG_SRP   },
    { TEXT_REG_TC,    PREG_TC    },
    { TEXT_REG_TT0,   PREG_TT0   },
    { TEXT_REG_TT1,   PREG_TT1   },
    { TEXT_REG_VAL,   PREG_VAL   },
};

PROGMEM constexpr NameTable PREG_TABLE{ARRAY_RANGE(PREG_ENTRIES)};
#endif

constexpr NameEntry CACHE_ENTRIES[] PROGMEM = {
    { TEXT_BC, CACHE_BOTH },
    { TEXT_DC, CACHE_DATA },
    { TEXT_IC, CACHE_INST },
    { TEXT_NC, CACHE_NONE },
};

PROGMEM constexpr NameTable CACHE_TABLE{ARRAY_RANGE(CACHE_ENTRIES)};

// clang-format on
}  // namespace

RegName parseRegName(StrScanner &scan, const ValueParser &parser) {
    auto p = scan;
    // exclude register size '.[WL]' at end of register name
    const auto *entry = TABLE.searchText(parser.readRegName(p, true));
    if (entry) {
        scan = p;
        return RegName(entry->name());
    }
    return REG_UNDEF;
}

StrBuffer &outRegName(StrBuffer &out, RegName name) {
    const auto *entry = TABLE.searchName(name);
    return entry ? entry->outText(out) : out;
}

bool isDataReg(RegName name) {
    return name >= REG_D0 && name <= REG_D7;
}

bool isAddrReg(RegName name) {
    return name >= REG_A0 && name <= REG_A7;
}

bool isGeneralReg(RegName name) {
    return name >= REG_D0 && name <= REG_A7;
}

bool isFloatReg(RegName name) {
    return name >= REG_FP0 && name <= REG_FP7;
}

bool isFloatControlReg(RegName name) {
    return name >= REG_FPCR && name <= REG_FPIAR;
}

Config::opcode_t encodeGeneralRegNo(RegName name) {
    return int8_t(name) & 7;
}

uint_fast8_t encodeGeneralRegPos(RegName name) {
    return uint_fast8_t(name);
}

uint_fast8_t encodeFloatRegPos(RegName name) {
    return REG_FP7 - int8_t(name);
}

uint_fast8_t encodeFloatControlRegPos(RegName name) {
    return REG_FPIAR - int_fast8_t(name) + 10;
}

RegName decodeGeneralReg(uint_fast8_t regno) {
    return RegName(regno & 0xF);
}

RegName decodeDataReg(uint_fast8_t regno) {
    return RegName(regno & 7);
}

RegName decodeAddrReg(uint_fast8_t regno) {
    return RegName((regno & 7) + 8);
}

CntlReg parseCntlReg(StrScanner &scan, const ValueParser &parser, PmmuType pmmuType) {
    auto p = scan;
    const auto *entry = CREG_TABLE.searchText(parser.readRegName(p));
    if (entry) {
        const auto name = CntlReg(entry->name());
        if (pmmuType == PMMU_MC68851 || pmmuType == PMMU_MC68030) {
            if (name == CREG_TC || name == CREG_SRP)
                return CREG_UNDEF;  // those are PREG_TC, PREG_SRP
        }
        scan = p;
        return name;
    }
    return CREG_UNDEF;
}

StrBuffer &outCntlReg(StrBuffer &out, CntlReg name) {
    const auto *entry = CREG_TABLE.searchName(name);
    return entry ? entry->outText(out) : out;
}

bool validCntlReg(CntlReg name, const CpuSpec &cpuSpec) {
    if (name >= CREG_CACR && cpuSpec.cpu == MC68010)
        return false;
    if (name >= CREG_TC && cpuSpec.cpu < MC68040)
        return false;
    if (cpuSpec.cpu == MC68040) {
        if (name == CREG_CAAR)
            return false;
        if (name <= CREG_ISP)
            return true;
        return (cpuSpec.pmmu == PMMU_MC68040) ? name <= CREG_SRP : name >= CREG_IACR0;
    }
    return true;
}

Config::opcode_t encodeCntlRegNo(CntlReg name) {
    static constexpr Config::opcode_t CREGNO[] = {
            0x000,  // CREG_SFC = 0,     // MC68010/MC68020/MC68030/MC68040/MC68851
            0x001,  // CREG_DFC = 1,     // MC68010/MC68020/MC68030/MC68040/MC68851
            0x800,  // CREG_USP = 2,     // MC68010/MC68020/MC68030/MC68040
            0x801,  // CREG_VBR = 3,     // MC68010/MC68020/MC68030/MC68040
            0x002,  // CREG_CACR = 4,    // MC68020/MC68030/MC68040
            0x802,  // CREG_CAAR = 5,    // MC68020/MC68030/MC68040
            0x803,  // CREG_MSP = 6,     // MC68020/MC68030/MC68040
            0x804,  // CREG_ISP = 7,     // MC68020/MC68030/MC68040
            0x003,  // CREG_TC = 8,      // MC68040
            0x004,  // CREG_ITT0 = 9,    // MC68040
            0x005,  // CREG_ITT1 = 10,   // MC68040
            0x006,  // CREG_DTT0 = 11,   // MC68040
            0x007,  // CREG_DTT1 = 12,   // MC68040
            0x805,  // CREG_MMUSR = 13,  // MC68040
            0x806,  // CREG_URP = 14,    // MC68040
            0x807,  // CREG_SRP = 15,    // MC68040
            0x004,  // CREG_IACR0 = 16,  // MC68EC040
            0x005,  // CREG_IACR1 = 17,  // MC68EC040
            0x006,  // CREG_DACR0 = 18,  // MC68EC040
            0x007,  // CREG_DACR1 = 19,  // MC68EC040
    };
    return pgm_read_word(CREGNO + name);
}

CntlReg decodeCntlRegNo(Config::opcode_t regno, const CpuSpec &cpuSpec) {
    regno &= 0xFFF;
    if (regno < 8) {
        static constexpr CntlReg CREG_0xx[] PROGMEM = {
                CREG_SFC,   // 0x000
                CREG_DFC,   // 0x001
                CREG_CACR,  // 0x002
                CREG_TC,    // 0x003
                CREG_ITT0,  // 0x004
                CREG_ITT1,  // 0x005
                CREG_DTT0,  // 0x006
                CREG_DTT1,  // 0x007
        };
        if (regno >= 2 && cpuSpec.cpu == MC68010)
            return CREG_UNDEF;
        if (regno >= 3 && cpuSpec.cpu < MC68040)
            return CREG_UNDEF;
        if (regno >= 4 && cpuSpec.pmmu == PMMU_MC68EC040)
            return CntlReg(CREG_IACR0 + (regno - 4));
        return CntlReg(pgm_read_byte(CREG_0xx + regno));
    }
    if (regno >= 0x800 && regno < 0x808) {
        static constexpr CntlReg CREG_8xx[] PROGMEM = {
                CREG_USP,    // 0x800
                CREG_VBR,    // 0x801
                CREG_CAAR,   // 0x802
                CREG_MSP,    // 0x803
                CREG_ISP,    // 0x804
                CREG_MMUSR,  // 0x805
                CREG_URP,    // 0x806
                CREG_SRP,    // 0x807
        };
        if (regno >= 0x802 && cpuSpec.cpu == MC68010)
            return CREG_UNDEF;
        if (regno >= 0x805 && cpuSpec.cpu < MC68040)
            return CREG_UNDEF;
        if (regno == 0x802 && cpuSpec.cpu == MC68040)
            return CREG_UNDEF;
        if (regno >= 0x805 && cpuSpec.pmmu == PMMU_MC68EC040)
            return CREG_UNDEF;
        return CntlReg(pgm_read_byte(CREG_8xx + (regno - 0x800)));
    }
    return CREG_UNDEF;
}

#if !defined(LIBASM_MC68000_NOPMMU)
PmmuReg parsePmmuReg(StrScanner &scan, const ValueParser &parser) {
    auto p = scan;
    auto name = parser.readRegName(p);
    const auto *entry = PREG_TABLE.searchText(name);
    if (entry) {
        scan = p;
        return PmmuReg(entry->name());
    }
    auto reg = PREG_UNDEF;
    if (name.iexpectText_P(TEXT_REG_BAD, 3)) {
        reg = PREG_BAD0;
    } else if (name.iexpectText_P(TEXT_REG_BAC, 3)) {
        reg = PREG_BAC0;
    }
    if (reg != PREG_UNDEF && *name >= '0' && *name < '8' && name[1] == 0) {
        scan = p;
        return PmmuReg(reg + (*name - '0'));
    }
    return PREG_UNDEF;
}

StrBuffer &outPmmuReg(StrBuffer &out, PmmuReg name) {
    const auto *entry = PREG_TABLE.searchName(name);
    if (entry)
        return entry->outText(out);
    if (name >= PREG_BAC0)
        return out.text_P(TEXT_REG_BAC).letter(name - PREG_BAC0 + '0');
    if (name >= PREG_BAD0)
        return out.text_P(TEXT_REG_BAD).letter(name - PREG_BAD0 + '0');
    return out;
}

Config::opcode_t encodePmmuReg(PmmuReg name, PmmuType pmmu) {
    if (pmmu == PMMU_MC68851) {
        if (name < PREG_BAD0)
            return name << 10;
        if (name <= PREG_BAD7)
            return (12 << 10) | ((name - PREG_BAD0) << 2);
        if (name <= PREG_BAC7)
            return (13 << 10) | ((name - PREG_BAC0) << 2);
    } else if (pmmu == PMMU_MC68030) {
        if (name == PREG_TC || name == PREG_SRP || name == PREG_CRP || name == PREG_PSR)
            return (name + 0x10) << 10;
        if (name == PREG_TT0 || name == PREG_TT1)
            return (name - PREG_TT0 + 2) << 10;
    } else if (pmmu == PMMU_MC68EC030) {
        if (name == PREG_AC0)
            return 1 << 10;
        if (name == PREG_AC1)
            return 3 << 10;
        if (name == PREG_ACUSR)
            return 0x18 << 10;
    }
    return 0;
}

PmmuReg decodePmmuReg(Config::opcode_t post, PmmuType pmmu) {
    if (pmmu == PMMU_MC68851) {
        const auto regno = (post >> 10) & 0xF;
        if (regno < 10)
            return PmmuReg(regno);
        if (regno == 12)
            return PmmuReg(PREG_BAD0 + ((post >> 2) & 7));
        if (regno == 13)
            return PmmuReg(PREG_BAC0 + ((post >> 2) & 7));
    } else if (pmmu == PMMU_MC68030) {
        const auto regno = (post >> 10) & 0x1F;
        if (regno == 2 || regno == 3)
            return PmmuReg(PREG_TT0 + regno - 2);
        if (regno >= 0x10 && regno < 0x14 && regno != 0x11)
            return PmmuReg(PREG_TC + regno - 0x10);
        if (regno == 0x18)
            return PREG_PSR;
    } else if (pmmu == PMMU_MC68EC030) {
        const auto regno = (post >> 10) & 0x1F;
        if (regno == 1 || regno == 3)
            return PmmuReg(PREG_AC0 + (regno / 2));
        if (regno == 0x18)
            return PREG_ACUSR;
    }
    return PREG_UNDEF;
}

OprSize pmmuRegSize(PmmuReg name, PmmuType pmmu) {
    if (name == PREG_UNDEF)
        return SZ_NONE;
    if (pmmu == PMMU_MC68851) {
        if (name > PREG_BAC7)
            return SZ_NONE;
        if (name >= PREG_BAD0)
            return SZ_WORD;
        static constexpr OprSize SIZE[] PROGMEM = {
                SZ_LONG,  // PREG_TC = 0
                SZ_QUAD,  // PREG_DRP = 1
                SZ_QUAD,  // PREG_SRP = 2
                SZ_QUAD,  // PREG_CRP = 3
                SZ_BYTE,  // PREG_CAL = 4
                SZ_BYTE,  // PREG_VAL = 5
                SZ_BYTE,  // PREG_SCC = 6
                SZ_WORD,  // PREG_AC = 7
                SZ_WORD,  // PREG_PSR = 8
                SZ_WORD,  // PREG_PCSR = 9
        };
        return OprSize(pgm_read_byte(SIZE + name));
    } else if (pmmu == PMMU_MC68030) {
        if (name == PREG_TT0 || name == PREG_TT1 || name == PREG_TC)
            return SZ_LONG;
        if (name == PREG_PSR)
            return SZ_WORD;
        if (name == PREG_SRP || name == PREG_CRP)
            return SZ_QUAD;
    } else if (pmmu == PMMU_MC68EC030) {
        if (name == PREG_AC0 || name == PREG_AC1)
            return SZ_LONG;
        if (name == PREG_ACUSR)
            return SZ_WORD;
    }
    return SZ_NONE;
}
#endif

InsnSize parseSize(StrScanner &scan) {
    auto p = scan;
    if (p.expect('.')) {
        auto size = ISZ_ERROR;
        if (p.iexpect('B')) {
            size = ISZ_BYTE;
        } else if (p.iexpect('W')) {
            size = ISZ_WORD;
        } else if (p.iexpect('L')) {
            size = ISZ_LONG;
        } else if (p.iexpect('S')) {
            size = ISZ_SNGL;
        } else if (p.iexpect('D')) {
            size = ISZ_DUBL;
        } else if (p.iexpect('X')) {
            size = ISZ_XTND;
        } else if (p.iexpect('P')) {
            size = ISZ_PBCD;
#if !defined(LIBASM_MC68000_NOPMMU)
        } else if (p.iexpect('Q')) {
            size = ISZ_QUAD;
#endif
        }
        if (size == ISZ_ERROR || isIdLetter(*p))
            return ISZ_ERROR;
        scan = p;
        return size;
    }
    return ISZ_NONE;
}

InsnSize parseIndexSize(StrScanner &scan) {
    auto p = scan;
    const auto size = parseSize(p);
    if (size == ISZ_NONE || size == ISZ_WORD || size == ISZ_LONG) {
        scan = p;
        return size;
    }
    return ISZ_ERROR;
}

uint_fast8_t sizeNameLen(OprSize size) {
    return size == SZ_NONE ? 0 : 2;
}

char sizeSuffix(OprSize size) {
    switch (size) {
    case SZ_BYTE:
        return 'B';
    case SZ_WORD:
        return 'W';
    case SZ_LONG:
        return 'L';
#if !defined(LIBASM_MC68000_NOPMMU)
    case SZ_QUAD:
        return 'Q';
#endif
    case SZ_SNGL:
        return 'S';
    case SZ_XTND:
        return 'X';
    case SZ_DUBL:
        return 'D';
    case SZ_PBCD:
        return 'P';
    default:
        return 0;
    }
}

namespace {
constexpr uint8_t MODE_REG[] = {
        000,  // M_NONE
        000,  // M_DREG
        010,  // M_AREG
        020,  // M_AIND
        030,  // M_PINC
        040,  // M_PDEC
        050,  // M_DISP
        060,  // M_INDX
        074,  // M_IMDAT
        074,  // M_IMFLT
        070,  // M_AWORD
        071,  // M_ALONG
        072,  // M_PCDSP
        073,  // M_PCIDX
};

static const AddrMode REG2MODE[] = {
        M_AWORD,  // 0
        M_ALONG,  // 1
        M_PCDSP,  // 2
        M_PCIDX,  // 3
        M_IMDAT,  // 4
        M_NONE,   // 5
        M_NONE,   // 6
        M_NONE,   // 7
};
}  // namespace

uint_fast8_t encodeAddrMode(AddrMode mode) {
    if (mode <= M_PCIDX)
        return MODE_REG[mode] >> 3;
    return 0;
}

uint_fast8_t encodeRegNo(AddrMode mode, RegName reg) {
    if (mode >= M_DREG && mode <= M_INDX)
        return encodeGeneralRegNo(reg);
    if (mode <= M_PCIDX)
        return MODE_REG[mode] & 7;
    return 0;
}

AddrMode decodeAddrMode(uint_fast8_t mode, uint_fast8_t regno) {
    mode &= 7;
    if (mode < 7)
        return AddrMode(mode + M_DREG);
    regno &= 7;
    return REG2MODE[regno];
}

RegName decodeRegNo(uint_fast8_t mode, uint_fast8_t regno) {
    mode &= 7;
    regno &= 7;
    if (mode == 0)
        return decodeDataReg(regno);
    if (mode >= 1 && mode < 7)
        return decodeAddrReg(regno);
    return REG_UNDEF;
}

CacheName parseCacheName(StrScanner &scan, const ValueParser &parser) {
    auto p = scan;
    const auto *entry = CACHE_TABLE.searchText(parser.readRegName(p));
    if (entry) {
        scan = p;
        return CacheName(entry->name());
    }
    return CACHE_UNDEF;
}

StrBuffer &outCacheName(StrBuffer &out, CacheName name) {
    const auto *entry = CACHE_TABLE.searchName(name);
    return entry ? entry->outText(out) : out;
}

Config::opcode_t encodeCacheNum(CacheName name) {
    return (static_cast<Config::opcode_t>(name) & 3) << 6;
}

CacheName decodeCacheName(Config::opcode_t opc) {
    return CacheName((opc >> 6) & 3);
}

}  // namespace reg
}  // namespace mc68000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

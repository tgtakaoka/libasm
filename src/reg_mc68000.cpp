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
    { TEXT_REG_DFC,   REG_DFC   },
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
    { TEXT_REG_SFC,   REG_SFC   },
    { TEXT_REG_SR,    REG_SR    },
    { TEXT_REG_USP,   REG_USP   },
    { TEXT_REG_VBR,   REG_VBR   },
};

PROGMEM constexpr NameTable TABLE{ARRAY_RANGE(REG_ENTRIES)};

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

bool isControlReg(RegName name) {
    return name >= REG_SFC;
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

Config::opcode_t encodeControlRegNo(RegName name) {
    switch (name) {
    case REG_SFC:
        return 0x000;
    case REG_DFC:
        return 0x001;
    case REG_USP:
        return 0x800;
    case REG_VBR:
        return 0x801;
    default:
        return 0;
    }
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

RegName decodeControlReg(Config::opcode_t regno) {
    switch (regno & Entry::Flags::postMask(EX_RC)) {
    case 0x000:
        return REG_SFC;
    case 0x001:
        return REG_DFC;
    case 0x800:
        return REG_USP;
    case 0x801:
        return REG_VBR;
    default:
        return REG_UNDEF;
    }
}

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
    case SZ_SNGL:
        return 'S';
    case SZ_DUBL:
        return 'D';
    case SZ_XTND:
        return 'X';
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

}  // namespace reg
}  // namespace mc68000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

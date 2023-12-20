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

using namespace libasm::reg;
using namespace libasm::text::mc68000;

namespace libasm {
namespace mc68000 {
namespace reg {

namespace {
// clang-format off

constexpr NameEntry REG_ENTRIES[] PROGMEM = {
    { TEXT_REG_A0,    REG_A0  },
    { TEXT_REG_A1,    REG_A1  },
    { TEXT_REG_A2,    REG_A2  },
    { TEXT_REG_A3,    REG_A3  },
    { TEXT_REG_A4,    REG_A4  },
    { TEXT_REG_A5,    REG_A5  },
    { TEXT_REG_A6,    REG_A6  },
    { TEXT_REG_A7,    REG_A7  },
    { TEXT_REG_CCR,   REG_CCR },
    { TEXT_REG_D0,    REG_D0  },
    { TEXT_REG_D1,    REG_D1  },
    { TEXT_REG_D2,    REG_D2  },
    { TEXT_REG_D3,    REG_D3  },
    { TEXT_REG_D4,    REG_D4  },
    { TEXT_REG_D5,    REG_D5  },
    { TEXT_REG_D6,    REG_D6  },
    { TEXT_REG_D7,    REG_D7  },
    { TEXT_REG_FP0,   REG_FP0 },
    { TEXT_REG_FP1,   REG_FP1 },
    { TEXT_REG_FP2,   REG_FP2 },
    { TEXT_REG_FP3,   REG_FP3 },
    { TEXT_REG_FP4,   REG_FP4 },
    { TEXT_REG_FP5,   REG_FP5 },
    { TEXT_REG_FP6,   REG_FP6 },
    { TEXT_REG_FP7,   REG_FP7 },
    { TEXT_REG_FPCR,  REG_FPCR },
    { TEXT_REG_FPIAR, REG_FPIAR },
    { TEXT_REG_FPSR,  REG_FPSR },
    { TEXT_REG_PC,    REG_PC  },
    { TEXT_REG_SR,    REG_SR  },
    { TEXT_REG_USP,   REG_USP },
};

PROGMEM constexpr NameTable TABLE{ARRAY_RANGE(REG_ENTRIES)};

// clang-format on
}  // namespace

RegName parseRegName(StrScanner &scan) {
    const auto *entry = TABLE.searchText(scan);
    return entry ? RegName(entry->name()) : REG_UNDEF;
}

StrBuffer &outRegName(StrBuffer &out, RegName name) {
    const auto *entry = TABLE.searchName(name);
    return entry ? entry->outText(out) : out;
}

bool isDataReg(RegName name) {
    const auto num = int8_t(name);
    return num >= REG_D0 && num <= REG_D7;
}

bool isAddrReg(RegName name) {
    const auto num = int8_t(name);
    return num >= REG_A0 && num <= REG_A7;
}

bool isGeneralReg(RegName name) {
    const auto num = int8_t(name);
    return num >= REG_D0 && num <= REG_A7;
}

bool isFloatReg(RegName name) {
    const auto num = int8_t(name);
    return num >= REG_FP0 && num <= REG_FP7;
}

bool isFloatControlReg(RegName name) {
    const auto num = int8_t(name);
    return num >= REG_FPCR && num <= REG_FPIAR;
}

Config::opcode_t encodeGeneralRegNo(RegName name) {
    return int8_t(name) & 7;
}

Config::opcode_t encodeFloatRegNo(RegName name) {
    return int8_t(name) & 7;
}

uint8_t encodeGeneralRegPos(RegName name) {
    return uint8_t(name);
}

uint8_t encodeFloatRegPos(RegName name) {
    return REG_FP7 - int8_t(name);
}

uint8_t encodeFloatControlRegPos(RegName name) {
    return REG_FPIAR - int8_t(name) + 10;
}

RegName decodeGeneralReg(uint8_t regno) {
    return RegName(regno & 0xF);
}

RegName decodeDataReg(uint8_t regno) {
    return RegName(regno & 7);
}

RegName decodeAddrReg(uint8_t regno) {
    return RegName((regno & 7) + 8);
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

uint8_t sizeNameLen(OprSize size) {
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

}  // namespace reg

Config::opcode_t EaMc68000::encodeMode(AddrMode mode) {
    if (mode == M_IMFLT)
        mode = M_IMDAT;
    const auto m = static_cast<uint8_t>(mode);
    return m >= 8 ? 7 : m;
}

Config::opcode_t EaMc68000::encodeRegNo(AddrMode mode, RegName reg) {
    if (mode == M_IMFLT)
        mode = M_IMDAT;
    const auto m = static_cast<uint8_t>(mode);
    if (m < 8)
        return reg::encodeGeneralRegNo(reg);
    return m - 8;
}

EaMc68000::EaMc68000(OprSize size_, uint8_t raw_mode, uint8_t regno) {
    regno &= 7;
    size = size_;
    if ((raw_mode &= 7) == 7) {
        mode = (regno < 5) ? AddrMode(regno + 8) : M_ERROR;
        reg = REG_UNDEF;
    } else {
        mode = AddrMode(raw_mode);
        reg = (mode == M_DREG) ? reg::decodeDataReg(regno) : reg::decodeAddrReg(regno);
    }
}

OprSize BriefExt::indexSize() const {
    return (word & 0x800) ? SZ_LONG : SZ_WORD;
}

RegName BriefExt::index() const {
    return reg::decodeGeneralReg(word >> 12);
}

uint8_t BriefExt::disp() const {
    return static_cast<uint8_t>(word);
}

}  // namespace mc68000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

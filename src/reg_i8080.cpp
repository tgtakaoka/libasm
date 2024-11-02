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

#include "reg_i8080.h"

#include "reg_base.h"
#include "text_i8080.h"

using namespace libasm::text::i8080;
using namespace libasm::reg;

namespace libasm {
namespace i8080 {
namespace reg {

namespace {
// clang-format off

constexpr NameEntry INTEL_ENTRIES[] PROGMEM = {
    { TEXT_REG_A,   REG_A   },
    { TEXT_REG_B,   REG_B   },
    { TEXT_REG_C,   REG_C   },
    { TEXT_REG_D,   REG_D   },
    { TEXT_REG_E,   REG_E   },
    { TEXT_REG_H,   REG_H   },
    { TEXT_REG_L,   REG_L   },
    { TEXT_REG_M,   REG_M   },
    { TEXT_REG_PSW, REG_PSW },
    { TEXT_REG_SP,  REG_SP  },
};

constexpr NameEntry ZILOG_ENTRIES[] PROGMEM = {
    { TEXT_REG_A,   REG_A   },
    { TEXT_REG_AF,  REG_AF  },
    { TEXT_REG_B,   REG_B   },
    { TEXT_REG_BC,  REG_BC  },
    { TEXT_REG_C,   REG_C   },
    { TEXT_REG_D,   REG_D   },
    { TEXT_REG_DE,  REG_DE  },
    { TEXT_REG_E,   REG_E   },
    { TEXT_REG_H,   REG_H   },
    { TEXT_REG_HL,  REG_HL  },
    { TEXT_REG_IM,  REG_IM  },
    { TEXT_REG_L,   REG_L   },
    { TEXT_REG_M,   REG_M   },
    { TEXT_REG_PSW, REG_PSW },
    { TEXT_REG_SP,  REG_SP  },
};

constexpr NameEntry CC_ENTRIES[] PROGMEM = {
    { TEXT_CC_C,  CC_C  },
    { TEXT_CC_M,  CC_M  },
    { TEXT_CC_NC, CC_NC },
    { TEXT_CC_NZ, CC_NZ },
    { TEXT_CC_P,  CC_P  },
    { TEXT_CC_PE, CC_PE },
    { TEXT_CC_PO, CC_PO },
    { TEXT_CC_Z,  CC_Z  },
};

PROGMEM constexpr NameTable INTEL_TABLE{ARRAY_RANGE(INTEL_ENTRIES)};
PROGMEM constexpr NameTable ZILOG_TABLE{ARRAY_RANGE(ZILOG_ENTRIES)};
PROGMEM constexpr NameTable CC_TABLE{ARRAY_RANGE(CC_ENTRIES)};

// clang-format on
}  // namespace

RegName parseRegName(StrScanner &scan, bool zilog) {
    const auto *entry = zilog ? ZILOG_TABLE.searchText(scan) : INTEL_TABLE.searchText(scan);
    return entry ? RegName(entry->name()) : REG_UNDEF;
}

StrBuffer &outRegName(StrBuffer &out, RegName name, bool indirect) {
    const auto *entry = ZILOG_TABLE.searchName(name);
    if (entry) {
        if (indirect)
            out.letter('(');
        entry->outText(out);
        if (indirect)
            out.letter(')');
    }
    return out;
}

bool isPointerReg(RegName name) {
    switch (name) {
    case REG_B:
    case REG_D:
    case REG_H:
    case REG_SP:
        return true;
    default:
        return false;
    }
}

uint8_t encodePointerReg(RegName name) {
    if (name >= REG_BC)
        return uint8_t(name) - 8;;
    return (uint8_t(name) >> 1);
}

bool isStackReg(RegName name) {
    switch (name) {
    case REG_B:
    case REG_D:
    case REG_H:
    case REG_PSW:
        return true;
    default:
        return false;
    }
}

uint8_t encodeStackReg(RegName name) {
    if (name == REG_PSW || name == REG_AF)
        return 3;
    if (name >= REG_BC)
        return uint8_t(name) - 8;
    return (uint8_t(name) >> 1);
}

bool isIndexReg(RegName name) {
    return name == REG_B || name == REG_D;
}

uint8_t encodeIndexReg(RegName name) {
    if (name >= REG_BC)
        return uint8_t(name) - 8;
    return (uint8_t(name) >> 1);
}

bool isDataReg(RegName name) {
    const auto num = int8_t(name);
    return num >= 0 && num < 8;
}

uint8_t encodeDataReg(RegName name) {
    if (name == REG_HL)
        name = REG_M;  // (HL)
    return uint8_t(name);
}

RegName decodePointerReg(uint8_t num, bool zilog) {
    num &= 3;
    if (zilog)
        return RegName(num + REG_BC);
    return num == 3 ? REG_SP : RegName(num * 2 + REG_B);
}

RegName decodeStackReg(uint8_t num, bool zilog) {
    num &= 3;
    if (num == 3)
        return zilog ? REG_AF : REG_PSW;
    return zilog ? RegName(num + REG_BC) : RegName(num * 2 + REG_B);
}

RegName decodeIndexReg(uint8_t num, bool zilog) {
    if (zilog) {
        return RegName((num & 1) + REG_BC);
    } else {
        return RegName((num & 1) << 1);
    }
}

RegName decodeDataReg(uint8_t num) {
    return RegName(num & 7);
}

CcName parseCcName(StrScanner &scan) {
    const auto *entry = CC_TABLE.searchText(scan);
    return entry ? CcName(entry->name()) : CC_UNDEF;
}

StrBuffer &outCcName(StrBuffer &out, const CcName name) {
    const auto *entry = CC_TABLE.searchName(name);
    return entry ? entry->outText(out) : out;
}

uint8_t encodeCcName(const CcName name) {
    return uint8_t(name);
}

CcName decodeCcName(uint8_t num) {
    return CcName(num & 7);
}

}  // namespace reg
}  // namespace i8080
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

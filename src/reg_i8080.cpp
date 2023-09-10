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

constexpr NameEntry REG_ENTRIES[] PROGMEM = {
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
    if (name == REG_SP)
        return 3;
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
    if (name == REG_PSW)
        return 3;
    return (uint8_t(name) >> 1);
}

bool isIndexReg(RegName name) {
    return name == REG_B || name == REG_D;
}

uint8_t encodeIndexReg(RegName name) {
    return (uint8_t(name) >> 1);
}

bool isDataReg(RegName name) {
    const auto num = int8_t(name);
    return num >= 0 && num < 8;
}

uint8_t encodeDataReg(RegName name) {
    return uint8_t(name);
}

RegName decodePointerReg(uint8_t num) {
    switch (num & 3) {
    case 0:
        return REG_B;
    case 1:
        return REG_D;
    case 2:
        return REG_H;
    default:
        return REG_SP;
    }
}

RegName decodeStackReg(uint8_t num) {
    switch (num & 3) {
    case 0:
        return REG_B;
    case 1:
        return REG_D;
    case 2:
        return REG_H;
    default:
        return REG_PSW;
    }
}

RegName decodeIndexReg(uint8_t num) {
    return RegName((num & 1) << 1);
}

RegName decodeDataReg(uint8_t num) {
    return RegName(num & 7);
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

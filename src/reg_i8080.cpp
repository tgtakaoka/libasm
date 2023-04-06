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

#include "text_i8080.h"

using namespace libasm::text::i8080;

namespace libasm {
namespace i8080 {

static constexpr RegBase::NameEntry REG_TABLE[] PROGMEM = {
        NAME_ENTRY(REG_A),
        NAME_ENTRY(REG_B),
        NAME_ENTRY(REG_C),
        NAME_ENTRY(REG_D),
        NAME_ENTRY(REG_E),
        NAME_ENTRY(REG_H),
        NAME_ENTRY(REG_L),
        NAME_ENTRY(REG_M),
        NAME_ENTRY(REG_SP),
        NAME_ENTRY(REG_PSW),
};

RegName RegI8080::parseRegName(StrScanner &scan) {
    const auto *entry = searchText(scan, ARRAY_RANGE(REG_TABLE));
    return entry ? RegName(entry->name()) : REG_UNDEF;
}

StrBuffer &RegI8080::outRegName(StrBuffer &out, const RegName name) const {
    const auto *entry = searchName(uint8_t(name), ARRAY_RANGE(REG_TABLE));
    if (entry)
        out.text_P(entry->text_P(), isUppercase());
    return out;
}

bool RegI8080::isPointerReg(RegName name) {
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

uint8_t RegI8080::encodePointerReg(RegName name) {
    if (name == REG_SP)
        return 3;
    return (uint8_t(name) >> 1);
}

bool RegI8080::isStackReg(RegName name) {
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

uint8_t RegI8080::encodeStackReg(RegName name) {
    if (name == REG_PSW)
        return 3;
    return (uint8_t(name) >> 1);
}

bool RegI8080::isIndexReg(RegName name) {
    return name == REG_B || name == REG_D;
}

uint8_t RegI8080::encodeIndexReg(RegName name) {
    return (uint8_t(name) >> 1);
}

bool RegI8080::isDataReg(RegName name) {
    const auto num = int8_t(name);
    return num >= 0 && num < 8;
}

uint8_t RegI8080::encodeDataReg(RegName name) {
    return uint8_t(name);
}

RegName RegI8080::decodePointerReg(uint8_t num) {
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

RegName RegI8080::decodeStackReg(uint8_t num) {
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

RegName RegI8080::decodeIndexReg(uint8_t num) {
    return RegName((num & 1) << 1);
}

RegName RegI8080::decodeDataReg(uint8_t num) {
    return RegName(num & 7);
}

}  // namespace i8080
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

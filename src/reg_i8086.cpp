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

#include "reg_i8086.h"
#include "reg_base.h"
#include "text_i8086.h"
#include "value_parser.h"

using namespace libasm::reg;
using namespace libasm::text::i8086;

namespace libasm {
namespace i8086 {
namespace reg {

namespace {
// clang-format off

constexpr NameEntry REG_ENTRIES[] PROGMEM = {
    { TEXT_REG_AH, REG_AH },
    { TEXT_REG_AL, REG_AL },
    { TEXT_REG_AX, REG_AX },
    { TEXT_REG_BH, REG_BH },
    { TEXT_REG_BL, REG_BL },
    { TEXT_REG_BP, REG_BP },
    { TEXT_REG_BX, REG_BX },
    { TEXT_REG_CH, REG_CH },
    { TEXT_REG_CL, REG_CL },
    { TEXT_REG_CS, REG_CS },
    { TEXT_REG_CX, REG_CX },
    { TEXT_REG_DH, REG_DH },
    { TEXT_REG_DI, REG_DI },
    { TEXT_REG_DL, REG_DL },
    { TEXT_REG_DS, REG_DS },
    { TEXT_REG_DX, REG_DX },
    { TEXT_REG_ES, REG_ES },
    { TEXT_REG_SI, REG_SI },
    { TEXT_REG_SP, REG_SP },
    { TEXT_REG_SS, REG_SS },
    { TEXT_REG_ST, REG_ST },
};

PROGMEM constexpr NameTable REG_TABLE{ARRAY_RANGE(REG_ENTRIES)};

constexpr NameEntry PRE_ENTRIES[] PROGMEM = {
    { TEXT_PRE_BYTE,  PRE_BYTE  },
    { TEXT_PRE_DWORD, PRE_DWORD },
    { TEXT_PRE_PTR,   PRE_PTR   },
    { TEXT_PRE_QWORD, PRE_QWORD },
    { TEXT_PRE_TBYTE, PRE_TBYTE },
    { TEXT_PRE_WORD,  PRE_WORD  },
};

PROGMEM constexpr NameTable PREFIX_TABLE{ARRAY_RANGE(PRE_ENTRIES)};

// clang-format on
}  // namespace

RegName parseRegName(StrScanner &scan, const ValueParser &parser) {
    auto p = scan;
    const auto *entry = REG_TABLE.searchText(parser.readRegName(p));
    if (entry) {
        scan = p;
        return RegName(entry->name());
    }
    return REG_UNDEF;
}

StrBuffer &outRegName(StrBuffer &out, RegName name) {
    const auto *entry = REG_TABLE.searchName(name);
    return entry ? entry->outText(out) : out;
}

RegName decodeByteReg(uint8_t num) {
    return RegName((num & 7) + 8);
}

RegName decodeWordReg(uint8_t num) {
    return RegName(num & 7);
}

RegName decodeSegReg(uint8_t num) {
    return RegName((num & 3) + 16);
}

bool isGeneralReg(RegName name) {
    const auto num = int8_t(name);
    return num >= 0 && num < 16;
}

bool isSegmentReg(RegName name) {
    const auto num = int8_t(name);
    return num >= 16 && num < 20;
}

OprSize generalRegSize(RegName name) {
    const auto num = int8_t(name);
    return num < 8 ? SZ_WORD : SZ_BYTE;
}

uint8_t encodeRegNum(RegName name) {
    const auto num = uint8_t(name);
    if (num < 8)
        return num;
    if (num < 16)
        return num - 8;
    return num - 16;
}

PrefixName parsePrefixName(StrScanner &scan, const ValueParser &parser) {
    auto p = scan;
    const auto *entry = PREFIX_TABLE.searchText(parser.readRegName(p));
    if (entry) {
        scan = p;
        return PrefixName(entry->name());
    }
    return PRE_UNDEF;
}

StrBuffer &outPrefixName(StrBuffer &out, PrefixName name) {
    const auto *entry = PREFIX_TABLE.searchName(name);
    return entry ? entry->outText(out) : out;
}

}  // namespace reg
}  // namespace i8086
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

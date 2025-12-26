/*
 * Copyright 2025 Tadashi G. Takaoka
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

#include "reg_tms320f.h"
#include "reg_base.h"
#include "str_buffer.h"
#include "str_scanner.h"
#include "text_tms320f.h"
#include "value_parser.h"

using namespace libasm::reg;
using namespace libasm::text::tms320f;

namespace libasm {
namespace tms320f {
namespace reg {

namespace {
// clang-format off
constexpr NameEntry REG_ENTRIES[] PROGMEM = {
    { TEXT_REG_BK,  REG_BK  },
    { TEXT_REG_DP,  REG_DP  },
    { TEXT_REG_IE,  REG_IE  },
    { TEXT_REG_IF,  REG_IF  },
    { TEXT_REG_IOF, REG_IOF },
    { TEXT_REG_IR0, REG_IR0 },
    { TEXT_REG_IR1, REG_IR1 },
    { TEXT_REG_RC,  REG_RC  },
    { TEXT_REG_RE,  REG_RE  },
    { TEXT_REG_RS,  REG_RS  },
    { TEXT_REG_SP,  REG_SP  },
    { TEXT_REG_ST,  REG_ST  },
};

PROGMEM constexpr NameTable REG_TABLE{ARRAY_RANGE(REG_ENTRIES)};
// clang-format on
}  // namespace

RegName parseRegName(StrScanner &scan, const ValueParser &parser) {
    auto p = scan;
    if (p.iexpectText_P(TEXT_REG_R)) {
        const auto num = parseRegNumber(p);
        if (num >= 0 && num <= 27) {
            scan = p;
            return RegName(REG_R0 + num);
        }
        return REG_UNDEF;
    }
    if (p.iexpectText_P(TEXT_REG_AR)) {
        const auto num = parseRegNumber(p);
        if (num >= 0 && num <= 7) {
            scan = p;
            return RegName(REG_AR0 + num);
        }
        return REG_UNDEF;
    }
    const auto *entry = REG_TABLE.searchText(parser.readRegName(p));
    if (entry) {
        scan = p;
        return RegName(entry->name());
    }
    return REG_UNDEF;
}

StrBuffer &outRegName(StrBuffer &out, RegName name) {
    if (name >= REG_DP) {
        const auto *entry = REG_TABLE.searchName(name);
        return entry ? entry->outText(out) : out;
    }
    if (name >= REG_AR0)
        return out.text_P(TEXT_REG_AR).int16(name - REG_AR0);
    return out.text_P(TEXT_REG_R).int16(name);
}

uint_fast8_t encodeRegName(RegName name) {
    return int8_t(name);
}

RegName decodeRegName(uint_fast16_t num) {
    return num < _REG_MAX ? RegName(num) : REG_UNDEF;
}

}  // namespace reg
}  // namespace tms320f
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

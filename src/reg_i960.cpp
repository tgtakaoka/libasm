/*
 * Copyright 2026 Tadashi G. Takaoka
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

#include "reg_i960.h"

#include "reg_base.h"
#include "text_i960.h"

namespace libasm {
namespace i960 {
namespace reg {

using namespace libasm::reg;
using namespace text::i960;

namespace {

constexpr NameEntry NAMED_REGS[] PROGMEM = {
        {TEXT_REG_FP, REG_FP},
        {TEXT_REG_PFP, REG_PFP},
        {TEXT_REG_RIP, REG_RIP},
        {TEXT_REG_SP, REG_SP},
};

PROGMEM constexpr NameTable NAMED_REG_TABLE{ARRAY_RANGE(NAMED_REGS)};

}  // namespace

uint8_t parseRegName(StrScanner &scan, const ValueParser &parser) {
    auto p = scan;
    const auto symbol = parser.readRegName(p);
    const auto *entry = NAMED_REG_TABLE.searchText(symbol);
    if (entry) {
        scan = p;
        return entry->name();
    }
    auto s = symbol;
    uint8_t base;
    if (s.iexpect('R')) {
        base = 0;
    } else if (s.iexpect('G')) {
        base = GLOBAL_BASE;
    } else {
        return NO_REG;
    }
    // Each bank holds sixteen registers, and the whole symbol has to be the
    // register name; g16 and g1x are symbols which merely start like one.
    const auto num = parseRegNumber(s);
    if (num < 0 || num >= GLOBAL_BASE || s.size())
        return NO_REG;
    scan = p;
    return base + static_cast<uint8_t>(num);
}

uint8_t parseFloatRegName(StrScanner &scan, const ValueParser &parser) {
    auto p = scan;
    auto symbol = parser.readRegName(p);
    if (!symbol.iexpect('F') || !symbol.iexpect('P'))
        return NO_REG;
    const auto num = parseRegNumber(symbol);
    if (num < 0 || num >= FLOAT_REGS || symbol.size())
        return NO_REG;
    scan = p;
    return static_cast<uint8_t>(num);
}

StrBuffer &outFloatRegName(StrBuffer &out, uint8_t regno) {
    return out.letter('F').letter('P').int16(regno);
}

StrBuffer &outRegName(StrBuffer &out, uint8_t regno) {
    // GNU as rejects r0, r1, r2 and g15 and takes only these four names for
    // them, so writing the name keeps the output assemblable.
    switch (regno) {
    case REG_PFP:
        return out.text_P(TEXT_REG_PFP);
    case REG_SP:
        return out.text_P(TEXT_REG_SP);
    case REG_RIP:
        return out.text_P(TEXT_REG_RIP);
    case REG_FP:
        return out.text_P(TEXT_REG_FP);
    default:
        break;
    }
    const auto global = regno >= GLOBAL_BASE;
    out.letter(global ? 'G' : 'R');
    return out.int16(global ? regno - GLOBAL_BASE : regno);
}

}  // namespace reg
}  // namespace i960
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

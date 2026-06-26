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

#include "reg_superh.h"

#include "reg_base.h"
#include "text_superh.h"
#include "value_parser.h"

using namespace libasm::reg;
using namespace libasm::text::superh;

namespace libasm {
namespace superh {
namespace reg {

namespace {

// clang-format off
// Special (non-numbered) registers, sorted alphabetically for binary search.
// The numbered families R0-R15, FR0-FR15 and DR0-DR14 are parsed/emitted as a
// prefix plus number instead of being enumerated here.
constexpr NameEntry REG_ENTRIES[] PROGMEM = {
    { TEXT_REG_A0,    REG_A0    },
    { TEXT_REG_DSR,   REG_DSR   },
    { TEXT_REG_FPSCR, REG_FPSCR },
    { TEXT_REG_FPUL,  REG_FPUL  },
    { TEXT_REG_GBR,   REG_GBR   },
    { TEXT_REG_MACH,  REG_MACH  },
    { TEXT_REG_MACL,  REG_MACL  },
    { TEXT_REG_MOD,   REG_MOD   },
    { TEXT_REG_PC,    REG_PC    },
    { TEXT_REG_PR,    REG_PR    },
    { TEXT_REG_RE,    REG_RE    },
    { TEXT_REG_RS,    REG_RS    },
    { TEXT_REG_SR,    REG_SR    },
    { TEXT_REG_TBR,   REG_TBR   },
    { TEXT_REG_VBR,   REG_VBR   },
    { TEXT_REG_X0,    REG_X0    },
    { TEXT_REG_X1,    REG_X1    },
    { TEXT_REG_Y0,    REG_Y0    },
    { TEXT_REG_Y1,    REG_Y1    },
};
// clang-format on

PROGMEM constexpr NameTable TABLE{ARRAY_RANGE(REG_ENTRIES)};

// Parse 0..15.  Returns -1 on failure (no digit, or value > 15).
int8_t parseRegNum(StrScanner &p) {
    if (!isdigit(*p))
        return -1;
    int8_t n = *p++ - '0';
    if (isdigit(*p))
        n = n * 10 + (*p++ - '0');
    return n > 15 ? -1 : n;
}

StrBuffer &outRegNum(StrBuffer &out, uint8_t num) {  // num = 0..15
    if (num >= 10)
        out.letter('1');
    return out.letter('0' + (num % 10));
}

}  // namespace

// Parse Rn/FRn (n=0..15) or DRn (n=0,2..14) as a prefix plus number; fall
// back to the special-register table (SR, GBR, FPUL, ...).
RegName parseRegName(StrScanner &scan, const ValueParser &parser) {
    auto p = scan;
    const auto token = parser.readRegName(p);
    if (token.size() == 0)
        return REG_UNDEF;
    auto t = token;
    auto base = REG_UNDEF;
    auto step = 1;  // DR registers step by 2 (even-numbered only)
    if (t.iexpect('F')) {
        if (t.iexpect('R'))
            base = REG_FR0;
    } else if (t.iexpect('D')) {
        if (t.iexpect('R')) {
            base = REG_DR0;
            step = 2;
        }
    } else if (t.iexpect('R')) {
        base = REG_R0;
    }
    if (base != REG_UNDEF) {
        const auto num = parseRegNum(t);
        if (num >= 0 && t.size() == 0 && (num % step) == 0) {
            scan = p;
            return RegName(base + num / step);
        }
    }
    // SP is an alias for R15 (stack pointer): accepted on input, emitted as R15.
    if (token.iequals_P(TEXT_REG_SP)) {
        scan = p;
        return REG_R15;
    }
    const auto *entry = TABLE.searchText(token);
    if (entry) {
        scan = p;
        return RegName(entry->name());
    }
    return REG_UNDEF;
}

StrBuffer &outRegName(StrBuffer &out, RegName name) {
    if (isGpr(name))
        return outRegNum(out.letter('R'), uint8_t(name - REG_R0));
    if (isFr(name))
        return outRegNum(out.letter('F').letter('R'), uint8_t(name - REG_FR0));
    if (isDr(name))
        return outRegNum(out.letter('D').letter('R'), uint8_t(name - REG_DR0) * 2);
    const auto *entry = TABLE.searchName(name);
    return entry ? entry->outText(out) : out;
}

RegName decodeRn(uint16_t word) {
    return RegName((word >> 8) & 0xF);
}

RegName decodeRm(uint16_t word) {
    return RegName((word >> 4) & 0xF);
}

RegName decodeFn(uint16_t word) {
    return RegName(REG_FR0 + ((word >> 8) & 0xF));
}

RegName decodeFm(uint16_t word) {
    return RegName(REG_FR0 + ((word >> 4) & 0xF));
}

RegName decodeDn(uint16_t word) {
    return RegName(REG_DR0 + ((word >> 9) & 0x7));
}

RegName decodeDm(uint16_t word) {
    return RegName(REG_DR0 + ((word >> 5) & 0x7));
}

uint8_t encodeRegNum(RegName r) {
    if (isFr(r))
        return static_cast<uint8_t>(r - REG_FR0) & 0x0F;
    if (isDr(r))
        return static_cast<uint8_t>(r - REG_DR0) & 0x07;  // 0..7
    return static_cast<uint8_t>(r) & 0x0F;
}

}  // namespace reg
}  // namespace superh
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

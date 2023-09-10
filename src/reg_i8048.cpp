/*
 * Copyright 2021 Tadashi G. Takaoka
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

#include "reg_i8048.h"

#include "reg_base.h"
#include "text_i8048.h"

using namespace libasm::text::i8048;
using namespace libasm::reg;

namespace libasm {
namespace i8048 {
namespace reg {

namespace {
// clang-format off

constexpr NameEntry REG_ENTRIES[] PROGMEM = {
    { TEXT_REG_A,     REG_A     },
    { TEXT_REG_BUS,   REG_BUS   },
    { TEXT_REG_C,     REG_C     },
    { TEXT_REG_CLK,   REG_CLK   },
    { TEXT_REG_CNT,   REG_CNT   },
    { TEXT_REG_F0,    REG_F0    },
    { TEXT_REG_F1,    REG_F1    },
    { TEXT_REG_I,     REG_I     },
    { TEXT_REG_MB0,   REG_MB0   },
    { TEXT_REG_MB1,   REG_MB1   },
    { TEXT_REG_P,     REG_P     },
    { TEXT_REG_P1,    REG_P1    },
    { TEXT_REG_P2,    REG_P2    },
    { TEXT_REG_P4,    REG_P4    },
    { TEXT_REG_P5,    REG_P5    },
    { TEXT_REG_P6,    REG_P6    },
    { TEXT_REG_P7,    REG_P7    },
    { TEXT_REG_PSW,   REG_PSW   },
    { TEXT_REG_R0,    REG_R0    },
    { TEXT_REG_R1,    REG_R1    },
    { TEXT_REG_R2,    REG_R2    },
    { TEXT_REG_R3,    REG_R3    },
    { TEXT_REG_R4,    REG_R4    },
    { TEXT_REG_R5,    REG_R5    },
    { TEXT_REG_R6,    REG_R6    },
    { TEXT_REG_R7,    REG_R7    },
    { TEXT_REG_RB0,   REG_RB0   },
    { TEXT_REG_RB1,   REG_RB1   },
    { TEXT_REG_T,     REG_T     },
    { TEXT_REG_TCNT,  REG_TCNT  },
    { TEXT_REG_TCNTI, REG_TCNTI },
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

bool isRReg(RegName name) {
    const auto num = int8_t(name);
    return num >= int8_t(REG_R0) && num <= int8_t(REG_R7);
}

bool isPort(RegName name) {
    const auto num = int8_t(name);
    return num >= int8_t(REG_P1) && num <= int8_t(REG_P7);
}

bool isTimer(RegName name) {
    const auto num = int8_t(name);
    return num >= int8_t(REG_T) && num <= int8_t(REG_TCNTI);
}

uint8_t encodeRRegName(RegName name) {
    return int8_t(name);
}

RegName decodeRRegNum(const uint8_t num) {
    return RegName(num & 7);
}

uint8_t encodePortName(RegName name) {
    return int8_t(name) - int8_t(REG_BUS);
}

RegName decodePortNum(const uint8_t num) {
    return RegName((num & 7) + int8_t(REG_BUS));
}

}  // namespace reg
}  // namespace i8048
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

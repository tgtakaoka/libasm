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

#include "reg_ns32000.h"
#include "reg_base.h"
#include "text_ns32000.h"
#include "value_parser.h"

using namespace libasm::reg;
using namespace libasm::text::ns32000;

namespace libasm {
namespace ns32000 {
namespace reg {

namespace {
// clang-format off

constexpr NameEntry REG_ENTRIES[] PROGMEM = {
    { TEXT_REG_EXT, REG_EXT },
    { TEXT_REG_F0,  REG_F0  },
    { TEXT_REG_F1,  REG_F1  },
    { TEXT_REG_F2,  REG_F2  },
    { TEXT_REG_F3,  REG_F3  },
    { TEXT_REG_F4,  REG_F4  },
    { TEXT_REG_F5,  REG_F5  },
    { TEXT_REG_F6,  REG_F6  },
    { TEXT_REG_F7,  REG_F7  },
    { TEXT_REG_FP,  REG_FP  },
    { TEXT_REG_PC,  REG_PC  },
    { TEXT_REG_R0,  REG_R0  },
    { TEXT_REG_R1,  REG_R1  },
    { TEXT_REG_R2,  REG_R2  },
    { TEXT_REG_R3,  REG_R3  },
    { TEXT_REG_R4,  REG_R4  },
    { TEXT_REG_R5,  REG_R5  },
    { TEXT_REG_R6,  REG_R6  },
    { TEXT_REG_R7,  REG_R7  },
    { TEXT_REG_SB,  REG_SB  },
    { TEXT_REG_SP,  REG_SP  },
    { TEXT_REG_TOS, REG_TOS },
};

constexpr NameEntry PREG_ENTRIES[] PROGMEM = {
    { TEXT_REG_FP,      PREG_FP      },
    { TEXT_REG_INTBASE, PREG_INTBASE },
    { TEXT_REG_MOD,     PREG_MOD     },
    { TEXT_REG_PSR,     PREG_PSR     },
    { TEXT_REG_SB,      PREG_SB      },
    { TEXT_REG_SP,      PREG_SP      },
    { TEXT_REG_UPSR,    PREG_UPSR    },
    { TEXT_REG_US,      PREG_UPSR    },
};

constexpr NameEntry MREG_ENTRIES[] PROGMEM = {
    { TEXT_REG_BCNT, MREG_BCNT },
    { TEXT_REG_BPR0, MREG_BPR0 },
    { TEXT_REG_BPR1, MREG_BPR1 },
    { TEXT_REG_EIA,  MREG_EIA  },
    { TEXT_REG_MSR,  MREG_MSR  },
    { TEXT_REG_PTB0, MREG_PTB0 },
    { TEXT_REG_PTB1, MREG_PTB1 },
};

constexpr NameEntry CONFIG_ENTRIES[] PROGMEM = {
    { TEXT_CONFIG_C, CONFIG_C },
    { TEXT_CONFIG_F, CONFIG_F },
    { TEXT_CONFIG_I, CONFIG_I },
    { TEXT_CONFIG_M, CONFIG_M },
};

PROGMEM constexpr NameTable REG_TABLE{ARRAY_RANGE(REG_ENTRIES)};
PROGMEM constexpr NameTable PREG_TABLE{ARRAY_RANGE(PREG_ENTRIES)};
PROGMEM constexpr NameTable MREG_TABLE{ARRAY_RANGE(MREG_ENTRIES)};
PROGMEM constexpr NameTable CONFIG_TABLE{ARRAY_RANGE(CONFIG_ENTRIES)};

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

RegName decodeRegName(uint8_t num, bool floating) {
    return RegName((num & 7) + (floating ? 8 : 0));
}

uint8_t encodeRegName(RegName name) {
    if (isFloat(name))
        return uint8_t(name) - 8;
    return uint8_t(name);
}

bool isGeneric(RegName name) {
    const auto num = int8_t(name);
    return num >= 0 && num < 8;
}

bool isFloat(RegName name) {
    const auto num = int8_t(name) - 8;
    return num >= 0 && num < 8;
}

bool isRegPair(RegName name) {
    const auto num = int8_t(name);
    return num >= 0 && num < 16 && num % 2 == 0;
}

PregName parsePregName(StrScanner &scan, const ValueParser &parser) {
    auto p = scan;
    const auto *entry = PREG_TABLE.searchText(parser.readRegName(p));
    if (entry) {
        scan = p;
        return PregName(entry->name());
    }
    return PREG_UNDEF;
}

StrBuffer &outPregName(StrBuffer &out, PregName name) {
    const auto *entry = PREG_TABLE.searchName(name);
    return entry ? entry->outText(out) : out;
}

PregName decodePregName(uint8_t num) {
    num &= 0xF;
    if (num >= 1 && num < 8)
        return PREG_UNDEF;
    if (num == 11 || num == 12)
        return PREG_UNDEF;
    return PregName(num);
}

uint8_t encodePregName(PregName name) {
    return uint8_t(name);
}

MregName parseMregName(StrScanner &scan, const ValueParser &parser) {
    auto p = scan;
    const auto *entry = MREG_TABLE.searchText(parser.readRegName(p));
    if (entry) {
        scan = p;
        return MregName(entry->name());
    }
    return MREG_UNDEF;
}

StrBuffer &outMregName(StrBuffer &out, MregName name) {
    const auto *entry = MREG_TABLE.searchName(name);
    return entry ? entry->outText(out) : out;
}

MregName decodeMregName(uint8_t num) {
    num &= 0xF;
    if (num == 14 || (num >= 2 && num < 10))
        return MREG_UNDEF;
    return MregName(num);
}

uint8_t encodeMregName(MregName name) {
    return uint8_t(name);
}

ConfigName parseConfigName(StrScanner &scan, const ValueParser &parser) {
    auto p = scan;
    const auto *entry = CONFIG_TABLE.searchText(parser.readRegName(p));
    if (entry) {
        scan = p;
        return ConfigName(entry->name());
    }
    return CONFIG_UNDEF;
}

StrBuffer &outConfigNames(StrBuffer &out, uint8_t configs) {
    out.letter('[');
    char sep = 0;
    for (int8_t mask = 0x01; mask < 0x10; mask <<= 1) {
        if (configs & mask) {
            if (sep)
                out.letter(sep);
            const auto *entry = CONFIG_TABLE.searchName(mask);
            entry->outText(out);
            sep = ',';
        }
    }
    return out.letter(']');
}

StrOptName parseStrOptName(StrScanner &scan) {
    auto p = scan;
    auto strOpt = STROPT_UNDEF;
    if (p.iexpect('B')) {
        strOpt = STROPT_B;
    } else if (p.iexpect('W')) {
        strOpt = STROPT_W;
    } else if (p.iexpect('U')) {
        strOpt = STROPT_U;
    }
    if (strOpt == STROPT_UNDEF || isIdLetter(*p))
        return STROPT_UNDEF;
    scan = p;
    return strOpt;
}

OprSize parseIndexSize(StrScanner &scan) {
    auto p = scan;
    auto size = SZ_NONE;
    if (p.iexpect('B')) {
        size = SZ_BYTE;
    } else if (p.iexpect('W')) {
        size = SZ_WORD;
    } else if (p.iexpect('D')) {
        size = SZ_QUAD;
    } else if (p.iexpect('Q')) {
        size = SZ_OCTA;
    }
    if (size == SZ_NONE || isIdLetter(*p))
        return SZ_NONE;
    scan = p;
    return size;
}

OprSize decodeIndexSize(uint8_t num) {
    static constexpr OprSize sizes[] = {SZ_BYTE, SZ_WORD, SZ_QUAD, SZ_OCTA};
    return sizes[num & 3];
}

char indexSizeChar(OprSize size) {
    switch (size) {
    case SZ_BYTE:
        return 'B';
    case SZ_WORD:
        return 'W';
    case SZ_QUAD:
        return 'D';
    case SZ_OCTA:
        return 'Q';
    default:
        return 0;
    }
}

}  // namespace reg
}  // namespace ns32000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

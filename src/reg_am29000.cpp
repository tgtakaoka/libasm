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

#include "reg_am29000.h"

#include "reg_base.h"
#include "text_am29000.h"

using namespace libasm::reg;
using namespace libasm::text::am29000;

namespace libasm {
namespace am29000 {
namespace reg {

namespace {

// clang-format off
constexpr NameEntry SPREG_ENTRIES[] PROGMEM = {
    { TEXT_REG_ALU,  REG_ALU  },
    { TEXT_REG_BP,   REG_BP   },
    { TEXT_REG_CFG,  REG_CFG  },
    { TEXT_REG_CHA,  REG_CHA  },
    { TEXT_REG_CHC,  REG_CHC  },
    { TEXT_REG_CHD,  REG_CHD  },
    { TEXT_REG_CPS,  REG_CPS  },
    { TEXT_REG_CR,   REG_CR   },
    { TEXT_REG_FC,   REG_FC   },
    { TEXT_REG_FPE,  REG_FPE  },
    { TEXT_REG_FPS,  REG_FPS  },
    { TEXT_REG_INTE, REG_INTE },
    { TEXT_REG_IPA,  REG_IPA  },
    { TEXT_REG_IPB,  REG_IPB  },
    { TEXT_REG_IPC,  REG_IPC  },
    { TEXT_REG_OPS,  REG_OPS  },
    { TEXT_REG_PC0,  REG_PC0  },
    { TEXT_REG_PC1,  REG_PC1  },
    { TEXT_REG_PC2,  REG_PC2  },
    { TEXT_REG_Q,    REG_Q    },
    { TEXT_REG_RBP,  REG_RBP  },
    { TEXT_REG_TMC,  REG_TMC  },
    { TEXT_REG_TMR,  REG_TMR  },
    { TEXT_REG_VAB,  REG_VAB  },
};

// The memory management unit; the Am29200 and Am29205 do not implement these
// register numbers.
constexpr NameEntry MMU_SPREG_ENTRIES[] PROGMEM = {
    { TEXT_REG_LRU,  REG_LRU  },
    { TEXT_REG_MMU,  REG_MMU  },
};

// Region mapping, shadow program counters, instruction breakpoints and the
// exception opcode, which the Am29050 alone implements.
constexpr NameEntry AM29050_SPREG_ENTRIES[] PROGMEM = {
    { TEXT_REG_EXOP, REG_EXOP },
    { TEXT_REG_IBA0, REG_IBA0 },
    { TEXT_REG_IBA1, REG_IBA1 },
    { TEXT_REG_IBC0, REG_IBC0 },
    { TEXT_REG_IBC1, REG_IBC1 },
    { TEXT_REG_RMA0, REG_RMA0 },
    { TEXT_REG_RMA1, REG_RMA1 },
    { TEXT_REG_RMC0, REG_RMC0 },
    { TEXT_REG_RMC1, REG_RMC1 },
    { TEXT_REG_RSN,  REG_RSN  },
    { TEXT_REG_SPC0, REG_SPC0 },
    { TEXT_REG_SPC1, REG_SPC1 },
    { TEXT_REG_SPC2, REG_SPC2 },
};

// The cache interface.  The Am29200 has no cache, and the Am29050 does not
// expose one through a special-purpose register.
constexpr NameEntry CACHE_SPREG_ENTRIES[] PROGMEM = {
    { TEXT_REG_CDR,  REG_CDR  },
    { TEXT_REG_CIR,  REG_CIR  },
};

// Special-purpose register numbers, indexed by RegName.
constexpr uint8_t SPREG_NUMBERS[] PROGMEM = {
      0,   1,   2,   3,   4,   5,   6,   7,     // VAB  OPS  CPS  CFG  CHA  CHD  CHC  RBP
      8,   9,  10,  11,  12,  13,  14,  15,     // TMC  TMR  PC0  PC1  PC2  MMU  LRU  RSN
     16,  17,  18,  19,  20,  21,  22,  23,     // RMA0 RMC0 RMA1 RMC1 SPC0 SPC1 SPC2 IBA0
     24,  25,  26, 164,  29,  30, 128, 129,     // IBC0 IBA1 IBC1 EXOP CIR  CDR  IPC  IPA
    130, 131, 132, 133, 134, 135, 160, 161,     // IPB  Q    ALU  BP   FC   CR   FPE  INTE
    162,                                        // FPS
};
// clang-format on

PROGMEM constexpr NameTable SPREGS{ARRAY_RANGE(SPREG_ENTRIES)};
PROGMEM constexpr NameTable MMU_SPREGS{ARRAY_RANGE(MMU_SPREG_ENTRIES)};
PROGMEM constexpr NameTable AM29050_SPREGS{ARRAY_RANGE(AM29050_SPREG_ENTRIES)};
PROGMEM constexpr NameTable CACHE_SPREGS{ARRAY_RANGE(CACHE_SPREG_ENTRIES)};

bool isMmuReg(RegName name) {
    return name >= REG_MMU && name <= REG_LRU;
}

bool isAm29050Reg(RegName name) {
    return name >= REG_RSN && name <= REG_EXOP;
}

bool isCacheReg(RegName name) {
    return name >= REG_CIR && name <= REG_CDR;
}

}  // namespace

bool parseGenReg(StrScanner &scan, uint8_t &regno) {
    auto p = scan;
    uint8_t base;
    if (p.iexpect('G')) {
        base = 0;
    } else if (p.iexpect('L')) {
        base = LOCAL_BASE;
    } else {
        return false;
    }
    if (!p.iexpect('R'))
        return false;
    const auto num = parseRegNumber(p);
    if (num < 0 || num >= LOCAL_BASE)
        return false;
    if (isIdLetter(*p))
        return false;
    regno = base + static_cast<uint8_t>(num);
    scan = p;
    return true;
}

StrBuffer &outGenReg(StrBuffer &out, uint8_t regno) {
    const auto local = regno >= LOCAL_BASE;
    out.letter(local ? 'L' : 'G').letter('R');
    return out.int16(local ? regno - LOCAL_BASE : regno);
}

namespace {

// Whether |name| is implemented by |cpuType|.
bool hasSpReg(RegName name, CpuType cpuType) {
    if (isMmuReg(name))
        return cpuType != AM29200;
    if (isAm29050Reg(name))
        return cpuType == AM29050;
    if (isCacheReg(name))
        return cpuType != AM29200 && cpuType != AM29050;
    return true;
}

}  // namespace

RegName parseSpReg(StrScanner &scan, const ValueParser &parser, CpuType cpuType) {
    auto p = scan;
    const auto symbol = parser.readRegName(p);
    auto entry = SPREGS.searchText(symbol);
    if (entry == nullptr)
        entry = MMU_SPREGS.searchText(symbol);
    if (entry == nullptr)
        entry = AM29050_SPREGS.searchText(symbol);
    if (entry == nullptr)
        entry = CACHE_SPREGS.searchText(symbol);
    if (entry == nullptr)
        return REG_UNDEF;
    const auto name = RegName(entry->name());
    if (!hasSpReg(name, cpuType))
        return REG_UNDEF;
    scan = p;
    return name;
}

StrBuffer &outSpReg(StrBuffer &out, RegName name) {
    const auto *entry = isMmuReg(name)       ? MMU_SPREGS.searchName(name)
                        : isAm29050Reg(name) ? AM29050_SPREGS.searchName(name)
                        : isCacheReg(name)   ? CACHE_SPREGS.searchName(name)
                                             : SPREGS.searchName(name);
    if (entry != nullptr)
        entry->outText(out);
    return out;
}

RegName decodeSpReg(uint8_t number, CpuType cpuType) {
    for (const auto *p = ARRAY_BEGIN(SPREG_NUMBERS); p < ARRAY_END(SPREG_NUMBERS); p++) {
        if (pgm_read_byte(p) == number) {
            const auto i = static_cast<int8_t>(p - ARRAY_BEGIN(SPREG_NUMBERS));
            const auto name = RegName(i);
            return hasSpReg(name, cpuType) ? name : REG_UNDEF;
        }
    }
    return REG_UNDEF;
}

uint8_t encodeSpReg(RegName name) {
    return pgm_read_byte(&SPREG_NUMBERS[name]);
}

}  // namespace reg
}  // namespace am29000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

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

#include "reg_c166.h"
#include "reg_base.h"
#include "text_c166.h"

using namespace libasm::reg;
using namespace libasm::text::c166;

namespace libasm {
namespace c166 {
namespace reg {

namespace {
// clang-format off

constexpr NameEntry CC_NAMES[] PROGMEM = {
    { TEXT_CC_C,   CC_C   },
    { TEXT_CC_N,   CC_N   },
    { TEXT_CC_NC,  CC_NC  },
    { TEXT_CC_NET, CC_NET },
    { TEXT_CC_NN,  CC_NN  },
    { TEXT_CC_NV,  CC_NV  },
    { TEXT_CC_NZ,  CC_NZ  },
    { TEXT_CC_SGE, CC_SGE },
    { TEXT_CC_SGT, CC_SGT },
    { TEXT_CC_SLE, CC_SLE },
    { TEXT_CC_SLT, CC_SLT },
    { TEXT_CC_UC,  CC_UC  },
    { TEXT_CC_UGT, CC_UGT },
    { TEXT_CC_ULE, CC_ULE },
    { TEXT_CC_V,   CC_V   },
    { TEXT_CC_Z,   CC_Z   },
};

PROGMEM constexpr NameTable CC_TABLE{ARRAY_RANGE(CC_NAMES)};

// The SFRs of the CPU core, by short register number.
constexpr NameEntry SFR_NAMES[] PROGMEM = {
    { TEXT_REG_CP,    int8_t(0x08) },
    { TEXT_REG_CSP,   int8_t(0x04) },
    { TEXT_REG_DPP0,  int8_t(0x00) },
    { TEXT_REG_DPP1,  int8_t(0x01) },
    { TEXT_REG_DPP2,  int8_t(0x02) },
    { TEXT_REG_DPP3,  int8_t(0x03) },
    { TEXT_REG_MDC,   int8_t(0x87) },
    { TEXT_REG_MDH,   int8_t(0x06) },
    { TEXT_REG_MDL,   int8_t(0x07) },
    { TEXT_REG_ONES,  int8_t(0x8F) },
    { TEXT_REG_PSW,   int8_t(0x88) },
    { TEXT_REG_SP,    int8_t(0x09) },
    { TEXT_REG_STKOV, int8_t(0x0A) },
    { TEXT_REG_STKUN, int8_t(0x0B) },
    { TEXT_REG_TFR,   int8_t(0xD6) },
    { TEXT_REG_ZEROS, int8_t(0x8E) },
};

PROGMEM constexpr NameTable SFR_TABLE{ARRAY_RANGE(SFR_NAMES)};

// clang-format on
}  // namespace

StrBuffer &outRegName(StrBuffer &out, RegName name) {
    const auto num = int8_t(name);
    if (num < 16)
        return out.letter('R').int16(num);
    out.letter('R').letter((num & 1) ? 'H' : 'L');
    return out.int16((num - 16) / 2);
}

StrBuffer &outCcName(StrBuffer &out, CcName name, bool prefix) {
    const auto *entry = CC_TABLE.searchName(name);
    if (entry) {
        if (prefix)
            out.text_P(TEXT_CC_);
        out.text_P(entry->text_P());
    }
    return out;
}

RegName decodeWordReg(uint8_t num) {
    return RegName(num & 0xF);
}

RegName decodeByteReg(uint8_t num) {
    return RegName((num & 0xF) + 16);
}

const /*PROGMEM*/ char *sfrName_P(uint16_t addr, CpuType) {
    if (addr < 0xFE00 || (addr & 1) != 0)
        return nullptr;
    const auto num = int8_t((addr - 0xFE00) / 2);
    auto entry = SFR_TABLE.searchName(num);
    return entry ? entry->text_P() : nullptr;
}

CcName decodeCcNum(uint8_t num) {
    return CcName(num & 0xF);
}

}  // namespace reg
}  // namespace c166
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

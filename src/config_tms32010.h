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

#ifndef __LIBASM_CONFIG_TMS32010_H__
#define __LIBASM_CONFIG_TMS32010_H__

#include "config_base.h"
#include "reg_tms32010.h"

namespace libasm {
namespace tms32010 {

enum CpuType : uint8_t {
    TMS32010,
    TMS32015,
};

struct Config
    : ConfigImpl<CpuType, ADDRESS_12BIT, ADDRESS_WORD, OPCODE_16BIT, ENDIAN_LITTLE, 4, 4> {
    Config(const InsnTable<CpuType> &table) : ConfigImpl(table, TMS32010) {}

protected:
    uint16_t dataMemoryMax() const { return cpuType() == TMS32010 ? UINT16_C(0x8F) : PAGE1_MAX; }

    uint_fast8_t maxAR() const { return 1; }
    uint_fast8_t maxPA() const { return 7; }
    bool isAR(RegName name) const { return name >= REG_AR0 && name <= REG_AR0 + maxAR(); }
    bool isPA(RegName name) const { return name >= REG_PA0 && name <= REG_PA0 + maxPA(); }
    RegName decodeAR(uint32_t r) const { return r <= maxAR() ? RegName(r + REG_AR0) : REG_UNDEF; }
    RegName decodePA(uint32_t r) const { return r <= maxPA() ? RegName(r + REG_PA0) : REG_UNDEF; }

    bool isSST(opcode_t opc) const {
        opc >>= 8;
        return opc == 0x7C;  // SST
    }

    bool validDmAddr(opcode_t opc, uint32_t dma) const {
        uint16_t min = UINT16_C(0);
        uint16_t max = dataMemoryMax();
        if (isSST(opc)) {
            // TMS3201x: SST destination must be in page 1.
            min = PAGE1_MIN;
        }
        return dma >= min && dma <= max;
    }

    uint16_t toDmAddr(opcode_t opc) const {
        auto dma = opc & 0x7F;
        if (isSST(opc)) {
            // TMS3201x: SST destination must be in page 1.
            return PAGE1_MIN + dma;
        }
        return dma;
    }

    static constexpr auto PAGE1_MIN = UINT16_C(0x0080);
    static constexpr auto PAGE1_MAX = UINT16_C(0x00FF);
};

}  // namespace tms32010
}  // namespace libasm

#endif  // __LIBASM_CONFIG_TMS32010_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

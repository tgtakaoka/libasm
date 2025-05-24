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

#ifndef __LIBASM_CONFIG_TMS320_H__
#define __LIBASM_CONFIG_TMS320_H__

#include "config_base.h"
#include "reg_tms320.h"

namespace libasm {
namespace tms320 {

enum CpuType : uint8_t {
    TMS32010,
    TMS32015,
    TMS32020,
    TMS320C25,
    TMS320C26,
};

struct Config
    : ConfigImpl<CpuType, ADDRESS_12BIT, ADDRESS_WORD, OPCODE_16BIT, ENDIAN_LITTLE, 4, 4> {
    Config(const InsnTable<CpuType> &table) : ConfigImpl(table, TMS32010) {}

    AddressWidth addressWidth() const override { return is3201x() ? ADDRESS_12BIT : ADDRESS_16BIT; }

protected:
    bool is3201x() const { return cpuType() == TMS32010 || cpuType() == TMS32015; }
    bool is3202x() const { return !is3201x(); }
    bool is320C2x() const { return cpuType() == TMS320C25 || cpuType() == TMS320C26; }

            uint16_t dataMemoryMax() const {
        if (is3201x()) {
            return cpuType() == TMS32010 ? UINT16_C(0x8F) : PAGE1_MAX;
        } else {
            return cpuType() == TMS320C26 ? 0x7FF : 0x3FF;
        }
    }

    uint_fast8_t maxAR() const { return is3201x() ? 1 : 7; }
    uint_fast8_t maxPA() const { return is3201x() ? 7 : 15; }
    bool isAR(RegName name) const { return name >= REG_AR0 && name <= RegName(REG_AR0 + maxAR()); }
    bool isPA(RegName name) const { return name >= REG_PA0 && name <= RegName(REG_PA0 + maxPA()); }
    RegName decodeAR(uint32_t r) const { return r <= maxAR() ? RegName(r + REG_AR0) : REG_UNDEF; }
    RegName decodePA(uint32_t r) const { return r <= maxPA() ? RegName(r + REG_PA0) : REG_UNDEF; }

    bool isSST(opcode_t opc) const {
        opc >>= 8;
        return is3201x() ? opc == 0x7C            // SST
                         : (opc & 0xFE) == 0x78;  // SST and SST1
    }

    bool validDmAddr(opcode_t opc, uint32_t dma) const {
        uint16_t min = UINT16_C(0);
        uint16_t max = dataMemoryMax();
        if (isSST(opc)) {
            if (is3201x()) {
                // TMS3201x: SST destination must be in page 1.
                min = PAGE1_MIN;
            } else {
                // TMS3202x: SST/SST1 destination must be in page 0.
                min = PAGE0_MIN;
                max = PAGE0_MAX;
            }
        }
        return dma >= min && dma <= max;
    }

    uint16_t toDmAddr(opcode_t opc) const {
        auto dma = opc & 0x7F;
        if (isSST(opc)) {
            // TMS3201x: SST destination must be in page 1.
            // TMS3202x: SST/SST1 destination must be in page 0.
            return (is3201x() ? PAGE1_MIN : PAGE0_MIN) + dma;
        }
        return dma;
    }

    static constexpr auto PAGE0_MIN = UINT16_C(0x0000);
    static constexpr auto PAGE0_MAX = UINT16_C(0x007F);
    static constexpr auto PAGE1_MIN = UINT16_C(0x0080);
    static constexpr auto PAGE1_MAX = UINT16_C(0x00FF);
};

}  // namespace tms320
}  // namespace libasm

#endif  // __LIBASM_CONFIG_TMS320_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

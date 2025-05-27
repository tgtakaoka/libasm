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
        if (is3201x())
            return cpuType() == TMS32010 ? UINT16_C(0x8F) : PAGE1_MAX;
        return UINT16_MAX;
    }

    uint_fast8_t maxAR() const { return is3201x() ? 1 : 7; }
    uint_fast8_t maxPA() const { return is3201x() ? 7 : 15; }
    bool isAR(RegName name) const { return name >= REG_AR0 && name <= RegName(REG_AR0 + maxAR()); }
    bool isPA(RegName name) const { return name >= REG_PA0 && name <= RegName(REG_PA0 + maxPA()); }
    RegName decodeAR(uint32_t r) const { return r <= maxAR() ? RegName(r + REG_AR0) : REG_UNDEF; }
    RegName decodePA(uint32_t r) const { return r <= maxPA() ? RegName(r + REG_PA0) : REG_UNDEF; }

    bool isSST1x(opcode_t opc) const {
        const uint8_t insn = opc >> 8;
        return is3201x() && insn == 0x7C;  // SST
    }

    bool isSST2x(opcode_t opc) const {
        const uint8_t insn = (opc >> 8) & 0xFE;
        return is3202x() && insn == 0x78;  // SST and SST1;
    }

    bool validDmAddr(opcode_t opc, uint32_t dma) const {
        auto min = PAGE0_MIN;
        auto max = dataMemoryMax();
        if (isSST1x(opc)) {
            // TMS3201x: SST destination must be in page 1.
            min = PAGE1_MIN;
        }
        if (isSST2x(opc)) {
            // TMS3202x: SST/SST1 destination must be in page 0.
            min = PAGE0_MIN;
            max = PAGE0_MAX;
        }
        return dma >= min && dma <= max;
    }

    uint16_t toDmAddr(opcode_t opc) const {
        const uint8_t dma = opc & 0x7F;
        if (isSST1x(opc)) {
            // TMS3201x: SST destination must be in page 1.
            return PAGE1_MIN + dma;
        }
        if (isSST2x(opc)) {
            // TMS3202x: SST/SST1 destination must be in page 0.
            return PAGE0_MIN + dma;
        }
        return dma;
    }

    static constexpr uint16_t PAGE0_MIN = 0x0000;
    static constexpr uint16_t PAGE0_MAX = 0x007F;
    static constexpr uint16_t PAGE1_MIN = 0x0080;
    static constexpr uint16_t PAGE1_MAX = 0x00FF;
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

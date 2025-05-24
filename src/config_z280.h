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

#ifndef __LIBASM_CONFIG_Z280_H__
#define __LIBASM_CONFIG_Z280_H__

#include "config_base.h"

namespace libasm {
namespace z280 {

enum CpuType : uint8_t {
    Z80,
    Z280,
};

struct Config : ConfigImpl<CpuType, ADDRESS_24BIT, ADDRESS_BYTE, OPCODE_8BIT, ENDIAN_LITTLE, 8, 6> {
    Config(const InsnTable<CpuType> &table) : ConfigImpl(table, Z280) {}

    AddressWidth addressWidth() const override {
        return (cpuType() == Z80) ? ADDRESS_16BIT : ADDRESS_24BIT;
    }
    uint8_t codeMax() const override { return cpuType() == Z80 ? 4 : 8; }
    uint8_t nameMax() const override { return cpuType() == Z80 ? 5 : 6; }

protected:
    bool z80() const { return cpuType() == Z80; }
};

}  // namespace z280
}  // namespace libasm

#endif  // __LIBASM_CONFIG_Z280_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

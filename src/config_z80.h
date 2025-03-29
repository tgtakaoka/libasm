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

#ifndef __LIBASM_CONFIG_Z80_H__
#define __LIBASM_CONFIG_Z80_H__

#include "config_base.h"

namespace libasm {
namespace z80 {

enum CpuType : uint8_t {
    Z80,
    Z180,
    Z280,
    Z380,
};

struct Config : ConfigImpl<CpuType, ADDRESS_32BIT, ADDRESS_BYTE, OPCODE_8BIT, ENDIAN_LITTLE, 4, 5> {
    Config(const InsnTable<CpuType> &table) : ConfigImpl(table, Z80) {}

    AddressWidth addressWidth() const override {
        if (cpuType() == Z80)
            return ADDRESS_16BIT;
        if (cpuType() == Z180)
            return ADDRESS_20BIT;
        if (cpuType() == Z280)
            return ADDRESS_24BIT;
        return ADDRESS_32BIT;
    }
    uint8_t codeMax() const override { return cpuType() >= Z280 ? 8 : 4; }
    uint8_t nameMax() const override { return cpuType() >= Z280 ? 6 : 5; }

protected:
    bool z80() const { return cpuType() == Z80 || cpuType() == Z180; }
    bool z180() const { return cpuType() == Z180; }
    bool z280() const { return cpuType() == Z280; }
    bool z380() const { return cpuType() == Z380; }
};

}  // namespace z80
}  // namespace libasm

#endif  // __LIBASM_CONFIG_Z80_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

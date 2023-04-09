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

#ifndef __CONFIG_MN1610_H__
#define __CONFIG_MN1610_H__

#include "config_base.h"

namespace libasm {
namespace mn1610 {

enum CpuType : uint8_t {
    MN1610,
    MN1613,
    MN1613A,
};

struct Config : ConfigImpl<CpuType, ADDRESS_18BIT, ADDRESS_WORD, OPCODE_16BIT, ENDIAN_BIG, 4, 4> {
    Config(const InsnTable<CpuType> &table) : ConfigImpl(table, MN1610) {}

    AddressWidth addressWidth() const override {
        return cpuType() == MN1610 ? ADDRESS_16BIT : ADDRESS_18BIT;
    }
};

}  // namespace mn1610
}  // namespace libasm

#endif  // __CONFIG_MN1610_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

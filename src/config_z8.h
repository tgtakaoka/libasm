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

#ifndef __LIBASM_CONFIG_Z8_H__
#define __LIBASM_CONFIG_Z8_H__

#include "config_base.h"

namespace libasm {
namespace z8 {

enum CpuType : uint8_t {
    Z8,
    Z86C,
    SUPER8,
};

struct Config : ConfigImpl<CpuType, ADDRESS_16BIT, ADDRESS_BYTE, OPCODE_8BIT, ENDIAN_BIG, 4, 7> {
    Config(const InsnTable<CpuType> &table) : ConfigImpl(table, Z8) {}

    bool isSuper8() const { return cpuType() == SUPER8; }

    uint8_t codeMax() const override { return isSuper8() ? 4 : 3; }
    uint8_t nameMax() const override { return isSuper8() ? 6 : 4; }
};

}  // namespace z8
}  // namespace libasm

#endif  // __LIBASM_CONFIG_Z8_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

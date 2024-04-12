/*
 * Copyright 2024 Tadashi G. Takaoka
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

#ifndef __LIBASM_CONFIG_PDP11_H__
#define __LIBASM_CONFIG_PDP11_H__

#include "config_base.h"

namespace libasm {
namespace pdp11 {

enum CpuType : uint8_t {
    J11,
    T11,
};

struct Config
    : ConfigImpl<CpuType, ADDRESS_16BIT, ADDRESS_BYTE, OPCODE_16BIT, ENDIAN_LITTLE, 6, 10> {
    Config(const InsnTable<CpuType> &table) : ConfigImpl(table, J11) {}
};

}  // namespace pdp11
}  // namespace libasm

#endif  // __LIBASM_CONFIG_PDP11_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

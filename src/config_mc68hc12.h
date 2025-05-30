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

#ifndef __LIBASM_CONFIG_MC68HC12_H__
#define __LIBASM_CONFIG_MC68HC12_H__

#include "config_base.h"

namespace libasm {
namespace mc68hc12 {

enum CpuType : uint8_t {
    MC68HC12,
};

struct Config
    : public ConfigImpl<CpuType, ADDRESS_16BIT, ADDRESS_BYTE, OPCODE_8BIT, ENDIAN_BIG, 6, 5> {
    Config(const InsnTable<CpuType> &table) : ConfigImpl(table, MC68HC12) {}
};

}  // namespace mc68hc12
}  // namespace libasm

#endif  // __LIBASM_CONFIG_MC68HC12_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

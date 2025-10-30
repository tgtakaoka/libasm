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

#ifndef __LIBASM_CONFIG_TMS320F_H__
#define __LIBASM_CONFIG_TMS320F_H__

#include "config_base.h"

namespace libasm {
namespace tms320f {

enum CpuType : uint8_t {
    TMS320C3X,
};

struct Config
    : ConfigImpl<CpuType, ADDRESS_24BIT, ADDRESS_QUAD, OPCODE_32BIT, ENDIAN_LITTLE, 1, 8> {
    Config(const InsnTable<CpuType> &table) : ConfigImpl(table, TMS320C3X) {}

protected:
    uint8_t nameMax() const override { return 8; }
};

}  // namespace tms320f
}  // namespace libasm

#endif  // __LIBASM_CONFIG_TMS320F_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

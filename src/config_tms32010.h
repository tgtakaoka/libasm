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

namespace libasm {
namespace tms32010 {

enum CpuType : uint8_t {
    TMS32010,
    TMS32015,
};

struct Config
    : ConfigImpl<CpuType, ADDRESS_12BIT, ADDRESS_WORD, OPCODE_16BIT, ENDIAN_LITTLE, 4, 4> {
    Config(const InsnTable<CpuType> &table) : ConfigImpl(table, TMS32010) {}

    uint16_t dataMemoryLimit() const {
        return cpuType() == TMS32010 ? 0x8F : 0xFF;
    }
};

}  // namespace tms32010
}  // namespace libasm

#endif  // __CONFIG_TMS32010_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

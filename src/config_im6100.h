/*
 * Copyright 2023 Tadashi G. Takaoka
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

#ifndef __CONFIG_IM6100_H__
#define __CONFIG_IM6100_H__

#include "config_base.h"

namespace libasm {
namespace im6100 {

enum CpuType : uint8_t {
    IM6100,
};

struct Config
    : ConfigImpl<CpuType, ADDRESS_12BIT, ADDRESS_WORD, OPCODE_12BIT, ENDIAN_BIG, 2, 4> {
    Config(const InsnTable<CpuType> &table) : ConfigImpl(table, IM6100) {}
};

}  // namespace im6100
}  // namespace libasm

#endif  // __CONFIG_IM6100_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

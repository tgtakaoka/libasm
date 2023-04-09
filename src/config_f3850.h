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

#ifndef __CONFIG_F3850_H__
#define __CONFIG_F3850_H__

#include "config_base.h"

#include "text_f3850.h"

namespace libasm {
namespace f3850 {

enum CpuType : uint8_t {
    F3850,
};

struct Config : ConfigImpl<ADDRESS_16BIT, ADDRESS_BYTE, OPCODE_8BIT, ENDIAN_BIG, 3, 4,
                        text::f3850::TEXT_CPU_F3850> {};

}  // namespace f3850
}  // namespace libasm

#endif  // __CONFIG_F3850_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

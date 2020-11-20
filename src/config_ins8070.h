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

#ifndef __CONFIG_INS8070_H__
#define __CONFIG_INS8070_H__

#include "config_base.h"

namespace libasm {
namespace ins8070 {

struct Config : ConfigImpl<ADDRESS_16BIT, uint16_t, int16_t, OPCODE_8BIT,
                        uint8_t, 3, ENDIAN_LITTLE, 4> {};

}  // namespace ins8070
}  // namespace libasm

#endif  // __CONFIG_INS8070_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

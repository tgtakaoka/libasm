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

#ifndef __CONFIG_MOS6502_H__
#define __CONFIG_MOS6502_H__

#include "config_base.h"

namespace libasm {
namespace mos6502 {

struct Config : ConfigImpl<
    ADDRESS_16BIT, uint16_t, int16_t,
    OPCODE_8BIT, 4, ENDIAN_LITTLE, uint8_t, uint8_t,
    4>
{};

enum CpuType : host::uint_t {
    MOS6502,
    W65SC02,
    R65C02,
    W65C02S,
    W65C816,
};

} // namespace mos6502
} // namespace libasm

#endif // __CONFIG_MOS6502_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

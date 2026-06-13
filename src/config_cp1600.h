/*
 * Copyright 2026 Tadashi G. Takaoka
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

#ifndef __LIBASM_CONFIG_CP1600_H__
#define __LIBASM_CONFIG_CP1600_H__

#include "config_base.h"

namespace libasm {
namespace cp1600 {

enum CpuType : uint8_t {
    CP1600,
};

// 16-bit word-addressed CPU.  Max instruction length is 3 words (J/JSR);
// SDBD-prefixed continuation grows the buffer up to 4 words (SDBD + MVII +
// double-byte data = 8 bytes), so use codeMax = 8.
struct Config : ConfigImpl<CpuType, ADDRESS_16BIT, ADDRESS_WORD, OPCODE_16BIT, ENDIAN_BIG, 8, 4> {
    Config(const InsnTable<CpuType> &table) : ConfigImpl(table, CP1600) {}
};

}  // namespace cp1600
}  // namespace libasm

#endif  // __LIBASM_CONFIG_CP1600_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

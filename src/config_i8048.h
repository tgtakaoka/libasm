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

#ifndef __CONFIG_I8048_H__
#define __CONFIG_I8048_H__

#include "config_base.h"

#include "text_i8048.h"

namespace libasm {
namespace i8048 {

enum CpuType : uint8_t {
    I8048,
    I8039,
    I80C48,
    I80C39,
    MSM80C48,
    MSM80C39,
};

struct Config : ConfigImpl<CpuType, ADDRESS_12BIT, ADDRESS_BYTE, OPCODE_8BIT, ENDIAN_BIG, 2, 5> {
    Config(const InsnTable<CpuType> &table) : ConfigImpl(table, I8048) {}
};

}  // namespace i8048
}  // namespace libasm

#endif  // __CONFIG_I8048_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

/*
 * Copyright 2022 Tadashi G. Takaoka
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

#ifndef __LIBASM_CONFIG_I8096_H__
#define __LIBASM_CONFIG_I8096_H__

#include "config_base.h"

namespace libasm {
namespace i8096 {

enum CpuType : uint8_t {
    I8096,
    I80196KB,
    I80196,
};

struct Config : ConfigImpl<CpuType, ADDRESS_16BIT, ADDRESS_BYTE, OPCODE_8BIT, ENDIAN_LITTLE, 7, 5> {
    Config(const InsnTable<CpuType> &table) : ConfigImpl(table, I8096) {}

protected:
    bool ioAddress(uint16_t addr) const {
        return addr >= 2 && addr < 0x18;
    }
};

}  // namespace i8096
}  // namespace libasm

#endif  // __LIBASM_CONFIG_I8096_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

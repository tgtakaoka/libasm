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

#ifndef __LIBASM_CONFIG_H16_H__
#define __LIBASM_CONFIG_H16_H__

#include "config_base.h"

namespace libasm {
namespace h16 {

enum CpuType : uint8_t {
    H16,
};

// MaxCode 24 = longest instruction: a bit-field op (opcode + Rnb|Rnf byte)
// with two maximal EAs, each a bank-prefixed register double-indirect
// (prefix + EA byte + ext + 4-byte ds1 + 4-byte ds2 = 11 bytes): 1+1+11+11.
struct Config
        : ConfigImpl<CpuType, ADDRESS_32BIT, ADDRESS_BYTE, OPCODE_8BIT, ENDIAN_BIG, 24, 8> {
    Config(const InsnTable<CpuType> &table) : ConfigImpl(table, H16) {}
};

}  // namespace h16
}  // namespace libasm

#endif  // __LIBASM_CONFIG_H16_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

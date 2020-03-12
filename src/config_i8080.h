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

#ifndef __CONFIG_I8080_H__
#define __CONFIG_I8080_H__

#include "config_host.h"

namespace target
{
    typedef uint16_t uintptr_t;
    typedef int16_t  ptrdiff_t;
    typedef uint8_t  opcode_t;
    typedef uint8_t  insn_t;
} // namespace target

enum AddrMode : host::uint_t {
    INHR,
    IMM8,
    IMM16,
    DIRECT,
    IOADR,
};

enum InsnFormat : host::uint_t {
    NO_FORMAT,
    POINTER_REG,                // **PP_****: B/D/H/SP
    STACK_REG,                  // **PP_****: B/D/H/PSW
    INDEX_REG,                  // ***I_****: B/D
    DATA_REG,                   // **DD_D***: B/C/D/E/H/L/M/A
    LOW_DATA_REG,               // ****_*DDD: B/C/D/E/H/L/M/A
    DATA_DATA_REG,              // **DD_DSSS: B/C/D/E/H/L/M/A
    VECTOR_NO,                  // **VV_V***: 0~7
};

#endif // __CONFIG_I8080_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

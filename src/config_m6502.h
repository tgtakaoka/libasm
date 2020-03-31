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

#ifndef __CONFIG_M6502_H__
#define __CONFIG_M6502_H__

#include "config_host.h"

// Not all 65C02 variants implemet bit manipulation operation
//#define W65C02_ENABLE_BITOPS

namespace target
{
    typedef uint16_t uintptr_t;
    typedef int16_t  ptrdiff_t;
    typedef uint8_t  opcode_t;
    typedef uint8_t  insn_t;
} // namespace target

enum CpuType : host::uint_t {
    M6502,
    W65SC02,
    R65C02,
    W65C02S,
};

enum AddrMode : host::uint_t {
    // M6502
    IMPL,                // Implied
    ACCM,                // Accumulator A
    IMM,                 // Immediate: #nn
    ABS,                 // Absolute: abs
    ZPG,                 // Zero Page: zp
    ZPG_IDX,             // Zero Page Indexed: zp,X
    ZPG_IDY,             // Zero Page Indexed: zp,Y
    ABS_IDX,             // Absolute Indexed: abs,X
    ABS_IDY,             // Absolute Indexed: abs,Y
    REL,                 // Relative: abs
    ZPG_IDX_IDIR,        // Indexed Indirect: (zp,X)
    ZPG_IDIR_IDY,        // Indirect Indexed: (zp),Y
    ABS_IDIR,            // Absolute Indirect: (abs)

    // W65SC02
    ABS_IDX_IDIR,        // Indexed Absolute Indirect: (abs,X)
    ZPG_IDIR,            // Zero Page Indirect: (zp)
    ZPG_REL,             // Zero Page Relative: zp,abs
};

#endif // __CONFIG_M6502_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

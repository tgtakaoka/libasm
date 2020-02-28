/* -*- mode: c++; -*- */
#ifndef __CONFIG_MC6800_H__
#define __CONFIG_MC6800_H__

#include "config_host.h"

namespace target
{
    typedef uint16_t uintptr_t;
    typedef int16_t  ptrdiff_t;
    typedef uint8_t  opcode_t;
    typedef uint8_t  insn_t;
} // namespace target

enum AddrMode : host::uint_t {
    INH,  // Inherent
    ACC,  // Accumulator
    DIR,  // Direct page
    EXT,  // Extended
    IDX,  // Indexed
    REL,  // Relative
    IMM,  // Immediate
};

enum InsnAdjust : host::uint_t {
    ADJ_ZERO = 0,
    ADJ_AB01 = 1,   // Accumulator A:+0, B:+1
    ADJ_AB16 = 2,  // Accumulator A:+0, B:+$10
    ADJ_AB64 = 3,  // Accumulator A:+0, B:+$40
};

enum OprSize : host::uint_t {
    SZ_NONE,                    // unknown
    SZ_BYTE = 1,
    SZ_WORD = 2,
};

#endif // __CONFIG_MC6800_H__

/* -*- mode: c++; -*- */
#ifndef __CONFIG_R65C02_H__
#define __CONFIG_R65C02_H__

#include "config_host.h"
#include "error_r65c02.h"

namespace target
{
    typedef uint16_t uintptr_t;
    typedef int16_t  ptrdiff_t;
    typedef uint8_t  insn_t;
} // namespace target

enum McuType : host::uint_t {
    R6502,
    R65C02,
};

enum AddrMode : host::uint_t {
    IMPLIED,
    ACCUMULATOR,
    IMMEDIATE,
    ABSOLUTE,
    ZEROPAGE,
    ZP_IDX_X,                   // Zero Page Indexed zp,X
    ZP_IDX_Y,                   // Zero Page Indexed zp,Y
    ABS_IDX_X,                  // Absolute Indexed abs,X
    ABS_IDX_Y,                  // Absolute Indexed abs,Y
    REL8,
    INDX_IND,                // Indexed Indirect (zp,X)
    INDIRECT_IDX,               // Indirect Indexed (zp),Y
    ABS_INDIRECT,               // Absolute Indirect (abs)

    // R65C02
    IDX_ABS_IND,                // Indexed Absolute Indirect (abs,X)
    ZP_REL8,                // Zero Page Relative zp,abs
    ZP_INDIRECT,                // Zero Page Indirect (zp)
};

#endif // __CONFIG_R65C02_H__

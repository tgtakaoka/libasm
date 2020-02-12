/* -*- mode: c++; -*- */
#ifndef __CONFIG_MC6809_H__
#define __CONFIG_MC6809_H__

#include "config_host.h"

namespace target
{
    typedef uint16_t uintptr_t;
    typedef int16_t  ptrdiff_t;
    typedef uint16_t insn_t;

    typedef uint8_t  opcode_t;
} // namespace target

enum McuType : host::uint_t {
    MC6809,
    HD6309,
};

enum OprSize : host::uint_t {
    SZ_NONE = 0,
    SZ_BYTE = 1,                // 8 bit operation
    SZ_WORD = 2,                // 16 bit operation
    SZ_LONG = 3,                // 32 bit operation
};

enum AddrMode : host::uint_t {
    INHR,
    DIRP,
    EXTD,
    INDX,
    REL,
    IMM,
    STKOP,
    REGS,
    // HD6309
    IMMDIR,
    IMMEXT,
    IMMIDX,
    BITOP,
    TFRM,
    // Pseudo instruction
    PSEUDO,
};

#endif // __CONFIG_MC6809_H__

/* -*- mode: c++; -*- */
#ifndef __CONFIG_HD6309_H__
#define __CONFIG_HD6309_H__

#include "config_host.h"
#include "error_hd6309.h"

namespace target
{
    typedef ::uint8_t  byte_t;
    typedef ::uint16_t uint16_t;
    typedef ::uint32_t uint32_t;
    typedef ::int8_t   int8_t;
    typedef ::int16_t  int16_t;
    typedef ::int32_t  int32_t;
    typedef ::uint16_t uintptr_t;
    typedef ::int16_t  ptrdiff_t;
    typedef ::uint8_t  opcode_t;
    typedef ::uint16_t insn_t;
} // namespace target

enum McuType : host::uint_t {
    MC6809,
    HD6309,
};

enum AddrMode : host::uint_t {
    INHR,
    DIRP,
    EXTD,
    INDX,
    REL8,
    REL16,
    IMM8,
    IMM16,
    STKOP,
    REGS,
    // HD6309
    IMM32,
    IMMDIR,
    IMMEXT,
    IMMIDX,
    BITOP,
    TFRM,
};

#include "registers_hd6309.h"

#endif // __CONFIG_HD6309_H__

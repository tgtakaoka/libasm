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
    typedef ::int16_t  intptr_t;
    typedef ::int16_t  ptrdiff_t;
    typedef ::uint8_t  opcode_t;
    typedef ::uint16_t insn_t;
} // namespace target

enum McuMode : host::uint_t {
    MC6809,
    HD6309,
};

enum AddrMode : host::uint_t {
    INHERENT,
    DIRECT_PG,
    EXTENDED,
    INDEXED,
    RELATIVE8,
    RELATIVE16,
    IMMEDIATE8,
    IMMEDIATE16,
    STACK_OP,
    REGISTERS,
    // HD6309
    IMMEDIATE32,
    IMM_DIRECT,
    IMM_EXTENDED,
    IMM_INDEXED,
    BIT_OPERATION,
    TRANSFER_MEM,
};

#include "registers_hd6309.h"

#endif // __CONFIG_HD6309_H__

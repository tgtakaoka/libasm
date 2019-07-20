/* -*- mode: c++; -*- */
#ifndef __CONFIG_MC6809_H__
#define __CONFIG_MC6809_H__

#include "config_host.h"
#include "error_mc6809.h"

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

#ifndef MC6809_ENUMS_DEFINED
enum AddrMode : host::uint_t {
    INHERENT,
    DIRECT_PG,
    EXTENDED,
    INDEXED,
    RELATIVE,
    IMMEDIATE,
    STACK_OP,
    REGISTERS,
};

enum RegName : char {
    NONE = 0,
    OFFSET = 'o',
    D = 'D',
    X = 'X',
    Y = 'Y',
    U = 'U',
    S = 'S',
    PC = 'P',
    A = 'A',
    B = 'B',
    CC = 'C',
    DP = 'd',
};
#endif

#include "insn_mc6809.h"

#endif // __CONFIG_MC6809_H__

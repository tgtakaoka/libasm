/* -*- mode: c++; -*- */
#ifndef __CONFIG_MC6809_H__
#define __CONFIG_MC6809_H__

#include "config_host.h"
#include "error_mc6809.h"

namespace target
{
    typedef std::uint8_t  byte_t;
    using std::uint16_t;
    using std::uint32_t;
    using std::int16_t;
    using std::int32_t;
    typedef std::uint16_t uintptr_t;
    typedef std::int16_t  intptr_t;
    typedef std::int16_t  ptrdiff_t;
    typedef std::uint8_t  opcode_t;
    typedef std::uint16_t insn_t;
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

#endif // __CONFIG_MC6809_H__

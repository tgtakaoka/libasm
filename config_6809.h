/* -*- mode: c++; -*- */
#ifndef __CONFIG_6809_H__
#define __CONFIG_6809_H__

#include "config_host.h"

#ifndef TARGET_ENUMS_DEFINED
#define TARGET_ENUMS_DEFINED
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

#endif // __CONFIG_6809_H__

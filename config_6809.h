/* -*- mode: c++; -*- */
#ifndef __CONFIG_6809_H__
#define __CONFIG_6809_H__

#include "config_host.h"

enum AddrMode6809 : host::uint_t {
    INHERENT,
    DIRECT_PG,
    EXTENDED,
    INDEXED,
    RELATIVE,
    IMMEDIATE,
    STACK_OP,
    REGISTERS,
};

enum RegName6809 : char {
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

#endif // __CONFIG_6809_H__

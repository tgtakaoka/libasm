/* -*- mode: c++; -*- */
#ifndef __CONFIG_6309_H__
#define __CONFIG_6309_H__

#include "config_host.h"

enum AddrMode6309 : host::uint_t {
    INHERENT,
    DIRECT_PG,
    EXTENDED,
    INDEXED,
    RELATIVE,
    IMMEDIATE,
    STACK_OP,
    REGISTERS,
    IMM_DIRECT,
    IMM_EXTENDED,
    IMM_INDEXED,
    BIT_OPERATION,
    TRANSFER_MEM,
    Mode_end_of_6309,
};

enum RegName6309 : char {
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
    E = 'E',
    F = 'F',
    W = 'W',
    V = 'V',
    ZERO = '0',
};

#endif // __CONFIG_6309_H__

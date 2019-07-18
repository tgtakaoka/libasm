/* -*- mode: c++; -*- */
#ifndef __CONFIG_HD6309_H__
#define __CONFIG_HD6309_H__

#include "config_host.h"

enum AddrMode : host::uint_t {
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
    Mode_end_of_hd6309,
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
    E = 'E',
    F = 'F',
    W = 'W',
    V = 'V',
    ZERO = '0',
};

#define TARGET_ENUMS_DEFINED
#include "config_mc6809.h"

#endif // __CONFIG_HD6309_H__

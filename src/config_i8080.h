/* -*- mode: c++; -*- */
#ifndef __CONFIG_I8080_H__
#define __CONFIG_I8080_H__

#include "config_host.h"
#include "error_i8080.h"

namespace target
{
    typedef ::uint8_t  byte_t;
    typedef ::uint16_t uint16_t;
    typedef ::int8_t   int8_t;
    typedef ::int16_t  int16_t;
    typedef ::uint16_t uintptr_t;
    typedef ::uint8_t  insn_t;
} // namespace target

enum AddrMode : host::uint_t {
    INHERENT,
    IMMEDIATE_8,
    IMMEDIATE_16,
    DIRECT,
    IOADDR,
};

enum InsnFormat : host::uint_t {
    NO_FORMAT,
    POINTER_REG,                // **PP_****: B/D/H/SP
    STACK_REG,                  // **PP_****: B/D/H/PSW
    INDEX_REG,                  // ***I_****: B/D
    DATA_REG,                   // **DD_D***: B/C/D/E/H/L/M/A
    LOW_DATA_REG,               // ****_*DDD: B/C/D/E/H/L/M/A
    DATA_DATA_REG,              // **DD_DSSS: B/C/D/E/H/L/M/A
    VECTOR_NO,                  // **VV_V***: 0~7
};

#include "registers_i8080.h"

#endif // __CONFIG_I8080_H__

/* -*- mode: c++; -*- */
#ifndef __ERROR_R65C02_H__
#define __ERROR_R65C02_H__

#include "config_host.h"

#define ERROR_DEFINED
enum Error : host::uint_t {
    OK = 0,
    INVALID_STATE,

    NO_MEMORY,
    INTERNAL_ERROR,
    ILLEGAL_REGISTER,

    NO_TEXT,
    UNKNOWN_INSTRUCTION,
    UNKNOWN_OPERAND,
    OPERAND_NOT_ZP,
    OPERAND_TOO_FAR,
    GARBAGE_AT_END,
};

#endif // __ERROR_R65C02_H__

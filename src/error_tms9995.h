/* -*- mode: c++; -*- */
#ifndef __ERROR_TMS9995_H__
#define __ERROR_TMS9995_H__

#include "config_host.h"

#define ERROR_DEFINED
enum Error : host::uint_t {
    OK = 0,
    INVALID_STATE,

    NO_MEMORY,
    INTERNAL_ERROR,

    NO_TEXT,
    UNKNOWN_INSTRUCTION,
    UNKNOWN_OPERAND,
    OPERAND_TOO_FAR,
    GARBAGE_AT_END,
};

#endif // __ERROR_TMS9995_H__

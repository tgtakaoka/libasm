#ifndef __ERROR_MC6809_H__
#define __ERROR_MC6809_H__

#include "config_host.h"

#define ERROR_DEFINED
enum Error : host::uint_t {
    OK = 0,
    INVALID_STATE,

    NO_TEXT,
    UNKNOWN_INSTRUCTION,
    UNKNOWN_REGISTER,
    UNKNOWN_OPERAND,
    OPERAND_TOO_FAR,
    ILLEGAL_BIT_NUMBER,
    GARBAGE_AT_END,
};

#endif // __ERROR_MC6809_H__

/* -*- mode: c++; -*- */
#ifndef __ERROR_REPORTER_H__
#define __ERROR_REPORTER_H__

#include "config_host.h"

enum Error : host::uint_t {
    OK = 0,
    INVALID_STATE,

    // Disassembler
    NO_MEMORY,
    INTERNAL_ERROR,
    UNKNOWN_POSTBYTE,           // HD6309
    ILLEGAL_REGISTER,
    ILLEGAL_OPERAND,            // Z80

    // Assembler
    NO_INSTRUCTION,
    UNKNOWN_INSTRUCTION,
    UNKNOWN_REGISTER,
    UNKNOWN_OPERAND,
    OPERAND_NOT_ZP,             // R6502
    OPERAND_TOO_FAR,
    ILLEGAL_BIT_NUMBER,         // HD6309
    GARBAGE_AT_END,

    ILLEGAL_CONSTANT,
    OVERFLOW_RANGE,
    MISSING_CLOSING_PAREN,
    UNKNOWN_EXPR_OPERATOR,
    DIVIDE_BY_ZERO,
};

class ErrorReporter {
public:
    Error getError() const { return _error; }

protected:
    Error _error;

    Error setError(Error error) {
        _error = error;
        return error;
    }

    void resetError() {
        _error = INVALID_STATE;
    }
};

#endif // __ERROR_HD6309_H__

/* -*- mode: c++; -*- */
#ifndef __ERROR_REPORTER_H__
#define __ERROR_REPORTER_H__

#include "config_host.h"

enum Error : host::uint_t {
    OK = 0,
    INVALID_STATE = 1,

    // Disassembler
    NO_MEMORY = 2,
    INTERNAL_ERROR = 3,
    UNKNOWN_POSTBYTE = 4,       // HD6309
    ILLEGAL_REGISTER = 5,
    ILLEGAL_OPERAND = 6,        // Z80
    ILLEGAL_SIZE = 7,           // MC68000
    ILLEGAL_OPERAND_MODE = 8,   // MC68000

    // Assembler
    NO_INSTRUCTION = 20,
    UNKNOWN_INSTRUCTION = 21,
    UNKNOWN_REGISTER = 22,
    UNKNOWN_OPERAND = 23,
    OPERAND_NOT_ZP = 24,        // R6502
    OPERAND_TOO_FAR = 25,
    ILLEGAL_BIT_NUMBER = 16,    // HD6309
    GARBAGE_AT_END = 27,

    // AsmOperand
    ILLEGAL_CONSTANT = 100,
    OVERFLOW_RANGE = 101,
    MISSING_CLOSING_PAREN = 102,
    MISSING_CLOSING_QUOTE = 103,
    UNKNOWN_ESCAPE_SEQUENCE = 104,
    UNKNOWN_EXPR_OPERATOR = 105,
    DIVIDE_BY_ZERO = 106,
    UNDEFINED_SYMBOL = 107,
    TOO_COMPLEX_EXPRESSION = 108,

    // AsmDirective
    UNKNOWN_DIRECTIVE = 150,
    ILLEGAL_LABEL = 151,
    DUPLICATE_LABEL = 152,
    MISSING_LABEL = 153,
    MISSING_CLOSING_DQUOTE = 154,
    UNSUPPORTED_CPU = 155,
    TOO_MANY_INCLUDE = 156,
    NO_INCLUDE_FOUND = 157,
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

    Error setError(const ErrorReporter &other) {
        return setError(other.getError());
    }

    void resetError() {
        _error = INVALID_STATE;
    }
};

#endif // __ERROR_HD6309_H__

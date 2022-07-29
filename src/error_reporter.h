/*
 * Copyright 2020 Tadashi G. Takaoka
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __ERROR_REPORTER_H__
#define __ERROR_REPORTER_H__

#include "config_host.h"
#include "str_scanner.h"

#include <stdint.h>

namespace libasm {

enum Error : uint8_t {
    OK = 0,
    INSTRUCTION_NOT_ALIGNED = 1,

    // Disassembler
    NO_MEMORY = 2,
    INTERNAL_ERROR = 3,
    UNKNOWN_POSTBYTE = 4,
    ILLEGAL_REGISTER = 5,
    ILLEGAL_OPERAND = 6,
    ILLEGAL_SIZE = 7,
    ILLEGAL_OPERAND_MODE = 8,
    ILLEGAL_SEGMENT = 9,
    BUFFER_OVERFLOW = 10,
    OVERWRAP_PAGE = 11,

    // Assembler
    UNKNOWN_INSTRUCTION = 21,
    UNKNOWN_REGISTER = 22,
    UNKNOWN_OPERAND = 23,
    OPERAND_NOT_ALLOWED = 24,
    OPERAND_TOO_FAR = 25,
    ILLEGAL_BIT_NUMBER = 26,
    GARBAGE_AT_END = 27,
    DUPLICATE_REGISTER = 28,
    REGISTER_NOT_ALLOWED = 29,
    MISSING_COMMA = 30,
    NOT_BIT_ADDRESSABLE = 31,
    OPERAND_NOT_ALIGNED = 32,
    OPCODE_HAS_NO_EFFECT = 33,
    REGISTERS_OVERLAPPED = 34,
    MISSING_CLOSING_BRACKET = 35,

    // ValueParser
    ILLEGAL_CONSTANT = 100,
    OVERFLOW_RANGE = 101,
    MISSING_CLOSING_PAREN = 102,
    MISSING_CLOSING_QUOTE = 103,
    UNKNOWN_ESCAPE_SEQUENCE = 104,
    UNKNOWN_EXPR_OPERATOR = 105,
    DIVIDE_BY_ZERO = 106,
    UNDEFINED_SYMBOL = 107,
    TOO_COMPLEX_EXPRESSION = 108,
    MISSING_FUNC_ARGUMENT = 109,
    UNKNOWN_FUNCTION = 110,
    DUPLICATE_FUNCTION = 111,
    SYMBOL_REQUIRE = 112,

    // AsmDirective
    UNKNOWN_DIRECTIVE = 150,
    ILLEGAL_LABEL = 151,
    DUPLICATE_LABEL = 152,
    MISSING_LABEL = 153,
    MISSING_CLOSING_DQUOTE = 154,
    UNSUPPORTED_CPU = 155,
    TOO_MANY_INCLUDE = 156,
    NO_INCLUDE_FOUND = 157,
    MISSING_CLOSING_DELIMITOR = 158,
    END_ASSEMBLE = 159,

    // Command line
    UNKNOWN_OPTION = 200,
};

class ErrorReporter {
public:
    ErrorReporter() : _error(OK) {}

    bool isOK() const { return _error == OK; }
    Error getError() const { return _error; }

    Error resetError() { return setOK(); }
    Error setOK() { return setError(OK); }
    Error setError(Error error) { return _error = error; }
    Error setError(const ErrorReporter &reporter) { return setError(reporter.getError()); }

    const /*PROGMEM*/ char *errorText_P() const { return errorText_P(_error); }
    static const /*PROGMEM*/ char *errorText_P(Error error);

private:
    Error _error;
};

class ErrorAt : public ErrorReporter {
public:
    ErrorAt() : ErrorReporter(), _at(StrScanner::EMPTY) {}

    Error setError(Error error) { return ErrorReporter::setError(error); }
    Error setError(const StrScanner &at, Error error) {
        setAt(at);
        return setError(error);
    }
    Error setError(const ErrorAt &o) { return setError(o._at, o.getError()); }
    Error setError(const ErrorAt &o, Error error) { return setError(o._at, error); }
    Error setErrorIf(const StrScanner &at, Error error) {
        if (getError())
            return getError();
        return setError(at, error);
    }
    Error setErrorIf(const ErrorAt &o) { return setErrorIf(o._at, o.getError()); }

    void setAt(const StrScanner &at) { _at = at; }
    const StrScanner &errorAt() const { return _at; }

private:
    StrScanner _at;
};

}  // namespace libasm

#endif  // __ERROR_REPORTER_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

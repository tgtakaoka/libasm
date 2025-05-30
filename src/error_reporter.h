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

#ifndef __LIBASM_ERROR_REPORTER_H__
#define __LIBASM_ERROR_REPORTER_H__

#include "config_host.h"
#include "str_scanner.h"

#include <stdint.h>

namespace libasm {

enum Error : uint8_t {
    OK = 0,

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
    OVERWRAP_SEGMENT = 11,
    INVALID_INSTRUCTION = 12,
    SUBOPTIMAL_INSTRUCTION = 13,
    FLOAT_NOT_SUPPORTED = 14,
    PREFIX_HAS_NO_EFFECT = 15,

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
    REGISTER_NOT_ALIGNED = 33,
    OPCODE_HAS_NO_EFFECT = 34,
    REGISTERS_OVERLAPPED = 35,
    MISSING_CLOSING_BRACKET = 36,
    ILLEGAL_COMBINATION = 37,
    ADDRESSING_NOT_ALLOWED = 38,

    // ValueParser
    ILLEGAL_CONSTANT = 100,
    OVERFLOW_RANGE = 101,
    MISSING_CLOSING_PAREN = 102,
    MISSING_CLOSING_QUOTE = 103,
    UNKNOWN_ESCAPE_SEQUENCE = 104,
    OPERATOR_NOT_ASSOCIATIVE = 105,
    DIVIDE_BY_ZERO = 106,
    UNDEFINED_SYMBOL = 107,
    TOO_COMPLEX_EXPRESSION = 108,
    MISSING_FUNC_ARGUMENT = 109,
    TOO_FEW_FUNC_ARGUMENT = 110,
    TOO_MANY_FUNC_ARGUMENT = 111,
    DUPLICATE_FUNCTION = 112,
    NOT_AN_EXPECTED = 113,
    MISSING_OPERAND = 114,
    INTEGER_REQUIRED = 115,
    UNDERFLOW_RANGE = 116,

    // AsmDirective
    UNKNOWN_DIRECTIVE = 150,
    ILLEGAL_LABEL = 151,
    DUPLICATE_LABEL = 152,
    MISSING_LABEL = 153,
    MISSING_CLOSING_DQUOTE = 154,
    UNSUPPORTED_CPU = 155,
    TOO_MANY_INCLUDE = 156,
    NO_INCLUDE_FOUND = 157,
    MISSING_CLOSING_DELIMITER = 158,
    END_ASSEMBLE = 159,

    // Command line
    UNKNOWN_OPTION = 200,
};

struct ErrorReporter {
    constexpr ErrorReporter() : _error(OK) {}

    constexpr bool isOK() const { return _error == OK; }
    constexpr Error getError() const { return _error; }

    Error resetError() { return setOK(); }
    Error setOK() { return setError(OK); }

    Error setError(Error error) { return _error = error; }
    Error setErrorIf(Error error) { return _error ? _error : setError(error); }
    Error setError(const ErrorReporter &o) { return setError(o.getError()); }
    Error setErrorIf(const ErrorReporter &o) { return setErrorIf(o.getError()); }

    const /*PROGMEM*/ char *errorText_P() const { return errorText_P(_error); }
    static const /*PROGMEM*/ char *errorText_P(Error error);

private:
    Error _error;
};

struct StrBuffer;

struct ErrorAt : ErrorReporter {
    constexpr ErrorAt() : ErrorReporter(), _at("") {}

    constexpr bool hasError() const { return getError() && getError() != UNDEFINED_SYMBOL; }

    Error setError(Error error) { return ErrorReporter::setError(error); }
    Error setErrorIf(Error error) { return ErrorReporter::setErrorIf(error); }
    Error setError(const ErrorReporter &o) { return ErrorReporter::setError(o); }
    Error setErrorIf(const ErrorReporter &o) { return ErrorReporter::setErrorIf(o.getError()); }
    Error setError(const ErrorAt &o) {
        setAt(o._at);
        return setError(o.getError());
    }
    Error setErrorIf(const ErrorAt &o) { return isOK() && o.getError() ? setError(o) : getError(); }
    Error setError(const ErrorAt &o, Error error) {
        setAt(o._at);
        return setError(error);
    }
    Error setErrorIf(const ErrorAt &o, Error error) {
        return isOK() && error ? setError(o, error) : getError();
    }
    Error setError(const ErrorAt &o, const ErrorReporter &error) {
        setAt(o._at);
        return setError(error);
    }
    Error setErrorIf(const ErrorAt &o, const ErrorReporter &error) {
        return isOK() && error.getError() ? setError(o, error) : getError();
    }
    Error setError(const StrScanner &at, Error error) {
        setAt(at);
        return setError(error);
    }
    Error setErrorIf(const StrScanner &at, Error error) {
        return isOK() && error ? setError(at, error) : getError();
    }
    Error setError(const StrScanner &at, const ErrorReporter &error) {
        setAt(at);
        return setError(error);
    }
    Error setErrorIf(const StrScanner &at, const ErrorReporter &error) {
        return isOK() && error.getError() ? setError(at, error) : getError();
    }
    Error setError(const StrBuffer &at, Error error) {
        setAt(at);
        return setError(error);
    }
    Error setErrorIf(const StrBuffer &at, Error error) {
        return isOK() && error ? setError(at, error) : getError();
    }
    Error setError(const StrBuffer &at, const ErrorReporter &error) {
        setAt(at);
        return setError(error);
    }
    Error setErrorIf(const StrBuffer &at, const ErrorReporter &error) {
        return isOK() && error.getError() ? setError(at, error) : getError();
    }

    void resetAt() { _at = ""; }
    void setAt(const char *at) { _at = at; }
    void setAt(const StrScanner &at) { _at = at.str(); }
    void setAt(const StrBuffer &at);
    void setAt(const ErrorAt &at) { _at = at.errorAt(); }
    constexpr const char *errorAt() const { return _at; }

private:
    const char *_at;
};

}  // namespace libasm

#endif  // __LIBASM_ERROR_REPORTER_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

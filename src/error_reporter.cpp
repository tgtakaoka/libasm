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

#include "error_reporter.h"

namespace libasm {

const char *ErrorReporter::errorText() const {
    return errorText(_error);
}

const char *ErrorReporter::errorText(Error error) {
    switch (error) {
    case OK:                   return "OK";

    // Disassembler
    case NO_MEMORY:            return "Not enough memory";
    case INTERNAL_ERROR:       return "Internal error";
    case UNKNOWN_POSTBYTE:     return "Unknown post-byte";
    case ILLEGAL_REGISTER:     return "Illegal register";
    case ILLEGAL_OPERAND:      return "Illegal operand";
    case ILLEGAL_SIZE:         return "Illegal size";
    case ILLEGAL_OPERAND_MODE: return "Illegal operand mode";

    // Assembler
    case NO_INSTRUCTION:       return "No instruction found";
    case UNKNOWN_INSTRUCTION:  return "Unknown instruction";
    case UNKNOWN_REGISTER:     return "Unknown register";
    case UNKNOWN_OPERAND:      return "Unknown operand";
    case OPERAND_NOT_ZP:       return "Operand isn't zero page";
    case OPERAND_TOO_FAR:      return "Operand too far";
    case ILLEGAL_BIT_NUMBER:   return "Illegal bit number";
    case GARBAGE_AT_END:       return "Garbage at line end";
    case DUPLICATE_REGISTER:   return "Duplicate register";
    case REGISTER_NOT_ALLOWED: return "Register not allowed";
    case MISSING_COMMA:        return "Missing comma";
    case NOT_BIT_ADDRESSABLE:  return "Not bit addressable address";

    // ValueParser
    case ILLEGAL_CONSTANT:        return "Illegal constant";
    case OVERFLOW_RANGE:          return "Overflow range";
    case MISSING_CLOSING_PAREN:   return "Missing closing parenthesis";
    case MISSING_CLOSING_QUOTE:   return "Missing closing single quote";
    case UNKNOWN_ESCAPE_SEQUENCE: return "Unknown escape sequence";
    case UNKNOWN_EXPR_OPERATOR:   return "Unknown expression operator";
    case DIVIDE_BY_ZERO:          return "Divided by zero";
    case UNDEFINED_SYMBOL:        return "Undefined symbol";
    case TOO_COMPLEX_EXPRESSION:  return "Too complex expression";

    // AsmDirective
    case UNKNOWN_DIRECTIVE:      return "Unknown directive";
    case ILLEGAL_LABEL:          return "Illegal label";
    case DUPLICATE_LABEL:        return "Duplicate label";
    case MISSING_LABEL:          return "Missing label";
    case MISSING_CLOSING_DQUOTE: return "Missing closing double quote";
    case UNSUPPORTED_CPU:        return "Unsupported CPU";
    case TOO_MANY_INCLUDE:       return "Too many include";
    case NO_INCLUDE_FOUND:       return "Include file not found";
    }

    return "Unknown error";
}

} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

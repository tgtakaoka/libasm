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

const /*PROGMEM*/ char *ErrorReporter::errorText_P(Error error) {
    switch (error) {
    case OK:
        return PSTR("OK");
    case INSTRUCTION_NOT_ALIGNED:
        return PSTR("Instruction not aligned");

    // Disassembler
    case NO_MEMORY:
        return PSTR("Not enough memory");
    case INTERNAL_ERROR:
        return PSTR("Internal error");
    case UNKNOWN_POSTBYTE:
        return PSTR("Unknown post-byte");
    case ILLEGAL_REGISTER:
        return PSTR("Illegal register");
    case ILLEGAL_OPERAND:
        return PSTR("Illegal operand");
    case ILLEGAL_SIZE:
        return PSTR("Illegal size");
    case ILLEGAL_OPERAND_MODE:
        return PSTR("Illegal operand mode");
    case ILLEGAL_SEGMENT:
        return PSTR("Illegal segment override");
    case BUFFER_OVERFLOW:
        return PSTR("Buffer overflow");
    case OVERWRAP_PAGE:
        return PSTR("Overwarp page boundary");

    // Assembler
    case UNKNOWN_INSTRUCTION:
        return PSTR("Unknown instruction");
    case UNKNOWN_REGISTER:
        return PSTR("Unknown register");
    case UNKNOWN_OPERAND:
        return PSTR("Unknown operand");
    case OPERAND_NOT_ALLOWED:
        return PSTR("Operand not allowed");
    case OPERAND_TOO_FAR:
        return PSTR("Operand too far");
    case ILLEGAL_BIT_NUMBER:
        return PSTR("Illegal bit number");
    case GARBAGE_AT_END:
        return PSTR("Garbage at line end");
    case DUPLICATE_REGISTER:
        return PSTR("Duplicate register");
    case REGISTER_NOT_ALLOWED:
        return PSTR("Register not allowed");
    case MISSING_COMMA:
        return PSTR("Missing comma");
    case NOT_BIT_ADDRESSABLE:
        return PSTR("Not bit addressable address");
    case OPERAND_NOT_ALIGNED:
        return PSTR("Operand not aligned");
    case OPCODE_HAS_NO_EFFECT:
        return PSTR("Opcode has no effect");
    case REGISTERS_OVERLAPPED:
        return PSTR("Registers overlapped");
    case MISSING_CLOSING_BRACKET:
        return PSTR("Missing closing bracket");

    // ValueParser
    case ILLEGAL_CONSTANT:
        return PSTR("Illegal constant");
    case OVERFLOW_RANGE:
        return PSTR("Overflow range");
    case MISSING_CLOSING_PAREN:
        return PSTR("Missing closing parenthesis");
    case MISSING_CLOSING_QUOTE:
        return PSTR("Missing closing single quote");
    case UNKNOWN_ESCAPE_SEQUENCE:
        return PSTR("Unknown escape sequence");
    case UNKNOWN_EXPR_OPERATOR:
        return PSTR("Unknown expression operator");
    case DIVIDE_BY_ZERO:
        return PSTR("Divided by zero");
    case UNDEFINED_SYMBOL:
        return PSTR("Undefined symbol");
    case TOO_COMPLEX_EXPRESSION:
        return PSTR("Too complex expression");
    case MISSING_FUNC_ARGUMENT:
        return PSTR("Missing function argument");
    case UNKNOWN_FUNCTION:
        return PSTR("Unknown function");
    case DUPLICATE_FUNCTION:
        return PSTR("Duplicate function");
    case SYMBOL_REQUIRE:
        return PSTR("Symbol name required");
    case NOT_AN_EXPECTED:
        return PSTR("Not an expected entity");

    // AsmDirective
    case UNKNOWN_DIRECTIVE:
        return PSTR("Unknown directive");
    case ILLEGAL_LABEL:
        return PSTR("Illegal label");
    case DUPLICATE_LABEL:
        return PSTR("Duplicate label");
    case MISSING_LABEL:
        return PSTR("Missing label");
    case MISSING_CLOSING_DQUOTE:
        return PSTR("Missing closing double quote");
    case UNSUPPORTED_CPU:
        return PSTR("Unsupported CPU");
    case TOO_MANY_INCLUDE:
        return PSTR("Too many include");
    case NO_INCLUDE_FOUND:
        return PSTR("Include file not found");
    case MISSING_CLOSING_DELIMITOR:
        return PSTR("Missing closing delimitor");
    case END_ASSEMBLE:
        return PSTR("End assemble");

    // Command line
    case UNKNOWN_OPTION:
        return PSTR("Unknown option");
    }

    return PSTR("Unknown error");
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

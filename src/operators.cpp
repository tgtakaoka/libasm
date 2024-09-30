/*
 * Copyright 2023 Tadashi G. Takaoka
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

#include "operators.h"
#include <ctype.h>
#include <math.h>
#include "str_scanner.h"

namespace libasm {

bool Operator::isHigher(const Operator &o) const {
    return _prec < o._prec || (_prec == o._prec && o._assoc == LEFT);
}

bool Operator::isNoneAssoc(const Operator &o) const {
    return _prec == o._prec && _assoc == NONE;
}

Error Operator::eval(ValueStack &stack, ParserContext &context, uint_fast8_t argc) const {
    if (argc == 0)
        argc = stack.size();
    if (_fn) {
        const auto nargs = _fn->nargs();
        if (nargs < 0 || argc == nargs)
            return _fn->eval(stack, context, argc);
        return argc > nargs ? TOO_MANY_FUNC_ARGUMENT : TOO_FEW_FUNC_ARGUMENT;
    }
    return argc >= _nargs ? (*_op)(stack) : MISSING_OPERAND;
}

static Error bitwise_not(ValueStack &stack) {
    const auto v = stack.pop();
    stack.push(~v);
    return OK;
}

static Error logical_not(ValueStack &stack) {
    const auto v = stack.pop().getUnsigned();
    stack.pushUnsigned(!v);
    return OK;
}

static Error unary_minus(ValueStack &stack) {
    const auto v = stack.pop();
    stack.push(-v);
#ifdef LIBASM_ASM_NOFLOAT
    if (v.negateOverflow())
        return OVERFLOW_RANGE;
#endif
    return OK;
}

static Error unary_plus(ValueStack &stack) {
    const auto v = stack.pop();
    stack.push(v);
#ifdef LIBASM_ASM_NOFLOAT
    if (v.overflowUint32())
        return OVERFLOW_RANGE;
#endif
    return OK;
}

static Error multiply(ValueStack &stack) {
    const auto rhs = stack.pop();
    const auto lhs = stack.pop();
    stack.push(lhs * rhs);
    return OK;
}

static Error divide(ValueStack &stack) {
    const auto rhs = stack.pop();
    const auto lhs = stack.pop();
    if (rhs.isZero())
        return DIVIDE_BY_ZERO;
    stack.push(lhs / rhs);
    return OK;
}

static Error modulo(ValueStack &stack) {
    const auto rhs = stack.pop();
    const auto lhs = stack.pop();
    if (rhs.isZero())
        return DIVIDE_BY_ZERO;
    stack.push(lhs % rhs);
    return OK;
}

static Error add(ValueStack &stack) {
    const auto rhs = stack.pop();
    const auto lhs = stack.pop();
    stack.push(lhs + rhs);
    return OK;
}

static Error subtract(ValueStack &stack) {
    const auto rhs = stack.pop();
    const auto lhs = stack.pop();
    stack.push(lhs - rhs);
    return OK;
}

static Error logical_shift_left_32bit(ValueStack &stack) {
    const auto rhs = stack.pop().getUnsigned();
    const auto lhs = stack.pop().getUnsigned() & UINT32_MAX;
    const auto value = (rhs < 32) ? (lhs << rhs) : 0;
    stack.pushUnsigned(value & UINT32_MAX);
    return OK;
}

static Error arithmetic_shift_right(ValueStack &stack) {
    const auto rhs = stack.pop();
    const auto lhs = stack.pop();
    stack.push(lhs >> rhs);
    return OK;
}

static Error less_than(ValueStack &stack) {
    const auto rhs = stack.pop();
    const auto lhs = stack.pop();
    stack.pushUnsigned(lhs < rhs);
    return OK;
}

static Error less_than_or_equal(ValueStack &stack) {
    const auto rhs = stack.pop();
    const auto lhs = stack.pop();
    stack.pushUnsigned(lhs < rhs || lhs == rhs);
    return OK;
}

static Error greater_than_or_equal(ValueStack &stack) {
    const auto rhs = stack.pop();
    const auto lhs = stack.pop();
    stack.pushUnsigned(!(lhs < rhs));
    return OK;
}

static Error greater_than(ValueStack &stack) {
    const auto rhs = stack.pop();
    const auto lhs = stack.pop();
    stack.pushUnsigned(!(lhs < rhs) && !(lhs == rhs));
    return OK;
}

static Error logical_equal(ValueStack &stack) {
    const auto rhs = stack.pop();
    const auto lhs = stack.pop();
    stack.pushUnsigned(lhs == rhs);
    return OK;
}

static Error logical_not_equal(ValueStack &stack) {
    const auto rhs = stack.pop();
    const auto lhs = stack.pop();
    stack.pushUnsigned(!(lhs == rhs));
    return OK;
}

static Error bitwise_and(ValueStack &stack) {
    const auto rhs = stack.pop();
    const auto lhs = stack.pop();
    stack.push(lhs & rhs);
    return OK;
}

static Error bitwise_xor(ValueStack &stack) {
    const auto rhs = stack.pop();
    const auto lhs = stack.pop();
    stack.push(lhs ^ rhs);
    return OK;
}

static Error bitwise_or(ValueStack &stack) {
    const auto rhs = stack.pop();
    const auto lhs = stack.pop();
    stack.push(lhs | rhs);
    return OK;
}

static Error logical_and(ValueStack &stack) {
    const auto rhs = stack.pop();
    const auto lhs = stack.pop();
    Value v;
    stack.push(v.setUnsigned(lhs && rhs));

    return OK;
}

static Error logical_or(ValueStack &stack) {
    const auto rhs = stack.pop();
    const auto lhs = stack.pop();
    Value v;
    stack.push(v.setUnsigned(lhs || rhs));
    return OK;
}

constexpr Operator::Assoc R = Operator::RIGHT;
constexpr Operator::Assoc L = Operator::LEFT;
constexpr Operator::Assoc N = Operator::NONE;

// clang-format off
const Operator Operator::OP_BITWISE_NOT( 3, R, 1, bitwise_not);
const Operator Operator::OP_LOGICAL_NOT( 3, R, 1, logical_not);
const Operator Operator::OP_UNARY_MINUS( 3, R, 1, unary_minus);
const Operator Operator::OP_UNARY_PLUS(  3, R, 1, unary_plus);
const Operator Operator::OP_MUL(         5, L, 2, multiply);
const Operator Operator::OP_DIV(         5, L, 2, divide);
const Operator Operator::OP_MOD(         5, L, 2, modulo);
const Operator Operator::OP_ADD(         6, L, 2, add);
const Operator Operator::OP_SUB(         6, L, 2, subtract);
const Operator Operator::OP_SHIFT_LEFT(  7, L, 2, logical_shift_left_32bit);
const Operator Operator::OP_SHIFT_RIGHT( 7, L, 2, arithmetic_shift_right);
const Operator Operator::OP_LOGICAL_LT(  9, N, 2, less_than);
const Operator Operator::OP_LOGICAL_LE(  9, N, 2, less_than_or_equal);
const Operator Operator::OP_LOGICAL_GE(  9, N, 2, greater_than_or_equal);
const Operator Operator::OP_LOGICAL_GT(  9, N, 2, greater_than);
const Operator Operator::OP_LOGICAL_EQ( 10, N, 2, logical_equal);
const Operator Operator::OP_LOGICAL_NE( 10, N, 2, logical_not_equal);
const Operator Operator::OP_BITWISE_AND(11, L, 2, bitwise_and);
const Operator Operator::OP_BITWISE_XOR(12, L, 2, bitwise_xor);
const Operator Operator::OP_BITWISE_OR( 13, L, 2, bitwise_or);
const Operator Operator::OP_LOGICAL_AND(14, L, 2, logical_and);
const Operator Operator::OP_LOGICAL_OR( 15, L, 2, logical_or);
// clang-format on

const Operator *CStyleOperatorParser::readOperator(
        StrScanner &scan, ErrorAt &error, Operator::Type type) const {
    UNUSED(error);
    auto p = scan;
    const Operator *opr = nullptr;
    if (type == Operator::PREFIX) {
        if (p.expect('~')) {
            opr = &Operator::OP_BITWISE_NOT;
        } else if (p.expect('!') && *p != '=') {
            opr = &Operator::OP_LOGICAL_NOT;
        } else if (p.expect('-')) {
            opr = &Operator::OP_UNARY_MINUS;
        } else if (p.expect('+')) {
            opr = &Operator::OP_UNARY_PLUS;
        }
    } else if (type == Operator::INFIX) {
        if (p.expect('*')) {
            opr = &Operator::OP_MUL;
        } else if (p.expect('/')) {
            opr = &Operator::OP_DIV;
        } else if (p.expect('%')) {
            opr = &Operator::OP_MOD;
        } else if (p.expect('+')) {
            opr = &Operator::OP_ADD;
        } else if (p.expect('-')) {
            opr = &Operator::OP_SUB;
        } else if (p.expect('<')) {
            if (p.expect('<')) {
                opr = &Operator::OP_SHIFT_LEFT;
            } else if (p.expect('=')) {
                opr = &Operator::OP_LOGICAL_LE;
            } else {
                opr = &Operator::OP_LOGICAL_LT;
            }
        } else if (p.expect('>')) {
            if (p.expect('>')) {
                opr = &Operator::OP_SHIFT_RIGHT;
            } else if (p.expect('=')) {
                opr = &Operator::OP_LOGICAL_GE;
            } else {
                opr = &Operator::OP_LOGICAL_GT;
            }
        } else if (p.expect('&')) {
            opr = p.expect('&') ? &Operator::OP_LOGICAL_AND : &Operator::OP_BITWISE_AND;
        } else if (p.expect('^')) {
            opr = &Operator::OP_BITWISE_XOR;
        } else if (p.expect('|')) {
            opr = p.expect('|') ? &Operator::OP_LOGICAL_OR : &Operator::OP_BITWISE_OR;
        } else if (p.expect('!')) {
            if (p.expect('='))
                opr = &Operator::OP_LOGICAL_NE;
        } else if (p.expect('=')) {
            if (p.expect('='))
                opr = &Operator::OP_LOGICAL_EQ;
        }
    }
    if (opr)
        scan = p;
    return opr;
}

static Error exponential(ValueStack &stack) {
    const auto rhs = stack.pop();
    const auto lhs = stack.pop();
    stack.push(lhs.exponential(rhs));
    return OK;
}

static Error rotate_left_16bit(ValueStack &stack) {
    const auto rhs = stack.pop().getUnsigned() % 16;
    const auto lhs = stack.pop().getUnsigned() & UINT16_MAX;
    const auto v = (lhs << rhs) | (lhs >> (16 - rhs));
    stack.pushUnsigned(v & UINT16_MAX);
    return OK;
}

static Error rotate_right_16bit(ValueStack &stack) {
    const auto rhs = stack.pop().getUnsigned() % 16;
    const auto lhs = stack.pop().getUnsigned() & UINT16_MAX;
    const auto v = (lhs >> rhs) | (lhs << (16 - rhs));
    stack.pushUnsigned(v & UINT16_MAX);
    return OK;
}

static Error logical_shift_left_16bit(ValueStack &stack) {
    const auto rhs = stack.pop().getUnsigned();
    const auto lhs = stack.pop().getUnsigned() & UINT16_MAX;
    const auto value = (rhs < 16) ? (lhs << rhs) : 0;
    stack.pushUnsigned(value & UINT16_MAX);
    return OK;
}

static Error logical_shift_right_16bit(ValueStack &stack) {
    const auto rhs = stack.pop().getUnsigned();
    const auto lhs = stack.pop().getUnsigned() & UINT16_MAX;
    const auto value = (rhs < 16) ? (lhs >> rhs) : 0;
    stack.pushUnsigned(value & UINT16_MAX);
    return OK;
}

// clang-format off
static const Operator MC68_EXPONENTIAL( 5, R, 2, exponential);
static const Operator MC68_ROTATE_LEFT( 5, L, 2, rotate_left_16bit);
static const Operator MC68_ROTATE_RIGHT(5, L, 2, rotate_right_16bit);
static const Operator MC68_SHIFT_LEFT(  5, L, 2, logical_shift_left_16bit);
static const Operator MC68_SHIFT_RIGHT( 5, L, 2, logical_shift_right_16bit);
static const Operator MC68_BITWISE_AND( 5, L, 2, bitwise_and);
static const Operator MC68_BITWISE_XOR( 5, L, 2, bitwise_xor);
static const Operator MC68_BITWISE_OR(  5, L, 2, bitwise_or);
// clang-format on

const Operator *Mc68xxOperatorParser::readOperator(
        StrScanner &scan, ErrorAt &error, Operator::Type type) const {
    if (type == Operator::INFIX) {
        auto p = scan;
        if (p.expect('!')) {
            const Operator *opr = nullptr;
            if (p.expect('.')) {
                opr = &MC68_BITWISE_AND;
            } else if (p.expect('+')) {
                opr = &MC68_BITWISE_OR;
            } else if (p.expect('X') && !isalnum(*p)) {
                opr = &MC68_BITWISE_XOR;
            } else if (p.expect('^')) {
                opr = &MC68_EXPONENTIAL;
            } else if (p.expect('<')) {
                opr = &MC68_SHIFT_LEFT;
            } else if (p.expect('>')) {
                opr = &MC68_SHIFT_RIGHT;
            } else if (p.expect('L') && !isalnum(*p)) {
                opr = &MC68_ROTATE_LEFT;
            } else if (p.expect('R') && !isalnum(*p)) {
                opr = &MC68_ROTATE_RIGHT;
            }
            if (opr) {
                scan = p;
                return opr;
            }
        }
    }
    return CStyleOperatorParser::singleton().readOperator(scan, error, type);
}

static Error most_siginificant_byte(ValueStack &stack) {
    const auto v = stack.pop().getUnsigned();
    stack.pushUnsigned((v >> 8) & UINT8_MAX);
    return OK;
}

static Error least_significant_byte(ValueStack &stack) {
    const auto v = stack.pop().getUnsigned();
    stack.pushUnsigned(v & UINT8_MAX);
    return OK;
}

static Error most_siginificant_word(ValueStack &stack) {
    const auto v = stack.pop().getUnsigned();
    stack.pushUnsigned((v >> 16) & UINT16_MAX);
    return OK;
}

static Error least_significant_word(ValueStack &stack) {
    const auto v = stack.pop().getUnsigned();
    stack.pushUnsigned(v & UINT16_MAX);
    return OK;
}

// clang-format off
static const Operator INTEL_HIGH(        3, R, 1, most_siginificant_byte);
static const Operator INTEL_LOW(         3, R, 1, least_significant_byte);
static const Operator INTEL_MSW(         3, R, 1, most_siginificant_word);
static const Operator INTEL_LSW(         3, R, 1, least_significant_word);
static const Operator INTEL_SHIFT_RIGHT( 5, L, 1, logical_shift_right_16bit);
static const Operator INTEL_SHIFT_LEFT(  5, L, 1, logical_shift_left_16bit);
static const Operator INTEL_UNARY_MINUS( 6, R, 1, unary_minus);
static const Operator INTEL_UNARY_PLUS(  6, R, 1, unary_plus);
static const Operator INTEL_LOGICAL_EQ(  9, N, 2, logical_equal);
static const Operator INTEL_LOGICAL_NE(  9, N, 2, logical_not_equal);
static const Operator INTEL_BITWISE_NOT(10, R, 1, bitwise_not);
static const Operator INTEL_BITWISE_AND(11, L, 2, bitwise_and);
static const Operator INTEL_BITWISE_XOR(12, L, 2, bitwise_xor);
static const Operator INTEL_BITWISE_OR( 12, L, 2, bitwise_or);
// clang-format on

const Operator *IntelOperatorParser::readOperator(
        StrScanner &scan, ErrorAt &error, Operator::Type type) const {
    auto p = scan;
    // TODO: Use SymbolParser
    p.trimStart(isalnum);
    const StrScanner name(scan.str(), p.str());
    const Operator *opr = nullptr;
    if (type == Operator::PREFIX) {
        if (scan.expect('-')) {
            return &INTEL_UNARY_MINUS;
        } else if (scan.expect('+')) {
            return &INTEL_UNARY_PLUS;
        }
        if (name.iequals_P(PSTR("NOT"))) {
            opr = &INTEL_BITWISE_NOT;
        } else if (name.iequals_P(PSTR("HIGH"))) {
            opr = &INTEL_HIGH;
        } else if (name.iequals_P(PSTR("LOW"))) {
            opr = &INTEL_LOW;
        } else if (name.iequals_P(PSTR("MSW"))) {
            opr = &INTEL_MSW;
        } else if (name.iequals_P(PSTR("LSW"))) {
            opr = &INTEL_LSW;
        }
        if (opr) {
            scan += name.size();
            return opr;
        }
    } else if (type == Operator::INFIX) {
        if (name.iequals_P(PSTR("MOD"))) {
            opr = &Operator::OP_MOD;
        } else if (name.iequals_P(PSTR("AND"))) {
            opr = &INTEL_BITWISE_AND;
        } else if (name.iequals_P(PSTR("OR"))) {
            opr = &INTEL_BITWISE_OR;
        } else if (name.iequals_P(PSTR("XOR"))) {
            opr = &INTEL_BITWISE_XOR;
        } else if (name.iequals_P(PSTR("SHR"))) {
            opr = &INTEL_SHIFT_RIGHT;
        } else if (name.iequals_P(PSTR("SHL"))) {
            opr = &INTEL_SHIFT_LEFT;
        } else if (name.iequals_P(PSTR("EQ"))) {
            opr = &INTEL_LOGICAL_EQ;
        } else if (name.iequals_P(PSTR("NE"))) {
            opr = &INTEL_LOGICAL_NE;
        } else if (name.iequals_P(PSTR("LT"))) {
            opr = &Operator::OP_LOGICAL_LT;
        } else if (name.iequals_P(PSTR("LE"))) {
            opr = &Operator::OP_LOGICAL_LE;
        } else if (name.iequals_P(PSTR("GE"))) {
            opr = &Operator::OP_LOGICAL_GE;
        } else if (name.iequals_P(PSTR("GT"))) {
            opr = &Operator::OP_LOGICAL_GT;
        }
        if (opr) {
            scan += name.size();
            return opr;
        }
    }
    return CStyleOperatorParser::singleton().readOperator(scan, error, type);
}

// clang-format off
static const Operator ZILOG_HIGH16(     3, R, 1, most_siginificant_word);
static const Operator ZILOG_LOW16(      3, R, 1, least_significant_word);
static const Operator ZILOG_SHIFT_RIGHT(5, L, 1, logical_shift_right_16bit);
static const Operator ZILOG_SHIFT_LEFT( 5, L, 1, logical_shift_left_16bit);
static const Operator ZILOG_BITWISE_AND(5, L, 2, bitwise_and);
static const Operator ZILOG_BITWISE_XOR(6, L, 2, bitwise_xor);
static const Operator ZILOG_BITWISE_OR( 6, L, 2, bitwise_or);
// clang-format on

const Operator *ZilogOperatorParser::readOperator(
        StrScanner &scan, ErrorAt &error, Operator::Type type) const {
    auto p = scan;
    // TODO: Use SymbolParser
    p.trimStart(isalnum);
    const StrScanner name(scan.str(), p.str());
    const Operator *opr = nullptr;
    if (type == Operator::PREFIX) {
        if (name.iequals_P(PSTR("LNOT"))) {
            opr = &Operator::OP_BITWISE_NOT;
        } else if (name.iequals_P(PSTR("HIGH"))) {
            opr = &INTEL_HIGH;
        } else if (name.iequals_P(PSTR("LOW"))) {
            opr = &INTEL_LOW;
        } else if (name.iequals_P(PSTR("HIGH16"))) {
            opr = &ZILOG_HIGH16;
        } else if (name.iequals_P(PSTR("LOW16"))) {
            opr = &ZILOG_LOW16;
        }
        if (opr) {
            scan += name.size();
            return opr;
        }
    } else if (type == Operator::INFIX) {
        if (name.iequals_P(PSTR("MOD"))) {
            opr = &Operator::OP_MOD;
        } else if (name.iequals_P(PSTR("SHR"))) {
            opr = &ZILOG_SHIFT_RIGHT;
        } else if (name.iequals_P(PSTR("SHL"))) {
            opr = &ZILOG_SHIFT_LEFT;
        } else if (name.iequals_P(PSTR("LAND"))) {
            opr = &ZILOG_BITWISE_AND;
        } else if (name.iequals_P(PSTR("LOR"))) {
            opr = &ZILOG_BITWISE_OR;
        } else if (name.iequals_P(PSTR("LXOR"))) {
            opr = &ZILOG_BITWISE_XOR;
        }
        if (opr) {
            scan += name.size();
            return opr;
        }
    }
    return CStyleOperatorParser::singleton().readOperator(scan, error, type);
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

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

#include "str_scanner.h"

#include <ctype.h>

namespace libasm {

bool Operator::isHigher(const Operator &o) const {
    return _prec < o._prec || (_prec == o._prec && o._assoc == LEFT);
}

Error Operator::eval(ValueStack &stack, uint8_t argc) const {
    if (argc == 0)
        argc = stack.size();
    if (_fn) {
        const auto nargs = _fn->nargs();
        if (nargs < 0 || argc == nargs)
            return _fn->eval(stack, argc);
        return argc > nargs ? TOO_MANY_FUNC_ARGUMENT : TOO_FEW_FUNC_ARGUMENT;
    }
    return argc >= _nargs ? (*_op)(stack) : MISSING_OPERAND;
}

static const Operator OP_BITWISE_NOT(3, Operator::RIGHT, 1, [](ValueStack &stack) {
    const auto v = stack.pop();
    stack.pushUnsigned(~v.getUnsigned());
    return OK;
});

static Operator OP_UNARY_MINUS(3, Operator::RIGHT, 1, [](ValueStack &stack) {
    const auto v = stack.pop();
    const auto error = v.isUnsigned() && v.getUnsigned() > 0x80000000 ? OVERFLOW_RANGE : OK;
    stack.pushSigned(-v.getSigned());
    return error;
});

static const Operator OP_UNARY_PLUS(3, Operator::RIGHT, 1, [](ValueStack &stack) {
    const auto v = stack.pop();
    stack.pushUnsigned(v.getUnsigned());
    return OK;
});

static const Operator OP_MUL(5, Operator::LEFT, 2, [](ValueStack &stack) {
    const auto rhs = stack.pop();
    const auto lhs = stack.pop();
    if (lhs.isSigned() || rhs.isSigned()) {
        stack.pushSigned(lhs.getSigned() * rhs.getSigned());
    } else {
        stack.pushUnsigned(lhs.getUnsigned() * rhs.getUnsigned());
    }
    return OK;
});

static const Operator OP_DIV(5, Operator::LEFT, 2, [](ValueStack &stack) {
    const auto rhs = stack.pop();
    const auto lhs = stack.pop();
    auto error = OK;
    if (rhs.getUnsigned() == 0) {
        stack.push(Value());
        error = DIVIDE_BY_ZERO;
    } else if (lhs.isSigned() || rhs.isSigned()) {
        stack.pushSigned(lhs.getSigned() / rhs.getSigned());
    } else {
        stack.pushUnsigned(lhs.getUnsigned() / rhs.getUnsigned());
    }
    return error;
});

static const Operator OP_MOD(5, Operator::LEFT, 2, [](ValueStack &stack) {
    const auto rhs = stack.pop();
    const auto lhs = stack.pop();
    auto error = OK;
    if (rhs.getUnsigned() == 0) {
        stack.push(Value());
        error = DIVIDE_BY_ZERO;
    } else if (lhs.isSigned() || rhs.isSigned()) {
        stack.pushSigned(lhs.getSigned() % rhs.getSigned());
    } else {
        stack.pushUnsigned(lhs.getUnsigned() % rhs.getUnsigned());
    }
    return error;
});

static const Operator OP_ADD(6, Operator::LEFT, 2, [](ValueStack &stack) {
    const auto rhs = stack.pop();
    const auto lhs = stack.pop();
    if (lhs.isSigned() || rhs.isSigned()) {
        stack.pushSigned(lhs.getSigned() + rhs.getSigned());
    } else {
        stack.pushUnsigned(lhs.getUnsigned() + rhs.getUnsigned());
    }
    return OK;
});

static const Operator OP_SUB(6, Operator::LEFT, 2, [](ValueStack &stack) {
    const auto rhs = stack.pop();
    const auto lhs = stack.pop();
    if (lhs.isSigned() || rhs.isSigned()) {
        stack.pushSigned(lhs.getSigned() - rhs.getSigned());
    } else if (lhs.getUnsigned() < rhs.getUnsigned()) {
        stack.pushSigned(lhs.getUnsigned() - rhs.getUnsigned());
    } else {
        stack.pushUnsigned(lhs.getUnsigned() - rhs.getUnsigned());
    }
    return OK;
});

static uint32_t shift_left(uint32_t value, uint8_t count) {
    for (unsigned i = 0; i <= 32 && i < count; i++)
        value <<= 1;
    return value;
}
static const Operator OP_SHIFT_LEFT(7, Operator::LEFT, 2, [](ValueStack &stack) {
    const auto rhs = stack.pop();
    const auto lhs = stack.pop();
    stack.pushUnsigned(shift_left(lhs.getUnsigned(), rhs.getUnsigned()));
    return OK;
});

static uint32_t shift_right(uint32_t value, uint8_t count) {
    for (unsigned i = 0; i <= 32 && i < count; i++)
        value >>= 1;
    return value;
}
static int32_t shift_right_negative(int32_t value, uint8_t count) {
    for (unsigned i = 0; i <= 32 && i < count; i++) {
        value >>= 1;
        value |= 0x80000000;
    }
    return value;
}
static const Operator OP_SHIFT_RIGHT(7, Operator::LEFT, 2, [](ValueStack &stack) {
    const auto rhs = stack.pop();
    const auto lhs = stack.pop();
    if (lhs.isSigned() && lhs.getSigned() < 0) {
        stack.pushSigned(shift_right_negative(lhs.getSigned(), rhs.getUnsigned()));
    } else {
        stack.pushUnsigned(shift_right(lhs.getUnsigned(), rhs.getUnsigned()));
    }
    return OK;
});

static const Operator OP_BITWISE_AND(11, Operator::LEFT, 2, [](ValueStack &stack) {
    const auto rhs = stack.pop();
    const auto lhs = stack.pop();
    stack.pushUnsigned(lhs.getUnsigned() & rhs.getUnsigned());

    return OK;
});

static const Operator OP_BITWISE_XOR(12, Operator::LEFT, 2, [](ValueStack &stack) {
    const auto rhs = stack.pop();
    const auto lhs = stack.pop();
    stack.pushUnsigned(lhs.getUnsigned() ^ rhs.getUnsigned());
    return OK;
});

static const Operator OP_BITWISE_OR(13, Operator::LEFT, 2, [](ValueStack &stack) {
    const auto rhs = stack.pop();
    const auto lhs = stack.pop();
    stack.pushUnsigned(lhs.getUnsigned() | rhs.getUnsigned());
    return OK;
});

const Operator *CStyleOperatorParser::readOperator(
        StrScanner &scan, ErrorAt &error, OperatorType type) const {
    const auto c = *scan++;
    if (type == PREFIX) {
        switch (c) {
        case 0:
            return nullptr;
        case '~':
            return &OP_BITWISE_NOT;
        case '-':
            if (*scan != '-' && *scan != '+')
                return &OP_UNARY_MINUS;
            error.setErrorIf(scan, UNKNOWN_EXPR_OPERATOR);
            break;
        case '+':
            if (*scan != '+' && *scan != '-')
                return &OP_UNARY_PLUS;
            error.setErrorIf(scan, UNKNOWN_EXPR_OPERATOR);
            break;
        default:
            break;
        }
    } else if (type == INFIX) {
        switch (c) {
        case 0:
            return nullptr;
        case '*':
            return &OP_MUL;
        case '/':
            return &OP_DIV;
        case '%':
            return &OP_MOD;
        case '+':
            return &OP_ADD;
        case '-':
            return &OP_SUB;
        case '<':
            if (scan.expect('<'))
                return &OP_SHIFT_LEFT;
            error.setError(scan, UNKNOWN_EXPR_OPERATOR);
            break;
        case '>':
            if (scan.expect('>'))
                return &OP_SHIFT_RIGHT;
            error.setError(scan, UNKNOWN_EXPR_OPERATOR);
            break;
        case '&':
            return &OP_BITWISE_AND;
        case '^':
            return &OP_BITWISE_XOR;
        case '|':
            return &OP_BITWISE_OR;
        default:
            break;
        }
    }
    --scan;
    return nullptr;
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

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

const Operator Operator::OP_NONE(18);

static const struct OpBitwiseNot : Operator {
    OpBitwiseNot() : Operator(3) {}
    Error eval(Value &val, const Value &rhs) const override {
        val.setUnsigned(~rhs.getUnsigned());
        return OK;
    }
} OP_BITWISE_NOT;

static const struct OpMinus : Operator {
    OpMinus() : Operator(3) {}
    Error eval(Value &val, const Value &rhs) const override {
        if (rhs.isUnsigned() && rhs.getUnsigned() > 0x80000000)
            return OVERFLOW_RANGE;
        val.setSigned(-rhs.getSigned());
        return OK;
    }
} OP_MINUS;

static const struct OpPlus : Operator {
    OpPlus() : Operator(3) {}
    Error eval(Value &val, const Value &rhs) const override {
        val.setUnsigned(rhs.getUnsigned());
        return OK;
    }
} OP_PLUS;

static const struct OpMultiplication : Operator {
    OpMultiplication() : Operator(5) {}
    Error eval(Value &val, const Value &lhs, const Value &rhs) const override {
        if (lhs.isSigned() || rhs.isSigned()) {
            val.setSigned(lhs.getSigned() * rhs.getSigned());
        } else {
            val.setUnsigned(lhs.getUnsigned() * rhs.getUnsigned());
        }
        return OK;
    }
} OP_MULTIPLICATION;

static const struct OpDivision : Operator {
    OpDivision() : Operator(5) {}
    Error eval(Value &val, const Value &lhs, const Value &rhs) const override {
        if (rhs.getUnsigned() == 0) {
            val.clear();
            return DIVIDE_BY_ZERO;
        }
        if (lhs.isSigned() || rhs.isSigned()) {
            val.setSigned(lhs.getSigned() / rhs.getSigned());
        } else {
            val.setUnsigned(lhs.getUnsigned() / rhs.getUnsigned());
        }
        return OK;
    }
} OP_DIVISION;

static const struct OpRemainder : Operator {
    OpRemainder() : Operator(5) {}
    Error eval(Value &val, const Value &lhs, const Value &rhs) const override {
        if (rhs.getUnsigned() == 0) {
            val.clear();
            return DIVIDE_BY_ZERO;
        }
        if (lhs.isSigned() || rhs.isSigned()) {
            val.setSigned(lhs.getSigned() % rhs.getSigned());
        } else {
            val.setUnsigned(lhs.getUnsigned() % rhs.getUnsigned());
        }
        return OK;
    }
} OP_REMAINDER;

static const struct OpAddition : Operator {
    OpAddition() : Operator(6) {}
    Error eval(Value &val, const Value &lhs, const Value &rhs) const override {
        if (lhs.isSigned() || rhs.isSigned()) {
            val.setSigned(lhs.getSigned() + rhs.getSigned());
        } else {
            val.setUnsigned(lhs.getUnsigned() + rhs.getUnsigned());
        }
        return OK;
    }
} OP_ADDITION;

static const struct OpSubtraction : Operator {
    OpSubtraction() : Operator(6) {}
    Error eval(Value &val, const Value &lhs, const Value &rhs) const override {
        if (lhs.isSigned() || rhs.isSigned()) {
            val.setSigned(lhs.getSigned() - rhs.getSigned());
        } else if (lhs.getUnsigned() < rhs.getUnsigned()) {
            val.setSigned(lhs.getUnsigned() - rhs.getUnsigned());
        } else {
            val.setUnsigned(lhs.getUnsigned() - rhs.getUnsigned());
        }
        return OK;
    }
} OP_SUBTRACTION;

static const struct OpBitwiseShiftLeft : Operator {
    OpBitwiseShiftLeft() : Operator(7) {}
    Error eval(Value &val, const Value &lhs, const Value &rhs) const override {
        val.setUnsigned(shift_left(lhs.getUnsigned(), rhs.getUnsigned()));
        return OK;
    }

    static uint32_t shift_left(uint32_t value, uint8_t count) {
        for (unsigned i = 0; i <= 32 && i < count; i++)
            value <<= 1;
        return value;
    }

} OP_BITWISE_SHIFT_LEFT;

static const struct OpBitwiseShiftRight : Operator {
    OpBitwiseShiftRight() : Operator(7) {}
    Error eval(Value &val, const Value &lhs, const Value &rhs) const override {
        if (lhs.isSigned() && lhs.getSigned() < 0) {
            val.setSigned(shift_right_negative(lhs.getSigned(), rhs.getUnsigned()));
        } else {
            val.setUnsigned(shift_right(lhs.getUnsigned(), rhs.getUnsigned()));
        }
        return OK;
    }

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

} OP_BITWISE_SHIFT_RIGHT;

static const struct OpBitwiseAnd : Operator {
    OpBitwiseAnd() : Operator(11) {}
    Error eval(Value &val, const Value &lhs, const Value &rhs) const override {
        val.setUnsigned(lhs.getUnsigned() & rhs.getUnsigned());
        return OK;
    }
} OP_BITWISE_AND;

static const struct OpBitwiseXor : Operator {
    OpBitwiseXor() : Operator(12) {}
    Error eval(Value &val, const Value &lhs, const Value &rhs) const override {
        val.setUnsigned(lhs.getUnsigned() ^ rhs.getUnsigned());
        return OK;
    }
} OP_BITWISE_XOR;

static const struct OpBitwiseOr : Operator {
    OpBitwiseOr() : Operator(13) {}
    Error eval(Value &val, const Value &lhs, const Value &rhs) const override {
        val.setUnsigned(lhs.getUnsigned() | rhs.getUnsigned());
        return OK;
    }
} OP_BITWISE_OR;

const Operator *CStyleOperatorParser::readUnary(StrScanner &scan, ErrorAt &error) const {
    switch (*scan++) {
    case '~':
        return &OP_BITWISE_NOT;
    case '-':
        if (*scan != '-' && *scan != '+')
            return &OP_MINUS;
        error.setErrorIf(scan, UNKNOWN_EXPR_OPERATOR);
        break;
    case '+':
        if (*scan != '+' && *scan != '-')
            return &OP_PLUS;
        error.setErrorIf(scan, UNKNOWN_EXPR_OPERATOR);
        break;
    default:
        break;
    }
    --scan;
    return &Operator::OP_NONE;
}

const Operator *CStyleOperatorParser::readBinary(StrScanner &scan, ErrorAt &error) const {
    switch (*scan++) {
    case '*':
        return &OP_MULTIPLICATION;
    case '/':
        return &OP_DIVISION;
    case '%':
        return &OP_REMAINDER;
    case '+':
        return &OP_ADDITION;
    case '-':
        return &OP_SUBTRACTION;
    case '<':
        if (scan.expect('<'))
            return &OP_BITWISE_SHIFT_LEFT;
        error.setError(scan, UNKNOWN_EXPR_OPERATOR);
        break;
    case '>':
        if (scan.expect('>'))
            return &OP_BITWISE_SHIFT_RIGHT;
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
    --scan;
    return &Operator::OP_NONE;
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

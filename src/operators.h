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

#ifndef __OPERATORS_H__
#define __OPERATORS_H__

#include "error_reporter.h"
#include "value.h"

#include <stdint.h>

namespace libasm {

/**
 * Immutable instance to represent a function.
 */
struct Functor {
    /** Arguments for function calliing. The maimum number of arguments is |MAX_ARGS| */
    struct Arguments {
        Arguments() : _size(0) {}

        uint8_t size() const { return _size; }
        bool full() const { return _size == MAX_ARGS; }
        void push(const Value &val) { _content[_size++] = val; }
        /** Returns |index|-th argument. |index| is from 1 to |size()|. */
        const Value &at(uint8_t index) const { return _content[index - 1]; }

        static constexpr auto MAX_ARGS = 4;

    private:
        uint8_t _size;
        Value _content[MAX_ARGS];
    };

    /** Returns the number of required qrguments. Negative value means variable arguments. */
    virtual int8_t nargs() const { return -1; }
    /** Evaluate function with |arguments|. */
    virtual Error eval(const Arguments &args, Value &val) const { return OK; }

    /** The singleton for not a function */
    static const Functor FN_NONE;
};

/**
 * Parsing function call.
 */
struct FunctionParser {
    /**
     * Parsing |scan| and returns a Functor pointer for a function. |scan| should point an opening
     * parenthes for arguments list. Returning Functor::FN_NONE means there is no function call and
     * |scan| shoud be unchanged.
     */
    virtual const Functor *parseFunction(StrScanner &scan, ErrorAt &error) const {
        return &Functor::FN_NONE;
    }
};

/**
 * Immutable instance to represent unary or binary operator.
 */
struct Operator {
    /** Evaluate as unary operator with |rhs| as an argument. */
    virtual Error eval(Value &val, const Value &rhs) const { return OK; }
    /** Evaluate as binary operator with |lhs| and |rhs| as arguments. */
    virtual Error eval(Value &val, const Value &lhs, const Value &rhs) const { return OK; }
    bool hasHigherPriority(const Operator &o) const { return _precedence <= o._precedence; }

    /** The singleton for not an operator */
    static const struct Operator OP_NONE;

protected:
    Operator(uint8_t precedence) : _precedence(precedence) {}

private:
    /** Operator precedence (smaller value means higher precedence). */
    const uint8_t _precedence;
};

/**
 * Parsing unary and binary operator.
 */
struct OperatorParser {
    virtual const Operator *readUnary(StrScanner &scan, ErrorAt &error) const = 0;
    virtual const Operator *readBinary(StrScanner &scan, ErrorAt &error) const = 0;
};

struct CStyleOperatorParser : OperatorParser {
    const Operator *readUnary(StrScanner &scan, ErrorAt &error) const override;
    const Operator *readBinary(StrScanner &scan, ErrorAt &error) const override;
};

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

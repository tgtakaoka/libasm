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
#include "stack.h"
#include "value.h"

#include <stdint.h>

namespace libasm {

struct ValueStack : Stack<Value, 8> {
    ValueStack() : Stack() {}
    void pushSigned(int32_t val) { _contents[_size++].setSigned(val); }
    void pushUnsigned(uint32_t val) { _contents[_size++].setUnsigned(val); }
    /** Returns |pos|-th element from stack top */
    const Value &at(uint8_t pos) const { return _contents[_size - pos - 1]; }
};

/**
 * Immutable instance to represent a function.
 */
struct Functor {
    /** Returns the number of required qrguments. Negative value means variable arguments. */
    virtual int8_t nargs() const { return -1; }
    /** Evaluate function with |arguments|. */
    virtual Error eval(ValueStack &stack, uint8_t argc) const { return OK; }
};

/**
 * Parsing function call.
 */
struct FunctionParser {
    /**
     * Parsing |scan| and returns  a Functor pointer for a function. |scan|  should point an
     * opening parenthes  for arguments  list. Returns  nullptr  if no  function  call and
     * |scan| shoud  be unchanged.
     */
    virtual const Functor *parseFunction(StrScanner &scan, ErrorAt &error) const { return nullptr; }
};

/**
 * Immutable instance to represent prefix or infix operators and functions.
 */
struct Operator : ErrorAt {
    enum Type : uint8_t {
        PREFIX,
        INFIX,
    };
    /** Associativity of an operator */
    enum Assoc : uint8_t {
        LEFT,
        RIGHT,
        NONE,
    };

    Operator() : ErrorAt(), _prec(255), _assoc(LEFT), _nargs(0), _op(nullptr), _fn(nullptr) {}

    void operator=(const Operator &o) {
        setError(o);
        _prec = o._prec;
        _assoc = o._assoc;
        _nargs = o._nargs;
        _op = o._op;
        _fn = o._fn;
    }

    /** Pop operands in |stack|, evaluate and push  result onto |stack|. */
    Error eval(ValueStack &stack, uint8_t argc = 0) const;
    /** Returns true when |this| operator has higher precedence than |o| */
    bool isHigher(const Operator &o) const;
    /** Returns true when |this| operator is none associative and has the same precedence of
        |o|. */
    bool isNoneAssoc(const Operator &o) const;

    /** Constructor for operators */
    typedef Error(OperatorEval)(ValueStack &stack);
    Operator(uint8_t prec, Assoc assoc, int8_t nargs = 0, OperatorEval *op = nullptr)
        : ErrorAt(), _prec(prec), _assoc(assoc), _nargs(nargs), _op(op), _fn(nullptr) {}

    /** Constructor for function */
    Operator(const Functor *fn)
        : ErrorAt(), _prec(2), _assoc(LEFT), _nargs(0), _op(nullptr), _fn(fn) {}
    bool isFunction() const { return _fn != nullptr; }

    /**
     * Constructor for opening parenthesis which records the stack position of possible function
     * argument list.
     */
    Operator(uint8_t stackPosition)
        : ErrorAt(), _prec(254), _assoc(LEFT), _nargs(stackPosition), _op(nullptr), _fn(nullptr) {}
    bool isOpenParen() const { return _prec == 254; }
    uint8_t stackPosition() const { return _nargs; }

private:
    /** Operator prec (smaller value means higher precedence). */
    uint8_t _prec;
    Assoc _assoc;
    int8_t _nargs;
    OperatorEval *_op;
    const Functor *_fn;
};

/**
 * Parsing prefix and infix operator.
 */
struct OperatorParser {
    virtual const Operator *readOperator(
            StrScanner &scan, ErrorAt &error, Operator::Type type) const = 0;
};

struct CStyleOperatorParser : OperatorParser {
    const Operator *readOperator(
            StrScanner &scan, ErrorAt &error, Operator::Type type) const override;
};

struct Mc68xxOperatorParser : CStyleOperatorParser {
    const Operator *readOperator(
            StrScanner &scan, ErrorAt &error, Operator::Type type) const override;
};

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

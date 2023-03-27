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

struct Operator {
    virtual Error eval(Value &val, const Value &rhs) const { return OK; }
    virtual Error eval(Value &val, const Value &lhs, const Value &rhs) const { return OK; }
    bool hasHigherPriority(const Operator &o) const { return _precedence <= o._precedence; }

    static const struct Operator OP_NONE;

protected:
    Operator(uint8_t precedence) : _precedence(precedence) {}

private:
    // Operator precedence (smaller value means higher precedence).
    const uint8_t _precedence;
};

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

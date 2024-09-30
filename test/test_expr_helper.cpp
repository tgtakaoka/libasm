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

#include "test_expr_helper.h"

#include <stdio.h>
#include "str_buffer.h"

namespace libasm {
namespace test {

TestAsserter asserter;
TestSymtab symtab;
ParserContext context{0, &symtab};

void val_assert(const char *file, int line, StrScanner &expr, char delim, uint32_t expected,
        const ErrorAt &expected_error, size_t size, const ValueParser &parser) {
    ErrorAt actual_error;
    StrScanner remain = expr;
    context.delimitor = delim;
    auto val = parser.eval(remain, actual_error, context);
    auto actual = val.getUnsigned();
    if (size == sizeof(uint8_t)) {
        if (val.overflowUint8())
            actual_error.setErrorIf(expr, OVERFLOW_RANGE);
        actual = static_cast<uint8_t>(val.getUnsigned());
        expected = static_cast<uint8_t>(expected);
    }
    if (size == sizeof(uint16_t)) {
        if (val.overflowUint16())
            actual_error.setErrorIf(expr, OVERFLOW_RANGE);
        actual = static_cast<uint16_t>(val.getUnsigned());
        expected = static_cast<uint16_t>(expected);
    }
    if (size == sizeof(uint32_t)) {
        if (val.overflowUint32())
            actual_error.setErrorIf(expr, OVERFLOW_RANGE);
    }
    asserter.equals(file, line, expr.str(), expected_error.getError(), actual_error);
    if (expected_error.isOK()) {
        asserter.equals(file, line, expr.str(), expected, actual);
    } else {
        asserter.equals(file, line, expr.str(), expected_error.errorAt(), actual_error.errorAt());
    }
    expr = remain;
}

void dec_assert(const char *file, int line, uint32_t value, int8_t bitWidth, const char *expected,
        const ValueFormatter &formatter, bool uppercase) {
    char msg[80];
    snprintf(msg, sizeof(msg), "%d", value);
    char actual[80];
    StrCaseBuffer buf(actual, sizeof(actual), uppercase);
    formatter.formatDec(buf, value, bitWidth);
    asserter.equals(file, line, msg, expected, actual);
}

void bin_assert(const char *file, int line, uint32_t value, int8_t bitWidth, const char *expected,
        const ValueFormatter &formatter, bool uppercase) {
    char msg[80];
    snprintf(msg, sizeof(msg), "%d", value);
    char actual[80];
    StrCaseBuffer buf(actual, sizeof(actual), uppercase);
    formatter.formatBin(buf, value, bitWidth);
    asserter.equals(file, line, msg, expected, actual);
}

void oct_assert(const char *file, int line, uint32_t value, int8_t bitWidth, const char *expected,
        const ValueFormatter &formatter, bool uppercase) {
    char msg[80];
    snprintf(msg, sizeof(msg), "%d", value);
    char actual[80];
    StrCaseBuffer buf(actual, sizeof(actual), uppercase);
    formatter.formatOct(buf, value, bitWidth);
    asserter.equals(file, line, msg, expected, actual);
}

void hex_assert(const char *file, int line, uint32_t value, int8_t bitWidth, const char *expected,
        const ValueFormatter &formatter, bool uppercase, bool relax) {
    char msg[80];
    snprintf(msg, sizeof(msg), "%#x", value);
    char actual[80];
    StrCaseBuffer buf(actual, sizeof(actual), uppercase);
    auto abs = value;
    ValueFormatter::absolute(abs, bitWidth);
    if (relax && abs <= 32) {
        formatter.formatDec(buf, value, bitWidth);
    } else {
        formatter.formatHex(buf, value, bitWidth);
    }
    asserter.equals(file, line, msg, expected, actual);
}

bool test_failed;

void run_test(void (*test)(), const char *name, void (*set_up)(), void (*tear_down)()) {
    asserter.clear(name);
    set_up();
    test();
    tear_down();
    if (!asserter.check())
        test_failed = true;
}

}  // namespace test
}  // namespace libasm

using namespace libasm::test;

int main(int argc, char **argv) {
    test_failed = false;
    run_tests();
    return test_failed ? 2 : 0;
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

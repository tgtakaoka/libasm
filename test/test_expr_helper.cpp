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

#include "str_buffer.h"

extern libasm::ValueParser parser;
extern libasm::ValueFormatter formatter;

namespace libasm {
namespace test {

TestAsserter asserter;
TestSymtab symtab;

void val_assert(const char *file, const int line, const char *expr, uint32_t expected,
        const Error expected_error, size_t size) {
    Value val;
    ErrorReporter error;
    StrScanner scan(expr);
    val = parser.eval(scan, &symtab);
    uint32_t actual = val.getUnsigned();
    error.setError(parser);
    if (size == 1) {
        if (val.overflowUint8() && error.isOK())
            error.setError(OVERFLOW_RANGE);
        actual = static_cast<uint8_t>(actual);
        expected = static_cast<uint8_t>(expected);
    }
    if (size == 2) {
        if (val.overflowUint16() && error.isOK())
            error.setError(OVERFLOW_RANGE);
        actual = static_cast<uint16_t>(actual);
        expected = static_cast<uint16_t>(expected);
    }
    if (val.isUndefined() && error.isOK())
        error.setError(UNDEFINED_SYMBOL);
    asserter.equals(file, line, expr, expected_error, error);
    if (error.isOK())
        asserter.equals(file, line, expr, expected, actual);
}

void dec_assert(const char *file, const int line, const uint32_t value, int8_t bitWidth,
        const char *expected) {
    char msg[80];
    sprintf(msg, "%d", value);
    char actual[80];
    StrBuffer buf(actual, sizeof(actual));
    formatter.formatDec(buf, value, bitWidth);
    asserter.equals(file, line, msg, expected, actual);
}

void hex_assert(const char *file, const int line, const uint32_t value, int8_t bitWidth,
        const bool relax, const char *expected) {
    char msg[80];
    sprintf(msg, "%#x", value);
    char actual[80];
    StrBuffer buf(actual, sizeof(actual));
    formatter.formatHex(buf, value, bitWidth, relax);
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

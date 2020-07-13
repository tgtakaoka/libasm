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

#ifndef __TEST_EXPT_HELPER_H__
#define __TEST_EXPT_HELPER_H__

#include "test_symtab.h"
#include "test_asserter.h"
#include "value_formatter.h"
#include "value_parser.h"

namespace libasm {
namespace test {

extern TestSymtab symtab;
extern TestAsserter asserter;

template<typename T>
void val_assert(
    const char *file, const int line, const char *expr,
    const T expected, const Error expected_error,
    ValueParser &parser) {
    char msg[80];
    sprintf(msg, "%s:%d: %s", file, line, expr);
    T actual;
    parser.eval(expr, actual, &symtab);
    asserter.equals(msg, expected_error, parser);
    if (parser.getError() == OK)
        asserter.equals(msg,
                        static_cast<uint32_t>(expected),
                        static_cast<uint32_t>(actual));
}
#define E8(expr, expected, expected_error)                              \
    val_assert<uint8_t>(__FILE__, __LINE__, expr, expected, expected_error, parser)
#define E16(expr, expected, expected_error)                             \
    val_assert<uint16_t>(__FILE__, __LINE__, expr, expected, expected_error, parser)
#define E32(expr, expected, expected_error)                             \
    val_assert<uint32_t>(__FILE__, __LINE__, expr, expected, expected_error, parser)

template<int BIT_WIDTH>
void fmt_assert(
    const char *file, const int line, const uint32_t value,
    const int8_t radix, const bool relax, const char *expected,
    ValueFormatter &formatter) {
    char msg[80];
    sprintf(msg, "%s:%d: %d (%x)", file, line, value, value);
    char actual[80];
    formatter.output(actual, value, radix, relax, BIT_WIDTH);
    asserter.equals(msg, expected, actual);
}

#define FN(n, value, radix, relax, expected)                            \
    fmt_assert<n>(__FILE__, __LINE__, value, radix, relax, expected, formatter)
#define F8(value, radix, relax, expected)                               \
    fmt_assert<8>(__FILE__, __LINE__, value, radix, relax, expected, formatter)
#define F16(value, radix, relax, expected)                              \
    fmt_assert<16>(__FILE__, __LINE__, value, radix, relax, expected, formatter)
#define F24(value, radix, relax, expected)                              \
    fmt_assert<24>(__FILE__, __LINE__, value, radix, relax, expected, formatter)
#define F32(value, radix, relax, expected)                              \
    fmt_assert<32>(__FILE__, __LINE__, value, radix, relax, expected, formatter)

#define RUN_TEST(test) run_test(test, #test)

} // namespace test
} // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

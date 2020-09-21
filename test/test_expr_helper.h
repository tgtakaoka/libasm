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

void run_tests();

namespace libasm {
namespace test {

extern TestSymtab symtab;
extern TestAsserter asserter;

template<typename T>
void val_assert(
    const char *file, const int line, const char *expr,
    const T expected, const Error expected_error,
    ValueParser &parser) {
    T actual;
    parser.eval(expr, actual, &symtab);
    asserter.equals(file, line, expr, expected_error, parser);
    if (parser.getError() == OK)
        asserter.equals(file, line, expr,
                        static_cast<uint32_t>(expected),
                        static_cast<uint32_t>(actual));
}

void dec_assert(
    const char *file, const int line, const uint32_t value, int8_t bitWidth,
    const char *expected, ValueFormatter &formatter);

void hex_assert(
    const char *file, const int line, const uint32_t value, int8_t bitWidth,
    const bool relax, const char *expected, ValueFormatter &formatter);

void run_test(
        void (*test)(),
        const char *name,
        void (*set_up)(),
        void (*tear_down)());

} // namespace test
} // namespace libasm

#define E8(expr, expected, expected_error)                              \
    val_assert<uint8_t>(__FILE__, __LINE__, expr, expected, expected_error, parser)
#define E16(expr, expected, expected_error)                             \
    val_assert<uint16_t>(__FILE__, __LINE__, expr, expected, expected_error, parser)
#define E32(expr, expected, expected_error)                             \
    val_assert<uint32_t>(__FILE__, __LINE__, expr, expected, expected_error, parser)

#define DEC(value, bits, expected)                               \
    dec_assert(__FILE__, __LINE__, value, bits, expected, formatter)
#define HEX(value, bits, relax, expected)                               \
    hex_assert(__FILE__, __LINE__, value, bits, relax, expected, formatter)

#define RUN_TEST(test) run_test(test, #test, set_up, tear_down)


#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

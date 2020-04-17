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

#include "asm_operand.h"
#include "test_symtab.h"
#include "test_asserter.h"

extern libasm::test::TestSymtab symtab;
extern libasm::test::TestAsserter asserter;

#define __ASSERT(file, line, T, expr, value, expected_error)        \
    do {                                                            \
        char msg[80];                                               \
        sprintf(msg, "%s:%d: %s", file, line, expr);                \
        const T expected = value;                                   \
        T actual;                                                   \
        parser.eval(expr, actual, &symtab);                         \
        asserter.equals(msg, expected_error, parser.getError());    \
        if (parser.getError() == OK)                                \
            asserter.equals(msg,                                    \
                            static_cast<uint32_t>(expected),        \
                            static_cast<uint32_t>(actual));         \
    } while (false);
#define E8(expr, expected, expected_error)                              \
    __ASSERT(__FILE__, __LINE__, uint8_t, expr, expected, expected_error)
#define E16(expr, expected, expected_error)                             \
    __ASSERT(__FILE__, __LINE__, uint16_t, expr, expected, expected_error)
#define E32(expr, expected, expected_error)                             \
    __ASSERT(__FILE__, __LINE__, uint32_t, expr, expected, expected_error)

#define RUN_TEST(test) run_test(test, #test)

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

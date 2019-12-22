/* -*- mode: c++; -*- */
#ifndef __TEST_EXPT_HELPER_H__
#define __TEST_EXPT_HELPER_H__

#include "asm_operand.h"
#include "test_symtab.h"
#include "test_asserter.h"

extern TestSymtab symtab;
extern TestAsserter asserter;

#define E8(expr, expected, expected_error) do {                     \
        uint8_t val;                                                \
        parser.eval(expr, val, &symtab);                            \
        asserter.equals(expr, expected_error, parser.getError());   \
        if (parser.getError() == OK)                                \
            asserter.equals(expr, expected, val);                   \
    } while (false);
#define E16(expr, expected, expected_error) do {                    \
        uint16_t val;                                               \
        parser.eval(expr, val, &symtab);                            \
        asserter.equals(expr, expected_error, parser.getError());   \
        if (parser.getError() == OK)                                \
            asserter.equals(expr, expected, val);                   \
    } while (false);

#define E32(expr, expected, expected_error) do {                    \
        uint32_t val;                                               \
        parser.eval(expr, val, &symtab);                            \
        asserter.equals(expr, expected_error, parser.getError());   \
        if (parser.getError() == OK)                                \
            asserter.equals(expr, expected, val);                   \
    } while (false);

#define RUN_TEST(test) run_test(test, #test)

#endif

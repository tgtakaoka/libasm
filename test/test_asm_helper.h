/* -*- mode: c++; -*- */
#ifndef __TEST_ASM_HELPER_H__
#define __TEST_ASM_HELPER_H__

#include "test_asserter.h"
#include "test_symtab.h"

#include <stdio.h>

extern TestAsserter asserter;
extern TestSymtab symtab;

#define ASSERT(addr, line, expected)                            \
    do {                                                        \
        Insn insn;                                              \
        char message[80];                                       \
        symtab.setCurrentOrigin(addr);                          \
        assembler.encode(line, insn, addr, &symtab);            \
        sprintf(message, "%s: %s", __FUNCTION__, line);         \
        asserter.equals(message, OK, assembler.getError());     \
        asserter.equals(message, expected, sizeof(expected),    \
                        insn.bytes(), insn.insnLen());          \
    } while (0)
#define ATEST(addr, line, ...)                                  \
    do {                                                        \
        const Insn::insn_unit_t expected[] = { __VA_ARGS__ };   \
        ASSERT(addr, line, expected);                           \
    } while (0)
#define TEST(line, ...) ATEST(0x0000, line, __VA_ARGS__)

#define RUN_TEST(test) run_test(test, #test)

#endif

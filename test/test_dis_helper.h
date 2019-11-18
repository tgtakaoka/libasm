/* -*- mode: c++; -*- */
#ifndef __TEST_DIS_HELPER_H__
#define __TEST_DIS_HELPER_H__

#include "test_asserter.h"
#include "test_memory.h"
#include "test_symtab.h"
#include "symbol_table.h"

#include <cstdio>

extern TestAsserter asserter;
extern TestMemory memory;
extern TestSymtab symtab;

#define ASSERT(addr, mnemonic, expected_operands, expected_comments, ...) \
    do {                                                                \
        const uint8_t mnemonic[] = { __VA_ARGS__ };                     \
        memory.setAddress(addr);                                        \
        memory.setBytes(mnemonic, sizeof(mnemonic));                    \
        Insn insn;                                                      \
        char operands[40], comments[40], message[40];                   \
        disassembler.decode(memory, insn, operands, comments, &symtab); \
        sprintf(message, "%s: %s: ", __FUNCTION__, #mnemonic);          \
        memory.dump(message + strlen(message));                         \
        asserter.equals(message, OK, disassembler.getError());          \
        asserter.equals(message, #mnemonic, insn.name());               \
        asserter.equals(message, expected_operands, operands);          \
        asserter.equals(message, expected_comments, comments);          \
        asserter.equals(message,                                        \
                        mnemonic, sizeof(mnemonic),                     \
                        insn.bytes(), insn.insnLen());                  \
    } while (0)
#define ATEST(addr, mnemonic, opr, com, ...)        \
    ASSERT(addr, mnemonic, opr, com, __VA_ARGS__)
#define TEST(mnemonic, opr, com, ...)               \
    ASSERT(0x0000, mnemonic, opr, com, __VA_ARGS__)

#define RUN_TEST(test) run_test(test, #test)

#endif

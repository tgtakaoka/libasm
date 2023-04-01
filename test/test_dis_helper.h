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

#ifndef __TEST_DIS_HELPER_H__
#define __TEST_DIS_HELPER_H__

#include "array_memory.h"
#include "dis_base.h"
#include "test_asserter.h"
#include "test_symtab.h"

void test_cpu();
void run_tests(const char *cpu);

namespace libasm {
namespace test {

extern TestAsserter asserter;
extern TestSymtab symtab;

void dis_assert(const char *file, int line, Error error, const ArrayMemory &src,
        const char *expected_name, const char *expected_opr);

void run_test(void (*test)(), const char *name, void (*set_up)(), void (*tear_down)());

}  // namespace test
}  // namespace libasm

#define EQUALS(msg, expected, actual) asserter.equals(__FILE__, __LINE__, msg, expected, actual)
#define EQUALS_P(msg, expected, /* PROGMEM */ actual_P) \
    asserter.equals_P(__FILE__, __LINE__, msg, expected, actual_P)
#define NOT_EQUALS(msg, expected, actual) \
    asserter.not_equals(__FILE__, __LINE__, msg, expected, actual)
#define __VASSERT(file, line, error, addr, name, opr, ...)                \
    do {                                                                  \
        const auto unit = disassembler.config().addressUnit();            \
        const auto endian = disassembler.config().endian();               \
        const Config::opcode_t name[] = {__VA_ARGS__};                    \
        const ArrayMemory memory(addr *unit, name, sizeof(name), endian); \
        dis_assert(file, line, error, memory, #name, opr);                \
    } while (0)
#define VASSERT(error, addr, name, opr, ...) \
    __VASSERT(__FILE__, __LINE__, error, addr, name, opr, __VA_ARGS__)
#define AERRT(addr, name, opr, err, ...) VASSERT(err, addr, name, opr, __VA_ARGS__)
#define ATEST(addr, name, opr, ...) AERRT(addr, name, opr, OK, __VA_ARGS__)
#define ERRT(name, opr, err, ...) AERRT(0, name, opr, err, __VA_ARGS__)
#define TEST(name, opr, ...) ERRT(name, opr, OK, __VA_ARGS__)
#define ERRP(...) ERRT(_, "", UNKNOWN_POSTBYTE, __VA_ARGS__)
#define ERRI(...) ERRT(_, "", UNKNOWN_INSTRUCTION, __VA_ARGS__)
#define ERBN(name, opr, ...) ERRT(name, opr, ILLEGAL_BIT_NUMBER, __VA_ARGS__)
#define ERIC(name, opr, ...) ERRT(name, opr, ILLEGAL_CONSTANT, __VA_ARGS__)
#define ERIO(name, opr, ...) ERRT(name, opr, ILLEGAL_OPERAND, __VA_ARGS__)
#define ERIR(name, opr, ...) ERRT(name, opr, ILLEGAL_REGISTER, __VA_ARGS__)
#define EROA(name, opr, ...) ERRT(name, opr, OPERAND_NOT_ALLOWED, __VA_ARGS__)
#define ERRV(name, opr, ...) ERRT(name, opr, REGISTERS_OVERLAPPED, __VA_ARGS__)
#define ERRR(name, opr, ...) ERRT(name, opr, REGISTER_NOT_ALLOWED, __VA_ARGS__)
#define ERUI(name, opr, ...) ERRT(name, opr, UNKNOWN_INSTRUCTION, __VA_ARGS__)
#define ERUR(name, opr, ...) ERRT(name, opr, UNKNOWN_REGISTER, __VA_ARGS__)
#define ERNE(name, opr, ...) ERRT(name, opr, OPCODE_HAS_NO_EFFECT, __VA_ARGS__)
#define ERSZ(name, opr, ...) ERRT(name, opr, ILLEGAL_SIZE, __VA_ARGS__)

#define RUN_TEST(test) run_test(test, #test, set_up, tear_down)

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

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

#include "dis_base.h"
#include "test_asserter.h"
#include "test_memory.h"
#include "test_symtab.h"

const char *run_cpu_test();
void run_tests(const char *cpu);

namespace libasm {
namespace test {

extern TestAsserter asserter;
extern TestMemory memory;
extern TestSymtab symtab;

void dis_assert(const char *file, int line, Error error,
        const char *expected_name, const char *expected_opr,
        Disassembler &disassembler);

void run_test(void (*test)(), const char *name, void (*set_up)(),
        void (*tear_down)());

}  // namespace test
}  // namespace libasm

#define EQUALS(msg, expected, actual) \
    asserter.equals(__FILE__, __LINE__, msg, expected, actual)
#define NOT_EQUALS(msg, expected, actual) \
    asserter.not_equals(__FILE__, __LINE__, msg, expected, actual)
#define __VASSERT(file, line, error, addr, name, opr, ...)       \
    do {                                                         \
        const Config::opcode_t name[] = {__VA_ARGS__};           \
        memory.setMemory(name, sizeof(name));                    \
        memory.setAddress(addr);                                 \
        dis_assert(file, line, error, #name, opr, disassembler); \
    } while (0)
#define EATEST(error, addr, name, opr, ...) \
    __VASSERT(__FILE__, __LINE__, error, addr, name, opr, __VA_ARGS__)
#define ATEST(addr, name, opr, ...) \
    __VASSERT(__FILE__, __LINE__, OK, addr, name, opr, __VA_ARGS__)
#define ETEST(error, name, opr, ...) \
    __VASSERT(__FILE__, __LINE__, error, 0x0000, name, opr, __VA_ARGS__)
#define TEST(name, opr, ...) ETEST(OK, name, opr, __VA_ARGS__)

#define ILLEGAL(...) ETEST(UNKNOWN_INSTRUCTION, _, "", __VA_ARGS__)

#define RUN_TEST(test) run_test(test, #test, set_up, tear_down)

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

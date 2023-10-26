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

#ifndef __TEST_ASM_HELPER_H__
#define __TEST_ASM_HELPER_H__

#include "array_memory.h"
#include "asm_base.h"
#include "test_asserter.h"
#include "test_symtab.h"

void test_cpu();
void run_tests(const char *cpu);

namespace libasm {
namespace test {

extern TestAsserter asserter;
extern TestSymtab symtab;

void asm_assert(const char *file, int line, const ErrorAt &error, const char *src,
        const ArrayMemory &expected);

void run_test(void (*test)(), const char *name, void (*set_up)(), void (*tear_down)());

}  // namespace test
}  // namespace libasm

#define EQUALS(msg, expected, actual) asserter.equals(__FILE__, __LINE__, msg, expected, actual)
#define EQUALS_P(msg, expected, /* PROGMEM */ actual_P) \
    asserter.equals_P(__FILE__, __LINE__, msg, expected, actual_P)
#define __VASSERT(file, line, err, at, addr, src, ...)                            \
    do {                                                                          \
        const auto unit = assembler.config().addressUnit();                       \
        const auto endian = assembler.config().endian();                          \
        const Config::opcode_t expected[] = {__VA_ARGS__};                        \
        const ArrayMemory memory(addr *unit, expected, sizeof(expected), endian); \
        ErrorAt error;                                                            \
        error.setError(at, err);                                                  \
        asm_assert(file, line, error, src, memory);                               \
    } while (0)
#define __BVASSERT(file, line, err, at, addr, src, ...)                           \
    do {                                                                          \
        const auto unit = assembler.config().addressUnit();                       \
        const auto endian = assembler.config().endian();                          \
        const uint8_t expected[] = {__VA_ARGS__};                                 \
        const ArrayMemory memory(addr *unit, expected, sizeof(expected), endian); \
        ErrorAt error;                                                            \
        error.setError(at, err);                                                  \
        asm_assert(file, line, error, src, memory);                               \
    } while (0)
#define VASSERT(error, at, addr, src, ...) \
    __VASSERT(__FILE__, __LINE__, error, at, addr, src, ##__VA_ARGS__)
#define BVASSERT(error, at, addr, src, ...) \
    __BVASSERT(__FILE__, __LINE__, error, at, addr, src, ##__VA_ARGS__)
#define AERRT(addr, src, error, at, ...) VASSERT(error, at, addr, src, ##__VA_ARGS__)
#define AERUS(addr, src, at, ...) AERRT(addr, src, UNDEFINED_SYMBOL, at, ##__VA_ARGS__)
#define ERRT(src, error, at, ...) AERRT(0x0000, src, error, at, ##__VA_ARGS__)
#define ERUS(src, at, ...) ERRT(src, UNDEFINED_SYMBOL, at, __VA_ARGS__)
#define ERUI(src, ...) ERRT(src, UNKNOWN_INSTRUCTION, src, ##__VA_ARGS__);
#define ATEST(addr, src, ...) AERRT(addr, src, OK, "", __VA_ARGS__)
#define TEST(src, ...) ATEST(0x0000, src, __VA_ARGS__)
#define BERRT(src, error, at, ...) BVASSERT(error, at, 0x0000, src, ##__VA_ARGS__)
#define BTEST(src, ...) BERRT(src, OK, "", __VA_ARGS__)

#define RUN_TEST(test) run_test(test, #test, set_up, tear_down)

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

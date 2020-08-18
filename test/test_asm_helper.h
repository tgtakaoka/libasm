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

#include "asm_base.h"
#include "test_asserter.h"
#include "test_symtab.h"

namespace libasm {
namespace test {

extern TestAsserter asserter;
extern TestSymtab symtab;

void asm_assert(
    const char *file, int line, Error error,
    uint32_t addr, const char *src,
    const uint8_t *expected, uint8_t length,
    Assembler &assembler);

void asm_assert(
    const char *file, int line, Error error,
    uint32_t addr, const char *src,
    const uint16_t *expected, uint8_t length,
    Assembler &assembler);

} // namespace test
} // namespace libasm

#define EQUALS(msg, expected, actual) \
    asserter.equals(__FILE__, __LINE__, msg, expected, actual)
#define __VASSERT(file, line, error, addr, src, ...)            \
    do {                                                        \
        const Config::opcode_t expected[] = { __VA_ARGS__ };    \
        const uint8_t length = sizeof(expected);                \
        asm_assert(file, line, error, addr, src,                \
                   expected, length, assembler);                \
    } while (0)
#define EATEST(error, addr, src, ...)                               \
    __VASSERT(__FILE__, __LINE__, error, addr, src, __VA_ARGS__)
#define ATEST(addr, src, ...)                                   \
    __VASSERT(__FILE__, __LINE__, OK, addr, src, __VA_ARGS__)
#define ETEST(error, src, ...)                                      \
    __VASSERT(__FILE__, __LINE__, error, 0x0000, src, __VA_ARGS__)
#define TEST(src, ...) ETEST(OK, src, __VA_ARGS__)

#define RUN_TEST(test) run_test(test, #test)

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

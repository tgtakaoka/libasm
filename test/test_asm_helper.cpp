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

#include "test_asm_helper.h"

#include <stdio.h>

namespace libasm {
namespace test {

TestAsserter asserter;
TestSymtab symtab;

void asm_assert(const char *file, int line, Error error, uint32_t addr,
        const char *src, const uint8_t *expected, uint8_t length,
        Assembler &assembler) {
    Insn insn;
    symtab.setCurrentOrigin(addr);
    assembler.encode(src, insn, addr, &symtab);
    asserter.equals(file, line, src, error, assembler);
    asserter.equals(
            file, line, src, expected, length, insn.bytes(), insn.length());
}

void asm_assert(const char *file, int line, Error error, uint32_t addr,
        const char *src, const uint16_t *expected, uint8_t length,
        Assembler &assembler) {
    Insn insn;
    symtab.setCurrentOrigin(addr);
    assembler.encode(src, insn, addr, &symtab);
    asserter.equals(file, line, src, error, assembler);
    asserter.equals(
            file, line, src, expected, length, insn.bytes(), insn.length());
}

bool test_failed;

void run_test(void (*test)(), const char *name, void (*set_up)(),
        void (*tear_down)()) {
    asserter.clear(name);
    set_up();
    test();
    tear_down();
    if (!asserter.check())
        test_failed = true;
}

}  // namespace test
}  // namespace libasm

using namespace libasm::test;

int main(int argc, char **argv) {
    test_failed = false;
    run_tests();
    return test_failed ? 2 : 0;
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

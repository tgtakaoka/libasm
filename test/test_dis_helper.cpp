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

#include "test_dis_helper.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <vector>

#include "test_memory.h"

namespace libasm {
namespace test {

TestAsserter asserter;
TestSymtab symtab;

static TestMemory memory;
static char actual_opr[128];
static char message[256];

void dis_assert(const char *file, int line, Error error, uint32_t addr, const uint8_t *src,
        uint8_t src_size, const char *expected_name, const char *expected_opr,
        Disassembler &disassembler) {
    memory.setAddress(addr);
    memory.setMemory(src, src_size);

    Insn insn;
    disassembler.setUppercase(true);
    disassembler.decode(memory, insn, actual_opr, sizeof(actual_opr), &symtab);

    strcpy(message, expected_name);
    memory.dump(message + strlen(message));
    asserter.equals(file, line, expected_name, error, disassembler);
    if (error == OK) {
        asserter.equals(file, line, expected_name, expected_name, insn.name());
        asserter.equals(file, line, expected_name, expected_opr, actual_opr);
        asserter.equals(file, line, expected_name, memory.bytes(), memory.length(), insn.bytes(),
                insn.length());
    }
}

void dis_assert(const char *file, int line, Error error, uint32_t addr, const uint16_t *src,
        uint8_t src_size, const char *expected_name, const char *expected_opr,
        Disassembler &disassembler) {
    memory.setAddress(addr);
    memory.setMemory(src, src_size);

    Insn insn;
    disassembler.setUppercase(true);
    disassembler.decode(memory, insn, actual_opr, sizeof(actual_opr), &symtab);

    strcpy(message, expected_name);
    memory.dump(message + strlen(message));
    asserter.equals(file, line, expected_name, error, disassembler);
    if (error == OK) {
        asserter.equals(file, line, expected_name, expected_name, insn.name());
        asserter.equals(file, line, expected_name, expected_opr, actual_opr);
        asserter.equals(file, line, expected_name, memory.bytes(), memory.length(), insn.bytes(),
                insn.length());
    }
}

bool test_failed;

void run_test(void (*test)(), const char *name, void (*set_up)(), void (*tear_down)()) {
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

std::vector<std::string> cpu_list(const char *cpu_list) {
    std::vector<std::string> list;
    const char *s = cpu_list;
    while (true) {
        const char *e = strchr(s, ',');
        if (e == nullptr) {
            list.emplace_back(s);
            return list;
        }
        list.emplace_back(s, e - s);
        for (s = e + 1; *s == ' '; s++)
            ;
    }
}

int main(int argc, char **argv) {
    test_failed = false;
    for (const auto &cpu : cpu_list(run_cpu_test())) {
        printf("  TEST %s disassembler\n", cpu.c_str());
        run_tests(cpu.c_str());
    }
    return test_failed ? 2 : 0;
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

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

#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

extern libasm::Disassembler &disassembler;

namespace libasm {
namespace test {

TestAsserter asserter;
TestSymtab symtab;

char actual_opr[128];

void dis_assert(const char *file, int line, const ErrorAt &error, const ArrayMemory &memory,
        const char *expected_name, const char *expected_opr, Insn &insn) {
    auto mem = memory.iterator();
    disassembler.decode(mem, insn, actual_opr, sizeof(actual_opr), &symtab);

    asserter.equals(file, line, expected_name, error.errorText_P(), insn.errorText_P());
    asserter.equals(file, line, "error at", error.errorAt(), insn.errorAt());
    asserter.equals(file, line, expected_name, expected_name, insn);
    asserter.equals(file, line, expected_name, expected_opr, actual_opr);
    asserter.equals(file, line, expected_name, memory, insn.bytes(), insn.length(),
            disassembler.listRadix());
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

std::vector<std::string> cpu_list() {
    const /* PROGMEM */ char *list_P = disassembler.config().listCpu_P();
    char buffer[strlen_P(list_P) + 1];
    strcpy_P(buffer, list_P);
    std::vector<std::string> list;
    const char *s = buffer;
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
    test_cpu();
    for (const auto &cpu : cpu_list()) {
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

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

#include "test_expr_helper.h"

namespace libasm {
namespace test {

TestAsserter asserter;
TestSymtab symtab;

void dec_assert(const char *file, const int line, const uint32_t value,
        int8_t bitWidth, const char *expected, ValueFormatter &formatter) {
    char msg[80];
    sprintf(msg, "%d", value);
    char actual[80];
    formatter.formatDec(actual, value, bitWidth);
    asserter.equals(file, line, msg, expected, actual);
}

void hex_assert(const char *file, const int line, const uint32_t value,
        int8_t bitWidth, const bool relax, const char *expected,
        ValueFormatter &formatter) {
    char msg[80];
    sprintf(msg, "%#x", value);
    char actual[80];
    formatter.formatHex(actual, value, bitWidth, relax);
    asserter.equals(file, line, msg, expected, actual);
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

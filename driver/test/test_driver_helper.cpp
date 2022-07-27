/*
 * Copyright 2022 Tadashi G. Takaoka
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

#include "test_driver_helper.h"

#include "test_asserter.h"

namespace libasm {
namespace driver {
namespace test {

libasm::test::TestAsserter asserter;

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
}  // namespace driver
}  // namespace libasm

using namespace libasm::driver::test;

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

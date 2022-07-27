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

#ifndef __TEST_DRIVER_HELPER_H__
#define __TEST_DRIVER_HELPER_H__

#include "test_asserter.h"

namespace libasm {
namespace driver {
namespace test {

void run_tests();

extern libasm::test::TestAsserter asserter;

void run_test(void (*test)(), const char *name, void (*set_up)(), void (*tear_down)());

}  // namespace test
}  // namespace driver
}  // namespace libasm

#define TRUE(msg, actual) asserter.isTrue(__FILE__, __LINE__, msg, actual)
#define FALSE(msg, actual) asserter.isFalse(__FILE__, __LINE__, msg, actual)
#define EQ(msg, expected, actual) asserter.equals(__FILE__, __LINE__, msg, expected, actual)

#define RUN_TEST(test) run_test(test, #test, set_up, tear_down)

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

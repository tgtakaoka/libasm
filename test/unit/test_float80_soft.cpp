/*
 * Copyright 2024 Tadashi G. Takaoka
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

#include "float80.h"
#include "ieee_float.h"
#include "test_unit_helper.h"

namespace libasm {
namespace test {
using f32 = float32_t;
using f64 = float64_t;
using f80 = __float80_soft;
}  // namespace test
}  // namespace libasm

#include "test_float80.inc"

namespace libasm {
namespace test {

void set_up() {}

void tear_down() {}

void run_tests() {
    RUN_TEST(test_decompose);
    RUN_TEST(test_compose);
    RUN_TEST(test_ctor);
    RUN_TEST(test_set);
    RUN_TEST(test_equals);
    RUN_TEST(test_lessthan);
    RUN_TEST(test_add);
    RUN_TEST(test_sub);
    RUN_TEST(test_mul);
    RUN_TEST(test_div);
    RUN_TEST(test_pow10);
    RUN_TEST(test_read);
    RUN_TEST(test_write);
    RUN_TEST(test_float32_set);
    RUN_TEST(test_float32_get);
    RUN_TEST(test_float64_set);
    RUN_TEST(test_float64_get);
}

}  // namespace test
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

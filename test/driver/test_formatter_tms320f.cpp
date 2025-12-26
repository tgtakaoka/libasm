/*
 * Copyright 2025 Tadashi G. Takaoka
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

#include "dis_tms320f.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_dis_tms320f() {
    PREP_DIS(tms320f::DisTms320f);

    driver.setUpperHex(false);

    DIS32("tms320c3x", 0x1000,
            R"(      cpu      320c3x
      org      001000h
      call     123456h
      addi3    *+ar1, *ar2++(ir0)b, r3
* test.bin: error: Unknown instruction
*     1002 : 788b0000
      negf     -0.50024, r0
)",
            R"(       0 :                            cpu      320c3x
    1000 :                            org      001000h
    1000 : 62123456                   call     123456h
    1001 : 2163ca01                   addi3    *+ar1, *ar2++(ir0)b, r3
test.bin: error: Unknown instruction
    1002 : 788b0000
    1003 : 0be0ffff                   negf     -0.50024, r0
)",
          0x62123456, 0x2163CA01, 0x788B0000, 0x0BE0FFFF);
}

void run_tests() {
    RUN_TEST(test_dis_tms320f);
}

}  // namespace test
}  // namespace driver
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

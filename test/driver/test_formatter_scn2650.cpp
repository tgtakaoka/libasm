/*
 * Copyright 2023 Tadashi G. Takaoka
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

#include "asm_scn2650.h"
#include "dis_scn2650.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_scn2650() {
    PREP_ASM(scn2650::AsmScn2650, IntelDirective);

    ASM("scn2650",
            R"(        cpu     scn2650
* comment line
        org     h'7bcd'        set location
label1: equ     h'7def'        define label
        loda,r0 *label1, r0, +;use label
)",
            R"(          0 :                            cpu     scn2650
          0 :                    * comment line
       7BCD :                            org     h'7bcd'        set location
       7BCD : =7DEF              label1: equ     h'7def'        define label
       7BCD : 0C BD EF                   loda,r0 *label1, r0, +;use label
)");
}

void test_dis_scn2650() {
    PREP_DIS(scn2650::DisScn2650);

    driver.setUpperHex(false);

    DIS8("scn2650", 0x7bcd,
            R"(      cpu     2650
      org     h'7bcd'
      loda,r0 *h'7def', r0, +
* test.bin: error: Unknown instruction
*     7bd0 : 90
)",
            R"(       0 :                            cpu     2650
    7bcd :                            org     h'7bcd'
    7bcd : 0c bd ef                   loda,r0 *h'7def', r0, +
test.bin: error: Unknown instruction
    7bd0 : 90
)",
            0x0C, 0xBD, 0xEF, 0x90);
}

void run_tests() {
    RUN_TEST(test_asm_scn2650);
    RUN_TEST(test_dis_scn2650);
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

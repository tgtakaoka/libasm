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

#include "asm_mn1610.h"
#include "dis_mn1610.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_mn1610() {
    PREP_ASM(mn1610::AsmMn1610, NationalDirective);

    ASM("mn1610",
            R"(        cpu   mn1610
* comment line
        org   x'abcd'
        tbit  r3, 5, nz
label:  dc    label
        ds    2
        dc    c'ABC',3
)",
            R"(          0 :                            cpu   mn1610
          0 :                    * comment line
       ABCD :                            org   x'abcd'
       ABCD : 2B55                       tbit  r3, 5, nz
       ABCE : ABCE               label:  dc    label
       ABCF :                            ds    2
       ABD1 : 4142 4300 0003             dc    c'ABC',3
)");
}

void test_asm_mn1613() {
    PREP_ASM(mn1610::AsmMn1610, NationalDirective);

    driver.setUpperHex(false);

    ASM("mn1613",
            R"(        cpu   mn1613
* comment line
        org   x'34567'
        mvwi  str, x'5678', skp
)",
            R"(          0 :                            cpu   mn1613
          0 :                    * comment line
      34567 :                            org   x'34567'
      34567 : 7e1f 5678                  mvwi  str, x'5678', skp
)");
}

void test_dis_mn1610() {
    PREP_DIS(mn1610::DisMn1610);

    driver.setUpperHex(false);

    DIS16("mn1610", 0xabcd,
            R"(      cpu   mn1610
      org   x'abcd'
      tbit  r3, 5, nz
* test.bin: error: Unknown instruction
*     abce : 0f06
)",
            R"(       0 :                            cpu   mn1610
    abcd :                            org   x'abcd'
    abcd : 2b55                       tbit  r3, 5, nz
test.bin: error: Unknown instruction
    abce : 0f06
)",
            0x2b55, 0x0F00 | (0 << 4) | 6);
}

void test_dis_mn1613() {
    PREP_DIS(mn1610::DisMn1610);

    driver.setUpperHex(false);

    DIS16("mn1613", 0x34567,
            R"(      cpu   mn1613
      org   x'34567'
      mvwi  str, x'5678', skp
* test.bin: error: Unknown instruction
*    34569 : 0f06
)",
            R"(       0 :                            cpu   mn1613
   34567 :                            org   x'34567'
   34567 : 7e1f 5678                  mvwi  str, x'5678', skp
test.bin: error: Unknown instruction
   34569 : 0f06
)",
            0x7e1f, 0x5678, 0x0f00 | (0 << 4) | 6);
}

void run_tests() {
    RUN_TEST(test_asm_mn1610);
    RUN_TEST(test_asm_mn1613);
    RUN_TEST(test_dis_mn1610);
    RUN_TEST(test_dis_mn1613);
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

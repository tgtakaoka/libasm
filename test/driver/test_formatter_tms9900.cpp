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

#include "asm_tms9900.h"
#include "dis_tms9900.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_tms99110() {
    PREP_ASM(tms9900::AsmTms9900, IntelDirective);

    driver.setUpperHex(false);

    ASM("tms99110",
            R"(        cpu   tms99110
* comment line
        aorg  >9abc          set location
        am    @>4a4b(r1), @>4c4d(r1)
        byte  1              2 is comment
        byte  2, 3           +1 will be added
label1: byte  4, 5, 6, 7     8 is comment
        data  >1234          define data
        byte  8
        even                 align
label2: ab    @label1, @label2
        jmp   label2;        loop
        mr    @f1
f1:     single 7.8125e-03
)",
            R"(          0 :                            cpu   tms99110
          0 :                    * comment line
       9abc :                            aorg  >9abc          set location
       9abc : 002a 4861 4a4b             am    @>4a4b(r1), @>4c4d(r1)
       9ac2 : 4c4d
       9ac4 : 01                         byte  1              2 is comment
       9ac5 :   02 04                    byte  2, 3           +1 will be added
       9ac7 :   04 0506 07       label1: byte  4, 5, 6, 7     8 is comment
       9acc : 1234                       data  >1234          define data
       9ace : 08                         byte  8
       9ad0 :                            even                 align
       9ad0 : b820 9ac7 9ad0     label2: ab    @label1, @label2
       9ad6 : 10fc                       jmp   label2;        loop
       9ad8 : 0d20 9adc                  mr    @f1
       9adc : 3f20 0000          f1:     single 7.8125e-03
)");
}

void test_dis_tms99110() {
    PREP_DIS(tms9900::DisTms9900);

    driver.setUpperHex(false);

    DIS16("tms99110", 0x9abc,
            R"(      cpu   99110
      org   >9abc
      am    @>4a4b(r1), @>4c4d(r1)
* test.bin: error: Unknown instruction
*     9ac4 : 0383
      ar    @>9ac8
)",
            R"(       0 :                            cpu   99110
    9abc :                            org   >9abc
    9abc : 002a 4861 4a4b             am    @>4a4b(r1), @>4c4d(r1)
    9ac2 : 4c4d
test.bin: error: Unknown instruction
    9ac4 : 0383
    9ac6 : 0c60 9ac8                  ar    @>9ac8
)",
          0x002a, 0x4861, 0x4a4b, 0x4c4d, 0x0383, 0x0c60, 0x9ac8);
}

void run_tests() {
    RUN_TEST(test_asm_tms99110);
    RUN_TEST(test_dis_tms99110);
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

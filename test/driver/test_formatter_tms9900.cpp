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

void test_asm_tms9900() {
    PREP_ASM(tms9900::AsmTms9900, IntelDirective);

    driver.setUpperHex(false);

    ASM("tms99105",
            R"(        cpu   tms99105
* comment line
        aorg  >9abc
        am    @>4a4b(r1), @>4c4d(r1)
        byte  1
        byte  2, 3
label1: byte  4, 5, 6, 7
        data  >1234
        byte  8
        even
label2: ab    @label1, @label2
)",
            R"(          0 :                            cpu   tms99105
          0 :                    * comment line
       9abc :                            aorg  >9abc
       9abc : 002a 4861 4a4b             am    @>4a4b(r1), @>4c4d(r1)
       9ac2 : 4c4d
       9ac4 : 01                         byte  1
       9ac5 :   02 03                    byte  2, 3
       9ac7 :   04 0506 07       label1: byte  4, 5, 6, 7
       9acc : 1234                       data  >1234
       9ace : 08                         byte  8
       9ad0 :                            even
       9ad0 : b820 9ac7 9ad0     label2: ab    @label1, @label2
)");
}

void test_dis_tms9900() {
    PREP_DIS(tms9900::DisTms9900);

    driver.setUpperHex(false);

    DIS16("tms99105", 0x9abc,
            R"(      cpu   99105
      org   >9abc
      am    @>4a4b(r1), @>4c4d(r1)
* test.bin: error: Unknown instruction
*     9ac4 : 0383
)",
            R"(       0 :                            cpu   99105
    9abc :                            org   >9abc
    9abc : 002a 4861 4a4b             am    @>4a4b(r1), @>4c4d(r1)
    9ac2 : 4c4d
test.bin: error: Unknown instruction
    9ac4 : 0383
)",
            0x002a, 0x4861, 0x4a4b, 0x4c4d, 0x0383);
}

void run_tests() {
    RUN_TEST(test_asm_tms9900);
    RUN_TEST(test_dis_tms9900);
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

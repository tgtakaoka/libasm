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
            "        cpu   tms99105\n"
            "* comment line\n"
            "        aorg  >9abc\n"
            "        am    @>4a4b(r1), @>4c4d(r1)\n"
            "        byte  1\n"
            "        byte  2, 3\n"
            "label1: byte  4, 5, 6, 7\n"
            "        data  >1234\n"
            "        byte  8\n"
            "        even\n"
            "label2: ab    @label1, @label2\n",
            "          0 :                            cpu   tms99105\n"
            "          0 :                    * comment line\n"
            "       9abc :                            aorg  >9abc\n"
            "       9abc : 002a 4861 4a4b             am    @>4a4b(r1), @>4c4d(r1)\n"
            "       9ac2 : 4c4d\n"
            "       9ac4 : 01                         byte  1\n"
            "       9ac5 :   02 03                    byte  2, 3\n"
            "       9ac7 :   04 0506 07       label1: byte  4, 5, 6, 7\n"
            "       9acc : 1234                       data  >1234\n"
            "       9ace : 08                         byte  8\n"
            "       9ad0 :                            even\n"
            "       9ad0 : b820 9ac7 9ad0     label2: ab    @label1, @label2\n");
}

void test_dis_tms9900() {
    PREP_DIS(tms9900::DisTms9900);

    driver.setUpperHex(false);

    DIS16("tms99105", 0x9abc,
            "      cpu   99105\n"
            "      org   >9abc\n"
            "      am    @>4a4b(r1), @>4c4d(r1)\n"
            "* test.bin: error: Unknown instruction\n"
            "*     9ac4 : 0383\n",
            "       0 :                            cpu   99105\n"
            "    9abc :                            org   >9abc\n"
            "    9abc : 002a 4861 4a4b             am    @>4a4b(r1), @>4c4d(r1)\n"
            "    9ac2 : 4c4d\n"
            "test.bin: error: Unknown instruction\n"
            "    9ac4 : 0383\n",
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

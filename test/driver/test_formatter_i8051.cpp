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

#include "asm_i8051.h"
#include "dis_i8051.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_i8051() {
    PREP_ASM(i8051::AsmI8051, IntelDirective);

    driver.setUpperHex(false);
    driver.setOption("smart-branch", "on");

    ASM("i8051",
            "        cpu   i8051\n"
            "; comment line\n"
            "        org   0abcdh\n"
            "data1:  equ   0b0h\n"
            "        anl   c, /data1.1\n"
            "        ljmp  label2\n"
            "        lcall label2\n"
            "        ajmp  label3\n"
            "        acall label3\n"
            "label2: equ   0ac00h\n"
            "label3: equ   0b000h\n",
            "          0 :                            cpu   i8051\n"
            "          0 :                    ; comment line\n"
            "       abcd :                            org   0abcdh\n"
            "       abcd : =b0                data1:  equ   0b0h\n"
            "       abcd : b0 b1                      anl   c, /data1.1\n"
            "       abcf : 81 00                      ljmp  label2\n"
            "       abd1 : 91 00                      lcall label2\n"
            "       abd3 : 02 b0 00                   ajmp  label3\n"
            "       abd6 : 12 b0 00                   acall label3\n"
            "       abd9 : =ac00              label2: equ   0ac00h\n"
            "       abd9 : =b000              label3: equ   0b000h\n");
}

void test_dis_i8051() {
    PREP_DIS(i8051::DisI8051);

    driver.setUpperHex(false);

    DIS8("i8051", 0xabcd,
            "      cpu   8051\n"
            "      org   0abcdh\n"
            "      anl   c, /0b0h.1\n"
            "; test.bin: error: Unknown instruction\n"
            ";     abcf : a5\n",
            "       0 :                            cpu   8051\n"
            "    abcd :                            org   0abcdh\n"
            "    abcd : b0 b1                      anl   c, /0b0h.1\n"
            "test.bin: error: Unknown instruction\n"
            "    abcf : a5\n",
            0xb0, 0xb1, 0xa5);
}

void run_tests() {
    RUN_TEST(test_asm_i8051);
    RUN_TEST(test_dis_i8051);
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

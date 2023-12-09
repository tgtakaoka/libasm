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

#include "asm_f3850.h"
#include "dis_f3850.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_f3850() {
    PREP_ASM(f3850::AsmF3850, FairchildDirective);

    TestReader inc("data/da.inc");
    sources.add(inc);
    inc.add("        rs    3\n"                         // RS allocates spaces
            "        da    H'1234', label1, H'9ABC'\n"  // DA generates words
            "        dc    'a,',',, '/, ' , 0\n"        // DC can omit closing quote
            "        dc    c'a',',, c'bc''de', 0\n"     // DC can accept string
            "        dc    C'A', #', C'C'+h'80'\n");    // C'c' requires surrounding quotes

    ASM("f3850",
            "        cpu   F3850\n"
            "* comment line\n"
            "        org   H'7BCD'\n"
            "label1  equ   H'7BD0'\n"
            "        clr\n"
            "        lr    a, j\n"
            "        ds    10\n"  // DS is the instruction of F3850
            "        bp    label1\n"
            "        include \"data/da.inc\"\n",
            "          0 :                            cpu   F3850\n"
            "          0 :                    * comment line\n"
            "       7BCD :                            org   H'7BCD'\n"
            "       7BCD : =7BD0              label1  equ   H'7BD0'\n"
            "       7BCD : 70                         clr\n"
            "       7BCE : 49                         lr    a, j\n"
            "       7BCF : 3A                         ds    10\n"
            "       7BD0 : 81 FF                      bp    label1\n"
            "       7BD2 :                            include \"data/da.inc\"\n"
            "(1)    7BD2 :                            rs    3\n"
            "(1)    7BD5 : 12 34 7B D0 9A BC          da    H'1234', label1, H'9ABC'\n"
            "(1)    7BDB : 61 2C 2C 2F 2C 20          dc    'a,',',, '/, ' , 0\n"
            "       7BE1 : 00\n"
            "(1)    7BE2 : 61 2C 62 63 27 64          dc    c'a',',, c'bc''de', 0\n"
            "       7BE8 : 65 00\n"
            "(1)    7BEA : 41 27 C3                   dc    C'A', #', C'C'+h'80'\n");
}

void test_dis_f3850() {
    PREP_DIS(f3850::DisF3850);

    driver.setUpperHex(false);

    DIS8("f3850", 0x7bcd,
            "      cpu   3850\n"
            "      org   h'7bcd'\n"
            "      clr\n"
            "      lr    a, j\n"
            "      bp    h'7bcf'\n"
            "* test.bin: error: Unknown instruction\n"
            "*     7bd1 : 2e\n",
            "       0 :                            cpu   3850\n"
            "    7bcd :                            org   h'7bcd'\n"
            "    7bcd : 70                         clr\n"
            "    7bce : 49                         lr    a, j\n"
            "    7bcf : 81 ff                      bp    h'7bcf'\n"
            "test.bin: error: Unknown instruction\n"
            "    7bd1 : 2e\n",
            0x70, 0x49, 0x81, 0xFF, 0x2E);
}

void run_tests() {
    RUN_TEST(test_asm_f3850);
    RUN_TEST(test_dis_f3850);
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

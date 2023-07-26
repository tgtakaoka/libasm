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

#include "asm_i8048.h"
#include "dis_i8048.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_i8048() {
    PREP_ASM(i8048::AsmI8048, IntelDirective);

    listing.setUpperHex(false);
    driver.internSymbol(0x8a, "data1");

    ASM("i8039",
            "        cpu   i8039\n"
            "; comment line\n"
            "        org   0bcdh\n"
            "        orl   p1, #data1\n",
            "          0 :                            cpu   i8039\n"
            "          0 :                    ; comment line\n"
            "        bcd :                            org   0bcdh\n"
            "        bcd : 89 8a                      orl   p1, #data1\n");
}

void test_dis_i8048() {
    PREP_DIS(i8048::DisI8048);

    listing.setUppercase(true);

    DIS8("i8039", 0xbcd,
            "      CPU   I8039\n"
            "      ORG   0BCDH\n"
            "      ORL   P1, #8AH\n"
            "; test.bin: 0BCFH: error: Unknown instruction\n"
            ";      BCF : 08\n",
            "       0 :                            CPU   I8039\n"
            "     BCD :                            ORG   0BCDH\n"
            "     BCD : 89 8A                      ORL   P1, #8AH\n"
            "test.bin: 0BCFH: error: Unknown instruction\n"
            "     BCF : 08                         IN    A, \n",
            0x89, 0x8A, 0x08);

    EQ("org", OVERFLOW_RANGE, listing.setOrigin(0xabcd));
}

void run_tests() {
    RUN_TEST(test_asm_i8048);
    RUN_TEST(test_dis_i8048);
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

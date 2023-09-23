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
            "        cpu   mn1610\n"
            "* comment line\n"
            "        org   x'abcd'\n"
            "        tbit  r3, 5, nz\n"
            "label:  dc    label\n"
            "        ds    2\n"
            "        dc    c'ABC',3\n",
            "          0 :                            cpu   mn1610\n"
            "          0 :                    * comment line\n"
            "       ABCD :                            org   x'abcd'\n"
            "       ABCD : 2B55                       tbit  r3, 5, nz\n"
            "       ABCE : ABCE               label:  dc    label\n"
            "       ABCF :                            ds    2\n"
            "       ABD1 : 4142 4300 0003             dc    c'ABC',3\n");
}

void test_asm_mn1613() {
    PREP_ASM(mn1610::AsmMn1610, NationalDirective);

    listing.setUpperHex(false);

    ASM("mn1613",
            "        cpu   mn1613\n"
            "* comment line\n"
            "        org   x'34567'\n"
            "        mvwi  str, x'5678', skp\n",
            "          0 :                            cpu   mn1613\n"
            "          0 :                    * comment line\n"
            "      34567 :                            org   x'34567'\n"
            "      34567 : 7e1f 5678                  mvwi  str, x'5678', skp\n");
}

void test_dis_mn1610() {
    PREP_DIS(mn1610::DisMn1610);

    listing.setUpperHex(false);

    DIS16("mn1610", 0xabcd,
            "      cpu   mn1610\n"
            "      org   x'abcd'\n"
            "      tbit  r3, 5, nz\n"
            "* test.bin: x'abce': error: Illegal register\n"
            "*     abce : 0f06\n",
            "       0 :                            cpu   mn1610\n"
            "    abcd :                            org   x'abcd'\n"
            "    abcd : 2b55                       tbit  r3, 5, nz\n"
            "test.bin: x'abce': error: Illegal register\n"
            "    abce : 0f06                       mvi\n",
            0x2b55, 0x0F00 | (0 << 4) | 6);

    EQ("org", OVERFLOW_RANGE, listing.setOrigin(0x12345));
}

void test_dis_mn1613() {
    PREP_DIS(mn1610::DisMn1610);

    listing.setUpperHex(false);

    DIS16("mn1613", 0x34567,
            "      cpu   mn1613\n"
            "      org   x'34567'\n"
            "      mvwi  str, x'5678', skp\n"
            "* test.bin: x'34569': error: Register not allowed\n"
            "*    34569 : 0f06\n",
            "       0 :                            cpu   mn1613\n"
            "   34567 :                            org   x'34567'\n"
            "   34567 : 7e1f 5678                  mvwi  str, x'5678', skp\n"
            "test.bin: x'34569': error: Register not allowed\n"
            "   34569 : 0f06                       setb  str, \n",
            0x7e1f, 0x5678, 0x0f00 | (0 << 4) | 6);
    EQ("org", OVERFLOW_RANGE, listing.setOrigin(0x45678));
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

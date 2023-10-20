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

#include "asm_mc6805.h"
#include "dis_mc6805.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_mc6805() {
    PREP_ASM(mc6805::AsmMc6805, MotorolaDirective);

    listing.setUpperHex(false);
    driver.internSymbol(0x0800, "label1");

    ASM("mc146805",
            "        cpu   mc146805\n"
            "* comment line\n"
            "        org   $1234\n"
            "        stx   label1, x\n",
            "          0 :                            cpu   mc146805\n"
            "          0 :                    * comment line\n"
            "       1234 :                            org   $1234\n"
            "       1234 : df 08 00                   stx   label1, x\n");
}

void test_dis_mc6805() {
    PREP_DIS(mc6805::DisMc6805);

    formatter.setUpperHex(false);
    formatter.setUppercase(true);

    DIS8("mc146805", 0x1234,
            "      CPU   MC146805\n"
            "      ORG   $1234\n"
            "      STX   $4000,X\n"
            "* test.bin: $1237: error: Unknown instruction\n"
            "*     1237 : 82\n"
            "* test.bin: $1238: error: Overflow range\n"
            "*     1238 : c6 20 00\n",
            "       0 :                            CPU   MC146805\n"
            "    1234 :                            ORG   $1234\n"
            "    1234 : df 40 00                   STX   $4000,X\n"
            "test.bin: $1237: error: Unknown instruction\n"
            "    1237 : 82\n"
            "test.bin: $1238: error: Overflow range\n"
            "    1238 : c6 20 00                   LDA   $2000\n",
            0xdf, 0x40, 0x00, 0x82, 0xC6, 0x20, 0x00);

    EQ("org", OVERFLOW_RANGE, formatter.setOrigin(0x3456));
}

void run_tests() {
    RUN_TEST(test_asm_mc6805);
    RUN_TEST(test_dis_mc6805);
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

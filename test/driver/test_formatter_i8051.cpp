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

    formatter.setUpperHex(false);
    symbols.internSymbol(0xb0, "data1");

    ASM("i8051",
            "        cpu   i8051\n"
            "; comment line\n"
            "        org   0abcdh\n"
            "        anl   c, /data1.1\n",
            "          0 :                            cpu   i8051\n"
            "          0 :                    ; comment line\n"
            "       abcd :                            org   0abcdh\n"
            "       abcd : b0 b1                      anl   c, /data1.1\n");
}

void test_dis_i8051() {
    PREP_DIS(i8051::DisI8051);

    formatter.setUpperHex(false);

    DIS8("i8051", 0xabcd,
            "      cpu   i8051\n"
            "      org   0abcdh\n"
            "      anl   c, /0b0h.1\n"
            "; test.bin: error: Unknown instruction\n"
            ";     abcf : a5\n",
            "       0 :                            cpu   i8051\n"
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

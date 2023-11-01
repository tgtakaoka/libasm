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

#include "asm_tms7000.h"
#include "dis_tms7000.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_tms7000() {
    PREP_ASM(tms7000::AsmTms7000, IntelDirective);

    ASM("tms7000",
            "        cpu     tms7000\n"
            "* comment line\n"
            "        aorg    >7BCD\n"
            "label1: equ     >7def\n"
            "        movd    %>a9aa(b), r171\n",
            "          0 :                            cpu     tms7000\n"
            "          0 :                    * comment line\n"
            "       7BCD :                            aorg    >7BCD\n"
            "       7BCD : =7DEF              label1: equ     >7def\n"
            "       7BCD : A8 A9 AA AB                movd    %>a9aa(b), r171\n");
}

void test_dis_tms7000() {
    PREP_DIS(tms7000::DisTms7000);

    driver.setUpperHex(false);

    DIS8("tms7000", 0x7bcd,
            "      cpu   tms7000\n"
            "      org   >7bcd\n"
            "      movd  %>a9aa(b), r171\n"
            "* test.bin: error: Unknown instruction\n"
            "*     7bd1 : af\n",
            "       0 :                            cpu   tms7000\n"
            "    7bcd :                            org   >7bcd\n"
            "    7bcd : a8 a9 aa ab                movd  %>a9aa(b), r171\n"
            "test.bin: error: Unknown instruction\n"
            "    7bd1 : af\n",
         0xA8, 0xA9, 0xAA, 0xAB, 0xAF);
}

void run_tests() {
    RUN_TEST(test_asm_tms7000);
    RUN_TEST(test_dis_tms7000);
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

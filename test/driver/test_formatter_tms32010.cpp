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

#include "asm_tms32010.h"
#include "dis_tms32010.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_tms32010() {
    PREP_ASM(tms32010::AsmTms32010, IntelDirective);

    formatter.setUpperHex(false);

    ASM("tms32010",
            "        .cpu  tms32010\n"
            "* comment line\n"
            "        .org  789h\n"
            "        call  0fedh\n"
            "        sacl  *+, 0, ar0\n"
            "label:  .word label\n"
            "        .byte 1,'''',3\n"
            "        .string \"A\"\"B'C\"\n",
            "          0 :                            .cpu  tms32010\n"
            "          0 :                    * comment line\n"
            "        789 :                            .org  789h\n"
            "        789 : f800 0fed                  call  0fedh\n"
            "        78b : 50a0                       sacl  *+, 0, ar0\n"
            "        78c : 078c               label:  .word label\n"
            "        78d : 0001 0027 0003             .byte 1,'''',3\n"
            "        790 : 2241 2742 0043             .string \"A\"\"B'C\"\n");
}

void test_dis_tms32010() {
    PREP_DIS(tms32010::DisTms32010);

    formatter.setUpperHex(false);

    DIS16("tms32010", 0x789,
            "      cpu   tms32010\n"
            "      org   789h\n"
            "      call  0fedh\n"
            "      sacl  *+, 0, ar0\n"
            "* test.bin: error: Unknown instruction\n"
            "*      78c : 0086\n",
            "       0 :                            cpu   tms32010\n"
            "     789 :                            org   789h\n"
            "     789 : f800 0fed                  call  0fedh\n"
            "     78b : 50a0                       sacl  *+, 0, ar0\n"
            "test.bin: error: Unknown instruction\n"
            "     78c : 0086\n",
            0xf800, 0x0fed, 0x50a0, 0x0086);

    EQ("org", OVERFLOW_RANGE, formatter.setOrigin(0xabcd));
}

void run_tests() {
    RUN_TEST(test_asm_tms32010);
    RUN_TEST(test_dis_tms32010);
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

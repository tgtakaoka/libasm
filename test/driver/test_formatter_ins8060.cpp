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

#include "asm_ins8060.h"
#include "dis_ins8060.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_ins8060() {
    PREP_ASM(ins8060::AsmIns8060, NationalDirective);

    ASM("ins8060",
            "        cpu    ins8060\n"
            "; comment line\n"
            "        .=x'abcd\n"
            "label:  and    @e(p1)\n"
            "        .byte  h(label)\n"
            "        .byte  l(label)\n"
            "        .dbyte addr(label)\n"
            "        .dbyte addr(x'1000)\n",
            "          0 :                            cpu    ins8060\n"
            "          0 :                    ; comment line\n"
            "       ABCD :                            .=x'abcd\n"
            "       ABCD : D5 80              label:  and    @e(p1)\n"
            "       ABCF : AB                         .byte  h(label)\n"
            "       ABD0 : CD                         .byte  l(label)\n"
            "       ABD1 : CC AB                      .dbyte addr(label)\n"
            "       ABD3 : FF 1F                      .dbyte addr(x'1000)\n");
}

void test_dis_ins8060() {
    PREP_DIS(ins8060::DisIns8060);

    DIS8("ins8060", 0xabcd,
            "      cpu   ins8060\n"
            "      org   X'ABCD\n"
            "      and   @e(p1)\n"
            "; test.bin: X'ABCF: error: Unknown instruction\n"
            ";     ABCF : 09\n",
            "       0 :                            cpu   ins8060\n"
            "    ABCD :                            org   X'ABCD\n"
            "    ABCD : D5 80                      and   @e(p1)\n"
            "test.bin: X'ABCF: error: Unknown instruction\n"
            "    ABCF : 09\n",
            0xd5, 0x80, 0x09);
}

void run_tests() {
    RUN_TEST(test_asm_ins8060);
    RUN_TEST(test_dis_ins8060);
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

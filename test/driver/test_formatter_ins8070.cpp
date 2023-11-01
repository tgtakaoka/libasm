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

#include "asm_ins8070.h"
#include "dis_ins8070.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_ins8070() {
    PREP_ASM(ins8070::AsmIns8070, NationalDirective);

    driver.setUpperHex(false);

    ASM("ins8070",
            "        cpu    ins8070\n"
            "; comment line\n"
            "        .=x'abcd\n"
            "label:  pli    p2, =x'2423\n"
            "        .byte  h(label)\n"
            "        .byte  l(label)\n"
            "        .dbyte addr(label)\n"
            "        .dbyte addr(x'1000)\n",
            "          0 :                            cpu    ins8070\n"
            "          0 :                    ; comment line\n"
            "       abcd :                            .=x'abcd\n"
            "       abcd : 22 23 24           label:  pli    p2, =x'2423\n"
            "       abd0 : ab                         .byte  h(label)\n"
            "       abd1 : cd                         .byte  l(label)\n"
            "       abd2 : cc ab                      .dbyte addr(label)\n"
            "       abd4 : ff 0f                      .dbyte addr(x'1000)\n");
}

void test_dis_ins8070() {
    PREP_DIS(ins8070::DisIns8070);

    driver.setUpperHex(false);

    DIS8("ins8070", 0xabcd,
            "      cpu   8070\n"
            "      org   x'abcd\n"
            "      pli   p2, =x'2423\n"
            "; test.bin: error: Unknown instruction\n"
            ";     abd0 : ef\n",
            "       0 :                            cpu   8070\n"
            "    abcd :                            org   x'abcd\n"
            "    abcd : 22 23 24                   pli   p2, =x'2423\n"
            "test.bin: error: Unknown instruction\n"
            "    abd0 : ef\n",
            0x22, 0x23, 0x24, 0xef);
}

void run_tests() {
    RUN_TEST(test_asm_ins8070);
    RUN_TEST(test_dis_ins8070);
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

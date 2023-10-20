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

#include "asm_tlcs90.h"
#include "dis_tlcs90.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_tlcs90() {
    PREP_ASM(tlcs90::AsmTlcs90, ZilogDirective);

    listing.setUpperHex(false);

    ASM("tlcs90",
            "        cpu   tlcs90\n"
            "; comment line\n"
            "        org   0abcdh\n"
            "        xor   (hl+a), 0efh\n"
            "        set   7,a\n",
            "          0 :                            cpu   tlcs90\n"
            "          0 :                    ; comment line\n"
            "       abcd :                            org   0abcdh\n"
            "       abcd : f7 6d ef                   xor   (hl+a), 0efh\n"
            "       abd0 : fe bf                      set   7,a\n");
}

void test_dis_tlcs90() {
    PREP_DIS(tlcs90::DisTlcs90);

    formatter.setUpperHex(false);

    DIS8("tlcs90", 0xabcd,
            "      cpu   tlcs90\n"
            "      org   0abcdh\n"
            "      xor   (hl+a), 0efh\n"
            "; test.bin: 0abd0h: error: Unknown instruction\n"
            ";     abd0 : e3 12 34 17\n",
            "       0 :                            cpu   tlcs90\n"
            "    abcd :                            org   0abcdh\n"
            "    abcd : f7 6d ef                   xor   (hl+a), 0efh\n"
            "test.bin: 0abd0h: error: Unknown instruction\n"
            "    abd0 : e3 12 34 17\n",
            0xf7, 0x6d, 0xef, 0xe3, 0x12, 0x34, 0x17);
}

void run_tests() {
    RUN_TEST(test_asm_tlcs90);
    RUN_TEST(test_dis_tlcs90);
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

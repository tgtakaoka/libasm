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

    driver.setUpperHex(false);
    options.set("smart-branch", "on");

    ASM("tlcs90",
            R"(        cpu   tlcs90
; comment line
        org   0abcdh
        xor   (hl+a), 0efh
        set   7,a
        jrl   label2
        jr    label3
label2: equ   0ac00h
label3: equ   0ad00h
)",
            R"(          0 :                            cpu   tlcs90
          0 :                    ; comment line
       abcd :                            org   0abcdh
       abcd : f7 6d ef                   xor   (hl+a), 0efh
       abd0 : fe bf                      set   7,a
       abd2 : c8 2c                      jrl   label2
       abd4 : 1b 2a 01                   jr    label3
       abd7 : =ac00              label2: equ   0ac00h
       abd7 : =ad00              label3: equ   0ad00h
)");
}

void test_dis_tlcs90() {
    PREP_DIS(tlcs90::DisTlcs90);

    driver.setUpperHex(false);

    DIS8("tlcs90", 0xabcd,
            R"(      cpu   tlcs90
      org   0abcdh
      xor   (hl+a), 0efh
; test.bin: error: Unknown instruction
;     abd0 : e3 12 34 17
)",
            R"(       0 :                            cpu   tlcs90
    abcd :                            org   0abcdh
    abcd : f7 6d ef                   xor   (hl+a), 0efh
test.bin: error: Unknown instruction
    abd0 : e3 12 34 17
)",
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

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

#include "asm_i8096.h"
#include "dis_i8096.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_i8096() {
    PREP_ASM(i8096::AsmI8096, IntelDirective);

    driver.setUpperHex(false);
    driver.setOption("smart-branch", "on");

    ASM("i8096",
            R"(        cpu   i8096
; comment line
label1: equ   817bh
        org   0abcdh
        mulb  130, label1[124]
        ljmp  label2
        scall label3
label2: equ   0ac00h
label3: equ   0b000h
)",
            R"(          0 :                            cpu   i8096
          0 :                    ; comment line
          0 : =817b              label1: equ   817bh
       abcd :                            org   0abcdh
       abcd : fe 7f 7d 7b 81 82          mulb  130, label1[124]
       abd3 : 20 2b                      ljmp  label2
       abd5 : ef 28 04                   scall label3
       abd8 : =ac00              label2: equ   0ac00h
       abd8 : =b000              label3: equ   0b000h
)");
}

void test_dis_i8096() {
    PREP_DIS(i8096::DisI8096);

    driver.setUpperHex(false);

    DIS8("i8096", 0xabcd,
            R"(      cpu   8096
      org   0abcdh
      mulb  130, 817bh[124]
; test.bin: error: Operand not aligned: "121, 5634h[18]"
;     abd3 : 67 13 34 56 79             add   121, 5634h[18]
)",
            R"(       0 :                            cpu   8096
    abcd :                            org   0abcdh
    abcd : fe 7f 7d 7b 81 82          mulb  130, 817bh[124]
test.bin: error: Operand not aligned: "121, 5634h[18]"
    abd3 : 67 13 34 56 79             add   121, 5634h[18]
)",
            0xfe, 0x7f, 0x7d, 0x7b, 0x81, 0x82, 0x67, 0x13, 0x34, 0x56, 0x79);
}

void run_tests() {
    RUN_TEST(test_asm_i8096);
    RUN_TEST(test_dis_i8096);
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

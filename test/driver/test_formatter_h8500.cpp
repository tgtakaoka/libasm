/*
 * Copyright 2026 Tadashi G. Takaoka
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

#include "asm_h8500.h"
#include "dis_h8500.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_h8500() {
    PREP_ASM(h8500::AsmH8500, MotorolaDirective);

    driver.setUpperHex(false);

    ASM("H8/500",
            R"(        cpu   H8/500
; comment line
        org   H'0100
label1: equ   H'0200
        nop
        mov:g r0, r1
        jmp   @label1
label2: nop
)",
            R"(          0 :                            cpu   H8/500
          0 :                    ; comment line
        100 :                            org   H'0100
        100 : =200               label1: equ   H'0200
        100 : 00                         nop
        101 : a8 81                      mov:g r0, r1
        103 : 10 02 00                   jmp   @label1
        106 : 00                 label2: nop
)");
}

void test_dis_h8500() {
    PREP_DIS(h8500::DisH8500);

    driver.setUppercase(true);

    DIS8("H8/500", 0x0100,
            R"(      CPU      H8/500
      ORG      H'0100
      NOP
      MOV:G    R0, R1
      JMP      @H'0200:16
)",
            R"(       0 :                            CPU      H8/500
     100 :                            ORG      H'0100
     100 : 00                         NOP
     101 : A8 81                      MOV:G    R0, R1
     103 : 10 02 00                   JMP      @H'0200:16
)",
            0x00, 0xA8, 0x81, 0x10, 0x02, 0x00);
}

void test_asm_h8520() {
    PREP_ASM(h8500::AsmH8500, MotorolaDirective);

    driver.setUpperHex(false);

    ASM("H8/520",
            R"(        cpu   H8/520
        org   H'0100
        nop
        mov:g r0, r1
)",
            R"(          0 :                            cpu   H8/520
        100 :                            org   H'0100
        100 : 00                         nop
        101 : a8 81                      mov:g r0, r1
)");
}

void run_tests() {
    RUN_TEST(test_asm_h8500);
    RUN_TEST(test_dis_h8500);
    RUN_TEST(test_asm_h8520);
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

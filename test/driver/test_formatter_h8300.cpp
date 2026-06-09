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

#include "asm_h8300.h"
#include "dis_h8300.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_h8300() {
    PREP_ASM(h8300::AsmH8300, MotorolaDirective);

    driver.setUpperHex(false);

    ASM("H8/300",
            R"(        cpu   H8/300
; comment line
        org   H'abcc
label1: equ   H'f1f2
        nop
        mov.b r0h, r0h
        jmp   @label1
label2: nop
)",
            R"(          0 :                            cpu   H8/300
          0 :                    ; comment line
       abcc :                            org   H'abcc
       abcc : =f1f2              label1: equ   H'f1f2
       abcc : 0000                       nop
       abce : 0c00                       mov.b r0h, r0h
       abd0 : 5a00 f1f2                  jmp   @label1
       abd4 : 0000               label2: nop
)");
}

void test_dis_h8300() {
    PREP_DIS(h8300::DisH8300);

    driver.setUppercase(true);

    DIS16("H8/300", 0xabcc,
            R"(      CPU    H8/300
      ORG    H'ABCC
      NOP
      MOV.B  R0H, R0H
)",
            R"(       0 :                            CPU    H8/300
    ABCC :                            ORG    H'ABCC
    ABCC : 0000                       NOP
    ABCE : 0C00                       MOV.B  R0H, R0H
)",
            0x0000, 0x0C00);
}

void test_asm_h8300h() {
    PREP_ASM(h8300::AsmH8300, MotorolaDirective);

    driver.setUpperHex(false);

    ASM("H8/300H",
            R"(        cpu     H8/300H
        option  advanced-mode, on
        org     H'abcdee
        nop
        mov.b   r0h, @H'fedcba:24
)",
            R"(          0 :                            cpu     H8/300H
          0 :                            option  advanced-mode, on
     abcdee :                            org     H'abcdee
     abcdee : 0000                       nop
     abcdf0 : 6aa0 00fe dcba             mov.b   r0h, @H'fedcba:24
)");
}

void test_dis_h8300h() {
    PREP_DIS(h8300::DisH8300);

    driver.setUppercase(true);
    driver.setOption("advanced-mode", "on");

    DIS16("H8/300H", 0xabcdee,
            R"(      CPU    H8/300H
      ORG    H'ABCDEE
      NOP
      MOV.B  R0H, @H'FEDCBA:24
)",
            R"(       0 :                            CPU    H8/300H
  ABCDEE :                            ORG    H'ABCDEE
  ABCDEE : 0000                       NOP
  ABCDF0 : 6AA0 00FE DCBA             MOV.B  R0H, @H'FEDCBA:24
)",
            0x0000, 0x6AA0, 0x00FE, 0xDCBA);
}

void run_tests() {
    RUN_TEST(test_asm_h8300);
    RUN_TEST(test_dis_h8300);
    RUN_TEST(test_asm_h8300h);
    RUN_TEST(test_dis_h8300h);
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

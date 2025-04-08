/*
 * Copyright 2024 Tadashi G. Takaoka
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

#include "asm_pdp11.h"
#include "dis_pdp11.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_j11() {
    PREP_ASM(pdp11::AsmPdp11, DecDirective);

    ASM("j11",
            R"(        cpu   j11
        option "implicit-word", "on"
; comment line
        .= 100
label:  mov     #100, r3
        addd    ac1, ac2
        mov     @#12345, r4
        .byte   'A
        .even
        .byte   'B
        .word   "CD
        .odd
        .byte   'E
        br      label
        .flt2   1.234e-5)",
            R"(          0 :                            cpu   j11
          0 :                            option "implicit-word", "on"
          0 :                    ; comment line
        100 :                            .= 100
        100 : 012703 000100      label:  mov     #100, r3
        104 : 172201                     addd    ac1, ac2
j11:7:17: error: Operand not aligned: "@#12345, r4"
        106 : 013704 012345              mov     @#12345, r4
        112 :    101                     .byte   'A
        114 :                            .even
        114 :    102                     .byte   'B
        116 : 042103                     .word   "CD
        121 :                            .odd
        121 : 105                        .byte   'E
        122 : 000766                     br      label
        124 : 034117 003745              .flt2   1.234e-5)");
}

void test_asm_t11() {
    PREP_ASM(pdp11::AsmPdp11, DecDirective);

    ASM("t11",
            R"(        cpu   t11
        option "implicit-word", "on"
        .= 100
clrsp   = 005046
; comment line
        clr    -(sp)
        clrsp          ; comment
        br     20000
        .flt4  1.234e-5)",
            R"(          0 :                            cpu   t11
          0 :                            option "implicit-word", "on"
        100 :                            .= 100
        100 : =5046              clrsp   = 005046
        100 :                    ; comment line
        100 : 005046                     clr    -(sp)
        102 : 005046                     clrsp          ; comment
t11:8:16: error: Operand too far: "20000"
        104 : 000735                     br     20000
        106 : 034117 003745              .flt4  1.234e-5
        112 : 076234 003736)");
}

void test_dis_j11() {
    PREP_DIS(pdp11::DisPdp11);

    DIS16("j11", 010000,
            R"(      cpu    j11
      org    010000
      tstset 012345(r5)
      csm    @022352
; test.bin: error: Register not allowed: "r1"
;    10010 : 006501                     mfpi   r1
      ldd    #1.70141183460469229371e+38, ac0)",
            R"(       0 :                            cpu    j11
   10000 :                            org    010000
   10000 : 007265 012345              tstset 012345(r5)
   10004 : 007077 012342              csm    @022352
test.bin: error: Register not allowed: "r1"
   10010 : 006501                     mfpi   r1
   10012 : 172427 077777              ldd    #1.70141183460469229371e+38, ac0
   10016 : 177777 177777
   10022 : 177777)",
            0007265, 0012345, 007077, 012342, 006501, 0172427, 0x7FFF, 0xFFFF, 0xFFFF, 0xFFFF);
}

void test_dis_t11() {
    PREP_DIS(pdp11::DisPdp11);

    DIS16("t11", 010000,
            R"(      cpu   t11
      org   010000
      mov   -(r4), -(r5)
      negb  012345(r5)
; test.bin: error: Unknown instruction
;    10006 : 070201
      jsr   r2, @#012346)",
            R"(       0 :                            cpu   t11
   10000 :                            org   010000
   10000 : 014445                     mov   -(r4), -(r5)
   10002 : 105465 012345              negb  012345(r5)
test.bin: error: Unknown instruction
   10006 : 070201
   10010 : 004237 012346              jsr   r2, @#012346)",
            0014445, 0105465, 012345, 070201, 0004237, 012346);
}

void run_tests() {
    RUN_TEST(test_asm_j11);
    RUN_TEST(test_asm_t11);
    RUN_TEST(test_dis_j11);
    RUN_TEST(test_dis_t11);
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

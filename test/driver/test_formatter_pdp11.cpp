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
        .=0100
label:  mov     #100, d0
        .byte   'A
        .even
        .byte   'B
        .word   "CD
        .odd
        .byte   'E
)",
            R"(          0 :                            cpu   j11
          0 :                            option "implicit-word", "on"
          0 :                    ; comment line
        100 :                            .=0100
        100 : 012767 000100 000000   label:  mov     #100, d0
        106 :    101                     .byte   'A
        110 :                            .even
        110 :    102                     .byte   'B
        112 : 042103                     .word   "CD
        115 :                            .odd
        115 : 105                        .byte   'E)");
}

void test_asm_t11() {
    PREP_ASM(pdp11::AsmPdp11, DecDirective);

    ASM("t11",
            R"(        cpu   t11
        option "implicit-word", "on"
; comment line
)",
        R"(          0 :                            cpu   t11
          0 :                            option "implicit-word", "on"
          0 :                    ; comment line)");
}

#if 0
void test_dis_j11() {
    PREP_DIS(pdp11::DisPdp11);

    driver.setOption("implicit-word", "off");

    DIS16("j11", 0400,
            R"(      cpu   6100
      org   0400
      tad   i 0105
/ test.bin: error: Invalid instruction combination
/      401 : 7175                       stl   cia
      jms   0474
      jmp   0401
/ test.bin: error: Invalid instruction combination
/      404 : 7777                       cla   swp
      cml   bsw iac
      spa   sna szl cla hlt
)",
            R"(       0 :                            cpu   6100
     400 :                            org   0400
     400 : 1505                       tad   i 0105
test.bin: error: Invalid instruction combination
     401 : 7175                       stl   cia
     402 : 4274                       jms   0474
     403 : 5201                       jmp   0401
test.bin: error: Invalid instruction combination
     404 : 7777                       cla   swp
     405 : 7023                       cml   bsw iac
     406 : 7772                       spa   sna szl cla hlt
)",
            01505, 07175, 04274, 05201, 07777, 07023, 07772);

    driver.setOption("implicit-word", "on");

    DIS16("j11", 0400,
            R"(      cpu   6100
      org   0400
      tad   i 0105
      jms   0474
      jmp   0401
      cml   bsw iac
      spa   sna szl cla hlt
)",
            R"(       0 :                            cpu   6100
     400 :                            org   0400
     400 : 1505                       tad   i 0105
     401 : 4274                       jms   0474
     402 : 5201                       jmp   0401
     403 : 7023                       cml   bsw iac
     404 : 7772                       spa   sna szl cla hlt
)",
            01505, 04274, 05201, 07023, 07772);
}

void test_dis_t11() {
    PREP_DIS(pdp11::DisPdp11);

    driver.setOption("implicit-word", "off");

    DIS16("t11", 070400,
            R"(      cpu   6120
      org   70400
      tad   i 00105
      stl   cia r3l
      jms   70474
      jmp   70401
/ test.bin: error: Invalid instruction combination
/    70404 : 7777                       cla   swp
      cml   bsw iac
      spa   sna szl cla hlt
)",
            R"(       0 :                            cpu   6120
   70400 :                            org   70400
   70400 : 1505                       tad   i 00105
   70401 : 7175                       stl   cia r3l
   70402 : 4274                       jms   70474
   70403 : 5201                       jmp   70401
test.bin: error: Invalid instruction combination
   70404 : 7777                       cla   swp
   70405 : 7023                       cml   bsw iac
   70406 : 7772                       spa   sna szl cla hlt
)",
            01505, 07175, 04274, 05201, 07777, 07023, 07772);

    driver.setOption("implicit-word", "on");

    DIS16("t11", 070400,
            R"(      cpu   6120
      org   70400
      tad   i 00105
      stl   cia r3l
      jms   70474
      jmp   70401
      cml   bsw iac
      spa   sna szl cla hlt
)",
            R"(       0 :                            cpu   6120
   70400 :                            org   70400
   70400 : 1505                       tad   i 00105
   70401 : 7175                       stl   cia r3l
   70402 : 4274                       jms   70474
   70403 : 5201                       jmp   70401
   70404 : 7023                       cml   bsw iac
   70405 : 7772                       spa   sna szl cla hlt
)",
            01505, 07175, 04274, 05201, 07023, 07772);
}
#endif

void run_tests() {
    RUN_TEST(test_asm_j11);
    RUN_TEST(test_asm_t11);
#if 0
    RUN_TEST(test_dis_j11);
    RUN_TEST(test_dis_t11);
#endif
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

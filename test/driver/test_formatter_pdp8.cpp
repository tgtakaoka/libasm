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

#include "asm_pdp8.h"
#include "dis_pdp8.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_im6100() {
    PREP_ASM(pdp8::AsmPdp8, DecDirective);

    ASM("im6100",
            R"(        cpu   im6100
/ comment line
        *0400
        tad   i 0105   / comment
label,  0
symbol=0474
        jms   symbol
        jmp   label
        page
        cml   bsw iac
        page  6
        spa   sna szl cla hlt
)",
            R"(          0 :                            cpu   im6100
          0 :                    / comment line
        400 :                            *0400
        400 : 1505                       tad   i 0105   / comment
        401 : 0000               label,  0
        402 : =474               symbol=0474
        402 : 4274                       jms   symbol
        403 : 5201                       jmp   label
        600 :                            page
        600 : 7023                       cml   bsw iac
       1400 :                            page  6
       1400 : 7772                       spa   sna szl cla hlt
)");
}

void test_asm_hd6120() {
    PREP_ASM(pdp8::AsmPdp8, DecDirective);

    ASM("hd6120",
            R"(        cpu   hd6120
/ comment line
        *0400
        tad   i 0105   / comment
label,  0
symbol=0474
        jms   symbol
        jmp   label
        page
        cml   bsw iac
        page  6
        spa   sna szl cla hlt
        field 7
        cif   70
        page  2
        cdf   cif 7
)",
            R"(          0 :                            cpu   hd6120
          0 :                    / comment line
        400 :                            *0400
        400 : 1505                       tad   i 0105   / comment
        401 : 0000               label,  0
        402 : =474               symbol=0474
        402 : 4274                       jms   symbol
        403 : 5201                       jmp   label
        600 :                            page
        600 : 7023                       cml   bsw iac
       1400 :                            page  6
       1400 : 7772                       spa   sna szl cla hlt
      70000 :                            field 7
      70000 : 6272                       cif   70
      70400 :                            page  2
      70400 : 6273                       cdf   cif 7
)");
}

void test_dis_im6100() {
    PREP_DIS(pdp8::DisPdp8);

    driver.setOption("ignore-literal", "on");

    DIS16("im6100", 0400,
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

    driver.setOption("ignore-literal", "off");

    DIS16("im6100", 0400,
            R"(      cpu   6100
      org   0400
      tad   i 0105
      7175
      jms   0474
      jmp   0401
      7777
      cml   bsw iac
      spa   sna szl cla hlt
)",
            R"(       0 :                            cpu   6100
     400 :                            org   0400
     400 : 1505                       tad   i 0105
     401 : 7175                       7175
     402 : 4274                       jms   0474
     403 : 5201                       jmp   0401
     404 : 7777                       7777
     405 : 7023                       cml   bsw iac
     406 : 7772                       spa   sna szl cla hlt
)",
            01505, 07175, 04274, 05201, 07777, 07023, 07772);
}

void test_dis_hd6120() {
    PREP_DIS(pdp8::DisPdp8);

    driver.setOption("ignore-literal", "on");

    DIS16("hd6120", 070400,
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

    driver.setOption("ignore-literal", "off");

    DIS16("hd6120", 070400,
            R"(      cpu   6120
      org   70400
      tad   i 00105
      stl   cia r3l
      jms   70474
      jmp   70401
      7777
      cml   bsw iac
      spa   sna szl cla hlt
)",
            R"(       0 :                            cpu   6120
   70400 :                            org   70400
   70400 : 1505                       tad   i 00105
   70401 : 7175                       stl   cia r3l
   70402 : 4274                       jms   70474
   70403 : 5201                       jmp   70401
   70404 : 7777                       7777
   70405 : 7023                       cml   bsw iac
   70406 : 7772                       spa   sna szl cla hlt
)",
            01505, 07175, 04274, 05201, 07777, 07023, 07772);
}

void run_tests() {
    RUN_TEST(test_asm_im6100);
    RUN_TEST(test_asm_hd6120);
    RUN_TEST(test_dis_im6100);
    RUN_TEST(test_dis_hd6120);
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

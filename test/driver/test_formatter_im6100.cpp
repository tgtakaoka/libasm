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

#include "asm_im6100.h"
#include "dis_im6100.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_im6100() {
    PREP_ASM(im6100::AsmIm6100, DecDirective);

    ASM("im6100",
            "        cpu   im6100\n"
            "/ comment line\n"
            "        *0400\n"
            "        tad   i 0105   / comment\n"
            "label,  0\n"
            "symbol=0474\n"
            "        jms   symbol\n"
            "        jmp   label\n"
            "        page\n"
            "        cml   bsw iac\n"
            "        page  6\n"
            "        spa   sna szl cla hlt\n",
            "          0 :                            cpu   im6100\n"
            "          0 :                    / comment line\n"
            "        400 :                            *0400\n"
            "        400 : 1505                       tad   i 0105   / comment\n"
            "        401 : 0000               label,  0\n"
            "        402 : =474               symbol=0474\n"
            "        402 : 4274                       jms   symbol\n"
            "        403 : 5201                       jmp   label\n"
            "        600 :                            page\n"
            "        600 : 7023                       cml   bsw iac\n"
            "       1400 :                            page  6\n"
            "       1400 : 7772                       spa   sna szl cla hlt\n");
}

void test_asm_hd6120() {
    PREP_ASM(im6100::AsmIm6100, DecDirective);

    ASM("hd6120",
            "        cpu   hd6120\n"
            "/ comment line\n"
            "        *0400\n"
            "        tad   i 0105   / comment\n"
            "label,  0\n"
            "symbol=0474\n"
            "        jms   symbol\n"
            "        jmp   label\n"
            "        page\n"
            "        cml   bsw iac\n"
            "        page  6\n"
            "        spa   sna szl cla hlt\n"
            "        field 7\n"
            "        cif   70\n"
            "        page  2\n"
            "        cdf   cif 7\n",
            "          0 :                            cpu   hd6120\n"
            "          0 :                    / comment line\n"
            "        400 :                            *0400\n"
            "        400 : 1505                       tad   i 0105   / comment\n"
            "        401 : 0000               label,  0\n"
            "        402 : =474               symbol=0474\n"
            "        402 : 4274                       jms   symbol\n"
            "        403 : 5201                       jmp   label\n"
            "        600 :                            page\n"
            "        600 : 7023                       cml   bsw iac\n"
            "       1400 :                            page  6\n"
            "       1400 : 7772                       spa   sna szl cla hlt\n"
            "      70000 :                            field 7\n"
            "      70000 : 6272                       cif   70\n"
            "      70400 :                            page  2\n"
            "      70400 : 6273                       cdf   cif 7\n");
}

void test_dis_im6100() {
    PREP_DIS(im6100::DisIm6100);

    driver.setOption("ignore-literal", "on");

    DIS16("im6100", 0400,
            "      cpu   6100\n"
            "      org   0400\n"
            "      tad   i 0105\n"
            "/ test.bin: error: Invalid instruction combination\n"
            "/      401 : 7175                       stl   cia\n"
            "      jms   0474\n"
            "      jmp   0401\n"
            "/ test.bin: error: Invalid instruction combination\n"
            "/      404 : 7777                       cla   swp\n"
            "      cml   bsw iac\n"
            "      spa   sna szl cla hlt\n",
            "       0 :                            cpu   6100\n"
            "     400 :                            org   0400\n"
            "     400 : 1505                       tad   i 0105\n"
            "test.bin: error: Invalid instruction combination\n"
            "     401 : 7175                       stl   cia\n"
            "     402 : 4274                       jms   0474\n"
            "     403 : 5201                       jmp   0401\n"
            "test.bin: error: Invalid instruction combination\n"
            "     404 : 7777                       cla   swp\n"
            "     405 : 7023                       cml   bsw iac\n"
            "     406 : 7772                       spa   sna szl cla hlt\n",
            01505, 07175, 04274, 05201, 07777, 07023, 07772);

    driver.setOption("ignore-literal", "off");
    
    DIS16("im6100", 0400,
            "      cpu   6100\n"
            "      org   0400\n"
            "      tad   i 0105\n"
            "      7175\n"
            "      jms   0474\n"
            "      jmp   0401\n"
            "      7777\n"
            "      cml   bsw iac\n"
            "      spa   sna szl cla hlt\n",
            "       0 :                            cpu   6100\n"
            "     400 :                            org   0400\n"
            "     400 : 1505                       tad   i 0105\n"
            "     401 : 7175                       7175\n"
            "     402 : 4274                       jms   0474\n"
            "     403 : 5201                       jmp   0401\n"
            "     404 : 7777                       7777\n"
            "     405 : 7023                       cml   bsw iac\n"
            "     406 : 7772                       spa   sna szl cla hlt\n",
            01505, 07175, 04274, 05201, 07777, 07023, 07772);
}

void test_dis_hd6120() {
    PREP_DIS(im6100::DisIm6100);

    driver.setOption("ignore-literal", "on");

    DIS16("hd6120", 070400,
            "      cpu   6120\n"
            "      org   70400\n"
            "      tad   i 00105\n"
            "      stl   cia r3l\n"
            "      jms   70474\n"
            "      jmp   70401\n"
            "/ test.bin: error: Invalid instruction combination\n"
            "/    70404 : 7777                       cla   swp\n"
            "      cml   bsw iac\n"
            "      spa   sna szl cla hlt\n",
            "       0 :                            cpu   6120\n"
            "   70400 :                            org   70400\n"
            "   70400 : 1505                       tad   i 00105\n"
            "   70401 : 7175                       stl   cia r3l\n"
            "   70402 : 4274                       jms   70474\n"
            "   70403 : 5201                       jmp   70401\n"
            "test.bin: error: Invalid instruction combination\n"
            "   70404 : 7777                       cla   swp\n"
            "   70405 : 7023                       cml   bsw iac\n"
            "   70406 : 7772                       spa   sna szl cla hlt\n",
            01505, 07175, 04274, 05201, 07777, 07023, 07772);

    driver.setOption("ignore-literal", "off");

    DIS16("hd6120", 070400,
            "      cpu   6120\n"
            "      org   70400\n"
            "      tad   i 00105\n"
            "      stl   cia r3l\n"
            "      jms   70474\n"
            "      jmp   70401\n"
            "      7777\n"
            "      cml   bsw iac\n"
            "      spa   sna szl cla hlt\n",
            "       0 :                            cpu   6120\n"
            "   70400 :                            org   70400\n"
            "   70400 : 1505                       tad   i 00105\n"
            "   70401 : 7175                       stl   cia r3l\n"
            "   70402 : 4274                       jms   70474\n"
            "   70403 : 5201                       jmp   70401\n"
            "   70404 : 7777                       7777\n"
            "   70405 : 7023                       cml   bsw iac\n"
            "   70406 : 7772                       spa   sna szl cla hlt\n",
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

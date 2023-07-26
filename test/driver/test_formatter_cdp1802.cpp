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

#include "asm_cdp1802.h"
#include "dis_cdp1802.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_cdp1802() {
    PREP_ASM(cdp1802::AsmCdp1802, RcaDirective);

    listing.setUpperHex(false);

    ASM("cdp1804",
            "        cpu   cdp1804\n"
            ".. comment line\n"
            "        org   x'abcd'\n"
            "        scal  3, #8485\n"
            "label:  dc    a(label)\n"
            "symbol  =     x'1234'\n"
            "        ldi   a.0(symbol)\n"
            "        ldi   a.1(symbol)\n",
            "          0 :                            cpu   cdp1804\n"
            "          0 :                    .. comment line\n"
            "       abcd :                            org   x'abcd'\n"
            "       abcd : 68 83 84 85                scal  3, #8485\n"
            "       abd1 : ab d1              label:  dc    a(label)\n"
            "       abd3 : =1234              symbol  =     x'1234'\n"
            "       abd3 : f8 34                      ldi   a.0(symbol)\n"
            "       abd5 : f8 12                      ldi   a.1(symbol)\n"

    );

    assembler.setOption("use-register", "on");

    ASM("cdp1804",
            "        org   #abcd\n"
            "        scal  r3, x'8485'\n",
            "       abcd :                            org   #abcd\n"
            "       abcd : 68 83 84 85                scal  r3, x'8485'\n");
}

void test_asm_dis_cdp1802() {
    PREP_DIS(cdp1802::DisCdp1802);

    listing.setUpperHex(false);
    EQ("use-register", OK, disassembler.setOption("use-register", "on"));

    DIS8("cdp1804", 0xabcd,
            "      cpu   cdp1804\n"
            "      org   0abcdh\n"
            "      scal  r3, 8485h\n"
            "; test.bin: 0abd1h: error: Unknown instruction\n"
            ";     abd1 : 68 0f\n",
            "       0 :                            cpu   cdp1804\n"
            "    abcd :                            org   0abcdh\n"
            "    abcd : 68 83 84 85                scal  r3, 8485h\n"
            "test.bin: 0abd1h: error: Unknown instruction\n"
            "    abd1 : 68 0f\n",
            0x68, 0x83, 0x84, 0x85, 0x68, 0x0f);
}

void run_tests() {
    RUN_TEST(test_asm_cdp1802);
    RUN_TEST(test_asm_dis_cdp1802);
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

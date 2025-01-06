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

    driver.setUpperHex(false);

    ASM("cdp1804",
            R"(        cpu   cdp1804
.. comment line
        org   x'abcd'
        scal  3, #8485
label:  dc    a(label)
symbol  =     x'1234'
        ldi   a.0(symbol)
        ldi   a.1(symbol)
        lbr   x'ab23'
)",
            R"(          0 :                            cpu   cdp1804
          0 :                    .. comment line
       abcd :                            org   x'abcd'
       abcd : 68 83 84 85                scal  3, #8485
       abd1 : ab d1              label:  dc    a(label)
       abd3 : =1234              symbol  =     x'1234'
       abd3 : f8 34                      ldi   a.0(symbol)
       abd5 : f8 12                      ldi   a.1(symbol)
       abd7 : c0 ab 23                   lbr   x'ab23'
)");

    options.set("use-register", "on");
    options.set("smart-branch", "on");

    ASM("cdp1804",
            R"(        org   x'abcd'
        scal  r3, x'8485'
        lbr   x'ab23'
        br    x'ac23')",
            R"(       abcd :                            org   x'abcd'
       abcd : 68 83 84 85                scal  r3, x'8485'
       abd1 : 30 23                      lbr   x'ab23'
       abd3 : c0 ac 23                   br    x'ac23'
)");
}

void test_dis_cdp1802() {
    PREP_DIS(cdp1802::DisCdp1802);

    driver.setUpperHex(false);
    EQ("use-register", OK, driver.setOption("use-register", "on"));

    DIS8("cdp1804", 0xabcd,
            R"(      cpu   1804
      org   x'abcd'
      scal  r3, x'8485'
.. test.bin: error: Unknown instruction
..     abd1 : 68 0f
      lbr   x'ab23'
      br    x'ab23'
)",
            R"(       0 :                            cpu   1804
    abcd :                            org   x'abcd'
    abcd : 68 83 84 85                scal  r3, x'8485'
test.bin: error: Unknown instruction
    abd1 : 68 0f
    abd3 : c0 ab 23                   lbr   x'ab23'
    abd6 : 30 23                      br    x'ab23'
)",
            0x68, 0x83, 0x84, 0x85, 0x68, 0x0f, 0xc0, 0xab, 0x23, 0x30, 0x23);
}

void run_tests() {
    RUN_TEST(test_asm_cdp1802);
    RUN_TEST(test_dis_cdp1802);
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

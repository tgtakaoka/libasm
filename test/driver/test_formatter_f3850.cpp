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

#include "asm_f3850.h"
#include "dis_f3850.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_f3850() {
    PREP_ASM(f3850::AsmF3850, FairchildDirective);

    TestReader inc("data/da.inc");
    sources.add(inc);
    inc.add(R"(        rs    3                        ; RS allocates spaces
        da    H'1234', label1, H'9ABC' ; DA generates words
        dc    'a,',',, '/, ' , 0       ; DC can omit closing quote
        dc    c'a',',, c'bc''de', 0    ; DC can accept string
        dc    C'A', #', C'C'+h'80'     ; C'c' requires surrounding quotes
)");

    ASM("f3850",
            R"(        cpu   F3850
* comment line
        org   H'7BCD'
label1  equ   H'7BD0'
        clr
        lr    a, j
        ds    10      ; DS is instruction
        bp    label1
        include "data/da.inc"
)",
            R"(          0 :                            cpu   F3850
          0 :                    * comment line
       7BCD :                            org   H'7BCD'
       7BCD : =7BD0              label1  equ   H'7BD0'
       7BCD : 70                         clr
       7BCE : 49                         lr    a, j
       7BCF : 3A                         ds    10      ; DS is instruction
       7BD0 : 81 FF                      bp    label1
       7BD2 :                            include "data/da.inc"
(1)    7BD2 :                            rs    3                        ; RS allocates spaces
(1)    7BD5 : 12 34 7B D0 9A BC          da    H'1234', label1, H'9ABC' ; DA generates words
(1)    7BDB : 61 2C 2C 2F 2C 20          dc    'a,',',, '/, ' , 0       ; DC can omit closing quote
       7BE1 : 00
(1)    7BE2 : 61 2C 62 63 27 64          dc    c'a',',, c'bc''de', 0    ; DC can accept string
       7BE8 : 65 00
(1)    7BEA : 41 27 C3                   dc    C'A', #', C'C'+h'80'     ; C'c' requires surrounding quotes
)");
}

void test_dis_f3850() {
    PREP_DIS(f3850::DisF3850);

    driver.setUpperHex(false);

    DIS8("f3850", 0x7bcd,
            R"(      cpu   3850
      org   h'7bcd'
      clr
      lr    a, j
      bp    h'7bcf'
* test.bin: error: Unknown instruction
*     7bd1 : 2e
)",
            R"(       0 :                            cpu   3850
    7bcd :                            org   h'7bcd'
    7bcd : 70                         clr
    7bce : 49                         lr    a, j
    7bcf : 81 ff                      bp    h'7bcf'
test.bin: error: Unknown instruction
    7bd1 : 2e
)",
            0x70, 0x49, 0x81, 0xFF, 0x2E);
}

void run_tests() {
    RUN_TEST(test_asm_f3850);
    RUN_TEST(test_dis_f3850);
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

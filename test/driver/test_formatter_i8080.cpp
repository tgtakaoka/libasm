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

#include "asm_i8080.h"
#include "dis_i8080.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_i8080() {
    PREP_ASM(i8080::AsmI8080, IntelDirective);

    driver.setUpperHex(false);

    ASM("i8080",
            R"(        cpu   i8080
; comment line
        org   0abcdh
label1: equ   0ebech
        jpe   label1
        db    'z'-'a', 'A''B', 0
label2  dw    label2
)",
            R"(          0 :                            cpu   i8080
          0 :                    ; comment line
       abcd :                            org   0abcdh
       abcd : =ebec              label1: equ   0ebech
       abcd : ea ec eb                   jpe   label1
       abd0 : 19 41 27 42 00             db    'z'-'a', 'A''B', 0
       abd5 : d5 ab              label2  dw    label2
)");
}

void test_dis_i8080() {
    PREP_DIS(i8080::DisI8080);

    DIS8("i8080", 0xabcd,
            R"(      cpu   8080
      org   0ABCDH
      jpe   0ECEBH
; test.bin: error: Unknown instruction
;     ABD0 : DD
)",
            R"(       0 :                            cpu   8080
    ABCD :                            org   0ABCDH
    ABCD : EA EB EC                   jpe   0ECEBH
test.bin: error: Unknown instruction
    ABD0 : DD
)",
            0xea, 0xeb, 0xec, 0xdd);
}

void run_tests() {
    RUN_TEST(test_asm_i8080);
    RUN_TEST(test_dis_i8080);
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

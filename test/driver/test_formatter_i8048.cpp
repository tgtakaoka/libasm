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

#include "asm_i8048.h"
#include "dis_i8048.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_i8048() {
    PREP_ASM(i8048::AsmI8048, IntelDirective);

    driver.setUpperHex(false);

    ASM("i8039",
            R"(        cpu   i8039
; comment line
        org   0bcdh
data1:  equ   8ah
        orl   p1, #data1
)",
            R"(          0 :                            cpu   i8039
          0 :                    ; comment line
        bcd :                            org   0bcdh
        bcd : =8a                data1:  equ   8ah
        bcd : 89 8a                      orl   p1, #data1
)");
}

void test_dis_i8048() {
    PREP_DIS(i8048::DisI8048);

    driver.setUppercase(true);

    DIS8("i8039", 0xbcd,
            R"(      CPU   8039
      ORG   0BCDH
      ORL   P1, #8AH
; test.bin: error: Unknown instruction
;      BCF : 08
)",
            R"(       0 :                            CPU   8039
     BCD :                            ORG   0BCDH
     BCD : 89 8A                      ORL   P1, #8AH
test.bin: error: Unknown instruction
     BCF : 08
)",
            0x89, 0x8A, 0x08);
}

void run_tests() {
    RUN_TEST(test_asm_i8048);
    RUN_TEST(test_dis_i8048);
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

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

#include "asm_tms320.h"
#include "dis_tms320.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_tms320() {
    PREP_ASM(tms320::AsmTms320, IntelDirective);

    driver.setUpperHex(false);

    ASM("tms32010",
            R"(        .cpu  tms32010
* comment line
        .org  789h
        call  0fedh
        sacl  *+, 0, ar0
label:  .word label
        .byte 1,'''',3
        .string "A""B'C"
)",

            R"(          0 :                            .cpu  tms32010
          0 :                    * comment line
        789 :                            .org  789h
        789 : f800 0fed                  call  0fedh
        78b : 50a0                       sacl  *+, 0, ar0
        78c : 078c               label:  .word label
        78d : 0001 0027 0003             .byte 1,'''',3
        790 : 2241 2742 0043             .string "A""B'C"
)");
}

void test_dis_tms320() {
    PREP_DIS(tms320::DisTms320);

    driver.setUpperHex(false);

    DIS16("tms32010", 0x789,
            R"(      cpu   32010
      org   789h
      call  0fedh
      sacl  *+, 0, ar0
* test.bin: error: Unknown instruction
*      78c : 0086
)",
            R"(       0 :                            cpu   32010
     789 :                            org   789h
     789 : f800 0fed                  call  0fedh
     78b : 50a0                       sacl  *+, 0, ar0
test.bin: error: Unknown instruction
     78c : 0086
)",
            0xf800, 0x0fed, 0x50a0, 0x0086);
}

void run_tests() {
    RUN_TEST(test_asm_tms320);
    RUN_TEST(test_dis_tms320);
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

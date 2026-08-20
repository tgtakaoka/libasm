/*
 * Copyright 2026 Tadashi G. Takaoka
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

#include "asm_am29000.h"
#include "dis_am29000.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_am29000() {
    PREP_ASM(am29000::AsmAm29000, IntelDirective);

    ASM("AM29240",
            R"(        cpu     AM29240
; label definition and reference, data and a Program-Counter relative branch
        org     0x1000
L_DATA: equ     0x1234
        add     gr96, gr97, gr98
        const   gr96, L_DATA
L_HERE:
        jmp     L_HERE
        call    gr96, L_HERE
        multiply gr96, gr97, gr98
        .word   L_DATA
)",
            R"(          0 :                            cpu     AM29240
          0 :                    ; label definition and reference, data and a Program-Counter relative branch
       1000 :                            org     0x1000
       1000 : =1234              L_DATA: equ     0x1234
       1000 : 14606162                   add     gr96, gr97, gr98
       1004 : 03126034                   const   gr96, L_DATA
       1008 :                    L_HERE:
       1008 : A0000000                   jmp     L_HERE
       100C : A8FF60FF                   call    gr96, L_HERE
       1010 : E0606162                   multiply gr96, gr97, gr98
       1014 : 00001234                   .word   L_DATA
)");
}

void test_dis_am29000() {
    PREP_DIS(am29000::DisAm29000);

    DIS32("AM29240", 0x1000,
            R"(      cpu      am29240
      org      0x00001000
      add      gr96, gr97, gr98
      const    gr96, 0x1234
)",
            R"(       0 :                            cpu      am29240
    1000 :                            org      0x00001000
    1000 : 14606162                   add      gr96, gr97, gr98
    1004 : 03126034                   const    gr96, 0x1234
)",
            0x14606162, 0x03126034);
}

void run_tests() {
    RUN_TEST(test_asm_am29000);
    RUN_TEST(test_dis_am29000);
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

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

#include "asm_i960.h"
#include "dis_i960.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_i960() {
    PREP_ASM(i960::AsmI960, I960Directive);

    ASM("80960",
            R"(        .cpu    80960
; .equ takes the symbol name as an operand rather than as a line label
        .org    0x00100000
        .equ    L_DATA, 0x1234
        addi    g0, g1, g2
        lda     L_DATA, g6
L_HERE:
        b       L_HERE
        cmpibe  g0, g1, L_HERE
        ld      0x1234(g5)[g4*4], g6
        .word   L_DATA
)",
            R"(          0 :                            .cpu    80960
          0 :                    ; .equ takes the symbol name as an operand rather than as a line label
     100000 :                            .org    0x00100000
     100000 : =1234                      .equ    L_DATA, 0x1234
     100000 : 59944090                   addi    g0, g1, g2
     100004 : 8CB03000 00001234          lda     L_DATA, g6
     10000C :                    L_HERE:
     10000C : 08000000                   b       L_HERE
     100010 : 3A845FFC                   cmpibe  g0, g1, L_HERE
     100014 : 90B57D14 00001234          ld      0x1234(g5)[g4*4], g6
     10001C : 00001234                   .word   L_DATA
)");
}

void test_dis_i960() {
    PREP_DIS(i960::DisI960);

    DIS32("80960", 0x00100000,
            R"(      cpu      80960
      org      0x00100000
      addi     g0, g1, g2
      ld       0x00001234(g5)[g4*4], g6
)",
            R"(       0 :                            cpu      80960
  100000 :                            org      0x00100000
  100000 : 59944090                   addi     g0, g1, g2
  100004 : 90B57D14 00001234          ld       0x00001234(g5)[g4*4], g6
)",
            0x59944090, 0x90B57D14, 0x00001234);
}

void run_tests() {
    RUN_TEST(test_asm_i960);
    RUN_TEST(test_dis_i960);
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

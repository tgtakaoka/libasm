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

#include "asm_superh.h"
#include "dis_superh.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

// clang-format off

// One assembler listing test (not a CPU-variant test): it exercises the listing
// formatting -- comment line, directives with and without an emitted value, a
// label column, a one-word and a two-word (32-bit) instruction in the code
// column, and a symbol reference resolved in an operand.
void test_asm_listing() {
    PREP_ASM(superh::AsmSuperH, HitachiDirective);

    driver.setUpperHex(false);

    ASM("SH-2A",
            R"(        .cpu    SH-2A
; comment line
        .org    H'1000
const:  .equ    H'1234
start:  movi20  #H'7FFFF, r0
        mov.w   data, r1
        bra     start
        nop
data:   .data   const
)",
            R"(          0 :                            .cpu    SH-2A
          0 :                    ; comment line
       1000 :                            .org    H'1000
       1000 : =1234              const:  .equ    H'1234
       1000 : 0070 ffff          start:  movi20  #H'7FFFF, r0
       1004 : 9101                       mov.w   data, r1
       1006 : affb                       bra     start
       1008 : 0009                       nop
       100a : 1234               data:   .data   const
)");
}

// One disassembler listing test: header pseudos, a one-word and a two-word
// (32-bit) instruction, and PC-relative operands rendered as the bare target.
void test_dis_listing() {
    PREP_DIS(superh::DisSuperH);

    driver.setUppercase(true);

    DIS16("SH-2A", 0x1000,
            R"(      CPU    SH-2A
      OPTION "fpu", "true"
      ORG    H'00001000
      MOVRT  R0
      MOVI20 #H'7FFFF, R0
      MOV.W  H'0000100C, R1
      BRA    H'00001000
      NOP
)",
            R"(       0 :                            CPU    SH-2A
       0 :                            OPTION "fpu", "true"
    1000 :                            ORG    H'00001000
    1000 : 0039                       MOVRT  R0
    1002 : 0070 FFFF                  MOVI20 #H'7FFFF, R0
    1006 : 9101                       MOV.W  H'0000100C, R1
    1008 : AFFA                       BRA    H'00001000
    100A : 0009                       NOP
)",
            0x0039, 0x0070, 0xFFFF, 0x9101, 0xAFFA, 0x0009);
}

// clang-format on

void run_tests() {
    RUN_TEST(test_asm_listing);
    RUN_TEST(test_dis_listing);
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

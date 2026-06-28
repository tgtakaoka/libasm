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

#include "asm_cp1600.h"
#include "dis_cp1600.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_cp1600() {
    PREP_ASM(cp1600::AsmCp1600, MotorolaDirective);

    ASM("CP1600",
            R"(        cpu     CP1600
; label definition + reference, varying length, SDBD-prefixed
        org     X'0100'
L_DATA: equ     X'1234'
        SWAP    R0              ; 1-word
        MVI     L_DATA, R0      ; 2-word: direct (label reference)
        ADDI    X'55', R1         ; 2-word: immediate
L_HERE:                         ; label definition
        SDBD                    ; 1-word: SDBD prefix
        MVII    L_DATA, R2      ; SDBD-prefixed: 3 words (opcode + low + high)
        J       L_HERE          ; 3-word: jump to label
        B       L_HERE          ; 2-word: PC-relative branch (backward)
)",
            R"(          0 :                            cpu     CP1600
          0 :                    ; label definition + reference, varying length, SDBD-prefixed
        100 :                            org     X'0100'
        100 : =1234              L_DATA: equ     X'1234'
        100 : 0040                       SWAP    R0              ; 1-word
        101 : 0280 1234                  MVI     L_DATA, R0      ; 2-word: direct (label reference)
        103 : 02F9 0055                  ADDI    X'55', R1         ; 2-word: immediate
        105 :                    L_HERE:                         ; label definition
        105 : 0001                       SDBD                    ; 1-word: SDBD prefix
        106 : 02BA 0034 0012             MVII    L_DATA, R2      ; SDBD-prefixed: 3 words (opcode + low + high)
        109 : 0004 0300 0105             J       L_HERE          ; 3-word: jump to label
        10C : 0220 0008                  B       L_HERE          ; 2-word: PC-relative branch (backward)
)");
}

void test_dis_cp1600() {
    PREP_DIS(cp1600::DisCp1600);

    driver.setUpperHex(false);
    driver.setUppercase(false);

    // Covers label/address reference, varying instruction length (1/2/3 word),
    // and SDBD-prefixed continuation.  SDBD owns the whole prefixed instruction
    // for the decode loop, but lists as its own 1-word line with the follower at
    // the next address (Insn::continueOffset splits the segments): a 3-word
    // immediate follower (MVII) and a 1-word indirect follower (MVI@).
    DIS16("cp1600", 0x0100,
            R"(      cpu   cp1600
      org   x'0100'
      swap  r0
      mvi   x'1234', r0
      addi  x'55', r1
      j     x'0100'
      b     x'0100'
      sdbd
      mvii  x'1234', r0
      sdbd
      mvii  x'5678', r1
      sdbd
      mvi@  r1, r1
)",
            R"(       0 :                            cpu   cp1600
     100 :                            org   x'0100'
     100 : 0040                       swap  r0
     101 : 0280 1234                  mvi   x'1234', r0
     103 : 02f9 0055                  addi  x'55', r1
     105 : 0004 0300 0100             j     x'0100'
     108 : 0220 0009                  b     x'0100'
     10a : 0001                       sdbd
     10b : 02b8 0034 0012             mvii  x'1234', r0
     10e : 0001                       sdbd
     10f : 02b9 0078 0056             mvii  x'5678', r1
     112 : 0001                       sdbd
     113 : 0289                       mvi@  r1, r1
)",
            // 1-word
            0x0040,
            // 2-word direct
            0x0280, 0x1234,
            // 2-word immediate
            0x02F9, 0x0055,
            // 3-word jump back to X'0100'
            0x0004, 0x0300, 0x0100,
            // 2-word backward branch to X'0100'
            // disp = (PC_after=0x10A) - 0x100 - 1 = 9
            0x0220, 0x0009,
            // SDBD + MVII X'1234', R0: 1-word prefix line + 3-word follower
            0x0001, 0x02B8, 0x0034, 0x0012,
            // SDBD + MVII X'5678', R1: another 3-word immediate follower
            0x0001, 0x02B9, 0x0078, 0x0056,
            // SDBD + MVI@ R1, R1: 1-word indirect follower
            0x0001, 0x0289);
}

void run_tests() {
    RUN_TEST(test_asm_cp1600);
    RUN_TEST(test_dis_cp1600);
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

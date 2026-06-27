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

#include "asm_h16.h"
#include "dis_h16.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_h16() {
    PREP_ASM(h16::AsmH16, HitachiDirective);

    options.set("smart-branch", "true");

    // Covers smart-branch size selection (bra/beq), a label/.equ reference, a
    // 32-bit-address listing, a multi-byte EA (disp32), and the HitachiDirective
    // .assign / .data directives.
    ASM("H16",
            R"(        .cpu    H16
        .org    $12345678
L_DATA: .equ    $123456
        clr.b   @r1
        mov:g   @($12345678,r0), r1
        add:q   #$10, r2
L_HERE:
        bra     L_HERE
        beq     L_DATA
        .assign var1, $abcc
        .data   var1
        .assign var1, $1234
        .data   var1
)",
            R"(          0 :                            .cpu    H16
   12345678 :                            .org    $12345678
   12345678 : =123456            L_DATA: .equ    $123456
   12345678 : 14 01                      clr.b   @r1
   1234567A : 0D 30 12 34 56 78          mov:g   @($12345678,r0), r1
   12345680 : 41
   12345681 : 11 10 42                   add:q   #$10, r2
   12345684 :                    L_HERE:
   12345684 : 98 FE                      bra     L_HERE
   12345686 : A2 ED DD DD CB             beq     L_DATA
   1234568B : =ABCC                      .assign var1, $abcc
   1234568B : AB CC                      .data   var1
   1234568D : =1234                      .assign var1, $1234
   1234568D : 12 34                      .data   var1
)");
}

void test_dis_h16() {
    PREP_DIS(h16::DisH16);

    driver.setUpperHex(false);
    driver.setUppercase(false);

    // Covers a label/address reference, a multi-byte EA (disp32) instruction
    // whose bytes overflow onto a continuation line, a register-list operand
    // (LDM), a PC-relative branch resolved to an absolute address, and the
    // longest H16 instruction (24 bytes = MaxCode): a bit-field op with two
    // maximal EAs (each a bank-prefixed register double-indirect), exercising
    // the listing formatter's byte-column overflow across four lines.
    DIS8("H16", 0x2000,
            R"(      cpu      h16
      org      $00002000
      mov:G.W  @($12345678, r0), r1
      ldm.W    @r3, r0-r2, r5
      bra.W    $00002000
      bfext    r1, r2, @($12345678,@($12345678, cr0)), @($12345678,@($12345678, pr1))
)",
            R"(       0 :                            cpu      h16
    2000 :                            org      $00002000
    2000 : 0d 30 12 34 56 78          mov:G.W  @($12345678, r0), r1
    2006 : 41
    2007 : 75 03 00 27                ldm.W    @r3, r0-r2, r5
    200b : 99 ff f2                   bra.W    $00002000
    200e : d4 12 70 7e f0 12          bfext    r1, r2, @($12345678,@($12345678, cr0)), @($12345678,@($12345678, pr1))
    2014 : 34 56 78 12 34 56
    201a : 78 74 7e f1 12 34
    2020 : 56 78 12 34 56 78
)",
            0x0D, 0x30, 0x12, 0x34, 0x56, 0x78, 0x41,  // mov:G.W @(disp32,r0),r1
            0x75, 0x03, 0x00, 0x27,                    // ldm.W @r3, r0-r2, r5
            0x99, 0xFF, 0xF2,                          // bra.W $2000
            0xD4, 0x12, 0x70, 0x7E, 0xF0, 0x12, 0x34, 0x56,  // bfext r1,r2,
            0x78, 0x12, 0x34, 0x56, 0x78,                    //   @(d32,@(d32,cr0)),
            0x74, 0x7E, 0xF1, 0x12, 0x34, 0x56, 0x78,        //   @(d32,@(d32,pr1))
            0x12, 0x34, 0x56, 0x78);
}

void run_tests() {
    RUN_TEST(test_asm_h16);
    RUN_TEST(test_dis_h16);
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

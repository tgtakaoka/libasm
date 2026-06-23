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

#include "asm_h8500.h"
#include "dis_h8500.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

// Minimum mode: a flat 64KB (16-bit) address space.
void test_asm_h8500() {
    PREP_ASM(h8500::AsmH8500, H8500Directive);

    driver.setUpperHex(false);

    ASM("H8/500",
            R"(        cpu   H8/500
; comment line
        org   H'0100
label1: equ   H'0200
        nop
        mov:g r0, r1
        jmp   @label1
label2: nop
)",
            R"(          0 :                            cpu   H8/500
          0 :                    ; comment line
        100 :                            org   H'0100
        100 : =200               label1: equ   H'0200
        100 : 00                         nop
        101 : a8 81                      mov:g r0, r1
        103 : 10 02 00                   jmp   @label1
        106 : 00                 label2: nop
)");
}

// Maximum mode on H8/500: the chip's full 16MB (24-bit) physical space, so
// addresses reach beyond the 64KB minimum mode (and page jumps are available).
void test_asm_h8500_max() {
    PREP_ASM(h8500::AsmH8500, H8500Directive);

    driver.setUpperHex(false);

    ASM("H8/500",
            R"(        cpu     H8/500
        .maxmode on
        org     H'010000
        nop
        mov:g   r0, r1
        pjmp    @H'020304
)",
            R"(          0 :                            cpu     H8/500
          0 :                            .maxmode on
      10000 :                            org     H'010000
      10000 : 00                         nop
      10001 : a8 81                      mov:g   r0, r1
      10003 : 13 02 03 04                pjmp    @H'020304
)");
}

// Minimum mode: 64KB (16-bit) address space; the org prints 4 hex digits.
void test_dis_h8500() {
    PREP_DIS(h8500::DisH8500);

    driver.setUppercase(true);

    DIS8("H8/500", 0x0100,
            R"(      CPU      H8/500
      ORG      H'0100
      NOP
      MOV:G.W  R0, R1
      JMP      @H'0200:16
)",
            R"(       0 :                            CPU      H8/500
     100 :                            ORG      H'0100
     100 : 00                         NOP
     101 : A8 81                      MOV:G.W  R0, R1
     103 : 10 02 00                   JMP      @H'0200:16
)",
            0x00, 0xA8, 0x81, 0x10, 0x02, 0x00);
}

// Maximum mode on H8/500: 16MB (24-bit) address space; the org prints 6 digits.
void test_dis_h8500_max() {
    PREP_DIS(h8500::DisH8500);

    driver.setUppercase(true);
    driver.setOption("maximum-mode", "on");

    DIS8("H8/500", 0x0100,
            R"(      CPU      H8/500
      ORG      H'000100
      NOP
      MOV:G.W  R0, R1
      PJMP     @H'020304:24
)",
            R"(       0 :                            CPU      H8/500
     100 :                            ORG      H'000100
     100 : 00                         NOP
     101 : A8 81                      MOV:G.W  R0, R1
     103 : 13 02 03 04                PJMP     @H'020304:24
)",
            0x00, 0xA8, 0x81, 0x13, 0x02, 0x03, 0x04);
}

// Minimum mode: 64KB (16-bit) address space.
void test_asm_h8520() {
    PREP_ASM(h8500::AsmH8500, H8500Directive);

    driver.setUpperHex(false);

    ASM("H8/520",
            R"(        cpu   H8/520
        org   H'0100
        nop
        mov:g r0, r1
)",
            R"(          0 :                            cpu   H8/520
        100 :                            org   H'0100
        100 : 00                         nop
        101 : a8 81                      mov:g r0, r1
)");
}

// HitachiDirective `.assign` defines a re-assignable symbol (H8/500 uses `.set`
// for page registers, so symbol assignment is spelled `.assign`).
void test_asm_h8500_assign() {
    PREP_ASM(h8500::AsmH8500, H8500Directive);

    driver.setUpperHex(false);

    ASM("H8/500",
            R"(        cpu     H8/500
        .assign var1, H'abcc
        .data   var1
        .assign var1, H'1234
        .data   var1
)",
            R"(          0 :                            cpu     H8/500
          0 : =abcc                      .assign var1, H'abcc
          0 : ab cc                      .data   var1
          2 : =1234                      .assign var1, H'1234
          2 : 12 34                      .data   var1
)");
}

// Maximum mode on H8/530: the pin-limited chip reaches 1MB (20-bit), so the org
// prints 5 hex digits (between the 64KB minimum and the H8/500 16MB maximum).
void test_dis_h8530_max() {
    PREP_DIS(h8500::DisH8500);

    driver.setUppercase(true);
    driver.setOption("maximum-mode", "on");

    DIS8("H8/530", 0x0100,
            R"(      CPU      H8/530
      ORG      H'00100
      NOP
      MOV:G.W  R0, R1
      PJMP     @H'020304:24
)",
            R"(       0 :                            CPU      H8/530
     100 :                            ORG      H'00100
     100 : 00                         NOP
     101 : A8 81                      MOV:G.W  R0, R1
     103 : 13 02 03 04                PJMP     @H'020304:24
)",
            0x00, 0xA8, 0x81, 0x13, 0x02, 0x03, 0x04);
}

void run_tests() {
    RUN_TEST(test_asm_h8500);
    RUN_TEST(test_asm_h8500_max);
    RUN_TEST(test_dis_h8500);
    RUN_TEST(test_dis_h8500_max);
    RUN_TEST(test_asm_h8520);
    RUN_TEST(test_asm_h8500_assign);
    RUN_TEST(test_dis_h8530_max);
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

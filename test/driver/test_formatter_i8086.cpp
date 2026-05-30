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

#include "asm_i8086.h"
#include "dis_i8086.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_i8086() {
    PREP_ASM(i8086::AsmI8086, IntelDirective);

    TestReader inc("data/db.inc");
    sources.add(inc);
    inc.add(R"(        ds     3                    ; DS allocates spaces
        db     1                    ; DB generates bytes
        dd     12345678H, 9abcdef0H ; DD doesn't care alignment
        dw     1234H, 5678H, 9abcH  ; DW doesn't care alignment
        db     'a,', 'bc''de', 0
        db     'A', '''', 'C'+80H, 'a''c'
)");

    driver.setUpperHex(false);

    ASM("i8086",
        R"(        cpu    i8086
; comment line
        org    0bcdefh
        test   word ptr [bp+di+0feffh], 0bbaah
        include 'data/db.inc'
        jmp    label2
        jmp    label3
label2: equ    0bce00h
label3: equ    0bde00h
)",
        R"(          0 :                            cpu    i8086
          0 :                    ; comment line
      bcdef :                            org    0bcdefh
      bcdef : f7 83 ff fe aa bb          test   word ptr [bp+di+0feffh], 0bbaah
      bcdf5 :                            include 'data/db.inc'
(1)   bcdf5 :                            ds     3                    ; DS allocates spaces
(1)   bcdf8 : 01                         db     1                    ; DB generates bytes
(1)   bcdf9 : 78 56 34 12 f0 de          dd     12345678H, 9abcdef0H ; DD doesn't care alignment
      bcdff : bc 9a
(1)   bce01 : 34 12 78 56 bc 9a          dw     1234H, 5678H, 9abcH  ; DW doesn't care alignment
(1)   bce07 : 61 2c 62 63 27 64          db     'a,', 'bc''de', 0
      bce0d : 65 00
(1)   bce0f : 41 27 c3 61 27 63          db     'A', '''', 'C'+80H, 'a''c'
      bce15 : eb e9                      jmp    label2
      bce17 : e9 e6 0f                   jmp    label3
      bce1a : =bce00             label2: equ    0bce00h
      bce1a : =bde00             label3: equ    0bde00h
)");
}

void test_dis_i8086() {
    PREP_DIS(i8086::DisI8086);

    driver.setUpperHex(false);

    DIS8("i8086", 0xbcdef,
         R"(      cpu     8086
      org     0bcdefh
      test    word [bp+di+0feffh], 0bbaah
; test.bin: error: Unknown instruction
;    bcdf5 : d1 f7
)",
         R"(       0 :                            cpu     8086
   bcdef :                            org     0bcdefh
   bcdef : f7 83 ff fe aa bb          test    word [bp+di+0feffh], 0bbaah
test.bin: error: Unknown instruction
   bcdf5 : d1 f7
)",
            0xf7, 0x83, 0xff, 0xfe, 0xaa, 0xbb, 0xd1, 0xf7);
}

void test_asm_i80486() {
    PREP_ASM(i8086::AsmI8086, IntelDirective);

    driver.setUpperHex(false);

    ASM("i80486",
        R"(        cpu    i80486
        org    01000h
        mov    ax, bx
        movzx  eax, cl
        jmp    label2
        jmp    label3
label2: equ    01080h
label3: equ    02000h
)",
        R"(          0 :                            cpu    i80486
       1000 :                            org    01000h
       1000 : 66 89 d8                   mov    ax, bx
       1003 : 0f b6 c1                   movzx  eax, cl
       1006 : eb 78                      jmp    label2
       1008 : e9 f3 0f 00 00             jmp    label3
       100d : =1080              label2: equ    01080h
       100d : =2000              label3: equ    02000h
)");
}

void test_dis_i80486() {
    PREP_DIS(i8086::DisI8086);

    driver.setUpperHex(false);

    DIS8("i80486", 0x1000,
         R"(      cpu     80486
      org     00001000h
      bswap   eax
      xadd    al, bl
      cmpxchg [eax], cl
      invd
      wbinvd
      invlpg  [edi]
)",
         R"(       0 :                            cpu     80486
    1000 :                            org     00001000h
    1000 : 0f c8                      bswap   eax
    1002 : 0f c0 d8                   xadd    al, bl
    1005 : 0f b0 08                   cmpxchg [eax], cl
    1008 : 0f 08                      invd
    100a : 0f 09                      wbinvd
    100c : 0f 01 3f                   invlpg  [edi]
)",
            0x0f, 0xc8,
            0x0f, 0xc0, 0xd8,
            0x0f, 0xb0, 0x08,
            0x0f, 0x08,
            0x0f, 0x09,
            0x0f, 0x01, 0x3f);
}

void run_tests() {
    RUN_TEST(test_asm_i8086);
    RUN_TEST(test_dis_i8086);
    RUN_TEST(test_asm_i80486);
    RUN_TEST(test_dis_i80486);
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

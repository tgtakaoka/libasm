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

#include "asm_tlcs900.h"
#include "dis_tlcs900.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_tlcs900l() {
    PREP_ASM(tlcs900::AsmTlcs900, IntelDirective);

    driver.setUpperHex(false);

    ASM("tlcs900l",
            R"(        cpu     tlcs900l
; comment line
        org     0bcdeh
        ld      a, (34h)
        ld      a, (1234h)
        ldx     (1234h), 56h
        bit     3, (xix+20h)
        ldc     dmas0, xwa
)",
            R"(          0 :                            cpu     tlcs900l
          0 :                    ; comment line
       bcde :                            org     0bcdeh
       bcde : c0 34 21                   ld      a, (34h)
       bce1 : c1 34 12 21                ld      a, (1234h)
       bce5 : f7 00 34 12 56 00          ldx     (1234h), 56h
       bceb : bc 20 cb                   bit     3, (xix+20h)
       bcee : e8 2e 00                   ldc     dmas0, xwa
)");
}

void test_dis_tlcs900l() {
    PREP_DIS(tlcs900::DisTlcs900);

    driver.setUpperHex(false);
    driver.setUppercase(false);

    DIS8("tlcs900l", 0xbcde,
            R"(      cpu    tlcs900l
      org    0bcdeh
      ld     a, (34h)
      ld     a, (1234h)
      ldx    (1234h), 0056h
      bit    3, (xix+32)
      ldc    dmas0, xwa
)",
            R"(       0 :                            cpu    tlcs900l
    bcde :                            org    0bcdeh
    bcde : c0 34 21                   ld     a, (34h)
    bce1 : c1 34 12 21                ld     a, (1234h)
    bce5 : f7 00 34 12 56 00          ldx    (1234h), 0056h
    bceb : bc 20 cb                   bit    3, (xix+32)
    bcee : e8 2e 00                   ldc    dmas0, xwa
)",
            0xc0, 0x34, 0x21, 0xc1, 0x34, 0x12, 0x21, 0xf7, 0x00, 0x34, 0x12, 0x56, 0x00, 0xbc,
            0x20, 0xcb, 0xe8, 0x2e, 0x00);
}

void test_asm_tlcs900h() {
    PREP_ASM(tlcs900::AsmTlcs900, IntelDirective);

    driver.setUpperHex(false);

    ASM("tlcs900h",
            R"(        cpu     tlcs900h
; comment line
        org     0bcdefh
        ld      a, (34h)
        ld      a, (1234h)
        ld      a, (123456h)
        ldx     (1234h), 56h
        bit     3, (xix+20h)
        ldc     dmas0, xwa
        ld      (0123456h), 012345678h
)",
            R"(          0 :                            cpu     tlcs900h
          0 :                    ; comment line
      bcdef :                            org     0bcdefh
      bcdef : c0 34 21                   ld      a, (34h)
      bcdf2 : c1 34 12 21                ld      a, (1234h)
      bcdf6 : c2 56 34 12 21             ld      a, (123456h)
      bcdfb : f7 00 34 12 56 00          ldx     (1234h), 56h
      bce01 : bc 20 cb                   bit     3, (xix+20h)
      bce04 : e8 2e 00                   ldc     dmas0, xwa
      bce07 : f2 56 34 12 02 78          ld      (0123456h), 012345678h
      bce0d : 56 34 12
)");
}

void test_dis_tlcs900h() {
    PREP_DIS(tlcs900::DisTlcs900);

    driver.setUpperHex(false);
    driver.setUppercase(false);

    DIS8("tlcs900h", 0xbcdef,
            R"(      cpu    tlcs900h
      org    0bcdefh
      ld     a, (34h)
      ld     a, (1234h)
      ld     a, (123456h)
      ldx    (1234h), 0056h
      bit    3, (xix+32)
      ldc    dmas0, xwa
      ld     (123456h), 12345678h
)",
            R"(       0 :                            cpu    tlcs900h
   bcdef :                            org    0bcdefh
   bcdef : c0 34 21                   ld     a, (34h)
   bcdf2 : c1 34 12 21                ld     a, (1234h)
   bcdf6 : c2 56 34 12 21             ld     a, (123456h)
   bcdfb : f7 00 34 12 56 00          ldx    (1234h), 0056h
   bce01 : bc 20 cb                   bit    3, (xix+32)
   bce04 : e8 2e 00                   ldc    dmas0, xwa
   bce07 : f2 56 34 12 02 78          ld     (123456h), 12345678h
   bce0d : 56 34 12
)",
            0xc0, 0x34, 0x21, 0xc1, 0x34, 0x12, 0x21, 0xc2, 0x56, 0x34, 0x12, 0x21, 0xf7, 0x00,
            0x34, 0x12, 0x56, 0x00, 0xbc, 0x20, 0xcb, 0xe8, 0x2e, 0x00, 0xf2, 0x56, 0x34, 0x12,
            0x02, 0x78, 0x56, 0x34, 0x12);
}

void run_tests() {
    RUN_TEST(test_asm_tlcs900l);
    RUN_TEST(test_dis_tlcs900l);
    RUN_TEST(test_asm_tlcs900h);
    RUN_TEST(test_dis_tlcs900h);
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

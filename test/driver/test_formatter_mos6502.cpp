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

#include "asm_mos6502.h"
#include "dis_mos6502.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_mos6502() {
    PREP_ASM(mos6502::AsmMos6502, MostekDirective);

    driver.setUpperHex(false);

    ASM("mos6502",
            R"(        cpu   mos6502
; comment line
label1 = $f1f2         define label1
        *= $abcd       set location
        sbc   label1   use label1
label2 = 2             define label2
label3 := 3            define label3
        lda   #label2  use label2
        ldy   #label3 ;use label3
label4: bne   label4   define label4 and use it
        .byte 'abc'    define constants
        fcb   'xyz'    motorola style
)",
            R"(          0 :                            cpu   mos6502
          0 :                    ; comment line
          0 : =f1f2              label1 = $f1f2         define label1
       abcd :                            *= $abcd       set location
       abcd : ed f2 f1                   sbc   label1   use label1
       abd0 : =2                 label2 = 2             define label2
       abd0 : =3                 label3 := 3            define label3
       abd0 : a9 02                      lda   #label2  use label2
       abd2 : a0 03                      ldy   #label3 ;use label3
       abd4 : d0 fe              label4: bne   label4   define label4 and use it
       abd6 : 61 62 63                   .byte 'abc'    define constants
       abd9 : 78 79 7a                   fcb   'xyz'    motorola style
)");
}

void test_asm_w65816() {
    PREP_ASM(mos6502::AsmMos6502, MostekDirective);

    driver.setUpperHex(false);
    options.set("smart-branch", "on");

    ASM("w65c816",
            R"(        cpu   w65c816
; comment line
label1 = $f2f1f0      define label1
        *=$abcdef     set location
        sbc   label1  use label1
        longa on      16-bit accumulator
        adc   #$1234  16-bit immediate
        brl   label2  forward reference
        bra   label3  forward reference
label2 = $abce00      define label2
label3 = $abd000      define label3
)",
            R"(          0 :                            cpu   w65c816
          0 :                    ; comment line
          0 : =f2f1f0            label1 = $f2f1f0      define label1
     abcdef :                            *=$abcdef     set location
     abcdef : ef f0 f1 f2                sbc   label1  use label1
     abcdf3 :                            longa on      16-bit accumulator
     abcdf3 : 69 34 12                   adc   #$1234  16-bit immediate
     abcdf6 : 80 08                      brl   label2  forward reference
     abcdf8 : 82 05 02                   bra   label3  forward reference
     abcdfb : =abce00            label2 = $abce00      define label2
     abcdfb : =abd000            label3 = $abd000      define label3
)");
}

void test_dis_mos6502() {
    PREP_DIS(mos6502::DisMos6502);

    driver.setUpperHex(false);

    DIS8("mos6502", 0xabcd,
            R"(      cpu   6502
      org   $abcd
      sbc   $f1f2
; test.bin: error: Unknown instruction
;     abd0 : 80
)",
            R"(       0 :                            cpu   6502
    abcd :                            org   $abcd
    abcd : ed f2 f1                   sbc   $f1f2
test.bin: error: Unknown instruction
    abd0 : 80
)",
            0xed, 0xf2, 0xf1, 0x80);
}

void test_dis_w65816() {
    PREP_DIS(mos6502::DisMos6502);

    driver.setUpperHex(true);
    driver.setUppercase(true);
    driver.setOption("longa", "on");

    DIS8("w65c816", 0xabcdef,
            R"(      CPU   65816
      ORG   $ABCDEF
      SBC   $F2F1F0
      ADC   #$1234
; test.bin: error: Overwrap segment/bank/page boundary: "$AC4DF8"
;   ABCDF6 : 82 FF 7F                   BRL   $AC4DF8
)",
            R"(       0 :                            CPU   65816
  ABCDEF :                            ORG   $ABCDEF
  ABCDEF : EF F0 F1 F2                SBC   $F2F1F0
  ABCDF3 : 69 34 12                   ADC   #$1234
test.bin: error: Overwrap segment/bank/page boundary: "$AC4DF8"
  ABCDF6 : 82 FF 7F                   BRL   $AC4DF8
)",
            0xef, 0xf0, 0xf1, 0xf2, 0x69, 0x34, 0x12, 0x82, 0xFF, 0x7F);
}

void run_tests() {
    RUN_TEST(test_asm_mos6502);
    RUN_TEST(test_asm_w65816);
    RUN_TEST(test_dis_mos6502);
    RUN_TEST(test_dis_w65816);
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

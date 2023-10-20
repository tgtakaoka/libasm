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

    listing.setUpperHex(false);
    driver.internSymbol(0xf1f2, "label1");

    ASM("mos6502",
            "        cpu   mos6502\n"
            "; comment line\n"
            "        *= $abcd\n"
            "        sbc   label1\n"
            "label2 = 2\n"
            "label3 := 3\n"
            "        lda   #label2\n"
            "        ldy   #label3\n"
            "label4: bne   label4\n"
            "        .byte 'abc'\n"
            "        fcb   'xyz'\n",
            "          0 :                            cpu   mos6502\n"
            "          0 :                    ; comment line\n"
            "       abcd :                            *= $abcd\n"
            "       abcd : ed f2 f1                   sbc   label1\n"
            "       abd0 : =2                 label2 = 2\n"
            "       abd0 : =3                 label3 := 3\n"
            "       abd0 : a9 02                      lda   #label2\n"
            "       abd2 : a0 03                      ldy   #label3\n"
            "       abd4 : d0 fe              label4: bne   label4\n"
            "       abd6 : 61 62 63                   .byte 'abc'\n"
            "       abd9 : 78 79 7a                   fcb   'xyz'\n");
}

void test_asm_w65816() {
    PREP_ASM(mos6502::AsmMos6502, MostekDirective);

    listing.setUpperHex(false);
    driver.internSymbol(0xf2f1f0, "label1");

    ASM("w65c816",
            "        cpu   w65c816\n"
            "; comment line\n"
            "        *=$abcdef\n"
            "        sbc   label1\n"
            "        longa on\n"
            "        adc   #$1234\n",
            "          0 :                            cpu   w65c816\n"
            "          0 :                    ; comment line\n"
            "     abcdef :                            *=$abcdef\n"
            "     abcdef : ef f0 f1 f2                sbc   label1\n"
            "     abcdf3 :                            longa on\n"
            "     abcdf3 : 69 34 12                   adc   #$1234\n");
}

void test_dis_mos6502() {
    PREP_DIS(mos6502::DisMos6502);

    formatter.setUpperHex(false);

    DIS8("mos6502", 0xabcd,
            "      cpu   mos6502\n"
            "      org   $abcd\n"
            "      sbc   $f1f2\n"
            "; test.bin: $abd0: error: Unknown instruction\n"
            ";     abd0 : 80\n",
            "       0 :                            cpu   mos6502\n"
            "    abcd :                            org   $abcd\n"
            "    abcd : ed f2 f1                   sbc   $f1f2\n"
            "test.bin: $abd0: error: Unknown instruction\n"
            "    abd0 : 80\n",
            0xed, 0xf2, 0xf1, 0x80);
}

void test_dis_w65816() {
    PREP_DIS(mos6502::DisMos6502);

    formatter.setUppercase(true);
    TRUE("setcpu", disassembler.setCpu("w65c816"));
    EQ("longa", OK, disassembler.setOption("longa", "on"));

    DIS8("w65c816", 0xabcdef,
            "      CPU   W65C816\n"
            "      ORG   $ABCDEF\n"
            "      SBC   $F2F1F0\n"
            "      ADC   #$1234\n"
            "; test.bin: $ABCDF6: error: Operand too far\n"
            ";   ABCDF6 : 82 FF 7F\n",
            "       0 :                            CPU   W65C816\n"
            "  ABCDEF :                            ORG   $ABCDEF\n"
            "  ABCDEF : EF F0 F1 F2                SBC   $F2F1F0\n"
            "  ABCDF3 : 69 34 12                   ADC   #$1234\n"
            "test.bin: $ABCDF6: error: Operand too far\n"
            "  ABCDF6 : 82 FF 7F                   BRL   $AC4DF8\n",
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

/*
 * Copyright 2025 Tadashi G. Takaoka
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

#include "asm_mc68hc12.h"
#include "dis_mc68hc12.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_mc68hc12() {
    PREP_ASM(mc68hc12::AsmMc68HC12, MotorolaDirective);

    driver.setUpperHex(false);

    ASM("mc68hc12",
            R"(        cpu   mc68hc12
* comment line
        org   $abcd    set location
label1: equ   $f1f2    define label
        subb  label1   use label
        jmp   label1   ; use label
)",
            R"(          0 :                            cpu   mc68hc12
          0 :                    * comment line
       abcd :                            org   $abcd    set location
       abcd : =f1f2              label1: equ   $f1f2    define label
       abcd : f0 f1 f2                   subb  label1   use label
       abd0 : 06 f1 f2                   jmp   label1   ; use label
)");
}

void test_asm_mc68hc12_mmc() {
    PREP_ASM(mc68hc12::AsmMc68HC12, MotorolaDirective);

    driver.setUpperHex(false);

    BinMemory image;
    ASM_MEMORY("mc68hc12",
            R"(        cpu    mc68hc12
        option mmc, on
        org    $0c0000
far0:   jmp    far0
        call   far1
        org    $0c4000
far1:   rtc
        org    $3fc000
        ppage  $30
        dpage  $20
        jsr    far0
        ldaa   $320123
)",
            R"(          0 :                            cpu    mc68hc12
          0 :                            option mmc, on
      c0000 :                            org    $0c0000
      c0000 : 06 80 00           far0:   jmp    far0
      c0003 : 4a 80 00 31                call   far1
      c4000 :                            org    $0c4000
      c4000 : 0a                 far1:   rtc
     3fc000 :                            org    $3fc000
     3fc000 :                            ppage  $30
     3fc000 :                            dpage  $20
     3fc000 : 16 80 00                   jsr    far0
     3fc003 : b6 71 23                   ldaa   $320123
)",
            image);
    EQ("page $30", 0x06, image.readByte(0x0C0000));
    EQ("page $31", 0x0A, image.readByte(0x0C4000));
    EQ("fixed", 0x16, image.readByte(0x3FC000));
    EQ("dpage", 0xB6, image.readByte(0x3FC003));
}

void test_asm_mc68hcs12_mmc() {
    PREP_ASM(mc68hc12::AsmMc68HC12, MotorolaDirective);

    driver.setUpperHex(false);

    BinMemory image;
    ASM_MEMORY("mc68hcs12",
            R"(        cpu    mc68hcs12
        option mmc, on
        org    $0100
        nop
        org    $0c0000
far0:   jmp    far0
        org    $0fc000
        ppage  $30
        jsr    far0
)",
            R"(          0 :                            cpu    mc68hcs12
          0 :                            option mmc, on
        100 :                            org    $0100
        100 : a7                         nop
      c0000 :                            org    $0c0000
      c0000 : 06 80 00           far0:   jmp    far0
      fc000 :                            org    $0fc000
      fc000 :                            ppage  $30
      fc000 : 16 80 00                   jsr    far0
)",
            image);
    EQ("low", 0xA7, image.readByte(0x000100));
    EQ("page $30", 0x06, image.readByte(0x0C0000));
    EQ("fixed", 0x16, image.readByte(0x0FC000));
}

void test_asm_mc68hcs12x_mmc() {
    PREP_ASM(mc68hc12::AsmMc68HC12, MotorolaDirective);

    driver.setUpperHex(false);

    BinMemory image;
    ASM_MEMORY("mc68hcs12x",
            R"(        cpu    mc68hcs12x
        option mmc, on
        org    $4c0000
far0:   jmp    far0
        org    $7fc000
        ppage  $30
        rpage  $fd
        jsr    far0
        ldaa   $0fd234
)",
            R"(          0 :                            cpu    mc68hcs12x
          0 :                            option mmc, on
     4c0000 :                            org    $4c0000
     4c0000 : 06 80 00           far0:   jmp    far0
     7fc000 :                            org    $7fc000
     7fc000 :                            ppage  $30
     7fc000 :                            rpage  $fd
     7fc000 : 16 80 00                   jsr    far0
     7fc003 : b6 12 34                   ldaa   $0fd234
)",
            image);
    EQ("page $30", 0x06, image.readByte(0x4C0000));
    EQ("fixed", 0x16, image.readByte(0x7FC000));
    EQ("rpage", 0xB6, image.readByte(0x7FC003));
}

void test_dis_mc68hcs12_mmc() {
    PREP_DIS(mc68hc12::DisMc68HC12);

    driver.setUppercase(true);
    driver.setOption("mmc", "on");

    DIS8("mc68hcs12", 0x0FBFFE,
            R"(      CPU   68HCS12
      OPTION    "mmc", "on"
      ORG   $0FBFFE
      NOP
      NOP
      BRA   $C000
)",
            R"(       0 :                            CPU   68HCS12
       0 :                            OPTION    "mmc", "on"
   FBFFE :                            ORG   $0FBFFE
   FBFFE : A7                         NOP
   FBFFF : A7                         NOP
   FC000 : 20 FE                      BRA   $C000
)",
            0xA7, 0xA7, 0x20, 0xFE);
}

void test_dis_mc68hcs12x_mmc() {
    PREP_DIS(mc68hc12::DisMc68HC12);

    driver.setUppercase(true);
    driver.setOption("mmc", "on");

    DIS8("mc68hcs12x", 0x7FBFFE,
            R"(      CPU   68HCS12X
      OPTION    "mmc", "on"
      ORG   $7FBFFE
      NOP
      NOP
      BRA   $C000
)",
            R"(       0 :                            CPU   68HCS12X
       0 :                            OPTION    "mmc", "on"
  7FBFFE :                            ORG   $7FBFFE
  7FBFFE : A7                         NOP
  7FBFFF : A7                         NOP
  7FC000 : 20 FE                      BRA   $C000
)",
            0xA7, 0xA7, 0x20, 0xFE);
}

void test_dis_mc68hc12_mmc() {
    PREP_DIS(mc68hc12::DisMc68HC12);

    driver.setUppercase(true);
    driver.setOption("mmc", "on");

    DIS8("mc68hc12", 0x0C3FFE,
            R"(      CPU   68HC12
      OPTION    "mmc", "on"
      ORG   $0C3FFE
      NOP
      NOP
      BRA   $0C4000
)",
            R"(       0 :                            CPU   68HC12
       0 :                            OPTION    "mmc", "on"
   C3FFE :                            ORG   $0C3FFE
   C3FFE : A7                         NOP
   C3FFF : A7                         NOP
   C4000 : 20 FE                      BRA   $0C4000
)",
            0xA7, 0xA7, 0x20, 0xFE);
}

void test_dis_mc68hc12() {
    PREP_DIS(mc68hc12::DisMc68HC12);

    driver.setUppercase(true);

    DIS8("mc68hc12", 0xabcd,
            R"(      CPU   68HC12
      ORG   $ABCD
      SUBB  $F1F2
* test.bin: error: Operand not allowed: "[D,X]"
*     ABD0 : 19 E7                      LEAY  [D,X]
)",
            R"(       0 :                            CPU   68HC12
    ABCD :                            ORG   $ABCD
    ABCD : F0 F1 F2                   SUBB  $F1F2
test.bin: error: Operand not allowed: "[D,X]"
    ABD0 : 19 E7                      LEAY  [D,X]
)",
         0xf0, 0xf1, 0xf2, 0x19, 0xE7);
}

void run_tests() {
    RUN_TEST(test_asm_mc68hc12);
    RUN_TEST(test_asm_mc68hc12_mmc);
    RUN_TEST(test_asm_mc68hcs12_mmc);
    RUN_TEST(test_asm_mc68hcs12x_mmc);
    RUN_TEST(test_dis_mc68hc12);
    RUN_TEST(test_dis_mc68hc12_mmc);
    RUN_TEST(test_dis_mc68hcs12_mmc);
    RUN_TEST(test_dis_mc68hcs12x_mmc);
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

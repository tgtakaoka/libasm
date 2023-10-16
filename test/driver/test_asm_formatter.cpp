/*
 * Copyright 2022 Tadashi G. Takaoka
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

#include "test_formatter_helper.h"

#include "asm_i8080.h"
#include "asm_ins8060.h"
#include "asm_mc6809.h"
#include "asm_mos6502.h"
#include "asm_z80.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_symbols_mc6809() {
    PREP_ASM_SYMBOL(mc6809::AsmMc6809, MotorolaDirective, REPORT_DUPLICATE);

    listing.setUpperHex(true);
    listing.enableLineNumber(true);

    ASM("mc6809",
            "label1  equ   $1234\n"
            "label1  set   $1234\n"
            "label1  equ   $3456\n"
            "var1    set   $1234\n"
            "var1    equ   $1234\n"
            "var1    set   $1234\n"
            "var1    set   $3456\n"
            "        org   $1234\n"
            "var1\n"
            "label2  bra   label2\n"
            "label3  fdb   label3\n",
            "       1/       0 : =1234              label1  equ   $1234\n"
            "mc6809:2: error: Duplicate label\n"
            "       2/       0 :                    label1  set   $1234\n"
            "mc6809:3: error: Duplicate label\n"
            "       3/       0 :                    label1  equ   $3456\n"
            "       4/       0 : =1234              var1    set   $1234\n"
            "mc6809:5: error: Duplicate label\n"
            "       5/       0 :                    var1    equ   $1234\n"
            "       6/       0 : =1234              var1    set   $1234\n"
            "       7/       0 : =3456              var1    set   $3456\n"
            "       8/    1234 :                            org   $1234\n"
            "mc6809:9:1: error: Duplicate label\n"
            "       9/    1234 :                    var1\n"
            "      10/    1234 : 20 FE              label2  bra   label2\n"
            "      11/    1236 : 12 36              label3  fdb   label3\n");
}

void test_symbols_ins8060() {
    PREP_ASM_SYMBOL(ins8060::AsmIns8060, NationalDirective, REPORT_DUPLICATE);

    listing.setUpperHex(true);
    listing.enableLineNumber(true);

    ASM("ins8060",
            "label1  =     X'1234\n"
            "        .set  label1, X'1234\n"
            "label1  =     X'3456\n"
            "        .set  var1, X'1234\n"
            "var1    =     X'1234\n"
            "        .set  var1, X'1234\n"
            "        .set  var1, X'3456\n"
            "        .=X'1234\n"
            "var1\n"
            "label2  jmp    label2\n"
            "label3  .dbyte label3\n",
            "       1/       0 : =1234              label1  =     X'1234\n"
            "ins8060:2: error: Duplicate label\n"
            "       2/       0 :                            .set  label1, X'1234\n"
            "ins8060:3: error: Duplicate label\n"
            "       3/       0 :                    label1  =     X'3456\n"
            "       4/       0 : =1234                      .set  var1, X'1234\n"
            "ins8060:5: error: Duplicate label\n"
            "       5/       0 :                    var1    =     X'1234\n"
            "       6/       0 : =1234                      .set  var1, X'1234\n"
            "       7/       0 : =3456                      .set  var1, X'3456\n"
            "       8/    1234 :                            .=X'1234\n"
            "ins8060:9:1: error: Duplicate label\n"
            "       9/    1234 :                    var1\n"
            "      10/    1234 : 90 FE              label2  jmp    label2\n"
            "      11/    1236 : 36 12              label3  .dbyte label3\n");
}

void test_symbols_z80() {
    PREP_ASM_SYMBOL(z80::AsmZ80, ZilogDirective, REPORT_DUPLICATE);

    listing.setUpperHex(true);
    listing.enableLineNumber(true);

    ASM("z80",
            "label1  equ   1234H\n"
            "label1  defl  1234H\n"
            "label1  equ   3456H\n"
            "var1    defl  1234H\n"
            "var1    equ   1234H\n"
            "var1    var   3456H\n"
            "        org   1234H\n"
            "label1  set   0, b\n"  // Z80 has SET instruction
            "var1\n"
            "label2  jr    label2\n"
            "label3  defw  label3\n",
            "       1/       0 : =1234              label1  equ   1234H\n"
            "z80:2: error: Duplicate label\n"
            "       2/       0 :                    label1  defl  1234H\n"  // SET
            "z80:3: error: Duplicate label\n"
            "       3/       0 :                    label1  equ   3456H\n"
            "       4/       0 : =1234              var1    defl  1234H\n"
            "z80:5: error: Duplicate label\n"
            "       5/       0 :                    var1    equ   1234H\n"
            "       6/       0 : =3456              var1    var   3456H\n"
            "       7/    1234 :                            org   1234H\n"
            "       8/    1234 : CB C0              label1  set   0, b\n"
            "z80:9:1: error: Duplicate label\n"
            "       9/    1236 :                    var1\n"
            "      10/    1236 : 18 FE              label2  jr    label2\n"
            "      11/    1238 : 38 12              label3  defw  label3\n");
}

void test_switch_cpu() {
    mc6809::AsmMc6809 asm6809;
    MotorolaDirective dir6809(asm6809);
    mos6502::AsmMos6502 asm6502;
    MostekDirective dir6502(asm6502);
    i8080::AsmI8080 asm8080;
    IntelDirective dir8080(asm8080);
    z80::AsmZ80 asmz80;
    ZilogDirective dirz80(asmz80);
    AsmDirective *dirs[] = {&dir6809, &dir6502, &dir8080, &dirz80};
    TestSources sources;
    AsmDriver driver(&dirs[0], &dirs[4], sources);
    BinMemory memory;
    AsmFormatter listing(driver, sources, memory);

    ASM("switch cpu",
            "        cpu   mc6809\n"
            "        org   $1000\n"
            "        ldx   #$1234\n"
            "        cpu   z80\n"
            "        ld    hl, 1234H\n"
            "        cpu   i8080\n"
            "        lxi   d, 5678H\n"
            "        z80syntax on\n"
            "        ld    bc, 9abcH\n"
            "        cpu   i8080zilog\n"
            "        ld    bc, 9abcH\n"
            "        z80syntax off\n"
            "        lxi   d, 5678H\n"
            "        cpu   65816\n"
            "        longa on\n"
            "        longi off\n"
            "        lda   #$1234\n"
            "        ldx   #$12\n"
            "        *=$123456\n"
            "        longa off\n"
            "        longi on\n"
            "        lda   #$12\n"
            "        ldx   #$1234\n",
            "          0 :                            cpu   mc6809\n"
            "       1000 :                            org   $1000\n"
            "       1000 : 8E 12 34                   ldx   #$1234\n"
            "       1003 :                            cpu   z80\n"
            "       1003 : 21 34 12                   ld    hl, 1234H\n"
            "       1006 :                            cpu   i8080\n"
            "       1006 : 11 78 56                   lxi   d, 5678H\n"
            "       1009 :                            z80syntax on\n"
            "       1009 : 01 BC 9A                   ld    bc, 9abcH\n"
            "       100C :                            cpu   i8080zilog\n"
            "       100C : 01 BC 9A                   ld    bc, 9abcH\n"
            "       100F :                            z80syntax off\n"
            "       100F : 11 78 56                   lxi   d, 5678H\n"
            "       1012 :                            cpu   65816\n"
            "       1012 :                            longa on\n"
            "       1012 :                            longi off\n"
            "       1012 : A9 34 12                   lda   #$1234\n"
            "       1015 : A2 12                      ldx   #$12\n"
            "     123456 :                            *=$123456\n"
            "     123456 :                            longa off\n"
            "     123456 :                            longi on\n"
            "     123456 : A9 12                      lda   #$12\n"
            "     123458 : A2 34 12                   ldx   #$1234\n");
}

void test_list_radix() {
    mos6502::AsmMos6502 asm6502;
    MostekDirective dir6502(asm6502);
    z80::AsmZ80 asmz80;
    ZilogDirective dirz80(asmz80);
    AsmDirective *dirs[] = {&dir6502, &dirz80};
    TestSources sources;
    std::map<std::string, std::string> options;
    AsmDriver driver(&dirs[0], &dirs[2], sources, &options);
    BinMemory memory;
    AsmFormatter listing(driver, sources, memory);

    options.emplace("list-radix", "8");
    driver.applyOptions();

    ASM("list-radix",
            "        cpu   z80\n"
            "        org   1234Q\n"
            "        ld    hl, 1234H\n"
            "        option \"list-radix\", \"16\"\n"
            "        jr    $\n"
            "        cpu   65816\n"
            "        adc   #$34\n"
            "        *=@1234567\n"
            "        option \"list-radix\", \"8\"\n"
            "        lda   #$34\n"
            "        *=@3456\n"
            "        ldx   #$12\n"
            "        cpu   z80\n"
            "        res   0, (iy-128)\n",
            "          0 :                            cpu   z80\n"
            "       1234 :                            org   1234Q\n"
            "       1234 : 041 064 022                ld    hl, 1234H\n"
            "        29F :                            option \"list-radix\", \"16\"\n"
            "        29F : 18 FE                      jr    $\n"
            "        2A1 :                            cpu   65816\n"
            "        2A1 : 69 34                      adc   #$34\n"
            "      53977 :                            *=@1234567\n"
            "    1234567 :                            option \"list-radix\", \"8\"\n"
            "    1234567 : 251 064                    lda   #$34\n"
            "       3456 :                            *=@3456\n"
            "       3456 : 242 022                    ldx   #$12\n"
            "       3460 :                            cpu   z80\n"
            "       3460 : 375 313 200 206            res   0, (iy-128)\n");
}

void test_function() {
    PREP_ASM_SYMBOL(ins8060::AsmIns8060, NationalDirective, REPORT_DUPLICATE);

    ASM("ins8060",
            "        cpu    ins8060\n"
            "high:   function v  , v >> 8 ; high 8-bit\n"
            "low:    function v, L(v)     ; predefined\n"
            "cons:   function hi, lo, (hi << 8) | lo ; 16-bit\n"
            "CONS:   function -1\n"
            "        .=x'abcd\n"
            "label:  and    @e(p1)\n"
            "        .byte  high(label)\n"
            "        .byte  low(label)\n"
            "        .dbyte cons(h(label), l(label))\n"
            "        .dbyte cons (high(x'1234),low(x'3456))\n"
            "        .dbyte CONS (  ) \n"
            "        .dbyte ADDR(label)\n"
            "high:   function x,x   ; duplicate\n"
            "label:  function y,y   ; symbol\n"
            "cons=0                 ; function\n"
            "        .dbyte cons(0) ; requires 2\n"
            "        .dbyte CONS(0) ; requires 0\n"
            "        .dbyte CONS    ; missing\n",
            "          0 :                            cpu    ins8060\n"
            "          0 :                    high:   function v  , v >> 8 ; high 8-bit\n"
            "          0 :                    low:    function v, L(v)     ; predefined\n"
            "          0 :                    cons:   function hi, lo, (hi << 8) | lo ; "
            "16-bit\n"
            "          0 :                    CONS:   function -1\n"
            "       ABCD :                            .=x'abcd\n"
            "       ABCD : D5 80              label:  and    @e(p1)\n"
            "       ABCF : AB                         .byte  high(label)\n"
            "       ABD0 : CD                         .byte  low(label)\n"
            "       ABD1 : CD AB                      .dbyte cons(h(label), l(label))\n"
            "       ABD3 : 56 12                      .dbyte cons (high(x'1234),low(x'3456))\n"
            "       ABD5 : FF FF                      .dbyte CONS (  ) \n"
            "       ABD7 : CC AB                      .dbyte ADDR(label)\n"
            "ins8060:14: error: Duplicate function\n"
            "       ABD9 :                    high:   function x,x   ; duplicate\n"
            "ins8060:15: error: Duplicate label\n"
            "       ABD9 :                    label:  function y,y   ; symbol\n"
            "ins8060:16: error: Duplicate label\n"
            "       ABD9 :                    cons=0                 ; function\n"
            "ins8060:17:16: error: Too few function arguments\n"
            "       ABD9 :                            .dbyte cons(0) ; requires 2\n"
            "ins8060:18:16: error: Too many function arguments\n"
            "       ABD9 :                            .dbyte CONS(0) ; requires 0\n"
            "ins8060:19:16: error: Missing function arguments\n"
            "       ABD9 :                            .dbyte CONS    ; missing\n");
}

void run_tests() {
    RUN_TEST(test_symbols_mc6809);
    RUN_TEST(test_symbols_ins8060);
    RUN_TEST(test_symbols_z80);
    RUN_TEST(test_switch_cpu);
    RUN_TEST(test_list_radix);
    RUN_TEST(test_function);
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

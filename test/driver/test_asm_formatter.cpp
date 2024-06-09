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
    PREP_ASM(mc6809::AsmMc6809, MotorolaDirective);

    driver.setUpperHex(true);
    driver.setLineNumber(true);

    ASM("mc6809",
            R"(label1  equ   $1234
label1  set   $1234
label1  equ   $3456
var1    set   $1234
var1    equ   $1234
var1    set   $1234
var1    set   $3456
        org   $1234
var1
label2  bra   label2
label3  fdb   label3
label2:
)",
            R"(       1/       0 : =1234              label1  equ   $1234
mc6809:2:1: error: Duplicate label: "label1  set   $1234"
       2/       0 :                    label1  set   $1234
mc6809:3:1: error: Duplicate label: "label1  equ   $3456"
       3/       0 :                    label1  equ   $3456
       4/       0 : =1234              var1    set   $1234
mc6809:5:1: error: Duplicate label: "var1    equ   $1234"
       5/       0 :                    var1    equ   $1234
       6/       0 : =1234              var1    set   $1234
       7/       0 : =3456              var1    set   $3456
       8/    1234 :                            org   $1234
mc6809:9:1: error: Duplicate label: "var1"
       9/    1234 :                    var1
      10/    1234 : 20 FE              label2  bra   label2
      11/    1236 : 12 36              label3  fdb   label3
mc6809:12:1: error: Duplicate label: "label2:"
      12/    1238 :                    label2:
)");
}

void test_symbols_ins8060() {
    PREP_ASM(ins8060::AsmIns8060, NationalDirective);

    driver.setUpperHex(true);
    driver.setLineNumber(true);

    ASM("ins8060",
            R"(label1  =     X'1234
        .set  label1, X'1234
label1  =     X'3456
        .set  var1, X'1234
var1    =     X'1234
        .set  var1, X'1234
        .set  var1, X'3456
        .=X'1234
var1
label2  jmp    label2
label3  .dbyte label3
)",
            R"(       1/       0 : =1234              label1  =     X'1234
ins8060:2:15: error: Duplicate label: "label1, X'1234"
       2/       0 :                            .set  label1, X'1234
ins8060:3:1: error: Duplicate label: "label1  =     X'3456"
       3/       0 :                    label1  =     X'3456
       4/       0 : =1234                      .set  var1, X'1234
ins8060:5:1: error: Duplicate label: "var1    =     X'1234"
       5/       0 :                    var1    =     X'1234
       6/       0 : =1234                      .set  var1, X'1234
       7/       0 : =3456                      .set  var1, X'3456
       8/    1234 :                            .=X'1234
ins8060:9:1: error: Duplicate label: "var1"
       9/    1234 :                    var1
      10/    1234 : 90 FE              label2  jmp    label2
      11/    1236 : 36 12              label3  .dbyte label3
)");
}

void test_symbols_z80() {
    PREP_ASM(z80::AsmZ80, ZilogDirective);

    driver.setUpperHex(true);
    driver.setLineNumber(true);

    ASM("z80",
            R"(label1  equ   1234H
label1  defl  1234H
label1  equ   3456H
var1    defl  1234H
var1    equ   1234H
var1    var   3456H
        org   1234H
label1  set   0, b  ; Z80 has SET instruction
var1
label2  jr    label2
label3  defw  label3
)",
            R"(       1/       0 : =1234              label1  equ   1234H
z80:2:1: error: Duplicate label: "label1  defl  1234H"
       2/       0 :                    label1  defl  1234H
z80:3:1: error: Duplicate label: "label1  equ   3456H"
       3/       0 :                    label1  equ   3456H
       4/       0 : =1234              var1    defl  1234H
z80:5:1: error: Duplicate label: "var1    equ   1234H"
       5/       0 :                    var1    equ   1234H
       6/       0 : =3456              var1    var   3456H
       7/    1234 :                            org   1234H
       8/    1234 : CB C0              label1  set   0, b  ; Z80 has SET instruction
z80:9:1: error: Duplicate label: "var1"
       9/    1236 :                    var1
      10/    1236 : 18 FE              label2  jr    label2
      11/    1238 : 38 12              label3  defw  label3
)");
}

void test_undefined() {
    PREP_ASM(mc6809::AsmMc6809, MotorolaDirective);

    driver.setUpperHex(true);

    ASM("mc6809",
            R"(        org    $1000
label1: lda   #0      ; comment
label2: bra   label1  comment
        lda   #1      * comment
)",
            R"(       1000 :                            org    $1000
       1000 : 86 00              label1: lda   #0      ; comment
       1002 : 20 FC              label2: bra   label1  comment
mc6809:4:25: error: Undefined symbol: "comment"
       1004 : 86 00                      lda   #1      * comment
)");

    driver.clearSymbols();

    ASM("mc6809",
            R"(        org    $1000
label1: equ   $1000   ; comment
label2: equ   label1  comment
label3: equ   1       * comment
)",
            R"(       1000 :                            org    $1000
       1000 : =1000              label1: equ   $1000   ; comment
       1000 : =1000              label2: equ   label1  comment
mc6809:4:25: error: Undefined symbol: "comment"
       1000 : =0                 label3: equ   1       * comment
)");

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
    AsmDriver driver{&dir6809, &dir6502, &dir8080, &dirz80};
    TestSources sources;

    ASM("switch cpu",
            R"(        cpu   mc6809
        org   $1000
        ldx   #$1234
        cpu   z80
        ld    hl, 1234H
        cpu   i8080
        lxi   d, 5678H
        z80syntax on
        ld    bc, 9abcH
        cpu   i8080zilog
        ld    bc, 9abcH
        z80syntax off
        lxi   d, 5678H
        cpu   65816
        longa on
        longi off
        lda   #$1234
        ldx   #$12
        *=$123456
        longa off
        longi on
        lda   #$12
        ldx   #$1234
)",
            R"(          0 :                            cpu   mc6809
       1000 :                            org   $1000
       1000 : 8E 12 34                   ldx   #$1234
       1003 :                            cpu   z80
       1003 : 21 34 12                   ld    hl, 1234H
       1006 :                            cpu   i8080
       1006 : 11 78 56                   lxi   d, 5678H
       1009 :                            z80syntax on
       1009 : 01 BC 9A                   ld    bc, 9abcH
       100C :                            cpu   i8080zilog
       100C : 01 BC 9A                   ld    bc, 9abcH
       100F :                            z80syntax off
       100F : 11 78 56                   lxi   d, 5678H
       1012 :                            cpu   65816
       1012 :                            longa on
       1012 :                            longi off
       1012 : A9 34 12                   lda   #$1234
       1015 : A2 12                      ldx   #$12
     123456 :                            *=$123456
     123456 :                            longa off
     123456 :                            longi on
     123456 : A9 12                      lda   #$12
     123458 : A2 34 12                   ldx   #$1234
)");
}

void test_list_radix() {
    mos6502::AsmMos6502 asm6502;
    MostekDirective dir6502(asm6502);
    z80::AsmZ80 asmz80;
    ZilogDirective dirz80(asmz80);
    AsmDriver driver{&dir6502, &dirz80};
    TestSources sources;

    ASM("list-radix",
            R"(        option "list-radix", 8
        cpu   z80
        org   1234Q
        ld    hl, 1234H
        option "list-radix", 16
        jr    $
        cpu   65816
        adc   #$34
        *=@1234567
        option "list-radix", "8"
        lda   #$34
        *=@3456
        ldx   #$12
        cpu   z80
        res   0, (iy-128)
)",
            R"(          0 :                            option "list-radix", 8
          0 :                            cpu   z80
       1234 :                            org   1234Q
       1234 : 041 064 022                ld    hl, 1234H
        29F :                            option "list-radix", 16
        29F : 18 FE                      jr    $
        2A1 :                            cpu   65816
        2A1 : 69 34                      adc   #$34
      53977 :                            *=@1234567
    1234567 :                            option "list-radix", "8"
    1234567 : 251 064                    lda   #$34
       3456 :                            *=@3456
       3456 : 242 022                    ldx   #$12
       3460 :                            cpu   z80
       3460 : 375 313 200 206            res   0, (iy-128)
)");
}

void test_function() {
    PREP_ASM(ins8060::AsmIns8060, NationalDirective);

    ASM("ins8060",
            R"(        cpu    ins8060
high:   function v  , v >> 8 ; high 8-bit
low:    function v, L(v)     ; predefined
cons:   function hi, lo, (hi << 8) | lo ; 16-bit
CONS:   function -1
        .=x'abcd
label:  and    @e(p1)
        .byte  high(label)
        .byte  low(label)
        .dbyte cons(h(label), l(label))
        .dbyte cons (high(x'1234),low(x'3456))
        .dbyte CONS (  ) 
        .dbyte ADDR(label)
high:   function x,x   ; duplicate
label:  function y,y   ; symbol
cons=0                 ; function
        .dbyte cons(0) ; requires 2
        .dbyte CONS(0) ; requires 0
        .dbyte CONS    ; missing
)",
            R"(          0 :                            cpu    ins8060
          0 :                    high:   function v  , v >> 8 ; high 8-bit
          0 :                    low:    function v, L(v)     ; predefined
          0 :                    cons:   function hi, lo, (hi << 8) | lo ; 16-bit
          0 :                    CONS:   function -1
       ABCD :                            .=x'abcd
       ABCD : D5 80              label:  and    @e(p1)
       ABCF : AB                         .byte  high(label)
       ABD0 : CD                         .byte  low(label)
       ABD1 : CD AB                      .dbyte cons(h(label), l(label))
       ABD3 : 56 12                      .dbyte cons (high(x'1234),low(x'3456))
       ABD5 : FF FF                      .dbyte CONS (  ) 
       ABD7 : CC AB                      .dbyte ADDR(label)
ins8060:14:1: error: Duplicate function: "high:   function x,x   ; duplicate"
       ABD9 :                    high:   function x,x   ; duplicate
ins8060:15:1: error: Duplicate label: "label:  function y,y   ; symbol"
       ABD9 :                    label:  function y,y   ; symbol
ins8060:16:1: error: Duplicate label: "cons=0                 ; function"
       ABD9 :                    cons=0                 ; function
ins8060:17:16: error: Too few function arguments: "cons(0) ; requires 2"
       ABD9 :                            .dbyte cons(0) ; requires 2
ins8060:18:16: error: Too many function arguments: "CONS(0) ; requires 0"
       ABD9 :                            .dbyte CONS(0) ; requires 0
ins8060:19:16: error: Missing function arguments: "CONS    ; missing"
       ABD9 :                            .dbyte CONS    ; missing
)");
}

void test_forward_labels() {
    PREP_ASM(mc6809::AsmMc6809, MotorolaDirective);

    driver.setUpperHex(true);
    driver.setLineNumber(true);

    ASM("mc6809",
            R"(       org   $1000
        lda   label1,pcr
        ldb   label3,x
        rmb   label2-label1
label1  equ   *
label2  equ   label1+label3
label3  equ   label4+1
label4  equ   label5-1
label5  equ   4
)",
            R"(       1/    1000 :                           org   $1000
       2/    1000 : A6 8C 06                   lda   label1,pcr
       3/    1003 : E6 04                      ldb   label3,x
       4/    1005 :                            rmb   label2-label1
       5/    1009 : =1009              label1  equ   *
       6/    1009 : =100D              label2  equ   label1+label3
       7/    1009 : =4                 label3  equ   label4+1
       8/    1009 : =3                 label4  equ   label5-1
       9/    1009 : =4                 label5  equ   4
)");
}

void test_include() {
    PREP_ASM(mc6809::AsmMc6809, MotorolaDirective);

    TestReader exist("data/exist.inc");
    sources.add(exist);
    exist.add("        fcc   /exist/");

    driver.setUpperHex(true);
    driver.setLineNumber(true);
    driver.setOption("smart-branch", "on");

    ASM("mc6809",
            R"(        cpu   mc6809
* comment line
        org   $abcd
        include "data/exist.inc"
        include "data/not-exist.inc"
)",
            R"(       1/       0 :                            cpu   mc6809
       2/       0 :                    * comment line
       3/    ABCD :                            org   $abcd
       4/    ABCD :                            include "data/exist.inc"
(1)    1/    ABCD : 65 78 69 73 74             fcc   /exist/
mc6809:5:18: error: Include file not found: "data/not-exist.inc""
       5/    ABD2 :                            include "data/not-exist.inc"
)");
}

void run_tests() {
    RUN_TEST(test_symbols_mc6809);
    RUN_TEST(test_symbols_ins8060);
    RUN_TEST(test_symbols_z80);
    RUN_TEST(test_undefined);
    RUN_TEST(test_switch_cpu);
    RUN_TEST(test_list_radix);
    RUN_TEST(test_function);
    RUN_TEST(test_forward_labels);
    RUN_TEST(test_include);
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

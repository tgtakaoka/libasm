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

void test_symbols() {
    PREP_ASM_SYMBOL(z80::AsmZ80, IntelDirective, REPORT_DUPLICATE);

    listing.setUpperHex(true);
    listing.enableLineNumber(true);

    ASM("z80",
            "label1  equ   1234H\n"
            "label1  :=    1234H\n"
            "label1  =     1234H\n"
            "label1  equ   3456H\n"
            "var1    :=    1234H\n"
            "var1    equ   1234H\n"
            "var1    .set  1234H\n"
            "var1    .set  3456H\n"
            "        org   1234H\n"
            "label1  set   0, b\n"  // Z80 has SET instruction
            "var1\n"
            "label2  jr    label2\n"
            "label3  dw    label3\n",
            "       1/       0 : =1234              label1  equ   1234H\n"
            "z80:2: error: Duplicate label\n"
            "       2/       0 :                    label1  :=    1234H\n"  // SET
            "       3/       0 : =1234              label1  =     1234H\n"  // EQU
            "z80:4: error: Duplicate label\n"
            "       4/       0 :                    label1  equ   3456H\n"
            "       5/       0 : =1234              var1    :=    1234H\n"  // SET
            "z80:6: error: Duplicate label\n"
            "       6/       0 :                    var1    equ   1234H\n"
            "       7/       0 : =1234              var1    .set  1234H\n"
            "       8/       0 : =3456              var1    .set  3456H\n"
            "       9/    1234 :                            org   1234H\n"
            "      10/    1234 : CB C0              label1  set   0, b\n"
            "z80:11:1: error: Duplicate label\n"
            "      11/    1236 :                    var1\n"
            "      12/    1236 : 18 FE              label2  jr    label2\n"
            "      13/    1238 : 38 12              label3  dw    label3\n");
}

void test_switch_cpu() {
    mc6809::AsmMc6809 asm6809;
    MotorolaDirective dir6809(asm6809);
    mos6502::AsmMos6502 asm6502;
    MotorolaDirective dir6502(asm6502);
    i8080::AsmI8080 asm8080;
    IntelDirective dir8080(asm8080);
    z80::AsmZ80 asmz80;
    IntelDirective dirz80(asmz80);
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
            "        org   $123456\n"
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
            "     123456 :                            org   $123456\n"
            "     123456 :                            longa off\n"
            "     123456 :                            longi on\n"
            "     123456 : A9 12                      lda   #$12\n"
            "     123458 : A2 34 12                   ldx   #$1234\n");
}

void test_function() {
    PREP_ASM_SYMBOL(ins8060::AsmIns8060, NationalDirective, REPORT_DUPLICATE);

    ASM("ins8060",
            "        cpu   ins8060\n"
            "high:   function v  , v >> 8 ; high 8-bit\n"
            "low:    function v, v & x'FF ; low 8-bit\n"
            "cons:   function hi, lo, (hi << 8) | lo ; 16-bit\n"
            "CONS:   function -1\n"
            "label:  org   x'abcd\n"
            "        and   @e(p1)\n"
            "        db    high(label)\n"
            "        db    low(label)\n"
            "        dw    cons(h(label), l(label))\n"
            "        dw    cons (high(x'1234),low(x'3456))\n"
            "        dw    CONS (  ) \n"
            "high:   function x,x  ; duplicate\n"
            "label:  function y,y  ; symbol\n"
            "cons:   equ   0       ; function\n"
            "        dw    cons(0) ; requires 2\n"
            "        dw    CONS(0) ; requires 0\n"
            "        dw    CONS    ; missing\n",
            "          0 :                            cpu   ins8060\n"
            "          0 :                    high:   function v  , v >> 8 ; high 8-bit\n"
            "          0 :                    low:    function v, v & x'FF ; low 8-bit\n"
            "          0 :                    cons:   function hi, lo, (hi << 8) | lo ; 16-bit\n"
            "          0 :                    CONS:   function -1\n"
            "       ABCD :                    label:  org   x'abcd\n"
            "       ABCD : D5 80                      and   @e(p1)\n"
            "       ABCF : AB                         db    high(label)\n"
            "       ABD0 : CD                         db    low(label)\n"
            "       ABD1 : CD AB                      dw    cons(h(label), l(label))\n"
            "       ABD3 : 56 12                      dw    cons (high(x'1234),low(x'3456))\n"
            "       ABD5 : FF FF                      dw    CONS (  ) \n"
            "ins8060:13: error: Duplicate function\n"
            "       ABD7 :                    high:   function x,x  ; duplicate\n"
            "ins8060:14: error: Duplicate label\n"
            "       ABD7 :                    label:  function y,y  ; symbol\n"
            "ins8060:15: error: Duplicate label\n"
            "       ABD7 :                    cons:   equ   0       ; function\n"
            "ins8060:16: error: Too few function arguments\n"
            "       ABD7 :                            dw    cons(0) ; requires 2\n"
            "ins8060:17: error: Too many function arguments\n"
            "       ABD7 :                            dw    CONS(0) ; requires 0\n"
            "ins8060:18: error: Missing function arguments\n"
            "       ABD7 :                            dw    CONS    ; missing\n");
}

void run_tests() {
    RUN_TEST(test_symbols);
    RUN_TEST(test_switch_cpu);
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

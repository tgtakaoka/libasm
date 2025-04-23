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

#include "asm_tms370.h"
#include "dis_tms370.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_tms370() {
    PREP_ASM(tms370::AsmTms370, IntelDirective);

    ASM("tms370",
            R"(        cpu     tms370
; comment line
        org     7BCDH           ; set location
label1: equ     7defh           ; define label
        mov     0abach(b), a
        movw    #0a9aah(b), r171
        mov     -2(r171), a
        movw    #-2(r171), r173
        br      label1          ; loop
)",
            R"(          0 :                            cpu     tms370
          0 :                    ; comment line
       7BCD :                            org     7BCDH           ; set location
       7BCD : =7DEF              label1: equ     7defh           ; define label
       7BCD : AA AB AC                   mov     0abach(b), a
       7BD0 : A8 A9 AA AB                movw    #0a9aah(b), r171
       7BD4 : F4 EA FE AB                mov     -2(r171), a
       7BD8 : F4 E8 FE AB AD             movw    #-2(r171), r173
       7BDD : 8C 7D EF                   br      label1          ; loop
)");
}

void test_dis_tms370() {
    PREP_DIS(tms370::DisTms370);

    driver.setUpperHex(false);

    DIS8("tms370", 0x7bcd,
            R"(      cpu   tms370
      org   7bcdh
      mov   0abach(b), a
      movw  #0a9aah(b), r171
      mov   -2(r171), a
      movw  #-2(r171), r173
; test.bin: error: Unknown instruction
;     7bdd : 82
)",
            R"(       0 :                            cpu   tms370
    7bcd :                            org   7bcdh
    7bcd : aa ab ac                   mov   0abach(b), a
    7bd0 : a8 a9 aa ab                movw  #0a9aah(b), r171
    7bd4 : f4 ea fe ab                mov   -2(r171), a
    7bd8 : f4 e8 fe ab ad             movw  #-2(r171), r173
test.bin: error: Unknown instruction
    7bdd : 82
)",
         0xAA, 0xAB, 0xAC, 0xA8, 0xA9, 0xAA, 0xAB, 0xF4, 0xEA, 0xFE, 0xAB,
         0xF4, 0xE8, 0xFE, 0xAB, 0xAD, 0x82);
}

void run_tests() {
    RUN_TEST(test_asm_tms370);
    RUN_TEST(test_dis_tms370);
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

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

#include "asm_mc68hc16.h"
#include "dis_mc68hc16.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_mc68hc16() {
    PREP_ASM(mc68hc16::AsmMc68HC16, MotorolaDirective);

    driver.setUpperHex(false);

    ASM("mc68hc16",
            R"(        cpu   mc68hc16
* comment line
        org   $abcc    set location
label1: equ   $f1f2    define label
        subb  label1   use label
        jmp   label1   ; use label
)",
            R"(          0 :                            cpu   mc68hc16
          0 :                    * comment line
       abcc :                            org   $abcc    set location
       abcc : =f1f2              label1: equ   $f1f2    define label
       abcc : 17 f0 f1 f2                subb  label1   use label
       abd0 : 7a 00 f1 f2                jmp   label1   ; use label
)");
}

void test_dis_mc68hc16() {
    PREP_DIS(mc68hc16::DisMc68HC16);

    driver.setUppercase(true);

    DIS8("mc68hc16", 0xabcc,
            R"(      CPU    68HC16
      ORG    $0ABCC
      SUBB   $F1F2
* test.bin: error: Unknown instruction
*     ABD0 : 07
)",
            R"(       0 :                            CPU    68HC16
    ABCC :                            ORG    $0ABCC
    ABCC : 17 F0 F1 F2                SUBB   $F1F2
test.bin: error: Unknown instruction
    ABD0 : 07
)",
         0x17, 0xf0, 0xf1, 0xf2, 0x07);
}

void run_tests() {
    RUN_TEST(test_asm_mc68hc16);
    RUN_TEST(test_dis_mc68hc16);
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

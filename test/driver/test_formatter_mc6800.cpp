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

#include "asm_mc6800.h"
#include "dis_mc6800.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_mc6800() {
    PREP_ASM(mc6800::AsmMc6800, MotorolaDirective);

    driver.setUpperHex(false);

    ASM("mc6800",
            R"(        cpu   mc6800
* comment line
        org   $abcd    set location
label1: equ   $f1f2    define label
        subb  label1   use label
        jmp   label1   ; use label
)",
            R"(          0 :                            cpu   mc6800
          0 :                    * comment line
       abcd :                            org   $abcd    set location
       abcd : =f1f2              label1: equ   $f1f2    define label
       abcd : f0 f1 f2                   subb  label1   use label
       abd0 : 7e f1 f2                   jmp   label1   ; use label
)");
}

void test_dis_mc6800() {
    PREP_DIS(mc6800::DisMc6800);

    driver.setUppercase(true);

    DIS8("mc6800", 0xabcd,
            R"(      CPU   6800
      ORG   $ABCD
      SUBB  $F1F2
* test.bin: error: Unknown instruction
*     ABD0 : 83
)",
            R"(       0 :                            CPU   6800
    ABCD :                            ORG   $ABCD
    ABCD : F0 F1 F2                   SUBB  $F1F2
test.bin: error: Unknown instruction
    ABD0 : 83
)",
            0xf0, 0xf1, 0xf2, 0x83);
}

void run_tests() {
    RUN_TEST(test_asm_mc6800);
    RUN_TEST(test_dis_mc6800);
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

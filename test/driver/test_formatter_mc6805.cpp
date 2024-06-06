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

#include "asm_mc6805.h"
#include "dis_mc6805.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_mc6805() {
    PREP_ASM(mc6805::AsmMc6805, MotorolaDirective);

    driver.setUpperHex(false);

    ASM("mc146805",
            R"(        cpu   mc146805
* comment line
        org   $1234
label1: equ   $0800
        stx   label1, x
)",
            R"(          0 :                            cpu   mc146805
          0 :                    * comment line
       1234 :                            org   $1234
       1234 : =800               label1: equ   $0800
       1234 : df 08 00                   stx   label1, x
)");
}

void test_dis_mc6805() {
    PREP_DIS(mc6805::DisMc6805);

    driver.setUpperHex(false);
    driver.setUppercase(true);

    DIS8("mc146805", 0x1234,
            R"(      CPU   146805
      ORG   $1234
      STX   $4000,X
* test.bin: error: Unknown instruction
*     1237 : 82
* test.bin: error: Overflow range: "$2000"
*     1238 : c6 20 00                   LDA   $2000
)",
            R"(       0 :                            CPU   146805
    1234 :                            ORG   $1234
    1234 : df 40 00                   STX   $4000,X
test.bin: error: Unknown instruction
    1237 : 82
test.bin: error: Overflow range: "$2000"
    1238 : c6 20 00                   LDA   $2000
)",
            0xdf, 0x40, 0x00, 0x82, 0xC6, 0x20, 0x00);
}

void run_tests() {
    RUN_TEST(test_asm_mc6805);
    RUN_TEST(test_dis_mc6805);
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

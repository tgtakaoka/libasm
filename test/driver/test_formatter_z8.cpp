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

#include "asm_z8.h"
#include "dis_z8.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_z8() {
    PREP_ASM(z8::AsmZ8, Z8Directive);

    driver.setUpperHex(false);

    ASM("z8",
            R"(        cpu     z8
; comment line
        org     0abcdh
        ld      r12, 0c9h(r8)
)",
            R"(          0 :                            cpu     z8
          0 :                    ; comment line
       abcd :                            org     0abcdh
       abcd : c7 c8 c9                   ld      r12, 0c9h(r8)
)");
}

void test_dis_z8() {
    PREP_DIS(z8::DisZ8);

    driver.setUpperHex(false);

    DIS8("z8", 0xabcd,
            R"(      cpu   z8
      org   %abcd
      ld    r12, %c9(r8)
; test.bin: error: Unknown instruction
;     abd0 : 0f
)",
            R"(       0 :                            cpu   z8
    abcd :                            org   %abcd
    abcd : c7 c8 c9                   ld    r12, %c9(r8)
test.bin: error: Unknown instruction
    abd0 : 0f
)",
            0xc7, 0xc8, 0xc9, 0x0f);
}

void run_tests() {
    RUN_TEST(test_asm_z8);
    RUN_TEST(test_dis_z8);
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

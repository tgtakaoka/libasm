/*
 * Copyright 2026 Tadashi G. Takaoka
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
#include "asm_c166.h"
#include "dis_c166.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_c166() {
    PREP_ASM(c166::AsmC166, IntelDirective);

    driver.setUpperHex(false);

    ASM("80c167",
            R"(        cpu   80c167
; comment line
        org   12340h
label1: equ   0fe12h
        mov   label1, #1234h
        add   r1, [r2+]
        jmpr  cc_nz, label2
        bset  0fd02h.3
label2: extr  #1
        mov   0f012h, r1
        jmpa  cc_uc, label2
)",
            R"(          0 :                            cpu   80c167
          0 :                    ; comment line
      12340 :                            org   12340h
      12340 : =fe12              label1: equ   0fe12h
      12340 : e6 09 34 12                mov   label1, #1234h
      12344 : 08 1e                      add   r1, [r2+]
      12346 : 3d 01                      jmpr  cc_nz, label2
      12348 : 3f 01                      bset  0fd02h.3
      1234a : d1 80              label2: extr  #1
      1234c : f6 f1 12 f0                mov   0f012h, r1
      12350 : ea 00 4a 23                jmpa  cc_uc, label2
)");
}

void test_dis_c166() {
    PREP_DIS(c166::DisC166);

    driver.setUpperHex(false);

    DIS8("80c166", 0x12340,
            R"(      cpu    80c166
      org    12340h
      mov    0fe12h, #1234h
      add    r1, [r2+]
      jmpr   cc_nz, 1234ah
      bset   0fd02h.3
; test.bin: error: Illegal operand mode: "4, 2345h"
;    1234a : fa 04 45 23                jmps   4, 2345h
)",
            R"(       0 :                            cpu    80c166
   12340 :                            org    12340h
   12340 : e6 09 34 12                mov    0fe12h, #1234h
   12344 : 08 1e                      add    r1, [r2+]
   12346 : 3d 01                      jmpr   cc_nz, 1234ah
   12348 : 3f 01                      bset   0fd02h.3
test.bin: error: Illegal operand mode: "4, 2345h"
   1234a : fa 04 45 23                jmps   4, 2345h
)",
            0xe6, 0x09, 0x34, 0x12, 0x08, 0x1e, 0x3d, 0x01, 0x3f, 0x01, 0xfa, 0x04, 0x45, 0x23);
}

void run_tests() {
    RUN_TEST(test_asm_c166);
    RUN_TEST(test_dis_c166);
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

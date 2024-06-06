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

#include "asm_z8000.h"
#include "dis_z8000.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_z8001() {
    PREP_ASM(z8000::AsmZ8000, ZilogDirective);

    driver.setUpperHex(false);

    ASM("z8001",
            R"(        cpu    z8001
; comment line
        org    789abch
        ldb    |160017h|(r1), #25
        set    r5, r15
)",
            R"(          0 :                            cpu    z8001
          0 :                    ; comment line
     789abc :                            org    789abch
     789abc : 4c15 1617 1919             ldb    |160017h|(r1), #25
     789ac2 : 250f 0500                  set    r5, r15
)");
}

void test_asm_z8002() {
    PREP_ASM(z8000::AsmZ8000, ZilogDirective);

    driver.setUpperHex(false);

    ASM("z8002",
            R"(        cpu    z8002
; comment line
        org    9abch
        cpl    rr0, #01020304h
        set    1234h(r5), #15
)",
            R"(          0 :                            cpu    z8002
          0 :                    ; comment line
       9abc :                            org    9abch
       9abc : 1000 0102 0304             cpl    rr0, #01020304h
       9ac2 : 655f 1234                  set    1234h(r5), #15
)");
}

void test_dis_z8001() {
    PREP_DIS(z8000::DisZ8000);

    driver.setUpperHex(true);
    driver.setUppercase(true);
    driver.setOption("short-direct", "on");

    DIS16("z8001", 0x789abc,
            R"(      CPU    Z8001
      ORG    %789ABC
      LDB    |<<22>>%0017|(R1), #25
; test.bin: error: Registers overlapped: "RR4"
;   789AC2 : 9745                       POP    R5, @RR4
)",
            R"(       0 :                            CPU    Z8001
  789ABC :                            ORG    %789ABC
  789ABC : 4C15 1617 1919             LDB    |<<22>>%0017|(R1), #25
test.bin: error: Registers overlapped: "RR4"
  789AC2 : 9745                       POP    R5, @RR4
)",
            0x4c15, 0x1617, 0x1919, 0x9745);
}

void test_dis_z8002() {
    PREP_DIS(z8000::DisZ8000);

    driver.setUpperHex(false);

    DIS16("z8002", 0x9abc,
            R"|(      cpu    z8002
      org    %9abc
      cpl    rr0, #%01020304
; test.bin: error: Registers overlapped: "r4)"
;     9ac2 : 5144 0000                  pushl  @r4, %0000(r4)
)|",
            R"|(       0 :                            cpu    z8002
    9abc :                            org    %9abc
    9abc : 1000 0102 0304             cpl    rr0, #%01020304
test.bin: error: Registers overlapped: "r4)"
    9ac2 : 5144 0000                  pushl  @r4, %0000(r4)
)|",
            0x1000, 0x0102, 0x0304, 0x5144, 0x0000);
}

void run_tests() {
    RUN_TEST(test_asm_z8001);
    RUN_TEST(test_asm_z8002);
    RUN_TEST(test_dis_z8001);
    RUN_TEST(test_dis_z8002);
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

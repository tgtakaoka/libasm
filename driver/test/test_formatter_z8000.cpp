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
    PREP_ASM(z8000::AsmZ8000, IntelDirective);

    listing.setUpperHex(false);

    ASM("z8001",
            "        cpu    z8001\n"
            "; comment line\n"
            "        org    789abch\n"
            "        ldb    |160017h|(r1), #25\n",
            "          0 :                            cpu    z8001\n"
            "          0 :                    ; comment line\n"
            "     789abc :                            org    789abch\n"
            "     789abc : 4c15 1617 1919             ldb    |160017h|(r1), #25\n");
}

void test_asm_z8002() {
    PREP_ASM(z8000::AsmZ8000, IntelDirective);

    listing.setUpperHex(false);

    ASM("z8002",
            "        cpu    z8002\n"
            "; comment line\n"
            "        org    9abch\n"
            "        cpl    rr0, #01020304h\n",
            "          0 :                            cpu    z8002\n"
            "          0 :                    ; comment line\n"
            "       9abc :                            org    9abch\n"
            "       9abc : 1000 0102 0304             cpl    rr0, #01020304h\n");
}

void test_dis_z8001() {
    PREP_DIS(z8000::DisZ8000);

    listing.setUppercase(true);
    EQ("short-direct", OK, disassembler.setOption("short-direct", "on"));

    DIS16("z8001", 0x789abc,
            "      CPU    Z8001\n"
            "      ORG    789ABCH\n"
            "      LDB    |160017H|(R1), #25\n"
            "; test.bin: 789AC2H: error: Registers overlapped\n"
            ";   789AC2 : 9745\n",
            "       0 :                            CPU    Z8001\n"
            "  789ABC :                            ORG    789ABCH\n"
            "  789ABC : 4C15 1617 1919             LDB    |160017H|(R1), #25\n"
            "test.bin: 789AC2H: error: Registers overlapped\n"
            "  789AC2 : 9745                       POP\n",
            0x4c15, 0x1617, 0x1919, 0x9745);

    EQ("org", OVERFLOW_RANGE, listing.setOrigin(0x89abcd));
    EQ("org", OPERAND_NOT_ALIGNED, listing.setOrigin(0x789abd));
}

void test_dis_z8002() {
    PREP_DIS(z8000::DisZ8000);

    listing.setUpperHex(false);

    DIS16("z8002", 0x9abc,
            "      cpu    z8002\n"
            "      org    9abch\n"
            "      cpl    rr0, #01020304h\n"
            "; test.bin: 9ac2h: error: Registers overlapped\n"
            ";     9ac2 : 5144\n",
            "       0 :                            cpu    z8002\n"
            "    9abc :                            org    9abch\n"
            "    9abc : 1000 0102 0304             cpl    rr0, #01020304h\n"
            "test.bin: 9ac2h: error: Registers overlapped\n"
            "    9ac2 : 5144                       pushl\n",
            0x1000, 0x0102, 0x0304, 0x5144);

    EQ("org", OVERFLOW_RANGE, listing.setOrigin(0x10000));
    EQ("org", OPERAND_NOT_ALIGNED, listing.setOrigin(0xABCD));
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

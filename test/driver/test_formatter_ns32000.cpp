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

#include "asm_ns32000.h"
#include "dis_ns32000.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_ns32000() {
    PREP_ASM(ns32000::AsmNs32000, NationalDirective);

    formatter.setUpperHex(false);
    driver.internSymbol(0x112233, "disp2");
    driver.internSymbol(0x334455, "disp1");
    driver.internSymbol(0x556677, "off2");
    driver.internSymbol(0x778899, "off1");

    ASM("ns32032",
            "        .cpu    ns32032\n"
            "# comment line\n"
            "        .org    x'abcdef\n"
            "        extd    r1, disp2(disp1(fp))[r3:w], off2(off1(sb))[r4:w], 32\n",
            "          0 :                            .cpu    ns32032\n"
            "          0 :                    # comment line\n"
            "     abcdef :                            .org    x'abcdef\n"
            // clang-format off
            "     abcdef : 2e 4b ef 83 94 c0          extd    r1, disp2(disp1(fp))[r3:w], off2(off1(sb))[r4:w], 32\n"
            // clang-format on
            "     abcdf5 : 33 44 55 c0 11 22\n"
            "     abcdfb : 33 c0 77 88 99 c0\n"
            "     abce01 : 55 66 77 20\n");
}

void test_dis_ns32000() {
    PREP_DIS(ns32000::DisNs32000);

    formatter.setUpperHex(false);

    DIS8("ns32032", 0xabcdef,
            "      cpu     ns32032\n"
            "      org     x'abcdef\n"
            // clang-format off
            "      extd    r1, x'00112233(x'00334455(fp))[r3:w], x'00556677(x'00778899(sb))[r4:w], 32\n"
            // clang-format on
            "# test.bin: x'abce05: error: Register not aligned\n"
            "#   abce05 : ce ec 08\n",
            "       0 :                            cpu     ns32032\n"
            "  abcdef :                            org     x'abcdef\n"
            // clang-format off
            "  abcdef : 2e 4b ef 83 94 c0          extd    r1, x'00112233(x'00334455(fp))[r3:w], x'00556677(x'00778899(sb))[r4:w], 32\n"
            // clang-format on
            "  abcdf5 : 33 44 55 c0 11 22\n"
            "  abcdfb : 33 c0 77 88 99 c0\n"
            "  abce01 : 55 66 77 20\n"
            "test.bin: x'abce05: error: Register not aligned\n"
            "  abce05 : ce ec 08                   deib    r1, r3\n",
            0x2e, 0x4b, 0xef, 0x83, 0x94, 0xc0, 0x33, 0x44, 0x55, 0xc0, 0x11, 0x22, 0x33, 0xc0,
            0x77, 0x88, 0x99, 0xc0, 0x55, 0x66, 0x77, 0x20, 0xce, 0xec, 0x08);
}

void run_tests() {
    RUN_TEST(test_asm_ns32000);
    RUN_TEST(test_dis_ns32000);
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

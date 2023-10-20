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

#include "asm_i8086.h"
#include "dis_i8086.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_i8086() {
    PREP_ASM(i8086::AsmI8086, IntelDirective);

    TestReader inc("data/db.inc");
    sources.add(inc);
    inc.add("        ds     3\n"                     // DS allocates spaces
            "        db     1\n"                     // DB generates bytes
            "        dd     12345678H, 9abcdef0H\n"  // DD doesn't care alignment
            "        dw     1234H, 5678H, 9abcH\n"   // DW doesn't care alignment
            "        db     'a,', 'bc''de', 0\n"
            "        db     'A', '''', 'C'+80H, 'a''c'\n");

    listing.setUpperHex(false);

    ASM("i8086",
            "        cpu    i8086\n"
            "; comment line\n"
            "        org    0bcdefh\n"
            "        test   word ptr [bp+di+0feffh], 0bbaah\n"
            "        include 'data/db.inc'\n",
            "          0 :                            cpu    i8086\n"
            "          0 :                    ; comment line\n"
            "      bcdef :                            org    0bcdefh\n"
            "      bcdef : f7 83 ff fe aa bb          test   word ptr [bp+di+0feffh], 0bbaah\n"
            "      bcdf5 :                            include 'data/db.inc'\n"
            "(1)   bcdf5 :                            ds     3\n"
            "(1)   bcdf8 : 01                         db     1\n"
            "(1)   bcdf9 : 78 56 34 12 f0 de          dd     12345678H, 9abcdef0H\n"
            "      bcdff : bc 9a\n"
            "(1)   bce01 : 34 12 78 56 bc 9a          dw     1234H, 5678H, 9abcH\n"
            "(1)   bce07 : 61 2c 62 63 27 64          db     'a,', 'bc''de', 0\n"
            "      bce0d : 65 00\n"
            "(1)   bce0f : 41 27 c3 61 27 63          db     'A', '''', 'C'+80H, 'a''c'\n");
}

void test_dis_i8086() {
    PREP_DIS(i8086::DisI8086);

    formatter.setUpperHex(false);

    DIS8("i8086", 0xbcdef,
            "      cpu    i8086\n"
            "      org    0bcdefh\n"
            "      test   word ptr [bp+di+0feffh], 0bbaah\n"
            "; test.bin: 0bcdf5h: error: Unknown instruction\n"
            ";    bcdf5 : d1 f7\n",
            "       0 :                            cpu    i8086\n"
            "   bcdef :                            org    0bcdefh\n"
            "   bcdef : f7 83 ff fe aa bb          test   word ptr [bp+di+0feffh], 0bbaah\n"
            "test.bin: 0bcdf5h: error: Unknown instruction\n"
            "   bcdf5 : d1 f7\n",
            0xf7, 0x83, 0xff, 0xfe, 0xaa, 0xbb, 0xd1, 0xf7);
}

void run_tests() {
    RUN_TEST(test_asm_i8086);
    RUN_TEST(test_dis_i8086);
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

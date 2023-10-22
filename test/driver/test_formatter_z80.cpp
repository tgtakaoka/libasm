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

#include "asm_z80.h"
#include "dis_z80.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_z80() {
    PREP_ASM(z80::AsmZ80, ZilogDirective);

    TestReader inc("data/db.inc");
    sources.add(inc);
    inc.add("        defw  1234H, 5678H, 9ABCH\n"
            "        defb  'a,', 'bc''de', 0\n"  // DB requires surrounding quotes
            "        defs  2\n"                  // DS allocate spaces
            "        defm  'A', '''', 'C'+80H, 'a''c'\n");

    formatter.setUpperHex(false);
    driver.internSymbol(0x8a, "data1");

    ASM("z80",
            "        cpu   z80\n"
            "; comment line\n"
            "        org   0abcdh\n"
            "        include \"data/db.inc\"\n"
            "        res   0, (iy-128)\n"
            "        set   7, (ix+127)\n",
            "          0 :                            cpu   z80\n"
            "          0 :                    ; comment line\n"
            "       abcd :                            org   0abcdh\n"
            "       abcd :                            include \"data/db.inc\"\n"
            "(1)    abcd : 34 12 78 56 bc 9a          defw  1234H, 5678H, 9ABCH\n"
            "(1)    abd3 : 61 2c 62 63 27 64          defb  'a,', 'bc''de', 0\n"
            "       abd9 : 65 00\n"
            "(1)    abdb :                            defs  2\n"
            "(1)    abdd : 41 27 c3 61 27 63          defm  'A', '''', 'C'+80H, 'a''c'\n"
            "       abe3 : fd cb 80 86                res   0, (iy-128)\n"
            "       abe7 : dd cb 7f fe                set   7, (ix+127)\n");
}

void test_dis_z80() {
    PREP_DIS(z80::DisZ80);

    DIS8("z80", 0xabcd,
            "      cpu   z80\n"
            "      org   0ABCDH\n"
            "      res   0, (iy-128)\n"
            "; test.bin: error: Unknown instruction\n"
            ";     ABD1 : DD CB 00 EF\n",
            "       0 :                            cpu   z80\n"
            "    ABCD :                            org   0ABCDH\n"
            "    ABCD : FD CB 80 86                res   0, (iy-128)\n"
            "test.bin: error: Unknown instruction\n"
            "    ABD1 : DD CB 00 EF\n",
            0xfd, 0xcb, 0x80, 0x86, 0xdd, 0xcb, 0x00, 0xef);
}

void run_tests() {
    RUN_TEST(test_asm_z80);
    RUN_TEST(test_dis_z80);
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

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

#include "asm_mc6809.h"
#include "dis_mc6809.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_mc6809() {
    PREP_ASM(mc6809::AsmMc6809, MotorolaDirective);

    TestReader inc("data/fdb.inc");
    sources.add(inc);
    inc.add("        fdb   $1234, $5678, $9abc\n"
            "        fcc   /a,/, /bc''de/\n"              // FCC requires surrounding quotes
            "        fcc   'a,', 'bc\\', \"de'\"\n"       // FCC can accept string
            "        fcb   'a, ',, '/, ' , 0\n"           // FCB can omit closing quote
            "        fcb   'A', '', 'C'+$80, $80+'a\n");  // FCB can accept closing quote

    driver.setUpperHex(true);
    driver.setLineNumber(true);

    ASM("mc6809",
            "        cpu   mc6809\n"
            "* comment line\n"
            "        org   $abcd\n"
            "        cmpd  [$1234,y] ; indirect\n"
            "        include \"data/fdb.inc\"\n"
            "        setdp $ff\n",
            "       1/       0 :                            cpu   mc6809\n"
            "       2/       0 :                    * comment line\n"
            "       3/    ABCD :                            org   $abcd\n"
            "       4/    ABCD : 10 A3 B9 12 34             cmpd  [$1234,y] ; indirect\n"
            "       5/    ABD2 :                            include \"data/fdb.inc\"\n"
            "(1)    1/    ABD2 : 12 34 56 78 9A BC          fdb   $1234, $5678, $9abc\n"
            "(1)    2/    ABD8 : 61 2C 62 63 27 27          fcc   /a,/, /bc''de/\n"
            "             ABDE : 64 65\n"
            "(1)    3/    ABE0 : 61 2C 62 63 5C 64          fcc   'a,', 'bc\\', \"de'\"\n"
            "             ABE6 : 65 27\n"
            "(1)    4/    ABE8 : 61 2C 2F 20 00             fcb   'a, ',, '/, ' , 0\n"
            "(1)    5/    ABED : 41 27 C3 E1                fcb   'A', '', 'C'+$80, $80+'a\n"
            "       6/    ABF1 :                            setdp $ff\n");
}

void test_dis_mc6809() {
    PREP_DIS(mc6809::DisMc6809);

    formatter.setUppercase(false);

    DIS8("mc6809", 0xabcd,
            "      cpu   mc6809\n"
            "      org   $ABCD\n"
            "      cmpd  [$1234,y]\n"
            "* test.bin: error: Unknown instruction\n"
            "*     ABD2 : 01\n"
            "* test.bin: error: Unknown post-byte\n"
            "*     ABD3 : A6 87                      lda\n",
            "       0 :                            cpu   mc6809\n"
            "    ABCD :                            org   $ABCD\n"
            "    ABCD : 10 A3 B9 12 34             cmpd  [$1234,y]\n"
            "test.bin: error: Unknown instruction\n"
            "    ABD2 : 01\n"
            "test.bin: error: Unknown post-byte\n"
            "    ABD3 : A6 87                      lda\n",
            0x10, 0xA3, 0xB9, 0x12, 0x34, 0x01, 0xA6, 0x87);
}

void run_tests() {
    RUN_TEST(test_asm_mc6809);
    RUN_TEST(test_dis_mc6809);
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

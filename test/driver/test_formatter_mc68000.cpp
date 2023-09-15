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

#include "asm_mc68000.h"
#include "dis_mc68000.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_mc68000() {
    PREP_ASM(mc68000::AsmMc68000, MotorolaDirective);

    TestReader inc("data/dc.inc");
    sources.add(inc);
    inc.add("        dc.b    1, 2, 3\n"               // DC.B generates odd bytes at even address
            "        dc.b    4, 5, 6\n"               // DC.B generates odd bytes at odd address
            "        dc.b    7, 8, 9\n"               // DC.B generates odd bytes at even address
            "        dc.w    $1234, $5678, $9abc\n"   // DC.W requires 2 byte alignment
            "        dc.b    1, 2, 3\n"               // DC.B generates odd bytes at even address
            "        dc.l    $12345678, $9abcdef0\n"  // DC.L requires 2 byte alignment
            "        dc.b    1, 2, 3\n"               // DC.B generates odd bytes at even address
            "        ds.b    1\n"                     // DS.B allocates odd spaces at odd address
            "        ds.b    3\n"                     // DS.B allocates odd spaces at even address
            "        ds.b    5\n"                     // DS.B allocates odd spaces at even address
            "        ds.w    2\n"                     // DS.W allocates 2 byte aligned spaces
            "        ds.b    1\n"                     // DS.B allocates odd spaces at even address
            "        ds.l    3\n"                     // DS.L allocates 2 byte aligned spaces
            "        dc.b    'a,', 'bc''de', 4, 0\n"  // DC.B can generate odd bytes at even address
            "        dc.b    'A', '''', 'C'+$80, 'a''c'\n");  // DC.B doesn't care alignment

    listing.setUpperHex(false);

    ASM("mc68000",
            "        cpu     mc68000\n"
            "* comment line\n"
            "        org     $9abcde\n"
            "        ori.l   #$bdbebfc0, ($c2c3c4).l\n"
            "        include \"data/dc.inc\"\n",
            "          0 :                            cpu     mc68000\n"
            "          0 :                    * comment line\n"
            "     9abcde :                            org     $9abcde\n"
            "     9abcde : 00b9 bdbe bfc0             ori.l   #$bdbebfc0, ($c2c3c4).l\n"
            "     9abce4 : 00c2 c3c4\n"
            "     9abce8 :                            include \"data/dc.inc\"\n"
            "(1)  9abce8 : 0102 03                    dc.b    1, 2, 3\n"
            "(1)  9abceb :   04 0506                  dc.b    4, 5, 6\n"
            "(1)  9abcee : 0708 09                    dc.b    7, 8, 9\n"
            "(1)  9abcf2 : 1234 5678 9abc             dc.w    $1234, $5678, $9abc\n"
            "(1)  9abcf8 : 0102 03                    dc.b    1, 2, 3\n"
            "(1)  9abcfc : 1234 5678 9abc             dc.l    $12345678, $9abcdef0\n"
            "     9abd02 : def0\n"
            "(1)  9abd04 : 0102 03                    dc.b    1, 2, 3\n"
            "(1)  9abd07 :                            ds.b    1\n"
            "(1)  9abd08 :                            ds.b    3\n"
            "(1)  9abd0b :                            ds.b    5\n"
            "(1)  9abd10 :                            ds.w    2\n"
            "(1)  9abd14 :                            ds.b    1\n"
            "(1)  9abd16 :                            ds.l    3\n"
            "(1)  9abd22 : 612c 6263 2764             dc.b    'a,', 'bc''de', 4, 0\n"
            "     9abd28 : 6504 00\n"
            "(1)  9abd2b :   41 27c3 6127 63          dc.b    'A', '''', 'C'+$80, 'a''c'\n");
}

void test_dis_mc68000() {
    PREP_DIS(mc68000::DisMc68000);

    listing.setUpperHex(false);

    DIS16("mc68000", 0x9abcde,
            "      cpu     mc68000\n"
            "      org     $9abcde\n"
            "      ori.l   #$bdbebfc0, ($c2c3c4).l\n"
            "; test.bin: $9abce8: error: Operand not allowed\n"
            ";   9abce8 : 43fc\n"
            "; test.bin: $9abcea: error: Operand not aligned\n"
            ";   9abcea : 617f\n",
            "       0 :                            cpu     mc68000\n"
            "  9abcde :                            org     $9abcde\n"
            "  9abcde : 00b9 bdbe bfc0             ori.l   #$bdbebfc0, ($c2c3c4).l\n"
            "  9abce4 : 00c2 c3c4\n"
            "test.bin: $9abce8: error: Operand not allowed\n"
            "  9abce8 : 43fc                       lea\n"
            "test.bin: $9abcea: error: Operand not aligned\n"
            "  9abcea : 617f                       bsr     $9abd6b\n",
            0x00b9, 0xbdbe, 0xbfc0, 0x00c2, 0xc3c4, 0041774, 0060400 | 0x7F);

    EQ("org", OPERAND_NOT_ALIGNED, listing.setOrigin(0xABCDEF));
}

void run_tests() {
    RUN_TEST(test_asm_mc68000);
    RUN_TEST(test_dis_mc68000);
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

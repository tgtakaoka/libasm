/*
 * Copyright 2022 Tadashi G. Takaoka
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

#include "asm_formatter.h"

#include "asm_cdp1802.h"
#include "asm_directive.h"
#include "asm_driver.h"
#include "asm_i8048.h"
#include "asm_i8051.h"
#include "asm_i8080.h"
#include "asm_i8086.h"
#include "asm_i8096.h"
#include "asm_ins8060.h"
#include "asm_ins8070.h"
#include "asm_mc6800.h"
#include "asm_mc68000.h"
#include "asm_mc6805.h"
#include "asm_mc6809.h"
#include "asm_mn1610.h"
#include "asm_mos6502.h"
#include "asm_ns32000.h"
#include "asm_scn2650.h"
#include "asm_tlcs90.h"
#include "asm_tms32010.h"
#include "asm_tms9900.h"
#include "asm_z8.h"
#include "asm_z80.h"
#include "asm_z8000.h"
#include "bin_memory.h"
#include "test_driver_helper.h"
#include "test_sources.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

#define PREP(typeof_asm, typeof_directive)     \
    typeof_asm assembler;                      \
    typeof_directive directive(assembler);     \
    AsmDirective *dir = &directive;            \
    TestSources sources;                       \
    AsmDriver driver(&dir, &dir + 1, sources); \
    BinMemory memory;                          \
    AsmFormatter listing(driver, sources, memory)

#define ASM(_cpu, _source, _expected)                               \
    do {                                                            \
        TestReader expected("expected");                            \
        expected.add(_expected);                                    \
        TestReader source(_cpu);                                    \
        source.add(_source);                                        \
        sources.add(source);                                        \
        sources.open(source.name().c_str());                        \
        StrScanner *line;                                           \
        while ((line = sources.readLine()) != nullptr) {            \
            listing.assemble(*line, /* reportError */ true);        \
            EQ("directive", OK, directive);                         \
            while (listing.hasNextLine())                           \
                EQ("line", expected.readLine(), listing.getLine()); \
        }                                                           \
        EQ(_cpu, nullptr, expected.readLine());                     \
    } while (0)

void test_mc6809() {
    PREP(mc6809::AsmMc6809, MotorolaDirective);

    listing.setUppercase(true);
    listing.enableLineNumber(true);
    driver.internSymbol(0xabd2, "label1");

    ASM("mc6809",
            "        cpu   mc6809\n"
            "        org   $abcd\n"
            "        cmpd  [$1234,y] ; indirect\n"
            "label1: fcc   /abcdefghijklmn/\n"
            "        setdp $ff00",
            "       1/    0:                             CPU   mc6809\n"
            "       2/ ABCD:                             ORG   $abcd\n"
            "       3/ ABCD: 10 A3 B9 12                 CMPD  [$1234,y]       ; indirect\n"
            "       3/ ABD1: 34\n"
            "       4/ ABD2: 61 62 63 64 label1:         FCC   /abcdefghijklmn/\n"
            "       4/ ABD6: 65 66 67 68\n"
            "       4/ ABDA: 69 6A 6B 6C\n"
            "       4/ ABDE: 6D 6E\n"
            "       5/ ABE0:                             SETDP $ff00");
}

void test_mc6800() {
    PREP(mc6800::AsmMc6800, MotorolaDirective);

    listing.setUppercase(false);
    listing.enableLineNumber(false);
    driver.internSymbol(0xf1f2, "label1");

    ASM("mc6800",
            "        cpu   mc6800\n"
            "        org   $abcd\n"
            "        subb  label1",
            "   0:                             cpu   mc6800\n"
            "abcd:                             org   $abcd\n"
            "abcd: f0 f1 f2                    subb  label1");
}

void test_mc6805() {
    PREP(mc6805::AsmMc6805, MotorolaDirective);

    listing.setUppercase(false);
    listing.enableLineNumber(false);
    driver.internSymbol(0x0800, "label1");

    ASM("mc146805",
            "        cpu   mc146805\n"
            "        org   $1234\n"
            "        stx   label1, x",
            "   0:                          cpu   mc146805\n"
            "1234:                          org   $1234\n"
            "1234: df 08 00                 stx   label1, x");
}

void test_mos6502() {
    PREP(mos6502::AsmMos6502, MotorolaDirective);

    listing.setUppercase(false);
    listing.enableLineNumber(false);
    driver.internSymbol(0xf1f2, "label1");

    ASM("mos6502",
            "        cpu   mos6502\n"
            "        org   $abcd\n"
            "        sbc   label1",
            "   0:                             cpu   mos6502\n"
            "abcd:                             org   $abcd\n"
            "abcd: ed f2 f1                    sbc   label1");
}

void test_w65816() {
    PREP(mos6502::AsmMos6502, MotorolaDirective);

    listing.setUppercase(false);
    listing.enableLineNumber(false);
    driver.internSymbol(0xf2f1f0, "label1");

    ASM("w65c816",
            "        cpu   w65c816\n"
            "        org   $abcdef\n"
            "        sbc   label1\n"
            "        longa on\n"
            "        adc   #$1234",
            "     0:                             cpu   w65c816\n"
            "abcdef:                             org   $abcdef\n"
            "abcdef: ef f0 f1 f2                 sbc   label1\n"
            "abcdf3:                             longa on\n"
            "abcdf3: 69 34 12                    adc   #$1234");
}

void test_i8048() {
    PREP(i8048::AsmI8048, IntelDirective);

    listing.setUppercase(false);
    listing.enableLineNumber(false);
    driver.internSymbol(0x8a, "data1");

    ASM("i8039",
            "        cpu   i8039\n"
            "        org   0bcdh\n"
            "        orl   p1, #data1",
            "  0:                       cpu   i8039\n"
            "bcd:                       org   0bcdh\n"
            "bcd: 89 8a                 orl   p1, #data1");
}

void test_i8051() {
    PREP(i8051::AsmI8051, IntelDirective);

    listing.setUppercase(false);
    listing.enableLineNumber(false);
    driver.internSymbol(0xb0, "data1");

    ASM("i8051",
            "        cpu   i8051\n"
            "        org   0abcdh\n"
            "        anl   c, /data1.1\n",
            "   0:                          cpu   i8051\n"
            "abcd:                          org   0abcdh\n"
            "abcd: b0 b1                    anl   c, /data1.1");
}

void test_i8080() {
    PREP(i8080::AsmI8080, IntelDirective);

    listing.setUppercase(false);
    listing.enableLineNumber(false);
    driver.internSymbol(0xebec, "label1");

    ASM("i8080",
            "        cpu   i8080\n"
            "        org   0abcdh\n"
            "        jpe   label1",
            "   0:                          cpu   i8080\n"
            "abcd:                          org   0abcdh\n"
            "abcd: ea ec eb                 jpe   label1");
}

void test_i8096() {
    PREP(i8096::AsmI8096, IntelDirective);

    listing.setUppercase(false);
    listing.enableLineNumber(false);
    driver.internSymbol(0x817b, "label1");

    ASM("i8096",
            "        cpu   i8096\n"
            "        org   0abcdh\n"
            "        mulb  130, label1[124]",
            "   0:                             cpu   i8096\n"
            "abcd:                             org   0abcdh\n"
            "abcd: fe 7f 7d 7b                 mulb  130, label1[124]\n"
            "abd1: 81 82");
}

void test_z80() {
    PREP(z80::AsmZ80, IntelDirective);

    listing.setUppercase(false);
    listing.enableLineNumber(false);
    driver.internSymbol(0x8a, "data1");

    ASM("z80",
            "        cpu   z80\n"
            "        org   0abcdh\n"
            "        res   0, (iy-128)",
            "   0:                             cpu   z80\n"
            "abcd:                             org   0abcdh\n"
            "abcd: fd cb 80 86                 res   0, (iy-128)");
}

void test_z8() {
    PREP(z8::AsmZ8, IntelDirective);

    ASM("z8",
            "        cpu     z8\n"
            "        org     0abcdh\n"
            "        ld      r12, 0c9h(r8)",
            "   0:                             cpu     z8\n"
            "abcd:                             org     0abcdh\n"
            "abcd: c7 c8 c9                    ld      r12, 0c9h(r8)");
}

void test_tlcs90() {
    PREP(tlcs90::AsmTlcs90, IntelDirective);

    ASM("tlcs90",
            "        cpu   tlcs90\n"
            "        org   0abcdh\n"
            "        xor   (hl+a), 0efh",
            "   0:                             cpu   tlcs90\n"
            "abcd:                             org   0abcdh\n"
            "abcd: f7 6d ef                    xor   (hl+a), 0efh");
}

void test_ins8060() {
    PREP(ins8060::AsmIns8060, NationalDirective);

    ASM("ins8060",
            "        cpu   ins8060\n"
            "        org   x'abcd\n"
            "        and   @e(p1)",
            "   0:                       cpu   ins8060\n"
            "abcd:                       org   x'abcd\n"
            "abcd: d5 80                 and   @e(p1)");
}

void test_ins8070() {
    PREP(ins8070::AsmIns8070, NationalDirective);

    ASM("ins8070",
            "        cpu   ins8070\n"
            "        org   x'abcd\n"
            "        pli   p2, =x'2423",
            "   0:                          cpu   ins8070\n"
            "abcd:                          org   x'abcd\n"
            "abcd: 22 23 24                 pli   p2, =x'2423");
}

void test_cdp1802() {
    PREP(cdp1802::AsmCdp1802, IntelDirective);

    ASM("cdp1804",
            "        cpu   cdp1804\n"
            "        org   0abcdh\n"
            "        scal  3, 8485h",
            "   0:                             cpu   cdp1804\n"
            "abcd:                             org   0abcdh\n"
            "abcd: 68 83 84 85                 scal  3, 8485h");

    assembler.setOption("use-register", "on");

    ASM("cdp1804",
            "        org   0abcdh\n"
            "        scal  r3, 8485h",
            "abcd:                             org   0abcdh\n"
            "abcd: 68 83 84 85                 scal  r3, 8485h");
}

void test_scn2650() {
    PREP(scn2650::AsmScn2650, IntelDirective);

    driver.internSymbol(0x7def, "label1");

    ASM("scn2650",
            "        cpu     scn2650\n"
            "        org     07bcdh\n"
            "        loda,r0 *label1, r0, +",
            "   0:                          cpu     scn2650\n"
            "7bcd:                          org     07bcdh\n"
            "7bcd: 0c bd ef                 loda,r0 *label1, r0, +");
}

void test_i8086() {
    PREP(i8086::AsmI8086, IntelDirective);

    ASM("i8086",
            "        cpu    i8086\n"
            "        org    0bcdefh\n"
            "        test   word ptr [bp+di+0feffh], 0bbaah",
            "    0:                             cpu    i8086\n"
            "bcdef:                             org    0bcdefh\n"
            "bcdef: f7 83 ff fe                 test   word ptr [bp+di+0feffh], 0bbaah\n"
            "bcdf3: aa bb");
}

void test_tms9900() {
    PREP(tms9900::AsmTms9900, IntelDirective);

    ASM("tms99105",
            "        cpu   tms99105\n"
            "        org   9abch\n"
            "        am    @4a4bh(r1), @4c4dh(r1)\n",
            "   0:                           cpu   tms99105\n"
            "9abc:                           org   9abch\n"
            "9abc: 002a 4861                 am    @4a4bh(r1), @4c4dh(r1)\n"
            "9ac0: 4a4b 4c4d");
}

void test_tms32010() {
    PREP(tms32010::AsmTms32010, IntelDirective);

    ASM("tms32010",
            "        cpu   tms32010\n"
            "        org   789h\n"
            "        call  0fedh\n"
            "        sacl  *+, 0, ar0",
            "  0:                           cpu   tms32010\n"
            "789:                           org   789h\n"
            "789: f800 0fed                 call  0fedh\n"
            "78b: 50a0                      sacl  *+, 0, ar0");
}

void test_mc68000() {
    PREP(mc68000::AsmMc68000, MotorolaDirective);

    ASM("mc68000",
            "        cpu     mc68000\n"
            "        org     $9abcde\n"
            "        ori.l   #$bdbebfc0, ($c2c3c4).l",
            "     0:                           cpu     mc68000\n"
            "9abcde:                           org     $9abcde\n"
            "9abcde: 00b9 bdbe                 ori     #$bdbebfc0, ($c2c3c4).l\n"
            "9abce2: bfc0 00c2\n"
            "9abce6: c3c4");
}

void test_ns32000() {
    PREP(ns32000::AsmNs32000, NationalDirective);

    ASM("ns32032",
            "        cpu     ns32032\n"
            "        org     x'abcdef\n"
            "        extd    r1, x'00112233(x'00334455(fp))[r3:w], "
            "x'00556677(x'00778899(sb))[r4:w], 32",
            "     0:                             cpu     ns32032\n"
            "abcdef:                             org     x'abcdef\n"
            "abcdef: 2e 4b ef 83                 extd    r1, "
            "x'00112233(x'00334455(fp))[r3:w], "
            "x'00556677(x'00778899(sb))[r4:w], 32\n"
            "abcdf3: 94 c0 33 44\n"
            "abcdf7: 55 c0 11 22\n"
            "abcdfb: 33 c0 77 88\n"
            "abcdff: 99 c0 55 66\n"
            "abce03: 77 20");
}

void test_z8001() {
    PREP(z8000::AsmZ8000, IntelDirective);

    ASM("z8001",
            "        cpu    z8001\n"
            "        org    789abch\n"
            "        ldb    |160017h|(r1), #25",
            "     0:                           cpu    z8001\n"
            "789abc:                           org    789abch\n"
            "789abc: 4c15 1617                 ldb    |160017h|(r1), #25\n"
            "789ac0: 1919");
}

void test_z8002() {
    PREP(z8000::AsmZ8000, IntelDirective);

    ASM("z8002",
            "        cpu    z8002\n"
            "        org    9abch\n"
            "        cpl    rr0, #01020304h",
            "   0:                           cpu    z8002\n"
            "9abc:                           org    9abch\n"
            "9abc: 1000 0102                 cpl    rr0, #01020304h\n"
            "9ac0: 0304");
}

void test_mn1610() {
    PREP(mn1610::AsmMn1610, NationalDirective);

    ASM("mn1610",
            "        cpu   mn1610\n"
            "        org   x'abcd'\n"
            "        tbit  r3, 5, nz",
            "   0:                           cpu   mn1610\n"
            "abcd:                           org   x'abcd'\n"
            "abcd: 2b55                      tbit  r3, 5, nz");
}

void test_mn1613() {
    PREP(mn1610::AsmMn1610, NationalDirective);

    ASM("mn1613",
            "        cpu   mn1613\n"
            "        org   x'34567'\n"
            "        mvwi  str, x'5678', skp",
            "    0:                           cpu   mn1613\n"
            "34567:                           org   x'34567'\n"
            "34567: 7e1f 5678                 mvwi  str, x'5678', skp");
}

void run_tests() {
    RUN_TEST(test_mc6809);
    RUN_TEST(test_mc6800);
    RUN_TEST(test_mc6805);
    RUN_TEST(test_mos6502);
    RUN_TEST(test_w65816);
    RUN_TEST(test_i8048);
    RUN_TEST(test_i8051);
    RUN_TEST(test_i8080);
    RUN_TEST(test_i8096);
    RUN_TEST(test_z80);
    RUN_TEST(test_z8);
    RUN_TEST(test_tlcs90);
    RUN_TEST(test_ins8060);
    RUN_TEST(test_ins8070);
    RUN_TEST(test_cdp1802);
    RUN_TEST(test_scn2650);
    RUN_TEST(test_i8086);
    RUN_TEST(test_tms9900);
    RUN_TEST(test_tms32010);
    RUN_TEST(test_mc68000);
    RUN_TEST(test_ns32000);
    RUN_TEST(test_z8001);
    RUN_TEST(test_z8002);
    RUN_TEST(test_mn1610);
    RUN_TEST(test_mn1613);
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
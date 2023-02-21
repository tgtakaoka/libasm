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
#include "asm_f3850.h"
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
            while (listing.hasNextLine())                           \
                EQ("line", expected.readLine(), listing.getLine()); \
        }                                                           \
        EQ(_cpu, nullptr, expected.readLine());                     \
    } while (0)

void test_mc6809() {
    PREP(mc6809::AsmMc6809, MotorolaDirective);

    TestReader inc("data/fdb.inc");
    sources.add(inc);
    inc.add("        fdb   $1234, $5678, $9abc\n"
            "        fcc   /abcdefghijklmn/\n"
            "        fcb   'A','B','C'+$80\n");

    listing.setUpperHex(true);
    listing.enableLineNumber(true);

    ASM("mc6809",
            "        cpu   mc6809\n"
            "        org   $abcd\n"
            "        cmpd  [$1234,y] ; indirect\n"
            "        include \"data/fdb.inc\"\n"
            "        setdp $ff00\n",
            "       1/       0 :                            cpu   mc6809\n"
            "       2/    ABCD :                            org   $abcd\n"
            "       3/    ABCD : 10 A3 B9 12 34             cmpd  [$1234,y] ; indirect\n"
            "       4/    ABD2 :                            include \"data/fdb.inc\"\n"
            "(1)    1/    ABD2 : 12 34 56 78 9A BC          fdb   $1234, $5678, $9abc\n"
            "(1)    2/    ABD8 : 61 62 63 64 65 66          fcc   /abcdefghijklmn/\n"
            "             ABDE : 67 68 69 6A 6B 6C\n"
            "             ABE4 : 6D 6E\n"
            "(1)    3/    ABE6 : 41 42 C3                   fcb   'A','B','C'+$80\n"
            "       5/    ABE9 :                            setdp $ff00\n");
}

void test_mc6800() {
    PREP(mc6800::AsmMc6800, MotorolaDirective);

    listing.setUpperHex(false);
    driver.internSymbol(0xf1f2, "label1");

    ASM("mc6800",
            "        cpu   mc6800\n"
            "        org   $abcd\n"
            "        subb  label1\n",
            "          0 :                            cpu   mc6800\n"
            "       abcd :                            org   $abcd\n"
            "       abcd : f0 f1 f2                   subb  label1\n");
}

void test_mc6805() {
    PREP(mc6805::AsmMc6805, MotorolaDirective);

    listing.setUpperHex(false);
    driver.internSymbol(0x0800, "label1");

    ASM("mc146805",
            "        cpu   mc146805\n"
            "        org   $1234\n"
            "        stx   label1, x\n",
            "          0 :                            cpu   mc146805\n"
            "       1234 :                            org   $1234\n"
            "       1234 : df 08 00                   stx   label1, x\n");
}

void test_mos6502() {
    PREP(mos6502::AsmMos6502, MotorolaDirective);

    listing.setUpperHex(false);
    driver.internSymbol(0xf1f2, "label1");

    ASM("mos6502",
            "        cpu   mos6502\n"
            "        org   $abcd\n"
            "        sbc   label1\n",
            "          0 :                            cpu   mos6502\n"
            "       abcd :                            org   $abcd\n"
            "       abcd : ed f2 f1                   sbc   label1\n");
}

void test_w65816() {
    PREP(mos6502::AsmMos6502, MotorolaDirective);

    listing.setUpperHex(false);
    driver.internSymbol(0xf2f1f0, "label1");

    ASM("w65c816",
            "        cpu   w65c816\n"
            "        org   $abcdef\n"
            "        sbc   label1\n"
            "        longa on\n"
            "        adc   #$1234\n",
            "          0 :                            cpu   w65c816\n"
            "     abcdef :                            org   $abcdef\n"
            "     abcdef : ef f0 f1 f2                sbc   label1\n"
            "     abcdf3 :                            longa on\n"
            "     abcdf3 : 69 34 12                   adc   #$1234\n");
}

void test_i8048() {
    PREP(i8048::AsmI8048, IntelDirective);

    listing.setUpperHex(false);
    driver.internSymbol(0x8a, "data1");

    ASM("i8039",
            "        cpu   i8039\n"
            "        org   0bcdh\n"
            "        orl   p1, #data1\n",
            "          0 :                            cpu   i8039\n"
            "        bcd :                            org   0bcdh\n"
            "        bcd : 89 8a                      orl   p1, #data1\n");
}

void test_i8051() {
    PREP(i8051::AsmI8051, IntelDirective);

    listing.setUpperHex(false);
    driver.internSymbol(0xb0, "data1");

    ASM("i8051",
            "        cpu   i8051\n"
            "        org   0abcdh\n"
            "        anl   c, /data1.1\n",
            "          0 :                            cpu   i8051\n"
            "       abcd :                            org   0abcdh\n"
            "       abcd : b0 b1                      anl   c, /data1.1\n");
}

void test_i8080() {
    PREP(i8080::AsmI8080, IntelDirective);

    listing.setUpperHex(false);
    driver.internSymbol(0xebec, "label1");

    ASM("i8080",
            "        cpu   i8080\n"
            "        org   0abcdh\n"
            "        jpe   label1\n",
            "          0 :                            cpu   i8080\n"
            "       abcd :                            org   0abcdh\n"
            "       abcd : ea ec eb                   jpe   label1\n");
}

void test_i8096() {
    PREP(i8096::AsmI8096, IntelDirective);

    listing.setUpperHex(false);
    driver.internSymbol(0x817b, "label1");

    ASM("i8096",
            "        cpu   i8096\n"
            "        org   0abcdh\n"
            "        mulb  130, label1[124]\n",
            "          0 :                            cpu   i8096\n"
            "       abcd :                            org   0abcdh\n"
            "       abcd : fe 7f 7d 7b 81 82          mulb  130, label1[124]\n");
}

void test_z80() {
    PREP(z80::AsmZ80, IntelDirective);

    TestReader inc("data/db.inc");
    sources.add(inc);
    inc.add("        dw    1234H, 5678H, 9ABCH\n"
            "        db    'a','b','c','d','e','f','g',0\n"
            "        db    'A','B','C'+80H\n");

    listing.setUpperHex(false);
    driver.internSymbol(0x8a, "data1");

    ASM("z80",
            "        cpu   z80\n"
            "        org   0abcdh\n"
            "        include \"data/db.inc\"\n"
            "        res   0, (iy-128)\n",
            "          0 :                            cpu   z80\n"
            "       abcd :                            org   0abcdh\n"
            "       abcd :                            include \"data/db.inc\"\n"
            "(1)    abcd : 34 12 78 56 bc 9a          dw    1234H, 5678H, 9ABCH\n"
            "(1)    abd3 : 61 62 63 64 65 66          db    'a','b','c','d','e','f','g',0\n"
            "       abd9 : 67 00\n"
            "(1)    abdb : 41 42 c3                   db    'A','B','C'+80H\n"
            "       abde : fd cb 80 86                res   0, (iy-128)\n");
}

void test_z8() {
    PREP(z8::AsmZ8, IntelDirective);

    listing.setUpperHex(false);

    ASM("z8",
            "        cpu     z8\n"
            "        org     0abcdh\n"
            "        ld      r12, 0c9h(r8)\n",
            "          0 :                            cpu     z8\n"
            "       abcd :                            org     0abcdh\n"
            "       abcd : c7 c8 c9                   ld      r12, 0c9h(r8)");
}

void test_tlcs90() {
    PREP(tlcs90::AsmTlcs90, IntelDirective);

    listing.setUpperHex(false);

    ASM("tlcs90",
            "        cpu   tlcs90\n"
            "        org   0abcdh\n"
            "        xor   (hl+a), 0efh\n",
            "          0 :                            cpu   tlcs90\n"
            "       abcd :                            org   0abcdh\n"
            "       abcd : f7 6d ef                   xor   (hl+a), 0efh\n");
}

void test_ins8060() {
    PREP(ins8060::AsmIns8060, NationalDirective);

    ASM("ins8060",
            "        cpu   ins8060\n"
            "label:  org   x'abcd\n"
            "        and   @e(p1)\n"
            "        db    h(label)\n"
            "        db    l(label)\n"
            "        dw    addr(label)\n"
            "        dw    addr(x'1000)\n",
            "          0 :                            cpu   ins8060\n"
            "       ABCD :                    label:  org   x'abcd\n"
            "       ABCD : D5 80                      and   @e(p1)\n"
            "       ABCF : AB                         db    h(label)\n"
            "       ABD0 : CD                         db    l(label)\n"
            "       ABD1 : CC AB                      dw    addr(label)\n"
            "       ABD3 : FF 1F                      dw    addr(x'1000)\n");
}

void test_ins8070() {
    PREP(ins8070::AsmIns8070, NationalDirective);

    listing.setUpperHex(false);

    ASM("ins8070",
            "        cpu   ins8070\n"
            "label:  org   x'abcd\n"
            "        pli   p2, =x'2423\n"
            "        db    h(label)\n"
            "        db    l(label)\n"
            "        dw    addr(label)\n"
            "        dw    addr(x'1000)\n",
            "          0 :                            cpu   ins8070\n"
            "       abcd :                    label:  org   x'abcd\n"
            "       abcd : 22 23 24                   pli   p2, =x'2423\n"
            "       abd0 : ab                         db    h(label)\n"
            "       abd1 : cd                         db    l(label)\n"
            "       abd2 : cc ab                      dw    addr(label)\n"
            "       abd4 : ff 0f                      dw    addr(x'1000)\n");
}

void test_cdp1802() {
    PREP(cdp1802::AsmCdp1802, IntelDirective);

    listing.setUpperHex(false);

    ASM("cdp1804",
            "        cpu   cdp1804\n"
            "        org   0abcdh\n"
            "        scal  3, 8485h\n",
            "          0 :                            cpu   cdp1804\n"
            "       abcd :                            org   0abcdh\n"
            "       abcd : 68 83 84 85                scal  3, 8485h\n");

    assembler.setOption("use-register", "on");

    ASM("cdp1804",
            "        org   0abcdh\n"
            "        scal  r3, 8485h\n",
            "       abcd :                            org   0abcdh\n"
            "       abcd : 68 83 84 85                scal  r3, 8485h\n");
}

void test_scn2650() {
    PREP(scn2650::AsmScn2650, IntelDirective);

    driver.internSymbol(0x7def, "label1");

    ASM("scn2650",
            "        cpu     scn2650\n"
            "        org     07bcdh\n"
            "        loda,r0 *label1, r0, +\n",
            "          0 :                            cpu     scn2650\n"
            "       7BCD :                            org     07bcdh\n"
            "       7BCD : 0C BD EF                   loda,r0 *label1, r0, +\n");
}

void test_f3850() {
    PREP(f3850::AsmF3850, FairchildDirective);

    TestReader inc("data/db.inc");
    sources.add(inc);
    inc.add("        dc    h'1234', h'5678', h'9abc'\n");

    driver.internSymbol(0x7bcf, "label1");

    ASM("f3850",
            "        cpu   F3850\n"
            "        org   H'7BCD'\n"
            "        clr\n"
            "        lr    a, j\n"
            "        bp    label1\n"
            "        include \"data/db.inc\"\n",
            "          0 :                            cpu   F3850\n"
            "       7BCD :                            org   H'7BCD'\n"
            "       7BCD : 70                         clr\n"
            "       7BCE : 49                         lr    a, j\n"
            "       7BCF : 81 FF                      bp    label1\n"
            "       7BD1 :                            include \"data/db.inc\"\n"
            "(1)    7BD1 : 12 34 56 78 9A BC          dc    h'1234', h'5678', h'9abc'\n");
}

void test_i8086() {
    PREP(i8086::AsmI8086, IntelDirective);

    listing.setUpperHex(false);

    ASM("i8086",
            "        cpu    i8086\n"
            "        org    0bcdefh\n"
            "        test   word ptr [bp+di+0feffh], 0bbaah\n",
            "          0 :                            cpu    i8086\n"
            "      bcdef :                            org    0bcdefh\n"
            "      bcdef : f7 83 ff fe aa bb          test   word ptr [bp+di+0feffh], 0bbaah\n");
}

void test_tms9900() {
    PREP(tms9900::AsmTms9900, IntelDirective);

    listing.setUpperHex(false);

    ASM("tms99105",
            "        cpu   tms99105\n"
            "        org   9abch\n"
            "        am    @4a4bh(r1), @4c4dh(r1)\n",
            "          0 :                            cpu   tms99105\n"
            "       9abc :                            org   9abch\n"
            "       9abc : 002a 4861 4a4b             am    @4a4bh(r1), @4c4dh(r1)\n"
            "       9ac2 : 4c4d\n");
}

void test_tms32010() {
    PREP(tms32010::AsmTms32010, IntelDirective);

    listing.setUpperHex(false);

    ASM("tms32010",
            "        cpu   tms32010\n"
            "        org   789h\n"
            "        call  0fedh\n"
            "        sacl  *+, 0, ar0\n",
            "          0 :                            cpu   tms32010\n"
            "        789 :                            org   789h\n"
            "        789 : f800 0fed                  call  0fedh\n"
            "        78b : 50a0                       sacl  *+, 0, ar0\n");
}

void test_mc68000() {
    PREP(mc68000::AsmMc68000, MotorolaDirective);

    listing.setUpperHex(false);

    ASM("mc68000",
            "        cpu     mc68000\n"
            "        org     $9abcde\n"
            "        ori.l   #$bdbebfc0, ($c2c3c4).l\n",
            "          0 :                            cpu     mc68000\n"
            "     9abcde :                            org     $9abcde\n"
            "     9abcde : 00b9 bdbe bfc0             ori.l   #$bdbebfc0, ($c2c3c4).l\n"
            "     9abce4 : 00c2 c3c4\n");
}

void test_ns32000() {
    PREP(ns32000::AsmNs32000, NationalDirective);

    listing.setUpperHex(false);
    driver.internSymbol(0x112233, "disp2");
    driver.internSymbol(0x334455, "disp1");
    driver.internSymbol(0x556677, "off2");
    driver.internSymbol(0x778899, "off1");

    ASM("ns32032",
            "        cpu     ns32032\n"
            "        org     x'abcdef\n"
            "        extd    r1, disp2(disp1(fp))[r3:w], off2(off1(sb))[r4:w], 32\n",
            "          0 :                            cpu     ns32032\n"
            "     abcdef :                            org     x'abcdef\n"
            // clang-format off
            "     abcdef : 2e 4b ef 83 94 c0          extd    r1, disp2(disp1(fp))[r3:w], off2(off1(sb))[r4:w], 32\n"
            // clang-format on
            "     abcdf5 : 33 44 55 c0 11 22\n"
            "     abcdfb : 33 c0 77 88 99 c0\n"
            "     abce01 : 55 66 77 20\n");
}

void test_z8001() {
    PREP(z8000::AsmZ8000, IntelDirective);

    listing.setUpperHex(false);

    ASM("z8001",
            "        cpu    z8001\n"
            "        org    789abch\n"
            "        ldb    |160017h|(r1), #25\n",
            "          0 :                            cpu    z8001\n"
            "     789abc :                            org    789abch\n"
            "     789abc : 4c15 1617 1919             ldb    |160017h|(r1), #25\n");
}

void test_z8002() {
    PREP(z8000::AsmZ8000, IntelDirective);

    listing.setUpperHex(false);

    ASM("z8002",
            "        cpu    z8002\n"
            "        org    9abch\n"
            "        cpl    rr0, #01020304h\n",
            "          0 :                            cpu    z8002\n"
            "       9abc :                            org    9abch\n"
            "       9abc : 1000 0102 0304             cpl    rr0, #01020304h\n");
}

void test_mn1610() {
    PREP(mn1610::AsmMn1610, NationalDirective);

    ASM("mn1610",
            "        cpu   mn1610\n"
            "        org   x'abcd'\n"
            "        tbit  r3, 5, nz\n",
            "          0 :                            cpu   mn1610\n"
            "       ABCD :                            org   x'abcd'\n"
            "       ABCD : 2B55                       tbit  r3, 5, nz\n");
}

void test_mn1613() {
    PREP(mn1610::AsmMn1610, NationalDirective);

    listing.setUpperHex(false);

    ASM("mn1613",
            "        cpu   mn1613\n"
            "        org   x'34567'\n"
            "        mvwi  str, x'5678', skp\n",
            "          0 :                            cpu   mn1613\n"
            "      34567 :                            org   x'34567'\n"
            "      34567 : 7e1f 5678                  mvwi  str, x'5678', skp\n");
}

void test_switch_cpu() {
    mc6809::AsmMc6809 asm6809;
    MotorolaDirective dir6809(asm6809);
    mos6502::AsmMos6502 asm6502;
    MotorolaDirective dir6502(asm6502);
    i8080::AsmI8080 asm8080;
    IntelDirective dir8080(asm8080);
    z80::AsmZ80 asmz80;
    IntelDirective dirz80(asmz80);
    AsmDirective *dirs[] = {&dir6809, &dir6502, &dir8080, &dirz80};
    TestSources sources;
    AsmDriver driver(&dirs[0], &dirs[4], sources);
    BinMemory memory;
    AsmFormatter listing(driver, sources, memory);

    ASM("switch cpu",
            "        cpu   mc6809\n"
            "        org   $1000\n"
            "        ldx   #$1234\n"
            "        cpu   z80\n"
            "        ld    hl, 1234H\n"
            "        cpu   i8080\n"
            "        lxi   d, 5678H\n"
            "        z80syntax on\n"
            "        ld    bc, 9abcH\n"
            "        cpu   65816\n"
            "        longi on\n"
            "        org   $123456\n"
            "        ldx   #$1234\n",
            "          0 :                            cpu   mc6809\n"
            "       1000 :                            org   $1000\n"
            "       1000 : 8E 12 34                   ldx   #$1234\n"
            "       1003 :                            cpu   z80\n"
            "       1003 : 21 34 12                   ld    hl, 1234H\n"
            "       1006 :                            cpu   i8080\n"
            "       1006 : 11 78 56                   lxi   d, 5678H\n"
            "       1009 :                            z80syntax on\n"
            "       1009 : 01 BC 9A                   ld    bc, 9abcH\n"
            "       100C :                            cpu   65816\n"
            "       100C :                            longi on\n"
            "     123456 :                            org   $123456\n"
            "     123456 : A2 34 12                   ldx   #$1234\n");
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
    RUN_TEST(test_f3850);
    RUN_TEST(test_i8086);
    RUN_TEST(test_tms9900);
    RUN_TEST(test_tms32010);
    RUN_TEST(test_mc68000);
    RUN_TEST(test_ns32000);
    RUN_TEST(test_z8001);
    RUN_TEST(test_z8002);
    RUN_TEST(test_mn1610);
    RUN_TEST(test_mn1613);
    RUN_TEST(test_switch_cpu);
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

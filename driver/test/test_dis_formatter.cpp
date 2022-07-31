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

#include "dis_formatter.h"

#include "array_memory.h"
#include "dis_cdp1802.h"
#include "dis_i8048.h"
#include "dis_i8051.h"
#include "dis_i8080.h"
#include "dis_i8086.h"
#include "dis_i8096.h"
#include "dis_ins8060.h"
#include "dis_ins8070.h"
#include "dis_mc6800.h"
#include "dis_mc68000.h"
#include "dis_mc6805.h"
#include "dis_mc6809.h"
#include "dis_mn1610.h"
#include "dis_mos6502.h"
#include "dis_ns32000.h"
#include "dis_scn2650.h"
#include "dis_tlcs90.h"
#include "dis_tms32010.h"
#include "dis_tms9900.h"
#include "dis_z8.h"
#include "dis_z80.h"
#include "dis_z8000.h"
#include "test_driver_helper.h"
#include "test_reader.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

#define PREP(typeof_disassembler)     \
    typeof_disassembler disassembler; \
    DisFormatter listing(disassembler, "test");

#define DIS_LINE(_msg, _expected)                                     \
    do {                                                              \
        if (listing.hasNextLine())                                    \
            EQ(_msg, _expected.readLine()->str(), listing.getLine()); \
    } while (0)

#define DIS(_cpu, _org, _contents, _expected, _memory)                  \
    do {                                                                \
        TestReader contents(_cpu);                                      \
        contents.add(_contents);                                        \
        TestReader expected(_cpu);                                      \
        expected.add(_expected);                                        \
        TRUE("setcpu" _cpu, listing.setCpu(_cpu));                      \
        EQ("setcpu", 0, listing.byteLength());                          \
        FALSE("setcpu", listing.isError());                             \
        EQ("setcpu", contents.readLine()->str(), listing.getContent()); \
        EQ("setcpu", expected.readLine()->str(), listing.getLine());    \
        EQ("origin", OK, listing.setOrigin(_org));                      \
        EQ("origin", contents.readLine()->str(), listing.getContent()); \
        EQ("origin", expected.readLine()->str(), listing.getLine());    \
        auto reader = _memory.iterator();                               \
        EQ("dis", OK, listing.disassemble(reader, _org));               \
        EQ("dis", contents.readLine()->str(), listing.getContent());    \
        FALSE("dis", listing.hasNextContent());                         \
        DIS_LINE("dis1", expected);                                     \
        DIS_LINE("dis2", expected);                                     \
        DIS_LINE("dis3", expected);                                     \
        DIS_LINE("dis4", expected);                                     \
        DIS_LINE("dis5", expected);                                     \
        DIS_LINE("dis6", expected);                                     \
        FALSE("dis", listing.hasNextLine());                            \
    } while (0)

#define DIS16(_cpu, _org, _contents, _expected, ...)                            \
    do {                                                                        \
        const auto unit = disassembler.config().addressUnit();                  \
        const auto endian = disassembler.config().endian();                     \
        const uint16_t _memory[] = {__VA_ARGS__};                               \
        const ArrayMemory memory(_org* unit, _memory, sizeof(_memory), endian); \
        DIS(_cpu, _org, _contents, _expected, memory);                          \
    } while (0)

#define DIS8(_cpu, _org, _contents, _expected, ...)               \
    do {                                                          \
        const uint8_t _memory[] = {__VA_ARGS__};                  \
        const ArrayMemory memory(_org, _memory, sizeof(_memory)); \
        DIS(_cpu, _org, _contents, _expected, memory);            \
    } while (0)

void test_mc6809() {
    PREP(mc6809::DisMc6809);

    listing.setUppercase(true);

    DIS8("mc6809", 0xabcd,
            "        CPU   MC6809\n"
            "        ORG   $ABCD\n"
            "        CMPD  [$1234,Y]",
            "   0:                     CPU   MC6809\n"
            "ABCD:                     ORG   $ABCD\n"
            "ABCD: 10 A3 B9 12         CMPD  [$1234,Y]\n"
            "ABD1: 34",
            0x10, 0xA3, 0xB9, 0x12, 0x34);
}

void test_mc6800() {
    PREP(mc6800::DisMc6800);

    DIS8("mc6800", 0xabcd,
            "        cpu   mc6800\n"
            "        org   $abcd\n"
            "        subb  $f1f2",
            "   0:                     cpu   mc6800\n"
            "abcd:                     org   $abcd\n"
            "abcd: f0 f1 f2            subb  $f1f2",
            0xf0, 0xf1, 0xf2);
}

void test_mc6805() {
    PREP(mc6805::DisMc6805);

    DIS8("mc146805", 0x1234,
            "        cpu   mc146805\n"
            "        org   $1234\n"
            "        stx   16384,x",
            "   0:                  cpu   mc146805\n"
            "1234:                  org   $1234\n"
            "1234: df 40 00         stx   16384,x",
            0xdf, 0x40, 0x00);
    EQ("org", OVERFLOW_RANGE, listing.setOrigin(0x3456));
}

void test_mos6502() {
    PREP(mos6502::DisMos6502);

    DIS8("mos6502", 0xabcd,
            "        cpu   mos6502\n"
            "        org   $abcd\n"
            "        sbc   $f1f2",
            "   0:                     cpu   mos6502\n"
            "abcd:                     org   $abcd\n"
            "abcd: ed f2 f1            sbc   $f1f2",
            0xed, 0xf2, 0xf1);
}

void test_w65816() {
    PREP(mos6502::DisMos6502);

    listing.setUppercase(true);
    disassembler.setOption("longa", "on");

    DIS8("w65c816", 0xabcdef,
            "        CPU   W65C816\n"
            "        ORG   $ABCDEF\n"
            "        SBC   $F2F1F0\n"
            "        ADC   #$1234",
            "     0:                     CPU   W65C816\n"
            "ABCDEF:                     ORG   $ABCDEF\n"
            "ABCDEF: EF F0 F1 F2         SBC   $F2F1F0\n"
            "ABCDF3: 69 34 12            ADC   #$1234",
            0xef, 0xf0, 0xf1, 0xf2, 0x69, 0x34, 0x12);
}

void test_i8048() {
    PREP(i8048::DisI8048);

    listing.setUppercase(true);

    DIS8("i8039", 0xbcd,
            "        CPU   I8039\n"
            "        ORG   0BCDH\n"
            "        ORL   P1, #8AH",
            "  0:               CPU   I8039\n"
            "BCD:               ORG   0BCDH\n"
            "BCD: 89 8A         ORL   P1, #8AH",
            0x89, 0x8A);
    EQ("org", OVERFLOW_RANGE, listing.setOrigin(0xabcd));
}

void test_i8051() {
    PREP(i8051::DisI8051);

    DIS8("i8051", 0xabcd,
            "        cpu   i8051\n"
            "        org   0abcdh\n"
            "        anl   c, /0b0h.1\n",
            "   0:                  cpu   i8051\n"
            "abcd:                  org   0abcdh\n"
            "abcd: b0 b1            anl   c, /0b0h.1",
            0xb0, 0xb1);
}

void test_i8080() {
    PREP(i8080::DisI8080);

    DIS8("i8080", 0xabcd,
            "        cpu   i8080\n"
            "        org   0abcdh\n"
            "        jpe   0ecebh",
            "   0:                  cpu   i8080\n"
            "abcd:                  org   0abcdh\n"
            "abcd: ea eb ec         jpe   0ecebh",
            0xea, 0xeb, 0xec);
}

void test_i8096() {
    PREP(i8096::DisI8096);

    DIS8("i8096", 0xabcd,
            "        cpu   i8096\n"
            "        org   0abcdh\n"
            "        mulb  130, 817bh[124]",
            "   0:                     cpu   i8096\n"
            "abcd:                     org   0abcdh\n"
            "abcd: fe 7f 7d 7b         mulb  130, 817bh[124]\n"
            "abd1: 81 82",
            0xfe, 0x7f, 0x7d, 0x7b, 0x81, 0x82);
}

void test_z80() {
    PREP(z80::DisZ80);

    DIS8("z80", 0xabcd,
            "        cpu   z80\n"
            "        org   0abcdh\n"
            "        res   0, (iy-128)",
            "   0:                     cpu   z80\n"
            "abcd:                     org   0abcdh\n"
            "abcd: fd cb 80 86         res   0, (iy-128)",
            0xfd, 0xcb, 0x80, 0x86);
}

void test_z8() {
    PREP(z8::DisZ8);

    DIS8("z8", 0xabcd,
            "        cpu     z8\n"
            "        org     0abcdh\n"
            "        ld      r12, 0c9h(r8)",
            "   0:                     cpu     z8\n"
            "abcd:                     org     0abcdh\n"
            "abcd: c7 c8 c9            ld      r12, 0c9h(r8)",
            0xc7, 0xc8, 0xc9);
}

void test_tlcs90() {
    PREP(tlcs90::DisTlcs90);

    DIS8("tlcs90", 0xabcd,
            "        cpu   tlcs90\n"
            "        org   0abcdh\n"
            "        xor   (hl+a), 0efh",
            "   0:                     cpu   tlcs90\n"
            "abcd:                     org   0abcdh\n"
            "abcd: f7 6d ef            xor   (hl+a), 0efh",
            0xf7, 0x6d, 0xef);
}

void test_ins8060() {
    PREP(ins8060::DisIns8060);

    DIS8("ins8060", 0xabcd,
            "        cpu   ins8060\n"
            "        org   x'abcd\n"
            "        and   @e(p1)",
            "   0:               cpu   ins8060\n"
            "abcd:               org   x'abcd\n"
            "abcd: d5 80         and   @e(p1)",
            0xd5, 0x80);
}

void test_ins8070() {
    PREP(ins8070::DisIns8070);

    DIS8("ins8070", 0xabcd,
            "        cpu   ins8070\n"
            "        org   x'abcd\n"
            "        pli   p2, =x'2423",
            "   0:                  cpu   ins8070\n"
            "abcd:                  org   x'abcd\n"
            "abcd: 22 23 24         pli   p2, =x'2423",
            0x22, 0x23, 0x24);
}

void test_cdp1802() {
    PREP(cdp1802::DisCdp1802);

    disassembler.setOption("use-register", "on");

    DIS8("cdp1804", 0xabcd,
            "        cpu   cdp1804\n"
            "        org   0abcdh\n"
            "        scal  r3, 8485h",
            "   0:                     cpu   cdp1804\n"
            "abcd:                     org   0abcdh\n"
            "abcd: 68 83 84 85         scal  r3, 8485h",
            0x68, 0x83, 0x84, 0x85);
}

void test_scn2650() {
    PREP(scn2650::DisScn2650);

    DIS8("scn2650", 0x7bcd,
            "        cpu     scn2650\n"
            "        org     7bcdh\n"
            "        loda,r0 *7defh, r0, +",
            "   0:                  cpu     scn2650\n"
            "7bcd:                  org     7bcdh\n"
            "7bcd: 0c bd ef         loda,r0 *7defh, r0, +",
            0x0C, 0xBD, 0xEF);
    EQ("org", OVERFLOW_RANGE, listing.setOrigin(0x89ab));
}

void test_i8086() {
    PREP(i8086::DisI8086);

    DIS8("i8086", 0xbcdef,
            "        cpu    i8086\n"
            "        org    0bcdefh\n"
            "        test   word ptr [bp+di+0feffh], 0bbaah",
            "    0:                     cpu    i8086\n"
            "bcdef:                     org    0bcdefh\n"
            "bcdef: f7 83 ff fe         test   word ptr [bp+di+0feffh], 0bbaah\n"
            "bcdf3: aa bb",
            0xf7, 0x83, 0xff, 0xfe, 0xaa, 0xbb);
}

void test_tms9900() {
    PREP(tms9900::DisTms9900);

    DIS16("tms99105", 0x9abc,
            "        cpu   tms99105\n"
            "        org   9abch\n"
            "        am    @4a4bh(r1), @4c4dh(r1)\n",
            "   0:                   cpu   tms99105\n"
            "9abc:                   org   9abch\n"
            "9abc: 002a 4861         am    @4a4bh(r1), @4c4dh(r1)\n"
            "9ac0: 4a4b 4c4d",
            0x002a, 0x4861, 0x4a4b, 0x4c4d);
    EQ("org", INSTRUCTION_NOT_ALIGNED, listing.setOrigin(0xABCD));
}

void test_tms32010() {
    PREP(tms32010::DisTms32010);

    DIS16("tms32010", 0x789,
            "        cpu   tms32010\n"
            "        org   789h\n"
            "        call  0fedh\n"
            "        sacl  *+, 0, ar0",
            "  0:                   cpu   tms32010\n"
            "789:                   org   789h\n"
            "789: f800 0fed         call  0fedh\n"
            "78b: 50a0              sacl  *+, 0, ar0",
            0xf800, 0x0fed, 0x50a0);
    EQ("org", OVERFLOW_RANGE, listing.setOrigin(0xabcd));
}

void test_mc68000() {
    PREP(mc68000::DisMc68000);

    DIS16("mc68000", 0x9abcde,
            "        cpu     mc68000\n"
            "        org     $9abcde\n"
            "        ori.l   #$bdbebfc0, ($c2c3c4).l",
            "     0:                   cpu     mc68000\n"
            "9abcde:                   org     $9abcde\n"
            "9abcde: 00b9 bdbe         ori.l   #$bdbebfc0, ($c2c3c4).l\n"
            "9abce2: bfc0 00c2\n"
            "9abce6: c3c4",
            0x00b9, 0xbdbe, 0xbfc0, 0x00c2, 0xc3c4);
    EQ("org", INSTRUCTION_NOT_ALIGNED, listing.setOrigin(0xABCDEF));
}

void test_ns32000() {
    PREP(ns32000::DisNs32000);

    DIS8("ns32032", 0xabcdef,
            "        cpu     ns32032\n"
            "        org     x'abcdef\n"
            "        extd    r1, x'00112233(x'00334455(fp))[r3:w], "
            "x'00556677(x'00778899(sb))[r4:w], 32",
            "     0:                     cpu     ns32032\n"
            "abcdef:                     org     x'abcdef\n"
            "abcdef: 2e 4b ef 83         extd    r1, x'00112233(x'00334455(fp))[r3:w], "
            "x'00556677(x'00778899(sb))[r4:w], 32\n"
            "abcdf3: 94 c0 33 44\n"
            "abcdf7: 55 c0 11 22\n"
            "abcdfb: 33 c0 77 88\n"
            "abcdff: 99 c0 55 66\n"
            "abce03: 77 20",
            0x2e, 0x4b, 0xef, 0x83, 0x94, 0xc0, 0x33, 0x44, 0x55, 0xc0, 0x11, 0x22, 0x33, 0xc0,
            0x77, 0x88, 0x99, 0xc0, 0x55, 0x66, 0x77, 0x20);
}

void test_z8001() {
    PREP(z8000::DisZ8000);

    listing.setUppercase(true);
    disassembler.setOption("short-address", "on");

    DIS16("z8001", 0x789abc,
            "        CPU    Z8001\n"
            "        ORG    789ABCH\n"
            "        LDB    |160017H|(R1), #25",
            "     0:                   CPU    Z8001\n"
            "789ABC:                   ORG    789ABCH\n"
            "789ABC: 4C15 1617         LDB    |160017H|(R1), #25\n"
            "789AC0: 1919",
            0x4c15, 0x1617, 0x1919);
    EQ("org", OVERFLOW_RANGE, listing.setOrigin(0x89abcd));
    EQ("org", INSTRUCTION_NOT_ALIGNED, listing.setOrigin(0x789abd));
}

void test_z8002() {
    PREP(z8000::DisZ8000);

    DIS16("z8002", 0x9abc,
            "        cpu    z8002\n"
            "        org    9abch\n"
            "        cpl    rr0, #01020304h",
            "   0:                   cpu    z8002\n"
            "9abc:                   org    9abch\n"
            "9abc: 1000 0102         cpl    rr0, #01020304h\n"
            "9ac0: 0304",
            0x1000, 0x0102, 0x0304);
    EQ("org", OVERFLOW_RANGE, listing.setOrigin(0x10000));
    EQ("org", INSTRUCTION_NOT_ALIGNED, listing.setOrigin(0xABCD));
}

void test_mn1610() {
    PREP(mn1610::DisMn1610);

    DIS16("mn1610", 0xabcd,
            "        cpu   mn1610\n"
            "        org   x'abcd'\n"
            "        tbit  r3, 5, nz",
            "   0:                   cpu   mn1610\n"
            "abcd:                   org   x'abcd'\n"
            "abcd: 2b55              tbit  r3, 5, nz",
            0x2b55);
    EQ("org", OVERFLOW_RANGE, listing.setOrigin(0x12345));
}

void test_mn1613() {
    PREP(mn1610::DisMn1610);

    DIS16("mn1613", 0x34567,
            "        cpu   mn1613\n"
            "        org   x'34567'\n"
            "        mvwi  str, x'5678', skp",
            "    0:                   cpu   mn1613\n"
            "34567:                   org   x'34567'\n"
            "34567: 7e1f 5678         mvwi  str, x'5678', skp",
            0x7e1f, 0x5678);
    EQ("org", OVERFLOW_RANGE, listing.setOrigin(0x45678));
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

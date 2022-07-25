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
#include "test_memory.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_mc6809() {
    mc6809::DisMc6809 dis;
    DisFormatter listing(dis, "test");
    listing.setUppercase(true);

    TRUE("setcpu", listing.setCpu("mc6809"));
    EQ("bytes", 0, listing.byteLength());
    FALSE("error", listing.isError());

    EQ("content", "        CPU   MC6809", listing.getContent());
    EQ("line", "   0:                     CPU   MC6809", listing.getLine());
    FALSE("content", listing.hasNextContent());
    FALSE("line", listing.hasNextLine());

    const uint32_t org = 0xabcd;
    EQ("org", OK, listing.setOrigin(org));
    EQ("content", "        ORG   $ABCD", listing.getContent());
    EQ("line", "ABCD:                     ORG   $ABCD", listing.getLine());

    TestMemory memory;
    auto writer = memory.writer(org);
    writer.add(0x10).add(0xA3).add(0xB9).add(0x12).add(0x34);
    auto reader = memory.reader(org);
    EQ("cmp", OK, listing.disassemble(reader, reader.address()));
    EQ("content", "        CMPD  [$1234,Y]", listing.getContent());
    EQ("line", "ABCD: 10 A3 B9 12         CMPD  [$1234,Y]", listing.getLine());
    TRUE("cmp", listing.hasNextLine());
    EQ("line", "ABD1: 34", listing.getLine());
}

void test_mc6800() {
    mc6800::DisMc6800 dis;
    DisFormatter listing(dis, "test");

    TRUE("setcpu", listing.setCpu("mc6800"));
    EQ("bytes", 0, listing.byteLength());
    FALSE("error", listing.isError());

    EQ("content", "        cpu   mc6800", listing.getContent());
    EQ("line", "   0:                     cpu   mc6800", listing.getLine());
    FALSE("content", listing.hasNextContent());
    FALSE("line", listing.hasNextLine());

    const uint32_t org = 0xabcd;
    EQ("org", OK, listing.setOrigin(org));
    EQ("content", "        org   $abcd", listing.getContent());
    EQ("line", "abcd:                     org   $abcd", listing.getLine());

    TestMemory memory;
    auto writer = memory.writer(org);
    writer.add(0xf0).add(0xf1).add(0xf2);
    auto reader = memory.reader(org);
    EQ("sub", OK, listing.disassemble(reader, reader.address()));
    EQ("content", "        subb  $f1f2", listing.getContent());
    EQ("line", "abcd: f0 f1 f2            subb  $f1f2", listing.getLine());
}

void test_mc6805() {
    mc6805::DisMc6805 dis;
    DisFormatter listing(dis, "test");

    TRUE("setcpu", listing.setCpu("mc146805"));
    EQ("bytes", 0, listing.byteLength());
    FALSE("error", listing.isError());

    EQ("content", "        cpu   mc146805", listing.getContent());
    EQ("line", "   0:                  cpu   mc146805", listing.getLine());
    FALSE("content", listing.hasNextContent());
    FALSE("line", listing.hasNextLine());

    const uint32_t org = 0x1234;
    EQ("org", OVERFLOW_RANGE, listing.setOrigin(0x3456));
    EQ("org", OK, listing.setOrigin(org));
    EQ("content", "        org   $1234", listing.getContent());
    EQ("line", "1234:                  org   $1234", listing.getLine());

    TestMemory memory;
    auto writer = memory.writer(org);
    writer.add(0xdf).add(0x40).add(0x00);
    auto reader = memory.reader(org);
    EQ("stx", OK, listing.disassemble(reader, reader.address()));
    EQ("content", "        stx   16384,x", listing.getContent());
    EQ("line", "1234: df 40 00         stx   16384,x", listing.getLine());
}

void test_mos6502() {
    mos6502::DisMos6502 dis;
    DisFormatter listing(dis, "test");

    TRUE("setcpu", listing.setCpu("mos6502"));
    EQ("bytes", 0, listing.byteLength());
    FALSE("error", listing.isError());

    EQ("content", "        cpu  mos6502", listing.getContent());
    EQ("line", "   0:                     cpu  mos6502", listing.getLine());
    FALSE("content", listing.hasNextContent());
    FALSE("line", listing.hasNextLine());

    const uint32_t org = 0xabcd;
    EQ("org", OK, listing.setOrigin(org));
    EQ("content", "        org  $abcd", listing.getContent());
    EQ("line", "abcd:                     org  $abcd", listing.getLine());

    TestMemory memory;
    auto writer = memory.writer(org);
    writer.add(0xed).add(0xf2).add(0xf1);
    auto reader = memory.reader(org);
    EQ("sbc", OK, listing.disassemble(reader, reader.address()));
    EQ("content", "        sbc  $f1f2", listing.getContent());
    EQ("line", "abcd: ed f2 f1            sbc  $f1f2", listing.getLine());
}

void test_w65816() {
    mos6502::DisMos6502 dis;
    DisFormatter listing(dis, "test");
    listing.setUppercase(true);

    TRUE("setcpu", listing.setCpu("w65c816"));
    EQ("bytes", 0, listing.byteLength());
    FALSE("error", listing.isError());

    EQ("content", "        CPU  W65C816", listing.getContent());
    EQ("line", "     0:                     CPU  W65C816", listing.getLine());
    FALSE("content", listing.hasNextContent());
    FALSE("line", listing.hasNextLine());

    const uint32_t org = 0xabcdef;
    EQ("org", OK, listing.setOrigin(org));
    EQ("content", "        ORG  $ABCDEF", listing.getContent());
    EQ("line", "ABCDEF:                     ORG  $ABCDEF", listing.getLine());

    TestMemory memory;
    auto writer = memory.writer(org);
    writer.add(0xef).add(0xf0).add(0xf1).add(0xf2);
    auto reader = memory.reader(org);
    EQ("sbc", OK, listing.disassemble(reader, reader.address()));
    EQ("content", "        SBC  $F2F1F0", listing.getContent());
    EQ("line", "ABCDEF: EF F0 F1 F2         SBC  $F2F1F0", listing.getLine());

    dis.setOption("longa", "on");
    writer.add(0x69).add(0x34).add(0x12);
    EQ("adc", OK, listing.disassemble(reader, reader.address()));
    EQ("content", "        ADC  #$1234", listing.getContent());
    EQ("line", "ABCDF3: 69 34 12            ADC  #$1234", listing.getLine());
}

void test_i8048() {
    i8048::DisI8048 dis;
    DisFormatter listing(dis, "test");
    listing.setUppercase(true);

    TRUE("setcpu", listing.setCpu("i8039"));
    EQ("bytes", 0, listing.byteLength());
    FALSE("error", listing.isError());

    EQ("content", "        CPU   I8039", listing.getContent());
    EQ("line", "  0:               CPU   I8039", listing.getLine());
    FALSE("content", listing.hasNextContent());
    FALSE("line", listing.hasNextLine());

    const uint32_t org = 0xbcd;
    EQ("org", OVERFLOW_RANGE, listing.setOrigin(0xabcd));
    EQ("org", OK, listing.setOrigin(org));
    EQ("content", "        ORG   0BCDH", listing.getContent());
    EQ("line", "BCD:               ORG   0BCDH", listing.getLine());

    TestMemory memory;
    const uint32_t start = 0xbcd;
    auto writer = memory.writer(start);
    writer.add(0x89).add(0x8A);
    auto reader = memory.reader(start);
    EQ("orl", OK, listing.disassemble(reader, reader.address()));
    EQ("content", "        ORL   P1, #8AH", listing.getContent());
    EQ("line", "BCD: 89 8A         ORL   P1, #8AH", listing.getLine());
}

void test_i8051() {
    i8051::DisI8051 dis;
    DisFormatter listing(dis, "test");

    TRUE("setcpu", listing.setCpu("i8051"));
    EQ("bytes", 0, listing.byteLength());
    FALSE("error", listing.isError());

    EQ("content", "        cpu   i8051", listing.getContent());
    EQ("line", "   0:                  cpu   i8051", listing.getLine());
    FALSE("content", listing.hasNextContent());
    FALSE("line", listing.hasNextLine());

    const uint32_t org = 0xabcd;
    EQ("org", OK, listing.setOrigin(org));
    EQ("content", "        org   0abcdh", listing.getContent());
    EQ("line", "abcd:                  org   0abcdh", listing.getLine());

    TestMemory memory;
    auto writer = memory.writer(org);
    writer.add(0xb0).add(0xb1);
    auto reader = memory.reader(org);
    EQ("anl", OK, listing.disassemble(reader, reader.address()));
    EQ("content", "        anl   c, /0b0h.1", listing.getContent());
    EQ("line", "abcd: b0 b1            anl   c, /0b0h.1", listing.getLine());
}

void test_i8080() {
    i8080::DisI8080 dis;
    DisFormatter listing(dis, "test");

    TRUE("setcpu", listing.setCpu("i8080"));
    EQ("bytes", 0, listing.byteLength());
    FALSE("error", listing.isError());

    EQ("content", "        cpu   i8080", listing.getContent());
    EQ("line", "   0:                  cpu   i8080", listing.getLine());
    FALSE("content", listing.hasNextContent());
    FALSE("line", listing.hasNextLine());

    const uint32_t org = 0xabcd;
    EQ("org", OK, listing.setOrigin(org));
    EQ("content", "        org   0abcdh", listing.getContent());
    EQ("line", "abcd:                  org   0abcdh", listing.getLine());

    TestMemory memory;
    auto writer = memory.writer(org);
    writer.add(0xea).add(0xeb).add(0xec);
    auto reader = memory.reader(org);
    EQ("jpe", OK, listing.disassemble(reader, reader.address()));
    EQ("content", "        jpe   0ecebh", listing.getContent());
    EQ("line", "abcd: ea eb ec         jpe   0ecebh", listing.getLine());
}

void test_i8096() {
    i8096::DisI8096 dis;
    DisFormatter listing(dis, "test");

    TRUE("setcpu", listing.setCpu("i8096"));
    EQ("bytes", 0, listing.byteLength());
    FALSE("error", listing.isError());

    EQ("content", "        cpu   i8096", listing.getContent());
    EQ("line", "   0:                     cpu   i8096", listing.getLine());
    FALSE("content", listing.hasNextContent());
    FALSE("line", listing.hasNextLine());

    const uint32_t org = 0xabcd;
    EQ("org", OK, listing.setOrigin(org));
    EQ("content", "        org   0abcdh", listing.getContent());
    EQ("line", "abcd:                     org   0abcdh", listing.getLine());

    TestMemory memory;
    auto writer = memory.writer(org);
    writer.add(0xfe).add(0x7f).add(0x7d).add(0x7b).add(0x81).add(0x82);
    auto reader = memory.reader(org);
    EQ("mul", OK, listing.disassemble(reader, reader.address()));
    EQ("content", "        mulb  130, 817bh[124]", listing.getContent());
    EQ("line", "abcd: fe 7f 7d 7b         mulb  130, 817bh[124]", listing.getLine());
    TRUE("mul", listing.hasNextLine());
    EQ("line", "abd1: 81 82", listing.getLine());
}

void test_z80() {
    z80::DisZ80 dis;
    DisFormatter listing(dis, "test");

    TRUE("setcpu", listing.setCpu("z80"));
    EQ("bytes", 0, listing.byteLength());
    FALSE("error", listing.isError());

    EQ("content", "        cpu   z80", listing.getContent());
    EQ("line", "   0:                     cpu   z80", listing.getLine());
    FALSE("content", listing.hasNextContent());
    FALSE("line", listing.hasNextLine());

    const uint32_t org = 0xabcd;
    EQ("org", OK, listing.setOrigin(org));
    EQ("content", "        org   0abcdh", listing.getContent());
    EQ("line", "abcd:                     org   0abcdh", listing.getLine());

    TestMemory memory;
    auto writer = memory.writer(org);
    writer.add(0xfd).add(0xcb).add(0x80).add(0x86);
    auto reader = memory.reader(org);
    EQ("res", OK, listing.disassemble(reader, reader.address()));
    EQ("content", "        res   0, (iy-128)", listing.getContent());
    EQ("line", "abcd: fd cb 80 86         res   0, (iy-128)", listing.getLine());
}

void test_z8() {
    z8::DisZ8 dis;
    DisFormatter listing(dis, "test");

    TRUE("setcpu", listing.setCpu("z8"));
    EQ("bytes", 0, listing.byteLength());
    FALSE("error", listing.isError());

    EQ("content", "        cpu     z8", listing.getContent());
    EQ("line", "   0:                     cpu     z8", listing.getLine());
    FALSE("content", listing.hasNextContent());
    FALSE("line", listing.hasNextLine());

    const uint32_t org = 0xabcd;
    EQ("org", OK, listing.setOrigin(org));
    EQ("content", "        org     0abcdh", listing.getContent());
    EQ("line", "abcd:                     org     0abcdh", listing.getLine());

    TestMemory memory;
    auto writer = memory.writer(org);
    writer.add(0xc7).add(0xc8).add(0xc9);
    auto reader = memory.reader(org);
    EQ("ld", OK, listing.disassemble(reader, reader.address()));
    EQ("content", "        ld      r12, 0c9h(r8)", listing.getContent());
    EQ("line", "abcd: c7 c8 c9            ld      r12, 0c9h(r8)", listing.getLine());
}

void test_tlcs90() {
    tlcs90::DisTlcs90 dis;
    DisFormatter listing(dis, "test");

    TRUE("setcpu", listing.setCpu("tlcs90"));
    EQ("bytes", 0, listing.byteLength());
    FALSE("error", listing.isError());

    EQ("content", "        cpu  tlcs90", listing.getContent());
    EQ("line", "   0:                     cpu  tlcs90", listing.getLine());
    FALSE("content", listing.hasNextContent());
    FALSE("line", listing.hasNextLine());

    const uint32_t org = 0xabcd;
    EQ("org", OK, listing.setOrigin(org));
    EQ("content", "        org  0abcdh", listing.getContent());
    EQ("line", "abcd:                     org  0abcdh", listing.getLine());

    TestMemory memory;
    auto writer = memory.writer(org);
    writer.add(0xf7).add(0x6d).add(0xef);
    auto reader = memory.reader(org);
    EQ("xor", OK, listing.disassemble(reader, reader.address()));
    EQ("content", "        xor  (hl+a), 0efh", listing.getContent());
    EQ("line", "abcd: f7 6d ef            xor  (hl+a), 0efh", listing.getLine());
}

void test_ins8060() {
    ins8060::DisIns8060 dis;
    DisFormatter listing(dis, "test");

    TRUE("setcpu", listing.setCpu("ins8060"));
    EQ("bytes", 0, listing.byteLength());
    FALSE("error", listing.isError());

    EQ("content", "        cpu  ins8060", listing.getContent());
    EQ("line", "   0:               cpu  ins8060", listing.getLine());
    FALSE("content", listing.hasNextContent());
    FALSE("line", listing.hasNextLine());

    const uint32_t org = 0xabcd;
    EQ("org", OK, listing.setOrigin(org));
    EQ("content", "        org  x'abcd", listing.getContent());
    EQ("line", "abcd:               org  x'abcd", listing.getLine());

    TestMemory memory;
    auto writer = memory.writer(org);
    writer.add(0xd5).add(0x80);
    auto reader = memory.reader(org);
    EQ("and", OK, listing.disassemble(reader, reader.address()));
    EQ("content", "        and  @e(p1)", listing.getContent());
    EQ("line", "abcd: d5 80         and  @e(p1)", listing.getLine());
}

void test_ins8070() {
    ins8070::DisIns8070 dis;
    DisFormatter listing(dis, "test");

    TRUE("setcpu", listing.setCpu("ins8070"));
    EQ("bytes", 0, listing.byteLength());
    FALSE("error", listing.isError());

    EQ("content", "        cpu  ins8070", listing.getContent());
    EQ("line", "   0:                  cpu  ins8070", listing.getLine());
    FALSE("content", listing.hasNextContent());
    FALSE("line", listing.hasNextLine());

    const uint32_t org = 0xabcd;
    EQ("org", OK, listing.setOrigin(org));
    EQ("content", "        org  x'abcd", listing.getContent());
    EQ("line", "abcd:                  org  x'abcd", listing.getLine());

    TestMemory memory;
    auto writer = memory.writer(org);
    writer.add(0x22).add(0x23).add(0x24);
    auto reader = memory.reader(org);
    EQ("pli", OK, listing.disassemble(reader, reader.address()));
    EQ("content", "        pli  p2, =x'2423", listing.getContent());
    EQ("line", "abcd: 22 23 24         pli  p2, =x'2423", listing.getLine());
}

void test_cdp1802() {
    cdp1802::DisCdp1802 dis;
    DisFormatter listing(dis, "test");

    TRUE("setcpu", listing.setCpu("cdp1804"));
    EQ("bytes", 0, listing.byteLength());
    FALSE("error", listing.isError());

    EQ("content", "        cpu  cdp1804", listing.getContent());
    EQ("line", "   0:                     cpu  cdp1804", listing.getLine());
    FALSE("content", listing.hasNextContent());
    FALSE("line", listing.hasNextLine());

    const uint32_t org = 0xabcd;
    EQ("org", OK, listing.setOrigin(org));
    EQ("content", "        org  0abcdh", listing.getContent());
    EQ("line", "abcd:                     org  0abcdh", listing.getLine());

    TestMemory memory;
    auto writer = memory.writer(org);
    writer.add(0x68).add(0x83).add(0x84).add(0x85);
    auto reader = memory.reader(org);
    EQ("scal", OK, listing.disassemble(reader, reader.address()));
    EQ("content", "        scal 3, 8485h", listing.getContent());
    EQ("line", "abcd: 68 83 84 85         scal 3, 8485h", listing.getLine());
}

void test_scn2650() {
    scn2650::DisScn2650 dis;
    DisFormatter listing(dis, "test");

    TRUE("setcpu", listing.setCpu("scn2650"));
    EQ("bytes", 0, listing.byteLength());
    FALSE("error", listing.isError());

    EQ("content", "        cpu     scn2650", listing.getContent());
    EQ("line", "   0:                  cpu     scn2650", listing.getLine());
    FALSE("content", listing.hasNextContent());
    FALSE("line", listing.hasNextLine());

    const uint32_t org = 0xabcd;
    EQ("org", OK, listing.setOrigin(org));
    EQ("content", "        org     0abcdh", listing.getContent());
    EQ("line", "abcd:                  org     0abcdh", listing.getLine());

    TestMemory memory;
    auto writer = memory.writer(org);
    writer.add(0x0c).add(0xad).add(0xEF);
    auto reader = memory.reader(org);
    EQ("loda", OK, listing.disassemble(reader, reader.address()));
    EQ("content", "        loda,r0 *0adefh, r0, +", listing.getContent());
    EQ("line", "abcd: 0c ad ef         loda,r0 *0adefh, r0, +", listing.getLine());
}

void test_i8086() {
    i8086::DisI8086 dis;
    DisFormatter listing(dis, "test");

    TRUE("setcpu", listing.setCpu("i8086"));
    EQ("bytes", 0, listing.byteLength());
    FALSE("error", listing.isError());

    EQ("content", "        cpu    i8086", listing.getContent());
    EQ("line", "    0:                     cpu    i8086", listing.getLine());
    FALSE("content", listing.hasNextContent());
    FALSE("line", listing.hasNextLine());

    const uint32_t org = 0xbcdef;
    EQ("org", OK, listing.setOrigin(org));
    EQ("content", "        org    0bcdefh", listing.getContent());
    EQ("line", "bcdef:                     org    0bcdefh", listing.getLine());

    TestMemory memory;
    auto writer = memory.writer(org);
    writer.add(0xf7).add(0x83).add(0xff).add(0xfe).add(0xaa).add(0xbb);
    auto reader = memory.reader(org);
    EQ("test", OK, listing.disassemble(reader, reader.address()));
    EQ("content", "        test   word ptr [bp+di+0feffh], 0bbaah", listing.getContent());
    EQ("line", "bcdef: f7 83 ff fe         test   word ptr [bp+di+0feffh], 0bbaah",
            listing.getLine());
    TRUE("test", listing.hasNextLine());
    EQ("line", "bcdf3: aa bb", listing.getLine());
}

void test_tms9900() {
    tms9900::DisTms9900 dis;
    DisFormatter listing(dis, "test");

    TRUE("setcpu", listing.setCpu("tms99105"));
    EQ("bytes", 0, listing.byteLength());
    FALSE("error", listing.isError());

    EQ("content", "        cpu  tms99105", listing.getContent());
    EQ("line", "   0:                   cpu  tms99105", listing.getLine());
    FALSE("content", listing.hasNextContent());
    FALSE("line", listing.hasNextLine());

    const uint32_t org = 0x9abc;
    EQ("org", INSTRUCTION_NOT_ALIGNED, listing.setOrigin(0xABCD));
    EQ("org", OK, listing.setOrigin(0x9abc));
    EQ("content", "        org  9abch", listing.getContent());
    EQ("line", "9abc:                   org  9abch", listing.getLine());

    TestMemory memory;
    auto writer = memory.writer(org, dis.config().endian());
    writer.add(0x002a).add(0x4861).add(0x4a4b).add(0x4c4d);
    auto reader = memory.reader(org);
    EQ("am", OK, listing.disassemble(reader, reader.address()));
    EQ("content", "        am   @4a4bh(r1), @4c4dh(r1)", listing.getContent());
    EQ("line", "9abc: 002a 4861         am   @4a4bh(r1), @4c4dh(r1)", listing.getLine());
    TRUE("am", listing.hasNextLine());
    EQ("line", "9ac0: 4a4b 4c4d", listing.getLine());
}

void test_tms32010() {
    tms32010::DisTms32010 dis;
    DisFormatter listing(dis, "test");

    TRUE("setcpu", listing.setCpu("tms32010"));
    EQ("bytes", 0, listing.byteLength());
    FALSE("error", listing.isError());

    EQ("content", "        cpu  tms32010", listing.getContent());
    EQ("line", "  0:                   cpu  tms32010", listing.getLine());
    FALSE("content", listing.hasNextContent());
    FALSE("line", listing.hasNextLine());

    const uint32_t org = 0x789;
    EQ("org", OVERFLOW_RANGE, listing.setOrigin(0xabcd));
    EQ("org", OK, listing.setOrigin(org));
    EQ("content", "        org  789h", listing.getContent());
    EQ("line", "789:                   org  789h", listing.getLine());

    TestMemory memory;
    const auto start = org * dis.config().addressUnit();
    auto writer = memory.writer(start, dis.config().endian());
    writer.add(0xf800).add(0x0fed);
    auto reader = memory.reader(start);
    EQ("call", OK, listing.disassemble(reader, reader.address() / dis.config().addressUnit()));
    EQ("content", "        call 0fedh", listing.getContent());
    EQ("line", "789: f800 0fed         call 0fedh", listing.getLine());

    writer.add(0x50a0);
    EQ("sacl", OK, listing.disassemble(reader, reader.address() / dis.config().addressUnit()));
    EQ("content", "        sacl *+, 0, ar0", listing.getContent());
    EQ("line", "78b: 50a0              sacl *+, 0, ar0", listing.getLine());
}

void test_mc68000() {
    mc68000::DisMc68000 dis;
    DisFormatter listing(dis, "test");

    TRUE("setcpu", listing.setCpu("mc68000"));
    EQ("bytes", 0, listing.byteLength());
    FALSE("error", listing.isError());

    EQ("content", "        cpu     mc68000", listing.getContent());
    EQ("line", "     0:                   cpu     mc68000", listing.getLine());
    FALSE("content", listing.hasNextContent());
    FALSE("line", listing.hasNextLine());

    const uint32_t org = 0x9abcde;
    EQ("org", INSTRUCTION_NOT_ALIGNED, listing.setOrigin(0xABCDEF));
    EQ("org", OK, listing.setOrigin(org));
    EQ("content", "        org     $9abcde", listing.getContent());
    EQ("line", "9abcde:                   org     $9abcde", listing.getLine());

    TestMemory memory;
    auto writer = memory.writer(org, dis.config().endian());
    writer.add(0x00b9).add(0xbdbe).add(0xbfc0).add(0x00c2).add(0xc3c4);
    auto reader = memory.reader(org);
    EQ("ori", OK, listing.disassemble(reader, reader.address()));
    EQ("content", "        ori.l   #$bdbebfc0, ($c2c3c4).l", listing.getContent());
    EQ("line", "9abcde: 00b9 bdbe         ori.l   #$bdbebfc0, ($c2c3c4).l", listing.getLine());
    TRUE("ori", listing.hasNextLine());
    EQ("line", "9abce2: bfc0 00c2", listing.getLine());
    TRUE("ori", listing.hasNextLine());
    EQ("line", "9abce6: c3c4", listing.getLine());
}

void test_ns32000() {
    ns32000::DisNs32000 dis;
    DisFormatter listing(dis, "test");

    TRUE("setcpu", listing.setCpu("ns32032"));
    EQ("bytes", 0, listing.byteLength());
    FALSE("error", listing.isError());

    EQ("content", "        cpu     ns32032", listing.getContent());
    EQ("line", "     0:                     cpu     ns32032", listing.getLine());
    FALSE("content", listing.hasNextContent());
    FALSE("line", listing.hasNextLine());

    const uint32_t org = 0xabcdef;
    EQ("org", OK, listing.setOrigin(org));
    EQ("content", "        org     x'abcdef", listing.getContent());
    EQ("line", "abcdef:                     org     x'abcdef", listing.getLine());

    TestMemory memory;
    auto writer = memory.writer(org);
    // clang-format off
    writer
        .add(0x2e).add(0x4b).add(0xef).add(0x83).add(0x94).add(0xc0)
        .add(0x33).add(0x44).add(0x55).add(0xc0).add(0x11).add(0x22)
        .add(0x33).add(0xc0).add(0x77).add(0x88).add(0x99).add(0xc0)
        .add(0x55).add(0x66).add(0x77).add(0x20);
    auto reader = memory.reader(org);
    EQ("extd", OK, listing.disassemble(reader, reader.address()));
    EQ("content", "        extd    r1, x'00112233(x'00334455(fp))[r3:w], "
            "x'00556677(x'00778899(sb))[r4:w], 32",
            listing.getContent());
    EQ("line", "abcdef: 2e 4b ef 83         extd    r1, x'00112233(x'00334455(fp))[r3:w], "
            "x'00556677(x'00778899(sb))[r4:w], 32", listing.getLine());
    TRUE("extd", listing.hasNextLine());
    EQ("line", "abcdf3: 94 c0 33 44", listing.getLine());
    TRUE("extd", listing.hasNextLine());
    EQ("line", "abcdf7: 55 c0 11 22", listing.getLine());
    TRUE("extd", listing.hasNextLine());
    EQ("line", "abcdfb: 33 c0 77 88", listing.getLine());
    TRUE("extd", listing.hasNextLine());
    EQ("line", "abcdff: 99 c0 55 66", listing.getLine());
    TRUE("extd", listing.hasNextLine());
    EQ("line", "abce03: 77 20", listing.getLine());
    // clang-format on
}

void test_z8001() {
    z8000::DisZ8000 dis;
    DisFormatter listing(dis, "test");
    listing.setUppercase(true);
    dis.setOption("short-address", "on");

    TRUE("setcpu", listing.setCpu("z8001"));
    EQ("bytes", 0, listing.byteLength());
    FALSE("error", listing.isError());

    EQ("content", "        CPU    Z8001", listing.getContent());
    EQ("line", "     0:                   CPU    Z8001", listing.getLine());
    FALSE("content", listing.hasNextContent());
    FALSE("line", listing.hasNextLine());

    const uint32_t org = 0x789abc;
    EQ("org", OVERFLOW_RANGE, listing.setOrigin(0x89abcd));
    EQ("org", INSTRUCTION_NOT_ALIGNED, listing.setOrigin(0x789abd));
    EQ("org", OK, listing.setOrigin(org));
    EQ("content", "        ORG    789ABCH", listing.getContent());
    EQ("line", "789ABC:                   ORG    789ABCH", listing.getLine());

    TestMemory memory;
    auto writer = memory.writer(org, dis.config().endian());
    writer.add(0x4c15).add(0x1617).add(0x1919);
    auto reader = memory.reader(org);
    EQ("ldb", OK, listing.disassemble(reader, reader.address()));
    EQ("content", "        LDB    |160017H|(R1), #25", listing.getContent());
    EQ("line", "789ABC: 4C15 1617         LDB    |160017H|(R1), #25", listing.getLine());
    TRUE("ldb", listing.hasNextLine());
    EQ("line", "789AC0: 1919", listing.getLine());
}

void test_z8002() {
    z8000::DisZ8000 dis;
    DisFormatter listing(dis, "test");

    TRUE("setcpu", listing.setCpu("z8002"));
    EQ("bytes", 0, listing.byteLength());
    FALSE("error", listing.isError());

    EQ("content", "        cpu    z8002", listing.getContent());
    EQ("line", "   0:                   cpu    z8002", listing.getLine());
    FALSE("content", listing.hasNextContent());
    FALSE("line", listing.hasNextLine());

    const uint32_t org = 0x9abc;
    EQ("org", INSTRUCTION_NOT_ALIGNED, listing.setOrigin(0xABCD));
    EQ("prg", OK, listing.setOrigin(org));
    EQ("content", "        org    9abch", listing.getContent());
    EQ("line", "9abc:                   org    9abch", listing.getLine());

    TestMemory memory;
    auto writer = memory.writer(org, dis.config().endian());
    writer.add(0x1000).add(0x0102).add(0x0304);
    auto reader = memory.reader(org);
    EQ("cpl", OK, listing.disassemble(reader, reader.address()));
    EQ("content", "        cpl    rr0, #01020304h", listing.getContent());
    EQ("line", "9abc: 1000 0102         cpl    rr0, #01020304h", listing.getLine());
    TRUE("cpl", listing.hasNextLine());
    EQ("line", "9ac0: 0304", listing.getLine());
}

void test_mn1610() {
    mn1610::DisMn1610 dis;
    DisFormatter listing(dis, "test");

    TRUE("setcpu", listing.setCpu("mn1610"));
    EQ("bytes", 0, listing.byteLength());
    FALSE("error", listing.isError());

    EQ("content", "        cpu  mn1610", listing.getContent());
    EQ("line", "   0:                   cpu  mn1610", listing.getLine());
    FALSE("content", listing.hasNextContent());
    FALSE("line", listing.hasNextLine());

    const uint32_t org = 0xabcd;
    EQ("org", OVERFLOW_RANGE, listing.setOrigin(0x12345));
    EQ("org", OK, listing.setOrigin(org));
    EQ("content", "        org  x'abcd'", listing.getContent());
    EQ("line", "abcd:                   org  x'abcd'", listing.getLine());

    TestMemory memory;
    const auto start = org * dis.config().addressUnit();
    auto writer = memory.writer(start, dis.config().endian());
    writer.add(0x2b55);
    auto reader = memory.reader(start);
    EQ("tbit", OK, listing.disassemble(reader, reader.address() / dis.config().addressUnit()));
    EQ("content", "        tbit r3, 5, nz", listing.getContent());
    EQ("line", "abcd: 2b55              tbit r3, 5, nz", listing.getLine());
}

void test_mn1613() {
    mn1610::DisMn1610 dis;
    DisFormatter listing(dis, "test");

    TRUE("setcpu", listing.setCpu("mn1613"));
    EQ("bytes", 0, listing.byteLength());
    FALSE("error", listing.isError());

    EQ("content", "        cpu  mn1613", listing.getContent());
    EQ("line", "    0:                   cpu  mn1613", listing.getLine());
    FALSE("content", listing.hasNextContent());
    FALSE("line", listing.hasNextLine());

    const uint32_t org = 0x34567;
    EQ("org", OVERFLOW_RANGE, listing.setOrigin(0x45678));
    EQ("org", OK, listing.setOrigin(org));
    EQ("content", "        org  x'34567'", listing.getContent());
    EQ("line", "34567:                   org  x'34567'", listing.getLine());

    TestMemory memory;
    const auto start = org * dis.config().addressUnit();
    auto writer = memory.writer(start, dis.config().endian());
    writer.add(0x7e1f).add(0x5678);
    auto reader = memory.reader(start);
    EQ("mvwi", OK, listing.disassemble(reader, reader.address() / dis.config().addressUnit()));
    EQ("content", "        mvwi str, x'5678', skp", listing.getContent());
    EQ("line", "34567: 7e1f 5678         mvwi str, x'5678', skp", listing.getLine());
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

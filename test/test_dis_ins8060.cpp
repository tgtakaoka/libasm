/*
 * Copyright 2020 Tadashi G. Takaoka
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

#include "dis_ins8060.h"
#include "test_dis_helper.h"

using namespace libasm;
using namespace libasm::ins8060;
using namespace libasm::test;

DisIns8060 dis8060;
Disassembler &disassembler(dis8060);

static void set_up() {
    disassembler.reset();
    disassembler.setRelativeTarget(false);
}

static void tear_down() {
    symtab.reset();
}

static void test_cpu() {
    EQUALS("cpu sc/mp", true,    disassembler.setCpu("SC/MP"));
    EQUALS("cpu sc/mp", "SC/MP", disassembler.getCpu());

    EQUALS("cpu 8060", true,    disassembler.setCpu("8060"));
    EQUALS("cpu 8060", "SC/MP", disassembler.getCpu());

    EQUALS("cpu INS8060", true,    disassembler.setCpu("INS8060"));
    EQUALS("cpu INS8060", "SC/MP", disassembler.getCpu());
}

static void test_inherent() {
    TEST(HALT, "", 0x00);
    TEST(XAE,  "", 0x01);
    TEST(CCL,  "", 0x02);
    TEST(SCL,  "", 0x03);
    TEST(DINT, "", 0x04);
    TEST(IEN,  "", 0x05);
    TEST(CSA,  "", 0x06);
    TEST(CAS,  "", 0x07);
    TEST(NOP,  "", 0x08);
    TEST(SIO,  "", 0x19);
    TEST(SR,   "", 0x1C);
    TEST(SRL,  "", 0x1D);
    TEST(RR,   "", 0x1E);
    TEST(RRL,  "", 0x1F);
    TEST(LDE,  "", 0x40);
    TEST(ANE,  "", 0x50);
    TEST(ORE,  "", 0x58);
    TEST(XRE,  "", 0x60);
    TEST(DAE,  "", 0x68);
    TEST(ADE,  "", 0x70);
    TEST(CAE,  "", 0x78);
}

static void test_pointer() {
    TEST(XPAL, "PC", 0x30);
    TEST(XPAL, "P1", 0x31);
    TEST(XPAL, "P2", 0x32);
    TEST(XPAL, "P3", 0x33);
    TEST(XPAH, "PC", 0x34);
    TEST(XPAH, "P1", 0x35);
    TEST(XPAH, "P2", 0x36);
    TEST(XPAH, "P3", 0x37);
    TEST(XPPC, "PC", 0x3C);
    TEST(XPPC, "P1", 0x3D);
    TEST(XPPC, "P2", 0x3E);
    TEST(XPPC, "P3", 0x3F);
}

static void test_immediate() {
    TEST(DLY, "18", 0x8F, 0x12);
}

static void test_jump() {
    ATEST(0x1000, JMP, "0x1000",   0x90, 0xFE);
    ATEST(0x1000, JP,  "0x1081",   0x94, 0x7F);
    ATEST(0x1000, JZ,  "0x1F83",   0x98, 0x81); // 4kB page boundary
    ATEST(0x1FF0, JZ,  "0x1071",   0x98, 0x7F); // 4kB page boundary
    ATEST(0x1000, JNZ, "E(PC)",    0x9C, 0x80);

    TEST(JMP, "E(PC)",    0x90, 0x80);
    TEST(JMP, "E(P1)",    0x91, 0x80);
    TEST(JMP, "127(P2)",  0x92, 0x7F);
    TEST(JMP, "-127(P3)", 0x93, 0x81);

    symtab.intern(0x1000, "label1000");
    symtab.intern(127,    "disp0x7F");
    symtab.intern(-127,   "disp0x81");
    symtab.intern(-128,   "disp0x80");

    ATEST(0x1000, JMP, "label1000",    0x90, 0xFE);
    TEST(JMP, "E(PC)",        0x90, 0x80);
    TEST(JMP, "disp0x7F(P1)", 0x91, 0x7F);
    TEST(JMP, "disp0x81(P2)", 0x92, 0x81);
    TEST(JMP, "E(P3)",        0x93, 0x80);

    disassembler.setRelativeTarget(true);
    ATEST(0x2800, JMP, ".-125",  0x90, 0x81);
    ATEST(0x2800, JMP, ".",      0x90, 0xFE);
    ATEST(0x2800, JMP, ".+2",    0x90, 0x00);
    ATEST(0x2800, JMP, ".+129",  0x90, 0x7F);
    ATEST(0x2000, JMP, "0x2F83", 0x90, 0x81); // 4kB page boundary
    ATEST(0x2FF0, JMP, "0x2071", 0x90, 0x7F); // 4kB page boundary
}

static void test_incr_decr() {
    ATEST(0x1000, ILD, "0x1000", 0xA8, 0xFF);
    ATEST(0x1000, ILD, "0x1F82", 0xA8, 0x81); // 4kB page boundary
    ATEST(0x1FF0, ILD, "0x1070", 0xA8, 0x7F); // 4kB page boundary
    TEST(ILD, "E(PC)",    0xA8, 0x80);
    TEST(ILD, "E(P1)",    0xA9, 0x80);
    TEST(ILD, "127(P2)",  0xAA, 0x7F);
    TEST(ILD, "-127(P3)", 0xAB, 0x81);

    ATEST(0x1000, DLD, "0x1000", 0xB8, 0xFF);
    ATEST(0x1000, DLD, "0x1F82", 0xB8, 0x81); // 4kB page boundary
    ATEST(0x1FF0, DLD, "0x1070", 0xB8, 0x7F); // 4kB page boundary
    TEST(DLD, "E(PC)",    0xB8, 0x80);
    TEST(DLD, "E(P1)",    0xB9, 0x80);
    TEST(DLD, "127(P2)",  0xBA, 0x7F);
    TEST(DLD, "-127(P3)", 0xBB, 0x81);

    symtab.intern(0x1000, "label1000");
    symtab.intern(127,    "disp0x7F");
    symtab.intern(-127,   "disp0x81");
    symtab.intern(-128,   "disp0x80");

    ATEST(0x1000, ILD, "label1000", 0xA8, 0xFF);
    TEST(ILD, "E(PC)",    0xA8, 0x80);
    TEST(ILD, "E(P1)",    0xA9, 0x80);
    TEST(ILD, "disp0x7F(P2)", 0xAA, 0x7F);
    TEST(ILD, "disp0x81(P3)", 0xAB, 0x81);
}

static void test_alu() {
    ATEST(0x1000, LD, "0x1000", 0xC0, 0xFF);
    ATEST(0x1000, LD, "0x1F82", 0xC0, 0x81); // 4kB page boundary
    ATEST(0x1FF0, LD, "0x1070", 0xC0, 0x7F); // 4kB page boundary
    TEST(LD, "E(PC)",     0xC0, 0x80);
    TEST(LD, "E(P1)",     0xC1, 0x80);
    TEST(LD, "127(P2)",   0xC2, 0x7F);
    TEST(LD, "-127(P3)",  0xC3, 0x81);
    TEST(LD, "@E(P1)",    0xC5, 0x80);
    TEST(LD, "@127(P2)",  0xC6, 0x7F);
    TEST(LD, "@-127(P3)", 0xC7, 0x81);

    ATEST(0x1000, ST,  "0x1000", 0xC8, 0xFF);
    ATEST(0x1000, AND, "0x1F82", 0xD0, 0x81); // 4kB page boundary
    ATEST(0x1FF0, OR,  "0x1070", 0xD8, 0x7F); // 4kB page boundary
    TEST(XOR, "E(PC)",     0xE0, 0x80);
    TEST(DAD, "E(P1)",     0xE9, 0x80);
    TEST(ADD, "127(P2)",   0xF2, 0x7F);
    TEST(CAD, "-127(P3)",  0xFB, 0x81);
    TEST(ST,  "@E(P1)",    0xCD, 0x80);
    TEST(AND, "@127(P2)",  0xD6, 0x7F);
    TEST(OR,  "@-127(P3)", 0xDF, 0x81);

    symtab.intern(0x1000, "label1000");
    symtab.intern(127,    "disp0x7F");
    symtab.intern(-127,   "disp0x81");
    symtab.intern(-128,   "disp0x80");

    ATEST(0x1000, ST,  "label1000", 0xC8, 0xFF);
    TEST(AND, "@disp0x7F(P2)", 0xD6, 0x7F);
    TEST(OR,  "@disp0x81(P3)", 0xDF, 0x81);

    disassembler.setRelativeTarget(true);
    ATEST(0x2800, LD, ".-126",  0xC0, 0x81);
    ATEST(0x2800, LD, ".",      0xC0, 0xFF);
    ATEST(0x2800, LD, ".+1",    0xC0, 0x00);
    ATEST(0x2800, LD, ".+128",  0xC0, 0x7F);
    ATEST(0x2000, LD, "0x2F82", 0xC0, 0x81); // 4kB page boundary
    ATEST(0x2FF0, LD, "0x2070", 0xC0, 0x7F); // 4kB page boundary
}

static void test_alu_immediate() {
    TEST(LDI, "0",    0xC4, 0x00);
    TEST(ANI, "0xFF", 0xD4, 0xFF);
    TEST(ORI, "1",    0xDC, 0x01);
    TEST(XRI, "0x80", 0xE4, 0x80);
    TEST(DAI, "0x99", 0xEC, 0x99);
    TEST(ADI, "18",   0xF4, 0x12);
    TEST(CAI, "0x34", 0xFC, 0x34);

    symtab.intern(-1, "minus1");

    TEST(LDI, "minus1", 0xC4, 0xFF);
}

static void test_illegal() {
    const uint8_t illegals[] = {
        0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
        0x18, 0x1A, 0x1B,
        0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
        0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
        0x38, 0x39, 0x3A, 0x3B,
        0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
        0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
        0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
        0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
        0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
        0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
        0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
        0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
        0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E,
        0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
        0xAC, 0xAD, 0xAE, 0xAF,
        0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7,
        0xBC, 0xBD, 0xBE, 0xBF,
        0xCC,
    };
    for (uint8_t idx = 0; idx < sizeof(illegals); idx++)
        ILLEGAL(illegals[idx]);
}

void run_tests() {
    RUN_TEST(test_cpu);
    RUN_TEST(test_inherent);
    RUN_TEST(test_pointer);
    RUN_TEST(test_immediate);
    RUN_TEST(test_jump);
    RUN_TEST(test_incr_decr);
    RUN_TEST(test_alu);
    RUN_TEST(test_alu_immediate);
    RUN_TEST(test_illegal);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

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

#include "dis_ins8070.h"
#include "test_dis_helper.h"

using namespace libasm;
using namespace libasm::ins8070;
using namespace libasm::test;

DisIns8070 dis8070;
Disassembler &disassembler(dis8070);

void set_up() {
    disassembler.reset();
}

void tear_down() {
    symtab.reset();
}

// clang-format off
void test_cpu() {
    EQUALS("cpu 8070", true,   disassembler.setCpu("8070"));
    EQUALS_P("cpu 8070", "8070", disassembler.config().cpu_P());

    EQUALS("cpu INS8070", true,   disassembler.setCpu("INS8070"));
    EQUALS_P("cpu INS8070", "8070", disassembler.config().cpu_P());
}

void test_implied() {
    TEST("NOP",  "",       0x00);
    TEST("XCH",  "A, E",   0x01);
    TEST("LD",   "A, S",   0x06);
    TEST("LD",   "S, A",   0x07);
    TEST("PUSH", "EA",     0x08);
    TEST("LD",   "T, EA",  0x09);
    TEST("PUSH", "A",      0x0A);
    TEST("LD",   "EA, T",  0x0B);
    TEST("SR",   "EA",     0x0C);
    TEST("DIV",  "EA, T",  0x0D);
    TEST("SL",   "A",      0x0E);
    TEST("SL",   "EA",     0x0F);
    TEST("CALL", "0",      0x10);
    TEST("CALL", "1",      0x11);
    TEST("CALL", "2",      0x12);
    TEST("CALL", "3",      0x13);
    TEST("CALL", "4",      0x14);
    TEST("CALL", "5",      0x15);
    TEST("CALL", "6",      0x16);
    TEST("CALL", "7",      0x17);
    TEST("CALL", "8",      0x18);
    TEST("CALL", "9",      0x19);
    TEST("CALL", "10",     0x1A);
    TEST("CALL", "11",     0x1B);
    TEST("CALL", "12",     0x1C);
    TEST("CALL", "13",     0x1D);
    TEST("CALL", "14",     0x1E);
    TEST("CALL", "15",     0x1F);
    TEST("MPY",  "EA, T",  0x2C);
    TEST("SSM",  "P2",     0x2E);
    TEST("SSM",  "P3",     0x2F);
    TEST("LD",   "EA, PC", 0x30);
    TEST("LD",   "EA, SP", 0x31);
    TEST("LD",   "EA, P2", 0x32);
    TEST("LD",   "EA, P3", 0x33);
    TEST("POP",  "A",      0x38);
    TEST("POP",  "EA",     0x3A);
    TEST("SR",   "A",      0x3C);
    TEST("SRL",  "A",      0x3D);
    TEST("RR",   "A",      0x3E);
    TEST("RRL",  "A",      0x3F);
    TEST("LD",   "A, E",   0x40);
    TEST("LD",   "PC, EA", 0x44);
    TEST("LD",   "SP, EA", 0x45);
    TEST("LD",   "P2, EA", 0x46);
    TEST("LD",   "P3, EA", 0x47);
    TEST("LD",   "E, A",   0x48);
    TEST("XCH",  "EA, PC", 0x4C);
    TEST("XCH",  "EA, SP", 0x4D);
    TEST("XCH",  "EA, P2", 0x4E);
    TEST("XCH",  "EA, P3", 0x4F);
    TEST("PUSH", "PC",     0x54);
    TEST("PUSH", "P2",     0x56);
    TEST("PUSH", "P3",     0x57);
    TEST("OR",   "A, E",   0x58);
    TEST("RET",  "",       0x5C);
    TEST("POP",  "P2",     0x5E);
    TEST("POP",  "P3",     0x5F);
    TEST("XOR",  "A, E",   0x60);
    TEST("ADD",  "A, E",   0x70);
    TEST("SUB",  "A, E",   0x78);
}

void test_immediate() {
    TEST("AND", "S, =18",   0x39, 0x12);
    TEST("OR",  "S, =X'34", 0x3B, 0x34);
    TEST("LD",  "A, =18",   0xC4, 0x12);
    TEST("AND", "A, =18",   0xD4, 0x12);
    TEST("OR",  "A, =18",   0xDC, 0x12);
    TEST("XOR", "A, =18",   0xE4, 0x12);
    TEST("ADD", "A, =18",   0xF4, 0x12);
    TEST("SUB", "A, =18",     0xFC, 0x12);
    NMEM("SUB", "A, =0", "0", 0xFC);

    TEST("PLI", "P2, =X'1234", 0x22, 0x34, 0x12);
    TEST("PLI", "P3, =X'1234", 0x23, 0x34, 0x12);
    TEST("LD",  "SP, =X'1234", 0x25, 0x34, 0x12);
    TEST("LD",  "P2, =X'1234", 0x26, 0x34, 0x12);
    TEST("LD",  "P3, =X'1234", 0x27, 0x34, 0x12);
    TEST("LD",  "EA, =X'1234", 0x84, 0x34, 0x12);
    TEST("LD",  "T, =X'1234",  0xA4, 0x34, 0x12);
    TEST("ADD", "EA, =X'1234", 0xB4, 0x34, 0x12);
    TEST("SUB", "EA, =X'1234",           0xBC, 0x34, 0x12);
    NMEM("SUB", "EA, =X'0034", "X'0034", 0xBC, 0x34);
    NMEM("SUB", "EA, =0",           "0", 0xBC);

    symtab.intern(0x12,   "sym12");
    symtab.intern(0x1234, "sym1234");

    TEST("AND", "S, =sym12",    0x39, 0x12);
    TEST("LD",  "A, =sym12",    0xC4, 0x12);
    TEST("AND", "A, =sym12",    0xD4, 0x12);
    TEST("SUB", "A, =sym12",    0xFC, 0x12);

    TEST("PLI", "P2, =sym1234", 0x22, 0x34, 0x12);
    TEST("LD",  "SP, =sym1234", 0x25, 0x34, 0x12);
    TEST("LD",  "P3, =sym1234", 0x27, 0x34, 0x12);
    TEST("LD",  "EA, =sym1234", 0x84, 0x34, 0x12);
    TEST("LD",  "T, =sym1234",  0xA4, 0x34, 0x12);
    TEST("ADD", "EA, =sym1234", 0xB4, 0x34, 0x12);

    disassembler.setOption("use-sharp", "true");
    TEST("PLI", "P2, #sym1234", 0x22, 0x34, 0x12);
}

void test_absolute() {
    TEST("JSR", "X'1234", 0x20, 0x33, 0x12);
    TEST("JMP", "X'1234",           0x24, 0x33, 0x12);
    NMEM("JMP", "X'0034", "X'0034", 0x24, 0x33);
    NMEM("JMP", "X'0001", "X'0001", 0x24);

    symtab.intern(0x1234, "sym1234");

    TEST("JSR", "sym1234", 0x20, 0x33, 0x12);
    TEST("JMP", "sym1234", 0x24, 0x33, 0x12);
}

void test_direct() {
    TEST("LD",  "EA, X'FF34", 0x85, 0x34);
    TEST("ST",  "EA, X'FF34", 0x8D, 0x34);
    TEST("ILD", "A, X'FF34",  0x95, 0x34);
    TEST("DLD", "A, X'FF34",  0x9D, 0x34);
    TEST("LD",  "T, X'FF34",  0xA5, 0x34);
    TEST("ADD", "EA, X'FF34", 0xB5, 0x34);
    TEST("SUB", "EA, X'FF34", 0xBD, 0x34);
    TEST("LD",  "A, X'FF34",  0xC5, 0x34);
    TEST("AND", "A, X'FF34",  0xD5, 0x34);
    TEST("OR",  "A, X'FF34",  0xDD, 0x34);
    TEST("XOR", "A, X'FF34",  0xE5, 0x34);
    TEST("ADD", "A, X'FF34",  0xF5, 0x34);
    TEST("SUB", "A, X'FF34",           0xFD, 0x34);
    NMEM("SUB", "A, X'FF00", "X'FF00", 0xFD);

    symtab.intern(0xFF34, "dir34");
    symtab.intern(0xFF00, "dir00");
    symtab.intern(0xFFFF, "dirFF");

    TEST("LD",  "EA, dir34", 0x85, 0x34);
    TEST("ST",  "EA, dir34", 0x8D, 0x34);
    TEST("ILD", "A, dir00",  0x95, 0x00);
    TEST("DLD", "A, dirFF",  0x9D, 0xFF);
    TEST("SUB", "EA, dir00", 0xBD, 0x00);
    TEST("LD",  "A, dir34",  0xC5, 0x34);
}

void test_relative() {
    ATEST(0x1000, "BP",  "X'1000", 0x64, 0xFE);
    ATEST(0x1100, "BZ",  "X'1082", 0x6C, 0x80);
    ATEST(0x1000, "BRA", "X'1081", 0x74, 0x7F);
    ATEST(0x1000, "BNZ", "X'1005", 0x7C, 0x03);
    ATEST(0x1000, "BND", "X'1005",           0x2D, 0x03);
    ANMEM(0x1000, "BND", "X'1002", "X'1002", 0x2D);

    symtab.intern(0x1000, "sym1000");
    symtab.intern(0x1005, "sym1005");
    symtab.intern(0x1081, "sym1081");
    symtab.intern(0x1082, "sym1082");

    ATEST(0x1000, "BP",  "sym1000", 0x64, 0xFE);
    ATEST(0x1100, "BZ",  "sym1082", 0x6C, 0x80);
    ATEST(0x1000, "BRA", "sym1081", 0x74, 0x7F);
    ATEST(0x1000, "BNZ", "sym1005", 0x7C, 0x03);

    disassembler.setOption("relative", "enable");
    ATEST(0x2000, "BRA", "$-126", 0x74, 0x80);
    ATEST(0x2000, "BRA", "$",     0x74, 0xFE);
    ATEST(0x2000, "BRA", "$+2",   0x74, 0x00);
    ATEST(0x2000, "BRA", "$+129", 0x74, 0x7F);
}

void test_indexed() {
    TEST("BP",  "0,P2",    0x66, 0x00);
    TEST("BP",  "-128,P3", 0x67, 0x80);
    TEST("BZ",  "127,P2",  0x6E, 0x7F);
    TEST("BZ",  "0,P3",    0x6F, 0x00);
    TEST("BRA", "2,P2",    0x76, 0x02);
    TEST("BRA", "3,P3",    0x77, 0x03);
    TEST("BNZ", "-2,P2",   0x7E, 0xFE);
    TEST("BNZ", "-3,P3",         0x7F, 0xFD);
    NMEM("BNZ",  "0,P3", "0,P3", 0x7F);

    ATEST(0x1000, "LD",  "EA, X'1000,PC", 0x80, 0xFF);
    TEST(         "LD",  "EA, 0,SP",      0x81, 0x00);
    TEST(         "LD",  "EA, -128,P2",   0x82, 0x80);
    TEST(         "LD",  "EA, 127,P3",    0x83, 0x7F);
    ATEST(0x1100, "ST",  "EA, X'1081,PC", 0x88, 0x80);
    TEST(         "ST",  "EA, 0,SP",      0x89, 0x00);
    TEST(         "ST",  "EA, -128,P2",   0x8A, 0x80);
    TEST(         "ST",  "EA, 127,P3",    0x8B, 0x7F);
    ATEST(0x1000, "LD",  "T, X'1080,PC",  0xA0, 0x7F);
    TEST(         "LD",  "T, 0,SP",       0xA1, 0x00);
    TEST(         "LD",  "T, -128,P2",    0xA2, 0x80);
    TEST(         "LD",  "T, 127,P3",     0xA3, 0x7F);
    ATEST(0x1000, "ADD", "EA, X'1005,PC", 0xB0, 0x04);
    TEST(         "ADD", "EA, 0,SP",      0xB1, 0x00);
    TEST(         "ADD", "EA, -128,P2",   0xB2, 0x80);
    TEST(         "ADD", "EA, 127,P3",    0xB3, 0x7F);
    ATEST(0x1000, "SUB", "EA, X'1000,PC", 0xB8, 0xFF);
    TEST(         "SUB", "EA, 0,SP",      0xB9, 0x00);
    TEST(         "SUB", "EA, -128,P2",   0xBA, 0x80);
    TEST(         "SUB", "EA, 127,P3",    0xBB, 0x7F);
    NMEM(         "SUB", "EA, 0,P3", "0,P3", 0xBB);

    ATEST(0x1000, "ILD", "A, X'1000,PC", 0x90, 0xFF);
    TEST(         "ILD", "A, 0,SP",      0x91, 0x00);
    TEST(         "ILD", "A, -128,P2",   0x92, 0x80);
    TEST(         "ILD", "A, 127,P3",    0x93, 0x7F);
    ATEST(0x1000, "DLD", "A, X'1000,PC", 0x98, 0xFF);
    TEST(         "DLD", "A, 0,SP",      0x99, 0x00);
    TEST(         "DLD", "A, -128,P2",   0x9A, 0x80);
    TEST(         "DLD", "A, 127,P3",    0x9B, 0x7F);
    ATEST(0x1000, "LD",  "A, X'1000,PC", 0xC0, 0xFF);
    TEST(         "LD",  "A, 0,SP",      0xC1, 0x00);
    TEST(         "LD",  "A, -128,P2",   0xC2, 0x80);
    TEST(         "LD",  "A, 127,P3",    0xC3, 0x7F);
    ATEST(0x1000, "ST",  "A, X'1000,PC", 0xC8, 0xFF);
    TEST(         "ST",  "A, 0,SP",      0xC9, 0x00);
    TEST(         "ST",  "A, -128,P2",   0xCA, 0x80);
    TEST(         "ST",  "A, 127,P3",    0xCB, 0x7F);
    ATEST(0x1000, "AND", "A, X'1000,PC", 0xD0, 0xFF);
    TEST(         "AND", "A, 0,SP",      0xD1, 0x00);
    TEST(         "AND", "A, -128,P2",   0xD2, 0x80);
    TEST(         "AND", "A, 127,P3",    0xD3, 0x7F);
    ATEST(0x1000, "OR",  "A, X'1000,PC", 0xD8, 0xFF);
    TEST(         "OR",  "A, 0,SP",      0xD9, 0x00);
    TEST(         "OR",  "A, -128,P2",   0xDA, 0x80);
    TEST(         "OR",  "A, 127,P3",    0xDB, 0x7F);
    ATEST(0x1000, "XOR", "A, X'1000,PC", 0xE0, 0xFF);
    TEST(         "XOR", "A, 0,SP",      0xE1, 0x00);
    TEST(         "XOR", "A, -128,P2",   0xE2, 0x80);
    TEST(         "XOR", "A, 127,P3",    0xE3, 0x7F);
    ATEST(0x1000, "ADD", "A, X'1000,PC", 0xF0, 0xFF);
    TEST(         "ADD", "A, 0,SP",      0xF1, 0x00);
    TEST(         "ADD", "A, -128,P2",   0xF2, 0x80);
    TEST(         "ADD", "A, 127,P3",    0xF3, 0x7F);
    ATEST(0x1000, "SUB", "A, X'1000,PC", 0xF8, 0xFF);
    TEST(         "SUB", "A, 0,SP",      0xF9, 0x00);
    TEST(         "SUB", "A, -128,P2",   0xFA, 0x80);
    TEST(         "SUB", "A, 127,P3",    0xFB, 0x7F);

    symtab.intern(0x1000, "sym1000");
    symtab.intern(0x1005, "sym1005");
    symtab.intern(0x1080, "sym1080");
    symtab.intern(0x1081, "sym1081");
    symtab.intern(-128, "neg128");
    symtab.intern(+127, "pos127");

    TEST(         "BZ",  "neg128,P3",      0x6F, 0x80);
    TEST(         "BRA", "pos127,P2",      0x76, 0x7F);
    ATEST(0x1000, "LD",  "EA, sym1000,PC", 0x80, 0xFF);
    ATEST(0x1100, "ST",  "EA, sym1081,PC", 0x88, 0x80);
    ATEST(0x1000, "LD",  "T, sym1080,PC",  0xA0, 0x7F);
    ATEST(0x1000, "ADD", "EA, sym1005,PC", 0xB0, 0x04);
    TEST(         "SUB", "A, neg128,SP",   0xF9, 0x80);
    TEST(         "SUB", "A, pos127,P2",   0xFA, 0x7F);

    symtab.reset();
    disassembler.setOption("relative", "on");
    ATEST(0x2000, "LD",  "EA, $-127,PC", 0x80, 0x80);
    ATEST(0x2000, "LD",  "EA, $,PC",     0x80, 0xFF);
    ATEST(0x2000, "LD",  "EA, $+1,PC",   0x80, 0x00);
    ATEST(0x2000, "LD",  "EA, $+128,PC", 0x80, 0x7F);
}

void test_auto_indexed() {
    TEST("LD",  "EA, @-128,P2", 0x86, 0x80);
    TEST("LD",  "EA, @127,P3",  0x87, 0x7F);
    TEST("ST",  "EA, @-128,P2", 0x8E, 0x80);
    TEST("ST",  "EA, @127,P3",  0x8F, 0x7F);
    TEST("LD",  "T, @-128,P2",  0xA6, 0x80);
    TEST("LD",  "T, @127,P3",   0xA7, 0x7F);
    TEST("ADD", "EA, @-128,P2", 0xB6, 0x80);
    TEST("ADD", "EA, @127,P3",  0xB7, 0x7F);
    TEST("SUB", "EA, @-128,P2", 0xBE, 0x80);
    TEST("SUB", "EA, @127,P3",       0xBF, 0x7F);
    NMEM("SUB", "EA, @0,P3", "0,P3", 0xBF);

    TEST("ILD", "A, @-128,P2",  0x96, 0x80);
    TEST("ILD", "A, @127,P3",   0x97, 0x7F);
    TEST("DLD", "A, @-128,P2",  0x9E, 0x80);
    TEST("DLD", "A, @127,P3",   0x9F, 0x7F);
    TEST("LD",  "A, @-128,P2",  0xC6, 0x80);
    TEST("LD",  "A, @127,P3",   0xC7, 0x7F);
    TEST("ST",  "A, @-128,P2",  0xCE, 0x80);
    TEST("ST",  "A, @127,P3",   0xCF, 0x7F);
    TEST("AND", "A, @-128,P2",  0xD6, 0x80);
    TEST("AND", "A, @127,P3",   0xD7, 0x7F);
    TEST("OR",  "A, @-128,P2",  0xDE, 0x80);
    TEST("OR",  "A, @127,P3",   0xDF, 0x7F);
    TEST("XOR", "A, @-128,P2",  0xE6, 0x80);
    TEST("XOR", "A, @127,P3",   0xE7, 0x7F);
    TEST("ADD", "A, @-128,P2",  0xF6, 0x80);
    TEST("ADD", "A, @127,P3",   0xF7, 0x7F);
    TEST("SUB", "A, @-128,P2",  0xFE, 0x80);
    TEST("SUB", "A, @127,P3",   0xFF, 0x7F);

    symtab.intern(-128, "neg128");
    symtab.intern(+127, "pos127");
    symtab.intern(-1,   "neg1");

    TEST("ILD", "A, @neg128,P2", 0x96, 0x80);
    TEST("ST",  "A, @pos127,P3", 0xCF, 0x7F);
    TEST("AND", "A, @neg128,P2", 0xD6, 0x80);
    TEST("SUB", "A, @pos127,P2", 0xFE, 0x7F);
    TEST("SUB", "A, @neg1,P3",   0xFF, 0xFF);
}

void test_formatter() {
    disassembler.setOption("c-style", "on");
    TEST("XOR", "A, =0x55",  0xE4, 0x55);

    disassembler.setOption("intel-hex", "on");
    TEST("ADD", "A, =0AAH",  0xF4, 0xAA);

    disassembler.setOption("intel-hex", "off");
    TEST("SUB", "A, =X'FF",  0xFC, 0xFF);

    disassembler.setOption("intel-hex", "on");
    TEST("JSR", "1234H",      0x20, 0x33, 0x12);

    disassembler.setOption("c-style", "on");
    TEST("PLI", "P2, =0x1234", 0x22, 0x34, 0x12);

    disassembler.setOption("c-style", "off");
    TEST("LD",  "P2, =X'1234", 0x26, 0x34, 0x12);
}

void test_illegal() {
    static constexpr Config::opcode_t illegals[] = {
        0x02, 0x03, 0x04, 0x05,
        0x21, 0x28, 0x29, 0x2A, 0x2B,
        0x34, 0x35, 0x36, 0x37,
        0x41, 0x42, 0x43, 0x49, 0x4A, 0x4B,
        0x51, 0x52, 0x53, 0x55, 0x59, 0x5A, 0x5B, 0x5D,
        0x61, 0x62, 0x63, 0x65, 0x68, 0x69, 0x6A, 0x6B, 0x6D,
        0x71, 0x72, 0x73, 0x75, 0x79, 0x7A, 0x7B, 0x7D,
        0x8C,
        0x94, 0x9C,
        0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
        0xCC,
        0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
    };
    for (const auto opc : illegals)
        UNKN(opc);
}
// clang-format on

void run_tests(const char *cpu) {
    disassembler.setCpu(cpu);
    RUN_TEST(test_implied);
    RUN_TEST(test_immediate);
    RUN_TEST(test_absolute);
    RUN_TEST(test_direct);
    RUN_TEST(test_relative);
    RUN_TEST(test_indexed);
    RUN_TEST(test_auto_indexed);
    RUN_TEST(test_formatter);
    RUN_TEST(test_illegal);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

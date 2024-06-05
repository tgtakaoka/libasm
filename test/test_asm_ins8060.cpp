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

#include "asm_ins8060.h"
#include "test_asm_helper.h"

using namespace libasm;
using namespace libasm::ins8060;
using namespace libasm::test;

AsmIns8060 asm8060;
Assembler &assembler(asm8060);

static void set_up() {
    assembler.reset();
}

static void tear_down() {
    symtab.reset();
}

// clang-format off
void test_cpu() {
    EQUALS("cpu sc/mp", true,    assembler.setCpu("SC/MP"));
    EQUALS_P("cpu sc/mp", "SC/MP", assembler.config().cpu_P());

    EQUALS("cpu 8060", true,    assembler.setCpu("8060"));
    EQUALS_P("cpu 8060", "SC/MP", assembler.config().cpu_P());

    EQUALS("cpu INS8060", true,    assembler.setCpu("INS8060"));
    EQUALS_P("cpu INS8060", "SC/MP", assembler.config().cpu_P());
}

static void test_inherent() {
    TEST("HALT", 0x00);
    TEST("XAE",  0x01);
    TEST("CCL",  0x02);
    TEST("SCL",  0x03);
    TEST("DINT", 0x04);
    TEST("IEN",  0x05);
    TEST("CSA",  0x06);
    TEST("CAS",  0x07);
    TEST("NOP",  0x08);
    TEST("SIO",  0x19);
    TEST("SR",   0x1C);
    TEST("SRL",  0x1D);
    TEST("RR",   0x1E);
    TEST("RRL",  0x1F);
    TEST("LDE",  0x40);
    TEST("ANE",  0x50);
    TEST("ORE",  0x58);
    TEST("XRE",  0x60);
    TEST("DAE",  0x68);
    TEST("ADE",  0x70);
    TEST("CAE",  0x78);
}

static void test_pointer() {
    TEST("xpal pc", 0x30);
    TEST("XPAL P1", 0x31);
    TEST("XPAL P2", 0x32);
    TEST("XPAL P3", 0x33);
    TEST("XPAH PC", 0x34);
    TEST("XPAH P1", 0x35);
    TEST("XPAH P2", 0x36);
    TEST("XPAH P3", 0x37);
    TEST("XPPC PC", 0x3C);
    TEST("XPPC P1", 0x3D);
    TEST("XPPC P2", 0x3E);
    TEST("XPPC P3", 0x3F);
}

static void test_immediate() {
    TEST("DLY 0x12", 0x8F, 0x12);
}

static void test_jump() {
    ATEST(0x1000, "JMP 0x1000",  0x90, 0xFE);
    ATEST(0x1000, "JP  0x1081",  0x94, 0x7F);
    ATEST(0x1000, "JMP x'1000",  0x90, 0xFE);
    ATEST(0x1000, "JP  x'1081'", 0x94, 0x7F);
    ATEST(0x1000, "JMP .+2",     0x90, 0x00);
    AERRT(0x1000, "JNZ E(PC)", REGISTER_NOT_ALLOWED, "E(PC)", 0x9C, 0x80);
    AERRT(0x1000, "JNZ E(P0)", REGISTER_NOT_ALLOWED, "E(P0)", 0x9C, 0x80);
    ATEST(0x1800, "JNZ x'1782'",                              0x9C, 0x80);

    TEST("JMP 127(PC)",                              0x90, 0x7F);
    ERRT("JMP 128(PC)",  OVERFLOW_RANGE, "128(PC)",  0x90, 0x80);
    TEST("JMP -127(PC)",                             0x90, 0x81);
    TEST("JMP -128(PC)",                             0x90, 0x80);
    ERRT("JMP E(PC)", REGISTER_NOT_ALLOWED, "E(PC)", 0x90, 0x80);
    ERRT("JMP E(P0)", REGISTER_NOT_ALLOWED, "E(P0)", 0x90, 0x80);
    TEST("JMP 127(P3)",                              0x93, 0x7F);
    ERRT("JMP 128(P3)",  OVERFLOW_RANGE, "128(P3)",  0x93, 0x80);
    TEST("JMP -127(P1)",                             0x91, 0x81);
    ERRT("JMP -128(P3)", OVERFLOW_RANGE, "-128(P3)", 0x93, 0x80);
    TEST("JMP E(P1)",                                0x91, 0x80);

    symtab.intern(0x0FF0, "label0FF0");
    symtab.intern(0x1000, "label1000");
    symtab.intern(0x1036, "$1");
    symtab.intern(127,    "disp0x7F");
    symtab.intern(-127,   "disp0x81");
    symtab.intern(-128,   "disp0x80");

    ATEST(0x1000, "JMP label1000", 0x90, 0xFE);
    AERRT(0x1000, "JMP label0FF0", OVERWRAP_SEGMENT, "label0FF0", 0x90, 0xEE);
    ATEST(0x1024, "JMP $1",        0x90, 0x10);

    TEST("JMP disp0x80(PC)", 0x90, 0x80);
    TEST("JMP disp0x7F(P1)", 0x91, 0x7F);
    TEST("JMP disp0x81(P2)", 0x92, 0x81);
    TEST("JMP E(P3)",        0x93, 0x80);
}

static void test_incr_decr() {
    ATEST(0x1000, "ILD 0x1000", 0xA8, 0xFF);
    ERRT("ILD @1(P1)",   OPERAND_NOT_ALLOWED, "@1(P1)");
    ERRT("DLD @E(P1)",   OPERAND_NOT_ALLOWED, "@E(P1)");
    ERRT("ILD E(P0)",   REGISTER_NOT_ALLOWED, "E(P0)", 0xA8, 0x80);
    TEST("ILD -128(P0)", 0xA8, 0x80);
    TEST("ILD E(P1)",    0xA9, 0x80);
    TEST("ILD 127(P2)",  0xAA, 0x7F);
    TEST("ILD -127(P3)", 0xAB, 0x81);
    ERRT("ILD -128(P3)", OVERFLOW_RANGE, "-128(P3)", 0xAB, 0x80);

    ATEST(0x1000, "DLD 0x1000", 0xB8, 0xFF);
    ERRT("DLD E(PC)", REGISTER_NOT_ALLOWED, "E(PC)", 0xB8, 0x80);
    TEST("DLD -128(PC)",                             0xB8, 0x80);
    TEST("DLD E(P1)",    0xB9, 0x80);
    TEST("DLD 127(P2)",  0xBA, 0x7F);
    TEST("DLD -127(P3)", 0xBB, 0x81);
    ERRT("DLD -128(P3)", OVERFLOW_RANGE, "-128(P3)", 0xBB, 0x80);

    symtab.intern(0x1000, "label1000");
    symtab.intern(0x2010, "label2010");
    symtab.intern(127,    "disp0x7F");
    symtab.intern(-127,   "disp0x81");
    symtab.intern(-128,   "disp0x80");

    ATEST(0x1000, "ILD label1000", 0xA8, 0xFF);
    AERRT(0x1FF0, "ILD label2010", OVERWRAP_SEGMENT, "label2010", 0xA8, 0x1F);
    ERRT("ILD E(P0)", REGISTER_NOT_ALLOWED, "E(P0)", 0xA8, 0x80);
    TEST("ILD -128(P0)", 0xA8, 0x80);
    TEST("ILD E(P1)",    0xA9, 0x80);
    TEST("ILD disp0x7F(P2)", 0xAA, 0x7F);
    TEST("ILD disp0x81(P3)", 0xAB, 0x81);
}

static void test_alu() {
    ATEST(0x1000, "LD 0x1000", 0xC0, 0xFF);
    TEST("LD 127(P2)",                               0xC2, 0x7F);
    ERRT("LD 128(P2)",   OVERFLOW_RANGE, "128(P2)",  0xC2, 0x80);
    TEST("LD -127(P3)",                              0xC3, 0x81);
    ERRT("LD -128(P3)",  OVERFLOW_RANGE, "-128(P3)", 0xC3, 0x80);
    TEST("LD E(P1)",                                 0xC1, 0x80);
    ERRT("LD E(PC)", REGISTER_NOT_ALLOWED, "E(PC)",  0xC0, 0x80);
    ERRT("LD E(P0)", REGISTER_NOT_ALLOWED, "E(P0)",  0xC0, 0x80);
    TEST("LD -128(PC)",                              0xC0, 0x80);
    TEST("LD -128(PC)",                              0xC0, 0x80);
    TEST("LD @127(P2)",                               0xC6, 0x7F);
    ERRT("LD @128(P2)",  OVERFLOW_RANGE, "@128(P2)",  0xC6, 0x80);
    TEST("LD @-127(P3)",                              0xC7, 0x81);
    ERRT("LD @-128(P3)", OVERFLOW_RANGE, "@-128(P3)", 0xC7, 0x80);
    TEST("LD @E(P1)",                                 0xC5, 0x80);

    ATEST(0x1000, "ST  0x1000", 0xC8, 0xFF);
    ERRT("XOR E(PC)", REGISTER_NOT_ALLOWED, "E(PC)", 0xE0, 0x80);
    TEST("XOR -128(PC)",  0xE0, 0x80);
    TEST("DAD E(P1)",     0xE9, 0x80);
    TEST("ADD 127(P2)",   0xF2, 0x7F);
    TEST("CAD -127(P3)",  0xFB, 0x81);
    TEST("ST  @E(P1)",    0xCD, 0x80);
    TEST("AND @127(P2)",  0xD6, 0x7F);
    TEST("OR  @-127(P3)", 0xDF, 0x81);

    symtab.intern(0x1000, "label1000");
    symtab.intern(127,    "disp0x7F");
    symtab.intern(-127,   "disp0x81");
    symtab.intern(-128,   "disp0x80");

    ATEST(0x1000, "ST label1000", 0xC8, 0xFF);
    TEST("AND @disp0x7F(P2)", 0xD6, 0x7F);
    TEST("OR  @disp0x81(P3)", 0xDF, 0x81);
}

static void test_alu_immediate() {
    TEST("LDI 0",                            0xC4, 0x00);
    TEST("LDI 255",                          0xC4, 0xFF);
    ERRT("LDI 256",  OVERFLOW_RANGE, "256",  0xC4, 0x00);
    TEST("LDI -128",                         0xC4, 0x80);
    ERRT("LDI -129", OVERFLOW_RANGE, "-129", 0xC4, 0x7F);
    TEST("LDI x'FF",                         0xC4, 0xFF);
    TEST("LDI x'FF'",                        0xC4, 0xFF);
    TEST("ANI 0xFF", 0xD4, 0xFF);
    TEST("ORI 1",    0xDC, 0x01);
    TEST("XRI 0x80", 0xE4, 0x80);
    TEST("DAI 0x99", 0xEC, 0x99);
    TEST("ADI 0x12", 0xF4, 0x12);
    TEST("CAI 0x34", 0xFC, 0x34);

    symtab.intern(-1, "$minus1");

    ATEST(0x1024, "LDI $minus1",      0xC4, 0xFF);
    ATEST(0x1024, "LDI L($)+$minus1", 0xC4, 0x23);
}

static void test_page_boundary() {
    ATEST(0x1FFE, "LDI 0",                        0xC4, 0x00);
    AERRT(0x1FFF, "LDI 0", OVERWRAP_SEGMENT, "0", 0xC4, 0x00);

    ATEST(0x1010, "LD 0x1000",                             0xC0, 0xEF);
    AERRT(0x1010, "LD 0x0FFF", OVERWRAP_SEGMENT, "0x0FFF", 0xC0, 0xEE);
    ATEST(0x1010, "LD 0x1FFF",                             0xC0, 0xEE);
    ATEST(0x1010, "LD 0x1F92",                             0xC0, 0x81);
    ATEST(0x1010, "LD 0x1F91",                             0xC0, 0x80);

    ATEST(0x1FF0, "LD 0x1FFF",                             0xC0, 0x0E);
    AERRT(0x1FF0, "LD 0x2000", OVERWRAP_SEGMENT, "0x2000", 0xC0, 0x0F);
    ATEST(0x1FF0, "LD 0x1000",                             0xC0, 0x0F);
    ATEST(0x1FF0, "LD 0x1070",                             0xC0, 0x7F);
    AERRT(0x1FF0, "LD 0x1071", OPERAND_TOO_FAR,  "0x1071", 0xC0, 0x80);

    ATEST(0x1010, "JZ 0x1000",                             0x98, 0xEE);
    AERRT(0x1010, "JZ 0x0FFF", OVERWRAP_SEGMENT, "0x0FFF", 0x98, 0xED);
    ATEST(0x1010, "JZ 0x1FFF",                             0x98, 0xED);
    ATEST(0x1010, "JZ 0x1F93",                             0x98, 0x81);
    ATEST(0x1010, "JZ 0x1F92",                             0x98, 0x80);

    ATEST(0x1FF0, "JZ 0x1FFF",                             0x98, 0x0D);
    AERRT(0x1FF0, "JZ 0x2000", OVERWRAP_SEGMENT, "0x2000", 0x98, 0x0E);
    ATEST(0x1FF0, "JZ 0x1000",                             0x98, 0x0E);
    ATEST(0x1FF0, "JZ 0x1071",                             0x98, 0x7F);
    AERRT(0x1FF0, "JZ 0x1072", OPERAND_TOO_FAR,  "0x1072", 0x98, 0x80);
}

static void test_func() {
    symtab.intern(0x1FFF, "stack");
    symtab.intern(0x1000, "main");
    symtab.intern(0x3210, "func");

    TEST("LDI L(stack)",      0xC4, 0xFF);
    TEST("LDI H(stack)",      0xC4, 0x1F);
    TEST("LDI l(ADDR(main))", 0xC4, 0xFF); // addr(main) == 0x1FFF
    TEST("LDI h(ADDR(main))", 0xC4, 0x1F);
    TEST("LDI L(addr(func))", 0xC4, 0x0F); // addr(func) == 0x320F
    TEST("LDI H(addr(func))", 0xC4, 0x32);
}

static void test_comment() {
    symtab.intern(-127, "m127");
    symtab.intern(127,  "p127");

    ERRT("NOP       ; comment", OK, "; comment", 0x08);
    ERRT("XPPC P3   ; comment", OK, "; comment", 0x3F);
    ERRT("DLY  0x12 ; comment", OK, "; comment", 0x8F, 0x12);
    AERRT(0x1000, "JMP 0x1000 ; comment", OK, "; comment", 0x90, 0xFE);
    AERRT(0x1000, "JNZ E (PC) ; comment", REGISTER_NOT_ALLOWED, "E (PC) ; comment", 0x9C, 0x80);
    ERRT("JMP E (P1)    ; comment", OK, "; comment", 0x91, 0x80);
    ERRT("JMP p127 (P2) ; comment", OK, "; comment", 0x92, 0x7F);
    ERRT("JMP m127 (P3) ; comment", OK, "; comment", 0x93, 0x81);
    ERRT("LD @ 126 (P3) ; comment", OK, "; comment", 0xC7, 0x7E);
}

static void test_undef() {
    ERUS("DLY UNDEF", "UNDEF", 0x8F, 0x00);
    ERUS("LD  UNDEF", "UNDEF", 0xC0, 0x00);
    ERUS("LD  UNDEF(PC)", "UNDEF(PC)", 0xC0, 0x00);
    ERUS("LD  UNDEF(P1)", "UNDEF(P1)", 0xC1, 0x00);
    ERUS("LD @UNDEF(P1)", "UNDEF(P1)", 0xC5, 0x00);
    ERUS("LDI UNDEF", "UNDEF", 0xC4, 0x00);

    ERUS("JMP UNDEF", "UNDEF", 0x90, 0x00);
}

static void test_error() {
    ERUS("LD (P3)", "P3)", 0xC0, 0x00);
    ERUS("LD (E)",  "E)",  0xC0, 0x00);
    ERRT("LD 1(E)", UNKNOWN_OPERAND, "1(E)");
    ERRT("LDI #1",  NOT_AN_EXPECTED, "#1");
    ERRT("LD 1(P3 ;comment", MISSING_CLOSING_PAREN, " ;comment");
}

static void test_data_constant() {
    TEST(".byte  -128, 255", 0x80, 0xFF);
    TEST(".byte  'A', '\"'", 0x41, 0x22);
    TEST(".byte  '9'-'0'",   0x09);
    TEST(".byte  ''''",      0x27);
    ERRT(".byte  '''",       MISSING_CLOSING_QUOTE, "'''");
    TEST(".byte  'A''B',0",  0x41, 0x27, 0x42, 0x00);
    ERRT(".byte  'A''B,0",   MISSING_CLOSING_QUOTE, "'A''B,0");
    TEST(".ascii 'A''B',0",  0x41, 0x27, 0x42, 0x00);
    ERRT(".ascii 'A''B,0",   MISSING_CLOSING_QUOTE, "'A''B,0");
    TEST(".dbyte -128, 255", 0x80, 0xFF, 0xFF, 0x00);
    TEST(".dbyte 'A''B'",    0x41, 0x27, 0x42, 0x00);
    ERRT(".dbyte 'A''B",     MISSING_CLOSING_QUOTE, "'A''B");
    TEST(".addr  x'0100",    0xFF, 0x00);
    TEST(".addr  x'1000",    0xFF, 0x1F);
    ERUS(".byte  1, UNDEF", "UNDEF", 0x01, 0x00);
    ERUS(".dbyte 1, UNDEF", "UNDEF", 0x01, 0x00, 0x00, 0x00);
    ERUS(".addr  2, UNDEF", "UNDEF", 0x01, 0x00, 0xFF, 0x0F);
    
    ERRT(".byte '"
         "1234567890" "1234567890" "1234567890" "1234567890" "1234567890" "1234567890"
         "1234567890'",
         NO_MEMORY,
         "567890'",
         0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30,
         0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30,
         0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30,
         0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30,
         0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30,
         0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30,
         0x31, 0x32, 0x33, 0x34);

    ERRT(".dbyte "
         "X'1234, X'5678, X'9ABC, X'DEF0, X'1234, X'5678, X'9ABC, X'DEF0, "
         "X'1234, X'5678, X'9ABC, X'DEF0, X'1234, X'5678, X'9ABC, X'DEF0, "
         "X'1234, X'5678, X'9ABC, X'DEF0, X'1234, X'5678, X'9ABC, X'DEF0, "
         "X'1234, X'5678, X'9ABC, X'DEF0, X'1234, X'5678, X'9ABC, X'DE, "
         "X'AAAA, X'BBBB",
         NO_MEMORY,
         "X'AAAA, X'BBBB",
         0x34, 0x12, 0x78, 0x56, 0xBC, 0x9A, 0xF0, 0xDE, 0x34, 0x12, 0x78, 0x56, 0xBC, 0x9A, 0xF0, 0xDE,
         0x34, 0x12, 0x78, 0x56, 0xBC, 0x9A, 0xF0, 0xDE, 0x34, 0x12, 0x78, 0x56, 0xBC, 0x9A, 0xF0, 0xDE,
         0x34, 0x12, 0x78, 0x56, 0xBC, 0x9A, 0xF0, 0xDE, 0x34, 0x12, 0x78, 0x56, 0xBC, 0x9A, 0xF0, 0xDE,
         0x34, 0x12, 0x78, 0x56, 0xBC, 0x9A, 0xF0, 0xDE, 0x34, 0x12, 0x78, 0x56, 0xBC, 0x9A, 0xDE, 0x00);
}

// clang-format on

void run_tests(const char *cpu) {
    assembler.setCpu(cpu);
    RUN_TEST(test_inherent);
    RUN_TEST(test_pointer);
    RUN_TEST(test_immediate);
    RUN_TEST(test_jump);
    RUN_TEST(test_incr_decr);
    RUN_TEST(test_alu);
    RUN_TEST(test_alu_immediate);
    RUN_TEST(test_page_boundary);
    RUN_TEST(test_func);
    RUN_TEST(test_comment);
    RUN_TEST(test_undef);
    RUN_TEST(test_error);
    RUN_TEST(test_data_constant);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

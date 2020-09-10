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

static void test_cpu() {
    EQUALS("cpu sc/mp", true,    assembler.setCpu("SC/MP"));
    EQUALS("cpu sc/mp", "SC/MP", assembler.getCpu());

    EQUALS("cpu 8060", true,    assembler.setCpu("8060"));
    EQUALS("cpu 8060", "SC/MP", assembler.getCpu());

    EQUALS("cpu INS8060", true,    assembler.setCpu("INS8060"));
    EQUALS("cpu INS8060", "SC/MP", assembler.getCpu());
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
    TEST("XPAL PC", 0x30);
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
    ATEST(0x1000, "JMP 0x1000",   0x90, 0xFE);
    ATEST(0x1000, "JP  0x1081",   0x94, 0x7F);
    ATEST(0x1000, "JMP $+2",      0x90, 0x00);
    // 4kB page boundary
    EATEST(OPERAND_TOO_FAR, 0x1000, "JZ 0x0FF0");
    EATEST(OPERAND_TOO_FAR, 0x1FF0, "JZ 0x2010");
    ATEST(0x1000, "JNZ E(PC)",    0x9C, 0x80);

    TEST("JMP E(PC)",    0x90, 0x80);
    TEST("JMP E(P1)",    0x91, 0x80);
    TEST("JMP 127(P2)",  0x92, 0x7F);
    TEST("JMP -127(P3)", 0x93, 0x81);

    symtab.intern(0x0FF0, "label0FF0");
    symtab.intern(0x1000, "label1000");
    symtab.intern(127,    "disp0x7F");
    symtab.intern(-127,   "disp0x81");
    symtab.intern(-128,   "disp0x80");

    ATEST(0x1000, "JMP label1000",    0x90, 0xFE);
    EATEST(OPERAND_TOO_FAR, 0x1000, "JMP label0FF0");
    
    TEST("JMP E(PC)",        0x90, 0x80);
    TEST("JMP disp0x7F(P1)", 0x91, 0x7F);
    TEST("JMP disp0x81(P2)", 0x92, 0x81);
    TEST("JMP E(P3)",        0x93, 0x80);
}

static void test_incr_decr() {
    ATEST(0x1000, "ILD 0x1000", 0xA8, 0xFF);
    // 4kB page boundary
    EATEST(OPERAND_TOO_FAR, 0x1000, "ILD 0x0F00");
    EATEST(OPERAND_TOO_FAR, 0x1FF0, "ILD 0x2010");
    ETEST(OPERAND_NOT_ALLOWED, "ILD @1(P1)");
    ETEST(OPERAND_NOT_ALLOWED, "DLD @E(P1)");
    TEST("ILD E(PC)",    0xA8, 0x80);
    TEST("ILD E(P1)",    0xA9, 0x80);
    TEST("ILD 127(P2)",  0xAA, 0x7F);
    TEST("ILD -127(P3)", 0xAB, 0x81);

    ATEST(0x1000, "DLD 0x1000", 0xB8, 0xFF);
    // 4kB page boundary
    EATEST(OPERAND_TOO_FAR, 0x1000, "DLD 0x0F00");
    EATEST(OPERAND_TOO_FAR, 0x1FF0, "DLD 0x2010");
    TEST("DLD E(PC)",    0xB8, 0x80);
    TEST("DLD E(P1)",    0xB9, 0x80);
    TEST("DLD 127(P2)",  0xBA, 0x7F);
    TEST("DLD -127(P3)", 0xBB, 0x81);

    symtab.intern(0x1000, "label1000");
    symtab.intern(0x2010, "label2010");
    symtab.intern(127,    "disp0x7F");
    symtab.intern(-127,   "disp0x81");
    symtab.intern(-128,   "disp0x80");

    ATEST(0x1000, "ILD label1000", 0xA8, 0xFF);
    EATEST(OPERAND_TOO_FAR, 0x1FF0, "ILD label2010");
    TEST("ILD E(PC)",    0xA8, 0x80);
    TEST("ILD E(P1)",    0xA9, 0x80);
    TEST("ILD disp0x7F(P2)", 0xAA, 0x7F);
    TEST("ILD disp0x81(P3)", 0xAB, 0x81);
}

static void test_alu() {
    ATEST(0x1000, "LD 0x1000", 0xC0, 0xFF);
    // 4kB page boundary
    EATEST(OPERAND_TOO_FAR, 0x1000, "LD 0x0FF0");
    EATEST(OPERAND_TOO_FAR, 0x1FF0, "LD 0x2010");
    TEST("LD E(PC)",     0xC0, 0x80);
    TEST("LD E(P1)",     0xC1, 0x80);
    TEST("LD 127(P2)",   0xC2, 0x7F);
    TEST("LD -127(P3)",  0xC3, 0x81);
    TEST("LD @E(P1)",    0xC5, 0x80);
    TEST("LD @127(P2)",  0xC6, 0x7F);
    TEST("LD @-127(P3)", 0xC7, 0x81);

    ATEST(0x1000, "ST  0x1000", 0xC8, 0xFF);
    // 4kB page boundary
    EATEST(OPERAND_TOO_FAR, 0x1000, "AND 0x0FF0");
    EATEST(OPERAND_TOO_FAR, 0x1FF0, "OR  0x2010");
    TEST("XOR E(PC)",     0xE0, 0x80);
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
    TEST("LDI 0",    0xC4, 0x00);
    TEST("ANI 0xFF", 0xD4, 0xFF);
    TEST("ORI 1",    0xDC, 0x01);
    TEST("XRI 0x80", 0xE4, 0x80);
    TEST("DAI 0x99", 0xEC, 0x99);
    TEST("ADI 0x12", 0xF4, 0x12);
    TEST("CAI 0x34", 0xFC, 0x34);

    symtab.intern(-1, "minus1");

    TEST("LDI minus1", 0xC4, 0xFF);
}

static void test_comment() {
    symtab.intern(-127, "m127");
    symtab.intern(127,  "p127");

    TEST("NOP       ; comment", 0x08);
    TEST("XPPC P3   ; comment", 0x3F);
    TEST("DLY  0x12 ; comment", 0x8F, 0x12);
    ATEST(0x1000, "JMP 0x1000 ; comment", 0x90, 0xFE);
    ATEST(0x1000, "JNZ E (PC) ; comment", 0x9C, 0x80);
    TEST("JMP E (P1)    ; comment", 0x91, 0x80);
    TEST("JMP p127 (P2) ; comment", 0x92, 0x7F);
    TEST("JMP m127 (P3) ; comment", 0x93, 0x81);
    TEST("LD @ 126 (P3) ; comment", 0xC7, 0x7E);
}

static void test_undefined_symbol() {
    ETEST(UNDEFINED_SYMBOL, "DLY UNDEF", 0x8F, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LD  UNDEF", 0xC0, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LD  UNDEF(PC)", 0xC0, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LD  UNDEF(P1)", 0xC1, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LD @UNDEF(P1)", 0xC5, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LDI UNDEF",     0xC4, 0x00);

    ETEST(UNDEFINED_SYMBOL, "JMP UNDEF", 0x90, 0x00);
}

static void test_error() {
    ETEST(UNDEFINED_SYMBOL, "LD (P3)", 0xC0, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LD (E)",  0xC0, 0x00);
    ETEST(UNKNOWN_OPERAND, "LD 1(E)");
    ETEST(MISSING_CLOSING_PAREN, "LD 1(P3");
    ETEST(ILLEGAL_CONSTANT, "LDI #1");
}

static void run_test(void (*test)(), const char *test_name) {
    asserter.clear(test_name);
    set_up();
    test();
    tear_down();
    asserter.check();
}

int main(int argc, char **argv) {
    RUN_TEST(test_cpu);
    RUN_TEST(test_inherent);
    RUN_TEST(test_pointer);
    RUN_TEST(test_immediate);
    RUN_TEST(test_jump);
    RUN_TEST(test_incr_decr);
    RUN_TEST(test_alu);
    RUN_TEST(test_alu_immediate);
    RUN_TEST(test_comment);
    RUN_TEST(test_undefined_symbol);
    RUN_TEST(test_error);
    return 0;
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

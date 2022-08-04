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

#include "asm_ins8070.h"
#include "test_asm_helper.h"

using namespace libasm;
using namespace libasm::ins8070;
using namespace libasm::test;

AsmIns8070 asm8070;
Assembler &assembler(asm8070);

static void set_up() {
    assembler.reset();
}

static void tear_down() {
    symtab.reset();
}

// clang-format off
void test_cpu() {
    EQUALS("cpu 8070", true,   assembler.setCpu("8070"));
    EQUALS_P("cpu 8070", "8070", assembler.cpu_P());

    EQUALS("cpu INS8070", true,   assembler.setCpu("INS8070"));
    EQUALS_P("cpu INS8070", "8070", assembler.cpu_P());
}

static void test_implied() {
    TEST("NOP  ",      0x00);
    TEST("XCH  A,E",   0x01);
    TEST("LD   A,S",   0x06);
    TEST("LD   S,A",   0x07);
    TEST("PUSH EA",    0x08);
    TEST("LD   T,EA",  0x09);
    TEST("PUSH A",     0x0A);
    TEST("LD   EA,T",  0x0B);
    TEST("SR   EA",    0x0C);
    TEST("DIV  EA,T",  0x0D);
    TEST("SL   A",     0x0E);
    TEST("SL   EA",    0x0F);
    TEST("CALL 0",     0x10);
    TEST("CALL 1",     0x11);
    TEST("CALL 2",     0x12);
    TEST("CALL 3",     0x13);
    TEST("CALL 4",     0x14);
    TEST("CALL 5",     0x15);
    TEST("CALL 6",     0x16);
    TEST("CALL 7",     0x17);
    TEST("CALL 8",     0x18);
    TEST("CALL 9",     0x19);
    TEST("CALL 10",    0x1A);
    TEST("CALL 11",    0x1B);
    TEST("CALL 12",    0x1C);
    TEST("CALL 13",    0x1D);
    TEST("CALL 14",    0x1E);
    TEST("CALL 15",    0x1F);
    ERRT("CALL -1",    OPERAND_NOT_ALLOWED, "-1");
    ERRT("CALL 16",    OPERAND_NOT_ALLOWED, "16");
    TEST("MPY  EA,T",  0x2C);
    TEST("SSM  P2",    0x2E);
    TEST("SSM  P3",    0x2F);
    TEST("LD   EA,PC", 0x30);
    TEST("LD   EA,SP", 0x31);
    TEST("LD   EA,P2", 0x32);
    TEST("LD   EA,P3", 0x33);
    TEST("POP  A",     0x38);
    TEST("POP  EA",    0x3A);
    TEST("SR   A",     0x3C);
    TEST("SRL  A",     0x3D);
    TEST("RR   A",     0x3E);
    TEST("RRL  A",     0x3F);
    TEST("LD   A,E",   0x40);
    TEST("LD   PC,EA", 0x44);
    TEST("LD   SP,EA", 0x45);
    TEST("LD   P2,EA", 0x46);
    TEST("LD   P3,EA", 0x47);
    TEST("LD   E,A",   0x48);
    TEST("XCH  EA,PC", 0x4C);
    TEST("XCH  EA,SP", 0x4D);
    TEST("XCH  EA,P2", 0x4E);
    TEST("XCH  EA,P3", 0x4F);
    TEST("PUSH PC",    0x54);
    ERRT("PUSH SP",    OPERAND_NOT_ALLOWED, "SP");
    TEST("PUSH P2",    0x56);
    TEST("PUSH P3",    0x57);
    TEST("OR   A,E",   0x58);
    TEST("RET  ",      0x5C);
    TEST("POP  P2",    0x5E);
    TEST("POP  P3",    0x5F);
    TEST("XOR  A,E",   0x60);
    TEST("ADD  A,E",   0x70);
    TEST("SUB  A,E",   0x78);
}

static void test_immediate() {
    TEST("AND S,=0x12", 0x39, 0x12);
    TEST("AND S,=-128", 0x39, 0x80);
    TEST("AND S,=255",  0x39, 0xFF);
    ERRT("AND S,=-129", OVERFLOW_RANGE, "=-129");
    ERRT("AND S,=256",  OVERFLOW_RANGE, "=256");
    TEST("OR  S,=0x34", 0x3B, 0x34);
    TEST("LD  A,=0x12", 0xC4, 0x12);
    ERRT("ST  A,=0x12", OPERAND_NOT_ALLOWED, "A,=0x12");
    TEST("AND A,=0x12", 0xD4, 0x12);
    TEST("OR  A,=0x12", 0xDC, 0x12);
    TEST("XOR A,=0x12", 0xE4, 0x12);
    TEST("ADD A,=0x12", 0xF4, 0x12);
    TEST("SUB A,=0x12", 0xFC, 0x12);

    TEST("PLI P2,=0x1234", 0x22, 0x34, 0x12);
    TEST("PLI P3,=0x1234", 0x23, 0x34, 0x12);
    TEST("LD  SP,=0x1234", 0x25, 0x34, 0x12);
    TEST("LD  P2,=0x1234", 0x26, 0x34, 0x12);
    TEST("LD  P3,=0x1234", 0x27, 0x34, 0x12);
    TEST("LD  EA,=0x1234", 0x84, 0x34, 0x12);
    ERRT("ST  EA,=0x1234", OPERAND_NOT_ALLOWED, "EA,=0x1234");
    TEST("LD  T,=0x1234",  0xA4, 0x34, 0x12);
    TEST("ADD EA,=0x1234", 0xB4, 0x34, 0x12);
    TEST("SUB EA,=0x1234", 0xBC, 0x34, 0x12);

    ERRT("ILD =0x1234", OPERAND_NOT_ALLOWED, "=0x1234");
    ERRT("DLD =0x1234", OPERAND_NOT_ALLOWED, "=0x1234");

    symtab.intern(0x12,   "sym12");
    symtab.intern(0x1234, "sym1234");

    TEST("AND S,=sym12",    0x39, 0x12);
    TEST("LD  A,=sym12",    0xC4, 0x12);
    TEST("AND A,=sym12",    0xD4, 0x12);
    TEST("SUB A,=sym12",    0xFC, 0x12);

    TEST("PLI P2,=sym1234", 0x22, 0x34, 0x12);
    TEST("LD  SP,=sym1234", 0x25, 0x34, 0x12);
    TEST("LD  P3,=sym1234", 0x27, 0x34, 0x12);
    TEST("LD  EA,=sym1234", 0x84, 0x34, 0x12);
    TEST("LD  T,=sym1234",  0xA4, 0x34, 0x12);
    TEST("ADD EA,=sym1234", 0xB4, 0x34, 0x12);
}

static void test_absolute() {
    TEST("JSR 0x1234", 0x20, 0x33, 0x12);
    TEST("JMP 0x1234", 0x24, 0x33, 0x12);
    ATEST(0x1000, "JMP $+3", 0x24, 0x02, 0x10);

    symtab.intern(0x1234, "sym1234");

    TEST("JSR sym1234", 0x20, 0x33, 0x12);
    TEST("JMP sym1234", 0x24, 0x33, 0x12);
}

static void test_direct() {
    TEST("LD  EA,0xFF34", 0x85, 0x34);
    TEST("ST  EA,0xFF34", 0x8D, 0x34);
    TEST("ILD A,0xFF34",  0x95, 0x34);
    TEST("DLD A,0xFF34",  0x9D, 0x34);
    TEST("LD  T,0xFF34",  0xA5, 0x34);
    TEST("ADD EA,0xFF34", 0xB5, 0x34);
    TEST("SUB EA,0xFF34", 0xBD, 0x34);
    TEST("LD  A,0xFF34",  0xC5, 0x34);
    TEST("AND A,0xFF34",  0xD5, 0x34);
    TEST("OR  A,0xFF34",  0xDD, 0x34);
    TEST("XOR A,0xFF34",  0xE5, 0x34);
    TEST("ADD A,0xFF34",  0xF5, 0x34);
    TEST("SUB A,0xFF34",  0xFD, 0x34);

    symtab.intern(0xFF34, "dir34");
    symtab.intern(0xFF00, "dir00");
    symtab.intern(0xFFFF, "dirFF");

    TEST("LD  EA,dir34", 0x85, 0x34);
    TEST("ST  EA,dir34", 0x8D, 0x34);
    TEST("ILD A,dir00",  0x95, 0x00);
    TEST("DLD A,dirFF",  0x9D, 0xFF);
    TEST("SUB EA,dir00", 0xBD, 0x00);
    TEST("LD  A,dir34",  0xC5, 0x34);
}

static void test_relative() {
    ATEST(0x1000, "BP  0x1000", 0x64, 0xFE);
    ATEST(0x1100, "BZ  0x1082", 0x6C, 0x80);
    ATEST(0x1000, "BRA 0x1081", 0x74, 0x7F);
    ATEST(0x1000, "BNZ 0x1005", 0x7C, 0x03);
    ATEST(0x1000, "BRA $",      0x74, 0xFE);

    symtab.intern(0x1000, "sym1000");
    symtab.intern(0x1005, "sym1005");
    symtab.intern(0x1081, "sym1081");
    symtab.intern(0x1082, "sym1082");

    ATEST(0x1000, "BP  sym1000", 0x64, 0xFE);
    ATEST(0x1100, "BZ  sym1082", 0x6C, 0x80);
    ATEST(0x1000, "BRA sym1081", 0x74, 0x7F);
    ATEST(0x1000, "BNZ sym1005", 0x7C, 0x03);
}

static void test_indexed() {
    TEST("BP  0,P2",    0x66, 0x00);
    TEST("BP  -128,P3", 0x67, 0x80);
    TEST("BZ  127,P2",  0x6E, 0x7F);
    TEST("BZ  0,P3",    0x6F, 0x00);
    TEST("BRA 2,P2",    0x76, 0x02);
    TEST("BRA 3,P3",    0x77, 0x03);
    TEST("BNZ -2,P2",   0x7E, 0xFE);
    TEST("BNZ -3,P3",   0x7F, 0xFD);
    ERRT("BRA -129,P2", OVERFLOW_RANGE, "-129,P2");
    ERRT("BRA 128,P3",  OVERFLOW_RANGE, "128,P3");

    ATEST(0x1000, "LD  EA,0x1000,PC", 0x80, 0xFF);
    ATEST(0x1000, "LD  EA,0x0F81,PC", 0x80, 0x80);
    ATEST(0x1000, "LD  EA,0x1080,PC", 0x80, 0x7F);
    AERRT(0x1000, "LD  EA,0x0F80,PC", OPERAND_TOO_FAR, "0x0F80,PC");
    AERRT(0x1000, "LD  EA,0x1081,PC", OPERAND_TOO_FAR, "0x1081,PC");
    TEST(         "LD  EA,0,SP",      0x81, 0x00);
    TEST(         "LD  EA,-128,P2",   0x82, 0x80);
    TEST(         "LD  EA,127,P3",    0x83, 0x7F);
    ERRT(         "LD  EA,-129,P2",   OVERFLOW_RANGE, "-129,P2");
    ERRT(         "LD  EA,128,P3",    OVERFLOW_RANGE, "128,P3");
    ATEST(0x1100, "ST  EA,0x1081,PC", 0x88, 0x80);
    TEST(         "ST  EA,0,SP",      0x89, 0x00);
    TEST(         "ST  EA,-128,P2",   0x8A, 0x80);
    TEST(         "ST  EA,127,P3",    0x8B, 0x7F);
    ATEST(0x1000, "LD  T,0x1080,PC",  0xA0, 0x7F);
    TEST(         "LD  T,0,SP",       0xA1, 0x00);
    TEST(         "LD  T,-128,P2",    0xA2, 0x80);
    TEST(         "LD  T,127,P3",     0xA3, 0x7F);
    ATEST(0x1000, "ADD EA,0x1005,PC", 0xB0, 0x04);
    TEST(         "ADD EA,0,SP",      0xB1, 0x00);
    TEST(         "ADD EA,-128,P2",   0xB2, 0x80);
    TEST(         "ADD EA,127,P3",    0xB3, 0x7F);
    ATEST(0x1000, "SUB EA,0x1000,PC", 0xB8, 0xFF);
    TEST(         "SUB EA,0,SP",      0xB9, 0x00);
    TEST(         "SUB EA,-128,P2",   0xBA, 0x80);
    TEST(         "SUB EA,127,P3",    0xBB, 0x7F);

    ATEST(0x1000, "ILD A,0x1000,PC",  0x90, 0xFF);
    TEST(         "ILD A,0,SP",       0x91, 0x00);
    TEST(         "ILD A,-128,P2",    0x92, 0x80);
    TEST(         "ILD A,127,P3",     0x93, 0x7F);
    ATEST(0x1000, "DLD A,0x1000,PC",  0x98, 0xFF);
    TEST(         "DLD A,0,SP",       0x99, 0x00);
    TEST(         "DLD A,-128,P2",    0x9A, 0x80);
    TEST(         "DLD A,127,P3",     0x9B, 0x7F);
    ATEST(0x1000, "LD  A,0x1000,PC",  0xC0, 0xFF);
    TEST(         "LD  A,0,SP",       0xC1, 0x00);
    TEST(         "LD  A,-128,P2",    0xC2, 0x80);
    TEST(         "LD  A,127,P3",     0xC3, 0x7F);
    ATEST(0x1000, "ST  A,0x1000,PC",  0xC8, 0xFF);
    TEST(         "ST  A,0,SP",       0xC9, 0x00);
    TEST(         "ST  A,-128,P2",    0xCA, 0x80);
    TEST(         "ST  A,127,P3",     0xCB, 0x7F);
    ATEST(0x1000, "AND A,0x1000,PC",  0xD0, 0xFF);
    TEST(         "AND A,0,SP",       0xD1, 0x00);
    TEST(         "AND A,-128,P2",    0xD2, 0x80);
    TEST(         "AND A,127,P3",     0xD3, 0x7F);
    ATEST(0x1000, "OR  A,0x1000,PC",  0xD8, 0xFF);
    TEST(         "OR  A,0,SP",       0xD9, 0x00);
    TEST(         "OR  A,-128,P2",    0xDA, 0x80);
    TEST(         "OR  A,127,P3",     0xDB, 0x7F);
    ATEST(0x1000, "XOR A,0x1000,PC",  0xE0, 0xFF);
    TEST(         "XOR A,0,SP",       0xE1, 0x00);
    TEST(         "XOR A,-128,P2",    0xE2, 0x80);
    TEST(         "XOR A,127,P3",     0xE3, 0x7F);
    ATEST(0x1000, "ADD A,0x1000,PC",  0xF0, 0xFF);
    TEST(         "ADD A,0,SP",       0xF1, 0x00);
    TEST(         "ADD A,-128,P2",    0xF2, 0x80);
    TEST(         "ADD A,127,P3",     0xF3, 0x7F);
    ATEST(0x1000, "SUB A,0x1000,PC",  0xF8, 0xFF);
    TEST(         "SUB A,0,SP",       0xF9, 0x00);
    TEST(         "SUB A,-128,P2",    0xFA, 0x80);
    TEST(         "SUB A,127,P3",     0xFB, 0x7F);

    symtab.intern(0x1000, "sym1000");
    symtab.intern(0x1005, "sym1005");
    symtab.intern(0x1080, "sym1080");
    symtab.intern(0x1081, "sym1081");
    symtab.intern(-128, "neg128");
    symtab.intern(+127, "pos127");

    TEST(         "BZ  neg128,P3",     0x6F, 0x80);
    TEST(         "BRA pos127,P2",     0x76, 0x7F);
    ATEST(0x1000, "LD  EA,sym1000,PC", 0x80, 0xFF);
    ATEST(0x1100, "ST  EA,sym1081,PC", 0x88, 0x80);
    ATEST(0x1000, "LD  T,sym1080,PC",  0xA0, 0x7F);
    ATEST(0x1000, "ADD EA,sym1005,PC", 0xB0, 0x04);
    TEST(         "SUB A,neg128,SP",   0xF9, 0x80);
    TEST(         "SUB A,pos127,P2",   0xFA, 0x7F);
}

static void test_auto_indexed() {
    TEST("LD  EA,@-128,P2", 0x86, 0x80);
    TEST("LD  EA,@127,P3",  0x87, 0x7F);
    ERRT("LD  EA,@-129,P2", OVERFLOW_RANGE, "@-129,P2");
    ERRT("LD  EA,@128,P3",  OVERFLOW_RANGE, "@128,P3");
    TEST("ST  EA,@-128,P2", 0x8E, 0x80);
    TEST("ST  EA,@127,P3",  0x8F, 0x7F);
    TEST("LD  T,@-128,P2",  0xA6, 0x80);
    TEST("LD  T,@127,P3",   0xA7, 0x7F);
    TEST("ADD EA,@-128,P2", 0xB6, 0x80);
    TEST("ADD EA,@127,P3",  0xB7, 0x7F);
    TEST("SUB EA,@-128,P2", 0xBE, 0x80);
    TEST("SUB EA,@127,P3",  0xBF, 0x7F);

    TEST("ILD A,@-128,P2",  0x96, 0x80);
    TEST("ILD A,@127,P3",   0x97, 0x7F);
    TEST("DLD A,@-128,P2",  0x9E, 0x80);
    TEST("DLD A,@127,P3",   0x9F, 0x7F);
    TEST("LD  A,@-128,P2",  0xC6, 0x80);
    TEST("LD  A,@127,P3",   0xC7, 0x7F);
    TEST("ST  A,@-128,P2",  0xCE, 0x80);
    TEST("ST  A,@127,P3",   0xCF, 0x7F);
    TEST("AND A,@-128,P2",  0xD6, 0x80);
    TEST("AND A,@127,P3",   0xD7, 0x7F);
    TEST("OR  A,@-128,P2",  0xDE, 0x80);
    TEST("OR  A,@127,P3",   0xDF, 0x7F);
    TEST("XOR A,@-128,P2",  0xE6, 0x80);
    TEST("XOR A,@127,P3",   0xE7, 0x7F);
    TEST("ADD A,@-128,P2",  0xF6, 0x80);
    TEST("ADD A,@127,P3",   0xF7, 0x7F);
    TEST("SUB A,@-128,P2",  0xFE, 0x80);
    TEST("SUB A,@127,P3",   0xFF, 0x7F);

    symtab.intern(-128, "neg128");
    symtab.intern(+127, "pos127");
    symtab.intern(-1,   "neg1");

    TEST("ILD A,@neg128,P2", 0x96, 0x80);
    TEST("ST  A,@pos127,P3", 0xCF, 0x7F);
    TEST("AND A,@neg128,P2", 0xD6, 0x80);
    TEST("SUB A,@pos127,P2", 0xFE, 0x7F);
    TEST("SUB A,@neg1,P3",   0xFF, 0xFF);
}

static void test_func() {
    symtab.intern(0x1FFF, "stack");
    symtab.intern(0x1000, "main");
    symtab.intern(0x3210, "func");

    TEST("LD  A, =L(stack)",    0xC4, 0xFF);
    TEST("LD  A, =H(stack)",    0xC4, 0x1F);
    TEST("PLI P2, =ADDR(main)", 0x22, 0xFF, 0x0F); // addr(main) == 0x0FFF
    TEST("PLI P3, =ADDR(func)", 0x23, 0x0F, 0x32); // addr(func) == 0x320F
}

static void test_comment() {
    TEST("NOP        ; comment", 0x00);
    TEST("XCH  A , E ; comment", 0x01);
    TEST("PUSH EA    ; comment", 0x08);
    TEST("CALL 15    ; comment", 0x1F);
    TEST("SSM  P2    ; comment", 0x2E);
    TEST("LD EA , SP ; comment", 0x31);
    TEST("ADD A , # 0x12    ; comment", 0xF4, 0x12);
    TEST("PLI P2 , # 0x1234 ; comment", 0x22, 0x34, 0x12);
    TEST("ADD A , = 0x12    ; comment", 0xF4, 0x12);
    TEST("PLI P2 , = 0x1234 ; comment", 0x22, 0x34, 0x12);
    TEST("ADD EA , 0xFF34   ; comment", 0xB5, 0x34);
    ATEST(0x1000, "BZ 0x1005; comment", 0x6C, 0x03);
    TEST("BZ 0 , P3         ; comment", 0x6F, 0x00);
    TEST("LD EA , 0 , SP    ; comment", 0x81, 0x00);
    ATEST(0x1000, "LD T , 0x1080 , PC ; comment",  0xA0, 0x7F);
    TEST("ST EA , @127 , P3; comment", 0x8F, 0x7F);
}

static void test_undefined_symbol() {
    ERUS("AND S,=UNDEF",  "UNDEF",  0x39, 0x00);
    ERUS("ADD A,=UNDEF",  "UNDEF",  0xF4, 0x00);
    ERUS("PLI P2,=UNDEF", "UNDEF", 0x22, 0x00, 0x00);
    ERUS("LD  SP,=UNDEF", "UNDEF", 0x25, 0x00, 0x00);
    ERUS("LD  EA,=UNDEF", "UNDEF", 0x84, 0x00, 0x00);
    ERUS("ST  EA,UNDEF",  "UNDEF",  0x8D, 0x00);
    ERUS("ILD A,UNDEF",   "UNDEF",   0x95, 0x00);
    ERUS("LD  T,@UNDEF,P3",  "UNDEF,P3",  0xA7, 0x00);
    ERUS("ADD EA,@UNDEF,P2", "UNDEF,P2", 0xB6, 0x00);
    ERUS("BP  UNDEF,P2", "UNDEF,P2", 0x66, 0x00);
    AERUS(0x1100, "BZ  UNDEF",       "UNDEF",    0x6C, 0x00);
    AERUS(0x1000, "LD  EA,UNDEF,PC", "UNDEF,PC", 0x80, 0x00);
    AERUS(0x1000, "JSR UNDEF",       "UNDEF",    0x20, 0x00, 0x00);
    AERUS(0x1000, "JMP UNDEF",       "UNDEF",    0x24, 0x00, 0x00);
}

static void test_error() {
    ERRT("LD A,@@1,P3", ILLEGAL_CONSTANT, "@1,P3");
    ERRT("LD A,@#1",    ILLEGAL_CONSTANT, "#1");
    ERRT("LD A,@=1",    ILLEGAL_CONSTANT, "=1");
    ERRT("LD A,1(P3)",  MISSING_COMMA,    "1(P3)"); // SC/MP style
    ERRT("LD A,@1(P3)", MISSING_COMMA,    "@1(P3)"); // SC/MP style
    ERRT("LD A,1,(EA)", GARBAGE_AT_END,   "(EA)");
}
// clang-format on

void run_tests(const char *cpu) {
    assembler.setCpu(cpu);
    RUN_TEST(test_implied);
    RUN_TEST(test_immediate);
    RUN_TEST(test_absolute);
    RUN_TEST(test_direct);
    RUN_TEST(test_relative);
    RUN_TEST(test_indexed);
    RUN_TEST(test_auto_indexed);
    RUN_TEST(test_func);
    RUN_TEST(test_comment);
    RUN_TEST(test_undefined_symbol);
    RUN_TEST(test_error);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

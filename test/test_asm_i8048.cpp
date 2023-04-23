/*
 * Copyright 2021 Tadashi G. Takaoka
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

#include "asm_i8048.h"
#include "test_asm_helper.h"

using namespace libasm;
using namespace libasm::i8048;
using namespace libasm::test;

AsmI8048 asm8048;
Assembler &assembler(asm8048);

static bool isCmos() {
    const /* PROGMEM */ auto cpu_P = assembler.cpu_P();
    return strcmp_P("80C39", cpu_P) == 0 || strcmp_P("80C48", cpu_P) == 0 ||
           strcmp_P("MSM80C39", cpu_P) == 0 || strcmp_P("MSM80C48", cpu_P) == 0;
}

static bool is8048() {
    const /* PROGMEM */ auto cpu_P = assembler.cpu_P();
    return strcmp_P("8048", cpu_P) == 0 || strcmp_P("80C48", cpu_P) == 0 ||
           strcmp_P("MSM80C48", cpu_P) == 0;
}

static bool isOki() {
    const /* PROGMEM */ auto cpu_P = assembler.cpu_P();
    return strcmp_P("MSM80C39", cpu_P) == 0 || strcmp_P("MSM80C48", cpu_P) == 0;
}

static void set_up() {
    assembler.reset();
}

static void tear_down() {
    symtab.reset();
}

// clang-format off
void test_cpu() {
    EQUALS("cpu 8039", true,   assembler.setCpu("8039"));
    EQUALS_P("cpu 8039", "8039", assembler.cpu_P());

    EQUALS("cpu I8039", true,   assembler.setCpu("I8039"));
    EQUALS_P("cpu I8039", "8039", assembler.cpu_P());

    EQUALS("cpu 8048", true,   assembler.setCpu("8048"));
    EQUALS_P("cpu 8048", "8048", assembler.cpu_P());

    EQUALS("cpu I8048", true,   assembler.setCpu("I8048"));
    EQUALS_P("cpu I8048", "8048", assembler.cpu_P());

    EQUALS("cpu 80c39", true,   assembler.setCpu("80c39"));
    EQUALS_P("cpu 80c39", "80C39", assembler.cpu_P());

    EQUALS("cpu I80c39", true,   assembler.setCpu("I80c39"));
    EQUALS_P("cpu I80c39", "80C39", assembler.cpu_P());

    EQUALS("cpu 80c48", true,   assembler.setCpu("80c48"));
    EQUALS_P("cpu 80c48", "80C48", assembler.cpu_P());

    EQUALS("cpu I80c48", true,   assembler.setCpu("I80c48"));
    EQUALS_P("cpu I80c48", "80C48", assembler.cpu_P());

    EQUALS("cpu msm80c39", true,   assembler.setCpu("msm80c39"));
    EQUALS_P("cpu msm80c39", "MSM80C39", assembler.cpu_P());

    EQUALS("cpu msm80c48", true,   assembler.setCpu("msm80c48"));
    EQUALS_P("cpu msm80c48", "MSM80C48", assembler.cpu_P());
}

static void test_accumlator() {
    TEST("add a, r0",  0x68);
    TEST("ADD A, R1",  0x69);
    TEST("ADD A, R2",  0x6A);
    TEST("ADD A, R3",  0x6B);
    TEST("ADD A, R4",  0x6C);
    TEST("ADD A, R5",  0x6D);
    TEST("ADD A, R6",  0x6E);
    TEST("ADD A, R7",  0x6F);
    ERRT("ADD A, R8",  OPERAND_NOT_ALLOWED, "A, R8");
    TEST("ADD A, @R0", 0x60);
    TEST("ADD A, @R1", 0x61);
    ERRT("ADD A, @R2", REGISTER_NOT_ALLOWED, "R2");
    TEST("ADD A, #37", 0x03, 0x25);
    TEST("ADD A, #-128", 0x03, 0x80);
    TEST("ADD A, #255",  0x03, 0xFF);
    ERRT("ADD A, #-129", OVERFLOW_RANGE, "#-129", 0x03, 0x7F);
    ERRT("ADD A, #256",  OVERFLOW_RANGE, "#256",  0x03, 0x00);

    TEST("ADDC A, R0",  0x78);
    TEST("ADDC A, R1",  0x79);
    TEST("ADDC A, R2",  0x7A);
    TEST("ADDC A, R3",  0x7B);
    TEST("ADDC A, R4",  0x7C);
    TEST("ADDC A, R5",  0x7D);
    TEST("ADDC A, R6",  0x7E);
    TEST("ADDC A, R7",  0x7F);
    TEST("ADDC A, @R0", 0x70);
    TEST("ADDC A, @R1", 0x71);
    TEST("ADDC A, #53", 0x13, 0x35);

    TEST("ANL A, R0",   0x58);
    TEST("ANL A, R1",   0x59);
    TEST("ANL A, R2",   0x5A);
    TEST("ANL A, R3",   0x5B);
    TEST("ANL A, R4",   0x5C);
    TEST("ANL A, R5",   0x5D);
    TEST("ANL A, R6",   0x5E);
    TEST("ANL A, R7",   0x5F);
    TEST("ANL A, @R0",  0x50);
    TEST("ANL A, @R1",  0x51);
    TEST("ANL A, #55H", 0x53, 0x55);

    TEST("ORL A, R0",   0x48);
    TEST("ORL A, R1",   0x49);
    TEST("ORL A, R2",   0x4A);
    TEST("ORL A, R3",   0x4B);
    TEST("ORL A, R4",   0x4C);
    TEST("ORL A, R5",   0x4D);
    TEST("ORL A, R6",   0x4E);
    TEST("ORL A, R7",   0x4F);
    TEST("ORL A, @R0",  0x40);
    TEST("ORL A, @R1",  0x41);
    TEST("ORL A, #45H", 0x43, 0x45);

    TEST("XRL A, R0",   0xD8);
    TEST("XRL A, R1",   0xD9);
    TEST("XRL A, R2",   0xDA);
    TEST("XRL A, R3",   0xDB);
    TEST("XRL A, R4",   0xDC);
    TEST("XRL A, R5",   0xDD);
    TEST("XRL A, R6",   0xDE);
    TEST("XRL A, R7",   0xDF);
    TEST("XRL A, @R0",  0xD0);
    TEST("XRL A, @R1",  0xD1);
    TEST("XRL A, #65H", 0xD3, 0x65);

    TEST("INC  A", 0x17);
    TEST("DEC  A", 0x07);
    TEST("CLR  A", 0x27);
    TEST("CPL  A", 0x37);
    TEST("DA   A", 0x57);
    TEST("SWAP A", 0x47);
    TEST("RL   A", 0xE7);
    TEST("RLC  A", 0xF7);
    TEST("RR   A", 0x77);
    TEST("RRC  A", 0x67);
}

static void test_io() {
    ERRT("IN   A, P0", OPERAND_NOT_ALLOWED, "A, P0");
    TEST("IN   A, P1", 0x09);
    TEST("IN   A, P2", 0x0A);
    ERRT("IN   A, P3", OPERAND_NOT_ALLOWED, "A, P3");
    ERRT("IN   A, P4", OPERAND_NOT_ALLOWED, "A, P4");
    ERRT("IN   A, P5", OPERAND_NOT_ALLOWED, "A, P5");
    ERRT("IN   A, P6", OPERAND_NOT_ALLOWED, "A, P6");
    ERRT("IN   A, P7", OPERAND_NOT_ALLOWED, "A, P7");
    ERRT("IN   A, P8", OPERAND_NOT_ALLOWED, "A, P8");
    if (isOki()) {
        ERRT("MOV A, P0",   OPERAND_NOT_ALLOWED, "A, P0");
        TEST("MOV A, P1",   0x63);
        TEST("MOV A, P2",   0x73);
        ERRT("MOV A, P3",   OPERAND_NOT_ALLOWED, "A, P3");
        ERRT("MOV A, P4",   OPERAND_NOT_ALLOWED, "A, P4");
        ERRT("MOV A, P5",   OPERAND_NOT_ALLOWED, "A, P5");
        ERRT("MOV A, P6",   OPERAND_NOT_ALLOWED, "A, P6");
        ERRT("MOV A, P7",   OPERAND_NOT_ALLOWED, "A, P7");
        ERRT("MOV A, P8",   OPERAND_NOT_ALLOWED, "A, P8");
        ERRT("MOV P0, @R3", OPERAND_NOT_ALLOWED, "P0, @R3");
        ERRT("MOV P1, @R0", OPERAND_NOT_ALLOWED, "P1, @R0");
        ERRT("MOV P1, @R1", OPERAND_NOT_ALLOWED, "P1, @R1");
        ERRT("MOV P1, @R2", REGISTER_NOT_ALLOWED, "R2");
        TEST("MOV P1, @R3", 0xF3);
        ERRT("MOV P2, @R3", OPERAND_NOT_ALLOWED, "P2, @R3");
    } else {
        ERRT("MOV A, P1",   OPERAND_NOT_ALLOWED, "A, P1");
        ERRT("MOV A, P2",   OPERAND_NOT_ALLOWED, "A, P2");
        ERRT("MOV P1, @R3", OPERAND_NOT_ALLOWED, "P1, @R3");
    }
    ERRT("OUTL P0, A", OPERAND_NOT_ALLOWED, "P0, A");
    TEST("OUTL P1, A", 0x39);
    TEST("OUTL P2, A", 0x3A);
    ERRT("OUTL P3, A", OPERAND_NOT_ALLOWED, "P3, A");
    ERRT("OUTL P4, A", OPERAND_NOT_ALLOWED, "P4, A");
    ERRT("OUTL P5, A", OPERAND_NOT_ALLOWED, "P5, A");
    ERRT("OUTL P6, A", OPERAND_NOT_ALLOWED, "P6, A");
    ERRT("OUTL P7, A", OPERAND_NOT_ALLOWED, "P7, A");
    ERRT("OUTL P8, A", OPERAND_NOT_ALLOWED, "P8, A");
    TEST("ANL  P1, #80H", 0x99, 0x80);
    TEST("ANL  P2, #40H", 0x9A, 0x40);
    TEST("ORL  P1, #20H", 0x89, 0x20);
    TEST("ORL  P2, #10H", 0x8A, 0x10);

    if (is8048()) {
        TEST("INS  A, BUS", 0x08);
        TEST("OUTL BUS, A", 0x02);
        TEST("ANL  BUS, #08H", 0x98, 0x08);
        TEST("ORL  BUS, #0CH", 0x88, 0x0C);
    } else {
        ERUI("INS  A, BUS");
        ERRT("OUTL BUS, A",    OPERAND_NOT_ALLOWED, "BUS, A");
        ERRT("ANL  BUS, #08H", OPERAND_NOT_ALLOWED, "BUS, #08H");
        ERRT("ORL  BUS, #0CH", OPERAND_NOT_ALLOWED, "BUS, #0CH");
    }

    ERRT("MOVD A, P0", OPERAND_NOT_ALLOWED, "A, P0");
    ERRT("MOVD A, P1", OPERAND_NOT_ALLOWED, "A, P1");
    ERRT("MOVD A, P2", OPERAND_NOT_ALLOWED, "A, P2");
    ERRT("MOVD A, P3", OPERAND_NOT_ALLOWED, "A, P3");
    TEST("MOVD A, P4", 0x0C);
    TEST("MOVD A, P5", 0x0D);
    TEST("MOVD A, P6", 0x0E);
    TEST("MOVD A, P7", 0x0F);
    TEST("MOVD P4, A", 0x3C);
    TEST("MOVD P5, A", 0x3D);
    TEST("MOVD P6, A", 0x3E);
    TEST("MOVD P7, A", 0x3F);
    TEST("ANLD P4, A", 0x9C);
    TEST("ANLD P5, A", 0x9D);
    TEST("ANLD P6, A", 0x9E);
    TEST("ANLD P7, A", 0x9F);
    TEST("ORLD P4, A", 0x8C);
    TEST("ORLD P5, A", 0x8D);
    TEST("ORLD P6, A", 0x8E);
    TEST("ORLD P7, A", 0x8F);
}

static void test_register() {
    TEST("INC R0", 0x18);
    TEST("INC R1", 0x19);
    TEST("INC R2", 0x1A);
    TEST("INC R3", 0x1B);
    TEST("INC R4", 0x1C);
    TEST("INC R5", 0x1D);
    TEST("INC R6", 0x1E);
    TEST("INC R7", 0x1F);

    TEST("INC @R0", 0x10);
    TEST("INC @R1", 0x11);

    TEST("DEC R0", 0xC8);
    TEST("DEC R1", 0xC9);
    TEST("DEC R2", 0xCA);
    TEST("DEC R3", 0xCB);
    TEST("DEC R4", 0xCC);
    TEST("DEC R5", 0xCD);
    TEST("DEC R6", 0xCE);
    TEST("DEC R7", 0xCF);
    if (isOki()) {
        TEST("DEC @R0", 0xC0);
        TEST("DEC @R1", 0xC1);
    } else {
        ERRT("DEC @R0", OPERAND_NOT_ALLOWED, "@R0");
        ERRT("DEC @R1", OPERAND_NOT_ALLOWED, "@R1");
    }
}

static void test_branch() {
    TEST("JMP  712H", 0xE4, 0x12);
    TEST("JMP  034H", 0x04, 0x34);
    TEST("JMPP @A",   0xB3);

    ATEST(0xF00, "DJNZ R0, 0F34H", 0xE8, 0x34);
    ATEST(0xE00, "DJNZ R1, 0E34H", 0xE9, 0x34);
    ATEST(0xD00, "DJNZ R2, 0D34H", 0xEA, 0x34);
    ATEST(0xC00, "DJNZ R3, 0C34H", 0xEB, 0x34);
    ATEST(0x300, "DJNZ R4, 334H",  0xEC, 0x34);
    ATEST(0x200, "DJNZ R5, 234H",  0xED, 0x34);
    ATEST(0x100, "DJNZ R6, 134H",  0xEE, 0x34);
    ATEST(0x000, "DJNZ R7, 034H",  0xEF, 0x34);
    AERRT(0x100, "DJNZ R7, 034H",  OPERAND_TOO_FAR, "034H", 0xEF, 0x34);
    if (isOki()) {
        ATEST(0xF00, "DJNZ @R0, 0F34H", 0xE0, 0x34);
        ATEST(0xE00, "DJNZ @R1, 0E34H", 0xE1, 0x34);
    } else {
        AERRT(0xF00, "DJNZ @R0, 0F34H", OPERAND_NOT_ALLOWED, "@R0, 0F34H");
        AERRT(0xE00, "DJNZ @R1, 0E34H", OPERAND_NOT_ALLOWED, "@R1, 0E34H");
    }

    ATEST(0xF00, "JC   0F34H", 0xF6, 0x34);
    ATEST(0xE00, "JNC  0E34H", 0xE6, 0x34);
    ATEST(0xD00, "JZ   0D34H", 0xC6, 0x34);
    ATEST(0xC00, "JNZ  0C34H", 0x96, 0x34);
    ATEST(0xB00, "JT0  0B34H", 0x36, 0x34);
    ATEST(0xA00, "JNT0 0A34H", 0x26, 0x34);
    ATEST(0x900, "JT1  934H",  0x56, 0x34);
    ATEST(0x800, "JNT1 834H",  0x46, 0x34);
    ATEST(0x700, "JF0  734H",  0xB6, 0x34);
    ATEST(0x600, "JF1  634H",  0x76, 0x34);
    ATEST(0x500, "JTF  534H",  0x16, 0x34);
    ATEST(0x400, "JNI  434H",  0x86, 0x34);
    AERRT(0x3FF, "JNI  334H",  OPERAND_TOO_FAR, "334H", 0x86, 0x34);
    ATEST(0x3FF, "JNI  434H",  0x86, 0x34);
    AERRT(0x500, "JNI  434H",  OPERAND_TOO_FAR, "434H", 0x86, 0x34);

    ATEST(0x300, "JB 0, 334H",  0x12, 0x34);
    ATEST(0x200, "JB 1, 234H",  0x32, 0x34);
    ATEST(0x100, "JB 2, 134H",  0x52, 0x34);
    ATEST(0x000, "JB 3, 034H",  0x72, 0x34);
    ATEST(0xF00, "JB 4, 0F34H", 0x92, 0x34);
    ATEST(0xE00, "JB 5, 0E34H", 0xB2, 0x34);
    ATEST(0xD00, "JB 6, 0D34H", 0xD2, 0x34);
    ATEST(0xC00, "JB 7, 0C34H", 0xF2, 0x34);
    AERRT(0xCFF, "JB 7, 0C34H", OPERAND_TOO_FAR, "0C34H", 0xF2, 0x34);
    ATEST(0xCFF, "JB 7, 0D34H", 0xF2, 0x34);
    AERRT(0xD00, "JB 7, 0C34H", OPERAND_TOO_FAR, "0C34H", 0xF2, 0x34);
    AERRT(0xC00, "JB -1, 0C34H", ILLEGAL_BIT_NUMBER, "-1, 0C34H", 0xF2, 0x34);
    AERRT(0xC00, "JB 8, 0C34H",  ILLEGAL_BIT_NUMBER, "8, 0C34H",  0x12, 0x34);
}

static void test_subroutine() {
    ATEST(0x700, "CALL 712H", 0xF4, 0x12);
    ATEST(0x000, "CALL 034H", 0x14, 0x34);
    TEST("RET  ", 0x83);
    TEST("RETR ", 0x93);
}

static void test_flag() {
    TEST("CLR C",  0x97);
    TEST("CLR F0", 0x85);
    TEST("CLR F1", 0xA5);

    TEST("CPL C",  0xA7);
    TEST("CPL F0", 0x95);
    TEST("CPL F1", 0xB5);
}

static void test_move() {
    TEST("MOV A, R0",  0xF8);
    TEST("MOV A, R1",  0xF9);
    TEST("MOV A, R2",  0xFA);
    TEST("MOV A, R3",  0xFB);
    TEST("MOV A, R4",  0xFC);
    TEST("MOV A, R5",  0xFD);
    TEST("MOV A, R6",  0xFE);
    TEST("MOV A, R7",  0xFF);
    TEST("MOV A, @R0", 0xF0);
    TEST("MOV A, @R1", 0xF1);
    TEST("MOV A, #37", 0x23, 0x25);

    TEST("MOV R0, A",  0xA8);
    TEST("MOV R1, A",  0xA9);
    TEST("MOV R2, A",  0xAA);
    TEST("MOV R3, A",  0xAB);
    TEST("MOV R4, A",  0xAC);
    TEST("MOV R5, A",  0xAD);
    TEST("MOV R6, A",  0xAE);
    TEST("MOV R7, A",  0xAF);
    TEST("MOV @R0, A", 0xA0);
    TEST("MOV @R1, A", 0xA1);

    TEST("MOV R0, #1",  0xB8, 0x01);
    TEST("MOV R1, #2",  0xB9, 0x02);
    TEST("MOV R2, #3",  0xBA, 0x03);
    TEST("MOV R3, #4",  0xBB, 0x04);
    TEST("MOV R4, #5",  0xBC, 0x05);
    TEST("MOV R5, #6",  0xBD, 0x06);
    TEST("MOV R6, #7",  0xBE, 0x07);
    TEST("MOV R7, #8",  0xBF, 0x08);
    TEST("MOV @R0, #9", 0xB0, 0x09);
    TEST("MOV @R1, #0", 0xB1, 0x00);

    TEST("MOV A, PSW", 0xC7);
    TEST("MOV PSW, A", 0xD7);

    TEST("XCH A, R0",  0x28);
    TEST("XCH A, R1",  0x29);
    TEST("XCH A, R2",  0x2A);
    TEST("XCH A, R3",  0x2B);
    TEST("XCH A, R4",  0x2C);
    TEST("XCH A, R5",  0x2D);
    TEST("XCH A, R6",  0x2E);
    TEST("XCH A, R7",  0x2F);
    TEST("XCH A, @R0", 0x20);
    TEST("XCH A, @R1", 0x21);

    TEST("XCHD A, @R0", 0x30);
    TEST("XCHD A, @R1", 0x31);

    TEST("MOVX @R0, A", 0x90);
    TEST("MOVX @R1, A", 0x91);
    TEST("MOVX A, @R0", 0x80);
    TEST("MOVX A, @R1", 0x81);

    TEST("MOVP  A, @A", 0xA3);
    TEST("MOVP3 A, @A", 0xE3);
    if (isOki()) {
        TEST("MOVP1 P, @R3", 0xC3);
    } else {
        ERUI("MOVP1 P, @R3");
    }
}

static void test_timer_counter() {
    TEST("MOV A, T", 0x42);
    TEST("MOV T, A", 0x62);

    TEST("STRT T",    0x55);
    TEST("STRT CNT",  0x45);
    TEST("STOP TCNT", 0x65);

    TEST("EN  TCNTI", 0x25);
    TEST("DIS TCNTI", 0x35);
}

static void test_control() {
    TEST("EN  I", 0x05);
    TEST("DIS I", 0x15);

    TEST("SEL RB0", 0xC5);
    TEST("SEL RB1", 0xD5);
    TEST("SEL MB0", 0xE5);
    TEST("SEL MB1", 0xF5);

    TEST("ENT0 CLK", 0x75);

    TEST("NOP  ", 0x00);
    if (isCmos()) {
        TEST("HALT ", 0x01);
        if (isOki()) {
            TEST("HLTS ", 0x82);
            TEST("FLT  ", 0xA2);
            TEST("FLTT ", 0xC2);
            TEST("FRES ", 0xE2);
        } else {
            ERUI("HLTS ");
            ERUI("FLT  ");
            ERUI("FLTT ");
            ERUI("FRES ");
        }
    } else {
        ERUI("HALT");
    }
}

static void test_data_constant() {
    TEST("DB -128, 255", 0x80, 0xFF);
    TEST("DB 'A', '\"'", 0x41, 0x22);
    TEST("DB '9'-'0'",   0x09);
    TEST("DB ''''",      0x27);
    ERRT("DB '''",       MISSING_CLOSING_QUOTE, "'''");
    TEST("DB 'A''B',0",  0x41, 0x27, 0x42, 0x00);
    ERRT("DB 'A''B,0",   MISSING_CLOSING_QUOTE, "'A''B,0");
    TEST("DW -128, 255", 0xFF, 0x80, 0x00, 0xFF);
    TEST("DW 'A''B'",    0x41, 0x27, 0x42, 0x00);
    ERRT("DW 'A''B",     MISSING_CLOSING_QUOTE, "'A''B");
}

// clang-format on

void run_tests(const char *cpu) {
    assembler.setCpu(cpu);
    RUN_TEST(test_accumlator);
    RUN_TEST(test_io);
    RUN_TEST(test_register);
    RUN_TEST(test_branch);
    RUN_TEST(test_subroutine);
    RUN_TEST(test_flag);
    RUN_TEST(test_move);
    RUN_TEST(test_timer_counter);
    RUN_TEST(test_control);
    RUN_TEST(test_data_constant);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

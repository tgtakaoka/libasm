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

#include "asm_z8.h"
#include "test_asm_helper.h"

using namespace libasm;
using namespace libasm::z8;
using namespace libasm::test;

AsmZ8 asmz8;
Assembler &assembler(asmz8);

static bool z86c() {
    return strcmp_P("Z86C", assembler.config().cpu_P()) == 0;
}

static bool z86() {
    return strcmp_P("Z8", assembler.config().cpu_P()) == 0 || z86c();
}

static bool z88() {
    return strcmp_P("Z88", assembler.config().cpu_P()) == 0;
}

#define TZ86(src, ...) \
    if (z86())         \
    TEST(src, __VA_ARGS__)
#define TZ88(src, ...) \
    if (z88())         \
    TEST(src, __VA_ARGS__)
#define EZ86(src, error, at, ...) \
    if (z86())                    \
    ERRT(src, error, at, __VA_ARGS__)
#define EZ88(src, error, at, ...) \
    if (z88())                    \
    ERRT(src, error, at, __VA_ARGS__)

static uint8_t R(uint8_t n) {
    if (z88())
        return 0xC0 + n;
    return 0xE0 + n;
}

static void set_up() {
    assembler.reset();
}

static void tear_down() {
    symtab.reset();
}

// clang-format off
 void test_cpu() {
    EQUALS("cpu z8", true, assembler.setCpu("z8"));
    EQUALS_P("cpu z8", "Z8", assembler.config().cpu_P());

    EQUALS("cpu Z8601", true, assembler.setCpu("Z8601"));
    EQUALS_P("cpu Z8601", "Z8", assembler.config().cpu_P());

    EQUALS("cpu Z86C91", true,   assembler.setCpu("Z86C91"));
    EQUALS_P("cpu Z86C91", "Z86C", assembler.config().cpu_P());

    EQUALS("cpu Super8", true,  assembler.setCpu("Super8"));
    EQUALS_P("cpu Super8", "Z88", assembler.config().cpu_P());

    EQUALS("cpu Z88C00", true,  assembler.setCpu("Z88C00"));
    EQUALS_P("cpu Z88C00", "Z88", assembler.config().cpu_P());
}

static void test_implied() {
    TZ88("NEXT",  0x0F);
    TZ88("ENTER", 0x1F);
    TZ88("EXIT",  0x2F);
    TZ88("WFI",   0x3F);
    TZ88("SB0",   0x4F);
    TZ88("SB1",   0x5F);

    EZ86("NEXT",  UNKNOWN_INSTRUCTION, "NEXT");
    EZ86("ENTER", UNKNOWN_INSTRUCTION, "ENTER");
    EZ86("EXIT",  UNKNOWN_INSTRUCTION, "EXIT");
    EZ86("WFI",   UNKNOWN_INSTRUCTION, "WFI");
    EZ86("SB0",   UNKNOWN_INSTRUCTION, "SB0");
    EZ86("SB1",   UNKNOWN_INSTRUCTION, "SB1");

    if (z86c())  {
        TEST("STOP", 0x6F);
        TEST("HALT", 0x7F);
    } else {
        ERUI("STOP");
        ERUI("HALT");
    }

    TEST("DI",   0x8F);
    TEST("EI",   0x9F);
    TEST("RET",  0xAF);
    TEST("IRET", 0xBF);
    TEST("RCF",  0xCF);
    TEST("SCF",  0xDF);
    TEST("CCF",  0xEF);
    TEST("NOP",  0xFF);
}

static void test_absolute() {
    symtab.intern(0x7E7F, "$s.7E7F");
    symtab.intern(0x8e8F, "?s_8E8F");
    symtab.intern(0xDEDF, ".s?DEDF");
    symtab.intern(0xD7D8, "_s$D7D8");

    TEST("JP F,   %0E0F", 0x0D, 0x0E, 0x0F);
    TEST("JP LT,  %1E1F", 0x1D, 0x1E, 0x1F);
    TEST("JP LE,  %2E2F", 0x2D, 0x2E, 0x2F);
    TEST("jp ule, 3e3fh", 0x3D, 0x3E, 0x3F);
    TEST("JP OV,  %4E4F", 0x4D, 0x4E, 0x4F);
    TEST("JP MI,  %5E5F", 0x5D, 0x5E, 0x5F);
    TEST("JP Z,   %6E6F", 0x6D, 0x6E, 0x6F);
    TEST("JP C, $s.7E7F", 0x7D, 0x7E, 0x7F);
    TEST("JP    ?s_8E8F", 0x8D, 0x8E, 0x8F);
    TEST("JP GE,  %9E9F", 0x9D, 0x9E, 0x9F);
    TEST("JP GT,  %AEAF", 0xAD, 0xAE, 0xAF);
    TEST("JP UGT, %BEBF", 0xBD, 0xBE, 0xBF);
    TEST("JP NOV, %CECF", 0xCD, 0xCE, 0xCF);
    TEST("JP PL,.s?DEDF", 0xDD, 0xDE, 0xDF);
    TEST("JP NZ,  %EEEF", 0xED, 0xEE, 0xEF);
    TEST("JP NC,  %FEFF", 0xFD, 0xFE, 0xFF);
    TZ86("CALL  _s$D7D8", 0xD6, 0xD7, 0xD8);
}

static void test_relative() {
    symtab.intern(0x106E, "?s$106E");
    symtab.intern(0x0F8E, ".s_0F8E");
    symtab.intern(0x1040, "rx40");
    symtab.intern(0x1040, "rr40");

    ATEST(0x1000, "JR F,   %100E", 0x0B, 0x0C);
    ATEST(0x1000, "JR LT,  %101E", 0x1B, 0x1C);
    ATEST(0x1000, "JR LE,  %102E", 0x2B, 0x2C);
    ATEST(0x1000, "JR ULE, %103E", 0x3B, 0x3C);
    ATEST(0x1000, "JR OV,  %104E", 0x4B, 0x4C);
    ATEST(0x1000, "JR MI,  %105E", 0x5B, 0x5C);
    ATEST(0x1000, "JR Z, ?s$106E", 0x6B, 0x6C);
    ATEST(0x1000, "JR EQ,  %106E", 0x6B, 0x6C);
    ATEST(0x1000, "JR C,   %107E", 0x7B, 0x7C);
    ATEST(0x1000, "JR ULT, %107E", 0x7B, 0x7C);
    ATEST(0x1000, "JR    .s_0F8E", 0x8B, 0x8C);
    ATEST(0x1000, "JR GE,  %0F9E", 0x9B, 0x9C);
    ATEST(0x1000, "JR GT,  %0FAE", 0xAB, 0xAC);
    ATEST(0x1000, "JR UGT, %0FBE", 0xBB, 0xBC);
    ATEST(0x1000, "JR NOV, %0FCE", 0xCB, 0xCC);
    ATEST(0x1000, "JR PL,  %0FDE", 0xDB, 0xDC);
    ATEST(0x1000, "JR NZ,  %0FEE", 0xEB, 0xEC);
    ATEST(0x1000, "JR NE,  %0FEE", 0xEB, 0xEC);
    ATEST(0x1000, "JR NC,  %0FFE", 0xFB, 0xFC);
    ATEST(0x1000, "JR UGE, %0FFE", 0xFB, 0xFC);

    ATEST(0x1000, "JR rx40", 0x8B, 0x3E);
    ATEST(0x1000, "JR rr40", 0x8B, 0x3E);
    ATEST(0x1000, "JR $+80H", 0x8B, 0x7E);

    ATEST(0x1000, "djnz  r0,100dh", 0x0A, 0x0B);
    ATEST(0x1000, "DJNZ  R1,%101D", 0x1A, 0x1B);
    ATEST(0x1000, "DJNZ  R2,%102D", 0x2A, 0x2B);
    ATEST(0x1000, "DJNZ  R3,%103D", 0x3A, 0x3B);
    ATEST(0x1000, "DJNZ  R4,%104D", 0x4A, 0x4B);
    ATEST(0x1000, "DJNZ  R5,%105D", 0x5A, 0x5B);
    ATEST(0x1000, "DJNZ  R6,%106D", 0x6A, 0x6B);
    ATEST(0x1000, "DJNZ  R7,%107D", 0x7A, 0x7B);
    ATEST(0x1000, "DJNZ  R8,%0F8D", 0x8A, 0x8B);
    ATEST(0x1000, "DJNZ  R9,%0F9D", 0x9A, 0x9B);
    ATEST(0x1000, "DJNZ R10,%0FAD", 0xAA, 0xAB);
    ATEST(0x1000, "DJNZ R11,%0FBD", 0xBA, 0xBB);
    ATEST(0x1000, "DJNZ R12,%0FCD", 0xCA, 0xCB);
    ATEST(0x1000, "DJNZ R13,%0FDD", 0xDA, 0xDB);
    ATEST(0x1000, "DJNZ R14,%0FED", 0xEA, 0xEB);
    ATEST(0x1000, "DJNZ R15,%0FFD", 0xFA, 0xFB);

    TZ88("CPIJE  R3,@R12,$", 0xC2, 0xC3, 0xFD);
    TZ88("CPIJNE R3,@R13,$", 0xD2, 0xD3, 0xFD);
    EZ86("CPIJE  R3,@R12,$", UNKNOWN_INSTRUCTION, "CPIJE  R3,@R12,$");
    EZ86("CPIJNE R3,@R13,$", UNKNOWN_INSTRUCTION, "CPIJNE R3,@R13,$");
}

static void test_operand_in_opcode() {
    TEST("LD  R0,>%09", 0x08, 0x09);
    TEST("LD  R0,R9",   0x08, R(9));
    TEST("LD  R1,>%0F", 0x18, 0x0F);
    TEST("LD  R2,R0",   0x28, R(0));
    TEST("LD  R3,%10",  0x38, 0x10);
    TEST("LD  R4,%49",  0x48, 0x49);
    TEST("LD  R5,%59",  0x58, 0x59);
    TEST("LD  R6,%69",  0x68, 0x69);
    TEST("LD  R7,%79",  0x78, 0x79);
    TEST("LD  R8,%89",  0x88, 0x89);
    TEST("LD  R9,%99",  0x98, 0x99);
    TEST("LD R10,%0A9", 0xA8, 0xA9);
    TEST("LD R11,%0B9", 0xB8, 0xB9);
    TEST("LD R12,%0C9", 0xC8, 0xC9);
    TEST("LD R13,%0D9", 0xD8, 0xD9);
    TEST("LD R14,%0E9", 0xE8, 0xE9);
    TEST("LD R15,%0F9", 0xF8, 0xF9);

    TEST("LD >%0A, R0", 0x09, 0x0A);
    TEST("LD >%0F, R1", 0x19, 0x0F);
    TEST("LD >%00, R2", 0x29, 0x00);
    TEST("LD  %10, R3", 0x39, 0x10);
    TEST("LD  %4A, R4", 0x49, 0x4A);
    TEST("LD  %5A, R5", 0x59, 0x5A);
    TEST("LD  %6A, R6", 0x69, 0x6A);
    TEST("LD  %7A, R7", 0x79, 0x7A);
    TEST("LD %FF, R8",  0x89, 0xFF);
    TEST("LD  %9A, R9", 0x99, 0x9A);
    TEST("LD %AA, R10", 0xA9, 0xAA);
    TEST("LD %BA, R11", 0xB9, 0xBA);
    TEST("LD %CA, R12", 0xC9, 0xCA);
    TEST("LD %DA, R13", 0xD9, 0xDA);
    TEST("LD %EA, R14", 0xE9, 0xEA);
    TEST("LD %FA, R15", 0xF9, 0xFA);

    TEST("LD  R0,#13",   0x0C, 0x0D);
    TEST("LD R1,#'%%'",  0x1C, 0x25);
    TEST("LD R1,#'%q'",  0x1C, 0x27);
    TEST("LD R1,#'%l'",  0x1C, 0x0A);
    TEST("LD R1,#'%r'",  0x1C, 0x0D);
    TEST("LD R1,#'%t'",  0x1C, 0x09);
    TEST("LD R1,#'%p'",  0x1C, 0x0C);
    TEST("LD R1,#'%1B'", 0x1C, 0x1B);
    TEST("LD  R2,#15",   0x2C, 0x0F);
    TEST("LD  R3,#%10",  0x3C, 0x10);
    TEST("LD  R4,#%4D",  0x4C, 0x4D);
    TEST("LD  R5,#%5D",  0x5C, 0x5D);
    TEST("LD  R6,#%6D",  0x6C, 0x6D);
    TEST("LD  R7,#%7D",  0x7C, 0x7D);
    TEST("LD  R8,#%8D",  0x8C, 0x8D);
    TEST("LD  R9,#%9D",  0x9C, 0x9D);
    TEST("LD R10,#%AD",  0xAC, 0xAD);
    TEST("LD R11,#%BD",  0xBC, 0xBD);
    TEST("LD R12,#%CD",  0xCC, 0xCD);
    TEST("LD R13,#%DD",  0xDC, 0xDD);
    TEST("LD R14,#%ED",  0xEC, 0xED);
    TEST("LD R15,#%FD",  0xFC, 0xFD);
    TEST("LD R15,#-128", 0xFC, 0x80);
    TEST("LD R15,#-1",   0xFC, 0xFF);
    TEST("LD R15,#255",  0xFC, 0xFF);
    ERRT("LD R15,#-129", OVERFLOW_RANGE, "#-129", 0xFC, 0x7F);
    ERRT("LD R15,#256",  OVERFLOW_RANGE, "#256",  0xFC, 0x00);

    TEST("INC  R0", 0x0E);
    TEST("INC  R1", 0x1E);
    TEST("INC  R2", 0x2E);
    TEST("INC  R3", 0x3E);
    TEST("INC  R4", 0x4E);
    TEST("INC  R5", 0x5E);
    TEST("INC  R6", 0x6E);
    TEST("INC  R7", 0x7E);
    TEST("INC  R8", 0x8E);
    TEST("INC  R9", 0x9E);
    TEST("INC R10", 0xAE);
    TEST("INC R11", 0xBE);
    TEST("INC R12", 0xCE);
    TEST("INC R13", 0xDE);
    TEST("INC R14", 0xEE);
    TEST("INC R15", 0xFE);
}

static void test_one_operand() {
    TEST("DEC >%01", 0x00, 0x01);
    ERRT("DEC -1",   OPERAND_NOT_ALLOWED, "-1");
    ERRT("DEC 256",  OPERAND_NOT_ALLOWED, "256");
    TEST("DEC R1",   0x00, R(1));
    TEST("DEC @%15", 0x01, 0x15);
    TEST("DEC @R2",  0x01, R(2));

    TEST("RLC %11",  0x10, 0x11);
    TEST("RLC R1",   0x10, R(1));
    TEST("RLC @%12", 0x11, 0x12);
    TEST("RLC @R2",  0x11, R(2));

    TEST("INC %21",  0x20, 0x21);
    TEST("INC R1",   0x1E);
    TEST("INC @%22", 0x21, 0x22);
    TEST("INC @R2",  0x21, R(2));

    TEST("DA %41",  0x40, 0x41);
    TEST("DA R1",   0x40, R(1));
    TEST("DA @%42", 0x41, 0x42);
    TEST("DA @R2",  0x41, R(2));

    TEST("POP %FC",  0x50, 0xFC);
    TEST("POP R1",   0x50, R(1));
    TEST("POP @%52", 0x51, 0x52);
    TEST("POP @R2",  0x51, R(2));

    TEST("COM %61",  0x60, 0x61);
    TEST("COM R1",   0x60, R(1));
    TEST("COM @%62", 0x61, 0x62);
    TEST("COM @R2",  0x61, R(2));

    TEST("PUSH %FF",  0x70, 0xFF);
    TEST("PUSH R1",   0x70, R(1));
    TEST("PUSH @%72", 0x71, 0x72);
    TEST("PUSH @R2",  0x71, R(2));

    TEST("DECW %82",  0x80, 0x82);
    TEST("decw rr2",  0x80, R(2));
    ERRT("DECW %81",  OPERAND_NOT_ALLOWED, "%81");
    ERRT("DECW RR1",  ILLEGAL_REGISTER, "RR1");
    TEST("DECW @%81", 0x81, 0x81);
    TEST("DECW @R1",  0x81, R(1));

    TEST("RL %91",  0x90, 0x91);
    TEST("RL R1",   0x90, R(1));
    TEST("RL @%92", 0x91, 0x92);
    TEST("RL @R2",  0x91, R(2));

    TEST("INCW %A2",  0xA0, 0xA2);
    TEST("INCW RR2",  0xA0, R(2));
    ERRT("INCW %A1",  OPERAND_NOT_ALLOWED, "%A1");
    ERRT("INCW RR1",  ILLEGAL_REGISTER, "RR1");
    TEST("INCW @%A1", 0xA1, 0xA1);
    TEST("INCW @R1",  0xA1, R(1));

    TEST("CLR %B1",  0xB0, 0xB1);
    TEST("CLR R1",   0xB0, R(1));
    TEST("CLR @%B2", 0xB1, 0xB2);
    TEST("CLR @R2",  0xB1, R(2));

    TEST("RRC %C1",  0xC0, 0xC1);
    TEST("RRC R1",   0xC0, R(1));
    TEST("RRC @%C2", 0xC1, 0xC2);
    TEST("RRC @R2",  0xC1, R(2));

    TEST("SRA %D1",  0xD0, 0xD1);
    TEST("SRA R1",   0xD0, R(1));
    TEST("SRA @%D2", 0xD1, 0xD2);
    TEST("SRA @R2",  0xD1, R(2));

    TEST("RR %C1",  0xE0, 0xC1);
    TEST("RR R1",   0xE0, R(1));
    TEST("RR @%C2", 0xE1, 0xC2);
    TEST("RR @R2",  0xE1, R(2));

    TEST("SWAP %CF",  0xF0, 0xCF);
    TEST("SWAP R1",   0xF0, R(1));
    TEST("SWAP @%CF", 0xF1, 0xCF);
    TEST("SWAP @R2",  0xF1, R(2));

    TEST("JP @%32",   0x30, 0x32);
    TEST("JP @RR2",   0x30, R(2));
    TZ86("CALL @%D6", 0xD4, 0xD6);
    TZ88("CALL @%D6", 0xF4, 0xD6);
    TZ86("CALL @RR2", 0xD4, R(2));
    TZ88("CALL @RR2", 0xF4, R(2));
    EZ86("CALL #%D6", OPERAND_NOT_ALLOWED, "#%D6");
    TZ88("CALL #%D6", 0xD4, 0xD6);
    EZ88("CALL #%D7", OPERAND_NOT_ALIGNED, "#%D7", 0xD4, 0xD7);
    TEST("SRP  #%30", 0x31, 0x30);
    ERRT("SRP  #%38", OPERAND_NOT_ALLOWED, "#%38", 0x31, 0x38);
    EZ86("SRP0 #%30", UNKNOWN_INSTRUCTION, "SRP0 #%30");
    TZ88("SRP0 #%30", 0x31, 0x32);
    EZ86("SRP1 #%38", UNKNOWN_INSTRUCTION, "SRP1 #%38");
    TZ88("SRP1 #%38", 0x31, 0x39);
    if (z86()) {
        ERRT("SRP #%100", OVERFLOW_RANGE, "#%100", 0x31, 0x00);
    } else {
        ERRT("SRP #%100", OVERFLOW_RANGE, "#%100", 0x31, 0x00);
    }
}

static void test_two_operands() {
    TEST("ADD R0,R3",     0x02, 0x03);
    TEST("ADD R4,@R5",    0x03, 0x45);
    TEST("ADD %78,%56",   0x04, 0x56, 0x78);
    TEST("ADD R8,%56",    0x04, 0x56, R(8));
    TEST("ADD %78,R6",    0x04, R(6), 0x78);
    TEST("ADD >%07,@%16", 0x05, 0x16, 0x07);
    TEST("ADD >%07,@R6",  0x05, R(6), 0x07);
    TEST("ADD R7,@%16",   0x05, 0x16, R(7));
    TEST("ADD >%07,#8",   0x06, 0x07, 0x08);
    TEST("ADD R7,#8",     0x06, R(7), 0x08);
    TZ86("ADD @%18,#9",   0x07, 0x18, 0x09);
    EZ88("ADD @%18,#9",   OPERAND_NOT_ALLOWED, "@%18,#9");
    TZ86("ADD @R8,#9",    0x07, R(8), 0x09);
    EZ88("ADD @R8,#9",    OPERAND_NOT_ALLOWED, "@R8,#9");
    TEST("ADD R7,#-128",  0x06, R(7), 0x80);
    TEST("ADD R7,#-1",    0x06, R(7), 0xFF);
    TEST("ADD R7,#%FF",   0x06, R(7), 0xFF);
    ERRT("ADD R7,#256",   OVERFLOW_RANGE, "#256",  0x06, R(7), 0x00);
    ERRT("ADD R7,#-129",  OVERFLOW_RANGE, "#-129", 0x06, R(7), 0x7F);

    TEST("ADC R3,R4",     0x12, 0x34);
    TEST("ADC R1,@R4",    0x13, 0x14);
    TEST("ADC %16,%15",   0x14, 0x15, 0x16);
    TEST("ADC R6,%15",    0x14, 0x15, R(6));
    TEST("ADC %16,R5",    0x14, R(5), 0x16);
    TEST("ADC %17,@%16",  0x15, 0x16, 0x17);
    TEST("ADC %17,@R6",   0x15, R(6), 0x17);
    TEST("ADC R7,@%16",   0x15, 0x16, R(7));
    TEST("ADC %17,#%18",  0x16, 0x17, 0x18);
    TEST("ADC R7,#%18",   0x16, R(7), 0x18);
    TZ86("ADC @%18,#%19", 0x17, 0x18, 0x19);
    EZ88("ADC @%18,#%19", OPERAND_NOT_ALLOWED, "@%18,#%19");
    TZ86("ADC @R8,#%19",  0x17, R(8), 0x19);
    EZ88("ADC @R8,#%19",  OPERAND_NOT_ALLOWED, "@R8,#%19");

    TEST("SUB R2,R3",     0x22, 0x23);
    TEST("SUB R4,@R5",    0x23, 0x45);
    TEST("SUB %26,%25",   0x24, 0x25, 0x26);
    TEST("SUB R6,%25",    0x24, 0x25, R(6));
    TEST("SUB %26,R5",    0x24, R(5), 0x26);
    TEST("SUB %27,@%26",  0x25, 0x26, 0x27);
    TEST("SUB %27,@R6",   0x25, R(6), 0x27);
    TEST("SUB R7,@%26",   0x25, 0x26, R(7));
    TEST("SUB %27,#%28",  0x26, 0x27, 0x28);
    TEST("SUB R7,#%28",   0x26, R(7), 0x28);
    TZ86("SUB @%28,#%29", 0x27, 0x28, 0x29);
    EZ88("SUB @%28,#%29", OPERAND_NOT_ALLOWED, "@%28,#%29");
    TZ86("SUB @R8,#%29",  0x27, R(8), 0x29);
    EZ88("SUB @R8,#%29",  OPERAND_NOT_ALLOWED, "@R8,#%29");

    TEST("SBC R3,R3",     0x32, 0x33);
    TEST("SBC R3,@R4",    0x33, 0x34);
    TEST("SBC %78,%56",   0x34, 0x56, 0x78);
    TEST("SBC R8,%56",    0x34, 0x56, R(8));
    TEST("SBC %78,R6",    0x34, R(6), 0x78);
    TEST("SBC %37,@%36",  0x35, 0x36, 0x37);
    TEST("SBC %37,@R6",   0x35, R(6), 0x37);
    TEST("SBC R7,@%36",   0x35, 0x36, R(7));
    TEST("SBC %37,#%38",  0x36, 0x37, 0x38);
    TEST("SBC R7,#%38",   0x36, R(7), 0x38);
    TZ86("SBC @%38,#%39", 0x37, 0x38, 0x39);
    EZ88("SBC @%38,#%39", OPERAND_NOT_ALLOWED, "@%38,#%39");
    TZ86("SBC @R8,#%39",  0x37, R(8), 0x39);
    EZ88("SBC @R8,#%39",  OPERAND_NOT_ALLOWED, "@R8,#%39");

    TEST("OR R4,R3",     0x42, 0x43);
    TEST("OR R4,@R4",    0x43, 0x44);
    TEST("OR %46,%45",   0x44, 0x45, 0x46);
    TEST("OR R6,%45",    0x44, 0x45, R(6));
    TEST("OR %46,R5",    0x44, R(5), 0x46);
    TEST("OR %89,@%67",  0x45, 0x67, 0x89);
    TEST("OR %89,@R7",   0x45, R(7), 0x89);
    TEST("OR R9,@%67",   0x45, 0x67, R(9));
    TEST("OR %47,#%48",  0x46, 0x47, 0x48);
    TEST("OR R7,#%48",   0x46, R(7), 0x48);
    TZ86("OR @%48,#%49", 0x47, 0x48, 0x49);
    EZ88("OR @%48,#%49", OPERAND_NOT_ALLOWED, "@%48,#%49");
    TZ86("OR @R8,#%49",  0x47, R(8), 0x49);
    EZ88("OR @R8,#%49",  OPERAND_NOT_ALLOWED, "@R8,#%49");

    TEST("AND R5,R3",     0x52, 0x53);
    TEST("AND R5,@R4",    0x53, 0x54);
    TEST("AND %56,%55",   0x54, 0x55, 0x56);
    TEST("AND R6,%55",    0x54, 0x55, R(6));
    TEST("AND %56,R5",    0x54, R(5), 0x56);
    TEST("AND %57,@%56",  0x55, 0x56, 0x57);
    TEST("AND %57,@R6",   0x55, R(6), 0x57);
    TEST("AND R7,@%56",   0x55, 0x56, R(7));
    TEST("AND %78,#%9A",  0x56, 0x78, 0x9A);
    TEST("AND R8,#%9A",   0x56, R(8), 0x9A);
    TZ86("AND @%58,#%59", 0x57, 0x58, 0x59);
    EZ88("AND @%58,#%59", OPERAND_NOT_ALLOWED, "@%58,#%59");
    TZ86("AND @R8,#%59",  0x57, R(8), 0x59);
    EZ88("AND @R8,#%59",  OPERAND_NOT_ALLOWED, "@R8,#%59");

    TEST("TCM R6,R3",     0x62, 0x63);
    TEST("TCM R6,@R4",    0x63, 0x64);
    TEST("TCM %66,%65",   0x64, 0x65, 0x66);
    TEST("TCM R6,%65",    0x64, 0x65, R(6));
    TEST("TCM %66,R5",    0x64, R(5), 0x66);
    TEST("TCM %67,@%66",  0x65, 0x66, 0x67);
    TEST("TCM %67,@R6",   0x65, R(6), 0x67);
    TEST("TCM R7,@%66",   0x65, 0x66, R(7));
    TEST("TCM %67,#%68",  0x66, 0x67, 0x68);
    TEST("TCM R7,#%68",   0x66, R(7), 0x68);
    TZ86("TCM @%89,#%AB", 0x67, 0x89, 0xAB);
    EZ88("TCM @%89,#%AB", OPERAND_NOT_ALLOWED, "@%89,#%AB");
    TZ86("TCM @R9,#%AB",  0x67, R(9), 0xAB);
    EZ88("TCM @R9,#%AB",  OPERAND_NOT_ALLOWED, "@R9,#%AB");

    TEST("TM R7,R3",     0x72, 0x73);
    TEST("TM R7,@R4",    0x73, 0x74);
    TEST("TM %76,%75",   0x74, 0x75, 0x76);
    TEST("TM R6,%75",    0x74, 0x75, R(6));
    TEST("TM %76,R5",    0x74, R(5), 0x76);
    TEST("TM %77,@%76",  0x75, 0x76, 0x77);
    TEST("TM %77,@R6",   0x75, R(6), 0x77);
    TEST("TM R7,@%76",   0x75, 0x76, R(7));
    TEST("TM %77,#%78",  0x76, 0x77, 0x78);
    TEST("TM R7,#%78",   0x76, R(7), 0x78);
    TZ86("TM @%78,#%79", 0x77, 0x78, 0x79);
    EZ88("TM @%78,#%79", OPERAND_NOT_ALLOWED, "@%78,#%79");
    TZ86("TM @R8,#%79",  0x77, R(8), 0x79);
    EZ88("TM @R8,#%79",  OPERAND_NOT_ALLOWED, "@R8,#%79");

    TEST("CP R10,R3",    0xA2, 0xA3);
    TEST("CP R10,@R4",   0xA3, 0xA4);
    TEST("CP %A6,%A5",   0xA4, 0xA5, 0xA6);
    TEST("CP R6,%A5",    0xA4, 0xA5, R(6));
    TEST("CP %A6,R5",    0xA4, R(5), 0xA6);
    TEST("CP %A7,@%A6",  0xA5, 0xA6, 0xA7);
    TEST("CP %A7,@R6",   0xA5, R(6), 0xA7);
    TEST("CP R7,@%A6",   0xA5, 0xA6, R(7));
    TEST("CP %A7,#%A8",  0xA6, 0xA7, 0xA8);
    TEST("CP R7,#%A8",   0xA6, R(7), 0xA8);
    TZ86("CP @%A8,#%A9", 0xA7, 0xA8, 0xA9);
    EZ88("CP @%A8,#%A9", OPERAND_NOT_ALLOWED, "@%A8,#%A9");
    TZ86("CP @R8,#%A9",  0xA7, R(8), 0xA9);
    EZ88("CP @R8,#%A9",  OPERAND_NOT_ALLOWED, "@R8,#%A9");

    TEST("XOR R11,R3",    0xB2, 0xB3);
    TEST("XOR R11,@R4",   0xB3, 0xB4);
    TEST("XOR %B6,%B5",   0xB4, 0xB5, 0xB6);
    TEST("XOR R6,%B5",    0xB4, 0xB5, R(6));
    TEST("XOR %B6,R5",    0xB4, R(5), 0xB6);
    TEST("XOR %B7,@%B6",  0xB5, 0xB6, 0xB7);
    TEST("XOR %B7,@R6",   0xB5, R(6), 0xB7);
    TEST("XOR R7,@%B6",   0xB5, 0xB6, R(7));
    TEST("XOR %B7,#%B8",  0xB6, 0xB7, 0xB8);
    TEST("XOR R7,#%B8",   0xB6, R(7), 0xB8);
    TZ86("XOR @%B8,#%B9", 0xB7, 0xB8, 0xB9);
    EZ88("XOR @%B8,#%B9", OPERAND_NOT_ALLOWED, "@%B8,#%B9");
    TZ86("XOR @R8,#%B9",  0xB7, R(8), 0xB9);
    EZ88("XOR @R8,#%B9",  OPERAND_NOT_ALLOWED, "@R8,#%B9");

    TZ88("DIV  %96,%95",  0x94, 0x95, 0x96);
    TZ88("DIV  RR6,R5",   0x94, R(5), R(6));
    TZ88("DIV  %98,@%96", 0x95, 0x96, 0x98);
    TZ88("DIV  RR8,@R6",  0x95, R(6), R(8));
    TZ88("DIV  %98,#%97", 0x96, 0x97, 0x98);
    TZ88("DIV  RR8,#%97", 0x96, 0x97, R(8));
    TZ88("MULT %86,%85",  0x84, 0x85, 0x86);
    TZ88("MULT RR6,R5",   0x84, R(5), R(6));
    TZ88("MULT %88,@%86", 0x85, 0x86, 0x88);
    TZ88("MULT RR8,@R6",  0x85, R(6), R(8));
    TZ88("MULT %88,#%87", 0x86, 0x87, 0x88);
    TZ88("MULT RR8,#%87", 0x86, 0x87, R(8));
    EZ86("DIV  %96,%95",  UNKNOWN_INSTRUCTION, "DIV  %96,%95");
    EZ86("MULT %96,%95",  UNKNOWN_INSTRUCTION, "MULT %96,%95");

    TZ86("LD R14,@R4",   0xE3, 0xE4);
    TZ88("LD R14,@R4",   0xC7, 0xE4);
    TEST("LD %36,%35",   0xE4, 0x35, 0x36);
    TEST("LD %37,@%36",  0xE5, 0x36, 0x37);
    TEST("LD %37,#%38",  0xE6, 0x37, 0x38);
    TZ86("LD @%38,#%39", 0xE7, 0x38, 0x39);
    TZ88("LD @%38,#%39", 0xD6, 0x38, 0x39);
    TEST("LD R6,R5",     0x68, R(5));
    TZ86("LD R7,@R6",    0xE3, 0x76);
    TZ88("LD R7,@R6",    0xC7, 0x76);
    TEST("LD R7,#%E8",   0x7C, 0xE8);
    TZ86("LD @R8,#%E9",  0xE7, R(8), 0xE9);
    TZ88("LD @R8,#%E9",  0xD6, R(8), 0xE9);
    TZ86("LD @R15,R4",   0xF3, 0xF4);
    TZ88("LD @R15,R4",   0xD7, 0xF4);
    TEST("LD @%C7,%F0",  0xF5, 0xF0, 0xC7);
    TEST("LD @R7,%F0",   0xF5, 0xF0, R(7));

    TZ88("LDW %34,%12",    0xC4, 0x12, 0x34);
    TZ88("LDW %34,@%12",   0xC5, 0x12, 0x34);
    TZ88("LDW %12,#%3456", 0xC6, 0x12, 0x34, 0x56);
    TZ88("LDW RR4,RR2",    0xC4, R(2), R(4));
    TZ88("LDW RR4,@R2",    0xC5, R(2), R(4));
    TZ88("LDW RR2,#%3456", 0xC6, R(2), 0x34, 0x56);
    EZ86("LDW %34,%12",    UNKNOWN_INSTRUCTION, "LDW %34,%12");

    TZ86("LDC R11,@RR4",   0xC2, 0xB4);
    TZ88("LDC R11,@RR4",   0xC3, 0xB4);
    TZ86("LDCI @R11,@RR4", 0xC3, 0xB4);
    EZ88("LDCI @R11,@RR4", OPERAND_NOT_ALLOWED, "@R11,@RR4");
    TZ86("LDC @RR4,R13",   0xD2, 0xD4);
    TZ88("LDC @RR4,R13",   0xD3, 0xD4);
    TZ86("LDCI @RR4,@R13", 0xD3, 0xD4);
    EZ88("LDCI @RR4,@R13", OPERAND_NOT_ALLOWED, "@RR4,@R13");

    TZ86("LDE R7,@RR4",   0x82, 0x74);
    TZ88("LDE R7,@RR4",   0xC3, 0x74 +1);
    TZ86("LDEI @R7,@RR4", 0x83, 0x74);
    EZ88("LDEI @R7,@RR4", OPERAND_NOT_ALLOWED, "@R7,@RR4");
    TZ86("LDE @RR4,R9",   0x92, 0x94);
    TZ88("LDE @RR4,R9",   0xD3, 0x94 +1);
    TZ86("LDEI @RR4,@R9", 0x93, 0x94);
    EZ88("LDEI @RR4,@R9", OPERAND_NOT_ALLOWED, "@RR4,@R9");

    TZ88("LDC R10,%A2A1", 0xA7, 0xA0, 0xA1, 0xA2);
    TZ88("LDE R10,%A3A2", 0xA7, 0xA1, 0xA2, 0xA3);
    TZ88("LDC %B2B1,R11", 0xB7, 0xB0, 0xB1, 0xB2);
    TZ88("LDE %B3B2,R11", 0xB7, 0xB1, 0xB2, 0xB3);
    TZ88("LDCD R15,@RR2",  0xE2, 0xF2);
    TZ88("LDED R15,@RR2",  0xE2, 0xF3);
    TZ88("LDCI R14,@RR4",  0xE3, 0xE4);
    TZ88("LDEI R14,@RR4",  0xE3, 0xE5);
    TZ88("LDCPD @RR2,R15", 0xF2, 0xF2);
    TZ88("LDEPD @RR2,R15", 0xF2, 0xF3);
    TZ88("LDCPI @RR4,R15", 0xF3, 0xF4);
    TZ88("LDEPI @RR4,R15", 0xF3, 0xF5);

    EZ86("LDC R10,%A2A1", OPERAND_NOT_ALLOWED, "R10,%A2A1");
    EZ86("LDE R10,%A3A2", OPERAND_NOT_ALLOWED, "R10,%A3A2");
    EZ86("LDC %B2B1,R11", OPERAND_NOT_ALLOWED, "%B2B1,R11");
    EZ86("LDE %B3B2,R11", OPERAND_NOT_ALLOWED, "%B3B2,R11");
    EZ86("LDCD R15,@RR2",  UNKNOWN_INSTRUCTION, "LDCD R15,@RR2");
    EZ86("LDED R15,@RR2",  UNKNOWN_INSTRUCTION, "LDED R15,@RR2");
    EZ86("LDCI R14,@RR4",  OPERAND_NOT_ALLOWED, "R14,@RR4");
    EZ86("LDEI R14,@RR4",  OPERAND_NOT_ALLOWED, "R14,@RR4");
    EZ86("LDCPD @RR2,R15", UNKNOWN_INSTRUCTION, "LDCPD @RR2,R15");
    EZ86("LDEPD @RR2,R15", UNKNOWN_INSTRUCTION, "LDEPD @RR2,R15");
    EZ86("LDCPI @RR4,R15", UNKNOWN_INSTRUCTION, "LDCPI @RR4,R15");
    EZ86("LDEPI @RR4,R15", UNKNOWN_INSTRUCTION, "LDEPI @RR4,R15");

    TZ88("PUSHUD @%23,%45", 0x82, 0x23, 0x45);
    TZ88("PUSHUI @%23,%45", 0x83, 0x23, 0x45);
    TZ88("PUSHUD @R2, R3",  0x82, 0xC2, 0xC3);
    TZ88("PUSHUI @R2, R3",  0x83, 0xC2, 0xC3);
    TZ88("POPUD  %23,@%45", 0x92, 0x45, 0x23);
    TZ88("POPUI  %23,@%45", 0x93, 0x45, 0x23);
    TZ88("POPUD  R2, @R3",  0x92, 0xC3, 0xC2);
    TZ88("POPUI  R2, @R3",  0x93, 0xC3, 0xC2);
}

static void test_indexed() {
    TZ86("LD R12,%C9(R8)",  0xC7, 0xC8, 0xC9);
    TZ88("LD R12,%C9(R8)",  0x87, 0xC8, 0xC9);
    TZ86("LD %D9(R8),R13",  0xD7, 0xD8, 0xD9);
    TZ88("LD %D9(R8),R13",  0x97, 0xD8, 0xD9);
    EZ86("LD R12, -129(R8)", OVERFLOW_RANGE, "-129(R8)", 0xC7, 0xC8, 0x7F);
    EZ88("LD R12, -129(R8)", OVERFLOW_RANGE, "-129(R8)", 0x87, 0xC8, 0x7F);
    TZ86("LD R12, -128(R8)", 0xC7, 0xC8, 0x80);
    TZ88("LD R12, -128(R8)", 0x87, 0xC8, 0x80);
    TZ86("LD R12,   -1(R8)", 0xC7, 0xC8, 0xFF);
    TZ88("LD R12,   -1(R8)", 0x87, 0xC8, 0xFF);
    TZ86("LD R12, +127(R8)", 0xC7, 0xC8, 0x7F);
    TZ88("LD R12, +127(R8)", 0x87, 0xC8, 0x7F);
    TZ86("LD R12, +128(R8)", 0xC7, 0xC8, 0x80);
    TZ88("LD R12, +128(R8)", 0x87, 0xC8, 0x80);
    TZ86("LD R12, +255(R8)", 0xC7, 0xC8, 0xFF);
    TZ88("LD R12, +255(R8)", 0x87, 0xC8, 0xFF);
    EZ86("LD R12, +256(R8)", OVERFLOW_RANGE, "+256(R8)", 0xC7, 0xC8, 0x00);
    EZ88("LD R12, +256(R8)", OVERFLOW_RANGE, "+256(R8)", 0x87, 0xC8, 0x00);
    EZ86("LD -129(R8), R12", OVERFLOW_RANGE, "-129(R8), R12", 0xD7, 0xC8, 0x7F);
    EZ88("LD -129(R8), R12", OVERFLOW_RANGE, "-129(R8), R12", 0x97, 0xC8, 0x7F);
    TZ86("LD -128(R8), R12", 0xD7, 0xC8, 0x80);
    TZ88("LD -128(R8), R12", 0x97, 0xC8, 0x80);
    TZ86("LD   -1(R8), R12", 0xD7, 0xC8, 0xFF);
    TZ88("LD   -1(R8), R12", 0x97, 0xC8, 0xFF);
    TZ86("LD +127(R8), R12", 0xD7, 0xC8, 0x7F);
    TZ88("LD +127(R8), R12", 0x97, 0xC8, 0x7F);
    TZ86("LD +128(R8), R12", 0xD7, 0xC8, 0x80);
    TZ88("LD +128(R8), R12", 0x97, 0xC8, 0x80);
    TZ86("LD +255(R8), R12", 0xD7, 0xC8, 0xFF);
    TZ88("LD +255(R8), R12", 0x97, 0xC8, 0xFF);
    EZ86("LD +256(R8), R12", OVERFLOW_RANGE, "+256(R8), R12", 0xD7, 0xC8, 0x00);
    EZ88("LD +256(R8), R12", OVERFLOW_RANGE, "+256(R8), R12", 0x97, 0xC8, 0x00);

    TZ88("LDC R14, -129(RR8)", 0xA7, 0xE8, 0x7F, 0xFF);
    TZ88("LDC R14, -128(RR8)", 0xE7, 0xE8, 0x80);
    TZ88("LDC R14,   -1(RR8)", 0xE7, 0xE8, 0xFF);
    TZ88("LDC R14,%FFFF(RR8)", 0xA7, 0xE8, 0xFF, 0xFF);
    TZ88("LDC R14, +127(RR8)", 0xE7, 0xE8, 0x7F);
    TZ88("LDC R14, +128(RR8)", 0xA7, 0xE8, 0x80, 0x00);
    TZ88("LDC -129(RR8), R15", 0xB7, 0xF8, 0x7F, 0xFF);
    TZ88("LDC -128(RR8), R15", 0xF7, 0xF8, 0x80);
    TZ88("LDC   -1(RR8), R15", 0xF7, 0xF8, 0xFF);
    TZ88("LDC %FFFF(RR8),R15", 0xB7, 0xF8, 0xFF, 0xFF);
    TZ88("LDC +127(RR8), R15", 0xF7, 0xF8, 0x7F);
    TZ88("LDC +128(RR8), R15", 0xB7, 0xF8, 0x80, 0x00);
    TZ88("LDE R14, -129(RR8)", 0xA7, 0xE9, 0x7F, 0xFF);
    TZ88("LDE R14, -128(RR8)", 0xE7, 0xE9, 0x80);
    TZ88("LDE R14,   -1(RR8)", 0xE7, 0xE9, 0xFF);
    TZ88("LDE R14,%FFFF(RR8)", 0xA7, 0xE9, 0xFF, 0xFF);
    TZ88("LDE R14, +127(RR8)", 0xE7, 0xE9, 0x7F);
    TZ88("LDE R14, +128(RR8)", 0xA7, 0xE9, 0x80, 0x00);
    TZ88("LDE -129(RR8), R15", 0xB7, 0xF9, 0x7F, 0xFF);
    TZ88("LDE -128(RR8), R15", 0xF7, 0xF9, 0x80);
    TZ88("LDE   -1(RR8), R15", 0xF7, 0xF9, 0xFF);
    TZ88("LDE %FFFF(RR8),R15", 0xB7, 0xF9, 0xFF, 0xFF);
    TZ88("LDE +127(RR8), R15", 0xF7, 0xF9, 0x7F);
    TZ88("LDE +128(RR8), R15", 0xB7, 0xF9, 0x80, 0x00);

    EZ86("LDC R14,-128(RR8)", OPERAND_NOT_ALLOWED, "R14,-128(RR8)");
    EZ86("LDC R14,+128(RR8)", OPERAND_NOT_ALLOWED, "R14,+128(RR8)");
    EZ86("LDE R14,-128(RR8)", OPERAND_NOT_ALLOWED, "R14,-128(RR8)");
    EZ86("LDE R14,+128(RR8)", OPERAND_NOT_ALLOWED, "R14,+128(RR8)");

    EZ88("LDC +128(RR0), R15", REGISTER_NOT_ALLOWED, "RR0), R15", 0xB7, 0xF0, 0x80, 0x00);
    EZ88("LDE R14, +128(RR0)", REGISTER_NOT_ALLOWED, "RR0)",      0xA7, 0xE1, 0x80, 0x00);

    symtab.intern(0xC9, "bufC9");
    symtab.intern(-2,   "offm2");
    symtab.intern(0xFFFE, "symm2");
    symtab.intern(0xABAA, "table");

    TZ86("LD R12,bufC9(R8)", 0xC7, 0xC8, 0xC9);
    TZ88("LD R12,bufC9(R8)", 0x87, 0xC8, 0xC9);
    TZ86("LD R12,offm2(R8)", 0xC7, 0xC8, 0xFE);
    TZ88("LD R12,offm2(R8)", 0x87, 0xC8, 0xFE);
    TZ88("LDE R10,bufC9(RR8)", 0xA7, 0xA9, 0xC9, 0x00);
    TZ88("LDE R10,offm2(RR8)", 0xE7, 0xA9, 0xFE);
    TZ88("LDE R10,symm2(RR8)", 0xA7, 0xA9, 0xFE, 0xFF);
    TZ88("LDE R10,table(RR8)", 0xA7, 0xA9, 0xAA, 0xAB);

    TEST("option optimize-index, on");
    TZ86("LD R3, -1(R4)", 0xC7, 0x34, 0xFF);
    TZ86("LD R3,  0(R4)", 0xE3, 0x34); // LD R3, @R4
    TZ86("LD R3,   @R4",  0xE3, 0x34);
    TZ86("LD -1(R4), R3", 0xD7, 0x34, 0xFF);
    TZ86("LD  0(R4), R3", 0xF3, 0x43); // LD @R4, R3
    TZ86("LD   @R4,  R3", 0xF3, 0x43);
    TZ88("LD R3, -1(R4)", 0x87, 0x34, 0xFF);
    TZ88("LD R3,  0(R4)", 0xC7, 0x34); // LD R3, @R4
    TZ88("LD R3,   @R4",  0xC7, 0x34);
    TZ88("LD -1(R4), R3", 0x97, 0x34, 0xFF);
    TZ88("LD  0(R4), R3", 0xD7, 0x43); // LD @R4, R3
    TZ88("LD   @R4,  R3", 0xD7, 0x43);
    TZ88("LDE R3, -1(RR4)", 0xE7, 0x35, 0xFF);
    TZ88("LDE R3,  0(RR4)", 0xC3, 0x35); // LDE R3, @RR4
    TZ88("LDE R3,   @RR4",  0xC3, 0x35);
    TZ88("LDE -1(RR4), R3", 0xF7, 0x35, 0xFF);
    TZ88("LDE  0(RR4), R3", 0xD3, 0x35); // LDE @RR4, R3
    TZ88("LDE   @RR4,  R3", 0xD3, 0x35);
}

static void test_bit_operation() {
    TEST("BITC R5,#4", 0x57, 0x58);
    TEST("BITR R7,#4", 0x77, 0x78);
    TEST("BITS R8,#4", 0x77, 0x89);

    TEST("BOR  R0,>%09,#4", 0x07, 0x08, 0x09);
    TEST("BOR  R0,R9,#4",   0x07, 0x08, R(9));
    TEST("BOR  %AB,#4,R8",  0x07, 0x89, 0xAB);
    TEST("BOR  R11,#4,R8",  0x07, 0x89, 0xCB);
    TEST("BCP  R1,%19,#4",  0x17, 0x18, 0x19);
    TEST("BCP  R1,R9,#4",   0x17, 0x18, R(9));
    TEST("BXOR R2,%29,#4",  0x27, 0x28, 0x29);
    TEST("BXOR R2,R9,#4",   0x27, 0x28, R(9));
    TEST("BXOR %AB,#4,R8",  0x27, 0x89, 0xAB);
    TEST("BXOR R11,#4,R8",  0x27, 0x89, 0xCB);
    TEST("LDB  R4,%49,#4",  0x47, 0x48, 0x49);
    TEST("LDB  R4,R9,#4",   0x47, 0x48, R(9));
    TEST("LDB  %AB,#4,R8",  0x47, 0x89, 0xAB);
    TEST("LDB  R11,#4,R8",  0x47, 0x89, 0xCB);
    TEST("BAND R7,%89,#4",  0x67, 0x78, 0x89);
    TEST("BAND R7,R9,#4",   0x67, 0x78, R(9));
    TEST("BAND %AB,#4,R8",  0x67, 0x89, 0xAB);
    TEST("BAND R11,#4,R8",  0x67, 0x89, 0xCB);

    TEST("BTJRF $,R3,#4", 0x37, 0x38, 0xFD);
    TEST("BTJRT $,R3,#4", 0x37, 0x39, 0xFD);
}

static void test_setrp() {
    TEST("SETRP -1");
    TEST("LD  %01,R4",   0x49, 0x01);
    TEST("LD  %01,%0E",  0xE4, 0x0E, 0x01);
    TEST("LD   R4,%0E",  0x48, 0x0E);
    TEST("LD  %21,R4",   0x49, 0x21);
    TEST("LD   R4,%21",  0x48, 0x21);
    TEST("LD  %00,#%66", 0xE6, 0x00, 0x66);
    TEST("LD  %0F,#%66", 0xE6, 0x0F, 0x66);
    TEST("LD  %21,#%66", 0xE6, 0x21, 0x66);
    TEST("LD  %0E,%24",  0xE4, 0x24, 0x0E);
    TEST("LD  %24,%01",  0XE4, 0x01, 0x24);
    TEST("LD  %0E,@%24", 0xE5, 0x24, 0x0E);
    TEST("LD  %21,@%0E", 0xE5, 0x0E, 0x21);
    TEST("LD @%0E,%24",  0xF5, 0x24, 0x0E);
    TEST("ADD %0E,%24",  0x04, 0x24, 0x0E);
    TEST("ADD %0E,@%24", 0x05, 0x24, 0x0E);
    TEST("CLR R14",      0xB0, R(14));
    TEST("CLR %0E",      0xB0, 0x0E);
    TEST("SETRP 0");
    TEST("LD  %01,R4",   0x18, R(4));
    TEST("LD  %01,%0E",  0x18, R(14));
    TEST("LD   R4,%0E",  0x48, R(14));
    TEST("LD  %21,R4",   0x49, 0x21);
    TEST("LD   R4,%21",  0x48, 0x21);
    TEST("LD  %00,#%66", 0x0C, 0x66);
    TEST("LD  %0F,#%66", 0xFC, 0x66);
    TEST("LD  %21,#%66", 0xE6, 0x21, 0x66);
    TEST("LD  %0E,%24",  0xE8, 0x24);
    TEST("LD  %24,%01",  0X19, 0x24);
    TEST("LD  %0E,@%24", 0xE5, 0x24, R(14));
    TEST("LD  %21,@%0E", 0xE5, R(14), 0x21);
    TEST("LD @%0E,%24",  0xF5, 0x24, R(14));
    TEST("ADD %0E,%24",  0x04, 0x24, R(14));
    TEST("ADD %0E,@%24", 0x05, 0x24, R(14));
    TEST("ADD %21,%24",  0x04, 0x24, 0x21);
    TEST("ADD %21,@%24", 0x05, 0x24, 0x21);
    TEST("CLR R14",      0xB0, R(14));
    TEST("CLR %0E",      0xB0, R(14));
    ERRT("LD  %21,%33(R4)", OPERAND_NOT_ALLOWED, "%21,%33(R4)"); // LD R,dd(r)
    ERRT("LD %33(%21),R4",  UNKNOWN_OPERAND, "%33(%21),R4"); // LD dd(R),r
    TZ86("LDE  R1,@RR4", 0x82, 0x14);
    TZ88("LDE  R1,@RR4", 0xC3, 0x14 +1);
    TZ86("LDE  @RR4,R1", 0x92, 0x14);
    TZ88("LDE  @RR4,R1", 0xD3, 0x14 +1);
    TZ86("LDEI @R1,@RR4", 0x83, 0x14);
    EZ88("LDEI @R1,@RR4", OPERAND_NOT_ALLOWED, "@R1,@RR4");
    TZ86("LDEI @RR4,@R1", 0x93, 0x14);
    EZ88("LDEI @RR4,@R1", OPERAND_NOT_ALLOWED, "@RR4,@R1");
    TEST("DJNZ  R1,$", 0x1A, 0xFE);
    TEST("DJNZ %0E,$", 0xEA, 0xFE);
    ERRT("DJNZ %21,$", OPERAND_NOT_ALLOWED, "%21,$");
    TEST("JP   @RR4", 0x30, R(4));
    TEST("JP   @%04", 0x30, R(4));
    TEST("JP   @%24", 0x30, 0x24);
    TZ86("CALL @%24", 0xD4, 0x24);
    TZ88("CALL @%24", 0xF4, 0x24);

    TEST(R"(option "reg-alias", "disable")");
    TEST("LD  %01,%0E",  0x18, 0x0E);
    TEST("LD   R4,%0E",  0x48, 0x0E);
    TEST("CLR %0E",      0xB0, 0x0E);
    TEST("JP   @%04",    0x30, 0x04);
    TEST(R"(option "reg-alias", "enable")");

    TEST("SETRP %21 & %F0");
    TEST("LD  %21,R4",   0x18, R(4));
    TEST("LD  R4,%21",   0x48, R(1));
    TEST("LD  %21,#%66", 0x1C, 0x66);
    TZ86("LD  %21,%33(R4)", 0xC7, 0x14, 0x33); // LD R1,%33(R4)
    TZ88("LD  %21,%33(R4)", 0x87, 0x14, 0x33); // LD R1,%33(R4)
    TZ86("LD  %33(R4),%21", 0xD7, 0x14, 0x33); // LD %33(R4),R1
    TZ88("LD  %33(R4),%21", 0x97, 0x14, 0x33); // LD %33(R4),R1
    TZ86("LD  R1,%33(%24)", 0xC7, 0x14, 0x33); // LD R1,%33(R4)
    TZ88("LD  R1,%33(%24)", 0x87, 0x14, 0x33); // LD R1,%33(R4)
    TZ86("LD  %33(%24),R1", 0xD7, 0x14, 0x33); // LD %33(R4),R1
    TZ88("LD  %33(%24),R1", 0x97, 0x14, 0x33); // LD %33(R4),R1
    TZ86("LD  %21,%33(%24)", 0xC7, 0x14, 0x33); // LD R1,%33(R4)
    TZ88("LD  %21,%33(%24)", 0x87, 0x14, 0x33); // LD R1,%33(R4)
    TZ86("LD  %33(%24),%21", 0xD7, 0x14, 0x33); // LD %33(R4),R1
    TZ88("LD  %33(%24),%21", 0x97, 0x14, 0x33); // LD %33(R4),R1
    TEST("LD  %21,%24",  0x18, R(4));
    TEST("LD  %24,%21",  0X48, R(1));
    TZ86("LD  %21,@%24", 0xE3, 0x14);
    TZ88("LD  %21,@%24", 0xC7, 0x14);
    TZ86("LD  @%21,%24", 0xF3, 0x14);
    TZ88("LD  @%21,%24", 0xD7, 0x14);
    TEST("ADD %21,%24",  0x02, 0x14);
    TEST("ADD %21,@%24", 0x03, 0x14);
    TZ86("LDE  %21,@%24",  0x82, 0x14);
    TZ88("LDE  %21,@%24",  0xC3, 0x14 +1);
    TZ86("LDE  @%24,%21",  0x92, 0x14);
    TZ88("LDE  @%24,%21",  0xD3, 0x14 +1);
    TZ86("LDEI @%21,@%24", 0x83, 0x14);
    EZ88("LDEI @%21,@%24", OPERAND_NOT_ALLOWED, "@%21,@%24");
    TZ86("LDEI @%24,@%21", 0x93, 0x14);
    EZ88("LDEI @%24,@%21", OPERAND_NOT_ALLOWED, "@%24,@%21");
    TEST("DJNZ %21,$", 0x1A, 0xFE);
    TEST("JP   @%24",  0x30, R(4));
    TZ86("CALL @%24", 0xD4, R(4));
    TZ88("CALL @%24", 0xF4, R(4));

    assembler.setOption("reg-alias", "false");
    TEST("LD  R4,%21",   0x48, 0x21);
    TEST("LD  %21,%24",  0x18, 0x24);
    TEST("LD  %24,%21",  0X48, 0x21);
    TEST("JP   @%24",  0x30, 0x24);
    TZ86("CALL @%24", 0xD4, 0x24);
    TZ88("CALL @%24", 0xF4, 0x24);
    assembler.setOption("reg-alias", "true");

    TEST("SETRP -1");
    ERRT("SETRP %100",  ILLEGAL_OPERAND,     "%100");

    EZ86("SETRP0 %08",  UNKNOWN_INSTRUCTION, "SETRP0 %08");
    TZ88("SETRP0 %08");
    EZ88("SETRP0 %09",  ILLEGAL_OPERAND,     "%09");
    TZ88("SETRP0 -1");
    EZ88("SETRP0 %100", ILLEGAL_OPERAND,     "%100");
    EZ86("SETRP1 %18",  UNKNOWN_INSTRUCTION, "SETRP1 %18");
    TZ88("SETRP1 %18");
    EZ88("SETRP1 %1F",  ILLEGAL_OPERAND,     "%1F");
    TZ88("SETRP1 -1");
    EZ88("SETRP1 %118", ILLEGAL_OPERAND,     "%118");

    TEST("SRP #%10",                              0x31, 0x10);

    EZ86("SRP #%11", OPERAND_NOT_ALLOWED, "#%11", 0x31, 0x11);
    EZ86("SRP #%12", OPERAND_NOT_ALLOWED, "#%12", 0x31, 0x12);
    EZ86("SRP #%13", OPERAND_NOT_ALLOWED, "#%13", 0x31, 0x13);
    EZ86("SRP #%14", OPERAND_NOT_ALLOWED, "#%14", 0x31, 0x14);
    EZ86("SRP #%15", OPERAND_NOT_ALLOWED, "#%15", 0x31, 0x15);
    EZ86("SRP #%16", OPERAND_NOT_ALLOWED, "#%16", 0x31, 0x16);
    EZ86("SRP #%17", OPERAND_NOT_ALLOWED, "#%17", 0x31, 0x17);
    EZ86("SRP #%18", OPERAND_NOT_ALLOWED, "#%18", 0x31, 0x18);
    EZ86("SRP #%19", OPERAND_NOT_ALLOWED, "#%19", 0x31, 0x19);
    EZ86("SRP #%1A", OPERAND_NOT_ALLOWED, "#%1A", 0x31, 0x1A);
    EZ86("SRP #%1B", OPERAND_NOT_ALLOWED, "#%1B", 0x31, 0x1B);
    EZ86("SRP #%1C", OPERAND_NOT_ALLOWED, "#%1C", 0x31, 0x1C);
    EZ86("SRP #%1D", OPERAND_NOT_ALLOWED, "#%1D", 0x31, 0x1D);
    EZ86("SRP #%1E", OPERAND_NOT_ALLOWED, "#%1E", 0x31, 0x1E);
    EZ86("SRP #%1F", OPERAND_NOT_ALLOWED, "#%1F", 0x31, 0x1F);

    EZ88("SRP #%11", OPERAND_NOT_ALLOWED, "#%11", 0x31, 0x10);
    EZ88("SRP #%12", OPERAND_NOT_ALLOWED, "#%12", 0x31, 0x10);
    EZ88("SRP #%13", OPERAND_NOT_ALLOWED, "#%13", 0x31, 0x10);
    EZ88("SRP #%14", OPERAND_NOT_ALLOWED, "#%14", 0x31, 0x14);
    EZ88("SRP #%15", OPERAND_NOT_ALLOWED, "#%15", 0x31, 0x14);
    EZ88("SRP #%16", OPERAND_NOT_ALLOWED, "#%16", 0x31, 0x14);
    EZ88("SRP #%17", OPERAND_NOT_ALLOWED, "#%17", 0x31, 0x14);
    EZ88("SRP #%18", OPERAND_NOT_ALLOWED, "#%18", 0x31, 0x18);
    EZ88("SRP #%19", OPERAND_NOT_ALLOWED, "#%19", 0x31, 0x18);
    EZ88("SRP #%1A", OPERAND_NOT_ALLOWED, "#%1A", 0x31, 0x18);
    EZ88("SRP #%1B", OPERAND_NOT_ALLOWED, "#%1B", 0x31, 0x18);
    EZ88("SRP #%1C", OPERAND_NOT_ALLOWED, "#%1C", 0x31, 0x1C);
    EZ88("SRP #%1D", OPERAND_NOT_ALLOWED, "#%1D", 0x31, 0x1C);
    EZ88("SRP #%1E", OPERAND_NOT_ALLOWED, "#%1E", 0x31, 0x1C);
    EZ88("SRP #%1F", OPERAND_NOT_ALLOWED, "#%1F", 0x31, 0x1C);

    TEST("SRP #%20",                              0x31, 0x20);

    TZ88("SRP0 #%10",                              0x31, 0x12);
    EZ88("SRP0 #%11", OPERAND_NOT_ALLOWED, "#%11", 0x31, 0x12);
    EZ88("SRP0 #%12", OPERAND_NOT_ALLOWED, "#%12", 0x31, 0x12);
    EZ88("SRP0 #%13", OPERAND_NOT_ALLOWED, "#%13", 0x31, 0x12);
    EZ88("SRP0 #%14", OPERAND_NOT_ALLOWED, "#%14", 0x31, 0x16);
    EZ88("SRP0 #%15", OPERAND_NOT_ALLOWED, "#%15", 0x31, 0x16);
    EZ88("SRP0 #%16", OPERAND_NOT_ALLOWED, "#%16", 0x31, 0x16);
    EZ88("SRP0 #%17", OPERAND_NOT_ALLOWED, "#%17", 0x31, 0x16);
    TZ88("SRP0 #%18",                              0x31, 0x1A);

    TZ88("SRP1 #%10",                              0x31, 0x11);
    EZ88("SRP1 #%11", OPERAND_NOT_ALLOWED, "#%11", 0x31, 0x11);
    EZ88("SRP1 #%12", OPERAND_NOT_ALLOWED, "#%12", 0x31, 0x11);
    EZ88("SRP1 #%13", OPERAND_NOT_ALLOWED, "#%13", 0x31, 0x11);
    EZ88("SRP1 #%14", OPERAND_NOT_ALLOWED, "#%14", 0x31, 0x15);
    EZ88("SRP1 #%15", OPERAND_NOT_ALLOWED, "#%15", 0x31, 0x15);
    EZ88("SRP1 #%16", OPERAND_NOT_ALLOWED, "#%16", 0x31, 0x15);
    EZ88("SRP1 #%17", OPERAND_NOT_ALLOWED, "#%17", 0x31, 0x15);
    TZ88("SRP1 #%18",                              0x31, 0x19);
}

static void test_comment() {
    COMM(" RET             ; comment", "; comment", 0xAF);
    COMM(" JP  ULE , %3E3F ; comment", "; comment", 0x3D, 0x3E, 0x3F);
    COMM(" JP  %8E8F       ; comment", "; comment", 0x8D, 0x8E, 0x8F);
    COMM(" LD  R1 , >%0F   ; comment", "; comment", 0x18, 0x0F);
    COMM(" LD  R2 , R0     ; comment", "; comment", 0x28, R(0));
    COMM(" LD  >%0F , R1   ; comment", "; comment", 0x19, 0x0F);
    COMM(" LD  R9 , #%9D   ; comment", "; comment", 0x9C, 0x9D);
    COMM(" INC R0          ; comment", "; comment", 0x0E);
    COMM(" RLC %11         ; comment", "; comment", 0x10, 0x11);
    COMM(" RLC R1          ; comment", "; comment", 0x10, R(1));
    COMM(" RLC @%12        ; comment", "; comment", 0x11, 0x12);
    COMM(" RLC @R2         ; comment", "; comment", 0x11, R(2));
    COMM(" ADD >%07, @%16  ; comment", "; comment", 0x05, 0x16, 0x07);
    COMM(" ADD >%07, @R6   ; comment", "; comment", 0x05, R(6), 0x07);
    COMM(" ADD R7 , @%16   ; comment", "; comment", 0x05, 0x16, R(7));
    if (z86()) {
        COMM(" LDC @RR4 , R13  ; comment", "; comment", 0xD2, 0xD4);
        COMM(" LD  R12 , %C9 (R8) ; comment", "; comment", 0xC7, 0xC8, 0xC9);
        COMM(" LD  %D9 (R8) , R13 ; comment", "; comment", 0xD7, 0xD8, 0xD9);
    }
    if (z88()) {
        COMM(" LDC @RR4 , R13  ; comment", "; comment", 0xD3, 0xD4);
        COMM(" LD  R12 , %C9 (R8) ; comment", "; comment", 0x87, 0xC8, 0xC9);
        COMM(" LD  %D9 (R8) , R13 ; comment", "; comment", 0x97, 0xD8, 0xD9);
    }
    ACOMM(0x1000, " JR   C , %107E  ; comment", "; comment", 0x7B, 0x7C);
    ACOMM(0x1000, " DJNZ R2 , %102D ; comment", "; comment", 0x2A, 0x2B);

    COMM("DB -128, 255 ; comment", "; comment", 0x80, 0xFF);
    COMM("DB 'TEXT'    ; comment", "; comment", 0x54, 0x45, 0x58, 0x54);
    COMM("DW -128, 255 ; comment", "; comment", 0xFF, 0x80, 0x00, 0xFF);
    COMM("DL %12345678 ; comment", "; comment", 0x12, 0x34, 0x56, 0x78);
}

static void test_undef() {
    ERUS("JP C,UNDEF", "UNDEF", 0x7D, 0x00, 0x00);
    ERUS("JP   UNDEF", "UNDEF", 0x8D, 0x00, 0x00);
    if (z86()) {
        ERUS("CALL UNDEF", "UNDEF", 0xD6, 0x00, 0x00);
    }
    if (z88()) {
        ERUS("CALL UNDEF", "UNDEF", 0xF6, 0x00, 0x00);
    }
    ERUS("LD    R0,UNDEF", "UNDEF",       0x08, 0x00);
    ERUS("LD UNDEF,R0",    "UNDEF,R0",    0x09, 0x00);
    ERUS("LD UNDEF,UNDEF", "UNDEF,UNDEF", 0xE4, 0x00, 0x00);
    ERUS("JR   UNDEF",     "UNDEF",       0x8B, 0x00);
    ERUS("JR Z,UNDEF",     "UNDEF",       0x6B, 0x00);
    ERUS("DJNZ R0,UNDEF",  "UNDEF",       0x0A, 0x00);

    if (z88()) {
        ERUS("BITS R8,#UNDEF",       "UNDEF",           0x77, 0x81);
        ERUS("BXOR R2,R8,#UNDEF",    "UNDEF",           0x27, 0x20, 0xC8);
        ERUS("BXOR R2,UNDEF,#4",     "UNDEF,#4",        0x27, 0x28, 0x00);
        ERUS("BXOR R2,UNDEF,#UNDEF", "UNDEF,#UNDEF",    0x27, 0x20, 0x00);
        ERUS("BXOR R2,#UNDEF,R8",    "UNDEF,R8",        0x27, 0x81, 0xC2);
        ERUS("BXOR UNDEF,#5,R8",     "UNDEF,#5,R8",     0x27, 0x8B, 0x00);
        ERUS("BXOR UNDEF,#UNDEF,R8", "UNDEF,#UNDEF,R8", 0x27, 0x81, 0x00);
        ERUS("CPIJE  R3,@R12,UNDEF", "UNDEF",           0xC2, 0xC3, 0x00);
    }
}

static void test_error() {
    ERRT("JP   @11",    OPERAND_NOT_ALLOWED, "@11");
    ERRT("CALL @0e5h",  OPERAND_NOT_ALLOWED, "@0e5h");
    ERRT("JP   @r0",    OPERAND_NOT_ALLOWED, "@r0");
    ERRT("CALL @r4",    OPERAND_NOT_ALLOWED, "@r4");
    TEST("LD  >%0F,R1", 0x19, 0x0F);
    ERRT("LD > %0F,R1", UNKNOWN_OPERAND, "> %0F,R1");
    TEST("DEC @>%15",   0x01, 0x15);
    ERRT("DEC @ >%15",  UNKNOWN_OPERAND, "@ >%15");
    ERRT("DEC @> %15",  UNKNOWN_OPERAND, "@> %15");
    EZ88("BITS R8,#8",     ILLEGAL_BIT_NUMBER, "#8",  0x77, 0x81);
    EZ88("BITS R8,#-1",    ILLEGAL_BIT_NUMBER, "#-1", 0x77, 0x8F);
    EZ88("BXOR R2,R8,#8",  ILLEGAL_BIT_NUMBER, "#8",  0x27, 0x20, 0xC8);
    EZ88("BXOR R2,R8,#-1", ILLEGAL_BIT_NUMBER, "#-1", 0x27, 0x2E, 0xC8);
}

static void test_data_constant() {
    TEST("DB -128, 255", 0x80, 0xFF);
    TEST(R"(DB 'A', '"')", 0x41, 0x22);
    TEST("DB '9'-'0'",   0x09);
    TEST("DB '%27'",     0x27);
    ERRT("DB '%2'",      UNKNOWN_ESCAPE_SEQUENCE, "%2'");
    TEST("DB '%q'",      0x27);
    ERRT("DB '''",       ILLEGAL_CONSTANT, "''");
    TEST("DB 'A%22B',0", 0x41, 0x22, 0x42, 0x00);
    TEST("DB 'A%QB',0",  0x41, 0x27, 0x42, 0x00);
    ERRT("DB 'A%QB,0",   MISSING_CLOSING_QUOTE, "'A%QB,0");
    TEST("DW -128, 255", 0xFF, 0x80, 0x00, 0xFF);
    TEST("DW 'A%QB'",    0x41, 0x27, 0x42, 0x00);
    ERRT("DW 'A%QB",     MISSING_CLOSING_QUOTE, "'A%QB");
    TEST("DL %12345678", 0x12, 0x34, 0x56, 0x78);
    TEST("DL 'A%QB%22C'", 0x41, 0x27, 0x42, 0x22, 0x43, 0x00, 0x00, 0x00);
    ERRT("DL 'A%QB%22C",  MISSING_CLOSING_QUOTE, "'A%QB%22C");
    ERUS("DB 1, UNDEF, 2", "UNDEF, 2", 0x01, 0x00, 0x02);
    ERUS("DW 1, UNDEF, 2", "UNDEF, 2", 0x00, 0x01, 0x00, 0x00, 0x00, 0x02);
    ERUS("DL 1, UNDEF, 2", "UNDEF, 2",
         0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02);

    ERRT("DB '"
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

    ERRT("DW "
         "%1234, %5678, %9ABC, %DEF0, %1234, %5678, %9ABC, %DEF0, "
         "%1234, %5678, %9ABC, %DEF0, %1234, %5678, %9ABC, %DEF0, "
         "%1234, %5678, %9ABC, %DEF0, %1234, %5678, %9ABC, %DEF0, "
         "%1234, %5678, %9ABC, %DEF0, %1234, %5678, %9ABC, %DE, "
         "%AAAA, %BBBB",
         NO_MEMORY,
         "%AAAA, %BBBB",
         0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
         0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
         0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
         0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0x00, 0xDE);
}

// clang-format on

void run_tests(const char *cpu) {
    assembler.setCpu(cpu);
    RUN_TEST(test_implied);
    RUN_TEST(test_absolute);
    RUN_TEST(test_relative);
    RUN_TEST(test_operand_in_opcode);
    RUN_TEST(test_one_operand);
    RUN_TEST(test_two_operands);
    RUN_TEST(test_indexed);
    if (z88())
        RUN_TEST(test_bit_operation);
    RUN_TEST(test_setrp);
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

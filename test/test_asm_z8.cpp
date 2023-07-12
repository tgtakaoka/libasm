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
    return strcmp_P("Z86C", assembler.cpu_P()) == 0;
}

static bool z86() {
    return strcmp_P("Z8", assembler.cpu_P()) == 0 || z86c();
}

static bool z88() {
    return strcmp_P("Z88", assembler.cpu_P()) == 0;
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
    EQUALS_P("cpu z8", "Z8", assembler.cpu_P());

    EQUALS("cpu Z8601", true, assembler.setCpu("Z8601"));
    EQUALS_P("cpu Z8601", "Z8", assembler.cpu_P());

    EQUALS("cpu Z86C91", true,   assembler.setCpu("Z86C91"));
    EQUALS_P("cpu Z86C91", "Z86C", assembler.cpu_P());

    EQUALS("cpu Super8", true,  assembler.setCpu("Super8"));
    EQUALS_P("cpu Super8", "Z88", assembler.cpu_P());

    EQUALS("cpu Z88C00", true,  assembler.setCpu("Z88C00"));
    EQUALS_P("cpu Z88C00", "Z88", assembler.cpu_P());
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

    TEST("JP F,    0E0FH", 0x0D, 0x0E, 0x0F);
    TEST("JP LT,   1E1FH", 0x1D, 0x1E, 0x1F);
    TEST("JP LE,   2E2FH", 0x2D, 0x2E, 0x2F);
    TEST("jp ule,  3e3fh", 0x3D, 0x3E, 0x3F);
    TEST("JP OV,   4E4FH", 0x4D, 0x4E, 0x4F);
    TEST("JP MI,   5E5FH", 0x5D, 0x5E, 0x5F);
    TEST("JP Z,    6E6FH", 0x6D, 0x6E, 0x6F);
    TEST("JP C,  $s.7E7F", 0x7D, 0x7E, 0x7F);
    TEST("JP     ?s_8E8F", 0x8D, 0x8E, 0x8F);
    TEST("JP GE,   9E9FH", 0x9D, 0x9E, 0x9F);
    TEST("JP GT,  0AEAFH", 0xAD, 0xAE, 0xAF);
    TEST("JP UGT, 0BEBFH", 0xBD, 0xBE, 0xBF);
    TEST("JP NOV, 0CECFH", 0xCD, 0xCE, 0xCF);
    TEST("JP PL, .s?DEDF", 0xDD, 0xDE, 0xDF);
    TEST("JP NZ,  0EEEFH", 0xED, 0xEE, 0xEF);
    TEST("JP NC,  0FEFFH", 0xFD, 0xFE, 0xFF);
    TZ86("CALL   _s$D7D8", 0xD6, 0xD7, 0xD8);
}

static void test_relative() {
    symtab.intern(0x106E, "?s$106E");
    symtab.intern(0x0F8E, ".s_0F8E");

    ATEST(0x1000, "JR F,   100EH", 0x0B, 0x0C);
    ATEST(0x1000, "JR LT,  101EH", 0x1B, 0x1C);
    ATEST(0x1000, "JR LE,  102EH", 0x2B, 0x2C);
    ATEST(0x1000, "JR ULE, 103EH", 0x3B, 0x3C);
    ATEST(0x1000, "JR OV,  104EH", 0x4B, 0x4C);
    ATEST(0x1000, "JR MI,  105EH", 0x5B, 0x5C);
    ATEST(0x1000, "JR Z, ?s$106E", 0x6B, 0x6C);
    ATEST(0x1000, "JR EQ,  106EH", 0x6B, 0x6C);
    ATEST(0x1000, "JR C,   107EH", 0x7B, 0x7C);
    ATEST(0x1000, "JR ULT, 107EH", 0x7B, 0x7C);
    ATEST(0x1000, "JR    .s_0F8E", 0x8B, 0x8C);
    ATEST(0x1000, "JR GE,  0F9EH", 0x9B, 0x9C);
    ATEST(0x1000, "JR GT,  0FAEH", 0xAB, 0xAC);
    ATEST(0x1000, "JR UGT, 0FBEH", 0xBB, 0xBC);
    ATEST(0x1000, "JR NOV, 0FCEH", 0xCB, 0xCC);
    ATEST(0x1000, "JR PL,  0FDEH", 0xDB, 0xDC);
    ATEST(0x1000, "JR NZ,  0FEEH", 0xEB, 0xEC);
    ATEST(0x1000, "JR NE,  0FEEH", 0xEB, 0xEC);
    ATEST(0x1000, "JR NC,  0FFEH", 0xFB, 0xFC);
    ATEST(0x1000, "JR UGE, 0FFEH", 0xFB, 0xFC);

    ATEST(0x1000, "djnz  r0,100dh", 0x0A, 0x0B);
    ATEST(0x1000, "DJNZ  R1,101DH", 0x1A, 0x1B);
    ATEST(0x1000, "DJNZ  R2,102DH", 0x2A, 0x2B);
    ATEST(0x1000, "DJNZ  R3,103DH", 0x3A, 0x3B);
    ATEST(0x1000, "DJNZ  R4,104DH", 0x4A, 0x4B);
    ATEST(0x1000, "DJNZ  R5,105DH", 0x5A, 0x5B);
    ATEST(0x1000, "DJNZ  R6,106DH", 0x6A, 0x6B);
    ATEST(0x1000, "DJNZ  R7,107DH", 0x7A, 0x7B);
    ATEST(0x1000, "DJNZ  R8,0F8DH", 0x8A, 0x8B);
    ATEST(0x1000, "DJNZ  R9,0F9DH", 0x9A, 0x9B);
    ATEST(0x1000, "DJNZ R10,0FADH", 0xAA, 0xAB);
    ATEST(0x1000, "DJNZ R11,0FBDH", 0xBA, 0xBB);
    ATEST(0x1000, "DJNZ R12,0FCDH", 0xCA, 0xCB);
    ATEST(0x1000, "DJNZ R13,0FDDH", 0xDA, 0xDB);
    ATEST(0x1000, "DJNZ R14,0FEDH", 0xEA, 0xEB);
    ATEST(0x1000, "DJNZ R15,0FFDH", 0xFA, 0xFB);

    TZ88("CPIJE  R3,@R12,$", 0xC2, 0xC3, 0xFD);
    TZ88("CPIJNE R3,@R13,$", 0xD2, 0xD3, 0xFD);
    EZ86("CPIJE  R3,@R12,$", UNKNOWN_INSTRUCTION, "CPIJE  R3,@R12,$");
    EZ86("CPIJNE R3,@R13,$", UNKNOWN_INSTRUCTION, "CPIJNE R3,@R13,$");
}

static void test_operand_in_opcode() {
    TEST("LD  R0,>09H", 0x08, 0x09);
    TEST("LD  R0,R9",   0x08, R(9));
    TEST("LD  R1,>0FH", 0x18, 0x0F);
    TEST("LD  R2,R0",   0x28, R(0));
    TEST("LD  R3,10H",  0x38, 0x10);
    TEST("LD  R4,49H",  0x48, 0x49);
    TEST("LD  R5,59H",  0x58, 0x59);
    TEST("LD  R6,69H",  0x68, 0x69);
    TEST("LD  R7,79H",  0x78, 0x79);
    TEST("LD  R8,89H",  0x88, 0x89);
    TEST("LD  R9,99H",  0x98, 0x99);
    TEST("LD R10,0A9H", 0xA8, 0xA9);
    TEST("LD R11,0B9H", 0xB8, 0xB9);
    TEST("LD R12,0C9H", 0xC8, 0xC9);
    TEST("LD R13,0D9H", 0xD8, 0xD9);
    TEST("LD R14,0E9H", 0xE8, 0xE9);
    TEST("LD R15,0F9H", 0xF8, 0xF9);

    TEST("LD >0AH, R0", 0x09, 0x0A);
    TEST("LD >0FH, R1", 0x19, 0x0F);
    TEST("LD >00H, R2", 0x29, 0x00);
    TEST("LD  10H, R3", 0x39, 0x10);
    TEST("LD  4AH, R4", 0x49, 0x4A);
    TEST("LD  5AH, R5", 0x59, 0x5A);
    TEST("LD  6AH, R6", 0x69, 0x6A);
    TEST("LD  7AH, R7", 0x79, 0x7A);
    TEST("LD 0FFH, R8", 0x89, 0xFF);
    TEST("LD  9AH, R9", 0x99, 0x9A);
    TEST("LD 0AAH,R10", 0xA9, 0xAA);
    TEST("LD 0BAH,R11", 0xB9, 0xBA);
    TEST("LD 0CAH,R12", 0xC9, 0xCA);
    TEST("LD 0DAH,R13", 0xD9, 0xDA);
    TEST("LD 0EAH,R14", 0xE9, 0xEA);
    TEST("LD 0FAH,R15", 0xF9, 0xFA);

    TEST("LD  R0,#13",   0x0C, 0x0D);
    TEST("LD R1,#'%%'",  0x1C, 0x25);
    TEST("LD R1,#'%q'",  0x1C, 0x27);
    TEST("LD R1,#'%l'",  0x1C, 0x0A);
    TEST("LD R1,#'%r'",  0x1C, 0x0D);
    TEST("LD R1,#'%t'",  0x1C, 0x09);
    TEST("LD R1,#'%p'",  0x1C, 0x0C);
    TEST("LD R1,#'%1B'", 0x1C, 0x1B);
    TEST("LD  R2,#15",   0x2C, 0x0F);
    TEST("LD  R3,#10H",  0x3C, 0x10);
    TEST("LD  R4,#4DH",  0x4C, 0x4D);
    TEST("LD  R5,#5DH",  0x5C, 0x5D);
    TEST("LD  R6,#6DH",  0x6C, 0x6D);
    TEST("LD  R7,#7DH",  0x7C, 0x7D);
    TEST("LD  R8,#8DH",  0x8C, 0x8D);
    TEST("LD  R9,#9DH",  0x9C, 0x9D);
    TEST("LD R10,#0ADH", 0xAC, 0xAD);
    TEST("LD R11,#0BDH", 0xBC, 0xBD);
    TEST("LD R12,#0CDH", 0xCC, 0xCD);
    TEST("LD R13,#0DDH", 0xDC, 0xDD);
    TEST("LD R14,#0EDH", 0xEC, 0xED);
    TEST("LD R15,#0FDH", 0xFC, 0xFD);
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
    TEST("DEC >01H", 0x00, 0x01);
    ERRT("DEC -1",   OPERAND_NOT_ALLOWED, "-1");
    ERRT("DEC 256",  OPERAND_NOT_ALLOWED, "256");
    TEST("DEC R1",   0x00, R(1));
    TEST("DEC @15H", 0x01, 0x15);
    TEST("DEC @R2",  0x01, R(2));

    TEST("RLC 11H",  0x10, 0x11);
    TEST("RLC R1",   0x10, R(1));
    TEST("RLC @12H", 0x11, 0x12);
    TEST("RLC @R2",  0x11, R(2));

    TEST("INC 21H",  0x20, 0x21);
    TEST("INC R1",   0x1E);
    TEST("INC @22H", 0x21, 0x22);
    TEST("INC @R2",  0x21, R(2));

    TEST("DA 41H",  0x40, 0x41);
    TEST("DA R1",   0x40, R(1));
    TEST("DA @42H", 0x41, 0x42);
    TEST("DA @R2",  0x41, R(2));

    TEST("POP 0FCH", 0x50, 0xFC);
    TEST("POP R1",   0x50, R(1));
    TEST("POP @52H", 0x51, 0x52);
    TEST("POP @R2",  0x51, R(2));

    TEST("COM 61H",  0x60, 0x61);
    TEST("COM R1",   0x60, R(1));
    TEST("COM @62H", 0x61, 0x62);
    TEST("COM @R2",  0x61, R(2));

    TEST("PUSH 0FFH", 0x70, 0xFF);
    TEST("PUSH R1",   0x70, R(1));
    TEST("PUSH @72H", 0x71, 0x72);
    TEST("PUSH @R2",  0x71, R(2));

    TEST("DECW 82H",  0x80, 0x82);
    TEST("decw rr2",  0x80, R(2));
    ERRT("DECW 81H",  OPERAND_NOT_ALLOWED, "81H");
    ERRT("DECW RR1",  ILLEGAL_REGISTER, "RR1");
    TEST("DECW @81H", 0x81, 0x81);
    TEST("DECW @R1",  0x81, R(1));

    TEST("RL 91H",  0x90, 0x91);
    TEST("RL R1",   0x90, R(1));
    TEST("RL @92H", 0x91, 0x92);
    TEST("RL @R2",  0x91, R(2));

    TEST("INCW 0A2H",  0xA0, 0xA2);
    TEST("INCW RR2",   0xA0, R(2));
    ERRT("INCW 0A1H",  OPERAND_NOT_ALLOWED, "0A1H");
    ERRT("INCW RR1",   ILLEGAL_REGISTER, "RR1");
    TEST("INCW @0A1H", 0xA1, 0xA1);
    TEST("INCW @R1",   0xA1, R(1));

    TEST("CLR 0B1H",  0xB0, 0xB1);
    TEST("CLR R1",    0xB0, R(1));
    TEST("CLR @0B2H", 0xB1, 0xB2);
    TEST("CLR @R2",   0xB1, R(2));

    TEST("RRC 0C1H",  0xC0, 0xC1);
    TEST("RRC R1",    0xC0, R(1));
    TEST("RRC @0C2H", 0xC1, 0xC2);
    TEST("RRC @R2",   0xC1, R(2));

    TEST("SRA 0D1H",  0xD0, 0xD1);
    TEST("SRA R1",    0xD0, R(1));
    TEST("SRA @0D2H", 0xD1, 0xD2);
    TEST("SRA @R2",   0xD1, R(2));

    TEST("RR 0C1H",  0xE0, 0xC1);
    TEST("RR R1",    0xE0, R(1));
    TEST("RR @0C2H", 0xE1, 0xC2);
    TEST("RR @R2",   0xE1, R(2));

    TEST("SWAP 0CFH",  0xF0, 0xCF);
    TEST("SWAP R1",    0xF0, R(1));
    TEST("SWAP @0CFH", 0xF1, 0xCF);
    TEST("SWAP @R2",   0xF1, R(2));

    TEST("JP @32H",    0x30, 0x32);
    TEST("JP @RR2",    0x30, R(2));
    TZ86("CALL @0D6H", 0xD4, 0xD6);
    TZ88("CALL @0D6H", 0xF4, 0xD6);
    TZ86("CALL @RR2",  0xD4, R(2));
    TZ88("CALL @RR2",  0xF4, R(2));
    EZ86("CALL #0D6H", OPERAND_NOT_ALLOWED, "#0D6H");
    TZ88("CALL #0D6H", 0xD4, 0xD6);
    TEST("SRP  #30H",  0x31, 0x30);
    ERRT("SRP  #38H",  OPERAND_NOT_ALLOWED, "#38H", 0x31, 0x30);
    EZ86("SRP0 #30H",  UNKNOWN_INSTRUCTION, "SRP0 #30H");
    TZ88("SRP0 #30H",  0x31, 0x32);
    EZ86("SRP1 #38H",  UNKNOWN_INSTRUCTION, "SRP1 #38H");
    TZ88("SRP1 #38H",  0x31, 0x39);
    if (z86()) {
        ERRT("SRP #100H",  OVERFLOW_RANGE, "#100H", 0x31, 0x00);
    } else {
        ERRT("SRP #100H",  OVERFLOW_RANGE, "#100H", 0x31, 0x00);
    }
}

static void test_two_operands() {
    TEST("ADD R0,R3",     0x02, 0x03);
    TEST("ADD R4,@R5",    0x03, 0x45);
    TEST("ADD 78H,56H",   0x04, 0x56, 0x78);
    TEST("ADD R8,56H",    0x04, 0x56, R(8));
    TEST("ADD 78H,R6",    0x04, R(6), 0x78);
    TEST("ADD >07H,@16H", 0x05, 0x16, 0x07);
    TEST("ADD >07H,@R6",  0x05, R(6), 0x07);
    TEST("ADD R7,@16H",   0x05, 0x16, R(7));
    TEST("ADD >07H,#8",   0x06, 0x07, 0x08);
    TEST("ADD R7,#8",     0x06, R(7), 0x08);
    TZ86("ADD @18H,#9",   0x07, 0x18, 0x09);
    EZ88("ADD @18H,#9",   OPERAND_NOT_ALLOWED, "@18H,#9");
    TZ86("ADD @R8,#9",    0x07, R(8), 0x09);
    EZ88("ADD @R8,#9",    OPERAND_NOT_ALLOWED, "@R8,#9");
    TEST("ADD R7,#-128",  0x06, R(7), 0x80);
    TEST("ADD R7,#-1",    0x06, R(7), 0xFF);
    TEST("ADD R7,#%FF",   0x06, R(7), 0xFF);
    ERRT("ADD R7,#256",   OVERFLOW_RANGE, "#256",  0x06, R(7), 0x00);
    ERRT("ADD R7,#-129",  OVERFLOW_RANGE, "#-129", 0x06, R(7), 0x7F);

    TEST("ADC R3,R4",     0x12, 0x34);
    TEST("ADC R1,@R4",    0x13, 0x14);
    TEST("ADC 16H,15H",   0x14, 0x15, 0x16);
    TEST("ADC R6,15H",    0x14, 0x15, R(6));
    TEST("ADC 16H,R5",    0x14, R(5), 0x16);
    TEST("ADC 17H,@16H",  0x15, 0x16, 0x17);
    TEST("ADC 17H,@R6",   0x15, R(6), 0x17);
    TEST("ADC R7,@16H",   0x15, 0x16, R(7));
    TEST("ADC 17H,#18H",  0x16, 0x17, 0x18);
    TEST("ADC R7,#18H",   0x16, R(7), 0x18);
    TZ86("ADC @18H,#19H", 0x17, 0x18, 0x19);
    EZ88("ADC @18H,#19H", OPERAND_NOT_ALLOWED, "@18H,#19H");
    TZ86("ADC @R8,#19H",  0x17, R(8), 0x19);
    EZ88("ADC @R8,#19H",  OPERAND_NOT_ALLOWED, "@R8,#19H");

    TEST("SUB R2,R3",     0x22, 0x23);
    TEST("SUB R4,@R5",    0x23, 0x45);
    TEST("SUB 26H,25H",   0x24, 0x25, 0x26);
    TEST("SUB R6,25H",    0x24, 0x25, R(6));
    TEST("SUB 26H,R5",    0x24, R(5), 0x26);
    TEST("SUB 27H,@26H",  0x25, 0x26, 0x27);
    TEST("SUB 27H,@R6",   0x25, R(6), 0x27);
    TEST("SUB R7,@26H",   0x25, 0x26, R(7));
    TEST("SUB 27H,#28H",  0x26, 0x27, 0x28);
    TEST("SUB R7,#28H",   0x26, R(7), 0x28);
    TZ86("SUB @28H,#29H", 0x27, 0x28, 0x29);
    EZ88("SUB @28H,#29H", OPERAND_NOT_ALLOWED, "@28H,#29H");
    TZ86("SUB @R8,#29H",  0x27, R(8), 0x29);
    EZ88("SUB @R8,#29H",  OPERAND_NOT_ALLOWED, "@R8,#29H");

    TEST("SBC R3,R3",     0x32, 0x33);
    TEST("SBC R3,@R4",    0x33, 0x34);
    TEST("SBC 78H,56H",   0x34, 0x56, 0x78);
    TEST("SBC R8,56H",    0x34, 0x56, R(8));
    TEST("SBC 78H,R6",    0x34, R(6), 0x78);
    TEST("SBC 37H,@36H",  0x35, 0x36, 0x37);
    TEST("SBC 37H,@R6",   0x35, R(6), 0x37);
    TEST("SBC R7,@36H",   0x35, 0x36, R(7));
    TEST("SBC 37H,#38H",  0x36, 0x37, 0x38);
    TEST("SBC R7,#38H",   0x36, R(7), 0x38);
    TZ86("SBC @38H,#39H", 0x37, 0x38, 0x39);
    EZ88("SBC @38H,#39H", OPERAND_NOT_ALLOWED, "@38H,#39H");
    TZ86("SBC @R8,#39H",  0x37, R(8), 0x39);
    EZ88("SBC @R8,#39H",  OPERAND_NOT_ALLOWED, "@R8,#39H");

    TEST("OR R4,R3",     0x42, 0x43);
    TEST("OR R4,@R4",    0x43, 0x44);
    TEST("OR 46H,45H",   0x44, 0x45, 0x46);
    TEST("OR R6,45H",    0x44, 0x45, R(6));
    TEST("OR 46H,R5",    0x44, R(5), 0x46);
    TEST("OR 89H,@67H",  0x45, 0x67, 0x89);
    TEST("OR 89H,@R7",   0x45, R(7), 0x89);
    TEST("OR R9,@67H",   0x45, 0x67, R(9));
    TEST("OR 47H,#48H",  0x46, 0x47, 0x48);
    TEST("OR R7,#48H",   0x46, R(7), 0x48);
    TZ86("OR @48H,#49H", 0x47, 0x48, 0x49);
    EZ88("OR @48H,#49H", OPERAND_NOT_ALLOWED, "@48H,#49H");
    TZ86("OR @R8,#49H",  0x47, R(8), 0x49);
    EZ88("OR @R8,#49H",  OPERAND_NOT_ALLOWED, "@R8,#49H");

    TEST("AND R5,R3",     0x52, 0x53);
    TEST("AND R5,@R4",    0x53, 0x54);
    TEST("AND 56H,55H",   0x54, 0x55, 0x56);
    TEST("AND R6,55H",    0x54, 0x55, R(6));
    TEST("AND 56H,R5",    0x54, R(5), 0x56);
    TEST("AND 57H,@56H",  0x55, 0x56, 0x57);
    TEST("AND 57H,@R6",   0x55, R(6), 0x57);
    TEST("AND R7,@56H",   0x55, 0x56, R(7));
    TEST("AND 78H,#9AH",  0x56, 0x78, 0x9A);
    TEST("AND R8,#9AH",   0x56, R(8), 0x9A);
    TZ86("AND @58H,#59H", 0x57, 0x58, 0x59);
    EZ88("AND @58H,#59H", OPERAND_NOT_ALLOWED, "@58H,#59H");
    TZ86("AND @R8,#59H",  0x57, R(8), 0x59);
    EZ88("AND @R8,#59H",  OPERAND_NOT_ALLOWED, "@R8,#59H");

    TEST("TCM R6,R3",      0x62, 0x63);
    TEST("TCM R6,@R4",     0x63, 0x64);
    TEST("TCM 66H,65H",    0x64, 0x65, 0x66);
    TEST("TCM R6,65H",     0x64, 0x65, R(6));
    TEST("TCM 66H,R5",     0x64, R(5), 0x66);
    TEST("TCM 67H,@66H",   0x65, 0x66, 0x67);
    TEST("TCM 67H,@R6",    0x65, R(6), 0x67);
    TEST("TCM R7,@66H",    0x65, 0x66, R(7));
    TEST("TCM 67H,#68H",   0x66, 0x67, 0x68);
    TEST("TCM R7,#68H",    0x66, R(7), 0x68);
    TZ86("TCM @89H,#0ABH", 0x67, 0x89, 0xAB);
    EZ88("TCM @89H,#0ABH", OPERAND_NOT_ALLOWED, "@89H,#0ABH");
    TZ86("TCM @R9,#0ABH",  0x67, R(9), 0xAB);
    EZ88("TCM @R9,#0ABH",  OPERAND_NOT_ALLOWED, "@R9,#0ABH");

    TEST("TM R7,R3",     0x72, 0x73);
    TEST("TM R7,@R4",    0x73, 0x74);
    TEST("TM 76H,75H",   0x74, 0x75, 0x76);
    TEST("TM R6,75H",    0x74, 0x75, R(6));
    TEST("TM 76H,R5",    0x74, R(5), 0x76);
    TEST("TM 77H,@76H",  0x75, 0x76, 0x77);
    TEST("TM 77H,@R6",   0x75, R(6), 0x77);
    TEST("TM R7,@76H",   0x75, 0x76, R(7));
    TEST("TM 77H,#78H",  0x76, 0x77, 0x78);
    TEST("TM R7,#78H",   0x76, R(7), 0x78);
    TZ86("TM @78H,#79H", 0x77, 0x78, 0x79);
    EZ88("TM @78H,#79H", OPERAND_NOT_ALLOWED, "@78H,#79H");
    TZ86("TM @R8,#79H",  0x77, R(8), 0x79);
    EZ88("TM @R8,#79H",  OPERAND_NOT_ALLOWED, "@R8,#79H");

    TEST("CP R10,R3",      0xA2, 0xA3);
    TEST("CP R10,@R4",     0xA3, 0xA4);
    TEST("CP 0A6H,0A5H",   0xA4, 0xA5, 0xA6);
    TEST("CP R6,0A5H",     0xA4, 0xA5, R(6));
    TEST("CP 0A6H,R5",     0xA4, R(5), 0xA6);
    TEST("CP 0A7H,@0A6H",  0xA5, 0xA6, 0xA7);
    TEST("CP 0A7H,@R6",    0xA5, R(6), 0xA7);
    TEST("CP R7,@0A6H",    0xA5, 0xA6, R(7));
    TEST("CP 0A7H,#0A8H",  0xA6, 0xA7, 0xA8);
    TEST("CP R7,#0A8H",    0xA6, R(7), 0xA8);
    TZ86("CP @0A8H,#0A9H", 0xA7, 0xA8, 0xA9);
    EZ88("CP @0A8H,#0A9H", OPERAND_NOT_ALLOWED, "@0A8H,#0A9H");
    TZ86("CP @R8,#0A9H",   0xA7, R(8), 0xA9);
    EZ88("CP @R8,#0A9H",   OPERAND_NOT_ALLOWED, "@R8,#0A9H");

    TEST("XOR R11,R3",      0xB2, 0xB3);
    TEST("XOR R11,@R4",     0xB3, 0xB4);
    TEST("XOR 0B6H,0B5H",   0xB4, 0xB5, 0xB6);
    TEST("XOR R6,0B5H",     0xB4, 0xB5, R(6));
    TEST("XOR 0B6H,R5",     0xB4, R(5), 0xB6);
    TEST("XOR 0B7H,@0B6H",  0xB5, 0xB6, 0xB7);
    TEST("XOR 0B7H,@R6",    0xB5, R(6), 0xB7);
    TEST("XOR R7,@0B6H",    0xB5, 0xB6, R(7));
    TEST("XOR 0B7H,#0B8H",  0xB6, 0xB7, 0xB8);
    TEST("XOR R7,#0B8H",    0xB6, R(7), 0xB8);
    TZ86("XOR @0B8H,#0B9H", 0xB7, 0xB8, 0xB9);
    EZ88("XOR @0B8H,#0B9H", OPERAND_NOT_ALLOWED, "@0B8H,#0B9H");
    TZ86("XOR @R8,#0B9H",   0xB7, R(8), 0xB9);
    EZ88("XOR @R8,#0B9H",   OPERAND_NOT_ALLOWED, "@R8,#0B9H");

    TZ88("DIV  96H,95H",  0x94, 0x95, 0x96);
    TZ88("DIV  RR6,R5",   0x94, R(5), R(6));
    TZ88("DIV  98H,@96H", 0x95, 0x96, 0x98);
    TZ88("DIV  RR8,@R6",  0x95, R(6), R(8));
    TZ88("DIV  98H,#97H", 0x96, 0x97, 0x98);
    TZ88("DIV  RR8,#97H", 0x96, 0x97, R(8));
    TZ88("MULT 86H,85H",  0x84, 0x85, 0x86);
    TZ88("MULT RR6,R5",   0x84, R(5), R(6));
    TZ88("MULT 88H,@86H", 0x85, 0x86, 0x88);
    TZ88("MULT RR8,@R6",  0x85, R(6), R(8));
    TZ88("MULT 88H,#87H", 0x86, 0x87, 0x88);
    TZ88("MULT RR8,#87H", 0x86, 0x87, R(8));
    EZ86("DIV  96H,95H",  UNKNOWN_INSTRUCTION, "DIV  96H,95H");
    EZ86("MULT 96H,95H",  UNKNOWN_INSTRUCTION, "MULT 96H,95H");

    TZ86("LD R14,@R4",    0xE3, 0xE4);
    TZ88("LD R14,@R4",    0xC7, 0xE4);
    TEST("LD 36H,35H",    0xE4, 0x35, 0x36);
    TEST("LD 37H,@36H",   0xE5, 0x36, 0x37);
    TEST("LD 37H,#38H",   0xE6, 0x37, 0x38);
    TZ86("LD @38H,#39H",  0xE7, 0x38, 0x39);
    TZ88("LD @38H,#39H",  0xD6, 0x38, 0x39);
    TEST("LD R6,R5",      0x68, R(5));
    TZ86("LD R7,@R6",     0xE3, 0x76);
    TZ88("LD R7,@R6",     0xC7, 0x76);
    TEST("LD R7,#0E8H",   0x7C, 0xE8);
    TZ86("LD @R8,#0E9H",  0xE7, R(8), 0xE9);
    TZ88("LD @R8,#0E9H",  0xD6, R(8), 0xE9);
    TZ86("LD @R15,R4",    0xF3, 0xF4);
    TZ88("LD @R15,R4",    0xD7, 0xF4);
    TEST("LD @0C7H,0F0H", 0xF5, 0xF0, 0xC7);
    TEST("LD @R7,0F0H",   0xF5, 0xF0, R(7));

    TZ88("LDW 34H,12H",    0xC4, 0x12, 0x34);
    TZ88("LDW 34H,@12H",   0xC5, 0x12, 0x34);
    TZ88("LDW 12H,#3456H", 0xC6, 0x12, 0x34, 0x56);
    TZ88("LDW RR4,RR2",    0xC4, R(2), R(4));
    TZ88("LDW RR4,@R2",    0xC5, R(2), R(4));
    TZ88("LDW RR2,#3456H", 0xC6, R(2), 0x34, 0x56);
    EZ86("LDW 34H,12H",    UNKNOWN_INSTRUCTION, "LDW 34H,12H");

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

    TZ88("LDC R10,0A2A1H", 0xA7, 0xA0, 0xA1, 0xA2);
    TZ88("LDE R10,0A3A2H", 0xA7, 0xA1, 0xA2, 0xA3);
    TZ88("LDC 0B2B1H,R11", 0xB7, 0xB0, 0xB1, 0xB2);
    TZ88("LDE 0B3B2H,R11", 0xB7, 0xB1, 0xB2, 0xB3);
    TZ88("LDCD R15,@RR2",  0xE2, 0xF2);
    TZ88("LDED R15,@RR2",  0xE2, 0xF3);
    TZ88("LDCI R14,@RR4",  0xE3, 0xE4);
    TZ88("LDEI R14,@RR4",  0xE3, 0xE5);
    TZ88("LDCPD @RR2,R15", 0xF2, 0xF2);
    TZ88("LDEPD @RR2,R15", 0xF2, 0xF3);
    TZ88("LDCPI @RR4,R15", 0xF3, 0xF4);
    TZ88("LDEPI @RR4,R15", 0xF3, 0xF5);

    EZ86("LDC R10,0A2A1H", OPERAND_NOT_ALLOWED, "R10,0A2A1H");
    EZ86("LDE R10,0A3A2H", OPERAND_NOT_ALLOWED, "R10,0A3A2H");
    EZ86("LDC 0B2B1H,R11", OPERAND_NOT_ALLOWED, "0B2B1H,R11");
    EZ86("LDE 0B3B2H,R11", OPERAND_NOT_ALLOWED, "0B3B2H,R11");
    EZ86("LDCD R15,@RR2",  UNKNOWN_INSTRUCTION, "LDCD R15,@RR2");
    EZ86("LDED R15,@RR2",  UNKNOWN_INSTRUCTION, "LDED R15,@RR2");
    EZ86("LDCI R14,@RR4",  OPERAND_NOT_ALLOWED, "R14,@RR4");
    EZ86("LDEI R14,@RR4",  OPERAND_NOT_ALLOWED, "R14,@RR4");
    EZ86("LDCPD @RR2,R15", UNKNOWN_INSTRUCTION, "LDCPD @RR2,R15");
    EZ86("LDEPD @RR2,R15", UNKNOWN_INSTRUCTION, "LDEPD @RR2,R15");
    EZ86("LDCPI @RR4,R15", UNKNOWN_INSTRUCTION, "LDCPI @RR4,R15");
    EZ86("LDEPI @RR4,R15", UNKNOWN_INSTRUCTION, "LDEPI @RR4,R15");

    TZ88("PUSHUD @23H,45H", 0x82, 0x23, 0x45);
    TZ88("PUSHUI @23H,45H", 0x83, 0x23, 0x45);
    TZ88("PUSHUD @R2, R3",  0x82, 0xC2, 0xC3);
    TZ88("PUSHUI @R2, R3",  0x83, 0xC2, 0xC3);
    TZ88("POPUD  23H,@45H", 0x92, 0x45, 0x23);
    TZ88("POPUI  23H,@45H", 0x93, 0x45, 0x23);
    TZ88("POPUD  R2, @R3",  0x92, 0xC3, 0xC2);
    TZ88("POPUI  R2, @R3",  0x93, 0xC3, 0xC2);
}

static void test_indexed() {
    TZ86("LD R12,0C9H(R8)",  0xC7, 0xC8, 0xC9);
    TZ88("LD R12,0C9H(R8)",  0x87, 0xC8, 0xC9);
    TZ86("LD 0D9H(R8),R13",  0xD7, 0xD8, 0xD9);
    TZ88("LD 0D9H(R8),R13",  0x97, 0xD8, 0xD9);
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

    TZ88("LDC R14, -129(RR8)",  0xA7, 0xE8, 0x7F, 0xFF);
    TZ88("LDC R14, -128(RR8)",  0xE7, 0xE8, 0x80);
    TZ88("LDC R14,   -1(RR8)",  0xE7, 0xE8, 0xFF);
    TZ88("LDC R14,0FFFFH(RR8)", 0xA7, 0xE8, 0xFF, 0xFF);
    TZ88("LDC R14, +127(RR8)",  0xE7, 0xE8, 0x7F);
    TZ88("LDC R14, +128(RR8)",  0xA7, 0xE8, 0x80, 0x00);
    TZ88("LDC -129(RR8), R15",  0xB7, 0xF8, 0x7F, 0xFF);
    TZ88("LDC -128(RR8), R15",  0xF7, 0xF8, 0x80);
    TZ88("LDC   -1(RR8), R15",  0xF7, 0xF8, 0xFF);
    TZ88("LDC 0FFFFH(RR8),R15", 0xB7, 0xF8, 0xFF, 0xFF);
    TZ88("LDC +127(RR8), R15",  0xF7, 0xF8, 0x7F);
    TZ88("LDC +128(RR8), R15",  0xB7, 0xF8, 0x80, 0x00);
    TZ88("LDE R14, -129(RR8)",  0xA7, 0xE9, 0x7F, 0xFF);
    TZ88("LDE R14, -128(RR8)",  0xE7, 0xE9, 0x80);
    TZ88("LDE R14,   -1(RR8)",  0xE7, 0xE9, 0xFF);
    TZ88("LDE R14,0FFFFH(RR8)", 0xA7, 0xE9, 0xFF, 0xFF);
    TZ88("LDE R14, +127(RR8)",  0xE7, 0xE9, 0x7F);
    TZ88("LDE R14, +128(RR8)",  0xA7, 0xE9, 0x80, 0x00);
    TZ88("LDE -129(RR8), R15",  0xB7, 0xF9, 0x7F, 0xFF);
    TZ88("LDE -128(RR8), R15",  0xF7, 0xF9, 0x80);
    TZ88("LDE   -1(RR8), R15",  0xF7, 0xF9, 0xFF);
    TZ88("LDE 0FFFFH(RR8),R15", 0xB7, 0xF9, 0xFF, 0xFF);
    TZ88("LDE +127(RR8), R15",  0xF7, 0xF9, 0x7F);
    TZ88("LDE +128(RR8), R15",  0xB7, 0xF9, 0x80, 0x00);

    EZ86("LDC R14,-128(RR8)", OPERAND_NOT_ALLOWED, "R14,-128(RR8)");
    EZ86("LDC R14,+128(RR8)", OPERAND_NOT_ALLOWED, "R14,+128(RR8)");
    EZ86("LDE R14,-128(RR8)", OPERAND_NOT_ALLOWED, "R14,-128(RR8)");
    EZ86("LDE R14,+128(RR8)", OPERAND_NOT_ALLOWED, "R14,+128(RR8)");

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
}

static void test_bit_operation() {
    TEST("BITC R5,#4",       0x57, 0x58);
    TEST("BITR R7,#4",       0x77, 0x78);
    TEST("BITS R8,#4",       0x77, 0x89);

    TEST("BOR  R0,>09H,#4",  0x07, 0x08, 0x09);
    TEST("BOR  R0,R9,#4",    0x07, 0x08, R(9));
    TEST("BOR  0ABH,#4,R8",  0x07, 0x89, 0xAB);
    TEST("BOR  R11,#4,R8",   0x07, 0x89, 0xCB);
    TEST("BCP  R1,19H,#4",   0x17, 0x18, 0x19);
    TEST("BCP  R1,R9,#4",    0x17, 0x18, R(9));
    TEST("BXOR R2,29H,#4",   0x27, 0x28, 0x29);
    TEST("BXOR R2,R9,#4",    0x27, 0x28, R(9));
    TEST("BXOR 0ABH,#4,R8",  0x27, 0x89, 0xAB);
    TEST("BXOR R11,#4,R8",   0x27, 0x89, 0xCB);
    TEST("LDB  R4,49H,#4",   0x47, 0x48, 0x49);
    TEST("LDB  R4,R9,#4",    0x47, 0x48, R(9));
    TEST("LDB  0ABH,#4,R8",  0x47, 0x89, 0xAB);
    TEST("LDB  R11,#4,R8",   0x47, 0x89, 0xCB);
    TEST("BAND R7,89H,#4",   0x67, 0x78, 0x89);
    TEST("BAND R7,R9,#4",    0x67, 0x78, R(9));
    TEST("BAND 0ABH,#4,R8",  0x67, 0x89, 0xAB);
    TEST("BAND R11,#4,R8",   0x67, 0x89, 0xCB);

    TEST("BTJRF $,R3,#4", 0x37, 0x38, 0xFD);
    TEST("BTJRT $,R3,#4", 0x37, 0x39, 0xFD);
}

static void test_setrp() {
    TEST("SETRP 0");
    TEST("LD  21H,R4",      0x49, 0x21);
    TEST("LD   R4,21H",     0x48, 0x21);
    TEST("LD  21H,#66H",    0xE6, 0x21, 0x66);
    ERRT("LD  21H,33H(R4)", OPERAND_NOT_ALLOWED, "21H,33H(R4)"); // LD R,dd(r)
    ERRT("LD 33H(21H),R4",  UNKNOWN_OPERAND, "33H(21H),R4"); // LD dd(R),r
    TEST("LD  21H,24H",     0xE4, 0x24, 0x21);
    TEST("LD  24H,21H",     0XE4, 0x21, 0x24);
    TEST("LD  21H,@24H",    0xE5, 0x24, 0x21);
    TEST("LD @21H,24H",     0xF5, 0x24, 0x21);
    TEST("ADD 21H,24H",  0x04, 0x24, 0x21);
    TEST("ADD 21H,@24H", 0x05, 0x24, 0x21);
    TZ86("LDE  R1,@RR4", 0x82, 0x14);
    TZ88("LDE  R1,@RR4", 0xC3, 0x14 +1);
    TZ86("LDE  @RR4,R1", 0x92, 0x14);
    TZ88("LDE  @RR4,R1", 0xD3, 0x14 +1);
    TZ86("LDEI @R1,@RR4", 0x83, 0x14);
    EZ88("LDEI @R1,@RR4", OPERAND_NOT_ALLOWED, "@R1,@RR4");
    TZ86("LDEI @RR4,@R1", 0x93, 0x14);
    EZ88("LDEI @RR4,@R1", OPERAND_NOT_ALLOWED, "@RR4,@R1");
    TEST("DJNZ R1,$", 0x1A, 0xFE);
    TEST("JP   @24H", 0x30, 0x24);
    TZ86("CALL @24H", 0xD4, 0x24);
    TZ88("CALL @24H", 0xF4, 0x24);

    TEST("SETRP 21H & 0F0H");
    TEST("LD  21H,R4",   0x18, R(4));
    TEST("LD  R4,21H",   0x48, R(1));
    TEST("LD  21H,#66H", 0x1C, 0x66);
    TZ86("LD  21H,33H(R4)", 0xC7, 0x14, 0x33); // LD R1,33H(R4)
    TZ88("LD  21H,33H(R4)", 0x87, 0x14, 0x33); // LD R1,33H(R4)
    TZ86("LD  33H(R4),21H", 0xD7, 0x14, 0x33); // LD 33H(R4),R1
    TZ88("LD  33H(R4),21H", 0x97, 0x14, 0x33); // LD 33H(R4),R1
    TZ86("LD  R1,33H(24H)", 0xC7, 0x14, 0x33); // LD R1,33H(R4)
    TZ88("LD  R1,33H(24H)", 0x87, 0x14, 0x33); // LD R1,33H(R4)
    TZ86("LD  33H(24H),R1", 0xD7, 0x14, 0x33); // LD 33H(R4),R1
    TZ88("LD  33H(24H),R1", 0x97, 0x14, 0x33); // LD 33H(R4),R1
    TZ86("LD  21H,33H(24H)", 0xC7, 0x14, 0x33); // LD R1,33H(R4)
    TZ88("LD  21H,33H(24H)", 0x87, 0x14, 0x33); // LD R1,33H(R4)
    TZ86("LD  33H(24H),21H", 0xD7, 0x14, 0x33); // LD 33H(R4),R1
    TZ88("LD  33H(24H),21H", 0x97, 0x14, 0x33); // LD 33H(R4),R1
    TEST("LD  21H,24H",  0x18, R(4));
    TEST("LD  24H,21H",  0X48, R(1));
    TZ86("LD  21H,@24H", 0xE3, 0x14);
    TZ88("LD  21H,@24H", 0xC7, 0x14);
    TZ86("LD  @21H,24H", 0xF3, 0x14);
    TZ88("LD  @21H,24H", 0xD7, 0x14);
    TEST("ADD 21H,24H",  0x02, 0x14);
    TEST("ADD 21H,@24H", 0x03, 0x14);
    TZ86("LDE  21H,@24H",  0x82, 0x14);
    TZ88("LDE  21H,@24H",  0xC3, 0x14 +1);
    TZ86("LDE  @24H,21H",  0x92, 0x14);
    TZ88("LDE  @24H,21H",  0xD3, 0x14 +1);
    TZ86("LDEI @21H,@24H", 0x83, 0x14);
    EZ88("LDEI @21H,@24H", OPERAND_NOT_ALLOWED, "@21H,@24H");
    TZ86("LDEI @24H,@21H", 0x93, 0x14);
    EZ88("LDEI @24H,@21H", OPERAND_NOT_ALLOWED, "@24H,@21H");
    TEST("DJNZ 21H,$", 0x1A, 0xFE);
    TEST("JP   @24H",  0x30, R(4));
    TZ86("CALL @24H", 0xD4, R(4));
    TZ88("CALL @24H", 0xF4, R(4));

    TEST("SETRP -1");
    ERRT("SETRP 100H",  ILLEGAL_OPERAND,     "100H");

    EZ86("SETRP0 08H",  UNKNOWN_INSTRUCTION, "SETRP0 08H");
    TZ88("SETRP0 08H");
    EZ88("SETRP0 09H",  ILLEGAL_OPERAND,     "09H");
    TZ88("SETRP0 -1");
    EZ88("SETRP0 100H", ILLEGAL_OPERAND,     "100H");
    EZ86("SETRP1 18H",  UNKNOWN_INSTRUCTION, "SETRP1 18H");
    TZ88("SETRP1 18H");
    EZ88("SETRP1 1FH",  ILLEGAL_OPERAND,     "1FH");
    TZ88("SETRP1 -1");
    EZ88("SETRP1 118H", ILLEGAL_OPERAND,     "118H");

    TEST("SRP #10H",                              0x31, 0x10);
    ERRT("SRP #11H", OPERAND_NOT_ALLOWED, "#11H", 0x31, 0x10);
    ERRT("SRP #12H", OPERAND_NOT_ALLOWED, "#12H", 0x31, 0x10);
    ERRT("SRP #13H", OPERAND_NOT_ALLOWED, "#13H", 0x31, 0x10);
    ERRT("SRP #14H", OPERAND_NOT_ALLOWED, "#14H", 0x31, 0x10);
    ERRT("SRP #15H", OPERAND_NOT_ALLOWED, "#15H", 0x31, 0x10);
    ERRT("SRP #16H", OPERAND_NOT_ALLOWED, "#16H", 0x31, 0x10);
    ERRT("SRP #17H", OPERAND_NOT_ALLOWED, "#17H", 0x31, 0x10);
    ERRT("SRP #18H", OPERAND_NOT_ALLOWED, "#18H", 0x31, 0x10);
    ERRT("SRP #19H", OPERAND_NOT_ALLOWED, "#19H", 0x31, 0x10);
    ERRT("SRP #1AH", OPERAND_NOT_ALLOWED, "#1AH", 0x31, 0x10);
    ERRT("SRP #1BH", OPERAND_NOT_ALLOWED, "#1BH", 0x31, 0x10);
    ERRT("SRP #1CH", OPERAND_NOT_ALLOWED, "#1CH", 0x31, 0x10);
    ERRT("SRP #1DH", OPERAND_NOT_ALLOWED, "#1DH", 0x31, 0x10);
    ERRT("SRP #1EH", OPERAND_NOT_ALLOWED, "#1EH", 0x31, 0x10);
    ERRT("SRP #1FH", OPERAND_NOT_ALLOWED, "#1FH", 0x31, 0x10);
    TEST("SRP #20H",                              0x31, 0x20);

    TZ88("SRP0 #10H",                              0x31, 0x12);
    EZ88("SRP0 #11H", OPERAND_NOT_ALLOWED, "#11H", 0x31, 0x12);
    EZ88("SRP0 #12H", OPERAND_NOT_ALLOWED, "#12H", 0x31, 0x12);
    EZ88("SRP0 #13H", OPERAND_NOT_ALLOWED, "#13H", 0x31, 0x12);
    EZ88("SRP0 #14H", OPERAND_NOT_ALLOWED, "#14H", 0x31, 0x12);
    EZ88("SRP0 #15H", OPERAND_NOT_ALLOWED, "#15H", 0x31, 0x12);
    EZ88("SRP0 #16H", OPERAND_NOT_ALLOWED, "#16H", 0x31, 0x12);
    EZ88("SRP0 #17H", OPERAND_NOT_ALLOWED, "#17H", 0x31, 0x12);
    TZ88("SRP0 #18H",                              0x31, 0x1A);

    TZ88("SRP1 #10H",                              0x31, 0x11);
    EZ88("SRP1 #11H", OPERAND_NOT_ALLOWED, "#11H", 0x31, 0x11);
    EZ88("SRP1 #12H", OPERAND_NOT_ALLOWED, "#12H", 0x31, 0x11);
    EZ88("SRP1 #13H", OPERAND_NOT_ALLOWED, "#13H", 0x31, 0x11);
    EZ88("SRP1 #14H", OPERAND_NOT_ALLOWED, "#14H", 0x31, 0x11);
    EZ88("SRP1 #15H", OPERAND_NOT_ALLOWED, "#15H", 0x31, 0x11);
    EZ88("SRP1 #16H", OPERAND_NOT_ALLOWED, "#16H", 0x31, 0x11);
    EZ88("SRP1 #17H", OPERAND_NOT_ALLOWED, "#17H", 0x31, 0x11);
    TZ88("SRP1 #18H",                              0x31, 0x19);
}

static void test_comment() {
    TEST(" RET             ; comment", 0xAF);
    TEST(" JP  ULE , 3E3FH ; comment", 0x3D, 0x3E, 0x3F);
    TEST(" JP  8E8FH       ; comment", 0x8D, 0x8E, 0x8F);
    TEST(" LD  R1 , >0FH   ; comment", 0x18, 0x0F);
    TEST(" LD  R2 , R0     ; comment", 0x28, R(0));
    TEST(" LD  >0FH , R1   ; comment", 0x19, 0x0F);
    TEST(" LD  R9 , #9DH   ; comment", 0x9C, 0x9D);
    TEST(" INC R0          ; comment", 0x0E);
    TEST(" RLC 11H         ; comment", 0x10, 0x11);
    TEST(" RLC R1          ; comment", 0x10, R(1));
    TEST(" RLC @12H        ; comment", 0x11, 0x12);
    TEST(" RLC @R2         ; comment", 0x11, R(2));
    TEST(" ADD >07H, @16H  ; comment", 0x05, 0x16, 0x07);
    TEST(" ADD >07H, @R6   ; comment", 0x05, R(6), 0x07);
    TEST(" ADD R7 , @16H   ; comment", 0x05, 0x16, R(7));
    TZ86(" LDC @RR4 , R13  ; comment", 0xD2, 0xD4);
    TZ88(" LDC @RR4 , R13  ; comment", 0xD3, 0xD4);
    TZ86(" LD  R12 , 0C9H (R8) ; comment", 0xC7, 0xC8, 0xC9);
    TZ88(" LD  R12 , 0C9H (R8) ; comment", 0x87, 0xC8, 0xC9);
    TZ86(" LD  0D9H (R8) , R13 ; comment", 0xD7, 0xD8, 0xD9);
    TZ88(" LD  0D9H (R8) , R13 ; comment", 0x97, 0xD8, 0xD9);
    ATEST(0x1000, " JR   C , 107EH  ; comment", 0x7B, 0x7C);
    ATEST(0x1000, " DJNZ R2 , 102DH ; comment", 0x2A, 0x2B);
}

static void test_undefined_symbol() {
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
    TEST("LD  >0FH,R1", 0x19, 0x0F);
    ERRT("LD > 0FH,R1", UNKNOWN_OPERAND, "> 0FH,R1");
    TEST("DEC @>15H",   0x01, 0x15);
    ERRT("DEC @ >15H",  UNKNOWN_OPERAND, "@ >15H");
    ERRT("DEC @> 15H",  UNKNOWN_OPERAND, "@> 15H");
    EZ88("BITS R8,#8",     ILLEGAL_BIT_NUMBER, "#8",  0x77, 0x81);
    EZ88("BITS R8,#-1",    ILLEGAL_BIT_NUMBER, "#-1", 0x77, 0x8F);
    EZ88("BXOR R2,R8,#8",  ILLEGAL_BIT_NUMBER, "#8",  0x27, 0x20, 0xC8);
    EZ88("BXOR R2,R8,#-1", ILLEGAL_BIT_NUMBER, "#-1", 0x27, 0x2E, 0xC8);
}

static void test_data_constant() {
    TEST("DB -128, 255", 0x80, 0xFF);
    TEST("DB 'A', '\"'", 0x41, 0x22);
    TEST("DB '9'-'0'",   0x09);
    TEST("DB '%27'",     0x27);
    ERRT("DB '%2'",      UNKNOWN_ESCAPE_SEQUENCE, "%2'");
    TEST("DB '%q'",      0x27);
    ERRT("DB '''",       MISSING_CLOSING_QUOTE, "");
    TEST("DB 'A%22B',0", 0x41, 0x22, 0x42, 0x00);
    TEST("DB 'A%QB',0",  0x41, 0x27, 0x42, 0x00);
    ERRT("DB 'A%QB,0",   MISSING_CLOSING_QUOTE, "'A%QB,0");
    TEST("DW -128, 255", 0xFF, 0x80, 0x00, 0xFF);
    TEST("DW 'A%QB'",    0x41, 0x27, 0x42, 0x00);
    ERRT("DW 'A%QB",     MISSING_CLOSING_QUOTE, "'A%QB");
    TEST("DL 12345678H", 0x12, 0x34, 0x56, 0x78);
    TEST("DL 'A%QB%22C'", 0x41, 0x27, 0x42, 0x22, 0x43, 0x00, 0x00, 0x00);
    ERRT("DL 'A%QB%22C",  MISSING_CLOSING_QUOTE, "'A%QB%22C");
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
    RUN_TEST(test_undefined_symbol);
    RUN_TEST(test_error);
    RUN_TEST(test_data_constant);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

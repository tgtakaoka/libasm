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

#include "dis_z8.h"
#include "test_dis_helper.h"

using namespace libasm;
using namespace libasm::z8;
using namespace libasm::test;

DisZ8 disz8;
Disassembler &disassembler(disz8);

static bool z86c() {
    return strcmp_P("Z86C", disassembler.cpu_P()) == 0;
}

static bool z86() {
    return strcmp_P("Z8", disassembler.cpu_P()) == 0 || z86c();
}

static bool z88() {
    return strcmp_P("Z88", disassembler.cpu_P()) == 0;
}

static uint8_t R(uint8_t n) {
    if (z88())
        return 0xC0 + n;
    return 0xE0 + n;
}

static void set_up() {
    disassembler.reset();
    disassembler.setOption("relative", "disable");
}

static void tear_down() {
    symtab.reset();
}

// clang-format off
void test_cpu() {
    EQUALS("cpu z8", true, disassembler.setCpu("z8"));
    EQUALS_P("cpu z8", "Z8", disassembler.cpu_P());

    EQUALS("cpu Z8601", true, disassembler.setCpu("Z8601"));
    EQUALS_P("cpu Z8601", "Z8", disassembler.cpu_P());

    EQUALS("cpu Z86C91", true,   disassembler.setCpu("Z86C91"));
    EQUALS_P("cpu Z86C91", "Z86C", disassembler.cpu_P());

    EQUALS("cpu Super8", true,  disassembler.setCpu("Super8"));
    EQUALS_P("cpu Super8", "Z88", disassembler.cpu_P());

    EQUALS("cpu Z88C00", true,  disassembler.setCpu("Z88C00"));
    EQUALS_P("cpu Z88C00", "Z88", disassembler.cpu_P());
}

static void test_implied() {
    TEST("DI",   "", 0x8F);
    TEST("EI",   "", 0x9F);
    TEST("RET",  "", 0xAF);
    TEST("IRET", "", 0xBF);
    TEST("RCF",  "", 0xCF);
    TEST("SCF",  "", 0xDF);
    TEST("CCF",  "", 0xEF);
    TEST("NOP",  "", 0xFF);

    if (z86c()) {
        TEST("STOP", "", 0x6F);
        TEST("HALT", "", 0x7F);
    }
}

static void test_absolute() {
    symtab.intern(0x7E7F, "sym7E7F");
    symtab.intern(0xD7D8, "symD7D8");

    TEST("JP", "F, %0E0F",   0x0D, 0x0E, 0x0F);
    TEST("JP", "LT, %1E1F",  0x1D, 0x1E, 0x1F);
    TEST("JP", "LE, %2E2F",  0x2D, 0x2E, 0x2F);
    TEST("JP", "ULE, %3E3F", 0x3D, 0x3E, 0x3F);
    TEST("JP", "OV, %4E4F",  0x4D, 0x4E, 0x4F);
    TEST("JP", "MI, %5E5F",  0x5D, 0x5E, 0x5F);
    TEST("JP", "Z, %6E6F",   0x6D, 0x6E, 0x6F);
    TEST("JP", "C, sym7E7F", 0x7D, 0x7E, 0x7F);
    TEST("JP", "%8E8F",      0x8D, 0x8E, 0x8F);
    TEST("JP", "GE, %9E9F",  0x9D, 0x9E, 0x9F);
    TEST("JP", "GT, %AEAF",  0xAD, 0xAE, 0xAF);
    TEST("JP", "UGT, %BEBF", 0xBD, 0xBE, 0xBF);
    TEST("JP", "NOV, %CECF", 0xCD, 0xCE, 0xCF);
    TEST("JP", "PL, %DEDF",  0xDD, 0xDE, 0xDF);
    TEST("JP", "NZ, %EEEF",  0xED, 0xEE, 0xEF);
    TEST("JP", "NC, %FEFF",          0xFD, 0xFE, 0xFF);
    NMEM("JP", "NC, %FE00", "%FE00", 0xFD, 0xFE);
    NMEM("JP", "NC, %0000", "%0000", 0xFD);
    if (z86()) {
        TEST("CALL", "symD7D8",  0xD6, 0xD7, 0xD8);
    } else {
        TEST("CALL", "symD7D8",  0xF6, 0xD7, 0xD8);
    }
}

static void test_relative() {
    symtab.intern(0x106E, "sym106E");
    symtab.intern(0x0F8E, "sym0F8E");

    disassembler.setOption("relative", "no");
    ATEST(0x1000, "JR", "F, %100E",   0x0B, 0x0C);
    ATEST(0x1000, "JR", "LT, %101E",  0x1B, 0x1C);
    ATEST(0x1000, "JR", "LE, %102E",  0x2B, 0x2C);
    ATEST(0x1000, "JR", "ULE, %103E", 0x3B, 0x3C);
    ATEST(0x1000, "JR", "OV, %104E",  0x4B, 0x4C);
    ATEST(0x1000, "JR", "MI, %105E",  0x5B, 0x5C);
    ATEST(0x1000, "JR", "Z, sym106E", 0x6B, 0x6C);
    ATEST(0x1000, "JR", "C, %107E",   0x7B, 0x7C);
    ATEST(0x1000, "JR", "sym0F8E",    0x8B, 0x8C);
    ATEST(0x1000, "JR", "GE, %0F9E",  0x9B, 0x9C);
    ATEST(0x1000, "JR", "GT, %0FAE",  0xAB, 0xAC);
    ATEST(0x1000, "JR", "UGT, %0FBE", 0xBB, 0xBC);
    ATEST(0x1000, "JR", "NOV, %0FCE", 0xCB, 0xCC);
    ATEST(0x1000, "JR", "PL, %0FDE",  0xDB, 0xDC);
    ATEST(0x1000, "JR", "NZ, %0FEE",  0xEB, 0xEC);
    ATEST(0x1000, "JR", "NC, %0FFE",          0xFB, 0xFC);
    ANMEM(0x1000, "JR", "NC, %1001", "%1001", 0xFB);

    ATEST(0x1000, "DJNZ", "R0, %100D", 0x0A, 0x0B);
    ATEST(0x1000, "DJNZ", "R1, %101D", 0x1A, 0x1B);
    ATEST(0x1000, "DJNZ", "R2, %102D", 0x2A, 0x2B);
    ATEST(0x1000, "DJNZ", "R3, %103D", 0x3A, 0x3B);
    ATEST(0x1000, "DJNZ", "R4, %104D", 0x4A, 0x4B);
    ATEST(0x1000, "DJNZ", "R5, %105D", 0x5A, 0x5B);
    ATEST(0x1000, "DJNZ", "R6, %106D", 0x6A, 0x6B);
    ATEST(0x1000, "DJNZ", "R7, %107D", 0x7A, 0x7B);
    ATEST(0x1000, "DJNZ", "R8, %0F8D", 0x8A, 0x8B);
    ATEST(0x1000, "DJNZ", "R9, %0F9D", 0x9A, 0x9B);
    ATEST(0x1000, "DJNZ", "R10, %0FAD", 0xAA, 0xAB);
    ATEST(0x1000, "DJNZ", "R11, %0FBD", 0xBA, 0xBB);
    ATEST(0x1000, "DJNZ", "R12, %0FCD", 0xCA, 0xCB);
    ATEST(0x1000, "DJNZ", "R13, %0FDD", 0xDA, 0xDB);
    ATEST(0x1000, "DJNZ", "R14, %0FED", 0xEA, 0xEB);
    ATEST(0x1000, "DJNZ", "R15, %0FFD", 0xFA, 0xFB);

    disassembler.setOption("relative", "yes");
    ATEST(0x2000, "JR", "$-126", 0x8B, 0x80);
    ATEST(0x2000, "JR", "$",     0x8B, 0xFE);
    ATEST(0x2000, "JR", "$+2",   0x8B, 0x00);
    ATEST(0x2000, "JR", "$+129", 0x8B, 0x7F);
    ATEST(0x2000, "DJNZ", "R0, $-126", 0x0A, 0x80);
    ATEST(0x2000, "DJNZ", "R0, $",     0x0A, 0xFE);
    ATEST(0x2000, "DJNZ", "R0, $+2",   0x0A, 0x00);
    ATEST(0x2000, "DJNZ", "R0, $+129", 0x0A, 0x7F);
    if (z88()) {
        ATEST(0x1000, "CPIJE",  "R3, @R12, $-125", 0xC2, 0xC3, 0x80);
        ATEST(0x1000, "CPIJE",  "R3, @R12, $",     0xC2, 0xC3, 0xFD);
        ATEST(0x1000, "CPIJE",  "R3, @R12, $+3",   0xC2, 0xC3, 0x00);
        ATEST(0x1000, "CPIJE",  "R3, @R12, $+130", 0xC2, 0xC3, 0x7F);
        ATEST(0x1000, "CPIJNE", "R3, @R12, $",                  0xD2, 0xC3, 0xFD);
        ANMEM(0x1000, "CPIJNE", "R3, @R12, $+2",         "$+2", 0xD2, 0xC3);
        ANMEM(0x1000, "CPIJNE", "R0, @R0, $+1", "R0, @R0, $+1", 0xD2);
    }
}

static void test_operand_in_opcode() {
    TEST("LD", "R0, >%09", 0x08, 0x09);
    TEST("LD", "R1, >%0F", 0x18, 0x0F);
    TEST("LD", "R2, >%00", 0x28, 0x00);
    TEST("LD", "R3, %10",  0x38, 0x10);
    TEST("LD", "R4, %49",  0x48, 0x49);
    TEST("LD", "R5, %59",  0x58, 0x59);
    TEST("LD", "R6, %69",  0x68, 0x69);
    TEST("LD", "R7, %79",  0x78, 0x79);
    TEST("LD", "R8, %89",  0x88, 0x89);
    TEST("LD", "R9, %99",  0x98, 0x99);
    TEST("LD", "R10, %A9", 0xA8, 0xA9);
    TEST("LD", "R11, %B9", 0xB8, 0xB9);
    TEST("LD", "R12, %B9", 0xC8, 0xB9);
    TEST("LD", "R13, %D9", 0xD8, 0xD9);
    TEST("LD", "R14, %D9", 0xE8, 0xD9);
    TEST("LD", "R15, %F9",          0xF8, 0xF9);
    NMEM("LD", "R15, >%00", ">%00", 0xF8);

    TEST("LD", ">%0A, R0", 0x09, 0x0A);
    TEST("LD", ">%0F, R1", 0x19, 0x0F);
    TEST("LD", ">%00, R2", 0x29, 0x00);
    TEST("LD", "%10, R3",  0x39, 0x10);
    TEST("LD", "%4A, R4",  0x49, 0x4A);
    TEST("LD", "%5A, R5",  0x59, 0x5A);
    TEST("LD", "%6A, R6",  0x69, 0x6A);
    TEST("LD", "%7A, R7",  0x79, 0x7A);
    TEST("LD", "%FF, R8",  0x89, 0xFF);
    TEST("LD", "%9A, R9",  0x99, 0x9A);
    TEST("LD", "%AA, R10", 0xA9, 0xAA);
    TEST("LD", "%BA, R11", 0xB9, 0xBA);
    TEST("LD", "%BA, R12", 0xC9, 0xBA);
    TEST("LD", "%DA, R13", 0xD9, 0xDA);
    TEST("LD", "%DA, R14", 0xE9, 0xDA);
    TEST("LD", "%FA, R15", 0xF9, 0xFA);

    TEST("LD", "R0, #13",   0x0C, 0x0D);
    TEST("LD", "R1, #0",    0x1C, 0x00);
    TEST("LD", "R2, #15",   0x2C, 0x0F);
    TEST("LD", "R3, #16",   0x3C, 0x10);
    TEST("LD", "R4, #%4D",  0x4C, 0x4D);
    TEST("LD", "R5, #%5D",  0x5C, 0x5D);
    TEST("LD", "R6, #%6D",  0x6C, 0x6D);
    TEST("LD", "R7, #%7D",  0x7C, 0x7D);
    TEST("LD", "R8, #%8D",  0x8C, 0x8D);
    TEST("LD", "R9, #%9D",  0x9C, 0x9D);
    TEST("LD", "R10, #%AD", 0xAC, 0xAD);
    TEST("LD", "R11, #%BD", 0xBC, 0xBD);
    TEST("LD", "R12, #%CD", 0xCC, 0xCD);
    TEST("LD", "R13, #%DD", 0xDC, 0xDD);
    TEST("LD", "R14, #%ED", 0xEC, 0xED);
    TEST("LD", "R15, #%FD",    0xFC, 0xFD);
    NMEM("LD", "R15, #0", "0", 0xFC);

    TEST("INC", "R0",  0x0E);
    TEST("INC", "R1",  0x1E);
    TEST("INC", "R2",  0x2E);
    TEST("INC", "R3",  0x3E);
    TEST("INC", "R4",  0x4E);
    TEST("INC", "R5",  0x5E);
    TEST("INC", "R6",  0x6E);
    TEST("INC", "R7",  0x7E);
    TEST("INC", "R8",  0x8E);
    TEST("INC", "R9",  0x9E);
    TEST("INC", "R10", 0xAE);
    TEST("INC", "R11", 0xBE);
    TEST("INC", "R12", 0xCE);
    TEST("INC", "R13", 0xDE);
    TEST("INC", "R14", 0xEE);
    TEST("INC", "R15", 0xFE);
}

static void test_one_operand() {
    TEST("DEC",  ">%01", 0x00, 0x01);
    TEST("DEC",  "R1",   0x00, R(1));
    TEST("DEC",  "@%05",        0x01, 0x05);
    NMEM("DEC",  "@%00", "%00", 0x01);
    TEST("DEC",  "@R2",  0x01, R(2));

    TEST("RLC",  "%11",  0x10, 0x11);
    TEST("RLC",  "R1",   0x10, R(1));
    TEST("RLC",  "@%12", 0x11, 0x12);
    TEST("RLC",  "@R2",  0x11, R(2));

    TEST("INC",  "%21",  0x20, 0x21);
    TEST("INC",  "R1",   0x20, R(1));
    TEST("INC",  "@%22", 0x21, 0x22);
    TEST("INC",  "@R2",  0x21, R(2));

    TEST("DA",   "%41",  0x40, 0x41);
    TEST("DA",   "R1",   0x40, R(1));
    TEST("DA",   "@%42", 0x41, 0x42);
    TEST("DA",   "@R2",  0x41, R(2));

    TEST("POP",  "%FC",  0x50, 0xFC);
    TEST("POP",  "R1",   0x50, R(1));
    TEST("POP",  "@%52", 0x51, 0x52);
    TEST("POP",  "@R2",  0x51, R(2));

    TEST("COM",  "%61",  0x60, 0x61);
    TEST("COM",  "R1",   0x60, R(1));
    TEST("COM",  "@%62", 0x61, 0x62);
    TEST("COM",  "@R2",  0x61, R(2));

    TEST("PUSH", "%FF",  0x70, 0xFF);
    TEST("PUSH", "R1",   0x70, R(1));
    TEST("PUSH", "@%72", 0x71, 0x72);
    TEST("PUSH", "@R2",  0x71, R(2));

    TEST("DECW", "%82",  0x80, 0x82);
    TEST("DECW", "RR2",  0x80, R(2));
    TEST("DECW", "@%81", 0x81, 0x81);
    TEST("DECW", "@R1",  0x81, R(1));

    TEST("RL",   "%91",  0x90, 0x91);
    TEST("RL",   "R1",   0x90, R(1));
    TEST("RL",   "@%92", 0x91, 0x92);
    TEST("RL",   "@R2",  0x91, R(2));

    TEST("INCW", "%A2",  0xA0, 0xA2);
    TEST("INCW", "RR2",  0xA0, R(2));
    TEST("INCW", "@%A1", 0xA1, 0xA1);
    TEST("INCW", "@R1",  0xA1, R(1));

    TEST("CLR",  "%B1",  0xB0, 0xB1);
    TEST("CLR",  "R1",   0xB0, R(1));
    TEST("CLR",  "@%B2", 0xB1, 0xB2);
    TEST("CLR",  "@R2",  0xB1, R(2));

    TEST("RRC",  "%B1",  0xC0, 0xB1);
    TEST("RRC",  "R1",   0xC0, R(1));
    TEST("RRC",  "@%B2", 0xC1, 0xB2);
    TEST("RRC",  "@R2",  0xC1, R(2));

    TEST("SRA",  "%D1",  0xD0, 0xD1);
    TEST("SRA",  "R1",   0xD0, R(1));
    TEST("SRA",  "@%D2", 0xD1, 0xD2);
    TEST("SRA",  "@R2",  0xD1, R(2));

    TEST("RR",   "%B1",  0xE0, 0xB1);
    TEST("RR",   "R1",   0xE0, R(1));
    TEST("RR",   "@%B2", 0xE1, 0xB2);
    TEST("RR",   "@R2",  0xE1, R(2));

    TEST("SWAP", "%BF",  0xF0, 0xBF);
    TEST("SWAP", "R1",   0xF0, R(1));
    TEST("SWAP", "@%BF", 0xF1, 0xBF);
    TEST("SWAP", "@R2",  0xF1, R(2));

    TEST("JP",   "@%32", 0x30, 0x32);
    TEST("JP",   "@RR2", 0x30, R(2));
    if (z86()) {
        TEST("CALL", "@%D6", 0xD4, 0xD6);
        TEST("CALL", "@RR2", 0xD4, R(2));
    } else {
        TEST("CALL", "@%D6", 0xF4, 0xD6);
        TEST("CALL", "@RR2", 0xF4, R(2));
        TEST("CALL", "#%D6", 0xD4, 0xD6);
        NMEM("CALL", "#0", "0", 0xD4);
    }
}

static void test_two_operands() {
    TEST("ADD", "R0, R3",     0x02, 0x03);
    TEST("ADD", "R4, @R5",    0x03, 0x45);
    TEST("ADD", "%78, %56",   0x04, 0x56, 0x78);
    TEST("ADD", "R8, %56",    0x04, 0x56, R(8));
    TEST("ADD", "%78, R6",    0x04, R(6), 0x78);
    TEST("ADD", ">%07, @%06", 0x05, 0x06, 0x07);
    TEST("ADD", ">%07, @R6",  0x05, R(6), 0x07);
    TEST("ADD", "R7, @%06",   0x05, 0x06, R(7));
    TEST("ADD", ">%07, #8",              0x06, 0x07, 0x08);
    NMEM("ADD", ">%07, #0",        "0",  0x06, 0x07);
    NMEM("ADD", ">%00, #0", ">%00, #0", 0x06);
    TEST("ADD", "R7, #8",     0x06, R(7), 0x08);
    if (z86()) {
        TEST("ADD", "@%08, #9", 0x07, 0x08, 0x09);
        TEST("ADD", "@R8, #9",  0x07, R(8), 0x09);
    }

    TEST("ADC", "R3, R4",    0x12, 0x34);
    TEST("ADC", "R1, @R4",   0x13, 0x14);
    TEST("ADC", "%16, %15",  0x14, 0x15, 0x16);
    TEST("ADC", "R6, %15",   0x14, 0x15, R(6));
    TEST("ADC", "%16, R5",   0x14, R(5), 0x16);
    TEST("ADC", "%17, @%16", 0x15, 0x16, 0x17);
    TEST("ADC", "%17, @R6",  0x15, R(6), 0x17);
    TEST("ADC", "R7, @%16",  0x15, 0x16, R(7));
    TEST("ADC", "%17, #24",  0x16, 0x17, 0x18);
    TEST("ADC", "R7, #24",   0x16, R(7), 0x18);
    if (z86()) {
        TEST("ADC", "@%18, #25", 0x17, 0x18, 0x19);
        TEST("ADC", "@R8, #25",  0x17, R(8), 0x19);
    }

    TEST("SUB", "R2, R3",    0x22, 0x23);
    TEST("SUB", "R4, @R5",   0x23, 0x45);
    TEST("SUB", "%26, %25",  0x24, 0x25, 0x26);
    TEST("SUB", "R6, %25",   0x24, 0x25, R(6));
    TEST("SUB", "%26, R5",   0x24, R(5), 0x26);
    TEST("SUB", "%27, @%26", 0x25, 0x26, 0x27);
    TEST("SUB", "%27, @R6",  0x25, R(6), 0x27);
    TEST("SUB", "R7, @%26",  0x25, 0x26, R(7));
    TEST("SUB", "%27, #%28", 0x26, 0x27, 0x28);
    TEST("SUB", "R7, #%28",  0x26, R(7), 0x28);
    if (z86()) {
        TEST("SUB", "@%28, #%29", 0x27, 0x28, 0x29);
        TEST("SUB", "@R8, #%29",  0x27, R(8), 0x29);
    }

    TEST("SBC", "R3, R3",    0x32, 0x33);
    TEST("SBC", "R3, @R4",   0x33, 0x34);
    TEST("SBC", "%78, %56",  0x34, 0x56, 0x78);
    TEST("SBC", "R8, %56",   0x34, 0x56, R(8));
    TEST("SBC", "%78, R6",   0x34, R(6), 0x78);
    TEST("SBC", "%37, @%36", 0x35, 0x36, 0x37);
    TEST("SBC", "%37, @R6",  0x35, R(6), 0x37);
    TEST("SBC", "R7, @%36",  0x35, 0x36, R(7));
    TEST("SBC", "%37, #%38", 0x36, 0x37, 0x38);
    TEST("SBC", "R7, #%38",  0x36, R(7), 0x38);
    if (z86()) {
        TEST("SBC", "@%38, #%39", 0x37, 0x38, 0x39);
        TEST("SBC", "@R8, #%39",  0x37, R(8), 0x39);
    }

    TEST("OR",  "R4, R3",    0x42, 0x43);
    TEST("OR",  "R4, @R4",   0x43, 0x44);
    TEST("OR",  "%46, %45",  0x44, 0x45, 0x46);
    TEST("OR",  "R6, %45",   0x44, 0x45, R(6));
    TEST("OR",  "%46, R5",   0x44, R(5), 0x46);
    TEST("OR",  "%89, @%67", 0x45, 0x67, 0x89);
    TEST("OR",  "%89, @R7",  0x45, R(7), 0x89);
    TEST("OR",  "R9, @%67",  0x45, 0x67, R(9));
    TEST("OR",  "%47, #%48", 0x46, 0x47, 0x48);
    TEST("OR",  "R7, #%48",  0x46, R(7), 0x48);
    if (z86()) {
        TEST("OR",  "@%48, #%49", 0x47, 0x48, 0x49);
        TEST("OR",  "@R8, #%49",  0x47, R(8), 0x49);
    }

    TEST("AND", "R5, R3",    0x52, 0x53);
    TEST("AND", "R5, @R4",   0x53, 0x54);
    TEST("AND", "%56, %55",  0x54, 0x55, 0x56);
    TEST("AND", "R6, %55",   0x54, 0x55, R(6));
    TEST("AND", "%56, R5",   0x54, R(5), 0x56);
    TEST("AND", "%57, @%56", 0x55, 0x56, 0x57);
    TEST("AND", "%57, @R6",  0x55, R(6), 0x57);
    TEST("AND", "R7, @%56",  0x55, 0x56, R(7));
    TEST("AND", "%78, #%9A", 0x56, 0x78, 0x9A);
    TEST("AND", "R8, #%9A",  0x56, R(8), 0x9A);
    if (z86()) {
        TEST("AND", "@%58, #%59", 0x57, 0x58, 0x59);
        TEST("AND", "@R8, #%59",  0x57, R(8), 0x59);
    }

    TEST("TCM", "R6, R3",    0x62, 0x63);
    TEST("TCM", "R6, @R4",   0x63, 0x64);
    TEST("TCM", "%66, %65",  0x64, 0x65, 0x66);
    TEST("TCM", "R6, %65",   0x64, 0x65, R(6));
    TEST("TCM", "%66, R5",   0x64, R(5), 0x66);
    TEST("TCM", "%67, @%66", 0x65, 0x66, 0x67);
    TEST("TCM", "%67, @R6",  0x65, R(6), 0x67);
    TEST("TCM", "R7, @%66",  0x65, 0x66, R(7));
    TEST("TCM", "%67, #%68", 0x66, 0x67, 0x68);
    TEST("TCM", "R7, #%68",  0x66, R(7), 0x68);
    if (z86()) {
        TEST("TCM", "@%89, #%AB",0x67, 0x89, 0xAB);
        TEST("TCM", "@R9, #%AB", 0x67, R(9), 0xAB);
    }

    TEST("TM",  "R7, R3",    0x72, 0x73);
    TEST("TM",  "R7, @R4",   0x73, 0x74);
    TEST("TM",  "%76, %75",  0x74, 0x75, 0x76);
    TEST("TM",  "R6, %75",   0x74, 0x75, R(6));
    TEST("TM",  "%76, R5",   0x74, R(5), 0x76);
    TEST("TM",  "%77, @%76", 0x75, 0x76, 0x77);
    TEST("TM",  "%77, @R6",  0x75, R(6), 0x77);
    TEST("TM",  "R7, @%76",  0x75, 0x76, R(7));
    TEST("TM",  "%77, #%78", 0x76, 0x77, 0x78);
    TEST("TM",  "R7, #%78",  0x76, R(7), 0x78);
    if (z86()) {
        TEST("TM",  "@%78, #%79", 0x77, 0x78, 0x79);
        TEST("TM",  "@R8, #%79",  0x77, R(8), 0x79);
    }

    TEST("CP",  "R10, R3",   0xA2, 0xA3);
    TEST("CP",  "R10, @R4",  0xA3, 0xA4);
    TEST("CP",  "%A6, %A5",  0xA4, 0xA5, 0xA6);
    TEST("CP",  "R6, %A5",   0xA4, 0xA5, R(6));
    TEST("CP",  "%A6, R5",   0xA4, R(5), 0xA6);
    TEST("CP",  "%A7, @%A6", 0xA5, 0xA6, 0xA7);
    TEST("CP",  "%A7, @R6",  0xA5, R(6), 0xA7);
    TEST("CP",  "R7, @%A6",  0xA5, 0xA6, R(7));
    TEST("CP",  "%A7, #%A8", 0xA6, 0xA7, 0xA8);
    TEST("CP",  "R7, #%A8",  0xA6, R(7), 0xA8);
    if (z86()) {
        TEST("CP",  "@%A8, #%A9", 0xA7, 0xA8, 0xA9);
        TEST("CP",  "@R8, #%A9",  0xA7, R(8), 0xA9);
    }

    TEST("XOR", "R11, R3",   0xB2, 0xB3);
    TEST("XOR", "R11, @R4",  0xB3, 0xB4);
    TEST("XOR", "%B6, %B5",  0xB4, 0xB5, 0xB6);
    TEST("XOR", "R6, %B5",   0xB4, 0xB5, R(6));
    TEST("XOR", "%B6, R5",   0xB4, R(5), 0xB6);
    TEST("XOR", "%B7, @%B6", 0xB5, 0xB6, 0xB7);
    TEST("XOR", "%B7, @R6",  0xB5, R(6), 0xB7);
    TEST("XOR", "R7, @%B6",  0xB5, 0xB6, R(7));
    TEST("XOR", "%B7, #%B8", 0xB6, 0xB7, 0xB8);
    TEST("XOR", "R7, #%B8",  0xB6, R(7), 0xB8);
    if (z86()) {
        TEST("XOR", "@%B8, #%B9", 0xB7, 0xB8, 0xB9);
        TEST("XOR", "@R8, #%B9",  0xB7, R(8), 0xB9);
    }

    if (z88()) {
        TEST("DIV",  "%96, %95",  0x94, 0x95, 0x96);
        TEST("DIV",  "RR6, R5",   0x94, R(5), R(6));
        TEST("DIV",  "%98, @%96", 0x95, 0x96, 0x98);
        TEST("DIV",  "RR8, @R6",  0x95, R(6), R(8));
        TEST("DIV",  "%98, #%97", 0x96, 0x97, 0x98);
        TEST("DIV",  "RR8, #%97", 0x96, 0x97, R(8));
        TEST("MULT", "%86, %85",  0x84, 0x85, 0x86);
        TEST("MULT", "RR6, R5",   0x84, R(5), R(6));
        TEST("MULT", "%88, @%86", 0x85, 0x86, 0x88);
        TEST("MULT", "RR8, @R6",  0x85, R(6), R(8));
        TEST("MULT", "%88, #%87", 0x86, 0x87, 0x88);
        TEST("MULT", "RR8, #%87", 0x86, 0x87, R(8));
    }

    if (z86()) {
        TEST("LD", "R14, @R4", 0xE3, 0xE4);
    } else {
        TEST("LD", "R14, @R4", 0xC7, 0xE4);
    }
    TEST("LD", "%36, %35",  0xE4, 0x35, 0x36);
    TEST("LD", "%37, @%36", 0xE5, 0x36, 0x37);
    TEST("LD", "%37, #%38", 0xE6, 0x37, 0x38);
    if (z86()) {
        TEST("LD", "@%38, #%39", 0xE7, 0x38, 0x39);
    } else {
        TEST("LD", "@%38, #%39", 0xD6, 0x38, 0x39);
    }
    TEST("LD", "R6, R5",   0xE4, R(5), R(6));
    TEST("LD", "R7, @R6",  0xE5, R(6), R(7));
    TEST("LD", "R7, #%E8", 0xE6, R(7), 0xE8);
    if (z86()) {
        TEST("LD", "@R8, #%E9", 0xE7, R(8), 0xE9);
        TEST("LD", "@R15, R4",  0xF3, 0xF4);
    } else {
        TEST("LD", "@R8, #%E9", 0xD6, R(8), 0xE9);
        TEST("LD", "@R15, R4",  0xD7, 0xF4);
    }
    TEST("LD", "@%B7, %F0", 0xF5, 0xF0, 0xB7);
    TEST("LD", "@R7, %F0",  0xF5, 0xF0, R(7));

    if (z88()) {
        TEST("LDW", "%34, %12",    0xC4, 0x12, 0x34);
        TEST("LDW", "%34, @%12",   0xC5, 0x12, 0x34);
        TEST("LDW", "%12, #%3456", 0xC6, 0x12, 0x34, 0x56);
        TEST("LDW", "RR4, RR2",    0xC4, R(2), R(4));
        TEST("LDW", "RR4, @R2",    0xC5, R(2), R(4));
        TEST("LDW", "RR2, #%3456", 0xC6, R(2), 0x34, 0x56);
    }

    if (z86()) {
        TEST("LDC", "R11, @RR4", 0xC2, 0xB4);
        TEST("LDC", "@RR4, R13", 0xD2, 0xD4);
        TEST("LDE", "R7, @RR4",  0x82, 0x74);
        TEST("LDE", "@RR4, R9",  0x92, 0x94);
    } else {
        TEST("LDC", "R11, @RR4", 0xC3, 0xB4);
        TEST("LDC", "@RR4, R13", 0xD3, 0xD4);
        TEST("LDE", "R7, @RR4",  0xC3, 0x75);
        TEST("LDE", "@RR4, R9",  0xD3, 0x95);
    }

    if (z86()) {
        TEST("LDCI", "@R11, @RR4", 0xC3, 0xB4);
        TEST("LDCI", "@RR4, @R13", 0xD3, 0xD4);
        TEST("LDEI", "@R7, @RR4",  0x83, 0x74);
        TEST("LDEI", "@RR4, @R9",  0x93, 0x94);
    } else {
        TEST("LDCI", "R11, @RR4",  0xE3, 0xB4);
        TEST("LDEI", "R7, @RR4",   0xE3, 0x75);
        TEST("LDC", "R10, %A2A1",  0xA7, 0xA0, 0xA1, 0xA2);
        TEST("LDE", "R10, %A3A2",  0xA7, 0xA1, 0xA2, 0xA3);
        TEST("LDC", "%B2B1, R11",  0xB7, 0xB0, 0xB1, 0xB2);
        TEST("LDE", "%B3B2, R11",  0xB7, 0xB1, 0xB2, 0xB3);
        TEST("LDCD", "R15, @RR2",  0xE2, 0xF2);
        TEST("LDED", "R15, @RR2",  0xE2, 0xF3);
        TEST("LDCPD", "@RR2, R15", 0xF2, 0xF2);
        TEST("LDEPD", "@RR2, R15", 0xF2, 0xF3);
        TEST("LDCPI", "@RR4, R15", 0xF3, 0xF4);
        TEST("LDEPI", "@RR4, R15", 0xF3, 0xF5);

        TEST("PUSHUD", "@%23, %45", 0x82, 0x23, 0x45);
        TEST("PUSHUI", "@%23, %45", 0x83, 0x23, 0x45);
        TEST("PUSHUD", "@R2, R3",   0x82, 0xC2, 0xC3);
        TEST("PUSHUI", "@R2, R3",   0x83, 0xC2, 0xC3);
        TEST("POPUD",  "%23, @%45", 0x92, 0x45, 0x23);
        TEST("POPUI",  "%23, @%45", 0x93, 0x45, 0x23);
        TEST("POPUD",  "R2, @R3" ,  0x92, 0xC3, 0xC2);
        TEST("POPUI",  "R2, @R3",   0x93, 0xC3, 0xC2);
    }
}

static void test_indexed() {
    if (z86()) {
        TEST("LD", "R12, %C9(R8)", 0xC7, 0xC8, 0xC9);
        TEST("LD", "%D9(R8), R13", 0xD7, 0xD8, 0xD9);
    } else {
        TEST("LD", "R12, %C9(R8)", 0x87, 0xC8, 0xC9);
        TEST("LD", "%D9(R8), R13", 0x97, 0xD8, 0xD9);

        TEST("LDC", "R14, +127(RR8)",  0xE7, 0xE8, 0x7F);
        TEST("LDE", "R14, -128(RR8)",  0xE7, 0xE9, 0x80);
        TEST("LDC", "0(RR8), R15",     0xF7, 0xF8, 0x00);
        TEST("LDE", "-1(RR8), R15",    0xF7, 0xF9, 0xFF);
        TEST("LDC", "R10, %0080(RR8)", 0xA7, 0xA8, 0x80, 0x00);
        TEST("LDE", "R10, %ABAA(RR8)", 0xA7, 0xA9, 0xAA, 0xAB);
        TEST("LDE", "R10, %ABAA",      0xA7, 0xA1, 0xAA, 0xAB);
        TEST("LDC", "%FF80(RR8), R11", 0xB7, 0xB8, 0x80, 0xFF);
        TEST("LDC", "%FF80, R11",      0xB7, 0xB0, 0x80, 0xFF);
        TEST("LDE", "%BBBA(RR8), R11", 0xB7, 0xB9, 0xBA, 0xBB);
    }

    symtab.intern(0xC9, "bufC9");
    symtab.intern(0xABAA, "table");

    if (z86()) {
        TEST("LD", "R12, bufC9(R8)",   0xC7, 0xC8, 0xC9);
    } else {
        TEST("LD", "R12, bufC9(R8)",   0x87, 0xC8, 0xC9);
        TEST("LDE", "R10, table(RR8)", 0xA7, 0xA9, 0xAA, 0xAB);
    }
}

static void test_setrp() {
    if (z86()) {
        TEST("SRP", "#%30",                             0x31, 0x30);
        ERRT("SRP", "#%31", OPERAND_NOT_ALLOWED, "%31", 0x31, 0x31);
        ERRT("SRP", "#%32", OPERAND_NOT_ALLOWED, "%32", 0x31, 0x32);
        ERRT("SRP", "#%33", OPERAND_NOT_ALLOWED, "%33", 0x31, 0x33);
        ERRT("SRP", "#%34", OPERAND_NOT_ALLOWED, "%34", 0x31, 0x34);
        ERRT("SRP", "#%35", OPERAND_NOT_ALLOWED, "%35", 0x31, 0x35);
        ERRT("SRP", "#%36", OPERAND_NOT_ALLOWED, "%36", 0x31, 0x36);
        ERRT("SRP", "#%37", OPERAND_NOT_ALLOWED, "%37", 0x31, 0x37);
        ERRT("SRP", "#%38", OPERAND_NOT_ALLOWED, "%38", 0x31, 0x38);
        ERRT("SRP", "#%39", OPERAND_NOT_ALLOWED, "%39", 0x31, 0x39);
        ERRT("SRP", "#%3A", OPERAND_NOT_ALLOWED, "%3A", 0x31, 0x3A);
        ERRT("SRP", "#%3B", OPERAND_NOT_ALLOWED, "%3B", 0x31, 0x3B);
        ERRT("SRP", "#%3C", OPERAND_NOT_ALLOWED, "%3C", 0x31, 0x3C);
        ERRT("SRP", "#%3D", OPERAND_NOT_ALLOWED, "%3D", 0x31, 0x3D);
        ERRT("SRP", "#%3E", OPERAND_NOT_ALLOWED, "%3E", 0x31, 0x3E);
        ERRT("SRP", "#%3F", OPERAND_NOT_ALLOWED, "%3F", 0x31, 0x3F);
        TEST("SRP", "#%40",                             0x31, 0x40);
    } else {
        TEST("SRP",  "#%30",                             0x31, 0x30);
        TEST("SRP1", "#%30",                             0x31, 0x31);
        TEST("SRP0", "#%30",                             0x31, 0x32);
        UNKN(                                            0x31, 0x33);
        ERRT("SRP",  "#%34", OPERAND_NOT_ALLOWED, "%34", 0x31, 0x34);
        ERRT("SRP1", "#%34", OPERAND_NOT_ALLOWED, "%34", 0x31, 0x35);
        ERRT("SRP0", "#%34", OPERAND_NOT_ALLOWED, "%34", 0x31, 0x36);
        UNKN(                                            0x31, 0x37);
        ERRT("SRP",  "#%38", OPERAND_NOT_ALLOWED, "%38", 0x31, 0x38);
        TEST("SRP1", "#%38",                             0x31, 0x39);
        TEST("SRP0", "#%38",                             0x31, 0x3A);
        UNKN(                                            0x31, 0x3B);
        ERRT("SRP",  "#%3C", OPERAND_NOT_ALLOWED, "%3C", 0x31, 0x3C);
        ERRT("SRP1", "#%3C", OPERAND_NOT_ALLOWED, "%3C", 0x31, 0x3D);
        ERRT("SRP0", "#%3C", OPERAND_NOT_ALLOWED, "%3C", 0x31, 0x3E);
        UNKN(                                            0x31, 0x3F);
        TEST("SRP",  "#%40",                             0x31, 0x40);
    }
}

static void test_bit_operation() {
    TEST("BITC", "R5, #4",       0x57, 0x58);
    TEST("BITR", "R7, #4",       0x77, 0x78);
    TEST("BITS", "R8, #4",       0x77, 0x89);

    TEST("BOR",  "R0, >%09, #4", 0x07, 0x08, 0x09);
    TEST("BOR",  "R0, R9, #4",   0x07, 0x08, R(9));
    TEST("BOR",  "%AB, #4, R8",  0x07, 0x89, 0xAB);
    TEST("BOR",  "R9, #4, R8",   0x07, 0x89, R(9));
    TEST("BCP",  "R1, %19, #4",  0x17, 0x18, 0x19);
    TEST("BCP",  "R1, R9, #4",   0x17, 0x18, R(9));
    TEST("BXOR", "R2, %29, #4",  0x27, 0x28, 0x29);
    TEST("BXOR", "R2, R9, #4",   0x27, 0x28, R(9));
    TEST("BXOR", "%AB, #4, R8",  0x27, 0x89, 0xAB);
    TEST("BXOR", "R9, #4, R8",   0x27, 0x89, R(9));
    TEST("LDB",  "R4, %49, #4",  0x47, 0x48, 0x49);
    TEST("LDB",  "R4, R9, #4",   0x47, 0x48, R(9));
    TEST("LDB",  "%AB, #4, R8",  0x47, 0x89, 0xAB);
    TEST("LDB",  "R9, #4, R8",   0x47, 0x89, R(9));
    TEST("BAND", "R7, %89, #4",  0x67, 0x78, 0x89);
    TEST("BAND", "R7, R9, #4",   0x67, 0x78, R(9));
    TEST("BAND", "%AB, #4, R8",  0x67, 0x89, 0xAB);
    TEST("BAND", "R9, #4, R8",   0x67, 0x89, R(9));

    disassembler.setOption("relative", "true");
    TEST("BTJRF", "$, R3, #4", 0x37, 0x38, 0xFD);
    TEST("BTJRT", "$, R3, #4", 0x37, 0x39, 0xFD);
}

static void test_illegal_z8() {
    const uint8_t illegals[] = {
        0x0F, 0x1F, 0x2F, 0x3F, 0x4F, 0x5F, 0x6F, 0x7F,
        0x84, 0x85, 0x86, 0x87,
        0x94, 0x95, 0x96, 0x97,
        0xC4, 0xC5, 0xC6,
        0xD5,
        0xE2,
        0xF2, 0xF4, 0xF6, 0xF7,
    };
    for (uint8_t idx = 0; idx < sizeof(illegals); idx++)
        UNKN(illegals[idx]);
}

static void test_illegal_z86c() {
    const uint8_t illegals[] = {
        0x0F, 0x1F, 0x2F, 0x3F, 0x4F, 0x5F,
        0x84, 0x85, 0x86, 0x87,
        0x94, 0x95, 0x96, 0x97,
        0xC4, 0xC5, 0xC6,
        0xD5,
        0xE2,
        0xF2, 0xF4, 0xF6, 0xF7,
    };
    for (uint8_t idx = 0; idx < sizeof(illegals); idx++)
        UNKN(illegals[idx]);
}

static void test_illegal_z88() {
    const uint8_t illegals[] = {
        0x6F, 0x7F, 0xD5,
    };
    for (uint8_t idx = 0; idx < sizeof(illegals); idx++)
        UNKN(illegals[idx]);
}
// clang-format on

void run_tests(const char *cpu) {
    disassembler.setCpu(cpu);
    RUN_TEST(test_implied);
    RUN_TEST(test_absolute);
    RUN_TEST(test_relative);
    RUN_TEST(test_operand_in_opcode);
    RUN_TEST(test_one_operand);
    RUN_TEST(test_two_operands);
    RUN_TEST(test_indexed);
    RUN_TEST(test_setrp);
    if (z88())
        RUN_TEST(test_bit_operation);
    if (z86c()) {
        RUN_TEST(test_illegal_z86c);
    } else if (z86c()) {
        RUN_TEST(test_illegal_z8);
    }
    if (z88())
        RUN_TEST(test_illegal_z88);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

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

#include "dis_i8051.h"
#include "test_dis_helper.h"

using namespace libasm;
using namespace libasm::i8051;
using namespace libasm::test;

DisI8051 dis8051;
Disassembler &disassembler(dis8051);

static void set_up() {
    disassembler.reset();
    disassembler.setCpu("8051");
}

static void tear_down() {
    symtab.reset();
}

// clang-format off
void test_cpu() {
    EQUALS("cpu 8051", true,   disassembler.setCpu("8051"));
    EQUALS_P("cpu 8051", "8051", disassembler.cpu_P());

    EQUALS("cpu I8051", true,   disassembler.setCpu("I8051"));
    EQUALS_P("cpu I8051", "8051", disassembler.cpu_P());
}

static void test_implied() {
    TEST("NOP",  "", 0x00);
    TEST("RET",  "", 0x22);
    TEST("RETI", "", 0x32);
}

static void test_regs() {
    TEST("RR",   "A", 0x03);
    TEST("INC",  "A", 0x04);
    TEST("RRC",  "A", 0x13);
    TEST("DEC",  "A", 0x14);
    TEST("RL",   "A", 0x23);
    TEST("RLC",  "A", 0x33);
    TEST("SWAP", "A", 0xC4);
    TEST("DA",   "A", 0xD4);
    TEST("CLR",  "A", 0xE4);
    TEST("CPL",  "A", 0xF4);

    TEST("DIV",  "AB", 0x84);
    TEST("MUL",  "AB", 0xA4);

    TEST("CPL",  "C", 0xB3);
    TEST("CLR",  "C", 0xC3);
    TEST("SETB", "C", 0xD3);

    TEST("INC",  "DPTR", 0xA3);
}

static void test_regr() {
    TEST("INC",  "R0", 0x08);
    TEST("INC",  "R1", 0x09);
    TEST("INC",  "R2", 0x0A);
    TEST("INC",  "R3", 0x0B);
    TEST("INC",  "R4", 0x0C);
    TEST("INC",  "R5", 0x0D);
    TEST("INC",  "R6", 0x0E);
    TEST("INC",  "R7", 0x0F);
    TEST("DEC",  "R0", 0x18);
    TEST("DEC",  "R1", 0x19);
    TEST("DEC",  "R2", 0x1A);
    TEST("DEC",  "R3", 0x1B);
    TEST("DEC",  "R4", 0x1C);
    TEST("DEC",  "R5", 0x1D);
    TEST("DEC",  "R6", 0x1E);
    TEST("DEC",  "R7", 0x1F);
    TEST("ADD",  "A, R0", 0x28);
    TEST("ADD",  "A, R1", 0x29);
    TEST("ADD",  "A, R2", 0x2A);
    TEST("ADD",  "A, R3", 0x2B);
    TEST("ADD",  "A, R4", 0x2C);
    TEST("ADD",  "A, R5", 0x2D);
    TEST("ADD",  "A, R6", 0x2E);
    TEST("ADD",  "A, R7", 0x2F);
    TEST("ADDC", "A, R0", 0x38);
    TEST("ADDC", "A, R1", 0x39);
    TEST("ADDC", "A, R2", 0x3A);
    TEST("ADDC", "A, R3", 0x3B);
    TEST("ADDC", "A, R4", 0x3C);
    TEST("ADDC", "A, R5", 0x3D);
    TEST("ADDC", "A, R6", 0x3E);
    TEST("ADDC", "A, R7", 0x3F);
    TEST("ORL",  "A, R0", 0x48);
    TEST("ORL",  "A, R1", 0x49);
    TEST("ORL",  "A, R2", 0x4A);
    TEST("ORL",  "A, R3", 0x4B);
    TEST("ORL",  "A, R4", 0x4C);
    TEST("ORL",  "A, R5", 0x4D);
    TEST("ORL",  "A, R6", 0x4E);
    TEST("ORL",  "A, R7", 0x4F);
    TEST("ANL",  "A, R0", 0x58);
    TEST("ANL",  "A, R1", 0x59);
    TEST("ANL",  "A, R2", 0x5A);
    TEST("ANL",  "A, R3", 0x5B);
    TEST("ANL",  "A, R4", 0x5C);
    TEST("ANL",  "A, R5", 0x5D);
    TEST("ANL",  "A, R6", 0x5E);
    TEST("ANL",  "A, R7", 0x5F);
    TEST("XRL",  "A, R0", 0x68);
    TEST("XRL",  "A, R1", 0x69);
    TEST("XRL",  "A, R2", 0x6A);
    TEST("XRL",  "A, R3", 0x6B);
    TEST("XRL",  "A, R4", 0x6C);
    TEST("XRL",  "A, R5", 0x6D);
    TEST("XRL",  "A, R6", 0x6E);
    TEST("XRL",  "A, R7", 0x6F);
    TEST("SUBB", "A, R0", 0x98);
    TEST("SUBB", "A, R1", 0x99);
    TEST("SUBB", "A, R2", 0x9A);
    TEST("SUBB", "A, R3", 0x9B);
    TEST("SUBB", "A, R4", 0x9C);
    TEST("SUBB", "A, R5", 0x9D);
    TEST("SUBB", "A, R6", 0x9E);
    TEST("SUBB", "A, R7", 0x9F);
    TEST("XCH",  "A, R0", 0xC8);
    TEST("XCH",  "A, R1", 0xC9);
    TEST("XCH",  "A, R2", 0xCA);
    TEST("XCH",  "A, R3", 0xCB);
    TEST("XCH",  "A, R4", 0xCC);
    TEST("XCH",  "A, R5", 0xCD);
    TEST("XCH",  "A, R6", 0xCE);
    TEST("XCH",  "A, R7", 0xCF);
    TEST("MOV",  "A, R0", 0xE8);
    TEST("MOV",  "A, R1", 0xE9);
    TEST("MOV",  "A, R2", 0xEA);
    TEST("MOV",  "A, R3", 0xEB);
    TEST("MOV",  "A, R4", 0xEC);
    TEST("MOV",  "A, R5", 0xED);
    TEST("MOV",  "A, R6", 0xEE);
    TEST("MOV",  "A, R7", 0xEF);
    TEST("MOV",  "R0, A", 0xF8);
    TEST("MOV",  "R1, A", 0xF9);
    TEST("MOV",  "R2, A", 0xFA);
    TEST("MOV",  "R3, A", 0xFB);
    TEST("MOV",  "R4, A", 0xFC);
    TEST("MOV",  "R5, A", 0xFD);
    TEST("MOV",  "R6, A", 0xFE);
    TEST("MOV",  "R7, A", 0xFF);
}

static void test_indirect() {
    TEST("JMP",  "@A+DPTR",    0x73);
    TEST("MOVC", "A, @A+PC",   0x83);
    TEST("MOVC", "A, @A+DPTR", 0x93);
    TEST("MOVX", "A, @DPTR",   0xE0);
    TEST("MOVX", "@DPTR, A",   0xF0);
    TEST("MOVX", "A, @R0",     0xE2);
    TEST("MOVX", "A, @R1",     0xE3);
    TEST("MOVX", "@R0, A",     0xF2);
    TEST("MOVX", "@R1, A",     0xF3);

    TEST("INC",  "@R0", 0x06);
    TEST("INC",  "@R1", 0x07);
    TEST("DEC",  "@R0", 0x16);
    TEST("DEC",  "@R1", 0x17);
    TEST("ADD",  "A, @R0", 0x26);
    TEST("ADD",  "A, @R1", 0x27);
    TEST("ADDC", "A, @R0", 0x36);
    TEST("ADDC", "A, @R1", 0x37);
    TEST("ORL",  "A, @R0", 0x46);
    TEST("ORL",  "A, @R1", 0x47);
    TEST("ANL",  "A, @R0", 0x56);
    TEST("ANL",  "A, @R1", 0x57);
    TEST("XRL",  "A, @R0", 0x66);
    TEST("XRL",  "A, @R1", 0x67);
    TEST("SUBB", "A, @R0", 0x96);
    TEST("SUBB", "A, @R1", 0x97);
    TEST("XCH",  "A, @R0", 0xC6);
    TEST("XCH",  "A, @R1", 0xC7);
    TEST("XCHD", "A, @R0", 0xD6);
    TEST("XCHD", "A, @R1", 0xD7);
    TEST("MOV",  "A, @R0", 0xE6);
    TEST("MOV",  "A, @R1", 0xE7);
    TEST("MOV",  "@R0, A", 0xF6);
    TEST("MOV",  "@R1, A", 0xF7);
}

static void test_immediate() {
    TEST("ADD",  "A, #25H", 0x24, 0x25);
    TEST("ADDC", "A, #35H", 0x34, 0x35);
    TEST("ORL",  "A, #45H", 0x44, 0x45);
    TEST("ANL",  "A, #55H", 0x54, 0x55);
    TEST("XRL",  "A, #65H", 0x64, 0x65);
    TEST("SUBB", "A, #95H",    0x94, 0x95);
    NMEM("SUBB", "A, #0", "0", 0x94);
    TEST("ORL",  "44H, #45H", 0x43, 0x44, 0x45);
    TEST("ANL",  "54H, #55H", 0x53, 0x54, 0x55);
    TEST("XRL",  "64H, #65H",          0x63, 0x64, 0x65);
    NMEM("XRL",  "64H, #0", "0",       0x63, 0x64);
    NMEM("XRL",  "00H, #0", "00H, #0", 0x63);

    TEST("MOV", "A, #75H",   0x74, 0x75);
    TEST("MOV", "76H, #77H", 0x75, 0x76, 0x77);
    TEST("MOV", "@R0, #77H", 0x76, 0x77);
    TEST("MOV", "@R1, #78H", 0x77, 0x78);
    TEST("MOV", "R0, #79H",  0x78, 0x79);
    TEST("MOV", "R1, #7AH",  0x79, 0x7A);
    TEST("MOV", "R2, #7BH",  0x7A, 0x7B);
    TEST("MOV", "R3, #7CH",  0x7B, 0x7C);
    TEST("MOV", "R4, #7DH",  0x7C, 0x7D);
    TEST("MOV", "R5, #7EH",  0x7D, 0x7E);
    TEST("MOV", "R6, #7FH",  0x7E, 0x7F);
    TEST("MOV", "R7, #80H",  0x7F, 0x80);

    TEST("MOV", "DPTR, #9192H",          0x90, 0x91, 0x92);
    NMEM("MOV", "DPTR, #9100H", "9100H", 0x90, 0x91);
    NMEM("MOV", "DPTR, #0000H", "0000H", 0x90);
}

static void test_relative() {
    ATEST(0x1000, "JBC", "22H.1, 1015H", 0x10, 0x11, 0x12);
    ATEST(0x1000, "JB",  "24H.1, 1025H", 0x20, 0x21, 0x22);
    ATEST(0x1000, "JNB", "26H.1, 1035H",                 0x30, 0x31, 0x32);
    ANMEM(0x1000, "JNB", "26H.1, 1002H", "1002H",        0x30, 0x31);
    ANMEM(0x1000, "JNB", "20H.0, 1001H", "20H.0, 1001H", 0x30);

    ATEST(0x1000, "JC",   "1043H", 0x40, 0x41);
    ATEST(0x1000, "JNC",  "1053H", 0x50, 0x51);
    ATEST(0x1000, "JZ",   "1063H", 0x60, 0x61);
    ATEST(0x1000, "JNZ",  "1073H", 0x70, 0x71);
    ATEST(0x1000, "SJMP", "0F83H",         0x80, 0x81);
    ANMEM(0x1000, "SJMP", "1001H","1001H", 0x80);

    ATEST(0x1000, "CJNE", "A, #0B5H, 0FB9H",   0xB4, 0xB5, 0xB6);
    ATEST(0x1000, "CJNE", "A, 0B6H, 0FBAH",    0xB5, 0xB6, 0xB7);
    ATEST(0x1000, "CJNE", "@R0, #0B7H, 0FBBH", 0xB6, 0xB7, 0xB8);
    ATEST(0x1000, "CJNE", "@R1, #0B8H, 0FBCH", 0xB7, 0xB8, 0xB9);
    ATEST(0x1000, "CJNE", "R0, #0B9H, 0FBDH",  0xB8, 0xB9, 0xBA);
    ATEST(0x1000, "CJNE", "R1, #0BAH, 0FBEH",  0xB9, 0xBA, 0xBB);
    ATEST(0x1000, "CJNE", "R2, #0BBH, 0FBFH",  0xBA, 0xBB, 0xBC);
    ATEST(0x1000, "CJNE", "R3, #0BCH, 0FC0H",  0xBB, 0xBC, 0xBD);
    ATEST(0x1000, "CJNE", "R4, #0BDH, 0FC1H",  0xBC, 0xBD, 0xBE);
    ATEST(0x1000, "CJNE", "R5, #0BEH, 0FC2H",  0xBD, 0xBE, 0xBF);
    ATEST(0x1000, "CJNE", "R6, #0BFH, 0FC3H",  0xBE, 0xBF, 0xC0);
    ATEST(0x1000, "CJNE", "R7, #0C0H, 0FC4H",          0xBF, 0xC0, 0xC1);
    ANMEM(0x1000, "CJNE", "R7, #0C0H, 1002H", "1002H", 0xBF, 0xC0);
    ANMEM(0x1000, "CJNE", "R7, #0, 1001H", "0, 1001H", 0xBF);

    ATEST(0x1000, "DJNZ", "0D6H, 0FDAH", 0xD5, 0xD6, 0xD7);
    ATEST(0x1000, "DJNZ", "R0, 0FDBH",   0xD8, 0xD9);
    ATEST(0x1000, "DJNZ", "R1, 0FDCH",   0xD9, 0xDA);
    ATEST(0x1000, "DJNZ", "R2, 0FDDH",   0xDA, 0xDB);
    ATEST(0x1000, "DJNZ", "R3, 0FDEH",   0xDB, 0xDC);
    ATEST(0x1000, "DJNZ", "R4, 0FDFH",   0xDC, 0xDD);
    ATEST(0x1000, "DJNZ", "R5, 0FE0H",   0xDD, 0xDE);
    ATEST(0x1000, "DJNZ", "R6, 0FE1H",   0xDE, 0xDF);
    ATEST(0x1000, "DJNZ", "R7, 0FE2H",          0xDF, 0xE0);
    ANMEM(0x1000, "DJNZ", "R7, 1001H", "1001H", 0xDF);

    disassembler.setOption("relative", "on");
    ATEST(0x2000, "JB", "22H.1, $-125", 0x20, 0x11, 0x80);
    ATEST(0x2000, "JB", "22H.1, $",     0x20, 0x11, 0xFD);
    ATEST(0x2000, "JB", "22H.1, $+3",   0x20, 0x11, 0x00);
    ATEST(0x2000, "JB", "22H.1, $+130", 0x20, 0x11, 0x7F);
    ATEST(0x2000, "CJNE", "A, #0B5H, $-125", 0xB4, 0xB5, 0x80);
    ATEST(0x2000, "CJNE", "A, #0B5H, $",     0xB4, 0xB5, 0xFD);
    ATEST(0x2000, "CJNE", "A, #0B5H, $+3",   0xB4, 0xB5, 0x00);
    ATEST(0x2000, "CJNE", "A, #0B5H, $+130", 0xB4, 0xB5, 0x7F);
    ATEST(0x2000, "DJNZ", "0D6H, $-125", 0xD5, 0xD6, 0x80);
    ATEST(0x2000, "DJNZ", "0D6H, $",     0xD5, 0xD6, 0xFD);
    ATEST(0x2000, "DJNZ", "0D6H, $+3",   0xD5, 0xD6, 0x00);
    ATEST(0x2000, "DJNZ", "0D6H, $+130", 0xD5, 0xD6, 0x7F);
    ATEST(0x2000, "JZ", "$-126", 0x60, 0x80);
    ATEST(0x2000, "JZ", "$",     0x60, 0xFE);
    ATEST(0x2000, "JZ", "$+2",   0x60, 0x00);
    ATEST(0x2000, "JZ", "$+129", 0x60, 0x7F);
    ATEST(0x2000, "DJNZ", "R0, $-126", 0xD8, 0x80);
    ATEST(0x2000, "DJNZ", "R0, $",     0xD8, 0xFE);
    ATEST(0x2000, "DJNZ", "R0, $+2",   0xD8, 0x00);
    ATEST(0x2000, "DJNZ", "R0, $+129", 0xD8, 0x7F);
}

static void test_bit_address() {
    dis8051.setOption("relative", "true");
    TEST("JBC", "22H.1, $+21", 0x10, 0x11, 0x12);
    TEST("JB",  "24H.1, $+37", 0x20, 0x21, 0x22);
    TEST("JNB", "26H.1, $+53", 0x30, 0x31, 0x32);

    TEST("ORL",  "C, 2EH.3",  0x72, 0x73);
    TEST("ANL",  "C, 80H.3",  0x82, 0x83);
    TEST("MOV",  "90H.3, C",  0x92, 0x93);
    TEST("MOV",  "C, 0A0H.3", 0xA2, 0xA3);
    TEST("CPL",  "0B0H.3",   0xB2, 0xB3);
    TEST("CLR",  "0C0H.3",   0xC2, 0xC3);
    TEST("SETB", "0D0H.3",   0xD2, 0xD3);

    TEST("ORL",  "C, /0A8H.1", 0xA0, 0xA9);
    TEST("ANL",  "C, /0B0H.1",         0xB0, 0xB1);
    NMEM("ANL",  "C, /20H.0", "20H.0", 0xB0);
}

static void test_direct() {
    TEST("INC",  "06H",    0x05, 0x06);
    TEST("DEC",  "16H",    0x15, 0x16);
    TEST("ADD",  "A, 26H",  0x25, 0x26);
    TEST("ADDC", "A, 36H",  0x35, 0x36);
    TEST("ORL",  "A, 46H",  0x45, 0x46);
    TEST("ANL",  "A, 56H",  0x55, 0x56);
    TEST("XRL",  "A, 66H",  0x65, 0x66);
    TEST("SUBB", "A, 96H",  0x95, 0x96);
    TEST("XCH",  "A, 0C6H", 0xC5, 0xC6);
    TEST("MOV",  "A, 0E6H", 0xE5, 0xE6);
    TEST("MOV",  "0F6H, A", 0xF5, 0xF6);
    NMEM("MOV",  "A, 00H", "00H",    0xE5);
    NMEM("MOV",  "00H, A", "00H, A", 0xF5);

    TEST("ORL", "43H, A", 0x42, 0x43);
    TEST("ANL", "53H, A", 0x52, 0x53);
    TEST("XRL", "63H, A", 0x62, 0x63);

    TEST("MOV", "76H, #77H", 0x75, 0x76, 0x77);
    TEST("MOV", "87H, 86H",  0x85, 0x86, 0x87);
    NMEM("MOV", "00H, 86H",  "00H, 86H", 0x85, 0x86);
    NMEM("MOV", "00H, 00H",  "00H", 0x85);
    TEST("MOV", "87H, @R0",  0x86, 0x87);
    TEST("MOV", "88H, @R1",  0x87, 0x88);
    TEST("MOV", "89H, R0",   0x88, 0x89);
    TEST("MOV", "8AH, R1",   0x89, 0x8A);
    TEST("MOV", "8BH, R2",   0x8A, 0x8B);
    TEST("MOV", "8CH, R3",   0x8B, 0x8C);
    TEST("MOV", "8DH, R4",   0x8C, 0x8D);
    TEST("MOV", "8EH, R5",   0x8D, 0x8E);
    TEST("MOV", "8FH, R6",   0x8E, 0x8F);
    TEST("MOV", "90H, R7",   0x8F, 0x90);
    TEST("MOV", "@R0, 0A7H", 0xA6, 0xA7);
    TEST("MOV", "@R1, 0A8H", 0xA7, 0xA8);
    TEST("MOV", "R0, 0A9H",  0xA8, 0xA9);
    TEST("MOV", "R1, 0AAH",  0xA9, 0xAA);
    TEST("MOV", "R2, 0ABH",  0xAA, 0xAB);
    TEST("MOV", "R3, 0ACH",  0xAB, 0xAC);
    TEST("MOV", "R4, 0ADH",  0xAC, 0xAD);
    TEST("MOV", "R5, 0AEH",  0xAD, 0xAE);
    TEST("MOV", "R6, 0AFH",  0xAE, 0xAF);
    TEST("MOV", "R7, 0B0H",  0xAF, 0xB0);

    TEST("PUSH", "0C1H", 0xC0, 0xC1);
    TEST("POP",  "0D1H", 0xD0, 0xD1);
}

static void test_page() {
    ATEST(0x1000, "AJMP",  "1002H", 0x01, 0x02);
    ATEST(0x1800, "AJMP",  "1922H", 0x21, 0x22);
    ATEST(0x1000, "AJMP",  "1242H", 0x41, 0x42);
    ATEST(0x1800, "AJMP",  "1B62H", 0x61, 0x62);
    ATEST(0x1000, "AJMP",  "1482H", 0x81, 0x82);
    ATEST(0x1800, "AJMP",  "1DA2H", 0xA1, 0xA2);
    ATEST(0x1000, "AJMP",  "16C2H", 0xC1, 0xC2);
    ATEST(0x1800, "AJMP",  "1FE2H", 0xE1, 0xE2);
    ATEST(0x1800, "ACALL", "1812H", 0x11, 0x12);
    ATEST(0x1000, "ACALL", "1132H", 0x31, 0x32);
    ATEST(0x1800, "ACALL", "1A52H", 0x51, 0x52);
    ATEST(0x1000, "ACALL", "1372H", 0x71, 0x72);
    ATEST(0x1800, "ACALL", "1C92H", 0x91, 0x92);
    ATEST(0x1000, "ACALL", "15B2H", 0xB1, 0xB2);
    ATEST(0x1800, "ACALL", "1ED2H", 0xD1, 0xD2);
    ATEST(0x1000, "ACALL", "17F2H",          0xF1, 0xF2);
    ANMEM(0x1000, "ACALL", "1700H", "1700H", 0xF1);

    ATEST(0x17FD, "AJMP",  "1002H", 0x01, 0x02);
    ATEST(0x17FE, "AJMP",  "1802H", 0x01, 0x02);
    ATEST(0x17FF, "AJMP",  "1802H", 0x01, 0x02);
    ATEST(0x1FFD, "ACALL", "1802H", 0x11, 0x02);
    ATEST(0x1FFE, "ACALL", "2002H", 0x11, 0x02);
    ATEST(0x1FFF, "ACALL", "2002H", 0x11, 0x02);
}

static void test_absolute() {
    TEST("LJMP",  "0304H", 0x02, 0x03, 0x04);
    TEST("LCALL", "1314H",          0x12, 0x13, 0x14);
    NMEM("LCALL", "1300H", "1300H", 0x12, 0x13);
    NMEM("LCALL", "0000H", "0000H", 0x12);
}

static void test_illegal() {
    UNKN(0xA5);
}
// clang-format on

void run_tests(const char *cpu) {
    disassembler.setCpu(cpu);
    RUN_TEST(test_implied);
    RUN_TEST(test_regs);
    RUN_TEST(test_regr);
    RUN_TEST(test_indirect);
    RUN_TEST(test_immediate);
    RUN_TEST(test_relative);
    RUN_TEST(test_bit_address);
    RUN_TEST(test_direct);
    RUN_TEST(test_page);
    RUN_TEST(test_absolute);
    RUN_TEST(test_illegal);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

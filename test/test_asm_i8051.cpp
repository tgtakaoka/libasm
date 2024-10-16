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

#include "asm_i8051.h"
#include "test_asm_helper.h"

using namespace libasm;
using namespace libasm::i8051;
using namespace libasm::test;

AsmI8051 asm8051;
Assembler &assembler(asm8051);

static void set_up() {
    assembler.reset();
    assembler.setCpu("8051");
}

static void tear_down() {
    symtab.reset();
}

// clang-format off
void test_cpu() {
    EQUALS("cpu 8051", true,   assembler.setCpu("8051"));
    EQUALS_P("cpu 8051", "8051", assembler.config().cpu_P());

    EQUALS("cpu I8051", true,   assembler.setCpu("I8051"));
    EQUALS_P("cpu I8051", "8051", assembler.config().cpu_P());
}

static void test_implied() {
    TEST("NOP",  0x00);
    TEST("RET",  0x22);
    TEST("RETI", 0x32);
}

static void test_regs() {
    TEST("rr   a", 0x03);
    TEST("INC  A", 0x04);
    TEST("RRC  A", 0x13);
    TEST("DEC  A", 0x14);
    TEST("RL   A", 0x23);
    TEST("RLC  A", 0x33);
    TEST("SWAP A", 0xC4);
    TEST("DA   A", 0xD4);
    TEST("CLR  A", 0xE4);
    TEST("CPL  A", 0xF4);

    TEST("DIV AB", 0x84);
    TEST("MUL AB", 0xA4);

    TEST("CPL  C", 0xB3);
    TEST("CLR  C", 0xC3);
    TEST("SETB C", 0xD3);

    TEST("INC DPTR", 0xA3);
}

static void test_regr() {
    TEST("INC R0", 0x08);
    TEST("INC R1", 0x09);
    TEST("INC R2", 0x0A);
    TEST("INC R3", 0x0B);
    TEST("INC R4", 0x0C);
    TEST("INC R5", 0x0D);
    TEST("INC R6", 0x0E);
    TEST("INC R7", 0x0F);
    TEST("DEC R0", 0x18);
    TEST("DEC R1", 0x19);
    TEST("DEC R2", 0x1A);
    TEST("DEC R3", 0x1B);
    TEST("DEC R4", 0x1C);
    TEST("DEC R5", 0x1D);
    TEST("DEC R6", 0x1E);
    TEST("DEC R7", 0x1F);
    TEST("ADD A,R0", 0x28);
    TEST("ADD A,R1", 0x29);
    TEST("ADD A,R2", 0x2A);
    TEST("ADD A,R3", 0x2B);
    TEST("ADD A,R4", 0x2C);
    TEST("ADD A,R5", 0x2D);
    TEST("ADD A,R6", 0x2E);
    TEST("ADD A,R7", 0x2F);
    TEST("ADDC A,R0", 0x38);
    TEST("ADDC A,R1", 0x39);
    TEST("ADDC A,R2", 0x3A);
    TEST("ADDC A,R3", 0x3B);
    TEST("ADDC A,R4", 0x3C);
    TEST("ADDC A,R5", 0x3D);
    TEST("ADDC A,R6", 0x3E);
    TEST("ADDC A,R7", 0x3F);
    TEST("ORL A,R0", 0x48);
    TEST("ORL A,R1", 0x49);
    TEST("ORL A,R2", 0x4A);
    TEST("ORL A,R3", 0x4B);
    TEST("ORL A,R4", 0x4C);
    TEST("ORL A,R5", 0x4D);
    TEST("ORL A,R6", 0x4E);
    TEST("ORL A,R7", 0x4F);
    TEST("ANL A,R0", 0x58);
    TEST("ANL A,R1", 0x59);
    TEST("ANL A,R2", 0x5A);
    TEST("ANL A,R3", 0x5B);
    TEST("ANL A,R4", 0x5C);
    TEST("ANL A,R5", 0x5D);
    TEST("ANL A,R6", 0x5E);
    TEST("ANL A,R7", 0x5F);
    TEST("XRL A,R0", 0x68);
    TEST("XRL A,R1", 0x69);
    TEST("XRL A,R2", 0x6A);
    TEST("XRL A,R3", 0x6B);
    TEST("XRL A,R4", 0x6C);
    TEST("XRL A,R5", 0x6D);
    TEST("XRL A,R6", 0x6E);
    TEST("XRL A,R7", 0x6F);
    TEST("SUBB A,R0", 0x98);
    TEST("SUBB A,R1", 0x99);
    TEST("SUBB A,R2", 0x9A);
    TEST("SUBB A,R3", 0x9B);
    TEST("SUBB A,R4", 0x9C);
    TEST("SUBB A,R5", 0x9D);
    TEST("SUBB A,R6", 0x9E);
    TEST("SUBB A,R7", 0x9F);
    TEST("XCH A,R0", 0xC8);
    TEST("XCH A,R1", 0xC9);
    TEST("XCH A,R2", 0xCA);
    TEST("XCH A,R3", 0xCB);
    TEST("XCH A,R4", 0xCC);
    TEST("XCH A,R5", 0xCD);
    TEST("XCH A,R6", 0xCE);
    TEST("XCH A,R7", 0xCF);
    TEST("MOV A,R0", 0xE8);
    TEST("MOV A,R1", 0xE9);
    TEST("MOV A,R2", 0xEA);
    TEST("MOV A,R3", 0xEB);
    TEST("MOV A,R4", 0xEC);
    TEST("MOV A,R5", 0xED);
    TEST("MOV A,R6", 0xEE);
    TEST("MOV A,R7", 0xEF);
    TEST("MOV R0,A", 0xF8);
    TEST("MOV R1,A", 0xF9);
    TEST("MOV R2,A", 0xFA);
    TEST("MOV R3,A", 0xFB);
    TEST("MOV R4,A", 0xFC);
    TEST("MOV R5,A", 0xFD);
    TEST("MOV R6,A", 0xFE);
    TEST("MOV R7,A", 0xFF);

    ERUS("INC R8", "R8", 0x05, 0x00);
}

static void test_indirect() {
    TEST("JMP  @A+DPTR",   0x73);
    TEST("MOVC A,@A+PC",   0x83);
    TEST("MOVC A,@A+DPTR", 0x93);
    TEST("MOVX A,@DPTR",   0xE0);
    TEST("MOVX @DPTR,A",   0xF0);
    TEST("MOVX A,@R0",     0xE2);
    TEST("MOVX A,@R1",     0xE3);
    TEST("MOVX @R0,A",     0xF2);
    TEST("MOVX @R1,A",     0xF3);

    TEST("INC @R0", 0x06);
    TEST("INC @R1", 0x07);
    TEST("DEC @R0", 0x16);
    TEST("DEC @R1", 0x17);
    TEST("ADD  A,@R0", 0x26);
    TEST("ADD  A,@R1", 0x27);
    TEST("ADDC A,@R0", 0x36);
    TEST("ADDC A,@R1", 0x37);
    TEST("ORL  A,@R0", 0x46);
    TEST("ORL  A,@R1", 0x47);
    TEST("ANL  A,@R0", 0x56);
    TEST("ANL  A,@R1", 0x57);
    TEST("XRL  A,@R0", 0x66);
    TEST("XRL  A,@R1", 0x67);
    TEST("SUBB A,@R0", 0x96);
    TEST("SUBB A,@R1", 0x97);
    TEST("XCH  A,@R0", 0xC6);
    TEST("XCH  A,@R1", 0xC7);
    TEST("XCHD A,@R0", 0xD6);
    TEST("XCHD A,@R1", 0xD7);
    TEST("MOV  A,@R0", 0xE6);
    TEST("MOV  A,@R1", 0xE7);
    TEST("MOV  @R0,A", 0xF6);
    TEST("MOV  @R1,A", 0xF7);

    ERRT("INC @R2",   UNKNOWN_OPERAND, "@R2");
    ERRT("MOV @R8,A", UNKNOWN_OPERAND, "@R8,A");
}

static void test_immediate() {
    TEST("ADD  A,#25H",   0x24, 0x25);
    TEST("ADD  A,#-128",  0x24, 0x80);
    TEST("ADD  A,#255",   0x24, 0xFF);
    ERRT("ADD  A,#-129",  OVERFLOW_RANGE, "#-129", 0x24, 0x7F);
    ERRT("ADD  A,#256",   OVERFLOW_RANGE, "#256",  0x24, 0x00);
    TEST("ADDC A,#35H",   0x34, 0x35);
    TEST("ORL  A,#45H",   0x44, 0x45);
    TEST("ANL  A,#55H",   0x54, 0x55);
    TEST("XRL  A,#65H",   0x64, 0x65);
    TEST("SUBB A,#95H",   0x94, 0x95);
    TEST("ORL  44H,#45H", 0x43, 0x44, 0x45);
    TEST("ANL  54H,#55H", 0x53, 0x54, 0x55);
    TEST("XRL  64H,#65H", 0x63, 0x64, 0x65);
    ERRT("XRL  100H,#65H", OVERFLOW_RANGE, "100H,#65H", 0x63, 0x00, 0x65);

    TEST("MOV A,#75H",   0x74, 0x75);
    TEST("MOV 76H,#77H", 0x75, 0x76, 0x77);
    TEST("MOV @R0,#77H", 0x76, 0x77);
    TEST("MOV @R1,#78H", 0x77, 0x78);
    TEST("MOV R0,#79H",  0x78, 0x79);
    TEST("MOV R1,#7AH",  0x79, 0x7A);
    TEST("MOV R2,#7BH",  0x7A, 0x7B);
    TEST("MOV R3,#7CH",  0x7B, 0x7C);
    TEST("MOV R4,#7DH",  0x7C, 0x7D);
    TEST("MOV R5,#7EH",  0x7D, 0x7E);
    TEST("MOV R6,#7FH",  0x7E, 0x7F);
    TEST("MOV R7,#80H",  0x7F, 0x80);
    ERRT("MOV R4,#-129", OVERFLOW_RANGE, "#-129", 0x7C, 0x7F);
    ERRT("MOV R4,#256",  OVERFLOW_RANGE, "#256",  0x7C, 0x00);

    TEST("MOV DPTR,#9192H", 0x90, 0x91, 0x92);

    symtab.intern(-129, "_129");
    symtab.intern(-128, "_128");
    symtab.intern(255,  "?255");
    symtab.intern(256,  "?256");
    symtab.intern(0x30, "dir30");
    symtab.intern(0x1234, "sym1234");

    TEST("ADD A,#_128", 0x24, 0x80);
    TEST("XRL A,#?255", 0x64, 0xFF);
    ERRT("ADD A,#_129", OVERFLOW_RANGE, "#_129", 0x24, 0x7F);
    ERRT("ORL A,#?256", OVERFLOW_RANGE, "#?256", 0x44, 0x00);
    TEST("MOV dir30,#_128",   0x75, 0x30, 0x80);
    TEST("MOV DPTR,#sym1234", 0x90, 0x12, 0x34);
}

static void test_relative() {
    ATEST(0x1000, "JBC 22H.1,1015H", 0x10, 0x11, 0x12);
    ATEST(0x1000, "JB  24H.1,1025H", 0x20, 0x21, 0x22);
    ATEST(0x1000, "JNB 26H.1,1035H", 0x30, 0x31, 0x32);
    AERRT(0x1000, "JNB 26H.1,0F82H", OPERAND_TOO_FAR, "0F82H", 0x30, 0x31, 0x7F);
    AERRT(0x1000, "JNB 26H.1,1083H", OPERAND_TOO_FAR, "1083H", 0x30, 0x31, 0x80);

    ATEST(0x1000, "JC   1043H", 0x40, 0x41);
    ATEST(0x1000, "JNC  1053H", 0x50, 0x51);
    ATEST(0x1000, "JZ   1063H", 0x60, 0x61);
    ATEST(0x1000, "JNZ  1073H", 0x70, 0x71);
    ATEST(0x1000, "SJMP 0F83H", 0x80, 0x81);
    AERRT(0x1000, "SJMP 0F81H", OPERAND_TOO_FAR, "0F81H", 0x80, 0x7F);
    AERRT(0x1000, "SJMP 1082H", OPERAND_TOO_FAR, "1082H", 0x80, 0x80);
    ATEST(0x1000, "SJMP $+80H", 0x80, 0x7E);

    ATEST(0x1000, "CJNE A,#0B5H,0FB9H",   0xB4, 0xB5, 0xB6);
    ATEST(0x1000, "CJNE A,0B6H,0FBAH",    0xB5, 0xB6, 0xB7);
    ATEST(0x1000, "CJNE @R0,#0B7H,0FBBH", 0xB6, 0xB7, 0xB8);
    ATEST(0x1000, "CJNE @R1,#0B8H,0FBCH", 0xB7, 0xB8, 0xB9);
    ATEST(0x1000, "CJNE R0,#0B9H,0FBDH",  0xB8, 0xB9, 0xBA);
    ATEST(0x1000, "CJNE R1,#0BAH,0FBEH",  0xB9, 0xBA, 0xBB);
    ATEST(0x1000, "CJNE R2,#0BBH,0FBFH",  0xBA, 0xBB, 0xBC);
    ATEST(0x1000, "CJNE R3,#0BCH,0FC0H",  0xBB, 0xBC, 0xBD);
    ATEST(0x1000, "CJNE R4,#0BDH,0FC1H",  0xBC, 0xBD, 0xBE);
    ATEST(0x1000, "CJNE R5,#0BEH,0FC2H",  0xBD, 0xBE, 0xBF);
    ATEST(0x1000, "CJNE R6,#0BFH,0FC3H",  0xBE, 0xBF, 0xC0);
    ATEST(0x1000, "CJNE R7,#0C0H,0FC4H",  0xBF, 0xC0, 0xC1);
    AERRT(0x1000, "CJNE R7,#256,0FC4H",   OVERFLOW_RANGE, "#256,0FC4H", 0xBF, 0x00, 0xC1);
    AERRT(0x1000, "CJNE R7,#0C0H,0F82H",  OPERAND_TOO_FAR, "0F82H",     0xBF, 0xC0, 0x7F);
    AERRT(0x1000, "CJNE R7,#0C0H,1083H",  OPERAND_TOO_FAR, "1083H",     0xBF, 0xC0, 0x80);

    ATEST(0x1000, "DJNZ 0D6H,0FDAH", 0xD5, 0xD6, 0xD7);
    ATEST(0x1000, "DJNZ R0,0FDBH",   0xD8, 0xD9);
    ATEST(0x1000, "DJNZ R1,0FDCH",   0xD9, 0xDA);
    ATEST(0x1000, "DJNZ R2,0FDDH",   0xDA, 0xDB);
    ATEST(0x1000, "DJNZ R3,0FDEH",   0xDB, 0xDC);
    ATEST(0x1000, "DJNZ R4,0FDFH",   0xDC, 0xDD);
    ATEST(0x1000, "DJNZ R5,0FE0H",   0xDD, 0xDE);
    ATEST(0x1000, "DJNZ R6,0FE1H",   0xDE, 0xDF);
    ATEST(0x1000, "DJNZ R7,0FE2H",   0xDF, 0xE0);
    AERRT(0x1000, "DJNZ 100H,0FDAH", OVERFLOW_RANGE, "100H,0FDAH", 0xD5, 0x00, 0xD7);
    AERRT(0x1000, "DJNZ R7,0F81H",   OPERAND_TOO_FAR, "0F81H",     0xDF, 0x7F);
    AERRT(0x1000, "DJNZ R7,1082H",   OPERAND_TOO_FAR, "1082H",     0xDF, 0x80);

    symtab.intern(0x0F81, "sym0F81");
    symtab.intern(0x0F82, "sym0F82");
    symtab.intern(0x0F83, "sym0F83");
    symtab.intern(0x1081, "sym1081");
    symtab.intern(0x1082, "sym1082");
    symtab.intern(0x1083, "sym1083");
    symtab.intern(0x40,   "counter");

    AERRT(0x1000, "SJMP sym0F81", OPERAND_TOO_FAR, "sym0F81", 0x80, 0x7F);
    ATEST(0x1000, "SJMP sym0F82",                             0x80, 0x80);
    ATEST(0x1000, "SJMP sym1081",                             0x80, 0x7F);
    AERRT(0x1000, "SJMP sym1082", OPERAND_TOO_FAR, "sym1082", 0x80, 0x80);
    AERRT(0x1000, "DJNZ counter,sym0F82", OPERAND_TOO_FAR, "sym0F82", 0xD5, 0x40, 0x7F);
    ATEST(0x1000, "DJNZ counter,sym0F83",                             0xD5, 0x40, 0x80);
    ATEST(0x1000, "DJNZ counter,sym1082",                             0xD5, 0x40, 0x7F);
    AERRT(0x1000, "DJNZ counter,sym1083", OPERAND_TOO_FAR, "sym1083", 0xD5, 0x40, 0x80);
}

static void test_bit_address() {
    TEST("JBC 22H.1,$+15H", 0x10, 0x11, 0x12);
    TEST("JB  24H.1,$+25H", 0x20, 0x21, 0x22);
    TEST("JNB 26H.1,$+35H", 0x30, 0x31, 0x32);
    TEST("JB  24H,$+25H",   0x20, 0x24, 0x22);
    AERRT(0x1000, "JB  24H,0F82H", OPERAND_TOO_FAR, "0F82H", 0x20, 0x24, 0x7F);
    AERRT(0x1000, "JB  24H,1083H", OPERAND_TOO_FAR, "1083H", 0x20, 0x24, 0x80);

    TEST("ORL  C,2EH.3",  0x72, 0x73);
    TEST("ANL  C,80H.3",  0x82, 0x83);
    TEST("MOV  90H.3,C",  0x92, 0x93);
    TEST("MOV  C,0A0H.3", 0xA2, 0xA3);
    TEST("CPL  0B0H.3",   0xB2, 0xB3);
    TEST("CLR  0C0H.3",   0xC2, 0xC3);
    TEST("SETB 0D0H.3",   0xD2, 0xD3);
    TEST("MOV  93H,C",    0x92, 0x93);
    TEST("MOV  C,0A3H",   0xA2, 0xA3);

    TEST("ORL C,/0A8H.1", 0xA0, 0xA9);
    TEST("ANL C,/0B0H.1", 0xB0, 0xB1);
    TEST("ANL C,/0B1H",   0xB0, 0xB1);

    symtab.intern(0x2F, "reg2F");
    symtab.intern(0xE0, "regE0");
    symtab.intern(3, "bit3");

    TEST("MOV reg2F.bit3,C",  0x92, 0x7B);
    TEST("ANL C,/regE0.bit3", 0xB0, 0xE3);

    ERRT("ORL C,2EH.8", ILLEGAL_BIT_NUMBER, "8");

    for (uint16_t i = 0x00; i < 0x100; i++) {
        char source[20], operand[10];
        sprintf(operand, "0%02XH.3", i);
        sprintf(source, "CLR %s", operand);
        if ((i & ~0x0F) == 0x20) {
            uint8_t addr = ((i & 0x0F) << 3) | 3;
            TEST(source, 0xC2, addr);
        } else if ((i & ~0x78) == 0x80) {
            uint8_t addr = (i & 0xF8) | 3;
            TEST(source, 0xC2, addr);
        } else {
            ERRT(source, NOT_BIT_ADDRESSABLE, operand);
        }
    }
}

static void test_direct() {
    TEST("INC  06H",    0x05, 0x06);
    TEST("DEC  16H",    0x15, 0x16);
    ERRT("DEC  100H",   OVERFLOW_RANGE, "100H", 0x15, 0x00);
    TEST("ADD  A,26H",  0x25, 0x26);
    TEST("ADDC A,36H",  0x35, 0x36);
    TEST("ORL  A,46H",  0x45, 0x46);
    TEST("ANL  A,56H",  0x55, 0x56);
    TEST("XRL  A,66H",  0x65, 0x66);
    TEST("SUBB A,96H",  0x95, 0x96);
    TEST("XCH  A,0C6H", 0xC5, 0xC6);
    TEST("MOV  A,0E6H", 0xE5, 0xE6);
    ERRT("MOV  A,101H", OVERFLOW_RANGE, "101H", 0xE5, 0x01);
    ERRT("MOV  A,0E0H", OPERAND_NOT_ALLOWED, "0E0H", 0xE5, 0xE0);
    TEST("MOV  0F6H,A", 0xF5, 0xF6);
    ERRT("MOV  102H,A", OVERFLOW_RANGE, "102H,A", 0xF5, 0x02);

    TEST("ORL 43H,A", 0x42, 0x43);
    TEST("ANL 53H,A", 0x52, 0x53);
    TEST("XRL 63H,A", 0x62, 0x63);

    TEST("MOV 76H,#77H", 0x75, 0x76, 0x77);
    TEST("MOV 76H,#-1",  0x75, 0x76, 0xFF);
    ERRT("MOV 101H,#2",  OVERFLOW_RANGE, "101H,#2", 0x75, 0x01, 0x02);
    ERRT("MOV 76H,#257", OVERFLOW_RANGE, "#257",    0x75, 0x76, 0x01);
    TEST("MOV 87H,86H",  0x85, 0x86, 0x87);
    TEST("MOV 87H,@R0",  0x86, 0x87);
    TEST("MOV 88H,@R1",  0x87, 0x88);
    TEST("MOV 89H,R0",   0x88, 0x89);
    TEST("MOV 8AH,R1",   0x89, 0x8A);
    TEST("MOV 8BH,R2",   0x8A, 0x8B);
    TEST("MOV 8CH,R3",   0x8B, 0x8C);
    TEST("MOV 8DH,R4",   0x8C, 0x8D);
    TEST("MOV 8EH,R5",   0x8D, 0x8E);
    TEST("MOV 8FH,R6",   0x8E, 0x8F);
    TEST("MOV 90H,R7",   0x8F, 0x90);
    TEST("MOV @R0,0A7H", 0xA6, 0xA7);
    TEST("MOV @R1,0A8H", 0xA7, 0xA8);
    TEST("MOV R0,0A9H",  0xA8, 0xA9);
    TEST("MOV R1,0AAH",  0xA9, 0xAA);
    TEST("MOV R2,0ABH",  0xAA, 0xAB);
    TEST("MOV R3,0ACH",  0xAB, 0xAC);
    TEST("MOV R4,0ADH",  0xAC, 0xAD);
    TEST("MOV R5,0AEH",  0xAD, 0xAE);
    TEST("MOV R6,0AFH",  0xAE, 0xAF);
    TEST("MOV R7,0B0H",  0xAF, 0xB0);
    ERRT("MOV R7,101H",  OVERFLOW_RANGE, "101H", 0xAF, 0x01);

    TEST("PUSH 0C1H", 0xC0, 0xC1);
    TEST("POP  0D1H", 0xD0, 0xD1);
    ERRT("POP  102H", OVERFLOW_RANGE, "102H", 0xD0, 0x02);

    symtab.intern(0x2F, "reg2F");
    symtab.intern(0xE0, "regE0");

    TEST("PUSH reg2F",     0xC0, 0x2F);
    TEST("MOV  @R0,regE0", 0xA6, 0xE0);
}

static void test_jump() {
    ATEST(0x1000, "AJMP  1002H", 0x01, 0x02);
    ATEST(0x1800, "AJMP  1922H", 0x21, 0x22);
    ATEST(0x1000, "AJMP  1242H", 0x41, 0x42);
    ATEST(0x1800, "AJMP  1B62H", 0x61, 0x62);
    ATEST(0x1000, "AJMP  1482H", 0x81, 0x82);
    ATEST(0x1800, "AJMP  1DA2H", 0xA1, 0xA2);
    ATEST(0x1000, "AJMP  16C2H", 0xC1, 0xC2);
    ATEST(0x1800, "AJMP  1FE2H", 0xE1, 0xE2);
    ATEST(0x1800, "ACALL 1812H", 0x11, 0x12);
    ATEST(0x1000, "ACALL 1132H", 0x31, 0x32);
    ATEST(0x1800, "ACALL 1A52H", 0x51, 0x52);
    ATEST(0x1000, "ACALL 1372H", 0x71, 0x72);
    ATEST(0x1800, "ACALL 1C92H", 0x91, 0x92);
    ATEST(0x1000, "ACALL 15B2H", 0xB1, 0xB2);
    ATEST(0x1800, "ACALL 1ED2H", 0xD1, 0xD2);
    ATEST(0x1000, "ACALL 17F2H", 0xF1, 0xF2);

    ATEST(0x17FD, "AJMP  1002H", 0x01, 0x02);
    AERRT(0x17FE, "AJMP  1002H", OPERAND_TOO_FAR, "1002H", 0x01, 0x02);
    ATEST(0x17FE, "AJMP  1802H", 0x01, 0x02);
    ATEST(0x17FF, "AJMP  1802H", 0x01, 0x02);
    ATEST(0x1FFD, "ACALL 1802H", 0x11, 0x02);
    AERRT(0x1FFE, "ACALL 1802H", OPERAND_TOO_FAR, "1802H", 0x11, 0x02);
    ATEST(0x1FFE, "ACALL 2002H", 0x11, 0x02);
    ATEST(0x1FFF, "ACALL 2002H", 0x11, 0x02);

    TEST("LJMP  0304H", 0x02, 0x03, 0x04);
    TEST("LCALL 1314H", 0x12, 0x13, 0x14);

    symtab.intern(0x1002, "sym1002");
    symtab.intern(0x1802, "sym1802");

    ATEST(0x17FD, "ACALL sym1002", 0x11, 0x02);
    ATEST(0x17FE, "ACALL sym1802", 0x11, 0x02);
    ATEST(0x17FF, "ACALL sym1802", 0x11, 0x02);

    TEST("option smart-branch, on");
    ATEST(0x1000, "AJMP  17FFH", 0xE1, 0xFF);
    ATEST(0x1000, "AJMP  1802H", 0x02, 0x18, 0x02);
    ATEST(0x1000, "LJMP  17FFH", 0xE1, 0xFF);
    ATEST(0x1000, "LJMP  1802H", 0x02, 0x18, 0x02);
    ATEST(0x1000, "ACALL 17FFH", 0xF1, 0xFF);
    ATEST(0x1000, "ACALL 1802H", 0x12, 0x18, 0x02);
    ATEST(0x1000, "LCALL 17FFH", 0xF1, 0xFF);
    ATEST(0x1000, "LCALL 1802H", 0x12, 0x18, 0x02);
}

static void test_comment() {
    COMM("NOP       ; comment",      "; comment", 0x00);
    COMM("SWAP A    ; comment",      "; comment", 0xC4);
    COMM("INC  DPTR ; comment",      "; comment", 0xA3);
    COMM("ANL  A , R0 ; comment",    "; comment", 0x58);
    COMM("JMP  @A+DPTR ; comment",   "; comment", 0x73);
    ERRT("JMP  @ A+DPTR ; comment",  UNKNOWN_OPERAND, "@ A+DPTR ; comment");
    ERRT("JMP  @A +DPTR ; comment",  UNKNOWN_OPERAND, "@A +DPTR ; comment");
    ERRT("JMP  @A+ DPTR ; comment",  UNKNOWN_OPERAND, "@A+ DPTR ; comment");
    ERRT("JMP  @ A+PC   ; comment",  UNKNOWN_OPERAND, "@ A+PC   ; comment");
    ERRT("JMP  @A +PC   ; comment",  UNKNOWN_OPERAND, "@A +PC   ; comment");
    ERRT("JMP  @A+ PC   ; comment",  UNKNOWN_OPERAND, "@A+ PC   ; comment");
    COMM("MOVX A , @DPTR ; comment", "; comment", 0xE0);
    COMM("MOVX @DPTR , A ; comment", "; comment", 0xF0);
    ERRT("MOVX A,@ DPTR ; comment",  UNKNOWN_OPERAND, "@ DPTR ; comment");
    ERRT("MOVX @ DPTR,A ; comment",  UNKNOWN_OPERAND, "@ DPTR,A ; comment");
    ERRT("INC  @ R0     ; comment",  UNKNOWN_OPERAND, "@ R0     ; comment");
    COMM("ADD  A , @R1  ; comment",  "; comment", 0x27);
    ERRT("ADD  A, @ R1  ; comment",  UNKNOWN_OPERAND, "@ R1  ; comment");
    COMM("ADD  A , # 25H   ; comment",      "; comment", 0x24, 0x25);
    COMM("ORL  44H , # 45H ; comment",      "; comment", 0x43, 0x44, 0x45);
    COMM("JBC  22H.1 , $ + 15H  ; comment", "; comment", 0x10, 0x11, 0x12);
    COMM("ORL  C , / 0A8H.1     ; comment", "; comment", 0xA0, 0xA9);
    ACOMM(0x1000, "CJNE A , # 0B5H , $ - 47H ; comment", "; comment", 0xB4, 0xB5, 0xB6);
    COMM("DB -128, 255 ; comment", "; comment", 0x80, 0xFF);
    COMM("DB 'TEXT'    ; comment", "; comment", 0x54, 0x45, 0x58, 0x54);
    COMM("DW -128, 255 ; comment", "; comment", 0xFF, 0x80, 0x00, 0xFF);
}

static void test_undef() {
    ERUS("ADD A,#UNDEF",     "UNDEF",        0x24, 0x00);
    ERUS("ORL UNDEF,#45H",   "UNDEF,#45H",   0x43, 0x00, 0x45);
    ERUS("ORL 44H,#UNDEF",   "UNDEF",        0x43, 0x44, 0x00);
    ERUS("ORL UNDEF,#UNDEF", "UNDEF,#UNDEF", 0x43, 0x00, 0x00);
    ERUS("MOV DPTR,#UNDEF",  "UNDEF",        0x90, 0x00, 0x00);

    ERUS("JBC UNDEF.1,$+15H",     "UNDEF.1,$+15H",       0x10, 0x01, 0x12);
    ERUS("JBC 22H.UNDEF,$+15H",   "UNDEF,$+15H",         0x10, 0x10, 0x12);
    ERUS("JBC UNDEF.UNDEF,$+15H", "UNDEF.UNDEF,$+15H",   0x10, 0x00, 0x12);
    ERUS("JBC 22H.1,UNDEF",       "UNDEF",               0x10, 0x11, 0x00);
    ERUS("JBC UNDEF.1,UNDEF",     "UNDEF.1,UNDEF",       0x10, 0x01, 0x00);
    ERUS("JBC 22H.UNDEF,UNDEF",   "UNDEF,UNDEF",         0x10, 0x10, 0x00);
    ERUS("JBC UNDEF.UNDEF,UNDEF", "UNDEF.UNDEF,UNDEF",   0x10, 0x00, 0x00);

    ERUS("JC  UNDEF",           "UNDEF",       0x40, 0x00);
    ERUS("CJNE A,#UNDEF,$-47H", "UNDEF,$-47H", 0xB4, 0x00, 0xB6);
    ERUS("CJNE A,#0B5H,UNDEF",  "UNDEF",       0xB4, 0xB5, 0x00);
    ERUS("CJNE A,#UNDEF,UNDEF", "UNDEF,UNDEF", 0xB4, 0x00, 0x00);
    ERUS("CJNE A,UNDEF,$-46H",  "UNDEF,$-46H", 0xB5, 0x00, 0xB7);
    ERUS("CJNE A,0B6H,UNDEF",   "UNDEF",       0xB5, 0xB6, 0x00);
    ERUS("CJNE A,UNDEF,UNDEF",  "UNDEF,UNDEF", 0xB5, 0x00, 0x00);
    ERUS("DJNZ UNDEF,$-26H",    "UNDEF,$-26H", 0xD5, 0x00, 0xD7);
    ERUS("DJNZ 0D6H,UNDEF",     "UNDEF",       0xD5, 0xD6, 0x00);
    ERUS("DJNZ UNDEF,UNDEF",    "UNDEF,UNDEF", 0xD5, 0x00, 0x00);
    ERUS("DJNZ R0,UNDEF",       "UNDEF",       0xD8, 0x00);

    ERUS("MOV  UNDEF.3,C",      "UNDEF.3,C",     0x92, 0x03);
    ERUS("MOV  90H.UNDEF,C",    "UNDEF,C",       0x92, 0x90);
    ERUS("MOV  UNDEF.UNDEF,C",  "UNDEF.UNDEF,C", 0x92, 0x00);
    ERUS("MOV  UNDEF,C",        "UNDEF,C",       0x92, 0x00);
    ERUS("ORL  C,/UNDEF.1",     "UNDEF.1",       0xA0, 0x01);
    ERUS("ORL  C,/0A8H.UNDEF",  "UNDEF",         0xA0, 0xA8);
    ERUS("ORL  C,/UNDEF.UNDEF", "UNDEF.UNDEF",   0xA0, 0x00);
    ERUS("ORL  C,/UNDEF",       "UNDEF",         0xA0, 0x00);

    ERUS("INC UNDEF",       "UNDEF",       0x05, 0x00);
    ERUS("MOV UNDEF,A",     "UNDEF,A",     0xF5, 0x00);
    ERUS("MOV UNDEF,86H",   "UNDEF,86H",   0x85, 0x86, 0x00);
    ERUS("MOV 87H,UNDEF",   "UNDEF",       0x85, 0x00, 0x87);
    ERUS("MOV UNDEF,UNDEF", "UNDEF,UNDEF", 0x85, 0x00, 0x00);

    AERUS(0x17FD, "AJMP  UNDEF", "UNDEF", 0x01, 0x00);
    AERUS(0x17FE, "AJMP  UNDEF", "UNDEF", 0x01, 0x00);
    AERUS(0x17FF, "AJMP  UNDEF", "UNDEF", 0x01, 0x00);
    AERUS(0x17FD, "ACALL UNDEF", "UNDEF", 0x11, 0x00);
    AERUS(0x17FE, "ACALL UNDEF", "UNDEF", 0x11, 0x00);
    AERUS(0x17FF, "ACALL UNDEF", "UNDEF", 0x11, 0x00);
    ERUS("LJMP  UNDEF", "UNDEF", 0x02, 0x00, 0x00);
    ERUS("LCALL UNDEF", "UNDEF", 0x12, 0x00, 0x00);

    TEST("option smart-branch, on");

    AERUS(0x17FD, "AJMP  UNDEF", "UNDEF", 0x02, 0x00, 0x00);
    AERUS(0x17FE, "AJMP  UNDEF", "UNDEF", 0x02, 0x00, 0x00);
    AERUS(0x17FF, "AJMP  UNDEF", "UNDEF", 0x02, 0x00, 0x00);
    AERUS(0x17FD, "ACALL UNDEF", "UNDEF", 0x12, 0x00, 0x00);
    AERUS(0x17FE, "ACALL UNDEF", "UNDEF", 0x12, 0x00, 0x00);
    AERUS(0x17FF, "ACALL UNDEF", "UNDEF", 0x12, 0x00, 0x00);
    ERUS("LJMP  UNDEF", "UNDEF", 0x02, 0x00, 0x00);
    ERUS("LCALL UNDEF", "UNDEF", 0x12, 0x00, 0x00);
}

static void test_data_constant() {
    TEST("DB -128, 255", 0x80, 0xFF);
    TEST(R"(DB 'A', '"')", 0x41, 0x22);
    TEST("DB '9'-'0'",   0x09);
    TEST("DB ''''",      0x27);
    ERRT("DB '''",       MISSING_CLOSING_QUOTE, "'''");
    TEST("DB 'A''B',0",  0x41, 0x27, 0x42, 0x00);
    ERRT("DB 'A''B,0",   MISSING_CLOSING_QUOTE, "'A''B,0");
    TEST("DW -128, 255", 0xFF, 0x80, 0x00, 0xFF);
    TEST("DW 'A''B'",    0x41, 0x27, 0x42, 0x00);
    ERRT("DW 'A''B",     MISSING_CLOSING_QUOTE, "'A''B");
    ERUS("DB 1, UNDEF, 2", "UNDEF, 2", 0x01, 0x00, 0x02);
    ERUS("DW 1, UNDEF, 2", "UNDEF, 2", 0x00, 0x01, 0x00, 0x00, 0x00, 0x02);

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
         "1234H, 5678H, 9ABCH, 0DEF0H, 1234H, 5678H, 9ABCH, 0DEF0H, "
         "1234H, 5678H, 9ABCH, 0DEF0H, 1234H, 5678H, 9ABCH, 0DEF0H, "
         "1234H, 5678H, 9ABCH, 0DEF0H, 1234H, 5678H, 9ABCH, 0DEF0H, "
         "1234H, 5678H, 9ABCH, 0DEF0H, 1234H, 5678H, 9ABCH, 0DEH, "
         "0AAAAH, 0BBBBH",
         NO_MEMORY,
         "0AAAAH, 0BBBBH",
         0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
         0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
         0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
         0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0x00, 0xDE);
}

// clang-format on

void run_tests(const char *cpu) {
    assembler.setCpu(cpu);
    RUN_TEST(test_implied);
    RUN_TEST(test_regs);
    RUN_TEST(test_regr);
    RUN_TEST(test_indirect);
    RUN_TEST(test_immediate);
    RUN_TEST(test_relative);
    RUN_TEST(test_bit_address);
    RUN_TEST(test_direct);
    RUN_TEST(test_jump);
    RUN_TEST(test_comment);
    RUN_TEST(test_undef);
    RUN_TEST(test_data_constant);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

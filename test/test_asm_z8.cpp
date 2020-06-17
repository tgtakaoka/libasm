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

static void set_up() {
    assembler.reset();
    assembler.setCpu("Z8");
}

static void tear_down() {
    symtab.reset();
}

static void test_cpu() {
    asserter.equals(
        "cpu z8", true, assembler.setCpu("z8"));
    asserter.equals(
        "get cpu", "Z8", assembler.getCpu());

    asserter.equals(
        "cpu Z8601", true, assembler.setCpu("Z8601"));
    asserter.equals(
        "get cpu", "Z8", assembler.getCpu());

    asserter.equals(
        "cpu Z86C91", true, assembler.setCpu("Z86C91"));
    asserter.equals(
        "get cpu", "Z86C", assembler.getCpu());
}

static void test_implied() {
    // Z8
    TEST("DI",   0x8F);
    TEST("EI",   0x9F);
    TEST("RET",  0xAF);
    TEST("IRET", 0xBF);
    TEST("RCF",  0xCF);
    TEST("SCF",  0xDF);
    TEST("CCF",  0xEF);
    TEST("NOP",  0xFF);
    ETEST(UNKNOWN_INSTRUCTION, "STOP");
    ETEST(UNKNOWN_INSTRUCTION, "HALT");

    // Z86C
    assembler.setCpu("Z86C");
    TEST("STOP", 0x6F);
    TEST("HALT", 0x7F);
}

static void test_absolute() {
    symtab.intern(0x7E7F, "sym7E7F");
    symtab.intern(0xD7D8, "symD7D8");

    TEST("JP F,0E0FH",    0x0D, 0x0E, 0x0F);
    TEST("JP LT,1E1FH",   0x1D, 0x1E, 0x1F);
    TEST("JP LE,2E2FH",   0x2D, 0x2E, 0x2F);
    TEST("JP ULE,3E3FH",  0x3D, 0x3E, 0x3F);
    TEST("JP OV,4E4FH",   0x4D, 0x4E, 0x4F);
    TEST("JP MI,5E5FH",   0x5D, 0x5E, 0x5F);
    TEST("JP Z,6E6FH",    0x6D, 0x6E, 0x6F);
    TEST("JP C,sym7E7F",  0x7D, 0x7E, 0x7F);
    TEST("JP 8E8FH",      0x8D, 0x8E, 0x8F);
    TEST("JP GE,9E9FH",   0x9D, 0x9E, 0x9F);
    TEST("JP GT,0AEAFH",  0xAD, 0xAE, 0xAF);
    TEST("JP UGT,0BEBFH", 0xBD, 0xBE, 0xBF);
    TEST("JP NOV,0CECFH", 0xCD, 0xCE, 0xCF);
    TEST("JP PL,0DEDFH",  0xDD, 0xDE, 0xDF);
    TEST("JP NZ,0EEEFH",  0xED, 0xEE, 0xEF);
    TEST("JP NC,0FEFFH",  0xFD, 0xFE, 0xFF);
    TEST("CALL symD7D8",  0xD6, 0xD7, 0xD8);
}

static void test_relative() {
    symtab.intern(0x106E, "sym106E");
    symtab.intern(0x0F8E, "sym0F8E");

    ATEST(0x1000, "JR F,100EH",   0x0B, 0x0C);
    ATEST(0x1000, "JR LT,101EH",  0x1B, 0x1C);
    ATEST(0x1000, "JR LE,102EH",  0x2B, 0x2C);
    ATEST(0x1000, "JR ULE,103EH", 0x3B, 0x3C);
    ATEST(0x1000, "JR OV,104EH",  0x4B, 0x4C);
    ATEST(0x1000, "JR MI,105EH",  0x5B, 0x5C);
    ATEST(0x1000, "JR Z,sym106E", 0x6B, 0x6C);
    ATEST(0x1000, "JR C,107EH",   0x7B, 0x7C);
    ATEST(0x1000, "JR sym0F8E",   0x8B, 0x8C);
    ATEST(0x1000, "JR GE,0F9EH",  0x9B, 0x9C);
    ATEST(0x1000, "JR GT,0FAEH",  0xAB, 0xAC);
    ATEST(0x1000, "JR UGT,0FBEH", 0xBB, 0xBC);
    ATEST(0x1000, "JR NOV,0FCEH", 0xCB, 0xCC);
    ATEST(0x1000, "JR PL,0FDEH",  0xDB, 0xDC);
    ATEST(0x1000, "JR NZ,0FEEH",  0xEB, 0xEC);
    ATEST(0x1000, "JR NC,0FFEH",  0xFB, 0xFC);

    ATEST(0x1000, "DJNZ R0,100DH",  0x0A, 0x0B);
    ATEST(0x1000, "DJNZ R1,101DH",  0x1A, 0x1B);
    ATEST(0x1000, "DJNZ R2,102DH",  0x2A, 0x2B);
    ATEST(0x1000, "DJNZ R3,103DH",  0x3A, 0x3B);
    ATEST(0x1000, "DJNZ R4,104DH",  0x4A, 0x4B);
    ATEST(0x1000, "DJNZ R5,105DH",  0x5A, 0x5B);
    ATEST(0x1000, "DJNZ R6,106DH",  0x6A, 0x6B);
    ATEST(0x1000, "DJNZ R7,107DH",  0x7A, 0x7B);
    ATEST(0x1000, "DJNZ R8,0F8DH",  0x8A, 0x8B);
    ATEST(0x1000, "DJNZ R9,0F9DH",  0x9A, 0x9B);
    ATEST(0x1000, "DJNZ R10,0FADH", 0xAA, 0xAB);
    ATEST(0x1000, "DJNZ R11,0FBDH", 0xBA, 0xBB);
    ATEST(0x1000, "DJNZ R12,0FCDH", 0xCA, 0xCB);
    ATEST(0x1000, "DJNZ R13,0FDDH", 0xDA, 0xDB);
    ATEST(0x1000, "DJNZ R14,0FEDH", 0xEA, 0xEB);
    ATEST(0x1000, "DJNZ R15,0FFDH", 0xFA, 0xFB);
}

static void test_operand_in_opcode() {
    TEST("LD R0,>09H",  0x08, 0x09);
    TEST("LD R0,R9",    0x08, 0xE9);
    TEST("LD R1,>0FH",  0x18, 0x0F);
    TEST("LD R2,R0",    0x28, 0xE0);
    TEST("LD R3,10H",   0x38, 0x10);
    TEST("LD R4,49H",   0x48, 0x49);
    TEST("LD R5,59H",   0x58, 0x59);
    TEST("LD R6,69H",   0x68, 0x69);
    TEST("LD R7,79H",   0x78, 0x79);
    TEST("LD R8,89H",   0x88, 0x89);
    TEST("LD R9,99H",   0x98, 0x99);
    TEST("LD R10,0A9H", 0xA8, 0xA9);
    TEST("LD R11,0B9H", 0xB8, 0xB9);
    TEST("LD R12,0C9H", 0xC8, 0xC9);
    TEST("LD R13,0D9H", 0xD8, 0xD9);
    TEST("LD R14,0E9H", 0xE8, 0xE9);
    TEST("LD R15,0F9H", 0xF8, 0xF9);

    TEST("LD >0AH,R0",  0x09, 0x0A);
    TEST("LD >0FH,R1",  0x19, 0x0F);
    TEST("LD >00H,R2",  0x29, 0x00);
    TEST("LD 10H,R3",   0x39, 0x10);
    TEST("LD 4AH,R4",   0x49, 0x4A);
    TEST("LD 5AH,R5",   0x59, 0x5A);
    TEST("LD 6AH,R6",   0x69, 0x6A);
    TEST("LD 7AH,R7",   0x79, 0x7A);
    TEST("LD 0FFH,R8",  0x89, 0xFF);
    TEST("LD 9AH,R9",   0x99, 0x9A);
    TEST("LD 0AAH,R10", 0xA9, 0xAA);
    TEST("LD 0BAH,R11", 0xB9, 0xBA);
    TEST("LD 0CAH,R12", 0xC9, 0xCA);
    TEST("LD 0DAH,R13", 0xD9, 0xDA);
    TEST("LD 0EAH,R14", 0xE9, 0xEA);
    TEST("LD 0FAH,R15", 0xF9, 0xFA);

    TEST("LD R0,#13",    0x0C, 0x0D);
    TEST("LD R1,#0",     0x1C, 0x00);
    TEST("LD R2,#15",    0x2C, 0x0F);
    TEST("LD R3,#10H",   0x3C, 0x10);
    TEST("LD R4,#4DH",   0x4C, 0x4D);
    TEST("LD R5,#5DH",   0x5C, 0x5D);
    TEST("LD R6,#6DH",   0x6C, 0x6D);
    TEST("LD R7,#7DH",   0x7C, 0x7D);
    TEST("LD R8,#8DH",   0x8C, 0x8D);
    TEST("LD R9,#9DH",   0x9C, 0x9D);
    TEST("LD R10,#0ADH", 0xAC, 0xAD);
    TEST("LD R11,#0BDH", 0xBC, 0xBD);
    TEST("LD R12,#0CDH", 0xCC, 0xCD);
    TEST("LD R13,#0DDH", 0xDC, 0xDD);
    TEST("LD R14,#0EDH", 0xEC, 0xED);
    TEST("LD R15,#0FDH", 0xFC, 0xFD);

    TEST("INC R0",  0x0E);
    TEST("INC R1",  0x1E);
    TEST("INC R2",  0x2E);
    TEST("INC R3",  0x3E);
    TEST("INC R4",  0x4E);
    TEST("INC R5",  0x5E);
    TEST("INC R6",  0x6E);
    TEST("INC R7",  0x7E);
    TEST("INC R8",  0x8E);
    TEST("INC R9",  0x9E);
    TEST("INC R10", 0xAE);
    TEST("INC R11", 0xBE);
    TEST("INC R12", 0xCE);
    TEST("INC R13", 0xDE);
    TEST("INC R14", 0xEE);
    TEST("INC R15", 0xFE);
}

static void test_one_operand() {
    TEST("DEC R1",   0x00, 0xE1);
    TEST("DEC @15H", 0x01, 0x15);
    TEST("DEC @R2",  0x01, 0xE2);

    TEST("RLC 11H",  0x10, 0x11);
    TEST("RLC R1",   0x10, 0xE1);
    TEST("RLC @12H", 0x11, 0x12);
    TEST("RLC @R2",  0x11, 0xE2);

    TEST("INC 21H",  0x20, 0x21);
    TEST("INC R1",   0x1E);
    TEST("INC @22H", 0x21, 0x22);
    TEST("INC @R2",  0x21, 0xE2);

    TEST("DA 41H",  0x40, 0x41);
    TEST("DA R1",   0x40, 0xE1);
    TEST("DA @42H", 0x41, 0x42);
    TEST("DA @R2",  0x41, 0xE2);

    TEST("POP 0FCH", 0x50, 0xFC);
    TEST("POP R1",   0x50, 0xE1);
    TEST("POP @52H", 0x51, 0x52);
    TEST("POP @R2",  0x51, 0xE2);

    TEST("COM 61H",  0x60, 0x61);
    TEST("COM R1",   0x60, 0xE1);
    TEST("COM @62H", 0x61, 0x62);
    TEST("COM @R2",  0x61, 0xE2);

    TEST("PUSH 0FFH", 0x70, 0xFF);
    TEST("PUSH R1",   0x70, 0xE1);
    TEST("PUSH @72H", 0x71, 0x72);
    TEST("PUSH @R2",  0x71, 0xE2);

    TEST("DECW 81H",  0x80, 0x81);
    TEST("DECW R1",   0x80, 0xE1);
    TEST("DECW @82H", 0x81, 0x82);
    TEST("DECW @R2",  0x81, 0xE2);

    TEST("RL 91H",  0x90, 0x91);
    TEST("RL R1",   0x90, 0xE1);
    TEST("RL @92H", 0x91, 0x92);
    TEST("RL @R2",  0x91, 0xE2);

    TEST("INCW 0A1H",  0xA0, 0xA1);
    TEST("INCW R1",    0xA0, 0xE1);
    TEST("INCW @0A2H", 0xA1, 0xA2);
    TEST("INCW @R2",   0xA1, 0xE2);

    TEST("CLR 0B1H",  0xB0, 0xB1);
    TEST("CLR R1",    0xB0, 0xE1);
    TEST("CLR @0B2H", 0xB1, 0xB2);
    TEST("CLR @R2",   0xB1, 0xE2);

    TEST("RRC 0C1H",  0xC0, 0xC1);
    TEST("RRC R1",    0xC0, 0xE1);
    TEST("RRC @0C2H", 0xC1, 0xC2);
    TEST("RRC @R2",   0xC1, 0xE2);

    TEST("SRA 0D1H",  0xD0, 0xD1);
    TEST("SRA R1",    0xD0, 0xE1);
    TEST("SRA @0D2H", 0xD1, 0xD2);
    TEST("SRA @R2",   0xD1, 0xE2);

    TEST("RR 0C1H",  0xE0, 0xC1);
    TEST("RR R1",    0xE0, 0xE1);
    TEST("RR @0C2H", 0xE1, 0xC2);
    TEST("RR @R2",   0xE1, 0xE2);

    TEST("SWAP 0CFH",  0xF0, 0xCF);
    TEST("SWAP R1",    0xF0, 0xE1);
    TEST("SWAP @0CFH", 0xF1, 0xCF);
    TEST("SWAP @R2",   0xF1, 0xE2);

    TEST("JP @32H",    0x30, 0x32);
    TEST("JP @RR2",    0x30, 0xE2);
    TEST("CALL @0D6H", 0xD4, 0xD6);
    TEST("CALL @RR2",  0xD4, 0xE2);
    TEST("SRP #30H",   0x31, 0x30);
}

static void test_two_operands() {
    TEST("ADD R0,R3",     0x02, 0x03);
    TEST("ADD R4,@R5",    0x03, 0x45);
    TEST("ADD 78H,56H",   0x04, 0x56, 0x78);
    TEST("ADD R8,56H",    0x04, 0x56, 0xE8);
    TEST("ADD 78H,R6",    0x04, 0xE6, 0x78);
    TEST("ADD >07H,@16H", 0x05, 0x16, 0x07);
    TEST("ADD >07H,@R6",  0x05, 0xE6, 0x07);
    TEST("ADD R7,@16H",   0x05, 0x16, 0xE7);
    TEST("ADD >07H,#8",   0x06, 0x07, 0x08);
    TEST("ADD R7,#8",     0x06, 0xE7, 0x08);
    TEST("ADD @18H,#9",   0x07, 0x18, 0x09);
    TEST("ADD @R8,#9",    0x07, 0xE8, 0x09);

    TEST("ADC R3,R4",     0x12, 0x34);
    TEST("ADC R1,@R4",    0x13, 0x14);
    TEST("ADC 16H,15H",   0x14, 0x15, 0x16);
    TEST("ADC R6,15H",    0x14, 0x15, 0xE6);
    TEST("ADC 16H,R5",    0x14, 0xE5, 0x16);
    TEST("ADC 17H,@16H",  0x15, 0x16, 0x17);
    TEST("ADC 17H,@R6",   0x15, 0xE6, 0x17);
    TEST("ADC R7,@16H",   0x15, 0x16, 0xE7);
    TEST("ADC 17H,#18H",  0x16, 0x17, 0x18);
    TEST("ADC R7,#18H",   0x16, 0xE7, 0x18);
    TEST("ADC @18H,#19H", 0x17, 0x18, 0x19);
    TEST("ADC @R8,#19H",  0x17, 0xE8, 0x19);

    TEST("SUB R2,R3",     0x22, 0x23);
    TEST("SUB R4,@R5",    0x23, 0x45);
    TEST("SUB 26H,25H",   0x24, 0x25, 0x26);
    TEST("SUB R6,25H",    0x24, 0x25, 0xE6);
    TEST("SUB 26H,R5",    0x24, 0xE5, 0x26);
    TEST("SUB 27H,@26H",  0x25, 0x26, 0x27);
    TEST("SUB 27H,@R6",   0x25, 0xE6, 0x27);
    TEST("SUB R7,@26H",   0x25, 0x26, 0xE7);
    TEST("SUB 27H,#28H",  0x26, 0x27, 0x28);
    TEST("SUB R7,#28H",   0x26, 0xE7, 0x28);
    TEST("SUB @28H,#29H", 0x27, 0x28, 0x29);
    TEST("SUB @R8,#29H",  0x27, 0xE8, 0x29);

    TEST("SBC R3,R3",     0x32, 0x33);
    TEST("SBC R3,@R4",    0x33, 0x34);
    TEST("SBC 78H,56H",   0x34, 0x56, 0x78);
    TEST("SBC R8,56H",    0x34, 0x56, 0xE8);
    TEST("SBC 78H,R6",    0x34, 0xE6, 0x78);
    TEST("SBC 37H,@36H",  0x35, 0x36, 0x37);
    TEST("SBC 37H,@R6",   0x35, 0xE6, 0x37);
    TEST("SBC R7,@36H",   0x35, 0x36, 0xE7);
    TEST("SBC 37H,#38H",  0x36, 0x37, 0x38);
    TEST("SBC R7,#38H",   0x36, 0xE7, 0x38);
    TEST("SBC @38H,#39H", 0x37, 0x38, 0x39);
    TEST("SBC @R8,#39H",  0x37, 0xE8, 0x39);

    TEST("OR R4,R3",     0x42, 0x43);
    TEST("OR R4,@R4",    0x43, 0x44);
    TEST("OR 46H,45H",   0x44, 0x45, 0x46);
    TEST("OR R6,45H",    0x44, 0x45, 0xE6);
    TEST("OR 46H,R5",    0x44, 0xE5, 0x46);
    TEST("OR 89H,@67H",  0x45, 0x67, 0x89);
    TEST("OR 89H,@R7",   0x45, 0xE7, 0x89);
    TEST("OR R9,@67H",   0x45, 0x67, 0xE9);
    TEST("OR 47H,#48H",  0x46, 0x47, 0x48);
    TEST("OR R7,#48H",   0x46, 0xE7, 0x48);
    TEST("OR @48H,#49H", 0x47, 0x48, 0x49);
    TEST("OR @R8,#49H",  0x47, 0xE8, 0x49);

    TEST("AND R5,R3",     0x52, 0x53);
    TEST("AND R5,@R4",    0x53, 0x54);
    TEST("AND 56H,55H",   0x54, 0x55, 0x56);
    TEST("AND R6,55H",    0x54, 0x55, 0xE6);
    TEST("AND 56H,R5",    0x54, 0xE5, 0x56);
    TEST("AND 57H,@56H",  0x55, 0x56, 0x57);
    TEST("AND 57H,@R6",   0x55, 0xE6, 0x57);
    TEST("AND R7,@56H",   0x55, 0x56, 0xE7);
    TEST("AND 78H,#9AH",  0x56, 0x78, 0x9A);
    TEST("AND R8,#9AH",   0x56, 0xE8, 0x9A);
    TEST("AND @58H,#59H", 0x57, 0x58, 0x59);
    TEST("AND @R8,#59H",  0x57, 0xE8, 0x59);

    TEST("TCM R6,R3",     0x62, 0x63);
    TEST("TCM R6,@R4",    0x63, 0x64);
    TEST("TCM 66H,65H",   0x64, 0x65, 0x66);
    TEST("TCM R6,65H",    0x64, 0x65, 0xE6);
    TEST("TCM 66H,R5",    0x64, 0xE5, 0x66);
    TEST("TCM 67H,@66H",  0x65, 0x66, 0x67);
    TEST("TCM 67H,@R6",   0x65, 0xE6, 0x67);
    TEST("TCM R7,@66H",   0x65, 0x66, 0xE7);
    TEST("TCM 67H,#68H",  0x66, 0x67, 0x68);
    TEST("TCM R7,#68H",   0x66, 0xE7, 0x68);
    TEST("TCM @89H,#0ABH",0x67, 0x89, 0xAB);
    TEST("TCM @R9,#0ABH", 0x67, 0xE9, 0xAB);

    TEST("TM R7,R3",     0x72, 0x73);
    TEST("TM R7,@R4",    0x73, 0x74);
    TEST("TM 76H,75H",   0x74, 0x75, 0x76);
    TEST("TM R6,75H",    0x74, 0x75, 0xE6);
    TEST("TM 76H,R5",    0x74, 0xE5, 0x76);
    TEST("TM 77H,@76H",  0x75, 0x76, 0x77);
    TEST("TM 77H,@R6",   0x75, 0xE6, 0x77);
    TEST("TM R7,@76H",   0x75, 0x76, 0xE7);
    TEST("TM 77H,#78H",  0x76, 0x77, 0x78);
    TEST("TM R7,#78H",   0x76, 0xE7, 0x78);
    TEST("TM @78H,#79H", 0x77, 0x78, 0x79);
    TEST("TM @R8,#79H",  0x77, 0xE8, 0x79);

    TEST("CP R10,R3",     0xA2, 0xA3);
    TEST("CP R10,@R4",    0xA3, 0xA4);
    TEST("CP 0A6H,0A5H",  0xA4, 0xA5, 0xA6);
    TEST("CP R6,0A5H",    0xA4, 0xA5, 0xE6);
    TEST("CP 0A6H,R5",    0xA4, 0xE5, 0xA6);
    TEST("CP 0A7H,@0A6H", 0xA5, 0xA6, 0xA7);
    TEST("CP 0A7H,@R6",   0xA5, 0xE6, 0xA7);
    TEST("CP R7,@0A6H",   0xA5, 0xA6, 0xE7);
    TEST("CP 0A7H,#0A8H", 0xA6, 0xA7, 0xA8);
    TEST("CP R7,#0A8H",   0xA6, 0xE7, 0xA8);
    TEST("CP @0A8H,#0A9H",0xA7, 0xA8, 0xA9);
    TEST("CP @R8,#0A9H",  0xA7, 0xE8, 0xA9);

    TEST("XOR R11,R3",     0xB2, 0xB3);
    TEST("XOR R11,@R4",    0xB3, 0xB4);
    TEST("XOR 0B6H,0B5H",  0xB4, 0xB5, 0xB6);
    TEST("XOR R6,0B5H",    0xB4, 0xB5, 0xE6);
    TEST("XOR 0B6H,R5",    0xB4, 0xE5, 0xB6);
    TEST("XOR 0B7H,@0B6H", 0xB5, 0xB6, 0xB7);
    TEST("XOR 0B7H,@R6",   0xB5, 0xE6, 0xB7);
    TEST("XOR R7,@0B6H",   0xB5, 0xB6, 0xE7);
    TEST("XOR 0B7H,#0B8H", 0xB6, 0xB7, 0xB8);
    TEST("XOR R7,#0B8H",   0xB6, 0xE7, 0xB8);
    TEST("XOR @0B8H,#0B9H",0xB7, 0xB8, 0xB9);
    TEST("XOR @R8,#0B9H",  0xB7, 0xE8, 0xB9);

    TEST("LD R14,@R4",    0xE3, 0xE4);
    TEST("LD 36H,35H",    0xE4, 0x35, 0x36);
    TEST("LD 37H,@36H",   0xE5, 0x36, 0x37);
    TEST("LD 37H,#38H",   0xE6, 0x37, 0x38);
    TEST("LD @38H,#39H",  0xE7, 0x38, 0x39);
    TEST("LD R6,R5",      0x68, 0xE5);
    TEST("LD R7,@R6",     0xE3, 0x76);
    TEST("LD R7,#0E8H",   0x7C, 0xE8);
    TEST("LD @R8,#0E9H",  0xE7, 0xE8, 0xE9);
    TEST("LD @R15,R4",    0xF3, 0xF4);
    TEST("LD @0C7H,0F0H", 0xF5, 0xF0, 0xC7);
    TEST("LD @R7,0F0H",   0xF5, 0xF0, 0xE7);

    TEST("LDE R7,@RR4",   0x82, 0x74);
    TEST("LDEI @R7,@RR4", 0x83, 0x74);
    TEST("LDE @RR4,R9",   0x92, 0x94);
    TEST("LDEI @RR4,@R9", 0x93, 0x94);

    TEST("LDC R11,@RR4",   0xC2, 0xB4);
    TEST("LDCI @R11,@RR4", 0xC3, 0xB4);
    TEST("LDC @RR4,R13",   0xD2, 0xD4);
    TEST("LDCI @RR4,@R13", 0xD3, 0xD4);
}

static void test_indexed() {
    TEST("LD R12,0C9H(R8)", 0xC7, 0xC8, 0xC9);
    TEST("LD 0D9H(R8),R13", 0xD7, 0xD8, 0xD9);
    ETEST(OVERFLOW_RANGE, "LD R12, -129(R8)");
    TEST(                 "LD R12, -128(R8)", 0xC7, 0xC8, 0x80);
    TEST(                 "LD R12,   -1(R8)", 0xC7, 0xC8, 0xFF);
    TEST(                 "LD R12, +127(R8)", 0xC7, 0xC8, 0x7F);
    TEST(                 "LD R12, +128(R8)", 0xC7, 0xC8, 0x80);
    TEST(                 "LD R12, +255(R8)", 0xC7, 0xC8, 0xFF);
    ETEST(OVERFLOW_RANGE, "LD R12, +256(R8)");
    ETEST(OVERFLOW_RANGE, "LD -129(R8), R12");
    TEST(                 "LD -128(R8), R12", 0xD7, 0xC8, 0x80);
    TEST(                 "LD   -1(R8), R12", 0xD7, 0xC8, 0xFF);
    TEST(                 "LD +127(R8), R12", 0xD7, 0xC8, 0x7F);
    TEST(                 "LD +128(R8), R12", 0xD7, 0xC8, 0x80);
    TEST(                 "LD +255(R8), R12", 0xD7, 0xC8, 0xFF);
    ETEST(OVERFLOW_RANGE, "LD +256(R8), R12");

    symtab.intern(0xC9, "bufC9");
    symtab.intern(-2,   "offm2");

    TEST("LD R12,bufC9(R8)", 0xC7, 0xC8, 0xC9);
    TEST("LD R12,offm2(R8)", 0xC7, 0xC8, 0xFE);
}

static void test_setrp() {
    TEST("SETRP 0");
    TEST("LD  21H,R4",   0x49, 0x21);
    TEST("LD  R4,21H",   0x48, 0x21);
    TEST("LD  21H,#66H", 0xE6, 0x21, 0x66);
    ETEST(UNKNOWN_OPERAND, "LD 21H,33H(R4)"); // LD R,X
    ETEST(UNKNOWN_OPERAND, "LD 33H(R4),21H"); // LD X,R
    TEST("LD  21H,24H",  0xE4, 0x24, 0x21);
    TEST("LD  24H,21H",  0XE4, 0x21, 0x24);
    TEST("LD  21H,@24H", 0xE5, 0x24, 0x21);
    TEST("LD  @21H,24H", 0xF5, 0x24, 0x21);
    TEST("ADD 21H,24H" , 0x04, 0x24, 0x21);
    TEST("ADD 21H,@24H", 0x05, 0x24, 0x21);
    TEST("LDE  R1,@RR4",  0x82, 0x14);
    TEST("LDE  @RR4,R1",  0x92, 0x14);
    TEST("LDEI @R1,@RR4", 0x83, 0x14);
    TEST("LDEI @RR4,@R1", 0x93, 0x14);
    TEST("DJNZ R1,$",     0x1A, 0xFE);
    TEST("JP   @24H",     0x30, 0x24);
    TEST("CALL @24H",     0xD4, 0x24);

    TEST("SETRP 21H & 0F0H");
    TEST("LD  21H,R4",   0x18, 0xE4);
    TEST("LD  R4,21H",   0x48, 0xE1);
    TEST("LD  21H,#66H", 0x1C, 0x66);
    TEST("LD  21H,33H(R4)", 0xC7, 0x14, 0x33); // LD r,X
    TEST("LD  33H(R4),21H", 0xD7, 0x14, 0x33); // LD X,r
    TEST("LD  21H,24H",  0x18, 0xE4);
    TEST("LD  24H,21H",  0X48, 0xE1);
    TEST("LD  21H,@24H", 0xE3, 0x14);
    TEST("LD  @21H,24H", 0xF3, 0x14);
    TEST("ADD 21H,24H",  0x02, 0x14);
    TEST("ADD 21H,@24H", 0x03, 0x14);
    TEST("LDE  21H,@24H",  0x82, 0x14);
    TEST("LDE  @24H,21H",  0x92, 0x14);
    TEST("LDEI @21H,@24H", 0x83, 0x14);
    TEST("LDEI @24H,@21H", 0x93, 0x14);
    TEST("DJNZ 21H,$",     0x1A, 0xFE);
    TEST("JP   @24H",      0x30, 0xE4);
    TEST("CALL @24H",      0xD4, 0xE4);

    TEST(                   "SRP #10H", 0x31, 0x10);
    ETEST(ILLEGAL_CONSTANT, "SRP #11H");
    ETEST(ILLEGAL_CONSTANT, "SRP #12H");
    ETEST(ILLEGAL_CONSTANT, "SRP #13H");
    ETEST(ILLEGAL_CONSTANT, "SRP #14H");
    ETEST(ILLEGAL_CONSTANT, "SRP #15H");
    ETEST(ILLEGAL_CONSTANT, "SRP #16H");
    ETEST(ILLEGAL_CONSTANT, "SRP #17H");
    ETEST(ILLEGAL_CONSTANT, "SRP #18H");
    ETEST(ILLEGAL_CONSTANT, "SRP #19H");
    ETEST(ILLEGAL_CONSTANT, "SRP #1AH");
    ETEST(ILLEGAL_CONSTANT, "SRP #1BH");
    ETEST(ILLEGAL_CONSTANT, "SRP #1CH");
    ETEST(ILLEGAL_CONSTANT, "SRP #1DH");
    ETEST(ILLEGAL_CONSTANT, "SRP #1EH");
    ETEST(ILLEGAL_CONSTANT, "SRP #1FH");
    TEST(                   "SRP #20H", 0x31, 0x20);
}

static void test_comment() {
    TEST(" RET             ; comment", 0xAF);
    TEST(" JP  ULE , 3E3FH ; comment", 0x3D, 0x3E, 0x3F);
    TEST(" JP  8E8FH       ; comment", 0x8D, 0x8E, 0x8F);
    TEST(" LD  R1 , >0FH   ; comment", 0x18, 0x0F);
    TEST(" LD  R2 , R0     ; comment", 0x28, 0xE0);
    TEST(" LD  >0FH , R1   ; comment", 0x19, 0x0F);
    TEST(" LD  R9 , #9DH   ; comment", 0x9C, 0x9D);
    TEST(" INC R0          ; comment", 0x0E);
    TEST(" RLC 11H         ; comment", 0x10, 0x11);
    TEST(" RLC R1          ; comment", 0x10, 0xE1);
    TEST(" RLC @12H        ; comment", 0x11, 0x12);
    TEST(" RLC @R2         ; comment", 0x11, 0xE2);
    TEST(" ADD >07H, @16H  ; comment", 0x05, 0x16, 0x07);
    TEST(" ADD >07H, @R6   ; comment", 0x05, 0xE6, 0x07);
    TEST(" ADD R7 , @16H   ; comment", 0x05, 0x16, 0xE7);
    TEST(" LDC @RR4 , R13  ; comment", 0xD2, 0xD4);
    TEST(" LD  R12 , 0C9H (R8) ; comment", 0xC7, 0xC8, 0xC9);
    TEST(" LD  0D9H (R8) , R13 ; comment", 0xD7, 0xD8, 0xD9);
    ATEST(0x1000, " JR   C , 107EH  ; comment", 0x7B, 0x7C);
    ATEST(0x1000, " DJNZ R2 , 102DH ; comment", 0x2A, 0x2B);
}

static void test_undefined_symbol() {
    ETEST(UNDEFINED_SYMBOL, "JP   C,UNDEF", 0x7D, 0x00, 0x00);
    ETEST(UNDEFINED_SYMBOL, "JP   UNDEF",   0x8D, 0x00, 0x00);
    ETEST(UNDEFINED_SYMBOL, "CALL UNDEF",   0xD6, 0x00, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LD R0,UNDEF",  0x08, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LD UNDEF,R0",  0x09, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LD UNDEF,UNDEF", 0xE4, 0x00, 0x00);
    EATEST(UNDEFINED_SYMBOL, 0x1000, "JR UNDEF",      0x8B, 0xFE);
    EATEST(UNDEFINED_SYMBOL, 0x1000, "JR Z,UNDEF",    0x6B, 0xFE);
    EATEST(UNDEFINED_SYMBOL, 0x1000, "DJNZ R0,UNDEF", 0x0A, 0xFE);
}

static void test_error() {
    ETEST(UNKNOWN_OPERAND, "JP   @11");
    ETEST(UNKNOWN_OPERAND, "CALL @0e5h");
    ETEST(UNKNOWN_OPERAND, "JP   @r0");
    ETEST(UNKNOWN_OPERAND, "CALL @r4");
    TEST(                  "LD >0FH,R1", 0x19, 0x0F);
    ETEST(UNKNOWN_OPERAND, "LD > 0FH,R1");
    TEST(                  "DEC @>15H",  0x01, 0x15);
    ETEST(UNKNOWN_OPERAND, "DEC @ >15H");
    ETEST(UNKNOWN_OPERAND, "DEC @> 15H");
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
    RUN_TEST(test_implied);
    RUN_TEST(test_absolute);
    RUN_TEST(test_relative);
    RUN_TEST(test_operand_in_opcode);
    RUN_TEST(test_one_operand);
    RUN_TEST(test_two_operands);
    RUN_TEST(test_indexed);
    RUN_TEST(test_setrp);
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

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
    assembler.setCpu("Super8");
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
        "cpu Z88C00", true, assembler.setCpu("Z88C00"));
    asserter.equals(
        "get cpu", "Z88", assembler.getCpu());

    asserter.equals(
        "cpu Super8", true, assembler.setCpu("Super8"));
    asserter.equals(
        "get cpu", "Z88", assembler.getCpu());
}

static void test_implied() {
    TEST("NEXT  ", 0x0F);
    TEST("ENTER ", 0x1F);
    TEST("EXIT  ", 0x2F);
    TEST("WFI   ", 0x3F);
    TEST("SB0   ", 0x4F);
    TEST("SB1   ", 0x5F);
    TEST("DI    ", 0x8F);
    TEST("EI    ", 0x9F);
    TEST("RET   ", 0xAF);
    TEST("IRET  ", 0xBF);
    TEST("RCF   ", 0xCF);
    TEST("SCF   ", 0xDF);
    TEST("CCF   ", 0xEF);
    TEST("NOP   ", 0xFF);
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
    TEST("CALL symD7D8",  0xF6, 0xD7, 0xD8);
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

    ATEST(0x1000, "CPIJE  R3,@R12,$-0039H", 0xC2, 0xC3, 0xC4);
    ATEST(0x1000, "CPIJNE R3,@R13,$-0029H", 0xD2, 0xD3, 0xD4);
}

static void test_operand_in_opcode() {
    TEST("LD R0,>09H",  0x08, 0x09);
    TEST("LD R1,>0FH",  0x18, 0x0F);
    TEST("LD R2,>00H",  0x28, 0x00);
    TEST("LD R3,10H",   0x38, 0x10);
    TEST("LD R4,49H",   0x48, 0x49);
    TEST("LD R5,59H",   0x58, 0x59);
    TEST("LD R6,69H",   0x68, 0x69);
    TEST("LD R7,79H",   0x78, 0x79);
    TEST("LD R8,89H",   0x88, 0x89);
    TEST("LD R9,99H",   0x98, 0x99);
    TEST("LD R10,0A9H", 0xA8, 0xA9);
    TEST("LD R11,0B9H", 0xB8, 0xB9);
    TEST("LD R12,R9",   0xC8, 0xC9);
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
    TEST("LD R3,#16",    0x3C, 0x10);
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
    TEST("DEC  >01H",  0x00, 0x01);
    TEST("DEC  R1",    0x00, 0xC1);
    TEST("DEC  @05H",  0x01, 0x05);
    TEST("DEC  @R2",   0x01, 0xC2);

    TEST("RLC  11H",   0x10, 0x11);
    TEST("RLC  R1",    0x10, 0xC1);
    TEST("RLC  @12H",  0x11, 0x12);
    TEST("RLC  @R2",   0x11, 0xC2);

    TEST("INC  21H",  0x20, 0x21);
    TEST("INC  R1",   0x1E);
    TEST("INC  @22H", 0x21, 0x22);
    TEST("INC  @R2",  0x21, 0xC2);

    TEST("DA   41H",  0x40, 0x41);
    TEST("DA   R1",   0x40, 0xC1);
    TEST("DA   @42H", 0x41, 0x42);
    TEST("DA   @R2",  0x41, 0xC2);

    TEST("POP  0FCH", 0x50, 0xFC);
    TEST("POP  R1",   0x50, 0xC1);
    TEST("POP  @52H", 0x51, 0x52);
    TEST("POP  @R2",  0x51, 0xC2);

    TEST("COM  61H",  0x60, 0x61);
    TEST("COM  R1",   0x60, 0xC1);
    TEST("COM  @62H", 0x61, 0x62);
    TEST("COM  @R2",  0x61, 0xC2);

    TEST("PUSH 0FFH", 0x70, 0xFF);
    TEST("PUSH R1",   0x70, 0xC1);
    TEST("PUSH @72H", 0x71, 0x72);
    TEST("PUSH @R2",  0x71, 0xC2);

    TEST("DECW 82H", 0x80, 0x82);
    TEST("DECW RR2", 0x80, 0xC2);
    ETEST(UNKNOWN_OPERAND,  "DECW 81H");
    ETEST(UNDEFINED_SYMBOL, "DECW RR1", 0x80, 0x00);
    TEST("DECW @81H", 0x81, 0x81);
    TEST("DECW @R1",  0x81, 0xC1);

    TEST("RL   91H",  0x90, 0x91);
    TEST("RL   R1",   0x90, 0xC1);
    TEST("RL   @92H", 0x91, 0x92);
    TEST("RL   @R2",  0x91, 0xC2);

    TEST("INCW 0A2H",  0xA0, 0xA2);
    TEST("INCW RR2",   0xA0, 0xC2);
    ETEST(UNKNOWN_OPERAND,  "INCW 0A1H");
    ETEST(UNDEFINED_SYMBOL, "INCW RR1", 0xA0, 0x00);
    TEST("INCW @0A1H", 0xA1, 0xA1);
    TEST("INCW @R1",   0xA1, 0xC1);

    TEST("CLR  0B1H",  0xB0, 0xB1);
    TEST("CLR  R1",    0xB0, 0xC1);
    TEST("CLR  @0B2H", 0xB1, 0xB2);
    TEST("CLR  @R2",   0xB1, 0xC2);

    TEST("RRC  0E1H",  0xC0, 0xE1);
    TEST("RRC  R1",    0xC0, 0xC1);
    TEST("RRC  @0E2H", 0xC1, 0xE2);
    TEST("RRC  @R2",   0xC1, 0xC2);

    TEST("SRA  0D1H",  0xD0, 0xD1);
    TEST("SRA  R1",    0xD0, 0xC1);
    TEST("SRA  @0D2H", 0xD1, 0xD2);
    TEST("SRA  @R2",   0xD1, 0xC2);

    TEST("RR   R1",    0xE0, 0xC1);
    TEST("RR   0E1H",  0xE0, 0xE1);
    TEST("RR   @R2",   0xE1, 0xC2);
    TEST("RR   @0E2H", 0xE1, 0xE2);

    TEST("SWAP 0EFH",  0xF0, 0xEF);
    TEST("SWAP R1",    0xF0, 0xC1);
    TEST("SWAP @0EFH", 0xF1, 0xEF);
    TEST("SWAP @R2",   0xF1, 0xC2);

    TEST("JP   @32H",  0x30, 0x32);
    TEST("JP   @RR2",  0x30, 0xC2);
    TEST("CALL @0D6H", 0xF4, 0xD6);
    TEST("CALL @RR2",  0xF4, 0xC2);
    TEST("CALL #0D6H", 0xD4, 0xD6);
    TEST("SRP  #30H",  0x31, 0x30);
    TEST("SRP0 #30H",  0x31, 0x32);
    TEST("SRP1 #38H",  0x31, 0x39);
}

static void test_two_operands() {
    TEST("ADD R0,R3",     0x02, 0x03);
    TEST("ADD R4,@R5",    0x03, 0x45);
    TEST("ADD 78H,56H",   0x04, 0x56, 0x78);
    TEST("ADD R8,56H",    0x04, 0x56, 0xC8);
    TEST("ADD 78H,R6",    0x04, 0xC6, 0x78);
    TEST("ADD >07H,@06H", 0x05, 0x06, 0x07);
    TEST("ADD >07H,@R6",  0x05, 0xC6, 0x07);
    TEST("ADD R7,@06H",   0x05, 0x06, 0xC7);
    TEST("ADD >07H,#8",   0x06, 0x07, 0x08);
    TEST("ADD R7,#8",     0x06, 0xC7, 0x08);

    TEST("ADC R3,R4",     0x12, 0x34);
    TEST("ADC R1,@R4",    0x13, 0x14);
    TEST("ADC 16H,15H",   0x14, 0x15, 0x16);
    TEST("ADC R6,15H",    0x14, 0x15, 0xC6);
    TEST("ADC 16H,R5",    0x14, 0xC5, 0x16);
    TEST("ADC 17H,@16H",  0x15, 0x16, 0x17);
    TEST("ADC 17H,@R6",   0x15, 0xC6, 0x17);
    TEST("ADC R7,@16H",   0x15, 0x16, 0xC7);
    TEST("ADC 17H,#18H",  0x16, 0x17, 0x18);
    TEST("ADC R7,#18H",   0x16, 0xC7, 0x18);

    TEST("SUB R2,R3",     0x22, 0x23);
    TEST("SUB R4,@R5",    0x23, 0x45);
    TEST("SUB 26H,25H",   0x24, 0x25, 0x26);
    TEST("SUB R6,25H",    0x24, 0x25, 0xC6);
    TEST("SUB 26H,R5",    0x24, 0xC5, 0x26);
    TEST("SUB 27H,@26H",  0x25, 0x26, 0x27);
    TEST("SUB 27H,@R6",   0x25, 0xC6, 0x27);
    TEST("SUB R7,@26H",   0x25, 0x26, 0xC7);
    TEST("SUB 27H,#28H",  0x26, 0x27, 0x28);
    TEST("SUB R7,#28H",   0x26, 0xC7, 0x28);

    TEST("SBC R3,R3",     0x32, 0x33);
    TEST("SBC R3,@R4",    0x33, 0x34);
    TEST("SBC 78H,56H",   0x34, 0x56, 0x78);
    TEST("SBC R8,56H",    0x34, 0x56, 0xC8);
    TEST("SBC 78H,R6",    0x34, 0xC6, 0x78);
    TEST("SBC 37H,@36H",  0x35, 0x36, 0x37);
    TEST("SBC 37H,@R6",   0x35, 0xC6, 0x37);
    TEST("SBC R7,@36H",   0x35, 0x36, 0xC7);
    TEST("SBC 37H,#38H",  0x36, 0x37, 0x38);
    TEST("SBC R7,#38H",   0x36, 0xC7, 0x38);

    TEST("OR  R4,R3",     0x42, 0x43);
    TEST("OR  R4,@R4",    0x43, 0x44);
    TEST("OR  46H,45H",   0x44, 0x45, 0x46);
    TEST("OR  R6,45H",    0x44, 0x45, 0xC6);
    TEST("OR  46H,R5",    0x44, 0xC5, 0x46);
    TEST("OR  89H,@67H",  0x45, 0x67, 0x89);
    TEST("OR  89H,@R7",   0x45, 0xC7, 0x89);
    TEST("OR  R9,@67H",   0x45, 0x67, 0xC9);
    TEST("OR  47H,#48H",  0x46, 0x47, 0x48);
    TEST("OR  R7,#48H",   0x46, 0xC7, 0x48);

    TEST("AND R5,R3",     0x52, 0x53);
    TEST("AND R5,@R4",    0x53, 0x54);
    TEST("AND 56H,55H",   0x54, 0x55, 0x56);
    TEST("AND R6,55H",    0x54, 0x55, 0xC6);
    TEST("AND 56H,R5",    0x54, 0xC5, 0x56);
    TEST("AND 57H,@56H",  0x55, 0x56, 0x57);
    TEST("AND 57H,@R6",   0x55, 0xC6, 0x57);
    TEST("AND R7,@56H",   0x55, 0x56, 0xC7);
    TEST("AND 78H,#9AH",  0x56, 0x78, 0x9A);
    TEST("AND R8,#9AH",   0x56, 0xC8, 0x9A);

    TEST("TCM R6,R3",     0x62, 0x63);
    TEST("TCM R6,@R4",    0x63, 0x64);
    TEST("TCM 66H,65H",   0x64, 0x65, 0x66);
    TEST("TCM R6,65H",    0x64, 0x65, 0xC6);
    TEST("TCM 66H,R5",    0x64, 0xC5, 0x66);
    TEST("TCM 67H,@66H",  0x65, 0x66, 0x67);
    TEST("TCM 67H,@R6",   0x65, 0xC6, 0x67);
    TEST("TCM R7,@66H",   0x65, 0x66, 0xC7);
    TEST("TCM 67H,#68H",  0x66, 0x67, 0x68);
    TEST("TCM R7,#68H",   0x66, 0xC7, 0x68);

    TEST("TM  R7,R3",     0x72, 0x73);
    TEST("TM  R7,@R4",    0x73, 0x74);
    TEST("TM  76H,75H",   0x74, 0x75, 0x76);
    TEST("TM  R6,75H",    0x74, 0x75, 0xC6);
    TEST("TM  76H,R5",    0x74, 0xC5, 0x76);
    TEST("TM  77H,@76H",  0x75, 0x76, 0x77);
    TEST("TM  77H,@R6",   0x75, 0xC6, 0x77);
    TEST("TM  R7,@76H",   0x75, 0x76, 0xC7);
    TEST("TM  77H,#78H",  0x76, 0x77, 0x78);
    TEST("TM  R7,#78H",   0x76, 0xC7, 0x78);

    TEST("CP  R10,R3",    0xA2, 0xA3);
    TEST("CP  R10,@R4",   0xA3, 0xA4);
    TEST("CP  0A6H,0A5H", 0xA4, 0xA5, 0xA6);
    TEST("CP  R6,0A5H",   0xA4, 0xA5, 0xC6);
    TEST("CP  0A6H,R5",   0xA4, 0xC5, 0xA6);
    TEST("CP  0A7H,@0A6H",0xA5, 0xA6, 0xA7);
    TEST("CP  0A7H,@R6",  0xA5, 0xC6, 0xA7);
    TEST("CP  R7,@0A6H",  0xA5, 0xA6, 0xC7);
    TEST("CP  0A7H,#0A8H",0xA6, 0xA7, 0xA8);
    TEST("CP  R7,#0A8H",  0xA6, 0xC7, 0xA8);

    TEST("XOR R11,R3",    0xB2, 0xB3);
    TEST("XOR R11,@R4",   0xB3, 0xB4);
    TEST("XOR 0B6H,0B5H", 0xB4, 0xB5, 0xB6);
    TEST("XOR R6,0B5H",   0xB4, 0xB5, 0xC6);
    TEST("XOR 0B6H,R5",   0xB4, 0xC5, 0xB6);
    TEST("XOR 0B7H,@0B6H",0xB5, 0xB6, 0xB7);
    TEST("XOR 0B7H,@R6",  0xB5, 0xC6, 0xB7);
    TEST("XOR R7,@0B6H",  0xB5, 0xB6, 0xC7);
    TEST("XOR 0B7H,#0B8H",0xB6, 0xB7, 0xB8);
    TEST("XOR R7,#0B8H",  0xB6, 0xC7, 0xB8);

    TEST("DIV  96H,95H",   0x94, 0x95, 0x96);
    TEST("DIV  RR6,R5",    0x94, 0xC5, 0xC6);
    TEST("DIV  98H,@96H",  0x95, 0x96, 0x98);
    TEST("DIV  RR8,@R6",   0x95, 0xC6, 0xC8);
    TEST("DIV  98H,#97H",  0x96, 0x97, 0x98);
    TEST("DIV  RR8,#97H",  0x96, 0x97, 0xC8);
    TEST("MULT 86H,85H",   0x84, 0x85, 0x86);
    TEST("MULT RR6,R5",    0x84, 0xC5, 0xC6);
    TEST("MULT 88H,@86H",  0x85, 0x86, 0x88);
    TEST("MULT RR8,@R6",   0x85, 0xC6, 0xC8);
    TEST("MULT 88H,#87H",  0x86, 0x87, 0x88);
    TEST("MULT RR8,#87H",  0x86, 0x87, 0xC8);

    TEST("LD R12,@R4",    0xC7, 0xC4);
    TEST("LD 36H,35H",    0xE4, 0x35, 0x36);
    TEST("LD 37H,@36H",   0xE5, 0x36, 0x37);
    TEST("LD 37H,#38H",   0xE6, 0x37, 0x38);
    TEST("LD @38H,#39H",  0xD6, 0x38, 0x39);
    TEST("LD R6,R5",      0x68, 0xC5);
    TEST("LD R7,@R6",     0xC7, 0x76);
    TEST("LD R7,#0E8H",   0x7C, 0xE8);
    TEST("LD @R8,#0E9H",  0xD6, 0xC8, 0xE9);
    TEST("LD @R15,R4",    0xD7, 0xF4);
    TEST("LD @R7,0F0H",   0xF5, 0xF0, 0xC7);
    TEST("LD @0E7H,0F0H", 0xF5, 0xF0, 0xE7);

    TEST("LDW 34H,12H",    0xC4, 0x12, 0x34);
    TEST("LDW 34H,@12H",   0xC5, 0x12, 0x34);
    TEST("LDW 12H,#3456H", 0xC6, 0x12, 0x34, 0x56);
    TEST("LDW RR4,RR2",    0xC4, 0xC2, 0xC4);
    TEST("LDW RR4,@R2",    0xC5, 0xC2, 0xC4);
    TEST("LDW RR2,#3456H", 0xC6, 0xC2, 0x34, 0x56);

    TEST("LDC R7,@RR4",    0xC3, 0x74);
    TEST("LDE R7,@RR4",    0xC3, 0x75);
    TEST("LDC @RR4,R13",   0xD3, 0xD4);
    TEST("LDE @RR4,R13",   0xD3, 0xD5);
    TEST("LDC R10,0A2A1H", 0xA7, 0xA0, 0xA1, 0xA2);
    TEST("LDE R10,0A3A2H", 0xA7, 0xA1, 0xA2, 0xA3);
    TEST("LDC 0B2B1H,R11", 0xB7, 0xB0, 0xB1, 0xB2);
    TEST("LDE 0B3B2H,R11", 0xB7, 0xB1, 0xB2, 0xB3);
    TEST("LDCD R15,@RR2",  0xE2, 0xF2);
    TEST("LDED R15,@RR2",  0xE2, 0xF3);
    TEST("LDCI R14,@RR4",  0xE3, 0xE4);
    TEST("LDEI R14,@RR4",  0xE3, 0xE5);
    TEST("LDCPD @RR2,R15", 0xF2, 0xF2);
    TEST("LDEPD @RR2,R15", 0xF2, 0xF3);
    TEST("LDCPI @RR4,R15", 0xF3, 0xF4);
    TEST("LDEPI @RR4,R15", 0xF3, 0xF5);
}

static void test_indexed() {
    TEST("LD  R12,0C9H(R8)",    0x87, 0xC8, 0xC9);
    TEST("LD  0D9H(R8),R13",    0x97, 0xD8, 0xD9);
    ETEST(OVERFLOW_RANGE, "LD R12, -129(R8)");
    TEST(                 "LD R12, -128(R8)", 0x87, 0xC8, 0x80);
    TEST(                 "LD R12,   -1(R8)", 0x87, 0xC8, 0xFF);
    TEST(                 "LD R12, +127(R8)", 0x87, 0xC8, 0x7F);
    TEST(                 "LD R12, +128(R8)", 0x87, 0xC8, 0x80);
    TEST(                 "LD R12, +255(R8)", 0x87, 0xC8, 0xFF);
    ETEST(OVERFLOW_RANGE, "LD R12, +256(R8)");
    ETEST(OVERFLOW_RANGE, "LD -129(R8), R12");
    TEST(                 "LD -128(R8), R12", 0x97, 0xC8, 0x80);
    TEST(                 "LD   -1(R8), R12", 0x97, 0xC8, 0xFF);
    TEST(                 "LD +127(R8), R12", 0x97, 0xC8, 0x7F);
    TEST(                 "LD +128(R8), R12", 0x97, 0xC8, 0x80);
    TEST(                 "LD +255(R8), R12", 0x97, 0xC8, 0xFF);
    ETEST(OVERFLOW_RANGE, "LD +256(R8), R12");

    TEST("LDC R14, -129(RR8)",  0xA7, 0xE8, 0x7F, 0xFF);
    TEST("LDC R14, -128(RR8)",  0xE7, 0xE8, 0x80);
    TEST("LDC R14,   -1(RR8)",  0xE7, 0xE8, 0xFF);
    TEST("LDC R14,0FFFFH(RR8)", 0xA7, 0xE8, 0xFF, 0xFF);
    TEST("LDC R14, +127(RR8)",  0xE7, 0xE8, 0x7F);
    TEST("LDC R14, +128(RR8)",  0xA7, 0xE8, 0x80, 0x00);
    TEST("LDC -129(RR8), R15",  0xB7, 0xF8, 0x7F, 0xFF);
    TEST("LDC -128(RR8), R15",  0xF7, 0xF8, 0x80);
    TEST("LDC   -1(RR8), R15",  0xF7, 0xF8, 0xFF);
    TEST("LDC 0FFFFH(RR8),R15", 0xB7, 0xF8, 0xFF, 0xFF);
    TEST("LDC +127(RR8), R15",  0xF7, 0xF8, 0x7F);
    TEST("LDC +128(RR8), R15",  0xB7, 0xF8, 0x80, 0x00);
    TEST("LDE R14, -129(RR8)",  0xA7, 0xE9, 0x7F, 0xFF);
    TEST("LDE R14, -128(RR8)",  0xE7, 0xE9, 0x80);
    TEST("LDE R14,   -1(RR8)",  0xE7, 0xE9, 0xFF);
    TEST("LDE R14,0FFFFH(RR8)", 0xA7, 0xE9, 0xFF, 0xFF);
    TEST("LDE R14, +127(RR8)",  0xE7, 0xE9, 0x7F);
    TEST("LDE R14, +128(RR8)",  0xA7, 0xE9, 0x80, 0x00);
    TEST("LDE -129(RR8), R15",  0xB7, 0xF9, 0x7F, 0xFF);
    TEST("LDE -128(RR8), R15",  0xF7, 0xF9, 0x80);
    TEST("LDE   -1(RR8), R15",  0xF7, 0xF9, 0xFF);
    TEST("LDE 0FFFFH(RR8),R15", 0xB7, 0xF9, 0xFF, 0xFF);
    TEST("LDE +127(RR8), R15",  0xF7, 0xF9, 0x7F);
    TEST("LDE +128(RR8), R15",  0xB7, 0xF9, 0x80, 0x00);

    symtab.intern(0xC9,   "bufC9");
    symtab.intern(-2,     "offm2");
    symtab.intern(0xFFFE, "symm2");
    symtab.intern(0xABAA, "table");

    TEST("LD  R12,bufC9(R8)",  0x87, 0xC8, 0xC9);
    TEST("LD  R12,offm2(R8)",  0x87, 0xC8, 0xFE);
    TEST("LDE R10,bufC9(RR8)", 0xA7, 0xA9, 0xC9, 0x00);
    TEST("LDE R10,offm2(RR8)", 0xE7, 0xA9, 0xFE);
    TEST("LDE R10,symm2(RR8)", 0xA7, 0xA9, 0xFE, 0xFF);
    TEST("LDE R10,table(RR8)", 0xA7, 0xA9, 0xAA, 0xAB);
}

static void test_bit_operation() {
    TEST("BITC R5,#4",       0x57, 0x58);
    TEST("BITR R7,#4",       0x77, 0x78);
    TEST("BITS R8,#4",       0x77, 0x89);

    TEST("BOR  R0,>09H,#4",  0x07, 0x08, 0x09);
    TEST("BOR  R0,R9,#4",    0x07, 0x08, 0xC9);
    TEST("BOR  0ABH,#4,R8",  0x07, 0x89, 0xAB);
    TEST("BOR  R11,#4,R8",   0x07, 0x89, 0xCB);
    TEST("BCP  R1,19H,#4",   0x17, 0x18, 0x19);
    TEST("BCP  R1,R9,#4",    0x17, 0x18, 0xC9);
    TEST("BXOR R2,29H,#4",   0x27, 0x28, 0x29);
    TEST("BXOR R2,R9,#4",    0x27, 0x28, 0xC9);
    TEST("BXOR 0ABH,#4,R8",  0x27, 0x89, 0xAB);
    TEST("BXOR R11,#4,R8",   0x27, 0x89, 0xCB);
    TEST("LDB  R4,49H,#4",   0x47, 0x48, 0x49);
    TEST("LDB  R4,R9,#4",    0x47, 0x48, 0xC9);
    TEST("LDB  0ABH,#4,R8",  0x47, 0x89, 0xAB);
    TEST("LDB  R11,#4,R8",   0x47, 0x89, 0xCB);
    TEST("BAND R7,89H,#4",   0x67, 0x78, 0x89);
    TEST("BAND R7,R9,#4",    0x67, 0x78, 0xC9);
    TEST("BAND 0ABH,#4,R8",  0x67, 0x89, 0xAB);
    TEST("BAND R11,#4,R8",   0x67, 0x89, 0xCB);

    TEST("BTJRF $+003CH,R3,#4", 0x37, 0x38, 0x39);
    TEST("BTJRT $+003DH,R3,#4", 0x37, 0x39, 0x3A);
}

static void test_setrp() {
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

    TEST(                   "SRP0 #10H", 0x31, 0x12);
    ETEST(ILLEGAL_CONSTANT, "SRP0 #11H");
    ETEST(ILLEGAL_CONSTANT, "SRP0 #12H");
    ETEST(ILLEGAL_CONSTANT, "SRP0 #13H");
    ETEST(ILLEGAL_CONSTANT, "SRP0 #14H");
    ETEST(ILLEGAL_CONSTANT, "SRP0 #15H");
    ETEST(ILLEGAL_CONSTANT, "SRP0 #16H");
    ETEST(ILLEGAL_CONSTANT, "SRP0 #17H");
    TEST(                   "SRP0 #18H", 0x31, 0x1A);

    TEST(                   "SRP1 #10H", 0x31, 0x11);
    ETEST(ILLEGAL_CONSTANT, "SRP1 #11H");
    ETEST(ILLEGAL_CONSTANT, "SRP1 #12H");
    ETEST(ILLEGAL_CONSTANT, "SRP1 #13H");
    ETEST(ILLEGAL_CONSTANT, "SRP1 #14H");
    ETEST(ILLEGAL_CONSTANT, "SRP1 #15H");
    ETEST(ILLEGAL_CONSTANT, "SRP1 #16H");
    ETEST(ILLEGAL_CONSTANT, "SRP1 #17H");
    TEST(                   "SRP1 #18H", 0x31, 0x19);

    TEST("SETRP 0");
    TEST("LD  22H,R10",  0xA9, 0x22);
    TEST("LD  R10,22H",  0xA8, 0x22);
    TEST("LD  22H,2AH",  0xE4, 0x2A, 0x22);
    TEST("LD  2AH,22H",  0XE4, 0x22, 0x2A);
    TEST("LD  22H,@2AH", 0xE5, 0x2A, 0x22);
    TEST("LD  @22H,2AH", 0xF5, 0x2A, 0x22);
    TEST("LD  22H,#66H", 0xE6, 0x22, 0x66);
    TEST("LD  2AH,#66H", 0xE6, 0x2A, 0x66);
    TEST("ADD 22H,2AH" , 0x04, 0x2A, 0x22);
    TEST("ADD 2AH,@22H", 0x05, 0x22, 0x2A);
    ETEST(UNKNOWN_OPERAND, "LD 22H,33H(R10)"); // LD R,dd(r)
    ETEST(UNKNOWN_OPERAND, "LD 33H(22H),R10"); // LD dd(R),r
    TEST("LDC R2,@RR10", 0xC3, 0x2A);
    TEST("LDC @RR10,R2", 0xD3, 0x2A);
    ETEST(UNKNOWN_OPERAND, "DJNZ 22H,$");
    ETEST(UNKNOWN_OPERAND, "DJNZ 24H,$");

    TEST("SETRP 22H & 0F0H");
    TEST("LD  22H,R10",  0x28, 0xCA);
    TEST("LD  R10,22H",  0xA8, 0xC2);
    TEST("LD  22H,2AH",  0x28, 0xCA);
    TEST("LD  2AH,22H",  0XA8, 0xC2);
    TEST("LD  22H,@2AH", 0xC7, 0x2A);
    TEST("LD  @22H,2AH", 0xD7, 0x2A);
    TEST("LD  22H,#66H", 0x2C, 0x66);
    TEST("LD  2AH,#66H", 0xAC, 0x66);
    TEST("ADD 22H,2AH" , 0x02, 0x2A);
    TEST("ADD 2AH,@22H", 0x03, 0xA2);
    TEST("LD  22H,33H(2AH)", 0x87, 0x2A, 0x33); // LD R2,33H(R10)
    TEST("LD  33H(2AH),22H", 0x97, 0x2A, 0x33); // LD 33H(R10),R2
    TEST("LDC 22H,@2AH", 0xC3, 0x2A);
    TEST("LDC @2AH,22H", 0xD3, 0x2A);
    TEST("DJNZ 22H,$",   0x2A, 0xFE);
    TEST("DJNZ 2AH,$",   0xAA, 0xFE);

    TEST("SETRP0 20H");
    TEST("SETRP1 00H");
    TEST("LD  22H,R10",  0x28, 0xCA);
    TEST("LD  R10,22H",  0xA8, 0xC2);
    TEST("LD  22H,2AH",  0x28, 0x2A);
    TEST("LD  2AH,22H",  0X29, 0x2A);
    TEST("LD  22H,@2AH", 0xE5, 0x2A, 0xC2);
    TEST("LD  @22H,2AH", 0xF5, 0x2A, 0xC2);
    TEST("LD  22H,#66H", 0x2C, 0x66);
    TEST("LD  2AH,#66H", 0xE6, 0x2A, 0x66);
    TEST("ADD 22H,2AH" , 0x04, 0x2A, 0xC2);
    TEST("ADD 2AH,@22H", 0x05, 0xC2, 0x2A);
    TEST("LD  22H,33H(R10)", 0x87, 0x2A, 0x33); // LD R2,33H(R10)
    TEST("LD  33H(R10),22H", 0x97, 0x2A, 0x33); // LD 33H(R10),R2
    TEST("LD  R10,33H(22H)", 0x87, 0xA2, 0x33); // LD R10,33H(R2)
    TEST("LD  33H(22H),R10", 0x97, 0xA2, 0x33); // LD 33H(R2),R10
    TEST("LDC 22H,@RR10", 0xC3, 0x2A);
    TEST("LDC @RR10,22H", 0xD3, 0x2A);
    TEST("DJNZ 22H,$",   0x2A, 0xFE);
    ETEST(UNKNOWN_OPERAND, "DJNZ 2AH,$");

    TEST("SETRP0 00H");
    TEST("SETRP1 28H");
    TEST("LD  22H,R10",  0xA9, 0x22);
    TEST("LD  R10,22H",  0xA8, 0x22);
    TEST("LD  22H,2AH",  0xA9, 0x22);
    TEST("LD  2AH,22H",  0XA8, 0x22);
    TEST("LD  22H,@2AH", 0xE5, 0xCA, 0x22);
    TEST("LD  @22H,2AH", 0xF5, 0xCA, 0x22);
    TEST("LD  22H,#66H", 0xE6, 0x22, 0x66);
    TEST("LD  2AH,#66H", 0xAC, 0x66);
    TEST("ADD 22H,2AH" , 0x04, 0xCA, 0x22);
    TEST("ADD 2AH,@22H", 0x05, 0x22, 0xCA);
    TEST("LD  2AH,33H(R2)", 0x87, 0xA2, 0x33); // LD R10,33H(R2)
    TEST("LD  33H(R2),2AH", 0x97, 0xA2, 0x33); // LD 33H(R2),R10
    TEST("LD  R2,33H(2AH)", 0x87, 0x2A, 0x33); // LD R2,33H(R10)
    TEST("LD  33H(2AH),R2", 0x97, 0x2A, 0x33); // LD 33H(R10),R2
    TEST("LDC R2,@2AH", 0xC3, 0x2A);
    TEST("LDC @2AH,R2", 0xD3, 0x2A);
    ETEST(UNKNOWN_OPERAND, "DJNZ 22H,$");
    TEST("DJNZ 2AH,$",   0xAA, 0xFE);
}

static void test_comment() {
    TEST(" RET             ; comment", 0xAF);
    TEST(" JP  ULE , 3E3FH ; comment", 0x3D, 0x3E, 0x3F);
    TEST(" JP  8E8FH       ; comment", 0x8D, 0x8E, 0x8F);
    TEST(" LD  R1 , >0FH   ; comment", 0x18, 0x0F);
    TEST(" LD  R2 , R0     ; comment", 0x28, 0xC0);
    TEST(" LD  >0FH , R1   ; comment", 0x19, 0x0F);
    TEST(" LD  R9 , #9DH   ; comment", 0x9C, 0x9D);
    TEST(" INC R0          ; comment", 0x0E);
    TEST(" RLC 11H         ; comment", 0x10, 0x11);
    TEST(" RLC R1          ; comment", 0x10, 0xC1);
    TEST(" RLC @12H        ; comment", 0x11, 0x12);
    TEST(" RLC @R2         ; comment", 0x11, 0xC2);
    TEST(" ADD >07H, @16H  ; comment", 0x05, 0x16, 0x07);
    TEST(" ADD >07H, @R6   ; comment", 0x05, 0xC6, 0x07);
    TEST(" ADD R7 , @16H   ; comment", 0x05, 0x16, 0xC7);
    TEST(" LDC @RR4 , R13  ; comment", 0xD3, 0xD4);
    TEST(" LD  R12 , 0C9H (R8) ; comment", 0x87, 0xC8, 0xC9);
    TEST(" LD  0D9H (R8) , R13 ; comment", 0x97, 0xD8, 0xD9);
    ATEST(0x1000, " JR   C , 107EH  ; comment", 0x7B, 0x7C);
    ATEST(0x1000, " DJNZ R2 , 102DH ; comment", 0x2A, 0x2B);
}

static void test_undefined_symbol() {
    ETEST(UNDEFINED_SYMBOL, "JP   C,UNDEF", 0x7D, 0x00, 0x00);
    ETEST(UNDEFINED_SYMBOL, "JP   UNDEF",   0x8D, 0x00, 0x00);
    ETEST(UNDEFINED_SYMBOL, "CALL UNDEF",   0xF6, 0x00, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LD R0,UNDEF",  0x08, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LD UNDEF,R0",  0x09, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LD UNDEF,UNDEF", 0xE4, 0x00, 0x00);
    ETEST(UNDEFINED_SYMBOL, "BITS R8,#UNDEF", 0x77, 0x81);
    ETEST(UNDEFINED_SYMBOL, "BXOR R2,R8,#UNDEF",    0x27, 0x20, 0xC8);
    ETEST(UNDEFINED_SYMBOL, "BXOR R2,UNDEF,#4",     0x27, 0x28, 0x00);
    ETEST(UNDEFINED_SYMBOL, "BXOR R2,UNDEF,#UNDEF", 0x27, 0x20, 0x00);
    ETEST(UNDEFINED_SYMBOL, "BXOR R2,#UNDEF,R8",    0x27, 0x81, 0xC2);
    ETEST(UNDEFINED_SYMBOL, "BXOR UNDEF,#5,R8",     0x27, 0x8B, 0x00);
    ETEST(UNDEFINED_SYMBOL, "BXOR UNDEF,#UNDEF,R8", 0x27, 0x81, 0x00);
    EATEST(UNDEFINED_SYMBOL, 0x1000, "JR UNDEF",      0x8B, 0x00);
    EATEST(UNDEFINED_SYMBOL, 0x1000, "JR Z,UNDEF",    0x6B, 0x00);
    EATEST(UNDEFINED_SYMBOL, 0x1000, "DJNZ R0,UNDEF", 0x0A, 0x00);
    EATEST(UNDEFINED_SYMBOL, 0x1000, "CPIJE  R3,@R12,UNDEF", 0xC2, 0xC3, 0x00);
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
    ETEST(ILLEGAL_BIT_NUMBER, "BITS R8,#8");
    ETEST(ILLEGAL_BIT_NUMBER, "BXOR R2,R8,#8");
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
    RUN_TEST(test_bit_operation);
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

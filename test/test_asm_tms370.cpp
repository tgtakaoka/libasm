/*
 * Copyright 2025 Tadashi G. Takaoka
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

#include "asm_tms370.h"
#include "test_asm_helper.h"

using namespace libasm;
using namespace libasm::tms370;
using namespace libasm::test;

AsmTms370 asm370;
Assembler &assembler(asm370);

void set_up() {
    assembler.reset();
}

void tear_down() {
    symtab.reset();
}

// clang-format off
void test_cpu() {
    EQUALS("cpu TMS370", true,   assembler.setCpu("TMS370"));
    EQUALS_P("cpu TMS370", "TMS370", assembler.config().cpu_P());

    EQUALS("cpu TMS370C250", true,   assembler.setCpu("TMS370C250"));
    EQUALS_P("cpu TMS370C250", "TMS370", assembler.config().cpu_P());
}

void test_implied() {
    TEST("DINT",  0xF0, 0x00);
    TEST("EINT",  0xF0, 0x0C);
    TEST("EINTH", 0xF0, 0x04);
    TEST("EINTL", 0xF0, 0x08);
    TEST("IDLE",  0xF6);
    TEST("LDSP",  0xFD);
    TEST("STSP",  0xFE);
    TEST("NOP",   0xFF);
    TEST("POP  ST", 0xFC);
    TEST("PUSH ST", 0xFB);
    TEST("CLRC",  0xB0);
    TEST("SETC",  0xF8);
    TEST("RTI",   0xFA);
    TEST("RTS",   0xF9);

    TEST("TRAP 15", 0xE0);
    TEST("TRAP 14", 0xE1);
    TEST("TRAP 13", 0xE2);
    TEST("TRAP 12", 0xE3);
    TEST("TRAP 11", 0xE4);
    TEST("TRAP 10", 0xE5);
    TEST("TRAP 9",  0xE6);
    TEST("TRAP 8",  0xE7);
    TEST("TRAP 7",  0xE8);
    TEST("TRAP 6",  0xE9);
    TEST("TRAP 5",  0xEA);
    TEST("TRAP 4",  0xEB);
    TEST("TRAP 3",  0xEC);
    TEST("TRAP 2",  0xED);
    TEST("TRAP 1",  0xEE);
    TEST("TRAP 0",  0xEF);
    ERRT("TRAP 16", OVERFLOW_RANGE, "16", 0xE0);
}

void test_single() {
    TEST("CLR A",    0xB5);
    TEST("CLR B",    0xC5);
    TEST("CLR R214", 0xD5, 0xD6);
    TEST("CLR 0D6H", 0xD5, 0xD6);
    ERRT("CLR 100H", OPERAND_NOT_ALLOWED, "100H");

    TEST("DEC A",    0xB2);
    TEST("DEC B",    0xC2);
    TEST("DEC R211", 0xD2, 0xD3);
    TEST("INCW #-2, R220", 0x70, 0xFE, 0xDC);

    TEST("INC A",    0xB3);
    TEST("INC B",    0xC3);
    TEST("INC R212", 0xD3, 0xD4);

    TEST("INV A",    0xB4);
    TEST("INV B",    0xC4);
    TEST("INV R213", 0xD4, 0xD5);

    TEST("COMPL A",    0xBB);
    TEST("COMPL B",    0xCB);
    TEST("COMPL R213", 0xDB, 0xD5);

    TEST("POP A",    0xB9);
    TEST("POP B",    0xC9);
    TEST("POP R218", 0xD9, 0xDA);

    TEST("PUSH A",    0xB8);
    TEST("PUSH B",    0xC8);
    TEST("PUSH R217", 0xD8, 0xD9);

    TEST("RL A",    0xBE);
    TEST("RL B",    0xCE);
    TEST("RL R223", 0xDE, 0xDF);

    TEST("RLC A",    0xBF);
    TEST("RLC B",    0xCF);
    TEST("RLC R224", 0xDF, 0xE0);

    TEST("RR A",    0xBC);
    TEST("RR B",    0xCC);
    TEST("RR R221", 0xDC, 0xDD);

    TEST("RRC A",    0xBD);
    TEST("RRC B",    0xCD);
    TEST("RRC R222", 0xDD, 0xDE);

    TEST("SWAP A",    0xB7);
    TEST("SWAP B",    0xC7);
    TEST("SWAP R216", 0xD7, 0xD8);

    TEST("XCHB A",    0xB6);
    TEST("XCHB B",    0xC6);
    TEST("XCHB R215", 0xD6, 0xD7);
}

void test_register() {
    TEST("ADC B, A",       0x69);
    TEST("ADC A, B",       0x39, 0x00);
    TEST("ADC R26, A",     0x19, 0x1A);
    TEST("ADC R58, B",     0x39, 0x3A);
    TEST("ADC A, R75",     0x49, 0x00, 0x4B);
    TEST("ADC B, R75",     0x49, 0x01, 0x4B);
    TEST("ADC R74, R75",   0x49, 0x4A, 0x4B);
    TEST("ADC #2AH, A",    0x29, 0x2A);
    TEST("ADC #5AH, B",    0x59, 0x5A);
    TEST("ADC #122, R123", 0x79, 0x7A, 0x7B);
    ERRT("ADC #122, 123H", OPERAND_NOT_ALLOWED, "#122, 123H");
    ERRT("ADC #122H, 123", OVERFLOW_RANGE, "#122H, 123", 0x79, 0x22, 0x7B);

    TEST("ADD B, A",       0x68);
    TEST("ADD A, B",       0x38, 0x00);
    TEST("ADD R25, A",     0x18, 0x19);
    TEST("ADD R57, B",     0x38, 0x39);
    TEST("ADD A, R74",     0x48, 0x00, 0x4A);
    TEST("ADD B, R74",     0x48, 0x01, 0x4A);
    TEST("ADD R73, R74",   0x48, 0x49, 0x4A);
    TEST("ADD #11001B, A", 0x28, 0x19);
    TEST("ADD #59H, B",    0x58, 0x59);
    TEST("ADD #-59, B",    0x58, 0xC5);
    TEST("ADD #79H, R122", 0x78, 0x79, 0x7A);

    TEST("AND B, A",       0x63);
    TEST("AND A, B",       0x33, 0x00);
    TEST("AND R20, A",     0x13, 0x14);
    TEST("AND R52, B",     0x33, 0x34);
    TEST("AND A, R69",     0x43, 0x00, 0x45);
    TEST("AND B, R69",     0x43, 0x01, 0x45);
    TEST("AND R68, R69",   0x43, 0x44, 0x45);
    TEST("AND #~24H, A",   0x23, 0xDB);
    TEST("AND #54H, B",    0x53, 0x54);
    TEST("AND #74H, R117", 0x73, 0x74, 0x75);

    TEST("CMP B, A",       0x6D);
    TEST("CMP A, B",       0x3D, 0x00);
    TEST("CMP R30, A",     0x1D, 0x1E);
    TEST("CMP R62, B",     0x3D, 0x3E);
    TEST("CMP A, R79",     0x4D, 0x00, 0x4F);
    TEST("CMP B, R79",     0x4D, 0x01, 0x4F);
    TEST("CMP R78, R79",   0x4D, 0x4E, 0x4F);
    TEST("CMP #2EH, A",    0x2D, 0x2E);
    TEST("CMP #5EH, B",    0x5D, 0x5E);
    TEST("CMP #7EH, R127", 0x7D, 0x7E, 0x7F);

    TEST("DAC B, A",       0x6E);
    TEST("DAC A, B",       0x3E, 0x00);
    TEST("DAC R31, A",     0x1E, 0x1F);
    TEST("DAC R63, B",     0x3E, 0x3F);
    TEST("DAC A, R80",     0x4E, 0x00, 0x50);
    TEST("DAC B, R80",     0x4E, 0x01, 0x50);
    TEST("DAC R79, R80",   0x4E, 0x4F, 0x50);
    TEST("DAC #2FH, A",    0x2E, 0x2F);
    TEST("DAC #5FH, B",    0x5E, 0x5F);
    TEST("DAC #7FH, R128", 0x7E, 0x7F, 0x80);

    TEST("DSB B, A",       0x6F);
    TEST("DSB A, B",       0x3F, 0x00);
    TEST("DSB R32, A",     0x1F, 0x20);
    TEST("DSB R64, B",     0x3F, 0x40);
    TEST("DSB A, R81",     0x4F, 0x00, 0x51);
    TEST("DSB B, R81",     0x4F, 0x01, 0x51);
    TEST("DSB R80, R81",   0x4F, 0x50, 0x51);
    TEST("DSB #30H, A",    0x2F, 0x30);
    TEST("DSB #60H, B",    0x5F, 0x60);
    TEST("DSB #80H, R129", 0x7F, 0x80, 0x81);

    TEST("MOV B, A",       0x62);
    TEST("MOV A, B",       0xC0);
    TEST("MOV R19, A",     0x12, 0x13);
    TEST("MOV R51, B",     0x32, 0x33);
    TEST("MOV A, R209",    0xD0, 0xD1);
    TEST("MOV B, R210",    0xD1, 0xD2);
    TEST("MOV R67, R68",   0x42, 0x43, 0x44);
    TEST("MOV #23H, A",    0x22, 0x23);
    TEST("MOV #53H, B",    0x52, 0x53);
    TEST("MOV #73H, R116", 0x72, 0x73, 0x74);

    TEST("MPY B, A",       0x6C);
    TEST("MPY A, B",       0x3C, 0x00);
    TEST("MPY R29, A",     0x1C, 0x1D);
    TEST("MPY R61, B",     0x3C, 0x3D);
    TEST("MPY A, R78",     0x4C, 0x00, 0x4E);
    TEST("MPY B, R78",     0x4C, 0x01, 0x4E);
    TEST("MPY R77, R78",   0x4C, 0x4D, 0x4E);
    TEST("MPY #2DH, A",    0x2C, 0x2D);
    TEST("MPY #5DH, B",    0x5C, 0x5D);
    TEST("MPY #7DH, R126", 0x7C, 0x7D, 0x7E);

    TEST("OR  B, A",       0x64);
    TEST("OR  A, B",       0x34, 0x00);
    TEST("OR  R21, A",     0x14, 0x15);
    TEST("OR  R53, B",     0x34, 0x35);
    TEST("OR  A, R70",     0x44, 0x00, 0x46);
    TEST("OR  B, R70",     0x44, 0x01, 0x46);
    TEST("OR  R69, R70",   0x44, 0x45, 0x46);
    TEST("OR  #25H, A",    0x24, 0x25);
    TEST("OR  #55H, B",    0x54, 0x55);
    TEST("OR  #75H, R118", 0x74, 0x75, 0x76);

    TEST("SBB B, A",       0x6B);
    TEST("SBB A, B",       0x3B, 0x00);
    TEST("SBB R28, A",     0x1B, 0x1C);
    TEST("SBB R60, B",     0x3B, 0x3C);
    TEST("SBB A, R77",     0x4B, 0x00, 0x4D);
    TEST("SBB B, R77",     0x4B, 0x01, 0x4D);
    TEST("SBB R76, R77",   0x4B, 0x4C, 0x4D);
    TEST("SBB #2CH, A",    0x2B, 0x2C);
    TEST("SBB #5CH, B",    0x5B, 0x5C);
    TEST("SBB #7CH, R125", 0x7B, 0x7C, 0x7D);

    TEST("SUB B, A",       0x6A);
    TEST("SUB A, B",       0x3A, 0x00);
    TEST("SUB R27, A",     0x1A, 0x1B);
    TEST("SUB R59, B",     0x3A, 0x3B);
    TEST("SUB A, R76",     0x4A, 0x00, 0x4C);
    TEST("SUB B, R76",     0x4A, 0x01, 0x4C);
    TEST("SUB R75, R76",   0x4A, 0x4B, 0x4C);
    TEST("SUB #2BH, A",    0x2A, 0x2B);
    TEST("SUB #5BH, B",    0x5A, 0x5B);
    TEST("SUB #7BH, R124", 0x7A, 0x7B, 0x7C);

    TEST("XOR B, A",       0x65);
    TEST("XOR A, B",       0x35, 0x00);
    TEST("XOR R22, A",     0x15, 0x16);
    TEST("XOR R54, B",     0x35, 0x36);
    TEST("XOR A, R71",     0x45, 0x00, 0x47);
    TEST("XOR B, R71",     0x45, 0x01, 0x47);
    TEST("XOR R70, R71",   0x45, 0x46, 0x47);
    TEST("XOR #26H, A",    0x25, 0x26);
    TEST("XOR #56H, B",    0x55, 0x56);
    TEST("XOR #76H, R119", 0x75, 0x76, 0x77);

    TEST("MOVW #898AH, R139",    0x88, 0x89, 0x8A, 0x8B);
    TEST("MOVW #0A9AAH(B),R171", 0xA8, 0xA9, 0xAA, 0xAB);
    TEST("MOVW #-2(R139),R171",  0xF4, 0xE8, 0xFE, 0x8B, 0xAB);
    TEST("MOVW R153, R154",      0x98, 0x99, 0x9A);
}

void test_peripheral() {
    TEST("AND A, P132",    0x83, 0x84);
    TEST("AND A, 132+256", 0x83, 0x84);
    TEST("AND B, P148",    0x93, 0x94);
    TEST("AND #0A4H,P165", 0xA3, 0xA4, 0xA5);

    TEST("MOV P129, A",    0x80, 0x81);
    TEST("MOV P146, B",    0x91, 0x92);
    TEST("MOV P163, R164", 0xA2, 0xA3, 0xA4);
    TEST("MOV A, P131",    0x21, 0x83);
    TEST("MOV B, P147",    0x51, 0x93);
    TEST("MOV #0A3H,P164", 0xF7, 0xA3, 0xA4);

    TEST("OR A, P133",    0x84, 0x85);
    TEST("OR B, P149",    0x94, 0x95);
    TEST("OR #0A5H,P166", 0xA4, 0xA5, 0xA6);

    TEST("XOR A, P134",    0x85, 0x86);
    TEST("XOR B, P150",    0x95, 0x96);
    TEST("XOR #0A6H,P167", 0xA5, 0xA6, 0xA7);
}

void test_single_relative() {
    ATEST(0x1000, "JMP $-29",  0x00, 0xE1);
    ATEST(0x1000, "JMP 1010H", 0x00, 0x0E);
    ATEST(0x1000, "JMP 1020H", 0x00, 0x1E);
    ATEST(0x1100, "JMP 1120H", 0x00, 0x1E);
    ATEST(0x1000, "JN  $-28",  0x01, 0xE2);
    ATEST(0x1000, "JZ  $-27",  0x02, 0xE3);
    ATEST(0x1000, "JC  $-26",  0x03, 0xE4);
    ATEST(0x1000, "JP  $-25",  0x04, 0xE5);
    ATEST(0x1000, "JPZ $-24",  0x05, 0xE6);
    ATEST(0x1000, "JNZ $-23",  0x06, 0xE7);
    ATEST(0x1000, "JNC $-22",  0x07, 0xE8);

    symtab.intern(0x1040, "a40");
    symtab.intern(0x1040, "b40");
    symtab.intern(0x1040, "rr40");
    symtab.intern(0x1040, "pa40");
    symtab.intern(0x1040, "st40");

    ATEST(0x1000, "JMP a40",  0x00, 0x3E);
    ATEST(0x1000, "JMP b40",  0x00, 0x3E);
    ATEST(0x1000, "JMP rr40", 0x00, 0x3E);
    ATEST(0x1000, "JMP pa40", 0x00, 0x3E);
    ATEST(0x1000, "JMP st40", 0x00, 0x3E);

    ATEST(0x1000, "DJNZ A, $-67",    0xBA, 0xBB);
    ATEST(0x1000, "DJNZ B, $-51",    0xCA, 0xCB);
    ATEST(0x1000, "DJNZ R219, $-33", 0xDA, 0xDB, 0xDC);
}

void test_dual_relative() {
    ATEST(0x1000, "BTJO B, A, $+105",       0x66, 0x67);
    ATEST(0x1000, "BTJO R23, A, $+27",      0x16, 0x17, 0x18);
    ATEST(0x1000, "BTJO R55, B, $+59",      0x36, 0x37, 0x38);
    ATEST(0x1000, "BTJO R71, R72, $+77",    0x46, 0x47, 0x48, 0x49);
    ATEST(0x1000, "BTJO #27H, A, $+43",     0x26, 0x27, 0x28);
    ATEST(0x1000, "BTJO #57H, B, $+91"   ,  0x56, 0x57, 0x58);
    ATEST(0x1000, "BTJO #77H, R120, $+125", 0x76, 0x77, 0x78, 0x79);

    ATEST(0x1000, "BTJZ B, A, $+106",       0x67, 0x68);
    ATEST(0x1000, "BTJZ R24, A, $+28",      0x17, 0x18, 0x19);
    ATEST(0x1000, "BTJZ R56, B, $+60",      0x37, 0x38, 0x39);
    ATEST(0x1000, "BTJZ R72, R73, $+78",    0x47, 0x48, 0x49, 0x4A);
    ATEST(0x1000, "BTJZ #28H, A, $+44",     0x27, 0x28, 0x29);
    ATEST(0x1000, "BTJZ #58H, B, $+92"   ,  0x57, 0x58, 0x59);
    ATEST(0x1000, "BTJZ #78H, R121, $+126", 0x77, 0x78, 0x79, 0x7A);

    ATEST(0x1000, "BTJO A, P135, $-117",   0x86, 0x87, 0x88);
    ATEST(0x1000, "BTJO B, P151, $-101",   0x96, 0x97, 0x98);
    ATEST(0x1000, "BTJO #0A7H,P168, $-83", 0xA6, 0xA7, 0xA8, 0xA9);

    ATEST(0x1000, "BTJZ A, P136, $-116",   0x87, 0x88, 0x89);
    ATEST(0x1000, "BTJZ B, P152, $-100",   0x97, 0x98, 0x99);
    ATEST(0x1000, "BTJZ #0A8H,P169, $-82", 0xA7, 0xA8, 0xA9, 0xAA);
}

void test_extended() {
    TEST("BR 8D8EH",     0x8C, 0x8D, 0x8E);
    TEST("BR @R157",     0x9C, 0x9D);
    TEST("BR @9DH",      0x9C, 0x9D);
    TEST("BR 0ADAEH(B)", 0xAC, 0xAD, 0xAE);
    TEST("BR -2(R10)",   0xF4, 0xEC, 0xFE, 0x0A);

    ATEST(0xF000, "JMPL 7A8EH", 0x89, 0x8A, 0x8B);
    TEST("JMPL @R154",          0x99, 0x9A);
    TEST("JMPL $+3+0AAABH(B)",  0xA9, 0xAA, 0xAB);
    TEST("JMPL -2(R10)",        0xF4, 0xE9, 0xFE, 0x0A);

    TEST("CALL 8F90H",     0x8E, 0x8F, 0x90);
    TEST("CALL @R159",     0x9E, 0x9F);
    TEST("CALL 0AFB0H(B)", 0xAE, 0xAF, 0xB0);
    TEST("CALL -2(R10)",   0xF4, 0xEE, 0xFE, 0x0A);

    ATEST(0xF000, "CALLR 8094H", 0x8F, 0x90, 0x91);
    TEST("CALLR @R160",          0x9F, 0xA0);
    TEST("CALLR $+3+0B0B1H(B)",  0xAF, 0xB0, 0xB1);
    TEST("CALLR -2(R10)",        0xF4, 0xEF, 0xFE, 0x0A);

    TEST("CMP 8E8FH, A",     0x8D, 0x8E, 0x8F);
    TEST("CMP @R158, A",     0x9D, 0x9E);
    TEST("CMP 0AEAFH(B), A", 0xAD, 0xAE, 0xAF);
    TEST("CMP -2(SP), A",    0xF3, 0xFE);
    TEST("CMP -2(R10), A",   0xF4, 0xED, 0xFE, 0x0A);

    TEST("MOV 8B8CH, A",     0x8A, 0x8B, 0x8C);
    TEST("MOV @R155, A",     0x9A, 0x9B);
    TEST("MOV 0ABACH(B), A", 0xAA, 0xAB, 0xAC);
    TEST("MOV -2(SP), A",    0xF1, 0xFE);
    TEST("MOV -2(R10), A",   0xF4, 0xEA, 0xFE, 0x0A);

    TEST("MOV A, 8C8DH",     0x8B, 0x8C, 0x8D);
    TEST("MOV A, @R156",     0x9B, 0x9C);
    TEST("MOV A, 0ACADH(B)", 0xAB, 0xAC, 0xAD);
    TEST("MOV A, -2(SP)",    0xF2, 0xFE);
    TEST("MOV A, -2(R10)",   0xF4, 0xEB, 0xFE, 0x0A);
}

void test_error() {
    ERRT("CLR R214X ; comment", UNDEFINED_SYMBOL, "R214X ; comment", 0xD5, 0x00);
    ERRT("CLR R000  ; comment", UNDEFINED_SYMBOL, "R000  ; comment", 0xD5, 0x00);
    ERRT("CLR R256  ; comment", UNDEFINED_SYMBOL, "R256  ; comment", 0xD5, 0x00);
    ERRT("CLR R     ; comment", UNDEFINED_SYMBOL, "R     ; comment", 0xD5, 0x00);

    ERRT("AND A, P132X", UNDEFINED_SYMBOL, "P132X", 0x43, 0x00, 0x00);
    ERRT("AND A, P000",  UNDEFINED_SYMBOL, "P000",  0x43, 0x00, 0x00);
    ERRT("AND A, P256",  UNDEFINED_SYMBOL, "P256",  0x43, 0x00, 0x00);
    ERRT("AND A, P",     UNDEFINED_SYMBOL, "P",     0x43, 0x00, 0x00);
    ERRT("AND A, > 184", NOT_AN_EXPECTED,  "> 184");

    ERRT("MOVW 0A9AAH(A), R171",  OPERAND_NOT_ALLOWED, "A), R171");
    ERRT("MOVW 0A9AAH(R3), R171", OPERAND_NOT_ALLOWED, "0A9AAH(R3), R171");
    ERRT("MOVW 0A9AAH(P3), R171", OPERAND_NOT_ALLOWED, "P3), R171");

    ERRT("BR 0ADAEH(A)",  OPERAND_NOT_ALLOWED, "A)");
    ERRT("BR 0ADAEH(R4)", OVERFLOW_RANGE, "0ADAEH(R4)", 0xF4, 0xEC, 0xAE, 0x04);
    ERRT("BR 0ADAEH(P4)", OPERAND_NOT_ALLOWED, "P4)");
}

void test_comment() {
    COMM("CLRC    ; comment", "; comment", 0xB0);
    COMM("CLR 0D6H; comment", "; comment", 0xD5, 0xD6);

    COMM("ADC #122, R123  ; comment", "; comment", 0x79, 0x7A, 0x7B);
    COMM("ADC # 122, R123 ; comment", "; comment", 0x79, 0x7A, 0x7B);

    COMM("MOVW #0A9AAH(B), R171     ; comment", "; comment", 0xA8, 0xA9, 0xAA, 0xAB);
    COMM("MOVW #0A9AAH (B), R171    ; comment", "; comment", 0xA8, 0xA9, 0xAA, 0xAB);
    COMM("MOVW #0A9AAH( B), R171    ; comment", "; comment", 0xA8, 0xA9, 0xAA, 0xAB);
    COMM("MOVW #0A9AAH(B ), R171    ; comment", "; comment", 0xA8, 0xA9, 0xAA, 0xAB);
    COMM("MOVW # 0A9AAH ( B ), R171 ; comment", "; comment", 0xA8, 0xA9, 0xAA, 0xAB);

    COMM("CALL 8F90H  ; comment", "; comment", 0x8E, 0x8F, 0x90);
    COMM("CALL @R159  ; comment", "; comment", 0x9E, 0x9F);
    COMM("CALL @ R159 ; comment", "; comment", 0x9E, 0x9F);
    COMM("CALL @159   ; comment", "; comment", 0x9E, 0x9F);
    COMM("CALL @ 159  ; comment", "; comment", 0x9E, 0x9F);

    COMM("CALL 0AFB0H(B)    ; comment", "; comment", 0xAE, 0xAF, 0xB0);
    COMM("CALL 0AFB0H (B)   ; comment", "; comment", 0xAE, 0xAF, 0xB0);
    COMM("CALL 0AFB0H( B)   ; comment", "; comment", 0xAE, 0xAF, 0xB0);
    COMM("CALL 0AFB0H(B )   ; comment", "; comment", 0xAE, 0xAF, 0xB0);
    COMM("CALL 0AFB0H ( B ) ; comment", "; comment", 0xAE, 0xAF, 0xB0);

    COMM(".byte   -128, 255 ; comment",  "; comment", 0x80, 0xFF);
    COMM(R"(.string "TEXT"  ; comment)", "; comment", 0x54, 0x45, 0x58, 0x54);
    COMM(".word   -128, 255 ; comment",  "; comment", 0xFF, 0x80, 0x00, 0xFF);
}

void test_undef() {
    ERUS("TRAP UNDEF", "UNDEF", 0xEF);
    ERUS("CLR  UNDEF", "UNDEF", 0xD5, 0x00);

    ERUS("MOV UNDEF, A",      "UNDEF, A",     0x12, 0x00);
    ERUS("MOV UNDEF, B",      "UNDEF, B",     0x32, 0x00);
    ERUS("MOV A, UNDEF",      "UNDEF",        0xD0, 0x00);
    ERUS("MOV B, UNDEF",      "UNDEF",        0xD1, 0x00);
    ERUS("MOV UNDEF, R75",    "UNDEF, R75",   0x42, 0x00, 0x4B);
    ERUS("MOV R74, UNDEF",    "UNDEF",        0x42, 0x4A, 0x00);
    ERUS("MOV UNDEF, UNDEF",  "UNDEF, UNDEF", 0x42, 0x00, 0x00);
    ERUS("MOV #UNDEF, A",     "UNDEF, A",     0x22, 0x00);
    ERUS("MOV #UNDEF, B",     "UNDEF, B",     0x52, 0x00);
    ERUS("MOV #UNDEF, R75",   "UNDEF, R75",   0x72, 0x00, 0x4B);
    ERUS("MOV #UNDEF, UNDEF", "UNDEF, UNDEF", 0x72, 0x00, 0x00);

    ERUS("MOVW #UNDEF, R139",     "UNDEF, R139",     0x88, 0x00, 0x00, 0x8B);
    ERUS("MOVW #898AH, UNDEF",    "UNDEF",           0x88, 0x89, 0x8A, 0x00);
    ERUS("MOVW #UNDEF, UNDEF",    "UNDEF, UNDEF",    0x88, 0x00, 0x00, 0x00);
    ERUS("MOVW #UNDEF(B), R171",  "UNDEF(B), R171",  0xA8, 0x00, 0x00, 0xAB);
    ERUS("MOVW #0A9AAH(B),UNDEF", "UNDEF",           0xA8, 0xA9, 0xAA, 0x00);
    ERUS("MOVW #UNDEF(B), UNDEF", "UNDEF(B), UNDEF", 0xA8, 0x00, 0x00, 0x00);
}

void test_data_constant() {
    TEST(".BYTE -128, 255", 0x80, 0xFF);
    TEST(R"(.BYTE 'A', '"')", 0x41, 0x22);
    TEST(".BYTE '9'-'0'",   0x09);
    TEST(".BYTE ''''",      0x27);
    ERRT(R"(.BYTE """)",       MISSING_CLOSING_DQUOTE, R"(""")");
    TEST(R"(.BYTE "A""B",0)",  0x41, 0x22, 0x42, 0x00);
    ERRT(R"(.BYTE "A""B,0)",   MISSING_CLOSING_DQUOTE, R"("A""B,0)");
    TEST(".WORD -128, 255", 0xFF, 0x80, 0x00, 0xFF);
    TEST(".WORD 'X'",       0x00, 0x58);
    TEST(".WORD 'X'+0",     0x00, 0x58);
    TEST(R"(.WORD "A""B")", 0x41, 0x22, 0x42, 0x00);
    ERRT(R"(.WORD "A""B)",  MISSING_CLOSING_DQUOTE, R"("A""B)");
    TEST(".STRING 'X'",       0x58);
    TEST(R"(.STRING "A""B'C")", 0x41, 0x22, 0x42, 0x27, 0x43);
    ERRT(R"(.STRING "A""B'C)",  MISSING_CLOSING_DQUOTE, R"("A""B'C)");
    ERUS(".BYTE 1, UNDEF, 2", "UNDEF, 2", 0x01, 0x00, 0x02);
    ERUS(".WORD 1, UNDEF, 2", "UNDEF, 2", 0x00, 0x01, 0x00, 0x00, 0x00, 0x02);

    ERRT(R"(.STRING ")"
         "1234567890" "1234567890" "1234567890" "1234567890" "1234567890" "1234567890"
         R"(1234567890")",
         NO_MEMORY,
         R"(567890")",
         0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30,
         0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30,
         0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30,
         0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30,
         0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30,
         0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30,
         0x31, 0x32, 0x33, 0x34);

    ERRT(".WORD "
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
    RUN_TEST(test_single);
    RUN_TEST(test_register);
    RUN_TEST(test_peripheral);
    RUN_TEST(test_single_relative);
    RUN_TEST(test_dual_relative);
    RUN_TEST(test_extended);
    RUN_TEST(test_error);
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

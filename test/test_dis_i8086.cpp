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

#include "dis_i8086.h"
#include "test_dis_helper.h"

using namespace libasm;
using namespace libasm::i8086;
using namespace libasm::test;

DisI8086 dis8086;
Disassembler &disassembler(dis8086);

static void set_up() {
    disassembler.reset();
}

static void tear_down() {
    symtab.reset();
}

// clang-format off
static void test_cpu() {
    EQUALS("cpu 8086", true,   disassembler.setCpu("8086"));
    EQUALS("cpu 8086", "8086", disassembler.getCpu());

    EQUALS("cpu i8086", true,   disassembler.setCpu("i8086"));
    EQUALS("cpu i8086", "8086", disassembler.getCpu());
}

static void test_data_transfer() {
    TEST(MOV, "AL,CL",            0x88, 0310);
    TEST(MOV, "DL,BL",            0x88, 0332);
    TEST(MOV, "AH,CH",            0x88, 0354);
    TEST(MOV, "[SI],DH",          0x88, 0064);
    TEST(MOV, "[SI+0],DH",        0x88, 0164, 0x00);
    TEST(MOV, "[1234H],BH",       0x88, 0076, 0x34, 0x12);
    TEST(MOV, "[DI+52],AL",       0x88, 0105, 0x34);
    TEST(MOV, "[BP+1234H],CL",    0x88, 0216, 0x34, 0x12);
    TEST(MOV, "[BP+0],CL",        0x88, 0116, 0x00);
    TEST(MOV, "[BX+SI],DL",       0x88, 0020);
    TEST(MOV, "[BX+DI+52],BL",    0x88, 0131, 0x34);
    TEST(MOV, "[BP+SI+1234H],AH", 0x88, 0242, 0x34, 0x12);
    TEST(MOV, "DH,[SI]",          0x8A, 0064);
    TEST(MOV, "BH,[1234H]",       0x8A, 0076, 0x34, 0x12);
    TEST(MOV, "AL,[DI-52]",       0x8A, 0105, 0xCC);
    TEST(MOV, "CL,[BP+1234H]",    0x8A, 0216, 0x34, 0x12);
    TEST(MOV, "DL,[BX+SI]",       0x8A, 0020);
    TEST(MOV, "BL,[BX+DI-52]",    0x8A, 0131, 0xCC);
    TEST(MOV, "AH,[BP+SI+1234H]", 0x8A, 0242, 0x34, 0x12);
    TEST(MOV, "AX,CX",            0x89, 0310);
    TEST(MOV, "DX,BX",            0x89, 0332);
    TEST(MOV, "SP,BP",            0x89, 0354);
    TEST(MOV, "[SI],SI",          0x89, 0064);
    TEST(MOV, "[1234H],DI",       0x89, 0076, 0x34, 0x12);
    TEST(MOV, "[DI-52],AX",       0x89, 0105, 0xCC);
    TEST(MOV, "[BP+1234H],CX",    0x89, 0216, 0x34, 0x12);
    TEST(MOV, "[BX+SI],DX",       0x89, 0020);
    TEST(MOV, "[BX+DI+52],BX",    0x89, 0131, 0x34);
    TEST(MOV, "[BP+SI+1234H],SP", 0x89, 0242, 0x34, 0x12);
    TEST(MOV, "SI,[SI]",          0x8B, 0064);
    TEST(MOV, "DI,[1234H]",       0x8B, 0076, 0x34, 0x12);
    TEST(MOV, "AX,[DI+52]",       0x8B, 0105, 0x34);
    TEST(MOV, "CX,[BP+1234H]",    0x8B, 0216, 0x34, 0x12);
    TEST(MOV, "DX,[BX+SI]",       0x8B, 0020);
    TEST(MOV, "BX,[BX+DI+52]",    0x8B, 0131, 0x34);
    TEST(MOV, "SP,[BP+SI+1234H]", 0x8B, 0242, 0x34, 0x12);

    TEST(MOV, "AL,56H",                       0xC6, 0300, 0x56);
    TEST(MOV, "BYTE PTR [SI],56H",            0xC6, 0004, 0x56);
    TEST(MOV, "BYTE PTR [89ABH],56H",         0xC6, 0006, 0xAB, 0x89, 0x56);
    TEST(MOV, "BYTE PTR [DI-52],56H",         0xC6, 0105, 0xCC, 0x56);
    TEST(MOV, "BYTE PTR [BP+89ABH],56H",      0xC6, 0206, 0xAB, 0x89, 0x56);
    TEST(MOV, "BYTE PTR [BX+SI],56H",         0xC6, 0000, 0x56);
    TEST(MOV, "BYTE PTR [BX+DI+52],56H",      0xC6, 0101, 0x34, 0x56);
    TEST(MOV, "BYTE PTR [BP+SI+1234H],56H",   0xC6, 0202, 0x34, 0x12, 0x56);
    TEST(MOV, "AX,5678H",                     0xC7, 0300, 0x78, 0x56);
    TEST(MOV, "WORD PTR [SI],5678H",          0xC7, 0004, 0x78, 0x56);
    TEST(MOV, "WORD PTR [1234H],5678H",       0xC7, 0006, 0x34, 0x12, 0x78, 0x56);
    TEST(MOV, "WORD PTR [DI+52],5678H",       0xC7, 0105, 0x34, 0x78, 0x56);
    TEST(MOV, "WORD PTR [BP+1234H],5678H",    0xC7, 0206, 0x34, 0x12, 0x78, 0x56);
    TEST(MOV, "WORD PTR [BX+SI],5678H",       0xC7, 0000, 0x78, 0x56);
    TEST(MOV, "WORD PTR [BX+DI-52],5678H",    0xC7, 0101, 0xCC, 0x78, 0x56);
    TEST(MOV, "WORD PTR [BP+SI+89ABH],5678H", 0xC7, 0202, 0xAB, 0x89, 0x78, 0x56);

    TEST(MOV, "AL,56H",   0260, 0x56);
    TEST(MOV, "CH,56H",   0265, 0x56);
    TEST(MOV, "AX,5678H", 0270, 0x78, 0x56);
    TEST(MOV, "BP,5678H", 0275, 0x78, 0x56);

    TEST(MOV, "AL,[1234H]", 0xA0, 0x34, 0x12);
    TEST(MOV, "AX,[1234H]", 0xA1, 0x34, 0x12);
    TEST(MOV, "[1234H],AL", 0xA2, 0x34, 0x12);
    TEST(MOV, "[1234H],AX", 0xA3, 0x34, 0x12);

    TEST(MOV, "AX,ES",              0x8C, 0300);
    TEST(MOV, "[SI],CS",            0x8C, 0014);
    TEST(MOV, "[89ABH],SS",         0x8C, 0026, 0xAB, 0x89);
    TEST(MOV, "[DI-52],DS",         0x8C, 0135, 0xCC);
    TEST(MOV, "[BP+89ABH],ES",      0x8C, 0206, 0xAB, 0x89);
    TEST(MOV, "[BX+SI],CS",         0x8C, 0010);
    TEST(MOV, "[BX+DI+52],SS",      0x8C, 0121, 0x34);
    TEST(MOV, "[BP+SI+1234H],DS",   0x8C, 0232, 0x34, 0x12);
    TEST(MOV, "ES,AX",              0x8E, 0300);
    TEST(MOV, "CS,[SI]",            0x8E, 0014);
    TEST(MOV, "SS,[89ABH]",         0x8E, 0026, 0xAB, 0x89);
    TEST(MOV, "DS,[DI-52]",         0x8E, 0135, 0xCC);
    TEST(MOV, "ES,[BP+89ABH]",      0x8E, 0206, 0xAB, 0x89);
    TEST(MOV, "CS,[BX+SI]",         0x8E, 0010);
    TEST(MOV, "SS,[BX+DI+52]",      0x8E, 0121, 0x34);
    TEST(MOV, "DS,[BP+SI+1234H]",   0x8E, 0232, 0x34, 0x12);

    TEST(PUSH, "BP",            0xFF, 0365);
    TEST(PUSH, "[SI]",          0xFF, 0064);
    TEST(PUSH, "[1234H]",       0xFF, 0066, 0x34, 0x12);
    TEST(PUSH, "[DI-52]",       0xFF, 0165, 0xCC);
    TEST(PUSH, "[BP+1234H]",    0xFF, 0266, 0x34, 0x12);
    TEST(PUSH, "[BX+SI]",       0xFF, 0060);
    TEST(PUSH, "[BX+DI+52]",    0xFF, 0161, 0x34);
    TEST(PUSH, "[BP+SI+1234H]", 0xFF, 0262, 0x34, 0x12);
    TEST(PUSH, "BP",            0125);
    TEST(PUSH, "ES",            0006);
    TEST(PUSH, "CS",            0016);
    TEST(PUSH, "SS",            0026);
    TEST(PUSH, "DS",            0036);

    TEST(POP, "BP",            0x8F, 0305);
    TEST(POP, "[SI]",          0x8F, 0004);
    TEST(POP, "[1234H]",       0x8F, 0006, 0x34, 0x12);
    TEST(POP, "[DI-52]",       0x8F, 0105, 0xCC);
    TEST(POP, "[BP+1234H]",    0x8F, 0206, 0x34, 0x12);
    TEST(POP, "[BX+SI]",       0x8F, 0000);
    TEST(POP, "[BX+DI+52]",    0x8F, 0101, 0x34);
    TEST(POP, "[BP+SI+1234H]", 0x8F, 0202, 0x34, 0x12);
    TEST(POP, "BP",            0135);
    TEST(POP, "ES",            0007);
    ERRR(POP, "CS",            0017);
    TEST(POP, "SS",            0027);
    TEST(POP, "DS",            0037);

    TEST(XCHG, "AL,CH",            0x86, 0305);
    TEST(XCHG, "CL,[SI]",          0x86, 0014);
    TEST(XCHG, "DL,[1234H]",       0x86, 0026, 0x34, 0x12);
    TEST(XCHG, "BL,[DI-52]",       0x86, 0135, 0xCC);
    TEST(XCHG, "AH,[BP+1234H]",    0x86, 0246, 0x34, 0x12);
    TEST(XCHG, "CH,[BX+SI]",       0x86, 0050);
    TEST(XCHG, "DH,[BX+DI+52]",    0x86, 0161, 0x34);
    TEST(XCHG, "BH,[BP+SI+1234H]", 0x86, 0272, 0x34, 0x12);
    TEST(XCHG, "BP,AX",            0x87, 0350);
    TEST(XCHG, "CX,[SI]",          0x87, 0014);
    TEST(XCHG, "DX,[1234H]",       0x87, 0026, 0x34, 0x12);
    TEST(XCHG, "BX,[DI-52]",       0x87, 0135, 0xCC);
    TEST(XCHG, "SP,[BP+1234H]",    0x87, 0246, 0x34, 0x12);
    TEST(XCHG, "BP,[BX+SI]",       0x87, 0050);
    TEST(XCHG, "SI,[BX+DI+52]",    0x87, 0161, 0x34);
    TEST(XCHG, "DI,[BP+SI+1234H]", 0x87, 0272, 0x34, 0x12);
    TEST(XCHG, "AX,BP",            0225);

    TEST(IN,   "AL,34H", 0xE4, 0x34);
    TEST(IN,   "AL,DX",  0xEC);
    TEST(IN,   "AX,34H", 0xE5, 0x34);
    TEST(IN,   "AX,DX",  0xED);

    TEST(OUT,  "34H,AL", 0xE6, 0x34);
    TEST(OUT,  "DX,AL",  0xEE);
    TEST(OUT,  "34H,AX", 0xE7, 0x34);
    TEST(OUT,  "DX,AX",  0xEF);

    TEST(XLAT, "", 0xD7);

    TEST(LEA,  "CX,[SI]",          0x8D, 0014);
    TEST(LEA,  "DX,[1234H]",       0x8D, 0026, 0x34, 0x12);
    TEST(LEA,  "BX,[DI-52]",       0x8D, 0135, 0xCC);
    TEST(LEA,  "SP,[BP+1234H]",    0x8D, 0246, 0x34, 0x12);
    TEST(LEA,  "BP,[BX+SI]",       0x8D, 0050);
    TEST(LEA,  "SI,[BX+DI+52]",    0x8D, 0161, 0x34);
    TEST(LEA,  "DI,[BP+SI+1234H]", 0x8D, 0272, 0x34, 0x12);

    TEST(LDS,  "CX,[SI]",          0xC5, 0014);
    TEST(LDS,  "DX,[1234H]",       0xC5, 0026, 0x34, 0x12);
    TEST(LDS,  "BX,[DI-52]",       0xC5, 0135, 0xCC);
    TEST(LDS,  "SP,[BP+1234H]",    0xC5, 0246, 0x34, 0x12);
    TEST(LDS,  "BP,[BX+SI]",       0xC5, 0050);
    TEST(LDS,  "SI,[BX+DI+52]",    0xC5, 0161, 0x34);
    TEST(LDS,  "DI,[BP+SI+1234H]", 0xC5, 0272, 0x34, 0x12);

    TEST(LES,  "CX,[SI]",          0xC4, 0014);
    TEST(LES,  "DX,[1234H]",       0xC4, 0026, 0x34, 0x12);
    TEST(LES,  "BX,[DI-52]",       0xC4, 0135, 0xCC);
    TEST(LES,  "SP,[BP+1234H]",    0xC4, 0246, 0x34, 0x12);
    TEST(LES,  "BP,[BX+SI]",       0xC4, 0050);
    TEST(LES,  "SI,[BX+DI+52]",    0xC4, 0161, 0x34);
    TEST(LES,  "DI,[BP+SI+1234H]", 0xC4, 0272, 0x34, 0x12);

    TEST(LAHF,  "", 0x9F);
    TEST(SAHF,  "", 0x9E);
    TEST(PUSHF, "", 0x9C);
    TEST(POPF,  "", 0x9D);
}

static void test_arithmetic() {
    TEST(ADD, "AL,CL",            0x00, 0310);
    TEST(ADD, "DL,BL",            0x00, 0332);
    TEST(ADD, "AH,CH",            0x00, 0354);
    TEST(ADD, "[SI],DH",          0x00, 0064);
    TEST(ADD, "[1234H],BH",       0x00, 0076, 0x34, 0x12);
    TEST(ADD, "[DI+52],AL",       0x00, 0105, 0x34);
    TEST(ADD, "[BP+1234H],CL",    0x00, 0216, 0x34, 0x12);
    TEST(ADD, "[BX+SI],DL",       0x00, 0020);
    TEST(ADD, "[BX+DI+52],BL",    0x00, 0131, 0x34);
    TEST(ADD, "[BP+SI+1234H],AH", 0x00, 0242, 0x34, 0x12);
    TEST(ADD, "DH,[SI]",          0x02, 0064);
    TEST(ADD, "BH,[1234H]",       0x02, 0076, 0x34, 0x12);
    TEST(ADD, "AL,[DI-52]",       0x02, 0105, 0xCC);
    TEST(ADD, "CL,[BP+1234H]",    0x02, 0216, 0x34, 0x12);
    TEST(ADD, "DL,[BX+SI]",       0x02, 0020);
    TEST(ADD, "BL,[BX+DI-52]",    0x02, 0131, 0xCC);
    TEST(ADD, "AH,[BP+SI+1234H]", 0x02, 0242, 0x34, 0x12);
    TEST(ADD, "AX,CX",            0x01, 0310);
    TEST(ADD, "DX,BX",            0x01, 0332);
    TEST(ADD, "SP,BP",            0x01, 0354);
    TEST(ADD, "[SI],SI",          0x01, 0064);
    TEST(ADD, "[1234H],DI",       0x01, 0076, 0x34, 0x12);
    TEST(ADD, "[DI-52],AX",       0x01, 0105, 0xCC);
    TEST(ADD, "[BP+1234H],CX",    0x01, 0216, 0x34, 0x12);
    TEST(ADD, "[BX+SI],DX",       0x01, 0020);
    TEST(ADD, "[BX+DI+52],BX",    0x01, 0131, 0x34);
    TEST(ADD, "[BP+SI+1234H],SP", 0x01, 0242, 0x34, 0x12);
    TEST(ADD, "SI,[SI]",          0x03, 0064);
    TEST(ADD, "DI,[1234H]",       0x03, 0076, 0x34, 0x12);
    TEST(ADD, "AX,[DI+52]",       0x03, 0105, 0x34);
    TEST(ADD, "CX,[BP+1234H]",    0x03, 0216, 0x34, 0x12);
    TEST(ADD, "DX,[BX+SI]",       0x03, 0020);
    TEST(ADD, "BX,[BX+DI+52]",    0x03, 0131, 0x34);
    TEST(ADD, "SP,[BP+SI+1234H]", 0x03, 0242, 0x34, 0x12);

    TEST(ADD, "AL,56H",                       0x80, 0300, 0x56);
    TEST(ADD, "CL,56H",                       0x80, 0301, 0x56);
    TEST(ADD, "BYTE PTR [SI],56H",            0x80, 0004, 0x56);
    TEST(ADD, "BYTE PTR [89ABH],56H",         0x80, 0006, 0xAB, 0x89, 0x56);
    TEST(ADD, "BYTE PTR [DI-52],56H",         0x80, 0105, 0xCC, 0x56);
    TEST(ADD, "BYTE PTR [BP+89ABH],56H",      0x80, 0206, 0xAB, 0x89, 0x56);
    TEST(ADD, "BYTE PTR [BX+SI],56H",         0x80, 0000, 0x56);
    TEST(ADD, "BYTE PTR [BX+DI+52],56H",      0x80, 0101, 0x34, 0x56);
    TEST(ADD, "BYTE PTR [BP+SI+1234H],56H",   0x80, 0202, 0x34, 0x12, 0x56);
    TEST(ADD, "AX,5678H",                     0x81, 0300, 0x78, 0x56);
    TEST(ADD, "CX,5678H",                     0x81, 0301, 0x78, 0x56);
    TEST(ADD, "WORD PTR [SI],5678H",          0x81, 0004, 0x78, 0x56);
    TEST(ADD, "WORD PTR [1234H],5678H",       0x81, 0006, 0x34, 0x12, 0x78, 0x56);
    TEST(ADD, "WORD PTR [DI+52],5678H",       0x81, 0105, 0x34, 0x78, 0x56);
    TEST(ADD, "WORD PTR [BP+1234H],5678H",    0x81, 0206, 0x34, 0x12, 0x78, 0x56);
    TEST(ADD, "WORD PTR [BX+SI],5678H",       0x81, 0000, 0x78, 0x56);
    TEST(ADD, "WORD PTR [BX+DI-52],5678H",    0x81, 0101, 0xCC, 0x78, 0x56);
    TEST(ADD, "WORD PTR [BP+SI+89ABH],5678H", 0x81, 0202, 0xAB, 0x89, 0x78, 0x56);
    TEST(ADD, "AX,56H",                       0x83, 0300, 0x56);
    TEST(ADD, "AX,-16",                       0x83, 0300, 0xF0);
    TEST(ADD, "CX,56H",                       0x83, 0301, 0x56);
    TEST(ADD, "CX,-16",                       0x83, 0301, 0xF0);
    TEST(ADD, "WORD PTR [SI],56H",            0x83, 0004, 0x56);
    TEST(ADD, "WORD PTR [1234H],56H",         0x83, 0006, 0x34, 0x12, 0x56);
    TEST(ADD, "WORD PTR [DI+52],56H",         0x83, 0105, 0x34, 0x56);
    TEST(ADD, "WORD PTR [BP+1234H],56H",      0x83, 0206, 0x34, 0x12, 0x56);
    TEST(ADD, "WORD PTR [BX+SI],56H",         0x83, 0000, 0x56);
    TEST(ADD, "WORD PTR [BX+DI-52],56H",      0x83, 0101, 0xCC, 0x56);
    TEST(ADD, "WORD PTR [BP+SI+89ABH],56H",   0x83, 0202, 0xAB, 0x89, 0x56);

    TEST(ADD, "AL,56H",   0x04, 0x56);
    TEST(ADD, "AX,5678H", 0x05, 0x78, 0x56);

    TEST(ADC, "AL,CL",            0x10, 0310);
    TEST(ADC, "DL,BL",            0x10, 0332);
    TEST(ADC, "AH,CH",            0x10, 0354);
    TEST(ADC, "[SI],DH",          0x10, 0064);
    TEST(ADC, "[1234H],BH",       0x10, 0076, 0x34, 0x12);
    TEST(ADC, "[DI+52],AL",       0x10, 0105, 0x34);
    TEST(ADC, "[BP+1234H],CL",    0x10, 0216, 0x34, 0x12);
    TEST(ADC, "[BX+SI],DL",       0x10, 0020);
    TEST(ADC, "[BX+DI+52],BL",    0x10, 0131, 0x34);
    TEST(ADC, "[BP+SI+1234H],AH", 0x10, 0242, 0x34, 0x12);
    TEST(ADC, "DH,[SI]",          0x12, 0064);
    TEST(ADC, "BH,[1234H]",       0x12, 0076, 0x34, 0x12);
    TEST(ADC, "AL,[DI-52]",       0x12, 0105, 0xCC);
    TEST(ADC, "CL,[BP+1234H]",    0x12, 0216, 0x34, 0x12);
    TEST(ADC, "DL,[BX+SI]",       0x12, 0020);
    TEST(ADC, "BL,[BX+DI-52]",    0x12, 0131, 0xCC);
    TEST(ADC, "AH,[BP+SI+1234H]", 0x12, 0242, 0x34, 0x12);
    TEST(ADC, "AX,CX",            0x11, 0310);
    TEST(ADC, "DX,BX",            0x11, 0332);
    TEST(ADC, "SP,BP",            0x11, 0354);
    TEST(ADC, "[SI],SI",          0x11, 0064);
    TEST(ADC, "[1234H],DI",       0x11, 0076, 0x34, 0x12);
    TEST(ADC, "[DI-52],AX",       0x11, 0105, 0xCC);
    TEST(ADC, "[BP+1234H],CX",    0x11, 0216, 0x34, 0x12);
    TEST(ADC, "[BX+SI],DX",       0x11, 0020);
    TEST(ADC, "[BX+DI+52],BX",    0x11, 0131, 0x34);
    TEST(ADC, "[BP+SI+1234H],SP", 0x11, 0242, 0x34, 0x12);
    TEST(ADC, "SI,[SI]",          0x13, 0064);
    TEST(ADC, "DI,[1234H]",       0x13, 0076, 0x34, 0x12);
    TEST(ADC, "AX,[DI+52]",       0x13, 0105, 0x34);
    TEST(ADC, "CX,[BP+1234H]",    0x13, 0216, 0x34, 0x12);
    TEST(ADC, "DX,[BX+SI]",       0x13, 0020);
    TEST(ADC, "BX,[BX+DI+52]",    0x13, 0131, 0x34);
    TEST(ADC, "SP,[BP+SI+1234H]", 0x13, 0242, 0x34, 0x12);

    TEST(ADC, "AL,56H",                       0x80, 0320, 0x56);
    TEST(ADC, "CL,56H",                       0x80, 0321, 0x56);
    TEST(ADC, "BYTE PTR [SI],56H",            0x80, 0024, 0x56);
    TEST(ADC, "BYTE PTR [89ABH],56H",         0x80, 0026, 0xAB, 0x89, 0x56);
    TEST(ADC, "BYTE PTR [DI-52],56H",         0x80, 0125, 0xCC, 0x56);
    TEST(ADC, "BYTE PTR [BP+89ABH],56H",      0x80, 0226, 0xAB, 0x89, 0x56);
    TEST(ADC, "BYTE PTR [BX+SI],56H",         0x80, 0020, 0x56);
    TEST(ADC, "BYTE PTR [BX+DI+52],56H",      0x80, 0121, 0x34, 0x56);
    TEST(ADC, "BYTE PTR [BP+SI+1234H],56H",   0x80, 0222, 0x34, 0x12, 0x56);
    TEST(ADC, "AX,5678H",                     0x81, 0320, 0x78, 0x56);
    TEST(ADC, "CX,5678H",                     0x81, 0321, 0x78, 0x56);
    TEST(ADC, "WORD PTR [SI],5678H",          0x81, 0024, 0x78, 0x56);
    TEST(ADC, "WORD PTR [1234H],5678H",       0x81, 0026, 0x34, 0x12, 0x78, 0x56);
    TEST(ADC, "WORD PTR [DI+52],5678H",       0x81, 0125, 0x34, 0x78, 0x56);
    TEST(ADC, "WORD PTR [BP+1234H],5678H",    0x81, 0226, 0x34, 0x12, 0x78, 0x56);
    TEST(ADC, "WORD PTR [BX+SI],5678H",       0x81, 0020, 0x78, 0x56);
    TEST(ADC, "WORD PTR [BX+DI-52],5678H",    0x81, 0121, 0xCC, 0x78, 0x56);
    TEST(ADC, "WORD PTR [BP+SI+89ABH],5678H", 0x81, 0222, 0xAB, 0x89, 0x78, 0x56);
    TEST(ADC, "AX,56H",                       0x83, 0320, 0x56);
    TEST(ADC, "AX,-16",                       0x83, 0320, 0xF0);
    TEST(ADC, "CX,56H",                       0x83, 0321, 0x56);
    TEST(ADC, "CX,-16",                       0x83, 0321, 0xF0);
    TEST(ADC, "WORD PTR [SI],56H",            0x83, 0024, 0x56);
    TEST(ADC, "WORD PTR [1234H],56H",         0x83, 0026, 0x34, 0x12, 0x56);
    TEST(ADC, "WORD PTR [DI+52],56H",         0x83, 0125, 0x34, 0x56);
    TEST(ADC, "WORD PTR [BP+1234H],56H",      0x83, 0226, 0x34, 0x12, 0x56);
    TEST(ADC, "WORD PTR [BX+SI],56H",         0x83, 0020, 0x56);
    TEST(ADC, "WORD PTR [BX+DI-52],56H",      0x83, 0121, 0xCC, 0x56);
    TEST(ADC, "WORD PTR [BP+SI+89ABH],56H",   0x83, 0222, 0xAB, 0x89, 0x56);

    TEST(ADC, "AL,56H",   0x14, 0x56);
    TEST(ADC, "AX,5678H", 0x15, 0x78, 0x56);

    TEST(INC, "CH",                     0xFE, 0305);
    TEST(INC, "BYTE PTR [SI]",          0xFE, 0004);
    TEST(INC, "BYTE PTR [1234H]",       0xFE, 0006, 0x34, 0x12);
    TEST(INC, "BYTE PTR [DI-52]",       0xFE, 0105, 0xCC);
    TEST(INC, "BYTE PTR [BP+1234H]",    0xFE, 0206, 0x34, 0x12);
    TEST(INC, "BYTE PTR [BX+SI]",       0xFE, 0000);
    TEST(INC, "BYTE PTR [BX+DI+52]",    0xFE, 0101, 0x34);
    TEST(INC, "BYTE PTR [BP+SI+1234H]", 0xFE, 0202, 0x34, 0x12);
    TEST(INC, "BP",                     0xFF, 0305);
    TEST(INC, "WORD PTR [SI]",          0xFF, 0004);
    TEST(INC, "WORD PTR [1234H]",       0xFF, 0006, 0x34, 0x12);
    TEST(INC, "WORD PTR [DI-52]",       0xFF, 0105, 0xCC);
    TEST(INC, "WORD PTR [BP+1234H]",    0xFF, 0206, 0x34, 0x12);
    TEST(INC, "WORD PTR [BX+SI]",       0xFF, 0000);
    TEST(INC, "WORD PTR [BX+DI+52]",    0xFF, 0101, 0x34);
    TEST(INC, "WORD PTR [BP+SI+1234H]", 0xFF, 0202, 0x34, 0x12);

    TEST(INC, "BP", 0105);

    TEST(AAA, "", 0x37);
    TEST(DAA, "", 0x27);

    TEST(SUB, "AL,CL",            0x28, 0310);
    TEST(SUB, "DL,BL",            0x28, 0332);
    TEST(SUB, "AH,CH",            0x28, 0354);
    TEST(SUB, "[SI],DH",          0x28, 0064);
    TEST(SUB, "[1234H],BH",       0x28, 0076, 0x34, 0x12);
    TEST(SUB, "[DI+52],AL",       0x28, 0105, 0x34);
    TEST(SUB, "[BP+1234H],CL",    0x28, 0216, 0x34, 0x12);
    TEST(SUB, "[BX+SI],DL",       0x28, 0020);
    TEST(SUB, "[BX+DI+52],BL",    0x28, 0131, 0x34);
    TEST(SUB, "[BP+SI+1234H],AH", 0x28, 0242, 0x34, 0x12);
    TEST(SUB, "DH,[SI]",          0x2A, 0064);
    TEST(SUB, "BH,[1234H]",       0x2A, 0076, 0x34, 0x12);
    TEST(SUB, "AL,[DI-52]",       0x2A, 0105, 0xCC);
    TEST(SUB, "CL,[BP+1234H]",    0x2A, 0216, 0x34, 0x12);
    TEST(SUB, "DL,[BX+SI]",       0x2A, 0020);
    TEST(SUB, "BL,[BX+DI-52]",    0x2A, 0131, 0xCC);
    TEST(SUB, "AH,[BP+SI+1234H]", 0x2A, 0242, 0x34, 0x12);
    TEST(SUB, "AX,CX",            0x29, 0310);
    TEST(SUB, "DX,BX",            0x29, 0332);
    TEST(SUB, "SP,BP",            0x29, 0354);
    TEST(SUB, "[SI],SI",          0x29, 0064);
    TEST(SUB, "[1234H],DI",       0x29, 0076, 0x34, 0x12);
    TEST(SUB, "[DI-52],AX",       0x29, 0105, 0xCC);
    TEST(SUB, "[BP+1234H],CX",    0x29, 0216, 0x34, 0x12);
    TEST(SUB, "[BX+SI],DX",       0x29, 0020);
    TEST(SUB, "[BX+DI+52],BX",    0x29, 0131, 0x34);
    TEST(SUB, "[BP+SI+1234H],SP", 0x29, 0242, 0x34, 0x12);
    TEST(SUB, "SI,[SI]",          0x2B, 0064);
    TEST(SUB, "DI,[1234H]",       0x2B, 0076, 0x34, 0x12);
    TEST(SUB, "AX,[DI+52]",       0x2B, 0105, 0x34);
    TEST(SUB, "CX,[BP+1234H]",    0x2B, 0216, 0x34, 0x12);
    TEST(SUB, "DX,[BX+SI]",       0x2B, 0020);
    TEST(SUB, "BX,[BX+DI+52]",    0x2B, 0131, 0x34);
    TEST(SUB, "SP,[BP+SI+1234H]", 0x2B, 0242, 0x34, 0x12);

    TEST(SUB, "AL,56H",                       0x80, 0350, 0x56);
    TEST(SUB, "CL,56H",                       0x80, 0351, 0x56);
    TEST(SUB, "BYTE PTR [SI],56H",            0x80, 0054, 0x56);
    TEST(SUB, "BYTE PTR [89ABH],56H",         0x80, 0056, 0xAB, 0x89, 0x56);
    TEST(SUB, "BYTE PTR [DI-52],56H",         0x80, 0155, 0xCC, 0x56);
    TEST(SUB, "BYTE PTR [BP+89ABH],56H",      0x80, 0256, 0xAB, 0x89, 0x56);
    TEST(SUB, "BYTE PTR [BX+SI],56H",         0x80, 0050, 0x56);
    TEST(SUB, "BYTE PTR [BX+DI+52],56H",      0x80, 0151, 0x34, 0x56);
    TEST(SUB, "BYTE PTR [BP+SI+1234H],56H",   0x80, 0252, 0x34, 0x12, 0x56);
    TEST(SUB, "AX,5678H",                     0x81, 0350, 0x78, 0x56);
    TEST(SUB, "CX,5678H",                     0x81, 0351, 0x78, 0x56);
    TEST(SUB, "WORD PTR [SI],5678H",          0x81, 0054, 0x78, 0x56);
    TEST(SUB, "WORD PTR [1234H],5678H",       0x81, 0056, 0x34, 0x12, 0x78, 0x56);
    TEST(SUB, "WORD PTR [DI+52],5678H",       0x81, 0155, 0x34, 0x78, 0x56);
    TEST(SUB, "WORD PTR [BP+1234H],5678H",    0x81, 0256, 0x34, 0x12, 0x78, 0x56);
    TEST(SUB, "WORD PTR [BX+SI],5678H",       0x81, 0050, 0x78, 0x56);
    TEST(SUB, "WORD PTR [BX+DI-52],5678H",    0x81, 0151, 0xCC, 0x78, 0x56);
    TEST(SUB, "WORD PTR [BP+SI+89ABH],5678H", 0x81, 0252, 0xAB, 0x89, 0x78, 0x56);
    TEST(SUB, "AX,56H",                       0x83, 0350, 0x56);
    TEST(SUB, "AX,-16",                       0x83, 0350, 0xF0);
    TEST(SUB, "CX,56H",                       0x83, 0351, 0x56);
    TEST(SUB, "CX,-16",                       0x83, 0351, 0xF0);
    TEST(SUB, "WORD PTR [SI],56H",            0x83, 0054, 0x56);
    TEST(SUB, "WORD PTR [1234H],56H",         0x83, 0056, 0x34, 0x12, 0x56);
    TEST(SUB, "WORD PTR [DI+52],56H",         0x83, 0155, 0x34, 0x56);
    TEST(SUB, "WORD PTR [BP+1234H],56H",      0x83, 0256, 0x34, 0x12, 0x56);
    TEST(SUB, "WORD PTR [BX+SI],56H",         0x83, 0050, 0x56);
    TEST(SUB, "WORD PTR [BX+DI-52],56H",      0x83, 0151, 0xCC, 0x56);
    TEST(SUB, "WORD PTR [BP+SI+89ABH],56H",   0x83, 0252, 0xAB, 0x89, 0x56);

    TEST(SUB, "AL,56H",   0x2C, 0x56);
    TEST(SUB, "AX,5678H", 0x2D, 0x78, 0x56);

    TEST(SBB, "AL,CL",            0x18, 0310);
    TEST(SBB, "DL,BL",            0x18, 0332);
    TEST(SBB, "AH,CH",            0x18, 0354);
    TEST(SBB, "[SI],DH",          0x18, 0064);
    TEST(SBB, "[1234H],BH",       0x18, 0076, 0x34, 0x12);
    TEST(SBB, "[DI+52],AL",       0x18, 0105, 0x34);
    TEST(SBB, "[BP+1234H],CL",    0x18, 0216, 0x34, 0x12);
    TEST(SBB, "[BX+SI],DL",       0x18, 0020);
    TEST(SBB, "[BX+DI+52],BL",    0x18, 0131, 0x34);
    TEST(SBB, "[BP+SI+1234H],AH", 0x18, 0242, 0x34, 0x12);
    TEST(SBB, "DH,[SI]",          0x1A, 0064);
    TEST(SBB, "BH,[1234H]",       0x1A, 0076, 0x34, 0x12);
    TEST(SBB, "AL,[DI-52]",       0x1A, 0105, 0xCC);
    TEST(SBB, "CL,[BP+1234H]",    0x1A, 0216, 0x34, 0x12);
    TEST(SBB, "DL,[BX+SI]",       0x1A, 0020);
    TEST(SBB, "BL,[BX+DI-52]",    0x1A, 0131, 0xCC);
    TEST(SBB, "AH,[BP+SI+1234H]", 0x1A, 0242, 0x34, 0x12);
    TEST(SBB, "AX,CX",            0x19, 0310);
    TEST(SBB, "DX,BX",            0x19, 0332);
    TEST(SBB, "SP,BP",            0x19, 0354);
    TEST(SBB, "[SI],SI",          0x19, 0064);
    TEST(SBB, "[1234H],DI",       0x19, 0076, 0x34, 0x12);
    TEST(SBB, "[DI-52],AX",       0x19, 0105, 0xCC);
    TEST(SBB, "[BP+1234H],CX",    0x19, 0216, 0x34, 0x12);
    TEST(SBB, "[BX+SI],DX",       0x19, 0020);
    TEST(SBB, "[BX+DI+52],BX",    0x19, 0131, 0x34);
    TEST(SBB, "[BP+SI+1234H],SP", 0x19, 0242, 0x34, 0x12);
    TEST(SBB, "SI,[SI]",          0x1B, 0064);
    TEST(SBB, "DI,[1234H]",       0x1B, 0076, 0x34, 0x12);
    TEST(SBB, "AX,[DI+52]",       0x1B, 0105, 0x34);
    TEST(SBB, "CX,[BP+1234H]",    0x1B, 0216, 0x34, 0x12);
    TEST(SBB, "DX,[BX+SI]",       0x1B, 0020);
    TEST(SBB, "BX,[BX+DI+52]",    0x1B, 0131, 0x34);
    TEST(SBB, "SP,[BP+SI+1234H]", 0x1B, 0242, 0x34, 0x12);

    TEST(SBB, "AL,56H",                       0x80, 0330, 0x56);
    TEST(SBB, "CL,56H",                       0x80, 0331, 0x56);
    TEST(SBB, "BYTE PTR [SI],56H",            0x80, 0034, 0x56);
    TEST(SBB, "BYTE PTR [89ABH],56H",         0x80, 0036, 0xAB, 0x89, 0x56);
    TEST(SBB, "BYTE PTR [DI-52],56H",         0x80, 0135, 0xCC, 0x56);
    TEST(SBB, "BYTE PTR [BP+89ABH],56H",      0x80, 0236, 0xAB, 0x89, 0x56);
    TEST(SBB, "BYTE PTR [BX+SI],56H",         0x80, 0030, 0x56);
    TEST(SBB, "BYTE PTR [BX+DI+52],56H",      0x80, 0131, 0x34, 0x56);
    TEST(SBB, "BYTE PTR [BP+SI+1234H],56H",   0x80, 0232, 0x34, 0x12, 0x56);
    TEST(SBB, "AX,5678H",                     0x81, 0330, 0x78, 0x56);
    TEST(SBB, "CX,5678H",                     0x81, 0331, 0x78, 0x56);
    TEST(SBB, "WORD PTR [SI],5678H",          0x81, 0034, 0x78, 0x56);
    TEST(SBB, "WORD PTR [1234H],5678H",       0x81, 0036, 0x34, 0x12, 0x78, 0x56);
    TEST(SBB, "WORD PTR [DI+52],5678H",       0x81, 0135, 0x34, 0x78, 0x56);
    TEST(SBB, "WORD PTR [BP+1234H],5678H",    0x81, 0236, 0x34, 0x12, 0x78, 0x56);
    TEST(SBB, "WORD PTR [BX+SI],5678H",       0x81, 0030, 0x78, 0x56);
    TEST(SBB, "WORD PTR [BX+DI-52],5678H",    0x81, 0131, 0xCC, 0x78, 0x56);
    TEST(SBB, "WORD PTR [BP+SI+89ABH],5678H", 0x81, 0232, 0xAB, 0x89, 0x78, 0x56);
    TEST(SBB, "AX,56H",                       0x83, 0330, 0x56);
    TEST(SBB, "AX,-16",                       0x83, 0330, 0xF0);
    TEST(SBB, "CX,56H",                       0x83, 0331, 0x56);
    TEST(SBB, "CX,-16",                       0x83, 0331, 0xF0);
    TEST(SBB, "WORD PTR [SI],56H",            0x83, 0034, 0x56);
    TEST(SBB, "WORD PTR [1234H],56H",         0x83, 0036, 0x34, 0x12, 0x56);
    TEST(SBB, "WORD PTR [DI+52],56H",         0x83, 0135, 0x34, 0x56);
    TEST(SBB, "WORD PTR [BP+1234H],56H",      0x83, 0236, 0x34, 0x12, 0x56);
    TEST(SBB, "WORD PTR [BX+SI],56H",         0x83, 0030, 0x56);
    TEST(SBB, "WORD PTR [BX+DI-52],56H",      0x83, 0131, 0xCC, 0x56);
    TEST(SBB, "WORD PTR [BP+SI+89ABH],56H",   0x83, 0232, 0xAB, 0x89, 0x56);

    TEST(SBB, "AL,56H",   0x1C, 0x56);
    TEST(SBB, "AX,5678H", 0x1D, 0x78, 0x56);

    TEST(DEC, "CH",                     0xFE, 0315);
    TEST(DEC, "BYTE PTR [SI]",          0xFE, 0014);
    TEST(DEC, "BYTE PTR [1234H]",       0xFE, 0016, 0x34, 0x12);
    TEST(DEC, "BYTE PTR [DI-52]",       0xFE, 0115, 0xCC);
    TEST(DEC, "BYTE PTR [BP+1234H]",    0xFE, 0216, 0x34, 0x12);
    TEST(DEC, "BYTE PTR [BX+SI]",       0xFE, 0010);
    TEST(DEC, "BYTE PTR [BX+DI+52]",    0xFE, 0111, 0x34);
    TEST(DEC, "BYTE PTR [BP+SI+1234H]", 0xFE, 0212, 0x34, 0x12);
    TEST(DEC, "BP",                     0xFF, 0315);
    TEST(DEC, "WORD PTR [SI]",          0xFF, 0014);
    TEST(DEC, "WORD PTR [1234H]",       0xFF, 0016, 0x34, 0x12);
    TEST(DEC, "WORD PTR [DI-52]",       0xFF, 0115, 0xCC);
    TEST(DEC, "WORD PTR [BP+1234H]",    0xFF, 0216, 0x34, 0x12);
    TEST(DEC, "WORD PTR [BX+SI]",       0xFF, 0010);
    TEST(DEC, "WORD PTR [BX+DI+52]",    0xFF, 0111, 0x34);
    TEST(DEC, "WORD PTR [BP+SI+1234H]", 0xFF, 0212, 0x34, 0x12);

    TEST(DEC, "BP", 0115);

    TEST(NEG, "CH",                     0xF6, 0335);
    TEST(NEG, "BYTE PTR [SI]",          0xF6, 0034);
    TEST(NEG, "BYTE PTR [1234H]",       0xF6, 0036, 0x34, 0x12);
    TEST(NEG, "BYTE PTR [DI-52]",       0xF6, 0135, 0xCC);
    TEST(NEG, "BYTE PTR [BP+1234H]",    0xF6, 0236, 0x34, 0x12);
    TEST(NEG, "BYTE PTR [BX+SI]",       0xF6, 0030);
    TEST(NEG, "BYTE PTR [BX+DI+52]",    0xF6, 0131, 0x34);
    TEST(NEG, "BYTE PTR [BP+SI+1234H]", 0xF6, 0232, 0x34, 0x12);
    TEST(NEG, "BP",                     0xF7, 0335);
    TEST(NEG, "WORD PTR [SI]",          0xF7, 0034);
    TEST(NEG, "WORD PTR [1234H]",       0xF7, 0036, 0x34, 0x12);
    TEST(NEG, "WORD PTR [DI-52]",       0xF7, 0135, 0xCC);
    TEST(NEG, "WORD PTR [BP+1234H]",    0xF7, 0236, 0x34, 0x12);
    TEST(NEG, "WORD PTR [BX+SI]",       0xF7, 0030);
    TEST(NEG, "WORD PTR [BX+DI+52]",    0xF7, 0131, 0x34);
    TEST(NEG, "WORD PTR [BP+SI+1234H]", 0xF7, 0232, 0x34, 0x12);

    TEST(CMP, "AL,CL",            0x38, 0310);
    TEST(CMP, "DL,BL",            0x38, 0332);
    TEST(CMP, "AH,CH",            0x38, 0354);
    TEST(CMP, "[SI],DH",          0x38, 0064);
    TEST(CMP, "[1234H],BH",       0x38, 0076, 0x34, 0x12);
    TEST(CMP, "[DI+52],AL",       0x38, 0105, 0x34);
    TEST(CMP, "[BP+1234H],CL",    0x38, 0216, 0x34, 0x12);
    TEST(CMP, "[BX+SI],DL",       0x38, 0020);
    TEST(CMP, "[BX+DI+52],BL",    0x38, 0131, 0x34);
    TEST(CMP, "[BP+SI+1234H],AH", 0x38, 0242, 0x34, 0x12);
    TEST(CMP, "DH,[SI]",          0x3A, 0064);
    TEST(CMP, "BH,[1234H]",       0x3A, 0076, 0x34, 0x12);
    TEST(CMP, "AL,[DI-52]",       0x3A, 0105, 0xCC);
    TEST(CMP, "CL,[BP+1234H]",    0x3A, 0216, 0x34, 0x12);
    TEST(CMP, "DL,[BX+SI]",       0x3A, 0020);
    TEST(CMP, "BL,[BX+DI-52]",    0x3A, 0131, 0xCC);
    TEST(CMP, "AH,[BP+SI+1234H]", 0x3A, 0242, 0x34, 0x12);
    TEST(CMP, "AX,CX",            0x39, 0310);
    TEST(CMP, "DX,BX",            0x39, 0332);
    TEST(CMP, "SP,BP",            0x39, 0354);
    TEST(CMP, "[SI],SI",          0x39, 0064);
    TEST(CMP, "[1234H],DI",       0x39, 0076, 0x34, 0x12);
    TEST(CMP, "[DI-52],AX",       0x39, 0105, 0xCC);
    TEST(CMP, "[BP+1234H],CX",    0x39, 0216, 0x34, 0x12);
    TEST(CMP, "[BX+SI],DX",       0x39, 0020);
    TEST(CMP, "[BX+DI+52],BX",    0x39, 0131, 0x34);
    TEST(CMP, "[BP+SI+1234H],SP", 0x39, 0242, 0x34, 0x12);
    TEST(CMP, "SI,[SI]",          0x3B, 0064);
    TEST(CMP, "DI,[1234H]",       0x3B, 0076, 0x34, 0x12);
    TEST(CMP, "AX,[DI+52]",       0x3B, 0105, 0x34);
    TEST(CMP, "CX,[BP+1234H]",    0x3B, 0216, 0x34, 0x12);
    TEST(CMP, "DX,[BX+SI]",       0x3B, 0020);
    TEST(CMP, "BX,[BX+DI+52]",    0x3B, 0131, 0x34);
    TEST(CMP, "SP,[BP+SI+1234H]", 0x3B, 0242, 0x34, 0x12);

    TEST(CMP, "AL,56H",                       0x80, 0370, 0x56);
    TEST(CMP, "CL,56H",                       0x80, 0371, 0x56);
    TEST(CMP, "BYTE PTR [SI],56H",            0x80, 0074, 0x56);
    TEST(CMP, "BYTE PTR [89ABH],56H",         0x80, 0076, 0xAB, 0x89, 0x56);
    TEST(CMP, "BYTE PTR [DI-52],56H",         0x80, 0175, 0xCC, 0x56);
    TEST(CMP, "BYTE PTR [BP+89ABH],56H",      0x80, 0276, 0xAB, 0x89, 0x56);
    TEST(CMP, "BYTE PTR [BX+SI],56H",         0x80, 0070, 0x56);
    TEST(CMP, "BYTE PTR [BX+DI+52],56H",      0x80, 0171, 0x34, 0x56);
    TEST(CMP, "BYTE PTR [BP+SI+1234H],56H",   0x80, 0272, 0x34, 0x12, 0x56);
    TEST(CMP, "AX,5678H",                     0x81, 0370, 0x78, 0x56);
    TEST(CMP, "CX,5678H",                     0x81, 0371, 0x78, 0x56);
    TEST(CMP, "WORD PTR [SI],5678H",          0x81, 0074, 0x78, 0x56);
    TEST(CMP, "WORD PTR [1234H],5678H",       0x81, 0076, 0x34, 0x12, 0x78, 0x56);
    TEST(CMP, "WORD PTR [DI+52],5678H",       0x81, 0175, 0x34, 0x78, 0x56);
    TEST(CMP, "WORD PTR [BP+1234H],5678H",    0x81, 0276, 0x34, 0x12, 0x78, 0x56);
    TEST(CMP, "WORD PTR [BX+SI],5678H",       0x81, 0070, 0x78, 0x56);
    TEST(CMP, "WORD PTR [BX+DI-52],5678H",    0x81, 0171, 0xCC, 0x78, 0x56);
    TEST(CMP, "WORD PTR [BP+SI+89ABH],5678H", 0x81, 0272, 0xAB, 0x89, 0x78, 0x56);
    TEST(CMP, "AX,56H",                       0x83, 0370, 0x56);
    TEST(CMP, "AX,-16",                       0x83, 0370, 0xF0);
    TEST(CMP, "CX,56H",                       0x83, 0371, 0x56);
    TEST(CMP, "CX,-16",                       0x83, 0371, 0xF0);
    TEST(CMP, "WORD PTR [SI],56H",            0x83, 0074, 0x56);
    TEST(CMP, "WORD PTR [1234H],56H",         0x83, 0076, 0x34, 0x12, 0x56);
    TEST(CMP, "WORD PTR [DI+52],56H",         0x83, 0175, 0x34, 0x56);
    TEST(CMP, "WORD PTR [BP+1234H],56H",      0x83, 0276, 0x34, 0x12, 0x56);
    TEST(CMP, "WORD PTR [BX+SI],56H",         0x83, 0070, 0x56);
    TEST(CMP, "WORD PTR [BX+DI-52],56H",      0x83, 0171, 0xCC, 0x56);
    TEST(CMP, "WORD PTR [BP+SI+89ABH],56H",   0x83, 0272, 0xAB, 0x89, 0x56);

    TEST(CMP, "AL,56H",   0x3C, 0x56);
    TEST(CMP, "AX,5678H", 0x3D, 0x78, 0x56);

    TEST(AAS, "", 0x3F);
    TEST(DAS, "", 0x2F);

    TEST(MUL, "CH",                     0xF6, 0345);
    TEST(MUL, "BYTE PTR [SI]",          0xF6, 0044);
    TEST(MUL, "BYTE PTR [1234H]",       0xF6, 0046, 0x34, 0x12);
    TEST(MUL, "BYTE PTR [DI-52]",       0xF6, 0145, 0xCC);
    TEST(MUL, "BYTE PTR [BP+1234H]",    0xF6, 0246, 0x34, 0x12);
    TEST(MUL, "BYTE PTR [BX+SI]",       0xF6, 0040);
    TEST(MUL, "BYTE PTR [BX+DI+52]",    0xF6, 0141, 0x34);
    TEST(MUL, "BYTE PTR [BP+SI+1234H]", 0xF6, 0242, 0x34, 0x12);
    TEST(MUL, "BP",                     0xF7, 0345);
    TEST(MUL, "WORD PTR [SI]",          0xF7, 0044);
    TEST(MUL, "WORD PTR [1234H]",       0xF7, 0046, 0x34, 0x12);
    TEST(MUL, "WORD PTR [DI-52]",       0xF7, 0145, 0xCC);
    TEST(MUL, "WORD PTR [BP+1234H]",    0xF7, 0246, 0x34, 0x12);
    TEST(MUL, "WORD PTR [BX+SI]",       0xF7, 0040);
    TEST(MUL, "WORD PTR [BX+DI+52]",    0xF7, 0141, 0x34);
    TEST(MUL, "WORD PTR [BP+SI+1234H]", 0xF7, 0242, 0x34, 0x12);

    TEST(IMUL, "CH",                     0xF6, 0355);
    TEST(IMUL, "BYTE PTR [SI]",          0xF6, 0054);
    TEST(IMUL, "BYTE PTR [1234H]",       0xF6, 0056, 0x34, 0x12);
    TEST(IMUL, "BYTE PTR [DI-52]",       0xF6, 0155, 0xCC);
    TEST(IMUL, "BYTE PTR [BP+1234H]",    0xF6, 0256, 0x34, 0x12);
    TEST(IMUL, "BYTE PTR [BX+SI]",       0xF6, 0050);
    TEST(IMUL, "BYTE PTR [BX+DI+52]",    0xF6, 0151, 0x34);
    TEST(IMUL, "BYTE PTR [BP+SI+1234H]", 0xF6, 0252, 0x34, 0x12);
    TEST(IMUL, "BP",                     0xF7, 0355);
    TEST(IMUL, "WORD PTR [SI]",          0xF7, 0054);
    TEST(IMUL, "WORD PTR [1234H]",       0xF7, 0056, 0x34, 0x12);
    TEST(IMUL, "WORD PTR [DI-52]",       0xF7, 0155, 0xCC);
    TEST(IMUL, "WORD PTR [BP+1234H]",    0xF7, 0256, 0x34, 0x12);
    TEST(IMUL, "WORD PTR [BX+SI]",       0xF7, 0050);
    TEST(IMUL, "WORD PTR [BX+DI+52]",    0xF7, 0151, 0x34);
    TEST(IMUL, "WORD PTR [BP+SI+1234H]", 0xF7, 0252, 0x34, 0x12);

    TEST(AAM, "", 0xD4, 0x0A);

    TEST(DIV, "CH",                     0xF6, 0365);
    TEST(DIV, "BYTE PTR [SI]",          0xF6, 0064);
    TEST(DIV, "BYTE PTR [1234H]",       0xF6, 0066, 0x34, 0x12);
    TEST(DIV, "BYTE PTR [DI-52]",       0xF6, 0165, 0xCC);
    TEST(DIV, "BYTE PTR [BP+1234H]",    0xF6, 0266, 0x34, 0x12);
    TEST(DIV, "BYTE PTR [BX+SI]",       0xF6, 0060);
    TEST(DIV, "BYTE PTR [BX+DI+52]",    0xF6, 0161, 0x34);
    TEST(DIV, "BYTE PTR [BP+SI+1234H]", 0xF6, 0262, 0x34, 0x12);
    TEST(DIV, "BP",                     0xF7, 0365);
    TEST(DIV, "WORD PTR [SI]",          0xF7, 0064);
    TEST(DIV, "WORD PTR [1234H]",       0xF7, 0066, 0x34, 0x12);
    TEST(DIV, "WORD PTR [DI-52]",       0xF7, 0165, 0xCC);
    TEST(DIV, "WORD PTR [BP+1234H]",    0xF7, 0266, 0x34, 0x12);
    TEST(DIV, "WORD PTR [BX+SI]",       0xF7, 0060);
    TEST(DIV, "WORD PTR [BX+DI+52]",    0xF7, 0161, 0x34);
    TEST(DIV, "WORD PTR [BP+SI+1234H]", 0xF7, 0262, 0x34, 0x12);

    TEST(IDIV, "CH",                     0xF6, 0375);
    TEST(IDIV, "BYTE PTR [SI]",          0xF6, 0074);
    TEST(IDIV, "BYTE PTR [1234H]",       0xF6, 0076, 0x34, 0x12);
    TEST(IDIV, "BYTE PTR [DI-52]",       0xF6, 0175, 0xCC);
    TEST(IDIV, "BYTE PTR [BP+1234H]",    0xF6, 0276, 0x34, 0x12);
    TEST(IDIV, "BYTE PTR [BX+SI]",       0xF6, 0070);
    TEST(IDIV, "BYTE PTR [BX+DI+52]",    0xF6, 0171, 0x34);
    TEST(IDIV, "BYTE PTR [BP+SI+1234H]", 0xF6, 0272, 0x34, 0x12);
    TEST(IDIV, "BP",                     0xF7, 0375);
    TEST(IDIV, "WORD PTR [SI]",          0xF7, 0074);
    TEST(IDIV, "WORD PTR [1234H]",       0xF7, 0076, 0x34, 0x12);
    TEST(IDIV, "WORD PTR [DI-52]",       0xF7, 0175, 0xCC);
    TEST(IDIV, "WORD PTR [BP+1234H]",    0xF7, 0276, 0x34, 0x12);
    TEST(IDIV, "WORD PTR [BX+SI]",       0xF7, 0070);
    TEST(IDIV, "WORD PTR [BX+DI+52]",    0xF7, 0171, 0x34);
    TEST(IDIV, "WORD PTR [BP+SI+1234H]", 0xF7, 0272, 0x34, 0x12);

    TEST(AAD, "", 0xD5, 0x0A);

    TEST(CBW, "", 0x98);
    TEST(CWD, "", 0x99);
}

static void test_logic() {
    TEST(NOT, "CH",                     0xF6, 0325);
    TEST(NOT, "BYTE PTR [SI]",          0xF6, 0024);
    TEST(NOT, "BYTE PTR [1234H]",       0xF6, 0026, 0x34, 0x12);
    TEST(NOT, "BYTE PTR [DI-52]",       0xF6, 0125, 0xCC);
    TEST(NOT, "BYTE PTR [BP+1234H]",    0xF6, 0226, 0x34, 0x12);
    TEST(NOT, "BYTE PTR [BX+SI]",       0xF6, 0020);
    TEST(NOT, "BYTE PTR [BX+DI+52]",    0xF6, 0121, 0x34);
    TEST(NOT, "BYTE PTR [BP+SI+1234H]", 0xF6, 0222, 0x34, 0x12);
    TEST(NOT, "BP",                     0xF7, 0325);
    TEST(NOT, "WORD PTR [SI]",          0xF7, 0024);
    TEST(NOT, "WORD PTR [1234H]",       0xF7, 0026, 0x34, 0x12);
    TEST(NOT, "WORD PTR [DI-52]",       0xF7, 0125, 0xCC);
    TEST(NOT, "WORD PTR [BP+1234H]",    0xF7, 0226, 0x34, 0x12);
    TEST(NOT, "WORD PTR [BX+SI]",       0xF7, 0020);
    TEST(NOT, "WORD PTR [BX+DI+52]",    0xF7, 0121, 0x34);
    TEST(NOT, "WORD PTR [BP+SI+1234H]", 0xF7, 0222, 0x34, 0x12);

    TEST(SHL, "CH,1",                      0xD0, 0345);
    TEST(SHL, "BYTE PTR [SI],1",           0xD0, 0044);
    TEST(SHL, "BYTE PTR [1234H],1",        0xD0, 0046, 0x34, 0x12);
    TEST(SHL, "BYTE PTR [DI-52],1",        0xD0, 0145, 0xCC);
    TEST(SHL, "BYTE PTR [BP+1234H],1",     0xD0, 0246, 0x34, 0x12);
    TEST(SHL, "BYTE PTR [BX+SI],1",        0xD0, 0040);
    TEST(SHL, "BYTE PTR [BX+DI+52],1",     0xD0, 0141, 0x34);
    TEST(SHL, "BYTE PTR [BP+SI+1234H],1",  0xD0, 0242, 0x34, 0x12);
    TEST(SHL, "BP,1",                      0xD1, 0345);
    TEST(SHL, "WORD PTR [SI],1",           0xD1, 0044);
    TEST(SHL, "WORD PTR [1234H],1",        0xD1, 0046, 0x34, 0x12);
    TEST(SHL, "WORD PTR [DI-52],1",        0xD1, 0145, 0xCC);
    TEST(SHL, "WORD PTR [BP+1234H],1",     0xD1, 0246, 0x34, 0x12);
    TEST(SHL, "WORD PTR [BX+SI],1",        0xD1, 0040);
    TEST(SHL, "WORD PTR [BX+DI+52],1",     0xD1, 0141, 0x34);
    TEST(SHL, "WORD PTR [BP+SI+1234H],1",  0xD1, 0242, 0x34, 0x12);
    TEST(SHL, "CH,CL",                     0xD2, 0345);
    TEST(SHL, "BYTE PTR [SI],CL",          0xD2, 0044);
    TEST(SHL, "BYTE PTR [1234H],CL",       0xD2, 0046, 0x34, 0x12);
    TEST(SHL, "BYTE PTR [DI-52],CL",       0xD2, 0145, 0xCC);
    TEST(SHL, "BYTE PTR [BP+1234H],CL",    0xD2, 0246, 0x34, 0x12);
    TEST(SHL, "BYTE PTR [BX+SI],CL",       0xD2, 0040);
    TEST(SHL, "BYTE PTR [BX+DI+52],CL",    0xD2, 0141, 0x34);
    TEST(SHL, "BYTE PTR [BP+SI+1234H],CL", 0xD2, 0242, 0x34, 0x12);
    TEST(SHL, "BP,CL",                     0xD3, 0345);
    TEST(SHL, "WORD PTR [SI],CL",          0xD3, 0044);
    TEST(SHL, "WORD PTR [1234H],CL",       0xD3, 0046, 0x34, 0x12);
    TEST(SHL, "WORD PTR [DI-52],CL",       0xD3, 0145, 0xCC);
    TEST(SHL, "WORD PTR [BP+1234H],CL",    0xD3, 0246, 0x34, 0x12);
    TEST(SHL, "WORD PTR [BX+SI],CL",       0xD3, 0040);
    TEST(SHL, "WORD PTR [BX+DI+52],CL",    0xD3, 0141, 0x34);
    TEST(SHL, "WORD PTR [BP+SI+1234H],CL", 0xD3, 0242, 0x34, 0x12);

    TEST(SHR, "CH,1",                      0xD0, 0355);
    TEST(SHR, "BYTE PTR [SI],1",           0xD0, 0054);
    TEST(SHR, "BYTE PTR [1234H],1",        0xD0, 0056, 0x34, 0x12);
    TEST(SHR, "BYTE PTR [DI-52],1",        0xD0, 0155, 0xCC);
    TEST(SHR, "BYTE PTR [BP+1234H],1",     0xD0, 0256, 0x34, 0x12);
    TEST(SHR, "BYTE PTR [BX+SI],1",        0xD0, 0050);
    TEST(SHR, "BYTE PTR [BX+DI+52],1",     0xD0, 0151, 0x34);
    TEST(SHR, "BYTE PTR [BP+SI+1234H],1",  0xD0, 0252, 0x34, 0x12);
    TEST(SHR, "BP,1",                      0xD1, 0355);
    TEST(SHR, "WORD PTR [SI],1",           0xD1, 0054);
    TEST(SHR, "WORD PTR [1234H],1",        0xD1, 0056, 0x34, 0x12);
    TEST(SHR, "WORD PTR [DI-52],1",        0xD1, 0155, 0xCC);
    TEST(SHR, "WORD PTR [BP+1234H],1",     0xD1, 0256, 0x34, 0x12);
    TEST(SHR, "WORD PTR [BX+SI],1",        0xD1, 0050);
    TEST(SHR, "WORD PTR [BX+DI+52],1",     0xD1, 0151, 0x34);
    TEST(SHR, "WORD PTR [BP+SI+1234H],1",  0xD1, 0252, 0x34, 0x12);
    TEST(SHR, "CH,CL",                     0xD2, 0355);
    TEST(SHR, "BYTE PTR [SI],CL",          0xD2, 0054);
    TEST(SHR, "BYTE PTR [1234H],CL",       0xD2, 0056, 0x34, 0x12);
    TEST(SHR, "BYTE PTR [DI-52],CL",       0xD2, 0155, 0xCC);
    TEST(SHR, "BYTE PTR [BP+1234H],CL",    0xD2, 0256, 0x34, 0x12);
    TEST(SHR, "BYTE PTR [BX+SI],CL",       0xD2, 0050);
    TEST(SHR, "BYTE PTR [BX+DI+52],CL",    0xD2, 0151, 0x34);
    TEST(SHR, "BYTE PTR [BP+SI+1234H],CL", 0xD2, 0252, 0x34, 0x12);
    TEST(SHR, "BP,CL",                     0xD3, 0355);
    TEST(SHR, "WORD PTR [SI],CL",          0xD3, 0054);
    TEST(SHR, "WORD PTR [1234H],CL",       0xD3, 0056, 0x34, 0x12);
    TEST(SHR, "WORD PTR [DI-52],CL",       0xD3, 0155, 0xCC);
    TEST(SHR, "WORD PTR [BP+1234H],CL",    0xD3, 0256, 0x34, 0x12);
    TEST(SHR, "WORD PTR [BX+SI],CL",       0xD3, 0050);
    TEST(SHR, "WORD PTR [BX+DI+52],CL",    0xD3, 0151, 0x34);
    TEST(SHR, "WORD PTR [BP+SI+1234H],CL", 0xD3, 0252, 0x34, 0x12);

    TEST(SAR, "CH,1",                      0xD0, 0375);
    TEST(SAR, "BYTE PTR [SI],1",           0xD0, 0074);
    TEST(SAR, "BYTE PTR [1234H],1",        0xD0, 0076, 0x34, 0x12);
    TEST(SAR, "BYTE PTR [DI-52],1",        0xD0, 0175, 0xCC);
    TEST(SAR, "BYTE PTR [BP+1234H],1",     0xD0, 0276, 0x34, 0x12);
    TEST(SAR, "BYTE PTR [BX+SI],1",        0xD0, 0070);
    TEST(SAR, "BYTE PTR [BX+DI+52],1",     0xD0, 0171, 0x34);
    TEST(SAR, "BYTE PTR [BP+SI+1234H],1",  0xD0, 0272, 0x34, 0x12);
    TEST(SAR, "BP,1",                      0xD1, 0375);
    TEST(SAR, "WORD PTR [SI],1",           0xD1, 0074);
    TEST(SAR, "WORD PTR [1234H],1",        0xD1, 0076, 0x34, 0x12);
    TEST(SAR, "WORD PTR [DI-52],1",        0xD1, 0175, 0xCC);
    TEST(SAR, "WORD PTR [BP+1234H],1",     0xD1, 0276, 0x34, 0x12);
    TEST(SAR, "WORD PTR [BX+SI],1",        0xD1, 0070);
    TEST(SAR, "WORD PTR [BX+DI+52],1",     0xD1, 0171, 0x34);
    TEST(SAR, "WORD PTR [BP+SI+1234H],1",  0xD1, 0272, 0x34, 0x12);
    TEST(SAR, "CH,CL",                     0xD2, 0375);
    TEST(SAR, "BYTE PTR [SI],CL",          0xD2, 0074);
    TEST(SAR, "BYTE PTR [1234H],CL",       0xD2, 0076, 0x34, 0x12);
    TEST(SAR, "BYTE PTR [DI-52],CL",       0xD2, 0175, 0xCC);
    TEST(SAR, "BYTE PTR [BP+1234H],CL",    0xD2, 0276, 0x34, 0x12);
    TEST(SAR, "BYTE PTR [BX+SI],CL",       0xD2, 0070);
    TEST(SAR, "BYTE PTR [BX+DI+52],CL",    0xD2, 0171, 0x34);
    TEST(SAR, "BYTE PTR [BP+SI+1234H],CL", 0xD2, 0272, 0x34, 0x12);
    TEST(SAR, "BP,CL",                     0xD3, 0375);
    TEST(SAR, "WORD PTR [SI],CL",          0xD3, 0074);
    TEST(SAR, "WORD PTR [1234H],CL",       0xD3, 0076, 0x34, 0x12);
    TEST(SAR, "WORD PTR [DI-52],CL",       0xD3, 0175, 0xCC);
    TEST(SAR, "WORD PTR [BP+1234H],CL",    0xD3, 0276, 0x34, 0x12);
    TEST(SAR, "WORD PTR [BX+SI],CL",       0xD3, 0070);
    TEST(SAR, "WORD PTR [BX+DI+52],CL",    0xD3, 0171, 0x34);
    TEST(SAR, "WORD PTR [BP+SI+1234H],CL", 0xD3, 0272, 0x34, 0x12);

    TEST(ROL, "CH,1",                      0xD0, 0305);
    TEST(ROL, "BYTE PTR [SI],1",           0xD0, 0004);
    TEST(ROL, "BYTE PTR [1234H],1",        0xD0, 0006, 0x34, 0x12);
    TEST(ROL, "BYTE PTR [DI-52],1",        0xD0, 0105, 0xCC);
    TEST(ROL, "BYTE PTR [BP+1234H],1",     0xD0, 0206, 0x34, 0x12);
    TEST(ROL, "BYTE PTR [BX+SI],1",        0xD0, 0000);
    TEST(ROL, "BYTE PTR [BX+DI+52],1",     0xD0, 0101, 0x34);
    TEST(ROL, "BYTE PTR [BP+SI+1234H],1",  0xD0, 0202, 0x34, 0x12);
    TEST(ROL, "BP,1",                      0xD1, 0305);
    TEST(ROL, "WORD PTR [SI],1",           0xD1, 0004);
    TEST(ROL, "WORD PTR [1234H],1",        0xD1, 0006, 0x34, 0x12);
    TEST(ROL, "WORD PTR [DI-52],1",        0xD1, 0105, 0xCC);
    TEST(ROL, "WORD PTR [BP+1234H],1",     0xD1, 0206, 0x34, 0x12);
    TEST(ROL, "WORD PTR [BX+SI],1",        0xD1, 0000);
    TEST(ROL, "WORD PTR [BX+DI+52],1",     0xD1, 0101, 0x34);
    TEST(ROL, "WORD PTR [BP+SI+1234H],1",  0xD1, 0202, 0x34, 0x12);
    TEST(ROL, "CH,CL",                     0xD2, 0305);
    TEST(ROL, "BYTE PTR [SI],CL",          0xD2, 0004);
    TEST(ROL, "BYTE PTR [1234H],CL",       0xD2, 0006, 0x34, 0x12);
    TEST(ROL, "BYTE PTR [DI-52],CL",       0xD2, 0105, 0xCC);
    TEST(ROL, "BYTE PTR [BP+1234H],CL",    0xD2, 0206, 0x34, 0x12);
    TEST(ROL, "BYTE PTR [BX+SI],CL",       0xD2, 0000);
    TEST(ROL, "BYTE PTR [BX+DI+52],CL",    0xD2, 0101, 0x34);
    TEST(ROL, "BYTE PTR [BP+SI+1234H],CL", 0xD2, 0202, 0x34, 0x12);
    TEST(ROL, "BP,CL",                     0xD3, 0305);
    TEST(ROL, "WORD PTR [SI],CL",          0xD3, 0004);
    TEST(ROL, "WORD PTR [1234H],CL",       0xD3, 0006, 0x34, 0x12);
    TEST(ROL, "WORD PTR [DI-52],CL",       0xD3, 0105, 0xCC);
    TEST(ROL, "WORD PTR [BP+1234H],CL",    0xD3, 0206, 0x34, 0x12);
    TEST(ROL, "WORD PTR [BX+SI],CL",       0xD3, 0000);
    TEST(ROL, "WORD PTR [BX+DI+52],CL",    0xD3, 0101, 0x34);
    TEST(ROL, "WORD PTR [BP+SI+1234H],CL", 0xD3, 0202, 0x34, 0x12);

    TEST(ROR, "CH,1",                      0xD0, 0315);
    TEST(ROR, "BYTE PTR [SI],1",           0xD0, 0014);
    TEST(ROR, "BYTE PTR [1234H],1",        0xD0, 0016, 0x34, 0x12);
    TEST(ROR, "BYTE PTR [DI-52],1",        0xD0, 0115, 0xCC);
    TEST(ROR, "BYTE PTR [BP+1234H],1",     0xD0, 0216, 0x34, 0x12);
    TEST(ROR, "BYTE PTR [BX+SI],1",        0xD0, 0010);
    TEST(ROR, "BYTE PTR [BX+DI+52],1",     0xD0, 0111, 0x34);
    TEST(ROR, "BYTE PTR [BP+SI+1234H],1",  0xD0, 0212, 0x34, 0x12);
    TEST(ROR, "BP,1",                      0xD1, 0315);
    TEST(ROR, "WORD PTR [SI],1",           0xD1, 0014);
    TEST(ROR, "WORD PTR [1234H],1",        0xD1, 0016, 0x34, 0x12);
    TEST(ROR, "WORD PTR [DI-52],1",        0xD1, 0115, 0xCC);
    TEST(ROR, "WORD PTR [BP+1234H],1",     0xD1, 0216, 0x34, 0x12);
    TEST(ROR, "WORD PTR [BX+SI],1",        0xD1, 0010);
    TEST(ROR, "WORD PTR [BX+DI+52],1",     0xD1, 0111, 0x34);
    TEST(ROR, "WORD PTR [BP+SI+1234H],1",  0xD1, 0212, 0x34, 0x12);
    TEST(ROR, "CH,CL",                     0xD2, 0315);
    TEST(ROR, "BYTE PTR [SI],CL",          0xD2, 0014);
    TEST(ROR, "BYTE PTR [1234H],CL",       0xD2, 0016, 0x34, 0x12);
    TEST(ROR, "BYTE PTR [DI-52],CL",       0xD2, 0115, 0xCC);
    TEST(ROR, "BYTE PTR [BP+1234H],CL",    0xD2, 0216, 0x34, 0x12);
    TEST(ROR, "BYTE PTR [BX+SI],CL",       0xD2, 0010);
    TEST(ROR, "BYTE PTR [BX+DI+52],CL",    0xD2, 0111, 0x34);
    TEST(ROR, "BYTE PTR [BP+SI+1234H],CL", 0xD2, 0212, 0x34, 0x12);
    TEST(ROR, "BP,CL",                     0xD3, 0315);
    TEST(ROR, "WORD PTR [SI],CL",          0xD3, 0014);
    TEST(ROR, "WORD PTR [1234H],CL",       0xD3, 0016, 0x34, 0x12);
    TEST(ROR, "WORD PTR [DI-52],CL",       0xD3, 0115, 0xCC);
    TEST(ROR, "WORD PTR [BP+1234H],CL",    0xD3, 0216, 0x34, 0x12);
    TEST(ROR, "WORD PTR [BX+SI],CL",       0xD3, 0010);
    TEST(ROR, "WORD PTR [BX+DI+52],CL",    0xD3, 0111, 0x34);
    TEST(ROR, "WORD PTR [BP+SI+1234H],CL", 0xD3, 0212, 0x34, 0x12);

    TEST(RCL, "CH,1",                      0xD0, 0325);
    TEST(RCL, "BYTE PTR [SI],1",           0xD0, 0024);
    TEST(RCL, "BYTE PTR [1234H],1",        0xD0, 0026, 0x34, 0x12);
    TEST(RCL, "BYTE PTR [DI-52],1",        0xD0, 0125, 0xCC);
    TEST(RCL, "BYTE PTR [BP+1234H],1",     0xD0, 0226, 0x34, 0x12);
    TEST(RCL, "BYTE PTR [BX+SI],1",        0xD0, 0020);
    TEST(RCL, "BYTE PTR [BX+DI+52],1",     0xD0, 0121, 0x34);
    TEST(RCL, "BYTE PTR [BP+SI+1234H],1",  0xD0, 0222, 0x34, 0x12);
    TEST(RCL, "BP,1",                      0xD1, 0325);
    TEST(RCL, "WORD PTR [SI],1",           0xD1, 0024);
    TEST(RCL, "WORD PTR [1234H],1",        0xD1, 0026, 0x34, 0x12);
    TEST(RCL, "WORD PTR [DI-52],1",        0xD1, 0125, 0xCC);
    TEST(RCL, "WORD PTR [BP+1234H],1",     0xD1, 0226, 0x34, 0x12);
    TEST(RCL, "WORD PTR [BX+SI],1",        0xD1, 0020);
    TEST(RCL, "WORD PTR [BX+DI+52],1",     0xD1, 0121, 0x34);
    TEST(RCL, "WORD PTR [BP+SI+1234H],1",  0xD1, 0222, 0x34, 0x12);
    TEST(RCL, "CH,CL",                     0xD2, 0325);
    TEST(RCL, "BYTE PTR [SI],CL",          0xD2, 0024);
    TEST(RCL, "BYTE PTR [1234H],CL",       0xD2, 0026, 0x34, 0x12);
    TEST(RCL, "BYTE PTR [DI-52],CL",       0xD2, 0125, 0xCC);
    TEST(RCL, "BYTE PTR [BP+1234H],CL",    0xD2, 0226, 0x34, 0x12);
    TEST(RCL, "BYTE PTR [BX+SI],CL",       0xD2, 0020);
    TEST(RCL, "BYTE PTR [BX+DI+52],CL",    0xD2, 0121, 0x34);
    TEST(RCL, "BYTE PTR [BP+SI+1234H],CL", 0xD2, 0222, 0x34, 0x12);
    TEST(RCL, "BP,CL",                     0xD3, 0325);
    TEST(RCL, "WORD PTR [SI],CL",          0xD3, 0024);
    TEST(RCL, "WORD PTR [1234H],CL",       0xD3, 0026, 0x34, 0x12);
    TEST(RCL, "WORD PTR [DI-52],CL",       0xD3, 0125, 0xCC);
    TEST(RCL, "WORD PTR [BP+1234H],CL",    0xD3, 0226, 0x34, 0x12);
    TEST(RCL, "WORD PTR [BX+SI],CL",       0xD3, 0020);
    TEST(RCL, "WORD PTR [BX+DI+52],CL",    0xD3, 0121, 0x34);
    TEST(RCL, "WORD PTR [BP+SI+1234H],CL", 0xD3, 0222, 0x34, 0x12);

    TEST(RCR, "CH,1",                      0xD0, 0335);
    TEST(RCR, "BYTE PTR [SI],1",           0xD0, 0034);
    TEST(RCR, "BYTE PTR [1234H],1",        0xD0, 0036, 0x34, 0x12);
    TEST(RCR, "BYTE PTR [DI-52],1",        0xD0, 0135, 0xCC);
    TEST(RCR, "BYTE PTR [BP+1234H],1",     0xD0, 0236, 0x34, 0x12);
    TEST(RCR, "BYTE PTR [BX+SI],1",        0xD0, 0030);
    TEST(RCR, "BYTE PTR [BX+DI+52],1",     0xD0, 0131, 0x34);
    TEST(RCR, "BYTE PTR [BP+SI+1234H],1",  0xD0, 0232, 0x34, 0x12);
    TEST(RCR, "BP,1",                      0xD1, 0335);
    TEST(RCR, "WORD PTR [SI],1",           0xD1, 0034);
    TEST(RCR, "WORD PTR [1234H],1",        0xD1, 0036, 0x34, 0x12);
    TEST(RCR, "WORD PTR [DI-52],1",        0xD1, 0135, 0xCC);
    TEST(RCR, "WORD PTR [BP+1234H],1",     0xD1, 0236, 0x34, 0x12);
    TEST(RCR, "WORD PTR [BX+SI],1",        0xD1, 0030);
    TEST(RCR, "WORD PTR [BX+DI+52],1",     0xD1, 0131, 0x34);
    TEST(RCR, "WORD PTR [BP+SI+1234H],1",  0xD1, 0232, 0x34, 0x12);
    TEST(RCR, "CH,CL",                     0xD2, 0335);
    TEST(RCR, "BYTE PTR [SI],CL",          0xD2, 0034);
    TEST(RCR, "BYTE PTR [1234H],CL",       0xD2, 0036, 0x34, 0x12);
    TEST(RCR, "BYTE PTR [DI-52],CL",       0xD2, 0135, 0xCC);
    TEST(RCR, "BYTE PTR [BP+1234H],CL",    0xD2, 0236, 0x34, 0x12);
    TEST(RCR, "BYTE PTR [BX+SI],CL",       0xD2, 0030);
    TEST(RCR, "BYTE PTR [BX+DI+52],CL",    0xD2, 0131, 0x34);
    TEST(RCR, "BYTE PTR [BP+SI+1234H],CL", 0xD2, 0232, 0x34, 0x12);
    TEST(RCR, "BP,CL",                     0xD3, 0335);
    TEST(RCR, "WORD PTR [SI],CL",          0xD3, 0034);
    TEST(RCR, "WORD PTR [1234H],CL",       0xD3, 0036, 0x34, 0x12);
    TEST(RCR, "WORD PTR [DI-52],CL",       0xD3, 0135, 0xCC);
    TEST(RCR, "WORD PTR [BP+1234H],CL",    0xD3, 0236, 0x34, 0x12);
    TEST(RCR, "WORD PTR [BX+SI],CL",       0xD3, 0030);
    TEST(RCR, "WORD PTR [BX+DI+52],CL",    0xD3, 0131, 0x34);
    TEST(RCR, "WORD PTR [BP+SI+1234H],CL", 0xD3, 0232, 0x34, 0x12);

    TEST(AND, "AL,CL",            0x20, 0310);
    TEST(AND, "DL,BL",            0x20, 0332);
    TEST(AND, "AH,CH",            0x20, 0354);
    TEST(AND, "[SI],DH",          0x20, 0064);
    TEST(AND, "[1234H],BH",       0x20, 0076, 0x34, 0x12);
    TEST(AND, "[DI+52],AL",       0x20, 0105, 0x34);
    TEST(AND, "[BP+1234H],CL",    0x20, 0216, 0x34, 0x12);
    TEST(AND, "[BX+SI],DL",       0x20, 0020);
    TEST(AND, "[BX+DI+52],BL",    0x20, 0131, 0x34);
    TEST(AND, "[BP+SI+1234H],AH", 0x20, 0242, 0x34, 0x12);
    TEST(AND, "DH,[SI]",          0x22, 0064);
    TEST(AND, "BH,[1234H]",       0x22, 0076, 0x34, 0x12);
    TEST(AND, "AL,[DI-52]",       0x22, 0105, 0xCC);
    TEST(AND, "CL,[BP+1234H]",    0x22, 0216, 0x34, 0x12);
    TEST(AND, "DL,[BX+SI]",       0x22, 0020);
    TEST(AND, "BL,[BX+DI-52]",    0x22, 0131, 0xCC);
    TEST(AND, "AH,[BP+SI+1234H]", 0x22, 0242, 0x34, 0x12);
    TEST(AND, "AX,CX",            0x21, 0310);
    TEST(AND, "DX,BX",            0x21, 0332);
    TEST(AND, "SP,BP",            0x21, 0354);
    TEST(AND, "[SI],SI",          0x21, 0064);
    TEST(AND, "[1234H],DI",       0x21, 0076, 0x34, 0x12);
    TEST(AND, "[DI-52],AX",       0x21, 0105, 0xCC);
    TEST(AND, "[BP+1234H],CX",    0x21, 0216, 0x34, 0x12);
    TEST(AND, "[BX+SI],DX",       0x21, 0020);
    TEST(AND, "[BX+DI+52],BX",    0x21, 0131, 0x34);
    TEST(AND, "[BP+SI+1234H],SP", 0x21, 0242, 0x34, 0x12);
    TEST(AND, "SI,[SI]",          0x23, 0064);
    TEST(AND, "DI,[1234H]",       0x23, 0076, 0x34, 0x12);
    TEST(AND, "AX,[DI+52]",       0x23, 0105, 0x34);
    TEST(AND, "CX,[BP+1234H]",    0x23, 0216, 0x34, 0x12);
    TEST(AND, "DX,[BX+SI]",       0x23, 0020);
    TEST(AND, "BX,[BX+DI+52]",    0x23, 0131, 0x34);
    TEST(AND, "SP,[BP+SI+1234H]", 0x23, 0242, 0x34, 0x12);

    TEST(AND, "AL,56H",                       0x80, 0340, 0x56);
    TEST(AND, "CL,56H",                       0x80, 0341, 0x56);
    TEST(AND, "BYTE PTR [SI],56H",            0x80, 0044, 0x56);
    TEST(AND, "BYTE PTR [89ABH],56H",         0x80, 0046, 0xAB, 0x89, 0x56);
    TEST(AND, "BYTE PTR [DI-52],56H",         0x80, 0145, 0xCC, 0x56);
    TEST(AND, "BYTE PTR [BP+89ABH],56H",      0x80, 0246, 0xAB, 0x89, 0x56);
    TEST(AND, "BYTE PTR [BX+SI],56H",         0x80, 0040, 0x56);
    TEST(AND, "BYTE PTR [BX+DI+52],56H",      0x80, 0141, 0x34, 0x56);
    TEST(AND, "BYTE PTR [BP+SI+1234H],56H",   0x80, 0242, 0x34, 0x12, 0x56);
    TEST(AND, "AX,5678H",                     0x81, 0340, 0x78, 0x56);
    TEST(AND, "CX,5678H",                     0x81, 0341, 0x78, 0x56);
    TEST(AND, "WORD PTR [SI],5678H",          0x81, 0044, 0x78, 0x56);
    TEST(AND, "WORD PTR [1234H],5678H",       0x81, 0046, 0x34, 0x12, 0x78, 0x56);
    TEST(AND, "WORD PTR [DI+52],5678H",       0x81, 0145, 0x34, 0x78, 0x56);
    TEST(AND, "WORD PTR [BP+1234H],5678H",    0x81, 0246, 0x34, 0x12, 0x78, 0x56);
    TEST(AND, "WORD PTR [BX+SI],5678H",       0x81, 0040, 0x78, 0x56);
    TEST(AND, "WORD PTR [BX+DI-52],5678H",    0x81, 0141, 0xCC, 0x78, 0x56);
    TEST(AND, "WORD PTR [BP+SI+89ABH],5678H", 0x81, 0242, 0xAB, 0x89, 0x78, 0x56);
    TEST(AND, "AX,56H",                       0x83, 0340, 0x56);
    TEST(AND, "AX,-16",                       0x83, 0340, 0xF0);
    TEST(AND, "CX,56H",                       0x83, 0341, 0x56);
    TEST(AND, "CX,-16",                       0x83, 0341, 0xF0);
    TEST(AND, "WORD PTR [SI],56H",            0x83, 0044, 0x56);
    TEST(AND, "WORD PTR [1234H],56H",         0x83, 0046, 0x34, 0x12, 0x56);
    TEST(AND, "WORD PTR [DI+52],56H",         0x83, 0145, 0x34, 0x56);
    TEST(AND, "WORD PTR [BP+1234H],56H",      0x83, 0246, 0x34, 0x12, 0x56);
    TEST(AND, "WORD PTR [BX+SI],56H",         0x83, 0040, 0x56);
    TEST(AND, "WORD PTR [BX+DI-52],56H",      0x83, 0141, 0xCC, 0x56);
    TEST(AND, "WORD PTR [BP+SI+89ABH],56H",   0x83, 0242, 0xAB, 0x89, 0x56);

    TEST(AND, "AL,56H",   0x24, 0x56);
    TEST(AND, "AX,5678H", 0x25, 0x78, 0x56);

    TEST(TEST, "AL,CL",            0x84, 0301);
    TEST(TEST, "DL,BL",            0x84, 0323);
    TEST(TEST, "AH,CH",            0x84, 0345);
    TEST(TEST, "DH,[SI]",          0x84, 0064);
    TEST(TEST, "BH,[1234H]",       0x84, 0076, 0x34, 0x12);
    TEST(TEST, "AL,[DI-52]",       0x84, 0105, 0xCC);
    TEST(TEST, "CL,[BP+1234H]",    0x84, 0216, 0x34, 0x12);
    TEST(TEST, "DL,[BX+SI]",       0x84, 0020);
    TEST(TEST, "BL,[BX+DI-52]",    0x84, 0131, 0xCC);
    TEST(TEST, "AH,[BP+SI+1234H]", 0x84, 0242, 0x34, 0x12);
    TEST(TEST, "AX,CX",            0x85, 0301);
    TEST(TEST, "DX,BX",            0x85, 0323);
    TEST(TEST, "SP,BP",            0x85, 0345);
    TEST(TEST, "SI,[SI]",          0x85, 0064);
    TEST(TEST, "DI,[1234H]",       0x85, 0076, 0x34, 0x12);
    TEST(TEST, "AX,[DI+52]",       0x85, 0105, 0x34);
    TEST(TEST, "CX,[BP+1234H]",    0x85, 0216, 0x34, 0x12);
    TEST(TEST, "DX,[BX+SI]",       0x85, 0020);
    TEST(TEST, "BX,[BX+DI+52]",    0x85, 0131, 0x34);
    TEST(TEST, "SP,[BP+SI+1234H]", 0x85, 0242, 0x34, 0x12);

    TEST(TEST, "AL,56H",                       0xF6, 0300, 0x56);
    TEST(TEST, "CL,56H",                       0xF6, 0301, 0x56);
    TEST(TEST, "BYTE PTR [SI],56H",            0xF6, 0004, 0x56);
    TEST(TEST, "BYTE PTR [89ABH],56H",         0xF6, 0006, 0xAB, 0x89, 0x56);
    TEST(TEST, "BYTE PTR [DI-52],56H",         0xF6, 0105, 0xCC, 0x56);
    TEST(TEST, "BYTE PTR [BP+89ABH],56H",      0xF6, 0206, 0xAB, 0x89, 0x56);
    TEST(TEST, "BYTE PTR [BX+SI],56H",         0xF6, 0000, 0x56);
    TEST(TEST, "BYTE PTR [BX+DI+52],56H",      0xF6, 0101, 0x34, 0x56);
    TEST(TEST, "BYTE PTR [BP+SI+1234H],56H",   0xF6, 0202, 0x34, 0x12, 0x56);
    TEST(TEST, "AX,5678H",                     0xF7, 0300, 0x78, 0x56);
    TEST(TEST, "CX,5678H",                     0xF7, 0301, 0x78, 0x56);
    TEST(TEST, "WORD PTR [SI],5678H",          0xF7, 0004, 0x78, 0x56);
    TEST(TEST, "WORD PTR [1234H],5678H",       0xF7, 0006, 0x34, 0x12, 0x78, 0x56);
    TEST(TEST, "WORD PTR [DI+52],5678H",       0xF7, 0105, 0x34, 0x78, 0x56);
    TEST(TEST, "WORD PTR [BP+1234H],5678H",    0xF7, 0206, 0x34, 0x12, 0x78, 0x56);
    TEST(TEST, "WORD PTR [BX+SI],5678H",       0xF7, 0000, 0x78, 0x56);
    TEST(TEST, "WORD PTR [BX+DI-52],5678H",    0xF7, 0101, 0xCC, 0x78, 0x56);
    TEST(TEST, "WORD PTR [BP+SI+89ABH],5678H", 0xF7, 0202, 0xAB, 0x89, 0x78, 0x56);

    TEST(TEST, "AL,56H",   0xA8, 0x56);
    TEST(TEST, "AX,5678H", 0xA9, 0x78, 0x56);

    TEST(OR, "AL,CL",            0x08, 0310);
    TEST(OR, "DL,BL",            0x08, 0332);
    TEST(OR, "AH,CH",            0x08, 0354);
    TEST(OR, "[SI],DH",          0x08, 0064);
    TEST(OR, "[1234H],BH",       0x08, 0076, 0x34, 0x12);
    TEST(OR, "[DI+52],AL",       0x08, 0105, 0x34);
    TEST(OR, "[BP+1234H],CL",    0x08, 0216, 0x34, 0x12);
    TEST(OR, "[BX+SI],DL",       0x08, 0020);
    TEST(OR, "[BX+DI+52],BL",    0x08, 0131, 0x34);
    TEST(OR, "[BP+SI+1234H],AH", 0x08, 0242, 0x34, 0x12);
    TEST(OR, "DH,[SI]",          0x0A, 0064);
    TEST(OR, "BH,[1234H]",       0x0A, 0076, 0x34, 0x12);
    TEST(OR, "AL,[DI-52]",       0x0A, 0105, 0xCC);
    TEST(OR, "CL,[BP+1234H]",    0x0A, 0216, 0x34, 0x12);
    TEST(OR, "DL,[BX+SI]",       0x0A, 0020);
    TEST(OR, "BL,[BX+DI-52]",    0x0A, 0131, 0xCC);
    TEST(OR, "AH,[BP+SI+1234H]", 0x0A, 0242, 0x34, 0x12);
    TEST(OR, "AX,CX",            0x09, 0310);
    TEST(OR, "DX,BX",            0x09, 0332);
    TEST(OR, "SP,BP",            0x09, 0354);
    TEST(OR, "[SI],SI",          0x09, 0064);
    TEST(OR, "[1234H],DI",       0x09, 0076, 0x34, 0x12);
    TEST(OR, "[DI-52],AX",       0x09, 0105, 0xCC);
    TEST(OR, "[BP+1234H],CX",    0x09, 0216, 0x34, 0x12);
    TEST(OR, "[BX+SI],DX",       0x09, 0020);
    TEST(OR, "[BX+DI+52],BX",    0x09, 0131, 0x34);
    TEST(OR, "[BP+SI+1234H],SP", 0x09, 0242, 0x34, 0x12);
    TEST(OR, "SI,[SI]",          0x0B, 0064);
    TEST(OR, "DI,[1234H]",       0x0B, 0076, 0x34, 0x12);
    TEST(OR, "AX,[DI+52]",       0x0B, 0105, 0x34);
    TEST(OR, "CX,[BP+1234H]",    0x0B, 0216, 0x34, 0x12);
    TEST(OR, "DX,[BX+SI]",       0x0B, 0020);
    TEST(OR, "BX,[BX+DI+52]",    0x0B, 0131, 0x34);
    TEST(OR, "SP,[BP+SI+1234H]", 0x0B, 0242, 0x34, 0x12);

    TEST(OR, "AL,56H",                       0x80, 0310, 0x56);
    TEST(OR, "CL,56H",                       0x80, 0311, 0x56);
    TEST(OR, "BYTE PTR [SI],56H",            0x80, 0014, 0x56);
    TEST(OR, "BYTE PTR [89ABH],56H",         0x80, 0016, 0xAB, 0x89, 0x56);
    TEST(OR, "BYTE PTR [DI-52],56H",         0x80, 0115, 0xCC, 0x56);
    TEST(OR, "BYTE PTR [BP+89ABH],56H",      0x80, 0216, 0xAB, 0x89, 0x56);
    TEST(OR, "BYTE PTR [BX+SI],56H",         0x80, 0010, 0x56);
    TEST(OR, "BYTE PTR [BX+DI+52],56H",      0x80, 0111, 0x34, 0x56);
    TEST(OR, "BYTE PTR [BP+SI+1234H],56H",   0x80, 0212, 0x34, 0x12, 0x56);
    TEST(OR, "AX,5678H",                     0x81, 0310, 0x78, 0x56);
    TEST(OR, "CX,5678H",                     0x81, 0311, 0x78, 0x56);
    TEST(OR, "WORD PTR [SI],5678H",          0x81, 0014, 0x78, 0x56);
    TEST(OR, "WORD PTR [1234H],5678H",       0x81, 0016, 0x34, 0x12, 0x78, 0x56);
    TEST(OR, "WORD PTR [DI+52],5678H",       0x81, 0115, 0x34, 0x78, 0x56);
    TEST(OR, "WORD PTR [BP+1234H],5678H",    0x81, 0216, 0x34, 0x12, 0x78, 0x56);
    TEST(OR, "WORD PTR [BX+SI],5678H",       0x81, 0010, 0x78, 0x56);
    TEST(OR, "WORD PTR [BX+DI-52],5678H",    0x81, 0111, 0xCC, 0x78, 0x56);
    TEST(OR, "WORD PTR [BP+SI+89ABH],5678H", 0x81, 0212, 0xAB, 0x89, 0x78, 0x56);
    TEST(OR, "AX,56H",                       0x83, 0310, 0x56);
    TEST(OR, "AX,-16",                       0x83, 0310, 0xF0);
    TEST(OR, "CX,56H",                       0x83, 0311, 0x56);
    TEST(OR, "CX,-16",                       0x83, 0311, 0xF0);
    TEST(OR, "WORD PTR [SI],56H",            0x83, 0014, 0x56);
    TEST(OR, "WORD PTR [1234H],56H",         0x83, 0016, 0x34, 0x12, 0x56);
    TEST(OR, "WORD PTR [DI+52],56H",         0x83, 0115, 0x34, 0x56);
    TEST(OR, "WORD PTR [BP+1234H],56H",      0x83, 0216, 0x34, 0x12, 0x56);
    TEST(OR, "WORD PTR [BX+SI],56H",         0x83, 0010, 0x56);
    TEST(OR, "WORD PTR [BX+DI-52],56H",      0x83, 0111, 0xCC, 0x56);
    TEST(OR, "WORD PTR [BP+SI+89ABH],56H",   0x83, 0212, 0xAB, 0x89, 0x56);

    TEST(OR, "AL,56H",   0x0C, 0x56);
    TEST(OR, "AX,5678H", 0x0D, 0x78, 0x56);

    TEST(XOR, "AL,CL",            0x30, 0310);
    TEST(XOR, "DL,BL",            0x30, 0332);
    TEST(XOR, "AH,CH",            0x30, 0354);
    TEST(XOR, "[SI],DH",          0x30, 0064);
    TEST(XOR, "[1234H],BH",       0x30, 0076, 0x34, 0x12);
    TEST(XOR, "[DI+52],AL",       0x30, 0105, 0x34);
    TEST(XOR, "[BP+1234H],CL",    0x30, 0216, 0x34, 0x12);
    TEST(XOR, "[BX+SI],DL",       0x30, 0020);
    TEST(XOR, "[BX+DI+52],BL",    0x30, 0131, 0x34);
    TEST(XOR, "[BP+SI+1234H],AH", 0x30, 0242, 0x34, 0x12);
    TEST(XOR, "DH,[SI]",          0x32, 0064);
    TEST(XOR, "BH,[1234H]",       0x32, 0076, 0x34, 0x12);
    TEST(XOR, "AL,[DI-52]",       0x32, 0105, 0xCC);
    TEST(XOR, "CL,[BP+1234H]",    0x32, 0216, 0x34, 0x12);
    TEST(XOR, "DL,[BX+SI]",       0x32, 0020);
    TEST(XOR, "BL,[BX+DI-52]",    0x32, 0131, 0xCC);
    TEST(XOR, "AH,[BP+SI+1234H]", 0x32, 0242, 0x34, 0x12);
    TEST(XOR, "AX,CX",            0x31, 0310);
    TEST(XOR, "DX,BX",            0x31, 0332);
    TEST(XOR, "SP,BP",            0x31, 0354);
    TEST(XOR, "[SI],SI",          0x31, 0064);
    TEST(XOR, "[1234H],DI",       0x31, 0076, 0x34, 0x12);
    TEST(XOR, "[DI-52],AX",       0x31, 0105, 0xCC);
    TEST(XOR, "[BP+1234H],CX",    0x31, 0216, 0x34, 0x12);
    TEST(XOR, "[BX+SI],DX",       0x31, 0020);
    TEST(XOR, "[BX+DI+52],BX",    0x31, 0131, 0x34);
    TEST(XOR, "[BP+SI+1234H],SP", 0x31, 0242, 0x34, 0x12);
    TEST(XOR, "SI,[SI]",          0x33, 0064);
    TEST(XOR, "DI,[1234H]",       0x33, 0076, 0x34, 0x12);
    TEST(XOR, "AX,[DI+52]",       0x33, 0105, 0x34);
    TEST(XOR, "CX,[BP+1234H]",    0x33, 0216, 0x34, 0x12);
    TEST(XOR, "DX,[BX+SI]",       0x33, 0020);
    TEST(XOR, "BX,[BX+DI+52]",    0x33, 0131, 0x34);
    TEST(XOR, "SP,[BP+SI+1234H]", 0x33, 0242, 0x34, 0x12);

    TEST(XOR, "AL,56H",                       0x80, 0360, 0x56);
    TEST(XOR, "CL,56H",                       0x80, 0361, 0x56);
    TEST(XOR, "BYTE PTR [SI],56H",            0x80, 0064, 0x56);
    TEST(XOR, "BYTE PTR [89ABH],56H",         0x80, 0066, 0xAB, 0x89, 0x56);
    TEST(XOR, "BYTE PTR [DI-52],56H",         0x80, 0165, 0xCC, 0x56);
    TEST(XOR, "BYTE PTR [BP+89ABH],56H",      0x80, 0266, 0xAB, 0x89, 0x56);
    TEST(XOR, "BYTE PTR [BX+SI],56H",         0x80, 0060, 0x56);
    TEST(XOR, "BYTE PTR [BX+DI+52],56H",      0x80, 0161, 0x34, 0x56);
    TEST(XOR, "BYTE PTR [BP+SI+1234H],56H",   0x80, 0262, 0x34, 0x12, 0x56);
    TEST(XOR, "AX,5678H",                     0x81, 0360, 0x78, 0x56);
    TEST(XOR, "CX,5678H",                     0x81, 0361, 0x78, 0x56);
    TEST(XOR, "WORD PTR [SI],5678H",          0x81, 0064, 0x78, 0x56);
    TEST(XOR, "WORD PTR [1234H],5678H",       0x81, 0066, 0x34, 0x12, 0x78, 0x56);
    TEST(XOR, "WORD PTR [DI+52],5678H",       0x81, 0165, 0x34, 0x78, 0x56);
    TEST(XOR, "WORD PTR [BP+1234H],5678H",    0x81, 0266, 0x34, 0x12, 0x78, 0x56);
    TEST(XOR, "WORD PTR [BX+SI],5678H",       0x81, 0060, 0x78, 0x56);
    TEST(XOR, "WORD PTR [BX+DI-52],5678H",    0x81, 0161, 0xCC, 0x78, 0x56);
    TEST(XOR, "WORD PTR [BP+SI+89ABH],5678H", 0x81, 0262, 0xAB, 0x89, 0x78, 0x56);
    TEST(XOR, "AX,56H",                       0x83, 0360, 0x56);
    TEST(XOR, "AX,-16",                       0x83, 0360, 0xF0);
    TEST(XOR, "CX,56H",                       0x83, 0361, 0x56);
    TEST(XOR, "CX,-16",                       0x83, 0361, 0xF0);
    TEST(XOR, "WORD PTR [SI],56H",            0x83, 0064, 0x56);
    TEST(XOR, "WORD PTR [1234H],56H",         0x83, 0066, 0x34, 0x12, 0x56);
    TEST(XOR, "WORD PTR [DI+52],56H",         0x83, 0165, 0x34, 0x56);
    TEST(XOR, "WORD PTR [BP+1234H],56H",      0x83, 0266, 0x34, 0x12, 0x56);
    TEST(XOR, "WORD PTR [BX+SI],56H",         0x83, 0060, 0x56);
    TEST(XOR, "WORD PTR [BX+DI-52],56H",      0x83, 0161, 0xCC, 0x56);
    TEST(XOR, "WORD PTR [BP+SI+89ABH],56H",   0x83, 0262, 0xAB, 0x89, 0x56);

    TEST(XOR, "AL,56H",   0x34, 0x56);
    TEST(XOR, "AX,5678H", 0x35, 0x78, 0x56);
}

static void test_string_manipulation() {
    dis8086.setRepeatHasStringInstruction(true);
    TEST(REPNE, "MOVSB", 0xF2, 0xA4);
    TEST(REPNE, "MOVSW", 0xF2, 0xA5);
    TEST(REPNE, "CMPSB", 0xF2, 0xA6);
    TEST(REPNE, "CMPSW", 0xF2, 0xA7);
    TEST(REPNE, "STOSB", 0xF2, 0xAA);
    TEST(REPNE, "STOSW", 0xF2, 0xAB);
    TEST(REPNE, "LODSB", 0xF2, 0xAC);
    TEST(REPNE, "LODSW", 0xF2, 0xAD);
    TEST(REPNE, "SCASB", 0xF2, 0xAE);
    TEST(REPNE, "SCASW", 0xF2, 0xAF);
    ERUI(REPNE, "NOP",   0xF2, 0x90);
    ERUI(REPNE, "JMP [SI]", 0xF2, 0xFF, 0044);

    TEST(REP, "MOVSB", 0xF3, 0xA4);
    TEST(REP, "MOVSW", 0xF3, 0xA5);
    TEST(REP, "CMPSB", 0xF3, 0xA6);
    TEST(REP, "CMPSW", 0xF3, 0xA7);
    TEST(REP, "STOSB", 0xF3, 0xAA);
    TEST(REP, "STOSW", 0xF3, 0xAB);
    TEST(REP, "LODSB", 0xF3, 0xAC);
    TEST(REP, "LODSW", 0xF3, 0xAD);
    TEST(REP, "SCASB", 0xF3, 0xAE);
    TEST(REP, "SCASW", 0xF3, 0xAF);

    TEST(MOVSB, "", 0xA4);
    TEST(MOVSW, "", 0xA5);

    TEST(CMPSB, "", 0xA6);
    TEST(CMPSW, "", 0xA7);

    TEST(SCASB, "", 0xAE);
    TEST(SCASW, "", 0xAF);

    TEST(LODSB, "", 0xAC);
    TEST(LODSW, "", 0xAD);

    TEST(STOSB, "", 0xAA);
    TEST(STOSW, "", 0xAB);
}

static void test_control_transfer() {
    disassembler.setRelativeTarget(true);

    TEST(CALL, "$", 0xE8, 0xFD, 0xFF);

    TEST(CALL, "[SI]",          0xFF, 0024);
    TEST(CALL, "[1234H]",       0xFF, 0026, 0x34, 0x12);
    TEST(CALL, "[DI-52]",       0xFF, 0125, 0xCC);
    TEST(CALL, "[BP+1234H]",    0xFF, 0226, 0x34, 0x12);
    TEST(CALL, "[BX+SI]",       0xFF, 0020);
    TEST(CALL, "[BX+DI+52]",    0xFF, 0121, 0x34);
    TEST(CALL, "[BP+SI+1234H]", 0xFF, 0222, 0x34, 0x12);

    TEST(CALLF, "1234H:5678H",   0x9A, 0x78, 0x56, 0x34, 0x12);

    TEST(CALLF, "[SI]",          0xFF, 0034);
    TEST(CALLF, "[1234H]",       0xFF, 0036, 0x34, 0x12);
    TEST(CALLF, "[DI-52]",       0xFF, 0135, 0xCC);
    TEST(CALLF, "[BP+1234H]",    0xFF, 0236, 0x34, 0x12);
    TEST(CALLF, "[BX+SI]",       0xFF, 0030);
    TEST(CALLF, "[BX+DI+52]",    0xFF, 0131, 0x34);
    TEST(CALLF, "[BP+SI+1234H]", 0xFF, 0232, 0x34, 0x12);

    TEST(JMP, "$+1234H", 0xE9, 0x31, 0x12);

    TEST(JMP, "$",       0xEB, 0xFE);

    TEST(JMP, "[SI]",          0xFF, 0044);
    TEST(JMP, "[1234H]",       0xFF, 0046, 0x34, 0x12);
    TEST(JMP, "[DI-52]",       0xFF, 0145, 0xCC);
    TEST(JMP, "[BP+1234H]",    0xFF, 0246, 0x34, 0x12);
    TEST(JMP, "[BX+SI]",       0xFF, 0040);
    TEST(JMP, "[BX+DI+52]",    0xFF, 0141, 0x34);
    TEST(JMP, "[BP+SI+1234H]", 0xFF, 0242, 0x34, 0x12);

    TEST(JMPF, "1234H:5678H",   0xEA, 0x78, 0x56, 0x34, 0x12);

    TEST(JMPF, "[SI]",          0xFF, 0054);
    TEST(JMPF, "[1234H]",       0xFF, 0056, 0x34, 0x12);
    TEST(JMPF, "[DI-52]",       0xFF, 0155, 0xCC);
    TEST(JMPF, "[BP+1234H]",    0xFF, 0256, 0x34, 0x12);
    TEST(JMPF, "[BX+SI]",       0xFF, 0050);
    TEST(JMPF, "[BX+DI+52]",    0xFF, 0151, 0x34);
    TEST(JMPF, "[BP+SI+1234H]", 0xFF, 0252, 0x34, 0x12);

    TEST(RET, "",    0xC3);
    TEST(RET, "16",  0xC2, 0x10, 0x00);
    TEST(RETF, "",   0xCB);
    TEST(RETF, "16", 0xCA, 0x10, 0x00);

    TEST(JE,  "$", 0x74, 0xFE);
    TEST(JL,  "$", 0x7C, 0xFE);
    TEST(JLE, "$", 0x7E, 0xFE);
    TEST(JB,  "$", 0x72, 0xFE);
    TEST(JBE, "$", 0x76, 0xFE);
    TEST(JPE, "$", 0x7A, 0xFE);
    TEST(JO,  "$", 0x70, 0xFE);
    TEST(JS,  "$", 0x78, 0xFE);
    TEST(JNE, "$", 0x75, 0xFE);
    TEST(JGE, "$", 0x7D, 0xFE);
    TEST(JG,  "$", 0x7F, 0xFE);
    TEST(JAE, "$", 0x73, 0xFE);
    TEST(JA,  "$", 0x77, 0xFE);
    TEST(JPO, "$", 0x7B, 0xFE);
    TEST(JNO, "$", 0x71, 0xFE);
    TEST(JNS, "$", 0x79, 0xFE);

    TEST(LOOP,   "$", 0xE2, 0xFE);
    TEST(LOOPE,  "$", 0xE1, 0xFE);
    TEST(LOOPNE, "$", 0xE0, 0xFE);
    TEST(JCXZ,   "$", 0xE3, 0xFE);

    TEST(INT,  "2", 0xCD, 0x02);
    TEST(INT,  "3", 0xCC);
    TEST(INTO, "",  0xCE);
    TEST(IRET, "",  0xCF);
}

static void test_processor_control() {
    TEST(CLC,  "", 0xF8);
    TEST(CMC,  "", 0xF5);
    TEST(STC,  "", 0xF9);
    TEST(CLD,  "", 0xFC);
    TEST(STD,  "", 0xFD);
    TEST(CLI,  "", 0xFA);
    TEST(STI,  "", 0xFB);
    TEST(HLT,  "", 0xF4);
    TEST(WAIT, "", 0x9B);
    TEST(LOCK, "", 0xF0);
    TEST(NOP,  "", 0x90);
}

static void test_segment_override() {
    TEST(MOV, "ES:[BX],AH",    0x26, 0x88, 0047);
    TEST(MOV, "ES:[BP+0],AH",  0x26, 0x88, 0146, 0x00);
    TEST(MOV, "ES:[SI],AH",    0x26, 0x88, 0044);
    TEST(MOV, "ES:[DI],AH",    0x26, 0x88, 0045);
    TEST(MOV, "ES:[1234H],AH", 0x26, 0x88, 0046, 0x34, 0x12);
    TEST(MOV, "CS:[BX],AH",    0x2E, 0x88, 0047);
    TEST(MOV, "CS:[BP+0],AH",  0x2E, 0x88, 0146, 0x00);
    TEST(MOV, "CS:[SI],AH",    0x2E, 0x88, 0044);
    TEST(MOV, "CS:[DI],AH",    0x2E, 0x88, 0045);
    TEST(MOV, "CS:[1234H],AH", 0x2E, 0x88, 0046, 0x34, 0x12);
    TEST(MOV, "SS:[BX],AH",    0x36, 0x88, 0047);
    TEST(MOV, "SS:[BP+0],AH",  0x36, 0x88, 0146, 0x00);
    TEST(MOV, "SS:[SI],AH",    0x36, 0x88, 0044);
    TEST(MOV, "SS:[DI],AH",    0x36, 0x88, 0045);
    TEST(MOV, "SS:[1234H],AH", 0x36, 0x88, 0046, 0x34, 0x12);
    TEST(MOV, "DS:[BX],AH",    0x3E, 0x88, 0047);
    TEST(MOV, "DS:[BP+0],AH",  0x3E, 0x88, 0146, 0x00);
    TEST(MOV, "DS:[SI],AH",    0x3E, 0x88, 0044);
    TEST(MOV, "DS:[DI],AH",    0x3E, 0x88, 0045);
    TEST(MOV, "DS:[1234H],AH", 0x3E, 0x88, 0046, 0x34, 0x12);

    TEST(JMP, "ES:[SI]", 0x26, 0xFF, 0044);
    TEST(JMP, "CS:[SI]", 0x2E, 0xFF, 0044);
    TEST(JMP, "SS:[SI]", 0x36, 0xFF, 0044);
    TEST(JMP, "DS:[SI]", 0x3E, 0xFF, 0044);

#define ERRS(name, opr, ...) VASSERT(ILLEGAL_SEGMENT, 0, name, opr, __VA_ARGS__)

    TEST(MOVSB, "ES:[DI],ES:[SI]", 0x26, 0xA4);
    TEST(MOVSB, "ES:[DI],CS:[SI]", 0x2E, 0xA4);
    TEST(MOVSW, "ES:[DI],SS:[SI]", 0x36, 0xA5);
    TEST(MOVSW, "ES:[DI],DS:[SI]", 0x3E, 0xA5);
    TEST(CMPSB, "ES:[SI],ES:[DI]", 0x26, 0xA6);
    TEST(CMPSB, "CS:[SI],ES:[DI]", 0x2E, 0xA6);
    TEST(CMPSW, "SS:[SI],ES:[DI]", 0x36, 0xA7);
    TEST(CMPSW, "DS:[SI],ES:[DI]", 0x3E, 0xA7);
    TEST(LODSB, "ES:[SI]", 0x26, 0xAC);
    TEST(LODSB, "CS:[SI]", 0x2E, 0xAC);
    TEST(LODSW, "SS:[SI]", 0x36, 0xAD);
    TEST(LODSW, "DS:[SI]", 0x3E, 0xAD);
    ERRS(STOSB, "ES:[DI]", 0x26, 0xAA);
    ERRS(STOSB, "CS:[DI]", 0x2E, 0xAA);
    ERRS(STOSW, "SS:[DI]", 0x36, 0xAB);
    ERRS(STOSW, "DS:[DI]", 0x3E, 0xAB);
    ERRS(SCASB, "ES:[DI]", 0x26, 0xAE);
    ERRS(SCASB, "CS:[DI]", 0x2E, 0xAE);
    ERRS(SCASW, "SS:[DI]", 0x36, 0xAF);
    ERRS(SCASW, "DS:[DI]", 0x3E, 0xAF);
}

static void test_illegal() {
    ERRR(POP, "CS", 0x0F);

    for (uint8_t opc = 0x60; opc < 0x70; opc++)
        ERRI(opc);

    ERRI(0x82);

    for (uint8_t mod = 0; mod < 4; mod++) {
        for (uint8_t reg = 0; reg < 8; reg++) {
            for (uint8_t r_m = 0; r_m < 8; r_m++) {
                const uint8_t modReg = (mod << 6) | (reg << 3) | r_m;
                if (reg == 1) {
                    ERRI(0xF6, modReg);
                    ERRI(0xF7, modReg);
                }
                if (reg == 6) {
                    ERRI(0xD0, modReg);
                    ERRI(0xD1, modReg);
                    ERRI(0xD2, modReg);
                    ERRI(0xD3, modReg);
                }
                if (reg == 7)
                    ERRI(0xFF, modReg);
                if (reg != 0 && reg != 1)
                    ERRI(0xFE, modReg);
                if (reg != 0) {
                    ERRI(0x8F, modReg);
                    ERRI(0xC6, modReg);
                    ERRI(0xC7, modReg);
                }
            }
        }
    }

    for (uint8_t second = 0x0B; second != 0x0A; second++) {
        ERRI(0xD4, second);
        ERRI(0xD5, second);
    }

    // Co-processor instructions
    ERRI(0xD8);
    ERRI(0xD9);
    ERRI(0xDA);
    ERRI(0xDB);
    ERRI(0xDC);
    ERRI(0xDD);
    ERRI(0xDE);
    ERRI(0xDF);
}
// clang-format on

const char *run_cpu_test() {
    RUN_TEST(test_cpu);
    return disassembler.listCpu();
}

void run_tests(const char *cpu) {
    disassembler.setCpu(cpu);
    RUN_TEST(test_data_transfer);
    RUN_TEST(test_arithmetic);
    RUN_TEST(test_logic);
    RUN_TEST(test_string_manipulation);
    RUN_TEST(test_control_transfer);
    RUN_TEST(test_processor_control);
    RUN_TEST(test_segment_override);
    RUN_TEST(test_illegal);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

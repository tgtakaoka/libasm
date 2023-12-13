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

static bool v30() {
    return strcmp_P("V30", disassembler.config().cpu_P()) == 0;
}

static bool is80186() {
    return strcmp_P("80186", disassembler.config().cpu_P()) == 0 || v30();
}

static bool is8086() {
    return strcmp_P("8086", disassembler.config().cpu_P()) == 0;
}

static void set_up() {
    disassembler.reset();
}

static void tear_down() {
    symtab.reset();
}

// clang-format off
void test_cpu() {
    EQUALS("cpu 8086", true,   disassembler.setCpu("8086"));
    EQUALS_P("cpu 8086", "8086", disassembler.config().cpu_P());

    EQUALS("cpu i8086", true,   disassembler.setCpu("i8086"));
    EQUALS_P("cpu i8086", "8086", disassembler.config().cpu_P());

    EQUALS("cpu 80186", true,   disassembler.setCpu("80186"));
    EQUALS_P("cpu 80186", "80186", disassembler.config().cpu_P());

    EQUALS("cpu i80186", true,   disassembler.setCpu("i80186"));
    EQUALS_P("cpu i80186", "80186", disassembler.config().cpu_P());

    EQUALS("cpu V30", true,   disassembler.setCpu("V30"));
    EQUALS_P("cpu V30", "V30", disassembler.config().cpu_P());
}

static void test_data_transfer() {
    TEST("MOV", "AL, CL",            0x88, 0310);
    TEST("MOV", "DL, BL",            0x88, 0332);
    TEST("MOV", "AH, CH",            0x88, 0354);
    TEST("MOV", "[SI], DH",          0x88, 0064);
    TEST("MOV", "[SI+0], DH",        0x88, 0164, 0x00);
    TEST("MOV", "[1234H], BH",       0x88, 0076, 0x34, 0x12);
    TEST("MOV", "[DI+52], AL",       0x88, 0105, 0x34);
    TEST("MOV", "[BP+1234H], CL",    0x88, 0216, 0x34, 0x12);
    TEST("MOV", "[BP+0], CL",        0x88, 0116, 0x00);
    TEST("MOV", "[BX+SI], DL",       0x88, 0020);
    TEST("MOV", "[BX+DI+52], BL",    0x88, 0131, 0x34);
    TEST("MOV", "[BP+SI+1234H], AH", 0x88, 0242, 0x34, 0x12);
    TEST("MOV", "DH, [SI]",          0x8A, 0064);
    TEST("MOV", "BH, [1234H]",       0x8A, 0076, 0x34, 0x12);
    TEST("MOV", "AL, [DI-52]",       0x8A, 0105, 0xCC);
    TEST("MOV", "CL, [BP+1234H]",    0x8A, 0216, 0x34, 0x12);
    TEST("MOV", "DL, [BX+SI]",       0x8A, 0020);
    TEST("MOV", "BL, [BX+DI-52]",    0x8A, 0131, 0xCC);
    TEST("MOV", "AH, [BP+SI+1234H]", 0x8A, 0242, 0x34, 0x12);
    TEST("MOV", "AX, CX",            0x89, 0310);
    TEST("MOV", "DX, BX",            0x89, 0332);
    TEST("MOV", "SP, BP",            0x89, 0354);
    TEST("MOV", "[SI], SI",          0x89, 0064);
    TEST("MOV", "[1234H], DI",       0x89, 0076, 0x34, 0x12);
    TEST("MOV", "[DI-52], AX",       0x89, 0105, 0xCC);
    TEST("MOV", "[BP+1234H], CX",    0x89, 0216, 0x34, 0x12);
    TEST("MOV", "[BX+SI], DX",       0x89, 0020);
    TEST("MOV", "[BX+DI+52], BX",    0x89, 0131, 0x34);
    TEST("MOV", "[BP+SI+1234H], SP", 0x89, 0242, 0x34, 0x12);
    TEST("MOV", "SI, [SI]",          0x8B, 0064);
    TEST("MOV", "DI, [1234H]",       0x8B, 0076, 0x34, 0x12);
    TEST("MOV", "AX, [DI+52]",       0x8B, 0105, 0x34);
    TEST("MOV", "CX, [BP+1234H]",    0x8B, 0216, 0x34, 0x12);
    TEST("MOV", "DX, [BX+SI]",       0x8B, 0020);
    TEST("MOV", "BX, [BX+DI+52]",    0x8B, 0131, 0x34);
    TEST("MOV", "SP, [BP+SI+1234H]", 0x8B, 0242, 0x34, 0x12);

    TEST("MOV", "AL, 56H",                       0xC6, 0300, 0x56);
    TEST("MOV", "BYTE PTR [SI], 56H",            0xC6, 0004, 0x56);
    TEST("MOV", "BYTE PTR [89ABH], 56H",         0xC6, 0006, 0xAB, 0x89, 0x56);
    TEST("MOV", "BYTE PTR [DI-52], 56H",         0xC6, 0105, 0xCC, 0x56);
    TEST("MOV", "BYTE PTR [BP+89ABH], 56H",      0xC6, 0206, 0xAB, 0x89, 0x56);
    TEST("MOV", "BYTE PTR [BX+SI], 56H",         0xC6, 0000, 0x56);
    TEST("MOV", "BYTE PTR [BX+DI+52], 56H",      0xC6, 0101, 0x34, 0x56);
    TEST("MOV", "BYTE PTR [BP+SI+1234H], 56H",   0xC6, 0202, 0x34, 0x12, 0x56);
    TEST("MOV", "AX, 5678H",                     0xC7, 0300, 0x78, 0x56);
    TEST("MOV", "WORD PTR [SI], 5678H",          0xC7, 0004, 0x78, 0x56);
    TEST("MOV", "WORD PTR [1234H], 5678H",       0xC7, 0006, 0x34, 0x12, 0x78, 0x56);
    TEST("MOV", "WORD PTR [DI+52], 5678H",       0xC7, 0105, 0x34, 0x78, 0x56);
    TEST("MOV", "WORD PTR [BP+1234H], 5678H",    0xC7, 0206, 0x34, 0x12, 0x78, 0x56);
    TEST("MOV", "WORD PTR [BX+SI], 5678H",       0xC7, 0000, 0x78, 0x56);
    TEST("MOV", "WORD PTR [BX+DI-52], 5678H",    0xC7, 0101, 0xCC, 0x78, 0x56);
    TEST("MOV", "WORD PTR [BP+SI+89ABH], 5678H", 0xC7, 0202, 0xAB, 0x89, 0x78, 0x56);

    TEST("MOV", "AL, 56H",   0260, 0x56);
    TEST("MOV", "CH, 56H",   0265, 0x56);
    TEST("MOV", "AX, 5678H", 0270, 0x78, 0x56);
    TEST("MOV", "BP, 5678H", 0275, 0x78, 0x56);

    TEST("MOV", "AL, [1234H]", 0xA0, 0x34, 0x12);
    TEST("MOV", "AX, [1234H]", 0xA1, 0x34, 0x12);
    TEST("MOV", "[1234H], AL", 0xA2, 0x34, 0x12);
    TEST("MOV", "[1234H], AX", 0xA3, 0x34, 0x12);

    TEST("MOV", "AX, ES",              0x8C, 0300);
    TEST("MOV", "[SI], CS",            0x8C, 0014);
    TEST("MOV", "[89ABH], SS",         0x8C, 0026, 0xAB, 0x89);
    TEST("MOV", "[DI-52], DS",         0x8C, 0135, 0xCC);
    TEST("MOV", "[BP+89ABH], ES",      0x8C, 0206, 0xAB, 0x89);
    TEST("MOV", "[BX+SI], CS",         0x8C, 0010);
    TEST("MOV", "[BX+DI+52], SS",      0x8C, 0121, 0x34);
    TEST("MOV", "[BP+SI+1234H], DS",   0x8C, 0232, 0x34, 0x12);
    TEST("MOV", "ES, AX",              0x8E, 0300);
    TEST("MOV", "CS, [SI]",            0x8E, 0014);
    TEST("MOV", "SS, [89ABH]",         0x8E, 0026, 0xAB, 0x89);
    TEST("MOV", "DS, [DI-52]",         0x8E, 0135, 0xCC);
    TEST("MOV", "ES, [BP+89ABH]",      0x8E, 0206, 0xAB, 0x89);
    TEST("MOV", "CS, [BX+SI]",         0x8E, 0010);
    TEST("MOV", "SS, [BX+DI+52]",      0x8E, 0121, 0x34);
    TEST("MOV", "DS, [BP+SI+1234H]",   0x8E, 0232, 0x34, 0x12);

    TEST("PUSH", "BP",            0xFF, 0365);
    TEST("PUSH", "[SI]",          0xFF, 0064);
    TEST("PUSH", "[1234H]",       0xFF, 0066, 0x34, 0x12);
    TEST("PUSH", "[DI-52]",       0xFF, 0165, 0xCC);
    TEST("PUSH", "[BP+1234H]",    0xFF, 0266, 0x34, 0x12);
    TEST("PUSH", "[BX+SI]",       0xFF, 0060);
    TEST("PUSH", "[BX+DI+52]",    0xFF, 0161, 0x34);
    TEST("PUSH", "[BP+SI+1234H]", 0xFF, 0262, 0x34, 0x12);
    TEST("PUSH", "BP",            0125);
    TEST("PUSH", "ES",            0006);
    TEST("PUSH", "CS",            0016);
    TEST("PUSH", "SS",            0026);
    TEST("PUSH", "DS",            0036);

    TEST("POP", "BP",            0x8F, 0305);
    TEST("POP", "[SI]",          0x8F, 0004);
    TEST("POP", "[1234H]",       0x8F, 0006, 0x34, 0x12);
    TEST("POP", "[DI-52]",       0x8F, 0105, 0xCC);
    TEST("POP", "[BP+1234H]",    0x8F, 0206, 0x34, 0x12);
    TEST("POP", "[BX+SI]",       0x8F, 0000);
    TEST("POP", "[BX+DI+52]",    0x8F, 0101, 0x34);
    TEST("POP", "[BP+SI+1234H]", 0x8F, 0202, 0x34, 0x12);
    TEST("POP", "BP",            0135);
    TEST("POP", "ES",            0007);
    if (!v30()) {
        ERRT("POP", "CS", REGISTER_NOT_ALLOWED, "CS", 0017);
    }
    TEST("POP", "SS",            0027);
    TEST("POP", "DS",            0037);

    if (is80186()) {
        TEST("PUSH", "34H",   0x6A, 0x34);
        TEST("PUSH", "1234H", 0x68, 0x34, 0x12);
        TEST("PUSHA", "", 0x60);
        TEST("POPA",  "", 0x61);
    }

    TEST("XCHG", "AL, CH",            0x86, 0305);
    TEST("XCHG", "CL, [SI]",          0x86, 0014);
    TEST("XCHG", "DL, [1234H]",       0x86, 0026, 0x34, 0x12);
    TEST("XCHG", "BL, [DI-52]",       0x86, 0135, 0xCC);
    TEST("XCHG", "AH, [BP+1234H]",    0x86, 0246, 0x34, 0x12);
    TEST("XCHG", "CH, [BX+SI]",       0x86, 0050);
    TEST("XCHG", "DH, [BX+DI+52]",    0x86, 0161, 0x34);
    TEST("XCHG", "BH, [BP+SI+1234H]", 0x86, 0272, 0x34, 0x12);
    TEST("XCHG", "BP, AX",            0x87, 0350);
    TEST("XCHG", "CX, [SI]",          0x87, 0014);
    TEST("XCHG", "DX, [1234H]",       0x87, 0026, 0x34, 0x12);
    TEST("XCHG", "BX, [DI-52]",       0x87, 0135, 0xCC);
    TEST("XCHG", "SP, [BP+1234H]",    0x87, 0246, 0x34, 0x12);
    TEST("XCHG", "BP, [BX+SI]",       0x87, 0050);
    TEST("XCHG", "SI, [BX+DI+52]",    0x87, 0161, 0x34);
    TEST("XCHG", "DI, [BP+SI+1234H]", 0x87, 0272, 0x34, 0x12);
    TEST("XCHG", "AX, BP",            0225);

    TEST("IN",   "AL, 34H", 0xE4, 0x34);
    TEST("IN",   "AL, DX",  0xEC);
    TEST("IN",   "AX, 34H", 0xE5, 0x34);
    TEST("IN",   "AX, DX",  0xED);

    TEST("OUT",  "34H, AL", 0xE6, 0x34);
    TEST("OUT",  "DX, AL",  0xEE);
    TEST("OUT",  "34H, AX", 0xE7, 0x34);
    TEST("OUT",  "DX, AX",  0xEF);

    TEST("XLAT", "", 0xD7);

    TEST("LEA",  "CX, [SI]",          0x8D, 0014);
    TEST("LEA",  "DX, [1235H]",       0x8D, 0026, 0x35, 0x12);
    TEST("LEA",  "BX, [DI-52]",       0x8D, 0135, 0xCC);
    TEST("LEA",  "SP, [BP+1234H]",    0x8D, 0246, 0x34, 0x12);
    TEST("LEA",  "BP, [BX+SI]",       0x8D, 0050);
    TEST("LEA",  "SI, [BX+DI+52]",    0x8D, 0161, 0x34);
    TEST("LEA",  "DI, [BP+SI+1234H]", 0x8D, 0272, 0x34, 0x12);

    TEST("LDS",  "CX, [SI]",          0xC5, 0014);
    TEST("LDS",  "DX, [1234H]",       0xC5, 0026, 0x34, 0x12);
    TEST("LDS",  "BX, [DI-52]",       0xC5, 0135, 0xCC);
    TEST("LDS",  "SP, [BP+1234H]",    0xC5, 0246, 0x34, 0x12);
    TEST("LDS",  "BP, [BX+SI]",       0xC5, 0050);
    TEST("LDS",  "SI, [BX+DI+52]",    0xC5, 0161, 0x34);
    TEST("LDS",  "DI, [BP+SI+1234H]", 0xC5, 0272, 0x34, 0x12);

    TEST("LES",  "CX, [SI]",          0xC4, 0014);
    TEST("LES",  "DX, [1234H]",       0xC4, 0026, 0x34, 0x12);
    TEST("LES",  "BX, [DI-52]",       0xC4, 0135, 0xCC);
    TEST("LES",  "SP, [BP+1234H]",    0xC4, 0246, 0x34, 0x12);
    TEST("LES",  "BP, [BX+SI]",       0xC4, 0050);
    TEST("LES",  "SI, [BX+DI+52]",    0xC4, 0161, 0x34);
    TEST("LES",  "DI, [BP+SI+1234H]", 0xC4, 0272, 0x34, 0x12);

    TEST("LAHF",  "", 0x9F);
    TEST("SAHF",  "", 0x9E);
    TEST("PUSHF", "", 0x9C);
    TEST("POPF",  "", 0x9D);
}

static void test_arithmetic() {
    TEST("ADD", "AL, CL",            0x00, 0310);
    TEST("ADD", "DL, BL",            0x00, 0332);
    TEST("ADD", "AH, CH",            0x00, 0354);
    TEST("ADD", "[SI], DH",          0x00, 0064);
    TEST("ADD", "[1234H], BH",       0x00, 0076, 0x34, 0x12);
    TEST("ADD", "[DI+52], AL",       0x00, 0105, 0x34);
    TEST("ADD", "[BP+1234H], CL",    0x00, 0216, 0x34, 0x12);
    TEST("ADD", "[BX+SI], DL",       0x00, 0020);
    TEST("ADD", "[BX+DI+52], BL",    0x00, 0131, 0x34);
    TEST("ADD", "[BP+SI+1234H], AH", 0x00, 0242, 0x34, 0x12);
    TEST("ADD", "DH, [SI]",          0x02, 0064);
    TEST("ADD", "BH, [1234H]",       0x02, 0076, 0x34, 0x12);
    TEST("ADD", "AL, [DI-52]",       0x02, 0105, 0xCC);
    TEST("ADD", "CL, [BP+1234H]",    0x02, 0216, 0x34, 0x12);
    TEST("ADD", "DL, [BX+SI]",       0x02, 0020);
    TEST("ADD", "BL, [BX+DI-52]",    0x02, 0131, 0xCC);
    TEST("ADD", "AH, [BP+SI+1234H]", 0x02, 0242, 0x34, 0x12);
    TEST("ADD", "AX, CX",            0x01, 0310);
    TEST("ADD", "DX, BX",            0x01, 0332);
    TEST("ADD", "SP, BP",            0x01, 0354);
    TEST("ADD", "[SI], SI",          0x01, 0064);
    TEST("ADD", "[1234H], DI",       0x01, 0076, 0x34, 0x12);
    TEST("ADD", "[DI-52], AX",       0x01, 0105, 0xCC);
    TEST("ADD", "[BP+1234H], CX",    0x01, 0216, 0x34, 0x12);
    TEST("ADD", "[BX+SI], DX",       0x01, 0020);
    TEST("ADD", "[BX+DI+52], BX",    0x01, 0131, 0x34);
    TEST("ADD", "[BP+SI+1234H], SP", 0x01, 0242, 0x34, 0x12);
    TEST("ADD", "SI, [SI]",          0x03, 0064);
    TEST("ADD", "DI, [1234H]",       0x03, 0076, 0x34, 0x12);
    TEST("ADD", "AX, [DI+52]",       0x03, 0105, 0x34);
    TEST("ADD", "CX, [BP+1234H]",    0x03, 0216, 0x34, 0x12);
    TEST("ADD", "DX, [BX+SI]",       0x03, 0020);
    TEST("ADD", "BX, [BX+DI+52]",    0x03, 0131, 0x34);
    TEST("ADD", "SP, [BP+SI+1234H]", 0x03, 0242, 0x34, 0x12);

    TEST("ADD", "AL, 56H",                       0x80, 0300, 0x56);
    TEST("ADD", "CL, 56H",                       0x80, 0301, 0x56);
    TEST("ADD", "BYTE PTR [SI], 56H",            0x80, 0004, 0x56);
    TEST("ADD", "BYTE PTR [89ABH], 56H",         0x80, 0006, 0xAB, 0x89, 0x56);
    TEST("ADD", "BYTE PTR [DI-52], 56H",         0x80, 0105, 0xCC, 0x56);
    TEST("ADD", "BYTE PTR [BP+89ABH], 56H",      0x80, 0206, 0xAB, 0x89, 0x56);
    TEST("ADD", "BYTE PTR [BX+SI], 56H",         0x80, 0000, 0x56);
    TEST("ADD", "BYTE PTR [BX+DI+52], 56H",      0x80, 0101, 0x34, 0x56);
    TEST("ADD", "BYTE PTR [BP+SI+1234H], 56H",   0x80, 0202, 0x34, 0x12, 0x56);
    TEST("ADD", "AX, 5678H",                     0x81, 0300, 0x78, 0x56);
    TEST("ADD", "CX, 5678H",                     0x81, 0301, 0x78, 0x56);
    TEST("ADD", "WORD PTR [SI], 5678H",          0x81, 0004, 0x78, 0x56);
    TEST("ADD", "WORD PTR [1234H], 5678H",       0x81, 0006, 0x34, 0x12, 0x78, 0x56);
    TEST("ADD", "WORD PTR [DI+52], 5678H",       0x81, 0105, 0x34, 0x78, 0x56);
    TEST("ADD", "WORD PTR [BP+1234H], 5678H",    0x81, 0206, 0x34, 0x12, 0x78, 0x56);
    TEST("ADD", "WORD PTR [BX+SI], 5678H",       0x81, 0000, 0x78, 0x56);
    TEST("ADD", "WORD PTR [BX+DI-52], 5678H",    0x81, 0101, 0xCC, 0x78, 0x56);
    TEST("ADD", "WORD PTR [BP+SI+89ABH], 5678H", 0x81, 0202, 0xAB, 0x89, 0x78, 0x56);
    TEST("ADD", "AX, 56H",                       0x83, 0300, 0x56);
    TEST("ADD", "AX, -16",                       0x83, 0300, 0xF0);
    TEST("ADD", "CX, 56H",                       0x83, 0301, 0x56);
    TEST("ADD", "CX, -16",                       0x83, 0301, 0xF0);
    TEST("ADD", "WORD PTR [SI], 56H",            0x83, 0004, 0x56);
    TEST("ADD", "WORD PTR [1234H], 56H",         0x83, 0006, 0x34, 0x12, 0x56);
    TEST("ADD", "WORD PTR [DI+52], 56H",         0x83, 0105, 0x34, 0x56);
    TEST("ADD", "WORD PTR [BP+1234H], 56H",      0x83, 0206, 0x34, 0x12, 0x56);
    TEST("ADD", "WORD PTR [BX+SI], 56H",         0x83, 0000, 0x56);
    TEST("ADD", "WORD PTR [BX+DI-52], 56H",      0x83, 0101, 0xCC, 0x56);
    TEST("ADD", "WORD PTR [BP+SI+89ABH], 56H",   0x83, 0202, 0xAB, 0x89, 0x56);

    TEST("ADD", "AL, 56H",   0x04, 0x56);
    TEST("ADD", "AX, 5678H", 0x05, 0x78, 0x56);

    TEST("ADC", "AL, CL",            0x10, 0310);
    TEST("ADC", "DL, BL",            0x10, 0332);
    TEST("ADC", "AH, CH",            0x10, 0354);
    TEST("ADC", "[SI], DH",          0x10, 0064);
    TEST("ADC", "[1234H], BH",       0x10, 0076, 0x34, 0x12);
    TEST("ADC", "[DI+52], AL",       0x10, 0105, 0x34);
    TEST("ADC", "[BP+1234H], CL",    0x10, 0216, 0x34, 0x12);
    TEST("ADC", "[BX+SI], DL",       0x10, 0020);
    TEST("ADC", "[BX+DI+52], BL",    0x10, 0131, 0x34);
    TEST("ADC", "[BP+SI+1234H], AH", 0x10, 0242, 0x34, 0x12);
    TEST("ADC", "DH, [SI]",          0x12, 0064);
    TEST("ADC", "BH, [1234H]",       0x12, 0076, 0x34, 0x12);
    TEST("ADC", "AL, [DI-52]",       0x12, 0105, 0xCC);
    TEST("ADC", "CL, [BP+1234H]",    0x12, 0216, 0x34, 0x12);
    TEST("ADC", "DL, [BX+SI]",       0x12, 0020);
    TEST("ADC", "BL, [BX+DI-52]",    0x12, 0131, 0xCC);
    TEST("ADC", "AH, [BP+SI+1234H]", 0x12, 0242, 0x34, 0x12);
    TEST("ADC", "AX, CX",            0x11, 0310);
    TEST("ADC", "DX, BX",            0x11, 0332);
    TEST("ADC", "SP, BP",            0x11, 0354);
    TEST("ADC", "[SI], SI",          0x11, 0064);
    TEST("ADC", "[1234H], DI",       0x11, 0076, 0x34, 0x12);
    TEST("ADC", "[DI-52], AX",       0x11, 0105, 0xCC);
    TEST("ADC", "[BP+1234H], CX",    0x11, 0216, 0x34, 0x12);
    TEST("ADC", "[BX+SI], DX",       0x11, 0020);
    TEST("ADC", "[BX+DI+52], BX",    0x11, 0131, 0x34);
    TEST("ADC", "[BP+SI+1234H], SP", 0x11, 0242, 0x34, 0x12);
    TEST("ADC", "SI, [SI]",          0x13, 0064);
    TEST("ADC", "DI, [1234H]",       0x13, 0076, 0x34, 0x12);
    TEST("ADC", "AX, [DI+52]",       0x13, 0105, 0x34);
    TEST("ADC", "CX, [BP+1234H]",    0x13, 0216, 0x34, 0x12);
    TEST("ADC", "DX, [BX+SI]",       0x13, 0020);
    TEST("ADC", "BX, [BX+DI+52]",    0x13, 0131, 0x34);
    TEST("ADC", "SP, [BP+SI+1234H]", 0x13, 0242, 0x34, 0x12);

    TEST("ADC", "AL, 56H",                       0x80, 0320, 0x56);
    TEST("ADC", "CL, 56H",                       0x80, 0321, 0x56);
    TEST("ADC", "BYTE PTR [SI], 56H",            0x80, 0024, 0x56);
    TEST("ADC", "BYTE PTR [89ABH], 56H",         0x80, 0026, 0xAB, 0x89, 0x56);
    TEST("ADC", "BYTE PTR [DI-52], 56H",         0x80, 0125, 0xCC, 0x56);
    TEST("ADC", "BYTE PTR [BP+89ABH], 56H",      0x80, 0226, 0xAB, 0x89, 0x56);
    TEST("ADC", "BYTE PTR [BX+SI], 56H",         0x80, 0020, 0x56);
    TEST("ADC", "BYTE PTR [BX+DI+52], 56H",      0x80, 0121, 0x34, 0x56);
    TEST("ADC", "BYTE PTR [BP+SI+1234H], 56H",   0x80, 0222, 0x34, 0x12, 0x56);
    TEST("ADC", "AX, 5678H",                     0x81, 0320, 0x78, 0x56);
    TEST("ADC", "CX, 5678H",                     0x81, 0321, 0x78, 0x56);
    TEST("ADC", "WORD PTR [SI], 5678H",          0x81, 0024, 0x78, 0x56);
    TEST("ADC", "WORD PTR [1234H], 5678H",       0x81, 0026, 0x34, 0x12, 0x78, 0x56);
    TEST("ADC", "WORD PTR [DI+52], 5678H",       0x81, 0125, 0x34, 0x78, 0x56);
    TEST("ADC", "WORD PTR [BP+1234H], 5678H",    0x81, 0226, 0x34, 0x12, 0x78, 0x56);
    TEST("ADC", "WORD PTR [BX+SI], 5678H",       0x81, 0020, 0x78, 0x56);
    TEST("ADC", "WORD PTR [BX+DI-52], 5678H",    0x81, 0121, 0xCC, 0x78, 0x56);
    TEST("ADC", "WORD PTR [BP+SI+89ABH], 5678H", 0x81, 0222, 0xAB, 0x89, 0x78, 0x56);
    TEST("ADC", "AX, 56H",                       0x83, 0320, 0x56);
    TEST("ADC", "AX, -16",                       0x83, 0320, 0xF0);
    TEST("ADC", "CX, 56H",                       0x83, 0321, 0x56);
    TEST("ADC", "CX, -16",                       0x83, 0321, 0xF0);
    TEST("ADC", "WORD PTR [SI], 56H",            0x83, 0024, 0x56);
    TEST("ADC", "WORD PTR [1234H], 56H",         0x83, 0026, 0x34, 0x12, 0x56);
    TEST("ADC", "WORD PTR [DI+52], 56H",         0x83, 0125, 0x34, 0x56);
    TEST("ADC", "WORD PTR [BP+1234H], 56H",      0x83, 0226, 0x34, 0x12, 0x56);
    TEST("ADC", "WORD PTR [BX+SI], 56H",         0x83, 0020, 0x56);
    TEST("ADC", "WORD PTR [BX+DI-52], 56H",      0x83, 0121, 0xCC, 0x56);
    TEST("ADC", "WORD PTR [BP+SI+89ABH], 56H",   0x83, 0222, 0xAB, 0x89, 0x56);

    TEST("ADC", "AL, 56H",   0x14, 0x56);
    TEST("ADC", "AX, 5678H", 0x15, 0x78, 0x56);

    TEST("INC", "CH",                     0xFE, 0305);
    TEST("INC", "BYTE PTR [SI]",          0xFE, 0004);
    TEST("INC", "BYTE PTR [1234H]",       0xFE, 0006, 0x34, 0x12);
    TEST("INC", "BYTE PTR [DI-52]",       0xFE, 0105, 0xCC);
    TEST("INC", "BYTE PTR [BP+1234H]",    0xFE, 0206, 0x34, 0x12);
    TEST("INC", "BYTE PTR [BX+SI]",       0xFE, 0000);
    TEST("INC", "BYTE PTR [BX+DI+52]",    0xFE, 0101, 0x34);
    TEST("INC", "BYTE PTR [BP+SI+1234H]", 0xFE, 0202, 0x34, 0x12);
    TEST("INC", "BP",                     0xFF, 0305);
    TEST("INC", "WORD PTR [SI]",          0xFF, 0004);
    TEST("INC", "WORD PTR [1234H]",       0xFF, 0006, 0x34, 0x12);
    TEST("INC", "WORD PTR [DI-52]",       0xFF, 0105, 0xCC);
    TEST("INC", "WORD PTR [BP+1234H]",    0xFF, 0206, 0x34, 0x12);
    TEST("INC", "WORD PTR [BX+SI]",       0xFF, 0000);
    TEST("INC", "WORD PTR [BX+DI+52]",    0xFF, 0101, 0x34);
    TEST("INC", "WORD PTR [BP+SI+1234H]", 0xFF, 0202, 0x34, 0x12);

    TEST("INC", "BP", 0105);

    TEST("AAA", "", 0x37);
    TEST("DAA", "", 0x27);

    TEST("SUB", "AL, CL",            0x28, 0310);
    TEST("SUB", "DL, BL",            0x28, 0332);
    TEST("SUB", "AH, CH",            0x28, 0354);
    TEST("SUB", "[SI], DH",          0x28, 0064);
    TEST("SUB", "[1234H], BH",       0x28, 0076, 0x34, 0x12);
    TEST("SUB", "[DI+52], AL",       0x28, 0105, 0x34);
    TEST("SUB", "[BP+1234H], CL",    0x28, 0216, 0x34, 0x12);
    TEST("SUB", "[BX+SI], DL",       0x28, 0020);
    TEST("SUB", "[BX+DI+52], BL",    0x28, 0131, 0x34);
    TEST("SUB", "[BP+SI+1234H], AH", 0x28, 0242, 0x34, 0x12);
    TEST("SUB", "DH, [SI]",          0x2A, 0064);
    TEST("SUB", "BH, [1234H]",       0x2A, 0076, 0x34, 0x12);
    TEST("SUB", "AL, [DI-52]",       0x2A, 0105, 0xCC);
    TEST("SUB", "CL, [BP+1234H]",    0x2A, 0216, 0x34, 0x12);
    TEST("SUB", "DL, [BX+SI]",       0x2A, 0020);
    TEST("SUB", "BL, [BX+DI-52]",    0x2A, 0131, 0xCC);
    TEST("SUB", "AH, [BP+SI+1234H]", 0x2A, 0242, 0x34, 0x12);
    TEST("SUB", "AX, CX",            0x29, 0310);
    TEST("SUB", "DX, BX",            0x29, 0332);
    TEST("SUB", "SP, BP",            0x29, 0354);
    TEST("SUB", "[SI], SI",          0x29, 0064);
    TEST("SUB", "[1234H], DI",       0x29, 0076, 0x34, 0x12);
    TEST("SUB", "[DI-52], AX",       0x29, 0105, 0xCC);
    TEST("SUB", "[BP+1234H], CX",    0x29, 0216, 0x34, 0x12);
    TEST("SUB", "[BX+SI], DX",       0x29, 0020);
    TEST("SUB", "[BX+DI+52], BX",    0x29, 0131, 0x34);
    TEST("SUB", "[BP+SI+1234H], SP", 0x29, 0242, 0x34, 0x12);
    TEST("SUB", "SI, [SI]",          0x2B, 0064);
    TEST("SUB", "DI, [1234H]",       0x2B, 0076, 0x34, 0x12);
    TEST("SUB", "AX, [DI+52]",       0x2B, 0105, 0x34);
    TEST("SUB", "CX, [BP+1234H]",    0x2B, 0216, 0x34, 0x12);
    TEST("SUB", "DX, [BX+SI]",       0x2B, 0020);
    TEST("SUB", "BX, [BX+DI+52]",    0x2B, 0131, 0x34);
    TEST("SUB", "SP, [BP+SI+1234H]", 0x2B, 0242, 0x34, 0x12);

    TEST("SUB", "AL, 56H",                       0x80, 0350, 0x56);
    TEST("SUB", "CL, 56H",                       0x80, 0351, 0x56);
    TEST("SUB", "BYTE PTR [SI], 56H",            0x80, 0054, 0x56);
    TEST("SUB", "BYTE PTR [89ABH], 56H",         0x80, 0056, 0xAB, 0x89, 0x56);
    TEST("SUB", "BYTE PTR [DI-52], 56H",         0x80, 0155, 0xCC, 0x56);
    TEST("SUB", "BYTE PTR [BP+89ABH], 56H",      0x80, 0256, 0xAB, 0x89, 0x56);
    TEST("SUB", "BYTE PTR [BX+SI], 56H",         0x80, 0050, 0x56);
    TEST("SUB", "BYTE PTR [BX+DI+52], 56H",      0x80, 0151, 0x34, 0x56);
    TEST("SUB", "BYTE PTR [BP+SI+1234H], 56H",   0x80, 0252, 0x34, 0x12, 0x56);
    TEST("SUB", "AX, 5678H",                     0x81, 0350, 0x78, 0x56);
    TEST("SUB", "CX, 5678H",                     0x81, 0351, 0x78, 0x56);
    TEST("SUB", "WORD PTR [SI], 5678H",          0x81, 0054, 0x78, 0x56);
    TEST("SUB", "WORD PTR [1234H], 5678H",       0x81, 0056, 0x34, 0x12, 0x78, 0x56);
    TEST("SUB", "WORD PTR [DI+52], 5678H",       0x81, 0155, 0x34, 0x78, 0x56);
    TEST("SUB", "WORD PTR [BP+1234H], 5678H",    0x81, 0256, 0x34, 0x12, 0x78, 0x56);
    TEST("SUB", "WORD PTR [BX+SI], 5678H",       0x81, 0050, 0x78, 0x56);
    TEST("SUB", "WORD PTR [BX+DI-52], 5678H",    0x81, 0151, 0xCC, 0x78, 0x56);
    TEST("SUB", "WORD PTR [BP+SI+89ABH], 5678H", 0x81, 0252, 0xAB, 0x89, 0x78, 0x56);
    TEST("SUB", "AX, 56H",                       0x83, 0350, 0x56);
    TEST("SUB", "AX, -16",                       0x83, 0350, 0xF0);
    TEST("SUB", "CX, 56H",                       0x83, 0351, 0x56);
    TEST("SUB", "CX, -16",                       0x83, 0351, 0xF0);
    TEST("SUB", "WORD PTR [SI], 56H",            0x83, 0054, 0x56);
    TEST("SUB", "WORD PTR [1234H], 56H",         0x83, 0056, 0x34, 0x12, 0x56);
    TEST("SUB", "WORD PTR [DI+52], 56H",         0x83, 0155, 0x34, 0x56);
    TEST("SUB", "WORD PTR [BP+1234H], 56H",      0x83, 0256, 0x34, 0x12, 0x56);
    TEST("SUB", "WORD PTR [BX+SI], 56H",         0x83, 0050, 0x56);
    TEST("SUB", "WORD PTR [BX+DI-52], 56H",      0x83, 0151, 0xCC, 0x56);
    TEST("SUB", "WORD PTR [BP+SI+89ABH], 56H",   0x83, 0252, 0xAB, 0x89, 0x56);

    TEST("SUB", "AL, 56H",   0x2C, 0x56);
    TEST("SUB", "AX, 5678H", 0x2D, 0x78, 0x56);

    TEST("SBB", "AL, CL",            0x18, 0310);
    TEST("SBB", "DL, BL",            0x18, 0332);
    TEST("SBB", "AH, CH",            0x18, 0354);
    TEST("SBB", "[SI], DH",          0x18, 0064);
    TEST("SBB", "[1234H], BH",       0x18, 0076, 0x34, 0x12);
    TEST("SBB", "[DI+52], AL",       0x18, 0105, 0x34);
    TEST("SBB", "[BP+1234H], CL",    0x18, 0216, 0x34, 0x12);
    TEST("SBB", "[BX+SI], DL",       0x18, 0020);
    TEST("SBB", "[BX+DI+52], BL",    0x18, 0131, 0x34);
    TEST("SBB", "[BP+SI+1234H], AH", 0x18, 0242, 0x34, 0x12);
    TEST("SBB", "DH, [SI]",          0x1A, 0064);
    TEST("SBB", "BH, [1234H]",       0x1A, 0076, 0x34, 0x12);
    TEST("SBB", "AL, [DI-52]",       0x1A, 0105, 0xCC);
    TEST("SBB", "CL, [BP+1234H]",    0x1A, 0216, 0x34, 0x12);
    TEST("SBB", "DL, [BX+SI]",       0x1A, 0020);
    TEST("SBB", "BL, [BX+DI-52]",    0x1A, 0131, 0xCC);
    TEST("SBB", "AH, [BP+SI+1234H]", 0x1A, 0242, 0x34, 0x12);
    TEST("SBB", "AX, CX",            0x19, 0310);
    TEST("SBB", "DX, BX",            0x19, 0332);
    TEST("SBB", "SP, BP",            0x19, 0354);
    TEST("SBB", "[SI], SI",          0x19, 0064);
    TEST("SBB", "[1234H], DI",       0x19, 0076, 0x34, 0x12);
    TEST("SBB", "[DI-52], AX",       0x19, 0105, 0xCC);
    TEST("SBB", "[BP+1234H], CX",    0x19, 0216, 0x34, 0x12);
    TEST("SBB", "[BX+SI], DX",       0x19, 0020);
    TEST("SBB", "[BX+DI+52], BX",    0x19, 0131, 0x34);
    TEST("SBB", "[BP+SI+1234H], SP", 0x19, 0242, 0x34, 0x12);
    TEST("SBB", "SI, [SI]",          0x1B, 0064);
    TEST("SBB", "DI, [1234H]",       0x1B, 0076, 0x34, 0x12);
    TEST("SBB", "AX, [DI+52]",       0x1B, 0105, 0x34);
    TEST("SBB", "CX, [BP+1234H]",    0x1B, 0216, 0x34, 0x12);
    TEST("SBB", "DX, [BX+SI]",       0x1B, 0020);
    TEST("SBB", "BX, [BX+DI+52]",    0x1B, 0131, 0x34);
    TEST("SBB", "SP, [BP+SI+1234H]", 0x1B, 0242, 0x34, 0x12);

    TEST("SBB", "AL, 56H",                       0x80, 0330, 0x56);
    TEST("SBB", "CL, 56H",                       0x80, 0331, 0x56);
    TEST("SBB", "BYTE PTR [SI], 56H",            0x80, 0034, 0x56);
    TEST("SBB", "BYTE PTR [89ABH], 56H",         0x80, 0036, 0xAB, 0x89, 0x56);
    TEST("SBB", "BYTE PTR [DI-52], 56H",         0x80, 0135, 0xCC, 0x56);
    TEST("SBB", "BYTE PTR [BP+89ABH], 56H",      0x80, 0236, 0xAB, 0x89, 0x56);
    TEST("SBB", "BYTE PTR [BX+SI], 56H",         0x80, 0030, 0x56);
    TEST("SBB", "BYTE PTR [BX+DI+52], 56H",      0x80, 0131, 0x34, 0x56);
    TEST("SBB", "BYTE PTR [BP+SI+1234H], 56H",   0x80, 0232, 0x34, 0x12, 0x56);
    TEST("SBB", "AX, 5678H",                     0x81, 0330, 0x78, 0x56);
    TEST("SBB", "CX, 5678H",                     0x81, 0331, 0x78, 0x56);
    TEST("SBB", "WORD PTR [SI], 5678H",          0x81, 0034, 0x78, 0x56);
    TEST("SBB", "WORD PTR [1234H], 5678H",       0x81, 0036, 0x34, 0x12, 0x78, 0x56);
    TEST("SBB", "WORD PTR [DI+52], 5678H",       0x81, 0135, 0x34, 0x78, 0x56);
    TEST("SBB", "WORD PTR [BP+1234H], 5678H",    0x81, 0236, 0x34, 0x12, 0x78, 0x56);
    TEST("SBB", "WORD PTR [BX+SI], 5678H",       0x81, 0030, 0x78, 0x56);
    TEST("SBB", "WORD PTR [BX+DI-52], 5678H",    0x81, 0131, 0xCC, 0x78, 0x56);
    TEST("SBB", "WORD PTR [BP+SI+89ABH], 5678H", 0x81, 0232, 0xAB, 0x89, 0x78, 0x56);
    TEST("SBB", "AX, 56H",                       0x83, 0330, 0x56);
    TEST("SBB", "AX, -16",                       0x83, 0330, 0xF0);
    TEST("SBB", "CX, 56H",                       0x83, 0331, 0x56);
    TEST("SBB", "CX, -16",                       0x83, 0331, 0xF0);
    TEST("SBB", "WORD PTR [SI], 56H",            0x83, 0034, 0x56);
    TEST("SBB", "WORD PTR [1234H], 56H",         0x83, 0036, 0x34, 0x12, 0x56);
    TEST("SBB", "WORD PTR [DI+52], 56H",         0x83, 0135, 0x34, 0x56);
    TEST("SBB", "WORD PTR [BP+1234H], 56H",      0x83, 0236, 0x34, 0x12, 0x56);
    TEST("SBB", "WORD PTR [BX+SI], 56H",         0x83, 0030, 0x56);
    TEST("SBB", "WORD PTR [BX+DI-52], 56H",      0x83, 0131, 0xCC, 0x56);
    TEST("SBB", "WORD PTR [BP+SI+89ABH], 56H",   0x83, 0232, 0xAB, 0x89, 0x56);

    TEST("SBB", "AL, 56H",   0x1C, 0x56);
    TEST("SBB", "AX, 5678H", 0x1D, 0x78, 0x56);

    TEST("DEC", "CH",                     0xFE, 0315);
    TEST("DEC", "BYTE PTR [SI]",          0xFE, 0014);
    TEST("DEC", "BYTE PTR [1234H]",       0xFE, 0016, 0x34, 0x12);
    TEST("DEC", "BYTE PTR [DI-52]",       0xFE, 0115, 0xCC);
    TEST("DEC", "BYTE PTR [BP+1234H]",    0xFE, 0216, 0x34, 0x12);
    TEST("DEC", "BYTE PTR [BX+SI]",       0xFE, 0010);
    TEST("DEC", "BYTE PTR [BX+DI+52]",    0xFE, 0111, 0x34);
    TEST("DEC", "BYTE PTR [BP+SI+1234H]", 0xFE, 0212, 0x34, 0x12);
    TEST("DEC", "BP",                     0xFF, 0315);
    TEST("DEC", "WORD PTR [SI]",          0xFF, 0014);
    TEST("DEC", "WORD PTR [1234H]",       0xFF, 0016, 0x34, 0x12);
    TEST("DEC", "WORD PTR [DI-52]",       0xFF, 0115, 0xCC);
    TEST("DEC", "WORD PTR [BP+1234H]",    0xFF, 0216, 0x34, 0x12);
    TEST("DEC", "WORD PTR [BX+SI]",       0xFF, 0010);
    TEST("DEC", "WORD PTR [BX+DI+52]",    0xFF, 0111, 0x34);
    TEST("DEC", "WORD PTR [BP+SI+1234H]", 0xFF, 0212, 0x34, 0x12);

    TEST("DEC", "BP", 0115);

    TEST("NEG", "CH",                     0xF6, 0335);
    TEST("NEG", "BYTE PTR [SI]",          0xF6, 0034);
    TEST("NEG", "BYTE PTR [1234H]",       0xF6, 0036, 0x34, 0x12);
    TEST("NEG", "BYTE PTR [DI-52]",       0xF6, 0135, 0xCC);
    TEST("NEG", "BYTE PTR [BP+1234H]",    0xF6, 0236, 0x34, 0x12);
    TEST("NEG", "BYTE PTR [BX+SI]",       0xF6, 0030);
    TEST("NEG", "BYTE PTR [BX+DI+52]",    0xF6, 0131, 0x34);
    TEST("NEG", "BYTE PTR [BP+SI+1234H]", 0xF6, 0232, 0x34, 0x12);
    TEST("NEG", "BP",                     0xF7, 0335);
    TEST("NEG", "WORD PTR [SI]",          0xF7, 0034);
    TEST("NEG", "WORD PTR [1234H]",       0xF7, 0036, 0x34, 0x12);
    TEST("NEG", "WORD PTR [DI-52]",       0xF7, 0135, 0xCC);
    TEST("NEG", "WORD PTR [BP+1234H]",    0xF7, 0236, 0x34, 0x12);
    TEST("NEG", "WORD PTR [BX+SI]",       0xF7, 0030);
    TEST("NEG", "WORD PTR [BX+DI+52]",    0xF7, 0131, 0x34);
    TEST("NEG", "WORD PTR [BP+SI+1234H]", 0xF7, 0232, 0x34, 0x12);

    TEST("CMP", "AL, CL",            0x38, 0310);
    TEST("CMP", "DL, BL",            0x38, 0332);
    TEST("CMP", "AH, CH",            0x38, 0354);
    TEST("CMP", "[SI], DH",          0x38, 0064);
    TEST("CMP", "[1234H], BH",       0x38, 0076, 0x34, 0x12);
    TEST("CMP", "[DI+52], AL",       0x38, 0105, 0x34);
    TEST("CMP", "[BP+1234H], CL",    0x38, 0216, 0x34, 0x12);
    TEST("CMP", "[BX+SI], DL",       0x38, 0020);
    TEST("CMP", "[BX+DI+52], BL",    0x38, 0131, 0x34);
    TEST("CMP", "[BP+SI+1234H], AH", 0x38, 0242, 0x34, 0x12);
    TEST("CMP", "DH, [SI]",          0x3A, 0064);
    TEST("CMP", "BH, [1234H]",       0x3A, 0076, 0x34, 0x12);
    TEST("CMP", "AL, [DI-52]",       0x3A, 0105, 0xCC);
    TEST("CMP", "CL, [BP+1234H]",    0x3A, 0216, 0x34, 0x12);
    TEST("CMP", "DL, [BX+SI]",       0x3A, 0020);
    TEST("CMP", "BL, [BX+DI-52]",    0x3A, 0131, 0xCC);
    TEST("CMP", "AH, [BP+SI+1234H]", 0x3A, 0242, 0x34, 0x12);
    TEST("CMP", "AX, CX",            0x39, 0310);
    TEST("CMP", "DX, BX",            0x39, 0332);
    TEST("CMP", "SP, BP",            0x39, 0354);
    TEST("CMP", "[SI], SI",          0x39, 0064);
    TEST("CMP", "[1234H], DI",       0x39, 0076, 0x34, 0x12);
    TEST("CMP", "[DI-52], AX",       0x39, 0105, 0xCC);
    TEST("CMP", "[BP+1234H], CX",    0x39, 0216, 0x34, 0x12);
    TEST("CMP", "[BX+SI], DX",       0x39, 0020);
    TEST("CMP", "[BX+DI+52], BX",    0x39, 0131, 0x34);
    TEST("CMP", "[BP+SI+1234H], SP", 0x39, 0242, 0x34, 0x12);
    TEST("CMP", "SI, [SI]",          0x3B, 0064);
    TEST("CMP", "DI, [1234H]",       0x3B, 0076, 0x34, 0x12);
    TEST("CMP", "AX, [DI+52]",       0x3B, 0105, 0x34);
    TEST("CMP", "CX, [BP+1234H]",    0x3B, 0216, 0x34, 0x12);
    TEST("CMP", "DX, [BX+SI]",       0x3B, 0020);
    TEST("CMP", "BX, [BX+DI+52]",    0x3B, 0131, 0x34);
    TEST("CMP", "SP, [BP+SI+1234H]", 0x3B, 0242, 0x34, 0x12);

    TEST("CMP", "AL, 56H",                       0x80, 0370, 0x56);
    TEST("CMP", "CL, 56H",                       0x80, 0371, 0x56);
    TEST("CMP", "BYTE PTR [SI], 56H",            0x80, 0074, 0x56);
    TEST("CMP", "BYTE PTR [89ABH], 56H",         0x80, 0076, 0xAB, 0x89, 0x56);
    TEST("CMP", "BYTE PTR [DI-52], 56H",         0x80, 0175, 0xCC, 0x56);
    TEST("CMP", "BYTE PTR [BP+89ABH], 56H",      0x80, 0276, 0xAB, 0x89, 0x56);
    TEST("CMP", "BYTE PTR [BX+SI], 56H",         0x80, 0070, 0x56);
    TEST("CMP", "BYTE PTR [BX+DI+52], 56H",      0x80, 0171, 0x34, 0x56);
    TEST("CMP", "BYTE PTR [BP+SI+1234H], 56H",   0x80, 0272, 0x34, 0x12, 0x56);
    TEST("CMP", "AX, 5678H",                     0x81, 0370, 0x78, 0x56);
    TEST("CMP", "CX, 5678H",                     0x81, 0371, 0x78, 0x56);
    TEST("CMP", "WORD PTR [SI], 5678H",          0x81, 0074, 0x78, 0x56);
    TEST("CMP", "WORD PTR [1234H], 5678H",       0x81, 0076, 0x34, 0x12, 0x78, 0x56);
    TEST("CMP", "WORD PTR [DI+52], 5678H",       0x81, 0175, 0x34, 0x78, 0x56);
    TEST("CMP", "WORD PTR [BP+1234H], 5678H",    0x81, 0276, 0x34, 0x12, 0x78, 0x56);
    TEST("CMP", "WORD PTR [BX+SI], 5678H",       0x81, 0070, 0x78, 0x56);
    TEST("CMP", "WORD PTR [BX+DI-52], 5678H",    0x81, 0171, 0xCC, 0x78, 0x56);
    TEST("CMP", "WORD PTR [BP+SI+89ABH], 5678H", 0x81, 0272, 0xAB, 0x89, 0x78, 0x56);
    TEST("CMP", "AX, 56H",                       0x83, 0370, 0x56);
    TEST("CMP", "AX, -16",                       0x83, 0370, 0xF0);
    TEST("CMP", "CX, 56H",                       0x83, 0371, 0x56);
    TEST("CMP", "CX, -16",                       0x83, 0371, 0xF0);
    TEST("CMP", "WORD PTR [SI], 56H",            0x83, 0074, 0x56);
    TEST("CMP", "WORD PTR [1234H], 56H",         0x83, 0076, 0x34, 0x12, 0x56);
    TEST("CMP", "WORD PTR [DI+52], 56H",         0x83, 0175, 0x34, 0x56);
    TEST("CMP", "WORD PTR [BP+1234H], 56H",      0x83, 0276, 0x34, 0x12, 0x56);
    TEST("CMP", "WORD PTR [BX+SI], 56H",         0x83, 0070, 0x56);
    TEST("CMP", "WORD PTR [BX+DI-52], 56H",      0x83, 0171, 0xCC, 0x56);
    TEST("CMP", "WORD PTR [BP+SI+89ABH], 56H",   0x83, 0272, 0xAB, 0x89, 0x56);

    TEST("CMP", "AL, 56H",   0x3C, 0x56);
    TEST("CMP", "AX, 5678H", 0x3D, 0x78, 0x56);

    TEST("AAS", "", 0x3F);
    TEST("DAS", "", 0x2F);

    TEST("MUL", "CH",                     0xF6, 0345);
    TEST("MUL", "BYTE PTR [SI]",          0xF6, 0044);
    TEST("MUL", "BYTE PTR [1234H]",       0xF6, 0046, 0x34, 0x12);
    TEST("MUL", "BYTE PTR [DI-52]",       0xF6, 0145, 0xCC);
    TEST("MUL", "BYTE PTR [BP+1234H]",    0xF6, 0246, 0x34, 0x12);
    TEST("MUL", "BYTE PTR [BX+SI]",       0xF6, 0040);
    TEST("MUL", "BYTE PTR [BX+DI+52]",    0xF6, 0141, 0x34);
    TEST("MUL", "BYTE PTR [BP+SI+1234H]", 0xF6, 0242, 0x34, 0x12);
    TEST("MUL", "BP",                     0xF7, 0345);
    TEST("MUL", "WORD PTR [SI]",          0xF7, 0044);
    TEST("MUL", "WORD PTR [1234H]",       0xF7, 0046, 0x34, 0x12);
    TEST("MUL", "WORD PTR [DI-52]",       0xF7, 0145, 0xCC);
    TEST("MUL", "WORD PTR [BP+1234H]",    0xF7, 0246, 0x34, 0x12);
    TEST("MUL", "WORD PTR [BX+SI]",       0xF7, 0040);
    TEST("MUL", "WORD PTR [BX+DI+52]",    0xF7, 0141, 0x34);
    TEST("MUL", "WORD PTR [BP+SI+1234H]", 0xF7, 0242, 0x34, 0x12);

    TEST("IMUL", "CH",                     0xF6, 0355);
    TEST("IMUL", "BYTE PTR [SI]",          0xF6, 0054);
    TEST("IMUL", "BYTE PTR [1234H]",       0xF6, 0056, 0x34, 0x12);
    TEST("IMUL", "BYTE PTR [DI-52]",       0xF6, 0155, 0xCC);
    TEST("IMUL", "BYTE PTR [BP+1234H]",    0xF6, 0256, 0x34, 0x12);
    TEST("IMUL", "BYTE PTR [BX+SI]",       0xF6, 0050);
    TEST("IMUL", "BYTE PTR [BX+DI+52]",    0xF6, 0151, 0x34);
    TEST("IMUL", "BYTE PTR [BP+SI+1234H]", 0xF6, 0252, 0x34, 0x12);
    TEST("IMUL", "BP",                     0xF7, 0355);
    TEST("IMUL", "WORD PTR [SI]",          0xF7, 0054);
    TEST("IMUL", "WORD PTR [1234H]",       0xF7, 0056, 0x34, 0x12);
    TEST("IMUL", "WORD PTR [DI-52]",       0xF7, 0155, 0xCC);
    TEST("IMUL", "WORD PTR [BP+1234H]",    0xF7, 0256, 0x34, 0x12);
    TEST("IMUL", "WORD PTR [BX+SI]",       0xF7, 0050);
    TEST("IMUL", "WORD PTR [BX+DI+52]",    0xF7, 0151, 0x34);
    TEST("IMUL", "WORD PTR [BP+SI+1234H]", 0xF7, 0252, 0x34, 0x12);
    if (is80186()) {
        TEST("IMUL", "AX, 1", 0x6B, 0300, 1);
        TEST("IMUL", "CX, 2", 0x6B, 0311, 2);
        TEST("IMUL", "DX, 3", 0x6B, 0322, 3);
        TEST("IMUL", "BX, 4", 0x6B, 0333, 4);
        TEST("IMUL", "SP, 5", 0x6B, 0344, 5);
        TEST("IMUL", "BP, 6", 0x6B, 0355, 6);
        TEST("IMUL", "SI, 7", 0x6B, 0366, 7);
        TEST("IMUL", "DI, 8", 0x6B, 0377, 8);
        TEST("IMUL", "AX, 0123H", 0x69, 0300, 0x23, 0x01);
        TEST("IMUL", "CX, 1234H", 0x69, 0311, 0x34, 0x12);
        TEST("IMUL", "DX, 2345H", 0x69, 0322, 0x45, 0x23);
        TEST("IMUL", "BX, 3456H", 0x69, 0333, 0x56, 0x34);
        TEST("IMUL", "SP, 4567H", 0x69, 0344, 0x67, 0x45);
        TEST("IMUL", "BP, 5678H", 0x69, 0355, 0x78, 0x56);
        TEST("IMUL", "SI, 6789H", 0x69, 0366, 0x89, 0x67);
        TEST("IMUL", "DI, 789AH", 0x69, 0377, 0x9A, 0x78);

        TEST("IMUL", "AX, BP, 1",            0x6B, 0305, 1);
        TEST("IMUL", "CX, [SI], 2",          0x6B, 0014, 2);
        TEST("IMUL", "DX, [1234H], 3",       0x6B, 0026, 0x34, 0x12, 3);
        TEST("IMUL", "BX, [DI-52], 4",       0x6B, 0135, 0xCC, 4);
        TEST("IMUL", "SP, [BP+1234H], 5",    0x6B, 0246, 0x34, 0x12, 5);
        TEST("IMUL", "BP, [BX+SI], 6",       0x6B, 0050, 6);
        TEST("IMUL", "SI, [BX+DI+52], 7",    0x6B, 0161, 0x34, 7);
        TEST("IMUL", "DI, [BP+SI+1234H], 8", 0x6B, 0272, 0x34, 0x12, 8);
        TEST("IMUL", "AX, BP, 0123H",            0x69, 0305, 0x23, 0x01);
        TEST("IMUL", "CX, [SI], 1234H",          0x69, 0014, 0x34, 0x12);
        TEST("IMUL", "DX, [1234H], 2345H",       0x69, 0026, 0x34, 0x12, 0x45, 0x23);
        TEST("IMUL", "BX, [DI-52], 3456H",       0x69, 0135, 0xCC, 0x56, 0x34);
        TEST("IMUL", "SP, [BP+1234H], 4567H",    0x69, 0246, 0x34, 0x12, 0x67, 0x45);
        TEST("IMUL", "BP, [BX+SI], 5678H",       0x69, 0050, 0x78, 0x56);
        TEST("IMUL", "SI, [BX+DI+52], 6789H",    0x69, 0161, 0x34, 0x89, 0x67);
        TEST("IMUL", "DI, [BP+SI+1234H], 789AH", 0x69, 0272, 0x34, 0x12, 0x9A, 0x78);
    }

    TEST("AAM", "", 0xD4, 0x0A);

    TEST("DIV", "CH",                     0xF6, 0365);
    TEST("DIV", "BYTE PTR [SI]",          0xF6, 0064);
    TEST("DIV", "BYTE PTR [1234H]",       0xF6, 0066, 0x34, 0x12);
    TEST("DIV", "BYTE PTR [DI-52]",       0xF6, 0165, 0xCC);
    TEST("DIV", "BYTE PTR [BP+1234H]",    0xF6, 0266, 0x34, 0x12);
    TEST("DIV", "BYTE PTR [BX+SI]",       0xF6, 0060);
    TEST("DIV", "BYTE PTR [BX+DI+52]",    0xF6, 0161, 0x34);
    TEST("DIV", "BYTE PTR [BP+SI+1234H]", 0xF6, 0262, 0x34, 0x12);
    TEST("DIV", "BP",                     0xF7, 0365);
    TEST("DIV", "WORD PTR [SI]",          0xF7, 0064);
    TEST("DIV", "WORD PTR [1234H]",       0xF7, 0066, 0x34, 0x12);
    TEST("DIV", "WORD PTR [DI-52]",       0xF7, 0165, 0xCC);
    TEST("DIV", "WORD PTR [BP+1234H]",    0xF7, 0266, 0x34, 0x12);
    TEST("DIV", "WORD PTR [BX+SI]",       0xF7, 0060);
    TEST("DIV", "WORD PTR [BX+DI+52]",    0xF7, 0161, 0x34);
    TEST("DIV", "WORD PTR [BP+SI+1234H]", 0xF7, 0262, 0x34, 0x12);

    TEST("IDIV", "CH",                     0xF6, 0375);
    TEST("IDIV", "BYTE PTR [SI]",          0xF6, 0074);
    TEST("IDIV", "BYTE PTR [1234H]",       0xF6, 0076, 0x34, 0x12);
    TEST("IDIV", "BYTE PTR [DI-52]",       0xF6, 0175, 0xCC);
    TEST("IDIV", "BYTE PTR [BP+1234H]",    0xF6, 0276, 0x34, 0x12);
    TEST("IDIV", "BYTE PTR [BX+SI]",       0xF6, 0070);
    TEST("IDIV", "BYTE PTR [BX+DI+52]",    0xF6, 0171, 0x34);
    TEST("IDIV", "BYTE PTR [BP+SI+1234H]", 0xF6, 0272, 0x34, 0x12);
    TEST("IDIV", "BP",                     0xF7, 0375);
    TEST("IDIV", "WORD PTR [SI]",          0xF7, 0074);
    TEST("IDIV", "WORD PTR [1234H]",       0xF7, 0076, 0x34, 0x12);
    TEST("IDIV", "WORD PTR [DI-52]",       0xF7, 0175, 0xCC);
    TEST("IDIV", "WORD PTR [BP+1234H]",    0xF7, 0276, 0x34, 0x12);
    TEST("IDIV", "WORD PTR [BX+SI]",       0xF7, 0070);
    TEST("IDIV", "WORD PTR [BX+DI+52]",    0xF7, 0171, 0x34);
    TEST("IDIV", "WORD PTR [BP+SI+1234H]", 0xF7, 0272, 0x34, 0x12);

    TEST("AAD", "", 0xD5, 0x0A);

    TEST("CBW", "", 0x98);
    TEST("CWD", "", 0x99);
}

static void test_logic() {
    TEST("NOT", "CH",                     0xF6, 0325);
    TEST("NOT", "BYTE PTR [SI]",          0xF6, 0024);
    TEST("NOT", "BYTE PTR [1234H]",       0xF6, 0026, 0x34, 0x12);
    TEST("NOT", "BYTE PTR [DI-52]",       0xF6, 0125, 0xCC);
    TEST("NOT", "BYTE PTR [BP+1234H]",    0xF6, 0226, 0x34, 0x12);
    TEST("NOT", "BYTE PTR [BX+SI]",       0xF6, 0020);
    TEST("NOT", "BYTE PTR [BX+DI+52]",    0xF6, 0121, 0x34);
    TEST("NOT", "BYTE PTR [BP+SI+1234H]", 0xF6, 0222, 0x34, 0x12);
    TEST("NOT", "BP",                     0xF7, 0325);
    TEST("NOT", "WORD PTR [SI]",          0xF7, 0024);
    TEST("NOT", "WORD PTR [1234H]",       0xF7, 0026, 0x34, 0x12);
    TEST("NOT", "WORD PTR [DI-52]",       0xF7, 0125, 0xCC);
    TEST("NOT", "WORD PTR [BP+1234H]",    0xF7, 0226, 0x34, 0x12);
    TEST("NOT", "WORD PTR [BX+SI]",       0xF7, 0020);
    TEST("NOT", "WORD PTR [BX+DI+52]",    0xF7, 0121, 0x34);
    TEST("NOT", "WORD PTR [BP+SI+1234H]", 0xF7, 0222, 0x34, 0x12);

    if (is8086()) {
        TEST("SHL", "CH, 1",                      0xD0, 0345);
        TEST("SHL", "BYTE PTR [SI], 1",           0xD0, 0044);
        TEST("SHL", "BYTE PTR [1234H], 1",        0xD0, 0046, 0x34, 0x12);
        TEST("SHL", "BYTE PTR [DI-52], 1",        0xD0, 0145, 0xCC);
        TEST("SHL", "BYTE PTR [BP+1234H], 1",     0xD0, 0246, 0x34, 0x12);
        TEST("SHL", "BYTE PTR [BX+SI], 1",        0xD0, 0040);
        TEST("SHL", "BYTE PTR [BX+DI+52], 1",     0xD0, 0141, 0x34);
        TEST("SHL", "BYTE PTR [BP+SI+1234H], 1",  0xD0, 0242, 0x34, 0x12);
        TEST("SHL", "BP, 1",                      0xD1, 0345);
        TEST("SHL", "WORD PTR [SI], 1",           0xD1, 0044);
        TEST("SHL", "WORD PTR [1234H], 1",        0xD1, 0046, 0x34, 0x12);
        TEST("SHL", "WORD PTR [DI-52], 1",        0xD1, 0145, 0xCC);
        TEST("SHL", "WORD PTR [BP+1234H], 1",     0xD1, 0246, 0x34, 0x12);
        TEST("SHL", "WORD PTR [BX+SI], 1",        0xD1, 0040);
        TEST("SHL", "WORD PTR [BX+DI+52], 1",     0xD1, 0141, 0x34);
        TEST("SHL", "WORD PTR [BP+SI+1234H], 1",  0xD1, 0242, 0x34, 0x12);
    } else {
        TEST("SHL", "CH, 2",                      0xC0, 0345, 2);
        TEST("SHL", "BYTE PTR [SI], 3",           0xC0, 0044, 3);
        TEST("SHL", "BYTE PTR [1234H], 4",        0xC0, 0046, 0x34, 0x12, 4);
        TEST("SHL", "BYTE PTR [DI-52], 5",        0xC0, 0145, 0xCC, 5);
        TEST("SHL", "BYTE PTR [BP+1234H], 6",     0xC0, 0246, 0x34, 0x12, 6);
        TEST("SHL", "BYTE PTR [BX+SI], 7",        0xC0, 0040, 7);
        TEST("SHL", "BYTE PTR [BX+DI+52], 0",     0xC0, 0141, 0x34, 0);
        TEST("SHL", "BYTE PTR [BP+SI+1234H], 1",  0xD0, 0242, 0x34, 0x12);
        TEST("SHL", "BP, 8",                      0xC1, 0345, 8);
        TEST("SHL", "WORD PTR [SI], 1",           0xD1, 0044);
        TEST("SHL", "WORD PTR [1234H], 10",       0xC1, 0046, 0x34, 0x12, 10);
        TEST("SHL", "WORD PTR [DI-52], 11",       0xC1, 0145, 0xCC, 11);
        TEST("SHL", "WORD PTR [BP+1234H], 12",    0xC1, 0246, 0x34, 0x12, 12);
        TEST("SHL", "WORD PTR [BX+SI], 13",       0xC1, 0040, 13);
        TEST("SHL", "WORD PTR [BX+DI+52], 14",    0xC1, 0141, 0x34, 14);
        TEST("SHL", "WORD PTR [BP+SI+1234H], 15", 0xC1, 0242, 0x34, 0x12, 15);
    }
    TEST("SHL", "CH, CL",                     0xD2, 0345);
    TEST("SHL", "BYTE PTR [SI], CL",          0xD2, 0044);
    TEST("SHL", "BYTE PTR [1234H], CL",       0xD2, 0046, 0x34, 0x12);
    TEST("SHL", "BYTE PTR [DI-52], CL",       0xD2, 0145, 0xCC);
    TEST("SHL", "BYTE PTR [BP+1234H], CL",    0xD2, 0246, 0x34, 0x12);
    TEST("SHL", "BYTE PTR [BX+SI], CL",       0xD2, 0040);
    TEST("SHL", "BYTE PTR [BX+DI+52], CL",    0xD2, 0141, 0x34);
    TEST("SHL", "BYTE PTR [BP+SI+1234H], CL", 0xD2, 0242, 0x34, 0x12);
    TEST("SHL", "BP, CL",                     0xD3, 0345);
    TEST("SHL", "WORD PTR [SI], CL",          0xD3, 0044);
    TEST("SHL", "WORD PTR [1234H], CL",       0xD3, 0046, 0x34, 0x12);
    TEST("SHL", "WORD PTR [DI-52], CL",       0xD3, 0145, 0xCC);
    TEST("SHL", "WORD PTR [BP+1234H], CL",    0xD3, 0246, 0x34, 0x12);
    TEST("SHL", "WORD PTR [BX+SI], CL",       0xD3, 0040);
    TEST("SHL", "WORD PTR [BX+DI+52], CL",    0xD3, 0141, 0x34);
    TEST("SHL", "WORD PTR [BP+SI+1234H], CL", 0xD3, 0242, 0x34, 0x12);

    if (is8086()) {
        TEST("SHR", "CH, 1",                      0xD0, 0355);
        TEST("SHR", "BYTE PTR [SI], 1",           0xD0, 0054);
        TEST("SHR", "BYTE PTR [1234H], 1",        0xD0, 0056, 0x34, 0x12);
        TEST("SHR", "BYTE PTR [DI-52], 1",        0xD0, 0155, 0xCC);
        TEST("SHR", "BYTE PTR [BP+1234H], 1",     0xD0, 0256, 0x34, 0x12);
        TEST("SHR", "BYTE PTR [BX+SI], 1",        0xD0, 0050);
        TEST("SHR", "BYTE PTR [BX+DI+52], 1",     0xD0, 0151, 0x34);
        TEST("SHR", "BYTE PTR [BP+SI+1234H], 1",  0xD0, 0252, 0x34, 0x12);
        TEST("SHR", "BP, 1",                      0xD1, 0355);
        TEST("SHR", "WORD PTR [SI], 1",           0xD1, 0054);
        TEST("SHR", "WORD PTR [1234H], 1",        0xD1, 0056, 0x34, 0x12);
        TEST("SHR", "WORD PTR [DI-52], 1",        0xD1, 0155, 0xCC);
        TEST("SHR", "WORD PTR [BP+1234H], 1",     0xD1, 0256, 0x34, 0x12);
        TEST("SHR", "WORD PTR [BX+SI], 1",        0xD1, 0050);
        TEST("SHR", "WORD PTR [BX+DI+52], 1",     0xD1, 0151, 0x34);
        TEST("SHR", "WORD PTR [BP+SI+1234H], 1",  0xD1, 0252, 0x34, 0x12);
    } else {
        TEST("SHR", "CH, 3",                      0xC0, 0355, 3);
        TEST("SHR", "BYTE PTR [SI], 4",           0xC0, 0054, 4);
        TEST("SHR", "BYTE PTR [1234H], 5",        0xC0, 0056, 0x34, 0x12, 5);
        TEST("SHR", "BYTE PTR [DI-52], 6",        0xC0, 0155, 0xCC, 6);
        TEST("SHR", "BYTE PTR [BP+1234H], 7",     0xC0, 0256, 0x34, 0x12, 7);
        TEST("SHR", "BYTE PTR [BX+SI], 0",        0xC0, 0050, 0);
        TEST("SHR", "BYTE PTR [BX+DI+52], 1",     0xD0, 0151, 0x34);
        TEST("SHR", "BYTE PTR [BP+SI+1234H], 2",  0xC0, 0252, 0x34, 0x12, 2);
        TEST("SHR", "BP, 8",                      0xC1, 0355, 8);
        TEST("SHR", "WORD PTR [SI], 9",           0xC1, 0054, 9);
        TEST("SHR", "WORD PTR [1234H], 1",        0xD1, 0056, 0x34, 0x12);
        TEST("SHR", "WORD PTR [DI-52], 11",       0xC1, 0155, 0xCC, 11);
        TEST("SHR", "WORD PTR [BP+1234H], 12",    0xC1, 0256, 0x34, 0x12, 12);
        TEST("SHR", "WORD PTR [BX+SI], 13",       0xC1, 0050, 13);
        TEST("SHR", "WORD PTR [BX+DI+52], 14",    0xC1, 0151, 0x34, 14);
        TEST("SHR", "WORD PTR [BP+SI+1234H], 15", 0xC1, 0252, 0x34, 0x12, 15);
    }
    TEST("SHR", "CH, CL",                     0xD2, 0355);
    TEST("SHR", "BYTE PTR [SI], CL",          0xD2, 0054);
    TEST("SHR", "BYTE PTR [1234H], CL",       0xD2, 0056, 0x34, 0x12);
    TEST("SHR", "BYTE PTR [DI-52], CL",       0xD2, 0155, 0xCC);
    TEST("SHR", "BYTE PTR [BP+1234H], CL",    0xD2, 0256, 0x34, 0x12);
    TEST("SHR", "BYTE PTR [BX+SI], CL",       0xD2, 0050);
    TEST("SHR", "BYTE PTR [BX+DI+52], CL",    0xD2, 0151, 0x34);
    TEST("SHR", "BYTE PTR [BP+SI+1234H], CL", 0xD2, 0252, 0x34, 0x12);
    TEST("SHR", "BP, CL",                     0xD3, 0355);
    TEST("SHR", "WORD PTR [SI], CL",          0xD3, 0054);
    TEST("SHR", "WORD PTR [1234H], CL",       0xD3, 0056, 0x34, 0x12);
    TEST("SHR", "WORD PTR [DI-52], CL",       0xD3, 0155, 0xCC);
    TEST("SHR", "WORD PTR [BP+1234H], CL",    0xD3, 0256, 0x34, 0x12);
    TEST("SHR", "WORD PTR [BX+SI], CL",       0xD3, 0050);
    TEST("SHR", "WORD PTR [BX+DI+52], CL",    0xD3, 0151, 0x34);
    TEST("SHR", "WORD PTR [BP+SI+1234H], CL", 0xD3, 0252, 0x34, 0x12);

    if (is8086()) {
        TEST("SAR", "CH, 1",                      0xD0, 0375);
        TEST("SAR", "BYTE PTR [SI], 1",           0xD0, 0074);
        TEST("SAR", "BYTE PTR [1234H], 1",        0xD0, 0076, 0x34, 0x12);
        TEST("SAR", "BYTE PTR [DI-52], 1",        0xD0, 0175, 0xCC);
        TEST("SAR", "BYTE PTR [BP+1234H], 1",     0xD0, 0276, 0x34, 0x12);
        TEST("SAR", "BYTE PTR [BX+SI], 1",        0xD0, 0070);
        TEST("SAR", "BYTE PTR [BX+DI+52], 1",     0xD0, 0171, 0x34);
        TEST("SAR", "BYTE PTR [BP+SI+1234H], 1",  0xD0, 0272, 0x34, 0x12);
        TEST("SAR", "BP, 1",                      0xD1, 0375);
        TEST("SAR", "WORD PTR [SI], 1",           0xD1, 0074);
        TEST("SAR", "WORD PTR [1234H], 1",        0xD1, 0076, 0x34, 0x12);
        TEST("SAR", "WORD PTR [DI-52], 1",        0xD1, 0175, 0xCC);
        TEST("SAR", "WORD PTR [BP+1234H], 1",     0xD1, 0276, 0x34, 0x12);
        TEST("SAR", "WORD PTR [BX+SI], 1",        0xD1, 0070);
        TEST("SAR", "WORD PTR [BX+DI+52], 1",     0xD1, 0171, 0x34);
        TEST("SAR", "WORD PTR [BP+SI+1234H], 1",  0xD1, 0272, 0x34, 0x12);
    } else {
        TEST("SAR", "CH, 1",                      0xD0, 0375);
        TEST("SAR", "BYTE PTR [SI], 2",           0xC0, 0074, 2);
        TEST("SAR", "BYTE PTR [1234H], 3",        0xC0, 0076, 0x34, 0x12, 3);
        TEST("SAR", "BYTE PTR [DI-52], 4",        0xC0, 0175, 0xCC, 4);
        TEST("SAR", "BYTE PTR [BP+1234H], 5",     0xC0, 0276, 0x34, 0x12, 5);
        TEST("SAR", "BYTE PTR [BX+SI], 6",        0xC0, 0070, 6);
        TEST("SAR", "BYTE PTR [BX+DI+52], 7",     0xC0, 0171, 0x34, 7);
        TEST("SAR", "BYTE PTR [BP+SI+1234H], 0",  0xC0, 0272, 0x34, 0x12, 0);
        TEST("SAR", "BP, 8",                      0xC1, 0375, 8);
        TEST("SAR", "WORD PTR [SI], 9",           0xC1, 0074, 9);
        TEST("SAR", "WORD PTR [1234H], 1",        0xD1, 0076, 0x34, 0x12);
        TEST("SAR", "WORD PTR [DI-52], 11",       0xC1, 0175, 0xCC, 11);
        TEST("SAR", "WORD PTR [BP+1234H], 12",    0xC1, 0276, 0x34, 0x12, 12);
        TEST("SAR", "WORD PTR [BX+SI], 13",       0xC1, 0070, 13);
        TEST("SAR", "WORD PTR [BX+DI+52], 14",    0xC1, 0171, 0x34, 14);
        TEST("SAR", "WORD PTR [BP+SI+1234H], 15", 0xC1, 0272, 0x34, 0x12, 15);
    }
    TEST("SAR", "CH, CL",                     0xD2, 0375);
    TEST("SAR", "BYTE PTR [SI], CL",          0xD2, 0074);
    TEST("SAR", "BYTE PTR [1234H], CL",       0xD2, 0076, 0x34, 0x12);
    TEST("SAR", "BYTE PTR [DI-52], CL",       0xD2, 0175, 0xCC);
    TEST("SAR", "BYTE PTR [BP+1234H], CL",    0xD2, 0276, 0x34, 0x12);
    TEST("SAR", "BYTE PTR [BX+SI], CL",       0xD2, 0070);
    TEST("SAR", "BYTE PTR [BX+DI+52], CL",    0xD2, 0171, 0x34);
    TEST("SAR", "BYTE PTR [BP+SI+1234H], CL", 0xD2, 0272, 0x34, 0x12);
    TEST("SAR", "BP, CL",                     0xD3, 0375);
    TEST("SAR", "WORD PTR [SI], CL",          0xD3, 0074);
    TEST("SAR", "WORD PTR [1234H], CL",       0xD3, 0076, 0x34, 0x12);
    TEST("SAR", "WORD PTR [DI-52], CL",       0xD3, 0175, 0xCC);
    TEST("SAR", "WORD PTR [BP+1234H], CL",    0xD3, 0276, 0x34, 0x12);
    TEST("SAR", "WORD PTR [BX+SI], CL",       0xD3, 0070);
    TEST("SAR", "WORD PTR [BX+DI+52], CL",    0xD3, 0171, 0x34);
    TEST("SAR", "WORD PTR [BP+SI+1234H], CL", 0xD3, 0272, 0x34, 0x12);

    if (is8086()) {
        TEST("ROL", "CH, 1",                      0xD0, 0305);
        TEST("ROL", "BYTE PTR [SI], 1",           0xD0, 0004);
        TEST("ROL", "BYTE PTR [1234H], 1",        0xD0, 0006, 0x34, 0x12);
        TEST("ROL", "BYTE PTR [DI-52], 1",        0xD0, 0105, 0xCC);
        TEST("ROL", "BYTE PTR [BP+1234H], 1",     0xD0, 0206, 0x34, 0x12);
        TEST("ROL", "BYTE PTR [BX+SI], 1",        0xD0, 0000);
        TEST("ROL", "BYTE PTR [BX+DI+52], 1",     0xD0, 0101, 0x34);
        TEST("ROL", "BYTE PTR [BP+SI+1234H], 1",  0xD0, 0202, 0x34, 0x12);
        TEST("ROL", "BP, 1",                      0xD1, 0305);
        TEST("ROL", "WORD PTR [SI], 1",           0xD1, 0004);
        TEST("ROL", "WORD PTR [1234H], 1",        0xD1, 0006, 0x34, 0x12);
        TEST("ROL", "WORD PTR [DI-52], 1",        0xD1, 0105, 0xCC);
        TEST("ROL", "WORD PTR [BP+1234H], 1",     0xD1, 0206, 0x34, 0x12);
        TEST("ROL", "WORD PTR [BX+SI], 1",        0xD1, 0000);
        TEST("ROL", "WORD PTR [BX+DI+52], 1",     0xD1, 0101, 0x34);
        TEST("ROL", "WORD PTR [BP+SI+1234H], 1",  0xD1, 0202, 0x34, 0x12);
    } else {
        TEST("ROL", "CH, 4",                      0xC0, 0305, 4);
        TEST("ROL", "BYTE PTR [SI], 5",           0xC0, 0004, 5);
        TEST("ROL", "BYTE PTR [1234H], 6",        0xC0, 0006, 0x34, 0x12, 6);
        TEST("ROL", "BYTE PTR [DI-52], 7",        0xC0, 0105, 0xCC, 7);
        TEST("ROL", "BYTE PTR [BP+1234H], 0",     0xC0, 0206, 0x34, 0x12, 0);
        TEST("ROL", "BYTE PTR [BX+SI], 1",        0xD0, 0000);
        TEST("ROL", "BYTE PTR [BX+DI+52], 2",     0xC0, 0101, 0x34, 2);
        TEST("ROL", "BYTE PTR [BP+SI+1234H], 3",  0xC0, 0202, 0x34, 0x12, 3);
        TEST("ROL", "BP, 8",                      0xC1, 0305, 8);
        TEST("ROL", "WORD PTR [SI], 9",           0xC1, 0004, 9);
        TEST("ROL", "WORD PTR [1234H], 10",       0xC1, 0006, 0x34, 0x12, 10);
        TEST("ROL", "WORD PTR [DI-52], 1",        0xD1, 0105, 0xCC);
        TEST("ROL", "WORD PTR [BP+1234H], 12",    0xC1, 0206, 0x34, 0x12, 12);
        TEST("ROL", "WORD PTR [BX+SI], 13",       0xC1, 0000, 13);
        TEST("ROL", "WORD PTR [BX+DI+52], 14",    0xC1, 0101, 0x34, 14);
        TEST("ROL", "WORD PTR [BP+SI+1234H], 15", 0xC1, 0202, 0x34, 0x12, 15);
    }
    TEST("ROL", "CH, CL",                     0xD2, 0305);
    TEST("ROL", "BYTE PTR [SI], CL",          0xD2, 0004);
    TEST("ROL", "BYTE PTR [1234H], CL",       0xD2, 0006, 0x34, 0x12);
    TEST("ROL", "BYTE PTR [DI-52], CL",       0xD2, 0105, 0xCC);
    TEST("ROL", "BYTE PTR [BP+1234H], CL",    0xD2, 0206, 0x34, 0x12);
    TEST("ROL", "BYTE PTR [BX+SI], CL",       0xD2, 0000);
    TEST("ROL", "BYTE PTR [BX+DI+52], CL",    0xD2, 0101, 0x34);
    TEST("ROL", "BYTE PTR [BP+SI+1234H], CL", 0xD2, 0202, 0x34, 0x12);
    TEST("ROL", "BP, CL",                     0xD3, 0305);
    TEST("ROL", "WORD PTR [SI], CL",          0xD3, 0004);
    TEST("ROL", "WORD PTR [1234H], CL",       0xD3, 0006, 0x34, 0x12);
    TEST("ROL", "WORD PTR [DI-52], CL",       0xD3, 0105, 0xCC);
    TEST("ROL", "WORD PTR [BP+1234H], CL",    0xD3, 0206, 0x34, 0x12);
    TEST("ROL", "WORD PTR [BX+SI], CL",       0xD3, 0000);
    TEST("ROL", "WORD PTR [BX+DI+52], CL",    0xD3, 0101, 0x34);
    TEST("ROL", "WORD PTR [BP+SI+1234H], CL", 0xD3, 0202, 0x34, 0x12);

    if (is8086()) {
        TEST("ROR", "CH, 1",                      0xD0, 0315);
        TEST("ROR", "BYTE PTR [SI], 1",           0xD0, 0014);
        TEST("ROR", "BYTE PTR [1234H], 1",        0xD0, 0016, 0x34, 0x12);
        TEST("ROR", "BYTE PTR [DI-52], 1",        0xD0, 0115, 0xCC);
        TEST("ROR", "BYTE PTR [BP+1234H], 1",     0xD0, 0216, 0x34, 0x12);
        TEST("ROR", "BYTE PTR [BX+SI], 1",        0xD0, 0010);
        TEST("ROR", "BYTE PTR [BX+DI+52], 1",     0xD0, 0111, 0x34);
        TEST("ROR", "BYTE PTR [BP+SI+1234H], 1",  0xD0, 0212, 0x34, 0x12);
        TEST("ROR", "BP, 1",                      0xD1, 0315);
        TEST("ROR", "WORD PTR [SI], 1",           0xD1, 0014);
        TEST("ROR", "WORD PTR [1234H], 1",        0xD1, 0016, 0x34, 0x12);
        TEST("ROR", "WORD PTR [DI-52], 1",        0xD1, 0115, 0xCC);
        TEST("ROR", "WORD PTR [BP+1234H], 1",     0xD1, 0216, 0x34, 0x12);
        TEST("ROR", "WORD PTR [BX+SI], 1",        0xD1, 0010);
        TEST("ROR", "WORD PTR [BX+DI+52], 1",     0xD1, 0111, 0x34);
        TEST("ROR", "WORD PTR [BP+SI+1234H], 1",  0xD1, 0212, 0x34, 0x12);
    } else {
        TEST("ROR", "CH, 5",                      0xC0, 0315, 5);
        TEST("ROR", "BYTE PTR [SI], 6",           0xC0, 0014, 6);
        TEST("ROR", "BYTE PTR [1234H], 7",        0xC0, 0016, 0x34, 0x12, 7);
        TEST("ROR", "BYTE PTR [DI-52], 0",        0xC0, 0115, 0xCC, 0);
        TEST("ROR", "BYTE PTR [BP+1234H], 1",     0xD0, 0216, 0x34, 0x12);
        TEST("ROR", "BYTE PTR [BX+SI], 2",        0xC0, 0010, 2);
        TEST("ROR", "BYTE PTR [BX+DI+52], 3",     0xC0, 0111, 0x34, 3);
        TEST("ROR", "BYTE PTR [BP+SI+1234H], 4",  0xC0, 0212, 0x34, 0x12, 4);
        TEST("ROR", "BP, 8",                      0xC1, 0315, 8);
        TEST("ROR", "WORD PTR [SI], 9",           0xC1, 0014, 9);
        TEST("ROR", "WORD PTR [1234H], 10",       0xC1, 0016, 0x34, 0x12, 10);
        TEST("ROR", "WORD PTR [DI-52], 11",       0xC1, 0115, 0xCC, 11);
        TEST("ROR", "WORD PTR [BP+1234H], 1",     0xD1, 0216, 0x34, 0x12);
        TEST("ROR", "WORD PTR [BX+SI], 13",       0xC1, 0010, 13);
        TEST("ROR", "WORD PTR [BX+DI+52], 14",    0xC1, 0111, 0x34, 14);
        TEST("ROR", "WORD PTR [BP+SI+1234H], 15", 0xC1, 0212, 0x34, 0x12, 15);
    }
    TEST("ROR", "CH, CL",                     0xD2, 0315);
    TEST("ROR", "BYTE PTR [SI], CL",          0xD2, 0014);
    TEST("ROR", "BYTE PTR [1234H], CL",       0xD2, 0016, 0x34, 0x12);
    TEST("ROR", "BYTE PTR [DI-52], CL",       0xD2, 0115, 0xCC);
    TEST("ROR", "BYTE PTR [BP+1234H], CL",    0xD2, 0216, 0x34, 0x12);
    TEST("ROR", "BYTE PTR [BX+SI], CL",       0xD2, 0010);
    TEST("ROR", "BYTE PTR [BX+DI+52], CL",    0xD2, 0111, 0x34);
    TEST("ROR", "BYTE PTR [BP+SI+1234H], CL", 0xD2, 0212, 0x34, 0x12);
    TEST("ROR", "BP, CL",                     0xD3, 0315);
    TEST("ROR", "WORD PTR [SI], CL",          0xD3, 0014);
    TEST("ROR", "WORD PTR [1234H], CL",       0xD3, 0016, 0x34, 0x12);
    TEST("ROR", "WORD PTR [DI-52], CL",       0xD3, 0115, 0xCC);
    TEST("ROR", "WORD PTR [BP+1234H], CL",    0xD3, 0216, 0x34, 0x12);
    TEST("ROR", "WORD PTR [BX+SI], CL",       0xD3, 0010);
    TEST("ROR", "WORD PTR [BX+DI+52], CL",    0xD3, 0111, 0x34);
    TEST("ROR", "WORD PTR [BP+SI+1234H], CL", 0xD3, 0212, 0x34, 0x12);

    if (is8086()) {
        TEST("RCL", "CH, 1",                      0xD0, 0325);
        TEST("RCL", "BYTE PTR [SI], 1",           0xD0, 0024);
        TEST("RCL", "BYTE PTR [1234H], 1",        0xD0, 0026, 0x34, 0x12);
        TEST("RCL", "BYTE PTR [DI-52], 1",        0xD0, 0125, 0xCC);
        TEST("RCL", "BYTE PTR [BP+1234H], 1",     0xD0, 0226, 0x34, 0x12);
        TEST("RCL", "BYTE PTR [BX+SI], 1",        0xD0, 0020);
        TEST("RCL", "BYTE PTR [BX+DI+52], 1",     0xD0, 0121, 0x34);
        TEST("RCL", "BYTE PTR [BP+SI+1234H], 1",  0xD0, 0222, 0x34, 0x12);
        TEST("RCL", "BP, 1",                      0xD1, 0325);
        TEST("RCL", "WORD PTR [SI], 1",           0xD1, 0024);
        TEST("RCL", "WORD PTR [1234H], 1",        0xD1, 0026, 0x34, 0x12);
        TEST("RCL", "WORD PTR [DI-52], 1",        0xD1, 0125, 0xCC);
        TEST("RCL", "WORD PTR [BP+1234H], 1",     0xD1, 0226, 0x34, 0x12);
        TEST("RCL", "WORD PTR [BX+SI], 1",        0xD1, 0020);
        TEST("RCL", "WORD PTR [BX+DI+52], 1",     0xD1, 0121, 0x34);
        TEST("RCL", "WORD PTR [BP+SI+1234H], 1",  0xD1, 0222, 0x34, 0x12);
    } else {
        TEST("RCL", "CH, 6",                      0xC0, 0325, 6);
        TEST("RCL", "BYTE PTR [SI], 7",           0xC0, 0024, 7);
        TEST("RCL", "BYTE PTR [1234H], 0",        0xC0, 0026, 0x34, 0x12, 0);
        TEST("RCL", "BYTE PTR [DI-52], 1",        0xD0, 0125, 0xCC);
        TEST("RCL", "BYTE PTR [BP+1234H], 2",     0xC0, 0226, 0x34, 0x12, 2);
        TEST("RCL", "BYTE PTR [BX+SI], 3",        0xC0, 0020, 3);
        TEST("RCL", "BYTE PTR [BX+DI+52], 4",     0xC0, 0121, 0x34, 4);
        TEST("RCL", "BYTE PTR [BP+SI+1234H], 5",  0xC0, 0222, 0x34, 0x12, 5);
        TEST("RCL", "BP, 8",                      0xC1, 0325, 8);
        TEST("RCL", "WORD PTR [SI], 9",           0xC1, 0024, 9);
        TEST("RCL", "WORD PTR [1234H], 10",       0xC1, 0026, 0x34, 0x12, 10);
        TEST("RCL", "WORD PTR [DI-52], 11",       0xC1, 0125, 0xCC, 11);
        TEST("RCL", "WORD PTR [BP+1234H], 12",    0xC1, 0226, 0x34, 0x12, 12);
        TEST("RCL", "WORD PTR [BX+SI], 1",        0xD1, 0020);
        TEST("RCL", "WORD PTR [BX+DI+52], 14",    0xC1, 0121, 0x34, 14);
        TEST("RCL", "WORD PTR [BP+SI+1234H], 15", 0xC1, 0222, 0x34, 0x12, 15);
    }
    TEST("RCL", "CH, CL",                     0xD2, 0325);
    TEST("RCL", "BYTE PTR [SI], CL",          0xD2, 0024);
    TEST("RCL", "BYTE PTR [1234H], CL",       0xD2, 0026, 0x34, 0x12);
    TEST("RCL", "BYTE PTR [DI-52], CL",       0xD2, 0125, 0xCC);
    TEST("RCL", "BYTE PTR [BP+1234H], CL",    0xD2, 0226, 0x34, 0x12);
    TEST("RCL", "BYTE PTR [BX+SI], CL",       0xD2, 0020);
    TEST("RCL", "BYTE PTR [BX+DI+52], CL",    0xD2, 0121, 0x34);
    TEST("RCL", "BYTE PTR [BP+SI+1234H], CL", 0xD2, 0222, 0x34, 0x12);
    TEST("RCL", "BP, CL",                     0xD3, 0325);
    TEST("RCL", "WORD PTR [SI], CL",          0xD3, 0024);
    TEST("RCL", "WORD PTR [1234H], CL",       0xD3, 0026, 0x34, 0x12);
    TEST("RCL", "WORD PTR [DI-52], CL",       0xD3, 0125, 0xCC);
    TEST("RCL", "WORD PTR [BP+1234H], CL",    0xD3, 0226, 0x34, 0x12);
    TEST("RCL", "WORD PTR [BX+SI], CL",       0xD3, 0020);
    TEST("RCL", "WORD PTR [BX+DI+52], CL",    0xD3, 0121, 0x34);
    TEST("RCL", "WORD PTR [BP+SI+1234H], CL", 0xD3, 0222, 0x34, 0x12);

    if (is8086()) {
        TEST("RCR", "CH, 1",                      0xD0, 0335);
        TEST("RCR", "BYTE PTR [SI], 1",           0xD0, 0034);
        TEST("RCR", "BYTE PTR [1234H], 1",        0xD0, 0036, 0x34, 0x12);
        TEST("RCR", "BYTE PTR [DI-52], 1",        0xD0, 0135, 0xCC);
        TEST("RCR", "BYTE PTR [BP+1234H], 1",     0xD0, 0236, 0x34, 0x12);
        TEST("RCR", "BYTE PTR [BX+SI], 1",        0xD0, 0030);
        TEST("RCR", "BYTE PTR [BX+DI+52], 1",     0xD0, 0131, 0x34);
        TEST("RCR", "BYTE PTR [BP+SI+1234H], 1",  0xD0, 0232, 0x34, 0x12);
        TEST("RCR", "BP, 1",                      0xD1, 0335);
        TEST("RCR", "WORD PTR [SI], 1",           0xD1, 0034);
        TEST("RCR", "WORD PTR [1234H], 1",        0xD1, 0036, 0x34, 0x12);
        TEST("RCR", "WORD PTR [DI-52], 1",        0xD1, 0135, 0xCC);
        TEST("RCR", "WORD PTR [BP+1234H], 1",     0xD1, 0236, 0x34, 0x12);
        TEST("RCR", "WORD PTR [BX+SI], 1",        0xD1, 0030);
        TEST("RCR", "WORD PTR [BX+DI+52], 1",     0xD1, 0131, 0x34);
        TEST("RCR", "WORD PTR [BP+SI+1234H], 1",  0xD1, 0232, 0x34, 0x12);
    } else {
        TEST("RCR", "CH, 7",                      0xC0, 0335, 7);
        TEST("RCR", "BYTE PTR [SI], 0",           0xC0, 0034, 0);
        TEST("RCR", "BYTE PTR [1234H], 1",        0xD0, 0036, 0x34, 0x12);
        TEST("RCR", "BYTE PTR [DI-52], 2",        0xC0, 0135, 0xCC, 2);
        TEST("RCR", "BYTE PTR [BP+1234H], 3",     0xC0, 0236, 0x34, 0x12, 3);
        TEST("RCR", "BYTE PTR [BX+SI], 4",        0xC0, 0030, 4);
        TEST("RCR", "BYTE PTR [BX+DI+52], 5",     0xC0, 0131, 0x34, 5);
        TEST("RCR", "BYTE PTR [BP+SI+1234H], 6",  0xC0, 0232, 0x34, 0x12, 6);
        TEST("RCR", "BP, 8",                      0xC1, 0335, 8);
        TEST("RCR", "WORD PTR [SI], 9",           0xC1, 0034, 9);
        TEST("RCR", "WORD PTR [1234H], 10",       0xC1, 0036, 0x34, 0x12, 10);
        TEST("RCR", "WORD PTR [DI-52], 11",       0xC1, 0135, 0xCC, 11);
        TEST("RCR", "WORD PTR [BP+1234H], 12",    0xC1, 0236, 0x34, 0x12, 12);
        TEST("RCR", "WORD PTR [BX+SI], 13",       0xC1, 0030, 13);
        TEST("RCR", "WORD PTR [BX+DI+52], 1",     0xD1, 0131, 0x34);
        TEST("RCR", "WORD PTR [BP+SI+1234H], 15", 0xC1, 0232, 0x34, 0x12, 15);
    }
    TEST("RCR", "CH, CL",                     0xD2, 0335);
    TEST("RCR", "BYTE PTR [SI], CL",          0xD2, 0034);
    TEST("RCR", "BYTE PTR [1234H], CL",       0xD2, 0036, 0x34, 0x12);
    TEST("RCR", "BYTE PTR [DI-52], CL",       0xD2, 0135, 0xCC);
    TEST("RCR", "BYTE PTR [BP+1234H], CL",    0xD2, 0236, 0x34, 0x12);
    TEST("RCR", "BYTE PTR [BX+SI], CL",       0xD2, 0030);
    TEST("RCR", "BYTE PTR [BX+DI+52], CL",    0xD2, 0131, 0x34);
    TEST("RCR", "BYTE PTR [BP+SI+1234H], CL", 0xD2, 0232, 0x34, 0x12);
    TEST("RCR", "BP, CL",                     0xD3, 0335);
    TEST("RCR", "WORD PTR [SI], CL",          0xD3, 0034);
    TEST("RCR", "WORD PTR [1234H], CL",       0xD3, 0036, 0x34, 0x12);
    TEST("RCR", "WORD PTR [DI-52], CL",       0xD3, 0135, 0xCC);
    TEST("RCR", "WORD PTR [BP+1234H], CL",    0xD3, 0236, 0x34, 0x12);
    TEST("RCR", "WORD PTR [BX+SI], CL",       0xD3, 0030);
    TEST("RCR", "WORD PTR [BX+DI+52], CL",    0xD3, 0131, 0x34);
    TEST("RCR", "WORD PTR [BP+SI+1234H], CL", 0xD3, 0232, 0x34, 0x12);

    TEST("AND", "AL, CL",            0x20, 0310);
    TEST("AND", "DL, BL",            0x20, 0332);
    TEST("AND", "AH, CH",            0x20, 0354);
    TEST("AND", "[SI], DH",          0x20, 0064);
    TEST("AND", "[1234H], BH",       0x20, 0076, 0x34, 0x12);
    TEST("AND", "[DI+52], AL",       0x20, 0105, 0x34);
    TEST("AND", "[BP+1234H], CL",    0x20, 0216, 0x34, 0x12);
    TEST("AND", "[BX+SI], DL",       0x20, 0020);
    TEST("AND", "[BX+DI+52], BL",    0x20, 0131, 0x34);
    TEST("AND", "[BP+SI+1234H], AH", 0x20, 0242, 0x34, 0x12);
    TEST("AND", "DH, [SI]",          0x22, 0064);
    TEST("AND", "BH, [1234H]",       0x22, 0076, 0x34, 0x12);
    TEST("AND", "AL, [DI-52]",       0x22, 0105, 0xCC);
    TEST("AND", "CL, [BP+1234H]",    0x22, 0216, 0x34, 0x12);
    TEST("AND", "DL, [BX+SI]",       0x22, 0020);
    TEST("AND", "BL, [BX+DI-52]",    0x22, 0131, 0xCC);
    TEST("AND", "AH, [BP+SI+1234H]", 0x22, 0242, 0x34, 0x12);
    TEST("AND", "AX, CX",            0x21, 0310);
    TEST("AND", "DX, BX",            0x21, 0332);
    TEST("AND", "SP, BP",            0x21, 0354);
    TEST("AND", "[SI], SI",          0x21, 0064);
    TEST("AND", "[1234H], DI",       0x21, 0076, 0x34, 0x12);
    TEST("AND", "[DI-52], AX",       0x21, 0105, 0xCC);
    TEST("AND", "[BP+1234H], CX",    0x21, 0216, 0x34, 0x12);
    TEST("AND", "[BX+SI], DX",       0x21, 0020);
    TEST("AND", "[BX+DI+52], BX",    0x21, 0131, 0x34);
    TEST("AND", "[BP+SI+1234H], SP", 0x21, 0242, 0x34, 0x12);
    TEST("AND", "SI, [SI]",          0x23, 0064);
    TEST("AND", "DI, [1234H]",       0x23, 0076, 0x34, 0x12);
    TEST("AND", "AX, [DI+52]",       0x23, 0105, 0x34);
    TEST("AND", "CX, [BP+1234H]",    0x23, 0216, 0x34, 0x12);
    TEST("AND", "DX, [BX+SI]",       0x23, 0020);
    TEST("AND", "BX, [BX+DI+52]",    0x23, 0131, 0x34);
    TEST("AND", "SP, [BP+SI+1234H]", 0x23, 0242, 0x34, 0x12);

    TEST("AND", "AL, 56H",                       0x80, 0340, 0x56);
    TEST("AND", "CL, 56H",                       0x80, 0341, 0x56);
    TEST("AND", "BYTE PTR [SI], 56H",            0x80, 0044, 0x56);
    TEST("AND", "BYTE PTR [89ABH], 56H",         0x80, 0046, 0xAB, 0x89, 0x56);
    TEST("AND", "BYTE PTR [DI-52], 56H",         0x80, 0145, 0xCC, 0x56);
    TEST("AND", "BYTE PTR [BP+89ABH], 56H",      0x80, 0246, 0xAB, 0x89, 0x56);
    TEST("AND", "BYTE PTR [BX+SI], 56H",         0x80, 0040, 0x56);
    TEST("AND", "BYTE PTR [BX+DI+52], 56H",      0x80, 0141, 0x34, 0x56);
    TEST("AND", "BYTE PTR [BP+SI+1234H], 56H",   0x80, 0242, 0x34, 0x12, 0x56);
    TEST("AND", "AX, 5678H",                     0x81, 0340, 0x78, 0x56);
    TEST("AND", "CX, 5678H",                     0x81, 0341, 0x78, 0x56);
    TEST("AND", "WORD PTR [SI], 5678H",          0x81, 0044, 0x78, 0x56);
    TEST("AND", "WORD PTR [1234H], 5678H",       0x81, 0046, 0x34, 0x12, 0x78, 0x56);
    TEST("AND", "WORD PTR [DI+52], 5678H",       0x81, 0145, 0x34, 0x78, 0x56);
    TEST("AND", "WORD PTR [BP+1234H], 5678H",    0x81, 0246, 0x34, 0x12, 0x78, 0x56);
    TEST("AND", "WORD PTR [BX+SI], 5678H",       0x81, 0040, 0x78, 0x56);
    TEST("AND", "WORD PTR [BX+DI-52], 5678H",    0x81, 0141, 0xCC, 0x78, 0x56);
    TEST("AND", "WORD PTR [BP+SI+89ABH], 5678H", 0x81, 0242, 0xAB, 0x89, 0x78, 0x56);
    TEST("AND", "AX, 56H",                       0x83, 0340, 0x56);
    TEST("AND", "AX, -16",                       0x83, 0340, 0xF0);
    TEST("AND", "CX, 56H",                       0x83, 0341, 0x56);
    TEST("AND", "CX, -16",                       0x83, 0341, 0xF0);
    TEST("AND", "WORD PTR [SI], 56H",            0x83, 0044, 0x56);
    TEST("AND", "WORD PTR [1234H], 56H",         0x83, 0046, 0x34, 0x12, 0x56);
    TEST("AND", "WORD PTR [DI+52], 56H",         0x83, 0145, 0x34, 0x56);
    TEST("AND", "WORD PTR [BP+1234H], 56H",      0x83, 0246, 0x34, 0x12, 0x56);
    TEST("AND", "WORD PTR [BX+SI], 56H",         0x83, 0040, 0x56);
    TEST("AND", "WORD PTR [BX+DI-52], 56H",      0x83, 0141, 0xCC, 0x56);
    TEST("AND", "WORD PTR [BP+SI+89ABH], 56H",   0x83, 0242, 0xAB, 0x89, 0x56);

    TEST("AND", "AL, 56H",   0x24, 0x56);
    TEST("AND", "AX, 5678H", 0x25, 0x78, 0x56);

    TEST("TEST", "AL, CL",            0x84, 0301);
    TEST("TEST", "DL, BL",            0x84, 0323);
    TEST("TEST", "AH, CH",            0x84, 0345);
    TEST("TEST", "DH, [SI]",          0x84, 0064);
    TEST("TEST", "BH, [1234H]",       0x84, 0076, 0x34, 0x12);
    TEST("TEST", "AL, [DI-52]",       0x84, 0105, 0xCC);
    TEST("TEST", "CL, [BP+1234H]",    0x84, 0216, 0x34, 0x12);
    TEST("TEST", "DL, [BX+SI]",       0x84, 0020);
    TEST("TEST", "BL, [BX+DI-52]",    0x84, 0131, 0xCC);
    TEST("TEST", "AH, [BP+SI+1234H]", 0x84, 0242, 0x34, 0x12);
    TEST("TEST", "AX, CX",            0x85, 0301);
    TEST("TEST", "DX, BX",            0x85, 0323);
    TEST("TEST", "SP, BP",            0x85, 0345);
    TEST("TEST", "SI, [SI]",          0x85, 0064);
    TEST("TEST", "DI, [1234H]",       0x85, 0076, 0x34, 0x12);
    TEST("TEST", "AX, [DI+52]",       0x85, 0105, 0x34);
    TEST("TEST", "CX, [BP+1234H]",    0x85, 0216, 0x34, 0x12);
    TEST("TEST", "DX, [BX+SI]",       0x85, 0020);
    TEST("TEST", "BX, [BX+DI+52]",    0x85, 0131, 0x34);
    TEST("TEST", "SP, [BP+SI+1234H]", 0x85, 0242, 0x34, 0x12);

    TEST("TEST", "AL, 56H",                       0xF6, 0300, 0x56);
    TEST("TEST", "CL, 56H",                       0xF6, 0301, 0x56);
    TEST("TEST", "BYTE PTR [SI], 56H",            0xF6, 0004, 0x56);
    TEST("TEST", "BYTE PTR [89ABH], 56H",         0xF6, 0006, 0xAB, 0x89, 0x56);
    TEST("TEST", "BYTE PTR [DI-52], 56H",         0xF6, 0105, 0xCC, 0x56);
    TEST("TEST", "BYTE PTR [BP+89ABH], 56H",      0xF6, 0206, 0xAB, 0x89, 0x56);
    TEST("TEST", "BYTE PTR [BX+SI], 56H",         0xF6, 0000, 0x56);
    TEST("TEST", "BYTE PTR [BX+DI+52], 56H",      0xF6, 0101, 0x34, 0x56);
    TEST("TEST", "BYTE PTR [BP+SI+1234H], 56H",   0xF6, 0202, 0x34, 0x12, 0x56);
    TEST("TEST", "AX, 5678H",                     0xF7, 0300, 0x78, 0x56);
    TEST("TEST", "CX, 5678H",                     0xF7, 0301, 0x78, 0x56);
    TEST("TEST", "WORD PTR [SI], 5678H",          0xF7, 0004, 0x78, 0x56);
    TEST("TEST", "WORD PTR [1234H], 5678H",       0xF7, 0006, 0x34, 0x12, 0x78, 0x56);
    TEST("TEST", "WORD PTR [DI+52], 5678H",       0xF7, 0105, 0x34, 0x78, 0x56);
    TEST("TEST", "WORD PTR [BP+1234H], 5678H",    0xF7, 0206, 0x34, 0x12, 0x78, 0x56);
    TEST("TEST", "WORD PTR [BX+SI], 5678H",       0xF7, 0000, 0x78, 0x56);
    TEST("TEST", "WORD PTR [BX+DI-52], 5678H",    0xF7, 0101, 0xCC, 0x78, 0x56);
    TEST("TEST", "WORD PTR [BP+SI+89ABH], 5678H", 0xF7, 0202, 0xAB, 0x89, 0x78, 0x56);

    TEST("TEST", "AL, 56H",   0xA8, 0x56);
    TEST("TEST", "AX, 5678H", 0xA9, 0x78, 0x56);

    TEST("OR", "AL, CL",            0x08, 0310);
    TEST("OR", "DL, BL",            0x08, 0332);
    TEST("OR", "AH, CH",            0x08, 0354);
    TEST("OR", "[SI], DH",          0x08, 0064);
    TEST("OR", "[1234H], BH",       0x08, 0076, 0x34, 0x12);
    TEST("OR", "[DI+52], AL",       0x08, 0105, 0x34);
    TEST("OR", "[BP+1234H], CL",    0x08, 0216, 0x34, 0x12);
    TEST("OR", "[BX+SI], DL",       0x08, 0020);
    TEST("OR", "[BX+DI+52], BL",    0x08, 0131, 0x34);
    TEST("OR", "[BP+SI+1234H], AH", 0x08, 0242, 0x34, 0x12);
    TEST("OR", "DH, [SI]",          0x0A, 0064);
    TEST("OR", "BH, [1234H]",       0x0A, 0076, 0x34, 0x12);
    TEST("OR", "AL, [DI-52]",       0x0A, 0105, 0xCC);
    TEST("OR", "CL, [BP+1234H]",    0x0A, 0216, 0x34, 0x12);
    TEST("OR", "DL, [BX+SI]",       0x0A, 0020);
    TEST("OR", "BL, [BX+DI-52]",    0x0A, 0131, 0xCC);
    TEST("OR", "AH, [BP+SI+1234H]", 0x0A, 0242, 0x34, 0x12);
    TEST("OR", "AX, CX",            0x09, 0310);
    TEST("OR", "DX, BX",            0x09, 0332);
    TEST("OR", "SP, BP",            0x09, 0354);
    TEST("OR", "[SI], SI",          0x09, 0064);
    TEST("OR", "[1234H], DI",       0x09, 0076, 0x34, 0x12);
    TEST("OR", "[DI-52], AX",       0x09, 0105, 0xCC);
    TEST("OR", "[BP+1234H], CX",    0x09, 0216, 0x34, 0x12);
    TEST("OR", "[BX+SI], DX",       0x09, 0020);
    TEST("OR", "[BX+DI+52], BX",    0x09, 0131, 0x34);
    TEST("OR", "[BP+SI+1234H], SP", 0x09, 0242, 0x34, 0x12);
    TEST("OR", "SI, [SI]",          0x0B, 0064);
    TEST("OR", "DI, [1234H]",       0x0B, 0076, 0x34, 0x12);
    TEST("OR", "AX, [DI+52]",       0x0B, 0105, 0x34);
    TEST("OR", "CX, [BP+1234H]",    0x0B, 0216, 0x34, 0x12);
    TEST("OR", "DX, [BX+SI]",       0x0B, 0020);
    TEST("OR", "BX, [BX+DI+52]",    0x0B, 0131, 0x34);
    TEST("OR", "SP, [BP+SI+1234H]", 0x0B, 0242, 0x34, 0x12);

    TEST("OR", "AL, 56H",                       0x80, 0310, 0x56);
    TEST("OR", "CL, 56H",                       0x80, 0311, 0x56);
    TEST("OR", "BYTE PTR [SI], 56H",            0x80, 0014, 0x56);
    TEST("OR", "BYTE PTR [89ABH], 56H",         0x80, 0016, 0xAB, 0x89, 0x56);
    TEST("OR", "BYTE PTR [DI-52], 56H",         0x80, 0115, 0xCC, 0x56);
    TEST("OR", "BYTE PTR [BP+89ABH], 56H",      0x80, 0216, 0xAB, 0x89, 0x56);
    TEST("OR", "BYTE PTR [BX+SI], 56H",         0x80, 0010, 0x56);
    TEST("OR", "BYTE PTR [BX+DI+52], 56H",      0x80, 0111, 0x34, 0x56);
    TEST("OR", "BYTE PTR [BP+SI+1234H], 56H",   0x80, 0212, 0x34, 0x12, 0x56);
    TEST("OR", "AX, 5678H",                     0x81, 0310, 0x78, 0x56);
    TEST("OR", "CX, 5678H",                     0x81, 0311, 0x78, 0x56);
    TEST("OR", "WORD PTR [SI], 5678H",          0x81, 0014, 0x78, 0x56);
    TEST("OR", "WORD PTR [1234H], 5678H",       0x81, 0016, 0x34, 0x12, 0x78, 0x56);
    TEST("OR", "WORD PTR [DI+52], 5678H",       0x81, 0115, 0x34, 0x78, 0x56);
    TEST("OR", "WORD PTR [BP+1234H], 5678H",    0x81, 0216, 0x34, 0x12, 0x78, 0x56);
    TEST("OR", "WORD PTR [BX+SI], 5678H",       0x81, 0010, 0x78, 0x56);
    TEST("OR", "WORD PTR [BX+DI-52], 5678H",    0x81, 0111, 0xCC, 0x78, 0x56);
    TEST("OR", "WORD PTR [BP+SI+89ABH], 5678H", 0x81, 0212, 0xAB, 0x89, 0x78, 0x56);
    TEST("OR", "AX, 56H",                       0x83, 0310, 0x56);
    TEST("OR", "AX, -16",                       0x83, 0310, 0xF0);
    TEST("OR", "CX, 56H",                       0x83, 0311, 0x56);
    TEST("OR", "CX, -16",                       0x83, 0311, 0xF0);
    TEST("OR", "WORD PTR [SI], 56H",            0x83, 0014, 0x56);
    TEST("OR", "WORD PTR [1234H], 56H",         0x83, 0016, 0x34, 0x12, 0x56);
    TEST("OR", "WORD PTR [DI+52], 56H",         0x83, 0115, 0x34, 0x56);
    TEST("OR", "WORD PTR [BP+1234H], 56H",      0x83, 0216, 0x34, 0x12, 0x56);
    TEST("OR", "WORD PTR [BX+SI], 56H",         0x83, 0010, 0x56);
    TEST("OR", "WORD PTR [BX+DI-52], 56H",      0x83, 0111, 0xCC, 0x56);
    TEST("OR", "WORD PTR [BP+SI+89ABH], 56H",   0x83, 0212, 0xAB, 0x89, 0x56);

    TEST("OR", "AL, 56H",   0x0C, 0x56);
    TEST("OR", "AX, 5678H", 0x0D, 0x78, 0x56);

    TEST("XOR", "AL, CL",            0x30, 0310);
    TEST("XOR", "DL, BL",            0x30, 0332);
    TEST("XOR", "AH, CH",            0x30, 0354);
    TEST("XOR", "[SI], DH",          0x30, 0064);
    TEST("XOR", "[1234H], BH",       0x30, 0076, 0x34, 0x12);
    TEST("XOR", "[DI+52], AL",       0x30, 0105, 0x34);
    TEST("XOR", "[BP+1234H], CL",    0x30, 0216, 0x34, 0x12);
    TEST("XOR", "[BX+SI], DL",       0x30, 0020);
    TEST("XOR", "[BX+DI+52], BL",    0x30, 0131, 0x34);
    TEST("XOR", "[BP+SI+1234H], AH", 0x30, 0242, 0x34, 0x12);
    TEST("XOR", "DH, [SI]",          0x32, 0064);
    TEST("XOR", "BH, [1234H]",       0x32, 0076, 0x34, 0x12);
    TEST("XOR", "AL, [DI-52]",       0x32, 0105, 0xCC);
    TEST("XOR", "CL, [BP+1234H]",    0x32, 0216, 0x34, 0x12);
    TEST("XOR", "DL, [BX+SI]",       0x32, 0020);
    TEST("XOR", "BL, [BX+DI-52]",    0x32, 0131, 0xCC);
    TEST("XOR", "AH, [BP+SI+1234H]", 0x32, 0242, 0x34, 0x12);
    TEST("XOR", "AX, CX",            0x31, 0310);
    TEST("XOR", "DX, BX",            0x31, 0332);
    TEST("XOR", "SP, BP",            0x31, 0354);
    TEST("XOR", "[SI], SI",          0x31, 0064);
    TEST("XOR", "[1234H], DI",       0x31, 0076, 0x34, 0x12);
    TEST("XOR", "[DI-52], AX",       0x31, 0105, 0xCC);
    TEST("XOR", "[BP+1234H], CX",    0x31, 0216, 0x34, 0x12);
    TEST("XOR", "[BX+SI], DX",       0x31, 0020);
    TEST("XOR", "[BX+DI+52], BX",    0x31, 0131, 0x34);
    TEST("XOR", "[BP+SI+1234H], SP", 0x31, 0242, 0x34, 0x12);
    TEST("XOR", "SI, [SI]",          0x33, 0064);
    TEST("XOR", "DI, [1234H]",       0x33, 0076, 0x34, 0x12);
    TEST("XOR", "AX, [DI+52]",       0x33, 0105, 0x34);
    TEST("XOR", "CX, [BP+1234H]",    0x33, 0216, 0x34, 0x12);
    TEST("XOR", "DX, [BX+SI]",       0x33, 0020);
    TEST("XOR", "BX, [BX+DI+52]",    0x33, 0131, 0x34);
    TEST("XOR", "SP, [BP+SI+1234H]", 0x33, 0242, 0x34, 0x12);

    TEST("XOR", "AL, 56H",                       0x80, 0360, 0x56);
    TEST("XOR", "CL, 56H",                       0x80, 0361, 0x56);
    TEST("XOR", "BYTE PTR [SI], 56H",            0x80, 0064, 0x56);
    TEST("XOR", "BYTE PTR [89ABH], 56H",         0x80, 0066, 0xAB, 0x89, 0x56);
    TEST("XOR", "BYTE PTR [DI-52], 56H",         0x80, 0165, 0xCC, 0x56);
    TEST("XOR", "BYTE PTR [BP+89ABH], 56H",      0x80, 0266, 0xAB, 0x89, 0x56);
    TEST("XOR", "BYTE PTR [BX+SI], 56H",         0x80, 0060, 0x56);
    TEST("XOR", "BYTE PTR [BX+DI+52], 56H",      0x80, 0161, 0x34, 0x56);
    TEST("XOR", "BYTE PTR [BP+SI+1234H], 56H",   0x80, 0262, 0x34, 0x12, 0x56);
    TEST("XOR", "AX, 5678H",                     0x81, 0360, 0x78, 0x56);
    TEST("XOR", "CX, 5678H",                     0x81, 0361, 0x78, 0x56);
    TEST("XOR", "WORD PTR [SI], 5678H",          0x81, 0064, 0x78, 0x56);
    TEST("XOR", "WORD PTR [1234H], 5678H",       0x81, 0066, 0x34, 0x12, 0x78, 0x56);
    TEST("XOR", "WORD PTR [DI+52], 5678H",       0x81, 0165, 0x34, 0x78, 0x56);
    TEST("XOR", "WORD PTR [BP+1234H], 5678H",    0x81, 0266, 0x34, 0x12, 0x78, 0x56);
    TEST("XOR", "WORD PTR [BX+SI], 5678H",       0x81, 0060, 0x78, 0x56);
    TEST("XOR", "WORD PTR [BX+DI-52], 5678H",    0x81, 0161, 0xCC, 0x78, 0x56);
    TEST("XOR", "WORD PTR [BP+SI+89ABH], 5678H", 0x81, 0262, 0xAB, 0x89, 0x78, 0x56);
    TEST("XOR", "AX, 56H",                       0x83, 0360, 0x56);
    TEST("XOR", "AX, -16",                       0x83, 0360, 0xF0);
    TEST("XOR", "CX, 56H",                       0x83, 0361, 0x56);
    TEST("XOR", "CX, -16",                       0x83, 0361, 0xF0);
    TEST("XOR", "WORD PTR [SI], 56H",            0x83, 0064, 0x56);
    TEST("XOR", "WORD PTR [1234H], 56H",         0x83, 0066, 0x34, 0x12, 0x56);
    TEST("XOR", "WORD PTR [DI+52], 56H",         0x83, 0165, 0x34, 0x56);
    TEST("XOR", "WORD PTR [BP+1234H], 56H",      0x83, 0266, 0x34, 0x12, 0x56);
    TEST("XOR", "WORD PTR [BX+SI], 56H",         0x83, 0060, 0x56);
    TEST("XOR", "WORD PTR [BX+DI-52], 56H",      0x83, 0161, 0xCC, 0x56);
    TEST("XOR", "WORD PTR [BP+SI+89ABH], 56H",   0x83, 0262, 0xAB, 0x89, 0x56);

    TEST("XOR", "AL, 56H",   0x34, 0x56);
    TEST("XOR", "AX, 5678H", 0x35, 0x78, 0x56);

    if (v30()) {
        TEST("CLR1", "CH, 1",                      0x0F, 0x1A, 0305, 0x01);
        TEST("CLR1", "BYTE PTR [SI], 2",           0x0F, 0x1A, 0004, 0x02);
        TEST("CLR1", "BYTE PTR [1234H], 3",        0x0F, 0x1A, 0006, 0x34, 0x12, 0x03);
        TEST("CLR1", "BYTE PTR [DI-52], 4",        0x0F, 0x1A, 0105, 0xCC, 0x04);
        TEST("CLR1", "BYTE PTR [BP+1234H], 5",     0x0F, 0x1A, 0206, 0x34, 0x12, 0x05);
        TEST("CLR1", "BYTE PTR [BX+SI], 6",        0x0F, 0x1A, 0000, 0x06);
        TEST("CLR1", "BYTE PTR [BX+DI+52], 7",     0x0F, 0x1A, 0101, 0x34, 0x07);
        TEST("CLR1", "BYTE PTR [BP+SI+1234H], 0",  0x0F, 0x1A, 0202, 0x34, 0x12, 0x00);
        TEST("CLR1", "BP, 1",                      0x0F, 0x1B, 0305, 0x01);
        TEST("CLR1", "WORD PTR [SI], 3",           0x0F, 0x1B, 0004, 0x03);
        TEST("CLR1", "WORD PTR [1234H], 5",        0x0F, 0x1B, 0006, 0x34, 0x12, 0x05);
        TEST("CLR1", "WORD PTR [DI-52], 7",        0x0F, 0x1B, 0105, 0xCC, 0x07);
        TEST("CLR1", "WORD PTR [BP+1234H], 9",     0x0F, 0x1B, 0206, 0x34, 0x12, 0x09);
        TEST("CLR1", "WORD PTR [BX+SI], 11",       0x0F, 0x1B, 0000, 0x0B);
        TEST("CLR1", "WORD PTR [BX+DI+52], 13",    0x0F, 0x1B, 0101, 0x34, 0x0D);
        TEST("CLR1", "WORD PTR [BP+SI+1234H], 15", 0x0F, 0x1B, 0202, 0x34, 0x12, 0x0F);
        TEST("CLR1", "CH, CL",                     0x0F, 0x12, 0305);
        TEST("CLR1", "BYTE PTR [SI], CL",          0x0F, 0x12, 0004);
        TEST("CLR1", "BYTE PTR [1234H], CL",       0x0F, 0x12, 0006, 0x34, 0x12);
        TEST("CLR1", "BYTE PTR [DI-52], CL",       0x0F, 0x12, 0105, 0xCC);
        TEST("CLR1", "BYTE PTR [BP+1234H], CL",    0x0F, 0x12, 0206, 0x34, 0x12);
        TEST("CLR1", "BYTE PTR [BX+SI], CL",       0x0F, 0x12, 0000);
        TEST("CLR1", "BYTE PTR [BX+DI+52], CL",    0x0F, 0x12, 0101, 0x34);
        TEST("CLR1", "BYTE PTR [BP+SI+1234H], CL", 0x0F, 0x12, 0202, 0x34, 0x12);
        TEST("CLR1", "BP, CL",                     0x0F, 0x13, 0305);
        TEST("CLR1", "WORD PTR [SI], CL",          0x0F, 0x13, 0004);
        TEST("CLR1", "WORD PTR [1234H], CL",       0x0F, 0x13, 0006, 0x34, 0x12);
        TEST("CLR1", "WORD PTR [DI-52], CL",       0x0F, 0x13, 0105, 0xCC);
        TEST("CLR1", "WORD PTR [BP+1234H], CL",    0x0F, 0x13, 0206, 0x34, 0x12);
        TEST("CLR1", "WORD PTR [BX+SI], CL",       0x0F, 0x13, 0000);
        TEST("CLR1", "WORD PTR [BX+DI+52], CL",    0x0F, 0x13, 0101, 0x34);
        TEST("CLR1", "WORD PTR [BP+SI+1234H], CL", 0x0F, 0x13, 0202, 0x34, 0x12);

        TEST("SET1", "CH, 1",                      0x0F, 0x1C, 0305, 0x01);
        TEST("SET1", "BYTE PTR [SI], 2",           0x0F, 0x1C, 0004, 0x02);
        TEST("SET1", "BYTE PTR [1234H], 3",        0x0F, 0x1C, 0006, 0x34, 0x12, 0x03);
        TEST("SET1", "BYTE PTR [DI-52], 4",        0x0F, 0x1C, 0105, 0xCC, 0x04);
        TEST("SET1", "BYTE PTR [BP+1234H], 5",     0x0F, 0x1C, 0206, 0x34, 0x12, 0x05);
        TEST("SET1", "BYTE PTR [BX+SI], 6",        0x0F, 0x1C, 0000, 0x06);
        TEST("SET1", "BYTE PTR [BX+DI+52], 7",     0x0F, 0x1C, 0101, 0x34, 0x07);
        TEST("SET1", "BYTE PTR [BP+SI+1234H], 0",  0x0F, 0x1C, 0202, 0x34, 0x12, 0x00);
        TEST("SET1", "BP, 1",                      0x0F, 0x1D, 0305, 0x01);
        TEST("SET1", "WORD PTR [SI], 3",           0x0F, 0x1D, 0004, 0x03);
        TEST("SET1", "WORD PTR [1234H], 5",        0x0F, 0x1D, 0006, 0x34, 0x12, 0x05);
        TEST("SET1", "WORD PTR [DI-52], 7",        0x0F, 0x1D, 0105, 0xCC, 0x07);
        TEST("SET1", "WORD PTR [BP+1234H], 9",     0x0F, 0x1D, 0206, 0x34, 0x12, 0x09);
        TEST("SET1", "WORD PTR [BX+SI], 11",       0x0F, 0x1D, 0000, 0x0B);
        TEST("SET1", "WORD PTR [BX+DI+52], 13",    0x0F, 0x1D, 0101, 0x34, 0x0D);
        TEST("SET1", "WORD PTR [BP+SI+1234H], 15", 0x0F, 0x1D, 0202, 0x34, 0x12, 0x0F);
        TEST("SET1", "CH, CL",                     0x0F, 0x14, 0305);
        TEST("SET1", "BYTE PTR [SI], CL",          0x0F, 0x14, 0004);
        TEST("SET1", "BYTE PTR [1234H], CL",       0x0F, 0x14, 0006, 0x34, 0x12);
        TEST("SET1", "BYTE PTR [DI-52], CL",       0x0F, 0x14, 0105, 0xCC);
        TEST("SET1", "BYTE PTR [BP+1234H], CL",    0x0F, 0x14, 0206, 0x34, 0x12);
        TEST("SET1", "BYTE PTR [BX+SI], CL",       0x0F, 0x14, 0000);
        TEST("SET1", "BYTE PTR [BX+DI+52], CL",    0x0F, 0x14, 0101, 0x34);
        TEST("SET1", "BYTE PTR [BP+SI+1234H], CL", 0x0F, 0x14, 0202, 0x34, 0x12);
        TEST("SET1", "BP, CL",                     0x0F, 0x15, 0305);
        TEST("SET1", "WORD PTR [SI], CL",          0x0F, 0x15, 0004);
        TEST("SET1", "WORD PTR [1234H], CL",       0x0F, 0x15, 0006, 0x34, 0x12);
        TEST("SET1", "WORD PTR [DI-52], CL",       0x0F, 0x15, 0105, 0xCC);
        TEST("SET1", "WORD PTR [BP+1234H], CL",    0x0F, 0x15, 0206, 0x34, 0x12);
        TEST("SET1", "WORD PTR [BX+SI], CL",       0x0F, 0x15, 0000);
        TEST("SET1", "WORD PTR [BX+DI+52], CL",    0x0F, 0x15, 0101, 0x34);
        TEST("SET1", "WORD PTR [BP+SI+1234H], CL", 0x0F, 0x15, 0202, 0x34, 0x12);

        TEST("NOT1", "CH, 1",                      0x0F, 0x1E, 0305, 0x01);
        TEST("NOT1", "BYTE PTR [SI], 2",           0x0F, 0x1E, 0004, 0x02);
        TEST("NOT1", "BYTE PTR [1234H], 3",        0x0F, 0x1E, 0006, 0x34, 0x12, 0x03);
        TEST("NOT1", "BYTE PTR [DI-52], 4",        0x0F, 0x1E, 0105, 0xCC, 0x04);
        TEST("NOT1", "BYTE PTR [BP+1234H], 5",     0x0F, 0x1E, 0206, 0x34, 0x12, 0x05);
        TEST("NOT1", "BYTE PTR [BX+SI], 6",        0x0F, 0x1E, 0000, 0x06);
        TEST("NOT1", "BYTE PTR [BX+DI+52], 7",     0x0F, 0x1E, 0101, 0x34, 0x07);
        TEST("NOT1", "BYTE PTR [BP+SI+1234H], 0",  0x0F, 0x1E, 0202, 0x34, 0x12, 0x00);
        TEST("NOT1", "BP, 1",                      0x0F, 0x1F, 0305, 0x01);
        TEST("NOT1", "WORD PTR [SI], 3",           0x0F, 0x1F, 0004, 0x03);
        TEST("NOT1", "WORD PTR [1234H], 5",        0x0F, 0x1F, 0006, 0x34, 0x12, 0x05);
        TEST("NOT1", "WORD PTR [DI-52], 7",        0x0F, 0x1F, 0105, 0xCC, 0x07);
        TEST("NOT1", "WORD PTR [BP+1234H], 9",     0x0F, 0x1F, 0206, 0x34, 0x12, 0x09);
        TEST("NOT1", "WORD PTR [BX+SI], 11",       0x0F, 0x1F, 0000, 0x0B);
        TEST("NOT1", "WORD PTR [BX+DI+52], 13",    0x0F, 0x1F, 0101, 0x34, 0x0D);
        TEST("NOT1", "WORD PTR [BP+SI+1234H], 15", 0x0F, 0x1F, 0202, 0x34, 0x12, 0x0F);
        TEST("NOT1", "CH, CL",                     0x0F, 0x16, 0305);
        TEST("NOT1", "BYTE PTR [SI], CL",          0x0F, 0x16, 0004);
        TEST("NOT1", "BYTE PTR [1234H], CL",       0x0F, 0x16, 0006, 0x34, 0x12);
        TEST("NOT1", "BYTE PTR [DI-52], CL",       0x0F, 0x16, 0105, 0xCC);
        TEST("NOT1", "BYTE PTR [BP+1234H], CL",    0x0F, 0x16, 0206, 0x34, 0x12);
        TEST("NOT1", "BYTE PTR [BX+SI], CL",       0x0F, 0x16, 0000);
        TEST("NOT1", "BYTE PTR [BX+DI+52], CL",    0x0F, 0x16, 0101, 0x34);
        TEST("NOT1", "BYTE PTR [BP+SI+1234H], CL", 0x0F, 0x16, 0202, 0x34, 0x12);
        TEST("NOT1", "BP, CL",                     0x0F, 0x17, 0305);
        TEST("NOT1", "WORD PTR [SI], CL",          0x0F, 0x17, 0004);
        TEST("NOT1", "WORD PTR [1234H], CL",       0x0F, 0x17, 0006, 0x34, 0x12);
        TEST("NOT1", "WORD PTR [DI-52], CL",       0x0F, 0x17, 0105, 0xCC);
        TEST("NOT1", "WORD PTR [BP+1234H], CL",    0x0F, 0x17, 0206, 0x34, 0x12);
        TEST("NOT1", "WORD PTR [BX+SI], CL",       0x0F, 0x17, 0000);
        TEST("NOT1", "WORD PTR [BX+DI+52], CL",    0x0F, 0x17, 0101, 0x34);
        TEST("NOT1", "WORD PTR [BP+SI+1234H], CL", 0x0F, 0x17, 0202, 0x34, 0x12);
    }
}

static void test_string_manipulation() {
    disassembler.setOption("string-insn", "false");
    TEST("REPNE", "", 0xF2);
    TEST("REP",   "", 0xF3);
    if (v30()) {
        TEST("REPNC", "", 0x64);
        TEST("REPC",  "", 0x65);
    }

    disassembler.setOption("string-insn", "enable");
    ERRT("REPNE", "", NO_MEMORY, "", 0xF2);
    ERRT("REP",   "", NO_MEMORY, "", 0xF3);
    if (v30()) {
        ERRT("REPNC", "", NO_MEMORY, "", 0x64);
        ERRT("REPC",  "", NO_MEMORY, "", 0x65);
    }

    TEST("REPNE", "MOVSB", 0xF2, 0xA4);
    TEST("REPNE", "MOVSW", 0xF2, 0xA5);
    TEST("REPNE", "CMPSB", 0xF2, 0xA6);
    TEST("REPNE", "CMPSW", 0xF2, 0xA7);
    TEST("REPNE", "STOSB", 0xF2, 0xAA);
    TEST("REPNE", "STOSW", 0xF2, 0xAB);
    TEST("REPNE", "LODSB", 0xF2, 0xAC);
    TEST("REPNE", "LODSW", 0xF2, 0xAD);
    TEST("REPNE", "SCASB", 0xF2, 0xAE);
    TEST("REPNE", "SCASW", 0xF2, 0xAF);
    ERRT("REPNE", "", UNKNOWN_INSTRUCTION, "", 0xF2, 0x90); // NOP
    ERRT("REPNE", "", UNKNOWN_INSTRUCTION, "", 0xF2, 0xFF); // JMP [SI]
    if (is80186()) {
        TEST("REPNE", "INSB",  0xF2, 0x6C);
        TEST("REPNE", "INSW",  0xF2, 0x6D);
        TEST("REPNE", "OUTSB", 0xF2, 0x6E);
        TEST("REPNE", "OUTSW", 0xF2, 0x6F);
    }

    TEST("REP", "MOVSB", 0xF3, 0xA4);
    TEST("REP", "MOVSW", 0xF3, 0xA5);
    TEST("REP", "CMPSB", 0xF3, 0xA6);
    TEST("REP", "CMPSW", 0xF3, 0xA7);
    TEST("REP", "STOSB", 0xF3, 0xAA);
    TEST("REP", "STOSW", 0xF3, 0xAB);
    TEST("REP", "LODSB", 0xF3, 0xAC);
    TEST("REP", "LODSW", 0xF3, 0xAD);
    TEST("REP", "SCASB", 0xF3, 0xAE);
    TEST("REP", "SCASW", 0xF3, 0xAF);
    if (is80186()) {
        TEST("REP", "INSB",  0xF3, 0x6C);
        TEST("REP", "INSW",  0xF3, 0x6D);
        TEST("REP", "OUTSB", 0xF3, 0x6E);
        TEST("REP", "OUTSW", 0xF3, 0x6F);
    }

    if (v30()) {
        TEST("REPNC", "CMPSB", 0x64, 0xA6);
        TEST("REPC",  "SCASW", 0x65, 0xAF);
    }

    TEST("MOVSB", "", 0xA4);
    TEST("MOVSW", "", 0xA5);
    TEST("CMPSB", "", 0xA6);
    TEST("CMPSW", "", 0xA7);
    TEST("STOSB", "", 0xAA);
    TEST("STOSW", "", 0xAB);
    TEST("LODSB", "", 0xAC);
    TEST("LODSW", "", 0xAD);
    TEST("SCASB", "", 0xAE);
    TEST("SCASW", "", 0xAF);

    if (is80186()) {
        TEST("INSB",  "", 0x6C);
        TEST("INSW",  "", 0x6D);
        TEST("OUTSB", "", 0x6E);
        TEST("OUTSW", "", 0x6F);
    }

    if (v30()) {
        TEST("ADD4S", "", 0x0F, 0x20);
        TEST("CMP4S", "", 0x0F, 0x26);
        TEST("SUB4S", "", 0x0F, 0x22);
    }
}

static void test_control_transfer() {
    disassembler.setOption("relative", "off");

    ATEST(0x01000, "CALL", "01082H",                             0xE8, 0x7F, 0x00);
    ATEST(0x01000, "CALL", "09002H",                             0xE8, 0xFF, 0x7F);
    AERRT(0x0F000, "CALL", "17002H", OVERWRAP_SEGMENT, "17002H", 0xE8, 0xFF, 0x7F);
    AERRT(0xFF000, "CALL", "07002H", OVERFLOW_RANGE,   "07002H", 0xE8, 0xFF, 0x7F);
    ATEST(0x01000, "CALL", "00F81H",                             0xE8, 0x7E, 0xFF);
    ATEST(0x09000, "CALL", "01003H",                             0xE8, 0x00, 0x80);
    AERRT(0x11000, "CALL", "09003H", OVERWRAP_SEGMENT, "09003H", 0xE8, 0x00, 0x80);
    AERRT(0x01000, "CALL", "0F9003H", OVERFLOW_RANGE, "0F9003H", 0xE8, 0x00, 0x80);

    TEST("CALL", "AX",            0xFF, 0320);
    TEST("CALL", "SI",            0xFF, 0326);
    TEST("CALL", "[SI]",          0xFF, 0024);
    TEST("CALL", "[1234H]",       0xFF, 0026, 0x34, 0x12);
    TEST("CALL", "[DI-52]",       0xFF, 0125, 0xCC);
    TEST("CALL", "[BP+1234H]",    0xFF, 0226, 0x34, 0x12);
    TEST("CALL", "[BX+SI]",       0xFF, 0020);
    TEST("CALL", "[BX+DI+52]",    0xFF, 0121, 0x34);
    TEST("CALL", "[BP+SI+1234H]", 0xFF, 0222, 0x34, 0x12);

    TEST("CALLF", "1234H:5678H",   0x9A, 0x78, 0x56, 0x34, 0x12);

    TEST("CALLF", "[SI]",          0xFF, 0034);
    TEST("CALLF", "[1234H]",       0xFF, 0036, 0x34, 0x12);
    TEST("CALLF", "[DI-52]",       0xFF, 0135, 0xCC);
    TEST("CALLF", "[BP+1234H]",    0xFF, 0236, 0x34, 0x12);
    TEST("CALLF", "[BX+SI]",       0xFF, 0030);
    TEST("CALLF", "[BX+DI+52]",    0xFF, 0131, 0x34);
    TEST("CALLF", "[BP+SI+1234H]", 0xFF, 0232, 0x34, 0x12);

    ATEST(0x01000, "JMP", "01081H",                             0xEB, 0x7F);
    AERRT(0x0FFF0, "JMP", "10071H", OVERWRAP_SEGMENT, "10071H", 0xEB, 0x7F);
    ATEST(0x01000, "JMP", "01082H",                             0xE9, 0x7F, 0x00);
    ATEST(0x01000, "JMP", "09002H",                             0xE9, 0xFF, 0x7F);
    AERRT(0x0F000, "JMP", "17002H", OVERWRAP_SEGMENT, "17002H", 0xE9, 0xFF, 0x7F);
    ATEST(0x01000, "JMP", "00F82H",                             0xEB, 0x80);
    AERRT(0x10010, "JMP", "0FF92H", OVERWRAP_SEGMENT, "0FF92H", 0xEB, 0x80);
    ATEST(0x01000, "JMP", "00F81H",                             0xE9, 0x7E, 0xFF);
    ATEST(0x09000, "JMP", "01003H",                             0xE9, 0x00, 0x80);
    AERRT(0x11000, "JMP", "09003H", OVERWRAP_SEGMENT, "09003H", 0xE9, 0x00, 0x80);

    TEST("JMP", "AX",            0xFF, 0340);
    TEST("JMP", "SI",            0xFF, 0346);
    TEST("JMP", "[SI]",          0xFF, 0044);
    TEST("JMP", "[1234H]",       0xFF, 0046, 0x34, 0x12);
    TEST("JMP", "[DI-52]",       0xFF, 0145, 0xCC);
    TEST("JMP", "[BP+1234H]",    0xFF, 0246, 0x34, 0x12);
    TEST("JMP", "[BX+SI]",       0xFF, 0040);
    TEST("JMP", "[BX+DI+52]",    0xFF, 0141, 0x34);
    TEST("JMP", "[BP+SI+1234H]", 0xFF, 0242, 0x34, 0x12);

    TEST("JMPF", "1234H:5678H",  0xEA, 0x78, 0x56, 0x34, 0x12);

    TEST("JMPF", "[SI]",          0xFF, 0054);
    TEST("JMPF", "[1234H]",       0xFF, 0056, 0x34, 0x12);
    TEST("JMPF", "[DI-52]",       0xFF, 0155, 0xCC);
    TEST("JMPF", "[BP+1234H]",    0xFF, 0256, 0x34, 0x12);
    TEST("JMPF", "[BX+SI]",       0xFF, 0050);
    TEST("JMPF", "[BX+DI+52]",    0xFF, 0151, 0x34);
    TEST("JMPF", "[BP+SI+1234H]", 0xFF, 0252, 0x34, 0x12);

    TEST("RET", "",    0xC3);
    TEST("RET", "16",  0xC2, 0x10, 0x00);
    TEST("RETF", "",   0xCB);
    TEST("RETF", "16", 0xCA, 0x10, 0x00);

    disassembler.setOption("relative", "on");
    TEST("JO",  "$", 0x70, 0xFE);
    TEST("JNO", "$", 0x71, 0xFE);
    TEST("JB",  "$", 0x72, 0xFE);
    TEST("JAE", "$", 0x73, 0xFE);
    TEST("JE",  "$", 0x74, 0xFE);
    TEST("JNE", "$", 0x75, 0xFE);
    TEST("JBE", "$", 0x76, 0xFE);
    TEST("JA",  "$", 0x77, 0xFE);
    TEST("JS",  "$", 0x78, 0xFE);
    TEST("JNS", "$", 0x79, 0xFE);
    TEST("JPE", "$", 0x7A, 0xFE);
    TEST("JPO", "$", 0x7B, 0xFE);
    TEST("JL",  "$", 0x7C, 0xFE);
    TEST("JGE", "$", 0x7D, 0xFE);
    TEST("JLE", "$", 0x7E, 0xFE);
    TEST("JG",  "$", 0x7F, 0xFE);
    ATEST(0x01000, "JS", "$+129",                               0x78, 0x7F);
    AERRT(0x0FFC0, "JS", "$+129", OVERWRAP_SEGMENT,    "$+129", 0x78, 0x7F);
    ATEST(0x01000, "JS", "$-126",                               0x78, 0x80);
    AERRT(0x10040, "JS", "$-126", OVERWRAP_SEGMENT,    "$-126", 0x78, 0x80);

    TEST("LOOP",   "$", 0xE2, 0xFE);
    TEST("LOOPE",  "$", 0xE1, 0xFE);
    TEST("LOOPNE", "$", 0xE0, 0xFE);
    TEST("JCXZ",   "$", 0xE3, 0xFE);

    TEST("INT",  "2", 0xCD, 0x02);
    TEST("INT",  "3", 0xCC);
    TEST("INTO", "",  0xCE);
    TEST("IRET", "",  0xCF);

    if (is80186()) {
        TEST("ENTER", "30, 1",    0xC8, 0x1E, 0x00, 0x01);
        TEST("ENTER", "65535, 0", 0xC8, 0xFF, 0xFF, 0x00);
        TEST("ENTER", "0, 255",   0xC8, 0x00, 0x00, 0xFF);

        TEST("LEAVE", "", 0xC9);

        TEST("BOUND", "SI, [SI]",          0x62, 0064);
        TEST("BOUND", "DI, [1234H]",       0x62, 0076, 0x34, 0x12);
        TEST("BOUND", "AX, [DI+52]",       0x62, 0105, 0x34);
        TEST("BOUND", "CX, [BP+1234H]",    0x62, 0216, 0x34, 0x12);
        TEST("BOUND", "DX, [BX+SI]",       0x62, 0020);
        TEST("BOUND", "BX, [BX+DI+52]",    0x62, 0131, 0x34);
        TEST("BOUND", "SP, [BP+SI+1234H]", 0x62, 0242, 0x34, 0x12);
    }

    if (v30()) {
        TEST("BRKEM", "40H", 0x0F, 0xFF, 0x40);
    }
}

static void test_processor_control() {
    TEST("CLC",  "", 0xF8);
    TEST("CMC",  "", 0xF5);
    TEST("STC",  "", 0xF9);
    TEST("CLD",  "", 0xFC);
    TEST("STD",  "", 0xFD);
    TEST("CLI",  "", 0xFA);
    TEST("STI",  "", 0xFB);
    TEST("HLT",  "", 0xF4);
    TEST("WAIT", "", 0x9B);
    TEST("LOCK", "", 0xF0);
    TEST("NOP",  "", 0x90);
}

static void test_segment_override() {
    TEST("SEGES", "", 0x26);
    TEST("SEGCS", "", 0x2E);
    TEST("SEGSS", "", 0x36);
    TEST("SEGDS", "", 0x3E);

    disassembler.setOption("segment-insn", "disable");

    TEST("MOV", "ES:[BX], AH",    0x26, 0x88, 0047);
    TEST("MOV", "ES:[BP+0], AH",  0x26, 0x88, 0146, 0x00);
    TEST("MOV", "ES:[SI], AH",    0x26, 0x88, 0044);
    TEST("MOV", "ES:[DI], AH",    0x26, 0x88, 0045);
    TEST("MOV", "ES:[1234H], AH", 0x26, 0x88, 0046, 0x34, 0x12);
    TEST("MOV", "CS:[BX], AH",    0x2E, 0x88, 0047);
    TEST("MOV", "CS:[BP+0], AH",  0x2E, 0x88, 0146, 0x00);
    TEST("MOV", "CS:[SI], AH",    0x2E, 0x88, 0044);
    TEST("MOV", "CS:[DI], AH",    0x2E, 0x88, 0045);
    TEST("MOV", "CS:[1234H], AH", 0x2E, 0x88, 0046, 0x34, 0x12);
    TEST("MOV", "SS:[BX], AH",    0x36, 0x88, 0047);
    TEST("MOV", "SS:[BP+0], AH",  0x36, 0x88, 0146, 0x00);
    TEST("MOV", "SS:[SI], AH",    0x36, 0x88, 0044);
    TEST("MOV", "SS:[DI], AH",    0x36, 0x88, 0045);
    TEST("MOV", "SS:[1234H], AH", 0x36, 0x88, 0046, 0x34, 0x12);
    TEST("MOV", "DS:[BX], AH",    0x3E, 0x88, 0047);
    TEST("MOV", "DS:[BP+0], AH",  0x3E, 0x88, 0146, 0x00);
    TEST("MOV", "DS:[SI], AH",    0x3E, 0x88, 0044);
    TEST("MOV", "DS:[DI], AH",    0x3E, 0x88, 0045);
    TEST("MOV", "DS:[1234H], AH", 0x3E, 0x88, 0046, 0x34, 0x12);

    if (is80186()) {
        TEST("BOUND", "SI, ES:[SI]",       0x26, 0x62, 0064);
        TEST("BOUND", "DI, CS:[1234H]",    0x2E, 0x62, 0076, 0x34, 0x12);
        TEST("BOUND", "AX, SS:[DI+52]",    0x36, 0x62, 0105, 0x34);
        TEST("BOUND", "CX, DS:[BP+1234H]", 0x3E, 0x62, 0216, 0x34, 0x12);
    }

    TEST("JMP", "ES:[SI]", 0x26, 0xFF, 0044);
    TEST("JMP", "CS:[SI]", 0x2E, 0xFF, 0044);
    TEST("JMP", "SS:[SI]", 0x36, 0xFF, 0044);
    TEST("JMP", "DS:[SI]", 0x3E, 0xFF, 0044);

    TEST("MOVSB", "ES:[DI], ES:[SI]", 0x26, 0xA4);
    TEST("MOVSB", "ES:[DI], CS:[SI]", 0x2E, 0xA4);
    TEST("MOVSW", "ES:[DI], SS:[SI]", 0x36, 0xA5);
    TEST("MOVSW", "ES:[DI], DS:[SI]", 0x3E, 0xA5);
    TEST("CMPSB", "ES:[SI], ES:[DI]", 0x26, 0xA6);
    TEST("CMPSB", "CS:[SI], ES:[DI]", 0x2E, 0xA6);
    TEST("CMPSW", "SS:[SI], ES:[DI]", 0x36, 0xA7);
    TEST("CMPSW", "DS:[SI], ES:[DI]", 0x3E, 0xA7);
    TEST("LODSB", "ES:[SI]", 0x26, 0xAC);
    TEST("LODSB", "CS:[SI]", 0x2E, 0xAC);
    TEST("LODSW", "SS:[SI]", 0x36, 0xAD);
    TEST("LODSW", "DS:[SI]", 0x3E, 0xAD);

    ERRT("STOSB", "CS:[DI]", ILLEGAL_SEGMENT, "CS:[DI]", 0x2E, 0xAA);
    ERRT("STOSW", "SS:[DI]", ILLEGAL_SEGMENT, "SS:[DI]", 0x36, 0xAB);
    ERRT("STOSW", "DS:[DI]", ILLEGAL_SEGMENT, "DS:[DI]", 0x3E, 0xAB);
    ERRT("SCASB", "ES:[DI]", ILLEGAL_SEGMENT, "ES:[DI]", 0x26, 0xAE);
    ERRT("SCASB", "CS:[DI]", ILLEGAL_SEGMENT, "CS:[DI]", 0x2E, 0xAE);
    ERRT("SCASW", "SS:[DI]", ILLEGAL_SEGMENT, "SS:[DI]", 0x36, 0xAF);
    ERRT("SCASW", "DS:[DI]", ILLEGAL_SEGMENT, "DS:[DI]", 0x3E, 0xAF);

    if (is80186()) {
        TEST("OUTSB", "ES:[SI]", 0x26, 0x6E);
        TEST("OUTSB", "CS:[SI]", 0x2E, 0x6E);
        TEST("OUTSW", "SS:[SI]", 0x36, 0x6F);
        TEST("OUTSW", "DS:[SI]", 0x3E, 0x6F);
        ERRT("INSB",  "ES:[DI]", ILLEGAL_SEGMENT, "ES:[DI]", 0x26, 0x6C);
        ERRT("INSB",  "CS:[DI]", ILLEGAL_SEGMENT, "CS:[DI]", 0x2E, 0x6C);
        ERRT("INSW",  "SS:[DI]", ILLEGAL_SEGMENT, "SS:[DI]", 0x36, 0x6D);
        ERRT("INSW",  "DS:[DI]", ILLEGAL_SEGMENT, "DS:[DI]", 0x3E, 0x6D);
    }

    if (v30()) {
        TEST("ADD4S", "ES:[DI], ES:[SI]", 0x26, 0x0F, 0x20);
        TEST("CMP4S", "ES:[DI], CS:[SI]", 0x2E, 0x0F, 0x26);
        TEST("SUB4S", "ES:[DI], SS:[SI]", 0x36, 0x0F, 0x22);
    }
}

static void test_float() {
    TEST("FINIT", "", 0x9B, 0xDB, 0xE3);

    TEST("FLDCW", "[SI]",          0x9B, 0xD9, 0054);
    TEST("FLDCW", "[1234H]",       0x9B, 0xD9, 0056, 0x34, 0x12);
    TEST("FLDCW", "[DI+52]",       0x9B, 0xD9, 0155, 0x34);
    TEST("FLDCW", "[BP+1234H]",    0x9B, 0xD9, 0256, 0x34, 0x12);
    TEST("FLDCW", "[BX+SI]",       0x9B, 0xD9, 0050);
    TEST("FLDCW", "[BX+DI-52]",    0x9B, 0xD9, 0151, 0xCC);
    TEST("FLDCW", "[BP+SI+89ABH]", 0x9B, 0xD9, 0252, 0xAB, 0x89);

    TEST("FSTCW", "[SI]",          0x9B, 0xD9, 0074);
    TEST("FSTCW", "[1234H]",       0x9B, 0xD9, 0076, 0x34, 0x12);
    TEST("FSTCW", "[DI+52]",       0x9B, 0xD9, 0175, 0x34);
    TEST("FSTCW", "[BP+1234H]",    0x9B, 0xD9, 0276, 0x34, 0x12);
    TEST("FSTCW", "[BX+SI]",       0x9B, 0xD9, 0070);
    TEST("FSTCW", "[BX+DI-52]",    0x9B, 0xD9, 0171, 0xCC);
    TEST("FSTCW", "[BP+SI+89ABH]", 0x9B, 0xD9, 0272, 0xAB, 0x89);

    TEST("FLDENV", "[SI]",          0x9B, 0xD9, 0044);
    TEST("FLDENV", "[1234H]",       0x9B, 0xD9, 0046, 0x34, 0x12);
    TEST("FLDENV", "[DI+52]",       0x9B, 0xD9, 0145, 0x34);
    TEST("FLDENV", "[BP+1234H]",    0x9B, 0xD9, 0246, 0x34, 0x12);
    TEST("FLDENV", "[BX+SI]",       0x9B, 0xD9, 0040);
    TEST("FLDENV", "[BX+DI-52]",    0x9B, 0xD9, 0141, 0xCC);
    TEST("FLDENV", "[BP+SI+89ABH]", 0x9B, 0xD9, 0242, 0xAB, 0x89);

    TEST("FSTSW", "[SI]",          0x9B, 0xDD, 0074);
    TEST("FSTSW", "[1234H]",       0x9B, 0xDD, 0076, 0x34, 0x12);
    TEST("FSTSW", "[DI+52]",       0x9B, 0xDD, 0175, 0x34);
    TEST("FSTSW", "[BP+1234H]",    0x9B, 0xDD, 0276, 0x34, 0x12);
    TEST("FSTSW", "[BX+SI]",       0x9B, 0xDD, 0070);
    TEST("FSTSW", "[BX+DI-52]",    0x9B, 0xDD, 0171, 0xCC);
    TEST("FSTSW", "[BP+SI+89ABH]", 0x9B, 0xDD, 0272, 0xAB, 0x89);

    TEST("FCLEX", "", 0x9B, 0xDB, 0xE2);

    TEST("FSTENV", "[SI]",          0x9B, 0xD9, 0064);
    TEST("FSTENV", "[1234H]",       0x9B, 0xD9, 0066, 0x34, 0x12);
    TEST("FSTENV", "[DI+52]",       0x9B, 0xD9, 0165, 0x34);
    TEST("FSTENV", "[BP+1234H]",    0x9B, 0xD9, 0266, 0x34, 0x12);
    TEST("FSTENV", "[BX+SI]",       0x9B, 0xD9, 0060);
    TEST("FSTENV", "[BX+DI-52]",    0x9B, 0xD9, 0161, 0xCC);
    TEST("FSTENV", "[BP+SI+89ABH]", 0x9B, 0xD9, 0262, 0xAB, 0x89);

    TEST("FSAVE", "[SI]",          0x9B, 0xDD, 0064);
    TEST("FSAVE", "[1234H]",       0x9B, 0xDD, 0066, 0x34, 0x12);
    TEST("FSAVE", "[DI+52]",       0x9B, 0xDD, 0165, 0x34);
    TEST("FSAVE", "[BP+1234H]",    0x9B, 0xDD, 0266, 0x34, 0x12);
    TEST("FSAVE", "[BX+SI]",       0x9B, 0xDD, 0060);
    TEST("FSAVE", "[BX+DI-52]",    0x9B, 0xDD, 0161, 0xCC);
    TEST("FSAVE", "[BP+SI+89ABH]", 0x9B, 0xDD, 0262, 0xAB, 0x89);

    TEST("FRSTOR", "[SI]",          0x9B, 0xDD, 0044);
    TEST("FRSTOR", "[1234H]",       0x9B, 0xDD, 0046, 0x34, 0x12);
    TEST("FRSTOR", "[DI+52]",       0x9B, 0xDD, 0145, 0x34);
    TEST("FRSTOR", "[BP+1234H]",    0x9B, 0xDD, 0246, 0x34, 0x12);
    TEST("FRSTOR", "[BX+SI]",       0x9B, 0xDD, 0040);
    TEST("FRSTOR", "[BX+DI-52]",    0x9B, 0xDD, 0141, 0xCC);
    TEST("FRSTOR", "[BP+SI+89ABH]", 0x9B, 0xDD, 0242, 0xAB, 0x89);

    TEST("FENI", "", 0x9B, 0xDB, 0xE0);
    TEST("FDISI", "", 0x9B, 0xDB, 0xE1);

    TEST("FLD", "DWORD PTR [SI]",          0x9B, 0xD9, 0004);
    TEST("FLD", "DWORD PTR [1234H]",       0x9B, 0xD9, 0006, 0x34, 0x12);
    TEST("FLD", "DWORD PTR [DI+52]",       0x9B, 0xD9, 0105, 0x34);
    TEST("FLD", "DWORD PTR [BP+1234H]",    0x9B, 0xD9, 0206, 0x34, 0x12);
    TEST("FLD", "DWORD PTR [BX+SI]",       0x9B, 0xD9, 0000);
    TEST("FLD", "DWORD PTR [BX+DI-52]",    0x9B, 0xD9, 0101, 0xCC);
    TEST("FLD", "DWORD PTR [BP+SI+89ABH]", 0x9B, 0xD9, 0202, 0xAB, 0x89);

    TEST("FLD", "QWORD PTR [SI]",          0x9B, 0xDD, 0004);
    TEST("FLD", "QWORD PTR [1234H]",       0x9B, 0xDD, 0006, 0x34, 0x12);
    TEST("FLD", "QWORD PTR [DI+52]",       0x9B, 0xDD, 0105, 0x34);
    TEST("FLD", "QWORD PTR [BP+1234H]",    0x9B, 0xDD, 0206, 0x34, 0x12);
    TEST("FLD", "QWORD PTR [BX+SI]",       0x9B, 0xDD, 0000);
    TEST("FLD", "QWORD PTR [BX+DI-52]",    0x9B, 0xDD, 0101, 0xCC);
    TEST("FLD", "QWORD PTR [BP+SI+89ABH]", 0x9B, 0xDD, 0202, 0xAB, 0x89);

    TEST("FLD", "TBYTE PTR [SI]",          0x9B, 0xDB, 0054);
    TEST("FLD", "TBYTE PTR [1234H]",       0x9B, 0xDB, 0056, 0x34, 0x12);
    TEST("FLD", "TBYTE PTR [DI+52]",       0x9B, 0xDB, 0155, 0x34);
    TEST("FLD", "TBYTE PTR [BP+1234H]",    0x9B, 0xDB, 0256, 0x34, 0x12);
    TEST("FLD", "TBYTE PTR [BX+SI]",       0x9B, 0xDB, 0050);
    TEST("FLD", "TBYTE PTR [BX+DI-52]",    0x9B, 0xDB, 0151, 0xCC);
    TEST("FLD", "TBYTE PTR [BP+SI+89ABH]", 0x9B, 0xDB, 0252, 0xAB, 0x89);

    TEST("FLD", "ST(0)", 0x9B, 0xD9, 0xC0);
    TEST("FLD", "ST(7)", 0x9B, 0xD9, 0xC7);

    TEST("FST", "DWORD PTR [SI]",          0x9B, 0xD9, 0024);
    TEST("FST", "DWORD PTR [1234H]",       0x9B, 0xD9, 0026, 0x34, 0x12);
    TEST("FST", "DWORD PTR [DI+52]",       0x9B, 0xD9, 0125, 0x34);
    TEST("FST", "DWORD PTR [BP+1234H]",    0x9B, 0xD9, 0226, 0x34, 0x12);
    TEST("FST", "DWORD PTR [BX+SI]",       0x9B, 0xD9, 0020);
    TEST("FST", "DWORD PTR [BX+DI-52]",    0x9B, 0xD9, 0121, 0xCC);
    TEST("FST", "DWORD PTR [BP+SI+89ABH]", 0x9B, 0xD9, 0222, 0xAB, 0x89);

    TEST("FST", "QWORD PTR [SI]",          0x9B, 0xDD, 0024);
    TEST("FST", "QWORD PTR [1234H]",       0x9B, 0xDD, 0026, 0x34, 0x12);
    TEST("FST", "QWORD PTR [DI+52]",       0x9B, 0xDD, 0125, 0x34);
    TEST("FST", "QWORD PTR [BP+1234H]",    0x9B, 0xDD, 0226, 0x34, 0x12);
    TEST("FST", "QWORD PTR [BX+SI]",       0x9B, 0xDD, 0020);
    TEST("FST", "QWORD PTR [BX+DI-52]",    0x9B, 0xDD, 0121, 0xCC);
    TEST("FST", "QWORD PTR [BP+SI+89ABH]", 0x9B, 0xDD, 0222, 0xAB, 0x89);

    TEST("FST", "ST(0)", 0x9B, 0xDD, 0xD0);
    TEST("FST", "ST(7)", 0x9B, 0xDD, 0xD7);

    TEST("FSTP", "DWORD PTR [SI]",          0x9B, 0xD9, 0034);
    TEST("FSTP", "DWORD PTR [1234H]",       0x9B, 0xD9, 0036, 0x34, 0x12);
    TEST("FSTP", "DWORD PTR [DI+52]",       0x9B, 0xD9, 0135, 0x34);
    TEST("FSTP", "DWORD PTR [BP+1234H]",    0x9B, 0xD9, 0236, 0x34, 0x12);
    TEST("FSTP", "DWORD PTR [BX+SI]",       0x9B, 0xD9, 0030);
    TEST("FSTP", "DWORD PTR [BX+DI-52]",    0x9B, 0xD9, 0131, 0xCC);
    TEST("FSTP", "DWORD PTR [BP+SI+89ABH]", 0x9B, 0xD9, 0232, 0xAB, 0x89);

    TEST("FSTP", "QWORD PTR [SI]",          0x9B, 0xDD, 0034);
    TEST("FSTP", "QWORD PTR [1234H]",       0x9B, 0xDD, 0036, 0x34, 0x12);
    TEST("FSTP", "QWORD PTR [DI+52]",       0x9B, 0xDD, 0135, 0x34);
    TEST("FSTP", "QWORD PTR [BP+1234H]",    0x9B, 0xDD, 0236, 0x34, 0x12);
    TEST("FSTP", "QWORD PTR [BX+SI]",       0x9B, 0xDD, 0030);
    TEST("FSTP", "QWORD PTR [BX+DI-52]",    0x9B, 0xDD, 0131, 0xCC);
    TEST("FSTP", "QWORD PTR [BP+SI+89ABH]", 0x9B, 0xDD, 0232, 0xAB, 0x89);

    TEST("FSTP", "TBYTE PTR [SI]",          0x9B, 0xDB, 0074);
    TEST("FSTP", "TBYTE PTR [1234H]",       0x9B, 0xDB, 0076, 0x34, 0x12);
    TEST("FSTP", "TBYTE PTR [DI+52]",       0x9B, 0xDB, 0175, 0x34);
    TEST("FSTP", "TBYTE PTR [BP+1234H]",    0x9B, 0xDB, 0276, 0x34, 0x12);
    TEST("FSTP", "TBYTE PTR [BX+SI]",       0x9B, 0xDB, 0070);
    TEST("FSTP", "TBYTE PTR [BX+DI-52]",    0x9B, 0xDB, 0171, 0xCC);
    TEST("FSTP", "TBYTE PTR [BP+SI+89ABH]", 0x9B, 0xDB, 0272, 0xAB, 0x89);

    TEST("FSTP", "ST(0)", 0x9B, 0xDD, 0xD8);
    TEST("FSTP", "ST(7)", 0x9B, 0xDD, 0xDF);

    TEST("FLDZ", "", 0x9B, 0xD9, 0xEE);
    TEST("FLD1", "", 0x9B, 0xD9, 0xE8);
    TEST("FLDPI", "", 0x9B, 0xD9, 0xEB);
    TEST("FLDL2T", "", 0x9B, 0xD9, 0xE9);
    TEST("FLDL2E", "", 0x9B, 0xD9, 0xEA);
    TEST("FLDLG2", "", 0x9B, 0xD9, 0xEC);
    TEST("FLDLN2", "", 0x9B, 0xD9, 0xED);

    TEST("FXCH", "ST(0)", 0x9B, 0xD9, 0xC8);
    TEST("FXCH", "ST(7)", 0x9B, 0xD9, 0xCF);

    TEST("FILD", "WORD PTR [SI]",          0x9B, 0xDF, 0004);
    TEST("FILD", "WORD PTR [1234H]",       0x9B, 0xDF, 0006, 0x34, 0x12);
    TEST("FILD", "WORD PTR [DI+52]",       0x9B, 0xDF, 0105, 0x34);
    TEST("FILD", "WORD PTR [BP+1234H]",    0x9B, 0xDF, 0206, 0x34, 0x12);
    TEST("FILD", "WORD PTR [BX+SI]",       0x9B, 0xDF, 0000);
    TEST("FILD", "WORD PTR [BX+DI-52]",    0x9B, 0xDF, 0101, 0xCC);
    TEST("FILD", "WORD PTR [BP+SI+89ABH]", 0x9B, 0xDF, 0202, 0xAB, 0x89);

    TEST("FILD", "DWORD PTR [SI]",          0x9B, 0xDB, 0004);
    TEST("FILD", "DWORD PTR [1234H]",       0x9B, 0xDB, 0006, 0x34, 0x12);
    TEST("FILD", "DWORD PTR [DI+52]",       0x9B, 0xDB, 0105, 0x34);
    TEST("FILD", "DWORD PTR [BP+1234H]",    0x9B, 0xDB, 0206, 0x34, 0x12);
    TEST("FILD", "DWORD PTR [BX+SI]",       0x9B, 0xDB, 0000);
    TEST("FILD", "DWORD PTR [BX+DI-52]",    0x9B, 0xDB, 0101, 0xCC);
    TEST("FILD", "DWORD PTR [BP+SI+89ABH]", 0x9B, 0xDB, 0202, 0xAB, 0x89);

    TEST("FILD", "QWORD PTR [SI]",          0x9B, 0xDF, 0054);
    TEST("FILD", "QWORD PTR [1234H]",       0x9B, 0xDF, 0056, 0x34, 0x12);
    TEST("FILD", "QWORD PTR [DI+52]",       0x9B, 0xDF, 0155, 0x34);
    TEST("FILD", "QWORD PTR [BP+1234H]",    0x9B, 0xDF, 0256, 0x34, 0x12);
    TEST("FILD", "QWORD PTR [BX+SI]",       0x9B, 0xDF, 0050);
    TEST("FILD", "QWORD PTR [BX+DI-52]",    0x9B, 0xDF, 0151, 0xCC);
    TEST("FILD", "QWORD PTR [BP+SI+89ABH]", 0x9B, 0xDF, 0252, 0xAB, 0x89);

    TEST("FIST", "WORD PTR [SI]",          0x9B, 0xDF, 0024);
    TEST("FIST", "WORD PTR [1234H]",       0x9B, 0xDF, 0026, 0x34, 0x12);
    TEST("FIST", "WORD PTR [DI+52]",       0x9B, 0xDF, 0125, 0x34);
    TEST("FIST", "WORD PTR [BP+1234H]",    0x9B, 0xDF, 0226, 0x34, 0x12);
    TEST("FIST", "WORD PTR [BX+SI]",       0x9B, 0xDF, 0020);
    TEST("FIST", "WORD PTR [BX+DI-52]",    0x9B, 0xDF, 0121, 0xCC);
    TEST("FIST", "WORD PTR [BP+SI+89ABH]", 0x9B, 0xDF, 0222, 0xAB, 0x89);

    TEST("FIST", "DWORD PTR [SI]",          0x9B, 0xDB, 0024);
    TEST("FIST", "DWORD PTR [1234H]",       0x9B, 0xDB, 0026, 0x34, 0x12);
    TEST("FIST", "DWORD PTR [DI+52]",       0x9B, 0xDB, 0125, 0x34);
    TEST("FIST", "DWORD PTR [BP+1234H]",    0x9B, 0xDB, 0226, 0x34, 0x12);
    TEST("FIST", "DWORD PTR [BX+SI]",       0x9B, 0xDB, 0020);
    TEST("FIST", "DWORD PTR [BX+DI-52]",    0x9B, 0xDB, 0121, 0xCC);
    TEST("FIST", "DWORD PTR [BP+SI+89ABH]", 0x9B, 0xDB, 0222, 0xAB, 0x89);

    TEST("FISTP", "WORD PTR [SI]",          0x9B, 0xDF, 0034);
    TEST("FISTP", "WORD PTR [1234H]",       0x9B, 0xDF, 0036, 0x34, 0x12);
    TEST("FISTP", "WORD PTR [DI+52]",       0x9B, 0xDF, 0135, 0x34);
    TEST("FISTP", "WORD PTR [BP+1234H]",    0x9B, 0xDF, 0236, 0x34, 0x12);
    TEST("FISTP", "WORD PTR [BX+SI]",       0x9B, 0xDF, 0030);
    TEST("FISTP", "WORD PTR [BX+DI-52]",    0x9B, 0xDF, 0131, 0xCC);
    TEST("FISTP", "WORD PTR [BP+SI+89ABH]", 0x9B, 0xDF, 0232, 0xAB, 0x89);

    TEST("FISTP", "DWORD PTR [SI]",          0x9B, 0xDB, 0034);
    TEST("FISTP", "DWORD PTR [1234H]",       0x9B, 0xDB, 0036, 0x34, 0x12);
    TEST("FISTP", "DWORD PTR [DI+52]",       0x9B, 0xDB, 0135, 0x34);
    TEST("FISTP", "DWORD PTR [BP+1234H]",    0x9B, 0xDB, 0236, 0x34, 0x12);
    TEST("FISTP", "DWORD PTR [BX+SI]",       0x9B, 0xDB, 0030);
    TEST("FISTP", "DWORD PTR [BX+DI-52]",    0x9B, 0xDB, 0131, 0xCC);
    TEST("FISTP", "DWORD PTR [BP+SI+89ABH]", 0x9B, 0xDB, 0232, 0xAB, 0x89);

    TEST("FISTP", "QWORD PTR [SI]",          0x9B, 0xDF, 0074);
    TEST("FISTP", "QWORD PTR [1234H]",       0x9B, 0xDF, 0076, 0x34, 0x12);
    TEST("FISTP", "QWORD PTR [DI+52]",       0x9B, 0xDF, 0175, 0x34);
    TEST("FISTP", "QWORD PTR [BP+1234H]",    0x9B, 0xDF, 0276, 0x34, 0x12);
    TEST("FISTP", "QWORD PTR [BX+SI]",       0x9B, 0xDF, 0070);
    TEST("FISTP", "QWORD PTR [BX+DI-52]",    0x9B, 0xDF, 0171, 0xCC);
    TEST("FISTP", "QWORD PTR [BP+SI+89ABH]", 0x9B, 0xDF, 0272, 0xAB, 0x89);

    TEST("FBLD", "TBYTE PTR [SI]",          0x9B, 0xDF, 0044);
    TEST("FBLD", "TBYTE PTR [1234H]",       0x9B, 0xDF, 0046, 0x34, 0x12);
    TEST("FBLD", "TBYTE PTR [DI+52]",       0x9B, 0xDF, 0145, 0x34);
    TEST("FBLD", "TBYTE PTR [BP+1234H]",    0x9B, 0xDF, 0246, 0x34, 0x12);
    TEST("FBLD", "TBYTE PTR [BX+SI]",       0x9B, 0xDF, 0040);
    TEST("FBLD", "TBYTE PTR [BX+DI-52]",    0x9B, 0xDF, 0141, 0xCC);
    TEST("FBLD", "TBYTE PTR [BP+SI+89ABH]", 0x9B, 0xDF, 0242, 0xAB, 0x89);

    TEST("FBSTP", "TBYTE PTR [SI]",          0x9B, 0xDF, 0064);
    TEST("FBSTP", "TBYTE PTR [1234H]",       0x9B, 0xDF, 0066, 0x34, 0x12);
    TEST("FBSTP", "TBYTE PTR [DI+52]",       0x9B, 0xDF, 0165, 0x34);
    TEST("FBSTP", "TBYTE PTR [BP+1234H]",    0x9B, 0xDF, 0266, 0x34, 0x12);
    TEST("FBSTP", "TBYTE PTR [BX+SI]",       0x9B, 0xDF, 0060);
    TEST("FBSTP", "TBYTE PTR [BX+DI-52]",    0x9B, 0xDF, 0161, 0xCC);
    TEST("FBSTP", "TBYTE PTR [BP+SI+89ABH]", 0x9B, 0xDF, 0262, 0xAB, 0x89);

    TEST("FADD", "DWORD PTR [SI]",          0x9B, 0xD8, 0004);
    TEST("FADD", "DWORD PTR [1234H]",       0x9B, 0xD8, 0006, 0x34, 0x12);
    TEST("FADD", "DWORD PTR [DI+52]",       0x9B, 0xD8, 0105, 0x34);
    TEST("FADD", "DWORD PTR [BP+1234H]",    0x9B, 0xD8, 0206, 0x34, 0x12);
    TEST("FADD", "DWORD PTR [BX+SI]",       0x9B, 0xD8, 0000);
    TEST("FADD", "DWORD PTR [BX+DI-52]",    0x9B, 0xD8, 0101, 0xCC);
    TEST("FADD", "DWORD PTR [BP+SI+89ABH]", 0x9B, 0xD8, 0202, 0xAB, 0x89);

    TEST("FADD", "QWORD PTR [SI]",          0x9B, 0xDC, 0004);
    TEST("FADD", "QWORD PTR [1234H]",       0x9B, 0xDC, 0006, 0x34, 0x12);
    TEST("FADD", "QWORD PTR [DI+52]",       0x9B, 0xDC, 0105, 0x34);
    TEST("FADD", "QWORD PTR [BP+1234H]",    0x9B, 0xDC, 0206, 0x34, 0x12);
    TEST("FADD", "QWORD PTR [BX+SI]",       0x9B, 0xDC, 0000);
    TEST("FADD", "QWORD PTR [BX+DI-52]",    0x9B, 0xDC, 0101, 0xCC);
    TEST("FADD", "QWORD PTR [BP+SI+89ABH]", 0x9B, 0xDC, 0202, 0xAB, 0x89);

    TEST("FADD", "ST, ST(1)",  0x9B, 0xD8, 0xC1);
    TEST("FADD", "ST(2), ST",  0x9B, 0xDC, 0xC2);
    TEST("FADDP", "ST(3), ST", 0x9B, 0xDE, 0xC3);

    TEST("FMUL", "DWORD PTR [SI]",          0x9B, 0xD8, 0014);
    TEST("FMUL", "DWORD PTR [1234H]",       0x9B, 0xD8, 0016, 0x34, 0x12);
    TEST("FMUL", "DWORD PTR [DI+52]",       0x9B, 0xD8, 0115, 0x34);
    TEST("FMUL", "DWORD PTR [BP+1234H]",    0x9B, 0xD8, 0216, 0x34, 0x12);
    TEST("FMUL", "DWORD PTR [BX+SI]",       0x9B, 0xD8, 0010);
    TEST("FMUL", "DWORD PTR [BX+DI-52]",    0x9B, 0xD8, 0111, 0xCC);
    TEST("FMUL", "DWORD PTR [BP+SI+89ABH]", 0x9B, 0xD8, 0212, 0xAB, 0x89);

    TEST("FMUL", "QWORD PTR [SI]",          0x9B, 0xDC, 0014);
    TEST("FMUL", "QWORD PTR [1234H]",       0x9B, 0xDC, 0016, 0x34, 0x12);
    TEST("FMUL", "QWORD PTR [DI+52]",       0x9B, 0xDC, 0115, 0x34);
    TEST("FMUL", "QWORD PTR [BP+1234H]",    0x9B, 0xDC, 0216, 0x34, 0x12);
    TEST("FMUL", "QWORD PTR [BX+SI]",       0x9B, 0xDC, 0010);
    TEST("FMUL", "QWORD PTR [BX+DI-52]",    0x9B, 0xDC, 0111, 0xCC);
    TEST("FMUL", "QWORD PTR [BP+SI+89ABH]", 0x9B, 0xDC, 0212, 0xAB, 0x89);

    TEST("FMUL", "ST, ST(4)",  0x9B, 0xD8, 0xCC);
    TEST("FMUL", "ST(5), ST",  0x9B, 0xDC, 0xCD);
    TEST("FMULP", "ST(6), ST", 0x9B, 0xDE, 0xCE);

    TEST("FSUB", "DWORD PTR [SI]",          0x9B, 0xD8, 0044);
    TEST("FSUB", "DWORD PTR [1234H]",       0x9B, 0xD8, 0046, 0x34, 0x12);
    TEST("FSUB", "DWORD PTR [DI+52]",       0x9B, 0xD8, 0145, 0x34);
    TEST("FSUB", "DWORD PTR [BP+1234H]",    0x9B, 0xD8, 0246, 0x34, 0x12);
    TEST("FSUB", "DWORD PTR [BX+SI]",       0x9B, 0xD8, 0040);
    TEST("FSUB", "DWORD PTR [BX+DI-52]",    0x9B, 0xD8, 0141, 0xCC);
    TEST("FSUB", "DWORD PTR [BP+SI+89ABH]", 0x9B, 0xD8, 0242, 0xAB, 0x89);

    TEST("FSUB", "QWORD PTR [SI]",          0x9B, 0xDC, 0044);
    TEST("FSUB", "QWORD PTR [1234H]",       0x9B, 0xDC, 0046, 0x34, 0x12);
    TEST("FSUB", "QWORD PTR [DI+52]",       0x9B, 0xDC, 0145, 0x34);
    TEST("FSUB", "QWORD PTR [BP+1234H]",    0x9B, 0xDC, 0246, 0x34, 0x12);
    TEST("FSUB", "QWORD PTR [BX+SI]",       0x9B, 0xDC, 0040);
    TEST("FSUB", "QWORD PTR [BX+DI-52]",    0x9B, 0xDC, 0141, 0xCC);
    TEST("FSUB", "QWORD PTR [BP+SI+89ABH]", 0x9B, 0xDC, 0242, 0xAB, 0x89);

    TEST("FSUB", "ST, ST(7)",  0x9B, 0xD8, 0xE7);
    TEST("FSUB", "ST(1), ST",  0x9B, 0xDC, 0xE9);
    TEST("FSUBP", "ST(1), ST", 0x9B, 0xDE, 0xE9);

    TEST("FSUBR", "DWORD PTR [SI]",          0x9B, 0xD8, 0054);
    TEST("FSUBR", "DWORD PTR [1234H]",       0x9B, 0xD8, 0056, 0x34, 0x12);
    TEST("FSUBR", "DWORD PTR [DI+52]",       0x9B, 0xD8, 0155, 0x34);
    TEST("FSUBR", "DWORD PTR [BP+1234H]",    0x9B, 0xD8, 0256, 0x34, 0x12);
    TEST("FSUBR", "DWORD PTR [BX+SI]",       0x9B, 0xD8, 0050);
    TEST("FSUBR", "DWORD PTR [BX+DI-52]",    0x9B, 0xD8, 0151, 0xCC);
    TEST("FSUBR", "DWORD PTR [BP+SI+89ABH]", 0x9B, 0xD8, 0252, 0xAB, 0x89);

    TEST("FSUBR", "QWORD PTR [SI]",          0x9B, 0xDC, 0054);
    TEST("FSUBR", "QWORD PTR [1234H]",       0x9B, 0xDC, 0056, 0x34, 0x12);
    TEST("FSUBR", "QWORD PTR [DI+52]",       0x9B, 0xDC, 0155, 0x34);
    TEST("FSUBR", "QWORD PTR [BP+1234H]",    0x9B, 0xDC, 0256, 0x34, 0x12);
    TEST("FSUBR", "QWORD PTR [BX+SI]",       0x9B, 0xDC, 0050);
    TEST("FSUBR", "QWORD PTR [BX+DI-52]",    0x9B, 0xDC, 0151, 0xCC);
    TEST("FSUBR", "QWORD PTR [BP+SI+89ABH]", 0x9B, 0xDC, 0252, 0xAB, 0x89);

    TEST("FSUBR", "ST, ST(2)",  0x9B, 0xD8, 0xEA);
    TEST("FSUBR", "ST(3), ST",  0x9B, 0xDC, 0xE3);
    TEST("FSUBRP", "ST(4), ST", 0x9B, 0xDE, 0xE4);

    TEST("FDIV", "DWORD PTR [SI]",          0x9B, 0xD8, 0064);
    TEST("FDIV", "DWORD PTR [1234H]",       0x9B, 0xD8, 0066, 0x34, 0x12);
    TEST("FDIV", "DWORD PTR [DI+52]",       0x9B, 0xD8, 0165, 0x34);
    TEST("FDIV", "DWORD PTR [BP+1234H]",    0x9B, 0xD8, 0266, 0x34, 0x12);
    TEST("FDIV", "DWORD PTR [BX+SI]",       0x9B, 0xD8, 0060);
    TEST("FDIV", "DWORD PTR [BX+DI-52]",    0x9B, 0xD8, 0161, 0xCC);
    TEST("FDIV", "DWORD PTR [BP+SI+89ABH]", 0x9B, 0xD8, 0262, 0xAB, 0x89);

    TEST("FDIV", "QWORD PTR [SI]",          0x9B, 0xDC, 0064);
    TEST("FDIV", "QWORD PTR [1234H]",       0x9B, 0xDC, 0066, 0x34, 0x12);
    TEST("FDIV", "QWORD PTR [DI+52]",       0x9B, 0xDC, 0165, 0x34);
    TEST("FDIV", "QWORD PTR [BP+1234H]",    0x9B, 0xDC, 0266, 0x34, 0x12);
    TEST("FDIV", "QWORD PTR [BX+SI]",       0x9B, 0xDC, 0060);
    TEST("FDIV", "QWORD PTR [BX+DI-52]",    0x9B, 0xDC, 0161, 0xCC);
    TEST("FDIV", "QWORD PTR [BP+SI+89ABH]", 0x9B, 0xDC, 0262, 0xAB, 0x89);

    TEST("FDIV", "ST, ST(7)",  0x9B, 0xD8, 0xF7);
    TEST("FDIV", "ST(1), ST",  0x9B, 0xDC, 0xF9);
    TEST("FDIVP", "ST(1), ST", 0x9B, 0xDE, 0xF9);

    TEST("FDIVR", "DWORD PTR [SI]",          0x9B, 0xD8, 0074);
    TEST("FDIVR", "DWORD PTR [1234H]",       0x9B, 0xD8, 0076, 0x34, 0x12);
    TEST("FDIVR", "DWORD PTR [DI+52]",       0x9B, 0xD8, 0175, 0x34);
    TEST("FDIVR", "DWORD PTR [BP+1234H]",    0x9B, 0xD8, 0276, 0x34, 0x12);
    TEST("FDIVR", "DWORD PTR [BX+SI]",       0x9B, 0xD8, 0070);
    TEST("FDIVR", "DWORD PTR [BX+DI-52]",    0x9B, 0xD8, 0171, 0xCC);
    TEST("FDIVR", "DWORD PTR [BP+SI+89ABH]", 0x9B, 0xD8, 0272, 0xAB, 0x89);

    TEST("FDIVR", "QWORD PTR [SI]",          0x9B, 0xDC, 0074);
    TEST("FDIVR", "QWORD PTR [1234H]",       0x9B, 0xDC, 0076, 0x34, 0x12);
    TEST("FDIVR", "QWORD PTR [DI+52]",       0x9B, 0xDC, 0175, 0x34);
    TEST("FDIVR", "QWORD PTR [BP+1234H]",    0x9B, 0xDC, 0276, 0x34, 0x12);
    TEST("FDIVR", "QWORD PTR [BX+SI]",       0x9B, 0xDC, 0070);
    TEST("FDIVR", "QWORD PTR [BX+DI-52]",    0x9B, 0xDC, 0171, 0xCC);
    TEST("FDIVR", "QWORD PTR [BP+SI+89ABH]", 0x9B, 0xDC, 0272, 0xAB, 0x89);

    TEST("FDIVR", "ST, ST(2)",  0x9B, 0xD8, 0xFA);
    TEST("FDIVR", "ST(3), ST",  0x9B, 0xDC, 0xF3);
    TEST("FDIVRP", "ST(4), ST", 0x9B, 0xDE, 0xF4);

    TEST("FCOM", "DWORD PTR [SI]",          0x9B, 0xD8, 0024);
    TEST("FCOM", "DWORD PTR [1234H]",       0x9B, 0xD8, 0026, 0x34, 0x12);
    TEST("FCOM", "DWORD PTR [DI+52]",       0x9B, 0xD8, 0125, 0x34);
    TEST("FCOM", "DWORD PTR [BP+1234H]",    0x9B, 0xD8, 0226, 0x34, 0x12);
    TEST("FCOM", "DWORD PTR [BX+SI]",       0x9B, 0xD8, 0020);
    TEST("FCOM", "DWORD PTR [BX+DI-52]",    0x9B, 0xD8, 0121, 0xCC);
    TEST("FCOM", "DWORD PTR [BP+SI+89ABH]", 0x9B, 0xD8, 0222, 0xAB, 0x89);

    TEST("FCOM", "QWORD PTR [SI]",          0x9B, 0xDC, 0024);
    TEST("FCOM", "QWORD PTR [1234H]",       0x9B, 0xDC, 0026, 0x34, 0x12);
    TEST("FCOM", "QWORD PTR [DI+52]",       0x9B, 0xDC, 0125, 0x34);
    TEST("FCOM", "QWORD PTR [BP+1234H]",    0x9B, 0xDC, 0226, 0x34, 0x12);
    TEST("FCOM", "QWORD PTR [BX+SI]",       0x9B, 0xDC, 0020);
    TEST("FCOM", "QWORD PTR [BX+DI-52]",    0x9B, 0xDC, 0121, 0xCC);
    TEST("FCOM", "QWORD PTR [BP+SI+89ABH]", 0x9B, 0xDC, 0222, 0xAB, 0x89);

    TEST("FCOM", "ST(2)", 0x9B, 0xD8, 0xD2);

    TEST("FCOMP", "DWORD PTR [SI]",          0x9B, 0xD8, 0034);
    TEST("FCOMP", "DWORD PTR [1234H]",       0x9B, 0xD8, 0036, 0x34, 0x12);
    TEST("FCOMP", "DWORD PTR [DI+52]",       0x9B, 0xD8, 0135, 0x34);
    TEST("FCOMP", "DWORD PTR [BP+1234H]",    0x9B, 0xD8, 0236, 0x34, 0x12);
    TEST("FCOMP", "DWORD PTR [BX+SI]",       0x9B, 0xD8, 0030);
    TEST("FCOMP", "DWORD PTR [BX+DI-52]",    0x9B, 0xD8, 0131, 0xCC);
    TEST("FCOMP", "DWORD PTR [BP+SI+89ABH]", 0x9B, 0xD8, 0232, 0xAB, 0x89);

    TEST("FCOMP", "QWORD PTR [SI]",          0x9B, 0xDC, 0034);
    TEST("FCOMP", "QWORD PTR [1234H]",       0x9B, 0xDC, 0036, 0x34, 0x12);
    TEST("FCOMP", "QWORD PTR [DI+52]",       0x9B, 0xDC, 0135, 0x34);
    TEST("FCOMP", "QWORD PTR [BP+1234H]",    0x9B, 0xDC, 0236, 0x34, 0x12);
    TEST("FCOMP", "QWORD PTR [BX+SI]",       0x9B, 0xDC, 0030);
    TEST("FCOMP", "QWORD PTR [BX+DI-52]",    0x9B, 0xDC, 0131, 0xCC);
    TEST("FCOMP", "QWORD PTR [BP+SI+89ABH]", 0x9B, 0xDC, 0232, 0xAB, 0x89);

    TEST("FCOMP", "ST(2)", 0x9B, 0xD8, 0xDA);
    TEST("FCOMPP", "",     0x9B, 0xDE, 0xD9);

    TEST("FIADD", "WORD PTR [SI]",          0x9B, 0xDE, 0004);
    TEST("FIADD", "WORD PTR [1234H]",       0x9B, 0xDE, 0006, 0x34, 0x12);
    TEST("FIADD", "WORD PTR [DI+52]",       0x9B, 0xDE, 0105, 0x34);
    TEST("FIADD", "WORD PTR [BP+1234H]",    0x9B, 0xDE, 0206, 0x34, 0x12);
    TEST("FIADD", "WORD PTR [BX+SI]",       0x9B, 0xDE, 0000);
    TEST("FIADD", "WORD PTR [BX+DI-52]",    0x9B, 0xDE, 0101, 0xCC);
    TEST("FIADD", "WORD PTR [BP+SI+89ABH]", 0x9B, 0xDE, 0202, 0xAB, 0x89);

    TEST("FIADD", "DWORD PTR [SI]",          0x9B, 0xDA, 0004);
    TEST("FIADD", "DWORD PTR [1234H]",       0x9B, 0xDA, 0006, 0x34, 0x12);
    TEST("FIADD", "DWORD PTR [DI+52]",       0x9B, 0xDA, 0105, 0x34);
    TEST("FIADD", "DWORD PTR [BP+1234H]",    0x9B, 0xDA, 0206, 0x34, 0x12);
    TEST("FIADD", "DWORD PTR [BX+SI]",       0x9B, 0xDA, 0000);
    TEST("FIADD", "DWORD PTR [BX+DI-52]",    0x9B, 0xDA, 0101, 0xCC);
    TEST("FIADD", "DWORD PTR [BP+SI+89ABH]", 0x9B, 0xDA, 0202, 0xAB, 0x89);

    TEST("FIMUL", "WORD PTR [SI]",          0x9B, 0xDE, 0014);
    TEST("FIMUL", "WORD PTR [1234H]",       0x9B, 0xDE, 0016, 0x34, 0x12);
    TEST("FIMUL", "WORD PTR [DI+52]",       0x9B, 0xDE, 0115, 0x34);
    TEST("FIMUL", "WORD PTR [BP+1234H]",    0x9B, 0xDE, 0216, 0x34, 0x12);
    TEST("FIMUL", "WORD PTR [BX+SI]",       0x9B, 0xDE, 0010);
    TEST("FIMUL", "WORD PTR [BX+DI-52]",    0x9B, 0xDE, 0111, 0xCC);
    TEST("FIMUL", "WORD PTR [BP+SI+89ABH]", 0x9B, 0xDE, 0212, 0xAB, 0x89);

    TEST("FIMUL", "DWORD PTR [SI]",          0x9B, 0xDA, 0014);
    TEST("FIMUL", "DWORD PTR [1234H]",       0x9B, 0xDA, 0016, 0x34, 0x12);
    TEST("FIMUL", "DWORD PTR [DI+52]",       0x9B, 0xDA, 0115, 0x34);
    TEST("FIMUL", "DWORD PTR [BP+1234H]",    0x9B, 0xDA, 0216, 0x34, 0x12);
    TEST("FIMUL", "DWORD PTR [BX+SI]",       0x9B, 0xDA, 0010);
    TEST("FIMUL", "DWORD PTR [BX+DI-52]",    0x9B, 0xDA, 0111, 0xCC);
    TEST("FIMUL", "DWORD PTR [BP+SI+89ABH]", 0x9B, 0xDA, 0212, 0xAB, 0x89);

    TEST("FISUB", "WORD PTR [SI]",          0x9B, 0xDE, 0044);
    TEST("FISUB", "WORD PTR [1234H]",       0x9B, 0xDE, 0046, 0x34, 0x12);
    TEST("FISUB", "WORD PTR [DI+52]",       0x9B, 0xDE, 0145, 0x34);
    TEST("FISUB", "WORD PTR [BP+1234H]",    0x9B, 0xDE, 0246, 0x34, 0x12);
    TEST("FISUB", "WORD PTR [BX+SI]",       0x9B, 0xDE, 0040);
    TEST("FISUB", "WORD PTR [BX+DI-52]",    0x9B, 0xDE, 0141, 0xCC);
    TEST("FISUB", "WORD PTR [BP+SI+89ABH]", 0x9B, 0xDE, 0242, 0xAB, 0x89);

    TEST("FISUB", "DWORD PTR [SI]",          0x9B, 0xDA, 0044);
    TEST("FISUB", "DWORD PTR [1234H]",       0x9B, 0xDA, 0046, 0x34, 0x12);
    TEST("FISUB", "DWORD PTR [DI+52]",       0x9B, 0xDA, 0145, 0x34);
    TEST("FISUB", "DWORD PTR [BP+1234H]",    0x9B, 0xDA, 0246, 0x34, 0x12);
    TEST("FISUB", "DWORD PTR [BX+SI]",       0x9B, 0xDA, 0040);
    TEST("FISUB", "DWORD PTR [BX+DI-52]",    0x9B, 0xDA, 0141, 0xCC);
    TEST("FISUB", "DWORD PTR [BP+SI+89ABH]", 0x9B, 0xDA, 0242, 0xAB, 0x89);

    TEST("FISUBR", "WORD PTR [SI]",          0x9B, 0xDE, 0054);
    TEST("FISUBR", "WORD PTR [1234H]",       0x9B, 0xDE, 0056, 0x34, 0x12);
    TEST("FISUBR", "WORD PTR [DI+52]",       0x9B, 0xDE, 0155, 0x34);
    TEST("FISUBR", "WORD PTR [BP+1234H]",    0x9B, 0xDE, 0256, 0x34, 0x12);
    TEST("FISUBR", "WORD PTR [BX+SI]",       0x9B, 0xDE, 0050);
    TEST("FISUBR", "WORD PTR [BX+DI-52]",    0x9B, 0xDE, 0151, 0xCC);
    TEST("FISUBR", "WORD PTR [BP+SI+89ABH]", 0x9B, 0xDE, 0252, 0xAB, 0x89);

    TEST("FISUBR", "DWORD PTR [SI]",          0x9B, 0xDA, 0054);
    TEST("FISUBR", "DWORD PTR [1234H]",       0x9B, 0xDA, 0056, 0x34, 0x12);
    TEST("FISUBR", "DWORD PTR [DI+52]",       0x9B, 0xDA, 0155, 0x34);
    TEST("FISUBR", "DWORD PTR [BP+1234H]",    0x9B, 0xDA, 0256, 0x34, 0x12);
    TEST("FISUBR", "DWORD PTR [BX+SI]",       0x9B, 0xDA, 0050);
    TEST("FISUBR", "DWORD PTR [BX+DI-52]",    0x9B, 0xDA, 0151, 0xCC);
    TEST("FISUBR", "DWORD PTR [BP+SI+89ABH]", 0x9B, 0xDA, 0252, 0xAB, 0x89);

    TEST("FIDIV", "WORD PTR [SI]",          0x9B, 0xDE, 0064);
    TEST("FIDIV", "WORD PTR [1234H]",       0x9B, 0xDE, 0066, 0x34, 0x12);
    TEST("FIDIV", "WORD PTR [DI+52]",       0x9B, 0xDE, 0165, 0x34);
    TEST("FIDIV", "WORD PTR [BP+1234H]",    0x9B, 0xDE, 0266, 0x34, 0x12);
    TEST("FIDIV", "WORD PTR [BX+SI]",       0x9B, 0xDE, 0060);
    TEST("FIDIV", "WORD PTR [BX+DI-52]",    0x9B, 0xDE, 0161, 0xCC);
    TEST("FIDIV", "WORD PTR [BP+SI+89ABH]", 0x9B, 0xDE, 0262, 0xAB, 0x89);

    TEST("FIDIV", "DWORD PTR [SI]",          0x9B, 0xDA, 0064);
    TEST("FIDIV", "DWORD PTR [1234H]",       0x9B, 0xDA, 0066, 0x34, 0x12);
    TEST("FIDIV", "DWORD PTR [DI+52]",       0x9B, 0xDA, 0165, 0x34);
    TEST("FIDIV", "DWORD PTR [BP+1234H]",    0x9B, 0xDA, 0266, 0x34, 0x12);
    TEST("FIDIV", "DWORD PTR [BX+SI]",       0x9B, 0xDA, 0060);
    TEST("FIDIV", "DWORD PTR [BX+DI-52]",    0x9B, 0xDA, 0161, 0xCC);
    TEST("FIDIV", "DWORD PTR [BP+SI+89ABH]", 0x9B, 0xDA, 0262, 0xAB, 0x89);

    TEST("FIDIVR", "WORD PTR [SI]",          0x9B, 0xDE, 0074);
    TEST("FIDIVR", "WORD PTR [1234H]",       0x9B, 0xDE, 0076, 0x34, 0x12);
    TEST("FIDIVR", "WORD PTR [DI+52]",       0x9B, 0xDE, 0175, 0x34);
    TEST("FIDIVR", "WORD PTR [BP+1234H]",    0x9B, 0xDE, 0276, 0x34, 0x12);
    TEST("FIDIVR", "WORD PTR [BX+SI]",       0x9B, 0xDE, 0070);
    TEST("FIDIVR", "WORD PTR [BX+DI-52]",    0x9B, 0xDE, 0171, 0xCC);
    TEST("FIDIVR", "WORD PTR [BP+SI+89ABH]", 0x9B, 0xDE, 0272, 0xAB, 0x89);

    TEST("FIDIVR", "DWORD PTR [SI]",          0x9B, 0xDA, 0074);
    TEST("FIDIVR", "DWORD PTR [1234H]",       0x9B, 0xDA, 0076, 0x34, 0x12);
    TEST("FIDIVR", "DWORD PTR [DI+52]",       0x9B, 0xDA, 0175, 0x34);
    TEST("FIDIVR", "DWORD PTR [BP+1234H]",    0x9B, 0xDA, 0276, 0x34, 0x12);
    TEST("FIDIVR", "DWORD PTR [BX+SI]",       0x9B, 0xDA, 0070);
    TEST("FIDIVR", "DWORD PTR [BX+DI-52]",    0x9B, 0xDA, 0171, 0xCC);
    TEST("FIDIVR", "DWORD PTR [BP+SI+89ABH]", 0x9B, 0xDA, 0272, 0xAB, 0x89);

    TEST("FICOM", "WORD PTR [SI]",          0x9B, 0xDE, 0024);
    TEST("FICOM", "WORD PTR [1234H]",       0x9B, 0xDE, 0026, 0x34, 0x12);
    TEST("FICOM", "WORD PTR [DI+52]",       0x9B, 0xDE, 0125, 0x34);
    TEST("FICOM", "WORD PTR [BP+1234H]",    0x9B, 0xDE, 0226, 0x34, 0x12);
    TEST("FICOM", "WORD PTR [BX+SI]",       0x9B, 0xDE, 0020);
    TEST("FICOM", "WORD PTR [BX+DI-52]",    0x9B, 0xDE, 0121, 0xCC);
    TEST("FICOM", "WORD PTR [BP+SI+89ABH]", 0x9B, 0xDE, 0222, 0xAB, 0x89);

    TEST("FICOM", "DWORD PTR [SI]",          0x9B, 0xDA, 0024);
    TEST("FICOM", "DWORD PTR [1234H]",       0x9B, 0xDA, 0026, 0x34, 0x12);
    TEST("FICOM", "DWORD PTR [DI+52]",       0x9B, 0xDA, 0125, 0x34);
    TEST("FICOM", "DWORD PTR [BP+1234H]",    0x9B, 0xDA, 0226, 0x34, 0x12);
    TEST("FICOM", "DWORD PTR [BX+SI]",       0x9B, 0xDA, 0020);
    TEST("FICOM", "DWORD PTR [BX+DI-52]",    0x9B, 0xDA, 0121, 0xCC);
    TEST("FICOM", "DWORD PTR [BP+SI+89ABH]", 0x9B, 0xDA, 0222, 0xAB, 0x89);

    TEST("FICOMP", "WORD PTR [SI]",          0x9B, 0xDE, 0034);
    TEST("FICOMP", "WORD PTR [1234H]",       0x9B, 0xDE, 0036, 0x34, 0x12);
    TEST("FICOMP", "WORD PTR [DI+52]",       0x9B, 0xDE, 0135, 0x34);
    TEST("FICOMP", "WORD PTR [BP+1234H]",    0x9B, 0xDE, 0236, 0x34, 0x12);
    TEST("FICOMP", "WORD PTR [BX+SI]",       0x9B, 0xDE, 0030);
    TEST("FICOMP", "WORD PTR [BX+DI-52]",    0x9B, 0xDE, 0131, 0xCC);
    TEST("FICOMP", "WORD PTR [BP+SI+89ABH]", 0x9B, 0xDE, 0232, 0xAB, 0x89);

    TEST("FICOMP", "DWORD PTR [SI]",          0x9B, 0xDA, 0034);
    TEST("FICOMP", "DWORD PTR [1234H]",       0x9B, 0xDA, 0036, 0x34, 0x12);
    TEST("FICOMP", "DWORD PTR [DI+52]",       0x9B, 0xDA, 0135, 0x34);
    TEST("FICOMP", "DWORD PTR [BP+1234H]",    0x9B, 0xDA, 0236, 0x34, 0x12);
    TEST("FICOMP", "DWORD PTR [BX+SI]",       0x9B, 0xDA, 0030);
    TEST("FICOMP", "DWORD PTR [BX+DI-52]",    0x9B, 0xDA, 0131, 0xCC);
    TEST("FICOMP", "DWORD PTR [BP+SI+89ABH]", 0x9B, 0xDA, 0232, 0xAB, 0x89);

    TEST("FCHS",    "", 0x9B, 0xD9, 0xE0);
    TEST("FABS",    "", 0x9B, 0xD9, 0xE1);
    TEST("FTST",    "", 0x9B, 0xD9, 0xE4);
    TEST("FXAM",    "", 0x9B, 0xD9, 0xE5);
    TEST("FXTRACT", "", 0x9B, 0xD9, 0xF4);
    TEST("FPREM",   "", 0x9B, 0xD9, 0xF8);
    TEST("FSQRT",   "", 0x9B, 0xD9, 0xFA);
    TEST("FRNDINT", "", 0x9B, 0xD9, 0xFC);
    TEST("FSCALE",  "", 0x9B, 0xD9, 0xFD);

    TEST("F2XM1",   "", 0x9B, 0xD9, 0xF0);
    TEST("FYL2X",   "", 0x9B, 0xD9, 0xF1);
    TEST("FPTAN",   "", 0x9B, 0xD9, 0xF2);
    TEST("FPATAN",  "", 0x9B, 0xD9, 0xF3);
    TEST("FYL2XP1", "", 0x9B, 0xD9, 0xF9);

    TEST("FNOP",    "", 0x9B, 0xD9, 0xD0);
    TEST("FDECSTP", "", 0x9B, 0xD9, 0xF6);
    TEST("FINCSTP", "", 0x9B, 0xD9, 0xF7);
    TEST("FFREE", "ST(0)", 0x9B, 0xDD, 0xC0);
    TEST("FFREE", "ST(1)", 0x9B, 0xDD, 0xC1);
    TEST("FFREE", "ST(2)", 0x9B, 0xDD, 0xC2);
    TEST("FFREE", "ST(3)", 0x9B, 0xDD, 0xC3);
    TEST("FFREE", "ST(4)", 0x9B, 0xDD, 0xC4);
    TEST("FFREE", "ST(5)", 0x9B, 0xDD, 0xC5);
    TEST("FFREE", "ST(6)", 0x9B, 0xDD, 0xC6);
    TEST("FFREE", "ST(7)", 0x9B, 0xDD, 0xC7);
}

static void test_float_nowait() {
    TEST("FNINIT", "",      0xDB, 0xE3);
    TEST("FNLDCW", "[SI]",  0xD9, 0054);
    TEST("FNSTCW", "[SI]",  0xD9, 0074);
    TEST("FNSTSW", "[SI]",  0xDD, 0074);
    TEST("FNCLEX", "",      0xDB, 0xE2);
    TEST("FNLDENV", "[SI]", 0xD9, 0044);
    TEST("FNSTENV", "[SI]", 0xD9, 0064);
    TEST("FNSAVE",  "[SI]", 0xDD, 0064);
    TEST("FNRSTOR", "[SI]", 0xDD, 0044);
    TEST("FNENI", "",       0xDB, 0xE0);
    TEST("FNDISI", "",      0xDB, 0xE1);

    TEST("FNLD", "DWORD PTR [SI]", 0xD9, 0004);
    TEST("FNLD", "QWORD PTR [SI]", 0xDD, 0004);
    TEST("FNLD", "TBYTE PTR [SI]", 0xDB, 0054);
    TEST("FNLD", "ST(0)",          0xD9, 0xC0);
    TEST("FNST", "DWORD PTR [SI]", 0xD9, 0024);
    TEST("FNST", "QWORD PTR [SI]", 0xDD, 0024);
    TEST("FNST", "ST(0)",          0xDD, 0xD0);
    TEST("FNSTP", "DWORD PTR [SI]", 0xD9, 0034);
    TEST("FNSTP", "QWORD PTR [SI]", 0xDD, 0034);
    TEST("FNSTP", "TBYTE PTR [SI]", 0xDB, 0074);
    TEST("FNSTP", "ST(0)",          0xDD, 0xD8);
    TEST("FNLDZ",   "", 0xD9, 0xEE);
    TEST("FNLD1",   "", 0xD9, 0xE8);
    TEST("FNLDPI",  "", 0xD9, 0xEB);
    TEST("FNLDL2T", "", 0xD9, 0xE9);
    TEST("FNLDL2E", "", 0xD9, 0xEA);
    TEST("FNLDLG2", "", 0xD9, 0xEC);
    TEST("FNLDLN2", "", 0xD9, 0xED);
    TEST("FNXCH", "ST(0)", 0xD9, 0xC8);
    TEST("FNILD",   "WORD PTR [SI]", 0xDF, 0004);
    TEST("FNILD",  "DWORD PTR [SI]", 0xDB, 0004);
    TEST("FNILD",  "QWORD PTR [SI]", 0xDF, 0054);
    TEST("FNIST",   "WORD PTR [SI]", 0xDF, 0024);
    TEST("FNIST",  "DWORD PTR [SI]", 0xDB, 0024);
    TEST("FNISTP",  "WORD PTR [SI]", 0xDF, 0034);
    TEST("FNISTP", "DWORD PTR [SI]", 0xDB, 0034);
    TEST("FNISTP", "QWORD PTR [SI]", 0xDF, 0074);
    TEST("FNBLD",  "TBYTE PTR [SI]", 0xDF, 0044);
    TEST("FNBSTP", "TBYTE PTR [SI]", 0xDF, 0064);
    TEST("FNADD",  "DWORD PTR [SI]", 0xD8, 0004);
    TEST("FNADD",  "QWORD PTR [SI]", 0xDC, 0004);
    TEST("FNADD", "ST, ST(1)",  0xD8, 0xC1);
    TEST("FNADD", "ST(2), ST",  0xDC, 0xC2);
    TEST("FNADDP", "ST(3), ST", 0xDE, 0xC3);
    TEST("FNMUL", "DWORD PTR [SI]", 0xD8, 0014);
    TEST("FNMUL", "QWORD PTR [SI]", 0xDC, 0014);
    TEST("FNMUL", "ST, ST(4)",  0xD8, 0xCC);
    TEST("FNMUL", "ST(5), ST",  0xDC, 0xCD);
    TEST("FNMULP", "ST(6), ST", 0xDE, 0xCE);
    TEST("FNSUB", "DWORD PTR [SI]", 0xD8, 0044);
    TEST("FNSUB", "QWORD PTR [SI]", 0xDC, 0044);
    TEST("FNSUB", "ST, ST(7)",  0xD8, 0xE7);
    TEST("FNSUB", "ST(1), ST",  0xDC, 0xE9);
    TEST("FNSUBP", "ST(1), ST", 0xDE, 0xE9);
    TEST("FNSUBR", "DWORD PTR [SI]", 0xD8, 0054);
    TEST("FNSUBR", "QWORD PTR [SI]", 0xDC, 0054);
    TEST("FNSUBR", "ST, ST(2)",  0xD8, 0xEA);
    TEST("FNSUBR", "ST(3), ST",  0xDC, 0xE3);
    TEST("FNSUBRP", "ST(4), ST", 0xDE, 0xE4);
    TEST("FNDIV", "DWORD PTR [SI]", 0xD8, 0064);
    TEST("FNDIV", "QWORD PTR [SI]", 0xDC, 0064);
    TEST("FNDIV", "ST, ST(7)",  0xD8, 0xF7);
    TEST("FNDIV", "ST(1), ST",  0xDC, 0xF9);
    TEST("FNDIVP", "ST(1), ST", 0xDE, 0xF9);
    TEST("FNDIVR", "DWORD PTR [SI]", 0xD8, 0074);
    TEST("FNDIVR", "QWORD PTR [SI]", 0xDC, 0074);
    TEST("FNDIVR", "ST, ST(2)",  0xD8, 0xFA);
    TEST("FNDIVR", "ST(3), ST",  0xDC, 0xF3);
    TEST("FNDIVRP", "ST(4), ST", 0xDE, 0xF4);
    TEST("FNCOM", "DWORD PTR [SI]", 0xD8, 0024);
    TEST("FNCOM", "QWORD PTR [SI]", 0xDC, 0024);
    TEST("FNCOM", "ST(2)", 0xD8, 0xD2);
    TEST("FNCOMP", "DWORD PTR [SI]", 0xD8, 0034);
    TEST("FNCOMP", "QWORD PTR [SI]", 0xDC, 0034);
    TEST("FNCOMP", "ST(2)", 0xD8, 0xDA);
    TEST("FNCOMPP", "",     0xDE, 0xD9);
    TEST("FNIADD",   "WORD PTR [SI]", 0xDE, 0004);
    TEST("FNIADD",  "DWORD PTR [SI]", 0xDA, 0004);
    TEST("FNIMUL",   "WORD PTR [SI]", 0xDE, 0014);
    TEST("FNIMUL",  "DWORD PTR [SI]", 0xDA, 0014);
    TEST("FNISUB",   "WORD PTR [SI]", 0xDE, 0044);
    TEST("FNISUB",  "DWORD PTR [SI]", 0xDA, 0044);
    TEST("FNISUBR",  "WORD PTR [SI]", 0xDE, 0054);
    TEST("FNISUBR", "DWORD PTR [SI]", 0xDA, 0054);
    TEST("FNIDIV",   "WORD PTR [SI]", 0xDE, 0064);
    TEST("FNIDIV",  "DWORD PTR [SI]", 0xDA, 0064);
    TEST("FNIDIVR",  "WORD PTR [SI]", 0xDE, 0074);
    TEST("FNIDIVR", "DWORD PTR [SI]", 0xDA, 0074);
    TEST("FNICOM",   "WORD PTR [SI]", 0xDE, 0024);
    TEST("FNICOM",  "DWORD PTR [SI]", 0xDA, 0024);
    TEST("FNICOMP",  "WORD PTR [SI]", 0xDE, 0034);
    TEST("FNICOMP", "DWORD PTR [SI]", 0xDA, 0034);
    TEST("FNCHS",    "", 0xD9, 0xE0);
    TEST("FNABS",    "", 0xD9, 0xE1);
    TEST("FNTST",    "", 0xD9, 0xE4);
    TEST("FNXAM",    "", 0xD9, 0xE5);
    TEST("FNXTRACT", "", 0xD9, 0xF4);
    TEST("FNPREM",   "", 0xD9, 0xF8);
    TEST("FNSQRT",   "", 0xD9, 0xFA);
    TEST("FNRNDINT", "", 0xD9, 0xFC);
    TEST("FNSCALE",  "", 0xD9, 0xFD);
    TEST("FN2XM1",   "", 0xD9, 0xF0);
    TEST("FNYL2X",   "", 0xD9, 0xF1);
    TEST("FNPTAN",   "", 0xD9, 0xF2);
    TEST("FNPATAN",  "", 0xD9, 0xF3);
    TEST("FNYL2XP1", "", 0xD9, 0xF9);
    TEST("FNNOP",    "", 0xD9, 0xD0);
    TEST("FNDECSTP", "", 0xD9, 0xF6);
    TEST("FNINCSTP", "", 0xD9, 0xF7);
    TEST("FNFREE", "ST(0)", 0xDD, 0xC0);
}

// clang-format on

static void test_illegal() {
    if (v30()) {
        for (Config::opcode_t opc = 0; opc < 0xFF; opc++) {
            if (opc >= 0x12 && opc < 0x18)
                continue;
            if (opc >= 0x1A && opc < 0x20)
                continue;
            if (opc == 0x20 || opc == 0x22 || opc == 0x26 || opc == 0x28 || opc == 0x2A)
                continue;
            if (opc == 0x31 || opc == 0x33 || opc == 0x39 || opc == 0x3B)
                continue;
            UNKN(0x0F, opc);
        }
    } else {
        ERRT("POP", "CS", REGISTER_NOT_ALLOWED, "CS", 0x0F);
    }

    for (Config::opcode_t opc = 0x60; opc < 0x70; opc++) {
        if (is80186() && (opc <= 0x62 || opc >= 0x68))
            continue;
        if (v30() && (opc == 0x64 || opc == 0x65))
            continue;
        UNKN(opc);
    }

    UNKN(0x82);

    if (is8086()) {
        UNKN(0xC0);
        UNKN(0xC1);
        UNKN(0xC8);
        UNKN(0xC9);
    }

    for (auto mod = 0; mod < 4; mod++) {
        for (auto reg = 0; reg < 8; reg++) {
            for (auto r_m = 0; r_m < 8; r_m++) {
                const Config::opcode_t modReg = (mod << 6) | (reg << 3) | r_m;
                if (mod == 3) {
                    if ((modReg >= 0xD1 && modReg < 0xE0) || modReg == 0xE2 || modReg == 0xE3 ||
                            modReg == 0xE6 || modReg == 0xE7 || modReg == 0xEF || modReg == 0xF5 ||
                            modReg == 0xFB || modReg >= 0xFE) {
                        UNKN(0xD9, modReg);
                    }
                    UNKN(0xDA, modReg);
                    if (modReg < 0xE0 || modReg >= 0xE4)
                        UNKN(0xDB, modReg);
                    if (modReg >= 0xD0 && modReg < 0xE0)
                        UNKN(0xDC, modReg);
                    if ((modReg >= 0xC8 && modReg < 0xD0) || modReg >= 0xE0)
                        UNKN(0xDD, modReg);
                    if ((modReg >= 0xD0 && modReg < 0xD9) || (modReg >= 0xDA && modReg < 0xE0))
                        UNKN(0xDE, modReg);
                    UNKN(0xDF, modReg);
                }
                if (reg == 1) {
                    UNKN(0xF6, modReg);
                    UNKN(0xF7, modReg);
                    UNKN(0xDF, modReg);
                }
                if (reg == 6) {
                    UNKN(0xD0, modReg);
                    UNKN(0xD1, modReg);
                    UNKN(0xD2, modReg);
                    UNKN(0xD3, modReg);
                    if (is80186()) {
                        UNKN(0xC0, modReg);
                        UNKN(0xC1, modReg);
                    }
                }
                if (reg == 7)
                    UNKN(0xFF, modReg);
                if (reg == 1 || reg == 6)
                    UNKN(0xDB, modReg);
                if (reg != 0 && reg != 1)
                    UNKN(0xFE, modReg);
                if (reg != 0) {
                    UNKN(0x8F, modReg);
                    UNKN(0xC6, modReg);
                    UNKN(0xC7, modReg);
                }
            }
        }
    }

    for (Config::opcode_t opc = 0;; opc++) {
        if (opc == 0x0A)
            continue;  // AAM, AAD
        UNKN(0xD4, opc);
        UNKN(0xD5, opc);
        if (opc == 0xFF)
            break;
    }

    UNKN(0xD6);
    UNKN(0xF1);
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
    RUN_TEST(test_float);
    RUN_TEST(test_float_nowait);
    RUN_TEST(test_illegal);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

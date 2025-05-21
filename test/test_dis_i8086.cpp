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

#define AEPRE(addr, name, opr, ...) AERRT(addr, name, opr, PREFIX_HAS_NO_EFFECT, opr, __VA_ARGS__)
#define EPRE(name, opr, ...) AEPRE(0, name, opr, __VA_ARGS__)

using namespace libasm;
using namespace libasm::i8086;
using namespace libasm::test;

DisI8086 dis8086;
Disassembler &disassembler(dis8086);

constexpr Config::opcode_t DATA32 = 0x66;
constexpr Config::opcode_t DATA16 = 0x66;
constexpr Config::opcode_t ADDR32 = 0x67;
constexpr Config::opcode_t ADDR16 = 0x67;

bool v30() {
    return strcmp_P("V30", disassembler.config().cpu_P()) == 0;
}

bool is8086() {
    return strcmp_P("8086", disassembler.config().cpu_P()) == 0;
}

bool is80186() {
    return strcmp_P("80186", disassembler.config().cpu_P()) == 0;
}

bool is80286() {
    return strcmp_P("80286", disassembler.config().cpu_P()) == 0;
}

bool is80386() {
    return strcmp_P("80386", disassembler.config().cpu_P()) == 0;
}

bool is80x86() {
    return is80186() || is80286() || is80386() || v30();
}

bool is16bit() {
    return is8086() || is80186() || is80286() || v30();
}

bool hasExtraSeg() {
    return is80386();
}

bool is8087() {
    return strcmp_P("8087", dis8086.fpu_P()) == 0;
}

bool is80287() {
    return strcmp_P("80287", dis8086.fpu_P()) == 0;
}

bool is80387() {
    return strcmp_P("80387", dis8086.fpu_P()) == 0 || strcmp_P("80C187", dis8086.fpu_P()) == 0;
}

bool fpu_on() {
    if (is8086()) {
        EQUALS("8086", OK, dis8086.setFpuType(FPU_ON));
        EQUALS_P("8086", "8087", dis8086.fpu_P());
    } else if (v30()) {
        EQUALS("v30", FLOAT_NOT_SUPPORTED, dis8086.setFpuType(FPU_ON));
        EQUALS_P("v30", "none", dis8086.fpu_P());
        return false;
    } else if (is80186()) {
        EQUALS("80186", OK, dis8086.setFpuType(FPU_ON));
        EQUALS_P("80186/8087", "8087", dis8086.fpu_P());
        EQUALS("80C187", OK, dis8086.setFpuType(FPU_I80C187));
        EQUALS_P("80186/80C187", "80C187", dis8086.fpu_P());
    } else if (is80286()) {
        EQUALS("80286", OK, dis8086.setFpuType(FPU_ON));
        EQUALS_P("80287", "80287", dis8086.fpu_P());
    } else if (is80386()) {
        EQUALS("80386", OK, dis8086.setFpuType(FPU_ON));
        EQUALS_P("80387", "80387", dis8086.fpu_P());
    } else {
        EQUALS("unknown CPU", "", dis8086.cpu_P());
        return false;
    }
    return true;
}

void use16() {
    disassembler.setOption("use16", "on");
}

void use32() {
    disassembler.setOption("use32", "on");
}

void gnu_as(bool enable) {
    disassembler.setOption("gnu-as", enable ? "on" : "off");
    disassembler.setOption("upper-case", enable ? "off" : "on");
}

void relative(bool enable) {
   disassembler.setOption("relative", enable ? "on" : "off");
}

constexpr auto LOCK = 0xF0;
constexpr auto SEGES = 0x26;
constexpr auto SEGCS = 0x2E;
constexpr auto SEGSS = 0x36;
constexpr auto SEGDS = 0x3E;
constexpr auto SEGFS = 0x64;
constexpr auto SEGGS = 0x65;
constexpr auto FWAIT = 0x9B;
constexpr auto REPNE = 0xF2;
constexpr auto REP = 0xF3;
constexpr auto REPNC = 0x64;  // V30
constexpr auto REPC = 0x65;   // V30

void lockInsn(bool enable) {
    dis8086.setOption("lock-insn", enable ? "on" : "off");
}

void repeatInsn(bool enable) {
    dis8086.setOption("repeat-insn", enable ? "on" : "off");
}

void set_up() {
    disassembler.reset();
    fpu_on();
    use16();
    lockInsn(true);
    relative(false);
}

void tear_down() {
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

    EQUALS("cpu 80286", true,   disassembler.setCpu("80286"));
    EQUALS_P("cpu 80286", "80286", disassembler.config().cpu_P());

    EQUALS("cpu i80286", true,   disassembler.setCpu("i80286"));
    EQUALS_P("cpu i80286", "80286", disassembler.config().cpu_P());

    EQUALS("cpu 80386", true,   disassembler.setCpu("80386"));
    EQUALS_P("cpu 80386", "80386", disassembler.config().cpu_P());

    EQUALS("cpu i80386", true,   disassembler.setCpu("i80386"));
    EQUALS_P("cpu i80386", "80386", disassembler.config().cpu_P());

    EQUALS("cpu V30", true,   disassembler.setCpu("V30"));
    EQUALS_P("cpu V30", "V30", disassembler.config().cpu_P());
}

void test_data_transfer() {
    // MOV r/m8,r8
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
    // MOV r8,r/m8
    TEST("MOV", "DH, [SI]",          0x8A, 0064);
    TEST("MOV", "BH, [1234H]",       0x8A, 0076, 0x34, 0x12);
    TEST("MOV", "AL, [DI-52]",       0x8A, 0105, 0xCC);
    TEST("MOV", "CL, [BP+1234H]",    0x8A, 0216, 0x34, 0x12);
    TEST("MOV", "DL, [BX+SI]",       0x8A, 0020);
    TEST("MOV", "BL, [BX+DI-52]",    0x8A, 0131, 0xCC);
    TEST("MOV", "AH, [BP+SI+1234H]", 0x8A, 0242, 0x34, 0x12);
    // MOV r/m16,r16
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
    // MOV r16,r/m16
    TEST("MOV", "SI, [SI]",          0x8B, 0064);
    TEST("MOV", "DI, [1234H]",       0x8B, 0076, 0x34, 0x12);
    TEST("MOV", "AX, [DI+52]",       0x8B, 0105, 0x34);
    TEST("MOV", "CX, [BP+1234H]",    0x8B, 0216, 0x34, 0x12);
    TEST("MOV", "DX, [BX+SI]",       0x8B, 0020);
    TEST("MOV", "BX, [BX+DI+52]",    0x8B, 0131, 0x34);
    TEST("MOV", "SP, [BP+SI+1234H]", 0x8B, 0242, 0x34, 0x12);
    if (is80386()) {
        // MOV r/m32,r32
        TEST("MOV", "EAX, ECX",           DATA32, 0x89, 0310);
        TEST("MOV", "EDX, EBX",           DATA32, 0x89, 0332);
        TEST("MOV", "ESP, EBP",           DATA32, 0x89, 0354);
        TEST("MOV", "[SI], ESI",          DATA32, 0x89, 0064);
        TEST("MOV", "[1234H], EDI",       DATA32, 0x89, 0076, 0x34, 0x12);
        TEST("MOV", "[DI-52], EAX",       DATA32, 0x89, 0105, 0xCC);
        TEST("MOV", "[BP+1234H], ECX",    DATA32, 0x89, 0216, 0x34, 0x12);
        TEST("MOV", "[BX+SI], EDX",       DATA32, 0x89, 0020);
        TEST("MOV", "[BX+DI+52], EBX",    DATA32, 0x89, 0131, 0x34);
        TEST("MOV", "[BP+SI+1234H], ESP", DATA32, 0x89, 0242, 0x34, 0x12);
        // MOV r32,r/m32
        TEST("MOV", "ESI, [SI]",          DATA32, 0x8B, 0064);
        TEST("MOV", "EDI, [1234H]",       DATA32, 0x8B, 0076, 0x34, 0x12);
        TEST("MOV", "EAX, [DI+52]",       DATA32, 0x8B, 0105, 0x34);
        TEST("MOV", "ECX, [BP+1234H]",    DATA32, 0x8B, 0216, 0x34, 0x12);
        TEST("MOV", "EDX, [BX+SI]",       DATA32, 0x8B, 0020);
        TEST("MOV", "EBX, [BX+DI+52]",    DATA32, 0x8B, 0131, 0x34);
        TEST("MOV", "ESP, [BP+SI+1234H]", DATA32, 0x8B, 0242, 0x34, 0x12);
        // MOV r/m32,r32
        use32();
        TEST("MOV", "EAX, ECX",           0x89, 0310);
        TEST("MOV", "EDX, EBX",           0x89, 0332);
        TEST("MOV", "ESP, EBP",           0x89, 0354);
        TEST("MOV", "[ESP+ESI], ESI",     0x89, 0064, 0x34);
        TEST("MOV", "[ESI], EDI",         0x89, 0076);
        TEST("MOV", "[EBP-52], EAX",      0x89, 0105, 0xCC);
        TEST("MOV", "[ESI+12345678H], ECX", 0x89, 0216, 0x78, 0x56, 0x34, 0x12);
        TEST("MOV", "[EAX], EDX",         0x89, 0020);
        TEST("MOV", "[ECX+52], EBX",      0x89, 0131, 0x34);
        TEST("MOV", "[EDX+12345678H], ESP", 0x89, 0242, 0x78, 0x56, 0x34, 0x12);
        // MOV r32,r/m32
        TEST("MOV", "ESI, [ESP+ESI]",     0x8B, 0064, 0x34);
        TEST("MOV", "EDI, [ESI]",         0x8B, 0076);
        TEST("MOV", "EAX, [EBP+52]",      0x8B, 0105, 0x34);
        TEST("MOV", "ECX, [ESI+12345678H]", 0x8B, 0216, 0x78, 0x56, 0x34, 0x12);
        TEST("MOV", "EDX, [EAX]",         0x8B, 0020);
        TEST("MOV", "EBX, [ECX+52]",      0x8B, 0131, 0x34);
        TEST("MOV", "ESP, [EDX+12345678H]", 0x8B, 0242, 0x78, 0x56, 0x34, 0x12);
        use16();
    }

    // MOV r/m8,imm8
    TEST("MOV", "AL, 86",                 0xC6, 0300, 0x56);
    TEST("MOV", "BYTE [SI], 86",          0xC6, 0004, 0x56);
    TEST("MOV", "BYTE [89ABH], 86",       0xC6, 0006, 0xAB, 0x89, 0x56);
    TEST("MOV", "BYTE [DI-52], 86",       0xC6, 0105, 0xCC, 0x56);
    TEST("MOV", "BYTE [BP+89ABH], 86",    0xC6, 0206, 0xAB, 0x89, 0x56);
    TEST("MOV", "BYTE [BX+SI], 86",       0xC6, 0000, 0x56);
    TEST("MOV", "BYTE [BX+DI+52], 86",    0xC6, 0101, 0x34, 0x56);
    TEST("MOV", "BYTE [BP+SI+1234H], 86", 0xC6, 0202, 0x34, 0x12, 0x56);
    // MOV r/m16, imm16
    TEST("MOV", "AX, 5678H",                 0xC7, 0300, 0x78, 0x56);
    TEST("MOV", "WORD [SI], 5678H",          0xC7, 0004, 0x78, 0x56);
    TEST("MOV", "WORD [1234H], 5678H",       0xC7, 0006, 0x34, 0x12, 0x78, 0x56);
    TEST("MOV", "WORD [DI+52], 5678H",       0xC7, 0105, 0x34, 0x78, 0x56);
    TEST("MOV", "WORD [BP+1234H], 5678H",    0xC7, 0206, 0x34, 0x12, 0x78, 0x56);
    TEST("MOV", "WORD [BX+SI], 5678H",       0xC7, 0000, 0x78, 0x56);
    TEST("MOV", "WORD [BX+DI-52], 5678H",    0xC7, 0101, 0xCC, 0x78, 0x56);
    TEST("MOV", "WORD [BP+SI+89ABH], 5678H", 0xC7, 0202, 0xAB, 0x89, 0x78, 0x56);
    // MOV r/m8,imm8
    gnu_as(true);
    TEST("mov", "al, 86",                      0xc6, 0300, 0x56);
    TEST("mov", "byte ptr [si], 86",           0xc6, 0004, 0x56);
    TEST("mov", "byte ptr [0x89AB], 86",       0xc6, 0006, 0xab, 0x89, 0x56);
    TEST("mov", "byte ptr [di-52], 86",        0xc6, 0105, 0xcc, 0x56);
    TEST("mov", "byte ptr [bp+0x89AB], 86",    0xc6, 0206, 0xab, 0x89, 0x56);
    TEST("mov", "byte ptr [bx+si], 86",        0xc6, 0000, 0x56);
    TEST("mov", "byte ptr [bx+di+52], 86",     0xc6, 0101, 0x34, 0x56);
    TEST("mov", "byte ptr [bp+si+0x1234], 86", 0xc6, 0202, 0x34, 0x12, 0x56);
    // MOV r/m16, imm16
    TEST("mov", "ax, 0x5678",                      0xc7, 0300, 0x78, 0x56);
    TEST("mov", "word ptr [si], 0x5678",           0xc7, 0004, 0x78, 0x56);
    TEST("mov", "word ptr [0x1234], 0x5678",       0xc7, 0006, 0x34, 0x12, 0x78, 0x56);
    TEST("mov", "word ptr [di+52], 0x5678",        0xc7, 0105, 0x34, 0x78, 0x56);
    TEST("mov", "word ptr [bp+0x1234], 0x5678",    0xc7, 0206, 0x34, 0x12, 0x78, 0x56);
    TEST("mov", "word ptr [bx+si], 0x5678",        0xc7, 0000, 0x78, 0x56);
    TEST("mov", "word ptr [bx+di-52], 0x5678",     0xc7, 0101, 0xcc, 0x78, 0x56);
    TEST("mov", "word ptr [bp+si+0x89AB], 0x5678", 0xc7, 0202, 0xab, 0x89, 0x78, 0x56);
    gnu_as(false);
    if (is80386()) {
        // MOV r/m32,imm32
        TEST("MOV", "EAX, 12345678H",                 DATA32, 0xC7, 0300, 0x78, 0x56, 0x34, 0x12);
        TEST("MOV", "DWORD [SI], 12345678H",          DATA32, 0xC7, 0004, 0x78, 0x56, 0x34, 0x12);
        TEST("MOV", "DWORD [1234H], 12345678H",       DATA32, 0xC7, 0006, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12);
        TEST("MOV", "DWORD [DI+52], 12345678H",       DATA32, 0xC7, 0105, 0x34, 0x78, 0x56, 0x34, 0x12);
        TEST("MOV", "DWORD [BP+1234H], 12345678H",    DATA32, 0xC7, 0206, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12);
        TEST("MOV", "DWORD [BX+SI], 12345678H",       DATA32, 0xC7, 0000, 0x78, 0x56, 0x34, 0x12);
        TEST("MOV", "DWORD [BX+DI-52], 12345678H",    DATA32, 0xC7, 0101, 0xCC, 0x78, 0x56, 0x34, 0x12);
        TEST("MOV", "DWORD [BP+SI+89ABH], 12345678H", DATA32, 0xC7, 0202, 0xAB, 0x89, 0x78, 0x56, 0x34, 0x12);
        // MOV r/m32,imm32
        gnu_as(true);
        TEST("mov", "eax, 0x12345678",                      DATA32, 0xC7, 0300, 0x78, 0x56, 0x34, 0x12);
        TEST("mov", "dword ptr [si], 0x12345678",           DATA32, 0xC7, 0004, 0x78, 0x56, 0x34, 0x12);
        TEST("mov", "dword ptr [0x1234], 0x12345678",       DATA32, 0xC7, 0006, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12);
        TEST("mov", "dword ptr [di+52], 0x12345678",        DATA32, 0xC7, 0105, 0x34, 0x78, 0x56, 0x34, 0x12);
        TEST("mov", "dword ptr [bp+0x1234], 0x12345678",    DATA32, 0xC7, 0206, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12);
        TEST("mov", "dword ptr [bx+si], 0x12345678",        DATA32, 0xC7, 0000, 0x78, 0x56, 0x34, 0x12);
        TEST("mov", "dword ptr [bx+di-52], 0x12345678",     DATA32, 0xC7, 0101, 0xCC, 0x78, 0x56, 0x34, 0x12);
        TEST("mov", "dword ptr [bp+si+0x89AB], 0x12345678", DATA32, 0xC7, 0202, 0xAB, 0x89, 0x78, 0x56, 0x34, 0x12);
        gnu_as(false);
    }

    // MOV r8,imm8
    TEST("MOV", "AL, 86", 0260, 0x56);
    TEST("MOV", "CH, 86", 0265, 0x56);
    // MOV r16,imm16
    TEST("MOV", "AX, 5678H", 0270, 0x78, 0x56);
    TEST("MOV", "BP, 5678H", 0275, 0x78, 0x56);
    if (is80386()) {
        // MOV r32,imm32
        TEST("MOV", "EAX, 12345678H", DATA32, 0270, 0x78, 0x56, 0x34, 0x12);
        TEST("MOV", "EBP, 12345678H", DATA32, 0275, 0x78, 0x56, 0x34, 0x12);
    }

    // MOV AL,moffs8
    TEST("MOV", "AL, [1234H]", 0xA0, 0x34, 0x12);
    // MOV moffs8,AL
    TEST("MOV", "[1234H], AL", 0xA2, 0x34, 0x12);
    // MOV AX,moffs16
    TEST("MOV", "AX, [1234H]", 0xA1, 0x34, 0x12);
    // MOV moffs16,AX
    TEST("MOV", "[1234H], AX", 0xA3, 0x34, 0x12);
    if (is80386()) {
        // MOV EAX,moffs32
        TEST("MOV", "EAX, [1234H]", DATA32, 0xA1, 0x34, 0x12);
        // MOV moffs32,EAX
        TEST("MOV", "[1234H], EAX", DATA32, 0xA3, 0x34, 0x12);
    }

    // MOV r/m16,sreg
    TEST("MOV", "AX, ES",            0x8C, 0300);
    TEST("MOV", "[SI], CS",          0x8C, 0014);
    TEST("MOV", "[89ABH], SS",       0x8C, 0026, 0xAB, 0x89);
    TEST("MOV", "[DI-52], DS",       0x8C, 0135, 0xCC);
    TEST("MOV", "[BP+89ABH], ES",    0x8C, 0206, 0xAB, 0x89);
    TEST("MOV", "[BX+SI], CS",       0x8C, 0010);
    TEST("MOV", "[BX+DI+52], SS",    0x8C, 0121, 0x34);
    TEST("MOV", "[BP+SI+1234H], DS", 0x8C, 0232, 0x34, 0x12);
    // MOV sreg,m/r16
    TEST("MOV", "ES, AX",            0x8E, 0300);
    TEST("MOV", "CS, [SI]",          0x8E, 0014);
    TEST("MOV", "SS, [89ABH]",       0x8E, 0026, 0xAB, 0x89);
    TEST("MOV", "DS, [DI-52]",       0x8E, 0135, 0xCC);
    TEST("MOV", "ES, [BP+89ABH]",    0x8E, 0206, 0xAB, 0x89);
    TEST("MOV", "CS, [BX+SI]",       0x8E, 0010);
    TEST("MOV", "SS, [BX+DI+52]",    0x8E, 0121, 0x34);
    TEST("MOV", "DS, [BP+SI+1234H]", 0x8E, 0232, 0x34, 0x12);
    if (hasExtraSeg()) {
        // MOV r/m16,sreg
        TEST("MOV", "AX, FS",      0x8C, 0340);
        TEST("MOV", "[SI], GS",    0x8C, 0054);
        // MOV sreg,r/m16
        TEST("MOV", "FS, [89ABH]", 0x8E, 0046, 0xAB, 0x89);
        TEST("MOV", "GS, [DI-52]", 0x8E, 0155, 0xCC);
        use32();
        // MOV r/m16,sreg
        TEST("MOV", "AX, FS",          0x8C, 0340);
        TEST("MOV", "[ESP+EBP*4], GS", 0x8C, 0054, 0254);
        // MOV sreg,r/m16
        TEST("MOV", "FS, [ESI]",    0x8E, 0046);
        TEST("MOV", "GS, [EBP-52]", 0x8E, 0155, 0xCC);
        use16();
    }

    if (is80386()) {
        // MOVSX r16,r/m8
        TEST("MOVSX", "AX, CL",                 0x0F, 0xBE, 0301);
        TEST("MOVSX", "SI, BYTE [SI]",          0x0F, 0xBE, 0064);
        TEST("MOVSX", "DI, BYTE [1234H]",       0x0F, 0xBE, 0076, 0x34, 0x12);
        TEST("MOVSX", "AX, BYTE [DI-52]",       0x0F, 0xBE, 0105, 0xCC);
        TEST("MOVSX", "CX, BYTE [BP+1234H]",    0x0F, 0xBE, 0216, 0x34, 0x12);
        TEST("MOVSX", "DX, BYTE [BX+SI]",       0x0F, 0xBE, 0020);
        TEST("MOVSX", "BX, BYTE [BX+DI-52]",    0x0F, 0xBE, 0131, 0xCC);
        TEST("MOVSX", "SP, BYTE [BP+SI+1234H]", 0x0F, 0xBE, 0242, 0x34, 0x12);

        // MOVSX r32,r/m8
        TEST("MOVSX", "EAX, CL",                 DATA32, 0x0F, 0xBE, 0301);
        TEST("MOVSX", "ESI, BYTE [SI]",          DATA32, 0x0F, 0xBE, 0064);
        TEST("MOVSX", "EDI, BYTE [1234H]",       DATA32, 0x0F, 0xBE, 0076, 0x34, 0x12);
        TEST("MOVSX", "EAX, BYTE [DI-52]",       DATA32, 0x0F, 0xBE, 0105, 0xCC);
        TEST("MOVSX", "ECX, BYTE [BP+1234H]",    DATA32, 0x0F, 0xBE, 0216, 0x34, 0x12);
        TEST("MOVSX", "EDX, BYTE [BX+SI]",       DATA32, 0x0F, 0xBE, 0020);
        TEST("MOVSX", "EBX, BYTE [BX+DI-52]",    DATA32, 0x0F, 0xBE, 0131, 0xCC);
        TEST("MOVSX", "ESP, BYTE [BP+SI+1234H]", DATA32, 0x0F, 0xBE, 0242, 0x34, 0x12);

        // MOVSX r32,r/m16
        TEST("MOVSX", "EAX, CX",                 0x0F, 0xBF, 0301);
        TEST("MOVSX", "ESI, WORD [SI]",          0x0F, 0xBF, 0064);
        TEST("MOVSX", "EDI, WORD [1234H]",       0x0F, 0xBF, 0076, 0x34, 0x12);
        TEST("MOVSX", "EAX, WORD [DI-52]",       0x0F, 0xBF, 0105, 0xCC);
        TEST("MOVSX", "ECX, WORD [BP+1234H]",    0x0F, 0xBF, 0216, 0x34, 0x12);
        TEST("MOVSX", "EDX, WORD [BX+SI]",       0x0F, 0xBF, 0020);
        TEST("MOVSX", "EBX, WORD [BX+DI-52]",    0x0F, 0xBF, 0131, 0xCC);
        TEST("MOVSX", "ESP, WORD [BP+SI+1234H]", 0x0F, 0xBF, 0242, 0x34, 0x12);

        // MOVZX r16,r/m8
        TEST("MOVZX", "AX, CL",                 0x0F, 0xB6, 0301);
        TEST("MOVZX", "SI, BYTE [SI]",          0x0F, 0xB6, 0064);
        TEST("MOVZX", "DI, BYTE [1234H]",       0x0F, 0xB6, 0076, 0x34, 0x12);
        TEST("MOVZX", "AX, BYTE [DI-52]",       0x0F, 0xB6, 0105, 0xCC);
        TEST("MOVZX", "CX, BYTE [BP+1234H]",    0x0F, 0xB6, 0216, 0x34, 0x12);
        TEST("MOVZX", "DX, BYTE [BX+SI]",       0x0F, 0xB6, 0020);
        TEST("MOVZX", "BX, BYTE [BX+DI-52]",    0x0F, 0xB6, 0131, 0xCC);
        TEST("MOVZX", "SP, BYTE [BP+SI+1234H]", 0x0F, 0xB6, 0242, 0x34, 0x12);

        // MOVZX r32,r/m8
        TEST("MOVZX", "EAX, CL",                 DATA32, 0x0F, 0xB6, 0301);
        TEST("MOVZX", "ESI, BYTE [SI]",          DATA32, 0x0F, 0xB6, 0064);
        TEST("MOVZX", "EDI, BYTE [1234H]",       DATA32, 0x0F, 0xB6, 0076, 0x34, 0x12);
        TEST("MOVZX", "EAX, BYTE [DI-52]",       DATA32, 0x0F, 0xB6, 0105, 0xCC);
        TEST("MOVZX", "ECX, BYTE [BP+1234H]",    DATA32, 0x0F, 0xB6, 0216, 0x34, 0x12);
        TEST("MOVZX", "EDX, BYTE [BX+SI]",       DATA32, 0x0F, 0xB6, 0020);
        TEST("MOVZX", "EBX, BYTE [BX+DI-52]",    DATA32, 0x0F, 0xB6, 0131, 0xCC);
        TEST("MOVZX", "ESP, BYTE [BP+SI+1234H]", DATA32, 0x0F, 0xB6, 0242, 0x34, 0x12);

        // MOVZX r32,r/m16
        TEST("MOVZX", "EAX, CX",                 0x0F, 0xB7, 0301);
        TEST("MOVZX", "ESI, WORD [SI]",          0x0F, 0xB7, 0064);
        TEST("MOVZX", "EDI, WORD [1234H]",       0x0F, 0xB7, 0076, 0x34, 0x12);
        TEST("MOVZX", "EAX, WORD [DI-52]",       0x0F, 0xB7, 0105, 0xCC);
        TEST("MOVZX", "ECX, WORD [BP+1234H]",    0x0F, 0xB7, 0216, 0x34, 0x12);
        TEST("MOVZX", "EDX, WORD [BX+SI]",       0x0F, 0xB7, 0020);
        TEST("MOVZX", "EBX, WORD [BX+DI-52]",    0x0F, 0xB7, 0131, 0xCC);
        TEST("MOVZX", "ESP, WORD [BP+SI+1234H]", 0x0F, 0xB7, 0242, 0x34, 0x12);
    }

    // PUSH r16
    TEST("PUSH", "AX", 0120);
    TEST("PUSH", "CX", 0121);
    TEST("PUSH", "DX", 0122);
    TEST("PUSH", "BX", 0123);
    TEST("PUSH", "SP", 0124);
    TEST("PUSH", "BP", 0125);
    TEST("PUSH", "SI", 0126);
    TEST("PUSH", "DI", 0127);
    if (is80386()) {
        // PUSH r32
        TEST("PUSH", "EAX", DATA32, 0120);
        TEST("PUSH", "ECX", DATA32, 0121);
        TEST("PUSH", "EDX", DATA32, 0122);
        TEST("PUSH", "EBX", DATA32, 0123);
        TEST("PUSH", "ESP", DATA32, 0124);
        TEST("PUSH", "EBP", DATA32, 0125);
        TEST("PUSH", "ESI", DATA32, 0126);
        TEST("PUSH", "EDI", DATA32, 0127);
    }

    if (is16bit()) {
        // PUSH m16
        TEST("PUSH", "AX",            0xFF, 0360);
        TEST("PUSH", "BP",            0xFF, 0365);
        TEST("PUSH", "DI",            0xFF, 0367);
        TEST("PUSH", "[SI]",          0xFF, 0064);
        TEST("PUSH", "[1234H]",       0xFF, 0066, 0x34, 0x12);
        TEST("PUSH", "[DI-52]",       0xFF, 0165, 0xCC);
        TEST("PUSH", "[BP+1234H]",    0xFF, 0266, 0x34, 0x12);
        TEST("PUSH", "[BX+SI]",       0xFF, 0060);
        TEST("PUSH", "[BX+DI+52]",    0xFF, 0161, 0x34);
        TEST("PUSH", "[BP+SI+1234H]", 0xFF, 0262, 0x34, 0x12);
    } else {
        // PUSH m16
        TEST("PUSH", "WORD [SI]",          0xFF, 0064);
        TEST("PUSH", "WORD [1234H]",       0xFF, 0066, 0x34, 0x12);
        TEST("PUSH", "WORD [DI-52]",       0xFF, 0165, 0xCC);
        TEST("PUSH", "WORD [BP+1234H]",    0xFF, 0266, 0x34, 0x12);
        TEST("PUSH", "WORD [BX+SI]",       0xFF, 0060);
        TEST("PUSH", "WORD [BX+DI+52]",    0xFF, 0161, 0x34);
        TEST("PUSH", "WORD [BP+SI+1234H]", 0xFF, 0262, 0x34, 0x12);

        // PUSH m32
        TEST("PUSH", "EAX",            DATA32, 0xFF, 0360);
        TEST("PUSH", "EBP",            DATA32, 0xFF, 0365);
        TEST("PUSH", "EDI",            DATA32, 0xFF, 0367);
        TEST("PUSH", "DWORD [SI]",          DATA32, 0xFF, 0064);
        TEST("PUSH", "DWORD [1234H]",       DATA32, 0xFF, 0066, 0x34, 0x12);
        TEST("PUSH", "DWORD [DI-52]",       DATA32, 0xFF, 0165, 0xCC);
        TEST("PUSH", "DWORD [BP+1234H]",    DATA32, 0xFF, 0266, 0x34, 0x12);
        TEST("PUSH", "DWORD [BX+SI]",       DATA32, 0xFF, 0060);
        TEST("PUSH", "DWORD [BX+DI+52]",    DATA32, 0xFF, 0161, 0x34);
        TEST("PUSH", "DWORD [BP+SI+1234H]", DATA32, 0xFF, 0262, 0x34, 0x12);
    }

    if (is80x86()) {
        // PUSH imm8
        TEST("PUSH", "52",    0x6A, 0x34);
        // PUSH imm16
        TEST("PUSH", "1234H", 0x68, 0x34, 0x12);
    }
    if (is80386()) {
        // PUSH imm32
        TEST("PUSH", "12345678H", DATA32, 0x68, 0x78, 0x56, 0x34, 0x12);

        gnu_as(true);
        TEST("data32 push", "0x12345678", DATA32, 0x68, 0x78, 0x56, 0x34, 0x12);
        gnu_as(false);
    }

    TEST("PUSH", "ES", 0006);
    TEST("PUSH", "CS", 0016);
    TEST("PUSH", "SS", 0026);
    TEST("PUSH", "DS", 0036);
    if (hasExtraSeg()) {
        TEST("PUSH", "FS", 0x0F, 0xA0);
        TEST("PUSH", "GS", 0x0F, 0xA8);
    }

    if (is80x86()) {
        TEST("PUSHA", "", 0x60);
        TEST("POPA",  "", 0x61);
    }
    if (is80386()) {
        TEST("PUSHAD", "", DATA32, 0x60);
        TEST("POPAD",  "", DATA32, 0x61);
    }

    // POP r16
    TEST("POP", "AX", 0130);
    TEST("POP", "CX", 0131);
    TEST("POP", "DX", 0132);
    TEST("POP", "BX", 0133);
    TEST("POP", "SP", 0134);
    TEST("POP", "BP", 0135);
    TEST("POP", "SI", 0136);
    TEST("POP", "DI", 0137);
    if (is80386()) {
        // POP r32
        TEST("POP", "EAX", DATA32, 0130);
        TEST("POP", "ECX", DATA32, 0131);
        TEST("POP", "EDX", DATA32, 0132);
        TEST("POP", "EBX", DATA32, 0133);
        TEST("POP", "ESP", DATA32, 0134);
        TEST("POP", "EBP", DATA32, 0135);
        TEST("POP", "ESI", DATA32, 0136);
        TEST("POP", "EDI", DATA32, 0137);
    }

    if (is16bit()) {
        // POP m16
        TEST("POP", "CX",            0x8F, 0301);
        TEST("POP", "BX",            0x8F, 0303);
        TEST("POP", "SI",            0x8F, 0306);
        TEST("POP", "[SI]",          0x8F, 0004);
        TEST("POP", "[1234H]",       0x8F, 0006, 0x34, 0x12);
        TEST("POP", "[DI-52]",       0x8F, 0105, 0xCC);
        TEST("POP", "[BP+1234H]",    0x8F, 0206, 0x34, 0x12);
        TEST("POP", "[BX+SI]",       0x8F, 0000);
        TEST("POP", "[BX+DI+52]",    0x8F, 0101, 0x34);
        TEST("POP", "[BP+SI+1234H]", 0x8F, 0202, 0x34, 0x12);
    } else {
        // POP m16
        TEST("POP", "CX",            0x8F, 0301);
        TEST("POP", "BX",            0x8F, 0303);
        TEST("POP", "SI",            0x8F, 0306);
        TEST("POP", "WORD [SI]",          0x8F, 0004);
        TEST("POP", "WORD [1234H]",       0x8F, 0006, 0x34, 0x12);
        TEST("POP", "WORD [DI-52]",       0x8F, 0105, 0xCC);
        TEST("POP", "WORD [BP+1234H]",    0x8F, 0206, 0x34, 0x12);
        TEST("POP", "WORD [BX+SI]",       0x8F, 0000);
        TEST("POP", "WORD [BX+DI+52]",    0x8F, 0101, 0x34);
        TEST("POP", "WORD [BP+SI+1234H]", 0x8F, 0202, 0x34, 0x12);

        // POP m32
        TEST("POP", "ECX",            DATA32, 0x8F, 0301);
        TEST("POP", "EBX",            DATA32, 0x8F, 0303);
        TEST("POP", "ESI",            DATA32, 0x8F, 0306);
        TEST("POP", "DWORD [SI]",          DATA32, 0x8F, 0004);
        TEST("POP", "DWORD [1234H]",       DATA32, 0x8F, 0006, 0x34, 0x12);
        TEST("POP", "DWORD [DI-52]",       DATA32, 0x8F, 0105, 0xCC);
        TEST("POP", "DWORD [BP+1234H]",    DATA32, 0x8F, 0206, 0x34, 0x12);
        TEST("POP", "DWORD [BX+SI]",       DATA32, 0x8F, 0000);
        TEST("POP", "DWORD [BX+DI+52]",    DATA32, 0x8F, 0101, 0x34);
        TEST("POP", "DWORD [BP+SI+1234H]", DATA32, 0x8F, 0202, 0x34, 0x12);
    }

    TEST("POP", "ES", 0007);
    if (is8086() || is80186()) {
        ERRT("POP", "CS", REGISTER_NOT_ALLOWED, "CS", 0017);
    }
    TEST("POP", "SS", 0027);
    TEST("POP", "DS", 0037);
    if (hasExtraSeg()) {
        TEST("POP", "FS", 0x0F, 0xA1);
        TEST("POP", "GS", 0x0F, 0xA9);
    }

    // XCHG r/m8,r8
    TEST("XCHG", "CH, AL",            0x86, 0305);
    TEST("XCHG", "[SI], CL",          0x86, 0014);
    TEST("XCHG", "[1234H], DL",       0x86, 0026, 0x34, 0x12);
    TEST("XCHG", "[DI-52], BL",       0x86, 0135, 0xCC);
    TEST("XCHG", "[BP+1234H], AH",    0x86, 0246, 0x34, 0x12);
    TEST("XCHG", "[BX+SI], CH",       0x86, 0050);
    TEST("XCHG", "[BX+DI+52], DH",    0x86, 0161, 0x34);
    TEST("XCHG", "[BP+SI+1234H], BH", 0x86, 0272, 0x34, 0x12);
    // XCHG r/m16,r16
    TEST("XCHG", "AX, BP",            0x87, 0350);
    TEST("XCHG", "[SI], CX",          0x87, 0014);
    TEST("XCHG", "[1234H], DX",       0x87, 0026, 0x34, 0x12);
    TEST("XCHG", "[DI-52], BX",       0x87, 0135, 0xCC);
    TEST("XCHG", "[BP+1234H], SP",    0x87, 0246, 0x34, 0x12);
    TEST("XCHG", "[BX+SI], BP",       0x87, 0050);
    TEST("XCHG", "[BX+DI+52], SI",    0x87, 0161, 0x34);
    TEST("XCHG", "[BP+SI+1234H], DI", 0x87, 0272, 0x34, 0x12);

    if (is80386()) {
        // XCHG r/m32,r32
        TEST("XCHG", "EAX, EBP",           DATA32, 0x87, 0350);
        TEST("XCHG", "[SI], ECX",          DATA32, 0x87, 0014);
        TEST("XCHG", "[1234H], EDX",       DATA32, 0x87, 0026, 0x34, 0x12);
        TEST("XCHG", "[DI-52], EBX",       DATA32, 0x87, 0135, 0xCC);
        TEST("XCHG", "[BP+1234H], ESP",    DATA32, 0x87, 0246, 0x34, 0x12);
        TEST("XCHG", "[BX+SI], EBP",       DATA32, 0x87, 0050);
        TEST("XCHG", "[BX+DI+52], ESI",    DATA32, 0x87, 0161, 0x34);
        TEST("XCHG", "[BP+SI+1234H], EDI", DATA32, 0x87, 0272, 0x34, 0x12);
    }

    // XCHG AX,r16
    TEST("XCHG", "AX, CX", 0x91);
    TEST("XCHG", "AX, DX", 0x92);
    TEST("XCHG", "AX, BX", 0x93);
    TEST("XCHG", "AX, SP", 0x94);
    TEST("XCHG", "AX, BP", 0x95);
    TEST("XCHG", "AX, SI", 0x96);
    TEST("XCHG", "AX, DI", 0x97);
    if (is80386()) {
        // XCHG EAX,r32
        TEST("XCHG", "EAX, ECX", DATA32, 0x91);
        TEST("XCHG", "EAX, EDX", DATA32, 0x92);
        TEST("XCHG", "EAX, EBX", DATA32, 0x93);
        TEST("XCHG", "EAX, ESP", DATA32, 0x94);
        TEST("XCHG", "EAX, EBP", DATA32, 0x95);
        TEST("XCHG", "EAX, ESI", DATA32, 0x96);
        TEST("XCHG", "EAX, EDI", DATA32, 0x97);
    }

    // IN AL,imm8
    TEST("IN", "AL, 52", 0xE4, 0x34);
    // IN AL,DX
    TEST("IN", "AL, DX", 0xEC);
    // IN AX,imm8
    TEST("IN", "AX, 52", 0xE5, 0x34);
    // IN AX,DX
    TEST("IN", "AX, DX", 0xED);
    if (is80386()) {
        // IN EAX,imm8
        TEST("IN", "EAX, 52", DATA32, 0xE5, 0x34);
        // IN EAX,DX
        TEST("IN", "EAX, DX", DATA32, 0xED);
    }

    // OUT imm8,AL
    TEST("OUT", "52, AL", 0xE6, 0x34);
    // OUT DX,AL
    TEST("OUT", "DX, AL", 0xEE);
    // OUT imm8,AX
    TEST("OUT", "52, AX", 0xE7, 0x34);
    // OUT DX,AX
    TEST("OUT", "DX, AX", 0xEF);
    if (is80386()) {
        // OUT imm8,EAX
        TEST("OUT", "52, EAX", DATA32, 0xE7, 0x34);
        // OUT DX,EAX
        TEST("OUT", "DX, EAX", DATA32, 0xEF);
    }

    if (is80286() || is80386()) {
        // XTLAT m8
        TEST("XLAT", "[BX]", 0xD7);
    } else if (is80386()) {
        // XLAT m8
        TEST("XLAT", "[EBX]", ADDR32, 0xD7);
    } else {
        TEST("XLAT", "", 0xD7);
    }

    // LEA r16,m
    TEST("LEA", "CX, [SI]",          0x8D, 0014);
    TEST("LEA", "DX, [1235H]",       0x8D, 0026, 0x35, 0x12);
    TEST("LEA", "BX, [DI-52]",       0x8D, 0135, 0xCC);
    TEST("LEA", "SP, [BP+1234H]",    0x8D, 0246, 0x34, 0x12);
    TEST("LEA", "BP, [BX+SI]",       0x8D, 0050);
    TEST("LEA", "SI, [BX+DI+52]",    0x8D, 0161, 0x34);
    TEST("LEA", "DI, [BP+SI+1234H]", 0x8D, 0272, 0x34, 0x12);
    if (is80386()) {
        // LEA r32,m
        TEST("LEA", "ECX, [SI]",          DATA32, 0x8D, 0014);
        TEST("LEA", "EDX, [1235H]",       DATA32, 0x8D, 0026, 0x35, 0x12);
        TEST("LEA", "EBX, [DI-52]",       DATA32, 0x8D, 0135, 0xCC);
        TEST("LEA", "ESP, [BP+1234H]",    DATA32, 0x8D, 0246, 0x34, 0x12);
        TEST("LEA", "EBP, [BX+SI]",       DATA32, 0x8D, 0050);
        TEST("LEA", "ESI, [BX+DI+52]",    DATA32, 0x8D, 0161, 0x34);
        TEST("LEA", "EDI, [BP+SI+1234H]", DATA32, 0x8D, 0272, 0x34, 0x12);
        use32();

        use16();
    }

    // LDS r16,m16:16
    TEST("LDS", "CX, [SI]",          0xC5, 0014);
    TEST("LDS", "DX, [1234H]",       0xC5, 0026, 0x34, 0x12);
    TEST("LDS", "BX, [DI-52]",       0xC5, 0135, 0xCC);
    TEST("LDS", "SP, [BP+1234H]",    0xC5, 0246, 0x34, 0x12);
    TEST("LDS", "BP, [BX+SI]",       0xC5, 0050);
    TEST("LDS", "SI, [BX+DI+52]",    0xC5, 0161, 0x34);
    TEST("LDS", "DI, [BP+SI+1234H]", 0xC5, 0272, 0x34, 0x12);
    ERRT("LDS", "DX, CX", ILLEGAL_OPERAND, "CX", 0xC5, 0321);
    if (is80386()) {
        // LDS r32,m16:32
        TEST("LDS", "ECX, [SI]",          DATA32, 0xC5, 0014);
        TEST("LDS", "EDX, [1234H]",       DATA32, 0xC5, 0026, 0x34, 0x12);
        TEST("LDS", "EBX, [DI-52]",       DATA32, 0xC5, 0135, 0xCC);
        TEST("LDS", "ESP, [BP+1234H]",    DATA32, 0xC5, 0246, 0x34, 0x12);
        TEST("LDS", "EBP, [BX+SI]",       DATA32, 0xC5, 0050);
        TEST("LDS", "ESI, [BX+DI+52]",    DATA32, 0xC5, 0161, 0x34);
        TEST("LDS", "EDI, [BP+SI+1234H]", DATA32, 0xC5, 0272, 0x34, 0x12);
        ERRT("LDS", "EDX, ECX", ILLEGAL_OPERAND, "ECX", DATA32, 0xC5, 0321);
    }

    // LES r16,m16:16
    TEST("LES", "CX, [SI]",          0xC4, 0014);
    TEST("LES", "DX, [1234H]",       0xC4, 0026, 0x34, 0x12);
    TEST("LES", "BX, [DI-52]",       0xC4, 0135, 0xCC);
    TEST("LES", "SP, [BP+1234H]",    0xC4, 0246, 0x34, 0x12);
    TEST("LES", "BP, [BX+SI]",       0xC4, 0050);
    TEST("LES", "SI, [BX+DI+52]",    0xC4, 0161, 0x34);
    TEST("LES", "DI, [BP+SI+1234H]", 0xC4, 0272, 0x34, 0x12);
    ERRT("LES", "BP, SP", ILLEGAL_OPERAND, "SP", 0xC4, 0354);
    if (is80386()) {
        // LES r32,m16:32
        TEST("LES", "ECX, [SI]",          DATA32, 0xC4, 0014);
        TEST("LES", "EDX, [1234H]",       DATA32, 0xC4, 0026, 0x34, 0x12);
        TEST("LES", "EBX, [DI-52]",       DATA32, 0xC4, 0135, 0xCC);
        TEST("LES", "ESP, [BP+1234H]",    DATA32, 0xC4, 0246, 0x34, 0x12);
        TEST("LES", "EBP, [BX+SI]",       DATA32, 0xC4, 0050);
        TEST("LES", "ESI, [BX+DI+52]",    DATA32, 0xC4, 0161, 0x34);
        TEST("LES", "EDI, [BP+SI+1234H]", DATA32, 0xC4, 0272, 0x34, 0x12);
        ERRT("LES", "EBP, ESP", ILLEGAL_OPERAND, "ESP", DATA32, 0xC4, 0354);
    }

    if (is80386()) {
        // LSS r16,m16:16
        TEST("LSS", "CX, [SI]",          0x0F, 0xB2, 0014);
        TEST("LSS", "DX, [1234H]",       0x0F, 0xB2, 0026, 0x34, 0x12);
        TEST("LSS", "BX, [DI-52]",       0x0F, 0xB2, 0135, 0xCC);
        TEST("LSS", "SP, [BP+1234H]",    0x0F, 0xB2, 0246, 0x34, 0x12);
        TEST("LSS", "BP, [BX+SI]",       0x0F, 0xB2, 0050);
        TEST("LSS", "SI, [BX+DI+52]",    0x0F, 0xB2, 0161, 0x34);
        TEST("LSS", "DI, [BP+SI+1234H]", 0x0F, 0xB2, 0272, 0x34, 0x12);
        ERRT("LSS", "BP, SP", ILLEGAL_OPERAND, "SP", 0x0F, 0xB2, 0354);
        // LSS r32,m16:32
        TEST("LSS", "ECX, [SI]",          DATA32, 0x0F, 0xB2, 0014);
        TEST("LSS", "EDX, [1234H]",       DATA32, 0x0F, 0xB2, 0026, 0x34, 0x12);
        TEST("LSS", "EBX, [DI-52]",       DATA32, 0x0F, 0xB2, 0135, 0xCC);
        TEST("LSS", "ESP, [BP+1234H]",    DATA32, 0x0F, 0xB2, 0246, 0x34, 0x12);
        TEST("LSS", "EBP, [BX+SI]",       DATA32, 0x0F, 0xB2, 0050);
        TEST("LSS", "ESI, [BX+DI+52]",    DATA32, 0x0F, 0xB2, 0161, 0x34);
        TEST("LSS", "EDI, [BP+SI+1234H]", DATA32, 0x0F, 0xB2, 0272, 0x34, 0x12);
        ERRT("LSS", "EBP, ESP", ILLEGAL_OPERAND, "ESP", DATA32, 0x0F, 0xB2, 0354);

        // LFS r16,m16:16
        TEST("LFS", "CX, [SI]",          0x0F, 0xB4, 0014);
        TEST("LFS", "DX, [1234H]",       0x0F, 0xB4, 0026, 0x34, 0x12);
        TEST("LFS", "BX, [DI-52]",       0x0F, 0xB4, 0135, 0xCC);
        TEST("LFS", "SP, [BP+1234H]",    0x0F, 0xB4, 0246, 0x34, 0x12);
        TEST("LFS", "BP, [BX+SI]",       0x0F, 0xB4, 0050);
        TEST("LFS", "SI, [BX+DI+52]",    0x0F, 0xB4, 0161, 0x34);
        TEST("LFS", "DI, [BP+SI+1234H]", 0x0F, 0xB4, 0272, 0x34, 0x12);
        ERRT("LFS", "BP, SP", ILLEGAL_OPERAND, "SP", 0x0F, 0xB4, 0354);
        // LFS r32,m16:32
        TEST("LFS", "ECX, [SI]",          DATA32, 0x0F, 0xB4, 0014);
        TEST("LFS", "EDX, [1234H]",       DATA32, 0x0F, 0xB4, 0026, 0x34, 0x12);
        TEST("LFS", "EBX, [DI-52]",       DATA32, 0x0F, 0xB4, 0135, 0xCC);
        TEST("LFS", "ESP, [BP+1234H]",    DATA32, 0x0F, 0xB4, 0246, 0x34, 0x12);
        TEST("LFS", "EBP, [BX+SI]",       DATA32, 0x0F, 0xB4, 0050);
        TEST("LFS", "ESI, [BX+DI+52]",    DATA32, 0x0F, 0xB4, 0161, 0x34);
        TEST("LFS", "EDI, [BP+SI+1234H]", DATA32, 0x0F, 0xB4, 0272, 0x34, 0x12);
        ERRT("LFS", "EBP, ESP", ILLEGAL_OPERAND, "ESP", DATA32, 0x0F, 0xB4, 0354);

        // LGS r16,m16:16
        TEST("LGS", "CX, [SI]",          0x0F, 0xB5, 0014);
        TEST("LGS", "DX, [1234H]",       0x0F, 0xB5, 0026, 0x34, 0x12);
        TEST("LGS", "BX, [DI-52]",       0x0F, 0xB5, 0135, 0xCC);
        TEST("LGS", "SP, [BP+1234H]",    0x0F, 0xB5, 0246, 0x34, 0x12);
        TEST("LGS", "BP, [BX+SI]",       0x0F, 0xB5, 0050);
        TEST("LGS", "SI, [BX+DI+52]",    0x0F, 0xB5, 0161, 0x34);
        TEST("LGS", "DI, [BP+SI+1234H]", 0x0F, 0xB5, 0272, 0x34, 0x12);
        ERRT("LGS", "BP, SP", ILLEGAL_OPERAND, "SP", 0x0F, 0xB5, 0354);
        // LGS r32,m16:32
        TEST("LGS", "ECX, [SI]",          DATA32, 0x0F, 0xB5, 0014);
        TEST("LGS", "EDX, [1234H]",       DATA32, 0x0F, 0xB5, 0026, 0x34, 0x12);
        TEST("LGS", "EBX, [DI-52]",       DATA32, 0x0F, 0xB5, 0135, 0xCC);
        TEST("LGS", "ESP, [BP+1234H]",    DATA32, 0x0F, 0xB5, 0246, 0x34, 0x12);
        TEST("LGS", "EBP, [BX+SI]",       DATA32, 0x0F, 0xB5, 0050);
        TEST("LGS", "ESI, [BX+DI+52]",    DATA32, 0x0F, 0xB5, 0161, 0x34);
        TEST("LGS", "EDI, [BP+SI+1234H]", DATA32, 0x0F, 0xB5, 0272, 0x34, 0x12);
        ERRT("LGS", "EBP, ESP", ILLEGAL_OPERAND, "ESP", DATA32, 0x0F, 0xB5, 0354);
    }

    TEST("LAHF",  "", 0x9F);
    TEST("SAHF",  "", 0x9E);
    TEST("PUSHF", "", 0x9C);
    TEST("POPF",  "", 0x9D);
    if (is80386()) {
        TEST("PUSHFD", "", DATA32, 0x9C);
        TEST("POPFD",  "", DATA32, 0x9D);
    }
}

void test_arithmetic() {
    // ADD r/m8,r8
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
    // ADD r8,r/m8
    TEST("ADD", "DH, [SI]",          0x02, 0064);
    TEST("ADD", "BH, [1234H]",       0x02, 0076, 0x34, 0x12);
    TEST("ADD", "AL, [DI-52]",       0x02, 0105, 0xCC);
    TEST("ADD", "CL, [BP+1234H]",    0x02, 0216, 0x34, 0x12);
    TEST("ADD", "DL, [BX+SI]",       0x02, 0020);
    TEST("ADD", "BL, [BX+DI-52]",    0x02, 0131, 0xCC);
    TEST("ADD", "AH, [BP+SI+1234H]", 0x02, 0242, 0x34, 0x12);
    // ADD r/m16,r16
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
    // ADD r16,r/m16
    TEST("ADD", "SI, [SI]",          0x03, 0064);
    TEST("ADD", "DI, [1234H]",       0x03, 0076, 0x34, 0x12);
    TEST("ADD", "AX, [DI+52]",       0x03, 0105, 0x34);
    TEST("ADD", "CX, [BP+1234H]",    0x03, 0216, 0x34, 0x12);
    TEST("ADD", "DX, [BX+SI]",       0x03, 0020);
    TEST("ADD", "BX, [BX+DI+52]",    0x03, 0131, 0x34);
    TEST("ADD", "SP, [BP+SI+1234H]", 0x03, 0242, 0x34, 0x12);
    if (is80386()) {
        // ADD r8,r/m8
        EPRE("ADD", "AL, [DI-52]",   DATA32, 0x02, 0105, 0xCC);
        // ADD r32,r/m32
        TEST("ADD", "EAX, ECX",           DATA32, 0x01, 0310);
        TEST("ADD", "EDX, EBX",           DATA32, 0x01, 0332);
        TEST("ADD", "ESP, EBP",           DATA32, 0x01, 0354);
        TEST("ADD", "[SI], ESI",          DATA32, 0x01, 0064);
        TEST("ADD", "[1234H], EDI",       DATA32, 0x01, 0076, 0x34, 0x12);
        TEST("ADD", "[DI-52], EAX",       DATA32, 0x01, 0105, 0xCC);
        TEST("ADD", "[BP+1234H], ECX",    DATA32, 0x01, 0216, 0x34, 0x12);
        TEST("ADD", "[BX+SI], EDX",       DATA32, 0x01, 0020);
        TEST("ADD", "[BX+DI+52], EBX",    DATA32, 0x01, 0131, 0x34);
        TEST("ADD", "[BP+SI+1234H], ESP", DATA32, 0x01, 0242, 0x34, 0x12);
        TEST("ADD", "ESI, [SI]",          DATA32, 0x03, 0064);
        TEST("ADD", "EDI, [1234H]",       DATA32, 0x03, 0076, 0x34, 0x12);
        TEST("ADD", "EAX, [DI+52]",       DATA32, 0x03, 0105, 0x34);
        TEST("ADD", "ECX, [BP+1234H]",    DATA32, 0x03, 0216, 0x34, 0x12);
        TEST("ADD", "EDX, [BX+SI]",       DATA32, 0x03, 0020);
        TEST("ADD", "EBX, [BX+DI+52]",    DATA32, 0x03, 0131, 0x34);
        TEST("ADD", "ESP, [BP+SI+1234H]", DATA32, 0x03, 0242, 0x34, 0x12);
    }

    // ADD r/m8,imm8
    TEST("ADD", "AL, 86",                 0x80, 0300, 0x56);
    TEST("ADD", "CL, 86",                 0x80, 0301, 0x56);
    TEST("ADD", "BYTE [SI], 86",          0x80, 0004, 0x56);
    TEST("ADD", "BYTE [89ABH], 86",       0x80, 0006, 0xAB, 0x89, 0x56);
    TEST("ADD", "BYTE [DI-52], 86",       0x80, 0105, 0xCC, 0x56);
    TEST("ADD", "BYTE [BP+89ABH], 86",    0x80, 0206, 0xAB, 0x89, 0x56);
    TEST("ADD", "BYTE [BX+SI], 86",       0x80, 0000, 0x56);
    TEST("ADD", "BYTE [BX+DI+52], 86",    0x80, 0101, 0x34, 0x56);
    TEST("ADD", "BYTE [BP+SI+1234H], 86", 0x80, 0202, 0x34, 0x12, 0x56);
    // ADD r/m16,imm16
    TEST("ADD", "AX, 5678H",                 0x81, 0300, 0x78, 0x56);
    TEST("ADD", "CX, 5678H",                 0x81, 0301, 0x78, 0x56);
    TEST("ADD", "WORD [SI], 5678H",          0x81, 0004, 0x78, 0x56);
    TEST("ADD", "WORD [1234H], 5678H",       0x81, 0006, 0x34, 0x12, 0x78, 0x56);
    TEST("ADD", "WORD [DI+52], 5678H",       0x81, 0105, 0x34, 0x78, 0x56);
    TEST("ADD", "WORD [BP+1234H], 5678H",    0x81, 0206, 0x34, 0x12, 0x78, 0x56);
    TEST("ADD", "WORD [BX+SI], 5678H",       0x81, 0000, 0x78, 0x56);
    TEST("ADD", "WORD [BX+DI-52], 5678H",    0x81, 0101, 0xCC, 0x78, 0x56);
    TEST("ADD", "WORD [BP+SI+89ABH], 5678H", 0x81, 0202, 0xAB, 0x89, 0x78, 0x56);
    // ADD r/m16,imm8
    TEST("ADD", "AX, 86",                 0x83, 0300, 0x56);
    TEST("ADD", "AX, -16",                0x83, 0300, 0xF0);
    TEST("ADD", "CX, 86",                 0x83, 0301, 0x56);
    TEST("ADD", "CX, -16",                0x83, 0301, 0xF0);
    TEST("ADD", "WORD [SI], 86",          0x83, 0004, 0x56);
    TEST("ADD", "WORD [1234H], 86",       0x83, 0006, 0x34, 0x12, 0x56);
    TEST("ADD", "WORD [DI+52], 86",       0x83, 0105, 0x34, 0x56);
    TEST("ADD", "WORD [BP+1234H], 86",    0x83, 0206, 0x34, 0x12, 0x56);
    TEST("ADD", "WORD [BX+SI], 86",       0x83, 0000, 0x56);
    TEST("ADD", "WORD [BX+DI-52], 86",    0x83, 0101, 0xCC, 0x56);
    TEST("ADD", "WORD [BP+SI+89ABH], 86", 0x83, 0202, 0xAB, 0x89, 0x56);
    if (is80386()) {
        // ADD r/m32,r32
        TEST("ADD", "EAX, 12345678H",                 DATA32, 0x81, 0300, 0x78, 0x56, 0x34, 0x12);
        TEST("ADD", "ECX, 12345678H",                 DATA32, 0x81, 0301, 0x78, 0x56, 0x34, 0x12);
        TEST("ADD", "DWORD [SI], 12345678H",          DATA32, 0x81, 0004, 0x78, 0x56, 0x34, 0x12);
        TEST("ADD", "DWORD [1234H], 12345678H",       DATA32, 0x81, 0006, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12);
        TEST("ADD", "DWORD [DI+52], 12345678H",       DATA32, 0x81, 0105, 0x34, 0x78, 0x56, 0x34, 0x12);
        TEST("ADD", "DWORD [BP+1234H], 12345678H",    DATA32, 0x81, 0206, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12);
        TEST("ADD", "DWORD [BX+SI], 12345678H",       DATA32, 0x81, 0000, 0x78, 0x56, 0x34, 0x12);
        TEST("ADD", "DWORD [BX+DI-52], 12345678H",    DATA32, 0x81, 0101, 0xCC, 0x78, 0x56, 0x34, 0x12);
        TEST("ADD", "DWORD [BP+SI+89ABH], 12345678H", DATA32, 0x81, 0202, 0xAB, 0x89, 0x78, 0x56, 0x34, 0x12);
        // ADD r32,r/m32
        TEST("ADD", "EAX, 86",                 DATA32, 0x83, 0300, 0x56);
        TEST("ADD", "EAX, -16",                DATA32, 0x83, 0300, 0xF0);
        TEST("ADD", "ECX, 86",                 DATA32, 0x83, 0301, 0x56);
        TEST("ADD", "ECX, -16",                DATA32, 0x83, 0301, 0xF0);
        TEST("ADD", "DWORD [SI], 86",          DATA32, 0x83, 0004, 0x56);
        TEST("ADD", "DWORD [1234H], 86",       DATA32, 0x83, 0006, 0x34, 0x12, 0x56);
        TEST("ADD", "DWORD [DI+52], 86",       DATA32, 0x83, 0105, 0x34, 0x56);
        TEST("ADD", "DWORD [BP+1234H], 86",    DATA32, 0x83, 0206, 0x34, 0x12, 0x56);
        TEST("ADD", "DWORD [BX+SI], 86",       DATA32, 0x83, 0000, 0x56);
        TEST("ADD", "DWORD [BX+DI-52], 86",    DATA32, 0x83, 0101, 0xCC, 0x56);
        TEST("ADD", "DWORD [BP+SI+89ABH], 86", DATA32, 0x83, 0202, 0xAB, 0x89, 0x56);
    }

    // ADD AL,imm8
    TEST("ADD", "AL, 86",   0x04, 0x56);
    // ADD AX,imm16
    TEST("ADD", "AX, 5678H", 0x05, 0x78, 0x56);
    if (is80386()) {
        // ADD EAX,imm32
        TEST("ADD", "EAX, 12345678H", DATA32,         0x05, 0x78, 0x56, 0x34, 0x12);
        EPRE("ADD", "AX, 5678H",      ADDR32,         0x05, 0x78, 0x56);
        EPRE("ADD", "EAX, 12345678H", DATA32, ADDR32, 0x05, 0x78, 0x56, 0x34, 0x12);
        use32();
        TEST("ADD", "EAX, 12345678H",                 0x05, 0x78, 0x56, 0x34, 0x12);
        TEST("ADD", "AX, 5678H",      DATA16,         0x05, 0x78, 0x56);
        EPRE("ADD", "EAX, 12345678H", ADDR16,         0x05, 0x78, 0x56, 0x34, 0x12);
        EPRE("ADD", "AX, 5678H",      DATA16, ADDR16, 0x05, 0x78, 0x56);
        use16();
    }

    // ADC r/m8,r8
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
    // ADC r/m16,r16
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
    if (is80386()) {
        // ADC r/m32,r32
        TEST("ADC", "EAX, ECX",           DATA32, 0x11, 0310);
        TEST("ADC", "EDX, EBX",           DATA32, 0x11, 0332);
        TEST("ADC", "ESP, EBP",           DATA32, 0x11, 0354);
        TEST("ADC", "[SI], ESI",          DATA32, 0x11, 0064);
        TEST("ADC", "[1234H], EDI",       DATA32, 0x11, 0076, 0x34, 0x12);
        TEST("ADC", "[DI-52], EAX",       DATA32, 0x11, 0105, 0xCC);
        TEST("ADC", "[BP+1234H], ECX",    DATA32, 0x11, 0216, 0x34, 0x12);
        TEST("ADC", "[BX+SI], EDX",       DATA32, 0x11, 0020);
        TEST("ADC", "[BX+DI+52], EBX",    DATA32, 0x11, 0131, 0x34);
        TEST("ADC", "[BP+SI+1234H], ESP", DATA32, 0x11, 0242, 0x34, 0x12);
        TEST("ADC", "ESI, [SI]",          DATA32, 0x13, 0064);
        TEST("ADC", "EDI, [1234H]",       DATA32, 0x13, 0076, 0x34, 0x12);
        TEST("ADC", "EAX, [DI+52]",       DATA32, 0x13, 0105, 0x34);
        TEST("ADC", "ECX, [BP+1234H]",    DATA32, 0x13, 0216, 0x34, 0x12);
        TEST("ADC", "EDX, [BX+SI]",       DATA32, 0x13, 0020);
        TEST("ADC", "EBX, [BX+DI+52]",    DATA32, 0x13, 0131, 0x34);
        TEST("ADC", "ESP, [BP+SI+1234H]", DATA32, 0x13, 0242, 0x34, 0x12);
    }

    // ADC r/m8,imm8
    TEST("ADC", "AL, 86",                 0x80, 0320, 0x56);
    TEST("ADC", "CL, 86",                 0x80, 0321, 0x56);
    TEST("ADC", "BYTE [SI], 86",          0x80, 0024, 0x56);
    TEST("ADC", "BYTE [89ABH], 86",       0x80, 0026, 0xAB, 0x89, 0x56);
    TEST("ADC", "BYTE [DI-52], 86",       0x80, 0125, 0xCC, 0x56);
    TEST("ADC", "BYTE [BP+89ABH], 86",    0x80, 0226, 0xAB, 0x89, 0x56);
    TEST("ADC", "BYTE [BX+SI], 86",       0x80, 0020, 0x56);
    TEST("ADC", "BYTE [BX+DI+52], 86",    0x80, 0121, 0x34, 0x56);
    TEST("ADC", "BYTE [BP+SI+1234H], 86", 0x80, 0222, 0x34, 0x12, 0x56);
    // ADC r/m16,imm16
    TEST("ADC", "AX, 5678H",                 0x81, 0320, 0x78, 0x56);
    TEST("ADC", "CX, 5678H",                 0x81, 0321, 0x78, 0x56);
    TEST("ADC", "WORD [SI], 5678H",          0x81, 0024, 0x78, 0x56);
    TEST("ADC", "WORD [1234H], 5678H",       0x81, 0026, 0x34, 0x12, 0x78, 0x56);
    TEST("ADC", "WORD [DI+52], 5678H",       0x81, 0125, 0x34, 0x78, 0x56);
    TEST("ADC", "WORD [BP+1234H], 5678H",    0x81, 0226, 0x34, 0x12, 0x78, 0x56);
    TEST("ADC", "WORD [BX+SI], 5678H",       0x81, 0020, 0x78, 0x56);
    TEST("ADC", "WORD [BX+DI-52], 5678H",    0x81, 0121, 0xCC, 0x78, 0x56);
    TEST("ADC", "WORD [BP+SI+89ABH], 5678H", 0x81, 0222, 0xAB, 0x89, 0x78, 0x56);
    // ADC r/m16,imm8
    TEST("ADC", "AX, 86",                 0x83, 0320, 0x56);
    TEST("ADC", "AX, -16",                0x83, 0320, 0xF0);
    TEST("ADC", "CX, 86",                 0x83, 0321, 0x56);
    TEST("ADC", "CX, -16",                0x83, 0321, 0xF0);
    TEST("ADC", "WORD [SI], 86",          0x83, 0024, 0x56);
    TEST("ADC", "WORD [1234H], 86",       0x83, 0026, 0x34, 0x12, 0x56);
    TEST("ADC", "WORD [DI+52], 86",       0x83, 0125, 0x34, 0x56);
    TEST("ADC", "WORD [BP+1234H], 86",    0x83, 0226, 0x34, 0x12, 0x56);
    TEST("ADC", "WORD [BX+SI], 86",       0x83, 0020, 0x56);
    TEST("ADC", "WORD [BX+DI-52], 86",    0x83, 0121, 0xCC, 0x56);
    TEST("ADC", "WORD [BP+SI+89ABH], 86", 0x83, 0222, 0xAB, 0x89, 0x56);
    if (is80386()) {
        // ADC r/m32,imm32
        TEST("ADC", "EAX, 12345678H",                 DATA32, 0x81, 0320, 0x78, 0x56, 0x34, 0x12);
        TEST("ADC", "ECX, 12345678H",                 DATA32, 0x81, 0321, 0x78, 0x56, 0x34, 0x12);
        TEST("ADC", "DWORD [SI], 12345678H",          DATA32, 0x81, 0024, 0x78, 0x56, 0x34, 0x12);
        TEST("ADC", "DWORD [1234H], 12345678H",       DATA32, 0x81, 0026, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12);
        TEST("ADC", "DWORD [DI+52], 12345678H",       DATA32, 0x81, 0125, 0x34, 0x78, 0x56, 0x34, 0x12);
        TEST("ADC", "DWORD [BP+1234H], 12345678H",    DATA32, 0x81, 0226, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12);
        TEST("ADC", "DWORD [BX+SI], 12345678H",       DATA32, 0x81, 0020, 0x78, 0x56, 0x34, 0x12);
        TEST("ADC", "DWORD [BX+DI-52], 12345678H",    DATA32, 0x81, 0121, 0xCC, 0x78, 0x56, 0x34, 0x12);
        TEST("ADC", "DWORD [BP+SI+89ABH], 12345678H", DATA32, 0x81, 0222, 0xAB, 0x89, 0x78, 0x56, 0x34, 0x12);
        // ADC r/m32,imm8
        TEST("ADC", "EAX, 86",                   DATA32, 0x83, 0320, 0x56);
        TEST("ADC", "EAX, -16",                  DATA32, 0x83, 0320, 0xF0);
        TEST("ADC", "ECX, 86",                   DATA32, 0x83, 0321, 0x56);
        TEST("ADC", "ECX, -16",                  DATA32, 0x83, 0321, 0xF0);
        TEST("ADC", "DWORD [SI], 86",            DATA32, 0x83, 0024, 0x56);
        TEST("ADC", "DWORD [1234H], 86",         DATA32, 0x83, 0026, 0x34, 0x12, 0x56);
        TEST("ADC", "DWORD [DI+52], 86",         DATA32, 0x83, 0125, 0x34, 0x56);
        TEST("ADC", "DWORD [BP+1234H], 86",      DATA32, 0x83, 0226, 0x34, 0x12, 0x56);
        TEST("ADC", "DWORD [BX+SI], 86",         DATA32, 0x83, 0020, 0x56);
        TEST("ADC", "DWORD [BX+DI-52], 86",      DATA32, 0x83, 0121, 0xCC, 0x56);
        TEST("ADC", "DWORD [BP+SI+89ABH], 86",   DATA32, 0x83, 0222, 0xAB, 0x89, 0x56);
    }

    // ADC AL,imm8
    TEST("ADC", "AL, 86",   0x14, 0x56);
    // ADC AX,imm16
    TEST("ADC", "AX, 5678H", 0x15, 0x78, 0x56);
    if (is80386()) {
        // ADC EAX,imm32
        TEST("ADC", "EAX, 12345678H", DATA32, 0x15, 0x78, 0x56, 0x34, 0x12);
    }

    // INC r/m8
    TEST("INC", "CH",                 0xFE, 0305);
    TEST("INC", "BYTE [SI]",          0xFE, 0004);
    TEST("INC", "BYTE [1234H]",       0xFE, 0006, 0x34, 0x12);
    TEST("INC", "BYTE [DI-52]",       0xFE, 0105, 0xCC);
    TEST("INC", "BYTE [BP+1234H]",    0xFE, 0206, 0x34, 0x12);
    TEST("INC", "BYTE [BX+SI]",       0xFE, 0000);
    TEST("INC", "BYTE [BX+DI+52]",    0xFE, 0101, 0x34);
    TEST("INC", "BYTE [BP+SI+1234H]", 0xFE, 0202, 0x34, 0x12);
    // INC r/m16
    TEST("INC", "BP",                 0xFF, 0305);
    TEST("INC", "WORD [SI]",          0xFF, 0004);
    TEST("INC", "WORD [1234H]",       0xFF, 0006, 0x34, 0x12);
    TEST("INC", "WORD [DI-52]",       0xFF, 0105, 0xCC);
    TEST("INC", "WORD [BP+1234H]",    0xFF, 0206, 0x34, 0x12);
    TEST("INC", "WORD [BX+SI]",       0xFF, 0000);
    TEST("INC", "WORD [BX+DI+52]",    0xFF, 0101, 0x34);
    TEST("INC", "WORD [BP+SI+1234H]", 0xFF, 0202, 0x34, 0x12);
    if (is80386()) {
        // INC r/m32
        TEST("INC", "EBP",                 DATA32, 0xFF, 0305);
        TEST("INC", "DWORD [SI]",          DATA32, 0xFF, 0004);
        TEST("INC", "DWORD [1234H]",       DATA32, 0xFF, 0006, 0x34, 0x12);
        TEST("INC", "DWORD [DI-52]",       DATA32, 0xFF, 0105, 0xCC);
        TEST("INC", "DWORD [BP+1234H]",    DATA32, 0xFF, 0206, 0x34, 0x12);
        TEST("INC", "DWORD [BX+SI]",       DATA32, 0xFF, 0000);
        TEST("INC", "DWORD [BX+DI+52]",    DATA32, 0xFF, 0101, 0x34);
        TEST("INC", "DWORD [BP+SI+1234H]", DATA32, 0xFF, 0202, 0x34, 0x12);
    }

    // INC r16
    TEST("INC", "AX", 0100);
    TEST("INC", "CX", 0101);
    TEST("INC", "DX", 0102);
    TEST("INC", "BX", 0103);
    TEST("INC", "SP", 0104);
    TEST("INC", "BP", 0105);
    TEST("INC", "SI", 0106);
    TEST("INC", "DI", 0107);
    if (is80386()) {
        // INC r32
        TEST("INC", "EAX", DATA32, 0100);
        TEST("INC", "ECX", DATA32, 0101);
        TEST("INC", "EDX", DATA32, 0102);
        TEST("INC", "EBX", DATA32, 0103);
        TEST("INC", "ESP", DATA32, 0104);
        TEST("INC", "EBP", DATA32, 0105);
        TEST("INC", "ESI", DATA32, 0106);
        TEST("INC", "EDI", DATA32, 0107);
    }

    TEST("AAA", "", 0x37);
    TEST("DAA", "", 0x27);

    // SUB r/m8,r8
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
    // SUB r8,r/m8
    TEST("SUB", "DH, [SI]",          0x2A, 0064);
    TEST("SUB", "BH, [1234H]",       0x2A, 0076, 0x34, 0x12);
    TEST("SUB", "AL, [DI-52]",       0x2A, 0105, 0xCC);
    TEST("SUB", "CL, [BP+1234H]",    0x2A, 0216, 0x34, 0x12);
    TEST("SUB", "DL, [BX+SI]",       0x2A, 0020);
    TEST("SUB", "BL, [BX+DI-52]",    0x2A, 0131, 0xCC);
    TEST("SUB", "AH, [BP+SI+1234H]", 0x2A, 0242, 0x34, 0x12);
    // SUB r/m16,r16
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
    // SUB r16,r/m16
    TEST("SUB", "SI, [SI]",          0x2B, 0064);
    TEST("SUB", "DI, [1234H]",       0x2B, 0076, 0x34, 0x12);
    TEST("SUB", "AX, [DI+52]",       0x2B, 0105, 0x34);
    TEST("SUB", "CX, [BP+1234H]",    0x2B, 0216, 0x34, 0x12);
    TEST("SUB", "DX, [BX+SI]",       0x2B, 0020);
    TEST("SUB", "BX, [BX+DI+52]",    0x2B, 0131, 0x34);
    TEST("SUB", "SP, [BP+SI+1234H]", 0x2B, 0242, 0x34, 0x12);
    if (is80386()) {
        // SUB r/m32,r32
        TEST("SUB", "EAX, ECX",           DATA32, 0x29, 0310);
        TEST("SUB", "EDX, EBX",           DATA32, 0x29, 0332);
        TEST("SUB", "ESP, EBP",           DATA32, 0x29, 0354);
        TEST("SUB", "[SI], ESI",          DATA32, 0x29, 0064);
        TEST("SUB", "[1234H], EDI",       DATA32, 0x29, 0076, 0x34, 0x12);
        TEST("SUB", "[DI-52], EAX",       DATA32, 0x29, 0105, 0xCC);
        TEST("SUB", "[BP+1234H], ECX",    DATA32, 0x29, 0216, 0x34, 0x12);
        TEST("SUB", "[BX+SI], EDX",       DATA32, 0x29, 0020);
        TEST("SUB", "[BX+DI+52], EBX",    DATA32, 0x29, 0131, 0x34);
        TEST("SUB", "[BP+SI+1234H], ESP", DATA32, 0x29, 0242, 0x34, 0x12);
        // SUB r32,r/m32
        TEST("SUB", "ESI, [SI]",          DATA32, 0x2B, 0064);
        TEST("SUB", "EDI, [1234H]",       DATA32, 0x2B, 0076, 0x34, 0x12);
        TEST("SUB", "EAX, [DI+52]",       DATA32, 0x2B, 0105, 0x34);
        TEST("SUB", "ECX, [BP+1234H]",    DATA32, 0x2B, 0216, 0x34, 0x12);
        TEST("SUB", "EDX, [BX+SI]",       DATA32, 0x2B, 0020);
        TEST("SUB", "EBX, [BX+DI+52]",    DATA32, 0x2B, 0131, 0x34);
        TEST("SUB", "ESP, [BP+SI+1234H]", DATA32, 0x2B, 0242, 0x34, 0x12);
    }
    // SUB AL,imm8
    TEST("SUB", "AL, 86",                 0x80, 0350, 0x56);
    TEST("SUB", "CL, 86",                 0x80, 0351, 0x56);
    TEST("SUB", "BYTE [SI], 86",          0x80, 0054, 0x56);
    TEST("SUB", "BYTE [89ABH], 86",       0x80, 0056, 0xAB, 0x89, 0x56);
    TEST("SUB", "BYTE [DI-52], 86",       0x80, 0155, 0xCC, 0x56);
    TEST("SUB", "BYTE [BP+89ABH], 86",    0x80, 0256, 0xAB, 0x89, 0x56);
    TEST("SUB", "BYTE [BX+SI], 86",       0x80, 0050, 0x56);
    TEST("SUB", "BYTE [BX+DI+52], 86",    0x80, 0151, 0x34, 0x56);
    TEST("SUB", "BYTE [BP+SI+1234H], 86", 0x80, 0252, 0x34, 0x12, 0x56);
    // SUB AX,imm16
    TEST("SUB", "AX, 5678H",                 0x81, 0350, 0x78, 0x56);
    TEST("SUB", "CX, 5678H",                 0x81, 0351, 0x78, 0x56);
    TEST("SUB", "WORD [SI], 5678H",          0x81, 0054, 0x78, 0x56);
    TEST("SUB", "WORD [1234H], 5678H",       0x81, 0056, 0x34, 0x12, 0x78, 0x56);
    TEST("SUB", "WORD [DI+52], 5678H",       0x81, 0155, 0x34, 0x78, 0x56);
    TEST("SUB", "WORD [BP+1234H], 5678H",    0x81, 0256, 0x34, 0x12, 0x78, 0x56);
    TEST("SUB", "WORD [BX+SI], 5678H",       0x81, 0050, 0x78, 0x56);
    TEST("SUB", "WORD [BX+DI-52], 5678H",    0x81, 0151, 0xCC, 0x78, 0x56);
    TEST("SUB", "WORD [BP+SI+89ABH], 5678H", 0x81, 0252, 0xAB, 0x89, 0x78, 0x56);
    // SUB AX,imm8
    TEST("SUB", "AX, 86",                 0x83, 0350, 0x56);
    TEST("SUB", "AX, -16",                0x83, 0350, 0xF0);
    TEST("SUB", "CX, 86",                 0x83, 0351, 0x56);
    TEST("SUB", "CX, -16",                0x83, 0351, 0xF0);
    TEST("SUB", "WORD [SI], 86",          0x83, 0054, 0x56);
    TEST("SUB", "WORD [1234H], 86",       0x83, 0056, 0x34, 0x12, 0x56);
    TEST("SUB", "WORD [DI+52], 86",       0x83, 0155, 0x34, 0x56);
    TEST("SUB", "WORD [BP+1234H], 86",    0x83, 0256, 0x34, 0x12, 0x56);
    TEST("SUB", "WORD [BX+SI], 86",       0x83, 0050, 0x56);
    TEST("SUB", "WORD [BX+DI-52], 86",    0x83, 0151, 0xCC, 0x56);
    TEST("SUB", "WORD [BP+SI+89ABH], 86", 0x83, 0252, 0xAB, 0x89, 0x56);
    if (is80386()) {
        // SUB EAX,imm32
        TEST("SUB", "EAX, 12345678H",                 DATA32, 0x81, 0350, 0x78, 0x56, 0x34, 0x12);
        TEST("SUB", "ECX, 12345678H",                 DATA32, 0x81, 0351, 0x78, 0x56, 0x34, 0x12);
        TEST("SUB", "DWORD [SI], 12345678H",          DATA32, 0x81, 0054, 0x78, 0x56, 0x34, 0x12);
        TEST("SUB", "DWORD [1234H], 12345678H",       DATA32, 0x81, 0056, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12);
        TEST("SUB", "DWORD [DI+52], 12345678H",       DATA32, 0x81, 0155, 0x34, 0x78, 0x56, 0x34, 0x12);
        TEST("SUB", "DWORD [BP+1234H], 12345678H",    DATA32, 0x81, 0256, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12);
        TEST("SUB", "DWORD [BX+SI], 12345678H",       DATA32, 0x81, 0050, 0x78, 0x56, 0x34, 0x12);
        TEST("SUB", "DWORD [BX+DI-52], 12345678H",    DATA32, 0x81, 0151, 0xCC, 0x78, 0x56, 0x34, 0x12);
        TEST("SUB", "DWORD [BP+SI+89ABH], 12345678H", DATA32, 0x81, 0252, 0xAB, 0x89, 0x78, 0x56, 0x34, 0x12);
        // SUB EAX,imm8
        TEST("SUB", "EAX, 86",                 DATA32, 0x83, 0350, 0x56);
        TEST("SUB", "EAX, -16",                DATA32, 0x83, 0350, 0xF0);
        TEST("SUB", "ECX, 86",                 DATA32, 0x83, 0351, 0x56);
        TEST("SUB", "ECX, -16",                DATA32, 0x83, 0351, 0xF0);
        TEST("SUB", "DWORD [SI], 86",          DATA32, 0x83, 0054, 0x56);
        TEST("SUB", "DWORD [1234H], 86",       DATA32, 0x83, 0056, 0x34, 0x12, 0x56);
        TEST("SUB", "DWORD [DI+52], 86",       DATA32, 0x83, 0155, 0x34, 0x56);
        TEST("SUB", "DWORD [BP+1234H], 86",    DATA32, 0x83, 0256, 0x34, 0x12, 0x56);
        TEST("SUB", "DWORD [BX+SI], 86",       DATA32, 0x83, 0050, 0x56);
        TEST("SUB", "DWORD [BX+DI-52], 86",    DATA32, 0x83, 0151, 0xCC, 0x56);
        TEST("SUB", "DWORD [BP+SI+89ABH], 86", DATA32, 0x83, 0252, 0xAB, 0x89, 0x56);
    }

    // SUB AL,imm8
    TEST("SUB", "AL, 86",   0x2C, 0x56);
    // SUB AX,imm16
    TEST("SUB", "AX, 5678H", 0x2D, 0x78, 0x56);
    if (is80386()) {
        // SUB EAX,imm32
        TEST("SUB", "EAX, 12345678H", DATA32, 0x2D, 0x78, 0x56, 0x34, 0x12);
    }

    // SBB r/m8,r8
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
    // SBB r8, r/m8
    TEST("SBB", "DH, [SI]",          0x1A, 0064);
    TEST("SBB", "BH, [1234H]",       0x1A, 0076, 0x34, 0x12);
    TEST("SBB", "AL, [DI-52]",       0x1A, 0105, 0xCC);
    TEST("SBB", "CL, [BP+1234H]",    0x1A, 0216, 0x34, 0x12);
    TEST("SBB", "DL, [BX+SI]",       0x1A, 0020);
    TEST("SBB", "BL, [BX+DI-52]",    0x1A, 0131, 0xCC);
    TEST("SBB", "AH, [BP+SI+1234H]", 0x1A, 0242, 0x34, 0x12);
    // SBB r/m16,r16
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
    // SBB r16,r/m16
    TEST("SBB", "SI, [SI]",          0x1B, 0064);
    TEST("SBB", "DI, [1234H]",       0x1B, 0076, 0x34, 0x12);
    TEST("SBB", "AX, [DI+52]",       0x1B, 0105, 0x34);
    TEST("SBB", "CX, [BP+1234H]",    0x1B, 0216, 0x34, 0x12);
    TEST("SBB", "DX, [BX+SI]",       0x1B, 0020);
    TEST("SBB", "BX, [BX+DI+52]",    0x1B, 0131, 0x34);
    TEST("SBB", "SP, [BP+SI+1234H]", 0x1B, 0242, 0x34, 0x12);
    if (is80386()) {
        // SBB r/m32,r32
        TEST("SBB", "EAX, ECX",           DATA32, 0x19, 0310);
        TEST("SBB", "EDX, EBX",           DATA32, 0x19, 0332);
        TEST("SBB", "ESP, EBP",           DATA32, 0x19, 0354);
        TEST("SBB", "[SI], ESI",          DATA32, 0x19, 0064);
        TEST("SBB", "[1234H], EDI",       DATA32, 0x19, 0076, 0x34, 0x12);
        TEST("SBB", "[DI-52], EAX",       DATA32, 0x19, 0105, 0xCC);
        TEST("SBB", "[BP+1234H], ECX",    DATA32, 0x19, 0216, 0x34, 0x12);
        TEST("SBB", "[BX+SI], EDX",       DATA32, 0x19, 0020);
        TEST("SBB", "[BX+DI+52], EBX",    DATA32, 0x19, 0131, 0x34);
        TEST("SBB", "[BP+SI+1234H], ESP", DATA32, 0x19, 0242, 0x34, 0x12);
        // SBB r32,r/m32
        TEST("SBB", "ESI, [SI]",          DATA32, 0x1B, 0064);
        TEST("SBB", "EDI, [1234H]",       DATA32, 0x1B, 0076, 0x34, 0x12);
        TEST("SBB", "EAX, [DI+52]",       DATA32, 0x1B, 0105, 0x34);
        TEST("SBB", "ECX, [BP+1234H]",    DATA32, 0x1B, 0216, 0x34, 0x12);
        TEST("SBB", "EDX, [BX+SI]",       DATA32, 0x1B, 0020);
        TEST("SBB", "EBX, [BX+DI+52]",    DATA32, 0x1B, 0131, 0x34);
        TEST("SBB", "ESP, [BP+SI+1234H]", DATA32, 0x1B, 0242, 0x34, 0x12);
    }

    // SBB AL,imm8
    TEST("SBB", "AL, 86",                 0x80, 0330, 0x56);
    TEST("SBB", "CL, 86",                 0x80, 0331, 0x56);
    TEST("SBB", "BYTE [SI], 86",          0x80, 0034, 0x56);
    TEST("SBB", "BYTE [89ABH], 86",       0x80, 0036, 0xAB, 0x89, 0x56);
    TEST("SBB", "BYTE [DI-52], 86",       0x80, 0135, 0xCC, 0x56);
    TEST("SBB", "BYTE [BP+89ABH], 86",    0x80, 0236, 0xAB, 0x89, 0x56);
    TEST("SBB", "BYTE [BX+SI], 86",       0x80, 0030, 0x56);
    TEST("SBB", "BYTE [BX+DI+52], 86",    0x80, 0131, 0x34, 0x56);
    TEST("SBB", "BYTE [BP+SI+1234H], 86", 0x80, 0232, 0x34, 0x12, 0x56);
    // SBB AX, imm16
    TEST("SBB", "AX, 5678H",                 0x81, 0330, 0x78, 0x56);
    TEST("SBB", "CX, 5678H",                 0x81, 0331, 0x78, 0x56);
    TEST("SBB", "WORD [SI], 5678H",          0x81, 0034, 0x78, 0x56);
    TEST("SBB", "WORD [1234H], 5678H",       0x81, 0036, 0x34, 0x12, 0x78, 0x56);
    TEST("SBB", "WORD [DI+52], 5678H",       0x81, 0135, 0x34, 0x78, 0x56);
    TEST("SBB", "WORD [BP+1234H], 5678H",    0x81, 0236, 0x34, 0x12, 0x78, 0x56);
    TEST("SBB", "WORD [BX+SI], 5678H",       0x81, 0030, 0x78, 0x56);
    TEST("SBB", "WORD [BX+DI-52], 5678H",    0x81, 0131, 0xCC, 0x78, 0x56);
    TEST("SBB", "WORD [BP+SI+89ABH], 5678H", 0x81, 0232, 0xAB, 0x89, 0x78, 0x56);
    // SBB AX,imm8
    TEST("SBB", "AX, 86",                 0x83, 0330, 0x56);
    TEST("SBB", "AX, -16",                0x83, 0330, 0xF0);
    TEST("SBB", "CX, 86",                 0x83, 0331, 0x56);
    TEST("SBB", "CX, -16",                0x83, 0331, 0xF0);
    TEST("SBB", "WORD [SI], 86",          0x83, 0034, 0x56);
    TEST("SBB", "WORD [1234H], 86",       0x83, 0036, 0x34, 0x12, 0x56);
    TEST("SBB", "WORD [DI+52], 86",       0x83, 0135, 0x34, 0x56);
    TEST("SBB", "WORD [BP+1234H], 86",    0x83, 0236, 0x34, 0x12, 0x56);
    TEST("SBB", "WORD [BX+SI], 86",       0x83, 0030, 0x56);
    TEST("SBB", "WORD [BX+DI-52], 86",    0x83, 0131, 0xCC, 0x56);
    TEST("SBB", "WORD [BP+SI+89ABH], 86", 0x83, 0232, 0xAB, 0x89, 0x56);
    if (is80386()) {
        // SBB EAX,imm32
        TEST("SBB", "EAX, 12345678H",                 DATA32, 0x81, 0330, 0x78, 0x56, 0x34, 0x12);
        TEST("SBB", "ECX, 12345678H",                 DATA32, 0x81, 0331, 0x78, 0x56, 0x34, 0x12);
        TEST("SBB", "DWORD [SI], 12345678H",          DATA32, 0x81, 0034, 0x78, 0x56, 0x34, 0x12);
        TEST("SBB", "DWORD [1234H], 12345678H",       DATA32, 0x81, 0036, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12);
        TEST("SBB", "DWORD [DI+52], 12345678H",       DATA32, 0x81, 0135, 0x34, 0x78, 0x56, 0x34, 0x12);
        TEST("SBB", "DWORD [BP+1234H], 12345678H",    DATA32, 0x81, 0236, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12);
        TEST("SBB", "DWORD [BX+SI], 12345678H",       DATA32, 0x81, 0030, 0x78, 0x56, 0x34, 0x12);
        TEST("SBB", "DWORD [BX+DI-52], 12345678H",    DATA32, 0x81, 0131, 0xCC, 0x78, 0x56, 0x34, 0x12);
        TEST("SBB", "DWORD [BP+SI+89ABH], 12345678H", DATA32, 0x81, 0232, 0xAB, 0x89, 0x78, 0x56, 0x34, 0x12);
        // SBB EAX,imm8
        TEST("SBB", "EAX, 86",                 DATA32, 0x83, 0330, 0x56);
        TEST("SBB", "EAX, -16",                DATA32, 0x83, 0330, 0xF0);
        TEST("SBB", "ECX, 86",                 DATA32, 0x83, 0331, 0x56);
        TEST("SBB", "ECX, -16",                DATA32, 0x83, 0331, 0xF0);
        TEST("SBB", "DWORD [SI], 86",          DATA32, 0x83, 0034, 0x56);
        TEST("SBB", "DWORD [1234H], 86",       DATA32, 0x83, 0036, 0x34, 0x12, 0x56);
        TEST("SBB", "DWORD [DI+52], 86",       DATA32, 0x83, 0135, 0x34, 0x56);
        TEST("SBB", "DWORD [BP+1234H], 86",    DATA32, 0x83, 0236, 0x34, 0x12, 0x56);
        TEST("SBB", "DWORD [BX+SI], 86",       DATA32, 0x83, 0030, 0x56);
        TEST("SBB", "DWORD [BX+DI-52], 86",    DATA32, 0x83, 0131, 0xCC, 0x56);
        TEST("SBB", "DWORD [BP+SI+89ABH], 86", DATA32, 0x83, 0232, 0xAB, 0x89, 0x56);
    }

    // SBB AL,imm8
    TEST("SBB", "AL, 86",   0x1C, 0x56);
    // SBB AX,imm16
    TEST("SBB", "AX, 5678H", 0x1D, 0x78, 0x56);
    if (is80386()) {
        // SBB EAX,imm32
        TEST("SBB", "EAX, 12345678H", DATA32, 0x1D, 0x78, 0x56, 0x34, 0x12);
    }

    // DEC r/m8
    TEST("DEC", "CH",                 0xFE, 0315);
    TEST("DEC", "BYTE [SI]",          0xFE, 0014);
    TEST("DEC", "BYTE [1234H]",       0xFE, 0016, 0x34, 0x12);
    TEST("DEC", "BYTE [DI-52]",       0xFE, 0115, 0xCC);
    TEST("DEC", "BYTE [BP+1234H]",    0xFE, 0216, 0x34, 0x12);
    TEST("DEC", "BYTE [BX+SI]",       0xFE, 0010);
    TEST("DEC", "BYTE [BX+DI+52]",    0xFE, 0111, 0x34);
    TEST("DEC", "BYTE [BP+SI+1234H]", 0xFE, 0212, 0x34, 0x12);
    // DEC r/m16
    TEST("DEC", "BP",                 0xFF, 0315);
    TEST("DEC", "WORD [SI]",          0xFF, 0014);
    TEST("DEC", "WORD [1234H]",       0xFF, 0016, 0x34, 0x12);
    TEST("DEC", "WORD [DI-52]",       0xFF, 0115, 0xCC);
    TEST("DEC", "WORD [BP+1234H]",    0xFF, 0216, 0x34, 0x12);
    TEST("DEC", "WORD [BX+SI]",       0xFF, 0010);
    TEST("DEC", "WORD [BX+DI+52]",    0xFF, 0111, 0x34);
    TEST("DEC", "WORD [BP+SI+1234H]", 0xFF, 0212, 0x34, 0x12);
    if (is80386()) {
        // DEC r/m32
        TEST("DEC", "EBP",                 DATA32, 0xFF, 0315);
        TEST("DEC", "DWORD [SI]",          DATA32, 0xFF, 0014);
        TEST("DEC", "DWORD [1234H]",       DATA32, 0xFF, 0016, 0x34, 0x12);
        TEST("DEC", "DWORD [DI-52]",       DATA32, 0xFF, 0115, 0xCC);
        TEST("DEC", "DWORD [BP+1234H]",    DATA32, 0xFF, 0216, 0x34, 0x12);
        TEST("DEC", "DWORD [BX+SI]",       DATA32, 0xFF, 0010);
        TEST("DEC", "DWORD [BX+DI+52]",    DATA32, 0xFF, 0111, 0x34);
        TEST("DEC", "DWORD [BP+SI+1234H]", DATA32, 0xFF, 0212, 0x34, 0x12);
    }

    // DEC r16
    TEST("DEC", "AX", 0110);
    TEST("DEC", "CX", 0111);
    TEST("DEC", "DX", 0112);
    TEST("DEC", "BX", 0113);
    TEST("DEC", "SP", 0114);
    TEST("DEC", "BP", 0115);
    TEST("DEC", "SI", 0116);
    TEST("DEC", "DI", 0117);
    if (is80386()) {
        // DEC r32
        TEST("DEC", "EAX", DATA32, 0110);
        TEST("DEC", "ECX", DATA32, 0111);
        TEST("DEC", "EDX", DATA32, 0112);
        TEST("DEC", "EBX", DATA32, 0113);
        TEST("DEC", "ESP", DATA32, 0114);
        TEST("DEC", "EBP", DATA32, 0115);
        TEST("DEC", "ESI", DATA32, 0116);
        TEST("DEC", "EDI", DATA32, 0117);
    }

    // NEG r/m8
    TEST("NEG", "CH",                 0xF6, 0335);
    TEST("NEG", "BYTE [SI]",          0xF6, 0034);
    TEST("NEG", "BYTE [1234H]",       0xF6, 0036, 0x34, 0x12);
    TEST("NEG", "BYTE [DI-52]",       0xF6, 0135, 0xCC);
    TEST("NEG", "BYTE [BP+1234H]",    0xF6, 0236, 0x34, 0x12);
    TEST("NEG", "BYTE [BX+SI]",       0xF6, 0030);
    TEST("NEG", "BYTE [BX+DI+52]",    0xF6, 0131, 0x34);
    TEST("NEG", "BYTE [BP+SI+1234H]", 0xF6, 0232, 0x34, 0x12);
    // NEG r/m16
    TEST("NEG", "BP",                 0xF7, 0335);
    TEST("NEG", "WORD [SI]",          0xF7, 0034);
    TEST("NEG", "WORD [1234H]",       0xF7, 0036, 0x34, 0x12);
    TEST("NEG", "WORD [DI-52]",       0xF7, 0135, 0xCC);
    TEST("NEG", "WORD [BP+1234H]",    0xF7, 0236, 0x34, 0x12);
    TEST("NEG", "WORD [BX+SI]",       0xF7, 0030);
    TEST("NEG", "WORD [BX+DI+52]",    0xF7, 0131, 0x34);
    TEST("NEG", "WORD [BP+SI+1234H]", 0xF7, 0232, 0x34, 0x12);
    if (is80386()) {
        // NEG r/m32
        TEST("NEG", "EBP",                 DATA32, 0xF7, 0335);
        TEST("NEG", "DWORD [SI]",          DATA32, 0xF7, 0034);
        TEST("NEG", "DWORD [1234H]",       DATA32, 0xF7, 0036, 0x34, 0x12);
        TEST("NEG", "DWORD [DI-52]",       DATA32, 0xF7, 0135, 0xCC);
        TEST("NEG", "DWORD [BP+1234H]",    DATA32, 0xF7, 0236, 0x34, 0x12);
        TEST("NEG", "DWORD [BX+SI]",       DATA32, 0xF7, 0030);
        TEST("NEG", "DWORD [BX+DI+52]",    DATA32, 0xF7, 0131, 0x34);
        TEST("NEG", "DWORD [BP+SI+1234H]", DATA32, 0xF7, 0232, 0x34, 0x12);
    }

    // CMP r/m8,r8
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
    // CMP r8,r/m8
    TEST("CMP", "DH, [SI]",          0x3A, 0064);
    TEST("CMP", "BH, [1234H]",       0x3A, 0076, 0x34, 0x12);
    TEST("CMP", "AL, [DI-52]",       0x3A, 0105, 0xCC);
    TEST("CMP", "CL, [BP+1234H]",    0x3A, 0216, 0x34, 0x12);
    TEST("CMP", "DL, [BX+SI]",       0x3A, 0020);
    TEST("CMP", "BL, [BX+DI-52]",    0x3A, 0131, 0xCC);
    TEST("CMP", "AH, [BP+SI+1234H]", 0x3A, 0242, 0x34, 0x12);
    // CMP r/m16,r16
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
    // CMP r16,r/m16
    TEST("CMP", "SI, [SI]",          0x3B, 0064);
    TEST("CMP", "DI, [1234H]",       0x3B, 0076, 0x34, 0x12);
    TEST("CMP", "AX, [DI+52]",       0x3B, 0105, 0x34);
    TEST("CMP", "CX, [BP+1234H]",    0x3B, 0216, 0x34, 0x12);
    TEST("CMP", "DX, [BX+SI]",       0x3B, 0020);
    TEST("CMP", "BX, [BX+DI+52]",    0x3B, 0131, 0x34);
    TEST("CMP", "SP, [BP+SI+1234H]", 0x3B, 0242, 0x34, 0x12);
    if (is80386()) {
        // CMP r/m32,r32
        TEST("CMP", "EAX, ECX",           DATA32, 0x39, 0310);
        TEST("CMP", "EDX, EBX",           DATA32, 0x39, 0332);
        TEST("CMP", "ESP, EBP",           DATA32, 0x39, 0354);
        TEST("CMP", "[SI], ESI",          DATA32, 0x39, 0064);
        TEST("CMP", "[1234H], EDI",       DATA32, 0x39, 0076, 0x34, 0x12);
        TEST("CMP", "[DI-52], EAX",       DATA32, 0x39, 0105, 0xCC);
        TEST("CMP", "[BP+1234H], ECX",    DATA32, 0x39, 0216, 0x34, 0x12);
        TEST("CMP", "[BX+SI], EDX",       DATA32, 0x39, 0020);
        TEST("CMP", "[BX+DI+52], EBX",    DATA32, 0x39, 0131, 0x34);
        TEST("CMP", "[BP+SI+1234H], ESP", DATA32, 0x39, 0242, 0x34, 0x12);
        // CMP r32,r/m32
        TEST("CMP", "ESI, [SI]",          DATA32, 0x3B, 0064);
        TEST("CMP", "EDI, [1234H]",       DATA32, 0x3B, 0076, 0x34, 0x12);
        TEST("CMP", "EAX, [DI+52]",       DATA32, 0x3B, 0105, 0x34);
        TEST("CMP", "ECX, [BP+1234H]",    DATA32, 0x3B, 0216, 0x34, 0x12);
        TEST("CMP", "EDX, [BX+SI]",       DATA32, 0x3B, 0020);
        TEST("CMP", "EBX, [BX+DI+52]",    DATA32, 0x3B, 0131, 0x34);
        TEST("CMP", "ESP, [BP+SI+1234H]", DATA32, 0x3B, 0242, 0x34, 0x12);
    }

    // CMP r/m8,imm8
    TEST("CMP", "AL, 86",                 0x80, 0370, 0x56);
    TEST("CMP", "CL, 86",                 0x80, 0371, 0x56);
    TEST("CMP", "BYTE [SI], 86",          0x80, 0074, 0x56);
    TEST("CMP", "BYTE [89ABH], 86",       0x80, 0076, 0xAB, 0x89, 0x56);
    TEST("CMP", "BYTE [DI-52], 86",       0x80, 0175, 0xCC, 0x56);
    TEST("CMP", "BYTE [BP+89ABH], 86",    0x80, 0276, 0xAB, 0x89, 0x56);
    TEST("CMP", "BYTE [BX+SI], 86",       0x80, 0070, 0x56);
    TEST("CMP", "BYTE [BX+DI+52], 86",    0x80, 0171, 0x34, 0x56);
    TEST("CMP", "BYTE [BP+SI+1234H], 86", 0x80, 0272, 0x34, 0x12, 0x56);
    // CMP r/m16,imm16
    TEST("CMP", "AX, 5678H",                 0x81, 0370, 0x78, 0x56);
    TEST("CMP", "CX, 5678H",                 0x81, 0371, 0x78, 0x56);
    TEST("CMP", "WORD [SI], 5678H",          0x81, 0074, 0x78, 0x56);
    TEST("CMP", "WORD [1234H], 5678H",       0x81, 0076, 0x34, 0x12, 0x78, 0x56);
    TEST("CMP", "WORD [DI+52], 5678H",       0x81, 0175, 0x34, 0x78, 0x56);
    TEST("CMP", "WORD [BP+1234H], 5678H",    0x81, 0276, 0x34, 0x12, 0x78, 0x56);
    TEST("CMP", "WORD [BX+SI], 5678H",       0x81, 0070, 0x78, 0x56);
    TEST("CMP", "WORD [BX+DI-52], 5678H",    0x81, 0171, 0xCC, 0x78, 0x56);
    TEST("CMP", "WORD [BP+SI+89ABH], 5678H", 0x81, 0272, 0xAB, 0x89, 0x78, 0x56);
    // CMP r/m16,imm8
    TEST("CMP", "AX, 86",                 0x83, 0370, 0x56);
    TEST("CMP", "AX, -16",                0x83, 0370, 0xF0);
    TEST("CMP", "CX, 86",                 0x83, 0371, 0x56);
    TEST("CMP", "CX, -16",                0x83, 0371, 0xF0);
    TEST("CMP", "WORD [SI], 86",          0x83, 0074, 0x56);
    TEST("CMP", "WORD [1234H], 86",       0x83, 0076, 0x34, 0x12, 0x56);
    TEST("CMP", "WORD [DI+52], 86",       0x83, 0175, 0x34, 0x56);
    TEST("CMP", "WORD [BP+1234H], 86",    0x83, 0276, 0x34, 0x12, 0x56);
    TEST("CMP", "WORD [BX+SI], 86",       0x83, 0070, 0x56);
    TEST("CMP", "WORD [BX+DI-52], 86",    0x83, 0171, 0xCC, 0x56);
    TEST("CMP", "WORD [BP+SI+89ABH], 86", 0x83, 0272, 0xAB, 0x89, 0x56);
    if (is80386()) {
        // CMP r/m32,imm32
        TEST("CMP", "EAX, 12345678H",                 DATA32, 0x81, 0370, 0x78, 0x56, 0x34, 0x12);
        TEST("CMP", "ECX, 12345678H",                 DATA32, 0x81, 0371, 0x78, 0x56, 0x34, 0x12);
        TEST("CMP", "DWORD [SI], 12345678H",          DATA32, 0x81, 0074, 0x78, 0x56, 0x34, 0x12);
        TEST("CMP", "DWORD [1234H], 12345678H",       DATA32, 0x81, 0076, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12);
        TEST("CMP", "DWORD [DI+52], 12345678H",       DATA32, 0x81, 0175, 0x34, 0x78, 0x56, 0x34, 0x12);
        TEST("CMP", "DWORD [BP+1234H], 12345678H",    DATA32, 0x81, 0276, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12);
        TEST("CMP", "DWORD [BX+SI], 12345678H",       DATA32, 0x81, 0070, 0x78, 0x56, 0x34, 0x12);
        TEST("CMP", "DWORD [BX+DI-52], 12345678H",    DATA32, 0x81, 0171, 0xCC, 0x78, 0x56, 0x34, 0x12);
        TEST("CMP", "DWORD [BP+SI+89ABH], 12345678H", DATA32, 0x81, 0272, 0xAB, 0x89, 0x78, 0x56, 0x34, 0x12);
        // CMP r/m32,imm8
        TEST("CMP", "EAX, 86",                 DATA32, 0x83, 0370, 0x56);
        TEST("CMP", "EAX, -16",                DATA32, 0x83, 0370, 0xF0);
        TEST("CMP", "ECX, 86",                 DATA32, 0x83, 0371, 0x56);
        TEST("CMP", "ECX, -16",                DATA32, 0x83, 0371, 0xF0);
        TEST("CMP", "DWORD [SI], 86",          DATA32, 0x83, 0074, 0x56);
        TEST("CMP", "DWORD [1234H], 86",       DATA32, 0x83, 0076, 0x34, 0x12, 0x56);
        TEST("CMP", "DWORD [DI+52], 86",       DATA32, 0x83, 0175, 0x34, 0x56);
        TEST("CMP", "DWORD [BP+1234H], 86",    DATA32, 0x83, 0276, 0x34, 0x12, 0x56);
        TEST("CMP", "DWORD [BX+SI], 86",       DATA32, 0x83, 0070, 0x56);
        TEST("CMP", "DWORD [BX+DI-52], 86",    DATA32, 0x83, 0171, 0xCC, 0x56);
        TEST("CMP", "DWORD [BP+SI+89ABH], 86", DATA32, 0x83, 0272, 0xAB, 0x89, 0x56);
    }

    // CMP AL,imm8
    TEST("CMP", "AL, 86",   0x3C, 0x56);
    // CMP AX,imm16
    TEST("CMP", "AX, 5678H", 0x3D, 0x78, 0x56);
    if (is80386()) {
        // CMP EAX,imm32
        TEST("CMP", "EAX, 12345678H", DATA32, 0x3D, 0x78, 0x56, 0x34, 0x12);
    }

    TEST("AAS", "", 0x3F);
    TEST("DAS", "", 0x2F);

    // MUL AL,r/m8; AL*(r/m8)=AX
    TEST("MUL", "CH",                 0xF6, 0345);
    TEST("MUL", "BYTE [SI]",          0xF6, 0044);
    TEST("MUL", "BYTE [1234H]",       0xF6, 0046, 0x34, 0x12);
    TEST("MUL", "BYTE [DI-52]",       0xF6, 0145, 0xCC);
    TEST("MUL", "BYTE [BP+1234H]",    0xF6, 0246, 0x34, 0x12);
    TEST("MUL", "BYTE [BX+SI]",       0xF6, 0040);
    TEST("MUL", "BYTE [BX+DI+52]",    0xF6, 0141, 0x34);
    TEST("MUL", "BYTE [BP+SI+1234H]", 0xF6, 0242, 0x34, 0x12);
    // MUL AX,r/m16; AX*(r/m16)=DX:AX
    TEST("MUL", "BP",                 0xF7, 0345);
    TEST("MUL", "WORD [SI]",          0xF7, 0044);
    TEST("MUL", "WORD [1234H]",       0xF7, 0046, 0x34, 0x12);
    TEST("MUL", "WORD [DI-52]",       0xF7, 0145, 0xCC);
    TEST("MUL", "WORD [BP+1234H]",    0xF7, 0246, 0x34, 0x12);
    TEST("MUL", "WORD [BX+SI]",       0xF7, 0040);
    TEST("MUL", "WORD [BX+DI+52]",    0xF7, 0141, 0x34);
    TEST("MUL", "WORD [BP+SI+1234H]", 0xF7, 0242, 0x34, 0x12);
    if (is80386()) {
        // MUL EAX,r/m32; EAX*(r/m32)=EDX:EAX
        TEST("MUL", "EBP",                 DATA32, 0xF7, 0345);
        TEST("MUL", "DWORD [SI]",          DATA32, 0xF7, 0044);
        TEST("MUL", "DWORD [1234H]",       DATA32, 0xF7, 0046, 0x34, 0x12);
        TEST("MUL", "DWORD [DI-52]",       DATA32, 0xF7, 0145, 0xCC);
        TEST("MUL", "DWORD [BP+1234H]",    DATA32, 0xF7, 0246, 0x34, 0x12);
        TEST("MUL", "DWORD [BX+SI]",       DATA32, 0xF7, 0040);
        TEST("MUL", "DWORD [BX+DI+52]",    DATA32, 0xF7, 0141, 0x34);
        TEST("MUL", "DWORD [BP+SI+1234H]", DATA32, 0xF7, 0242, 0x34, 0x12);
    }

    // IMUL r/m8; AL*(r/m8)=AX
    TEST("IMUL", "CH",                 0xF6, 0355);
    TEST("IMUL", "BYTE [SI]",          0xF6, 0054);
    TEST("IMUL", "BYTE [1234H]",       0xF6, 0056, 0x34, 0x12);
    TEST("IMUL", "BYTE [DI-52]",       0xF6, 0155, 0xCC);
    TEST("IMUL", "BYTE [BP+1234H]",    0xF6, 0256, 0x34, 0x12);
    TEST("IMUL", "BYTE [BX+SI]",       0xF6, 0050);
    TEST("IMUL", "BYTE [BX+DI+52]",    0xF6, 0151, 0x34);
    TEST("IMUL", "BYTE [BP+SI+1234H]", 0xF6, 0252, 0x34, 0x12);
    // IMUL r/m16; AX*(r/m16)=DX:AX
    TEST("IMUL", "BP",                 0xF7, 0355);
    TEST("IMUL", "WORD [SI]",          0xF7, 0054);
    TEST("IMUL", "WORD [1234H]",       0xF7, 0056, 0x34, 0x12);
    TEST("IMUL", "WORD [DI-52]",       0xF7, 0155, 0xCC);
    TEST("IMUL", "WORD [BP+1234H]",    0xF7, 0256, 0x34, 0x12);
    TEST("IMUL", "WORD [BX+SI]",       0xF7, 0050);
    TEST("IMUL", "WORD [BX+DI+52]",    0xF7, 0151, 0x34);
    TEST("IMUL", "WORD [BP+SI+1234H]", 0xF7, 0252, 0x34, 0x12);
    if (is80x86()) {
        // IMUL r16,imm8; r16*imm8=r16
        TEST("IMUL", "AX, 1", 0x6B, 0300, 1);
        TEST("IMUL", "CX, 2", 0x6B, 0311, 2);
        TEST("IMUL", "DX, 3", 0x6B, 0322, 3);
        TEST("IMUL", "BX, 4", 0x6B, 0333, 4);
        TEST("IMUL", "SP, 5", 0x6B, 0344, 5);
        TEST("IMUL", "BP, 6", 0x6B, 0355, 6);
        TEST("IMUL", "SI, 7", 0x6B, 0366, 7);
        TEST("IMUL", "DI, 8", 0x6B, 0377, 8);
        // IMUL r16,imm16; r16*imm16=r16
        TEST("IMUL", "AX, 0123H", 0x69, 0300, 0x23, 0x01);
        TEST("IMUL", "CX, 1234H", 0x69, 0311, 0x34, 0x12);
        TEST("IMUL", "DX, 2345H", 0x69, 0322, 0x45, 0x23);
        TEST("IMUL", "BX, 3456H", 0x69, 0333, 0x56, 0x34);
        TEST("IMUL", "SP, 4567H", 0x69, 0344, 0x67, 0x45);
        TEST("IMUL", "BP, 5678H", 0x69, 0355, 0x78, 0x56);
        TEST("IMUL", "SI, 6789H", 0x69, 0366, 0x89, 0x67);
        TEST("IMUL", "DI, 789AH", 0x69, 0377, 0x9A, 0x78);

        // IMUL r16,r/m16,imm8; (r/m16)*imm8=r16
        TEST("IMUL", "AX, BP, 1",            0x6B, 0305, 1);
        TEST("IMUL", "CX, [SI], 2",          0x6B, 0014, 2);
        TEST("IMUL", "DX, [1234H], 3",       0x6B, 0026, 0x34, 0x12, 3);
        TEST("IMUL", "BX, [DI-52], 4",       0x6B, 0135, 0xCC, 4);
        TEST("IMUL", "SP, [BP+1234H], 5",    0x6B, 0246, 0x34, 0x12, 5);
        TEST("IMUL", "BP, [BX+SI], 6",       0x6B, 0050, 6);
        TEST("IMUL", "SI, [BX+DI+52], 7",    0x6B, 0161, 0x34, 7);
        TEST("IMUL", "DI, [BP+SI+1234H], 8", 0x6B, 0272, 0x34, 0x12, 8);
        // IMUL r16,r/m16,imm16; (r/m16)*imm16=r16
        TEST("IMUL", "AX, BP, 0123H",            0x69, 0305, 0x23, 0x01);
        TEST("IMUL", "CX, [SI], 1234H",          0x69, 0014, 0x34, 0x12);
        TEST("IMUL", "DX, [1234H], 2345H",       0x69, 0026, 0x34, 0x12, 0x45, 0x23);
        TEST("IMUL", "BX, [DI-52], 3456H",       0x69, 0135, 0xCC, 0x56, 0x34);
        TEST("IMUL", "SP, [BP+1234H], 4567H",    0x69, 0246, 0x34, 0x12, 0x67, 0x45);
        TEST("IMUL", "BP, [BX+SI], 5678H",       0x69, 0050, 0x78, 0x56);
        TEST("IMUL", "SI, [BX+DI+52], 6789H",    0x69, 0161, 0x34, 0x89, 0x67);
        TEST("IMUL", "DI, [BP+SI+1234H], 789AH", 0x69, 0272, 0x34, 0x12, 0x9A, 0x78);
    }
    if (is80386()) {
        // IMUL r32,imm8; r32*imm8=r32
        TEST("IMUL", "EAX, 1", DATA32, 0x6B, 0300, 1);
        TEST("IMUL", "ECX, 2", DATA32, 0x6B, 0311, 2);
        TEST("IMUL", "EDX, 3", DATA32, 0x6B, 0322, 3);
        TEST("IMUL", "EBX, 4", DATA32, 0x6B, 0333, 4);
        TEST("IMUL", "ESP, 5", DATA32, 0x6B, 0344, 5);
        TEST("IMUL", "EBP, 6", DATA32, 0x6B, 0355, 6);
        TEST("IMUL", "ESI, 7", DATA32, 0x6B, 0366, 7);
        TEST("IMUL", "EDI, 8", DATA32, 0x6B, 0377, 8);
        // IMUL r32,imm32; r32*imm32=tr32
        TEST("IMUL", "EAX, 01234567H", DATA32, 0x69, 0300, 0x67, 0x45, 0x23, 0x01);
        TEST("IMUL", "ECX, 12345678H", DATA32, 0x69, 0311, 0x78, 0x56, 0x34, 0x12);
        TEST("IMUL", "EDX, 23456789H", DATA32, 0x69, 0322, 0x89, 0x67, 0x45, 0x23);
        TEST("IMUL", "EBX, 3456789AH", DATA32, 0x69, 0333, 0x9A, 0x78, 0x56, 0x34);
        TEST("IMUL", "ESP, 456789ABH", DATA32, 0x69, 0344, 0xAB, 0x89, 0x67, 0x45);
        TEST("IMUL", "EBP, 12345678H", DATA32, 0x69, 0355, 0x78, 0x56, 0x34, 0x12);
        TEST("IMUL", "ESI, 6789ABCDH", DATA32, 0x69, 0366, 0xCD, 0xAB, 0x89, 0x67);
        TEST("IMUL", "EDI, 789ABCDEH", DATA32, 0x69, 0377, 0xDE, 0xBC, 0x9A, 0x78);

        // IMUL r32,r/m32,imm8; (r/m32)*imm8=r32
        TEST("IMUL", "EAX, EBP, 1",           DATA32, 0x6B, 0305, 1);
        TEST("IMUL", "ECX, [SI], 2",          DATA32, 0x6B, 0014, 2);
        TEST("IMUL", "EDX, [1234H], 3",       DATA32, 0x6B, 0026, 0x34, 0x12, 3);
        TEST("IMUL", "EBX, [DI-52], 4",       DATA32, 0x6B, 0135, 0xCC, 4);
        TEST("IMUL", "ESP, [BP+1234H], 5",    DATA32, 0x6B, 0246, 0x34, 0x12, 5);
        TEST("IMUL", "EBP, [BX+SI], 6",       DATA32, 0x6B, 0050, 6);
        TEST("IMUL", "ESI, [BX+DI+52], 7",    DATA32, 0x6B, 0161, 0x34, 7);
        TEST("IMUL", "EDI, [BP+SI+1234H], 8", DATA32, 0x6B, 0272, 0x34, 0x12, 8);
        // IMUL r32,r/m32,imm32; (r/m32)*imm32=r32
        TEST("IMUL", "EAX, EBP, 01234567H",           DATA32, 0x69, 0305, 0x67, 0x45, 0x23, 0x01);
        TEST("IMUL", "ECX, [SI], 12345678H",          DATA32, 0x69, 0014, 0x78, 0x56, 0x34, 0x12);
        TEST("IMUL", "EDX, [1234H], 23456789H",       DATA32, 0x69, 0026, 0x34, 0x12, 0x89, 0x67, 0x45, 0x23);
        TEST("IMUL", "EBX, [DI-52], 3456789AH",       DATA32, 0x69, 0135, 0xCC, 0x9A, 0x78, 0x56, 0x34);
        TEST("IMUL", "ESP, [BP+1234H], 456789ABH",    DATA32, 0x69, 0246, 0x34, 0x12, 0xAB, 0x89, 0x67, 0x45);
        TEST("IMUL", "EBP, [BX+SI], 56789ABCH",       DATA32, 0x69, 0050, 0xBC, 0x9A, 0x78, 0x56);
        TEST("IMUL", "ESI, [BX+DI+52], 6789ABCDH",    DATA32, 0x69, 0161, 0x34, 0xCD, 0xAB, 0x89, 0x67);
        TEST("IMUL", "EDI, [BP+SI+1234H], 789ABCDEH", DATA32, 0x69, 0272, 0x34, 0x12, 0xDE, 0xBC, 0x9A, 0x78);

        // IMUL r/m32; EAX*(r/m32)=EDX:EAX
        TEST("IMUL", "EBP",                 DATA32, 0xF7, 0355);
        TEST("IMUL", "DWORD [SI]",          DATA32, 0xF7, 0054);
        TEST("IMUL", "DWORD [1234H]",       DATA32, 0xF7, 0056, 0x34, 0x12);
        TEST("IMUL", "DWORD [DI-52]",       DATA32, 0xF7, 0155, 0xCC);
        TEST("IMUL", "DWORD [BP+1234H]",    DATA32, 0xF7, 0256, 0x34, 0x12);
        TEST("IMUL", "DWORD [BX+SI]",       DATA32, 0xF7, 0050);
        TEST("IMUL", "DWORD [BX+DI+52]",    DATA32, 0xF7, 0151, 0x34);
        TEST("IMUL", "DWORD [BP+SI+1234H]", DATA32, 0xF7, 0252, 0x34, 0x12);

        // IMUL r16,r/m16; r16*(r/m16)=r16
        TEST("IMUL", "SI, [SI]",          0x0F, 0xAF, 0064);
        TEST("IMUL", "DI, [1234H]",       0x0F, 0xAF, 0076, 0x34, 0x12);
        TEST("IMUL", "AX, [DI+52]",       0x0F, 0xAF, 0105, 0x34);
        TEST("IMUL", "CX, [BP+1234H]",    0x0F, 0xAF, 0216, 0x34, 0x12);
        TEST("IMUL", "DX, [BX+SI]",       0x0F, 0xAF, 0020);
        TEST("IMUL", "BX, [BX+DI+52]",    0x0F, 0xAF, 0131, 0x34);
        TEST("IMUL", "SP, [BP+SI+1234H]", 0x0F, 0xAF, 0242, 0x34, 0x12);
        // IMUL r32,r/m32; r32*(r/m32)=r32
        TEST("IMUL", "ESI, [SI]",          DATA32, 0x0F, 0xAF, 0064);
        TEST("IMUL", "EDI, [1234H]",       DATA32, 0x0F, 0xAF, 0076, 0x34, 0x12);
        TEST("IMUL", "EAX, [DI+52]",       DATA32, 0x0F, 0xAF, 0105, 0x34);
        TEST("IMUL", "ECX, [BP+1234H]",    DATA32, 0x0F, 0xAF, 0216, 0x34, 0x12);
        TEST("IMUL", "EDX, [BX+SI]",       DATA32, 0x0F, 0xAF, 0020);
        TEST("IMUL", "EBX, [BX+DI+52]",    DATA32, 0x0F, 0xAF, 0131, 0x34);
        TEST("IMUL", "ESP, [BP+SI+1234H]", DATA32, 0x0F, 0xAF, 0242, 0x34, 0x12);
    }

    TEST("AAM", "", 0xD4, 0x0A);

    // DIV AL,r/m8; AX/(r/m8)=AL..AH
    TEST("DIV", "CH",                 0xF6, 0365);
    TEST("DIV", "BYTE [SI]",          0xF6, 0064);
    TEST("DIV", "BYTE [1234H]",       0xF6, 0066, 0x34, 0x12);
    TEST("DIV", "BYTE [DI-52]",       0xF6, 0165, 0xCC);
    TEST("DIV", "BYTE [BP+1234H]",    0xF6, 0266, 0x34, 0x12);
    TEST("DIV", "BYTE [BX+SI]",       0xF6, 0060);
    TEST("DIV", "BYTE [BX+DI+52]",    0xF6, 0161, 0x34);
    TEST("DIV", "BYTE [BP+SI+1234H]", 0xF6, 0262, 0x34, 0x12);
    // DIV AX,r/m16; (DX:AX)/(r/m16)=AX...DX
    TEST("DIV", "BP",                 0xF7, 0365);
    TEST("DIV", "WORD [SI]",          0xF7, 0064);
    TEST("DIV", "WORD [1234H]",       0xF7, 0066, 0x34, 0x12);
    TEST("DIV", "WORD [DI-52]",       0xF7, 0165, 0xCC);
    TEST("DIV", "WORD [BP+1234H]",    0xF7, 0266, 0x34, 0x12);
    TEST("DIV", "WORD [BX+SI]",       0xF7, 0060);
    TEST("DIV", "WORD [BX+DI+52]",    0xF7, 0161, 0x34);
    TEST("DIV", "WORD [BP+SI+1234H]", 0xF7, 0262, 0x34, 0x12);
    if (is80386()) {
        // DIV EAX,r/m32; (EDX:EAX)/(r/m32)=EAX...EDX
        TEST("DIV", "EBP",                 DATA32, 0xF7, 0365);
        TEST("DIV", "DWORD [SI]",          DATA32, 0xF7, 0064);
        TEST("DIV", "DWORD [1234H]",       DATA32, 0xF7, 0066, 0x34, 0x12);
        TEST("DIV", "DWORD [DI-52]",       DATA32, 0xF7, 0165, 0xCC);
        TEST("DIV", "DWORD [BP+1234H]",    DATA32, 0xF7, 0266, 0x34, 0x12);
        TEST("DIV", "DWORD [BX+SI]",       DATA32, 0xF7, 0060);
        TEST("DIV", "DWORD [BX+DI+52]",    DATA32, 0xF7, 0161, 0x34);
        TEST("DIV", "DWORD [BP+SI+1234H]", DATA32, 0xF7, 0262, 0x34, 0x12);
    }

    // IDIV AL,r/m8; AX/(r/m8)=AL...AH
    TEST("IDIV", "CH",                 0xF6, 0375);
    TEST("IDIV", "BYTE [SI]",          0xF6, 0074);
    TEST("IDIV", "BYTE [1234H]",       0xF6, 0076, 0x34, 0x12);
    TEST("IDIV", "BYTE [DI-52]",       0xF6, 0175, 0xCC);
    TEST("IDIV", "BYTE [BP+1234H]",    0xF6, 0276, 0x34, 0x12);
    TEST("IDIV", "BYTE [BX+SI]",       0xF6, 0070);
    TEST("IDIV", "BYTE [BX+DI+52]",    0xF6, 0171, 0x34);
    TEST("IDIV", "BYTE [BP+SI+1234H]", 0xF6, 0272, 0x34, 0x12);
    // IDIV AX,r/m8; (DX:AX)/(r/m16)=AX...DX
    TEST("IDIV", "BP",                 0xF7, 0375);
    TEST("IDIV", "WORD [SI]",          0xF7, 0074);
    TEST("IDIV", "WORD [1234H]",       0xF7, 0076, 0x34, 0x12);
    TEST("IDIV", "WORD [DI-52]",       0xF7, 0175, 0xCC);
    TEST("IDIV", "WORD [BP+1234H]",    0xF7, 0276, 0x34, 0x12);
    TEST("IDIV", "WORD [BX+SI]",       0xF7, 0070);
    TEST("IDIV", "WORD [BX+DI+52]",    0xF7, 0171, 0x34);
    TEST("IDIV", "WORD [BP+SI+1234H]", 0xF7, 0272, 0x34, 0x12);
    if (is80386()) {
        // IDIV EAX,r/m32; (EDX:EAX)/(r/m32)=EAX...EDX
        TEST("IDIV", "EBP",                 DATA32, 0xF7, 0375);
        TEST("IDIV", "DWORD [SI]",          DATA32, 0xF7, 0074);
        TEST("IDIV", "DWORD [1234H]",       DATA32, 0xF7, 0076, 0x34, 0x12);
        TEST("IDIV", "DWORD [DI-52]",       DATA32, 0xF7, 0175, 0xCC);
        TEST("IDIV", "DWORD [BP+1234H]",    DATA32, 0xF7, 0276, 0x34, 0x12);
        TEST("IDIV", "DWORD [BX+SI]",       DATA32, 0xF7, 0070);
        TEST("IDIV", "DWORD [BX+DI+52]",    DATA32, 0xF7, 0171, 0x34);
        TEST("IDIV", "DWORD [BP+SI+1234H]", DATA32, 0xF7, 0272, 0x34, 0x12);
    }

    TEST("AAD", "", 0xD5, 0x0A);

    TEST("CBW", "", 0x98);
    TEST("CWD", "", 0x99);
    if (is80386()) {
        TEST("CWDE", "", DATA32, 0x98);
        TEST("CDQ",  "", DATA32, 0x99);
    }
}

void test_logic() {
    // NOT r/m8
    TEST("NOT", "CH",                 0xF6, 0325);
    TEST("NOT", "BYTE [SI]",          0xF6, 0024);
    TEST("NOT", "BYTE [1234H]",       0xF6, 0026, 0x34, 0x12);
    TEST("NOT", "BYTE [DI-52]",       0xF6, 0125, 0xCC);
    TEST("NOT", "BYTE [BP+1234H]",    0xF6, 0226, 0x34, 0x12);
    TEST("NOT", "BYTE [BX+SI]",       0xF6, 0020);
    TEST("NOT", "BYTE [BX+DI+52]",    0xF6, 0121, 0x34);
    TEST("NOT", "BYTE [BP+SI+1234H]", 0xF6, 0222, 0x34, 0x12);
    // NOT r/m16
    TEST("NOT", "BP",                 0xF7, 0325);
    TEST("NOT", "WORD [SI]",          0xF7, 0024);
    TEST("NOT", "WORD [1234H]",       0xF7, 0026, 0x34, 0x12);
    TEST("NOT", "WORD [DI-52]",       0xF7, 0125, 0xCC);
    TEST("NOT", "WORD [BP+1234H]",    0xF7, 0226, 0x34, 0x12);
    TEST("NOT", "WORD [BX+SI]",       0xF7, 0020);
    TEST("NOT", "WORD [BX+DI+52]",    0xF7, 0121, 0x34);
    TEST("NOT", "WORD [BP+SI+1234H]", 0xF7, 0222, 0x34, 0x12);
    if (is80386()) {
        // NOT r/m32
        TEST("NOT", "EBP",                 DATA32, 0xF7, 0325);
        TEST("NOT", "DWORD [SI]",          DATA32, 0xF7, 0024);
        TEST("NOT", "DWORD [1234H]",       DATA32, 0xF7, 0026, 0x34, 0x12);
        TEST("NOT", "DWORD [DI-52]",       DATA32, 0xF7, 0125, 0xCC);
        TEST("NOT", "DWORD [BP+1234H]",    DATA32, 0xF7, 0226, 0x34, 0x12);
        TEST("NOT", "DWORD [BX+SI]",       DATA32, 0xF7, 0020);
        TEST("NOT", "DWORD [BX+DI+52]",    DATA32, 0xF7, 0121, 0x34);
        TEST("NOT", "DWORD [BP+SI+1234H]", DATA32, 0xF7, 0222, 0x34, 0x12);
    }

    // SHL/SAL r/m8,1
    TEST("SHL", "CH, 1",                  0xD0, 0345);
    TEST("SHL", "BYTE [SI], 1",           0xD0, 0044);
    TEST("SHL", "BYTE [1234H], 1",        0xD0, 0046, 0x34, 0x12);
    TEST("SHL", "BYTE [DI-52], 1",        0xD0, 0145, 0xCC);
    TEST("SHL", "BYTE [BP+1234H], 1",     0xD0, 0246, 0x34, 0x12);
    TEST("SHL", "BYTE [BX+SI], 1",        0xD0, 0040);
    TEST("SHL", "BYTE [BX+DI+52], 1",     0xD0, 0141, 0x34);
    TEST("SHL", "BYTE [BP+SI+1234H], 1",  0xD0, 0242, 0x34, 0x12);
    // SHL/SAL r/m16,1
    TEST("SHL", "BP, 1",                  0xD1, 0345);
    TEST("SHL", "WORD [SI], 1",           0xD1, 0044);
    TEST("SHL", "WORD [1234H], 1",        0xD1, 0046, 0x34, 0x12);
    TEST("SHL", "WORD [DI-52], 1",        0xD1, 0145, 0xCC);
    TEST("SHL", "WORD [BP+1234H], 1",     0xD1, 0246, 0x34, 0x12);
    TEST("SHL", "WORD [BX+SI], 1",        0xD1, 0040);
    TEST("SHL", "WORD [BX+DI+52], 1",     0xD1, 0141, 0x34);
    TEST("SHL", "WORD [BP+SI+1234H], 1",  0xD1, 0242, 0x34, 0x12);
    if (is80x86()) {
        // SHL/SAL r/m8,imm8
        TEST("SHL", "CH, 2",                  0xC0, 0345, 2);
        TEST("SHL", "BYTE [SI], 3",           0xC0, 0044, 3);
        TEST("SHL", "BYTE [1234H], 4",        0xC0, 0046, 0x34, 0x12, 4);
        TEST("SHL", "BYTE [DI-52], 5",        0xC0, 0145, 0xCC, 5);
        TEST("SHL", "BYTE [BP+1234H], 6",     0xC0, 0246, 0x34, 0x12, 6);
        TEST("SHL", "BYTE [BX+SI], 7",        0xC0, 0040, 7);
        TEST("SHL", "BYTE [BX+DI+52], 0",     0xC0, 0141, 0x34, 0);
        TEST("SHL", "BYTE [BP+SI+1234H], 1",  0xC0, 0242, 0x34, 0x12, 1);
        ERRT("SHL", "CH, 8",  OVERFLOW_RANGE,  "8", 0xC0, 0345, 8);
        // SHL/SAL r/m16,imm8
        TEST("SHL", "BP, 8",                  0xC1, 0345, 8);
        TEST("SHL", "WORD [SI], 1",           0xC1, 0044, 1);
        TEST("SHL", "WORD [1234H], 10",       0xC1, 0046, 0x34, 0x12, 10);
        TEST("SHL", "WORD [DI-52], 11",       0xC1, 0145, 0xCC, 11);
        TEST("SHL", "WORD [BP+1234H], 12",    0xC1, 0246, 0x34, 0x12, 12);
        TEST("SHL", "WORD [BX+SI], 13",       0xC1, 0040, 13);
        TEST("SHL", "WORD [BX+DI+52], 14",    0xC1, 0141, 0x34, 14);
        TEST("SHL", "WORD [BP+SI+1234H], 15", 0xC1, 0242, 0x34, 0x12, 15);
        ERRT("SHL", "BP, 16", OVERFLOW_RANGE, "16", 0xC1, 0345, 16);
    }
    if (is80386()) {
        // SHL/SAL r/m32,1
        TEST("SHL", "EBP, 1",                 DATA32, 0xD1, 0345);
        TEST("SHL", "DWORD [SI], 1",          DATA32, 0xD1, 0044);
        TEST("SHL", "DWORD [1234H], 1",       DATA32, 0xD1, 0046, 0x34, 0x12);
        TEST("SHL", "DWORD [DI-52], 1",       DATA32, 0xD1, 0145, 0xCC);
        TEST("SHL", "DWORD [BP+1234H], 1",    DATA32, 0xD1, 0246, 0x34, 0x12);
        TEST("SHL", "DWORD [BX+SI], 1",       DATA32, 0xD1, 0040);
        TEST("SHL", "DWORD [BX+DI+52], 1",    DATA32, 0xD1, 0141, 0x34);
        TEST("SHL", "DWORD [BP+SI+1234H], 1", DATA32, 0xD1, 0242, 0x34, 0x12);
        // SHL/SAL r/m32,imm8
        TEST("SHL", "EBP, 16",                 DATA32, 0xC1, 0345, 16);
        TEST("SHL", "DWORD [SI], 1",           DATA32, 0xC1, 0044, 1);
        TEST("SHL", "DWORD [1234H], 10",       DATA32, 0xC1, 0046, 0x34, 0x12, 10);
        TEST("SHL", "DWORD [DI-52], 11",       DATA32, 0xC1, 0145, 0xCC, 11);
        TEST("SHL", "DWORD [BP+1234H], 12",    DATA32, 0xC1, 0246, 0x34, 0x12, 12);
        TEST("SHL", "DWORD [BX+SI], 13",       DATA32, 0xC1, 0040, 13);
        TEST("SHL", "DWORD [BX+DI+52], 14",    DATA32, 0xC1, 0141, 0x34, 14);
        TEST("SHL", "DWORD [BP+SI+1234H], 15", DATA32, 0xC1, 0242, 0x34, 0x12, 15);
        ERRT("SHL", "EBP, 32", OVERFLOW_RANGE, "32", DATA32, 0xC1, 0345, 32);
    }
    // SHL/SAL, r/m8,CL
    TEST("SHL", "CH, CL",                 0xD2, 0345);
    TEST("SHL", "BYTE [SI], CL",          0xD2, 0044);
    TEST("SHL", "BYTE [1234H], CL",       0xD2, 0046, 0x34, 0x12);
    TEST("SHL", "BYTE [DI-52], CL",       0xD2, 0145, 0xCC);
    TEST("SHL", "BYTE [BP+1234H], CL",    0xD2, 0246, 0x34, 0x12);
    TEST("SHL", "BYTE [BX+SI], CL",       0xD2, 0040);
    TEST("SHL", "BYTE [BX+DI+52], CL",    0xD2, 0141, 0x34);
    TEST("SHL", "BYTE [BP+SI+1234H], CL", 0xD2, 0242, 0x34, 0x12);
    // SHL/SAL, r/m16,CL
    TEST("SHL", "BP, CL",                 0xD3, 0345);
    TEST("SHL", "WORD [SI], CL",          0xD3, 0044);
    TEST("SHL", "WORD [1234H], CL",       0xD3, 0046, 0x34, 0x12);
    TEST("SHL", "WORD [DI-52], CL",       0xD3, 0145, 0xCC);
    TEST("SHL", "WORD [BP+1234H], CL",    0xD3, 0246, 0x34, 0x12);
    TEST("SHL", "WORD [BX+SI], CL",       0xD3, 0040);
    TEST("SHL", "WORD [BX+DI+52], CL",    0xD3, 0141, 0x34);
    TEST("SHL", "WORD [BP+SI+1234H], CL", 0xD3, 0242, 0x34, 0x12);
    if (is80386()) {
        // SHL/SAL, r/m32,CL
        TEST("SHL", "EBP, CL",                 DATA32, 0xD3, 0345);
        TEST("SHL", "DWORD [SI], CL",          DATA32, 0xD3, 0044);
        TEST("SHL", "DWORD [1234H], CL",       DATA32, 0xD3, 0046, 0x34, 0x12);
        TEST("SHL", "DWORD [DI-52], CL",       DATA32, 0xD3, 0145, 0xCC);
        TEST("SHL", "DWORD [BP+1234H], CL",    DATA32, 0xD3, 0246, 0x34, 0x12);
        TEST("SHL", "DWORD [BX+SI], CL",       DATA32, 0xD3, 0040);
        TEST("SHL", "DWORD [BX+DI+52], CL",    DATA32, 0xD3, 0141, 0x34);
        TEST("SHL", "DWORD [BP+SI+1234H], CL", DATA32, 0xD3, 0242, 0x34, 0x12);
    }

    // SHR r/m8,1
    TEST("SHR", "CH, 1",                 0xD0, 0355);
    TEST("SHR", "BYTE [SI], 1",          0xD0, 0054);
    TEST("SHR", "BYTE [1234H], 1",       0xD0, 0056, 0x34, 0x12);
    TEST("SHR", "BYTE [DI-52], 1",       0xD0, 0155, 0xCC);
    TEST("SHR", "BYTE [BP+1234H], 1",    0xD0, 0256, 0x34, 0x12);
    TEST("SHR", "BYTE [BX+SI], 1",       0xD0, 0050);
    TEST("SHR", "BYTE [BX+DI+52], 1",    0xD0, 0151, 0x34);
    TEST("SHR", "BYTE [BP+SI+1234H], 1", 0xD0, 0252, 0x34, 0x12);
    // SHR r/m16,1
    TEST("SHR", "BP, 1",                 0xD1, 0355);
    TEST("SHR", "WORD [SI], 1",          0xD1, 0054);
    TEST("SHR", "WORD [1234H], 1",       0xD1, 0056, 0x34, 0x12);
    TEST("SHR", "WORD [DI-52], 1",       0xD1, 0155, 0xCC);
    TEST("SHR", "WORD [BP+1234H], 1",    0xD1, 0256, 0x34, 0x12);
    TEST("SHR", "WORD [BX+SI], 1",       0xD1, 0050);
    TEST("SHR", "WORD [BX+DI+52], 1",    0xD1, 0151, 0x34);
    TEST("SHR", "WORD [BP+SI+1234H], 1", 0xD1, 0252, 0x34, 0x12);
    if (is80x86()) {
        // SHR r/m8,imm8
        TEST("SHR", "CH, 3",                 0xC0, 0355, 3);
        TEST("SHR", "BYTE [SI], 4",          0xC0, 0054, 4);
        TEST("SHR", "BYTE [1234H], 5",       0xC0, 0056, 0x34, 0x12, 5);
        TEST("SHR", "BYTE [DI-52], 6",       0xC0, 0155, 0xCC, 6);
        TEST("SHR", "BYTE [BP+1234H], 7",    0xC0, 0256, 0x34, 0x12, 7);
        TEST("SHR", "BYTE [BX+SI], 0",       0xC0, 0050, 0);
        TEST("SHR", "BYTE [BX+DI+52], 1",    0xC0, 0151, 0x34, 1);
        TEST("SHR", "BYTE [BP+SI+1234H], 2", 0xC0, 0252, 0x34, 0x12, 2);
        ERRT("SHR", "BYTE [SI], 8",  OVERFLOW_RANGE,  "8", 0xC0, 0054, 8);
        // SHR r/m16,imm8
        TEST("SHR", "BP, 8",                  0xC1, 0355, 8);
        TEST("SHR", "WORD [SI], 9",           0xC1, 0054, 9);
        TEST("SHR", "WORD [1234H], 1",        0xC1, 0056, 0x34, 0x12, 1);
        TEST("SHR", "WORD [DI-52], 11",       0xC1, 0155, 0xCC, 11);
        TEST("SHR", "WORD [BP+1234H], 12",    0xC1, 0256, 0x34, 0x12, 12);
        TEST("SHR", "WORD [BX+SI], 13",       0xC1, 0050, 13);
        TEST("SHR", "WORD [BX+DI+52], 14",    0xC1, 0151, 0x34, 14);
        TEST("SHR", "WORD [BP+SI+1234H], 15", 0xC1, 0252, 0x34, 0x12, 15);
        ERRT("SHR", "WORD [SI], 16", OVERFLOW_RANGE, "16", 0xC1, 0054, 16);
    }
    if (is80386()) {
        // SHR r/m32,1
        TEST("SHR", "EBP, 1",                 DATA32, 0xD1, 0355);
        TEST("SHR", "DWORD [SI], 1",          DATA32, 0xD1, 0054);
        TEST("SHR", "DWORD [1234H], 1",       DATA32, 0xD1, 0056, 0x34, 0x12);
        TEST("SHR", "DWORD [DI-52], 1",       DATA32, 0xD1, 0155, 0xCC);
        TEST("SHR", "DWORD [BP+1234H], 1",    DATA32, 0xD1, 0256, 0x34, 0x12);
        TEST("SHR", "DWORD [BX+SI], 1",       DATA32, 0xD1, 0050);
        TEST("SHR", "DWORD [BX+DI+52], 1",    DATA32, 0xD1, 0151, 0x34);
        TEST("SHR", "DWORD [BP+SI+1234H], 1", DATA32, 0xD1, 0252, 0x34, 0x12);
        // SHR r/m32,imm8
        TEST("SHR", "EBP, 16",                 DATA32, 0xC1, 0355, 16);
        TEST("SHR", "DWORD [SI], 9",           DATA32, 0xC1, 0054, 9);
        TEST("SHR", "DWORD [1234H], 1",        DATA32, 0xC1, 0056, 0x34, 0x12, 1);
        TEST("SHR", "DWORD [DI-52], 11",       DATA32, 0xC1, 0155, 0xCC, 11);
        TEST("SHR", "DWORD [BP+1234H], 12",    DATA32, 0xC1, 0256, 0x34, 0x12, 12);
        TEST("SHR", "DWORD [BX+SI], 13",       DATA32, 0xC1, 0050, 13);
        TEST("SHR", "DWORD [BX+DI+52], 14",    DATA32, 0xC1, 0151, 0x34, 14);
        TEST("SHR", "DWORD [BP+SI+1234H], 15", DATA32, 0xC1, 0252, 0x34, 0x12, 15);
        ERRT("SHR", "DWORD [SI], 32", OVERFLOW_RANGE, "32", DATA32, 0xC1, 0054, 32);
    }
    // SHR r/m8,CL
    TEST("SHR", "CH, CL",                 0xD2, 0355);
    TEST("SHR", "BYTE [SI], CL",          0xD2, 0054);
    TEST("SHR", "BYTE [1234H], CL",       0xD2, 0056, 0x34, 0x12);
    TEST("SHR", "BYTE [DI-52], CL",       0xD2, 0155, 0xCC);
    TEST("SHR", "BYTE [BP+1234H], CL",    0xD2, 0256, 0x34, 0x12);
    TEST("SHR", "BYTE [BX+SI], CL",       0xD2, 0050);
    TEST("SHR", "BYTE [BX+DI+52], CL",    0xD2, 0151, 0x34);
    TEST("SHR", "BYTE [BP+SI+1234H], CL", 0xD2, 0252, 0x34, 0x12);
    // SHR r/m16,CL
    TEST("SHR", "BP, CL",                 0xD3, 0355);
    TEST("SHR", "WORD [SI], CL",          0xD3, 0054);
    TEST("SHR", "WORD [1234H], CL",       0xD3, 0056, 0x34, 0x12);
    TEST("SHR", "WORD [DI-52], CL",       0xD3, 0155, 0xCC);
    TEST("SHR", "WORD [BP+1234H], CL",    0xD3, 0256, 0x34, 0x12);
    TEST("SHR", "WORD [BX+SI], CL",       0xD3, 0050);
    TEST("SHR", "WORD [BX+DI+52], CL",    0xD3, 0151, 0x34);
    TEST("SHR", "WORD [BP+SI+1234H], CL", 0xD3, 0252, 0x34, 0x12);
    if (is80386()) {
        // SHR r/m32,CL
        TEST("SHR", "EBP, CL",                 DATA32, 0xD3, 0355);
        TEST("SHR", "DWORD [SI], CL",          DATA32, 0xD3, 0054);
        TEST("SHR", "DWORD [1234H], CL",       DATA32, 0xD3, 0056, 0x34, 0x12);
        TEST("SHR", "DWORD [DI-52], CL",       DATA32, 0xD3, 0155, 0xCC);
        TEST("SHR", "DWORD [BP+1234H], CL",    DATA32, 0xD3, 0256, 0x34, 0x12);
        TEST("SHR", "DWORD [BX+SI], CL",       DATA32, 0xD3, 0050);
        TEST("SHR", "DWORD [BX+DI+52], CL",    DATA32, 0xD3, 0151, 0x34);
        TEST("SHR", "DWORD [BP+SI+1234H], CL", DATA32, 0xD3, 0252, 0x34, 0x12);
    }

    // SAR r/m8,1
    TEST("SAR", "CH, 1",                 0xD0, 0375);
    TEST("SAR", "BYTE [SI], 1",          0xD0, 0074);
    TEST("SAR", "BYTE [1234H], 1",       0xD0, 0076, 0x34, 0x12);
    TEST("SAR", "BYTE [DI-52], 1",       0xD0, 0175, 0xCC);
    TEST("SAR", "BYTE [BP+1234H], 1",    0xD0, 0276, 0x34, 0x12);
    TEST("SAR", "BYTE [BX+SI], 1",       0xD0, 0070);
    TEST("SAR", "BYTE [BX+DI+52], 1",    0xD0, 0171, 0x34);
    TEST("SAR", "BYTE [BP+SI+1234H], 1", 0xD0, 0272, 0x34, 0x12);
    // SAR r/m16,1
    TEST("SAR", "BP, 1",                 0xD1, 0375);
    TEST("SAR", "WORD [SI], 1",          0xD1, 0074);
    TEST("SAR", "WORD [1234H], 1",       0xD1, 0076, 0x34, 0x12);
    TEST("SAR", "WORD [DI-52], 1",       0xD1, 0175, 0xCC);
    TEST("SAR", "WORD [BP+1234H], 1",    0xD1, 0276, 0x34, 0x12);
    TEST("SAR", "WORD [BX+SI], 1",       0xD1, 0070);
    TEST("SAR", "WORD [BX+DI+52], 1",    0xD1, 0171, 0x34);
    TEST("SAR", "WORD [BP+SI+1234H], 1", 0xD1, 0272, 0x34, 0x12);
    if (is80x86()) {
        // SAR r/m8,imm8
        TEST("SAR", "CH, 1",                 0xC0, 0375, 1);
        TEST("SAR", "BYTE [SI], 2",          0xC0, 0074, 2);
        TEST("SAR", "BYTE [1234H], 3",       0xC0, 0076, 0x34, 0x12, 3);
        TEST("SAR", "BYTE [DI-52], 4",       0xC0, 0175, 0xCC, 4);
        TEST("SAR", "BYTE [BP+1234H], 5",    0xC0, 0276, 0x34, 0x12, 5);
        TEST("SAR", "BYTE [BX+SI], 6",       0xC0, 0070, 6);
        TEST("SAR", "BYTE [BX+DI+52], 7",    0xC0, 0171, 0x34, 7);
        TEST("SAR", "BYTE [BP+SI+1234H], 0", 0xC0, 0272, 0x34, 0x12, 0);
        ERRT("SAR", "BYTE [1234H], 8",  OVERFLOW_RANGE,  "8", 0xC0, 0076, 0x34, 0x12, 8);
        // SAR r/m16,imm8
        TEST("SAR", "BP, 8",                  0xC1, 0375, 8);
        TEST("SAR", "WORD [SI], 9",           0xC1, 0074, 9);
        TEST("SAR", "WORD [1234H], 1",        0xC1, 0076, 0x34, 0x12, 1);
        TEST("SAR", "WORD [DI-52], 11",       0xC1, 0175, 0xCC, 11);
        TEST("SAR", "WORD [BP+1234H], 12",    0xC1, 0276, 0x34, 0x12, 12);
        TEST("SAR", "WORD [BX+SI], 13",       0xC1, 0070, 13);
        TEST("SAR", "WORD [BX+DI+52], 14",    0xC1, 0171, 0x34, 14);
        TEST("SAR", "WORD [BP+SI+1234H], 15", 0xC1, 0272, 0x34, 0x12, 15);
        ERRT("SAR", "WORD [1234H], 16", OVERFLOW_RANGE, "16", 0xC1, 0076, 0x34, 0x12, 16);
    }
    if (is80386()) {
        // SAR r/m32,1
        TEST("SAR", "EBP, 1",                 DATA32, 0xD1, 0375);
        TEST("SAR", "DWORD [SI], 1",          DATA32, 0xD1, 0074);
        TEST("SAR", "DWORD [1234H], 1",       DATA32, 0xD1, 0076, 0x34, 0x12);
        TEST("SAR", "DWORD [DI-52], 1",       DATA32, 0xD1, 0175, 0xCC);
        TEST("SAR", "DWORD [BP+1234H], 1",    DATA32, 0xD1, 0276, 0x34, 0x12);
        TEST("SAR", "DWORD [BX+SI], 1",       DATA32, 0xD1, 0070);
        TEST("SAR", "DWORD [BX+DI+52], 1",    DATA32, 0xD1, 0171, 0x34);
        TEST("SAR", "DWORD [BP+SI+1234H], 1", DATA32, 0xD1, 0272, 0x34, 0x12);
        // SAR r/m32,imm8
        TEST("SAR", "EBP, 16",                 DATA32, 0xC1, 0375, 16);
        TEST("SAR", "DWORD [SI], 9",           DATA32, 0xC1, 0074, 9);
        TEST("SAR", "DWORD [1234H], 1",        DATA32, 0xC1, 0076, 0x34, 0x12, 1);
        TEST("SAR", "DWORD [DI-52], 11",       DATA32, 0xC1, 0175, 0xCC, 11);
        TEST("SAR", "DWORD [BP+1234H], 12",    DATA32, 0xC1, 0276, 0x34, 0x12, 12);
        TEST("SAR", "DWORD [BX+SI], 13",       DATA32, 0xC1, 0070, 13);
        TEST("SAR", "DWORD [BX+DI+52], 14",    DATA32, 0xC1, 0171, 0x34, 14);
        TEST("SAR", "DWORD [BP+SI+1234H], 15", DATA32, 0xC1, 0272, 0x34, 0x12, 15);
        ERRT("SAR", "DWORD [1234H], 32", OVERFLOW_RANGE, "32", DATA32, 0xC1, 0076, 0x34, 0x12, 32);
    }
    // SAR r/m8,CL
    TEST("SAR", "CH, CL",                 0xD2, 0375);
    TEST("SAR", "BYTE [SI], CL",          0xD2, 0074);
    TEST("SAR", "BYTE [1234H], CL",       0xD2, 0076, 0x34, 0x12);
    TEST("SAR", "BYTE [DI-52], CL",       0xD2, 0175, 0xCC);
    TEST("SAR", "BYTE [BP+1234H], CL",    0xD2, 0276, 0x34, 0x12);
    TEST("SAR", "BYTE [BX+SI], CL",       0xD2, 0070);
    TEST("SAR", "BYTE [BX+DI+52], CL",    0xD2, 0171, 0x34);
    TEST("SAR", "BYTE [BP+SI+1234H], CL", 0xD2, 0272, 0x34, 0x12);
    // SAR r/m16,CL
    TEST("SAR", "BP, CL",                 0xD3, 0375);
    TEST("SAR", "WORD [SI], CL",          0xD3, 0074);
    TEST("SAR", "WORD [1234H], CL",       0xD3, 0076, 0x34, 0x12);
    TEST("SAR", "WORD [DI-52], CL",       0xD3, 0175, 0xCC);
    TEST("SAR", "WORD [BP+1234H], CL",    0xD3, 0276, 0x34, 0x12);
    TEST("SAR", "WORD [BX+SI], CL",       0xD3, 0070);
    TEST("SAR", "WORD [BX+DI+52], CL",    0xD3, 0171, 0x34);
    TEST("SAR", "WORD [BP+SI+1234H], CL", 0xD3, 0272, 0x34, 0x12);
    if (is80386()) {
        // SAR r/m32,CL
        TEST("SAR", "EBP, CL",                 DATA32, 0xD3, 0375);
        TEST("SAR", "DWORD [SI], CL",          DATA32, 0xD3, 0074);
        TEST("SAR", "DWORD [1234H], CL",       DATA32, 0xD3, 0076, 0x34, 0x12);
        TEST("SAR", "DWORD [DI-52], CL",       DATA32, 0xD3, 0175, 0xCC);
        TEST("SAR", "DWORD [BP+1234H], CL",    DATA32, 0xD3, 0276, 0x34, 0x12);
        TEST("SAR", "DWORD [BX+SI], CL",       DATA32, 0xD3, 0070);
        TEST("SAR", "DWORD [BX+DI+52], CL",    DATA32, 0xD3, 0171, 0x34);
        TEST("SAR", "DWORD [BP+SI+1234H], CL", DATA32, 0xD3, 0272, 0x34, 0x12);
    }

    // ROL r/m8,1
    TEST("ROL", "CH, 1",                 0xD0, 0305);
    TEST("ROL", "BYTE [SI], 1",          0xD0, 0004);
    TEST("ROL", "BYTE [1234H], 1",       0xD0, 0006, 0x34, 0x12);
    TEST("ROL", "BYTE [DI-52], 1",       0xD0, 0105, 0xCC);
    TEST("ROL", "BYTE [BP+1234H], 1",    0xD0, 0206, 0x34, 0x12);
    TEST("ROL", "BYTE [BX+SI], 1",       0xD0, 0000);
    TEST("ROL", "BYTE [BX+DI+52], 1",    0xD0, 0101, 0x34);
    TEST("ROL", "BYTE [BP+SI+1234H], 1", 0xD0, 0202, 0x34, 0x12);
    // ROL r/m16,1
    TEST("ROL", "BP, 1",                 0xD1, 0305);
    TEST("ROL", "WORD [SI], 1",          0xD1, 0004);
    TEST("ROL", "WORD [1234H], 1",       0xD1, 0006, 0x34, 0x12);
    TEST("ROL", "WORD [DI-52], 1",       0xD1, 0105, 0xCC);
    TEST("ROL", "WORD [BP+1234H], 1",    0xD1, 0206, 0x34, 0x12);
    TEST("ROL", "WORD [BX+SI], 1",       0xD1, 0000);
    TEST("ROL", "WORD [BX+DI+52], 1",    0xD1, 0101, 0x34);
    TEST("ROL", "WORD [BP+SI+1234H], 1", 0xD1, 0202, 0x34, 0x12);
    if (is80x86()) {
        // ROL r/m8,imm8
        TEST("ROL", "CH, 4",                 0xC0, 0305, 4);
        TEST("ROL", "BYTE [SI], 5",          0xC0, 0004, 5);
        TEST("ROL", "BYTE [1234H], 6",       0xC0, 0006, 0x34, 0x12, 6);
        TEST("ROL", "BYTE [DI-52], 7",       0xC0, 0105, 0xCC, 7);
        TEST("ROL", "BYTE [BP+1234H], 0",    0xC0, 0206, 0x34, 0x12, 0);
        TEST("ROL", "BYTE [BX+SI], 1",       0xC0, 0000, 1);
        TEST("ROL", "BYTE [BX+DI+52], 2",    0xC0, 0101, 0x34, 2);
        TEST("ROL", "BYTE [BP+SI+1234H], 3", 0xC0, 0202, 0x34, 0x12, 3);
        ERRT("ROL", "BYTE [DI-52], 8",  OVERFLOW_RANGE,  "8", 0xC0, 0105, 0xCC, 8);
        // ROL r/m16,imm8
        TEST("ROL", "BP, 8",                  0xC1, 0305, 8);
        TEST("ROL", "WORD [SI], 9",           0xC1, 0004, 9);
        TEST("ROL", "WORD [1234H], 10",       0xC1, 0006, 0x34, 0x12, 10);
        TEST("ROL", "WORD [DI-52], 1",        0xC1, 0105, 0xCC, 1);
        TEST("ROL", "WORD [BP+1234H], 12",    0xC1, 0206, 0x34, 0x12, 12);
        TEST("ROL", "WORD [BX+SI], 13",       0xC1, 0000, 13);
        TEST("ROL", "WORD [BX+DI+52], 14",    0xC1, 0101, 0x34, 14);
        TEST("ROL", "WORD [BP+SI+1234H], 15", 0xC1, 0202, 0x34, 0x12, 15);
        ERRT("ROL", "WORD [DI-52], 16", OVERFLOW_RANGE, "16", 0xC1, 0105, 0xCC, 16);
    }
    if (is80386()) {
        // ROL r/m32,1
        TEST("ROL", "EBP, 1",                 DATA32, 0xD1, 0305);
        TEST("ROL", "DWORD [SI], 1",          DATA32, 0xD1, 0004);
        TEST("ROL", "DWORD [1234H], 1",       DATA32, 0xD1, 0006, 0x34, 0x12);
        TEST("ROL", "DWORD [DI-52], 1",       DATA32, 0xD1, 0105, 0xCC);
        TEST("ROL", "DWORD [BP+1234H], 1",    DATA32, 0xD1, 0206, 0x34, 0x12);
        TEST("ROL", "DWORD [BX+SI], 1",       DATA32, 0xD1, 0000);
        TEST("ROL", "DWORD [BX+DI+52], 1",    DATA32, 0xD1, 0101, 0x34);
        TEST("ROL", "DWORD [BP+SI+1234H], 1", DATA32, 0xD1, 0202, 0x34, 0x12);
        // ROL r/m32,imm8
        TEST("ROL", "EBP, 16",                 DATA32, 0xC1, 0305, 16);
        TEST("ROL", "DWORD [SI], 9",           DATA32, 0xC1, 0004, 9);
        TEST("ROL", "DWORD [1234H], 10",       DATA32, 0xC1, 0006, 0x34, 0x12, 10);
        TEST("ROL", "DWORD [DI-52], 1",        DATA32, 0xC1, 0105, 0xCC, 1);
        TEST("ROL", "DWORD [BP+1234H], 12",    DATA32, 0xC1, 0206, 0x34, 0x12, 12);
        TEST("ROL", "DWORD [BX+SI], 13",       DATA32, 0xC1, 0000, 13);
        TEST("ROL", "DWORD [BX+DI+52], 14",    DATA32, 0xC1, 0101, 0x34, 14);
        TEST("ROL", "DWORD [BP+SI+1234H], 15", DATA32, 0xC1, 0202, 0x34, 0x12, 15);
        ERRT("ROL", "DWORD [DI-52], 32", OVERFLOW_RANGE, "32", DATA32, 0xC1, 0105, 0xCC, 32);
    }
    // ROL r/m8,CL
    TEST("ROL", "CH, CL",                 0xD2, 0305);
    TEST("ROL", "BYTE [SI], CL",          0xD2, 0004);
    TEST("ROL", "BYTE [1234H], CL",       0xD2, 0006, 0x34, 0x12);
    TEST("ROL", "BYTE [DI-52], CL",       0xD2, 0105, 0xCC);
    TEST("ROL", "BYTE [BP+1234H], CL",    0xD2, 0206, 0x34, 0x12);
    TEST("ROL", "BYTE [BX+SI], CL",       0xD2, 0000);
    TEST("ROL", "BYTE [BX+DI+52], CL",    0xD2, 0101, 0x34);
    TEST("ROL", "BYTE [BP+SI+1234H], CL", 0xD2, 0202, 0x34, 0x12);
    // ROL r/m16,CL
    TEST("ROL", "BP, CL",                 0xD3, 0305);
    TEST("ROL", "WORD [SI], CL",          0xD3, 0004);
    TEST("ROL", "WORD [1234H], CL",       0xD3, 0006, 0x34, 0x12);
    TEST("ROL", "WORD [DI-52], CL",       0xD3, 0105, 0xCC);
    TEST("ROL", "WORD [BP+1234H], CL",    0xD3, 0206, 0x34, 0x12);
    TEST("ROL", "WORD [BX+SI], CL",       0xD3, 0000);
    TEST("ROL", "WORD [BX+DI+52], CL",    0xD3, 0101, 0x34);
    TEST("ROL", "WORD [BP+SI+1234H], CL", 0xD3, 0202, 0x34, 0x12);
    if (is80386()) {
        // ROL r/m32,CL
        TEST("ROL", "EBP, CL",                 DATA32, 0xD3, 0305);
        TEST("ROL", "DWORD [SI], CL",          DATA32, 0xD3, 0004);
        TEST("ROL", "DWORD [1234H], CL",       DATA32, 0xD3, 0006, 0x34, 0x12);
        TEST("ROL", "DWORD [DI-52], CL",       DATA32, 0xD3, 0105, 0xCC);
        TEST("ROL", "DWORD [BP+1234H], CL",    DATA32, 0xD3, 0206, 0x34, 0x12);
        TEST("ROL", "DWORD [BX+SI], CL",       DATA32, 0xD3, 0000);
        TEST("ROL", "DWORD [BX+DI+52], CL",    DATA32, 0xD3, 0101, 0x34);
        TEST("ROL", "DWORD [BP+SI+1234H], CL", DATA32, 0xD3, 0202, 0x34, 0x12);
    }

    // ROR r/m8,1
    TEST("ROR", "CH, 1",                 0xD0, 0315);
    TEST("ROR", "BYTE [SI], 1",          0xD0, 0014);
    TEST("ROR", "BYTE [1234H], 1",       0xD0, 0016, 0x34, 0x12);
    TEST("ROR", "BYTE [DI-52], 1",       0xD0, 0115, 0xCC);
    TEST("ROR", "BYTE [BP+1234H], 1",    0xD0, 0216, 0x34, 0x12);
    TEST("ROR", "BYTE [BX+SI], 1",       0xD0, 0010);
    TEST("ROR", "BYTE [BX+DI+52], 1",    0xD0, 0111, 0x34);
    TEST("ROR", "BYTE [BP+SI+1234H], 1", 0xD0, 0212, 0x34, 0x12);
    // ROR r/m16,1
    TEST("ROR", "BP, 1",                 0xD1, 0315);
    TEST("ROR", "WORD [SI], 1",          0xD1, 0014);
    TEST("ROR", "WORD [1234H], 1",       0xD1, 0016, 0x34, 0x12);
    TEST("ROR", "WORD [DI-52], 1",       0xD1, 0115, 0xCC);
    TEST("ROR", "WORD [BP+1234H], 1",    0xD1, 0216, 0x34, 0x12);
    TEST("ROR", "WORD [BX+SI], 1",       0xD1, 0010);
    TEST("ROR", "WORD [BX+DI+52], 1",    0xD1, 0111, 0x34);
    TEST("ROR", "WORD [BP+SI+1234H], 1", 0xD1, 0212, 0x34, 0x12);
    if (is80x86()) {
        // ROR r/m8,imm8
        TEST("ROR", "CH, 5",                 0xC0, 0315, 5);
        TEST("ROR", "BYTE [SI], 6",          0xC0, 0014, 6);
        TEST("ROR", "BYTE [1234H], 7",       0xC0, 0016, 0x34, 0x12, 7);
        TEST("ROR", "BYTE [DI-52], 0",       0xC0, 0115, 0xCC, 0);
        TEST("ROR", "BYTE [BP+1234H], 1",    0xC0, 0216, 0x34, 0x12, 1);
        TEST("ROR", "BYTE [BX+SI], 2",       0xC0, 0010, 2);
        TEST("ROR", "BYTE [BX+DI+52], 3",    0xC0, 0111, 0x34, 3);
        TEST("ROR", "BYTE [BP+SI+1234H], 4", 0xC0, 0212, 0x34, 0x12, 4);
        ERRT("ROR", "BYTE [BP+1234H], 8",  OVERFLOW_RANGE,  "8", 0xC0, 0216, 0x34, 0x12, 8);
        // ROR r/m16,imm8
        TEST("ROR", "BP, 8",                  0xC1, 0315, 8);
        TEST("ROR", "WORD [SI], 9",           0xC1, 0014, 9);
        TEST("ROR", "WORD [1234H], 10",       0xC1, 0016, 0x34, 0x12, 10);
        TEST("ROR", "WORD [DI-52], 11",       0xC1, 0115, 0xCC, 11);
        TEST("ROR", "WORD [BP+1234H], 1",     0xC1, 0216, 0x34, 0x12, 1);
        TEST("ROR", "WORD [BX+SI], 13",       0xC1, 0010, 13);
        TEST("ROR", "WORD [BX+DI+52], 14",    0xC1, 0111, 0x34, 14);
        TEST("ROR", "WORD [BP+SI+1234H], 15", 0xC1, 0212, 0x34, 0x12, 15);
        ERRT("ROR", "WORD [BP+1234H], 16", OVERFLOW_RANGE, "16", 0xC1, 0216, 0x34, 0x12, 16);
    }
    if (is80386()) {
        // ROR r/m32,1
        TEST("ROR", "EBP, 1",                 DATA32, 0xD1, 0315);
        TEST("ROR", "DWORD [SI], 1",          DATA32, 0xD1, 0014);
        TEST("ROR", "DWORD [1234H], 1",       DATA32, 0xD1, 0016, 0x34, 0x12);
        TEST("ROR", "DWORD [DI-52], 1",       DATA32, 0xD1, 0115, 0xCC);
        TEST("ROR", "DWORD [BP+1234H], 1",    DATA32, 0xD1, 0216, 0x34, 0x12);
        TEST("ROR", "DWORD [BX+SI], 1",       DATA32, 0xD1, 0010);
        TEST("ROR", "DWORD [BX+DI+52], 1",    DATA32, 0xD1, 0111, 0x34);
        TEST("ROR", "DWORD [BP+SI+1234H], 1", DATA32, 0xD1, 0212, 0x34, 0x12);
        // ROR r/m32,imm8
        TEST("ROR", "EBP, 16",                 DATA32, 0xC1, 0315, 16);
        TEST("ROR", "DWORD [SI], 9",           DATA32, 0xC1, 0014, 9);
        TEST("ROR", "DWORD [1234H], 10",       DATA32, 0xC1, 0016, 0x34, 0x12, 10);
        TEST("ROR", "DWORD [DI-52], 11",       DATA32, 0xC1, 0115, 0xCC, 11);
        TEST("ROR", "DWORD [BP+1234H], 1",     DATA32, 0xC1, 0216, 0x34, 0x12, 1);
        TEST("ROR", "DWORD [BX+SI], 13",       DATA32, 0xC1, 0010, 13);
        TEST("ROR", "DWORD [BX+DI+52], 14",    DATA32, 0xC1, 0111, 0x34, 14);
        TEST("ROR", "DWORD [BP+SI+1234H], 15", DATA32, 0xC1, 0212, 0x34, 0x12, 15);
        ERRT("ROR", "DWORD [BP+1234H], 32", OVERFLOW_RANGE, "32", DATA32, 0xC1, 0216, 0x34, 0x12, 32);
    }
    // ROR r/m8,CL
    TEST("ROR", "CH, CL",                 0xD2, 0315);
    TEST("ROR", "BYTE [SI], CL",          0xD2, 0014);
    TEST("ROR", "BYTE [1234H], CL",       0xD2, 0016, 0x34, 0x12);
    TEST("ROR", "BYTE [DI-52], CL",       0xD2, 0115, 0xCC);
    TEST("ROR", "BYTE [BP+1234H], CL",    0xD2, 0216, 0x34, 0x12);
    TEST("ROR", "BYTE [BX+SI], CL",       0xD2, 0010);
    TEST("ROR", "BYTE [BX+DI+52], CL",    0xD2, 0111, 0x34);
    TEST("ROR", "BYTE [BP+SI+1234H], CL", 0xD2, 0212, 0x34, 0x12);
    // ROR r/m16,CL
    TEST("ROR", "BP, CL",                 0xD3, 0315);
    TEST("ROR", "WORD [SI], CL",          0xD3, 0014);
    TEST("ROR", "WORD [1234H], CL",       0xD3, 0016, 0x34, 0x12);
    TEST("ROR", "WORD [DI-52], CL",       0xD3, 0115, 0xCC);
    TEST("ROR", "WORD [BP+1234H], CL",    0xD3, 0216, 0x34, 0x12);
    TEST("ROR", "WORD [BX+SI], CL",       0xD3, 0010);
    TEST("ROR", "WORD [BX+DI+52], CL",    0xD3, 0111, 0x34);
    TEST("ROR", "WORD [BP+SI+1234H], CL", 0xD3, 0212, 0x34, 0x12);
    if (is80386()) {
        // ROR r/m23,CL
        TEST("ROR", "EBP, CL",                 DATA32, 0xD3, 0315);
        TEST("ROR", "DWORD [SI], CL",          DATA32, 0xD3, 0014);
        TEST("ROR", "DWORD [1234H], CL",       DATA32, 0xD3, 0016, 0x34, 0x12);
        TEST("ROR", "DWORD [DI-52], CL",       DATA32, 0xD3, 0115, 0xCC);
        TEST("ROR", "DWORD [BP+1234H], CL",    DATA32, 0xD3, 0216, 0x34, 0x12);
        TEST("ROR", "DWORD [BX+SI], CL",       DATA32, 0xD3, 0010);
        TEST("ROR", "DWORD [BX+DI+52], CL",    DATA32, 0xD3, 0111, 0x34);
        TEST("ROR", "DWORD [BP+SI+1234H], CL", DATA32, 0xD3, 0212, 0x34, 0x12);
    }

    // RCL r/m8,1
    TEST("RCL", "CH, 1",                 0xD0, 0325);
    TEST("RCL", "BYTE [SI], 1",          0xD0, 0024);
    TEST("RCL", "BYTE [1234H], 1",       0xD0, 0026, 0x34, 0x12);
    TEST("RCL", "BYTE [DI-52], 1",       0xD0, 0125, 0xCC);
    TEST("RCL", "BYTE [BP+1234H], 1",    0xD0, 0226, 0x34, 0x12);
    TEST("RCL", "BYTE [BX+SI], 1",       0xD0, 0020);
    TEST("RCL", "BYTE [BX+DI+52], 1",    0xD0, 0121, 0x34);
    TEST("RCL", "BYTE [BP+SI+1234H], 1", 0xD0, 0222, 0x34, 0x12);
    // RCL r/m16,1
    TEST("RCL", "BP, 1",                 0xD1, 0325);
    TEST("RCL", "WORD [SI], 1",          0xD1, 0024);
    TEST("RCL", "WORD [1234H], 1",       0xD1, 0026, 0x34, 0x12);
    TEST("RCL", "WORD [DI-52], 1",       0xD1, 0125, 0xCC);
    TEST("RCL", "WORD [BP+1234H], 1",    0xD1, 0226, 0x34, 0x12);
    TEST("RCL", "WORD [BX+SI], 1",       0xD1, 0020);
    TEST("RCL", "WORD [BX+DI+52], 1",    0xD1, 0121, 0x34);
    TEST("RCL", "WORD [BP+SI+1234H], 1", 0xD1, 0222, 0x34, 0x12);
    if (is80x86()) {
        // RCL r/m8,imm8
        TEST("RCL", "CH, 6",                 0xC0, 0325, 6);
        TEST("RCL", "BYTE [SI], 7",          0xC0, 0024, 7);
        TEST("RCL", "BYTE [1234H], 0",       0xC0, 0026, 0x34, 0x12, 0);
        TEST("RCL", "BYTE [DI-52], 1",       0xC0, 0125, 0xCC, 1);
        TEST("RCL", "BYTE [BP+1234H], 2",    0xC0, 0226, 0x34, 0x12, 2);
        TEST("RCL", "BYTE [BX+SI], 3",       0xC0, 0020, 3);
        TEST("RCL", "BYTE [BX+DI+52], 4",    0xC0, 0121, 0x34, 4);
        TEST("RCL", "BYTE [BP+SI+1234H], 5", 0xC0, 0222, 0x34, 0x12, 5);
        ERRT("RCL", "BYTE [BX+SI], 8",  OVERFLOW_RANGE,  "8", 0xC0, 0020, 8);
        // RCL r/m16,1mm8
        TEST("RCL", "BP, 8",                  0xC1, 0325, 8);
        TEST("RCL", "WORD [SI], 9",           0xC1, 0024, 9);
        TEST("RCL", "WORD [1234H], 10",       0xC1, 0026, 0x34, 0x12, 10);
        TEST("RCL", "WORD [DI-52], 11",       0xC1, 0125, 0xCC, 11);
        TEST("RCL", "WORD [BP+1234H], 12",    0xC1, 0226, 0x34, 0x12, 12);
        TEST("RCL", "WORD [BX+SI], 1",        0xC1, 0020, 1);
        TEST("RCL", "WORD [BX+DI+52], 14",    0xC1, 0121, 0x34, 14);
        TEST("RCL", "WORD [BP+SI+1234H], 15", 0xC1, 0222, 0x34, 0x12, 15);
        ERRT("RCL", "WORD [BX+SI], 16", OVERFLOW_RANGE, "16", 0xC1, 0020, 16);
    }
    if (is80386()) {
        // RCL r/m32,1
        TEST("RCL", "EBP, 1",                 DATA32, 0xD1, 0325);
        TEST("RCL", "DWORD [SI], 1",          DATA32, 0xD1, 0024);
        TEST("RCL", "DWORD [1234H], 1",       DATA32, 0xD1, 0026, 0x34, 0x12);
        TEST("RCL", "DWORD [DI-52], 1",       DATA32, 0xD1, 0125, 0xCC);
        TEST("RCL", "DWORD [BP+1234H], 1",    DATA32, 0xD1, 0226, 0x34, 0x12);
        TEST("RCL", "DWORD [BX+SI], 1",       DATA32, 0xD1, 0020);
        TEST("RCL", "DWORD [BX+DI+52], 1",    DATA32, 0xD1, 0121, 0x34);
        TEST("RCL", "DWORD [BP+SI+1234H], 1", DATA32, 0xD1, 0222, 0x34, 0x12);
        // RCL r/m32,imm8
        TEST("RCL", "EBP, 16",                 DATA32, 0xC1, 0325, 16);
        TEST("RCL", "DWORD [SI], 9",           DATA32, 0xC1, 0024, 9);
        TEST("RCL", "DWORD [1234H], 10",       DATA32, 0xC1, 0026, 0x34, 0x12, 10);
        TEST("RCL", "DWORD [DI-52], 11",       DATA32, 0xC1, 0125, 0xCC, 11);
        TEST("RCL", "DWORD [BP+1234H], 12",    DATA32, 0xC1, 0226, 0x34, 0x12, 12);
        TEST("RCL", "DWORD [BX+SI], 1",        DATA32, 0xC1, 0020, 1);
        TEST("RCL", "DWORD [BX+DI+52], 14",    DATA32, 0xC1, 0121, 0x34, 14);
        TEST("RCL", "DWORD [BP+SI+1234H], 15", DATA32, 0xC1, 0222, 0x34, 0x12, 15);
        ERRT("RCL", "DWORD [BX+SI], 32", OVERFLOW_RANGE, "32", DATA32, 0xC1, 0020, 32);
    }
    // RCL r/m8,CL
    TEST("RCL", "CH, CL",                 0xD2, 0325);
    TEST("RCL", "BYTE [SI], CL",          0xD2, 0024);
    TEST("RCL", "BYTE [1234H], CL",       0xD2, 0026, 0x34, 0x12);
    TEST("RCL", "BYTE [DI-52], CL",       0xD2, 0125, 0xCC);
    TEST("RCL", "BYTE [BP+1234H], CL",    0xD2, 0226, 0x34, 0x12);
    TEST("RCL", "BYTE [BX+SI], CL",       0xD2, 0020);
    TEST("RCL", "BYTE [BX+DI+52], CL",    0xD2, 0121, 0x34);
    TEST("RCL", "BYTE [BP+SI+1234H], CL", 0xD2, 0222, 0x34, 0x12);
    // RCL r/m16,CL
    TEST("RCL", "BP, CL",                 0xD3, 0325);
    TEST("RCL", "WORD [SI], CL",          0xD3, 0024);
    TEST("RCL", "WORD [1234H], CL",       0xD3, 0026, 0x34, 0x12);
    TEST("RCL", "WORD [DI-52], CL",       0xD3, 0125, 0xCC);
    TEST("RCL", "WORD [BP+1234H], CL",    0xD3, 0226, 0x34, 0x12);
    TEST("RCL", "WORD [BX+SI], CL",       0xD3, 0020);
    TEST("RCL", "WORD [BX+DI+52], CL",    0xD3, 0121, 0x34);
    TEST("RCL", "WORD [BP+SI+1234H], CL", 0xD3, 0222, 0x34, 0x12);
    if (is80386()) {
        // RCL r/m32,CL
        TEST("RCL", "EBP, CL",                 DATA32, 0xD3, 0325);
        TEST("RCL", "DWORD [SI], CL",          DATA32, 0xD3, 0024);
        TEST("RCL", "DWORD [1234H], CL",       DATA32, 0xD3, 0026, 0x34, 0x12);
        TEST("RCL", "DWORD [DI-52], CL",       DATA32, 0xD3, 0125, 0xCC);
        TEST("RCL", "DWORD [BP+1234H], CL",    DATA32, 0xD3, 0226, 0x34, 0x12);
        TEST("RCL", "DWORD [BX+SI], CL",       DATA32, 0xD3, 0020);
        TEST("RCL", "DWORD [BX+DI+52], CL",    DATA32, 0xD3, 0121, 0x34);
        TEST("RCL", "DWORD [BP+SI+1234H], CL", DATA32, 0xD3, 0222, 0x34, 0x12);
    }

    // RCR r/m8,1
    TEST("RCR", "CH, 1",                 0xD0, 0335);
    TEST("RCR", "BYTE [SI], 1",          0xD0, 0034);
    TEST("RCR", "BYTE [1234H], 1",       0xD0, 0036, 0x34, 0x12);
    TEST("RCR", "BYTE [DI-52], 1",       0xD0, 0135, 0xCC);
    TEST("RCR", "BYTE [BP+1234H], 1",    0xD0, 0236, 0x34, 0x12);
    TEST("RCR", "BYTE [BX+SI], 1",       0xD0, 0030);
    TEST("RCR", "BYTE [BX+DI+52], 1",    0xD0, 0131, 0x34);
    TEST("RCR", "BYTE [BP+SI+1234H], 1", 0xD0, 0232, 0x34, 0x12);
    // RCR r/m16,1
    TEST("RCR", "BP, 1",                 0xD1, 0335);
    TEST("RCR", "WORD [SI], 1",          0xD1, 0034);
    TEST("RCR", "WORD [1234H], 1",       0xD1, 0036, 0x34, 0x12);
    TEST("RCR", "WORD [DI-52], 1",       0xD1, 0135, 0xCC);
    TEST("RCR", "WORD [BP+1234H], 1",    0xD1, 0236, 0x34, 0x12);
    TEST("RCR", "WORD [BX+SI], 1",       0xD1, 0030);
    TEST("RCR", "WORD [BX+DI+52], 1",    0xD1, 0131, 0x34);
    TEST("RCR", "WORD [BP+SI+1234H], 1", 0xD1, 0232, 0x34, 0x12);
    if (is80x86()) {
        // RCR r/m8,imm8
        TEST("RCR", "CH, 7",                 0xC0, 0335, 7);
        TEST("RCR", "BYTE [SI], 0",          0xC0, 0034, 0);
        TEST("RCR", "BYTE [1234H], 1",       0xC0, 0036, 0x34, 0x12, 1);
        TEST("RCR", "BYTE [DI-52], 2",       0xC0, 0135, 0xCC, 2);
        TEST("RCR", "BYTE [BP+1234H], 3",    0xC0, 0236, 0x34, 0x12, 3);
        TEST("RCR", "BYTE [BX+SI], 4",       0xC0, 0030, 4);
        TEST("RCR", "BYTE [BX+DI+52], 5",    0xC0, 0131, 0x34, 5);
        TEST("RCR", "BYTE [BP+SI+1234H], 6", 0xC0, 0232, 0x34, 0x12, 6);
        ERRT("RCR", "BYTE [BX+DI+52], 8",  OVERFLOW_RANGE,  "8", 0xC0, 0131, 0x34, 8);
        // RCR r/m16,imm8
        TEST("RCR", "BP, 8",                  0xC1, 0335, 8);
        TEST("RCR", "WORD [SI], 9",           0xC1, 0034, 9);
        TEST("RCR", "WORD [1234H], 10",       0xC1, 0036, 0x34, 0x12, 10);
        TEST("RCR", "WORD [DI-52], 11",       0xC1, 0135, 0xCC, 11);
        TEST("RCR", "WORD [BP+1234H], 12",    0xC1, 0236, 0x34, 0x12, 12);
        TEST("RCR", "WORD [BX+SI], 13",       0xC1, 0030, 13);
        TEST("RCR", "WORD [BX+DI+52], 1",     0xC1, 0131, 0x34, 1);
        TEST("RCR", "WORD [BP+SI+1234H], 15", 0xC1, 0232, 0x34, 0x12, 15);
        ERRT("RCR", "WORD [BX+DI+52], 16", OVERFLOW_RANGE, "16", 0xC1, 0131, 0x34, 16);
    }
    if (is80386()) {
        // RCR r/m32,1
        TEST("RCR", "EBP, 1",                 DATA32, 0xD1, 0335);
        TEST("RCR", "DWORD [SI], 1",          DATA32, 0xD1, 0034);
        TEST("RCR", "DWORD [1234H], 1",       DATA32, 0xD1, 0036, 0x34, 0x12);
        TEST("RCR", "DWORD [DI-52], 1",       DATA32, 0xD1, 0135, 0xCC);
        TEST("RCR", "DWORD [BP+1234H], 1",    DATA32, 0xD1, 0236, 0x34, 0x12);
        TEST("RCR", "DWORD [BX+SI], 1",       DATA32, 0xD1, 0030);
        TEST("RCR", "DWORD [BX+DI+52], 1",    DATA32, 0xD1, 0131, 0x34);
        TEST("RCR", "DWORD [BP+SI+1234H], 1", DATA32, 0xD1, 0232, 0x34, 0x12);
        // RCR r/m32,imm8
        TEST("RCR", "EBP, 16",                 DATA32, 0xC1, 0335, 16);
        TEST("RCR", "DWORD [SI], 9",           DATA32, 0xC1, 0034, 9);
        TEST("RCR", "DWORD [1234H], 10",       DATA32, 0xC1, 0036, 0x34, 0x12, 10);
        TEST("RCR", "DWORD [DI-52], 11",       DATA32, 0xC1, 0135, 0xCC, 11);
        TEST("RCR", "DWORD [BP+1234H], 12",    DATA32, 0xC1, 0236, 0x34, 0x12, 12);
        TEST("RCR", "DWORD [BX+SI], 13",       DATA32, 0xC1, 0030, 13);
        TEST("RCR", "DWORD [BX+DI+52], 1",     DATA32, 0xC1, 0131, 0x34, 1);
        TEST("RCR", "DWORD [BP+SI+1234H], 15", DATA32, 0xC1, 0232, 0x34, 0x12, 15);
        ERRT("RCR", "DWORD [BX+DI+52], 32", OVERFLOW_RANGE, "32", DATA32, 0xC1, 0131, 0x34, 32);
    }
    // RCR r/m8,CL
    TEST("RCR", "CH, CL",                 0xD2, 0335);
    TEST("RCR", "BYTE [SI], CL",          0xD2, 0034);
    TEST("RCR", "BYTE [1234H], CL",       0xD2, 0036, 0x34, 0x12);
    TEST("RCR", "BYTE [DI-52], CL",       0xD2, 0135, 0xCC);
    TEST("RCR", "BYTE [BP+1234H], CL",    0xD2, 0236, 0x34, 0x12);
    TEST("RCR", "BYTE [BX+SI], CL",       0xD2, 0030);
    TEST("RCR", "BYTE [BX+DI+52], CL",    0xD2, 0131, 0x34);
    TEST("RCR", "BYTE [BP+SI+1234H], CL", 0xD2, 0232, 0x34, 0x12);
    // RCR r/m16,CL
    TEST("RCR", "BP, CL",                 0xD3, 0335);
    TEST("RCR", "WORD [SI], CL",          0xD3, 0034);
    TEST("RCR", "WORD [1234H], CL",       0xD3, 0036, 0x34, 0x12);
    TEST("RCR", "WORD [DI-52], CL",       0xD3, 0135, 0xCC);
    TEST("RCR", "WORD [BP+1234H], CL",    0xD3, 0236, 0x34, 0x12);
    TEST("RCR", "WORD [BX+SI], CL",       0xD3, 0030);
    TEST("RCR", "WORD [BX+DI+52], CL",    0xD3, 0131, 0x34);
    TEST("RCR", "WORD [BP+SI+1234H], CL", 0xD3, 0232, 0x34, 0x12);
    if (is80386()) {
        // RCR r/m32,CL
        TEST("RCR", "EBP, CL",                 DATA32, 0xD3, 0335);
        TEST("RCR", "DWORD [SI], CL",          DATA32, 0xD3, 0034);
        TEST("RCR", "DWORD [1234H], CL",       DATA32, 0xD3, 0036, 0x34, 0x12);
        TEST("RCR", "DWORD [DI-52], CL",       DATA32, 0xD3, 0135, 0xCC);
        TEST("RCR", "DWORD [BP+1234H], CL",    DATA32, 0xD3, 0236, 0x34, 0x12);
        TEST("RCR", "DWORD [BX+SI], CL",       DATA32, 0xD3, 0030);
        TEST("RCR", "DWORD [BX+DI+52], CL",    DATA32, 0xD3, 0131, 0x34);
        TEST("RCR", "DWORD [BP+SI+1234H], CL", DATA32, 0xD3, 0232, 0x34, 0x12);
    }

    if (is80386()) {
        // SHLD r/m16,r16,imm8
        TEST("SHLD", "BP, AX, 8",             0x0F, 0xA4, 0305, 8);
        TEST("SHLD", "[SI], CX, 9",           0x0F, 0xA4, 0014, 9);
        TEST("SHLD", "[1234H], DX, 10",       0x0F, 0xA4, 0026, 0x34, 0x12, 10);
        TEST("SHLD", "[DI-52], BX, 11",       0x0F, 0xA4, 0135, 0xCC, 11);
        TEST("SHLD", "[BP+1234H], SP, 12",    0x0F, 0xA4, 0246, 0x34, 0x12, 12);
        TEST("SHLD", "[BX+SI], BP, 13",       0x0F, 0xA4, 0050, 13);
        TEST("SHLD", "[BX+DI+52], SI, 1",     0x0F, 0xA4, 0161, 0x34, 1);
        TEST("SHLD", "[BP+SI+1234H], DI, 15", 0x0F, 0xA4, 0272, 0x34, 0x12, 15);
        ERRT("SHLD", "[BX+DI+52], BX, 16", OVERFLOW_RANGE, "16", 0x0F, 0xA4, 0131, 0x34, 16);
        // SHLD r/m32,r32,imm8
        TEST("SHLD", "EBP, EDI, 16",           DATA32, 0x0F, 0xA4, 0375, 16);
        TEST("SHLD", "[SI], ESI, 9",           DATA32, 0x0F, 0xA4, 0064, 9);
        TEST("SHLD", "[1234H], EBP, 10",       DATA32, 0x0F, 0xA4, 0056, 0x34, 0x12, 10);
        TEST("SHLD", "[DI-52], ESP, 11",       DATA32, 0x0F, 0xA4, 0145, 0xCC, 11);
        TEST("SHLD", "[BP+1234H], EBX, 12",    DATA32, 0x0F, 0xA4, 0236, 0x34, 0x12, 12);
        TEST("SHLD", "[BX+SI], EDX, 13",       DATA32, 0x0F, 0xA4, 0020, 13);
        TEST("SHLD", "[BX+DI+52], ECX, 1",     DATA32, 0x0F, 0xA4, 0111, 0x34, 1);
        TEST("SHLD", "[BP+SI+1234H], EAX, 15", DATA32, 0x0F, 0xA4, 0202, 0x34, 0x12, 15);
        ERRT("SHLD", "[BX+DI+52], EBX, 32", OVERFLOW_RANGE, "32", DATA32, 0x0F, 0xA4, 0131, 0x34, 32);
        // SHLD r/m16,r16,CL
        TEST("SHLD", "BP, AX, CL",            0x0F, 0xA5, 0305);
        TEST("SHLD", "[SI], CX, CL",          0x0F, 0xA5, 0014);
        TEST("SHLD", "[1234H], DX, CL",       0x0F, 0xA5, 0026, 0x34, 0x12);
        TEST("SHLD", "[DI-52], BX, CL",       0x0F, 0xA5, 0135, 0xCC);
        TEST("SHLD", "[BP+1234H], SP, CL",    0x0F, 0xA5, 0246, 0x34, 0x12);
        TEST("SHLD", "[BX+SI], BP, CL",       0x0F, 0xA5, 0050);
        TEST("SHLD", "[BX+DI+52], SI, CL",    0x0F, 0xA5, 0161, 0x34);
        TEST("SHLD", "[BP+SI+1234H], DI, CL", 0x0F, 0xA5, 0272, 0x34, 0x12);
        // SHLD r/m32,r32,CL
        TEST("SHLD", "EBP, EDI, CL",           DATA32, 0x0F, 0xA5, 0375);
        TEST("SHLD", "[SI], ESI, CL",          DATA32, 0x0F, 0xA5, 0064);
        TEST("SHLD", "[1234H], EBP, CL",       DATA32, 0x0F, 0xA5, 0056, 0x34, 0x12);
        TEST("SHLD", "[DI-52], ESP, CL",       DATA32, 0x0F, 0xA5, 0145, 0xCC);
        TEST("SHLD", "[BP+1234H], EBX, CL",    DATA32, 0x0F, 0xA5, 0236, 0x34, 0x12);
        TEST("SHLD", "[BX+SI], EDX, CL",       DATA32, 0x0F, 0xA5, 0020);
        TEST("SHLD", "[BX+DI+52], ECX, CL",    DATA32, 0x0F, 0xA5, 0111, 0x34);
        TEST("SHLD", "[BP+SI+1234H], EAX, CL", DATA32, 0x0F, 0xA5, 0202, 0x34, 0x12);
    }

    if (is80386()) {
        // SHRD r/m16,r16,imm8
        TEST("SHRD", "BP, AX, 8",             0x0F, 0xAC, 0305, 8);
        TEST("SHRD", "[SI], CX, 9",           0x0F, 0xAC, 0014, 9);
        TEST("SHRD", "[1234H], DX, 10",       0x0F, 0xAC, 0026, 0x34, 0x12, 10);
        TEST("SHRD", "[DI-52], BX, 11",       0x0F, 0xAC, 0135, 0xCC, 11);
        TEST("SHRD", "[BP+1234H], SP, 12",    0x0F, 0xAC, 0246, 0x34, 0x12, 12);
        TEST("SHRD", "[BX+SI], BP, 13",       0x0F, 0xAC, 0050, 13);
        TEST("SHRD", "[BX+DI+52], SI, 1",     0x0F, 0xAC, 0161, 0x34, 1);
        TEST("SHRD", "[BP+SI+1234H], DI, 15", 0x0F, 0xAC, 0272, 0x34, 0x12, 15);
        ERRT("SHRD", "[BX+DI+52], BX, 16", OVERFLOW_RANGE, "16", 0x0F, 0xAC, 0131, 0x34, 16);
        // SHRD r/m32,r32,imm8
        TEST("SHRD", "EBP, EDI, 16",           DATA32, 0x0F, 0xAC, 0375, 16);
        TEST("SHRD", "[SI], ESI, 9",           DATA32, 0x0F, 0xAC, 0064, 9);
        TEST("SHRD", "[1234H], EBP, 10",       DATA32, 0x0F, 0xAC, 0056, 0x34, 0x12, 10);
        TEST("SHRD", "[DI-52], ESP, 11",       DATA32, 0x0F, 0xAC, 0145, 0xCC, 11);
        TEST("SHRD", "[BP+1234H], EBX, 12",    DATA32, 0x0F, 0xAC, 0236, 0x34, 0x12, 12);
        TEST("SHRD", "[BX+SI], EDX, 13",       DATA32, 0x0F, 0xAC, 0020, 13);
        TEST("SHRD", "[BX+DI+52], ECX, 1",     DATA32, 0x0F, 0xAC, 0111, 0x34, 1);
        TEST("SHRD", "[BP+SI+1234H], EAX, 15", DATA32, 0x0F, 0xAC, 0202, 0x34, 0x12, 15);
        ERRT("SHRD", "[BX+DI+52], EBX, 32", OVERFLOW_RANGE, "32", DATA32, 0x0F, 0xAC, 0131, 0x34, 32);
        // SHRD r/m16,r16,CL
        TEST("SHRD", "BP, AX, CL",            0x0F, 0xAD, 0305);
        TEST("SHRD", "[SI], CX, CL",          0x0F, 0xAD, 0014);
        TEST("SHRD", "[1234H], DX, CL",       0x0F, 0xAD, 0026, 0x34, 0x12);
        TEST("SHRD", "[DI-52], BX, CL",       0x0F, 0xAD, 0135, 0xCC);
        TEST("SHRD", "[BP+1234H], SP, CL",    0x0F, 0xAD, 0246, 0x34, 0x12);
        TEST("SHRD", "[BX+SI], BP, CL",       0x0F, 0xAD, 0050);
        TEST("SHRD", "[BX+DI+52], SI, CL",    0x0F, 0xAD, 0161, 0x34);
        TEST("SHRD", "[BP+SI+1234H], DI, CL", 0x0F, 0xAD, 0272, 0x34, 0x12);
        // SHRD r/m32,r32,CL
        TEST("SHRD", "EBP, EDI, CL",           DATA32, 0x0F, 0xAD, 0375);
        TEST("SHRD", "[SI], ESI, CL",          DATA32, 0x0F, 0xAD, 0064);
        TEST("SHRD", "[1234H], EBP, CL",       DATA32, 0x0F, 0xAD, 0056, 0x34, 0x12);
        TEST("SHRD", "[DI-52], ESP, CL",       DATA32, 0x0F, 0xAD, 0145, 0xCC);
        TEST("SHRD", "[BP+1234H], EBX, CL",    DATA32, 0x0F, 0xAD, 0236, 0x34, 0x12);
        TEST("SHRD", "[BX+SI], EDX, CL",       DATA32, 0x0F, 0xAD, 0020);
        TEST("SHRD", "[BX+DI+52], ECX, CL",    DATA32, 0x0F, 0xAD, 0111, 0x34);
        TEST("SHRD", "[BP+SI+1234H], EAX, CL", DATA32, 0x0F, 0xAD, 0202, 0x34, 0x12);
    }

    // AND r/m8,r8
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
    // AND r8,r/m8
    TEST("AND", "DH, [SI]",          0x22, 0064);
    TEST("AND", "BH, [1234H]",       0x22, 0076, 0x34, 0x12);
    TEST("AND", "AL, [DI-52]",       0x22, 0105, 0xCC);
    TEST("AND", "CL, [BP+1234H]",    0x22, 0216, 0x34, 0x12);
    TEST("AND", "DL, [BX+SI]",       0x22, 0020);
    TEST("AND", "BL, [BX+DI-52]",    0x22, 0131, 0xCC);
    TEST("AND", "AH, [BP+SI+1234H]", 0x22, 0242, 0x34, 0x12);
    // AND r/m16,r16
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
    // AND r16,r/m16
    TEST("AND", "SI, [SI]",          0x23, 0064);
    TEST("AND", "DI, [1234H]",       0x23, 0076, 0x34, 0x12);
    TEST("AND", "AX, [DI+52]",       0x23, 0105, 0x34);
    TEST("AND", "CX, [BP+1234H]",    0x23, 0216, 0x34, 0x12);
    TEST("AND", "DX, [BX+SI]",       0x23, 0020);
    TEST("AND", "BX, [BX+DI+52]",    0x23, 0131, 0x34);
    TEST("AND", "SP, [BP+SI+1234H]", 0x23, 0242, 0x34, 0x12);
    if (is80386()) {
        // AND r/m32,r32
        TEST("AND", "EAX, ECX",           DATA32, 0x21, 0310);
        TEST("AND", "EDX, EBX",           DATA32, 0x21, 0332);
        TEST("AND", "ESP, EBP",           DATA32, 0x21, 0354);
        TEST("AND", "[SI], ESI",          DATA32, 0x21, 0064);
        TEST("AND", "[1234H], EDI",       DATA32, 0x21, 0076, 0x34, 0x12);
        TEST("AND", "[DI-52], EAX",       DATA32, 0x21, 0105, 0xCC);
        TEST("AND", "[BP+1234H], ECX",    DATA32, 0x21, 0216, 0x34, 0x12);
        TEST("AND", "[BX+SI], EDX",       DATA32, 0x21, 0020);
        TEST("AND", "[BX+DI+52], EBX",    DATA32, 0x21, 0131, 0x34);
        TEST("AND", "[BP+SI+1234H], ESP", DATA32, 0x21, 0242, 0x34, 0x12);
        // AND r32,r/m32
        TEST("AND", "ESI, [SI]",          DATA32, 0x23, 0064);
        TEST("AND", "EDI, [1234H]",       DATA32, 0x23, 0076, 0x34, 0x12);
        TEST("AND", "EAX, [DI+52]",       DATA32, 0x23, 0105, 0x34);
        TEST("AND", "ECX, [BP+1234H]",    DATA32, 0x23, 0216, 0x34, 0x12);
        TEST("AND", "EDX, [BX+SI]",       DATA32, 0x23, 0020);
        TEST("AND", "EBX, [BX+DI+52]",    DATA32, 0x23, 0131, 0x34);
        TEST("AND", "ESP, [BP+SI+1234H]", DATA32, 0x23, 0242, 0x34, 0x12);
    }

    // AND r/m8,imm8
    TEST("AND", "AL, 86",                   0x80, 0340, 0x56);
    TEST("AND", "CL, 86",                   0x80, 0341, 0x56);
    TEST("AND", "BYTE [SI], 86",            0x80, 0044, 0x56);
    TEST("AND", "BYTE [89ABH], 86",         0x80, 0046, 0xAB, 0x89, 0x56);
    TEST("AND", "BYTE [DI-52], 86",         0x80, 0145, 0xCC, 0x56);
    TEST("AND", "BYTE [BP+89ABH], 86",      0x80, 0246, 0xAB, 0x89, 0x56);
    TEST("AND", "BYTE [BX+SI], 86",         0x80, 0040, 0x56);
    TEST("AND", "BYTE [BX+DI+52], 86",      0x80, 0141, 0x34, 0x56);
    TEST("AND", "BYTE [BP+SI+1234H], 86",   0x80, 0242, 0x34, 0x12, 0x56);
    // AND r/m16,imm16
    TEST("AND", "AX, 5678H",                 0x81, 0340, 0x78, 0x56);
    TEST("AND", "CX, 5678H",                 0x81, 0341, 0x78, 0x56);
    TEST("AND", "WORD [SI], 5678H",          0x81, 0044, 0x78, 0x56);
    TEST("AND", "WORD [1234H], 5678H",       0x81, 0046, 0x34, 0x12, 0x78, 0x56);
    TEST("AND", "WORD [DI+52], 5678H",       0x81, 0145, 0x34, 0x78, 0x56);
    TEST("AND", "WORD [BP+1234H], 5678H",    0x81, 0246, 0x34, 0x12, 0x78, 0x56);
    TEST("AND", "WORD [BX+SI], 5678H",       0x81, 0040, 0x78, 0x56);
    TEST("AND", "WORD [BX+DI-52], 5678H",    0x81, 0141, 0xCC, 0x78, 0x56);
    TEST("AND", "WORD [BP+SI+89ABH], 5678H", 0x81, 0242, 0xAB, 0x89, 0x78, 0x56);
    // AND r/m16,imm8
    TEST("AND", "AX, 86",                   0x83, 0340, 0x56);
    TEST("AND", "AX, -16",                  0x83, 0340, 0xF0);
    TEST("AND", "CX, 86",                   0x83, 0341, 0x56);
    TEST("AND", "CX, -16",                  0x83, 0341, 0xF0);
    TEST("AND", "WORD [SI], 86",            0x83, 0044, 0x56);
    TEST("AND", "WORD [1234H], 86",         0x83, 0046, 0x34, 0x12, 0x56);
    TEST("AND", "WORD [DI+52], 86",         0x83, 0145, 0x34, 0x56);
    TEST("AND", "WORD [BP+1234H], 86",      0x83, 0246, 0x34, 0x12, 0x56);
    TEST("AND", "WORD [BX+SI], 86",         0x83, 0040, 0x56);
    TEST("AND", "WORD [BX+DI-52], 86",      0x83, 0141, 0xCC, 0x56);
    TEST("AND", "WORD [BP+SI+89ABH], 86",   0x83, 0242, 0xAB, 0x89, 0x56);
    if (is80386()) {
        // AND r/m32,imm32
        TEST("AND", "EAX, 12345678H",                 DATA32, 0x81, 0340, 0x78, 0x56, 0x34, 0x12);
        TEST("AND", "ECX, 12345678H",                 DATA32, 0x81, 0341, 0x78, 0x56, 0x34, 0x12);
        TEST("AND", "DWORD [SI], 12345678H",          DATA32, 0x81, 0044, 0x78, 0x56, 0x34, 0x12);
        TEST("AND", "DWORD [1234H], 12345678H",       DATA32, 0x81, 0046, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12);
        TEST("AND", "DWORD [DI+52], 12345678H",       DATA32, 0x81, 0145, 0x34, 0x78, 0x56, 0x34, 0x12);
        TEST("AND", "DWORD [BP+1234H], 12345678H",    DATA32, 0x81, 0246, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12);
        TEST("AND", "DWORD [BX+SI], 12345678H",       DATA32, 0x81, 0040, 0x78, 0x56, 0x34, 0x12);
        TEST("AND", "DWORD [BX+DI-52], 12345678H",    DATA32, 0x81, 0141, 0xCC, 0x78, 0x56, 0x34, 0x12);
        TEST("AND", "DWORD [BP+SI+89ABH], 12345678H", DATA32, 0x81, 0242, 0xAB, 0x89, 0x78, 0x56, 0x34, 0x12);
        // AND r/m32,imm8
        TEST("AND", "EAX, 86",                 DATA32, 0x83, 0340, 0x56);
        TEST("AND", "EAX, -16",                DATA32, 0x83, 0340, 0xF0);
        TEST("AND", "ECX, 86",                 DATA32, 0x83, 0341, 0x56);
        TEST("AND", "ECX, -16",                DATA32, 0x83, 0341, 0xF0);
        TEST("AND", "DWORD [SI], 86",          DATA32, 0x83, 0044, 0x56);
        TEST("AND", "DWORD [1234H], 86",       DATA32, 0x83, 0046, 0x34, 0x12, 0x56);
        TEST("AND", "DWORD [DI+52], 86",       DATA32, 0x83, 0145, 0x34, 0x56);
        TEST("AND", "DWORD [BP+1234H], 86",    DATA32, 0x83, 0246, 0x34, 0x12, 0x56);
        TEST("AND", "DWORD [BX+SI], 86",       DATA32, 0x83, 0040, 0x56);
        TEST("AND", "DWORD [BX+DI-52], 86",    DATA32, 0x83, 0141, 0xCC, 0x56);
        TEST("AND", "DWORD [BP+SI+89ABH], 86", DATA32, 0x83, 0242, 0xAB, 0x89, 0x56);
    }

    // AND AL,imm8
    TEST("AND", "AL, 86",   0x24, 0x56);
    // AND AX,imm16
    TEST("AND", "AX, 5678H", 0x25, 0x78, 0x56);
    if (is80386()) {
        // AND EAX,imm32
        TEST("AND", "EAX, 12345678H", DATA32, 0x25, 0x78, 0x56, 0x34, 0x12);
    }

    // TEST r/m8,r8
    TEST("TEST", "CL, AL",            0x84, 0301);
    TEST("TEST", "BL, DL",            0x84, 0323);
    TEST("TEST", "CH, AH",            0x84, 0345);
    TEST("TEST", "[SI], DH",          0x84, 0064);
    TEST("TEST", "[1234H], BH",       0x84, 0076, 0x34, 0x12);
    TEST("TEST", "[DI-52], AL",       0x84, 0105, 0xCC);
    TEST("TEST", "[BP+1234H], CL",    0x84, 0216, 0x34, 0x12);
    TEST("TEST", "[BX+SI], DL",       0x84, 0020);
    TEST("TEST", "[BX+DI-52], BL",    0x84, 0131, 0xCC);
    TEST("TEST", "[BP+SI+1234H], AH", 0x84, 0242, 0x34, 0x12);
    // TEST r/m16,r16
    TEST("TEST", "CX, AX",            0x85, 0301);
    TEST("TEST", "BX, DX",            0x85, 0323);
    TEST("TEST", "BP, SP",            0x85, 0345);
    TEST("TEST", "[SI], SI",          0x85, 0064);
    TEST("TEST", "[1234H], DI",       0x85, 0076, 0x34, 0x12);
    TEST("TEST", "[DI+52], AX",       0x85, 0105, 0x34);
    TEST("TEST", "[BP+1234H], CX",    0x85, 0216, 0x34, 0x12);
    TEST("TEST", "[BX+SI], DX",       0x85, 0020);
    TEST("TEST", "[BX+DI+52], BX",    0x85, 0131, 0x34);
    TEST("TEST", "[BP+SI+1234H], SP", 0x85, 0242, 0x34, 0x12);
    if (is80386()) {
        // TEST r/m32,r32
        TEST("TEST", "ECX, EAX",           DATA32, 0x85, 0301);
        TEST("TEST", "EBX, EDX",           DATA32, 0x85, 0323);
        TEST("TEST", "EBP, ESP",           DATA32, 0x85, 0345);
        TEST("TEST", "[SI], ESI",          DATA32, 0x85, 0064);
        TEST("TEST", "[1234H], EDI",       DATA32, 0x85, 0076, 0x34, 0x12);
        TEST("TEST", "[DI+52], EAX",       DATA32, 0x85, 0105, 0x34);
        TEST("TEST", "[BP+1234H], ECX",    DATA32, 0x85, 0216, 0x34, 0x12);
        TEST("TEST", "[BX+SI], EDX",       DATA32, 0x85, 0020);
        TEST("TEST", "[BX+DI+52], EBX",    DATA32, 0x85, 0131, 0x34);
        TEST("TEST", "[BP+SI+1234H], ESP", DATA32, 0x85, 0242, 0x34, 0x12);
    }
    // TEST r/m8,imm8
    TEST("TEST", "AL, 86",                 0xF6, 0300, 0x56);
    TEST("TEST", "CL, 86",                 0xF6, 0301, 0x56);
    TEST("TEST", "BYTE [SI], 86",          0xF6, 0004, 0x56);
    TEST("TEST", "BYTE [89ABH], 86",       0xF6, 0006, 0xAB, 0x89, 0x56);
    TEST("TEST", "BYTE [DI-52], 86",       0xF6, 0105, 0xCC, 0x56);
    TEST("TEST", "BYTE [BP+89ABH], 86",    0xF6, 0206, 0xAB, 0x89, 0x56);
    TEST("TEST", "BYTE [BX+SI], 86",       0xF6, 0000, 0x56);
    TEST("TEST", "BYTE [BX+DI+52], 86",    0xF6, 0101, 0x34, 0x56);
    TEST("TEST", "BYTE [BP+SI+1234H], 86", 0xF6, 0202, 0x34, 0x12, 0x56);
    // TEST r/m16,imm16
    TEST("TEST", "AX, 5678H",                 0xF7, 0300, 0x78, 0x56);
    TEST("TEST", "CX, 5678H",                 0xF7, 0301, 0x78, 0x56);
    TEST("TEST", "WORD [SI], 5678H",          0xF7, 0004, 0x78, 0x56);
    TEST("TEST", "WORD [1234H], 5678H",       0xF7, 0006, 0x34, 0x12, 0x78, 0x56);
    TEST("TEST", "WORD [DI+52], 5678H",       0xF7, 0105, 0x34, 0x78, 0x56);
    TEST("TEST", "WORD [BP+1234H], 5678H",    0xF7, 0206, 0x34, 0x12, 0x78, 0x56);
    TEST("TEST", "WORD [BX+SI], 5678H",       0xF7, 0000, 0x78, 0x56);
    TEST("TEST", "WORD [BX+DI-52], 5678H",    0xF7, 0101, 0xCC, 0x78, 0x56);
    TEST("TEST", "WORD [BP+SI+89ABH], 5678H", 0xF7, 0202, 0xAB, 0x89, 0x78, 0x56);
    if (is80386()) {
        // TEST r/m32,imm32
        TEST("TEST", "EAX, 12345678H",                 DATA32, 0xF7, 0300, 0x78, 0x56, 0x34, 0x12);
        TEST("TEST", "ECX, 12345678H",                 DATA32, 0xF7, 0301, 0x78, 0x56, 0x34, 0x12);
        TEST("TEST", "DWORD [SI], 12345678H",          DATA32, 0xF7, 0004, 0x78, 0x56, 0x34, 0x12);
        TEST("TEST", "DWORD [1234H], 12345678H",       DATA32, 0xF7, 0006, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12);
        TEST("TEST", "DWORD [DI+52], 12345678H",       DATA32, 0xF7, 0105, 0x34, 0x78, 0x56, 0x34, 0x12);
        TEST("TEST", "DWORD [BP+1234H], 12345678H",    DATA32, 0xF7, 0206, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12);
        TEST("TEST", "DWORD [BX+SI], 12345678H",       DATA32, 0xF7, 0000, 0x78, 0x56, 0x34, 0x12);
        TEST("TEST", "DWORD [BX+DI-52], 12345678H",    DATA32, 0xF7, 0101, 0xCC, 0x78, 0x56, 0x34, 0x12);
        TEST("TEST", "DWORD [BP+SI+89ABH], 12345678H", DATA32, 0xF7, 0202, 0xAB, 0x89, 0x78, 0x56, 0x34, 0x12);
    }

    // TEST AL,imm8
    TEST("TEST", "AL, 86",   0xA8, 0x56);
    // TEST AX,imm16
    TEST("TEST", "AX, 5678H", 0xA9, 0x78, 0x56);
    if (is80386()) {
        // TEST EAX,imm32
        TEST("TEST", "EAX, 12345678H", DATA32, 0xA9, 0x78, 0x56, 0x34, 0x12);
    }

    // OR r/m8,r8
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
    // OR r8,r/m8
    TEST("OR", "DH, [SI]",          0x0A, 0064);
    TEST("OR", "BH, [1234H]",       0x0A, 0076, 0x34, 0x12);
    TEST("OR", "AL, [DI-52]",       0x0A, 0105, 0xCC);
    TEST("OR", "CL, [BP+1234H]",    0x0A, 0216, 0x34, 0x12);
    TEST("OR", "DL, [BX+SI]",       0x0A, 0020);
    TEST("OR", "BL, [BX+DI-52]",    0x0A, 0131, 0xCC);
    TEST("OR", "AH, [BP+SI+1234H]", 0x0A, 0242, 0x34, 0x12);
    // OR r/m16,r16
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
    // OR r16,r/m16
    TEST("OR", "SI, [SI]",          0x0B, 0064);
    TEST("OR", "DI, [1234H]",       0x0B, 0076, 0x34, 0x12);
    TEST("OR", "AX, [DI+52]",       0x0B, 0105, 0x34);
    TEST("OR", "CX, [BP+1234H]",    0x0B, 0216, 0x34, 0x12);
    TEST("OR", "DX, [BX+SI]",       0x0B, 0020);
    TEST("OR", "BX, [BX+DI+52]",    0x0B, 0131, 0x34);
    TEST("OR", "SP, [BP+SI+1234H]", 0x0B, 0242, 0x34, 0x12);
    if (is80386()) {
        // OR r/m32,r32
        TEST("OR", "EAX, ECX",           DATA32, 0x09, 0310);
        TEST("OR", "EDX, EBX",           DATA32, 0x09, 0332);
        TEST("OR", "ESP, EBP",           DATA32, 0x09, 0354);
        TEST("OR", "[SI], ESI",          DATA32, 0x09, 0064);
        TEST("OR", "[1234H], EDI",       DATA32, 0x09, 0076, 0x34, 0x12);
        TEST("OR", "[DI-52], EAX",       DATA32, 0x09, 0105, 0xCC);
        TEST("OR", "[BP+1234H], ECX",    DATA32, 0x09, 0216, 0x34, 0x12);
        TEST("OR", "[BX+SI], EDX",       DATA32, 0x09, 0020);
        TEST("OR", "[BX+DI+52], EBX",    DATA32, 0x09, 0131, 0x34);
        TEST("OR", "[BP+SI+1234H], ESP", DATA32, 0x09, 0242, 0x34, 0x12);
        // OR r32,r/m32
        TEST("OR", "ESI, [SI]",          DATA32, 0x0B, 0064);
        TEST("OR", "EDI, [1234H]",       DATA32, 0x0B, 0076, 0x34, 0x12);
        TEST("OR", "EAX, [DI+52]",       DATA32, 0x0B, 0105, 0x34);
        TEST("OR", "ECX, [BP+1234H]",    DATA32, 0x0B, 0216, 0x34, 0x12);
        TEST("OR", "EDX, [BX+SI]",       DATA32, 0x0B, 0020);
        TEST("OR", "EBX, [BX+DI+52]",    DATA32, 0x0B, 0131, 0x34);
        TEST("OR", "ESP, [BP+SI+1234H]", DATA32, 0x0B, 0242, 0x34, 0x12);
    }

    // OR r/m8,imm8
    TEST("OR", "AL, 86",                 0x80, 0310, 0x56);
    TEST("OR", "CL, 86",                 0x80, 0311, 0x56);
    TEST("OR", "BYTE [SI], 86",          0x80, 0014, 0x56);
    TEST("OR", "BYTE [89ABH], 86",       0x80, 0016, 0xAB, 0x89, 0x56);
    TEST("OR", "BYTE [DI-52], 86",       0x80, 0115, 0xCC, 0x56);
    TEST("OR", "BYTE [BP+89ABH], 86",    0x80, 0216, 0xAB, 0x89, 0x56);
    TEST("OR", "BYTE [BX+SI], 86",       0x80, 0010, 0x56);
    TEST("OR", "BYTE [BX+DI+52], 86",    0x80, 0111, 0x34, 0x56);
    TEST("OR", "BYTE [BP+SI+1234H], 86", 0x80, 0212, 0x34, 0x12, 0x56);
    // OR r/m16,imm16
    TEST("OR", "AX, 5678H",                 0x81, 0310, 0x78, 0x56);
    TEST("OR", "CX, 5678H",                 0x81, 0311, 0x78, 0x56);
    TEST("OR", "WORD [SI], 5678H",          0x81, 0014, 0x78, 0x56);
    TEST("OR", "WORD [1234H], 5678H",       0x81, 0016, 0x34, 0x12, 0x78, 0x56);
    TEST("OR", "WORD [DI+52], 5678H",       0x81, 0115, 0x34, 0x78, 0x56);
    TEST("OR", "WORD [BP+1234H], 5678H",    0x81, 0216, 0x34, 0x12, 0x78, 0x56);
    TEST("OR", "WORD [BX+SI], 5678H",       0x81, 0010, 0x78, 0x56);
    TEST("OR", "WORD [BX+DI-52], 5678H",    0x81, 0111, 0xCC, 0x78, 0x56);
    TEST("OR", "WORD [BP+SI+89ABH], 5678H", 0x81, 0212, 0xAB, 0x89, 0x78, 0x56);
    // OR r/m16,imm8
    TEST("OR", "AX, 86",                 0x83, 0310, 0x56);
    TEST("OR", "AX, -16",                0x83, 0310, 0xF0);
    TEST("OR", "CX, 86",                 0x83, 0311, 0x56);
    TEST("OR", "CX, -16",                0x83, 0311, 0xF0);
    TEST("OR", "WORD [SI], 86",          0x83, 0014, 0x56);
    TEST("OR", "WORD [1234H], 86",       0x83, 0016, 0x34, 0x12, 0x56);
    TEST("OR", "WORD [DI+52], 86",       0x83, 0115, 0x34, 0x56);
    TEST("OR", "WORD [BP+1234H], 86",    0x83, 0216, 0x34, 0x12, 0x56);
    TEST("OR", "WORD [BX+SI], 86",       0x83, 0010, 0x56);
    TEST("OR", "WORD [BX+DI-52], 86",    0x83, 0111, 0xCC, 0x56);
    TEST("OR", "WORD [BP+SI+89ABH], 86", 0x83, 0212, 0xAB, 0x89, 0x56);
    if (is80386()) {
        // OR r/m32,imm32
        TEST("OR", "EAX, 12345678H",                 DATA32, 0x81, 0310, 0x78, 0x56, 0x34, 0x12);
        TEST("OR", "ECX, 12345678H",                 DATA32, 0x81, 0311, 0x78, 0x56, 0x34, 0x12);
        TEST("OR", "DWORD [SI], 12345678H",          DATA32, 0x81, 0014, 0x78, 0x56, 0x34, 0x12);
        TEST("OR", "DWORD [1234H], 12345678H",       DATA32, 0x81, 0016, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12);
        TEST("OR", "DWORD [DI+52], 12345678H",       DATA32, 0x81, 0115, 0x34, 0x78, 0x56, 0x34, 0x12);
        TEST("OR", "DWORD [BP+1234H], 12345678H",    DATA32, 0x81, 0216, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12);
        TEST("OR", "DWORD [BX+SI], 12345678H",       DATA32, 0x81, 0010, 0x78, 0x56, 0x34, 0x12);
        TEST("OR", "DWORD [BX+DI-52], 12345678H",    DATA32, 0x81, 0111, 0xCC, 0x78, 0x56, 0x34, 0x12);
        TEST("OR", "DWORD [BP+SI+89ABH], 12345678H", DATA32, 0x81, 0212, 0xAB, 0x89, 0x78, 0x56, 0x34, 0x12);
        // OR r/m32,imm8
        TEST("OR", "EAX, 86",                 DATA32, 0x83, 0310, 0x56);
        TEST("OR", "EAX, -16",                DATA32, 0x83, 0310, 0xF0);
        TEST("OR", "ECX, 86",                 DATA32, 0x83, 0311, 0x56);
        TEST("OR", "ECX, -16",                DATA32, 0x83, 0311, 0xF0);
        TEST("OR", "DWORD [SI], 86",          DATA32, 0x83, 0014, 0x56);
        TEST("OR", "DWORD [1234H], 86",       DATA32, 0x83, 0016, 0x34, 0x12, 0x56);
        TEST("OR", "DWORD [DI+52], 86",       DATA32, 0x83, 0115, 0x34, 0x56);
        TEST("OR", "DWORD [BP+1234H], 86",    DATA32, 0x83, 0216, 0x34, 0x12, 0x56);
        TEST("OR", "DWORD [BX+SI], 86",       DATA32, 0x83, 0010, 0x56);
        TEST("OR", "DWORD [BX+DI-52], 86",    DATA32, 0x83, 0111, 0xCC, 0x56);
        TEST("OR", "DWORD [BP+SI+89ABH], 86", DATA32, 0x83, 0212, 0xAB, 0x89, 0x56);
    }

    // OR AL,imm8
    TEST("OR", "AL, 86",   0x0C, 0x56);
    // OR AX,imm16
    TEST("OR", "AX, 5678H", 0x0D, 0x78, 0x56);
    if (is80386()) {
        // OR EAX,imm32
        TEST("OR", "EAX, 12345678H", DATA32, 0x0D, 0x78, 0x56, 0x34, 0x12);
    }

    // XOR r/m8,r8
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
    // XOR r8,r/m8
    TEST("XOR", "DH, [SI]",          0x32, 0064);
    TEST("XOR", "BH, [1234H]",       0x32, 0076, 0x34, 0x12);
    TEST("XOR", "AL, [DI-52]",       0x32, 0105, 0xCC);
    TEST("XOR", "CL, [BP+1234H]",    0x32, 0216, 0x34, 0x12);
    TEST("XOR", "DL, [BX+SI]",       0x32, 0020);
    TEST("XOR", "BL, [BX+DI-52]",    0x32, 0131, 0xCC);
    TEST("XOR", "AH, [BP+SI+1234H]", 0x32, 0242, 0x34, 0x12);
    // XOR r/m16,r16
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
    // XOR r16,r/m16
    TEST("XOR", "SI, [SI]",          0x33, 0064);
    TEST("XOR", "DI, [1234H]",       0x33, 0076, 0x34, 0x12);
    TEST("XOR", "AX, [DI+52]",       0x33, 0105, 0x34);
    TEST("XOR", "CX, [BP+1234H]",    0x33, 0216, 0x34, 0x12);
    TEST("XOR", "DX, [BX+SI]",       0x33, 0020);
    TEST("XOR", "BX, [BX+DI+52]",    0x33, 0131, 0x34);
    TEST("XOR", "SP, [BP+SI+1234H]", 0x33, 0242, 0x34, 0x12);
    if (is80386()) {
        // XOR r/m32,r32
        TEST("XOR", "EAX, ECX",           DATA32, 0x31, 0310);
        TEST("XOR", "EDX, EBX",           DATA32, 0x31, 0332);
        TEST("XOR", "ESP, EBP",           DATA32, 0x31, 0354);
        TEST("XOR", "[SI], ESI",          DATA32, 0x31, 0064);
        TEST("XOR", "[1234H], EDI",       DATA32, 0x31, 0076, 0x34, 0x12);
        TEST("XOR", "[DI-52], EAX",       DATA32, 0x31, 0105, 0xCC);
        TEST("XOR", "[BP+1234H], ECX",    DATA32, 0x31, 0216, 0x34, 0x12);
        TEST("XOR", "[BX+SI], EDX",       DATA32, 0x31, 0020);
        TEST("XOR", "[BX+DI+52], EBX",    DATA32, 0x31, 0131, 0x34);
        TEST("XOR", "[BP+SI+1234H], ESP", DATA32, 0x31, 0242, 0x34, 0x12);
        // XOR r32,r/m32
        TEST("XOR", "ESI, [SI]",          DATA32, 0x33, 0064);
        TEST("XOR", "EDI, [1234H]",       DATA32, 0x33, 0076, 0x34, 0x12);
        TEST("XOR", "EAX, [DI+52]",       DATA32, 0x33, 0105, 0x34);
        TEST("XOR", "ECX, [BP+1234H]",    DATA32, 0x33, 0216, 0x34, 0x12);
        TEST("XOR", "EDX, [BX+SI]",       DATA32, 0x33, 0020);
        TEST("XOR", "EBX, [BX+DI+52]",    DATA32, 0x33, 0131, 0x34);
        TEST("XOR", "ESP, [BP+SI+1234H]", DATA32, 0x33, 0242, 0x34, 0x12);
    }
    // XOR r/m8,imm8
    TEST("XOR", "AL, 86",                 0x80, 0360, 0x56);
    TEST("XOR", "CL, 86",                 0x80, 0361, 0x56);
    TEST("XOR", "BYTE [SI], 86",          0x80, 0064, 0x56);
    TEST("XOR", "BYTE [89ABH], 86",       0x80, 0066, 0xAB, 0x89, 0x56);
    TEST("XOR", "BYTE [DI-52], 86",       0x80, 0165, 0xCC, 0x56);
    TEST("XOR", "BYTE [BP+89ABH], 86",    0x80, 0266, 0xAB, 0x89, 0x56);
    TEST("XOR", "BYTE [BX+SI], 86",       0x80, 0060, 0x56);
    TEST("XOR", "BYTE [BX+DI+52], 86",    0x80, 0161, 0x34, 0x56);
    TEST("XOR", "BYTE [BP+SI+1234H], 86", 0x80, 0262, 0x34, 0x12, 0x56);
    // XOR r/m16,imm16
    TEST("XOR", "AX, 5678H",                 0x81, 0360, 0x78, 0x56);
    TEST("XOR", "CX, 5678H",                 0x81, 0361, 0x78, 0x56);
    TEST("XOR", "WORD [SI], 5678H",          0x81, 0064, 0x78, 0x56);
    TEST("XOR", "WORD [1234H], 5678H",       0x81, 0066, 0x34, 0x12, 0x78, 0x56);
    TEST("XOR", "WORD [DI+52], 5678H",       0x81, 0165, 0x34, 0x78, 0x56);
    TEST("XOR", "WORD [BP+1234H], 5678H",    0x81, 0266, 0x34, 0x12, 0x78, 0x56);
    TEST("XOR", "WORD [BX+SI], 5678H",       0x81, 0060, 0x78, 0x56);
    TEST("XOR", "WORD [BX+DI-52], 5678H",    0x81, 0161, 0xCC, 0x78, 0x56);
    TEST("XOR", "WORD [BP+SI+89ABH], 5678H", 0x81, 0262, 0xAB, 0x89, 0x78, 0x56);
    // XOR r/m16,imm8
    TEST("XOR", "AX, 86",                 0x83, 0360, 0x56);
    TEST("XOR", "AX, -16",                0x83, 0360, 0xF0);
    TEST("XOR", "CX, 86",                 0x83, 0361, 0x56);
    TEST("XOR", "CX, -16",                0x83, 0361, 0xF0);
    TEST("XOR", "WORD [SI], 86",          0x83, 0064, 0x56);
    TEST("XOR", "WORD [1234H], 86",       0x83, 0066, 0x34, 0x12, 0x56);
    TEST("XOR", "WORD [DI+52], 86",       0x83, 0165, 0x34, 0x56);
    TEST("XOR", "WORD [BP+1234H], 86",    0x83, 0266, 0x34, 0x12, 0x56);
    TEST("XOR", "WORD [BX+SI], 86",       0x83, 0060, 0x56);
    TEST("XOR", "WORD [BX+DI-52], 86",    0x83, 0161, 0xCC, 0x56);
    TEST("XOR", "WORD [BP+SI+89ABH], 86",   0x83, 0262, 0xAB, 0x89, 0x56);
    if (is80386()) {
        // XOR r/m32,imm32
        TEST("XOR", "EAX, 12345678H",                 DATA32, 0x81, 0360, 0x78, 0x56, 0x34, 0x12);
        TEST("XOR", "ECX, 12345678H",                 DATA32, 0x81, 0361, 0x78, 0x56, 0x34, 0x12);
        TEST("XOR", "DWORD [SI], 12345678H",          DATA32, 0x81, 0064, 0x78, 0x56, 0x34, 0x12);
        TEST("XOR", "DWORD [1234H], 12345678H",       DATA32, 0x81, 0066, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12);
        TEST("XOR", "DWORD [DI+52], 12345678H",       DATA32, 0x81, 0165, 0x34, 0x78, 0x56, 0x34, 0x12);
        TEST("XOR", "DWORD [BP+1234H], 12345678H",    DATA32, 0x81, 0266, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12);
        TEST("XOR", "DWORD [BX+SI], 12345678H",       DATA32, 0x81, 0060, 0x78, 0x56, 0x34, 0x12);
        TEST("XOR", "DWORD [BX+DI-52], 12345678H",    DATA32, 0x81, 0161, 0xCC, 0x78, 0x56, 0x34, 0x12);
        TEST("XOR", "DWORD [BP+SI+89ABH], 12345678H", DATA32, 0x81, 0262, 0xAB, 0x89, 0x78, 0x56, 0x34, 0x12);
        // XOR r/m32,imm8
        TEST("XOR", "EAX, 86",                 DATA32, 0x83, 0360, 0x56);
        TEST("XOR", "EAX, -16",                DATA32, 0x83, 0360, 0xF0);
        TEST("XOR", "ECX, 86",                 DATA32, 0x83, 0361, 0x56);
        TEST("XOR", "ECX, -16",                DATA32, 0x83, 0361, 0xF0);
        TEST("XOR", "DWORD [SI], 86",          DATA32, 0x83, 0064, 0x56);
        TEST("XOR", "DWORD [1234H], 86",       DATA32, 0x83, 0066, 0x34, 0x12, 0x56);
        TEST("XOR", "DWORD [DI+52], 86",       DATA32, 0x83, 0165, 0x34, 0x56);
        TEST("XOR", "DWORD [BP+1234H], 86",    DATA32, 0x83, 0266, 0x34, 0x12, 0x56);
        TEST("XOR", "DWORD [BX+SI], 86",       DATA32, 0x83, 0060, 0x56);
        TEST("XOR", "DWORD [BX+DI-52], 86",    DATA32, 0x83, 0161, 0xCC, 0x56);
        TEST("XOR", "DWORD [BP+SI+89ABH], 86", DATA32, 0x83, 0262, 0xAB, 0x89, 0x56);
    }
    // XOR AL,imm8
    TEST("XOR", "AL, 86",   0x34, 0x56);
    // XOR AX,imm16
    TEST("XOR", "AX, 5678H", 0x35, 0x78, 0x56);
    if (is80386()) {
        // XOR EAX,imm32
        TEST("XOR", "EAX, 12345678H", DATA32, 0x35, 0x78, 0x56, 0x34, 0x12);
    }

    if (is80386()) {
        // SETcc r/m8
        TEST("SETO",  "CH",            0x0F, 0x90, 0305);
        TEST("SETNO", "[SI]",          0x0F, 0x91, 0004);
        TEST("SETB",  "[1234H]",       0x0F, 0x92, 0006, 0x34, 0x12);
        TEST("SETAE", "[DI+52]",       0x0F, 0x93, 0105, 0x34);
        TEST("SETE",  "[BP+1234H]",    0x0F, 0x94, 0206, 0x34, 0x12);
        TEST("SETNE", "[BX+SI]",       0x0F, 0x95, 0000);
        TEST("SETBE", "[BX+DI+52]",    0x0F, 0x96, 0101, 0x34);
        TEST("SETA",  "[BP+SI+1234H]", 0x0F, 0x97, 0202, 0x34, 0x12);
        TEST("SETS",  "CH",            0x0F, 0x98, 0305);
        TEST("SETNS", "[SI]",          0x0F, 0x99, 0004);
        TEST("SETPE", "[1234H]",       0x0F, 0x9A, 0006, 0x34, 0x12);
        TEST("SETPO", "[DI-52]",       0x0F, 0x9B, 0105, 0xCC);
        TEST("SETL",  "[BP+1234H]",    0x0F, 0x9C, 0206, 0x34, 0x12);
        TEST("SETGE", "[BX+SI]",       0x0F, 0x9D, 0000);
        TEST("SETLE", "[BX+DI+52]",    0x0F, 0x9E, 0101, 0x34);
        TEST("SETG",  "[BP+SI+1234H]", 0x0F, 0x9F, 0202, 0x34, 0x12);
        use32();
        TEST("SETO",  "CH",            0x0F, 0x90, 0305);
        TEST("SETNO", "[ESP+ESI]",     0x0F, 0x91, 0004, 0x34);
        TEST("SETB",  "[ESI]",         0x0F, 0x92, 0006);
        TEST("SETAE", "[EBP+52]",      0x0F, 0x93, 0105, 0x34);
        TEST("SETE",  "[ESI+12345678H]", 0x0F, 0x94, 0206, 0x78, 0x56, 0x34, 0x12);
        TEST("SETNE", "[EAX]",         0x0F, 0x95, 0000);
        TEST("SETBE", "[ECX+52]",      0x0F, 0x96, 0101, 0x34);
        TEST("SETA",  "[EDX+12345678H]", 0x0F, 0x97, 0202, 0x78, 0x56, 0x34, 0x12);
        TEST("SETS",  "CH",            0x0F, 0x98, 0305);
        TEST("SETNS", "[EAX+EDI*2]",   0x0F, 0x99, 0004, 0x78);
        TEST("SETPE", "[ESI]",         0x0F, 0x9A, 0006);
        TEST("SETPO", "[EBP-52]",      0x0F, 0x9B, 0105, 0xCC);
        TEST("SETL",  "[ESI+12345678H]", 0x0F, 0x9C, 0206, 0x78, 0x56, 0x34, 0x12);
        TEST("SETGE", "[EAX]",         0x0F, 0x9D, 0000);
        TEST("SETLE", "[ECX+52]",      0x0F, 0x9E, 0101, 0x34);
        TEST("SETG",  "[EDX+12345678H]", 0x0F, 0x9F, 0202, 0x78, 0x56, 0x34, 0x12);
        use16();
    }

    if (v30()) {
        TEST("CLR1", "CH, 1",                  0x0F, 0x1A, 0305, 0x01);
        TEST("CLR1", "BYTE [SI], 2",           0x0F, 0x1A, 0004, 0x02);
        TEST("CLR1", "BYTE [1234H], 3",        0x0F, 0x1A, 0006, 0x34, 0x12, 0x03);
        TEST("CLR1", "BYTE [DI-52], 4",        0x0F, 0x1A, 0105, 0xCC, 0x04);
        TEST("CLR1", "BYTE [BP+1234H], 5",     0x0F, 0x1A, 0206, 0x34, 0x12, 0x05);
        TEST("CLR1", "BYTE [BX+SI], 6",        0x0F, 0x1A, 0000, 0x06);
        TEST("CLR1", "BYTE [BX+DI+52], 7",     0x0F, 0x1A, 0101, 0x34, 0x07);
        TEST("CLR1", "BYTE [BP+SI+1234H], 0",  0x0F, 0x1A, 0202, 0x34, 0x12, 0x00);
        TEST("CLR1", "BP, 1",                  0x0F, 0x1B, 0305, 0x01);
        TEST("CLR1", "WORD [SI], 3",           0x0F, 0x1B, 0004, 0x03);
        TEST("CLR1", "WORD [1234H], 5",        0x0F, 0x1B, 0006, 0x34, 0x12, 0x05);
        TEST("CLR1", "WORD [DI-52], 7",        0x0F, 0x1B, 0105, 0xCC, 0x07);
        TEST("CLR1", "WORD [BP+1234H], 9",     0x0F, 0x1B, 0206, 0x34, 0x12, 0x09);
        TEST("CLR1", "WORD [BX+SI], 11",       0x0F, 0x1B, 0000, 0x0B);
        TEST("CLR1", "WORD [BX+DI+52], 13",    0x0F, 0x1B, 0101, 0x34, 0x0D);
        TEST("CLR1", "WORD [BP+SI+1234H], 15", 0x0F, 0x1B, 0202, 0x34, 0x12, 0x0F);
        TEST("CLR1", "CH, CL",                 0x0F, 0x12, 0305);
        TEST("CLR1", "BYTE [SI], CL",          0x0F, 0x12, 0004);
        TEST("CLR1", "BYTE [1234H], CL",       0x0F, 0x12, 0006, 0x34, 0x12);
        TEST("CLR1", "BYTE [DI-52], CL",       0x0F, 0x12, 0105, 0xCC);
        TEST("CLR1", "BYTE [BP+1234H], CL",    0x0F, 0x12, 0206, 0x34, 0x12);
        TEST("CLR1", "BYTE [BX+SI], CL",       0x0F, 0x12, 0000);
        TEST("CLR1", "BYTE [BX+DI+52], CL",    0x0F, 0x12, 0101, 0x34);
        TEST("CLR1", "BYTE [BP+SI+1234H], CL", 0x0F, 0x12, 0202, 0x34, 0x12);
        TEST("CLR1", "BP, CL",                 0x0F, 0x13, 0305);
        TEST("CLR1", "WORD [SI], CL",          0x0F, 0x13, 0004);
        TEST("CLR1", "WORD [1234H], CL",       0x0F, 0x13, 0006, 0x34, 0x12);
        TEST("CLR1", "WORD [DI-52], CL",       0x0F, 0x13, 0105, 0xCC);
        TEST("CLR1", "WORD [BP+1234H], CL",    0x0F, 0x13, 0206, 0x34, 0x12);
        TEST("CLR1", "WORD [BX+SI], CL",       0x0F, 0x13, 0000);
        TEST("CLR1", "WORD [BX+DI+52], CL",    0x0F, 0x13, 0101, 0x34);
        TEST("CLR1", "WORD [BP+SI+1234H], CL", 0x0F, 0x13, 0202, 0x34, 0x12);

        TEST("SET1", "CH, 1",                  0x0F, 0x1C, 0305, 0x01);
        TEST("SET1", "BYTE [SI], 2",           0x0F, 0x1C, 0004, 0x02);
        TEST("SET1", "BYTE [1234H], 3",        0x0F, 0x1C, 0006, 0x34, 0x12, 0x03);
        TEST("SET1", "BYTE [DI-52], 4",        0x0F, 0x1C, 0105, 0xCC, 0x04);
        TEST("SET1", "BYTE [BP+1234H], 5",     0x0F, 0x1C, 0206, 0x34, 0x12, 0x05);
        TEST("SET1", "BYTE [BX+SI], 6",        0x0F, 0x1C, 0000, 0x06);
        TEST("SET1", "BYTE [BX+DI+52], 7",     0x0F, 0x1C, 0101, 0x34, 0x07);
        TEST("SET1", "BYTE [BP+SI+1234H], 0",  0x0F, 0x1C, 0202, 0x34, 0x12, 0x00);
        TEST("SET1", "BP, 1",                  0x0F, 0x1D, 0305, 0x01);
        TEST("SET1", "WORD [SI], 3",           0x0F, 0x1D, 0004, 0x03);
        TEST("SET1", "WORD [1234H], 5",        0x0F, 0x1D, 0006, 0x34, 0x12, 0x05);
        TEST("SET1", "WORD [DI-52], 7",        0x0F, 0x1D, 0105, 0xCC, 0x07);
        TEST("SET1", "WORD [BP+1234H], 9",     0x0F, 0x1D, 0206, 0x34, 0x12, 0x09);
        TEST("SET1", "WORD [BX+SI], 11",       0x0F, 0x1D, 0000, 0x0B);
        TEST("SET1", "WORD [BX+DI+52], 13",    0x0F, 0x1D, 0101, 0x34, 0x0D);
        TEST("SET1", "WORD [BP+SI+1234H], 15", 0x0F, 0x1D, 0202, 0x34, 0x12, 0x0F);
        TEST("SET1", "CH, CL",                 0x0F, 0x14, 0305);
        TEST("SET1", "BYTE [SI], CL",          0x0F, 0x14, 0004);
        TEST("SET1", "BYTE [1234H], CL",       0x0F, 0x14, 0006, 0x34, 0x12);
        TEST("SET1", "BYTE [DI-52], CL",       0x0F, 0x14, 0105, 0xCC);
        TEST("SET1", "BYTE [BP+1234H], CL",    0x0F, 0x14, 0206, 0x34, 0x12);
        TEST("SET1", "BYTE [BX+SI], CL",       0x0F, 0x14, 0000);
        TEST("SET1", "BYTE [BX+DI+52], CL",    0x0F, 0x14, 0101, 0x34);
        TEST("SET1", "BYTE [BP+SI+1234H], CL", 0x0F, 0x14, 0202, 0x34, 0x12);
        TEST("SET1", "BP, CL",                 0x0F, 0x15, 0305);
        TEST("SET1", "WORD [SI], CL",          0x0F, 0x15, 0004);
        TEST("SET1", "WORD [1234H], CL",       0x0F, 0x15, 0006, 0x34, 0x12);
        TEST("SET1", "WORD [DI-52], CL",       0x0F, 0x15, 0105, 0xCC);
        TEST("SET1", "WORD [BP+1234H], CL",    0x0F, 0x15, 0206, 0x34, 0x12);
        TEST("SET1", "WORD [BX+SI], CL",       0x0F, 0x15, 0000);
        TEST("SET1", "WORD [BX+DI+52], CL",    0x0F, 0x15, 0101, 0x34);
        TEST("SET1", "WORD [BP+SI+1234H], CL", 0x0F, 0x15, 0202, 0x34, 0x12);

        TEST("NOT1", "CH, 1",                  0x0F, 0x1E, 0305, 0x01);
        TEST("NOT1", "BYTE [SI], 2",           0x0F, 0x1E, 0004, 0x02);
        TEST("NOT1", "BYTE [1234H], 3",        0x0F, 0x1E, 0006, 0x34, 0x12, 0x03);
        TEST("NOT1", "BYTE [DI-52], 4",        0x0F, 0x1E, 0105, 0xCC, 0x04);
        TEST("NOT1", "BYTE [BP+1234H], 5",     0x0F, 0x1E, 0206, 0x34, 0x12, 0x05);
        TEST("NOT1", "BYTE [BX+SI], 6",        0x0F, 0x1E, 0000, 0x06);
        TEST("NOT1", "BYTE [BX+DI+52], 7",     0x0F, 0x1E, 0101, 0x34, 0x07);
        TEST("NOT1", "BYTE [BP+SI+1234H], 0",  0x0F, 0x1E, 0202, 0x34, 0x12, 0x00);
        TEST("NOT1", "BP, 1",                  0x0F, 0x1F, 0305, 0x01);
        TEST("NOT1", "WORD [SI], 3",           0x0F, 0x1F, 0004, 0x03);
        TEST("NOT1", "WORD [1234H], 5",        0x0F, 0x1F, 0006, 0x34, 0x12, 0x05);
        TEST("NOT1", "WORD [DI-52], 7",        0x0F, 0x1F, 0105, 0xCC, 0x07);
        TEST("NOT1", "WORD [BP+1234H], 9",     0x0F, 0x1F, 0206, 0x34, 0x12, 0x09);
        TEST("NOT1", "WORD [BX+SI], 11",       0x0F, 0x1F, 0000, 0x0B);
        TEST("NOT1", "WORD [BX+DI+52], 13",    0x0F, 0x1F, 0101, 0x34, 0x0D);
        TEST("NOT1", "WORD [BP+SI+1234H], 15", 0x0F, 0x1F, 0202, 0x34, 0x12, 0x0F);
        TEST("NOT1", "CH, CL",                 0x0F, 0x16, 0305);
        TEST("NOT1", "BYTE [SI], CL",          0x0F, 0x16, 0004);
        TEST("NOT1", "BYTE [1234H], CL",       0x0F, 0x16, 0006, 0x34, 0x12);
        TEST("NOT1", "BYTE [DI-52], CL",       0x0F, 0x16, 0105, 0xCC);
        TEST("NOT1", "BYTE [BP+1234H], CL",    0x0F, 0x16, 0206, 0x34, 0x12);
        TEST("NOT1", "BYTE [BX+SI], CL",       0x0F, 0x16, 0000);
        TEST("NOT1", "BYTE [BX+DI+52], CL",    0x0F, 0x16, 0101, 0x34);
        TEST("NOT1", "BYTE [BP+SI+1234H], CL", 0x0F, 0x16, 0202, 0x34, 0x12);
        TEST("NOT1", "BP, CL",                 0x0F, 0x17, 0305);
        TEST("NOT1", "WORD [SI], CL",          0x0F, 0x17, 0004);
        TEST("NOT1", "WORD [1234H], CL",       0x0F, 0x17, 0006, 0x34, 0x12);
        TEST("NOT1", "WORD [DI-52], CL",       0x0F, 0x17, 0105, 0xCC);
        TEST("NOT1", "WORD [BP+1234H], CL",    0x0F, 0x17, 0206, 0x34, 0x12);
        TEST("NOT1", "WORD [BX+SI], CL",       0x0F, 0x17, 0000);
        TEST("NOT1", "WORD [BX+DI+52], CL",    0x0F, 0x17, 0101, 0x34);
        TEST("NOT1", "WORD [BP+SI+1234H], CL", 0x0F, 0x17, 0202, 0x34, 0x12);
    }
}

void test_repeat() {
    repeatInsn(true);
    TEST("REPNE", "", REPNE);
    TEST("REP",   "", REP);
    if (v30()) {
        TEST("REPNC", "", REPNC);
        TEST("REPC",  "", REPC);
    }

    repeatInsn(false);
    NMEM("", "", "", REPNE);
    NMEM("", "", "", REP);
    EPRE("", "", REP, REP);
    EPRE("", "", REP, REPNE);
    EPRE("", "", REPNE, REP);
    EPRE("", "", REPNE, REPNE);
    if (v30()) {
        NMEM("", "", "", REPNC);
        NMEM("", "", "", REPC);
        EPRE("", "", REPNE, REPNC);
        EPRE("", "", REPC,  REP);
    }

    TEST("REPNE MOVSB", "", REPNE, 0xA4);
    TEST("REPNE MOVSW", "", REPNE, 0xA5);
    TEST("REPNE CMPSB", "", REPNE, 0xA6);
    TEST("REPNE CMPSW", "", REPNE, 0xA7);
    TEST("REPNE STOSB", "", REPNE, 0xAA);
    TEST("REPNE STOSW", "", REPNE, 0xAB);
    TEST("REPNE LODSB", "", REPNE, 0xAC);
    TEST("REPNE LODSW", "", REPNE, 0xAD);
    TEST("REPNE SCASB", "", REPNE, 0xAE);
    TEST("REPNE SCASW", "", REPNE, 0xAF);
    ERRT("REPNE NOP", "", ILLEGAL_COMBINATION, "NOP", REPNE, 0x90);
    NMEM("", "", "",        REPNE, 0xFF);
    if (is80186()) {
        TEST("REPNE INSB", "",  REPNE, 0x6C);
        TEST("REPNE INSW", "",  REPNE, 0x6D);
        TEST("REPNE OUTSB", "", REPNE, 0x6E);
        TEST("REPNE OUTSW", "", REPNE, 0x6F);
    }
    if (is80386()) {
        TEST("REPNE INSD",  "", DATA32, 0xF2, 0x6D);
        TEST("REPNE OUTSD", "", DATA32, 0xF2, 0x6F);
    }

    TEST("REP MOVSB", "", REP, 0xA4);
    TEST("REP MOVSW", "", REP, 0xA5);
    TEST("REP CMPSB", "", REP, 0xA6);
    TEST("REP CMPSW", "", REP, 0xA7);
    TEST("REP STOSB", "", REP, 0xAA);
    TEST("REP STOSW", "", REP, 0xAB);
    TEST("REP LODSB", "", REP, 0xAC);
    TEST("REP LODSW", "", REP, 0xAD);
    TEST("REP SCASB", "", REP, 0xAE);
    TEST("REP SCASW", "", REP, 0xAF);
    if (is80186()) {
        TEST("REP INSB", "",  REP, 0x6C);
        TEST("REP INSW", "",  REP, 0x6D);
        TEST("REP OUTSB", "", REP, 0x6E);
        TEST("REP OUTSW", "", REP, 0x6F);
    }
    if (is80386()) {
        TEST("REP INSD",  "", DATA32, 0xF3, 0x6D);
        TEST("REP OUTSD", "", DATA32, 0xF3, 0x6F);
    }
    if (v30()) {
        TEST("REPNC CMPSB", "", REPNC, 0xA6);
        TEST("REP SCASW", "", REP, 0xAF);
        TEST("REP ADD4S", "", REP, 0x0F, 0x20);
        TEST("REP SUB4S", "", REP, 0x0F, 0x22);
        TEST("REP CMP4S", "", REP, 0x0F, 0x26);
        TEST("REPC SCASW", "", REPC, 0xAF);
        TEST("REPC ADD4S", "", REPC, 0x0F, 0x20);
        TEST("REPC SUB4S", "", REPC, 0x0F, 0x22);
        TEST("REPC CMP4S", "", REPC, 0x0F, 0x26);
    }

    TEST("MOVSB", "", 0xA4);    // MOVS m8,m8
    TEST("MOVSW", "", 0xA5);    // MOVS m16,m16
    TEST("CMPSB", "", 0xA6);    // CMPS m8,m8
    TEST("CMPSW", "", 0xA7);    // CMPS m16,m16
    TEST("STOSB", "", 0xAA);    // STOS m8
    TEST("STOSW", "", 0xAB);    // STOS m16
    TEST("LODSB", "", 0xAC);    // LODS m8
    TEST("LODSW", "", 0xAD);    // LODS m16
    TEST("SCASB", "", 0xAE);    // SCAS m8
    TEST("SCASW", "", 0xAF);    // SCAS m16

    if (is80x86()) {
        TEST("INSB",  "", 0x6C); // INS r/m8,DX
        TEST("INSW",  "", 0x6D); // INS r/m16,DX
        TEST("OUTSB", "", 0x6E); // OUTS DX,r/m8
        TEST("OUTSW", "", 0x6F); // OUTS DX,r/m16
    }
    if (is80386()) {
        EPRE("MOVSB", "", DATA32, 0xA4);
        TEST("MOVSD", "", DATA32, 0xA5); // MOVS m32,m32
        EPRE("CMPSB", "", DATA32, 0xA6);
        TEST("CMPSD", "", DATA32, 0xA7); // CMPS m32,m32
        EPRE("STOSB", "", DATA32, 0xAA);
        TEST("STOSD", "", DATA32, 0xAB); // STOS m32
        EPRE("LODSB", "", DATA32, 0xAC);
        TEST("LODSD", "", DATA32, 0xAD); // LODS m32
        EPRE("SCASB", "", DATA32, 0xAE);
        TEST("SCASD", "", DATA32, 0xAF); // SCAS m32

        EPRE("INSB",  "", DATA32, 0x6C);
        TEST("INSD",  "", DATA32, 0x6D); // INS r/m32,DX
        EPRE("OUTSB", "", DATA32, 0x6E);
        TEST("OUTSD", "", DATA32, 0x6F); // OUTS DX,r/m32
    }
    if (v30()) {
        TEST("ADD4S", "", 0x0F, 0x20);
        TEST("CMP4S", "", 0x0F, 0x26);
        TEST("SUB4S", "", 0x0F, 0x22);
    }

    gnu_as(true);

    TEST("movsb", "es:[di], ds:[si]", 0xa4); // MOVS m8,m8
    TEST("movsw", "es:[di], ds:[si]", 0xa5); // MOVS m16,m16
    TEST("cmpsb", "ds:[si], es:[di]", 0xa6); // CMPS m8,m8
    TEST("cmpsw", "ds:[si], es:[di]", 0xa7); // CMPS m16,m16
    TEST("stosb", "es:[di]", 0xaa);          // STOS m8
    TEST("stosw", "es:[di]", 0xab);          // STOS m16
    TEST("lodsb", "ds:[si]", 0xac);          // LODS m8
    TEST("lodsw", "ds:[si]", 0xad);          // LODS m16
    TEST("scasb", "es:[di]", 0xae);          // SCAS m8
    TEST("scasw", "es:[di]", 0xaf);          // SCAS m16

    if (is80x86()) {
        TEST("insb",  "es:[di], dx", 0x6c); // INS r/m8,DX
        TEST("insw",  "es:[di], dx", 0x6d); // INS r/m16,DX
        TEST("outsb", "dx, ds:[si]", 0x6e); // OUTS DX,r/m8
        TEST("outsw", "dx, ds:[si]", 0x6f); // OUTS DX,r/m16
    }
    if (is80386()) {
        EPRE("movsb", "es:[di], ds:[si]", DATA32, 0xa4);
        TEST("movsd", "es:[di], ds:[si]", DATA32, 0xa5); // MOVS m32,m32
        EPRE("cmpsb", "ds:[si], es:[di]", DATA32, 0xa6);
        TEST("cmpsd", "ds:[si], es:[di]", DATA32, 0xa7); // CMPS m32,m32
        EPRE("stosb", "es:[di]", DATA32, 0xaa);
        TEST("stosd", "es:[di]", DATA32, 0xab); // STOS m32
        EPRE("lodsb", "ds:[si]", DATA32, 0xac);
        TEST("lodsd", "ds:[si]", DATA32, 0xad); // LODS m32
        EPRE("scasb", "es:[di]", DATA32, 0xae);
        TEST("scasd", "es:[di]", DATA32, 0xaf); // SCAS m32

        EPRE("insb",  "es:[di], dx", DATA32, 0x6c);
        TEST("insd",  "es:[di], dx", DATA32, 0x6d); // INS r/m32,DX
        EPRE("outsb", "dx, ds:[si]", DATA32, 0x6e);
        TEST("outsd", "dx, ds:[si]", DATA32, 0x6f); // OUTS DX,r/m32
    }
    if (v30()) {
        TEST("add4s", "es:[di], ds:[si]", 0x0f, 0x20);
        TEST("cmp4s", "es:[di], ds:[si]", 0x0f, 0x26);
        TEST("sub4s", "es:[di], ds:[si]", 0x0f, 0x22);
    }
}

void test_lock() {
    lockInsn(false);
    ERRT("LOCK XCHG", "CH, AL",
         ILLEGAL_COMBINATION, "XCHG", LOCK, 0x86, 0305);
    TEST("LOCK XCHG", "[SI], CL",     LOCK, 0x86, 0014);
    ERRT("LOCK XCHG", "AX, BP",
         ILLEGAL_COMBINATION, "XCHG", LOCK, 0x87, 0350);
    TEST("LOCK XCHG", "[1234H], DL",  LOCK, 0x86, 0026, 0x34, 0x12);

    if (is80386()) {
        ERRT("LOCK XCHG", "EAX, EBP",
             ILLEGAL_COMBINATION, "XCHG", LOCK, DATA32, 0x87, 0350);
        TEST("LOCK XCHG", "[1234H], EDX",  LOCK, DATA32, 0x86, 0026, 0x34, 0x12);
    }

    TEST("LOCK XCHG", "[BX], AL",              0xF0, 0x86, 0007);
    TEST("LOCK ADD", "BYTE PTR [SI], 1",       0xF0, 0x80, 0004, 1);
    TEST("LOCK OR",  "[DI], AX",               0xF0, 0x09, 0x05);
    TEST("LOCK ADC", "BYTE PTR [BP+SI+1234H], 5", 0xF0, 0x80, 0222, 0x34, 0x12, 5);
    TEST("LOCK SBB", "[BX+DI-52], DX",         0xF0, 0x19, 0121, 0xCC);
    TEST("LOCK AND", "BYTE PTR [SI+1234H], 0FFH", 0xF0, 0x80, 0244, 0x34, 0x12, 0xFF);
    TEST("LOCK SUB", "[BX], SI",               0xF0, 0x29, 0067);
    TEST("LOCK XOR", "[BP+52], CL",            0xF0, 0x30, 0x4E, 0x34);
    ERRT("LOCK CMP", "WORD PTR [SI], 1234H",
         ILLEGAL_COMBINATION, "CMP",          0xF0, 0x81, 0074, 0x34, 0x12);
    TEST("LOCK NOT", "BYTE PTR [DI]",          0xF0, 0xF6, 0025);
    TEST("LOCK NEG", "WORD PTR [BX+SI]",       0xF0, 0xF7, 0030);
    TEST("LOCK INC", "BYTE PTR [BX+DI+52]",    0xF0, 0xFE, 0101, 0x34);
    TEST("LOCK DEC", "WORD PTR [BP+SI+1234H]", 0xF0, 0xFF, 0212, 0x34, 0x12);

    ERRT("LOCK MOV", "[BX], AL", ILLEGAL_COMBINATION, "MOV",  0xF0, 0x88, 0007);
    ERRT("LOCK ADD", "AL, 1",    ILLEGAL_COMBINATION, "ADD",  0xF0, 0x04, 0001);
    ERRT("LOCK PUSH", "AX",      ILLEGAL_COMBINATION, "PUSH", 0xF0, 0120);
    ERRT("", "", OPCODE_HAS_NO_EFFECT, "", 0xF0, 0xF0);

    TEST("LOCK ADD", "ES:[BX], AL", 0xF0, 0x26, 0x00, 0007);

    ERRT("LOCK ADD", "AH, CH",
         ILLEGAL_COMBINATION, "ADD", LOCK, 0x00, 0354);
    TEST("LOCK ADD", "[DI+52], AL",  LOCK, 0x00, 0105, 0x34);
    ERRT("LOCK ADD", "CL, [BP+1234H]",
         ILLEGAL_COMBINATION, "ADD", LOCK, 0x02, 0216, 0x34, 0x12);
    ERRT("LOCK ADD", "SP, BP",
         ILLEGAL_COMBINATION, "ADD", LOCK, 0x01, 0354);
    TEST("LOCK ADD", "[BX+SI], DX",  LOCK, 0x01, 0020);
    ERRT("LOCK ADD", "BX, [BX+DI+52]",
         ILLEGAL_COMBINATION, "ADD", LOCK, 0x03, 0131, 0x34);
    ERRT("LOCK ADD", "CL, 56H",
         ILLEGAL_COMBINATION, "ADD", LOCK, 0x80, 0301, 0x56);
    TEST("LOCK ADD", "BYTE PTR [BP+SI+1234H], 56H", LOCK, 0x80, 0202, 0x34, 0x12, 0x56);
    ERRT("LOCK ADD", "AX, 5678H",
         ILLEGAL_COMBINATION, "ADD", LOCK, 0x81, 0300, 0x78, 0x56);
    TEST("LOCK ADD", "WORD PTR [SI], 5678H", LOCK, 0x81, 0004, 0x78, 0x56);
    ERRT("LOCK ADD", "AX, -16",
         ILLEGAL_COMBINATION, "ADD", LOCK, 0x83, 0300, 0xF0);
    TEST("LOCK ADD", "WORD PTR [1234H], 56H", LOCK, 0x83, 0006, 0x34, 0x12, 0x56);
    ERRT("LOCK ADD", "AL, 56H",   ILLEGAL_COMBINATION, "ADD", LOCK, 0x04, 0x56);
    ERRT("LOCK ADD", "AX, 5678H", ILLEGAL_COMBINATION, "ADD", LOCK, 0x05, 0x78, 0x56);

    ERRT("LOCK INC", "CH", ILLEGAL_COMBINATION, "INC", LOCK, 0xFE, 0305);
    TEST("LOCK INC", "BYTE PTR [SI]",     LOCK, 0xFE, 0004);
    ERRT("LOCK INC", "BP", ILLEGAL_COMBINATION, "INC", LOCK, 0xFF, 0305);
    TEST("LOCK INC", "WORD PTR [1234H]",  LOCK, 0xFF, 0006, 0x34, 0x12);
    ERRT("LOCK INC", "BP", ILLEGAL_COMBINATION, "INC", LOCK, 0105);

    NMEM("", "", "", LOCK);
    ERRT("", "", OPCODE_HAS_NO_EFFECT, "", LOCK, LOCK);
    TEST("LOCK ADD",  "[SI], CL", LOCK, 0x00, 0014);
    TEST("LOCK ADD",  "[SI], DX", LOCK, 0x01, 0024);
    TEST("LOCK ADC",  "[SI], CL", LOCK, 0x10, 0014);
    TEST("LOCK ADC",  "[SI], DX", LOCK, 0x11, 0024);
    TEST("LOCK SBB",  "[SI], CL", LOCK, 0x18, 0014);
    TEST("LOCK SBB",  "[SI], DX", LOCK, 0x19, 0024);
    TEST("LOCK SUB",  "[SI], CL", LOCK, 0x28, 0014);
    TEST("LOCK SUB",  "[SI], DX", LOCK, 0x29, 0024);
    TEST("LOCK OR",   "[SI], CL", LOCK, 0x08, 0014);
    TEST("LOCK OR",   "[SI], DX", LOCK, 0x09, 0024);
    TEST("LOCK AND",  "[SI], CL", LOCK, 0x20, 0014);
    TEST("LOCK AND",  "[SI], DX", LOCK, 0x21, 0024);
    TEST("LOCK XOR",  "[SI], CL", LOCK, 0x30, 0014);
    TEST("LOCK XOR",  "[SI], DX", LOCK, 0x31, 0024);
    TEST("LOCK XCHG", "[SI], CL", LOCK, 0x86, 0014);
    TEST("LOCK XCHG", "[SI], DX", LOCK, 0x87, 0024);
    TEST("LOCK ADD", "BYTE PTR [SI], 0CCH", LOCK, 0x80, 0004, 0xCC);
    TEST("LOCK OR",  "BYTE PTR [SI], 0CCH", LOCK, 0x80, 0014, 0xCC);
    TEST("LOCK ADC", "BYTE PTR [SI], 0CCH", LOCK, 0x80, 0024, 0xCC);
    TEST("LOCK SBB", "BYTE PTR [SI], 0CCH", LOCK, 0x80, 0034, 0xCC);
    TEST("LOCK AND", "BYTE PTR [SI], 0CCH", LOCK, 0x80, 0044, 0xCC);
    TEST("LOCK SUB", "BYTE PTR [SI], 0CCH", LOCK, 0x80, 0054, 0xCC);
    TEST("LOCK XOR", "BYTE PTR [SI], 0CCH", LOCK, 0x80, 0064, 0xCC);
    ERRT("LOCK CMP", "BYTE PTR [SI], 0CCH",
         ILLEGAL_COMBINATION, "CMP",       LOCK, 0x80, 0074, 0xCC);
    TEST("LOCK ADD", "WORD PTR [SI], -34H", LOCK, 0x83, 0004, 0xCC);
    TEST("LOCK OR",  "WORD PTR [SI], -34H", LOCK, 0x83, 0014, 0xCC);
    TEST("LOCK ADC", "WORD PTR [SI], -34H", LOCK, 0x83, 0024, 0xCC);
    TEST("LOCK SBB", "WORD PTR [SI], -34H", LOCK, 0x83, 0034, 0xCC);
    TEST("LOCK AND", "WORD PTR [SI], -34H", LOCK, 0x83, 0044, 0xCC);
    TEST("LOCK SUB", "WORD PTR [SI], -34H", LOCK, 0x83, 0054, 0xCC);
    TEST("LOCK XOR", "WORD PTR [SI], -34H", LOCK, 0x83, 0064, 0xCC);
    ERRT("LOCK CMP", "WORD PTR [SI], -34H",
         ILLEGAL_COMBINATION, "CMP",        LOCK, 0x83, 0074, 0xCC);
    TEST("LOCK ADD", "WORD PTR [SI], 1234H", LOCK, 0x81, 0004, 0x34, 0x12);
    TEST("LOCK OR",  "WORD PTR [SI], 1234H", LOCK, 0x81, 0014, 0x34, 0x12);
    TEST("LOCK ADC", "WORD PTR [SI], 1234H", LOCK, 0x81, 0024, 0x34, 0x12);
    TEST("LOCK SBB", "WORD PTR [SI], 1234H", LOCK, 0x81, 0034, 0x34, 0x12);
    TEST("LOCK AND", "WORD PTR [SI], 1234H", LOCK, 0x81, 0044, 0x34, 0x12);
    TEST("LOCK SUB", "WORD PTR [SI], 1234H", LOCK, 0x81, 0054, 0x34, 0x12);
    TEST("LOCK XOR", "WORD PTR [SI], 1234H", LOCK, 0x81, 0064, 0x34, 0x12);
    ERRT("LOCK CMP", "WORD PTR [SI], 1234H",
         ILLEGAL_COMBINATION, "CMP",         LOCK, 0x81, 0074, 0x34, 0x12);
    TEST("LOCK NOT", "BYTE PTR [SI]", LOCK, 0xF6, 0024);
    TEST("LOCK NEG", "BYTE PTR [SI]", LOCK, 0xF6, 0034);
    TEST("LOCK NOT", "WORD PTR [SI]", LOCK, 0xF7, 0024);
    TEST("LOCK NEG", "WORD PTR [SI]", LOCK, 0xF7, 0034);
    TEST("LOCK INC", "BYTE PTR [SI]", LOCK, 0xFE, 0004);
    TEST("LOCK DEC", "BYTE PTR [SI]", LOCK, 0xFE, 0014);
    TEST("LOCK INC", "WORD PTR [SI]", LOCK, 0xFF, 0004);
    TEST("LOCK DEC", "WORD PTR [SI]", LOCK, 0xFF, 0014);
}

void test_control_transfer() {
    if (is80386()) {
        // CALL rel16
        ATEST(0x00001000, "CALL", "00001082H",                                0xE8, 0x7F, 0x00);
        ATEST(0x00001000, "CALL", "00009002H",                                0xE8, 0xFF, 0x7F);
        AERRT(0xFFFFF000, "CALL", "00007002H", OVERFLOW_RANGE,   "00007002H", 0xE8, 0xFF, 0x7F);
        ATEST(0x00001000, "CALL", "00000F81H",                                0xE8, 0x7E, 0xFF);
        ATEST(0x00009000, "CALL", "00001003H",                                0xE8, 0x00, 0x80);
        AERRT(0x00001000, "CALL", "0FFFF9003H", OVERFLOW_RANGE, "0FFFF9003H", 0xE8, 0x00, 0x80);
        // CALL rel32
        ATEST(0x00001000, "CALL", "80001005H",                                DATA32, 0xE8, 0xFF, 0xFF, 0xFF, 0x7F);
        AERRT(0x80000000, "CALL", "00000005H", OVERFLOW_RANGE,   "00000005H", DATA32, 0xE8, 0XFF, 0xFF, 0xFF, 0x7F);
        ATEST(0x90000000, "CALL", "10000006H",                                DATA32, 0xE8, 0x00, 0x00, 0x00, 0x80);
        AERRT(0x70000000, "CALL", "0F0000006H", OVERFLOW_RANGE, "0F0000006H", DATA32, 0xE8, 0x00, 0x00, 0x00, 0x80);
    } else if (is80286()) {
        // CALL rel16
        ATEST(0x001000, "CALL", "001082H",                              0xE8, 0x7F, 0x00);
        ATEST(0x001000, "CALL", "009002H",                              0xE8, 0xFF, 0x7F);
        AERRT(0x00F000, "CALL", "017002H", OVERWRAP_SEGMENT, "017002H", 0xE8, 0xFF, 0x7F);
        AERRT(0xFFF000, "CALL", "007002H", OVERFLOW_RANGE,   "007002H", 0xE8, 0xFF, 0x7F);
        ATEST(0x001000, "CALL", "000F81H",                              0xE8, 0x7E, 0xFF);
        ATEST(0x009000, "CALL", "001003H",                              0xE8, 0x00, 0x80);
        AERRT(0x011000, "CALL", "009003H", OVERWRAP_SEGMENT, "009003H", 0xE8, 0x00, 0x80);
        AERRT(0x001000, "CALL", "0FF9003H", OVERFLOW_RANGE, "0FF9003H", 0xE8, 0x00, 0x80);
    } else {
        // CALL rel16
        ATEST(0x01000, "CALL", "01082H",                             0xE8, 0x7F, 0x00);
        ATEST(0x01000, "CALL", "09002H",                             0xE8, 0xFF, 0x7F);
        AERRT(0x0F000, "CALL", "17002H", OVERWRAP_SEGMENT, "17002H", 0xE8, 0xFF, 0x7F);
        AERRT(0xFF000, "CALL", "07002H", OVERFLOW_RANGE,   "07002H", 0xE8, 0xFF, 0x7F);
        ATEST(0x01000, "CALL", "00F81H",                             0xE8, 0x7E, 0xFF);
        ATEST(0x09000, "CALL", "01003H",                             0xE8, 0x00, 0x80);
        AERRT(0x11000, "CALL", "09003H", OVERWRAP_SEGMENT, "09003H", 0xE8, 0x00, 0x80);
        AERRT(0x01000, "CALL", "0F9003H", OVERFLOW_RANGE, "0F9003H", 0xE8, 0x00, 0x80);
    }

    if (is16bit()) {
        // CALL r/m16
        TEST("CALL", "AX",            0xFF, 0320);
        TEST("CALL", "SI",            0xFF, 0326);
        TEST("CALL", "[SI]",          0xFF, 0024);
        TEST("CALL", "[1234H]",       0xFF, 0026, 0x34, 0x12);
        TEST("CALL", "[DI-52]",       0xFF, 0125, 0xCC);
        TEST("CALL", "[BP+1234H]",    0xFF, 0226, 0x34, 0x12);
        TEST("CALL", "[BX+SI]",       0xFF, 0020);
        TEST("CALL", "[BX+DI+52]",    0xFF, 0121, 0x34);
        TEST("CALL", "[BP+SI+1234H]", 0xFF, 0222, 0x34, 0x12);
    } else {
        // CALL r/m16
        TEST("CALL", "AX",            0xFF, 0320);
        TEST("CALL", "SI",            0xFF, 0326);
        TEST("CALL", "WORD [SI]",          0xFF, 0024);
        TEST("CALL", "WORD [1234H]",       0xFF, 0026, 0x34, 0x12);
        TEST("CALL", "WORD [DI-52]",       0xFF, 0125, 0xCC);
        TEST("CALL", "WORD [BP+1234H]",    0xFF, 0226, 0x34, 0x12);
        TEST("CALL", "WORD [BX+SI]",       0xFF, 0020);
        TEST("CALL", "WORD [BX+DI+52]",    0xFF, 0121, 0x34);
        TEST("CALL", "WORD [BP+SI+1234H]", 0xFF, 0222, 0x34, 0x12);

        // CALL r/m32
        TEST("CALL", "EAX",           DATA32, 0xFF, 0320);
        TEST("CALL", "ESI",           DATA32, 0xFF, 0326);
        TEST("CALL", "DWORD [SI]",          DATA32, 0xFF, 0024);
        TEST("CALL", "DWORD [1234H]",       DATA32, 0xFF, 0026, 0x34, 0x12);
        TEST("CALL", "DWORD [DI-52]",       DATA32, 0xFF, 0125, 0xCC);
        TEST("CALL", "DWORD [BP+1234H]",    DATA32, 0xFF, 0226, 0x34, 0x12);
        TEST("CALL", "DWORD [BX+SI]",       DATA32, 0xFF, 0020);
        TEST("CALL", "DWORD [BX+DI+52]",    DATA32, 0xFF, 0121, 0x34);
        TEST("CALL", "DWORD [BP+SI+1234H]", DATA32, 0xFF, 0222, 0x34, 0x12);
    }

    // CALL ptr16:16
    TEST("CALL", "1234H:5678H", 0x9A, 0x78, 0x56, 0x34, 0x12);
    if (is80386()) {
        // CALL ptr16:32
        TEST("CALL", "1234H:56789ABCH", DATA32, 0x9A, 0xBC, 0x9A, 0x78, 0x56, 0x34, 0x12);
        use32();
        TEST("CALL", "1234H:56789ABCH", 0x9A, 0xBC, 0x9A, 0x78, 0x56, 0x34, 0x12);
        use16();
    }

    if (is16bit()) {
        // CALL m16:16
        TEST("CALL", "FAR [SI]",          0xFF, 0034);
        TEST("CALL", "FAR [1234H]",       0xFF, 0036, 0x34, 0x12);
        TEST("CALL", "FAR [DI-52]",       0xFF, 0135, 0xCC);
        TEST("CALL", "FAR [BP+1234H]",    0xFF, 0236, 0x34, 0x12);
        TEST("CALL", "FAR [BX+SI]",       0xFF, 0030);
        TEST("CALL", "FAR [BX+DI+52]",    0xFF, 0131, 0x34);
        TEST("CALL", "FAR [BP+SI+1234H]", 0xFF, 0232, 0x34, 0x12);

        disassembler.setOption("far-insn", "on");
        // CALLF m16:16
        TEST("CALLF", "[SI]",          0xFF, 0034);
        TEST("CALLF", "[1234H]",       0xFF, 0036, 0x34, 0x12);
        TEST("CALLF", "[DI-52]",       0xFF, 0135, 0xCC);
        TEST("CALLF", "[BP+1234H]",    0xFF, 0236, 0x34, 0x12);
        TEST("CALLF", "[BX+SI]",       0xFF, 0030);
        TEST("CALLF", "[BX+DI+52]",    0xFF, 0131, 0x34);
        TEST("CALLF", "[BP+SI+1234H]", 0xFF, 0232, 0x34, 0x12);
        disassembler.setOption("far-insn", "off");
    } else {
        // CALL m16:16
        TEST("CALL", "FAR WORD [SI]",          0xFF, 0034);
        TEST("CALL", "FAR WORD [1234H]",       0xFF, 0036, 0x34, 0x12);
        TEST("CALL", "FAR WORD [DI-52]",       0xFF, 0135, 0xCC);
        TEST("CALL", "FAR WORD [BP+1234H]",    0xFF, 0236, 0x34, 0x12);
        TEST("CALL", "FAR WORD [BX+SI]",       0xFF, 0030);
        TEST("CALL", "FAR WORD [BX+DI+52]",    0xFF, 0131, 0x34);
        TEST("CALL", "FAR WORD [BP+SI+1234H]", 0xFF, 0232, 0x34, 0x12);

        // CALL m16:32
        TEST("CALL", "FAR DWORD [SI]",          DATA32, 0xFF, 0034);
        TEST("CALL", "FAR DWORD [1234H]",       DATA32, 0xFF, 0036, 0x34, 0x12);
        TEST("CALL", "FAR DWORD [DI-52]",       DATA32, 0xFF, 0135, 0xCC);
        TEST("CALL", "FAR DWORD [BP+1234H]",    DATA32, 0xFF, 0236, 0x34, 0x12);
        TEST("CALL", "FAR DWORD [BX+SI]",       DATA32, 0xFF, 0030);
        TEST("CALL", "FAR DWORD [BX+DI+52]",    DATA32, 0xFF, 0131, 0x34);
        TEST("CALL", "FAR DWORD [BP+SI+1234H]", DATA32, 0xFF, 0232, 0x34, 0x12);

        use32();
        TEST("CALL", "FAR DWORD [EAX+EDI*2]",   0xFF, 0034, 0170);
        TEST("CALL", "FAR DWORD [ESI]",         0xFF, 0036);
        TEST("CALL", "FAR DWORD [EBP-52]",      0xFF, 0135, 0xCC);
        TEST("CALL", "FAR DWORD [ESI+12345678H]", 0xFF, 0236, 0x78, 0x56, 0x34, 0x12);
        TEST("CALL", "FAR DWORD [EAX]",         0xFF, 0030);
        TEST("CALL", "FAR DWORD [ECX+52]",      0xFF, 0131, 0x34);
        TEST("CALL", "FAR DWORD [EDX+12345678H]", 0xFF, 0232, 0x78, 0x56, 0x34, 0x12);
        use16();
    }

    if (is80386()) {
        // JMP rel8
        ATEST(0x00001000, "JMP", "00001081H",                                0xEB, 0x7F);
        ATEST(0x00001000, "JMP", "00000F82H",                                0xEB, 0x80);
        // JMP rel16
        ATEST(0x00001000, "JMP", "00001082H",                                0xE9, 0x7F, 0x00);
        ATEST(0x00001000, "JMP", "00009002H",                                0xE9, 0xFF, 0x7F);
        AERRT(0xFFFFF000, "JMP", "00007002H", OVERFLOW_RANGE,   "00007002H", 0xE9, 0xFF, 0x7F);
        ATEST(0x00001000, "JMP", "00000F81H",                                0xE9, 0x7E, 0xFF);
        ATEST(0x00009000, "JMP", "00001003H",                                0xE9, 0x00, 0x80);
        AERRT(0x00001000, "JMP", "0FFFF9003H", OVERFLOW_RANGE, "0FFFF9003H", 0xE9, 0x00, 0x80);
        // JMP rel32
        ATEST(0x00001000, "JMP", "80001005H",                                DATA32, 0xE9, 0xFF, 0xFF, 0xFF, 0x7F);
        AERRT(0x80000000, "JMP", "00000005H", OVERFLOW_RANGE,   "00000005H", DATA32, 0xE9, 0XFF, 0xFF, 0xFF, 0x7F);
        ATEST(0x90000000, "JMP", "10000006H",                                DATA32, 0xE9, 0x00, 0x00, 0x00, 0x80);
        AERRT(0x70000000, "JMP", "0F0000006H", OVERFLOW_RANGE, "0F0000006H", DATA32, 0xE9, 0x00, 0x00, 0x00, 0x80);
    } else if (is80286()) {
        // JMP rel8
        ATEST(0x001000, "JMP", "001081H",                              0xEB, 0x7F);
        AERRT(0x00FFF0, "JMP", "010071H", OVERWRAP_SEGMENT, "010071H", 0xEB, 0x7F);
        ATEST(0x001000, "JMP", "000F82H",                              0xEB, 0x80);
        AERRT(0x010010, "JMP", "00FF92H", OVERWRAP_SEGMENT, "00FF92H", 0xEB, 0x80);
        // JMP rel16
        ATEST(0x001000, "JMP", "001082H",                              0xE9, 0x7F, 0x00);
        ATEST(0x001000, "JMP", "009002H",                              0xE9, 0xFF, 0x7F);
        AERRT(0x00F000, "JMP", "017002H", OVERWRAP_SEGMENT, "017002H", 0xE9, 0xFF, 0x7F);
        AERRT(0xFFF000, "JMP", "007002H", OVERFLOW_RANGE,   "007002H", 0xE9, 0xFF, 0x7F);
        ATEST(0x001000, "JMP", "000F81H",                              0xE9, 0x7E, 0xFF);
        ATEST(0x009000, "JMP", "001003H",                              0xE9, 0x00, 0x80);
        AERRT(0x011000, "JMP", "009003H", OVERWRAP_SEGMENT, "009003H", 0xE9, 0x00, 0x80);
        AERRT(0x001000, "JMP", "0FF9003H", OVERFLOW_RANGE, "0FF9003H", 0xE9, 0x00, 0x80);
    } else {
        // JMP rel8
        ATEST(0x01000, "JMP", "01081H",                             0xEB, 0x7F);
        AERRT(0x0FFF0, "JMP", "10071H", OVERWRAP_SEGMENT, "10071H", 0xEB, 0x7F);
        ATEST(0x01000, "JMP", "00F82H",                             0xEB, 0x80);
        AERRT(0x10010, "JMP", "0FF92H", OVERWRAP_SEGMENT, "0FF92H", 0xEB, 0x80);
        // JMP rel16
        ATEST(0x01000, "JMP", "01082H",                             0xE9, 0x7F, 0x00);
        ATEST(0x01000, "JMP", "09002H",                             0xE9, 0xFF, 0x7F);
        AERRT(0x0F000, "JMP", "17002H", OVERWRAP_SEGMENT, "17002H", 0xE9, 0xFF, 0x7F);
        AERRT(0xFF000, "JMP", "07002H", OVERFLOW_RANGE,   "07002H", 0xE9, 0xFF, 0x7F);
        ATEST(0x01000, "JMP", "00F81H",                             0xE9, 0x7E, 0xFF);
        ATEST(0x09000, "JMP", "01003H",                             0xE9, 0x00, 0x80);
        AERRT(0x11000, "JMP", "09003H", OVERWRAP_SEGMENT, "09003H", 0xE9, 0x00, 0x80);
        AERRT(0x01000, "JMP", "0F9003H", OVERFLOW_RANGE, "0F9003H", 0xE9, 0x00, 0x80);
    }

    if (is16bit()) {
        // JMP r/m16
        TEST("JMP", "AX",            0xFF, 0340);
        TEST("JMP", "SI",            0xFF, 0346);
        TEST("JMP", "[SI]",          0xFF, 0044);
        TEST("JMP", "[1234H]",       0xFF, 0046, 0x34, 0x12);
        TEST("JMP", "[DI-52]",       0xFF, 0145, 0xCC);
        TEST("JMP", "[BP+1234H]",    0xFF, 0246, 0x34, 0x12);
        TEST("JMP", "[BX+SI]",       0xFF, 0040);
        TEST("JMP", "[BX+DI+52]",    0xFF, 0141, 0x34);
        TEST("JMP", "[BP+SI+1234H]", 0xFF, 0242, 0x34, 0x12);
    } else {
        // JMP r/m16
        TEST("JMP", "AX",            0xFF, 0340);
        TEST("JMP", "SI",            0xFF, 0346);
        TEST("JMP", "WORD [SI]",          0xFF, 0044);
        TEST("JMP", "WORD [1234H]",       0xFF, 0046, 0x34, 0x12);
        TEST("JMP", "WORD [DI-52]",       0xFF, 0145, 0xCC);
        TEST("JMP", "WORD [BP+1234H]",    0xFF, 0246, 0x34, 0x12);
        TEST("JMP", "WORD [BX+SI]",       0xFF, 0040);
        TEST("JMP", "WORD [BX+DI+52]",    0xFF, 0141, 0x34);
        TEST("JMP", "WORD [BP+SI+1234H]", 0xFF, 0242, 0x34, 0x12);

        // JMP r/m32
        TEST("JMP", "EAX",            DATA32, 0xFF, 0340);
        TEST("JMP", "ESI",            DATA32, 0xFF, 0346);
        TEST("JMP", "DWORD [SI]",          DATA32, 0xFF, 0044);
        TEST("JMP", "DWORD [1234H]",       DATA32, 0xFF, 0046, 0x34, 0x12);
        TEST("JMP", "DWORD [DI-52]",       DATA32, 0xFF, 0145, 0xCC);
        TEST("JMP", "DWORD [BP+1234H]",    DATA32, 0xFF, 0246, 0x34, 0x12);
        TEST("JMP", "DWORD [BX+SI]",       DATA32, 0xFF, 0040);
        TEST("JMP", "DWORD [BX+DI+52]",    DATA32, 0xFF, 0141, 0x34);
        TEST("JMP", "DWORD [BP+SI+1234H]", DATA32, 0xFF, 0242, 0x34, 0x12);
    }

    // JMP ptr16:16
    TEST("JMP", "1234H:5678H", 0xEA, 0x78, 0x56, 0x34, 0x12);
    if (is80386()) {
        // JMP ptr16:32
        TEST("JMP", "1234H:56789ABCH", DATA32, 0xEA, 0xBC, 0x9A, 0x78, 0x56, 0x34, 0x12);
    }

    if (is16bit()) {
        // JMP m16:16
        TEST("JMP", "FAR [SI]",          0xFF, 0054);
        TEST("JMP", "FAR [1234H]",       0xFF, 0056, 0x34, 0x12);
        TEST("JMP", "FAR [DI-52]",       0xFF, 0155, 0xCC);
        TEST("JMP", "FAR [BP+1234H]",    0xFF, 0256, 0x34, 0x12);
        TEST("JMP", "FAR [BX+SI]",       0xFF, 0050);
        TEST("JMP", "FAR [BX+DI+52]",    0xFF, 0151, 0x34);
        TEST("JMP", "FAR [BP+SI+1234H]", 0xFF, 0252, 0x34, 0x12);

        disassembler.setOption("far-insn", "on");
        // JMP m16:16
        TEST("JMPF", "[SI]",          0xFF, 0054);
        TEST("JMPF", "[1234H]",       0xFF, 0056, 0x34, 0x12);
        TEST("JMPF", "[DI-52]",       0xFF, 0155, 0xCC);
        TEST("JMPF", "[BP+1234H]",    0xFF, 0256, 0x34, 0x12);
        TEST("JMPF", "[BX+SI]",       0xFF, 0050);
        TEST("JMPF", "[BX+DI+52]",    0xFF, 0151, 0x34);
        TEST("JMPF", "[BP+SI+1234H]", 0xFF, 0252, 0x34, 0x12);
        disassembler.setOption("far-insn", "off");
    } else {
        // JMP m16:16
        TEST("JMP", "FAR WORD [SI]",          0xFF, 0054);
        TEST("JMP", "FAR WORD [1234H]",       0xFF, 0056, 0x34, 0x12);
        TEST("JMP", "FAR WORD [DI-52]",       0xFF, 0155, 0xCC);
        TEST("JMP", "FAR WORD [BP+1234H]",    0xFF, 0256, 0x34, 0x12);
        TEST("JMP", "FAR WORD [BX+SI]",       0xFF, 0050);
        TEST("JMP", "FAR WORD [BX+DI+52]",    0xFF, 0151, 0x34);
        TEST("JMP", "FAR WORD [BP+SI+1234H]", 0xFF, 0252, 0x34, 0x12);

        // JMP m16:32
        TEST("JMP", "FAR DWORD [SI]",          DATA32, 0xFF, 0054);
        TEST("JMP", "FAR DWORD [1234H]",       DATA32, 0xFF, 0056, 0x34, 0x12);
        TEST("JMP", "FAR DWORD [DI-52]",       DATA32, 0xFF, 0155, 0xCC);
        TEST("JMP", "FAR DWORD [BP+1234H]",    DATA32, 0xFF, 0256, 0x34, 0x12);
        TEST("JMP", "FAR DWORD [BX+SI]",       DATA32, 0xFF, 0050);
        TEST("JMP", "FAR DWORD [BX+DI+52]",    DATA32, 0xFF, 0151, 0x34);
        TEST("JMP", "FAR DWORD [BP+SI+1234H]", DATA32, 0xFF, 0252, 0x34, 0x12);
    }

    TEST("RET", "",    0xC3);
    // RET imm16
    TEST("RET", "16",  0xC2, 0x10, 0x00);
    TEST("RETF", "",   0xCB);
    // RETF imm16
    TEST("RETF", "16", 0xCA, 0x10, 0x00);

    disassembler.setOption("far-insn", "on");
    TEST("RETF", "",   0xCB);
    TEST("RETF", "16", 0xCA, 0x10, 0x00);
    disassembler.setOption("far-insn", "off");

    // Jcc rel8
    relative(true);
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
    ATEST(0x01000, "JS", "$+129", 0x78, 0x7F);
    ATEST(0x01000, "JS", "$-126", 0x78, 0x80);
    if (is80386()) {
        AERRT(0xFFFFFFC0, "JS", "$+129", OVERFLOW_RANGE, "$+129", 0x78, 0x7F);
        AERRT(0x00000040, "JS", "$-126", OVERFLOW_RANGE, "$-126", 0x78, 0x80);
        AEPRE(0x00010000, "JS",  "$", DATA32, 0x78, 0xFD);
    } else {
        AERRT(0x0FFC0, "JS", "$+129", OVERWRAP_SEGMENT, "$+129", 0x78, 0x7F);
        AERRT(0x10040, "JS", "$-126", OVERWRAP_SEGMENT, "$-126", 0x78, 0x80);
    }

    if (is80386()) {
        // Jcc rel16
        TEST("JO",  "$", 0x0F, 0x80, 0xFC, 0xFF);
        TEST("JNO", "$", 0x0F, 0x81, 0xFC, 0xFF);
        TEST("JB",  "$", 0x0F, 0x82, 0xFC, 0xFF);
        TEST("JAE", "$", 0x0F, 0x83, 0xFC, 0xFF);
        TEST("JE",  "$", 0x0F, 0x84, 0xFC, 0xFF);
        TEST("JNE", "$", 0x0F, 0x85, 0xFC, 0xFF);
        TEST("JBE", "$", 0x0F, 0x86, 0xFC, 0xFF);
        TEST("JA",  "$", 0x0F, 0x87, 0xFC, 0xFF);
        TEST("JS",  "$", 0x0F, 0x88, 0xFC, 0xFF);
        TEST("JNS", "$", 0x0F, 0x89, 0xFC, 0xFF);
        TEST("JPE", "$", 0x0F, 0x8A, 0xFC, 0xFF);
        TEST("JPO", "$", 0x0F, 0x8B, 0xFC, 0xFF);
        TEST("JL",  "$", 0x0F, 0x8C, 0xFC, 0xFF);
        TEST("JGE", "$", 0x0F, 0x8D, 0xFC, 0xFF);
        TEST("JLE", "$", 0x0F, 0x8E, 0xFC, 0xFF);
        TEST("JG",  "$", 0x0F, 0x8F, 0xFC, 0xFF);
        ATEST(0x10000, "JS", "$+8003H", 0x0F, 0x88, 0xFF, 0x7F);
        ATEST(0x10000, "JS", "$-7FFCH", 0x0F, 0x88, 0x00, 0x80);

        // Jcc rel32
        TEST("JO",  "$", DATA32, 0x0F, 0x80, 0xF9, 0xFF, 0xFF, 0xFF);
        TEST("JNO", "$", DATA32, 0x0F, 0x81, 0xF9, 0xFF, 0xFF, 0xFF);
        TEST("JB",  "$", DATA32, 0x0F, 0x82, 0xF9, 0xFF, 0xFF, 0xFF);
        TEST("JAE", "$", DATA32, 0x0F, 0x83, 0xF9, 0xFF, 0xFF, 0xFF);
        TEST("JE",  "$", DATA32, 0x0F, 0x84, 0xF9, 0xFF, 0xFF, 0xFF);
        TEST("JNE", "$", DATA32, 0x0F, 0x85, 0xF9, 0xFF, 0xFF, 0xFF);
        TEST("JBE", "$", DATA32, 0x0F, 0x86, 0xF9, 0xFF, 0xFF, 0xFF);
        TEST("JA",  "$", DATA32, 0x0F, 0x87, 0xF9, 0xFF, 0xFF, 0xFF);
        TEST("JS",  "$", DATA32, 0x0F, 0x88, 0xF9, 0xFF, 0xFF, 0xFF);
        TEST("JNS", "$", DATA32, 0x0F, 0x89, 0xF9, 0xFF, 0xFF, 0xFF);
        TEST("JPE", "$", DATA32, 0x0F, 0x8A, 0xF9, 0xFF, 0xFF, 0xFF);
        TEST("JPO", "$", DATA32, 0x0F, 0x8B, 0xF9, 0xFF, 0xFF, 0xFF);
        TEST("JL",  "$", DATA32, 0x0F, 0x8C, 0xF9, 0xFF, 0xFF, 0xFF);
        TEST("JGE", "$", DATA32, 0x0F, 0x8D, 0xF9, 0xFF, 0xFF, 0xFF);
        TEST("JLE", "$", DATA32, 0x0F, 0x8E, 0xF9, 0xFF, 0xFF, 0xFF);
        TEST("JG",  "$", DATA32, 0x0F, 0x8F, 0xF9, 0xFF, 0xFF, 0xFF);
        relative(false);
        ATEST(0x10000000, "JS", "90000006H", DATA32, 0x0F, 0x88, 0xFF, 0xFF, 0xFF, 0x7F);
        ATEST(0x90000000, "JS", "10000007H", DATA32, 0x0F, 0x88, 0x00, 0x00, 0x00, 0x80);
    }

    relative(true);
    // LOOP rel8
    TEST("LOOP",   "$", 0xE2, 0xFE);
    // LOOPE rel8
    TEST("LOOPE",  "$", 0xE1, 0xFE);
    // LOOPNE rel8
    TEST("LOOPNE", "$", 0xE0, 0xFE);
    if (is80386()) {
        EPRE("LOOP",   "$",      DATA32, 0xE2, 0xFD);
        EPRE("LOOPE",  "$",      DATA32, 0xE1, 0xFD);
        EPRE("LOOPNE", "$",      DATA32, 0xE0, 0xFD);
        TEST("LOOP",   "$, ECX", ADDR32, 0xE2, 0xFD);
        TEST("LOOPE",  "$, ECX", ADDR32, 0xE1, 0xFD);
        TEST("LOOPNE", "$, ECX", ADDR32, 0xE0, 0xFD);
        use32();
        TEST("LOOP",   "$, ECX",         0xE2, 0xFD);
        TEST("LOOPE",  "$, ECX",         0xE1, 0xFD);
        TEST("LOOPNE", "$, ECX",         0xE0, 0xFD);
        TEST("LOOP",   "$, CX",  ADDR16, 0xE2, 0xFD);
        TEST("LOOPE",  "$, CX",  ADDR16, 0xE1, 0xFD);
        TEST("LOOPNE", "$, CX",  ADDR16, 0xE0, 0xFD);
        EPRE("LOOP",   "$",      DATA16, 0xE2, 0xFD);
        EPRE("LOOPE",  "$",      DATA16, 0xE1, 0xFD);
        EPRE("LOOPNE", "$",      DATA16, 0xE0, 0xFD);
        EPRE("LOOP",   "$",      ADDR16, DATA16, 0xE2, 0xFC);
        EPRE("LOOPE",  "$",      DATA16, ADDR16, 0xE1, 0xFC);
        EPRE("LOOPNE", "$",      ADDR16, DATA16, 0xE0, 0xFC);
        use16();
    }

    // JCXZ rel8
    TEST("JCXZ",   "$", 0xE3, 0xFE);
    if (is80386()) {
        // JECXZ rel8
        TEST("JECXZ", "$", ADDR32, 0xE3, 0xFD);
        use32();
        TEST("JECXZ", "$",         0xE3, 0xFE);
        TEST("JCXZ", "$",  ADDR16, 0xE3, 0xFD);
        use16();
    }
    relative(false);

    gnu_as(true);
    if (is80386()) {
        ATEST(0x10000, "js", "0x00018003", 0x0F, 0x88, 0xFF, 0x7F);
        ATEST(0x10000, "js", "0x00008004", 0x0F, 0x88, 0x00, 0x80);
        ATEST(0x10000000, "data32 js", "0x90000006", DATA32, 0x0F, 0x88, 0xFF, 0xFF, 0xFF, 0x7F);
        ATEST(0x90000000, "data32 js", "0x10000007", DATA32, 0x0F, 0x88, 0x00, 0x00, 0x00, 0x80);

        AEPRE(0x10000, "loop",  "0x0000FFF0", DATA32, 0xE2, 0xED);
        ATEST(0x10000, "jecxz", "0x0000FFF0", ADDR32, 0xE3, 0xED);
    }
    gnu_as(false);

    // INT imm8
    TEST("INT",  "2", 0xCD, 0x02);
    TEST("INT",  "3", 0xCC);
    TEST("INTO", "",  0xCE);
    TEST("IRET", "",  0xCF);
    if (is80386()) {
        TEST("IRETD", "", DATA32, 0xCF);
    }

    if (is80x86()) {
        // ENTER imm16,imm8
        TEST("ENTER", "30, 1",     0xC8, 0x1E, 0x00, 0x01);
        TEST("ENTER", "0FFFFH, 0", 0xC8, 0xFF, 0xFF, 0x00);
        TEST("ENTER", "0, 255",    0xC8, 0x00, 0x00, 0xFF);

        TEST("LEAVE", "", 0xC9);

        // BOUND r16,m16&16
        TEST("BOUND", "SI, [SI]",          0x62, 0064);
        TEST("BOUND", "DI, [1234H]",       0x62, 0076, 0x34, 0x12);
        TEST("BOUND", "AX, [DI+52]",       0x62, 0105, 0x34);
        TEST("BOUND", "CX, [BP+1234H]",    0x62, 0216, 0x34, 0x12);
        TEST("BOUND", "DX, [BX+SI]",       0x62, 0020);
        TEST("BOUND", "BX, [BX+DI+52]",    0x62, 0131, 0x34);
        TEST("BOUND", "SP, [BP+SI+1234H]", 0x62, 0242, 0x34, 0x12);
        ERRT("BOUND", "SI, AX",  ILLEGAL_OPERAND, "AX", 0x62, 0360);
        ERRT("BOUND", "SP, DI",  ILLEGAL_OPERAND, "DI", 0x62, 0347);
    }
    if (is80386()) {
        // BOUND r32,m32&32
        TEST("BOUND", "ESI, [SI]",          DATA32, 0x62, 0064);
        TEST("BOUND", "EDI, [1234H]",       DATA32, 0x62, 0076, 0x34, 0x12);
        TEST("BOUND", "EAX, [DI+52]",       DATA32, 0x62, 0105, 0x34);
        TEST("BOUND", "ECX, [BP+1234H]",    DATA32, 0x62, 0216, 0x34, 0x12);
        TEST("BOUND", "EDX, [BX+SI]",       DATA32, 0x62, 0020);
        TEST("BOUND", "EBX, [BX+DI+52]",    DATA32, 0x62, 0131, 0x34);
        TEST("BOUND", "ESP, [BP+SI+1234H]", DATA32, 0x62, 0242, 0x34, 0x12);
        ERRT("BOUND", "ESI, EAX",  ILLEGAL_OPERAND, "EAX", DATA32, 0x62, 0360);
        ERRT("BOUND", "ESP, EDI",  ILLEGAL_OPERAND, "EDI", DATA32, 0x62, 0347);
    }

    if (v30()) {
        TEST("BRKEM", "64", 0x0F, 0xFF, 0x40);
    }

    gnu_as(true);

    if (is80386()) {
        // CALL rel32
        ATEST(0x00001000, "data32 call", "0x80001005",                               DATA32, 0xE8, 0xFF, 0xFF, 0xFF, 0x7F);
        AERRT(0x80000000, "data32 call", "0x00000005", OVERFLOW_RANGE, "0x00000005", DATA32, 0xE8, 0XFF, 0xFF, 0xFF, 0x7F);
        ATEST(0x90000000, "data32 call", "0x10000006",                               DATA32, 0xE8, 0x00, 0x00, 0x00, 0x80);
        AERRT(0x70000000, "data32 call", "0xF0000006", OVERFLOW_RANGE, "0xF0000006", DATA32, 0xE8, 0x00, 0x00, 0x00, 0x80);

        // JMP rel32
        ATEST(0x00001000, "data32 jmp", "0x80001005",                               DATA32, 0xE9, 0xFF, 0xFF, 0xFF, 0x7F);
        AERRT(0x80000000, "data32 jmp", "0x00000005", OVERFLOW_RANGE, "0x00000005", DATA32, 0xE9, 0XFF, 0xFF, 0xFF, 0x7F);
        ATEST(0x90000000, "data32 jmp", "0x10000006",                               DATA32, 0xE9, 0x00, 0x00, 0x00, 0x80);
        AERRT(0x70000000, "data32 jmp", "0xF0000006", OVERFLOW_RANGE, "0xF0000006", DATA32, 0xE9, 0x00, 0x00, 0x00, 0x80);
    }

    TEST("lcall", "0x1234, 0x5678", 0x9a, 0x78, 0x56, 0x34, 0x12);
    TEST("lcall", "[si]",           0xff, 0034);
    TEST("lcall", "[0x1234]",       0xff, 0036, 0x34, 0x12);
    TEST("lcall", "[di-52]",        0xff, 0135, 0xcc);
    TEST("lcall", "[bp+0x1234]",    0xff, 0236, 0x34, 0x12);
    TEST("lcall", "[bx+si]",        0xff, 0030);
    TEST("lcall", "[bx+di+52]",     0xff, 0131, 0x34);
    TEST("lcall", "[bp+si+0x1234]", 0xff, 0232, 0x34, 0x12);
    if (is80386()) {
        TEST("data32 lcall", "0x1234, 0x56789ABC", DATA32, 0x9a, 0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12);
        EPRE("lcall", "[si]",           DATA32, 0xff, 0034);
        EPRE("lcall", "[0x1234]",       DATA32, 0xff, 0036, 0x34, 0x12);
        EPRE("lcall", "[di-52]",        DATA32, 0xff, 0135, 0xcc);
        EPRE("lcall", "[bp+0x1234]",    DATA32, 0xff, 0236, 0x34, 0x12);
        EPRE("lcall", "[bx+si]",        DATA32, 0xff, 0030);
        EPRE("lcall", "[bx+di+52]",     DATA32, 0xff, 0131, 0x34);
        EPRE("lcall", "[bp+si+0x1234]", DATA32, 0xff, 0232, 0x34, 0x12);

        use32();
        TEST("lcall", "0x1234, 0x56789ABC", 0x9a, 0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12);
        TEST("lcall", "[si]",           ADDR32, 0xff, 0034);
        TEST("lcall", "[0x1234]",       ADDR32, 0xff, 0036, 0x34, 0x12);
        TEST("lcall", "[di-52]",        ADDR32, 0xff, 0135, 0xcc);
        TEST("lcall", "[bp+0x1234]",    ADDR32, 0xff, 0236, 0x34, 0x12);
        TEST("lcall", "[bx+si]",        ADDR32, 0xff, 0030);
        TEST("lcall", "[bx+di+52]",     ADDR32, 0xff, 0131, 0x34);
        TEST("lcall", "[bp+si+0x1234]", ADDR32, 0xff, 0232, 0x34, 0x12);
        TEST("lcall", "0x1234, 0x56789ABC", 0x9a, 0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12);
        TEST("lcall", "[si]",           ADDR32, 0xff, 0034);
        TEST("lcall", "[0x1234]",       ADDR32, 0xff, 0036, 0x34, 0x12);
        TEST("lcall", "[di-52]",        ADDR32, 0xff, 0135, 0xcc);
        TEST("lcall", "[bp+0x1234]",    ADDR32, 0xff, 0236, 0x34, 0x12);
        TEST("lcall", "[bx+si]",        ADDR32, 0xff, 0030);
        TEST("lcall", "[bx+di+52]",     ADDR32, 0xff, 0131, 0x34);
        TEST("lcall", "[bp+si+0x1234]", ADDR32, 0xff, 0232, 0x34, 0x12);
        use16();
    }

    TEST("ljmp", "0x1234, 0x5678", 0xea, 0x78, 0x56, 0x34, 0x12);
    TEST("ljmp", "[si]",           0xff, 0054);
    TEST("ljmp", "[0x1234]",       0xff, 0056, 0x34, 0x12);
    TEST("ljmp", "[di-52]",        0xff, 0155, 0xcc);
    TEST("ljmp", "[bp+0x1234]",    0xff, 0256, 0x34, 0x12);
    TEST("ljmp", "[bx+si]",        0xff, 0050);
    TEST("ljmp", "[bx+di+52]",     0xff, 0151, 0x34);
    TEST("ljmp", "[bp+si+0x1234]", 0xff, 0252, 0x34, 0x12);
    if (is80386()) {
        TEST("data32 ljmp", "0x1234, 0x56789ABC", DATA32, 0xea, 0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12);
        EPRE("ljmp", "[si]",           DATA32, 0xff, 0054);
        EPRE("ljmp", "[0x1234]",       DATA32, 0xff, 0056, 0x34, 0x12);
        EPRE("ljmp", "[di-52]",        DATA32, 0xff, 0155, 0xcc);
        EPRE("ljmp", "[bp+0x1234]",    DATA32, 0xff, 0256, 0x34, 0x12);
        EPRE("ljmp", "[bx+si]",        DATA32, 0xff, 0050);
        EPRE("ljmp", "[bx+di+52]",     DATA32, 0xff, 0151, 0x34);
        EPRE("ljmp", "[bp+si+0x1234]", DATA32, 0xff, 0252, 0x34, 0x12);

        use32();
        TEST("ljmp", "0x1234, 0x56789ABC", 0xea, 0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12);
        TEST("ljmp", "[si]",           ADDR32, 0xff, 0054);
        TEST("ljmp", "[0x1234]",       ADDR32, 0xff, 0056, 0x34, 0x12);
        TEST("ljmp", "[di-52]",        ADDR32, 0xff, 0155, 0xcc);
        TEST("ljmp", "[bp+0x1234]",    ADDR32, 0xff, 0256, 0x34, 0x12);
        TEST("ljmp", "[bx+si]",        ADDR32, 0xff, 0050);
        TEST("ljmp", "[bx+di+52]",     ADDR32, 0xff, 0151, 0x34);
        TEST("ljmp", "[bp+si+0x1234]", ADDR32, 0xff, 0252, 0x34, 0x12);
        TEST("ljmp", "0x1234, 0x5678", DATA32, 0xea, 0x78, 0x56, 0x34, 0x12);
        EPRE("ljmp", "[esp+ebx*4]",    DATA32, 0xff, 0054, 0234);
        EPRE("ljmp", "[esi]",          DATA32, 0xff, 0056);
        EPRE("ljmp", "[ebp-52]",       DATA32, 0xff, 0155, 0xcc);
        EPRE("ljmp", "[esi+0x12345678]", DATA32, 0xff, 0256, 0x78, 0x56, 0x34, 0x12);
        EPRE("ljmp", "[eax]",          DATA32, 0xff, 0050);
        EPRE("ljmp", "[ecx+52]",       DATA32, 0xff, 0151, 0x34);
        EPRE("ljmp", "[edx+0x12345678]", DATA32, 0xff, 0252, 0x78, 0x56, 0x34, 0x12);
        use16();
    }

    TEST("lret", "",   0xcb);
    TEST("lret", "16", 0xca, 0x10, 0x00);
}

void test_processor_control() {
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

    if (is80286() || is80386()) {
        // ARPL r/m16,r16
        TEST("ARPL", "[BX+SI], SI",       0x63, 0060);
        TEST("ARPL", "[BX+DI+52], DI",    0x63, 0171, 0x34);
        TEST("ARPL", "[BP+SI+1234H], AX", 0x63, 0202, 0x34, 0x12);
        TEST("ARPL", "BX, CX",            0x63, 0313);
        TEST("ARPL", "[SI], DX",          0x63, 0024);
        TEST("ARPL", "[DI+52], BX",       0x63, 0135, 0x34);
        TEST("ARPL", "[BP+1234H], SP",    0x63, 0246, 0x34, 0x12);
        TEST("ARPL", "DI, BP",            0x63, 0357);
        if (is80386()) {
            EPRE("ARPL", "[BX+SI], SI", DATA32, 0x63, 0060);
            TEST("ARPL", "[EAX], SI",   ADDR32, 0x63, 0060);
            use32();
            TEST("ARPL", "[EAX], SI",           0x63, 0060);
            EPRE("ARPL", "[EAX], SI",   DATA32, 0x63, 0060);
            TEST("ARPL", "[BX+SI], SI", ADDR32, 0x63, 0060);
            use16();
        }

        // LAR r16,r/m16
        TEST("LAR", "SI, [BX+SI+52]",    0x0F, 0x02, 0160, 0x34);
        TEST("LAR", "DI, [BX+DI+1234H]", 0x0F, 0x02, 0271, 0x34, 0x12);
        TEST("LAR", "AX, DX",            0x0F, 0x02, 0302);
        TEST("LAR", "CX, [BP+DI]",       0x0F, 0x02, 0013);
        TEST("LAR", "DX, [SI+52]",       0x0F, 0x02, 0124, 0x34);
        TEST("LAR", "BX, [DI+1234H]",    0x0F, 0x02, 0235, 0x34, 0x12);
        TEST("LAR", "SP, SI",            0x0F, 0x02, 0346);
        TEST("LAR", "BP, [BX]",          0x0F, 0x02, 0057);
        if (is80386()) {
            // LAR r32,r/m32
            TEST("LAR", "ESI, [BX+SI+52]", DATA32,         0x0F, 0x02, 0160, 0x34);
            TEST("LAR", "SI, [EAX+52]",    ADDR32,         0x0F, 0x02, 0160, 0x34);
            TEST("LAR", "ESI, [EAX+52]",   DATA32, ADDR32, 0x0F, 0x02, 0160, 0x34);
            use32();
            TEST("LAR", "ESI, [EAX+52]",                   0x0F, 0x02, 0160, 0x34);
            TEST("LAR", "SI, [EAX+52]",    DATA32,         0x0F, 0x02, 0160, 0x34);
            TEST("LAR", "ESI, [BX+SI+52]", ADDR32,         0x0F, 0x02, 0160, 0x34);
            TEST("LAR", "SI, [BX+SI+52]",  ADDR32, DATA32, 0x0F, 0x02, 0160, 0x34);
            use16();
        }

        // LSL r16,r/m16
        TEST("LSL", "SI, [BX+SI+1234H]", 0x0F, 0x03, 0260, 0x34, 0x12);
        TEST("LSL", "DI, CX",            0x0F, 0x03, 0371);
        TEST("LSL", "AX, [BP+SI]",       0x0F, 0x03, 0002);
        TEST("LSL", "CX, [BP+DI+52]",    0x0F, 0x03, 0113, 0x34);
        TEST("LSL", "DX, [SI+1234H]",    0x0F, 0x03, 0224, 0x34, 0x12);
        TEST("LSL", "BX, BP",            0x0F, 0x03, 0335);
        TEST("LSL", "SP, [1234H]",       0x0F, 0x03, 0046, 0x34, 0x12);
        TEST("LSL", "BP, [BX+52]",       0x0F, 0x03, 0157, 0x34);
        // LSL r32,r/m32

        NMEM("", "", "", 0x0F, 0x01);
        // LGDT m16&32
        TEST("LGDT", "[BX+SI]",       0x0F, 0x01, 0020);
        TEST("LGDT", "[BX+DI+52]",    0x0F, 0x01, 0121, 0x34);
        TEST("LGDT", "[BP+SI+1234H]", 0x0F, 0x01, 0222, 0x34, 0x12);
        TEST("LGDT", "[BP+DI]",       0x0F, 0x01, 0023);
        TEST("LGDT", "[SI+52]",       0x0F, 0x01, 0124, 0x34);
        TEST("LGDT", "[DI+1234H]",    0x0F, 0x01, 0225, 0x34, 0x12);
        TEST("LGDT", "[1234H]",       0x0F, 0x01, 0026, 0x34, 0x12);
        TEST("LGDT", "[BX+52]",       0x0F, 0x01, 0127, 0x34);
        ERRT("LGDT", "AX", ILLEGAL_OPERAND, "AX", 0x0F, 0x01, 0320);
        ERRT("LGDT", "DI", ILLEGAL_OPERAND, "DI", 0x0F, 0x01, 0327);

        // SGDT m16&32
        TEST("SGDT", "[BX+SI+1234H]", 0x0F, 0x01, 0200, 0x34, 0x12);
        TEST("SGDT", "[BX+DI]",       0x0F, 0x01, 0001);
        TEST("SGDT", "[BP+SI+52]",    0x0F, 0x01, 0102, 0x34);
        TEST("SGDT", "[BP+DI+1234H]", 0x0F, 0x01, 0203, 0x34, 0x12);
        TEST("SGDT", "[SI]",          0x0F, 0x01, 0004);
        TEST("SGDT", "[DI+52]",       0x0F, 0x01, 0105, 0x34);
        TEST("SGDT", "[BP+1234H]",    0x0F, 0x01, 0206, 0x34, 0x12);
        TEST("SGDT", "[BX]",          0x0F, 0x01, 0007);
        ERRT("SGDT", "CX", ILLEGAL_OPERAND, "CX", 0x0F, 0x01, 0301);
        ERRT("SGDT", "SI", ILLEGAL_OPERAND, "SI", 0x0F, 0x01, 0306);

        NMEM("", "", "", 0x0F, 0x00);
        // LLDT r/m16
        TEST("LLDT", "[BX+SI+52]",    0x0F, 0x00, 0120, 0x34);
        TEST("LLDT", "[BX+DI+1234H]", 0x0F, 0x00, 0221, 0x34, 0x12);
        TEST("LLDT", "DX",            0x0F, 0x00, 0322);
        TEST("LLDT", "[BP+DI]",       0x0F, 0x00, 0023);
        TEST("LLDT", "[SI+52]",       0x0F, 0x00, 0124, 0x34);
        TEST("LLDT", "[DI+1234H]",    0x0F, 0x00, 0225, 0x34, 0x12);
        TEST("LLDT", "SI",            0x0F, 0x00, 0326);
        TEST("LLDT", "[BX]",          0x0F, 0x00, 0027);

        // SLDR r/m16
        TEST("SLDT", "[BX+SI+1234H]", 0x0F, 0x00, 0200, 0x34, 0x12);
        TEST("SLDT", "CX",            0x0F, 0x00, 0301);
        TEST("SLDT", "[BP+SI]",       0x0F, 0x00, 0002);
        TEST("SLDT", "[BP+DI+52]",    0x0F, 0x00, 0103, 0x34);
        TEST("SLDT", "[SI+1234H]",    0x0F, 0x00, 0204, 0x34, 0x12);
        TEST("SLDT", "BP",            0x0F, 0x00, 0305);
        TEST("SLDT", "[1234H]",       0x0F, 0x00, 0006, 0x34, 0x12);
        TEST("SLDT", "[BX+52]",       0x0F, 0x00, 0107, 0x34);

        // LIDT m16&32
        TEST("LIDT", "[BX+SI+52]",    0x0F, 0x01, 0130, 0x34);
        TEST("LIDT", "[BX+DI+1234H]", 0x0F, 0x01, 0231, 0x34, 0x12);
        TEST("LIDT", "[BP+SI]",       0x0F, 0x01, 0032);
        TEST("LIDT", "[BP+DI+52]",    0x0F, 0x01, 0133, 0x34);
        TEST("LIDT", "[SI+1234H]",    0x0F, 0x01, 0234, 0x34, 0x12);
        TEST("LIDT", "[DI]",          0x0F, 0x01, 0035);
        TEST("LIDT", "[BP+52]",       0x0F, 0x01, 0136, 0x34);
        TEST("LIDT", "[BX+1234H]",    0x0F, 0x01, 0237, 0x34, 0x12);
        ERRT("LIDT", "DX", ILLEGAL_OPERAND, "DX", 0x0F, 0x01, 0332);
        ERRT("LIDT", "BP", ILLEGAL_OPERAND, "BP", 0x0F, 0x01, 0335);

        // SIDT m16&32
        TEST("SIDT", "[BX+SI]",       0x0F, 0x01, 0010);
        TEST("SIDT", "[BX+DI+52]",    0x0F, 0x01, 0111, 0x34);
        TEST("SIDT", "[BP+SI+1234H]", 0x0F, 0x01, 0212, 0x34, 0x12);
        TEST("SIDT", "[BP+DI]",       0x0F, 0x01, 0013);
        TEST("SIDT", "[SI+52]",       0x0F, 0x01, 0114, 0x34);
        TEST("SIDT", "[DI+1234H]",    0x0F, 0x01, 0215, 0x34, 0x12);
        TEST("SIDT", "[1234H]",       0x0F, 0x01, 0016, 0x34, 0x12);
        TEST("SIDT", "[BX+52]",       0x0F, 0x01, 0117, 0x34);
        ERRT("SIDT", "BX", ILLEGAL_OPERAND, "BX", 0x0F, 0x01, 0313);
        ERRT("SIDT", "SP", ILLEGAL_OPERAND, "SP", 0x0F, 0x01, 0314);

        // LTR r/m16
        TEST("LTR", "[BX+SI+52]",    0x0F, 0x00, 0130, 0x34);
        TEST("LTR", "[BX+DI+1234H]", 0x0F, 0x00, 0231, 0x34, 0x12);
        TEST("LTR", "DX",            0x0F, 0x00, 0332);
        TEST("LTR", "[BP+DI]",       0x0F, 0x00, 0033);
        TEST("LTR", "[SI+52]",       0x0F, 0x00, 0134, 0x34);
        TEST("LTR", "[DI+1234H]",    0x0F, 0x00, 0235, 0x34, 0x12);
        TEST("LTR", "SI",            0x0F, 0x00, 0336);
        TEST("LTR", "[BX]",          0x0F, 0x00, 0037);

        // STR r/m16
        TEST("STR", "[BX+SI+1234H]", 0x0F, 0x00, 0210, 0x34, 0x12);
        TEST("STR", "CX",            0x0F, 0x00, 0311);
        TEST("STR", "[BP+SI]",       0x0F, 0x00, 0012);
        TEST("STR", "[BP+DI+52]",    0x0F, 0x00, 0113, 0x34);
        TEST("STR", "[SI+1234H]",    0x0F, 0x00, 0214, 0x34, 0x12);
        TEST("STR", "BP",            0x0F, 0x00, 0315);
        TEST("STR", "[1234H]",       0x0F, 0x00, 0016, 0x34, 0x12);
        TEST("STR", "[BX+52]",       0x0F, 0x00, 0117, 0x34);

        // VERR r/m16
        TEST("VERR", "AX",            0x0F, 0x00, 0340);
        TEST("VERR", "[BX+DI]",       0x0F, 0x00, 0041);
        TEST("VERR", "[BP+SI+52]",    0x0F, 0x00, 0142, 0x34);
        TEST("VERR", "[BP+DI+1234H]", 0x0F, 0x00, 0243, 0x34, 0x12);
        TEST("VERR", "SP",            0x0F, 0x00, 0344);
        TEST("VERR", "[DI]",          0x0F, 0x00, 0045);
        TEST("VERR", "[BP+52]",       0x0F, 0x00, 0146, 0x34);
        TEST("VERR", "[BX+1234H]",    0x0F, 0x00, 0247, 0x34, 0x12);

        // VERW r/m16
        TEST("VERW", "[BX+SI]",       0x0F, 0x00, 0050);
        TEST("VERW", "[BX+DI+52]",    0x0F, 0x00, 0151, 0x34);
        TEST("VERW", "[BP+SI+1234H]", 0x0F, 0x00, 0252, 0x34, 0x12);
        TEST("VERW", "BX",            0x0F, 0x00, 0353);
        TEST("VERW", "[SI]",          0x0F, 0x00, 0054);
        TEST("VERW", "[DI+52]",       0x0F, 0x00, 0155, 0x34);
        TEST("VERW", "[BP+1234H]",    0x0F, 0x00, 0256, 0x34, 0x12);
        TEST("VERW", "DI",            0x0F, 0x00, 0357);

        TEST("CLTS", "", 0x0F, 0x06);

        // LMSW r/m16
        TEST("LMSW", "[BX+SI+52]",    0x0F, 0x01, 0160, 0x34);
        TEST("LMSW", "[BX+DI+1234H]", 0x0F, 0x01, 0261, 0x34, 0x12);
        TEST("LMSW", "DX",            0x0F, 0x01, 0362);
        TEST("LMSW", "[BP+DI]",       0x0F, 0x01, 0063);
        TEST("LMSW", "[SI+52]",       0x0F, 0x01, 0164, 0x34);
        TEST("LMSW", "[DI+1234H]",    0x0F, 0x01, 0265, 0x34, 0x12);
        TEST("LMSW", "SI",            0x0F, 0x01, 0366);
        TEST("LMSW", "[BX]",          0x0F, 0x01, 0067);

        // SMSW r/m16
        TEST("SMSW", "[BX+SI+1234H]", 0x0F, 0x01, 0240, 0x34, 0x12);
        TEST("SMSW", "CX",            0x0F, 0x01, 0341);
        TEST("SMSW", "[BP+SI]",       0x0F, 0x01, 0042);
        TEST("SMSW", "[BP+DI+52]",    0x0F, 0x01, 0143, 0x34);
        TEST("SMSW", "[SI+1234H]",    0x0F, 0x01, 0244, 0x34, 0x12);
        TEST("SMSW", "BP",            0x0F, 0x01, 0345);
        TEST("SMSW", "[1234H]",       0x0F, 0x01, 0046, 0x34, 0x12);
        TEST("SMSW", "[BX+52]",       0x0F, 0x01, 0147, 0x34);
    }

    if (is80386()) {
        TEST("LAR", "ESI, [BX+SI+52]",    DATA32, 0x0F, 0x02, 0160, 0x34);
        TEST("LAR", "EDI, [BX+DI+1234H]", DATA32, 0x0F, 0x02, 0271, 0x34, 0x12);
        TEST("LAR", "EAX, EDX",           DATA32, 0x0F, 0x02, 0302);
        TEST("LAR", "ECX, [BP+DI]",       DATA32, 0x0F, 0x02, 0013);
        TEST("LAR", "EDX, [SI+52]",       DATA32, 0x0F, 0x02, 0124, 0x34);
        TEST("LAR", "EBX, [DI+1234H]",    DATA32, 0x0F, 0x02, 0235, 0x34, 0x12);
        TEST("LAR", "ESP, ESI",           DATA32, 0x0F, 0x02, 0346);
        TEST("LAR", "EBP, [BX]",          DATA32, 0x0F, 0x02, 0057);

        TEST("LSL", "ESI, [BX+SI+1234H]", DATA32, 0x0F, 0x03, 0260, 0x34, 0x12);
        TEST("LSL", "EDI, ECX",           DATA32, 0x0F, 0x03, 0371);
        TEST("LSL", "EAX, [BP+SI]",       DATA32, 0x0F, 0x03, 0002);
        TEST("LSL", "ECX, [BP+DI+52]",    DATA32, 0x0F, 0x03, 0113, 0x34);
        TEST("LSL", "EDX, [SI+1234H]",    DATA32, 0x0F, 0x03, 0224, 0x34, 0x12);
        TEST("LSL", "EBX, EBP",           DATA32, 0x0F, 0x03, 0335);
        TEST("LSL", "ESP, [1234H]",       DATA32, 0x0F, 0x03, 0046, 0x34, 0x12);
        TEST("LSL", "EBP, [BX+52]",       DATA32, 0x0F, 0x03, 0157, 0x34);

        // MOV r32,CRx
        TEST("MOV", "EDI, CR0", 0x0F, 0x20, 0307);
        TEST("MOV", "ESI, CR1", 0x0F, 0x20, 0316);
        TEST("MOV", "EBX, CR2", 0x0F, 0x20, 0323);
        TEST("MOV", "EDX, CR3", 0x0F, 0x20, 0332);
        ERRT("MOV", "EAX, ", ILLEGAL_REGISTER, "", 0x0F, 0x20, 0340);
        ERRT("MOV", "ECX, ", ILLEGAL_REGISTER, "", 0x0F, 0x20, 0351);
        ERRT("MOV", "EDX, ", ILLEGAL_REGISTER, "", 0x0F, 0x20, 0362);
        ERRT("MOV", "EBX, ", ILLEGAL_REGISTER, "", 0x0F, 0x20, 0373);
        ERRT("MOV", "EDX, CR1", ILLEGAL_OPERAND_MODE, "EDX, CR1", 0x0F, 0x20, 0012);
        ERRT("MOV", "EBX, CR2", ILLEGAL_OPERAND_MODE, "EBX, CR2", 0x0F, 0x20, 0123);
        ERRT("MOV", "ESP, CR3", ILLEGAL_OPERAND_MODE, "ESP, CR3", 0x0F, 0x20, 0234);
        // MOV CRx,r32
        TEST("MOV", "CR0, EBX", 0x0F, 0x22, 0303);
        TEST("MOV", "CR1, EDX", 0x0F, 0x22, 0312);
        TEST("MOV", "CR2, ECX", 0x0F, 0x22, 0321);
        TEST("MOV", "CR3, EAX", 0x0F, 0x22, 0330);
        ERRT("MOV", ", EDI", ILLEGAL_REGISTER,      ", EDI", 0x0F, 0x22, 0347);
        ERRT("MOV", ", ESI", ILLEGAL_REGISTER,      ", ESI", 0x0F, 0x22, 0356);
        ERRT("MOV", ", EBP", ILLEGAL_REGISTER,      ", EBP", 0x0F, 0x22, 0365);
        ERRT("MOV", ", ESP", ILLEGAL_REGISTER,      ", ESP", 0x0F, 0x22, 0374);
        ERRT("MOV", "CR0, EBX", ILLEGAL_OPERAND_MODE, "EBX", 0x0F, 0x22, 0003);
        ERRT("MOV", "CR1, EDX", ILLEGAL_OPERAND_MODE, "EDX", 0x0F, 0x22, 0112);
        ERRT("MOV", "CR2, ECX", ILLEGAL_OPERAND_MODE, "ECX", 0x0F, 0x22, 0221);

        // MOV r32,DRx
        TEST("MOV", "EDI, DR0", 0x0F, 0x21, 0307);
        TEST("MOV", "ESI, DR1", 0x0F, 0x21, 0316);
        TEST("MOV", "EBX, DR2", 0x0F, 0x21, 0323);
        TEST("MOV", "EDX, DR3", 0x0F, 0x21, 0332);
        ERRT("MOV", "ECX, ", ILLEGAL_REGISTER, "", 0x0F, 0x21, 0341);
        ERRT("MOV", "EAX, ", ILLEGAL_REGISTER, "", 0x0F, 0x21, 0350);
        TEST("MOV", "EDI, DR6", 0x0F, 0x21, 0367);
        TEST("MOV", "ESI, DR7", 0x0F, 0x21, 0376);
        ERRT("MOV", "EDX, DR1", ILLEGAL_OPERAND_MODE, "EDX, DR1", 0x0F, 0x21, 0012);
        ERRT("MOV", "EBX, DR2", ILLEGAL_OPERAND_MODE, "EBX, DR2", 0x0F, 0x21, 0123);
        ERRT("MOV", "ESP, DR3", ILLEGAL_OPERAND_MODE, "ESP, DR3", 0x0F, 0x21, 0234);
        // MOV DRx,r32
        TEST("MOV", "DR0, EBX", 0x0F, 0x23, 0303);
        TEST("MOV", "DR1, EDX", 0x0F, 0x23, 0312);
        TEST("MOV", "DR2, ECX", 0x0F, 0x23, 0321);
        TEST("MOV", "DR3, EAX", 0x0F, 0x23, 0330);
        ERRT("MOV", ", EDI", ILLEGAL_REGISTER, ", EDI", 0x0F, 0x23, 0347);
        ERRT("MOV", ", ESI", ILLEGAL_REGISTER, ", ESI", 0x0F, 0x23, 0346);
        TEST("MOV", "DR6, EDI", 0x0F, 0x23, 0367);
        TEST("MOV", "DR7, ESI", 0x0F, 0x23, 0376);
        ERRT("MOV", "DR3, EAX", ILLEGAL_OPERAND_MODE, "EAX", 0x0F, 0x23, 0030);
        ERRT("MOV", "DR6, ESP", ILLEGAL_OPERAND_MODE, "ESP", 0x0F, 0x23, 0164);
        ERRT("MOV", "DR7, EBP", ILLEGAL_OPERAND_MODE, "EBP", 0x0F, 0x23, 0275);

        // MOV r32,TRx
        ERRT("MOV", "EBP, ", ILLEGAL_REGISTER, "", 0x0F, 0x24, 0305);
        ERRT("MOV", "ESP, ", ILLEGAL_REGISTER, "", 0x0F, 0x24, 0314);
        ERRT("MOV", "EBX, ", ILLEGAL_REGISTER, "", 0x0F, 0x24, 0323);
        ERRT("MOV", "EDX, ", ILLEGAL_REGISTER, "", 0x0F, 0x24, 0332);
        ERRT("MOV", "ECX, ", ILLEGAL_REGISTER, "", 0x0F, 0x24, 0341);
        ERRT("MOV", "EAX, ", ILLEGAL_REGISTER, "", 0x0F, 0x24, 0350);
        TEST("MOV", "EDI, TR6", 0x0F, 0x24, 0367);
        TEST("MOV", "ESI, TR7", 0x0F, 0x24, 0376);
        ERRT("MOV", "EDX, TR6", ILLEGAL_OPERAND_MODE, "EDX, TR6", 0x0F, 0x24, 0062);
        ERRT("MOV", "EBX, TR6", ILLEGAL_OPERAND_MODE, "EBX, TR6", 0x0F, 0x24, 0163);
        ERRT("MOV", "ESP, TR6", ILLEGAL_OPERAND_MODE, "ESP, TR6", 0x0F, 0x24, 0264);
        // MOV TRx,r32
        ERRT("MOV", ", EBX", ILLEGAL_REGISTER, ", EBX", 0x0F, 0x26, 0303);
        ERRT("MOV", ", EDX", ILLEGAL_REGISTER, ", EDX", 0x0F, 0x26, 0312);
        ERRT("MOV", ", ECX", ILLEGAL_REGISTER, ", ECX", 0x0F, 0x26, 0321);
        ERRT("MOV", ", EAX", ILLEGAL_REGISTER, ", EAX", 0x0F, 0x26, 0330);
        ERRT("MOV", ", EDI", ILLEGAL_REGISTER, ", EDI", 0x0F, 0x26, 0347);
        ERRT("MOV", ", ESI", ILLEGAL_REGISTER, ", ESI", 0x0F, 0x26, 0356);
        TEST("MOV", "TR6, EDI", 0x0F, 0x26, 0367);
        TEST("MOV", "TR7, ESI", 0x0F, 0x26, 0376);
        ERRT("MOV", "TR7, EAX", ILLEGAL_OPERAND_MODE, "EAX", 0x0F, 0x26, 0070);
        ERRT("MOV", "TR7, ESP", ILLEGAL_OPERAND_MODE, "ESP", 0x0F, 0x26, 0174);
        ERRT("MOV", "TR7, EBP", ILLEGAL_OPERAND_MODE, "EBP", 0x0F, 0x26, 0275);
    }
}

void test_bit_manipulation() {
    // BT r/m16,r16
    TEST("BT",  "AX, CX",            0x0F, 0xA3, 0310);
    TEST("BT",  "[SI], DX",          0x0F, 0xA3, 0024);
    TEST("BT",  "[1234H], BX",       0x0F, 0xA3, 0036, 0x34, 0x12);
    TEST("BT",  "[BX+SI], SP",       0x0F, 0xA3, 0040);
    TEST("BT",  "[BX+DI-52], BP",    0x0F, 0xA3, 0151, 0xCC);
    TEST("BT",  "[DI+52], SI",       0x0F, 0xA3, 0165, 0x34);
    TEST("BT",  "[BP+SI+1234H], DI", 0x0F, 0xA3, 0272, 0x34, 0x12);
    TEST("BT",  "[BP+1234H], AX",    0x0F, 0xA3, 0206, 0x34, 0x12);
    // BT r/m32,r32
    TEST("BT",  "EAX, ECX",           DATA32, 0x0F, 0xA3, 0310);
    TEST("BT",  "[SI], EDX",          DATA32, 0x0F, 0xA3, 0024);
    TEST("BT",  "[1234H], EBX",       DATA32, 0x0F, 0xA3, 0036, 0x34, 0x12);
    TEST("BT",  "[BX+SI], ESP",       DATA32, 0x0F, 0xA3, 0040);
    TEST("BT",  "[BX+DI-52], EBP",    DATA32, 0x0F, 0xA3, 0151, 0xCC);
    TEST("BT",  "[DI+52], ESI",       DATA32, 0x0F, 0xA3, 0165, 0x34);
    TEST("BT",  "[BP+SI+1234H], EDI", DATA32, 0x0F, 0xA3, 0272, 0x34, 0x12);
    TEST("BT",  "[BP+1234H], EAX",    DATA32, 0x0F, 0xA3, 0206, 0x34, 0x12);

    // BTS r/m16,r16
    TEST("BTS", "BX, DX",            0x0F, 0xAB, 0323);
    TEST("BTS", "[DI], BX",          0x0F, 0xAB, 0035);
    TEST("BTS", "[1234H], SP",       0x0F, 0xAB, 0046, 0x34, 0x12);
    TEST("BTS", "[BP+SI], BP",       0x0F, 0xAB, 0052);
    TEST("BTS", "[BP+DI-52], SI",    0x0F, 0xAB, 0163, 0xCC);
    TEST("BTS", "[BX+52], DI",       0x0F, 0xAB, 0177, 0x34);
    TEST("BTS", "[SI+1234H], AX",    0x0F, 0xAB, 0204, 0x34, 0x12);
    TEST("BTS", "[BX+SI+1234H], CX", 0x0F, 0xAB, 0210, 0x34, 0x12);
    // BTS r/m32,r32
    TEST("BTS", "EBX, EDX",           DATA32, 0x0F, 0xAB, 0323);
    TEST("BTS", "[DI], EBX",          DATA32, 0x0F, 0xAB, 0035);
    TEST("BTS", "[1234H], ESP",       DATA32, 0x0F, 0xAB, 0046, 0x34, 0x12);
    TEST("BTS", "[BP+SI], EBP",       DATA32, 0x0F, 0xAB, 0052);
    TEST("BTS", "[BP+DI-52], ESI",    DATA32, 0x0F, 0xAB, 0163, 0xCC);
    TEST("BTS", "[BX+52], EDI",       DATA32, 0x0F, 0xAB, 0177, 0x34);
    TEST("BTS", "[SI+1234H], EAX",    DATA32, 0x0F, 0xAB, 0204, 0x34, 0x12);
    TEST("BTS", "[BX+SI+1234H], ECX", DATA32, 0x0F, 0xAB, 0210, 0x34, 0x12);

    // BTR r/m16,r16
    TEST("BTR", "DI, BX",            0x0F, 0xB3, 0337);
    TEST("BTR", "[BX], SP",          0x0F, 0xB3, 0047);
    TEST("BTR", "[1234H], BP",       0x0F, 0xB3, 0056, 0x34, 0x12);
    TEST("BTR", "[BP+DI], SI",       0x0F, 0xB3, 0063);
    TEST("BTR", "[SI+52], DI",       0x0F, 0xB3, 0174, 0x34);
    TEST("BTR", "[BX+SI+52], AX",    0x0F, 0xB3, 0100, 0x34);
    TEST("BTR", "[DI+1234H], CX",    0x0F, 0xB3, 0215, 0x34, 0x12);
    TEST("BTR", "[BX+DI+1234H], DX", 0x0F, 0xB3, 0221, 0x34, 0x12);
    // BTR r/m32,r32
    TEST("BTR", "EDI, EBX",           DATA32, 0x0F, 0xB3, 0337);
    TEST("BTR", "[BX], ESP",          DATA32, 0x0F, 0xB3, 0047);
    TEST("BTR", "[1234H], EBP",       DATA32, 0x0F, 0xB3, 0056, 0x34, 0x12);
    TEST("BTR", "[BP+DI], ESI",       DATA32, 0x0F, 0xB3, 0063);
    TEST("BTR", "[SI+52], EDI",       DATA32, 0x0F, 0xB3, 0174, 0x34);
    TEST("BTR", "[BX+SI+52], EAX",    DATA32, 0x0F, 0xB3, 0100, 0x34);
    TEST("BTR", "[DI+1234H], ECX",    DATA32, 0x0F, 0xB3, 0215, 0x34, 0x12);
    TEST("BTR", "[BX+DI+1234H], EDX", DATA32, 0x0F, 0xB3, 0221, 0x34, 0x12);

    // BTC r/m16,r16
    TEST("BTC", "SP, AX",            0x0F, 0xBB, 0340);
    TEST("BTC", "[BX+SI], AX",       0x0F, 0xBB, 0050);
    TEST("BTC", "[1234H], SI",       0x0F, 0xBB, 0066, 0x34, 0x12);
    TEST("BTC", "[SI], SP",          0x0F, 0xBB, 0074);
    TEST("BTC", "[DI+52], BP",       0x0F, 0xBB, 0105, 0x34);
    TEST("BTC", "[BX+DI+52], CX",    0x0F, 0xBB, 0111, 0x34);
    TEST("BTC", "[BP+1234H], SI",    0x0F, 0xBB, 0226, 0x34, 0x12);
    TEST("BTC", "[BP+SI+1234H], DX", 0x0F, 0xBB, 0232, 0x34, 0x12);
    // BTC r/m32,r32
    TEST("BTC", "ESP, EAX",           DATA32, 0x0F, 0xBB, 0340);
    TEST("BTC", "[BX+SI], EAX",       DATA32, 0x0F, 0xBB, 0050);
    TEST("BTC", "[1234H], ESI",       DATA32, 0x0F, 0xBB, 0066, 0x34, 0x12);
    TEST("BTC", "[SI], ESP",          DATA32, 0x0F, 0xBB, 0074);
    TEST("BTC", "[DI+52], EBP",       DATA32, 0x0F, 0xBB, 0105, 0x34);
    TEST("BTC", "[BX+DI+52], ECX",    DATA32, 0x0F, 0xBB, 0111, 0x34);
    TEST("BTC", "[BP+1234H], ESI",    DATA32, 0x0F, 0xBB, 0226, 0x34, 0x12);
    TEST("BTC", "[BP+SI+1234H], EDX", DATA32, 0x0F, 0xBB, 0232, 0x34, 0x12);

    // BT r/m16,imm8
    TEST("BT", "BP, 255",                   0x0F, 0xBA, 0345, 255);
    TEST("BT", "WORD [SI], 2",          0x0F, 0xBA, 0044, 2);
    TEST("BT", "WORD [1234H], 3",       0x0F, 0xBA, 0046, 0x34, 0x12, 3);
    TEST("BT", "WORD [DI-52], 4",       0x0F, 0xBA, 0145, 0xCC, 4);
    TEST("BT", "WORD [BP+1234H], 5",    0x0F, 0xBA, 0246, 0x34, 0x12, 5);
    TEST("BT", "WORD [BX+SI], 6",       0x0F, 0xBA, 0040, 6);
    TEST("BT", "WORD [BX+DI+52], 7",    0x0F, 0xBA, 0141, 0x34, 7);
    TEST("BT", "WORD [BP+SI+1234H], 8", 0x0F, 0xBA, 0242, 0x34, 0x12, 8);
    // BT r/m32,imm8
    TEST("BT", "EBP, 255",                   DATA32, 0x0F, 0xBA, 0345, 255);
    TEST("BT", "DWORD [SI], 2",          DATA32, 0x0F, 0xBA, 0044, 2);
    TEST("BT", "DWORD [1234H], 3",       DATA32, 0x0F, 0xBA, 0046, 0x34, 0x12, 3);
    TEST("BT", "DWORD [DI-52], 4",       DATA32, 0x0F, 0xBA, 0145, 0xCC, 4);
    TEST("BT", "DWORD [BP+1234H], 5",    DATA32, 0x0F, 0xBA, 0246, 0x34, 0x12, 5);
    TEST("BT", "DWORD [BX+SI], 6",       DATA32, 0x0F, 0xBA, 0040, 6);
    TEST("BT", "DWORD [BX+DI+52], 7",    DATA32, 0x0F, 0xBA, 0141, 0x34, 7);
    TEST("BT", "DWORD [BP+SI+1234H], 8", DATA32, 0x0F, 0xBA, 0242, 0x34, 0x12, 8);

    // BTS r/m16,imm8
    TEST("BTS", "SI, 9",                      0x0F, 0xBA, 0356, 9);
    TEST("BTS", "WORD [DI], 10",          0x0F, 0xBA, 0055, 10);
    TEST("BTS", "WORD [1234H], 11",       0x0F, 0xBA, 0056, 0x34, 0x12, 11);
    TEST("BTS", "WORD [BP-52], 12",       0x0F, 0xBA, 0156, 0xCC, 12);
    TEST("BTS", "WORD [BX+1234H], 13",    0x0F, 0xBA, 0257, 0x34, 0x12, 13);
    TEST("BTS", "WORD [BX+DI], 14",       0x0F, 0xBA, 0051, 14);
    TEST("BTS", "WORD [BP+SI+52], 15",    0x0F, 0xBA, 0152, 0x34, 15);
    TEST("BTS", "WORD [BP+DI+1234H], 16", 0x0F, 0xBA, 0253, 0x34, 0x12, 16);
    // BTS r/m32,imm8
    TEST("BTS", "ESI, 9",                      DATA32, 0x0F, 0xBA, 0356, 9);
    TEST("BTS", "DWORD [DI], 10",          DATA32, 0x0F, 0xBA, 0055, 10);
    TEST("BTS", "DWORD [1234H], 11",       DATA32, 0x0F, 0xBA, 0056, 0x34, 0x12, 11);
    TEST("BTS", "DWORD [BP-52], 12",       DATA32, 0x0F, 0xBA, 0156, 0xCC, 12);
    TEST("BTS", "DWORD [BX+1234H], 13",    DATA32, 0x0F, 0xBA, 0257, 0x34, 0x12, 13);
    TEST("BTS", "DWORD [BX+DI], 14",       DATA32, 0x0F, 0xBA, 0051, 14);
    TEST("BTS", "DWORD [BP+SI+52], 15",    DATA32, 0x0F, 0xBA, 0152, 0x34, 15);
    TEST("BTS", "DWORD [BP+DI+1234H], 16", DATA32, 0x0F, 0xBA, 0253, 0x34, 0x12, 16);

    // BTR r/m16,imm8
    TEST("BTR", "DI, 17",                     0x0F, 0xBA, 0367, 17);
    TEST("BTR", "WORD [BP+DI], 18",       0x0F, 0xBA, 0063, 18);
    TEST("BTR", "WORD [1234H], 19",       0x0F, 0xBA, 0066, 0x34, 0x12, 19);
    TEST("BTR", "WORD [BX-52], 20",       0x0F, 0xBA, 0167, 0xCC, 20);
    TEST("BTR", "WORD [BX+SI+1234H], 21", 0x0F, 0xBA, 0260, 0x34, 0x12, 21);
    TEST("BTR", "WORD [BP+SI], 22",       0x0F, 0xBA, 0062, 22);
    TEST("BTR", "WORD [BP+DI+52], 23",    0x0F, 0xBA, 0163, 0x34, 23);
    TEST("BTR", "WORD [SI+1234H], 24",    0x0F, 0xBA, 0264, 0x34, 0x12, 24);
    // BTR r/m32,imm8
    TEST("BTR", "EDI, 17",                     DATA32, 0x0F, 0xBA, 0367, 17);
    TEST("BTR", "DWORD [BP+DI], 18",       DATA32, 0x0F, 0xBA, 0063, 18);
    TEST("BTR", "DWORD [1234H], 19",       DATA32, 0x0F, 0xBA, 0066, 0x34, 0x12, 19);
    TEST("BTR", "DWORD [BX-52], 20",       DATA32, 0x0F, 0xBA, 0167, 0xCC, 20);
    TEST("BTR", "DWORD [BX+SI+1234H], 21", DATA32, 0x0F, 0xBA, 0260, 0x34, 0x12, 21);
    TEST("BTR", "DWORD [BP+SI], 22",       DATA32, 0x0F, 0xBA, 0062, 22);
    TEST("BTR", "DWORD [BP+DI+52], 23",    DATA32, 0x0F, 0xBA, 0163, 0x34, 23);
    TEST("BTR", "DWORD [SI+1234H], 24",    DATA32, 0x0F, 0xBA, 0264, 0x34, 0x12, 24);

    // BTC r/m16,imm8
    TEST("BTC", "AX, 25",                     0x0F, 0xBA, 0370, 25);
    TEST("BTC", "WORD [BX], 26",          0x0F, 0xBA, 0077, 26);
    TEST("BTC", "WORD [1234H], 27",       0x0F, 0xBA, 0076, 0x34, 0x12, 27);
    TEST("BTC", "WORD [BX+SI-52], 28",    0x0F, 0xBA, 0170, 0xCC, 28);
    TEST("BTC", "WORD [BX+DI+1234H], 29", 0x0F, 0xBA, 0271, 0x34, 0x12, 29);
    TEST("BTC", "WORD [BP+DI], 30",       0x0F, 0xBA, 0073, 30);
    TEST("BTC", "WORD [SI+52], 31",       0x0F, 0xBA, 0174, 0x34, 31);
    TEST("BTC", "WORD [DI+1234H], 0",     0x0F, 0xBA, 0275, 0x34, 0x12, 0);
    // BTC r/m32,imm8
    TEST("BTC", "EAX, 25",                     DATA32, 0x0F, 0xBA, 0370, 25);
    TEST("BTC", "DWORD [BX], 26",          DATA32, 0x0F, 0xBA, 0077, 26);
    TEST("BTC", "DWORD [1234H], 27",       DATA32, 0x0F, 0xBA, 0076, 0x34, 0x12, 27);
    TEST("BTC", "DWORD [BX+SI-52], 28",    DATA32, 0x0F, 0xBA, 0170, 0xCC, 28);
    TEST("BTC", "DWORD [BX+DI+1234H], 29", DATA32, 0x0F, 0xBA, 0271, 0x34, 0x12, 29);
    TEST("BTC", "DWORD [BP+DI], 30",       DATA32, 0x0F, 0xBA, 0073, 30);
    TEST("BTC", "DWORD [SI+52], 31",       DATA32, 0x0F, 0xBA, 0174, 0x34, 31);
    TEST("BTC", "DWORD [DI+1234H], 0",     DATA32, 0x0F, 0xBA, 0275, 0x34, 0x12, 0);

    // BSF r16,r/m16
    TEST("BSF", "CX, AX",            0x0F, 0xBC, 0310);
    TEST("BSF", "DX, [SI]",          0x0F, 0xBC, 0024);
    TEST("BSF", "BX, [1234H]",       0x0F, 0xBC, 0036, 0x34, 0x12);
    TEST("BSF", "SP, [BX+SI]",       0x0F, 0xBC, 0040);
    TEST("BSF", "BP, [BX+DI-52]",    0x0F, 0xBC, 0151, 0xCC);
    TEST("BSF", "SI, [DI+52]",       0x0F, 0xBC, 0165, 0x34);
    TEST("BSF", "DI, [BP+SI+1234H]", 0x0F, 0xBC, 0272, 0x34, 0x12);
    TEST("BSF", "AX, [BP+1234H]",    0x0F, 0xBC, 0206, 0x34, 0x12);
    // BSF r32,r/m32
    TEST("BSF", "ECX, EAX",           DATA32, 0x0F, 0xBC, 0310);
    TEST("BSF", "EDX, [SI]",          DATA32, 0x0F, 0xBC, 0024);
    TEST("BSF", "EBX, [1234H]",       DATA32, 0x0F, 0xBC, 0036, 0x34, 0x12);
    TEST("BSF", "ESP, [BX+SI]",       DATA32, 0x0F, 0xBC, 0040);
    TEST("BSF", "EBP, [BX+DI-52]",    DATA32, 0x0F, 0xBC, 0151, 0xCC);
    TEST("BSF", "ESI, [DI+52]",       DATA32, 0x0F, 0xBC, 0165, 0x34);
    TEST("BSF", "EDI, [BP+SI+1234H]", DATA32, 0x0F, 0xBC, 0272, 0x34, 0x12);
    TEST("BSF", "EAX, [BP+1234H]",    DATA32, 0x0F, 0xBC, 0206, 0x34, 0x12);

    // BSR  r16,r/m16
    TEST("BSR", "CX, AX",            0x0F, 0xBD, 0310);
    TEST("BSR", "DX, [SI]",          0x0F, 0xBD, 0024);
    TEST("BSR", "BX, [1234H]",       0x0F, 0xBD, 0036, 0x34, 0x12);
    TEST("BSR", "SP, [BX+SI]",       0x0F, 0xBD, 0040);
    TEST("BSR", "BP, [BX+DI-52]",    0x0F, 0xBD, 0151, 0xCC);
    TEST("BSR", "SI, [DI+52]",       0x0F, 0xBD, 0165, 0x34);
    TEST("BSR", "DI, [BP+SI+1234H]", 0x0F, 0xBD, 0272, 0x34, 0x12);
    TEST("BSR", "AX, [BP+1234H]",    0x0F, 0xBD, 0206, 0x34, 0x12);
    // BSR r32,r/m32
    TEST("BSR", "ECX, EAX",           DATA32, 0x0F, 0xBD, 0310);
    TEST("BSR", "EDX, [SI]",          DATA32, 0x0F, 0xBD, 0024);
    TEST("BSR", "EBX, [1234H]",       DATA32, 0x0F, 0xBD, 0036, 0x34, 0x12);
    TEST("BSR", "ESP, [BX+SI]",       DATA32, 0x0F, 0xBD, 0040);
    TEST("BSR", "EBP, [BX+DI-52]",    DATA32, 0x0F, 0xBD, 0151, 0xCC);
    TEST("BSR", "ESI, [DI+52]",       DATA32, 0x0F, 0xBD, 0165, 0x34);
    TEST("BSR", "EDI, [BP+SI+1234H]", DATA32, 0x0F, 0xBD, 0272, 0x34, 0x12);
    TEST("BSR", "EAX, [BP+1234H]",    DATA32, 0x0F, 0xBD, 0206, 0x34, 0x12);
}

void test_segment_override() {
    disassembler.setOption("segment-insn", "enable");

    TEST("SEGES", "", SEGES);
    TEST("SEGCS", "", SEGCS);
    TEST("SEGSS", "", SEGSS);
    TEST("SEGDS", "", SEGDS);
    if (hasExtraSeg()) {
        TEST("SEGFS", "", SEGFS);
        TEST("SEGGS", "", SEGGS);
    }

    disassembler.setOption("segment-insn", "disable");

    NMEM("", "", "", SEGES);
    NMEM("", "", "", SEGCS);
    NMEM("", "", "", SEGSS);
    NMEM("", "", "", SEGDS);
    EPRE("", "", SEGES, SEGCS);
    EPRE("", "", SEGDS, REP, SEGSS);
    if (hasExtraSeg()) {
        NMEM("", "", "", SEGFS);
        NMEM("", "", "", SEGGS);
        EPRE("", "", SEGES, SEGFS);
        EPRE("", "", SEGGS, REP, SEGSS);
    }

    TEST("MOV", "ES:[BX], AH",    SEGES, 0x88, 0047);
    TEST("MOV", "ES:[BP+0], AH",  SEGES, 0x88, 0146, 0x00);
    TEST("MOV", "ES:[SI], AH",    SEGES, 0x88, 0044);
    TEST("MOV", "ES:[DI], AH",    SEGES, 0x88, 0045);
    TEST("MOV", "ES:[1234H], AH", SEGES, 0x88, 0046, 0x34, 0x12);
    TEST("MOV", "CS:[BX], AH",    SEGCS, 0x88, 0047);
    TEST("MOV", "CS:[BP+0], AH",  SEGCS, 0x88, 0146, 0x00);
    TEST("MOV", "CS:[SI], AH",    SEGCS, 0x88, 0044);
    TEST("MOV", "CS:[DI], AH",    SEGCS, 0x88, 0045);
    TEST("MOV", "CS:[1234H], AH", SEGCS, 0x88, 0046, 0x34, 0x12);
    TEST("MOV", "SS:[BX], AH",    SEGSS, 0x88, 0047);
    TEST("MOV", "SS:[BP+0], AH",  SEGSS, 0x88, 0146, 0x00);
    TEST("MOV", "SS:[SI], AH",    SEGSS, 0x88, 0044);
    TEST("MOV", "SS:[DI], AH",    SEGSS, 0x88, 0045);
    TEST("MOV", "SS:[1234H], AH", SEGSS, 0x88, 0046, 0x34, 0x12);
    TEST("MOV", "DS:[BX], AH",    SEGDS, 0x88, 0047);
    TEST("MOV", "DS:[BP+0], AH",  SEGDS, 0x88, 0146, 0x00);
    TEST("MOV", "DS:[SI], AH",    SEGDS, 0x88, 0044);
    TEST("MOV", "DS:[DI], AH",    SEGDS, 0x88, 0045);
    TEST("MOV", "DS:[1234H], AH", SEGDS, 0x88, 0046, 0x34, 0x12);

    if (is80x86()) {
        TEST("BOUND", "SI, ES:[SI]",       SEGES, 0x62, 0064);
        TEST("BOUND", "CX, CS:[1234H]",    SEGCS, 0x62, 0016, 0x34, 0x12);
        TEST("BOUND", "DI, CS:[1234H]",    SEGCS, 0x62, 0076, 0x34, 0x12);
        TEST("BOUND", "AX, SS:[DI+52]",    SEGSS, 0x62, 0105, 0x34);
        TEST("BOUND", "CX, DS:[BP+1234H]", SEGDS, 0x62, 0216, 0x34, 0x12);
    }

    if (is16bit()) {
        // jmp r/m16
        TEST("JMP", "ES:[SI]", SEGES, 0xFF, 0044);
        TEST("JMP", "CS:[SI]", SEGCS, 0xFF, 0044);
        TEST("JMP", "SS:[SI]", SEGSS, 0xFF, 0044);
        TEST("JMP", "DS:[SI]", SEGDS, 0xFF, 0044);
        // jmp m16:16
        TEST("JMP", "FAR ES:[SI]",          SEGES, 0xFF, 0054);
        TEST("JMP", "FAR CS:[1234H]",       SEGCS, 0xFF, 0056, 0x34, 0x12);
        TEST("JMP", "FAR SS:[DI-52]",       SEGSS, 0xFF, 0155, 0xCC);
        TEST("JMP", "FAR DS:[BP+1234H]",    SEGDS, 0xFF, 0256, 0x34, 0x12);
        // JMP m16:16
        disassembler.setOption("far-insn", "on");
        TEST("JMPF", "ES:[SI]",          SEGES, 0xFF, 0054);
        TEST("JMPF", "CS:[1234H]",       SEGCS, 0xFF, 0056, 0x34, 0x12);
        TEST("JMPF", "SS:[DI-52]",       SEGSS, 0xFF, 0155, 0xCC);
        TEST("JMPF", "DS:[BP+1234H]",    SEGDS, 0xFF, 0256, 0x34, 0x12);
        disassembler.setOption("far-insn", "off");
    } else {
        // JMP r/m16
        TEST("JMP", "WORD ES:[SI]", SEGES, 0xFF, 0044);
        TEST("JMP", "WORD CS:[SI]", SEGCS, 0xFF, 0044);
        TEST("JMP", "WORD SS:[SI]", SEGSS, 0xFF, 0044);
        TEST("JMP", "WORD DS:[SI]", SEGDS, 0xFF, 0044);
        // JMP m16:16
        TEST("JMP", "FAR WORD ES:[SI]",          SEGES, 0xFF, 0054);
        TEST("JMP", "FAR WORD CS:[1234H]",       SEGCS, 0xFF, 0056, 0x34, 0x12);
        TEST("JMP", "FAR WORD SS:[DI-52]",       SEGSS, 0xFF, 0155, 0xCC);
        TEST("JMP", "FAR WORD DS:[BP+1234H]",    SEGDS, 0xFF, 0256, 0x34, 0x12);
        TEST("JMP", "FAR WORD FS:[BX+SI]",       SEGFS, 0xFF, 0050);
        TEST("JMP", "FAR WORD GS:[BX+DI+52]",    SEGGS, 0xFF, 0151, 0x34);
        // JMP m16:32
        TEST("JMP", "FAR DWORD ES:[SI]",          SEGES, DATA32, 0xFF, 0054);
        TEST("JMP", "FAR DWORD CS:[1234H]",       DATA32, SEGCS, 0xFF, 0056, 0x34, 0x12);
        TEST("JMP", "FAR DWORD SS:[DI-52]",       DATA32, SEGSS, 0xFF, 0155, 0xCC);
        TEST("JMP", "FAR DWORD DS:[BP+1234H]",    DATA32, SEGDS, 0xFF, 0256, 0x34, 0x12);
        TEST("JMP", "FAR DWORD FS:[BX+SI]",       SEGFS, DATA32, 0xFF, 0050);
        TEST("JMP", "FAR DWORD GS:[BX+DI+52]",    DATA32, SEGGS, 0xFF, 0151, 0x34);
     }

    ERRT("LEA", "CX, CS:[SI]",
         OPCODE_HAS_NO_EFFECT, "CS:[SI]",          SEGCS, 0x8D, 0014);
    ERRT("LEA", "DX, ES:[1235H]",
         OPCODE_HAS_NO_EFFECT, "ES:[1235H]",       SEGES, 0x8D, 0026, 0x35, 0x12);
    ERRT("LEA", "BX, ES:[DI-52]",
         OPCODE_HAS_NO_EFFECT, "ES:[DI-52]",       SEGES, 0x8D, 0135, 0xCC);
    ERRT("LEA", "SP, DS:[BP+1234H]",
         OPCODE_HAS_NO_EFFECT, "DS:[BP+1234H]",    SEGDS, 0x8D, 0246, 0x34, 0x12);
    ERRT("LEA", "BP, SS:[BX+SI]",
         OPCODE_HAS_NO_EFFECT, "SS:[BX+SI]",       SEGSS, 0x8D, 0050);
    ERRT("LEA", "SI, DS:[BX+DI+52]",
         OPCODE_HAS_NO_EFFECT, "DS:[BX+DI+52]",    SEGDS, 0x8D, 0161, 0x34);
    ERRT("LEA", "DI, CS:[BP+SI+1234H]",
         OPCODE_HAS_NO_EFFECT, "CS:[BP+SI+1234H]", SEGCS, 0x8D, 0272, 0x34, 0x12);

    gnu_as(true);

    TEST("movsb", "es:[di], es:[si]", SEGES, 0xa4);
    TEST("movsb", "es:[di], cs:[si]", SEGCS, 0xa4);
    TEST("movsw", "es:[di], ss:[si]", SEGSS, 0xa5);
    TEST("movsw", "es:[di], ds:[si]", SEGDS, 0xa5);
    TEST("cmpsb", "es:[si], es:[di]", SEGES, 0xa6);
    TEST("cmpsb", "cs:[si], es:[di]", SEGCS, 0xa6);
    TEST("cmpsw", "ss:[si], es:[di]", SEGSS, 0xa7);
    TEST("cmpsw", "ds:[si], es:[di]", SEGDS, 0xa7);
    TEST("lodsb", "es:[si]", SEGES, 0xac);
    TEST("lodsb", "cs:[si]", SEGCS, 0xac);
    TEST("lodsw", "ss:[si]", SEGSS, 0xad);
    TEST("lodsw", "ds:[si]", SEGDS, 0xad);

    ERRT("stosb", "cs:[di]", ILLEGAL_SEGMENT, "cs:[di]", SEGCS, 0xaa);
    ERRT("stosw", "ss:[di]", ILLEGAL_SEGMENT, "ss:[di]", SEGSS, 0xab);
    ERRT("stosw", "ds:[di]", ILLEGAL_SEGMENT, "ds:[di]", SEGDS, 0xab);
    ERRT("scasb", "es:[di]", ILLEGAL_SEGMENT, "es:[di]", SEGES, 0xae);
    ERRT("scasb", "cs:[di]", ILLEGAL_SEGMENT, "cs:[di]", SEGCS, 0xae);
    ERRT("scasw", "ss:[di]", ILLEGAL_SEGMENT, "ss:[di]", SEGSS, 0xaf);
    ERRT("scasw", "ds:[di]", ILLEGAL_SEGMENT, "ds:[di]", SEGDS, 0xaf);

    if (is80x86()) {
        TEST("outsb", "dx, es:[si]", SEGES, 0x6e);
        TEST("outsb", "dx, cs:[si]", SEGCS, 0x6e);
        TEST("outsw", "dx, ss:[si]", SEGSS, 0x6f);
        TEST("outsw", "dx, ds:[si]", SEGDS, 0x6f);
        ERRT("insb",  "es:[di], dx", ILLEGAL_SEGMENT, "es:[di], dx", SEGES, 0x6c);
        ERRT("insb",  "cs:[di], dx", ILLEGAL_SEGMENT, "cs:[di], dx", SEGCS, 0x6c);
        ERRT("insw",  "ss:[di], dx", ILLEGAL_SEGMENT, "ss:[di], dx", SEGSS, 0x6d);
        ERRT("insw",  "ds:[di], dx", ILLEGAL_SEGMENT, "ds:[di], dx", SEGDS, 0x6d);
    }

    if (v30()) {
        TEST("add4s", "es:[di], es:[si]", SEGES, 0x0f, 0x20);
        TEST("cmp4s", "es:[di], cs:[si]", SEGCS, 0x0f, 0x26);
        TEST("sub4s", "es:[di], ss:[si]", SEGSS, 0x0f, 0x22);
    }

    gnu_as(false);

    ERRT("MOVSB", "ES:[DI], ES:[SI]", ILLEGAL_SEGMENT, "ES:[DI], ES:[SI]", SEGES, 0xA4);
    ERRT("MOVSB", "ES:[DI], CS:[SI]", ILLEGAL_SEGMENT, "ES:[DI], CS:[SI]", SEGCS, 0xA4);
    ERRT("MOVSW", "ES:[DI], SS:[SI]", ILLEGAL_SEGMENT, "ES:[DI], SS:[SI]", SEGSS, 0xA5);
    ERRT("MOVSW", "ES:[DI], DS:[SI]", ILLEGAL_SEGMENT, "ES:[DI], DS:[SI]", SEGDS, 0xA5);
    ERRT("CMPSB", "ES:[SI], ES:[DI]", ILLEGAL_SEGMENT, "ES:[SI], ES:[DI]", SEGES, 0xA6);
    ERRT("CMPSB", "CS:[SI], ES:[DI]", ILLEGAL_SEGMENT, "CS:[SI], ES:[DI]", SEGCS, 0xA6);
    ERRT("CMPSW", "SS:[SI], ES:[DI]", ILLEGAL_SEGMENT, "SS:[SI], ES:[DI]", SEGSS, 0xA7);
    ERRT("CMPSW", "DS:[SI], ES:[DI]", ILLEGAL_SEGMENT, "DS:[SI], ES:[DI]", SEGDS, 0xA7);
    ERRT("LODSB", "ES:[SI]", ILLEGAL_SEGMENT, "ES:[SI]", SEGES, 0xAC);
    ERRT("LODSB", "CS:[SI]", ILLEGAL_SEGMENT, "CS:[SI]", SEGCS, 0xAC);
    ERRT("LODSW", "SS:[SI]", ILLEGAL_SEGMENT, "SS:[SI]", SEGSS, 0xAD);
    ERRT("LODSW", "DS:[SI]", ILLEGAL_SEGMENT, "DS:[SI]", SEGDS, 0xAD);

    ERRT("STOSB", "CS:[DI]", ILLEGAL_SEGMENT, "CS:[DI]", SEGCS, 0xAA);
    ERRT("STOSW", "SS:[DI]", ILLEGAL_SEGMENT, "SS:[DI]", SEGSS, 0xAB);
    ERRT("STOSW", "DS:[DI]", ILLEGAL_SEGMENT, "DS:[DI]", SEGDS, 0xAB);
    ERRT("SCASB", "ES:[DI]", ILLEGAL_SEGMENT, "ES:[DI]", SEGES, 0xAE);
    ERRT("SCASB", "CS:[DI]", ILLEGAL_SEGMENT, "CS:[DI]", SEGCS, 0xAE);
    ERRT("SCASW", "SS:[DI]", ILLEGAL_SEGMENT, "SS:[DI]", SEGSS, 0xAF);
    ERRT("SCASW", "DS:[DI]", ILLEGAL_SEGMENT, "DS:[DI]", SEGDS, 0xAF);

    if (is80x86()) {
        ERRT("OUTSB", "DX, ES:[SI]", ILLEGAL_SEGMENT, "DX, ES:[SI]", SEGES, 0x6E);
        ERRT("OUTSB", "DX, CS:[SI]", ILLEGAL_SEGMENT, "DX, CS:[SI]", SEGCS, 0x6E);
        ERRT("OUTSW", "DX, SS:[SI]", ILLEGAL_SEGMENT, "DX, SS:[SI]", SEGSS, 0x6F);
        ERRT("OUTSW", "DX, DS:[SI]", ILLEGAL_SEGMENT, "DX, DS:[SI]", SEGDS, 0x6F);
        ERRT("INSB",  "ES:[DI], DX", ILLEGAL_SEGMENT, "ES:[DI], DX", SEGES, 0x6C);
        ERRT("INSB",  "CS:[DI], DX", ILLEGAL_SEGMENT, "CS:[DI], DX", SEGCS, 0x6C);
        ERRT("INSW",  "SS:[DI], DX", ILLEGAL_SEGMENT, "SS:[DI], DX", SEGSS, 0x6D);
        ERRT("INSW",  "DS:[DI], DX", ILLEGAL_SEGMENT, "DS:[DI], DX", SEGDS, 0x6D);
    }

    if (v30()) {
        ERRT("ADD4S", "ES:[DI], ES:[SI]", ILLEGAL_SEGMENT, "ES:[DI], ES:[SI]", SEGES, 0x0F, 0x20);
        ERRT("CMP4S", "ES:[DI], CS:[SI]", ILLEGAL_SEGMENT, "ES:[DI], CS:[SI]", SEGCS, 0x0F, 0x26);
        ERRT("SUB4S", "ES:[DI], SS:[SI]", ILLEGAL_SEGMENT, "ES:[DI], SS:[SI]", SEGSS, 0x0F, 0x22);
    }

#if !defined(LIBASM_I8086_NOFPU)
    if (!fpu_on())
        return;

    NMEM("", "", "", FWAIT, SEGES);
    NMEM("", "", "", FWAIT, REP, SEGCS);
    NMEM("", "", "", FWAIT, SEGSS, REP);
    UNKN(            FWAIT, FWAIT);
    ERRT("WAIT", "", ILLEGAL_SEGMENT, "", SEGDS, FWAIT);

    TEST("FLDCW", "SS:[SI]",          SEGSS, 0xD9, 0054);
    TEST("FLDCW", "CS:[1234H]",       SEGCS, 0xD9, 0056, 0x34, 0x12);
    TEST("FLDCW", "ES:[DI+52]",       SEGES, 0xD9, 0155, 0x34);
    TEST("FLDCW", "ES:[BP+1234H]",    SEGES, 0xD9, 0256, 0x34, 0x12);
    TEST("FLDCW", "ES:[BX+SI]",       SEGES, 0xD9, 0050);
    TEST("FLDCW", "CS:[BX+DI-52]",    SEGCS, 0xD9, 0151, 0xCC);
    TEST("FLDCW", "ES:[BP+SI+89ABH]", SEGES, 0xD9, 0252, 0xAB, 0x89);

    TEST("FSTCW", "ES:[SI]",          FWAIT, SEGES, 0xD9, 0074);
    TEST("FSTCW", "ES:[1234H]",       FWAIT, SEGES, 0xD9, 0076, 0x34, 0x12);
    TEST("FSTCW", "SS:[DI+52]",       FWAIT, SEGSS, 0xD9, 0175, 0x34);
    TEST("FSTCW", "DS:[BP+1234H]",    FWAIT, SEGDS, 0xD9, 0276, 0x34, 0x12);
    TEST("FSTCW", "SS:[BX+SI]",       FWAIT, SEGSS, 0xD9, 0070);
    TEST("FSTCW", "ES:[BX+DI-52]",    FWAIT, SEGES, 0xD9, 0171, 0xCC);
    TEST("FSTCW", "DS:[BP+SI+89ABH]", FWAIT, SEGDS, 0xD9, 0272, 0xAB, 0x89);
#endif
}

void test_size_prefix() {
    EPRE("", "", DATA32, DATA32);
    EPRE("", "", ADDR32, ADDR32);
    EPRE("", "", DATA32, ADDR32, DATA32);
    EPRE("", "", ADDR32, DATA32, ADDR32);

    use16();
    TEST("MOV", "AL, CL",           0x88, 0310);
    EPRE("MOV", "AL, CL",   DATA32, 0x88, 0310);
    EPRE("MOV", "AL, CL",   ADDR32, 0x88, 0310);
    TEST("MOV", "AX, CX",           0x89, 0310);
    TEST("MOV", "EAX, ECX", DATA32, 0x89, 0310);
    EPRE("MOV", "AX, CX",   ADDR32, 0x89, 0310);

    use32();
    TEST("MOV", "AL, CL",           0x88, 0310);
    EPRE("MOV", "AL, CL",   DATA32, 0x88, 0310);
    EPRE("MOV", "AL, CL",   ADDR32, 0x88, 0310);
    TEST("MOV", "EAX, ECX",         0x89, 0310);
    TEST("MOV", "AX, CX",   DATA32, 0x89, 0310);
    EPRE("MOV", "EAX, ECX", ADDR32, 0x89, 0310);
}

void test_32bit_addressing() {
    gnu_as(false);
    TEST("XOR", "AX, [EAX]",                 ADDR32, 0x33, 0000);
    TEST("XOR", "AX, [ECX]",                 ADDR32, 0x33, 0001);
    TEST("XOR", "AX, [EDX]",                 ADDR32, 0x33, 0002);
    TEST("XOR", "AX, [EBX]",                 ADDR32, 0x33, 0003);
    TEST("XOR", "AX, [ECX+EAX]",             ADDR32, 0x33, 0004, 0001);
    TEST("XOR", "AX, [EDX+ECX*2]",           ADDR32, 0x33, 0004, 0112);
    TEST("XOR", "AX, [EBX+EDX*4]",           ADDR32, 0x33, 0004, 0223);
    TEST("XOR", "AX, [ESP+EBX*8]",           ADDR32, 0x33, 0004, 0334);
    TEST("XOR", "AX, [EDX+ECX]",             ADDR32, 0x33, 0004, 0012);
    TEST("XOR", "AX, [EBX+12345678H]",       ADDR32, 0x33, 0004, 0035, 0x78, 0x56, 0x34, 0x12);
    TEST("XOR", "AX, [EBP+EBX+56]",          ADDR32, 0x33, 0104, 0035, 56);
    TEST("XOR", "AX, [EBP+EBX+12345678H]",   ADDR32, 0x33, 0204, 0035, 0x78, 0x56, 0x34, 0x12);
    TEST("XOR", "AX, [12345678H]",           ADDR32, 0x33, 0004, 0045, 0x78, 0x56, 0x34, 0x12);
    TEST("XOR", "AX, [EBP+56]",              ADDR32, 0x33, 0104, 0045, 56);
    TEST("XOR", "AX, [EBP+12345678H]",       ADDR32, 0x33, 0204, 0045, 0x78, 0x56, 0x34, 0x12);
    TEST("XOR", "AX, [ESI]",                 ADDR32, 0x33, 0004, 0046);
    TEST("XOR", "AX, [ESI+56]",              ADDR32, 0x33, 0104, 0046, 56);
    TEST("XOR", "AX, [ESI+12345678H]",       ADDR32, 0x33, 0204, 0046, 0x78, 0x56, 0x34, 0x12);
    TEST("XOR", "AX, [EDX+ECX*2]",           ADDR32, 0x33, 0004, 0112);
    TEST("XOR", "AX, [EBX*2+12345678H]",     ADDR32, 0x33, 0004, 0135, 0x78, 0x56, 0x34, 0x12);
    TEST("XOR", "AX, [EBP+EBX*2+56]",        ADDR32, 0x33, 0104, 0135, 56);
    TEST("XOR", "AX, [EBP+EBX*2+12345678H]", ADDR32, 0x33, 0204, 0135, 0x78, 0x56, 0x34, 0x12);
    TEST("XOR", "AX, [12345678H]",           ADDR32, 0x33, 0004, 0145, 0x78, 0x56, 0x34, 0x12);
    TEST("XOR", "AX, [EBP+0]",               ADDR32, 0x33, 0104, 0145, 0);
    TEST("XOR", "AX, [EBP+12345678H]",       ADDR32, 0x33, 0204, 0145, 0x78, 0x56, 0x34, 0x12);
    TEST("XOR", "AX, [ESI]",                 ADDR32, 0x33, 0004, 0146);
    TEST("XOR", "AX, [ESI+0]",               ADDR32, 0x33, 0104, 0146, 0);
    TEST("XOR", "AX, [ESI+12345678H]",       ADDR32, 0x33, 0204, 0146, 0x78, 0x56, 0x34, 0x12);
    TEST("XOR", "AX, [EDX+ECX*4]",           ADDR32, 0x33, 0004, 0212);
    TEST("XOR", "AX, [EBX*4+12345678H]",     ADDR32, 0x33, 0004, 0235, 0x78, 0x56, 0x34, 0x12);
    TEST("XOR", "AX, [12345678H]",           ADDR32, 0x33, 0004, 0245, 0x78, 0x56, 0x34, 0x12);
    TEST("XOR", "AX, [ESI]",                 ADDR32, 0x33, 0004, 0246);
    TEST("XOR", "AX, [EDX+ECX*8]",           ADDR32, 0x33, 0004, 0312);
    TEST("XOR", "AX, [EBX*8+12345678H]",     ADDR32, 0x33, 0004, 0335, 0x78, 0x56, 0x34, 0x12);
    TEST("XOR", "AX, [12345678H]",           ADDR32, 0x33, 0004, 0345, 0x78, 0x56, 0x34, 0x12);
    TEST("XOR", "AX, [ESI]",                 ADDR32, 0x33, 0004, 0346);
    TEST("XOR", "AX, [12345678H]",           ADDR32, 0x33, 0005, 0x78, 0x56, 0x34, 0x12);
    TEST("XOR", "AX, [ESI]",                 ADDR32, 0x33, 0006);
    TEST("XOR", "AX, [EDI]",                 ADDR32, 0x33, 0007);

    TEST("XOR", "AX, [EAX+52]",       ADDR32, 0x33, 0100, 52);
    TEST("XOR", "AX, [ECX+52]",       ADDR32, 0x33, 0101, 52);
    TEST("XOR", "AX, [EDX+52]",       ADDR32, 0x33, 0102, 52);
    TEST("XOR", "AX, [EBX+52]",       ADDR32, 0x33, 0103, 52);
    TEST("XOR", "AX, [EDI+ECX+52]",   ADDR32, 0x33, 0104, 0017, 52);
    TEST("XOR", "AX, [ESI+EDX*2+52]", ADDR32, 0x33, 0104, 0126, 52);
    TEST("XOR", "AX, [EBP+EAX+52]",   ADDR32, 0x33, 0104, 0005, 52);
    TEST("XOR", "AX, [EBP+EAX*2+52]", ADDR32, 0x33, 0104, 0105, 52);
    TEST("XOR", "AX, [EBP+EAX*4+52]", ADDR32, 0x33, 0104, 0205, 52);
    TEST("XOR", "AX, [EBP+EAX*8+52]", ADDR32, 0x33, 0104, 0305, 52);
    TEST("XOR", "AX, [ECX+EAX*4+52]", ADDR32, 0x33, 0104, 0201, 52);
    TEST("XOR", "AX, [ECX+EAX*8+52]", ADDR32, 0x33, 0104, 0301, 52);
    TEST("XOR", "AX, [EDX+EAX*8+52]", ADDR32, 0x33, 0104, 0302, 52);
    TEST("XOR", "AX, [EBX+EAX*8+52]", ADDR32, 0x33, 0104, 0303, 52);
    TEST("XOR", "AX, [ESP+EAX*8+52]", ADDR32, 0x33, 0104, 0304, 52);
    TEST("XOR", "AX, [EBP+52]",       ADDR32, 0x33, 0105, 52);
    TEST("XOR", "AX, [ESI+52]",       ADDR32, 0x33, 0106, 52);
    TEST("XOR", "AX, [EDI+52]",       ADDR32, 0x33, 0107, 52);

    TEST("XOR", "AX, [EAX+12345678H]",       ADDR32, 0x33, 0200, 0x78, 0x56, 0x34, 0x12);
    TEST("XOR", "AX, [ECX+12345678H]",       ADDR32, 0x33, 0201, 0x78, 0x56, 0x34, 0x12);
    TEST("XOR", "AX, [EDX+12345678H]",       ADDR32, 0x33, 0202, 0x78, 0x56, 0x34, 0x12);
    TEST("XOR", "AX, [EBX+12345678H]",       ADDR32, 0x33, 0203, 0x78, 0x56, 0x34, 0x12);
    TEST("XOR", "AX, [ECX+EAX+12345678H]",   ADDR32, 0x33, 0204, 0001, 0x78, 0x56, 0x34, 0x12);
    TEST("XOR", "AX, [ECX+EAX*2+12345678H]", ADDR32, 0x33, 0204, 0101, 0x78, 0x56, 0x34, 0x12);
    TEST("XOR", "AX, [ECX+EAX*4+12345678H]", ADDR32, 0x33, 0204, 0201, 0x78, 0x56, 0x34, 0x12);
    TEST("XOR", "AX, [ECX+EAX*8+12345678H]", ADDR32, 0x33, 0204, 0301, 0x78, 0x56, 0x34, 0x12);
    TEST("XOR", "AX, [EBP+EAX+12345678H]",   ADDR32, 0x33, 0204, 0005, 0x78, 0x56, 0x34, 0x12);
    TEST("XOR", "AX, [EBP+EAX*2+12345678H]", ADDR32, 0x33, 0204, 0105, 0x78, 0x56, 0x34, 0x12);
    TEST("XOR", "AX, [EBP+EAX*4+12345678H]", ADDR32, 0x33, 0204, 0205, 0x78, 0x56, 0x34, 0x12);
    TEST("XOR", "AX, [EBP+EAX*8+12345678H]", ADDR32, 0x33, 0204, 0305, 0x78, 0x56, 0x34, 0x12);
    TEST("XOR", "AX, [EBP+12345678H]",       ADDR32, 0x33, 0205, 0x78, 0x56, 0x34, 0x12);
    TEST("XOR", "AX, [ESI+12345678H]",       ADDR32, 0x33, 0206, 0x78, 0x56, 0x34, 0x12);
    TEST("XOR", "AX, [EDI+12345678H]",       ADDR32, 0x33, 0207, 0x78, 0x56, 0x34, 0x12);

    EPRE("XOR", "AX, AX", ADDR32, 0x33, 0300);
    EPRE("XOR", "AX, CX", ADDR32, 0x33, 0301);
    EPRE("XOR", "AX, DX", ADDR32, 0x33, 0302);
    EPRE("XOR", "AX, BX", ADDR32, 0x33, 0303);
    EPRE("XOR", "AX, SP", ADDR32, 0x33, 0304);
    EPRE("XOR", "AX, BP", ADDR32, 0x33, 0305);
    EPRE("XOR", "AX, SI", ADDR32, 0x33, 0306);
    EPRE("XOR", "AX, DI", ADDR32, 0x33, 0307);
    EPRE("XOR", "DI, AX", ADDR32, 0x33, 0370);
    EPRE("XOR", "SI, AX", ADDR32, 0x33, 0360);
    EPRE("XOR", "BP, AX", ADDR32, 0x33, 0350);
    EPRE("XOR", "SP, AX", ADDR32, 0x33, 0340);
    EPRE("XOR", "BX, AX", ADDR32, 0x33, 0330);
    EPRE("XOR", "DX, AX", ADDR32, 0x33, 0320);
    EPRE("XOR", "CX, AX", ADDR32, 0x33, 0310);

    gnu_as(true);
    TEST("addr32 xor", "ax, [ebx+0x12345678]",       ADDR32, 0x33, 0004, 0035, 0x78, 0x56, 0x34, 0x12);
    TEST("addr32 xor", "ax, [ebp+ebx+0x12345678]",   ADDR32, 0x33, 0204, 0035, 0x78, 0x56, 0x34, 0x12);
    TEST("addr32 xor", "ax, [0x12345678]",           ADDR32, 0x33, 0004, 0045, 0x78, 0x56, 0x34, 0x12);
    TEST("addr32 xor", "ax, [ebp+0x12345678]",       ADDR32, 0x33, 0204, 0045, 0x78, 0x56, 0x34, 0x12);
    TEST("addr32 xor", "ax, [esi+0x12345678]",       ADDR32, 0x33, 0204, 0046, 0x78, 0x56, 0x34, 0x12);
    TEST("addr32 xor", "ax, [ebx*2+0x12345678]",     ADDR32, 0x33, 0004, 0135, 0x78, 0x56, 0x34, 0x12);
    TEST("addr32 xor", "ax, [ebp+ebx*2+0x12345678]", ADDR32, 0x33, 0204, 0135, 0x78, 0x56, 0x34, 0x12);
    TEST("addr32 xor", "ax, [ebp+0x12345678]",       ADDR32, 0x33, 0204, 0145, 0x78, 0x56, 0x34, 0x12);
    TEST("addr32 xor", "ax, [esi+0x12345678]",       ADDR32, 0x33, 0204, 0146, 0x78, 0x56, 0x34, 0x12);
    TEST("addr32 xor", "ax, [ebx*4+0x12345678]",     ADDR32, 0x33, 0004, 0235, 0x78, 0x56, 0x34, 0x12);
    TEST("addr32 xor", "ax, [0x12345678]",           ADDR32, 0x33, 0004, 0245, 0x78, 0x56, 0x34, 0x12);
    TEST("addr32 xor", "ax, [ebx*8+0x12345678]",     ADDR32, 0x33, 0004, 0335, 0x78, 0x56, 0x34, 0x12);
    TEST("addr32 xor", "ax, [0x12345678]",           ADDR32, 0x33, 0004, 0345, 0x78, 0x56, 0x34, 0x12);
    TEST("addr32 xor", "ax, [0x12345678]",           ADDR32, 0x33, 0005, 0x78, 0x56, 0x34, 0x12);

    TEST("addr32 xor", "ax, [eax+0x12345678]",       ADDR32, 0x33, 0200, 0x78, 0x56, 0x34, 0x12);
    TEST("addr32 xor", "ax, [ecx+0x12345678]",       ADDR32, 0x33, 0201, 0x78, 0x56, 0x34, 0x12);
    TEST("addr32 xor", "ax, [edx+0x12345678]",       ADDR32, 0x33, 0202, 0x78, 0x56, 0x34, 0x12);
    TEST("addr32 xor", "ax, [ebx+0x12345678]",       ADDR32, 0x33, 0203, 0x78, 0x56, 0x34, 0x12);
    TEST("addr32 xor", "ax, [ecx+eax+0x12345678]",   ADDR32, 0x33, 0204, 0001, 0x78, 0x56, 0x34, 0x12);
    TEST("addr32 xor", "ax, [ecx+eax*2+0x12345678]", ADDR32, 0x33, 0204, 0101, 0x78, 0x56, 0x34, 0x12);
    TEST("addr32 xor", "ax, [ecx+eax*4+0x12345678]", ADDR32, 0x33, 0204, 0201, 0x78, 0x56, 0x34, 0x12);
    TEST("addr32 xor", "ax, [ecx+eax*8+0x12345678]", ADDR32, 0x33, 0204, 0301, 0x78, 0x56, 0x34, 0x12);
    TEST("addr32 xor", "ax, [ebp+eax+0x12345678]",   ADDR32, 0x33, 0204, 0005, 0x78, 0x56, 0x34, 0x12);
    TEST("addr32 xor", "ax, [ebp+eax*2+0x12345678]", ADDR32, 0x33, 0204, 0105, 0x78, 0x56, 0x34, 0x12);
    TEST("addr32 xor", "ax, [ebp+eax*4+0x12345678]", ADDR32, 0x33, 0204, 0205, 0x78, 0x56, 0x34, 0x12);
    TEST("addr32 xor", "ax, [ebp+eax*8+0x12345678]", ADDR32, 0x33, 0204, 0305, 0x78, 0x56, 0x34, 0x12);
    TEST("addr32 xor", "ax, [ebp+0x12345678]",       ADDR32, 0x33, 0205, 0x78, 0x56, 0x34, 0x12);
    TEST("addr32 xor", "ax, [esi+0x12345678]",       ADDR32, 0x33, 0206, 0x78, 0x56, 0x34, 0x12);
    TEST("addr32 xor", "ax, [edi+0x12345678]",       ADDR32, 0x33, 0207, 0x78, 0x56, 0x34, 0x12);
}

#if !defined(LIBASM_I8086_NOFPU)

void test_float() {
    if (!fpu_on())
        return;

    constexpr auto FWAIT = 0x9B;
    UNKN(            FWAIT, 0x00);
    UNKN(            FWAIT, 0xD7);
    UNKN(            FWAIT, 0xD8);
    NMEM("", "", "", FWAIT, 0xD9);
    UNKN(            FWAIT, 0xDA);
    NMEM("", "", "", FWAIT, 0xDB);
    UNKN(            FWAIT, 0xDC);
    NMEM("", "", "", FWAIT, 0xDD);
    UNKN(            FWAIT, 0xDE);
    UNKN(            FWAIT, 0xDF);
    UNKN(            FWAIT, 0xE0);

    TEST("FINIT", "", FWAIT, 0xDB, 0xE3);

    TEST("FLDCW", "[SI]",          0xD9, 0054);
    TEST("FLDCW", "[1234H]",       0xD9, 0056, 0x34, 0x12);
    TEST("FLDCW", "[DI+52]",       0xD9, 0155, 0x34);
    TEST("FLDCW", "[BP+1234H]",    0xD9, 0256, 0x34, 0x12);
    TEST("FLDCW", "[BX+SI]",       0xD9, 0050);
    TEST("FLDCW", "[BX+DI-52]",    0xD9, 0151, 0xCC);
    TEST("FLDCW", "[BP+SI+89ABH]", 0xD9, 0252, 0xAB, 0x89);
    if (is80386() && is80387()) {
        EPRE("FLDCW", "[SI]", DATA32, 0xD9, 0054);
    }

    TEST("FSTCW", "[SI]",          FWAIT, 0xD9, 0074);
    TEST("FSTCW", "[1234H]",       FWAIT, 0xD9, 0076, 0x34, 0x12);
    TEST("FSTCW", "[DI+52]",       FWAIT, 0xD9, 0175, 0x34);
    TEST("FSTCW", "[BP+1234H]",    FWAIT, 0xD9, 0276, 0x34, 0x12);
    TEST("FSTCW", "[BX+SI]",       FWAIT, 0xD9, 0070);
    TEST("FSTCW", "[BX+DI-52]",    FWAIT, 0xD9, 0171, 0xCC);
    TEST("FSTCW", "[BP+SI+89ABH]", FWAIT, 0xD9, 0272, 0xAB, 0x89);
    if (is80386() && is80387()) {
        EPRE("FSTCW", "[1234H]",   FWAIT, DATA32, 0xD9, 0076, 0x34, 0x12);
    }

    TEST("FLDENV", "[SI]",          0xD9, 0044);
    TEST("FLDENV", "[1234H]",       0xD9, 0046, 0x34, 0x12);
    TEST("FLDENV", "[DI+52]",       0xD9, 0145, 0x34);
    TEST("FLDENV", "[BP+1234H]",    0xD9, 0246, 0x34, 0x12);
    TEST("FLDENV", "[BX+SI]",       0xD9, 0040);
    TEST("FLDENV", "[BX+DI-52]",    0xD9, 0141, 0xCC);
    TEST("FLDENV", "[BP+SI+89ABH]", 0xD9, 0242, 0xAB, 0x89);

    TEST("FSTSW", "[SI]",          FWAIT, 0xDD, 0074);
    TEST("FSTSW", "[1234H]",       FWAIT, 0xDD, 0076, 0x34, 0x12);
    TEST("FSTSW", "[DI+52]",       FWAIT, 0xDD, 0175, 0x34);
    TEST("FSTSW", "[BP+1234H]",    FWAIT, 0xDD, 0276, 0x34, 0x12);
    TEST("FSTSW", "[BX+SI]",       FWAIT, 0xDD, 0070);
    TEST("FSTSW", "[BX+DI-52]",    FWAIT, 0xDD, 0171, 0xCC);
    TEST("FSTSW", "[BP+SI+89ABH]", FWAIT, 0xDD, 0272, 0xAB, 0x89);
    UNKN(FWAIT, 0xDD, 0370);    // FSTSW AX
    if (is80386() && is80387()) {
        EPRE("FSTSW", "[DI+52]",   FWAIT, DATA32, 0xDD, 0175, 0x34);
    }

    TEST("FCLEX", "", FWAIT, 0xDB, 0xE2);

    TEST("FSTENV", "[SI]",          FWAIT, 0xD9, 0064);
    TEST("FSTENV", "[1234H]",       FWAIT, 0xD9, 0066, 0x34, 0x12);
    TEST("FSTENV", "[DI+52]",       FWAIT, 0xD9, 0165, 0x34);
    TEST("FSTENV", "[BP+1234H]",    FWAIT, 0xD9, 0266, 0x34, 0x12);
    TEST("FSTENV", "[BX+SI]",       FWAIT, 0xD9, 0060);
    TEST("FSTENV", "[BX+DI-52]",    FWAIT, 0xD9, 0161, 0xCC);
    TEST("FSTENV", "[BP+SI+89ABH]", FWAIT, 0xD9, 0262, 0xAB, 0x89);

    TEST("FSAVE", "[SI]",          FWAIT, 0xDD, 0064);
    TEST("FSAVE", "[1234H]",       FWAIT, 0xDD, 0066, 0x34, 0x12);
    TEST("FSAVE", "[DI+52]",       FWAIT, 0xDD, 0165, 0x34);
    TEST("FSAVE", "[BP+1234H]",    FWAIT, 0xDD, 0266, 0x34, 0x12);
    TEST("FSAVE", "[BX+SI]",       FWAIT, 0xDD, 0060);
    TEST("FSAVE", "[BX+DI-52]",    FWAIT, 0xDD, 0161, 0xCC);
    TEST("FSAVE", "[BP+SI+89ABH]", FWAIT, 0xDD, 0262, 0xAB, 0x89);

    TEST("FRSTOR", "[SI]",          0xDD, 0044);
    TEST("FRSTOR", "[1234H]",       0xDD, 0046, 0x34, 0x12);
    TEST("FRSTOR", "[DI+52]",       0xDD, 0145, 0x34);
    TEST("FRSTOR", "[BP+1234H]",    0xDD, 0246, 0x34, 0x12);
    TEST("FRSTOR", "[BX+SI]",       0xDD, 0040);
    TEST("FRSTOR", "[BX+DI-52]",    0xDD, 0141, 0xCC);
    TEST("FRSTOR", "[BP+SI+89ABH]", 0xDD, 0242, 0xAB, 0x89);

    TEST("FENI",  "", FWAIT, 0xDB, 0xE0);
    TEST("FDISI", "", FWAIT, 0xDB, 0xE1);

    TEST("FLD", "DWORD [SI]",          0xD9, 0004);
    TEST("FLD", "DWORD [1234H]",       0xD9, 0006, 0x34, 0x12);
    TEST("FLD", "DWORD [DI+52]",       0xD9, 0105, 0x34);
    TEST("FLD", "DWORD [BP+1234H]",    0xD9, 0206, 0x34, 0x12);
    TEST("FLD", "DWORD [BX+SI]",       0xD9, 0000);
    TEST("FLD", "DWORD [BX+DI-52]",    0xD9, 0101, 0xCC);
    TEST("FLD", "DWORD [BP+SI+89ABH]", 0xD9, 0202, 0xAB, 0x89);

    TEST("FLD", "QWORD [SI]",          0xDD, 0004);
    TEST("FLD", "QWORD [1234H]",       0xDD, 0006, 0x34, 0x12);
    TEST("FLD", "QWORD [DI+52]",       0xDD, 0105, 0x34);
    TEST("FLD", "QWORD [BP+1234H]",    0xDD, 0206, 0x34, 0x12);
    TEST("FLD", "QWORD [BX+SI]",       0xDD, 0000);
    TEST("FLD", "QWORD [BX+DI-52]",    0xDD, 0101, 0xCC);
    TEST("FLD", "QWORD [BP+SI+89ABH]", 0xDD, 0202, 0xAB, 0x89);

    TEST("FLD", "TBYTE [SI]",          0xDB, 0054);
    TEST("FLD", "TBYTE [1234H]",       0xDB, 0056, 0x34, 0x12);
    TEST("FLD", "TBYTE [DI+52]",       0xDB, 0155, 0x34);
    TEST("FLD", "TBYTE [BP+1234H]",    0xDB, 0256, 0x34, 0x12);
    TEST("FLD", "TBYTE [BX+SI]",       0xDB, 0050);
    TEST("FLD", "TBYTE [BX+DI-52]",    0xDB, 0151, 0xCC);
    TEST("FLD", "TBYTE [BP+SI+89ABH]", 0xDB, 0252, 0xAB, 0x89);

    TEST("FLD", "ST(0)", 0xD9, 0xC0);
    TEST("FLD", "ST(7)", 0xD9, 0xC7);

    TEST("FST", "DWORD [SI]",          0xD9, 0024);
    TEST("FST", "DWORD [1234H]",       0xD9, 0026, 0x34, 0x12);
    TEST("FST", "DWORD [DI+52]",       0xD9, 0125, 0x34);
    TEST("FST", "DWORD [BP+1234H]",    0xD9, 0226, 0x34, 0x12);
    TEST("FST", "DWORD [BX+SI]",       0xD9, 0020);
    TEST("FST", "DWORD [BX+DI-52]",    0xD9, 0121, 0xCC);
    TEST("FST", "DWORD [BP+SI+89ABH]", 0xD9, 0222, 0xAB, 0x89);

    TEST("FST", "QWORD [SI]",          0xDD, 0024);
    TEST("FST", "QWORD [1234H]",       0xDD, 0026, 0x34, 0x12);
    TEST("FST", "QWORD [DI+52]",       0xDD, 0125, 0x34);
    TEST("FST", "QWORD [BP+1234H]",    0xDD, 0226, 0x34, 0x12);
    TEST("FST", "QWORD [BX+SI]",       0xDD, 0020);
    TEST("FST", "QWORD [BX+DI-52]",    0xDD, 0121, 0xCC);
    TEST("FST", "QWORD [BP+SI+89ABH]", 0xDD, 0222, 0xAB, 0x89);

    TEST("FST", "ST(0)", 0xDD, 0xD0);
    TEST("FST", "ST(7)", 0xDD, 0xD7);

    TEST("FSTP", "DWORD [SI]",          0xD9, 0034);
    TEST("FSTP", "DWORD [1234H]",       0xD9, 0036, 0x34, 0x12);
    TEST("FSTP", "DWORD [DI+52]",       0xD9, 0135, 0x34);
    TEST("FSTP", "DWORD [BP+1234H]",    0xD9, 0236, 0x34, 0x12);
    TEST("FSTP", "DWORD [BX+SI]",       0xD9, 0030);
    TEST("FSTP", "DWORD [BX+DI-52]",    0xD9, 0131, 0xCC);
    TEST("FSTP", "DWORD [BP+SI+89ABH]", 0xD9, 0232, 0xAB, 0x89);

    TEST("FSTP", "QWORD [SI]",          0xDD, 0034);
    TEST("FSTP", "QWORD [1234H]",       0xDD, 0036, 0x34, 0x12);
    TEST("FSTP", "QWORD [DI+52]",       0xDD, 0135, 0x34);
    TEST("FSTP", "QWORD [BP+1234H]",    0xDD, 0236, 0x34, 0x12);
    TEST("FSTP", "QWORD [BX+SI]",       0xDD, 0030);
    TEST("FSTP", "QWORD [BX+DI-52]",    0xDD, 0131, 0xCC);
    TEST("FSTP", "QWORD [BP+SI+89ABH]", 0xDD, 0232, 0xAB, 0x89);

    TEST("FSTP", "TBYTE [SI]",          0xDB, 0074);
    TEST("FSTP", "TBYTE [1234H]",       0xDB, 0076, 0x34, 0x12);
    TEST("FSTP", "TBYTE [DI+52]",       0xDB, 0175, 0x34);
    TEST("FSTP", "TBYTE [BP+1234H]",    0xDB, 0276, 0x34, 0x12);
    TEST("FSTP", "TBYTE [BX+SI]",       0xDB, 0070);
    TEST("FSTP", "TBYTE [BX+DI-52]",    0xDB, 0171, 0xCC);
    TEST("FSTP", "TBYTE [BP+SI+89ABH]", 0xDB, 0272, 0xAB, 0x89);

    TEST("FSTP", "ST(0)", 0xDD, 0xD8);
    TEST("FSTP", "ST(7)", 0xDD, 0xDF);

    TEST("FLDZ", "", 0xD9, 0xEE);
    TEST("FLD1", "", 0xD9, 0xE8);
    TEST("FLDPI", "", 0xD9, 0xEB);
    TEST("FLDL2T", "", 0xD9, 0xE9);
    TEST("FLDL2E", "", 0xD9, 0xEA);
    TEST("FLDLG2", "", 0xD9, 0xEC);
    TEST("FLDLN2", "", 0xD9, 0xED);

    TEST("FXCH", "ST(0)", 0xD9, 0xC8);
    TEST("FXCH", "ST(7)", 0xD9, 0xCF);

    TEST("FILD", "WORD [SI]",          0xDF, 0004);
    TEST("FILD", "WORD [1234H]",       0xDF, 0006, 0x34, 0x12);
    TEST("FILD", "WORD [DI+52]",       0xDF, 0105, 0x34);
    TEST("FILD", "WORD [BP+1234H]",    0xDF, 0206, 0x34, 0x12);
    TEST("FILD", "WORD [BX+SI]",       0xDF, 0000);
    TEST("FILD", "WORD [BX+DI-52]",    0xDF, 0101, 0xCC);
    TEST("FILD", "WORD [BP+SI+89ABH]", 0xDF, 0202, 0xAB, 0x89);

    TEST("FILD", "DWORD [SI]",          0xDB, 0004);
    TEST("FILD", "DWORD [1234H]",       0xDB, 0006, 0x34, 0x12);
    TEST("FILD", "DWORD [DI+52]",       0xDB, 0105, 0x34);
    TEST("FILD", "DWORD [BP+1234H]",    0xDB, 0206, 0x34, 0x12);
    TEST("FILD", "DWORD [BX+SI]",       0xDB, 0000);
    TEST("FILD", "DWORD [BX+DI-52]",    0xDB, 0101, 0xCC);
    TEST("FILD", "DWORD [BP+SI+89ABH]", 0xDB, 0202, 0xAB, 0x89);

    TEST("FILD", "QWORD [SI]",          0xDF, 0054);
    TEST("FILD", "QWORD [1234H]",       0xDF, 0056, 0x34, 0x12);
    TEST("FILD", "QWORD [DI+52]",       0xDF, 0155, 0x34);
    TEST("FILD", "QWORD [BP+1234H]",    0xDF, 0256, 0x34, 0x12);
    TEST("FILD", "QWORD [BX+SI]",       0xDF, 0050);
    TEST("FILD", "QWORD [BX+DI-52]",    0xDF, 0151, 0xCC);
    TEST("FILD", "QWORD [BP+SI+89ABH]", 0xDF, 0252, 0xAB, 0x89);

    TEST("FIST", "WORD [SI]",          0xDF, 0024);
    TEST("FIST", "WORD [1234H]",       0xDF, 0026, 0x34, 0x12);
    TEST("FIST", "WORD [DI+52]",       0xDF, 0125, 0x34);
    TEST("FIST", "WORD [BP+1234H]",    0xDF, 0226, 0x34, 0x12);
    TEST("FIST", "WORD [BX+SI]",       0xDF, 0020);
    TEST("FIST", "WORD [BX+DI-52]",    0xDF, 0121, 0xCC);
    TEST("FIST", "WORD [BP+SI+89ABH]", 0xDF, 0222, 0xAB, 0x89);

    TEST("FIST", "DWORD [SI]",          0xDB, 0024);
    TEST("FIST", "DWORD [1234H]",       0xDB, 0026, 0x34, 0x12);
    TEST("FIST", "DWORD [DI+52]",       0xDB, 0125, 0x34);
    TEST("FIST", "DWORD [BP+1234H]",    0xDB, 0226, 0x34, 0x12);
    TEST("FIST", "DWORD [BX+SI]",       0xDB, 0020);
    TEST("FIST", "DWORD [BX+DI-52]",    0xDB, 0121, 0xCC);
    TEST("FIST", "DWORD [BP+SI+89ABH]", 0xDB, 0222, 0xAB, 0x89);

    TEST("FISTP", "WORD [SI]",          0xDF, 0034);
    TEST("FISTP", "WORD [1234H]",       0xDF, 0036, 0x34, 0x12);
    TEST("FISTP", "WORD [DI+52]",       0xDF, 0135, 0x34);
    TEST("FISTP", "WORD [BP+1234H]",    0xDF, 0236, 0x34, 0x12);
    TEST("FISTP", "WORD [BX+SI]",       0xDF, 0030);
    TEST("FISTP", "WORD [BX+DI-52]",    0xDF, 0131, 0xCC);
    TEST("FISTP", "WORD [BP+SI+89ABH]", 0xDF, 0232, 0xAB, 0x89);

    TEST("FISTP", "DWORD [SI]",          0xDB, 0034);
    TEST("FISTP", "DWORD [1234H]",       0xDB, 0036, 0x34, 0x12);
    TEST("FISTP", "DWORD [DI+52]",       0xDB, 0135, 0x34);
    TEST("FISTP", "DWORD [BP+1234H]",    0xDB, 0236, 0x34, 0x12);
    TEST("FISTP", "DWORD [BX+SI]",       0xDB, 0030);
    TEST("FISTP", "DWORD [BX+DI-52]",    0xDB, 0131, 0xCC);
    TEST("FISTP", "DWORD [BP+SI+89ABH]", 0xDB, 0232, 0xAB, 0x89);

    TEST("FISTP", "QWORD [SI]",          0xDF, 0074);
    TEST("FISTP", "QWORD [1234H]",       0xDF, 0076, 0x34, 0x12);
    TEST("FISTP", "QWORD [DI+52]",       0xDF, 0175, 0x34);
    TEST("FISTP", "QWORD [BP+1234H]",    0xDF, 0276, 0x34, 0x12);
    TEST("FISTP", "QWORD [BX+SI]",       0xDF, 0070);
    TEST("FISTP", "QWORD [BX+DI-52]",    0xDF, 0171, 0xCC);
    TEST("FISTP", "QWORD [BP+SI+89ABH]", 0xDF, 0272, 0xAB, 0x89);

    TEST("FBLD", "TBYTE [SI]",          0xDF, 0044);
    TEST("FBLD", "TBYTE [1234H]",       0xDF, 0046, 0x34, 0x12);
    TEST("FBLD", "TBYTE [DI+52]",       0xDF, 0145, 0x34);
    TEST("FBLD", "TBYTE [BP+1234H]",    0xDF, 0246, 0x34, 0x12);
    TEST("FBLD", "TBYTE [BX+SI]",       0xDF, 0040);
    TEST("FBLD", "TBYTE [BX+DI-52]",    0xDF, 0141, 0xCC);
    TEST("FBLD", "TBYTE [BP+SI+89ABH]", 0xDF, 0242, 0xAB, 0x89);

    TEST("FBSTP", "TBYTE [SI]",          0xDF, 0064);
    TEST("FBSTP", "TBYTE [1234H]",       0xDF, 0066, 0x34, 0x12);
    TEST("FBSTP", "TBYTE [DI+52]",       0xDF, 0165, 0x34);
    TEST("FBSTP", "TBYTE [BP+1234H]",    0xDF, 0266, 0x34, 0x12);
    TEST("FBSTP", "TBYTE [BX+SI]",       0xDF, 0060);
    TEST("FBSTP", "TBYTE [BX+DI-52]",    0xDF, 0161, 0xCC);
    TEST("FBSTP", "TBYTE [BP+SI+89ABH]", 0xDF, 0262, 0xAB, 0x89);

    TEST("FADD", "DWORD [SI]",          0xD8, 0004);
    TEST("FADD", "DWORD [1234H]",       0xD8, 0006, 0x34, 0x12);
    TEST("FADD", "DWORD [DI+52]",       0xD8, 0105, 0x34);
    TEST("FADD", "DWORD [BP+1234H]",    0xD8, 0206, 0x34, 0x12);
    TEST("FADD", "DWORD [BX+SI]",       0xD8, 0000);
    TEST("FADD", "DWORD [BX+DI-52]",    0xD8, 0101, 0xCC);
    TEST("FADD", "DWORD [BP+SI+89ABH]", 0xD8, 0202, 0xAB, 0x89);

    TEST("FADD", "QWORD [SI]",          0xDC, 0004);
    TEST("FADD", "QWORD [1234H]",       0xDC, 0006, 0x34, 0x12);
    TEST("FADD", "QWORD [DI+52]",       0xDC, 0105, 0x34);
    TEST("FADD", "QWORD [BP+1234H]",    0xDC, 0206, 0x34, 0x12);
    TEST("FADD", "QWORD [BX+SI]",       0xDC, 0000);
    TEST("FADD", "QWORD [BX+DI-52]",    0xDC, 0101, 0xCC);
    TEST("FADD", "QWORD [BP+SI+89ABH]", 0xDC, 0202, 0xAB, 0x89);

    TEST("FADD", "ST, ST(1)",  0xD8, 0xC1);
    TEST("FADD", "ST(2), ST",  0xDC, 0xC2);
    TEST("FADDP", "ST(3), ST", 0xDE, 0xC3);

    TEST("FMUL", "DWORD [SI]",          0xD8, 0014);
    TEST("FMUL", "DWORD [1234H]",       0xD8, 0016, 0x34, 0x12);
    TEST("FMUL", "DWORD [DI+52]",       0xD8, 0115, 0x34);
    TEST("FMUL", "DWORD [BP+1234H]",    0xD8, 0216, 0x34, 0x12);
    TEST("FMUL", "DWORD [BX+SI]",       0xD8, 0010);
    TEST("FMUL", "DWORD [BX+DI-52]",    0xD8, 0111, 0xCC);
    TEST("FMUL", "DWORD [BP+SI+89ABH]", 0xD8, 0212, 0xAB, 0x89);

    TEST("FMUL", "QWORD [SI]",          0xDC, 0014);
    TEST("FMUL", "QWORD [1234H]",       0xDC, 0016, 0x34, 0x12);
    TEST("FMUL", "QWORD [DI+52]",       0xDC, 0115, 0x34);
    TEST("FMUL", "QWORD [BP+1234H]",    0xDC, 0216, 0x34, 0x12);
    TEST("FMUL", "QWORD [BX+SI]",       0xDC, 0010);
    TEST("FMUL", "QWORD [BX+DI-52]",    0xDC, 0111, 0xCC);
    TEST("FMUL", "QWORD [BP+SI+89ABH]", 0xDC, 0212, 0xAB, 0x89);

    TEST("FMUL", "ST, ST(4)",  0xD8, 0xCC);
    TEST("FMUL", "ST(5), ST",  0xDC, 0xCD);
    TEST("FMULP", "ST(6), ST", 0xDE, 0xCE);

    TEST("FSUB", "DWORD [SI]",          0xD8, 0044);
    TEST("FSUB", "DWORD [1234H]",       0xD8, 0046, 0x34, 0x12);
    TEST("FSUB", "DWORD [DI+52]",       0xD8, 0145, 0x34);
    TEST("FSUB", "DWORD [BP+1234H]",    0xD8, 0246, 0x34, 0x12);
    TEST("FSUB", "DWORD [BX+SI]",       0xD8, 0040);
    TEST("FSUB", "DWORD [BX+DI-52]",    0xD8, 0141, 0xCC);
    TEST("FSUB", "DWORD [BP+SI+89ABH]", 0xD8, 0242, 0xAB, 0x89);

    TEST("FSUB", "QWORD [SI]",          0xDC, 0044);
    TEST("FSUB", "QWORD [1234H]",       0xDC, 0046, 0x34, 0x12);
    TEST("FSUB", "QWORD [DI+52]",       0xDC, 0145, 0x34);
    TEST("FSUB", "QWORD [BP+1234H]",    0xDC, 0246, 0x34, 0x12);
    TEST("FSUB", "QWORD [BX+SI]",       0xDC, 0040);
    TEST("FSUB", "QWORD [BX+DI-52]",    0xDC, 0141, 0xCC);
    TEST("FSUB", "QWORD [BP+SI+89ABH]", 0xDC, 0242, 0xAB, 0x89);

    TEST("FSUB", "ST, ST(7)",  0xD8, 0xE7);
    TEST("FSUB", "ST(1), ST",  0xDC, 0xE9);
    TEST("FSUBP", "ST(1), ST", 0xDE, 0xE9);

    TEST("FSUBR", "DWORD [SI]",          0xD8, 0054);
    TEST("FSUBR", "DWORD [1234H]",       0xD8, 0056, 0x34, 0x12);
    TEST("FSUBR", "DWORD [DI+52]",       0xD8, 0155, 0x34);
    TEST("FSUBR", "DWORD [BP+1234H]",    0xD8, 0256, 0x34, 0x12);
    TEST("FSUBR", "DWORD [BX+SI]",       0xD8, 0050);
    TEST("FSUBR", "DWORD [BX+DI-52]",    0xD8, 0151, 0xCC);
    TEST("FSUBR", "DWORD [BP+SI+89ABH]", 0xD8, 0252, 0xAB, 0x89);

    TEST("FSUBR", "QWORD [SI]",          0xDC, 0054);
    TEST("FSUBR", "QWORD [1234H]",       0xDC, 0056, 0x34, 0x12);
    TEST("FSUBR", "QWORD [DI+52]",       0xDC, 0155, 0x34);
    TEST("FSUBR", "QWORD [BP+1234H]",    0xDC, 0256, 0x34, 0x12);
    TEST("FSUBR", "QWORD [BX+SI]",       0xDC, 0050);
    TEST("FSUBR", "QWORD [BX+DI-52]",    0xDC, 0151, 0xCC);
    TEST("FSUBR", "QWORD [BP+SI+89ABH]", 0xDC, 0252, 0xAB, 0x89);

    TEST("FSUBR", "ST, ST(2)",  0xD8, 0xEA);
    TEST("FSUBR", "ST(3), ST",  0xDC, 0xE3);
    TEST("FSUBRP", "ST(4), ST", 0xDE, 0xE4);

    TEST("FDIV", "DWORD [SI]",          0xD8, 0064);
    TEST("FDIV", "DWORD [1234H]",       0xD8, 0066, 0x34, 0x12);
    TEST("FDIV", "DWORD [DI+52]",       0xD8, 0165, 0x34);
    TEST("FDIV", "DWORD [BP+1234H]",    0xD8, 0266, 0x34, 0x12);
    TEST("FDIV", "DWORD [BX+SI]",       0xD8, 0060);
    TEST("FDIV", "DWORD [BX+DI-52]",    0xD8, 0161, 0xCC);
    TEST("FDIV", "DWORD [BP+SI+89ABH]", 0xD8, 0262, 0xAB, 0x89);

    TEST("FDIV", "QWORD [SI]",          0xDC, 0064);
    TEST("FDIV", "QWORD [1234H]",       0xDC, 0066, 0x34, 0x12);
    TEST("FDIV", "QWORD [DI+52]",       0xDC, 0165, 0x34);
    TEST("FDIV", "QWORD [BP+1234H]",    0xDC, 0266, 0x34, 0x12);
    TEST("FDIV", "QWORD [BX+SI]",       0xDC, 0060);
    TEST("FDIV", "QWORD [BX+DI-52]",    0xDC, 0161, 0xCC);
    TEST("FDIV", "QWORD [BP+SI+89ABH]", 0xDC, 0262, 0xAB, 0x89);

    TEST("FDIV", "ST, ST(7)",  0xD8, 0xF7);
    TEST("FDIV", "ST(1), ST",  0xDC, 0xF9);
    TEST("FDIVP", "ST(1), ST", 0xDE, 0xF9);

    TEST("FDIVR", "DWORD [SI]",          0xD8, 0074);
    TEST("FDIVR", "DWORD [1234H]",       0xD8, 0076, 0x34, 0x12);
    TEST("FDIVR", "DWORD [DI+52]",       0xD8, 0175, 0x34);
    TEST("FDIVR", "DWORD [BP+1234H]",    0xD8, 0276, 0x34, 0x12);
    TEST("FDIVR", "DWORD [BX+SI]",       0xD8, 0070);
    TEST("FDIVR", "DWORD [BX+DI-52]",    0xD8, 0171, 0xCC);
    TEST("FDIVR", "DWORD [BP+SI+89ABH]", 0xD8, 0272, 0xAB, 0x89);

    TEST("FDIVR", "QWORD [SI]",          0xDC, 0074);
    TEST("FDIVR", "QWORD [1234H]",       0xDC, 0076, 0x34, 0x12);
    TEST("FDIVR", "QWORD [DI+52]",       0xDC, 0175, 0x34);
    TEST("FDIVR", "QWORD [BP+1234H]",    0xDC, 0276, 0x34, 0x12);
    TEST("FDIVR", "QWORD [BX+SI]",       0xDC, 0070);
    TEST("FDIVR", "QWORD [BX+DI-52]",    0xDC, 0171, 0xCC);
    TEST("FDIVR", "QWORD [BP+SI+89ABH]", 0xDC, 0272, 0xAB, 0x89);

    TEST("FDIVR", "ST, ST(2)",  0xD8, 0xFA);
    TEST("FDIVR", "ST(3), ST",  0xDC, 0xF3);
    TEST("FDIVRP", "ST(4), ST", 0xDE, 0xF4);

    TEST("FCOM", "DWORD [SI]",          0xD8, 0024);
    TEST("FCOM", "DWORD [1234H]",       0xD8, 0026, 0x34, 0x12);
    TEST("FCOM", "DWORD [DI+52]",       0xD8, 0125, 0x34);
    TEST("FCOM", "DWORD [BP+1234H]",    0xD8, 0226, 0x34, 0x12);
    TEST("FCOM", "DWORD [BX+SI]",       0xD8, 0020);
    TEST("FCOM", "DWORD [BX+DI-52]",    0xD8, 0121, 0xCC);
    TEST("FCOM", "DWORD [BP+SI+89ABH]", 0xD8, 0222, 0xAB, 0x89);

    TEST("FCOM", "QWORD [SI]",          0xDC, 0024);
    TEST("FCOM", "QWORD [1234H]",       0xDC, 0026, 0x34, 0x12);
    TEST("FCOM", "QWORD [DI+52]",       0xDC, 0125, 0x34);
    TEST("FCOM", "QWORD [BP+1234H]",    0xDC, 0226, 0x34, 0x12);
    TEST("FCOM", "QWORD [BX+SI]",       0xDC, 0020);
    TEST("FCOM", "QWORD [BX+DI-52]",    0xDC, 0121, 0xCC);
    TEST("FCOM", "QWORD [BP+SI+89ABH]", 0xDC, 0222, 0xAB, 0x89);

    TEST("FCOM", "ST(2)", 0xD8, 0xD2);

    TEST("FCOMP", "DWORD [SI]",          0xD8, 0034);
    TEST("FCOMP", "DWORD [1234H]",       0xD8, 0036, 0x34, 0x12);
    TEST("FCOMP", "DWORD [DI+52]",       0xD8, 0135, 0x34);
    TEST("FCOMP", "DWORD [BP+1234H]",    0xD8, 0236, 0x34, 0x12);
    TEST("FCOMP", "DWORD [BX+SI]",       0xD8, 0030);
    TEST("FCOMP", "DWORD [BX+DI-52]",    0xD8, 0131, 0xCC);
    TEST("FCOMP", "DWORD [BP+SI+89ABH]", 0xD8, 0232, 0xAB, 0x89);

    TEST("FCOMP", "QWORD [SI]",          0xDC, 0034);
    TEST("FCOMP", "QWORD [1234H]",       0xDC, 0036, 0x34, 0x12);
    TEST("FCOMP", "QWORD [DI+52]",       0xDC, 0135, 0x34);
    TEST("FCOMP", "QWORD [BP+1234H]",    0xDC, 0236, 0x34, 0x12);
    TEST("FCOMP", "QWORD [BX+SI]",       0xDC, 0030);
    TEST("FCOMP", "QWORD [BX+DI-52]",    0xDC, 0131, 0xCC);
    TEST("FCOMP", "QWORD [BP+SI+89ABH]", 0xDC, 0232, 0xAB, 0x89);

    TEST("FCOMP", "ST(2)", 0xD8, 0xDA);
    TEST("FCOMPP", "",     0xDE, 0xD9);

    if (is80387()) {
        TEST("FUCOM", "ST(0)", 0xDD, 0xE0);
        TEST("FUCOM", "ST(1)", 0xDD, 0xE1);
        TEST("FUCOM", "ST(2)", 0xDD, 0xE2);
        TEST("FUCOM", "ST(3)", 0xDD, 0xE3);
        TEST("FUCOM", "ST(4)", 0xDD, 0xE4);
        TEST("FUCOM", "ST(5)", 0xDD, 0xE5);
        TEST("FUCOM", "ST(6)", 0xDD, 0xE6);
        TEST("FUCOM", "ST(7)", 0xDD, 0xE7);

        TEST("FUCOMP", "ST(0)", 0xDD, 0xE8);
        TEST("FUCOMP", "ST(1)", 0xDD, 0xE9);
        TEST("FUCOMP", "ST(2)", 0xDD, 0xEA);
        TEST("FUCOMP", "ST(3)", 0xDD, 0xEB);
        TEST("FUCOMP", "ST(4)", 0xDD, 0xEC);
        TEST("FUCOMP", "ST(5)", 0xDD, 0xED);
        TEST("FUCOMP", "ST(6)", 0xDD, 0xEE);
        TEST("FUCOMP", "ST(7)", 0xDD, 0xEF);

        TEST("FUCOMPP", "", 0xDA, 0xE9);
    }

    TEST("FIADD", "WORD [SI]",          0xDE, 0004);
    TEST("FIADD", "WORD [1234H]",       0xDE, 0006, 0x34, 0x12);
    TEST("FIADD", "WORD [DI+52]",       0xDE, 0105, 0x34);
    TEST("FIADD", "WORD [BP+1234H]",    0xDE, 0206, 0x34, 0x12);
    TEST("FIADD", "WORD [BX+SI]",       0xDE, 0000);
    TEST("FIADD", "WORD [BX+DI-52]",    0xDE, 0101, 0xCC);
    TEST("FIADD", "WORD [BP+SI+89ABH]", 0xDE, 0202, 0xAB, 0x89);

    TEST("FIADD", "DWORD [SI]",          0xDA, 0004);
    TEST("FIADD", "DWORD [1234H]",       0xDA, 0006, 0x34, 0x12);
    TEST("FIADD", "DWORD [DI+52]",       0xDA, 0105, 0x34);
    TEST("FIADD", "DWORD [BP+1234H]",    0xDA, 0206, 0x34, 0x12);
    TEST("FIADD", "DWORD [BX+SI]",       0xDA, 0000);
    TEST("FIADD", "DWORD [BX+DI-52]",    0xDA, 0101, 0xCC);
    TEST("FIADD", "DWORD [BP+SI+89ABH]", 0xDA, 0202, 0xAB, 0x89);

    TEST("FIMUL", "WORD [SI]",          0xDE, 0014);
    TEST("FIMUL", "WORD [1234H]",       0xDE, 0016, 0x34, 0x12);
    TEST("FIMUL", "WORD [DI+52]",       0xDE, 0115, 0x34);
    TEST("FIMUL", "WORD [BP+1234H]",    0xDE, 0216, 0x34, 0x12);
    TEST("FIMUL", "WORD [BX+SI]",       0xDE, 0010);
    TEST("FIMUL", "WORD [BX+DI-52]",    0xDE, 0111, 0xCC);
    TEST("FIMUL", "WORD [BP+SI+89ABH]", 0xDE, 0212, 0xAB, 0x89);

    TEST("FIMUL", "DWORD [SI]",          0xDA, 0014);
    TEST("FIMUL", "DWORD [1234H]",       0xDA, 0016, 0x34, 0x12);
    TEST("FIMUL", "DWORD [DI+52]",       0xDA, 0115, 0x34);
    TEST("FIMUL", "DWORD [BP+1234H]",    0xDA, 0216, 0x34, 0x12);
    TEST("FIMUL", "DWORD [BX+SI]",       0xDA, 0010);
    TEST("FIMUL", "DWORD [BX+DI-52]",    0xDA, 0111, 0xCC);
    TEST("FIMUL", "DWORD [BP+SI+89ABH]", 0xDA, 0212, 0xAB, 0x89);

    TEST("FISUB", "WORD [SI]",          0xDE, 0044);
    TEST("FISUB", "WORD [1234H]",       0xDE, 0046, 0x34, 0x12);
    TEST("FISUB", "WORD [DI+52]",       0xDE, 0145, 0x34);
    TEST("FISUB", "WORD [BP+1234H]",    0xDE, 0246, 0x34, 0x12);
    TEST("FISUB", "WORD [BX+SI]",       0xDE, 0040);
    TEST("FISUB", "WORD [BX+DI-52]",    0xDE, 0141, 0xCC);
    TEST("FISUB", "WORD [BP+SI+89ABH]", 0xDE, 0242, 0xAB, 0x89);

    TEST("FISUB", "DWORD [SI]",          0xDA, 0044);
    TEST("FISUB", "DWORD [1234H]",       0xDA, 0046, 0x34, 0x12);
    TEST("FISUB", "DWORD [DI+52]",       0xDA, 0145, 0x34);
    TEST("FISUB", "DWORD [BP+1234H]",    0xDA, 0246, 0x34, 0x12);
    TEST("FISUB", "DWORD [BX+SI]",       0xDA, 0040);
    TEST("FISUB", "DWORD [BX+DI-52]",    0xDA, 0141, 0xCC);
    TEST("FISUB", "DWORD [BP+SI+89ABH]", 0xDA, 0242, 0xAB, 0x89);

    TEST("FISUBR", "WORD [SI]",          0xDE, 0054);
    TEST("FISUBR", "WORD [1234H]",       0xDE, 0056, 0x34, 0x12);
    TEST("FISUBR", "WORD [DI+52]",       0xDE, 0155, 0x34);
    TEST("FISUBR", "WORD [BP+1234H]",    0xDE, 0256, 0x34, 0x12);
    TEST("FISUBR", "WORD [BX+SI]",       0xDE, 0050);
    TEST("FISUBR", "WORD [BX+DI-52]",    0xDE, 0151, 0xCC);
    TEST("FISUBR", "WORD [BP+SI+89ABH]", 0xDE, 0252, 0xAB, 0x89);

    TEST("FISUBR", "DWORD [SI]",          0xDA, 0054);
    TEST("FISUBR", "DWORD [1234H]",       0xDA, 0056, 0x34, 0x12);
    TEST("FISUBR", "DWORD [DI+52]",       0xDA, 0155, 0x34);
    TEST("FISUBR", "DWORD [BP+1234H]",    0xDA, 0256, 0x34, 0x12);
    TEST("FISUBR", "DWORD [BX+SI]",       0xDA, 0050);
    TEST("FISUBR", "DWORD [BX+DI-52]",    0xDA, 0151, 0xCC);
    TEST("FISUBR", "DWORD [BP+SI+89ABH]", 0xDA, 0252, 0xAB, 0x89);

    TEST("FIDIV", "WORD [SI]",          0xDE, 0064);
    TEST("FIDIV", "WORD [1234H]",       0xDE, 0066, 0x34, 0x12);
    TEST("FIDIV", "WORD [DI+52]",       0xDE, 0165, 0x34);
    TEST("FIDIV", "WORD [BP+1234H]",    0xDE, 0266, 0x34, 0x12);
    TEST("FIDIV", "WORD [BX+SI]",       0xDE, 0060);
    TEST("FIDIV", "WORD [BX+DI-52]",    0xDE, 0161, 0xCC);
    TEST("FIDIV", "WORD [BP+SI+89ABH]", 0xDE, 0262, 0xAB, 0x89);

    TEST("FIDIV", "DWORD [SI]",          0xDA, 0064);
    TEST("FIDIV", "DWORD [1234H]",       0xDA, 0066, 0x34, 0x12);
    TEST("FIDIV", "DWORD [DI+52]",       0xDA, 0165, 0x34);
    TEST("FIDIV", "DWORD [BP+1234H]",    0xDA, 0266, 0x34, 0x12);
    TEST("FIDIV", "DWORD [BX+SI]",       0xDA, 0060);
    TEST("FIDIV", "DWORD [BX+DI-52]",    0xDA, 0161, 0xCC);
    TEST("FIDIV", "DWORD [BP+SI+89ABH]", 0xDA, 0262, 0xAB, 0x89);

    TEST("FIDIVR", "WORD [SI]",          0xDE, 0074);
    TEST("FIDIVR", "WORD [1234H]",       0xDE, 0076, 0x34, 0x12);
    TEST("FIDIVR", "WORD [DI+52]",       0xDE, 0175, 0x34);
    TEST("FIDIVR", "WORD [BP+1234H]",    0xDE, 0276, 0x34, 0x12);
    TEST("FIDIVR", "WORD [BX+SI]",       0xDE, 0070);
    TEST("FIDIVR", "WORD [BX+DI-52]",    0xDE, 0171, 0xCC);
    TEST("FIDIVR", "WORD [BP+SI+89ABH]", 0xDE, 0272, 0xAB, 0x89);

    TEST("FIDIVR", "DWORD [SI]",          0xDA, 0074);
    TEST("FIDIVR", "DWORD [1234H]",       0xDA, 0076, 0x34, 0x12);
    TEST("FIDIVR", "DWORD [DI+52]",       0xDA, 0175, 0x34);
    TEST("FIDIVR", "DWORD [BP+1234H]",    0xDA, 0276, 0x34, 0x12);
    TEST("FIDIVR", "DWORD [BX+SI]",       0xDA, 0070);
    TEST("FIDIVR", "DWORD [BX+DI-52]",    0xDA, 0171, 0xCC);
    TEST("FIDIVR", "DWORD [BP+SI+89ABH]", 0xDA, 0272, 0xAB, 0x89);

    TEST("FICOM", "WORD [SI]",          0xDE, 0024);
    TEST("FICOM", "WORD [1234H]",       0xDE, 0026, 0x34, 0x12);
    TEST("FICOM", "WORD [DI+52]",       0xDE, 0125, 0x34);
    TEST("FICOM", "WORD [BP+1234H]",    0xDE, 0226, 0x34, 0x12);
    TEST("FICOM", "WORD [BX+SI]",       0xDE, 0020);
    TEST("FICOM", "WORD [BX+DI-52]",    0xDE, 0121, 0xCC);
    TEST("FICOM", "WORD [BP+SI+89ABH]", 0xDE, 0222, 0xAB, 0x89);

    TEST("FICOM", "DWORD [SI]",          0xDA, 0024);
    TEST("FICOM", "DWORD [1234H]",       0xDA, 0026, 0x34, 0x12);
    TEST("FICOM", "DWORD [DI+52]",       0xDA, 0125, 0x34);
    TEST("FICOM", "DWORD [BP+1234H]",    0xDA, 0226, 0x34, 0x12);
    TEST("FICOM", "DWORD [BX+SI]",       0xDA, 0020);
    TEST("FICOM", "DWORD [BX+DI-52]",    0xDA, 0121, 0xCC);
    TEST("FICOM", "DWORD [BP+SI+89ABH]", 0xDA, 0222, 0xAB, 0x89);

    TEST("FICOMP", "WORD [SI]",          0xDE, 0034);
    TEST("FICOMP", "WORD [1234H]",       0xDE, 0036, 0x34, 0x12);
    TEST("FICOMP", "WORD [DI+52]",       0xDE, 0135, 0x34);
    TEST("FICOMP", "WORD [BP+1234H]",    0xDE, 0236, 0x34, 0x12);
    TEST("FICOMP", "WORD [BX+SI]",       0xDE, 0030);
    TEST("FICOMP", "WORD [BX+DI-52]",    0xDE, 0131, 0xCC);
    TEST("FICOMP", "WORD [BP+SI+89ABH]", 0xDE, 0232, 0xAB, 0x89);

    TEST("FICOMP", "DWORD [SI]",          0xDA, 0034);
    TEST("FICOMP", "DWORD [1234H]",       0xDA, 0036, 0x34, 0x12);
    TEST("FICOMP", "DWORD [DI+52]",       0xDA, 0135, 0x34);
    TEST("FICOMP", "DWORD [BP+1234H]",    0xDA, 0236, 0x34, 0x12);
    TEST("FICOMP", "DWORD [BX+SI]",       0xDA, 0030);
    TEST("FICOMP", "DWORD [BX+DI-52]",    0xDA, 0131, 0xCC);
    TEST("FICOMP", "DWORD [BP+SI+89ABH]", 0xDA, 0232, 0xAB, 0x89);

    TEST("FCHS",    "", 0xD9, 0xE0);
    TEST("FABS",    "", 0xD9, 0xE1);
    TEST("FTST",    "", 0xD9, 0xE4);
    TEST("FXAM",    "", 0xD9, 0xE5);
    TEST("FXTRACT", "", 0xD9, 0xF4);
    TEST("FPREM",   "", 0xD9, 0xF8);
    TEST("FSQRT",   "", 0xD9, 0xFA);
    TEST("FRNDINT", "", 0xD9, 0xFC);
    TEST("FSCALE",  "", 0xD9, 0xFD);

    TEST("F2XM1",   "", 0xD9, 0xF0);
    TEST("FYL2X",   "", 0xD9, 0xF1);
    TEST("FPTAN",   "", 0xD9, 0xF2);
    TEST("FPATAN",  "", 0xD9, 0xF3);
    TEST("FYL2XP1", "", 0xD9, 0xF9);

    TEST("FNOP",    "", 0xD9, 0xD0);
    TEST("FDECSTP", "", 0xD9, 0xF6);
    TEST("FINCSTP", "", 0xD9, 0xF7);
    TEST("FFREE", "ST(0)", 0xDD, 0xC0);
    TEST("FFREE", "ST(1)", 0xDD, 0xC1);
    TEST("FFREE", "ST(2)", 0xDD, 0xC2);
    TEST("FFREE", "ST(3)", 0xDD, 0xC3);
    TEST("FFREE", "ST(4)", 0xDD, 0xC4);
    TEST("FFREE", "ST(5)", 0xDD, 0xC5);
    TEST("FFREE", "ST(6)", 0xDD, 0xC6);
    TEST("FFREE", "ST(7)", 0xDD, 0xC7);
}

void test_float_nowait() {
    if (!fpu_on())
        return;

    TEST("FNINIT", "",      0xDB, 0xE3);
    TEST("FNSTCW", "[SI]",  0xD9, 0074);
    TEST("FNSTSW", "[SI]",  0xDD, 0074);
    TEST("FNCLEX", "",      0xDB, 0xE2);
    TEST("FNSTENV", "[SI]", 0xD9, 0064);
    TEST("FNSAVE",  "[SI]", 0xDD, 0064);
    TEST("FNENI", "",       0xDB, 0xE0);
    TEST("FNDISI", "",      0xDB, 0xE1);

    if (is80287() || is80387()) {
        TEST("FNSETPM", "",  0xDB, 0xE4);
        TEST("FNSTSW", "AX", 0xDF, 0xE0);
    }

    if (is80386() && is80387()) {
        EPRE("FNSTCW", "[SI]", DATA32, 0xD9, 0074);
        EPRE("FNSTSW", "[SI]", DATA32, 0xDD, 0074);
        EPRE("FNSTSW", "AX",   DATA32, 0xDF, 0xE0);
    }
}

#endif

bool contains(const uint8_t *begin, const uint8_t *end, const uint8_t opc) {
    for (auto p = begin; p < end; p++) {
        if (*p == opc)
            return true;
    }
    return false;
}

// clang-format on
#define ILOP(msg, ...)                                                   \
    do {                                                                 \
        const Config::opcode_t codes[] = {__VA_ARGS__};                  \
        const auto endian = disassembler.config().endian();              \
        const auto unit = disassembler.config().addressUnit();           \
        const ArrayMemory memory(0, codes, sizeof(codes), endian, unit); \
        char operands[20];                                               \
        Insn insn(0);                                                    \
        auto mem = memory.iterator();                                    \
        disassembler.decode(mem, insn, operands, sizeof(operands));      \
        asserter.equals(__FILE__, __LINE__, msg, ILLEGAL_OPERAND, insn); \
    } while (0)
// clang-format off

void test_illegal_aax() {
    for (auto i = 0; i < 0x100; i++) {
        const Config::opcode_t opc = i;
        if (opc == 0x0A)
            continue;  // AAM, AAD
        UNKN(0xD4, opc);
        UNKN(0xD5, opc);
    }
}

void test_illegal_modreg() {
    for (auto mod = 0; mod < 4; mod++) {
        for (auto reg = 0; reg < 8; reg++) {
            for (auto r_m = 0; r_m < 8; r_m++) {
                const Config::opcode_t opc = (mod << 6) | (reg << 3) | r_m;
                if (mod == 3) {
                    ILOP("LDS", 0xC4, opc);
                    ILOP("LES", 0xC5, opc);
                    if (is80186())
                        ILOP("BOUND", 0x62, opc);
                }
                if (reg == 1) {
                    UNKN(0xF6, opc);
                    UNKN(0xF7, opc);
                }
                if (reg == 6) {
                    UNKN(0xD0, opc);
                    UNKN(0xD1, opc);
                    UNKN(0xD2, opc);
                    UNKN(0xD3, opc);
                    if (is80186()) {
                        UNKN(0xC0, opc);
                        UNKN(0xC1, opc);
                    }
                }
                if (reg == 7)
                    UNKN(0xFF, opc);
                if (reg != 0 && reg != 1)
                    UNKN(0xFE, opc);
                if (reg != 0) {
                    UNKN(0x8F, opc);
                    UNKN(0xC6, opc);
                    UNKN(0xC7, opc);
                }
            }
        }
    }
}

void test_illegal_8086() {
    ERRT("POP", "CS", REGISTER_NOT_ALLOWED, "CS", 0x0F);

    static constexpr Config::opcode_t ILLEGALS[] = {
        0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
        0x68, 0x69, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
        0x82, 0xC0, 0xC1, 0xC8, 0xC9, 0xD6, 0xF1,
    };
    for (const auto opc : ILLEGALS)
        UNKN(opc);

    test_illegal_aax();
    test_illegal_modreg();
}

void test_illegal_v30() {
    static constexpr Config::opcode_t ILLEGALS[] = {
        0x63, 0x66, 0x67, 0x82, 0xD6, 0xF1,
    };
    for (const auto opc : ILLEGALS)
        UNKN(opc);

    test_illegal_aax();
    test_illegal_modreg();

    static constexpr Config::opcode_t LEGALS[] = {
        0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
        0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
        0x20, 0x22, 0x26, 0x28, 0x2A,
        0x31, 0x33, 0x39, 0x3B, 0xFF,
    };
    for (auto i = 0; i < 0x100; i++) {
        const Config::opcode_t opc = i;
        if (contains(ARRAY_RANGE(LEGALS), opc))
            continue;
        UNKN(0x0F, opc);
    }
}

void test_illegal_80186() {
    if (v30()) {
        test_illegal_v30();
        return;
    }

    ERRT("POP", "CS", REGISTER_NOT_ALLOWED, "CS", 0x0F);

    static constexpr Config::opcode_t ILLEGALS[] = {
        0x63, 0x64, 0x65, 0x66, 0x67, 0x82, 0xD6, 0xF1,
    };
    for (const auto opc : ILLEGALS)
        UNKN(opc);

    test_illegal_aax();
    test_illegal_modreg();
}

void test_illegal_80286() {
    static constexpr Config::opcode_t ILLEGALS[] = {
        0x82, 0xD6, 0xF1,
    };
    for (const auto opc : ILLEGALS)
        UNKN(opc);

    test_illegal_aax();
    test_illegal_modreg();

    static constexpr Config::opcode_t LEGALS_0F[] = {
        0x00, 0x01, 0x02, 0x03, 0x06,
    };
    for (auto i = 0; i < 0x100; i++) {
        const Config::opcode_t opc = i;
        if (contains(ARRAY_RANGE(LEGALS_0F), opc))
            continue;
        UNKN(0x0F, opc);
    }
    for (auto mod = 0; mod < 4; mod++) {
        for (auto reg = 0; reg < 8; reg++) {
            for (auto r_m = 0; r_m < 8; r_m++) {
                const Config::opcode_t opc = (mod << 6) | (reg << 3) | r_m;
                if (mod == 3) {
                    if (reg == 0) ILOP("SGDT", 0x0F, 0x01, opc);
                    if (reg == 1) ILOP("SIDT", 0x0F, 0x01, opc);
                    if (reg == 2) ILOP("LGDT", 0x0F, 0x01, opc);
                    if (reg == 3) ILOP("LIDT", 0x0F, 0x01, opc);
                }
                if (reg == 6 || reg == 7)
                    UNKN(0x0F, 0x00, opc);
                if (reg == 5 || reg == 7)
                    UNKN(0x0F, 0x01, opc);
            }
        }
    }
}

void test_illegal_8087() {
    if (!fpu_on()) {
        UNKN(0xD8);
        UNKN(0xD9);
        UNKN(0xDA);
        UNKN(0xDB);
        UNKN(0xDC);
        UNKN(0xDD);
        UNKN(0xDF);
        return;
    }

    static constexpr Config::opcode_t ILLEGALS_D9[] = {
        0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7,
        0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
        0xE2, 0xE3, 0xE6, 0xE7, 0xEF, 0xF5, 0xFB, 0xFE, 0xFF,
    };
    static constexpr Config::opcode_t LEGALS_DB[] = {
        0xE0, 0xE1, 0xE2, 0xE3,
    };
    static constexpr Config::opcode_t ILLEGALS_DC[] = {
        0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7,
        0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
    };
    static constexpr Config::opcode_t LEGALS_DD[] = {
        0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,
        0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7,
        0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
    };
    static constexpr Config::opcode_t ILLEGALS_DE[] = {
        0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7,
    };

    for (auto mod = 0; mod < 4; mod++) {
        for (auto reg = 0; reg < 8; reg++) {
            for (auto r_m = 0; r_m < 8; r_m++) {
                const Config::opcode_t opc = (mod << 6) | (reg << 3) | r_m;
                if (mod < 3) {
                    if (reg == 1) {
                        UNKN(0xD9, opc);
                        UNKN(0xDB, opc);
                        UNKN(0xDD, opc);
                        UNKN(0xDF, opc);
                    }
                    if (reg == 4)
                        UNKN(0xDB, opc);
                    if (reg == 5)
                        UNKN(0xDD, opc);
                    if (reg == 6)
                        UNKN(0xDB, opc);
                } else {
                    if (contains(ARRAY_RANGE(ILLEGALS_D9), opc)) {
                        if (is80387() && opc >= 0xF0) {
                            ;   // FPREM1/FSINCOS/FSIN/FCOS
                        } else {
                            UNKN(0xD9, opc);
                        }
                    }
                    if (is80387() && opc == 0xE9) {
                        ;       // FUCOMPP
                    } else {
                        UNKN(0xDA, opc);
                    }
                    if (!contains(ARRAY_RANGE(LEGALS_DB), opc)) {
                        if ((is80287() || is80387()) && opc == 0xE4) {
                            ;   // FNSETPM
                        } else {
                            UNKN(0xDB, opc);
                        }
                    }
                    if (contains(ARRAY_RANGE(ILLEGALS_DC), opc))
                        UNKN(0xDC, opc);
                    if (!contains(ARRAY_RANGE(LEGALS_DD), opc)) {
                        if (is80387() && opc >= 0xE0 && opc < 0xF0) {
                            ;   // FUCOM/FUCOMP
                        } else {
                            UNKN(0xDD, opc);
                        }
                    }
                    if (contains(ARRAY_RANGE(ILLEGALS_DE), opc))
                        UNKN(0xDE, opc);
                    if ((is80287() || is80387()) && opc == 0xE0) {
                        ;       // FNSTSW AX
                    } else {
                        UNKN(0xDF, opc);
                    }
                }
            }
        }
    }
}
// clang-format on

void run_tests(const char *cpu) {
    disassembler.setCpu(cpu);
    RUN_TEST(test_data_transfer);
    RUN_TEST(test_arithmetic);
    RUN_TEST(test_logic);
    RUN_TEST(test_repeat);
    RUN_TEST(test_lock);
    RUN_TEST(test_control_transfer);
    RUN_TEST(test_processor_control);
    RUN_TEST(test_segment_override);
    if (is80386()) {
        RUN_TEST(test_bit_manipulation);
        RUN_TEST(test_size_prefix);
        RUN_TEST(test_32bit_addressing);
    }
#if !defined(LIBASM_I8086_NOFPU)
    RUN_TEST(test_float);
    RUN_TEST(test_float_nowait);
    RUN_TEST(test_illegal_8087);
#endif
    if (is8086())
        RUN_TEST(test_illegal_8086);
    if (is80186())
        RUN_TEST(test_illegal_80186);
    if (is80286())
        RUN_TEST(test_illegal_80286);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

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

#include "asm_z80.h"
#include "test_asm_helper.h"

using namespace libasm;
using namespace libasm::z80;
using namespace libasm::test;

AsmZ80 asz80;
Assembler &assembler(asz80);

static bool isZ80() {
    return strcasecmp_P("Z80", assembler.config().cpu_P()) == 0;
}

static bool is8085() {
    return strcmp_P("8085", assembler.config().cpu_P()) == 0;
}

static bool v30emu() {
    return strcasecmp_P("V30EMU", assembler.config().cpu_P()) == 0;
}

static void set_up() {
    assembler.reset();
}

static void tear_down() {
    symtab.reset();
}

// clang-format off
void test_cpu() {
    EQUALS("cpu 8080", true, assembler.setCpu("8080"));
    EQUALS_P("get cpu", "8080", assembler.config().cpu_P());

    EQUALS("cpu 8085", true, assembler.setCpu("8085"));
    EQUALS_P("get cpu", "8085", assembler.config().cpu_P());

    EQUALS("cpu i8080", true, assembler.setCpu("i8080"));
    EQUALS_P("get cpu", "8080", assembler.config().cpu_P());

    EQUALS("cpu i8085", true, assembler.setCpu("i8085"));
    EQUALS_P("get cpu", "8085", assembler.config().cpu_P());

    EQUALS("cpu v30emu", true, assembler.setCpu("v30emu"));
    EQUALS_P("get cpu", "V30EMU", assembler.config().cpu_P());

    EQUALS("cpu z80", true, assembler.setCpu("z80"));
    EQUALS_P("get cpu", "Z80", assembler.config().cpu_P());

    EQUALS("cpu 8080zilog", true, assembler.setCpu("8080Z"));
    EQUALS_P("get cpu", "8080", assembler.config().cpu_P());

    EQUALS("cpu 8085zilog", true, assembler.setCpu("8085z"));
    EQUALS_P("get cpu", "8085", assembler.config().cpu_P());

    EQUALS("cpu i8080zilog", true, assembler.setCpu("i8080Zilog"));
    EQUALS_P("get cpu", "8080", assembler.config().cpu_P());

    EQUALS("cpu v30emuZ", true, assembler.setCpu("v30emuZ"));
    EQUALS_P("get cpu", "V30EMU", assembler.config().cpu_P());
}

static void test_move_inherent() {
    TEST("LD B,B", 0x40);
    TEST("LD B,C", 0x41);
    TEST("LD B,D", 0x42);
    TEST("LD B,E", 0x43);
    TEST("LD B,H", 0x44);
    TEST("LD B,L", 0x45);
    TEST("ld b,(hl)", 0x46);
    TEST("LD B,A", 0x47);

    TEST("LD C,B", 0x48);
    TEST("LD C,C", 0x49);
    TEST("LD C,D", 0x4A);
    TEST("LD C,E", 0x4B);
    TEST("LD C,H", 0x4C);
    TEST("LD C,L", 0x4D);
    TEST("LD C,(HL)", 0x4E);
    TEST("LD C,A", 0x4F);

    TEST("LD D,B", 0x50);
    TEST("LD D,C", 0x51);
    TEST("LD D,D", 0x52);
    TEST("LD D,E", 0x53);
    TEST("LD D,H", 0x54);
    TEST("LD D,L", 0x55);
    TEST("LD D,(HL)", 0x56);
    TEST("LD D,A", 0x57);

    TEST("LD E,B", 0x58);
    TEST("LD E,C", 0x59);
    TEST("LD E,D", 0x5A);
    TEST("LD E,E", 0x5B);
    TEST("LD E,H", 0x5C);
    TEST("LD E,L", 0x5D);
    TEST("LD E,(HL)", 0x5E);
    TEST("LD E,A", 0x5F);

    TEST("LD H,B", 0x60);
    TEST("LD H,C", 0x61);
    TEST("LD H,D", 0x62);
    TEST("LD H,E", 0x63);
    TEST("LD H,H", 0x64);
    TEST("LD H,L", 0x65);
    TEST("LD H,(HL)", 0x66);
    TEST("LD H,A", 0x67);

    TEST("LD L,B", 0x68);
    TEST("LD L,C", 0x69);
    TEST("LD L,D", 0x6A);
    TEST("LD L,E", 0x6B);
    TEST("LD L,H", 0x6C);
    TEST("LD L,L", 0x6D);
    TEST("LD L,(HL)", 0x6E);
    TEST("LD L,A", 0x6F);

    TEST("LD (HL),B", 0x70);
    TEST("LD (HL),C", 0x71);
    TEST("LD (HL),D", 0x72);
    TEST("LD (HL),E", 0x73);
    TEST("LD (HL),H", 0x74);
    TEST("LD (HL),L", 0x75);
    ERRT("LD (HL),(HL)", OPERAND_NOT_ALLOWED, "(HL),(HL)");
    TEST("LD (HL),A", 0x77);

    TEST("LD A,B", 0x78);
    TEST("LD A,C", 0x79);
    TEST("LD A,D", 0x7A);
    TEST("LD A,E", 0x7B);
    TEST("LD A,H", 0x7C);
    TEST("LD A,L", 0x7D);
    TEST("LD A,(HL)", 0x7E);
    TEST("LD A,A", 0x7F);

    TEST("LD (BC),A", 0x02);
    TEST("LD (DE),A", 0x12);
    TEST("LD A,(BC)", 0x0A);
    TEST("LD A,(DE)", 0x1A);

    if (isZ80()) {
        TEST("LD I,A", 0xED, 0x47);
        TEST("LD R,A", 0xED, 0x4F);
        TEST("LD A,I", 0xED, 0x57);
        TEST("LD A,R", 0xED, 0x5F);

        TEST("LDI",  0xED, 0xA0);
        TEST("LDD",  0xED, 0xA8);
        TEST("LDIR", 0xED, 0xB0);
        TEST("LDDR", 0xED, 0xB8);
        TEST("CPI",  0xED, 0xA1);
        TEST("CPD",  0xED, 0xA9);
        TEST("CPIR", 0xED, 0xB1);
        TEST("CPDR", 0xED, 0xB9);
        TEST("INI",  0xED, 0xA2);
        TEST("IND",  0xED, 0xAA);
        TEST("INIR", 0xED, 0xB2);
        TEST("INDR", 0xED, 0xBA);
        TEST("OUTI", 0xED, 0xA3);
        TEST("OUTD", 0xED, 0xAB);
        TEST("OTIR", 0xED, 0xB3);
        TEST("OTDR", 0xED, 0xBB);
    } else {
        ERRT("LD I,A", OPERAND_NOT_ALLOWED, "I,A");
        ERRT("LD R,A", OPERAND_NOT_ALLOWED, "R,A");
        ERRT("LD A,I", OPERAND_NOT_ALLOWED, "A,I");
        ERRT("LD A,R", OPERAND_NOT_ALLOWED, "A,R");

        ERUI("LDI");
        ERUI("LDD");
        ERUI("LDIR");
        ERUI("LDDR");
        ERUI("CPI");
        ERUI("CPD");
        ERUI("CPIR");
        ERUI("CPDR");
        ERUI("INI");
        ERUI("IND");
        ERUI("INIR");
        ERUI("INDR");
        ERUI("OUTI");
        ERUI("OUTD");
        ERUI("OTIR");
        ERUI("OTDR");
    }
}

static void test_move_immediate() {
    TEST("LD B,0F6H", 0x06, 0xF6);
    TEST("LD C,9FH",  0x0E, 0x9F);
    TEST("LD D,3AH",  0x16, 0x3A);
    TEST("LD E,80H",  0x1E, 0x80);
    TEST("LD H,0F6H", 0x26, 0xF6);
    TEST("LD L,0F6H", 0x2E, 0xF6);
    TEST("LD (HL),0F6H", 0x36, 0xF6);
    TEST("LD A,0FEH", 0x3E, 0xFE);
    TEST("LD A,-1",   0x3E, 0xFF);
    TEST("LD A,255",  0x3E, 0xFF);
    ERRT("LD A,256",  OVERFLOW_RANGE, "256",  0x3E, 0x00);
    TEST("LD A,-128", 0x3E, 0x80);
    ERRT("LD A,-129", OVERFLOW_RANGE, "-129", 0x3E, 0x7F);

    TEST("LD BC,0BEEFH", 0x01, 0xEF, 0xBE);
    TEST("LD DE,1234H",  0x11, 0x34, 0x12);
    TEST("LD HL,0BEEFH", 0x21, 0xEF, 0xBE);
    TEST("LD SP,6789H",  0x31, 0x89, 0x67);


    symtab.intern(-1,  "minus_1");
    symtab.intern(255, "max?255");

    TEST("LD B, minus_1", 0x06, 0xFF);
    TEST("LD C, max?255", 0x0E, 0xFF);
}

static void test_move_direct() {
    TEST("LD (9ABCH),A", 0x32, 0xBC, 0x9A);
    TEST("LD A,(1234H)", 0x3A, 0x34, 0x12);

    TEST("LD (0ABCDH),HL", 0x22, 0xCD, 0xAB);
    TEST("LD HL,(5678H)",  0x2A, 0x78, 0x56);

    if (isZ80()) {
        TEST("LD (0ABCDH),BC", 0xED, 0x43, 0xCD, 0xAB);
        TEST("LD (0ABCDH),DE", 0xED, 0x53, 0xCD, 0xAB);
        TEST("LD (0ABCDH),SP", 0xED, 0x73, 0xCD, 0xAB);
        TEST("LD BC,(05678H)", 0xED, 0x4B, 0x78, 0x56);
        TEST("LD DE,(05678H)", 0xED, 0x5B, 0x78, 0x56);
        TEST("LD SP,(05678H)", 0xED, 0x7B, 0x78, 0x56);
    } else {
        ERRT("LD (0ABCDH),BC", OPERAND_NOT_ALLOWED, "(0ABCDH),BC");
        ERRT("LD (0ABCDH),DE", OPERAND_NOT_ALLOWED, "(0ABCDH),DE");
        ERRT("LD (0ABCDH),SP", OPERAND_NOT_ALLOWED, "(0ABCDH),SP");
        ERRT("LD BC,(05678H)", OPERAND_NOT_ALLOWED, "BC,(05678H)");
        ERRT("LD DE,(05678H)", OPERAND_NOT_ALLOWED, "DE,(05678H)");
        ERRT("LD SP,(05678H)", OPERAND_NOT_ALLOWED, "SP,(05678H)");
    }
}

static void test_stack_op() {
    TEST("PUSH BC", 0xC5);
    TEST("PUSH DE", 0xD5);
    TEST("PUSH HL", 0xE5);
    TEST("PUSH AF", 0xF5);
    TEST("POP BC", 0xC1);
    TEST("POP DE", 0xD1);
    TEST("POP HL", 0xE1);
    TEST("POP AF", 0xF1);

    TEST("EX (SP),HL", 0xE3);
    TEST("EX HL,(SP)", 0xE3);
    TEST("JP (HL)",    0xE9);
    TEST("LD SP,HL",   0xF9);
    TEST("ex de,hl",   0xEB);
    TEST("EX HL,DE",   0xEB);

    if (isZ80()) {
        TEST("EX AF,AF'", 0x08);
        TEST("EXX",       0xD9);
    } else {
        ERRT("EX AF,AF'", OPERAND_NOT_ALLOWED, "AF,AF'");
        ERUI("EXX");
    }
}

static void test_jump_call() {
    TEST("JP 1234H",    0xC3, 0x34, 0x12);
    TEST("jp nz,1234h", 0xC2, 0x34, 0x12);
    TEST("JP Z,1234H",  0xCA, 0x34, 0x12);
    TEST("JP NC,1234H", 0xD2, 0x34, 0x12);
    TEST("JP C,1234H",  0xDA, 0x34, 0x12);
    TEST("JP PO,1234H", 0xE2, 0x34, 0x12);
    TEST("JP PE,1234H", 0xEA, 0x34, 0x12);
    TEST("JP P,1234H",  0xF2, 0x34, 0x12);
    TEST("JP M,1234H",  0xFA, 0x34, 0x12);

    TEST("CALL 1234H",    0xCD, 0x34, 0x12);
    TEST("CALL NZ,1234H", 0xC4, 0x34, 0x12);
    TEST("CALL Z,1234H",  0xCC, 0x34, 0x12);
    TEST("CALL NC,1234H", 0xD4, 0x34, 0x12);
    TEST("CALL C,1234H",  0xDC, 0x34, 0x12);
    TEST("CALL PO,1234H", 0xE4, 0x34, 0x12);
    TEST("CALL PE,1234H", 0xEC, 0x34, 0x12);
    TEST("CALL P,1234H",  0xF4, 0x34, 0x12);
    TEST("CALL M,1234H",  0xFC, 0x34, 0x12);

    TEST("RET",    0xC9);
    TEST("RET NZ", 0xC0);
    TEST("RET Z",  0xC8);
    TEST("RET NC", 0xD0);
    TEST("RET C",  0xD8);
    TEST("RET PO", 0xE0);
    TEST("RET PE", 0xE8);
    TEST("RET P",  0xF0);
    TEST("RET M",  0xF8);

    if (isZ80()) {
        TEST("RETN", 0xED, 0x45);
        TEST("RETI", 0xED, 0x4D);

        TEST("IM 0", 0xED, 0x46);
        TEST("IM 1", 0xED, 0x56);
        TEST("IM 2", 0xED, 0x5E);
        ERRT("IM 3",  ILLEGAL_OPERAND, "3",  0xED, 0x46);
        ERRT("IM -1", ILLEGAL_OPERAND, "-1", 0xED, 0x46);
    } else {
        ERUI("RETN");
        ERUI("RETI");
        ERUI("IM 0");
    }
}

static void test_incr_decr() {
    TEST("INC B", 0x04);
    TEST("INC C", 0x0C);
    TEST("INC D", 0x14);
    TEST("INC E", 0x1C);
    TEST("INC H", 0x24);
    TEST("INC L", 0x2C);
    TEST("INC (HL)",0x34);
    TEST("INC A", 0x3C);

    TEST("DEC B", 0x05);
    TEST("DEC C", 0x0D);
    TEST("DEC D", 0x15);
    TEST("DEC E", 0x1D);
    TEST("DEC H", 0x25);
    TEST("DEC L", 0x2D);
    TEST("DEC (HL)",0x35);
    TEST("DEC A", 0x3D);

    TEST("INC BC", 0x03);
    TEST("INC DE", 0x13);
    TEST("INC HL", 0x23);
    TEST("INC SP", 0x33);
    TEST("DEC BC", 0x0B);
    TEST("DEC DE", 0x1B);
    TEST("DEC HL", 0x2B);
    TEST("DEC SP", 0x3B);
}

static void test_alu_register() {
    TEST("ADD A,B", 0x80);
    TEST("ADD A,C", 0x81);
    TEST("ADD A,D", 0x82);
    TEST("ADD A,E", 0x83);
    TEST("ADD A,H", 0x84);
    TEST("ADD A,L", 0x85);
    TEST("ADD A,(HL)", 0x86);
    TEST("ADD A,A", 0x87);

    TEST("ADC A,B", 0x88);
    TEST("ADC A,C", 0x89);
    TEST("ADC A,D", 0x8A);
    TEST("ADC A,E", 0x8B);
    TEST("ADC A,H", 0x8C);
    TEST("ADC A,L", 0x8D);
    TEST("ADC A,(HL)", 0x8E);
    TEST("ADC A,A", 0x8F);

    TEST("SUB A,B", 0x90);
    TEST("SUB A,C", 0x91);
    TEST("SUB A,D", 0x92);
    TEST("SUB A,E", 0x93);
    TEST("SUB A,H", 0x94);
    TEST("SUB A,L", 0x95);
    TEST("SUB A,(HL)", 0x96);
    TEST("SUB A,A", 0x97);
    TEST("SUB B", 0x90);
    TEST("SUB C", 0x91);
    TEST("SUB D", 0x92);
    TEST("SUB E", 0x93);
    TEST("SUB H", 0x94);
    TEST("SUB L", 0x95);
    TEST("SUB (HL)", 0x96);
    TEST("SUB A", 0x97);

    TEST("SBC A,B", 0x98);
    TEST("SBC A,C", 0x99);
    TEST("SBC A,D", 0x9A);
    TEST("SBC A,E", 0x9B);
    TEST("SBC A,H", 0x9C);
    TEST("SBC A,L", 0x9D);
    TEST("SBC A,(HL)", 0x9E);
    TEST("SBC A,A", 0x9F);

    TEST("AND A,B", 0xA0);
    TEST("AND A,C", 0xA1);
    TEST("AND A,D", 0xA2);
    TEST("AND A,E", 0xA3);
    TEST("AND A,H", 0xA4);
    TEST("AND A,L", 0xA5);
    TEST("AND A,(HL)", 0xA6);
    TEST("AND A,A", 0xA7);
    TEST("AND B", 0xA0);
    TEST("AND C", 0xA1);
    TEST("AND D", 0xA2);
    TEST("AND E", 0xA3);
    TEST("AND H", 0xA4);
    TEST("AND L", 0xA5);
    TEST("AND (HL)", 0xA6);
    TEST("AND A", 0xA7);

    TEST("XOR A,B", 0xA8);
    TEST("XOR A,C", 0xA9);
    TEST("XOR A,D", 0xAA);
    TEST("XOR A,E", 0xAB);
    TEST("XOR A,H", 0xAC);
    TEST("XOR A,L", 0xAD);
    TEST("XOR A,(HL)", 0xAE);
    TEST("XOR A,A", 0xAF);
    TEST("XOR B", 0xA8);
    TEST("XOR C", 0xA9);
    TEST("XOR D", 0xAA);
    TEST("XOR E", 0xAB);
    TEST("XOR H", 0xAC);
    TEST("XOR L", 0xAD);
    TEST("XOR (HL)", 0xAE);
    TEST("XOR A", 0xAF);

    TEST("OR A,B", 0xB0);
    TEST("OR A,C", 0xB1);
    TEST("OR A,D", 0xB2);
    TEST("OR A,E", 0xB3);
    TEST("OR A,H", 0xB4);
    TEST("OR A,L", 0xB5);
    TEST("OR A,(HL)", 0xB6);
    TEST("OR A,A", 0xB7);
    TEST("OR B", 0xB0);
    TEST("OR C", 0xB1);
    TEST("OR D", 0xB2);
    TEST("OR E", 0xB3);
    TEST("OR H", 0xB4);
    TEST("OR L", 0xB5);
    TEST("OR (HL)", 0xB6);
    TEST("OR A", 0xB7);

    TEST("CP A,B", 0xB8);
    TEST("CP A,C", 0xB9);
    TEST("CP A,D", 0xBA);
    TEST("CP A,E", 0xBB);
    TEST("CP A,H", 0xBC);
    TEST("CP A,L", 0xBD);
    TEST("CP A,(HL)", 0xBE);
    TEST("CP A,A", 0xBF);
    TEST("CP B", 0xB8);
    TEST("CP C", 0xB9);
    TEST("CP D", 0xBA);
    TEST("CP E", 0xBB);
    TEST("CP H", 0xBC);
    TEST("CP L", 0xBD);
    TEST("CP (HL)", 0xBE);
    TEST("CP A", 0xBF);

    TEST("ADD HL,BC", 0x09);
    TEST("ADD HL,DE", 0x19);
    TEST("ADD HL,HL", 0x29);
    TEST("ADD HL,SP", 0x39);

    if (isZ80()) {
        TEST("ADC HL,BC", 0xED, 0x4A);
        TEST("ADC HL,DE", 0xED, 0x5A);
        TEST("ADC HL,HL", 0xED, 0x6A);
        TEST("ADC HL,SP", 0xED, 0x7A);
        TEST("SBC HL,BC", 0xED, 0x42);
        TEST("SBC HL,DE", 0xED, 0x52);
        TEST("SBC HL,HL", 0xED, 0x62);
        TEST("SBC HL,SP", 0xED, 0x72);
    } else {
        ERRT("ADC HL,BC", OPERAND_NOT_ALLOWED, "HL,BC");
        ERRT("ADC HL,DE", OPERAND_NOT_ALLOWED, "HL,DE");
        ERRT("ADC HL,HL", OPERAND_NOT_ALLOWED, "HL,HL");
        ERRT("ADC HL,SP", OPERAND_NOT_ALLOWED, "HL,SP");
        ERRT("SBC HL,BC", OPERAND_NOT_ALLOWED, "HL,BC");
        ERRT("SBC HL,DE", OPERAND_NOT_ALLOWED, "HL,DE");
        ERRT("SBC HL,HL", OPERAND_NOT_ALLOWED, "HL,HL");
        ERRT("SBC HL,SP", OPERAND_NOT_ALLOWED, "HL,SP");
    }
}

static void test_alu_immediate() {
    TEST("ADD A,10B",  0xC6, 0x02);
    TEST("ADC A,255",  0xCE, 0xFF);
    TEST("SUB A,-2",   0xD6, 0xFE);
    TEST("SBC A,177O", 0xDE, 0x7F);
    TEST("AND A,~0FH", 0xE6, 0xF0);
    TEST("XOR A,~001B",0xEE, 0xFE);
    TEST("OR  A,+127", 0xF6, 0x7F);
    TEST("CP  A,-128", 0xFE, 0x80);
    TEST("SUB -2",   0xD6, 0xFE);
    TEST("AND ~0FH", 0xE6, 0xF0);
    TEST("XOR ~001B",0xEE, 0xFE);
    TEST("OR  +127", 0xF6, 0x7F);
    TEST("CP  -128", 0xFE, 0x80);
}

static void test_io() {
    TEST("OUT (0F1H),A", 0xD3, 0xF1);
    TEST("IN A,(0F0H)",  0xDB, 0xF0);
    ERRT("IN A,(-1)",    OVERFLOW_RANGE, "(-1)",  0xDB, 0xFF);
    ERRT("IN A,(256)",   OVERFLOW_RANGE, "(256)", 0xDB, 0x00);

    if (isZ80()) {
        TEST("IN B,(C)", 0xED, 0x40);
        TEST("IN C,(C)", 0xED, 0x48);
        TEST("IN D,(C)", 0xED, 0x50);
        TEST("IN E,(C)", 0xED, 0x58);
        TEST("IN H,(C)", 0xED, 0x60);
        TEST("IN L,(C)", 0xED, 0x68);
        ERRT("IN (HL),(C)", OPERAND_NOT_ALLOWED, "(HL),(C)");
        TEST("IN A,(C)", 0xED, 0x78);

        TEST("OUT (C),B", 0xED, 0x41);
        TEST("OUT (C),C", 0xED, 0x49);
        TEST("OUT (C),D", 0xED, 0x51);
        TEST("OUT (C),E", 0xED, 0x59);
        TEST("OUT (C),H", 0xED, 0x61);
        TEST("OUT (C),L", 0xED, 0x69);
        ERRT("OUT (C),(HL)", OPERAND_NOT_ALLOWED, "(C),(HL)");
        TEST("OUT (C),A", 0xED, 0x79);
    } else {
        ERRT("IN  B,(C)", OPERAND_NOT_ALLOWED, "B,(C)");
        ERRT("OUT (C),A", OPERAND_NOT_ALLOWED, "(C),A");
    }
}

static void test_inherent() {
    TEST("DI", 0xF3);
    TEST("EI", 0xFB);

    TEST("NOP",  0x00);
    TEST("HALT", 0x76);

    TEST("RLCA", 0x07);
    TEST("RRCA", 0x0F);
    TEST("RLA", 0x17);
    TEST("RRA", 0x1F);

    TEST("DAA", 0x27);
    TEST("CPL", 0x2F);
    TEST("SCF", 0x37);
    TEST("CCF", 0x3F);

    if (is8085()) {
        TEST("LD A,IM", 0x20);
        TEST("LD IM,A", 0x30);
        ERUI("RETEM");
        ERUI("CALLN 40H");
    } else if (v30emu()) {
        ERRT("LD A,IM", OPERAND_NOT_ALLOWED, "A,IM");
        ERRT("LD IM,A", OPERAND_NOT_ALLOWED, "IM,A");
        TEST("RETEM",     0xED, 0xFD);
        TEST("CALLN 40H", 0xED, 0xED, 0x40);
    } else {
        ERRT("LD A,IM", OPERAND_NOT_ALLOWED, "A,IM");
        ERRT("LD IM,A", OPERAND_NOT_ALLOWED, "IM,A");
        ERUI("RETEM");
        ERUI("CALLN 40H");
    }

    if (isZ80()) {
        TEST("NEG", 0xED, 0x44);
    } else {
        ERUI("NEG");
    }
}

static void test_restart() {
    TEST("RST 00H", 0xC7);
    TEST("RST 08H", 0xCF);
    TEST("RST 10H", 0xD7);
    TEST("RST 18H", 0xDF);
    TEST("RST 20H", 0xE7);
    TEST("RST 28H", 0xEF);
    TEST("RST 30H", 0xF7);
    TEST("RST 38H", 0xFF);
    ERRT("RST 39H", ILLEGAL_OPERAND, "39H", 0xC7);
    ERRT("RST 40H", ILLEGAL_OPERAND, "40H", 0xC7);
    ERRT("RST -1",  ILLEGAL_OPERAND, "-1",  0xC7);
}

static void test_relative() {
    if (isZ80()) {
        ATEST(0x1000, "DJNZ 1000H",  0x10, 0xFE);
        ATEST(0x1000, "DJNZ 1081H",  0x10, 0x7F);
        ATEST(0x1000, "DJNZ 0F82H",  0x10, 0x80);
        AERRT(0x1000, "DJNZ 1082H",  OPERAND_TOO_FAR, "1082H", 0x10, 0x80);
        AERRT(0x1000, "DJNZ 0F81H",  OPERAND_TOO_FAR, "0F81H", 0x10, 0x7F);
        ATEST(0x1000, "JR 1000H",    0x18, 0xFE);
        ATEST(0x1000, "JR NZ,1004H", 0x20, 0x02);
        ATEST(0x1000, "JR Z,1081H",  0x28, 0x7F);
        ATEST(0x1000, "JR NC,0F82H", 0x30, 0x80);
        ATEST(0x1000, "JR C,0F82H",  0x38, 0x80);
        ATEST(0x1000, "JR C,1081H",  0x38, 0x7F);
        ATEST(0x1000, "JR C,0F82H",  0x38, 0x80);
        AERRT(0x1000, "JR C,1082H",  OPERAND_TOO_FAR, "1082H", 0x38, 0x80);
        AERRT(0x1000, "JR C,0F81H",  OPERAND_TOO_FAR, "0F81H", 0x38, 0x7F);
    } else {
        ERUI("DJNZ 1000H");
        ERUI("JR 1000H");
        ERUI("JR NZ,1004H");
    }
}

static void test_shift() {
    if (isZ80()) {
        TEST("RLC B", 0xCB, 0x00);
        TEST("RLC C", 0xCB, 0x01);
        TEST("RLC D", 0xCB, 0x02);
        TEST("RLC E", 0xCB, 0x03);
        TEST("RLC H", 0xCB, 0x04);
        TEST("RLC L", 0xCB, 0x05);
        TEST("RLC (HL)", 0xCB, 0x06);
        TEST("RLC A", 0xCB, 0x07);

        TEST("RRC B", 0xCB, 0x08);
        TEST("RRC C", 0xCB, 0x09);
        TEST("RRC D", 0xCB, 0x0A);
        TEST("RRC E", 0xCB, 0x0B);
        TEST("RRC H", 0xCB, 0x0C);
        TEST("RRC L", 0xCB, 0x0D);
        TEST("RRC (HL)", 0xCB, 0x0E);
        TEST("RRC A", 0xCB, 0x0F);

        TEST("RL B", 0xCB, 0x10);
        TEST("RL C", 0xCB, 0x11);
        TEST("RL D", 0xCB, 0x12);
        TEST("RL E", 0xCB, 0x13);
        TEST("RL H", 0xCB, 0x14);
        TEST("RL L", 0xCB, 0x15);
        TEST("RL (HL)", 0xCB, 0x16);
        TEST("RL A", 0xCB, 0x17);

        TEST("RR B", 0xCB, 0x18);
        TEST("RR C", 0xCB, 0x19);
        TEST("RR D", 0xCB, 0x1A);
        TEST("RR E", 0xCB, 0x1B);
        TEST("RR H", 0xCB, 0x1C);
        TEST("RR L", 0xCB, 0x1D);
        TEST("RR (HL)", 0xCB, 0x1E);
        TEST("RR A", 0xCB, 0x1F);

        TEST("SLA B", 0xCB, 0x20);
        TEST("SLA C", 0xCB, 0x21);
        TEST("SLA D", 0xCB, 0x22);
        TEST("SLA E", 0xCB, 0x23);
        TEST("SLA H", 0xCB, 0x24);
        TEST("SLA L", 0xCB, 0x25);
        TEST("SLA (HL)", 0xCB, 0x26);
        TEST("SLA A", 0xCB, 0x27);

        TEST("SRA B", 0xCB, 0x28);
        TEST("SRA C", 0xCB, 0x29);
        TEST("SRA D", 0xCB, 0x2A);
        TEST("SRA E", 0xCB, 0x2B);
        TEST("SRA H", 0xCB, 0x2C);
        TEST("SRA L", 0xCB, 0x2D);
        TEST("SRA (HL)", 0xCB, 0x2E);
        TEST("SRA A", 0xCB, 0x2F);

        TEST("SRL B", 0xCB, 0x38);
        TEST("SRL C", 0xCB, 0x39);
        TEST("SRL D", 0xCB, 0x3A);
        TEST("SRL E", 0xCB, 0x3B);
        TEST("SRL H", 0xCB, 0x3C);
        TEST("SRL L", 0xCB, 0x3D);
        TEST("SRL (HL)", 0xCB, 0x3E);
        TEST("SRL A", 0xCB, 0x3F);

        TEST("RRD", 0xED, 0x67);
        TEST("RLD", 0xED, 0x6F);
    } else {
        ERUI("RLC A");
        ERUI("RRC B");
        ERUI("RL  C");
        ERUI("RR  D");
        ERUI("SLA E");
        ERUI("SRA H");
        ERUI("SRL L");
        ERUI("RLD");
    }
}

static void test_bitop() {
    if (isZ80()) {
        TEST("BIT 0,B", 0xCB, 0x40);
        TEST("BIT 1,C", 0xCB, 0x49);
        TEST("BIT 2,D", 0xCB, 0x52);
        TEST("BIT 3,E", 0xCB, 0x5B);
        TEST("BIT 4,H", 0xCB, 0x64);
        TEST("BIT 5,L", 0xCB, 0x6D);
        TEST("BIT 6,(HL)", 0xCB, 0x76);
        TEST("BIT 7,A", 0xCB, 0x7F);
        ERRT("BIT 8,A",  ILLEGAL_BIT_NUMBER, "8,A",  0xCB, 0x47);
        ERRT("BIT -1,A", ILLEGAL_BIT_NUMBER, "-1,A", 0xCB, 0x7F);

        TEST("RES 0,B", 0xCB, 0x80);
        TEST("RES 1,C", 0xCB, 0x89);
        TEST("RES 2,D", 0xCB, 0x92);
        TEST("RES 3,E", 0xCB, 0x9B);
        TEST("RES 4,H", 0xCB, 0xA4);
        TEST("RES 5,L", 0xCB, 0xAD);
        TEST("RES 6,(HL)", 0xCB, 0xB6);
        TEST("RES 7,A", 0xCB, 0xBF);

        TEST("SET 0,B", 0xCB, 0xC0);
        TEST("SET 1,C", 0xCB, 0xC9);
        TEST("SET 2,D", 0xCB, 0xD2);
        TEST("SET 3,E", 0xCB, 0xDB);
        TEST("SET 4,H", 0xCB, 0xE4);
        TEST("SET 5,L", 0xCB, 0xED);
        TEST("SET 6,(HL)", 0xCB, 0xF6);
        TEST("SET 7,A", 0xCB, 0xFF);
    } else {
        ERUI("BIT 0,B");
        ERUI("RES 1,C");
        ERUI("SET 2,D");
    }
}

static void test_index_registers() {
    if (isZ80()) {
        TEST("ADD IX,BC", 0xDD, 0x09);
        TEST("ADD IX,DE", 0xDD, 0x19);
        TEST("ADD IX,IX", 0xDD, 0x29);
        TEST("ADD IX,SP", 0xDD, 0x39);
        TEST("LD IX,0ABCDH",   0xDD, 0x21, 0xCD, 0xAB);
        TEST("LD (0ABCDH),IX", 0xDD, 0x22, 0xCD, 0xAB);
        TEST("LD IX,(5678H)",  0xDD, 0x2A, 0x78, 0x56);
        TEST("INC IX",  0xDD, 0x23);
        TEST("DEC IX",  0xDD, 0x2B);
        TEST("POP IX",  0xDD, 0xE1);
        TEST("PUSH IX", 0xDD, 0xE5);
        TEST("EX (SP),IX", 0xDD, 0xE3);
        TEST("JP (IX)",    0xDD, 0xE9);
        TEST("LD SP,IX",   0xDD, 0xF9);

        TEST("ADD IY,BC", 0xFD, 0x09);
        TEST("ADD IY,DE", 0xFD, 0x19);
        TEST("ADD IY,IY", 0xFD, 0x29);
        TEST("ADD IY,SP", 0xFD, 0x39);
        TEST("LD IY,0ABCDH",   0xFD, 0x21, 0xCD, 0xAB);
        TEST("LD (0ABCDH),IY", 0xFD, 0x22, 0xCD, 0xAB);
        TEST("LD IY,(5678H)",  0xFD, 0x2A, 0x78, 0x56);
        TEST("INC IY",  0xFD, 0x23);
        TEST("DEC IY",  0xFD, 0x2B);
        TEST("POP IY",  0xFD, 0xE1);
        TEST("PUSH IY", 0xFD, 0xE5);
        TEST("EX (SP),IY", 0xFD, 0xE3);
        TEST("JP (IY)",    0xFD, 0xE9);
        TEST("LD SP,IY",   0xFD, 0xF9);
    } else {
        ERRT("ADD IX,BC", OPERAND_NOT_ALLOWED, "IX,BC");
        ERRT("ADD IX,DE", OPERAND_NOT_ALLOWED, "IX,DE");
        ERRT("ADD IX,IX", OPERAND_NOT_ALLOWED, "IX,IX");
        ERRT("ADD IX,SP", OPERAND_NOT_ALLOWED, "IX,SP");
        ERRT("LD (0ABCDH),IX", OPERAND_NOT_ALLOWED, "(0ABCDH),IX");
        ERRT("LD IX,(5678H)",  OPERAND_NOT_ALLOWED, "IX,(5678H)");
        ERRT("INC IX",  OPERAND_NOT_ALLOWED, "IX");
        ERRT("DEC IX",  OPERAND_NOT_ALLOWED, "IX");
        ERRT("POP IX",  OPERAND_NOT_ALLOWED, "IX");
        ERRT("PUSH IX", OPERAND_NOT_ALLOWED, "IX");
        ERRT("EX (SP),IX", OPERAND_NOT_ALLOWED, "(SP),IX");
        ERRT("JP (IX)",  OPERAND_NOT_ALLOWED, "(IX)");
        ERRT("LD SP,IX", OPERAND_NOT_ALLOWED, "SP,IX");
        ERRT("ADD IY,BC", OPERAND_NOT_ALLOWED, "IY,BC");
        ERRT("ADD IY,DE", OPERAND_NOT_ALLOWED, "IY,DE");
        ERRT("ADD IY,IY", OPERAND_NOT_ALLOWED, "IY,IY");
        ERRT("ADD IY,SP", OPERAND_NOT_ALLOWED, "IY,SP");
        ERRT("LD (0ABCDH),IY", OPERAND_NOT_ALLOWED, "(0ABCDH),IY");
        ERRT("LD IY,(5678H)",  OPERAND_NOT_ALLOWED, "IY,(5678H)");
        ERRT("INC IY",  OPERAND_NOT_ALLOWED, "IY");
        ERRT("DEC IY",  OPERAND_NOT_ALLOWED, "IY");
        ERRT("POP IY",  OPERAND_NOT_ALLOWED, "IY");
        ERRT("PUSH IY", OPERAND_NOT_ALLOWED, "IY");
        ERRT("EX (SP),IY", OPERAND_NOT_ALLOWED, "(SP),IY");
        ERRT("JP (IY)",  OPERAND_NOT_ALLOWED, "(IY)");
        ERRT("LD SP,IY", OPERAND_NOT_ALLOWED, "SP,IY");
    }
}

static void test_indexed() {
    if (isZ80()) {
        TEST("INC (IX+2)",   0xDD, 0x34, 0x02);
        TEST("DEC (IX+2)",   0xDD, 0x35, 0x02);
        ERRT("INC (IX+128)", OVERFLOW_RANGE, "(IX+128)", 0xDD, 0x34, 0x80);
        ERRT("DEC (IX-129)", OVERFLOW_RANGE, "(IX-129)", 0xDD, 0x35, 0x7F);

        TEST("LD B,(IX+2)", 0xDD, 0x46, 0x02);
        TEST("LD C,(IX+2)", 0xDD, 0x4E, 0x02);
        TEST("LD D,(IX+2)", 0xDD, 0x56, 0x02);
        TEST("LD E,(IX+2)", 0xDD, 0x5E, 0x02);
        TEST("LD H,(IX+2)", 0xDD, 0x66, 0x02);
        TEST("LD L,(IX+2)", 0xDD, 0x6E, 0x02);
        TEST("LD A,(IX+2)", 0xDD, 0x7E, 0x02);
        ERRT("LD (HL),(IX+2)", OPERAND_NOT_ALLOWED, "(HL),(IX+2)");

        TEST("LD (IX+2),B", 0xDD, 0x70, 0x02);
        TEST("LD (IX+2),C", 0xDD, 0x71, 0x02);
        TEST("LD (IX+2),D", 0xDD, 0x72, 0x02);
        TEST("LD (IX+0),E", 0xDD, 0x73, 0x00);
        TEST("LD (IX+2),H", 0xDD, 0x74, 0x02);
        TEST("LD (IX+2),L", 0xDD, 0x75, 0x02);
        TEST("LD (IX+2),A", 0xDD, 0x77, 0x02);
        ERRT("LD (IX+2),(HL)", OPERAND_NOT_ALLOWED, "(IX+2),(HL)");

        TEST("LD (IX+2),0F6H", 0xDD, 0x36, 0x02, 0xF6);

        TEST("ADD A,(IX+2)", 0xDD, 0x86, 0x02);
        TEST("ADC A,(IX+2)", 0xDD, 0x8E, 0x02);
        TEST("SUB A,(IX+2)", 0xDD, 0x96, 0x02);
        TEST("SBC A,(IX+2)", 0xDD, 0x9E, 0x02);
        TEST("AND A,(IX+2)", 0xDD, 0xA6, 0x02);
        TEST("XOR A,(IX+2)", 0xDD, 0xAE, 0x02);
        TEST("OR  A,(IX+2)", 0xDD, 0xB6, 0x02);
        TEST("CP  A,(IX+2)", 0xDD, 0xBE, 0x02);
        TEST("SUB (IX+2)", 0xDD, 0x96, 0x02);
        TEST("AND (IX+2)", 0xDD, 0xA6, 0x02);
        TEST("XOR (IX+2)", 0xDD, 0xAE, 0x02);
        TEST("OR  (IX+2)", 0xDD, 0xB6, 0x02);
        TEST("CP  (IX+2)", 0xDD, 0xBE, 0x02);

        TEST("INC (IY-2)",0xFD, 0x34, 0xFE);
        TEST("DEC (IY-2)",0xFD, 0x35, 0xFE);

        TEST("LD B,(IY-2)", 0xFD, 0x46, 0xFE);
        TEST("LD C,(IY-2)", 0xFD, 0x4E, 0xFE);
        TEST("LD D,(IY-2)", 0xFD, 0x56, 0xFE);
        TEST("LD E,(IY-2)", 0xFD, 0x5E, 0xFE);
        TEST("LD H,(IY-2)", 0xFD, 0x66, 0xFE);
        TEST("LD L,(IY-2)", 0xFD, 0x6E, 0xFE);
        TEST("LD A,(IY-2)", 0xFD, 0x7E, 0xFE);

        TEST("LD (IY-2),B", 0xFD, 0x70, 0xFE);
        TEST("LD (IY-2),C", 0xFD, 0x71, 0xFE);
        TEST("LD (IY-2),D", 0xFD, 0x72, 0xFE);
        TEST("LD (IY-2),E", 0xFD, 0x73, 0xFE);
        TEST("LD (IY-2),H", 0xFD, 0x74, 0xFE);
        TEST("LD (IY-2),L", 0xFD, 0x75, 0xFE);
        TEST("LD (IY-2),A", 0xFD, 0x77, 0xFE);

        TEST("LD (IY-2),0F6H", 0xFD, 0x36, 0xFE, 0xF6);

        TEST("ADD A,(IY-2)", 0xFD, 0x86, 0xFE);
        TEST("ADC A,(IY-2)", 0xFD, 0x8E, 0xFE);
        TEST("SUB A,(IY-2)", 0xFD, 0x96, 0xFE);
        TEST("SBC A,(IY-2)", 0xFD, 0x9E, 0xFE);
        TEST("AND A,(IY-2)", 0xFD, 0xA6, 0xFE);
        TEST("XOR A,(IY-2)", 0xFD, 0xAE, 0xFE);
        TEST("OR  A,(IY-2)", 0xFD, 0xB6, 0xFE);
        TEST("CP  A,(IY-2)", 0xFD, 0xBE, 0xFE);
        TEST("SUB (IY-2)", 0xFD, 0x96, 0xFE);
        TEST("AND (IY-2)", 0xFD, 0xA6, 0xFE);
        TEST("XOR (IY-2)", 0xFD, 0xAE, 0xFE);
        TEST("OR  (IY-2)", 0xFD, 0xB6, 0xFE);
        TEST("CP  (IY-2)", 0xFD, 0xBE, 0xFE);
    } else {
        ERRT("INC (IX+2)", OPERAND_NOT_ALLOWED, "(IX+2)");
        ERRT("INC (IY-2)", OPERAND_NOT_ALLOWED, "(IY-2)");
        ERRT("DEC (IX+2)", OPERAND_NOT_ALLOWED, "(IX+2)");
        ERRT("DEC (IY-2)", OPERAND_NOT_ALLOWED, "(IY-2)");
        ERRT("LD B,(IX+2)", OPERAND_NOT_ALLOWED, "B,(IX+2)");
        ERRT("LD C,(IY-2)", OPERAND_NOT_ALLOWED, "C,(IY-2)");
        ERRT("LD (IX+2),D", OPERAND_NOT_ALLOWED, "(IX+2),D");
        ERRT("LD (IY-2),E", OPERAND_NOT_ALLOWED, "(IY-2),E");
        ERRT("ADD A,(IX+2)", OPERAND_NOT_ALLOWED, "A,(IX+2)");
        ERRT("ADC A,(IY-2)", OPERAND_NOT_ALLOWED, "A,(IY-2)");
        ERRT("AND (IX+2)", OPERAND_NOT_ALLOWED, "(IX+2)");
        ERRT("SUB (IY-2)", OPERAND_NOT_ALLOWED, "(IY-2)");
    }
}

static void test_shift_indexed() {
    if (isZ80()) {
        TEST("RLC (IX+127)", 0xDD, 0xCB, 0x7F, 0x06);
        TEST("RRC (IX+127)", 0xDD, 0xCB, 0x7F, 0x0E);
        TEST("RL  (IX+127)", 0xDD, 0xCB, 0x7F, 0x16);
        TEST("RR  (IX+127)", 0xDD, 0xCB, 0x7F, 0x1E);
        TEST("SLA (IX+127)", 0xDD, 0xCB, 0x7F, 0x26);
        TEST("SRA (IX+127)", 0xDD, 0xCB, 0x7F, 0x2E);
        TEST("SRL (IX+127)", 0xDD, 0xCB, 0x7F, 0x3E);
        TEST("SRL (IX-128)", 0xDD, 0xCB, 0x80, 0x3E);
        ERRT("SRL (IX+128)", OVERFLOW_RANGE, "(IX+128)", 0xDD, 0xCB, 0x80, 0x3E);
        ERRT("SRL (IX-129)", OVERFLOW_RANGE, "(IX-129)", 0xDD, 0xCB, 0x7F, 0x3E);

        TEST("RLC (IY-128)", 0xFD, 0xCB, 0x80, 0x06);
        TEST("RRC (IY-128)", 0xFD, 0xCB, 0x80, 0x0E);
        TEST("RL  (IY-128)", 0xFD, 0xCB, 0x80, 0x16);
        TEST("RR  (IY-128)", 0xFD, 0xCB, 0x80, 0x1E);
        TEST("SLA (IY-128)", 0xFD, 0xCB, 0x80, 0x26);
        TEST("SRA (IY-128)", 0xFD, 0xCB, 0x80, 0x2E);
        TEST("SRL (IY-128)", 0xFD, 0xCB, 0x80, 0x3E);
    } else {
        ERUI("RLC (IY-128)");
        ERUI("RRC (IY-128)");
        ERUI("RL  (IY-128)");
        ERUI("RR  (IY-128)");
        ERUI("SLA (IY-128)");
        ERUI("SRA (IY-128)");
        ERUI("SRL (IY-128)");
    }
}

static void test_bitop_indexed() {
    if (isZ80()) {
        TEST("BIT 0,(IX-128)", 0xDD, 0xCB, 0x80, 0x46);
        TEST("RES 1,(IX-128)", 0xDD, 0xCB, 0x80, 0x8E);
        TEST("SET 2,(IX-128)", 0xDD, 0xCB, 0x80, 0xD6);
        ERRT("SET 8,(IX-128)", ILLEGAL_BIT_NUMBER, "8,(IX-128)", 0xDD, 0xCB, 0x80, 0xC6);
        ERRT("SET -1,(IX+1)",  ILLEGAL_BIT_NUMBER, "-1,(IX+1)",  0xDD, 0xCB, 0x01, 0xFE);
        ERRT("SET 2,(IX-129)", OVERFLOW_RANGE, "(IX-129)",       0xDD, 0xCB, 0x7F, 0xD6);
        ERRT("SET 2,(IX+128)", OVERFLOW_RANGE, "(IX+128)",       0xDD, 0xCB, 0x80, 0xD6);

        TEST("BIT 5,(IY+127)", 0xFD, 0xCB, 0x7F, 0x6E);
        TEST("RES 6,(IY+127)", 0xFD, 0xCB, 0x7F, 0xB6);
        TEST("SET 7,(IY+127)", 0xFD, 0xCB, 0x7F, 0xFE);
    } else {
        ERUI("BIT 0,(IX-128)");
        ERUI("RES 1,(IX-128)");
        ERUI("SET 2,(IX-128)");
    }
}

static void test_comment() {
    COMM("RST 18H        ; comment", "; comment", 0xDF);
    COMM("LD B , B       ; comment", "; comment", 0x40);
    COMM("NOP            ; comment", "; comment", 0x00);
    COMM("LD BC , 0BEEFH ; comment", "; comment", 0x01, 0xEF, 0xBE);
    COMM("LD HL , ( 5678H ) ; comment", "; comment", 0x2A, 0x78, 0x56);
    COMM("CALL C , 1234H ; comment", "; comment", 0xDC, 0x34, 0x12);
    COMM("RET         ; comment", "; comment", 0xC9);
    COMM("RET NZ      ; comment", "; comment", 0xC0);

    if (isZ80()) {
        COMM("IM 2           ; comment", "; comment", 0xED, 0x5E);
        COMM("BIT 0 , B      ; comment", "; comment", 0xCB, 0x40);
        COMM("BIT 6 , ( HL ) ; comment", "; comment", 0xCB, 0x76);
        COMM("INC ( IX + 2 ) ; comment", "; comment", 0xDD, 0x34, 0x02);
        COMM("LD ( IY - 2 ) , B  ; comment", "; comment", 0xFD, 0x70, 0xFE);
        COMM("ADD A , ( IY - 2 ) ; comment", "; comment", 0xFD, 0x86, 0xFE);
        COMM("EX AF , AF'        ; comment", "; comment", 0x08);
        COMM("LD ( 0ABCDH ) , IX ; comment", "; comment", 0xDD, 0x22, 0xCD, 0xAB);
        COMM("LD IX , ( 5678H )  ; comment", "; comment", 0xDD, 0x2A, 0x78, 0x56);
        COMM("EX ( SP ) , IX     ; comment", "; comment", 0xDD, 0xE3);
        COMM("JP ( IX )          ; comment", "; comment", 0xDD, 0xE9);

        ACOMM(0x1000, "JR 1000H     ; comment", "; comment", 0x18, 0xFE);
        ACOMM(0x1000, "JR NZ , 1004H; comment", "; comment", 0x20, 0x02);
    }
}

static void test_undefined_symbol() {
    ERUS("LD  B,UNDEF",   "UNDEF",     0x06, 0x00);
    ERUS("LD BC,UNDEF",   "UNDEF",     0x01, 0x00, 0x00);
    ERUS("LD (UNDEF),A",  "UNDEF),A",  0x32, 0x00, 0x00);
    ERUS("LD A,(UNDEF)",  "UNDEF)",    0x3A, 0x00, 0x00);
    ERUS("LD (UNDEF),HL", "UNDEF),HL", 0x22, 0x00, 0x00);
    ERUS("LD HL,(UNDEF)", "UNDEF)",    0x2A, 0x00, 0x00);
    ERUS("JP    UNDEF",   "UNDEF",     0xC3, 0x00, 0x00);
    ERUS("JP NZ,UNDEF",   "UNDEF",     0xC2, 0x00, 0x00);
    ERUS("CALL    UNDEF", "UNDEF",     0xCD, 0x00, 0x00);
    ERUS("CALL NZ,UNDEF", "UNDEF",     0xC4, 0x00, 0x00);
    ERUS("ADD  A,UNDEF",  "UNDEF",     0xC6, 0x00);
    ERUS("OUT (UNDEF),A", "UNDEF),A",  0xD3, 0x00);
    ERUS("IN  A,(UNDEF)", "UNDEF)",    0xDB, 0x00);
    ERUS("RST UNDEF",     "UNDEF",     0xC7);

    if (isZ80()) {
        ERUS("LD (UNDEF),BC",        "UNDEF),BC",      0xED, 0x43, 0x00, 0x00);
        ERUS("LD BC,(UNDEF)",        "UNDEF)",         0xED, 0x4B, 0x00, 0x00);
        ERUS("IM UNDEF",             "UNDEF",          0xED, 0x46);
        ERUS("BIT UNDEF,B",          "UNDEF,B",        0xCB, 0x40);
        ERUS("LD (UNDEF),IY",        "UNDEF),IY",      0xFD, 0x22, 0x00, 0x00);
        ERUS("LD IY,(UNDEF)",        "UNDEF)",         0xFD, 0x2A, 0x00, 0x00);
        ERUS("INC (IX+UNDEF)",       "UNDEF)",         0xDD, 0x34, 0x00);
        ERUS("LD (IY-UNDEF),B",      "UNDEF),B",       0xFD, 0x70, 0x00);
        ERUS("LD (IY-2),UNDEF",      "UNDEF",          0xFD, 0x36, 0xFE, 0x00);
        ERUS("RLC (IX+UNDEF)",       "UNDEF)",         0xDD, 0xCB, 0x00, 0x06);
        ERUS("BIT UNDEF,(IX-128)",   "UNDEF,(IX-128)", 0xDD, 0xCB, 0x80, 0x46);
        ERUS("RES 7,(IX-UNDEF)",     "UNDEF)",         0xDD, 0xCB, 0x00, 0xBE);
        ERUS("SET UNDEF,(IX-UNDEF)", "UNDEF,(IX-UNDEF)", 0xDD, 0xCB, 0x00, 0xC6);

        AERUS(0x1000, "JR UNDEF",    "UNDEF", 0x18, 0x00);
        AERUS(0x1000, "JR NZ,UNDEF", "UNDEF", 0x20, 0x00);
        AERUS(0x1000, "DJNZ UNDEF",  "UNDEF", 0x10, 0x00);
    }

    COMM("DB -128, 255 ; comment", "; comment", 0x80, 0xFF);
    COMM("DB 'TEXT'    ; comment", "; comment", 0x54, 0x45, 0x58, 0x54);
    COMM("DW -128, 255 ; comment", "; comment", 0x80, 0xFF, 0xFF, 0x00);
    COMM("DL 12345678H ; comment", "; comment", 0x78, 0x56, 0x34, 0x12);
    COMM("DEFB -128, 255 ; comment", "; comment", 0x80, 0xFF);
    COMM("DEFB 'TEXT'    ; comment", "; comment", 0x54, 0x45, 0x58, 0x54);
    COMM("DEFM 'TEXT'    ; comment", "; comment", 0x54, 0x45, 0x58, 0x54);
    COMM("DEFW -128, 255 ; comment", "; comment", 0x80, 0xFF, 0xFF, 0x00);
}

static void test_data_constant() {
    TEST("DB -128, 255", 0x80, 0xFF);
    TEST(R"(DB 'A', '"')", 0x41, 0x22);
    TEST("DB '9'-'0'",   0x09);
    TEST("DB ''''",      0x27);
    ERRT("DB '''",       MISSING_CLOSING_QUOTE, "'''");
    TEST("DB 'A''B',0",  0x41, 0x27, 0x42, 0x00);
    ERRT("DB 'A''B,0",   MISSING_CLOSING_QUOTE, "'A''B,0");
    TEST("DW -128, 255", 0x80, 0xFF, 0xFF, 0x00);
    TEST("DW 'A''B'",    0x41, 0x27, 0x42, 0x00);
    ERRT("DW 'A''B",     MISSING_CLOSING_QUOTE, "'A''B");
    TEST("DL 12345678H", 0x78, 0x56, 0x34, 0x12);
    TEST(R"(DL 'A''B"C')", 0x41, 0x27, 0x42, 0x22, 0x43, 0x00, 0x00, 0x00);
    ERRT(R"(DL 'A''B"C)",  MISSING_CLOSING_QUOTE, R"('A''B"C)");
    TEST("DEFB -128, 255", 0x80, 0xFF);
    TEST(R"(DEFB 'A', '"')", 0x41, 0x22);
    TEST("DEFB '9'-'0'",   0x09);
    TEST("DEFB ''''",      0x27);
    ERRT("DEFB '''",       MISSING_CLOSING_QUOTE, "'''");
    TEST("DEFB 'A''B',0",  0x41, 0x27, 0x42, 0x00);
    ERRT("DEFB 'A''B,0",   MISSING_CLOSING_QUOTE, "'A''B,0");
    TEST("DEFW -128, 255", 0x80, 0xFF, 0xFF, 0x00);
    TEST("DEFW 'A''B'",    0x41, 0x27, 0x42, 0x00);
    ERRT("DEFW 'A''B",     MISSING_CLOSING_QUOTE, "'A''B");
    TEST(R"(DEFM 'A''B"C')", 0x41, 0x27, 0x42, 0x22, 0x43);
    ERUS("DB 1, UNDEF, 2", "UNDEF, 2", 0x01, 0x00, 0x02);
    ERUS("DW 1, UNDEF, 2", "UNDEF, 2", 0x01, 0x00, 0x00, 0x00, 0x02, 0x00);
    ERUS("DL 1, UNDEF, 2", "UNDEF, 2",
         0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00);
    ERUS("DEFB 1, UNDEF, 2", "UNDEF, 2", 0x01, 0x00, 0x02);
    ERUS("DEFM 1, UNDEF, 2", "UNDEF, 2", 0x01, 0x00, 0x02);
    ERUS("DEFW 1, UNDEF, 2", "UNDEF, 2", 0x01, 0x00, 0x00, 0x00, 0x02, 0x00);

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
         0x34, 0x12, 0x78, 0x56, 0xBC, 0x9A, 0xF0, 0xDE, 0x34, 0x12, 0x78, 0x56, 0xBC, 0x9A, 0xF0, 0xDE,
         0x34, 0x12, 0x78, 0x56, 0xBC, 0x9A, 0xF0, 0xDE, 0x34, 0x12, 0x78, 0x56, 0xBC, 0x9A, 0xF0, 0xDE,
         0x34, 0x12, 0x78, 0x56, 0xBC, 0x9A, 0xF0, 0xDE, 0x34, 0x12, 0x78, 0x56, 0xBC, 0x9A, 0xF0, 0xDE,
         0x34, 0x12, 0x78, 0x56, 0xBC, 0x9A, 0xF0, 0xDE, 0x34, 0x12, 0x78, 0x56, 0xBC, 0x9A, 0xDE, 0x00);
}

// clang-format on

void run_tests(const char *cpu) {
    assembler.setCpu(cpu);
    RUN_TEST(test_move_inherent);
    RUN_TEST(test_move_immediate);
    RUN_TEST(test_move_direct);
    RUN_TEST(test_stack_op);
    RUN_TEST(test_jump_call);
    RUN_TEST(test_incr_decr);
    RUN_TEST(test_alu_register);
    RUN_TEST(test_alu_immediate);
    RUN_TEST(test_io);
    RUN_TEST(test_inherent);
    RUN_TEST(test_restart);
    RUN_TEST(test_relative);
    RUN_TEST(test_shift);
    RUN_TEST(test_bitop);
    RUN_TEST(test_index_registers);
    RUN_TEST(test_indexed);
    RUN_TEST(test_shift_indexed);
    RUN_TEST(test_bitop_indexed);
    RUN_TEST(test_comment);
    RUN_TEST(test_undefined_symbol);
    RUN_TEST(test_data_constant);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

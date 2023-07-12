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

#include "asm_i8080.h"
#include "test_asm_helper.h"

using namespace libasm;
using namespace libasm::i8080;
using namespace libasm::test;

AsmI8080 as8080;
Assembler &assembler(as8080);

static bool is8085() {
    return strcmp_P("8085", assembler.cpu_P()) == 0;
}

static bool v30emu() {
    return strcasecmp_P("V30EMU", assembler.cpu_P()) == 0;
}

static void set_up() {
    assembler.reset();
}

static void tear_down() {
    symtab.reset();
}

// clang-format off
void test_cpu() {
    EQUALS("cpu 8080", true,   assembler.setCpu("8080"));
    EQUALS_P("cpu 8080", "8080", assembler.cpu_P());

    EQUALS("cpu i8080", true,   assembler.setCpu("i8080"));
    EQUALS_P("cpu i8080", "8080", assembler.cpu_P());

    EQUALS("cpu 8085", true,   assembler.setCpu("8085"));
    EQUALS_P("cpu 8085", "8085", assembler.cpu_P());

    EQUALS("cpu i8085", true,   assembler.setCpu("i8085"));
    EQUALS_P("cpu i8085", "8085", assembler.cpu_P());

    EQUALS("cpu v30emu", true,   assembler.setCpu("v30emu"));
    EQUALS_P("cpu v30emu", "V30EMU", assembler.cpu_P());
}

static void test_move_inherent() {
    TEST("mov b,b", 0x40);
    TEST("MOV B,C", 0x41);
    TEST("MOV B,D", 0x42);
    TEST("MOV B,E", 0x43);
    TEST("MOV B,H", 0x44);
    TEST("MOV B,L", 0x45);
    TEST("MOV B,M", 0x46);
    TEST("MOV B,A", 0x47);
    TEST("MOV C,B", 0x48);
    TEST("MOV C,C", 0x49);
    TEST("MOV C,D", 0x4A);
    TEST("MOV C,E", 0x4B);
    TEST("MOV C,H", 0x4C);
    TEST("MOV C,L", 0x4D);
    TEST("MOV C,M", 0x4E);
    TEST("MOV C,A", 0x4F);

    TEST("MOV D,B", 0x50);
    TEST("MOV D,C", 0x51);
    TEST("MOV D,D", 0x52);
    TEST("MOV D,E", 0x53);
    TEST("MOV D,H", 0x54);
    TEST("MOV D,L", 0x55);
    TEST("MOV D,M", 0x56);
    TEST("MOV D,A", 0x57);
    TEST("MOV E,B", 0x58);
    TEST("MOV E,C", 0x59);
    TEST("MOV E,D", 0x5A);
    TEST("MOV E,E", 0x5B);
    TEST("MOV E,H", 0x5C);
    TEST("MOV E,L", 0x5D);
    TEST("MOV E,M", 0x5E);
    TEST("MOV E,A", 0x5F);

    TEST("MOV H,B", 0x60);
    TEST("MOV H,C", 0x61);
    TEST("MOV H,D", 0x62);
    TEST("MOV H,E", 0x63);
    TEST("MOV H,H", 0x64);
    TEST("MOV H,L", 0x65);
    TEST("MOV H,M", 0x66);
    TEST("MOV H,A", 0x67);
    TEST("MOV L,B", 0x68);
    TEST("MOV L,C", 0x69);
    TEST("MOV L,D", 0x6A);
    TEST("MOV L,E", 0x6B);
    TEST("MOV L,H", 0x6C);
    TEST("MOV L,L", 0x6D);
    TEST("MOV L,M", 0x6E);
    TEST("MOV L,A", 0x6F);

    TEST("MOV M,B", 0x70);
    TEST("MOV M,C", 0x71);
    TEST("MOV M,D", 0x72);
    TEST("MOV M,E", 0x73);
    TEST("MOV M,H", 0x74);
    TEST("MOV M,L", 0x75);
    // HLT
    TEST("MOV M,A", 0x77);
    TEST("MOV A,B", 0x78);
    TEST("MOV A,C", 0x79);
    TEST("MOV A,D", 0x7A);
    TEST("MOV A,E", 0x7B);
    TEST("MOV A,H", 0x7C);
    TEST("MOV A,L", 0x7D);
    TEST("MOV A,M", 0x7E);
    TEST("MOV A,A", 0x7F);

    TEST("STAX B",  0x02);
    TEST("STAX D",  0x12);
    TEST("LDAX B",  0x0A);
    TEST("LDAX D",  0x1A);

    if (is8085()) {
        // i8085
        TEST("RIM", 0x20);
        TEST("SIM", 0x30);
        ERUI("RETEM");
        ERUI("CALLN 40H");
    } else if (v30emu()) {
        ERUI("RIM");
        ERUI("SIM");
        TEST("RETEM",     0xED, 0xFD);
        TEST("CALLN 40H", 0xED, 0xED, 0x40);
    } else {
        ERUI("RIM");
        ERUI("SIM");
        ERUI("RETEM");
        ERUI("CALLN 40H");
    }
}

static void test_move_immediate() {
    TEST("MVI B,0F6H", 0x06, 0xF6);
    TEST("MVI C,9FH",  0x0E, 0x9F);
    TEST("MVI D,58",   0x16, 0x3A);
    TEST("MVI E,-128", 0x1E, 0x80);
    ERRT("MVI E,-129", OVERFLOW_RANGE, "-129", 0x1E, 0x7F);
    TEST("MVI H,255",  0x26, 0xFF);
    ERRT("MVI H,256",  OVERFLOW_RANGE, "256",  0x26, 0x00);
    TEST("MVI L,-10",  0x2E, 0xF6);
    TEST("MVI M,0F6H", 0x36, 0xF6);
    TEST("MVI A,0FEH", 0x3E, 0xFE);

    TEST("LXI B,0BEEFH", 0x01, 0xEF, 0xBE);
    TEST("LXI D,1234H",  0x11, 0x34, 0x12);
    TEST("LXI H,-16657", 0x21, 0xEF, 0xBE);
    TEST("LXI SP,6789H", 0x31, 0x89, 0x67);

    symtab.intern(0x1234, "@1234");
    symtab.intern(0x6789, "?6789");
    symtab.intern(0xBEEF, "BEEF");

    TEST("LXI B,BEEF",   0x01, 0xEF, 0xBE);
    TEST("LXI D,@1234",  0x11, 0x34, 0x12);
    TEST("LXI H,BEEF",   0x21, 0xEF, 0xBE);
    TEST("LXI SP,?6789", 0x31, 0x89, 0x67);
}

static void test_move_direct() {
    TEST("STA 9ABCH", 0x32, 0xBC, 0x9A);
    TEST("LDA 1234H", 0x3A, 0x34, 0x12);

    TEST("SHLD 0ABCDH", 0x22, 0xCD, 0xAB);
    TEST("LHLD 5678H",  0x2A, 0x78, 0x56);

    symtab.intern(0x1234, "label1234");
    symtab.intern(0x5678, "label5678");
    symtab.intern(0x9ABC, "label9ABC");
    symtab.intern(0xABCD, "ABCD");

    TEST("STA label9ABC",  0x32, 0xBC, 0x9A);
    TEST("LDA label1234",  0x3A, 0x34, 0x12);

    TEST("SHLD ABCD",      0x22, 0xCD, 0xAB);
    TEST("LHLD label5678", 0x2A, 0x78, 0x56);
}

static void test_stack_op() {
    TEST("PUSH B",   0xC5);
    TEST("PUSH D",   0xD5);
    TEST("PUSH H",   0xE5);
    TEST("PUSH PSW", 0xF5);
    TEST("POP  B",   0xC1);
    TEST("POP  D",   0xD1);
    TEST("POP  H",   0xE1);
    TEST("POP  PSW", 0xF1);

    TEST("XTHL",  0xE3);
    TEST("PCHL",  0xE9);
    TEST("SPHL",  0xF9);
    TEST("XCHG",  0xEB);
}

static void test_jump_call() {
    TEST("JMP 1234H", 0xC3, 0x34, 0x12);
    TEST("JNZ 1234H", 0xC2, 0x34, 0x12);
    TEST("JZ 1234H",  0xCA, 0x34, 0x12);
    TEST("JNC 1234H", 0xD2, 0x34, 0x12);
    TEST("JC 1234H",  0xDA, 0x34, 0x12);
    TEST("JPO 1234H", 0xE2, 0x34, 0x12);
    TEST("JPE 1234H", 0xEA, 0x34, 0x12);
    TEST("JP 1234H",  0xF2, 0x34, 0x12);
    TEST("JM 1234H",  0xFA, 0x34, 0x12);

    TEST("CALL 1234H", 0xCD, 0x34, 0x12);
    TEST("CNZ 1234H",  0xC4, 0x34, 0x12);
    TEST("CZ 1234H",   0xCC, 0x34, 0x12);
    TEST("CNC 1234H",  0xD4, 0x34, 0x12);
    TEST("CC 1234H",   0xDC, 0x34, 0x12);
    TEST("CPO 1234H",  0xE4, 0x34, 0x12);
    TEST("CPE 1234H",  0xEC, 0x34, 0x12);
    TEST("CP 1234H",   0xF4, 0x34, 0x12);
    TEST("CM 1234H",   0xFC, 0x34, 0x12);

    TEST("RET", 0xC9);
    TEST("RNZ", 0xC0);
    TEST("RZ",  0xC8);
    TEST("RNC", 0xD0);
    TEST("RC",  0xD8);
    TEST("RPO", 0xE0);
    TEST("RPE", 0xE8);
    TEST("RP",  0xF0);
    TEST("RM",  0xF8);

    symtab.intern(0x1234, "label1234");

    TEST("JMP label1234", 0xC3, 0x34, 0x12);
    TEST("CC  label1234", 0xDC, 0x34, 0x12);
}

static void test_incr_decr() {
    TEST("INR B", 0x04);
    TEST("INR C", 0x0C);
    TEST("INR D", 0x14);
    TEST("INR E", 0x1C);
    TEST("INR H", 0x24);
    TEST("INR L", 0x2C);
    TEST("INR M", 0x34);
    TEST("INR A", 0x3C);

    TEST("DCR B", 0x05);
    TEST("DCR C", 0x0D);
    TEST("DCR D", 0x15);
    TEST("DCR E", 0x1D);
    TEST("DCR H", 0x25);
    TEST("DCR L", 0x2D);
    TEST("DCR M", 0x35);
    TEST("DCR A", 0x3D);

    TEST("INX B",  0x03);
    TEST("INX D",  0x13);
    TEST("INX H",  0x23);
    TEST("INX SP", 0x33);
    TEST("DCX B",  0x0B);
    TEST("DCX D",  0x1B);
    TEST("DCX H",  0x2B);
    TEST("DCX SP", 0x3B);

    TEST("RST 0", 0xC7);
    TEST("RST 1", 0xCF);
    TEST("RST 2", 0xD7);
    TEST("RST 3", 0xDF);
    TEST("RST 4", 0xE7);
    TEST("RST 5", 0xEF);
    TEST("RST 6", 0xF7);
    TEST("RST 7", 0xFF);
    ERRT("RST -1", OVERFLOW_RANGE, "-1", 0xFF);
    ERRT("RST 8",  OVERFLOW_RANGE, "8",  0xC7);
}

static void test_alu_register() {
    TEST("ADD B", 0x80);
    TEST("ADD C", 0x81);
    TEST("ADD D", 0x82);
    TEST("ADD E", 0x83);
    TEST("ADD H", 0x84);
    TEST("ADD L", 0x85);
    TEST("ADD M", 0x86);
    TEST("ADD A", 0x87);

    TEST("ADC B", 0x88);
    TEST("ADC C", 0x89);
    TEST("ADC D", 0x8A);
    TEST("ADC E", 0x8B);
    TEST("ADC H", 0x8C);
    TEST("ADC L", 0x8D);
    TEST("ADC M", 0x8E);
    TEST("ADC A", 0x8F);

    TEST("SUB B", 0x90);
    TEST("SUB C", 0x91);
    TEST("SUB D", 0x92);
    TEST("SUB E", 0x93);
    TEST("SUB H", 0x94);
    TEST("SUB L", 0x95);
    TEST("SUB M", 0x96);
    TEST("SUB A", 0x97);

    TEST("SBB B", 0x98);
    TEST("SBB C", 0x99);
    TEST("SBB D", 0x9A);
    TEST("SBB E", 0x9B);
    TEST("SBB H", 0x9C);
    TEST("SBB L", 0x9D);
    TEST("SBB M", 0x9E);
    TEST("SBB A", 0x9F);

    TEST("ANA B", 0xA0);
    TEST("ANA C", 0xA1);
    TEST("ANA D", 0xA2);
    TEST("ANA E", 0xA3);
    TEST("ANA H", 0xA4);
    TEST("ANA L", 0xA5);
    TEST("ANA M", 0xA6);
    TEST("ANA A", 0xA7);

    TEST("XRA B", 0xA8);
    TEST("XRA C", 0xA9);
    TEST("XRA D", 0xAA);
    TEST("XRA E", 0xAB);
    TEST("XRA H", 0xAC);
    TEST("XRA L", 0xAD);
    TEST("XRA M", 0xAE);
    TEST("XRA A", 0xAF);

    TEST("ORA B", 0xB0);
    TEST("ORA C", 0xB1);
    TEST("ORA D", 0xB2);
    TEST("ORA E", 0xB3);
    TEST("ORA H", 0xB4);
    TEST("ORA L", 0xB5);
    TEST("ORA M", 0xB6);
    TEST("ORA A", 0xB7);

    TEST("CMP B", 0xB8);
    TEST("CMP C", 0xB9);
    TEST("CMP D", 0xBA);
    TEST("CMP E", 0xBB);
    TEST("CMP H", 0xBC);
    TEST("CMP L", 0xBD);
    TEST("CMP M", 0xBE);
    TEST("CMP A", 0xBF);

    TEST("RLC", 0x07);
    TEST("RRC", 0x0F);
    TEST("RAL", 0x17);
    TEST("RAR", 0x1F);

    TEST("DAA", 0x27);
    TEST("CMA", 0x2F);
    TEST("STC", 0x37);
    TEST("CMC", 0x3F);

    TEST("DAD B",  0x09);
    TEST("DAD D",  0x19);
    TEST("DAD H",  0x29);
    TEST("DAD SP", 0x39);

    TEST("DI",  0xF3);
    TEST("EI",  0xFB);

    TEST("NOP", 0x00);
    TEST("HLT", 0x76);
}

static void test_alu_immediate() {
    TEST("ADI 10B", 0xC6, 0x02);
    TEST("ACI 255", 0xCE, 0xFF);
    ERRT("ACI 256", OVERFLOW_RANGE, "256", 0xCE, 0x00);

    TEST("SUI -2",   0xD6, 0xFE);
    ERRT("SUI -129", OVERFLOW_RANGE, "-129", 0xD6, 0x7F);
    TEST("SBI 177O", 0xDE, 0x7F);

    TEST("ANI ~0FH", 0xE6, 0xF0);
    TEST("XRI ~001B",0xEE, 0xFE);
    TEST("ORI +127", 0xF6, 0x7F);
    TEST("CPI -128", 0xFE, 0x80);
}

static void test_io() {
    TEST("OUT 0F1H", 0xD3, 0xF1);
    TEST("IN  0F0H", 0xDB, 0xF0);
    ERRT("OUT 101H", OVERFLOW_RANGE, "101H", 0xD3, 0x01);
    ERRT("IN  -1",   OVERFLOW_RANGE, "-1",   0xDB, 0xFF);
}

static void test_comment() {
    TEST("RST 1    ; comment", 0xCF);
    TEST("MOV B,B  ; comment", 0x40);
    TEST("STAX B   ; comment", 0x02);
    TEST("XTHL     ; comment", 0xE3);
    TEST("JMP 1234H; comment", 0xC3, 0x34, 0x12);
    TEST("ANI ~0FH ; comment", 0xE6, 0xF0);
    TEST("CPI -1-23; comment", 0xFE, 0xE8);
    TEST("MOV B , B  ; comment",     0x40);
    TEST("MVI M , 0F6H ; comment",   0x36, 0xF6);
    TEST("LXI B , 0BEEFH ; comment", 0x01, 0xEF, 0xBE);
}

static void test_undefined_symbol() {
    ERUS("MVI  B,UNDEF", "UNDEF", 0x06, 0x00);
    ERUS("LXI  B,UNDEF", "UNDEF", 0x01, 0x00, 0x00);
    ERUS("STA  UNDEF", "UNDEF", 0x32, 0x00, 0x00);
    ERUS("SHLD UNDEF", "UNDEF", 0x22, 0x00, 0x00);
    ERUS("JMP  UNDEF", "UNDEF", 0xC3, 0x00, 0x00);
    ERUS("CALL UNDEF", "UNDEF", 0xCD, 0x00, 0x00);
    ERUS("ADI  UNDEF", "UNDEF", 0xC6, 0x00);
    ERUS("OUT  UNDEF", "UNDEF", 0xD3, 0x00);
    ERUS("IN   UNDEF", "UNDEF", 0xDB, 0x00);
}

static void test_data_constant() {
    TEST("DB -128, 255", 0x80, 0xFF);
    TEST("DB 'A', '\"'", 0x41, 0x22);
    TEST("DB '9'-'0'",   0x09);
    TEST("DB ''''",      0x27);
    ERRT("DB '''",       MISSING_CLOSING_QUOTE, "'''");
    TEST("DB 'A''B',0",  0x41, 0x27, 0x42, 0x00);
    ERRT("DB 'A''B,0",   MISSING_CLOSING_QUOTE, "'A''B,0");
    TEST("DW -128, 255", 0x80, 0xFF, 0xFF, 0x00);
    TEST("DW 'A''B'",    0x41, 0x27, 0x42, 0x00);
    ERRT("DW 'A''B",     MISSING_CLOSING_QUOTE, "'A''B");
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

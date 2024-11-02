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

#include "dis_i8080.h"
#include "test_dis_helper.h"

using namespace libasm;
using namespace libasm::i8080;
using namespace libasm::test;

DisI8080 dis8080;
Disassembler &disassembler(dis8080);

static bool is8080() {
    return strcmp_P("8080", disassembler.config().cpu_P()) == 0;
}

static bool is8085() {
    return strcmp_P("8085", disassembler.config().cpu_P()) == 0;
}

static bool v30emu() {
    return strcasecmp_P("V30EMU", disassembler.config().cpu_P()) == 0;
}

static void set_up() {
    disassembler.reset();
}

static void tear_down() {
    symtab.reset();
}

// clang-format off
void test_cpu() {
    EQUALS("cpu 8080", true,   disassembler.setCpu("8080"));
    EQUALS_P("cpu 8080", "8080", disassembler.config().cpu_P());

    EQUALS("cpu I8080", true,   disassembler.setCpu("I8080"));
    EQUALS_P("cpu I8080", "8080", disassembler.config().cpu_P());

    EQUALS("cpu 8085", true,   disassembler.setCpu("8085"));
    EQUALS_P("cpu 8085", "8085", disassembler.config().cpu_P());

    EQUALS("cpu I8085", true,   disassembler.setCpu("I8085"));
    EQUALS_P("cpu I8085", "8085", disassembler.config().cpu_P());

    EQUALS("cpu v30emu", true,   disassembler.setCpu("v30emu"));
    EQUALS_P("cpu v30emu", "V30EMU", disassembler.config().cpu_P());
}

static void intel_move_inherent() {
    TEST("MOV",  "B, B", 0x40);
    TEST("MOV",  "B, C", 0x41);
    TEST("MOV",  "B, D", 0x42);
    TEST("MOV",  "B, E", 0x43);
    TEST("MOV",  "B, H", 0x44);
    TEST("MOV",  "B, L", 0x45);
    TEST("MOV",  "B, M", 0x46);
    TEST("MOV",  "B, A", 0x47);
    TEST("MOV",  "C, B", 0x48);
    TEST("MOV",  "C, C", 0x49);
    TEST("MOV",  "C, D", 0x4A);
    TEST("MOV",  "C, E", 0x4B);
    TEST("MOV",  "C, H", 0x4C);
    TEST("MOV",  "C, L", 0x4D);
    TEST("MOV",  "C, M", 0x4E);
    TEST("MOV",  "C, A", 0x4F);

    TEST("MOV",  "D, B", 0x50);
    TEST("MOV",  "D, C", 0x51);
    TEST("MOV",  "D, D", 0x52);
    TEST("MOV",  "D, E", 0x53);
    TEST("MOV",  "D, H", 0x54);
    TEST("MOV",  "D, L", 0x55);
    TEST("MOV",  "D, M", 0x56);
    TEST("MOV",  "D, A", 0x57);
    TEST("MOV",  "E, B", 0x58);
    TEST("MOV",  "E, C", 0x59);
    TEST("MOV",  "E, D", 0x5A);
    TEST("MOV",  "E, E", 0x5B);
    TEST("MOV",  "E, H", 0x5C);
    TEST("MOV",  "E, L", 0x5D);
    TEST("MOV",  "E, M", 0x5E);
    TEST("MOV",  "E, A", 0x5F);

    TEST("MOV",  "H, B", 0x60);
    TEST("MOV",  "H, C", 0x61);
    TEST("MOV",  "H, D", 0x62);
    TEST("MOV",  "H, E", 0x63);
    TEST("MOV",  "H, H", 0x64);
    TEST("MOV",  "H, L", 0x65);
    TEST("MOV",  "H, M", 0x66);
    TEST("MOV",  "H, A", 0x67);
    TEST("MOV",  "L, B", 0x68);
    TEST("MOV",  "L, C", 0x69);
    TEST("MOV",  "L, D", 0x6A);
    TEST("MOV",  "L, E", 0x6B);
    TEST("MOV",  "L, H", 0x6C);
    TEST("MOV",  "L, L", 0x6D);
    TEST("MOV",  "L, M", 0x6E);
    TEST("MOV",  "L, A", 0x6F);

    TEST("MOV",  "M, B", 0x70);
    TEST("MOV",  "M, C", 0x71);
    TEST("MOV",  "M, D", 0x72);
    TEST("MOV",  "M, E", 0x73);
    TEST("MOV",  "M, H", 0x74);
    TEST("MOV",  "M, L", 0x75);
    // HLT
    TEST("MOV",  "M, A", 0x77);
    TEST("MOV",  "A, B", 0x78);
    TEST("MOV",  "A, C", 0x79);
    TEST("MOV",  "A, D", 0x7A);
    TEST("MOV",  "A, E", 0x7B);
    TEST("MOV",  "A, H", 0x7C);
    TEST("MOV",  "A, L", 0x7D);
    TEST("MOV",  "A, M", 0x7E);
    TEST("MOV",  "A, A", 0x7F);

    TEST("STAX", "B",   0x02);
    TEST("STAX", "D",   0x12);
    TEST("LDAX", "B",   0x0A);
    TEST("LDAX", "D",   0x1A);

    if (is8085()) {
        // i8085
        TEST("RIM", "", 0x20);
        TEST("SIM", "", 0x30);
        UNKN(0xED);
    } else if (v30emu()) {
        UNKN(0x20);
        UNKN(0x30);
        NMEM("", "", "", 0xED);
        TEST("RETEM", "",    0xED, 0xFD);
        TEST("CALLN", "40H",    0xED, 0xED, 0x40);
        NMEM("CALLN", "0", "0", 0xED, 0xED);
    } else {
        UNKN(0x20);
        UNKN(0x30);
        UNKN(0xED);
    }
}

static void zilog_move_inherent() {
    disassembler.setOption("zilog-syntax", "on");

    TEST("LD", "B, B", 0x40);
    TEST("LD", "B, C", 0x41);
    TEST("LD", "B, D", 0x42);
    TEST("LD", "B, E", 0x43);
    TEST("LD", "B, H", 0x44);
    TEST("LD", "B, L", 0x45);
    TEST("LD", "B, (HL)", 0x46);
    TEST("LD", "B, A", 0x47);
    TEST("LD", "C, B", 0x48);
    TEST("LD", "C, C", 0x49);
    TEST("LD", "C, D", 0x4A);
    TEST("LD", "C, E", 0x4B);
    TEST("LD", "C, H", 0x4C);
    TEST("LD", "C, L", 0x4D);
    TEST("LD", "C, (HL)", 0x4E);
    TEST("LD", "C, A", 0x4F);

    TEST("LD", "D, B", 0x50);
    TEST("LD", "D, C", 0x51);
    TEST("LD", "D, D", 0x52);
    TEST("LD", "D, E", 0x53);
    TEST("LD", "D, H", 0x54);
    TEST("LD", "D, L", 0x55);
    TEST("LD", "D, (HL)", 0x56);
    TEST("LD", "D, A", 0x57);
    TEST("LD", "E, B", 0x58);
    TEST("LD", "E, C", 0x59);
    TEST("LD", "E, D", 0x5A);
    TEST("LD", "E, E", 0x5B);
    TEST("LD", "E, H", 0x5C);
    TEST("LD", "E, L", 0x5D);
    TEST("LD", "E, (HL)", 0x5E);
    TEST("LD", "E, A", 0x5F);

    TEST("LD", "H, B", 0x60);
    TEST("LD", "H, C", 0x61);
    TEST("LD", "H, D", 0x62);
    TEST("LD", "H, E", 0x63);
    TEST("LD", "H, H", 0x64);
    TEST("LD", "H, L", 0x65);
    TEST("LD", "H, (HL)", 0x66);
    TEST("LD", "H, A", 0x67);
    TEST("LD", "L, B", 0x68);
    TEST("LD", "L, C", 0x69);
    TEST("LD", "L, D", 0x6A);
    TEST("LD", "L, E", 0x6B);
    TEST("LD", "L, H", 0x6C);
    TEST("LD", "L, L", 0x6D);
    TEST("LD", "L, (HL)", 0x6E);
    TEST("LD", "L, A", 0x6F);

    TEST("LD", "(HL), B", 0x70);
    TEST("LD", "(HL), C", 0x71);
    TEST("LD", "(HL), D", 0x72);
    TEST("LD", "(HL), E", 0x73);
    TEST("LD", "(HL), H", 0x74);
    TEST("LD", "(HL), L", 0x75);
    // HALT
    TEST("LD", "(HL), A", 0x77);
    TEST("LD", "A, B", 0x78);
    TEST("LD", "A, C", 0x79);
    TEST("LD", "A, D", 0x7A);
    TEST("LD", "A, E", 0x7B);
    TEST("LD", "A, H", 0x7C);
    TEST("LD", "A, L", 0x7D);
    TEST("LD", "A, (HL)", 0x7E);
    TEST("LD", "A, A", 0x7F);

    TEST("LD", "(BC), A", 0x02);
    TEST("LD", "(DE), A", 0x12);
    TEST("LD", "A, (BC)", 0x0A);
    TEST("LD", "A, (DE)", 0x1A);

    if (is8085()) {
        // i8085
        TEST("LD", "A, IM", 0x20);
        TEST("LD", "IM, A", 0x30);
        UNKN(0xED);
    } else if (v30emu()) {
        UNKN(0x20);
        UNKN(0x30);
        NMEM("", "", "", 0xED);
        TEST("RETEM", "",    0xED, 0xFD);
        TEST("CALLN", "40H",    0xED, 0xED, 0x40);
        NMEM("CALLN", "0", "0", 0xED, 0xED);
    } else {
        UNKN(0x20);
        UNKN(0x30);
        UNKN(0xED);
    }
}

static void intel_move_immediate() {
    TEST("MVI", "B, 0F6H", 0x06, 0xF6);
    TEST("MVI", "C, 9FH",  0x0E, 0x9F);
    TEST("MVI", "D, 3AH",  0x16, 0x3A);
    TEST("MVI", "E, 80H",  0x1E, 0x80);
    TEST("MVI", "H, 0F6H", 0x26, 0xF6);
    TEST("MVI", "L, 0F6H", 0x2E, 0xF6);
    TEST("MVI", "M, 0F6H", 0x36, 0xF6);
    TEST("MVI", "A, 0FEH", 0x3E, 0xFE);
    NMEM("MVI", "A, 0", "0", 0x3E);

    TEST("LXI", "B, 0BEEFH", 0x01, 0xEF, 0xBE);
    TEST("LXI", "D, 1234H",  0x11, 0x34, 0x12);
    TEST("LXI", "H, 0BEEFH", 0x21, 0xEF, 0xBE);
    TEST("LXI", "SP, 6789H", 0x31, 0x89, 0x67);
    NMEM("LXI", "SP, 0089H", "0089H", 0x31, 0x89);
    NMEM("LXI", "SP, 0", "0",         0x31);

    symtab.intern(0xBEEF, "BEEF");
    symtab.intern(0x1234, "label1234");
    symtab.intern(0x6789, "label6789");

    TEST("LXI", "B, BEEF",       0x01, 0xEF, 0xBE);
    TEST("LXI", "D, label1234",  0x11, 0x34, 0x12);
    TEST("LXI", "H, BEEF",       0x21, 0xEF, 0xBE);
    TEST("LXI", "SP, label6789", 0x31, 0x89, 0x67);
}

static void zilog_move_immediate() {
    disassembler.setOption("zilog-syntax", "on");

    TEST("LD", "B, 0F6H", 0x06, 0xF6);
    TEST("LD", "C, 9FH",  0x0E, 0x9F);
    TEST("LD", "D, 3AH",  0x16, 0x3A);
    TEST("LD", "E, 80H",  0x1E, 0x80);
    TEST("LD", "H, 0F6H", 0x26, 0xF6);
    TEST("LD", "L, 0F6H", 0x2E, 0xF6);
    TEST("LD", "(HL), 0F6H", 0x36, 0xF6);
    TEST("LD", "A, 0FEH",    0x3E, 0xFE);
    NMEM("LD", "A, 0",  "0", 0x3E);

    TEST("LD", "BC, 0BEEFH", 0x01, 0xEF, 0xBE);
    TEST("LD", "DE, 1234H",  0x11, 0x34, 0x12);
    TEST("LD", "HL, 0BEEFH", 0x21, 0xEF, 0xBE);
    TEST("LD", "SP, 6789H",          0x31, 0x89, 0x67);
    NMEM("LD", "SP, 0089H", "0089H", 0x31, 0x89);
    NMEM("LD", "SP, 0",         "0", 0x31);
}

static void intel_move_direct() {
    TEST("STA", "9ABCH", 0x32, 0xBC, 0x9A);
    TEST("LDA", "1234H", 0x3A, 0x34, 0x12);

    TEST("SHLD", "0ABCDH", 0x22, 0xCD, 0xAB);
    TEST("LHLD", "5678H",  0x2A, 0x78, 0x56);
    NMEM("LHLD", "0078H",  "0078H", 0x2A, 0x78);
    NMEM("LHLD", "0000H",  "0000H", 0x2A);

    symtab.intern(0x1234, "label1234");
    symtab.intern(0x5678, "label5678");
    symtab.intern(0x9ABC, "label9ABC");
    symtab.intern(0xABCD, "ABCD");

    TEST("STA", "label9ABC", 0x32, 0xBC, 0x9A);
    TEST("LDA", "label1234", 0x3A, 0x34, 0x12);

    TEST("SHLD", "ABCD",      0x22, 0xCD, 0xAB);
    TEST("LHLD", "label5678", 0x2A, 0x78, 0x56);
}

static void zilog_move_direct() {
    disassembler.setOption("zilog-syntax", "on");

    TEST("LD", "(9ABCH), A", 0x32, 0xBC, 0x9A);
    TEST("LD", "A, (1234H)", 0x3A, 0x34, 0x12);

    TEST("LD", "(0ABCDH), HL", 0x22, 0xCD, 0xAB);
    TEST("LD", "HL, (5678H)",  0x2A, 0x78, 0x56);
}

static void intel_stack_op() {
    TEST("PUSH", "B",   0xC5);
    TEST("PUSH", "D",   0xD5);
    TEST("PUSH", "H",   0xE5);
    TEST("PUSH", "PSW", 0xF5);
    TEST("POP",  "B",   0xC1);
    TEST("POP",  "D",   0xD1);
    TEST("POP",  "H",   0xE1);
    TEST("POP",  "PSW", 0xF1);

    TEST("XTHL", "",  0xE3);
    TEST("PCHL", "",  0xE9);
    TEST("SPHL", "",  0xF9);
    TEST("XCHG", "",  0xEB);
}

static void zilog_stack_op() {
    disassembler.setOption("zilog-syntax", "on");

    TEST("PUSH", "BC", 0xC5);
    TEST("PUSH", "DE", 0xD5);
    TEST("PUSH", "HL", 0xE5);
    TEST("PUSH", "AF", 0xF5);
    TEST("POP",  "BC", 0xC1);
    TEST("POP",  "DE", 0xD1);
    TEST("POP",  "HL", 0xE1);
    TEST("POP",  "AF", 0xF1);

    TEST("EX", "(SP), HL", 0xE3);
    TEST("JP", "(HL)",    0xE9);
    TEST("LD", "SP, HL",   0xF9);
    TEST("EX", "DE, HL",   0xEB);
}

static void intel_jump_call() {
    TEST("JMP", "1234H", 0xC3, 0x34, 0x12);
    TEST("JNZ", "1234H", 0xC2, 0x34, 0x12);
    TEST("JZ",  "1234H", 0xCA, 0x34, 0x12);
    TEST("JNC", "1234H", 0xD2, 0x34, 0x12);
    TEST("JC",  "1234H", 0xDA, 0x34, 0x12);
    TEST("JPO", "1234H", 0xE2, 0x34, 0x12);
    TEST("JPE", "1234H", 0xEA, 0x34, 0x12);
    TEST("JP",  "1234H", 0xF2, 0x34, 0x12);
    TEST("JM",  "1234H", 0xFA, 0x34, 0x12);
    NMEM("JM",  "0034H", "0034H", 0xFA, 0x34);
    NMEM("JM",  "0000H", "0000H", 0xFA);

    TEST("CALL", "1234H", 0xCD, 0x34, 0x12);
    TEST("CNZ",  "1234H", 0xC4, 0x34, 0x12);
    TEST("CZ",   "1234H", 0xCC, 0x34, 0x12);
    TEST("CNC",  "1234H", 0xD4, 0x34, 0x12);
    TEST("CC",   "1234H", 0xDC, 0x34, 0x12);
    TEST("CPO",  "1234H", 0xE4, 0x34, 0x12);
    TEST("CPE",  "1234H", 0xEC, 0x34, 0x12);
    TEST("CP",   "1234H", 0xF4, 0x34, 0x12);
    TEST("CM",   "1234H", 0xFC, 0x34, 0x12);

    TEST("RET", "", 0xC9);
    TEST("RNZ", "", 0xC0);
    TEST("RZ",  "", 0xC8);
    TEST("RNC", "", 0xD0);
    TEST("RC",  "", 0xD8);
    TEST("RPO", "", 0xE0);
    TEST("RPE", "", 0xE8);
    TEST("RP",  "", 0xF0);
    TEST("RM",  "", 0xF8);

    symtab.intern(0x1234, "label1234");

    TEST("JMP", "label1234", 0xC3, 0x34, 0x12);
    TEST("CC",  "label1234", 0xDC, 0x34, 0x12);
}

static void zilog_jump_call() {
    disassembler.setOption("zilog-syntax", "on");

    TEST("JP", "1234H",  0xC3, 0x34, 0x12);
    TEST("JP", "NZ, 1234H",  0xC2, 0x34, 0x12);
    TEST("JP",  "Z, 1234H",  0xCA, 0x34, 0x12);
    TEST("JP", "NC, 1234H",  0xD2, 0x34, 0x12);
    TEST("JP",  "C, 1234H",  0xDA, 0x34, 0x12);
    TEST("JP", "PO, 1234H",  0xE2, 0x34, 0x12);
    TEST("JP", "PE, 1234H",  0xEA, 0x34, 0x12);
    TEST("JP",  "P, 1234H",  0xF2, 0x34, 0x12);
    TEST("JP",  "M, 1234H",  0xFA, 0x34, 0x12);

    TEST("CALL", "1234H",  0xCD, 0x34, 0x12);
    TEST("CALL", "NZ, 1234H",  0xC4, 0x34, 0x12);
    TEST("CALL",  "Z, 1234H",  0xCC, 0x34, 0x12);
    TEST("CALL", "NC, 1234H",  0xD4, 0x34, 0x12);
    TEST("CALL",  "C, 1234H",  0xDC, 0x34, 0x12);
    TEST("CALL", "PO, 1234H",  0xE4, 0x34, 0x12);
    TEST("CALL", "PE, 1234H",  0xEC, 0x34, 0x12);
    TEST("CALL",  "P, 1234H",  0xF4, 0x34, 0x12);
    TEST("CALL",  "M, 1234H",  0xFC, 0x34, 0x12);

    TEST("RET", "",    0xC9);
    TEST("RET", "NZ",  0xC0);
    TEST("RET",  "Z",  0xC8);
    TEST("RET", "NC",  0xD0);
    TEST("RET",  "C",  0xD8);
    TEST("RET", "PO",  0xE0);
    TEST("RET", "PE",  0xE8);
    TEST("RET",  "P",  0xF0);
    TEST("RET",  "M",  0xF8);
}

static void intel_incr_decr() {
    TEST("INR", "B", 0x04);
    TEST("INR", "C", 0x0C);
    TEST("INR", "D", 0x14);
    TEST("INR", "E", 0x1C);
    TEST("INR", "H", 0x24);
    TEST("INR", "L", 0x2C);
    TEST("INR", "M", 0x34);
    TEST("INR", "A", 0x3C);

    TEST("DCR", "B", 0x05);
    TEST("DCR", "C", 0x0D);
    TEST("DCR", "D", 0x15);
    TEST("DCR", "E", 0x1D);
    TEST("DCR", "H", 0x25);
    TEST("DCR", "L", 0x2D);
    TEST("DCR", "M", 0x35);
    TEST("DCR", "A", 0x3D);

    TEST("INX", "B",  0x03);
    TEST("INX", "D",  0x13);
    TEST("INX", "H",  0x23);
    TEST("INX", "SP", 0x33);
    TEST("DCX", "B",  0x0B);
    TEST("DCX", "D",  0x1B);
    TEST("DCX", "H",  0x2B);
    TEST("DCX", "SP", 0x3B);
}

static void zilog_incr_decr() {
    disassembler.setOption("zilog-syntax", "on");

    TEST("INC", "B",   0x04);
    TEST("INC", "C",   0x0C);
    TEST("INC", "D",   0x14);
    TEST("INC", "E",   0x1C);
    TEST("INC", "H",   0x24);
    TEST("INC", "L",   0x2C);
    TEST("INC", "(HL)",0x34);
    TEST("INC", "A",   0x3C);

    TEST("DEC", "B",   0x05);
    TEST("DEC", "C",   0x0D);
    TEST("DEC", "D",   0x15);
    TEST("DEC", "E",   0x1D);
    TEST("DEC", "H",   0x25);
    TEST("DEC", "L",   0x2D);
    TEST("DEC", "(HL)",0x35);
    TEST("DEC", "A",   0x3D);

    TEST("INC", "BC", 0x03);
    TEST("INC", "DE", 0x13);
    TEST("INC", "HL", 0x23);
    TEST("INC", "SP", 0x33);
    TEST("DEC", "BC", 0x0B);
    TEST("DEC", "DE", 0x1B);
    TEST("DEC", "HL", 0x2B);
    TEST("DEC", "SP", 0x3B);
}

static void intel_alu_register() {
    TEST("ADD", "B", 0x80);
    TEST("ADD", "C", 0x81);
    TEST("ADD", "D", 0x82);
    TEST("ADD", "E", 0x83);
    TEST("ADD", "H", 0x84);
    TEST("ADD", "L", 0x85);
    TEST("ADD", "M", 0x86);
    TEST("ADD", "A", 0x87);

    TEST("ADC", "B", 0x88);
    TEST("ADC", "C", 0x89);
    TEST("ADC", "D", 0x8A);
    TEST("ADC", "E", 0x8B);
    TEST("ADC", "H", 0x8C);
    TEST("ADC", "L", 0x8D);
    TEST("ADC", "M", 0x8E);
    TEST("ADC", "A", 0x8F);

    TEST("SUB", "B", 0x90);
    TEST("SUB", "C", 0x91);
    TEST("SUB", "D", 0x92);
    TEST("SUB", "E", 0x93);
    TEST("SUB", "H", 0x94);
    TEST("SUB", "L", 0x95);
    TEST("SUB", "M", 0x96);
    TEST("SUB", "A", 0x97);

    TEST("SBB", "B", 0x98);
    TEST("SBB", "C", 0x99);
    TEST("SBB", "D", 0x9A);
    TEST("SBB", "E", 0x9B);
    TEST("SBB", "H", 0x9C);
    TEST("SBB", "L", 0x9D);
    TEST("SBB", "M", 0x9E);
    TEST("SBB", "A", 0x9F);

    TEST("ANA", "B", 0xA0);
    TEST("ANA", "C", 0xA1);
    TEST("ANA", "D", 0xA2);
    TEST("ANA", "E", 0xA3);
    TEST("ANA", "H", 0xA4);
    TEST("ANA", "L", 0xA5);
    TEST("ANA", "M", 0xA6);
    TEST("ANA", "A", 0xA7);

    TEST("XRA", "B", 0xA8);
    TEST("XRA", "C", 0xA9);
    TEST("XRA", "D", 0xAA);
    TEST("XRA", "E", 0xAB);
    TEST("XRA", "H", 0xAC);
    TEST("XRA", "L", 0xAD);
    TEST("XRA", "M", 0xAE);
    TEST("XRA", "A", 0xAF);

    TEST("ORA", "B", 0xB0);
    TEST("ORA", "C", 0xB1);
    TEST("ORA", "D", 0xB2);
    TEST("ORA", "E", 0xB3);
    TEST("ORA", "H", 0xB4);
    TEST("ORA", "L", 0xB5);
    TEST("ORA", "M", 0xB6);
    TEST("ORA", "A", 0xB7);

    TEST("CMP", "B", 0xB8);
    TEST("CMP", "C", 0xB9);
    TEST("CMP", "D", 0xBA);
    TEST("CMP", "E", 0xBB);
    TEST("CMP", "H", 0xBC);
    TEST("CMP", "L", 0xBD);
    TEST("CMP", "M", 0xBE);
    TEST("CMP", "A", 0xBF);

    TEST("DAD", "B",  0x09);
    TEST("DAD", "D",  0x19);
    TEST("DAD", "H",  0x29);
    TEST("DAD", "SP", 0x39);
}

static void zilog_alu_register() {
    disassembler.setOption("zilog-syntax", "on");

    TEST("ADD", "A, B", 0x80);
    TEST("ADD", "A, C", 0x81);
    TEST("ADD", "A, D", 0x82);
    TEST("ADD", "A, E", 0x83);
    TEST("ADD", "A, H", 0x84);
    TEST("ADD", "A, L", 0x85);
    TEST("ADD", "A, (HL)", 0x86);
    TEST("ADD", "A, A", 0x87);

    TEST("ADC", "A, B", 0x88);
    TEST("ADC", "A, C", 0x89);
    TEST("ADC", "A, D", 0x8A);
    TEST("ADC", "A, E", 0x8B);
    TEST("ADC", "A, H", 0x8C);
    TEST("ADC", "A, L", 0x8D);
    TEST("ADC", "A, (HL)", 0x8E);
    TEST("ADC", "A, A", 0x8F);

    TEST("SUB", "A, B", 0x90);
    TEST("SUB", "A, C", 0x91);
    TEST("SUB", "A, D", 0x92);
    TEST("SUB", "A, E", 0x93);
    TEST("SUB", "A, H", 0x94);
    TEST("SUB", "A, L", 0x95);
    TEST("SUB", "A, (HL)", 0x96);
    TEST("SUB", "A, A", 0x97);

    TEST("SBC", "A, B", 0x98);
    TEST("SBC", "A, C", 0x99);
    TEST("SBC", "A, D", 0x9A);
    TEST("SBC", "A, E", 0x9B);
    TEST("SBC", "A, H", 0x9C);
    TEST("SBC", "A, L", 0x9D);
    TEST("SBC", "A, (HL)", 0x9E);
    TEST("SBC", "A, A", 0x9F);

    TEST("AND", "A, B", 0xA0);
    TEST("AND", "A, C", 0xA1);
    TEST("AND", "A, D", 0xA2);
    TEST("AND", "A, E", 0xA3);
    TEST("AND", "A, H", 0xA4);
    TEST("AND", "A, L", 0xA5);
    TEST("AND", "A, (HL)", 0xA6);
    TEST("AND", "A, A", 0xA7);

    TEST("XOR", "A, B", 0xA8);
    TEST("XOR", "A, C", 0xA9);
    TEST("XOR", "A, D", 0xAA);
    TEST("XOR", "A, E", 0xAB);
    TEST("XOR", "A, H", 0xAC);
    TEST("XOR", "A, L", 0xAD);
    TEST("XOR", "A, (HL)", 0xAE);
    TEST("XOR", "A, A", 0xAF);

    TEST("OR",  "A, B", 0xB0);
    TEST("OR",  "A, C", 0xB1);
    TEST("OR",  "A, D", 0xB2);
    TEST("OR",  "A, E", 0xB3);
    TEST("OR",  "A, H", 0xB4);
    TEST("OR",  "A, L", 0xB5);
    TEST("OR",  "A, (HL)", 0xB6);
    TEST("OR",  "A, A", 0xB7);

    TEST("CP",  "A, B", 0xB8);
    TEST("CP",  "A, C", 0xB9);
    TEST("CP",  "A, D", 0xBA);
    TEST("CP",  "A, E", 0xBB);
    TEST("CP",  "A, H", 0xBC);
    TEST("CP",  "A, L", 0xBD);
    TEST("CP",  "A, (HL)", 0xBE);
    TEST("CP",  "A, A", 0xBF);

    TEST("ADD", "HL, BC", 0x09);
    TEST("ADD", "HL, DE", 0x19);
    TEST("ADD", "HL, HL", 0x29);
    TEST("ADD", "HL, SP", 0x39);
}

static void intel_alu_immediate() {
    TEST("ADI", "1",    0xC6, 0x01);
    TEST("ACI", "0FFH", 0xCE, 0xFF);

    TEST("SUI", "2",    0xD6, 0x02);
    TEST("SBI", "0FFH", 0xDE, 0xFF);

    TEST("ANI", "9",    0xE6, 0x09);
    TEST("XRI", "0FFH", 0xEE, 0xFF);
    TEST("ORI", "10",   0xF6, 0x0A);
    TEST("CPI", "0",    0xFE, 0x00);
    NMEM("CPI", "0", "0", 0xFE);
}

static void zilog_alu_immediate() {
    disassembler.setOption("zilog-syntax", "on");

    TEST("ADD", "A, 1",    0xC6, 0x01);
    TEST("ADC", "A, 0FFH", 0xCE, 0xFF);
    TEST("SUB", "A, 2",    0xD6, 0x02);
    TEST("SBC", "A, 0FFH", 0xDE, 0xFF);
    TEST("AND", "A, 9",    0xE6, 0x09);
    TEST("XOR", "A, 0FFH", 0xEE, 0xFF);
    TEST("OR",  "A, 10",   0xF6, 0x0A);
    TEST("CP",  "A, 0",    0xFE, 0x00);
}

static void intel_io() {
    TEST("OUT", "0F1H", 0xD3, 0xF1);
    TEST("IN",  "0F0H", 0xDB, 0xF0);
    NMEM("IN",  "00H", "00H", 0xDB);
}

static void zilog_io() {
    disassembler.setOption("zilog-syntax", "on");

    TEST("OUT", "(0F1H), A", 0xD3, 0xF1);
    TEST("IN",  "A, (0F0H)", 0xDB, 0xF0);
}

static void intel_inherent() {
    TEST("DI",  "", 0xF3);
    TEST("EI",  "", 0xFB);

    TEST("NOP", "", 0x00);
    TEST("HLT", "", 0x76);

    TEST("RLC", "", 0x07);
    TEST("RRC", "", 0x0F);
    TEST("RAL", "", 0x17);
    TEST("RAR", "", 0x1F);

    TEST("DAA", "", 0x27);
    TEST("CMA", "", 0x2F);
    TEST("STC", "", 0x37);
    TEST("CMC", "", 0x3F);
}

static void zilog_inherent() {
    disassembler.setOption("zilog-syntax", "on");

    TEST("DI",   "", 0xF3);
    TEST("EI",   "", 0xFB);

    TEST("NOP",  "", 0x00);
    TEST("HALT", "", 0x76);

    TEST("RLCA", "", 0x07);
    TEST("RRCA", "", 0x0F);
    TEST("RLA",  "", 0x17);
    TEST("RRA",  "", 0x1F);

    TEST("DAA", "", 0x27);
    TEST("CPL", "", 0x2F);
    TEST("SCF", "", 0x37);
    TEST("CCF", "", 0x3F);

    if (is8085()) {
        TEST("LD", "A, IM", 0x20);
        TEST("LD", "IM, A", 0x30);
    } else if (is8080()) {
        UNKN(0x20);
        UNKN(0x30);
    }
}

static void intel_restart() {
    TEST("RST", "0", 0xC7);
    TEST("RST", "1", 0xCF);
    TEST("RST", "2", 0xD7);
    TEST("RST", "3", 0xDF);
    TEST("RST", "4", 0xE7);
    TEST("RST", "5", 0xEF);
    TEST("RST", "6", 0xF7);
    TEST("RST", "7", 0xFF);
}

static void zilog_restart() {
    disassembler.setOption("zilog-syntax", "on");

    TEST("RST", "0",   0xC7);
    TEST("RST", "8",   0xCF);
    TEST("RST", "16",  0xD7);
    TEST("RST", "24",  0xDF);
    TEST("RST", "32",  0xE7);
    TEST("RST", "28H", 0xEF);
    TEST("RST", "30H", 0xF7);
    TEST("RST", "38H", 0xFF);

    if (v30emu()) {
        TEST("RETEM", "", 0xED, 0xFD);
    }
}

// clang-format on

static void test_illegal() {
    if (is8080()) {
        UNKN(0x20);
        UNKN(0x30);
    }
    UNKN(0x08);
    UNKN(0x10);
    UNKN(0x18);
    UNKN(0x28);
    UNKN(0x38);
    UNKN(0xCB);
    UNKN(0xD9);
    UNKN(0xDD);
    if (!v30emu()) {
        UNKN(0xED);
        UNKN(0xFD);
    }
}

void run_tests(const char *cpu) {
    disassembler.setCpu(cpu);
    RUN_TEST(intel_move_inherent);
    RUN_TEST(intel_move_immediate);
    RUN_TEST(intel_move_direct);
    RUN_TEST(intel_stack_op);
    RUN_TEST(intel_jump_call);
    RUN_TEST(intel_incr_decr);
    RUN_TEST(intel_alu_register);
    RUN_TEST(intel_alu_immediate);
    RUN_TEST(intel_io);
    RUN_TEST(intel_restart);
    RUN_TEST(intel_inherent);
    RUN_TEST(zilog_move_inherent);
    RUN_TEST(zilog_move_immediate);
    RUN_TEST(zilog_move_direct);
    RUN_TEST(zilog_stack_op);
    RUN_TEST(zilog_jump_call);
    RUN_TEST(zilog_incr_decr);
    RUN_TEST(zilog_alu_register);
    RUN_TEST(zilog_alu_immediate);
    RUN_TEST(zilog_io);
    RUN_TEST(zilog_restart);
    RUN_TEST(zilog_inherent);
    RUN_TEST(test_illegal);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

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

#include "dis_mc6800.h"
#include "test_dis_helper.h"
#include "test_symtab.h"

using namespace libasm;
using namespace libasm::mc6800;
using namespace libasm::test;

DisMc6800 dis6800;
Disassembler &disassembler(dis6800);

static bool mb8861() {
    return strcasecmp_P("MB8861", disassembler.cpu_P()) == 0;
}

static bool m6800() {
    return strcmp_P("6800", disassembler.cpu_P()) == 0 || mb8861();
}

static bool hd6301() {
    return strcmp_P("6301", disassembler.cpu_P()) == 0;
}

static bool m68hc11() {
    return strcmp_P("6811", disassembler.cpu_P()) == 0;
}

static bool m6801() {
    return strcmp_P("6801", disassembler.cpu_P()) == 0 || hd6301() || m68hc11();
}

static void set_up() {
    disassembler.reset();
    disassembler.setOption("relative", "no");
}

static void tear_down() {
    symtab.reset();
}

// clang-format off
void test_cpu() {
    EQUALS("cpu 6800", true, disassembler.setCpu("6800"));
    EQUALS_P("cpu 6800", "6800", disassembler.cpu_P());

    EQUALS("cpu 6801", true, disassembler.setCpu("6801"));
    EQUALS_P("cpu 6801", "6801", disassembler.cpu_P());

    EQUALS("cpu 6301", true, disassembler.setCpu("6301"));
    EQUALS_P("cpu 6301", "6301", disassembler.cpu_P());

    EQUALS("cpu 6811", true, disassembler.setCpu("6811"));
    EQUALS_P("cpu 6811", "6811", disassembler.cpu_P());

    EQUALS("cpu MC6800", true, disassembler.setCpu("MC6800"));
    EQUALS_P("cpu MC6800", "6800", disassembler.cpu_P());

    EQUALS("cpu mb8861", true, disassembler.setCpu("mb8861"));
    EQUALS_P("cpu MB8861", "MB8861", disassembler.cpu_P());

    EQUALS("cpu MC6801", true, disassembler.setCpu("MC6801"));
    EQUALS_P("cpu MC6801", "6801", disassembler.cpu_P());

    EQUALS("cpu HD6301", true, disassembler.setCpu("HD6301"));
    EQUALS_P("cpu HD6301", "6301", disassembler.cpu_P());

    EQUALS("cpu MC6811", true, disassembler.setCpu("MC6811"));
    EQUALS_P("cpu MC6811", "6811", disassembler.cpu_P());

    EQUALS("cpu MC68HC11", true, disassembler.setCpu("MC68HC11"));
    EQUALS_P("cpu MC68HC11", "6811", disassembler.cpu_P());
}

static void test_inherent() {
    TEST("NOP", "", 0x01);
    TEST("TAP", "", 0x06);
    TEST("TPA", "", 0x07);
    TEST("INX", "", 0x08);
    TEST("DEX", "", 0x09);
    TEST("CLV", "", 0x0A);
    TEST("SEV", "", 0x0B);
    TEST("CLC", "", 0x0C);
    TEST("SEC", "", 0x0D);
    TEST("CLI", "", 0x0E);
    TEST("SEI", "", 0x0F);
    TEST("SBA", "", 0x10);
    TEST("CBA", "", 0x11);
    TEST("TAB", "", 0x16);
    TEST("TBA", "", 0x17);
    TEST("DAA", "", 0x19);
    TEST("ABA", "", 0x1B);
    TEST("TSX", "", 0x30);
    TEST("INS", "", 0x31);
    TEST("DES", "", 0x34);
    TEST("TXS", "", 0x35);

    TEST("RTS", "", 0x39);
    TEST("RTI", "", 0x3B);
    TEST("WAI", "", 0x3E);
    TEST("SWI", "", 0x3F);

    TEST("PULA", "", 0x32);
    TEST("PULB", "", 0x33);
    TEST("PSHA", "", 0x36);
    TEST("PSHB", "", 0x37);

    TEST("NEGB", "", 0x50);
    TEST("COMB", "", 0x53);
    TEST("LSRB", "", 0x54);
    TEST("RORB", "", 0x56);
    TEST("ASRB", "", 0x57);
    TEST("ASLB", "", 0x58);
    TEST("ROLB", "", 0x59);
    TEST("DECB", "", 0x5A);
    TEST("INCB", "", 0x5C);
    TEST("TSTB", "", 0x5D);
    TEST("CLRB", "", 0x5F);

    TEST("NEGA", "", 0x40);
    TEST("COMA", "", 0x43);
    TEST("LSRA", "", 0x44);
    TEST("RORA", "", 0x46);
    TEST("ASRA", "", 0x47);
    TEST("ASLA", "", 0x48);
    TEST("ROLA", "", 0x49);
    TEST("DECA", "", 0x4A);
    TEST("INCA", "", 0x4C);
    TEST("TSTA", "", 0x4D);
    TEST("CLRA", "", 0x4F);

    if (m6801()) {
        // MC6801
        TEST("LSRD", "", 0x04);
        TEST("ASLD", "", 0x05);
        TEST("PULX", "", 0x38);
        TEST("ABX",  "", 0x3A);
        TEST("PSHX", "", 0x3C);
        TEST("MUL",  "", 0x3D);
    }

    if (hd6301()) {
        // HD6301
        TEST("XGDX", "", 0x18);
        TEST("SLP",  "", 0x1A);
    }

    if (m68hc11()) {
        // MC68HC11
        TEST("IDIV", "", 0x02);
        TEST("FDIV", "", 0x03);
        TEST("XGDX", "", 0x8F);
        TEST("STOP", "", 0xCF);
        NMEM("", "", "", 0x18);
        TEST("INY",  "", 0x18, 0x08);
        TEST("DEY",  "", 0x18, 0x09);
        TEST("TSY",  "", 0x18, 0x30);
        TEST("TYS",  "", 0x18, 0x35);
        TEST("PULY", "", 0x18, 0x38);
        TEST("ABY",  "", 0x18, 0x3A);
        TEST("PSHY", "", 0x18, 0x3C);
        TEST("XGDY", "", 0x18, 0x8F);
    }
}

static void test_immediate() {
    TEST("SUBA", "#$90", 0x80, 0x90);
    TEST("CMPA", "#$90", 0x81, 0x90);
    TEST("SBCA", "#$90", 0x82, 0x90);
    TEST("ANDA", "#$90", 0x84, 0x90);
    TEST("BITA", "#$90", 0x85, 0x90);
    TEST("LDAA", "#$90", 0x86, 0x90);
    TEST("EORA", "#$90", 0x88, 0x90);
    TEST("ADCA", "#$90", 0x89, 0x90);
    TEST("ORAA", "#$90", 0x8A, 0x90);
    TEST("ADDA", "#$90",    0x8B, 0x90);
    NMEM("ADDA", "#0", "0", 0x8B);

    TEST("SUBB", "#$90", 0xC0, 0x90);
    TEST("CMPB", "#$90", 0xC1, 0x90);
    TEST("SBCB", "#$90", 0xC2, 0x90);
    TEST("ANDB", "#$90", 0xC4, 0x90);
    TEST("BITB", "#$90", 0xC5, 0x90);
    TEST("LDAB", "#$90", 0xC6, 0x90);
    TEST("EORB", "#$90", 0xC8, 0x90);
    TEST("ADCB", "#$90", 0xC9, 0x90);
    TEST("ORAB", "#$90", 0xCA, 0x90);
    TEST("ADDB", "#$90", 0xCB, 0x90);

    TEST("CPX", "#$90A0", 0x8C, 0x90, 0xA0);
    TEST("LDX", "#$90A0", 0xCE, 0x90, 0xA0);
    TEST("LDS", "#$90A0",          0x8E, 0x90, 0xA0);
    NMEM("LDS", "#$9000", "$9000", 0x8E, 0x90);
    NMEM("LDS", "#0",     "0",     0x8E);

    if (mb8861()) {
        // MB8861
        TEST("ADX", "#$90", 0xEC, 0x90);
    }

    if (m6801()) {
        // MC6801
        TEST("SUBD", "#$90A0", 0x83, 0x90, 0xA0);
        TEST("ADDD", "#$90A0", 0xC3, 0x90, 0xA0);
        TEST("LDD",  "#$90A0", 0xCC, 0x90, 0xA0);
    }

    if (m68hc11()) {
        // MC68HC11
        TEST("CPY", "#$90A0", 0x18, 0x8C, 0x90, 0xA0);
        TEST("LDY", "#$90A0", 0x18, 0xCE, 0x90, 0xA0);
        TEST("CPD", "#$90A0", 0x1A, 0x83, 0x90, 0xA0);
        NMEM("CPD", "#$9000", "$9000", 0x1A, 0x83, 0x90);
        NMEM("CPD", "#$9000", "$9000", 0x1A, 0x83, 0x90);
        NMEM("CPD", "#0",     "0",     0x1A, 0x83);
        NMEM("",    "",       "",      0x1A);
    }

    symtab.intern(0x90, "dir90");
    symtab.intern(0x90A0, "dir90A0");

    TEST("LDAB", "#dir90",   0xC6, 0x90);
    TEST("CPX",  "#dir90A0", 0x8C, 0x90, 0xA0);
    TEST("LDX",  "#dir90",   0xCE, 0x00, 0x90);
    TEST("LDS",  "#dir90A0", 0x8E, 0x90, 0xA0);

    if (m6801()) {
        // MC6801
        TEST("LDD",  "#dir90A0", 0xCC, 0x90, 0xA0);
        TEST("ADDD", "#dir90",   0xC3, 0x00, 0x90);
    }

    if (m68hc11()) {
        // MC68HC11
        TEST("CPY",  "#dir90A0", 0x18, 0x8C, 0x90, 0xA0);
        TEST("LDY",  "#dir90",   0x18, 0xCE, 0x00, 0x90);
        TEST("CPD",  "#dir90A0", 0x1A, 0x83, 0x90, 0xA0);
    }
}

static void test_direct() {
    TEST("SUBA", "$90", 0x90, 0x90);
    TEST("CMPA", "$90", 0x91, 0x90);
    TEST("SBCA", "$90", 0x92, 0x90);
    TEST("ANDA", "$90", 0x94, 0x90);
    TEST("BITA", "$90", 0x95, 0x90);
    TEST("LDAA", "$90", 0x96, 0x90);
    TEST("STAA", "$90", 0x97, 0x90);
    TEST("EORA", "$90", 0x98, 0x90);
    TEST("ADCA", "$90", 0x99, 0x90);
    TEST("ORAA", "$90", 0x9A, 0x90);
    TEST("ADDA", "$90",        0x9B, 0x90);
    NMEM("ADDA", "$00", "$00", 0x9B);

    TEST("SUBB", "$90", 0xD0, 0x90);
    TEST("CMPB", "$90", 0xD1, 0x90);
    TEST("SBCB", "$90", 0xD2, 0x90);
    TEST("ANDB", "$90", 0xD4, 0x90);
    TEST("BITB", "$90", 0xD5, 0x90);
    TEST("LDAB", "$90", 0xD6, 0x90);
    TEST("STAB", "$90", 0xD7, 0x90);
    TEST("EORB", "$90", 0xD8, 0x90);
    TEST("ADCB", "$90", 0xD9, 0x90);
    TEST("ORAB", "$90", 0xDA, 0x90);
    TEST("ADDB", "$90", 0xDB, 0x90);

    TEST("CPX", "$90", 0x9C, 0x90);
    TEST("LDX", "$90", 0xDE, 0x90);
    TEST("STX", "$90", 0xDF, 0x90);
    TEST("LDS", "$90", 0x9E, 0x90);
    TEST("STS", "$90", 0x9F, 0x90);

    if (m6801()) {
        // MC6801
        TEST("SUBD", "$90", 0x93, 0x90);
        TEST("ADDD", "$90", 0xD3, 0x90);
        TEST("LDD",  "$90", 0xDC, 0x90);
        TEST("STD",  "$90", 0xDD, 0x90);
        TEST("JSR",  "$90", 0x9D, 0x90);
    }

    if (m68hc11()) {
        // MC68HC11
        TEST("CPY", "$90", 0x18, 0x9C, 0x90);
        TEST("LDY", "$90", 0x18, 0xDE, 0x90);
        TEST("STY", "$90", 0x18, 0xDF, 0x90);
        TEST("CPD", "$90",        0x1A, 0x93, 0x90);
        NMEM("CPD", "$00", "$00", 0x1A, 0x93);
        NMEM("",    "",    "",    0x1A);
    }

    symtab.intern(0x10, "dir10");
    symtab.intern(0x22, "dir22");
    symtab.intern(0x90, "dir90");

    TEST("LDAA", "<dir90", 0x96, 0x90);
    TEST("STAB", "<dir90", 0xD7, 0x90);
    TEST("CPX",  "<dir22", 0x9C, 0x22);
    TEST("LDX",  "<dir22", 0xDE, 0x22);
    TEST("STX",  "<dir22", 0xDF, 0x22);
    TEST("LDS",  "<dir90", 0x9E, 0x90);
    TEST("STS",  "<dir90", 0x9F, 0x90);

    if (m6801()) {
        // MC6801
        TEST("LDD", "<dir90", 0xDC, 0x90);
        TEST("JSR", "<dir22", 0x9D, 0x22);
    }

    if (m68hc11()) {
        // MC68HC11
        TEST("CPY", "<dir90", 0x18, 0x9C, 0x90);
        TEST("STY", "<dir22", 0x18, 0xDF, 0x22);
        TEST("CPD", "<dir90", 0x1A, 0x93, 0x90);
    }
}

static void test_extended() {
    TEST("NEG", ">$0000", 0x70, 0x00, 0x00);
    TEST("COM", ">$0009", 0x73, 0x00, 0x09);
    TEST("LSR", ">$0034", 0x74, 0x00, 0x34);
    TEST("ROR", "$1234",  0x76, 0x12, 0x34);
    TEST("ASR", "$1234",  0x77, 0x12, 0x34);
    TEST("ASL", "$1234",  0x78, 0x12, 0x34);
    TEST("ROL", "$1234",  0x79, 0x12, 0x34);
    TEST("DEC", "$1234",  0x7A, 0x12, 0x34);
    TEST("INC", "$1234",  0x7C, 0x12, 0x34);
    TEST("TST", "$1234",  0x7D, 0x12, 0x34);
    TEST("CLR",  "$1234",           0x7F, 0x12, 0x34);
    NMEM("CLR",  "$1200",  "$1200", 0x7F, 0x12);
    NMEM("CLR", ">$0000", ">$0000", 0x7F);

    TEST("SUBA", ">$0090", 0xB0, 0x00, 0x90);
    TEST("CMPA", ">$0090", 0xB1, 0x00, 0x90);
    TEST("SBCA", ">$0090", 0xB2, 0x00, 0x90);
    TEST("ANDA", ">$0090", 0xB4, 0x00, 0x90);
    TEST("BITA", ">$0090", 0xB5, 0x00, 0x90);
    TEST("LDAA", ">$0090", 0xB6, 0x00, 0x90);
    TEST("STAA", ">$0090", 0xB7, 0x00, 0x90);
    TEST("EORA", ">$0090", 0xB8, 0x00, 0x90);
    TEST("ADCA", ">$0090", 0xB9, 0x00, 0x90);
    TEST("ORAA", ">$0090", 0xBA, 0x00, 0x90);
    TEST("ADDA", ">$0090", 0xBB, 0x00, 0x90);
    
    TEST("SUBB", "$9ABC", 0xF0, 0x9A, 0xBC);
    TEST("CMPB", "$9ABC", 0xF1, 0x9A, 0xBC);
    TEST("SBCB", "$9ABC", 0xF2, 0x9A, 0xBC);
    TEST("ANDB", "$9ABC", 0xF4, 0x9A, 0xBC);
    TEST("BITB", "$9ABC", 0xF5, 0x9A, 0xBC);
    TEST("LDAB", "$9ABC", 0xF6, 0x9A, 0xBC);
    TEST("STAB", "$9ABC", 0xF7, 0x9A, 0xBC);
    TEST("EORB", "$9ABC", 0xF8, 0x9A, 0xBC);
    TEST("ADCB", "$9ABC", 0xF9, 0x9A, 0xBC);
    TEST("ORAB", "$9ABC", 0xFA, 0x9A, 0xBC);
    TEST("ADDB", "$9ABC", 0xFB, 0x9A, 0xBC);

    TEST("CPX", "$9ABC", 0xBC, 0x9A, 0xBC);
    TEST("LDX", "$9ABC", 0xFE, 0x9A, 0xBC);
    TEST("STX", "$9ABC", 0xFF, 0x9A, 0xBC);
    TEST("LDS", "$9ABC", 0xBE, 0x9A, 0xBC);
    TEST("STS", "$9ABC", 0xBF, 0x9A, 0xBC);

    TEST("JMP", "$1234", 0x7E, 0x12, 0x34);
    TEST("JSR", "$1234", 0xBD, 0x12, 0x34);

    if (mb8861()) {
        // MB8861
        TEST("ADX", "$1234", 0xFC, 0x12, 0x34);
    }

    if (m6801()) {
        // MC6801
        TEST("SUBD", "$1234", 0xB3, 0x12, 0x34);
        TEST("ADDD", "$1234", 0xF3, 0x12, 0x34);
        TEST("LDD",  "$1234", 0xFC, 0x12, 0x34);
        TEST("STD",  "$1234", 0xFD, 0x12, 0x34);
    }

    if (m68hc11()) {
        // MC68HC11
        TEST("CPY", "$1234", 0x18, 0xBC, 0x12, 0x34);
        TEST("LDY", "$1234", 0x18, 0xFE, 0x12, 0x34);
        TEST("STY", "$1234", 0x18, 0xFF, 0x12, 0x34);
        TEST("CPD",  "$1234",           0x1A, 0xB3, 0x12, 0x34);
        NMEM("CPD",  "$1200",  "$1200", 0x1A, 0xB3, 0x12);
        NMEM("CPD", ">$0000", ">$0000", 0x1A, 0xB3);
        NMEM("",    "",       "",       0x1A);
    }

    symtab.intern(0x0090, "ext0090");
    symtab.intern(0x1ABC, "ext1ABC");
    symtab.intern(0x9ABC, "ext9ABC");

    TEST("NEG",  ">ext0090",  0x70, 0x00, 0x90);
    TEST("LDAA", ">ext9ABC",  0xB6, 0x9A, 0xBC);
    TEST("STAB", ">ext0090",  0xF7, 0x00, 0x90);
    TEST("CPX",  ">ext9ABC",  0xBC, 0x9A, 0xBC);
    TEST("LDX",  ">ext9ABC",  0xFE, 0x9A, 0xBC);
    TEST("STX",  ">ext0090",  0xFF, 0x00, 0x90);
    TEST("LDS",  ">ext9ABC",  0xBE, 0x9A, 0xBC);
    TEST("STS",  ">ext0090",  0xBF, 0x00, 0x90);
    TEST("JMP",  ">ext9ABC",  0x7E, 0x9A, 0xBC);
    TEST("JSR",  ">ext0090",  0xBD, 0x00, 0x90);

    if (m6801()) {
        // MC6801
        TEST("SUBD", ">ext0090", 0xB3, 0x00, 0x90);
        TEST("ADDD", ">ext9ABC", 0xF3, 0x9A, 0xBC);
        TEST("LDD",  ">ext0090", 0xFC, 0x00, 0x90);
        TEST("STD",  ">ext9ABC", 0xFD, 0x9A, 0xBC);
    }

    if (m68hc11()) {
        // MC68HC11
        TEST("LDY", ">ext0090", 0x18, 0xFE, 0x00, 0x90);
        TEST("STY", ">ext9ABC", 0x18, 0xFF, 0x9A, 0xBC);
        TEST("CPD", ">ext9ABC", 0x1A, 0xB3, 0x9A, 0xBC);
    }
}

static void test_indexed() {
    TEST("NEG", "0,X",  0x60, 0x00);
    TEST("COM", "0,X",  0x63, 0x00);
    TEST("LSR", "1,X",   0x64, 0x01);
    TEST("ROR", "2,X",   0x66, 0x02);
    TEST("ASR", "3,X",   0x67, 0x03);
    TEST("ASL", "4,X",   0x68, 0x04);
    TEST("ROL", "5,X",   0x69, 0x05);
    TEST("DEC", "6,X",   0x6A, 0x06);
    TEST("INC", "127,X", 0x6C, 0x7F);
    TEST("TST", "128,X", 0x6D, 0x80);
    TEST("CLR", "255,X",      0x6F, 0xFF);
    NMEM("CLR", "0,X", "0,X", 0x6F);

    TEST("SUBA", "0,X", 0xA0, 0x00);
    TEST("CMPA", "0,X", 0xA1, 0x00);
    TEST("SBCA", "1,X", 0xA2, 0x01);
    TEST("ANDA", "2,X", 0xA4, 0x02);
    TEST("BITA", "3,X", 0xA5, 0x03);
    TEST("LDAA", "4,X", 0xA6, 0x04);
    TEST("STAA", "5,X", 0xA7, 0x05);
    TEST("EORA", "6,X", 0xA8, 0x06);
    TEST("ADCA", "127,X", 0xA9, 0x7F);
    TEST("ORAA", "128,X", 0xAA, 0x80);
    TEST("ADDA", "255,X", 0xAB, 0xFF);

    TEST("SUBB", "0,X", 0xE0, 0x00);
    TEST("CMPB", "0,X", 0xE1, 0x00);
    TEST("SBCB", "1,X", 0xE2, 0x01);
    TEST("ANDB", "2,X", 0xE4, 0x02);
    TEST("BITB", "3,X", 0xE5, 0x03);
    TEST("LDAB", "4,X", 0xE6, 0x04);
    TEST("STAB", "5,X", 0xE7, 0x05);
    TEST("EORB", "6,X", 0xE8, 0x06);
    TEST("ADCB", "127,X", 0xE9, 0x7F);
    TEST("ORAB", "128,X", 0xEA, 0x80);
    TEST("ADDB", "255,X", 0xEB, 0xFF);

    TEST("CPX", "0,X", 0xAC, 0x00);
    TEST("LDX", "0,X", 0xEE, 0x00);
    TEST("STX", "2,X", 0xEF, 0x02);
    TEST("LDS", "128,X", 0xAE, 0x80);
    TEST("STS", "255,X", 0xAF, 0xFF);

    TEST("JMP", "0,X",   0x6E, 0x00);
    TEST("JSR", "255,X", 0xAD, 0xFF);

    if (m6801()) {
        // MC6801
        TEST("SUBD", "0,X",   0xA3, 0x00);
        TEST("ADDD", "128,X", 0xE3, 0x80);
        TEST("LDD",  "255,X", 0xEC, 0xFF);
        TEST("STD",  "0,X",   0xED, 0x00);
    }

    if (m68hc11()) {
        // MC68HC11
        TEST("CPD", "0,X",   0x1A, 0xA3, 0x00);
        TEST("LDY", "0,X",   0x1A, 0xEE, 0x00);
        TEST("STY", "128,X", 0x1A, 0xEF, 0x80);
        TEST("CPY", "255,X",      0x1A, 0xAC, 0xFF);
        NMEM("CPY", "0,X", "0,X", 0x1A, 0xAC);
        NMEM("",    "",    "",    0x1A);
    }

    symtab.intern(0,   "offset0");
    symtab.intern(128, "offset128");
    symtab.intern(255, "offset255");
    symtab.intern(256, "offset256");

    TEST("NEG",  "offset0,X",   0x60, 0x00);
    TEST("COM",  "offset255,X", 0x63, 0xFF);
    TEST("CMPA", "offset0,X",   0xA1, 0x00);
    TEST("ADDB", "offset255,X", 0xEB, 0xFF);
    TEST("JMP",  "offset0,X",   0x6E, 0x00);
    TEST("JSR",  "offset255,X", 0xAD, 0xFF);

    if (m6801()) {
        // MC6801
        TEST("SUBD", "offset0,X",   0xA3, 0x00);
        TEST("ADDD", "offset255,X", 0xE3, 0xFF);
        TEST("LDD",  "offset0,X",   0xEC, 0x00);
        TEST("STD",  "offset255,X", 0xED, 0xFF);
    }

    if (m68hc11()) {
        // MC68HC11
        TEST("CPD", "offset0,X",   0x1A, 0xA3, 0x00);
        TEST("LDY", "offset0,X",   0x1A, 0xEE, 0x00);
        TEST("STY", "offset128,X", 0x1A, 0xEF, 0x80);
        TEST("CPY", "offset255,X", 0x1A, 0xAC, 0xFF);
    }
}

static void test_indexed_y() {
    if (m68hc11()) {
        // MC68HC11
        TEST("NEG", "0,Y", 0x18, 0x60, 0x00);
        TEST("COM", "0,Y", 0x18, 0x63, 0x00);
        TEST("LSR", "1,Y", 0x18, 0x64, 0x01);
        TEST("ROR", "2,Y", 0x18, 0x66, 0x02);
        TEST("ASR", "3,Y", 0x18, 0x67, 0x03);
        TEST("ASL", "4,Y", 0x18, 0x68, 0x04);
        TEST("ROL", "5,Y", 0x18, 0x69, 0x05);
        TEST("DEC", "6,Y", 0x18, 0x6A, 0x06);
        TEST("INC", "127,Y", 0x18, 0x6C, 0x7F);
        TEST("TST", "128,Y", 0x18, 0x6D, 0x80);
        TEST("JMP", "254,Y", 0x18, 0x6E, 0xFE);
        TEST("CLR", "255,Y",      0x18, 0x6F, 0xFF);
        NMEM("CLR", "0,Y", "0,Y", 0x18, 0x6F);
        NMEM("",    "",    "",    0x18);

        TEST("SUBA", "0,Y", 0x18, 0xA0, 0x00);
        TEST("CMPA", "0,Y", 0x18, 0xA1, 0x00);
        TEST("SBCA", "1,Y", 0x18, 0xA2, 0x01);
        TEST("ANDA", "2,Y", 0x18, 0xA4, 0x02);
        TEST("BITA", "3,Y", 0x18, 0xA5, 0x03);
        TEST("LDAA", "4,Y", 0x18, 0xA6, 0x04);
        TEST("STAA", "5,Y", 0x18, 0xA7, 0x05);
        TEST("EORA", "6,Y", 0x18, 0xA8, 0x06);
        TEST("ADCA", "127,Y", 0x18, 0xA9, 0x7F);
        TEST("ORAA", "128,Y", 0x18, 0xAA, 0x80);
        TEST("ADDA", "255,Y", 0x18, 0xAB, 0xFF);

        TEST("SUBB", "0,Y", 0x18, 0xE0, 0x00);
        TEST("CMPB", "0,Y", 0x18, 0xE1, 0x00);
        TEST("SBCB", "1,Y", 0x18, 0xE2, 0x01);
        TEST("ANDB", "2,Y", 0x18, 0xE4, 0x02);
        TEST("BITB", "3,Y", 0x18, 0xE5, 0x03);
        TEST("LDAB", "4,Y", 0x18, 0xE6, 0x04);
        TEST("STAB", "5,Y", 0x18, 0xE7, 0x05);
        TEST("EORB", "6,Y", 0x18, 0xE8, 0x06);
        TEST("ADCB", "127,Y", 0x18, 0xE9, 0x7F);
        TEST("ORAB", "128,Y", 0x18, 0xEA, 0x80);
        TEST("ADDB", "255,Y", 0x18, 0xEB, 0xFF);

        TEST("CPY", "0,Y", 0x18, 0xAC, 0x00);
        TEST("LDY", "0,Y", 0x18, 0xEE, 0x00);
        TEST("STY", "2,Y", 0x18, 0xEF, 0x02);
        TEST("LDS", "128,Y", 0x18, 0xAE, 0x80);
        TEST("STS", "255,Y", 0x18, 0xAF, 0xFF);

        TEST("JMP", "0,Y",   0x18, 0x6E, 0x00);
        TEST("JSR", "255,Y", 0x18, 0xAD, 0xFF);

        TEST("SUBD", "0,Y",   0x18, 0xA3, 0x00);
        TEST("ADDD", "128,Y", 0x18, 0xE3, 0x80);
        TEST("LDD",  "255,Y", 0x18, 0xEC, 0xFF);
        TEST("STD",  "0,Y",   0x18, 0xED, 0x00);

        TEST("CPD", "0,Y",   0xCD, 0xA3, 0x00);
        TEST("LDX", "0,Y",   0xCD, 0xEE, 0x00);
        TEST("STX", "128,Y", 0xCD, 0xEF, 0x80);
        TEST("CPX", "255,Y", 0xCD, 0xAC, 0xFF);

        symtab.intern(0,   "offset0");
        symtab.intern(128, "offset128");
        symtab.intern(255, "offset255");

        // MC68HC11
        TEST("NEG",  "offset0,Y",   0x18, 0x60, 0x00);
        TEST("COM",  "offset255,Y", 0x18, 0x63, 0xFF);
        TEST("CMPA", "offset0,Y",   0x18, 0xA1, 0x00);
        TEST("ADDB", "offset255,Y", 0x18, 0xEB, 0xFF);
        TEST("JMP",  "offset0,Y",   0x18, 0x6E, 0x00);
        TEST("JSR",  "offset255,Y", 0x18, 0xAD, 0xFF);

        TEST("SUBD", "offset0,Y",   0x18, 0xA3, 0x00);
        TEST("ADDD", "offset255,Y", 0x18, 0xE3, 0xFF);
        TEST("LDD",  "offset0,Y",   0x18, 0xEC, 0x00);
        TEST("STD",  "offset255,Y", 0x18, 0xED, 0xFF);

        TEST("CPD", "offset0,Y",   0xCD, 0xA3, 0x00);
        TEST("LDX", "offset0,Y",   0xCD, 0xEE, 0x00);
        TEST("STX", "offset128,Y", 0xCD, 0xEF, 0x80);
        TEST("CPX", "offset255,Y", 0xCD, 0xAC, 0xFF);
    }
}

static void test_relative() {
    ATEST(0x1000, "BRA", "$1002", 0x20, 0x00);
    AERRT(0x0010, "BRA", "$FF92", OVERFLOW_RANGE, "$FF92", 0x20, 0x80);
    AERRT(0xFFF0, "BRA", "$0071", OVERFLOW_RANGE, "$0071", 0x20, 0x7F);
    ANMEM(0x1000, "BRA", "$1001",                 "$1001", 0x20);
    ATEST(0x1000, "BHI", "$1004", 0x22, 0x02);
    ATEST(0x1000, "BLS", "$1002", 0x23, 0x00);
    ATEST(0x1000, "BHS", "$1002", 0x24, 0x00);
    ATEST(0x1000, "BLO", "$1002", 0x25, 0x00);
    ATEST(0x1000, "BNE", "$1002", 0x26, 0x00);
    ATEST(0x1000, "BEQ", "$1002", 0x27, 0x00);
    ATEST(0x1000, "BPL", "$1002", 0x2A, 0x00);
    ATEST(0x1000, "BMI", "$1002", 0x2B, 0x00);

    ATEST(0x1000, "BVC", "$1002", 0x28, 0x00);
    ATEST(0x1000, "BVS", "$1002", 0x29, 0x00);
    ATEST(0x1000, "BGE", "$1002", 0x2C, 0x00);
    ATEST(0x1000, "BLT", "$1002", 0x2D, 0x00);
    ATEST(0x1000, "BGT", "$1002", 0x2E, 0x00);
    ATEST(0x1000, "BLE", "$1002", 0x2F, 0x00);

    ATEST(0x1000, "BSR", "$1042", 0x8D, 0x40);

    if (m6801()) {
        // MC6801
        ATEST(0x1000, "BRN", "$1081", 0x21, 0x7F);
    }

    symtab.intern(0x0F82, "sub0F82");
    symtab.intern(0x1081, "sub1081");

    ATEST(0x1000, "BSR", "sub1081", 0x8D, 0x7F);
    ATEST(0x1000, "BSR", "sub0F82", 0x8D, 0x80);

    if (m6801()) {
        // MC6801
        ATEST(0x1000, "BRN", "sub0F82", 0x21, 0x80);
    }

    disassembler.setOption("relative", "enable");
    ATEST(0x2000, "BSR", "*-126", 0x8D, 0x80);
    ATEST(0x2000, "BSR", "*",     0x8D, 0xFE);
    ATEST(0x2000, "BSR", "*+2",   0x8D, 0x00);
    ATEST(0x2000, "BSR", "*+129", 0x8D, 0x7F);
}

static void test_bit_ops() {
    if (m68hc11()) {
        // MC68HC11
        TEST("BSET", "$90, #$88",   0x14, 0x90, 0x88);
        TEST("BCLR", "$90, #$88",          0x15, 0x90, 0x88);
        NMEM("BCLR", "$90, #0",       "0", 0x15, 0x90);
        NMEM("BCLR", "$00, #0", "$00, #0", 0x15);
        TEST("BSET", "127,X, #$88", 0x1C, 0x7F, 0x88);
        TEST("BCLR", "128,X, #$88", 0x1D, 0x80, 0x88);
        TEST("BSET", "255,Y, #$88",          0x18, 0x1C, 0xFF, 0x88);
        NMEM("BSET", "255,Y, #0",       "0", 0x18, 0x1C, 0xFF);
        NMEM("BSET",   "0,Y, #0", "0,Y, #0", 0x18, 0x1C);
        TEST("BCLR", "0,Y, #$88",   0x18, 0x1D, 0x00, 0x88);

        ATEST(0x1000, "BRSET", "$90, #$88, $1083",   0x12, 0x90, 0x88, 0x7F);
        ATEST(0x1000, "BRCLR", "$90, #$88, $0F84",   0x13, 0x90, 0x88, 0x80);
        ATEST(0x1000, "BRSET", "127,X, #$88, $1000", 0x1E, 0x7F, 0x88, 0xFC);
        ATEST(0x1000, "BRCLR", "128,X, #$88, $1006", 0x1F, 0x80, 0x88, 0x02);
        ATEST(0x1000, "BRSET", "255,Y, #$88, $0F85", 0x18, 0x1E, 0xFF, 0x88, 0x80);
        ATEST(0x1000, "BRCLR", "0,Y, #$88, $1084",   0x18, 0x1F, 0x00, 0x88, 0x7F);
    }

    if (hd6301()) {
        // HD6301
        TEST("AIM", "#$88, 0,X",   0x61, 0x88, 0x00);
        TEST("OIM", "#$44, 1,X",   0x62, 0x44, 0x01);
        TEST("EIM", "#$22, 128,X", 0x65, 0x22, 0x80);
        TEST("TIM", "#$33, 255,X",        0x6B, 0x33, 0xFF);
        NMEM("TIM", "#$33, 0,X",   "0,X", 0x6B, 0x33);
        NMEM("TIM", "#0, 0,X", "#0, 0,X", 0x6B);
        TEST("BCLR", "0, 1,X",     0x61, 0xFE, 0x01);
        TEST("BSET", "1, 128,X",   0x62, 0x02, 0x80);
        TEST("BTGL", "6, 255,X",          0x65, 0x40, 0xFF);
        NMEM("BTGL", "6, 0,X",     "0,X", 0x65, 0x40);
        NMEM("EIM", "#0, 0,X", "#0, 0,X", 0x65);
        TEST("BTST", "7, 0,X",     0x6B, 0x80, 0x00);

        TEST("AIM", "#$88, $90", 0x71, 0x88, 0x90);
        TEST("OIM", "#$44, $90", 0x72, 0x44, 0x90);
        TEST("EIM", "#$22, $90", 0x75, 0x22, 0x90);
        TEST("TIM", "#$33, $90", 0x7B, 0x33, 0x90);
        TEST("BCLR", "7, $90",   0x71, 0x7F, 0x90);
        TEST("BSET", "6, $90",   0x72, 0x40, 0x90);
        TEST("BTGL", "1, $90",   0x75, 0x02, 0x90);
        TEST("BTST", "0, $90",   0x7B, 0x01, 0x90);
    }

    if (mb8861()) {
        // MB8861
        TEST("NIM", "#$88, 0,X",   0x71, 0x88, 0x00);
        TEST("OIM", "#$44, 1,X",   0x72, 0x44, 0x01);
        TEST("XIM", "#$22, 128,X", 0x75, 0x22, 0x80);
        TEST("TMM", "#$33, 255,X",       0x7B, 0x33, 0xFF);
        NMEM("TMM", "#$33, 0,X",  "0,X", 0x7B, 0x33);
        NMEM("TMM", "#0, 0,X", "0, 0,X", 0x7B);
    }

    symtab.intern(0x90, "dir90");
    symtab.intern(255,  "offset255");
    symtab.intern(128,  "offset128");
    symtab.intern(127,  "offset127");
    symtab.intern(10,   "offset10");
    symtab.intern(0x88, "data88");
    symtab.intern(0x0F83, "sym0F83");
    symtab.intern(0x0F84, "sym0F84");
    symtab.intern(0x0F85, "sym0F85");
    symtab.intern(0x1000, "sym1000");
    symtab.intern(0x1006, "sym1006");
    symtab.intern(0x1082, "sym1082");
    symtab.intern(0x1083, "sym1083");
    symtab.intern(0x1084, "sym1084");
    symtab.intern(0x10, "bm4");
    symtab.intern(0x40, "bm6");
    symtab.intern(4, "bp4");
    symtab.intern(6, "bp6");

    if (m68hc11()) {
        // MC68HC11
        TEST("BSET", "<dir90, #data88",      0x14, 0x90, 0x88);
        TEST("BCLR", "<dir90, #data88",      0x15, 0x90, 0x88);
        TEST("BSET", "offset127,X, #data88", 0x1C, 0x7F, 0x88);
        TEST("BCLR", "offset128,X, #data88", 0x1D, 0x80, 0x88);
        TEST("BSET", "offset255,Y, #data88", 0x18, 0x1C, 0xFF, 0x88);
        TEST("BCLR", "offset10,Y, #data88",  0x18, 0x1D, 0x0A, 0x88);

        ATEST(0x1000, "BRSET", "<dir90, #data88, sym1083",      0x12, 0x90, 0x88, 0x7F);
        ATEST(0x1000, "BRCLR", "<dir90, #data88, sym0F84",      0x13, 0x90, 0x88, 0x80);
        ATEST(0x1000, "BRSET", "offset127,X, #data88, sym1000", 0x1E, 0x7F, 0x88, 0xFC);
        ATEST(0x1000, "BRCLR", "offset128,X, #data88, sym1006", 0x1F, 0x80, 0x88, 0x02);
        ATEST(0x1000, "BRSET", "offset255,Y, #data88, sym0F85", 0x18, 0x1E, 0xFF, 0x88, 0x80);
        ATEST(0x1000, "BRCLR", "offset10,Y, #data88, sym1084",  0x18, 0x1F, 0x0A, 0x88, 0x7F);
    }

    if (hd6301()) {
        // HD6301
        TEST("AIM",  "#data88, offset10,X",  0x61, 0x88, 0x0A);
        TEST("OIM",  "#data88, <dir90",      0x72, 0x88, 0x90);
        TEST("EIM",  "#data88, offset255,X", 0x65, 0x88, 0xFF);
        TEST("BCLR", "bp4, offset10,X",      0x61, 0xEF, 0x0A);
        TEST("BSET", "bp4, <dir90",          0x72, 0x10, 0x90);
        TEST("BTGL", "bp6, offset255,X",     0x65, 0x40, 0xFF);
    }
}

static void test_illegal_mc6800() {
    static constexpr Config::opcode_t illegals[] = {
        0x00, 0x02, 0x03, 0x04, 0x05,
        0x12, 0x13, 0x14, 0x15, 0x18, 0x1A, 0x1C, 0x1D, 0x1E, 0x1F,
        0x21, 0x38, 0x3A, 0x3C, 0x3D,
        0x41, 0x42, 0x45, 0x4B, 0x4E, 0x51, 0x52, 0x55, 0x5B, 0x5E,
        0x61, 0x62, 0x65, 0x6B, 0x71, 0x72, 0x75, 0x7B,
        0x83, 0x87, 0x8F, 0x93, 0x9D, 0xA3, 0xB3,
        0xC3, 0xC7, 0xCC, 0xCD, 0xCF, 0xD3, 0xDC, 0xDD,
        0xE3, 0xEC, 0xED, 0xF3, 0xFC, 0xFD,
    };
    for (const auto opc : illegals)
        UNKN(opc);
}

static void test_illegal_mb8861() {
    static constexpr Config::opcode_t illegals[] = {
        0x00, 0x02, 0x03, 0x04, 0x05,
        0x12, 0x13, 0x14, 0x15, 0x18, 0x1A, 0x1C, 0x1D, 0x1E, 0x1F,
        0x21, 0x38, 0x3A, 0x3C, 0x3D,
        0x41, 0x42, 0x45, 0x4B, 0x4E, 0x51, 0x52, 0x55, 0x5B, 0x5E,
        0x61, 0x62, 0x65, 0x6B,
        0x83, 0x87, 0x8F, 0x93, 0x9D, 0xA3, 0xB3,
        0xC3, 0xC7, 0xCC, 0xCD, 0xCF, 0xD3, 0xDC, 0xDD,
        0xE3, 0xED, 0xF3, 0xFD,
    };
    for (const auto opc : illegals)
        UNKN(opc);
}

static void test_illegal_mc6801() {
    static constexpr Config::opcode_t illegals[] = {
        0x00, 0x02, 0x03,
        0x12, 0x13, 0x14, 0x15, 0x18, 0x1A, 0x1C, 0x1D, 0x1E, 0x1F,
        0x41, 0x42, 0x45, 0x4B, 0x4E, 0x51, 0x52, 0x55, 0x5B, 0x5E,
        0x61, 0x62, 0x65, 0x6B, 0x71, 0x72, 0x75, 0x7B,
        0x87, 0x8F,
        0xC7, 0xCD, 0xCF,
    };
    for (const auto opc : illegals)
        UNKN(opc);
}

static void test_illegal_hd6301() {
    static constexpr Config::opcode_t illegals[] = {
        0x00, 0x02, 0x03,
        0x12, 0x13, 0x14, 0x15, 0x1C, 0x1D, 0x1E, 0x1F,
        0x41, 0x42, 0x45, 0x4B, 0x4E, 0x51, 0x52, 0x55, 0x5B, 0x5E,
        0x87, 0x8F,
        0xC7, 0xCD, 0xCF,
    };
    for (const auto opc : illegals)
        UNKN(opc);
}

static void test_illegal_mc68hc11() {
    static constexpr Config::opcode_t p00_illegals[] = {
        0x00,
        0x41, 0x42, 0x45, 0x4B, 0x4E, 0x51, 0x52, 0x55, 0x5B, 0x5E,
        0x61, 0x62, 0x65, 0x6B, 0x71, 0x72, 0x75, 0x7B,
        0x87,
        0xC7,
    };
    for (const auto opc : p00_illegals)
        UNKN(opc);

    static constexpr Config::opcode_t p18_legals[] = {
        0x08, 0x09,
        0x1c, 0x1d, 0x1e, 0x1f,
        0x30, 0x35, 0x38, 0x3a, 0x3c,
        0x60, 0x63, 0x64, 0x66, 0x67,
        0x68, 0x69, 0x6a, 0x6c, 0x6d, 0x6e, 0x6f,
        0x8c, 0x8f, 0x9c,
        0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
        0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
        0xbc, 0xce, 0xde, 0xdf,
        0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7,
        0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
        0xfe, 0xff, 0
    };
    Config::opcode_t opc = 0;
    for (const auto legal : p18_legals) {
        while (opc != legal) {
            UNKN(0x18, opc);
            opc++;
        }
        opc++;
    }

    static constexpr Config::opcode_t p1a_legals[] = {
        0x83, 0x93, 0xa3, 0xac, 0xb3, 0xee, 0xef, 0
    };
    opc = 0;
    for (const auto legal : p1a_legals) {
        while (opc != legal) {
            UNKN(0x1A, opc);
            opc++;
        }
        opc++;
    }

    static constexpr Config::opcode_t pcd_legals[] = {
        0xa3, 0xac, 0xee, 0xef, 0
    };
    opc = 0;
    for (const auto legal : pcd_legals) {
        while (opc != legal) {
            UNKN(0xCD, opc);
            opc++;
        }
        opc++;
    }
}
// clang-format on

void run_tests(const char *cpu) {
    disassembler.setCpu(cpu);
    RUN_TEST(test_inherent);
    RUN_TEST(test_immediate);
    RUN_TEST(test_direct);
    RUN_TEST(test_extended);
    RUN_TEST(test_indexed);
    RUN_TEST(test_indexed_y);
    RUN_TEST(test_relative);
    RUN_TEST(test_bit_ops);
    if (mb8861()) {
        RUN_TEST(test_illegal_mb8861);
    } else if (m6800()) {
        RUN_TEST(test_illegal_mc6800);
    } else if (hd6301()) {
        RUN_TEST(test_illegal_hd6301);
    } else if (m68hc11()) {
        RUN_TEST(test_illegal_mc68hc11);
    } else {
        RUN_TEST(test_illegal_mc6801);
    }
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

/*
 * Copyright 2021 Tadashi G. Takaoka
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

#include "dis_mc6805.h"
#include "test_dis_helper.h"
#include "test_symtab.h"

using namespace libasm;
using namespace libasm::mc6805;
using namespace libasm::test;

DisMc6805 dis6805;
Disassembler &disassembler(dis6805);

static bool m146805() {
    return strcmp_P("146805", disassembler.cpu_P()) == 0;
}

static bool m68hc05() {
    return strcmp_P("68HC05", disassembler.cpu_P()) == 0;
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
    EQUALS("cpu 6805", true, disassembler.setCpu("6805"));
    EQUALS_P("cpu 6805", "6805", disassembler.cpu_P());

    EQUALS("cpu 146805", true, disassembler.setCpu("146805"));
    EQUALS_P("cpu 146805", "146805", disassembler.cpu_P());

    EQUALS("cpu 68HC05", true, disassembler.setCpu("68HC05"));
    EQUALS_P("cpu 68GC05", "68HC05", disassembler.cpu_P());

    EQUALS("cpu MC6805", true, disassembler.setCpu("MC6805"));
    EQUALS_P("cpu MC6805", "6805", disassembler.cpu_P());

    EQUALS("cpu MC146805", true, disassembler.setCpu("MC146805"));
    EQUALS_P("cpu MC146805", "146805", disassembler.cpu_P());

    EQUALS("cpu MC68HC05", true, disassembler.setCpu("MC68HC05"));
    EQUALS_P("cpu MC68GC05", "68HC05", disassembler.cpu_P());
}

static void test_inherent() {
    TEST("NOP", "", 0x9D);
    TEST("CLC", "", 0x98);
    TEST("SEC", "", 0x99);
    TEST("CLI", "", 0x9A);
    TEST("SEI", "", 0x9B);

    TEST("RTS", "", 0x81);
    TEST("RTI", "", 0x80);
    TEST("SWI", "", 0x83);

    TEST("TAX",  "", 0x97);
    TEST("TXA",  "", 0x9F);
    if (m146805() || m68hc05()) {
        // MC146805/MC68HC05
        TEST("WAIT", "", 0x8F);
    }

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

    TEST("NEGX", "", 0x50);
    TEST("COMX", "", 0x53);
    TEST("LSRX", "", 0x54);
    TEST("RORX", "", 0x56);
    TEST("ASRX", "", 0x57);
    TEST("ASLX", "", 0x58);
    TEST("ROLX", "", 0x59);
    TEST("DECX", "", 0x5A);
    TEST("INCX", "", 0x5C);
    TEST("TSTX", "", 0x5D);
    TEST("CLRX", "", 0x5F);
}

static void test_immediate() {
    TEST("SUB", "#$90", 0xA0, 0x90);
    TEST("CMP", "#$90", 0xA1, 0x90);
    TEST("SBC", "#$90", 0xA2, 0x90);
    TEST("AND", "#$90", 0xA4, 0x90);
    TEST("BIT", "#$90", 0xA5, 0x90);
    TEST("LDA", "#$90", 0xA6, 0x90);
    TEST("EOR", "#$90", 0xA8, 0x90);
    TEST("ADC", "#$90", 0xA9, 0x90);
    TEST("ORA", "#$90", 0xAA, 0x90);
    TEST("ADD", "#$90", 0xAB, 0x90);

    TEST("CPX", "#$90", 0xA3, 0x90);
    TEST("LDX", "#$90", 0xAE, 0x90);

    symtab.intern(0x90, "dir90");
    symtab.intern(0x90A0, "dir90A0");

    TEST("LDA", "#dir90", 0xA6, 0x90);
    TEST("CPX", "#dir90", 0xA3, 0x90);
    TEST("LDX", "#dir90", 0xAE, 0x90);
}

static void test_direct() {
    TEST("SUB", "$90", 0xB0, 0x90);
    TEST("CMP", "$90", 0xB1, 0x90);
    TEST("SBC", "$90", 0xB2, 0x90);
    TEST("AND", "$90", 0xB4, 0x90);
    TEST("BIT", "$90", 0xB5, 0x90);
    TEST("LDA", "$90", 0xB6, 0x90);
    TEST("STA", "$90", 0xB7, 0x90);
    TEST("EOR", "$90", 0xB8, 0x90);
    TEST("ADC", "$90", 0xB9, 0x90);
    TEST("ORA", "$90", 0xBA, 0x90);
    TEST("ADD", "$90", 0xBB, 0x90);

    TEST("CPX", "$90", 0xB3, 0x90);
    TEST("LDX", "$90", 0xBE, 0x90);
    TEST("STX", "$90", 0xBF, 0x90);

    symtab.intern(0x10, "dir10");
    symtab.intern(0x22, "dir22");
    symtab.intern(0x90, "dir90");

    TEST("LDA", "<dir90", 0xB6, 0x90);
    TEST("STA", "<dir90", 0xB7, 0x90);
    TEST("CPX", "<dir22", 0xB3, 0x22);
    TEST("LDX", "<dir22", 0xBE, 0x22);
    TEST("STX", "<dir22", 0xBF, 0x22);
}

static void test_extended() {
    TEST("SUB", "$1ABC", 0xC0, 0x1A, 0xBC);
    TEST("CMP", "$1ABC", 0xC1, 0x1A, 0xBC);
    TEST("SBC", "$1ABC", 0xC2, 0x1A, 0xBC);
    TEST("AND", "$1ABC", 0xC4, 0x1A, 0xBC);
    TEST("BIT", "$1ABC", 0xC5, 0x1A, 0xBC);
    TEST("LDA", "$1ABC", 0xC6, 0x1A, 0xBC);
    TEST("STA", "$1ABC", 0xC7, 0x1A, 0xBC);
    TEST("EOR", "$1ABC", 0xC8, 0x1A, 0xBC);
    TEST("ADC", "$1ABC", 0xC9, 0x1A, 0xBC);
    TEST("ORA", "$1ABC", 0xCA, 0x1A, 0xBC);
    TEST("ADD", "$1ABC", 0xCB, 0x1A, 0xBC);

    TEST("CPX", "$1ABC", 0xC3, 0x1A, 0xBC);
    TEST("LDX", "$1ABC", 0xCE, 0x1A, 0xBC);
    TEST("STX", "$1ABC", 0xCF, 0x1A, 0xBC);

    TEST("JMP", "$1234", 0xCC, 0x12, 0x34);
    TEST("JSR", "$1234", 0xCD, 0x12, 0x34);

    symtab.intern(0x0090, "ext0090");
    symtab.intern(0x1ABC, "ext1ABC");
    symtab.intern(0x9ABC, "ext9ABC");

    TEST("LDA", ">ext1ABC",  0xC6, 0x1A, 0xBC);
    TEST("STA", ">ext0090",  0xC7, 0x00, 0x90);
    TEST("CPX", ">ext1ABC",  0xC3, 0x1A, 0xBC);
    TEST("LDX", ">ext1ABC",  0xCE, 0x1A, 0xBC);
    TEST("STX", ">ext0090",  0xCF, 0x00, 0x90);
    TEST("JMP", ">ext1ABC",  0xCC, 0x1A, 0xBC);
    TEST("JSR", ">ext0090",  0xCD, 0x00, 0x90);

    dis6805.setOption("pc-bits", "11"); // MC68HC05J for instance
    TEST(         "LDA", "$7FF",                 0xC6, 0x07, 0xFF);
    ERRT(         "LDA", "$800", OVERFLOW_RANGE, 0xC6, 0x08, 0x00);
    ATEST(0x07F0, "BSR", "$7FF",                 0xAD, 0x0D);
    AERRT(0x07F0, "BSR", "$800", OVERFLOW_RANGE, 0xAD, 0x0E);

    dis6805.setOption("pc-bits", "0");  // Most of MC68HC05 has 13bits PC.
    TEST(         "LDA", "$1FFF",                 0xC6, 0x1F, 0xFF);
    ERRT(         "LDA", "$2000", OVERFLOW_RANGE, 0xC6, 0x20, 0x00);
    ATEST(0x1FF0, "BSR", "$1FFF",                 0xAD, 0x0D);
    AERRT(0x1FF0, "BSR", "$2000", OVERFLOW_RANGE, 0xAD, 0x0E);

    dis6805.setOption("pc-bits", "14"); // MC68HC05X for instance
    TEST(         "LDA", "$3FFF",                 0xC6, 0x3F, 0xFF);
    ERRT(         "LDA", "$4000", OVERFLOW_RANGE, 0xC6, 0x40, 0x00);
    ATEST(0x3FF0, "BSR", "$3FFF",                 0xAD, 0x0D);
    AERRT(0x3FF0, "BSR", "$4000", OVERFLOW_RANGE, 0xAD, 0x0E);
}

static void test_indexed() {
    TEST("NEG", "<0,X", 0x60, 0x00);
    TEST("COM", "<0,X", 0x63, 0x00);
    TEST("LSR", "1,X",   0x64, 0x01);
    TEST("ROR", "2,X",   0x66, 0x02);
    TEST("ASR", "3,X",   0x67, 0x03);
    TEST("ASL", "4,X",   0x68, 0x04);
    TEST("ROL", "5,X",   0x69, 0x05);
    TEST("DEC", "6,X",   0x6A, 0x06);
    TEST("INC", "127,X", 0x6C, 0x7F);
    TEST("TST", "128,X", 0x6D, 0x80);
    TEST("CLR", "255,X", 0x6F, 0xFF);

    TEST("SUB", ",X", 0xF0);
    TEST("CMP", ",X", 0xF1);
    TEST("SBC", ",X", 0xF2);
    TEST("AND", ",X", 0xF4);
    TEST("BIT", ",X", 0xF5);
    TEST("LDA", ",X", 0xF6);
    TEST("STA", ",X", 0xF7);
    TEST("EOR", ",X", 0xF8);
    TEST("ADC", ",X", 0xF9);
    TEST("ORA", ",X", 0xFA);
    TEST("ADD", ",X", 0xFB);
    TEST("CPX", ",X", 0xF3);
    TEST("LDX", ",X", 0xFE);
    TEST("STX", ",X", 0xFF);
    TEST("JMP", ",X", 0xFC);
    TEST("JSR", ",X", 0xFD);

    TEST("SUB", "<0,X",  0xE0, 0x00);
    TEST("CMP", "<0,X",  0xE1, 0x00);
    TEST("SBC", "1,X",   0xE2, 0x01);
    TEST("AND", "2,X",   0xE4, 0x02);
    TEST("BIT", "3,X",   0xE5, 0x03);
    TEST("LDA", "4,X",   0xE6, 0x04);
    TEST("STA", "5,X",   0xE7, 0x05);
    TEST("EOR", "6,X",   0xE8, 0x06);
    TEST("ADC", "127,X", 0xE9, 0x7F);
    TEST("ORA", "128,X", 0xEA, 0x80);
    TEST("ADD", "255,X", 0xEB, 0xFF);
    TEST("CPX", "127,X", 0xE3, 0x7F);
    TEST("LDX", "128,X", 0xEE, 0x80);
    TEST("STX", "255,X", 0xEF, 0xFF);
    TEST("JMP", "<0,X",  0xEC, 0x00);
    TEST("JSR", "255,X", 0xED, 0xFF);

    TEST("SUB", ">0,X",    0xD0, 0x00, 0x00);
    TEST("CMP", ">255,X",  0xD1, 0x00, 0xFF);
    TEST("SBC", "256,X",   0xD2, 0x01, 0x00);
    TEST("AND", "512,X",   0xD4, 0x02, 0x00);
    TEST("BIT", "1024,X",  0xD5, 0x04, 0x00);
    TEST("LDA", "2048,X",  0xD6, 0x08, 0x00);
    TEST("STA", "4096,X",  0xD7, 0x10, 0x00);
    TEST("EOR", "8192,X",  0xD8, 0x20, 0x00);
    TEST("ADC", "16384,X", 0xD9, 0x40, 0x00);
    TEST("ORA", "32768,X", 0xDA, 0x80, 0x00);
    TEST("ADD", "65535,X", 0xDB, 0xFF, 0xFF);
    TEST("CPX", ">0,X",    0xD3, 0x00, 0x00);
    TEST("LDX", ">0,X",    0xDE, 0x00, 0x00);
    TEST("STX", ">2,X",    0xDF, 0x00, 0x02);
    TEST("JMP", ">0,X",    0xDC, 0x00, 0x00);
    TEST("JSR", ">255,X",  0xDD, 0x00, 0xFF);

    symtab.intern(0,   "offset0");
    symtab.intern(128, "offset128");
    symtab.intern(255, "offset255");
    symtab.intern(256, "offset256");

    TEST("NEG", "<offset0,X",  0x60, 0x00);
    TEST("COM", "offset255,X", 0x63, 0xFF);

    TEST("CMP", "<offset0,X",  0xE1, 0x00);
    TEST("ADD", "offset255,X", 0xEB, 0xFF);
    TEST("JMP", "<offset0,X",  0xEC, 0x00);
    TEST("JSR", "offset255,X", 0xED, 0xFF);

    TEST("CMP", "offset256,X", 0xD1, 0x01, 0x00);
    TEST("ADD", "offset256,X", 0xDB, 0x01, 0x00);
    TEST("JMP", "offset256,X", 0xDC, 0x01, 0x00);
    TEST("JSR", "offset256,X", 0xDD, 0x01, 0x00);
}

static void test_relative() {
    ATEST(0x1000, "BRA", "$1002", 0x20, 0x00);
    ATEST(0x1000, "BHI", "$1004", 0x22, 0x02);
    ATEST(0x1000, "BLS", "$1002", 0x23, 0x00);
    ATEST(0x1000, "BHS", "$1002", 0x24, 0x00);
    ATEST(0x1000, "BLO", "$1002", 0x25, 0x00);
    ATEST(0x1000, "BNE", "$1002", 0x26, 0x00);
    ATEST(0x1000, "BEQ", "$1002", 0x27, 0x00);
    ATEST(0x1000, "BPL", "$1002", 0x2A, 0x00);
    ATEST(0x1000, "BMI", "$1002", 0x2B, 0x00);

    ATEST(0x1000, "BHCC", "$1002", 0x28, 0x00);
    ATEST(0x1000, "BHCS", "$1002", 0x29, 0x00);
    ATEST(0x1000, "BMC",  "$1002", 0x2C, 0x00);
    ATEST(0x1000, "BMS",  "$1002", 0x2D, 0x00);
    ATEST(0x1000, "BIL",  "$1002", 0x2E, 0x00);
    ATEST(0x1000, "BIH",  "$1002", 0x2F, 0x00);

    ATEST(0x1000, "BSR",  "$1042", 0xAD, 0x40);

    symtab.intern(0x0F82, "sub0F82");
    symtab.intern(0x1081, "sub1081");

    ATEST(0x1000, "BSR", "sub1081", 0xAD, 0x7F);
    ATEST(0x1000, "BSR", "sub0F82", 0xAD, 0x80);

    disassembler.setOption("relative", "enable");

    ATEST(0x1000, "BSR", "sub0F82", 0xAD, 0x80);
    ATEST(0x1000, "BSR", "*",       0xAD, 0xFE);
    ATEST(0x1000, "BSR", "*+2",     0xAD, 0x00);
    ATEST(0x1000, "BSR", "sub1081", 0xAD, 0x7F);
    AERRT(0x1FF0, "BSR", "*+16",    OVERFLOW_RANGE, 0xAD, 0x0E);
}

static void test_bit_ops() {
    TEST("BSET", "0, $90", 0x10, 0x90);
    TEST("BSET", "7, $90", 0x1E, 0x90);
    TEST("BCLR", "0, $90", 0x11, 0x90);
    TEST("BCLR", "7, $90", 0x1F, 0x90);

    disassembler.setOption("relative", "yes");
    ATEST(0x1000, "BRSET", "0, $90, *+130", 0x00, 0x90, 0x7F);
    ATEST(0x1000, "BRSET", "7, $90, *-125", 0x0E, 0x90, 0x80);
    ATEST(0x1000, "BRCLR", "0, $90, *+130", 0x01, 0x90, 0x7F);
    ATEST(0x1000, "BRCLR", "7, $90, *-125", 0x0F, 0x90, 0x80);
    AERRT(0x1FF0, "BRCLR", "7, $90, *+16",  OVERFLOW_RANGE, 0x0F, 0x90, 0x0E);
    disassembler.setOption("relative", "disable");

    symtab.intern(0x90, "dir90");
    symtab.intern(0x0F83, "sym0F83");
    symtab.intern(0x1082, "sym1082");
    symtab.intern(4, "bp4");
    symtab.intern(6, "bp6");

    TEST("BSET", "bp4, <dir90", 0x18, 0x90);
    TEST("BCLR", "bp6, <dir90", 0x1D, 0x90);

    ATEST(0x1000, "BRSET", "bp4, <dir90, sym1082", 0x08, 0x90, 0x7F);
    ATEST(0x1000, "BRCLR", "bp6, <dir90, sym0F83", 0x0D, 0x90, 0x80);
}

static void test_illegal() {
    const uint8_t illegals[] = {
        0x31, 0x32, 0x35, 0x3B, 0x3E,
        0x41, 0x45, 0x4B, 0x4E,
        0x51, 0x52, 0x55, 0x5B, 0x5E,
        0x61, 0x62, 0x65, 0x6B, 0x6E,
        0x71, 0x72, 0x75, 0x7B, 0x7E,
        0x82, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D,
        0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x9E,
        0xA7, 0xAC, 0xAF,
    };
    for (uint8_t idx = 0; idx < sizeof(illegals); idx++)
        UNKN(illegals[idx]);
    if (m68hc05()) {
        return;
    } else if (m146805()) {
        UNKN(0x42);             // MUL
    } else {
        UNKN(0x42);             // MUL
        UNKN(0x8E);             // STOP
        UNKN(0x8F);             // WAIT
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
    RUN_TEST(test_relative);
    RUN_TEST(test_bit_ops);
    RUN_TEST(test_illegal);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

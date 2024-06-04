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

#include "asm_mc6805.h"
#include "test_asm_helper.h"

using namespace libasm;
using namespace libasm::mc6805;
using namespace libasm::test;

AsmMc6805 asm6805;
Assembler &assembler(asm6805);

static bool m146805() {
    return strcmp_P("146805", assembler.config().cpu_P()) == 0;
}

static bool m68hc05() {
    return strcmp_P("68HC05", assembler.config().cpu_P()) == 0;
}

static void set_up() {
    assembler.reset();
}

static void tear_down() {
    symtab.reset();
}

// clang-format off
void test_cpu() {
    EQUALS("cpu 6805", true,   assembler.setCpu("6805"));
    EQUALS_P("cpu 6805", "6805", assembler.config().cpu_P());

    EQUALS("cpu 146805", true,   assembler.setCpu("146805"));
    EQUALS_P("cpu 146805", "146805", assembler.config().cpu_P());

    EQUALS("cpu 68HC05", true,   assembler.setCpu("68hc05"));
    EQUALS_P("cpu 68HC05", "68HC05", assembler.config().cpu_P());

    EQUALS("cpu MC6805", true,   assembler.setCpu("mc6805"));
    EQUALS_P("cpu MC6805", "6805", assembler.config().cpu_P());

    EQUALS("cpu MC146805", true,   assembler.setCpu("mc146805"));
    EQUALS_P("cpu MC146805", "146805", assembler.config().cpu_P());

    EQUALS("cpu MC68HC05", true,   assembler.setCpu("mc68hc05"));
    EQUALS_P("cpu MC68HC05", "68HC05", assembler.config().cpu_P());
}

static void test_inherent() {
    TEST("NOP", 0x9D);
    TEST("CLC", 0x98);
    TEST("SEC", 0x99);
    TEST("CLI", 0x9A);
    TEST("SEI", 0x9B);

    TEST("RTS", 0x81);
    TEST("RTI", 0x80);
    TEST("SWI", 0x83);
    TEST("TAX",  0x97);
    TEST("TXA",  0x9F);
    if (m146805() || m68hc05()) {
        // MC146805/MC68HC05
        TEST("WAIT", 0x8F);
    } else {
        ERUI("WAIT");
    }

    TEST("NEGA", 0x40);
    TEST("COMA", 0x43);
    TEST("LSRA", 0x44);
    TEST("RORA", 0x46);
    TEST("ASRA", 0x47);
    TEST("ASLA", 0x48);
    TEST("ROLA", 0x49);
    TEST("DECA", 0x4A);
    TEST("INCA", 0x4C);
    TEST("TSTA", 0x4D);
    TEST("CLRA", 0x4F);

    TEST("NEGX", 0x50);
    TEST("COMX", 0x53);
    TEST("LSRX", 0x54);
    TEST("RORX", 0x56);
    TEST("ASRX", 0x57);
    TEST("ASLX", 0x58);
    TEST("ROLX", 0x59);
    TEST("DECX", 0x5A);
    TEST("INCX", 0x5C);
    TEST("TSTX", 0x5D);
    TEST("CLRX", 0x5F);

    if (m68hc05()) {
        // MC68HC05
        TEST("MUL",  0x42);
    } else {
        ERUI("MUL");
    }
    if (m146805() || m68hc05()) {
        // MC146805/MC68HC05
        TEST("STOP", 0x8E);
    } else {
        ERUI("STOP");
    }
}

static void test_relative() {
    ATEST(0x1000, "BRA $1002", 0x20, 0x00);
    ATEST(0x1000, "BHI $1004", 0x22, 0x02);
    ATEST(0x1000, "BLS $0F82", 0x23, 0x80);
    ATEST(0x1000, "BHS $1081", 0x24, 0x7F);
    ATEST(0x1000, "BLO $1002", 0x25, 0x00);
    ATEST(0x1000, "BNE $1002", 0x26, 0x00);
    ATEST(0x1000, "BEQ $1002", 0x27, 0x00);
    ATEST(0x1000, "BPL $1002", 0x2A, 0x00);
    ATEST(0x1000, "BMI $1002", 0x2B, 0x00);
    AERRT(0x1000, "BMI $0F81", OPERAND_TOO_FAR, "$0F81", 0x2B, 0x7F);
    AERRT(0x1000, "BMI $1082", OPERAND_TOO_FAR, "$1082", 0x2B, 0x80);

    ATEST(0x1000, "BHCC $1002", 0x28, 0x00);
    ATEST(0x1000, "BHCS $1002", 0x29, 0x00);
    ATEST(0x1000, "BMC $1002", 0x2C, 0x00);
    ATEST(0x1000, "BMS $1002", 0x2D, 0x00);
    ATEST(0x1000, "BIL $1002", 0x2E, 0x00);
    ATEST(0x1000, "BIH $1002", 0x2F, 0x00);
    ATEST(0x1000, "BSR $1042", 0xAD, 0x40);

    AERRT(0x1FF0, "BRA $2000", OVERFLOW_RANGE, "$2000", 0x20, 0x0E);
    ATEST(0x1000, "BRN $1081", 0x21, 0x7F);

    symtab.intern(0x0F82, "sub0F82");
    symtab.intern(0x1081, "sub1081");

    ATEST(0x1000, "BSR sub1081", 0xAD, 0x7F);
    ATEST(0x1000, "BSR sub0F82", 0xAD, 0x80);
    ATEST(0x1000, "BRN sub0F82", 0x21, 0x80);
}

static void test_immediate() {
    TEST("SUB #$90",  0xA0, 0x90);
    TEST("CMP #$90",  0xA1, 0x90);
    TEST("SBC #$90",  0xA2, 0x90);
    TEST("AND #$90",  0xA4, 0x90);
    TEST("BIT #$90",  0xA5, 0x90);
    TEST("LDA #$90",  0xA6, 0x90);
    ERRT("STA #$90",  OPERAND_NOT_ALLOWED, "#$90");
    TEST("EOR #$90",  0xA8, 0x90);
    TEST("ADC #-1",   0xA9, 0xFF);
    TEST("ORA #255",  0xAA, 0xFF);
    TEST("ADD #-128", 0xAB, 0x80);
    ERRT("ADD #256",  OVERFLOW_RANGE, "#256",  0xAB, 0x00);
    ERRT("ADD #-129", OVERFLOW_RANGE, "#-129", 0xAB, 0x7F);

    TEST("CPX #$90", 0xA3, 0x90);
    TEST("LDX #$90", 0xAE, 0x90);
    ERRT("STX #$90", OPERAND_NOT_ALLOWED, "#$90");

    ERRT("BSR #$90", OPERAND_NOT_ALLOWED, "#$90");
    ERRT("JSR #$90", OPERAND_NOT_ALLOWED, "#$90");
    ERRT("JMP #$90", OPERAND_NOT_ALLOWED, "#$90");

    symtab.intern(0x90, ".dir_90");
    symtab.intern(0x90A0, "dir90A0");

    TEST("LDA #.dir_90",     0xA6, 0x90);
    ERRT("LDX #dir90A0",     OVERFLOW_RANGE, "#dir90A0", 0xAE, 0xA0);
    TEST("LDX #dir90A0&$FF", 0xAE, 0xA0);
}

static void test_direct() {
    TEST("NEG $90", 0x30, 0x90);
    TEST("COM $90", 0x33, 0x90);
    TEST("LSR $90", 0x34, 0x90);
    TEST("ROR $90", 0x36, 0x90);
    TEST("ASR $90", 0x37, 0x90);
    TEST("ASL $90", 0x38, 0x90);
    TEST("ROL $90", 0x39, 0x90);
    TEST("DEC $90", 0x3A, 0x90);
    TEST("INC $90", 0x3C, 0x90);
    TEST("TST $90", 0x3D, 0x90);
    TEST("CLR $90", 0x3F, 0x90);

    TEST("SUB $90", 0xB0, 0x90);
    TEST("CMP $90", 0xB1, 0x90);
    TEST("SBC $90", 0xB2, 0x90);
    TEST("AND $90", 0xB4, 0x90);
    TEST("BIT $90", 0xB5, 0x90);
    TEST("LDA $90", 0xB6, 0x90);
    TEST("STA $90", 0xB7, 0x90);
    TEST("EOR $90", 0xB8, 0x90);
    TEST("ADC $90", 0xB9, 0x90);
    TEST("ORA $90", 0xBA, 0x90);
    TEST("ADD $90", 0xBB, 0x90);

    TEST("CPX $90", 0xB3, 0x90);
    TEST("LDX $90", 0xBE, 0x90);
    TEST("STX $90", 0xBF, 0x90);

    TEST("JSR $90", 0xBD, 0x90);

    symtab.intern(0x10, "dir_10");
    symtab.intern(0x22, "dir$22");
    symtab.intern(0x90, "dir.90");

    TEST("LDA  dir.90", 0xB6, 0x90);
    TEST("STA  dir_10", 0xB7, 0x10);
    TEST("CMP <dir_10", 0xB1, 0x10);
    TEST("CPX <dir.90", 0xB3, 0x90);
    TEST("LDX  dir$22", 0xBE, 0x22);
    TEST("STX <dir$22", 0xBF, 0x22);
}

static void test_extended() {
    ERRT("NEG >$0000", OPERAND_NOT_ALLOWED, ">$0000");
    ERRT("COM >$0009", OPERAND_NOT_ALLOWED, ">$0009");
    ERRT("LSR >$0034", OPERAND_NOT_ALLOWED, ">$0034");
    ERRT("ROR  $1234", OPERAND_NOT_ALLOWED, "$1234");
    ERRT("ASR  $1234", OPERAND_NOT_ALLOWED, "$1234");
    ERRT("ASL  $1234", OPERAND_NOT_ALLOWED, "$1234");
    ERRT("ROL  $1234", OPERAND_NOT_ALLOWED, "$1234");
    ERRT("DEC  $1234", OPERAND_NOT_ALLOWED, "$1234");
    ERRT("INC  $1234", OPERAND_NOT_ALLOWED, "$1234");
    ERRT("TST  $1234", OPERAND_NOT_ALLOWED, "$1234");
    ERRT("CLR  $1234", OPERAND_NOT_ALLOWED, "$1234");

    TEST("SUB >$0090", 0xC0, 0x00, 0x90);
    TEST("CMP >$0090", 0xC1, 0x00, 0x90);
    TEST("SBC >$0090", 0xC2, 0x00, 0x90);
    TEST("AND >$0090", 0xC4, 0x00, 0x90);
    TEST("BIT >$0090", 0xC5, 0x00, 0x90);
    TEST("LDA >$0090", 0xC6, 0x00, 0x90);
    TEST("STA >$0090", 0xC7, 0x00, 0x90);
    TEST("EOR >$0090", 0xC8, 0x00, 0x90);
    TEST("ADC >$0090", 0xC9, 0x00, 0x90);
    TEST("ORA >$0090", 0xCA, 0x00, 0x90);
    TEST("ADD  $1FFF", 0xCB, 0x1F, 0xFF);
    ERRT("SUB  $2000", OVERFLOW_RANGE, "$2000", 0xC0, 0x20, 0x00);

    TEST("CPX $1ABC", 0xC3, 0x1A, 0xBC);
    TEST("LDX $1ABC", 0xCE, 0x1A, 0xBC);
    TEST("STX $1ABC", 0xCF, 0x1A, 0xBC);

    TEST("JMP >$0034", 0xCC, 0x00, 0x34);
    TEST("JSR  $1234", 0xCD, 0x12, 0x34);

    ERRT("CPX $2000", OVERFLOW_RANGE, "$2000", 0xC3, 0x20, 0x00);
    ERRT("JMP $4000", OVERFLOW_RANGE, "$4000", 0xCC, 0x40, 0x00);
    ERRT("JSR $8000", OVERFLOW_RANGE, "$8000", 0xCD, 0x80, 0x00);

    symtab.intern(0x0090, "ext0090");
    symtab.intern(0x1ABC, "ext1ABC");
    symtab.intern(0x9ABC, "ext9ABC");

    ERRT("NEG >ext0090", OPERAND_NOT_ALLOWED, ">ext0090");
    TEST("LDA  ext1ABC", 0xC6, 0x1A, 0xBC);
    TEST("STA >ext0090", 0xC7, 0x00, 0x90);

    TEST("CPX  ext1ABC", 0xC3, 0x1A, 0xBC);
    TEST("LDX  ext1ABC", 0xCE, 0x1A, 0xBC);
    TEST("STX >ext0090", 0xCF, 0x00, 0x90);
    TEST("JMP  ext1ABC", 0xCC, 0x1A, 0xBC);
    TEST("JSR  ext0090", 0xBD, 0x90);

    TEST(R"(option "pc-bits", "11")"); // MC68HC05J for instance
    TEST(         "LDA $07FF",                          0xC6, 0x07, 0xFF);
    ERRT(         "LDA $0800", OVERFLOW_RANGE, "$0800", 0xC6, 0x08, 0x00);
    ATEST(0x07F0, "BSR $07FF",                          0xAD, 0x0D);
    AERRT(0x07F0, "BSR $0800", OVERFLOW_RANGE, "$0800", 0xAD, 0x0E);

    TEST(R"(option "pc-bits", 0)");  // Most of MC68HC05 has 13bits PC.
    TEST(         "LDA $1FFF",                          0xC6, 0x1F, 0xFF);
    ERRT(         "LDA $2000", OVERFLOW_RANGE, "$2000", 0xC6, 0x20, 0x00);
    ATEST(0x1FF0, "BSR $1FFF",                          0xAD, 0x0D);
    AERRT(0x1FF0, "BSR $2000", OVERFLOW_RANGE, "$2000", 0xAD, 0x0E);

    assembler.setOption("pc-bits", "14"); // MC68HC05X for instance
    TEST(         "LDA $3FFF",                          0xC6, 0x3F, 0xFF);
    ERRT(         "LDA $4000", OVERFLOW_RANGE, "$4000", 0xC6, 0x40, 0x00);
    ATEST(0x3FF0, "BSR $3FFF",                          0xAD, 0x0D);
    AERRT(0x3FF0, "BSR $4000", OVERFLOW_RANGE, "$4000", 0xAD, 0x0E);
}

static void test_indexed() {
    TEST("neg   0,x", 0x70);
    TEST("COM  <0,X", 0x63, 0x00);
    TEST("LSR   1,X", 0x64, 0x01);
    TEST("ROR   2,X", 0x66, 0x02);
    TEST("ASR   3,X", 0x67, 0x03);
    TEST("ASL   4,X", 0x68, 0x04);
    TEST("LSL   5,X", 0x68, 0x05);
    TEST("ROL   6,X", 0x69, 0x06);
    TEST("DEC   7,X", 0x6A, 0x07);
    TEST("INC 127,X", 0x6C, 0x7F);
    TEST("TST 128,X", 0x6D, 0x80);
    TEST("CLR 255,X", 0x6F, 0xFF);

    ERRT("NEG >0,X",    OPERAND_NOT_ALLOWED, ">0,X");
    ERRT("COM >1,X",    OPERAND_NOT_ALLOWED, ">1,X");
    ERRT("LSR >255,X",  OPERAND_NOT_ALLOWED, ">255,X");
    ERRT("ROR 256,X",   OPERAND_NOT_ALLOWED, "256,X");
    ERRT("ASR 512,X",   OPERAND_NOT_ALLOWED, "512,X");
    ERRT("ASL 1024,X",  OPERAND_NOT_ALLOWED, "1024,X");
    ERRT("LSL 2048,X",  OPERAND_NOT_ALLOWED, "2048,X");
    ERRT("ROL 4096,X",  OPERAND_NOT_ALLOWED, "4096,X");
    ERRT("DEC 8192,X",  OPERAND_NOT_ALLOWED, "8192,X");
    ERRT("INC 16384,X", OPERAND_NOT_ALLOWED, "16384,X");
    ERRT("TST 32768,X", OPERAND_NOT_ALLOWED, "32768,X");
    ERRT("CLR 65535,X", OPERAND_NOT_ALLOWED, "65535,X");

    TEST("NEG ,X", 0x70);
    TEST("COM ,X", 0x73);
    TEST("LSR ,X", 0x74);
    TEST("ROR ,X", 0x76);
    TEST("ASR ,X", 0x77);
    TEST("LSL ,X", 0x78);
    TEST("ASL ,X", 0x78);
    TEST("ROL ,X", 0x79);
    TEST("DEC ,X", 0x7A);
    TEST("INC ,X", 0x7C);
    TEST("TST ,X", 0x7D);
    TEST("CLR ,X", 0x7F);

    TEST("SUB   0,X", 0xF0);
    TEST("CMP  <0,X", 0xE1, 0x00);
    TEST("SBC   1,X", 0xE2, 0x01);
    TEST("AND   2,X", 0xE4, 0x02);
    TEST("BIT   3,X", 0xE5, 0x03);
    TEST("LDA   4,X", 0xE6, 0x04);
    TEST("STA   5,X", 0xE7, 0x05);
    TEST("EOR   6,X", 0xE8, 0x06);
    TEST("ADC 127,X", 0xE9, 0x7F);
    TEST("ORA 128,X", 0xEA, 0x80);
    TEST("ADD 255,X", 0xEB, 0xFF);

    TEST("SUB ,X", 0xF0);
    TEST("CMP ,X", 0xF1);
    TEST("SBC ,X", 0xF2);
    TEST("AND ,X", 0xF4);
    TEST("BIT ,X", 0xF5);
    TEST("LDA ,X", 0xF6);
    TEST("STA ,X", 0xF7);
    TEST("EOR ,X", 0xF8);
    TEST("ADC ,X", 0xF9);
    TEST("ORA ,X", 0xFA);
    TEST("ADD ,X", 0xFB);

    TEST("SUB >0,X",    0xD0, 0x00, 0x00);
    TEST("CMP >1,X",    0xD1, 0x00, 0x01);
    TEST("SBC 256,X",   0xD2, 0x01, 0x00);
    TEST("AND 512,X",   0xD4, 0x02, 0x00);
    TEST("BIT 1024,X",  0xD5, 0x04, 0x00);
    TEST("LDA 2048,X",  0xD6, 0x08, 0x00);
    TEST("STA 4096,X",  0xD7, 0x10, 0x00);
    TEST("EOR 8192,X",  0xD8, 0x20, 0x00);
    TEST("ADC 16384,X", 0xD9, 0x40, 0x00);
    TEST("ORA 32768,X", 0xDA, 0x80, 0x00);
    TEST("ADD 65535,X", 0xDB, 0xFF, 0xFF);

    TEST("CPX ,X",   0xF3);
    TEST("LDX ,X",   0xFE);
    TEST("STX ,X",   0xFF);
    TEST("CPX 0,X",  0xF3);
    TEST("LDX <0,X", 0xEE, 0x00);
    TEST("STX 2,X", 0xEF, 0x02);

    TEST("JMP ,X",    0xFC);
    TEST("JSR ,X",    0xFD);
    TEST("JMP 0,X",   0xFC);
    TEST("JMP <0,X",  0xEC, 0x00);
    TEST("JSR 255,X", 0xED, 0xFF);

    symtab.intern(0,   ".offset0");
    symtab.intern(128, "offset128");
    symtab.intern(255, "offset255");

    TEST("NEG <.offset0,X", 0x60, 0x00);
    TEST("COM offset255,X", 0x63, 0xFF);
    TEST("CMP <.offset0,X", 0xE1, 0x00);
    TEST("ADD offset255,X", 0xEB, 0xFF);
    TEST("JMP <.offset0,X", 0xEC, 0x00);
    TEST("JSR offset255,X", 0xED, 0xFF);
}

static void test_bit_ops() {
    TEST("BSET 0, $23", 0x10, 0x23);
    TEST("BSET 7, $89", 0x1E, 0x89);
    TEST("BCLR 0, $23", 0x11, 0x23);
    TEST("BCLR 7, $89", 0x1F, 0x89);
    ERRT("BSET -1, $23", OPERAND_NOT_ALLOWED, "-1, $23");
    ERRT("BCLR 8, $89",  OPERAND_NOT_ALLOWED, "8, $89");

    ATEST(0x1000, "BRSET 0,$23,$1082", 0x00, 0x23, 0x7F);
    ATEST(0x1000, "BRSET 7,$89,$0F83", 0x0E, 0x89, 0x80);
    ATEST(0x1000, "BRCLR 0,$23,$1082", 0x01, 0x23, 0x7F);
    ATEST(0x1000, "BRCLR 7,$89,$0F83", 0x0F, 0x89, 0x80);
    AERRT(0x1000, "BRSET 7,$89,$0F82", OPERAND_TOO_FAR, "$0F82", 0x0E, 0x89, 0x7F);
    AERRT(0x1000, "BRCLR 0,$23,$1083", OPERAND_TOO_FAR, "$1083", 0x01, 0x23, 0x80);

    ERRT("BSET $90,#$88", OPERAND_NOT_ALLOWED, "$90,#$88");
    ERRT("BCLR $90,#$88", OPERAND_NOT_ALLOWED, "$90,#$88");
    ERRT("BRSET $90,#$88,$1083", OPERAND_NOT_ALLOWED, "$90,#$88,$1083");
    ERRT("BRCLR $90,#$88,$1F84", OPERAND_NOT_ALLOWED, "$90,#$88,$1F84");

    TEST("BCLR   7,$90", 0x1F, 0x90);
    TEST("BSET   6,$90", 0x1C, 0x90);
}

static void test_comment() {
    symtab.intern(255,    "sym255");
    symtab.intern(0x1234, "sym1234");

    COMM("NOP       ; comment", "; comment", 0x9D);
    COMM("SUB   #$90; comment", "; comment", 0xA0, 0x90);
    COMM("NOP         comment", "comment", 0x9D);
    COMM("SUB   #$90  comment", "comment", 0xA0, 0x90);
    COMM("NEG < $0010 comment", "comment", 0x30, 0x10);
    COMM("SUB   >$90  comment", "comment", 0xC0, 0x00, 0x90);
    COMM("SUB sym255  comment", "comment", 0xB0, 0xFF);
    COMM("SUB >sym255 comment", "comment", 0xC0, 0x00, 0xFF);
    COMM("SUB sym1234 comment", "comment", 0xC0, 0x12, 0x34);
    COMM("JMP sym255  comment", "comment", 0xBC, 0xFF);
    COMM("JSR sym1234 comment", "comment", 0xCD, 0x12, 0x34);
    COMM("LDA 0 , X   comment", "comment", 0xF6);
    COMM("LDA < 0 , X comment", "comment", 0xE6, 0x00);
    COMM("LDA > 0 , X comment", "comment", 0xD6, 0x00, 0x00);
    COMM("FCB -128, 255 comment", "comment", 0x80, 0xFF);
    COMM("FDB -128, 255 comment", "comment", 0xFF, 0x80, 0x00, 0xFF);
    COMM("FCC ;TEXT;    comment", "comment", 0x54, 0x45, 0x58, 0x54);
}

static void test_undef() {
    ERUS("LDA  #UNDEF",  "UNDEF", 0xA6, 0x00);
    ERUS("LDX  #UNDEF",  "UNDEF", 0xAE, 0x00);
    ERUS("NEG  UNDEF",   "UNDEF", 0x30, 0x00);
    ERUS("SUB  UNDEF",   "UNDEF", 0xB0, 0x00);
    ERUS("SUB <UNDEF",   "UNDEF", 0xB0, 0x00);
    ERUS("SUB >UNDEF",   "UNDEF", 0xC0, 0x00, 0x00);
    ERUS("JMP  UNDEF",   "UNDEF", 0xBC, 0x00);
    ERUS("LDA  UNDEF,X", "UNDEF,X", 0xF6);
    ERUS("LDA <UNDEF,X", "UNDEF,X", 0xE6, 0x00);
    ERUS("LDA >UNDEF,X", "UNDEF,X", 0xD6, 0x00, 0x00);
    ERUS("JSR UNDEF",   "UNDEF",   0xBD, 0x00);
    ERUS("BRA UNDEF+2", "UNDEF+2", 0x20, 0x00);
    ERUS("BRA 2+UNDEF", "UNDEF",   0x20, 0x00);
    ERUS("BSR UNDEF",   "UNDEF",   0xAD, 0x00);
}

static void test_data_constant() {
    TEST("FCB -128, 255", 0x80, 0xFF);
    TEST(R"(FCB 'A', '"')", 0x41, 0x22);
    TEST("FCB '9'-'0'",   0x09);
    TEST("FCB '''",       0x27);
    TEST("FCB ''",        0x27);
    ERRT("FCB '",         ILLEGAL_CONSTANT, "'");
    TEST("FCB 'A,'','B",  0x41, 0x27, 0x42);
    TEST("FDB -128, 255", 0xFF, 0x80, 0x00, 0xFF);
    TEST(R"(FDB 'A, '")", 0x00, 0x41, 0x00, 0x22);
    TEST("FCC 'ABC'",     0x41, 0x42, 0x43);
    TEST("FCC /ABC/",     0x41, 0x42, 0x43);
    ERRT("FCC 'ABC",      MISSING_CLOSING_DELIMITER, "", 0x41, 0x42, 0x43);
    ERRT("FCC /ABC",      MISSING_CLOSING_DELIMITER, "", 0x41, 0x42, 0x43);
    ERUS("FCB 1, UNDEF", "UNDEF", 0x01, 0x00);
    ERUS("FDB 1, UNDEF", "UNDEF", 0x00, 0x01, 0x00, 0x00);

    ERRT("FCC /"
         "1234567890" "1234567890" "1234567890" "1234567890" "1234567890" "1234567890"
         "1234567890/",
         NO_MEMORY,
         "567890/",
         0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30,
         0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30,
         0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30,
         0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30,
         0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30,
         0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30,
         0x31, 0x32, 0x33, 0x34);

    ERRT("FDB "
         "$1234, $5678, $9ABC, $DEF0, $1234, $5678, $9ABC, $DEF0, "
         "$1234, $5678, $9ABC, $DEF0, $1234, $5678, $9ABC, $DEF0, "
         "$1234, $5678, $9ABC, $DEF0, $1234, $5678, $9ABC, $DEF0, "
         "$1234, $5678, $9ABC, $DEF0, $1234, $5678, $9ABC, $DE, "
         "$AAAA, $BBBB",
         NO_MEMORY,
         "$AAAA, $BBBB",
         0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
         0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
         0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
         0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0x00, 0xDE);
}

// clang-format on

void run_tests(const char *cpu) {
    assembler.setCpu(cpu);
    RUN_TEST(test_inherent);
    RUN_TEST(test_immediate);
    RUN_TEST(test_direct);
    RUN_TEST(test_extended);
    RUN_TEST(test_indexed);
    RUN_TEST(test_relative);
    RUN_TEST(test_bit_ops);
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

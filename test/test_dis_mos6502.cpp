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

#include "dis_mos6502.h"
#include "test_dis_helper.h"

using namespace libasm;
using namespace libasm::mos6502;
using namespace libasm::test;

DisMos6502 dis6502;
Disassembler &disassembler(dis6502);

static bool m6502() {
    return strcmp_P("6502", disassembler.cpu_P()) == 0;
}

static bool g65sc02() {
    return strcmp_P("65SC02", disassembler.cpu_P()) == 0;
}

static bool r65c02() {
    return strcmp_P("65C02", disassembler.cpu_P()) == 0;
}

static bool w65c02s() {
    return strcmp_P("W65C02S", disassembler.cpu_P()) == 0;
}

static bool w65c816() {
    return strcmp_P("65816", disassembler.cpu_P()) == 0;
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
    EQUALS("cpu 6502", true,   disassembler.setCpu("6502"));
    EQUALS_P("cpu 6502", "6502", disassembler.cpu_P());

    EQUALS("cpu 65SC02", true,     disassembler.setCpu("65SC02"));
    EQUALS_P("cpu 65SC02", "65SC02", disassembler.cpu_P());

    EQUALS("cpu 65C02", true,    disassembler.setCpu("65C02"));
    EQUALS_P("cpu 65C02", "65C02", disassembler.cpu_P());

    EQUALS("cpu MOS6502", true,   disassembler.setCpu("MOS6502"));
    EQUALS_P("cpu MOS6502", "6502", disassembler.cpu_P());

    EQUALS("cpu G65SC02", true,     disassembler.setCpu("G65SC02"));
    EQUALS_P("cpu G65SC02", "65SC02", disassembler.cpu_P());

    EQUALS("cpu R65C02", true,    disassembler.setCpu("R65C02"));
    EQUALS_P("cpu R65C02", "65C02", disassembler.cpu_P());

    EQUALS("cpu W65C02S", true,      disassembler.setCpu("W65C02S"));
    EQUALS_P("cpu W65C02S", "W65C02S", disassembler.cpu_P());

    EQUALS("cpu 65816", true,    disassembler.setCpu("65816"));
    EQUALS_P("cpu 65816", "65816", disassembler.cpu_P());

    EQUALS("cpu W65816", true,    disassembler.setCpu("W65816"));
    EQUALS_P("cpu W65816", "65816", disassembler.cpu_P());

    EQUALS("cpu W65C816", true,    disassembler.setCpu("W65C816"));
    EQUALS_P("cpu W65C816", "65816", disassembler.cpu_P());

    EQUALS("cpu W65C816S", true,    disassembler.setCpu("W65C816S"));
    EQUALS_P("cpu W65C816S", "65816", disassembler.cpu_P());
}

static void test_impl() {
    // MOS6502
    TEST(BRK, "", 0x00);
    TEST(RTI, "", 0x40);
    TEST(RTS, "", 0x60);
    TEST(NOP, "", 0xEA);

    TEST(PHP, "", 0x08);
    TEST(PLP, "", 0x28);
    TEST(PHA, "", 0x48);
    TEST(PLA, "", 0x68);

    TEST(DEY, "", 0x88);
    TEST(INY, "", 0xC8);
    TEST(DEX, "", 0xCA);
    TEST(INX, "", 0xE8);

    TEST(TYA, "", 0x98);
    TEST(TAY, "", 0xA8);
    TEST(TXA, "", 0x8A);
    TEST(TAX, "", 0xAA);
    TEST(TXS, "", 0x9A);
    TEST(TSX, "", 0xBA);

    TEST(CLC, "", 0x18);
    TEST(SEC, "", 0x38);
    TEST(CLI, "", 0x58);
    TEST(SEI, "", 0x78);
    TEST(CLV, "", 0xB8);
    TEST(CLD, "", 0xD8);
    TEST(SED, "", 0xF8);

    if (m6502()) {
        ERRI(0x5A);
        ERRI(0x7A);
        ERRI(0xDA);
        ERRI(0xFA);
    } else {
        // G65SC02
        TEST(PHY, "", 0x5A);
        TEST(PLY, "", 0x7A);
        TEST(PHX, "", 0xDA);
        TEST(PLX, "", 0xFA);
    }
    if (w65c02s() || w65c816()) {
        // W65C02S, W65C816
        TEST(WAI, "", 0xCB);
        TEST(STP, "", 0xDB);
    } else {
        ERRI(0xCB);
        ERRI(0xDB);
    }
    if (w65c816()) {
        // W65C816
        TEST(RTL, "", 0x6B);
        TEST(PHB, "", 0x8B);
        TEST(PHD, "", 0x0B);
        TEST(PHK, "", 0x4B);
        TEST(PLB, "", 0xAB);
        TEST(PLD, "", 0x2B);
        TEST(TCS, "", 0x1B);
        TEST(TSC, "", 0x3B);
        TEST(TCD, "", 0x5B);
        TEST(TDC, "", 0x7B);
        TEST(TXY, "", 0x9B);
        TEST(TYX, "", 0xBB);
        TEST(XBA, "", 0xEB);
        TEST(XCE, "", 0xFB);
    }
}

static void test_accm() {
    // MOS6502
    TEST(ASL, "A", 0x0A);
    TEST(ROL, "A", 0x2A);
    TEST(LSR, "A", 0x4A);
    TEST(ROR, "A", 0x6A);

    if (m6502()) {
        ERRI(0x1A);
        ERRI(0x3A);
    } else {
        // G65SC02
        TEST(INC, "A", 0x1A);
        TEST(DEC, "A", 0x3A);
    }
}

static void test_imm() {
    // MOS6502
    TEST(LDY, "#0",   0xA0, 0x00);
    TEST(LDX, "#9",   0xA2, 0x09);
    TEST(CPY, "#10",  0xC0, 0x0A);
    TEST(CPX, "#$FF", 0xE0, 0xFF);

    TEST(ORA, "#$90", 0x09, 0x90);
    TEST(AND, "#$90", 0x29, 0x90);
    TEST(EOR, "#$90", 0x49, 0x90);
    TEST(ADC, "#$90", 0x69, 0x90);
    TEST(LDA, "#$90", 0xA9, 0x90);
    TEST(CMP, "#$90", 0xC9, 0x90);
    TEST(SBC, "#$90", 0xE9, 0x90);

    if (m6502()) {
        ERRI(0x89);
    } else {
        // G65SC02
        TEST(BIT, "#$90", 0x89, 0x90);
    }
    if (w65c816()) {
        // W65C816
        TEST(COP, "#16",  0x02, 0x10);
        ERUI(WDM, "#$10", 0x42, 0x10);
        TEST(REP, "#32",  0xC2, 0x20);
        TEST(SEP, "#16",  0xE2, 0x10);
    }

    symtab.intern(0x0010, "zero10");
    symtab.intern(0x00FF, "zeroFF");
    symtab.intern(0x0090, "zero90");

    // MOS6502
    TEST(LDX, "#zero10", 0xA2, 0x10);
    TEST(CPY, "#zeroFF", 0xC0, 0xFF);
    TEST(SBC, "#zero90", 0xE9, 0x90);

    if (g65sc02()) {
        // G65SC02
        TEST(BIT, "#zero90", 0x89, 0x90);
    }
}

static void test_long_imm() {
    dis6502.setOption("longa", "disable");
    dis6502.setOption("longi", "enable");

    if (w65c816()) {
        TEST(LDY, "#0",     0xA0, 0x00, 0x00);
        TEST(LDX, "#$1000", 0xA2, 0x00, 0x10);
        TEST(CPY, "#$00FF", 0xC0, 0xFF, 0x00);
        TEST(CPX, "#$FFFF", 0xE0, 0xFF, 0xFF);
        TEST(ORA, "#9",     0x09, 0x09);
    } else {
        TEST(LDY, "#0",   0xA0, 0x00);
        TEST(LDX, "#0",   0xA2, 0x00);
        TEST(CPY, "#$FF", 0xC0, 0xFF);
        TEST(CPX, "#$FF", 0xE0, 0xFF);
        TEST(ORA, "#9",   0x09, 0x09);
    }

    dis6502.setOption("longa", "enable");
    dis6502.setOption("longi", "disable");

    if (w65c816()) {
        TEST(ORA, "#9",     0x09, 0x09, 0x00);
        TEST(AND, "#$FFF0", 0x29, 0xF0, 0xFF);
        TEST(EOR, "#$007F", 0x49, 0x7F, 0x00);
        TEST(ADC, "#$8000", 0x69, 0x00, 0x80);
        TEST(LDA, "#$FFFF", 0xA9, 0xFF, 0xFF);
        TEST(CMP, "#$8000", 0xC9, 0x00, 0x80);
        TEST(SBC, "#$FFFF", 0xE9, 0xFF, 0xFF);
        TEST(CPX, "#$FF",   0xE0, 0xFF);
        // G65SC02
        TEST(BIT, "#$1234", 0x89, 0x34, 0x12);
    } else {
        TEST(ORA, "#9",   0x09, 0x09);
        TEST(AND, "#$F0", 0x29, 0xF0);
        TEST(EOR, "#$7F", 0x49, 0x7F);
        TEST(ADC, "#0",   0x69, 0x00);
        TEST(LDA, "#$FF", 0xA9, 0xFF);
        TEST(CMP, "#0",   0xC9, 0x00);
        TEST(SBC, "#$FF", 0xE9, 0xFF);
        TEST(CPX, "#$FF", 0xE0, 0xFF);
        if (!m6502()) {
            // G65SC02
            TEST(BIT, "#$34", 0x89, 0x34);
        }
    }

    symtab.intern(0x0010, "zero10");
    symtab.intern(0x01FF, "zero1FF");
    symtab.intern(-1,     "minus1");

    dis6502.setOption("longa", "enable");
    dis6502.setOption("longi", "enable");

    if (w65c816()) {
        TEST(LDX, "#zero10",  0xA2, 0x10, 0x00);
        TEST(CPY, "#zero1FF", 0xC0, 0xFF, 0x01);
        TEST(SBC, "#minus1",  0xE9, 0xFF, 0xFF);

        // G65SC02
        TEST(BIT, "#zero10",  0x89, 0x10, 0x00);

        // always 8bit immediate
        TEST(SEP, "#zero10",  0xE2, 0x10);
    }
}

static void test_zpg() {
    // MOS6502
    TEST(BIT, "$10", 0x24, 0x10);
    TEST(ORA, "$10", 0x05, 0x10);
    TEST(AND, "$10", 0x25, 0x10);
    TEST(EOR, "$10", 0x45, 0x10);
    TEST(ADC, "$10", 0x65, 0x10);
    TEST(STA, "$10", 0x85, 0x10);
    TEST(LDA, "$10", 0xA5, 0x10);
    TEST(CMP, "$10", 0xC5, 0x10);
    TEST(SBC, "$10", 0xE5, 0x10);

    TEST(STY, "$10", 0x84, 0x10);
    TEST(LDY, "$10", 0xA4, 0x10);
    TEST(CPY, "$10", 0xC4, 0x10);
    TEST(STX, "$10", 0x86, 0x10);
    TEST(LDX, "$10", 0xA6, 0x10);
    TEST(CPX, "$10", 0xE4, 0x10);

    TEST(DEC, "$10", 0xC6, 0x10);
    TEST(INC, "$10", 0xE6, 0x10);

    TEST(ASL, "$10", 0x06, 0x10);
    TEST(ROL, "$10", 0x26, 0x10);
    TEST(LSR, "$10", 0x46, 0x10);
    TEST(ROR, "$10", 0x66, 0x10);

    if (m6502()) {
        ERRI(0x04);
        ERRI(0x14);
        ERRI(0x64);
    } else {
        // G65SC02
        TEST(TSB, "$10", 0x04, 0x10);
        TEST(TRB, "$10", 0x14, 0x10);
        TEST(STZ, "$10", 0x64, 0x10);
    }

    symtab.intern(0x0010, "zero10");
    symtab.intern(0x00FF, "zeroFF");
    symtab.intern(0x0090, "zero90");

    // MOS6502
    TEST(SBC, "<zero10", 0xE5, 0x10);
    TEST(DEC, "<zeroFF", 0xC6, 0xFF);
    TEST(ROR, "<zero90", 0x66, 0x90);

    if (g65sc02()) {
        // G65SC02
        TEST(STZ, "<zero10", 0x64, 0x10);
    }
}

static void test_zpg_indexed() {
    // MOS6502
    TEST(ORA, "$00,X", 0x15, 0x00);
    TEST(AND, "$09,X", 0x35, 0x09);
    TEST(EOR, "$0A,X", 0x55, 0x0A);
    TEST(ADC, "$10,X", 0x75, 0x10);
    TEST(STA, "$10,X", 0x95, 0x10);
    TEST(LDA, "$10,X", 0xB5, 0x10);
    TEST(CMP, "$10,X", 0xD5, 0x10);
    TEST(SBC, "$10,X", 0xF5, 0x10);

    TEST(STY, "$10,X", 0x94, 0x10);
    TEST(LDY, "$10,X", 0xB4, 0x10);
    TEST(STX, "$10,Y", 0x96, 0x10);
    TEST(LDX, "$10,Y", 0xB6, 0x10);

    TEST(DEC, "$10,X", 0xD6, 0x10);
    TEST(INC, "$10,X", 0xF6, 0x10);

    TEST(ASL, "$10,X", 0x16, 0x10);
    TEST(ROL, "$10,X", 0x36, 0x10);
    TEST(LSR, "$10,X", 0x56, 0x10);
    TEST(ROR, "$10,X", 0x76, 0x10);

    if (m6502()) {
        ERRI(0x34);
        ERRI(0x74);
    } else {
        // G65SC02
        TEST(BIT, "$10,X", 0x34, 0x10);
        TEST(STZ, "$10,X", 0x74, 0x10);
    }

    symtab.intern(0x0010, "zero10");
    symtab.intern(0x00FF, "zeroFF");
    symtab.intern(0x0090, "zero90");

    // MOS6502
    TEST(SBC, "<zero10,X", 0xF5, 0x10);
    TEST(STY, "<zero90,X", 0x94, 0x90);
    TEST(LDY, "<zeroFF,X", 0xB4, 0xFF);
    TEST(STX, "<zero90,Y", 0x96, 0x90);
    TEST(LDX, "<zeroFF,Y", 0xB6, 0xFF);
    TEST(ROR, "<zero90,X", 0x76, 0x90);

    if (g65sc02()) {
        // G65SC02
        TEST(BIT, "<zero10,X", 0x34, 0x10);
    }
}

static void test_zpg_long() {
    // W65C816
    TEST(ORA, "[$10]", 0x07, 0x10);
    TEST(AND, "[$10]", 0x27, 0x10);
    TEST(EOR, "[$10]", 0x47, 0x10);
    TEST(ADC, "[$10]", 0x67, 0x10);
    TEST(STA, "[$10]", 0x87, 0x10);
    TEST(LDA, "[$10]", 0xA7, 0x10);
    TEST(CMP, "[$10]", 0xC7, 0x10);
    TEST(SBC, "[$10]", 0xE7, 0x10);

    TEST(ORA, "[$10],Y", 0x17, 0x10);
    TEST(AND, "[$10],Y", 0x37, 0x10);
    TEST(EOR, "[$10],Y", 0x57, 0x10);
    TEST(ADC, "[$10],Y", 0x77, 0x10);
    TEST(STA, "[$10],Y", 0x97, 0x10);
    TEST(LDA, "[$10],Y", 0xB7, 0x10);
    TEST(CMP, "[$10],Y", 0xD7, 0x10);
    TEST(SBC, "[$10],Y", 0xF7, 0x10);

    dis6502.setOption("indirect-long", "disable");
    TEST(ORAL, "($10)", 0x07, 0x10);
    TEST(ANDL, "($10)", 0x27, 0x10);
    TEST(EORL, "($10)", 0x47, 0x10);
    TEST(ADCL, "($10)", 0x67, 0x10);
    TEST(STAL, "($10)", 0x87, 0x10);
    TEST(LDAL, "($10)", 0xA7, 0x10);
    TEST(CMPL, "($10)", 0xC7, 0x10);
    TEST(SBCL, "($10)", 0xE7, 0x10);

    TEST(ORAL, "($10),Y", 0x17, 0x10);
    TEST(ANDL, "($10),Y", 0x37, 0x10);
    TEST(EORL, "($10),Y", 0x57, 0x10);
    TEST(ADCL, "($10),Y", 0x77, 0x10);
    TEST(STAL, "($10),Y", 0x97, 0x10);
    TEST(LDAL, "($10),Y", 0xB7, 0x10);
    TEST(CMPL, "($10),Y", 0xD7, 0x10);
    TEST(SBCL, "($10),Y", 0xF7, 0x10);

    symtab.intern(0x10, "dir10");

    dis6502.setOption("indirect-long", "enable");
    TEST(ORA, "[<dir10]",   0x07, 0x10);
    TEST(ORA, "[<dir10],Y", 0x17, 0x10);

    dis6502.setOption("indirect-long", "disable");
    TEST(ORAL, "(<dir10)",  0x07, 0x10);
    TEST(ORAL, "(<dir10),Y", 0x17, 0x10);
}

static void test_sp_rel() {
    // W65C816
    TEST(ORA, "$10,S", 0x03, 0x10);
    TEST(AND, "$10,S", 0x23, 0x10);
    TEST(EOR, "$10,S", 0x43, 0x10);
    TEST(ADC, "$10,S", 0x63, 0x10);
    TEST(STA, "$10,S", 0x83, 0x10);
    TEST(LDA, "$10,S", 0xA3, 0x10);
    TEST(CMP, "$10,S", 0xC3, 0x10);
    TEST(SBC, "$10,S", 0xE3, 0x10);

    TEST(ORA, "($10,S),Y", 0x13, 0x10);
    TEST(AND, "($10,S),Y", 0x33, 0x10);
    TEST(EOR, "($10,S),Y", 0x53, 0x10);
    TEST(ADC, "($10,S),Y", 0x73, 0x10);
    TEST(STA, "($10,S),Y", 0x93, 0x10);
    TEST(LDA, "($10,S),Y", 0xB3, 0x10);
    TEST(CMP, "($10,S),Y", 0xD3, 0x10);
    TEST(SBC, "($10,S),Y", 0xF3, 0x10);
}

static void test_abs() {
    // MOS6502
    TEST(BIT,  "$1234", 0x2C, 0x34, 0x12);
    TEST(ORA,  "$1234", 0x0D, 0x34, 0x12);
    TEST(AND,  "$1234", 0x2D, 0x34, 0x12);
    TEST(EOR,  "$1234", 0x4D, 0x34, 0x12);
    TEST(ADC,  "$1234", 0x6D, 0x34, 0x12);
    TEST(STA,  "$1234", 0x8D, 0x34, 0x12);
    TEST(LDA,  "$1234", 0xAD, 0x34, 0x12);
    TEST(CMP,  "$1234", 0xCD, 0x34, 0x12);
    TEST(SBC, ">$0034", 0xED, 0x34, 0x00);

    TEST(STY,  "$1234", 0x8C, 0x34, 0x12);
    TEST(LDY,  "$1234", 0xAC, 0x34, 0x12);
    TEST(CPY,  "$1234", 0xCC, 0x34, 0x12);
    TEST(STX,  "$1234", 0x8E, 0x34, 0x12);
    TEST(LDX,  "$1234", 0xAE, 0x34, 0x12);
    TEST(CPX, ">$0034", 0xEC, 0x34, 0x00);

    TEST(DEC,  "$1234", 0xCE, 0x34, 0x12);
    TEST(INC,  "$1234", 0xEE, 0x34, 0x12);
    TEST(INC, ">$0034", 0xEE, 0x34, 0x00);

    TEST(ASL,  "$1234", 0x0E, 0x34, 0x12);
    TEST(ROL,  "$1234", 0x2E, 0x34, 0x12);
    TEST(LSR,  "$1234", 0x4E, 0x34, 0x12);
    TEST(ROR,  "$1234", 0x6E, 0x34, 0x12);
    TEST(ROR, ">$0034", 0x6E, 0x34, 0x00);

    TEST(JMP,  "$1234", 0x4C, 0x34, 0x12);
    TEST(JMP, ">$0034", 0x4C, 0x34, 0x00);
    TEST(JSR,  "$1234", 0x20, 0x34, 0x12);
    TEST(JSR, ">$0034", 0x20, 0x34, 0x00);

    if (m6502()) {
        ERRI(0x0C);
        ERRI(0x1C);
        ERRI(0x9C);
    } else {
        // G65SC02
        TEST(TSB,  "$1234", 0x0C, 0x34, 0x12);
        TEST(TRB,  "$1234", 0x1C, 0x34, 0x12);
        TEST(STZ,  "$1234", 0x9C, 0x34, 0x12);
        TEST(STZ, ">$0034", 0x9C, 0x34, 0x00);
    }
    if (w65c816()) {
        // W65C816
        TEST(PEA, "$1234", 0xF4, 0x34, 0x12);
    }

    symtab.intern(0x0010, "abs0010");
    symtab.intern(0x1234, "abs1234");
    symtab.intern(0x0100, "abs0100");

    // MOS6502
    TEST(SBC, ">abs0010", 0xED, 0x10, 0x00);
    TEST(LDX, ">abs1234", 0xAE, 0x34, 0x12);
    TEST(JSR, ">abs0100", 0x20, 0x00, 0x01);

    if (g65sc02()) {
        // G65SC02
        TEST(TSB, ">abs1234", 0x0C, 0x34, 0x12);
        TEST(TRB, ">abs0010", 0x1C, 0x10, 0x00);
    }
    if (w65c816()) {
        // W65C816
        TEST(PEA, ">abs1234", 0xF4, 0x34, 0x12);
    }
}

static void test_abs_long() {
    // W65C816
    TEST(ORA, "$123456", 0x0F, 0x56, 0x34, 0x12);
    TEST(AND, "$123456", 0x2F, 0x56, 0x34, 0x12);
    TEST(EOR, "$123456", 0x4F, 0x56, 0x34, 0x12);
    TEST(ADC, "$123456", 0x6F, 0x56, 0x34, 0x12);
    TEST(LDA, "$123456", 0xAF, 0x56, 0x34, 0x12);
    TEST(CMP, "$123456", 0xCF, 0x56, 0x34, 0x12);
    TEST(SBC, "$123456", 0xEF, 0x56, 0x34, 0x12);

    TEST(ORA, "$123456,X", 0x1F, 0x56, 0x34, 0x12);
    TEST(AND, "$123456,X", 0x3F, 0x56, 0x34, 0x12);
    TEST(EOR, "$123456,X", 0x5F, 0x56, 0x34, 0x12);
    TEST(ADC, "$123456,X", 0x7F, 0x56, 0x34, 0x12);
    TEST(LDA, "$123456,X", 0xBF, 0x56, 0x34, 0x12);
    TEST(CMP, "$123456,X", 0xDF, 0x56, 0x34, 0x12);
    TEST(SBC, "$123456,X", 0xFF, 0x56, 0x34, 0x12);

    TEST(JMP, "$123456", 0x5C, 0x56, 0x34, 0x12);
    TEST(JSL, "$123456", 0x22, 0x56, 0x34, 0x12);

    TEST(JMP,  "[$1234]", 0xDC, 0x34, 0x12);
    dis6502.setOption("indirect-long", "disable");
    TEST(JMPL, "($1234)", 0xDC, 0x34, 0x12);

    TEST(MVP, "$120000, $340000", 0x44, 0x34, 0x12);
    TEST(MVN, "$000000, $340000", 0x54, 0x34, 0x00);

    symtab.intern(0x1234, "sym1234");
    symtab.intern(0x123456, "long3456");
    symtab.intern(0x120000, "bank12");
    symtab.intern(0x340000, "bank34");

    // W65C816
    TEST(ORA, ">>long3456",   0x0F, 0x56, 0x34, 0x12);
    TEST(ORA, ">>long3456,X", 0x1F, 0x56, 0x34, 0x12);
    TEST(JMP, ">>long3456",   0x5C, 0x56, 0x34, 0x12);
    TEST(JSL, "long3456",     0x22, 0x56, 0x34, 0x12); // absolute long only

    TEST(MVP, "bank12, bank34", 0x44, 0x34, 0x12);

    dis6502.setOption("indirect-long", "enable");
    TEST(JMP,  "[>sym1234]", 0xDC, 0x34, 0x12);

    dis6502.setOption("indirect-long", "disable");
    TEST(JMPL, "(>sym1234)", 0xDC, 0x34, 0x12);
}

static void test_abs_indexed() {
    // MOS6502
    TEST(ORA, ">$0000,X", 0x1D, 0x00, 0x00);
    TEST(AND, ">$0009,X", 0x3D, 0x09, 0x00);
    TEST(EOR, ">$0034,X", 0x5D, 0x34, 0x00);
    TEST(ADC,  "$1234,X", 0x7D, 0x34, 0x12);
    TEST(STA,  "$1234,X", 0x9D, 0x34, 0x12);
    TEST(LDA,  "$1234,X", 0xBD, 0x34, 0x12);
    TEST(CMP,  "$1234,X", 0xDD, 0x34, 0x12);
    TEST(SBC,  "$1234,X", 0xFD, 0x34, 0x12);

    TEST(ORA,  "$1234,Y", 0x19, 0x34, 0x12);
    TEST(AND,  "$1234,Y", 0x39, 0x34, 0x12);
    TEST(EOR, ">$0000,Y", 0x59, 0x00, 0x00);
    TEST(ADC, ">$0009,Y", 0x79, 0x09, 0x00);
    TEST(STA, ">$0034,Y", 0x99, 0x34, 0x00);
    TEST(LDA,  "$1234,Y", 0xB9, 0x34, 0x12);
    TEST(CMP,  "$1234,Y", 0xD9, 0x34, 0x12);
    TEST(SBC,  "$1234,Y", 0xF9, 0x34, 0x12);

    TEST(LDY,  "$1234,X", 0xBC, 0x34, 0x12);
    TEST(LDX,  "$1234,Y", 0xBE, 0x34, 0x12);
    TEST(LDX, ">$0010,Y", 0xBE, 0x10, 0x00);

    TEST(DEC,  "$1234,X", 0xDE, 0x34, 0x12);
    TEST(INC,  "$1234,X", 0xFE, 0x34, 0x12);
    TEST(INC, ">$0034,X", 0xFE, 0x34, 0x00);

    TEST(ASL,  "$1234,X", 0x1E, 0x34, 0x12);
    TEST(ROL,  "$1234,X", 0x3E, 0x34, 0x12);
    TEST(LSR,  "$1234,X", 0x5E, 0x34, 0x12);
    TEST(ROR,  "$1234,X", 0x7E, 0x34, 0x12);
    TEST(ROR, ">$0034,X", 0x7E, 0x34, 0x00);

    if (m6502()) {
        ERRI(0x3C);
        ERRI(0x9E);
    } else {
        // G65SC02
        TEST(BIT,  "$1234,X", 0x3C, 0x34, 0x12);
        TEST(STZ,  "$1234,X", 0x9E, 0x34, 0x12);
    }

    symtab.intern(0x0010, "abs0010");
    symtab.intern(0x1234, "abs1234");
    symtab.intern(0x0100, "abs0100");

    // MOS6502
    TEST(SBC, ">abs0010,X", 0xFD, 0x10, 0x00);
    TEST(STA, ">abs1234,Y", 0x99, 0x34, 0x12);
    TEST(LDY, ">abs0100,X", 0xBC, 0x00, 0x01);
    TEST(LDX, ">abs1234,Y", 0xBE, 0x34, 0x12);
    TEST(LSR, ">abs0100,X", 0x5E, 0x00, 0x01);

    if (g65sc02()) {
        // G65SC02
        TEST(STZ, ">abs1234,X",  0x9E, 0x34, 0x12);
    }
}

static void test_abs_idir() {
    // MOS6502
    TEST(JMP, "(>$0009)", 0x6C, 0x09, 0x00);
    TEST(JMP,  "($1234)", 0x6C, 0x34, 0x12);

    symtab.intern(0x0010, "abs0010");
    symtab.intern(0x1234, "abs1234");

    // MOS6502
    TEST(JMP, "(>abs0010)", 0x6C, 0x10, 0x00);
    TEST(JMP, "(>abs1234)", 0x6C, 0x34, 0x12);
}

static void test_abs_indexed_idir() {
    if (m6502()) {
        ERRI(0x7C);
    } else {
        // G65SC02
        TEST(JMP, "(>$0009,X)", 0x7C, 0x09, 0x00);
        TEST(JMP, "($1234,X)",  0x7C, 0x34, 0x12);

        symtab.intern(0x0010, "abs0010");
        symtab.intern(0x1234, "abs1234");

        // G65SC02
        TEST(JMP, "(>abs0010,X)", 0x7C, 0x10, 0x00);
        TEST(JMP, "(>abs1234,X)", 0x7C, 0x34, 0x12);
    }
    if (w65c816()) {
        // W65C816
        TEST(JSR, "($1235,X)",    0xFC, 0x35, 0x12);
        TEST(JSR, "(>abs1234,X)", 0xFC, 0x34, 0x12);
    }
}

static void test_zpg_idir() {
    if (m6502()) {
        ERRI(0x12);
        ERRI(0x32);
        ERRI(0x52);
        ERRI(0x72);
        ERRI(0x92);
        ERRI(0xB2);
        ERRI(0xD2);
        ERRI(0xF2);
    } else {
        // G65SC02
        TEST(ORA, "($00)", 0x12, 0x00);
        TEST(AND, "($09)", 0x32, 0x09);
        TEST(EOR, "($10)", 0x52, 0x10);
        TEST(ADC, "($10)", 0x72, 0x10);
        TEST(STA, "($10)", 0x92, 0x10);
        TEST(LDA, "($10)", 0xB2, 0x10);
        TEST(CMP, "($10)", 0xD2, 0x10);
        TEST(SBC, "($10)", 0xF2, 0x10);

        symtab.intern(0x0010, "zero10");
        symtab.intern(0x00FF, "zeroFF");

        // G65SC02
        TEST(ORA, "(<zero10)", 0x12, 0x10);
        TEST(AND, "(<zeroFF)", 0x32, 0xFF);
    }
    if (w65c816()) {
        // W65C816
        TEST(PEI, "($11)", 0xD4, 0x11);
        TEST(PEI, "(<zero10)", 0xD4, 0x10);
    }
}

static void test_zpg_indexed_idir() {
    // MOS6502
    TEST(ORA, "($00,X)", 0x01, 0x00);
    TEST(AND, "($09,X)", 0x21, 0x09);
    TEST(EOR, "($10,X)", 0x41, 0x10);
    TEST(ADC, "($10,X)", 0x61, 0x10);
    TEST(STA, "($10,X)", 0x81, 0x10);
    TEST(LDA, "($10,X)", 0xA1, 0x10);
    TEST(CMP, "($10,X)", 0xC1, 0x10);
    TEST(SBC, "($10,X)", 0xE1, 0x10);

    symtab.intern(0x0010, "zero10");

    TEST(LDA, "(<zero10,X)", 0xA1, 0x10);
}

static void test_zpg_idir_indexed() {
    // MOS6502
    TEST(ORA, "($00),Y", 0x11, 0x00);
    TEST(AND, "($09),Y", 0x31, 0x09);
    TEST(EOR, "($10),Y", 0x51, 0x10);
    TEST(ADC, "($10),Y", 0x71, 0x10);
    TEST(STA, "($10),Y", 0x91, 0x10);
    TEST(LDA, "($10),Y", 0xB1, 0x10);
    TEST(CMP, "($10),Y", 0xD1, 0x10);
    TEST(SBC, "($10),Y", 0xF1, 0x10);

    symtab.intern(0x0010, "zero10");

    TEST(LDA, "(<zero10),Y", 0xB1, 0x10);
}

static void test_rel() {
    if (w65c816()) {
        // W65C816
        ATEST(0x001000, BPL, "$001002", 0x10, 0x00);
        ATEST(0x001000, BMI, "$001000", 0x30, 0xFE);
        ATEST(0x001000, BVC, "$001004", 0x50, 0x02);
        ATEST(0x001000, BVS, "$001081", 0x70, 0x7F);
        ATEST(0x001000, BCC, "$000F82", 0x90, 0x80);
        AERRF(0x120000, BCS, "$12FF82", 0xB0, 0x80);
        AERRF(0x12FFFE, BNE, "$120000", 0xD0, 0x00);
        AERRF(0x12FFF0, BEQ, "$120071", 0xF0, 0x7F);
    } else {
        // MOS6502
        ATEST(0x1000, BPL, "$1002", 0x10, 0x00);
        ATEST(0x1000, BMI, "$1000", 0x30, 0xFE);
        ATEST(0x1000, BVC, "$1004", 0x50, 0x02);
        ATEST(0x1000, BVS, "$1081", 0x70, 0x7F);
        ATEST(0x1000, BCC, "$0F82", 0x90, 0x80);
        AERRF(0x0000, BCS, "$FF82", 0xB0, 0x80);
        ATEST(0xFFFE, BNE, "$0000", 0xD0, 0x00);
        AERRF(0xFFF0, BEQ, "$0071", 0xF0, 0x7F);
    }

    if (m6502()) {
        ERRI(0x80);
    } else if (w65c816()) {
        // W65C816
        ATEST(0x121000, BRA, "$121002", 0x80, 0x00);
        ATEST(0x121000, BRL, "$121234", 0x82, 0x31, 0x02);
        ATEST(0x121000, PER, "$121234", 0x62, 0x31, 0x02);
        AERRF(0x121000, BRL, "$129003", 0x82, 0x00, 0x80);
        AERRF(0x129000, PER, "$121002", 0x62, 0xFF, 0x7F);
    } else {
        // G65SC02
        ATEST(0x1000, BRA, "$1002", 0x80, 0x00);
    }

    symtab.intern(0x0F82, "label0F82");
    symtab.intern(0x1000, "label1000");
    symtab.intern(0x1081, "label1081");

    // MO6502
    ATEST(0x1000, BMI, "label1000", 0x30, 0xFE);
    ATEST(0x1000, BVS, "label1081", 0x70, 0x7F);
    ATEST(0x1000, BCC, "label0F82", 0x90, 0x80);

    if (g65sc02()) {
        // G65SC02
        ATEST(0x1000, BRA, "label1000", 0x80, 0xFE);
    }

    disassembler.setOption("relative", "true");
    ATEST(0x2000, BPL, "*-126", 0x10, 0x80);
    ATEST(0x2000, BPL, "*",     0x10, 0xFE);
    ATEST(0x2000, BPL, "*+2",   0x10, 0x00);
    ATEST(0x2000, BPL, "*+129", 0x10, 0x7F);
    if (w65c816()) {
        AERRF(0x120000, BCS, "$12FF82", 0xB0, 0x80);
        AERRF(0x12FFFE, BNE, "$120000", 0xD0, 0x00);
        AERRF(0x12FFF0, BEQ, "$120071", 0xF0, 0x7F);
        AERRF(0x121000, BRL, "$129003", 0x82, 0x00, 0x80);
        AERRF(0x129000, PER, "$121002", 0x62, 0xFF, 0x7F);
    } else {
        AERRF(0x0000, BCS, "*-126", 0xB0, 0x80);
        ATEST(0xFFFE, BNE, "*+2",   0xD0, 0x00);
        AERRF(0xFFF0, BEQ, "*+129", 0xF0, 0x7F);
    }
}

static void test_bitop() {
    if (m6502() || g65sc02()) {
        ERRI(0x07);
        ERRI(0x17);
        ERRI(0x27);
        ERRI(0x37);
        ERRI(0x47);
        ERRI(0x57);
        ERRI(0x67);
        ERRI(0x77);
        ERRI(0x87);
        ERRI(0x97);
        ERRI(0xA7);
        ERRI(0xB7);
        ERRI(0xC7);
        ERRI(0xD7);
        ERRI(0xE7);
        ERRI(0xF7);
    } else if (r65c02() || w65c02s()) {
        // R65C02, W65C02S
        TEST(RMB0, "$10", 0x07, 0x10);
        TEST(RMB1, "$10", 0x17, 0x10);
        TEST(RMB2, "$10", 0x27, 0x10);
        TEST(RMB3, "$10", 0x37, 0x10);
        TEST(RMB4, "$10", 0x47, 0x10);
        TEST(RMB5, "$10", 0x57, 0x10);
        TEST(RMB6, "$10", 0x67, 0x10);
        TEST(RMB7, "$10", 0x77, 0x10);
        TEST(SMB0, "$10", 0x87, 0x10);
        TEST(SMB1, "$10", 0x97, 0x10);
        TEST(SMB2, "$10", 0xA7, 0x10);
        TEST(SMB3, "$10", 0xB7, 0x10);
        TEST(SMB4, "$10", 0xC7, 0x10);
        TEST(SMB5, "$10", 0xD7, 0x10);
        TEST(SMB6, "$10", 0xE7, 0x10);
        TEST(SMB7, "$10", 0xF7, 0x10);

        symtab.intern(0x0010, "zero10");

        // R65C02, W65C02S
        TEST(RMB7, "<zero10", 0x77, 0x10);
        TEST(SMB0, "<zero10", 0x87, 0x10);
    }
}

static void test_zpg_rel() {
    if (m6502() || g65sc02()) {
        ERRI(0x0F);
        ERRI(0x1F);
        ERRI(0x2F);
        ERRI(0x3F);
        ERRI(0x4F);
        ERRI(0x5F);
        ERRI(0x6F);
        ERRI(0x7F);
        ERRI(0x8F);
        ERRI(0x9F);
        ERRI(0xAF);
        ERRI(0xBF);
        ERRI(0xCF);
        ERRI(0xDF);
        ERRI(0xEF);
        ERRI(0xFF);
    } else if (r65c02() || w65c02s()) {
        // R65C02, W65C02S
        ATEST(0x1000, BBR0, "$10,$1003", 0x0F, 0x10, 0x00);
        ATEST(0x1000, BBR1, "$10,$1000", 0x1F, 0x10, 0xFD);
        ATEST(0x1000, BBR2, "$10,$1006", 0x2F, 0x10, 0x03);
        ATEST(0x1000, BBR3, "$10,$1082", 0x3F, 0x10, 0x7F);
        ATEST(0x1000, BBR4, "$10,$0F83", 0x4F, 0x10, 0x80);
        ATEST(0x1000, BBR5, "$10,$1003", 0x5F, 0x10, 0x00);
        ATEST(0x1000, BBR6, "$10,$1003", 0x6F, 0x10, 0x00);
        ATEST(0x1000, BBR7, "$10,$1003", 0x7F, 0x10, 0x00);
        ATEST(0x1000, BBS0, "$10,$1003", 0x8F, 0x10, 0x00);
        ATEST(0x1000, BBS1, "$10,$1000", 0x9F, 0x10, 0xFD);
        ATEST(0x1000, BBS2, "$10,$1006", 0xAF, 0x10, 0x03);
        ATEST(0x1000, BBS3, "$10,$1082", 0xBF, 0x10, 0x7F);
        ATEST(0x1000, BBS4, "$10,$0F83", 0xCF, 0x10, 0x80);
        ATEST(0x1000, BBS5, "$10,$1003", 0xDF, 0x10, 0x00);
        ATEST(0x1000, BBS6, "$10,$1003", 0xEF, 0x10, 0x00);
        ATEST(0x1000, BBS7, "$10,$1003", 0xFF, 0x10, 0x00);

        symtab.intern(0x0010, "zero10");
        symtab.intern(0x0F83, "label0F83");
        symtab.intern(0x1000, "label1000");
        symtab.intern(0x1003, "label1003");
        symtab.intern(0x1082, "label1082");

        // R65C02, W65C02S
        ATEST(0x1000, BBR3, "<zero10,label1082", 0x3F, 0x10, 0x7F);
        ATEST(0x1000, BBR4, "<zero10,label0F83", 0x4F, 0x10, 0x80);
        ATEST(0x1000, BBS4, "<zero10,label1003", 0xCF, 0x10, 0x00);
        ATEST(0x1000, BBS5, "<zero10,label1000", 0xDF, 0x10, 0xFD);
    }
}

static void test_illegal_mos6502() {
    const uint8_t illegals[] = {
        0x80,
        0x02, 0x12, 0x22, 0x32, 0x42, 0x52, 0x62, 0x72,
        0x82, 0x92, 0xB2, 0xC2, 0xD2, 0xE2, 0xF2,
        0x03, 0x13, 0x23, 0x33, 0x43, 0x53, 0x63, 0x73,
        0x83, 0x93, 0xA3, 0xB3, 0xC3, 0xD3, 0xE3, 0xF3,
        0x04, 0x14, 0x34, 0x44, 0x54, 0x64, 0x74, 0xD4, 0xF4,
        0x07, 0x17, 0x27, 0x37, 0x47, 0x57, 0x67, 0x77,
        0x87, 0x97, 0xA7, 0xB7, 0xC7, 0xD7, 0xE7, 0xF7,
        0x89,
        0x1A, 0x3A, 0x5A, 0x7A, 0xDA, 0xFA,
        0x0B, 0x1B, 0x2B, 0x3B, 0x4B, 0x5B, 0x6B, 0x7B,
        0x8B, 0x9B, 0xAB, 0xBB, 0xCB, 0xDB, 0xEB, 0xFB,
        0x0C, 0x1C, 0x3C, 0x5C, 0x7C, 0x9C, 0xDC, 0xFC,
        0x9E,
        0x0F, 0x1F, 0x2F, 0x3F, 0x4F, 0x5F, 0x6F, 0x7F,
        0x8F, 0x9F, 0xAF, 0xBF, 0xCF, 0xDF, 0xEF, 0xFF,
    };
    for (uint8_t idx = 0; idx < sizeof(illegals); idx++)
        ERRI(illegals[idx]);
}

static void test_illegal_g65sc02() {
    const uint8_t illegals[] = {
        0x02, 0x22, 0x42, 0x62, 0x82, 0xC2, 0xE2,
        0x03, 0x13, 0x23, 0x33, 0x43, 0x53, 0x63, 0x73,
        0x83, 0x93, 0xA3, 0xB3, 0xC3, 0xD3, 0xE3, 0xF3,
        0x44, 0x54, 0xD4, 0xF4,
        0x07, 0x17, 0x27, 0x37, 0x47, 0x57, 0x67, 0x77,
        0x87, 0x97, 0xA7, 0xB7, 0xC7, 0xD7, 0xE7, 0xF7,
        0x0B, 0x1B, 0x2B, 0x3B, 0x4B, 0x5B, 0x6B, 0x7B,
        0x8B, 0x9B, 0xAB, 0xBB, 0xCB, 0xDB, 0xEB, 0xFB,
        0x5C, 0xDC, 0xFC,
        0x0F, 0x1F, 0x2F, 0x3F, 0x4F, 0x5F, 0x6F, 0x7F,
        0x8F, 0x9F, 0xAF, 0xBF, 0xCF, 0xDF, 0xEF, 0xFF,
    };
    for (uint8_t idx = 0; idx < sizeof(illegals); idx++)
        ERRI(illegals[idx]);
}

static void test_illegal_r65c02() {
    const uint8_t illegals[] = {
        0x02, 0x22, 0x42, 0x62, 0x82, 0xC2, 0xE2,
        0x03, 0x13, 0x23, 0x33, 0x43, 0x53, 0x63, 0x73,
        0x83, 0x93, 0xA3, 0xB3, 0xC3, 0xD3, 0xE3, 0xF3,
        0x44, 0x54, 0xD4, 0xF4,
        0x0B, 0x1B, 0x2B, 0x3B, 0x4B, 0x5B, 0x6B, 0x7B,
        0x8B, 0x9B, 0xAB, 0xBB, 0xCB, 0xDB, 0xEB, 0xFB,
        0x5C, 0xDC, 0xFC,
    };
    for (uint8_t idx = 0; idx < sizeof(illegals); idx++)
        ERRI(illegals[idx]);
}

static void test_illegal_w65c02s() {
    const uint8_t illegals[] = {
        0x02, 0x22, 0x42, 0x62, 0x82, 0xC2, 0xE2,
        0x03, 0x13, 0x23, 0x33, 0x43, 0x53, 0x63, 0x73,
        0x83, 0x93, 0xA3, 0xB3, 0xC3, 0xD3, 0xE3, 0xF3,
        0x44, 0x54, 0xD4, 0xF4,
        0x0B, 0x1B, 0x2B, 0x3B, 0x4B, 0x5B, 0x6B, 0x7B,
        0x8B, 0x9B, 0xAB, 0xBB, 0xEB, 0xFB,
        0x5C, 0xDC, 0xFC,
    };
    for (uint8_t idx = 0; idx < sizeof(illegals); idx++)
        ERRI(illegals[idx]);
}
// clang-format on

void run_tests(const char *cpu) {
    disassembler.setCpu(cpu);
    RUN_TEST(test_impl);
    RUN_TEST(test_accm);
    RUN_TEST(test_imm);
    RUN_TEST(test_long_imm);
    RUN_TEST(test_zpg);
    RUN_TEST(test_zpg_indexed);
    if (w65c816())
        RUN_TEST(test_zpg_long);
    if (w65c816())
        RUN_TEST(test_sp_rel);
    RUN_TEST(test_abs);
    if (w65c816())
        RUN_TEST(test_abs_long);
    RUN_TEST(test_abs_indexed);
    RUN_TEST(test_abs_idir);
    RUN_TEST(test_zpg_idir);
    RUN_TEST(test_abs_indexed_idir);
    RUN_TEST(test_zpg_indexed_idir);
    RUN_TEST(test_zpg_idir_indexed);
    RUN_TEST(test_rel);
    RUN_TEST(test_bitop);
    RUN_TEST(test_zpg_rel);
    if (m6502())
        RUN_TEST(test_illegal_mos6502);
    if (g65sc02())
        RUN_TEST(test_illegal_g65sc02);
    if (r65c02())
        RUN_TEST(test_illegal_r65c02);
    if (w65c02s())
        RUN_TEST(test_illegal_w65c02s);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

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

static void set_up() {
    disassembler.setCpu("65816");
    dis6502.useIndirectLong(true);
    dis6502.longAccumlator(false);
    dis6502.longIndex(false);
}

static void tear_down() {
    symtab.reset();
}

static void test_cpu() {
    asserter.equals(
        "cpu 65816", true, disassembler.setCpu("65816"));
    asserter.equals(
        "get cpu", "65816", disassembler.getCpu());

    asserter.equals(
        "cpu W65816", true, disassembler.setCpu("W65816"));
    asserter.equals(
        "get cpu", "65816", disassembler.getCpu());

    asserter.equals(
        "cpu W65C816", true, disassembler.setCpu("W65C816"));
    asserter.equals(
        "get cpu", "65816", disassembler.getCpu());

    asserter.equals(
        "cpu W65C816S", true, disassembler.setCpu("W65C816S"));
    asserter.equals(
        "get cpu", "65816", disassembler.getCpu());
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

    // W65SC02
    TEST(PHY, "", 0x5A);
    TEST(PLY, "", 0x7A);
    TEST(PHX, "", 0xDA);
    TEST(PLX, "", 0xFA);

    // W65C02S
    TEST(WAI, "", 0xCB);
    TEST(STP, "", 0xDB);

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

static void test_accm() {
    // MOS6502
    TEST(ASL, "A", 0x0A);
    TEST(ROL, "A", 0x2A);
    TEST(LSR, "A", 0x4A);
    TEST(ROR, "A", 0x6A);

    // W65SC02
    TEST(INC, "A", 0x1A);
    TEST(DEC, "A", 0x3A);
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

    // W65SC02
    TEST(BIT, "#$90", 0x89, 0x90);

    // W65C816
    TEST(COP, "#16",  0x02, 0x10);
    ETEST(UNKNOWN_INSTRUCTION, WDM, "#$10", 0x42, 0x10);
    TEST(REP, "#$20", 0xC2, 0x20);
    TEST(SEP, "#16",  0xE2, 0x10);

    symtab.intern(0x0010, "zero10");
    symtab.intern(0x00FF, "zeroFF");
    symtab.intern(0x0090, "zero90");

    // MOS6502
    TEST(LDX, "#zero10", 0xA2, 0x10);
    TEST(CPY, "#zeroFF", 0xC0, 0xFF);
    TEST(SBC, "#zero90", 0xE9, 0x90);

    // W65SC02
    TEST(BIT, "#zero90", 0x89, 0x90);
}

static void test_long_imm() {
    dis6502.longAccumlator(false);
    dis6502.longIndex(true);

    // MOS6502
    TEST(LDY, "#0",     0xA0, 0x00, 0x00);
    TEST(LDX, "#$1000", 0xA2, 0x00, 0x10);
    TEST(CPY, "#$00FF", 0xC0, 0xFF, 0x00);
    TEST(CPX, "#$FFFF", 0xE0, 0xFF, 0xFF);
    TEST(ORA, "#9",   0x09, 0x09);

    dis6502.longAccumlator(true);
    dis6502.longIndex(false);

    TEST(ORA, "#9",     0x09, 0x09, 0x00);
    TEST(AND, "#$FFF0", 0x29, 0xF0, 0xFF);
    TEST(EOR, "#$007F", 0x49, 0x7F, 0x00);
    TEST(ADC, "#$8000", 0x69, 0x00, 0x80);
    TEST(LDA, "#$FFFF", 0xA9, 0xFF, 0xFF);
    TEST(CMP, "#$8000", 0xC9, 0x00, 0x80);
    TEST(SBC, "#$FFFF", 0xE9, 0xFF, 0xFF);
    TEST(CPX, "#$FF",   0xE0, 0xFF);

    // W65SC02
    TEST(BIT, "#$1234", 0x89, 0x34, 0x12);

    symtab.intern(0x0010, "zero10");
    symtab.intern(0x01FF, "zero1FF");
    symtab.intern(-1,     "minus1");

    dis6502.longAccumlator(true);
    dis6502.longIndex(true);

    TEST(LDX, "#zero10",  0xA2, 0x10, 0x00);
    TEST(CPY, "#zero1FF", 0xC0, 0xFF, 0x01);
    TEST(SBC, "#minus1",  0xE9, 0xFF, 0xFF);

    // W65SC02
    TEST(BIT, "#zero10",  0x89, 0x10, 0x00);

    // always 8bit immediate
    TEST(SEP, "#zero10",  0xE2, 0x10);
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

    // W65SC02
    TEST(TSB, "$10", 0x04, 0x10);
    TEST(TRB, "$10", 0x14, 0x10);
    TEST(STZ, "$10", 0x64, 0x10);

    symtab.intern(0x0010, "zero10");
    symtab.intern(0x00FF, "zeroFF");
    symtab.intern(0x0090, "zero90");

    // MOS6502
    TEST(SBC, "<zero10", 0xE5, 0x10);
    TEST(DEC, "<zeroFF", 0xC6, 0xFF);
    TEST(ROR, "<zero90", 0x66, 0x90);

    // W65SC02
    TEST(STZ, "<zero10", 0x64, 0x10);
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

    // W65SC02
    TEST(BIT, "$10,X", 0x34, 0x10);
    TEST(STZ, "$10,X", 0x74, 0x10);

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

    // W65SC02
    TEST(BIT, "<zero10,X", 0x34, 0x10);
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

    dis6502.useIndirectLong(false);
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

    dis6502.useIndirectLong(true);
    TEST(ORA, "[<dir10]",   0x07, 0x10);
    TEST(ORA, "[<dir10],Y", 0x17, 0x10);

    dis6502.useIndirectLong(false);
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

    // W65SC02
    TEST(TSB,  "$1234", 0x0C, 0x34, 0x12);
    TEST(TRB,  "$1234", 0x1C, 0x34, 0x12);
    TEST(STZ,  "$1234", 0x9C, 0x34, 0x12);
    TEST(STZ, ">$0034", 0x9C, 0x34, 0x00);

    // W65C816
    TEST(PEA, "$1234", 0xF4, 0x34, 0x12);

    symtab.intern(0x0010, "abs0010");
    symtab.intern(0x1234, "abs1234");
    symtab.intern(0x0100, "abs0100");

    // MOS6502
    TEST(SBC, ">abs0010", 0xED, 0x10, 0x00);
    TEST(LDX, ">abs1234", 0xAE, 0x34, 0x12);
    TEST(JSR, ">abs0100", 0x20, 0x00, 0x01);

    // W65SC02
    TEST(TSB, ">abs1234", 0x0C, 0x34, 0x12);
    TEST(TRB, ">abs0010", 0x1C, 0x10, 0x00);

    // W65C816
    TEST(PEA, ">abs1234", 0xF4, 0x34, 0x12);
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
    dis6502.useIndirectLong(false);
    TEST(JMPL, "($1234)", 0xDC, 0x34, 0x12);

    TEST(MVP, "$120000,$340000", 0x44, 0x34, 0x12);
    TEST(MVN, "$000000,$340000", 0x54, 0x34, 0x00);

    symtab.intern(0x1234, "sym1234");
    symtab.intern(0x123456, "long3456");
    symtab.intern(0x120000, "bank12");
    symtab.intern(0x340000, "bank34");

    // W65C816
    TEST(ORA, ">>long3456",   0x0F, 0x56, 0x34, 0x12);
    TEST(ORA, ">>long3456,X", 0x1F, 0x56, 0x34, 0x12);
    TEST(JMP, ">>long3456", 0x5C, 0x56, 0x34, 0x12);
    TEST(JSL, ">>long3456", 0x22, 0x56, 0x34, 0x12);

    TEST(MVP, "bank12,bank34", 0x44, 0x34, 0x12);

    dis6502.useIndirectLong(true);
    TEST(JMP,  "[>sym1234]", 0xDC, 0x34, 0x12);

    dis6502.useIndirectLong(false);
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

    // W65SC02
    TEST(BIT,  "$1234,X", 0x3C, 0x34, 0x12);
    TEST(STZ,  "$1234,X", 0x9E, 0x34, 0x12);

    symtab.intern(0x0010, "abs0010");
    symtab.intern(0x1234, "abs1234");
    symtab.intern(0x0100, "abs0100");

    // MOS6502
    TEST(SBC, ">abs0010,X", 0xFD, 0x10, 0x00);
    TEST(STA, ">abs1234,Y", 0x99, 0x34, 0x12);
    TEST(LDY, ">abs0100,X", 0xBC, 0x00, 0x01);
    TEST(LDX, ">abs1234,Y", 0xBE, 0x34, 0x12);
    TEST(LSR, ">abs0100,X", 0x5E, 0x00, 0x01);

    // W65SC02
    TEST(STZ, ">abs1234,X",  0x9E, 0x34, 0x12);
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
    // W65SC02
    TEST(JMP, "(>$0009,X)", 0x7C, 0x09, 0x00);
    TEST(JMP, "($1234,X)",  0x7C, 0x34, 0x12);

    // W65C816
    TEST(JSR, "($1234,X)",  0xFC, 0x34, 0x12);

    symtab.intern(0x0010, "abs0010");
    symtab.intern(0x1234, "abs1234");

    // W65SC02
    TEST(JMP, "(>abs0010,X)", 0x7C, 0x10, 0x00);
    TEST(JMP, "(>abs1234,X)", 0x7C, 0x34, 0x12);

    // W65C816
    TEST(JSR, "(>abs0010,X)", 0xFC, 0x10, 0x00);
}

static void test_zpg_idir() {
    // W65SC02
    TEST(ORA, "($00)", 0x12, 0x00);
    TEST(AND, "($09)", 0x32, 0x09);
    TEST(EOR, "($10)", 0x52, 0x10);
    TEST(ADC, "($10)", 0x72, 0x10);
    TEST(STA, "($10)", 0x92, 0x10);
    TEST(LDA, "($10)", 0xB2, 0x10);
    TEST(CMP, "($10)", 0xD2, 0x10);
    TEST(SBC, "($10)", 0xF2, 0x10);

    // W65C816
    TEST(PEI, "($10)", 0xD4, 0x10);

    symtab.intern(0x0010, "zero10");
    symtab.intern(0x00FF, "zeroFF");

    // W65SC02
    TEST(ORA, "(<zero10)", 0x12, 0x10);
    TEST(AND, "(<zeroFF)", 0x32, 0xFF);

    // W65C816
    TEST(PEI, "(<zero10)", 0xD4, 0x10);
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

    // MOS6502
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

    // MOS6502
    TEST(LDA, "(<zero10),Y", 0xB1, 0x10);
}

static void test_rel() {
    // MOS6502
    ATEST(0x121000, BPL, "$121002", 0x10, 0x00);
    ATEST(0x121000, BMI, "$121000", 0x30, 0xFE);
    ATEST(0x121000, BVC, "$121004", 0x50, 0x02);
    ATEST(0x121000, BVS, "$121081", 0x70, 0x7F);
    ATEST(0x121000, BCC, "$120F82", 0x90, 0x80);
    ATEST(0x121000, BCS, "$121002", 0xB0, 0x00);
    ATEST(0x121000, BNE, "$121002", 0xD0, 0x00);
    ATEST(0x121000, BEQ, "$121002", 0xF0, 0x00);

    // W65SC02
    ATEST(0x121000, BRA, "$121002", 0x80, 0x00);

    // W65C816
    ATEST(0x121000, BRL, "$121234", 0x82, 0x31, 0x02);
    ATEST(0x121000, PER, "$121234", 0x62, 0x31, 0x02);

    symtab.intern(0x120F82, "label0F82");
    symtab.intern(0x121000, "label1000");
    symtab.intern(0x121081, "label1081");

    // MOS6502
    ATEST(0x121000, BMI, "label1000", 0x30, 0xFE);
    ATEST(0x121000, BVS, "label1081", 0x70, 0x7F);
    ATEST(0x121000, BCC, "label0F82", 0x90, 0x80);

    // W65SC02
    ATEST(0x121000, BRA, "label1000", 0x80, 0xFE);
}

static void run_test(void (*test)(), const char *test_name) {
    asserter.clear(test_name);
    set_up();
    test();
    tear_down();
    asserter.check();
}

int main(int argc, char **argv) {
    RUN_TEST(test_cpu);
    RUN_TEST(test_impl);
    RUN_TEST(test_accm);
    RUN_TEST(test_imm);
    RUN_TEST(test_long_imm);
    RUN_TEST(test_zpg);
    RUN_TEST(test_zpg_indexed);
    RUN_TEST(test_zpg_long);
    RUN_TEST(test_sp_rel);
    RUN_TEST(test_abs);
    RUN_TEST(test_abs_long);
    RUN_TEST(test_abs_indexed);
    RUN_TEST(test_abs_idir);
    RUN_TEST(test_zpg_idir);
    RUN_TEST(test_abs_indexed_idir);
    RUN_TEST(test_zpg_indexed_idir);
    RUN_TEST(test_zpg_idir_indexed);
    RUN_TEST(test_rel);
    return 0;
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

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

#include "asm_mos6502.h"
#include "test_asm_helper.h"

using namespace libasm;
using namespace libasm::mos6502;
using namespace libasm::test;

TestAsserter asserter;
TestSymtab symtab;
AsmMos6502 as6502;
Assembler &assembler(as6502);

static void set_up() {
    assembler.setCpu("6502");
}

static void tear_down() {
    symtab.reset();
}

static void test_cpu() {
    asserter.equals(
        "cpu 6502", true, assembler.setCpu("6502"));
    asserter.equals(
        "cpu mos6502", true, assembler.setCpu("mos6502"));
    asserter.equals(
        "cpu 65sc02", true, assembler.setCpu("65sc02"));
    asserter.equals(
        "cpu w65sc02", true, assembler.setCpu("w65sc02"));
    asserter.equals(
        "cpu 65c02", true, assembler.setCpu("65c02"));
    asserter.equals(
        "cpu r65c02", true, assembler.setCpu("r65c02"));
    asserter.equals(
        "cpu w65c02s", true, assembler.setCpu("w65c02s"));
    asserter.equals(
        "cpu 65816", true, assembler.setCpu("65816"));
    asserter.equals(
        "cpu w65816", true, assembler.setCpu("w65816"));
    asserter.equals(
        "cpu w65c816", true, assembler.setCpu("w65c816"));
    asserter.equals(
        "cpu w65c816s", true, assembler.setCpu("w65c816s"));
}

static void test_impl() {
    TEST("BRK", 0x00);
    TEST("RTI", 0x40);
    TEST("RTS", 0x60);
    TEST("NOP", 0xEA);

    TEST("PHP", 0x08);
    TEST("PLP", 0x28);
    TEST("PHA", 0x48);
    TEST("PLA", 0x68);

    TEST("DEY", 0x88);
    TEST("INY", 0xC8);
    TEST("DEX", 0xCA);
    TEST("INX", 0xE8);

    TEST("TYA", 0x98);
    TEST("TAY", 0xA8);
    TEST("TXA", 0x8A);
    TEST("TAX", 0xAA);
    TEST("TXS", 0x9A);
    TEST("TSX", 0xBA);

    TEST("CLC", 0x18);
    TEST("SEC", 0x38);
    TEST("CLI", 0x58);
    TEST("SEI", 0x78);
    TEST("CLV", 0xB8);
    TEST("CLD", 0xD8);
    TEST("SED", 0xF8);

    // 65SC02
    assembler.setCpu("65sc02");
    TEST("PHY", 0x5A);
    TEST("PLY", 0x7A);
    TEST("PHX", 0xDA);
    TEST("PLX", 0xFA);

    // W65C02S
    assembler.setCpu("W65C02S");
    TEST("WAI", 0xCB);
    TEST("STP", 0xDB);

    // 65816
    assembler.setCpu("65816");
    TEST("RTL", 0x6B);
    TEST("PHB", 0x8B);
    TEST("PHD", 0x0B);
    TEST("PHK", 0x4B);
    TEST("PLB", 0xAB);
    TEST("PLD", 0x2B);
    TEST("TCS", 0x1B);
    TEST("TSC", 0x3B);
    TEST("TCD", 0x5B);
    TEST("TDC", 0x7B);
    TEST("TXY", 0x9B);
    TEST("TYX", 0xBB);
    TEST("XBA", 0xEB);
    TEST("XCE", 0xFB);
}

static void test_accm() {
    TEST("ASL A", 0x0A);
    TEST("ROL A", 0x2A);
    TEST("LSR A", 0x4A);
    TEST("ROR A", 0x6A);

    // 65SC02
    assembler.setCpu("65sc02");
    TEST("INC A", 0x1A);
    TEST("DEC A", 0x3A);
}

static void test_imm() {
    TEST("LDY #0",   0xA0, 0x00);
    TEST("LDX #16",  0xA2, 0x10);
    TEST("CPY #+255", 0xC0, 0xFF);
    TEST("CPX #-1",  0xE0, 0xFF);

    TEST("ORA #%1001", 0x09, 0x09);
    TEST("AND #~$0F", 0x29, 0xF0);
    TEST("EOR #@177", 0x49, 0x7F);
    TEST("ADC #$90", 0x69, 0x90);
    TEST("LDA #$90", 0xA9, 0x90);
    TEST("CMP #$90", 0xC9, 0x90);
    TEST("SBC #$90", 0xE9, 0x90);

    // 65SC02
    assembler.setCpu("65sc02");
    TEST("BIT #$90", 0x89, 0x90);

    // 65816
    assembler.setCpu("65816");
    TEST("COP #$10", 0x02, 0x10);
    TEST("WDM #$10", 0x42, 0x10);
    TEST("REP #$20", 0xC2, 0x20);
    TEST("SEP #$10", 0xE2, 0x10);
    TEST("MVP $12,$34", 0x44, 0x12, 0x34);
    TEST("MVN $12,$34", 0x54, 0x12, 0x34);

    symtab.intern(0x0010, "zero10");
    symtab.intern(0x00FF, "zeroFF");
    symtab.intern(0x0090, "zero90");

    assembler.setCpu("6502");
    TEST("LDX #zero10", 0xA2, 0x10);
    TEST("CPY #zeroFF", 0xC0, 0xFF);
    TEST("SBC #zero90", 0xE9, 0x90);

    // 65SC02
    assembler.setCpu("65sc02");
    TEST("BIT #zero90", 0x89, 0x90);
}

static void test_zpg() {
    TEST("BIT $10", 0x24, 0x10);
    TEST("ORA $10", 0x05, 0x10);
    TEST("AND $10", 0x25, 0x10);
    TEST("EOR $10", 0x45, 0x10);
    TEST("ADC $10", 0x65, 0x10);
    TEST("STA $10", 0x85, 0x10);
    TEST("LDA $10", 0xA5, 0x10);
    TEST("CMP $10", 0xC5, 0x10);
    TEST("SBC $10", 0xE5, 0x10);

    TEST("STY $10", 0x84, 0x10);
    TEST("LDY $10", 0xA4, 0x10);
    TEST("CPY $10", 0xC4, 0x10);
    TEST("STX $10", 0x86, 0x10);
    TEST("LDX $10", 0xA6, 0x10);
    TEST("CPX $10", 0xE4, 0x10);

    TEST("DEC $10", 0xC6, 0x10);
    TEST("INC $10", 0xE6, 0x10);

    TEST("ASL $10", 0x06, 0x10);
    TEST("ROL $10", 0x26, 0x10);
    TEST("LSR $10", 0x46, 0x10);
    TEST("ROR $10", 0x66, 0x10);

    // 65SC02
    assembler.setCpu("65sc02");
    TEST("TSB $10", 0x04, 0x10);
    TEST("TRB $10", 0x14, 0x10);
    TEST("STZ $10", 0x64, 0x10);

    symtab.intern(0x0010, "zero10");
    symtab.intern(0x00FF, "zeroFF");
    symtab.intern(0x0090, "zero90");

    assembler.setCpu("6502");
    TEST("SBC zero10", 0xE5, 0x10);
    TEST("DEC zeroFF", 0xC6, 0xFF);
    TEST("ROR zero90", 0x66, 0x90);

    // 65SC02
    assembler.setCpu("65sc02");
    TEST("STZ zero10", 0x64, 0x10);
}

static void test_zpg_indexed() {
    TEST("ORA $10,X", 0x15, 0x10);
    TEST("AND $10,X", 0x35, 0x10);
    TEST("EOR $10,X", 0x55, 0x10);
    TEST("ADC $10,X", 0x75, 0x10);
    TEST("STA $10,X", 0x95, 0x10);
    TEST("LDA $10,X", 0xB5, 0x10);
    TEST("CMP $10,X", 0xD5, 0x10);
    TEST("SBC $10,X", 0xF5, 0x10);

    TEST("STY $10,X", 0x94, 0x10);
    TEST("LDY $10,X", 0xB4, 0x10);
    TEST("STX $10,Y", 0x96, 0x10);
    TEST("LDX $10,Y", 0xB6, 0x10);

    TEST("DEC $10,X", 0xD6, 0x10);
    TEST("INC $10,X", 0xF6, 0x10);

    TEST("ASL $10,X", 0x16, 0x10);
    TEST("ROL $10,X", 0x36, 0x10);
    TEST("LSR $10,X", 0x56, 0x10);
    TEST("ROR $10,X", 0x76, 0x10);

    // 65SC02
    assembler.setCpu("65sc02");
    TEST("BIT $10,X", 0x34, 0x10);
    TEST("STZ $10,X", 0x74, 0x10);

    symtab.intern(0x0010, "zero10");
    symtab.intern(0x00FF, "zeroFF");
    symtab.intern(0x0090, "zero90");

    assembler.setCpu("6502");
    TEST("SBC zero10,X", 0xF5, 0x10);
    TEST("STY zero90,X", 0x94, 0x90);
    TEST("LDY zeroFF,X", 0xB4, 0xFF);
    TEST("STX zero90,Y", 0x96, 0x90);
    TEST("LDX zeroFF,Y", 0xB6, 0xFF);
    TEST("ROR zero90,X", 0x76, 0x90);

    // 65SC02
    assembler.setCpu("65sc02");
    TEST("BIT zero10,X", 0x34, 0x10);
}

static void test_zpg_long() {
    // 65816
    assembler.setCpu("65816");

    TEST("ORA [$10]", 0x07, 0x10);
    TEST("AND [$10]", 0x27, 0x10);
    TEST("EOR [$10]", 0x47, 0x10);
    TEST("ADC [$10]", 0x67, 0x10);
    TEST("STA [$10]", 0x87, 0x10);
    TEST("LDA [$10]", 0xA7, 0x10);
    TEST("CMP [$10]", 0xC7, 0x10);
    TEST("SBC [$10]", 0xE7, 0x10);

    TEST("ORA [$10],Y", 0x17, 0x10);
    TEST("AND [$10],Y", 0x37, 0x10);
    TEST("EOR [$10],Y", 0x57, 0x10);
    TEST("ADC [$10],Y", 0x77, 0x10);
    TEST("STA [$10],Y", 0x97, 0x10);
    TEST("LDA [$10],Y", 0xB7, 0x10);
    TEST("CMP [$10],Y", 0xD7, 0x10);
    TEST("SBC [$10],Y", 0xF7, 0x10);
}

static void test_sp_rel() {
    // 65816
    assembler.setCpu("65816");
    TEST("ORA $10,S", 0x03, 0x10);
    TEST("AND $10,S", 0x23, 0x10);
    TEST("EOR $10,S", 0x43, 0x10);
    TEST("ADC $10,S", 0x63, 0x10);
    TEST("STA $10,S", 0x83, 0x10);
    TEST("LDA $10,S", 0xA3, 0x10);
    TEST("CMP $10,S", 0xC3, 0x10);
    TEST("SBC $10,S", 0xE3, 0x10);

    TEST("ORA ($10,S),Y", 0x13, 0x10);
    TEST("AND ($10,S),Y", 0x33, 0x10);
    TEST("EOR ($10,S),Y", 0x53, 0x10);
    TEST("ADC ($10,S),Y", 0x73, 0x10);
    TEST("STA ($10,S),Y", 0x93, 0x10);
    TEST("LDA ($10,S),Y", 0xB3, 0x10);
    TEST("CMP ($10,S),Y", 0xD3, 0x10);
    TEST("SBC ($10,S),Y", 0xF3, 0x10);
}

static void test_abs() {
    TEST("BIT $1234", 0x2C, 0x34, 0x12);
    TEST("ORA $1234", 0x0D, 0x34, 0x12);
    TEST("AND $1234", 0x2D, 0x34, 0x12);
    TEST("EOR $1234", 0x4D, 0x34, 0x12);
    TEST("ADC $1234", 0x6D, 0x34, 0x12);
    TEST("STA $1234", 0x8D, 0x34, 0x12);
    TEST("LDA $1234", 0xAD, 0x34, 0x12);
    TEST("CMP $1234", 0xCD, 0x34, 0x12);
    TEST("SBC $1234", 0xED, 0x34, 0x12);

    TEST("STY $1234", 0x8C, 0x34, 0x12);
    TEST("LDY $1234", 0xAC, 0x34, 0x12);
    TEST("CPY $1234", 0xCC, 0x34, 0x12);
    TEST("STX $1234", 0x8E, 0x34, 0x12);
    TEST("LDX $1234", 0xAE, 0x34, 0x12);
    TEST("CPX $1234", 0xEC, 0x34, 0x12);

    TEST("DEC $1234", 0xCE, 0x34, 0x12);
    TEST("INC $1234", 0xEE, 0x34, 0x12);

    TEST("ASL $1234", 0x0E, 0x34, 0x12);
    TEST("ROL $1234", 0x2E, 0x34, 0x12);
    TEST("LSR $1234", 0x4E, 0x34, 0x12);
    TEST("ROR $1234", 0x6E, 0x34, 0x12);

    TEST("JMP $1234", 0x4C, 0x34, 0x12);
    TEST("JSR $1234", 0x20, 0x34, 0x12);

    // 65SC02
    assembler.setCpu("65sc02");
    TEST("TSB $1234", 0x0C, 0x34, 0x12);
    TEST("TRB $1234", 0x1C, 0x34, 0x12);
    TEST("STZ $1234", 0x9C, 0x34, 0x12);

    // 65816
    assembler.setCpu("65816");
    TEST("PEA $1234", 0xF4, 0x34, 0x12);

    symtab.intern(0x0010, "abs0010");
    symtab.intern(0x1234, "abs1234");
    symtab.intern(0x0100, "abs0100");

    assembler.setCpu("6502");
    TEST("SBC >abs0010", 0xED, 0x10, 0x00);
    TEST("LDX abs1234",  0xAE, 0x34, 0x12);
    TEST("JSR abs0100",  0x20, 0x00, 0x01);

    // 65SC02
    assembler.setCpu("65sc02");
    TEST("TSB abs1234",  0x0C, 0x34, 0x12);

    // 65816
    assembler.setCpu("65816");
    TEST("PEA abs0010", 0xF4, 0x10, 0x00);
}

static void test_abs_long() {
    // 65816
    assembler.setCpu("65816");
    TEST("ORA $123456", 0x0F, 0x56, 0x34, 0x12);
    TEST("AND $123456", 0x2F, 0x56, 0x34, 0x12);
    TEST("EOR $123456", 0x4F, 0x56, 0x34, 0x12);
    TEST("ADC $123456", 0x6F, 0x56, 0x34, 0x12);
    TEST("STA $123456", 0x8F, 0x56, 0x34, 0x12);
    TEST("LDA $123456", 0xAF, 0x56, 0x34, 0x12);
    TEST("CMP $123456", 0xCF, 0x56, 0x34, 0x12);
    TEST("SBC $123456", 0xEF, 0x56, 0x34, 0x12);

    TEST("JMP $123456", 0x5C, 0x56, 0x34, 0x12);
    TEST("JSL $123456", 0x22, 0x56, 0x34, 0x12);
    TEST("JMP [$1234]", 0xDC, 0x34, 0x12);

    TEST("ORA $123456,X", 0x1F, 0x56, 0x34, 0x12);
    TEST("AND $123456,X", 0x3F, 0x56, 0x34, 0x12);
    TEST("EOR $123456,X", 0x5F, 0x56, 0x34, 0x12);
    TEST("ADC $123456,X", 0x7F, 0x56, 0x34, 0x12);
    TEST("STA $123456,X", 0x9F, 0x56, 0x34, 0x12);
    TEST("LDA $123456,X", 0xBF, 0x56, 0x34, 0x12);
    TEST("CMP $123456,X", 0xDF, 0x56, 0x34, 0x12);
    TEST("SBC $123456,X", 0xFF, 0x56, 0x34, 0x12);

    symtab.intern(0x1234, "sym1234");
    symtab.intern(0x123456, "long3456");

    TEST("JMP [sym1234]", 0xDC, 0x34, 0x12);
    TEST("JMP >>sym1234",   0x5C, 0x34, 0x12, 0x00);
    TEST("JSL long3456",    0x22, 0x56, 0x34, 0x12);
    TEST("ORA long3456",    0x0F, 0x56, 0x34, 0x12);
    TEST("ORA >>sym1234,X", 0x1F, 0x34, 0x12, 0x00);
    TEST("ORA long3456,X",  0x1F, 0x56, 0x34, 0x12);
}

static void test_abs_indexed() {
    TEST("ORA $1234,X", 0x1D, 0x34, 0x12);
    TEST("AND $1234,X", 0x3D, 0x34, 0x12);
    TEST("EOR $1234,X", 0x5D, 0x34, 0x12);
    TEST("ADC $1234,X", 0x7D, 0x34, 0x12);
    TEST("STA $1234,X", 0x9D, 0x34, 0x12);
    TEST("LDA $1234,X", 0xBD, 0x34, 0x12);
    TEST("CMP $1234,X", 0xDD, 0x34, 0x12);
    TEST("SBC $1234,X", 0xFD, 0x34, 0x12);

    TEST("ORA $1234,Y", 0x19, 0x34, 0x12);
    TEST("AND $1234,Y", 0x39, 0x34, 0x12);
    TEST("EOR $1234,Y", 0x59, 0x34, 0x12);
    TEST("ADC $1234,Y", 0x79, 0x34, 0x12);
    TEST("STA $1234,Y", 0x99, 0x34, 0x12);
    TEST("LDA $1234,Y", 0xB9, 0x34, 0x12);
    TEST("CMP $1234,Y", 0xD9, 0x34, 0x12);
    TEST("SBC $1234,Y", 0xF9, 0x34, 0x12);

    TEST("LDY $1234,X", 0xBC, 0x34, 0x12);
    TEST("LDX $1234,Y", 0xBE, 0x34, 0x12);

    TEST("DEC $1234,X", 0xDE, 0x34, 0x12);
    TEST("INC $1234,X", 0xFE, 0x34, 0x12);

    TEST("ASL $1234,X", 0x1E, 0x34, 0x12);
    TEST("ROL $1234,X", 0x3E, 0x34, 0x12);
    TEST("LSR $1234,X", 0x5E, 0x34, 0x12);
    TEST("ROR $1234,X", 0x7E, 0x34, 0x12);

    // 65SC02
    assembler.setCpu("65sc02");
    TEST("BIT $1234,X", 0x3C, 0x34, 0x12);
    TEST("STZ $1234,X", 0x9E, 0x34, 0x12);

    symtab.intern(0x0010, "abs0010");
    symtab.intern(0x1234, "abs1234");
    symtab.intern(0x0100, "abs0100");

    assembler.setCpu("6502");
    TEST("SBC >abs0010,X", 0xFD, 0x10, 0x00);
    TEST("STA abs1234,Y",  0x99, 0x34, 0x12);
    TEST("LDY abs0100,X",  0xBC, 0x00, 0x01);
    TEST("LDX abs1234,Y",  0xBE, 0x34, 0x12);
    TEST("LSR abs0100,X",  0x5E, 0x00, 0x01);

    // 65SC02
    assembler.setCpu("65sc02");
    TEST("STZ abs1234,X",  0x9E, 0x34, 0x12);
}

static void test_abs_idir() {
    TEST("JMP ($1234)", 0x6C, 0x34, 0x12);

    symtab.intern(0x0010, "abs0010");
    symtab.intern(0x1234, "abs1234");

    TEST("JMP (>abs0010)", 0x6C, 0x10, 0x00);
    TEST("JMP (abs1234)",  0x6C, 0x34, 0x12);
}

static void test_abs_indexed_idir() {
    // 65SC02
    assembler.setCpu("65sc02");
    TEST("JMP ($1234,X)", 0x7C, 0x34, 0x12);

    // 65816
    assembler.setCpu("65816");
    TEST("JSR ($1234,X)",  0xFC, 0x34, 0x12);

    symtab.intern(0x0010, "abs0010");
    symtab.intern(0x1234, "abs1234");

    // 65SC02
    TEST("JMP (>abs0010,X)", 0x7C, 0x10, 0x00);
    TEST("JMP (abs1234,X)",  0x7C, 0x34, 0x12);

    // 65816
    assembler.setCpu("65816");
    TEST("JSR (>abs0010,X)", 0xFC, 0x10, 0x00);
}

static void test_zpg_idir() {
    // 65SC02
    assembler.setCpu("65sc02");
    TEST("ORA ($10)", 0x12, 0x10);
    TEST("AND ($10)", 0x32, 0x10);
    TEST("EOR ($10)", 0x52, 0x10);
    TEST("ADC ($10)", 0x72, 0x10);
    TEST("STA ($10)", 0x92, 0x10);
    TEST("LDA ($10)", 0xB2, 0x10);
    TEST("CMP ($10)", 0xD2, 0x10);
    TEST("SBC ($10)", 0xF2, 0x10);

    // 65816
    assembler.setCpu("65816");
    TEST("PEI ($10)", 0xD4, 0x10);

    symtab.intern(0x0010, "zero10");
    symtab.intern(0x00FF, "zeroFF");

    TEST("ORA (zero10)", 0x12, 0x10);
    TEST("AND (zeroFF)", 0x32, 0xFF);
}

static void test_zpg_indexed_idir() {
    TEST("ORA ($10,X)", 0x01, 0x10);
    TEST("AND ($10,X)", 0x21, 0x10);
    TEST("EOR ($10,X)", 0x41, 0x10);
    TEST("ADC ($10,X)", 0x61, 0x10);
    TEST("STA ($10,X)", 0x81, 0x10);
    TEST("LDA ($10,X)", 0xA1, 0x10);
    TEST("CMP ($10,X)", 0xC1, 0x10);
    TEST("SBC ($10,X)", 0xE1, 0x10);

    symtab.intern(0x0010, "zero10");

    TEST("LDA (zero10,X)", 0xA1, 0x10);
}

static void test_zpg_idir_indexed() {
    TEST("ORA ($10),Y", 0x11, 0x10);
    TEST("AND ($10),Y", 0x31, 0x10);
    TEST("EOR ($10),Y", 0x51, 0x10);
    TEST("ADC ($10),Y", 0x71, 0x10);
    TEST("STA ($10),Y", 0x91, 0x10);
    TEST("LDA ($10),Y", 0xB1, 0x10);
    TEST("CMP ($10),Y", 0xD1, 0x10);
    TEST("SBC ($10),Y", 0xF1, 0x10);

    symtab.intern(0x0010, "zero10");

    TEST("LDA (zero10),Y", 0xB1, 0x10);
}

static void test_rel() {
    ATEST(0x1000, "BPL $1002", 0x10, 0x00);
    ATEST(0x1000, "BMI $1000", 0x30, 0xFE);
    ATEST(0x1000, "BVC $1004", 0x50, 0x02);
    ATEST(0x1000, "BVS $1081", 0x70, 0x7F);
    ATEST(0x1000, "BCC $0F82", 0x90, 0x80);
    ATEST(0x1000, "BCS $1002", 0xB0, 0x00);
    ATEST(0x1000, "BNE $1002", 0xD0, 0x00);
    ATEST(0x1000, "BEQ $1002", 0xF0, 0x00);

    // 65SC02
    assembler.setCpu("65sc02");
    ATEST(0x1000, "BRA $1002", 0x80, 0x00);

    // 65816
    assembler.setCpu("65816");
    ATEST(0x1000, "BRL $1234", 0x82, 0x31, 0x02);
    ATEST(0x1000, "PER $1234", 0x62, 0x31, 0x02);

    symtab.intern(0x0F82, "label0F82");
    symtab.intern(0x1000, "label1000");
    symtab.intern(0x1081, "label1081");

    assembler.setCpu("6502");
    ATEST(0x1000, "BMI label1000", 0x30, 0xFE);
    ATEST(0x1000, "BVS label1081", 0x70, 0x7F);
    ATEST(0x1000, "BCC label0F82", 0x90, 0x80);

    // 65SC02
    assembler.setCpu("65sc02");
    ATEST(0x1000, "BRA label1000", 0x80, 0xFE);
}

static void test_bitop() {
    // 65C02
    assembler.setCpu("65c02");
    TEST("RMB0 $10", 0x07, 0x10);
    TEST("RMB1 $10", 0x17, 0x10);
    TEST("RMB2 $10", 0x27, 0x10);
    TEST("RMB3 $10", 0x37, 0x10);
    TEST("RMB4 $10", 0x47, 0x10);
    TEST("RMB5 $10", 0x57, 0x10);
    TEST("RMB6 $10", 0x67, 0x10);
    TEST("RMB7 $10", 0x77, 0x10);
    TEST("SMB0 $10", 0x87, 0x10);
    TEST("SMB1 $10", 0x97, 0x10);
    TEST("SMB2 $10", 0xA7, 0x10);
    TEST("SMB3 $10", 0xB7, 0x10);
    TEST("SMB4 $10", 0xC7, 0x10);
    TEST("SMB5 $10", 0xD7, 0x10);
    TEST("SMB6 $10", 0xE7, 0x10);
    TEST("SMB7 $10", 0xF7, 0x10);

    symtab.intern(0x0010, "zero10");

    TEST("RMB7 zero10", 0x77, 0x10);
    TEST("SMB0 zero10", 0x87, 0x10);
}

static void test_zpg_rel() {
    // 65C02
    assembler.setCpu("65c02");
    ATEST(0x1000, "BBR0 $10,$1003", 0x0F, 0x10, 0x00);
    ATEST(0x1000, "BBR1 $10,$1000", 0x1F, 0x10, 0xFD);
    ATEST(0x1000, "BBR2 $10,$1006", 0x2F, 0x10, 0x03);
    ATEST(0x1000, "BBR3 $10,$1082", 0x3F, 0x10, 0x7F);
    ATEST(0x1000, "BBR4 $10,$0F83", 0x4F, 0x10, 0x80);
    ATEST(0x1000, "BBR5 $10,$1003", 0x5F, 0x10, 0x00);
    ATEST(0x1000, "BBR6 $10,$1003", 0x6F, 0x10, 0x00);
    ATEST(0x1000, "BBR7 $10,$1003", 0x7F, 0x10, 0x00);
    ATEST(0x1000, "BBS0 $10,$1003", 0x8F, 0x10, 0x00);
    ATEST(0x1000, "BBS1 $10,$1000", 0x9F, 0x10, 0xFD);
    ATEST(0x1000, "BBS2 $10,$1006", 0xAF, 0x10, 0x03);
    ATEST(0x1000, "BBS3 $10,$1082", 0xBF, 0x10, 0x7F);
    ATEST(0x1000, "BBS4 $10,$0F83", 0xCF, 0x10, 0x80);
    ATEST(0x1000, "BBS5 $10,$1003", 0xDF, 0x10, 0x00);
    ATEST(0x1000, "BBS6 $10,$1003", 0xEF, 0x10, 0x00);
    ATEST(0x1000, "BBS7 $10,$1003", 0xFF, 0x10, 0x00);

    symtab.intern(0x0010, "zero10");
    symtab.intern(0x0F83, "label0F83");
    symtab.intern(0x1000, "label1000");
    symtab.intern(0x1003, "label1003");
    symtab.intern(0x1082, "label1082");

    ATEST(0x1000, "BBR3 zero10,label1082", 0x3F, 0x10, 0x7F);
    ATEST(0x1000, "BBR4 zero10,label0F83", 0x4F, 0x10, 0x80);
    ATEST(0x1000, "BBS4 zero10,label1003", 0xCF, 0x10, 0x00);
    ATEST(0x1000, "BBS5 zero10,label1000", 0xDF, 0x10, 0xFD);
}

static void test_no_idir_long() {
    // 65816
    assembler.setCpu("65816");

    TEST("ORAL ($10)", 0x07, 0x10);
    TEST("ANDL ($10)", 0x27, 0x10);
    TEST("EORL ($10)", 0x47, 0x10);
    TEST("ADCL ($10)", 0x67, 0x10);
    TEST("STAL ($10)", 0x87, 0x10);
    TEST("LDAL ($10)", 0xA7, 0x10);
    TEST("CMPL ($10)", 0xC7, 0x10);
    TEST("SBCL ($10)", 0xE7, 0x10);

    TEST("ORAL ($10),Y", 0x17, 0x10);
    TEST("ANDL ($10),Y", 0x37, 0x10);
    TEST("EORL ($10),Y", 0x57, 0x10);
    TEST("ADCL ($10),Y", 0x77, 0x10);
    TEST("STAL ($10),Y", 0x97, 0x10);
    TEST("LDAL ($10),Y", 0xB7, 0x10);
    TEST("CMPL ($10),Y", 0xD7, 0x10);
    TEST("SBCL ($10),Y", 0xF7, 0x10);

    TEST("JMPL ($1234)", 0xDC, 0x34, 0x12);

    symtab.intern(0x1234, "sym1234");

    TEST("JMPL (sym1234)", 0xDC, 0x34, 0x12);
}

static void test_comment() {
    TEST("BRK        ; comment", 0x00);
    TEST("ASL A      ; comment", 0x0A);
    TEST("CPY #+255  ; comment", 0xC0, 0xFF);
    TEST("BIT $10    ; comment", 0x24, 0x10);
    TEST("ORA $10,X  ; comment", 0x15, 0x10);
    TEST("LDX $10,Y  ; comment", 0xB6, 0x10);
    TEST("BIT $1234  ; comment", 0x2C, 0x34, 0x12);
    TEST("ORA $1234,X; comment", 0x1D, 0x34, 0x12);
    TEST("ORA $1234,Y; comment", 0x19, 0x34, 0x12);
    TEST("JMP ($1234); comment", 0x6C, 0x34, 0x12);
    TEST("ORA ($10,X); comment", 0x01, 0x10);
    TEST("ORA ($10),Y; comment", 0x11, 0x10);
    ATEST(0x1000, "BPL $1002; comment", 0x10, 0x00);
}

static void test_undefined_symbol() {
    ETEST(UNDEFINED_SYMBOL, "LDA #UNDEF", 0xA9, 0x00);
    ETEST(UNDEFINED_SYMBOL, "JSR UNDEF",  0x20, 0x00, 0x00);
    ETEST(UNDEFINED_SYMBOL, "JSR >UNDEF", 0x20, 0x00, 0x00);
    ETEST(UNDEFINED_SYMBOL, "BIT UNDEF",  0x24, 0x00);
    ETEST(UNDEFINED_SYMBOL, "BIT <UNDEF", 0x24, 0x00);
    ETEST(UNDEFINED_SYMBOL, "BIT >UNDEF", 0x2C, 0x00, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LDY UNDEF,X",  0xB4, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LDY <UNDEF,X", 0xB4, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LDY >UNDEF,X", 0xBC, 0x00, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LDX UNDEF,Y",  0xB6, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LDX <UNDEF,Y", 0xB6, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LDX >UNDEF,Y", 0xBE, 0x00, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LDA (UNDEF,X)",  0xA1, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LDA (<UNDEF,X)", 0xA1, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LDA (UNDEF),Y",  0xB1, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LDA (<UNDEF),Y", 0xB1, 0x00);
    ETEST(UNDEFINED_SYMBOL, "JMP (UNDEF)",    0x6C, 0x00, 0x00);

    EATEST(UNDEFINED_SYMBOL, 0x1000, "BCC UNDEF",     0x90, 0xFE);

    // 65SC02
    assembler.setCpu("65sc02");
    ETEST(UNDEFINED_SYMBOL, "BIT #UNDEF",   0x89, 0x00);
    ETEST(UNDEFINED_SYMBOL, "BIT UNDEF,X",  0x34, 0x00);
    ETEST(UNDEFINED_SYMBOL, "BIT <UNDEF,X", 0x34, 0x00);
    ETEST(UNDEFINED_SYMBOL, "BIT >UNDEF,X", 0x3C, 0x00, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LDA (UNDEF)",  0xB2, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LDA (<UNDEF)", 0xB2, 0x00);
    ETEST(UNDEFINED_SYMBOL, "JMP (UNDEF,X)",  0x7C, 0x00, 0x00);
    ETEST(UNDEFINED_SYMBOL, "JMP (>UNDEF,X)", 0x7C, 0x00, 0x00);

    EATEST(UNDEFINED_SYMBOL, 0x1000, "BRA UNDEF",     0x80, 0xFE);

    assembler.setCpu("65c02");
    ETEST(UNDEFINED_SYMBOL, "RMB5 UNDEF",  0x57, 0x00);
    ETEST(UNDEFINED_SYMBOL, "SMB4 <UNDEF", 0xC7, 0x00);

    EATEST(UNDEFINED_SYMBOL, 0x1000, "BBR3 UNDEF,$1082", 0x3F, 0x00, 0x7F);
    EATEST(UNDEFINED_SYMBOL, 0x1000, "BBS6 $10,UNDEF",   0xEF, 0x10, 0xFD);
    EATEST(UNDEFINED_SYMBOL, 0x1000, "BBS7 UNDEF,UNDEF", 0xFF, 0x00, 0xFD);

    // 65816
    assembler.setCpu("65816");
    ETEST(UNDEFINED_SYMBOL, "SEP #UNDEF",      0xE2, 0x00);
    ETEST(UNDEFINED_SYMBOL, "MVP $12,UNDEF",   0x44, 0x12, 0x00);
    ETEST(UNDEFINED_SYMBOL, "MVP UNDEF,$12",   0x44, 0x00, 0x12);
    ETEST(UNDEFINED_SYMBOL, "MVN UNDEF,UNDEF", 0x54, 0x00, 0x00);
    ETEST(UNDEFINED_SYMBOL, "ORA [UNDEF]",     0x07, 0x00);
    ETEST(UNDEFINED_SYMBOL, "AND [UNDEF],Y",   0x37, 0x00);
    ETEST(UNDEFINED_SYMBOL, "EOR UNDEF,S",     0x43, 0x00);
    ETEST(UNDEFINED_SYMBOL, "ADC (UNDEF,S),Y", 0x73, 0x00);
    ETEST(UNDEFINED_SYMBOL, "PEA UNDEF",       0xF4, 0x00, 0x00);
    ETEST(UNDEFINED_SYMBOL, "PEI (UNDEF)",     0xD4, 0x00);
    ETEST(UNDEFINED_SYMBOL, "STA   UNDEF",     0x85, 0x00);
    ETEST(UNDEFINED_SYMBOL, "STA  <UNDEF",     0x85, 0x00);
    ETEST(UNDEFINED_SYMBOL, "STA  >UNDEF",     0x8D, 0x00, 0x00);
    ETEST(UNDEFINED_SYMBOL, "STA >>UNDEF",     0x8F, 0x00, 0x00, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LDA   UNDEF,X",   0xB5, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LDA  <UNDEF,X",   0xB5, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LDA  >UNDEF,X",   0xBD, 0x00, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LDA >>UNDEF,X",   0xBF, 0x00, 0x00, 0x00);
    ETEST(UNDEFINED_SYMBOL, "JMP [UNDEF]",     0xDC, 0x00, 0x00);
    ETEST(UNDEFINED_SYMBOL, "JSR (UNDEF,X)",   0xFC, 0x00, 0x00);

    ETEST(UNDEFINED_SYMBOL, "SBCL (UNDEF)",    0xE7, 0x00);
    ETEST(UNDEFINED_SYMBOL, "CMPL (UNDEF),Y",  0xD7, 0x00);
    ETEST(UNDEFINED_SYMBOL, "JMPL (UNDEF)",    0xDC, 0x00, 0x00);

    EATEST(UNDEFINED_SYMBOL, 0x1000, "BRL UNDEF", 0x82, 0xFD, 0xFF);
    EATEST(UNDEFINED_SYMBOL, 0x1000, "PER UNDEF", 0x62, 0xFD, 0xFF);
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
    RUN_TEST(test_bitop);
    RUN_TEST(test_zpg_rel);
    RUN_TEST(test_no_idir_long);
    RUN_TEST(test_comment);
    RUN_TEST(test_undefined_symbol);
    return 0;
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
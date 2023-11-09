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

AsmMos6502 as6502;
Assembler &assembler(as6502);

static bool m6502() {
    return strcmp_P("6502", assembler.config().cpu_P()) == 0;
}

static bool r65c02() {
    return strcmp_P("65C02", assembler.config().cpu_P()) == 0;
}

static bool w65c02s() {
    return strcmp_P("W65C02S", assembler.config().cpu_P()) == 0;
}

static bool w65c816() {
    return strcmp_P("65816", assembler.config().cpu_P()) == 0;
}

static void set_up() {
    assembler.reset();
}

static void tear_down() {
    symtab.reset();
}

// clang-format off
void test_cpu() {
    EQUALS("cpu 6502", true, assembler.setCpu("6502"));
    EQUALS_P("cpu 6502", "6502", assembler.config().cpu_P());

    EQUALS("cpu 65sc02", true, assembler.setCpu("65sc02"));
    EQUALS_P("cpu 65sc02", "65SC02", assembler.config().cpu_P());

    EQUALS("cpu 65c02", true, assembler.setCpu("65c02"));
    EQUALS_P("cpu 65c02", "65C02", assembler.config().cpu_P());

    EQUALS("cpu mos6502", true, assembler.setCpu("mos6502"));
    EQUALS_P("cpu mos6502", "6502", assembler.config().cpu_P());

    EQUALS("cpu g65sc02", true, assembler.setCpu("g65sc02"));
    EQUALS_P("cpu g65sc02", "65SC02", assembler.config().cpu_P());

    EQUALS("cpu r65c02", true, assembler.setCpu("r65c02"));
    EQUALS_P("cpu r65c02", "65C02", assembler.config().cpu_P());

    EQUALS("cpu w65c02", true, assembler.setCpu("w65c02"));
    EQUALS_P("cpu w65c02", "W65C02S", assembler.config().cpu_P());

    EQUALS("cpu w65c02s", true, assembler.setCpu("w65c02s"));
    EQUALS_P("cpu w65c02s", "W65C02S", assembler.config().cpu_P());

    EQUALS("cpu 65816", true, assembler.setCpu("65816"));
    EQUALS_P("cpu 65816", "65816", assembler.config().cpu_P());

    EQUALS("cpu w65816", true, assembler.setCpu("w65816"));
    EQUALS_P("cpu w65816", "65816", assembler.config().cpu_P());

    EQUALS("cpu w65c816", true, assembler.setCpu("w65c816"));
    EQUALS_P("cpu w65c816", "65816", assembler.config().cpu_P());

    EQUALS("cpu w65c816s", true, assembler.setCpu("w65c816s"));
    EQUALS_P("cpu w65c816s", "65816", assembler.config().cpu_P());
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

    if (m6502()) {
        ERUI("PHY");
        ERUI("PLY");
        ERUI("PHX");
        ERUI("PLX");
    } else {
        // G65SC02
        TEST("PHY", 0x5A);
        TEST("PLY", 0x7A);
        TEST("PHX", 0xDA);
        TEST("PLX", 0xFA);
    }
    if (w65c02s() || w65c816()) {
        TEST("WAI", 0xCB);
        TEST("STP", 0xDB);
    } else {
        ERUI("WAI");
        ERUI("STP");
    }
    if (w65c816()) {
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
    } else {
        ERUI("RTL");
        ERUI("PHB");
        ERUI("PHD");
        ERUI("PHK");
        ERUI("PLB");
        ERUI("PLD");
        ERUI("TCS");
        ERUI("TSC");
        ERUI("TCD");
        ERUI("TDC");
        ERUI("TXY");
        ERUI("TYX");
        ERUI("XBA");
        ERUI("XCE");
    }
}

static void test_accm() {
    // MOS6502
    TEST("ASL A", 0x0A);
    TEST("ROL A", 0x2A);
    TEST("LSR A", 0x4A);
    TEST("ROR A", 0x6A);

    if (m6502()) {
        ERRT("INC A", REGISTER_NOT_ALLOWED, "A");
        ERRT("DEC A", REGISTER_NOT_ALLOWED, "A");
    } else {
        // G65SC02
        TEST("INC A", 0x1A);
        TEST("DEC A", 0x3A);
    }
}

static void test_imm() {
    // MOS6502
    TEST("LDY #0",    0xA0, 0x00);
    TEST("LDX #16",   0xA2, 0x10);
    TEST("CPY #+255", 0xC0, 0xFF);
    TEST("CPX #-1",   0xE0, 0xFF);

    TEST("ORA #%1001", 0x09, 0x09);
    TEST("AND #~$0F",  0x29, 0xF0);
    TEST("EOR #@177",  0x49, 0x7F);
    TEST("ADC #$90",   0x69, 0x90);
    TEST("LDA #$90",   0xA9, 0x90);
    TEST("CMP #$90",   0xC9, 0x90);
    TEST("SBC #$90",   0xE9, 0x90);
    ERRT("LDA #256",   OVERFLOW_RANGE, "#256", 0xA9, 0x00);
    TEST("LDA #-1",    0xA9, 0xFF);
    TEST("LDA #-128",  0xA9, 0x80);

    if (m6502()) {
        ERRT("BIT #$90", OPERAND_NOT_ALLOWED, "#$90");
    } else {
        // G65SC02
        TEST("BIT #$90", 0x89, 0x90);
    }
    if (w65c816()) {
        TEST("COP #$10", 0x02, 0x10);
        TEST("WDM #$10", 0x42, 0x10);
        TEST("REP #$20", 0xC2, 0x20);
        TEST("SEP #$10", 0xE2, 0x10);
    } else {
        ERUI("COP #$10");
        ERUI("WDM #$10");
        ERUI("REP #$20");
        ERUI("SEP #$10");
    }

    if (w65c816()) {
        TEST("LONGA ON");
        TEST("LONGI ON");
        TEST("LDA #$1234",  0xA9, 0x34, 0x12);
        TEST("LDX #$1234",  0xA2, 0x34, 0x12);
        TEST("LDA #-1",     0xA9, 0xFF, 0xFF);
        TEST("LDX #-32768", 0xA2, 0x00, 0x80);
        ERRT("LDA #$12345", OVERFLOW_RANGE, "#$12345", 0xA9, 0x45, 0x23);
        ERRT("LDX #$12345", OVERFLOW_RANGE, "#$12345", 0xA2, 0x45, 0x23);

        TEST("LONGA OFF");
        TEST("LONGI ON");
        ERRT("LDA #$1234", OVERFLOW_RANGE, "#$1234", 0xA9, 0x34);
        TEST("LDX #$1234", 0xA2, 0x34, 0x12);

        TEST("LONGA ON");
        TEST("LONGI OFF");
        TEST("LDA #$1234", 0xA9, 0x34, 0x12);
        ERRT("LDX #$1234", OVERFLOW_RANGE, "#$1234", 0xA2, 0x34);

        TEST("LONGA OFF");
        TEST("LONGI OFF");
        ERRT("LDA #$1234", OVERFLOW_RANGE, "#$1234", 0xA9, 0x34);
        ERRT("LDX #$1234", OVERFLOW_RANGE, "#$1234", 0xA2, 0x34);
    } else {
        TEST("LONGA OFF");
        TEST("LONGI OFF");
        ERRT("LONGA ON", OPERAND_NOT_ALLOWED, "ON");
        ERRT("LONGI ON", OPERAND_NOT_ALLOWED, "ON");
    }

    symtab.intern(0x0010, "zero10");
    symtab.intern(0x00FF, "zeroFF");
    symtab.intern(0x0090, "zero90");
    symtab.intern(0x1234, "HERE");

    // MOS6502
    TEST("LDX #zero10", 0xA2, 0x10);
    TEST("CPY #zeroFF", 0xC0, 0xFF);
    TEST("SBC #zero90", 0xE9, 0x90);
    TEST("LDA #<HERE",  0xA9, 0x34);
    TEST("LDA #>HERE",  0xA9, 0x12);

    if (m6502()) {
        ERRT("BIT #zero90", OPERAND_NOT_ALLOWED, "#zero90");
    } else {
        // G65SC02
        TEST("BIT #zero90", 0x89, 0x90);
    }
    if (w65c816()) {
        TEST("REP #zero10", 0xC2, 0x10);
        TEST("SEP #zeroFF", 0xE2, 0xFF);
    }
}

static void test_zpg() {
    // MOS6502
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

    if (m6502()) {
        ERUI("TSB $10");
        ERUI("TRB $10");
        ERUI("STZ $10");
    } else {
        // G65SC02
        TEST("TSB $10", 0x04, 0x10);
        TEST("TRB $10", 0x14, 0x10);
        TEST("STZ $10", 0x64, 0x10);
    }

    symtab.intern(0x0010, "zero10");
    symtab.intern(0x00FF, "zeroFF");
    symtab.intern(0x0090, "zero90");

    // MOS6502
    TEST("SBC zero10", 0xE5, 0x10);
    TEST("DEC zeroFF", 0xC6, 0xFF);
    TEST("ROR zero90", 0x66, 0x90);

    if (m6502()) {
        ERUI("STZ zero10");
    } else {
        // G65SC02
        TEST("STZ zero10", 0x64, 0x10);
    }
}

static void test_zpg_indexed() {
    // MOS6502
    TEST("ora $10,x", 0x15, 0x10);
    TEST("AND $10,X", 0x35, 0x10);
    TEST("EOR $10,X", 0x55, 0x10);
    TEST("ADC $10,X", 0x75, 0x10);
    TEST("STA $10,X", 0x95, 0x10);
    TEST("LDA $10,X", 0xB5, 0x10);
    TEST("CMP $10,X", 0xD5, 0x10);
    TEST("SBC $10,X", 0xF5, 0x10);

    TEST("STY $10,X", 0x94, 0x10);
    TEST("LDY $10,X", 0xB4, 0x10);
    TEST("stx $10,y", 0x96, 0x10);
    TEST("LDX $10,Y", 0xB6, 0x10);

    TEST("DEC $10,X", 0xD6, 0x10);
    TEST("INC $10,X", 0xF6, 0x10);

    TEST("ASL $10,X", 0x16, 0x10);
    TEST("ROL $10,X", 0x36, 0x10);
    TEST("LSR $10,X", 0x56, 0x10);
    TEST("ROR $10,X", 0x76, 0x10);

    if (m6502()) {
        ERRT("BIT $10,X", REGISTER_NOT_ALLOWED, "X");
        ERUI("STZ $10,X");
    } else {
        // G65SC02
        TEST("BIT $10,X", 0x34, 0x10);
        TEST("STZ $10,X", 0x74, 0x10);
    }

    symtab.intern(0x0010, "zero10");
    symtab.intern(0x00FF, "zeroFF");
    symtab.intern(0x0090, "zero90");

    // MOS6502
    TEST("SBC zero10,X", 0xF5, 0x10);
    TEST("STY zero90,X", 0x94, 0x90);
    TEST("LDY zeroFF,X", 0xB4, 0xFF);
    TEST("STX zero90,Y", 0x96, 0x90);
    TEST("LDX zeroFF,Y", 0xB6, 0xFF);
    TEST("ROR zero90,X", 0x76, 0x90);

    if (m6502()) {
        ERRT("BIT zero10,X", REGISTER_NOT_ALLOWED, "X");
    } else {
        // G65SC02
        TEST("BIT zero10,X", 0x34, 0x10);
    }
}

static void test_zpg_long() {
    if (w65c816()) {
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

        symtab.intern(0x10, "dir10");

        TEST("ORA  [dir10]",   0x07, 0x10);
        TEST("ORA  [dir10],Y", 0x17, 0x10);
        TEST("ORAL (dir10)",   0x07, 0x10);
        TEST("ORAL (dir10),Y", 0x17, 0x10);
    } else {
        ERRT("ORA [$10]",   OPERAND_NOT_ALLOWED, "[$10]");
        ERRT("ORA [$10],Y", REGISTER_NOT_ALLOWED, "Y");
        ERUI("ORAL ($10)");
    }
}

static void test_sp_rel() {
    if (w65c816()) {
        TEST("LDA ($10,S),Y", 0xB3, 0x10);
        TEST("STA ($10,S),Y", 0x93, 0x10);
    } else {
        ERRT("LDA ($10,S),Y", REGISTER_NOT_ALLOWED, "S),Y");
        ERRT("STA ($10,S),Y", REGISTER_NOT_ALLOWED, "S),Y");
    }
    if (w65c816()) {
        TEST("ora $10,s", 0x03, 0x10);
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
        TEST("CMP ($10,S),Y", 0xD3, 0x10);
        TEST("SBC ($10,S),Y", 0xF3, 0x10);
    } else {
        ERRT("ORA $10,S",     REGISTER_NOT_ALLOWED, "S");
        ERRT("ORA ($10,S),Y", REGISTER_NOT_ALLOWED, "S),Y");
    }
}

static void test_abs() {
    // MOS6502
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

    if (m6502()) {
        ERUI("TSB $1234");
        ERUI("TRB $1234");
        ERUI("STZ $1234");
    } else {
        // G65SC02
        TEST("TSB $1234", 0x0C, 0x34, 0x12);
        TEST("TRB $1234", 0x1C, 0x34, 0x12);
        TEST("STZ $1234", 0x9C, 0x34, 0x12);
    }
    if (w65c816()) {
        TEST("PEA $1234", 0xF4, 0x34, 0x12);
    } else {
        ERUI("PEA $1234");
    }

    symtab.intern(0x0010, "abs0010");
    symtab.intern(0x1234, "abs1234");
    symtab.intern(0x0100, "abs0100");

    // MOS6502
    TEST("SBC >abs0010", 0xED, 0x10, 0x00);
    TEST("LDX abs1234",  0xAE, 0x34, 0x12);
    TEST("JSR abs0100",  0x20, 0x00, 0x01);

    if (m6502()) {
        ERUI("TSB abs1234");
    } else {
        // G65SC02
        TEST("TSB abs1234", 0x0C, 0x34, 0x12);
    }
    if (w65c816()) {
        TEST("PEA abs0010", 0xF4, 0x10, 0x00);
    }
}

static void test_abs_long() {
    if (w65c816()) {
        TEST("ORA $123456", 0x0F, 0x56, 0x34, 0x12);
        TEST("AND $123456", 0x2F, 0x56, 0x34, 0x12);
        TEST("EOR $123456", 0x4F, 0x56, 0x34, 0x12);
        TEST("ADC $123456", 0x6F, 0x56, 0x34, 0x12);
        TEST("STA $123456", 0x8F, 0x56, 0x34, 0x12);
        TEST("LDA $123456", 0xAF, 0x56, 0x34, 0x12);
        TEST("CMP $123456", 0xCF, 0x56, 0x34, 0x12);
        TEST("SBC $123456", 0xEF, 0x56, 0x34, 0x12);

        TEST("ORA $123456,X", 0x1F, 0x56, 0x34, 0x12);
        TEST("AND $123456,X", 0x3F, 0x56, 0x34, 0x12);
        TEST("EOR $123456,X", 0x5F, 0x56, 0x34, 0x12);
        TEST("ADC $123456,X", 0x7F, 0x56, 0x34, 0x12);
        TEST("STA $123456,X", 0x9F, 0x56, 0x34, 0x12);
        TEST("LDA $123456,X", 0xBF, 0x56, 0x34, 0x12);
        TEST("CMP $123456,X", 0xDF, 0x56, 0x34, 0x12);
        TEST("SBC $123456,X", 0xFF, 0x56, 0x34, 0x12);

        TEST("JMP $123456", 0x5C, 0x56, 0x34, 0x12);
        TEST("JSL $123456", 0x22, 0x56, 0x34, 0x12);

        TEST("JMP  [$1234]", 0xDC, 0x34, 0x12);
        TEST("JMPL ($1234)", 0xDC, 0x34, 0x12);

        TEST("MVP $123456,$345678", 0x44, 0x34, 0x12);
        TEST("MVN $003456,$345678", 0x54, 0x34, 0x00);

        symtab.intern(0x1234, "sym1234");
        symtab.intern(0x123456, "long3456");
        symtab.intern(0x10,     "zero10");
        symtab.intern(0x123456, "bank12");
        symtab.intern(0x345678, "bank34");

        TEST("ORA long3456",    0x0F, 0x56, 0x34, 0x12);
        TEST("ORA >>sym1234,X", 0x1F, 0x34, 0x12, 0x00);
        TEST("ORA long3456,X",  0x1F, 0x56, 0x34, 0x12);
        TEST("JMP >>sym1234",   0x5C, 0x34, 0x12, 0x00);
        TEST("JSL long3456",    0x22, 0x56, 0x34, 0x12);

        TEST("JMP  [sym1234]",  0xDC, 0x34, 0x12);
        TEST("JMPL (sym1234)",  0xDC, 0x34, 0x12);

        TEST("MVP bank12,bank34", 0x44, 0x34, 0x12);
        TEST("MVN zero10,bank34", 0x54, 0x34, 0x00);
    } else {
        ERRT("ORA $123456",   OPERAND_NOT_ALLOWED,  "$123456");
        ERRT("ORA $123456,X", REGISTER_NOT_ALLOWED, "X");
        ERRT("JMP $123456",   OPERAND_NOT_ALLOWED,  "$123456");
        ERUI("JSL $123456");
        ERRT("JMP  [$1234]",  OPERAND_NOT_ALLOWED,  "[$1234]");
        ERUI("JMPL ($1234)");
        ERUI("MVP $123456,$345678");
        ERUI("MVN $003456,$345678");
    }
}

static void test_abs_indexed() {
    // MOS6502
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

    if (m6502()) {
        ERRT("BIT $1234,X", REGISTER_NOT_ALLOWED, "X");
        ERUI("STZ $1234,X");
    } else {
        // G65SC02
        TEST("BIT $1234,X", 0x3C, 0x34, 0x12);
        TEST("STZ $1234,X", 0x9E, 0x34, 0x12);
    }

    symtab.intern(0x0010, "abs0010");
    symtab.intern(0x1234, "abs1234");
    symtab.intern(0x0100, "abs0100");

    // MOS6502
    TEST("SBC >abs0010,X", 0xFD, 0x10, 0x00);
    TEST("STA abs1234,Y",  0x99, 0x34, 0x12);
    TEST("LDY abs0100,X",  0xBC, 0x00, 0x01);
    TEST("LDX abs1234,Y",  0xBE, 0x34, 0x12);
    TEST("LSR abs0100,X",  0x5E, 0x00, 0x01);

    if (m6502()) {
        ERRT("BIT abs1234,X", REGISTER_NOT_ALLOWED, "X");
        ERUI("STZ abs1234,X");
    } else {
        // G65SC02
        TEST("BIT abs1234,X",  0x3C, 0x34, 0x12);
        TEST("STZ abs1234,X",  0x9E, 0x34, 0x12);
    }
}

static void test_abs_idir() {
    symtab.intern(0x0010, "abs0010");
    symtab.intern(0x1234, "abs1234");

    // MOS6502
    TEST("JMP (>$0009)",   0x6C, 0x09, 0x00);
    TEST("JMP ($1234)",    0x6C, 0x34, 0x12);
    TEST("JMP (>abs0010)", 0x6C, 0x10, 0x00);
    TEST("JMP (abs1234)",  0x6C, 0x34, 0x12);

}

static void test_abs_indexed_idir() {
    symtab.intern(0x0010, "abs0010");
    symtab.intern(0x1234, "abs1234");

    if (m6502()) {
        ERRT("JMP (>$0009,X)",   REGISTER_NOT_ALLOWED, "X)");
        ERRT("JMP ($1235,X)",    REGISTER_NOT_ALLOWED, "X)");
        ERRT("JMP (>abs0010,X)", REGISTER_NOT_ALLOWED, "X)");
        ERRT("JMP (>abs1234,X)", REGISTER_NOT_ALLOWED, "X)");
    } else {
        // G65SC02
        TEST("JMP (>$0009,X)",   0x7C, 0x09, 0x00);
        TEST("JMP ($1235,X)",    0x7C, 0x35, 0x12);
        TEST("JMP (>abs0010,X)", 0x7C, 0x10, 0x00);
        TEST("JMP (>abs1234,X)", 0x7C, 0x34, 0x12);
    }

    if (w65c816()) {
        TEST("JSR (>$0009,X)",   0xFC, 0x09, 0x00);
        TEST("JSR ($1235,X)",    0xFC, 0x35, 0x12);
        TEST("JSR (>abs0010,X)", 0xFC, 0x10, 0x00);
        TEST("JSR (>abs1234,X)", 0xFC, 0x34, 0x12);
    } else {
        ERRT("JSR (>$0009,X)",   REGISTER_NOT_ALLOWED, "X)");
        ERRT("JSR ($1235,X)",    REGISTER_NOT_ALLOWED, "X)");
        ERRT("JSR (>abs0010,X)", REGISTER_NOT_ALLOWED, "X)");
        ERRT("JSR (>abs1234,X)", REGISTER_NOT_ALLOWED, "X)");
    }
}

static void test_zpg_idir() {
    if (m6502()) {
        ERRT("ORA ($10)", OPERAND_NOT_ALLOWED, "($10)");
        ERRT("AND ($10)", OPERAND_NOT_ALLOWED, "($10)");
        ERRT("EOR ($10)", OPERAND_NOT_ALLOWED, "($10)");
        ERRT("ADC ($10)", OPERAND_NOT_ALLOWED, "($10)");
        ERRT("STA ($10)", OPERAND_NOT_ALLOWED, "($10)");
        ERRT("LDA ($10)", OPERAND_NOT_ALLOWED, "($10)");
        ERRT("CMP ($10)", OPERAND_NOT_ALLOWED, "($10)");
        ERRT("SBC ($10)", OPERAND_NOT_ALLOWED, "($10)");
    } else {
        // G65SC02
        TEST("ORA ($10)", 0x12, 0x10);
        TEST("AND ($10)", 0x32, 0x10);
        TEST("EOR ($10)", 0x52, 0x10);
        TEST("ADC ($10)", 0x72, 0x10);
        TEST("STA ($10)", 0x92, 0x10);
        TEST("LDA ($10)", 0xB2, 0x10);
        TEST("CMP ($10)", 0xD2, 0x10);
        TEST("SBC ($10)", 0xF2, 0x10);
    }
    if (w65c816()) {
        TEST("PEI ($10)", 0xD4, 0x10);
    } else {
        ERUI("PEI ($10)");
    }
}

static void test_zpg_indexed_idir() {
    // MOS6502
    TEST("ORA ($10,X)", 0x01, 0x10);
    TEST("AND ($10,X)", 0x21, 0x10);
    TEST("EOR ($10,X)", 0x41, 0x10);
    TEST("ADC ($10,X)", 0x61, 0x10);
    TEST("STA ($10,X)", 0x81, 0x10);
    TEST("LDA ($10,X)", 0xA1, 0x10);
    TEST("CMP ($10,X)", 0xC1, 0x10);
    TEST("SBC ($10,X)", 0xE1, 0x10);

    symtab.intern(0x0010, "zero10");

    // MOS6502
    TEST("LDA (zero10,X)", 0xA1, 0x10);
}

static void test_zpg_idir_indexed() {
    // MOS6502
    TEST("ORA ($10),Y", 0x11, 0x10);
    TEST("AND ($10),Y", 0x31, 0x10);
    TEST("EOR ($10),Y", 0x51, 0x10);
    TEST("ADC ($10),Y", 0x71, 0x10);
    TEST("STA ($10),Y", 0x91, 0x10);
    TEST("LDA ($10),Y", 0xB1, 0x10);
    TEST("CMP ($10),Y", 0xD1, 0x10);
    TEST("SBC ($10),Y", 0xF1, 0x10);

    symtab.intern(0x0010, "zero10");

    // MOS6502
    TEST("LDA (zero10),Y", 0xB1, 0x10);
}

static void test_rel() {
    // MOS6502
    ATEST(0x1000, "BPL $1002", 0x10, 0x00);
    ATEST(0x1000, "BMI $1000", 0x30, 0xFE);
    ATEST(0x1000, "BVC $1004", 0x50, 0x02);
    ATEST(0x1000, "BVS $1081", 0x70, 0x7F);
    ATEST(0x1000, "BCC $0F82", 0x90, 0x80);

    if (m6502()) {
        ERUI("BRA $1000");
    } else {
        // G65SC02
        ATEST(0x1000, "BRA $1002", 0x80, 0x00);
    }

    if (w65c816()) {
        AERRT(0x0000, "BCS $FF82", OPERAND_TOO_FAR, "$FF82", 0xB0, 0x80);
        AERRT(0xFFFE, "BNE $0000", OPERAND_TOO_FAR, "$0000", 0xD0, 0x00);
        AERRT(0xFFF0, "BEQ $0071", OPERAND_TOO_FAR, "$0071", 0xF0, 0x7F);
        AERRT(0x0000, "BCS *-126", OVERFLOW_RANGE,  "*-126", 0xB0, 0x80);
        AERRT(0xFFFE, "BNE *+2",   OVERFLOW_RANGE,  "*+2",   0xD0, 0x00);
        AERRT(0xFFF0, "BEQ *+129", OVERFLOW_RANGE,  "*+129", 0xF0, 0x7F);
    } else {
        AERRT(0x0000, "BCS $FF82", OVERFLOW_RANGE, "$FF82", 0xB0, 0x80);
        AERRT(0xFFFE, "BNE $0000", OVERFLOW_RANGE, "$0000", 0xD0, 0x00);
        AERRT(0xFFF0, "BEQ $0071", OVERFLOW_RANGE, "$0071", 0xF0, 0x7F);
        AERRT(0x0000, "BCS *-126", OVERFLOW_RANGE, "*-126", 0xB0, 0x80);
        AERRT(0xFFFE, "BNE *+2",   OVERFLOW_RANGE, "*+2",   0xD0, 0x00);
        AERRT(0xFFF0, "BEQ *+129", OVERFLOW_RANGE, "*+129", 0xF0, 0x7F);
    }

    if (w65c816()) {
        AERRT(0x120000, "BCS $12FF82", OPERAND_TOO_FAR, "$12FF82", 0xB0, 0x80);
        AERRT(0x12FFFE, "BNE $120000", OPERAND_TOO_FAR, "$120000", 0xD0, 0x00);
        AERRT(0x12FFF0, "BEQ $120071", OPERAND_TOO_FAR, "$120071", 0xF0, 0x7F);
        AERRT(0x120000, "BCS *-126",   OVERFLOW_RANGE,  "*-126",   0xB0, 0x80);
        AERRT(0x12FFFE, "BNE *+2",     OVERFLOW_RANGE,  "*+2",     0xD0, 0x00);
        AERRT(0x12FFF0, "BEQ *+129",   OVERFLOW_RANGE,  "*+129",   0xF0, 0x7F);

        AERRT(0x120000, "BCS $11FF82", OVERFLOW_RANGE,  "$11FF82", 0xB0, 0x80);
        AERRT(0x12FFFE, "BCS $130000", OVERFLOW_RANGE,  "$130000", 0xB0, 0x00);
        AERRT(0x12FFF0, "BCS $130061", OVERFLOW_RANGE,  "$130061", 0xB0, 0x6F);
    }

    if (w65c816()) {
        ATEST(0x129000, "BRL $121003", 0x82, 0x00, 0x80);
        ATEST(0x121000, "PER $129002", 0x62, 0xFF, 0x7F);
        ATEST(0x129000, "BRL *-$7FFD", 0x82, 0x00, 0x80);
        ATEST(0x121000, "PER *+$8002", 0x62, 0xFF, 0x7F);
        AERRT(0x129000, "BRL $131003", OVERFLOW_RANGE, "$131003", 0x82, 0x00, 0x80);
        AERRT(0x121000, "PER $118002", OVERFLOW_RANGE, "$118002", 0x62, 0xFF, 0x6F);

        ATEST(0x121000, "BRL $129003", 0x82, 0x00, 0x80);
        ATEST(0x129000, "PER $121002", 0x62, 0xFF, 0x7F);
        AERRT(0x121000, "BRL *-$7FFD", OVERFLOW_RANGE, "*-$7FFD", 0x82, 0x00, 0x80);
        AERRT(0x129000, "PER *+$8002", OVERFLOW_RANGE, "*+$8002", 0x62, 0xFF, 0x7F);
    } else {
        ERUI("BRL $1234");
        ERUI("PER $1234");
    }

    symtab.intern(0x0F82, "label0F82");
    symtab.intern(0x1000, "label1000");
    symtab.intern(0x1081, "label1081");

    // MOS6502
    ATEST(0x1000, "BMI label1000", 0x30, 0xFE);
    ATEST(0x1000, "BVS label1081", 0x70, 0x7F);
    ATEST(0x1000, "BCC label0F82", 0x90, 0x80);

    if (m6502()) {
        ERUI("BRA label1000");
    } else {
        // G65SC02
        ATEST(0x1000, "BRA label1000", 0x80, 0xFE);
    }

    TEST("option smart-branch, on");
    AERRT(0x1000, "BPL $1082", OPERAND_TOO_FAR, "$1082", 0x10, 0x80);
    AERRT(0x1000, "BMI $1082", OPERAND_TOO_FAR, "$1082", 0x30, 0x80);
    AERRT(0x1000, "BVC $1082", OPERAND_TOO_FAR, "$1082", 0x50, 0x80);
    AERRT(0x1000, "BVS $1082", OPERAND_TOO_FAR, "$1082", 0x70, 0x80);
    AERRT(0x1000, "BCC $0F81", OPERAND_TOO_FAR, "$0F81", 0x90, 0x7F);
    AERRT(0x1000, "BCS $0F81", OPERAND_TOO_FAR, "$0F81", 0xB0, 0x7F);
    AERRT(0x1000, "BNE $0F81", OPERAND_TOO_FAR, "$0F81", 0xD0, 0x7F);
    AERRT(0x1000, "BEQ $0F81", OPERAND_TOO_FAR, "$0F81", 0xF0, 0x7F);
    AERRT(0x0000, "BCS *-126", OVERFLOW_RANGE,  "*-126", 0xB0, 0x80);
    AERRT(0xFFFE, "BNE *+2",   OVERFLOW_RANGE,  "*+2",   0xD0, 0x00);
    if (w65c816()) {
        ATEST(0x1000, "BRA $1081", 0x80, 0x7F);
        ATEST(0x1000, "BRA $1082", 0x82, 0x7F, 0x00);
        ATEST(0x1000, "BRA $0F82", 0x80, 0x80);
        ATEST(0x1000, "BRA $0F81", 0x82, 0x7E, 0xFF);
        ATEST(0x1000, "BRL $1081", 0x80, 0x7F);
        ATEST(0x1000, "BRL $1082", 0x82, 0x7F, 0x00);
        ATEST(0x1000, "BRL $0F82", 0x80, 0x80);
        ATEST(0x1000, "BRL $0F81", 0x82, 0x7E, 0xFF);
        AERRT(0x0000, "BRA *-126", OVERFLOW_RANGE,  "*-126", 0x80, 0x80);
        AERRT(0xFFFE, "BRA *+2",   OVERFLOW_RANGE,  "*+2",   0x80, 0x00);
        AERRT(0x129000, "BRL $131003", OVERFLOW_RANGE, "$131003", 0x82, 0x00, 0x80);
        AERRT(0x121000, "BRL $118002", OVERFLOW_RANGE, "$118002", 0x82, 0xFF, 0x6F);
    } else if (!m6502()) {
        // G65SC02
        ATEST(0x1000, "BRA $1081", 0x80, 0x7F);
        AERRT(0x1000, "BRA $1082", OPERAND_TOO_FAR, "$1082", 0x80, 0x80);
        ATEST(0x1000, "BRA $0F82", 0x80, 0x80);
        AERRT(0x1000, "BRA $0F81", OPERAND_TOO_FAR, "$0F81", 0x80, 0x7F);
        AERRT(0x0000, "BRA *-126", OVERFLOW_RANGE,  "*-126", 0x80, 0x80);
        AERRT(0xFFFE, "BRA *+2",   OVERFLOW_RANGE,  "*+2",   0x80, 0x00);
    }
}

static void test_bitop() {
    if (r65c02() || w65c02s()) {
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
    } else {
        ERUI("RMB0 $10");
        ERUI("RMB1 $10");
        ERUI("RMB2 $10");
        ERUI("RMB3 $10");
        ERUI("RMB4 $10");
        ERUI("RMB5 $10");
        ERUI("RMB6 $10");
        ERUI("RMB7 $10");
        ERUI("SMB0 $10");
        ERUI("SMB1 $10");
        ERUI("SMB2 $10");
        ERUI("SMB3 $10");
        ERUI("SMB4 $10");
        ERUI("SMB5 $10");
        ERUI("SMB6 $10");
        ERUI("SMB7 $10");
    }
}

static void test_zpg_rel() {
    if (r65c02() || w65c02s()) {
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
    } else {
        ERUI("BBR0 $10,$1003");
        ERUI("BBR1 $10,$1000");
        ERUI("BBR2 $10,$1006");
        ERUI("BBR3 $10,$1082");
        ERUI("BBR4 $10,$0F83");
        ERUI("BBR5 $10,$1003");
        ERUI("BBR6 $10,$1003");
        ERUI("BBR7 $10,$1003");
        ERUI("BBS0 $10,$1003");
        ERUI("BBS1 $10,$1000");
        ERUI("BBS2 $10,$1006");
        ERUI("BBS3 $10,$1082");
        ERUI("BBS4 $10,$0F83");
        ERUI("BBS5 $10,$1003");
        ERUI("BBS6 $10,$1003");
        ERUI("BBS7 $10,$1003");
    }
}

static void test_comment() {
    // MOS6502
    ERRT("BRK        ; comment", OK, "; comment", 0x00);
    ERRT("ASL A      ; comment", OK, "; comment", 0x0A);
    ERRT("CPY #+255  ; comment", OK, "; comment", 0xC0, 0xFF);
    ERRT("BIT $10    ; comment", OK, "; comment", 0x24, 0x10);
    ERRT("ORA $10,X  ; comment", OK, "; comment", 0x15, 0x10);
    ERRT("LDX $10,Y  ; comment", OK, "; comment", 0xB6, 0x10);
    ERRT("BIT $1234  ; comment", OK, "; comment", 0x2C, 0x34, 0x12);
    ERRT("ORA $1234,X; comment", OK, "; comment", 0x1D, 0x34, 0x12);
    ERRT("ORA $1234,Y; comment", OK, "; comment", 0x19, 0x34, 0x12);
    ERRT("JMP ($1234); comment", OK, "; comment", 0x6C, 0x34, 0x12);
    ERRT("ORA ($10,X); comment", OK, "; comment", 0x01, 0x10);
    ERRT("ORA ($10),Y; comment", OK, "; comment", 0x11, 0x10);
    AERRT(0x1000, "BPL $1002; comment", OK, "; comment", 0x10, 0x00);

    ERRT("LDX $10 , Y     ; comment", OK, "; comment", 0xB6, 0x10);
    ERRT("ORA $1234 , X   ; comment", OK, "; comment", 0x1D, 0x34, 0x12);
    ERRT("JMP ( $1234 )   ; comment", OK, "; comment", 0x6C, 0x34, 0x12);
    ERRT("ORA ( $10 , X ) ; comment", OK, "; comment", 0x01, 0x10);
    ERRT("ORA ( $10 ) , Y ; comment", OK, "; comment", 0x11, 0x10);
}

static void test_undef() {
    // MOS6502
    ERUS("LDA #UNDEF", "UNDEF", 0xA9, 0x00);
    ERUS("JSR  UNDEF", "UNDEF", 0x20, 0x00, 0x00);
    ERUS("JSR >UNDEF", "UNDEF", 0x20, 0x00, 0x00);
    ERUS("BIT  UNDEF", "UNDEF", 0x24, 0x00);
    ERUS("BIT <UNDEF", "UNDEF", 0x24, 0x00);
    ERUS("BIT >UNDEF", "UNDEF", 0x2C, 0x00, 0x00);
    ERUS("LDY  UNDEF,X", "UNDEF,X", 0xB4, 0x00);
    ERUS("LDY <UNDEF,X", "UNDEF,X", 0xB4, 0x00);
    ERUS("LDY >UNDEF,X", "UNDEF,X", 0xBC, 0x00, 0x00);
    ERUS("LDX  UNDEF,Y", "UNDEF,Y", 0xB6, 0x00);
    ERUS("LDX <UNDEF,Y", "UNDEF,Y", 0xB6, 0x00);
    ERUS("LDX >UNDEF,Y", "UNDEF,Y", 0xBE, 0x00, 0x00);
    ERUS("LDA  (UNDEF,X)", "UNDEF,X)", 0xA1, 0x00);
    ERUS("LDA (<UNDEF,X)", "UNDEF,X)", 0xA1, 0x00);
    ERUS("LDA  (UNDEF),Y", "UNDEF),Y", 0xB1, 0x00);
    ERUS("LDA (<UNDEF),Y", "UNDEF),Y", 0xB1, 0x00);
    ERUS("JMP  (UNDEF)",   "UNDEF)",   0x6C, 0x00, 0x00);

    AERUS(0x1000, "BCC UNDEF", "UNDEF", 0x90, 0x00);

    if (!m6502()) {
        // G65SC02
        ERUS("BIT #UNDEF",   "UNDEF",   0x89, 0x00);
        ERUS("BIT  UNDEF,X", "UNDEF,X", 0x34, 0x00);
        ERUS("BIT <UNDEF,X", "UNDEF,X", 0x34, 0x00);
        ERUS("BIT >UNDEF,X", "UNDEF,X", 0x3C, 0x00, 0x00);
        ERUS("LDA  (UNDEF)", "UNDEF)", 0xB2, 0x00);
        ERUS("LDA (<UNDEF)", "UNDEF)", 0xB2, 0x00);
        ERUS("JMP  (UNDEF,X)", "UNDEF,X)", 0x7C, 0x00, 0x00);
        ERUS("JMP (>UNDEF,X)", "UNDEF,X)", 0x7C, 0x00, 0x00);

        AERUS(0x1000, "BRA UNDEF", "UNDEF", 0x80, 0x00);
    }

    if (r65c02() || w65c02s()) {
        // R65C02
        ERUS("RMB5  UNDEF", "UNDEF", 0x57, 0x00);
        ERUS("SMB4 <UNDEF", "UNDEF", 0xC7, 0x00);

        AERUS(0x1000, "BBR3 UNDEF,$1082", "UNDEF,$1082", 0x3F, 0x00, 0x7F);
        AERUS(0x1000, "BBS6 $10,UNDEF",   "UNDEF",       0xEF, 0x10, 0x00);
        AERUS(0x1000, "BBS7 UNDEF,UNDEF", "UNDEF,UNDEF", 0xFF, 0x00, 0x00);
    }
    if (w65c816()) {
        ERUS("SEP #UNDEF",        "UNDEF",    0xE2, 0x00);
        ERUS("MVP $123456,UNDEF", "UNDEF",    0x44, 0x00, 0x12);
        ERUS("MVP UNDEF,$123456", "UNDEF,$123456", 0x44, 0x12, 0x00);
        ERUS("MVN UNDEF,UNDEF",   "UNDEF,UNDEF",   0x54, 0x00, 0x00);
        ERUS("ORA [UNDEF]",       "UNDEF]",   0x07, 0x00);
        ERUS("AND [UNDEF],Y",     "UNDEF],Y", 0x37, 0x00);
        ERUS("EOR UNDEF,S",       "UNDEF,S",  0x43, 0x00);
        ERUS("ADC (UNDEF,S),Y",   "UNDEF,S),Y", 0x73, 0x00);
        ERUS("PEA UNDEF",         "UNDEF",    0xF4, 0x00, 0x00);
        ERUS("PEI (UNDEF)",       "UNDEF)",   0xD4, 0x00);
        ERUS("STA   UNDEF",       "UNDEF",    0x85, 0x00);
        ERUS("STA  <UNDEF",       "UNDEF",    0x85, 0x00);
        ERUS("STA  >UNDEF",       "UNDEF",    0x8D, 0x00, 0x00);
        ERUS("STA >>UNDEF",       "UNDEF",    0x8F, 0x00, 0x00, 0x00);
        ERUS("LDA   UNDEF,X",     "UNDEF,X",  0xB5, 0x00);
        ERUS("LDA  <UNDEF,X",     "UNDEF,X",  0xB5, 0x00);
        ERUS("LDA  >UNDEF,X",     "UNDEF,X",  0xBD, 0x00, 0x00);
        ERUS("LDA >>UNDEF,X",     "UNDEF,X",  0xBF, 0x00, 0x00, 0x00);
        ERUS("JMP [UNDEF]",       "UNDEF]",   0xDC, 0x00, 0x00);
        ERUS("JSR (UNDEF,X)",     "UNDEF,X)", 0xFC, 0x00, 0x00);

        ERUS("SBCL (UNDEF)",   "UNDEF)",   0xE7, 0x00);
        ERUS("CMPL (UNDEF),Y", "UNDEF),Y", 0xD7, 0x00);
        ERUS("JMPL (UNDEF)",   "UNDEF)",   0xDC, 0x00, 0x00);

        AERUS(0x121000, "BRA UNDEF", "UNDEF", 0x80, 0x00);
        AERUS(0x121000, "BRL UNDEF", "UNDEF", 0x82, 0x00, 0x00);
        AERUS(0x121000, "PER UNDEF", "UNDEF", 0x62, 0x00, 0x00);

        TEST("option smart-branch, on");
        AERUS(0x121000, "BRA UNDEF", "UNDEF", 0x82, 0x00, 0x00);
        AERUS(0x121000, "BRL UNDEF", "UNDEF", 0x82, 0x00, 0x00);
        AERUS(0x121000, "PER UNDEF", "UNDEF", 0x62, 0x00, 0x00);
    }
}

static void test_error() {
    if (w65c816()) {
        ERRT("SBC  [$10],X",   REGISTER_NOT_ALLOWED, "X");
        ERRT("ORAL ($10),X",   REGISTER_NOT_ALLOWED, "X");
        ERRT("ORA  ($10,S),X", REGISTER_NOT_ALLOWED, "X");
        ERRT("ORA  ($10,X),X", REGISTER_NOT_ALLOWED, "X");
        ERRT("ORA  ($10,X),Y", REGISTER_NOT_ALLOWED, "Y");
        ERRT("ORA  ($10,Y),X", REGISTER_NOT_ALLOWED, "X");
        ERRT("ORA  ($10,Y),Y", REGISTER_NOT_ALLOWED, "Y");
        ERRT("ORA  $123456,Y", REGISTER_NOT_ALLOWED, "Y");
        ERRT("ORA  [$10,$20]", OPERAND_NOT_ALLOWED,  "[$10,$20]");
    }
    ERRT("ORA ($10,$20)", OPERAND_NOT_ALLOWED,  "($10,$20)");
    ERRT("ORA ($10,Y)",   REGISTER_NOT_ALLOWED, "Y)");
    ERRT("ORA ($10),X",   REGISTER_NOT_ALLOWED, "X");
    if (w65c816()) {
        ERRT("JMP ($1234,Y)", REGISTER_NOT_ALLOWED, "Y)");
        ERRT("JSR ($1234,Y)", REGISTER_NOT_ALLOWED, "Y)");
    } else {
        ERRT("JSR ($1234,Y)", REGISTER_NOT_ALLOWED, "Y)");
        ERRT("JSR ($1234,X)", REGISTER_NOT_ALLOWED, "X)");
        ERRT("JMP ($1234,Y)", REGISTER_NOT_ALLOWED, "Y)");
    }
}

static void test_data_constant() {
    TEST(".BYTE -128, 255", 0x80, 0xFF);
    TEST(".BYTE 'A', '\"'", 0x41, 0x22);
    TEST(".BYTE '9'-'0'",   0x09);
    TEST(".BYTE ''''",      0x27);
    TEST(".BYTE '''",       0x27);
    TEST(".BYTE ''");
    ERRT(".BYTE '",         MISSING_CLOSING_QUOTE, "'");
    TEST(".BYTE 'A''B',0",  0x41, 0x27, 0x42, 0x00);
    ERRT(".BYTE 'A''B,0",   MISSING_CLOSING_QUOTE, "'A''B,0");
    TEST(".WORD -128, 255", 0x80, 0xFF, 0xFF, 0x00);
    TEST(".WORD 'X'",       0x58, 0x00);
    TEST(".WORD 'X'+0",     0x58, 0x00);
    TEST(".WORD 'A''B'",    0x41, 0x27, 0x42, 0x00);
    ERRT(".WORD 'A''B",     MISSING_CLOSING_QUOTE, "'A''B");

    TEST("FCB -128, 255", 0x80, 0xFF);
    TEST("FCB 'A', '\"'", 0x41, 0x22);
    TEST("FCB '9'-'0'",   0x09);
    TEST("FCB ''''",      0x27);
    TEST("FCB '''",       0x27);
    TEST("FCB ''");
    ERRT("FCB '",         MISSING_CLOSING_QUOTE, "'");
    TEST("FCB \"\"");
    TEST("FCB \"\"\"\"",  0x22);
    TEST("FCB 'A''B',0",  0x41, 0x27, 0x42, 0x00);
    ERRT("FCB 'A''B,0",   MISSING_CLOSING_QUOTE, "'A''B,0");
    TEST("FDB -128, 255", 0x80, 0xFF, 0xFF, 0x00);

    ERRT(".BYTE '"
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

    ERRT(".WORD "
         "$1234, $5678, $9ABC, $DEF0, $1234, $5678, $9ABC, $DEF0, "
         "$1234, $5678, $9ABC, $DEF0, $1234, $5678, $9ABC, $DEF0, "
         "$1234, $5678, $9ABC, $DEF0, $1234, $5678, $9ABC, $DEF0, "
         "$1234, $5678, $9ABC, $DEF0, $1234, $5678, $9ABC, $DE, "
         "$AAAA, $BBBB",
         NO_MEMORY,
         "$AAAA, $BBBB",
         0x34, 0x12, 0x78, 0x56, 0xBC, 0x9A, 0xF0, 0xDE, 0x34, 0x12, 0x78, 0x56, 0xBC, 0x9A, 0xF0, 0xDE,
         0x34, 0x12, 0x78, 0x56, 0xBC, 0x9A, 0xF0, 0xDE, 0x34, 0x12, 0x78, 0x56, 0xBC, 0x9A, 0xF0, 0xDE,
         0x34, 0x12, 0x78, 0x56, 0xBC, 0x9A, 0xF0, 0xDE, 0x34, 0x12, 0x78, 0x56, 0xBC, 0x9A, 0xF0, 0xDE,
         0x34, 0x12, 0x78, 0x56, 0xBC, 0x9A, 0xF0, 0xDE, 0x34, 0x12, 0x78, 0x56, 0xBC, 0x9A, 0xDE, 0x00);
}

// clang-format on

void run_tests(const char *cpu) {
    assembler.setCpu(cpu);
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
    RUN_TEST(test_comment);
    RUN_TEST(test_undef);
    RUN_TEST(test_error);
    RUN_TEST(test_data_constant);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

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

#include "asm_mc6800.h"
#include "test_asm_helper.h"

using namespace libasm;
using namespace libasm::mc6800;
using namespace libasm::test;

TestAsserter asserter;
TestSymtab symtab;
AsmMc6800 as6800;
Assembler &assembler(as6800);

static void set_up() {
}

static void tear_down() {
    symtab.reset();
}

static void test_cpu() {
    asserter.equals(
        "cpu 6800", true, assembler.setCpu("6800"));
}

static void test_inherent() {
    TEST("NOP", 0x01);
    TEST("TAP", 0x06);
    TEST("TPA", 0x07);
    TEST("INX", 0x08);
    TEST("DEX", 0x09);
    TEST("CLV", 0x0A);
    TEST("SEV", 0x0B);
    TEST("CLC", 0x0C);
    TEST("SEC", 0x0D);
    TEST("CLI", 0x0E);
    TEST("SEI", 0x0F);
    TEST("SBA", 0x10);
    TEST("CBA", 0x11);
    TEST("TAB", 0x16);
    TEST("TBA", 0x17);
    TEST("DAA", 0x19);
    TEST("ABA", 0x1B);
    TEST("TSX", 0x30);
    TEST("INS", 0x31);
    TEST("DES", 0x34);
    TEST("TXS", 0x35);

    TEST("RTS", 0x39);
    TEST("RTI", 0x3B);
    TEST("WAI", 0x3E);
    TEST("SWI", 0x3F);

    TEST("PUL A", 0x32);
    TEST("PUL B", 0x33);
    TEST("PSH A", 0x36);
    TEST("PSH B", 0x37);
    TEST("PULA", 0x32);
    TEST("PULB", 0x33);
    TEST("PSHA", 0x36);
    TEST("PSHB", 0x37);

    TEST("NEG A", 0x40);
    TEST("COM A", 0x43);
    TEST("LSR A", 0x44);
    TEST("ROR A", 0x46);
    TEST("ASR A", 0x47);
    TEST("ASL A", 0x48);
    TEST("ROL A", 0x49);
    TEST("DEC A", 0x4A);
    TEST("INC A", 0x4C);
    TEST("TST A", 0x4D);
    TEST("CLR A", 0x4F);
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

    TEST("NEG B", 0x50);
    TEST("COM B", 0x53);
    TEST("LSR B", 0x54);
    TEST("ROR B", 0x56);
    TEST("ASR B", 0x57);
    TEST("ASL B", 0x58);
    TEST("ROL B", 0x59);
    TEST("DEC B", 0x5A);
    TEST("INC B", 0x5C);
    TEST("TST B", 0x5D);
    TEST("CLR B", 0x5F);
    TEST("NEGB", 0x50);
    TEST("COMB", 0x53);
    TEST("LSRB", 0x54);
    TEST("RORB", 0x56);
    TEST("ASRB", 0x57);
    TEST("ASLB", 0x58);
    TEST("ROLB", 0x59);
    TEST("DECB", 0x5A);
    TEST("INCB", 0x5C);
    TEST("TSTB", 0x5D);
    TEST("CLRB", 0x5F);
}

static void test_relative() {
    ATEST(0x1000, "BRA $1002", 0x20, 0x00);
    ATEST(0x1000, "BHI $1004", 0x22, 0x02);
    ATEST(0x1000, "BLS $1002", 0x23, 0x00);
    ATEST(0x1000, "BHS $1002", 0x24, 0x00);
    ATEST(0x1000, "BLO $1002", 0x25, 0x00);
    ATEST(0x1000, "BNE $1002", 0x26, 0x00);
    ATEST(0x1000, "BEQ $1002", 0x27, 0x00);
    ATEST(0x1000, "BVC $1002", 0x28, 0x00);
    ATEST(0x1000, "BVS $1002", 0x29, 0x00);
    ATEST(0x1000, "BPL $1002", 0x2A, 0x00);
    ATEST(0x1000, "BMI $1002", 0x2B, 0x00);
    ATEST(0x1000, "BGE $1002", 0x2C, 0x00);
    ATEST(0x1000, "BLT $1002", 0x2D, 0x00);
    ATEST(0x1000, "BGT $1002", 0x2E, 0x00);
    ATEST(0x1000, "BLE $1002", 0x2F, 0x00);

    ATEST(0x1000, "BSR  $1042", 0x8D, 0x40);

    symtab.intern(0x0F82, "sub0F82");
    symtab.intern(0x1081, "sub1081");
    symtab.intern(0x9002, "sub9002");
    symtab.intern(0x9003, "sub9003");

    ATEST(0x1000, "BSR  sub1081", 0x8D, 0x7F);
    ATEST(0x1000, "BSR  sub0F82", 0x8D, 0x80);
}

static void test_immediate() {
    TEST("SUB A #$90", 0x80, 0x90);
    TEST("CMP A #$90", 0x81, 0x90);
    TEST("SBC A #$90", 0x82, 0x90);
    TEST("AND A #$90", 0x84, 0x90);
    TEST("BIT A #$90", 0x85, 0x90);
    TEST("LDA A #$90", 0x86, 0x90);
    TEST("EOR A #$90", 0x88, 0x90);
    TEST("ADC A #$90", 0x89, 0x90);
    TEST("ORA A #$90", 0x8A, 0x90);
    TEST("ADD A #$90", 0x8B, 0x90);
    TEST("SUBA #$90", 0x80, 0x90);
    TEST("CMPA #$90", 0x81, 0x90);
    TEST("SBCA #$90", 0x82, 0x90);
    TEST("ANDA #$90", 0x84, 0x90);
    TEST("BITA #$90", 0x85, 0x90);
    TEST("LDAA #$90", 0x86, 0x90);
    TEST("EORA #$90", 0x88, 0x90);
    TEST("ADCA #$90", 0x89, 0x90);
    TEST("ORAA #$90", 0x8A, 0x90);
    TEST("ADDA #$90", 0x8B, 0x90);

    TEST("SUB B #$90", 0xC0, 0x90);
    TEST("CMP B #$90", 0xC1, 0x90);
    TEST("SBC B #$90", 0xC2, 0x90);
    TEST("AND B #$90", 0xC4, 0x90);
    TEST("BIT B #$90", 0xC5, 0x90);
    TEST("LDA B #$90", 0xC6, 0x90);
    TEST("EOR B #$90", 0xC8, 0x90);
    TEST("ADC B #$90", 0xC9, 0x90);
    TEST("ORA B #$90", 0xCA, 0x90);
    TEST("ADD B #$90", 0xCB, 0x90);
    TEST("SUBB #$90", 0xC0, 0x90);
    TEST("CMPB #$90", 0xC1, 0x90);
    TEST("SBCB #$90", 0xC2, 0x90);
    TEST("ANDB #$90", 0xC4, 0x90);
    TEST("BITB #$90", 0xC5, 0x90);
    TEST("LDAB #$90", 0xC6, 0x90);
    TEST("EORB #$90", 0xC8, 0x90);
    TEST("ADCB #$90", 0xC9, 0x90);
    TEST("ORAB #$90", 0xCA, 0x90);
    TEST("ADDB #$90", 0xCB, 0x90);

    TEST("CPX #$90A0", 0x8C, 0x90, 0xA0);
    TEST("LDX #$90A0", 0xCE, 0x90, 0xA0);
    TEST("LDS #$90A0", 0x8E, 0x90, 0xA0);

    symtab.intern(0x90, "dir90");
    symtab.intern(0x90A0, "dir90A0");

    TEST("LDA A #$90", 0x86, 0x90);
    TEST("LDAA #$90",  0x86, 0x90);
    TEST("CPX #dir90A0", 0x8C, 0x90, 0xA0);
    TEST("LDX #dir90A0", 0xCE, 0x90, 0xA0);
    TEST("LDS #dir90A0", 0x8E, 0x90, 0xA0);
}

static void test_direct() {
    TEST("SUB A $90", 0x90, 0x90);
    TEST("CMP A $90", 0x91, 0x90);
    TEST("SBC A $90", 0x92, 0x90);
    TEST("AND A $90", 0x94, 0x90);
    TEST("BIT A $90", 0x95, 0x90);
    TEST("LDA A $90", 0x96, 0x90);
    TEST("STA A $90", 0x97, 0x90);
    TEST("EOR A $90", 0x98, 0x90);
    TEST("ADC A $90", 0x99, 0x90);
    TEST("ORA A $90", 0x9A, 0x90);
    TEST("ADD A $90", 0x9B, 0x90);
    TEST("SUBA $90", 0x90, 0x90);
    TEST("CMPA $90", 0x91, 0x90);
    TEST("SBCA $90", 0x92, 0x90);
    TEST("ANDA $90", 0x94, 0x90);
    TEST("BITA $90", 0x95, 0x90);
    TEST("LDAA $90", 0x96, 0x90);
    TEST("STAA $90", 0x97, 0x90);
    TEST("EORA $90", 0x98, 0x90);
    TEST("ADCA $90", 0x99, 0x90);
    TEST("ORAA $90", 0x9A, 0x90);
    TEST("ADDA $90", 0x9B, 0x90);

    TEST("SUB B $90", 0xD0, 0x90);
    TEST("CMP B $90", 0xD1, 0x90);
    TEST("SBC B $90", 0xD2, 0x90);
    TEST("AND B $90", 0xD4, 0x90);
    TEST("BIT B $90", 0xD5, 0x90);
    TEST("LDA B $90", 0xD6, 0x90);
    TEST("STA B $90", 0xD7, 0x90);
    TEST("EOR B $90", 0xD8, 0x90);
    TEST("ADC B $90", 0xD9, 0x90);
    TEST("ORA B $90", 0xDA, 0x90);
    TEST("ADD B $90", 0xDB, 0x90);
    TEST("SUBB $90", 0xD0, 0x90);
    TEST("CMPB $90", 0xD1, 0x90);
    TEST("SBCB $90", 0xD2, 0x90);
    TEST("ANDB $90", 0xD4, 0x90);
    TEST("BITB $90", 0xD5, 0x90);
    TEST("LDAB $90", 0xD6, 0x90);
    TEST("STAB $90", 0xD7, 0x90);
    TEST("EORB $90", 0xD8, 0x90);
    TEST("ADCB $90", 0xD9, 0x90);
    TEST("ORAB $90", 0xDA, 0x90);
    TEST("ADDB $90", 0xDB, 0x90);

    TEST("CPX $90", 0x9C, 0x90);
    TEST("LDX $90", 0xDE, 0x90);
    TEST("STX $90", 0xDF, 0x90);
    TEST("LDS $90", 0x9E, 0x90);
    TEST("STS $90", 0x9F, 0x90);

    symtab.intern(0x10, "dir10");
    symtab.intern(0x22, "dir22");
    symtab.intern(0x90, "dir90");

    TEST("LDA A <dir90", 0x96, 0x90);
    TEST("STA B <dir90", 0xD7, 0x90);
    TEST("LDAA <dir90",  0x96, 0x90);
    TEST("STAB <dir90",  0xD7, 0x90);
    TEST("CPX <dir22", 0x9C, 0x22);
    TEST("LDX <dir22", 0xDE, 0x22);
    TEST("STX <dir22", 0xDF, 0x22);
    TEST("LDS <dir90", 0x9E, 0x90);
    TEST("STS <dir90", 0x9F, 0x90);
}

static void test_extended() {
    TEST("NEG >$0000", 0x70, 0x00, 0x00);
    TEST("COM >$0009", 0x73, 0x00, 0x09);
    TEST("LSR >$0034", 0x74, 0x00, 0x34);
    TEST("ROR $1234",  0x76, 0x12, 0x34);
    TEST("ASR $1234",  0x77, 0x12, 0x34);
    TEST("ASL $1234",  0x78, 0x12, 0x34);
    TEST("ROL $1234",  0x79, 0x12, 0x34);
    TEST("DEC $1234",  0x7A, 0x12, 0x34);
    TEST("INC $1234",  0x7C, 0x12, 0x34);
    TEST("TST $1234",  0x7D, 0x12, 0x34);
    TEST("CLR $1234",  0x7F, 0x12, 0x34);

    TEST("SUB A >$0090", 0xB0, 0x00, 0x90);
    TEST("CMP A >$0090", 0xB1, 0x00, 0x90);
    TEST("SBC A >$0090", 0xB2, 0x00, 0x90);
    TEST("AND A >$0090", 0xB4, 0x00, 0x90);
    TEST("BIT A >$0090", 0xB5, 0x00, 0x90);
    TEST("LDA A >$0090", 0xB6, 0x00, 0x90);
    TEST("STA A >$0090", 0xB7, 0x00, 0x90);
    TEST("EOR A >$0090", 0xB8, 0x00, 0x90);
    TEST("ADC A >$0090", 0xB9, 0x00, 0x90);
    TEST("ORA A >$0090", 0xBA, 0x00, 0x90);
    TEST("ADD A >$0090", 0xBB, 0x00, 0x90);
    TEST("SUBA >$0090", 0xB0, 0x00, 0x90);
    TEST("CMPA >$0090", 0xB1, 0x00, 0x90);
    TEST("SBCA >$0090", 0xB2, 0x00, 0x90);
    TEST("ANDA >$0090", 0xB4, 0x00, 0x90);
    TEST("BITA >$0090", 0xB5, 0x00, 0x90);
    TEST("LDAA >$0090", 0xB6, 0x00, 0x90);
    TEST("STAA >$0090", 0xB7, 0x00, 0x90);
    TEST("EORA >$0090", 0xB8, 0x00, 0x90);
    TEST("ADCA >$0090", 0xB9, 0x00, 0x90);
    TEST("ORAA >$0090", 0xBA, 0x00, 0x90);
    TEST("ADDA >$0090", 0xBB, 0x00, 0x90);

    TEST("SUB B $9ABC", 0xF0, 0x9A, 0xBC);
    TEST("CMP B $9ABC", 0xF1, 0x9A, 0xBC);
    TEST("SBC B $9ABC", 0xF2, 0x9A, 0xBC);
    TEST("AND B $9ABC", 0xF4, 0x9A, 0xBC);
    TEST("BIT B $9ABC", 0xF5, 0x9A, 0xBC);
    TEST("LDA B $9ABC", 0xF6, 0x9A, 0xBC);
    TEST("STA B $9ABC", 0xF7, 0x9A, 0xBC);
    TEST("EOR B $9ABC", 0xF8, 0x9A, 0xBC);
    TEST("ADC B $9ABC", 0xF9, 0x9A, 0xBC);
    TEST("ORA B $9ABC", 0xFA, 0x9A, 0xBC);
    TEST("ADD B $9ABC", 0xFB, 0x9A, 0xBC);
    TEST("SUBB $9ABC", 0xF0, 0x9A, 0xBC);
    TEST("CMPB $9ABC", 0xF1, 0x9A, 0xBC);
    TEST("SBCB $9ABC", 0xF2, 0x9A, 0xBC);
    TEST("ANDB $9ABC", 0xF4, 0x9A, 0xBC);
    TEST("BITB $9ABC", 0xF5, 0x9A, 0xBC);
    TEST("LDAB $9ABC", 0xF6, 0x9A, 0xBC);
    TEST("STAB $9ABC", 0xF7, 0x9A, 0xBC);
    TEST("EORB $9ABC", 0xF8, 0x9A, 0xBC);
    TEST("ADCB $9ABC", 0xF9, 0x9A, 0xBC);
    TEST("ORAB $9ABC", 0xFA, 0x9A, 0xBC);
    TEST("ADDB $9ABC", 0xFB, 0x9A, 0xBC);

    TEST("CPX $9ABC", 0xBC, 0x9A, 0xBC);
    TEST("LDX $9ABC", 0xFE, 0x9A, 0xBC);
    TEST("STX $9ABC", 0xFF, 0x9A, 0xBC);
    TEST("LDS $9ABC", 0xBE, 0x9A, 0xBC);
    TEST("STS $9ABC", 0xBF, 0x9A, 0xBC);

    TEST("JMP $0034", 0x7E, 0x00, 0x34);
    TEST("JSR $1234", 0xBD, 0x12, 0x34);

    symtab.intern(0x0090, "ext0090");
    symtab.intern(0x9ABC, "ext9ABC");

    TEST("NEG >ext0090",   0x70, 0x00, 0x90);
    TEST("LDA A ext9ABC",  0xB6, 0x9A, 0xBC);
    TEST("STA B >ext0090", 0xF7, 0x00, 0x90);
    TEST("LDAA  ext9ABC",  0xB6, 0x9A, 0xBC);
    TEST("STAB >ext0090",  0xF7, 0x00, 0x90);
    TEST("CPX ext9ABC",  0xBC, 0x9A, 0xBC);
    TEST("LDX ext9ABC",  0xFE, 0x9A, 0xBC);
    TEST("STX >ext0090", 0xFF, 0x00, 0x90);
    TEST("LDS ext9ABC",  0xBE, 0x9A, 0xBC);
    TEST("STS >ext0090", 0xBF, 0x00, 0x90);
    TEST("JMP ext9ABC",  0x7E, 0x9A, 0xBC);
    TEST("JSR ext0090",  0xBD, 0x00, 0x90);
}

static void test_indexed() {
    TEST("NEG ,X",  0x60, 0x00);
    TEST("COM 0,X", 0x63, 0x00);
    TEST("LSR 1,X", 0x64, 0x01);
    TEST("ROR 2,X", 0x66, 0x02);
    TEST("ASR 3,X", 0x67, 0x03);
    TEST("ASL 4,X", 0x68, 0x04);
    TEST("ROL 5,X", 0x69, 0x05);
    TEST("DEC 6,X", 0x6A, 0x06);
    TEST("INC 127,X", 0x6C, 0x7F);
    TEST("TST 128,X", 0x6D, 0x80);
    TEST("CLR 255,X", 0x6F, 0xFF);

    TEST("SUB A ,X",  0xA0, 0x00);
    TEST("CMP A 0,X", 0xA1, 0x00);
    TEST("SBC A 1,X", 0xA2, 0x01);
    TEST("AND A 2,X", 0xA4, 0x02);
    TEST("BIT A 3,X", 0xA5, 0x03);
    TEST("LDA A 4,X", 0xA6, 0x04);
    TEST("STA A 5,X", 0xA7, 0x05);
    TEST("EOR A 6,X", 0xA8, 0x06);
    TEST("ADC A 127,X", 0xA9, 0x7F);
    TEST("ORA A 128,X", 0xAA, 0x80);
    TEST("ADD A 255,X", 0xAB, 0xFF);
    TEST("SUBA ,X",  0xA0, 0x00);
    TEST("CMPA 0,X", 0xA1, 0x00);
    TEST("SBCA 1,X", 0xA2, 0x01);
    TEST("ANDA 2,X", 0xA4, 0x02);
    TEST("BITA 3,X", 0xA5, 0x03);
    TEST("LDAA 4,X", 0xA6, 0x04);
    TEST("STAA 5,X", 0xA7, 0x05);
    TEST("EORA 6,X", 0xA8, 0x06);
    TEST("ADCA 127,X", 0xA9, 0x7F);
    TEST("ORAA 128,X", 0xAA, 0x80);
    TEST("ADDA 255,X", 0xAB, 0xFF);

    TEST("SUB B ,X",  0xE0, 0x00);
    TEST("CMP B 0,X", 0xE1, 0x00);
    TEST("SBC B 1,X", 0xE2, 0x01);
    TEST("AND B 2,X", 0xE4, 0x02);
    TEST("BIT B 3,X", 0xE5, 0x03);
    TEST("LDA B 4,X", 0xE6, 0x04);
    TEST("STA B 5,X", 0xE7, 0x05);
    TEST("EOR B 6,X", 0xE8, 0x06);
    TEST("ADC B 127,X", 0xE9, 0x7F);
    TEST("ORA B 128,X", 0xEA, 0x80);
    TEST("ADD B 255,X", 0xEB, 0xFF);
    TEST("SUBB ,X",  0xE0, 0x00);
    TEST("CMPB 0,X", 0xE1, 0x00);
    TEST("SBCB 1,X", 0xE2, 0x01);
    TEST("ANDB 2,X", 0xE4, 0x02);
    TEST("BITB 3,X", 0xE5, 0x03);
    TEST("LDAB 4,X", 0xE6, 0x04);
    TEST("STAB 5,X", 0xE7, 0x05);
    TEST("EORB 6,X", 0xE8, 0x06);
    TEST("ADCB 127,X", 0xE9, 0x7F);
    TEST("ORAB 128,X", 0xEA, 0x80);
    TEST("ADDB 255,X", 0xEB, 0xFF);

    TEST("CPX ,X",  0xAC, 0x00);
    TEST("LDX 0,X", 0xEE, 0x00);
    TEST("STX 2,X", 0xEF, 0x02);
    TEST("LDS 128,X", 0xAE, 0x80);
    TEST("STS 255,X", 0xAF, 0xFF);

    TEST("JMP 0,X",   0x6E, 0x00);
    TEST("JSR 255,X", 0xAD, 0xFF);

    symtab.intern(0,   "offset0");
    symtab.intern(255, "offset255");

    TEST("NEG offset0,X",     0x60, 0x00);
    TEST("COM offset255,X",   0x63, 0xFF);
    TEST("CMP A offset0,X",   0xA1, 0x00);
    TEST("ADD B offset255,X", 0xEB, 0xFF);
    TEST("CMPA offset0,X",    0xA1, 0x00);
    TEST("ADDB offset255,X",  0xEB, 0xFF);
    TEST("JMP offset0,X",     0x6E, 0x00);
    TEST("JSR offset255,X",   0xAD, 0xFF);
}

static void test_comment() {
    symtab.intern(255,    "sym255");
    symtab.intern(0x1234, "sym1234");

    TEST("NOP       ; comment", 0x01);
    TEST("PSH A     ; comment", 0x36);
    TEST("PSHA      ; comment", 0x36);
    TEST("SUB A #$90; comment", 0x80, 0x90);
    TEST("SUBA  #$90; comment", 0x80, 0x90);
    TEST("NEG >$0010; comment", 0x70, 0x00, 0x10);
    TEST("SUB A >$90; comment", 0xB0, 0x00, 0x90);
    TEST("SUB A sym255;comment ", 0x90, 0xFF);
    TEST("SUB A >sym255;comment", 0xB0, 0x00, 0xFF);
    TEST("SUB A sym1234;comment", 0xB0, 0x12, 0x34);
    TEST("SUBA   >$90; comment", 0xB0, 0x00, 0x90);
    TEST("SUBA sym255;comment ", 0x90, 0xFF);
    TEST("SUBA >sym255;comment", 0xB0, 0x00, 0xFF);
    TEST("SUBA sym1234;comment", 0xB0, 0x12, 0x34);
    TEST("JMP sym255;comment ", 0x7E, 0x00, 0xFF);
    TEST("JSR sym1234;comment", 0xBD, 0x12, 0x34);
}

static void test_undefined_symbol() {
    ETEST(UNDEFINED_SYMBOL, "LDA  A #UNDEF", 0x86, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LDAA #UNDEF",   0x86, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LDS  #UNDEF",   0x8E, 0x00, 0x00);
    ETEST(UNDEFINED_SYMBOL, "NEG  UNDEF",    0x70, 0x00, 0x00);
    ETEST(UNDEFINED_SYMBOL, "SUB  A UNDEF",  0x90, 0x00);
    ETEST(UNDEFINED_SYMBOL, "SUB  A <UNDEF", 0x90, 0x00);
    ETEST(UNDEFINED_SYMBOL, "SUB  B >UNDEF", 0xF0, 0x00, 0x00);
    ETEST(UNDEFINED_SYMBOL, "SUBA UNDEF",  0x90, 0x00);
    ETEST(UNDEFINED_SYMBOL, "SUBA <UNDEF", 0x90, 0x00);
    ETEST(UNDEFINED_SYMBOL, "SUBB >UNDEF", 0xF0, 0x00, 0x00);
    ETEST(UNDEFINED_SYMBOL, "JMP  UNDEF",    0x7E, 0x00, 0x00);

    ETEST(UNDEFINED_SYMBOL, "LDA  UNDEF,X",    0xA6, 0x00);

    EATEST(UNDEFINED_SYMBOL, 0x1000, "BRA UNDEF", 0x20, 0xFE);
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
    RUN_TEST(test_inherent);
    RUN_TEST(test_immediate);
    RUN_TEST(test_direct);
    RUN_TEST(test_extended);
    RUN_TEST(test_indexed);
    RUN_TEST(test_relative);
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

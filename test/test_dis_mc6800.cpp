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

using namespace libasm;
using namespace libasm::mc6800;
using namespace libasm::test;

TestAsserter asserter;
TestMemory memory;
TestSymtab symtab;
DisMc6800 dis6800;
Disassembler &disassembler(dis6800);

static void set_up() {
    disassembler.setCpu("6800");
}

static void tear_down() {
    symtab.reset();
}

static void test_cpu() {
    asserter.equals(
        "cpu 6800", true, disassembler.setCpu("6800"));
    asserter.equals(
        "cpu 6801", true, disassembler.setCpu("6801"));
}

static void test_inherent() {
    TEST(NOP, "", 0x01);
    TEST(TAP, "", 0x06);
    TEST(TPA, "", 0x07);
    TEST(INX, "", 0x08);
    TEST(DEX, "", 0x09);
    TEST(CLV, "", 0x0A);
    TEST(SEV, "", 0x0B);
    TEST(CLC, "", 0x0C);
    TEST(SEC, "", 0x0D);
    TEST(CLI, "", 0x0E);
    TEST(SEI, "", 0x0F);
    TEST(SBA, "", 0x10);
    TEST(CBA, "", 0x11);
    TEST(TAB, "", 0x16);
    TEST(TBA, "", 0x17);
    TEST(DAA, "", 0x19);
    TEST(ABA, "", 0x1B);
    TEST(TSX, "", 0x30);
    TEST(INS, "", 0x31);
    TEST(DES, "", 0x34);
    TEST(TXS, "", 0x35);

    TEST(RTS, "", 0x39);
    TEST(RTI, "", 0x3B);
    TEST(WAI, "", 0x3E);
    TEST(SWI, "", 0x3F);

    TEST(PUL, "A", 0x32);
    TEST(PUL, "B", 0x33);
    TEST(PSH, "A", 0x36);
    TEST(PSH, "B", 0x37);

    TEST(NEG, "A", 0x40);
    TEST(COM, "A", 0x43);
    TEST(LSR, "A", 0x44);
    TEST(ROR, "A", 0x46);
    TEST(ASR, "A", 0x47);
    TEST(ASL, "A", 0x48);
    TEST(ROL, "A", 0x49);
    TEST(DEC, "A", 0x4A);
    TEST(INC, "A", 0x4C);
    TEST(TST, "A", 0x4D);
    TEST(CLR, "A", 0x4F);

    TEST(NEG, "B", 0x50);
    TEST(COM, "B", 0x53);
    TEST(LSR, "B", 0x54);
    TEST(ROR, "B", 0x56);
    TEST(ASR, "B", 0x57);
    TEST(ASL, "B", 0x58);
    TEST(ROL, "B", 0x59);
    TEST(DEC, "B", 0x5A);
    TEST(INC, "B", 0x5C);
    TEST(TST, "B", 0x5D);
    TEST(CLR, "B", 0x5F);

    // MC6801
    disassembler.setCpu("6801");
    TEST(LSRD, "", 0x04);
    TEST(ASLD, "", 0x05);
    TEST(PULX, "", 0x38);
    TEST(ABX,  "", 0x3A);
    TEST(PSHX, "", 0x3C);
    TEST(MUL,  "", 0x3D);
}

static void test_immediate() {
    TEST(SUB, "A #$90", 0x80, 0x90);
    TEST(CMP, "A #$90", 0x81, 0x90);
    TEST(SBC, "A #$90", 0x82, 0x90);
    TEST(AND, "A #$90", 0x84, 0x90);
    TEST(BIT, "A #$90", 0x85, 0x90);
    TEST(LDA, "A #$90", 0x86, 0x90);
    TEST(EOR, "A #$90", 0x88, 0x90);
    TEST(ADC, "A #$90", 0x89, 0x90);
    TEST(ORA, "A #$90", 0x8A, 0x90);
    TEST(ADD, "A #$90", 0x8B, 0x90);

    TEST(SUB, "B #$90", 0xC0, 0x90);
    TEST(CMP, "B #$90", 0xC1, 0x90);
    TEST(SBC, "B #$90", 0xC2, 0x90);
    TEST(AND, "B #$90", 0xC4, 0x90);
    TEST(BIT, "B #$90", 0xC5, 0x90);
    TEST(LDA, "B #$90", 0xC6, 0x90);
    TEST(EOR, "B #$90", 0xC8, 0x90);
    TEST(ADC, "B #$90", 0xC9, 0x90);
    TEST(ORA, "B #$90", 0xCA, 0x90);
    TEST(ADD, "B #$90", 0xCB, 0x90);

    TEST(CPX, "#$90A0", 0x8C, 0x90, 0xA0);
    TEST(LDX, "#$90A0", 0xCE, 0x90, 0xA0);
    TEST(LDS, "#$90A0", 0x8E, 0x90, 0xA0);

    // MC6801
    disassembler.setCpu("6801");
    TEST(SUBD, "#$90A0", 0x83, 0x90, 0xA0);
    TEST(ADDD, "#$90A0", 0xC3, 0x90, 0xA0);
    TEST(LDD,  "#$90A0", 0xCC, 0x90, 0xA0);

    symtab.intern(0x90, "dir90");
    symtab.intern(0x90A0, "dir90A0");

    disassembler.setCpu("6800");
    TEST(LDA, "B #dir90", 0xC6, 0x90);
    TEST(CPX, "#dir90A0", 0x8C, 0x90, 0xA0);
    TEST(LDX, "#dir90",   0xCE, 0x00, 0x90);
    TEST(LDS, "#dir90A0", 0x8E, 0x90, 0xA0);

    // MC6801
    disassembler.setCpu("6801");
    TEST(LDD,  "#dir90A0", 0xCC, 0x90, 0xA0);
    TEST(ADDD, "#dir90",   0xC3, 0x00, 0x90);
}

static void test_direct() {
    TEST(SUB, "A $90", 0x90, 0x90);
    TEST(CMP, "A $90", 0x91, 0x90);
    TEST(SBC, "A $90", 0x92, 0x90);
    TEST(AND, "A $90", 0x94, 0x90);
    TEST(BIT, "A $90", 0x95, 0x90);
    TEST(LDA, "A $90", 0x96, 0x90);
    TEST(STA, "A $90", 0x97, 0x90);
    TEST(EOR, "A $90", 0x98, 0x90);
    TEST(ADC, "A $90", 0x99, 0x90);
    TEST(ORA, "A $90", 0x9A, 0x90);
    TEST(ADD, "A $90", 0x9B, 0x90);

    TEST(SUB, "B $90", 0xD0, 0x90);
    TEST(CMP, "B $90", 0xD1, 0x90);
    TEST(SBC, "B $90", 0xD2, 0x90);
    TEST(AND, "B $90", 0xD4, 0x90);
    TEST(BIT, "B $90", 0xD5, 0x90);
    TEST(LDA, "B $90", 0xD6, 0x90);
    TEST(STA, "B $90", 0xD7, 0x90);
    TEST(EOR, "B $90", 0xD8, 0x90);
    TEST(ADC, "B $90", 0xD9, 0x90);
    TEST(ORA, "B $90", 0xDA, 0x90);
    TEST(ADD, "B $90", 0xDB, 0x90);

    TEST(CPX, "$90", 0x9C, 0x90);
    TEST(LDX, "$90", 0xDE, 0x90);
    TEST(STX, "$90", 0xDF, 0x90);
    TEST(LDS, "$90", 0x9E, 0x90);
    TEST(STS, "$90", 0x9F, 0x90);

    // MC6801
    disassembler.setCpu("6801");
    TEST(SUBD, "$90", 0x93, 0x90);
    TEST(ADDD, "$90", 0xD3, 0x90);
    TEST(LDD,  "$90", 0xDC, 0x90);
    TEST(STD,  "$90", 0xDD, 0x90);
    TEST(JSR,  "$90", 0x9D, 0x90);

    symtab.intern(0x10, "dir10");
    symtab.intern(0x22, "dir22");
    symtab.intern(0x90, "dir90");

    disassembler.setCpu("6800");
    TEST(LDA, "A <dir90", 0x96, 0x90);
    TEST(STA, "B <dir90", 0xD7, 0x90);
    TEST(CPX, "<dir22", 0x9C, 0x22);
    TEST(LDX, "<dir22", 0xDE, 0x22);
    TEST(STX, "<dir22", 0xDF, 0x22);
    TEST(LDS, "<dir90", 0x9E, 0x90);
    TEST(STS, "<dir90", 0x9F, 0x90);

    // MC6801
    disassembler.setCpu("6801");
    TEST(LDD, "<dir90", 0xDC, 0x90);
    TEST(JSR, "<dir22", 0x9D, 0x22);
}

static void test_extended() {
    TEST(NEG,  ">$0000", 0x70, 0x00, 0x00);
    TEST(COM,  ">$0009", 0x73, 0x00, 0x09);
    TEST(LSR,  ">$0034", 0x74, 0x00, 0x34);
    TEST(ROR,  "$1234",  0x76, 0x12, 0x34);
    TEST(ASR,  "$1234",  0x77, 0x12, 0x34);
    TEST(ASL,  "$1234",  0x78, 0x12, 0x34);
    TEST(ROL,  "$1234",  0x79, 0x12, 0x34);
    TEST(DEC,  "$1234",  0x7A, 0x12, 0x34);
    TEST(INC,  "$1234",  0x7C, 0x12, 0x34);
    TEST(TST,  "$1234",  0x7D, 0x12, 0x34);
    TEST(CLR,  "$1234",  0x7F, 0x12, 0x34);

    TEST(SUB, "A >$0090", 0xB0, 0x00, 0x90);
    TEST(CMP, "A >$0090", 0xB1, 0x00, 0x90);
    TEST(SBC, "A >$0090", 0xB2, 0x00, 0x90);
    TEST(AND, "A >$0090", 0xB4, 0x00, 0x90);
    TEST(BIT, "A >$0090", 0xB5, 0x00, 0x90);
    TEST(LDA, "A >$0090", 0xB6, 0x00, 0x90);
    TEST(STA, "A >$0090", 0xB7, 0x00, 0x90);
    TEST(EOR, "A >$0090", 0xB8, 0x00, 0x90);
    TEST(ADC, "A >$0090", 0xB9, 0x00, 0x90);
    TEST(ORA, "A >$0090", 0xBA, 0x00, 0x90);
    TEST(ADD, "A >$0090", 0xBB, 0x00, 0x90);

    TEST(SUB, "B $9ABC", 0xF0, 0x9A, 0xBC);
    TEST(CMP, "B $9ABC", 0xF1, 0x9A, 0xBC);
    TEST(SBC, "B $9ABC", 0xF2, 0x9A, 0xBC);
    TEST(AND, "B $9ABC", 0xF4, 0x9A, 0xBC);
    TEST(BIT, "B $9ABC", 0xF5, 0x9A, 0xBC);
    TEST(LDA, "B $9ABC", 0xF6, 0x9A, 0xBC);
    TEST(STA, "B $9ABC", 0xF7, 0x9A, 0xBC);
    TEST(EOR, "B $9ABC", 0xF8, 0x9A, 0xBC);
    TEST(ADC, "B $9ABC", 0xF9, 0x9A, 0xBC);
    TEST(ORA, "B $9ABC", 0xFA, 0x9A, 0xBC);
    TEST(ADD, "B $9ABC", 0xFB, 0x9A, 0xBC);

    TEST(CPX, "$9ABC", 0xBC, 0x9A, 0xBC);
    TEST(LDX, "$9ABC", 0xFE, 0x9A, 0xBC);
    TEST(STX, "$9ABC", 0xFF, 0x9A, 0xBC);
    TEST(LDS, "$9ABC", 0xBE, 0x9A, 0xBC);
    TEST(STS, "$9ABC", 0xBF, 0x9A, 0xBC);

    TEST(JMP, "$1234", 0x7E, 0x12, 0x34);
    TEST(JSR, "$1234", 0xBD, 0x12, 0x34);

    // MC6801
    disassembler.setCpu("6801");
    TEST(SUBD, "$1234", 0xB3, 0x12, 0x34);
    TEST(ADDD, "$1234", 0xF3, 0x12, 0x34);
    TEST(LDD,  "$1234", 0xFC, 0x12, 0x34);
    TEST(STD,  "$1234", 0xFD, 0x12, 0x34);

    symtab.intern(0x0090, "ext0090");
    symtab.intern(0x9ABC, "ext9ABC");

    disassembler.setCpu("6800");
    TEST(NEG, ">ext0090",   0x70, 0x00, 0x90);
    TEST(LDA, "A ext9ABC",  0xB6, 0x9A, 0xBC);
    TEST(STA, "B >ext0090", 0xF7, 0x00, 0x90);
    TEST(CPX, "ext9ABC",  0xBC, 0x9A, 0xBC);
    TEST(LDX, "ext9ABC",  0xFE, 0x9A, 0xBC);
    TEST(STX, ">ext0090", 0xFF, 0x00, 0x90);
    TEST(LDS, "ext9ABC",  0xBE, 0x9A, 0xBC);
    TEST(STS, ">ext0090", 0xBF, 0x00, 0x90);
    TEST(JMP, "ext9ABC",  0x7E, 0x9A, 0xBC);
    TEST(JSR, ">ext0090", 0xBD, 0x00, 0x90);

    // MC6801
    disassembler.setCpu("6801");
    TEST(SUBD, ">ext0090", 0xB3, 0x00, 0x90);
    TEST(ADDD,  "ext9ABC", 0xF3, 0x9A, 0xBC);
    TEST(LDD,  ">ext0090", 0xFC, 0x00, 0x90);
    TEST(STD,   "ext9ABC", 0xFD, 0x9A, 0xBC);
}

static void test_indexed() {
    TEST(NEG, "0,X", 0x60, 0x00);
    TEST(COM, "0,X", 0x63, 0x00);
    TEST(LSR, "1,X", 0x64, 0x01);
    TEST(ROR, "2,X", 0x66, 0x02);
    TEST(ASR, "3,X", 0x67, 0x03);
    TEST(ASL, "4,X", 0x68, 0x04);
    TEST(ROL, "5,X", 0x69, 0x05);
    TEST(DEC, "6,X", 0x6A, 0x06);
    TEST(INC, "127,X", 0x6C, 0x7F);
    TEST(TST, "128,X", 0x6D, 0x80);
    TEST(JMP, "254,X", 0x6E, 0xFE);
    TEST(CLR, "255,X", 0x6F, 0xFF);

    TEST(SUB, "A 0,X", 0xA0, 0x00);
    TEST(CMP, "A 0,X", 0xA1, 0x00);
    TEST(SBC, "A 1,X", 0xA2, 0x01);
    TEST(AND, "A 2,X", 0xA4, 0x02);
    TEST(BIT, "A 3,X", 0xA5, 0x03);
    TEST(LDA, "A 4,X", 0xA6, 0x04);
    TEST(STA, "A 5,X", 0xA7, 0x05);
    TEST(EOR, "A 6,X", 0xA8, 0x06);
    TEST(ADC, "A 127,X", 0xA9, 0x7F);
    TEST(ORA, "A 128,X", 0xAA, 0x80);
    TEST(ADD, "A 255,X", 0xAB, 0xFF);

    TEST(SUB, "B 0,X", 0xE0, 0x00);
    TEST(CMP, "B 0,X", 0xE1, 0x00);
    TEST(SBC, "B 1,X", 0xE2, 0x01);
    TEST(AND, "B 2,X", 0xE4, 0x02);
    TEST(BIT, "B 3,X", 0xE5, 0x03);
    TEST(LDA, "B 4,X", 0xE6, 0x04);
    TEST(STA, "B 5,X", 0xE7, 0x05);
    TEST(EOR, "B 6,X", 0xE8, 0x06);
    TEST(ADC, "B 127,X", 0xE9, 0x7F);
    TEST(ORA, "B 128,X", 0xEA, 0x80);
    TEST(ADD, "B 255,X", 0xEB, 0xFF);

    TEST(CPX, "0,X", 0xAC, 0x00);
    TEST(LDX, "0,X", 0xEE, 0x00);
    TEST(STX, "2,X", 0xEF, 0x02);
    TEST(LDS, "128,X", 0xAE, 0x80);
    TEST(STS, "255,X", 0xAF, 0xFF);

    TEST(JMP, "0,X",   0x6E, 0x00);
    TEST(JSR, "255,X", 0xAD, 0xFF);

    // MC6801
    disassembler.setCpu("6801");
    TEST(SUBD, "0,X",   0xA3, 0x00);
    TEST(ADDD, "128,X", 0xE3, 0x80);
    TEST(LDD,  "255,X", 0xEC, 0xFF);
    TEST(STD,  "0,X",   0xED, 0x00);

    symtab.intern(0,   "offset0");
    symtab.intern(255, "offset255");

    disassembler.setCpu("6800");
    TEST(NEG,     "offset0,X", 0x60, 0x00);
    TEST(COM,   "offset255,X", 0x63, 0xFF);
    TEST(CMP,   "A offset0,X", 0xA1, 0x00);
    TEST(ADD, "B offset255,X", 0xEB, 0xFF);
    TEST(JMP,     "offset0,X", 0x6E, 0x00);
    TEST(JSR,   "offset255,X", 0xAD, 0xFF);

    // MC6801
    disassembler.setCpu("6801");
    TEST(SUBD,   "offset0,X", 0xA3, 0x00);
    TEST(ADDD, "offset255,X", 0xE3, 0xFF);
    TEST(LDD,    "offset0,X", 0xEC, 0x00);
    TEST(STD,  "offset255,X", 0xED, 0xFF);
}

static void test_relative() {
    ATEST(0x1000, BRA, "$1002", 0x20, 0x00);
    ATEST(0x1000, BHI, "$1004", 0x22, 0x02);
    ATEST(0x1000, BLS, "$1002", 0x23, 0x00);
    ATEST(0x1000, BHS, "$1002", 0x24, 0x00);
    ATEST(0x1000, BLO, "$1002", 0x25, 0x00);
    ATEST(0x1000, BNE, "$1002", 0x26, 0x00);
    ATEST(0x1000, BEQ, "$1002", 0x27, 0x00);
    ATEST(0x1000, BVC, "$1002", 0x28, 0x00);
    ATEST(0x1000, BVS, "$1002", 0x29, 0x00);
    ATEST(0x1000, BPL, "$1002", 0x2A, 0x00);
    ATEST(0x1000, BMI, "$1002", 0x2B, 0x00);
    ATEST(0x1000, BGE, "$1002", 0x2C, 0x00);
    ATEST(0x1000, BLT, "$1002", 0x2D, 0x00);
    ATEST(0x1000, BGT, "$1002", 0x2E, 0x00);
    ATEST(0x1000, BLE, "$1002", 0x2F, 0x00);

    ATEST(0x1000, BSR, "$1042", 0x8D, 0x40);

    // MC6801
    disassembler.setCpu("6801");
    ATEST(0x1000, BRN, "$1081", 0x21, 0x7F);

    symtab.intern(0x0F82, "sub0F82");
    symtab.intern(0x1081, "sub1081");

    disassembler.setCpu("6800");
    ATEST(0x1000, BSR, "sub1081", 0x8D, 0x7F);
    ATEST(0x1000, BSR, "sub0F82", 0x8D, 0x80);

    // MC6801
    disassembler.setCpu("6801");
    ATEST(0x1000, BRN, "sub0F82", 0x21, 0x80);
}

static void assert_illegal(uint8_t opc) {
    char operands[40];
    Insn insn;
    const uint8_t codes[] = { opc };
    memory.setMemory(&codes[0], 1);
    disassembler.decode(memory, insn, operands, nullptr);
    char message[40];
    sprintf(message, "%s opecode 0x%02x", __FUNCTION__, opc);
    asserter.equals(message, UNKNOWN_INSTRUCTION, disassembler.getError());
}

static void test_illegal_mc6800() {
    const uint8_t illegals[] = {
        0x00, 0x02, 0x03, 0x04, 0x05,
        0x12, 0x13, 0x14, 0x15, 0x18, 0x1A, 0x1C, 0x1D, 0x1E, 0x1F,
        0x21, 0x38, 0x3A, 0x3C, 0x3D,
        0x41, 0x42, 0x45, 0x4B, 0x4E, 0x51, 0x52, 0x55, 0x5B, 0x5E,
        0x61, 0x62, 0x65, 0x6B, 0x71, 0x72, 0x75, 0x7B,
        0x83, 0x87, 0x8F, 0x93, 0x9D, 0xA3, 0xB3,
        0xC3, 0xC7, 0xCC, 0xCD, 0xCF, 0xD3, 0xDC, 0xDD,
        0xE3, 0xEC, 0xED, 0xF3, 0xFC, 0xFD,
    };
    disassembler.setCpu("6800");
    for (uint8_t idx = 0; idx < sizeof(illegals); idx++)
        assert_illegal(illegals[idx]);
}

static void test_illegal_mc6801() {
    const uint8_t illegals[] = {
        0x00, 0x02, 0x03,
        0x12, 0x13, 0x14, 0x15, 0x18, 0x1A, 0x1C, 0x1D, 0x1E, 0x1F,
        0x41, 0x42, 0x45, 0x4B, 0x4E, 0x51, 0x52, 0x55, 0x5B, 0x5E,
        0x61, 0x62, 0x65, 0x6B, 0x71, 0x72, 0x75, 0x7B,
        0x87, 0x8F,
        0xC7, 0xCD, 0xCF,
    };
    disassembler.setCpu("6801");
    for (uint8_t idx = 0; idx < sizeof(illegals); idx++)
        assert_illegal(illegals[idx]);
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
    RUN_TEST(test_illegal_mc6800);
    RUN_TEST(test_illegal_mc6801);
    return 0;
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

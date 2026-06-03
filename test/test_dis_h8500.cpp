/*
 * Copyright 2025 Tadashi G. Takaoka
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

#include "dis_h8500.h"
#include "test_dis_helper.h"

using namespace libasm;
using namespace libasm::h8500;
using namespace libasm::test;

DisH8500 dish8500;
Disassembler &disassembler(dish8500);

bool isH8520() {
    return strcasecmp_P("H8/520", disassembler.config().cpu_P()) == 0;
}

bool isH8532() {
    return strcasecmp_P("H8/532", disassembler.config().cpu_P()) == 0;
}

void set_up() {
    disassembler.reset();
}

void tear_down() {
    symtab.reset();
}

// clang-format off
void test_cpu() {
    EQUALS("cpu H8/500", true,    disassembler.setCpu("H8/500"));
    EQUALS_P("cpu H8/500", "H8/500", disassembler.config().cpu_P());

    EQUALS("cpu H8/520", true,    disassembler.setCpu("H8/520"));
    EQUALS_P("cpu H8/520", "H8/520", disassembler.config().cpu_P());

    EQUALS("cpu H8/532", true,    disassembler.setCpu("H8/532"));
    EQUALS_P("cpu H8/532", "H8/532", disassembler.config().cpu_P());
}

void test_spc_misc() {
    // No-operand instructions
    TEST("NOP",     "",    0x00);
    TEST("RTE",     "",    0x0A);
    TEST("RTS",     "",    0x31);
    TEST("SLEEP",   "",    0x32);
    TEST("TRAP/VS", "",    0x09);
    TEST("UNLK",    "FP",  0x0B);
    // TRAPA
    TEST("TRAPA",  "#0",   0x08, 0x10);
    TEST("TRAPA",  "#1",   0x08, 0x11);
    TEST("TRAPA",  "#3",   0x08, 0x13);
    // RTD
    TEST("RTD",    "#8",    0x14, 0x08);
    TEST("RTD",    "#$FF",  0x14, 0xFF);
    TEST("RTD",    "#$0200", 0x3C, 0x02, 0x00);
    // LINK/UNLK
    TEST("LINK",   "FP, #8",     0x17, 0x08);
    TEST("LINK",   "FP, #$FF",   0x17, 0xFF);
    TEST("LINK",   "FP, #$0200", 0x3F, 0x02, 0x00);
}

void test_spc_branch() {
    // Bcc d:8 (base addr=0, target = addr+2+delta)
    // Aliases share opcodes; disassembler outputs alphabetically-first name.
    TEST("BRA",  "$0002", 0x20, 0x00);  // BRA=BT, BRA wins
    TEST("BF",   "$0002", 0x21, 0x00);  // BF=BRN, BF wins
    TEST("BHI",  "$0002", 0x22, 0x00);
    TEST("BLS",  "$0002", 0x23, 0x00);
    TEST("BCC",  "$0002", 0x24, 0x00);  // BCC=BHS, BCC wins
    TEST("BCS",  "$0002", 0x25, 0x00);  // BCS=BLO, BCS wins
    TEST("BNE",  "$0002", 0x26, 0x00);
    TEST("BEQ",  "$0002", 0x27, 0x00);
    TEST("BVC",  "$0002", 0x28, 0x00);
    TEST("BVS",  "$0002", 0x29, 0x00);
    TEST("BPL",  "$0002", 0x2A, 0x00);
    TEST("BMI",  "$0002", 0x2B, 0x00);
    TEST("BGE",  "$0002", 0x2C, 0x00);
    TEST("BLT",  "$0002", 0x2D, 0x00);
    TEST("BGT",  "$0002", 0x2E, 0x00);
    TEST("BLE",  "$0002", 0x2F, 0x00);
    // Forward branch
    TEST("BRA",  "$0012", 0x20, 0x10);
    // Backward branch
    ATEST(0x0100, "BRA", "$00FE", 0x20, 0xFC);
    // BSR d:8
    TEST("BSR",  "$0002", 0x1E, 0x00);
    TEST("BSR",  "$0012", 0x1E, 0x10);

    // Bcc d:16 (target = addr+3+delta)
    TEST("BRA",  "$0003", 0x30, 0x00, 0x00);  // BRA=BT, BRA wins
    TEST("BLS",  "$0003", 0x33, 0x00, 0x00);
    TEST("BCC",  "$0003", 0x34, 0x00, 0x00);  // BCC=BHS, BCC wins
    TEST("BCS",  "$0003", 0x35, 0x00, 0x00);  // BCS=BLO, BCS wins
    TEST("BNE",  "$0003", 0x36, 0x00, 0x00);
    TEST("BEQ",  "$0003", 0x37, 0x00, 0x00);
    TEST("BVC",  "$0003", 0x38, 0x00, 0x00);
    TEST("BVS",  "$0003", 0x39, 0x00, 0x00);
    TEST("BPL",  "$0003", 0x3A, 0x00, 0x00);
    TEST("BMI",  "$0003", 0x3B, 0x00, 0x00);
    TEST("BLT",  "$0003", 0x3D, 0x00, 0x00);
    // BSR d:16
    TEST("BSR",  "$0003", 0x3E, 0x00, 0x00);
    TEST("BSR",  "$0103", 0x3E, 0x01, 0x00);
    // Forward branch d:16
    TEST("BRA",  "$0103", 0x30, 0x01, 0x00);
    // Backward branch d:16
    ATEST(0x0100, "BRA", "$0003", 0x30, 0xFF, 0x00);
}

void test_spc_move() {
    // MOV:E #imm8,Rd
    TEST("MOV:E", "#0, R0",    0x50, 0x00);
    TEST("MOV:E", "#$FF, R0",  0x50, 0xFF);
    TEST("MOV:E", "#$AB, R5",  0x55, 0xAB);
    // MOV:I #imm16,Rd
    TEST("MOV:I", "#0, R0",      0x58, 0x00, 0x00);
    TEST("MOV:I", "#$1234, R5",  0x5D, 0x12, 0x34);
    // CMP:E #imm8,Rd
    TEST("CMP:E", "#$FF, R0",  0x40, 0xFF);
    TEST("CMP:E", "#$AB, R3",  0x43, 0xAB);
    // CMP:I #imm16,Rd
    TEST("CMP:I", "#$FFFF, R0", 0x48, 0xFF, 0xFF);
    TEST("CMP:I", "#$1234, R7", 0x4F, 0x12, 0x34);
    // MOV:L @aa:8,Rd
    TEST("MOV:L", "@$10:8, R0", 0x60, 0x10);
    TEST("MOV:L", "@$FF:8, R7", 0x67, 0xFF);
    // MOV:S Rs,@aa:8
    TEST("MOV:S", "R0, @$10:8", 0x70, 0x10);
    TEST("MOV:S", "R7, @$FF:8", 0x77, 0xFF);
    // MOV:F @(d:8,FP),Rd
    TEST("MOV:F", "@(8, FP), R0",  0x80, 0x08);
    TEST("MOV:F", "@(-1, FP), R0", 0x80, 0xFF);
    TEST("MOV:F", "@(0, FP), R7",  0x87, 0x00);
    // MOV:F Rs,@(d:8,FP)
    TEST("MOV:F", "R0, @(8, FP)",  0x90, 0x08);
    TEST("MOV:F", "R7, @(-1, FP)", 0x97, 0xFF);
}

void test_spc_ldm_stm() {
    // LDM {reglist} — valid on all CPUs
    TEST("LDM", "{R0}",          0x02, 0x01);
    TEST("LDM", "{R7}",          0x02, 0x80);
    TEST("LDM", "{R7, FP}",      0x02, 0xC0);
    TEST("LDM", "{R5, R4, R3}",  0x02, 0x38);
    if (isH8532()) {
        // On H8/532, opcode 0x03 is PJSR @aa:24
        TEST("PJSR", "@$000102", 0x03, 0x00, 0x01, 0x02);
    } else {
        // STM {reglist} — H8/500 and H8/520 only
        TEST("STM", "{R0}",          0x03, 0x01);
        TEST("STM", "{R7}",          0x03, 0x80);
        TEST("STM", "{R7, FP}",      0x03, 0xC0);
        TEST("STM", "{R5, R4, R3}",  0x03, 0x38);
    }
}

void test_spc_jmp() {
    // JMP/JSR absolute
    TEST("JMP", "@$0000:16", 0x10, 0x00, 0x00);
    TEST("JMP", "@$1234:16", 0x10, 0x12, 0x34);
    TEST("JSR", "@$0000:16", 0x18, 0x00, 0x00);
    TEST("JSR", "@$1234:16", 0x18, 0x12, 0x34);
    // JMP/JSR indirect (0x11 prefix)
    TEST("JMP", "@R0",  0x11, 0xD0);
    TEST("JMP", "@R7",  0x11, 0xD7);
    TEST("JSR", "@R0",  0x11, 0xD8);
    TEST("JSR", "@R7",  0x11, 0xDF);
    // JMP/JSR @(d:8,Rn)
    TEST("JMP", "@(8, R0)",   0x11, 0xE0, 0x08);
    TEST("JMP", "@(-1, R0)",  0x11, 0xE0, 0xFF);
    TEST("JSR", "@(8, R0)",   0x11, 0xE8, 0x08);
    // JMP/JSR @(d:16,Rn)
    TEST("JMP", "@($0100, R0)", 0x11, 0xF0, 0x01, 0x00);
    TEST("JSR", "@($0100, R0)", 0x11, 0xF8, 0x01, 0x00);
}

void test_spc_scb() {
    // SCB/F, SCB/EQ, SCB/NE Rc,rel8
    TEST("SCB/F",  "R0, $0004",  0x01, 0xB8, 0x01);
    TEST("SCB/EQ", "R3, $0004",  0x07, 0xBB, 0x01);
    TEST("SCB/NE", "R7, $0004",  0x06, 0xBF, 0x01);
}

void test_spc_add_q() {
    // ADD:Q — encoded as GEN format (EA=Rn word, OP=imm opcode)
    TEST("ADD:Q", "R0",  0xA8, 0x08);  // +1
    TEST("ADD:Q", "R0",  0xA8, 0x09);  // -1
    TEST("ADD:Q", "R0",  0xA8, 0x0C);  // +2
    TEST("ADD:Q", "R0",  0xA8, 0x0D);  // -2
}

void test_h8532_specific() {
    if (!isH8532())
        return;
    // PJMP/PJSR absolute
    TEST("PJMP", "@$010203", 0x13, 0x01, 0x02, 0x03);
    TEST("PJSR", "@$010203", 0x03, 0x01, 0x02, 0x03);
    // PJMP/PJSR indirect (0x11 prefix)
    TEST("PJMP", "@R0", 0x11, 0xC0);
    TEST("PJMP", "@R7", 0x11, 0xC7);
    TEST("PJSR", "@R0", 0x11, 0xC8);
    TEST("PJSR", "@R7", 0x11, 0xCF);
    // PRTS, PRTD
    TEST("PRTS", "",      0x11, 0x19);
    TEST("PRTD", "#8",    0x11, 0x14, 0x08);
    TEST("PRTD", "#$0200", 0x11, 0x1C, 0x02, 0x00);
}

void test_gen_mov() {
    // MOV:G EA→Rd (opcode 0x80|Rd)
    TEST("MOV:G", "R0, R1",     0xA8, 0x81);  // EA=R0 word, OP=0x81
    TEST("MOV:G", "@R0, R1",    0xD8, 0x81);  // EA=@R0 word
    TEST("MOV:G", "@-R0, R1",   0xB8, 0x81);  // EA=@-R0 word
    TEST("MOV:G", "@R0+, R1",   0xC8, 0x81);  // EA=@R0+ word
    TEST("MOV:G", "@(8, R0), R1",   0xE8, 0x08, 0x81);  // EA=@(d8,R0) word
    TEST("MOV:G", "@(-1, R0), R1",  0xE8, 0xFF, 0x81);  // negative disp
    TEST("MOV:G", "@($0100, R0), R1", 0xF8, 0x01, 0x00, 0x81);  // disp16
    TEST("MOV:G", "@$10:8, R1",  0x0D, 0x10, 0x81);   // abs8 word
    TEST("MOV:G", "@$1000:16, R1", 0x1D, 0x10, 0x00, 0x81);  // abs16 word
    TEST("MOV:G", "#$FF, R1",    0x04, 0xFF, 0x81);    // imm8
    TEST("MOV:G", "#$1234, R1",  0x0C, 0x12, 0x34, 0x81);  // imm16
    // MOV:G Rs→EA (opcode 0x90|Rs)
    TEST("MOV:G", "R0, @R1",     0xD9, 0x90);  // EA=@R1 word
    TEST("MOV:G", "R0, @-R1",    0xB9, 0x90);
    TEST("MOV:G", "R0, @R1+",    0xC9, 0x90);
    TEST("MOV:G", "R5, @(8, R0)", 0xE8, 0x08, 0x95);
    TEST("MOV:G", "R5, @$10:8",  0x0D, 0x10, 0x95);   // abs8 word
}

void test_gen_alu() {
    // ADD:G @R0,R1
    TEST("ADD:G", "@R0, R1",  0xD8, 0x21);  // OP=0x20|R1
    // SUB @R0,R1
    TEST("SUB",   "@R0, R1",  0xD8, 0x31);
    // ADDS @R0,R1
    TEST("ADDS",  "@R0, R1",  0xD8, 0x29);
    // ADDX @R0,R1 (byte EA)
    TEST("ADDX",  "R0, R1",   0xA0, 0xA1);  // byte EA M_REG R0
    // SUBX
    TEST("SUBX",  "R0, R1",   0xA0, 0xB1);
    // CMP:G
    TEST("CMP:G", "@R0, R1",  0xD8, 0x71);
    // AND
    TEST("AND",   "@R0, R1",  0xD8, 0x51);
    // OR
    TEST("OR",    "@R0, R1",  0xD8, 0x41);
    // XOR
    TEST("XOR",   "@R0, R1",  0xD8, 0x61);
    // MULXU (byte)
    TEST("MULXU", "R0, R1",   0xA0, 0xA9);
    // DIVXU (byte)
    TEST("DIVXU", "R0, R1",   0xA0, 0xB9);
    // CLR (unary)
    TEST("CLR",   "R0",       0xA8, 0x13);   // EA=R0 word
    // NEG
    TEST("NEG",   "R0",       0xA8, 0x14);
    // NOT
    TEST("NOT",   "R0",       0xA8, 0x15);
    // EXTS/EXTU (byte → no szBit in EA)
    TEST("EXTS",  "R0",       0xA0, 0x11);   // EA=R0 byte
    TEST("EXTU",  "R0",       0xA0, 0x12);
    // SWAP, TAS (byte)
    TEST("SWAP",  "R0",       0xA0, 0x10);
    TEST("TAS",   "R0",       0xA0, 0x17);
    // TST (word)
    TEST("TST",   "R0",       0xA8, 0x16);
}

void test_gen_shift() {
    TEST("SHAL",  "R0",  0xA8, 0x18);
    TEST("SHAR",  "R0",  0xA8, 0x19);
    TEST("SHLL",  "R0",  0xA8, 0x1A);
    TEST("SHLR",  "R0",  0xA8, 0x1B);
    TEST("ROTL",  "R0",  0xA8, 0x3C);
    TEST("ROTR",  "R0",  0xA8, 0x3D);
    TEST("ROTXL", "R0",  0xA8, 0x3E);
    TEST("ROTXR", "R0",  0xA8, 0x3F);
}

void test_gen_bit() {
    // BSET R5,@R0 (byte)
    TEST("BSET", "R5, @R0",  0xD0, 0x4D);
    // BSET #3,@R0
    TEST("BSET", "#3, @R0",  0xD0, 0xC3);
    // BCLR R5,@R0
    TEST("BCLR", "R5, @R0",  0xD0, 0x5D);
    // BCLR #3,@R0
    TEST("BCLR", "#3, @R0",  0xD0, 0xD3);
    // BNOT R5,@R0
    TEST("BNOT", "R5, @R0",  0xD0, 0x6D);
    // BNOT #3,@R0
    TEST("BNOT", "#3, @R0",  0xD0, 0xE3);
    // BTST R5,@R0
    TEST("BTST", "R5, @R0",  0xD0, 0x7D);
    // BTST #3,@R0
    TEST("BTST", "#3, @R0",  0xD0, 0xF3);
}

void test_gen_cr() {
    // LDC EA,CR
    TEST("LDC",  "R0, CCR",  0xA0, 0x89);   // byte EA R0, CCR=1
    TEST("LDC",  "R0, SR",   0xA0, 0x88);   // SR=0
    TEST("LDC",  "R0, BR",   0xA0, 0x8B);   // BR=3
    TEST("LDC",  "R0, EP",   0xA0, 0x8C);   // EP=4
    TEST("LDC",  "R0, DP",   0xA0, 0x8D);   // DP=5
    TEST("LDC",  "R0, TP",   0xA0, 0x8F);   // TP=7
    // STC CR,EA
    TEST("STC",  "CCR, R0",  0xA8, 0x99);   // word EA R0, CCR=1
    TEST("STC",  "SR, R0",   0xA8, 0x98);
    // ANDC #imm8,CR
    TEST("ANDC", "#$F0, CCR", 0x04, 0xF0, 0x59);  // EA=imm8, OP=0x58|CCR(1)
    // ORC #imm8,CR
    TEST("ORC",  "#15, CCR", 0x04, 0x0F, 0x79);  // OP=0x78|CCR(1)
    // XORC #imm8,CR
    TEST("XORC", "#$FF, CCR", 0x04, 0xFF, 0x69);  // OP=0x68|CCR(1)
}

void test_ext_format() {
    // DADD/DSUB: valid on all CPUs
    TEST("DADD", "@R0, R1",  0xD0, 0x00, 0xA1);
    TEST("DSUB", "@R0, R1",  0xD0, 0x00, 0xB1);
    // MOVFPE/MOVTPE: H8/500 and H8/532 only (have E-clock interface)
    if (isH8520()) {
        ERRT("", "", UNKNOWN_INSTRUCTION, "", 0xD0, 0x00, 0x81);
    } else {
        TEST("MOVFPE", "@R0, R1",  0xD0, 0x00, 0x81);
        TEST("MOVTPE", "R1, @R0",  0xD8, 0x00, 0x91);
    }
}

// clang-format on

void run_tests(const char *cpu) {
    disassembler.setCpu(cpu);
    RUN_TEST(test_spc_misc);
    RUN_TEST(test_spc_branch);
    RUN_TEST(test_spc_move);
    RUN_TEST(test_spc_ldm_stm);
    RUN_TEST(test_spc_jmp);
    RUN_TEST(test_spc_scb);
    RUN_TEST(test_spc_add_q);
    RUN_TEST(test_h8532_specific);
    RUN_TEST(test_gen_mov);
    RUN_TEST(test_gen_alu);
    RUN_TEST(test_gen_shift);
    RUN_TEST(test_gen_bit);
    RUN_TEST(test_gen_cr);
    RUN_TEST(test_ext_format);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

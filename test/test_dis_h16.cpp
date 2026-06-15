/*
 * Copyright 2026 Tadashi G. Takaoka
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

#include "dis_h16.h"
#include "test_dis_helper.h"

using namespace libasm;
using namespace libasm::h16;
using namespace libasm::test;

DisH16 dish16;
Disassembler &disassembler(dish16);

void set_up() {
    disassembler.reset();
}

void tear_down() {
    symtab.reset();
}

// clang-format off

void test_cpu() {
    EQUALS("cpu HD641016", true, disassembler.setCpu("HD641016"));
    EQUALS_P("cpu HD641016", "HD641016", disassembler.config().cpu_P());
}

// System / no-op / exception / bank-control instructions.
void test_system() {
    TEST("NOP",   "",   0xFF);
    TEST("RTS",   "",   0xBB);
    TEST("RTE",   "",   0xF1);
    TEST("RTR",   "",   0xF4);
    TEST("RESET", "",   0xF0);
    TEST("SLEEP", "",   0xF5);
    TEST("ICBN",  "",   0xFD);
    TEST("DCBN",  "",   0xFE);
    TEST("CGBN",  "R1", 0xE4, 0x01);
    TEST("TRAPA", "#5", 0xF2, 0x05);
    TEST("TRAPA", "#0", 0xF2, 0x00);
    TEST("TRAPA", "#15", 0xF2, 0x0F);
}

// Arithmetic: ADD/SUB variants, sign-extend, CX-flag, negate, decimal.
void test_arith() {
    // ADD:G  general-format EA-EA across .B/.W/.L and addressing modes
    TEST("ADD:G.B", "R0, R1",  0x00, 0x40, 0x41);
    TEST("ADD:G.W", "R0, R1",  0x01, 0x40, 0x41);
    TEST("ADD:G.L", "R0, R1",  0x02, 0x40, 0x41);
    TEST("ADD:G.W", "R1, R0",  0x01, 0xC1);          // accumulator form
    TEST("ADD:G.W", "@R0, R1", 0x01, 0x00, 0x41);
    // ADD:Q  quick (8-bit signed imm, EAd)
    TEST("ADD:Q.B", "#16, R0",  0x10, 0x10, 0x40);
    TEST("ADD:Q.W", "#16, R0",  0x11, 0x10, 0x40);
    TEST("ADD:Q.L", "#16, R0",  0x12, 0x10, 0x40);
    TEST("ADD:Q.B", "#-16, R0", 0x10, 0xF0, 0x40);
    // ADD:R / SUB:R register-register
    TEST("ADD:R.B",  "R0, R1", 0x20, 0x01);
    TEST("SUB:R.B",  "R0, R1", 0x24, 0x01);
    // ADD:RQ / SUB:RQ register-quick (imm4, Rnd)
    TEST("ADD:RQ.B", "#5, R0", 0x30, 0x05);
    // ADDS / SUBS sign-extend; ADDX / SUBX with CX
    TEST("ADDS.B", "R0, R1", 0x40, 0x40, 0x41);
    TEST("ADDX.B", "R0, R1", 0x50, 0x40, 0x41);
    // NEG / NEGX
    TEST("NEG.B",  "R0", 0x8C, 0x40);
    TEST("NEG.W",  "R0", 0x8D, 0x40);
    TEST("NEG.L",  "R0", 0x8E, 0x40);
    TEST("NEGX.B", "R0", 0x9C, 0x40);
    // Decimal arithmetic (no Sz)
    TEST("DADD",   "R0, R1", 0x83, 0x40, 0x41);
    TEST("DSUB",   "R0, R1", 0x87, 0x40, 0x41);
    TEST("DNEG.B", "R0",     0xAF, 0x40);
}

// Compare and test (read-only).
void test_compare() {
    TEST("CMP:G.B",  "R0, R1",  0x08, 0x40, 0x41);
    TEST("CMP:G.W",  "R0, R1",  0x09, 0x40, 0x41);
    TEST("CMP:G.L",  "R0, R1",  0x0A, 0x40, 0x41);
    TEST("CMP:Q.B",  "#16, R0", 0x18, 0x10, 0x40);
    TEST("CMP:R.B",  "R0, R1",  0x28, 0x01);
    TEST("CMP:RQ.B", "#5, R0",  0x38, 0x05);
    TEST("CMPS.B",   "R0, R1",  0x48, 0x40, 0x41);
    TEST("TST.B",    "R0",      0x58, 0x40);
}

// Data movement: MOV variants, MOVA, MOVF, CLR, XCH, SWAP.
void test_data_move() {
    TEST("MOV:G.B",  "R0, R1",  0x0C, 0x40, 0x41);
    TEST("MOV:G.W",  "R0, R1",  0x0D, 0x40, 0x41);
    TEST("MOV:G.L",  "R0, R1",  0x0E, 0x40, 0x41);
    TEST("MOV:Q.B",  "#16, R0", 0x1C, 0x10, 0x40);
    TEST("MOV:R.B",  "R0, R1",  0x2C, 0x01);
    TEST("MOV:R.W",  "R0, R1",  0x2D, 0x01);
    TEST("MOV:R.L",  "R0, R1",  0x2E, 0x01);
    TEST("MOV:RQ.B", "#5, R0",  0x3C, 0x05);
    TEST("MOVS.B",   "R0, R1",  0x4C, 0x40, 0x41);
    TEST("MOVF.B",   "R1",      0x5C, 0x41);
    TEST("MOVF.W",   "R1",      0x5D, 0x41);
    TEST("MOVF.L",   "R1",      0x5E, 0x41);
    TEST("CLR.B",    "R0",      0x14, 0x40);
    TEST("CLR.W",    "R0",      0x15, 0x40);
    TEST("CLR.L",    "R0",      0x16, 0x40);
    TEST("XCH.L",    "R0, R1",  0xB3, 0x01);
    TEST("SWAP.W",   "R0",      0xEB, 0x40);
}

// Logical ops AND/OR/XOR/NOT.
void test_logic() {
    TEST("AND.B", "R0, R1", 0x80, 0x40, 0x41);
    TEST("OR.B",  "R0, R1", 0x88, 0x40, 0x41);
    TEST("XOR.B", "R0, R1", 0x84, 0x40, 0x41);
    TEST("NOT.B", "R0",     0x90, 0x40);
}

// Shift, rotate, and bit-manipulation (2-byte opcodes).
void test_shift_bit() {
    // SFT dynamic (count in Rn)
    TEST("SHAR.B",  "R2, R1", 0x60, 0x02, 0x41);
    TEST("SHLR.B",  "R2, R1", 0x60, 0x22, 0x41);
    TEST("ROTR.B",  "R2, R1", 0x60, 0x42, 0x41);
    TEST("ROTXR.B", "R2, R1", 0x60, 0x62, 0x41);
    TEST("SHAL.B",  "R2, R1", 0x60, 0x82, 0x41);
    TEST("SHAL.W",  "R2, R1", 0x61, 0x82, 0x41);
    TEST("SHAL.L",  "R2, R1", 0x62, 0x82, 0x41);
    // SFT static (count in imm5)
    TEST("SHAL.B",  "#3, R1", 0x64, 0x83, 0x41);
    // BIT dynamic (bit-no in Rn)
    TEST("BSET.B",  "R2, @R1", 0x68, 0x02, 0x01);
    TEST("BNOT.B",  "R2, @R1", 0x68, 0x22, 0x01);
    TEST("BCLR.B",  "R2, @R1", 0x68, 0x42, 0x01);
    TEST("BTST.B",  "R2, @R1", 0x68, 0x62, 0x01);
    // BIT static (bit-no in imm5)
    TEST("BSET.B",  "#3, @R1", 0x6C, 0x03, 0x01);
}

// Multiply and divide (2-byte opcodes); .B / .W from bit 4 of mode byte.
void test_mul_div() {
    TEST("MULXS.B", "R0, R1", 0xEE, 0x00, 0x40, 0x41);
    TEST("MULXS.W", "R0, R1", 0xEE, 0x10, 0x40, 0x41);
    TEST("MULXU.B", "R0, R1", 0xEE, 0x40, 0x40, 0x41);
    TEST("MULXU.W", "R0, R1", 0xEE, 0x50, 0x40, 0x41);
    TEST("DIVXS.B", "R0, R1", 0xEF, 0x00, 0x40, 0x41);
    TEST("DIVXU.B", "R0, R1", 0xEF, 0x40, 0x40, 0x41);
}

// EXTU / EXTS use the non-standard Sz mapping 0=W, 1=L, 2=B.
void test_extend() {
    TEST("EXTU.W", "R0", 0xAC, 0x00);
    TEST("EXTU.L", "R0", 0xAD, 0x00);
    TEST("EXTU.B", "R0", 0xAE, 0x00);
    TEST("EXTS.W", "R0", 0xBC, 0x00);
    TEST("EXTS.L", "R0", 0xBD, 0x00);
    TEST("EXTS.B", "R0", 0xBE, 0x00);
}

// PC-relative branches.  Includes the full 16-condition Bcc:G fan-out and
// the HS / LO aliases which encode the same low nibble as CC / CS.
void test_branch() {
    // BRA / BEQ / BNE / BSR (Sd in low 2 bits)
    TEST("BRA.B", "$00000002", 0x98, 0x00);
    TEST("BEQ.B", "$00000002", 0xA0, 0x00);
    TEST("BNE.B", "$00000002", 0xB0, 0x00);
    TEST("BSR.B", "$00000002", 0xA8, 0x00);
    // Bcc:G, one per condition value (cc = low nibble of 2nd byte)
    TEST("BT:G.B",  "$00000003", 0xA4, 0x00, 0x00);
    TEST("BF:G.B",  "$00000003", 0xA4, 0x01, 0x00);
    TEST("BHI:G.B", "$00000003", 0xA4, 0x02, 0x00);
    TEST("BLS:G.B", "$00000003", 0xA4, 0x03, 0x00);
    TEST("BCC:G.B", "$00000003", 0xA4, 0x04, 0x00);  // CC = HS
    TEST("BCS:G.B", "$00000003", 0xA4, 0x05, 0x00);  // CS = LO
    TEST("BNE:G.B", "$00000003", 0xA4, 0x06, 0x00);
    TEST("BEQ:G.B", "$00000003", 0xA4, 0x07, 0x00);
    TEST("BVC:G.B", "$00000003", 0xA4, 0x08, 0x00);
    TEST("BVS:G.B", "$00000003", 0xA4, 0x09, 0x00);
    TEST("BPL:G.B", "$00000003", 0xA4, 0x0A, 0x00);
    TEST("BMI:G.B", "$00000003", 0xA4, 0x0B, 0x00);
    TEST("BGE:G.B", "$00000003", 0xA4, 0x0C, 0x00);
    TEST("BLT:G.B", "$00000003", 0xA4, 0x0D, 0x00);
    TEST("BGT:G.B", "$00000003", 0xA4, 0x0E, 0x00);
    TEST("BLE:G.B", "$00000003", 0xA4, 0x0F, 0x00);
    // Word-sized Sd
    TEST("BHI:G.W", "$00000004", 0xA5, 0x02, 0x00, 0x00);
}

// Computed jumps (any EA).
void test_jump() {
    TEST("JMP", "@R0",          0x9B, 0x00);
    TEST("JMP", "@($1234, R0)", 0x9B, 0x20, 0x12, 0x34);
    TEST("JSR", "@R0",          0xAB, 0x00);
    TEST("JSR", "@$1234:16", 0xAB, 0x76, 0x12, 0x34);
}

// Stack-frame setup/teardown.
void test_frame() {
    TEST("LINK.B", "R0, #16",    0xD0, 0x00, 0x10);
    TEST("LINK.W", "R7, #-$0100", 0xD1, 0x07, 0xFF, 0x00);
    TEST("UNLK",   "R0",         0xD3, 0x00);
    TEST("UNLK",   "R7",         0xD3, 0x07);
    TEST("RTD.B",  "#16",        0xB8, 0x10);
    TEST("RTD.W",  "#$0200",     0xB9, 0x02, 0x00);
}

// Control-register access: ANDC / ORC / XORC / LDC / STC.
void test_cr() {
    TEST("ANDC", "R0, CCR",  0xF8, 0x20, 0x40);
    TEST("ANDC", "R0, SR",   0xF8, 0xA0, 0x40);
    TEST("ANDC", "R0, CBNR", 0xF8, 0x40, 0x40);
    TEST("ORC",  "R0, CCR",  0xF9, 0x20, 0x40);
    TEST("XORC", "R0, CCR",  0xFA, 0x20, 0x40);
    TEST("LDC",  "R0, CBNR", 0xFB, 0x40, 0x40);
    TEST("LDC",  "R0, VBNR", 0xFB, 0x01, 0x40);
    TEST("STC",  "CBNR, R0", 0xFC, 0x40, 0x40);
}

// Bank-prefix EA bytes 0x70 (<CRn>) and 0x74 (<PRn>) make the following EA's
// register field reference the current or previous bank.  Per Manual Note 3
// to Table 4-7, the prefix can precede any of EA modes #1..#13.
void test_bank_prefix() {
    TEST("ADD:G.W", "CR0, R1",  0x01, 0x70, 0x40, 0x41);
    TEST("ADD:G.W", "@CR0, R1", 0x01, 0x70, 0x00, 0x41);
    TEST("ADD:G.W", "PR5, R1",  0x01, 0x74, 0x45, 0x41);
}

// Don't-care ("*") bits in the manual are zero-required: setting any of them
// to 1 must report ILLEGAL_OPERAND_MODE.
void test_illegal_zero() {
    // SFT dynamic byte: bit 4 must be 0
    ERRT("SHAR.B", "R2, R1",  ILLEGAL_OPERAND_MODE, "", 0x60, 0x12, 0x41);
    // BIT dynamic byte: bits 7 and 4 must be 0
    ERRT("BSET.B", "R2, @R1", ILLEGAL_OPERAND_MODE, "", 0x68, 0x82, 0x01);
    ERRT("BSET.B", "R2, @R1", ILLEGAL_OPERAND_MODE, "", 0x68, 0x12, 0x01);
    // BIT static byte: bit 7 must be 0
    ERRT("BSET.B", "#3, @R1", ILLEGAL_OPERAND_MODE, "", 0x6C, 0x83, 0x01);
    // MUL mode byte: bits 7, 5, 3-0 must be 0
    ERRT("MULXS.B", "R0, R1", ILLEGAL_OPERAND_MODE, "", 0xEE, 0x01, 0x40, 0x41);
    // Bcc:G cc byte: high nibble must be 0
    ERRT("BEQ:G.B", "$00000003", ILLEGAL_OPERAND_MODE, "", 0xA4, 0x17, 0x00);
    // TRAPA, UNLK Rn: upper nibble of operand byte must be 0
    ERRT("TRAPA", "#5", ILLEGAL_OPERAND_MODE, "", 0xF2, 0x15);
    ERRT("UNLK",  "R0", ILLEGAL_OPERAND_MODE, "", 0xD3, 0x10);
}

// clang-format on

void run_tests(const char *cpu) {
    disassembler.setCpu(cpu);
    RUN_TEST(test_system);
    RUN_TEST(test_arith);
    RUN_TEST(test_compare);
    RUN_TEST(test_data_move);
    RUN_TEST(test_logic);
    RUN_TEST(test_shift_bit);
    RUN_TEST(test_mul_div);
    RUN_TEST(test_extend);
    RUN_TEST(test_branch);
    RUN_TEST(test_jump);
    RUN_TEST(test_frame);
    RUN_TEST(test_cr);
    RUN_TEST(test_bank_prefix);
    RUN_TEST(test_illegal_zero);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

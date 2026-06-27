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
    EQUALS("cpu H16", true, disassembler.setCpu("H16"));
    EQUALS_P("cpu H16", "H16", disassembler.config().cpu_P());
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
    TEST("TAS",   "@R0", 0xEC, 0x00);
    TEST("TRAP/T",  "",    0xF3, 0x00);
    TEST("TRAP/EQ", "",    0xF3, 0x07);
    TEST("SET/NE",  "@R0", 0xB7, 0x06, 0x00);
    ATEST(0x1000, "SCB/EQ.B", "R1, $00001003", 0xB4, 0x17, 0x00);
    ATEST(0x1000, "SCB/EQ.W", "R1, $00001010", 0xB5, 0x17, 0x00, 0x0C);
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
    TEST("DNEG",   "R0",     0xAF, 0x40);  // byte-only, no suffix
}

// Compare and test (read-only).
void test_compare() {
    TEST("CMP:G.B",  "R0, R1",  0x08, 0x40, 0x41);
    TEST("CMP:G.W",  "R0, R1",  0x09, 0x40, 0x41);
    TEST("CMP:G.L",  "R0, R1",  0x0A, 0x40, 0x41);
    TEST("CMP:Q.B",  "#16, R0", 0x18, 0x10, 0x40);
    TEST("CMP:R.B",  "R0, R1",  0x28, 0x01);
    // CMP:RQ imm4 is signed -8..7 (sign-extended).
    TEST("CMP:RQ.B", "#5, R0",  0x38, 0x05);
    TEST("CMP:RQ.B", "#-8, R0", 0x38, 0x08);
    TEST("CMP:RQ.B", "#-1, R1", 0x38, 0x1F);
    TEST("CMPS.B",   "R0, R1",  0x48, 0x40, 0x41);
    TEST("TST.B",    "R0",      0x58, 0x40);
    // Accumulator form (src EA bit 7 set): the implicit-R0 second operand has
    // no byte.  CMP:G / CMPS carry it as M_EASRC, which must substitute R0
    // rather than consuming the next instruction's byte.
    TEST("CMP:G.W",  "@R1, R0", 0x09, 0x81);
    TEST("CMPS.W",   "@R1, R0", 0x49, 0x81);
}

// Data movement: MOV variants, MOVA, MOVF, CLR, XCH, SWAP.
void test_bitfield() {
    TEST("BFEXT", "R0, R1, @R2, R3", 0xD4, 0x01, 0x02, 0x43);
    TEST("BFINS", "R0, R1, R2, @R3", 0xD5, 0x01, 0x42, 0x03);
    TEST("BFSCH", "R0, R1, @R2, R3", 0xD6, 0x01, 0x02, 0x43);
    TEST("BFMOV", "R0, R1, R2, R3, R4, R5", 0xD7, 0x01, 0x23, 0x45);
}

void test_string() {
    TEST("SMOV/F.W", "R0, R1, R2",     0x95, 0x22, 0x01);
    TEST("SMOV/B.W", "R0, R1, R2",     0x95, 0x52, 0x01);
    TEST("SMOV/F.B", "R0, R1, R2",     0x94, 0x22, 0x01);
    TEST("SSTR/F.W", "R0, R1, R2",     0x95, 0x02, 0x01);
    TEST("SCMP/EQ/F.W", "R0, R1, R2, R3", 0x95, 0xA2, 0x01, 0x37);
    TEST("SCMP/EQ/B.W", "R0, R1, R2, R3", 0x95, 0xD2, 0x01, 0x37);
    TEST("SSCH/NE/F.W", "R0, R1, R2, R3", 0x95, 0x82, 0x01, 0x36);
}

void test_reg_list() {
    TEST("STM.W", "R0-R3, @R4",   0x71, 0x04, 0x00, 0x0F);
    TEST("STM.B", "R0-R15, @R4",  0x70, 0x04, 0xFF, 0xFF);
    // List elements are comma-separated single registers and ranges (ASL).
    TEST("STM.W", "R0, R2, R5, @R4",        0x71, 0x04, 0x00, 0x25);
    TEST("STM.L", "R0, R3-R12, R15, @R4",   0x72, 0x04, 0x9F, 0xF9);
    TEST("LDM.W", "@R4, R0-R3",   0x75, 0x04, 0x00, 0x0F);
    TEST("LDM.W", "@R4, R0, R3-R12, R15",   0x75, 0x04, 0x9F, 0xF9);
    TEST("PGBN",  "",             0xE8);
    TEST("PGBN",  "R0-R3",        0xE9, 0x00, 0x0F);
    TEST("PGBN",  "R0-R3, R8",    0xE9, 0x01, 0x0F);
}

void test_data_move() {
    TEST("MOV:G.B",  "R0, R1",  0x0C, 0x40, 0x41);
    TEST("MOV:G.W",  "R0, R1",  0x0D, 0x40, 0x41);
    TEST("MOV:G.L",  "R0, R1",  0x0E, 0x40, 0x41);
    TEST("MOV:Q.B",  "#16, R0", 0x1C, 0x10, 0x40);
    TEST("MOV:R.B",  "R0, R1",  0x2C, 0x01);
    TEST("MOV:R.W",  "R0, R1",  0x2D, 0x01);
    TEST("MOV:R.L",  "R0, R1",  0x2E, 0x01);
    TEST("MOV:RQ.B", "#5, R0",  0x3C, 0x05);
    TEST("MOV:RQ.B", "#-3, R0", 0x3C, 0x0D);  // signed imm4
    TEST("MOVS.B",   "R0, R1",  0x4C, 0x40, 0x41);
    TEST("MOVF.B",   "R1",      0x5C, 0x41);
    TEST("MOVF.W",   "R1",      0x5D, 0x41);
    TEST("MOVF.L",   "R1",      0x5E, 0x41);
    TEST("CLR.B",    "R0",      0x14, 0x40);
    TEST("CLR.W",    "R0",      0x15, 0x40);
    TEST("CLR.L",    "R0",      0x16, 0x40);
    TEST("XCH",      "R0, R1",  0xB3, 0x01);  // long-only, no suffix
    TEST("SWAP.B",   "R0",      0xEA, 0x40);
    TEST("SWAP.W",   "R0",      0xEB, 0x40);
    TEST("MOVTP.W",  "R0, @R1", 0xE0, 0x40, 0x01);
    TEST("MOVTP.L",  "R0, @R1", 0xE1, 0x40, 0x01);
    TEST("MOVFP.W",  "@R1, R0", 0xE2, 0x01, 0x40);
    TEST("MOVFP.L",  "@R1, R0", 0xE3, 0x01, 0x40);
    TEST("MOVTPE.B", "R0, @R1", 0x78, 0x40, 0x01);
    TEST("MOVTPE.L", "R0, @R1", 0x7A, 0x40, 0x01);
    TEST("MOVFPE.W", "@R1, R0", 0x7D, 0x01, 0x40);
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
    TEST("JSR", "@$1234", 0xAB, 0x76, 0x12, 0x34);  // 16-bit minimal -> bare
}

// EA field size suffix: disp/abs auto-size, so a bare value is emitted when the
// encoded field is its natural (signed) width and an explicit :N only when the
// field is wider -- the bare form then re-assembles to the same bytes.
void test_ea_size() {
    // Displacement: bare at 8-bit minimal, :16/:32 when the field is wider.
    TEST("ADD:G.W", "@(5, R0), R1",     0x01, 0x10, 0x05, 0x41);
    TEST("ADD:G.W", "@(5:16, R0), R1",  0x01, 0x20, 0x00, 0x05, 0x41);
    TEST("ADD:G.W", "@(5:32, R0), R1",  0x01, 0x30, 0x00, 0x00, 0x00, 0x05, 0x41);
    TEST("ADD:G.W", "@($1234, R0), R1", 0x01, 0x20, 0x12, 0x34, 0x41);  // 16-bit minimal
    // Absolute: bare when minimal, :N when wider.  A negative absolute uses its
    // full 32-bit form (a leading "@-" would be the @-Rn auto-decrement mode).
    TEST("ADD:G.W", "@5, R1",           0x01, 0x75, 0x05, 0x41);
    TEST("ADD:G.W", "@$1234, R1",       0x01, 0x76, 0x12, 0x34, 0x41);
    TEST("ADD:G.W", "@5:16, R1",        0x01, 0x76, 0x00, 0x05, 0x41);
    TEST("ADD:G.W", "@$FFFFFF80, R1",   0x01, 0x75, 0x80, 0x41);
    // Immediate keeps :N always (ASL auto-sizes a bare immediate to word) and
    // renders signed.
    TEST("ADD:G.W", "#5:8, R1",         0x01, 0x71, 0x05, 0x41);
    TEST("ADD:G.W", "#-1:8, R1",        0x01, 0x71, 0xFF, 0x41);
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
    // TRAP/SET: the [*|cc] byte's high nibble (don't-care) must be 0.
    ERRT("TRAP/EQ", "",    ILLEGAL_OPERAND_MODE, "", 0xF3, 0x17);
    ERRT("SET/EQ",  "@R0", ILLEGAL_OPERAND_MODE, "", 0xB7, 0x17, 0x00);
    // Double-indirect: each Sd field must be byte (01) or long (11); a 0 or
    // word (10) field is illegal (here s1=s2=00).
    ERRT("ADD:G.W", "@(0,@(0, R0)), R1", ILLEGAL_OPERAND_MODE, "",
            0x01, 0x7E, 0x00, 0x41);
    // Scale mode (0x78-0x7B) ext byte [*|*|Sf|Rn]: mode 7 has no index-size
    // bit, so both spare top bits (7 and 6) must be 0.
    ERRT("ADD:G.W", "@(R5*4), R2", ILLEGAL_OPERAND_MODE, "", 0x01, 0x78, 0x65, 0x42);
    ERRT("ADD:G.W", "@(R5*4), R2", ILLEGAL_OPERAND_MODE, "", 0x01, 0x78, 0xA5, 0x42);
    // Index mode (0x7C) ext byte [**|Sf|Xm]: the top 2 bits are "don't care"
    // (manual Second Expansion Byte) and must be 0 to round-trip exactly.
    ERRT("ADD:G.W", "@(R2.W*2, R1), R1", ILLEGAL_OPERAND_MODE, "", 0x01, 0x7C, 0x01, 0x52, 0x41);
    ERRT("ADD:G.W", "@(R2.W*2, R1), R1", ILLEGAL_OPERAND_MODE, "", 0x01, 0x7C, 0x01, 0x92, 0x41);
    // Bit-field EAs/EAd never use accumulator form, so bit 7 of either EA byte
    // is a reserved bit that must be 0.
    ERRT("BFINS", "R0, R1, @R2, @R3", ILLEGAL_OPERAND_MODE, "", 0xD5, 0x01, 0x82, 0x03);
    ERRT("BFINS", "R0, R1, @R2, @R3", ILLEGAL_OPERAND_MODE, "", 0xD5, 0x01, 0x02, 0x83);
    ERRT("BFEXT", "R0, R1, @R2, @R3", ILLEGAL_OPERAND_MODE, "", 0xD4, 0x01, 0x82, 0x03);
    // STM/LDM/PGBN with an empty register list (mask 0) is not valid: the
    // assembler requires at least one register, so it can't round-trip.
    ERRT("LDM.W", "@R3, ", ILLEGAL_OPERAND_MODE, "", 0x75, 0x03, 0x00, 0x00);
    ERRT("STM.W", ", @R3", ILLEGAL_OPERAND_MODE, "", 0x71, 0x03, 0x00, 0x00);
    ERRT("PGBN",  "",      ILLEGAL_OPERAND_MODE, "", 0xE9, 0x00, 0x00);
    // Bit-field operands forbid immediate and auto-inc/dec (manual Available
    // EA), on both the source and the destination.
    ERRT("BFEXT", "R1, R2, @R3, #5:8", ILLEGAL_OPERAND_MODE, "",
            0xD4, 0x12, 0x03, 0x71, 0x05);
    ERRT("BFEXT", "R1, R2, @R3+, @R4", ILLEGAL_OPERAND_MODE, "",
            0xD4, 0x12, 0x53, 0x04);
    // The EA byte's bit 7 is the accumulator marker, valid only on the source
    // EA of a two-EA instruction.  Set on a single-operand EA (NOT) or on a
    // destination byte (ADDS dst), nothing consumes it -- illegal.
    ERRT("NOT.B",  "@R9", ILLEGAL_OPERAND_MODE, "", 0x90, 0x89);
    ERRT("ADDS.L", "@R3+, @(-$77, R0)", ILLEGAL_OPERAND_MODE, "",
            0x42, 0x53, 0x90, 0x89);
    // JMP/JSR target forbids register-direct and immediate (manual Available
    // EA): both have no effective address to jump to.
    ERRT("JMP", "R0",   ILLEGAL_OPERAND_MODE, "", 0x9B, 0x40);
    ERRT("JMP", "#5:8", ILLEGAL_OPERAND_MODE, "", 0x9B, 0x71, 0x05);
    ERRT("JSR", "R0",   ILLEGAL_OPERAND_MODE, "", 0xAB, 0x40);
    // STM/LDM memory operand: register-direct is illegal (manual Available EA).
    ERRT("STM.W", "R0-R3, R4", ILLEGAL_OPERAND_MODE, "", 0x71, 0x44, 0x00, 0x0F);
    ERRT("LDM.W", "R4, R0-R3", ILLEGAL_OPERAND_MODE, "", 0x75, 0x44, 0x00, 0x0F);
    // MOVTP/MOVFP peripheral operand: register-direct and auto-inc/dec illegal.
    ERRT("MOVTP.W", "R0, R1",   ILLEGAL_OPERAND_MODE, "", 0xE0, 0x40, 0x41);
    ERRT("MOVFP.W", "@R1+, R0", ILLEGAL_OPERAND_MODE, "", 0xE2, 0x51, 0x40);
}

// clang-format on

void run_tests(const char *cpu) {
    disassembler.setCpu(cpu);
    RUN_TEST(test_system);
    RUN_TEST(test_arith);
    RUN_TEST(test_compare);
    RUN_TEST(test_reg_list);
    RUN_TEST(test_string);
    RUN_TEST(test_bitfield);
    RUN_TEST(test_data_move);
    RUN_TEST(test_logic);
    RUN_TEST(test_shift_bit);
    RUN_TEST(test_mul_div);
    RUN_TEST(test_extend);
    RUN_TEST(test_branch);
    RUN_TEST(test_jump);
    RUN_TEST(test_ea_size);
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

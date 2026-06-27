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

#include "asm_h16.h"
#include "test_asm_helper.h"

using namespace libasm;
using namespace libasm::h16;
using namespace libasm::test;

AsmH16 asmh16;
Assembler &assembler(asmh16);

void set_up() {
    assembler.reset();
}

void tear_down() {
    symtab.reset();
}

// clang-format off

void test_cpu() {
    EQUALS("cpu H16", true, assembler.setCpu("H16"));
    EQUALS_P("cpu H16", "H16", assembler.config().cpu_P());
}

// System / no-op / exception / bank-control instructions.
void test_system() {
    TEST("NOP",   0xFF);
    TEST("RTS",   0xBB);
    TEST("RTE",   0xF1);
    TEST("RTR",   0xF4);
    TEST("RESET", 0xF0);
    TEST("SLEEP", 0xF5);
    TEST("ICBN",    0xFD);
    TEST("ICBN.L",  0xFD);  // no operand, but manual "Operand Size: Long word"
    TEST("DCBN",    0xFE);
    TEST("DCBN.L",  0xFE);
    TEST("CGBN R1",   0xE4, 0x01);
    TEST("TRAPA #5",  0xF2, 0x05);
    TEST("TRAPA #0",  0xF2, 0x00);
    TEST("TRAPA #15", 0xF2, 0x0F);
    TEST("TAS @R0",   0xEC, 0x00);
    TEST("TAS @R1",   0xEC, 0x01);
    TEST("TAS.B @R0", 0xEC, 0x00);  // manual "Operand Size: Byte" -> optional .B
    // /cc condition-code instructions.
    TEST("TRAP/T",    0xF3, 0x00);
    TEST("TRAP/EQ",   0xF3, 0x07);
    TEST("SET/NE @R0",0xB7, 0x06, 0x00);
    TEST("SET/EQ @R1",0xB7, 0x07, 0x01);
    TEST("SET/EQ.B @R1", 0xB7, 0x07, 0x01);  // byte -> optional .B (after /cc)
    ATEST(0x1000, "SCB/EQ.B R1, $1003", 0xB4, 0x17, 0x00);
    ATEST(0x1000, "SCB/EQ.W R1, $1010", 0xB5, 0x17, 0x00, 0x0C);
    // JMP/JSR target: any EA except register-direct and immediate (manual
    // Available EA forbids Rn and #imm).
    TEST("JMP @R0",       0x9B, 0x00);
    TEST("JMP @$1234:16", 0x9B, 0x76, 0x12, 0x34);
    TEST("JSR @R0",       0xAB, 0x00);
}

// Arithmetic.
void test_arith() {
    // ADD:G register direct (.B/.W/.L)
    TEST("ADD:G.B R0, R1", 0x00, 0x40, 0x41);
    TEST("ADD:G.W R0, R1", 0x01, 0x40, 0x41);
    TEST("ADD:G.L R0, R1", 0x02, 0x40, 0x41);
    // Accumulator form (dst = R0)
    TEST("ADD:G.W R1, R0", 0x01, 0xC1);
    // Register indirect
    TEST("ADD:G.W @R0, R1", 0x01, 0x00, 0x41);
    // ADD:Q  quick (8-bit signed imm, EAd)
    TEST("ADD:Q.B #16, R0",  0x10, 0x10, 0x40);
    TEST("ADD:Q.W #16, R0",  0x11, 0x10, 0x40);
    TEST("ADD:Q.L #16, R0",  0x12, 0x10, 0x40);
    // ADD:R / SUB:R register-register
    TEST("ADD:R.B R0, R1", 0x20, 0x01);
    TEST("SUB:R.B R0, R1", 0x24, 0x01);
    // ADD:RQ register-quick; imm4 is unsigned 0..15 (zero-extended) per manual.
    TEST("ADD:RQ.B #5, R0",  0x30, 0x05);
    TEST("ADD:RQ.B #15, R0", 0x30, 0x0F);
    // Sign-extend / CX
    TEST("ADDS.B R0, R1", 0x40, 0x40, 0x41);
    TEST("ADDX.B R0, R1", 0x50, 0x40, 0x41);
    // NEG / NEGX
    TEST("NEG.B R0",  0x8C, 0x40);
    TEST("NEGX.B R0", 0x9C, 0x40);
    // Decimal: manual "Operand Size: Byte" -- bare or an explicit ".B".
    TEST("DADD R0, R1",   0x83, 0x40, 0x41);
    TEST("DADD.B R0, R1", 0x83, 0x40, 0x41);
    TEST("DSUB R0, R1",   0x87, 0x40, 0x41);
    TEST("DSUB.B R0, R1", 0x87, 0x40, 0x41);
    TEST("DNEG R0",       0xAF, 0x40);
    TEST("DNEG.B R0",     0xAF, 0x40);
}

// Compare and test.
void test_compare() {
    TEST("CMP:G.B R0, R1",  0x08, 0x40, 0x41);
    TEST("CMP:Q.B #16, R0", 0x18, 0x10, 0x40);
    TEST("CMP:R.B R0, R1",  0x28, 0x01);
    // CMP:RQ imm4 is signed -8..7 (sign-extended) per the manual.
    TEST("CMP:RQ.B #5, R0",  0x38, 0x05);
    TEST("CMP:RQ.B #7, R0",  0x38, 0x07);
    TEST("CMP:RQ.B #-8, R0", 0x38, 0x08);
    TEST("CMP:RQ.B #-1, R1", 0x38, 0x1F);
    TEST("CMPS.B R0, R1",   0x48, 0x40, 0x41);
    TEST("TST.B R0",        0x58, 0x40);
    // Accumulator form (dst = R0): src EA bit 7 set, no dst byte.  CMP:G and
    // CMPS carry the second operand as M_EASRC, so this exercises the EASRC
    // accumulator path on both the assembler and (round-trip) disassembler.
    TEST("CMP:G.W @R1, R0", 0x09, 0x81);
    TEST("CMPS.W @R1, R0",  0x49, 0x81);
}

// Data movement.
void test_data_move() {
    TEST("MOV:G.B R0, R1",  0x0C, 0x40, 0x41);
    TEST("MOV:Q.B #16, R0", 0x1C, 0x10, 0x40);
    TEST("MOV:R.B R0, R1",  0x2C, 0x01);
    // MOV:RQ imm4 is signed -8..7 (sign-extended) per the manual.
    TEST("MOV:RQ.B #5, R0",  0x3C, 0x05);
    TEST("MOV:RQ.B #-3, R0", 0x3C, 0x0D);
    TEST("MOVS.B R0, R1",   0x4C, 0x40, 0x41);
    TEST("MOVF.B R1",       0x5C, 0x41);
    TEST("CLR.B R0",        0x14, 0x40);
    TEST("CLR.W R0",        0x15, 0x40);
    // XCH / MOVA: manual "Operand Size: Long word" -- bare or an explicit ".L".
    TEST("XCH R0, R1",      0xB3, 0x01);
    TEST("XCH.L R0, R1",    0xB3, 0x01);
    TEST("MOVA @R0, R1",    0xBF, 0x00, 0x41);
    TEST("MOVA.L @R0, R1",  0xBF, 0x00, 0x41);
    // SWAP: byte/word half-swap via opcode bit 0 (manual Note: S=0 byte, 1 word).
    TEST("SWAP.B R0",       0xEA, 0x40);
    TEST("SWAP.W R0",       0xEB, 0x40);
    // MOVTP / MOVFP: word/long via opcode bit 0 (manual: S=0 word, 1 long).
    TEST("MOVTP.W R0, @R1", 0xE0, 0x40, 0x01);
    TEST("MOVTP.L R0, @R1", 0xE1, 0x40, 0x01);
    TEST("MOVFP.W @R1, R2", 0xE2, 0x01, 0x42);
    TEST("MOVFP.L @R1, R2", 0xE3, 0x01, 0x42);
    // MOVTPE / MOVFPE: full .B/.W/.L (2-bit Sz).
    TEST("MOVTPE.B R0, @R1", 0x78, 0x40, 0x01);
    TEST("MOVTPE.W R0, @R1", 0x79, 0x40, 0x01);
    TEST("MOVTPE.L R0, @R1", 0x7A, 0x40, 0x01);
    TEST("MOVFPE.W @R1, R2", 0x7D, 0x01, 0x42);
}

// Logical / NOT.
void test_logic() {
    TEST("AND.B R0, R1", 0x80, 0x40, 0x41);
    TEST("OR.B R0, R1",  0x88, 0x40, 0x41);
    TEST("XOR.B R0, R1", 0x84, 0x40, 0x41);
    TEST("NOT.B R0",     0x90, 0x40);
}

// Sign/zero extend.
void test_extend() {
    TEST("EXTU.W R0", 0xAC, 0x00);
    TEST("EXTU.L R0", 0xAD, 0x00);
    TEST("EXTU.B R0", 0xAE, 0x00);
    TEST("EXTS.W R0", 0xBC, 0x00);
    TEST("EXTS.L R0", 0xBD, 0x00);
    TEST("EXTS.B R0", 0xBE, 0x00);
}

// Stack-frame.
void test_frame() {
    TEST("LINK.B R0, #16",    0xD0, 0x00, 0x10);
    TEST("LINK.W R7, #-$100", 0xD1, 0x07, 0xFF, 0x00);
    TEST("LINK.L R0, #16",    0xD2, 0x00, 0x00, 0x00, 0x00, 0x10);
    TEST("UNLK R0",           0xD3, 0x00);
    TEST("UNLK R7",           0xD3, 0x07);
    TEST("RTD.B #16",         0xB8, 0x10);
    TEST("RTD.W #$0200",      0xB9, 0x02, 0x00);
    TEST("RTD.L #16",         0xBA, 0x00, 0x00, 0x00, 0x10);
    // LINK/RTD frame size auto-sizes when unsuffixed (manual: byte/word/long,
    // no suffix shown): smallest form that holds the value.
    TEST("RTD #16",           0xB8, 0x10);              // -> byte
    TEST("RTD #$200",         0xB9, 0x02, 0x00);        // -> word
    TEST("RTD #$10000",       0xBA, 0x00, 0x01, 0x00, 0x00);  // -> long
    TEST("LINK R0, #16",      0xD0, 0x00, 0x10);        // -> byte
    TEST("LINK R15, #$10000", 0xD2, 0x0F, 0x00, 0x01, 0x00, 0x00);  // -> long
}

// Shift, rotate, and bit-manipulation (2-byte opcodes via prefix pages).
void test_shift_bit() {
    // SFT dynamic
    TEST("SHAR.B R2, R1",  0x60, 0x02, 0x41);
    TEST("SHLR.B R2, R1",  0x60, 0x22, 0x41);
    TEST("ROTR.B R2, R1",  0x60, 0x42, 0x41);
    TEST("ROTXR.B R2, R1", 0x60, 0x62, 0x41);
    TEST("SHAL.B R2, R1",  0x60, 0x82, 0x41);
    TEST("SHAL.W R2, R1",  0x61, 0x82, 0x41);
    TEST("SHAL.L R2, R1",  0x62, 0x82, 0x41);
    // SFT static
    TEST("SHAL.B #3, R1",  0x64, 0x83, 0x41);
    // BIT dynamic
    TEST("BSET.B R2, @R1", 0x68, 0x02, 0x01);
    TEST("BNOT.B R2, @R1", 0x68, 0x22, 0x01);
    TEST("BCLR.B R2, @R1", 0x68, 0x42, 0x01);
    TEST("BTST.B R2, @R1", 0x68, 0x62, 0x01);
    // BIT static
    TEST("BSET.B #3, @R1", 0x6C, 0x03, 0x01);
}

// Multiply and divide.
void test_mul_div() {
    TEST("MULXS.B R0, R1", 0xEE, 0x00, 0x40, 0x41);
    TEST("MULXS.W R0, R1", 0xEE, 0x10, 0x40, 0x41);
    TEST("MULXU.B R0, R1", 0xEE, 0x40, 0x40, 0x41);
    TEST("DIVXS.B R0, R1", 0xEF, 0x00, 0x40, 0x41);
    TEST("DIVXU.B R0, R1", 0xEF, 0x40, 0x40, 0x41);
}

// Control-register access.
void test_cr() {
    TEST("ANDC R0, CCR",  0xF8, 0x20, 0x40);
    TEST("ANDC R0, SR",   0xF8, 0xA0, 0x40);
    TEST("ORC R0, CCR",   0xF9, 0x20, 0x40);
    TEST("XORC R0, CCR",  0xFA, 0x20, 0x40);
    TEST("LDC R0, CBNR",  0xFB, 0x40, 0x40);
    TEST("STC CBNR, R0",  0xFC, 0x40, 0x40);
}

// Various EA addressing modes via ADD:G.W.
void test_ea_modes() {
    // Register indirect (no disp)
    TEST("ADD:G.W @R0, R1",           0x01, 0x00, 0x41);
    // Register indirect with 8-bit displacement
    TEST("ADD:G.W @($10:8, R0), R1",  0x01, 0x10, 0x10, 0x41);
    // 16-bit displacement
    TEST("ADD:G.W @($1000:16, R0), R1", 0x01, 0x20, 0x10, 0x00, 0x41);
    // Auto-increment / decrement
    TEST("ADD:G.W @R0+, R1", 0x01, 0x50, 0x41);
    TEST("ADD:G.W @-R0, R1", 0x01, 0x60, 0x41);
    // Immediate
    TEST("ADD:G.W #$12:8, R0",   0x01, 0xF1, 0x12);
    TEST("ADD:G.W #$1234:16, R0", 0x01, 0xF2, 0x12, 0x34);
    // Absolute
    TEST("ADD:G.W @$12:8, R0",    0x01, 0xF5, 0x12);
    TEST("ADD:G.W @$1234:16, R0", 0x01, 0xF6, 0x12, 0x34);
    // Index mode: the register carrying .Sz/*Sf is the index (Xm); with
    // neither, first=base, second=index (matches the no-disp form and ASL).
    TEST("ADD:G.W @(R1, R2), R3",        0x01, 0x7C, 0x41, 0x02, 0x43);
    TEST("ADD:G.W @($10, R1, R2), R3",   0x01, 0x7C, 0x51, 0x02, 0x10, 0x43);
    TEST("ADD:G.W @($10, R1.W, R2), R3", 0x01, 0x7C, 0x12, 0x01, 0x10, 0x43);
    TEST("ADD:G.W @($10, R1, R2.W), R3", 0x01, 0x7C, 0x11, 0x02, 0x10, 0x43);
    // Index scale "*N": factors 2/4/8 set Sf in the ext byte.  An invalid
    // factor (*16, *3) is rejected -- parseScale leaves it for the ')' check
    // rather than silently mis-parsing (*16 must not become scale 1).
    TEST("ADD:G.W @(R1.W*2, R2), R3", 0x01, 0x7C, 0x02, 0x11, 0x43);
    TEST("ADD:G.W @(R1.W*4, R2), R3", 0x01, 0x7C, 0x02, 0x21, 0x43);
    TEST("ADD:G.W @(R1.W*8, R2), R3", 0x01, 0x7C, 0x02, 0x31, 0x43);
    ERRT("ADD:G.W @(R1.W*16, R2), R3", MISSING_CLOSING_PAREN, "@(R1.W*16, R2), R3");
    ERRT("ADD:G.W @(R1.W*3, R2), R3",  MISSING_CLOSING_PAREN, "@(R1.W*3, R2), R3");
}

// H8/300-family data/space directives: .DATA[.B/.W/.L], .SDATA, .RES[.B/.W/.L].
void test_data_constant() {
    // .data.b: byte values (strings go through .sdata, not here).
    TEST(".data.b $12",            0x12);
    TEST(".data.b $12, $34, $56",  0x12, 0x34, 0x56);
    ERRT(".data.b 'Hi'", ILLEGAL_CONSTANT, "'Hi'");
    // .data.w: big-endian 16-bit values.
    TEST(".data.w $1234",          0x12, 0x34);
    TEST(".data.w $1234, $5678",   0x12, 0x34, 0x56, 0x78);
    // .data.l: big-endian 32-bit values.
    TEST(".data.l $12345678",      0x12, 0x34, 0x56, 0x78);
    TEST(".data.l 1",              0x00, 0x00, 0x00, 0x01);
    // .data: defaults to word, big-endian.
    TEST(".data $ABCD",            0xAB, 0xCD);
    // .sdata: raw string bytes; comma-separated operands concatenate.
    TEST(".sdata 'Hi'",            0x48, 0x69);
    TEST(".sdata 'A', 'BC'",       0x41, 0x42, 0x43);
    // .res family: reserve space (no output) in byte/word/long units.
    TEST(".res.b 4");
    TEST(".res.w 1");
    TEST(".res.l 1");
    TEST(".res 2");
    TEST(".res.b 0");
}

// Multiple-register transfer (register lists) and bank pull.
void test_reg_list() {
    TEST("STM R0-R3, @R4",   0x71, 0x04, 0x00, 0x0F);  // .W default
    TEST("STM.B R0-R3, @R4", 0x70, 0x04, 0x00, 0x0F);
    TEST("STM.L R0-R15, @R4",0x72, 0x04, 0xFF, 0xFF);
    TEST("STM R5, @R4",      0x71, 0x04, 0x00, 0x20);  // single register
    // ASL spells a list as comma-separated single registers and ranges.
    TEST("STM R0, R2, R5, @R4",        0x71, 0x04, 0x00, 0x25);  // non-contiguous
    TEST("STM.L R0, R3-R12, R15, @R4", 0x72, 0x04, 0x9F, 0xF9);  // mixed ranges
    TEST("LDM @R4, R0-R3",   0x75, 0x04, 0x00, 0x0F);
    TEST("LDM @R4, R0, R3-R12, R15",   0x75, 0x04, 0x9F, 0xF9);
    TEST("STM R0-R3, @-R4",  0x71, 0x64, 0x00, 0x0F);
    TEST("PGBN",             0xE8);
    TEST("PGBN.B",           0xE8);                     // optional .B accepted (ASL)
    TEST("PGBN R0-R3",       0xE9, 0x00, 0x0F);
    TEST("PGBN.B R0-R3, R8", 0xE9, 0x01, 0x0F);
}

// Bit-field instructions.
void test_bitfield() {
    TEST("BFEXT R0, R1, @R2, R3",  0xD4, 0x01, 0x02, 0x43);
    TEST("BFINS R0, R1, R2, @R3",  0xD5, 0x01, 0x42, 0x03);
    TEST("BFSCH R0, R1, @R2, R3",  0xD6, 0x01, 0x02, 0x43);
    TEST("BFMOV R0, R1, R2, R3, R4, R5", 0xD7, 0x01, 0x23, 0x45);
}

// String instructions: /F|/B direction, /cc on the 4-register forms.
void test_string() {
    TEST("SMOV/F R0, R1, R2",     0x95, 0x22, 0x01);  // .W default
    TEST("SMOV/B R0, R1, R2",     0x95, 0x52, 0x01);
    TEST("SMOV/F.B R0, R1, R2",   0x94, 0x22, 0x01);
    TEST("SSTR/F R0, R1, R2",     0x95, 0x02, 0x01);
    TEST("SSTR/B R0, R1, R2",     0x95, 0x42, 0x01);
    TEST("SCMP/EQ/F R0, R1, R2, R3", 0x95, 0xA2, 0x01, 0x37);
    TEST("SCMP/EQ/B R0, R1, R2, R3", 0x95, 0xD2, 0x01, 0x37);
    TEST("SSCH/NE/F R0, R1, R2, R3", 0x95, 0x82, 0x01, 0x36);
    TEST("SSCH/NE/B R0, R1, R2, R3", 0x95, 0xC2, 0x01, 0x36);
}

// Class-less mnemonics: the assembler picks the smallest fitting InsnClass
// (matching ASL), and a missing size suffix defaults to .W.
void test_optimal_class() {
    TEST("ADD #5, R2",      0x31, 0x25);                   // -> ADD:RQ.W
    TEST("ADD #5, @R2",     0x11, 0x05, 0x02);             // -> ADD:Q.W  (dst memory)
    TEST("ADD #200, R2",    0x01, 0x72, 0x00, 0xC8, 0x42); // -> ADD:G.W  (imm needs 16 bits)
    TEST("ADD R1, R2",      0x21, 0x12);                   // -> ADD:R.W
    TEST("ADD @R1, R2",     0x01, 0x01, 0x42);             // -> ADD:G.W
    TEST("SUB #5, R2",      0x35, 0x25);                   // -> SUB:RQ.W
    TEST("CMP R1, R2",      0x29, 0x12);                   // -> CMP:R.W
    TEST("MOV #5, R2",      0x3D, 0x25);                   // -> MOV:RQ.W
    // CMP/MOV :RQ imm4 is signed -8..7, so a small value picks :RQ but one
    // outside that range falls through to :Q -- matching ASL.
    TEST("CMP #5, R2",      0x39, 0x25);                   // -> CMP:RQ.W (5 fits -8..7)
    TEST("CMP #-1, R2",     0x39, 0x2F);                   // -> CMP:RQ.W (-1 fits)
    TEST("CMP #10, R2",     0x19, 0x0A, 0x42);             // -> CMP:Q.W  (10 > 7)
    TEST("MOV #-1, R2",     0x3D, 0x2F);                   // -> MOV:RQ.W
    // Class-less with an explicit size keeps the optimal class.
    TEST("ADD.B #5, R2",    0x30, 0x25);                   // -> ADD:RQ.B
    // Explicit class is still honored exactly.
    TEST("ADD:G.W R1, R0",  0x01, 0xC1);
    TEST("ADD:Q.W #16, R0", 0x11, 0x10, 0x40);
}

// smart-branch option: a size-less branch auto-sizes its displacement by
// distance when on, and defaults to .W when off.
void test_smart_branch() {
    TEST("option smart-branch, off");
    ATEST(0x1000, "BRA $1004", 0x99, 0x00, 0x01);  // .W default
    TEST("option smart-branch, on");
    ATEST(0x1000, "BRA $1004", 0x98, 0x02);        // -> .B (reaches in a byte)
    ATEST(0x1000, "BRA $2000", 0x99, 0x0F, 0xFD);  // -> .W
    ATEST(0x1000, "BHI $1004", 0xA4, 0x02, 0x01);  // Bcc:G -> .B
    // HS/LO (Table 16-8) are accepted as aliases of CC/CS (same cc 4/5).
    ATEST(0x1000, "BHS:G.B $1003", 0xA4, 0x04, 0x00);
    ATEST(0x1000, "BLO:G.B $1003", 0xA4, 0x05, 0x00);
    // SCB is a branch too: its displacement auto-sizes by distance.
    ATEST(0x1000, "SCB/EQ R1, $1003",  0xB4, 0x17, 0x00);              // -> .B
    ATEST(0x1000, "SCB/EQ R1, $2000",  0xB5, 0x17, 0x0F, 0xFC);        // -> .W
    ATEST(0x1000, "SCB/EQ R1, $40000", 0xB6, 0x17, 0x00, 0x03, 0xEF, 0xFA);  // -> .L
    TEST("option smart-branch, off");
    ATEST(0x1000, "SCB/EQ R1, $1003",  0xB5, 0x17, 0xFF, 0xFF);  // .W default
}

// Error cases: bad sizes, out-of-range PC-relative displacements,
// invalid addressing modes, and unknown mnemonics.
void test_errors() {
    // A malformed .size suffix is reported as ILLEGAL_SIZE (from the mnemonic
    // parser), not masked as UNKNOWN_INSTRUCTION.
    ERRT("ADD:G.    R0, R1",    ILLEGAL_SIZE, "");
    ERRT("ADD:G.W.B R0, R1",    ILLEGAL_SIZE, ".B");
    // No-suffix instructions reject any size; bit0-sized SWAP rejects .L.
    ERRT("XCH.W   R0, R1",      ILLEGAL_SIZE, "");
    ERRT("SWAP.L  R0",          ILLEGAL_SIZE, "");
    ERRT("DNEG.W  R0",          ILLEGAL_SIZE, "");
    // MOVTP/MOVFP are word/long only (opcode bit 0); .B is illegal.
    ERRT("MOVTP.B R0, @R1",     ILLEGAL_SIZE, "");
    ERRT("MOVFP.B @R1, R0",     ILLEGAL_SIZE, "");
    // Byte ops (TAS/SET) reject .W/.L; long ops (MOVA) reject .B/.W -- only the
    // operand's natural size or a bare mnemonic is allowed.
    ERRT("TAS.W   @R0",         ILLEGAL_SIZE, "");
    ERRT("SET/EQ.L @R0",        ILLEGAL_SIZE, "");
    ERRT("MOVA.B  @R0, R1",     ILLEGAL_SIZE, "");
    ERRT("MOVA.W  @R0, R1",     ILLEGAL_SIZE, "");
    // ICBN/DCBN are long word: .L ok, .B/.W not.
    ERRT("ICBN.W",              ILLEGAL_SIZE, "");
    ERRT("DCBN.B",              ILLEGAL_SIZE, "");
    // No-size instructions reject any suffix (the ICBN/DCBN .L exception is by
    // opcode, so NOP/RTS/JMP are unaffected).
    ERRT("NOP.B",               ILLEGAL_SIZE, "");
    ERRT("RTS.W",               ILLEGAL_SIZE, "");
    ERRT("JMP.L   @R0",         ILLEGAL_SIZE, "");
    // JMP/JSR forbid register-direct (no effective address) and immediate.
    ERRT("JMP R0",              OPERAND_NOT_ALLOWED, "");
    ERRT("JMP #5",              OPERAND_NOT_ALLOWED, "");
    ERRT("JSR R0",              OPERAND_NOT_ALLOWED, "");
    ERRT("JSR #5",              OPERAND_NOT_ALLOWED, "");
    // STM/LDM operate on memory: the EA forbids register-direct and immediate.
    ERRT("STM.W R0-R3, R5",     OPERAND_NOT_ALLOWED, "");
    ERRT("STM.W R0-R3, #5",     OPERAND_NOT_ALLOWED, "");
    ERRT("LDM.W R5, R0-R3",     OPERAND_NOT_ALLOWED, "");
    ERRT("LDM.W #5, R0-R3",     OPERAND_NOT_ALLOWED, "");
    // MOVTP/MOVFP peripheral operand also forbids auto-inc/dec (the move
    // post-modifies the address itself), plus register-direct and immediate.
    ERRT("MOVTP.W R0, R1",      OPERAND_NOT_ALLOWED, "");
    ERRT("MOVTP.W R0, @R1+",    OPERAND_NOT_ALLOWED, "");
    ERRT("MOVTPE.B R0, @-R1",   OPERAND_NOT_ALLOWED, "");
    ERRT("MOVFP.W @R1+, R0",    OPERAND_NOT_ALLOWED, "");
    ERRT("MOVFP.W #5, R0",      OPERAND_NOT_ALLOWED, "");
    // PGBN's operand size is byte: .B is accepted but .W / .L are not.
    ERRT("PGBN.W",              ILLEGAL_SIZE, "");
    ERRT("PGBN.L R0-R3",        ILLEGAL_SIZE, "");
    // MUL/DIV only accept .B / .W (no .L).
    ERRT("MULXS.L R0, R1",      ILLEGAL_SIZE, "");
    ERRT("DIVXS.L R0, R1",      ILLEGAL_SIZE, "");
    // String ops are byte/word only; .L would emit an undecodable prefix.
    ERRT("SMOV/F.L R0, R1, R2",     ILLEGAL_SIZE, "");
    ERRT("SCMP/NE/B.L R0, R1, R2, R3", ILLEGAL_SIZE, "");
    // A register past the form's count (3 for SMOV/SSTR, 4 for SCMP/SSCH) is
    // rejected, not silently dropped.
    ERRT("SMOV/F.B R0, R1, R2, R3",      OPERAND_NOT_ALLOWED, "");
    ERRT("SCMP/NE/B.B R0, R1, R2, R3, R4", OPERAND_NOT_ALLOWED, "");
    // At most one /cc condition and one /F|/B direction; a repeated suffix is
    // rejected (matching ASL), not silently last-wins.
    ERRT("SCB/EQ/NE.B R1, *",         UNKNOWN_INSTRUCTION, "SCB/EQ/NE.B R1, *");
    ERRT("SCMP/EQ/NE/F.B R0, R1, R2, R3", UNKNOWN_INSTRUCTION, "SCMP/EQ/NE/F.B R0, R1, R2, R3");
    ERRT("SMOV/F/B.B R0, R1, R2",     UNKNOWN_INSTRUCTION, "SMOV/F/B.B R0, R1, R2");
    // Class / cc suffixes are matched as whole tokens (word boundary): a class
    // or cc with a trailing letter is rejected, not matched as a prefix.
    ERRT("ADD:GX R0, R1",       UNKNOWN_INSTRUCTION, "ADD:GX R0, R1");
    ERRT("ADD:RQX R0, R1",      UNKNOWN_INSTRUCTION, "ADD:RQX R0, R1");
    ERRT("SCB/FF.B R1, $0",     UNKNOWN_INSTRUCTION, "SCB/FF.B R1, $0");
    ERRT("SCB/EQX.B R1, $0",    UNKNOWN_INSTRUCTION, "SCB/EQX.B R1, $0");
    // Trailing garbage after a :N size qualifier / .size suffix is rejected.
    ERRT("ADD:G.W #$12:8X, R0", OPERAND_NOT_ALLOWED, "");
    ERRT("ADD:G.B- R0, R1",     OPERAND_NOT_ALLOWED, "");
    // PC-relative explicit :8 / :16 with a delta that doesn't fit at this
    // address: OVERFLOW_RANGE rather than silently truncating.
    AERRT(0x1000, "ADD:G.B @R1, @($2000:8, PC)",   OVERFLOW_RANGE,
            "@($2000:8, PC)",   0x00, 0x01, 0x7D, 0x90, 0xFB);
    AERRT(0x1000, "ADD:G.B @R1, @($10000:16, PC)", OVERFLOW_RANGE,
            "@($10000:16, PC)", 0x00, 0x01, 0x7D, 0xA0, 0xEF, 0xFA);
    // A forward (undefined) PC-relative ref uses base as the target, so its
    // displacement field is zero (and not range-checked) until the label
    // resolves -- even at a high address, where a delta vs an unresolved 0 would
    // otherwise falsely overflow.
    AERUS(0x1000, "ADD:G.B @R1, @(undef:8, PC)", "undef:8, PC)",
            0x00, 0x01, 0x7D, 0x90, 0x00);
    AERUS(0x1000, "ADD:G.W @R1, @(undef:16, PC)", "undef:16, PC)",
            0x01, 0x01, 0x7D, 0xA0, 0x00, 0x00);
    // Disp / immediate / absolute fields are sign-extended (manual), so an
    // explicit :N is a SIGNED range: a value outside [-2^(N-1), 2^(N-1)-1] is
    // OVERFLOW_RANGE.  128 / 200 / 255 do NOT fit :8 (they read back negative).
    ERRT("ADD:G.W #$100:8, R0",         OVERFLOW_RANGE, "#$100:8, R0",
            0x01, 0xF1, 0x00);
    ERRT("ADD:G.W #$80:8, R0",          OVERFLOW_RANGE, "#$80:8, R0",
            0x01, 0xF1, 0x80);
    ERRT("ADD:G.W @$1234:8, R0",        OVERFLOW_RANGE, "@$1234:8, R0",
            0x01, 0xF5, 0x34);
    ERRT("ADD:G.W @($1234:8, R0), R1",  OVERFLOW_RANGE, "@($1234:8, R0), R1",
            0x01, 0x10, 0x34, 0x41);
    ERRT("ADD:G.W @(200:8, R0), R1",    OVERFLOW_RANGE, "@(200:8, R0), R1",
            0x01, 0x10, 0xC8, 0x41);
    // The signed extremes of each :N field are accepted.
    TEST("ADD:G.W #127:8, R0",          0x01, 0xF1, 0x7F);
    TEST("ADD:G.W #-128:8, R0",         0x01, 0xF1, 0x80);
    TEST("ADD:G.W @(-128:8, R0), R1",   0x01, 0x10, 0x80, 0x41);
    // Explicit class with an out-of-range immediate is rejected rather than
    // silently truncating.  :RQ imm4 is unsigned 0..15 for ADD/SUB but signed
    // -8..7 for CMP/MOV, so each rejects what the other accepts.
    ERRT("ADD:RQ.W #200, R0",   OPERAND_NOT_ALLOWED, "");
    ERRT("ADD:RQ.W #-1, R0",    OPERAND_NOT_ALLOWED, "");  // unsigned: no negatives
    ERRT("ADD:RQ.W #16, R0",    OPERAND_NOT_ALLOWED, "");  // unsigned: max 15
    ERRT("CMP:RQ.W #8, R0",     OPERAND_NOT_ALLOWED, "");  // signed: max 7
    ERRT("CMP:RQ.W #-9, R0",    OPERAND_NOT_ALLOWED, "");  // signed: min -8
    ERRT("MOV:RQ.W #8, R0",     OPERAND_NOT_ALLOWED, "");
    // Unknown mnemonic / non-existent register.
    ERRT("FOO     R0, R1",      UNKNOWN_INSTRUCTION, "FOO     R0, R1");
    ERRT("ADD:G.W R16, R0",     OPERAND_NOT_ALLOWED, "");
    // An immediate can't be a write destination -- the disassembler rejects it
    // (destOnly), so the assembler must too (no asm/dis mismatch).
    ERRT("ADD:G.W R0, #5",      OPERAND_NOT_ALLOWED, "");
    ERRT("CLR.W   #5",          OPERAND_NOT_ALLOWED, "");
    ERRT("NEG.W   #5",          OPERAND_NOT_ALLOWED, "");
    // BFEXT/BFINS/BFSCH bit-field operands (both EAs and EAd) forbid immediate
    // and auto-inc/dec (manual Available EA); register-direct + memory are OK.
    ERRT("BFEXT R0, R1, @R2, #5",   OPERAND_NOT_ALLOWED, "");  // imm EAd
    ERRT("BFEXT R0, R1, #5, @R2",   OPERAND_NOT_ALLOWED, "");  // imm EAs
    ERRT("BFEXT R0, R1, @R2+, @R3", OPERAND_NOT_ALLOWED, "");  // auto-inc EAs
    ERRT("BFINS R0, R1, @R2, @R3+", OPERAND_NOT_ALLOWED, "");  // auto-inc EAd
    ERRT("BFSCH R0, R1, @-R2, @R3", OPERAND_NOT_ALLOWED, "");  // auto-dec EAs
    // A /cc or /F|/B suffix on an instruction that takes neither is unknown,
    // not a silently-ignored decoration.
    ERRT("ADD/EQ R0, R1",       UNKNOWN_INSTRUCTION, "ADD/EQ R0, R1");
    ERRT("MOV/B  R0, R1",       UNKNOWN_INSTRUCTION, "MOV/B  R0, R1");
    ERRT("NOP/EQ",              UNKNOWN_INSTRUCTION, "NOP/EQ");
    ERRT("TAS/F  @R0",          UNKNOWN_INSTRUCTION, "TAS/F  @R0");
    // Trailing text after the operands is not an implicit comment (H16 uses
    // ';'), so it is rejected, not silently ignored.
    ERRT("ADD:G.W R0, R1 junk", GARBAGE_AT_END, "junk");
    ERRT("TAS @R0 junk",        GARBAGE_AT_END, "junk");
    ERRT("BRA $0 junk",         GARBAGE_AT_END, "junk");
    // An explicit-size branch that can't reach its target overflows rather than
    // silently truncating the displacement (smart-branch picks a wider form,
    // and .L always reaches).
    AERRT(0x1000, "BRA.B $200000", OVERFLOW_RANGE, "$200000", 0x98, 0xFE);
    AERRT(0x1000, "BRA.W $200000", OVERFLOW_RANGE, "$200000", 0x99, 0xEF, 0xFD);
    AERRT(0x1000, "BSR.B $200000", OVERFLOW_RANGE, "$200000", 0xA8, 0xFE);
    // Fixed-width quick fields (TRAP vector = 4 bits, static shift/bit count =
    // 5 bits, frame size = sized) reject an out-of-range value instead of
    // silently masking/truncating it to the field.
    ERRT("TRAPA #16",       OVERFLOW_RANGE, "#16",       0xF2, 0x00);
    ERRT("SHAR.B #32, R0",  OVERFLOW_RANGE, "#32, R0",   0x64, 0x00, 0x40);
    ERRT("BSET.B #32, @R1", OVERFLOW_RANGE, "#32, @R1",  0x6C, 0x00, 0x01);
    ERRT("RTD.B  #300",     OVERFLOW_RANGE, "#300",      0xB8, 0x2C);
}

// Unresolved symbols must be flagged on the final pass (a deferred
// UNDEFINED_SYMBOL that hasError() ignores), not silently encoded as 0.  The
// emitted bytes are the zero encoding.
void test_undef() {
    ERUS("ADD:G.W #undef, R0",     "undef, R0",     0x01, 0xF1, 0x00);
    ERUS("ADD:G.W @undef, R0",     "undef, R0",     0x01, 0xF5, 0x00);
    ERUS("ADD:G.W @undef:16, R0",  "undef:16, R0",  0x01, 0xF6, 0x00, 0x00);
    ERUS("BRA undef",              "undef",         0x99, 0x00, 0x00);
    ERUS("SCB/EQ R1, undef",       "undef",         0xB5, 0x17, 0x00, 0x00);
    // Regression: an undefined relative target defers to a zero displacement; it
    // must NOT range-check (target - base) against the unresolved 0 -- here a
    // short branch at a high address whose -base delta would otherwise overflow.
    AERUS(0x100000, "BRA.B undef", "undef",         0x98, 0x00);
    AERUS(0x100000, "SCB/EQ.B R1, undef", "undef",  0xB4, 0x17, 0x00);
    // Two-EA insn: the undefined operand is flagged in any position (the
    // end-of-encode pass propagates whichever operand carries the error).
    ERUS("ADD:G.W @undef, @$20",   "undef, @$20",   0x01, 0x75, 0x00, 0x75, 0x20);
    ERUS("ADD:G.W @$20, @undef",   "undef",         0x01, 0x75, 0x20, 0x75, 0x00);
    ERUS("ADD:G.W @undef, @undef2","undef, @undef2",0x01, 0x75, 0x00, 0x75, 0x00);
    // Double-indirect carries two symbols (ds1 and ds2) in one operand.
    ERUS("ADD:G.W @($10, @(undef, R0)), R1", "undef, R0)), R1",
            0x01, 0x7E, 0x50, 0x00, 0x10, 0x41);
}

// clang-format on

void run_tests(const char *cpu) {
    assembler.setCpu(cpu);
    RUN_TEST(test_system);
    RUN_TEST(test_arith);
    RUN_TEST(test_compare);
    RUN_TEST(test_data_move);
    RUN_TEST(test_logic);
    RUN_TEST(test_extend);
    RUN_TEST(test_frame);
    RUN_TEST(test_shift_bit);
    RUN_TEST(test_mul_div);
    RUN_TEST(test_cr);
    RUN_TEST(test_ea_modes);
    RUN_TEST(test_reg_list);
    RUN_TEST(test_string);
    RUN_TEST(test_bitfield);
    RUN_TEST(test_optimal_class);
    RUN_TEST(test_smart_branch);
    RUN_TEST(test_data_constant);
    RUN_TEST(test_errors);
    RUN_TEST(test_undef);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

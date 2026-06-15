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
    EQUALS("cpu HD641016", true, assembler.setCpu("HD641016"));
    EQUALS_P("cpu HD641016", "HD641016", assembler.config().cpu_P());
}

// System / no-op / exception / bank-control instructions.
void test_system() {
    TEST("NOP",   0xFF);
    TEST("RTS",   0xBB);
    TEST("RTE",   0xF1);
    TEST("RTR",   0xF4);
    TEST("RESET", 0xF0);
    TEST("SLEEP", 0xF5);
    TEST("ICBN",  0xFD);
    TEST("DCBN",  0xFE);
    TEST("CGBN R1",   0xE4, 0x01);
    TEST("TRAPA #5",  0xF2, 0x05);
    TEST("TRAPA #0",  0xF2, 0x00);
    TEST("TRAPA #15", 0xF2, 0x0F);
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
    // ADD:RQ register-quick
    TEST("ADD:RQ.B #5, R0", 0x30, 0x05);
    // Sign-extend / CX
    TEST("ADDS.B R0, R1", 0x40, 0x40, 0x41);
    TEST("ADDX.B R0, R1", 0x50, 0x40, 0x41);
    // NEG / NEGX
    TEST("NEG.B R0",  0x8C, 0x40);
    TEST("NEGX.B R0", 0x9C, 0x40);
    // Decimal
    TEST("DADD R0, R1", 0x83, 0x40, 0x41);
    TEST("DSUB R0, R1", 0x87, 0x40, 0x41);
    TEST("DNEG.B R0",   0xAF, 0x40);
}

// Compare and test.
void test_compare() {
    TEST("CMP:G.B R0, R1",  0x08, 0x40, 0x41);
    TEST("CMP:Q.B #16, R0", 0x18, 0x10, 0x40);
    TEST("CMP:R.B R0, R1",  0x28, 0x01);
    TEST("CMP:RQ.B #5, R0", 0x38, 0x05);
    TEST("CMPS.B R0, R1",   0x48, 0x40, 0x41);
    TEST("TST.B R0",        0x58, 0x40);
}

// Data movement.
void test_data_move() {
    TEST("MOV:G.B R0, R1",  0x0C, 0x40, 0x41);
    TEST("MOV:Q.B #16, R0", 0x1C, 0x10, 0x40);
    TEST("MOV:R.B R0, R1",  0x2C, 0x01);
    TEST("MOV:RQ.B #5, R0", 0x3C, 0x05);
    TEST("MOVS.B R0, R1",   0x4C, 0x40, 0x41);
    TEST("MOVF.B R1",       0x5C, 0x41);
    TEST("CLR.B R0",        0x14, 0x40);
    TEST("CLR.W R0",        0x15, 0x40);
    TEST("XCH R0, R1",      0xB3, 0x01);
    TEST("SWAP.W R0",       0xEB, 0x40);
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
    TEST("UNLK R0",           0xD3, 0x00);
    TEST("UNLK R7",           0xD3, 0x07);
    TEST("RTD.B #16",         0xB8, 0x10);
    TEST("RTD.W #$0200",      0xB9, 0x02, 0x00);
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
}

// dc.b / dc.w / dc.l data-constant directives, with FCB / FDB aliases.
void test_data_constant() {
    // dc.b: byte values.
    TEST("dc.b $12",            0x12);
    TEST("dc.b $12, $34, $56",  0x12, 0x34, 0x56);
    TEST("fcb $AB, $CD",        0xAB, 0xCD);
    // dc.w: big-endian 16-bit values.
    TEST("dc.w $1234",          0x12, 0x34);
    TEST("dc.w $1234, $5678",   0x12, 0x34, 0x56, 0x78);
    TEST("fdb $ABCD",           0xAB, 0xCD);
    // dc.l: big-endian 32-bit values.
    TEST("dc.l $12345678",      0x12, 0x34, 0x56, 0x78);
    TEST("dc.l 1",              0x00, 0x00, 0x00, 0x01);
    // rmb: reserve bytes (no output).
    TEST("rmb 4");
    TEST("rmb 0");
}

// Error cases: bad sizes, out-of-range PC-relative displacements,
// invalid addressing modes, and unknown mnemonics.
void test_errors() {
    // Sized instructions need a .B / .W / .L suffix.
    ERRT("ADD:G   R0, R1",      ILLEGAL_SIZE, "");
    ERRT("MOV:G   R0, R1",      ILLEGAL_SIZE, "");
    // Fixed-size instructions reject a mismatching suffix.
    ERRT("XCH.W   R0, R1",      ILLEGAL_SIZE, "");
    ERRT("SWAP.B  R0",          ILLEGAL_SIZE, "");
    ERRT("DNEG.W  R0",          ILLEGAL_SIZE, "");
    // No-size instructions reject any suffix.
    ERRT("NOP.B",               ILLEGAL_SIZE, "");
    ERRT("RTS.W",               ILLEGAL_SIZE, "");
    // MUL/DIV only accept .B / .W (no .L).
    ERRT("MULXS.L R0, R1",      ILLEGAL_SIZE, "");
    ERRT("DIVXS.L R0, R1",      ILLEGAL_SIZE, "");
    // PC-relative explicit :8 / :16 with a delta that doesn't fit at this
    // address: OVERFLOW_RANGE rather than silently truncating.
    AERRT(0x1000, "ADD:G.B @R1, @($2000:8, PC)",   OVERFLOW_RANGE,
            "@($2000:8, PC)",   0x00, 0x01, 0x7D, 0x90, 0xFB);
    AERRT(0x1000, "ADD:G.B @R1, @($10000:16, PC)", OVERFLOW_RANGE,
            "@($10000:16, PC)", 0x00, 0x01, 0x7D, 0xA0, 0xEF, 0xFA);
    // Unknown mnemonic / non-existent register.
    ERRT("FOO     R0, R1",      UNKNOWN_INSTRUCTION, "FOO     R0, R1");
    ERRT("ADD:G.W R16, R0",     OPERAND_NOT_ALLOWED, "");
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
    RUN_TEST(test_data_constant);
    RUN_TEST(test_errors);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

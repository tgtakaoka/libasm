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

#include "asm_h8500.h"
#include "test_asm_helper.h"

using namespace libasm;
using namespace libasm::h8500;
using namespace libasm::test;

AsmH8500 asmh8500;
Assembler &assembler(asmh8500);

bool isH8520() {
    return strcasecmp_P("H8/520", assembler.config().cpu_P()) == 0;
}

void set_up() {
    assembler.reset();
}

void tear_down() {
    symtab.reset();
}

// clang-format off
void test_cpu() {
    EQUALS("cpu H8/500", true, assembler.setCpu("H8/500"));
    EQUALS_P("cpu H8/500", "H8/500", assembler.config().cpu_P());

    EQUALS("cpu H8/520", true, assembler.setCpu("H8/520"));
    EQUALS_P("cpu H8/520", "H8/520", assembler.config().cpu_P());
}

void test_spc_misc() {
    TEST("NOP",     0x00);
    TEST("RTE",     0x0A);
    TEST("RTS",     0x19);
    TEST("SLEEP",   0x1A);
    TEST("TRAP/VS", 0x09);
    TEST("UNLK FP", 0x0F);
    // TRAPA #vec
    TEST("TRAPA #0", 0x08, 0x10);
    TEST("TRAPA #1", 0x08, 0x11);
    TEST("TRAPA #3", 0x08, 0x13);
    // RTD #imm8 / #imm16
    TEST("RTD #8",     0x14, 0x08);
    TEST("RTD #$FF",   0x14, 0xFF);
    TEST("RTD #$200",  0x1C, 0x02, 0x00);
    // LINK FP, #imm8 / #imm16
    TEST("LINK FP, #8",     0x17, 0x08);
    TEST("LINK FP, #$FF",   0x17, 0xFF);
    TEST("LINK FP, #$200",  0x1F, 0x02, 0x00);
}

void test_spc_branch() {
    // Bcc d:8 (forward to address 0x12, base addr=0, delta=0x10)
    ATEST(0x0000, "BRA $0012", 0x20, 0x10);
    ATEST(0x0000, "BT  $0012", 0x20, 0x10);
    ATEST(0x0000, "BF  $0012", 0x21, 0x10);
    ATEST(0x0000, "BRN $0012", 0x21, 0x10);
    ATEST(0x0000, "BHI $0012", 0x22, 0x10);
    ATEST(0x0000, "BLS $0012", 0x23, 0x10);
    ATEST(0x0000, "BCC $0012", 0x24, 0x10);
    ATEST(0x0000, "BHS $0012", 0x24, 0x10);
    ATEST(0x0000, "BCS $0012", 0x25, 0x10);
    ATEST(0x0000, "BLO $0012", 0x25, 0x10);
    ATEST(0x0000, "BNE $0012", 0x26, 0x10);
    ATEST(0x0000, "BEQ $0012", 0x27, 0x10);
    ATEST(0x0000, "BVC $0012", 0x28, 0x10);
    ATEST(0x0000, "BVS $0012", 0x29, 0x10);
    ATEST(0x0000, "BPL $0012", 0x2A, 0x10);
    ATEST(0x0000, "BMI $0012", 0x2B, 0x10);
    ATEST(0x0000, "BGE $0012", 0x2C, 0x10);
    ATEST(0x0000, "BLT $0012", 0x2D, 0x10);
    ATEST(0x0000, "BGT $0012", 0x2E, 0x10);
    ATEST(0x0000, "BLE $0012", 0x2F, 0x10);
    // Backward branch
    ATEST(0x0100, "BRA $00FE", 0x20, 0xFC);
    // BSR d:8
    ATEST(0x0000, "BSR $0012", 0x0E, 0x10);
}

void test_spc_branch16() {
    // Bcc d:16 explicit (need :16 selector since smart branch is not implemented)
    // For now, test d:16 forms used by the disassembler.
    ATEST(0x0000, "BRA $0103", 0x30, 0x01, 0x00);
    // BSR d:16
    ATEST(0x0000, "BSR $0103", 0x1E, 0x01, 0x00);
}

void test_spc_move() {
    // MOV:E #imm8, Rd
    TEST("MOV:E #0, R0",     0x50, 0x00);
    TEST("MOV:E #$55, R3",   0x53, 0x55);
    TEST("MOV:E #$FF, R7",   0x57, 0xFF);
    // MOV:I #imm16, Rd
    TEST("MOV:I #$1234, R0", 0x58, 0x12, 0x34);
    TEST("MOV:I #$ABCD, R5", 0x5D, 0xAB, 0xCD);
    // CMP:E #imm8, Rd
    TEST("CMP:E #$55, R0",   0x40, 0x55);
    TEST("CMP:E #$FF, R3",   0x43, 0xFF);
    // CMP:I #imm16, Rd
    TEST("CMP:I #$1234, R0", 0x48, 0x12, 0x34);
    TEST("CMP:I #$ABCD, R5", 0x4D, 0xAB, 0xCD);
    // MOV:L @aa:8, Rd / MOV:S Rs, @aa:8
    TEST("MOV:L @$10:8, R0",   0x60, 0x10);
    TEST("MOV:L @$FF:8, R7",   0x67, 0xFF);
    TEST("MOV:S R0, @$10:8",   0x70, 0x10);
    TEST("MOV:S R7, @$FF:8",   0x77, 0xFF);
    // MOV:F @(d:8, FP), Rd / MOV:F Rs, @(d:8, FP)
    TEST("MOV:F @(8, FP), R0",   0x80, 0x08);
    TEST("MOV:F @(-1, FP), R0",  0x80, 0xFF);
    TEST("MOV:F R0, @(8, FP)",   0x90, 0x08);
}

void test_spc_ldm_stm() {
    // LDM @SP+, {reglist}
    TEST("LDM {R0}",          0x02, 0x01);
    TEST("LDM {R7}",          0x02, 0x80);
    TEST("LDM {R7, FP}",      0x02, 0xC0);
    TEST("LDM {R5, R4, R3}",  0x02, 0x38);
    // STM {reglist}, @-SP
    TEST("STM {R0}",          0x12, 0x01);
    TEST("STM {R7}",          0x12, 0x80);
    TEST("STM {R7, FP}",      0x12, 0xC0);
    TEST("STM {R5, R4, R3}",  0x12, 0x38);
}

void test_sec_jmp_jsr() {
    // JMP / JSR through SEC table (0x11 prefix)
    TEST("JMP @R0",                0x11, 0xD0);
    TEST("JMP @R7",                0x11, 0xD7);
    TEST("JSR @R0",                0x11, 0xD8);
    TEST("JSR @R7",                0x11, 0xDF);
    // JMP/JSR @(d:8, Rn) — disp8 form
    TEST("JMP @(8, R0)",           0x11, 0xE0, 0x08);
    TEST("JMP @(-1, R0)",          0x11, 0xE0, 0xFF);
    TEST("JSR @(8, R7)",           0x11, 0xEF, 0x08);
    // JMP/JSR @(d:16, Rn)
    TEST("JMP @($0100, R0)",       0x11, 0xF0, 0x01, 0x00);
    TEST("JSR @($0100, R0)",       0x11, 0xF8, 0x01, 0x00);
    // JMP/JSR @aa:16 (SPC form, no 0x11 prefix)
    TEST("JMP @$1234",             0x10, 0x12, 0x34);
    TEST("JSR @$1234",             0x18, 0x12, 0x34);
}

void test_sec_max_mode() {
    // PJMP/PJSR/PRTS/PRTD: invalid in min-mode, valid in max-mode.
    // Default after reset is min-mode (max-mode off). The mnemonic is
    // recognised but the entry lives on a MAXMODE-only page, so the table
    // search reports OPERAND_NOT_ALLOWED rather than UNKNOWN_INSTRUCTION.
    ERRT("PJMP @$010203", OPERAND_NOT_ALLOWED, "@$010203");
    ERRT("PJSR @$010203", OPERAND_NOT_ALLOWED, "@$010203");
    ERRT("PJMP @R0",      OPERAND_NOT_ALLOWED, "@R0");
    ERRT("PJSR @R0",      OPERAND_NOT_ALLOWED, "@R0");
    ERRT("PRTS",          OPERAND_NOT_ALLOWED, "");
    ERRT("PRTD #8",       OPERAND_NOT_ALLOWED, "#8");

    // The maxmode pseudo directive switches assembler mode (no bytes emitted).
    TEST("maxmode on");
    // PJMP/PJSR @aa:24 (SPC form)
    TEST("PJMP @$010203",  0x13, 0x01, 0x02, 0x03);
    TEST("PJSR @$010203",  0x03, 0x01, 0x02, 0x03);
    // PJMP/PJSR @Rn (SEC form, 0x11 prefix)
    TEST("PJMP @R0",       0x11, 0xC0);
    TEST("PJMP @R7",       0x11, 0xC7);
    TEST("PJSR @R0",       0x11, 0xC8);
    TEST("PJSR @R7",       0x11, 0xCF);
    // PRTS / PRTD (SEC form, 0x11 prefix)
    TEST("PRTS",           0x11, 0x19);
    TEST("PRTD #8",        0x11, 0x14, 0x08);
    TEST("PRTD #$1234",    0x11, 0x1C, 0x12, 0x34);
    TEST("maxmode off");
    ERRT("PJMP @$010203", OPERAND_NOT_ALLOWED, "@$010203");
}

void test_spc_scb() {
    // SCB/F, SCB/EQ, SCB/NE Rc, rel8
    ATEST(0x0000, "SCB/F  R0, $0004",  0x01, 0xB8, 0x01);
    ATEST(0x0000, "SCB/EQ R3, $0004",  0x07, 0xBB, 0x01);
    ATEST(0x0000, "SCB/NE R7, $0004",  0x06, 0xBF, 0x01);
}

void test_spc_add_q() {
    // ADD:Q #imm2, Rn (encoded via GEN format)
    TEST("ADD:Q #1, R0",  0xA8, 0x08);
    TEST("ADD:Q #-1, R0", 0xA8, 0x09);
    TEST("ADD:Q #2, R0",  0xA8, 0x0C);
    TEST("ADD:Q #-2, R0", 0xA8, 0x0D);
}

void test_gen_mov() {
    // MOV:G EA → Rd (no eaSz suffix → default word)
    TEST("MOV:G R0, R1",          0xA8, 0x81);
    TEST("MOV:G @R0, R1",         0xD8, 0x81);
    TEST("MOV:G @-R0, R1",        0xB8, 0x81);
    TEST("MOV:G @R0+, R1",        0xC8, 0x81);
    TEST("MOV:G @(8, R0), R1",    0xE8, 0x08, 0x81);
    TEST("MOV:G @(-1, R0), R1",   0xE8, 0xFF, 0x81);
    TEST("MOV:G @($100, R0), R1", 0xF8, 0x01, 0x00, 0x81);
    // MOV:G Rs → EA
    TEST("MOV:G R0, @R1",         0xD9, 0x90);
    TEST("MOV:G R0, @-R1",        0xB9, 0x90);
    TEST("MOV:G R0, @R1+",        0xC9, 0x90);
    TEST("MOV:G R5, @(8, R0)",    0xE8, 0x08, 0x95);
}

void test_gen_alu() {
    TEST("ADD:G @R0, R1",  0xD8, 0x21);
    TEST("SUB   @R0, R1",  0xD8, 0x31);
    TEST("ADDS  @R0, R1",  0xD8, 0x29);
    TEST("ADDX  R0, R1",   0xA0, 0xA1);
    TEST("SUBX  R0, R1",   0xA0, 0xB1);
    TEST("CMP:G @R0, R1",  0xD8, 0x71);
    TEST("AND   @R0, R1",  0xD8, 0x51);
    TEST("OR    @R0, R1",  0xD8, 0x41);
    TEST("XOR   @R0, R1",  0xD8, 0x61);
    TEST("MULXU R0, R1",   0xA0, 0xA9);
    TEST("DIVXU R0, R1",   0xA0, 0xB9);
    TEST("CLR   R0",       0xA8, 0x13);
    TEST("NEG   R0",       0xA8, 0x14);
    TEST("NOT   R0",       0xA8, 0x15);
    TEST("EXTS  R0",       0xA0, 0x11);
    TEST("EXTU  R0",       0xA0, 0x12);
    TEST("SWAP  R0",       0xA0, 0x10);
    TEST("TAS   R0",       0xA0, 0x17);
    TEST("TST   R0",       0xA8, 0x16);
}

void test_gen_shift() {
    TEST("SHAL  R0",  0xA8, 0x18);
    TEST("SHAR  R0",  0xA8, 0x19);
    TEST("SHLL  R0",  0xA8, 0x1A);
    TEST("SHLR  R0",  0xA8, 0x1B);
    TEST("ROTL  R0",  0xA8, 0x3C);
    TEST("ROTR  R0",  0xA8, 0x3D);
    TEST("ROTXL R0",  0xA8, 0x3E);
    TEST("ROTXR R0",  0xA8, 0x3F);
}

void test_gen_cr() {
    // LDC/STC: EA size depends on CR. SR is word, others are byte.
    TEST("LDC R0, CCR",  0xA0, 0x89);  // byte EA, CCR
    TEST("LDC R0, SR",   0xA8, 0x88);  // word EA, SR
    TEST("LDC R0, BR",   0xA0, 0x8B);
    TEST("LDC R0, EP",   0xA0, 0x8C);
    TEST("LDC R0, DP",   0xA0, 0x8D);
    TEST("LDC R0, TP",   0xA0, 0x8F);
    TEST("STC CCR, R0",  0xA0, 0x99);  // byte EA, CCR
    TEST("STC SR, R0",   0xA8, 0x98);  // word EA, SR
}

void test_gen_bit() {
    TEST("BSET R5, @R0",  0xD0, 0x4D);
    TEST("BSET #3, @R0",  0xD0, 0xC3);
    TEST("BCLR R5, @R0",  0xD0, 0x5D);
    TEST("BCLR #3, @R0",  0xD0, 0xD3);
    TEST("BNOT R5, @R0",  0xD0, 0x6D);
    TEST("BNOT #3, @R0",  0xD0, 0xE3);
    TEST("BTST R5, @R0",  0xD0, 0x7D);
    TEST("BTST #3, @R0",  0xD0, 0xF3);
}

void test_ext_format() {
    // DADD/DSUB: Format C (extended) - available on all CPUs
    TEST("DADD @R0, R1",  0xD0, 0x00, 0xA1);
    TEST("DSUB @R0, R1",  0xD0, 0x00, 0xB1);
    // MOVFPE/MOVTPE: H8/500 only (H8/520 has no E-clock pin)
    if (isH8520()) {
        ERRT("MOVFPE @R0, R1", UNKNOWN_INSTRUCTION, "MOVFPE @R0, R1");
        ERRT("MOVTPE R1, @R0", UNKNOWN_INSTRUCTION, "MOVTPE R1, @R0");
    } else {
        TEST("MOVFPE @R0, R1",  0xD0, 0x00, 0x81);
        TEST("MOVTPE R1, @R0",  0xD8, 0x00, 0x91);
    }
}

// clang-format on

void run_tests(const char *cpu) {
    assembler.setCpu(cpu);
    RUN_TEST(test_spc_misc);
    RUN_TEST(test_spc_branch);
    RUN_TEST(test_spc_branch16);
    RUN_TEST(test_spc_move);
    RUN_TEST(test_spc_ldm_stm);
    RUN_TEST(test_sec_jmp_jsr);
    RUN_TEST(test_sec_max_mode);
    RUN_TEST(test_spc_scb);
    RUN_TEST(test_spc_add_q);
    RUN_TEST(test_gen_mov);
    RUN_TEST(test_gen_alu);
    RUN_TEST(test_gen_shift);
    RUN_TEST(test_gen_cr);
    RUN_TEST(test_gen_bit);
    RUN_TEST(test_ext_format);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

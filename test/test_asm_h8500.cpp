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
    // A size suffix on a sizeless instruction is rejected.
    ERRT("NOP.W", ILLEGAL_SIZE, ".W");
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
    // Bcc d:16: smart branch promotes an out-of-d:8-range target to the 16-bit
    // form. The conditional 16-bit opcodes are the d:8 ones + 0x10.
    ATEST(0x0000, "BRA $0103", 0x30, 0x01, 0x00);
    ATEST(0x0000, "BF  $0103", 0x31, 0x01, 0x00);
    ATEST(0x0000, "BRN $0103", 0x31, 0x01, 0x00);
    ATEST(0x0000, "BHI $0103", 0x32, 0x01, 0x00);
    ATEST(0x0000, "BGE $0103", 0x3C, 0x01, 0x00);
    ATEST(0x0000, "BGT $0103", 0x3E, 0x01, 0x00);
    ATEST(0x0000, "BLE $0103", 0x3F, 0x01, 0x00);
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
    // MOV:L @aa:8, Rd / MOV:S Rs, @aa:8 -- Sz is OP-byte bit 3.
    TEST("MOV:L.B @$10:8, R0",   0x60, 0x10);
    TEST("MOV:L.B @$FF:8, R7",   0x67, 0xFF);
    TEST("MOV:L.W @$10:8, R0",   0x68, 0x10);
    TEST("MOV:L.W @$FF:8, R7",   0x6F, 0xFF);
    TEST("MOV:S.B R0, @$10:8",   0x70, 0x10);
    TEST("MOV:S.B R7, @$FF:8",   0x77, 0xFF);
    TEST("MOV:S.W R0, @$10:8",   0x78, 0x10);
    TEST("MOV:S.W R7, @$FF:8",   0x7F, 0xFF);
    // MOV:F @(d:8, FP), Rd / MOV:F Rs, @(d:8, FP)
    TEST("MOV:F.B @(8, FP), R0",   0x80, 0x08);
    TEST("MOV:F.B @(-1, FP), R0",  0x80, 0xFF);
    TEST("MOV:F.W @(8, FP), R0",   0x88, 0x08);
    TEST("MOV:F.B R0, @(8, FP)",   0x90, 0x08);
    TEST("MOV:F.W R0, @(8, FP)",   0x98, 0x08);
}

void test_spc_ldm_stm() {
    // LDM @SP+,(list) with Rm-Rn ranges; STM (list),@-SP.
    TEST("LDM @SP+, (R0)",         0x02, 0x01);
    TEST("LDM @SP+, (R7)",         0x02, 0x80);
    TEST("LDM @SP+, (R0-R3)",      0x02, 0x0F);
    TEST("LDM @SP+, (R0, R2-R4)",  0x02, 0x1D);
    TEST("STM (R0), @-SP",         0x12, 0x01);
    TEST("STM (R7), @-SP",         0x12, 0x80);
    TEST("STM (R0-R3), @-SP",      0x12, 0x0F);
    TEST("STM (R0, R2-R4), @-SP",  0x12, 0x1D);
    // Wrong stack direction (LDM needs @SP+, STM needs @-SP) and a descending
    // range are rejected.
    ERRT("LDM @-SP, (R0)",    REGISTER_NOT_ALLOWED, "@-SP, (R0)");
    ERRT("STM (R3-R1), @-SP", ILLEGAL_REGISTER,     "R1), @-SP");
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
    TEST(".maxmode on");
    // PJMP/PJSR @aa:24 (SPC form). The address must fit the physical space:
    // 24-bit (16MB) on the H8/500, 20-bit (1MB) on the H8/520/H8/530. 0x0FFFFF
    // fits both; 0x1000000 (16MB) overflows both.
    TEST("PJMP @$010203",  0x13, 0x01, 0x02, 0x03);
    TEST("PJSR @$010203",  0x03, 0x01, 0x02, 0x03);
    TEST("PJMP @$0FFFFF",   0x13, 0x0F, 0xFF, 0xFF);
    ERRT("PJMP @$1000000", OVERFLOW_RANGE, "@$1000000", 0x13, 0x00, 0x00, 0x00);
    ERRT("PJSR @$1000000", OVERFLOW_RANGE, "@$1000000", 0x03, 0x00, 0x00, 0x00);
    // PJMP/PJSR @Rn (SEC form, 0x11 prefix): @Rn copies Rn->CP and Rn+1->PC, so
    // Rn must be the even base of a register pair. An odd register is encodable
    // but misaligned (REGISTER_NOT_ALIGNED); the bytes are still emitted.
    TEST("PJMP @R0",       0x11, 0xC0);
    TEST("PJMP @R6",       0x11, 0xC6);
    TEST("PJSR @R0",       0x11, 0xC8);
    TEST("PJSR @R6",       0x11, 0xCE);
    ERRT("PJMP @R7",       REGISTER_NOT_ALIGNED, "@R7", 0x11, 0xC7);
    ERRT("PJSR @R7",       REGISTER_NOT_ALIGNED, "@R7", 0x11, 0xCF);
    // PRTS / PRTD (SEC form, 0x11 prefix)
    TEST("PRTS",           0x11, 0x19);
    TEST("PRTD #8",        0x11, 0x14, 0x08);
    TEST("PRTD #$1234",    0x11, 0x1C, 0x12, 0x34);
    TEST(".maxmode off");
    ERRT("PJMP @$010203", OPERAND_NOT_ALLOWED, "@$010203");
}

void test_spc_scb() {
    // SCB/F, SCB/EQ, SCB/NE Rc, rel8
    ATEST(0x0000, "SCB/F  R0, $0004",  0x01, 0xB8, 0x01);
    ATEST(0x0000, "SCB/EQ R3, $0004",  0x07, 0xBB, 0x01);
    ATEST(0x0000, "SCB/NE R7, $0004",  0x06, 0xBF, 0x01);
}

void test_spc_add_q() {
    // ADD:Q #xx,<EAd> -- GEN format; quick value in the OP byte
    // (08=+1 09=+2 0C=-1 0D=-2). Destination is a register or memory.
    TEST("ADD:Q #1, R0",        0xA8, 0x08);
    TEST("ADD:Q #2, R0",        0xA8, 0x09);
    TEST("ADD:Q #-1, R0",       0xA8, 0x0C);
    TEST("ADD:Q #-2, R0",       0xA8, 0x0D);
    TEST("ADD:Q #1, @R0",       0xD8, 0x08);
    TEST("ADD:Q #-2, @$30:8",   0x0D, 0x30, 0x0D);
    ERRT("ADD:Q #3, R0", OVERFLOW_RANGE, "#3, R0");
}

void test_optimal_class() {
    // A bare MOV/ADD/CMP stem (no ":x" class) selects the shortest form that
    // fits, matching the manual ("the assembler automatically generates the
    // optimum object code") and both ASL and h8500-hms-as.
    TEST("MOV.B @(4, R6), R0",  0x80, 0x04);            // MOV:F (vs :G E6 04 80)
    TEST("MOV.B R0, @(4, R6)",  0x90, 0x04);            // MOV:F store
    TEST("MOV.B #5, R0",        0x50, 0x05);            // MOV:E (vs :G 04 05 80)
    TEST("MOV.W #$1234, R0",    0x58, 0x12, 0x34);      // MOV:I
    TEST("MOV.B @$A0:8, R0",    0x60, 0xA0);            // MOV:L (vs :G 05 A0 80)
    TEST("MOV.B R0, @$A0:8",    0x70, 0xA0);            // MOV:S
    TEST("MOV.W R0, R1",        0xA8, 0x81);            // no short form -> MOV:G
    TEST("ADD.W #1, R0",        0xA8, 0x08);            // ADD:Q
    TEST("ADD.W #-2, R0",       0xA8, 0x0D);            // ADD:Q
    TEST("ADD.W #100, R0",      0x0C, 0x00, 0x64, 0x20);  // too big for :Q -> ADD:G
    TEST("CMP.B #5, R0",        0x40, 0x05);            // CMP:E
    TEST("CMP.W #$1234, R0",    0x48, 0x12, 0x34);      // CMP:I
    // No size suffix defaults to word, like ASL and GAS -- so a bare immediate
    // to a register takes the word form (MOV:I/CMP:I, ADD:G.W), not the byte one.
    TEST("MOV R0, R1",          0xA8, 0x81);
    TEST("ADD R0, R1",          0xA8, 0x21);
    TEST("MOV #5, R0",          0x58, 0x00, 0x05);        // MOV:I word (not MOV:E)
    TEST("CMP #5, R0",          0x48, 0x00, 0x05);        // CMP:I word (not CMP:E)
    TEST("ADD #5, R0",          0x0C, 0x00, 0x05, 0x20);  // ADD:G word imm EA (0x0C)
    // ADD:Q is byte/word via the EA Sz bit; the quick value stays in the OP byte.
    TEST("ADD.B #1, @R1",       0xD1, 0x08);
    TEST("ADD.W #1, @R1",       0xD9, 0x08);
    // An explicit ":x" class overrides optimal selection (forces the form).
    TEST("MOV:G.B @(4, R6), R0", 0xE6, 0x04, 0x80);
    TEST("ADD:G.W #1, R0",       0x0C, 0x00, 0x01, 0x20);
}

void test_gen_mov() {
    // MOV:G EA -> Rd; Sz bit is EA-byte bit 3 (A0/A8 byte/word).
    TEST("MOV:G.W R0, R1",          0xA8, 0x81);
    TEST("MOV:G.W @R0, R1",         0xD8, 0x81);
    TEST("MOV:G.W @-R0, R1",        0xB8, 0x81);
    TEST("MOV:G.W @R0+, R1",        0xC8, 0x81);
    TEST("MOV:G.W @(8, R0), R1",    0xE8, 0x08, 0x81);
    TEST("MOV:G.W @(-1, R0), R1",   0xE8, 0xFF, 0x81);
    TEST("MOV:G.W @($100, R0), R1", 0xF8, 0x01, 0x00, 0x81);
    // @(d:8,Rn) EA: byte EA byte is E0, word E8; the Sz bit follows the size.
    TEST("MOV:G.B @(8, R0), R1",    0xE0, 0x08, 0x81);
    TEST("MOV:G.W @(127, R0), R1",  0xE8, 0x7F, 0x81);   // max disp8
    TEST("MOV:G.W @(-128, R0), R1", 0xE8, 0x80, 0x81);   // min disp8
    // Beyond +/-127 the displacement widens to d:16 (F0/F8).
    TEST("MOV:G.W @(128, R0), R1",  0xF8, 0x00, 0x80, 0x81);
    TEST("MOV:G.W @(-129, R0), R1", 0xF8, 0xFF, 0x7F, 0x81);
    TEST("MOV:G.B @($1000, R7), R1", 0xF7, 0x10, 0x00, 0x81);  // disp16 byte EA, R7
    TEST("MOV:G.B @$10:8, R1",      0x05, 0x10, 0x81);
    TEST("MOV:G.W @$10:8, R1",      0x0D, 0x10, 0x81);
    TEST("MOV:G.W @$1000:16, R1",   0x1D, 0x10, 0x00, 0x81);
    TEST("MOV:G.B #$FF, R1",        0x04, 0xFF, 0x81);
    TEST("MOV:G.W #$1234, R1",      0x0C, 0x12, 0x34, 0x81);
    // MOV:G Rs -> EA
    TEST("MOV:G.W R0, @R1",         0xD9, 0x90);
    TEST("MOV:G.W R0, @-R1",        0xB9, 0x90);
    TEST("MOV:G.W R0, @R1+",        0xC9, 0x90);
    TEST("MOV:G.W R5, @(8, R0)",    0xE8, 0x08, 0x95);
    TEST("MOV:G.W R5, @$10:8",      0x0D, 0x10, 0x95);
    // MOV:G #xx,<EAd>: immediate to memory (data trails OP 0x06/0x07).
    TEST("MOV:G.B #$55, @R0",       0xD0, 0x06, 0x55);
    TEST("MOV:G.W #$1234, @R1",     0xD9, 0x07, 0x12, 0x34);
}

void test_gen_alu() {
    // Each op encodes both byte and word: Sz bit is EA-byte bit 3
    // (A0/A8 for Rn, D0/D8 for @Rn).
    TEST("ADD:G.B @R0, R1",  0xD0, 0x21);
    TEST("ADD:G.W @R0, R1",  0xD8, 0x21);
    TEST("SUB.B   @R0, R1",  0xD0, 0x31);
    TEST("SUB.W   @R0, R1",  0xD8, 0x31);
    TEST("ADDS.B  @R0, R1",  0xD0, 0x29);
    TEST("ADDS.W  @R0, R1",  0xD8, 0x29);
    TEST("ADDX.B  R0, R1",   0xA0, 0xA1);
    TEST("ADDX.W  R0, R1",   0xA8, 0xA1);
    TEST("SUBX.B  R0, R1",   0xA0, 0xB1);
    TEST("SUBX.W  R0, R1",   0xA8, 0xB1);
    TEST("CMP:G.B @R0, R1",  0xD0, 0x71);
    TEST("CMP:G.W @R0, R1",  0xD8, 0x71);
    // CMP:G #xx,<EAd>: immediate to memory (data trails OP 0x04/0x05).
    TEST("CMP:G.B #$AA, @R3",   0xD3, 0x04, 0xAA);
    TEST("CMP:G.W #$1234, @R1", 0xD9, 0x05, 0x12, 0x34);
    TEST("AND.B   @R0, R1",  0xD0, 0x51);
    TEST("AND.W   @R0, R1",  0xD8, 0x51);
    TEST("OR.B    @R0, R1",  0xD0, 0x41);
    TEST("OR.W    @R0, R1",  0xD8, 0x41);
    TEST("XOR.B   @R0, R1",  0xD0, 0x61);
    TEST("XOR.W   @R0, R1",  0xD8, 0x61);
    // .B multiplies/divides into a single register (any number); .W places the
    // 32-bit result in the pair Rd:Rd+1, so Rd must be even. An odd Rd in the
    // word form is encodable but misaligned (REGISTER_NOT_ALIGNED).
    TEST("MULXU.B R0, R1",   0xA0, 0xA9);
    TEST("MULXU.W R0, R2",   0xA8, 0xAA);
    TEST("DIVXU.B R0, R1",   0xA0, 0xB9);
    TEST("DIVXU.W R0, R2",   0xA8, 0xBA);
    ERRT("MULXU.W R0, R1",   REGISTER_NOT_ALIGNED, "R1", 0xA8, 0xA9);
    ERRT("DIVXU.W R0, R1",   REGISTER_NOT_ALIGNED, "R1", 0xA8, 0xB9);
    TEST("CLR.B   R0",       0xA0, 0x13);
    TEST("CLR.W   R0",       0xA8, 0x13);
    TEST("NEG.B   R0",       0xA0, 0x14);
    TEST("NEG.W   R0",       0xA8, 0x14);
    TEST("NOT.B   R0",       0xA0, 0x15);
    TEST("NOT.W   R0",       0xA8, 0x15);
    TEST("TST.B   R0",       0xA0, 0x16);
    TEST("TST.W   R0",       0xA8, 0x16);
    // Fixed-size ops: no suffix.
    // SWAP/EXTS/EXTU take a register operand only (no memory EA).
    TEST("EXTS  R0",       0xA0, 0x11);
    TEST("EXTU  R0",       0xA0, 0x12);
    TEST("SWAP  R0",       0xA0, 0x10);
    ERRT("SWAP  @R0",      OPERAND_NOT_ALLOWED, "@R0");
    ERRT("EXTS  @R0",      OPERAND_NOT_ALLOWED, "@R0");
    // TAS operates on memory or a register.
    TEST("TAS   R0",       0xA0, 0x17);
    TEST("TAS   @R0",      0xD0, 0x17);
    // XCH Rs,Rd: Rs is the EA byte (word register), Rd the OP-byte field.
    // The EA is register-only (no memory operand).
    TEST("XCH   R0, R1",   0xA8, 0x91);
    TEST("XCH   R7, R3",   0xAF, 0x93);
    ERRT("XCH   @R0, R1",  OPERAND_NOT_ALLOWED, "@R0, R1");
}

void test_gen_shift() {
    TEST("SHAL.B  R0",  0xA0, 0x18);
    TEST("SHAL.W  R0",  0xA8, 0x18);
    TEST("SHAR.W  R0",  0xA8, 0x19);
    TEST("SHLL.W  R0",  0xA8, 0x1A);
    TEST("SHLR.W  R0",  0xA8, 0x1B);
    TEST("ROTL.W  R0",  0xA8, 0x1C);
    TEST("ROTR.W  R0",  0xA8, 0x1D);
    TEST("ROTXL.W R0",  0xA8, 0x1E);
    TEST("ROTXR.W R0",  0xA8, 0x1F);
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
    // ANDC/ORC/XORC: .B uses imm8 EA (0x04), .W uses imm16 EA (0x0C).
    TEST("ANDC.B #$F0, CCR",   0x04, 0xF0, 0x59);
    TEST("ORC.B  #15, CCR",    0x04, 0x0F, 0x49);
    TEST("XORC.B #$FF, CCR",   0x04, 0xFF, 0x69);
    TEST("ANDC.W #$1234, SR",  0x0C, 0x12, 0x34, 0x58);
    TEST("ORC.W  #$1234, SR",  0x0C, 0x12, 0x34, 0x48);
    TEST("XORC.W #$1234, SR",  0x0C, 0x12, 0x34, 0x68);
    // LDC/STC need no suffix (the CR fixes the size), but the assembler still
    // accepts a suffix that matches the CR size...
    TEST("LDC.B R0, CCR",  0xA0, 0x89);
    TEST("LDC.W R0, SR",   0xA8, 0x88);
    TEST("STC.B CCR, R0",  0xA0, 0x99);
    TEST("STC.W SR, R0",   0xA8, 0x98);
    // ...and rejects a suffix that contradicts the control register.
    ERRT("LDC.W R0, CCR",  ILLEGAL_SIZE, ".W R0, CCR");
    ERRT("LDC.B R0, SR",   ILLEGAL_SIZE, ".B R0, SR");
    ERRT("STC.W CCR, R0",  ILLEGAL_SIZE, ".W CCR, R0");
    // An immediate whose size contradicts the control register width: a byte
    // immediate to the word SR, or a word immediate to a byte CR.
    ERRT("ORC.B #5, SR",      ILLEGAL_SIZE, "#5, SR");
    ERRT("ORC.W #5, CCR",     ILLEGAL_SIZE, "#5, CCR");
    ERRT("LDC #$1234, CCR",   ILLEGAL_SIZE, "#$1234, CCR");
    // A bare ANDC/ORC/XORC/LDC #imm,<CR> (no suffix) takes its size from the
    // control register: SR is word (imm16 EA 0x0C), CCR is byte (imm8 EA 0x04).
    TEST("ANDC #$F0, CCR",     0x04, 0xF0, 0x59);
    TEST("ANDC #$1234, SR",    0x0C, 0x12, 0x34, 0x58);
    TEST("ORC  #$1234, SR",    0x0C, 0x12, 0x34, 0x48);
    TEST("XORC #$1234, SR",    0x0C, 0x12, 0x34, 0x68);
    TEST("LDC  #$1234, SR",    0x0C, 0x12, 0x34, 0x88);
    TEST("LDC  #$12, CCR",     0x04, 0x12, 0x89);
}

void test_gen_bit() {
    // Unsuffixed defaults to word (D8/A8) for both register and memory targets;
    // the .B/.W suffix overrides either way.
    TEST("BSET R5, @R0",  0xD8, 0x4D);
    TEST("BSET #3, @R0",  0xD8, 0xC3);
    TEST("BCLR R5, @R0",  0xD8, 0x5D);
    TEST("BCLR #3, @R0",  0xD8, 0xD3);
    TEST("BNOT R5, @R0",  0xD8, 0x6D);
    TEST("BNOT #3, @R0",  0xD8, 0xE3);
    TEST("BTST R5, @R0",  0xD8, 0x7D);
    TEST("BTST #3, @R0",  0xD8, 0xF3);
    TEST("BSET R5, R1",     0xA9, 0x4D);  // register -> word default
    TEST("BSET.B R5, R1",   0xA1, 0x4D);  // register, .B override
    TEST("BSET.B R5, @R0",  0xD0, 0x4D);  // memory, .B override
    TEST("BCLR.B #3, @R0",  0xD0, 0xD3);
    // A byte-size target only has bits 0-7.
    TEST("BSET.B #7, @R0",     0xD0, 0xC7);
    ERRT("BSET.B #8, @R0",     ILLEGAL_BIT_NUMBER, "#8, @R0");
    ERRT("BSET.B #8, R1",      ILLEGAL_BIT_NUMBER, "#8, R1");
    TEST("BSET #8, @R0",       0xD8, 0xC8);  // word default allows 8-15
    TEST("BSET #15, R1",       0xA9, 0xCF);  // register default word
    // A word-size target has only bits 0-15; the 4-bit field must not wrap.
    ERRT("BSET.W #16, R1",     ILLEGAL_BIT_NUMBER, "#16, R1");
    ERRT("BCLR.W #16, @R0",    ILLEGAL_BIT_NUMBER, "#16, @R0");
    ERRT("BTST #20, R1",       ILLEGAL_BIT_NUMBER, "#20, R1");
}

void test_ext_format() {
    // DADD/DSUB Rs,Rd: Format C (extended), register operands only.
    TEST("DADD R0, R1",  0xA0, 0x00, 0xA1);
    TEST("DSUB R0, R1",  0xA0, 0x00, 0xB1);
    ERRT("DADD @R0, R1", OPERAND_NOT_ALLOWED, "@R0, R1");
    // MOVFPE/MOVTPE: all variants except H8/520, which has no E-clock pin.
    if (isH8520()) {
        ERRT("MOVFPE @R0, R1", UNKNOWN_INSTRUCTION, "MOVFPE @R0, R1");
        ERRT("MOVTPE R1, @R0", UNKNOWN_INSTRUCTION, "MOVTPE R1, @R0");
    } else {
        TEST("MOVFPE @R0, R1",  0xD0, 0x00, 0x81);
        TEST("MOVTPE R1, @R0",  0xD0, 0x00, 0x91);
    }
}

void test_insn_name_boundary() {
    // A :class or .size suffix must end the mnemonic; a trailing letter makes the
    // whole name unknown rather than being silently dropped.
    ERRT("MOV:G.BX R0, R1", UNKNOWN_INSTRUCTION, "MOV:G.BX R0, R1");
    ERRT("MOV:GX R0, R1",   UNKNOWN_INSTRUCTION, "MOV:GX R0, R1");
    ERRT("MOV.BW R0, R1",   UNKNOWN_INSTRUCTION, "MOV.BW R0, R1");
}

void test_size_suffix_boundary() {
    // parseSizeQual reads only the :8/:16/:24 number; a trailing letter is left
    // for the line-end check to reject as garbage.
    ERRT("MOV:G.B @$12:8x, R0",    GARBAGE_AT_END, "x, R0");
    ERRT("MOV:G.W @$1234:16z, R0", GARBAGE_AT_END, "z, R0");
}

void test_undef_symbol() {
    // Every operand kind must report an undefined symbol (placeholder bytes emit).
    ERUS("MOV:G #UNDEF, R0",   "UNDEF, R0",  0x0C, 0x00, 0x00, 0x80);  // imm via EA
    ERUS("MOV:G @UNDEF, R0",   "UNDEF, R0",  0x1D, 0x00, 0x00, 0x80);  // abs16 via EA
    ERUS("MOV:G @(UNDEF, R0), R1", "UNDEF, R0), R1", 0xE8, 0x00, 0x81);  // disp via EA
    ERUS("JMP @UNDEF",         "UNDEF",      0x10, 0x00, 0x00);  // abs16 (SPC)
    ERUS("JSR @UNDEF",         "UNDEF",      0x18, 0x00, 0x00);
    ERUS("BRA UNDEF",          "UNDEF",      0x20, 0x00);        // PC-relative d:8
    ERUS("BSR UNDEF",          "UNDEF",      0x0E, 0x00);
    ERUS("MOV:L @UNDEF:8, R0", "UNDEF:8, R0", 0x68, 0x00);      // abs8 (SPC)
    ERUS("MOV:G.B #UNDEF, @R0", "UNDEF, @R0", 0xD0, 0x06, 0x00);  // imm-to-mem (trailing)
    ERUS("TRAPA #UNDEF",       "UNDEF",      0x08, 0x10);        // trap vector
    ERUS("SCB/EQ R0, UNDEF",   "UNDEF",      0x07, 0xB8, 0x00);  // SCB rel8
}

void test_garbage_at_end() {
    // A third operand or any trailing token (past a comment) must be rejected,
    // not silently dropped. No bytes are emitted (the check precedes encoding).
    ERRT("MOV:G R0, R1, R2", GARBAGE_AT_END, ", R2");
    ERRT("ADD:G R0, R1 X",   GARBAGE_AT_END, "X");
    ERRT("JMP @R0 XYZ",      GARBAGE_AT_END, "XYZ");
    ERRT("BRA $0002 FOO",    GARBAGE_AT_END, "FOO");
}

void test_set_page_register() {
    // Page registers start UNDEFINED; ".set <reg>, <value>" records an assumed
    // value so the assembler can shorten a full absolute operand to its page
    // form. ".maxmode" is the maximum-mode directive (dotted, like .set).
    TEST(".maxmode on");

    // Undefined DP: a full 24-bit data address silently drops its high byte to
    // the @aa:16 form (DP supplies the page at run time). A <=0xFFFF value is
    // the literal 16-bit field, unchanged.
    TEST("MOV:G.W @$123456, R0", 0x1D, 0x34, 0x56, 0x80);
    TEST("MOV:G.W @$3456, R0",   0x1D, 0x34, 0x56, 0x80);

    // A defined DP validates the page: a matching one drops, a mismatch errors.
    TEST(".set DP, $12");
    TEST("MOV:G.W @$123456, R0", 0x1D, 0x34, 0x56, 0x80);
    ERRT("MOV:G.W @$993456, R0", OVERFLOW_RANGE, "@$993456, R0", 0x1D, 0x34, 0x56, 0x80);

    // A defined BR shortens a no-suffix address in its page (0 || BR || aa8) to
    // @aa:8; an address outside BR's page stays @aa:16.
    TEST(".set BR, $12");
    TEST("MOV:G.W @$1234, R0", 0x0D, 0x34, 0x80);
    TEST("MOV:G.W @$5634, R0", 0x1D, 0x56, 0x34, 0x80);

    // Unknown page register / value out of 0..255 are reported.
    ERRT(".set FOO, 1",   UNKNOWN_REGISTER, "FOO, 1");
    ERRT(".set DP, $100", OVERFLOW_RANGE,   "$100");

    // A negative value restores the undefined state; reset for later tests.
    TEST(".set DP, -1");
    TEST(".set BR, -1");
    TEST(".maxmode off");
}

void test_data_directives() {
    // H8/500 shares H8/300's data/space directives (big-endian).
    TEST(".DATA.B $12, $34",  0x12, 0x34);
    TEST(".DATA.W $1234",     0x12, 0x34);
    TEST(".DATA   $1234",     0x12, 0x34);
    TEST(".DATA.L $12345678", 0x12, 0x34, 0x56, 0x78);
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
    RUN_TEST(test_optimal_class);
    RUN_TEST(test_gen_mov);
    RUN_TEST(test_gen_alu);
    RUN_TEST(test_gen_shift);
    RUN_TEST(test_gen_cr);
    RUN_TEST(test_gen_bit);
    RUN_TEST(test_ext_format);
    RUN_TEST(test_insn_name_boundary);
    RUN_TEST(test_size_suffix_boundary);
    RUN_TEST(test_undef_symbol);
    RUN_TEST(test_garbage_at_end);
    RUN_TEST(test_set_page_register);
    RUN_TEST(test_data_directives);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

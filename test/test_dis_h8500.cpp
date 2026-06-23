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

bool isH8530() {
    return strcasecmp_P("H8/530", disassembler.config().cpu_P()) == 0;
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

    EQUALS("cpu H8/530", true,    disassembler.setCpu("H8/530"));
    EQUALS_P("cpu H8/530", "H8/530", disassembler.config().cpu_P());
}

void test_spc_misc() {
    // No-operand instructions
    TEST("NOP",     "",    0x00);
    TEST("RTE",     "",    0x0A);
    TEST("RTS",     "",    0x19);
    TEST("SLEEP",   "",    0x1A);
    TEST("TRAP/VS", "",    0x09);
    TEST("UNLK",    "FP",  0x0F);
    // TRAPA
    TEST("TRAPA",  "#0",   0x08, 0x10);
    TEST("TRAPA",  "#1",   0x08, 0x11);
    TEST("TRAPA",  "#3",   0x08, 0x13);
    // RTD
    TEST("RTD",    "#8",     0x14, 0x08);
    TEST("RTD",    "#H'FF",   0x14, 0xFF);
    TEST("RTD",    "#H'0200", 0x1C, 0x02, 0x00);
    // LINK/UNLK
    TEST("LINK",   "FP, #8",     0x17, 0x08);
    TEST("LINK",   "FP, #H'FF",   0x17, 0xFF);
    TEST("LINK",   "FP, #H'0200", 0x1F, 0x02, 0x00);
}

void test_spc_branch() {
    // Bcc d:8 (base addr=0, target = addr+2+delta)
    // Aliases share opcodes; disassembler outputs alphabetically-first name.
    TEST("BRA",  "H'0002", 0x20, 0x00);  // BRA=BT, BRA wins
    TEST("BF",   "H'0002", 0x21, 0x00);  // BF=BRN, BF wins
    TEST("BHI",  "H'0002", 0x22, 0x00);
    TEST("BLS",  "H'0002", 0x23, 0x00);
    TEST("BCC",  "H'0002", 0x24, 0x00);  // BCC=BHS, BCC wins
    TEST("BCS",  "H'0002", 0x25, 0x00);  // BCS=BLO, BCS wins
    TEST("BNE",  "H'0002", 0x26, 0x00);
    TEST("BEQ",  "H'0002", 0x27, 0x00);
    TEST("BVC",  "H'0002", 0x28, 0x00);
    TEST("BVS",  "H'0002", 0x29, 0x00);
    TEST("BPL",  "H'0002", 0x2A, 0x00);
    TEST("BMI",  "H'0002", 0x2B, 0x00);
    TEST("BGE",  "H'0002", 0x2C, 0x00);
    TEST("BLT",  "H'0002", 0x2D, 0x00);
    TEST("BGT",  "H'0002", 0x2E, 0x00);
    TEST("BLE",  "H'0002", 0x2F, 0x00);
    // Forward branch
    TEST("BRA",  "H'0012", 0x20, 0x10);
    // Backward branch
    ATEST(0x0100, "BRA", "H'00FE", 0x20, 0xFC);
    // BSR d:8
    TEST("BSR",  "H'0002", 0x0E, 0x00);
    TEST("BSR",  "H'0012", 0x0E, 0x10);

    // Bcc d:16 (target = addr+3+delta)
    TEST("BRA",  "H'0003", 0x30, 0x00, 0x00);  // BRA=BT, BRA wins
    TEST("BLS",  "H'0003", 0x33, 0x00, 0x00);
    TEST("BCC",  "H'0003", 0x34, 0x00, 0x00);  // BCC=BHS, BCC wins
    TEST("BCS",  "H'0003", 0x35, 0x00, 0x00);  // BCS=BLO, BCS wins
    TEST("BNE",  "H'0003", 0x36, 0x00, 0x00);
    TEST("BEQ",  "H'0003", 0x37, 0x00, 0x00);
    TEST("BVC",  "H'0003", 0x38, 0x00, 0x00);
    TEST("BVS",  "H'0003", 0x39, 0x00, 0x00);
    TEST("BPL",  "H'0003", 0x3A, 0x00, 0x00);
    TEST("BMI",  "H'0003", 0x3B, 0x00, 0x00);
    TEST("BLT",  "H'0003", 0x3D, 0x00, 0x00);
    TEST("BF",   "H'0003", 0x31, 0x00, 0x00);  // BF=BRN, BF wins
    TEST("BHI",  "H'0003", 0x32, 0x00, 0x00);
    TEST("BGE",  "H'0003", 0x3C, 0x00, 0x00);
    TEST("BGT",  "H'0003", 0x3E, 0x00, 0x00);
    TEST("BLE",  "H'0003", 0x3F, 0x00, 0x00);
    // BSR d:16
    TEST("BSR",  "H'0003", 0x1E, 0x00, 0x00);
    TEST("BSR",  "H'0103", 0x1E, 0x01, 0x00);
    // Forward branch d:16
    TEST("BRA",  "H'0103", 0x30, 0x01, 0x00);
    // Backward branch d:16
    ATEST(0x0100, "BRA", "H'0003", 0x30, 0xFF, 0x00);
}

void test_spc_move() {
    // MOV:E #imm8,Rd
    TEST("MOV:E", "#0, R0",    0x50, 0x00);
    TEST("MOV:E", "#H'FF, R0",  0x50, 0xFF);
    TEST("MOV:E", "#H'AB, R5",  0x55, 0xAB);
    // MOV:I #imm16,Rd
    TEST("MOV:I", "#0, R0",      0x58, 0x00, 0x00);
    TEST("MOV:I", "#H'1234, R5",  0x5D, 0x12, 0x34);
    // CMP:E #imm8,Rd
    TEST("CMP:E", "#H'FF, R0",  0x40, 0xFF);
    TEST("CMP:E", "#H'AB, R3",  0x43, 0xAB);
    // CMP:I #imm16,Rd
    TEST("CMP:I", "#H'FFFF, R0", 0x48, 0xFF, 0xFF);
    TEST("CMP:I", "#H'1234, R7", 0x4F, 0x12, 0x34);
    // MOV:L @aa:8,Rd -- Sz is OP-byte bit 3 (0x60 byte, 0x68 word)
    TEST("MOV:L.B", "@H'10:8, R0", 0x60, 0x10);
    TEST("MOV:L.B", "@H'FF:8, R7", 0x67, 0xFF);
    TEST("MOV:L.W", "@H'10:8, R0", 0x68, 0x10);
    TEST("MOV:L.W", "@H'FF:8, R7", 0x6F, 0xFF);
    // MOV:S Rs,@aa:8 (0x70 byte, 0x78 word)
    TEST("MOV:S.B", "R0, @H'10:8", 0x70, 0x10);
    TEST("MOV:S.B", "R7, @H'FF:8", 0x77, 0xFF);
    TEST("MOV:S.W", "R0, @H'10:8", 0x78, 0x10);
    TEST("MOV:S.W", "R7, @H'FF:8", 0x7F, 0xFF);
    // MOV:F @(d:8,FP),Rd (0x80 byte, 0x88 word)
    TEST("MOV:F.B", "@(8,FP), R0",  0x80, 0x08);
    TEST("MOV:F.B", "@(-1,FP), R0", 0x80, 0xFF);
    TEST("MOV:F.B", "@(0,FP), R7",  0x87, 0x00);
    TEST("MOV:F.W", "@(8,FP), R0",  0x88, 0x08);
    TEST("MOV:F.W", "@(0,FP), R7",  0x8F, 0x00);
    // MOV:F Rs,@(d:8,FP) (0x90 byte, 0x98 word)
    TEST("MOV:F.B", "R0, @(8,FP)",  0x90, 0x08);
    TEST("MOV:F.B", "R7, @(-1,FP)", 0x97, 0xFF);
    TEST("MOV:F.W", "R0, @(8,FP)",  0x98, 0x08);
    TEST("MOV:F.W", "R7, @(-1,FP)", 0x9F, 0xFF);
}

void test_spc_ldm_stm() {
    // LDM @SP+,(list): low-to-high, consecutive runs collapse to Rm-Rn.
    TEST("LDM", "@SP+, (R0)",        0x02, 0x01);
    TEST("LDM", "@SP+, (R7)",        0x02, 0x80);
    TEST("LDM", "@SP+, (R0-R3)",     0x02, 0x0F);
    TEST("LDM", "@SP+, (R0,R2-R4)", 0x02, 0x1D);
    // STM (list),@-SP at opcode 0x12
    TEST("STM", "(R0), @-SP",        0x12, 0x01);
    TEST("STM", "(R7), @-SP",        0x12, 0x80);
    TEST("STM", "(R0-R3), @-SP",     0x12, 0x0F);
    TEST("STM", "(R0,R2-R4), @-SP", 0x12, 0x1D);
}

void test_spc_jmp() {
    // JMP/JSR absolute
    TEST("JMP", "@H'0000:16", 0x10, 0x00, 0x00);
    TEST("JMP", "@H'1234:16", 0x10, 0x12, 0x34);
    TEST("JSR", "@H'0000:16", 0x18, 0x00, 0x00);
    TEST("JSR", "@H'1234:16", 0x18, 0x12, 0x34);
    // JMP/JSR indirect (0x11 prefix)
    TEST("JMP", "@R0",  0x11, 0xD0);
    TEST("JMP", "@SP",  0x11, 0xD7);
    TEST("JSR", "@R0",  0x11, 0xD8);
    TEST("JSR", "@SP",  0x11, 0xDF);
    // JMP/JSR @(d:8,Rn)
    TEST("JMP", "@(8,R0)",   0x11, 0xE0, 0x08);
    TEST("JMP", "@(-1,R0)",  0x11, 0xE0, 0xFF);
    TEST("JSR", "@(8,R0)",   0x11, 0xE8, 0x08);
    // JMP/JSR @(d:16,Rn)
    TEST("JMP", "@(H'0100,R0)", 0x11, 0xF0, 0x01, 0x00);
    TEST("JSR", "@(H'0100,R0)", 0x11, 0xF8, 0x01, 0x00);
}

void test_spc_scb() {
    // SCB/F, SCB/EQ, SCB/NE Rc,rel8
    TEST("SCB/F",  "R0, H'0004",  0x01, 0xB8, 0x01);
    TEST("SCB/EQ", "R3, H'0004",  0x07, 0xBB, 0x01);
    TEST("SCB/NE", "R7, H'0004",  0x06, 0xBF, 0x01);
}

void test_spc_add_q() {
    // ADD:Q #xx,<EAd> -- GEN format; the quick value is in the OP byte
    // (08=+1 09=+2 0C=-1 0D=-2). Byte/word follow the EA Sz bit (A0/A8 etc).
    TEST("ADD:Q.W", "#1, R0",        0xA8, 0x08);
    TEST("ADD:Q.W", "#2, R0",        0xA8, 0x09);
    TEST("ADD:Q.W", "#-1, R0",       0xA8, 0x0C);
    TEST("ADD:Q.W", "#-2, R0",       0xA8, 0x0D);
    TEST("ADD:Q.B", "#1, R0",        0xA0, 0x08);
    TEST("ADD:Q.W", "#1, @R0",       0xD8, 0x08);
    TEST("ADD:Q.B", "#1, @R0",       0xD0, 0x08);
    TEST("ADD:Q.W", "#-2, @H'30:8",  0x0D, 0x30, 0x0D);
}

void test_page_jumps() {
    // PJMP/PJSR/PRTS/PRTD are maximum-mode instructions on every variant; the
    // disassembler decodes them only with maximum mode enabled (see
    // test_page_jumps_minimum for the minimum-mode illegal cases).
    disassembler.setOption("maximum-mode", "on");
    // PJMP/PJSR absolute: the @aa:24 form is the only one, so it is marked :24
    // (a page-0 address would otherwise re-parse as @aa:16, which they reject).
    TEST("PJMP", "@H'010203:24", 0x13, 0x01, 0x02, 0x03);
    TEST("PJSR", "@H'010203:24", 0x03, 0x01, 0x02, 0x03);
    TEST("PJMP", "@H'001234:24", 0x13, 0x00, 0x12, 0x34);  // page-0 needs :24
    // PJMP/PJSR indirect (0x11 prefix): @Rn copies Rn->CP and Rn+1->PC, so Rn
    // must be the even base of a pair. An odd register decodes but is flagged
    // REGISTER_NOT_ALIGNED.
    TEST("PJMP", "@R0", 0x11, 0xC0);
    TEST("PJMP", "@R2", 0x11, 0xC2);
    TEST("PJSR", "@R0", 0x11, 0xC8);
    TEST("PJSR", "@R4", 0x11, 0xCC);
    ERRT("PJMP", "@SP", REGISTER_NOT_ALIGNED, "@SP", 0x11, 0xC7);
    ERRT("PJSR", "@SP", REGISTER_NOT_ALIGNED, "@SP", 0x11, 0xCF);
    // PRTS, PRTD (the #xx:16 long form with a byte-fitting value is marked :16
    // so it round-trips instead of shrinking to the #xx:8 form).
    TEST("PRTS", "",         0x11, 0x19);
    TEST("PRTD", "#8",       0x11, 0x14, 0x08);
    TEST("PRTD", "#8:16",    0x11, 0x1C, 0x00, 0x08);
    TEST("PRTD", "#H'0200",  0x11, 0x1C, 0x02, 0x00);
    // @aa:24 beyond the chip's physical space: 24-bit (16M) on the H8/500,
    // 20-bit (1M) on the pin-limited H8/520/H8/530. 0x100000 fits only H8/500.
    if (isH8520() || isH8530()) {
        ERRT("PJMP", "@H'100000:24", OVERFLOW_RANGE, "@H'100000:24", 0x13, 0x10, 0x00, 0x00);
    } else {
        TEST("PJMP", "@H'100000:24", 0x13, 0x10, 0x00, 0x00);
    }
}

void test_page_jumps_minimum() {
    // In minimum mode the page-jump opcodes are illegal: the disassembler hides
    // the maxMode-only pages, so PJMP/PJSR/PRTS/PRTD decode as unknown.
    UNKN(0x13);              // PJMP @aa:24 (SPC prefix unknown in minimum mode)
    UNKN(0x03);              // PJSR @aa:24
    UNKN(0x11, 0xC0);        // PJMP @R0
    UNKN(0x11, 0xC8);        // PJSR @R0
    UNKN(0x11, 0x19);        // PRTS
    UNKN(0x11, 0x14);        // PRTD #8
}

void test_gen_mov() {
    // MOV:G EA→Rd (opcode 0x80|Rd). The .B/.W suffix comes from the EA-byte Sz bit.
    TEST("MOV:G.B", "R0, R1",     0xA0, 0x81);  // EA=R0 byte
    TEST("MOV:G.W", "R0, R1",     0xA8, 0x81);  // EA=R0 word
    TEST("MOV:G.W", "@R0, R1",    0xD8, 0x81);  // EA=@R0 word
    TEST("MOV:G.W", "@-R0, R1",   0xB8, 0x81);  // EA=@-R0 word
    TEST("MOV:G.W", "@R0+, R1",   0xC8, 0x81);  // EA=@R0+ word
    TEST("MOV:G.W", "@(8,R0), R1",   0xE8, 0x08, 0x81);  // EA=@(d8,R0) word
    TEST("MOV:G.W", "@(-1,R0), R1",  0xE8, 0xFF, 0x81);  // negative disp
    TEST("MOV:G.W", "@(H'0100,R0), R1", 0xF8, 0x01, 0x00, 0x81);  // disp16
    // @(d:8,Rn): byte EA byte E0, word E8; full signed disp8 range.
    TEST("MOV:G.B", "@(8,R0), R1",      0xE0, 0x08, 0x81);
    TEST("MOV:G.W", "@(H'7F,R0), R1",   0xE8, 0x7F, 0x81);   // +127
    TEST("MOV:G.W", "@(-H'80,R0), R1",  0xE8, 0x80, 0x81);   // -128
    // @(d:16,Rn): byte EA byte F0, word F8.
    TEST("MOV:G.W", "@(H'0080,R0), R1", 0xF8, 0x00, 0x80, 0x81);
    TEST("MOV:G.B", "@(H'1000,SP), R1", 0xF7, 0x10, 0x00, 0x81);  // disp16 byte, R7=SP
    TEST("MOV:G.B", "@H'10:8, R1",  0x05, 0x10, 0x81);   // abs8 byte
    TEST("MOV:G.W", "@H'10:8, R1",  0x0D, 0x10, 0x81);   // abs8 word
    TEST("MOV:G.W", "@H'1000:16, R1", 0x1D, 0x10, 0x00, 0x81);  // abs16 word
    TEST("MOV:G.B", "#H'FF, R1",    0x04, 0xFF, 0x81);    // imm8
    TEST("MOV:G.W", "#H'1234, R1",  0x0C, 0x12, 0x34, 0x81);  // imm16
    // MOV:G Rs→EA (opcode 0x90|Rs)
    TEST("MOV:G.W", "R0, @R1",     0xD9, 0x90);  // EA=@R1 word
    TEST("MOV:G.W", "R0, @-R1",    0xB9, 0x90);
    TEST("MOV:G.W", "R0, @R1+",    0xC9, 0x90);
    TEST("MOV:G.W", "R5, @(8,R0)", 0xE8, 0x08, 0x95);
    TEST("MOV:G.W", "R5, @H'10:8",  0x0D, 0x10, 0x95);   // abs8 word
    // MOV:G #xx,<EAd>: immediate to memory; data trails OP 0x06 (byte)/0x07 (word).
    TEST("MOV:G.B", "#H'55, @R0",   0xD0, 0x06, 0x55);
    TEST("MOV:G.W", "#H'1234, @R1", 0xD9, 0x07, 0x12, 0x34);
}

void test_gen_alu() {
    // Each op decoded both byte and word: Sz bit is EA-byte bit 3
    // (A0/A8 for Rn, D0/D8 for @Rn).
    TEST("ADD:G.B", "@R0, R1",  0xD0, 0x21);
    TEST("ADD:G.W", "@R0, R1",  0xD8, 0x21);
    TEST("SUB.B",   "@R0, R1",  0xD0, 0x31);
    TEST("SUB.W",   "@R0, R1",  0xD8, 0x31);
    TEST("ADDS.B",  "@R0, R1",  0xD0, 0x29);
    TEST("ADDS.W",  "@R0, R1",  0xD8, 0x29);
    TEST("ADDX.B",  "R0, R1",   0xA0, 0xA1);
    TEST("ADDX.W",  "R0, R1",   0xA8, 0xA1);
    TEST("SUBX.B",  "R0, R1",   0xA0, 0xB1);
    TEST("SUBX.W",  "R0, R1",   0xA8, 0xB1);
    TEST("CMP:G.B", "@R0, R1",  0xD0, 0x71);
    TEST("CMP:G.W", "@R0, R1",  0xD8, 0x71);
    // CMP:G #xx,<EAd>: immediate to memory; data trails OP 0x04 (byte)/0x05 (word).
    TEST("CMP:G.B", "#H'AA, @R3",   0xD3, 0x04, 0xAA);
    TEST("CMP:G.W", "#H'1234, @R1", 0xD9, 0x05, 0x12, 0x34);
    TEST("AND.B",   "@R0, R1",  0xD0, 0x51);
    TEST("AND.W",   "@R0, R1",  0xD8, 0x51);
    TEST("OR.B",    "@R0, R1",  0xD0, 0x41);
    TEST("OR.W",    "@R0, R1",  0xD8, 0x41);
    TEST("XOR.B",   "@R0, R1",  0xD0, 0x61);
    TEST("XOR.W",   "@R0, R1",  0xD8, 0x61);
    // .B uses a single register (any number); .W places the 32-bit result in the
    // pair Rd:Rd+1, so an odd Rd is misaligned (REGISTER_NOT_ALIGNED).
    TEST("MULXU.B", "R0, R1",   0xA0, 0xA9);
    TEST("MULXU.W", "R0, R2",   0xA8, 0xAA);
    TEST("DIVXU.B", "R0, R1",   0xA0, 0xB9);
    TEST("DIVXU.W", "R0, R2",   0xA8, 0xBA);
    ERRT("MULXU.W", "R0, R1", REGISTER_NOT_ALIGNED, "R1", 0xA8, 0xA9);
    ERRT("DIVXU.W", "R0, R1", REGISTER_NOT_ALIGNED, "R1", 0xA8, 0xB9);
    TEST("CLR.B",   "R0",       0xA0, 0x13);
    TEST("CLR.W",   "R0",       0xA8, 0x13);
    TEST("NEG.B",   "R0",       0xA0, 0x14);
    TEST("NEG.W",   "R0",       0xA8, 0x14);
    TEST("NOT.B",   "R0",       0xA0, 0x15);
    TEST("NOT.W",   "R0",       0xA8, 0x15);
    TEST("TST.B",   "R0",       0xA0, 0x16);
    TEST("TST.W",   "R0",       0xA8, 0x16);
    // Fixed-size ops: no suffix regardless of the EA byte.
    TEST("EXTS",    "R0",       0xA0, 0x11);
    TEST("EXTU",    "R0",       0xA0, 0x12);
    TEST("SWAP",    "R0",       0xA0, 0x10);
    TEST("TAS",     "R0",       0xA0, 0x17);
    // XCH (register-only EA) shares OP 0x90 with MOV:G store but wins for a
    // register EA; a memory EA at 0x90 stays MOV:G.
    TEST("XCH",     "R0, R1",   0xA8, 0x91);
    TEST("XCH",     "R7, R3",   0xAF, 0x93);
    TEST("MOV:G.W", "R0, @R1",  0xD9, 0x90);
}

void test_gen_shift() {
    TEST("SHAL.B",  "R0",  0xA0, 0x18);
    TEST("SHAL.W",  "R0",  0xA8, 0x18);
    TEST("SHAR.W",  "R0",  0xA8, 0x19);
    TEST("SHLL.W",  "R0",  0xA8, 0x1A);
    TEST("SHLR.W",  "R0",  0xA8, 0x1B);
    TEST("ROTL.W",  "R0",  0xA8, 0x1C);
    TEST("ROTR.W",  "R0",  0xA8, 0x1D);
    TEST("ROTXL.W", "R0",  0xA8, 0x1E);
    TEST("ROTXR.W", "R0",  0xA8, 0x1F);
}

void test_gen_bit() {
    // The EA byte's Sz bit gives the .B/.W suffix (byte D0, word D8/A8); it must
    // be emitted so the byte/word target distinction round-trips.
    TEST("BSET.B", "R5, @R0",  0xD0, 0x4D);
    TEST("BSET.W", "R5, @R0",  0xD8, 0x4D);
    TEST("BSET.B", "#3, @R0",  0xD0, 0xC3);
    TEST("BSET.W", "#3, @R0",  0xD8, 0xC3);
    TEST("BCLR.B", "R5, @R0",  0xD0, 0x5D);
    TEST("BCLR.B", "#3, @R0",  0xD0, 0xD3);
    TEST("BNOT.B", "R5, @R0",  0xD0, 0x6D);
    TEST("BNOT.B", "#3, @R0",  0xD0, 0xE3);
    TEST("BTST.B", "R5, @R0",  0xD0, 0x7D);
    TEST("BTST.B", "#3, @R0",  0xD0, 0xF3);
    TEST("BSET.W", "R5, R1",   0xA9, 0x4D);  // register word
    TEST("BSET.W", "#3, R1",   0xA9, 0xC3);
    TEST("BSET.W", "#8, @R0",  0xD8, 0xC8);  // word target, bit 8 OK
    TEST("BSET.W", "#15, R1",  0xA9, 0xCF);
}

void test_gen_cr() {
    // LDC/STC carry no size suffix: the control register fixes the operand
    // size (SR is word, others byte), so the EA Sz bit just mirrors the CR.
    TEST("LDC", "R0, CCR",  0xA0, 0x89);
    TEST("LDC", "R0, SR",   0xA8, 0x88);
    TEST("LDC", "R0, BR",   0xA0, 0x8B);
    TEST("LDC", "R0, EP",   0xA0, 0x8C);
    TEST("LDC", "R0, DP",   0xA0, 0x8D);
    TEST("LDC", "R0, TP",   0xA0, 0x8F);
    TEST("STC", "CCR, R0",  0xA0, 0x99);
    TEST("STC", "SR, R0",   0xA8, 0x98);
    // ANDC/ORC/XORC: .B uses imm8 EA (0x04), .W uses imm16 EA (0x0C).
    TEST("ANDC.B", "#H'F0, CCR",   0x04, 0xF0, 0x59);
    TEST("ORC.B",  "#15, CCR",     0x04, 0x0F, 0x49);
    TEST("XORC.B", "#H'FF, CCR",   0x04, 0xFF, 0x69);
    TEST("ANDC.W", "#H'1234, SR",  0x0C, 0x12, 0x34, 0x58);
    TEST("ORC.W",  "#H'1234, SR",  0x0C, 0x12, 0x34, 0x48);
    TEST("XORC.W", "#H'1234, SR",  0x0C, 0x12, 0x34, 0x68);
}

void test_illegal_h8500() {
    // Illegal SPC opcodes (gaps in 0x00-0x1F that are neither EA bytes
    // nor mapped instructions). EA bytes used by GEN: 0x04,0x05,0x0C,0x0D,0x15,0x1D.
    // 0x11 is the SEC prefix. Remaining gaps:
    UNKN(0x0B);
    UNKN(0x16);
    UNKN(0x1B);

    // Illegal bytes after 0x11 prefix. Valid SEC opcodes:
    // 0x14, 0x19, 0x1C, 0xC0-0xFF (PJMP/PJSR/JMP/JSR family).
    UNKN(0x11, 0x00);
    UNKN(0x11, 0x13);
    UNKN(0x11, 0x15);
    UNKN(0x11, 0x18);
    UNKN(0x11, 0x1B);
    UNKN(0x11, 0x1D);
    UNKN(0x11, 0x7F);
    UNKN(0x11, 0xBF);

    // Illegal GEN OP byte after a valid EA byte: EA=0xA0 (R0 byte register) or
    // EA=0xD8 (@R0 word) followed by an unused OP byte (0x0B).
    UNKN(0xA0, 0x0B);
    UNKN(0xD8, 0x0B);
    // A 0x00 OP byte is the EXT-format marker, so [EA, 0x00] still needs the
    // real OP byte and a 2-byte sequence runs out of memory.
    NMEM("", "", "", 0xA0, 0x00);

    // Illegal EXT OP byte (after EA byte + 0x00 separator).
    UNKN(0xA0, 0x00, 0x00);

    // MOV:G #imm: OP 0x06 is 8-bit data, 0x07 is 16-bit data. The 16-bit-data OP
    // with a byte EA (0x05) is impossible (a word of data, byte operation); the
    // 8-bit-data OP with a word EA is the legal sign-extended form, not illegal.
    UNKN(0x05, 0x06, 0x07);

    // CMP:G #imm: same Sz rule. OP 0x05 (word data) with a byte EA (D0=@R0) is
    // impossible; OP 0x04 (byte data) with a word EA is the sign-extended form.
    UNKN(0xD0, 0x05);
    TEST("CMP:G.W", "#18, @R0", 0xD8, 0x04, 0x12);

    // BSET/BCLR #xx on a byte-size target (EA Sz=0): bit number must be 0-7.
    // D0=@R0 byte, C8=BSET #8 -> illegal; D8=@R0 word, C8=BSET #8 -> OK.
    ERRT("BSET.B", "", ILLEGAL_BIT_NUMBER, "", 0xD0, 0xC8);
    ERRT("BCLR.B", "", ILLEGAL_BIT_NUMBER, "", 0xD0, 0xDF);

    // LDM/STM with an empty register list (mask 0) transfers nothing.
    ERRT("LDM", "@SP+, ()", OPCODE_HAS_NO_EFFECT, "()", 0x02, 0x00);
    ERRT("STM", "()", OPCODE_HAS_NO_EFFECT, "()", 0x12, 0x00);

    // Fixed-size ops: a Sz bit that contradicts the fixed size (TAS/SWAP/EXTS/
    // EXTU are byte; XCH is word; DADD/DSUB are byte, EXT format) is a
    // non-canonical encoding the assembler never emits.
    UNKN(0xA8, 0x17);  // TAS r0 with Sz=1 (byte op)
    UNKN(0xA8, 0x10);  // SWAP r0 with Sz=1
    UNKN(0xA8, 0x11);  // EXTS r0 with Sz=1
    UNKN(0xA8, 0x12);  // EXTU r0 with Sz=1
    UNKN(0xA0, 0x90);  // XCH r0,r0 with Sz=0 (word op)
    UNKN(0xA8, 0x00, 0xA1);  // DADD r0,r0 with Sz=1 (EXT)
    UNKN(0xA8, 0x00, 0xB1);  // DSUB r0,r0 with Sz=1 (EXT)

    // LDC/STC: the EA Sz bit must match the control-register width (SR is word,
    // CCR/BR/EP/DP/TP are byte); a mismatch is a non-canonical encoding.
    UNKN(0xA0, 0x88);  // LDC byte EA -> SR (word CR)
    UNKN(0xA8, 0x89);  // LDC word EA -> CCR (byte CR)
    UNKN(0xA0, 0x98);  // STC SR (word CR) -> byte EA

    // TRAPA second byte must be 0x10|vec; SCB second byte must be 0xB8|Rc.
    UNKN(0x08, 0x00);  // TRAPA with a non-1x second byte
    UNKN(0x08, 0x20);
    UNKN(0x01, 0xC0);  // SCB/F with a non-(0xB8|r) count byte (rejected before rel8)
}

void test_ext_format() {
    // DADD/DSUB Rs,Rd: valid on all CPUs, register operands only.
    TEST("DADD", "R0, R1",  0xA0, 0x00, 0xA1);
    TEST("DSUB", "R0, R1",  0xA0, 0x00, 0xB1);
    // MOVFPE/MOVTPE: H8/500 only (H8/520 has no E-clock pin)
    if (isH8520()) {
        ERRT("", "", UNKNOWN_INSTRUCTION, "", 0xD0, 0x00, 0x81);
    } else {
        TEST("MOVFPE", "@R0, R1",  0xD0, 0x00, 0x81);
        TEST("MOVTPE", "R1, @R0",  0xD0, 0x00, 0x91);
    }
}

void test_symbol() {
    // Absolute and PC-relative operands resolve to a symbol when the table has
    // one for the target address.
    symtab.intern(0xABCD, "data");
    symtab.intern(0x0030, "page0");
    symtab.intern(0x1012, "loop");
    TEST("MOV:G.W", "@data:16, R0", 0x1D, 0xAB, 0xCD, 0x80);  // @aa:16 -> symbol
    TEST("MOV:G.B", "@page0:8, R0", 0x05, 0x30, 0x80);        // @aa:8  -> symbol
    ATEST(0x1000, "BRA", "loop",     0x20, 0x10);             // Bcc d:8 target -> symbol
    ATEST(0x1000, "JMP", "@data:16", 0x10, 0xAB, 0xCD);       // @aa:16 jump -> symbol
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
    RUN_TEST(test_page_jumps);
    RUN_TEST(test_page_jumps_minimum);
    RUN_TEST(test_gen_mov);
    RUN_TEST(test_gen_alu);
    RUN_TEST(test_gen_shift);
    RUN_TEST(test_gen_bit);
    RUN_TEST(test_gen_cr);
    RUN_TEST(test_ext_format);
    RUN_TEST(test_illegal_h8500);
    RUN_TEST(test_symbol);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

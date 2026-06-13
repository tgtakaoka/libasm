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
    // MOV:L @aa:8,Rd
    TEST("MOV:L", "@H'10:8, R0", 0x60, 0x10);
    TEST("MOV:L", "@H'FF:8, R7", 0x67, 0xFF);
    // MOV:S Rs,@aa:8
    TEST("MOV:S", "R0, @H'10:8", 0x70, 0x10);
    TEST("MOV:S", "R7, @H'FF:8", 0x77, 0xFF);
    // MOV:F @(d:8,FP),Rd
    TEST("MOV:F", "@(8, FP), R0",  0x80, 0x08);
    TEST("MOV:F", "@(-1, FP), R0", 0x80, 0xFF);
    TEST("MOV:F", "@(0, FP), R7",  0x87, 0x00);
    // MOV:F Rs,@(d:8,FP)
    TEST("MOV:F", "R0, @(8, FP)",  0x90, 0x08);
    TEST("MOV:F", "R7, @(-1, FP)", 0x97, 0xFF);
}

void test_spc_ldm_stm() {
    // LDM {reglist}
    TEST("LDM", "{R0}",          0x02, 0x01);
    TEST("LDM", "{R7}",          0x02, 0x80);
    TEST("LDM", "{R7, FP}",      0x02, 0xC0);
    TEST("LDM", "{R5, R4, R3}",  0x02, 0x38);
    // STM {reglist} at opcode 0x12
    TEST("STM", "{R0}",          0x12, 0x01);
    TEST("STM", "{R7}",          0x12, 0x80);
    TEST("STM", "{R7, FP}",      0x12, 0xC0);
    TEST("STM", "{R5, R4, R3}",  0x12, 0x38);
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
    TEST("JMP", "@(8, R0)",   0x11, 0xE0, 0x08);
    TEST("JMP", "@(-1, R0)",  0x11, 0xE0, 0xFF);
    TEST("JSR", "@(8, R0)",   0x11, 0xE8, 0x08);
    // JMP/JSR @(d:16,Rn)
    TEST("JMP", "@(H'0100, R0)", 0x11, 0xF0, 0x01, 0x00);
    TEST("JSR", "@(H'0100, R0)", 0x11, 0xF8, 0x01, 0x00);
}

void test_spc_scb() {
    // SCB/F, SCB/EQ, SCB/NE Rc,rel8
    TEST("SCB/F",  "R0, H'0004",  0x01, 0xB8, 0x01);
    TEST("SCB/EQ", "R3, H'0004",  0x07, 0xBB, 0x01);
    TEST("SCB/NE", "R7, H'0004",  0x06, 0xBF, 0x01);
}

void test_spc_add_q() {
    // ADD:Q — encoded as GEN format (EA=Rn word, OP=imm opcode)
    TEST("ADD:Q", "R0",  0xA8, 0x08);  // +1
    TEST("ADD:Q", "R0",  0xA8, 0x09);  // -1
    TEST("ADD:Q", "R0",  0xA8, 0x0C);  // +2
    TEST("ADD:Q", "R0",  0xA8, 0x0D);  // -2
}

void test_page_jumps() {
    // PJMP/PJSR/PRTS/PRTD: max-mode only at runtime, but the disassembler
    // always decodes them (codes are not reused for other instructions).
    // PJMP/PJSR absolute (24-bit address)
    TEST("PJMP", "@H'010203", 0x13, 0x01, 0x02, 0x03);
    TEST("PJSR", "@H'010203", 0x03, 0x01, 0x02, 0x03);
    // PJMP/PJSR indirect (0x11 prefix)
    TEST("PJMP", "@R0", 0x11, 0xC0);
    TEST("PJMP", "@SP", 0x11, 0xC7);
    TEST("PJSR", "@R0", 0x11, 0xC8);
    TEST("PJSR", "@SP", 0x11, 0xCF);
    // PRTS, PRTD
    TEST("PRTS", "",       0x11, 0x19);
    TEST("PRTD", "#8",     0x11, 0x14, 0x08);
    TEST("PRTD", "#H'0200", 0x11, 0x1C, 0x02, 0x00);
}

void test_gen_mov() {
    // MOV:G EA→Rd (opcode 0x80|Rd)
    TEST("MOV:G", "R0, R1",     0xA8, 0x81);  // EA=R0 word, OP=0x81
    TEST("MOV:G", "@R0, R1",    0xD8, 0x81);  // EA=@R0 word
    TEST("MOV:G", "@-R0, R1",   0xB8, 0x81);  // EA=@-R0 word
    TEST("MOV:G", "@R0+, R1",   0xC8, 0x81);  // EA=@R0+ word
    TEST("MOV:G", "@(8, R0), R1",   0xE8, 0x08, 0x81);  // EA=@(d8,R0) word
    TEST("MOV:G", "@(-1, R0), R1",  0xE8, 0xFF, 0x81);  // negative disp
    TEST("MOV:G", "@(H'0100, R0), R1", 0xF8, 0x01, 0x00, 0x81);  // disp16
    TEST("MOV:G", "@H'10:8, R1",  0x0D, 0x10, 0x81);   // abs8 word
    TEST("MOV:G", "@H'1000:16, R1", 0x1D, 0x10, 0x00, 0x81);  // abs16 word
    TEST("MOV:G", "#H'FF, R1",    0x04, 0xFF, 0x81);    // imm8
    TEST("MOV:G", "#H'1234, R1",  0x0C, 0x12, 0x34, 0x81);  // imm16
    // MOV:G Rs→EA (opcode 0x90|Rs)
    TEST("MOV:G", "R0, @R1",     0xD9, 0x90);  // EA=@R1 word
    TEST("MOV:G", "R0, @-R1",    0xB9, 0x90);
    TEST("MOV:G", "R0, @R1+",    0xC9, 0x90);
    TEST("MOV:G", "R5, @(8, R0)", 0xE8, 0x08, 0x95);
    TEST("MOV:G", "R5, @H'10:8",  0x0D, 0x10, 0x95);   // abs8 word
}

void test_gen_alu() {
    TEST("ADD:G", "@R0, R1",  0xD8, 0x21);
    TEST("SUB",   "@R0, R1",  0xD8, 0x31);
    TEST("ADDS",  "@R0, R1",  0xD8, 0x29);
    TEST("ADDX",  "R0, R1",   0xA0, 0xA1);
    TEST("SUBX",  "R0, R1",   0xA0, 0xB1);
    TEST("CMP:G", "@R0, R1",  0xD8, 0x71);
    TEST("AND",   "@R0, R1",  0xD8, 0x51);
    TEST("OR",    "@R0, R1",  0xD8, 0x41);
    TEST("XOR",   "@R0, R1",  0xD8, 0x61);
    TEST("MULXU", "R0, R1",   0xA0, 0xA9);
    TEST("DIVXU", "R0, R1",   0xA0, 0xB9);
    TEST("CLR",   "R0",       0xA8, 0x13);
    TEST("NEG",   "R0",       0xA8, 0x14);
    TEST("NOT",   "R0",       0xA8, 0x15);
    TEST("EXTS",  "R0",       0xA0, 0x11);
    TEST("EXTU",  "R0",       0xA0, 0x12);
    TEST("SWAP",  "R0",       0xA0, 0x10);
    TEST("TAS",   "R0",       0xA0, 0x17);
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
    TEST("BSET", "R5, @R0",  0xD0, 0x4D);
    TEST("BSET", "#3, @R0",  0xD0, 0xC3);
    TEST("BCLR", "R5, @R0",  0xD0, 0x5D);
    TEST("BCLR", "#3, @R0",  0xD0, 0xD3);
    TEST("BNOT", "R5, @R0",  0xD0, 0x6D);
    TEST("BNOT", "#3, @R0",  0xD0, 0xE3);
    TEST("BTST", "R5, @R0",  0xD0, 0x7D);
    TEST("BTST", "#3, @R0",  0xD0, 0xF3);
}

void test_gen_cr() {
    TEST("LDC",  "R0, CCR",  0xA0, 0x89);
    TEST("LDC",  "R0, SR",   0xA0, 0x88);
    TEST("LDC",  "R0, BR",   0xA0, 0x8B);
    TEST("LDC",  "R0, EP",   0xA0, 0x8C);
    TEST("LDC",  "R0, DP",   0xA0, 0x8D);
    TEST("LDC",  "R0, TP",   0xA0, 0x8F);
    TEST("STC",  "CCR, R0",  0xA8, 0x99);
    TEST("STC",  "SR, R0",   0xA8, 0x98);
    TEST("ANDC", "#H'F0, CCR", 0x04, 0xF0, 0x59);
    TEST("ORC",  "#15, CCR", 0x04, 0x0F, 0x79);
    TEST("XORC", "#H'FF, CCR", 0x04, 0xFF, 0x69);
}

void test_illegal_h8500() {
    // Illegal SPC opcodes (gaps in 0x00-0x1F that are neither EA bytes
    // nor mapped instructions). EA bytes used by GEN: 0x04,0x05,0x0C,0x0D,0x15,0x1D.
    // 0x11 is the SEC prefix. Remaining gaps:
    UNKN(0x0B);
    UNKN(0x16);
    UNKN(0x1B);

    // Illegal Bcc d:16 cc-field values. Manual omits cc=1 (BRN), cc=2 (BHI),
    // cc=12 (BGE), cc=14 (BGT), cc=15 (BLE). The disassembler returns
    // UNKNOWN_INSTRUCTION after reading just the opcode byte.
    UNKN(0x31);
    UNKN(0x32);
    UNKN(0x3C);
    UNKN(0x3E);
    UNKN(0x3F);

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

    // Illegal GEN OP byte after a valid EA byte. EA=0xA0 (R0 byte register).
    // OP byte 0x00 is not a GEN entry.
    UNKN(0xA0, 0x00);
    // EA=0xD8 (@R0 word) followed by non-GEN OP byte.
    UNKN(0xD8, 0x00);
    UNKN(0xD8, 0x07);

    // Illegal EXT OP byte (after EA byte + 0x00 separator).
    UNKN(0xA0, 0x00, 0x00);
}

void test_ext_format() {
    // DADD/DSUB: valid on all CPUs
    TEST("DADD", "@R0, R1",  0xD0, 0x00, 0xA1);
    TEST("DSUB", "@R0, R1",  0xD0, 0x00, 0xB1);
    // MOVFPE/MOVTPE: H8/500 only (H8/520 has no E-clock pin)
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
    RUN_TEST(test_page_jumps);
    RUN_TEST(test_gen_mov);
    RUN_TEST(test_gen_alu);
    RUN_TEST(test_gen_shift);
    RUN_TEST(test_gen_bit);
    RUN_TEST(test_gen_cr);
    RUN_TEST(test_ext_format);
    RUN_TEST(test_illegal_h8500);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

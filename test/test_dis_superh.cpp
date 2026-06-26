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

#include "dis_superh.h"
#include "test_dis_helper.h"

using namespace libasm;
using namespace libasm::superh;
using namespace libasm::test;

DisSuperH dissuperh;
Disassembler &disassembler(dissuperh);

bool isSh1() {
    return strcasecmp_P("SH-1", disassembler.config().cpu_P()) == 0;
}

bool isSh2() {
    return strcasecmp_P("SH-2", disassembler.config().cpu_P()) == 0;
}

bool isShDsp() {
    return strcasecmp_P("SH-DSP", disassembler.config().cpu_P()) == 0;
}

bool isSh2e() {
    return strcasecmp_P("SH-2E", disassembler.config().cpu_P()) == 0;
}

bool isSh2a() {
    return strcasecmp_P("SH-2A", disassembler.config().cpu_P()) == 0;
}

void set_up() {
    disassembler.reset();
}

void tear_down() {
    symtab.reset();
}

// 0x0000 is the SH reset vector area; tests use a typical program address.
#define SH_UNKN(...) AERRT(0x1000, "", "", UNKNOWN_INSTRUCTION, "", __VA_ARGS__)

// clang-format off
void test_cpu() {
    EQUALS("cpu SH-1",   true, disassembler.setCpu("SH-1"));
    EQUALS_P("cpu SH-1", "SH-1", disassembler.config().cpu_P());

    EQUALS("cpu SH-2",   true, disassembler.setCpu("SH-2"));
    EQUALS_P("cpu SH-2", "SH-2", disassembler.config().cpu_P());

    EQUALS("cpu SH-DSP",   true, disassembler.setCpu("SH-DSP"));
    EQUALS_P("cpu SH-DSP", "SH-DSP", disassembler.config().cpu_P());

    EQUALS("cpu SH-2E",   true, disassembler.setCpu("SH-2E"));
    EQUALS_P("cpu SH-2E", "SH-2E", disassembler.config().cpu_P());

    EQUALS("cpu SH-2A",   true, disassembler.setCpu("SH-2A"));
    EQUALS_P("cpu SH-2A", "SH-2A", disassembler.config().cpu_P());
}

// Representative positive tests for SH-1 (which SH-2 also inherits).  Covers
// each main instruction class with a few cases; exhaustive encoding is
// validated through the assembler tests.
void test_data_transfer() {
    // MOV #imm,Rn  (1110 nnnn iiiiiiii)
    TEST("MOV",   "#0, R0",      0xE000);
    TEST("MOV",   "#H'7F, R1",   0xE17F);
    TEST("MOV",   "#-H'80, R4", 0xE480);
    // MOV Rm,Rn  (0110 nnnn mmmm 0011)
    TEST("MOV",   "R0, R1",      0x6103);
    TEST("MOV",   "R14, R15",    0x6FE3);
    // MOV.B/W/L @Rm,Rn
    TEST("MOV.B", "@R1, R0",     0x6010);
    TEST("MOV.W", "@R5, R4",     0x6451);
    TEST("MOV.L", "@R9, R8",     0x6892);
    // MOV.B/W/L Rm,@Rn
    TEST("MOV.B", "R0, @R1",     0x2100);
    TEST("MOV.L", "R8, @R9",     0x2982);
    // Auto-increment / decrement
    TEST("MOV.B", "@R1+, R0",    0x6014);
    TEST("MOV.L", "R8, @-R9",    0x2986);
    // Indexed (@(R0,Rm))
    TEST("MOV.B", "@(R0,R1), R2",  0x021C);
    TEST("MOV.L", "R0, @(R0,R6)",  0x0606);
    // Displacement on Rn / GBR  (commas inside @() have NO space per SH style)
    TEST("MOV.L", "R0, @(0,R1)",          0x1100);
    TEST("MOV.L", "@(H'3C,R14), R15",     0x5FEF);
    TEST("MOV.B", "R0, @(H'00FF,GBR)",    0xC0FF);
    TEST("MOV.L", "@(H'03FC,GBR), R0",    0xC6FF);
    // PC-relative load: target = (PC+4) + d*2 for .W,
    //                            (PC+4) & ~3 + d*4 for .L / MOVA
    // PC-relative loads disassemble to the bare target (no "@(disp,PC)"):
    // the absolute address when no symbol is known, ...
    ATEST(0x0100, "MOV.W", "H'00000104, R3",          0x9300);
    ATEST(0x0100, "MOV.L", "H'00000104, R5",          0xD500);
    ATEST(0x0100, "MOVA",  "H'00000104, R0",          0xC700);
    // ... and the bare label when one is defined at the target.
    symtab.intern(0x0104, "litw");
    ATEST(0x0100, "MOV.W", "litw, R3",                0x9300);
    ATEST(0x0100, "MOV.L", "litw, R5",                0xD500);
    ATEST(0x0100, "MOVA",  "litw, R0",                0xC700);
    // MOVT
    TEST("MOVT",  "R0",                            0x0029);
    TEST("MOVT",  "R15",                           0x0F29);
}

void test_arith() {
    // ADD Rm,Rn / ADD #imm,Rn
    TEST("ADD",   "R0, R1",      0x310C);
    TEST("ADD",   "#1, R0",      0x7001);
    TEST("ADD",   "#-1, R15",    0x7FFF);
    // ADDC / ADDV
    TEST("ADDC",  "R0, R1",      0x310E);
    TEST("ADDV",  "R0, R1",      0x310F);
    // SUB / SUBC / SUBV
    TEST("SUB",   "R0, R1",      0x3108);
    TEST("SUBC",  "R0, R1",      0x310A);
    TEST("SUBV",  "R0, R1",      0x310B);
    // CMP/cond  (0011 nnnn mmmm 0000)
    TEST("CMP/EQ", "R0, R1",     0x3100);
    TEST("CMP/HS", "R0, R1",     0x3102);
    TEST("CMP/GE", "R0, R1",     0x3103);
    TEST("CMP/HI", "R0, R1",     0x3106);
    TEST("CMP/GT", "R0, R1",     0x3107);
    TEST("CMP/EQ", "#0, R0",     0x8800);
    TEST("CMP/EQ", "#-1, R0",    0x88FF);
    TEST("CMP/PZ", "R0",         0x4011);
    TEST("CMP/PL", "R0",         0x4015);
    TEST("CMP/STR", "R0, R1",    0x210C);
    // EXTU / EXTS
    TEST("EXTU.B", "R0, R1",     0x610C);
    TEST("EXTU.W", "R0, R1",     0x610D);
    TEST("EXTS.B", "R0, R1",     0x610E);
    TEST("EXTS.W", "R0, R1",     0x610F);
    // DIV / MAC.W / MUL
    TEST("DIV0S", "R0, R1",      0x2107);
    TEST("DIV0U", "",            0x0019);
    TEST("DIV1",  "R0, R1",      0x3104);
    TEST("MULS.W", "R0, R1",     0x210F);
    TEST("MULU.W", "R0, R1",     0x210E);
    TEST("MAC.W", "@R0+, @R1+",  0x410F);
    // NEG / NEGC
    TEST("NEG",   "R0, R1",      0x610B);
    TEST("NEGC",  "R0, R1",      0x610A);
    // SWAP / XTRCT
    TEST("SWAP.B", "R0, R1",     0x6108);
    TEST("SWAP.W", "R0, R1",     0x6109);
    TEST("XTRCT", "R0, R1",      0x210D);
}

void test_logic_shift() {
    // AND / OR / XOR / TST
    TEST("AND",   "R0, R1",        0x2109);
    TEST("AND",   "#-1, R0",         0xC9FF);
    TEST("AND.B", "#-1, @(R0,GBR)",  0xCDFF);
    TEST("OR",    "R0, R1",          0x210B);
    TEST("OR",    "#-H'80, R0",      0xCB80);
    TEST("OR.B",  "#-1, @(R0,GBR)",  0xCFFF);
    TEST("XOR",   "R0, R1",          0x210A);
    TEST("XOR",   "#H'7F, R0",       0xCA7F);
    TEST("XOR.B", "#H'7F, @(R0,GBR)", 0xCE7F);
    TEST("TST",   "R0, R1",          0x2108);
    TEST("TST",   "#-H'80, R0",      0xC880);
    TEST("TST.B", "#-H'80, @(R0,GBR)", 0xCC80);
    // NOT
    TEST("NOT",   "R0, R1",        0x6107);
    // SHAL / SHAR / SHLL / SHLR (single bit)
    TEST("SHAL",  "R0",            0x4020);
    TEST("SHAR",  "R0",            0x4021);
    TEST("SHLL",  "R0",            0x4000);
    TEST("SHLR",  "R0",            0x4001);
    TEST("SHLL2", "R0",            0x4008);
    TEST("SHLL8", "R0",            0x4018);
    TEST("SHLL16","R0",            0x4028);
    TEST("SHLR2", "R0",            0x4009);
    TEST("SHLR8", "R0",            0x4019);
    TEST("SHLR16","R0",            0x4029);
    // ROTL / ROTR / ROTCL / ROTCR
    TEST("ROTL",  "R0",            0x4004);
    TEST("ROTR",  "R0",            0x4005);
    TEST("ROTCL", "R0",            0x4024);
    TEST("ROTCR", "R0",            0x4025);
}

void test_branch() {
    // BF / BT  (1000 1011 dddddddd) -- target = (PC+4) + d*2
    ATEST(0x0100, "BF",    "H'00000104",  0x8B00);
    ATEST(0x0100, "BF",    "H'00000106",  0x8B01);
    ATEST(0x0100, "BF",    "H'00000102",  0x8BFF);  // backward
    ATEST(0x0100, "BT",    "H'00000104",  0x8900);
    ATEST(0x0100, "BT",    "H'00000200",  0x897E);
    // BRA / BSR  (1010 dddddddddddd) -- 12-bit signed disp, *2
    ATEST(0x0100, "BRA",   "H'00000104",  0xA000);
    ATEST(0x0100, "BRA",   "H'00000200",  0xA07E);
    ATEST(0x0100, "BRA",   "H'00000102",  0xAFFF);  // backward
    ATEST(0x0100, "BSR",   "H'00000104",  0xB000);
    // Indirect branches
    TEST("JMP",   "@R0",   0x402B);
    TEST("JMP",   "@R15",  0x4F2B);
    TEST("JSR",   "@R0",   0x400B);
    TEST("JSR",   "@R15",  0x4F0B);
    TEST("RTS",   "",      0x000B);
}

void test_system() {
    TEST("NOP",     "",       0x0009);
    TEST("CLRT",    "",       0x0008);
    TEST("SETT",    "",       0x0018);
    TEST("CLRMAC",  "",       0x0028);
    TEST("SLEEP",   "",       0x001B);
    TEST("RTE",     "",       0x002B);
    TEST("TAS.B",   "@R0",    0x401B);
    TEST("TAS.B",   "@R7",    0x471B);
    TEST("TRAPA",   "#0",     0xC300);
    TEST("TRAPA",   "#H'FF",  0xC3FF);
}

void test_control_regs() {
    // LDC Rm,SR/GBR/VBR
    TEST("LDC",   "R0, SR",        0x400E);
    TEST("LDC",   "R0, GBR",       0x401E);
    TEST("LDC",   "R0, VBR",       0x402E);
    // LDC.L @Rm+,SR/GBR/VBR
    TEST("LDC.L", "@R0+, SR",      0x4007);
    TEST("LDC.L", "@R0+, GBR",     0x4017);
    TEST("LDC.L", "@R0+, VBR",     0x4027);
    // LDS Rm,MACH/MACL/PR
    TEST("LDS",   "R0, MACH",      0x400A);
    TEST("LDS",   "R0, MACL",      0x401A);
    TEST("LDS",   "R0, PR",        0x402A);
    // STC SR/GBR/VBR,Rn
    TEST("STC",   "SR, R0",        0x0002);
    TEST("STC",   "GBR, R7",       0x0712);
    TEST("STC",   "VBR, R15",      0x0F22);
    // STC.L SR/GBR/VBR,@-Rn
    TEST("STC.L", "SR, @-R0",      0x4003);
    // STS MACH/MACL/PR,Rn
    TEST("STS",   "MACH, R0",      0x000A);
    TEST("STS",   "MACL, R7",      0x071A);
    TEST("STS",   "PR, R15",       0x0F2A);
    // STS.L MACH/MACL/PR,@-Rn
    TEST("STS.L", "MACH, @-R0",    0x4002);
}

// SH-2 adds DMULS.L / DMULU.L / MUL.L / MAC.L, DT, BRAF / BSRF, BT/S / BF/S.
void test_sh2_additions() {
    TEST("DMULS.L", "R0, R1",      0x310D);
    TEST("DMULU.L", "R0, R1",      0x3105);
    TEST("MUL.L",   "R0, R1",      0x0107);
    TEST("MAC.L",   "@R0+, @R1+",  0x010F);
    TEST("DT",      "R0",          0x4010);
    TEST("DT",      "R15",         0x4F10);
    TEST("BRAF",    "R0",          0x0023);
    TEST("BRAF",    "R7",          0x0723);
    TEST("BSRF",    "R0",          0x0003);
    TEST("BSRF",    "R15",         0x0F03);
    ATEST(0x0100, "BF/S", "H'00000104", 0x8F00);
    ATEST(0x0100, "BT/S", "H'00000104", 0x8D00);
}

// SH-DSP adds DSP-register transfers (DSR/A0/X0/X1/Y0/Y1) plus the
// repeat-control registers (MOD/RS/RE) and SETRC / LDRS / LDRE.
void test_shdsp_additions() {
    // DSP register transfers via LDS/STS
    TEST("LDS",   "R0, DSR",       0x406A);
    TEST("LDS",   "R7, A0",        0x477A);
    TEST("LDS",   "R15, X0",       0x4F8A);
    TEST("LDS",   "R0, X1",        0x409A);
    TEST("LDS",   "R7, Y0",        0x47AA);
    TEST("LDS",   "R15, Y1",       0x4FBA);
    TEST("STS",   "DSR, R0",       0x006A);
    TEST("STS",   "A0, R7",        0x077A);
    TEST("STS",   "Y1, R15",       0x0FBA);
    TEST("LDS.L", "@R0+, DSR",     0x4066);
    TEST("STS.L", "A0, @-R0",      0x4072);
    // Repeat-control registers via LDC/STC
    TEST("LDC",   "R0, MOD",       0x405E);
    TEST("LDC",   "R7, RS",        0x476E);
    TEST("LDC",   "R15, RE",       0x4F7E);
    TEST("STC",   "MOD, R0",       0x0052);
    TEST("STC",   "RE, R15",       0x0F72);
    // SETRC (Rm form and #imm form)
    TEST("SETRC", "#0",            0x8200);
    TEST("SETRC", "#H'7F",         0x827F);
    TEST("SETRC", "R0",            0x4014);
    TEST("SETRC", "R15",           0x4F14);
    // LDRS / LDRE disassemble as the bare PC-relative target.
    TEST("LDRS",  "H'00000004",    0x8C00);  // (PC+4) + 0
    TEST("LDRE",  "H'00000040",    0x8E1E);  // (PC+4) + 0x1E*2
    symtab.intern(0x0104, "loop");
    ATEST(0x0100, "LDRS", "loop",  0x8C00);  // label when defined at the target
}


// SH-2E adds single-precision FPU.
void test_sh2e_fpu() {
    // Arithmetic
    TEST("FADD",    "FR0, FR1",       0xF100);
    TEST("FADD",    "FR14, FR15",     0xFFE0);
    TEST("FSUB",    "FR0, FR1",       0xF101);
    TEST("FMUL",    "FR0, FR1",       0xF102);
    TEST("FDIV",    "FR0, FR1",       0xF103);
    // Compare
    TEST("FCMP/EQ", "FR0, FR1",       0xF104);
    TEST("FCMP/GT", "FR0, FR1",       0xF105);
    // Unary
    TEST("FABS",    "FR0",            0xF05D);
    TEST("FABS",    "FR15",           0xFF5D);
    TEST("FNEG",    "FR0",            0xF04D);
    TEST("FNEG",    "FR15",           0xFF4D);
    // Data transfer
    TEST("FMOV",    "FR0, FR1",       0xF10C);
    TEST("FMOV.S",  "@R0, FR0",       0xF008);
    TEST("FMOV.S",  "FR0, @R0",       0xF00A);
    TEST("FMOV.S",  "@R0+, FR0",      0xF009);
    TEST("FMOV.S",  "FR0, @-R0",      0xF00B);
    TEST("FMOV.S",  "@(R0,R1), FR0",  0xF016);
    TEST("FMOV.S",  "FR0, @(R0,R1)",  0xF107);
    // Constants
    TEST("FLDI0",   "FR0",            0xF08D);
    TEST("FLDI1",   "FR0",            0xF09D);
    // Conversion
    TEST("FLOAT",   "FPUL, FR0",      0xF02D);
    TEST("FTRC",    "FR0, FPUL",      0xF03D);
    // FMAC: dis omits the implicit FR0 source.
    TEST("FMAC",    "FR1, FR2",  0xF21E);
    TEST("FMAC",    "FR14, FR15",0xFFEE);
    // FLDS / FSTS
    TEST("FLDS",    "FR0, FPUL",      0xF01D);
    TEST("FSTS",    "FPUL, FR0",      0xF00D);
    // LDS/STS for FPUL / FPSCR
    TEST("LDS",    "R0, FPUL",        0x405A);
    TEST("LDS",    "R15, FPSCR",      0x4F6A);
    TEST("STS",    "FPUL, R0",        0x005A);
    TEST("STS",    "FPSCR, R15",      0x0F6A);
    TEST("LDS.L",  "@R0+, FPUL",      0x4056);
    TEST("STS.L",  "FPUL, @-R0",      0x4052);
}


// SH-2A adds CPU extensions (CLIPS/CLIPU, DIVS/DIVU, MULR, SHAD/SHLD, bank
// instructions, bit instructions, MOVI20 / MOVI20S) plus a few FPU additions
// (FCNVDS / FCNVSD on DR registers, requires --fpu).
void test_sh2a_additions() {
    // CPU extensions
    TEST("CLIPS.B", "R0",             0x4091);
    TEST("CLIPS.W", "R15",            0x4F95);
    TEST("CLIPU.B", "R0",             0x4081);
    TEST("CLIPU.W", "R15",            0x4F85);
    TEST("DIVS",    "R0, R1",         0x4194);
    TEST("DIVU",    "R0, R1",         0x4184);
    TEST("MULR",    "R0, R1",         0x4180);
    TEST("SHAD",    "R0, R1",         0x410C);
    TEST("SHLD",    "R0, R1",         0x410D);
    TEST("MOVRT",   "R0",             0x0039);
    TEST("NOTT",    "",               0x0068);
    TEST("RTS/N",   "",               0x006B);
    TEST("RTV/N",   "R0",             0x007B);
    TEST("PREF",    "@R0",            0x0083);
    TEST("RESBANK", "",               0x005B);
    TEST("JSR/N",   "@R0",            0x404B);
    TEST("JSR/N",   "@R15",           0x4F4B);
    TEST("LDBANK",  "@R0, R0",        0x40E5);
    TEST("LDBANK",  "@R15, R0",       0x4FE5);
    TEST("STBANK",  "R0, @R0",        0x40E1);
    TEST("STBANK",  "R0, @R15",       0x4FE1);
    TEST("MOVMU.L", "R0, @-R15",      0x40F0);
    TEST("MOVML.L", "R0, @-R15",      0x40F1);
    TEST("MOVMU.L", "@R15+, R0",      0x40F4);
    TEST("MOVML.L", "@R15+, R0",      0x40F5);
    // Bit instructions
    TEST("BCLR",    "#0, R0",         0x8600);
    TEST("BSET",    "#7, R15",        0x86FF);
    TEST("BLD",     "#0, R0",         0x8701);
    TEST("BST",     "#7, R15",        0x87FE);
    // MOVI20 / MOVI20S (two-word instructions; pass each word as uint16_t)
    TEST("MOVI20",  "#0, R0",                  uint16_t(0x0000), uint16_t(0x0000));
    TEST("MOVI20",  "#H'7FFFF, R1",            uint16_t(0x0170), uint16_t(0xFFFF));
    TEST("MOVI20",  "#-H'80000, R2",           uint16_t(0x0280), uint16_t(0x0000));
    TEST("MOVI20S", "#0, R0",                  uint16_t(0x0001), uint16_t(0x0000));
    TEST("MOVI20S", "#H'0FF0000, R1",          uint16_t(0x0101), uint16_t(0xFF00));
    // FPU additions (require --fpu enabled)
    disassembler.setOption("fpu", "true");
    TEST("FCNVDS",  "DR0, FPUL",      0xF0BD);
    TEST("FCNVDS",  "DR14, FPUL",     0xFEBD);
    TEST("FCNVSD",  "FPUL, DR0",      0xF0AD);
    TEST("FCNVSD",  "FPUL, DR14",     0xFEAD);
    disassembler.setOption("fpu", "false");
}

void test_illegal_sh1() {
    // Unmapped opcodes in the 0x0000..0x00FF region.
    SH_UNKN(0x0000);
    SH_UNKN(0x0001);
    SH_UNKN(0x000F);    // MAC.L (SH-2 only) -- illegal on SH-1
    // Zero-required nibbles in 0-operand instructions: nnnn must equal 0.
    SH_UNKN(0x0108);    // CLRT template + n=1 -- nnnn must be 0
    SH_UNKN(0x0109);    // NOP   template + n=1
    SH_UNKN(0x010B);    // RTS   template + n=1
    SH_UNKN(0x0118);    // SETT  template + n=1
    SH_UNKN(0x0119);    // DIV0U template + n=1
    SH_UNKN(0x011B);    // SLEEP template + n=1
    SH_UNKN(0x0128);    // CLRMAC template + n=1
    SH_UNKN(0x012B);    // RTE   template + n=1
    // STC variant index (bits[5:4]) only valid for 00/01/02; 03 is reserved.
    SH_UNKN(0x0032);    // STC ?,R0 with index 3
    // SH-2-only instructions must not decode on SH-1.
    SH_UNKN(0x0003);    // BSRF R0
    SH_UNKN(0x0007);    // MUL.L
    SH_UNKN(0x0023);    // BRAF R0
    SH_UNKN(0x3005);    // DMULU.L
    SH_UNKN(0x300D);    // DMULS.L
    SH_UNKN(0x4010);    // DT R0
    SH_UNKN(0x8D00);    // BT/S
    SH_UNKN(0x8F00);    // BF/S
}

void test_illegal_sh2() {
    // SH-2 inherits SH-1; 0x0000..0x0001 still reserved.
    SH_UNKN(0x0000);
    SH_UNKN(0x0001);
    // Same zero-required-nibble illegals as SH-1.
    SH_UNKN(0x0108);    // CLRT + n=1
    SH_UNKN(0x0109);    // NOP  + n=1
    SH_UNKN(0x010B);    // RTS  + n=1
    // STC variant index 3 is reserved on SH-2 too.
    SH_UNKN(0x0032);
    // SH-DSP-only DSP register transfers must not decode on SH-2.
    SH_UNKN(0x406A);    // LDS Rm,DSR (SH-DSP)
    SH_UNKN(0x407A);    // LDS Rm,A0  (SH-DSP)
    SH_UNKN(0x4014);    // SETRC Rm   (SH-DSP)
    SH_UNKN(0x8200);    // SETRC #imm (SH-DSP)
    SH_UNKN(0x8C00);    // LDRS       (SH-DSP)
    SH_UNKN(0x8E00);    // LDRE       (SH-DSP)
}

void test_illegal_shdsp() {
    SH_UNKN(0x0000);
    SH_UNKN(0x0001);
    SH_UNKN(0x0032);
    // The 0xF000 family belongs to SH-2E FPU; SH-DSP itself does not map it.
    SH_UNKN(0xF000);
    SH_UNKN(0xF001);
}

void test_illegal_sh2e() {
    SH_UNKN(0x0000);
    SH_UNKN(0x0001);
    SH_UNKN(0x0032);
    // FPU sub-op nibble 0x0F is reserved (FADD..FMAC use 0..E).
    SH_UNKN(0xF00F);
    SH_UNKN(0xFF0F);
    // SH-DSP register-transfer instructions must not decode on SH-2E.
    SH_UNKN(0x4014);    // SETRC Rm
    SH_UNKN(0x8200);    // SETRC #imm
    SH_UNKN(0x8C00);    // LDRS
    SH_UNKN(0x8E00);    // LDRE
}

void test_illegal_sh2a() {
    // With the FPU disabled the FPU pages aren't searched; FCNVDS / FCNVSD
    // and the SH-2E FPU instructions must all be rejected. (The disassembler
    // enables the FPU by default, so turn it off explicitly here.)
    disassembler.setOption("fpu", "false");
    SH_UNKN(0xF0BD);    // FCNVDS DR0,FPUL (needs FPU_SH2A)
    SH_UNKN(0xF0AD);    // FCNVSD FPUL,DR0 (needs FPU_SH2A)
    SH_UNKN(0xF000);    // FADD FR0,FR0    (needs FPU)
    SH_UNKN(0xF005);    // FCMP/GT FR0,FR0 (needs FPU)
    SH_UNKN(0xF09D);    // FLDI1 FR0       (needs FPU)
}

// clang-format on

void run_tests(const char *cpu) {
    disassembler.setCpu(cpu);
    // Positive coverage common to SH-1 / SH-2 (and SH-DSP / SH-2E / SH-2A
    // which all inherit the SH-1 instruction set).
    RUN_TEST(test_data_transfer);
    RUN_TEST(test_arith);
    RUN_TEST(test_logic_shift);
    RUN_TEST(test_branch);
    RUN_TEST(test_system);
    RUN_TEST(test_control_regs);
    if (!isSh1())
        RUN_TEST(test_sh2_additions);
    if (isShDsp())
        RUN_TEST(test_shdsp_additions);
    if (isSh2e())
        RUN_TEST(test_sh2e_fpu);
    if (isSh2a())
        RUN_TEST(test_sh2a_additions);
    // Per-CPU illegal-instruction probes.
    if (isSh1())
        RUN_TEST(test_illegal_sh1);
    if (isSh2())
        RUN_TEST(test_illegal_sh2);
    if (isShDsp())
        RUN_TEST(test_illegal_shdsp);
    if (isSh2e())
        RUN_TEST(test_illegal_sh2e);
    if (isSh2a())
        RUN_TEST(test_illegal_sh2a);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

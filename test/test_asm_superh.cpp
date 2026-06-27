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

#include "asm_superh.h"
#include "test_asm_helper.h"

using namespace libasm;
using namespace libasm::superh;
using namespace libasm::test;

AsmSuperH asmsuperh;
Assembler &assembler(asmsuperh);

bool isSh2() {
    const auto *cpu = assembler.config().cpu_P();
    return strcasecmp_P("SH-2", cpu) == 0 || strcasecmp_P("SH-DSP", cpu) == 0 ||
           strcasecmp_P("SH-2E", cpu) == 0 || strcasecmp_P("SH-2A", cpu) == 0;
}

void set_up() {
    assembler.reset();
}

void tear_down() {
    symtab.reset();
}

// clang-format off
// The TEST/ATEST/ERRT macros take the expected bytes as Config::opcode_t
// values; SH's opcode_t is uint16_t, so each variadic argument is one
// big-endian 16-bit opcode (not two bytes).
void test_cpu() {
    EQUALS("cpu SH-1", true, assembler.setCpu("SH-1"));
    EQUALS_P("cpu SH-1", "SH-1", assembler.config().cpu_P());

    EQUALS("cpu SH-2", true, assembler.setCpu("SH-2"));
    EQUALS_P("cpu SH-2", "SH-2", assembler.config().cpu_P());
}

void test_mov_imm() {
    // MOV #imm,Rn  (1110 nnnn iiiiiiii)
    TEST("MOV #0, R0",        0xE000);
    TEST("MOV #H'7F, R1",     0xE17F);
    TEST("MOV #-H'80, R2",    0xE280);
    TEST("MOV #-1, R15",      0xEFFF);
    TEST("MOV #127, R3",      0xE37F);
    TEST("MOV #-128, R4",     0xE480);
    // The MOV/ADD/CMP family takes a signed 8-bit immediate that the CPU
    // sign-extends to 32-bit; the AND/OR/XOR/TST family takes an unsigned
    // 8-bit immediate. Both share M_IMM8 in the table, so the encoder
    // accepts the union range [-128, 255] and lets the user's choice of
    // mnemonic clarify intent. The bit pattern matches the SH manual: e.g.
    // "MOV #H'FF" and "MOV #-1" both encode to 0xEFFF.
    TEST("MOV #H'FF, R0",     0xE0FF);  // accepted as 0xFF == -1 sign-extended
}

void test_mov_reg() {
    // MOV Rm,Rn  (0110 nnnn mmmm 0011)
    TEST("MOV R0, R1",     0x6103);
    TEST("MOV R14, R15",   0x6FE3);
    TEST("MOV R5, R5",     0x6553);
}

void test_mov_indirect() {
    TEST("MOV.B @R1, R0",    0x6010);
    TEST("MOV.B @R3, R2",    0x6230);
    TEST("MOV.W @R5, R4",    0x6451);
    TEST("MOV.L @R9, R8",    0x6892);
    TEST("MOV.L @R11, R10",  0x6AB2);
    TEST("MOV.B R0, @R1",    0x2100);
    TEST("MOV.W R4, @R5",    0x2541);
    TEST("MOV.L R8, @R9",    0x2982);
}

void test_mov_inc_dec() {
    TEST("MOV.B @R1+, R0",   0x6014);
    TEST("MOV.W @R5+, R4",   0x6455);
    TEST("MOV.L @R9+, R8",   0x6896);
    TEST("MOV.B R0, @-R1",   0x2104);
    TEST("MOV.W R4, @-R5",   0x2545);
    TEST("MOV.L R8, @-R9",   0x2986);
    // SP is an input alias for R15 (disassembly emits R15).
    TEST("MOV.L @SP+, R8",   0x68F6);
    TEST("MOV.L R8, @-SP",   0x2F86);
}

void test_mov_indexed() {
    // MOV.B/W/L @(R0,Rm),Rn  (0000 nnnn mmmm 110x)
    TEST("MOV.B @(R0,R1), R2",     0x021C);
    TEST("MOV.B @(R0,R13), R14",   0x0EDC);
    TEST("MOV.W @(R0,R3), R4",     0x043D);
    TEST("MOV.L @(R0,R5), R6",     0x065E);
    // MOV.B/W/L Rm,@(R0,Rn)  (0000 nnnn mmmm 010x)
    TEST("MOV.B R0, @(R0,R2)",     0x0204);
    TEST("MOV.B R7, @(R0,R15)",    0x0F74);
    TEST("MOV.W R0, @(R0,R4)",     0x0405);
    TEST("MOV.L R0, @(R0,R6)",     0x0606);
}

void test_mov_disp_reg() {
    // MOV.B R0,@(disp,Rn) (1000 0000 nnnn dddd)
    TEST("MOV.B R0, @(0, R0)",      0x8000);
    TEST("MOV.B R0, @(7, R4)",      0x8047);
    TEST("MOV.B R0, @(15, R15)",    0x80FF);
    // MOV.W R0,@(disp,Rn) disp scaled by 2
    TEST("MOV.W R0, @(0, R0)",      0x8100);
    TEST("MOV.W R0, @(14, R5)",     0x8157);
    TEST("MOV.W R0, @(30, R15)",    0x81FF);
    // MOV.B/W @(disp,Rm),R0
    TEST("MOV.B @(0, R0), R0",      0x8400);
    TEST("MOV.B @(7, R6), R0",      0x8467);
    TEST("MOV.W @(0, R0), R0",      0x8500);
    TEST("MOV.W @(14, R7), R0",     0x8577);
    // MOV.L Rm,@(disp,Rn) (0001 nnnn mmmm dddd) disp scaled by 4
    TEST("MOV.L R0, @(0, R1)",      0x1100);
    TEST("MOV.L R5, @(8, R6)",      0x1652);
    TEST("MOV.L R14, @(60, R15)",   0x1FEF);
    // MOV.L @(disp,Rm),Rn
    TEST("MOV.L @(0, R0), R1",      0x5100);
    TEST("MOV.L @(8, R7), R8",      0x5872);
    TEST("MOV.L @(60, R14), R15",   0x5FEF);
    // Misalignment / out-of-range
    ERRT("MOV.W R0, @(1, R5)",  OPERAND_NOT_ALIGNED, "@(1, R5)",  0x8150);
    ERRT("MOV.L R0, @(2, R5)",  OPERAND_NOT_ALIGNED, "@(2, R5)",  0x1500);
    ERRT("MOV.B R0, @(16, R5)", OVERFLOW_RANGE,      "@(16, R5)", 0x8050);
    ERRT("MOV.L R0, @(64, R5)", OVERFLOW_RANGE,      "@(64, R5)", 0x1500);
}

void test_mov_disp_gbr() {
    // MOV.B R0,@(disp,GBR) (1100 0000 dddd dddd)
    TEST("MOV.B R0, @(0, GBR)",       0xC000);
    TEST("MOV.B R0, @(H'80, GBR)",    0xC080);
    TEST("MOV.B R0, @(H'FF, GBR)",    0xC0FF);
    // MOV.W R0,@(disp,GBR) disp/2
    TEST("MOV.W R0, @(0, GBR)",       0xC100);
    TEST("MOV.W R0, @(H'100, GBR)",   0xC180);
    TEST("MOV.W R0, @(H'1FE, GBR)",   0xC1FF);
    // MOV.L R0,@(disp,GBR) disp/4
    TEST("MOV.L R0, @(0, GBR)",       0xC200);
    TEST("MOV.L R0, @(H'200, GBR)",   0xC280);
    TEST("MOV.L R0, @(H'3FC, GBR)",   0xC2FF);
    // Load forms
    TEST("MOV.B @(0, GBR), R0",       0xC400);
    TEST("MOV.B @(H'FF, GBR), R0",    0xC4FF);
    TEST("MOV.W @(0, GBR), R0",       0xC500);
    TEST("MOV.W @(H'1FE, GBR), R0",   0xC5FF);
    TEST("MOV.L @(0, GBR), R0",       0xC600);
    TEST("MOV.L @(H'3FC, GBR), R0",   0xC6FF);
    // Out-of-range / misalignment
    ERRT("MOV.B R0, @(H'100, GBR)", OVERFLOW_RANGE,      "@(H'100, GBR)", 0xC000);
    ERRT("MOV.W R0, @(1, GBR)",     OPERAND_NOT_ALIGNED, "@(1, GBR)",     0xC100);
    ERRT("MOV.L R0, @(2, GBR)",     OPERAND_NOT_ALIGNED, "@(2, GBR)",     0xC200);
}

void test_mov_pcrel() {
    // Two PC-relative operand forms (manual table 3-1):
    //   @(disp,PC)            -- disp is a raw byte displacement; field = disp/scale.
    //   bare symbol / address -- the operand is the target; the assembler derives
    //                            the field (.W: target-(PC+4); .L/MOVA: -((PC+4)&~3)).
    // .W scales by 2, .L and MOVA scale by 4.

    // @(disp,PC): raw displacement.
    ATEST(0x0100, "MOV.W @(0, PC), R0",      0x9000);  // field 0
    ATEST(0x0100, "MOV.W @(4, PC), R3",      0x9302);  // field 4/2=2
    ATEST(0x0100, "MOV.W @(H'1FE, PC), R7",  0x97FF);  // field 510/2=255 (max)
    ATEST(0x0100, "MOV.L @(0, PC), R0",      0xD000);
    ATEST(0x0100, "MOV.L @(8, PC), R5",      0xD502);  // field 8/4=2
    ATEST(0x0100, "MOV.L @(H'3FC, PC), R9",  0xD9FF);  // field 1020/4=255
    ATEST(0x0100, "MOVA @(0, PC), R0",       0xC700);
    ATEST(0x0100, "MOVA @(4, PC), R0",       0xC701);
    // @(disp,PC) errors: misaligned disp, out of range.
    AERRT(0x0100, "MOV.W @(1, PC), R0",     OPERAND_NOT_ALIGNED, "@(1, PC), R0",     0x9000);
    AERRT(0x0100, "MOV.L @(2, PC), R0",     OPERAND_NOT_ALIGNED, "@(2, PC), R0",     0xD000);
    AERRT(0x0100, "MOV.W @(H'200, PC), R0", OPERAND_TOO_FAR,     "@(H'200, PC), R0", 0x9000);

    // Bare symbol/address target form. .W: field = (target-(PC+4))/2.
    ATEST(0x0100, "MOV.W H'104, R0",   0x9000);  // target 0x104 -> field 0
    ATEST(0x0100, "MOV.W H'106, R3",   0x9301);  // field (0x106-0x104)/2=1
    ATEST(0x0100, "MOV.W $+4, R0",     0x9000);  // $=PC=0x100 -> target 0x104
    ATEST(0x0100, "MOV.L H'104, R0",   0xD000);
    ATEST(0x0100, "MOV.L H'108, R5",   0xD501);  // field (0x108-0x104)/4=1
    ATEST(0x0100, "MOVA H'104, R0",    0xC700);
    // PC need not be 4-aligned for .L/MOVA: base is (PC+4)&~3.
    ATEST(0x0102, "MOV.L H'108, R0",   0xD001);
    // Bare-target errors: misaligned target, backward (before PC+4).
    AERRT(0x0100, "MOV.L H'106, R0", OPERAND_NOT_ALIGNED, "H'106, R0", 0xD000);
    AERRT(0x0100, "MOV.W H'100, R0", OPERAND_TOO_FAR,     "H'100, R0", 0x9000);

    // Bare "symbol" form with a defined label.
    symtab.intern(0x0104, "litw");
    ATEST(0x0100, "MOV.W litw, R0",  0x9000);
    ATEST(0x0100, "MOV.L litw, R1",  0xD100);
    ATEST(0x0100, "MOVA litw, R0",   0xC700);

    // Undefined target: the displacement field is left zero.
    AERUS(0x0100, "MOV.W UNDEF, R0",  "UNDEF, R0",  0x9000);
    AERUS(0x0100, "MOV.L UNDEF, R1",  "UNDEF, R1",  0xD100);
    AERUS(0x0100, "MOVA UNDEF, R0",   "UNDEF, R0",  0xC700);
}

void test_movt() {
    TEST("MOVT R0",     0x0029);
    TEST("MOVT R7",     0x0729);
    TEST("MOVT R15",    0x0F29);
}

void test_add() {
    // ADD Rm,Rn (0011 nnnn mmmm 1100)
    TEST("ADD R0, R1",    0x310C);
    TEST("ADD R14, R15",  0x3FEC);
    // ADD #imm,Rn (0111 nnnn iiiiiiii) 8-bit signed
    TEST("ADD #0, R0",        0x7000);
    TEST("ADD #1, R1",        0x7101);
    TEST("ADD #H'7F, R7",     0x777F);
    TEST("ADD #-1, R8",       0x78FF);
    TEST("ADD #-H'80, R15",   0x7F80);
}

void test_addc_addv_sub() {
    TEST("ADDC R0, R1",   0x310E);
    TEST("ADDC R14, R15", 0x3FEE);
    TEST("ADDV R0, R1",   0x310F);
    TEST("SUB R0, R1",    0x3108);
    TEST("SUBC R0, R1",   0x310A);
    TEST("SUBV R0, R1",   0x310B);
}

void test_cmp() {
    // CMP/EQ #imm,R0  (1000 1000 iiiiiiii)
    TEST("CMP/EQ #0, R0",     0x8800);
    TEST("CMP/EQ #H'7F, R0",  0x887F);
    TEST("CMP/EQ #-1, R0",    0x88FF);
    // CMP/EQ Rm,Rn  (0011 nnnn mmmm 0000)
    TEST("CMP/EQ R0, R1",     0x3100);
    TEST("CMP/EQ R14, R15",   0x3FE0);
    // CMP/GE/GT/HI/HS
    TEST("CMP/GE R0, R1",     0x3103);
    TEST("CMP/GT R0, R1",     0x3107);
    TEST("CMP/HI R0, R1",     0x3106);
    TEST("CMP/HS R0, R1",     0x3102);
    // CMP/PL Rn  (0100 nnnn 0001 0101)
    TEST("CMP/PL R0",         0x4015);
    TEST("CMP/PL R15",        0x4F15);
    TEST("CMP/PZ R0",         0x4011);
    // CMP/STR Rm,Rn
    TEST("CMP/STR R0, R1",    0x210C);
}

void test_div_ext() {
    TEST("DIV0S R0, R1",  0x2107);
    TEST("DIV0U",         0x0019);
    TEST("DIV1 R0, R1",   0x3104);
    TEST("EXTS.B R0, R1", 0x610E);
    TEST("EXTS.W R0, R1", 0x610F);
    TEST("EXTU.B R0, R1", 0x610C);
    TEST("EXTU.W R0, R1", 0x610D);
}

void test_mul() {
    TEST("MAC.W @R0+, @R1+",    0x410F);
    TEST("MAC.W @R14+, @R15+",  0x4FEF);
    TEST("MULS.W R0, R1",       0x210F);
    TEST("MULU.W R0, R1",       0x210E);
}

void test_neg_swap_xtrct() {
    TEST("NEG R0, R1",    0x610B);
    TEST("NEGC R0, R1",   0x610A);
    TEST("SWAP.B R0, R1", 0x6108);
    TEST("SWAP.W R0, R1", 0x6109);
    TEST("XTRCT R0, R1",  0x210D);
}

void test_logic() {
    // AND
    TEST("AND R0, R1",                  0x2109);
    TEST("AND #0, R0",                  0xC900);
    TEST("AND #H'AA, R0",               0xC9AA);
    TEST("AND #H'FF, R0",               0xC9FF);
    TEST("AND.B #0, @(R0, GBR)",        0xCD00);
    TEST("AND.B #H'AA, @(R0, GBR)",     0xCDAA);
    // OR
    TEST("OR R0, R1",                   0x210B);
    TEST("OR #H'55, R0",                0xCB55);
    TEST("OR.B #H'55, @(R0, GBR)",      0xCF55);
    // NOT
    TEST("NOT R0, R1",                  0x6107);
    // TST
    TEST("TST R0, R1",                  0x2108);
    TEST("TST #H'FF, R0",               0xC8FF);
    TEST("TST.B #H'FF, @(R0, GBR)",     0xCCFF);
    // XOR
    TEST("XOR R0, R1",                  0x210A);
    TEST("XOR #H'0F, R0",               0xCA0F);
    TEST("XOR.B #H'0F, @(R0, GBR)",     0xCE0F);
    // TAS.B
    TEST("TAS.B @R0",                   0x401B);
    TEST("TAS.B @R15",                  0x4F1B);
}

void test_shift_rotate() {
    TEST("ROTL R0",   0x4004);
    TEST("ROTR R0",   0x4005);
    TEST("ROTCL R0",  0x4024);
    TEST("ROTCR R0",  0x4025);
    TEST("SHAL R0",   0x4020);
    TEST("SHAR R0",   0x4021);
    TEST("SHLL R0",   0x4000);
    TEST("SHLR R0",   0x4001);
    TEST("SHLL2 R0",  0x4008);
    TEST("SHLR2 R0",  0x4009);
    TEST("SHLL8 R0",  0x4018);
    TEST("SHLR8 R0",  0x4019);
    TEST("SHLL16 R0", 0x4028);
    TEST("SHLR16 R0", 0x4029);
}

void test_branch() {
    // BF label  (1000 1011 dddddddd)  d = (target - (PC+4)) / 2
    ATEST(0x0100, "BF H'104",     0x8B00);
    ATEST(0x0100, "BF H'106",     0x8B01);
    ATEST(0x0100, "BF H'102",     0x8BFF);  // backward (delta=-2 -> d=-1=0xFF)
    // BT label
    ATEST(0x0100, "BT H'104",     0x8900);
    ATEST(0x0100, "BT H'200",     0x897E);
    // BRA label (1010 dddddddddddd)
    ATEST(0x0100, "BRA H'104",    0xA000);
    ATEST(0x0100, "BRA H'200",    0xA07E);
    ATEST(0x0100, "BRA H'102",    0xAFFF);  // backward (delta=-2 -> d=-1=0xFFF)
    // BSR label
    ATEST(0x0100, "BSR H'104",    0xB000);
    ATEST(0x0100, "BSR H'200",    0xB07E);
    // Out-of-range branches.
    AERRT(0x0100, "BF H'400",    OPERAND_TOO_FAR, "H'400",  0x8B7E);
    AERRT(0x0100, "BRA H'2100", OPERAND_TOO_FAR, "H'2100", 0xAFFE);
}

void test_jmp_jsr_rts() {
    TEST("JMP @R0",   0x402B);
    TEST("JMP @R7",   0x472B);
    TEST("JMP @R15",  0x4F2B);
    TEST("JSR @R0",   0x400B);
    TEST("JSR @R15",  0x4F0B);
    TEST("RTS",       0x000B);
}

void test_system_control() {
    TEST("CLRMAC",  0x0028);
    TEST("CLRT",    0x0008);
    TEST("NOP",     0x0009);
    TEST("RTE",     0x002B);
    TEST("SETT",    0x0018);
    TEST("SLEEP",   0x001B);
    TEST("TRAPA #0",     0xC300);
    TEST("TRAPA #H'20",  0xC320);
    TEST("TRAPA #H'FF",  0xC3FF);
}

void test_ldc_stc_lds_sts() {
    // LDC Rm,SR/GBR/VBR
    TEST("LDC R0, SR",       0x400E);
    TEST("LDC R7, SR",       0x470E);
    TEST("LDC R0, GBR",      0x401E);
    TEST("LDC R0, VBR",      0x402E);
    // LDC.L @Rm+,SR/GBR/VBR
    TEST("LDC.L @R0+, SR",   0x4007);
    TEST("LDC.L @R0+, GBR",  0x4017);
    TEST("LDC.L @R0+, VBR",  0x4027);
    // LDS Rm,MACH/MACL/PR
    TEST("LDS R0, MACH",     0x400A);
    TEST("LDS R0, MACL",     0x401A);
    TEST("LDS R0, PR",       0x402A);
    // LDS.L @Rm+,MACH/MACL/PR
    TEST("LDS.L @R0+, MACH", 0x4006);
    TEST("LDS.L @R0+, MACL", 0x4016);
    TEST("LDS.L @R0+, PR",   0x4026);
    // STC SR/GBR/VBR,Rn
    TEST("STC SR, R0",       0x0002);
    TEST("STC GBR, R7",      0x0712);
    TEST("STC VBR, R15",     0x0F22);
    // STC.L SR/GBR/VBR,@-Rn
    TEST("STC.L SR, @-R0",   0x4003);
    TEST("STC.L GBR, @-R7",  0x4713);
    TEST("STC.L VBR, @-R15", 0x4F23);
    // STS MACH/MACL/PR,Rn
    TEST("STS MACH, R0",     0x000A);
    TEST("STS MACL, R7",     0x071A);
    TEST("STS PR, R15",      0x0F2A);
    // STS.L MACH/MACL/PR,@-Rn
    TEST("STS.L MACH, @-R0", 0x4002);
    TEST("STS.L MACL, @-R7", 0x4712);
    TEST("STS.L PR, @-R15",  0x4F22);
}

void test_sh2_only() {
    if (isSh2()) {
        TEST("DMULS.L R0, R1",   0x310D);
        TEST("DMULU.L R0, R1",   0x3105);
        TEST("MUL.L R0, R1",     0x0107);
        TEST("MAC.L @R0+, @R1+", 0x010F);
        TEST("DT R0",            0x4010);
        TEST("DT R15",           0x4F10);
        TEST("BRAF R0",          0x0023);
        TEST("BRAF R7",          0x0723);
        TEST("BSRF R0",          0x0003);
        TEST("BSRF R15",         0x0F03);
        ATEST(0x0100, "BF/S H'104", 0x8F00);
        ATEST(0x0100, "BT/S H'104", 0x8D00);
    } else {
        ERRT("DMULS.L R0, R1",    UNKNOWN_INSTRUCTION, "DMULS.L R0, R1");
        ERRT("MUL.L R0, R1",      UNKNOWN_INSTRUCTION, "MUL.L R0, R1");
        ERRT("MAC.L @R0+, @R1+",  UNKNOWN_INSTRUCTION, "MAC.L @R0+, @R1+");
        ERRT("DT R0",             UNKNOWN_INSTRUCTION, "DT R0");
        ERRT("BRAF R0",           UNKNOWN_INSTRUCTION, "BRAF R0");
        ERRT("BSRF R0",           UNKNOWN_INSTRUCTION, "BSRF R0");
    }
}

bool isShDsp() {
    return strcasecmp_P("SH-DSP", assembler.config().cpu_P()) == 0;
}

bool isSh2e() {
    return strcasecmp_P("SH-2E", assembler.config().cpu_P()) == 0;
}

bool isSh2a() {
    return strcasecmp_P("SH-2A", assembler.config().cpu_P()) == 0;
}

// The FPU is usable only when compiled in (LIBASM_SUPERH_NOFPU keeps fpuType()
// at FPU_NONE) and enabled for the current CPU.
bool is_fpu_ready() {
    return asmsuperh.fpuType() != FPU_NONE;
}

void test_shdsp_only() {
    if (isShDsp()) {
        // DSP register transfers
        TEST("LDS R0, DSR",       0x406A);
        TEST("LDS R7, A0",        0x477A);
        TEST("LDS R15, X0",       0x4F8A);
        TEST("LDS R0, X1",        0x409A);
        TEST("LDS R7, Y0",        0x47AA);
        TEST("LDS R15, Y1",       0x4FBA);
        TEST("STS DSR, R0",       0x006A);
        TEST("STS A0, R7",        0x077A);
        TEST("STS X0, R15",       0x0F8A);
        TEST("STS X1, R0",        0x009A);
        TEST("STS Y0, R7",        0x07AA);
        TEST("STS Y1, R15",       0x0FBA);
        TEST("LDS.L @R0+, DSR",   0x4066);
        TEST("LDS.L @R15+, Y1",   0x4FB6);
        TEST("STS.L A0, @-R0",    0x4072);
        TEST("STS.L Y1, @-R15",   0x4FB2);
        // Repeat-control registers
        TEST("LDC R0, MOD",       0x405E);
        TEST("LDC R7, RS",        0x476E);
        TEST("LDC R15, RE",       0x4F7E);
        TEST("STC MOD, R0",       0x0052);
        TEST("STC RS, R7",        0x0762);
        TEST("STC RE, R15",       0x0F72);
        TEST("LDC.L @R0+, MOD",   0x4057);
        TEST("STC.L RE, @-R15",   0x4F73);
        // LDRS / LDRE / SETRC. Bare symbol/address = target; @(disp,PC) = raw.
        ATEST(0x0100, "LDRS $+4",            0x8C00);  // target -> field 0
        ATEST(0x0100, "LDRE $+4",            0x8E00);
        ATEST(0x0100, "LDRS $+H'40",         0x8C1E);  // (0x40-4)/2 = 0x1E
        ATEST(0x0100, "LDRS @(4, PC)",       0x8C02);  // raw disp 4 -> field 2
        ATEST(0x0100, "LDRS @(-2, PC)",      0x8CFF);  // raw disp -2 -> field -1
        TEST("SETRC #0",          0x8200);
        TEST("SETRC #H'7F",       0x827F);
        TEST("SETRC #H'FF",       0x82FF);
        TEST("SETRC R0",          0x4014);
        TEST("SETRC R15",         0x4F14);
    } else {
        // LDS/STS/LDC/STC are SH-1/SH-2 mnemonics too; the operand fails
        // rather than the mnemonic, so just check the DSP-only ones.
        ERRT("LDRS $+4",          UNKNOWN_INSTRUCTION, "LDRS $+4");
        ERRT("LDRE $+4",          UNKNOWN_INSTRUCTION, "LDRE $+4");
        ERRT("SETRC #0",          UNKNOWN_INSTRUCTION, "SETRC #0");
        ERRT("SETRC R0",          UNKNOWN_INSTRUCTION, "SETRC R0");
    }
}

void test_sh2e_only() {
    // The FPU is mandatory on the SH-2E, so it is active here unless compiled
    // out with LIBASM_SUPERH_NOFPU (then fpuType() stays FPU_NONE and the
    // floating-point instructions are unknown, like on a non-FPU CPU).
    if (isSh2e() && is_fpu_ready()) {
        // Arithmetic
        TEST("FADD FR0, FR1",     0xF100);
        TEST("FADD FR14, FR15",   0xFFE0);
        TEST("FSUB FR0, FR1",     0xF101);
        TEST("FMUL FR0, FR1",     0xF102);
        TEST("FDIV FR0, FR1",     0xF103);
        // Compare
        TEST("FCMP/EQ FR0, FR1",  0xF104);
        TEST("FCMP/GT FR0, FR1",  0xF105);
        // Unary
        TEST("FABS FR0",          0xF05D);
        TEST("FABS FR15",         0xFF5D);
        TEST("FNEG FR0",          0xF04D);
        TEST("FNEG FR15",         0xFF4D);
        // Data transfer
        TEST("FMOV FR0, FR1",     0xF10C);
        TEST("FMOV.S @R0, FR0",   0xF008);
        TEST("FMOV.S FR0, @R0",   0xF00A);
        TEST("FMOV.S @R0+, FR0",  0xF009);
        TEST("FMOV.S FR0, @-R0",  0xF00B);
        TEST("FMOV.S @(R0,R1), FR0", 0xF016);
        TEST("FMOV.S FR0, @(R0,R1)", 0xF107);
        // Constants
        TEST("FLDI0 FR0",         0xF08D);
        TEST("FLDI1 FR0",         0xF09D);
        // Conversion
        TEST("FLOAT FPUL, FR0",   0xF02D);
        TEST("FTRC FR0, FPUL",    0xF03D);
        // FMAC (3-operand form with explicit FR0)
        TEST("FMAC FR0, FR1, FR2",   0xF21E);
        TEST("FMAC FR0, FR14, FR15", 0xFFEE);
        // System
        TEST("FLDS FR0, FPUL",    0xF01D);
        TEST("FSTS FPUL, FR0",    0xF00D);
        // LDS/STS FPUL/FPSCR
        TEST("LDS R0, FPUL",      0x405A);
        TEST("LDS R15, FPSCR",    0x4F6A);
        TEST("STS FPUL, R0",      0x005A);
        TEST("STS FPSCR, R15",    0x0F6A);
        TEST("LDS.L @R0+, FPUL",  0x4056);
        TEST("STS.L FPUL, @-R0",  0x4052);
        TEST("LDS.L @R0+, FPSCR", 0x4066);
        TEST("STS.L FPSCR, @-R0", 0x4062);
    } else {
        ERRT("FADD FR0, FR1",     UNKNOWN_INSTRUCTION, "FADD FR0, FR1");
        ERRT("FABS FR0",          UNKNOWN_INSTRUCTION, "FABS FR0");
        ERRT("FMOV FR0, FR1",     UNKNOWN_INSTRUCTION, "FMOV FR0, FR1");
        ERRT("FLDI0 FR0",         UNKNOWN_INSTRUCTION, "FLDI0 FR0");
    }
}

void test_sh2a_only() {
    if (isSh2a()) {
        // SH-2A 16-bit CPU additions
        TEST("CLIPS.B R0",         0x4091);
        TEST("CLIPS.W R15",        0x4F95);
        TEST("CLIPU.B R0",         0x4081);
        TEST("CLIPU.W R15",        0x4F85);
        TEST("DIVS R0, R1",        0x4194);
        TEST("DIVU R0, R1",        0x4184);
        TEST("MULR R0, R1",        0x4180);
        TEST("SHAD R0, R1",        0x410C);
        TEST("SHLD R0, R1",        0x410D);
        TEST("MOVRT R0",           0x0039);
        TEST("NOTT",               0x0068);
        TEST("RTS/N",              0x006B);
        TEST("RTV/N R0",           0x007B);
        TEST("PREF @R0",           0x0083);
        TEST("RESBANK",            0x005B);
        TEST("JSR/N @R0",          0x404B);
        TEST("STBANK R0, @R0",     0x40E1);
        TEST("LDBANK @R0, R0",     0x40E5);
        TEST("MOVMU.L R0, @-R15",  0x40F0);
        TEST("MOVML.L R0, @-R15",  0x40F1);
        TEST("MOVMU.L @R15+, R0",  0x40F4);
        TEST("MOVML.L @R15+, R0",  0x40F5);
        TEST("BCLR #0, R0",        0x8600);
        TEST("BSET #7, R15",       0x86FF);
        TEST("BLD #0, R0",         0x8701);
        TEST("BST #7, R15",        0x87FE);
        // SH-2A 32-bit CPU additions (MOVI20 / MOVI20S).
        // Use uint16_t casts so the two 16-bit opcode words in a long-form
        // instruction read clearly as a pair of words.
        TEST("MOVI20 #0, R0",          uint16_t(0x0000), uint16_t(0x0000));
        TEST("MOVI20 #H'7FFFF, R1",    uint16_t(0x0170), uint16_t(0xFFFF));
        TEST("MOVI20 #-H'80000, R2",   uint16_t(0x0280), uint16_t(0x0000));
        TEST("MOVI20S #0, R0",         uint16_t(0x0001), uint16_t(0x0000));
        TEST("MOVI20S #H'FF0000, R1",  uint16_t(0x0101), uint16_t(0xFF00));
        // Without the FPU the SH-2A has no FPU system registers, so the
        // FPU-related CPU instructions (LDS/STS FPUL/FPSCR) are undefined code.
        ERRT("LDS R0, FPUL",       OPERAND_NOT_ALLOWED, "R0, FPUL");
        ERRT("STS.L FPSCR, @-R0",  OPERAND_NOT_ALLOWED, "FPSCR, @-R0");
        // The FPU-related CPU instructions and the SH-2A FPU additions require
        // the FPU (--fpu enabled). With LIBASM_SUPERH_NOFPU it cannot be enabled
        // (fpuType() stays FPU_NONE) and those instructions remain undefined.
        assembler.setOption("fpu", "true");
        if (is_fpu_ready()) {
            TEST("LDS R0, FPUL",       0x405A);
            TEST("LDS R15, FPSCR",     0x4F6A);
            TEST("STS FPUL, R0",       0x005A);
            TEST("STS FPSCR, R15",     0x0F6A);
            TEST("LDS.L @R0+, FPUL",   0x4056);
            TEST("STS.L FPUL, @-R0",   0x4052);
            TEST("LDS.L @R0+, FPSCR",  0x4066);
            TEST("STS.L FPSCR, @-R0",  0x4062);
            TEST("FCNVDS DR0, FPUL",   0xF0BD);
            TEST("FCNVDS DR14, FPUL",  0xFEBD);
            TEST("FCNVSD FPUL, DR0",   0xF0AD);
            TEST("FCNVSD FPUL, DR14", 0xFEAD);
        } else {
            ERRT("FCNVDS DR0, FPUL",   UNKNOWN_INSTRUCTION, "FCNVDS DR0, FPUL");
            ERRT("FCNVSD FPUL, DR0",   UNKNOWN_INSTRUCTION, "FCNVSD FPUL, DR0");
        }
    } else {
        ERRT("CLIPS.B R0",         UNKNOWN_INSTRUCTION, "CLIPS.B R0");
        ERRT("MULR R0, R1",        UNKNOWN_INSTRUCTION, "MULR R0, R1");
        ERRT("MOVI20 #0, R0",      UNKNOWN_INSTRUCTION, "MOVI20 #0, R0");
        ERRT("MOVRT R0",           UNKNOWN_INSTRUCTION, "MOVRT R0");
        ERRT("JSR/N @R0",          UNKNOWN_INSTRUCTION, "JSR/N @R0");
        ERRT("RESBANK",            UNKNOWN_INSTRUCTION, "RESBANK");
    }
}

void test_data_constant() {
    // .data emits 16-bit big-endian words; multiple comma-separated values
    // and strings are both accepted.
    TEST(".data H'1234",                          0x1234);
    TEST(".data H'1234, H'5678",                  0x1234, 0x5678);
    TEST(".data H'1234, H'5678, H'9ABC",          0x1234, 0x5678, 0x9ABC);
    TEST(".data 0",                               0x0000);
    TEST(".data 65535",                           0xFFFF);
    TEST(".data -1",                              0xFFFF);
    TEST(".data H'7FFF, -H'8000",                 0x7FFF, 0x8000);
    // Hitachi character literal: "X" inside .data emits the ASCII code.
    TEST(R"(.data "AB")",                         0x4142);
    TEST(R"(.data "Hi!")",                        0x4869, 0x2100);  // odd-length string padded
    TEST(R"(.data H'1234, "OK")",                 0x1234, 0x4F4B);

    // Floating-point data constants. .fdata.s emits a 4-byte IEEE-754 single,
    // .fdata.d an 8-byte double, both big-endian (TEST args are 16-bit words).
    // The Hitachi float literal is F'[+-]n[.m][S|D][+-]xx; a plain C-style float
    // (no F' prefix) is accepted as a backup.
    // .fdata is a float-data feature gated only by LIBASM_ASM_NOFLOAT (not the
    // FPU instruction set), so it works the same with or without the FPU.
#if defined(LIBASM_ASM_NOFLOAT)
    ERRT(".fdata.s F'0.5S-2", FLOAT_NOT_SUPPORTED, "F'0.5S-2", 0x0000, 0x0000);
    ERRT(".fdata.d F'.123D3", FLOAT_NOT_SUPPORTED, "F'.123D3", 0x0000, 0x0000, 0x0000, 0x0000);
#else
    // Manual examples: F'0.5S-2 -> H'3BA3D70A, F'.123D3 -> H'405EC00000000000.
    TEST(".fdata.s F'0.5S-2",                     0x3BA3, 0xD70A);
    TEST(".fdata.d F'.123D3",                     0x405E, 0xC000, 0x0000, 0x0000);
    // Precision marker without an exponent, and a signed mantissa.
    TEST(".fdata.s F'1.0",                        0x3F80, 0x0000);
    TEST(".fdata.s F'1S0",                        0x3F80, 0x0000);
    TEST(".fdata.s F'-2.5",                       0xC020, 0x0000);
    TEST(".fdata.d F'1.5",                        0x3FF8, 0x0000, 0x0000, 0x0000);
    // C-style float backup (no F' prefix) still works.
    TEST(".fdata.s 0.5",                          0x3F00, 0x0000);
    TEST(".fdata.s 0.5, F'1.0",                   0x3F00, 0x0000, 0x3F80, 0x0000);
#endif
}

// Documentation note: the libasm SH-DSP table does NOT yet implement the
// DSP compute / parallel-data-transfer instructions: PADD, PSUB, PMULS,
// PMAC, PCMP, PABS, PNEG, PINC, PDEC, PCOPY, PSHA, PSHL, PRND, DCT/DCF
// (DSP conditionals), MOVX.W, MOVY.W, MOVS.W, MOVS.L.  The assembler's
// permissive parser doesn't cleanly report these as UNKNOWN_INSTRUCTION
// (it errors on register-operand parsing instead), so we don't have an
// ERRT-based pin here.  Adding these is a SH-DSP scope-extension task.

// Sanity-extend the SH-2A bank / return instructions (Rn variations) and
// pin the asm-side encoding for JSR/N / LDBANK / STBANK.  Note: the SH-2A
// disassembler does NOT currently recognise the bytes asm produces for
// JSR/N (0x404B) and the LDBANK / STBANK families (0x40E5 / 0x40E1) --
// they decode as "Unknown instruction".  Tracking that as a known dis gap.
void test_sh2a_bank_extras() {
    if (!isSh2a())
        return;
    // JSR/N @Rn  (0100 nnnn 0100 1011) -- register field at bits[11:8].
    TEST("JSR/N @R0",   0x404B);
    TEST("JSR/N @R7",   0x474B);
    TEST("JSR/N @R15",  0x4F4B);
    // LDBANK @Rn, R0  (0100 nnnn 1110 0101) -- register field at bits[11:8].
    TEST("LDBANK @R0, R0",   0x40E5);
    TEST("LDBANK @R7, R0",   0x47E5);
    TEST("LDBANK @R15, R0",  0x4FE5);
    // STBANK R0, @Rn  (0100 nnnn 1110 0001) -- register field at bits[11:8].
    TEST("STBANK R0, @R0",   0x40E1);
    TEST("STBANK R0, @R7",   0x47E1);
    TEST("STBANK R0, @R15",  0x4FE1);
    // MOVMU.L / MOVML.L (push/pop multi)
    TEST("MOVMU.L R0, @-R15",   0x40F0);
    TEST("MOVMU.L R7, @-R15",   0x47F0);
    TEST("MOVML.L R0, @-R15",   0x40F1);
    TEST("MOVML.L R7, @-R15",   0x47F1);
    TEST("MOVMU.L @R15+, R0",   0x40F4);
    TEST("MOVML.L @R15+, R0",   0x40F5);
}

// An undefined symbol reads as value 0 and must surface UNDEFINED_SYMBOL --
// never silently assemble, and never report a misleading OVERFLOW/TOO_FAR
// caused by the placeholder 0. The PC-relative modes emit a zero
// displacement (the base opcode) rather than a delta from address 0.
void test_undef() {
    // M_IMM8 -- signed immediate (MOV/ADD/CMP) and unsigned (AND/OR/XOR/TST).
    ERUS("MOV    #UNDEF, R0",         "UNDEF, R0",          0xE000);
    ERUS("ADD    #UNDEF, R1",         "UNDEF, R1",          0x7100);
    ERUS("CMP/EQ #UNDEF, R0",         "UNDEF, R0",          0x8800);
    ERUS("AND    #UNDEF, R0",         "UNDEF, R0",          0xC900);
    ERUS("OR     #UNDEF, R0",         "UNDEF, R0",          0xCB00);
    ERUS("XOR    #UNDEF, R0",         "UNDEF, R0",          0xCA00);
    ERUS("TST    #UNDEF, R0",         "UNDEF, R0",          0xC800);
    ERUS("AND.B  #UNDEF, @(R0, GBR)", "UNDEF, @(R0, GBR)",  0xCD00);
    ERUS("OR.B   #UNDEF, @(R0, GBR)", "UNDEF, @(R0, GBR)",  0xCF00);
    ERUS("XOR.B  #UNDEF, @(R0, GBR)", "UNDEF, @(R0, GBR)",  0xCE00);
    ERUS("TST.B  #UNDEF, @(R0, GBR)", "UNDEF, @(R0, GBR)",  0xCC00);
    // M_TNUM -- TRAPA #imm (unsigned 8-bit).
    ERUS("TRAPA  #UNDEF",             "UNDEF",              0xC300);
    // M_D4N -- MOV.L Rm,@(disp,Rn), disp scaled by 4.
    ERUS("MOV.L  R0, @(UNDEF, R1)",   "UNDEF, R1)",         0x1100);
    // M_D4M -- @(disp,Rm), disp scaled by mnemonic size.
    ERUS("MOV.B  R0, @(UNDEF, R1)",   "UNDEF, R1)",         0x8010);
    ERUS("MOV.W  R0, @(UNDEF, R1)",   "UNDEF, R1)",         0x8110);
    ERUS("MOV.B  @(UNDEF, R1), R0",   "UNDEF, R1), R0",     0x8410);
    ERUS("MOV.W  @(UNDEF, R1), R0",   "UNDEF, R1), R0",     0x8510);
    ERUS("MOV.L  @(UNDEF, R1), R2",   "UNDEF, R1), R2",     0x5210);
    // M_D8B/W/L -- @(disp,GBR), disp scaled by size.
    ERUS("MOV.B  R0, @(UNDEF, GBR)",  "UNDEF, GBR)",        0xC000);
    ERUS("MOV.W  R0, @(UNDEF, GBR)",  "UNDEF, GBR)",        0xC100);
    ERUS("MOV.L  R0, @(UNDEF, GBR)",  "UNDEF, GBR)",        0xC200);
    ERUS("MOV.B  @(UNDEF, GBR), R0",  "UNDEF, GBR), R0",    0xC400);
    ERUS("MOV.W  @(UNDEF, GBR), R0",  "UNDEF, GBR), R0",    0xC500);
    ERUS("MOV.L  @(UNDEF, GBR), R0",  "UNDEF, GBR), R0",    0xC600);
    // M_PCW / M_PCL -- @(target,PC); emit zero displacement (base opcode).
    ERUS("MOV.W  @(UNDEF, PC), R0",   "UNDEF, PC), R0",     0x9000);
    ERUS("MOV.L  @(UNDEF, PC), R0",   "UNDEF, PC), R0",     0xD000);
    ERUS("MOVA   @(UNDEF, PC), R0",   "UNDEF, PC), R0",     0xC700);
    // M_REL8 / M_REL12 -- PC-relative branch targets; emit zero delta.
    ERUS("BT     UNDEF",              "UNDEF",              0x8900);
    ERUS("BF     UNDEF",              "UNDEF",              0x8B00);
    ERUS("BRA    UNDEF",              "UNDEF",              0xA000);
    ERUS("BSR    UNDEF",              "UNDEF",              0xB000);
    // Same, but pinned at a non-zero address to prove the delta is still 0.
    AERRT(0x0100, "BRA UNDEF", UNDEFINED_SYMBOL, "UNDEF", 0xA000);
    AERRT(0x0100, "BF  UNDEF", UNDEFINED_SYMBOL, "UNDEF", 0x8B00);

    if (isSh2()) {
        // M_REL8 delay-slot branches (SH-2).
        ERUS("BT/S UNDEF",            "UNDEF",              0x8D00);
        ERUS("BF/S UNDEF",            "UNDEF",              0x8F00);
    }
    if (isShDsp()) {
        // M_REL8P -- LDRS/LDRE bare symbol target; M_TNUM -- SETRC #imm (SH-DSP).
        ERUS("LDRS  UNDEF",           "UNDEF",              0x8C00);
        ERUS("LDRE  UNDEF",           "UNDEF",              0x8E00);
        ERUS("SETRC #UNDEF",          "UNDEF",              0x8200);
    }
    if (isSh2a()) {
        // M_IMM3 -- BCLR/BSET/BST #imm3,Rn (SH-2A).
        ERUS("BCLR #UNDEF, R0",       "UNDEF, R0",          0x8600);
        ERUS("BSET #UNDEF, R0",       "UNDEF, R0",          0x8601);
        ERUS("BST  #UNDEF, R0",       "UNDEF, R0",          0x8700);
        // M_IMM20 / M_IMM20S -- 32-bit MOVI20/MOVI20S (two words).
        ERUS("MOVI20  #UNDEF, R0",    "UNDEF, R0", uint16_t(0x0000), uint16_t(0x0000));
        ERUS("MOVI20S #UNDEF, R0",    "UNDEF, R0", uint16_t(0x0001), uint16_t(0x0000));
    }
}
// clang-format on

void run_tests(const char *cpu) {
    assembler.setCpu(cpu);
    RUN_TEST(test_undef);
    RUN_TEST(test_mov_imm);
    RUN_TEST(test_mov_reg);
    RUN_TEST(test_mov_indirect);
    RUN_TEST(test_mov_inc_dec);
    RUN_TEST(test_mov_indexed);
    RUN_TEST(test_mov_disp_reg);
    RUN_TEST(test_mov_disp_gbr);
    RUN_TEST(test_mov_pcrel);
    RUN_TEST(test_movt);
    RUN_TEST(test_add);
    RUN_TEST(test_addc_addv_sub);
    RUN_TEST(test_cmp);
    RUN_TEST(test_div_ext);
    RUN_TEST(test_mul);
    RUN_TEST(test_neg_swap_xtrct);
    RUN_TEST(test_logic);
    RUN_TEST(test_shift_rotate);
    RUN_TEST(test_branch);
    RUN_TEST(test_jmp_jsr_rts);
    RUN_TEST(test_system_control);
    RUN_TEST(test_ldc_stc_lds_sts);
    RUN_TEST(test_sh2_only);
    RUN_TEST(test_shdsp_only);
    RUN_TEST(test_sh2e_only);
    RUN_TEST(test_sh2a_only);
    RUN_TEST(test_sh2a_bank_extras);
    RUN_TEST(test_data_constant);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

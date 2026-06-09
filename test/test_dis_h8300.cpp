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

#include "dis_h8300.h"
#include "test_dis_helper.h"
#include "test_symtab.h"

using namespace libasm;
using namespace libasm::h8300;
using namespace libasm::test;

DisH8300 dis8300;
Disassembler &disassembler(dis8300);

// True for any CPU in the H8S family (H8S/2000 and H8S/2600).
bool is_h8s() {
    const auto *cpu_P = disassembler.config().cpu_P();
    return strcmp_P("H8S/2000", cpu_P) == 0 || strcmp_P("H8S/2600", cpu_P) == 0;
}

bool is_h8s2600() {
    return strcmp_P("H8S/2600", disassembler.config().cpu_P()) == 0;
}

bool is_h8300h() {
    return strcmp_P("H8/300H", disassembler.config().cpu_P()) == 0;
}

// Pick the H8/300 (r16) or H8/300H (r32) form of an address-register-bearing
// operand string. Pointers like @R0 disasm to @ER0 on H8/300H because the
// address register is 32-bit there.
#define AREG(r16, r32) ((is_h8300h() || is_h8s()) ? (r32) : (r16))

void set_up() {
    disassembler.reset();
}

void tear_down() {
    symtab.reset();
}

// clang-format off

void test_cpu() {
    EQUALS("cpu H8/300", true,    disassembler.setCpu("H8/300"));
    EQUALS_P("cpu H8/300", "H8/300", disassembler.config().cpu_P());

    EQUALS("cpu H8300", false, disassembler.setCpu("H8300"));
}

void test_system() {
    TEST("NOP",    "", 0x0000);
    TEST("SLEEP",  "", 0x0180);
    TEST("RTS",    "", 0x5470);
    TEST("RTE",    "", 0x5670);
    TEST("EEPMOV", "", 0x7B5C, 0x598F);

    UNKN(0x0000|0x01);        // NOP: byte2 must be 0x00
    if (!is_h8300h() && !is_h8s())
        UNKN(0x0180 ^ 0x80);  // SLEEP: byte2 must be 0x80 (H8/300H uses 0x0100 as super-prefix)
    UNKN(0x7B5C, 0x598E);     // EEPMOV: wrong trailing bytes

    if (is_h8300h() || is_h8s()) {
        // TRAPA #vec
        TEST("TRAPA", "#0", 0x5700|(0<<4));
        TEST("TRAPA", "#1", 0x5700|(1<<4));
        TEST("TRAPA", "#2", 0x5700|(2<<4));
        TEST("TRAPA", "#3", 0x5700|(3<<4));

        // EEPMOV.W (0x7BD4 prefix)
        TEST("EEPMOV.W", "", 0x7BD4, 0x598F);
    }
}

void test_ccr() {
    // STC CCR,Rd: byte2=Rd nibble (high nibble must be 0)
    TEST("STC", "CCR, R0H", 0x0200|0x0);
    TEST("STC", "CCR, R4L", 0x0200|0xC);
    if (!is_h8s())  // 0x0210 is STC EXR,Rd on H8S
        UNKN(0x0200|0x10);
    UNKN(0x0200|0x80);

    // LDC Rs,CCR: byte2=Rs nibble
    TEST("LDC",  "R0H, CCR", 0x0300|0x0);
    TEST("LDC",  "R2L, CCR", 0x0300|0xA);
    if (!is_h8s())  // 0x0310 is LDC Rs,EXR on H8S
        UNKN(0x0300|0x10);
    UNKN(0x0300|0x80);

    // ORC/XORC/ANDC/LDC #imm,CCR: byte2=imm8
    TEST("ORC",  "#H'FF, CCR", 0x0400|0xFF);
    TEST("XORC", "#15, CCR",   0x0500|0x0F);
    TEST("ANDC", "#H'F0, CCR", 0x0600|0xF0);
    TEST("LDC",  "#0, CCR",    0x0700|0x00);

    if (is_h8300h() || is_h8s()) {
        // LDC / STC ccr with memory operand via 0x0140 super-prefix
        TEST("LDC", "@ER0, CCR",       0x0140, 0x6900|(0<<4));
        TEST("LDC", "@SP, CCR",        0x0140, 0x6900|(7<<4));   // ER7 -> SP
        TEST("STC", "CCR, @ER0",       0x0140, 0x6980|(0<<4));
        TEST("STC", "CCR, @SP",        0x0140, 0x6980|(7<<4));
        TEST("LDC", "@ER0+, CCR",      0x0140, 0x6D00|(0<<4));
        TEST("STC", "CCR, @-ER1",      0x0140, 0x6D80|(1<<4));
        TEST("LDC", "@H'1234, CCR", 0x0140, 0x6B00, 0x1234);   // low addr, no :16
        TEST("STC", "CCR, @H'1234", 0x0140, 0x6B80, 0x1234);
        TEST("LDC", "@(H'1234,ER0), CCR", 0x0140, 0x6F00|(0<<4), 0x1234);
        TEST("STC", "CCR, @(H'1234,ER0)", 0x0140, 0x6F80|(0<<4), 0x1234);

        if (!is_h8s()) {
            // LDC / STC ccr,@(d:24,ERn) via SPRX_0140 + 0x7800 prefix.
            // H8S decodes this as @(d:32, ERn) instead.
            TEST("LDC", "@(H'001234:24,ER0), CCR", 0x0140, 0x7800|(0<<4), 0x6B20, 0x0000, 0x1234);
            TEST("STC", "CCR, @(H'FFFFE0:24,SP)",  0x0140, 0x7800|(7<<4), 0x6BA0, 0x00FF, 0xFFE0);
        }
    }
}

void test_data_move() {
    // MOV.B Rs,Rd: byte2=(Rs<<4)|Rd
    TEST("MOV.B", "R0H, R0H", 0x0C00|(0x0<<4)|0x0);
    TEST("MOV.B", "R1H, R3L", 0x0C00|(0x1<<4)|0xB);
    TEST("MOV.B", "R7L, R7L", 0x0C00|(0xF<<4)|0xF);

    // MOV.W Rs,Rd: byte2=(Rs3<<4)|Rd3
    TEST("MOV.W", "R0, R0", 0x0D00|(0<<4)|0);
    TEST("MOV.W", "R2, R5", 0x0D00|(2<<4)|5);

    // MOV.B @aa:8,Rd (0x20-0x2F): byte1=(0x20|Rd nibble), byte2=aa
    // Rd nibble: 0..7=R0H..R7H, 8..F=R0L..R7L (full 16 byte registers)
    TEST("MOV.B", "@H'FF00:8, R0H", 0x2000|(0<<8)|0x00);
    TEST("MOV.B", "@H'FF12:8, R3H", 0x2000|(3<<8)|0x12);
    TEST("MOV.B", "@H'FF34:8, R0L", 0x2000|(8<<8)|0x34);
    TEST("MOV.B", "@H'FFFF:8, R7L", 0x2000|(0xF<<8)|0xFF);

    // MOV.B Rs,@aa:8 (0x30-0x3F): byte1=(0x30|Rs nibble), byte2=aa
    TEST("MOV.B", "R0H, @H'FF00:8", 0x3000|(0<<8)|0x00);
    TEST("MOV.B", "R5H, @H'FFAB:8", 0x3000|(5<<8)|0xAB);
    TEST("MOV.B", "R0L, @H'FF34:8", 0x3000|(8<<8)|0x34);
    TEST("MOV.B", "R7L, @H'FFFF:8", 0x3000|(0xF<<8)|0xFF);

    // MOV.B @Rn,Rd (0x68, bit7=0): byte2=(Rn3<<4)|Rd
    TEST("MOV.B", AREG("@R0, R0H", "@ER0, R0H"), 0x6800|(0<<4)|0x0);
    TEST("MOV.B", AREG("@R2, R5L", "@ER2, R5L"), 0x6800|(2<<4)|0xD);

    // MOV.B Rs,@Rn (0x68, bit7=1): byte2=0x80|(Rn3<<4)|Rs
    TEST("MOV.B", AREG("R0H, @R0", "R0H, @ER0"), 0x6880|(0<<4)|0x0);
    TEST("MOV.B", AREG("R3L, @R6", "R3L, @ER6"), 0x6880|(6<<4)|0xB);

    // MOV.W @Rn,Rd (0x69, bit7=0): byte2=(Rn3<<4)|Rd3
    TEST("MOV.W", AREG("@R0, R0", "@ER0, R0"), 0x6900|(0<<4)|0);
    TEST("MOV.W", "@SP, R5",                   0x6900|(7<<4)|5);

    // MOV.W Rs,@Rn (0x69, bit7=1): byte2=0x80|(Rn3<<4)|Rs3
    TEST("MOV.W", AREG("R0, @R0", "R0, @ER0"), 0x6980|(0<<4)|0);
    TEST("MOV.W", "R7, @SP",                   0x6980|(7<<4)|7);

    // MOV.B @abs16,Rd (0x6A, bits[7:6]=00): byte2=Rd nibble
    TEST("MOV.B", "@H'1234, R0H",    0x6A00|0x0, 0x1234);
    TEST("MOV.B", "@H'FFFF:16, R7L", 0x6A00|0xF, 0xFFFF);

    // MOV.B Rs,@abs16 (0x6A, bits[7:6]=10): byte2=0x80|Rs nibble
    TEST("MOV.B", "R0H, @H'1234", 0x6A80|0x0, 0x1234);
    TEST("MOV.B", "R3L, @H'0100", 0x6A80|0xB, 0x0100);

    // MOV.W @abs16,Rd (0x6B, bit7=0): byte2=Rd3
    TEST("MOV.W", "@H'1234, R0",    0x6B00|0, 0x1234);
    TEST("MOV.W", "@H'FFFE:16, R7", 0x6B00|7, 0xFFFE);

    // MOV.W Rs,@abs16 (0x6B, bit7=1): byte2=0x80|Rs3
    TEST("MOV.W", "R0, @H'1234", 0x6B80|0, 0x1234);
    TEST("MOV.W", "R5, @H'0200", 0x6B80|5, 0x0200);

    // MOV.B @Rn+,Rd (0x6C, bit7=0): byte2=(Rn3<<4)|Rd
    TEST("MOV.B", AREG("@R0+, R0H", "@ER0+, R0H"), 0x6C00|(0<<4)|0x0);
    TEST("MOV.B", AREG("@R3+, R5L", "@ER3+, R5L"), 0x6C00|(3<<4)|0xD);
    ERRT("MOV.B", "@SP+, R5L", ILLEGAL_SIZE, "@SP+, R5L", 0x6C00|(7<<4)|0xD);

    // MOV.B Rs,@-Rn (0x6C, bit7=1): byte2=0x80|(Rn3<<4)|Rs
    TEST("MOV.B", AREG("R0H, @-R0", "R0H, @-ER0"), 0x6C80|(0<<4)|0x0);
    TEST("MOV.B", AREG("R4L, @-R2", "R4L, @-ER2"), 0x6C80|(2<<4)|0xC);
    ERRT("MOV.B", "R4L, @-SP", ILLEGAL_SIZE, "@-SP", 0x6C80|(7<<4)|0xC);

    // MOV.W @Rn+,Rd (0x6D, bit7=0, Rn!=SP): byte2=(Rn3<<4)|Rd3
    TEST("MOV.W", AREG("@R0+, R0", "@ER0+, R0"), 0x6D00|(0<<4)|0);
    TEST("MOV.W", AREG("@R2+, R4", "@ER2+, R4"), 0x6D00|(2<<4)|4);

    // POP Rd (0x6D, Rn=SP=R7, bit7=0): byte2=0x70|Rd3
    TEST("POP",  "R0", 0x6D70|0);
    TEST("POP",  "R5", 0x6D70|5);

    // MOV.W Rs,@-Rn (0x6D, bit7=1, Rn!=SP): byte2=0x80|(Rn3<<4)|Rs3
    TEST("MOV.W", AREG("R0, @-R0", "R0, @-ER0"), 0x6D80|(0<<4)|0);
    TEST("MOV.W", AREG("R5, @-R5", "R5, @-ER5"), 0x6D80|(5<<4)|5);

    // PUSH Rs (0x6D, Rn=SP=R7, bit7=1): byte2=0xF0|Rs3
    TEST("PUSH", "R0", 0x6DF0|0);
    TEST("PUSH", "R6", 0x6DF0|6);

    // MOV.B @(d16,Rn),Rd (0x6E, bit7=0): byte2=(Rn3<<4)|Rd, then disp16
    TEST("MOV.B", AREG("@(H'0000,R0), R0H", "@(H'0000,ER0), R0H"), 0x6E00|(0<<4)|0x0, 0x0000);
    TEST("MOV.B", AREG("@(H'0010,R2), R3L", "@(H'0010,ER2), R3L"), 0x6E00|(2<<4)|0xB, 0x0010);
    TEST("MOV.B", "@(-1,SP), R7H",                                 0x6E00|(7<<4)|0x7, 0xFFFF);

    // MOV.B Rs,@(d16,Rn) (0x6E, bit7=1): byte2=0x80|(Rn3<<4)|Rs
    TEST("MOV.B", AREG("R0H, @(H'0000,R0)", "R0H, @(H'0000,ER0)"), 0x6E80|(0<<4)|0x0, 0x0000);
    TEST("MOV.B", AREG("R4L, @(H'0001,R3)", "R4L, @(H'0001,ER3)"), 0x6E80|(3<<4)|0xC, 0x0001);

    // MOV.W @(d16,Rn),Rd (0x6F, bit7=0): byte2=(Rn3<<4)|Rd3
    TEST("MOV.W", AREG("@(H'0000,R0), R0", "@(H'0000,ER0), R0"),   0x6F00|(0<<4)|0, 0x0000);
    TEST("MOV.W", "@(H'0020,SP), R2",                              0x6F00|(7<<4)|2, 0x0020);

    // MOV.W Rs,@(d16,Rn) (0x6F, bit7=1): byte2=0x80|(Rn3<<4)|Rs3
    TEST("MOV.W", AREG("R0, @(H'0000,R0)", "R0, @(H'0000,ER0)"),   0x6F80|(0<<4)|0, 0x0000);
    TEST("MOV.W", AREG("R5, @(-2,R6)", "R5, @(-2,ER6)"),           0x6F80|(6<<4)|5, 0xFFFE);

    // MOV.W #imm,Rd (0x79): byte2=Rd3, then imm16
    TEST("MOV.W", "#0, R0",     0x7900|0, 0x0000);
    TEST("MOV.W", "#H'1234, R3", 0x7900|3, 0x1234);
    TEST("MOV.W", "#H'FFFF, R7", 0x7900|7, 0xFFFF);

    // MOV.B #imm,Rd (0xF0-0xFF): byte1=0xF0|(Rd nibble), byte2=imm8
    TEST("MOV.B", "#0, R0H",   0xF000|(0x0<<8)|0x00);
    TEST("MOV.B", "#H'FF, R7L", 0xF000|(0xF<<8)|0xFF);

    // MOVFPE @abs16,Rd (0x6A, bits[7:6]=01): byte2=0x40|Rd nibble (full 16 byte registers)
    TEST("MOVFPE", "@H'1234, R0H", 0x6A40|0x0, 0x1234);
    TEST("MOVFPE", "@H'5678, R7L", 0x6A40|0xF, 0x5678);
    // MOVTPE Rs,@abs16 (0x6A, bits[7:6]=11): byte2=0xC0|Rs nibble (full 16 byte registers)
    TEST("MOVTPE", "R0H, @H'1234", 0x6AC0|0x0, 0x1234);
    TEST("MOVTPE", "R5L, @H'5678", 0x6AC0|0xD, 0x5678);

    if (is_h8300h() || is_h8s()) {
        // MOV.L ERs,ERd: 0x0F80 | (ERs<<4) | ERd. Both fields are 3-bit
        // (POS__7_ / POS___7); the destination bit-3 reserved bit is checked.
        TEST("MOV.L", "ER0, ER1",                0x0F80|(0<<4)|1);
        TEST("MOV.L", "ER2, ER5",                0x0F80|(2<<4)|5);
        TEST("MOV.L", "ER7, ER0",                0x0F80|(7<<4)|0);
        UNKN(                                    0x0F80|(0<<4)|0x8);  // bit 3 of ERd reserved

        // MOV.L #imm32, ERd via 0x7A00 prefix
        TEST("MOV.L", "#H'12345678, ER0",        0x7A00|0, 0x1234, 0x5678);
        TEST("MOV.L", "#H'FFFFFFFF, ER7",        0x7A00|7, 0xFFFF, 0xFFFF);

        // MOV.B / MOV.W with ER pointer (decoded via decodeOprAddrReg).
        TEST("MOV.B", "@ER0, R0H",               0x6800|(0<<4)|0x0);
        TEST("MOV.B", "R0H, @SP",                0x6880|(7<<4)|0x0);  // ER7 prints as SP
        TEST("MOV.W", "@ER3, R5",                0x6900|(3<<4)|0x5);
        TEST("MOV.W", "R0, @-ER1",               0x6D80|(1<<4)|0x0);
        TEST("MOV.B", "@(H'1234,ER0), R0H",      0x6E00|(0<<4)|0x0, 0x1234);
        TEST("MOV.W", "@(-2,SP), R0",            0x6F00|(7<<4)|0x0, 0xFFFE);

        if (!is_h8s()) {
            // MOV.B / MOV.W @(d:24,ERn) via 0x7800 normal prefix.
            // H8S decodes this as @(d:32, ERn).
            TEST("MOV.B", "@(H'000000:24,ER0), R0H", 0x7800|(0<<4), 0x6A20|0x0, 0x0000, 0x0000);
            TEST("MOV.B", "@(H'FFFFFF:24,SP), R7L",  0x7800|(7<<4), 0x6A20|0xF, 0x00FF, 0xFFFF);
            TEST("MOV.B", "R0H, @(H'000000:24,ER0)", 0x7800|(0<<4), 0x6AA0|0x0, 0x0000, 0x0000);
            TEST("MOV.W", "@(H'001234:24,ER3), R5",  0x7800|(3<<4), 0x6B20|0x5, 0x0000, 0x1234);
        }

        // MOV.B / MOV.W @aa:24 and MOV.L @aa:24 (MOV.L uses 0x0100 super-prefix)
        TEST("MOV.B", "@H'000000:24, R0H",       0x6A20|0x0,         0x0000, 0x0000);
        TEST("MOV.B", "@H'FFFFFF:24, R7L",       0x6A20|0xF,         0x00FF, 0xFFFF);
        TEST("MOV.B", "R0H, @H'000000:24",       0x6AA0|0x0,         0x0000, 0x0000);
        TEST("MOV.W", "@H'000100:24, R0",        0x6B20|0x0,         0x0000, 0x0100);
        TEST("MOV.W", "R7, @H'FFFFFE:24",        0x6BA0|0x7,         0x00FF, 0xFFFE);
        TEST("MOV.L", "@H'000100:24, ER0",       0x0100, 0x6B20|0,   0x0000, 0x0100);
        TEST("MOV.L", "ER7, @H'FFFFFE:24",       0x0100, 0x6BA0|7,   0x00FF, 0xFFFE);

        // MOV.L with addressing modes via 0x0100 super-prefix
        TEST("MOV.L", "@ER0, ER1",               0x0100, 0x6900|(0<<4)|1);
        TEST("MOV.L", "ER1, @ER0",               0x0100, 0x6980|(0<<4)|1);
        TEST("MOV.L", "@ER0+, ER1",              0x0100, 0x6D00|(0<<4)|1);
        TEST("MOV.L", "ER0, @-ER1",              0x0100, 0x6D80|(1<<4)|0);
        TEST("MOV.L", "@H'1234, ER0",            0x0100, 0x6B00|0,        0x1234);
        TEST("MOV.L", "ER0, @H'1234",            0x0100, 0x6B80|0,        0x1234);
        TEST("MOV.L", "@(H'1234,ER0), ER1",      0x0100, 0x6F00|(0<<4)|1, 0x1234);
        TEST("MOV.L", "ER0, @(H'1234,ER1)",      0x0100, 0x6F80|(1<<4)|0, 0x1234);
        // ER7 prints as SP for pointer-style operands (non-PINC/PDEC forms)
        TEST("MOV.L", "@SP, ER0",                0x0100, 0x6900|(7<<4)|0);

        // POP.L / PUSH.L decode (preferred over MOV.L @SP+/-SP for ER7)
        TEST("POP.L",  "ER0",                    0x0100, 0x6D70|0);
        TEST("POP.L",  "ER5",                    0x0100, 0x6D70|5);
        TEST("PUSH.L", "ER0",                    0x0100, 0x6DF0|0);
        TEST("PUSH.L", "ER6",                    0x0100, 0x6DF0|6);
    }
}

void test_arithmetic() {
    // ADD/SUB/CMP/ADDX/SUBX reg-reg: byte2=(Rs<<4)|Rd
    TEST("ADD.B", "R0H, R0H", 0x0800|(0x0<<4)|0x0);
    TEST("ADD.B", "R1H, R3L", 0x0800|(0x1<<4)|0xB);
    TEST("ADD.W", "R0, R0",   0x0900|(0<<4)|0);
    TEST("ADD.W", "R3, R5",   0x0900|(3<<4)|5);
    TEST("ADDX",  "R0H, R1H", 0x0E00|(0x0<<4)|0x1);

    TEST("SUB.B", "R0H, R0H", 0x1800|(0x0<<4)|0x0);
    TEST("SUB.B", "R5L, R2H", 0x1800|(0xD<<4)|0x2);
    TEST("SUB.W", "R0, R0",   0x1900|(0<<4)|0);
    TEST("SUB.W", "R4, R7",   0x1900|(4<<4)|7);
    TEST("SUBX",  "R0H, R7L", 0x1E00|(0x0<<4)|0xF);

    TEST("CMP.B", "R0H, R0H", 0x1C00|(0x0<<4)|0x0);
    TEST("CMP.B", "R2H, R6L", 0x1C00|(0x2<<4)|0xE);
    TEST("CMP.W", "R0, R0",   0x1D00|(0<<4)|0);
    TEST("CMP.W", "R1, R6",   0x1D00|(1<<4)|6);

    // INC/DEC/DAA/DAS Rd: byte2=Rd nibble (high nibble must be 0)
    TEST("INC", "R0H", 0x0A00|0x0);
    TEST("INC", "R7L", 0x0A00|0xF);
    UNKN(0x0A00|0x10);        // INC: high nibble of byte2 must be 0

    TEST("DEC", "R0H", 0x1A00|0x0);
    TEST("DEC", "R4H", 0x1A00|0x4);
    if (!is_h8300h() && !is_h8s())
        UNKN(0x1A00|0xF0);    // DEC: high nibble of byte2 must be 0 (H8/300H reuses bit 7 for SUB.L)

    // ADDS/SUBS #1/#2,Rd: byte2=Rd3 (bit7=0→#1, bit7=1→#2)
    TEST("ADDS", AREG("#1, R0", "#1, ER0"), 0x0B00|0);
    TEST("ADDS", AREG("#1, R5", "#1, ER5"), 0x0B00|5);
    TEST("ADDS", AREG("#2, R0", "#2, ER0"), 0x0B80|0);
    TEST("ADDS", AREG("#2, R7", "#2, ER7"), 0x0B80|7);

    TEST("SUBS", AREG("#1, R0", "#1, ER0"), 0x1B00|0);
    TEST("SUBS", AREG("#1, R3", "#1, ER3"), 0x1B00|3);
    TEST("SUBS", AREG("#2, R0", "#2, ER0"), 0x1B80|0);
    TEST("SUBS", AREG("#2, R7", "#2, ER7"), 0x1B80|7);

    TEST("DAA", "R0H", 0x0F00|0x0);
    TEST("DAA", "R7L", 0x0F00|0xF);
    UNKN(0x0F00|0x10);        // DAA: high nibble of byte2 must be 0

    TEST("DAS", "R0H", 0x1F00|0x0);
    TEST("DAS", "R7L", 0x1F00|0xF);
    if (!is_h8300h() && !is_h8s())
        UNKN(0x1F00|0x80);    // DAS: high nibble of byte2 must be 0 (H8/300H reuses bit 7 for CMP.L)

    // MULXU/DIVXU Rs8,Rd16: byte2=(Rs nibble<<4)|Rd3
    TEST("MULXU", "R0H, R0", 0x5000|(0x0<<4)|0);
    TEST("MULXU", "R3H, R5", 0x5000|(0x3<<4)|5);
    TEST("DIVXU", "R0H, R0", 0x5100|(0x0<<4)|0);
    TEST("DIVXU", "R7L, R2", 0x5100|(0xF<<4)|2);

    // NEG Rd (0x17, bit7=1): byte2=0x80|Rd nibble
    TEST("NEG", "R0H", 0x1780|0x0);
    TEST("NEG", "R7L", 0x1780|0xF);
    if (!is_h8300h() && !is_h8s())
        UNKN(0x1780|0x10);    // NEG: bits[6:4] of byte2 must be 0 (H8/300H reuses for NEG.W)

    // ADD/ADDX/CMP/SUBX #imm,Rd (0x80-0xBF): byte1=0x?0|(Rd nibble), byte2=imm8
    TEST("ADD.B", "#0, R0H",    0x8000|(0x0<<8)|0x00);
    TEST("ADD.B", "#H'FF, R7L", 0x8000|(0xF<<8)|0xFF);
    TEST("ADDX",  "#1, R0H",    0x9000|(0x0<<8)|0x01);
    TEST("CMP.B", "#H'55, R5H", 0xA000|(0x5<<8)|0x55);
    TEST("SUBX",  "#2, R2L",    0xB000|(0xA<<8)|0x02);

    if (is_h8300h() || is_h8s()) {
        // ADD.L / SUB.L / CMP.L ERs,ERd (same shape as MOV.L)
        TEST("ADD.L",   "ER0, ER1",         0x0A80|(0<<4)|1);
        TEST("ADD.L",   "ER7, ER0",         0x0A80|(7<<4)|0);
        TEST("SUB.L",   "ER0, ER1",         0x1A80|(0<<4)|1);
        TEST("CMP.L",   "ER3, ER5",         0x1F80|(3<<4)|5);

        // NEG.W / NEG.L Rd
        TEST("NEG.W",   "R3",               0x1790|0x3);
        TEST("NEG.L",   "ER2",              0x17B0|2);

        // INC.W / INC.L / DEC.W / DEC.L
        TEST("INC.W",   "#1, R0",           0x0B50|0x0);
        TEST("INC.W",   "#2, R7",           0x0BD0|0x7);
        TEST("INC.W",   "#1, E5",           0x0B50|0xD);
        TEST("INC.L",   "#1, ER0",          0x0B70|0);
        TEST("INC.L",   "#2, ER7",          0x0BF0|7);
        TEST("DEC.W",   "#1, R0",           0x1B50|0x0);
        TEST("DEC.W",   "#2, R7",           0x1BD0|0x7);
        TEST("DEC.L",   "#1, ER3",          0x1B70|3);
        TEST("DEC.L",   "#2, ER5",          0x1BF0|5);

        // ADDS / SUBS via M_ADREG: ER target on H8/300H (suffix is implicit).
        TEST("ADDS",    "#1, ER0",          0x0B00|0);
        TEST("ADDS",    "#1, ER7",          0x0B00|7);
        TEST("ADDS",    "#2, ER3",          0x0B80|3);
        TEST("SUBS",    "#1, ER4",          0x1B00|4);
        TEST("SUBS",    "#2, ER5",          0x1B80|5);
        // ADDS / SUBS #4 (H8/300H-only encoding 0x0B90 / 0x1B90)
        TEST("ADDS",    "#4, ER0",          0x0B90|0);
        TEST("ADDS",    "#4, ER7",          0x0B90|7);
        TEST("SUBS",    "#4, ER3",          0x1B90|3);

        // ADD.W / SUB.W / CMP.W #imm16, Rd
        TEST("ADD.W",   "#H'1234, R0",      0x7910|0x0, 0x1234);
        TEST("ADD.W",   "#H'FFFF, E7",      0x7910|0xF, 0xFFFF);
        TEST("CMP.W",   "#H'1234, R0",      0x7920|0x0, 0x1234);
        TEST("SUB.W",   "#H'1234, R0",      0x7930|0x0, 0x1234);

        // ADD.L / SUB.L / CMP.L #imm32, ERd
        TEST("ADD.L",   "#H'12345678, ER0", 0x7A10|0, 0x1234, 0x5678);
        TEST("CMP.L",   "#1, ER1",          0x7A20|1, 0x0000, 0x0001);
        TEST("SUB.L",   "#H'00010000, ER2", 0x7A30|2, 0x0001, 0x0000);

        // MULXU.W / DIVXU.W Rs,ERd
        TEST("MULXU.W", "R0, ER0",          0x5200|(0x0<<4)|0);
        TEST("MULXU.W", "R3, ER5",          0x5200|(0x3<<4)|5);
        TEST("MULXU.W", "E2, ER7",          0x5200|(0xA<<4)|7);
        TEST("DIVXU.W", "R0, ER1",          0x5300|(0x0<<4)|1);
        TEST("DIVXU.W", "E7, ER7",          0x5300|(0xF<<4)|7);
        UNKN(                               0x5200|(0x0<<4)|0x8);  // bit 3 of ERd reserved

        // MULXS / DIVXS via 0x01C0 / 0x01D0 prefix
        TEST("MULXS.B", "R0H, R0",          0x01C0, 0x5000|(0x0<<4)|0x0);
        TEST("MULXS.B", "R3H, R5",          0x01C0, 0x5000|(0x3<<4)|0x5);
        TEST("MULXS.W", "R0, ER0",          0x01C0, 0x5200|(0x0<<4)|0);
        TEST("MULXS.W", "E7, ER7",          0x01C0, 0x5200|(0xF<<4)|7);
        TEST("DIVXS.B", "R0H, R0",          0x01D0, 0x5100|(0x0<<4)|0x0);
        TEST("DIVXS.W", "R3, ER2",          0x01D0, 0x5300|(0x3<<4)|2);
    }
}

void test_logic() {
    // OR/XOR/AND reg-reg: byte2=(Rs<<4)|Rd
    TEST("OR",  "R0H, R0H", 0x1400|(0x0<<4)|0x0);
    TEST("OR",  "R2H, R3L", 0x1400|(0x2<<4)|0xB);
    TEST("XOR", "R0H, R0H", 0x1500|(0x0<<4)|0x0);
    TEST("XOR", "R1H, R5L", 0x1500|(0x1<<4)|0xD);
    TEST("AND", "R0H, R0H", 0x1600|(0x0<<4)|0x0);
    TEST("AND", "R4L, R3H", 0x1600|(0xC<<4)|0x3);

    // NOT Rd (0x17, bit7=0): byte2=Rd nibble (bits[6:4] must be 0)
    TEST("NOT", "R0H", 0x1700|0x0);
    TEST("NOT", "R7L", 0x1700|0xF);
    if (!is_h8300h() && !is_h8s())
        UNKN(0x1700|0x10);    // NOT: bits[6:4] must be 0 (H8/300H reuses for NOT.W)

    // OR/XOR/AND #imm,Rd (0xC0-0xEF): byte1=0x?0|(Rd nibble), byte2=imm8
    TEST("OR",  "#H'FF, R7L", 0xC000|(0xF<<8)|0xFF);
    TEST("XOR", "#H'AA, R5H", 0xD000|(0x5<<8)|0xAA);
    TEST("AND", "#H'F0, R0H", 0xE000|(0x0<<8)|0xF0);

    if (is_h8300h() || is_h8s()) {
        // NOT.W / NOT.L Rd
        TEST("NOT.W",  "R0",                0x1710|0x0);
        TEST("NOT.W",  "E5",                0x1710|0xD);
        TEST("NOT.L",  "ER0",               0x1730|0);
        TEST("NOT.L",  "ER7",               0x1730|7);
        UNKN(                               0x1730|0x8);  // bit 3 of ERd reserved

        // EXTU.W / EXTU.L Rd  (zero-extend)
        TEST("EXTU.W", "R0",                0x1750|0x0);
        TEST("EXTU.W", "E7",                0x1750|0xF);
        TEST("EXTU.L", "ER4",               0x1770|4);
        // EXTS.W / EXTS.L Rd  (sign-extend)
        TEST("EXTS.W", "R1",                0x17D0|0x1);
        TEST("EXTS.L", "ER6",               0x17F0|6);

        // AND.W / OR.W / XOR.W Rs,Rd
        TEST("AND.W",  "R0, R1",            0x6600|(0x0<<4)|0x1);
        TEST("AND.W",  "E0, R1",            0x6600|(0x8<<4)|0x1);
        TEST("OR.W",   "R0, E7",            0x6400|(0x0<<4)|0xF);
        TEST("XOR.W",  "E0, E7",            0x6500|(0x8<<4)|0xF);
        // AND.W / OR.W / XOR.W #imm16, Rd
        TEST("AND.W",  "#H'1234, R0",       0x7960|0x0, 0x1234);
        TEST("AND.W",  "#H'FFFF, E7",       0x7960|0xF, 0xFFFF);
        TEST("OR.W",   "#H'1234, R0",       0x7940|0x0, 0x1234);
        TEST("XOR.W",  "#H'1234, R0",       0x7950|0x0, 0x1234);

        // OR.L / XOR.L / AND.L #imm32, ERd
        TEST("OR.L",   "#H'AABBCCDD, ER3",  0x7A40|3, 0xAABB, 0xCCDD);
        TEST("XOR.L",  "#H'5A5A5A5A, ER4",  0x7A50|4, 0x5A5A, 0x5A5A);
        TEST("AND.L",  "#H'F0F0F0F0, ER5",  0x7A60|5, 0xF0F0, 0xF0F0);

        // AND.L / OR.L / XOR.L reg-reg via 0x01F0 prefix
        TEST("AND.L",  "ER0, ER1",          0x01F0, 0x6600|(0<<4)|1);
        TEST("AND.L",  "ER5, ER6",          0x01F0, 0x6600|(5<<4)|6);
        TEST("OR.L",   "ER0, ER1",          0x01F0, 0x6400|(0<<4)|1);
        TEST("XOR.L",  "ER3, ER4",          0x01F0, 0x6500|(3<<4)|4);
    }
}

void test_shift_rotate() {
    // Shift/rotate Rd (0x10-0x13): byte2=Rd nibble (bits[6:4] must be 0)
    // bit7=0: SHLL/SHLR/ROTXL/ROTXR; bit7=1: SHAL/SHAR/ROTL/ROTR
    TEST("SHLL",  "R0H", 0x1000|0x0);
    TEST("SHLL",  "R7L", 0x1000|0xF);
    if (!is_h8300h() && !is_h8s())
        UNKN(0x1000|0x10);    // SHLL: bits[6:4] must be 0 (H8/300H reuses for SHLL.W)

    TEST("SHAL",  "R0H", 0x1080|0x0);
    TEST("SHAL",  "R5L", 0x1080|0xD);
    if (!is_h8300h() && !is_h8s())
        UNKN(0x1080|0x10);    // SHAL: bits[6:4] must be 0 (H8/300H reuses for SHAL.W)

    TEST("SHLR",  "R0H", 0x1100|0x0);
    TEST("SHAR",  "R3L", 0x1180|0xB);

    TEST("ROTXL", "R0H", 0x1200|0x0);
    TEST("ROTL",  "R0H", 0x1280|0x0);

    TEST("ROTXR", "R0H", 0x1300|0x0);
    TEST("ROTR",  "R7L", 0x1380|0xF);

    if (is_h8300h() || is_h8s()) {
        // Shift / rotate .W and .L
        TEST("SHLL.W",  "R0",  0x1010|0x0);
        TEST("SHLL.L",  "ER0", 0x1030|0);
        TEST("SHAL.W",  "R3",  0x1090|0x3);
        TEST("SHAL.L",  "ER5", 0x10B0|5);
        TEST("SHLR.W",  "E0",  0x1110|0x8);
        TEST("SHLR.L",  "ER7", 0x1130|7);
        TEST("SHAR.W",  "R7",  0x1190|0x7);
        TEST("SHAR.L",  "ER1", 0x11B0|1);
        TEST("ROTXL.W", "R0",  0x1210|0x0);
        TEST("ROTXL.L", "ER2", 0x1230|2);
        TEST("ROTL.W",  "R4",  0x1290|0x4);
        TEST("ROTL.L",  "ER3", 0x12B0|3);
        TEST("ROTXR.W", "R0",  0x1310|0x0);
        TEST("ROTXR.L", "ER6", 0x1330|6);
        TEST("ROTR.W",  "E6",  0x1390|0xE);
        TEST("ROTR.L",  "ER0", 0x13B0|0);
    }
}

void test_bit_ops() {
    // BSET/BNOT/BCLR/BTST Rs,Rd (0x60-0x63): byte2=(Rs<<4)|Rd
    TEST("BSET", "R0H, R0H", 0x6000|(0x0<<4)|0x0);
    TEST("BSET", "R2H, R5L", 0x6000|(0x2<<4)|0xD);
    TEST("BNOT", "R0H, R0H", 0x6100|(0x0<<4)|0x0);
    TEST("BNOT", "R3L, R1H", 0x6100|(0xB<<4)|0x1);
    TEST("BCLR", "R0H, R0H", 0x6200|(0x0<<4)|0x0);
    TEST("BCLR", "R7L, R4H", 0x6200|(0xF<<4)|0x4);
    TEST("BTST", "R0H, R0H", 0x6300|(0x0<<4)|0x0);
    TEST("BTST", "R5H, R2L", 0x6300|(0x5<<4)|0xA);

    // BSET/BNOT/BCLR/BTST #bit,Rd (0x70-0x73): byte2=(bit<<4)|Rd
    TEST("BSET", "#0, R0H", 0x7000|(0<<4)|0x0);
    TEST("BSET", "#7, R7L", 0x7000|(7<<4)|0xF);
    TEST("BNOT", "#3, R2H", 0x7100|(3<<4)|0x2);
    TEST("BCLR", "#5, R4L", 0x7200|(5<<4)|0xC);
    TEST("BTST", "#1, R0H", 0x7300|(1<<4)|0x0);

    // BSET/BNOT/BCLR/BTST Rs,@Rd (0x60-0x63): byte2=(Rd<<4) byte4=(Rs<<4)
    TEST("BSET", AREG("R0H, @R0", "R0H, @ER0"), 0x7D00, 0x6000|(0x0<<4));
    TEST("BSET", AREG("R2H, @R1", "R2H, @ER1"), 0x7D10, 0x6000|(0x2<<4));
    TEST("BNOT", AREG("R0H, @R2", "R0H, @ER2"), 0x7D20, 0x6100|(0x0<<4));
    TEST("BNOT", AREG("R3L, @R3", "R3L, @ER3"), 0x7D30, 0x6100|(0xB<<4));
    TEST("BCLR", AREG("R0H, @R4", "R0H, @ER4"), 0x7D40, 0x6200|(0x0<<4));
    TEST("BCLR", AREG("R7L, @R5", "R7L, @ER5"), 0x7D50, 0x6200|(0xF<<4));
    TEST("BTST", AREG("R0H, @R6", "R0H, @ER6"), 0x7C60, 0x6300|(0x0<<4));
    TEST("BTST", "R5H, @SP",                    0x7C70, 0x6300|(0x5<<4));

    // BSET/BNOT/BCLR/BTST #bit,@Rd (0x70-0x73): byte2=(Rd<<4) byte4=(bit<<4)
    TEST("BSET", "#0, @SP",                  0x7D70, 0x7000|(0<<4));
    TEST("BSET", AREG("#7, @R6", "#7, @ER6"), 0x7D60, 0x7000|(7<<4));
    TEST("BNOT", AREG("#3, @R5", "#3, @ER5"), 0x7D50, 0x7100|(3<<4));
    TEST("BCLR", AREG("#5, @R4", "#5, @ER4"), 0x7D40, 0x7200|(5<<4));
    TEST("BTST", AREG("#1, @R3", "#1, @ER3"), 0x7C30, 0x7300|(1<<4));

    // BSET/BNOT/BCLR/BTST Rs,@aa:8 (0x60-0x63): byte2=aa:8 byte4=(Rs<<4)
    TEST("BSET", "R0H, @H'FF00:8", 0x7F00, 0x6000|(0x0<<4));
    TEST("BSET", "R2H, @H'FF10:8", 0x7F10, 0x6000|(0x2<<4));
    TEST("BNOT", "R0H, @H'FF20:8", 0x7F20, 0x6100|(0x0<<4));
    TEST("BNOT", "R3L, @H'FF30:8", 0x7F30, 0x6100|(0xB<<4));
    TEST("BCLR", "R0H, @H'FF40:8", 0x7F40, 0x6200|(0x0<<4));
    TEST("BCLR", "R7L, @H'FF50:8", 0x7F50, 0x6200|(0xF<<4));
    TEST("BTST", "R0H, @H'FF60:8", 0x7E60, 0x6300|(0x0<<4));
    TEST("BTST", "R5H, @H'FFFF:8", 0x7EFF, 0x6300|(0x5<<4));

    // BSET/BNOT/BCLR/BTST #bit,@aa:8 (0x70-0x73): byte2=aa:8 byte4=(bit<<4)
    TEST("BSET", "#0, @H'FF70:8", 0x7F70, 0x7000|(0<<4));
    TEST("BSET", "#7, @H'FF60:8", 0x7F60, 0x7000|(7<<4));
    TEST("BNOT", "#3, @H'FF50:8", 0x7F50, 0x7100|(3<<4));
    TEST("BCLR", "#5, @H'FF40:8", 0x7F40, 0x7200|(5<<4));
    TEST("BTST", "#1, @H'FF30:8", 0x7E30, 0x7300|(1<<4));

    // BST/BIST #bit,Rd (0x67): byte2=(bit<<4)|Rd (bit7=0→BST, bit7=1→BIST)
    TEST("BST",  "#0, R0H", 0x6700|(0<<4)|0x0);
    TEST("BST",  "#7, R5L", 0x6700|(7<<4)|0xD);
    TEST("BIST", "#0, R0H", 0x6780|(0<<4)|0x0);
    TEST("BIST", "#3, R6L", 0x6780|(3<<4)|0xE);

    // BOR/BIOR/BXOR/BIXOR/BAND/BIAND/BLD/BILD #bit,Rd (0x74-0x77):
    // byte2=(bit<<4)|Rd (bit7=0→Bxx, bit7=1→BIxx)
    TEST("BOR",   "#0, R0H", 0x7400|(0<<4)|0x0);
    TEST("BOR",   "#5, R3H", 0x7400|(5<<4)|0x3);
    TEST("BIOR",  "#2, R1H", 0x7480|(2<<4)|0x1);
    TEST("BXOR",  "#4, R0H", 0x7500|(4<<4)|0x0);
    TEST("BIXOR", "#1, R7L", 0x7580|(1<<4)|0xF);
    TEST("BAND",  "#6, R2H", 0x7600|(6<<4)|0x2);
    TEST("BIAND", "#3, R4L", 0x7680|(3<<4)|0xC);
    TEST("BLD",   "#7, R0H", 0x7700|(7<<4)|0x0);
    TEST("BILD",  "#0, R7L", 0x7780|(0<<4)|0xF);

    // BST/BIST #bit,@Rd (0x67): byte2=Rd byte4=(bit<<4)(bit7=0→BST, bit7=1→BIST)
    TEST("BST",  AREG("#0, @R0", "#0, @ER0"), 0x7D00, 0x6700|(0<<4));
    TEST("BST",  AREG("#7, @R5", "#7, @ER5"), 0x7D50, 0x6700|(7<<4));
    TEST("BIST", AREG("#1, @R2", "#1, @ER2"), 0x7D20, 0x6780|(1<<4));
    TEST("BIST", AREG("#3, @R6", "#3, @ER6"), 0x7D60, 0x6780|(3<<4));

    // BOR/BIOR/BXOR/BIXOR/BAND/BIAND/BLD/BILD #bit,@Rd (0x74-0x77):
    // byte2=Rd byte4=(bit<<4) (bit7=0→Bxx, bit7=1→BIxx)
    TEST("BOR",   AREG("#0, @R0", "#0, @ER0"), 0x7C00, 0x7400|(0<<4));
    TEST("BOR",   AREG("#5, @R3", "#5, @ER3"), 0x7C30, 0x7400|(5<<4));
    TEST("BIOR",  AREG("#2, @R1", "#2, @ER1"), 0x7C10, 0x7480|(2<<4));
    TEST("BXOR",  AREG("#4, @R0", "#4, @ER0"), 0x7C00, 0x7500|(4<<4));
    TEST("BIXOR", "#1, @SP",                   0x7C70, 0x7580|(1<<4));
    TEST("BAND",  AREG("#6, @R2", "#6, @ER2"), 0x7C20, 0x7600|(6<<4));
    TEST("BIAND", AREG("#3, @R4", "#3, @ER4"), 0x7C40, 0x7680|(3<<4));
    TEST("BLD",   AREG("#7, @R0", "#7, @ER0"), 0x7C00, 0x7700|(7<<4));
    TEST("BILD",  "#0, @SP",                   0x7C70, 0x7780|(0<<4));

    // BST/BIST #bit,@aa:8 (0x67): byte2=aa:8 byte4=(bit<<4)(bit7=0→BST, bit7=1→BIST)
    TEST("BST",  "#0, @H'FF00:8", 0x7F00, 0x6700|(0<<4));
    TEST("BST",  "#7, @H'FF50:8", 0x7F50, 0x6700|(7<<4));
    TEST("BIST", "#1, @H'FF20:8", 0x7F20, 0x6780|(1<<4));
    TEST("BIST", "#3, @H'FF60:8", 0x7F60, 0x6780|(3<<4));

    // BOR/BIOR/BXOR/BIXOR/BAND/BIAND/BLD/BILD #bit,@aa:8 (0x74-0x77):
    // byte2=aa:8 byte4=(bit<<4) (bit7=0→Bxx, bit7=1→BIxx)
    TEST("BOR",   "#0, @H'FF00:8", 0x7E00, 0x7400|(0<<4));
    TEST("BOR",   "#5, @H'FF30:8", 0x7E30, 0x7400|(5<<4));
    TEST("BIOR",  "#2, @H'FF10:8", 0x7E10, 0x7480|(2<<4));
    TEST("BXOR",  "#4, @H'FF00:8", 0x7E00, 0x7500|(4<<4));
    TEST("BIXOR", "#1, @H'FF70:8", 0x7E70, 0x7580|(1<<4));
    TEST("BAND",  "#6, @H'FF20:8", 0x7E20, 0x7600|(6<<4));
    TEST("BIAND", "#3, @H'FF40:8", 0x7E40, 0x7680|(3<<4));
    TEST("BLD",   "#7, @H'FF00:8", 0x7E00, 0x7700|(7<<4));
    TEST("BILD",  "#0, @H'FF70:8", 0x7E70, 0x7780|(0<<4));
}

void test_branch() {
    // Bcc d:8 / BSR d:8 — byte1 = 0x4c (condition) or 0x55 (BSR), byte2 = signed disp.
    // target = origin + 2 + disp_signed. Range: -128..+127.
    // Vary the displacement across conditions and exercise the ±128 edges.
    // Disassembler emits the canonical mnemonic (BRA, BRN, BCC, BCS), not BT/BF/BHS/BLO.

    // Trivial case: origin=0, disp=0 → target=0x0002.
    TEST("BRA", "H'0002", 0x4000|0x00);

    // ----- Edge: max positive +126 (disp = 0x7E) -----
    ATEST(0x1000, "BRA", "H'1080", 0x4000|0x7E);
    ATEST(0x1000, "BCC", "H'1080", 0x4400|0x7E);
    ATEST(0x1000, "BVS", "H'1080", 0x4900|0x7E);
    ATEST(0x1000, "BLE", "H'1080", 0x4F00|0x7E);
    ATEST(0x1000, "BSR", "H'1080", 0x5500|0x7E);

    // ----- Edge: max negative -128 (disp = 0x80) -----
    ATEST(0x1000, "BRN", "H'0F82", 0x4100|0x80);
    ATEST(0x1000, "BCS", "H'0F82", 0x4500|0x80);
    ATEST(0x1000, "BPL", "H'0F82", 0x4A00|0x80);
    ATEST(0x1000, "BSR", "H'0F82", 0x5500|0x80);

    // ----- Edge: branch to self start (disp = -2, byte = 0xFE) -----
    ATEST(0x1000, "BRN", "H'1000", 0x4100|0xFE);
    ATEST(0x1000, "BEQ", "H'1000", 0x4700|0xFE);

    // ----- Mid-range displacements (assorted) -----
    ATEST(0x1000, "BHI", "H'1004", 0x4200|0x02);
    ATEST(0x1000, "BLS", "H'0FFE", 0x4300|0xFC);
    ATEST(0x1000, "BCC", "H'107E", 0x4400|0x7C);
    ATEST(0x1000, "BCS", "H'1010", 0x4500|0x0E);
    ATEST(0x1000, "BNE", "H'1020", 0x4600|0x1E);
    ATEST(0x1000, "BVC", "H'1060", 0x4800|0x5E);
    ATEST(0x1000, "BMI", "H'0FF0", 0x4B00|0xEE);
    ATEST(0x1000, "BGE", "H'1012", 0x4C00|0x10);
    ATEST(0x1000, "BLT", "H'0FF4", 0x4D00|0xF2);
    ATEST(0x1000, "BGT", "H'0FFA", 0x4E00|0xF8);

    if (is_h8300h() || is_h8s()) {
        // Long branches BRA/Bcc/BSR :16 (post-instruction PC + disp16)
        ATEST(0x1000, "BRA", "H'1234", 0x5800, 0x1234 - 0x1004);
        ATEST(0x1000, "BHI", "H'1234", 0x5820, 0x1234 - 0x1004);
        ATEST(0x1000, "BEQ", "H'1234", 0x5870, 0x1234 - 0x1004);
        ATEST(0x1000, "BLE", "H'1234", 0x58F0, 0x1234 - 0x1004);
        ATEST(0x1000, "BSR", "H'1234", 0x5C00, 0x1234 - 0x1004);
        // Backward branch (negative disp16)
        ATEST(0x1000, "BRA", "H'0FF0", 0x5800, 0xFFEC);
    }
}

void test_jump() {
    // JMP @Rn (0x59): byte2=(Rn3<<4)
    TEST("JMP", AREG("@R0", "@ER0"), 0x5900|(0<<4));
    TEST("JMP", AREG("@R5", "@ER5"), 0x5900|(5<<4));
    TEST("JMP", "@SP",               0x5900|(7<<4));

    // JMP @abs16 (0x5A): byte2 ignored, addr from next word
    TEST("JMP", "@H'1234", 0x5A00, 0x1234);
    TEST("JMP", "@H'0000", 0x5A00, 0x0000);

    // JMP @@abs8 (0x5B): byte2=abs8
    TEST("JMP", "@@H'0000", 0x5B00|0x00);
    TEST("JMP", "@@H'00FF", 0x5B00|0xFF);

    // JSR @Rn (0x5D): byte2=(Rn3<<4)
    TEST("JSR", AREG("@R0", "@ER0"), 0x5D00|(0<<4));
    TEST("JSR", AREG("@R3", "@ER3"), 0x5D00|(3<<4));

    // JSR @abs16 (0x5E)
    TEST("JSR", "@H'5678", 0x5E00, 0x5678);

    // JSR @@abs8 (0x5F): byte2=abs8
    TEST("JSR", "@@H'0080", 0x5F00|0x80);

    if (is_h8300h() || is_h8s()) {
        // JMP / JSR @aa:24 (high byte 0 falls through to @aa:16 entry; non-zero
        // high byte routes here)
        TEST("JMP", "@H'FFFFE0:24", 0x5AFF, 0xFFE0);
        TEST("JSR", "@H'FFFFE0:24", 0x5EFF, 0xFFE0);
    }
}

// {byte-1, mask, canonical byte-2}: flip each constrained bit of byte 2.
struct IllegalCase { uint8_t b1, mask, value; };

#define WALK_ILLEGAL_CASES(cases)                                              \
    for (const auto &c : cases) {                                              \
        for (uint_fast8_t pos = 0; pos < 8; pos++) {                           \
            const uint8_t bit = static_cast<uint8_t>(1u << pos);               \
            if (!(c.mask & bit))                                               \
                continue;                                                      \
            const uint8_t b2 = static_cast<uint8_t>(c.value ^ bit);            \
            UNKN(static_cast<Config::opcode_t>((c.b1 << 8) | b2));             \
        }                                                                      \
    }

// Constraints identical on H8/300 and H8/300H.
void test_illegal_common() {
    // 0x01 SLEEP and 0x59/0x5D JMP/JSR @Rn are NOT in this shared set
    // because byte-2 flips on H8/300H land on valid prefixes
    // (0x0100/0x0140/0x01C0/0x01D0/0x01F0) or ERn (E0..E7) operands.
    // On H8S, bit 4 of byte 2 selects EXR (vs CCR) for STC/LDC. On H8S/2600,
    // bit 5 additionally selects STMAC/LDMAC (0x02|0x20 = STMAC MACH/MACL,
    // 0x03|0x20 = LDMAC ERs,MACH/MACL). Mask only the truly reserved bits.
    const uint8_t stc_ldc_mask = is_h8s2600() ? 0xC0 : is_h8s() ? 0xE0 : 0xF0;
    const IllegalCase shared[] = {
        { 0x00, 0xFF, 0x00 },           // NOP
        { 0x02, stc_ldc_mask, 0x00 },   // STC.B CCR, Rd
        { 0x03, stc_ldc_mask, 0x00 },   // LDC.B Rs, CCR
        { 0x54, 0xFF, 0x70 },           // RTS
        { 0x56, 0xFF, 0x70 },   // RTE
        { 0x70, 0x80, 0x00 },   // BSET #bit, Rd
        { 0x71, 0x80, 0x00 },   // BNOT #bit, Rd
        { 0x72, 0x80, 0x00 },   // BCLR #bit, Rd
        { 0x73, 0x80, 0x00 },   // BTST #bit, Rd
        { 0x7B, 0xFF, 0x5C },   // EEPMOV
        { 0x7C, 0x8F, 0x00 },   // bit-op @Rn read   (prefix)
        { 0x7D, 0x8F, 0x00 },   // bit-op @Rn write  (prefix)
    };
    WALK_ILLEGAL_CASES(shared);

    // EEPMOV  (prefix 0x7B5C, second word = 0x598F).
    for (uint_fast8_t pos = 0; pos < 16; pos++)
        UNKN(0x7B5C, static_cast<Config::opcode_t>(0x598F ^ (1u << pos)));

    // bit-op read second word: BTST Rn,@ / BTST #i,@ / BOR / BIOR / BXOR /
    // BIXOR / BAND / BIAND / BLD / BILD.
    auto valid_read = [](uint16_t w) {
        return ((w & 0xFF0F) == 0x6300) ||
               ((w & 0xFF8F) == 0x7300) ||
               ((w & 0xFF0F) == 0x7400) ||
               ((w & 0xFF0F) == 0x7500) ||
               ((w & 0xFF0F) == 0x7600) ||
               ((w & 0xFF0F) == 0x7700);
    };
    static constexpr uint16_t canon_read[] = {
            0x6300, 0x7300, 0x7400, 0x7500, 0x7600, 0x7700,
    };
    static constexpr uint16_t read_prefixes[] = {0x7C00, 0x7E00};   // @Rn / @aa:8
    for (const auto prefix : read_prefixes) {
        for (const auto canon : canon_read) {
            for (uint_fast8_t pos = 0; pos < 16; pos++) {
                const uint16_t flipped = canon ^ static_cast<uint16_t>(1u << pos);
                if (!valid_read(flipped))
                    UNKN(prefix, static_cast<Config::opcode_t>(flipped));
            }
        }
    }

    // bit-op write second word: BSET / BNOT / BCLR Rn,@ /
    // BST / BIST / BSET / BNOT / BCLR #i,@.
    auto valid_write = [](uint16_t w) {
        return ((w & 0xFF0F) == 0x6000) ||
               ((w & 0xFF0F) == 0x6100) ||
               ((w & 0xFF0F) == 0x6200) ||
               ((w & 0xFF8F) == 0x6700) ||
               ((w & 0xFF8F) == 0x6780) ||
               ((w & 0xFF8F) == 0x7000) ||
               ((w & 0xFF8F) == 0x7100) ||
               ((w & 0xFF8F) == 0x7200);
    };
    static constexpr uint16_t canon_write[] = {
            0x6000, 0x6100, 0x6200, 0x6700, 0x6780, 0x7000, 0x7100, 0x7200,
    };
    static constexpr uint16_t write_prefixes[] = {0x7D00, 0x7F00};  // @Rn / @aa:8
    for (const auto prefix : write_prefixes) {
        for (const auto canon : canon_write) {
            for (uint_fast8_t pos = 0; pos < 16; pos++) {
                const uint16_t flipped = canon ^ static_cast<uint16_t>(1u << pos);
                if (!valid_write(flipped))
                    UNKN(prefix, static_cast<Config::opcode_t>(flipped));
            }
        }
    }
}

void test_illegal_h8300() {
    // First-byte slots with no H8/300 instruction. Any byte-2 value is
    // illegal; one spot-check per slot is enough to lock in coverage.
    static constexpr uint8_t illegal_first[] = {
        0x52, 0x53,                     // (H8/300H: MULXU.W / DIVXU.W)
        0x57,                           // (H8/300H: TRAPA)
        0x58,                           // (H8/300H: long Bcc :16)
        0x5C,                           // (H8/300H: long BSR :16)
        0x64, 0x65, 0x66,               // (H8/300H: OR.W / XOR.W / AND.W Rs,Rd)
        0x78,                           // (H8/300H: @(d:24,ERn) prefix)
        0x7A,                           // (H8/300H: MOV.L #imm32 prefix)
    };
    for (const auto b1 : illegal_first)
        UNKN(static_cast<Config::opcode_t>(b1 << 8));

    test_illegal_common();

    // H8/300-specific byte-2 constraints.
    static constexpr IllegalCase cases[] = {
        { 0x01, 0xFF, 0x80 },   // SLEEP (H8/300H reuses 0x01 byte-2 for prefixes)
        { 0x09, 0x88, 0x00 },   // ADD.W Rs,Rd
        { 0x0A, 0xF0, 0x00 },   // INC.B Rd
        { 0x0B, 0x78, 0x00 },   // ADDS #1/#2, Rd
        { 0x0D, 0x88, 0x00 },   // MOV.W Rs,Rd
        { 0x0F, 0xF0, 0x00 },   // DAA Rd
        { 0x10, 0x70, 0x00 },   // SHLL.B Rd
        { 0x10, 0x70, 0x80 },   // SHAL.B Rd
        { 0x11, 0x70, 0x00 },   // SHLR.B Rd
        { 0x11, 0x70, 0x80 },   // SHAR.B Rd
        { 0x12, 0x70, 0x00 },   // ROTXL.B Rd
        { 0x12, 0x70, 0x80 },   // ROTL.B Rd
        { 0x13, 0x70, 0x00 },   // ROTXR.B Rd
        { 0x13, 0x70, 0x80 },   // ROTR.B Rd
        { 0x17, 0x70, 0x00 },   // NOT.B Rd
        { 0x17, 0x70, 0x80 },   // NEG.B Rd
        { 0x19, 0x88, 0x00 },   // SUB.W Rs,Rd
        { 0x1A, 0xF0, 0x00 },   // DEC.B Rd
        { 0x1B, 0x78, 0x00 },   // SUBS #1/#2, Rd
        { 0x1D, 0x88, 0x00 },   // CMP.W Rs,Rd
        { 0x1F, 0xF0, 0x00 },   // DAS Rd
        { 0x50, 0x08, 0x00 },   // MULXU.B Rs8,Rd16
        { 0x51, 0x08, 0x00 },   // DIVXU.B Rs8,Rd16
        { 0x59, 0x8F, 0x00 },   // JMP @Rn (H8/300H also allows ERn)
        { 0x5A, 0xFF, 0x00 },   // JMP @aa:16
        { 0x5D, 0x8F, 0x00 },   // JSR @Rn (H8/300H also allows ERn)
        { 0x5E, 0xFF, 0x00 },   // JSR @aa:16
        { 0x69, 0x08, 0x00 },   // MOV.W @Rs, Rd
        { 0x69, 0x08, 0x80 },   // MOV.W Rs, @Rd
        { 0x6A, 0x30, 0x00 },   // MOV.B @aa:16, Rd
        { 0x6A, 0x30, 0x40 },   // MOVFPE @aa:16, Rd
        { 0x6A, 0x30, 0x80 },   // MOV.B Rs, @aa:16
        { 0x6A, 0x30, 0xC0 },   // MOVTPE Rs, @aa:16
        { 0x6B, 0x70, 0x00 },   // MOV.W @aa:16, Rd
        { 0x6B, 0x70, 0x80 },   // MOV.W Rs, @aa:16
        { 0x6D, 0x08, 0x00 },   // MOV.W @Rs+, Rd / POP
        { 0x6D, 0x08, 0x80 },   // MOV.W Rs, @-Rd / PUSH
        { 0x79, 0xF0, 0x00 },   // MOV.W #imm16, Rd
    };
    WALK_ILLEGAL_CASES(cases);

    // 4-byte forms: bit-3-of-Rd flip lands past the abs16/disp16/imm16 read,
    // so supply the trailing word.
    UNKN(0x6B08, 0x0000);   // MOV.W @aa:16, Rd
    UNKN(0x6B88, 0x0000);   // MOV.W Rs, @aa:16
    UNKN(0x6F08, 0x0000);   // MOV.W @(d:16,Rs), Rd
    UNKN(0x6F88, 0x0000);   // MOV.W Rs, @(d:16,Rd)
    UNKN(0x7908, 0x0000);   // MOV.W #imm16, Rd

}

void test_illegal_h8300hn() {
    test_illegal_common();

    // 0x0A / 0x0F / 0x1A / 0x1F: .B (high nibble 0) paired with .L
    // (high nibble 0x80, ERd 3-bit so bit 3 reserved).
    //   0x0A INC.B Rd  | ADD.L ERs,ERd
    //   0x0F DAA Rd    | MOV.L ERs,ERd
    //   0x1A DEC.B Rd  | SUB.L ERs,ERd
    //   0x1F DAS Rd    | CMP.L ERs,ERd
    auto valid_b_l_pair = [](uint16_t w) {
        const uint8_t b2 = static_cast<uint8_t>(w & 0xFF);
        if ((b2 & 0xF0) == 0x00) return true;   // .B  Rd (any low nibble)
        // .L: bit 7 = 1, bit 3 (ERd) = 0; ERs lives in bits[6:4] (3-bit 0..7).
        if ((b2 & 0x88) == 0x80) return true;
        return false;
    };
    static constexpr uint16_t canon_b_l_pair[] = {
            0x0A00, 0x0A80, 0x0F00, 0x0F80,
            0x1A00, 0x1A80, 0x1F00, 0x1F80,
    };
    for (const auto canon : canon_b_l_pair) {
        for (uint_fast8_t pos = 0; pos < 8; pos++) {
            const uint16_t flipped = canon ^ static_cast<uint16_t>(1u << pos);
            if (!valid_b_l_pair(flipped))
                UNKN(static_cast<Config::opcode_t>(flipped));
        }
    }

    static constexpr IllegalCase cases[] = {
        { 0x52, 0x08, 0x00 },   // MULXU.W Rs, ERd
        { 0x53, 0x08, 0x00 },   // DIVXU.W Rs, ERd
        // 0x6A MOV.B @aa:16/24, MOVFPE, MOVTPE — 6 templates, bit 4 reserved.
        { 0x6A, 0x10, 0x00 },   // MOV.B @aa:16, Rd
        { 0x6A, 0x10, 0x20 },   // MOV.B @aa:24, Rd
        { 0x6A, 0x10, 0x40 },   // MOVFPE @aa:16, Rd
        { 0x6A, 0x10, 0x80 },   // MOV.B Rs, @aa:16
        { 0x6A, 0x10, 0xA0 },   // MOV.B Rs, @aa:24
        { 0x6A, 0x10, 0xC0 },   // MOVTPE Rs, @aa:16
        // 0x6B MOV.W @aa:16/24 — 4 templates, bits 4 and 6 reserved.
        { 0x6B, 0x50, 0x00 },   // MOV.W @aa:16, Rd
        { 0x6B, 0x50, 0x20 },   // MOV.W @aa:24, Rd
        { 0x6B, 0x50, 0x80 },   // MOV.W Rs, @aa:16
        { 0x6B, 0x50, 0xA0 },   // MOV.W Rs, @aa:24
    };
    WALK_ILLEGAL_CASES(cases);

    // 0x79xx: MOV.W / ADD.W / CMP.W / SUB.W / OR.W / XOR.W / AND.W #imm16, Rd.
    // Byte-2 high nibble selects op (0..6); bit 7 reserved and bits 6:4 = 7
    // is illegal.
    auto valid_79 = [](uint16_t w) {
        const uint8_t b2 = static_cast<uint8_t>(w & 0xFF);
        if (b2 & 0x80) return false;
        return (b2 & 0x70) <= 0x60;
    };
    static constexpr uint16_t canon_79[] = {
            0x7900, 0x7910, 0x7920, 0x7930, 0x7940, 0x7950, 0x7960,
    };
    for (const auto canon : canon_79) {
        for (uint_fast8_t pos = 0; pos < 8; pos++) {
            const uint16_t flipped = canon ^ static_cast<uint16_t>(1u << pos);
            if (!valid_79(flipped))
                UNKN(static_cast<Config::opcode_t>(flipped));
        }
    }

    // 0x17xx family: NOT / EXTU / NEG / EXTS in .B / .W / .L variants share
    // the byte-1 opcode; the byte-2 high nibble selects the operation, and
    // the .L variants require low-nibble bit 3 = 0 (ERd is a 3-bit field).
    auto valid_17 = [](uint16_t w) {
        const uint8_t b2 = static_cast<uint8_t>(w & 0xFF);
        switch (b2 & 0xF0) {
        case 0x00: case 0x80:                              // NOT.B / NEG.B
        case 0x10: case 0x50: case 0x90: case 0xD0:        // .W: R0..E7
            return true;
        case 0x30: case 0x70: case 0xB0: case 0xF0:        // .L: ERd 3-bit
            return (b2 & 0x08) == 0;
        default:                                           // 0x20/40/60/A0/C0/E0
            return false;
        }
    };
    static constexpr uint16_t canon_17[] = {
            0x1700, 0x1710, 0x1730, 0x1750, 0x1770,
            0x1780, 0x1790, 0x17B0, 0x17D0, 0x17F0,
    };
    for (const auto canon : canon_17) {
        for (uint_fast8_t pos = 0; pos < 8; pos++) {
            const uint16_t flipped = canon ^ static_cast<uint16_t>(1u << pos);
            if (!valid_17(flipped))
                UNKN(static_cast<Config::opcode_t>(flipped));
        }
    }

    // 0x0Bxx / 0x1Bxx: ADDS / SUBS (#1/#2/#4) and INC.W / INC.L / DEC.W /
    // DEC.L share the byte-1 opcode; templates by byte-2 high nibble.
    //   0x0B / 0x1B  byte-2 hi-nibble templates:
    //     0x00 ADDS/SUBS #1 ER    (bit 3 = 0)
    //     0x50 INC.W   #1 R/E
    //     0x70 INC.L   #1 ER     (bit 3 = 0)
    //     0x80 ADDS/SUBS #2 ER   (bit 3 = 0)
    //     0x90 ADDS/SUBS #4 ER   (bit 3 = 0)  -- H8/300H-only
    //     0xD0 DEC.W   #2 R/E
    //     0xF0 DEC.L   #2 ER     (bit 3 = 0)
    auto valid_0b_1b = [](uint16_t w) {
        const uint8_t b2 = static_cast<uint8_t>(w & 0xFF);
        const uint8_t hi = b2 & 0xF0;
        const bool needs_lo3 = (hi == 0x00 || hi == 0x70 || hi == 0x80 ||
                                hi == 0x90 || hi == 0xF0);
        switch (hi) {
        case 0x00: case 0x50: case 0x70: case 0x80:
        case 0x90: case 0xD0: case 0xF0:
            return !needs_lo3 || (b2 & 0x08) == 0;
        default:
            return false;
        }
    };
    static constexpr uint16_t canon_0b[] = {
            0x0B00, 0x0B50, 0x0B70, 0x0B80, 0x0B90, 0x0BD0, 0x0BF0,
            0x1B00, 0x1B50, 0x1B70, 0x1B80, 0x1B90, 0x1BD0, 0x1BF0,
    };
    for (const auto canon : canon_0b) {
        for (uint_fast8_t pos = 0; pos < 8; pos++) {
            const uint16_t flipped = canon ^ static_cast<uint16_t>(1u << pos);
            if (!valid_0b_1b(flipped))
                UNKN(static_cast<Config::opcode_t>(flipped));
        }
    }

    // 0x10 / 0x11 / 0x12 / 0x13: shift / rotate in .B (POS___F),
    // .W (POS___F), and .L (POS___7) variants. Valid high nibbles of
    // byte-2 on H8/300H: {0x0, 0x1, 0x3, 0x8, 0x9, 0xB}. H8S adds the
    // #2,Rd forms: {0x4, 0x5, 0x7, 0xC, 0xD, 0xF}. .L variants require
    // bit 3 = 0.
    const bool h8s = is_h8s();
    auto valid_shift_rot = [h8s](uint16_t w) {
        const uint8_t b2 = static_cast<uint8_t>(w & 0xFF);
        switch (b2 & 0xF0) {
        case 0x00: case 0x10: case 0x80: case 0x90:    // #1 .B / .W
            return true;
        case 0x30: case 0xB0:                          // #1 .L: bit 3 reserved
            return (b2 & 0x08) == 0;
        case 0x40: case 0x50: case 0xC0: case 0xD0:    // H8S #2 .B / .W
            return h8s;
        case 0x70: case 0xF0:                          // H8S #2 .L: bit 3 reserved
            return h8s && (b2 & 0x08) == 0;
        default:                                       // 0x20/60/A0/E0
            return false;
        }
    };
    static constexpr uint16_t canon_shift_rot[] = {
            0x1000, 0x1010, 0x1030, 0x1080, 0x1090, 0x10B0,    // 0x10 SHLL/SHAL
            0x1100, 0x1110, 0x1130, 0x1180, 0x1190, 0x11B0,    // 0x11 SHLR/SHAR
            0x1200, 0x1210, 0x1230, 0x1280, 0x1290, 0x12B0,    // 0x12 ROTXL/ROTL
            0x1300, 0x1310, 0x1330, 0x1380, 0x1390, 0x13B0,    // 0x13 ROTXR/ROTR
    };
    for (const auto canon : canon_shift_rot) {
        for (uint_fast8_t pos = 0; pos < 8; pos++) {
            const uint16_t flipped = canon ^ static_cast<uint16_t>(1u << pos);
            if (!valid_shift_rot(flipped))
                UNKN(static_cast<Config::opcode_t>(flipped));
        }
    }

    // Constrained byte-2 cases on H8/300H. (0x09/0x19/0x0D/0x1D have no
    // reserved bits on H8/300H — all 4 register bits are valid for E0..E7.)
    static constexpr IllegalCase lcases[] = {
        { 0x57, 0x8F, 0x00 },   // TRAPA #vec: bits 6:4 = vec, bit 7 and bits 3:0 = 0
        { 0x58, 0x0F, 0x00 },   // Bcc :16: byte-2 low nibble = 0 (4-bit cc in hi)
        { 0x5C, 0xFF, 0x00 },   // BSR :16: byte-2 = 0
    };
    WALK_ILLEGAL_CASES(lcases);

    // 0x7A family (MOV.L / ADD.L / CMP.L / SUB.L / OR.L / XOR.L / AND.L
    // #imm32, ERd). High nibble of byte 2 selects the op (0..6); bit 3
    // of the low nibble (ERd) is reserved; bit 7 must be 0.
    auto valid_7a = [](uint16_t w) {
        const uint8_t b2 = static_cast<uint8_t>(w & 0xFF);
        if ((b2 & 0x80) != 0) return false;
        if ((b2 & 0x70) > 0x60) return false;
        return (b2 & 0x08) == 0;
    };
    static constexpr uint16_t canon_7a[] = {
            0x7A00, 0x7A10, 0x7A20, 0x7A30, 0x7A40, 0x7A50, 0x7A60,
    };
    for (const auto canon : canon_7a) {
        for (uint_fast8_t pos = 0; pos < 8; pos++) {
            const uint16_t flipped = canon ^ static_cast<uint16_t>(1u << pos);
            if (!valid_7a(flipped))
                UNKN(static_cast<Config::opcode_t>(flipped));
        }
    }

    // 0x7BD4 EEPMOV.W: second word must be exactly 0x598F.
    for (uint_fast8_t pos = 0; pos < 16; pos++)
        UNKN(0x7BD4, static_cast<Config::opcode_t>(0x598F ^ (1u << pos)));

    // 0x01F0 (OR.L / XOR.L / AND.L reg-reg): second-word patterns
    // 0x64XX/0x65XX/0x66XX; both ERs and ERd are 3-bit so bit 7 and bit 3
    // of the second-word byte 2 are reserved.
    auto valid_01f0 = [](uint16_t w) {
        const uint8_t hi = static_cast<uint8_t>(w >> 8);
        if (hi != 0x64 && hi != 0x65 && hi != 0x66) return false;
        return (w & 0x88) == 0;
    };
    static constexpr uint16_t canon_01f0[] = {0x6400, 0x6500, 0x6600};
    for (const auto canon : canon_01f0) {
        for (uint_fast8_t pos = 0; pos < 16; pos++) {
            const uint16_t flipped = canon ^ static_cast<uint16_t>(1u << pos);
            if (!valid_01f0(flipped))
                UNKN(0x01F0, static_cast<Config::opcode_t>(flipped));
        }
    }

    // 0x01C0 (MULXS) / 0x01D0 (DIVXS): second-word patterns
    // 0x50XX|Rs<<4|Rd (.B, all bits valid) and 0x52XX|Rs<<4|ERd (.W, ERd
    // 3-bit so bit 3 reserved). 0x01D0 mirrors with 0x51/0x53.
    auto valid_01c0 = [](uint16_t w) {
        const uint8_t hi = static_cast<uint8_t>(w >> 8);
        if (hi == 0x50) return true;                      // MULXS.B
        if (hi == 0x52) return (w & 0x08) == 0;           // MULXS.W
        return false;
    };
    static constexpr uint16_t canon_01c0[] = {0x5000, 0x5200};
    for (const auto canon : canon_01c0) {
        for (uint_fast8_t pos = 0; pos < 16; pos++) {
            const uint16_t flipped = canon ^ static_cast<uint16_t>(1u << pos);
            if (!valid_01c0(flipped))
                UNKN(0x01C0, static_cast<Config::opcode_t>(flipped));
        }
    }
    auto valid_01d0 = [](uint16_t w) {
        const uint8_t hi = static_cast<uint8_t>(w >> 8);
        if (hi == 0x51) return true;                      // DIVXS.B
        if (hi == 0x53) return (w & 0x08) == 0;           // DIVXS.W
        return false;
    };
    static constexpr uint16_t canon_01d0[] = {0x5100, 0x5300};
    for (const auto canon : canon_01d0) {
        for (uint_fast8_t pos = 0; pos < 16; pos++) {
            const uint16_t flipped = canon ^ static_cast<uint16_t>(1u << pos);
            if (!valid_01d0(flipped))
                UNKN(0x01D0, static_cast<Config::opcode_t>(flipped));
        }
    }

    // 0x7800 (MOV @(d:24,ERn)): second-word patterns 0x6A20/0x6AA0/
    // 0x6B20/0x6BA0; Rd/Rs are 4-bit (POS___F) so only the high nibble
    // of byte 2 is constrained (0x20 load / 0xA0 store).
    auto valid_7800 = [](uint16_t w) {
        const uint8_t hi = static_cast<uint8_t>(w >> 8);
        if (hi != 0x6A && hi != 0x6B) return false;
        const uint8_t lo_hi = (w & 0xF0);
        return lo_hi == 0x20 || lo_hi == 0xA0;
    };
    static constexpr uint16_t canon_7800[] = {0x6A20, 0x6AA0, 0x6B20, 0x6BA0};
    for (const auto canon : canon_7800) {
        for (uint_fast8_t pos = 0; pos < 16; pos++) {
            const uint16_t flipped = canon ^ static_cast<uint16_t>(1u << pos);
            if (!valid_7800(flipped))
                UNKN(0x7800, static_cast<Config::opcode_t>(flipped));
        }
    }

    // Super-prefix 0x0100 / 0x0140: an unrecognized body word is illegal.
    UNKN(0x0100, 0x0000);
    UNKN(0x0140, 0x0000);

    // 24-bit absolute / displacement: the top byte of the 32-bit operand
    // slot is reserved and must be zero. The disasm bails out after only
    // the high half is consumed (see M_ABS24 / M_IDX24 split-read in
    // dis_h8300), so we feed only as many bytes as the disasm actually
    // reads before flagging the error. Walk each bit of the reserved byte.
    // H8S advanced mode reuses the same slot as @aa:32 (per H8S table 1.5),
    // so any high-byte value is part of the address and not illegal.
    const bool h8s_advanced = is_h8s() && dis8300.advancedMode();
    for (uint_fast8_t pos = 0; pos < 8; pos++) {
        const uint16_t hi = static_cast<uint16_t>(1u << pos) << 8;
        if (!h8s_advanced) {
            UNKN(0x6A20, hi);                     // MOV.B @aa:24, R0H
            UNKN(0x6B20, hi);                     // MOV.W @aa:24, R0
            UNKN(0x0100, 0x6B20, hi);             // MOV.L @aa:24, ER0
        }
        if (!is_h8s())                            // H8S uses @(d:32,ER) here
            UNKN(0x7800, 0x6A20, hi);             // MOV.B @(d:24,ER0), R0H
    }
}

// H8S family illegal patterns (both normal and advanced mode): inherits all
// of test_illegal_h8300hn (which gates @aa:24 reserved-byte tests on H8S
// advanced mode itself) and adds prefix-walks for TAS / LDM / STM / EXR /
// @(d:32,ERn). H8S/2600 adds CLRMAC / MAC / LDMAC / STMAC reservations under
// an is_h8s2600() guard. Called twice from run_tests, once per mode.
void test_illegal_h8s() {
    test_illegal_h8300hn();

    // 0x01E0 prefix (TAS): only 0x7B|er*16|C is a TAS body. Walk byte 4
    // bits to flip away from a canonical TAS.
    const IllegalCase tas_cases[] = {
        // byte 2 of opcode word: high nibble = ER (any 0..7), low nibble = C.
        { 0x7B, 0x0F, 0x0C },   // byte 4 low nibble must be C
    };
    for (const auto &c : tas_cases) {
        for (uint_fast8_t pos = 0; pos < 8; pos++) {
            const uint8_t bit = static_cast<uint8_t>(1u << pos);
            if (!(c.mask & bit))
                continue;
            const uint8_t b2 = static_cast<uint8_t>(c.value ^ bit);
            UNKN(0x01E0, static_cast<Config::opcode_t>((c.b1 << 8) | b2));
        }
    }
    // 0x01E0 with a non-7B byte 1 is illegal (no other instruction shares
    // this super-prefix on H8S/2000).
    for (uint_fast8_t pos = 0; pos < 8; pos++)
        UNKN(0x01E0, static_cast<Config::opcode_t>(((0x7B ^ (1u << pos)) << 8) | 0x0C));

    // 0x0110/0x0120/0x0130 prefix (LDM/STM): body must be 0x6D 7n (LDM) or
    // 0x6D Fn (STM). The low 3 bits must be a valid last/first register; the
    // high nibble must be 0x7 (LDM load) or 0xF (STM store).
    const Config::opcode_t ldm_stm_prefixes[] = {0x0110, 0x0120, 0x0130};
    for (auto prefix : ldm_stm_prefixes) {
        // Flip each bit of the 0x6D byte 1.
        for (uint_fast8_t pos = 0; pos < 8; pos++)
            UNKN(prefix, static_cast<Config::opcode_t>(((0x6D ^ (1u << pos)) << 8) | 0x70));
        // Byte 2 high nibble must be 7 (LDM) or F (STM); flipping bits 6:4
        // produces unrecognized direction.
        for (uint_fast8_t hi = 0; hi < 16; hi++) {
            if (hi == 0x7 || hi == 0xF)
                continue;
            UNKN(prefix, static_cast<Config::opcode_t>(0x6D00 | (hi << 4)));
        }
    }

    // 0x0141 super-prefix (EXR memory ops). Valid second words mirror the
    // 0x0140 CCR set: 0x07/06/05/04 (immediate), 0x69/6B/6D/6F (memory).
    // 0x78 is a normal prefix word (@(d:32,ERn)); 0x7C..0x7F are bit-op
    // prefixes. The decoder treats those as prefixes regardless of the
    // active super-prefix, so testing them under 0x0141 would consume
    // bytes beyond our 4-byte test stream and yield "Not enough memory"
    // instead of "Unknown instruction" -- skip them here.
    auto valid_0141_body = [](uint16_t w) {
        const uint8_t b1 = static_cast<uint8_t>(w >> 8);
        switch (b1) {
        case 0x04: case 0x05: case 0x06: case 0x07:    // ORC/XORC/ANDC/LDC #imm
        case 0x69: case 0x6B: case 0x6D: case 0x6F:    // memory forms
        case 0x78:                                      // @(d:32,ERn) prefix
        case 0x7C: case 0x7D: case 0x7E: case 0x7F:    // bit-op prefixes
            return true;
        default:
            return false;
        }
    };
    // Walk byte 1 of body to confirm only the listed prefixes are accepted.
    static constexpr uint16_t canon_0141[] = {
        0x0700, 0x0600, 0x0500, 0x0400,        // immediate forms
        0x6900, 0x6980, 0x6B00, 0x6B80,        // @ER / @ER store / @aa:16 / @aa:16 store
        0x6D00, 0x6D80, 0x6F00, 0x6F80,        // @ER+ / @-ER / @(d:16) / @(d:16) store
    };
    for (const auto canon : canon_0141) {
        for (uint_fast8_t pos = 8; pos < 16; pos++) {
            const uint16_t flipped = canon ^ static_cast<uint16_t>(1u << pos);
            if (!valid_0141_body(flipped))
                UNKN(0x0141, flipped);
        }
    }

    if (is_h8s2600()) {
        // 0x01A0 CLRMAC: byte 2 flips must land on undefined or known prefix.
        // Skip flips that hit SLEEP (0x80), CLRMAC itself (0xA0), or any
        // 0x01xx prefix (super-prefixes, LDM/STM, MAC, MULXS, DIVXS, TAS,
        // OR/XOR/AND.L).
        auto valid_or_prefix_01 = [](uint8_t b2) {
            const uint8_t hi = b2 & 0xF0;
            switch (hi) {
            case 0x00: case 0x10: case 0x20: case 0x30:
            case 0x40: case 0x60: case 0xC0: case 0xD0:
            case 0xE0: case 0xF0:
                return true;
            }
            return b2 == 0x80 || b2 == 0xA0;
        };
        for (uint_fast8_t pos = 0; pos < 8; pos++) {
            const uint8_t b2 = static_cast<uint8_t>(0xA0u ^ (1u << pos));
            if (!valid_or_prefix_01(b2))
                UNKN(static_cast<Config::opcode_t>(0x0100 | b2));
        }

        // MAC body under 0x0160 prefix: must be 0x6Dnm with n,m in 0..7.
        // Bit 3 of byte 2 (ERm > 7) and bit 7 (ERn > 7) are reserved as 0.
        UNKN(0x0160, 0x6D08);
        UNKN(0x0160, 0x6D80);
        UNKN(0x0160, 0x6D88);
        // Body byte 1 must be 0x6D under the 0x0160 prefix.
        for (uint_fast8_t pos = 0; pos < 8; pos++)
            UNKN(0x0160, static_cast<Config::opcode_t>((0x6Du ^ (1u << pos)) << 8));
    } else {
        // H8S/2600-only MAC family: canonical encodings must be illegal on
        // H8S/2000. CLRMAC (0x01A0), MAC (0x0160 6D|n*16|m), LDMAC/STMAC
        // (0x02/0x03 with bit 5 set, ERd/ERs in bits 2:0).
        UNKN(0x01A0);
        // 0x0160 is not a recognized prefix on H8S/2000; reject at the
        // first word without reading the body.
        UNKN(0x0160);
        for (uint_fast8_t r = 0; r < 8; r++) {
            UNKN(static_cast<Config::opcode_t>(0x0220 | r));   // STMAC MACH, ERd
            UNKN(static_cast<Config::opcode_t>(0x0230 | r));   // STMAC MACL, ERd
            UNKN(static_cast<Config::opcode_t>(0x0320 | r));   // LDMAC ERs, MACH
            UNKN(static_cast<Config::opcode_t>(0x0330 | r));   // LDMAC ERs, MACL
        }
    }
}

void test_sp_alias() {
    // Default: SP alias for ER7 (H8/300H+) / R7 (H8/300) in indirect modes.
    if (is_h8300h() || is_h8s()) {
        TEST("MOV.L", "@SP, ER0",   0x0100, 0x6900|(7<<4)|0);
        TEST("MOV.W", "R7, @SP",    0x6980|(7<<4)|7);
    } else {
        TEST("MOV.W", "@SP, R0",    0x6900|(7<<4)|0);
    }

    // Disable the alias: expect canonical ER7/R7 instead.
    disassembler.setOption("sp-alias", "off");
    if (is_h8300h() || is_h8s()) {
        TEST("MOV.L", "@ER7, ER0",  0x0100, 0x6900|(7<<4)|0);
        TEST("MOV.W", "R7, @ER7",   0x6980|(7<<4)|7);
    } else {
        TEST("MOV.W", "@R7, R0",    0x6900|(7<<4)|0);
    }
    disassembler.setOption("sp-alias", "on");
}

void test_advanced_mode() {
    disassembler.setOption("advanced-mode", "on");

    // @aa:8 short page in advanced mode disassembles to 0xFFFFnn.
    TEST("MOV.B", "@H'FFFFFF:8, R0H", 0x2000|(0<<8)|0xFF);
    TEST("MOV.B", "@H'FFFF80:8, R7L", 0x2000|(0xF<<8)|0x80);

    // @aa:16 sign-extends bit 15 in advanced mode.
    TEST("MOV.B", "@H'FFFF80:16, R0H", 0x6A00|0x0, 0xFF80);
    TEST("MOV.B", "@H'FF8000:16, R0H", 0x6A00|0x0, 0x8000);
    TEST("MOV.B", "@H'0080:16, R0H",   0x6A00|0x0, 0x0080);
    TEST("MOV.B", "@H'7FFF:16, R0H",   0x6A00|0x0, 0x7FFF);

    // Data @aa:24 vs @aa:32 in advanced mode: H8/300H prints :24, H8S
    // prints :32 (per H8S manual table 1.5; same bytes, different syntax).
    if (is_h8s()) {
        TEST("MOV.B", "@H'00001234:32, R0H", 0x6A20|0x0, 0x0000, 0x1234);
        TEST("MOV.B", "@H'00FFFFE0:32, R7L", 0x6A20|0xF, 0x00FF, 0xFFE0);
    } else {
        TEST("MOV.B", "@H'001234:24, R0H", 0x6A20|0x0, 0x0000, 0x1234);
        TEST("MOV.B", "@H'FFFFE0:24, R7L", 0x6A20|0xF, 0x00FF, 0xFFE0);
    }

    // JMP / JSR @aa:24 reach the top of advanced-mode space (program
    // address; same :24 form on both CPUs).
    TEST("JMP", "@H'FFFFFF:24", 0x5AFF, 0xFFFF);
    TEST("JSR", "@H'FFFFFF:24", 0x5EFF, 0xFFFF);

    if (is_h8s()) {
        // H8S advanced mode: data @aa:32 takes the full 32-bit slot.
        TEST("MOV.B", "@H'00000000:32, R0H", 0x6A20|0x0, 0x0000, 0x0000);
        TEST("MOV.B", "@H'FFFFFFFF:32, R7L", 0x6A20|0xF, 0xFFFF, 0xFFFF);
        TEST("MOV.B", "R0H, @H'00000000:32", 0x6AA0|0x0, 0x0000, 0x0000);
        TEST("MOV.B", "R7L, @H'FFFFFFFF:32", 0x6AA0|0xF, 0xFFFF, 0xFFFF);
        TEST("MOV.W", "@H'00000100:32, R0",  0x6B20|0x0, 0x0000, 0x0100);
        TEST("MOV.W", "R7, @H'FFFFFFFE:32",  0x6BA0|0x7, 0xFFFF, 0xFFFE);
        TEST("MOV.L", "@H'00000100:32, ER0", 0x0100, 0x6B20|0x0, 0x0000, 0x0100);
        TEST("MOV.L", "ER7, @H'FFFFFFFE:32", 0x0100, 0x6BA0|0x7, 0xFFFF, 0xFFFE);
        TEST("LDC",   "@H'12345678:32, CCR", 0x0140, 0x6B20, 0x1234, 0x5678);
        TEST("STC",   "CCR, @H'12345678:32", 0x0140, 0x6BA0, 0x1234, 0x5678);
        TEST("LDC",   "@H'12345678:32, EXR", 0x0141, 0x6B20, 0x1234, 0x5678);
        TEST("STC",   "EXR, @H'12345678:32", 0x0141, 0x6BA0, 0x1234, 0x5678);
    }

    disassembler.setOption("advanced-mode", "off");

    // Back to normal mode: same bytes disassemble to 16-bit page.
    TEST("MOV.B", "@H'FFFF:8, R0H",  0x2000|(0<<8)|0xFF);
    TEST("MOV.B", "@H'FF80:16, R0H", 0x6A00|0x0, 0xFF80);
}

void test_h8s_extensions() {

    // TAS @ERn (01E0 7B|er*16|C); ER7 disassembles to @SP per convention.
    TEST("TAS", "@ER0",           0x01E0, 0x7B0C);
    TEST("TAS", "@ER1",           0x01E0, 0x7B1C);
    TEST("TAS", "@ER4",           0x01E0, 0x7B4C);
    TEST("TAS", "@ER5",           0x01E0, 0x7B5C);
    TEST("TAS", "@SP",            0x01E0, 0x7B7C);

    // LDM.L @SP+, ER list -- byte 4 low bits = last register.
    TEST("LDM.L", "@SP+, ER0-ER1", 0x0110, 0x6D71);
    TEST("LDM.L", "@SP+, ER2-ER3", 0x0110, 0x6D73);
    TEST("LDM.L", "@SP+, ER4-ER5", 0x0110, 0x6D75);
    TEST("LDM.L", "@SP+, ER0-ER2", 0x0120, 0x6D72);
    TEST("LDM.L", "@SP+, ER4-ER6", 0x0120, 0x6D76);
    TEST("LDM.L", "@SP+, ER0-ER3", 0x0130, 0x6D73);
    // STM.L ER list, @-SP -- byte 4 low bits = first register.
    TEST("STM.L", "ER0-ER1, @-SP", 0x0110, 0x6DF0);
    TEST("STM.L", "ER2-ER3, @-SP", 0x0110, 0x6DF2);
    TEST("STM.L", "ER4-ER5, @-SP", 0x0110, 0x6DF4);
    TEST("STM.L", "ER0-ER2, @-SP", 0x0120, 0x6DF0);
    TEST("STM.L", "ER4-ER6, @-SP", 0x0120, 0x6DF4);
    TEST("STM.L", "ER0-ER3, @-SP", 0x0130, 0x6DF0);

    // LDC/STC EXR direct, immediate, and memory forms.
    TEST("LDC",  "R0L, EXR",           0x0318);
    TEST("LDC",  "R3H, EXR",           0x0313);
    TEST("STC",  "EXR, R1L",           0x0219);
    TEST("LDC",  "#H'A5, EXR",         0x0141, 0x07A5);
    TEST("ANDC", "#H'F0, EXR",         0x0141, 0x06F0);
    TEST("ORC",  "#H'A5, EXR",         0x0141, 0x04A5);
    TEST("XORC", "#15, EXR",           0x0141, 0x050F);
    TEST("LDC",  "@ER0, EXR",          0x0141, 0x6900);
    TEST("LDC",  "@SP, EXR",           0x0141, 0x6970);
    TEST("STC",  "EXR, @ER0",          0x0141, 0x6980);
    TEST("STC",  "EXR, @SP",           0x0141, 0x69F0);
    TEST("LDC",  "@ER0+, EXR",         0x0141, 0x6D00);
    TEST("STC",  "EXR, @-ER1",         0x0141, 0x6D90);
    TEST("LDC",  "@H'1234, EXR",       0x0141, 0x6B00, 0x1234);
    TEST("STC",  "EXR, @H'1234",       0x0141, 0x6B80, 0x1234);
    TEST("LDC",  "@(H'1234,ER0), EXR", 0x0141, 0x6F00, 0x1234);
    TEST("STC",  "EXR, @(H'1234,ER0)", 0x0141, 0x6F80, 0x1234);

    // 2-bit shift/rotate #2,Rd forms.
    TEST("SHLL",   "#2, R0H", 0x1040);
    TEST("SHLL",   "#2, R7L", 0x104F);
    TEST("SHLL.W", "#2, R0",  0x1050);
    TEST("SHLL.W", "#2, E7",  0x105F);
    TEST("SHLL.L", "#2, ER0", 0x1070);
    TEST("SHLL.L", "#2, ER7", 0x1077);
    TEST("SHAL",   "#2, R0H", 0x10C0);
    TEST("SHAL.W", "#2, R0",  0x10D0);
    TEST("SHAL.L", "#2, ER0", 0x10F0);
    TEST("SHLR",   "#2, R0H", 0x1140);
    TEST("SHLR.W", "#2, R0",  0x1150);
    TEST("SHLR.L", "#2, ER0", 0x1170);
    TEST("SHAR",   "#2, R0H", 0x11C0);
    TEST("SHAR.W", "#2, R0",  0x11D0);
    TEST("SHAR.L", "#2, ER0", 0x11F0);
    TEST("ROTXL",   "#2, R0H", 0x1240);
    TEST("ROTXL.W", "#2, R0",  0x1250);
    TEST("ROTXL.L", "#2, ER0", 0x1270);
    TEST("ROTL",    "#2, R0H", 0x12C0);
    TEST("ROTL.W",  "#2, R0",  0x12D0);
    TEST("ROTL.L",  "#2, ER0", 0x12F0);
    TEST("ROTXR",   "#2, R0H", 0x1340);
    TEST("ROTXR.W", "#2, R0",  0x1350);
    TEST("ROTXR.L", "#2, ER0", 0x1370);
    TEST("ROTR",    "#2, R0H", 0x13C0);
    TEST("ROTR.W",  "#2, R0",  0x13D0);
    TEST("ROTR.L",  "#2, ER0", 0x13F0);

    // @(d:32, ERn) addressing for MOV.B/W/L and LDC/STC CCR/EXR.
    TEST("MOV.B", "@(H'12345678:32,ER0), R0H", 0x7800, 0x6A20, 0x1234, 0x5678);
    TEST("MOV.B", "@(H'7FFFFFFF:32,SP), R7L",  0x7870, 0x6A2F, 0x7FFF, 0xFFFF);
    TEST("MOV.B", "R0H, @(H'12345678:32,ER0)", 0x7800, 0x6AA0, 0x1234, 0x5678);
    TEST("MOV.B", "R7L, @(H'7FFFFFFF:32,SP)",  0x7870, 0x6AAF, 0x7FFF, 0xFFFF);
    TEST("MOV.W", "@(H'12345678:32,ER0), R0",  0x7800, 0x6B20, 0x1234, 0x5678);
    TEST("MOV.W", "R7, @(H'7FFFFFFF:32,SP)",   0x7870, 0x6BA7, 0x7FFF, 0xFFFF);
    TEST("MOV.L", "@(H'12345678:32,ER0), ER1", 0x0100, 0x7800, 0x6B21, 0x1234, 0x5678);
    TEST("MOV.L", "ER0, @(H'12345678:32,ER0)", 0x0100, 0x7880, 0x6BA0, 0x1234, 0x5678);
    TEST("MOV.L", "ER6, @(H'7FFFFFFF:32,SP)",  0x0100, 0x78F0, 0x6BA6, 0x7FFF, 0xFFFF);
    TEST("LDC",   "@(H'12345678:32,ER0), CCR", 0x0140, 0x7800, 0x6B20, 0x1234, 0x5678);
    TEST("STC",   "CCR, @(H'12345678:32,ER0)", 0x0140, 0x7800, 0x6BA0, 0x1234, 0x5678);
    TEST("LDC",   "@(H'12345678:32,ER0), EXR", 0x0141, 0x7800, 0x6B20, 0x1234, 0x5678);
    TEST("STC",   "EXR, @(H'12345678:32,ER0)", 0x0141, 0x7800, 0x6BA0, 0x1234, 0x5678);
}

void test_h8s2600_mac() {
    TEST("CLRMAC", "", 0x01A0);

    // M_PINC with ER7 disassembles as @SP+; M_REG32 keeps the ER7 form.
    TEST("MAC", "@ER0+, @ER0+", 0x0160, 0x6D00);
    TEST("MAC", "@ER0+, @SP+",  0x0160, 0x6D07);
    TEST("MAC", "@SP+, @ER0+",  0x0160, 0x6D70);
    TEST("MAC", "@SP+, @SP+",   0x0160, 0x6D77);
    TEST("MAC", "@ER3+, @ER4+", 0x0160, 0x6D34);

    TEST("LDMAC", "ER0, MACH", 0x0320);
    TEST("LDMAC", "ER7, MACH", 0x0327);
    TEST("LDMAC", "ER0, MACL", 0x0330);
    TEST("LDMAC", "ER7, MACL", 0x0337);

    TEST("STMAC", "MACH, ER0", 0x0220);
    TEST("STMAC", "MACH, ER7", 0x0227);
    TEST("STMAC", "MACL, ER0", 0x0230);
    TEST("STMAC", "MACL, ER7", 0x0237);
}


void run_tests(const char *cpu) {
    disassembler.setCpu(cpu);
    RUN_TEST(test_system);
    RUN_TEST(test_ccr);
    RUN_TEST(test_data_move);
    RUN_TEST(test_arithmetic);
    RUN_TEST(test_logic);
    RUN_TEST(test_shift_rotate);
    RUN_TEST(test_bit_ops);
    RUN_TEST(test_branch);
    RUN_TEST(test_jump);
    RUN_TEST(test_sp_alias);
    if (is_h8300h() || is_h8s())
        RUN_TEST(test_advanced_mode);
    if (is_h8s())
        RUN_TEST(test_h8s_extensions);
    if (is_h8s2600())
        RUN_TEST(test_h8s2600_mac);
    if (is_h8s()) {
        RUN_TEST(test_illegal_h8s);
        disassembler.setOption("advanced-mode", "on");
        RUN_TEST(test_illegal_h8s);
        disassembler.setOption("advanced-mode", "off");
    } else if (is_h8300h()) {
        RUN_TEST(test_illegal_h8300hn);
    } else {
        RUN_TEST(test_illegal_h8300);
    }
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

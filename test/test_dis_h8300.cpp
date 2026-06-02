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
    UNKN(0x0180 ^ 0x80);        // SLEEP: byte2 must be 0x80
    UNKN(0x7B5C, 0x598E);       // EEPMOV: wrong trailing bytes
}

void test_ccr() {
    // STC CCR,Rd: byte2=Rd nibble (high nibble must be 0)
    TEST("STC", "CCR, R0H", 0x0200|0x0);
    TEST("STC", "CCR, R4L", 0x0200|0xC);
    UNKN(0x0200|0x10);
    UNKN(0x0200|0x80);

    // LDC Rs,CCR: byte2=Rs nibble
    TEST("LDC",  "R0H, CCR", 0x0300|0x0);
    TEST("LDC",  "R2L, CCR", 0x0300|0xA);
    UNKN(0x0300|0x10);
    UNKN(0x0300|0x80);

    // ORC/XORC/ANDC/LDC #imm,CCR: byte2=imm8
    TEST("ORC",  "#H'FF, CCR", 0x0400|0xFF);
    TEST("XORC", "#15, CCR",   0x0500|0x0F);
    TEST("ANDC", "#H'F0, CCR", 0x0600|0xF0);
    TEST("LDC",  "#0, CCR",    0x0700|0x00);
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
    TEST("MOV.B", "@R0, R0H", 0x6800|(0<<4)|0x0);
    TEST("MOV.B", "@R2, R5L", 0x6800|(2<<4)|0xD);

    // MOV.B Rs,@Rn (0x68, bit7=1): byte2=0x80|(Rn3<<4)|Rs
    TEST("MOV.B", "R0H, @R0", 0x6880|(0<<4)|0x0);
    TEST("MOV.B", "R3L, @R6", 0x6880|(6<<4)|0xB);

    // MOV.W @Rn,Rd (0x69, bit7=0): byte2=(Rn3<<4)|Rd3
    TEST("MOV.W", "@R0, R0", 0x6900|(0<<4)|0);
    TEST("MOV.W", "@SP, R5", 0x6900|(7<<4)|5);

    // MOV.W Rs,@Rn (0x69, bit7=1): byte2=0x80|(Rn3<<4)|Rs3
    TEST("MOV.W", "R0, @R0", 0x6980|(0<<4)|0);
    TEST("MOV.W", "R7, @SP", 0x6980|(7<<4)|7);

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
    TEST("MOV.B", "@R0+, R0H", 0x6C00|(0<<4)|0x0);
    TEST("MOV.B", "@R3+, R5L", 0x6C00|(3<<4)|0xD);
    ERRT("MOV.B", "@SP+, R5L", ILLEGAL_SIZE, "@SP+, R5L", 0x6C00|(7<<4)|0xD);

    // MOV.B Rs,@-Rn (0x6C, bit7=1): byte2=0x80|(Rn3<<4)|Rs
    TEST("MOV.B", "R0H, @-R0", 0x6C80|(0<<4)|0x0);
    TEST("MOV.B", "R4L, @-R2", 0x6C80|(2<<4)|0xC);
    ERRT("MOV.B", "R4L, @-SP", ILLEGAL_SIZE, "@-SP", 0x6C80|(7<<4)|0xC);

    // MOV.W @Rn+,Rd (0x6D, bit7=0, Rn!=SP): byte2=(Rn3<<4)|Rd3
    TEST("MOV.W", "@R0+, R0", 0x6D00|(0<<4)|0);
    TEST("MOV.W", "@R2+, R4", 0x6D00|(2<<4)|4);

    // POP Rd (0x6D, Rn=SP=R7, bit7=0): byte2=0x70|Rd3
    TEST("POP",  "R0", 0x6D70|0);
    TEST("POP",  "R5", 0x6D70|5);

    // MOV.W Rs,@-Rn (0x6D, bit7=1, Rn!=SP): byte2=0x80|(Rn3<<4)|Rs3
    TEST("MOV.W", "R0, @-R0", 0x6D80|(0<<4)|0);
    TEST("MOV.W", "R5, @-R5", 0x6D80|(5<<4)|5);

    // PUSH Rs (0x6D, Rn=SP=R7, bit7=1): byte2=0xF0|Rs3
    TEST("PUSH", "R0", 0x6DF0|0);
    TEST("PUSH", "R6", 0x6DF0|6);

    // MOV.B @(d16,Rn),Rd (0x6E, bit7=0): byte2=(Rn3<<4)|Rd, then disp16
    TEST("MOV.B", "@(H'0000,R0), R0H", 0x6E00|(0<<4)|0x0, 0x0000);
    TEST("MOV.B", "@(H'0010,R2), R3L", 0x6E00|(2<<4)|0xB, 0x0010);
    TEST("MOV.B", "@(H'FFFF,SP), R7H", 0x6E00|(7<<4)|0x7, 0xFFFF);

    // MOV.B Rs,@(d16,Rn) (0x6E, bit7=1): byte2=0x80|(Rn3<<4)|Rs
    TEST("MOV.B", "R0H, @(H'0000,R0)", 0x6E80|(0<<4)|0x0, 0x0000);
    TEST("MOV.B", "R4L, @(H'0001,R3)", 0x6E80|(3<<4)|0xC, 0x0001);

    // MOV.W @(d16,Rn),Rd (0x6F, bit7=0): byte2=(Rn3<<4)|Rd3
    TEST("MOV.W", "@(H'0000,R0), R0",  0x6F00|(0<<4)|0, 0x0000);
    TEST("MOV.W", "@(H'0020,SP), R2",  0x6F00|(7<<4)|2, 0x0020);

    // MOV.W Rs,@(d16,Rn) (0x6F, bit7=1): byte2=0x80|(Rn3<<4)|Rs3
    TEST("MOV.W", "R0, @(H'0000,R0)", 0x6F80|(0<<4)|0, 0x0000);
    TEST("MOV.W", "R5, @(H'FFFE,R6)", 0x6F80|(6<<4)|5, 0xFFFE);

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
    UNKN(0x1A00|0xF0);        // DEC: high nibble of byte2 must be 0

    // ADDS/SUBS #1/#2,Rd: byte2=Rd3 (bit7=0→#1, bit7=1→#2)
    TEST("ADDS", "#1, R0", 0x0B00|0);
    TEST("ADDS", "#1, R5", 0x0B00|5);
    TEST("ADDS", "#2, R0", 0x0B80|0);
    TEST("ADDS", "#2, SP", 0x0B80|7);

    TEST("SUBS", "#1, R0", 0x1B00|0);
    TEST("SUBS", "#1, R3", 0x1B00|3);
    TEST("SUBS", "#2, R0", 0x1B80|0);
    TEST("SUBS", "#2, SP", 0x1B80|7);

    TEST("DAA", "R0H", 0x0F00|0x0);
    TEST("DAA", "R7L", 0x0F00|0xF);
    UNKN(0x0F00|0x10);        // DAA: high nibble of byte2 must be 0

    TEST("DAS", "R0H", 0x1F00|0x0);
    TEST("DAS", "R7L", 0x1F00|0xF);
    UNKN(0x1F00|0x80);        // DAS: high nibble of byte2 must be 0

    // MULXU/DIVXU Rs8,Rd16: byte2=(Rs nibble<<4)|Rd3
    TEST("MULXU", "R0H, R0", 0x5000|(0x0<<4)|0);
    TEST("MULXU", "R3H, R5", 0x5000|(0x3<<4)|5);
    TEST("DIVXU", "R0H, R0", 0x5100|(0x0<<4)|0);
    TEST("DIVXU", "R7L, R2", 0x5100|(0xF<<4)|2);

    // NEG Rd (0x17, bit7=1): byte2=0x80|Rd nibble
    TEST("NEG", "R0H", 0x1780|0x0);
    TEST("NEG", "R7L", 0x1780|0xF);
    UNKN(0x1780|0x10);        // NEG: bits[6:4] of byte2 must be 0

    // ADD/ADDX/CMP/SUBX #imm,Rd (0x80-0xBF): byte1=0x?0|(Rd nibble), byte2=imm8
    TEST("ADD.B", "#0, R0H",    0x8000|(0x0<<8)|0x00);
    TEST("ADD.B", "#H'FF, R7L", 0x8000|(0xF<<8)|0xFF);
    TEST("ADDX",  "#1, R0H",    0x9000|(0x0<<8)|0x01);
    TEST("CMP.B", "#H'55, R5H", 0xA000|(0x5<<8)|0x55);
    TEST("SUBX",  "#2, R2L",    0xB000|(0xA<<8)|0x02);
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
    UNKN(0x1700|0x10);        // NOT: bits[6:4] of byte2 must be 0

    // OR/XOR/AND #imm,Rd (0xC0-0xEF): byte1=0x?0|(Rd nibble), byte2=imm8
    TEST("OR",  "#H'FF, R7L", 0xC000|(0xF<<8)|0xFF);
    TEST("XOR", "#H'AA, R5H", 0xD000|(0x5<<8)|0xAA);
    TEST("AND", "#H'F0, R0H", 0xE000|(0x0<<8)|0xF0);
}

void test_shift_rotate() {
    // Shift/rotate Rd (0x10-0x13): byte2=Rd nibble (bits[6:4] must be 0)
    // bit7=0: SHLL/SHLR/ROTXL/ROTXR; bit7=1: SHAL/SHAR/ROTL/ROTR
    TEST("SHLL",  "R0H", 0x1000|0x0);
    TEST("SHLL",  "R7L", 0x1000|0xF);
    UNKN(0x1000|0x10);        // SHLL: bits[6:4] of byte2 must be 0

    TEST("SHAL",  "R0H", 0x1080|0x0);
    TEST("SHAL",  "R5L", 0x1080|0xD);
    UNKN(0x1080|0x10);        // SHAL: bits[6:4] of byte2 must be 0

    TEST("SHLR",  "R0H", 0x1100|0x0);
    TEST("SHAR",  "R3L", 0x1180|0xB);

    TEST("ROTXL", "R0H", 0x1200|0x0);
    TEST("ROTL",  "R0H", 0x1280|0x0);

    TEST("ROTXR", "R0H", 0x1300|0x0);
    TEST("ROTR",  "R7L", 0x1380|0xF);
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
    TEST("BSET", "R0H, @R0", 0x7D00, 0x6000|(0x0<<4));
    TEST("BSET", "R2H, @R1", 0x7D10, 0x6000|(0x2<<4));
    TEST("BNOT", "R0H, @R2", 0x7D20, 0x6100|(0x0<<4));
    TEST("BNOT", "R3L, @R3", 0x7D30, 0x6100|(0xB<<4));
    TEST("BCLR", "R0H, @R4", 0x7D40, 0x6200|(0x0<<4));
    TEST("BCLR", "R7L, @R5", 0x7D50, 0x6200|(0xF<<4));
    TEST("BTST", "R0H, @R6", 0x7C60, 0x6300|(0x0<<4));
    TEST("BTST", "R5H, @SP", 0x7C70, 0x6300|(0x5<<4));

    // BSET/BNOT/BCLR/BTST #bit,@Rd (0x70-0x73): byte2=(Rd<<4) byte4=(bit<<4)
    TEST("BSET", "#0, @SP", 0x7D70, 0x7000|(0<<4));
    TEST("BSET", "#7, @R6", 0x7D60, 0x7000|(7<<4));
    TEST("BNOT", "#3, @R5", 0x7D50, 0x7100|(3<<4));
    TEST("BCLR", "#5, @R4", 0x7D40, 0x7200|(5<<4));
    TEST("BTST", "#1, @R3", 0x7C30, 0x7300|(1<<4));

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
    TEST("BST",  "#0, @R0", 0x7D00, 0x6700|(0<<4));
    TEST("BST",  "#7, @R5", 0x7D50, 0x6700|(7<<4));
    TEST("BIST", "#1, @R2", 0x7D20, 0x6780|(1<<4));
    TEST("BIST", "#3, @R6", 0x7D60, 0x6780|(3<<4));

    // BOR/BIOR/BXOR/BIXOR/BAND/BIAND/BLD/BILD #bit,@Rd (0x74-0x77):
    // byte2=Rd byte4=(bit<<4) (bit7=0→Bxx, bit7=1→BIxx)
    TEST("BOR",   "#0, @R0", 0x7C00, 0x7400|(0<<4));
    TEST("BOR",   "#5, @R3", 0x7C30, 0x7400|(5<<4));
    TEST("BIOR",  "#2, @R1", 0x7C10, 0x7480|(2<<4));
    TEST("BXOR",  "#4, @R0", 0x7C00, 0x7500|(4<<4));
    TEST("BIXOR", "#1, @SP", 0x7C70, 0x7580|(1<<4));
    TEST("BAND",  "#6, @R2", 0x7C20, 0x7600|(6<<4));
    TEST("BIAND", "#3, @R4", 0x7C40, 0x7680|(3<<4));
    TEST("BLD",   "#7, @R0", 0x7C00, 0x7700|(7<<4));
    TEST("BILD",  "#0, @SP", 0x7C70, 0x7780|(0<<4));

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
}

void test_jump() {
    // JMP @Rn (0x59): byte2=(Rn3<<4)
    TEST("JMP", "@R0", 0x5900|(0<<4));
    TEST("JMP", "@R5", 0x5900|(5<<4));
    TEST("JMP", "@SP", 0x5900|(7<<4));

    // JMP @abs16 (0x5A): byte2 ignored, addr from next word
    TEST("JMP", "@H'1234", 0x5A00, 0x1234);
    TEST("JMP", "@H'0000", 0x5A00, 0x0000);

    // JMP @@abs8 (0x5B): byte2=abs8
    TEST("JMP", "@@H'0000", 0x5B00|0x00);
    TEST("JMP", "@@H'00FF", 0x5B00|0xFF);

    // JSR @Rn (0x5D): byte2=(Rn3<<4)
    TEST("JSR", "@R0", 0x5D00|(0<<4));
    TEST("JSR", "@R3", 0x5D00|(3<<4));

    // JSR @abs16 (0x5E)
    TEST("JSR", "@H'5678", 0x5E00, 0x5678);

    // JSR @@abs8 (0x5F): byte2=abs8
    TEST("JSR", "@@H'0080", 0x5F00|0x80);
}

void test_illegal_h8300() {
    // First-byte slots with no H8/300 instruction. Any byte-2 value is
    // illegal; one spot-check per slot is enough to lock in coverage.
    static constexpr uint8_t illegal_first[] = {
        0x52, 0x53,                     // gap between DIVXU and RTS
        0x57,                           // no TRAPA in H8/300 (added H8/300H)
        0x58,                           // no long Bcc :16 (added H8/300H)
        0x5C,                           // no long BSR :16 (added H8/300H)
        0x64, 0x65, 0x66,               // no AND.L / OR.L / XOR.L (added H8/300H)
        0x78,                           // no @(d:24,ERn) prefix (added H8/300H)
        0x7A,                           // no MOV.L #imm32 (added H8/300H)
    };
    for (const auto b1 : illegal_first)
        UNKN(static_cast<Config::opcode_t>(b1 << 8));

    // For each "byte-1 fixes the opcode, byte-2 has constrained bits"
    // encoding: flip every constrained bit of byte 2 in turn and check it
    // disassembles as unknown. Covers each reserved-bit assertion without
    // enumerating every byte-2 value.
    // The dual-canonical entries (SHLL/SHAL, SHLR/SHAR, ROTXL/ROTL,
    // ROTXR/ROTR, NOT/NEG) list both canonical byte-2 values; a single
    // {b1, mask, value} only flips bits around one canonical and would miss
    // the other family's reserved-bit violations.
    struct Case { uint8_t b1, mask, value; };
    static constexpr Case cases[] = {
        { 0x00, 0xFF, 0x00 },   // NOP:    byte2 = 0x00 only
        { 0x01, 0xFF, 0x80 },   // SLEEP:  byte2 = 0x80 only
        { 0x02, 0xF0, 0x00 },   // STC.B CCR,Rd:  byte2 = 0|Rd nibble
        { 0x03, 0xF0, 0x00 },   // LDC.B Rs,CCR:  byte2 = 0|Rs nibble
        { 0x09, 0x88, 0x00 },   // ADD.W Rs,Rd:   byte2 bits[7] and [3] must be 0
        { 0x0A, 0xF0, 0x00 },   // INC.B Rd:      byte2 high nibble = 0
        { 0x0B, 0x78, 0x00 },   // ADDS.W #1/#2,Rd: bits[6:4] and [3] must be 0
        { 0x0D, 0x88, 0x00 },   // MOV.W Rs,Rd
        { 0x0F, 0xF0, 0x00 },   // DAA.B Rd
        { 0x10, 0x70, 0x00 },   // SHLL.B Rd: bits[6:4] = 0
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
        { 0x1B, 0x78, 0x00 },   // SUBS.W #1/#2,Rd
        { 0x1D, 0x88, 0x00 },   // CMP.W Rs,Rd
        { 0x1F, 0xF0, 0x00 },   // DAS.B Rd
        { 0x50, 0x08, 0x00 },   // MULXU.B Rs8,Rd16: bit 3 of Rd (16-bit, 3-bit) reserved
        { 0x51, 0x08, 0x00 },   // DIVXU.B Rs8,Rd16
        { 0x54, 0xFF, 0x70 },   // RTS:  byte2 = 0x70 only
        { 0x56, 0xFF, 0x70 },   // RTE:  byte2 = 0x70 only
        { 0x59, 0x8F, 0x00 },   // JMP @Rn: byte2 = Rn3<<4 (bit[7] and bits[3:0] = 0)
        { 0x5A, 0xFF, 0x00 },   // JMP @aa:16: byte2 = 0x00 only
        { 0x5D, 0x8F, 0x00 },   // JSR @Rn
        { 0x5E, 0xFF, 0x00 },   // JSR @aa:16: byte2 = 0x00 only
        // MOV.W register pointers: Rd is a 16-bit register (3-bit field on
        // H8/300), so bit 3 of byte 2 is reserved across both load (bit 7 =
        // 0) and store (bit 7 = 1) canonical forms. 0x69 / 0x6D are 2-byte
        // instructions so the Rd check fires before any additional read;
        // 0x6B / 0x6F / 0x79 read trailing bytes first and are handled
        // separately below.
        { 0x69, 0x08, 0x00 },   // MOV.W @Rs, Rd  (load)
        { 0x69, 0x08, 0x80 },   // MOV.W Rs, @Rd  (store)
        { 0x6A, 0x30, 0x00 },   // MOV.B @aa:16, Rd  (load)
        { 0x6A, 0x30, 0x40 },   // MOVFPE @aa:16, Rd
        { 0x6A, 0x30, 0x80 },   // MOV.B Rs, @aa:16  (store)
        { 0x6A, 0x30, 0xC0 },   // MOVTPE Rs, @aa:16
        { 0x6B, 0x70, 0x00 },   // MOV.W @aa:16, Rd  (load): bits[6:4] = 0
        { 0x6B, 0x70, 0x80 },   // MOV.W Rs, @aa:16  (store)
        { 0x6D, 0x08, 0x00 },   // MOV.W @Rs+, Rd / POP   (Rs = SP)
        { 0x6D, 0x08, 0x80 },   // MOV.W Rs, @-Rd / PUSH
        // MOV.W #imm16, Rd: byte 2 high nibble must be 0; bit 3 of Rd
        // also reserved, but that needs the imm16 trailer (handled below).
        { 0x79, 0xF0, 0x00 },
        // BSET/BNOT/BCLR/BTST #bit, Rd: byte 2 bit 7 reserved (bit field
        // is 3-bit).
        { 0x70, 0x80, 0x00 },
        { 0x71, 0x80, 0x00 },
        { 0x72, 0x80, 0x00 },
        { 0x73, 0x80, 0x00 },
        { 0x7B, 0xFF, 0x5C },   // EEPMOV: byte2 = 0x5C only
        { 0x7C, 0x8F, 0x00 },   // bit-op @Rd (read):  byte2 = 0|Rd3<<4|0
        { 0x7D, 0x8F, 0x00 },   // bit-op @Rd (write): byte2 = 0|Rd3<<4|0
    };
    for (const auto &c : cases) {
        for (uint_fast8_t pos = 0; pos < 8; pos++) {
            const uint8_t bit = static_cast<uint8_t>(1u << pos);
            if (!(c.mask & bit))
                continue;  // bit unconstrained, nothing to verify
            const uint8_t b2 = static_cast<uint8_t>(c.value ^ bit);
            UNKN(static_cast<Config::opcode_t>((c.b1 << 8) | b2));
        }
    }

    // 4-byte instructions with bit-3-of-Rd reserved: the disasm reads the
    // trailing word (abs16 / disp16 / imm16) before reaching the Rd check,
    // so we supply that trailer explicitly. The mnemonic decode then errors
    // UNKNOWN once it sees Rd = 8..F (16-bit Rd is 3-bit on H8/300).
    UNKN(0x6B08, 0x0000);   // MOV.W @aa:16, Rd  bit 3 of Rd (load)
    UNKN(0x6B88, 0x0000);   // MOV.W Rs, @aa:16  bit 3 of Rd (store)
    UNKN(0x6F08, 0x0000);   // MOV.W @(d:16,Rs), Rd  (load)
    UNKN(0x6F88, 0x0000);   // MOV.W Rs, @(d:16,Rd)  (store)
    UNKN(0x7908, 0x0000);   // MOV.W #imm16, Rd

    // Two-word opcodes: walk single-bit flips of each canonical valid
    // second word, skipping any flip that happens to land on another
    // valid template (e.g., 0x6300 -> 0x7300 is still valid in 0x7C00).

    // EEPMOV (0x7B5C ...): second word must be exactly 0x598F.
    for (uint_fast8_t pos = 0; pos < 16; pos++)
        UNKN(0x7B5C, static_cast<Config::opcode_t>(0x598F ^ (1u << pos)));

    // Bit op @Rn (read, 0x7C00 prefix): second word must match one of
    //   BTST Rn,@      : 0x63|rn|0   (low nibble = 0)
    //   BTST #imm,@    : 0x73|imm|0  (bit[7] = 0, bits[3:0] = 0)
    //   BOR/BIOR #imm,@: 0x74|imm|0
    //   BXOR/BIXOR     : 0x75|imm|0
    //   BAND/BIAND     : 0x76|imm|0
    //   BLD/BILD       : 0x77|imm|0
    auto valid_7c00 = [](uint16_t w) {
        return ((w & 0xFF0F) == 0x6300) ||
               ((w & 0xFF8F) == 0x7300) ||
               ((w & 0xFF0F) == 0x7400) ||
               ((w & 0xFF0F) == 0x7500) ||
               ((w & 0xFF0F) == 0x7600) ||
               ((w & 0xFF0F) == 0x7700);
    };
    static constexpr uint16_t canon_7c00[] = {0x6300, 0x7300, 0x7400, 0x7500, 0x7600, 0x7700};
    for (const auto canon : canon_7c00) {
        for (uint_fast8_t pos = 0; pos < 16; pos++) {
            const uint16_t flipped = canon ^ static_cast<uint16_t>(1u << pos);
            if (!valid_7c00(flipped))
                UNKN(0x7C00, static_cast<Config::opcode_t>(flipped));
        }
    }

    // Bit op @Rn (write, 0x7D00 prefix): second word must match one of
    //   BSET/BNOT/BCLR Rn,@   : 0x60/61/62|rn|0
    //   BST/BIST #imm,@       : 0x67|0/8|imm|0
    //   BSET/BNOT/BCLR #imm,@ : 0x70/71/72|imm|0
    auto valid_7d00 = [](uint16_t w) {
        return ((w & 0xFF0F) == 0x6000) ||
               ((w & 0xFF0F) == 0x6100) ||
               ((w & 0xFF0F) == 0x6200) ||
               ((w & 0xFF8F) == 0x6700) ||
               ((w & 0xFF8F) == 0x6780) ||
               ((w & 0xFF8F) == 0x7000) ||
               ((w & 0xFF8F) == 0x7100) ||
               ((w & 0xFF8F) == 0x7200);
    };
    static constexpr uint16_t canon_7d00[] = {
            0x6000, 0x6100, 0x6200, 0x6700, 0x6780, 0x7000, 0x7100, 0x7200,
    };
    for (const auto canon : canon_7d00) {
        for (uint_fast8_t pos = 0; pos < 16; pos++) {
            const uint16_t flipped = canon ^ static_cast<uint16_t>(1u << pos);
            if (!valid_7d00(flipped))
                UNKN(0x7D00, static_cast<Config::opcode_t>(flipped));
        }
    }
    // bit-op @aa:8 read (prefix 0x7E00) / write (prefix 0x7F00): same
    // second-word tables as 0x7C00 / 0x7D00.
    for (const auto canon : canon_7c00) {
        for (uint_fast8_t pos = 0; pos < 16; pos++) {
            const uint16_t flipped = canon ^ static_cast<uint16_t>(1u << pos);
            if (!valid_7c00(flipped))
                UNKN(0x7E00, static_cast<Config::opcode_t>(flipped));
        }
    }
    for (const auto canon : canon_7d00) {
        for (uint_fast8_t pos = 0; pos < 16; pos++) {
            const uint16_t flipped = canon ^ static_cast<uint16_t>(1u << pos);
            if (!valid_7d00(flipped))
                UNKN(0x7F00, static_cast<Config::opcode_t>(flipped));
        }
    }
}

void test_sp_alias() {
    // Default: SP alias for R7 in indirect modes.
    TEST("MOV.W", "@SP, R0",    0x6900|(7<<4)|0);
    // Disable the alias: expect canonical R7.
    disassembler.setOption("sp-alias", "off");
    TEST("MOV.W", "@R7, R0",    0x6900|(7<<4)|0);
    disassembler.setOption("sp-alias", "on");
}

void run_tests(const char *cpu) {
    disassembler.setCpu(cpu);
    RUN_TEST(test_cpu);
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
    RUN_TEST(test_illegal_h8300);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

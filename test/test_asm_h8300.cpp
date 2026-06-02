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

#include "asm_h8300.h"
#include "test_asm_helper.h"

using namespace libasm;
using namespace libasm::h8300;
using namespace libasm::test;

AsmH8300 as8300;
Assembler &assembler(as8300);

void set_up() {
    assembler.reset();
}

void tear_down() {
    symtab.reset();
}

// clang-format off

void test_cpu() {
    EQUALS("cpu H8/300", true,    assembler.setCpu("H8/300"));
    EQUALS_P("cpu H8/300", "H8/300", assembler.config().cpu_P());

    EQUALS("cpu H8300", false, assembler.setCpu("H8300"));
}

void test_system() {
    TEST("NOP",    0x0000);
    TEST("SLEEP",  0x0180);
    TEST("RTS",    0x5470);
    TEST("RTE",    0x5670);
    TEST("EEPMOV", 0x7B5C, 0x598F);
}

void test_ccr() {
    // STC CCR,Rd: byte2=Rd nibble
    TEST("STC   CCR, R0H", 0x0200|0x0);
    TEST("STC.B CCR, R4L", 0x0200|0xC);
    ERRT("STC.W CCR, R4L", ILLEGAL_SIZE, ".W CCR, R4L");

    // LDC Rs,CCR: byte2=Rs nibble
    TEST("LDC   R0H, CCR", 0x0300|0x0);
    TEST("LDC.b R2L, CCR", 0x0300|0xA);
    ERRT("LDC.w R2L, CCR", ILLEGAL_SIZE, ".w R2L, CCR");

    // ORC/XORC/ANDC/LDC #imm,CCR: byte2=imm8
    TEST("ORC    #H'FF, CCR", 0x0400|0xFF);
    TEST("ORC.B  #H'F0, CCR", 0x0400|0xF0);
    TEST("XORC   #15, CCR",   0x0500|0x0F);
    TEST("XORC.B #H'55, CCR", 0x0500|0x55);
    TEST("ANDC   #H'F0, CCR", 0x0600|0xF0);
    TEST("ANDC.B #H'0F, CCR", 0x0600|0x0F);
    TEST("LDC    #0, CCR",    0x0700|0x00);
    ERRT("LDC    #0:16, CCR", OPERAND_NOT_ALLOWED, "#0:16, CCR");
}

void test_data_move() {
    // MOV.B Rs,Rd: byte2=(Rs<<4)|Rd
    TEST("MOV.B R0H, R0H", 0x0C00|(0x0<<4)|0x0);
    TEST("MOV.B R1H, R3L", 0x0C00|(0x1<<4)|0xB);
    TEST("MOV   R7L, R7L", 0x0C00|(0xF<<4)|0xF);
    ERRT("MOV   R1H, R3",  OPERAND_NOT_ALLOWED, "R1H, R3");

    // MOV.W Rs,Rd: byte2=(Rs3<<4)|Rd3
    TEST("MOV.W R0, R0", 0x0D00|(0<<4)|0);
    TEST("MOV   R2, R5", 0x0D00|(2<<4)|5);

    // MOV.B @aa:8,Rd (0x20-0x2F): byte1=(0x20|Rd nibble), byte2=aa
    // Rd nibble: 0..7=R0H..R7H, 8..F=R0L..R7L (full 16 byte registers)
    TEST("MOV.B @H'00:8,   R0H", 0x2000|(0<<8)|0x00);
    TEST("MOV.B @H'FF12:8, R3H", 0x2000|(3<<8)|0x12);
    TEST("MOV   @H'FF12,   R3H", 0x2000|(3<<8)|0x12);
    TEST("MOV   @H'12:8,   R3H", 0x2000|(3<<8)|0x12);
    TEST("MOV   @H'12,     R3H", 0x6A00|3, 0x0012);
    TEST("MOV.B @H'FF34:8, R0L", 0x2000|(8<<8)|0x34);
    TEST("MOV.B @H'FFFF:8, R7L", 0x2000|(0xF<<8)|0xFF);

    // MOV.B Rs,@aa:8 (0x30-0x3F): byte1=(0x30|Rs nibble), byte2=aa
    TEST("MOV.B R0H, @H'00:8",   0x3000|(0<<8)|0x00);
    TEST("MOV.B R5H, @H'FFAB:8", 0x3000|(5<<8)|0xAB);
    TEST("MOV   R5H, @H'FFAB",   0x3000|(5<<8)|0xAB);
    TEST("MOV   R5H, @H'AB:8",   0x3000|(5<<8)|0xAB);
    TEST("MOV   R5H, @H'AB",     0x6A80|5, 0x00AB);
    TEST("MOV.B R0L, @H'FF34:8", 0x3000|(8<<8)|0x34);
    TEST("MOV.B R7L, @H'FFFF:8", 0x3000|(0xF<<8)|0xFF);

    // MOV.B @Rn,Rd (0x68, bit7=0): byte2=(Rn3<<4)|Rd
    TEST("MOV.B @R0, R0H", 0x6800|(0<<4)|0x0);
    TEST("MOV   @R2, R5L", 0x6800|(2<<4)|0xD);

    // MOV.B Rs,@Rn (0x68, bit7=1): byte2=0x80|(Rn3<<4)|Rs
    TEST("MOV.B R0H, @R0", 0x6880|(0<<4)|0x0);
    TEST("MOV   R3L, @R6", 0x6880|(6<<4)|0xB);

    // MOV.W @Rn,Rd (0x69, bit7=0): byte2=(Rn3<<4)|Rd3
    TEST("MOV.W @R0, R0", 0x6900|(0<<4)|0);
    TEST("MOV   @R3, R5", 0x6900|(3<<4)|5);

    // MOV.W Rs,@Rn (0x69, bit7=1): byte2=0x80|(Rn3<<4)|Rs3
    TEST("MOV.W R0, @R0", 0x6980|(0<<4)|0);
    TEST("MOV   R7, @R4", 0x6980|(4<<4)|7);

    // MOV.B @abs16,Rd (0x6A, bits[7:6]=00): byte2=Rd nibble
    TEST("MOV.B @H'1234:16, R0H", 0x6A00|0x0, 0x1234);
    TEST("MOV   @H'1234:16, R0H", 0x6A00|0x0, 0x1234);
    TEST("MOV.B @H'FFFF:16, R7L", 0x6A00|0xF, 0xFFFF);
    TEST("MOV   @H'FFFF,    R7L", 0x2000|(0xF<<8)|0xFF);

    // MOV.B Rs,@abs16 (0x6A, bits[7:6]=10): byte2=0x80|Rs nibble
    TEST("MOV.B R0H, @H'1234:16", 0x6A80|0x0, 0x1234);
    TEST("MOV   R0H, @H'1234:16", 0x6A80|0x0, 0x1234);
    TEST("MOV.B R3L, @H'0100:16", 0x6A80|0xB, 0x0100);
    TEST("MOV   R3L, @H'FF12",    0x3000|(0xB<<8)|0x12);

    // MOV.W @abs16,Rd (0x6B, bit7=0): byte2=Rd3
    TEST("MOV.W @H'1234:16, R0", 0x6B00|0, 0x1234);
    TEST("MOV   @H'FFFE:16, R7", 0x6B00|7, 0xFFFE);
    TEST("MOV   @H'FE:8,    R7", 0x6B00|7, 0xFFFE);

    // MOV.W Rs,@abs16 (0x6B, bit7=1): byte2=0x80|Rs3
    TEST("MOV.W R0, @H'1234:16", 0x6B80|0, 0x1234);
    TEST("MOV   R5, @H'0200:16", 0x6B80|5, 0x0200);
    TEST("MOV   R5, @H'14:8",    0x6B80|5, 0xFF14);

    // MOV.B @Rn+,Rd (0x6C, bit7=0): byte2=(Rn3<<4)|Rd
    TEST("MOV.B @R0+, R0H", 0x6C00|(0<<4)|0x0);
    TEST("MOV   @R3+, R5L", 0x6C00|(3<<4)|0xD);

    // MOV.B Rs,@-Rn (0x6C, bit7=1): byte2=0x80|(Rn3<<4)|Rs
    TEST("MOV.B R0H, @-R0", 0x6C80|(0<<4)|0x0);
    TEST("MOV   R4L, @-R2", 0x6C80|(2<<4)|0xC);

    // MOV.W @Rn+,Rd (0x6D, bit7=0, Rn!=SP): byte2=(Rn3<<4)|Rd3
    TEST("MOV.W @R0+, R0", 0x6D00|(0<<4)|0);
    TEST("MOV   @R2+, R4", 0x6D00|(2<<4)|4);

    // POP Rd (0x6D, Rn=SP=R7, bit7=0): byte2=0x70|Rd3
    TEST("POP   R0", 0x6D70|0);
    TEST("POP.W R5", 0x6D70|5);
    ERRT("POP   R5L", OPERAND_NOT_ALLOWED, "R5L");

    // MOV.W Rs,@-Rn (0x6D, bit7=1, Rn!=SP): byte2=0x80|(Rn3<<4)|Rs3
    TEST("MOV.W R0, @-R0", 0x6D80|(0<<4)|0);
    TEST("MOV   R5, @-R3", 0x6D80|(3<<4)|5);
    // SP is a register-name alias for R7; @SP / @SP+ / @-SP must encode identically to @R7 forms.
    TEST("MOV.W R0, @SP",  0x6980|(7<<4)|0);
    TEST("MOV.W @SP+, R1", 0x6D00|(7<<4)|1);
    TEST("MOV   R5, @-SP", 0x6DF0|5);
    TEST("MOV.W @SP, R6",  0x6900|(7<<4)|6);

    // PUSH Rs (0x6D, Rn=SP=R7, bit7=1): byte2=0xF0|Rs3
    TEST("PUSH   R0", 0x6DF0|0);
    TEST("PUSH.W R6", 0x6DF0|6);
    ERRT("PUSH   R6H", OPERAND_NOT_ALLOWED, "R6H");

    // MOV.B @(d16,Rn),Rd (0x6E, bit7=0): byte2=(Rn3<<4)|Rd, then disp16
    TEST("MOV.B @(0, R0),  R0H", 0x6E00|(0<<4)|0x0, 0x0000);
    TEST("MOV   @(16, R2), R3L", 0x6E00|(2<<4)|0xB, 0x0010);
    TEST("MOV.B @(-1, R5), R7H", 0x6E00|(5<<4)|0x7, 0xFFFF);
    TEST("MOV   @(16:16, R5), R7H", 0x6E00|(5<<4)|0x7, 0x0010);
    ERRT("MOV   @(16:8, R5),  R7H", ILLEGAL_SIZE, ":8, R5),  R7H");

    // MOV.B Rs,@(d16,Rn) (0x6E, bit7=1): byte2=0x80|(Rn3<<4)|Rs
    TEST("MOV.B R0H, @(0, R0)", 0x6E80|(0<<4)|0x0, 0x0000);
    TEST("MOV   R4L, @(1, R3)", 0x6E80|(3<<4)|0xC, 0x0001);
    TEST("MOV   R4L, @(1, R3)", 0x6E80|(3<<4)|0xC, 0x0001);
    TEST("MOV.B R4L, @(1:16, R3)", 0x6E80|(3<<4)|0xC, 0x0001);
    ERRT("MOV.B R4L, @(1:8, R3)",  ILLEGAL_SIZE, ":8, R3)");

    // MOV.W @(d16,Rn),Rd (0x6F, bit7=0): byte2=(Rn3<<4)|Rd3
    TEST("MOV.W @(0, R0), R0",   0x6F00|(0<<4)|0, 0x0000);
    TEST("MOV   @(32, R1), R2",  0x6F00|(1<<4)|2, 0x0020);

    // MOV.W Rs,@(d16,Rn) (0x6F, bit7=1): byte2=0x80|(Rn3<<4)|Rs3
    TEST("MOV.W R0, @(0, R0)",  0x6F80|(0<<4)|0, 0x0000);
    TEST("MOV   R5, @(-2, R6)", 0x6F80|(6<<4)|5, 0xFFFE);

    // MOV.W #imm,Rd (0x79): byte2=Rd3, then imm16
    TEST("MOV.W #0, R0",      0x7900|0, 0x0000);
    TEST("MOV   #H'1234, R3", 0x7900|3, 0x1234);
    TEST("MOV.W #H'FF:8, R7", 0x7900|7, 0x00FF);
    TEST("MOV   #-1, R7",     0x7900|7, 0xFFFF);

    // MOV.B #imm,Rd (0xF0-0xFF): byte1=0xF0|(Rd nibble), byte2=imm8
    TEST("MOV.B #0, R0H",    0xF000|(0x0<<8)|0x00);
    TEST("MOV   #H'FF, R7L", 0xF000|(0xF<<8)|0xFF);
    ERRT("MOV.B #-1, R7L",   OPERAND_NOT_ALLOWED, "#-1, R7L");

    // MOVFPE @abs16,Rd (0x6A, bits[7:6]=01): byte2=0x40|Rd nibble (full 16 byte registers)
    TEST("MOVFPE   @H'1234:16, R0H", 0x6A40|0x0, 0x1234);
    TEST("MOVFPE.B @H'1234:16, R0H", 0x6A40|0x0, 0x1234);
    TEST("MOVFPE   @H'5678:16, R7L", 0x6A40|0xF, 0x5678);
    // MOVTPE Rs,@abs16 (0x6A, bits[7:6]=11): byte2=0xC0|Rs nibble (full 16 byte registers)
    TEST("MOVTPE   R0H, @H'1234:16", 0x6AC0|0x0, 0x1234);
    TEST("MOVTPE   R5L, @H'5678:16", 0x6AC0|0xD, 0x5678);
    TEST("MOVTPE.B R5L, @H'5678:16", 0x6AC0|0xD, 0x5678);
}

void test_arithmetic() {
    // ADD/SUB/CMP/ADDX/SUBX reg-reg: byte2=(Rs<<4)|Rd
    TEST("ADD.B  R0H, R0H", 0x0800|(0x0<<4)|0x0);
    TEST("ADD    R1H, R3L", 0x0800|(0x1<<4)|0xB);
    TEST("ADD.W  R0, R0",   0x0900|(0<<4)|0);
    TEST("ADD    R3, R5",   0x0900|(3<<4)|5);
    TEST("ADDX.B R0H, R1H", 0x0E00|(0x0<<4)|0x1);
    TEST("ADDX   R0H, R1H", 0x0E00|(0x0<<4)|0x1);

    TEST("SUB.B  R0H, R0H", 0x1800|(0x0<<4)|0x0);
    TEST("SUB    R5L, R2H", 0x1800|(0xD<<4)|0x2);
    TEST("SUB.W  R0, R0",   0x1900|(0<<4)|0);
    TEST("SUB    R4, R7",   0x1900|(4<<4)|7);
    TEST("SUBX.B R0H, R7L", 0x1E00|(0x0<<4)|0xF);
    TEST("SUBX   R0H, R7L", 0x1E00|(0x0<<4)|0xF);

    TEST("CMP.B  R0H, R0H", 0x1C00|(0x0<<4)|0x0);
    TEST("CMP    R2H, R6L", 0x1C00|(0x2<<4)|0xE);
    TEST("CMP.W  R0, R0",   0x1D00|(0<<4)|0);
    TEST("CMP    R1, R6",   0x1D00|(1<<4)|6);

    // INC/DEC Rd: byte2=Rd nibble
    TEST("INC   R0H", 0x0A00|0x0);
    TEST("INC.B R7L", 0x0A00|0xF);
    TEST("DEC   R0H", 0x1A00|0x0);
    TEST("DEC.B R4H", 0x1A00|0x4);

    // ADDS/SUBS #1/#2,Rd
    TEST("ADDS   #1, R0", 0x0B00|0);
    TEST("ADDS.W #1, R5", 0x0B00|5);
    TEST("ADDS   #2, R0", 0x0B80|0);
    TEST("ADDS.W #2, R7", 0x0B80|7);
    TEST("SUBS   #1, R0", 0x1B00|0);
    TEST("SUBS.W #1, R3", 0x1B00|3);
    TEST("SUBS   #2, R0", 0x1B80|0);
    TEST("SUBS.W #2, R7", 0x1B80|7);

    // DAA/DAS Rd
    TEST("DAA   R0H", 0x0F00|0x0);
    TEST("DAA.B R7L", 0x0F00|0xF);
    TEST("DAS   R0H", 0x1F00|0x0);
    TEST("DAS.B R7L", 0x1F00|0xF);

    // MULXU/DIVXU Rs8,Rd16: byte2=(Rs nibble<<4)|Rd3
    TEST("MULXU   R0H, R0", 0x5000|(0x0<<4)|0);
    TEST("MULXU.B R3H, R5", 0x5000|(0x3<<4)|5);
    TEST("DIVXU   R0H, R0", 0x5100|(0x0<<4)|0);
    TEST("DIVXU.B R7L, R2", 0x5100|(0xF<<4)|2);

    // NEG Rd (0x17, bit7=1): byte2=0x80|Rd nibble
    TEST("NEG   R0H", 0x1780|0x0);
    TEST("NEG.B R7L", 0x1780|0xF);

    // ADD/ADDX/CMP/SUBX #imm,Rd (0x80-0xBF): byte1=0x?0|(Rd nibble), byte2=imm8
    TEST("ADD.B  #0, R0H",    0x8000|(0x0<<8)|0x00);
    TEST("ADD    #H'FF, R7L", 0x8000|(0xF<<8)|0xFF);
    TEST("ADDX.B #1, R0H",    0x9000|(0x0<<8)|0x01);
    TEST("ADDX   #1, R0H",    0x9000|(0x0<<8)|0x01);
    TEST("CMP.B  #H'55, R5H", 0xA000|(0x5<<8)|0x55);
    TEST("CMP    #H'55, R5H", 0xA000|(0x5<<8)|0x55);
    TEST("SUBX.B #2, R2L",    0xB000|(0xA<<8)|0x02);
    TEST("SUBX   #2, R2L",    0xB000|(0xA<<8)|0x02);
}

void test_logic() {
    // OR/XOR/AND reg-reg: byte2=(Rs<<4)|Rd
    TEST("OR.B  R0H, R0H", 0x1400|(0x0<<4)|0x0);
    TEST("OR    R2H, R3L", 0x1400|(0x2<<4)|0xB);
    TEST("XOR.B R0H, R0H", 0x1500|(0x0<<4)|0x0);
    TEST("XOR   R1H, R5L", 0x1500|(0x1<<4)|0xD);
    TEST("AND.B R0H, R0H", 0x1600|(0x0<<4)|0x0);
    TEST("AND   R4L, R3H", 0x1600|(0xC<<4)|0x3);

    // NOT Rd (0x17, bit7=0): byte2=Rd nibble
    TEST("NOT   R0H", 0x1700|0x0);
    TEST("NOT.B R7L", 0x1700|0xF);

    // OR/XOR/AND #imm,Rd (0xC0-0xEF): byte1=0x?0|(Rd nibble), byte2=imm8
    TEST("OR.B  #H'FF, R7L", 0xC000|(0xF<<8)|0xFF);
    TEST("OR    #H'FF, R7L", 0xC000|(0xF<<8)|0xFF);
    TEST("XOR.B #H'AA, R5H", 0xD000|(0x5<<8)|0xAA);
    TEST("XOR   #H'AA, R5H", 0xD000|(0x5<<8)|0xAA);
    TEST("AND.B #H'F0, R0H", 0xE000|(0x0<<8)|0xF0);
    TEST("AND   #H'F0, R0H", 0xE000|(0x0<<8)|0xF0);
}

void test_shift_rotate() {
    // Shift/rotate Rd: bit7=0 logical/rotate-thru-X, bit7=1 arithmetic/rotate
    TEST("SHLL    R0H", 0x1000|0x0);
    TEST("SHLL.B  R7L", 0x1000|0xF);
    TEST("SHAL    R0H", 0x1080|0x0);
    TEST("SHAL.B  R5L", 0x1080|0xD);
    TEST("SHLR    R0H", 0x1100|0x0);
    TEST("SHLR.B  R7H", 0x1100|0x7);
    TEST("SHAR    R3L", 0x1180|0xB);
    TEST("SHAR.B  R3L", 0x1180|0xB);
    TEST("ROTXL   R0H", 0x1200|0x0);
    TEST("ROTXL.B R0H", 0x1200|0x0);
    TEST("ROTL    R0H", 0x1280|0x0);
    TEST("ROTL.B  R0H", 0x1280|0x0);
    TEST("ROTXR   R0H", 0x1300|0x0);
    TEST("ROTXR.B R0H", 0x1300|0x0);
    TEST("ROTR    R7L", 0x1380|0xF);
    TEST("ROTR.B  R7L", 0x1380|0xF);
}

void test_bit_ops() {
    // BSET/BNOT/BCLR/BTST Rs,Rd (0x60-0x63): byte2=(Rs<<4)|Rd
    TEST("BSET   R0H, R0H", 0x6000|(0x0<<4)|0x0);
    TEST("BSET.B R2H, R5L", 0x6000|(0x2<<4)|0xD);
    TEST("BNOT   R0H, R0H", 0x6100|(0x0<<4)|0x0);
    TEST("BNOT.B R3L, R1H", 0x6100|(0xB<<4)|0x1);
    TEST("BCLR   R0H, R0H", 0x6200|(0x0<<4)|0x0);
    TEST("BCLR.B R7L, R4H", 0x6200|(0xF<<4)|0x4);
    TEST("BTST   R0H, R0H", 0x6300|(0x0<<4)|0x0);
    TEST("BTST.B R5H, R2L", 0x6300|(0x5<<4)|0xA);

    // BSET/BNOT/BCLR/BTST #bit,Rd (0x70-0x73): byte2=(bit<<4)|Rd
    TEST("BSET   #0, R0H", 0x7000|(0<<4)|0x0);
    TEST("BSET.B #7, R7L", 0x7000|(7<<4)|0xF);
    TEST("BNOT   #3, R2H", 0x7100|(3<<4)|0x2);
    TEST("BNOT.B #3, R2H", 0x7100|(3<<4)|0x2);
    TEST("BCLR   #5, R4L", 0x7200|(5<<4)|0xC);
    TEST("BCLR.B #5, R4L", 0x7200|(5<<4)|0xC);
    TEST("BTST   #1, R0H", 0x7300|(1<<4)|0x0);
    TEST("BTST.B #1, R0H", 0x7300|(1<<4)|0x0);

    // BSET/BNOT/BCLR/BTST Rs,@Rd (0x60-0x63): byte2=(Rd<<4) byte4=(Rs<<4)
    TEST("BSET R0H, @R0", 0x7D00, 0x6000|(0x0<<4));
    TEST("BSET R2H, @R1", 0x7D10, 0x6000|(0x2<<4));
    TEST("BNOT R0H, @R2", 0x7D20, 0x6100|(0x0<<4));
    TEST("BNOT R3L, @R3", 0x7D30, 0x6100|(0xB<<4));
    TEST("BCLR R0H, @R4", 0x7D40, 0x6200|(0x0<<4));
    TEST("BCLR R7L, @R5", 0x7D50, 0x6200|(0xF<<4));
    TEST("BTST R0H, @R6", 0x7C60, 0x6300|(0x0<<4));
    TEST("BTST R5H, @R7", 0x7C70, 0x6300|(0x5<<4));

    // BSET/BNOT/BCLR/BTST #bit,@Rd (0x70-0x73): byte2=(Rd<<4) byte4=(bit<<4)
    TEST("BSET #0, @R7", 0x7D70, 0x7000|(0<<4));
    TEST("BSET #7, @R6", 0x7D60, 0x7000|(7<<4));
    TEST("BNOT #3, @R5", 0x7D50, 0x7100|(3<<4));
    TEST("BCLR #5, @R4", 0x7D40, 0x7200|(5<<4));
    TEST("BTST #1, @R3", 0x7C30, 0x7300|(1<<4));

    // BSET/BNOT/BCLR/BTST Rs,@aa:8 (0x60-0x63): byte2=aa:8 byte4=(Rs<<4)
    TEST("BSET R0H, @H'FF00:8", 0x7F00, 0x6000|(0x0<<4));
    TEST("BSET R2H, @H'FF10:8", 0x7F10, 0x6000|(0x2<<4));
    TEST("BNOT R0H, @H'FF20:8", 0x7F20, 0x6100|(0x0<<4));
    TEST("BNOT R3L, @H'FF30:8", 0x7F30, 0x6100|(0xB<<4));
    TEST("BCLR R0H, @H'FF40:8", 0x7F40, 0x6200|(0x0<<4));
    TEST("BCLR R7L, @H'FF50:8", 0x7F50, 0x6200|(0xF<<4));
    TEST("BTST R0H, @H'FF60:8", 0x7E60, 0x6300|(0x0<<4));
    TEST("BTST R5H, @H'FFFF:8", 0x7EFF, 0x6300|(0x5<<4));

    // BSET/BNOT/BCLR/BTST #bit,@aa:8 (0x70-0x73): byte2=aa:8 byte4=(bit<<4)
    TEST("BSET #0, @H'FF70:8", 0x7F70, 0x7000|(0<<4));
    TEST("BSET #7, @H'FF60:8", 0x7F60, 0x7000|(7<<4));
    TEST("BNOT #3, @H'FF50:8", 0x7F50, 0x7100|(3<<4));
    TEST("BCLR #5, @H'FF40:8", 0x7F40, 0x7200|(5<<4));
    TEST("BTST #1, @H'FF30:8", 0x7E30, 0x7300|(1<<4));

    // BST/BIST #bit,Rd (0x67)
    TEST("BST    #0, R0H", 0x6700|(0<<4)|0x0);
    TEST("BST.B  #7, R5L", 0x6700|(7<<4)|0xD);
    TEST("BIST   #0, R0H", 0x6780|(0<<4)|0x0);
    TEST("BIST.B #3, R6L", 0x6780|(3<<4)|0xE);

    // BOR/BIOR/BXOR/BIXOR/BAND/BIAND/BLD/BILD #bit,Rd (0x74-0x77)
    TEST("BOR     #0, R0H", 0x7400|(0<<4)|0x0);
    TEST("BOR.B   #5, R3H", 0x7400|(5<<4)|0x3);
    TEST("BIOR    #2, R1H", 0x7480|(2<<4)|0x1);
    TEST("BIOR.B  #2, R1H", 0x7480|(2<<4)|0x1);
    TEST("BXOR    #4, R0H", 0x7500|(4<<4)|0x0);
    TEST("BXOR.B  #4, R0H", 0x7500|(4<<4)|0x0);
    TEST("BIXOR   #1, R7L", 0x7580|(1<<4)|0xF);
    TEST("BIXOR.B #1, R7L", 0x7580|(1<<4)|0xF);
    TEST("BAND    #6, R2H", 0x7600|(6<<4)|0x2);
    TEST("BAND.B  #6, R2H", 0x7600|(6<<4)|0x2);
    TEST("BIAND   #3, R4L", 0x7680|(3<<4)|0xC);
    TEST("BIAND.B #3, R4L", 0x7680|(3<<4)|0xC);
    TEST("BLD     #7, R0H", 0x7700|(7<<4)|0x0);
    TEST("BLD.B   #7, R0H", 0x7700|(7<<4)|0x0);
    TEST("BILD    #0, R7L", 0x7780|(0<<4)|0xF);
    TEST("BILD.B  #0, R7L", 0x7780|(0<<4)|0xF);

    // BST/BIST #bit,@Rd (0x67): byte2=Rd byte4=(bit<<4)(bit7=0→BST, bit7=1→BIST)
    TEST("BST  #0, @R0", 0x7D00, 0x6700|(0<<4));
    TEST("BST  #7, @R5", 0x7D50, 0x6700|(7<<4));
    TEST("BIST #1, @R2", 0x7D20, 0x6780|(1<<4));
    TEST("BIST #3, @R6", 0x7D60, 0x6780|(3<<4));

    // BOR/BIOR/BXOR/BIXOR/BAND/BIAND/BLD/BILD #bit,@Rd (0x74-0x77):
    // byte2=Rd byte4=(bit<<4) (bit7=0→Bxx, bit7=1→BIxx)
    TEST("BOR   #0, @R0", 0x7C00, 0x7400|(0<<4));
    TEST("BOR   #5, @R3", 0x7C30, 0x7400|(5<<4));
    TEST("BIOR  #2, @R1", 0x7C10, 0x7480|(2<<4));
    TEST("BXOR  #4, @R0", 0x7C00, 0x7500|(4<<4));
    TEST("BIXOR #1, @R7", 0x7C70, 0x7580|(1<<4));
    TEST("BAND  #6, @R2", 0x7C20, 0x7600|(6<<4));
    TEST("BIAND #3, @R4", 0x7C40, 0x7680|(3<<4));
    TEST("BLD   #7, @R0", 0x7C00, 0x7700|(7<<4));
    TEST("BILD  #0, @R7", 0x7C70, 0x7780|(0<<4));

    // BST/BIST #bit,@aa:8 (0x67): byte2=aa:8 byte4=(bit<<4)(bit7=0→BST, bit7=1→BIST)
    TEST("BST  #0, @H'FF00:8", 0x7F00, 0x6700|(0<<4));
    TEST("BST  #7, @H'FF50:8", 0x7F50, 0x6700|(7<<4));
    TEST("BIST #1, @H'FF20:8", 0x7F20, 0x6780|(1<<4));
    TEST("BIST #3, @H'FF60:8", 0x7F60, 0x6780|(3<<4));

    // BOR/BIOR/BXOR/BIXOR/BAND/BIAND/BLD/BILD #bit,@aa:8 (0x74-0x77):
    // byte2=aa:8 byte4=(bit<<4) (bit7=0→Bxx, bit7=1→BIxx)
    TEST("BOR   #0, @H'FF00:8", 0x7E00, 0x7400|(0<<4));
    TEST("BOR   #5, @H'FF30:8", 0x7E30, 0x7400|(5<<4));
    TEST("BIOR  #2, @H'FF10:8", 0x7E10, 0x7480|(2<<4));
    TEST("BXOR  #4, @H'FF00:8", 0x7E00, 0x7500|(4<<4));
    TEST("BIXOR #1, @H'FF70:8", 0x7E70, 0x7580|(1<<4));
    TEST("BAND  #6, @H'FF20:8", 0x7E20, 0x7600|(6<<4));
    TEST("BIAND #3, @H'FF40:8", 0x7E40, 0x7680|(3<<4));
    TEST("BLD   #7, @H'FF00:8", 0x7E00, 0x7700|(7<<4));
    TEST("BILD  #0, @H'FF70:8", 0x7E70, 0x7780|(0<<4));

    ERRT("BSET #8, R0H", ILLEGAL_BIT_NUMBER, "#8, R0H", 0x7000|(0<<4)|0x0);
}

void test_branch() {
    // Bcc d:8 / BSR d:8 — byte1 = 0x4c (condition) or 0x55 (BSR), byte2 = signed disp.
    // target = origin + 2 + disp_signed. Range: -128..+127.
    // Vary the displacement across conditions and exercise the ±128 edges.

    // Trivial case: origin=0, disp=0 → target=0x0002.
    TEST("BRA H'0002", 0x4000|0x00);

    // ----- Edge: max positive +126 (disp = 0x7E) -----
    ATEST(0x1000, "BT  H'1080", 0x4000|0x7E);   // BT alias for BRA
    ATEST(0x1000, "BHS H'1080", 0x4400|0x7E);   // BHS alias for BCC
    ATEST(0x1000, "BVS H'1080", 0x4900|0x7E);
    ATEST(0x1000, "BLE H'1080", 0x4F00|0x7E);
    ATEST(0x1000, "BSR H'1080", 0x5500|0x7E);

    // ----- Edge: max negative -128 (disp = 0x80) -----
    ATEST(0x1000, "BF  H'0F82", 0x4100|0x80);   // BF alias for BRN
    ATEST(0x1000, "BLO H'0F82", 0x4500|0x80);   // BLO alias for BCS
    ATEST(0x1000, "BPL H'0F82", 0x4A00|0x80);
    ATEST(0x1000, "BSR H'0F82", 0x5500|0x80);

    // ----- Edge: branch to self start (disp = -2, byte = 0xFE) -----
    ATEST(0x1000, "BRN H'1000", 0x4100|0xFE);
    ATEST(0x1000, "BEQ H'1000", 0x4700|0xFE);

    // ----- Mid-range displacements (assorted) -----
    ATEST(0x1000, "BHI H'1004", 0x4200|0x02);
    ATEST(0x1000, "BLS H'0FFE", 0x4300|0xFC);
    ATEST(0x1000, "BCC H'107E", 0x4400|0x7C);
    ATEST(0x1000, "BCS H'1010", 0x4500|0x0E);
    ATEST(0x1000, "BNE H'1020", 0x4600|0x1E);
    ATEST(0x1000, "BVC H'1060", 0x4800|0x5E);
    ATEST(0x1000, "BMI H'0FF0", 0x4B00|0xEE);
    ATEST(0x1000, "BGE H'1012", 0x4C00|0x10);
    ATEST(0x1000, "BLT H'0FF4", 0x4D00|0xF2);
    ATEST(0x1000, "BGT H'0FFA", 0x4E00|0xF8);

    // ----- Out-of-range error: signed 8-bit disp must be in -128..+127 -----
    AERRT(0x1000, "BRA H'1082", OPERAND_TOO_FAR, "H'1082", 0x4000|0x80);
}

void test_jump() {
    // JMP @Rn (0x59): byte2=(Rn3<<4)
    TEST("JMP @R0", 0x5900|(0<<4));
    TEST("JMP @R5", 0x5900|(5<<4));
    TEST("JMP @R7", 0x5900|(7<<4));

    // JMP @abs16 (0x5A)
    TEST("JMP @H'1234:16", 0x5A00, 0x1234);
    TEST("JMP @H'0000:16", 0x5A00, 0x0000);

    // JMP @@abs8 (0x5B): byte2=abs8
    TEST("JMP @@H'00:8", 0x5B00|0x00);
    TEST("JMP @@H'FF:8", 0x5B00|0xFF);

    // JSR @Rn (0x5D): byte2=(Rn3<<4)
    TEST("JSR @R0", 0x5D00|(0<<4));
    TEST("JSR @R3", 0x5D00|(3<<4));

    // JSR @abs16 (0x5E)
    TEST("JSR @H'5678:16", 0x5E00, 0x5678);

    // JSR @@abs8 (0x5F): byte2=abs8
    TEST("JSR @@H'80:8", 0x5F00|0x80);
}

// clang-format on

void test_data_constant() {
    // .DATA defaults to word, big-endian; strings not allowed.
    TEST(".data H'1234, H'5678", 0x1234, 0x5678);
    TEST(".data.w H'8000",       0x8000);
    TEST(".data.b H'AB, H'CD",   0xABCD);
    TEST(".data.l H'12345678",   0x1234, 0x5678);

    // .SDATA emits raw string bytes.
    TEST(R"(.sdata "Hi")",       0x4869);
    TEST(R"(.sdata "")");

    // .RES reserves space without emitting bytes; output buffer is empty.
    TEST(".res 2");
    TEST(".res.b 3");
    TEST(".res.w 1");
    TEST(".res.l 1");
}

void run_tests(const char *cpu) {
    assembler.setCpu(cpu);
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
    RUN_TEST(test_data_constant);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

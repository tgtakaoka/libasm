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

#include "asm_c166.h"
#include "test_asm_helper.h"

using namespace libasm;
using namespace libasm::c166;
using namespace libasm::test;

AsmC166 asm166;
Assembler &assembler(asm166);

static bool c167() {
    return strcmp_P("80C167", assembler.config().cpu_P()) == 0;
}

void set_up() {
    assembler.reset();
}

void tear_down() {
    symtab.reset();
}

void test_cpu() {
    EQUALS("cpu 80C166", true, assembler.setCpu("80C166"));
    EQUALS_P("cpu 80C166", "80C166", assembler.config().cpu_P());

    EQUALS("cpu 80C167", true, assembler.setCpu("80C167"));
    EQUALS_P("cpu 80C167", "80C167", assembler.config().cpu_P());

    EQUALS("cpu 80c167cs", true, assembler.setCpu("80c167cs"));
    EQUALS_P("cpu 80c167cs", "80C167", assembler.config().cpu_P());
}

// clang-format off
void test_arithmetic() {
    TEST("ADD   R1, R2",            0x00, 0x12);
    TEST("ADD   R1, [R2]",          0x08, 0x1A);
    TEST("ADD   R1, [R3+]",         0x08, 0x1F);
    TEST("ADD   R1, #7",            0x08, 0x17);
    TEST("ADD   R1, #8",            0x06, 0xF1, 0x08, 0x00);
    TEST("ADD   0FE12H, #1234H",    0x06, 0x09, 0x34, 0x12);
    TEST("ADD   R4, #1234H",        0x06, 0xF4, 0x34, 0x12);
    TEST("ADD   0FE12H, 1234H",     0x02, 0x09, 0x34, 0x12);
    TEST("ADD   R1, 1234H",         0x02, 0xF1, 0x34, 0x12);
    TEST("ADD   1234H, 0FE12H",     0x04, 0x09, 0x34, 0x12);
    TEST("ADD   1234H, R1",         0x04, 0xF1, 0x34, 0x12);
    TEST("ADDB  RL1, RH2",          0x01, 0x25);
    TEST("ADDB  RH0, [R0]",         0x09, 0x18);
    TEST("ADDB  RL0, [R1+]",        0x09, 0x0D);
    TEST("ADDB  RH7, #5",           0x09, 0xF5);
    TEST("ADDB  0FE12H, #34H",      0x07, 0x09, 0x34, 0x00);
    TEST("ADDB  RL2, #34H",         0x07, 0xF4, 0x34, 0x00);
    TEST("ADDB  0FE12H, 1234H",     0x03, 0x09, 0x34, 0x12);
    TEST("ADDB  1234H, RH1",        0x05, 0xF3, 0x34, 0x12);
    TEST("ADDC  R1, R2",            0x10, 0x12);
    TEST("ADDC  R1, #0",            0x18, 0x10);
    TEST("ADDC  R1, [R2]",          0x18, 0x1A);
    TEST("ADDC  R1, [R3+]",         0x18, 0x1F);
    TEST("ADDC  0FE12H, #1234H",    0x16, 0x09, 0x34, 0x12);
    TEST("ADDC  0FE12H, 1234H",     0x12, 0x09, 0x34, 0x12);
    TEST("ADDC  1234H, 0FE12H",     0x14, 0x09, 0x34, 0x12);
    TEST("ADDCB RL1, [R3]",         0x19, 0x2B);
    TEST("ADDCB RL1, RH2",          0x11, 0x25);
    TEST("ADDCB RL0, [R1+]",        0x19, 0x0D);
    TEST("ADDCB RH7, #5",           0x19, 0xF5);
    TEST("ADDCB 0FE12H, #34H",      0x17, 0x09, 0x34, 0x00);
    TEST("ADDCB 0FE12H, 1234H",     0x13, 0x09, 0x34, 0x12);
    TEST("ADDCB 1234H, RH1",        0x15, 0xF3, 0x34, 0x12);
    TEST("SUB   R15, [R0+]",        0x28, 0xFC);
    TEST("SUB   1234H, R1",         0x24, 0xF1, 0x34, 0x12);
    TEST("SUB   R1, R2",            0x20, 0x12);
    TEST("SUB   R1, [R2]",          0x28, 0x1A);
    TEST("SUB   R1, #7",            0x28, 0x17);
    TEST("SUB   0FE12H, #1234H",    0x26, 0x09, 0x34, 0x12);
    TEST("SUB   0FE12H, 1234H",     0x22, 0x09, 0x34, 0x12);
    TEST("SUBB  0FE12H, #0FFH",     0x27, 0x09, 0xFF, 0x00);
    TEST("SUBB  RL1, RH2",          0x21, 0x25);
    TEST("SUBB  RH0, [R0]",         0x29, 0x18);
    TEST("SUBB  RL0, [R1+]",        0x29, 0x0D);
    TEST("SUBB  RH7, #5",           0x29, 0xF5);
    TEST("SUBB  0FE12H, 1234H",     0x23, 0x09, 0x34, 0x12);
    TEST("SUBB  1234H, RH1",        0x25, 0xF3, 0x34, 0x12);
    TEST("SUBC  0FFDEH, 1234H",     0x32, 0xEF, 0x34, 0x12);
    TEST("SUBC  R1, R2",            0x30, 0x12);
    TEST("SUBC  R1, [R2]",          0x38, 0x1A);
    TEST("SUBC  R1, [R3+]",         0x38, 0x1F);
    TEST("SUBC  R1, #7",            0x38, 0x17);
    TEST("SUBC  0FE12H, #1234H",    0x36, 0x09, 0x34, 0x12);
    TEST("SUBC  1234H, 0FE12H",     0x34, 0x09, 0x34, 0x12);
    TEST("SUBCB RH7, RL0",          0x31, 0xF0);
    TEST("SUBCB RH0, [R0]",         0x39, 0x18);
    TEST("SUBCB RL0, [R1+]",        0x39, 0x0D);
    TEST("SUBCB RH7, #5",           0x39, 0xF5);
    TEST("SUBCB 0FE12H, #34H",      0x37, 0x09, 0x34, 0x00);
    TEST("SUBCB 0FE12H, 1234H",     0x33, 0x09, 0x34, 0x12);
    TEST("SUBCB 1234H, RH1",        0x35, 0xF3, 0x34, 0x12);
    TEST("CMP   R1, #1234H",        0x46, 0xF1, 0x34, 0x12);
    TEST("CMP   0FE00H, 1234H",     0x42, 0x00, 0x34, 0x12);
    TEST("CMP   R1, R2",            0x40, 0x12);
    TEST("CMP   R1, [R2]",          0x48, 0x1A);
    TEST("CMP   R1, [R3+]",         0x48, 0x1F);
    TEST("CMP   R1, #7",            0x48, 0x17);
    TEST("CMPB  RL0, [R2]",         0x49, 0x0A);
    TEST("CMPB  0FE12H, #12H",      0x47, 0x09, 0x12, 0x00);
    TEST("CMPB  RL1, RH2",          0x41, 0x25);
    TEST("CMPB  RL0, [R1+]",        0x49, 0x0D);
    TEST("CMPB  RH7, #5",           0x49, 0xF5);
    TEST("CMPB  0FE12H, 1234H",     0x43, 0x09, 0x34, 0x12);
    TEST("XOR   R2, #3",            0x58, 0x23);
    TEST("XOR   R1, R2",            0x50, 0x12);
    TEST("XOR   R1, [R2]",          0x58, 0x1A);
    TEST("XOR   R1, [R3+]",         0x58, 0x1F);
    TEST("XOR   0FE12H, #1234H",    0x56, 0x09, 0x34, 0x12);
    TEST("XOR   0FE12H, 1234H",     0x52, 0x09, 0x34, 0x12);
    TEST("XOR   1234H, 0FE12H",     0x54, 0x09, 0x34, 0x12);
    TEST("XORB  1234H, 0FE12H",     0x55, 0x09, 0x34, 0x12);
    TEST("XORB  RL1, RH2",          0x51, 0x25);
    TEST("XORB  RH0, [R0]",         0x59, 0x18);
    TEST("XORB  RL0, [R1+]",        0x59, 0x0D);
    TEST("XORB  RH7, #5",           0x59, 0xF5);
    TEST("XORB  0FE12H, #34H",      0x57, 0x09, 0x34, 0x00);
    TEST("XORB  0FE12H, 1234H",     0x53, 0x09, 0x34, 0x12);
    TEST("AND   R3, [R1+]",         0x68, 0x3D);
    TEST("AND   R1, R2",            0x60, 0x12);
    TEST("AND   R1, [R2]",          0x68, 0x1A);
    TEST("AND   R1, #7",            0x68, 0x17);
    TEST("AND   0FE12H, #1234H",    0x66, 0x09, 0x34, 0x12);
    TEST("AND   0FE12H, 1234H",     0x62, 0x09, 0x34, 0x12);
    TEST("AND   1234H, 0FE12H",     0x64, 0x09, 0x34, 0x12);
    TEST("ANDB  RL3, RH3",          0x61, 0x67);
    TEST("ANDB  RH0, [R0]",         0x69, 0x18);
    TEST("ANDB  RL0, [R1+]",        0x69, 0x0D);
    TEST("ANDB  RH7, #5",           0x69, 0xF5);
    TEST("ANDB  0FE12H, #34H",      0x67, 0x09, 0x34, 0x00);
    TEST("ANDB  0FE12H, 1234H",     0x63, 0x09, 0x34, 0x12);
    TEST("ANDB  1234H, RH1",        0x65, 0xF3, 0x34, 0x12);
    TEST("OR    0FE12H, #0FFFFH",   0x76, 0x09, 0xFF, 0xFF);
    TEST("OR    R1, R2",            0x70, 0x12);
    TEST("OR    R1, [R2]",          0x78, 0x1A);
    TEST("OR    R1, [R3+]",         0x78, 0x1F);
    TEST("OR    R1, #7",            0x78, 0x17);
    TEST("OR    0FE12H, 1234H",     0x72, 0x09, 0x34, 0x12);
    TEST("OR    1234H, 0FE12H",     0x74, 0x09, 0x34, 0x12);
    TEST("ORB   RH1, #7",           0x79, 0x37);
    TEST("ORB   RL1, RH2",          0x71, 0x25);
    TEST("ORB   RH0, [R0]",         0x79, 0x18);
    TEST("ORB   RL0, [R1+]",        0x79, 0x0D);
    TEST("ORB   0FE12H, #34H",      0x77, 0x09, 0x34, 0x00);
    TEST("ORB   0FE12H, 1234H",     0x73, 0x09, 0x34, 0x12);
    TEST("ORB   1234H, RH1",        0x75, 0xF3, 0x34, 0x12);
}

void test_multiply() {
    TEST("MUL   R1, R2", 0x0B, 0x12);
    TEST("MULU  R3, R4", 0x1B, 0x34);
    TEST("PRIOR R5, R6", 0x2B, 0x56);
    TEST("DIV   R3",     0x4B, 0x33);
    TEST("DIVU  R4",     0x5B, 0x44);
    TEST("DIVL  R5",     0x6B, 0x55);
    TEST("DIVLU R15",    0x7B, 0xFF);
}

void test_shift() {
    TEST("ROL  R1, R2",   0x0C, 0x12);
    TEST("ROL  R1, #15",  0x1C, 0xF1);
    TEST("ROR  R3, R4",   0x2C, 0x34);
    TEST("ROR  R3, #1",   0x3C, 0x13);
    TEST("SHL  R5, R6",   0x4C, 0x56);
    TEST("SHL  R5, #0",   0x5C, 0x05);
    TEST("SHR  R7, R8",   0x6C, 0x78);
    TEST("SHR  R7, #8",   0x7C, 0x87);
    TEST("ASHR R9, R10",  0xAC, 0x9A);
    TEST("ASHR R9, #10",  0xBC, 0xA9);
}

void test_compare() {
    TEST("CMPI1 R1, #3",       0x80, 0x31);
    TEST("CMPI1 R2, #1234H",   0x86, 0xF2, 0x34, 0x12);
    TEST("CMPI1 R2, #16",      0x86, 0xF2, 0x10, 0x00);
    TEST("CMPI1 R3, 1234H",    0x82, 0xF3, 0x34, 0x12);
    TEST("CMPI2 R4, #15",      0x90, 0xF4);
    TEST("CMPI2 R5, #1234H",   0x96, 0xF5, 0x34, 0x12);
    TEST("CMPI2 R6, 1234H",    0x92, 0xF6, 0x34, 0x12);
    TEST("CMPD1 R7, #0",       0xA0, 0x07);
    TEST("CMPD1 R8, #1234H",   0xA6, 0xF8, 0x34, 0x12);
    TEST("CMPD1 R9, 1234H",    0xA2, 0xF9, 0x34, 0x12);
    TEST("CMPD2 R10, #1",      0xB0, 0x1A);
    TEST("CMPD2 R11, #1234H",  0xB6, 0xFB, 0x34, 0x12);
    TEST("CMPD2 R12, 1234H",   0xB2, 0xFC, 0x34, 0x12);
}

void test_unary() {
    TEST("NEG  R1",  0x81, 0x10);
    TEST("CPL  R2",  0x91, 0x20);
    TEST("NEGB RH0", 0xA1, 0x10);
    TEST("CPLB RL1", 0xB1, 0x20);
}

void test_move() {
    TEST("MOV   R1, R2",            0xF0, 0x12);
    TEST("MOV   R1, #5",            0xE0, 0x51);
    TEST("MOV   R1, #16",           0xE6, 0xF1, 0x10, 0x00);
    TEST("MOV   0FE12H, #1234H",    0xE6, 0x09, 0x34, 0x12);
    TEST("MOV   R1, [R2]",          0xA8, 0x12);
    TEST("MOV   R1, [R2+]",         0x98, 0x12);
    TEST("MOV   [R2], R1",          0xB8, 0x12);
    TEST("MOV   [-R2], R1",         0x88, 0x12);
    TEST("MOV   [R1], [R2]",        0xC8, 0x12);
    TEST("MOV   [R1+], [R2]",       0xD8, 0x12);
    TEST("MOV   [R1], [R2+]",       0xE8, 0x12);
    TEST("MOV   R1, [R2+#1234H]",   0xD4, 0x12, 0x34, 0x12);
    TEST("MOV   [R2+#1234H], R1",   0xC4, 0x12, 0x34, 0x12);
    TEST("MOV   R1, [R2+1234H]",    0xD4, 0x12, 0x34, 0x12);
    TEST("MOV   [R2 + #-2], R1",    0xC4, 0x12, 0xFE, 0xFF);
    TEST("MOV   [R1], 1234H",       0x84, 0x01, 0x34, 0x12);
    TEST("MOV   1234H, [R1]",       0x94, 0x01, 0x34, 0x12);
    TEST("MOV   0FE12H, 1234H",     0xF2, 0x09, 0x34, 0x12);
    TEST("MOV   1234H, 0FE12H",     0xF6, 0x09, 0x34, 0x12);
    TEST("MOV   0FE12H, 0FE14H",    0xF2, 0x09, 0x14, 0xFE);
    TEST("MOVB  RL1, RH2",          0xF1, 0x25);
    TEST("MOVB  RH0, #5",           0xE1, 0x51);
    TEST("MOVB  0FE12H, #34H",      0xE7, 0x09, 0x34, 0x00);
    TEST("MOVB  RH0, [R2]",         0xA9, 0x12);
    TEST("MOVB  RH0, [R2+]",        0x99, 0x12);
    TEST("MOVB  [R2], RH0",         0xB9, 0x12);
    TEST("MOVB  [-R2], RH0",        0x89, 0x12);
    TEST("MOVB  [R1], [R2]",        0xC9, 0x12);
    TEST("MOVB  [R1+], [R2]",       0xD9, 0x12);
    TEST("MOVB  [R1], [R2+]",       0xE9, 0x12);
    TEST("MOVB  RH0, [R2+#1234H]",  0xF4, 0x12, 0x34, 0x12);
    TEST("MOVB  [R2+#1234H], RH0",  0xE4, 0x12, 0x34, 0x12);
    TEST("MOVB  [R1], 1234H",       0xA4, 0x01, 0x34, 0x12);
    TEST("MOVB  1234H, [R1]",       0xB4, 0x01, 0x34, 0x12);
    TEST("MOVB  0FE12H, 1234H",     0xF3, 0x09, 0x34, 0x12);
    TEST("MOVB  1234H, 0FE12H",     0xF7, 0x09, 0x34, 0x12);
    TEST("MOVBZ R1, RH1",           0xC0, 0x31);
    TEST("MOVBZ 0FE12H, 1234H",     0xC2, 0x09, 0x34, 0x12);
    TEST("MOVBZ 1234H, RL0",        0xC5, 0xF0, 0x34, 0x12);
    TEST("MOVBS R2, RL1",           0xD0, 0x22);
    TEST("MOVBS R1, 1234H",         0xD2, 0xF1, 0x34, 0x12);
    TEST("MOVBS 1234H, 0FE12H",     0xD5, 0x09, 0x34, 0x12);
    TEST("SCXT  0FE12H, #1234H",    0xC6, 0x09, 0x34, 0x12);
    TEST("SCXT  R1, 1234H",         0xD6, 0xF1, 0x34, 0x12);
    TEST("PUSH  0FE12H",            0xEC, 0x09);
    TEST("POP   R1",                0xFC, 0xF1);
    ERRT("PUSH  RL1",               OPERAND_NOT_ALLOWED, "RL1");
}

void test_bit() {
    TEST("BCLR  0FD02H.3",             0x3E, 0x01);
    TEST("BSET  R1.15",                0xFF, 0xF1);
    TEST("BSET  0FF02H.0",             0x0F, 0x81);
    TEST("BCLR  0FFDEH.9",             0x9E, 0xEF);
    TEST("BFLDL 0FD02H, #12H, #34H",   0x0A, 0x01, 0x12, 0x34);
    TEST("BFLDH 0FD02H, #12H, #34H",   0x1A, 0x01, 0x34, 0x12);
    TEST("BFLDL R2, #0FFH, #0",        0x0A, 0xF2, 0xFF, 0x00);
    TEST("BCMP  0FD02H.3, 0FD04H.5",   0x2A, 0x02, 0x01, 0x53);
    TEST("BMOVN R1.0, 0FF00H.15",      0x3A, 0x80, 0xF1, 0xF0);
    TEST("BMOV  0FD02H.3, 0FD04H.5",   0x4A, 0x02, 0x01, 0x53);
    TEST("BOR   0FD02H.3, 0FD04H.5",   0x5A, 0x02, 0x01, 0x53);
    TEST("BAND  0FD02H.3, 0FD04H.5",   0x6A, 0x02, 0x01, 0x53);
    TEST("BXOR  0FD02H.3, 0FD04H.5",   0x7A, 0x02, 0x01, 0x53);
}

void test_jump() {
    ATEST(0x1000,  "JMPR  CC_UC, 100CH",          0x0D, 0x05);
    ATEST(0x1000,  "JMPR  cc_Z, 0FFEH",           0x2D, 0xFE);
    ATEST(0x1000,  "JMPR  EQ, 0FFEH",             0x2D, 0xFE);
    ATEST(0x1000,  "JMPR  CC_ULE, 1002H",         0xFD, 0x00);
    ATEST(0x1000,  "JMPR  UGE, 1002H",            0x9D, 0x00);
    ATEST(0x1000,  "JMPR  ULT, 1002H",            0x8D, 0x00);
    ATEST(0x1000,  "JMPR  NE, 1002H",             0x3D, 0x00);
    ATEST(0x1FFFE, "JMPR  CC_NET, 10002H",        0x1D, 0x01);
    ATEST(0x10000, "JMPR  CC_SGT, 1FF04H",        0xAD, 0x81);
    ATEST(0x1000,  "CALLR 1100H",                 0xBB, 0x7F);
    ATEST(0x1000,  "JB    0FD02H.3, 1024H",       0x8A, 0x01, 0x10, 0x30);
    ATEST(0x1000,  "JNB   R1.0, 0F84H",           0x9A, 0xF1, 0xC0, 0x00);
    ATEST(0x1000,  "JBC   0FF02H.15, 1004H",      0xAA, 0x81, 0x00, 0xF0);
    ATEST(0x1000,  "JNBS  0FD00H.1, 1006H",       0xBA, 0x00, 0x01, 0x10);
    ATEST(0x21000, "JMPA  CC_NZ, 21234H",         0xEA, 0x30, 0x34, 0x12);
    ATEST(0x1000,  "CALLA CC_UC, 0FFFEH",         0xCA, 0x00, 0xFE, 0xFF);
    ATEST(0x1000,  "PCALL 0FE12H, 1234H",         0xE2, 0x09, 0x34, 0x12);
    TEST("JMPI  CC_UC, [R1]",       0x9C, 0x01);
    TEST("JMPI  CC_NET, [R1]",      0x9C, 0x11);
    TEST("JMPI  CC_Z, [R1]",        0x9C, 0x21);
    TEST("JMPI  CC_NZ, [R1]",       0x9C, 0x31);
    TEST("JMPI  CC_V, [R1]",        0x9C, 0x41);
    TEST("JMPI  CC_NV, [R1]",       0x9C, 0x51);
    TEST("JMPI  CC_N, [R1]",        0x9C, 0x61);
    TEST("JMPI  CC_NN, [R1]",       0x9C, 0x71);
    TEST("JMPI  CC_C, [R1]",        0x9C, 0x81);
    TEST("JMPI  CC_NC, [R1]",       0x9C, 0x91);
    TEST("JMPI  CC_SGT, [R1]",      0x9C, 0xA1);
    TEST("JMPI  CC_SLE, [R1]",      0x9C, 0xB1);
    TEST("JMPI  CC_SLT, [R1]",      0x9C, 0xC1);
    TEST("JMPI  CC_SGE, [R1]",      0x9C, 0xD1);
    TEST("JMPI  CC_UGT, [R1]",      0x9C, 0xE1);
    TEST("JMPI  CC_ULE, [R1]",      0x9C, 0xF1);
    TEST("CALLI CC_Z, [R15]",       0xAB, 0x2F);
    TEST("JMPS  1, 2345H",          0xFA, 0x01, 0x45, 0x23);
    TEST("CALLS 3, 0FFFEH",         0xDA, 0x03, 0xFE, 0xFF);
    TEST("TRAP  #7FH",              0x9B, 0xFE);
    TEST("TRAP  #0",                0x9B, 0x00);
    if (c167()) {
        TEST("JMPS  0FFH, 2345H",   0xFA, 0xFF, 0x45, 0x23);
        ATEST(0xFF0000, "JMPA CC_C, 0FF1234H", 0xEA, 0x80, 0x34, 0x12);
    } else {
        ERRT("JMPS  4, 2345H", OVERFLOW_RANGE, "4, 2345H", 0xFA, 0x04, 0x45, 0x23);
    }

    symtab.intern(0x1010, "label");
    ATEST(0x1000, "JMPR CC_UC, label",          0x0D, 0x07);
    ATEST(0x1000, "JB   R2.1, label",           0x8A, 0xF2, 0x06, 0x10);
    ATEST(0x1000, "JMPA CC_UC, label",          0xEA, 0x00, 0x10, 0x10);

    ATEST(0x1000, "JMPR  CC_UC, $",             0x0D, 0xFF);
    ATEST(0x1000, "JMPR  CC_Z, $+4",            0x2D, 0x01);
    ATEST(0x1000, "CALLR $-2",                  0xBB, 0xFE);
    ATEST(0x1000, "JB    R3.4, $",              0x8A, 0xF3, 0xFE, 0x40);
    ATEST(0x21000, "JMPA CC_UC, $",             0xEA, 0x00, 0x00, 0x10);
    ATEST(0x1000, "CALLA CC_NZ, $+10H",         0xCA, 0x30, 0x10, 0x10);
}

void test_return() {
    TEST("RET",         0xCB, 0x00);
    TEST("RETS",        0xDB, 0x00);
    TEST("RETI",        0xFB, 0x88);
    TEST("RETP 0FE12H", 0xEB, 0x09);
    TEST("RETP R5",     0xEB, 0xF5);
}

void test_system() {
    TEST("NOP",    0xCC, 0x00);
    TEST("IDLE",   0x87, 0x78, 0x87, 0x87);
    TEST("PWRDN",  0x97, 0x68, 0x97, 0x97);
    TEST("SRVWDT", 0xA7, 0x58, 0xA7, 0xA7);
    TEST("SRST",   0xB7, 0x48, 0xB7, 0xB7);
    TEST("DISWDT", 0xA5, 0x5A, 0xA5, 0xA5);
    TEST("EINIT",  0xB5, 0x4A, 0xB5, 0xB5);
}

// A core SFR name stands for its address, wherever an address may be.
void test_sfr_name() {
    TEST("ADD  SP, #1234H",     0x06, 0x09, 0x34, 0x12);
    TEST("ADD  sp, 1234H",      0x02, 0x09, 0x34, 0x12);
    TEST("MOV  R1, SP",         0xF2, 0xF1, 0x12, 0xFE);
    TEST("MOV  DPP0, ONES",     0xF2, 0x00, 0x1E, 0xFF);
    TEST("BSET PSW.3",          0x3F, 0x88);
    TEST("BSET ZEROS.15",       0xFF, 0x8E);
    TEST("PUSH CP",             0xEC, 0x08);
    TEST("BSET TFR.3",          0x3F, 0xD6);
}

// GNU as writes the location counter as '.', and data as .byte and .word.
void test_gnu_syntax() {
    ATEST(0x1000, "JMPR CC_UC, .+4",  0x0D, 0x01);
    ATEST(0x1000, "JMPR CC_UC, $+4",  0x0D, 0x01);
    TEST(".byte 1, 0xFF",       0x01, 0xFF);
    TEST(".word 0x1234, -1",    0x34, 0x12, 0xFF, 0xFF);
    ERRT(".byte 'AB'",          ILLEGAL_CONSTANT, "'AB'");
}

void test_data_constant() {
    TEST("DB -128, 255",     0x80, 0xFF);
    TEST(R"(DB 'A', '"')",   0x41, 0x22);
    TEST("DB 'A''B',0",      0x41, 0x27, 0x42, 0x00);
    ERRT("DB 'A''B,0",       MISSING_CLOSING_QUOTE, "'A''B,0");
    TEST("DW -128, 255",     0x80, 0xFF, 0xFF, 0x00);
    TEST("DW 1234H",         0x34, 0x12);
    // A letter is a value; DW takes no string.
    TEST("DW 'A', 'A'+1",    0x41, 0x00, 0x42, 0x00);
    TEST(R"(DW "\n")",       0x0A, 0x00);
    ERRT("DW 'AB'",          ILLEGAL_CONSTANT, "'AB'");
    TEST(R"(DB "A", "'")",          0x41, 0x27);
    TEST(R"(DB "It's", 'a"b')",     0x49, 0x74, 0x27, 0x73, 0x61, 0x22, 0x62);
    TEST(R"(DB "say ""hi""")",      0x73, 0x61, 0x79, 0x20, 0x22, 0x68, 0x69, 0x22);
    // A backslash begins an escape sequence between double quotes only.
    TEST(R"(DB '\', "\\")",         0x5C, 0x5C);
    TEST(R"(DB "\a\b\f\n\r\t\v")",  0x07, 0x08, 0x0C, 0x0A, 0x0D, 0x09, 0x0B);
    TEST(R"(DB "\?\'\"\x41\101")",  0x3F, 0x27, 0x22, 0x41, 0x41);
    ERRT(R"(DB "\q")",              UNKNOWN_ESCAPE_SEQUENCE, R"(\q")");
    ERRT(R"(DB "abc)",              MISSING_CLOSING_DQUOTE, R"("abc)");
    TEST(R"(MOV R0, #"A"+1)",       0xE6, 0xF0, 0x42, 0x00);
    ERUS("DB 1, UNDEF, 2",   "UNDEF, 2", 0x01, 0x00, 0x02);
    ERUS("DW 1, UNDEF, 2",   "UNDEF, 2", 0x01, 0x00, 0x00, 0x00, 0x02, 0x00);
}

void test_undefined() {
    // A forward reference takes the long form.
    ERUS("ADD  R1, #UNDEF",       "UNDEF",       0x06, 0xF1, 0x00, 0x00);
    ERUS("MOV  R1, #UNDEF",       "UNDEF",       0xE6, 0xF1, 0x00, 0x00);
    ERUS("CMPI1 R1, #UNDEF",      "UNDEF",       0x86, 0xF1, 0x00, 0x00);
    ERUS("MOV  UNDEF, 1234H",     "UNDEF, 1234H", 0xF2, 0x00, 0x34, 0x12);
    ERUS("PUSH UNDEF",            "UNDEF",       0xEC, 0x00);
    AERRT(0x1000, "JMPR CC_UC, UNDEF", UNDEFINED_SYMBOL, "UNDEF", 0x0D, 0x00);
    AERRT(0x1000, "JMPA CC_UC, UNDEF", UNDEFINED_SYMBOL, "UNDEF", 0xEA, 0x00, 0x00, 0x00);
}

void test_errors() {
    ERUI("NOSUCH R1");
    ERRT("ADD  R1, [R4]",        OPERAND_NOT_ALLOWED, "R1, [R4]");
    ERRT("ADD  R1, [R4+]",       OPERAND_NOT_ALLOWED, "R1, [R4+]");
    ERRT("MOV  R16, R1",         UNDEFINED_SYMBOL, "R16, R1", 0xF6, 0xF1, 0x00, 0x00);
    ERRT("MOV  [RL1], R1",       UNKNOWN_OPERAND, "RL1], R1");
    ERRT("MOV  [R1, R2",         MISSING_CLOSING_BRACKET, ", R2");
    ERRT("BSET 0FD02H.16",       ILLEGAL_BIT_NUMBER, "16");
    ERRT("BSET 0FD03H.1",        NOT_BIT_ADDRESSABLE, "0FD03H.1", 0x1F, 0x00);
    ERRT("BSET 0FE00H.1",        NOT_BIT_ADDRESSABLE, "0FE00H.1", 0x1F, 0x00);
    ERRT("PUSH 0FE13H",          OPERAND_NOT_ALLOWED, "0FE13H");
    ERRT("PUSH 0FFE0H",          OPERAND_NOT_ALLOWED, "0FFE0H");
    ERRT("MOV  10000H, R1",      OVERFLOW_RANGE, "10000H, R1", 0xF6, 0xF1, 0x00, 0x00);
    ERRT("MOV  0FE12H, #10000H", OVERFLOW_RANGE, "#10000H", 0xE6, 0x09, 0x00, 0x00);
    ERRT("MOVB 0FE12H, #100H",   OVERFLOW_RANGE, "#100H", 0xE7, 0x09, 0x00, 0x00);
    ERRT("TRAP #80H",            OVERFLOW_RANGE, "#80H", 0x9B, 0x00);
    ERRT("BFLDL 0FD02H, #100H, #34H", OVERFLOW_RANGE, "#100H, #34H", 0x0A, 0x01, 0x00, 0x34);
    ERRT("ROL  R1, #16",         OPERAND_NOT_ALLOWED, "R1, #16");
    AERRT(0x1000, "JMPR CC_UC, 1101H", OPERAND_NOT_ALIGNED, "1101H", 0x0D, 0x7F);
    AERRT(0x1000, "JMPR CC_UC, 1102H", OPERAND_TOO_FAR, "1102H", 0x0D, 0x80);
    AERRT(0x1000, "JMPR CC_UC, 0F00H", OPERAND_TOO_FAR, "0F00H", 0x0D, 0x7F);
    AERRT(0x1000, "JMPR CC_UC, 10000H", OPERAND_TOO_FAR, "10000H", 0x0D, 0xFF);
    AERRT(0x1000, "JMPA CC_UC, 11234H", OPERAND_TOO_FAR, "11234H", 0xEA, 0x00, 0x34, 0x12);
    ERRT("JMPS 1, 10000H",       OVERFLOW_RANGE, "10000H", 0xFA, 0x01, 0x00, 0x00);
}
// clang-format on

void run_tests(const char *cpu) {
    assembler.setCpu(cpu);
    RUN_TEST(test_arithmetic);
    RUN_TEST(test_multiply);
    RUN_TEST(test_shift);
    RUN_TEST(test_compare);
    RUN_TEST(test_unary);
    RUN_TEST(test_move);
    RUN_TEST(test_bit);
    RUN_TEST(test_jump);
    RUN_TEST(test_return);
    RUN_TEST(test_system);
    RUN_TEST(test_sfr_name);
    RUN_TEST(test_gnu_syntax);
    RUN_TEST(test_data_constant);
    RUN_TEST(test_undefined);
    RUN_TEST(test_errors);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

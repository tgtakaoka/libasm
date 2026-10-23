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

#include "dis_c166.h"
#include "test_dis_helper.h"

using namespace libasm;
using namespace libasm::c166;
using namespace libasm::test;

DisC166 dis166;
Disassembler &disassembler(dis166);

void set_up() {
    disassembler.reset();
}

void tear_down() {
    symtab.reset();
}

void test_cpu() {
    EQUALS("cpu 80C166", true, disassembler.setCpu("80C166"));
    EQUALS_P("cpu 80C166", "80C166", disassembler.config().cpu_P());
}

// clang-format off
void test_arithmetic() {
    TEST("ADD",   "R1, R2",          0x00, 0x12);
    TEST("ADD",   "R1, [R2]",        0x08, 0x1A);
    TEST("ADD",   "R1, [R3+]",       0x08, 0x1F);
    TEST("ADD",   "R1, #7",          0x08, 0x17);
    TEST("ADD",   "0FE12H, #1234H",  0x06, 0x09, 0x34, 0x12);
    TEST("ADD",   "R4, #1234H",      0x06, 0xF4, 0x34, 0x12);
    TEST("ADD",   "0FE12H, 1234H",   0x02, 0x09, 0x34, 0x12);
    TEST("ADD",   "1234H, 0FE12H",   0x04, 0x09, 0x34, 0x12);
    TEST("ADDB",  "RL1, RH2",        0x01, 0x25);
    TEST("ADDB",  "RH0, [R0]",       0x09, 0x18);
    TEST("ADDB",  "RL0, [R1+]",      0x09, 0x0D);
    TEST("ADDB",  "RH7, #5",         0x09, 0xF5);
    TEST("ADDB",  "0FE12H, #34H",    0x07, 0x09, 0x34, 0x00);
    TEST("ADDB",  "RL2, #34H",       0x07, 0xF4, 0x34, 0x00);
    TEST("ADDB",  "0FE12H, 1234H",   0x03, 0x09, 0x34, 0x12);
    TEST("ADDB",  "1234H, RH1",      0x05, 0xF3, 0x34, 0x12);
    ERRT("ADDB",  "0FE12H, #34H", ILLEGAL_OPERAND_MODE, "#34H", 0x07, 0x09, 0x34, 0x12);

    TEST("ADDC",  "R1, R2",          0x10, 0x12);
    TEST("ADDC",  "R1, #0",          0x18, 0x10);
    TEST("ADDC",  "R1, [R2]",        0x18, 0x1A);
    TEST("ADDC",  "R1, [R3+]",       0x18, 0x1F);
    TEST("ADDC",  "0FE12H, #1234H",  0x16, 0x09, 0x34, 0x12);
    TEST("ADDC",  "0FE12H, 1234H",   0x12, 0x09, 0x34, 0x12);
    TEST("ADDC",  "1234H, 0FE12H",   0x14, 0x09, 0x34, 0x12);
    TEST("ADDCB", "RL1, [R3]",       0x19, 0x2B);
    TEST("ADDCB", "RL1, RH2",        0x11, 0x25);
    TEST("ADDCB", "RL0, [R1+]",      0x19, 0x0D);
    TEST("ADDCB", "RH7, #5",         0x19, 0xF5);
    TEST("ADDCB", "0FE12H, #34H",    0x17, 0x09, 0x34, 0x00);
    TEST("ADDCB", "0FE12H, 1234H",   0x13, 0x09, 0x34, 0x12);
    TEST("ADDCB", "1234H, RH1",      0x15, 0xF3, 0x34, 0x12);
    TEST("SUB",   "R15, [R0+]",      0x28, 0xFC);
    TEST("SUB",   "1234H, R1",       0x24, 0xF1, 0x34, 0x12);
    TEST("SUB",   "R1, R2",          0x20, 0x12);
    TEST("SUB",   "R1, [R2]",        0x28, 0x1A);
    TEST("SUB",   "R1, #7",          0x28, 0x17);
    TEST("SUB",   "0FE12H, #1234H",  0x26, 0x09, 0x34, 0x12);
    TEST("SUB",   "0FE12H, 1234H",   0x22, 0x09, 0x34, 0x12);
    TEST("SUBB",  "0FE12H, #0FFH",   0x27, 0x09, 0xFF, 0x00);
    TEST("SUBB",  "RL1, RH2",        0x21, 0x25);
    TEST("SUBB",  "RH0, [R0]",       0x29, 0x18);
    TEST("SUBB",  "RL0, [R1+]",      0x29, 0x0D);
    TEST("SUBB",  "RH7, #5",         0x29, 0xF5);
    TEST("SUBB",  "0FE12H, 1234H",   0x23, 0x09, 0x34, 0x12);
    TEST("SUBB",  "1234H, RH1",      0x25, 0xF3, 0x34, 0x12);
    TEST("SUBC",  "0FFDEH, 1234H",   0x32, 0xEF, 0x34, 0x12);
    TEST("SUBC",  "R1, R2",          0x30, 0x12);
    TEST("SUBC",  "R1, [R2]",        0x38, 0x1A);
    TEST("SUBC",  "R1, [R3+]",       0x38, 0x1F);
    TEST("SUBC",  "R1, #7",          0x38, 0x17);
    TEST("SUBC",  "0FE12H, #1234H",  0x36, 0x09, 0x34, 0x12);
    TEST("SUBC",  "1234H, 0FE12H",   0x34, 0x09, 0x34, 0x12);
    TEST("SUBCB", "RH7, RL0",        0x31, 0xF0);
    TEST("SUBCB", "RH0, [R0]",       0x39, 0x18);
    TEST("SUBCB", "RL0, [R1+]",      0x39, 0x0D);
    TEST("SUBCB", "RH7, #5",         0x39, 0xF5);
    TEST("SUBCB", "0FE12H, #34H",    0x37, 0x09, 0x34, 0x00);
    TEST("SUBCB", "0FE12H, 1234H",   0x33, 0x09, 0x34, 0x12);
    TEST("SUBCB", "1234H, RH1",      0x35, 0xF3, 0x34, 0x12);
    TEST("CMP",   "R1, #1234H",      0x46, 0xF1, 0x34, 0x12);
    TEST("CMP",   "0FE00H, 1234H",   0x42, 0x00, 0x34, 0x12);
    TEST("CMP",   "R1, R2",          0x40, 0x12);
    TEST("CMP",   "R1, [R2]",        0x48, 0x1A);
    TEST("CMP",   "R1, [R3+]",       0x48, 0x1F);
    TEST("CMP",   "R1, #7",          0x48, 0x17);
    TEST("CMPB",  "RL0, [R2]",       0x49, 0x0A);
    TEST("CMPB",  "0FE12H, #18",    0x47, 0x09, 0x12, 0x00);
    TEST("CMPB",  "RL1, RH2",        0x41, 0x25);
    TEST("CMPB",  "RL0, [R1+]",      0x49, 0x0D);
    TEST("CMPB",  "RH7, #5",         0x49, 0xF5);
    TEST("CMPB",  "0FE12H, 1234H",   0x43, 0x09, 0x34, 0x12);
    TEST("XOR",   "R2, #3",          0x58, 0x23);
    TEST("XOR",   "R1, R2",          0x50, 0x12);
    TEST("XOR",   "R1, [R2]",        0x58, 0x1A);
    TEST("XOR",   "R1, [R3+]",       0x58, 0x1F);
    TEST("XOR",   "0FE12H, #1234H",  0x56, 0x09, 0x34, 0x12);
    TEST("XOR",   "0FE12H, 1234H",   0x52, 0x09, 0x34, 0x12);
    TEST("XOR",   "1234H, 0FE12H",   0x54, 0x09, 0x34, 0x12);
    TEST("XORB",  "1234H, 0FE12H",   0x55, 0x09, 0x34, 0x12);
    TEST("XORB",  "RL1, RH2",        0x51, 0x25);
    TEST("XORB",  "RH0, [R0]",       0x59, 0x18);
    TEST("XORB",  "RL0, [R1+]",      0x59, 0x0D);
    TEST("XORB",  "RH7, #5",         0x59, 0xF5);
    TEST("XORB",  "0FE12H, #34H",    0x57, 0x09, 0x34, 0x00);
    TEST("XORB",  "0FE12H, 1234H",   0x53, 0x09, 0x34, 0x12);
    TEST("AND",   "R3, [R1+]",       0x68, 0x3D);
    TEST("AND",   "R1, R2",          0x60, 0x12);
    TEST("AND",   "R1, [R2]",        0x68, 0x1A);
    TEST("AND",   "R1, #7",          0x68, 0x17);
    TEST("AND",   "0FE12H, #1234H",  0x66, 0x09, 0x34, 0x12);
    TEST("AND",   "0FE12H, 1234H",   0x62, 0x09, 0x34, 0x12);
    TEST("AND",   "1234H, 0FE12H",   0x64, 0x09, 0x34, 0x12);
    TEST("ANDB",  "RL3, RH3",        0x61, 0x67);
    TEST("ANDB",  "RH0, [R0]",       0x69, 0x18);
    TEST("ANDB",  "RL0, [R1+]",      0x69, 0x0D);
    TEST("ANDB",  "RH7, #5",         0x69, 0xF5);
    TEST("ANDB",  "0FE12H, #34H",    0x67, 0x09, 0x34, 0x00);
    TEST("ANDB",  "0FE12H, 1234H",   0x63, 0x09, 0x34, 0x12);
    TEST("ANDB",  "1234H, RH1",      0x65, 0xF3, 0x34, 0x12);
    TEST("OR",    "0FE12H, #0FFFFH", 0x76, 0x09, 0xFF, 0xFF);
    TEST("OR",    "R1, R2",          0x70, 0x12);
    TEST("OR",    "R1, [R2]",        0x78, 0x1A);
    TEST("OR",    "R1, [R3+]",       0x78, 0x1F);
    TEST("OR",    "R1, #7",          0x78, 0x17);
    TEST("OR",    "0FE12H, 1234H",   0x72, 0x09, 0x34, 0x12);
    TEST("OR",    "1234H, 0FE12H",   0x74, 0x09, 0x34, 0x12);
    TEST("ORB",   "RH1, #7",         0x79, 0x37);
    TEST("ORB",   "RL1, RH2",        0x71, 0x25);
    TEST("ORB",   "RH0, [R0]",       0x79, 0x18);
    TEST("ORB",   "RL0, [R1+]",      0x79, 0x0D);
    TEST("ORB",   "0FE12H, #34H",    0x77, 0x09, 0x34, 0x00);
    TEST("ORB",   "0FE12H, 1234H",   0x73, 0x09, 0x34, 0x12);
    TEST("ORB",   "1234H, RH1",      0x75, 0xF3, 0x34, 0x12);
}

void test_multiply() {
    TEST("MUL",   "R1, R2", 0x0B, 0x12);
    TEST("MULU",  "R3, R4", 0x1B, 0x34);
    TEST("PRIOR", "R5, R6", 0x2B, 0x56);
    TEST("DIV",   "R3",     0x4B, 0x33);
    TEST("DIVU",  "R4",     0x5B, 0x44);
    TEST("DIVL",  "R5",     0x6B, 0x55);
    TEST("DIVLU", "R15",    0x7B, 0xFF);
}

void test_shift() {
    TEST("ROL",  "R1, R2",   0x0C, 0x12);
    TEST("ROL",  "R1, #15",  0x1C, 0xF1);
    TEST("ROR",  "R3, R4",   0x2C, 0x34);
    TEST("ROR",  "R3, #1",   0x3C, 0x13);
    TEST("SHL",  "R5, R6",   0x4C, 0x56);
    TEST("SHL",  "R5, #0",   0x5C, 0x05);
    TEST("SHR",  "R7, R8",   0x6C, 0x78);
    TEST("SHR",  "R7, #8",   0x7C, 0x87);
    TEST("ASHR", "R9, R10",  0xAC, 0x9A);
    TEST("ASHR", "R9, #10",  0xBC, 0xA9);
}

void test_compare() {
    TEST("CMPI1", "R1, #3",        0x80, 0x31);
    TEST("CMPI1", "R2, #1234H",    0x86, 0xF2, 0x34, 0x12);
    TEST("CMPI1", "R3, 1234H",     0x82, 0xF3, 0x34, 0x12);
    TEST("CMPI2", "R4, #15",       0x90, 0xF4);
    TEST("CMPI2", "R5, #1234H",    0x96, 0xF5, 0x34, 0x12);
    TEST("CMPI2", "R6, 1234H",     0x92, 0xF6, 0x34, 0x12);
    TEST("CMPD1", "R7, #0",        0xA0, 0x07);
    TEST("CMPD1", "R8, #1234H",    0xA6, 0xF8, 0x34, 0x12);
    TEST("CMPD1", "R9, 1234H",     0xA2, 0xF9, 0x34, 0x12);
    TEST("CMPD2", "R10, #1",       0xB0, 0x1A);
    TEST("CMPD2", "R11, #1234H",   0xB6, 0xFB, 0x34, 0x12);
    TEST("CMPD2", "R12, 1234H",    0xB2, 0xFC, 0x34, 0x12);
}

void test_unary() {
    TEST("NEG",  "R1",  0x81, 0x10);
    TEST("CPL",  "R2",  0x91, 0x20);
    TEST("NEGB", "RH0", 0xA1, 0x10);
    TEST("CPLB", "RL1", 0xB1, 0x20);
}

void test_move() {
    TEST("MOV",   "R1, R2",            0xF0, 0x12);
    TEST("MOV",   "R1, #5",            0xE0, 0x51);
    TEST("MOV",   "0FE12H, #1234H",    0xE6, 0x09, 0x34, 0x12);
    TEST("MOV",   "R1, [R2]",          0xA8, 0x12);
    TEST("MOV",   "R1, [R2+]",         0x98, 0x12);
    TEST("MOV",   "[R2], R1",          0xB8, 0x12);
    TEST("MOV",   "[-R2], R1",         0x88, 0x12);
    TEST("MOV",   "[R1], [R2]",        0xC8, 0x12);
    TEST("MOV",   "[R1+], [R2]",       0xD8, 0x12);
    TEST("MOV",   "[R1], [R2+]",       0xE8, 0x12);
    TEST("MOV",   "R1, [R2+#1234H]",   0xD4, 0x12, 0x34, 0x12);
    TEST("MOV",   "[R2+#1234H], R1",   0xC4, 0x12, 0x34, 0x12);
    TEST("MOV",   "[R1], 1234H",       0x84, 0x01, 0x34, 0x12);
    TEST("MOV",   "1234H, [R1]",       0x94, 0x01, 0x34, 0x12);
    TEST("MOV",   "0FE12H, 1234H",     0xF2, 0x09, 0x34, 0x12);
    TEST("MOV",   "1234H, 0FE12H",     0xF6, 0x09, 0x34, 0x12);
    TEST("MOVB",  "RL1, RH2",          0xF1, 0x25);
    TEST("MOVB",  "RH0, #5",           0xE1, 0x51);
    TEST("MOVB",  "0FE12H, #34H",      0xE7, 0x09, 0x34, 0x00);
    TEST("MOVB",  "RH0, [R2]",         0xA9, 0x12);
    TEST("MOVB",  "RH0, [R2+]",        0x99, 0x12);
    TEST("MOVB",  "[R2], RH0",         0xB9, 0x12);
    TEST("MOVB",  "[-R2], RH0",        0x89, 0x12);
    TEST("MOVB",  "[R1], [R2]",        0xC9, 0x12);
    TEST("MOVB",  "[R1+], [R2]",       0xD9, 0x12);
    TEST("MOVB",  "[R1], [R2+]",       0xE9, 0x12);
    TEST("MOVB",  "RH0, [R2+#1234H]",  0xF4, 0x12, 0x34, 0x12);
    TEST("MOVB",  "[R2+#1234H], RH0",  0xE4, 0x12, 0x34, 0x12);
    TEST("MOVB",  "[R1], 1234H",       0xA4, 0x01, 0x34, 0x12);
    TEST("MOVB",  "1234H, [R1]",       0xB4, 0x01, 0x34, 0x12);
    TEST("MOVB",  "0FE12H, 1234H",     0xF3, 0x09, 0x34, 0x12);
    TEST("MOVB",  "1234H, 0FE12H",     0xF7, 0x09, 0x34, 0x12);
    TEST("MOVBZ", "R1, RH1",           0xC0, 0x31);
    TEST("MOVBZ", "0FE12H, 1234H",     0xC2, 0x09, 0x34, 0x12);
    TEST("MOVBZ", "1234H, RL0",        0xC5, 0xF0, 0x34, 0x12);
    TEST("MOVBS", "R2, RL1",           0xD0, 0x22);
    TEST("MOVBS", "R1, 1234H",         0xD2, 0xF1, 0x34, 0x12);
    TEST("MOVBS", "1234H, 0FE12H",     0xD5, 0x09, 0x34, 0x12);
    TEST("SCXT",  "0FE12H, #1234H",    0xC6, 0x09, 0x34, 0x12);
    TEST("SCXT",  "R1, 1234H",         0xD6, 0xF1, 0x34, 0x12);
    TEST("PUSH",  "0FE12H",            0xEC, 0x09);
    TEST("POP",   "R1",                0xFC, 0xF1);
}

void test_bit() {
    TEST("BCLR",  "0FD02H.3",            0x3E, 0x01);
    TEST("BSET",  "R1.15",               0xFF, 0xF1);
    TEST("BSET",  "0FF02H.0",            0x0F, 0x81);
    TEST("BCLR",  "0FFDEH.9",            0x9E, 0xEF);
    TEST("BFLDL", "0FD02H, #18, #34H",  0x0A, 0x01, 0x12, 0x34);
    TEST("BFLDH", "0FD02H, #18, #34H",  0x1A, 0x01, 0x34, 0x12);
    TEST("BFLDL", "R2, #0FFH, #0",       0x0A, 0xF2, 0xFF, 0x00);
    TEST("BCMP",  "0FD02H.3, 0FD04H.5",  0x2A, 0x02, 0x01, 0x53);
    TEST("BMOVN", "R1.0, 0FF00H.15",     0x3A, 0x80, 0xF1, 0xF0);
    TEST("BMOV",  "0FD02H.3, 0FD04H.5",  0x4A, 0x02, 0x01, 0x53);
    TEST("BOR",   "0FD02H.3, 0FD04H.5",  0x5A, 0x02, 0x01, 0x53);
    TEST("BAND",  "0FD02H.3, 0FD04H.5",  0x6A, 0x02, 0x01, 0x53);
    TEST("BXOR",  "0FD02H.3, 0FD04H.5",  0x7A, 0x02, 0x01, 0x53);
}

void test_jump() {
    // Code addresses print at the width of the address space.
    ATEST(0x1000, "JMPR",  "CC_UC, 0100CH",      0x0D, 0x05);
    ATEST(0x1000, "JMPR",  "CC_Z, 00FFEH",        0x2D, 0xFE);
    ATEST(0x1000, "JMPR",  "CC_ULE, 01002H",    0xFD, 0x00);
    ATEST(0x1FFFE, "JMPR", "CC_NET, 10002H",    0x1D, 0x01);
    ATEST(0x10000, "JMPR", "CC_SGT, 1FF04H",    0xAD, 0x81);
    ATEST(0x1000, "CALLR", "01100H",                    0xBB, 0x7F);
    ATEST(0x1000, "JB",    "0FD02H.3, 01024H",
          0x8A, 0x01, 0x10, 0x30);
    ATEST(0x1000, "JNB",   "R1.0, 00F84H",        0x9A, 0xF1, 0xC0, 0x00);
    ATEST(0x1000, "JBC",   "0FF02H.15, 01004H",
          0xAA, 0x81, 0x00, 0xF0);
    ATEST(0x1000, "JNBS",  "0FD00H.1, 01006H",
          0xBA, 0x00, 0x01, 0x10);
    AERRT(0x1000, "JB",    "0FD02H.3, 01024H",
          ILLEGAL_OPERAND_MODE, "01024H",
          0x8A, 0x01, 0x10, 0x31);
    ATEST(0x21000, "JMPA", "CC_NZ, 21234H",      0xEA, 0x30, 0x34, 0x12);
    ATEST(0x1000, "CALLA", "CC_UC, 0FFFEH",      0xCA, 0x00, 0xFE, 0xFF);
    ATEST(0x1000, "PCALL", "0FE12H, 01234H",    0xE2, 0x09, 0x34, 0x12);
    TEST("JMPI",  "CC_UC, [R1]",       0x9C, 0x01);
    TEST("JMPI",  "CC_NET, [R1]",      0x9C, 0x11);
    TEST("JMPI",  "CC_Z, [R1]",        0x9C, 0x21);
    TEST("JMPI",  "CC_NZ, [R1]",       0x9C, 0x31);
    TEST("JMPI",  "CC_V, [R1]",        0x9C, 0x41);
    TEST("JMPI",  "CC_NV, [R1]",       0x9C, 0x51);
    TEST("JMPI",  "CC_N, [R1]",        0x9C, 0x61);
    TEST("JMPI",  "CC_NN, [R1]",       0x9C, 0x71);
    TEST("JMPI",  "CC_C, [R1]",        0x9C, 0x81);
    TEST("JMPI",  "CC_NC, [R1]",       0x9C, 0x91);
    TEST("JMPI",  "CC_SGT, [R1]",      0x9C, 0xA1);
    TEST("JMPI",  "CC_SLE, [R1]",      0x9C, 0xB1);
    TEST("JMPI",  "CC_SLT, [R1]",      0x9C, 0xC1);
    TEST("JMPI",  "CC_SGE, [R1]",      0x9C, 0xD1);
    TEST("JMPI",  "CC_UGT, [R1]",      0x9C, 0xE1);
    TEST("JMPI",  "CC_ULE, [R1]",      0x9C, 0xF1);
    TEST("CALLI", "CC_Z, [R15]",       0xAB, 0x2F);
    TEST("JMPS",  "1, 2345H",          0xFA, 0x01, 0x45, 0x23);
    TEST("CALLS", "3, 0FFFEH",         0xDA, 0x03, 0xFE, 0xFF);
    TEST("TRAP",  "#7FH",              0x9B, 0xFE);
    TEST("TRAP",  "#0",                0x9B, 0x00);
    ERRT("JMPS",  "4, 2345H", ILLEGAL_OPERAND_MODE, "4, 2345H", 0xFA, 0x04, 0x45, 0x23);
}

void test_relative() {
    disassembler.setOption("relative", "on");

    ATEST(0x1000, "JMPR",  "CC_UC, $",          0x0D, 0xFF);
    ATEST(0x1000, "JMPR",  "CC_Z, $+4",         0x2D, 0x01);
    ATEST(0x1000, "CALLR", "$-2",               0xBB, 0xFE);
    ATEST(0x1000, "JB",    "R3.4, $",           0x8A, 0xF3, 0xFE, 0x40);
    ATEST(0x1000, "JNB",   "R1.0, $-124",       0x9A, 0xF1, 0xC0, 0x00);
    ATEST(0x1000, "JMPR",  "CC_UC, $+256",      0x0D, 0x7F);
}

void test_options() {
    disassembler.setOption("cc-prefix", "off");
    disassembler.setOption("index-sharp", "off");

    TEST("JMPI",  "UC, [R1]",        0x9C, 0x01);
    TEST("CALLI", "Z, [R15]",        0xAB, 0x2F);
    ATEST(0x21000, "JMPA", "NZ, 21234H", 0xEA, 0x30, 0x34, 0x12);
    TEST("MOV",   "R1, [R2+1234H]",  0xD4, 0x12, 0x34, 0x12);
    TEST("MOVB",  "[R2+1234H], RL0", 0xE4, 0x02, 0x34, 0x12);
}

// The sfr-name option writes a core SFR by name in reg, bitoff and mem.
void test_sfr_name() {
    TEST("ADD",  "0FE12H, #1234H",  0x06, 0x09, 0x34, 0x12);

    disassembler.setOption("sfr-name", "on");
    TEST("ADD",  "SP, #1234H",      0x06, 0x09, 0x34, 0x12);
    TEST("BSET", "PSW.3",           0x3F, 0x88);
    TEST("MOV",  "R1, SP",          0xF2, 0xF1, 0x12, 0xFE);
    TEST("MOV",  "DPP0, ONES",      0xF2, 0x00, 0x1E, 0xFF);
    TEST("ADD",  "0FFDEH, #1234H",  0x06, 0xEF, 0x34, 0x12);
    TEST("BSET", "0FD02H.3",        0x3F, 0x01);
    TEST("BSET", "TFR.3",           0x3F, 0xD6);
}

// The gnu-as option names the core SFRs, writes numbers as C does and
// turns down what GNU as cannot write.
void test_gnu_as() {
    disassembler.setOption("gnu-as", "on");
    TEST("ADD",  "SP, #0x1234",      0x06, 0x09, 0x34, 0x12);
    TEST("BSET", "PSW.3",           0x3F, 0x88);
    TEST("ADD",  "R1, #7",          0x08, 0x17);
    // GNU as reads a name for mem as a symbol in some instructions.
    TEST("MOV",  "R1, 0xFE12",      0xF2, 0xF1, 0x12, 0xFE);
    ERRT("ADD",  "0xFFDE, #0x1234", UNKNOWN_REGISTER, "0xFFDE, #0x1234", 0x06, 0xEF, 0x34, 0x12);
    ERRT("BSET", "0xFF02.3", UNKNOWN_REGISTER, "0xFF02.3", 0x3F, 0x81);
    ERRT("BSET", "0xFD02.3", ILLEGAL_OPERAND_MODE, "0xFD02.3", 0x3F, 0x01);
    ERRT("ADD",  "R1, #0", ILLEGAL_OPERAND_MODE, "#0", 0x08, 0x10);
}

// With an SFR in both operands, "mem, reg" assembles back as "reg, mem";
// the canonical-mem option refuses it for a caller which needs the encoding
// to survive reassembly.
void test_canonical_mem() {
    TEST("ADD",   "0FE04H, 0FE06H",  0x04, 0x03, 0x04, 0xFE);
    TEST("MOVBZ", "0FE04H, 0FE06H",  0xC5, 0x03, 0x04, 0xFE);

    disassembler.setOption("canonical-mem", "on");
    ERRT("ADD",   "", ILLEGAL_OPERAND_MODE, "", 0x04, 0x03, 0x04, 0xFE);
    ERRT("MOVBZ", "", ILLEGAL_OPERAND_MODE, "", 0xC5, 0x03, 0x04, 0xFE);
    ERRT("MOV",   "", ILLEGAL_OPERAND_MODE, "", 0xF6, 0xEF, 0xDE, 0xFF);
    // Not when the mem is no SFR address, or the reg is a GPR.
    TEST("ADD",   "0FE05H, 0FE06H",  0x04, 0x03, 0x05, 0xFE);
    TEST("ADD",   "0FFE0H, 0FE06H",  0x04, 0x03, 0xE0, 0xFF);
    TEST("ADD",   "0FE04H, R0",      0x04, 0xF0, 0x04, 0xFE);
    TEST("ADD",   "0FE06H, 0FE04H",  0x02, 0x03, 0x04, 0xFE);
}

void test_return() {
    TEST("RET",  "",       0xCB, 0x00);
    TEST("RETS", "",       0xDB, 0x00);
    TEST("RETI", "",       0xFB, 0x88);
    TEST("RETP", "0FE12H", 0xEB, 0x09);
    TEST("RETP", "R5",     0xEB, 0xF5);
}

void test_system() {
    TEST("NOP",    "", 0xCC, 0x00);
    TEST("IDLE",   "", 0x87, 0x78, 0x87, 0x87);
    TEST("PWRDN",  "", 0x97, 0x68, 0x97, 0x97);
    TEST("SRVWDT", "", 0xA7, 0x58, 0xA7, 0xA7);
    TEST("SRST",   "", 0xB7, 0x48, 0xB7, 0xB7);
    TEST("DISWDT", "", 0xA5, 0x5A, 0xA5, 0xA5);
    TEST("EINIT",  "", 0xB5, 0x4A, 0xB5, 0xB5);
}

void test_illegal() {
    // The undefined opcodes of the opcode tables (manual section 4).
    static constexpr Config::opcode_t undefined[] = {
        0x3B, 0x44, 0x45, 0x83, 0x85, 0x8B, 0x8C, 0x93, 0x95, 0xA3,
        0xB3, 0xC1, 0xC3, 0xC7, 0xD3, 0xE3, 0xE5, 0xF5, 0xF8, 0xF9,
    };
    for (const auto opc : undefined)
        UNKN(opc, 0x00);
    // D1, DC and D7 are not available on the 80C166.
    UNKN(0xD1, 0x00);
    UNKN(0xD1, 0x80);
    UNKN(0xDC, 0x01);
    UNKN(0xD7, 0x00);

    // Constant bits of the operand byte (manual section 5).
    UNKN(0x81, 0x11);  // NEG Rwn: n0
    UNKN(0x91, 0x1F);  // CPL Rwn: n0
    UNKN(0xA1, 0x18);  // NEGB Rbn: n0
    UNKN(0xB1, 0x12);  // CPLB Rbn: n0
    UNKN(0x84, 0x11);  // MOV [Rwn], mem: 0n
    UNKN(0x94, 0xF1);  // MOV mem, [Rwn]: 0n
    UNKN(0xA4, 0x21);  // MOVB [Rwn], mem: 0n
    UNKN(0xB4, 0x81);  // MOVB mem, [Rwn]: 0n
    UNKN(0x86, 0xE1);  // CMPI1 Rwn, #data16: Fn
    UNKN(0x82, 0x01);  // CMPI1 Rwn, mem: Fn
    UNKN(0x96, 0x01);  // CMPI2 Rwn, #data16: Fn
    UNKN(0x92, 0x71);  // CMPI2 Rwn, mem: Fn
    UNKN(0xA6, 0xE1);  // CMPD1 Rwn, #data16: Fn
    UNKN(0xA2, 0x01);  // CMPD1 Rwn, mem: Fn
    UNKN(0xB6, 0x01);  // CMPD2 Rwn, #data16: Fn
    UNKN(0xB2, 0x71);  // CMPD2 Rwn, mem: Fn
    UNKN(0x4B, 0x34);  // DIV Rwn: nn
    UNKN(0x5B, 0x10);  // DIVU Rwn: nn
    UNKN(0x6B, 0x01);  // DIVL Rwn: nn
    UNKN(0x7B, 0xFE);  // DIVLU Rwn: nn
    UNKN(0xCA, 0x01);  // CALLA cc, caddr: c0
    UNKN(0xEA, 0x0F);  // JMPA cc, caddr: c0
    UNKN(0x9B, 0x01);  // TRAP #trap7: t:ttt0
    UNKN(0xCC, 0x01);  // NOP: 00
    UNKN(0xCB, 0x80);  // RET: 00
    UNKN(0xDB, 0x08);  // RETS: 00
    UNKN(0xFB, 0x00);  // RETI: 88
    UNKN(0x87, 0x87);  // IDLE: 87 78 87 87
    UNKN(0x97, 0x00);  // PWRDN: 97 68 97 97
    UNKN(0xA7, 0xA7);  // SRVWDT: A7 58 A7 A7
    UNKN(0xB7, 0x00);  // SRST: B7 48 B7 B7
    UNKN(0xA5, 0xA5);  // DISWDT: A5 5A A5 A5
    UNKN(0xB5, 0x00);  // EINIT: B5 4A B5 B5
    UNKN(0x87, 0x78, 0x87, 0x00);
    UNKN(0x97, 0x68, 0x00, 0x97);
    UNKN(0xA7, 0x58, 0xA7, 0x58);
    UNKN(0xB7, 0x48, 0x48, 0xB7);
    UNKN(0xA5, 0x5A, 0x5A, 0xA5);
    UNKN(0xB5, 0x4A, 0xB5, 0x4A);
}
// clang-format on

void run_tests(const char *cpu) {
    disassembler.setCpu(cpu);
    RUN_TEST(test_arithmetic);
    RUN_TEST(test_multiply);
    RUN_TEST(test_shift);
    RUN_TEST(test_compare);
    RUN_TEST(test_unary);
    RUN_TEST(test_move);
    RUN_TEST(test_bit);
    RUN_TEST(test_jump);
    RUN_TEST(test_relative);
    RUN_TEST(test_options);
    RUN_TEST(test_canonical_mem);
    RUN_TEST(test_sfr_name);
    RUN_TEST(test_gnu_as);
    RUN_TEST(test_return);
    RUN_TEST(test_system);
    RUN_TEST(test_illegal);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

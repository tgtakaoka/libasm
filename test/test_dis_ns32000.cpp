/*
 * Copyright 2020 Tadashi G. Takaoka
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

#include "dis_ns32000.h"
#include "test_dis_helper.h"

using namespace libasm;
using namespace libasm::ns32000;
using namespace libasm::test;

DisNs32000 dis32k;
Disassembler &disassembler(dis32k);

static void set_up() {
    disassembler.reset();
    disassembler.setRelativeTarget(true);
}

static void tear_down() {
    symtab.reset();
}

// clang-format off
static void test_cpu() {
    EQUALS("cpu ns32032", true,    disassembler.setCpu("NS32032"));
    EQUALS("cpu ns32032", "32032", disassembler.getCpu());

    EQUALS("cpu 32032", true,    disassembler.setCpu("32032"));
    EQUALS("cpu 32032", "32032", disassembler.getCpu());
}

static void test_format_0() {
    disassembler.setRelativeTarget(true);
    TEST(BEQ, ".",      0x0A, 0x00);
    TEST(BNE, ".+10",   0x1A, 0x0A);
    TEST(BCS, ".-64",   0x2A, 0x40);
    TEST(BCC, ".+8191", 0x3A, 0x9F, 0xFF);
    TEST(BHI, ".-8192", 0x4A, 0xA0, 0x00);
    TEST(BLS, ".+0x007FFFFF", 0x5A, 0xC0, 0x7F, 0xFF, 0xFF);
    TEST(BGT, ".-0x00800000", 0x6A, 0xFF, 0x80, 0x00, 0x00);
    TEST(BLE, ".+0x1FFFFFFF", 0x7A, 0xDF, 0xFF, 0xFF, 0xFF);
    TEST(BFS, ".-0x1F000000", 0x8A, 0xE1, 0x00, 0x00, 0x00);
    TEST(BFC, ".+2",    0x9A, 0x02);
    TEST(BLO, ".-154",  0xAA, 0xBF, 0x66);
    TEST(BHS, ".+63",   0xBA, 0x3F);
    TEST(BLT, ".-128",  0xCA, 0xBF, 0x80);
    TEST(BGE, ".+256",  0xDA, 0x81, 0x00);
    TEST(BR,  ".+10",   0xEA, 0x0A);
    TEST(BR,  ".-154",  0xEA, 0xBF, 0x66);
}

static void test_format_1() {
    TEST(BSR, ".+16", 0x02, 0x10);

    TEST(RET,  "16", 0x12, 0x10);
    TEST(RETT, "16", 0x42, 0x10);
    TEST(RETI, "",   0x52);
    TEST(NOP,  "",   0xA2);
    TEST(WAIT, "",   0xB2);
    TEST(DIA,  "",   0xC2);
    TEST(FLAG, "",   0xD2);
    TEST(SVC,  "",   0xE2);
    TEST(BPT,  "",   0xF2);

    TEST(CXP, "1",  0x22, 0x01);
    TEST(RXP, "16", 0x32, 0x10);

    TEST(SAVE,    "[R0,R2,R7]",     0x62, 0x85);
    TEST(RESTORE, "[R0,R2,R7]",     0x72, 0xA1);
    TEST(ENTER,   "[R0,R2,R7], 16", 0x82, 0x85, 0x10);
    TEST(EXIT,    "[R0,R2,R7]",     0x92, 0xA1);
    ERNE(SAVE,    "",               0x62, 0x00);
    ERNE(RESTORE, "",               0x72, 0x00);

    // Various displacement.
    TEST(RET, "0",           0x12, 0x00);
    TEST(RET, "63",          0x12, 0x3F);
    TEST(RET, "-64",         0x12, 0x40);
    TEST(RET, "0x1FFF",      0x12, 0x9F, 0xFF);
    TEST(RET, "-0x2000",     0x12, 0xA0, 0x00);
    TEST(RET, "0x1FFFFFFF",  0x12, 0xDF, 0xFF, 0xFF, 0xFF);
    TEST(RET, "-0x1F000000", 0x12, 0xE1, 0x00, 0x00, 0x00);
    ERIC(RET, "reserved",    0x12, 0xE0);
}

static void test_format_2() {
    TEST(ADDQB, "-8, R0", 0x0C, 0x04);
    TEST(ADDQW, "7, R1",  0x8D, 0x0B);
    TEST(ADDQD, "-8, R2", 0x0F, 0x14);
    TEST(CMPQB, "-8, R0", 0x1C, 0x04);
    TEST(CMPQW, "7, R1",  0x9D, 0x0B);
    TEST(CMPQD, "-8, R2", 0x1F, 0x14);
    TEST(MOVQB, "0, R0",  0x5C, 0x00);
    TEST(MOVQW, "7, TOS", 0xDD, 0xBB);
    TEST(MOVQD, "-8, R2", 0x5F, 0x14);

    TEST(ACBB,  "-1, R0, .-3", 0xCC, 0x07, 0x7D);
    TEST(ACBW,  "-2, R1, .-2", 0x4D, 0x0F, 0x7E);
    TEST(ACBD,  "4, R2, .+2",  0x4F, 0x12, 0x02);

    TEST(LPRB, "UPSR, R1", 0x6C, 0x08);
    ERUR(LPRB, "pr1, R1",  0xEC, 0x00);
    ERUR(LPRB, "pr2, R1",  0x6C, 0x01);
    ERUR(LPRB, "pr3, R1",  0xEC, 0x01);
    ERUR(LPRB, "pr4, R1",  0x6C, 0x02);
    ERUR(LPRB, "pr5, R1",  0xEC, 0x02);
    ERUR(LPRB, "pr6, R1",  0x6C, 0x03);
    ERUR(LPRB, "pr7, R1",  0xEC, 0x03);
    TEST(LPRB, "FP, R2",   0x6C, 0x14);
    TEST(LPRB, "SP, R3",   0xEC, 0x1C);
    TEST(LPRB, "SB, R4",   0x6C, 0x25);
    ERUR(LPRB, "pr11, R1", 0xEC, 0x05);
    ERUR(LPRB, "pr12, R1", 0x6C, 0x06);
    TEST(LPRB, "PSR, R5",  0xEC, 0x2E);
    TEST(LPRB, "INTBASE, R6", 0x6C, 0x37);
    TEST(LPRB, "MOD, R7",  0xEC, 0x3F);

    TEST(LPRW, "UPSR, R1", 0x6D, 0x08);
    ERUR(LPRW, "pr1, R1",  0xED, 0x00);
    ERUR(LPRW, "pr2, R1",  0x6D, 0x01);
    ERUR(LPRW, "pr3, R1",  0xED, 0x01);
    ERUR(LPRW, "pr4, R1",  0x6D, 0x02);
    ERUR(LPRW, "pr5, R1",  0xED, 0x02);
    ERUR(LPRW, "pr6, R1",  0x6D, 0x03);
    ERUR(LPRW, "pr7, R1",  0xED, 0x03);
    TEST(LPRW, "FP, R2",   0x6D, 0x14);
    TEST(LPRW, "SP, R3",   0xED, 0x1C);
    TEST(LPRW, "SB, R4",   0x6D, 0x25);
    ERUR(LPRW, "pr11, R1", 0xED, 0x05);
    ERUR(LPRW, "pr12, R1", 0x6D, 0x06);
    TEST(LPRW, "PSR, R5",  0xED, 0x2E);
    TEST(LPRW, "INTBASE, R6", 0x6D, 0x37);
    TEST(LPRW, "MOD, 4(SB)",  0xED, 0xD7, 0x04);

    TEST(LPRD, "UPSR, R1", 0x6F, 0x08);
    ERUR(LPRD, "pr1, R1",  0xEF, 0x00);
    ERUR(LPRD, "pr2, R1",  0x6F, 0x01);
    ERUR(LPRD, "pr3, R1",  0xEF, 0x01);
    ERUR(LPRD, "pr4, R1",  0x6F, 0x02);
    ERUR(LPRD, "pr5, R1",  0xEF, 0x02);
    ERUR(LPRD, "pr6, R1",  0x6F, 0x03);
    ERUR(LPRD, "pr7, R1",  0xEF, 0x03);
    TEST(LPRD, "FP, R2",   0x6F, 0x14);
    TEST(LPRD, "SP, R3",   0xEF, 0x1C);
    TEST(LPRD, "SB, R4",   0x6F, 0x25);
    ERUR(LPRD, "pr11, R1", 0xEF, 0x05);
    ERUR(LPRD, "pr12, R1", 0x6F, 0x06);
    TEST(LPRD, "PSR, R5",  0xEF, 0x2E);
    TEST(LPRD, "INTBASE, R6", 0x6F, 0x37);
    TEST(LPRD, "MOD, 4(SB)",  0xEF, 0xD7, 0x04);

    TEST(SPRB, "UPSR, R1", 0x2C, 0x08);
    ERUR(SPRB, "pr1, R1",  0xAC, 0x00);
    ERUR(SPRB, "pr2, R1",  0x2C, 0x01);
    ERUR(SPRB, "pr3, R1",  0xAC, 0x01);
    ERUR(SPRB, "pr4, R1",  0x2C, 0x02);
    ERUR(SPRB, "pr5, R1",  0xAC, 0x02);
    ERUR(SPRB, "pr6, R1",  0x2C, 0x03);
    ERUR(SPRB, "pr7, R1",  0xAC, 0x03);
    TEST(SPRB, "FP, R2",   0x2C, 0x14);
    TEST(SPRB, "SP, R3",   0xAC, 0x1C);
    TEST(SPRB, "SB, R4",   0x2C, 0x25);
    ERUR(SPRB, "pr11, R1", 0xAC, 0x05);
    ERUR(SPRB, "pr12, R1", 0x2C, 0x06);
    TEST(SPRB, "PSR, R5",  0xAC, 0x2E);
    TEST(SPRB, "INTBASE, R6", 0x2C, 0x37);
    TEST(SPRB, "MOD, R7",     0xAC, 0x3F);

    TEST(SPRW, "UPSR, R1", 0x2D, 0x08);
    ERUR(SPRW, "pr1, R1",  0xAD, 0x00);
    ERUR(SPRW, "pr2, R1",  0x2D, 0x01);
    ERUR(SPRW, "pr3, R1",  0xAD, 0x01);
    ERUR(SPRW, "pr4, R1",  0x2D, 0x02);
    ERUR(SPRW, "pr5, R1",  0xAD, 0x02);
    ERUR(SPRW, "pr6, R1",  0x2D, 0x03);
    ERUR(SPRW, "pr7, R1",  0xAD, 0x03);
    TEST(SPRW, "FP, R2",   0x2D, 0x14);
    TEST(SPRW, "SP, R3",   0xAD, 0x1C);
    TEST(SPRW, "SB, R4",   0x2D, 0x25);
    ERUR(SPRW, "pr11, R1", 0xAD, 0x05);
    ERUR(SPRW, "pr12, R1", 0x2D, 0x06);
    TEST(SPRW, "PSR, R5",  0xAD, 0x2E);
    TEST(SPRW, "INTBASE, R6", 0x2D, 0x37);
    TEST(SPRW, "MOD, 4(SB)",  0xAD, 0xD7, 0x04);

    TEST(SPRD, "UPSR, R1", 0x2F, 0x08);
    ERUR(SPRD, "pr1, R1",  0xAF, 0x00);
    ERUR(SPRD, "pr2, R1",  0x2F, 0x01);
    ERUR(SPRD, "pr3, R1",  0xAF, 0x01);
    ERUR(SPRD, "pr4, R1",  0x2F, 0x02);
    ERUR(SPRD, "pr5, R1",  0xAF, 0x02);
    ERUR(SPRD, "pr6, R1",  0x2F, 0x03);
    ERUR(SPRD, "pr7, R1",  0xAF, 0x03);
    TEST(SPRD, "FP, R2",   0x2F, 0x14);
    TEST(SPRD, "SP, R3",   0xAF, 0x1C);
    TEST(SPRD, "SB, R4",   0x2F, 0x25);
    ERUR(SPRD, "pr11, R1", 0xAF, 0x05);
    ERUR(SPRD, "pr12, R1", 0x2F, 0x06);
    TEST(SPRD, "PSR, R5",  0xAF, 0x2E);
    TEST(SPRD, "INTBASE, R6", 0x2F, 0x37);
    TEST(SPRD, "MOD, 4(SB)",  0xAF, 0xD7, 0x04);


    TEST(SEQB, "R0",           0x3C, 0x00);
    TEST(SNEB, "R1",           0xBC, 0x08);
    TEST(SCSB, "R2",           0x3C, 0x11);
    TEST(SCCB, "R3",           0xBC, 0x19);
    TEST(SHIB, "R4",           0x3C, 0x22);
    TEST(SLSB, "R5",           0xBC, 0x2A);
    TEST(SGTB, "R6",           0x3C, 0x33);
    TEST(SLEB, "R7",           0xBC, 0x3B);
    TEST(SFSB, "0(R0)",        0x3C, 0x44, 0x00);
    TEST(SFCB, "1(R1)",        0xBC, 0x4C, 0x01);
    TEST(SLOB, "2(R2)",        0x3C, 0x55, 0x02);
    TEST(SHSB, "3(R3)",        0xBC, 0x5D, 0x03);
    TEST(SLTB, "4(R4)",        0x3C, 0x66, 0x04);
    TEST(SGEB, "5(R5)",        0xBC, 0x6E, 0x05);
    TEST(SEQW, "6(R6)",        0x3D, 0x70, 0x06);
    TEST(SNEW, "7(R7)",        0xBD, 0x78, 0x07);
    TEST(SCSW, "8(9(FP))",     0x3D, 0x81, 0x09, 0x08);
    TEST(SCCW, "10(11(SP))",   0xBD, 0x89, 0x0B, 0x0A);
    TEST(SHIW, "12(13(SB))",   0x3D, 0x92, 0x0D, 0x0C);
    TEST(SLSW, "@14",          0xBD, 0xAA, 0x0E);
    TEST(SGTW, "EXT(15)+16",   0x3D, 0xB3, 0x0F, 0x10);
    TEST(SLEW, "TOS",          0xBD, 0xBB);
    TEST(SFSW, "12(FP)",       0x3D, 0xC4, 0x0C);
    TEST(SFCW, "13(SP)",       0xBD, 0xCC, 0x0D);
    TEST(SLOW, "10(SB)",       0x3D, 0xD5, 0x0A);
    TEST(SHSW, ".+15(PC)",     0xBD, 0xDD, 0x0F);
    TEST(SLTW, "2(R0)[R7:B]",  0x3D, 0xE6, 0x47, 0x02);
    TEST(SGEW, "3(R1)[R6:W]",  0xBD, 0xEE, 0x4E, 0x03);
    TEST(SEQD, "4(R2)[R5:D]",  0x3F, 0xF0, 0x55, 0x04);
    TEST(SNED, "5(R3)[R4:Q]",  0xBF, 0xF8, 0x5C, 0x05);
    TEST(SCSD, "-1(R5)",       0x3F, 0x69, 0x7F);
    TEST(SCCD, "-2(R6)",       0xBF, 0x71, 0x7E);
    TEST(SHID, "TOS",          0x3F, 0xBA);
    TEST(SLSD, "-4(-5(FP))",   0xBF, 0x82, 0x7B, 0x7C);
    TEST(SGTD, "-6(-7(SP))",   0x3F, 0x8B, 0x79, 0x7A);
    TEST(SLED, "-8(-9(SB))",   0xBF, 0x93, 0x77, 0x78);
    TEST(SFSD, "@0x1234",      0x3F, 0xAC, 0x92, 0x34);
    TEST(SFCD, "EXT(-9)+(-8)", 0xBF, 0xB4, 0x77, 0x78);
    TEST(SLOD, "-12(FP)",      0x3F, 0xC5, 0x74);
    TEST(SHSD, "-13(SP)",      0xBF, 0xCD, 0x73);
    TEST(SLTD, "-14(SB)",      0x3F, 0xD6, 0x72);
    TEST(SGED, ".-15(PC)",     0xBF, 0xDE, 0x71);
}

static void test_format_3() {
    TEST(BICPSRB, "0xA2",  0x7C, 0xA1, 0xA2);
    TEST(BISPSRB, "0xA2",  0x7C, 0xA3, 0xA2);
    TEST(ADJSPD, "-4(FP)", 0x7F, 0xC5, 0x7C);
    TEST(CASEB,  ".+4(PC)[R7:B]", 0x7C, 0xE7, 0xDF, 0x04);
    TEST(CXPD,   "8(SB)",     0x7F, 0xD0, 0x08);
    TEST(JSR,    "0(4(SB))",  0x7F, 0x96, 0x04, 0x00);
    TEST(JUMP,   "0(-8(FP))", 0x7F, 0x82, 0x78, 0x00);
}

static void test_format_4() {
    TEST(MOVB,  "R0, R1",    0x54, 0x00);
    TEST(MOVW,  "R2, R3",    0xD5, 0x10);
    TEST(MOVD,  "R0, 8(SB)", 0x97, 0x06, 0x08);

    TEST(ADDB,  "R0, R1",        0x40, 0x00);
    TEST(ADDW,  "R2, R3",        0xC1, 0x10);
    TEST(ADDD,  "4(SB), -4(FP)", 0x03, 0xD6, 0x04, 0x7C);
    TEST(ADDCB, "32, R0",        0x10, 0xA0, 0x20);
    TEST(ADDCW, "R2, R3",        0xD1, 0x10);
    TEST(ADDCD, "8(SB), R0",     0x13, 0xD0, 0x08);
    TEST(SUBB,  "R0, R1",        0x60, 0x00);
    TEST(SUBW,  "R2, R3",        0xE1, 0x10);
    TEST(SUBD,  "4(SB), 20(SB)", 0xA3, 0xD6, 0x04, 0x14);
    TEST(SUBCB, "32, R1",        0x70, 0xA0, 0x20);
    TEST(SUBCW, "TOS, -8(FP)",   0x31, 0xBE, 0x78);
    TEST(SUBCD, "R4, R5",        0x73, 0x21);
    TEST(CMPB,  "7(SB), 4(R0)",  0x04, 0xD2, 0x07, 0x04);
    TEST(CMPW,  "R2, R3",        0xC5, 0x10);
    TEST(CMPD,  "R4, R5",        0x47, 0x21);

    TEST(ANDB,  "R0, R1", 0x68, 0x00);
    TEST(ANDW,  "R2, R3", 0xE9, 0x10);
    TEST(ANDD,  "R4, R5", 0x6B, 0x21);
    TEST(ORB,   "-6(FP), 11(SB)", 0x98, 0xC6, 0x7A, 0x0B);
    TEST(ORW,   "R2, R3",    0xD9, 0x10);
    TEST(ORD,   "R4, R5",    0x5B, 0x21);
    TEST(BICB,  "R0, 3(SB)", 0x88, 0x06, 0x03);
    TEST(BICW,  "R2, R3",    0xC9, 0x10);
    TEST(BICD,  "R4, R5",    0x4B, 0x21);
    TEST(XORB,  "-8(FP), -4(FP)", 0x38, 0xC6, 0x78, 0x7C);
    TEST(XORW,  "R2, R3",    0xF9, 0x10);
    TEST(XORD,  "R4, R5",    0x7B, 0x21);
    TEST(TBITB, "R0, R1",    0x74, 0x00);
    TEST(TBITW, "R0, 0(R1)", 0x75, 0x02, 0x00);
    TEST(TBITD, "R4, R5",    0x77, 0x21);

    TEST(ADDR,  "4(FP), R0", 0x27, 0xC0, 0x04);
}

static void test_format_5() {
    TEST(CMPSB, "",    0x0E, 0x04, 0x00);
    TEST(MOVST, "",    0x0E, 0x80, 0x00);
    TEST(SKPSB, "U",   0x0E, 0x0C, 0x06);
    TEST(SKPSB, "W",   0x0E, 0x0C, 0x02);
    TEST(SKPSB, "B,U", 0x0E, 0x0C, 0x07);
    TEST(SKPSB, "B,W", 0x0E, 0x0C, 0x03);

    TEST(SETCFG, "[I,F,M]", 0x0E, 0x8B, 0x03);
    TEST(SETCFG, "[]",      0x0E, 0x0B, 0x00);
}

static void test_format_6() {
    TEST(ABSB,  "R5, R6",        0x4E, 0xB0, 0x29);
    TEST(ABSD,  "8(SP), R7",     0x4E, 0xF3, 0xC9, 0x08);
    TEST(ADDPB, "5(SB), TOS",    0x4E, 0xFC, 0xD5, 0x05);
    TEST(ADDPD, "R0, R1",        0x4E, 0x7F, 0x00);
    TEST(ASHB,  "2, 16(SB)",     0x4E, 0x84, 0xA6, 0x02, 0x10);
    TEST(ASHW,  "TOS, 16(SB)",   0x4E, 0x85, 0xBE, 0x10);
    TEST(CBITW, "R0, 0(R1)",     0x4E, 0x49, 0x02, 0x00);
    TEST(COMB,  "R0, -4(FP)",    0x4E, 0x34, 0x06, 0x7C);
    TEST(IBITW, "R0, 1(R1)",     0x4E, 0x79, 0x02, 0x01);
    TEST(LSHB,  "4, 8(SB)",      0x4E, 0x94, 0xA6, 0x04, 0x08);
    TEST(LSHW,  "-4(FP), 8(SB)", 0x4E, 0x95, 0xC6, 0x7C, 0x08);
    TEST(NEGB,  "R5, R6",        0x4E, 0xA0, 0x29);
    TEST(NEGW,  "4(SB), 6(SB)",  0x4E, 0xA1, 0xD6, 0x04, 0x06);
    TEST(NOTB,  "R0, R0",        0x4E, 0x24, 0x00);
    TEST(NOTW,  "10(R1), TOS",   0x4E, 0xE5, 0x4D, 0x0A);
    TEST(ROTB,  "4, R5",         0x4E, 0x40, 0xA1, 0x04);
    TEST(ROTW,  "-3, 16(SP)",    0x4E, 0x41, 0xA6, 0xFD, 0x10);
    TEST(SBITW, "R0, 1(R1)",     0x4E, 0x59, 0x02, 0x01);
    TEST(SUBPB, "-8(FP), 16(FP)", 0x4E, 0x2C, 0xC6, 0x78, 0x10);
    TEST(SUBPD, "0x00000099, R1", 0x4E, 0x6F, 0xA0, 0x00, 0x00, 0x00, 0x99);
}

static void test_format_7() {
    TEST(CMPMW, "10(R0), 16(R1), 4", 0xCE, 0x45, 0x42, 0x0A, 0x10, 0x06);
    TEST(DEIB,  "R1, R0", 0xCE, 0x2C, 0x08);
    TEST(DEIW,  "R3, R2", 0xCE, 0xAD, 0x18);
    TEST(DEID,  "R5, R4", 0xCE, 0x2F, 0x29);
    TEST(DIVW,  "10(SP), 4(SP)",     0xCE, 0x7D, 0xCE, 0x0A, 0x04);
    TEST(DIVD,  "-6(FP), 12(SB)",    0xCE, 0xBF, 0xC6, 0x7A, 0x0C);
    TEST(EXTSW, "16(SB), R2, 4, 7",  0xCE, 0x8D, 0xD0, 0x10, 0x86);
    TEST(INSSW, "R2, 16(SB), 4, 7",  0xCE, 0x89, 0x16, 0x10, 0x86);
    TEST(MEIB,  "R1, R0", 0xCE, 0x24, 0x08);
    TEST(MEIW,  "R3, R2", 0xCE, 0xA5, 0x18);
    TEST(MEID,  "R5, R4", 0xCE, 0x27, 0x29);
    TEST(MODB,  "4(SB), 8(SB)",      0xCE, 0xB8, 0xD6, 0x04, 0x08);
    TEST(MOVMW, "10(R0), 16(R1), 4", 0xCE, 0x41, 0x42, 0x0A, 0x10, 0x06);
    TEST(MOVXBW, "2(SB), R0",        0xCE, 0x10, 0xD0, 0x02);
    TEST(MOVZBW, "-4(FP), R0",       0xCE, 0x14, 0xC0, 0x7C);
    TEST(MULW,  "5, R0",             0xCE, 0x21, 0xA0, 0x00, 0x05);
    TEST(MULD,  "4(-4(FP)), 3(SB)",  0xCE, 0xA3, 0x86, 0x7C, 0x04, 0x03);
    TEST(QUOB,  "R0, R7",            0xCE, 0xF0, 0x01);
    TEST(QUOW,  "4(SB), 8(SB)",      0xCE, 0xB1, 0xD6, 0x04, 0x08);
    TEST(REMB,  "4(SB), 8(SB)",      0xCE, 0xB4, 0xD6, 0x04, 0x08);

    ERRR(DEIB, "R1,R3", 0xCE, 0xEC, 0x08);
    ERRR(DEIW, "R3,R1", 0xCE, 0x6D, 0x18);
    ERRR(DEID, "R5,R7", 0xCE, 0xEF, 0x29);
    ERRR(MEIB, "R1,R3", 0xCE, 0xE4, 0x08);
    ERRR(MEIW, "R3,R1", 0xCE, 0x65, 0x18);
    ERRR(MEID, "R5,R7", 0xCE, 0xE7, 0x29);
}

static void test_format_8() {
    TEST(CHECKB, "R0, 4(SB), R2",       0xEE, 0x80, 0xD0, 0x04);
    TEST(CVTP,   "R0, 32(SB), R2",      0x6E, 0x83, 0xD0, 0x20);
    TEST(EXTW,   "R0, 0(R1), R2, 7",    0x2E, 0x81, 0x48, 0x00, 0x07);
    TEST(FFSB,   "-4(FP), TOS",         0x6E, 0xC4, 0xC5, 0x7C);
    TEST(FFSW,   "8(SB), R0",           0x6E, 0x05, 0xD0, 0x08);
    TEST(INDEXB, "R0, 20(SB), -4(FP)",  0x2E, 0x04, 0xD6, 0x14, 0x7C);
    TEST(INSW,   "R0, R2, 0(R1), 7",    0xAE, 0x41, 0x12, 0x00, 0x07);
}

static void test_format_9() {
    TEST(MOVF,  "F1, 8(SB)",  0xBE, 0x85, 0x0E, 0x08);
    TEST(MOVL,  "F2, 8(SB)",  0xBE, 0x84, 0x16, 0x08);
    TEST(MOVBF, "2, F0",      0x3E, 0x04, 0xA0, 0x02);
    TEST(MOVBL, "R1, F2",     0x3E, 0x80, 0x08);
    TEST(MOVWL, "R3, F2",     0x3E, 0x81, 0x18);
    TEST(MOVDL, "R5, F2",     0x3E, 0x83, 0x28);
    TEST(MOVDL, "16(SB), F2", 0x3E, 0x83, 0xD0, 0x10);
    TEST(MOVFL, "8(SB), F0",  0x3E, 0x1B, 0xD0, 0x08);
    TEST(MOVFL, "F1, F2",     0x3E, 0x9B, 0x08);
    TEST(MOVLF, "F0, 12(SB)", 0x3E, 0x96, 0x06, 0x0C);
    TEST(MOVLF, "F0, F3",     0x3E, 0xD6, 0x00);
    TEST(FLOORFB, "F1, R3",   0x3E, 0xFC, 0x08);
    TEST(FLOORLD, "F2, R5",   0x3E, 0x7B, 0x11);
    TEST(ROUNDFB, "F1, R5",   0x3E, 0x64, 0x09);
    TEST(ROUNDLD, "F2, R5",   0x3E, 0x63, 0x11);
    TEST(TRUNCFB, "F1, R3",   0x3E, 0xEC, 0x08);
    TEST(TRUNCLD, "F2, R5",   0x3E, 0x6B, 0x11);
    TEST(LFSR, "R0",  0x3E, 0x0F, 0x00);
    TEST(SFSR, "TOS", 0x3E, 0xF7, 0x05);

    ERRR(MOVL, "F3,8(SB)",   0xBE, 0x84, 0x1E, 0x08);
    ERRR(MOVBL, "R1,F5",     0x3E, 0x40, 0x09);
    ERRR(MOVWL, "R3,F7",     0x3E, 0xC1, 0x19);
    ERRR(MOVDL, "R5,F1",     0x3E, 0x43, 0x28);
    ERRR(MOVDL, "16(SB),F3", 0x3E, 0xC3, 0xD0, 0x10);
    ERRR(MOVFL, "8(SB),F5",  0x3E, 0x5B, 0xD1, 0x08);
    ERRR(MOVFL, "F1,F7",     0x3E, 0xDB, 0x09);
    ERRR(MOVLF, "F1,12(SB)", 0x3E, 0x96, 0x0E, 0x0C);
    ERRR(MOVLF, "F3,F3",     0x3E, 0xD6, 0x18);
    ERRR(FLOORLD, "F5,R5",   0x3E, 0x7B, 0x29);
    ERRR(ROUNDLD, "F7,R5",   0x3E, 0x63, 0x39);
    ERRR(TRUNCLD, "F1,R5",   0x3E, 0x6B, 0x09);
}

static void test_format_11() {
    TEST(ABSF, "F1, F5",     0xBE, 0x75, 0x09);
    TEST(ABSL, "F2, F4",     0xBE, 0x34, 0x11);
    TEST(ADDF, "F3, F7",     0xBE, 0xC1, 0x19);
    TEST(ADDL, "F2, 16(SB)", 0xBE, 0x80, 0x16, 0x10);
    TEST(ADDL, "16(SB), F4", 0xBE, 0x00, 0xD1, 0x10);
    TEST(CMPF, "F1, F3",     0xBE, 0xC9, 0x08);
    TEST(CMPL, "F6, F4",     0xBE, 0x08, 0x31);
    TEST(DIVF, "F1, F7",     0xBE, 0xE1, 0x09);
    TEST(DIVL, "F2, 16(SB)", 0xBE, 0xA0, 0x16, 0x10);
    TEST(DIVL, "-8(FP), F4", 0xBE, 0x20, 0xC1, 0x78);
    TEST(MULF, "F5, F7",     0xBE, 0xF1, 0x29);
    TEST(MULL, "F6, 8(SB)",  0xBE, 0xB0, 0x36, 0x08);
    TEST(MULL, "-8(FP), F0", 0xBE, 0x30, 0xC0, 0x78);
    TEST(NEGF, "F3, F5",     0xBE, 0x55, 0x19);
    TEST(NEGL, "F2, F4",     0xBE, 0x14, 0x11);
    TEST(SUBF, "F1, F7",     0xBE, 0xD1, 0x09);
    TEST(SUBL, "F2, 16(SB)", 0xBE, 0x90, 0x16, 0x10);
    TEST(SUBL, "16(SB), F4", 0xBE, 0x10, 0xD1, 0x10);

    ERRR(ABSL, "F1, F4",     0xBE, 0x34, 0x09);
    ERRR(ABSL, "F2, F3",     0xBE, 0xF4, 0x10);
    ERRR(ADDL, "F1, 16(SB)", 0xBE, 0x80, 0x0E, 0x10);
    ERRR(ADDL, "16(SB), F3", 0xBE, 0xC0, 0xD0, 0x10);
    ERRR(CMPL, "F7, F4",     0xBE, 0x08, 0x39);
    ERRR(CMPL, "F6, F3",     0xBE, 0xC8, 0x30);
    ERRR(DIVL, "F1, 16(SB)", 0xBE, 0xA0, 0x0E, 0x10);
    ERRR(DIVL, "-8(FP), F3", 0xBE, 0xE0, 0xC0, 0x78);
    ERRR(MULL, "F7, 8(SB)",  0xBE, 0xB0, 0x3E, 0x08);
    ERRR(MULL, "-8(FP), F1", 0xBE, 0x70, 0xC0, 0x78);
    ERRR(NEGL, "F1, F4",     0xBE, 0x14, 0x09);
    ERRR(NEGL, "F2, F3",     0xBE, 0xD4, 0x10);
    ERRR(SUBL, "F1, 16(SB)", 0xBE, 0x90, 0x0D, 0x10);
    ERRR(SUBL, "16(SB), F3", 0xBE, 0xD0, 0xD0, 0x10);
}

static void test_format_8_mmu() {
    TEST(MOVSUB, "5(SP), 9(SB)",        0xAE, 0x8C, 0xCE, 0x05, 0x09);
    TEST(MOVUSB, "9(SB), 5(SP)",        0xAE, 0x5C, 0xD6, 0x09, 0x05);
}

static void test_format_14() {
    TEST(LMR, "BPR0, R1", 0x1E, 0x0B, 0x08);
    TEST(LMR, "BPR1, R2", 0x1E, 0x8B, 0x10);
    ERUR(LMR, "mr2, R3",  0x1E, 0x0B, 0x19);
    ERUR(LMR, "mr3, R4",  0x1E, 0x8B, 0x21);
    ERUR(LMR, "mr4, R5",  0x1E, 0x0B, 0x2A);
    ERUR(LMR, "mr5, R6",  0x1E, 0x8B, 0x32);
    ERUR(LMR, "mr6, R7",  0x1E, 0x0B, 0x3B);
    ERUR(LMR, "mr7, R0",  0x1E, 0x8B, 0x03);
    ERUR(LMR, "mr8, R1",  0x1E, 0x0B, 0x0C);
    ERUR(LMR, "mr9, R2",  0x1E, 0x8B, 0x14);
    TEST(LMR, "MSR, R3",  0x1E, 0x0B, 0x1D);
    TEST(LMR, "BCNT, R4", 0x1E, 0x8B, 0x25);
    TEST(LMR, "PTB0, R5", 0x1E, 0x0B, 0x2E);
    TEST(LMR, "PTB1, R6", 0x1E, 0x8B, 0x36);
    ERUR(LMR, "mr14, R7", 0x1E, 0x0B, 0x3F);
    TEST(LMR, "EIA, R0",  0x1E, 0x8B, 0x07);

    TEST(SMR, "BPR0, R1", 0x1E, 0x0F, 0x08);
    TEST(SMR, "BPR1, R2", 0x1E, 0x8F, 0x10);
    ERUR(SMR, "mr2, R3",  0x1E, 0x0F, 0x19);
    ERUR(SMR, "mr3, R4",  0x1E, 0x8F, 0x21);
    ERUR(SMR, "mr4, R5",  0x1E, 0x0F, 0x2A);
    ERUR(SMR, "mr5, R6",  0x1E, 0x8F, 0x32);
    ERUR(SMR, "mr6, R7",  0x1E, 0x0F, 0x3B);
    ERUR(SMR, "mr7, R0",  0x1E, 0x8F, 0x03);
    ERUR(SMR, "mr8, R1",  0x1E, 0x0F, 0x0C);
    ERUR(SMR, "mr9, R2",  0x1E, 0x8F, 0x14);
    TEST(SMR, "MSR, R3",  0x1E, 0x0F, 0x1D);
    TEST(SMR, "BCNT, R4", 0x1E, 0x8F, 0x25);
    TEST(SMR, "PTB0, R5", 0x1E, 0x0F, 0x2E);
    TEST(SMR, "PTB1, R6", 0x1E, 0x8F, 0x36);
    ERUR(SMR, "mr14, R7", 0x1E, 0x0F, 0x3F);
    TEST(SMR, "EIA, R0",  0x1E, 0x8F, 0x07);

    TEST(RDVAL, "0x0200(R0)", 0x1E, 0x03, 0x40, 0x82, 0x00);
    TEST(WRVAL, "0x0200(R0)", 0x1E, 0x07, 0x40, 0x82, 0x00);
}

static void test_generic_addressing() {
    // Register
    TEST(ADDW, "R1, R2", 0x81, 0x08);
    TEST(ADDF, "F1, F2", 0xBE, 0x81, 0x08);
    // Register Relative
    TEST(ADDW, "4(R1), R2",     0x81, 0x48, 0x04);
    TEST(ADDW, "4(R1), 32(R2)", 0x81, 0x4A, 0x04, 0x80, 0x20);
    TEST(ADDW, "R1, 32(R2)",    0x81, 0x0A, 0x80, 0x20);
    // Memory Relative
    TEST(ADDW, "6(4(FP)), R2",         0x81, 0x80, 0x04, 0x06);
    TEST(ADDW, "6(4(FP)), 16(32(SB))", 0x81, 0x84, 0x04, 0x06, 0x80, 0x20, 0x10);
    TEST(ADDW, "R2, 16(32(SB))",       0x81, 0x14, 0x80, 0x20, 0x10);
    // Immediate
    TEST(ADDB, "0x56, R1",       0x40, 0xA0, 0x56);
    TEST(ADDW, "0x1234, R1",     0x41, 0xA0, 0x12, 0x34);
    TEST(ADDD, "0x12345678, R1", 0x43, 0xA0, 0x12, 0x34, 0x56, 0x78);
    EROA(ADDB, "R1, imm", 0x00, 0x0D);
    EROA(ADDW, "R1, imm", 0x01, 0x0D);
    EROA(ADDD, "R1, imm", 0x03, 0x0D);
    TEST(ADDF, "0x12345678, F1", 0xBE, 0x41, 0xA0, 0x12, 0x34, 0x56, 0x78);
    TEST(ADDL, "0x12345678:0x9ABCDEF0, F2",
         0xBE, 0x80, 0xA0, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0);
    EROA(ADDF, "F1, imm", 0xBE, 0x01, 0x05);
    EROA(ADDL, "F2, imm", 0xBE, 0x00, 0x15);

    // Absolute
    TEST(ADDW, "@0x1234, 4(R2)",   0x81, 0xAA, 0x92, 0x34, 0x04);
    TEST(ADDW, "@0x1234, @0x00005678", 0x41, 0xAD, 0x92, 0x34, 0xC0, 0x00, 0x56, 0x78);
    TEST(ADDW, "4(R2), @0x00005678",   0x41, 0x55, 0x04, 0xC0, 0x00, 0x56, 0x78);
    // External
    TEST(ADDW, "EXT(2)+4, 10(R2)",           0x81, 0xB2, 0x02, 0x04, 0x0A);
    TEST(ADDW, "EXT(-2)+(-4), 10(R2)",       0x81, 0xB2, 0x7E, 0x7C, 0x0A);
    TEST(ADDW, "EXT(-2)+4, EXT(-6)+8",       0x81, 0xB5, 0x7E, 0x04, 0x7A, 0x08);
    TEST(ADDW, "EXT(2)+(-4), EXT(6)+(-8)",   0x81, 0xB5, 0x02, 0x7C, 0x06, 0x78);
    TEST(ADDW, "10(R2), EXT(6)+8",           0x81, 0x55, 0x0A, 0x06, 0x08);
    TEST(ADDW, "10(R2), EXT(-6)+(-8)",       0x81, 0x55, 0x0A, 0x7A, 0x78);
    // Top of Stack
    TEST(ADDW, "TOS, 4(R2)",   0x81, 0xBA, 0x04);
    TEST(ADDW, "TOS, TOS",     0xC1, 0xBD);
    TEST(ADDW, "4(R2), TOS",   0xC1, 0x55, 0x04);
    // Memory Space
    TEST(ADDW, "4(FP), 6(R2)",      0x81, 0xC2, 0x04, 0x06);
    TEST(ADDW, "4(SP), 6(R2)",      0x81, 0xCA, 0x04, 0x06);
    TEST(ADDW, "4(SB), 6(R2)",      0x81, 0xD2, 0x04, 0x06);
    TEST(ADDW, ".+4(PC), 6(R2)",    0x81, 0xDA, 0x04, 0x06);
    TEST(ADDW, "6(R2), 4(FP)",      0x01, 0x56, 0x06, 0x04);
    TEST(ADDW, "6(R2), 4(SP)",      0x41, 0x56, 0x06, 0x04);
    TEST(ADDW, "6(R2), 4(SB)",      0x81, 0x56, 0x06, 0x04);
    TEST(ADDW, "6(R2), .+4(PC)",    0xC1, 0x56, 0x06, 0x04);
    TEST(ADDW, "4(FP), 6(SB)",      0x81, 0xC6, 0x04, 0x06);
    TEST(ADDW, "6(SB), 8(SP)",      0x41, 0xD6, 0x06, 0x08);
    TEST(ADDW, "8(SP), .+10(PC)",   0xC1, 0xCE, 0x08, 0x0A);
    TEST(ADDW, ".+2(PC), .+12(PC)", 0xC1, 0xDE, 0x02, 0x0C);
    // Scaled Indexing
    TEST(ADDW, "0(R1)[R3:B], 8(R2)",          0x81, 0xE2, 0x0B, 0x08);
    TEST(ADDW, "4(R1)[R3:B], 8(R2)",          0x81, 0xE2, 0x4B, 0x04, 0x08);
    TEST(ADDW, "6(4(FP))[R3:B], 8(R2)",       0x81, 0xE2, 0x83, 0x04, 0x06, 0x08);
    TEST(ADDW, "@0x1234[R3:B], 8(R2)",        0x81, 0xE2, 0xAB, 0x92, 0x34, 0x08);
    TEST(ADDW, "EXT(4)+6[R3:B], 8(R2)",       0x81, 0xE2, 0xB3, 0x04, 0x06, 0x08);
    TEST(ADDW, "TOS[R3:B], 8(R2)",            0x81, 0xE2, 0xBB, 0x08);
    TEST(ADDW, "4(SP)[R3:B], 8(R2)",          0x81, 0xE2, 0xCB, 0x04, 0x08);
    TEST(ADDW, ".+4(PC)[R3:B], 8(R2)",        0x81, 0xE2, 0xDB, 0x04, 0x08);
    TEST(ADDW, "8(R2), 0(R1)[R3:W]",          0x41, 0x57, 0x0B, 0x08);
    TEST(ADDW, "8(R2), 4(R1)[R3:W]",          0x41, 0x57, 0x4B, 0x08, 0x04);
    TEST(ADDW, "8(R2), 6(4(FP))[R3:W]",       0x41, 0x57, 0x83, 0x08, 0x04, 0x06);
    TEST(ADDW, "8(R2), @0x1234[R3:W]",        0x41, 0x57, 0xAB, 0x08, 0x92, 0x34);
    TEST(ADDW, "8(R2), EXT(4)+6[R3:W]",       0x41, 0x57, 0xB3, 0x08, 0x04, 0x06);
    TEST(ADDW, "8(R2), TOS[R3:W]",            0x41, 0x57, 0xBB, 0x08);
    TEST(ADDW, "8(R2), 4(SP)[R3:W]",          0x41, 0x57, 0xCB, 0x08, 0x04);
    TEST(ADDW, "8(R2), .+4(PC)[R3:W]",        0x41, 0x57, 0xDB, 0x08, 0x04);
    TEST(ADDW, "8(R2)[R3:D], 0(R1)[R5:W]",    0x41, 0xF7, 0x53, 0x0D, 0x08);
    TEST(ADDW, "8(R2)[R3:D], 4(R1)[R5:W]",    0x41, 0xF7, 0x53, 0x4D, 0x08, 0x04);
    TEST(ADDW, "8(R2)[R3:D], 6(4(FP))[R5:W]", 0x41, 0xF7, 0x53, 0x85, 0x08, 0x04, 0x06);
    TEST(ADDW, "8(R2)[R3:D], @0x1234[R5:W]",  0x41, 0xF7, 0x53, 0xAD, 0x08, 0x92, 0x34);
    TEST(ADDW, "8(R2)[R3:Q], EXT(4)+6[R5:W]", 0x41, 0xFF, 0x53, 0xB5, 0x08, 0x04, 0x06);
    TEST(ADDW, "8(R2)[R3:Q], TOS[R5:W]",      0x41, 0xFF, 0x53, 0xBD, 0x08);
    TEST(ADDW, "8(R2)[R3:Q], 4(SP)[R5:W]",    0x41, 0xFF, 0x53, 0xCD, 0x08, 0x04);
    TEST(ADDW, "8(R2)[R3:Q], .+4(PC)[R5:W]",  0x41, 0xFF, 0x53, 0xDD, 0x08, 0x04);

    TEST(EXTD, "R1, 0x00112233(0x00334455(FP))[R3:W], 0x00556677(0x00778899(SB))[R4:W], 32",
         // 11101|111 01|001|0|11 00101111
         // 10000 011 10010 100
         0x2E, 0x4B, 0xEF,
         0x83, 0x94,
         0xC0, 0x33, 0x44, 0x55,
         0xC0, 0x11, 0x22, 0x33,
         0xC0, 0x77, 0x88, 0x99,
         0xC0, 0x55, 0x66, 0x77,
         0x20);
}
// clang-format on

static void assert_illegal(
        const char *file, int line, uint8_t post, uint8_t opc, uint8_t prefix) {
    __VASSERT(
            file, line, UNKNOWN_INSTRUCTION, 0x0000, _, "", prefix, opc, post);
}

static void assert_illegal(
        const char *file, int line, uint8_t opc, uint8_t prefix) {
    __VASSERT(file, line, UNKNOWN_INSTRUCTION, 0x0000, _, "", prefix, opc);
}

static void assert_illegal(const char *file, int line, uint8_t opc) {
    __VASSERT(file, line, UNKNOWN_INSTRUCTION, 0x0000, _, "", opc);
}

#define ILLEGAL(...) assert_illegal(__FILE__, __LINE__, __VA_ARGS__)

static void test_illegal() {
    // Format 0: |cond|1010|
    ILLEGAL(0xFA);

    // Format 3: |_gen_|_op| |011111|ii|
    for (uint8_t gen = 0x00; gen <= 0x1F; gen++) {
        for (uint8_t ii = 0; ii <= 3; ii++) {
            static const uint8_t opc[] = {
                    0x1, 0x3, 0x5, 0x7, 0x8, 0x9, 0xB, 0xD, 0xF};
            for (uint8_t i = 0; i < sizeof(opc); i++) {
                if (ii != 2)
                    ILLEGAL((gen << 3) | (opc[i] >> 1),
                            (opc[i] << 7) | 0x7C | ii);
            }
            if (ii == 3) {
                ILLEGAL((gen << 3) | (0x2 >> 1),
                        (0x2 << 7) | 0x7C | ii);  // BICPSR
                ILLEGAL((gen << 3) | (0x6 >> 1),
                        (0x6 << 7) | 0x7C | ii);  // BISPSR
            }
            if (ii != 3) {
                ILLEGAL((gen << 3) | (0x0 >> 1),
                        (0x0 << 7) | 0x7C | ii);  // CXPD
                ILLEGAL((gen << 3) | (0x4 >> 1),
                        (0x4 << 7) | 0x7C | ii);  // JUMP
                ILLEGAL((gen << 3) | (0xC >> 1),
                        (0xC << 7) | 0x7C | ii);  // JSR
            }
        }
    }

    // Format 4: |gen1_|gen| |2_|_op_|ii|
    for (uint8_t gen2 = 0x00; gen2 <= 0x1F; gen2++) {
        for (uint8_t ii = 0; ii <= 3; ii++) {
            if (ii != 3)
                ILLEGAL((gen2 << 6) | (0x9 << 2) | ii, 0x0E);  // ADDR
        }
    }

    // Format 5: |00000|sho| |r|0|_op_|ii| |0000|1110|
    for (uint8_t ii = 0; ii <= 3; ii++) {
        for (uint8_t data = 0; data <= 0xF; data++) {
            for (uint8_t opc = 0x4; opc <= 0xF; opc++) {
                ILLEGAL(data >> 1, (data << 7) | (opc << 2) | ii, 0x0E);
            }
            if (ii != 3)
                ILLEGAL(data >> 1, (data << 7) | (0x2 << 2) | ii,
                        0x0E);  // SETCFG
        }
        if (ii == 2) {
            ILLEGAL(0x00 | (0x0 << 2) | ii, 0x0E);  // MOVSi
            ILLEGAL(0x00 | (0x1 << 2) | ii, 0x0E);  // CMPSi
            ILLEGAL(0x00 | (0x3 << 2) | ii, 0x0E);  // SKPSi
        }
        if (ii != 0) {
            ILLEGAL(0x80 | (0x0 << 2) | ii, 0x0E);  // MOVST
            ILLEGAL(0x80 | (0x1 << 2) | ii, 0x0E);  // CMPST
            ILLEGAL(0x80 | (0x3 << 2) | ii, 0x0E);  // SKPST
        }
    }

    // Format 6: |gen1_|gen| |2_|_op_|ii|0100|1110|
    for (uint8_t gen2 = 0x00; gen2 <= 0x1F; gen2++) {
        for (uint8_t ii = 0; ii <= 3; ii++) {
            static const uint8_t opc[] = {0x4, 0xA};
            for (uint8_t i = 0; i < sizeof(opc); i++) {
                ILLEGAL((gen2 << 6) | (opc[i] << 2) | ii, 0x4E);
            }
        }
    }

    // Format 7: |gen1_|gen| |2_|_op_|ii| |1100|1110|
    for (uint8_t gen2 = 0x00; gen2 <= 0x1F; gen2++) {
        for (uint8_t ii = 0; ii <= 3; ii++) {
            ILLEGAL((gen2 << 6) | (0xA << 2) | ii, 0xCE);
            if (ii != 0) {
                ILLEGAL((gen2 << 6) | (0x4 << 2) | ii, 0xCE);  // MOVXBW
                ILLEGAL((gen2 << 6) | (0x5 << 2) | ii, 0xCE);  // MOVZBW
            }
            if (ii == 2 || ii == 3) {
                ILLEGAL((gen2 << 6) | (0x6 << 2) | ii, 0xCE);  // MOVZiD
                ILLEGAL((gen2 << 6) | (0x7 << 2) | ii, 0xCE);  // MOVXiD
            }
        }
    }

    // Format 8: |gen1_|gen| |2_|reg|o|ii| |op|10|1110|
    for (uint8_t gen2 = 0x00; gen2 <= 0x1F; gen2++) {
        for (uint8_t ii = 0; ii <= 3; ii++) {
            for (uint8_t reg = 0; reg <= 7; reg++) {
                if (reg != 1 && reg != 3) {
                    uint8_t opc = 6;
                    ILLEGAL((gen2 << 6) | (reg << 3) | (opc & 4) | ii,
                            (opc << 6) | 0x2E);  // MOVSUi/MOVUSi
                }
                if (ii != 3) {
                    uint8_t opc = 1;
                    ILLEGAL((gen2 << 6) | (reg << 3) | (opc & 4) | ii,
                            (opc << 6) | 0x6E);  // CVP
                }
            }
        }
    }

    // Format 9: |gen1_|gen| |2_|_op|f|ii| |0011|1110|
    for (uint8_t gen2 = 0x00; gen2 <= 0x1F; gen2++) {
        for (uint8_t ii = 0; ii <= 3; ii++) {
            for (uint8_t f = 0; f <= 4; f += 4) {
                if (ii != 3) {
                    ILLEGAL((gen2 << 6) | (1 << 3) | f | ii, 0x3E);  // LFSR
                    ILLEGAL((gen2 << 6) | (6 << 3) | f | ii, 0x3E);  // SFSR
                }
            }
            if (ii != 3)
                ILLEGAL((gen2 << 6) | (2 << 3) | 0 | ii, 0x3E);  // MOVLF
            if (ii != 2)
                ILLEGAL((gen2 << 6) | (3 << 3) | 4 | ii, 0x3E);  // MOVFL
        }
    }

    // Format 10
    ILLEGAL(0x7E);

    // Format 11: |gen1_|gen| |2_|_op_|0f| |1011|1110|
    for (int8_t gen2 = 0x00; gen2 <= 0x1F; gen2++) {
        static uint8_t opc[] = {0x3, 0x6, 0x7, 0x9, 0xA, 0xB, 0xE, 0xF};
        for (uint8_t i = 0; i < sizeof(opc); i++) {
            for (uint8_t f = 0; f <= 1; f++) {
                ILLEGAL((gen2 << 6) | (opc[i] << 2) | f, 0xBE);
            }
        }
    }

    // Format 12
    ILLEGAL(0xFE);

    // Format 13
    ILLEGAL(0x9E);

    // Format 14: |gen1_|sho| |t|0|_op_|ii| |0001|1110|
    for (uint8_t ii = 0; ii <= 3; ii++) {
        for (uint8_t data = 0x0; data <= 0xF; data++) {
            if (ii != 3) {
                ILLEGAL((data >> 1), (data << 7) | (2 << 2) | ii, 0x1E);  // LMR
                ILLEGAL((data >> 1), (data << 7) | (3 << 2) | ii, 0x1E);  // SMR
            }
            for (uint8_t opc = 0x4; opc <= 0xF; opc++) {
                ILLEGAL((data >> 1), (data << 7) | (opc << 2) | ii, 0x1E);
            }
        }
        if (ii != 3) {
            ILLEGAL(0x80 | (0 << 2) | ii, 0x1E);  // RDVAL
            ILLEGAL(0x80 | (1 << 2) | ii, 0x1E);  // WRVAL
        }
    }

    // Format 15
    for (uint8_t nnn = 0; nnn <= 7; nnn++) {
        ILLEGAL((nnn << 5) | 0x16);
    }
}

const char *run_cpu_test() {
    RUN_TEST(test_cpu);
    return disassembler.listCpu();
}

void run_tests(const char *cpu) {
    disassembler.setCpu(cpu);
    RUN_TEST(test_format_0);
    RUN_TEST(test_format_1);
    RUN_TEST(test_format_2);
    RUN_TEST(test_format_3);
    RUN_TEST(test_format_4);
    RUN_TEST(test_format_5);
    RUN_TEST(test_format_6);
    RUN_TEST(test_format_7);
    RUN_TEST(test_format_8);
    RUN_TEST(test_format_9);
    RUN_TEST(test_format_11);
    RUN_TEST(test_format_8_mmu);
    RUN_TEST(test_format_14);
    RUN_TEST(test_generic_addressing);
    RUN_TEST(test_illegal);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

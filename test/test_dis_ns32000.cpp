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
#include "table_ns32000.h"
#include "test_dis_helper.h"

using namespace libasm;
using namespace libasm::ns32000;
using namespace libasm::test;

DisNs32000 dis32k;
Disassembler &disassembler(dis32k);

#if defined(LIBASM_DIS_NOFLOAT)
#define TFLT(insn, opr, hex, ...) ERRT(insn, hex, FLOAT_NOT_SUPPORTED, hex, __VA_ARGS__)
#define EFLT(insn, opr, error, at, hex, ...) ERRT(insn, hex, FLOAT_NOT_SUPPORTED, hex, __VA_ARGS__)
#else
#define TFLT(insn, opr, hex, ...) TEST(insn, opr, __VA_ARGS__)
#define EFLT(insn, opr, error, at, hex, ...) ERRT(insn, opr, error, at, __VA_ARGS__)
#endif

void set_up() {
    disassembler.reset();
    disassembler.setOption("relative", "enable");
    disassembler.setOption("c-style", "enable");
}

void tear_down() {
    symtab.reset();
}

// clang-format off
void test_cpu() {
    EQUALS("cpu ns32032", true,    disassembler.setCpu("NS32032"));
    EQUALS_P("cpu ns32032", "32032", disassembler.config().cpu_P());

    EQUALS("cpu 32032", true,    disassembler.setCpu("32032"));
    EQUALS_P("cpu 32032", "32032", disassembler.config().cpu_P());
}

void test_format_0() {
    disassembler.setOption("relative", "on");
    ATEST(0x001000, "BEQ", ".",      0x0A, 0x00);
    ATEST(0x001000, "BNE", ".+10",   0x1A, 0x0A);
    ATEST(0x001000, "BCS", ".-64",   0x2A, 0x40);
    ATEST(0x010000, "BCC", ".+8191", 0x3A, 0x9F, 0xFF);
    ATEST(0x010000, "BHI", ".-8192", 0x4A, 0xA0, 0x00);
    ATEST(0x800000, "BLS", ".+0x007FFFFF", 0x5A, 0xC0, 0x7F, 0xFF, 0xFF);
    ATEST(0x800000, "BGT", ".-0x00800000", 0x6A, 0xFF, 0x80, 0x00, 0x00);
    ATEST(0x000000, "BLS", ".+0x00FFFFFF", 0x5A, 0xC0, 0xFF, 0xFF, 0xFF);
    ATEST(0xFFFFFF, "BGT", ".-0x00FFFFFF", 0x6A, 0xFF, 0x00, 0x00, 0x01);
    AERRT(0x000000, "BLE", ".+0x01000000", OVERFLOW_RANGE, ".+0x01000000", 0x7A, 0xC1, 0x00, 0x00, 0x00);
    AERRT(0xFFFFFF, "BFS", ".-0x01000000", OVERFLOW_RANGE, ".-0x01000000", 0x8A, 0xFF, 0x00, 0x00, 0x00);
    AERRT(0x000000, "BLE", ".+0x1FFFFFFF", OVERFLOW_RANGE, ".+0x1FFFFFFF", 0x7A, 0xDF, 0xFF, 0xFF, 0xFF);
    AERRT(0xFFFFFF, "BFS", ".-0x1F000000", OVERFLOW_RANGE, ".-0x1F000000", 0x8A, 0xE1, 0x00, 0x00, 0x00);
    ATEST(0x001000, "BFC", ".+2",    0x9A, 0x02);
    ATEST(0x001000, "BLO", ".-154",  0xAA, 0xBF, 0x66);
    ATEST(0x001000, "BHS", ".+63",   0xBA, 0x3F);
    ATEST(0x001000, "BLT", ".-128",  0xCA, 0xBF, 0x80);
    ATEST(0x001000, "BGE", ".+256",  0xDA, 0x81, 0x00);
    ATEST(0x001000, "BR",  ".+10",   0xEA, 0x0A);
    ATEST(0x001000, "BR",  ".-154",  0xEA, 0xBF, 0x66);
    ANMEM(0x000000, "BLS", ".+0x00FFFF00", ".+0x00FFFF00", 0x5A, 0xC0, 0xFF, 0xFF);
    ANMEM(0x000000, "BLS", ".+0x00FF0000", ".+0x00FF0000", 0x5A, 0xC0, 0xFF);
    ANMEM(0x000000, "BLS", ".",            ".",            0x5A, 0xC0);
}

void test_format_1() {
    TEST("BSR", ".+16", 0x02, 0x10);

    TEST("RET",  "16", 0x12, 0x10);
    TEST("RETT", "16", 0x42, 0x10);
    TEST("RETI", "",   0x52);
    TEST("NOP",  "",   0xA2);
    TEST("WAIT", "",   0xB2);
    TEST("DIA",  "",   0xC2);
    TEST("FLAG", "",   0xD2);
    TEST("SVC",  "",   0xE2);
    TEST("BPT",  "",   0xF2);

    TEST("CXP", "1",  0x22, 0x01);
    TEST("RXP", "16", 0x32, 0x10);

    TEST("SAVE",    "[R0,R2,R7]",                     0x62, 0x85);
    ERRT("SAVE",    "[]", OPCODE_HAS_NO_EFFECT, "[]", 0x62, 0x00);
    TEST("RESTORE", "[R0,R2,R7]",                     0x72, 0xA1);
    ERRT("RESTORE", "[]", OPCODE_HAS_NO_EFFECT, "[]", 0x72, 0x00);
    TEST("ENTER",   "[R0,R2,R7], 16", 0x82, 0x85, 0x10);
    TEST("EXIT",    "[R0,R2,R7]",     0x92, 0xA1);

    // Various displacement.
    TEST("RET", "0",                             0x12, 0x00);
    TEST("RET", "63",                            0x12, 0x3F);
    TEST("RET", "-64",                           0x12, 0x40);
    TEST("RET", "0x1FFF",                        0x12, 0x9F, 0xFF);
    TEST("RET", "-0x2000",                       0x12, 0xA0, 0x00);
    TEST("RET", "0x1FFFFFFF",                    0x12, 0xDF, 0xFF, 0xFF, 0xFF);
    TEST("RET", "-0x1F000000",                   0x12, 0xE1, 0x00, 0x00, 0x00);
    ERRT("RET", "-0x20000000", ILLEGAL_CONSTANT, "-0x20000000", 0x12, 0xE0, 0x00, 0x00, 0x00);
}

void test_format_2() {
    TEST("ADDQB", "-8, R0", 0x0C, 0x04);
    TEST("ADDQW", "7, R1",  0x8D, 0x0B);
    TEST("ADDQD", "-8, R2", 0x0F, 0x14);
    TEST("CMPQB", "-8, R0", 0x1C, 0x04);
    TEST("CMPQW", "7, R1",  0x9D, 0x0B);
    TEST("CMPQD", "-8, R2", 0x1F, 0x14);
    TEST("CMPQB", "-8, 18",         0x1C, 0xA4, 0x12);
    TEST("CMPQW", "7, 0x1234",      0x9D, 0xA3, 0x12, 0x34);
    TEST("CMPQD", "-8, 0x12345678", 0x1F, 0xA4, 0x12, 0x34, 0x56, 0x78);
    TEST("MOVQB", "0, R0",  0x5C, 0x00);
    TEST("MOVQW", "7, TOS", 0xDD, 0xBB);
    TEST("MOVQD", "-8, R2", 0x5F, 0x14);

    ATEST(0x1000, "ACBB",  "-1, R0, .-3", 0xCC, 0x07, 0x7D);
    ATEST(0x1000, "ACBW",  "-2, R1, .-2", 0x4D, 0x0F, 0x7E);
    ATEST(0x1000, "ACBD",  "4, R2, .+2",  0x4F, 0x12, 0x02);

    TEST("LPRB",    "UPSR, R1",                           0x6C, 0x08);
    ERRT("LPRB",        ", R2", UNKNOWN_REGISTER, ", R2", 0xEC, 0x10);
    ERRT("LPRB",        ", R3", UNKNOWN_REGISTER, ", R3", 0x6C, 0x19);
    ERRT("LPRB",        ", R4", UNKNOWN_REGISTER, ", R4", 0xEC, 0x21);
    ERRT("LPRB",        ", R5", UNKNOWN_REGISTER, ", R5", 0x6C, 0x2A);
    ERRT("LPRB",        ", R6", UNKNOWN_REGISTER, ", R6", 0xEC, 0x32);
    ERRT("LPRB",        ", R7", UNKNOWN_REGISTER, ", R7", 0x6C, 0x3B);
    ERRT("LPRB",        ", R0", UNKNOWN_REGISTER, ", R0", 0xEC, 0x03);
    TEST("LPRB",      "FP, R2",                           0x6C, 0x14);
    TEST("LPRB",      "SP, R3",                           0xEC, 0x1C);
    TEST("LPRB",      "SB, R4",                           0x6C, 0x25);
    ERRT("LPRB",        ", R1", UNKNOWN_REGISTER, ", R1", 0xEC, 0x0D);
    ERRT("LPRB",        ", R2", UNKNOWN_REGISTER, ", R2", 0x6C, 0x16);
    TEST("LPRB",     "PSR, R5",                           0xEC, 0x2E);
    TEST("LPRB", "INTBASE, R6",                           0x6C, 0x37);
    TEST("LPRB",     "MOD, R7",                           0xEC, 0x3F);

    TEST("LPRW",    "UPSR, R1",                           0x6D, 0x08);
    ERRT("LPRW",        ", R2", UNKNOWN_REGISTER, ", R2", 0xED, 0x10);
    ERRT("LPRW",        ", R3", UNKNOWN_REGISTER, ", R3", 0x6D, 0x19);
    ERRT("LPRW",        ", R4", UNKNOWN_REGISTER, ", R4", 0xED, 0x21);
    ERRT("LPRW",        ", R5", UNKNOWN_REGISTER, ", R5", 0x6D, 0x2A);
    ERRT("LPRW",        ", R6", UNKNOWN_REGISTER, ", R6", 0xED, 0x32);
    ERRT("LPRW",        ", R7", UNKNOWN_REGISTER, ", R7", 0x6D, 0x3B);
    ERRT("LPRW",        ", R0", UNKNOWN_REGISTER, ", R0", 0xED, 0x03);
    TEST("LPRW",      "FP, R2",                           0x6D, 0x14);
    TEST("LPRW",      "SP, R3",                           0xED, 0x1C);
    TEST("LPRW",      "SB, R4",                           0x6D, 0x25);
    ERRT("LPRW",        ", R1", UNKNOWN_REGISTER, ", R1", 0xED, 0x0A);
    ERRT("LPRW",        ", R2", UNKNOWN_REGISTER, ", R2", 0x6D, 0x16);
    TEST("LPRW",     "PSR, R5",                           0xED, 0x2E);
    TEST("LPRW", "INTBASE, R6",                           0x6D, 0x37);
    TEST("LPRW",     "MOD, 4(SB)",                        0xED, 0xD7, 0x04);

    TEST("LPRD",    "UPSR, R1",                           0x6F, 0x08);
    ERRT("LPRD",        ", R1", UNKNOWN_REGISTER, ", R1", 0xEF, 0x08);
    ERRT("LPRD",        ", R2", UNKNOWN_REGISTER, ", R2", 0x6F, 0x11);
    ERRT("LPRD",        ", R3", UNKNOWN_REGISTER, ", R3", 0xEF, 0x19);
    ERRT("LPRD",        ", R4", UNKNOWN_REGISTER, ", R4", 0x6F, 0x22);
    ERRT("LPRD",        ", R5", UNKNOWN_REGISTER, ", R5", 0xEF, 0x2A);
    ERRT("LPRD",        ", R6", UNKNOWN_REGISTER, ", R6", 0x6F, 0x33);
    ERRT("LPRD",        ", R7", UNKNOWN_REGISTER, ", R7", 0xEF, 0x3B);
    TEST("LPRD",      "FP, R2",                           0x6F, 0x14);
    TEST("LPRD",      "SP, R3",                           0xEF, 0x1C);
    TEST("LPRD",      "SB, R4",                           0x6F, 0x25);
    ERRT("LPRD",        ", R1", UNKNOWN_REGISTER, ", R1", 0xEF, 0x0A);
    ERRT("LPRD",        ", R2", UNKNOWN_REGISTER, ", R2", 0x6F, 0x16);
    TEST("LPRD",     "PSR, R5",                           0xEF, 0x2E);
    TEST("LPRD", "INTBASE, R6",                           0x6F, 0x37);
    TEST("LPRD",     "MOD, 4(SB)",                        0xEF, 0xD7, 0x04);

    TEST("SPRB",    "UPSR, R1",                           0x2C, 0x08);
    ERRT("SPRB",        ", R0", UNKNOWN_REGISTER, ", R0", 0xAC, 0x00);
    ERRT("SPRB",        ", R0", UNKNOWN_REGISTER, ", R0", 0x2C, 0x01);
    ERRT("SPRB",        ", R0", UNKNOWN_REGISTER, ", R0", 0xAC, 0x01);
    ERRT("SPRB",        ", R0", UNKNOWN_REGISTER, ", R0", 0x2C, 0x02);
    ERRT("SPRB",        ", R0", UNKNOWN_REGISTER, ", R0", 0xAC, 0x02);
    ERRT("SPRB",        ", R0", UNKNOWN_REGISTER, ", R0", 0x2C, 0x03);
    ERRT("SPRB",        ", R0", UNKNOWN_REGISTER, ", R0", 0xAC, 0x03);
    TEST("SPRB",      "FP, R2",                           0x2C, 0x14);
    TEST("SPRB",      "SP, R3",                           0xAC, 0x1C);
    TEST("SPRB",      "SB, R4",                           0x2C, 0x25);
    ERRT("SPRB",        ", R0", UNKNOWN_REGISTER, ", R0", 0xAC, 0x05);
    ERRT("SPRB",        ", R0", UNKNOWN_REGISTER, ", R0", 0x2C, 0x06);
    TEST("SPRB",     "PSR, R5",                           0xAC, 0x2E);
    TEST("SPRB", "INTBASE, R6",                           0x2C, 0x37);
    TEST("SPRB",     "MOD, R7",                           0xAC, 0x3F);

    TEST("SPRW",    "UPSR, R1",                           0x2D, 0x08);
    ERRT("SPRW",        ", R0", UNKNOWN_REGISTER, ", R0", 0xAD, 0x00);
    ERRT("SPRW",        ", R0", UNKNOWN_REGISTER, ", R0", 0x2D, 0x01);
    ERRT("SPRW",        ", R0", UNKNOWN_REGISTER, ", R0", 0xAD, 0x01);
    ERRT("SPRW",        ", R0", UNKNOWN_REGISTER, ", R0", 0x2D, 0x02);
    ERRT("SPRW",        ", R0", UNKNOWN_REGISTER, ", R0", 0xAD, 0x02);
    ERRT("SPRW",        ", R0", UNKNOWN_REGISTER, ", R0", 0x2D, 0x03);
    ERRT("SPRW",        ", R0", UNKNOWN_REGISTER, ", R0", 0xAD, 0x03);
    TEST("SPRW",      "FP, R2",                           0x2D, 0x14);
    TEST("SPRW",      "SP, R3",                           0xAD, 0x1C);
    TEST("SPRW",      "SB, R4",                           0x2D, 0x25);
    ERRT("SPRW",        ", R0", UNKNOWN_REGISTER, ", R0", 0xAD, 0x05);
    ERRT("SPRW",        ", R0", UNKNOWN_REGISTER, ", R0", 0x2D, 0x06);
    TEST("SPRW",     "PSR, R5",                           0xAD, 0x2E);
    TEST("SPRW", "INTBASE, R6",                           0x2D, 0x37);
    TEST("SPRW",     "MOD, 4(SB)",                        0xAD, 0xD7, 0x04);

    TEST("SPRD",    "UPSR, R1",                           0x2F, 0x08);
    ERRT("SPRD",        ", R0", UNKNOWN_REGISTER, ", R0", 0xAF, 0x00);
    ERRT("SPRD",        ", R0", UNKNOWN_REGISTER, ", R0", 0x2F, 0x01);
    ERRT("SPRD",        ", R0", UNKNOWN_REGISTER, ", R0", 0xAF, 0x01);
    ERRT("SPRD",        ", R0", UNKNOWN_REGISTER, ", R0", 0x2F, 0x02);
    ERRT("SPRD",        ", R0", UNKNOWN_REGISTER, ", R0", 0xAF, 0x02);
    ERRT("SPRD",        ", R0", UNKNOWN_REGISTER, ", R0", 0x2F, 0x03);
    ERRT("SPRD",        ", R0", UNKNOWN_REGISTER, ", R0", 0xAF, 0x03);
    TEST("SPRD",      "FP, R2",                           0x2F, 0x14);
    TEST("SPRD",      "SP, R3",                           0xAF, 0x1C);
    TEST("SPRD",      "SB, R4",                           0x2F, 0x25);
    ERRT("SPRD",        ", R0", UNKNOWN_REGISTER, ", R0", 0xAF, 0x05);
    ERRT("SPRD",        ", R0", UNKNOWN_REGISTER, ", R0", 0x2F, 0x06);
    TEST("SPRD",     "PSR, R5",                           0xAF, 0x2E);
    TEST("SPRD", "INTBASE, R6",                           0x2F, 0x37);
    TEST("SPRD",     "MOD, 4(SB)",                        0xAF, 0xD7, 0x04);

    TEST("SEQB", "R0",           0x3C, 0x00);
    TEST("SNEB", "R1",           0xBC, 0x08);
    TEST("SCSB", "R2",           0x3C, 0x11);
    TEST("SCCB", "R3",           0xBC, 0x19);
    TEST("SHIB", "R4",           0x3C, 0x22);
    TEST("SLSB", "R5",           0xBC, 0x2A);
    TEST("SGTB", "R6",           0x3C, 0x33);
    TEST("SLEB", "R7",           0xBC, 0x3B);
    TEST("SFSB", "0(R0)",        0x3C, 0x44, 0x00);
    TEST("SFCB", "1(R1)",        0xBC, 0x4C, 0x01);
    TEST("SLOB", "2(R2)",        0x3C, 0x55, 0x02);
    TEST("SHSB", "3(R3)",        0xBC, 0x5D, 0x03);
    TEST("SLTB", "4(R4)",        0x3C, 0x66, 0x04);
    TEST("SGEB", "5(R5)",        0xBC, 0x6E, 0x05);
    TEST("SEQW", "6(R6)",        0x3D, 0x70, 0x06);
    TEST("SNEW", "7(R7)",        0xBD, 0x78, 0x07);
    TEST("SCSW", "8(9(FP))",     0x3D, 0x81, 0x09, 0x08);
    TEST("SCCW", "10(11(SP))",   0xBD, 0x89, 0x0B, 0x0A);
    TEST("SHIW", "12(13(SB))",   0x3D, 0x92, 0x0D, 0x0C);
    TEST("SLSW", "@0x00000E",    0xBD, 0xAA, 0x0E);
    TEST("SGTW", "EXT(15)+16",   0x3D, 0xB3, 0x0F, 0x10);
    TEST("SLEW", "TOS",          0xBD, 0xBB);
    TEST("SFSW", "12(FP)",       0x3D, 0xC4, 0x0C);
    TEST("SFCW", "13(SP)",       0xBD, 0xCC, 0x0D);
    TEST("SLOW", "10(SB)",       0x3D, 0xD5, 0x0A);
    TEST("SHSW", ".+15",         0xBD, 0xDD, 0x0F);
    TEST("SLTW", "2(R0)[R7:B]",  0x3D, 0xE6, 0x47, 0x02);
    TEST("SGEW", "3(R1)[R6:W]",  0xBD, 0xEE, 0x4E, 0x03);
    TEST("SEQD", "4(R2)[R5:D]",  0x3F, 0xF0, 0x55, 0x04);
    TEST("SNED", "5(R3)[R4:Q]",  0xBF, 0xF8, 0x5C, 0x05);
    TEST("SCSD", "-1(R5)",       0x3F, 0x69, 0x7F);
    TEST("SCCD", "-2(R6)",       0xBF, 0x71, 0x7E);
    TEST("SHID", "TOS",          0x3F, 0xBA);
    TEST("SLSD", "-4(-5(FP))",   0xBF, 0x82, 0x7B, 0x7C);
    TEST("SGTD", "-6(-7(SP))",   0x3F, 0x8B, 0x79, 0x7A);
    TEST("SLED", "-8(-9(SB))",   0xBF, 0x93, 0x77, 0x78);
    TEST("SFSD", "@0x001234",    0x3F, 0xAC, 0x92, 0x34);
    TEST("SFCD", "EXT(-9)+(-8)", 0xBF, 0xB4, 0x77, 0x78);
    TEST("SLOD", "-12(FP)",      0x3F, 0xC5, 0x74);
    TEST("SHSD", "-13(SP)",      0xBF, 0xCD, 0x73);
    TEST("SLTD", "-14(SB)",      0x3F, 0xD6, 0x72);
    ATEST(0x1000, "SGED", ".-15",         0xBF, 0xDE, 0x71);

    disassembler.setOption("gnu-as", "enable");
    ATEST(0x1000, "SHSW", "0x00100F(PC)", 0xBD, 0xDD, 0x0F);
    ATEST(0x1000, "SGED", "0x000FF1(PC)", 0xBF, 0xDE, 0x71);

    disassembler.setOption("external-paren", "enable");
    TEST("SGTW", "EXT(15)+16", 0x3D, 0xB3, 0x0F, 0x10);
    TEST("SFCD", "EXT(-9)+(-8)", 0xBF, 0xB4, 0x77, 0x78);

    disassembler.setOption("gnu-as", "disable");
    TEST("SGTW", "16(15(EXT))", 0x3D, 0xB3, 0x0F, 0x10);
    TEST("SFCD", "-8(-9(EXT))", 0xBF, 0xB4, 0x77, 0x78);
}

void test_format_3() {
    TEST("BICPSRB", "0xA2",  0x7C, 0xA1, 0xA2);
    TEST("BISPSRB", "0xA2",  0x7C, 0xA3, 0xA2);
    TEST("ADJSPD", "-4(FP)", 0x7F, 0xC5, 0x7C);
    TEST("CASEB",  ".+4[R7:B]", 0x7C, 0xE7, 0xDF, 0x04);
    TEST("CXPD",   "8(SB)",     0x7F, 0xD0, 0x08);
    TEST("JSR",    "0(4(SB))",  0x7F, 0x96, 0x04, 0x00);
    TEST("JUMP",   "0(-8(FP))", 0x7F, 0x82, 0x78, 0x00);

    disassembler.setOption("gnu-as", "enable");
    ATEST(0x100000, "CASEB",  "0x100004(PC)[R7:B]", 0x7C, 0xE7, 0xDF, 0x04);
}

void test_format_4() {
    TEST("MOVB",  "R0, R1",    0x54, 0x00);
    TEST("MOVW",  "R2, R3",    0xD5, 0x10);
    TEST("MOVD",  "R0, 8(SB)", 0x97, 0x06, 0x08);
    NMEM("MOVD",  "R2, 0(SB)", "0(SB)", 0x97, 0x16);
    NMEM("MOVD",  "",          "", 0x97);

    TEST("ADDB",  "R0, R1",        0x40, 0x00);
    TEST("ADDW",  "R2, R3",        0xC1, 0x10);
    TEST("ADDD",  "4(SB), -4(FP)", 0x03, 0xD6, 0x04, 0x7C);
    TEST("ADDCB", "32, R0",        0x10, 0xA0, 0x20);
    TEST("ADDCW", "R2, R3",        0xD1, 0x10);
    TEST("ADDCD", "8(SB), R0",     0x13, 0xD0, 0x08);
    TEST("SUBB",  "R0, R1",        0x60, 0x00);
    TEST("SUBW",  "R2, R3",        0xE1, 0x10);
    TEST("SUBD",  "4(SB), 20(SB)", 0xA3, 0xD6, 0x04, 0x14);
    TEST("SUBCB", "32, R1",        0x70, 0xA0, 0x20);
    TEST("SUBCW", "TOS, -8(FP)",   0x31, 0xBE, 0x78);
    TEST("SUBCD", "R4, R5",        0x73, 0x21);
    TEST("CMPB",  "7(SB), 4(R0)",  0x04, 0xD2, 0x07, 0x04);
    TEST("CMPW",  "R2, R3",        0xC5, 0x10);
    TEST("CMPD",  "R4, R5",        0x47, 0x21);
    TEST("CMPB",  "18, 0x34",               0x04, 0xA5, 0x12, 0x34);
    TEST("CMPW",  "0x1234, 0x5678",         0x05, 0xA5, 0x12, 0x34, 0x56, 0x78);
    TEST("CMPD",  "0x12345678, 0x9ABCDEF0", 0x07, 0xA5, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0);

    TEST("ANDB",  "R0, R1", 0x68, 0x00);
    TEST("ANDW",  "R2, R3", 0xE9, 0x10);
    TEST("ANDD",  "R4, R5", 0x6B, 0x21);
    TEST("ORB",   "-6(FP), 11(SB)", 0x98, 0xC6, 0x7A, 0x0B);
    TEST("ORW",   "R2, R3",    0xD9, 0x10);
    TEST("ORD",   "R4, R5",    0x5B, 0x21);
    TEST("BICB",  "R0, 3(SB)", 0x88, 0x06, 0x03);
    TEST("BICW",  "R2, R3",    0xC9, 0x10);
    TEST("BICD",  "R4, R5",    0x4B, 0x21);
    TEST("XORB",  "-8(FP), -4(FP)", 0x38, 0xC6, 0x78, 0x7C);
    TEST("XORW",  "R2, R3",    0xF9, 0x10);
    TEST("XORD",  "R4, R5",    0x7B, 0x21);
    TEST("TBITB", "R0, R1",    0x74, 0x00);
    TEST("TBITW", "R0, 0(R1)", 0x75, 0x02, 0x00);
    TEST("TBITD", "R4, R5",    0x77, 0x21);

    TEST("ADDR",  "4(FP), R0", 0x27, 0xC0, 0x04);
}

void test_format_5() {
    TEST("CMPSB", "",    0x0E, 0x04, 0x00);
    UNKN(                0x0E, 0x04, 0x80);
    UNKN(                0x0E, 0x04, 0x40);
    UNKN(                0x0E, 0x04, 0x20);
    UNKN(                0x0E, 0x04, 0x10);
    UNKN(                0x0E, 0x04, 0x08);
    UNKN(                0x0E, 0x04, 0x80);
    TEST("MOVST", "",    0x0E, 0x80, 0x00);
    TEST("MOVSB", "B",   0x0E, 0x00, 0x01);
    UNKN(                0x0E, 0x00, 0x80);
    UNKN(                0x0E, 0x00, 0x40);
    UNKN(                0x0E, 0x00, 0x20);
    UNKN(                0x0E, 0x00, 0x10);
    UNKN(                0x0E, 0x00, 0x08);
    TEST("SKPSB", "U",   0x0E, 0x0C, 0x06);
    TEST("SKPSB", "W",   0x0E, 0x0C, 0x02);
    TEST("SKPSB", "B,U", 0x0E, 0x0C, 0x07);
    TEST("SKPSB", "B,W", 0x0E, 0x0C, 0x03);
    UNKN(                0x0E, 0x0C, 0x80);
    UNKN(                0x0E, 0x0C, 0x40);
    UNKN(                0x0E, 0x0C, 0x20);
    UNKN(                0x0E, 0x0C, 0x10);
    UNKN(                0x0E, 0x0C, 0x08);

    TEST("MOVSB", "",    0x0E, 0x00, 0x00);
    TEST("MOVSB", "B",   0x0E, 0x00, 0x01);
    TEST("MOVSB", "W",   0x0E, 0x00, 0x02);
    TEST("MOVSB", "B,W", 0x0E, 0x00, 0x03);
    ERRT("MOVSB", "",    ILLEGAL_OPERAND_MODE, "", 0x0E, 0x00, 0x04);
    ERRT("MOVSB", "B,",  ILLEGAL_OPERAND_MODE, "", 0x0E, 0x00, 0x05);
    TEST("MOVSB", "U",   0x0E, 0x00, 0x06);
    TEST("MOVSB", "B,U", 0x0E, 0x00, 0x07);

    TEST("SETCFG", "[I,F,M]", 0x0E, 0x8B, 0x03);
    TEST("SETCFG", "[]",      0x0E, 0x0B, 0x00);
    UNKN(                     0x0E, 0x0B, 0x80);
    UNKN(                     0x0E, 0x0B, 0x40);
    UNKN(                     0x0E, 0x0B, 0x20);
    UNKN(                     0x0E, 0x0B, 0x10);
    UNKN(                     0x0E, 0x0B, 0x08);
}

void test_format_6() {
    TEST("ABSB",  "R5, R6",        0x4E, 0xB0, 0x29);
    TEST("ABSD",  "8(SP), R7",     0x4E, 0xF3, 0xC9, 0x08);
    TEST("ADDPB", "5(SB), TOS",    0x4E, 0xFC, 0xD5, 0x05);
    TEST("ADDPD", "R0, R1",        0x4E, 0x7F, 0x00);
    TEST("ASHB",  "2, 16(SB)",     0x4E, 0x84, 0xA6, 0x02, 0x10);
    TEST("ASHW",  "TOS, 16(SB)",   0x4E, 0x85, 0xBE, 0x10);
    TEST("CBITW", "R0, 0(R1)",     0x4E, 0x49, 0x02, 0x00);
    TEST("COMB",  "R0, -4(FP)",    0x4E, 0x34, 0x06, 0x7C);
    TEST("IBITW", "R0, 1(R1)",     0x4E, 0x79, 0x02, 0x01);
    TEST("LSHB",  "4, 8(SB)",      0x4E, 0x94, 0xA6, 0x04, 0x08);
    TEST("LSHW",  "-4(FP), 8(SB)", 0x4E, 0x95, 0xC6, 0x7C, 0x08);
    TEST("NEGB",  "R5, R6",        0x4E, 0xA0, 0x29);
    TEST("NEGW",  "4(SB), 6(SB)",  0x4E, 0xA1, 0xD6, 0x04, 0x06);
    TEST("NOTB",  "R0, R0",        0x4E, 0x24, 0x00);
    TEST("NOTW",  "10(R1), TOS",   0x4E, 0xE5, 0x4D, 0x0A);
    TEST("ROTB",  "4, R5",         0x4E, 0x40, 0xA1, 0x04);
    TEST("ROTW",  "-3, 16(SP)",    0x4E, 0x41, 0xA6, 0xFD, 0x10);
    TEST("SBITW", "R0, 1(R1)",     0x4E, 0x59, 0x02, 0x01);
    TEST("SUBPB", "-8(FP), 16(FP)", 0x4E, 0x2C, 0xC6, 0x78, 0x10);
    TEST("SUBPD", "0x00000099, R1", 0x4E, 0x6F, 0xA0, 0x00, 0x00, 0x00, 0x99);
}

void test_format_7() {
    TEST("CMPMW", "10(R0), 16(R1), 4", 0xCE, 0x45, 0x42, 0x0A, 0x10, 0x06);
    TEST("DEIB",  "R1, R0", 0xCE, 0x2C, 0x08);
    TEST("DEIW",  "R3, R2", 0xCE, 0xAD, 0x18);
    TEST("DEID",  "R5, R4", 0xCE, 0x2F, 0x29);
    TEST("DIVW",  "10(SP), 4(SP)",     0xCE, 0x7D, 0xCE, 0x0A, 0x04);
    TEST("DIVD",  "-6(FP), 12(SB)",    0xCE, 0xBF, 0xC6, 0x7A, 0x0C);
    TEST("EXTSB", "16(SB), R2, 0, 32", 0xCE, 0x8C, 0xD0, 0x10, 0x1F);
    TEST("EXTSW", "16(SB), R2, 2, 30", 0xCE, 0x8D, 0xD0, 0x10, 0x5D);
    TEST("EXTSD", "16(SB), R2, 7, 25", 0xCE, 0x8F, 0xD0, 0x10, 0xF8);
    TEST("EXTSD", "16(SB), R2, 7, 1",  0xCE, 0x8F, 0xD0, 0x10, 0xE0);
    ERRT("EXTSB", "16(SB), R2, 1, 32", OVERFLOW_RANGE, "1, 32", 0xCE, 0x8C, 0xD0, 0x10, 0x3F);
    ERRT("EXTSW", "16(SB), R2, 2, 31", OVERFLOW_RANGE, "2, 31", 0xCE, 0x8D, 0xD0, 0x10, 0x5E);
    ERRT("EXTSD", "16(SB), R2, 7, 26", OVERFLOW_RANGE, "7, 26", 0xCE, 0x8F, 0xD0, 0x10, 0xF9);
    TEST("INSSB", "R2, 16(SB), 0, 32", 0xCE, 0x88, 0x16, 0x10, 0x1F);
    TEST("INSSW", "R2, 16(SB), 2, 30", 0xCE, 0x89, 0x16, 0x10, 0x5D);
    TEST("INSSD", "R2, 16(SB), 7, 25", 0xCE, 0x8B, 0x16, 0x10, 0xF8);
    TEST("INSSD", "R2, 16(SB), 7, 1",  0xCE, 0x8B, 0x16, 0x10, 0xE0);
    ERRT("INSSB", "R2, 16(SB), 1, 32", OVERFLOW_RANGE, "1, 32", 0xCE, 0x88, 0x16, 0x10, 0x3F);
    ERRT("INSSW", "R2, 16(SB), 2, 31", OVERFLOW_RANGE, "2, 31", 0xCE, 0x89, 0x16, 0x10, 0x5E);
    ERRT("INSSD", "R2, 16(SB), 7, 26", OVERFLOW_RANGE, "7, 26", 0xCE, 0x8B, 0x16, 0x10, 0xF9);
    TEST("MEIB",  "R1, R0", 0xCE, 0x24, 0x08);
    TEST("MEIW",  "R3, R2", 0xCE, 0xA5, 0x18);
    TEST("MEID",  "R5, R4", 0xCE, 0x27, 0x29);
    TEST("MODB",  "4(SB), 8(SB)",       0xCE, 0xB8, 0xD6, 0x04, 0x08);
    TEST("MOVMB", "10(R0), 16(R1), 16", 0xCE, 0x40, 0x42, 0x0A, 0x10, 0x0F);
    TEST("MOVMW", "10(R0), 16(R1), 8",  0xCE, 0x41, 0x42, 0x0A, 0x10, 0x0E);
    TEST("MOVMD", "10(R0), 16(R1), 4",  0xCE, 0x43, 0x42, 0x0A, 0x10, 0x0C);
    ERRT("CMPMB", "10(R0), 16(R1), 17", OVERFLOW_RANGE, "17",   0xCE, 0x44, 0x42, 0x0A, 0x10, 0x10);
    ERRT("CMPMW", "10(R0), 16(R1), 9",  OVERFLOW_RANGE, "9",    0xCE, 0x45, 0x42, 0x0A, 0x10, 0x10);
    ERRT("CMPMD", "10(R0), 16(R1), 5",  OVERFLOW_RANGE, "5",    0xCE, 0x47, 0x42, 0x0A, 0x10, 0x10);
    ERRT("CMPMW", "10(R0), 16(R1), 15", ILLEGAL_CONSTANT, "15", 0xCE, 0x45, 0x42, 0x0A, 0x10, 0x0F);
    ERRT("CMPMD", "10(R0), 16(R1), 15", ILLEGAL_CONSTANT, "15", 0xCE, 0x47, 0x42, 0x0A, 0x10, 0x0F);
    ERRT("CMPMD", "10(R0), 16(R1), 14", ILLEGAL_CONSTANT, "14", 0xCE, 0x47, 0x42, 0x0A, 0x10, 0x0E);
    ERRT("CMPMD", "10(R0), 16(R1), 13", ILLEGAL_CONSTANT, "13", 0xCE, 0x47, 0x42, 0x0A, 0x10, 0x0D);
    TEST("MOVMW", "10(R0), 16(R1), 4", 0xCE, 0x41, 0x42, 0x0A, 0x10, 0x06);
    TEST("MOVXBW", "2(SB), R0",        0xCE, 0x10, 0xD0, 0x02);
    TEST("MOVZBW", "-4(FP), R0",       0xCE, 0x14, 0xC0, 0x7C);
    TEST("MULW",  "5, R0",             0xCE, 0x21, 0xA0, 0x00, 0x05);
    TEST("MULD",  "4(-4(FP)), 3(SB)",  0xCE, 0xA3, 0x86, 0x7C, 0x04, 0x03);
    TEST("QUOB",  "R0, R7",            0xCE, 0xF0, 0x01);
    TEST("QUOW",  "4(SB), 8(SB)",      0xCE, 0xB1, 0xD6, 0x04, 0x08);
    TEST("REMB",  "4(SB), 8(SB)",      0xCE, 0xB4, 0xD6, 0x04, 0x08);

    ERRT("DEIB", "R1, R3", REGISTER_NOT_ALIGNED, "R3", 0xCE, 0xEC, 0x08);
    ERRT("DEIW", "R3, R1", REGISTER_NOT_ALIGNED, "R1", 0xCE, 0x6D, 0x18);
    ERRT("DEID", "R5, R7", REGISTER_NOT_ALIGNED, "R7", 0xCE, 0xEF, 0x29);
    ERRT("MEIB", "R1, R3", REGISTER_NOT_ALIGNED, "R3", 0xCE, 0xE4, 0x08);
    ERRT("MEIW", "R3, R1", REGISTER_NOT_ALIGNED, "R1", 0xCE, 0x65, 0x18);
    ERRT("MEID", "R5, R7", REGISTER_NOT_ALIGNED, "R7", 0xCE, 0xE7, 0x29);
}

void test_format_8() {
    TEST("CHECKB", "R0, 4(SB), R2",       0xEE, 0x80, 0xD0, 0x04);
    TEST("CVTP",   "R0, 32(SB), R2",      0x6E, 0x83, 0xD0, 0x20);
    TEST("FFSB",   "-4(FP), TOS",         0x6E, 0xC4, 0xC5, 0x7C);
    UNKN(                                 0x6E, 0xE4);
    UNKN(                                 0x6E, 0xD4);
    UNKN(                                 0x6E, 0xCC);
    TEST("FFSW",   "8(SB), R0",           0x6E, 0x05, 0xD0, 0x08);
    UNKN(                                 0x6E, 0x25);
    UNKN(                                 0x6E, 0x15);
    UNKN(                                 0x6E, 0x0D);
    TEST("FFSD",   "R7, 2(R1)",           0x6E, 0x47, 0x3A, 0x02);
    UNKN(                                 0x6E, 0x67);
    UNKN(                                 0x6E, 0x57);
    UNKN(                                 0x6E, 0x4F);
    TEST("INDEXB", "R0, 20(SB), -4(FP)",  0x2E, 0x04, 0xD6, 0x14, 0x7C);
    TEST("EXTB", "R0, 0(R1), R2, 8",  0x2E, 0x80, 0x48, 0x00, 0x08);
    TEST("EXTW", "R0, 0(R1), R2, 16", 0x2E, 0x81, 0x48, 0x00, 0x10);
    TEST("EXTD", "R0, 0(R1), R2, 32", 0x2E, 0x83, 0x48, 0x00, 0x20);
    ERRT("EXTB", "R0, 0(R1), R2, 0",  ILLEGAL_CONSTANT, "0", 0x2E, 0x80, 0x48, 0x00, 0x00);
    ERRT("EXTD", "R0, 0(R1), R2, 33", OVERFLOW_RANGE,  "33", 0x2E, 0x83, 0x48, 0x00, 0x21);
    TEST("INSB", "R0, R2, 0(R1), 8",  0xAE, 0x40, 0x12, 0x00, 0x08);
    TEST("INSW", "R0, R2, 0(R1), 16", 0xAE, 0x41, 0x12, 0x00, 0x10);
    TEST("INSD", "R0, R2, 0(R1), 32", 0xAE, 0x43, 0x12, 0x00, 0x20);
    ERRT("INSB", "R0, R2, 0(R1), 0",  ILLEGAL_CONSTANT, "0", 0xAE, 0x40, 0x12, 0x00, 0x00);
    ERRT("INSD", "R0, R2, 0(R1), 33", OVERFLOW_RANGE,  "33", 0xAE, 0x43, 0x12, 0x00, 0x21);
}

#if !defined(LIBASM_NS32000_NOFPU)

void test_format_9_fpu() {
    TEST("MOVF", "F1, 8(SB)", 0xBE, 0x85, 0x0E, 0x08);
    TEST("MOVL", "F2, 8(SB)", 0xBE, 0x84, 0x16, 0x08);
    TFLT("MOVF", "NAN, F1",  "0x0000C07F, F1", 0xBE, 0x45, 0xA0, 0x7F, 0xC0, 0x00, 0x00);
    TFLT("MOVF", "INF, F2",  "0x0000807F, F2", 0xBE, 0x85, 0xA0, 0x7F, 0x80, 0x00, 0x00);
    TFLT("MOVF", "-INF, F2", "0x000080FF, F2", 0xBE, 0x85, 0xA0, 0xFF, 0x80, 0x00, 0x00);
    TFLT("MOVF", "0, F3",    "0x00000000, F3", 0xBE, 0xC5, 0xA0, 0x00, 0x00, 0x00, 0x00);
    TFLT("MOVF", "-0, F3",   "0x00000080, F3", 0xBE, 0xC5, 0xA0, 0x80, 0x00, 0x00, 0x00);
    TFLT("MOVL", "NAN, F0",  "0x000000000000F87F, F0",
         0xBE, 0x04, 0xA0, 0x7F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    TFLT("MOVL", "INF, F2",  "0x000000000000F07F, F2",
         0xBE, 0x84, 0xA0, 0x7F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    TFLT("MOVL", "-INF, F2", "0x000000000000F0FF, F2",
         0xBE, 0x84, 0xA0, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    TFLT("MOVL", "0, F4",    "0x0000000000000000, F4",
         0xBE, 0x04, 0xA1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    TFLT("MOVL", "-0, F4",   "0x0000000000000080, F4",
         0xBE, 0x04, 0xA1, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    TEST("MOVBF", "2, F0",      0x3E, 0x04, 0xA0, 0x02);
    TEST("MOVBL", "R1, F2",     0x3E, 0x80, 0x08);
    TEST("MOVWL", "R3, F2",     0x3E, 0x81, 0x18);
    TEST("MOVDL", "R5, F2",     0x3E, 0x83, 0x28);
    TEST("MOVDL", "16(SB), F2", 0x3E, 0x83, 0xD0, 0x10);
    TEST("MOVFL", "8(SB), F0",  0x3E, 0x1B, 0xD0, 0x08);
    TEST("MOVFL", "F1, F2",     0x3E, 0x9B, 0x08);
    TEST("MOVLF", "F0, 12(SB)", 0x3E, 0x96, 0x06, 0x0C);
    TEST("MOVLF", "F0, F3",     0x3E, 0xD6, 0x00);
    TEST("FLOORFB", "F1, R3",   0x3E, 0xFC, 0x08);
    TEST("FLOORLD", "F2, R5",   0x3E, 0x7B, 0x11);
    TEST("ROUNDFB", "F1, R5",   0x3E, 0x64, 0x09);
    TEST("ROUNDLD", "F2, R5",   0x3E, 0x63, 0x11);
    TEST("TRUNCFB", "F1, R3",   0x3E, 0xEC, 0x08);
    TEST("TRUNCLD", "F2, R5",   0x3E, 0x6B, 0x11);
    TEST("LFSR", "R0",  0x3E, 0x0F, 0x00);
    UNKN(               0x3E, 0x4F, 0x00);
    UNKN(               0x3E, 0x8F, 0x00);
    UNKN(               0x3E, 0x0F, 0x01);
    UNKN(               0x3E, 0x0F, 0x02);
    UNKN(               0x3E, 0x0F, 0x03);
    TEST("SFSR", "TOS", 0x3E, 0xF7, 0x05);
    UNKN(               0x3E, 0xF7, 0x85);
    UNKN(               0x3E, 0xF7, 0x45);
    UNKN(               0x3E, 0xF7, 0x25);
    UNKN(               0x3E, 0xF7, 0x15);
    UNKN(               0x3E, 0xF7, 0x0D);

    ERRT("MOVL", "F3, 8(SB)",   REGISTER_NOT_ALIGNED, "F3, 8(SB)",  0xBE, 0x84, 0x1E, 0x08);
    ERRT("MOVBL", "R1, F5",     REGISTER_NOT_ALIGNED, "F5",         0x3E, 0x40, 0x09);
    ERRT("MOVWL", "R3, F7",     REGISTER_NOT_ALIGNED, "F7",         0x3E, 0xC1, 0x19);
    ERRT("MOVDL", "R5, F1",     REGISTER_NOT_ALIGNED, "F1",         0x3E, 0x43, 0x28);
    ERRT("MOVDL", "16(SB), F3", REGISTER_NOT_ALIGNED, "F3",         0x3E, 0xC3, 0xD0, 0x10);
    ERRT("MOVFL", "8(SB), F5",  REGISTER_NOT_ALIGNED, "F5",         0x3E, 0x5B, 0xD1, 0x08);
    ERRT("MOVFL", "F1, F7",     REGISTER_NOT_ALIGNED, "F7",         0x3E, 0xDB, 0x09);
    ERRT("MOVLF", "F1, 12(SB)", REGISTER_NOT_ALIGNED, "F1, 12(SB)", 0x3E, 0x96, 0x0E, 0x0C);
    ERRT("MOVLF", "F3, F3",     REGISTER_NOT_ALIGNED, "F3, F3",     0x3E, 0xD6, 0x18);
    ERRT("FLOORLD", "F5, R5",   REGISTER_NOT_ALIGNED, "F5, R5",     0x3E, 0x7B, 0x29);
    ERRT("ROUNDLD", "F7, R5",   REGISTER_NOT_ALIGNED, "F7, R5",     0x3E, 0x63, 0x39);
    ERRT("TRUNCLD", "F1, R5",   REGISTER_NOT_ALIGNED, "F1, R5",     0x3E, 0x6B, 0x09);
}

void test_format_11_fpu() {
    TEST("ABSF", "F1, F5",     0xBE, 0x75, 0x09);
    TEST("ABSL", "F2, F4",     0xBE, 0x34, 0x11);
    TEST("ADDF", "F3, F7",     0xBE, 0xC1, 0x19);
    TEST("ADDL", "F2, 16(SB)", 0xBE, 0x80, 0x16, 0x10);
    TEST("ADDL", "16(SB), F4", 0xBE, 0x00, 0xD1, 0x10);
    TEST("CMPF", "F1, F3",     0xBE, 0xC9, 0x08);
    TEST("CMPL", "F6, F4",     0xBE, 0x08, 0x31);
    TFLT("CMPF", "1.25, 3.75", "0x0000A03F, 0x00007040",
         0xBE, 0x09, 0xA5,
         0x3F, 0xA0, 0x00, 0x00,
         0x40, 0x70, 0x00, 0x00);
    TFLT("CMPL", "1.25, 3.75", "0x000000000000F43F, 0x0000000000000E40",
         0xBE, 0x08, 0xA5,
         0x3F, 0xF4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x40, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    TEST("DIVF", "F1, F7",     0xBE, 0xE1, 0x09);
    TEST("DIVL", "F2, 16(SB)", 0xBE, 0xA0, 0x16, 0x10);
    TEST("DIVL", "-8(FP), F4", 0xBE, 0x20, 0xC1, 0x78);
    TEST("MULF", "F5, F7",     0xBE, 0xF1, 0x29);
    TEST("MULL", "F6, 8(SB)",  0xBE, 0xB0, 0x36, 0x08);
    TEST("MULL", "-8(FP), F0", 0xBE, 0x30, 0xC0, 0x78);
    TEST("NEGF", "F3, F5",     0xBE, 0x55, 0x19);
    TEST("NEGL", "F2, F4",     0xBE, 0x14, 0x11);
    TEST("SUBF", "F1, F7",     0xBE, 0xD1, 0x09);
    TEST("SUBL", "F2, 16(SB)", 0xBE, 0x90, 0x16, 0x10);
    TEST("SUBL", "16(SB), F4", 0xBE, 0x10, 0xD1, 0x10);

    ERRT("ABSL", "F1, F4",     REGISTER_NOT_ALIGNED, "F1, F4",     0xBE, 0x34, 0x09);
    ERRT("ABSL", "F2, F3",     REGISTER_NOT_ALIGNED, "F3",         0xBE, 0xF4, 0x10);
    ERRT("ADDL", "F1, 16(SB)", REGISTER_NOT_ALIGNED, "F1, 16(SB)", 0xBE, 0x80, 0x0E, 0x10);
    ERRT("ADDL", "16(SB), F3", REGISTER_NOT_ALIGNED, "F3",         0xBE, 0xC0, 0xD0, 0x10);
    ERRT("CMPL", "F7, F4",     REGISTER_NOT_ALIGNED, "F7, F4",     0xBE, 0x08, 0x39);
    ERRT("CMPL", "F6, F3",     REGISTER_NOT_ALIGNED, "F3",         0xBE, 0xC8, 0x30);
    ERRT("DIVL", "F1, 16(SB)", REGISTER_NOT_ALIGNED, "F1, 16(SB)", 0xBE, 0xA0, 0x0E, 0x10);
    ERRT("DIVL", "-8(FP), F3", REGISTER_NOT_ALIGNED, "F3",         0xBE, 0xE0, 0xC0, 0x78);
    ERRT("MULL", "F7, 8(SB)",  REGISTER_NOT_ALIGNED, "F7, 8(SB)",  0xBE, 0xB0, 0x3E, 0x08);
    ERRT("MULL", "-8(FP), F1", REGISTER_NOT_ALIGNED, "F1",         0xBE, 0x70, 0xC0, 0x78);
    ERRT("NEGL", "F1, F4",     REGISTER_NOT_ALIGNED, "F1, F4",     0xBE, 0x14, 0x09);
    ERRT("NEGL", "F2, F3",     REGISTER_NOT_ALIGNED, "F3",         0xBE, 0xD4, 0x10);
    ERRT("SUBL", "F1, 16(SB)", REGISTER_NOT_ALIGNED, "F1, 16(SB)", 0xBE, 0x90, 0x0E, 0x10);
    ERRT("SUBL", "16(SB), F3", REGISTER_NOT_ALIGNED, "F3",         0xBE, 0xD0, 0xD0, 0x10);

    disassembler.setOption("gnu-as", "on");
    TFLT("CMPF", "0F1.25, 0F3.75", "0x0000A03F, 0x00007040",
         0xBE, 0x09, 0xA5,
         0x3F, 0xA0, 0x00, 0x00,
         0x40, 0x70, 0x00, 0x00);
    TFLT("CMPL", "0L1.25, 0L3.75", "0x000000000000F43F, 0x0000000000000E40",
         0xBE, 0x08, 0xA5,
         0x3F, 0xF4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x40, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
}

#endif

#if !defined(LIBASM_NS32000_NOMMU)

void test_format_8_mmu() {
    TEST("MOVSUB", "5(SP), 9(SB)",        0xAE, 0x8C, 0xCE, 0x05, 0x09);
    TEST("MOVUSB", "9(SB), 5(SP)",        0xAE, 0x5C, 0xD6, 0x09, 0x05);
}

void test_format_14_mmu() {
    TEST("LMR", "BPR0, R1",                           0x1E, 0x0B, 0x08);
    TEST("LMR", "BPR1, R2",                           0x1E, 0x8B, 0x10);
    ERRT("LMR",     ", R3", UNKNOWN_REGISTER, ", R3", 0x1E, 0x0B, 0x19);
    ERRT("LMR",     ", R4", UNKNOWN_REGISTER, ", R4", 0x1E, 0x8B, 0x21);
    ERRT("LMR",     ", R5", UNKNOWN_REGISTER, ", R5", 0x1E, 0x0B, 0x2A);
    ERRT("LMR",     ", R6", UNKNOWN_REGISTER, ", R6", 0x1E, 0x8B, 0x32);
    ERRT("LMR",     ", R7", UNKNOWN_REGISTER, ", R7", 0x1E, 0x0B, 0x3B);
    ERRT("LMR",     ", R0", UNKNOWN_REGISTER, ", R0", 0x1E, 0x8B, 0x03);
    ERRT("LMR",     ", R1", UNKNOWN_REGISTER, ", R1", 0x1E, 0x0B, 0x0C);
    ERRT("LMR",     ", R2", UNKNOWN_REGISTER, ", R2", 0x1E, 0x8B, 0x14);
    TEST("LMR",  "MSR, R3",                           0x1E, 0x0B, 0x1D);
    TEST("LMR", "BCNT, R4",                           0x1E, 0x8B, 0x25);
    TEST("LMR", "PTB0, R5",                           0x1E, 0x0B, 0x2E);
    TEST("LMR", "PTB1, R6",                           0x1E, 0x8B, 0x36);
    ERRT("LMR",     ", R7", UNKNOWN_REGISTER, ", R7", 0x1E, 0x0B, 0x3F);
    TEST("LMR",  "EIA, R0",                           0x1E, 0x8B, 0x07);

    TEST("SMR", "BPR0, R1",                           0x1E, 0x0F, 0x08);
    TEST("SMR", "BPR1, R2",                           0x1E, 0x8F, 0x10);
    ERRT("SMR",     ", R3", UNKNOWN_REGISTER, ", R3", 0x1E, 0x0F, 0x19);
    ERRT("SMR",     ", R4", UNKNOWN_REGISTER, ", R4", 0x1E, 0x8F, 0x21);
    ERRT("SMR",     ", R5", UNKNOWN_REGISTER, ", R5", 0x1E, 0x0F, 0x2A);
    ERRT("SMR",     ", R6", UNKNOWN_REGISTER, ", R6", 0x1E, 0x8F, 0x32);
    ERRT("SMR",     ", R7", UNKNOWN_REGISTER, ", R7", 0x1E, 0x0F, 0x3B);
    ERRT("SMR",     ", R0", UNKNOWN_REGISTER, ", R0", 0x1E, 0x8F, 0x03);
    ERRT("SMR",     ", R1", UNKNOWN_REGISTER, ", R1", 0x1E, 0x0F, 0x0C);
    ERRT("SMR",     ", R2", UNKNOWN_REGISTER, ", R2", 0x1E, 0x8F, 0x14);
    TEST("SMR",  "MSR, R3",                           0x1E, 0x0F, 0x1D);
    TEST("SMR", "BCNT, R4",                           0x1E, 0x8F, 0x25);
    TEST("SMR", "PTB0, R5",                           0x1E, 0x0F, 0x2E);
    TEST("SMR", "PTB1, R6",                           0x1E, 0x8F, 0x36);
    ERRT("SMR",     ", R7", UNKNOWN_REGISTER, ", R7", 0x1E, 0x0F, 0x3F);
    TEST("SMR",  "EIA, R0",                           0x1E, 0x8F, 0x07);

    TEST("RDVAL", "0x0200(R0)", 0x1E, 0x03, 0x40, 0x82, 0x00);
    UNKN(                       0x1E, 0x03, 0x44);
    UNKN(                       0x1E, 0x03, 0x42);
    UNKN(                       0x1E, 0x03, 0x41);
    TEST("WRVAL", "0x0200(R0)", 0x1E, 0x07, 0x40, 0x82, 0x00);
    UNKN(                       0x1E, 0x07, 0x44);
    UNKN(                       0x1E, 0x07, 0x42);
    UNKN(                       0x1E, 0x07, 0x41);
}
#endif

void test_generic_addressing() {
    // Register
    TEST("ADDW", "R1, R2", 0x81, 0x08);
#if !defined(LIBASM_NS32000_NOFPU)
    TEST("ADDF", "F1, F2", 0xBE, 0x81, 0x08);
#endif
    // Register Relative
    TEST("ADDW", "4(R1), R2",     0x81, 0x48, 0x04);
    TEST("ADDW", "4(R1), 32(R2)", 0x81, 0x4A, 0x04, 0x80, 0x20);
    TEST("ADDW", "R1, 32(R2)",    0x81, 0x0A, 0x80, 0x20);
    // Memory Relative
    TEST("ADDW", "6(4(FP)), R2",         0x81, 0x80, 0x04, 0x06);
    TEST("ADDW", "6(4(FP)), 16(32(SB))", 0x81, 0x84, 0x04, 0x06, 0x80, 0x20, 0x10);
    TEST("ADDW", "R2, 16(32(SB))",       0x81, 0x14, 0x80, 0x20, 0x10);
    // Immediate
    TEST("ADDB", "0x56, R1",       0x40, 0xA0, 0x56);
    TEST("ADDW", "0x1234, R1",     0x41, 0xA0, 0x12, 0x34);
    TEST("ADDD", "0x12345678, R1", 0x43, 0xA0, 0x12, 0x34, 0x56, 0x78);
    ERRT("ADDB", "R1, 0", OPERAND_NOT_ALLOWED, "0", 0x00, 0x0D);
    ERRT("ADDW", "R1, 0", OPERAND_NOT_ALLOWED, "0", 0x01, 0x0D);
    ERRT("ADDD", "R1, 0", OPERAND_NOT_ALLOWED, "0", 0x03, 0x0D);
#if !defined(LIBASM_NS32000_NOFPU)
    TFLT("ADDF", "3.14159012, F1", "0xD00F4940, F1",
         0xBE, 0x41, 0xA0, 0x40, 0x49, 0x0F, 0xD0);
    TFLT("ADDF", "299792000, F3",  "0xB4F38E4D, F3",
         0xBE, 0xC1, 0xA0, 0x4D, 0x8E, 0xF3, 0xB4);
    TFLT("ADDL", "2.7182818284590451, F2", "0x6957148B0ABF0540, F2",
         0xBE, 0x80, 0xA0, 0x40, 0x05, 0xBF, 0x0A, 0x8B, 0x14, 0x57, 0x69);
    TFLT("ADDL", "6.6260701499999998E-34, F4", "0x113102DE0B860B39, F4",
         0xBE, 0x00, 0xA1, 0x39, 0x0B, 0x86, 0x0B, 0xDE, 0x02, 0x31, 0x11);
#if defined(LIBASM_DIS_NOFLOAT)
    ERRT("ADDF", "F1, 0x00000000",         OPERAND_NOT_ALLOWED,
         "0x00000000", 0xBE, 0x01, 0x0D);
    ERRT("ADDL", "F2, 0x0000000000000000", OPERAND_NOT_ALLOWED,
         "0x0000000000000000", 0xBE, 0x00, 0x15);
#else
    ERRT("ADDF", "F1, 0", OPERAND_NOT_ALLOWED, "0", 0xBE, 0x01, 0x0D);
    ERRT("ADDL", "F2, 0", OPERAND_NOT_ALLOWED, "0", 0xBE, 0x00, 0x15);
#endif
#endif

    // Absolute
    TEST("ADDW", "@0x001234, 4(R2)"  ,   0x81, 0xAA, 0x92, 0x34, 0x04);
    TEST("ADDW", "@0x001234, @0x005678", 0x41, 0xAD, 0x92, 0x34, 0xC0, 0x00, 0x56, 0x78);
    TEST("ADDW", "4(R2), @0x005678",     0x41, 0x55, 0x04, 0xC0, 0x00, 0x56, 0x78);
    TEST("MOVW", "@0x000000, R0",                   0x15, 0xA8, 0xC0, 0x00, 0x00, 0x00);
    TEST("MOVW", "@0x000000, R0",                   0x15, 0xA8, 0x00);
    TEST("MOVW", "@0x00003F, R0",                   0x15, 0xA8, 0x3F);
    TEST("MOVW", "@0x001FFF, R0",                   0x15, 0xA8, 0x9F, 0xFF);
    TEST("MOVW", "@0x7FFFFF, R0",                   0x15, 0xA8, 0xC0, 0x7F, 0xFF, 0xFF);
    TEST("MOVW", "@0x800000, R0",                   0x15, 0xA8, 0xC0, 0x80, 0x00, 0x00);
    TEST("MOVW", "@0xFFFFFF, R0",                   0x15, 0xA8, 0xC0, 0xFF, 0xFF, 0xFF);
    ERRT("MOVW", "@0x01000000, R0",  OVERFLOW_RANGE, "@0x01000000, R0",  0x15, 0xA8, 0xC1, 0x00, 0x00, 0x00);
    ERRT("MOVW", "@-0x01000000, R0", OVERFLOW_RANGE, "@-0x01000000, R0", 0x15, 0xA8, 0xFF, 0x00, 0x00, 0x00);
    TEST("MOVW", "@0x800000, R0",                   0x15, 0xA8, 0xFF, 0x80, 0x00, 0x00);
    TEST("MOVW", "@0xFFE000, R0",                   0x15, 0xA8, 0xA0, 0x00);
    TEST("MOVW", "@0xFFFFC0, R0",                   0x15, 0xA8, 0x40);
    TEST("MOVW", "@0xFFFFFF, R0",                   0x15, 0xA8, 0x7F);
    TEST("MOVW", "@0xFFFFFF, R0",                   0x15, 0xA8, 0xFF, 0xFF, 0xFF, 0xFF);
    // External
    TEST("ADDW", "EXT(2)+4, 10(R2)",           0x81, 0xB2, 0x02, 0x04, 0x0A);
    TEST("ADDW", "EXT(-2)+(-4), 10(R2)",       0x81, 0xB2, 0x7E, 0x7C, 0x0A);
    TEST("ADDW", "EXT(-2)+4, EXT(-6)+8",       0x81, 0xB5, 0x7E, 0x04, 0x7A, 0x08);
    TEST("ADDW", "EXT(2)+(-4), EXT(6)+(-8)",   0x81, 0xB5, 0x02, 0x7C, 0x06, 0x78);
    TEST("ADDW", "10(R2), EXT(6)+8",           0x81, 0x55, 0x0A, 0x06, 0x08);
    TEST("ADDW", "10(R2), EXT(-6)+(-8)",       0x81, 0x55, 0x0A, 0x7A, 0x78);
    // Top of Stack
    TEST("ADDW", "TOS, 4(R2)",   0x81, 0xBA, 0x04);
    TEST("ADDW", "TOS, TOS",     0xC1, 0xBD);
    TEST("ADDW", "4(R2), TOS",   0xC1, 0x55, 0x04);
    // Memory Space
    TEST("ADDW", "4(FP), 6(R2)", 0x81, 0xC2, 0x04, 0x06);
    TEST("ADDW", "4(SP), 6(R2)", 0x81, 0xCA, 0x04, 0x06);
    TEST("ADDW", "4(SB), 6(R2)", 0x81, 0xD2, 0x04, 0x06);
    TEST("ADDW", ".+4, 6(R2)",   0x81, 0xDA, 0x04, 0x06);
    TEST("ADDW", "6(R2), 4(FP)", 0x01, 0x56, 0x06, 0x04);
    TEST("ADDW", "6(R2), 4(SP)", 0x41, 0x56, 0x06, 0x04);
    TEST("ADDW", "6(R2), 4(SB)", 0x81, 0x56, 0x06, 0x04);
    TEST("ADDW", "6(R2), .+4",   0xC1, 0x56, 0x06, 0x04);
    TEST("ADDW", "4(FP), 6(SB)", 0x81, 0xC6, 0x04, 0x06);
    TEST("ADDW", "6(SB), 8(SP)", 0x41, 0xD6, 0x06, 0x08);
    TEST("ADDW", "8(SP), .+10",  0xC1, 0xCE, 0x08, 0x0A);
    TEST("ADDW", ".+2, .+12",    0xC1, 0xDE, 0x02, 0x0C);
    // Scaled Indexing
    TEST("ADDW", "0(R1)[R3:B], 8(R2)",          0x81, 0xE2, 0x0B, 0x08);
    TEST("ADDW", "4(R1)[R3:B], 8(R2)",          0x81, 0xE2, 0x4B, 0x04, 0x08);
    TEST("ADDW", "6(4(FP))[R3:B], 8(R2)",       0x81, 0xE2, 0x83, 0x04, 0x06, 0x08);
    TEST("ADDW", "@0x001234[R3:B], 8(R2)",      0x81, 0xE2, 0xAB, 0x92, 0x34, 0x08);
    TEST("ADDW", "EXT(4)+6[R3:B], 8(R2)",       0x81, 0xE2, 0xB3, 0x04, 0x06, 0x08);
    TEST("ADDW", "TOS[R3:B], 8(R2)",            0x81, 0xE2, 0xBB, 0x08);
    TEST("ADDW", "4(SP)[R3:B], 8(R2)",          0x81, 0xE2, 0xCB, 0x04, 0x08);
    TEST("ADDW", ".+4[R3:B], 8(R2)",            0x81, 0xE2, 0xDB, 0x04, 0x08);
    TEST("ADDW", "8(R2), 0(R1)[R3:W]",          0x41, 0x57, 0x0B, 0x08);
    TEST("ADDW", "8(R2), 4(R1)[R3:W]",          0x41, 0x57, 0x4B, 0x08, 0x04);
    TEST("ADDW", "8(R2), 6(4(FP))[R3:W]",       0x41, 0x57, 0x83, 0x08, 0x04, 0x06);
    TEST("ADDW", "8(R2), @0x001234[R3:W]",      0x41, 0x57, 0xAB, 0x08, 0x92, 0x34);
    TEST("ADDW", "8(R2), EXT(4)+6[R3:W]",       0x41, 0x57, 0xB3, 0x08, 0x04, 0x06);
    TEST("ADDW", "8(R2), TOS[R3:W]",            0x41, 0x57, 0xBB, 0x08);
    TEST("ADDW", "8(R2), 4(SP)[R3:W]",          0x41, 0x57, 0xCB, 0x08, 0x04);
    TEST("ADDW", "8(R2), .+4[R3:W]",            0x41, 0x57, 0xDB, 0x08, 0x04);
    TEST("ADDW", "8(R2)[R3:D], 0(R1)[R5:W]",    0x41, 0xF7, 0x53, 0x0D, 0x08);
    TEST("ADDW", "8(R2)[R3:D], 4(R1)[R5:W]",    0x41, 0xF7, 0x53, 0x4D, 0x08, 0x04);
    TEST("ADDW", "8(R2)[R3:D], 6(4(FP))[R5:W]", 0x41, 0xF7, 0x53, 0x85, 0x08, 0x04, 0x06);
    TEST("ADDW", "8(R2)[R3:D], @0x001234[R5:W]",0x41, 0xF7, 0x53, 0xAD, 0x08, 0x92, 0x34);
    TEST("ADDW", "8(R2)[R3:Q], EXT(4)+6[R5:W]", 0x41, 0xFF, 0x53, 0xB5, 0x08, 0x04, 0x06);
    TEST("ADDW", "8(R2)[R3:Q], TOS[R5:W]",      0x41, 0xFF, 0x53, 0xBD, 0x08);
    TEST("ADDW", "8(R2)[R3:Q], 4(SP)[R5:W]",    0x41, 0xFF, 0x53, 0xCD, 0x08, 0x04);
    TEST("ADDW", "8(R2)[R3:Q], .+4[R5:W]",      0x41, 0xFF, 0x53, 0xDD, 0x08, 0x04);

    TEST("EXTD", "R1, 0x00112233(0x00334455(FP))[R3:W], 0x00556677(0x00778899(SB))[R4:W], 32",
         // 11101|111 01|001|0|11 00101111
         // 10000 011 10010 100
         0x2E, 0x4B, 0xEF,
         0x83, 0x94,
         0xC0, 0x33, 0x44, 0x55,
         0xC0, 0x11, 0x22, 0x33,
         0xC0, 0x77, 0x88, 0x99,
         0xC0, 0x55, 0x66, 0x77,
         0x20);

    disassembler.setOption("gnu-as", "enable");
    symtab.intern(0x10004, "label4");
    ATEST(0x10000, "ADDW", "label4(PC), 6(R2)",   0x81, 0xDA, 0x04, 0x06);
    ATEST(0x10000, "ADDW", "6(R2), label4(PC)",   0xC1, 0x56, 0x06, 0x04);
    ATEST(0x10000, "ADDW", "8(SP), 0x01000A(PC)", 0xC1, 0xCE, 0x08, 0x0A);
    ATEST(0x10000, "ADDW", "0x010002(PC), 0x01000C(PC)",     0xC1, 0xDE, 0x02, 0x0C);
    ATEST(0x10000, "ADDW", "label4(PC)[R3:B], 8(R2)",        0x81, 0xE2, 0xDB, 0x04, 0x08);
    ATEST(0x10000, "ADDW", "8(R2), label4(PC)[R3:W]",        0x41, 0x57, 0xDB, 0x08, 0x04);
    ATEST(0x10000, "ADDW", "8(R2)[R3:Q], label4(PC)[R5:W]",  0x41, 0xFF, 0x53, 0xDD, 0x08, 0x04);

    disassembler.setOption("external-paren", "enable");
    TEST("ADDW", "EXT(2)+4, 10(R2)",         0x81, 0xB2, 0x02, 0x04, 0x0A);
    TEST("ADDW", "EXT(-2)+(-4), 10(R2)",     0x81, 0xB2, 0x7E, 0x7C, 0x0A);
    TEST("ADDW", "EXT(-2)+4, EXT(-6)+8",     0x81, 0xB5, 0x7E, 0x04, 0x7A, 0x08);
    TEST("ADDW", "EXT(2)+(-4), EXT(6)+(-8)", 0x81, 0xB5, 0x02, 0x7C, 0x06, 0x78);
    TEST("ADDW", "10(R2), EXT(6)+8",         0x81, 0x55, 0x0A, 0x06, 0x08);
    TEST("ADDW", "10(R2), EXT(-6)+(-8)",     0x81, 0x55, 0x0A, 0x7A, 0x78);

    disassembler.setOption("gnu-as", "disable");
    TEST("ADDW", "4(2(EXT)), 10(R2)",      0x81, 0xB2, 0x02, 0x04, 0x0A);
    TEST("ADDW", "-4(-2(EXT)), 10(R2)",    0x81, 0xB2, 0x7E, 0x7C, 0x0A);
    TEST("ADDW", "4(-2(EXT)), 8(-6(EXT))", 0x81, 0xB5, 0x7E, 0x04, 0x7A, 0x08);
    TEST("ADDW", "-4(2(EXT)), -8(6(EXT))", 0x81, 0xB5, 0x02, 0x7C, 0x06, 0x78);
    TEST("ADDW", "10(R2), 8(6(EXT))",      0x81, 0x55, 0x0A, 0x06, 0x08);
    TEST("ADDW", "10(R2), -8(-6(EXT))",    0x81, 0x55, 0x0A, 0x7A, 0x78);
}

void test_formatter() {
    disassembler.setCStyle(false);
    ATEST(0x800000, "BGT", ".-X'00800000", 0x6A, 0xFF, 0x80, 0x00, 0x00);
    ATEST(0x000000, "BLS", ".+X'00FFFFFF", 0x5A, 0xC0, 0xFF, 0xFF, 0xFF);
    TEST("RET",  "-X'1F000000",            0x12, 0xE1, 0x00, 0x00, 0x00);
    TEST("SLSW", "@X'00000E",              0xBD, 0xAA, 0x0E);
    TEST("CMPW", "X'1234, X'5678",         0x05, 0xA5, 0x12, 0x34, 0x56, 0x78);

#if !defined(LIBASM_NS32000_NOFPU)
    TFLT("ADDF", "3.14159012, F1", "X'D00F4940, F1",
         0xBE, 0x41, 0xA0, 0x40, 0x49, 0x0F, 0xD0);
    TFLT("ADDF", "299792000, F3",  "X'B4F38E4D, F3",
         0xBE, 0xC1, 0xA0, 0x4D, 0x8E, 0xF3, 0xB4);
    TFLT("ADDL", "2.7182818284590451, F2", "X'6957148B0ABF0540, F2",
         0xBE, 0x80, 0xA0, 0x40, 0x05, 0xBF, 0x0A, 0x8B, 0x14, 0x57, 0x69);
    TFLT("ADDL", "6.6260701499999998E-34, F4", "X'113102DE0B860B39, F4",
         0xBE, 0x00, 0xA1, 0x39, 0x0B, 0x86, 0x0B, 0xDE, 0x02, 0x31, 0x11);
#endif

    disassembler.setOption("gnu-as", "enable");
    ATEST(0x1000, "SHSW", "0x00100F(PC)", 0xBD, 0xDD, 0x0F);

#if !defined(LIBASM_NS32000_NOFPU)
    TFLT("ADDF", "0F3.14159012, F1", "0xD00F4940, F1",
         0xBE, 0x41, 0xA0, 0x40, 0x49, 0x0F, 0xD0);
    TFLT("ADDF", "0F299792000, F3",  "0xB4F38E4D, F3",
         0xBE, 0xC1, 0xA0, 0x4D, 0x8E, 0xF3, 0xB4);
    TFLT("ADDL", "0L2.7182818284590451, F2", "0x6957148B0ABF0540, F2",
         0xBE, 0x80, 0xA0, 0x40, 0x05, 0xBF, 0x0A, 0x8B, 0x14, 0x57, 0x69);
    TFLT("ADDL", "0L6.6260701499999998E-34, F4", "0x113102DE0B860B39, F4",
         0xBE, 0x00, 0xA1, 0x39, 0x0B, 0x86, 0x0B, 0xDE, 0x02, 0x31, 0x11);
#endif
}
// clang-format on

const CpuSpec SPEC{NS32032,
#if defined(LIBASM_NS32000_NOFPU)
        FPU_NONE,
#else
        FPU_NS32081,
#endif
#if defined(LIBASM_NS32000_NOMMU)
        MMU_NONE
#else
        MMU_NS32082
#endif
};

void assert_unknown(
        const char *file, int line, Config::opcode_t opc, Config::opcode_t prefix) {
    if (isPrefixCode(SPEC, prefix)) {
        __VASSERT(file, line, UNKNOWN_INSTRUCTION, "", 0x0000, "", "", prefix, opc);
    } else {
        __VASSERT(file, line, UNKNOWN_INSTRUCTION, "", 0x0000, "", "", prefix);
    }
}

void assert_unknown(const char *file, int line, Config::opcode_t opc) {
    __VASSERT(file, line, UNKNOWN_INSTRUCTION, "", 0x0000, "", "", opc);
}

#define UNKNOWN(...) assert_unknown(__FILE__, __LINE__, __VA_ARGS__)

void test_illegal() {
    // Format 0: |cond|1010|
    UNKN(0xFA);

    // Format 3: |_gen_|_op| |011111|ii|
    for (auto gen = 0x00; gen <= 0x1F; gen++) {
        for (auto ii = 0; ii <= 3; ii++) {
            static constexpr Config::opcode_t opc[] = {0x1, 0x3, 0x5, 0x7, 0x8, 0x9, 0xB, 0xD, 0xF};
            for (size_t i = 0; i < sizeof(opc); i++) {
                if (ii != 2) {
                    UNKNOWN((gen << 3) | (opc[i] >> 1), (opc[i] << 7) | (0x7C | ii));
                }
            }
            if (ii == 3) {
                UNKNOWN((gen << 3) | (0x2 >> 1), (0x2 << 7) | 0x7C | ii);  // BICPSR
                UNKNOWN((gen << 3) | (0x6 >> 1), (0x6 << 7) | 0x7C | ii);  // BISPSR
            }
            if (ii != 3) {
                UNKNOWN((gen << 3) | (0x0 >> 1), (0x0 << 7) | 0x7C | ii);  // CXPD
                UNKNOWN((gen << 3) | (0x4 >> 1), (0x4 << 7) | 0x7C | ii);  // JUMP
                UNKNOWN((gen << 3) | (0xC >> 1), (0xC << 7) | 0x7C | ii);  // JSR
            }
        }
    }

    // Format 4: |gen1_|gen| |2_|_op_|ii|
    for (auto gen2 = 0x00; gen2 <= 0x1F; gen2++) {
        for (auto ii = 0; ii <= 3; ii++) {
            if (ii != 3)
                UNKNOWN((gen2 << 6) | (0x9 << 2) | ii, 0x0E);  // ADDR
        }
    }

    // Format 5: |00000|sho| |r|0|_op_|ii| |0000|1110|
    for (auto ii = 0; ii <= 3; ii++) {
        for (auto data = 0; data <= 0xF; data++) {
            for (auto opc = 0x4; opc <= 0xF; opc++) {
                UNKNOWN((data << 7) | (opc << 2) | ii, 0x0E);
            }
            if (ii != 3)
                UNKNOWN((data << 7) | (0x2 << 2) | ii, 0x0E);  // SETCFG
        }
        if (ii == 2) {
            UNKNOWN(0x00 | (0x0 << 2) | ii, 0x0E);  // MOVSi
            UNKNOWN(0x00 | (0x1 << 2) | ii, 0x0E);  // CMPSi
            UNKNOWN(0x00 | (0x3 << 2) | ii, 0x0E);  // SKPSi
        }
        if (ii != 0) {
            UNKNOWN(0x80 | (0x0 << 2) | ii, 0x0E);  // MOVST
            UNKNOWN(0x80 | (0x1 << 2) | ii, 0x0E);  // CMPST
            UNKNOWN(0x80 | (0x3 << 2) | ii, 0x0E);  // SKPST
        }
    }

    // Format 6: |gen1_|gen| |2_|_op_|ii|0100|1110|
    for (auto gen2 = 0x00; gen2 <= 0x1F; gen2++) {
        for (auto ii = 0; ii <= 3; ii++) {
            static constexpr Config::opcode_t opc[] = {0x4, 0xA};
            for (size_t i = 0; i < sizeof(opc); i++) {
                UNKNOWN((gen2 << 6) | (opc[i] << 2) | ii, 0x4E);
            }
        }
    }

    // Format 7: |gen1_|gen| |2_|_op_|ii| |1100|1110|
    for (auto gen2 = 0x00; gen2 <= 0x1F; gen2++) {
        for (auto ii = 0; ii <= 3; ii++) {
            UNKNOWN((gen2 << 6) | (0xA << 2) | ii, 0xCE);
            if (ii != 0) {
                UNKNOWN((gen2 << 6) | (0x4 << 2) | ii, 0xCE);  // MOVXBW
                UNKNOWN((gen2 << 6) | (0x5 << 2) | ii, 0xCE);  // MOVZBW
            }
            if (ii == 2 || ii == 3) {
                UNKNOWN((gen2 << 6) | (0x6 << 2) | ii, 0xCE);  // MOVZiD
                UNKNOWN((gen2 << 6) | (0x7 << 2) | ii, 0xCE);  // MOVXiD
            }
        }
    }

    // Format 8: |gen1_|gen| |2_|reg|o|ii| |op|10|1110|
    for (auto gen2 = 0x00; gen2 <= 0x1F; gen2++) {
        for (auto ii = 0; ii <= 3; ii++) {
            for (auto reg = 0; reg <= 7; reg++) {
                if (reg != 1 && reg != 3) {
                    auto opc = 6;
                    UNKNOWN((gen2 << 6) | (reg << 3) | (opc & 4) | ii,
                            (opc << 6) | 0x2E);  // MOVSUi/MOVUSi
                }
                if (ii != 3) {
                    auto opc = 1;
                    UNKNOWN((gen2 << 6) | (reg << 3) | (opc & 4) | ii, (opc << 6) | 0x6E);  // CVP
                }
            }
        }
    }

    // Format 9: |gen1_|gen| |2_|_op|f|ii| |0011|1110|
    for (auto gen2 = 0x00; gen2 <= 0x1F; gen2++) {
        for (auto ii = 0; ii <= 3; ii++) {
            for (auto f = 0; f <= 4; f += 4) {
                if (ii != 3) {
                    UNKNOWN((gen2 << 6) | (1 << 3) | f | ii, 0x3E);  // LFSR
                    UNKNOWN((gen2 << 6) | (6 << 3) | f | ii, 0x3E);  // SFSR
                }
            }
            if (ii != 3)
                UNKNOWN((gen2 << 6) | (2 << 3) | 0 | ii, 0x3E);  // MOVLF
            if (ii != 2)
                UNKNOWN((gen2 << 6) | (3 << 3) | 4 | ii, 0x3E);  // MOVFL
        }
    }

    // Format 10
    UNKNOWN(0x7E);

    // Format 11: |gen1_|gen| |2_|_op_|0f| |1011|1110|
    for (int8_t gen2 = 0x00; gen2 <= 0x1F; gen2++) {
        static constexpr Config::opcode_t opc[] = {0x3, 0x6, 0x7, 0x9, 0xA, 0xB, 0xE, 0xF};
        for (size_t i = 0; i < sizeof(opc); i++) {
            for (auto f = 0; f <= 1; f++) {
                UNKNOWN((gen2 << 6) | (opc[i] << 2) | f, 0xBE);
            }
        }
    }

    // Format 12
    UNKNOWN(0xFE);

    // Format 13
    UNKNOWN(0x9E);

    // Format 14: |gen1_|sho| |t|0|_op_|ii| |0001|1110|
    for (auto ii = 0; ii <= 3; ii++) {
        for (auto data = 0x0; data <= 0xF; data++) {
            if (ii != 3) {
                UNKNOWN((data << 7) | (2 << 2) | ii, 0x1E);  // LMR
                UNKNOWN((data << 7) | (3 << 2) | ii, 0x1E);  // SMR
            }
            for (auto opc = 0x4; opc <= 0xF; opc++) {
                UNKNOWN((data << 7) | (opc << 2) | ii, 0x1E);
            }
        }
        if (ii != 3) {
            UNKNOWN(0x80 | (0 << 2) | ii, 0x1E);  // RDVAL
            UNKNOWN(0x80 | (1 << 2) | ii, 0x1E);  // WRVAL
        }
    }

    // Format 15
    for (auto nnn = 0; nnn <= 7; nnn++) {
        UNKNOWN((nnn << 5) | 0x16);
    }
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
#if !defined(LIBASM_NS32000_NOFPU)
    RUN_TEST(test_format_9_fpu);
    RUN_TEST(test_format_11_fpu);
#endif
#if !defined(LIBASM_NS32000_NOMMU)
    RUN_TEST(test_format_8_mmu);
    RUN_TEST(test_format_14_mmu);
#endif
    RUN_TEST(test_generic_addressing);
    RUN_TEST(test_formatter);
    RUN_TEST(test_illegal);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

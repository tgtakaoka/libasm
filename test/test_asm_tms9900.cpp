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

#include "asm_tms9900.h"
#include "test_asm_helper.h"

using namespace libasm;
using namespace libasm::tms9900;
using namespace libasm::test;

AsmTms9900 as9900;
Assembler &assembler(as9900);

static bool is99105() {
    return strcmp_P("99105", assembler.config().cpu_P()) == 0;
}

static bool is9995() {
    return strcmp_P("9995", assembler.config().cpu_P()) == 0 || is99105();
}

static bool is9980() {
    return strcmp_P("9980", assembler.config().cpu_P()) == 0;
}

static void set_up() {
    assembler.reset();
}

static void tear_down() {
    symtab.reset();
}

// clang-format off
void test_cpu() {
    EQUALS("cpu 9900", true,   assembler.setCpu("9900"));
    EQUALS_P("cpu 9900", "9900", assembler.config().cpu_P());

    EQUALS("cpu 9980", true,   assembler.setCpu("9980"));
    EQUALS_P("cpu 9980", "9980", assembler.config().cpu_P());

    EQUALS("cpu 9995", true,   assembler.setCpu("9995"));
    EQUALS_P("cpu 9995", "9995", assembler.config().cpu_P());

    EQUALS("cpu 99105", true,    assembler.setCpu("99105"));
    EQUALS_P("cpu 99105", "99105", assembler.config().cpu_P());

    EQUALS("cpu tms9900", true,   assembler.setCpu("tms9900"));
    EQUALS_P("cpu tms9900", "9900", assembler.config().cpu_P());

    EQUALS("cpu tms9980", true,   assembler.setCpu("tms9980"));
    EQUALS_P("cpu tms9980", "9980", assembler.config().cpu_P());

    EQUALS("cpu tms9995", true,   assembler.setCpu("tms9995"));
    EQUALS_P("cpu tms9995", "9995", assembler.config().cpu_P());

    EQUALS("cpu tms99105", true,    assembler.setCpu("tms99105"));
    EQUALS_P("cpu tms99105", "99105", assembler.config().cpu_P());
}

static void test_inh() {
    TEST("IDLE", 0x0340);
    TEST("RSET", 0x0360);
    TEST("RTWP", 0x0380);
    TEST("CKON", 0x03A0);
    TEST("CKOF", 0x03C0);
    TEST("LREX", 0x03E0);

    if (is99105()) {
        // TMS99105
        TEST("RTWP 0", 0x0380);
        TEST("RTWP 1", 0x0381);
        TEST("RTWP 2", 0x0382);
        TEST("RTWP 4", 0x0384);
        ERRT("RTWP 3", OPERAND_NOT_ALLOWED, "3", 0x0383);
    } else {
        COMM("RTWP 0", "0", 0x0380);
        COMM("RTWP 1", "1", 0x0380);
    }
}

static void test_imm() {
    TEST("LWPI >1234", 0x02E0, 0x1234);
    TEST("LIMI >89AB", 0x0300, 0x89AB);

    symtab.intern(0x1234, "sym1234");
    symtab.intern(0x89AB, "sym89AB");

    TEST("LWPI sym1234", 0x02E0, 0x1234);
    TEST("LIMI sym89AB", 0x0300, 0x89AB);
}

static void test_reg() {
    TEST("stwp r14", 0x02AE);
    TEST("STST R15", 0x02CF);

    if (is9995()) {
        // TMS9995
        TEST("LST R0", 0x0080);
        TEST("LWP R1", 0x0091);
    } else {
        ERUI("LST R0");
        ERUI("LWP R1");
    }
}

static void test_reg_imm() {
    TEST("LI   R0, >0000",    0x0200, 0x0000);
    TEST("AI   R1, +1",       0x0221, 0x0001);
    TEST("ANDI R8, >00FF",    0x0248, 0x00FF);
    TEST("ORI  R14, #>00FF",  0x026E, 0xFF00);
    TEST("ORI  R14, >1200++>0034", 0x026E, 0x1234);
    TEST("ORI  R14, >1234&>0FF0",  0x026E, 0x0230);
    TEST("ORI  R14, >00FF&&>0FF0", 0x026E, 0x0F0F);
    TEST("CI   R15, -1",    0x028F, 0xFFFF);
    TEST("CI   R15, -1//8", 0x028F, 0x00FF);

    symtab.intern(0x1234, "sym1234");

    TEST("LI R2,sym1234", 0x0202, 0x1234);

    if (is99105()) {
        // TMS99105
        TEST("BLSK R3,>4567",   0x00B3, 0x4567);
        TEST("BLSK R3,sym1234", 0x00B3, 0x1234);
    } else {
        ERUI("BLSK R3,>4567");
    }
}

static void test_cnt_reg() {
    TEST("SRA R1,R0", 0x0801);
    TEST("SRL R4,12", 0x09C4);
    TEST("SLA R8,4",  0x0A48);
    TEST("SRC R9,15", 0x0BF9);
    ERRT("SRC R9,16", OVERFLOW_RANGE, "16", 0x0B09);
}

static void test_src() {
    TEST("BLWP @>1000", 0x0420, 0x1000);
    TEST("BLWP @>3FFE", 0x0420, 0x3FFE);
    ERRT("BLWP @>3FFF", OPERAND_NOT_ALIGNED, "@>3FFF", 0x0420, 0x3FFF);
    if (is9980()) {
        ERRT("BLWP @>4000", OVERFLOW_RANGE,  "@>4000", 0x0420, 0x4000);
        ERRT("BLWP @>FFFE", OVERFLOW_RANGE,  "@>FFFE", 0x0420, 0xFFFE);
    } else {
        TEST("BLWP @>4000", 0x0420, 0x4000);
        TEST("BLWP @>FFFE", 0x0420, 0xFFFE);
    }
    TEST("B    R13",    0x044D);
    TEST("X    *R10",   0x049A);
    TEST("CLR  *R12+",  0x04FC);
    TEST("NEG  R0",     0x0500);
    TEST("INV  @>1234", 0x0560, 0x1234);
    TEST("INC  @2(R7)", 0x05A7, 0x0002);
    TEST("INCT R7",     0x05C7);
    TEST("DEC  @-2(R7)",0x0627, 0xFFFE);
    TEST("DECT R7",     0x0647);
    TEST("BL   R13",    0x068D);
    TEST("SWPB *R1",    0x06D1);
    TEST("SETO R12",    0x070C);
    TEST("ABS  @8(R3)", 0x0763, 0x0008);
    ERRT("ABS  @8(R0)", REGISTER_NOT_ALLOWED, "R0)");

    if (is9995()) {
        // TMS9995
        TEST("DIVS R2",         0x0182);
        TEST("DIVS *R3",        0x0193);
        TEST("DIVS @>1234",     0x01A0, 0x1234);
        TEST("DIVS @>1000(R4)", 0x01A4, 0x1000);
        TEST("DIVS *R5+",       0x01B5);
        TEST("MPYS R0",         0x01C0);
        TEST("MPYS @2(R8)",     0x01E8, 0x0002);
        TEST("MPYS *R15+",      0x01FF);
    } else {
        ERUI("DIVS R2");
        ERUI("MPYS R0");
    }

    if (is99105()) {
        // TMS99105
        TEST("TMB  @>0123(R15),7", 0x0C09, 0x01EF, 0x0123);
        TEST("TCMB R0,0",          0x0C0A, 0x0000);
        TEST("TSMB *R2,15",        0x0C0B, 0x03D2);
        TEST("BIND @>2223(R1)", 0x0161, 0x2223);
        TEST("EVAD R5",         0x0105);
        ERRT("TMB  *R1+,7",  OPERAND_NOT_ALLOWED, "*R1+,7",  0x0C09, 0x01F1);
        ERRT("TCMB *R1+,0",  OPERAND_NOT_ALLOWED, "*R1+,0",  0x0C0A, 0x0031);
        ERRT("TSMB *R1+,15", OPERAND_NOT_ALLOWED, "*R1+,15", 0x0C0B, 0x03F1);
    } else {
        ERUI("TMB  @>0123(R15),7");
        ERUI("TCMB R0,0");
        ERUI("TSMB *R2,15");
        ERUI("BIND @>2223(R1)");
        ERUI("EVAD R5");
    }

    symtab.intern(-2, "neg2");
    symtab.intern(0x1000, "sym1000");
    symtab.intern(0x1234, "sym1234");
    symtab.intern(0x9876, "sym9876");

    TEST("BLWP @sym1234",     0x0420, 0x1234);
    TEST("DEC  @neg2(R7)",    0x0627, 0xFFFE);

    if (is9995()) {
        // TMS9995
        TEST("DIVS @sym1234",     0x01A0, 0x1234);
        TEST("DIVS @sym1000(R4)", 0x01A4, 0x1000);
    }

    if (is99105()) {
        // TMS99105
        TEST("BIND @sym1000(R1)", 0x0161, 0x1000);
    }
}

static void test_reg_src() {
    TEST("COC R1,R2",         0x2081);
    TEST("CZC @>1234(R3),R7", 0x25E3, 0x1234);
    TEST("XOR @2(R5),R4",     0x2925, 0x0002);
    TEST("MPY R4,R2",         0x3884);
    TEST("DIV R14,R12",       0x3F0E);

    symtab.intern(0x1234, "sym1234");
    symtab.intern(0x0002, "offset2");

    TEST("CZC @sym1234(R3),R7", 0x25E3, 0x1234);
    TEST("XOR @offset2(R5),R4", 0x2925, 0x0002);
}

static void test_cnt_src() {
    TEST("LDCR *R13+,16",  0x303D);
    TEST("STCR @2(R4),15",                       0x37E4, 0x0002);
    ERRT("STCR @2(R4),17", OVERFLOW_RANGE, "17", 0x3464, 0x0002);
    ERRT("STCR @2(R4),R0", OPERAND_NOT_ALLOWED, "@2(R4),R0");

    symtab.intern(7, "size7");
    symtab.intern(2, "offset2");

    TEST("STCR @offset2(R4),size7", 0x35E4, 0x0002);
    TEST("STCR @offset2(R4),16",    0x3424, 0x0002);
    TEST("STCR @>1000(R4),size7",   0x35E4, 0x1000);

    if (is99105()) {
        // TMS99105
        TEST("SRAM @offset2(R4),15", 0x001C, 0x43E4, 0x0002);
        TEST("SLAM R11,R0",          0x001D, 0x400B);
        TEST("SLAM *R13+,1",         0x001D, 0x407D);
        ERRT("SRAM R11,R2", REGISTER_NOT_ALLOWED, "R2", 0x001C, 0x400B);
        ERRT("SLAM R11,0",  OPERAND_NOT_ALLOWED,  "0",  0x001D, 0x400B);
        ERRT("SLAM R11,16", OVERFLOW_RANGE,       "16", 0x001D, 0x400B);
    } else {
        ERUI("SRAM R11,R0");
        ERUI("SLAM *R13+,1");
    }
}

static void test_xop_src() {
    TEST("XOP @>1234,0",                             0x2C20, 0x1234);
    TEST("XOP @>1234,15",                            0x2FE0, 0x1234);
    ERRT("XOP @>1234,16", OVERFLOW_RANGE,      "16", 0x2C20, 0x1234);
    ERRT("XOP @>1234,R0", OPERAND_NOT_ALLOWED, "@>1234,R0");

    symtab.intern(10, "xop10");
    symtab.intern(0x1234, "sym1234");

    TEST("XOP @sym1234,xop10",   0x2EA0, 0x1234);
    TEST("XOP @sym1234(R1),8",   0x2E21, 0x1234);
    TEST("XOP @>1234(R1),xop10", 0x2EA1, 0x1234);
}

static void test_dst_src() {
    TEST("SZC  @>1234(R10),@>5678(R11)", 0x4AEA, 0x1234, 0x5678);
    TEST("SZC  @>1235(R10),@>5679(R11)", 0x4AEA, 0x1235, 0x5679);
    TEST("SZC  @>1234,@>3456",                                        0x4820, 0x1234, 0x3456);
    ERRT("SZC  @>1234,@>3457", OPERAND_NOT_ALIGNED, "@>3457",         0x4820, 0x1234, 0x3457);
    ERRT("SZC  @>1235,@>3456", OPERAND_NOT_ALIGNED, "@>1235,@>3456",  0x4820, 0x1235, 0x3456);
    ERRT("SZC  @>1235,@>3457", OPERAND_NOT_ALIGNED, "@>1235,@>3457",  0x4820, 0x1235, 0x3457);
    TEST("SZCB @>1234,@>3456", 0x5820, 0x1234, 0x3456);
    TEST("SZCB @>1234,@>3457", 0x5820, 0x1234, 0x3457);
    TEST("SZCB @>1235,@>3456", 0x5820, 0x1235, 0x3456);
    TEST("SZCB @>1235,@>3457", 0x5820, 0x1235, 0x3457);
    TEST("S    *R10,*R11",       0x66DA);
    TEST("SB   *R10+,*R11+",     0x7EFA);
    TEST("C    *R10+,*R10+",     0x8EBA);
    TEST("CB   *R10+,*R11+",     0x9EFA);
    TEST("A    @>2000,R11",      0xA2E0, 0x2000);
    TEST("AB   R10,@>4000(R11)", 0xBACA, 0x4000);
    TEST("MOV  @0(R10),@1(R11)", 0xCAEA, 0x0000, 0x0001);
    TEST("MOVB R10,R11",         0xD2CA);
    TEST("SOC  @>1234,@>2345(R11)", 0xEAE0, 0x1234, 0x2345);
    TEST("SOCB @>1234(R10),@>2345", 0xF82A, 0x1234, 0x2345);

    symtab.intern(0x0000, "zero");
    symtab.intern(0x1234, "sym1234");
    symtab.intern(0x4000, "sym4000");
    symtab.intern(0x2345, "sym2345");

    TEST("SZC  @sym1234(R10),@sym2345(R11)", 0x4AEA, 0x1234, 0x2345);
    TEST("SZCB @sym1234,@sym2345",      0x5820, 0x1234, 0x2345);
    TEST("AB   R10,@sym4000(R11)",      0xBACA, 0x4000);
    TEST("MOV  @zero(R10),@>0001(R11)", 0xCAEA, 0x0000, 0x0001);
    TEST("SOC  @sym1234,@sym2345(R11)", 0xEAE0, 0x1234, 0x2345);
    TEST("SOCB @sym1234(R10),@sym2345", 0xF82A, 0x1234, 0x2345);

    if (is99105()) {
        // TMS99105
        TEST("SM @sym2345(R10),@sym4000(R11)", 0x0029, 0x4AEA, 0x2345, 0x4000);
        TEST("SM @sym1234,@sym4000",           0x0029, 0x4820, 0x1234, 0x4000);
        TEST("SM R10,@sym4000(R11)",           0x0029, 0x4ACA, 0x4000);
        TEST("AM @zero(R10),@1(R11)",          0x002A, 0x4AEA, 0x0000, 0x0001);
        TEST("AM @sym1234,@sym2345(R11)",      0x002A, 0x4AE0, 0x1234, 0x2345);
        TEST("AM @sym2345(R10),@sym4000",      0x002A, 0x482A, 0x2345, 0x4000);
    } else {
        ERUI("SM R10,@sym4000(R11)");
        ERUI("AM @zero(R10),@1(R11)");
    }
}

static void test_rel() {
    ATEST(0x1000, "JMP >1002", 0x1000);
    ATEST(0x1000, "JLT >1000", 0x11FF);
    ATEST(0x1000, "JLE >1100", 0x127F);
    ATEST(0x1000, "JEQ >0F02", 0x1380);
    ATEST(0x1000, "JHE >1004", 0x1401);
    ATEST(0x1000, "JGT >1000", 0x15FF);
    ATEST(0x1000, "JNE >1008", 0x1603);
    ATEST(0x1000, "JNC >100A", 0x1704);
    ATEST(0x1000, "JOC >0FFE", 0x18FE);
    ATEST(0x1000, "JNO >0FFC", 0x19FD);
    ATEST(0x1000, "JL  >0FFA", 0x1AFC);
    ATEST(0x1000, "JH  >0FF8", 0x1BFB);
    ATEST(0x1000, "JOP >0FF6", 0x1CFA);

    AERRT(0x1000, "JMP >1001", OPERAND_NOT_ALIGNED, ">1001", 0x1000);
    ATEST(0x1000, "JMP $+256",                                   0x107F);
    ATEST(0x1000, "JMP $-254",                                   0x1080);
    AERRT(0x1000, "JMP $+258", OPERAND_TOO_FAR,         "$+258", 0x1080);
    AERRT(0x1000, "JMP $-256", OPERAND_TOO_FAR,         "$-256", 0x107F);

    symtab.intern(0x0F02, "sym0F02");
    symtab.intern(0x1000, "sym1000");
    symtab.intern(0x1002, "sym1002");
    symtab.intern(0x1100, "sym1100");

    ATEST(0x1000, "JMP sym1002", 0x1000);
    ATEST(0x1000, "JLT sym1000", 0x11FF);
    ATEST(0x1000, "JLE sym1100", 0x127F);
    ATEST(0x1000, "JEQ sym0F02", 0x1380);
}

static void test_cru_off() {
    TEST("SBO 0",    0x1D00);
    TEST("SBZ >7F",  0x1E7F);
    TEST("TB  -128", 0x1F80);

    symtab.intern(0x00, "zero");
    symtab.intern(127, "off127");
    symtab.intern(-128, "offm128");

    TEST("SBO zero",   0x1D00);
    TEST("SBZ off127", 0x1E7F);
    TEST("TB offm128", 0x1F80);
}

static void test_comment() {
    COMM("IDLE      ; comment", "; comment", 0x0340);
    COMM("LWPI >1234; comment", "; comment", 0x02E0, 0x1234);
    COMM("IDLE         comment", "comment", 0x0340);
    COMM("LWPI >1234   comment", "comment", 0x02E0, 0x1234);
    COMM("STWP R14     comment", "comment", 0x02AE);
    COMM("LI   R0 , 0  comment", "comment", 0x0200, 0x0000);
    COMM("X *R10       comment", "comment", 0x049A);
    ERRT("X * R10      comment",  UNKNOWN_OPERAND, "* R10      comment");
    COMM("CLR *R12+    comment", "comment", 0x04FC);
    ERRT("CLR * R12+   comment",  UNKNOWN_OPERAND, "* R12+   comment");
    COMM("BLWP @ >3456 comment", "comment", 0x0420, 0x3456);
    COMM("SBO  0       comment", "comment", 0x1D00);
    COMM("INC  @ 2 ( R7 )  comment", "comment", 0x05A7, 0x0002);
    COMM("LDCR *R13+  , 16 comment", "comment", 0x303D);
    ERRT("LDCR * R13+ , 16 comment", UNKNOWN_OPERAND, "* R13+ , 16 comment");
    COMM("SZC  @ >1234 ( R10 ) , @ >2345 ( R11 ) comment", "comment", 0x4AEA, 0x1234, 0x2345);
    ACOMM(0x1000, "JMP >1002 comment", "comment", 0x1000);
    COMM("CLR  *R12 +     comment", "+     comment", 0x04DC);
    ERRT("LDCR *R13 +, 16 comment", OPERAND_NOT_ALLOWED, "*R13 +, 16 comment");

    COMM("BYTE -128, 255 comment", "comment", 0x80FF);
    COMM("TEXT 'TEXT'    comment", "comment", 0x5445, 0x5854);
    COMM("DATA -128, 255 comment", "comment", 0xFF80, 0x00FF);
}

static void test_undef() {
    ERUS("LWPI UNDEF",    "UNDEF", 0x02E0, 0x0000);
    ERUS("LI   R0,UNDEF", "UNDEF", 0x0200, 0x0000);
    ERUS("SRL  R4,UNDEF", "UNDEF", 0x0904);
    ERUS("SBZ  UNDEF",    "UNDEF", 0x1E00);
    ERUS("BLWP @UNDEF",   "UNDEF", 0x0420, 0x0000);
    ERUS("INC  @UNDEF(R7)",       "UNDEF(R7)",       0x05A7, 0x0000);
    ERUS("CZC  @UNDEF(R3),R7",    "UNDEF(R3),R7",    0x25E3, 0x0000);
    ERUS("STCR @UNDEF(R4),15",    "UNDEF(R4),15",    0x37E4, 0x0000);
    ERUS("STCR @2(R4),UNDEF",     "UNDEF",            0x3424, 0x0002);
    ERUS("STCR @UNDEF(R4),UNDEF", "UNDEF(R4),UNDEF", 0x3424, 0x0000);
    ERUS("XOP  @UNDEF,15",    "UNDEF,15",    0x2FE0, 0x0000);
    ERUS("XOP  @>1234,UNDEF", "UNDEF",       0x2C20, 0x1234);
    ERUS("XOP  @UNDEF,UNDEF", "UNDEF,UNDEF", 0x2C20, 0x0000);
    ERUS("SZC  @UNDEF(R10),@>2345(R11)", "UNDEF(R10),@>2345(R11)", 0x4AEA, 0x0000, 0x2345);
    ERUS("SZC  @>1234(R10),@UNDEF(R11)", "UNDEF(R11)",             0x4AEA, 0x1234, 0x0000);
    ERUS("SZC  @UNDEF(R10),@UNDEF(R11)", "UNDEF(R10),@UNDEF(R11)", 0x4AEA, 0x0000, 0x0000);

    AERUS(0x1000, "JMP UNDEF", "UNDEF", 0x1000);
    AERUS(0x1000, "JNE UNDEF", "UNDEF", 0x1600);
}

static void test_data_constant() {
    BTEST("BYTE -128, 255", 0x80, 0xFF);
    BTEST(R"(BYTE 'A', '"')", 0x41, 0x22);
    BTEST("BYTE '9'-'0'",   0x09);
    BTEST("BYTE ''''",      0x27);
    ERRT("BYTE '''",        MISSING_CLOSING_QUOTE, "'''");
    BTEST("BYTE 'A''B',0",  0x41, 0x27, 0x42, 0x00);
    ERRT("BYTE 'A''B,0",    MISSING_CLOSING_QUOTE, "'A''B,0");
    BTEST("DATA -128, 255", 0xFF, 0x80, 0x00, 0xFF);
    BTEST("DATA 'X'",       0x58, 0x00);
    BTEST("DATA 'X'+0",     0x00, 0x58);
    BTEST("DATA 'A''B'",    0x41, 0x27, 0x42, 0x00);
    ERRT("DATA  'A''B",     MISSING_CLOSING_QUOTE, "'A''B");
    BTEST("TEXT 'X'",       0x58);
    BTEST(R"(TEXT 'A''B"C')", 0x41, 0x27, 0x42, 0x22, 0x43);
    ERRT(R"(TEXT 'A''B"C)",   MISSING_CLOSING_QUOTE, R"('A''B"C)");
    BERRT("BYTE 1, UNDEF, 2", UNDEFINED_SYMBOL, "UNDEF, 2", 0x01, 0x00, 0x02);
    BERRT("DATA 1, UNDEF, 2", UNDEFINED_SYMBOL, "UNDEF, 2", 0x00, 0x01, 0x00, 0x00, 0x00, 0x02);
    BERRT("TEXT 1, UNDEF, 2", UNDEFINED_SYMBOL, "UNDEF, 2", 0x01, 0x00, 0x02);

    BERRT("TEXT '"
         "1234567890" "1234567890" "1234567890" "1234567890" "1234567890" "1234567890"
         "1234567890'",
         NO_MEMORY,
         "567890'",
         0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30,
         0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30,
         0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30,
         0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30,
         0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30,
         0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30,
         0x31, 0x32, 0x33, 0x34);

    BERRT("DATA "
         ">1234, >5678, >9ABC, >DEF0, >1234, >5678, >9ABC, >DEF0, "
         ">1234, >5678, >9ABC, >DEF0, >1234, >5678, >9ABC, >DEF0, "
         ">1234, >5678, >9ABC, >DEF0, >1234, >5678, >9ABC, >DEF0, "
         ">1234, >5678, >9ABC, >DEF0, >1234, >5678, >9ABC, >DE, "
         ">AAAA, >BBBB",
         NO_MEMORY,
         ">AAAA, >BBBB",
         0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
         0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
         0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
         0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0x00, 0xDE);
}

// clang-format on

void run_tests(const char *cpu) {
    assembler.setCpu(cpu);
    RUN_TEST(test_inh);
    RUN_TEST(test_imm);
    RUN_TEST(test_reg);
    RUN_TEST(test_reg_imm);
    RUN_TEST(test_cnt_reg);
    RUN_TEST(test_src);
    RUN_TEST(test_reg_src);
    RUN_TEST(test_cnt_src);
    RUN_TEST(test_xop_src);
    RUN_TEST(test_dst_src);
    RUN_TEST(test_rel);
    RUN_TEST(test_cru_off);
    RUN_TEST(test_comment);
    RUN_TEST(test_undef);
    RUN_TEST(test_data_constant);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

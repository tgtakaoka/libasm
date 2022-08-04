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
    return strcmp_P("99105", assembler.cpu_P()) == 0;
}

static bool is9995() {
    return strcmp_P("9995", assembler.cpu_P()) == 0 || is99105();
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
    EQUALS_P("cpu 9900", "9900", assembler.cpu_P());

    EQUALS("cpu 9995", true,   assembler.setCpu("9995"));
    EQUALS_P("cpu 9995", "9995", assembler.cpu_P());

    EQUALS("cpu 99105", true,    assembler.setCpu("99105"));
    EQUALS_P("cpu 99105", "99105", assembler.cpu_P());

    EQUALS("cpu tms9900", true,   assembler.setCpu("tms9900"));
    EQUALS_P("cpu tms9900", "9900", assembler.cpu_P());

    EQUALS("cpu tms9995", true,   assembler.setCpu("tms9995"));
    EQUALS_P("cpu tms9995", "9995", assembler.cpu_P());

    EQUALS("cpu tms99105", true,    assembler.setCpu("tms99105"));
    EQUALS_P("cpu tms99105", "99105", assembler.cpu_P());
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
        ERRT("RTWP 3", OPERAND_NOT_ALLOWED, "3");
    } else {
        ERRT("RTWP 0", OPERAND_NOT_ALLOWED, "0");
    }
}

static void test_imm() {
    TEST("LWPI 1234H", 0x02E0, 0x1234);
    TEST("LIMI 89ABH", 0x0300, 0x89AB);

    symtab.intern(0x1234, "sym1234");
    symtab.intern(0x89AB, "sym89AB");

    TEST("LWPI sym1234", 0x02E0, 0x1234);
    TEST("LIMI sym89AB", 0x0300, 0x89AB);
}

static void test_reg() {
    TEST("STWP R14", 0x02AE);
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
    TEST("LI   R0,0000H", 0x0200, 0x0000);
    TEST("AI   R1,+1",    0x0221, 0x0001);
    TEST("ANDI R8,377O",  0x0248, 0x00FF);
    TEST("ORI  R14,~11111111B", 0x026E, 0xFF00);
    TEST("CI   R15,-1",   0x028F, 0xFFFF);

    symtab.intern(0x1234, "sym1234");

    TEST("LI R2,sym1234", 0x0202, 0x1234);

    if (is99105()) {
        // TMS99105
        TEST("BLSK R3,4567H",   0x00B3, 0x4567);
        TEST("BLSK R3,sym1234", 0x00B3, 0x1234);
    } else {
        ERUI("BLSK R3,4567H");
    }
}

static void test_cnt_reg() {
    TEST("SRA R1,R0", 0x0801);
    TEST("SRL R4,12", 0x09C4);
    TEST("SLA R8,4",  0x0A48);
    TEST("SRC R9,15", 0x0BF9);
    ERRT("SRC R9,16", OVERFLOW_RANGE, "16");
}

static void test_src() {
    TEST("BLWP @9876H", 0x0420, 0x9876);
    TEST("B    R13",    0x044D);
    TEST("X    *R10",   0x049A);
    TEST("CLR  *R12+",  0x04FC);
    TEST("NEG  R0",     0x0500);
    TEST("INV  @1234H", 0x0560, 0x1234);
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
        TEST("DIVS @1234H",     0x01A0, 0x1234);
        TEST("DIVS @1000H(R4)", 0x01A4, 0x1000);
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
        TEST("TMB  @0123H(R15),7", 0x0C09, 0x01EF, 0x0123);
        TEST("TCMB R0,0",          0x0C0A, 0x0000);
        TEST("TSMB *R2,15",        0x0C0B, 0x03D2);
        TEST("BIND @2223H(R1)", 0x0161, 0x2223);
        TEST("EVAD R5",         0x0105);
        ERRT("TMB  *R1+,7",  OPERAND_NOT_ALLOWED, "*R1+,7");
        ERRT("TCMB *R1+,0",  OPERAND_NOT_ALLOWED, "*R1+,0");
        ERRT("TSMB *R1+,15", OPERAND_NOT_ALLOWED, "*R1+,15");
    } else {
        ERUI("TMB  @0123H(R15),7");
        ERUI("TCMB R0,0");
        ERUI("TSMB *R2,15");
        ERUI("BIND @2223H(R1)");
        ERUI("EVAD R5");
    }

    symtab.intern(-2, "neg2");
    symtab.intern(0x1000, "sym1000");
    symtab.intern(0x1234, "sym1234");
    symtab.intern(0x9876, "sym9876");

    TEST("BLWP @sym9876",     0x0420, 0x9876);
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
    TEST("CZC @1234H(R3),R7", 0x25E3, 0x1234);
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
    TEST("STCR @2(R4),15", 0x37E4, 0x0002);
    ERRT("STCR @2(R4),R0", OPERAND_NOT_ALLOWED, "@2(R4),R0");
    ERRT("STCR @2(R4),17", OVERFLOW_RANGE, "17");

    symtab.intern(7, "size7");
    symtab.intern(2, "offset2");

    TEST("STCR @offset2(R4),size7", 0x35E4, 0x0002);
    TEST("STCR @offset2(R4),16",    0x3424, 0x0002);
    TEST("STCR @1000H(R4),size7",   0x35E4, 0x1000);

    if (is99105()) {
        // TMS99105
        TEST("SRAM @offset2(R4),15", 0x001C, 0x43E4, 0x0002);
        TEST("SLAM R11,R0",          0x001D, 0x400B);
        TEST("SLAM *R13+,1",         0x001D, 0x407D);
        ERRT("SRAM R11,R2", REGISTER_NOT_ALLOWED, "R2");
        ERRT("SLAM R11,0",  OPERAND_NOT_ALLOWED,  "0");
        ERRT("SLAM R11,16", OVERFLOW_RANGE,       "16");
    } else {
        ERUI("SRAM R11,R0");
        ERUI("SLAM *R13+,1");
    }
}

static void test_xop_src() {
    TEST("XOP @9876H,0",  0x2C20, 0x9876);
    TEST("XOP @9876H,15", 0x2FE0, 0x9876);
    ERRT("XOP @9876H,R0", OPERAND_NOT_ALLOWED, "@9876H,R0");
    ERRT("XOP @9876H,16", OVERFLOW_RANGE,      "16");

    symtab.intern(10, "xop10");
    symtab.intern(0x9876, "sym9876");

    TEST("XOP @sym9876,xop10",   0x2EA0, 0x9876);
    TEST("XOP @sym9876(R1),8",   0x2E21, 0x9876);
    TEST("XOP @1234H(R1),xop10", 0x2EA1, 0x1234);
}

static void test_dst_src() {
    TEST("SZC  @1234H(R10),@5678H(R11)", 0x4AEA, 0x1234, 0x5678);
    TEST("SZCB @1234H,@5678H",   0x5820, 0x1234, 0x5678);
    TEST("S    *R10,*R11",       0x66DA);
    TEST("SB   *R10+,*R11+",     0x7EFA);
    TEST("C    *R10+,*R10+",     0x8EBA);
    TEST("CB   *R10+,*R11+",     0x9EFA);
    TEST("A    @2000H,R11",      0xA2E0, 0x2000);
    TEST("AB   R10,@4000H(R11)", 0xBACA, 0x4000);
    TEST("MOV  @0(R10),@1(R11)", 0xCAEA, 0x0000, 0x0001);
    TEST("MOVB R10,R11",         0xD2CA);
    TEST("SOC  @1234H,@5678H(R11)", 0xEAE0, 0x1234, 0x5678);
    TEST("SOCB @1234H(R10),@5678H", 0xF82A, 0x1234, 0x5678);

    symtab.intern(0x0000, "zero");
    symtab.intern(0x1234, "sym1234");
    symtab.intern(0x4000, "sym4000");
    symtab.intern(0x5678, "sym5678");

    TEST("SZC  @sym1234(R10),@sym5678(R11)", 0x4AEA, 0x1234, 0x5678);
    TEST("SZCB @sym1234,@sym5678",      0x5820, 0x1234, 0x5678);
    TEST("AB   R10,@sym4000(R11)",      0xBACA, 0x4000);
    TEST("MOV  @zero(R10),@0001H(R11)", 0xCAEA, 0x0000, 0x0001);
    TEST("SOC  @sym1234,@sym5678(R11)", 0xEAE0, 0x1234, 0x5678);
    TEST("SOCB @sym1234(R10),@sym5678", 0xF82A, 0x1234, 0x5678);

    if (is99105()) {
        // TMS99105
        TEST("SM @sym1234(R10),@sym5678(R11)", 0x0029, 0x4AEA, 0x1234, 0x5678);
        TEST("SM @sym1234,@sym5678",           0x0029, 0x4820, 0x1234, 0x5678);
        TEST("SM R10,@sym4000(R11)",           0x0029, 0x4ACA, 0x4000);
        TEST("AM @zero(R10),@1(R11)",          0x002A, 0x4AEA, 0x0000, 0x0001);
        TEST("AM @sym1234,@sym5678(R11)",      0x002A, 0x4AE0, 0x1234, 0x5678);
        TEST("AM @sym1234(R10),@sym5678",      0x002A, 0x482A, 0x1234, 0x5678);
    } else {
        ERUI("SM R10,@sym4000(R11)");
        ERUI("AM @zero(R10),@1(R11)");
    }
}

static void test_rel() {
    ATEST(0x1000, "JMP 1002H", 0x1000);
    ATEST(0x1000, "JLT 1000H", 0x11FF);
    ATEST(0x1000, "JLE 1100H", 0x127F);
    ATEST(0x1000, "JEQ 0F02H", 0x1380);
    ATEST(0x1000, "JHE 1004H", 0x1401);
    ATEST(0x1000, "JGT 1000H", 0x15FF);
    ATEST(0x1000, "JNE 1008H", 0x1603);
    ATEST(0x1000, "JNC 100AH", 0x1704);
    ATEST(0x1000, "JOC 0FFEH", 0x18FE);
    ATEST(0x1000, "JNO 0FFCH", 0x19FD);
    ATEST(0x1000, "JL  0FFAH", 0x1AFC);
    ATEST(0x1000, "JH  0FF8H", 0x1BFB);
    ATEST(0x1000, "JOP 0FF6H", 0x1CFA);

    ERRT("JMP 1001H", OPERAND_NOT_ALIGNED, "1001H");
    ATEST(0x1000, "JMP $+256", 0x107F);
    ATEST(0x1000, "JMP $-254", 0x1080);
    AERRT(0x1000, "JMP $+258", OPERAND_TOO_FAR, "$+258");
    AERRT(0x1000, "JMP $-256", OPERAND_TOO_FAR, "$-256");

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
    TEST("SBZ 7FH",  0x1E7F);
    TEST("TB  -128", 0x1F80);

    symtab.intern(0x00, "zero");
    symtab.intern(127, "off127");
    symtab.intern(-128, "off_128");

    TEST("SBO zero",   0x1D00);
    TEST("SBZ off127", 0x1E7F);
    TEST("TB off_128", 0x1F80);
}

static void test_comment() {
    TEST("IDLE         ; comment", 0x0340);
    TEST("LWPI 1234H   ; comment", 0x02E0, 0x1234);
    TEST("STWP R14     ; comment", 0x02AE);
    TEST("LI   R0 , 0  ; comment", 0x0200, 0x0000);
    TEST("X *R10   ; comment",     0x049A);
    ERRT("X * R10  ; comment",     UNKNOWN_OPERAND, "* R10  ; comment");
    TEST("CLR *R12+  ; comment",   0x04FC);
    ERRT("CLR * R12+ ; comment",   UNKNOWN_OPERAND, "* R12+ ; comment");
    TEST("BLWP @ 9876H ; comment", 0x0420, 0x9876);
    TEST("SBO  0       ; comment", 0x1D00);
    TEST("INC  @ 2 ( R7 )  ; comment", 0x05A7, 0x0002);
    TEST("LDCR *R13+  , 16 ; comment", 0x303D);
    ERRT("LDCR * R13+ , 16 ; comment", UNKNOWN_OPERAND, "* R13+ , 16 ; comment");
    TEST("SZC  @ 1234H ( R10 ) , @ 5678H ( R11 ) ; comment", 0x4AEA, 0x1234, 0x5678);
    ATEST(0x1000, "JMP 1002H ; comment", 0x1000);
    ERRT("CLR  *R12 + ; comment",     GARBAGE_AT_END, "+ ; comment");
    ERRT("LDCR *R13 +, 16 ; comment", GARBAGE_AT_END, "+, 16 ; comment");
}

static void test_undefined_symbol() {
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
    ERUS("XOP  @9876H,UNDEF", "UNDEF",       0x2C20, 0x9876);
    ERUS("XOP  @UNDEF,UNDEF", "UNDEF,UNDEF", 0x2C20, 0x0000);
    ERUS("SZC  @UNDEF(R10),@5678H(R11)", "UNDEF(R10),@5678H(R11)", 0x4AEA, 0x0000, 0x5678);
    ERUS("SZC  @1234H(R10),@UNDEF(R11)", "UNDEF(R11)",             0x4AEA, 0x1234, 0x0000);
    ERUS("SZC  @UNDEF(R10),@UNDEF(R11)", "UNDEF(R10),@UNDEF(R11)", 0x4AEA, 0x0000, 0x0000);

    AERUS(0x1000, "JMP UNDEF", "UNDEF", 0x1000);
    AERUS(0x1000, "JNE UNDEF", "UNDEF", 0x1600);
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
    RUN_TEST(test_undefined_symbol);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

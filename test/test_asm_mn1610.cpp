/*
 * Copyright 2021 Tadashi G. Takaoka
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

#include "asm_mn1610.h"
#include "test_asm_helper.h"

using namespace libasm;
using namespace libasm::mn1610;
using namespace libasm::test;

AsmMn1610 as1610;
Assembler &assembler(as1610);

static bool is1613() {
    return strcmp_P("MN1613", assembler.config().cpu_P()) == 0;
}

static bool is1613a() {
    return strcmp_P("MN1613A", assembler.config().cpu_P()) == 0;
}

static void set_up() {
    assembler.reset();
}

static void tear_down() {
    symtab.reset();
}

// clang-format off
void test_cpu() {
    EQUALS("cpu 1610", true,     assembler.setCpu("1610"));
    EQUALS_P("cpu 1610", "MN1610", assembler.config().cpu_P());

    EQUALS("cpu 1613", true,     assembler.setCpu("1613"));
    EQUALS_P("cpu 1613", "MN1613", assembler.config().cpu_P());

    EQUALS("cpu 1613A", true,      assembler.setCpu("1613A"));
    EQUALS_P("cpu 1613A", "MN1613A", assembler.config().cpu_P());

    EQUALS("cpu mn1610", true,     assembler.setCpu("mn1610"));
    EQUALS_P("cpu MN1610", "MN1610", assembler.config().cpu_P());

    EQUALS("cpu mn1613", true,     assembler.setCpu("mn1613"));
    EQUALS_P("cpu MN1613", "MN1613", assembler.config().cpu_P());

    EQUALS("cpu mn1613a", true,      assembler.setCpu("mn1613a"));
    EQUALS_P("cpu MN1613A", "MN1613A", assembler.config().cpu_P());
}

static void test_transfer() {
    TEST(         "l r1, 0x10",            0xC000|(0<<11)|(1<<8)|0x10);
    TEST(         "L R1, X'10'",           0xC000|(0<<11)|(1<<8)|0x10);
    ERRT(         "L STR, 0x10",           OPERAND_NOT_ALLOWED, "STR, 0x10");
    ERRT(         "L R5, 0x10",            OPERAND_NOT_ALLOWED, "R5, 0x10");
    ERRT(         "L X2, 0x10",            OPERAND_NOT_ALLOWED, "X2, 0x10");
    ATEST(0x1000, "L R2, *-128",                                            0xC000|(1<<11)|(2<<8)|0x80);
    ATEST(0x1000, "l r2, 0x0f80-*(ic)",                                     0xC000|(1<<11)|(2<<8)|0x80);
    AERRT(0x1000, "L R2, *-129",           OPERAND_TOO_FAR, "*-129",        0xC000|(1<<11)|(2<<8)|0x7F);
    AERRT(0x1000, "L R2, 0x0F7F-*(IC)",    OPERAND_TOO_FAR, "0x0F7F-*(IC)", 0xC000|(1<<11)|(2<<8)|0x7F);
    AERRT(0x1000, "L R2, *+128",           OPERAND_TOO_FAR, "*+128",        0xC000|(1<<11)|(2<<8)|0x80);
    AERRT(0x1000, "L R2, 0x1080-*(IC)",    OPERAND_TOO_FAR, "0x1080-*(IC)", 0xC000|(1<<11)|(2<<8)|0x80);
    TEST(         "L R3, (0xFF)",          0xC000|(2<<11)|(3<<8)|0xFF);
    ATEST(0x1000, "L R4, (*+127)",         0xC000|(3<<11)|(4<<8)|0x7F);
    ATEST(0x1000, "L R4, (0x107F-*(IC))",  0xC000|(3<<11)|(4<<8)|0x7F);
    AERRT(0x1000, "L R4, (*-129)",         OPERAND_TOO_FAR, "(*-129)",        0xC000|(3<<11)|(4<<8)|0x7F);
    AERRT(0x1000, "L R4, (0x0F7F-*(IC))",  OPERAND_TOO_FAR, "(0x0F7F-*(IC))", 0xC000|(3<<11)|(4<<8)|0x7F);
    AERRT(0x1000, "L R4, (*+128)",         OPERAND_TOO_FAR, "(*+128)",        0xC000|(3<<11)|(4<<8)|0x80);
    AERRT(0x1000, "L R4, (0x1080-*(IC))",  OPERAND_TOO_FAR, "(0x1080-*(IC))", 0xC000|(3<<11)|(4<<8)|0x80);
    TEST(         "L R0, 128(X0)",         0xC000|(4<<11)|(0<<8)|0x80);
    TEST(         "L SP, 255(X1)",         0xC000|(5<<11)|(5<<8)|0xFF);
    TEST(         "L R1, (0x10)(X0)",      0xC000|(6<<11)|(1<<8)|0x10);
    TEST(         "L R2, (0x80)(X1)",      0xC000|(7<<11)|(2<<8)|0x80);
    TEST(         "ST R1, 0x10",           0x8000|(0<<11)|(1<<8)|0x10);
    ERRT(         "st str, 0x10",          OPERAND_NOT_ALLOWED, "str, 0x10");
    ERRT(         "ST R5, 0x10",           OPERAND_NOT_ALLOWED, "R5, 0x10");
    ERRT(         "ST X2, 0x10",           OPERAND_NOT_ALLOWED, "X2, 0x10");
    ATEST(0x1000, "ST R2, *-128",          0x8000|(1<<11)|(2<<8)|0x80);
    ATEST(0x1000, "ST R2, 0x0F80-*(IC)",   0x8000|(1<<11)|(2<<8)|0x80);
    TEST(         "ST R3, (0xFF)",         0x8000|(2<<11)|(3<<8)|0xFF);
    ATEST(0x1000, "ST R4, (*+127)",        0x8000|(3<<11)|(4<<8)|0x7F);
    ATEST(0x1000, "ST R4, (0x107F-*(IC))", 0x8000|(3<<11)|(4<<8)|0x7F);
    TEST(         "ST R0, 128(X0)",        0x8000|(4<<11)|(0<<8)|0x80);
    TEST(         "ST SP, 255(X1)",        0x8000|(5<<11)|(5<<8)|0xFF);
    TEST(         "ST R1, (0x10)(X0)",     0x8000|(6<<11)|(1<<8)|0x10);
    TEST(         "ST R2, (0x80)(X1)",     0x8000|(7<<11)|(2<<8)|0x80);

    TEST("MV  R4, SP",       0x7808|(4<<8)|(0<<4)|5);
    TEST("mv  sp, str, skp", 0x7808|(5<<8)|(1<<4)|6);
    TEST("MV  STR, R0, M",   0x7808|(6<<8)|(2<<4)|0);
    TEST("MV  R0, R1, PZ",   0x7808|(0<<8)|(3<<4)|1);
    TEST("MVB R0, R1, Z",    0x7800|(0<<8)|(4<<4)|1);
    TEST("MVB R1, R2, NZ",   0x7800|(1<<8)|(5<<4)|2);
    TEST("MVB R2, R3, MZ",   0x7800|(2<<8)|(6<<4)|3);
    TEST("MVB R3, R4, P",    0x7800|(3<<8)|(7<<4)|4);

    TEST("BSWP R0, STR, LM",  0x7008|(0<<8)|(15<<4)|6);
    TEST("BSWP R1, SP, LPZ",  0x7008|(1<<8)|(14<<4)|5);
    TEST("BSWP R2, R4, LP",   0x7008|(2<<8)|(13<<4)|4);
    TEST("BSWP R3, R3, LMZ",  0x7008|(3<<8)|(12<<4)|3);
    TEST("BSWP R4, R2, ONZ",  0x7008|(4<<8)|(11<<4)|2);
    TEST("BSWP SP, R1, OZ",   0x7008|(5<<8)|(10<<4)|1);
    TEST("BSWP STR, R0, ENZ", 0x7008|(6<<8)|(9<<4)|0);
    TEST("DSWP R0, STR, EZ",  0x7000|(0<<8)|(8<<4)|6);
    TEST("DSWP R1, SP, P",    0x7000|(1<<8)|(7<<4)|5);
    TEST("DSWP R2, R4, MZ",   0x7000|(2<<8)|(6<<4)|4);
    TEST("DSWP R3, R3, NZ",   0x7000|(3<<8)|(5<<4)|3);
    TEST("DSWP R4, R2, Z",    0x7000|(4<<8)|(4<<4)|2);
    TEST("DSWP SP, R1, PZ",   0x7000|(5<<8)|(3<<4)|1);
    TEST("DSWP STR, R0, M",   0x7000|(6<<8)|(2<<4)|0);

    TEST("PUSH R0",  0x2001|(0<<8));
    TEST("PUSH R1",  0x2001|(1<<8));
    TEST("PUSH R2",  0x2001|(2<<8));
    TEST("PUSH R3",  0x2001|(3<<8));
    TEST("PUSH X0",  0x2001|(3<<8));
    TEST("PUSH R4",  0x2001|(4<<8));
    TEST("PUSH SP",  0x2001|(5<<8));
    TEST("PUSH STR", 0x2001|(6<<8));
    TEST("POP  R0",  0x2002|(0<<8));
    TEST("POP  R1",  0x2002|(1<<8));
    TEST("POP  R2",  0x2002|(2<<8));
    TEST("POP  R3",  0x2002|(3<<8));
    TEST("POP  R4",  0x2002|(4<<8));
    TEST("POP  X1",  0x2002|(4<<8));
    TEST("POP  SP",  0x2002|(5<<8));
    TEST("POP  STR", 0x2002|(6<<8));

    if (is1613() || is1613a()) {
        TEST("LD  R1, 0x1234",       0x2708|(0<<4)|1, 0x1234);
        TEST("LD  R1, CSBR, 0x1234", 0x2708|(0<<4)|1, 0x1234);
        TEST("LD  R2, SSBR, 0x1234", 0x2708|(1<<4)|2, 0x1234);
        TEST("LD  R3, TSR0, 0x1234", 0x2708|(2<<4)|3, 0x1234);
        TEST("LD  R4, TSR1, 0x1234", 0x2708|(3<<4)|4, 0x1234);
        TEST("LR  SP, TSR1, (R1)",   0x2000|(5<<8)|(1<<6)|(3<<4)|0);
        TEST("LR  R0, TSR0, -(R2)",  0x2000|(0<<8)|(2<<6)|(2<<4)|1);
        TEST("LR  R1, SSBR, (R3)+",  0x2000|(1<<8)|(3<<6)|(1<<4)|2);
        TEST("LR  R1, SSBR, (X0)+",  0x2000|(1<<8)|(3<<6)|(1<<4)|2);
        TEST("LR  R2, (R4)",         0x2000|(2<<8)|(1<<6)|(0<<5)|3);
        TEST("LR  R2, (X1)",         0x2000|(2<<8)|(1<<6)|(0<<5)|3);
        ERRT("LR  R2, (R0)",         REGISTER_NOT_ALLOWED, "R0)");
        TEST("STD R1, 0x1234",       0x2748|(0<<4)|1, 0x1234);
        TEST("STD R2, SSBR, 0x1234", 0x2748|(1<<4)|2, 0x1234);
        TEST("STD R3, TSR0, 0x1234", 0x2748|(2<<4)|3, 0x1234);
        TEST("STD R4, TSR1, 0x1234", 0x2748|(3<<4)|4, 0x1234);
        TEST("STR SP, TSR1, -(R1)",  0x2004|(5<<8)|(2<<6)|(3<<4)|0);
        TEST("STR R0, TSR0, (R2)",   0x2004|(0<<8)|(1<<6)|(2<<4)|1);
        TEST("STR R1, SSBR, (R3)+",  0x2004|(1<<8)|(3<<6)|(1<<4)|2);
        TEST("STR R2, (R4)",         0x2004|(2<<8)|(1<<6)|(0<<5)|3);

        // TODO: Support Segment register directive
        if (is1613()) {
            TEST("LD  R1, 0x1FFFF",      0x2708|(0<<4)|1, 0xFFFF);
            TEST("LD  R1, CSBR, 0x1234", 0x2708|(0<<4)|1, 0x1234);
            TEST("LD  R2, SSBR, 0x1234", 0x2708|(1<<4)|2, 0x1234);
        }

        TEST("MVWR R0, (R1), EZ",  0x7F08|(8<<4)|0);
        ERRT("MVWR R1, (R1), EZ",  OPERAND_NOT_ALLOWED, "R1, (R1), EZ");
        ERRT("MVWR R0, (R1)+, EZ", OPERAND_NOT_ALLOWED, "R0, (R1)+, EZ");
        TEST("MVWR R0, (R2), ENZ", 0x7F08|(9<<4)|1);
        TEST("MVWR R0, (R3), OZ",  0x7F08|(10<<4)|2);
        TEST("MVWR R0, (R4)",      0x7F08|(0<<4)|3);
        ERRT("MVWR R0, -(R4)",     OPERAND_NOT_ALLOWED, "R0, -(R4)");
        TEST("MVBR R0, (R1)",      0x7F00|(0<<4)|0);
        TEST("MVBR R0, (R2), ONZ", 0x7F00|(11<<4)|1);
        TEST("MVBR R0, (R3), LMZ", 0x7F00|(12<<4)|2);
        TEST("MVBR R0, (R4), LP",  0x7F00|(13<<4)|3);

        TEST("MVWI R0, 0x1234, LPZ", 0x780F|(0<<8)|(14<<4), 0x1234);
        TEST("MVWI R2, 0x5678, LM",  0x780F|(2<<8)|(15<<4), 0x5678);
        TEST("MVWI SP, 0x9ABC, SKP", 0x780F|(5<<8)|(1<<4),  0x9ABC);
        TEST("MVWI STR, 0xDEF0",     0x780F|(6<<8)|(0<<4),  0xDEF0);

        TEST("BSWR R0, (R1), SKP", 0x7708|(1<<4)|0);
        TEST("BSWR R0, (R2)",      0x7708|(0<<4)|1);
        TEST("BSWR R0, (R3), Z",   0x7708|(4<<4)|2);
        TEST("BSWR R0, (R4), NZ",  0x7708|(5<<4)|3);
        TEST("DSWR R0, (R1), SKP", 0x7700|(1<<4)|0);
        TEST("DSWR R0, (R2)",      0x7700|(0<<4)|1);
        TEST("DSWR R0, (R3), Z",   0x7700|(4<<4)|2);
        TEST("DSWR R0, (R4), NZ",  0x7700|(5<<4)|3);

        TEST("PSHM ", 0x170F);
        TEST("POPM ", 0x1707);

        TEST("MVI R0, 0",     0x0800|(0<<8)|0);
        TEST("MVI R1, 255",   0x0800|(1<<8)|0xFF);
        TEST("MVI R1, X'FF'", 0x0800|(1<<8)|0xFF);
        ERRT("MVI R1, 256",   OVERFLOW_RANGE, "256", 0x0800|(1<<8)|0x00);
        TEST("MVI R2, C'4'",  0x0800|(2<<8)|0x34);
        TEST("MVI R2, -128",  0x0800|(2<<8)|0x80);
        TEST("MVI R3, 127",   0x0800|(3<<8)|0x7F);
        TEST("MVI R4, 1",     0x0800|(4<<8)|1);
        TEST("MVI SP, 2",     0x0800|(5<<8)|2);
        TEST("MVI STR, 64",   0x0800|(6<<8)|64);
    }
}

static void test_integer() {
    TEST("A R0, STR, SKP", 0x5808|(0<<8)|(1<<4)|6);
    TEST("A R1, SP, M",    0x5808|(1<<8)|(2<<4)|5);
    TEST("A R2, R4, PZ",   0x5808|(2<<8)|(3<<4)|4);
    TEST("A R3, R3, Z",    0x5808|(3<<8)|(4<<4)|3);
    TEST("A R4, R2, NZ",   0x5808|(4<<8)|(5<<4)|2);
    TEST("A SP, R1, MZ",   0x5808|(5<<8)|(6<<4)|1);
    TEST("A STR, R0, P",   0x5808|(6<<8)|(7<<4)|0);
    TEST("A R0, SP, EZ",   0x5808|(0<<8)|(8<<4)|5);
    TEST("AI R0, 1, PZ",   0x4800|(0<<8)|(3<<4)|1);
    TEST("AI R1, 2",       0x4800|(1<<8)|(0<<4)|2);
    ERRT("AI R1, -1, Z",   OVERFLOW_RANGE, "-1, Z", 0x4800|(1<<8)|(4<<4)|15);
    TEST("AI R2, 3, M",    0x4800|(2<<8)|(2<<4)|3);
    TEST("AI R3, 13, P",   0x4800|(3<<8)|(7<<4)|13);
    TEST("AI R4, 14, SKP", 0x4800|(4<<8)|(1<<4)|14);
    TEST("AI SP, 15, NZ",  0x4800|(5<<8)|(5<<4)|15);
    ERRT("AI SP, 16, NZ",  OVERFLOW_RANGE, "16, NZ", 0x4800|(5<<8)|(5<<4)|0);
    TEST("AI STR, 0, Z",   0x4800|(6<<8)|(4<<4)|0);

    TEST("S R0, STR, ENZ", 0x5800|(0<<8)|(9<<4)|6);
    TEST("S R1, SP, OZ",   0x5800|(1<<8)|(10<<4)|5);
    TEST("S R2, R4, ONZ",  0x5800|(2<<8)|(11<<4)|4);
    TEST("S R3, R3, LMZ",  0x5800|(3<<8)|(12<<4)|3);
    TEST("S R4, R2, LP",   0x5800|(4<<8)|(13<<4)|2);
    TEST("S SP, R1, LPZ",  0x5800|(5<<8)|(14<<4)|1);
    TEST("S STR, R0, LM",  0x5800|(6<<8)|(15<<4)|0);
    TEST("S R0, SP",       0x5800|(0<<8)|(0<<4)|5);
    TEST("SI R0, 1, PZ",   0x4000|(0<<8)|(3<<4)|1);
    ERRT("SI R0, -1, PZ",  OVERFLOW_RANGE, "-1, PZ", 0x4000|(0<<8)|(3<<4)|15);
    TEST("SI R1, 2",       0x4000|(1<<8)|(0<<4)|2);
    TEST("SI R2, 3, M",    0x4000|(2<<8)|(2<<4)|3);
    TEST("SI R3, 13, P",   0x4000|(3<<8)|(7<<4)|13);
    TEST("SI R4, 14, SKP", 0x4000|(4<<8)|(1<<4)|14);
    TEST("SI SP, 15, NZ",  0x4000|(5<<8)|(5<<4)|15);
    ERRT("SI SP, 16, NZ",  OVERFLOW_RANGE, "16, NZ", 0x4000|(5<<8)|(5<<4)|0);
    TEST("SI STR, 0, Z",   0x4000|(6<<8)|(4<<4)|0);

    TEST("C R2, R4, PZ",   0x5008|(2<<8)|(3<<4)|4);
    TEST("C R3, R3, Z",    0x5008|(3<<8)|(4<<4)|3);
    TEST("C R4, R2, NZ",   0x5008|(4<<8)|(5<<4)|2);
    TEST("C SP, R1, MZ",   0x5008|(5<<8)|(6<<4)|1);
    TEST("C STR, R0, P",   0x5008|(6<<8)|(7<<4)|0);
    TEST("C R0, SP, EZ",   0x5008|(0<<8)|(8<<4)|5);
    TEST("CB R0, STR, ENZ", 0x5000|(0<<8)|(9<<4)|6);
    TEST("CB R1, SP, OZ",   0x5000|(1<<8)|(10<<4)|5);
    TEST("CB R2, R4, ONZ",  0x5000|(2<<8)|(11<<4)|4);
    TEST("CB R3, R3, LMZ",  0x5000|(3<<8)|(12<<4)|3);
    TEST("CB R4, R2, LP",   0x5000|(4<<8)|(13<<4)|2);
    TEST("CB SP, R1, LPZ",  0x5000|(5<<8)|(14<<4)|1);
    TEST("CB STR, R0, LM",  0x5000|(6<<8)|(15<<4)|0);
    TEST("CB R0, SP",       0x5000|(0<<8)|(0<<4)|5);

    TEST("LAD R0, STR, ENZ", 0x6800|(0<<8)|(9<<4)|6);
    TEST("LAD R1, SP, OZ",   0x6800|(1<<8)|(10<<4)|5);
    TEST("LAD R2, R4, ONZ",  0x6800|(2<<8)|(11<<4)|4);
    TEST("LAD R3, R3, LMZ",  0x6800|(3<<8)|(12<<4)|3);
    TEST("LAD R4, R2, LP",   0x6800|(4<<8)|(13<<4)|2);
    TEST("LAD SP, R1, LPZ",  0x6800|(5<<8)|(14<<4)|1);
    TEST("LAD STR, R0, LM",  0x6800|(6<<8)|(15<<4)|0);
    TEST("LAD R0, SP",       0x6800|(0<<8)|(0<<4)|5);

    if (is1613() || is1613a()) {
        TEST("AWR R0, (R1), EZ",    0x5F08|(8<<4)|0);
        TEST("AWR R0, (R2), ENZ",   0x5F08|(9<<4)|1);
        TEST("AWR R0, (R3), OZ",    0x5F08|(10<<4)|2);
        TEST("AWR R0, (R4)",        0x5F08|(0<<4)|3);
        TEST("AWI R0, 0x1234, LPZ", 0x580F|(0<<8)|(14<<4), 0x1234);
        TEST("AWI R2, 0x5678, LM",  0x580F|(2<<8)|(15<<4), 0x5678);
        TEST("AWI SP, 0x9ABC, SKP", 0x580F|(5<<8)|(1<<4),  0x9ABC);
        TEST("AWI STR, 0xDEF0",     0x580F|(6<<8)|(0<<4),  0xDEF0);
        TEST("AWI X0, -32768",      0x580F|(3<<8)|(0<<4),  0x8000);
        TEST("AWI X1, 65535",       0x580F|(4<<8)|(0<<4),  0xFFFF);
        ERRT("AWI R1, -32769",      OVERFLOW_RANGE, "-32769", 0x580F|(1<<8)|(0<<4), 0x7FFF);
        ERRT("AWI R3, 65536",       OVERFLOW_RANGE, "65536",  0x580F|(3<<8)|(0<<4), 0x0000);

        TEST("SWR R0, (R1)",        0x5F00|(0<<4)|0);
        TEST("SWR R0, (R2), ONZ",   0x5F00|(11<<4)|1);
        TEST("SWR R0, (R3), LMZ",   0x5F00|(12<<4)|2);
        TEST("SWR R0, (R4), LP",    0x5F00|(13<<4)|3);
        TEST("SWI R0, 0x1234, LPZ", 0x5807|(0<<8)|(14<<4), 0x1234);
        TEST("SWI R2, 0x5678, LM",  0x5807|(2<<8)|(15<<4), 0x5678);
        TEST("SWI SP, 0x9ABC, SKP", 0x5807|(5<<8)|(1<<4),  0x9ABC);
        TEST("SWI STR, 0xDEF0",     0x5807|(6<<8)|(0<<4),  0xDEF0);

        TEST("CWR R0, (R1), EZ",    0x5708|(8<<4)|0);
        TEST("CWR R0, (R2), ENZ",   0x5708|(9<<4)|1);
        TEST("CWR R0, (R3), OZ",    0x5708|(10<<4)|2);
        TEST("CWR R0, (R4)",        0x5708|(0<<4)|3);
        TEST("CWI R0, 0x1234, LPZ", 0x500F|(0<<8)|(14<<4), 0x1234);
        TEST("CWI R0, -256,   LPZ", 0x500F|(0<<8)|(14<<4), 0xFF00);
        TEST("CWI R2, 0x5678, LM",  0x500F|(2<<8)|(15<<4), 0x5678);
        TEST("CWI SP, 0x9ABC, SKP", 0x500F|(5<<8)|(1<<4),  0x9ABC);
        TEST("CWI STR, 0xDEF0",     0x500F|(6<<8)|(0<<4),  0xDEF0);

        TEST("CBR R0, (R1)",      0x5700|(0<<4)|0);
        TEST("CBR R0, (R2), ONZ", 0x5700|(11<<4)|1);
        TEST("CBR R0, (R3), LMZ", 0x5700|(12<<4)|2);
        TEST("CBR R0, (R4), LP",  0x5700|(13<<4)|3);
        TEST("CBI R0, 52, LPZ",   0x5007|(0<<8)|(14<<4), 0x0034);
        TEST("CBI R0, -128, LPZ", 0x5007|(0<<8)|(14<<4), 0xFF80);
        TEST("CBI R0, 257, LPZ",  0x5007|(0<<8)|(14<<4), 0x0101);
        TEST("CBI R2, 120, LM",   0x5007|(2<<8)|(15<<4), 0x0078);
        TEST("CBI SP, 188, SKP",  0x5007|(5<<8)|(1<<4),  0x00BC);
        TEST("CBI STR, 240",      0x5007|(6<<8)|(0<<4),  0x00F0);

        TEST("NEG R0",     0x1F00|(0<<3)|0);
        TEST("NEG R0, 0",  0x1F00|(0<<3)|0);
        TEST("NEG R1, C",  0x1F00|(1<<3)|1);
        TEST("NEG R1, 1",  0x1F00|(1<<3)|1);
        ERRT("NEG R1, 2",  ILLEGAL_CONSTANT, "2",  0x1F00|(0<<3)|1);
        ERRT("NEG R1, -1", ILLEGAL_CONSTANT, "-1", 0x1F00|(1<<3)|1);
        TEST("NEG R2",     0x1F00|(0<<3)|2);
        TEST("NEG R3, C",  0x1F00|(1<<3)|3);
        TEST("NEG R4",     0x1F00|(0<<3)|4);
        TEST("NEG SP, C",  0x1F00|(1<<3)|5);
        TEST("NEG STR",    0x1F00|(0<<3)|6);

        TEST("AD DR0, (R1), EZ",     0x4F04|(8<<4)|(0<<3)|0);
        ERRT("AD DR0, (R0), EZ",     REGISTER_NOT_ALLOWED, "R0), EZ");
        TEST("AD DR0, (R2), C, ENZ", 0x4F04|(9<<4)|(1<<3)|1);
        TEST("AD DR0, (R3), OZ",     0x4F04|(10<<4)|(0<<3)|2);
        TEST("AD DR0, (X1), C",      0x4F04|(0<<4)|(1<<3)|3);
        TEST("SD DR0, (R1), C, EZ",  0x4704|(8<<4)|(1<<3)|0);
        TEST("SD DR0, (R2), ENZ",    0x4704|(9<<4)|(0<<3)|1);
        TEST("SD DR0, (X0), C, OZ",  0x4704|(10<<4)|(1<<3)|2);
        TEST("SD DR0, (R4)",         0x4704|(0<<4)|(0<<3)|3);

        TEST("M DR0, (R1), EZ",  0x7F0C|(8<<4)|0);
        TEST("M DR0, (R2), ENZ", 0x7F0C|(9<<4)|1);
        TEST("M DR0, (R3), OZ",  0x7F0C|(10<<4)|2);
        TEST("M DR0, (R4)",      0x7F0C|(0<<4)|3);
        TEST("D DR0, (R1), EZ",  0x770C|(8<<4)|0);
        TEST("D DR0, (R2), ENZ", 0x770C|(9<<4)|1);
        TEST("D DR0, (R3), OZ",  0x770C|(10<<4)|2);
        TEST("D DR0, (R4)",      0x770C|(0<<4)|3);

        TEST("DAA R0, (R1), EZ",     0x5F04|(8<<4)|(0<<3)|0);
        TEST("DAA R0, (R2), C, ENZ", 0x5F04|(9<<4)|(1<<3)|1);
        TEST("DAA R0, (R3), OZ",     0x5F04|(10<<4)|(0<<3)|2);
        TEST("DAA R0, (R4), C",      0x5F04|(0<<4)|(1<<3)|3);
        TEST("DAS R0, (R1), C, EZ",  0x5704|(8<<4)|(1<<3)|0);
        TEST("DAS R0, (R2), ENZ",    0x5704|(9<<4)|(0<<3)|1);
        TEST("DAS R0, (R3), C, OZ",  0x5704|(10<<4)|(1<<3)|2);
        TEST("DAS R0, (R4)",         0x5704|(0<<4)|(0<<3)|3);

        TEST("LADR R0, (R1), EZ",  0x6F00|(8<<4)|0);
        TEST("LADR R0, (R2), ENZ", 0x6F00|(9<<4)|1);
        TEST("LADR R0, (R3), OZ",  0x6F00|(10<<4)|2);
        TEST("LADR R0, (R4)",      0x6F00|(0<<4)|3);
        TEST("LADI R0, 0x1234, LPZ", 0x6807|(0<<8)|(14<<4), 0x1234);
        TEST("LADI R2, 0x5678, LM",  0x6807|(2<<8)|(15<<4), 0x5678);
        TEST("LADI SP, 0x9ABC, SKP", 0x6807|(5<<8)|(1<<4),  0x9ABC);
        TEST("LADI STR, 0xDEF0",     0x6807|(6<<8)|(0<<4),  0xDEF0);
    }
}

static void test_float() {
    if (is1613() || is1613a()) {
        TEST("FA DR0, (R1), EZ",  0x6F0C|(8<<4)|0);
        ERRT("FA DR1, (R1), EZ",  OPERAND_NOT_ALLOWED, "DR1, (R1), EZ");
        ERRT("FA DR0, (R0), EZ",  REGISTER_NOT_ALLOWED, "R0), EZ");
        TEST("FA DR0, (R2), ENZ", 0x6F0C|(9<<4)|1);
        TEST("FA DR0, (X0), OZ",  0x6F0C|(10<<4)|2);
        TEST("FA DR0, (R4)",      0x6F0C|(0<<4)|3);
        TEST("FS DR0, (R1), EZ",  0x6F04|(8<<4)|0);
        TEST("FS DR0, (R2), ENZ", 0x6F04|(9<<4)|1);
        TEST("FS DR0, (R3), OZ",  0x6F04|(10<<4)|2);
        TEST("FS DR0, (X1)",      0x6F04|(0<<4)|3);
        TEST("FM DR0, (R1), EZ",  0x670C|(8<<4)|0);
        TEST("FM DR0, (R2), ENZ", 0x670C|(9<<4)|1);
        TEST("FM DR0, (R3), OZ",  0x670C|(10<<4)|2);
        TEST("FM DR0, (R4)",      0x670C|(0<<4)|3);
        TEST("FD DR0, (R1), EZ",  0x6704|(8<<4)|0);
        TEST("FD DR0, (R2), ENZ", 0x6704|(9<<4)|1);
        TEST("FD DR0, (R3), OZ",  0x6704|(10<<4)|2);
        TEST("FD DR0, (R4)",      0x6704|(0<<4)|3);

        TEST("FIX R0, DR0, EZ",  0x1F0F|(8<<4));
        ERRT("FIX R1, DR0, EZ",  OPERAND_NOT_ALLOWED, "R1, DR0, EZ");
        TEST("FIX R0, DR0, ENZ", 0x1F0F|(9<<4));
        TEST("FIX R0, DR0, OZ",  0x1F0F|(10<<4));
        TEST("FIX R0, DR0",      0x1F0F|(0<<4));
        TEST("FLT DR0, R0, EZ",  0x1F07|(8<<4));
        ERRT("FLT DR0, R1, EZ",  OPERAND_NOT_ALLOWED, "DR0, R1, EZ");
        TEST("FLT DR0, R0, ENZ", 0x1F07|(9<<4));
        TEST("FLT DR0, R0, OZ",  0x1F07|(10<<4));
        TEST("FLT DR0, R0",      0x1F07|(0<<4));
    }
}

static void test_logical() {
    TEST("AND R0, STR, SKP", 0x6808|(0<<8)|(1<<4)|6);
    TEST("AND R1, SP, M",    0x6808|(1<<8)|(2<<4)|5);
    TEST("AND R2, R4, PZ",   0x6808|(2<<8)|(3<<4)|4);
    TEST("AND R3, R3, Z",    0x6808|(3<<8)|(4<<4)|3);
    TEST("AND R4, R2, NZ",   0x6808|(4<<8)|(5<<4)|2);
    TEST("AND SP, R1, MZ",   0x6808|(5<<8)|(6<<4)|1);
    TEST("AND STR, R0, P",   0x6808|(6<<8)|(7<<4)|0);
    TEST("AND R0, SP, EZ",   0x6808|(0<<8)|(8<<4)|5);
    TEST("OR R0, STR, SKP", 0x6008|(0<<8)|(1<<4)|6);
    TEST("OR R1, SP, M",    0x6008|(1<<8)|(2<<4)|5);
    TEST("OR R2, R4, PZ",   0x6008|(2<<8)|(3<<4)|4);
    TEST("OR R3, R3, Z",    0x6008|(3<<8)|(4<<4)|3);
    TEST("OR R4, R2, NZ",   0x6008|(4<<8)|(5<<4)|2);
    TEST("OR SP, R1, MZ",   0x6008|(5<<8)|(6<<4)|1);
    TEST("OR STR, R0, P",   0x6008|(6<<8)|(7<<4)|0);
    TEST("OR R0, SP, EZ",   0x6008|(0<<8)|(8<<4)|5);
    TEST("EOR R0, STR, SKP", 0x6000|(0<<8)|(1<<4)|6);
    TEST("EOR R1, SP, M",    0x6000|(1<<8)|(2<<4)|5);
    TEST("EOR R2, R4, PZ",   0x6000|(2<<8)|(3<<4)|4);
    TEST("EOR R3, R3, Z",    0x6000|(3<<8)|(4<<4)|3);
    TEST("EOR R4, R2, NZ",   0x6000|(4<<8)|(5<<4)|2);
    TEST("EOR SP, R1, MZ",   0x6000|(5<<8)|(6<<4)|1);
    TEST("EOR STR, R0, P",   0x6000|(6<<8)|(7<<4)|0);
    TEST("EOR R0, SP, EZ",   0x6000|(0<<8)|(8<<4)|5);

    TEST(         "IMS 0x10",       0xC600|(0<<11)|0x10);
    ATEST(0x1000, "IMS *-128",      0xC600|(1<<11)|0x80);
    TEST(         "IMS (0xFF)",     0xC600|(2<<11)|0xFF);
    ATEST(0x1000, "IMS (*+127)",    0xC600|(3<<11)|0x7F);
    TEST(         "IMS 128(X0)",    0xC600|(4<<11)|0x80);
    TEST(         "IMS 255(X1)",    0xC600|(5<<11)|0xFF);
    TEST(         "IMS (0x10)(X0)", 0xC600|(6<<11)|0x10);
    TEST(         "IMS (0x80)(X1)", 0xC600|(7<<11)|0x80);
    TEST(         "DMS 0x10",       0x8600|(0<<11)|0x10);
    ATEST(0x1000, "DMS *-128",      0x8600|(1<<11)|0x80);
    TEST(         "DMS (0xFF)",     0x8600|(2<<11)|0xFF);
    ATEST(0x1000, "DMS (*+127)",    0x8600|(3<<11)|0x7F);
    TEST(         "DMS 128(X0)",    0x8600|(4<<11)|0x80);
    TEST(         "DMS 255(X1)",    0x8600|(5<<11)|0xFF);
    TEST(         "DMS (0x10)(X0)", 0x8600|(6<<11)|0x10);
    TEST(         "DMS (0x80)(X1)", 0x8600|(7<<11)|0x80);

    if (is1613() || is1613a()) {
        TEST("ANDR R0, (R1), EZ",    0x6F08|(8<<4)|0);
        TEST("ANDR R0, (R2), ENZ",   0x6F08|(9<<4)|1);
        TEST("ANDR R0, (R3), OZ",    0x6F08|(10<<4)|2);
        TEST("ANDR R0, (R4)",        0x6F08|(0<<4)|3);
        TEST("ANDI R0, 0x1234, LPZ", 0x680F|(0<<8)|(14<<4), 0x1234);
        TEST("ANDI R2, 0x5678, LM",  0x680F|(2<<8)|(15<<4), 0x5678);
        TEST("ANDI SP, 0x9ABC, SKP", 0x680F|(5<<8)|(1<<4),  0x9ABC);
        TEST("ANDI STR, 0xDEF0",     0x680F|(6<<8)|(0<<4),  0xDEF0);

        TEST("ORR R0, (R1), EZ",    0x6708|(8<<4)|0);
        TEST("ORR R0, (R2), ENZ",   0x6708|(9<<4)|1);
        TEST("ORR R0, (R3), OZ",    0x6708|(10<<4)|2);
        TEST("ORR R0, (R4)",        0x6708|(0<<4)|3);
        TEST("ORI R0, 0x1234, LPZ", 0x600F|(0<<8)|(14<<4), 0x1234);
        TEST("ORI R2, 0x5678, LM",  0x600F|(2<<8)|(15<<4), 0x5678);
        TEST("ORI SP, 0x9ABC, SKP", 0x600F|(5<<8)|(1<<4),  0x9ABC);
        TEST("ORI STR, 0xDEF0",     0x600F|(6<<8)|(0<<4),  0xDEF0);

        TEST("EORR R0, (R1), EZ",    0x6700|(8<<4)|0);
        TEST("EORR R0, (R2), ENZ",   0x6700|(9<<4)|1);
        TEST("EORR R0, (R3), OZ",    0x6700|(10<<4)|2);
        TEST("EORR R0, (R4)",        0x6700|(0<<4)|3);
        TEST("EORI R0, 0x1234, LPZ", 0x6007|(0<<8)|(14<<4), 0x1234);
        TEST("EORI R2, 0x5678, LM",  0x6007|(2<<8)|(15<<4), 0x5678);
        TEST("EORI SP, 0x9ABC, SKP", 0x6007|(5<<8)|(1<<4),  0x9ABC);
        TEST("EORI STR, 0xDEF0",     0x6007|(6<<8)|(0<<4),  0xDEF0);
    }
}

static void test_branch() {
    TEST(         "B 0x10",       0xC700|(0<<11)|0x10);
    ATEST(0x1000, "B *-128",      0xC700|(1<<11)|0x80);
    TEST(         "B (0xFF)",     0xC700|(2<<11)|0xFF);
    ATEST(0x1000, "B (*+127)",    0xC700|(3<<11)|0x7F);
    TEST(         "B 128(X0)",    0xC700|(4<<11)|0x80);
    TEST(         "B 255(X1)",    0xC700|(5<<11)|0xFF);
    TEST(         "B (0x10)(X0)", 0xC700|(6<<11)|0x10);
    TEST(         "B (0x80)(X1)", 0xC700|(7<<11)|0x80);
    TEST(         "BAL 0x10",       0x8700|(0<<11)|0x10);
    ATEST(0x1000, "BAL *-128",      0x8700|(1<<11)|0x80);
    TEST(         "BAL (0xFF)",     0x8700|(2<<11)|0xFF);
    ATEST(0x1000, "BAL (*+127)",    0x8700|(3<<11)|0x7F);
    TEST(         "BAL 128(X0)",    0x8700|(4<<11)|0x80);
    TEST(         "BAL 255(X1)",    0x8700|(5<<11)|0xFF);
    TEST(         "BAL (0x10)(X0)", 0x8700|(6<<11)|0x10);
    TEST(         "BAL (0x80)(X1)", 0x8700|(7<<11)|0x80);

    TEST("RET ", 0x2003);
    TEST("LPSW 0", 0x2004|0);
    TEST("LPSW 1", 0x2004|1);
    TEST("LPSW 2", 0x2004|2);
    TEST("LPSW 3", 0x2004|3);
    ERRT("LPSW -1", ILLEGAL_CONSTANT, "-1", 0x2004|7);
    ERRT("LPSW 4",  ILLEGAL_CONSTANT, "4",  0x2004|4);

    if (is1613() || is1613a()) {
        TEST("BD    0x1234",  0x2607, 0x1234);
        TEST("BL   (0x1234)", 0x270F, 0x1234);
        TEST("BALD  0x1234",  0x2617, 0x1234);
        TEST("BALL (0x1234)", 0x271F, 0x1234);

        TEST("BR (R1)", 0x2704|0);
        TEST("BR (R2)", 0x2704|1);
        TEST("BR (R3)", 0x2704|2);
        TEST("BR (R4)", 0x2704|3);
        TEST("BALR (R1)", 0x2714|0);
        TEST("BALR (R2)", 0x2714|1);
        TEST("BALR (R3)", 0x2714|2);
        TEST("BALR (R4)", 0x2714|3);

        TEST("RETL ", 0x3F07);
    }
}

static void test_bitops() {
    TEST("TBIT R0, 1",       0x2800|(0<<8)|(0<<4)|1);
    TEST("TBIT R1, 0, SKP",  0x2800|(1<<8)|(1<<4)|0);
    TEST("TBIT R2, 2, Z",    0x2800|(2<<8)|(4<<4)|2);
    TEST("TBIT R3, 12, NZ",  0x2800|(3<<8)|(5<<4)|12);
    TEST("TBIT R4, 13",      0x2800|(4<<8)|(0<<4)|13);
    TEST("TBIT SP, 14, Z",   0x2800|(5<<8)|(4<<4)|14);
    TEST("TBIT STR, 15, NZ", 0x2800|(6<<8)|(5<<4)|15);
    ERRT("TBIT STR, -1, NZ", ILLEGAL_BIT_NUMBER, "-1, NZ", 0x2800|(6<<8)|(5<<4)|15);
    ERRT("TBIT STR, 16, NZ", ILLEGAL_BIT_NUMBER, "16, NZ", 0x2800|(6<<8)|(5<<4)|0);
    TEST("SBIT R0, 1",       0x3800|(0<<8)|(0<<4)|1);
    TEST("SBIT R1, 0, SKP",  0x3800|(1<<8)|(1<<4)|0);
    TEST("SBIT R2, 2, Z",    0x3800|(2<<8)|(4<<4)|2);
    TEST("SBIT R3, 12, NZ",  0x3800|(3<<8)|(5<<4)|12);
    TEST("SBIT R4, 13",      0x3800|(4<<8)|(0<<4)|13);
    TEST("SBIT SP, 14, Z",   0x3800|(5<<8)|(4<<4)|14);
    TEST("SBIT STR, 15, NZ", 0x3800|(6<<8)|(5<<4)|15);
    TEST("RBIT R0, 1",       0x3000|(0<<8)|(0<<4)|1);
    TEST("RBIT R1, 0, SKP",  0x3000|(1<<8)|(1<<4)|0);
    TEST("RBIT R2, 2, Z",    0x3000|(2<<8)|(4<<4)|2);
    TEST("RBIT R3, 12, NZ",  0x3000|(3<<8)|(5<<4)|12);
    TEST("RBIT R4, 13",      0x3000|(4<<8)|(0<<4)|13);
    TEST("RBIT SP, 14, Z",   0x3000|(5<<8)|(4<<4)|14);
    TEST("RBIT STR, 15, NZ", 0x3000|(6<<8)|(5<<4)|15);

    TEST("SR R0",         0x2008|(0<<8)|(0<<4)|0);
    TEST("SR R1, RE",     0x2008|(1<<8)|(0<<4)|1);
    TEST("SR R2, SE",     0x2008|(2<<8)|(0<<4)|2);
    TEST("SR R3, CE",     0x2008|(3<<8)|(0<<4)|3);
    TEST("SR R4, SKP",    0x2008|(4<<8)|(1<<4)|0);
    TEST("SR SP, RE, NZ", 0x2008|(5<<8)|(5<<4)|1);
    TEST("SR STR, SE, Z", 0x2008|(6<<8)|(4<<4)|2);
    TEST("SL R0",         0x200C|(0<<8)|(0<<4)|0);
    TEST("SL R1, RE",     0x200C|(1<<8)|(0<<4)|1);
    TEST("SL R2, SE",     0x200C|(2<<8)|(0<<4)|2);
    TEST("SL R3, CE",     0x200C|(3<<8)|(0<<4)|3);
    TEST("SL R4, SKP",    0x200C|(4<<8)|(1<<4)|0);
    TEST("SL SP, RE, NZ", 0x200C|(5<<8)|(5<<4)|1);
    TEST("SL STR, SE, Z", 0x200C|(6<<8)|(4<<4)|2);

    if (is1613() || is1613a()) {
        TEST("TSET R0, 0x1234",      0x1708|(0<<4)|0, 0x1234);
        TEST("TSET R1, 0x1234, NZ",  0x1708|(5<<4)|1, 0x1234);
        TEST("TSET R2, 0x1234, Z",   0x1708|(4<<4)|2, 0x1234);
        TEST("TSET R3, 0x1234, SKP", 0x1708|(1<<4)|3, 0x1234);
        TEST("TSET R4, 0x1234",      0x1708|(0<<4)|4, 0x1234);
        TEST("TSET SP, 0x1234, NZ",  0x1708|(5<<4)|5, 0x1234);
        TEST("TSET STR, 0x1234, Z",  0x1708|(4<<4)|6, 0x1234);
        TEST("TRST R0, 0x1234",      0x1700|(0<<4)|0, 0x1234);
        TEST("TRST R1, 0x1234, NZ",  0x1700|(5<<4)|1, 0x1234);
        TEST("TRST R2, 0x1234, Z",   0x1700|(4<<4)|2, 0x1234);
        TEST("TRST R3, 0x1234, SKP", 0x1700|(1<<4)|3, 0x1234);
        TEST("TRST R4, 0x1234",      0x1700|(0<<4)|4, 0x1234);
        TEST("TRST SP, 0x1234, NZ",  0x1700|(5<<4)|5, 0x1234);
        TEST("TRST STR, 0x1234, Z",  0x1700|(4<<4)|6, 0x1234);

        TEST("SRBT R0, R0",  0x3F70|0);
        TEST("SRBT R0, R1",  0x3F70|1);
        TEST("SRBT R0, R2",  0x3F70|2);
        TEST("SRBT R0, R3",  0x3F70|3);
        TEST("SRBT R0, R4",  0x3F70|4);
        TEST("SRBT R0, SP",  0x3F70|5);
        TEST("SRBT R0, STR", 0x3F70|6);
        TEST("DEBP R0, R0",  0x3FF0|0);
        TEST("DEBP R1, R0",  0x3FF0|1);
        TEST("DEBP R2, R0",  0x3FF0|2);
        TEST("DEBP R3, R0",  0x3FF0|3);
        TEST("DEBP R4, R0",  0x3FF0|4);
        TEST("DEBP SP, R0",  0x3FF0|5);
        TEST("DEBP STR, R0", 0x3FF0|6);
    }
}

static void test_misc() {
    TEST("RD R0, 0x34",  0x1800|(0<<8)|0x34);
    TEST("RD R1, 0x34",  0x1800|(1<<8)|0x34);
    TEST("RD R2, 0x34",  0x1800|(2<<8)|0x34);
    TEST("RD R3, 0x34",  0x1800|(3<<8)|0x34);
    TEST("RD R4, 0x34",  0x1800|(4<<8)|0x34);
    TEST("RD SP, 0x34",  0x1800|(5<<8)|0x34);
    TEST("RD STR, 0x34", 0x1800|(6<<8)|0x34);
    TEST("WT R0, 0x34",  0x1000|(0<<8)|0x34);
    TEST("WT R1, 0x34",  0x1000|(1<<8)|0x34);
    TEST("WT R2, 0x34",  0x1000|(2<<8)|0x34);
    TEST("WT R3, 0x34",  0x1000|(3<<8)|0x34);
    TEST("WT R4, 0x34",  0x1000|(4<<8)|0x34);
    TEST("WT SP, 0x34",  0x1000|(5<<8)|0x34);
    TEST("WT STR, 0x34", 0x1000|(6<<8)|0x34);
    ERRT("RD R0, 0x100", OVERFLOW_RANGE, "0x100", 0x1800|(0<<8)|0x00);
    ERRT("WT R0, 0x100", OVERFLOW_RANGE, "0x100", 0x1000|(0<<8)|0x00);

    TEST("H ", 0x2000);

    if (is1613() || is1613a()) {
        TEST("RDR R0, (R1)",  0x2014|(0<<8)|0);
        TEST("RDR R1, (R2)",  0x2014|(1<<8)|1);
        TEST("RDR R2, (R3)",  0x2014|(2<<8)|2);
        TEST("RDR R3, (R4)",  0x2014|(3<<8)|3);
        TEST("RDR R4, (R1)",  0x2014|(4<<8)|0);
        TEST("RDR SP, (R2)",  0x2014|(5<<8)|1);
        ERRT("RDR STR, (R2)", OPERAND_NOT_ALLOWED, "STR, (R2)");
        TEST("WTR R0, (R1)",  0x2010|(0<<8)|0);
        TEST("WTR R1, (R2)",  0x2010|(1<<8)|1);
        TEST("WTR R2, (R3)",  0x2010|(2<<8)|2);
        TEST("WTR R3, (R4)",  0x2010|(3<<8)|3);
        TEST("WTR R4, (R1)",  0x2010|(4<<8)|0);
        TEST("WTR SP, (R2)",  0x2010|(5<<8)|1);
        ERRT("WTR STR, (R2)", OPERAND_NOT_ALLOWED, "STR, (R2)");

        TEST("BLK (R2), (R1), R0", 0x3F17);
        ERRT("BLK (R3), (R1), R0", OPERAND_NOT_ALLOWED, "(R3), (R1), R0");
        ERRT("BLK (R2), (R4), R0", OPERAND_NOT_ALLOWED, "(R2), (R4), R0");
        ERRT("BLK (R2), (R1), R3", OPERAND_NOT_ALLOWED, "(R2), (R1), R3");

        ERRT("LB CSBR, 0x1234", REGISTER_NOT_ALLOWED, "CSBR, 0x1234", 0x0F07|(0<<4), 0x1234);
        TEST("LB SSBR, 0x1234", 0x0F07|(1<<4), 0x1234);
        TEST("LB TSR0, 0x1234", 0x0F07|(2<<4), 0x1234);
        TEST("LB TSR1, 0x1234", 0x0F07|(3<<4), 0x1234);
        TEST("LB OSR0, 0x1234", 0x0F07|(4<<4), 0x1234);
        TEST("LB OSR1, 0x1234", 0x0F07|(5<<4), 0x1234);
        TEST("LB OSR2, 0x1234", 0x0F07|(6<<4), 0x1234);
        TEST("LB OSR3, 0x1234", 0x0F07|(7<<4), 0x1234);
        TEST("STB CSBR, 0x1234", 0x0F87|(0<<4), 0x1234);
        TEST("STB SSBR, 0x1234", 0x0F87|(1<<4), 0x1234);
        TEST("STB TSR0, 0x1234", 0x0F87|(2<<4), 0x1234);
        TEST("STB TSR1, 0x1234", 0x0F87|(3<<4), 0x1234);
        TEST("STB OSR0, 0x1234", 0x0F87|(4<<4), 0x1234);
        TEST("STB OSR1, 0x1234", 0x0F87|(5<<4), 0x1234);
        TEST("STB OSR2, 0x1234", 0x0F87|(6<<4), 0x1234);
        TEST("STB OSR3, 0x1234", 0x0F87|(7<<4), 0x1234);
        TEST("CPYB STR, CSBR", 0x0F80|(0<<4)|6);
        TEST("CPYB SP, SSBR",  0x0F80|(1<<4)|5);
        TEST("CPYB R4, TSR0",  0x0F80|(2<<4)|4);
        TEST("CPYB R3, TSR1",  0x0F80|(3<<4)|3);
        TEST("CPYB R2, OSR0",  0x0F80|(4<<4)|2);
        TEST("CPYB R1, OSR1",  0x0F80|(5<<4)|1);
        TEST("CPYB R0, OSR2",  0x0F80|(6<<4)|0);
        TEST("CPYB R1, OSR3",  0x0F80|(7<<4)|1);
        ERRT("SETB STR, CSBR", REGISTER_NOT_ALLOWED, "CSBR", 0x0F00|(0<<4)|6);
        TEST("SETB SP, SSBR",  0x0F00|(1<<4)|5);
        TEST("SETB R4, TSR0",  0x0F00|(2<<4)|4);
        TEST("SETB R3, TSR1",  0x0F00|(3<<4)|3);
        TEST("SETB R2, OSR0",  0x0F00|(4<<4)|2);
        TEST("SETB R1, OSR1",  0x0F00|(5<<4)|1);
        TEST("SETB R0, OSR2",  0x0F00|(6<<4)|0);
        TEST("SETB R1, OSR3",  0x0F00|(7<<4)|1);

        TEST("LS SBRB, 0x1234", 0x0F0F|(0<<4), 0x1234);
        TEST("LS ICB, 0x1234",  0x0F0F|(1<<4), 0x1234);
        TEST("LS NPP, 0x1234",  0x0F0F|(2<<4), 0x1234);
        ERRT("LS R0, 0x1234",   OPERAND_NOT_ALLOWED, "R0, 0x1234");
        ERRT("LS CSBR, 0x1234", OPERAND_NOT_ALLOWED, "CSBR, 0x1234");
        ERRT("LS TCR, 0x1234",  OPERAND_NOT_ALLOWED, "TCR, 0x1234");
        ERRT("LS SP, 0x1234",   OPERAND_NOT_ALLOWED, "SP, 0x1234");
        ERRT("LS STR, 0x1234",  OPERAND_NOT_ALLOWED, "STR, 0x1234");
        TEST("STS SBRB, 0x1234", 0x0F8F|(0<<4), 0x1234);
        TEST("STS ICB, 0x1234",  0x0F8F|(1<<4), 0x1234);
        TEST("STS NPP, 0x1234",  0x0F8F|(2<<4), 0x1234);
        ERRT("STS R3, 0x1234",   OPERAND_NOT_ALLOWED, "R3, 0x1234");
        ERRT("STS SIR, 0x1234",  OPERAND_NOT_ALLOWED, "SIR, 0x1234");
        ERRT("STS X0, 0x1234",   OPERAND_NOT_ALLOWED, "X0, 0x1234");
        ERRT("STS SSBR, 0x1234", OPERAND_NOT_ALLOWED, "SSBR, 0x1234");
        TEST("CPYS R2, SBRB", 0x0F88|(0<<4)|2);
        TEST("CPYS R1, ICB",  0x0F88|(1<<4)|1);
        TEST("CPYS R0, NPP",  0x0F88|(2<<4)|0);
        ERRT("CPYS R3, R4",   OPERAND_NOT_ALLOWED, "R3, R4");
        ERRT("CPYS R4, TSR1", OPERAND_NOT_ALLOWED, "R4, TSR1");
        ERRT("CPYS SP, SIO",  OPERAND_NOT_ALLOWED, "SP, SIO");
        ERRT("CPYS R1, DR0",  OPERAND_NOT_ALLOWED, "R1, DR0");
        TEST("SETS R2, SBRB", 0x0F08|(0<<4)|2);
        TEST("SETS R1, ICB",  0x0F08|(1<<4)|1);
        TEST("SETS R0, NPP",  0x0F08|(2<<4)|0);
        ERRT("SETS R3, R2",   OPERAND_NOT_ALLOWED, "R3, R2");
        ERRT("SETS R4, SOR",  OPERAND_NOT_ALLOWED, "R4, SOR");
        ERRT("SETS SP, CSBR", OPERAND_NOT_ALLOWED, "SP, CSBR");

        TEST("CPYH STR, TCR", 0x3F80|(0<<4)|6);
        TEST("CPYH SP, TIR",  0x3F80|(1<<4)|5);
        TEST("CPYH R4, TSR",  0x3F80|(2<<4)|4);
        TEST("CPYH R3, SCR",  0x3F80|(3<<4)|3);
        TEST("CPYH R2, SSR",  0x3F80|(4<<4)|2);
        TEST("CPYH R1, SIR",  0x3F80|(5<<4)|1);
        ERRT("CPYH R1, SOR",  REGISTER_NOT_ALLOWED, "SOR", 0x3F80|(5<<4)|1);
        TEST("CPYH R0, IISR", 0x3F80|(6<<4)|0);
        TEST("SETH STR, TCR", 0x3F00|(0<<4)|6);
        TEST("SETH SP, TIR",  0x3F00|(1<<4)|5);
        TEST("SETH R4, TSR",  0x3F00|(2<<4)|4);
        TEST("SETH R3, SCR",  0x3F00|(3<<4)|3);
        TEST("SETH R2, SSR",  0x3F00|(4<<4)|2);
        TEST("SETH R1, SOR",  0x3F00|(5<<4)|1);
        ERRT("SETH R1, SIR",  REGISTER_NOT_ALLOWED, "SIR", 0x3F00|(5<<4)|1);
        TEST("SETH R0, IISR", 0x3F00|(6<<4)|0);
    }
}

static void test_comment() {
    ACOMM(0x1000, "L R2, * - 128    ; comment", "; comment", 0xC000|(1<<11)|(2<<8)|0x80);
    ACOMM(0x1000, "L R2, -128 ( IC ); comment", "; comment", 0xC000|(1<<11)|(2<<8)|0x80);
    ACOMM(0x1000, "L R2, * - 128               comment", "comment", 0xC000|(1<<11)|(2<<8)|0x80);
    ACOMM(0x1000, "L R2, -128 ( IC )           comment", "comment", 0xC000|(1<<11)|(2<<8)|0x80);
    ACOMM(0x1000, "L R2, 0x0F80-* ( IC )       comment", "comment", 0xC000|(1<<11)|(2<<8)|0x80);
    ACOMM(0x1000, "L R4, ( * + 127 )           comment", "comment", 0xC000|(3<<11)|(4<<8)|0x7F);
    ACOMM(0x1000, "L R4, ( 127 ( IC ) )        comment", "comment", 0xC000|(3<<11)|(4<<8)|0x7F);
    ACOMM(0x1000, "L R4, ( 0x107F - * ( IC ) ) comment", "comment", 0xC000|(3<<11)|(4<<8)|0x7F);
    COMM(         "L R0, 16 ( X0 )             comment", "comment", 0xC000|(4<<11)|(0<<8)|0x10);
    COMM(         "L R0, 0 + (16) ( X0 )       comment", "comment", 0xC000|(4<<11)|(0<<8)|0x10);
    COMM(         "L R1, ( 16 ) ( X0 )         comment", "comment", 0xC000|(6<<11)|(1<<8)|0x10);
    COMM(         "L R1, ( 0 + 16 ) ( X0 )     comment", "comment", 0xC000|(6<<11)|(1<<8)|0x10);

    symtab.intern(0x0F80, "sym0F80");
    symtab.intern(0x107F, "sym107F");
    ACOMM(0x1000, "L R2, sym0F80          comment", "comment", 0xC000|(1<<11)|(2<<8)|0x80);
    ACOMM(0x1000, "L R2, sym0F80-* (IC)   comment", "comment", 0xC000|(1<<11)|(2<<8)|0x80);
    ACOMM(0x1000, "L R4, (sym107F)        comment", "comment", 0xC000|(3<<11)|(4<<8)|0x7F);
    ACOMM(0x1000, "L R4, (sym107F-* (IC)) comment", "comment", 0xC000|(3<<11)|(4<<8)|0x7F);

    COMM("DC -128,255 comment", "comment", 0xFF80, 0x00FF);
    COMM("DC 'TEXT'   comment", "comment", 0x5445, 0x5854);
}

static void test_undef() {
    symtab.intern(0x1020, "SYM1020");
    ATEST(0x1000, "L R2, SYM1020",                       0xC000|(1<<11)|(2<<8)|0x20);
    AERUS(0x1000, "L R2, UNDEF",         "UNDEF",        0xC000|(0<<11)|(2<<8)|0x00);
    AERUS(0x1000, "L R2, UNDEF-*(IC)",   "UNDEF-*(IC)",  0xC000|(1<<11)|(2<<8)|0x00);
    ATEST(0x1000, "L R4, (SYM1020)",                     0xC000|(3<<11)|(4<<8)|0x20);
    AERUS(0x1000, "L R4, (UNDEF)",       "UNDEF)",       0xC000|(2<<11)|(4<<8)|0x00);
    AERUS(0x1000, "L R4, (UNDEF-*(IC))", "UNDEF-*(IC))", 0xC000|(3<<11)|(4<<8)|0x00);

    if (is1613() || is1613a()) {
        ERUS("BD    UNDEF",  "UNDEF",  0x2607, 0x0000);
        ERUS("BL   (UNDEF)", "UNDEF)", 0x270F, 0x0000);
        ERUS("BALD  UNDEF",  "UNDEF",  0x2617, 0x0000);
        ERUS("BALL (UNDEF)", "UNDEF)", 0x271F, 0x0000);
        ERUS("TSET R0, UNDEF",    "UNDEF",    0x1708|(0<<4)|0, 0x0000);
        ERUS("TRST R2, UNDEF, Z", "UNDEF, Z", 0x1700|(4<<4)|2, 0x0000);
    }
}

static void test_data_constant() {
    TEST("DC -128,255",    0xFF80, 0x00FF);
    TEST("DC -129,256",    0xFF7F, 0x0100);
    TEST("DC C'TEXTa'",    0x5445, 0x5854, 0x6100);
    TEST("DC C'X'",        0x5800);
    TEST("DC C'X'+0",      0x0058);
    TEST("DC 'X'",         0x5800);
    TEST("DC 'X'+0",       0x0058);
    TEST("DC C'A''B'",     0x4127, 0x4200);
    TEST("DC 'TEXT'",      0x5445, 0x5854);
    ERRT("DC C'A",         MISSING_CLOSING_QUOTE, "C'A");
    ERRT("DC C'TEXT",      MISSING_CLOSING_QUOTE, "C'TEXT");
    ERRT("DC 'TEXT",       MISSING_CLOSING_QUOTE, "'TEXT");
    ERUS("DC 1, UNDEF, 2", "UNDEF, 2", 0x0001, 0x0000, 0x0002);
#if defined(LIBASM_ASM_NOFLOAT)
    ERRT("DC -1.0, 42.0, 7.8125e-03", FLOAT_NOT_SUPPORTED, "-1.0, 42.0, 7.8125e-03",
         0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000);
    ERRT("DC 1.0, UNDEF + 0.0, 2.0", FLOAT_NOT_SUPPORTED, "1.0, UNDEF + 0.0, 2.0",
         0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000);
#else
    TEST("DC -1.0, 42.0, 7.8125e-03",
         0xC110, 0x0000, 0x422A, 0x0000, 0x3F20, 0x0000);
    ERUS("DC 1.0, UNDEF + 0.0, 2.0", "UNDEF + 0.0, 2.0",
         0x4110, 0x0000, 0x0000, 0x0000, 0x4120, 0x0000);
#endif

    ERRT("DC C'"
         "1234567890" "1234567890" "1234567890" "1234567890" "1234567890" "1234567890"
         "1234567890'",
         NO_MEMORY,
         "567890'",
         0x3132, 0x3334, 0x3536, 0x3738, 0x3930,
         0x3132, 0x3334, 0x3536, 0x3738, 0x3930,
         0x3132, 0x3334, 0x3536, 0x3738, 0x3930,
         0x3132, 0x3334, 0x3536, 0x3738, 0x3930,
         0x3132, 0x3334, 0x3536, 0x3738, 0x3930,
         0x3132, 0x3334, 0x3536, 0x3738, 0x3930,
         0x3132, 0x3334);

    ERRT("DC "
         "X'1234', X'5678', X'9ABC', X'DEF0', X'1234', X'5678', X'9ABC', X'DEF0', "
         "X'1234', X'5678', X'9ABC', X'DEF0', X'1234', X'5678', X'9ABC', X'DEF0', "
         "X'1234', X'5678', X'9ABC', X'DEF0', X'1234', X'5678', X'9ABC', X'DEF0', "
         "X'1234', X'5678', X'9ABC', X'DEF0', X'1234', X'5678', X'9ABC', X'DE', "
         "X'AAAA', X'BBBB'",
         NO_MEMORY,
         "X'AAAA', X'BBBB'",
         0x1234, 0x5678, 0x9ABC, 0xDEF0, 0x1234, 0x5678, 0x9ABC, 0xDEF0,
         0x1234, 0x5678, 0x9ABC, 0xDEF0, 0x1234, 0x5678, 0x9ABC, 0xDEF0,
         0x1234, 0x5678, 0x9ABC, 0xDEF0, 0x1234, 0x5678, 0x9ABC, 0xDEF0,
         0x1234, 0x5678, 0x9ABC, 0xDEF0, 0x1234, 0x5678, 0x9ABC, 0x00DE);
}
// clang-format on

void run_tests(const char *cpu) {
    assembler.setCpu(cpu);
    RUN_TEST(test_transfer);
    RUN_TEST(test_integer);
    RUN_TEST(test_float);
    RUN_TEST(test_logical);
    RUN_TEST(test_branch);
    RUN_TEST(test_bitops);
    RUN_TEST(test_misc);
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

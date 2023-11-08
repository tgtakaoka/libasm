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

#include "dis_z8000.h"
#include "test_dis_helper.h"

using namespace libasm;
using namespace libasm::z8000;
using namespace libasm::test;

DisZ8000 dis8000;
Disassembler &disassembler(dis8000);

static bool z8001() {
    return strcmp_P("Z8001", disassembler.config().cpu_P()) == 0;
}

static void set_up() {
    disassembler.reset();
    disassembler.setOption("relative", "false");
    disassembler.setOption("segmented-addr", "false");
}

static void tear_down() {
    symtab.reset();
}

// clang-format off
void test_cpu() {
    EQUALS("cpu 8001", true,    disassembler.setCpu("Z8001"));
    EQUALS_P("cpu 8001", "Z8001", disassembler.config().cpu_P());

    EQUALS("cpu Z8002", true,    disassembler.setCpu("Z8002"));
    EQUALS_P("cpu Z8002", "Z8002", disassembler.config().cpu_P());
}

static void test_load_and_exchange() {
    // Clear
    TEST("CLR",  "R2",  0x8D28);
    TEST("CLRB", "RH2", 0x8C28);
    if (z8001()) {
        TEST("CLR",  "@RR2",          0x0D28);
        TEST("CLR",  "|%120034|",     0x4D08, 0x1234);
        TEST("CLR",  "%561234",       0x4D08, 0xD600, 0x1234);
        TEST("CLR",  "|%120034|(R2)", 0x4D28, 0x1234);
        TEST("CLR",  "%561234(R2)",   0x4D28, 0xD600, 0x1234);
        TEST("CLRB", "@RR2",          0x0C28);
        TEST("CLRB", "|%120034|",                      0x4C08, 0x1234);
        TEST("CLRB", "%561234",                        0x4C08, 0xD600, 0x1234);
        NMEM("CLRB", "%560000",        "%560000",      0x4C08, 0xD600);
        NMEM("CLRB", "|%000000|",     "|%000000|",     0x4C08);
        TEST("CLRB", "|%120034|(R2)",                  0x4C28, 0x1234);
        TEST("CLRB", "%561234(R2)",                    0x4C28, 0xD600, 0x1234);
        NMEM("CLRB", "%560000(R2)",    "%560000(R2)",  0x4C28, 0xD600);
        NMEM("CLRB", "|%000000|(R2)", "|%000000|(R2)", 0x4C28);
    } else {
        TEST("CLR",  "@R2",       0x0D28);
        TEST("CLR",  "%1234",     0x4D08, 0x1234);
        TEST("CLR",  "%1234(R2)", 0x4D28, 0x1234);
        TEST("CLRB", "@R2",       0x0C28);
        TEST("CLRB", "%1234",                  0x4C08, 0x1234);
        NMEM("CLRB", "%0000",     "%0000",     0x4C08);
        TEST("CLRB", "%1234(R2)",              0x4C28, 0x1234);
        NMEM("CLRB", "%0000(R2)", "%0000(R2)", 0x4C28);
    }

    // Exchange
    TEST("EX",  "R1, R2",   0xAD21);
    TEST("EXB", "RL1, RH2", 0xAC29);
    if (z8001()) {
        TEST("EX",  "R9, @RR2",           0x2D29);
        TEST("EX",  "R9, |%120034|",      0x6D09, 0x1234);
        TEST("EX",  "R9, %561234",        0x6D09, 0xD600, 0x1234);
        TEST("EX",  "R9, |%120034|(R2)",  0x6D29, 0x1234);
        TEST("EX",  "R9, %561234(R2)",    0x6D29, 0xD600, 0x1234);
        TEST("EXB", "RL1, @RR2",          0x2C29);
        TEST("EXB", "RL1, |%120034|",     0x6C09, 0x1234);
        TEST("EXB", "RL1, %561234",       0x6C09, 0xD600, 0x1234);
        TEST("EXB", "RL1, |%120034|(R2)", 0x6C29, 0x1234);
        TEST("EXB", "RL1, %561234(R2)",   0x6C29, 0xD600, 0x1234);
    } else {
        TEST("EX",  "R9, @R2",        0x2D29);
        TEST("EX",  "R9, %1234",      0x6D09, 0x1234);
        TEST("EX",  "R9, %1234(R2)",  0x6D29, 0x1234);
        TEST("EXB", "RL1, @R2",       0x2C29);
        TEST("EXB", "RL1, %1234",     0x6C09, 0x1234);
        TEST("EXB", "RL1, %1234(R2)", 0x6C29, 0x1234);
    }

    // Load
    TEST("LD",  "R2, R8",   0xA182);
    TEST("LDB", "RH2, RL0", 0xA082);
    TEST("LDL", "RR2, RR8", 0x9482);
    TEST("LD",  "R2, #%5678", 0x2102, 0x5678);
    ERRT("LDB", "RH2, #%56",  SUBOPTIMAL_INSTRUCTION, "#%56", 0x2002, 0x5656);
    TEST("LDB", "RH2, #%56",  0xC256);
    TEST("LDL", "RR2, #%12345678", 0x1402, 0x1234, 0x5678);
    if (z8001()) {
        TEST("LD",  "R9, @RR2",          0x2129);
        TEST("LD",  "R9, |%120034|",     0x6109, 0x1234);
        TEST("LD",  "R9, %561234",       0x6109, 0xD600, 0x1234);
        TEST("LD",  "R9, |%120034|(R2)", 0x6129, 0x1234);
        TEST("LD",  "R9, %561234(R2)",   0x6129, 0xD600, 0x1234);
        TEST("LD",  "R9, RR2(#%1234)",   0x3129, 0x1234);
        TEST("LD",  "R9, RR2(R4)",       0x7129, 0x0400);
        TEST("LD",  "R9, RR2(R0)",       0x7129, 0x0000);
        ERRT("LD",  "R9, RR0(R4)", REGISTER_NOT_ALLOWED, "RR0(R4)", 0x7109, 0x0400);
        UNKN(                            0x7129, 0x1400);
        UNKN(                            0x7129, 0x0420);
        UNKN(                            0x7129, 0x0403);
        TEST("LDB", "RL1, @RR2",          0x2029);
        TEST("LDB", "RL1, |%120034|",     0x6009, 0x1234);
        TEST("LDB", "RL1, %561234",       0x6009, 0xD600, 0x1234);
        TEST("LDB", "RL1, |%120034|(R2)", 0x6029, 0x1234);
        TEST("LDB", "RL1, %561234(R2)",   0x6029, 0xD600, 0x1234);
        TEST("LDB", "RL1, RR2(#%1234)",   0x3029, 0x1234);
        TEST("LDB", "RL1, RR2(R4)",       0x7029, 0x0400);
        TEST("LDB", "RL1, RR2(R0)",       0x7029, 0x0000);
        ERRT("LDB", "RL1, RR0(R4)", REGISTER_NOT_ALLOWED, "RR0(R4)", 0x7009, 0x0400);
        UNKN(                             0x7029, 0x4400);
        UNKN(                             0x7029, 0x0450);
        UNKN(                             0x7029, 0x0406);
        TEST("LDL", "RR2, @RR8",          0x1482);
        TEST("LDL", "RR2, |%120034|",     0x5402, 0x1234);
        TEST("LDL", "RR2, %561234",       0x5402, 0xD600, 0x1234);
        TEST("LDL", "RR2, |%120034|(R8)", 0x5482, 0x1234);
        TEST("LDL", "RR2, %561234(R8)",   0x5482, 0xD600, 0x1234);
        TEST("LDL", "RR2, RR8(#%1234)",   0x3582, 0x1234);
        TEST("LDL", "RR2, RR8(R4)",       0x7582, 0x0400);
        TEST("LDL", "RR2, RR8(R0)",       0x7582, 0x0000);
        ERRT("LDL", "RR2, RR0(R4)", REGISTER_NOT_ALLOWED, "RR0(R4)", 0x7502, 0x0400);
        UNKN(                             0x7582, 0x1400);
        UNKN(                             0x7582, 0x0420);
        UNKN(                             0x7582, 0x0403);
    } else {
        TEST("LD",  "R9, @R2",        0x2129);
        TEST("LD",  "R9, %1234",      0x6109, 0x1234);
        TEST("LD",  "R9, %1234(R2)",  0x6129, 0x1234);
        TEST("LD",  "R9, R2(#%1234)", 0x3129, 0x1234);
        TEST("LD",  "R9, R2(R4)",     0x7129, 0x0400);
        TEST("LD",  "R9, R2(R0)",     0x7129, 0x0000);
        ERRT("LD",  "R9, R0(R4)", REGISTER_NOT_ALLOWED, "R0(R4)", 0x7109, 0x0400);
        UNKN(                         0x7129, 0x7400);
        UNKN(                         0x7129, 0x0480);
        UNKN(                         0x7129, 0x0409);
        TEST("LDB", "RL1, @R2",        0x2029);
        TEST("LDB", "RL1, %1234",      0x6009, 0x1234);
        TEST("LDB", "RL1, %1234(R2)",  0x6029, 0x1234);
        TEST("LDB", "RL1, R2(#%1234)", 0x3029, 0x1234);
        TEST("LDB", "RL1, R2(R4)",     0x7029, 0x0400);
        TEST("LDB", "RL1, R2(R0)",     0x7029, 0x0000);
        ERRT("LDB", "RL1, R0(R4)", REGISTER_NOT_ALLOWED, "R0(R4)", 0x7009, 0x0400);
        UNKN(                          0x7029, 0xA400);
        UNKN(                          0x7029, 0x04B0);
        UNKN(                          0x7029, 0x040C);
        TEST("LDL", "RR2, @R8",        0x1482);
        TEST("LDL", "RR2, %1234",      0x5402, 0x1234);
        TEST("LDL", "RR2, %1234(R8)",  0x5482, 0x1234);
        TEST("LDL", "RR2, R8(#%1234)", 0x3582, 0x1234);
        TEST("LDL", "RR2, R8(R4)",     0x7582, 0x0400);
        TEST("LDL", "RR2, R8(R0)",     0x7582, 0x0000);
        ERRT("LDL", "RR2, R0(R4)", REGISTER_NOT_ALLOWED, "R0(R4)", 0x7502, 0x0400);
        UNKN(                          0x7582, 0x4400);
        UNKN(                          0x7582, 0x0450);
        UNKN(                          0x7582, 0x0406);
    }
    if (z8001()) {
        TEST("LD",  "@RR2, R9",          0x2F29);
        TEST("LD",  "|%120034|, R9",     0x6F09, 0x1234);
        TEST("LD",  "%561234, R9",       0x6F09, 0xD600, 0x1234);
        TEST("LD",  "|%120034|(R2), R9", 0x6F29, 0x1234);
        TEST("LD",  "%561234(R2), R9",   0x6F29, 0xD600, 0x1234);
        TEST("LD",  "RR2(#%1234), R9",   0x3329, 0x1234);
        TEST("LD",  "RR2(R4), R9",       0x7329, 0x0400);
        TEST("LD",  "RR2(R0), R9",       0x7329, 0x0000);
        ERRT("LD",  "RR0(R4), R9", REGISTER_NOT_ALLOWED, "RR0(R4), R9", 0x7309, 0x0400);
        UNKN(                            0x7329, 0xD400);
        UNKN(                             0x7329, 0x04E0);
        UNKN(                             0x7329, 0x040F);
        TEST("LDB", "@RR2, RL1",          0x2E29);
        TEST("LDB", "|%120034|, RL1",     0x6E09, 0x1234);
        TEST("LDB", "%561234, RL1",       0x6E09, 0xD600, 0x1234);
        TEST("LDB", "|%120034|(R2), RL1", 0x6E29, 0x1234);
        TEST("LDB", "%561234(R2), RL1",   0x6E29, 0xD600, 0x1234);
        TEST("LDB", "RR2(#%1234), RL1",   0x3229, 0x1234);
        TEST("LDB", "RR2(R4), RL1",       0x7229, 0x0400);
        TEST("LDB", "RR2(R0), RL1",       0x7229, 0x0000);
        ERRT("LDB", "RR0(R4), RL1", REGISTER_NOT_ALLOWED, "RR0(R4), RL1", 0x7209, 0x0400);
        UNKN(                             0x7229, 0x1400);
        UNKN(                             0x7229, 0x0420);
        UNKN(                             0x7229, 0x0403);
        TEST("LDL", "@RR8, RR2",          0x1D82);
        TEST("LDL", "|%120034|, RR2",     0x5D02, 0x1234);
        TEST("LDL", "%561234, RR2",       0x5D02, 0xD600, 0x1234);
        TEST("LDL", "|%120034|(R8), RR2", 0x5D82, 0x1234);
        TEST("LDL", "%561234(R8), RR2",   0x5D82, 0xD600, 0x1234);
        TEST("LDL", "RR8(#%1234), RR2",   0x3782, 0x1234);
        TEST("LDL", "RR8(R4), RR2",       0x7782, 0x0400);
        TEST("LDL", "RR8(R0), RR2",       0x7782, 0x0000);
        ERRT("LDL", "RR0(R4), RR2", REGISTER_NOT_ALLOWED, "RR0(R4), RR2", 0x7702, 0x0400);
        UNKN(                             0x7782, 0x7400);
        UNKN(                             0x7782, 0x0480);
        UNKN(                             0x7782, 0x0409);
    } else {
        TEST("LD",  "@R2, R9",        0x2F29);
        TEST("LD",  "%1234, R9",      0x6F09, 0x1234);
        TEST("LD",  "%1234(R2), R9",  0x6F29, 0x1234);
        TEST("LD",  "R2(#%1234), R9", 0x3329, 0x1234);
        TEST("LD",  "R2(R4), R9",     0x7329, 0x0400);
        TEST("LD",  "R2(R0), R9",     0x7329, 0x0000);
        ERRT("LD",  "R0(R4), R9", REGISTER_NOT_ALLOWED, "R0(R4), R9", 0x7309, 0x0400);
        UNKN(                         0x7329, 0x5400);
        UNKN(                         0x7329, 0x0460);
        UNKN(                         0x7329, 0x0407);
        TEST("LDB", "@R2, RL1",        0x2E29);
        TEST("LDB", "%1234, RL1",      0x6E09, 0x1234);
        TEST("LDB", "%1234(R2), RL1",  0x6E29, 0x1234);
        TEST("LDB", "R2(#%1234), RL1", 0x3229, 0x1234);
        TEST("LDB", "R2(R4), RL1",     0x7229, 0x0400);
        TEST("LDB", "R2(R0), RL1",     0x7229, 0x0000);
        ERRT("LDB", "R0(R4), RL1", REGISTER_NOT_ALLOWED, "R0(R4), RL1", 0x7209, 0x0400);
        UNKN(                          0x7229, 0x8400);
        UNKN(                          0x7229, 0x0490);
        UNKN(                          0x7229, 0x040A);
        TEST("LDL", "@R8, RR2",        0x1D82);
        TEST("LDL", "%1234, RR2",      0x5D02, 0x1234);
        TEST("LDL", "%1234(R8), RR2",  0x5D82, 0x1234);
        TEST("LDL", "R8(#%1234), RR2", 0x3782, 0x1234);
        TEST("LDL", "R8(R4), RR2",     0x7782, 0x0400);
        TEST("LDL", "R8(R0), RR2",     0x7782, 0x0000);
        ERRT("LDL", "R0(R4), RR2", REGISTER_NOT_ALLOWED, "R0(R4), RR2", 0x7702, 0x0400);
        UNKN(                          0x7782, 0xA400);
        UNKN(                          0x7782, 0x04B0);
        UNKN(                          0x7782, 0x040C);
    }
    if (z8001()) {
        TEST("LD",  "R2, #%5678",            0x2102, 0x5678);
        TEST("LD",  "@RR2, #%5678",          0x0D25, 0x5678);
        TEST("LD",  "|%120034|, #%5678",     0x4D05, 0x1234, 0x5678);
        TEST("LD",  "%561234, #%5678",       0x4D05, 0xD600, 0x1234, 0x5678);
        TEST("LD",  "|%120034|(R2), #%5678", 0x4D25, 0x1234, 0x5678);
        TEST("LD",  "%561234(R2), #%5678",   0x4D25, 0xD600, 0x1234, 0x5678);
        ERRT("LDB", "RH2, #%56", SUBOPTIMAL_INSTRUCTION, "#%56", 0x2002, 0x5656);
        TEST("LDB", "RH2, #%56", 0xC256);
        TEST("LDB", "@RR2, #%56",            0x0C25, 0x5656);
        TEST("LDB", "|%120034|, #%56",       0x4C05, 0x1234, 0x5656);
        TEST("LDB", "%561234, #%56",         0x4C05, 0xD600, 0x1234, 0x5656);
        TEST("LDB", "|%120034|(R2), #%56",   0x4C25, 0x1234, 0x5656);
        TEST("LDB", "%561234(R2), #%56",     0x4C25, 0xD600, 0x1234, 0x5656);
    } else {
        TEST("LD",  "R2, #%5678",        0x2102, 0x5678);
        TEST("LD",  "@R2, #%5678",       0x0D25, 0x5678);
        TEST("LD",  "%1234, #%5678",     0x4D05, 0x1234, 0x5678);
        TEST("LD",  "%1234(R2), #%5678", 0x4D25, 0x1234, 0x5678);
        ERRT("LDB", "RH2, #%56", SUBOPTIMAL_INSTRUCTION, "#%56", 0x2002, 0x5656);
        TEST("LDB", "RH2, #%56", 0xC256);
        TEST("LDB", "@R2, #%56",         0x0C25, 0x5656);
        TEST("LDB", "%1234, #%56",       0x4C05, 0x1234, 0x5656);
        TEST("LDB", "%1234(R2), #%56",   0x4C25, 0x1234, 0x5656);
    }

    // Load Address
    if (z8001()) {
        TEST("LDA", "RR8, |%120034|",     0x7608, 0x1234);
        TEST("LDA", "RR8, %561234",       0x7608, 0xD600, 0x1234);
        TEST("LDA", "RR8, |%120034|(R2)", 0x7628, 0x1234);
        TEST("LDA", "RR8, %561234(R2)",   0x7628, 0xD600, 0x1234);
        TEST("LDA", "RR8, RR2(#%1234)",   0x3428, 0x1234);
        TEST("LDA", "RR8, RR2(R4)",       0x7428, 0x0400);
        UNKN(                             0x7428, 0x1400);
        UNKN(                             0x7428, 0x0420);
        UNKN(                             0x7428, 0x0403);
    } else {
        TEST("LDA", "R8, %1234(R2)",  0x7628, 0x1234);
        TEST("LDA", "R8, R2(#%1234)", 0x3428, 0x1234);
        TEST("LDA", "R8, R2(R4)",     0x7428, 0x0400);
        UNKN(                         0x7428, 0x4400);
        UNKN(                         0x7428, 0x0450);
        UNKN(                         0x7428, 0x0406);
    }

    // Load Address Relative
    if (z8001()) {
        ATEST(0x2000, "LDAR", "RR8, %002000", 0x3408, 0xFFFC);
        ATEST(0x2000, "LDAR", "RR8, %002004", 0x3408, 0x0000);
        ATEST(0xF000, "LDAR", "RR8, %007004", 0x3408, 0x8000);
        ATEST(0x2000, "LDAR", "RR8, %00A002", 0x3408, 0x7FFE);
        ATEST(0x2000, "LDAR", "RR8, %00A003", 0x3408, 0x7FFF);
        ANMEM(0x2000, "LDAR", "RR8, %002002", "%002002", 0x3408);
    } else {
        ATEST(0x2000, "LDAR", "R8, %2000", 0x3408, 0xFFFC);
        ATEST(0x2000, "LDAR", "R8, %2004", 0x3408, 0x0000);
        ATEST(0xF000, "LDAR", "R8, %7004", 0x3408, 0x8000);
        ATEST(0x2000, "LDAR", "R8, %A002", 0x3408, 0x7FFE);
        ATEST(0x2000, "LDAR", "R8, %A003", 0x3408, 0x7FFF);
        ANMEM(0x2000, "LDAR", "R8, %2002", "%2002", 0x3408);
    }

    // Load Constant
    TEST("LDK", "R1, #0",  0xBD10);
    TEST("LDK", "R1, #15", 0xBD1F);

    // Load Multiple
    if (z8001()) {
        TEST("LDM", "R15, @RR2, #1",         0x1C21, 0x0F00);
        TEST("LDM", "R1, @RR2, #2",          0x1C21, 0x0101);
        NMEM("LDM", "",                  "", 0x1C21);
        UNKN(                                0x1C21, 0x1101);
        UNKN(                                0x1C21, 0x0111);
        TEST("LDM", "R13, |%120034|, #3",    0x5C01, 0x0D02, 0x1234);
        TEST("LDM", "R12, %561234, #4",      0x5C01, 0x0C03, 0xD600, 0x1234);
        UNKN(                                0x5C01, 0x8C03);
        UNKN(                                0x5C01, 0x0C93);
        TEST("LDM", "R1, |%120034|(R2), #5", 0x5C21, 0x0104, 0x1234);
        TEST("LDM", "R0, %561234(R2), #16",  0x5C21, 0x000F, 0xD600, 0x1234);
        ERRT("LDM", "R15, @RR2, #2", OVERFLOW_RANGE, "2", 0x1C21, 0x0F01);
        TEST("LDM", "@RR2, R15, #1",         0x1C29, 0x0F00);
        UNKN(                                0x1C29, 0x1F00);
        UNKN(                                0x1C29, 0x0F20);
        TEST("LDM", "@RR2, R1, #2",          0x1C29, 0x0101);
        TEST("LDM", "|%120034|, R13, #3",    0x5C09, 0x0D02, 0x1234);
        TEST("LDM", "%561234, R12, #4",      0x5C09, 0x0C03, 0xD600, 0x1234);
        TEST("LDM", "|%120034|(R2), R1, #5", 0x5C29, 0x0104, 0x1234);
        UNKN(                                0x5C29, 0x100F);
        UNKN(                                0x5C29, 0x002F);
        TEST("LDM", "%561234(R2), R0, #16",  0x5C29, 0x000F, 0xD600, 0x1234);
        ERRT("LDM", "@RR2, R15, #2", OVERFLOW_RANGE, "2", 0x1C29, 0x0F01);
    } else {
        TEST("LDM", "R9, @R2, #7",        0x1C21, 0x0906);
        NMEM("LDM", "",               "", 0x1C21);
        TEST("LDM", "R1, @R2, #8",        0x1C21, 0x0107);
        ERRT("LDM", "R9, @R2, #8", OVERFLOW_RANGE, "8", 0x1C21, 0x0907);
        UNKN(                             0x1C21, 0x4107);
        UNKN(                             0x1C21, 0x0157);
        TEST("LDM", "R7, %1234, #9",      0x5C01, 0x0708, 0x1234);
        TEST("LDM", "R0, %1234(R2), #16", 0x5C21, 0x000F, 0x1234);
        UNKN(                             0x5C21, 0xF00F);
        UNKN(                             0x5C21, 0x00FF);
        TEST("LDM", "@R2, R9, #7",        0x1C29, 0x0906);
        ERRT("LDM", "@R2, R9, #8", OVERFLOW_RANGE, "8", 0x1C29, 0x0907);
        TEST("LDM", "@R2, R1, #8",        0x1C29, 0x0107);
        UNKN(                             0x1C29, 0xA107);
        UNKN(                             0x1C29, 0x01B7);
        TEST("LDM", "%1234, R7, #9",      0x5C09, 0x0708, 0x1234);
        TEST("LDM", "%1234(R2), R0, #16", 0x5C29, 0x000F, 0x1234);
        UNKN(                             0x5C29, 0x800F);
        UNKN(                             0x5C29, 0x008F);
    }

    // Load Relative
    if (z8001()) {
        ATEST(0x2000, "LDR",  "R1, %002000",  0x3101, 0xFFFC);
        ATEST(0x2000, "LDR",  "%002000, R1",  0x3301, 0xFFFC);
        ATEST(0x2000, "LDRB", "RH1, %002000", 0x3001, 0xFFFC);
        ATEST(0x2000, "LDRB", "%002000, RH1", 0x3201, 0xFFFC);
        ATEST(0x2000, "LDRL", "RR2, %002000", 0x3502, 0xFFFC);
        ATEST(0x2000, "LDRL", "%002000, RR2", 0x3702, 0xFFFC);
        ANMEM(0x2000, "LDRL", "%002002, RR2", "%002002, RR2", 0x3702);
        AERRT(0x2000, "LDRL", "%002000, ", ILLEGAL_REGISTER, "", 0x3703, 0xFFFC);
    } else {
        ATEST(0x2000, "LDR",  "R1, %2000",  0x3101, 0xFFFC);
        ATEST(0x2000, "LDR",  "%2000, R1",  0x3301, 0xFFFC);
        ATEST(0x2000, "LDRB", "RH1, %2000", 0x3001, 0xFFFC);
        ATEST(0x2000, "LDRB", "%2000, RH1", 0x3201, 0xFFFC);
        ATEST(0x2000, "LDRL", "RR2, %2000", 0x3502, 0xFFFC);
        ATEST(0x2000, "LDRL", "%2000, RR2", 0x3702, 0xFFFC);
        ANMEM(0x2000, "LDRL", "%2002, RR2", "%2002, RR2", 0x3702);
        AERRT(0x2000, "LDRL", "%2000, ", ILLEGAL_REGISTER, "", 0x3703, 0xFFFC);
    }

    // Pop
    if (z8001()) {
        TEST("POP",  "R2, @RR4",            0x9742);
        TEST("POP",  "@RR2, @RR4",          0x1742);
        TEST("POP",  "|%120034|, @RR4",     0x5740, 0x1234);
        TEST("POP",  "%561234, @RR4",       0x5740, 0xD600, 0x1234);
        TEST("POP",  "|%120034|(R2), @RR4", 0x5742, 0x1234);
        TEST("POP",  "%561234(R2), @RR4",   0x5742, 0xD600, 0x1234);
        TEST("POPL", "RR2, @RR4",           0x9542);
        TEST("POPL", "@RR2, @RR4",          0x1542);
        TEST("POPL", "|%120034|, @RR4",     0x5540, 0x1234);
        TEST("POPL", "%561234, @RR4",       0x5540, 0xD600, 0x1234);
        TEST("POPL", "|%120034|(R2), @RR4", 0x5542, 0x1234);
        TEST("POPL", "%561234(R2), @RR4",   0x5542, 0xD600, 0x1234);
        ERRT("POP",  "R5, @RR4",            REGISTERS_OVERLAPPED, "RR4", 0x9745);
        ERRT("POP",  "@RR4, @RR4",          REGISTERS_OVERLAPPED, "RR4", 0x1744);
        ERRT("POP",  "|%120034|(R4), @RR4", REGISTERS_OVERLAPPED, "RR4", 0x5744, 0x1234);
        ERRT("POPL", "RR4, @RR4",           REGISTERS_OVERLAPPED, "RR4", 0x9544);
        ERRT("POPL", "@RR4, @RR4",          REGISTERS_OVERLAPPED, "RR4",0x1544);
        ERRT("POPL", "|%120034|(R5), @RR4", REGISTERS_OVERLAPPED, "RR4", 0x5545, 0x1234);
    } else {
        TEST("POP",  "R2, @R4",        0x9742);
        TEST("POP",  "@R2, @R4",       0x1742);
        TEST("POP",  "%1234, @R4",     0x5740, 0x1234);
        TEST("POP",  "%1234(R2), @R4", 0x5742, 0x1234);
        TEST("POPL", "RR2, @R4",       0x9542);
        TEST("POPL", "@R2, @R4",       0x1542);
        TEST("POPL", "%1234, @R4",     0x5540, 0x1234);
        TEST("POPL", "%1234(R2), @R4", 0x5542, 0x1234);
        ERRT("POP",  "R4, @R4",        REGISTERS_OVERLAPPED, "R4", 0x9744);
        ERRT("POP",  "@R4, @R4",       REGISTERS_OVERLAPPED, "R4", 0x1744);
        ERRT("POP",  "%1234(R4), @R4", REGISTERS_OVERLAPPED, "R4", 0x5744, 0x1234);
        ERRT("POPL", "RR2, @R3",       REGISTERS_OVERLAPPED, "R3", 0x9532);
        ERRT("POPL", "@R4, @R4",       REGISTERS_OVERLAPPED, "R4", 0x1544);
        ERRT("POPL", "%1234(R4), @R4", REGISTERS_OVERLAPPED, "R4", 0x5544, 0x1234);
    }

    // Push
    if (z8001()) {
        TEST("PUSH",  "@RR4, R2",            0x9342);
        TEST("PUSH",  "@RR4, @RR2",          0x1342);
        TEST("PUSH",  "@RR4, |%120034|",     0x5340, 0x1234);
        TEST("PUSH",  "@RR4, %561234",       0x5340, 0xD600, 0x1234);
        TEST("PUSH",  "@RR4, |%120034|(R2)", 0x5342, 0x1234);
        TEST("PUSH",  "@RR4, %561234(R2)",   0x5342, 0xD600, 0x1234);
        TEST("PUSHL", "@RR4, RR2",           0x9142);
        ERRT("PUSHL", "@RR2, @RR0",          REGISTER_NOT_ALLOWED, "RR0", 0x1120);
        TEST("PUSHL", "@RR4, @RR2",          0x1142);
        TEST("PUSHL", "@RR4, |%120034|",     0x5140, 0x1234);
        TEST("PUSHL", "@RR4, %561234",       0x5140, 0xD600, 0x1234);
        TEST("PUSHL", "@RR4, |%120034|(R2)", 0x5142, 0x1234);
        TEST("PUSHL", "@RR4, %561234(R2)",   0x5142, 0xD600, 0x1234);
        TEST("PUSH",  "@RR4, #%1234",        0x0D49, 0x1234);
        ERRT("PUSH",  "@RR4, R5",            REGISTERS_OVERLAPPED, "R5",  0x9345);
        ERRT("PUSH",  "@RR4, @RR4",          REGISTERS_OVERLAPPED, "RR4", 0x1344);
        ERRT("PUSH",  "@RR4, |%120034|(R4)", REGISTERS_OVERLAPPED, "R4)", 0x5344, 0x1234);
        ERRT("PUSHL", "@RR4, RR4",           REGISTERS_OVERLAPPED, "RR4", 0x9144);
        ERRT("PUSHL", "@RR4, @RR4",          REGISTERS_OVERLAPPED, "RR4", 0x1144);
        ERRT("PUSHL", "@RR4, |%120034|(R5)", REGISTERS_OVERLAPPED, "R5)", 0x5145, 0x1234);
    } else {
        TEST("PUSH",  "@R4, R2",        0x9342);
        TEST("PUSH",  "@R4, @R2",       0x1342);
        TEST("PUSH",  "@R4, %1234",     0x5340, 0x1234);
        TEST("PUSH",  "@R4, %1234(R2)", 0x5342, 0x1234);
        TEST("PUSHL", "@R4, RR2",       0x9142);
        TEST("PUSHL", "@R4, @R2",       0x1142);
        ERRT("PUSHL", "@R4, @R0",       REGISTER_NOT_ALLOWED, "R0", 0x1140);
        TEST("PUSHL", "@R4, %1234",     0x5140, 0x1234);
        TEST("PUSHL", "@R4, %1234(R2)", 0x5142, 0x1234);
        TEST("PUSH",  "@R4, #%1234",    0x0D49, 0x1234);
        ERRT("PUSH",  "@R4, R4",        REGISTERS_OVERLAPPED, "R4",   0x9344);
        ERRT("PUSH",  "@R4, @R4",       REGISTERS_OVERLAPPED, "R4",   0x1344);
        ERRT("PUSH",  "@R4, %1234(R4)", REGISTERS_OVERLAPPED, "R4)",  0x5344, 0x1234);
        ERRT("PUSHL", "@R5, RR4",       REGISTERS_OVERLAPPED, "RR4",  0x9154);
        ERRT("PUSHL", "@R4, @R4",       REGISTERS_OVERLAPPED, "R4",   0x1144);
        ERRT("PUSHL", "@R4, %1234(R4)", REGISTERS_OVERLAPPED, "R4)",  0x5144, 0x1234);
    }
}

static void test_arithmetic() {
    // Add with Carry
    TEST("ADC",  "R1, R2",   0xB521);
    TEST("ADCB", "RH1, RL1", 0xB491);

    // Add
    TEST("ADD",  "R1, R2",   0x8121);
    TEST("ADDB", "RH1, RL1", 0x8091);
    TEST("ADDL", "RR2, RR4", 0x9642);
    TEST("ADD",  "R1, #%1234", 0x0101, 0x1234);
    TEST("ADDB", "RH1, #%56",  0x0001, 0x5656);
    TEST("ADDL", "RR2, #%12345678", 0x1602, 0x1234, 0x5678);
    if (z8001()) {
        TEST("ADD",  "R9, @RR2",           0x0129);
        TEST("ADD",  "R9, |%120034|",      0x4109, 0x1234);
        TEST("ADD",  "R9, %561234",        0x4109, 0xD600, 0x1234);
        TEST("ADD",  "R9, |%120034|(R2)",  0x4129, 0x1234);
        TEST("ADD",  "R9, %561234(R2)",    0x4129, 0xD600, 0x1234);
        TEST("ADDB", "RL1, @RR2",          0x0029);
        TEST("ADDB", "RL1, |%120034|",     0x4009, 0x1234);
        TEST("ADDB", "RL1, %561234",       0x4009, 0xD600, 0x1234);
        TEST("ADDB", "RL1, |%120034|(R2)", 0x4029, 0x1234);
        TEST("ADDB", "RL1, %561234(R2)",   0x4029, 0xD600, 0x1234);
        TEST("ADDL", "RR2, @RR8",          0x1682);
        TEST("ADDL", "RR2, |%120034|",     0x5602, 0x1234);
        TEST("ADDL", "RR2, %561234",       0x5602, 0xD600, 0x1234);
        TEST("ADDL", "RR2, |%120034|(R8)", 0x5682, 0x1234);
        TEST("ADDL", "RR2, %561234(R8)",   0x5682, 0xD600, 0x1234);
    } else {
        TEST("ADD",  "R9, @R2",        0x0129);
        TEST("ADD",  "R9, %1234",      0x4109, 0x1234);
        TEST("ADD",  "R9, %1234(R2)",  0x4129, 0x1234);
        TEST("ADDB", "RL1, @R2",       0x0029);
        TEST("ADDB", "RL1, %1234",     0x4009, 0x1234);
        TEST("ADDB", "RL1, %1234(R2)", 0x4029, 0x1234);
        TEST("ADDL", "RR2, @R8",       0x1682);
        TEST("ADDL", "RR2, %1234",     0x5602, 0x1234);
        TEST("ADDL", "RR2, %1234(R8)", 0x5682, 0x1234);
    }

    // Compare
    TEST("CP",  "R1, R2",   0x8b21);
    TEST("CPB", "RH1, RL1", 0x8a91);
    TEST("CPL", "RR2, RR4", 0x9042);
    TEST("CP",  "R1, #%1234", 0x0B01, 0x1234);
    TEST("CPB", "RH1, #%56",  0x0a01, 0x5656);
    TEST("CPL", "RR2, #%12345678", 0x1002, 0x1234, 0x5678);
    if (z8001()) {
        TEST("CP",  "R9, @RR2",           0x0B29);
        TEST("CP",  "R9, |%120034|",      0x4B09, 0x1234);
        TEST("CP",  "R9, %561234",        0x4B09, 0xD600, 0x1234);
        TEST("CP",  "R9, |%120034|(R2)",  0x4B29, 0x1234);
        TEST("CP",  "R9, %561234(R2)",    0x4B29, 0xD600, 0x1234);
        TEST("CPB", "RL1, @RR2",          0x0A29);
        TEST("CPB", "RL1, |%120034|",     0x4A09, 0x1234);
        TEST("CPB", "RL1, %561234",       0x4A09, 0xD600, 0x1234);
        TEST("CPB", "RL1, |%120034|(R2)", 0x4A29, 0x1234);
        TEST("CPB", "RL1, %561234(R2)",   0x4A29, 0xD600, 0x1234);
        TEST("CPL", "RR2, @RR8",          0x1082);
        TEST("CPL", "RR2, |%120034|",     0x5002, 0x1234);
        TEST("CPL", "RR2, %561234",       0x5002, 0xD600, 0x1234);
        TEST("CPL", "RR2, |%120034|(R8)", 0x5082, 0x1234);
        TEST("CPL", "RR2, %561234(R8)",   0x5082, 0xD600, 0x1234);
    } else {
        TEST("CP",  "R9, @R2",        0x0B29);
        TEST("CP",  "R9, %1234",      0x4B09, 0x1234);
        TEST("CP",  "R9, %1234(R2)",  0x4B29, 0x1234);
        TEST("CPB", "RL1, @R2",       0x0A29);
        TEST("CPB", "RL1, %1234",     0x4A09, 0x1234);
        TEST("CPB", "RL1, %1234(R2)", 0x4A29, 0x1234);
        TEST("CPL", "RR2, @R8",       0x1082);
        TEST("CPL", "RR2, %1234",     0x5002, 0x1234);
        TEST("CPL", "RR2, %1234(R8)", 0x5082, 0x1234);
    }
    if (z8001()) {
        TEST("CP",  "@RR2, #%5678",          0x0D21, 0x5678);
        TEST("CP",  "|%120034|, #%5678",     0x4D01, 0x1234, 0x5678);
        TEST("CP",  "%561234, #%5678",       0x4D01, 0xD600, 0x1234, 0x5678);
        TEST("CP",  "|%120034|(R2), #%5678", 0x4D21, 0x1234, 0x5678);
        TEST("CP",  "%561234(R2), #%5678",   0x4D21, 0xD600, 0x1234, 0x5678);
        TEST("CPB", "@RR2, #%56",            0x0C21, 0x5656);
        TEST("CPB", "|%120034|, #%56",       0x4C01, 0x1234, 0x5656);
        TEST("CPB", "%561234, #%56",         0x4C01, 0xD600, 0x1234, 0x5656);
        TEST("CPB", "|%120034|(R2), #%56",   0x4C21, 0x1234, 0x5656);
        TEST("CPB", "%561234(R2), #%56",     0x4C21, 0xD600, 0x1234, 0x5656);
    } else {
        TEST("CP",  "@R2, #%5678",       0x0D21, 0x5678);
        TEST("CP",  "%1234, #%5678",     0x4D01, 0x1234, 0x5678);
        TEST("CP",  "%1234(R2), #%5678", 0x4D21, 0x1234, 0x5678);
        TEST("CPB", "@R2, #%56",         0x0C21, 0x5656);
        TEST("CPB", "%1234, #%56",       0x4C01, 0x1234, 0x5656);
        TEST("CPB", "%1234(R2), #%56",   0x4C21, 0x1234, 0x5656);
    }

    // Decimal Adjust
    TEST("DAB", "RH1", 0xB010);
    TEST("DAB", "RH7", 0xB070);
    TEST("DAB", "RL0", 0xB080);
    TEST("DAB", "RL7", 0xB0F0);

    // Decrement
    TEST("DEC",  "R1, #1",   0xAB10);
    TEST("DEC",  "R1, #16",  0xAB1F);
    TEST("DECB", "RH1, #1",  0xAA10);
    TEST("DECB", "RH1, #16", 0xAA1F);
    if (z8001()) {
        TEST("DEC",  "@RR2, #2",          0x2B21);
        TEST("DEC",  "|%120034|, #4",     0x6B03, 0x1234);
        TEST("DEC",  "%561234, #4",       0x6B03, 0xD600, 0x1234);
        TEST("DEC",  "|%120034|(R2), #8", 0x6B27, 0x1234);
        TEST("DEC",  "%561234(R2), #8",   0x6B27, 0xD600, 0x1234);
        TEST("DECB", "@RR2, #2",          0x2A21);
        TEST("DECB", "|%120034|, #4",     0x6A03, 0x1234);
        TEST("DECB", "%561234, #4",       0x6A03, 0xD600, 0x1234);
        TEST("DECB", "|%120034|(R2), #8", 0x6A27, 0x1234);
        TEST("DECB", "%561234(R2), #8",   0x6A27, 0xD600, 0x1234);
    } else {
        TEST("DEC",  "@R2, #2",       0x2B21);
        TEST("DEC",  "%1234, #4",     0x6B03, 0x1234);
        TEST("DEC",  "%1234(R2), #8", 0x6B27, 0x1234);
        TEST("DECB", "@R2, #2",       0x2A21);
        TEST("DECB", "%1234, #4",     0x6A03, 0x1234);
        TEST("DECB", "%1234(R2), #8", 0x6A27, 0x1234);
    }

    // Divide
    TEST("DIV",  "RR4, R2",         0x9B24);
    TEST("DIV",  "RR4, #%1234",     0x1B04, 0x1234);
    TEST("DIVL", "RQ4, RR2",        0x9A24);
    TEST("DIVL", "RQ4, #%12345678", 0x1A04, 0x1234, 0x5678);
    if (z8001()) {
        TEST("DIV",  "RR4, @RR2",          0x1B24);
        TEST("DIV",  "RR4, |%120034|",     0x5B04, 0x1234);
        TEST("DIV",  "RR4, %561234",       0x5B04, 0xD600, 0x1234);
        TEST("DIV",  "RR4, |%120034|(R2)", 0x5B24, 0x1234);
        TEST("DIV",  "RR4, %561234(R2)",   0x5B24, 0xD600, 0x1234);
        TEST("DIVL", "RQ4, @RR2",          0x1A24);
        TEST("DIVL", "RQ4, |%120034|",     0x5A04, 0x1234);
        TEST("DIVL", "RQ4, %561234",       0x5A04, 0xD600, 0x1234);
        TEST("DIVL", "RQ4, |%120034|(R2)", 0x5A24, 0x1234);
        TEST("DIVL", "RQ4, %561234(R2)",   0x5A24, 0xD600, 0x1234);
    } else {
        TEST("DIV",  "RR4, @R2",       0x1B24);
        TEST("DIV",  "RR4, %1234",     0x5B04, 0x1234);
        TEST("DIV",  "RR4, %1234(R2)", 0x5B24, 0x1234);
        TEST("DIVL", "RQ4, @R2",       0x1A24);
        TEST("DIVL", "RQ4, %1234",     0x5A04, 0x1234);
        TEST("DIVL", "RQ4, %1234(R2)", 0x5A24, 0x1234);
    }

    // Extend Sign
    TEST("EXTSB", "R1",  0xB110);
    TEST("EXTSB", "R7",  0xB170);
    TEST("EXTSB", "R8",  0xB180);
    TEST("EXTSB", "R15", 0xB1F0);
    TEST("EXTS",  "RR2", 0xB12A);
    TEST("EXTS",  "RR8", 0xB18A);
    TEST("EXTSL", "RQ4", 0xB147);
    TEST("EXTSL", "RQ8", 0xB187);
    ERRT("EXTS",  "", ILLEGAL_REGISTER, "", 0xB17A);
    ERRT("EXTS",  "", ILLEGAL_REGISTER, "", 0xB1FA);
    ERRT("EXTSL", "", ILLEGAL_REGISTER, "", 0xB167);
    ERRT("EXTSL", "", ILLEGAL_REGISTER, "", 0xB1E7);

    // Increment
    TEST("INC",  "R1, #1",   0xA910);
    TEST("INC",  "R1, #16",  0xA91F);
    TEST("INCB", "RH1, #1",  0xA810);
    TEST("INCB", "RH1, #16", 0xA81F);
    if (z8001()) {
        TEST("INC",  "@RR2, #2",          0x2921);
        TEST("INC",  "|%120034|, #4",     0x6903, 0x1234);
        TEST("INC",  "%561234, #4",       0x6903, 0xD600, 0x1234);
        TEST("INC",  "|%120034|(R2), #8", 0x6927, 0x1234);
        TEST("INC",  "%561234(R2), #8",   0x6927, 0xD600, 0x1234);
        TEST("INCB", "@RR2, #2",          0x2821);
        TEST("INCB", "|%120034|, #4",     0x6803, 0x1234);
        TEST("INCB", "%561234, #4",       0x6803, 0xD600, 0x1234);
        TEST("INCB", "|%120034|(R2), #8", 0x6827, 0x1234);
        TEST("INCB", "%561234(R2), #8",   0x6827, 0xD600, 0x1234);
    } else {
        TEST("INC",  "@R2, #2",       0x2921);
        TEST("INC",  "%1234, #4",     0x6903, 0x1234);
        TEST("INC",  "%1234(R2), #8", 0x6927, 0x1234);
        TEST("INCB", "@R2, #2",       0x2821);
        TEST("INCB", "%1234, #4",     0x6803, 0x1234);
        TEST("INCB", "%1234(R2), #8", 0x6827, 0x1234);
    }

    // Multiply
    TEST("MULT",  "RR4, R2",         0x9924);
    TEST("MULT",  "RR4, #%1234",     0x1904, 0x1234);
    TEST("MULTL", "RQ4, RR2",        0x9824);
    TEST("MULTL", "RQ4, #%12345678", 0x1804, 0x1234, 0x5678);
    if (z8001()) {
        TEST("MULT",  "RR4, @RR2",          0x1924);
        TEST("MULT",  "RR4, |%120034|",     0x5904, 0x1234);
        TEST("MULT",  "RR4, %561234",       0x5904, 0xD600, 0x1234);
        TEST("MULT",  "RR4, |%120034|(R2)", 0x5924, 0x1234);
        TEST("MULT",  "RR4, %561234(R2)",   0x5924, 0xD600, 0x1234);
        TEST("MULTL", "RQ4, @RR2",          0x1824);
        TEST("MULTL", "RQ4, |%120034|",     0x5804, 0x1234);
        TEST("MULTL", "RQ4, %561234",       0x5804, 0xD600, 0x1234);
        TEST("MULTL", "RQ4, |%120034|(R2)", 0x5824, 0x1234);
        TEST("MULTL", "RQ4, %561234(R2)",   0x5824, 0xD600, 0x1234);
    } else {
        TEST("MULT",  "RR4, @R2",       0x1924);
        TEST("MULT",  "RR4, %1234",     0x5904, 0x1234);
        TEST("MULT",  "RR4, %1234(R2)", 0x5924, 0x1234);
        TEST("MULTL", "RQ4, @R2",       0x1824);
        TEST("MULTL", "RQ4, %1234",     0x5804, 0x1234);
        TEST("MULTL", "RQ4, %1234(R2)", 0x5824, 0x1234);
    }

    // Negate
    TEST("NEG",  "R2",  0x8D22);
    TEST("NEGB", "RL2", 0x8CA2);
    if (z8001()) {
        TEST("NEG",  "@RR2",          0x0D22);
        TEST("NEG",  "|%120034|",     0x4D02, 0x1234);
        TEST("NEG",  "%561234",       0x4D02, 0xD600, 0x1234);
        TEST("NEG",  "|%120034|(R2)", 0x4D22, 0x1234);
        TEST("NEG",  "%561234(R2)",   0x4D22, 0xD600, 0x1234);
        TEST("NEGB", "@RR2",          0x0C22);
        TEST("NEGB", "|%120034|",     0x4C02, 0x1234);
        TEST("NEGB", "%561234",       0x4C02, 0xD600, 0x1234);
        TEST("NEGB", "|%120034|(R2)", 0x4C22, 0x1234);
        TEST("NEGB", "%561234(R2)",   0x4C22, 0xD600, 0x1234);
    } else {
        TEST("NEG",  "@R2",       0x0D22);
        TEST("NEG",  "%1234",     0x4D02, 0x1234);
        TEST("NEG",  "%1234(R2)", 0x4D22, 0x1234);
        TEST("NEGB", "@R2",       0x0C22);
        TEST("NEGB", "%1234",     0x4C02, 0x1234);
        TEST("NEGB", "%1234(R2)", 0x4C22, 0x1234);
    }

    // Subtract with Carry
    TEST("SBC",  "R1, R2",   0xB721);
    TEST("SBCB", "RH1, RL1", 0xB691);

    // Subtract
    TEST("SUB",  "R1, R2",   0x8321);
    TEST("SUBB", "RH1, RL1", 0x8291);
    TEST("SUBL", "RR2, RR4", 0x9242);
    TEST("SUB",  "R1, #%1234", 0x0301, 0x1234);
    TEST("SUBB", "RH1, #%56",  0x0201, 0x5656);
    TEST("SUBL", "RR2, #%12345678", 0x1202, 0x1234, 0x5678);
    if (z8001()) {
        TEST("SUB",  "R9, @RR2",           0x0329);
        TEST("SUB",  "R9, |%120034|",      0x4309, 0x1234);
        TEST("SUB",  "R9, %561234",        0x4309, 0xD600, 0x1234);
        TEST("SUB",  "R9, |%120034|(R2)",  0x4329, 0x1234);
        TEST("SUB",  "R9, %561234(R2)",    0x4329, 0xD600, 0x1234);
        TEST("SUBB", "RL1, @RR2",          0x0229);
        TEST("SUBB", "RL1, |%120034|",     0x4209, 0x1234);
        TEST("SUBB", "RL1, %561234",       0x4209, 0xD600, 0x1234);
        TEST("SUBB", "RL1, |%120034|(R2)", 0x4229, 0x1234);
        TEST("SUBB", "RL1, %561234(R2)",   0x4229, 0xD600, 0x1234);
        TEST("SUBL", "RR2, @RR8",          0x1282);
        TEST("SUBL", "RR2, |%120034|",     0x5202, 0x1234);
        TEST("SUBL", "RR2, %561234",       0x5202, 0xD600, 0x1234);
        TEST("SUBL", "RR2, |%120034|(R8)", 0x5282, 0x1234);
        TEST("SUBL", "RR2, %561234(R8)",   0x5282, 0xD600, 0x1234);
    } else {
        TEST("SUB",  "R9, @R2",        0x0329);
        TEST("SUB",  "R9, %1234",      0x4309, 0x1234);
        TEST("SUB",  "R9, %1234(R2)",  0x4329, 0x1234);
        TEST("SUBB", "RL1, @R2",       0x0229);
        TEST("SUBB", "RL1, %1234",     0x4209, 0x1234);
        TEST("SUBB", "RL1, %1234(R2)", 0x4229, 0x1234);
        TEST("SUBL", "RR2, @R8",       0x1282);
        TEST("SUBL", "RR2, %1234",     0x5202, 0x1234);
        TEST("SUBL", "RR2, %1234(R8)", 0x5282, 0x1234);
    }
}

static void test_logical() {
    // And
    TEST("AND",  "R1, R2",   0x8721);
    TEST("ANDB", "RH1, RL1", 0x8691);
    TEST("AND",  "R1, #%1234", 0x0701, 0x1234);
    TEST("ANDB", "RH1, #%56",  0x0601, 0x5656);
    if (z8001()) {
        TEST("AND",  "R9, @RR2",           0x0729);
        TEST("AND",  "R9, |%120034|",      0x4709, 0x1234);
        TEST("AND",  "R9, %561234",        0x4709, 0xD600, 0x1234);
        TEST("AND",  "R9, |%120034|(R2)",  0x4729, 0x1234);
        TEST("AND",  "R9, %561234(R2)",    0x4729, 0xD600, 0x1234);
        TEST("ANDB", "RL1, @RR2",          0x0629);
        TEST("ANDB", "RL1, |%120034|",     0x4609, 0x1234);
        TEST("ANDB", "RL1, %561234",       0x4609, 0xD600, 0x1234);
        TEST("ANDB", "RL1, |%120034|(R2)", 0x4629, 0x1234);
        TEST("ANDB", "RL1, %561234(R2)",   0x4629, 0xD600, 0x1234);
    } else {
        TEST("AND",  "R9, @R2",        0x0729);
        TEST("AND",  "R9, %1234",      0x4709, 0x1234);
        TEST("AND",  "R9, %1234(R2)",  0x4729, 0x1234);
        TEST("ANDB", "RL1, @R2",       0x0629);
        TEST("ANDB", "RL1, %1234",     0x4609, 0x1234);
        TEST("ANDB", "RL1, %1234(R2)", 0x4629, 0x1234);
    }

    // Complement
    TEST("COM",  "R2",  0x8D20);
    TEST("COMB", "RL1", 0x8C90);
    if (z8001()) {
        TEST("COM",  "@RR2",          0x0D20);
        TEST("COM",  "|%120034|",     0x4D00, 0x1234);
        TEST("COM",  "%561234",       0x4D00, 0xD600, 0x1234);
        TEST("COM",  "|%120034|(R2)", 0x4D20, 0x1234);
        TEST("COM",  "%561234(R2)",   0x4D20, 0xD600, 0x1234);
        TEST("COMB", "@RR2",          0x0C20);
        TEST("COMB", "|%120034|",     0x4C00, 0x1234);
        TEST("COMB", "%561234",       0x4C00, 0xD600, 0x1234);
        TEST("COMB", "|%120034|(R2)", 0x4C20, 0x1234);
        TEST("COMB", "%561234(R2)",   0x4C20, 0xD600, 0x1234);
    } else {
        TEST("COM",  "@R2",       0x0D20);
        TEST("COM",  "%1234",     0x4D00, 0x1234);
        TEST("COM",  "%1234(R2)", 0x4D20, 0x1234);
        TEST("COMB", "@R2",       0x0C20);
        TEST("COMB", "%1234",     0x4C00, 0x1234);
        TEST("COMB", "%1234(R2)", 0x4C20, 0x1234);
    }

    // Or
    TEST("OR",  "R1, R2",   0x8521);
    TEST("ORB", "RH1, RL1", 0x8491);
    TEST("OR",  "R1, #%1234", 0x0501, 0x1234);
    TEST("ORB", "RH1, #%56",  0x0401, 0x5656);
    if (z8001()) {
        TEST("OR",  "R9, @RR2",           0x0529);
        TEST("OR",  "R9, |%120034|",      0x4509, 0x1234);
        TEST("OR",  "R9, %561234",        0x4509, 0xD600, 0x1234);
        TEST("OR",  "R9, |%120034|(R2)",  0x4529, 0x1234);
        TEST("OR",  "R9, %561234(R2)",    0x4529, 0xD600, 0x1234);
        TEST("ORB", "RL1, @RR2",          0x0429);
        TEST("ORB", "RL1, |%120034|",     0x4409, 0x1234);
        TEST("ORB", "RL1, %561234",       0x4409, 0xD600, 0x1234);
        TEST("ORB", "RL1, |%120034|(R2)", 0x4429, 0x1234);
        TEST("ORB", "RL1, %561234(R2)",   0x4429, 0xD600, 0x1234);
    } else {
        TEST("OR",  "R9, @R2",        0x0529);
        TEST("OR",  "R9, %1234",      0x4509, 0x1234);
        TEST("OR",  "R9, %1234(R2)",  0x4529, 0x1234);
        TEST("ORB", "RL1, @R2",       0x0429);
        TEST("ORB", "RL1, %1234",     0x4409, 0x1234);
        TEST("ORB", "RL1, %1234(R2)", 0x4429, 0x1234);
    }

    // Test
    TEST("TEST",  "R2",  0x8D24);
    TEST("TESTB", "RL1", 0x8C94);
    TEST("TESTL", "RR2", 0x9C28);
    if (z8001()) {
        TEST("TEST",  "@RR2",          0x0D24);
        TEST("TEST",  "|%120034|",     0x4D04, 0x1234);
        TEST("TEST",  "%561234",       0x4D04, 0xD600, 0x1234);
        TEST("TEST",  "|%120034|(R2)", 0x4D24, 0x1234);
        TEST("TEST",  "%561234(R2)",   0x4D24, 0xD600, 0x1234);
        TEST("TESTB", "@RR2",          0x0C24);
        TEST("TESTB", "|%120034|",     0x4C04, 0x1234);
        TEST("TESTB", "%561234",       0x4C04, 0xD600, 0x1234);
        TEST("TESTB", "|%120034|(R2)", 0x4C24, 0x1234);
        TEST("TESTB", "%561234(R2)",   0x4C24, 0xD600, 0x1234);
        TEST("TESTL", "@RR2",          0x1C28);
        TEST("TESTL", "|%120034|",     0x5C08, 0x1234);
        TEST("TESTL", "%561234",       0x5C08, 0xD600, 0x1234);
        TEST("TESTL", "|%120034|(R2)", 0x5C28, 0x1234);
        TEST("TESTL", "%561234(R2)",   0x5C28, 0xD600, 0x1234);
    } else {
        TEST("TEST",  "@R2",       0x0D24);
        TEST("TEST",  "%1234",     0x4D04, 0x1234);
        TEST("TEST",  "%1234(R2)", 0x4D24, 0x1234);
        TEST("TESTB", "@R2",       0x0C24);
        TEST("TESTB", "%1234",     0x4C04, 0x1234);
        TEST("TESTB", "%1234(R2)", 0x4C24, 0x1234);
        TEST("TESTL", "@R2",       0x1C28);
        TEST("TESTL", "%1234",     0x5C08, 0x1234);
        TEST("TESTL", "%1234(R2)", 0x5C28, 0x1234);
    }

    // Exclusive Or
    TEST("XOR",  "R1, R2",   0x8921);
    TEST("XORB", "RH1, RL1", 0x8891);
    TEST("XOR",  "R1, #%1234", 0x0901, 0x1234);
    TEST("XORB", "RH1, #%56",  0x0801, 0x5656);
    if (z8001()) {
        TEST("XOR",  "R9, @RR2",           0x0929);
        TEST("XOR",  "R9, |%120034|",      0x4909, 0x1234);
        TEST("XOR",  "R9, %561234",        0x4909, 0xD600, 0x1234);
        TEST("XOR",  "R9, |%120034|(R2)",  0x4929, 0x1234);
        TEST("XOR",  "R9, %561234(R2)",    0x4929, 0xD600, 0x1234);
        TEST("XORB", "RL1, @RR2",          0x0829);
        TEST("XORB", "RL1, |%120034|",     0x4809, 0x1234);
        TEST("XORB", "RL1, %561234",       0x4809, 0xD600, 0x1234);
        TEST("XORB", "RL1, |%120034|(R2)", 0x4829, 0x1234);
        TEST("XORB", "RL1, %561234(R2)",   0x4829, 0xD600, 0x1234);
    } else {
        TEST("XOR",  "R9, @R2",        0x0929);
        TEST("XOR",  "R9, %1234",      0x4909, 0x1234);
        TEST("XOR",  "R9, %1234(R2)",  0x4929, 0x1234);
        TEST("XORB", "RL1, @R2",       0x0829);
        TEST("XORB", "RL1, %1234",     0x4809, 0x1234);
        TEST("XORB", "RL1, %1234(R2)", 0x4829, 0x1234);
    }
}

static void test_program_control() {
    // Call Procedure
    if (z8001()) {
        TEST("CALL", "@RR2",          0x1F20);
        TEST("CALL", "|%120034|",     0x5F00, 0x1234);
        TEST("CALL", "%561234",       0x5F00, 0xD600, 0x1234);
        TEST("CALL", "|%120034|(R2)", 0x5F20, 0x1234);
        TEST("CALL", "%561234(R2)",   0x5F20, 0xD600, 0x1234);
    } else {
        TEST("CALL", "@R2",       0x1F20);
        TEST("CALL", "%1234",     0x5F00, 0x1234);
        TEST("CALL", "%1234(R2)", 0x5F20, 0x1234);
    }

    // Call Procedure Relative
    disassembler.setOption("relative", "on");
    ATEST(0x1000, "CALR", "$+2",    0xD000);
    ATEST(0x1000, "CALR", "$+10",   0xDFFC);
    ATEST(0x1000, "CALR", "$+4098", 0xD800);
    ATEST(0x1000, "CALR", "$-4092", 0xD7FF);
    ATEST(0x1000, "CALR", "$-10",   0xD006);
    ATEST(0x1000, "CALR", "$",      0xD001);

    // Decrement and Jump if Not Zero
    TEST("DJNZ",  "R2, $",  0xF281);
    TEST("DBJNZ", "RL0, $", 0xF801);

    // Interrupt Return
    TEST("IRET", "", 0x7B00);

    // Jump
    if (z8001()) {
        TEST("JP", "F, @RR2",          0x1E20);
        TEST("JP", "F, |%120034|",     0x5E00, 0x1234);
        TEST("JP", "F, %561234",       0x5E00, 0xD600, 0x1234);
        TEST("JP", "F, |%120034|(R2)", 0x5E20, 0x1234);
        TEST("JP", "F, %561234(R2)",   0x5E20, 0xD600, 0x1234);
    } else {
        TEST("JP", "F, @R2",       0x1E20);
        TEST("JP", "F, %1234",     0x5E00, 0x1234);
        TEST("JP", "F, %1234(R2)", 0x5E20, 0x1234);
    }
    if (z8001()) {
        TEST("JP", "LT, @RR2",          0x1E21);
        TEST("JP", "LT, |%120034|",     0x5E01, 0x1234);
        TEST("JP", "LT, %561234",       0x5E01, 0xD600, 0x1234);
        TEST("JP", "LT, |%120034|(R2)", 0x5E21, 0x1234);
        TEST("JP", "LT, %561234(R2)",   0x5E21, 0xD600, 0x1234);
    } else {
        TEST("JP", "LT, @R2",       0x1E21);
        TEST("JP", "LT, %1234",     0x5E01, 0x1234);
        TEST("JP", "LT, %1234(R2)", 0x5E21, 0x1234);
    }
    if (z8001()) {
        TEST("JP", "LE, @RR2",          0x1E22);
        TEST("JP", "LE, |%120034|",     0x5E02, 0x1234);
        TEST("JP", "LE, %561234",       0x5E02, 0xD600, 0x1234);
        TEST("JP", "LE, |%120034|(R2)", 0x5E22, 0x1234);
        TEST("JP", "LE, %561234(R2)",   0x5E22, 0xD600, 0x1234);
    } else {
        TEST("JP", "LE, @R2",       0x1E22);
        TEST("JP", "LE, %1234",     0x5E02, 0x1234);
        TEST("JP", "LE, %1234(R2)", 0x5E22, 0x1234);
    }
    if (z8001()) {
        TEST("JP", "ULE, @RR2",          0x1E23);
        TEST("JP", "ULE, |%120034|",     0x5E03, 0x1234);
        TEST("JP", "ULE, %561234",       0x5E03, 0xD600, 0x1234);
        TEST("JP", "ULE, |%120034|(R2)", 0x5E23, 0x1234);
        TEST("JP", "ULE, %561234(R2)",   0x5E23, 0xD600, 0x1234);
    } else {
        TEST("JP", "ULE, @R2",       0x1E23);
        TEST("JP", "ULE, %1234",     0x5E03, 0x1234);
        TEST("JP", "ULE, %1234(R2)", 0x5E23, 0x1234);
    }
    if (z8001()) {
        TEST("JP", "OV, @RR2",          0x1E24);
        TEST("JP", "OV, |%120034|",     0x5E04, 0x1234);
        TEST("JP", "OV, %561234",       0x5E04, 0xD600, 0x1234);
        TEST("JP", "OV, |%120034|(R2)", 0x5E24, 0x1234);
        TEST("JP", "OV, %561234(R2)",   0x5E24, 0xD600, 0x1234);
    } else {
        TEST("JP", "OV, @R2",       0x1E24);
        TEST("JP", "OV, %1234",     0x5E04, 0x1234);
        TEST("JP", "OV, %1234(R2)", 0x5E24, 0x1234);
    }
    if (z8001()) {
        TEST("JP", "MI, @RR2",          0x1E25);
        TEST("JP", "MI, |%120034|",     0x5E05, 0x1234);
        TEST("JP", "MI, %561234",       0x5E05, 0xD600, 0x1234);
        TEST("JP", "MI, |%120034|(R2)", 0x5E25, 0x1234);
        TEST("JP", "MI, %561234(R2)",   0x5E25, 0xD600, 0x1234);
    } else {
        TEST("JP", "MI, @R2",       0x1E25);
        TEST("JP", "MI, %1234",     0x5E05, 0x1234);
        TEST("JP", "MI, %1234(R2)", 0x5E25, 0x1234);
    }
    if (z8001()) {
        TEST("JP", "Z, @RR2",          0x1E26);
        TEST("JP", "Z, |%120034|",     0x5E06, 0x1234);
        TEST("JP", "Z, %561234",       0x5E06, 0xD600, 0x1234);
        TEST("JP", "Z, |%120034|(R2)", 0x5E26, 0x1234);
        TEST("JP", "Z, %561234(R2)",   0x5E26, 0xD600, 0x1234);
    } else {
        TEST("JP", "Z, @R2",       0x1E26);
        TEST("JP", "Z, %1234",     0x5E06, 0x1234);
        TEST("JP", "Z, %1234(R2)", 0x5E26, 0x1234);
    }
    if (z8001()) {
        TEST("JP", "C, @RR2",          0x1E27);
        TEST("JP", "C, |%120034|",     0x5E07, 0x1234);
        TEST("JP", "C, %561234",       0x5E07, 0xD600, 0x1234);
        TEST("JP", "C, |%120034|(R2)", 0x5E27, 0x1234);
        TEST("JP", "C, %561234(R2)",   0x5E27, 0xD600, 0x1234);
    } else {
        TEST("JP", "C, @R2",       0x1E27);
        TEST("JP", "C, %1234",     0x5E07, 0x1234);
        TEST("JP", "C, %1234(R2)", 0x5E27, 0x1234);
    }
    if (z8001()) {
        TEST("JP", "@RR2",          0x1E28);
        TEST("JP", "|%120034|",     0x5E08, 0x1234);
        TEST("JP", "%561234",       0x5E08, 0xD600, 0x1234);
        TEST("JP", "|%120034|(R2)", 0x5E28, 0x1234);
        TEST("JP", "%561234(R2)",   0x5E28, 0xD600, 0x1234);
    } else {
        TEST("JP", "@R2",       0x1E28);
        TEST("JP", "%1234",     0x5E08, 0x1234);
        TEST("JP", "%1234(R2)", 0x5E28, 0x1234);
    }
    if (z8001()) {
        TEST("JP", "GE, @RR2",          0x1E29);
        TEST("JP", "GE, |%120034|",     0x5E09, 0x1234);
        TEST("JP", "GE, %561234",       0x5E09, 0xD600, 0x1234);
        TEST("JP", "GE, |%120034|(R2)", 0x5E29, 0x1234);
        TEST("JP", "GE, %561234(R2)",   0x5E29, 0xD600, 0x1234);
    } else {
        TEST("JP", "GE, @R2",       0x1E29);
        TEST("JP", "GE, %1234",     0x5E09, 0x1234);
        TEST("JP", "GE, %1234(R2)", 0x5E29, 0x1234);
    }
    if (z8001()) {
        TEST("JP", "GT, @RR2",          0x1E2A);
        TEST("JP", "GT, |%120034|",     0x5E0A, 0x1234);
        TEST("JP", "GT, %561234",       0x5E0A, 0xD600, 0x1234);
        TEST("JP", "GT, |%120034|(R2)", 0x5E2A, 0x1234);
        TEST("JP", "GT, %561234(R2)",   0x5E2A, 0xD600, 0x1234);
    } else {
        TEST("JP", "GT, @R2",       0x1E2A);
        TEST("JP", "GT, %1234",     0x5E0A, 0x1234);
        TEST("JP", "GT, %1234(R2)", 0x5E2A, 0x1234);
    }
    if (z8001()) {
        TEST("JP", "UGT, @RR2",          0x1E2B);
        TEST("JP", "UGT, |%120034|",     0x5E0B, 0x1234);
        TEST("JP", "UGT, %561234",       0x5E0B, 0xD600, 0x1234);
        TEST("JP", "UGT, |%120034|(R2)", 0x5E2B, 0x1234);
        TEST("JP", "UGT, %561234(R2)",   0x5E2B, 0xD600, 0x1234);
    } else {
        TEST("JP", "UGT, @R2",       0x1E2B);
        TEST("JP", "UGT, %1234",     0x5E0B, 0x1234);
        TEST("JP", "UGT, %1234(R2)", 0x5E2B, 0x1234);
    }
    if (z8001()) {
        TEST("JP", "NOV, @RR2",          0x1E2C);
        TEST("JP", "NOV, |%120034|",     0x5E0C, 0x1234);
        TEST("JP", "NOV, %561234",       0x5E0C, 0xD600, 0x1234);
        TEST("JP", "NOV, |%120034|(R2)", 0x5E2C, 0x1234);
        TEST("JP", "NOV, %561234(R2)",   0x5E2C, 0xD600, 0x1234);
    } else {
        TEST("JP", "NOV, @R2",       0x1E2C);
        TEST("JP", "NOV, %1234",     0x5E0C, 0x1234);
        TEST("JP", "NOV, %1234(R2)", 0x5E2C, 0x1234);
    }
    if (z8001()) {
        TEST("JP", "PL, @RR2",          0x1E2D);
        TEST("JP", "PL, |%120034|",     0x5E0D, 0x1234);
        TEST("JP", "PL, %561234",       0x5E0D, 0xD600, 0x1234);
        TEST("JP", "PL, |%120034|(R2)", 0x5E2D, 0x1234);
        TEST("JP", "PL, %561234(R2)",   0x5E2D, 0xD600, 0x1234);
    } else {
        TEST("JP", "PL, @R2",       0x1E2D);
        TEST("JP", "PL, %1234",     0x5E0D, 0x1234);
        TEST("JP", "PL, %1234(R2)", 0x5E2D, 0x1234);
    }
    if (z8001()) {
        TEST("JP", "NZ, @RR2",          0x1E2E);
        TEST("JP", "NZ, |%120034|",     0x5E0E, 0x1234);
        TEST("JP", "NZ, %561234",       0x5E0E, 0xD600, 0x1234);
        TEST("JP", "NZ, |%120034|(R2)", 0x5E2E, 0x1234);
        TEST("JP", "NZ, %561234(R2)",   0x5E2E, 0xD600, 0x1234);
    } else {
        TEST("JP", "NZ, @R2",       0x1E2E);
        TEST("JP", "NZ, %1234",     0x5E0E, 0x1234);
        TEST("JP", "NZ, %1234(R2)", 0x5E2E, 0x1234);
    }
    if (z8001()) {
        TEST("JP", "NC, @RR2",          0x1E2F);
        TEST("JP", "NC, |%120034|",     0x5E0F, 0x1234);
        TEST("JP", "NC, %561234",       0x5E0F, 0xD600, 0x1234);
        TEST("JP", "NC, |%120034|(R2)", 0x5E2F, 0x1234);
        TEST("JP", "NC, %561234(R2)",   0x5E2F, 0xD600, 0x1234);
        ERRT("JP", "NC, |%120035|", OPERAND_NOT_ALIGNED, "|%120035|", 0x5E0F, 0x1235);
        ERRT("JP", "NC, %561235",   OPERAND_NOT_ALIGNED,  "%561235",  0x5E0F, 0xD600, 0x1235);
        TEST("JP", "NC, |%120035|(R2)", 0x5E2F, 0x1235);
        TEST("JP", "NC, %561235(R2)",   0x5E2F, 0xD600, 0x1235);
    } else {
        TEST("JP", "NC, @R2",       0x1E2F);
        TEST("JP", "NC, %1234",     0x5E0F, 0x1234);
        ERRT("JP", "NC, %1235", OPERAND_NOT_ALIGNED, "%1235", 0x5E0F, 0x1235);
        TEST("JP", "NC, %1234(R2)", 0x5E2F, 0x1234);
        TEST("JP", "NC, %1235(R2)", 0x5E2F, 0x1235);
    }

    // Jump Relative
    TEST("JR", "F, $",   0xE0FF);
    TEST("JR", "LT, $",  0xE1FF);
    TEST("JR", "LE, $",  0xE2FF);
    TEST("JR", "ULE, $", 0xE3FF);
    TEST("JR", "OV, $",  0xE4FF);
    TEST("JR", "MI, $",  0xE5FF);
    TEST("JR", "Z, $",   0xE6FF);
    TEST("JR", "C, $",   0xE7FF);
    TEST("JR", "$",      0xE8FF);
    TEST("JR", "GE, $",  0xE9FF);
    TEST("JR", "GT, $",  0xEAFF);
    TEST("JR", "UGT, $", 0xEBFF);
    TEST("JR", "NOV, $", 0xECFF);
    TEST("JR", "PL, $",  0xEDFF);
    TEST("JR", "NZ, $",  0xEEFF);
    TEST("JR", "NC, $",  0xEFFF);

    // Return from Procedure
    TEST("RET", "F",   0x9E00);
    TEST("RET", "LT",  0x9E01);
    TEST("RET", "LE",  0x9E02);
    TEST("RET", "ULE", 0x9E03);
    TEST("RET", "OV",  0x9E04);
    TEST("RET", "MI",  0x9E05);
    TEST("RET", "Z",   0x9E06);
    TEST("RET", "C",   0x9E07);
    TEST("RET", "",    0x9E08);
    TEST("RET", "GE",  0x9E09);
    TEST("RET", "GT",  0x9E0A);
    TEST("RET", "UGT", 0x9E0B);
    TEST("RET", "NOV", 0x9E0C);
    TEST("RET", "PL",  0x9E0D);
    TEST("RET", "NZ",  0x9E0E);
    TEST("RET", "NC",  0x9E0F);

    // System call
    TEST("SC", "#0",   0x7F00);
    TEST("SC", "#%FF", 0x7FFF);
}

static void test_bit_manipulation() {
    // Bit Test
    TEST("BIT", "R2, #1", 0xA721);
    TEST("BIT", "R2, R8", 0x2708, 0x0200);
    NMEM("BIT", "", "",   0x2708);
    UNKN(                 0x2708, 0x1200);
    UNKN(                 0x2708, 0x0220);
    UNKN(                 0x2708, 0x0204);
    TEST("BITB", "RL2, #1",                          0xA6A1);
    ERRT("BITB", "RL2, #8", ILLEGAL_BIT_NUMBER, "8", 0xA6A8);
    TEST("BITB", "RL2, R1",                             0x2601, 0x0A00);
    ERRT("BITB", "RL2, R8", REGISTER_NOT_ALLOWED, "R8", 0x2608, 0x0A00);
    UNKN(                                               0x2601, 0x8A00);
    UNKN(                                               0x2601, 0x0A40);
    UNKN(                                               0x2601, 0x0A02);
    NMEM("BITB", "", "",                                0x2601);
    if (z8001()) {
        TEST("BIT",  "@RR2, #3",           0x2723);
        TEST("BIT",  "|%120034|, #4",      0x6704, 0x1234);
        TEST("BIT",  "%561234, #9",        0x6709, 0xD600, 0x1234);
        TEST("BIT",  "|%120034|(R2), #14", 0x672E, 0x1234);
        TEST("BIT",  "%561234(R2), #15",   0x672F, 0xD600, 0x1234);
        TEST("BITB", "@RR2, #3",           0x2623);
        TEST("BITB", "|%120034|, #4",      0x6604, 0x1234);
        TEST("BITB", "%561234, #7",        0x6607, 0xD600, 0x1234);
        TEST("BITB", "|%120034|(R2), #0",  0x6620, 0x1234);
        TEST("BITB", "%561234(R2), #7",    0x6627, 0xD600, 0x1234);
    } else {
        TEST("BIT",  "@R2, #3",        0x2723);
        TEST("BIT",  "%1234, #4",      0x6704, 0x1234);
        TEST("BIT",  "%1234(R2), #14", 0x672E, 0x1234);
        TEST("BITB", "@R2, #3",        0x2623);
        TEST("BITB", "%1234, #4",      0x6604, 0x1234);
        TEST("BITB", "%1234(R2), #7",  0x6627, 0x1234);
    }

    // Reset Bit
    TEST("RES",  "R2, #1",  0xA321);
    TEST("RES",  "R2, R8",  0x2308, 0x0200);
    UNKN(                   0x2308, 0x1200);
    UNKN(                   0x2308, 0x0220);
    UNKN(                   0x2308, 0x0203);
    TEST("RESB", "RL2, #1",                          0xA2A1);
    ERRT("RESB", "RL2, #8", ILLEGAL_BIT_NUMBER, "8", 0xA2A8);
    TEST("RESB", "RL2, R1", 0x2201, 0x0A00);
    UNKN(                   0x2201, 0x4A00);
    UNKN(                   0x2201, 0x0A50);
    UNKN(                   0x2201, 0x0A06);
    ERRT("RESB", "RL2, R8", REGISTER_NOT_ALLOWED, "R8", 0x2208, 0x0A00);
    if (z8001()) {
        TEST("RES",  "@RR2, #3",           0x2323);
        TEST("RES",  "|%120034|, #4",      0x6304, 0x1234);
        TEST("RES",  "%561234, #9",        0x6309, 0xD600, 0x1234);
        TEST("RES",  "|%120034|(R2), #14", 0x632E, 0x1234);
        TEST("RES",  "%561234(R2), #15",   0x632F, 0xD600, 0x1234);
        TEST("RESB", "@RR2, #3",           0x2223);
        TEST("RESB", "|%120034|, #4",      0x6204, 0x1234);
        TEST("RESB", "%561234, #7",        0x6207, 0xD600, 0x1234);
        TEST("RESB", "|%120034|(R2), #0",  0x6220, 0x1234);
        TEST("RESB", "%561234(R2), #7",    0x6227, 0xD600, 0x1234);
    } else {
        TEST("RES",  "@R2, #3",        0x2323);
        TEST("RES",  "%1234, #4",      0x6304, 0x1234);
        TEST("RES",  "%1234(R2), #14", 0x632E, 0x1234);
        TEST("RESB", "@R2, #3",        0x2223);
        TEST("RESB", "%1234, #4",      0x6204, 0x1234);
        TEST("RESB", "%1234(R2), #7",  0x6227, 0x1234);
    }

    // Set Bit
    TEST("SET",  "R2, #1",  0xA521);
    TEST("SET",  "R2, R8",  0x2508, 0x0200);
    UNKN(                   0x2508, 0x7200);
    UNKN(                   0x2508, 0x0280);
    UNKN(                   0x2508, 0x0209);
    TEST("SETB", "RL2, #1",                            0xA4A1);
    ERRT("SETB", "RL2, #15", ILLEGAL_BIT_NUMBER, "15", 0xA4AF);
    TEST("SETB", "RL2, R1", 0x2401, 0x0A00);
    UNKN(                   0x2401, 0xAA00);
    UNKN(                   0x2401, 0x0AB0);
    UNKN(                   0x2401, 0x0A0C);
    ERRT("SETB", "RL2, R8", REGISTER_NOT_ALLOWED, "R8", 0x2408, 0x0A00);
    if (z8001()) {
        TEST("SET",  "@RR2, #3",           0x2523);
        TEST("SET",  "|%120034|, #4",      0x6504, 0x1234);
        TEST("SET",  "%561234, #9",        0x6509, 0xD600, 0x1234);
        TEST("SET",  "|%120034|(R2), #14", 0x652E, 0x1234);
        TEST("SET",  "%561234(R2), #15",   0x652F, 0xD600, 0x1234);
        TEST("SETB", "@RR2, #3",           0x2423);
        TEST("SETB", "|%120034|, #4",      0x6404, 0x1234);
        TEST("SETB", "%561234, #7",        0x6407, 0xD600, 0x1234);
        TEST("SETB", "|%120034|(R2), #0",  0x6420, 0x1234);
        TEST("SETB", "%561234(R2), #7",    0x6427, 0xD600, 0x1234);
    } else {
        TEST("SET",  "@R2, #3",        0x2523);
        TEST("SET",  "%1234, #4",      0x6504, 0x1234);
        TEST("SET",  "%1234(R2), #14", 0x652E, 0x1234);
        TEST("SETB", "@R2, #3",        0x2423);
        TEST("SETB", "%1234, #4",      0x6404, 0x1234);
        TEST("SETB", "%1234(R2), #7",  0x6427, 0x1234);
    }

    // Test and Set
    TEST("TSET",  "R2",  0x8D26);
    TEST("TSETB", "RL1", 0x8C96);
    if (z8001()) {
        TEST("TSET",  "@RR2",          0x0D26);
        TEST("TSET",  "|%120034|",     0x4D06, 0x1234);
        TEST("TSET",  "%561234",       0x4D06, 0xD600, 0x1234);
        TEST("TSET",  "|%120034|(R2)", 0x4D26, 0x1234);
        TEST("TSET",  "%561234(R2)",   0x4D26, 0xD600, 0x1234);
        TEST("TSETB", "@RR2",          0x0C26);
        TEST("TSETB", "|%120034|",     0x4C06, 0x1234);
        TEST("TSETB", "%561234",       0x4C06, 0xD600, 0x1234);
        TEST("TSETB", "|%120034|(R2)", 0x4C26, 0x1234);
        TEST("TSETB", "%561234(R2)",   0x4C26, 0xD600, 0x1234);
    } else {
        TEST("TSET",  "@R2",       0x0D26);
        TEST("TSET",  "%1234",     0x4D06, 0x1234);
        TEST("TSET",  "%1234(R2)", 0x4D26, 0x1234);
        TEST("TSETB", "@R2",       0x0C26);
        TEST("TSETB", "%1234",     0x4C06, 0x1234);
        TEST("TSETB", "%1234(R2)", 0x4C26, 0x1234);
    }

    // Test Conditions Code
    TEST("TCC",  "F, R15",   0xAFF0);
    TEST("TCC",  "LT, R14",  0xAFE1);
    TEST("TCC",  "LE, R13",  0xAFD2);
    TEST("TCC",  "ULE, R12", 0xAFC3);
    TEST("TCC",  "OV, R11",  0xAFB4);
    TEST("TCC",  "MI, R10",  0xAFA5);
    TEST("TCC",  "Z, R9",    0xAF96);
    TEST("TCC",  "C, R8",    0xAF87);
    TEST("TCC",  "R7",       0xAF78);
    TEST("TCC",  "GE, R6",   0xAF69);
    TEST("TCC",  "GT, R5",   0xAF5A);
    TEST("TCC",  "UGT, R4",  0xAF4B);
    TEST("TCC",  "NOV, R3",  0xAF3C);
    TEST("TCC",  "PL, R2",   0xAF2D);
    TEST("TCC",  "NZ, R1",   0xAF1E);
    TEST("TCC",  "NC, R0",   0xAF0F);
    TEST("TCCB", "F, RL7",   0xAEF0);
    TEST("TCCB", "LT, RL6",  0xAEE1);
    TEST("TCCB", "LE, RL5",  0xAED2);
    TEST("TCCB", "ULE, RL4", 0xAEC3);
    TEST("TCCB", "OV, RL3",  0xAEB4);
    TEST("TCCB", "MI, RL2",  0xAEA5);
    TEST("TCCB", "Z, RL1",   0xAE96);
    TEST("TCCB", "C, RL0",   0xAE87);
    TEST("TCCB", "RH7",      0xAE78);
    TEST("TCCB", "GE, RH6",  0xAE69);
    TEST("TCCB", "GT, RH5",  0xAE5A);
    TEST("TCCB", "UGT, RH4", 0xAE4B);
    TEST("TCCB", "NOV, RH3", 0xAE3C);
    TEST("TCCB", "PL, RH2",  0xAE2D);
    TEST("TCCB", "NZ, RH1",  0xAE1E);
    TEST("TCCB", "NC, RH0",  0xAE0F);
}

static void test_rotate() {
    // Rotate Left
    TEST("RL",  "R1, #1",  0xB310);
    TEST("RL",  "R8, #2",  0xB382);
    TEST("RLB", "RH1, #1", 0xB210);
    TEST("RLB", "RL0, #2", 0xB282);

    // Rotate Left through Carry
    TEST("RLC",  "R1, #1",  0xB318);
    TEST("RLC",  "R8, #2",  0xB38A);
    TEST("RLCB", "RH1, #1", 0xB218);
    TEST("RLCB", "RL0, #2", 0xB28A);

    // Rotate Left Digit
    TEST("RLDB", "RH1, RL0", 0xBE81);

    // Rotate Right
    TEST("RR",  "R1, #1",  0xB314);
    TEST("RR",  "R8, #2",  0xB386);
    TEST("RRB", "RH1, #1", 0xB214);
    TEST("RRB", "RL0, #2", 0xB286);

    // Rotate Right through Carry
    TEST("RRC",  "R1, #1",  0xB31C);
    TEST("RRC",  "R8, #2",  0xB38E);
    TEST("RRCB", "RH1, #1", 0xB21C);
    TEST("RRCB", "RL0, #2", 0xB28E);

    // Rotate Right Digit
    TEST("RRDB", "RL7, RH7", 0xBC7F);
}

static void test_shift() {
    // Shift Dynamic Arithmetic
    TEST("SDA",  "R1, R2",   0xB31B, 0x0200);
    NMEM("SDA",  "", "",     0xB31B);
    UNKN(                    0xB31B, 0x1200);
    UNKN(                    0xB31B, 0x0220);
    UNKN(                    0xB31B, 0x0203);
    TEST("SDAB", "RL1, R15", 0xB29B, 0x0F00);
    UNKN(                    0xB29B, 0x4F00);
    UNKN(                    0xB29B, 0x0F50);
    UNKN(                    0xB29B, 0x0F06);
    TEST("SDAL", "RR2, R8",  0xB32F, 0x0800);
    UNKN(                    0xB32F, 0x7800);
    UNKN(                    0xB32F, 0x0880);
    UNKN(                    0xB32F, 0x0809);

    // Shift Dynamic Logical
    TEST("SDL",  "R1, R2",   0xB313, 0x0200);
    UNKN(                    0xB313, 0xA200);
    UNKN(                    0xB313, 0x02B0);
    UNKN(                    0xB313, 0x020C);
    TEST("SDLB", "RL1, R15", 0xB293, 0x0F00);
    UNKN(                    0xB293, 0xDF00);
    UNKN(                    0xB293, 0x0FE0);
    UNKN(                    0xB293, 0x0F0F);
    TEST("SDLL", "RR2, R8",  0xB327, 0x0800);
    UNKN(                    0xB327, 0x1800);
    UNKN(                    0xB327, 0x0820);
    UNKN(                    0xB327, 0x0803);

    // Shift Left Arithmetic
    TEST("SLA", "R2, #16",                          0xB329, 0x0010);
    NMEM("SLA", "R2, #0",                      "0", 0xB329);
    ERRT("SLA", "R2, #17",   ILLEGAL_OPERAND, "17", 0xB329, 0x0011);
    TEST("SLAB", "RH2, #8",                         0xB229, 0x0008);
    ERRT("SLAB", "RH2, #9",  ILLEGAL_OPERAND, "9",  0xB229, 0x0009);
    TEST("SLAL", "RR2, #32",                        0xB32D, 0x0020);
    ERRT("SLAL", "RR2, #33", ILLEGAL_OPERAND, "33", 0xB32D, 0x0021);

    // Shift Right Arithmetic
    TEST("SRA", "R2, #16",                          0xB329, 0xFFF0);
    ERRT("SRA", "R2, #17",   ILLEGAL_OPERAND, "17", 0xB329, 0xFFEF);
    TEST("SRAB", "RH2, #8",                         0xB229, 0x00F8);
    ERRT("SRAB", "RH2, #9",  ILLEGAL_OPERAND, "9",  0xB229, 0x00F7);
    TEST("SRAL", "RR2, #32",                        0xB32D, 0xFFE0);
    ERRT("SRAL", "RR2, #33", ILLEGAL_OPERAND, "33", 0xB32D, 0xFFDF);

    // Shift Left Logical
    TEST("SLL", "R2, #16",                          0xB321, 0x0010);
    ERRT("SLL", "R2, #17",   ILLEGAL_OPERAND, "17", 0xB321, 0x0011);
    TEST("SLLB", "RH2, #8",                         0xB221, 0x0008);
    ERRT("SLLB", "RH2, #9",  ILLEGAL_OPERAND, "9",  0xB221, 0x0009);
    TEST("SLLL", "RR2, #32",                        0xB325, 0x0020);
    ERRT("SLLL", "RR2, #33", ILLEGAL_OPERAND, "33", 0xB325, 0x0021);

    // Shift Right Logical
    TEST("SRL", "R2, #16",                          0xB321, 0xFFF0);
    ERRT("SRL", "R2, #17",   ILLEGAL_OPERAND, "17", 0xB321, 0xFFEF);
    TEST("SRLB", "RH2, #8",                         0xB221, 0x00F8);
    ERRT("SRLB", "RH2, #9",  ILLEGAL_OPERAND, "9",  0xB221, 0x00F7);
    TEST("SRLL", "RR2, #32",                        0xB325, 0xFFE0);
    ERRT("SRLL", "RR2, #33", ILLEGAL_OPERAND, "33", 0xB325, 0xFFDF);
}

static void test_compare_block() {
    // Compare and Decrement
    // @RR0 and @R0 must not be used as source register. The destination and
    // counter register have no restriction. The source, destination, and
    // count registers must be separated and non-overlapping registers
    if (z8001()) {
        TEST("CPD",  "R1, @RR2, R4, F", 0xBB28, 0x0410);
        TEST("CPD",  "R0, @RR2, R4, F", 0xBB28, 0x0400);
        TEST("CPD",  "R1, @RR2, R0, F", 0xBB28, 0x0010);
        NMEM("CPD",  "", "",            0xBB28);
        UNKN(                           0xBB28, 0x1410);
        ERRT("CPD",  "R2, @RR0, R4, F", REGISTER_NOT_ALLOWED, "RR0, R4, F", 0xBB08, 0x0420);
        ERRT("CPD",  "R2, @RR2, R4, F", REGISTERS_OVERLAPPED, "RR2, R4, F", 0xBB28, 0x0420);
        ERRT("CPD",  "R1, @RR2, R1, F", REGISTERS_OVERLAPPED,      "R1, F", 0xBB28, 0x0110);
        ERRT("CPD",  "R1, @RR2, R2, F", REGISTERS_OVERLAPPED,      "R2, F", 0xBB28, 0x0210);
        TEST("CPDB", "RH5, @RR2, R4, LT", 0xBA28, 0x0451);
        TEST("CPDB", "RH0, @RR2, R4, LT", 0xBA28, 0x0401);
        TEST("CPDB", "RH5, @RR2, R0, LT", 0xBA28, 0x0051);
        ERRT("CPDB", "RH5, @RR0, R4, LT", REGISTER_NOT_ALLOWED, "RR0, R4, LT", 0xBA08, 0x0451);
        ERRT("CPDB", "RH3, @RR2, R4, LT", REGISTERS_OVERLAPPED, "RR2, R4, LT", 0xBA28, 0x0431);
        ERRT("CPDB", "RH5, @RR2, R5, LT", REGISTERS_OVERLAPPED,      "R5, LT", 0xBA28, 0x0551);
        ERRT("CPDB", "RH5, @RR2, R3, LT", REGISTERS_OVERLAPPED,      "R3, LT", 0xBA28, 0x0351);
    } else {
        TEST("CPD",  "R3, @R1, R2, F", 0xBB18, 0x0230);
        TEST("CPD",  "R0, @R1, R2, F", 0xBB18, 0x0200);
        TEST("CPD",  "R3, @R1, R0, F", 0xBB18, 0x0030);
        UNKN(                          0xBB18, 0x1230);
        ERRT("CPD",  "R2, @R0, R4, F", REGISTER_NOT_ALLOWED, "R0, R4, F", 0xBB08, 0x0420);
        ERRT("CPD",  "R3, @R3, R2, F", REGISTERS_OVERLAPPED, "R3, R2, F", 0xBB38, 0x0230);
        ERRT("CPD",  "R3, @R1, R3, F", REGISTERS_OVERLAPPED,     "R3, F", 0xBB18, 0x0330);
        ERRT("CPD",  "R3, @R1, R1, F", REGISTERS_OVERLAPPED,     "R1, F", 0xBB18, 0x0130);
        TEST("CPDB", "RL3, @R1, R2, LT", 0xBA18, 0x02B1);
        TEST("CPDB", "RL0, @R1, R2, LT", 0xBA18, 0x0281);
        TEST("CPDB", "RL3, @R1, R0, LT", 0xBA18, 0x00B1);
        ERRT("CPDB", "RL0, @R0, R4, LT", REGISTER_NOT_ALLOWED, "R0, R4, LT", 0xBA08, 0x0481);
        ERRT("CPDB", "RL3, @R3, R2, LT", REGISTERS_OVERLAPPED, "R3, R2, LT", 0xBA38, 0x02B1);
        ERRT("CPDB", "RL3, @R1, R3, LT", REGISTERS_OVERLAPPED,     "R3, LT", 0xBA18, 0x03B1);
        ERRT("CPDB", "RL3, @R1, R1, LT", REGISTERS_OVERLAPPED,     "R1, LT", 0xBA18, 0x01B1);
    }

    // Compare, Decrement, and Repeat
    if (z8001()) {
        TEST("CPDR",  "R1, @RR4, R3, LE", 0xBB4C, 0x0312);
        TEST("CPDR",  "R0, @RR4, R3, LE", 0xBB4C, 0x0302);
        TEST("CPDR",  "R1, @RR4, R0, LE", 0xBB4C, 0x0012);
        UNKN(                             0xBB4C, 0x8312);
        ERRT("CPDR",  "R1, @RR0, R4, LE", REGISTER_NOT_ALLOWED, "RR0, R4, LE", 0xBB0C, 0x0412);
        ERRT("CPDR",  "R5, @RR4, R3, LE", REGISTERS_OVERLAPPED, "RR4, R3, LE", 0xBB4C, 0x0352);
        ERRT("CPDR",  "R1, @RR4, R1, LE", REGISTERS_OVERLAPPED,      "R1, LE", 0xBB4C, 0x0112);
        ERRT("CPDR",  "R1, @RR4, R4, LE", REGISTERS_OVERLAPPED,      "R4, LE", 0xBB4C, 0x0412);
        TEST("CPDRB", "RL1, @RR4, R3, ULE", 0xBA4C, 0x0393);
        TEST("CPDRB", "RL0, @RR4, R3, ULE", 0xBA4C, 0x0383);
        TEST("CPDRB", "RL1, @RR4, R0, ULE", 0xBA4C, 0x0093);
        UNKN(                               0xBA4C, 0x3393);
        ERRT("CPDRB", "RL1, @RR0, R4, ULE", REGISTER_NOT_ALLOWED, "RR0, R4, ULE", 0xBA0C, 0x0493);
        ERRT("CPDRB", "RL5, @RR4, R3, ULE", REGISTERS_OVERLAPPED, "RR4, R3, ULE", 0xBA4C, 0x03D3);
        ERRT("CPDRB", "RL1, @RR4, R1, ULE", REGISTERS_OVERLAPPED,      "R1, ULE", 0xBA4C, 0x0193);
        ERRT("CPDRB", "RL1, @RR4, R4, ULE", REGISTERS_OVERLAPPED,      "R4, ULE", 0xBA4C, 0x0493);
    } else {
        TEST("CPDR",  "R3, @R4, R5, LE", 0xBB4C, 0x0532);
        TEST("CPDR",  "R3, @R4, R5, LE", 0xBB4C, 0x0532);
        TEST("CPDR",  "R0, @R4, R5, LE", 0xBB4C, 0x0502);
        TEST("CPDR",  "R3, @R4, R0, LE", 0xBB4C, 0x0032);
        UNKN(                            0xBB4C, 0xF532);
        ERRT("CPDR",  "R1, @R0, R4, LE", REGISTER_NOT_ALLOWED, "R0, R4, LE", 0xBB0C, 0x0412);
        ERRT("CPDR",  "R3, @R3, R5, LE", REGISTERS_OVERLAPPED, "R3, R5, LE", 0xBB3C, 0x0532);
        ERRT("CPDR",  "R3, @R4, R3, LE", REGISTERS_OVERLAPPED,     "R3, LE", 0xBB4C, 0x0332);
        ERRT("CPDR",  "R3, @R4, R4, LE", REGISTERS_OVERLAPPED,     "R4, LE", 0xBB4C, 0x0432);
        TEST("CPDRB", "RL3, @R4, R5, ULE", 0xBA4C, 0x05B3);
        TEST("CPDRB", "RL0, @R4, R5, ULE", 0xBA4C, 0x0583);
        TEST("CPDRB", "RL3, @R4, R0, ULE", 0xBA4C, 0x00B3);
        UNKN(                              0xBA4C, 0x25B3);
        ERRT("CPDRB", "RL1, @R0, R4, ULE", REGISTER_NOT_ALLOWED, "R0, R4, ULE", 0xBA0C, 0x0493);
        ERRT("CPDRB", "RL3, @R3, R5, ULE", REGISTERS_OVERLAPPED, "R3, R5, ULE", 0xBA3C, 0x05B3);
        ERRT("CPDRB", "RL3, @R4, R3, ULE", REGISTERS_OVERLAPPED,     "R3, ULE", 0xBA4C, 0x03B3);
        ERRT("CPDRB", "RL3, @R4, R4, ULE", REGISTERS_OVERLAPPED,     "R4, ULE", 0xBA4C, 0x04B3);
    }

    // Compare and Increment
    if (z8001()) {
        TEST("CPI",  "R2, @RR6, R4, OV", 0xBB60, 0x0424);
        TEST("CPI",  "R0, @RR6, R4, OV", 0xBB60, 0x0404);
        TEST("CPI",  "R2, @RR6, R0, OV", 0xBB60, 0x0024);
        UNKN(                            0xBB60, 0x4424);
        ERRT("CPI",  "R1, @RR0, R4, OV", REGISTER_NOT_ALLOWED, "RR0, R4, OV", 0xBB00, 0x0414);
        ERRT("CPI",  "R7, @RR6, R4, OV", REGISTERS_OVERLAPPED, "RR6, R4, OV", 0xBB60, 0x0474);
        ERRT("CPI",  "R2, @RR6, R2, OV", REGISTERS_OVERLAPPED,      "R2, OV", 0xBB60, 0x0224);
        ERRT("CPI",  "R2, @RR6, R6, OV", REGISTERS_OVERLAPPED,      "R6, OV", 0xBB60, 0x0624);
        TEST("CPIB", "RL2, @RR6, R4", 0xBA60, 0x04A8);
        TEST("CPIB", "RL0, @RR6, R4", 0xBA60, 0x0488);
        TEST("CPIB", "RL2, @RR6, R0", 0xBA60, 0x00A8);
        UNKN(                         0xBA60, 0xB4A8);
        ERRT("CPIB", "RL1, @RR0, R4", REGISTER_NOT_ALLOWED, "RR0, R4", 0xBA00, 0x0498);
        ERRT("CPIB", "RL7, @RR6, R4", REGISTERS_OVERLAPPED, "RR6, R4", 0xBA60, 0x04F8);
        ERRT("CPIB", "RL2, @RR6, R2", REGISTERS_OVERLAPPED,      "R2", 0xBA60, 0x02A8);
        ERRT("CPIB", "RL2, @RR6, R7", REGISTERS_OVERLAPPED,      "R7", 0xBA60, 0x07A8);
    } else {
        TEST("CPI",  "R6, @R7, R8, OV", 0xBB70, 0x0864);
        TEST("CPI",  "R0, @R7, R8, OV", 0xBB70, 0x0804);
        TEST("CPI",  "R6, @R7, R0, OV", 0xBB70, 0x0064);
        UNKN(                           0xBB70, 0x8864);
        ERRT("CPI",  "R2, @R0, R4, OV", REGISTER_NOT_ALLOWED, "R0, R4, OV", 0xBB00, 0x0424);
        ERRT("CPI",  "R6, @R6, R8, OV", REGISTERS_OVERLAPPED, "R6, R8, OV", 0xBB60, 0x0864);
        ERRT("CPI",  "R6, @R7, R6, OV", REGISTERS_OVERLAPPED,     "R6, OV", 0xBB70, 0x0664);
        ERRT("CPI",  "R6, @R7, R7, OV", REGISTERS_OVERLAPPED,     "R7, OV", 0xBB70, 0x0764);
        TEST("CPIB", "RL6, @R7, R8", 0xBA70, 0x08E8);
        TEST("CPIB", "RL0, @R7, R8", 0xBA70, 0x0888);
        TEST("CPIB", "RL6, @R7, R0", 0xBA70, 0x00E8);
        UNKN(                        0xBA70, 0x28E8);
        ERRT("CPIB", "RL0, @R0, R4", REGISTER_NOT_ALLOWED, "R0, R4", 0xBA00, 0x0488);
        ERRT("CPIB", "RL6, @R6, R8", REGISTERS_OVERLAPPED, "R6, R8", 0xBA60, 0x08E8);
        ERRT("CPIB", "RL6, @R7, R6", REGISTERS_OVERLAPPED,     "R6", 0xBA70, 0x06E8);
        ERRT("CPIB", "RL6, @R7, R7", REGISTERS_OVERLAPPED,     "R7", 0xBA70, 0x07E8);
    }

    // Compare, Increment, and Repeat
    if (z8001()) {
        TEST("CPIR",  "R3, @RR8, R5, Z", 0xBB84, 0x0536);
        TEST("CPIR",  "R0, @RR8, R5, Z", 0xBB84, 0x0506);
        TEST("CPIR",  "R3, @RR8, R0, Z", 0xBB84, 0x0036);
        UNKN(                            0xBB84, 0x5536);
        ERRT("CPIR",  "R1, @RR0, R4, Z", REGISTER_NOT_ALLOWED, "RR0, R4, Z", 0xBB04, 0x0416);
        ERRT("CPIR",  "R8, @RR8, R5, Z", REGISTERS_OVERLAPPED, "RR8, R5, Z", 0xBB84, 0x0586);
        ERRT("CPIR",  "R3, @RR8, R3, Z", REGISTERS_OVERLAPPED,      "R3, Z", 0xBB84, 0x0336);
        ERRT("CPIR",  "R3, @RR8, R9, Z", REGISTERS_OVERLAPPED,      "R9, Z", 0xBB84, 0x0936);
        TEST("CPIRB", "RL3, @RR8, R5, C", 0xBA84, 0x05B7);
        TEST("CPIRB", "RL0, @RR8, R5, C", 0xBA84, 0x0587);
        TEST("CPIRB", "RL3, @RR8, R0, C", 0xBA84, 0x00B7);
        UNKN(                             0xBA84, 0x75B7);
        ERRT("CPIRB", "RL1, @RR0, R4, C", REGISTER_NOT_ALLOWED, "RR0, R4, C", 0xBA04, 0x0497);
        ERRT("CPIRB", "RL2, @RR2, R5, C", REGISTERS_OVERLAPPED, "RR2, R5, C", 0xBA24, 0x05A7);
        ERRT("CPIRB", "RL3, @RR8, R3, C", REGISTERS_OVERLAPPED,      "R3, C", 0xBA84, 0x03B7);
        ERRT("CPIRB", "RL3, @RR8, R8, C", REGISTERS_OVERLAPPED,      "R8, C", 0xBA84, 0x08B7);
    } else {
        TEST("CPIR",  "R9, @R10, R11, Z", 0xBBA4, 0x0B96);
        TEST("CPIR",  "R0, @R10, R11, Z", 0xBBA4, 0x0B06);
        TEST("CPIR",  "R9, @R10, R0, Z",  0xBBA4, 0x0096);
        UNKN(                             0xBBA4, 0x1B96);
        ERRT("CPIR",  "R1, @R0, R4, LE",  REGISTER_NOT_ALLOWED, "R0, R4, LE", 0xBB04, 0x0412);
        ERRT("CPIR",  "R9, @R9, R11, Z",  REGISTERS_OVERLAPPED, "R9, R11, Z", 0xBB94, 0x0B96);
        ERRT("CPIR",  "R9, @R10, R9, Z",  REGISTERS_OVERLAPPED,      "R9, Z", 0xBBA4, 0x0996);
        ERRT("CPIR",  "R9, @R10, R10, Z", REGISTERS_OVERLAPPED,     "R10, Z", 0xBBA4, 0x0A96);
        TEST("CPIRB", "RH1, @R10, R11, C", 0xBAA4, 0x0B17);
        TEST("CPIRB", "RH0, @R10, R11, C", 0xBAA4, 0x0B07);
        TEST("CPIRB", "RH1, @R10, R0, C",  0xBAA4, 0x0017);
        UNKN(                              0xBAA4, 0x8B17);
        ERRT("CPIRB", "RL1, @R0, R4, ULE", REGISTER_NOT_ALLOWED, "R0, R4, ULE", 0xBA04, 0x0493);
        ERRT("CPIRB", "RH2, @R2, R11, C",  REGISTERS_OVERLAPPED, "R2, R11, C", 0xBA24, 0x0B27);
        ERRT("CPIRB", "RH1, @R2, R1, C",   REGISTERS_OVERLAPPED,      "R1, C", 0xBA24, 0x0117);
        ERRT("CPIRB", "RH1, @R2, R2, C",   REGISTERS_OVERLAPPED,      "R2, C", 0xBA24, 0x0217);
    }
}

static void test_block_transfer() {
    // Load and Decrement
    if (z8001()) {
        TEST("LDD",  "@RR2, @RR4, R6", 0xBB49, 0x0628);
        TEST("LDD",  "@RR2, @RR4, R0", 0xBB49, 0x0028);
        UNKN(                          0xBB49, 0x1628);
        UNKN(                          0xBB49, 0x0629);
        ERRT("LDD",  "@RR0, @RR4, R6", REGISTER_NOT_ALLOWED, "RR0, @RR4, R6", 0xBB49, 0x0608);
        ERRT("LDD",  "@RR2, @RR0, R6", REGISTER_NOT_ALLOWED,       "RR0, R6", 0xBB09, 0x0628);
        ERRT("LDD",  "@RR2, @RR2, R6", REGISTERS_OVERLAPPED,       "RR2, R6", 0xBB29, 0x0628);
        ERRT("LDD",  "@RR2, @RR4, R2", REGISTERS_OVERLAPPED,            "R2", 0xBB49, 0x0228);
        ERRT("LDD",  "@RR2, @RR4, R4", REGISTERS_OVERLAPPED,            "R4", 0xBB49, 0x0428);
        TEST("LDDB", "@RR2, @RR4, R6", 0xBA49, 0x0628);
        TEST("LDDB", "@RR2, @RR4, R0", 0xBA49, 0x0028);
        UNKN(                          0xBA49, 0x2628);
        UNKN(                          0xBA49, 0x062A);
        ERRT("LDDB", "@RR0, @RR4, R6", REGISTER_NOT_ALLOWED, "RR0, @RR4, R6", 0xBA49, 0x0608);
        ERRT("LDDB", "@RR2, @RR0, R6", REGISTER_NOT_ALLOWED,       "RR0, R6", 0xBA09, 0x0628);
        ERRT("LDDB", "@RR2, @RR2, R6", REGISTERS_OVERLAPPED,       "RR2, R6", 0xBA29, 0x0628);
        ERRT("LDDB", "@RR2, @RR4, R2", REGISTERS_OVERLAPPED,            "R2", 0xBA49, 0x0228);
        ERRT("LDDB", "@RR2, @RR4, R4", REGISTERS_OVERLAPPED,            "R4", 0xBA49, 0x0428);
    } else {
        TEST("LDD",  "@R1, @R2, R3", 0xBB29, 0x0318);
        TEST("LDD",  "@R1, @R2, R0", 0xBB29, 0x0018);
        NMEM("LDDR", "",         "", 0xBB29);
        UNKN(                        0xBB29, 0x3318);
        UNKN(                        0xBB29, 0x031B);
        ERRT("LDD",  "@R0, @R2, R3", REGISTER_NOT_ALLOWED, "R0, @R2, R3", 0xBB29, 0x0308);
        ERRT("LDD",  "@R1, @R0, R3", REGISTER_NOT_ALLOWED,      "R0, R3", 0xBB09, 0x0318);
        ERRT("LDD",  "@R1, @R1, R3", REGISTERS_OVERLAPPED,      "R1, R3", 0xBB19, 0x0318);
        ERRT("LDD",  "@R1, @R2, R1", REGISTERS_OVERLAPPED,          "R1", 0xBB29, 0x0118);
        ERRT("LDD",  "@R1, @R2, R2", REGISTERS_OVERLAPPED,          "R2", 0xBB29, 0x0218);
        TEST("LDDB", "@R1, @R2, R3", 0xBA29, 0x0318);
        TEST("LDDB", "@R1, @R2, R0", 0xBA29, 0x0018);
        UNKN(                        0xBA29, 0x4318);
        UNKN(                        0xBA29, 0x031C);
        ERRT("LDDB", "@R0, @R2, R3", REGISTER_NOT_ALLOWED, "R0, @R2, R3", 0xBA29, 0x0308);
        ERRT("LDDB", "@R1, @R0, R3", REGISTER_NOT_ALLOWED,      "R0, R3", 0xBA09, 0x0318);
        ERRT("LDDB", "@R1, @R1, R3", REGISTERS_OVERLAPPED,      "R1, R3", 0xBA19, 0x0318);
        ERRT("LDDB", "@R1, @R2, R1", REGISTERS_OVERLAPPED,          "R1", 0xBA29, 0x0118);
        ERRT("LDDB", "@R1, @R2, R2", REGISTERS_OVERLAPPED,          "R2", 0xBA29, 0x0218);
    }

    // Load, Decrement, and Repeat
    if (z8001()) {
        TEST("LDDR",  "@RR4, @RR6, R8", 0xBB69, 0x0840);
        TEST("LDDR",  "@RR4, @RR6, R0", 0xBB69, 0x0040);
        UNKN(                           0xBB69, 0x5840);
        UNKN(                           0xBB69, 0x084D);
        ERRT("LDDR",  "@RR0, @RR6, R8", REGISTER_NOT_ALLOWED, "RR0, @RR6, R8", 0xBB69, 0x0800);
        ERRT("LDDR",  "@RR4, @RR0, R8", REGISTER_NOT_ALLOWED,       "RR0, R8", 0xBB09, 0x0840);
        ERRT("LDDR",  "@RR4, @RR4, R8", REGISTERS_OVERLAPPED,       "RR4, R8", 0xBB49, 0x0840);
        ERRT("LDDR",  "@RR4, @RR6, R4", REGISTERS_OVERLAPPED,            "R4", 0xBB69, 0x0440);
        ERRT("LDDR",  "@RR4, @RR6, R6", REGISTERS_OVERLAPPED,            "R6", 0xBB69, 0x0640);
        TEST("LDDRB", "@RR4, @RR6, R8", 0xBA69, 0x0840);
        TEST("LDDRB", "@RR4, @RR6, R0", 0xBA69, 0x0040);
        UNKN(                           0xBA69, 0x6840);
        UNKN(                           0xBA69, 0x084E);
        ERRT("LDDRB", "@RR0, @RR6, R8", REGISTER_NOT_ALLOWED, "RR0, @RR6, R8", 0xBA69, 0x0800);
        ERRT("LDDRB", "@RR4, @RR0, R8", REGISTER_NOT_ALLOWED,       "RR0, R8", 0xBA09, 0x0840);
        ERRT("LDDRB", "@RR4, @RR4, R8", REGISTERS_OVERLAPPED,       "RR4, R8", 0xBA49, 0x0840);
        ERRT("LDDRB", "@RR4, @RR6, R5", REGISTERS_OVERLAPPED,       "R5", 0xBA69, 0x0540);
        ERRT("LDDRB", "@RR4, @RR6, R7", REGISTERS_OVERLAPPED,       "R7", 0xBA69, 0x0740);
    } else {
        TEST("LDDR",  "@R4, @R5, R6", 0xBB59, 0x0640);
        TEST("LDDR",  "@R4, @R5, R0", 0xBB59, 0x0040);
        UNKN(                         0xBB59, 0x7640);
        UNKN(                         0xBB59, 0x064F);
        ERRT("LDDR",  "@R0, @R5, R6", REGISTER_NOT_ALLOWED, "R0, @R5, R6", 0xBB59, 0x0600);
        ERRT("LDDR",  "@R4, @R0, R6", REGISTER_NOT_ALLOWED,      "R0, R6", 0xBB09, 0x0640);
        ERRT("LDDR",  "@R4, @R4, R6", REGISTERS_OVERLAPPED,      "R4, R6", 0xBB49, 0x0640);
        ERRT("LDDR",  "@R4, @R5, R4", REGISTERS_OVERLAPPED,          "R4", 0xBB59, 0x0440);
        ERRT("LDDR",  "@R4, @R5, R5", REGISTERS_OVERLAPPED,          "R5", 0xBB59, 0x0540);
        TEST("LDDRB", "@R4, @R5, R6", 0xBA59, 0x0640);
        TEST("LDDRB", "@R4, @R5, R0", 0xBA59, 0x0040);
        UNKN(                         0xBA59, 0x8640);
        UNKN(                         0xBA59, 0x0641);
        ERRT("LDDRB", "@R0, @R5, R6", REGISTER_NOT_ALLOWED, "R0, @R5, R6", 0xBA59, 0x0600);
        ERRT("LDDRB", "@R4, @R0, R6", REGISTER_NOT_ALLOWED,      "R0, R6", 0xBA09, 0x0640);
        ERRT("LDDRB", "@R4, @R4, R6", REGISTERS_OVERLAPPED,      "R4, R6", 0xBA49, 0x0640);
        ERRT("LDDRB", "@R4, @R5, R4", REGISTERS_OVERLAPPED,          "R4", 0xBA59, 0x0440);
        ERRT("LDDRB", "@R4, @R5, R5", REGISTERS_OVERLAPPED,          "R5", 0xBA59, 0x0540);
    }

    // Load and Increment
    if (z8001()) {
        TEST("LDI",  "@RR6, @RR8, R10", 0xBB81, 0x0A68);
        TEST("LDI",  "@RR6, @RR8, R0",  0xBB81, 0x0068);
        UNKN(                           0xBB81, 0x9A68);
        UNKN(                           0xBB81, 0x0A62);
        ERRT("LDI",  "@RR0, @RR8, R10", REGISTER_NOT_ALLOWED, "RR0, @RR8, R10", 0xBB81, 0x0A08);
        ERRT("LDI",  "@RR6, @RR0, R10", REGISTER_NOT_ALLOWED,       "RR0, R10", 0xBB01, 0x0A68);
        ERRT("LDI",  "@RR6, @RR6, R10", REGISTERS_OVERLAPPED,       "RR6, R10", 0xBB61, 0x0A68);
        ERRT("LDI",  "@RR6, @RR8, R7",  REGISTERS_OVERLAPPED,             "R7", 0xBB81, 0x0768);
        ERRT("LDI",  "@RR6, @RR8, R9",  REGISTERS_OVERLAPPED,             "R9", 0xBB81, 0x0968);
        TEST("LDIB", "@RR6, @RR8, R10", 0xBA81, 0x0A68);
        TEST("LDIB", "@RR6, @RR8, R0",  0xBA81, 0x0068);
        UNKN(                           0xBA81, 0xAA68);
        UNKN(                           0xBA81, 0x0A63);
        ERRT("LDIB", "@RR0, @RR8, R10", REGISTER_NOT_ALLOWED, "RR0, @RR8, R10", 0xBA81, 0x0A08);
        ERRT("LDIB", "@RR6, @RR0, R10", REGISTER_NOT_ALLOWED,       "RR0, R10", 0xBA01, 0x0A68);
        ERRT("LDIB", "@RR6, @RR6, R10", REGISTERS_OVERLAPPED,       "RR6, R10", 0xBA61, 0x0A68);
        ERRT("LDIB", "@RR6, @RR8, R6",  REGISTERS_OVERLAPPED,             "R6", 0xBA81, 0x0668);
        ERRT("LDIB", "@RR6, @RR8, R8",  REGISTERS_OVERLAPPED,             "R8", 0xBA81, 0x0868);
    } else {
        TEST("LDI",  "@R7, @R8, R9", 0xBB81, 0x0978);
        TEST("LDI",  "@R7, @R8, R0", 0xBB81, 0x0078);
        NMEM("LDIR", "",         "", 0xBB81);
        UNKN(                        0xBB81, 0xB978);
        UNKN(                        0xBB81, 0x0974);
        ERRT("LDI",  "@R0, @R8, R9", REGISTER_NOT_ALLOWED, "R0, @R8, R9", 0xBB81, 0x0908);
        ERRT("LDI",  "@R7, @R0, R9", REGISTER_NOT_ALLOWED,      "R0, R9", 0xBB01, 0x0978);
        ERRT("LDI",  "@R7, @R7, R9", REGISTERS_OVERLAPPED,      "R7, R9", 0xBB71, 0x0978);
        ERRT("LDI",  "@R7, @R8, R7", REGISTERS_OVERLAPPED,          "R7", 0xBB81, 0x0778);
        ERRT("LDI",  "@R7, @R8, R8", REGISTERS_OVERLAPPED,          "R8", 0xBB81, 0x0878);
        TEST("LDIB", "@R7, @R8, R9", 0xBA81, 0x0978);
        TEST("LDIB", "@R7, @R8, R0", 0xBA81, 0x0078);
        UNKN(                        0xBA81, 0xC978);
        UNKN(                        0xBA81, 0x0975);
        ERRT("LDIB", "@R0, @R8, R9", REGISTER_NOT_ALLOWED, "R0, @R8, R9", 0xBA81, 0x0908);
        ERRT("LDIB", "@R7, @R0, R9", REGISTER_NOT_ALLOWED,      "R0, R9", 0xBA01, 0x0978);
        ERRT("LDIB", "@R7, @R7, R9", REGISTERS_OVERLAPPED,      "R7, R9", 0xBA71, 0x0978);
        ERRT("LDIB", "@R7, @R8, R7", REGISTERS_OVERLAPPED,          "R7", 0xBA81, 0x0778);
        ERRT("LDIB", "@R7, @R8, R8", REGISTERS_OVERLAPPED,          "R8", 0xBA81, 0x0878);
    }

    // Load, Increment, and Repeat
    if (z8001()) {
        TEST("LDIR",  "@RR8, @RR10, R12", 0xBBA1, 0x0C80);
        TEST("LDIR",  "@RR8, @RR10, R0",  0xBBA1, 0x0080);
        UNKN(                             0xBBA1, 0xDC80);
        UNKN(                             0xBBA1, 0x0C86);
        ERRT("LDIR",  "@RR0, @RR10, R12", REGISTER_NOT_ALLOWED, "RR0, @RR10, R12", 0xBBA1, 0x0C00);
        ERRT("LDIR",  "@RR8, @RR0, R12",  REGISTER_NOT_ALLOWED,        "RR0, R12", 0xBB01, 0x0C80);
        ERRT("LDIR",  "@RR8, @RR8, R12",  REGISTERS_OVERLAPPED,        "RR8, R12", 0xBB81, 0x0C80);
        ERRT("LDIR",  "@RR8, @RR10, R9",  REGISTERS_OVERLAPPED,              "R9", 0xBBA1, 0x0980);
        ERRT("LDIR",  "@RR8, @RR10, R11", REGISTERS_OVERLAPPED,             "R11", 0xBBA1, 0x0B80);
        TEST("LDIRB", "@RR8, @RR10, R12", 0xBAA1, 0x0C80);
        TEST("LDIRB", "@RR8, @RR10, R0",  0xBAA1, 0x0080);
        UNKN(                             0xBAA1, 0xEC80);
        UNKN(                             0xBAA1, 0x0C87);
        ERRT("LDIRB", "@RR0, @RR10, R12", REGISTER_NOT_ALLOWED, "RR0, @RR10, R12", 0xBAA1, 0x0C00);
        ERRT("LDIRB", "@RR8, @RR0, R12",  REGISTER_NOT_ALLOWED,        "RR0, R12", 0xBA01, 0x0C80);
        ERRT("LDIRB", "@RR8, @RR8, R12",  REGISTERS_OVERLAPPED,        "RR8, R12", 0xBA81, 0x0C80);
        ERRT("LDIRB", "@RR8, @RR10, R8",  REGISTERS_OVERLAPPED,              "R8", 0xBAA1, 0x0880);
        ERRT("LDIRB", "@RR8, @RR10, R10", REGISTERS_OVERLAPPED,             "R10", 0xBAA1, 0x0A80);
    } else {
        TEST("LDIR",  "@R10, @R11, R12", 0xBBB1, 0x0CA0);
        TEST("LDIR",  "@R10, @R11, R0",  0xBBB1, 0x00A0);
        UNKN(                            0xBBB1, 0xFCA0);
        UNKN(                            0xBBB1, 0x0CA9);
        ERRT("LDIR",  "@R0, @R11, R12",  REGISTER_NOT_ALLOWED, "R0, @R11, R12", 0xBBB1, 0x0C00);
        ERRT("LDIR",  "@R10, @R0, R12",  REGISTER_NOT_ALLOWED,       "R0, R12", 0xBB01, 0x0CA0);
        ERRT("LDIR",  "@R10, @R10, R12", REGISTERS_OVERLAPPED,      "R10, R12", 0xBBA1, 0x0CA0);
        ERRT("LDIR",  "@R10, @R11, R10", REGISTERS_OVERLAPPED,           "R10", 0xBBB1, 0x0AA0);
        ERRT("LDIR",  "@R10, @R11, R11", REGISTERS_OVERLAPPED,          "R11", 0xBBB1, 0x0BA0);
        TEST("LDIRB", "@R10, @R11, R12", 0xBAB1, 0x0CA0);
        TEST("LDIRB", "@R10, @R11, R0",  0xBAB1, 0x00A0);
        UNKN(                            0xBAB1, 0x1CA0);
        UNKN(                            0xBAB1, 0x0CAA);
        ERRT("LDIRB", "@R0, @R11, R12",  REGISTER_NOT_ALLOWED, "R0, @R11, R12", 0xBAB1, 0x0C00);
        ERRT("LDIRB", "@R10, @R0, R12",  REGISTER_NOT_ALLOWED,       "R0, R12", 0xBA01, 0x0CA0);
        ERRT("LDIRB", "@R10, @R10, R12", REGISTERS_OVERLAPPED,      "R10, R12", 0xBAA1, 0x0CA0);
        ERRT("LDIRB", "@R10, @R11, R10", REGISTERS_OVERLAPPED,           "R10", 0xBAB1, 0x0AA0);
        ERRT("LDIRB", "@R10, @R11, R11", REGISTERS_OVERLAPPED,           "R11", 0xBAB1, 0x0BA0);
    }
}

static void test_compare_string() {
    // Compare String and Decrement
    // @RR0 and @R0 mus not be used as source and destination registers. The
    // counter register has no restriction. The source, destination, and
    // counter registers must be separate and non-overlapping registers.
    if (z8001()) {
        TEST("CPSD",  "@RR4, @RR2, R6", 0xBB2A, 0x0648);
        TEST("CPSD",  "@RR4, @RR2, R0", 0xBB2A, 0x0048);
        UNKN(                           0xBB2A, 0x1648);
        ERRT("CPSD",  "@RR0, @RR2, R0", REGISTER_NOT_ALLOWED, "RR0, @RR2, R0", 0xBB2A, 0x0008);
        ERRT("CPSD",  "@RR4, @RR0, R6", REGISTER_NOT_ALLOWED,       "RR0, R6", 0xBB0A, 0x0648);
        TEST("CPSDB", "@RR4, @RR2, R6, GE", 0xBA2A, 0x0649);
        TEST("CPSDB", "@RR4, @RR2, R0, GE", 0xBA2A, 0x0049);
        UNKN(                               0xBA2A, 0x2649);
        ERRT("CPSDB", "@RR0, @RR2, R4, GE", REGISTER_NOT_ALLOWED, "RR0, @RR2, R4, GE", 0xBA2A, 0x0409);
        ERRT("CPSDB", "@RR2, @RR0, R4, GE", REGISTER_NOT_ALLOWED,       "RR0, R4, GE", 0xBA0A, 0x0429);
    } else {
        TEST("CPSD",  "@R3, @R1, R2", 0xBB1A, 0x0238);
        TEST("CPSD",  "@R3, @R1, R0", 0xBB1A, 0x0038);
        UNKN(                         0xBB1A, 0x3238);
        ERRT("CPSD",  "@R0, @R3, R2", REGISTER_NOT_ALLOWED, "R0, @R3, R2", 0xBB3A, 0x0208);
        ERRT("CPSD",  "@R3, @R0, R2", REGISTER_NOT_ALLOWED,      "R0, R2", 0xBB0A, 0x0238);
        TEST("CPSDB", "@R3, @R1, R2, GE", 0xBA1A, 0x0239);
        TEST("CPSDB", "@R3, @R1, R0, GE", 0xBA1A, 0x0039);
        UNKN(                             0xBA1A, 0x4239);
        ERRT("CPSDB", "@R0, @R3, R2, GE", REGISTER_NOT_ALLOWED, "R0, @R3, R2, GE", 0xBA3A, 0x0209);
        ERRT("CPSDB", "@R3, @R0, R2, GE", REGISTER_NOT_ALLOWED,      "R0, R2, GE", 0xBA0A, 0x0239);
    }

    // Compare String, Decrement, and Repeat
    if (z8001()) {
        TEST("CPSDR",  "@RR2, @RR4, R6, GT", 0xBB4E, 0x062A);
        TEST("CPSDR",  "@RR2, @RR4, R0, GT", 0xBB4E, 0x002A);
        UNKN(                                0xBB4E, 0x562A);
        ERRT("CPSDR",  "@RR0, @RR4, R5, GT", REGISTER_NOT_ALLOWED, "RR0, @RR4, R5, GT", 0xBB4E, 0x050A);
        ERRT("CPSDR",  "@RR2, @RR0, R5, GT", REGISTER_NOT_ALLOWED,       "RR0, R5, GT", 0xBB0E, 0x052A);
        TEST("CPSDRB", "@RR2, @RR4, R6, UGT", 0xBA4E, 0x062B);
        TEST("CPSDRB", "@RR2, @RR4, R0, UGT", 0xBA4E, 0x002B);
        UNKN(                                 0xBA4E, 0x662B);
        ERRT("CPSDRB", "@RR0, @RR4, R5, UGT", REGISTER_NOT_ALLOWED, "RR0, @RR4, R5, UGT", 0xBA4E, 0x050B);
        ERRT("CPSDRB", "@RR2, @RR0, R5, UGT", REGISTER_NOT_ALLOWED,       "RR0, R5, UGT", 0xBA0E, 0x052B);
    } else {
        TEST("CPSDR",  "@R3, @R4, R5, GT", 0xBB4E, 0x053A);
        TEST("CPSDR",  "@R3, @R4, R0, GT", 0xBB4E, 0x003A);
        UNKN(                              0xBB4E, 0x753A);
        ERRT("CPSDR",  "@R0, @R4, R5, GT", REGISTER_NOT_ALLOWED, "R0, @R4, R5, GT", 0xBB4E, 0x050A);
        ERRT("CPSDR",  "@R3, @R0, R5, GT", REGISTER_NOT_ALLOWED,      "R0, R5, GT", 0xBB0E, 0x053A);
        TEST("CPSDRB", "@R3, @R4, R2, UGT", 0xBA4E, 0x023B);
        TEST("CPSDRB", "@R3, @R4, R0, UGT", 0xBA4E, 0x003B);
        UNKN(                               0xBA4E, 0x823B);
        ERRT("CPSDRB", "@R0, @R4, R6, UGT", REGISTER_NOT_ALLOWED, "R0, @R4, R6, UGT", 0xBA4E, 0x060B);
        ERRT("CPSDRB", "@R3, @R0, R6, UGT", REGISTER_NOT_ALLOWED,      "R0, R6, UGT", 0xBA0E, 0x063B);
    }

    // Compare String and Increment
    if (z8001()) {
        TEST("CPSI",  "@RR4, @RR6, R2, NOV", 0xBB62, 0x024C);
        TEST("CPSI",  "@RR4, @RR6, R0, NOV", 0xBB62, 0x004C);
        UNKN(                                0xBB62, 0x924C);
        ERRT("CPSI",  "@RR0, @RR6, R5, NOV", REGISTER_NOT_ALLOWED, "RR0, @RR6, R5, NOV", 0xBB62, 0x050C);
        ERRT("CPSI",  "@RR4, @RR0, R5, NOV", REGISTER_NOT_ALLOWED,       "RR0, R5, NOV", 0xBB02, 0x054C);
        TEST("CPSIB", "@RR4, @RR6, R2, PL", 0xBA62, 0x024D);
        TEST("CPSIB", "@RR4, @RR6, R0, PL", 0xBA62, 0x004D);
        UNKN(                               0xBA62, 0xA24D);
        ERRT("CPSIB", "@RR0, @RR6, R5, PL", REGISTER_NOT_ALLOWED, "RR0, @RR6, R5, PL", 0xBA62, 0x050D);
        ERRT("CPSIB", "@RR4, @RR0, R5, PL", REGISTER_NOT_ALLOWED,       "RR0, R5, PL", 0xBA02, 0x054D);
    } else {
        TEST("CPSI",  "@R6, @R7, R8, NOV", 0xBB72, 0x086C);
        TEST("CPSI",  "@R6, @R7, R0, NOV", 0xBB72, 0x006C);
        UNKN(                              0xBB72, 0xB86C);
        ERRT("CPSI",  "@R0, @R7, R8, NOV", REGISTER_NOT_ALLOWED, "R0, @R7, R8, NOV", 0xBB72, 0x080C);
        ERRT("CPSI",  "@R6, @R0, R8, NOV", REGISTER_NOT_ALLOWED,      "R0, R8, NOV", 0xBB02, 0x086C);
        TEST("CPSIB", "@R6, @R7, R8, PL", 0xBA72, 0x086D);
        TEST("CPSIB", "@R6, @R7, R0, PL", 0xBA72, 0x006D);
        UNKN(                             0xBA72, 0xC86D);
        ERRT("CPSIB", "@R0, @R7, R8, PL", REGISTER_NOT_ALLOWED, "R0, @R7, R8, PL", 0xBA72, 0x080D);
        ERRT("CPSIB", "@R6, @R0, R8, PL", REGISTER_NOT_ALLOWED,      "R0, R8, PL", 0xBA02, 0x086D);
    }

    // Compare String, Increment, and Repeat
    if (z8001()) {
        TEST("CPSIR",  "@RR4, @RR8, R6, NZ", 0xBB86, 0x064E);
        TEST("CPSIR",  "@RR4, @RR8, R0, NZ", 0xBB86, 0x004E);
        UNKN(                                0xBB86, 0xD64E);
        ERRT("CPSIR",  "@RR0, @RR8, R6, NZ", REGISTER_NOT_ALLOWED, "RR0, @RR8, R6, NZ", 0xBB86, 0x060E);
        ERRT("CPSIR",  "@RR4, @RR0, R6, NZ", REGISTER_NOT_ALLOWED,       "RR0, R6, NZ", 0xBB06, 0x064E);
        TEST("CPSIRB", "@RR4, @RR8, R6, NC", 0xBA86, 0x064F);
        TEST("CPSIRB", "@RR4, @RR8, R0, NC", 0xBA86, 0x004F);
        UNKN(                                0xBA86, 0xE64F);
        ERRT("CPSIRB", "@RR0, @RR8, R6, NC", REGISTER_NOT_ALLOWED, "RR0, @RR8, R6, NC", 0xBA86, 0x060F);
        ERRT("CPSIRB", "@RR4, @RR0, R6, NC", REGISTER_NOT_ALLOWED,       "RR0, R6, NC", 0xBA06, 0x064F);
    } else {
        TEST("CPSIR",  "@R9, @R10, R11, NZ", 0xBBA6, 0x0B9E);
        TEST("CPSIR",  "@R9, @R10, R0, NZ",  0xBBA6, 0x009E);
        UNKN(                                0xBBA6, 0xFB9E);
        ERRT("CPSIR",  "@R0, @R10, R11, NZ", REGISTER_NOT_ALLOWED, "R0, @R10, R11, NZ", 0xBBA6, 0x0B0E);
        ERRT("CPSIR",  "@R9, @R0, R11, NZ",  REGISTER_NOT_ALLOWED,      "R0, R11, NZ", 0xBB06, 0x0B9E);
        TEST("CPSIRB", "@R1, @R10, R11, NC", 0xBAA6, 0x0B1F);
        TEST("CPSIRB", "@R1, @R10, R0, NC",  0xBAA6, 0x001F);
        UNKN(                                0xBAA6, 0x1B1F);
        ERRT("CPSIRB", "@R0, @R10, R11, NC", REGISTER_NOT_ALLOWED, "R0, @R10, R11, NC", 0xBAA6, 0x0B0F);
        ERRT("CPSIRB", "@R1, @R0, R11, NC",  REGISTER_NOT_ALLOWED,      "R0, R11, NC", 0xBA06, 0x0B1F);
    }
}

static void test_translation() {
    // Translate and Decrement
    // Original content of RH1 are lost. R0/R1 or RR0 must not be used as a soure or destination
    // R1 should not be used as a counter as welll
    if (z8001()) {
        TEST("TRDB", "@RR2, @RR4, R6", 0xB828, 0x0640);
        UNKN(                          0xB828, 0x1640);
        UNKN(                          0xB828, 0x0642);
        TEST("TRDB", "@RR2, @RR4, R0", 0xB828, 0x0040);
        ERRT("TRDB", "@RR0, @RR4, R6", REGISTER_NOT_ALLOWED, "RR0, @RR4, R6", 0xB808, 0x0640);
        ERRT("TRDB", "@RR2, @RR0, R6", REGISTER_NOT_ALLOWED,       "RR0, R6", 0xB828, 0x0600);
        ERRT("TRDB", "@RR2, @RR4, R1", REGISTER_NOT_ALLOWED,            "R1", 0xB828, 0x0140);
        ERRT("TRDB", "@RR2, @RR2, R6", REGISTERS_OVERLAPPED,       "RR2, R6", 0xB828, 0x0620);
        ERRT("TRDB", "@RR2, @RR4, R3", REGISTERS_OVERLAPPED,            "R3", 0xB828, 0x0340);
        ERRT("TRDB", "@RR2, @RR4, R5", REGISTERS_OVERLAPPED,            "R5", 0xB828, 0x0540);
    } else {
        TEST("TRDB", "@R3, @R2, R4", 0xB838, 0x0420);
        UNKN(                        0xB838, 0x3420);
        UNKN(                        0xB838, 0x0424);
        TEST("TRDB", "@R3, @R2, R0", 0xB838, 0x0020);
        ERRT("TRDB", "@R0, @R2, R3", REGISTER_NOT_ALLOWED, "R0, @R2, R3", 0xB808, 0x0320);
        ERRT("TRDB", "@R1, @R2, R3", REGISTER_NOT_ALLOWED, "R1, @R2, R3", 0xB818, 0x0320);
        ERRT("TRDB", "@R2, @R0, R3", REGISTER_NOT_ALLOWED,      "R0, R3", 0xB828, 0x0300);
        ERRT("TRDB", "@R2, @R1, R3", REGISTER_NOT_ALLOWED,      "R1, R3", 0xB828, 0x0310);
        ERRT("TRDB", "@R3, @R2, R1", REGISTER_NOT_ALLOWED,          "R1", 0xB838, 0x0120);
        ERRT("TRDB", "@R3, @R3, R4", REGISTERS_OVERLAPPED,      "R3, R4", 0xB838, 0x0430);
        ERRT("TRDB", "@R3, @R2, R3", REGISTERS_OVERLAPPED,          "R3", 0xB838, 0x0320);
        ERRT("TRDB", "@R3, @R2, R2", REGISTERS_OVERLAPPED,          "R2", 0xB838, 0x0220);
    }

    // Translate, Decrement, and Repeat
    if (z8001()) {
        TEST("TRDRB", "@RR4, @RR6, R2", 0xB84C, 0x0260);
        UNKN(                           0xB84C, 0x5260);
        UNKN(                           0xB84C, 0x0266);
        TEST("TRDRB", "@RR4, @RR6, R0", 0xB84C, 0x0060);
        ERRT("TRDRB", "@RR0, @RR6, R2", REGISTER_NOT_ALLOWED, "RR0, @RR6, R2", 0xB80C, 0x0260);
        ERRT("TRDRB", "@RR4, @RR0, R2", REGISTER_NOT_ALLOWED,       "RR0, R2", 0xB84C, 0x0200);
        ERRT("TRDRB", "@RR4, @RR6, R1", REGISTER_NOT_ALLOWED,            "R1", 0xB84C, 0x0160);
        ERRT("TRDRB", "@RR4, @RR4, R2", REGISTERS_OVERLAPPED,       "RR4, R2", 0xB84C, 0x0240);
        ERRT("TRDRB", "@RR4, @RR6, R5", REGISTERS_OVERLAPPED,            "R5", 0xB84C, 0x0560);
        ERRT("TRDRB", "@RR4, @RR6, R7", REGISTERS_OVERLAPPED,            "R7", 0xB84C, 0x0760);
    } else {
        TEST("TRDRB", "@R4, @R5, R6", 0xB84C, 0x0650);
        UNKN(                         0xB84C, 0x7650);
        UNKN(                         0xB84C, 0x0658);
        TEST("TRDRB", "@R4, @R5, R0", 0xB84C, 0x0050);
        ERRT("TRDRB", "@R0, @R5, R6", REGISTER_NOT_ALLOWED, "R0, @R5, R6", 0xB80C, 0x0650);
        ERRT("TRDRB", "@R1, @R5, R6", REGISTER_NOT_ALLOWED, "R1, @R5, R6", 0xB81C, 0x0650);
        ERRT("TRDRB", "@R4, @R0, R6", REGISTER_NOT_ALLOWED,      "R0, R6", 0xB84C, 0x0600);
        ERRT("TRDRB", "@R4, @R1, R6", REGISTER_NOT_ALLOWED,      "R1, R6", 0xB84C, 0x0610);
        ERRT("TRDRB", "@R4, @R5, R1", REGISTER_NOT_ALLOWED,          "R1", 0xB84C, 0x0150);
        ERRT("TRDRB", "@R4, @R4, R6", REGISTERS_OVERLAPPED,      "R4, R6", 0xB84C, 0x0640);
        ERRT("TRDRB", "@R4, @R5, R4", REGISTERS_OVERLAPPED,          "R4", 0xB84C, 0x0450);
        ERRT("TRDRB", "@R4, @R5, R5", REGISTERS_OVERLAPPED,          "R5", 0xB84C, 0x0550);
    }

    // Translate and Increment
    if (z8001()) {
        TEST("TRIB", "@RR6, @RR8, R3", 0xB860, 0x0380);
        UNKN(                          0xB860, 0x9380);
        UNKN(                          0xB860, 0x038A);
        TEST("TRIB", "@RR6, @RR8, R0", 0xB860, 0x0080);
        ERRT("TRIB", "@RR0, @RR8, R3", REGISTER_NOT_ALLOWED, "RR0, @RR8, R3", 0xB800, 0x0380);
        ERRT("TRIB", "@RR6, @RR0, R3", REGISTER_NOT_ALLOWED,       "RR0, R3", 0xB860, 0x0300);
        ERRT("TRIB", "@RR6, @RR8, R1", REGISTER_NOT_ALLOWED,            "R1", 0xB860, 0x0180);
        ERRT("TRIB", "@RR6, @RR6, R3", REGISTERS_OVERLAPPED,       "RR6, R3", 0xB860, 0x0360);
        ERRT("TRIB", "@RR6, @RR8, R7", REGISTERS_OVERLAPPED,            "R7", 0xB860, 0x0780);
        ERRT("TRIB", "@RR6, @RR8, R9", REGISTERS_OVERLAPPED,            "R9", 0xB860, 0x0980);
    } else {
        TEST("TRIB", "@R7, @R8, R9", 0xB870, 0x0980);
        UNKN(                        0xB870, 0xB980);
        UNKN(                        0xB870, 0x098C);
        TEST("TRIB", "@R7, @R8, R0", 0xB870, 0x0080);
        ERRT("TRIB", "@R0, @R8, R9", REGISTER_NOT_ALLOWED, "R0, @R8, R9", 0xB800, 0x0980);
        ERRT("TRIB", "@R1, @R8, R9", REGISTER_NOT_ALLOWED, "R1, @R8, R9", 0xB810, 0x0980);
        ERRT("TRIB", "@R7, @R0, R9", REGISTER_NOT_ALLOWED,      "R0, R9", 0xB870, 0x0900);
        ERRT("TRIB", "@R7, @R1, R9", REGISTER_NOT_ALLOWED,      "R1, R9", 0xB870, 0x0910);
        ERRT("TRIB", "@R7, @R8, R1", REGISTER_NOT_ALLOWED,          "R1", 0xB870, 0x0180);
        ERRT("TRIB", "@R7, @R7, R9", REGISTERS_OVERLAPPED,      "R7, R9", 0xB870, 0x0970);
        ERRT("TRIB", "@R7, @R8, R7", REGISTERS_OVERLAPPED,          "R7", 0xB870, 0x0780);
        ERRT("TRIB", "@R7, @R8, R8", REGISTERS_OVERLAPPED,          "R8", 0xB870, 0x0880);
    }

    // Translate, Increment, and Repeat
    if (z8001()) {
        TEST("TRIRB", "@RR8, @RR10, R12", 0xB884, 0x0CA0);
        UNKN(                             0xB884, 0xDCA0);
        UNKN(                             0xB884, 0x0CAE);
        TEST("TRIRB", "@RR8, @RR10, R0",  0xB884, 0x00A0);
        ERRT("TRIRB", "@RR0, @RR10, R12", REGISTER_NOT_ALLOWED, "RR0, @RR10, R12", 0xB804, 0x0CA0);
        ERRT("TRIRB", "@RR8, @RR0, R12",  REGISTER_NOT_ALLOWED,        "RR0, R12", 0xB884, 0x0C00);
        ERRT("TRIRB", "@RR8, @RR10, R1",  REGISTER_NOT_ALLOWED,              "R1", 0xB884, 0x01A0);
        ERRT("TRIRB", "@RR8, @RR8, R12",  REGISTERS_OVERLAPPED,        "RR8, R12", 0xB884, 0x0C80);
        ERRT("TRIRB", "@RR8, @RR10, R9",  REGISTERS_OVERLAPPED,              "R9", 0xB884, 0x09A0);
        ERRT("TRIRB", "@RR8, @RR10, R11", REGISTERS_OVERLAPPED,             "R11", 0xB884, 0x0BA0);
    } else {
        TEST("TRIRB", "@R10, @R11, R12", 0xB8A4, 0x0CB0);
        UNKN(                            0xB8A4, 0xFCB0);
        UNKN(                            0xB8A4, 0x0CB1);
        TEST("TRIRB", "@R10, @R11, R0",  0xB8A4, 0x00B0);
        ERRT("TRIRB", "@R0, @R11, R12",  REGISTER_NOT_ALLOWED, "R0, @R11, R12", 0xB804, 0x0CB0);
        ERRT("TRIRB", "@R1, @R11, R12",  REGISTER_NOT_ALLOWED, "R1, @R11, R12", 0xB814, 0x0CB0);
        ERRT("TRIRB", "@R10, @R0, R12",  REGISTER_NOT_ALLOWED,       "R0, R12", 0xB8A4, 0x0C00);
        ERRT("TRIRB", "@R10, @R1, R12",  REGISTER_NOT_ALLOWED,       "R1, R12", 0xB8A4, 0x0C10);
        ERRT("TRIRB", "@R10, @R11, R1",  REGISTER_NOT_ALLOWED,            "R1", 0xB8A4, 0x01B0);
        ERRT("TRIRB", "@R10, @R10, R12", REGISTERS_OVERLAPPED,      "R10, R12", 0xB8A4, 0x0CA0);
        ERRT("TRIRB", "@R10, @R11, R10", REGISTERS_OVERLAPPED,           "R10", 0xB8A4, 0x0AB0);
        ERRT("TRIRB", "@R10, @R11, R11", REGISTERS_OVERLAPPED,           "R11", 0xB8A4, 0x0BB0);
    }

    // Translate, Test, and Decrement
    if (z8001()) {
        TEST("TRTDB", "@RR10, @RR12, R9", 0xB8AA, 0x09C0);
        UNKN(                             0xB8AA, 0x29C0);
        UNKN(                             0xB8AA, 0x09C3);
        TEST("TRTDB", "@RR10, @RR12, R0", 0xB8AA, 0x00C0);
        ERRT("TRTDB", "@RR0, @RR12, R9",   REGISTER_NOT_ALLOWED, "RR0, @RR12, R9", 0xB80A, 0x09C0);
        ERRT("TRTDB", "@RR10, @RR0, R9",   REGISTER_NOT_ALLOWED,        "RR0, R9", 0xB8AA, 0x0900);
        ERRT("TRTDB", "@RR10, @RR12, R1",  REGISTER_NOT_ALLOWED,             "R1", 0xB8AA, 0x01C0);
        ERRT("TRTDB", "@RR10, @RR10, R9",  REGISTERS_OVERLAPPED,       "RR10, R9", 0xB8AA, 0x09A0);
        ERRT("TRTDB", "@RR10, @RR12, R10", REGISTERS_OVERLAPPED,            "R10", 0xB8AA, 0x0AC0);
        ERRT("TRTDB", "@RR10, @RR12, R13", REGISTERS_OVERLAPPED,            "R13", 0xB8AA, 0x0DC0);
    } else {
        TEST("TRTDB", "@R13, @R14, R15", 0xB8DA, 0x0FE0);
        UNKN(                            0xB8DA, 0x4FE0);
        UNKN(                            0xB8DA, 0x0FE5);
        TEST("TRTDB", "@R13, @R14, R0",  0xB8DA, 0x00E0);
        ERRT("TRTDB", "@R0, @R14, R15",  REGISTER_NOT_ALLOWED, "R0, @R14, R15", 0xB80A, 0x0FE0);
        ERRT("TRTDB", "@R1, @R14, R15",  REGISTER_NOT_ALLOWED, "R1, @R14, R15", 0xB81A, 0x0FE0);
        ERRT("TRTDB", "@R13, @R0, R15",  REGISTER_NOT_ALLOWED,       "R0, R15", 0xB8DA, 0x0F00);
        ERRT("TRTDB", "@R13, @R1, R15",  REGISTER_NOT_ALLOWED,       "R1, R15", 0xB8DA, 0x0F10);
        ERRT("TRTDB", "@R13, @R14, R1",  REGISTER_NOT_ALLOWED,            "R1", 0xB8DA, 0x01E0);
        ERRT("TRTDB", "@R13, @R13, R15", REGISTERS_OVERLAPPED,      "R13, R15", 0xB8DA, 0x0FD0);
        ERRT("TRTDB", "@R13, @R14, R13", REGISTERS_OVERLAPPED,           "R13", 0xB8DA, 0x0DE0);
        ERRT("TRTDB", "@R13, @R14, R14", REGISTERS_OVERLAPPED,           "R14", 0xB8DA, 0x0EE0);
    }

    // Translate, Test, Decrement, and Repeat
    if (z8001()) {
        TEST("TRTDRB", "@RR12, @RR14, R2", 0xB8CE, 0x02EE);
        NMEM("",       "",             "", 0xB8CE);
        UNKN(                              0xB8CE, 0x62EE);
        UNKN(                              0xB8CE, 0x02E0);
        TEST("TRTDRB", "@RR12, @RR14, R0", 0xB8CE, 0x00EE);
        ERRT("TRTDRB", "@RR0, @RR14, R2",   REGISTER_NOT_ALLOWED, "RR0, @RR14, R2", 0xB80E, 0x02EE);
        ERRT("TRTDRB", "@RR12, @RR0, R2",   REGISTER_NOT_ALLOWED,        "RR0, R2", 0xB8CE, 0x020E);
        ERRT("TRTDRB", "@RR12, @RR14, R1",  REGISTER_NOT_ALLOWED,             "R1", 0xB8CE, 0x01EE);
        ERRT("TRTDRB", "@RR12, @RR12, R2",  REGISTERS_OVERLAPPED,       "RR12, R2", 0xB8CE, 0x02CE);
        ERRT("TRTDRB", "@RR12, @RR14, R13", REGISTERS_OVERLAPPED,            "R13", 0xB8CE, 0x0DEE);
        ERRT("TRTDRB", "@RR12, @RR14, R14", REGISTERS_OVERLAPPED,            "R14", 0xB8CE, 0x0EEE);
    } else {
        TEST("TRTDRB", "@R15, @R14, R13", 0xB8FE, 0x0DEE);
        UNKN(                             0xB8FE, 0x8DEE);
        UNKN(                             0xB8FE, 0x0DE9);
        TEST("TRTDRB", "@R15, @R14, R0",  0xB8FE, 0x00EE);
        ERRT("TRTDRB", "@R0, @R14, R13",  REGISTER_NOT_ALLOWED, "R0, @R14, R13", 0xB80E, 0x0DEE);
        ERRT("TRTDRB", "@R1, @R14, R13",  REGISTER_NOT_ALLOWED, "R1, @R14, R13", 0xB81E, 0x0DEE);
        ERRT("TRTDRB", "@R15, @R0, R13",  REGISTER_NOT_ALLOWED,       "R0, R13", 0xB8FE, 0x0D0E);
        ERRT("TRTDRB", "@R15, @R1, R13",  REGISTER_NOT_ALLOWED,       "R1, R13", 0xB8FE, 0x0D1E);
        ERRT("TRTDRB", "@R15, @R14, R1",  REGISTER_NOT_ALLOWED,            "R1", 0xB8FE, 0x01EE);
        ERRT("TRTDRB", "@R15, @R15, R13", REGISTERS_OVERLAPPED,      "R15, R13", 0xB8FE, 0x0DFE);
        ERRT("TRTDRB", "@R15, @R14, R15", REGISTERS_OVERLAPPED,           "R15", 0xB8FE, 0x0FEE);
        ERRT("TRTDRB", "@R15, @R14, R14", REGISTERS_OVERLAPPED,           "R14", 0xB8FE, 0x0EEE);
    }

    // Translate, Test, and Increment
    if (z8001()) {
        TEST("TRTIB", "@RR14, @RR12, R11", 0xB8E2, 0x0BC0);
        UNKN(                              0xB8E2, 0xABC0);
        UNKN(                              0xB8E2, 0x0BCB);
        TEST("TRTIB", "@RR14, @RR12, R0",  0xB8E2, 0x00C0);
        ERRT("TRTIB", "@RR0, @RR12, R11",  REGISTER_NOT_ALLOWED, "RR0, @RR12, R11", 0xB802, 0x0BC0);
        ERRT("TRTIB", "@RR14, @RR0, R11",  REGISTER_NOT_ALLOWED,        "RR0, R11", 0xB8E2, 0x0B00);
        ERRT("TRTIB", "@RR14, @RR12, R1",  REGISTER_NOT_ALLOWED,              "R1", 0xB8E2, 0x01C0);
        ERRT("TRTIB", "@RR14, @RR14, R11", REGISTERS_OVERLAPPED,       "RR14, R11", 0xB8E2, 0x0BE0);
        ERRT("TRTIB", "@RR14, @RR12, R14", REGISTERS_OVERLAPPED,             "R14", 0xB8E2, 0x0EC0);
        ERRT("TRTIB", "@RR14, @RR12, R12", REGISTERS_OVERLAPPED,             "R12", 0xB8E2, 0x0CC0);
    } else {
        TEST("TRTIB", "@R12, @R11, R10", 0xB8C2, 0x0AB0);
        UNKN(                            0xB8C2, 0xCAB0);
        UNKN(                            0xB8C2, 0x0ABD);
        TEST("TRTIB", "@R12, @R11, R0" , 0xB8C2, 0x00B0);
        ERRT("TRTIB", "@R0, @R11, R10",  REGISTER_NOT_ALLOWED, "R0, @R11, R10", 0xB802, 0x0AB0);
        ERRT("TRTIB", "@R1, @R11, R10",  REGISTER_NOT_ALLOWED, "R1, @R11, R10", 0xB812, 0x0AB0);
        ERRT("TRTIB", "@R12, @R0, R10",  REGISTER_NOT_ALLOWED,       "R0, R10", 0xB8C2, 0x0A00);
        ERRT("TRTIB", "@R12, @R1, R10",  REGISTER_NOT_ALLOWED,       "R1, R10", 0xB8C2, 0x0A10);
        ERRT("TRTIB", "@R12, @R11, R1",  REGISTER_NOT_ALLOWED,            "R1", 0xB8C2, 0x01B0);
        ERRT("TRTIB", "@R12, @R12, R10", REGISTERS_OVERLAPPED,      "R12, R10", 0xB8C2, 0x0AC0);
        ERRT("TRTIB", "@R12, @R11, R12", REGISTERS_OVERLAPPED,           "R12", 0xB8C2, 0x0CB0);
        ERRT("TRTIB", "@R12, @R11, R11", REGISTERS_OVERLAPPED,           "R11", 0xB8C2, 0x0BB0);
    }

    // Translate, Test, Increment, and Repeat
    if (z8001()) {
        TEST("TRTIRB", "@RR10, @RR8, R7", 0xB8A6, 0x078E);
        UNKN(                             0xB8A6, 0xE78E);
        UNKN(                             0xB8A6, 0x0780);
        TEST("TRTIRB", "@RR10, @RR8, R0", 0xB8A6, 0x008E);
        ERRT("TRTIRB", "@RR0, @RR8, R7",   REGISTER_NOT_ALLOWED, "RR0, @RR8, R7", 0xB806, 0x078E);
        ERRT("TRTIRB", "@RR10, @RR0, R7",  REGISTER_NOT_ALLOWED,       "RR0, R7", 0xB8A6, 0x070E);
        ERRT("TRTIRB", "@RR10, @RR8, R1",  REGISTER_NOT_ALLOWED,            "R1", 0xB8A6, 0x018E);
        ERRT("TRTIRB", "@RR10, @RR10, R7", REGISTERS_OVERLAPPED,      "RR10, R7", 0xB8A6, 0x07AE);
        ERRT("TRTIRB", "@RR10, @RR8, R11", REGISTERS_OVERLAPPED,           "R11", 0xB8A6, 0x0B8E);
        ERRT("TRTIRB", "@RR10, @RR8, R9",  REGISTERS_OVERLAPPED,            "R9", 0xB8A6, 0x098E);
    } else {
        TEST("TRTIRB", "@R9, @R8, R7", 0xB896, 0x078E);
        NMEM("",       "",         "", 0xB896);
        UNKN(                          0xB896, 0xF78E);
        UNKN(                          0xB896, 0x0780);
        TEST("TRTIRB", "@R9, @R8, R0", 0xB896, 0x008E);
        ERRT("TRTIRB", "@R0, @R8, R7", REGISTER_NOT_ALLOWED, "R0, @R8, R7", 0xB806, 0x078E);
        ERRT("TRTIRB", "@R1, @R8, R7", REGISTER_NOT_ALLOWED, "R1, @R8, R7", 0xB816, 0x078E);
        ERRT("TRTIRB", "@R9, @R0, R7", REGISTER_NOT_ALLOWED,      "R0, R7", 0xB896, 0x070E);
        ERRT("TRTIRB", "@R9, @R1, R7", REGISTER_NOT_ALLOWED,      "R1, R7", 0xB896, 0x071E);
        ERRT("TRTIRB", "@R9, @R8, R1", REGISTER_NOT_ALLOWED,          "R1", 0xB896, 0x018E);
        ERRT("TRTIRB", "@R9, @R9, R7", REGISTERS_OVERLAPPED,      "R9, R7", 0xB896, 0x079E);
        ERRT("TRTIRB", "@R9, @R8, R9", REGISTERS_OVERLAPPED,          "R9", 0xB896, 0x098E);
        ERRT("TRTIRB", "@R9, @R8, R8", REGISTERS_OVERLAPPED,          "R8", 0xB896, 0x088E);
    }
}


static void test_input() {
    // Input
    TEST("IN",  "R1, @R2",  0x3D21);
    TEST("INB", "RH1, @R2", 0x3C21);
    ERRT("IN",  "R1, @R0",  REGISTER_NOT_ALLOWED, "R0", 0x3D01);
    ERRT("INB", "RH1, @R0", REGISTER_NOT_ALLOWED, "R0", 0x3C01);
    TEST("IN",  "R1, %1234",  0x3B14, 0x1234);
    TEST("INB", "RH1, %1234", 0x3A14, 0x1234);

    // Special Input
    TEST("SIN",  "R1, %1234",  0x3B15, 0x1234);
    TEST("SINB", "RH1, %1234", 0x3A15, 0x1234);

    // Input and Decrement
    if (z8001()) {
        TEST("IND",  "@RR2, @R4, R6", 0x3B48, 0x0628);
        TEST("INDB", "@RR2, @R4, R6", 0x3A48, 0x0628);
        NMEM("INDRB","",          "", 0x3A48);
        UNKN(                         0x3B48, 0x1628);
        UNKN(                         0x3A48, 0x2628);
        UNKN(                         0x3B48, 0x0629);
        UNKN(                         0x3A48, 0x062A);
        ERRT("IND",  "@RR2, @R0, R6", REGISTER_NOT_ALLOWED,       "R0, R6", 0x3B08, 0x0628);
        ERRT("IND",  "@RR0, @R4, R6", REGISTER_NOT_ALLOWED, "RR0, @R4, R6", 0x3B48, 0x0608);
        ERRT("INDB", "@RR2, @R0, R6", REGISTER_NOT_ALLOWED,       "R0, R6", 0x3A08, 0x0628);
        ERRT("INDB", "@RR0, @R4, R6", REGISTER_NOT_ALLOWED, "RR0, @R4, R6", 0x3A48, 0x0608);
    } else {
        TEST("IND",  "@R1, @R2, R0", 0x3B28, 0x0018);
        TEST("INDB", "@R1, @R2, R0", 0x3A28, 0x0018);
        UNKN(                        0x3B28, 0x3018);
        UNKN(                        0x3A28, 0x4018);
        UNKN(                        0x3B28, 0x001B);
        UNKN(                        0x3A28, 0x001C);
        ERRT("IND",  "@R1, @R0, R6", REGISTER_NOT_ALLOWED,      "R0, R6", 0x3B08, 0x0618);
        ERRT("IND",  "@R0, @R2, R6", REGISTER_NOT_ALLOWED, "R0, @R2, R6", 0x3B28, 0x0608);
        ERRT("INDB", "@R1, @R0, R6", REGISTER_NOT_ALLOWED,      "R0, R6", 0x3A08, 0x0618);
        ERRT("INDB", "@R0, @R2, R6", REGISTER_NOT_ALLOWED, "R0, @R2, R6", 0x3A28, 0x0608);
    }

    // Special Input and Decrement
    if (z8001()) {
        TEST("SIND",  "@RR4, @R3, R0", 0x3B39, 0x0048);
        TEST("SINDB", "@RR4, @R3, R0", 0x3A39, 0x0048);
        UNKN(                          0x3B39, 0x5048);
        UNKN(                          0x3A39, 0x6048);
        UNKN(                          0x3B39, 0x004D);
        UNKN(                          0x3A39, 0x004E);
        ERRT("SIND",  "@RR4, @R0, R6", REGISTER_NOT_ALLOWED,       "R0, R6", 0x3B09, 0x0648);
        ERRT("SIND",  "@RR0, @R3, R6", REGISTER_NOT_ALLOWED, "RR0, @R3, R6", 0x3B39, 0x0608);
        ERRT("SINDB", "@RR4, @R0, R6", REGISTER_NOT_ALLOWED,       "R0, R6", 0x3A09, 0x0648);
        ERRT("SINDB", "@RR0, @R3, R6", REGISTER_NOT_ALLOWED, "RR0, @R3, R6", 0x3A39, 0x0608);
    } else {
        TEST("SIND",  "@R3, @R4, R5", 0x3B49, 0x0538);
        TEST("SINDB", "@R3, @R4, R5", 0x3A49, 0x0538);
        UNKN(                         0x3B49, 0x7538);
        UNKN(                         0x3A49, 0x8538);
        UNKN(                         0x3B49, 0x053F);
        UNKN(                         0x3A49, 0x0531);
        ERRT("SIND",  "@R3, @R0, R5", REGISTER_NOT_ALLOWED,      "R0, R5", 0x3B09, 0x0538);
        ERRT("SIND",  "@R0, @R4, R5", REGISTER_NOT_ALLOWED, "R0, @R4, R5", 0x3B49, 0x0508);
        ERRT("SINDB", "@R3, @R0, R5", REGISTER_NOT_ALLOWED,      "R0, R5", 0x3A09, 0x0538);
        ERRT("SINDB", "@R0, @R4, R5", REGISTER_NOT_ALLOWED, "R0, @R4, R5", 0x3A49, 0x0508);
    }

    // Input, Decrement, and Repeat
    if (z8001()) {
        TEST("INDR",  "@RR6, @R4, R8", 0x3B48, 0x0860);
        TEST("INDRB", "@RR6, @R4, R8", 0x3A48, 0x0860);
        UNKN(                          0x3B48, 0x9860);
        UNKN(                          0x3A48, 0xA860);
        UNKN(                          0x3B48, 0x0862);
        UNKN(                          0x3A48, 0x0863);
        ERRT("INDR",  "@RR6, @R0, R8", REGISTER_NOT_ALLOWED,       "R0, R8", 0x3B08, 0x0860);
        ERRT("INDR",  "@RR0, @R4, R8", REGISTER_NOT_ALLOWED, "RR0, @R4, R8", 0x3B48, 0x0800);
        ERRT("INDRB", "@RR6, @R0, R8", REGISTER_NOT_ALLOWED,       "R0, R8", 0x3A08, 0x0860);
        ERRT("INDRB", "@RR0, @R4, R8", REGISTER_NOT_ALLOWED, "RR0, @R4, R8", 0x3A48, 0x0800);
    } else {
        TEST("INDR",  "@R6, @R7, R8", 0x3B78, 0x0860);
        TEST("INDRB", "@R6, @R7, R8", 0x3A78, 0x0860);
        UNKN(                         0x3B78, 0xB860);
        UNKN(                         0x3A78, 0xC860);
        UNKN(                         0x3B78, 0x0864);
        UNKN(                         0x3A78, 0x0865);
        ERRT("INDR",  "@R6, @R0, R8", REGISTER_NOT_ALLOWED,      "R0, R8", 0x3B08, 0x0860);
        ERRT("INDR",  "@R0, @R7, R8", REGISTER_NOT_ALLOWED, "R0, @R7, R8", 0x3B78, 0x0800);
        ERRT("INDRB", "@R6, @R0, R8", REGISTER_NOT_ALLOWED,      "R0, R8", 0x3A08, 0x0860);
        ERRT("INDRB", "@R0, @R7, R8", REGISTER_NOT_ALLOWED, "R0, @R7, R8", 0x3A78, 0x0800);
    }

    // Special Input, Decrement, and Repeat
    if (z8001()) {
        TEST("SINDR",  "@RR8, @R10, R12", 0x3BA9, 0x0C80);
        TEST("SINDRB", "@RR8, @R10, R12", 0x3AA9, 0x0C80);
        UNKN(                             0x3BA9, 0xBC80);
        UNKN(                             0x3AA9, 0xCC80);
        UNKN(                             0x3BA9, 0x0C86);
        UNKN(                             0x3AA9, 0x0C87);
        ERRT("SINDR",  "@RR8, @R0, R12",  REGISTER_NOT_ALLOWED,        "R0, R12", 0x3B09, 0x0C80);
        ERRT("SINDR",  "@RR0, @R10, R12", REGISTER_NOT_ALLOWED, "RR0, @R10, R12", 0x3BA9, 0x0C00);
        ERRT("SINDRB", "@RR8, @R0, R12",  REGISTER_NOT_ALLOWED,        "R0, R12", 0x3A09, 0x0C80);
        ERRT("SINDRB", "@RR0, @R10, R12", REGISTER_NOT_ALLOWED, "RR0, @R10, R12", 0x3AA9, 0x0C00);
    } else {
        TEST("SINDR",  "@R9, @R10, R11", 0x3BA9, 0x0B90);
        TEST("SINDRB", "@R9, @R10, R11", 0x3AA9, 0x0B90);
        UNKN(                            0x3BA9, 0xDB90);
        UNKN(                            0x3AA9, 0xEB90);
        UNKN(                            0x3BA9, 0x0B99);
        UNKN(                            0x3AA9, 0x0B9A);
        ERRT("SINDR",  "@R9, @R0, R11",  REGISTER_NOT_ALLOWED,       "R0, R11", 0x3B09, 0x0B90);
        ERRT("SINDR",  "@R0, @R10, R11", REGISTER_NOT_ALLOWED, "R0, @R10, R11", 0x3BA9, 0x0B00);
        ERRT("SINDRB", "@R9, @R0, R11",  REGISTER_NOT_ALLOWED,       "R0, R11", 0x3A09, 0x0B90);
        ERRT("SINDRB", "@R0, @R10, R11", REGISTER_NOT_ALLOWED, "R0, @R10, R11", 0x3AA9, 0x0B00);
    }

    // Input and Increment
    if (z8001()) {
        TEST("INI",  "@RR14, @R2, R1", 0x3B20, 0x01E8);
        TEST("INIB", "@RR14, @R2, R1", 0x3A20, 0x01E8);
        UNKN(                          0x3B20, 0xF1E8);
        UNKN(                          0x3A20, 0x11E8);
        UNKN(                          0x3B20, 0x01EB);
        UNKN(                          0x3A20, 0x01EC);
        ERRT("INI",  "@RR14, @R0, R1", REGISTER_NOT_ALLOWED,       "R0, R1", 0x3B00, 0x01E8);
        ERRT("INI",  "@RR0, @R2, R1",  REGISTER_NOT_ALLOWED, "RR0, @R2, R1", 0x3B20, 0x0108);
        ERRT("INIB", "@RR14, @R0, R1", REGISTER_NOT_ALLOWED,       "R0, R1", 0x3A00, 0x01E8);
        ERRT("INIB", "@RR0, @R2, R1",  REGISTER_NOT_ALLOWED, "RR0, @R2, R1", 0x3A20, 0x0108);
    } else {
        TEST("INI",  "@R12, @R13, R14", 0x3BD0, 0x0EC8);
        TEST("INIB", "@R12, @R13, R14", 0x3AD0, 0x0EC8);
        NMEM("INIRB", "",           "", 0x3AD0);
        UNKN(                           0x3BD0, 0x2EC8);
        UNKN(                           0x3AD0, 0x3EC8);
        UNKN(                           0x3BD0, 0x0ECB);
        UNKN(                           0x3AD0, 0x0ECC);
        ERRT("INI",  "@R12, @R0, R14", REGISTER_NOT_ALLOWED,       "R0, R14", 0x3B00, 0x0EC8);
        ERRT("INI",  "@R0, @R13, R14", REGISTER_NOT_ALLOWED, "R0, @R13, R14", 0x3BD0, 0x0E08);
        ERRT("INIB", "@R12, @R0, R14", REGISTER_NOT_ALLOWED,       "R0, R14", 0x3A00, 0x0EC8);
        ERRT("INIB", "@R0, @R13, R14", REGISTER_NOT_ALLOWED, "R0, @R13, R14", 0x3AD0, 0x0E08);
    }

    // Special Input and Increment
    if (z8001()) {
        TEST("SINI",  "@RR12, @R11, R10", 0x3BB1, 0x0AC8);
        TEST("SINIB", "@RR12, @R11, R10", 0x3AB1, 0x0AC8);
        UNKN(                             0x3BB1, 0x2AC8);
        UNKN(                             0x3AB1, 0x3AC8);
        UNKN(                             0x3BB1, 0x0ACD);
        UNKN(                             0x3AB1, 0x0ACE);
        ERRT("SINI",  "@RR12, @R0, R10",  REGISTER_NOT_ALLOWED,        "R0, R10", 0x3B01, 0x0AC8);
        ERRT("SINI",  "@RR0, @R11, R10",  REGISTER_NOT_ALLOWED, "RR0, @R11, R10", 0x3BB1, 0x0A08);
        ERRT("SINIB", "@RR12, @R0, R10",  REGISTER_NOT_ALLOWED,        "R0, R10", 0x3A01, 0x0AC8);
        ERRT("SINIB", "@RR0, @R11, R10",  REGISTER_NOT_ALLOWED, "RR0, @R11, R10", 0x3AB1, 0x0A08);
    } else {
        TEST("SINI",  "@R15, @R1, R0", 0x3B11, 0x00F8);
        TEST("SINIB", "@R15, @R1, R0", 0x3A11, 0x00F8);
        UNKN(                          0x3B11, 0x40F8);
        UNKN(                          0x3A11, 0x50F8);
        UNKN(                          0x3B11, 0x00FF);
        UNKN(                          0x3A11, 0x00F1);
        ERRT("SINI",  "@R15, @R0, R7", REGISTER_NOT_ALLOWED,      "R0, R7", 0x3B01, 0x07F8);
        ERRT("SINI",  "@R0, @R1, R7",  REGISTER_NOT_ALLOWED, "R0, @R1, R7", 0x3B11, 0x0708);
        ERRT("SINIB", "@R15, @R0, R7", REGISTER_NOT_ALLOWED,      "R0, R7", 0x3A01, 0x07F8);
        ERRT("SINIB", "@R0, @R1, R7",  REGISTER_NOT_ALLOWED, "R0, @R1, R7", 0x3A11, 0x0708);
    }

    // Input, Increment and Repeat
    if (z8001()) {
        TEST("INIR",  "@RR10, @R9, R8", 0x3B90, 0x08A0);
        TEST("INIRB", "@RR10, @R9, R8", 0x3A90, 0x08A0);
        UNKN(                           0x3B90, 0x68A0);
        UNKN(                           0x3A90, 0x78A0);
        UNKN(                           0x3B90, 0x08A2);
        UNKN(                           0x3A90, 0x08A3);
        ERRT("INIR",  "@RR10, @R0, R8", REGISTER_NOT_ALLOWED,       "R0, R8", 0x3B00, 0x08A0);
        ERRT("INIR",  "@RR0, @R9, R8",  REGISTER_NOT_ALLOWED, "RR0, @R9, R8", 0x3B90, 0x0800);
        ERRT("INIRB", "@RR10, @R0, R8", REGISTER_NOT_ALLOWED,       "R0, R8", 0x3A00, 0x08A0);
        ERRT("INIRB", "@RR0, @R9, R8",  REGISTER_NOT_ALLOWED, "RR0, @R9, R8", 0x3A90, 0x0800);
    } else {
        TEST("INIR",  "@R3, @R2, R1", 0x3B20, 0x0130);
        TEST("INIRB", "@R3, @R2, R1", 0x3A20, 0x0130);
        UNKN(                         0x3B20, 0x8130);
        UNKN(                         0x3A20, 0x9130);
        UNKN(                         0x3B20, 0x0134);
        UNKN(                         0x3A20, 0x0135);
        ERRT("INIR",  "@R3, @R0, R1", REGISTER_NOT_ALLOWED,      "R0, R1", 0x3B00, 0x0130);
        ERRT("INIR",  "@R0, @R2, R1", REGISTER_NOT_ALLOWED, "R0, @R2, R1", 0x3B20, 0x0100);
        ERRT("INIRB", "@R3, @R0, R1", REGISTER_NOT_ALLOWED,      "R0, R1", 0x3A00, 0x0130);
        ERRT("INIRB", "@R0, @R2, R1", REGISTER_NOT_ALLOWED, "R0, @R2, R1", 0x3A20, 0x0100);
    }

    // Special Input, Increment, and Repeat
    if (z8001()) {
        TEST("SINIR",  "@RR8, @R7, R6", 0x3B71, 0x0680);
        TEST("SINIRB", "@RR8, @R7, R6", 0x3A71, 0x0680);
        UNKN(                           0x3B71, 0xA680);
        UNKN(                           0x3A71, 0xB680);
        UNKN(                           0x3B71, 0x0686);
        UNKN(                           0x3A71, 0x0687);
        ERRT("SINIR",  "@RR8, @R0, R6", REGISTER_NOT_ALLOWED,       "R0, R6", 0x3B01, 0x0680);
        ERRT("SINIR",  "@RR0, @R7, R6", REGISTER_NOT_ALLOWED, "RR0, @R7, R6", 0x3B71, 0x0600);
        ERRT("SINIRB", "@RR8, @R0, R6", REGISTER_NOT_ALLOWED,       "R0, R6", 0x3A01, 0x0680);
        ERRT("SINIRB", "@RR0, @R7, R6", REGISTER_NOT_ALLOWED, "RR0, @R7, R6", 0x3A71, 0x0600);
    } else {
        TEST("SINIR",  "@R4, @R3, R2", 0x3B31, 0x0240);
        TEST("SINIRB", "@R4, @R3, R2", 0x3A31, 0x0240);
        UNKN(                          0x3B31, 0xC240);
        UNKN(                          0x3A31, 0xD240);
        UNKN(                          0x3B31, 0x0249);
        UNKN(                          0x3A31, 0x024A);
        ERRT("SINIR",  "@R4, @R0, R2", REGISTER_NOT_ALLOWED,      "R0, R2", 0x3B01, 0x0240);
        ERRT("SINIR",  "@R0, @R3, R2", REGISTER_NOT_ALLOWED, "R0, @R3, R2", 0x3B31, 0x0200);
        ERRT("SINIRB", "@R4, @R0, R2", REGISTER_NOT_ALLOWED,      "R0, R2", 0x3A01, 0x0240);
        ERRT("SINIRB", "@R0, @R3, R2", REGISTER_NOT_ALLOWED, "R0, @R3, R2", 0x3A31, 0x0200);
    }
}

static void test_output() {
    // Output
    TEST("OUT",  "@R2, R1",  0x3F21);
    TEST("OUTB", "@R2, RH1", 0x3E21);
    ERRT("OUT",  "@R0, R1",  REGISTER_NOT_ALLOWED, "R0, R1",  0x3F01);
    ERRT("OUTB", "@R0, RH1", REGISTER_NOT_ALLOWED, "R0, RH1", 0x3E01);
    TEST("OUT",  "%1234, R1",  0x3B16, 0x1234);
    TEST("OUTB", "%1234, RH1", 0x3A16, 0x1234);

    // Special Output
    TEST("SOUT",  "%1234, R1",  0x3B17, 0x1234);
    TEST("SOUTB", "%1234, RH1", 0x3A17, 0x1234);

    // Output and Decrement
    if (z8001()) {
        TEST("OUTD",  "@R2, @RR4, R6", 0x3B4A, 0x0628);
        TEST("OUTDB", "@R2, @RR4, R6", 0x3A4A, 0x0628);
        UNKN(                          0x3B4A, 0x1628);
        UNKN(                          0x3A4A, 0x2628);
        UNKN(                          0x3B4A, 0x0629);
        UNKN(                          0x3A4A, 0x062A);
        ERRT("OUTD",  "@R2, @RR0, R6", REGISTER_NOT_ALLOWED,      "RR0, R6", 0x3B0A, 0x0628);
        ERRT("OUTD",  "@R0, @RR4, R6", REGISTER_NOT_ALLOWED, "R0, @RR4, R6", 0x3B4A, 0x0608);
        ERRT("OUTDB", "@R2, @RR0, R6", REGISTER_NOT_ALLOWED,      "RR0, R6", 0x3A0A, 0x0628);
        ERRT("OUTDB", "@R0, @RR4, R6", REGISTER_NOT_ALLOWED, "R0, @RR4, R6", 0x3A4A, 0x0608);
    } else {
        TEST("OUTD",  "@R2, @R1, R0", 0x3B1A, 0x0028);
        TEST("OUTDB", "@R2, @R1, R0", 0x3A1A, 0x0028);
        NMEM("OTDRB", "",         "", 0x3A1A);
        UNKN(                         0x3B1A, 0x3028);
        UNKN(                         0x3A1A, 0x4028);
        UNKN(                         0x3B1A, 0x002B);
        UNKN(                         0x3A1A, 0x002C);
        ERRT("OUTD",  "@R2, @R0, R6", REGISTER_NOT_ALLOWED,      "R0, R6", 0x3B0A, 0x0628);
        ERRT("OUTD",  "@R0, @R1, R6", REGISTER_NOT_ALLOWED, "R0, @R1, R6", 0x3B1A, 0x0608);
        ERRT("OUTDB", "@R2, @R0, R6", REGISTER_NOT_ALLOWED,      "R0, R6", 0x3A0A, 0x0628);
        ERRT("OUTDB", "@R0, @R1, R6", REGISTER_NOT_ALLOWED, "R0, @R1, R6", 0x3A1A, 0x0608);
    }

    // Special Output and Decrement
    if (z8001()) {
        TEST("SOUTD",  "@R3, @RR4, R0", 0x3B4B, 0x0038);
        TEST("SOUTDB", "@R3, @RR4, R0", 0x3A4B, 0x0038);
        UNKN(                           0x3B4B, 0x5038);
        UNKN(                           0x3A4B, 0x6038);
        UNKN(                           0x3B4B, 0x003D);
        UNKN(                           0x3A4B, 0x003E);
        ERRT("SOUTD",  "@R3, @RR0, R7", REGISTER_NOT_ALLOWED,      "RR0, R7", 0x3B0B, 0x0738);
        ERRT("SOUTD",  "@R0, @RR4, R7", REGISTER_NOT_ALLOWED, "R0, @RR4, R7", 0x3B4B, 0x0708);
        ERRT("SOUTDB", "@R3, @RR0, R7", REGISTER_NOT_ALLOWED,      "RR0, R7", 0x3A0B, 0x0738);
        ERRT("SOUTDB", "@R0, @RR4, R7", REGISTER_NOT_ALLOWED, "R0, @RR4, R7", 0x3A4B, 0x0708);
    } else {
        TEST("SOUTD",  "@R3, @R4, R5", 0x3B4B, 0x0538);
        TEST("SOUTDB", "@R3, @R4, R5", 0x3A4B, 0x0538);
        UNKN(                          0x3B4B, 0x7538);
        UNKN(                          0x3A4B, 0x8538);
        UNKN(                          0x3B4B, 0x053F);
        UNKN(                          0x3A4B, 0x0531);
        ERRT("SOUTD",  "@R3, @R0, R5", REGISTER_NOT_ALLOWED,      "R0, R5", 0x3B0B, 0x0538);
        ERRT("SOUTD",  "@R0, @R4, R5", REGISTER_NOT_ALLOWED, "R0, @R4, R5", 0x3B4B, 0x0508);
        ERRT("SOUTDB", "@R3, @R0, R5", REGISTER_NOT_ALLOWED,      "R0, R5", 0x3A0B, 0x0538);
        ERRT("SOUTDB", "@R0, @R4, R5", REGISTER_NOT_ALLOWED, "R0, @R4, R5", 0x3A4B, 0x0508);
    }

    // Output, Decrement, and Repeat
    if (z8001()) {
        TEST("OTDR",  "@R4, @RR6, R8", 0x3B6A, 0x0840);
        TEST("OTDRB", "@R4, @RR6, R8", 0x3A6A, 0x0840);
        UNKN(                          0x3B6A, 0x9840);
        UNKN(                          0x3A6A, 0xA840);
        UNKN(                          0x3B6A, 0x0842);
        UNKN(                          0x3A6A, 0x0843);
        ERRT("OTDR",  "@R4, @RR0, R8", REGISTER_NOT_ALLOWED,      "RR0, R8", 0x3B0A, 0x0840);
        ERRT("OTDR",  "@R0, @RR6, R8", REGISTER_NOT_ALLOWED, "R0, @RR6, R8", 0x3B6A, 0x0800);
        ERRT("OTDRB", "@R4, @RR0, R8", REGISTER_NOT_ALLOWED,      "RR0, R8", 0x3A0A, 0x0840);
        ERRT("OTDRB", "@R0, @RR6, R8", REGISTER_NOT_ALLOWED, "R0, @RR6, R8", 0x3A6A, 0x0800);
    } else {
        TEST("OTDR",  "@R6, @R7, R8", 0x3B7A, 0x0860);
        TEST("OTDRB", "@R6, @R7, R8", 0x3A7A, 0x0860);
        UNKN(                         0x3B7A, 0xB860);
        UNKN(                         0x3A7A, 0xC860);
        UNKN(                         0x3B7A, 0x0864);
        UNKN(                         0x3A7A, 0x0865);
        ERRT("OTDR",  "@R6, @R0, R8", REGISTER_NOT_ALLOWED,      "R0, R8", 0x3B0A, 0x0860);
        ERRT("OTDR",  "@R0, @R7, R8", REGISTER_NOT_ALLOWED, "R0, @R7, R8", 0x3B7A, 0x0800);
        ERRT("OTDRB", "@R6, @R0, R8", REGISTER_NOT_ALLOWED,      "R0, R8", 0x3A0A, 0x0860);
        ERRT("OTDRB", "@R0, @R7, R8", REGISTER_NOT_ALLOWED, "R0, @R7, R8", 0x3A7A, 0x0800);
    }

    // Special Output, Decrement, and Repeat
    if (z8001()) {
        TEST("SOTDR",  "@R8, @RR10, R12", 0x3BAB, 0x0C80);
        TEST("SOTDRB", "@R8, @RR10, R12", 0x3AAB, 0x0C80);
        UNKN(                             0x3BAB, 0xDC80);
        UNKN(                             0x3AAB, 0xEC80);
        UNKN(                             0x3BAB, 0x0C86);
        UNKN(                             0x3AAB, 0x0C87);
        ERRT("SOTDR",  "@R8, @RR0, R12",  REGISTER_NOT_ALLOWED,       "RR0, R12", 0x3B0B, 0x0C80);
        ERRT("SOTDR",  "@R0, @RR10, R12", REGISTER_NOT_ALLOWED, "R0, @RR10, R12", 0x3BAB, 0x0C00);
        ERRT("SOTDRB", "@R8, @RR0, R12",  REGISTER_NOT_ALLOWED,       "RR0, R12", 0x3A0B, 0x0C80);
        ERRT("SOTDRB", "@R0, @RR10, R12", REGISTER_NOT_ALLOWED, "R0, @RR10, R12", 0x3AAB, 0x0C00);
    } else {
        TEST("SOTDR",  "@R9, @R10, R11", 0x3BAB, 0x0B90);
        TEST("SOTDRB", "@R9, @R10, R11", 0x3AAB, 0x0B90);
        UNKN(                            0x3BAB, 0xFB90);
        UNKN(                            0x3AAB, 0x1B90);
        UNKN(                            0x3BAB, 0x0B99);
        UNKN(                            0x3AAB, 0x0B9A);
        ERRT("SOTDR",  "@R9, @R0, R11",  REGISTER_NOT_ALLOWED,       "R0, R11", 0x3B0B, 0x0B90);
        ERRT("SOTDR",  "@R0, @R10, R11", REGISTER_NOT_ALLOWED, "R0, @R10, R11", 0x3BAB, 0x0B00);
        ERRT("SOTDRB", "@R9, @R0, R11",  REGISTER_NOT_ALLOWED,       "R0, R11", 0x3A0B, 0x0B90);
        ERRT("SOTDRB", "@R0, @R10, R11", REGISTER_NOT_ALLOWED, "R0, @R10, R11", 0x3AAB, 0x0B00);
    }

    // Output and Increment
    if (z8001()) {
        TEST("OUTI",  "@R14, @RR2, R1", 0x3B22, 0x01E8);
        TEST("OUTIB", "@R14, @RR2, R1", 0x3A22, 0x01E8);
        UNKN(                           0x3B22, 0x21E8);
        UNKN(                           0x3A22, 0x31E8);
        UNKN(                           0x3B22, 0x01EB);
        UNKN(                           0x3A22, 0x01EC);
        ERRT("OUTI",  "@R14, @RR0, R1", REGISTER_NOT_ALLOWED,      "RR0, R1", 0x3B02, 0x01E8);
        ERRT("OUTI",  "@R0, @RR2, R1",  REGISTER_NOT_ALLOWED, "R0, @RR2, R1", 0x3B22, 0x0108);
        ERRT("OUTIB", "@R14, @RR0, R1", REGISTER_NOT_ALLOWED,      "RR0, R1", 0x3A02, 0x01E8);
        ERRT("OUTIB", "@R0, @RR2, R1",  REGISTER_NOT_ALLOWED, "R0, @RR2, R1", 0x3A22, 0x0108);
    } else {
        TEST("OUTI",  "@R12, @R13, R14", 0x3BD2, 0x0EC8);
        TEST("OUTIB", "@R12, @R13, R14", 0x3AD2, 0x0EC8);
        UNKN(                            0x3BD2, 0x4EC8);
        UNKN(                            0x3AD2, 0x5EC8);
        UNKN(                            0x3BD2, 0x0ECD);
        UNKN(                            0x3AD2, 0x0ECE);
        ERRT("OUTI",  "@R12, @R0, R14", REGISTER_NOT_ALLOWED,       "R0, R14", 0x3B02, 0x0EC8);
        ERRT("OUTI",  "@R0, @R13, R14", REGISTER_NOT_ALLOWED, "R0, @R13, R14", 0x3BD2, 0x0E08);
        ERRT("OUTIB", "@R12, @R0, R14", REGISTER_NOT_ALLOWED,       "R0, R14", 0x3A02, 0x0EC8);
        ERRT("OUTIB", "@R0, @R13, R14", REGISTER_NOT_ALLOWED, "R0, @R13, R14", 0x3AD2, 0x0E08);
    }

    // Special Output and Increment
    if (z8001()) {
        TEST("SOUTI",  "@R10, @RR12, R11", 0x3BC3, 0x0BA8);
        TEST("SOUTIB", "@R10, @RR12, R11", 0x3AC3, 0x0BA8);
        UNKN(                              0x3BC3, 0x6BA8);
        UNKN(                              0x3AC3, 0x7BA8);
        UNKN(                              0x3BC3, 0x0BAF);
        UNKN(                              0x3AC3, 0x0BA1);
        ERRT("SOUTI",  "@R10, @RR0, R11", REGISTER_NOT_ALLOWED,       "RR0, R11", 0x3B03, 0x0BA8);
        ERRT("SOUTI",  "@R0, @RR12, R11", REGISTER_NOT_ALLOWED, "R0, @RR12, R11", 0x3BC3, 0x0B08);
        ERRT("SOUTIB", "@R10, @RR0, R11", REGISTER_NOT_ALLOWED,       "RR0, R11", 0x3A03, 0x0BA8);
        ERRT("SOUTIB", "@R0, @RR12, R11", REGISTER_NOT_ALLOWED, "R0, @RR12, R11", 0x3AC3, 0x0B08);
    } else {
        TEST("SOUTI",  "@R15, @R1, R0", 0x3B13, 0x00F8);
        TEST("SOUTIB", "@R15, @R1, R0", 0x3A13, 0x00F8);
        UNKN(                           0x3B13, 0x80F8);
        UNKN(                           0x3A13, 0x90F8);
        UNKN(                           0x3B13, 0x00F2);
        UNKN(                           0x3A13, 0x00F3);
        ERRT("SOUTI",  "@R15, @R0, R8", REGISTER_NOT_ALLOWED,      "R0, R8", 0x3B03, 0x08F8);
        ERRT("SOUTI",  "@R0, @R1, R8",  REGISTER_NOT_ALLOWED, "R0, @R1, R8", 0x3B13, 0x0808);
        ERRT("SOUTIB", "@R15, @R0, R8", REGISTER_NOT_ALLOWED,      "R0, R8", 0x3A03, 0x08F8);
        ERRT("SOUTIB", "@R0, @R1, R8",  REGISTER_NOT_ALLOWED, "R0, @R1, R8", 0x3A13, 0x0808);
    }

    // Output, Increment and Repeat
    if (z8001()) {
        TEST("OTIR",  "@R9, @RR10, R8", 0x3BA2, 0x0890);
        TEST("OTIRB", "@R9, @RR10, R8", 0x3AA2, 0x0890);
        UNKN(                           0x3BA2, 0xA890);
        UNKN(                           0x3AA2, 0xB890);
        UNKN(                           0x3BA2, 0x0894);
        UNKN(                           0x3AA2, 0x0895);
        ERRT("OTIR",  "@R9, @RR0, R8",  REGISTER_NOT_ALLOWED,       "RR0, R8", 0x3B02, 0x0890);
        ERRT("OTIR",  "@R0, @RR10, R8", REGISTER_NOT_ALLOWED, "R0, @RR10, R8", 0x3BA2, 0x0800);
        ERRT("OTIRB", "@R9, @RR0, R8",  REGISTER_NOT_ALLOWED,       "RR0, R8", 0x3A02, 0x0890);
        ERRT("OTIRB", "@R0, @RR10, R8", REGISTER_NOT_ALLOWED, "R0, @RR10, R8", 0x3AA2, 0x0800);
    } else {
        TEST("OTIR",  "@R3, @R2, R1", 0x3B22, 0x0130);
        TEST("OTIRB", "@R3, @R2, R1", 0x3A22, 0x0130);
        UNKN(                         0x3B22, 0xC130);
        UNKN(                         0x3A22, 0xD130);
        UNKN(                         0x3B22, 0x0136);
        UNKN(                         0x3A22, 0x0137);
        ERRT("OTIR",  "@R3, @R0, R1", REGISTER_NOT_ALLOWED,      "R0, R1", 0x3B02, 0x0130);
        ERRT("OTIR",  "@R0, @R2, R1", REGISTER_NOT_ALLOWED, "R0, @R2, R1", 0x3B22, 0x0100);
        ERRT("OTIRB", "@R3, @R0, R1", REGISTER_NOT_ALLOWED,      "R0, R1", 0x3A02, 0x0130);
        ERRT("OTIRB", "@R0, @R2, R1", REGISTER_NOT_ALLOWED, "R0, @R2, R1", 0x3A22, 0x0100);
    }

    // Special Output, Increment, and Repeat
    if (z8001()) {
        TEST("SOTIR",  "@R7, @RR8, R6", 0x3B83, 0x0670);
        TEST("SOTIRB", "@R7, @RR8, R6", 0x3A83, 0x0670);
        UNKN(                           0x3B83, 0xE670);
        UNKN(                           0x3A83, 0xF670);
        UNKN(                           0x3B83, 0x0679);
        UNKN(                           0x3A83, 0x067A);
        ERRT("SOTIR",  "@R7, @RR0, R6", REGISTER_NOT_ALLOWED,      "RR0, R6", 0x3B03, 0x0670);
        ERRT("SOTIR",  "@R0, @RR8, R6", REGISTER_NOT_ALLOWED, "R0, @RR8, R6", 0x3B83, 0x0600);
        ERRT("SOTIRB", "@R7, @RR0, R6", REGISTER_NOT_ALLOWED,      "RR0, R6", 0x3A03, 0x0670);
        ERRT("SOTIRB", "@R0, @RR8, R6", REGISTER_NOT_ALLOWED, "R0, @RR8, R6", 0x3A83, 0x0600);
    } else {
        TEST("SOTIR",  "@R4, @R3, R2", 0x3B33, 0x0240);
        TEST("SOTIRB", "@R4, @R3, R2", 0x3A33, 0x0240);
        UNKN(                          0x3B33, 0x1240);
        UNKN(                          0x3A33, 0x2240);
        UNKN(                          0x3B33, 0x024B);
        UNKN(                          0x3A33, 0x024C);
        ERRT("SOTIR",  "@R4, @R0, R2", REGISTER_NOT_ALLOWED,      "R0, R2", 0x3B03, 0x0240);
        ERRT("SOTIR",  "@R0, @R3, R2", REGISTER_NOT_ALLOWED, "R0, @R3, R2", 0x3B33, 0x0200);
        ERRT("SOTIRB", "@R4, @R0, R2", REGISTER_NOT_ALLOWED,      "R0, R2", 0x3A03, 0x0240);
        ERRT("SOTIRB", "@R0, @R3, R2", REGISTER_NOT_ALLOWED, "R0, @R3, R2", 0x3A33, 0x0200);
    }
}

static void test_cpu_conrtol() {
    // Complement Flag
    ERRT("COMFLG", "", OPCODE_HAS_NO_EFFECT, "", 0x8D05);
    TEST("COMFLG", "P",       0x8D15);
    TEST("COMFLG", "S",       0x8D25);
    TEST("COMFLG", "S,P",     0x8D35);
    TEST("COMFLG", "Z",       0x8D45);
    TEST("COMFLG", "Z,P",     0x8D55);
    TEST("COMFLG", "Z,S",     0x8D65);
    TEST("COMFLG", "Z,S,P",   0x8D75);
    TEST("COMFLG", "C",       0x8D85);
    TEST("COMFLG", "C,P",     0x8D95);
    TEST("COMFLG", "C,S",     0x8DA5);
    TEST("COMFLG", "C,S,P",   0x8DB5);
    TEST("COMFLG", "C,Z",     0x8DC5);
    TEST("COMFLG", "C,Z,P",   0x8DD5);
    TEST("COMFLG", "C,Z,S",   0x8DE5);
    TEST("COMFLG", "C,Z,S,P", 0x8DF5);

    // Disassembler Interrupt
    TEST("DI", "VI,NVI", 0x7C00);
    TEST("DI", "VI",     0x7C01);
    TEST("DI", "NVI",    0x7C02);
    ERRT("DI", "", OPCODE_HAS_NO_EFFECT, "", 0x7C03);

    // Enable Interrupt
    TEST("EI", "VI,NVI", 0x7C04);
    TEST("EI", "VI",     0x7C05);
    TEST("EI", "NVI",    0x7C06);
    ERRT("EI", "", OPCODE_HAS_NO_EFFECT, "", 0x7C07);

    // Halt
    TEST("HALT", "", 0x7A00);

    // Load Control Register
    ERRT("LDCTL", "R14, FLAGS", ILLEGAL_SIZE, "FLAGS", 0x7DE1);
    TEST("LDCTL", "R13, FCW",     0x7DD2);
    TEST("LDCTL", "R12, REFRESH", 0x7DC3);
    if (z8001()) {
        TEST("LDCTL", "R11, PSAPSEG", 0x7DB4);
        TEST("LDCTL", "R10, PSAPOFF", 0x7DA5);
        TEST("LDCTL", "R9, NSPSEG",   0x7D96);
        TEST("LDCTL", "R8, NSPOFF",   0x7D87);
    } else {
        ERRT("LDCTL", "R11, ", UNKNOWN_REGISTER, "", 0x7DB4);
        TEST("LDCTL", "R10, PSAP",                   0x7DA5);
        ERRT("LDCTL", "R9, ",  UNKNOWN_REGISTER, "", 0x7D96);
        TEST("LDCTL", "R8, NSP",                     0x7D87);
    }
    ERRT("LDCTL", "FLAGS, R6", ILLEGAL_SIZE, "FLAGS, R6", 0x7D69);
    TEST("LDCTL", "FCW, R5",                 0x7D5A);
    TEST("LDCTL", "REFRESH, R4",             0x7D4B);
    if (z8001()) {
        TEST("LDCTL", "PSAPSEG, R3", 0x7D3C);
        TEST("LDCTL", "PSAPOFF, R2", 0x7D2D);
        TEST("LDCTL", "NSPSEG, R1",  0x7D1E);
        TEST("LDCTL", "NSPOFF, R0",  0x7D0F);
    } else {
        ERRT("LDCTL", ", R3", UNKNOWN_REGISTER, ", R3", 0x7D3C);
        TEST("LDCTL", "PSAP, R2",                       0x7D2D);
        ERRT("LDCTL", ", R1", UNKNOWN_REGISTER, ", R1", 0x7D1E);
        TEST("LDCTL", "NSP, R0",                        0x7D0F);
    }
    TEST("LDCTLB", "RL6, FLAGS", 0x8CE1);
    TEST("LDCTLB", "FLAGS, RH6", 0x8C69);

    // Load Program Status
    if (z8001()) {
        TEST("LDPS", "@RR2",          0x3920);
        TEST("LDPS", "|%120034|",     0x7900, 0x1234);
        TEST("LDPS", "%561234",       0x7900, 0xD600, 0x1234);
        TEST("LDPS", "|%120034|(R1)", 0x7910, 0x1234);
        TEST("LDPS", "%561234(R1)",   0x7910, 0xD600, 0x1234);
    } else {
        TEST("LDPS", "@R2",       0x3920);
        TEST("LDPS", "%1234",     0x7900, 0x1234);
        TEST("LDPS", "%1234(R1)", 0x7910, 0x1234);
    }

    // Multi-Micro Bit Test, Request, Reset, and Set
    TEST("MBIT", "", 0x7B0A);
    TEST("MRES", "", 0x7B09);
    TEST("MSET", "", 0x7B08);

    // No Operation
    TEST("NOP",  "", 0x8D07);

    // Reset Flag
    ERRT("RESFLG", "", OPCODE_HAS_NO_EFFECT, "", 0x8D03);
    TEST("RESFLG", "P",       0x8D13);
    TEST("RESFLG", "S",       0x8D23);
    TEST("RESFLG", "S,P",     0x8D33);
    TEST("RESFLG", "Z",       0x8D43);
    TEST("RESFLG", "Z,P",     0x8D53);
    TEST("RESFLG", "Z,S",     0x8D63);
    TEST("RESFLG", "Z,S,P",   0x8D73);
    TEST("RESFLG", "C",       0x8D83);
    TEST("RESFLG", "C,P",     0x8D93);
    TEST("RESFLG", "C,S",     0x8DA3);
    TEST("RESFLG", "C,S,P",   0x8DB3);
    TEST("RESFLG", "C,Z",     0x8DC3);
    TEST("RESFLG", "C,Z,P",   0x8DD3);
    TEST("RESFLG", "C,Z,S",   0x8DE3);
    TEST("RESFLG", "C,Z,S,P", 0x8DF3);

    // Set Flag
    ERRT("SETFLG", "", OPCODE_HAS_NO_EFFECT, "", 0x8D01);
    TEST("SETFLG", "P",       0x8D11);
    TEST("SETFLG", "S",       0x8D21);
    TEST("SETFLG", "S,P",     0x8D31);
    TEST("SETFLG", "Z",       0x8D41);
    TEST("SETFLG", "Z,P",     0x8D51);
    TEST("SETFLG", "Z,S",     0x8D61);
    TEST("SETFLG", "Z,S,P",   0x8D71);
    TEST("SETFLG", "C",       0x8D81);
    TEST("SETFLG", "C,P",     0x8D91);
    TEST("SETFLG", "C,S",     0x8DA1);
    TEST("SETFLG", "C,S,P",   0x8DB1);
    TEST("SETFLG", "C,Z",     0x8DC1);
    TEST("SETFLG", "C,Z,P",   0x8DD1);
    TEST("SETFLG", "C,Z,S",   0x8DE1);
    TEST("SETFLG", "C,Z,S,P", 0x8DF1);
}

static void test_direct_address() {
    disassembler.setOption("short-direct", "on");
    TEST("CLR", "|%120034|",     0x4D08, 0x1234);
    TEST("CLR",  "%561234",      0x4D08, 0xD600, 0x1234);
    TEST("CLR", "|%120034|(R2)", 0x4D28, 0x1234);
    TEST("CLR",  "%561234(R2)",  0x4D28, 0xD600, 0x1234);

    disassembler.setOption("segmented-addr", "on");
    TEST("CLR", "|<<18>>%0034|",     0x4D08, 0x1234);
    TEST("CLR",  "<<%56>>%1234",     0x4D08, 0xD600, 0x1234);
    TEST("CLR", "|<<18>>%0034|(R2)", 0x4D28, 0x1234);
    TEST("CLR",  "<<%56>>%1234(R2)", 0x4D28, 0xD600, 0x1234);

    disassembler.setOption("short-direct", "disable");
    disassembler.setOption("segmented-addr", "off");
    TEST("CLR", "%120034",     0x4D08, 0x1234);
    TEST("CLR", "%561234",     0x4D08, 0xD600, 0x1234);
    TEST("CLR", "%120034(R2)", 0x4D28, 0x1234);
    TEST("CLR", "%561234(R2)", 0x4D28, 0xD600, 0x1234);

    disassembler.setOption("segmented-addr", "on");
    TEST("CLR", "<<18>>%0034",      0x4D08, 0x1234);
    TEST("CLR", "<<%56>>%1234",     0x4D08, 0xD600, 0x1234);
    TEST("CLR", "<<18>>%0034(R2)",  0x4D28, 0x1234);
    TEST("CLR", "<<%56>>%1234(R2)", 0x4D28, 0xD600, 0x1234);
}

// clang-format on

void run_tests(const char *cpu) {
    disassembler.setCpu(cpu);
    RUN_TEST(test_load_and_exchange);
    RUN_TEST(test_arithmetic);
    RUN_TEST(test_logical);
    RUN_TEST(test_program_control);
    RUN_TEST(test_bit_manipulation);
    RUN_TEST(test_rotate);
    RUN_TEST(test_shift);
    RUN_TEST(test_compare_block);
    RUN_TEST(test_block_transfer);
    RUN_TEST(test_compare_string);
    RUN_TEST(test_translation);
    RUN_TEST(test_input);
    RUN_TEST(test_output);
    RUN_TEST(test_cpu_conrtol);
    if (z8001()) {
        RUN_TEST(test_direct_address);
    }
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

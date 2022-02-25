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

#include "asm_z8000.h"
#include "test_asm_helper.h"

using namespace libasm;
using namespace libasm::z8000;
using namespace libasm::test;

AsmZ8000 asm8000;
Assembler &assembler(asm8000);

static bool z8001() {
    return strcmp_P("Z8001", assembler.cpu_P()) == 0;
}

static void set_up() {
    assembler.reset();
    asm8000.setAutoShortDirect(true);
}

static void tear_down() {
    symtab.reset();
}

// clang-format off
void test_cpu() {
    EQUALS("cpu 8001", true,    assembler.setCpu("Z8001"));
    EQUALS_P("cpu 8001", "Z8001", assembler.cpu_P());

    EQUALS("cpu Z8002", true,    assembler.setCpu("Z8002"));
    EQUALS_P("cpu Z8002", "Z8002", assembler.cpu_P());
}

static void test_load_and_exchange() {
    // Clear
    TEST("CLR  R2",  0x8D28);
    TEST("CLRB RH2", 0x8C28);
    if (z8001()) {
        TEST("CLR  @RR2",         0x0D28);
        TEST("CLR  0x120034",     0x4D08, 0x1234);
        TEST("CLR  0x561234",     0x4D08, 0xD600, 0x1234);
        TEST("CLR  0x7FFFFF",     0x4D08, 0xFF00, 0xFFFF);
        ERRT("CLR  0x800000",     OVERFLOW_RANGE);
        TEST("CLR  0x120034(R2)", 0x4D28, 0x1234);
        TEST("CLR  0x561234(R2)", 0x4D28, 0xD600, 0x1234);
        TEST("CLR  0x7FFFFF(R2)", 0x4D28, 0xFF00, 0xFFFF);
        ERRT("CLR  0x800000(R2)", OVERFLOW_RANGE);
        TEST("CLRB @RR2",         0x0C28);
        TEST("CLRB 0x120034",     0x4C08, 0x1234);
        TEST("CLRB 0x561234",     0x4C08, 0xD600, 0x1234);
        TEST("CLRB 0x120034(R2)", 0x4C28, 0x1234);
        TEST("CLRB 0x561234(R2)", 0x4C28, 0xD600, 0x1234);
    } else {
        TEST("CLR  @R2",         0x0D28);
        TEST("CLR  0x1234",      0x4D08, 0x1234);
        TEST("CLR  0xFFFF",      0x4D08, 0xFFFF);
        ERRT("CLR  0x10000",     OVERFLOW_RANGE);
        TEST("CLR  0x1234(R2)",  0x4D28, 0x1234);
        TEST("CLR  0xFFFF(R2)",  0x4D28, 0xFFFF);
        ERRT("CLR  0x10000(R2)", OVERFLOW_RANGE);
        TEST("CLRB @R2",         0x0C28);
        TEST("CLRB 0x1234",      0x4C08, 0x1234);
        TEST("CLRB 0x1234(R2)",  0x4C28, 0x1234);
    }

    // Exchange
    TEST("EX  R1,R2",   0xAD21);
    TEST("EXB RL1,RH2", 0xAC29);
    if (z8001()) {
        TEST("EX  R9,@RR2",          0x2D29);
        TEST("EX  R9,0x120034",      0x6D09, 0x1234);
        TEST("EX  R9,0x561234",      0x6D09, 0xD600, 0x1234);
        TEST("EX  R9,0x120034(R2)",  0x6D29, 0x1234);
        TEST("EX  R9,0x561234(R2)",  0x6D29, 0xD600, 0x1234);
        TEST("EXB RL1,@RR2",         0x2C29);
        TEST("EXB RL1,0x120034",     0x6C09, 0x1234);
        TEST("EXB RL1,0x561234",     0x6C09, 0xD600, 0x1234);
        TEST("EXB RL1,0x120034(R2)", 0x6C29, 0x1234);
        TEST("EXB RL1,0x561234(R2)", 0x6C29, 0xD600, 0x1234);
    } else {
        TEST("EX  R9,@R2",         0x2D29);
        TEST("EX  R9,0x1234",      0x6D09, 0x1234);
        TEST("EX  R9,0x1234(R2)",  0x6D29, 0x1234);
        TEST("EXB RL1,@R2",        0x2C29);
        TEST("EXB RL1,0x1234",     0x6C09, 0x1234);
        TEST("EXB RL1,0x1234(R2)", 0x6C29, 0x1234);
    }

    // Load
    TEST("LD  R2,R8",   0xA182);
    TEST("LDB RH2,RL0", 0xA082);
    TEST("LDL RR2,RR8", 0x9482);
    TEST("LD  R2,#0x5678",      0x2102, 0x5678);
    TEST("LDB RH2,#0x56",       0xC256);
    TEST("LDL RR2,#0x12345678", 0x1402, 0x1234, 0x5678);
    if (z8001()) {
        TEST("LD  R9,@RR2",          0x2129);
        TEST("LD  R9,0x120034",      0x6109, 0x1234);
        TEST("LD  R9,0x561234",      0x6109, 0xD600, 0x1234);
        TEST("LD  R9,0x120034(R2)",  0x6129, 0x1234);
        TEST("LD  R9,0x561234(R2)",  0x6129, 0xD600, 0x1234);
        TEST("LD  R9,RR2(#0x1234)",  0x3129, 0x1234);
        TEST("LD  R9,RR2(#-1)",      0x3129, 0xFFFF);
        TEST("LD  R9,RR2(#32767)",   0x3129, 0x7FFF);
        TEST("LD  R9,RR2(#-32768)",  0x3129, 0x8000);
        ERRT("LD  R9,RR2(#32768)",   OVERFLOW_RANGE);
        ERRT("LD  R9,RR2(#-32769)",  OVERFLOW_RANGE);
        TEST("LD  R9,RR2(R4)",       0x7129, 0x0400);
        TEST("LDB RL1,@RR2",         0x2029);
        TEST("LDB RL1,0x120034",     0x6009, 0x1234);
        TEST("LDB RL1,0x561234",     0x6009, 0xD600, 0x1234);
        TEST("LDB RL1,0x120034(R2)", 0x6029, 0x1234);
        TEST("LDB RL1,0x561234(R2)", 0x6029, 0xD600, 0x1234);
        TEST("LDB RL1,RR2(#0x1234)", 0x3029, 0x1234);
        TEST("LDB RL1,RR2(R4)",      0x7029, 0x0400);
        TEST("LDL RR2,@RR8",         0x1482);
        TEST("LDL RR2,0x120034",     0x5402, 0x1234);
        TEST("LDL RR2,0x561234",     0x5402, 0xD600, 0x1234);
        TEST("LDL RR2,0x120034(R8)", 0x5482, 0x1234);
        TEST("LDL RR2,0x561234(R8)", 0x5482, 0xD600, 0x1234);
        TEST("LDL RR2,RR8(#0x1234)", 0x3582, 0x1234);
        TEST("LDL RR2,RR8(R4)",      0x7582, 0x0400);
    } else {
        TEST("LD  R9,@R2",          0x2129);
        TEST("LD  R9,0x1234",       0x6109, 0x1234);
        TEST("LD  R9,0x1234(R2)",   0x6129, 0x1234);
        TEST("LD  R9,R2(#0x1234)",  0x3129, 0x1234);
        TEST("LD  R9,R2(R4)",       0x7129, 0x0400);
        TEST("LDB RL1,@R2",         0x2029);
        TEST("LDB RL1,0x1234",      0x6009, 0x1234);
        TEST("LDB RL1,0x1234(R2)",  0x6029, 0x1234);
        TEST("LDB RL1,R2(#0x1234)", 0x3029, 0x1234);
        TEST("LDB RL1,R2(R4)",      0x7029, 0x0400);
        TEST("LDL RR2,@R8",         0x1482);
        TEST("LDL RR2,0x1234",      0x5402, 0x1234);
        TEST("LDL RR2,0x1234(R8)",  0x5482, 0x1234);
        TEST("LDL RR2,R8(#0x1234)", 0x3582, 0x1234);
        TEST("LDL RR2,R8(R4)",      0x7582, 0x0400);
    }
    if (z8001()) {
        TEST("LD  @RR2,R9",          0x2F29);
        TEST("LD  0x120034,R9",      0x6F09, 0x1234);
        TEST("LD  0x561234,R9",      0x6F09, 0xD600, 0x1234);
        TEST("LD  0x120034(R2),R9",  0x6F29, 0x1234);
        TEST("LD  0x561234(R2),R9",  0x6F29, 0xD600, 0x1234);
        TEST("LD  RR2(#0x1234),R9",  0x3329, 0x1234);
        TEST("LD  RR2(R4),R9",       0x7329, 0x0400);
        TEST("LDB @RR2,RL1",         0x2E29);
        TEST("LDB 0x120034,RL1",     0x6E09, 0x1234);
        TEST("LDB 0x561234,RL1",     0x6E09, 0xD600, 0x1234);
        TEST("LDB 0x120034(R2),RL1", 0x6E29, 0x1234);
        TEST("LDB 0x561234(R2),RL1", 0x6E29, 0xD600, 0x1234);
        TEST("LDB RR2(#0x1234),RL1", 0x3229, 0x1234);
        TEST("LDB RR2(R4),RL1",      0x7229, 0x0400);
        TEST("LDL @RR8,RR2",         0x1D82);
        TEST("LDL 0x120034,RR2",     0x5D02, 0x1234);
        TEST("LDL 0x561234,RR2",     0x5D02, 0xD600, 0x1234);
        TEST("LDL 0x120034(R8),RR2", 0x5D82, 0x1234);
        TEST("LDL 0x561234(R8),RR2", 0x5D82, 0xD600, 0x1234);
        TEST("LDL RR8(#0x1234),RR2", 0x3782, 0x1234);
        TEST("LDL RR8(R4),RR2",      0x7782, 0x0400);
    } else {
        TEST("LD  @R2,R9",          0x2F29);
        TEST("LD  0x1234,R9",       0x6F09, 0x1234);
        TEST("LD  0x1234(R2),R9",   0x6F29, 0x1234);
        TEST("LD  R2(#0x1234),R9",  0x3329, 0x1234);
        TEST("LD  R2(R4),R9",       0x7329, 0x0400);
        TEST("LDB @R2,RL1",         0x2E29);
        TEST("LDB 0x1234,RL1",      0x6E09, 0x1234);
        TEST("LDB 0x1234(R2),RL1",  0x6E29, 0x1234);
        TEST("LDB R2(#0x1234),RL1", 0x3229, 0x1234);
        TEST("LDB R2(R4),RL1",      0x7229, 0x0400);
        TEST("LDL @R8,RR2",         0x1D82);
        TEST("LDL 0x1234,RR2",      0x5D02, 0x1234);
        TEST("LDL 0x1234(R8),RR2",  0x5D82, 0x1234);
        TEST("LDL R8(#0x1234),RR2", 0x3782, 0x1234);
        TEST("LDL R8(R4),RR2",      0x7782, 0x0400);
    }
    if (z8001()) {
        TEST("LD  R2,#0x5678",           0x2102, 0x5678);
        TEST("LD  @RR2,#0x5678",         0x0D25, 0x5678);
        TEST("LD  0x120034,#0x5678",     0x4D05, 0x1234, 0x5678);
        TEST("LD  0x561234,#0x5678",     0x4D05, 0xD600, 0x1234, 0x5678);
        TEST("LD  0x120034(R2),#0x5678", 0x4D25, 0x1234, 0x5678);
        TEST("LD  0x561234(R2),#0x5678", 0x4D25, 0xD600, 0x1234, 0x5678);
        TEST("LDB RH2,#0x56",            0xC256);
        TEST("LDB @RR2,#0x56",           0x0C25, 0x5656);
        TEST("LDB 0x120034,#0x56",       0x4C05, 0x1234, 0x5656);
        TEST("LDB 0x561234,#0x56",       0x4C05, 0xD600, 0x1234, 0x5656);
        TEST("LDB 0x120034(R2),#0x56",   0x4C25, 0x1234, 0x5656);
        TEST("LDB 0x561234(R2),#0x56",   0x4C25, 0xD600, 0x1234, 0x5656);
    } else {
        TEST("LD  R2,#0x5678",         0x2102, 0x5678);
        TEST("LD  @R2,#0x5678",        0x0D25, 0x5678);
        TEST("LD  0x1234,#0x5678",     0x4D05, 0x1234, 0x5678);
        TEST("LD  0x1234(R2),#0x5678", 0x4D25, 0x1234, 0x5678);
        TEST("LDB RH2,#0x56",          0xC256);
        TEST("LDB @R2,#0x56",          0x0C25, 0x5656);
        TEST("LDB 0x1234,#0x56",       0x4C05, 0x1234, 0x5656);
        TEST("LDB 0x1234(R2),#0x56",   0x4C25, 0x1234, 0x5656);
    }

    // Load Address
    if (z8001()) {
        TEST("LDA RR8,0x120034",     0x7608, 0x1234);
        TEST("LDA RR8,0x561234",     0x7608, 0xD600, 0x1234);
        TEST("LDA RR8,0x120034(R2)", 0x7628, 0x1234);
        TEST("LDA RR8,0x561234(R2)", 0x7628, 0xD600, 0x1234);
        TEST("LDA RR8,RR2(#0x1234)", 0x3428, 0x1234);
        TEST("LDA RR8,RR2(R4)",      0x7428, 0x0400);
    } else {
        TEST("LDA R8,0x1234",      0x7608, 0x1234);
        TEST("LDA R8,0x1234(R2)",  0x7628, 0x1234);
        TEST("LDA R8,R2(#0x1234)", 0x3428, 0x1234);
        TEST("LDA R8,R2(R4)",      0x7428, 0x0400);
    }

    // Load Address Relative
    if (z8001()) {
        ATEST(0x002000, "LDAR RR8,0x002000", 0x3408, 0xFFFC);
        ATEST(0x002000, "LDAR RR8,0x002004", 0x3408, 0x0000);
        ATEST(0x102000, "LDAR RR8,0x0FA004", 0x3408, 0x8000);
        ATEST(0x002000, "LDAR RR8,0x00A003", 0x3408, 0x7FFF);
    } else {
        ATEST(0x2000, "LDAR R8,0x2000", 0x3408, 0xFFFC);
        ATEST(0x2000, "LDAR R8,0x2004", 0x3408, 0x0000);
        ATEST(0xA000, "LDAR R8,0x2004", 0x3408, 0x8000);
        ATEST(0x2000, "LDAR R8,0xA003", 0x3408, 0x7FFF);
    }

    // Load Constant
    TEST("LDK R1,#0",  0xBD10);
    TEST("LDK R1,#15", 0xBD1F);

    // Load Multiple
    if (z8001()) {
        TEST("LDM R15,@RR2,#1",        0x1C21, 0x0F00);
        ERRT("LDM R15,@RR2,#2",        OVERFLOW_RANGE);
        TEST("LDM R1,@RR2,#2",         0x1C21, 0x0101);
        TEST("LDM R13,0x120034,#3",    0x5C01, 0x0D02, 0x1234);
        TEST("LDM R12,0x561234,#4",    0x5C01, 0x0C03, 0xD600, 0x1234);
        TEST("LDM R1,0x120034(R2),#5", 0x5C21, 0x0104, 0x1234);
        TEST("LDM R1,0x561234(R2),#6", 0x5C21, 0x0105, 0xD600, 0x1234);
        TEST("LDM @RR2,R15,#1",        0x1C29, 0x0F00);
        ERRT("LDM @RR2,R15,#2",        OVERFLOW_RANGE);
        TEST("LDM @RR2,R1,#2",         0x1C29, 0x0101);
        TEST("LDM 120034H,R13,#3",     0x5C09, 0x0D02, 0x1234);
        TEST("LDM 561234H,R12,#4",     0x5C09, 0x0C03, 0xD600, 0x1234);
        TEST("LDM 120034H(R2),R1,#5",  0x5C29, 0x0104, 0x1234);
        TEST("LDM 561234H(R2),R0,#16", 0x5C29, 0x000F, 0xD600, 0x1234);
    } else {
        TEST("LDM R9,@R2,#7",         0x1C21, 0x0906);
        ERRT("LDM R9,@R2,#8",         OVERFLOW_RANGE);
        TEST("LDM R1,@R2,#8",         0x1C21, 0x0107);
        TEST("LDM R7,0x1234,#9",      0x5C01, 0x0708, 0x1234);
        TEST("LDM R1,0x1234(R2),#15", 0x5C21, 0x010E, 0x1234);
        TEST("LDM @R2,R9,#7",        0x1C29, 0x0906);
        ERRT("LDM @R2,R9,#8",        OVERFLOW_RANGE);
        TEST("LDM @R2,R1,#8",        0x1C29, 0x0107);
        TEST("LDM 1234H,R7,#9",      0x5C09, 0x0708, 0x1234);
        TEST("LDM 1234H(R2),R0,#16", 0x5C29, 0x000F, 0x1234);
    }

    // Load Relative
    if (z8001()) {
        ATEST(0x2000, "LDR  R1,0x002000",  0x3101, 0xFFFC);
        ATEST(0x2000, "LDR  0x002000,R1",  0x3301, 0xFFFC);
        ATEST(0xA000, "LDR  0x002004,R1",  0x3301, 0x8000);
        AERRT(0xA000, "LDR  0x002003,R1",  OPERAND_TOO_FAR);
        ATEST(0x2000, "LDR  0x00A003,R1",  0x3301, 0x7FFF);
        AERRT(0x2000, "LDR  0x00A004,R1",  OPERAND_TOO_FAR);
        ATEST(0x2000, "LDRB RH1,0x002000", 0x3001, 0xFFFC);
        ATEST(0x2000, "LDRB 0x002000,RH1", 0x3201, 0xFFFC);
        ATEST(0x2000, "LDRL RR2,0x002000", 0x3502, 0xFFFC);
        ATEST(0x2000, "LDRL 0x002000,RR2", 0x3702, 0xFFFC);
    } else {
        ATEST(0x2000, "LDR  R1,0x2000",  0x3101, 0xFFFC);
        ATEST(0x2000, "LDR  0x2000,R1",  0x3301, 0xFFFC);
        ATEST(0xA000, "LDR  0x2004,R1",  0x3301, 0x8000);
        AERRT(0xA000, "LDR  0x2003,R1",  OPERAND_TOO_FAR);
        ATEST(0x2000, "LDR  0xA003,R1",  0x3301, 0x7FFF);
        AERRT(0x2000, "LDR  0xA004,R1",  OPERAND_TOO_FAR);
        ATEST(0x2000, "LDRB RH1,0x2000", 0x3001, 0xFFFC);
        ATEST(0x2000, "LDRB 0x2000,RH1", 0x3201, 0xFFFC);
        ATEST(0x2000, "LDRL RR2,0x2000", 0x3502, 0xFFFC);
        ATEST(0x2000, "LDRL 0x2000,RR2", 0x3702, 0xFFFC);
    }

    // Pop
    if (z8001()) {
        TEST("POP  R2,@RR4",           0x9742);
        TEST("POP  @RR2,@RR4",         0x1742);
        TEST("POP  0x120034,@RR4",     0x5740, 0x1234);
        TEST("POP  0x561234,@RR4",     0x5740, 0xD600, 0x1234);
        TEST("POP  0x120034(R2),@RR4", 0x5742, 0x1234);
        TEST("POP  0x561234(R2),@RR4", 0x5742, 0xD600, 0x1234);
        TEST("POPL RR2,@RR4",          0x9542);
        TEST("POPL @RR2,@RR4",         0x1542);
        TEST("POPL 0x120034,@RR4",     0x5540, 0x1234);
        TEST("POPL 0x561234,@RR4",     0x5540, 0xD600, 0x1234);
        TEST("POPL 0x120034(R2),@RR4", 0x5542, 0x1234);
        TEST("POPL 0x561234(R2),@RR4", 0x5542, 0xD600, 0x1234);
    } else {
        TEST("POP  R2,@R4",         0x9742);
        TEST("POP  @R2,@R4",        0x1742);
        TEST("POP  0x1234,@R4",     0x5740, 0x1234);
        TEST("POP  0x1234(R2),@R4", 0x5742, 0x1234);
        TEST("POPL RR2,@R4",        0x9542);
        TEST("POPL @R2,@R4",        0x1542);
        TEST("POPL 0x1234,@R4",     0x5540, 0x1234);
        TEST("POPL 0x1234(R2),@R4", 0x5542, 0x1234);
    }

    // Push
    if (z8001()) {
        TEST("PUSH  @RR4,R2",           0x9342);
        TEST("PUSH  @RR4,@RR2",         0x1342);
        TEST("PUSH  @RR4,0x120034",     0x5340, 0x1234);
        TEST("PUSH  @RR4,0x561234",     0x5340, 0xD600, 0x1234);
        TEST("PUSH  @RR4,0x120034(R2)", 0x5342, 0x1234);
        TEST("PUSH  @RR4,0x561234(R2)", 0x5342, 0xD600, 0x1234);
        TEST("PUSHL @RR4,RR2",          0x9142);
        TEST("PUSHL @RR4,@RR2",         0x1142);
        TEST("PUSHL @RR4,0x120034",     0x5140, 0x1234);
        TEST("PUSHL @RR4,0x561234",     0x5140, 0xD600, 0x1234);
        TEST("PUSHL @RR4,0x120034(R2)", 0x5142, 0x1234);
        TEST("PUSHL @RR4,0x561234(R2)", 0x5142, 0xD600, 0x1234);
        TEST("PUSH  @RR4,#0x1234",      0x0D49, 0x1234);
    } else {
        TEST("PUSH  @R4,R2",         0x9342);
        TEST("PUSH  @R4,@R2",        0x1342);
        TEST("PUSH  @R4,0x1234",     0x5340, 0x1234);
        TEST("PUSH  @R4,0x1234(R2)", 0x5342, 0x1234);
        TEST("PUSHL @R4,RR2",        0x9142);
        TEST("PUSHL @R4,@R2",        0x1142);
        TEST("PUSHL @R4,0x1234",     0x5140, 0x1234);
        TEST("PUSHL @R4,0x1234(R2)", 0x5142, 0x1234);
        TEST("PUSH  @R4,#0x1234",    0x0D49, 0x1234);
    }
}

static void test_arithmetic() {
    // Add with Carry
    TEST("ADC  R1,R2",   0xB521);
    TEST("ADCB RH1,RL1", 0xB491);

    // Add
    TEST("ADD  R1,R2",   0x8121);
    TEST("ADDB RH1,RL1", 0x8091);
    TEST("ADDL RR2,RR4", 0x9642);
    TEST("ADD  R1,#0x1234", 0x0101, 0x1234);
    TEST("ADDB RH1,#0x56",  0x0001, 0x5656);
    TEST("ADDL RR2,#0x12345678", 0x1602, 0x1234, 0x5678);
    if (z8001()) {
        TEST("ADD  R9,@RR2",          0x0129);
        TEST("ADD  R9,0x120034",      0x4109, 0x1234);
        TEST("ADD  R9,0x561234",      0x4109, 0xD600, 0x1234);
        TEST("ADD  R9,0x120034(R2)",  0x4129, 0x1234);
        TEST("ADD  R9,0x561234(R2)",  0x4129, 0xD600, 0x1234);
        TEST("ADDB RL1,@RR2",         0x0029);
        TEST("ADDB RL1,0x120034",     0x4009, 0x1234);
        TEST("ADDB RL1,0x561234",     0x4009, 0xD600, 0x1234);
        TEST("ADDB RL1,0x120034(R2)", 0x4029, 0x1234);
        TEST("ADDB RL1,0x561234(R2)", 0x4029, 0xD600, 0x1234);
        TEST("ADDL RR2,@RR8",         0x1682);
        TEST("ADDL RR2,0x120034",     0x5602, 0x1234);
        TEST("ADDL RR2,0x561234",     0x5602, 0xD600, 0x1234);
        TEST("ADDL RR2,0x120034(R8)", 0x5682, 0x1234);
        TEST("ADDL RR2,0x561234(R8)", 0x5682, 0xD600, 0x1234);
    } else {
        TEST("ADD  R9,@R2",         0x0129);
        TEST("ADD  R9,0x1234",      0x4109, 0x1234);
        TEST("ADD  R9,0x1234(R2)",  0x4129, 0x1234);
        TEST("ADDB RL1,@R2",        0x0029);
        TEST("ADDB RL1,0x1234",     0x4009, 0x1234);
        TEST("ADDB RL1,0x1234(R2)", 0x4029, 0x1234);
        TEST("ADDL RR2,@R8",        0x1682);
        TEST("ADDL RR2,0x1234",     0x5602, 0x1234);
        TEST("ADDL RR2,0x1234(R8)", 0x5682, 0x1234);
    }

    // Compare
    TEST("CP  R1,R2",   0x8b21);
    TEST("CPB RH1,RL1", 0x8a91);
    TEST("CPL RR2,RR4", 0x9042);
    TEST("CP  R1,#0x1234", 0x0B01, 0x1234);
    TEST("CPB RH1,#0x56",  0x0a01, 0x5656);
    TEST("CPL RR2,#0x12345678", 0x1002, 0x1234, 0x5678);
    if (z8001()) {
        TEST("CP  R9,@RR2",          0x0B29);
        TEST("CP  R9,0x120034",      0x4B09, 0x1234);
        TEST("CP  R9,0x561234",      0x4B09, 0xD600, 0x1234);
        TEST("CP  R9,0x120034(R2)",  0x4B29, 0x1234);
        TEST("CP  R9,0x561234(R2)",  0x4B29, 0xD600, 0x1234);
        TEST("CPB RL1,@RR2",         0x0A29);
        TEST("CPB RL1,0x120034",     0x4A09, 0x1234);
        TEST("CPB RL1,0x561234",     0x4A09, 0xD600, 0x1234);
        TEST("CPB RL1,0x120034(R2)", 0x4A29, 0x1234);
        TEST("CPB RL1,0x561234(R2)", 0x4A29, 0xD600, 0x1234);
        TEST("CPL RR2,@RR8",         0x1082);
        TEST("CPL RR2,0x120034",     0x5002, 0x1234);
        TEST("CPL RR2,0x561234",     0x5002, 0xD600, 0x1234);
        TEST("CPL RR2,0x120034(R8)", 0x5082, 0x1234);
        TEST("CPL RR2,0x561234(R8)", 0x5082, 0xD600, 0x1234);
    } else {
        TEST("CP  R9,@R2",         0x0B29);
        TEST("CP  R9,0x1234",      0x4B09, 0x1234);
        TEST("CP  R9,0x1234(R2)",  0x4B29, 0x1234);
        TEST("CPB RL1,@R2",        0x0A29);
        TEST("CPB RL1,0x1234",     0x4A09, 0x1234);
        TEST("CPB RL1,0x1234(R2)", 0x4A29, 0x1234);
        TEST("CPL RR2,@R8",        0x1082);
        TEST("CPL RR2,0x1234",     0x5002, 0x1234);
        TEST("CPL RR2,0x1234(R8)", 0x5082, 0x1234);
    }
    if (z8001()) {
        TEST("CP  @RR2,#0x5678",         0x0D21, 0x5678);
        TEST("CP  0x120034,#0x5678",     0x4D01, 0x1234, 0x5678);
        TEST("CP  0x561234,#0x5678",     0x4D01, 0xD600, 0x1234, 0x5678);
        TEST("CP  0x120034(R2),#0x5678", 0x4D21, 0x1234, 0x5678);
        TEST("CP  0x561234(R2),#0x5678", 0x4D21, 0xD600, 0x1234, 0x5678);
        TEST("CPB @RR2,#0x56",           0x0C21, 0x5656);
        TEST("CPB 0x120034,#0x56",       0x4C01, 0x1234, 0x5656);
        TEST("CPB 0x561234,#0x56",       0x4C01, 0xD600, 0x1234, 0x5656);
        TEST("CPB 0x120034(R2),#0x56",   0x4C21, 0x1234, 0x5656);
        TEST("CPB 0x561234(R2),#0x56",   0x4C21, 0xD600, 0x1234, 0x5656);
    } else {
        TEST("CP  @R2,#0x5678",        0x0D21, 0x5678);
        TEST("CP  0x1234,#0x5678",     0x4D01, 0x1234, 0x5678);
        TEST("CP  0x1234(R2),#0x5678", 0x4D21, 0x1234, 0x5678);
        TEST("CPB @R2,#0x56",          0x0C21, 0x5656);
        TEST("CPB 0x1234,#0x56",       0x4C01, 0x1234, 0x5656);
        TEST("CPB 0x1234(R2),#0x56",   0x4C21, 0x1234, 0x5656);
    }

    // Decimal Adjust
    TEST("DAB RH1", 0xB010);
    TEST("DAB RH7", 0xB070);
    TEST("DAB RL0", 0xB080);
    TEST("DAB RL7", 0xB0F0);

    // Decrement
    TEST("DEC  R1,#1",   0xAB10);
    TEST("DEC  R1,#16",  0xAB1F);
    TEST("DECB RH1,#1",  0xAA10);
    TEST("DECB RH1,#16", 0xAA1F);
    if (z8001()) {
        TEST("DEC  @RR2,#2",         0x2B21);
        TEST("DEC  0x120034,#4",     0x6B03, 0x1234);
        TEST("DEC  0x561234,#4",     0x6B03, 0xD600, 0x1234);
        TEST("DEC  0x120034(R2),#8", 0x6B27, 0x1234);
        TEST("DEC  0x561234(R2),#8", 0x6B27, 0xD600, 0x1234);
        TEST("DECB @RR2,#2",         0x2A21);
        TEST("DECB 0x120034,#4",     0x6A03, 0x1234);
        TEST("DECB 0x561234,#4",     0x6A03, 0xD600, 0x1234);
        TEST("DECB 0x120034(R2),#8", 0x6A27, 0x1234);
        TEST("DECB 0x561234(R2),#8", 0x6A27, 0xD600, 0x1234);
    } else {
        TEST("DEC  @R2,#2",        0x2B21);
        TEST("DEC  0x1234,#4",     0x6B03, 0x1234);
        TEST("DEC  0x1234(R2),#8", 0x6B27, 0x1234);
        TEST("DECB @R2,#2",        0x2A21);
        TEST("DECB 0x1234,#4",     0x6A03, 0x1234);
        TEST("DECB 0x1234(R2),#8", 0x6A27, 0x1234);
    }

    // Divide
    TEST("DIV  RR4,R2",          0x9B24);
    TEST("DIV  RR4,#0x1234",     0x1B04, 0x1234);
    TEST("DIVL RQ4,RR2",         0x9A24);
    TEST("DIVL RQ4,#0x12345678", 0x1A04, 0x1234, 0x5678);
    if (z8001()) {
        TEST("DIV  RR4,@RR2",         0x1B24);
        TEST("DIV  RR4,0x120034",     0x5B04, 0x1234);
        TEST("DIV  RR4,0x561234",     0x5B04, 0xD600, 0x1234);
        TEST("DIV  RR4,0x120034(R2)", 0x5B24, 0x1234);
        TEST("DIV  RR4,0x561234(R2)", 0x5B24, 0xD600, 0x1234);
        TEST("DIVL RQ4,@RR2",         0x1A24);
        TEST("DIVL RQ4,0x120034",     0x5A04, 0x1234);
        TEST("DIVL RQ4,0x561234",     0x5A04, 0xD600, 0x1234);
        TEST("DIVL RQ4,0x120034(R2)", 0x5A24, 0x1234);
        TEST("DIVL RQ4,0x561234(R2)", 0x5A24, 0xD600, 0x1234);
    } else {
        TEST("DIV  RR4,@R2",        0x1B24);
        TEST("DIV  RR4,0x1234",     0x5B04, 0x1234);
        TEST("DIV  RR4,0x1234(R2)", 0x5B24, 0x1234);
        TEST("DIVL RQ4,@R2",        0x1A24);
        TEST("DIVL RQ4,0x1234",     0x5A04, 0x1234);
        TEST("DIVL RQ4,0x1234(R2)", 0x5A24, 0x1234);
    }

    // Extend Sign
    TEST("EXTSB R1",  0xB110);
    TEST("EXTSB R7",  0xB170);
    TEST("EXTSB R8",  0xB180);
    TEST("EXTSB R15", 0xB1F0);
    TEST("EXTS  RR2", 0xB12A);
    TEST("EXTS  RR8", 0xB18A);
    TEST("EXTSL RQ4", 0xB147);
    TEST("EXTSL RQ8", 0xB187);
    ERRT("    EXTS  RR7", ILLEGAL_REGISTER);
    ERRT("EXTS  R15", REGISTER_NOT_ALLOWED);
    ERRT("EXTSL RQ6", ILLEGAL_REGISTER);
    ERRT("EXTSL RR14", REGISTER_NOT_ALLOWED);

    // Increment
    TEST("INC  R1,#1",   0xA910);
    TEST("INC  R1,#16",  0xA91F);
    TEST("INCB RH1,#1",  0xA810);
    TEST("INCB RH1,#16", 0xA81F);
    if (z8001()) {
        TEST("INC  @RR2,#2",         0x2921);
        TEST("INC  0x120034,#4",     0x6903, 0x1234);
        TEST("INC  0x561234,#4",     0x6903, 0xD600, 0x1234);
        TEST("INC  0x120034(R2),#8", 0x6927, 0x1234);
        TEST("INC  0x561234(R2),#8", 0x6927, 0xD600, 0x1234);
        TEST("INCB @RR2,#2",         0x2821);
        TEST("INCB 0x120034,#4",     0x6803, 0x1234);
        TEST("INCB 0x561234,#4",     0x6803, 0xD600, 0x1234);
        TEST("INCB 0x120034(R2),#8", 0x6827, 0x1234);
        TEST("INCB 0x561234(R2),#8", 0x6827, 0xD600, 0x1234);
    } else {
        TEST("INC  @R2,#2",        0x2921);
        TEST("INC  0x1234,#4",     0x6903, 0x1234);
        TEST("INC  0x1234(R2),#8", 0x6927, 0x1234);
        TEST("INCB @R2,#2",        0x2821);
        TEST("INCB 0x1234,#4",     0x6803, 0x1234);
        TEST("INCB 0x1234(R2),#8", 0x6827, 0x1234);
    }

    // Multiply
    TEST("MULT  RR4,R2",          0x9924);
    TEST("MULT  RR4,#0x1234",     0x1904, 0x1234);
    TEST("MULTL RQ4,RR2",         0x9824);
    TEST("MULTL RQ4,#0x12345678", 0x1804, 0x1234, 0x5678);
    if (z8001()) {
        TEST("MULT  RR4,@RR2",         0x1924);
        TEST("MULT  RR4,0x120034",     0x5904, 0x1234);
        TEST("MULT  RR4,0x561234",     0x5904, 0xD600, 0x1234);
        TEST("MULT  RR4,0x120034(R2)", 0x5924, 0x1234);
        TEST("MULT  RR4,0x561234(R2)", 0x5924, 0xD600, 0x1234);
        TEST("MULTL RQ4,@RR2",         0x1824);
        TEST("MULTL RQ4,0x120034",     0x5804, 0x1234);
        TEST("MULTL RQ4,0x561234",     0x5804, 0xD600, 0x1234);
        TEST("MULTL RQ4,0x120034(R2)", 0x5824, 0x1234);
        TEST("MULTL RQ4,0x561234(R2)", 0x5824, 0xD600, 0x1234);
    } else {
        TEST("MULT  RR4,@R2",        0x1924);
        TEST("MULT  RR4,0x1234",     0x5904, 0x1234);
        TEST("MULT  RR4,0x1234(R2)", 0x5924, 0x1234);
        TEST("MULTL RQ4,@R2",        0x1824);
        TEST("MULTL RQ4,0x1234",     0x5804, 0x1234);
        TEST("MULTL RQ4,0x1234(R2)", 0x5824, 0x1234);
    }

    // Negate
    TEST("NEG  R2",  0x8D22);
    TEST("NEGB RL2", 0x8CA2);
    if (z8001()) {
        TEST("NEG  @RR2",         0x0D22);
        TEST("NEG  0x120034",     0x4D02, 0x1234);
        TEST("NEG  0x561234",     0x4D02, 0xD600, 0x1234);
        TEST("NEG  0x120034(R2)", 0x4D22, 0x1234);
        TEST("NEG  0x561234(R2)", 0x4D22, 0xD600, 0x1234);
        TEST("NEGB @RR2",         0x0C22);
        TEST("NEGB 0x120034",     0x4C02, 0x1234);
        TEST("NEGB 0x561234",     0x4C02, 0xD600, 0x1234);
        TEST("NEGB 0x120034(R2)", 0x4C22, 0x1234);
        TEST("NEGB 0x561234(R2)", 0x4C22, 0xD600, 0x1234);
    } else {
        TEST("NEG  @R2",        0x0D22);
        TEST("NEG  0x1234",     0x4D02, 0x1234);
        TEST("NEG  0x1234(R2)", 0x4D22, 0x1234);
        TEST("NEGB @R2",        0x0C22);
        TEST("NEGB 0x1234",     0x4C02, 0x1234);
        TEST("NEGB 0x1234(R2)", 0x4C22, 0x1234);
    }

    // Subtract with Carry
    TEST("SBC  R1,R2",   0xB721);
    TEST("SBCB RH1,RL1", 0xB691);

    // Subtract
    TEST("SUB  R1,R2",   0x8321);
    TEST("SUBB RH1,RL1", 0x8291);
    TEST("SUBL RR2,RR4", 0x9242);
    TEST("SUB  R1,#0x1234", 0x0301, 0x1234);
    TEST("SUBB RH1,#0x56",  0x0201, 0x5656);
    TEST("SUBL RR2,#0x12345678", 0x1202, 0x1234, 0x5678);
    if (z8001()) {
        TEST("SUB  R9,@RR2",          0x0329);
        TEST("SUB  R9,0x120034",      0x4309, 0x1234);
        TEST("SUB  R9,0x561234",      0x4309, 0xD600, 0x1234);
        TEST("SUB  R9,0x120034(R2)",  0x4329, 0x1234);
        TEST("SUB  R9,0x561234(R2)",  0x4329, 0xD600, 0x1234);
        TEST("SUBB RL1,@RR2",         0x0229);
        TEST("SUBB RL1,0x120034",     0x4209, 0x1234);
        TEST("SUBB RL1,0x561234",     0x4209, 0xD600, 0x1234);
        TEST("SUBB RL1,0x120034(R2)", 0x4229, 0x1234);
        TEST("SUBB RL1,0x561234(R2)", 0x4229, 0xD600, 0x1234);
        TEST("SUBL RR2,@RR8",         0x1282);
        TEST("SUBL RR2,0x120034",     0x5202, 0x1234);
        TEST("SUBL RR2,0x561234",     0x5202, 0xD600, 0x1234);
        TEST("SUBL RR2,0x120034(R8)", 0x5282, 0x1234);
        TEST("SUBL RR2,0x561234(R8)", 0x5282, 0xD600, 0x1234);
    } else {
        TEST("SUB  R9,@R2",         0x0329);
        TEST("SUB  R9,0x1234",      0x4309, 0x1234);
        TEST("SUB  R9,0x1234(R2)",  0x4329, 0x1234);
        TEST("SUBB RL1,@R2",        0x0229);
        TEST("SUBB RL1,0x1234",     0x4209, 0x1234);
        TEST("SUBB RL1,0x1234(R2)", 0x4229, 0x1234);
        TEST("SUBL RR2,@R8",        0x1282);
        TEST("SUBL RR2,0x1234",     0x5202, 0x1234);
        TEST("SUBL RR2,0x1234(R8)", 0x5282, 0x1234);
    }
}

static void test_logical() {
    // And
    TEST("AND  R1,R2",   0x8721);
    TEST("ANDB RH1,RL1", 0x8691);
    TEST("AND  R1,#0x1234", 0x0701, 0x1234);
    TEST("ANDB RH1,#0x56",  0x0601, 0x5656);
    if (z8001()) {
        TEST("AND  R9,@RR2",          0x0729);
        TEST("AND  R9,0x120034",      0x4709, 0x1234);
        TEST("AND  R9,0x561234",      0x4709, 0xD600, 0x1234);
        TEST("AND  R9,0x120034(R2)",  0x4729, 0x1234);
        TEST("AND  R9,0x561234(R2)",  0x4729, 0xD600, 0x1234);
        TEST("ANDB RL1,@RR2",         0x0629);
        TEST("ANDB RL1,0x120034",     0x4609, 0x1234);
        TEST("ANDB RL1,0x561234",     0x4609, 0xD600, 0x1234);
        TEST("ANDB RL1,0x120034(R2)", 0x4629, 0x1234);
        TEST("ANDB RL1,0x561234(R2)", 0x4629, 0xD600, 0x1234);
    } else {
        TEST("AND  R9,@R2",         0x0729);
        TEST("AND  R9,0x1234",      0x4709, 0x1234);
        TEST("AND  R9,0x1234(R2)",  0x4729, 0x1234);
        TEST("ANDB RL1,@R2",        0x0629);
        TEST("ANDB RL1,0x1234",     0x4609, 0x1234);
        TEST("ANDB RL1,0x1234(R2)", 0x4629, 0x1234);
    }

    // Complement
    TEST("COM  R2",  0x8D20);
    TEST("COMB RL1", 0x8C90);
    if (z8001()) {
        TEST("COM  @RR2",         0x0D20);
        TEST("COM  0x120034",     0x4D00, 0x1234);
        TEST("COM  0x561234",     0x4D00, 0xD600, 0x1234);
        TEST("COM  0x120034(R2)", 0x4D20, 0x1234);
        TEST("COM  0x561234(R2)", 0x4D20, 0xD600, 0x1234);
        TEST("COMB @RR2",         0x0C20);
        TEST("COMB 0x120034",     0x4C00, 0x1234);
        TEST("COMB 0x561234",     0x4C00, 0xD600, 0x1234);
        TEST("COMB 0x120034(R2)", 0x4C20, 0x1234);
        TEST("COMB 0x561234(R2)", 0x4C20, 0xD600, 0x1234);
    } else {
        TEST("COM  @R2",        0x0D20);
        TEST("COM  0x1234",     0x4D00, 0x1234);
        TEST("COM  0x1234(R2)", 0x4D20, 0x1234);
        TEST("COMB @R2",        0x0C20);
        TEST("COMB 0x1234",     0x4C00, 0x1234);
        TEST("COMB 0x1234(R2)", 0x4C20, 0x1234);
    }

    // Or
    TEST("OR  R1,R2",   0x8521);
    TEST("ORB RH1,RL1", 0x8491);
    TEST("OR  R1,#0x1234", 0x0501, 0x1234);
    TEST("ORB RH1,#0x56",  0x0401, 0x5656);
    if (z8001()) {
        TEST("OR  R9,@RR2",          0x0529);
        TEST("OR  R9,0x120034",      0x4509, 0x1234);
        TEST("OR  R9,0x561234",      0x4509, 0xD600, 0x1234);
        TEST("OR  R9,0x120034(R2)",  0x4529, 0x1234);
        TEST("OR  R9,0x561234(R2)",  0x4529, 0xD600, 0x1234);
        TEST("ORB RL1,@RR2",         0x0429);
        TEST("ORB RL1,0x120034",     0x4409, 0x1234);
        TEST("ORB RL1,0x561234",     0x4409, 0xD600, 0x1234);
        TEST("ORB RL1,0x120034(R2)", 0x4429, 0x1234);
        TEST("ORB RL1,0x561234(R2)", 0x4429, 0xD600, 0x1234);
    } else {
        TEST("OR  R9,@R2",         0x0529);
        TEST("OR  R9,0x1234",      0x4509, 0x1234);
        TEST("OR  R9,0x1234(R2)",  0x4529, 0x1234);
        TEST("ORB RL1,@R2",        0x0429);
        TEST("ORB RL1,0x1234",     0x4409, 0x1234);
        TEST("ORB RL1,0x1234(R2)", 0x4429, 0x1234);
    }

    // Test
    TEST("TEST  R2",  0x8D24);
    TEST("TESTB RL1", 0x8C94);
    TEST("TESTL RR2", 0x9C28);
    if (z8001()) {
        TEST("TEST  @RR2",         0x0D24);
        TEST("TEST  0x120034",     0x4D04, 0x1234);
        TEST("TEST  0x561234",     0x4D04, 0xD600, 0x1234);
        TEST("TEST  0x120034(R2)", 0x4D24, 0x1234);
        TEST("TEST  0x561234(R2)", 0x4D24, 0xD600, 0x1234);
        TEST("TESTB @RR2",         0x0C24);
        TEST("TESTB 0x120034",     0x4C04, 0x1234);
        TEST("TESTB 0x561234",     0x4C04, 0xD600, 0x1234);
        TEST("TESTB 0x120034(R2)", 0x4C24, 0x1234);
        TEST("TESTB 0x561234(R2)", 0x4C24, 0xD600, 0x1234);
        TEST("TESTL @RR2",         0x1C28);
        TEST("TESTL 0x120034",     0x5C08, 0x1234);
        TEST("TESTL 0x561234",     0x5C08, 0xD600, 0x1234);
        TEST("TESTL 0x120034(R2)", 0x5C28, 0x1234);
        TEST("TESTL 0x561234(R2)", 0x5C28, 0xD600, 0x1234);
    } else {
        TEST("TEST  @R2",        0x0D24);
        TEST("TEST  0x1234",     0x4D04, 0x1234);
        TEST("TEST  0x1234(R2)", 0x4D24, 0x1234);
        TEST("TESTB @R2",        0x0C24);
        TEST("TESTB 0x1234",     0x4C04, 0x1234);
        TEST("TESTB 0x1234(R2)", 0x4C24, 0x1234);
        TEST("TESTL @R2",        0x1C28);
        TEST("TESTL 0x1234",     0x5C08, 0x1234);
        TEST("TESTL 0x1234(R2)", 0x5C28, 0x1234);
    }

    // Exclusive Or
    TEST("XOR  R1,R2",   0x8921);
    TEST("XORB RH1,RL1", 0x8891);
    TEST("XOR  R1,#0x1234", 0x0901, 0x1234);
    TEST("XORB RH1,#0x56",  0x0801, 0x5656);
    if (z8001()) {
        TEST("XOR  R9,@RR2",          0x0929);
        TEST("XOR  R9,0x120034",      0x4909, 0x1234);
        TEST("XOR  R9,0x561234",      0x4909, 0xD600, 0x1234);
        TEST("XOR  R9,0x120034(R2)",  0x4929, 0x1234);
        TEST("XOR  R9,0x561234(R2)",  0x4929, 0xD600, 0x1234);
        TEST("XORB RL1,@RR2",         0x0829);
        TEST("XORB RL1,0x120034",     0x4809, 0x1234);
        TEST("XORB RL1,0x561234",     0x4809, 0xD600, 0x1234);
        TEST("XORB RL1,0x120034(R2)", 0x4829, 0x1234);
        TEST("XORB RL1,0x561234(R2)", 0x4829, 0xD600, 0x1234);
    } else {
        TEST("XOR  R9,@R2",         0x0929);
        TEST("XOR  R9,0x1234",      0x4909, 0x1234);
        TEST("XOR  R9,0x1234(R2)",  0x4929, 0x1234);
        TEST("XORB RL1,@R2",        0x0829);
        TEST("XORB RL1,0x1234",     0x4809, 0x1234);
        TEST("XORB RL1,0x1234(R2)", 0x4829, 0x1234);
    }
}

static void test_program_control() {
    // Call Procedure
    if (z8001()) {
        TEST("CALL @RR2",         0x1F20);
        TEST("CALL 0x120034",     0x5F00, 0x1234);
        TEST("CALL 0x561234",     0x5F00, 0xD600, 0x1234);
        TEST("CALL 0x120034(R2)", 0x5F20, 0x1234);
        TEST("CALL 0x561234(R2)", 0x5F20, 0xD600, 0x1234);
    } else {
        TEST("CALL @R2",        0x1F20);
        TEST("CALL 0x1234",     0x5F00, 0x1234);
        TEST("CALL 0x1234(R2)", 0x5F20, 0x1234);
    }

    // Call Procedure Relative
    TEST("CALR $+2",      0xD000);
    TEST("CALR $+10",     0xDFFC);
    TEST("CALR $+0x1002", 0xD800);
    TEST("CALR $-0x0FFC", 0xD7FF);
    ERRT("CALR $+0x1004", OPERAND_TOO_FAR);
    ERRT("CALR $-0x0FFE", OPERAND_TOO_FAR);
    TEST("CALR $-10",     0xD006);
    TEST("CALR $",        0xD001);
    ERRT("CALR $+13",     OPERAND_NOT_ALIGNED);
    ERRT("CALR $-13",     OPERAND_NOT_ALIGNED);

    // Decrement and Jump if Not Zero
    TEST("DJNZ  R2,$",      0xF281);
    TEST("DBJNZ RL0,$",     0xF801);
    TEST("DBJNZ RL0,$+2",   0xF800);
    ERRT("DBJNZ RL0,$+3",   OPERAND_NOT_ALIGNED);
    ERRT("DBJNZ RL0,$+4",   OPERAND_TOO_FAR);
    TEST("DBJNZ RL0,$-252", 0xF87F);
    ERRT("DBJNZ RL0,$-253", OPERAND_NOT_ALIGNED);
    ERRT("DBJNZ RL0,$-254", OPERAND_TOO_FAR);

    // Interrupt Return
    TEST("IRET  ", 0x7B00);

    // Jump
    if (z8001()) {
        TEST("JP F,@RR2",         0x1E20);
        TEST("JP F,0x120034",     0x5E00, 0x1234);
        TEST("JP F,0x561234",     0x5E00, 0xD600, 0x1234);
        TEST("JP F,0x120034(R2)", 0x5E20, 0x1234);
        TEST("JP F,0x561234(R2)", 0x5E20, 0xD600, 0x1234);
    } else {
        TEST("JP F,@R2",        0x1E20);
        TEST("JP F,0x1234",     0x5E00, 0x1234);
        TEST("JP F,0x1234(R2)", 0x5E20, 0x1234);
    }
    if (z8001()) {
        TEST("JP LT,@RR2",         0x1E21);
        TEST("JP LT,0x120034",     0x5E01, 0x1234);
        TEST("JP LT,0x561234",     0x5E01, 0xD600, 0x1234);
        TEST("JP LT,0x120034(R2)", 0x5E21, 0x1234);
        TEST("JP LT,0x561234(R2)", 0x5E21, 0xD600, 0x1234);
    } else {
        TEST("JP LT,@R2",        0x1E21);
        TEST("JP LT,0x1234",     0x5E01, 0x1234);
        TEST("JP LT,0x1234(R2)", 0x5E21, 0x1234);
    }
    if (z8001()) {
        TEST("JP LE,@RR2",         0x1E22);
        TEST("JP LE,0x120034",     0x5E02, 0x1234);
        TEST("JP LE,0x561234",     0x5E02, 0xD600, 0x1234);
        TEST("JP LE,0x120034(R2)", 0x5E22, 0x1234);
        TEST("JP LE,0x561234(R2)", 0x5E22, 0xD600, 0x1234);
    } else {
        TEST("JP LE,@R2",        0x1E22);
        TEST("JP LE,0x1234",     0x5E02, 0x1234);
        TEST("JP LE,0x1234(R2)", 0x5E22, 0x1234);
    }
    if (z8001()) {
        TEST("JP ULE,@RR2",         0x1E23);
        TEST("JP ULE,0x120034",     0x5E03, 0x1234);
        TEST("JP ULE,0x561234",     0x5E03, 0xD600, 0x1234);
        TEST("JP ULE,0x120034(R2)", 0x5E23, 0x1234);
        TEST("JP ULE,0x561234(R2)", 0x5E23, 0xD600, 0x1234);
    } else {
        TEST("JP ULE,@R2",        0x1E23);
        TEST("JP ULE,0x1234",     0x5E03, 0x1234);
        TEST("JP ULE,0x1234(R2)", 0x5E23, 0x1234);
    }
    if (z8001()) {
        TEST("JP OV,@RR2",         0x1E24);
        TEST("JP OV,0x120034",     0x5E04, 0x1234);
        TEST("JP OV,0x561234",     0x5E04, 0xD600, 0x1234);
        TEST("JP OV,0x120034(R2)", 0x5E24, 0x1234);
        TEST("JP OV,0x561234(R2)", 0x5E24, 0xD600, 0x1234);
    } else {
        TEST("JP OV,@R2",        0x1E24);
        TEST("JP OV,0x1234",     0x5E04, 0x1234);
        TEST("JP OV,0x1234(R2)", 0x5E24, 0x1234);
    }
    if (z8001()) {
        TEST("JP MI,@RR2",         0x1E25);
        TEST("JP MI,0x120034",     0x5E05, 0x1234);
        TEST("JP MI,0x561234",     0x5E05, 0xD600, 0x1234);
        TEST("JP MI,0x120034(R2)", 0x5E25, 0x1234);
        TEST("JP MI,0x561234(R2)", 0x5E25, 0xD600, 0x1234);
    } else {
        TEST("JP MI,@R2",        0x1E25);
        TEST("JP MI,0x1234",     0x5E05, 0x1234);
        TEST("JP MI,0x1234(R2)", 0x5E25, 0x1234);
    }
    if (z8001()) {
        TEST("JP Z,@RR2",         0x1E26);
        TEST("JP Z,0x120034",     0x5E06, 0x1234);
        TEST("JP Z,0x561234",     0x5E06, 0xD600, 0x1234);
        TEST("JP Z,0x120034(R2)", 0x5E26, 0x1234);
        TEST("JP Z,0x561234(R2)", 0x5E26, 0xD600, 0x1234);
    } else {
        TEST("JP Z,@R2",        0x1E26);
        TEST("JP Z,0x1234",     0x5E06, 0x1234);
        TEST("JP Z,0x1234(R2)", 0x5E26, 0x1234);
    }
    if (z8001()) {
        TEST("JP C,@RR2",         0x1E27);
        TEST("JP C,0x120034",     0x5E07, 0x1234);
        TEST("JP C,0x561234",     0x5E07, 0xD600, 0x1234);
        TEST("JP C,0x120034(R2)", 0x5E27, 0x1234);
        TEST("JP C,0x561234(R2)", 0x5E27, 0xD600, 0x1234);
    } else {
        TEST("JP C,@R2",        0x1E27);
        TEST("JP C,0x1234",     0x5E07, 0x1234);
        TEST("JP C,0x1234(R2)", 0x5E27, 0x1234);
    }
    if (z8001()) {
        TEST("JP @RR2",         0x1E28);
        TEST("JP 0x120034",     0x5E08, 0x1234);
        TEST("JP 0x561234",     0x5E08, 0xD600, 0x1234);
        TEST("JP 0x120034(R2)", 0x5E28, 0x1234);
        TEST("JP 0x561234(R2)", 0x5E28, 0xD600, 0x1234);
    } else {
        TEST("JP @R2",        0x1E28);
        TEST("JP 0x1234",     0x5E08, 0x1234);
        TEST("JP 0x1234(R2)", 0x5E28, 0x1234);
    }
    if (z8001()) {
        TEST("JP GE,@RR2",         0x1E29);
        TEST("JP GE,0x120034",     0x5E09, 0x1234);
        TEST("JP GE,0x561234",     0x5E09, 0xD600, 0x1234);
        TEST("JP GE,0x120034(R2)", 0x5E29, 0x1234);
        TEST("JP GE,0x561234(R2)", 0x5E29, 0xD600, 0x1234);
    } else {
        TEST("JP GE,@R2",        0x1E29);
        TEST("JP GE,0x1234",     0x5E09, 0x1234);
        TEST("JP GE,0x1234(R2)", 0x5E29, 0x1234);
    }
    if (z8001()) {
        TEST("JP GT,@RR2",         0x1E2A);
        TEST("JP GT,0x120034",     0x5E0A, 0x1234);
        TEST("JP GT,0x561234",     0x5E0A, 0xD600, 0x1234);
        TEST("JP GT,0x120034(R2)", 0x5E2A, 0x1234);
        TEST("JP GT,0x561234(R2)", 0x5E2A, 0xD600, 0x1234);
    } else {
        TEST("JP GT,@R2",        0x1E2A);
        TEST("JP GT,0x1234",     0x5E0A, 0x1234);
        TEST("JP GT,0x1234(R2)", 0x5E2A, 0x1234);
    }
    if (z8001()) {
        TEST("JP UGT,@RR2",         0x1E2B);
        TEST("JP UGT,0x120034",     0x5E0B, 0x1234);
        TEST("JP UGT,0x561234",     0x5E0B, 0xD600, 0x1234);
        TEST("JP UGT,0x120034(R2)", 0x5E2B, 0x1234);
        TEST("JP UGT,0x561234(R2)", 0x5E2B, 0xD600, 0x1234);
    } else {
        TEST("JP UGT,@R2",        0x1E2B);
        TEST("JP UGT,0x1234",     0x5E0B, 0x1234);
        TEST("JP UGT,0x1234(R2)", 0x5E2B, 0x1234);
    }
    if (z8001()) {
        TEST("JP NOV,@RR2",         0x1E2C);
        TEST("JP NOV,0x120034",     0x5E0C, 0x1234);
        TEST("JP NOV,0x561234",     0x5E0C, 0xD600, 0x1234);
        TEST("JP NOV,0x120034(R2)", 0x5E2C, 0x1234);
        TEST("JP NOV,0x561234(R2)", 0x5E2C, 0xD600, 0x1234);
    } else {
        TEST("JP NOV,@R2",        0x1E2C);
        TEST("JP NOV,0x1234",     0x5E0C, 0x1234);
        TEST("JP NOV,0x1234(R2)", 0x5E2C, 0x1234);
    }
    if (z8001()) {
        TEST("JP PL,@RR2",         0x1E2D);
        TEST("JP PL,0x120034",     0x5E0D, 0x1234);
        TEST("JP PL,0x561234",     0x5E0D, 0xD600, 0x1234);
        TEST("JP PL,0x120034(R2)", 0x5E2D, 0x1234);
        TEST("JP PL,0x561234(R2)", 0x5E2D, 0xD600, 0x1234);
    } else {
        TEST("JP PL,@R2",        0x1E2D);
        TEST("JP PL,0x1234",     0x5E0D, 0x1234);
        TEST("JP PL,0x1234(R2)", 0x5E2D, 0x1234);
    }
    if (z8001()) {
        TEST("JP NZ,@RR2",         0x1E2E);
        TEST("JP NZ,0x120034",     0x5E0E, 0x1234);
        TEST("JP NZ,0x561234",     0x5E0E, 0xD600, 0x1234);
        TEST("JP NZ,0x120034(R2)", 0x5E2E, 0x1234);
        TEST("JP NZ,0x561234(R2)", 0x5E2E, 0xD600, 0x1234);
    } else {
        TEST("JP NZ,@R2",        0x1E2E);
        TEST("JP NZ,0x1234",     0x5E0E, 0x1234);
        TEST("JP NZ,0x1234(R2)", 0x5E2E, 0x1234);
    }
    if (z8001()) {
        TEST("JP NC,@RR2",         0x1E2F);
        TEST("JP NC,0x120034",     0x5E0F, 0x1234);
        TEST("JP NC,0x561234",     0x5E0F, 0xD600, 0x1234);
        TEST("JP NC,0x120034(R2)", 0x5E2F, 0x1234);
        TEST("JP NC,0x561234(R2)", 0x5E2F, 0xD600, 0x1234);
    } else {
        TEST("JP NC,@R2",        0x1E2F);
        TEST("JP NC,0x1234",     0x5E0F, 0x1234);
        TEST("JP NC,0x1234(R2)", 0x5E2F, 0x1234);
    }

    // Jump Relative
    TEST("JR F,$",   0xE0FF);
    TEST("JR LT,$",  0xE1FF);
    TEST("JR LE,$",  0xE2FF);
    TEST("JR ULE,$", 0xE3FF);
    TEST("JR OV,$",  0xE4FF);
    TEST("JR MI,$",  0xE5FF);
    TEST("JR Z,$",   0xE6FF);
    TEST("JR C,$",   0xE7FF);
    TEST("JR $",     0xE8FF);
    TEST("JR GE,$",  0xE9FF);
    TEST("JR GT,$",  0xEAFF);
    TEST("JR UGT,$", 0xEBFF);
    TEST("JR NOV,$", 0xECFF);
    TEST("JR PL,$",  0xEDFF);
    TEST("JR NZ,$",  0xEEFF);
    TEST("JR NC,$",  0xEFFF);
    TEST("JR EQ,$",  0xE6FF);
    TEST("JR ULT,$", 0xE7FF);
    TEST("JR NE,$",  0xEEFF);
    TEST("JR UGE,$", 0xEFFF);
    TEST("JR $-254", 0xE880);
    ERRT("JR $-255", OPERAND_NOT_ALIGNED);
    ERRT("JR $-256", OPERAND_TOO_FAR);
    TEST("JR $+256", 0xE87F);
    ERRT("JR $+257", OPERAND_NOT_ALIGNED);
    ERRT("JR $+258", OPERAND_TOO_FAR);

    // Return from Procedure
    TEST("RET F",   0x9E00);
    TEST("RET LT",  0x9E01);
    TEST("RET LE",  0x9E02);
    TEST("RET ULE", 0x9E03);
    TEST("RET OV",  0x9E04);
    TEST("RET MI",  0x9E05);
    TEST("RET Z",   0x9E06);
    TEST("RET C",   0x9E07);
    TEST("RET ",    0x9E08);
    TEST("RET GE",  0x9E09);
    TEST("RET GT",  0x9E0A);
    TEST("RET UGT", 0x9E0B);
    TEST("RET NOV", 0x9E0C);
    TEST("RET PL",  0x9E0D);
    TEST("RET NZ",  0x9E0E);
    TEST("RET NC",  0x9E0F);
    TEST("RET EQ",  0x9E06);
    TEST("RET ULT", 0x9E07);
    TEST("RET NE",  0x9E0E);
    TEST("RET UGE", 0x9E0F);
    
    // System call
    TEST("SC #0",    0x7F00);
    TEST("SC #0xFF", 0x7FFF);
}

static void test_bit_manipulation() {
    // Bit Test
    TEST("BIT  R2,#1",  0xA721);
    TEST("BIT  R2,R8",  0x2708, 0x0200);
    TEST("BITB RL2,#1", 0xA6A1);
    TEST("BITB RL2,R1", 0x2601, 0x0A00);
    ERRT("BITB RL2,#8", ILLEGAL_BIT_NUMBER);
    ERRT("BITB RL2,R8", REGISTER_NOT_ALLOWED);
    if (z8001()) {
        TEST("BIT  @RR2,#3",          0x2723);
        TEST("BIT  0x120034,#4",      0x6704, 0x1234);
        TEST("BIT  0x561234,#9",      0x6709, 0xD600, 0x1234);
        TEST("BIT  0x120034(R2),#14", 0x672E, 0x1234);
        TEST("BIT  0x561234(R2),#15", 0x672F, 0xD600, 0x1234);
        TEST("BITB @RR2,#3",          0x2623);
        TEST("BITB 0x120034,#4",      0x6604, 0x1234);
        TEST("BITB 0x561234,#7",      0x6607, 0xD600, 0x1234);
        TEST("BITB 0x120034(R2),#0",  0x6620, 0x1234);
        TEST("BITB 0x561234(R2),#7",  0x6627, 0xD600, 0x1234);
    } else {
        TEST("BIT  @R2,#3",         0x2723);
        TEST("BIT  0x1234,#4",      0x6704, 0x1234);
        TEST("BIT  0x1234(R2),#14", 0x672E, 0x1234);
        TEST("BITB @R2,#3",         0x2623);
        TEST("BITB 0x1234,#4",      0x6604, 0x1234);
        TEST("BITB 0x1234(R2),#7",  0x6627, 0x1234);
    }

    // Reset Bit
    TEST("RES  R2,#1",  0xA321);
    TEST("RES  R2,R8",  0x2308, 0x0200);
    TEST("RESB RL2,#1", 0xA2A1);
    TEST("RESB RL2,R1", 0x2201, 0x0A00);
    ERRT("RESB RL2,#8", ILLEGAL_BIT_NUMBER);
    ERRT("RESB RL2,R8", REGISTER_NOT_ALLOWED);
    if (z8001()) {
        TEST("RES  @RR2,#3",          0x2323);
        TEST("RES  0x120034,#4",      0x6304, 0x1234);
        TEST("RES  0x561234,#9",      0x6309, 0xD600, 0x1234);
        TEST("RES  0x120034(R2),#14", 0x632E, 0x1234);
        TEST("RES  0x561234(R2),#15", 0x632F, 0xD600, 0x1234);
        TEST("RESB @RR2,#3",          0x2223);
        TEST("RESB 0x120034,#4",      0x6204, 0x1234);
        TEST("RESB 0x561234,#7",      0x6207, 0xD600, 0x1234);
        TEST("RESB 0x120034(R2),#0",  0x6220, 0x1234);
        TEST("RESB 0x561234(R2),#7",  0x6227, 0xD600, 0x1234);
    } else {
        TEST("RES  @R2,#3",         0x2323);
        TEST("RES  0x1234,#4",      0x6304, 0x1234);
        TEST("RES  0x1234(R2),#14", 0x632E, 0x1234);
        TEST("RESB @R2,#3",         0x2223);
        TEST("RESB 0x1234,#4",      0x6204, 0x1234);
        TEST("RESB 0x1234(R2),#7",  0x6227, 0x1234);
    }

    // Set Bit
    TEST("SET  R2,#1",  0xA521);
    TEST("SET  R2,R8",  0x2508, 0x0200);
    TEST("SETB RL2,#1", 0xA4A1);
    TEST("SETB RL2,R1", 0x2401, 0x0A00);
    ERRT("SETB RL2,#8", ILLEGAL_BIT_NUMBER);
    ERRT("SETB RL2,R8", REGISTER_NOT_ALLOWED);
    if (z8001()) {
        TEST("SET  @RR2,#3",          0x2523);
        TEST("SET  0x120034,#4",      0x6504, 0x1234);
        TEST("SET  0x561234,#9",      0x6509, 0xD600, 0x1234);
        TEST("SET  0x120034(R2),#14", 0x652E, 0x1234);
        TEST("SET  0x561234(R2),#15", 0x652F, 0xD600, 0x1234);
        TEST("SETB @RR2,#3",          0x2423);
        TEST("SETB 0x120034,#4",      0x6404, 0x1234);
        TEST("SETB 0x561234,#7",      0x6407, 0xD600, 0x1234);
        TEST("SETB 0x120034(R2),#0",  0x6420, 0x1234);
        TEST("SETB 0x561234(R2),#7",  0x6427, 0xD600, 0x1234);
    } else {
        TEST("SET  @R2,#3",         0x2523);
        TEST("SET  0x1234,#4",      0x6504, 0x1234);
        TEST("SET  0x1234(R2),#14", 0x652E, 0x1234);
        TEST("SETB @R2,#3",         0x2423);
        TEST("SETB 0x1234,#4",      0x6404, 0x1234);
        TEST("SETB 0x1234(R2),#7",  0x6427, 0x1234);
    }

    // Test and Set
    TEST("TSET  R2",  0x8D26);
    TEST("TSETB RL1", 0x8C96);
    if (z8001()) {
        TEST("TSET  @RR2",         0x0D26);
        TEST("TSET  0x120034",     0x4D06, 0x1234);
        TEST("TSET  0x561234",     0x4D06, 0xD600, 0x1234);
        TEST("TSET  0x120034(R2)", 0x4D26, 0x1234);
        TEST("TSET  0x561234(R2)", 0x4D26, 0xD600, 0x1234);
        TEST("TSETB @RR2",         0x0C26);
        TEST("TSETB 0x120034",     0x4C06, 0x1234);
        TEST("TSETB 0x561234",     0x4C06, 0xD600, 0x1234);
        TEST("TSETB 0x120034(R2)", 0x4C26, 0x1234);
        TEST("TSETB 0x561234(R2)", 0x4C26, 0xD600, 0x1234);
    } else {
        TEST("TSET  @R2",        0x0D26);
        TEST("TSET  0x1234",     0x4D06, 0x1234);
        TEST("TSET  0x1234(R2)", 0x4D26, 0x1234);
        TEST("TSETB @R2",        0x0C26);
        TEST("TSETB 0x1234",     0x4C06, 0x1234);
        TEST("TSETB 0x1234(R2)", 0x4C26, 0x1234);
    }

    // Test Conditions Code
    TEST("TCC F,R15",   0xAFF0);
    TEST("TCC LT,R14",  0xAFE1);
    TEST("TCC LE,R13",  0xAFD2);
    TEST("TCC ULE,R12", 0xAFC3);
    TEST("TCC OV,R11",  0xAFB4);
    TEST("TCC MI,R10",  0xAFA5);
    TEST("TCC Z,R9",    0xAF96);
    TEST("TCC C,R8",    0xAF87);
    TEST("TCC R7",      0xAF78);
    TEST("TCC GE,R6",   0xAF69);
    TEST("TCC GT,R5",   0xAF5A);
    TEST("TCC UGT,R4",  0xAF4B);
    TEST("TCC NOV,R3",  0xAF3C);
    TEST("TCC PL,R2",   0xAF2D);
    TEST("TCC NZ,R1",   0xAF1E);
    TEST("TCC NC,R0",   0xAF0F);
    TEST("TCCB F,RL7",   0xAEF0);
    TEST("TCCB LT,RL6",  0xAEE1);
    TEST("TCCB LE,RL5",  0xAED2);
    TEST("TCCB ULE,RL4", 0xAEC3);
    TEST("TCCB OV,RL3",  0xAEB4);
    TEST("TCCB MI,RL2",  0xAEA5);
    TEST("TCCB Z,RL1",   0xAE96);
    TEST("TCCB C,RL0",   0xAE87);
    TEST("TCCB RH7",     0xAE78);
    TEST("TCCB GE,RH6",  0xAE69);
    TEST("TCCB GT,RH5",  0xAE5A);
    TEST("TCCB UGT,RH4", 0xAE4B);
    TEST("TCCB NOV,RH3", 0xAE3C);
    TEST("TCCB PL,RH2",  0xAE2D);
    TEST("TCCB NZ,RH1",  0xAE1E);
    TEST("TCCB NC,RH0",  0xAE0F);
}

static void test_rotate() {
    // Rotate Left
    TEST("RL  R1,#1",  0xB310);
    TEST("RL  R8,#2",  0xB382);
    ERRT("RL  R8,#0",  OPERAND_NOT_ALLOWED);
    ERRT("RL  R8,#3",  OVERFLOW_RANGE);
    TEST("RLB RH1,#1", 0xB210);
    TEST("RLB RL0,#2", 0xB282);

    // Rotate Left through Carry
    TEST("RLC  R1,#1",  0xB318);
    TEST("RLC  R8,#2",  0xB38A);
    TEST("RLCB RH1,#1", 0xB218);
    TEST("RLCB RL0,#2", 0xB28A);

    // Rotate Left Digit
    TEST("RLDB RH1,RL0", 0xBE81);

    // Rotate Right
    TEST("RR  R1,#1",  0xB314);
    TEST("RR  R8,#2",  0xB386);
    TEST("RRB RH1,#1", 0xB214);
    TEST("RRB RL0,#2", 0xB286);

    // Rotate Right through Carry
    TEST("RRC  R1,#1",  0xB31C);
    TEST("RRC  R8,#2",  0xB38E);
    TEST("RRCB RH1,#1", 0xB21C);
    TEST("RRCB RL0,#2", 0xB28E);

    // Rotate Right Digit
    TEST("RRDB RL7,RH7", 0xBC7F);
}

static void test_shift() {
    // Shift Dynamic Arithmetic
    TEST("SDA  R1,R2",   0xB31B, 0x0200);
    TEST("SDAB RL1,R15", 0xB29B, 0x0F00);
    TEST("SDAL RR2,R8",  0xB32F, 0x0800);

    // Shift Dynamic Logical
    TEST("SDL  R1,R2",   0xB313, 0x0200);
    TEST("SDLB RL1,R15", 0xB293, 0x0F00);
    TEST("SDLL RR2,R8",  0xB327, 0x0800);

    // Shift Left Arithmetic
    TEST("SLA  R2,#16" ,  0xB329, 0x0010);
    TEST("SLAB RH2,#8",   0xB229, 0x0008);
    TEST("SLAL RR2,#32",  0xB32D, 0x0020);
    TEST("SLA  R2,#0",    0xB329, 0x0000);
    TEST("SLAB RH2,#0",   0xB229, 0x0000);
    TEST("SLAL RR2,#0",   0xB32D, 0x0000);
    ERRT("SLA  R2,#17",   OVERFLOW_RANGE);
    ERRT("SLAB R2,#9",    OVERFLOW_RANGE);
    ERRT("SLAL R2,#33",   OVERFLOW_RANGE);
    ERRT("SLA  R2,#-1",   OVERFLOW_RANGE);
    ERRT("SLAB RH2,#-1",  OVERFLOW_RANGE);
    ERRT("SLAL RR2,#-1",  OVERFLOW_RANGE);

    // Shift Right Arithmetic
    TEST("SRA  R2,#16",   0xB329, 0xFFF0);
    TEST("SRAB RH2,#8",   0xB229, 0x00F8);
    TEST("SRAL RR2,#32",  0xB32D, 0xFFE0);
    TEST("SRA  R2,#0",    0xB329, 0x0000);
    TEST("SRAB RH2,#0",   0xB229, 0x0000);
    TEST("SRAL RR2,#0",   0xB32D, 0x0000);
    ERRT("SRA  R2,#17",   OVERFLOW_RANGE);
    ERRT("SRAB R2,#9",    OVERFLOW_RANGE);
    ERRT("SRAL R2,#33",   OVERFLOW_RANGE);
    ERRT("SRA  R2,#-1",   OVERFLOW_RANGE);
    ERRT("SRAB RH2,#-1",  OVERFLOW_RANGE);
    ERRT("SRAL RR2,#-1",  OVERFLOW_RANGE);

    // Shift Left Logical
    TEST("SLL  R2,#16",  0xB321, 0x0010);
    TEST("SLLB RH2,#8",  0xB221, 0x0008);
    TEST("SLLL RR2,#32", 0xB325, 0x0020);
    ERRT("SLL  R2,#17",  OVERFLOW_RANGE);
    ERRT("SLLB R2,#9",   OVERFLOW_RANGE);
    ERRT("SLLL R2,#33",  OVERFLOW_RANGE);

    // Shift Right Logical
    TEST("SRL  R2,#16",  0xB321, 0xFFF0);
    TEST("SRLB RH2,#8",  0xB221, 0x00F8);
    TEST("SRLL RR2,#32", 0xB325, 0xFFE0);
    ERRT("SRL  R2,#17",  OVERFLOW_RANGE);
    ERRT("SRLB R2,#9",   OVERFLOW_RANGE);
    ERRT("SRLL R2,#33",  OVERFLOW_RANGE);
}

static void test_block_transfer() {
    // Compare and Decrement
    if (z8001()) {
        TEST("CPD  R0,@RR2,R4,F",   0xBB28, 0x0400);
        TEST("CPDB RL0,@RR2,R4,LT", 0xBA28, 0x0481);
        ERRT("CPD  R2,@RR0,R4,F",   REGISTER_NOT_ALLOWED);
        ERRT("CPDB RL0,@RR0,R4,LT", REGISTER_NOT_ALLOWED);
    } else {
        TEST("CPD  R0,@R1,R2,F",   0xBB18, 0x0200);
        TEST("CPDB RL0,@R1,R2,LT", 0xBA18, 0x0281);
        ERRT("CPD  R2,@R0,R4,F",   REGISTER_NOT_ALLOWED);
        ERRT("CPDB RL0,@R0,R4,LT", REGISTER_NOT_ALLOWED);
    }

    // Compare, Decrement, and Repeat
    if (z8001()) {
        TEST("CPDR  R1,@RR4,R3,LE",   0xBB4C, 0x0312);
        TEST("CPDRB RL1,@RR4,R3,ULE", 0xBA4C, 0x0393);
        ERRT("CPDR  R1,@RR0,R4,LE",   REGISTER_NOT_ALLOWED);
        ERRT("CPDRB RL1,@RR0,R4,ULE", REGISTER_NOT_ALLOWED);
    } else {
        TEST("CPDR  R3,@R4,R5,LE",   0xBB4C, 0x0532);
        TEST("CPDRB RL3,@R4,R5,ULE", 0xBA4C, 0x05B3);
        ERRT("CPDR  R1,@R0,R4,LE",   REGISTER_NOT_ALLOWED);
        ERRT("CPDRB RL1,@R0,R4,ULE", REGISTER_NOT_ALLOWED);
    }

    // Compare and Increment
    if (z8001()) {
        TEST("CPI  R2,@RR6,R4,OV", 0xBB60, 0x0424);
        TEST("CPIB RL2,@RR6,R4",   0xBA60, 0x04A8);
        ERRT("CPI  R1,@RR0,R4,OV", REGISTER_NOT_ALLOWED);
        ERRT("CPIB RL1,@RR0,R4",   REGISTER_NOT_ALLOWED);
    } else {
        TEST("CPI  R6,@R7,R8,OV", 0xBB70, 0x0864);
        TEST("CPIB RL6,@R7,R8",   0xBA70, 0x08E8);
        ERRT("CPI  R2,@R0,R4,OV", REGISTER_NOT_ALLOWED);
        ERRT("CPIB RL0,@R0,R4",   REGISTER_NOT_ALLOWED);
    }

    // Compare, Increment, and Repeat
    if (z8001()) {
        TEST("CPIR  R3,@RR8,R5,Z",  0xBB84, 0x0536);
        TEST("CPIRB RL3,@RR8,R5,C", 0xBA84, 0x05B7);
        ERRT("CPIR  R1,@RR0,R4,Z",  REGISTER_NOT_ALLOWED);
        ERRT("CPIRB RL1,@RR0,R4,C", REGISTER_NOT_ALLOWED);
    } else {
        TEST("CPIR  R9,@R10,R11,Z",  0xBBA4, 0x0B96);
        TEST("CPIRB RH1,@R10,R11,C", 0xBAA4, 0x0B17);
        ERRT("CPIR  R1,@R0,R4,LE",   REGISTER_NOT_ALLOWED);
        ERRT("CPIRB RL1,@R0,R4,ULE", REGISTER_NOT_ALLOWED);
    }

    // Load and Decrement
    if (z8001()) {
        TEST("LDD  @RR2,@RR4,R6", 0xBB49, 0x0628);
        TEST("LDDB @RR2,@RR4,R6", 0xBA49, 0x0628);
        ERRT("LDD  @RR0,@RR4,R6", REGISTER_NOT_ALLOWED);
        ERRT("LDDB @RR0,@RR4,R6", REGISTER_NOT_ALLOWED);
        ERRT("LDD  @RR2,@RR0,R6", REGISTER_NOT_ALLOWED);
        ERRT("LDDB @RR2,@RR0,R6", REGISTER_NOT_ALLOWED);
    } else {
        TEST("LDD  @R1,@R2,R3", 0xBB29, 0x0318);
        TEST("LDDB @R1,@R2,R3", 0xBA29, 0x0318);
        ERRT("LDD  @R0,@R2,R3", REGISTER_NOT_ALLOWED);
        ERRT("LDDB @R0,@R2,R3", REGISTER_NOT_ALLOWED);
        ERRT("LDD  @R1,@R0,R3", REGISTER_NOT_ALLOWED);
        ERRT("LDDB @R1,@R0,R3", REGISTER_NOT_ALLOWED);
    }

    // Load, Decrement, and Repeat
    if (z8001()) {
        TEST("LDDR  @RR4,@RR6,R8", 0xBB69, 0x0840);
        TEST("LDDRB @RR4,@RR6,R8", 0xBA69, 0x0840);
        ERRT("LDDR  @RR0,@RR6,R8", REGISTER_NOT_ALLOWED);
        ERRT("LDDRB @RR0,@RR6,R8", REGISTER_NOT_ALLOWED);
        ERRT("LDDR  @RR4,@RR0,R8", REGISTER_NOT_ALLOWED);
        ERRT("LDDRB @RR4,@RR0,R8", REGISTER_NOT_ALLOWED);
    } else {
        TEST("LDDR  @R4,@R5,R6", 0xBB59, 0x0640);
        TEST("LDDRB @R4,@R5,R6", 0xBA59, 0x0640);
        ERRT("LDDR  @R0,@R5,R6", REGISTER_NOT_ALLOWED);
        ERRT("LDDRB @R0,@R5,R6", REGISTER_NOT_ALLOWED);
        ERRT("LDDR  @R4,@R0,R6", REGISTER_NOT_ALLOWED);
        ERRT("LDDRB @R4,@R0,R6", REGISTER_NOT_ALLOWED);
    }

    // Load and Increment
    if (z8001()) {
        TEST("LDI  @RR6,@RR8,R10", 0xBB81, 0x0A68);
        TEST("LDIB @RR6,@RR8,R10", 0xBA81, 0x0A68);
        ERRT("LDI  @RR0,@RR8,R10", REGISTER_NOT_ALLOWED);
        ERRT("LDIB @RR0,@RR8,R10", REGISTER_NOT_ALLOWED);
        ERRT("LDI  @RR6,@RR0,R10", REGISTER_NOT_ALLOWED);
        ERRT("LDIB @RR6,@RR0,R10", REGISTER_NOT_ALLOWED);
    } else {
        TEST("LDI  @R7,@R8,R9", 0xBB81, 0x0978);
        TEST("LDIB @R7,@R8,R9", 0xBA81, 0x0978);
        ERRT("LDI  @R0,@R8,R9", REGISTER_NOT_ALLOWED);
        ERRT("LDIB @R0,@R8,R9", REGISTER_NOT_ALLOWED);
        ERRT("LDI  @R7,@R0,R9", REGISTER_NOT_ALLOWED);
        ERRT("LDIB @R7,@R0,R9", REGISTER_NOT_ALLOWED);
    }

    // Load, Increment, and Repeat
    if (z8001()) {
        TEST("LDIR  @RR8,@RR10,R12", 0xBBA1, 0x0C80);
        TEST("LDIRB @RR8,@RR10,R12", 0xBAA1, 0x0C80);
        ERRT("LDIR  @RR0,@RR10,R12", REGISTER_NOT_ALLOWED);
        ERRT("LDIRB @RR0,@RR10,R12", REGISTER_NOT_ALLOWED);
        ERRT("LDIR  @RR8,@RR0,R12",  REGISTER_NOT_ALLOWED);
        ERRT("LDIRB @RR8,@RR0,R12",  REGISTER_NOT_ALLOWED);
    } else {
        TEST("LDIR  @R10,@R11,R12", 0xBBB1, 0x0CA0);
        TEST("LDIRB @R10,@R11,R12", 0xBAB1, 0x0CA0);
        ERRT("LDIR  @R0,@R11,R12",  REGISTER_NOT_ALLOWED);
        ERRT("LDIRB @R0,@R11,R12",  REGISTER_NOT_ALLOWED);
        ERRT("LDIR  @R10,@R0,R12",  REGISTER_NOT_ALLOWED);
        ERRT("LDIRB @R10,@R0,R12",  REGISTER_NOT_ALLOWED);
    }
}

static void test_string_manipulation() {
    // Compare String and Decrement
    if (z8001()) {
        TEST("CPSD  @RR4,@RR2,R0",    0xBB2A, 0x0048);
        TEST("CPSDB @RR4,@RR2,R6,GE", 0xBA2A, 0x0649);
        ERRT("CPSD  @RR0,@RR2,R0",    REGISTER_NOT_ALLOWED);
        ERRT("CPSD  @RR4,@RR0,R0",    REGISTER_NOT_ALLOWED);
        ERRT("CPSDB @RR0,@RR2,R4,GE", REGISTER_NOT_ALLOWED);
        ERRT("CPSDB @RR2,@RR0,R4,GE", REGISTER_NOT_ALLOWED);
    } else {
        TEST("CPSD  @R3,@R1,R2",    0xBB1A, 0x0238);
        TEST("CPSDB @R3,@R1,R2,GE", 0xBA1A, 0x0239);
        ERRT("CPSD  @R3,@R0,R2",    REGISTER_NOT_ALLOWED);
        ERRT("CPSD  @R0,@R3,R2",    REGISTER_NOT_ALLOWED);
        ERRT("CPSDB @R3,@R0,R2,GE", REGISTER_NOT_ALLOWED);
        ERRT("CPSDB @R0,@R3,R2,GE", REGISTER_NOT_ALLOWED);
    }

    // Compare String, Decrement, and Repeat
    if (z8001()) {
        TEST("CPSDR  @RR2,@RR4,R6,GT",  0xBB4E, 0x062A);
        TEST("CPSDRB @RR2,@RR4,R6,UGT", 0xBA4E, 0x062B);
        ERRT("CPSDR  @RR0,@RR4,R5,GT",  REGISTER_NOT_ALLOWED);
        ERRT("CPSDR  @RR2,@RR0,R5,GT",  REGISTER_NOT_ALLOWED);
        ERRT("CPSDRB @RR0,@RR4,R5,UGT", REGISTER_NOT_ALLOWED);
        ERRT("CPSDRB @RR2,@RR0,R5,UGT", REGISTER_NOT_ALLOWED);
    } else {
        TEST("CPSDR  @R3,@R4,R5,GT",  0xBB4E, 0x053A);
        TEST("CPSDRB @R3,@R4,R0,UGT", 0xBA4E, 0x003B);
        ERRT("CPSDR  @R0,@R4,R5,GT",  REGISTER_NOT_ALLOWED);
        ERRT("CPSDR  @R3,@R0,R5,GT",  REGISTER_NOT_ALLOWED);
        ERRT("CPSDRB @R0,@R4,R0,UGT", REGISTER_NOT_ALLOWED);
        ERRT("CPSDRB @R3,@R0,R0,UGT", REGISTER_NOT_ALLOWED);
    }

    // Compare String and Increment
    if (z8001()) {
        TEST("CPSI  @RR4,@RR6,R2,NOV", 0xBB62, 0x024C);
        TEST("CPSIB @RR4,@RR6,R2,PL",  0xBA62, 0x024D);
        ERRT("CPSI  @RR0,@RR6,R5,NOV", REGISTER_NOT_ALLOWED);
        ERRT("CPSI  @RR4,@RR0,R5,NOV", REGISTER_NOT_ALLOWED);
        ERRT("CPSIB @RR0,@RR6,R5,PL",  REGISTER_NOT_ALLOWED);
        ERRT("CPSIB @RR4,@RR0,R5,PL",  REGISTER_NOT_ALLOWED);
    } else {
        TEST("CPSI  @R6,@R7,R8,NOV", 0xBB72, 0x086C);
        TEST("CPSIB @R6,@R7,R8,PL",  0xBA72, 0x086D);
        ERRT("CPSI  @R0,@R7,R8,NOV", REGISTER_NOT_ALLOWED);
        ERRT("CPSI  @R6,@R0,R8,NOV", REGISTER_NOT_ALLOWED);
        ERRT("CPSIB @R0,@R7,R8,PL",  REGISTER_NOT_ALLOWED);
        ERRT("CPSIB @R6,@R0,R8,PL",  REGISTER_NOT_ALLOWED);
    }

    // Compare String, Increment, and Repeat
    if (z8001()) {
        TEST("CPSIR  @RR4,@RR8,R6,NZ", 0xBB86, 0x064E);
        TEST("CPSIRB @RR4,@RR8,R6,NC", 0xBA86, 0x064F);
        ERRT("CPSIR  @RR0,@RR8,R6,NZ", REGISTER_NOT_ALLOWED);
        ERRT("CPSIR  @RR4,@RR0,R6,NZ", REGISTER_NOT_ALLOWED);
        ERRT("CPSIRB @RR0,@RR8,R6,NC", REGISTER_NOT_ALLOWED);
        ERRT("CPSIRB @RR4,@RR0,R6,NC", REGISTER_NOT_ALLOWED);
    } else {
        TEST("CPSIR  @R9,@R10,R11,NZ", 0xBBA6, 0x0B9E);
        TEST("CPSIRB @R1,@R10,R11,NC", 0xBAA6, 0x0B1F);
        ERRT("CPSIR  @R0,@R10,R11,NZ", REGISTER_NOT_ALLOWED);
        ERRT("CPSIR  @R9,@R0,R11,NZ",  REGISTER_NOT_ALLOWED);
        ERRT("CPSIRB @R0,@R10,R11,NC", REGISTER_NOT_ALLOWED);
        ERRT("CPSIRB @R1,@R0,R11,NC",  REGISTER_NOT_ALLOWED);
    }

    // Translate and Decrement
    if (z8001()) {
        TEST("TRDB @RR2,@RR4,R6", 0xB828, 0x0640);
        TEST("TRDB @RR2,@RR4,R0", 0xB828, 0x0040);
        ERRT("TRDB @RR2,@RR4,R1", REGISTER_NOT_ALLOWED);
        ERRT("TRDB @RR0,@RR4,R6", REGISTER_NOT_ALLOWED);
        ERRT("TRDB @RR2,@RR0,R6", REGISTER_NOT_ALLOWED);
    } else {
        ERRT("TRDB @R0,@R2,R3", REGISTER_NOT_ALLOWED);
        ERRT("TRDB @R1,@R2,R3", REGISTER_NOT_ALLOWED);
        ERRT("TRDB @R2,@R0,R3", REGISTER_NOT_ALLOWED);
        ERRT("TRDB @R2,@R1,R3", REGISTER_NOT_ALLOWED);
        ERRT("TRDB @R2,@R2,R3", REGISTERS_OVERWRAPPED);
        TEST("TRDB @R3,@R2,R4", 0xB838, 0x0420);
        TEST("TRDB @R3,@R2,R0", 0xB838, 0x0020);
        ERRT("TRDB @R3,@R2,R1", REGISTER_NOT_ALLOWED);
    }

    // Translate, Decrement, and Repeat
    if (z8001()) {
        TEST("TRDRB @RR4,@RR6,R2", 0xB84C, 0x0260);
        ERRT("TRDRB @RR0,@RR6,R2", REGISTER_NOT_ALLOWED);
        ERRT("TRDRB @RR4,@RR0,R2", REGISTER_NOT_ALLOWED);
    } else {
        TEST("TRDRB @R4,@R5,R6",   0xB84C, 0x0650);
        ERRT("TRDRB @R0,@R5,R6", REGISTER_NOT_ALLOWED);
        ERRT("TRDRB @R4,@R0,R6", REGISTER_NOT_ALLOWED);
    }

    // Translate and Increment
    if (z8001()) {
        TEST("TRIB @RR6,@RR8,R3", 0xB860, 0x0380);
        ERRT("TRIB @RR0,@RR8,R3", REGISTER_NOT_ALLOWED);
        ERRT("TRIB @RR6,@RR0,R3", REGISTER_NOT_ALLOWED);
    } else {
        TEST("TRIB @R7,@R8,R9", 0xB870, 0x0980);
        ERRT("TRIB @R0,@R8,R9", REGISTER_NOT_ALLOWED);
        ERRT("TRIB @R7,@R0,R9", REGISTER_NOT_ALLOWED);
    }

    // Translate, Increment, and Repeat
    if (z8001()) {
        TEST("TRIRB @RR8,@RR10,R12", 0xB884, 0x0CA0);
        ERRT("TRIRB @RR0,@RR10,R12", REGISTER_NOT_ALLOWED);
        ERRT("TRIRB @RR8,@RR0,R12",  REGISTER_NOT_ALLOWED);
    } else {
        TEST("TRIRB @R10,@R11,R12", 0xB8A4, 0x0CB0);
        ERRT("TRIRB @R0,@R11,R12",  REGISTER_NOT_ALLOWED);
        ERRT("TRIRB @R10,@R0,R12",  REGISTER_NOT_ALLOWED);
    }

    // Translate, Test, and Decrement
    if (z8001()) {
        TEST("TRTDB @RR10,@RR12,R9", 0xB8AA, 0x09C0);
        ERRT("TRTDB @RR0,@RR12,R9",  REGISTER_NOT_ALLOWED);
        ERRT("TRTDB @RR10,@RR0,R9",  REGISTER_NOT_ALLOWED);
    } else {
        TEST("TRTDB @R13,@R14,R15", 0xB8DA, 0x0FE0);
        ERRT("TRTDB @R0,@R14,R15",  REGISTER_NOT_ALLOWED);
        ERRT("TRTDB @R13,@R0,R15",  REGISTER_NOT_ALLOWED);
    }

    // Translate, Test, Decrement, and Repeat
    if (z8001()) {
        TEST("TRTDRB @RR12,@RR14,R0", 0xB8CE, 0x00EE);
        ERRT("TRTDRB @RR0,@RR14,R0",  REGISTER_NOT_ALLOWED);
        ERRT("TRTDRB @RR12,@RR0,R0",  REGISTER_NOT_ALLOWED);
    } else {
        TEST("TRTDRB @R15,@R14,R13", 0xB8FE, 0x0DEE);
        ERRT("TRTDRB @R0,@R14,R13",  REGISTER_NOT_ALLOWED);
        ERRT("TRTDRB @R15,@R0,R13",  REGISTER_NOT_ALLOWED);
    }

    // Translate, Test, and Increment
    if (z8001()) {
        TEST("TRTIB @RR14,@RR12,R11", 0xB8E2, 0x0BC0);
        ERRT("TRTIB @RR0,@RR12,R11",  REGISTER_NOT_ALLOWED);
        ERRT("TRTIB @RR14,@RR0,R11",  REGISTER_NOT_ALLOWED);
    } else {
        TEST("TRTIB @R12,@R11,R10", 0xB8C2, 0x0AB0);
        ERRT("TRTIB @R0,@R11,R10",  REGISTER_NOT_ALLOWED);
        ERRT("TRTIB @R12,@R0,R10",  REGISTER_NOT_ALLOWED);
    }

    // Translate, Test, Increment, and Repeat
    if (z8001()) {
        TEST("TRTIRB @RR10,@RR8,R7", 0xB8A6, 0x078E);
        ERRT("TRTIRB @RR0,@RR8,R7",  REGISTER_NOT_ALLOWED);
        ERRT("TRTIRB @RR10,@RR0,R7", REGISTER_NOT_ALLOWED);
    } else {
        TEST("TRTIRB @R9,@R8,R7", 0xB896, 0x078E);
        ERRT("TRTIRB @R0,@R8,R7", REGISTER_NOT_ALLOWED);
        ERRT("TRTIRB @R9,@R0,R7", REGISTER_NOT_ALLOWED);
    }
}

static void test_input() {
    // Input
    TEST("IN  R1,@R2",      0x3D21);
    TEST("INB RH1,@R2",     0x3C21);
    ERRT("IN  R1,@R0",      REGISTER_NOT_ALLOWED);
    ERRT("INB RH1,@R0",     REGISTER_NOT_ALLOWED);
    TEST("IN  R1, 0x1234",  0x3B14, 0x1234);
    TEST("INB RH1, 0x1234", 0x3A14, 0x1234);
    TEST("IN  R1, 0xFFFF",  0x3B14, 0xFFFF);
    ERRT("IN  R1, 0x10000", OVERFLOW_RANGE);
    // GNU as compatibility
    TEST("IN  R1,#0x1234",  0x3B14, 0x1234);
    TEST("INB RH1,#0x1234", 0x3A14, 0x1234);

    // Special Input
    TEST("SIN  R1, 0x1234",  0x3B15, 0x1234);
    TEST("SINB RH1, 0x1234", 0x3A15, 0x1234);
    // GNU as compatibility
    TEST("SIN  R1,#0x1234",  0x3B15, 0x1234);
    TEST("SINB RH1,#0x1234", 0x3A15, 0x1234);

    // Input and Decrement
    if (z8001()) {
        TEST("IND  @RR2,@R4,R6", 0x3B48, 0x0628);
        TEST("INDB @RR2,@R4,R6", 0x3A48, 0x0628);
        ERRT("IND  @RR0,@R4,R6", REGISTER_NOT_ALLOWED);
        ERRT("IND  @RR2,@R0,R6", REGISTER_NOT_ALLOWED);
        ERRT("INDB @RR0,@R4,R6", REGISTER_NOT_ALLOWED);
        ERRT("INDB @RR2,@R0,R6", REGISTER_NOT_ALLOWED);
    } else {
        TEST("IND  @R1,@R2,R0", 0x3B28, 0x0018);
        TEST("INDB @R1,@R2,R0", 0x3A28, 0x0018);
        ERRT("IND  @R0,@R2,R0", REGISTER_NOT_ALLOWED);
        ERRT("IND  @R1,@R0,R0", REGISTER_NOT_ALLOWED);
        ERRT("INDB @R0,@R2,R0", REGISTER_NOT_ALLOWED);
        ERRT("INDB @R1,@R0,R0", REGISTER_NOT_ALLOWED);
    }

    // Special Input and Decrement
    if (z8001()) {
        TEST("SIND  @RR4,@R3,R0", 0x3B39, 0x0048);
        TEST("SINDB @RR4,@R3,R0", 0x3A39, 0x0048);
        ERRT("SIND  @RR0,@R3,R0", REGISTER_NOT_ALLOWED);
        ERRT("SIND  @RR4,@R0,R0", REGISTER_NOT_ALLOWED);
        ERRT("SINDB @RR0,@R3,R0", REGISTER_NOT_ALLOWED);
        ERRT("SINDB @RR4,@R0,R0", REGISTER_NOT_ALLOWED);
    } else {
        TEST("SIND  @R3,@R4,R5", 0x3B49, 0x0538);
        TEST("SINDB @R3,@R4,R5", 0x3A49, 0x0538);
        ERRT("SIND  @R0,@R4,R5", REGISTER_NOT_ALLOWED);
        ERRT("SIND  @R3,@R0,R5", REGISTER_NOT_ALLOWED);
        ERRT("SINDB @R0,@R4,R5", REGISTER_NOT_ALLOWED);
        ERRT("SINDB @R3,@R0,R5", REGISTER_NOT_ALLOWED);
    }

    // Input, Decrement, and Repeat
    if (z8001()) {
        TEST("INDR  @RR6,@R4,R8", 0x3B48, 0x0860);
        TEST("INDRB @RR6,@R4,R8", 0x3A48, 0x0860);
        ERRT("INDR  @RR0,@R4,R8", REGISTER_NOT_ALLOWED);
        ERRT("INDR  @RR6,@R0,R8", REGISTER_NOT_ALLOWED);
        ERRT("INDRB @RR0,@R4,R8", REGISTER_NOT_ALLOWED);
        ERRT("INDRB @RR6,@R0,R8", REGISTER_NOT_ALLOWED);
    } else {
        TEST("INDR  @R6,@R7,R8", 0x3B78, 0x0860);
        TEST("INDRB @R6,@R7,R8", 0x3A78, 0x0860);
        ERRT("INDR  @R0,@R7,R8", REGISTER_NOT_ALLOWED);
        ERRT("INDR  @R6,@R0,R8", REGISTER_NOT_ALLOWED);
        ERRT("INDRB @R0,@R7,R8", REGISTER_NOT_ALLOWED);
        ERRT("INDRB @R6,@R0,R8", REGISTER_NOT_ALLOWED);
    }

    // Special Input, Decrement, and Repeat
    if (z8001()) {
        TEST("SINDR  @RR8,@R10,R12", 0x3BA9, 0x0C80);
        TEST("SINDRB @RR8,@R10,R12", 0x3AA9, 0x0C80);
        ERRT("SINDR  @RR0,@R10,R12", REGISTER_NOT_ALLOWED);
        ERRT("SINDR  @RR8,@R0,R12",  REGISTER_NOT_ALLOWED);
        ERRT("SINDRB @RR0,@R10,R12", REGISTER_NOT_ALLOWED);
        ERRT("SINDRB @RR8,@R0,R12",  REGISTER_NOT_ALLOWED);
    } else {
        TEST("SINDR  @R9,@R10,R11", 0x3BA9, 0x0B90);
        TEST("SINDRB @R9,@R10,R11", 0x3AA9, 0x0B90);
        ERRT("SINDR  @R0,@R10,R11", REGISTER_NOT_ALLOWED);
        ERRT("SINDR  @R9,@R0,R11",  REGISTER_NOT_ALLOWED);
        ERRT("SINDRB @R0,@R10,R11", REGISTER_NOT_ALLOWED);
        ERRT("SINDRB @R9,@R0,R11",  REGISTER_NOT_ALLOWED);
    }

    // Input and Increment
    if (z8001()) {
        TEST("INI  @RR14,@R2,R1", 0x3B20, 0x01E8);
        TEST("INIB @RR14,@R2,R1", 0x3A20, 0x01E8);
        ERRT("INI  @RR0,@R2,R1",  REGISTER_NOT_ALLOWED);
        ERRT("INI  @RR14,@R0,R1", REGISTER_NOT_ALLOWED);
        ERRT("INIB @RR0,@R2,R1",  REGISTER_NOT_ALLOWED);
        ERRT("INIB @RR14,@R0,R1", REGISTER_NOT_ALLOWED);
    } else {
        TEST("INI  @R12,@R13,R14", 0x3BD0, 0x0EC8);
        TEST("INIB @R12,@R13,R14", 0x3AD0, 0x0EC8);
        ERRT("INI  @R0,@R13,R14",  REGISTER_NOT_ALLOWED);
        ERRT("INI  @R12,@R0,R14",  REGISTER_NOT_ALLOWED);
        ERRT("INIB @R0,@R13,R14",  REGISTER_NOT_ALLOWED);
        ERRT("INIB @R12,@R0,R14",  REGISTER_NOT_ALLOWED);
    }

    // Special Input and Increment
    if (z8001()) {
        TEST("SINI  @RR12,@R11,R10", 0x3BB1, 0x0AC8);
        TEST("SINIB @RR12,@R11,R10", 0x3AB1, 0x0AC8);
        ERRT("SINI  @RR0,@R11,R10",  REGISTER_NOT_ALLOWED);
        ERRT("SINI  @RR12,@R0,R10",  REGISTER_NOT_ALLOWED);
        ERRT("SINIB @RR0,@R11,R10",  REGISTER_NOT_ALLOWED);
        ERRT("SINIB @RR12,@R0,R10",  REGISTER_NOT_ALLOWED);
    } else {
        TEST("SINI  @R15,@R1,R0", 0x3B11, 0x00F8);
        TEST("SINIB @R15,@R1,R0", 0x3A11, 0x00F8);
        ERRT("SINI  @R0,@R1,R0",  REGISTER_NOT_ALLOWED);
        ERRT("SINI  @R15,@R0,R0", REGISTER_NOT_ALLOWED);
        ERRT("SINIB @R0,@R1,R0",  REGISTER_NOT_ALLOWED);
        ERRT("SINIB @R15,@R0,R0", REGISTER_NOT_ALLOWED);
    }

    // Input, Increment and Repeat
    if (z8001()) {
        TEST("INIR  @RR10,@R9,R8", 0x3B90, 0x08A0);
        TEST("INIRB @RR10,@R9,R8", 0x3A90, 0x08A0);
        ERRT("INIR  @RR0,@R9,R8",  REGISTER_NOT_ALLOWED);
        ERRT("INIR  @RR10,@R0,R8", REGISTER_NOT_ALLOWED);
        ERRT("INIRB @RR0,@R9,R8",  REGISTER_NOT_ALLOWED);
        ERRT("INIRB @RR10,@R0,R8", REGISTER_NOT_ALLOWED);
    } else {
        TEST("INIR  @R3,@R2,R1", 0x3B20, 0x0130);
        TEST("INIRB @R3,@R2,R1", 0x3A20, 0x0130);
        ERRT("INIR  @R0,@R2,R1", REGISTER_NOT_ALLOWED);
        ERRT("INIR  @R3,@R0,R1", REGISTER_NOT_ALLOWED);
        ERRT("INIRB @R0,@R2,R1", REGISTER_NOT_ALLOWED);
        ERRT("INIRB @R3,@R0,R1", REGISTER_NOT_ALLOWED);
    }

    // Special Input, Increment, and Repeat
    if (z8001()) {
        TEST("SINIR  @RR8,@R7,R6", 0x3B71, 0x0680);
        TEST("SINIRB @RR8,@R7,R6", 0x3A71, 0x0680);
        ERRT("SINIR  @RR0,@R7,R6", REGISTER_NOT_ALLOWED);
        ERRT("SINIR  @RR8,@R0,R6", REGISTER_NOT_ALLOWED);
        ERRT("SINIRB @RR0,@R7,R6", REGISTER_NOT_ALLOWED);
        ERRT("SINIRB @RR8,@R0,R6", REGISTER_NOT_ALLOWED);
    } else {
        TEST("SINIR  @R4,@R3,R2", 0x3B31, 0x0240);
        TEST("SINIRB @R4,@R3,R2", 0x3A31, 0x0240);
        ERRT("SINIR  @R0,@R3,R2", REGISTER_NOT_ALLOWED);
        ERRT("SINIR  @R4,@R0,R2", REGISTER_NOT_ALLOWED);
        ERRT("SINIRB @R0,@R3,R2", REGISTER_NOT_ALLOWED);
        ERRT("SINIRB @R4,@R0,R2", REGISTER_NOT_ALLOWED);
    }
}

static void test_output() {
    // Output
    TEST("OUT  @R2,R1",      0x3F21);
    TEST("OUTB @R2,RH1",     0x3E21);
    ERRT("OUT  @R0,R1",      REGISTER_NOT_ALLOWED);
    ERRT("OUTB @R0,RH1",     REGISTER_NOT_ALLOWED);
    TEST("OUT   0x1234,R1",  0x3B16, 0x1234);
    TEST("OUTB  0x1234,RH1", 0x3A16, 0x1234);
    TEST("OUT   0xFFFF,R1",  0x3B16, 0xFFFF);
    ERRT("OUT  0x10000,R1",  OVERFLOW_RANGE);
    // GNU as compatibility
    TEST("OUT  #0x1234,R1",  0x3B16, 0x1234);
    TEST("OUTB #0x1234,RH1", 0x3A16, 0x1234);

    // Special Output
    TEST("SOUT   0x1234,R1",  0x3B17, 0x1234);
    TEST("SOUTB  0x1234,RH1", 0x3A17, 0x1234);
    // GNU as compatibility
    TEST("SOUT  #0x1234,R1",  0x3B17, 0x1234);
    TEST("SOUTB #0x1234,RH1", 0x3A17, 0x1234);

    // Output and Decrement
    if (z8001()) {
        TEST("OUTD  @R2,@RR4,R6", 0x3B4A, 0x0628);
        TEST("OUTDB @R2,@RR4,R6", 0x3A4A, 0x0628);
        ERRT("OUTD  @R0,@RR4,R6", REGISTER_NOT_ALLOWED);
        ERRT("OUTD  @R2,@RR0,R6", REGISTER_NOT_ALLOWED);
        ERRT("OUTDB @R0,@RR4,R6", REGISTER_NOT_ALLOWED);
        ERRT("OUTDB @R2,@RR0,R6", REGISTER_NOT_ALLOWED);
    } else {
        TEST("OUTD  @R2,@R1,R0", 0x3B1A, 0x0028);
        TEST("OUTDB @R2,@R1,R0", 0x3A1A, 0x0028);
        ERRT("OUTD  @R0,@R1,R0", REGISTER_NOT_ALLOWED);
        ERRT("OUTD  @R2,@R0,R0", REGISTER_NOT_ALLOWED);
        ERRT("OUTDB @R0,@R1,R0", REGISTER_NOT_ALLOWED);
        ERRT("OUTDB @R2,@R0,R0", REGISTER_NOT_ALLOWED);
    }

    // Special Output and Decrement
    if (z8001()) {
        TEST("SOUTD  @R3,@RR4,R0", 0x3B4B, 0x0038);
        TEST("SOUTDB @R3,@RR4,R0", 0x3A4B, 0x0038);
        ERRT("SOUTD  @R0,@RR4,R0", REGISTER_NOT_ALLOWED);
        ERRT("SOUTD  @R3,@RR0,R0", REGISTER_NOT_ALLOWED);
        ERRT("SOUTDB @R0,@RR4,R0", REGISTER_NOT_ALLOWED);
        ERRT("SOUTDB @R3,@RR0,R0", REGISTER_NOT_ALLOWED);
    } else {
        TEST("SOUTD  @R3,@R4,R5", 0x3B4B, 0x0538);
        TEST("SOUTDB @R3,@R4,R5", 0x3A4B, 0x0538);
        ERRT("SOUTD  @R0,@R4,R5", REGISTER_NOT_ALLOWED);
        ERRT("SOUTD  @R3,@R0,R5", REGISTER_NOT_ALLOWED);
        ERRT("SOUTDB @R0,@R4,R5", REGISTER_NOT_ALLOWED);
        ERRT("SOUTDB @R3,@R0,R5", REGISTER_NOT_ALLOWED);
    }

    // Output, Decrement, and Repeat
    if (z8001()) {
        TEST("OTDR  @R4,@RR6,R8", 0x3B6A, 0x0840);
        TEST("OTDRB @R4,@RR6,R8", 0x3A6A, 0x0840);
        ERRT("OTDR  @R0,@RR6,R8", REGISTER_NOT_ALLOWED);
        ERRT("OTDR  @R4,@RR0,R8", REGISTER_NOT_ALLOWED);
        ERRT("OTDRB @R0,@RR6,R8", REGISTER_NOT_ALLOWED);
        ERRT("OTDRB @R4,@RR0,R8", REGISTER_NOT_ALLOWED);
    } else {
        TEST("OTDR  @R6,@R7,R8", 0x3B7A, 0x0860);
        TEST("OTDRB @R6,@R7,R8", 0x3A7A, 0x0860);
        ERRT("OTDR  @R0,@R7,R8", REGISTER_NOT_ALLOWED);
        ERRT("OTDR  @R6,@R0,R8", REGISTER_NOT_ALLOWED);
        ERRT("OTDRB @R0,@R7,R8", REGISTER_NOT_ALLOWED);
        ERRT("OTDRB @R6,@R0,R8", REGISTER_NOT_ALLOWED);
    }

    // Special Output, Decrement, and Repeat
    if (z8001()) {
        TEST("SOTDR  @R8,@RR10,R12", 0x3BAB, 0x0C80);
        TEST("SOTDRB @R8,@RR10,R12", 0x3AAB, 0x0C80);
        ERRT("SOTDR  @R0,@RR10,R12", REGISTER_NOT_ALLOWED);
        ERRT("SOTDR  @R8,@RR0,R12",  REGISTER_NOT_ALLOWED);
        ERRT("SOTDRB @R0,@RR10,R12", REGISTER_NOT_ALLOWED);
        ERRT("SOTDRB @R8,@RR0,R12",  REGISTER_NOT_ALLOWED);
    } else {
        TEST("SOTDR  @R9,@R10,R11", 0x3BAB, 0x0B90);
        TEST("SOTDRB @R9,@R10,R11", 0x3AAB, 0x0B90);
        ERRT("SOTDR  @R0,@R10,R11", REGISTER_NOT_ALLOWED);
        ERRT("SOTDR  @R9,@R0,R11",  REGISTER_NOT_ALLOWED);
        ERRT("SOTDRB @R0,@R10,R11", REGISTER_NOT_ALLOWED);
        ERRT("SOTDRB @R9,@R0,R11",  REGISTER_NOT_ALLOWED);
    }

    // Output and Increment
    if (z8001()) {
        TEST("OUTI  @R14,@RR2,R1", 0x3B22, 0x01E8);
        TEST("OUTIB @R14,@RR2,R1", 0x3A22, 0x01E8);
        ERRT("OUTI  @R0,@RR2,R1",  REGISTER_NOT_ALLOWED);
        ERRT("OUTI  @R14,@RR0,R1", REGISTER_NOT_ALLOWED);
        ERRT("OUTIB @R0,@RR2,R1",  REGISTER_NOT_ALLOWED);
        ERRT("OUTIB @R14,@RR0,R1", REGISTER_NOT_ALLOWED);
    } else {
        TEST("OUTI  @R12,@R13,R14", 0x3BD2, 0x0EC8);
        TEST("OUTIB @R12,@R13,R14", 0x3AD2, 0x0EC8);
        ERRT("OUTI  @R0,@R13,R14",  REGISTER_NOT_ALLOWED);
        ERRT("OUTI  @R12,@R0,R14",  REGISTER_NOT_ALLOWED);
        ERRT("OUTIB @R0,@R13,R14",  REGISTER_NOT_ALLOWED);
        ERRT("OUTIB @R12,@R0,R14",  REGISTER_NOT_ALLOWED);
    }

    // Special Output and Increment
    if (z8001()) {
        TEST("SOUTI  @R10,@RR12,R11", 0x3BC3, 0x0BA8);
        TEST("SOUTIB @R10,@RR12,R11", 0x3AC3, 0x0BA8);
        ERRT("SOUTI  @R0,@RR12,R11",  REGISTER_NOT_ALLOWED);
        ERRT("SOUTI  @R10,@RR0,R11",  REGISTER_NOT_ALLOWED);
        ERRT("SOUTIB @R0,@RR12,R11",  REGISTER_NOT_ALLOWED);
        ERRT("SOUTIB @R10,@RR0,R11",  REGISTER_NOT_ALLOWED);
    } else {
        TEST("SOUTI  @R15,@R1,R0", 0x3B13, 0x00F8);
        TEST("SOUTIB @R15,@R1,R0", 0x3A13, 0x00F8);
        ERRT("SOUTI  @R0,@R1,R0",  REGISTER_NOT_ALLOWED);
        ERRT("SOUTI  @R15,@R0,R0", REGISTER_NOT_ALLOWED);
        ERRT("SOUTIB @R0,@R1,R0",  REGISTER_NOT_ALLOWED);
        ERRT("SOUTIB @R15,@R0,R0", REGISTER_NOT_ALLOWED);
    }

    // Output, Increment and Repeat
    if (z8001()) {
        TEST("OTIR  @R9,@RR10,R8", 0x3BA2, 0x0890);
        TEST("OTIRB @R9,@RR10,R8", 0x3AA2, 0x0890);
        ERRT("OTIR  @R0,@RR10,R8", REGISTER_NOT_ALLOWED);
        ERRT("OTIR  @R9,@RR0,R8",  REGISTER_NOT_ALLOWED);
        ERRT("OTIRB @R0,@RR10,R8", REGISTER_NOT_ALLOWED);
        ERRT("OTIRB @R9,@RR0,R8",  REGISTER_NOT_ALLOWED);
    } else {
        TEST("OTIR  @R3,@R2,R1", 0x3B22, 0x0130);
        TEST("OTIRB @R3,@R2,R1", 0x3A22, 0x0130);
        ERRT("OTIR  @R0,@R2,R1", REGISTER_NOT_ALLOWED);
        ERRT("OTIR  @R3,@R0,R1", REGISTER_NOT_ALLOWED);
        ERRT("OTIRB @R0,@R2,R1", REGISTER_NOT_ALLOWED);
        ERRT("OTIRB @R3,@R0,R1", REGISTER_NOT_ALLOWED);
    }

    // Special Output, Increment, and Repeat
    if (z8001()) {
        TEST("SOTIR  @R7,@RR8,R6", 0x3B83, 0x0670);
        TEST("SOTIRB @R7,@RR8,R6", 0x3A83, 0x0670);
        ERRT("SOTIR  @R0,@RR8,R6", REGISTER_NOT_ALLOWED);
        ERRT("SOTIR  @R7,@RR0,R6", REGISTER_NOT_ALLOWED);
        ERRT("SOTIRB @R0,@RR8,R6", REGISTER_NOT_ALLOWED);
        ERRT("SOTIRB @R7,@RR0,R6", REGISTER_NOT_ALLOWED);
    } else {
        TEST("SOTIR  @R4,@R3,R2", 0x3B33, 0x0240);
        TEST("SOTIRB @R4,@R3,R2", 0x3A33, 0x0240);
        ERRT("SOTIR  @R0,@R3,R2", REGISTER_NOT_ALLOWED);
        ERRT("SOTIR  @R4,@R0,R2", REGISTER_NOT_ALLOWED);
        ERRT("SOTIRB @R0,@R3,R2", REGISTER_NOT_ALLOWED);
        ERRT("SOTIRB @R4,@R0,R2", REGISTER_NOT_ALLOWED);
    }
}

static void test_cpu_conrtol() {
    // Complement Flag
    ERRT("COMFLG",         OPCODE_HAS_NO_EFFECT);
    TEST("COMFLG P",       0x8D15);
    TEST("COMFLG V",       0x8D15);
    TEST("COMFLG S",       0x8D25);
    TEST("COMFLG S,P",     0x8D35);
    TEST("COMFLG Z",       0x8D45);
    TEST("COMFLG Z,P",     0x8D55);
    TEST("COMFLG Z,S",     0x8D65);
    TEST("COMFLG Z,S,P",   0x8D75);
    TEST("COMFLG C",       0x8D85);
    TEST("COMFLG C,P",     0x8D95);
    TEST("COMFLG C,S",     0x8DA5);
    TEST("COMFLG C,S,P",   0x8DB5);
    TEST("COMFLG C,Z",     0x8DC5);
    TEST("COMFLG C,Z,P",   0x8DD5);
    TEST("COMFLG C,Z,S",   0x8DE5);
    TEST("COMFLG C,Z,S,P", 0x8DF5);
    TEST("COMFLG C,Z,S,V", 0x8DF5);

    // Assembler Interrupt
    TEST("DI VI,NVI", 0x7C00);
    TEST("DI VI",     0x7C01);
    TEST("DI NVI",    0x7C02);
    ERRT("DI",        OPCODE_HAS_NO_EFFECT);

    // Enable Interrupt
    TEST("EI VI,NVI", 0x7C04);
    TEST("EI VI",     0x7C05);
    TEST("EI NVI",    0x7C06);
    ERRT("EI",        OPCODE_HAS_NO_EFFECT);

    // Halt
    TEST("HALT", 0x7A00);

    // Load Control Register
    ERRT("LDCTL R14,FLAGS",   ILLEGAL_SIZE);
    TEST("LDCTL R13,FCW",     0x7DD2);
    TEST("LDCTL R12,REFRESH", 0x7DC3);
    if (z8001()) {
        TEST("LDCTL R11,PSAPSEG", 0x7DB4);
        TEST("LDCTL R10,PSAPOFF", 0x7DA5);
        TEST("LDCTL R9,NSPSEG",   0x7D96);
        TEST("LDCTL R8,NSPOFF",   0x7D87);
        ERRT("LDCTL R10,PSAP",    ILLEGAL_REGISTER);
        ERRT("LDCTL R11,NSP",     ILLEGAL_REGISTER);
    } else {
        TEST("LDCTL R10,PSAP",   0x7DA5);
        TEST("LDCTL R9,NSP",     0x7D97);
        ERRT("LDCTL R8,PSAPSEG", ILLEGAL_REGISTER);
        ERRT("LDCTL R7,PSAPOFF", ILLEGAL_REGISTER);
        ERRT("LDCTL R6,NSPSEG",  ILLEGAL_REGISTER);
        ERRT("LDCTL R5,NSPOFF",  ILLEGAL_REGISTER);
    }
    ERRT("LDCTL FLAGS,R6",   ILLEGAL_SIZE);
    TEST("LDCTL FCW,R5",     0x7D5A);
    TEST("LDCTL REFRESH,R4", 0x7D4B);
    if (z8001()) {
        TEST("LDCTL PSAPSEG,R3", 0x7D3C);
        TEST("LDCTL PSAPOFF,R2", 0x7D2D);
        TEST("LDCTL NSPSEG,R1",  0x7D1E);
        TEST("LDCTL NSPOFF,R0",  0x7D0F);
        ERRT("LDCTL PSAP,R15",   ILLEGAL_REGISTER);
        ERRT("LDCTL NSP,R14",    ILLEGAL_REGISTER);
    } else {
        TEST("LDCTL PSAP,R2",     0x7D2D);
        TEST("LDCTL NSP,R1",      0x7D1F);
        ERRT("LDCTL PSAPSEG,R15", ILLEGAL_REGISTER);
        ERRT("LDCTL PSAPOFF,R14", ILLEGAL_REGISTER);
        ERRT("LDCTL NSPSEG,R13",  ILLEGAL_REGISTER);
        ERRT("LDCTL NSPOFF,R12",  ILLEGAL_REGISTER);
    }
    ERRT("LDCTLB RL0,FCW",     ILLEGAL_SIZE);
    ERRT("LDCTLB RL1,REFRESH", ILLEGAL_SIZE);
    if (z8001()) {
        ERRT("LDCTLB RL2,PSAPSEG", ILLEGAL_SIZE);
        ERRT("LDCTLB RL3,PSAPOFF", ILLEGAL_SIZE);
        ERRT("LDCTLB RL4,NSPSEG",  ILLEGAL_SIZE);
        ERRT("LDCTLB RL5,NSPOFF",  ILLEGAL_SIZE);
    } else {
        ERRT("LDCTLB RL3,PSAP", ILLEGAL_SIZE);
        ERRT("LDCTLB RL5,NSP",  ILLEGAL_SIZE);
    }
    TEST("LDCTLB RL6,FLAGS",   0x8CE1);
    ERRT("LDCTLB FCW,RH0",     ILLEGAL_SIZE);
    ERRT("LDCTLB REFRESH,RH1", ILLEGAL_SIZE);
    if (z8001()) {
        ERRT("LDCTLB PSAPSEG,RH2", ILLEGAL_SIZE);
        ERRT("LDCTLB PSAPOFF,RH3", ILLEGAL_SIZE);
        ERRT("LDCTLB NSPSEG,RH4",  ILLEGAL_SIZE);
        ERRT("LDCTLB NSPOFF,RH5",  ILLEGAL_SIZE);
    } else {
        ERRT("LDCTLB PSAP,RH3", ILLEGAL_SIZE);
        ERRT("LDCTLB NSP,RH5",  ILLEGAL_SIZE);
    }
    TEST("LDCTLB FLAGS,RH6", 0x8C69);

    // Load Program Status
    if (z8001()) {
        TEST("LDPS @RR2",         0x3920);
        TEST("LDPS 0x120034",     0x7900, 0x1234);
        TEST("LDPS 0x561234",     0x7900, 0xD600, 0x1234);
        TEST("LDPS 0x120034(R1)", 0x7910, 0x1234);
        TEST("LDPS 0x561234(R1)", 0x7910, 0xD600, 0x1234);
    } else {
        TEST("LDPS @R2",        0x3920);
        TEST("LDPS 0x1234",     0x7900, 0x1234);
        TEST("LDPS 0x1234(R1)", 0x7910, 0x1234);
    }

    // Multi-Micro Bit Test, Request, Reset, and Set
    TEST("MBIT", 0x7B0A);
    TEST("MRES", 0x7B09);
    TEST("MSET", 0x7B08);

    // No Operation
    TEST("NOP", 0x8D07);

    // Reset Flag
    ERRT("RESFLG",         OPCODE_HAS_NO_EFFECT);
    TEST("RESFLG P",       0x8D13);
    TEST("RESFLG V",       0x8D13);
    TEST("RESFLG S",       0x8D23);
    TEST("RESFLG S,P",     0x8D33);
    TEST("RESFLG Z",       0x8D43);
    TEST("RESFLG Z,P",     0x8D53);
    TEST("RESFLG Z,S",     0x8D63);
    TEST("RESFLG Z,S,P",   0x8D73);
    TEST("RESFLG C",       0x8D83);
    TEST("RESFLG C,P",     0x8D93);
    TEST("RESFLG C,S",     0x8DA3);
    TEST("RESFLG C,S,P",   0x8DB3);
    TEST("RESFLG C,Z",     0x8DC3);
    TEST("RESFLG C,Z,P",   0x8DD3);
    TEST("RESFLG C,Z,S",   0x8DE3);
    TEST("RESFLG C,Z,S,P", 0x8DF3);
    TEST("RESFLG C,Z,S,V", 0x8DF3);

    // Set Flag
    ERRT("SETFLG",         OPCODE_HAS_NO_EFFECT);
    TEST("SETFLG P",       0x8D11);
    TEST("SETFLG V",       0x8D11);
    TEST("SETFLG S",       0x8D21);
    TEST("SETFLG S,P",     0x8D31);
    TEST("SETFLG Z",       0x8D41);
    TEST("SETFLG Z,P",     0x8D51);
    TEST("SETFLG Z,S",     0x8D61);
    TEST("SETFLG Z,S,P",   0x8D71);
    TEST("SETFLG C",       0x8D81);
    TEST("SETFLG C,P",     0x8D91);
    TEST("SETFLG C,S",     0x8DA1);
    TEST("SETFLG C,S,P",   0x8DB1);
    TEST("SETFLG C,Z",     0x8DC1);
    TEST("SETFLG C,Z,P",   0x8DD1);
    TEST("SETFLG C,Z,S",   0x8DE1);
    TEST("SETFLG C,Z,S,P", 0x8DF1);
    TEST("SETFLG C,Z,S,V", 0x8DF1);
}

static void test_short_direct() {
    symtab.intern(0x0034, "soff");
    symtab.intern(0x1234, "loff");
    symtab.intern(0x560000, "seg");

    TEST("CLR 0x560034       ; auto short",  0x4D08, 0x5634);
    TEST("CLR 0x561234       ; long offset", 0x4D08, 0xD600, 0x1234);
    TEST("CLR 0x560034(R2)   ; auto short",  0x4D28, 0x5634);
    TEST("CLR 0x561234(R2)   ; long offset", 0x4D28, 0xD600, 0x1234);
    TEST("CLR seg|soff       ; auto short",  0x4D08, 0x5634);
    TEST("CLR seg+soff(R2)   ; auto short",  0x4D28, 0x5634);
    TEST("CLR (seg+soff)     ; auto short",  0x4D08, 0x5634);
    TEST("CLR (seg|soff)(R2) ; auto short",  0x4D28, 0x5634);
    TEST("CLR seg+loff       ; long offset", 0x4D08, 0xD600, 0x1234);
    TEST("CLR seg|loff(R2)   ; long offset", 0x4D28, 0xD600, 0x1234);
    TEST("CLR (seg|loff)     ; long offset", 0x4D08, 0xD600, 0x1234);
    TEST("CLR (seg+loff)(R2) ; long offset", 0x4D28, 0xD600, 0x1234);

    asm8000.setAutoShortDirect(false);

    TEST("CLR 0x560034",       0x4D08, 0xD600, 0x0034);
    TEST("CLR 0x561234",       0x4D08, 0xD600, 0x1234);
    TEST("CLR 0x560034(R2)",   0x4D28, 0xD600, 0x0034);
    TEST("CLR 0x561234(R2)",   0x4D28, 0xD600, 0x1234);
    TEST("CLR seg+soff",       0x4D08, 0xD600, 0x0034);
    TEST("CLR seg|soff(R2)",   0x4D28, 0xD600, 0x0034);
    TEST("CLR seg|loff",       0x4D08, 0xD600, 0x1234);
    TEST("CLR seg+loff(R2)",   0x4D28, 0xD600, 0x1234);
    TEST("CLR (seg|soff)",     0x4D08, 0xD600, 0x0034);
    TEST("CLR (seg+loff)",     0x4D08, 0xD600, 0x1234);
    TEST("CLR (seg+soff)(R2)", 0x4D28, 0xD600, 0x0034);
    TEST("CLR (seg|loff)(R2)", 0x4D28, 0xD600, 0x1234);

    TEST("CLR |0x560034|       ; short direct", 0x4D08, 0x5634);
    TEST("CLR |0x560034|(R2)   ; short direct", 0x4D28, 0x5634);
    TEST("CLR |seg+soff|(R2)   ; short direct", 0x4D28, 0x5634);
    TEST("CLR |(seg|soff)|     ; short direct", 0x4D08, 0x5634);
    TEST("CLR |(seg+soff)|(R2) ; short direct", 0x4D28, 0x5634);
    ERRT("CLR |0x561234|       ; long offset",  OVERFLOW_RANGE);
    ERRT("CLR |0x561234|(R2)   ; long offset",  OVERFLOW_RANGE);
    ERRT("CLR |seg+loff|       ; long offset",  OVERFLOW_RANGE);
    ERRT("CLR |(seg+loff)|     ; long offset",  OVERFLOW_RANGE);
    ERRT("CLR |(seg|loff)|(R2) ; long offset",  OVERFLOW_RANGE);
}

static void test_comment() {
    TEST("ADD R1 , # 0x1234", 0x0101, 0x1234);
    if (z8001()) {
        TEST("EXB RL1 , 0x1234 ( R2 ) ; comment", 0x6C29 ,0x8000, 0x1234);
        TEST("PUSH @RR4 , R2",  0x9342);
        ERRT("PUSH @ RR4 , R2", UNKNOWN_REGISTER);
    } else {
        TEST("EXB RL1 , 0x1234 ( R2 ) ; comment", 0x6C29 ,0x1234);
        TEST("PUSH @R4 , R2",  0x9342);
        ERRT("PUSH @ R4 , R2", UNKNOWN_REGISTER);
    }
    TEST("SETFLG C , Z , S , P , V", 0x8DF1);
}

static void test_undefined_symbol() {
    ERUS("LDB RH2,#UNDEF",   0xC200);
    ERUS("LD  R2,#UNDEF",    0x2102, 0x0000);
    ERUS("LDL RR2,#UNDEF",   0x1402, 0x0000, 0x0000);
    if (z8001()) {
        ERUS("LD  R9,UNDEF",           0x6109, 0x8000, 0x0000);
        ERUS("LD  R9,UNDEF(R2)",       0x6129, 0x8000, 0x0000);
        ERUS("LD  UNDEF,R9",           0x6F09, 0x8000, 0x0000);
        ERUS("LD  UNDEF(R2),R9",       0x6F29, 0x8000, 0x0000);
        ERUS("LD  R9,|UNDEF|",         0x6109, 0x0000);
        ERUS("LD  R9,|UNDEF|(R2)",     0x6129, 0x0000);
        ERUS("LD  |UNDEF|,R9",         0x6F09, 0x0000);
        ERUS("LD  |UNDEF|(R2),R9",     0x6F29, 0x0000);
        ERUS("LD  R9,RR2(#UNDEF)",     0x3129, 0x0000);
        ERUS("LD  RR2(#UNDEF),R9",     0x3329, 0x0000);
        ERUS("LDA RR8,UNDEF",          0x7608, 0x8000, 0x0000);
        ERUS("LDA RR8,UNDEF(R2)",      0x7628, 0x8000, 0x0000);
        ERUS("LDA RR8,|UNDEF|",        0x7608, 0x0000);
        ERUS("LDA RR8,|UNDEF|(R2)",    0x7628, 0x0000);
        ERUS("LDA RR8,RR2(#UNDEF)",    0x3428, 0x0000);
        ERUS("LDAR RR8,UNDEF",         0x3408, 0x0000);
        ERUS("LDAR RR8,|UNDEF|",       0x3408, 0x0000);
        ERUS("LDM R1,@RR2,#UNDEF",     0x1C21, 0x0100);
        ERUS("LDM R1,UNDEF,#2",        0x5C01, 0x0101, 0x8000, 0x0000);
        ERUS("LDM R1,UNDEF,#UNDEF",    0x5C01, 0x0100, 0x8000, 0x0000);
        ERUS("LDM R1,|UNDEF|,#2",      0x5C01, 0x0101, 0x0000);
        ERUS("LDM R1,|UNDEF|,#UNDEF",  0x5C01, 0x0100, 0x0000);
        ERUS("CALL UNDEF",             0x5F00, 0x8000, 0x0000);
        ERUS("CALL UNDEF(R2)",         0x5F20, 0x8000, 0x0000);
        ERUS("CALL |UNDEF|",           0x5F00, 0x0000);
        ERUS("CALL |UNDEF|(R2)",       0x5F20, 0x0000);
        ERUS("JP   UNDEF",             0x5E08, 0x8000, 0x0000);
        ERUS("JP C,UNDEF",             0x5E07, 0x8000, 0x0000);
        ERUS("JP C,UNDEF(R2)",         0x5E27, 0x8000, 0x0000);
        ERUS("JP   |UNDEF|",           0x5E08, 0x0000);
        ERUS("JP C,|UNDEF|",           0x5E07, 0x0000);
        ERUS("JP C,|UNDEF|(R2)",       0x5E27, 0x0000);
        ERUS("BIT UNDEF,#4",           0x6704, 0x8000, 0x0000);
        ERUS("BIT UNDEF(R2),#4",       0x6724, 0x8000, 0x0000);
        ERUS("BIT UNDEF,#UNDEF",       0x6700, 0x8000, 0x0000);
        ERUS("BIT UNDEF(R2),#UNDEF",   0x6720, 0x8000, 0x0000);
        ERUS("BIT |UNDEF|,#4",         0x6704, 0x0000);
        ERUS("BIT |UNDEF|(R2),#4",     0x6724, 0x0000);
        ERUS("BIT |UNDEF|,#UNDEF",     0x6700, 0x0000);
        ERUS("BIT |UNDEF|(R2),#UNDEF", 0x6720, 0x0000);
    } else {
        ERUS("LD  R9,UNDEF",         0x6109, 0x0000);
        ERUS("LD  R9,UNDEF(R2)",     0x6129, 0x0000);
        ERUS("LD  UNDEF,R9",         0x6F09, 0x0000);
        ERUS("LD  UNDEF(R2),R9",     0x6F29, 0x0000);
        ERUS("LD  R9,R2(#UNDEF)",    0x3129, 0x0000);
        ERUS("LD  R2(#UNDEF),R9",    0x3329, 0x0000);
        ERUS("LDA R8,UNDEF",         0x7608, 0x0000);
        ERUS("LDA R8,UNDEF(R2)",     0x7628, 0x0000);
        ERUS("LDA R8,R2(#UNDEF)",    0x3428, 0x0000);
        ERUS("LDAR R8,UNDEF",        0x3408, 0x0000);
        ERUS("LDM R1,@R2,#UNDEF",    0x1C21, 0x0100);
        ERUS("LDM R1,UNDEF,#2",      0x5C01, 0x0101, 0x0000);
        ERUS("LDM R1,UNDEF,#UNDEF",  0x5C01, 0x0100, 0x0000);
        ERUS("CALL UNDEF",           0x5F00, 0x0000);
        ERUS("CALL UNDEF(R2)",       0x5F20, 0x0000);
        ERUS("JP   UNDEF",           0x5E08, 0x0000);
        ERUS("JP C,UNDEF",           0x5E07, 0x0000);
        ERUS("JP C,UNDEF(R2)",       0x5E27, 0x0000);
        ERUS("BIT UNDEF,#4",         0x6704, 0x0000);
        ERUS("BIT UNDEF(R2),#4",     0x6724, 0x0000);
        ERUS("BIT UNDEF,#UNDEF",     0x6700, 0x0000);
        ERUS("BIT UNDEF(R2),#UNDEF", 0x6720, 0x0000);
    }

    ERUS("LDR R1,UNDEF",    0x3101, 0x0000);
    ERUS("LDR UNDEF,R1",    0x3301, 0x0000);
    ERUS("CALR UNDEF",      0xD000);
    ERUS("DJNZ  R2,UNDEF",  0xF280);
    ERUS("DBJNZ RH2,UNDEF", 0xF200);
    ERUS("JR   UNDEF",      0xE800);
    ERUS("JR C,UNDEF",      0xE700);

    ERUS("LDK R1,#UNDEF", 0xBD10);
    ERUS("DEC R1,#UNDEF", 0xAB10);
    ERUS("SC  #UNDEF",    0x7F00);
    ERUS("BIT R2,#UNDEF", 0xA720);
    ERUS("RL  R1,#UNDEF", 0xB310);
    ERUS("SLA R2,#UNDEF", 0xB329, 0x0000);
    ERUS("SRA R2,#UNDEF", 0xB329, 0x0000);
    ERUS("IN  R1,#UNDEF", 0x3B14, 0x0000);
    ERUS("OUT #UNDEF,R1", 0x3B16, 0x0000);
}
// clang-format on

void run_tests(const char *cpu) {
    assembler.setCpu(cpu);
    RUN_TEST(test_load_and_exchange);
    RUN_TEST(test_arithmetic);
    RUN_TEST(test_logical);
    RUN_TEST(test_program_control);
    RUN_TEST(test_bit_manipulation);
    RUN_TEST(test_rotate);
    RUN_TEST(test_shift);
    RUN_TEST(test_block_transfer);
    RUN_TEST(test_string_manipulation);
    RUN_TEST(test_input);
    RUN_TEST(test_output);
    RUN_TEST(test_cpu_conrtol);
    if (z8001()) {
        RUN_TEST(test_short_direct);
    }
    RUN_TEST(test_comment);
    RUN_TEST(test_undefined_symbol);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

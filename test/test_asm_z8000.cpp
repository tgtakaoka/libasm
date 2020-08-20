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
    return strcmp(assembler.getCpu(), "Z8001") == 0;
}

static void set_up() {
    assembler.reset();
}

static void tear_down() {
    symtab.reset();
}

static void test_cpu() {
    EQUALS("cpu 8001", true,    assembler.setCpu("Z8001"));
    EQUALS("cpu 8001", "Z8001", assembler.getCpu());

    EQUALS("cpu Z8002", true,    assembler.setCpu("Z8002"));
    EQUALS("cpu Z8002", "Z8002", assembler.getCpu());
}

static void test_load_and_exchange() {
    // Clear
    TEST("CLR  R2",  0x8D28);
    TEST("CLRB RH2", 0x8C28);
    if (z8001()) {
        TEST("CLR  @RR2",         0x0D28);
        TEST("CLR  0x120034",     0x4D08, 0x1234);
        TEST("CLR  0x561234",     0x4D08, 0xD600, 0x1234);
        TEST("CLR  0x120034(R2)", 0x4D28, 0x1234);
        TEST("CLR  0x561234(R2)", 0x4D28, 0xD600, 0x1234);
        TEST("CLRB @RR2",         0x0C28);
        TEST("CLRB 0x120034",     0x4C08, 0x1234);
        TEST("CLRB 0x561234",     0x4C08, 0xD600, 0x1234);
        TEST("CLRB 0x120034(R2)", 0x4C28, 0x1234);
        TEST("CLRB 0x561234(R2)", 0x4C28, 0xD600, 0x1234);
    } else {
        TEST("CLR  @R2",        0x0D28);
        TEST("CLR  0x1234",     0x4D08, 0x1234);
        TEST("CLR  0x1234(R2)", 0x4D28, 0x1234);
        TEST("CLRB @R2",        0x0C28);
        TEST("CLRB 0x1234",     0x4C08, 0x1234);
        TEST("CLRB 0x1234(R2)", 0x4C28, 0x1234);
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
        ATEST(0x2000, "LDAR RR8,0x002000", 0x3408, 0xFFFC);
        ATEST(0x2000, "LDAR RR8,0x002004", 0x3408, 0x0000);
        ATEST(0x102000, "LDAR RR8,0x0FA004", 0x3408, 0x8000);
        ATEST(0x2000, "LDAR RR8,0x00A003", 0x3408, 0x7FFF);
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
        TEST("LDM R1,@RR2,#2",         0x1C21, 0x0101);
        TEST("LDM R15,0x120034,#3",    0x5C01, 0x0F02, 0x1234);
        TEST("LDM R15,0x561234,#4",    0x5C01, 0x0F03, 0xD600, 0x1234);
        TEST("LDM R1,0x120034(R2),#5", 0x5C21, 0x0104, 0x1234);
        TEST("LDM R1,0x561234(R2),#6", 0x5C21, 0x0105, 0xD600, 0x1234);
    } else {
        TEST("LDM R15,@R2,#7",        0x1C21, 0x0F06);
        TEST("LDM R1,@R2,#8",         0x1C21, 0x0107);
        TEST("LDM R15,0x1234,#9",     0x5C01, 0x0F08, 0x1234);
        TEST("LDM R1,0x1234(R2),#15", 0x5C21, 0x010E, 0x1234);
    }

    // Load Relative
    if (z8001()) {
        ATEST(0x2000, "LDR  R1,0x002000",  0x3101, 0xFFFC);
        ATEST(0x2000, "LDR  0x002000,R1",  0x3301, 0xFFFC);
        ATEST(0x2000, "LDRB RH1,0x002000", 0x3001, 0xFFFC);
        ATEST(0x2000, "LDRB 0x002000,RH1", 0x3201, 0xFFFC);
        ATEST(0x2000, "LDRL RR2,0x002000", 0x3502, 0xFFFC);
        ATEST(0x2000, "LDRL 0x002000,RR2", 0x3702, 0xFFFC);
    } else {
        ATEST(0x2000, "LDR  R1,0x2000",  0x3101, 0xFFFC);
        ATEST(0x2000, "LDR  0x2000,R1",  0x3301, 0xFFFC);
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
    ETEST(ILLEGAL_REGISTER, "    EXTS  RR7");
    ETEST(REGISTER_NOT_ALLOWED, "EXTS  R15");
    ETEST(ILLEGAL_REGISTER,     "EXTSL RQ6");
    ETEST(REGISTER_NOT_ALLOWED, "EXTSL RR14");

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
    TEST("CALR .+2",      0xD000);
    TEST("CALR .+10",     0xD004);
    TEST("CALR .+0x1000", 0xD7FF);
    TEST("CALR .-0x0FFE", 0xD800);
    TEST("CALR .-10",     0xDFFA);
    TEST("CALR .",        0xDFFF);

    // Decrement and Jump if Not Zero
    TEST("DJNZ  R2,.",  0xF281);
    TEST("DBJNZ RL0,.", 0xF801);

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
    TEST("JR F,.",   0xE0FF);
    TEST("JR LT,.",  0xE1FF);
    TEST("JR LE,.",  0xE2FF);
    TEST("JR ULE,.", 0xE3FF);
    TEST("JR OV,.",  0xE4FF);
    TEST("JR MI,.",  0xE5FF);
    TEST("JR Z,.",   0xE6FF);
    TEST("JR C,.",   0xE7FF);
    TEST("JR .",     0xE8FF);
    TEST("JR GE,.",  0xE9FF);
    TEST("JR GT,.",  0xEAFF);
    TEST("JR UGT,.", 0xEBFF);
    TEST("JR NOV,.", 0xECFF);
    TEST("JR PL,.",  0xEDFF);
    TEST("JR NZ,.",  0xEEFF);
    TEST("JR NC,.",  0xEFFF);
    TEST("JR EQ,.",  0xE6FF);
    TEST("JR ULT,.", 0xE7FF);
    TEST("JR NE,.",  0xEEFF);
    TEST("JR UGE,.", 0xEFFF);

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
    TEST("BIT  R2,#1",       0xA721);
    TEST("BIT  R2,R8",       0x2708, 0x0200);
    TEST("BITB RL2,#1",      0xA6A1);
    TEST("BITB RL2,R1",      0x2601, 0x0A00);
    ETEST(ILLEGAL_BIT_NUMBER,   "BITB RL2,#8");
    ETEST(REGISTER_NOT_ALLOWED, "BITB RL2,R8");
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
    TEST("RES  R2,#1",       0xA321);
    TEST("RES  R2,R8",       0x2308, 0x0200);
    TEST("RESB RL2,#1",      0xA2A1);
    TEST("RESB RL2,R1",      0x2201, 0x0A00);
    ETEST(ILLEGAL_BIT_NUMBER,   "RESB RL2,#8");
    ETEST(REGISTER_NOT_ALLOWED, "RESB RL2,R8");
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
    TEST("SET  R2,#1",       0xA521);
    TEST("SET  R2,R8",       0x2508, 0x0200);
    TEST("SETB RL2,#1",      0xA4A1);
    TEST("SETB RL2,R1",      0x2401, 0x0A00);
    ETEST(ILLEGAL_BIT_NUMBER,   "SETB RL2,#8");
    ETEST(REGISTER_NOT_ALLOWED, "SETB RL2,R8");
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
    TEST("RL  R1,#1",   0xB310);
    TEST("RL  R8,#2",   0xB382);
    TEST("RLB RH1,#1",  0xB210);
    TEST("RLB RL0,#2",  0xB282);

    // Rotate Left through Carry
    TEST("RLC  R1,#1",   0xB318);
    TEST("RLC  R8,#2",   0xB38A);
    TEST("RLCB RH1,#1",  0xB218);
    TEST("RLCB RL0,#2",  0xB28A);

    // Rotate Left Digit
    TEST("RLDB RH1,RL0", 0xBE81);

    // Rotate Right
    TEST("RR  R1,#1",   0xB314);
    TEST("RR  R8,#2",   0xB386);
    TEST("RRB RH1,#1",  0xB214);
    TEST("RRB RL0,#2",  0xB286);

    // Rotate Right through Carry
    TEST("RRC  R1,#1",   0xB31C);
    TEST("RRC  R8,#2",   0xB38E);
    TEST("RRCB RH1,#1",  0xB21C);
    TEST("RRCB RL0,#2",  0xB28E);

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
    TEST("SLA  R2,#16",  0xB329, 0x0010);
    TEST("SLAB RH2,#8",  0xB229, 0x0008);
    TEST("SLAL RR2,#32", 0xB32D, 0x0020);
    ETEST(OVERFLOW_RANGE, "SLA  R2,#17");
    ETEST(OVERFLOW_RANGE, "SLAB R2,#9");
    ETEST(OVERFLOW_RANGE, "SLAL R2,#33");

    // Shift Right Arithmetic
    TEST("SRA  R2,#16",  0xB329, 0xFFF0);
    TEST("SRAB RH2,#8",  0xB229, 0x00F8);
    TEST("SRAL RR2,#32", 0xB32D, 0xFFE0);
    ETEST(OVERFLOW_RANGE, "SRA  R2,#17");
    ETEST(OVERFLOW_RANGE, "SRAB R2,#9");
    ETEST(OVERFLOW_RANGE, "SRAL R2,#33");

    // Shift Left Logical
    TEST("SLL  R2,#16",  0xB321, 0x0010);
    TEST("SLLB RH2,#8",  0xB221, 0x0008);
    TEST("SLLL RR2,#32", 0xB325, 0x0020);
    ETEST(OVERFLOW_RANGE, "SLL  R2,#17");
    ETEST(OVERFLOW_RANGE, "SLLB R2,#9");
    ETEST(OVERFLOW_RANGE, "SLLL R2,#33");

    // Shift Right Logical
    TEST("SRL  R2,#16",  0xB321, 0xFFF0);
    TEST("SRLB RH2,#8",  0xB221, 0x00F8);
    TEST("SRLL RR2,#32", 0xB325, 0xFFE0);
    ETEST(OVERFLOW_RANGE, "SRL  R2,#17");
    ETEST(OVERFLOW_RANGE, "SRLB R2,#9");
    ETEST(OVERFLOW_RANGE, "SRLL R2,#33");
}

static void test_block_transfer() {
    // Compare and Decrement
    if (z8001()) {
        TEST("CPD  R0,@RR2,R4,F",   0xBB28, 0x0400);
        TEST("CPDB RL0,@RR2,R4,LT", 0xBA28, 0x0481);
        ETEST(REGISTER_NOT_ALLOWED, "CPD  R2,@RR0,R4,F");
        ETEST(REGISTER_NOT_ALLOWED, "CPDB RL0,@RR0,R4,LT");
    } else {
        TEST("CPD  R0,@R1,R2,F",    0xBB18, 0x0200);
        TEST("CPDB RL0,@R1,R2,LT",  0xBA18, 0x0281);
        ETEST(REGISTER_NOT_ALLOWED, "CPD  R2,@R0,R4,F");
        ETEST(REGISTER_NOT_ALLOWED, "CPDB RL0,@R0,R4,LT");
    }

    // Compare, Decrement, and Repeat
    if (z8001()) {
        TEST("CPDR  R1,@RR4,R3,LE",   0xBB4C, 0x0312);
        TEST("CPDRB RL1,@RR4,R3,ULE", 0xBA4C, 0x0393);
        ETEST(REGISTER_NOT_ALLOWED, "CPDR  R1,@RR0,R4,LE");
        ETEST(REGISTER_NOT_ALLOWED, "CPDRB RL1,@RR0,R4,ULE");
    } else {
        TEST("CPDR  R3,@R4,R5,LE",    0xBB4C, 0x0532);
        TEST("CPDRB RL3,@R4,R5,ULE",  0xBA4C, 0x05B3);
        ETEST(REGISTER_NOT_ALLOWED, "CPDR  R1,@R0,R4,LE");
        ETEST(REGISTER_NOT_ALLOWED, "CPDRB RL1,@R0,R4,ULE");
    }

    // Compare and Increment
    if (z8001()) {
        TEST("CPI  R2,@RR6,R4,OV", 0xBB60, 0x0424);
        TEST("CPIB RL2,@RR6,R4",   0xBA60, 0x04A8);
        ETEST(REGISTER_NOT_ALLOWED, "CPI  R1,@RR0,R4,OV");
        ETEST(REGISTER_NOT_ALLOWED, "CPIB RL1,@RR0,R4");
    } else {
        TEST("CPI  R6,@R7,R8,OV",  0xBB70, 0x0864);
        TEST("CPIB RL6,@R7,R8",    0xBA70, 0x08E8);
        ETEST(REGISTER_NOT_ALLOWED, "CPI  R2,@R0,R4,OV");
        ETEST(REGISTER_NOT_ALLOWED, "CPIB RL0,@R0,R4");
    }

    // Compare, Increment, and Repeat
    if (z8001()) {
        TEST("CPIR  R3,@RR8,R5,Z",  0xBB84, 0x0536);
        TEST("CPIRB RL3,@RR8,R5,C", 0xBA84, 0x05B7);
        ETEST(REGISTER_NOT_ALLOWED, "CPIR  R1,@RR0,R4,Z");
        ETEST(REGISTER_NOT_ALLOWED, "CPIRB RL1,@RR0,R4,C");
    } else {
        TEST("CPIR  R9,@R10,R11,Z",  0xBBA4, 0x0B96);
        TEST("CPIRB RH1,@R10,R11,C", 0xBAA4, 0x0B17);
        ETEST(REGISTER_NOT_ALLOWED, "CPIR  R1,@R0,R4,LE");
        ETEST(REGISTER_NOT_ALLOWED, "CPIRB RL1,@R0,R4,ULE");
    }

    // Load and Decrement
    if (z8001()) {
        TEST("LDD  @RR2,@RR4,R6", 0xBB49, 0x0628);
        TEST("LDDB @RR2,@RR4,R6", 0xBA49, 0x0628);
        ETEST(REGISTER_NOT_ALLOWED, "LDD  @RR0,@RR4,R6");
        ETEST(REGISTER_NOT_ALLOWED, "LDDB @RR0,@RR4,R6");
        ETEST(REGISTER_NOT_ALLOWED, "LDD  @RR2,@RR0,R6");
        ETEST(REGISTER_NOT_ALLOWED, "LDDB @RR2,@RR0,R6");
    } else {
        TEST("LDD  @R1,@R2,R3",   0xBB29, 0x0318);
        TEST("LDDB @R1,@R2,R3",   0xBA29, 0x0318);
        ETEST(REGISTER_NOT_ALLOWED, "LDD  @R0,@R2,R3");
        ETEST(REGISTER_NOT_ALLOWED, "LDDB @R0,@R2,R3");
        ETEST(REGISTER_NOT_ALLOWED, "LDD  @R1,@R0,R3");
        ETEST(REGISTER_NOT_ALLOWED, "LDDB @R1,@R0,R3");
    }

    // Load, Decrement, and Repeat
    if (z8001()) {
        TEST("LDDR  @RR4,@RR6,R8", 0xBB69, 0x0840);
        TEST("LDDRB @RR4,@RR6,R8", 0xBA69, 0x0840);
        ETEST(REGISTER_NOT_ALLOWED, "LDDR  @RR0,@RR6,R8");
        ETEST(REGISTER_NOT_ALLOWED, "LDDRB @RR0,@RR6,R8");
        ETEST(REGISTER_NOT_ALLOWED, "LDDR  @RR4,@RR0,R8");
        ETEST(REGISTER_NOT_ALLOWED, "LDDRB @RR4,@RR0,R8");
    } else {
        TEST("LDDR  @R4,@R5,R6",   0xBB59, 0x0640);
        TEST("LDDRB @R4,@R5,R6",   0xBA59, 0x0640);
        ETEST(REGISTER_NOT_ALLOWED, "LDDR  @R0,@R5,R6");
        ETEST(REGISTER_NOT_ALLOWED, "LDDRB @R0,@R5,R6");
        ETEST(REGISTER_NOT_ALLOWED, "LDDR  @R4,@R0,R6");
        ETEST(REGISTER_NOT_ALLOWED, "LDDRB @R4,@R0,R6");
    }

    // Load and Increment
    if (z8001()) {
        TEST("LDI  @RR6,@RR8,R10", 0xBB81, 0x0A68);
        TEST("LDIB @RR6,@RR8,R10", 0xBA81, 0x0A68);
        ETEST(REGISTER_NOT_ALLOWED, "LDI  @RR0,@RR8,R10");
        ETEST(REGISTER_NOT_ALLOWED, "LDIB @RR0,@RR8,R10");
        ETEST(REGISTER_NOT_ALLOWED, "LDI  @RR6,@RR0,R10");
        ETEST(REGISTER_NOT_ALLOWED, "LDIB @RR6,@RR0,R10");
    } else {
        TEST("LDI  @R7,@R8,R9",    0xBB81, 0x0978);
        TEST("LDIB @R7,@R8,R9",    0xBA81, 0x0978);
        ETEST(REGISTER_NOT_ALLOWED, "LDI  @R0,@R8,R9");
        ETEST(REGISTER_NOT_ALLOWED, "LDIB @R0,@R8,R9");
        ETEST(REGISTER_NOT_ALLOWED, "LDI  @R7,@R0,R9");
        ETEST(REGISTER_NOT_ALLOWED, "LDIB @R7,@R0,R9");
    }

    // Load, Increment, and Repeat
    if (z8001()) {
        TEST("LDIR  @RR8,@RR10,R12", 0xBBA1, 0x0C80);
        TEST("LDIRB @RR8,@RR10,R12", 0xBAA1, 0x0C80);
        ETEST(REGISTER_NOT_ALLOWED, "LDIR  @RR0,@RR10,R12");
        ETEST(REGISTER_NOT_ALLOWED, "LDIRB @RR0,@RR10,R12");
        ETEST(REGISTER_NOT_ALLOWED, "LDIR  @RR8,@RR0,R12");
        ETEST(REGISTER_NOT_ALLOWED, "LDIRB @RR8,@RR0,R12");
    } else {
        TEST("LDIR  @R10,@R11,R12",  0xBBB1, 0x0CA0);
        TEST("LDIRB @R10,@R11,R12",  0xBAB1, 0x0CA0);
        ETEST(REGISTER_NOT_ALLOWED, "LDIR  @R0,@R11,R12");
        ETEST(REGISTER_NOT_ALLOWED, "LDIRB @R0,@R11,R12");
        ETEST(REGISTER_NOT_ALLOWED, "LDIR  @R10,@R0,R12");
        ETEST(REGISTER_NOT_ALLOWED, "LDIRB @R10,@R0,R12");
    }
}

static void test_string_manipulation() {
    // Compare String and Decrement
    if (z8001()) {
        TEST("CPSD  @RR4,@RR2,R0",    0xBB2A, 0x0048);
        TEST("CPSDB @RR4,@RR2,R4,GE", 0xBA2A, 0x0449);
        ETEST(REGISTER_NOT_ALLOWED, "CPSD  @RR0,@RR2,R0");
        ETEST(REGISTER_NOT_ALLOWED, "CPSD  @RR4,@RR0,R0");
        ETEST(REGISTER_NOT_ALLOWED, "CPSDB @RR0,@RR2,R4,GE");
        ETEST(REGISTER_NOT_ALLOWED, "CPSDB @RR2,@RR0,R4,GE");
    } else {
        TEST("CPSD  @R3,@R1,R2",     0xBB1A, 0x0238);
        TEST("CPSDB @R3,@R1,R2,GE",  0xBA1A, 0x0239);
        ETEST(REGISTER_NOT_ALLOWED, "CPSD  @R3,@R0,R2");
        ETEST(REGISTER_NOT_ALLOWED, "CPSD  @R0,@R3,R2");
        ETEST(REGISTER_NOT_ALLOWED, "CPSDB @R3,@R0,R2,GE");
        ETEST(REGISTER_NOT_ALLOWED, "CPSDB @R0,@R3,R2,GE");
    }

    // Compare String, Decrement, and Repeat
    if (z8001()) {
        TEST("CPSDR  @RR2,@RR4,R5,GT",  0xBB4E, 0x052A);
        TEST("CPSDRB @RR2,@RR4,R5,UGT", 0xBA4E, 0x052B);
        ETEST(REGISTER_NOT_ALLOWED, "CPSDR  @RR0,@RR4,R5,GT");
        ETEST(REGISTER_NOT_ALLOWED, "CPSDR  @RR2,@RR0,R5,GT");
        ETEST(REGISTER_NOT_ALLOWED, "CPSDRB @RR0,@RR4,R5,UGT");
        ETEST(REGISTER_NOT_ALLOWED, "CPSDRB @RR2,@RR0,R5,UGT");
    } else {
        TEST("CPSDR  @R3,@R4,R5,GT",  0xBB4E, 0x053A);
        TEST("CPSDRB @R3,@R4,R0,UGT", 0xBA4E, 0x003B);
        ETEST(REGISTER_NOT_ALLOWED, "CPSDR  @R0,@R4,R5,GT");
        ETEST(REGISTER_NOT_ALLOWED, "CPSDR  @R3,@R0,R5,GT");
        ETEST(REGISTER_NOT_ALLOWED, "CPSDRB @R0,@R4,R0,UGT");
        ETEST(REGISTER_NOT_ALLOWED, "CPSDRB @R3,@R0,R0,UGT");
    }

    // Compare String and Increment
    if (z8001()) {
        TEST("CPSI  @RR4,@RR6,R5,NOV", 0xBB62, 0x054C);
        TEST("CPSIB @RR4,@RR6,R5,PL",  0xBA62, 0x054D);
        ETEST(REGISTER_NOT_ALLOWED, "CPSI  @RR0,@RR6,R5,NOV");
        ETEST(REGISTER_NOT_ALLOWED, "CPSI  @RR4,@RR0,R5,NOV");
        ETEST(REGISTER_NOT_ALLOWED, "CPSIB @RR0,@RR6,R5,PL");
        ETEST(REGISTER_NOT_ALLOWED, "CPSIB @RR4,@RR0,R5,PL");
    } else {
        TEST("CPSI  @R6,@R7,R8,NOV", 0xBB72, 0x086C);
        TEST("CPSIB @R6,@R7,R8,PL",  0xBA72, 0x086D);
        ETEST(REGISTER_NOT_ALLOWED, "CPSI  @R0,@R7,R8,NOV");
        ETEST(REGISTER_NOT_ALLOWED, "CPSI  @R6,@R0,R8,NOV");
        ETEST(REGISTER_NOT_ALLOWED, "CPSIB @R0,@R7,R8,PL");
        ETEST(REGISTER_NOT_ALLOWED, "CPSIB @R6,@R0,R8,PL");
    }

    // Compare String, Increment, and Repeat
    if (z8001()) {
        TEST("CPSIR  @RR4,@RR8,R6,NZ", 0xBB86, 0x064E);
        TEST("CPSIRB @RR4,@RR8,R6,NC", 0xBA86, 0x064F);
        ETEST(REGISTER_NOT_ALLOWED, "CPSIR  @RR0,@RR8,R6,NZ");
        ETEST(REGISTER_NOT_ALLOWED, "CPSIR  @RR4,@RR0,R6,NZ");
        ETEST(REGISTER_NOT_ALLOWED, "CPSIRB @RR0,@RR8,R6,NC");
        ETEST(REGISTER_NOT_ALLOWED, "CPSIRB @RR4,@RR0,R6,NC");
    } else {
        TEST("CPSIR  @R9,@R10,R11,NZ", 0xBBA6, 0x0B9E);
        TEST("CPSIRB @R1,@R10,R11,NC", 0xBAA6, 0x0B1F);
        ETEST(REGISTER_NOT_ALLOWED, "CPSIR  @R0,@R10,R11,NZ");
        ETEST(REGISTER_NOT_ALLOWED, "CPSIR  @R9,@R0,R11,NZ");
        ETEST(REGISTER_NOT_ALLOWED, "CPSIRB @R0,@R10,R11,NC");
        ETEST(REGISTER_NOT_ALLOWED, "CPSIRB @R1,@R0,R11,NC");
    }

    // Translate and Decrement
    if (z8001()) {
        TEST("TRDB @RR2,@RR4,R1", 0xB828, 0x0140);
        ETEST(REGISTER_NOT_ALLOWED, "TRDB @RR0,@RR4,R1");
        ETEST(REGISTER_NOT_ALLOWED, "TRDB @RR2,@RR0,R1");
    } else {
        TEST("TRDB @R1,@R2,R0",   0xB818, 0x0020);
        ETEST(REGISTER_NOT_ALLOWED, "TRDB @R0,@R2,R0");
        ETEST(REGISTER_NOT_ALLOWED, "TRDB @R1,@R0,R0");
    }

    // Translate, Decrement, and Repeat
    if (z8001()) {
        TEST("TRDRB @RR4,@RR6,R2", 0xB84C, 0x0260);
        ETEST(REGISTER_NOT_ALLOWED, "TRDRB @RR0,@RR6,R2");
        ETEST(REGISTER_NOT_ALLOWED, "TRDRB @RR4,@RR0,R2");
    } else {
        TEST("TRDRB @R4,@R5,R6",   0xB84C, 0x0650);
        ETEST(REGISTER_NOT_ALLOWED, "TRDRB @R0,@R5,R6");
        ETEST(REGISTER_NOT_ALLOWED, "TRDRB @R4,@R0,R6");
    }

    // Translate and Increment
    if (z8001()) {
        TEST("TRIB @RR6,@RR8,R3", 0xB860, 0x0380);
        ETEST(REGISTER_NOT_ALLOWED, "TRIB @RR0,@RR8,R3");
        ETEST(REGISTER_NOT_ALLOWED, "TRIB @RR6,@RR0,R3");
    } else {
        TEST("TRIB @R7,@R8,R9",   0xB870, 0x0980);
        ETEST(REGISTER_NOT_ALLOWED, "TRIB @R0,@R8,R9");
        ETEST(REGISTER_NOT_ALLOWED, "TRIB @R7,@R0,R9");
    }

    // Translate, Increment, and Repeat
    if (z8001()) {
        TEST("TRIRB @RR8,@RR10,R12", 0xB884, 0x0CA0);
        ETEST(REGISTER_NOT_ALLOWED, "TRIRB @RR0,@RR10,R12");
        ETEST(REGISTER_NOT_ALLOWED, "TRIRB @RR8,@RR0,R12");
    } else {
        TEST("TRIRB @R10,@R11,R12",  0xB8A4, 0x0CB0);
        ETEST(REGISTER_NOT_ALLOWED, "TRIRB @R0,@R11,R12");
        ETEST(REGISTER_NOT_ALLOWED, "TRIRB @R10,@R0,R12");
    }

    // Translate, Test, and Decrement
    if (z8001()) {
        TEST("TRTDB @RR10,@RR12,R9", 0xB8AA, 0x09C0);
        ETEST(REGISTER_NOT_ALLOWED, "TRTDB @RR0,@RR12,R9");
        ETEST(REGISTER_NOT_ALLOWED, "TRTDB @RR10,@RR0,R9");
    } else {
        TEST("TRTDB @R13,@R14,R15",  0xB8DA, 0x0FE0);
        ETEST(REGISTER_NOT_ALLOWED, "TRTDB @R0,@R14,R15");
        ETEST(REGISTER_NOT_ALLOWED, "TRTDB @R13,@R0,R15");
    }

    // Translate, Test, Decrement, and Repeat
    if (z8001()) {
        TEST("TRTDRB @RR12,@RR14,R0", 0xB8CE, 0x00EE);
        ETEST(REGISTER_NOT_ALLOWED, "TRTDRB @RR0,@RR14,R0");
        ETEST(REGISTER_NOT_ALLOWED, "TRTDRB @RR12,@RR0,R0");
    } else {
        TEST("TRTDRB @R15,@R14,R13",  0xB8FE, 0x0DEE);
        ETEST(REGISTER_NOT_ALLOWED, "TRTDRB @R0,@R14,R13");
        ETEST(REGISTER_NOT_ALLOWED, "TRTDRB @R15,@R0,R13");
    }

    // Translate, Test, and Increment
    if (z8001()) {
        TEST("TRTIB @RR14,@RR12,R11", 0xB8E2, 0x0BC0);
        ETEST(REGISTER_NOT_ALLOWED, "TRTIB @RR0,@RR12,R11");
        ETEST(REGISTER_NOT_ALLOWED, "TRTIB @RR14,@RR0,R11");
    } else {
        TEST("TRTIB @R12,@R11,R10",   0xB8C2, 0x0AB0);
        ETEST(REGISTER_NOT_ALLOWED, "TRTIB @R0,@R11,R10");
        ETEST(REGISTER_NOT_ALLOWED, "TRTIB @R12,@R0,R10");
    }

    // Translate, Test, Increment, and Repeat
    if (z8001()) {
        TEST("TRTIRB @RR10,@RR8,R7", 0xB8A6, 0x078E);
        ETEST(REGISTER_NOT_ALLOWED, "TRTIRB @RR0,@RR8,R7");
        ETEST(REGISTER_NOT_ALLOWED, "TRTIRB @RR10,@RR0,R7");
    } else {
        TEST("TRTIRB @R9,@R8,R7",    0xB896, 0x078E);
        ETEST(REGISTER_NOT_ALLOWED, "TRTIRB @R0,@R8,R7");
        ETEST(REGISTER_NOT_ALLOWED, "TRTIRB @R9,@R0,R7");
    }
}

static void test_input() {
    // Input
    TEST("IN  R1,@R2",      0x3D21);
    TEST("INB RH1,@R2",     0x3C21);
    ETEST(REGISTER_NOT_ALLOWED, "IN  R1,@R0");
    ETEST(REGISTER_NOT_ALLOWED, "INB RH1,@R0");
    TEST("IN  R1,#0x1234",  0x3B14, 0x1234);
    TEST("INB RH1,#0x1234", 0x3A14, 0x1234);

    // Special Input
    TEST("SIN  R1,#0x1234",  0x3B15, 0x1234);
    TEST("SINB RH1,#0x1234", 0x3A15, 0x1234);

    // Input and Decrement
    if (z8001()) {
        TEST("IND  @RR2,@R4,R6", 0x3B48, 0x0628);
        TEST("INDB @RR2,@R4,R6", 0x3A48, 0x0628);
        ETEST(REGISTER_NOT_ALLOWED, "IND  @RR0,@R4,R6");
        ETEST(REGISTER_NOT_ALLOWED, "IND  @RR2,@R0,R6");
        ETEST(REGISTER_NOT_ALLOWED, "INDB @RR0,@R4,R6");
        ETEST(REGISTER_NOT_ALLOWED, "INDB @RR2,@R0,R6");
    } else {
        TEST("IND  @R1,@R2,R0",  0x3B28, 0x0018);
        TEST("INDB @R1,@R2,R0",  0x3A28, 0x0018);
        ETEST(REGISTER_NOT_ALLOWED, "IND  @R0,@R2,R0");
        ETEST(REGISTER_NOT_ALLOWED, "IND  @R1,@R0,R0");
        ETEST(REGISTER_NOT_ALLOWED, "INDB @R0,@R2,R0");
        ETEST(REGISTER_NOT_ALLOWED, "INDB @R1,@R0,R0");
    }

    // Special Input and Decrement
    if (z8001()) {
        TEST("SIND  @RR4,@R3,R0", 0x3B39, 0x0048);
        TEST("SINDB @RR4,@R3,R0", 0x3A39, 0x0048);
        ETEST(REGISTER_NOT_ALLOWED, "SIND  @RR0,@R3,R0");
        ETEST(REGISTER_NOT_ALLOWED, "SIND  @RR4,@R0,R0");
        ETEST(REGISTER_NOT_ALLOWED, "SINDB @RR0,@R3,R0");
        ETEST(REGISTER_NOT_ALLOWED, "SINDB @RR4,@R0,R0");
    } else {
        TEST("SIND  @R3,@R4,R5",  0x3B49, 0x0538);
        TEST("SINDB @R3,@R4,R5",  0x3A49, 0x0538);
        ETEST(REGISTER_NOT_ALLOWED, "SIND  @R0,@R4,R5");
        ETEST(REGISTER_NOT_ALLOWED, "SIND  @R3,@R0,R5");
        ETEST(REGISTER_NOT_ALLOWED, "SINDB @R0,@R4,R5");
        ETEST(REGISTER_NOT_ALLOWED, "SINDB @R3,@R0,R5");
    }

    // Input, Decrement, and Repeat
    if (z8001()) {
        TEST("INDR  @RR6,@R4,R8", 0x3B48, 0x0860);
        TEST("INDRB @RR6,@R4,R8", 0x3A48, 0x0860);
        ETEST(REGISTER_NOT_ALLOWED, "INDR  @RR0,@R4,R8");
        ETEST(REGISTER_NOT_ALLOWED, "INDR  @RR6,@R0,R8");
        ETEST(REGISTER_NOT_ALLOWED, "INDRB @RR0,@R4,R8");
        ETEST(REGISTER_NOT_ALLOWED, "INDRB @RR6,@R0,R8");
    } else {
        TEST("INDR  @R6,@R7,R8",  0x3B78, 0x0860);
        TEST("INDRB @R6,@R7,R8",  0x3A78, 0x0860);
        ETEST(REGISTER_NOT_ALLOWED, "INDR  @R0,@R7,R8");
        ETEST(REGISTER_NOT_ALLOWED, "INDR  @R6,@R0,R8");
        ETEST(REGISTER_NOT_ALLOWED, "INDRB @R0,@R7,R8");
        ETEST(REGISTER_NOT_ALLOWED, "INDRB @R6,@R0,R8");
    }

    // Special Input, Decrement, and Repeat
    if (z8001()) {
        TEST("SINDR  @RR8,@R10,R12", 0x3BA9, 0x0C80);
        TEST("SINDRB @RR8,@R10,R12", 0x3AA9, 0x0C80);
        ETEST(REGISTER_NOT_ALLOWED, "SINDR  @RR0,@R10,R12");
        ETEST(REGISTER_NOT_ALLOWED, "SINDR  @RR8,@R0,R12");
        ETEST(REGISTER_NOT_ALLOWED, "SINDRB @RR0,@R10,R12");
        ETEST(REGISTER_NOT_ALLOWED, "SINDRB @RR8,@R0,R12");
    } else {
        TEST("SINDR  @R9,@R10,R11",  0x3BA9, 0x0B90);
        TEST("SINDRB @R9,@R10,R11",  0x3AA9, 0x0B90);
        ETEST(REGISTER_NOT_ALLOWED, "SINDR  @R0,@R10,R11");
        ETEST(REGISTER_NOT_ALLOWED, "SINDR  @R9,@R0,R11");
        ETEST(REGISTER_NOT_ALLOWED, "SINDRB @R0,@R10,R11");
        ETEST(REGISTER_NOT_ALLOWED, "SINDRB @R9,@R0,R11");
    }

    // Input and Increment
    if (z8001()) {
        TEST("INI  @RR14,@R2,R1", 0x3B20, 0x01E8);
        TEST("INIB @RR14,@R2,R1", 0x3A20, 0x01E8);
        ETEST(REGISTER_NOT_ALLOWED, "INI  @RR0,@R2,R1");
        ETEST(REGISTER_NOT_ALLOWED, "INI  @RR14,@R0,R1");
        ETEST(REGISTER_NOT_ALLOWED, "INIB @RR0,@R2,R1");
        ETEST(REGISTER_NOT_ALLOWED, "INIB @RR14,@R0,R1");
    } else {
        TEST("INI  @R12,@R13,R14", 0x3BD0, 0x0EC8);
        TEST("INIB @R12,@R13,R14", 0x3AD0, 0x0EC8);
        ETEST(REGISTER_NOT_ALLOWED, "INI  @R0,@R13,R14");
        ETEST(REGISTER_NOT_ALLOWED, "INI  @R12,@R0,R14");
        ETEST(REGISTER_NOT_ALLOWED, "INIB @R0,@R13,R14");
        ETEST(REGISTER_NOT_ALLOWED, "INIB @R12,@R0,R14");
    }

    // Special Input and Increment
    if (z8001()) {
        TEST("SINI  @RR12,@R11,R10", 0x3BB1, 0x0AC8);
        TEST("SINIB @RR12,@R11,R10", 0x3AB1, 0x0AC8);
        ETEST(REGISTER_NOT_ALLOWED, "SINI  @RR0,@R11,R10");
        ETEST(REGISTER_NOT_ALLOWED, "SINI  @RR12,@R0,R10");
        ETEST(REGISTER_NOT_ALLOWED, "SINIB @RR0,@R11,R10");
        ETEST(REGISTER_NOT_ALLOWED, "SINIB @RR12,@R0,R10");
    } else {
        TEST("SINI  @R15,@R1,R0", 0x3B11, 0x00F8);
        TEST("SINIB @R15,@R1,R0", 0x3A11, 0x00F8);
        ETEST(REGISTER_NOT_ALLOWED, "SINI  @R0,@R1,R0");
        ETEST(REGISTER_NOT_ALLOWED, "SINI  @R15,@R0,R0");
        ETEST(REGISTER_NOT_ALLOWED, "SINIB @R0,@R1,R0");
        ETEST(REGISTER_NOT_ALLOWED, "SINIB @R15,@R0,R0");
    }

    // Input, Increment and Repeat
    if (z8001()) {
        TEST("INIR  @RR10,@R9,R8", 0x3B90, 0x08A0);
        TEST("INIRB @RR10,@R9,R8", 0x3A90, 0x08A0);
        ETEST(REGISTER_NOT_ALLOWED, "INIR  @RR0,@R9,R8");
        ETEST(REGISTER_NOT_ALLOWED, "INIR  @RR10,@R0,R8");
        ETEST(REGISTER_NOT_ALLOWED, "INIRB @RR0,@R9,R8");
        ETEST(REGISTER_NOT_ALLOWED, "INIRB @RR10,@R0,R8");
    } else {
        TEST("INIR  @R3,@R2,R1", 0x3B20, 0x0130);
        TEST("INIRB @R3,@R2,R1", 0x3A20, 0x0130);
        ETEST(REGISTER_NOT_ALLOWED, "INIR  @R0,@R2,R1");
        ETEST(REGISTER_NOT_ALLOWED, "INIR  @R3,@R0,R1");
        ETEST(REGISTER_NOT_ALLOWED, "INIRB @R0,@R2,R1");
        ETEST(REGISTER_NOT_ALLOWED, "INIRB @R3,@R0,R1");
    }

    // Special Input, Increment, and Repeat
    if (z8001()) {
        TEST("SINIR  @RR8,@R7,R6", 0x3B71, 0x0680);
        TEST("SINIRB @RR8,@R7,R6", 0x3A71, 0x0680);
        ETEST(REGISTER_NOT_ALLOWED, "SINIR  @RR0,@R7,R6");
        ETEST(REGISTER_NOT_ALLOWED, "SINIR  @RR8,@R0,R6");
        ETEST(REGISTER_NOT_ALLOWED, "SINIRB @RR0,@R7,R6");
        ETEST(REGISTER_NOT_ALLOWED, "SINIRB @RR8,@R0,R6");
    } else {
        TEST("SINIR  @R4,@R3,R2", 0x3B31, 0x0240);
        TEST("SINIRB @R4,@R3,R2", 0x3A31, 0x0240);
        ETEST(REGISTER_NOT_ALLOWED, "SINIR  @R0,@R3,R2");
        ETEST(REGISTER_NOT_ALLOWED, "SINIR  @R4,@R0,R2");
        ETEST(REGISTER_NOT_ALLOWED, "SINIRB @R0,@R3,R2");
        ETEST(REGISTER_NOT_ALLOWED, "SINIRB @R4,@R0,R2");
    }
}

static void test_output() {
    // Output
    TEST("OUT  @R2,R1",      0x3F21);
    TEST("OUTB @R2,RH1",     0x3E21);
    ETEST(REGISTER_NOT_ALLOWED, "OUT  @R0,R1");
    ETEST(REGISTER_NOT_ALLOWED, "OUTB @R0,RH1");
    TEST("OUT   #0x1234,R1",  0x3B16, 0x1234);
    TEST("OUTB  #0x1234,RH1", 0x3A16, 0x1234);

    // Special Output
    TEST("SOUT   #0x1234,R1",  0x3B17, 0x1234);
    TEST("SOUTB  #0x1234,RH1", 0x3A17, 0x1234);

    // Output and Decrement
    if (z8001()) {
        TEST("OUTD  @R2,@RR4,R6", 0x3B4A, 0x0628);
        TEST("OUTDB @R2,@RR4,R6", 0x3A4A, 0x0628);
        ETEST(REGISTER_NOT_ALLOWED, "OUTD  @R0,@RR4,R6");
        ETEST(REGISTER_NOT_ALLOWED, "OUTD  @R2,@RR0,R6");
        ETEST(REGISTER_NOT_ALLOWED, "OUTDB @R0,@RR4,R6");
        ETEST(REGISTER_NOT_ALLOWED, "OUTDB @R2,@RR0,R6");
    } else {
        TEST("OUTD  @R2,@R1,R0",  0x3B1A, 0x0028);
        TEST("OUTDB @R2,@R1,R0",  0x3A1A, 0x0028);
        ETEST(REGISTER_NOT_ALLOWED, "OUTD  @R0,@R1,R0",);
        ETEST(REGISTER_NOT_ALLOWED, "OUTD  @R2,@R0,R0",);
        ETEST(REGISTER_NOT_ALLOWED, "OUTDB @R0,@R1,R0",);
        ETEST(REGISTER_NOT_ALLOWED, "OUTDB @R2,@R0,R0",);
    }

    // Special Output and Decrement
    if (z8001()) {
        TEST("SOUTD  @R3,@RR4,R0", 0x3B4B, 0x0038);
        TEST("SOUTDB @R3,@RR4,R0", 0x3A4B, 0x0038);
        ETEST(REGISTER_NOT_ALLOWED, "SOUTD  @R0,@RR4,R0");
        ETEST(REGISTER_NOT_ALLOWED, "SOUTD  @R3,@RR0,R0");
        ETEST(REGISTER_NOT_ALLOWED, "SOUTDB @R0,@RR4,R0");
        ETEST(REGISTER_NOT_ALLOWED, "SOUTDB @R3,@RR0,R0");
    } else {
        TEST("SOUTD  @R3,@R4,R5",  0x3B4B, 0x0538);
        TEST("SOUTDB @R3,@R4,R5",  0x3A4B, 0x0538);
        ETEST(REGISTER_NOT_ALLOWED, "SOUTD  @R0,@R4,R5",);
        ETEST(REGISTER_NOT_ALLOWED, "SOUTD  @R3,@R0,R5",);
        ETEST(REGISTER_NOT_ALLOWED, "SOUTDB @R0,@R4,R5",);
        ETEST(REGISTER_NOT_ALLOWED, "SOUTDB @R3,@R0,R5",);
    }

    // Output, Decrement, and Repeat
    if (z8001()) {
        TEST("OTDR  @R4,@RR6,R8", 0x3B6A, 0x0840);
        TEST("OTDRB @R4,@RR6,R8", 0x3A6A, 0x0840);
        ETEST(REGISTER_NOT_ALLOWED, "OTDR  @R0,@RR6,R8");
        ETEST(REGISTER_NOT_ALLOWED, "OTDR  @R4,@RR0,R8");
        ETEST(REGISTER_NOT_ALLOWED, "OTDRB @R0,@RR6,R8");
        ETEST(REGISTER_NOT_ALLOWED, "OTDRB @R4,@RR0,R8");
    } else {
        TEST("OTDR  @R6,@R7,R8",  0x3B7A, 0x0860);
        TEST("OTDRB @R6,@R7,R8",  0x3A7A, 0x0860);
        ETEST(REGISTER_NOT_ALLOWED, "OTDR  @R0,@R7,R8",);
        ETEST(REGISTER_NOT_ALLOWED, "OTDR  @R6,@R0,R8",);
        ETEST(REGISTER_NOT_ALLOWED, "OTDRB @R0,@R7,R8",);
        ETEST(REGISTER_NOT_ALLOWED, "OTDRB @R6,@R0,R8",);
    }

    // Special Output, Decrement, and Repeat
    if (z8001()) {
        TEST("SOTDR  @R8,@RR10,R12", 0x3BAB, 0x0C80);
        TEST("SOTDRB @R8,@RR10,R12", 0x3AAB, 0x0C80);
        ETEST(REGISTER_NOT_ALLOWED, "SOTDR  @R0,@RR10,R12");
        ETEST(REGISTER_NOT_ALLOWED, "SOTDR  @R8,@RR0,R12");
        ETEST(REGISTER_NOT_ALLOWED, "SOTDRB @R0,@RR10,R12");
        ETEST(REGISTER_NOT_ALLOWED, "SOTDRB @R8,@RR0,R12");
    } else {
        TEST("SOTDR  @R9,@R10,R11",  0x3BAB, 0x0B90);
        TEST("SOTDRB @R9,@R10,R11",  0x3AAB, 0x0B90);
        ETEST(REGISTER_NOT_ALLOWED, "SOTDR  @R0,@R10,R11",);
        ETEST(REGISTER_NOT_ALLOWED, "SOTDR  @R9,@R0,R11",);
        ETEST(REGISTER_NOT_ALLOWED, "SOTDRB @R0,@R10,R11",);
        ETEST(REGISTER_NOT_ALLOWED, "SOTDRB @R9,@R0,R11",);
    }

    // Output and Increment
    if (z8001()) {
        TEST("OUTI  @R14,@RR2,R1", 0x3B22, 0x01E8);
        TEST("OUTIB @R14,@RR2,R1", 0x3A22, 0x01E8);
        ETEST(REGISTER_NOT_ALLOWED, "OUTI  @R0,@RR2,R1");
        ETEST(REGISTER_NOT_ALLOWED, "OUTI  @R14,@RR0,R1");
        ETEST(REGISTER_NOT_ALLOWED, "OUTIB @R0,@RR2,R1");
        ETEST(REGISTER_NOT_ALLOWED, "OUTIB @R14,@RR0,R1");
    } else {
        TEST("OUTI  @R12,@R13,R14", 0x3BD2, 0x0EC8);
        TEST("OUTIB @R12,@R13,R14", 0x3AD2, 0x0EC8);
        ETEST(REGISTER_NOT_ALLOWED, "OUTI  @R0,@R13,R14");
        ETEST(REGISTER_NOT_ALLOWED, "OUTI  @R12,@R0,R14");
        ETEST(REGISTER_NOT_ALLOWED, "OUTIB @R0,@R13,R14");
        ETEST(REGISTER_NOT_ALLOWED, "OUTIB @R12,@R0,R14");
    }

    // Special Output and Increment
    if (z8001()) {
        TEST("SOUTI  @R10,@RR12,R11", 0x3BC3, 0x0BA8);
        TEST("SOUTIB @R10,@RR12,R11", 0x3AC3, 0x0BA8);
        ETEST(REGISTER_NOT_ALLOWED, "SOUTI  @R0,@RR12,R11");
        ETEST(REGISTER_NOT_ALLOWED, "SOUTI  @R10,@RR0,R11");
        ETEST(REGISTER_NOT_ALLOWED, "SOUTIB @R0,@RR12,R11");
        ETEST(REGISTER_NOT_ALLOWED, "SOUTIB @R10,@RR0,R11");
    } else {
        TEST("SOUTI  @R15,@R1,R0", 0x3B13, 0x00F8);
        TEST("SOUTIB @R15,@R1,R0", 0x3A13, 0x00F8);
        ETEST(REGISTER_NOT_ALLOWED, "SOUTI  @R0,@R1,R0");
        ETEST(REGISTER_NOT_ALLOWED, "SOUTI  @R15,@R0,R0");
        ETEST(REGISTER_NOT_ALLOWED, "SOUTIB @R0,@R1,R0");
        ETEST(REGISTER_NOT_ALLOWED, "SOUTIB @R15,@R0,R0");
    }

    // Output, Increment and Repeat
    if (z8001()) {
        TEST("OTIR  @R9,@RR10,R8", 0x3BA2, 0x0890);
        TEST("OTIRB @R9,@RR10,R8", 0x3AA2, 0x0890);
        ETEST(REGISTER_NOT_ALLOWED, "OTIR  @R0,@RR10,R8");
        ETEST(REGISTER_NOT_ALLOWED, "OTIR  @R9,@RR0,R8");
        ETEST(REGISTER_NOT_ALLOWED, "OTIRB @R0,@RR10,R8");
        ETEST(REGISTER_NOT_ALLOWED, "OTIRB @R9,@RR0,R8");
    } else {
        TEST("OTIR  @R3,@R2,R1", 0x3B22, 0x0130);
        TEST("OTIRB @R3,@R2,R1", 0x3A22, 0x0130);
        ETEST(REGISTER_NOT_ALLOWED, "OTIR  @R0,@R2,R1");
        ETEST(REGISTER_NOT_ALLOWED, "OTIR  @R3,@R0,R1");
        ETEST(REGISTER_NOT_ALLOWED, "OTIRB @R0,@R2,R1");
        ETEST(REGISTER_NOT_ALLOWED, "OTIRB @R3,@R0,R1");
    }

    // Special Output, Increment, and Repeat
    if (z8001()) {
        TEST("SOTIR  @R7,@RR8,R6", 0x3B83, 0x0670);
        TEST("SOTIRB @R7,@RR8,R6", 0x3A83, 0x0670);
        ETEST(REGISTER_NOT_ALLOWED, "SOTIR  @R0,@RR8,R6");
        ETEST(REGISTER_NOT_ALLOWED, "SOTIR  @R7,@RR0,R6");
        ETEST(REGISTER_NOT_ALLOWED, "SOTIRB @R0,@RR8,R6");
        ETEST(REGISTER_NOT_ALLOWED, "SOTIRB @R7,@RR0,R6");
    } else {
        TEST("SOTIR  @R4,@R3,R2", 0x3B33, 0x0240);
        TEST("SOTIRB @R4,@R3,R2", 0x3A33, 0x0240);
        ETEST(REGISTER_NOT_ALLOWED, "SOTIR  @R0,@R3,R2");
        ETEST(REGISTER_NOT_ALLOWED, "SOTIR  @R4,@R0,R2");
        ETEST(REGISTER_NOT_ALLOWED, "SOTIRB @R0,@R3,R2");
        ETEST(REGISTER_NOT_ALLOWED, "SOTIRB @R4,@R0,R2");
    }
}

static void test_cpu_conrtol() {
    // Complement Flag
    ETEST(OPCODE_HAS_NO_EFFECT, "COMFLG");
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
    TEST("DI VI,NVI",  0x7C00);
    TEST("DI VI",      0x7C01);
    TEST("DI NVI",     0x7C02);
    ETEST(OPCODE_HAS_NO_EFFECT, "DI");

    // Enable Interrupt
    TEST("EI VI,NVI",  0x7C04);
    TEST("EI VI",      0x7C05);
    TEST("EI NVI",     0x7C06);
    ETEST(OPCODE_HAS_NO_EFFECT, "EI");

    // Halt
    TEST("HALT  ", 0x7A00);

    // Load Control Register
    ETEST(ILLEGAL_SIZE, "LDCTL R14,FLAGS");
    TEST("LDCTL R13,FCW",     0x7DD2);
    TEST("LDCTL R12,REFRESH", 0x7DC3);
    TEST("LDCTL R11,PSAPSEG", 0x7DB4);
    TEST("LDCTL R10,PSAPOFF", 0x7DA5);
    TEST("LDCTL R9,NSPSEG",   0x7D96);
    TEST("LDCTL R8,NSPOFF",   0x7D87);
    ETEST(ILLEGAL_SIZE, "LDCTL FLAGS,R6");
    TEST("LDCTL FCW,R5",      0x7D5A);
    TEST("LDCTL REFRESH,R4",  0x7D4B);
    TEST("LDCTL PSAPSEG,R3",  0x7D3C);
    TEST("LDCTL PSAPOFF,R2",  0x7D2D);
    TEST("LDCTL NSPSEG,R1",   0x7D1E);
    TEST("LDCTL NSPOFF,R0",   0x7D0F);
    ETEST(ILLEGAL_SIZE, "LDCTLB RL0,FCW");
    ETEST(ILLEGAL_SIZE, "LDCTLB RL1,REFRESH");
    ETEST(ILLEGAL_SIZE, "LDCTLB RL2,PSAPSEG");
    ETEST(ILLEGAL_SIZE, "LDCTLB RL3,PSAPOFF");
    ETEST(ILLEGAL_SIZE, "LDCTLB RL4,NSPSEG");
    ETEST(ILLEGAL_SIZE, "LDCTLB RL5,NSPOFF");
    TEST("LDCTLB RL6,FLAGS",   0x8CE1);
    ETEST(ILLEGAL_SIZE, "LDCTLB FCW,RH0");
    ETEST(ILLEGAL_SIZE, "LDCTLB REFRESH,RH1");
    ETEST(ILLEGAL_SIZE, "LDCTLB PSAPSEG,RH2");
    ETEST(ILLEGAL_SIZE, "LDCTLB PSAPOFF,RH3");
    ETEST(ILLEGAL_SIZE, "LDCTLB NSPSEG,RH4");
    ETEST(ILLEGAL_SIZE, "LDCTLB NSPOFF,RH5");
    TEST("LDCTLB FLAGS,RH6",   0x8C69);

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
    ETEST(OPCODE_HAS_NO_EFFECT, "RESFLG");
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
    ETEST(OPCODE_HAS_NO_EFFECT, "SETFLG");
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

static void test_comment() {
    TEST(                 "ADD R1 , # 0x1234", 0x0101, 0x1234);
    if (z8001()) {
        TEST("EXB RL1 , 0x1234 ( R2 ) ; comment", 0x6C29 ,0x8000, 0x1234);
        TEST(                   "PUSH @RR4 , R2",  0x9342);
        ETEST(UNKNOWN_REGISTER, "PUSH @ RR4 , R2");
    } else {
        TEST("EXB RL1 , 0x1234 ( R2 ) ; comment", 0x6C29 ,0x1234);
        TEST(                   "PUSH @R4 , R2",  0x9342);
        ETEST(UNKNOWN_REGISTER, "PUSH @ R4 , R2");
    }
    TEST("SETFLG C , Z , S , P , V", 0x8DF1);
}

static void test_undefined_symbol() {
    ETEST(UNDEFINED_SYMBOL, "LDB RH2,#UNDEF",   0xC200);
    ETEST(UNDEFINED_SYMBOL, "LD  R2,#UNDEF",    0x2102, 0x0000);
    ETEST(UNDEFINED_SYMBOL, "LDL RR2,#UNDEF",   0x1402, 0x0000, 0x0000);
    ETEST(UNDEFINED_SYMBOL, "LD  R9,UNDEF",     0x6109, 0x0000);
    ETEST(UNDEFINED_SYMBOL, "LD  R9,UNDEF(R2)", 0x6129, 0x0000);
    ETEST(UNDEFINED_SYMBOL, "LD  UNDEF,R9",     0x6F09, 0x0000);
    ETEST(UNDEFINED_SYMBOL, "LD  UNDEF(R2),R9", 0x6F29, 0x0000);
    if (z8001()) {
        ETEST(UNDEFINED_SYMBOL, "LD  R9,RR2(#UNDEF)",  0x3129, 0x0000);
        ETEST(UNDEFINED_SYMBOL, "LD  RR2(#UNDEF),R9",  0x3329, 0x0000);
        ETEST(UNDEFINED_SYMBOL, "LDA RR8,UNDEF",       0x7608, 0x0000);
        ETEST(UNDEFINED_SYMBOL, "LDA RR8,UNDEF(R2)",   0x7628, 0x0000);
        ETEST(UNDEFINED_SYMBOL, "LDA RR8,RR2(#UNDEF)", 0x3428, 0x0000);
        ETEST(UNDEFINED_SYMBOL, "LDAR RR8,UNDEF",      0x3408, 0x0000);
        ETEST(UNDEFINED_SYMBOL, "LDM R1,@RR2,#UNDEF",  0x1C21, 0x0100);
        ETEST(UNDEFINED_SYMBOL, "LDM R1,UNDEF,#2",     0x5C01, 0x0101, 0x0000);
        ETEST(UNDEFINED_SYMBOL, "LDM R1,UNDEF,#UNDEF", 0x5C01, 0x0100, 0x0000);
    } else {
        ETEST(UNDEFINED_SYMBOL, "LD  R9,R2(#UNDEF)",   0x3129, 0x0000);
        ETEST(UNDEFINED_SYMBOL, "LD  R2(#UNDEF),R9",   0x3329, 0x0000);
        ETEST(UNDEFINED_SYMBOL, "LDA R8,UNDEF",        0x7608, 0x0000);
        ETEST(UNDEFINED_SYMBOL, "LDA R8,UNDEF(R2)",    0x7628, 0x0000);
        ETEST(UNDEFINED_SYMBOL, "LDA R8,R2(#UNDEF)",   0x3428, 0x0000);
        ETEST(UNDEFINED_SYMBOL, "LDAR R8,UNDEF",       0x3408, 0x0000);
        ETEST(UNDEFINED_SYMBOL, "LDM R1,@R2,#UNDEF",   0x1C21, 0x0100);
        ETEST(UNDEFINED_SYMBOL, "LDM R1,UNDEF,#2",     0x5C01, 0x0101, 0x0000);
        ETEST(UNDEFINED_SYMBOL, "LDM R1,UNDEF,#UNDEF", 0x5C01, 0x0100, 0x0000);
    }
    ETEST(UNDEFINED_SYMBOL, "LDK R1,#UNDEF", 0xBD10);
    ETEST(UNDEFINED_SYMBOL, "LDR R1,UNDEF",  0x3101, 0x0000);
    ETEST(UNDEFINED_SYMBOL, "LDR UNDEF,R1",  0x3301, 0x0000);
    ETEST(UNDEFINED_SYMBOL, "DEC R1,#UNDEF", 0xAB10);

    ETEST(UNDEFINED_SYMBOL, "CALL UNDEF",      0x5F00, 0x0000);
    ETEST(UNDEFINED_SYMBOL, "CALL UNDEF(R2)",  0x5F20, 0x0000);
    ETEST(UNDEFINED_SYMBOL, "CALR UNDEF",      0xD000);
    ETEST(UNDEFINED_SYMBOL, "DJNZ  R2,UNDEF",  0xF280);
    ETEST(UNDEFINED_SYMBOL, "DBJNZ RH2,UNDEF", 0xF200);
    ETEST(UNDEFINED_SYMBOL, "JP UNDEF",        0x5E08, 0x0000);
    ETEST(UNDEFINED_SYMBOL, "JP C,UNDEF",      0x5E07, 0x0000);
    ETEST(UNDEFINED_SYMBOL, "JP C,UNDEF(R2)",  0x5E27, 0x0000);
    ETEST(UNDEFINED_SYMBOL, "JR UNDEF",        0xE800);
    ETEST(UNDEFINED_SYMBOL, "JR C,UNDEF",      0xE700);
    ETEST(UNDEFINED_SYMBOL, "SC #UNDEF",       0x7F00);

    ETEST(UNDEFINED_SYMBOL, "BIT R2,#UNDEF",        0xA720);
    ETEST(UNDEFINED_SYMBOL, "BIT UNDEF,#4",         0x6704, 0x0000);
    ETEST(UNDEFINED_SYMBOL, "BIT UNDEF(R2),#4",     0x6724, 0x0000);
    ETEST(UNDEFINED_SYMBOL, "BIT UNDEF,#UNDEF",     0x6700, 0x0000);
    ETEST(UNDEFINED_SYMBOL, "BIT UNDEF(R2),#UNDEF", 0x6720, 0x0000);

    ETEST(UNDEFINED_SYMBOL, "RL  R1,#UNDEF", 0xB310);
    ETEST(UNDEFINED_SYMBOL, "SLA R2,#UNDEF", 0xB329, 0x0000);
    ETEST(UNDEFINED_SYMBOL, "SRA R2,#UNDEF", 0xB329, 0x0000);

    ETEST(UNDEFINED_SYMBOL, "IN  R1,#UNDEF", 0x3B14, 0x0000);
    ETEST(UNDEFINED_SYMBOL, "OUT #UNDEF,R1", 0x3B16, 0x0000);
}

static void run_test(void (*test)(), const char *test_name) {
    asserter.clear(test_name);
    set_up();
    test();
    tear_down();
    asserter.check();
}

int main(int argc, char **argv) {
    RUN_TEST(test_cpu);
    static const char *cpus[] = {
        "Z8001", "Z8002",
    };
    for (size_t i = 0; i < sizeof(cpus)/sizeof(cpus[0]); i++) {
        const char *cpu = cpus[i];
        assembler.setCpu(cpu);
        printf("  TEST CPU %s\n", cpu);
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
        RUN_TEST(test_comment);
        RUN_TEST(test_undefined_symbol);
    }
    return 0;
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
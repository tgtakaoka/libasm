/*
 * Copyright 2024 Tadashi G. Takaoka
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

#include "dis_pdp11.h"
#include "test_dis_helper.h"

using namespace libasm;
using namespace libasm::pdp11;
using namespace libasm::test;

DisPdp11 dispdp11;
Disassembler &disassembler(dispdp11);

static bool dcj11() {
    return strcmp_P("DCJ11", disassembler.config().cpu_P()) == 0;
}

static void set_up() {
    disassembler.reset();
    disassembler.setOption("relative", "true");
}

static void tear_down() {
    symtab.reset();
}

// clang-format off
void test_cpu() {
    EQUALS("cpu DCT11", true,    disassembler.setCpu("dct11"));
    EQUALS_P("cpu DCT11", "DCT11", disassembler.config().cpu_P());

    EQUALS("cpu T-11", true,    disassembler.setCpu("T-11"));
    EQUALS_P("cpu T-11", "DCT11", disassembler.config().cpu_P());

    EQUALS("cpu DCJ11", true,    disassembler.setCpu("DCJ11"));
    EQUALS_P("cpu DCJ11", "DCJ11", disassembler.config().cpu_P());

    EQUALS("cpu J-11", true,    disassembler.setCpu("J-11"));
    EQUALS_P("cpu J-11", "DCJ11", disassembler.config().cpu_P());
}

void test_double_operands() {
    TEST("MOV", "R0, R1",          0010001);
    TEST("MOV", "R1, (R2)",        0010112);
    TEST("MOV", "R2, (R3)+",       0010223);
    ERRT("MOV", "R2, #012345",  OPERAND_NOT_ALLOWED, "#012345", 0010227, 012345);
    TEST("MOV", "R3, @(R4)+",      0010334);
    ERRT("MOV", "R3, @#012345", OPERAND_NOT_ALIGNED, "@#012345", 0010337, 012345);
    TEST("MOV", "R3, @#012346",    0010337, 012346);
    TEST("MOV", "R4, -(R5)",       0010445);
    TEST("MOV", "R5, @-(SP)",      0010556);
    ERRT("MOV", "SP, .+012345", OPERAND_NOT_ALIGNED, ".+012345", 0010667, 012341);
    TEST("MOV", "SP, .+012346",    0010667, 012342);
    TEST("MOV", "PC, @012345(R0)", 0010770, 012345);
    ERRT("MOV", "PC, @.+012345", OPERAND_NOT_ALIGNED, "@.+012345", 0010777, 012341);
    TEST("MOV", "PC, @.+012346",   0010777, 012342);
    TEST("MOV", "R1, R0",          0010100);
    TEST("MOV", "(R2), R1",        0011201);
    TEST("MOV", "(R3)+, R2",       0012302);
    TEST("MOV", "#012345, R2",     0012702, 012345);
    TEST("MOV", "@(R4)+, R3",      0013403);
    ERRT("MOV", "@#012345, R3", OPERAND_NOT_ALIGNED, "@#012345, R3", 0013703, 012345);
    TEST("MOV", "@#012346, R3",    0013703, 012346);
    TEST("MOV", "-(R5), R4",       0014504);
    TEST("MOV", "@-(SP), R5",      0015605);
    ERRT("MOV", ".+012345, SP", OPERAND_NOT_ALIGNED, ".+012345, SP", 0016706, 012341);
    TEST("MOV", ".+012346, SP",    0016706, 012342);
    TEST("MOV", "@012345(R0), PC", 0017007, 012345);
    TEST("MOV", "@.+012346, PC",   0017707, 012342);

    TEST("MOVB", "R0, R1",          0110001);
    TEST("MOVB", "R1, (R2)",        0110112);
    TEST("MOVB", "R2, (R3)+",       0110223);
    ERRT("MOVB", "R2, #377",       OPERAND_NOT_ALLOWED, "#377", 0110227, 000377);
    TEST("MOVB", "R3, @(R4)+",      0110334);
    TEST("MOVB", "R3, @#012345",    0110337, 012345);
    TEST("MOVB", "R4, -(R5)",       0110445);
    TEST("MOVB", "R5, @-(SP)",      0110556);
    TEST("MOVB", "SP, .+012345",    0110667, 012341);
    TEST("MOVB", "PC, @012345(R0)", 0110770, 012345);
    TEST("MOVB", "R1, R0",          0110100);
    TEST("MOVB", "(R2), R1",        0111201);
    TEST("MOVB", "(R3)+, R2",       0112302);
    TEST("MOVB", "#377, R2",        0112702, 000377);
    TEST("MOVB", "@(R4)+, R3",      0113403);
    TEST("MOVB", "@#012345, R3",    0113703, 012345);
    TEST("MOVB", "-(R5), R4",       0114504);
    TEST("MOVB", "@-(SP), R5",      0115605);
    TEST("MOVB", ".+012345, SP",    0116706, 012341);
    TEST("MOVB", "@012345(R0), PC", 0117007, 012345);

    TEST("CMP", "R0, R1",        0020001);
    TEST("CMP", "#000000, #177777", 0022727, 0000000, 0177777);
    TEST("BIT", "R1, (R2)",      0030112);
    TEST("BIC", "R2, (R3)+",     0040223);
    TEST("BIS", "R3, @(R4)+",    0050334);
    TEST("ADD", "R4, -(R5)",     0060445);
    TEST("SUB", "R5, @-(SP)",    0160556);
    TEST("CMP", "R1, R0",        0020100);
    TEST("BIT", "(R2), R1",      0031201);
    TEST("BIC", "(R3)+, R2",     0042302);
    TEST("BIS", "@(R4)+, R3",    0053403);
    TEST("ADD", "-(R5), R4",     0064504);
    TEST("SUB", "@-(SP), R5",    0165605);

    TEST("CMPB", "R0, R1",     0120001);
    TEST("CMPB", "#377, #001", 0122727, 0000377, 0000001);
    TEST("BITB", "R1, (R2)",   0130112);
    TEST("BICB", "R2, (R3)+",  0140223);
    TEST("BISB", "R3, @(R4)+", 0150334);
    TEST("CMPB", "R1, R0",     0120100);
    TEST("BITB", "(R2), R1",   0131201);
    TEST("BICB", "(R3)+, R2",  0142302);
    TEST("BISB", "@(R4)+, R3", 0153403);

    TEST("XOR", "R1, (R2)",  074112);

    if (dcj11()) {
        TEST("MUL", "R1, R2",      070201);
        TEST("DIV", "#^D5349, R3", 071327, 012345);
        TEST("ASH", "(R5), R4",    072415);
        TEST("ASHC", "@#012346, SP",  073637, 012346);
    }
}

void test_single_operand() {
    TEST("SWAB", "(R0)",  000310);
    TEST("CLR", "(R1)+",  005021);
    TEST("COM", "@(R2)+", 005132);
    TEST("INC", "-(R3)",  005243);
    TEST("DEC", "@-(R4)", 005354);
    TEST("NEG", "012345(R5)",  005465, 012345);
    TEST("ADC", "@012345(SP)", 005576, 012345);
    TEST("SBC", ".+012346",    005667, 012342);
    TEST("TST", "@.+012346",   005777, 012342);
    TEST("ROL", "(R1)+",  006121);
    TEST("ASR", "@(R2)+", 006232);
    TEST("ASL", "-(R3)",  006343);
    TEST("SXT", "@-(R4)", 006754);

    if (dcj11()) {
        TEST("TSTSET", "012345(R5)",  007265, 012345);
        TEST("WRTLCK", "@012345(SP)", 007376, 012345);
        ERRT("TSTSET", "R5", OPERAND_NOT_ALLOWED, "R5", 007205);
        ERRT("WRTLCK", "SP", OPERAND_NOT_ALLOWED, "SP", 007306);
    }

    TEST("CLRB", "(R1)+",  0105021);
    TEST("COMB", "@(R2)+", 0105132);
    TEST("INCB", "-(R3)",  0105243);
    TEST("DECB", "@-(R4)", 0105354);
    TEST("NEGB", "012345(R5)",  0105465, 012345);
    TEST("ADCB", "@012345(SP)", 0105576, 012345);
    TEST("SBCB", ".+012346",    0105667, 012342);
    TEST("TSTB", "@.+012346",   0105777, 012342);
    TEST("ROLB", "(R1)+",  0106121);
    TEST("ASRB", "@(R2)+", 0106232);
    TEST("ASLB", "-(R3)",  0106343);
}

void test_program_control() {
    TEST("BPT", "", 000003);
    TEST("IOT", "", 000004);
    TEST("EMT",  "377", 0104377);
    TEST("TRAP", "377", 0104777);
    TEST("RTS", "R2",   000202);
    TEST("JMP", "(R3)", 000113);
    ERRT("JMP", "R3", OPERAND_NOT_ALLOWED, "R3", 000103);
    TEST("JSR", "PC, @#012346", 004737, 012346);
    ERRT("JSR", "R2, R1", OPERAND_NOT_ALLOWED, "R1", 004201);

    if (dcj11()) {
        TEST("SPL", "3",   000233);
        TEST("MARK", "04", 006404);
        TEST("CSM", "(R4)+", 007024);
    }

    ATEST(01000, "SOB",  "R2, .-174", 0077277);
    ATEST(01000, "BR",   ".-376",     0000600);
    ATEST(01000, "BNE",  ".",         0001377);
    ATEST(01000, "BEQ",  ".+400",     0001577);
    ATEST(01000, "BGE",  ".+002",     0002000);
    ATEST(01000, "BLT",  ".-376",     0002600);
    ATEST(01000, "BGT",  ".",         0003377);
    ATEST(01000, "BLE",  ".+400",     0003577);
    ATEST(01000, "BLT",  ".+002",     0002400);
    ATEST(01000, "BPL",  ".-376",     0100200);
    ATEST(01000, "BMI",  ".+002",     0100400);
    ATEST(01000, "BHI",  ".",         0101377);
    ATEST(01000, "BLOS", ".+400",     0101577);
    ATEST(01000, "BVC",  ".+002",     0102000);
    ATEST(01000, "BVS",  ".-376",     0102600);
    ATEST(01000, "BCC",  ".",         0103377);
    ATEST(01000, "BCS",  ".+400",     0103577);
}

void test_condition_code() {
    TEST("NOP", "", 000240);
    TEST("CLC", "", 000241);
    TEST("CLV", "", 000242);
    TEST("CLZ", "", 000244);
    TEST("CLN", "", 000250);
    TEST("CCC", "", 000257);
    TEST("NOP", "", 000260);
    TEST("SEC", "", 000261);
    TEST("SEV", "", 000262);
    TEST("SEZ", "", 000264);
    TEST("SEN", "", 000270);
    TEST("SCC", "", 000277);
}

void test_miscellaneous() {
    TEST("HALT",  "", 000000);
    TEST("WAIT",  "", 000001);
    TEST("RTI",   "", 000002);
    TEST("RESET", "", 000005);
    TEST("RTT",   "", 000006);
    TEST("MFPT",  "", 000007);
    TEST("MTPS",  "R1",   0106401);
    TEST("MTPS",  "#377", 0106427, 000377);
    ERRT("MTPS",  "#000400", OVERFLOW_RANGE, "000400", 0106427, 000400);
    TEST("MFPS",  "R1", 0106701);

    if (dcj11()) {
        TEST("MFPI", "(R1)+",  0006521);
        TEST("MTPI", "-(R2)",  0006642);
        TEST("MFPD", "@(R3)+", 0106533);
        TEST("MTPD", "@-(R4)", 0106654);
        ERRT("MFPI", "R1", OPERAND_NOT_ALLOWED, "R1", 0006501);
        ERRT("MTPI", "R2", OPERAND_NOT_ALLOWED, "R2", 0006602);
        ERRT("MFPD", "R3", OPERAND_NOT_ALLOWED, "R3", 0106503);
        ERRT("MTPD", "R4", OPERAND_NOT_ALLOWED, "R4", 0106604);
    }
}

void test_floating_point() {
    TEST("CFCC", "", 0170000);
    TEST("SETF", "", 0170001);
    TEST("SETI", "", 0170002);
    TEST("SETD", "", 0170011);
    TEST("SETL", "", 0170012);

    TEST("LDFPS", "#123456", 0170127, 0123456);
    TEST("STFPS", "-(SP)",   0170246);
    TEST("STST",  "(R4)",    0170314);
    TEST("CLRD",  "AC5",     0170405);
    TEST("TSTD",  "(R4)",    0170514);
    TEST("ABSD",  "(R3)+",   0170623);
    TEST("NEGD",  "-(R2)",   0170742);
    TEST("MULD",  "AC1, AC2",    0171201);
    TEST("MULD",  "#-2, AC2",    0171227, 0xC100, 0x0000, 0x0000, 0x0000);
    TEST("MODD",  "(R1), AC3",   0171711);
    TEST("ADDD",  "(R2)+, AC1",  0172122);
    TEST("LDD",   "-(R3), AC0",  0172443);
    TEST("SUBD",  "@(R4)+, AC3", 0173334);
    TEST("CMPD",  "#0.5, AC2",   0173627, 0x4000, 0x0000, 0x0000, 0x0000);
    TEST("STD",   "AC1, -(SP)",  0174146);
    TEST("DIVD",  "(SP)+, AC0",  0174426);
    TEST("STEXP", "AC3, (R1)",   0175311);
    TEST("STCDL", "AC2, -(SP)",  0175646);
    TEST("STCFD", "AC1, (R2)+",  0176122);
    TEST("LDEXP", "(R3), AC0",   0176413);
    TEST("LDCLD", "(SP)+, AC1",  0177126);
    TEST("LDCDF", "(R4)+, AC2",  0177624);
}

// clang-format on

void run_tests(const char *cpu) {
    disassembler.setCpu(cpu);
    RUN_TEST(test_double_operands);
    RUN_TEST(test_single_operand);
    RUN_TEST(test_program_control);
    RUN_TEST(test_condition_code);
    RUN_TEST(test_miscellaneous);
    if (dcj11())
        RUN_TEST(test_floating_point);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

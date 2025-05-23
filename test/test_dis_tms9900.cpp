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

#include "array_memory.h"
#include "dis_tms9900.h"
#include "test_dis_helper.h"

using namespace libasm;
using namespace libasm::tms9900;
using namespace libasm::test;

DisTms9900 dis9900;
Disassembler &disassembler(dis9900);

bool is99110() {
    return strcmp_P("99110", disassembler.config().cpu_P()) == 0;
}

bool is99105() {
    return strcmp_P("99105", disassembler.config().cpu_P()) == 0;
}

bool is9980() {
    return strcmp_P("9980", disassembler.config().cpu_P()) == 0;
}

bool is9900() {
    return strcmp_P("9900", disassembler.config().cpu_P()) == 0 || is9980();
}

bool is9995() {
    return strcmp_P("9995", disassembler.config().cpu_P()) == 0;
}

void set_up() {
    disassembler.reset();
}

void tear_down() {
    symtab.reset();
}

// clang-format off
void test_cpu() {
    EQUALS("cpu 9900", true,   disassembler.setCpu("9900"));
    EQUALS_P("cpu 9900", "9900", disassembler.config().cpu_P());

    EQUALS("cpu 9980", true,   disassembler.setCpu("9980"));
    EQUALS_P("cpu 9980", "9980", disassembler.config().cpu_P());

    EQUALS("cpu 9995", true,   disassembler.setCpu("9995"));
    EQUALS_P("cpu 9995", "9995", disassembler.config().cpu_P());

    EQUALS("cpu 99105", true,    disassembler.setCpu("99105"));
    EQUALS_P("cpu 99105", "99105", disassembler.config().cpu_P());

    EQUALS("cpu TMS9900", true,   disassembler.setCpu("TMS9900"));
    EQUALS_P("cpu TMS9900", "9900", disassembler.config().cpu_P());

    EQUALS("cpu TMS9980", true,   disassembler.setCpu("TMS9980"));
    EQUALS_P("cpu TMS9980", "9980", disassembler.config().cpu_P());

    EQUALS("cpu TMS9995", true,   disassembler.setCpu("TMS9995"));
    EQUALS_P("cpu TMS9995", "9995", disassembler.config().cpu_P());

    EQUALS("cpu TMS99105", true,    disassembler.setCpu("TMS99105"));
    EQUALS_P("cpu TMS99105", "99105", disassembler.config().cpu_P());
}

void test_inh() {
    TEST("IDLE", "", 0x0340);
    TEST("RSET", "", 0x0360);
    TEST("RTWP", "", 0x0380);
    TEST("CKON", "", 0x03A0);
    TEST("CKOF", "", 0x03C0);
    TEST("LREX", "", 0x03E0);

    if (is99105() || is99110()) {
        TEST("RTWP", "1", 0x0381);
        TEST("RTWP", "2", 0x0382);
        TEST("RTWP", "4", 0x0384);
    } else {
        UNKN(0x0381);
        UNKN(0x0382);
        UNKN(0x0384);
    }
    if (is99110()) {
        TEST("LDS",  "", 0x0780);
        TEST("LDD",  "", 0x07C0);
        TEST("CRI",  "", 0x0C00);
        TEST("NEGR", "", 0x0C02);
        TEST("CRE",  "", 0x0C04);
        TEST("CER",  "", 0x0C06);
    } else {
        UNKN(0x0780);
        UNKN(0x07C0);
        UNKN(0x0C00);
        UNKN(0x0C02);
        UNKN(0x0C04);
        UNKN(0x0C06);
    }
}

void test_imm() {
    TEST("LWPI", ">1234",  0x02E0, 0x1234);
    TEST("LIMI", ">89AB",  0x0300, 0x89AB);
    NMEM("LIMI", "0", "0", 0x0300);

    symtab.intern(0x1234, "sym1234");
    symtab.intern(0x89AB, "sym89AB");

    TEST("LWPI", "sym1234", 0x02E0, 0x1234);
    TEST("LIMI", "sym89AB", 0x0300, 0x89AB);
}

void test_reg() {
    TEST("STWP", "R14", 0x02AE);
    TEST("STST", "R15", 0x02CF);

    if (is9900()) {
        UNKN(0x0080);
        UNKN(0x0091);
    } else {
        // TMS9995, TMS99105
        TEST("LST", "R0", 0x0080);
        TEST("LWP", "R1", 0x0091);
    }
}

void test_reg_imm() {
    TEST("LI",   "R0, 0",       0x0200, 0x0000);
    TEST("AI",   "R1, 10",      0x0221, 0x000A);
    TEST("ANDI", "R8, >00FF",   0x0248, 0x00FF);
    TEST("ORI",  "R14, >FF00",  0x026E, 0xFF00);
    TEST("CI",   "R15, >FFFF",  0x028F, 0xFFFF);
    NMEM("CI",   "R15, 0", "0", 0x028F);

    symtab.intern(0x1234, "sym1234");

    TEST("LI",   "R2, sym1234", 0x0202, 0x1234);

    if (is99105() || is99110()) {
        TEST("BLSK", "R3, >4567",   0x00B3, 0x4567);
        TEST("BLSK", "R3, sym1234", 0x00B3, 0x1234);
    } else {
        UNKN(0x00B3);
    }
}

void test_cnt_reg() {
    TEST("SRA",  "R1, R0",  0x0801);
    TEST("SRL",  "R4, 12",  0x09C4);
    TEST("SLA",  "R8, 4",   0x0A48);
    TEST("SRC",  "R9, 15",  0x0BF9);
}

void test_src() {
    TEST("BLWP", "@>3876",  0x0420, 0x3876);
    NMEM("BLWP", "@0", "0", 0x0420);
    TEST("B",    "*R11",    0x045B); // RT
    TEST("B",    "*R12",    0x045C);
    TEST("B",    "R13",     0x044D);
    TEST("X",    "*R10",    0x049A);
    TEST("CLR",  "*R12+",   0x04FC);
    TEST("NEG",  "R0",      0x0500);
    TEST("INV",  "@>1234",  0x0560, 0x1234);
    TEST("INC",  "@2(R7)",          0x05A7, 0x0002);
    NMEM("INC",  "@0(R7)", "0(R7)", 0x05A7);
    TEST("INCT", "R7",     0x05C7);
    TEST("DEC",  "@>FFFE(R7)",0x0627, 0xFFFE);
    TEST("DECT", "R7",     0x0647);
    TEST("BL",   "R13",    0x068D);
    TEST("SWPB", "*R1",    0x06D1);
    TEST("SETO", "R12",    0x070C);
    TEST("ABS",  "@8(R3)", 0x0763, 0x0008);

    if (is9900()) {
        UNKN(0x0182);
        UNKN(0x0193);
        UNKN(0x01A0);
        UNKN(0x01A4);
        UNKN(0x01B5);
        UNKN(0x01C0);
        UNKN(0x01E8);
        UNKN(0x01FF);
    } else {
        // TMS9995, TMS99105, TMS99110
        TEST("DIVS", "R2",         0x0182);
        TEST("DIVS", "*R3",        0x0193);
        TEST("DIVS", "@>1234",     0x01A0, 0x1234);
        TEST("DIVS", "@>1000(R4)", 0x01A4, 0x1000);
        TEST("DIVS", "*R5+",       0x01B5);
        TEST("MPYS", "R0",         0x01C0);
        TEST("MPYS", "@2(R8)",     0x01E8, 0x0002);
        TEST("MPYS", "*R15+",      0x01FF);
    }

    if (is99105() || is99110()) {
        TEST("TMB",  "@>0123(R15), 7",          0x0C09, 0x01EF, 0x0123);
        NMEM("TMB",  "@0(R15), 7", "0(R15), 7", 0x0C09, 0x01EF);
        NMEM("TMB",  "",           "",          0x0C09);
        TEST("TCMB", "R0, 0",          0x0C0A, 0x0000);
        TEST("TSMB", "*R2, 15",        0x0C0B, 0x03D2);
        TEST("BIND", "@>2223(R1)",     0x0161, 0x2223);
        TEST("EVAD", "R5",             0x0105);
    } else {
        UNKN(0x0C09);
        UNKN(0x0C0A);
        UNKN(0x0C0B);
        UNKN(0x0161);
        UNKN(0x0105);
    }

    if (is99110()) {
        TEST("AR",  "R2",         0x0C42);
        TEST("CIR", "*R3",        0x0C93);
        TEST("SR",  "@>1234",     0x0CE0, 0x1234);
        TEST("MR",  "@>1234(R5)", 0x0D25, 0x1234);
        TEST("DR",  "*R6+",       0x0D76);
        TEST("LR",  "R2",         0x0D82);
        TEST("STR", "*R3",        0x0DD3);
    } else {
        UNKN(0x0C42);
        UNKN(0x0C93);
        UNKN(0x0CE0);
        UNKN(0x0D25);
        UNKN(0x0D76);
        UNKN(0x0D82);
        UNKN(0x0DD3);
    }

    symtab.intern(-2, "neg2");
    symtab.intern(0x1000, "sym1000");
    symtab.intern(0x1234, "sym1234");
    symtab.intern(0x3876, "sym3876");

    TEST("BLWP", "@sym3876",     0x0420, 0x3876);
    TEST("DEC",  "@neg2(R7)",    0x0627, 0xFFFE);

    if (is9995() || is99105() || is99110()) {
        TEST("DIVS", "@sym1234",     0x01A0, 0x1234);
        TEST("DIVS", "@sym1000(R4)", 0x01A4, 0x1000);
    }

    if (is99105() || is99110()) {
        TEST("BIND", "@sym1000(R1)", 0x0161, 0x1000);
    }
}

void test_reg_src() {
    TEST("COC", "R1, R2",         0x2081);
    TEST("CZC", "@>1234(R3), R7", 0x25E3, 0x1234);
    TEST("XOR", "@2(R5), R4",     0x2925, 0x0002);
    TEST("MPY", "R4, R2",         0x3884);
    TEST("DIV", "R14, R12",       0x3F0E);

    symtab.intern(0x1234, "sym1234");
    symtab.intern(0x0002, "offset2");

    TEST("CZC", "@sym1234(R3), R7", 0x25E3, 0x1234);
    TEST("XOR", "@offset2(R5), R4", 0x2925, 0x0002);
}

void test_cnt_src() {
    TEST("LDCR", "*R13+, 16",  0x303D);
    TEST("STCR", "@2(R4), 15", 0x37E4, 0x0002);

    if (is99105() || is99110()) {
        TEST("SRAM", "@2(R4), 15",              0x001C, 0x43E4, 0x0002);
        NMEM("SRAM", "@0(R4), 15", "0(R4), 15", 0x001C, 0x43E4);
        NMEM("SRAM", "",           "",          0x001C);
        TEST("SLAM", "R11, R0",  0x001D, 0x400B);
        TEST("SLAM", "*R13+, 1", 0x001D, 0x407D);
    }

    symtab.intern(7, "size7");
    symtab.intern(2, "offset2");

    TEST("STCR", "@offset2(R4), size7", 0x35E4, 0x0002);
    TEST("STCR", "@offset2(R4), 16",    0x3424, 0x0002);
    TEST("STCR", "@>1000(R4), size7",   0x35E4, 0x1000);

    if (is99105() || is99110()) {
        TEST("SRAM", "@offset2(R4), 15", 0x001C, 0x43E4, 0x0002);
        TEST("SLAM", "R11, R0",          0x001D, 0x400B);
        TEST("SLAM", "*R13+, 1",         0x001D, 0x407D);
    } else {
        UNKN(0x001C);
        UNKN(0x001D);
    }
}

void test_xop_src() {
    TEST("XOP",  "@>3876, 0",  0x2C20, 0x3876);
    TEST("XOP",  "@>3876, 15", 0x2FE0, 0x3876);

    symtab.intern(10, "xop10");
    symtab.intern(0x3876, "sym3876");
    symtab.intern(0x9876, "sym9876");

    TEST("XOP",  "@sym3876, xop10",   0x2EA0, 0x3876);
    TEST("XOP",  "@sym9876(R1), 8",   0x2E21, 0x9876);
    TEST("XOP",  "@>1234(R1), xop10", 0x2EA1, 0x1234);
}

void test_dst_src() {
    TEST("SZC",  "@>1234(R10), @>5678(R11)", 0x4AEA, 0x1234, 0x5678);
    TEST("SZCB", "@>1234, @>3456",           0x5820, 0x1234, 0x3456);
    TEST("S",    "*R10, *R11",               0x66DA);
    TEST("SB",   "*R10+, *R11+",             0x7EFA);
    TEST("C",    "*R10+, *R10+",             0x8EBA);
    TEST("CB",   "*R10+, *R11+",             0x9EFA);
    TEST("A",    "@>2000, R11",              0xA2E0, 0x2000);
    TEST("AB",   "R10, @>4000(R11)",         0xBACA, 0x4000);
    TEST("MOV",  "@0(R10), @1(R11)",         0xCAEA, 0x0000, 0x0001);
    TEST("MOVB", "R10, R11",                 0xD2CA);
    TEST("SOC",  "@>1234, @>5678(R11)",      0xEAE0, 0x1234, 0x5678);
    TEST("SOC",  "@>1234, @>5679(R11)",      0xEAE0, 0x1234, 0x5679);
    ERRT("SOC",  "@>1235, @>5678(R11)", OPERAND_NOT_ALIGNED, ">1235, @>5678(R11)", 0xEAE0, 0x1235, 0x5678);
    TEST("SOCB", "@>1234(R10), @>3456",      0xF82A, 0x1234, 0x3456);
    TEST("SOCB", "@>1235(R10), @>3456",      0xF82A, 0x1235, 0x3456);
    TEST("SOCB", "@>1234(R10), @>3457",      0xF82A, 0x1234, 0x3457);

    if (is9980()) {
        TEST("A",  "@>3FFE, R11",                               0xA2E0, 0x3FFE);
        ERRT("A",  "@>4000, R11", OVERFLOW_RANGE, ">4000, R11", 0xA2E0, 0x4000);
        TEST("AB", "R10, @>3FFF",                               0xB80A, 0x3FFF);
        ERRT("AB", "R10, @>4000", OVERFLOW_RANGE, ">4000",      0xB80A, 0x4000);
    }
    if (is99105() || is99110()) {
        TEST("SM", "@>1234(R10), @>5678(R11)",            0x0029, 0x4AEA, 0x1234, 0x5678);
        NMEM("SM", "@>1234(R10), @0(R11)",      "0(R11)", 0x0029, 0x4AEA, 0x1234);
        NMEM("SM", "@0(R10), @0(R11)", "0(R10), @0(R11)", 0x0029, 0x4AEA);
        NMEM("SM", "",                 "",                0x0029);
        TEST("SM", "@>1234, @>5678",      0x0029, 0x4820, 0x1234, 0x5678);
        TEST("SM", "R10, @>4000(R11)",    0x0029, 0x4ACA, 0x4000);
        TEST("AM", "@0(R10), @1(R11)",    0x002A, 0x4AEA, 0x0000, 0x0001);
        TEST("AM", "@>1234, @>5678(R11)", 0x002A, 0x4AE0, 0x1234, 0x5678);
        TEST("AM", "@>1234(R10), @>5678", 0x002A, 0x482A, 0x1234, 0x5678);
    } else {
        UNKN(0x0029);
        UNKN(0x002A);
    }

    if (is99110()) {
        TEST("CR", "@>1234(R10), @>5678(R11)",            0x0301, 0x0AEA, 0x1234, 0x5678);
        NMEM("CR", "@>1234(R10), @0(R11)",      "0(R11)", 0x0301, 0x0AEA, 0x1234);
        NMEM("CR", "@0(R10), @0(R11)", "0(R10), @0(R11)", 0x0301, 0x0AEA);
        NMEM("CR", "",                 "",                0x0301);
        TEST("CR", "@>1234, @>5678",      0x0301, 0x0820, 0x1234, 0x5678);
        TEST("CR", "R10, @>4000(R11)",    0x0301, 0x0ACA, 0x4000);
        TEST("MM", "@0(R10), @1(R11)",    0x0302, 0x0AEA, 0x0000, 0x0001);
        TEST("MM", "@>1234, @>5678(R11)", 0x0302, 0x0AE0, 0x1234, 0x5678);
        TEST("MM", "@>1234(R10), @>5678", 0x0302, 0x082A, 0x1234, 0x5678);
    } else {
        UNKN(0x0301);
        UNKN(0x0302);
    }

    symtab.intern(0x0000, "zero");
    symtab.intern(0x1234, "sym1234");
    symtab.intern(0x4000, "sym4000");
    symtab.intern(0x3456, "sym3456");
    symtab.intern(0x5678, "sym5678");

    TEST("SZC",  "@sym1234(R10), @sym5678(R11)", 0x4AEA, 0x1234, 0x5678);
    TEST("SZCB", "@sym1234, @sym3456",           0x5820, 0x1234, 0x3456);
    TEST("AB",   "R10, @sym4000(R11)",           0xBACA, 0x4000);
    TEST("MOV",  "@zero(R10), @1(R11)",          0xCAEA, 0x0000, 0x0001);
    TEST("SOC",  "@sym1234, @sym5678(R11)",      0xEAE0, 0x1234, 0x5678);
    TEST("SOCB", "@sym1234(R10), @sym3456",      0xF82A, 0x1234, 0x3456);

    if (is99105() || is99110()) {
        TEST("SM", "@sym1234(R10), @sym5678(R11)", 0x0029, 0x4AEA, 0x1234, 0x5678);
        TEST("SM", "@sym1234, @sym5678",           0x0029, 0x4820, 0x1234, 0x5678);
        TEST("SM", "R10, @sym4000(R11)",           0x0029, 0x4ACA, 0x4000);
        TEST("AM", "@zero(R10), @1(R11)",          0x002A, 0x4AEA, 0x0000, 0x0001);
        TEST("AM", "@sym1234, @sym5678(R11)",      0x002A, 0x4AE0, 0x1234, 0x5678);
        TEST("AM", "@sym1234(R10), @sym5678",      0x002A, 0x482A, 0x1234, 0x5678);
    }
}

void test_rel() {
    ATEST(0x1000, "JMP", ">1004", 0x1001);
    ATEST(0x1000, "JLT", ">1000", 0x11FF);
    ATEST(0x1000, "JLE", ">1100", 0x127F);
    ATEST(0x1000, "JEQ", ">0F02", 0x1380);
    ATEST(0x1000, "JHE", ">1004", 0x1401);
    ATEST(0x1000, "JGT", ">1006", 0x1502);
    ATEST(0x1000, "JNE", ">1008", 0x1603);
    ATEST(0x1000, "JNC", ">100A", 0x1704);
    ATEST(0x1000, "JOC", ">0FFE", 0x18FE);
    ATEST(0x1000, "JNO", ">0FFC", 0x19FD);
    ATEST(0x1000, "JL",  ">0FFA", 0x1AFC);
    ATEST(0x1000, "JH",  ">0FF8", 0x1BFB);
    ATEST(0x1000, "JOP", ">0FF6", 0x1CFA);

    disassembler.setOption("relative", "true");
    ATEST(0x1000, "JMP", "$-254", 0x1080);
    ATEST(0x1000, "JMP", "$",     0x10FF);
    ATEST(0x1000, "JMP", "$+256", 0x107F);
    ATEST(0x1000, "JMP", "$+2",   0x1000); // NOP

    symtab.intern(0x0F02, "sym0F02");
    symtab.intern(0x1000, "sym1000");
    symtab.intern(0x1004, "sym1004");
    symtab.intern(0x1100, "sym1100");

    ATEST(0x1000, "JMP", "sym1004", 0x1001);
    ATEST(0x1000, "JLT", "sym1000", 0x11FF);
    ATEST(0x1000, "JLE", "sym1100", 0x127F);
    ATEST(0x1000, "JEQ", "sym0F02", 0x1380);
}

void test_cru_off() {
    TEST("SBO", "0",    0x1D00);
    TEST("SBZ", "127",  0x1E7F);
    TEST("TB",  "-128", 0x1F80);

    symtab.intern(0x00, "zero");
    symtab.intern(127, "off127");
    symtab.intern(-128, "off_128");

    TEST("SBO", "zero",    0x1D00);
    TEST("SBZ", "off127",  0x1E7F);
    TEST("TB",  "off_128", 0x1F80);
}

struct illegal_range {
    Config::opcode_t start;
    Config::opcode_t end;
};

// 2 words instructions
constexpr Config::opcode_t SRAM = 0x001C;
constexpr Config::opcode_t SLAM = 0x001D;
constexpr Config::opcode_t SM   = 0x0029;
constexpr Config::opcode_t AM   = 0x002A;
constexpr Config::opcode_t TMB  = 0x0C09;
constexpr Config::opcode_t TCMB = 0x0C0A;
constexpr Config::opcode_t TSMB = 0x0C0B;

void test_illegal_tms9900() {
    static constexpr illegal_range mids[] = {
        { 0x0000, 0x01ff },
        { 0x0210, 0x021f }, { 0x0230, 0x023f }, { 0x0250, 0x025f }, { 0x0270, 0x027f },
        { 0x0290, 0x029f }, { 0x02b0, 0x02bf }, { 0x02d0, 0x02df }, { 0x02e1, 0x02ff },
        { 0x0301, 0x033f }, { 0x0341, 0x035f }, { 0x0361, 0x037f }, { 0x0381, 0x039f },
        { 0x03a1, 0x03bf }, { 0x03c1, 0x03df }, { 0x03e1, 0x03ff },
        { 0x0780, 0x07ff }, { 0x0c00, 0x0fff }
    };
    for (const auto &r : mids) {
        for (auto opc = r.start; opc <= r.end; opc++) {
            UNKN(opc);
        }
    }
}

void test_illegal_tms9995() {
    static constexpr illegal_range mids[] = {
        { 0x0000, 0x007f }, { 0x00A0, 0x017f },
        { 0x0210, 0x021f }, { 0x0230, 0x023f }, { 0x0250, 0x025f }, { 0x0270, 0x027f },
        { 0x0290, 0x029f }, { 0x02b0, 0x02bf }, { 0x02d0, 0x02df }, { 0x02e1, 0x02ff },
        { 0x0301, 0x033f }, { 0x0341, 0x035f }, { 0x0361, 0x037f }, { 0x0381, 0x039f },
        { 0x03a1, 0x03bf }, { 0x03c1, 0x03df }, { 0x03e1, 0x03ff },
        { 0x0780, 0x07ff }, { 0x0c00, 0x0fff }
    };
    for (const auto &r : mids) {
        for (auto opc = r.start; opc <= r.end; opc++) {
            UNKN(opc);
        }
    }
}

void test_illegal_tms99105() {
    static constexpr illegal_range mids[] = {
        { 0x0000, 0x001b }, { 0x001e, 0x0028 }, { 0x002b, 0x007f }, { 0x00a0, 0x00af },
        { 0x00c0, 0x00ff },
        { 0x0210, 0x021f }, { 0x0230, 0x023f }, { 0x0250, 0x025f }, { 0x0270, 0x027f },
        { 0x0290, 0x029f }, { 0x02b0, 0x02bf }, { 0x02d0, 0x02df }, { 0x02e1, 0x02ff },
        { 0x0301, 0x033f }, { 0x0341, 0x035f }, { 0x0361, 0x037f }, { 0x0383, 0x0383 },
        { 0x0385, 0x039f }, { 0x03a1, 0x03bf }, { 0x03c1, 0x03df }, { 0x03e1, 0x03ff },
        { 0x0780, 0x07ff }, { 0x0c00, 0x0c08 }, { 0x0c0c, 0x0fff }
    };
    for (const auto &r : mids) {
        for (auto opc = r.start; opc <= r.end; opc++) {
            UNKN(opc);
        }
    }

    for (auto post = 0; post < 0x10000; post++) {
        const Config::opcode_t opc = post;
        const auto hi4 = (post >> 12);
        if (hi4 != 4) {
            ERRT("SM", "", UNKNOWN_POSTBYTE, "", SM, opc);
            ERRT("AM", "", UNKNOWN_POSTBYTE, "", AM, opc);
        }
        const auto hi6 = (post >> 10);
        if (hi6 != 0x10) {
            ERRT("SRAM", "", UNKNOWN_POSTBYTE, "", SRAM, opc);
            ERRT("SLAM", "", UNKNOWN_POSTBYTE, "", SLAM, opc);
        }
        const auto ts = (post >> 4) & 3;
        if (hi6 || ts == 3) {
            ERRT("TMB",  "", UNKNOWN_POSTBYTE, "", TMB,  opc);
            ERRT("TCMB", "", UNKNOWN_POSTBYTE, "", TCMB, opc);
            ERRT("TSMB", "", UNKNOWN_POSTBYTE, "", TSMB, opc);
        }
    }
}

void test_illegal_tms99110() {
    static constexpr illegal_range mids[] = {
        { 0x0000, 0x001b }, { 0x001e, 0x0028 }, { 0x002b, 0x007f }, { 0x00a0, 0x00af },
        { 0x00c0, 0x00ff },
        { 0x0210, 0x021f }, { 0x0230, 0x023f }, { 0x0250, 0x025f }, { 0x0270, 0x027f },
        { 0x0290, 0x029f }, { 0x02b0, 0x02bf }, { 0x02d0, 0x02df }, { 0x02e1, 0x02ff },
        { 0x0303, 0x033f }, { 0x0341, 0x035f }, { 0x0361, 0x037f }, { 0x0383, 0x0383 },
        { 0x0385, 0x039f }, { 0x03a1, 0x03bf }, { 0x03c1, 0x03df }, { 0x03e1, 0x03ff },
        { 0x0781, 0x07bf }, { 0x07c1, 0x07ff }, { 0x0c01, 0x0c01 }, { 0x0c03, 0x0c03 },
        { 0x0c05, 0x0c05 }, { 0x0c07, 0x0c08 }, { 0x0e00, 0x0fff },
   };
    for (const auto &r : mids) {
        for (auto opc = r.start; opc <= r.end; opc++) {
            UNKN(opc);
        }
    }

    for (auto post = 0; post < 0x10000; post++) {
        const Config::opcode_t opc = post;
        const auto hi4 = (post >> 12);
        if (hi4 != 4) {
            ERRT("SM", "", UNKNOWN_POSTBYTE, "", SM, opc);
            ERRT("AM", "", UNKNOWN_POSTBYTE, "", AM, opc);
        }
        const auto hi6 = (post >> 10);
        if (hi6 != 0x10) {
            ERRT("SRAM", "", UNKNOWN_POSTBYTE, "", SRAM, opc);
            ERRT("SLAM", "", UNKNOWN_POSTBYTE, "", SLAM, opc);
        }
        const auto ts = (post >> 4) & 3;
        if (hi6 || ts == 3) {
            ERRT("TMB",  "", UNKNOWN_POSTBYTE, "", TMB,  opc);
            ERRT("TCMB", "", UNKNOWN_POSTBYTE, "", TCMB, opc);
            ERRT("TSMB", "", UNKNOWN_POSTBYTE, "", TSMB, opc);
        }
    }
}
// clang-format on

void run_tests(const char *cpu) {
    disassembler.setCpu(cpu);
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
    if (is9900())
        RUN_TEST(test_illegal_tms9900);
    if (is9995())
        RUN_TEST(test_illegal_tms9995);
    if (is99105())
        RUN_TEST(test_illegal_tms99105);
    if (is99110())
        RUN_TEST(test_illegal_tms99110);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

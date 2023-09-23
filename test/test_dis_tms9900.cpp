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

static bool is9900() {
    return strcmp_P("9900", disassembler.cpu_P()) == 0 ||
           strcmp_P("9980", disassembler.cpu_P()) == 0;
}

static bool is9995() {
    return strcmp_P("9995", disassembler.cpu_P()) == 0;
}

static bool is99105() {
    return strcmp_P("99105", disassembler.cpu_P()) == 0;
}

static void set_up() {
    disassembler.reset();
    disassembler.setOption("relative", "false");
}

static void tear_down() {
    symtab.reset();
}

// clang-format off
void test_cpu() {
    EQUALS("cpu 9900", true,   disassembler.setCpu("9900"));
    EQUALS_P("cpu 9900", "9900", disassembler.cpu_P());

    EQUALS("cpu 9980", true,   disassembler.setCpu("9980"));
    EQUALS_P("cpu 9980", "9980", disassembler.cpu_P());

    EQUALS("cpu 9995", true,   disassembler.setCpu("9995"));
    EQUALS_P("cpu 9995", "9995", disassembler.cpu_P());

    EQUALS("cpu 99105", true,    disassembler.setCpu("99105"));
    EQUALS_P("cpu 99105", "99105", disassembler.cpu_P());

    EQUALS("cpu TMS9900", true,   disassembler.setCpu("TMS9900"));
    EQUALS_P("cpu TMS9900", "9900", disassembler.cpu_P());

    EQUALS("cpu TMS9980", true,   disassembler.setCpu("TMS9980"));
    EQUALS_P("cpu TMS9980", "9980", disassembler.cpu_P());

    EQUALS("cpu TMS9995", true,   disassembler.setCpu("TMS9995"));
    EQUALS_P("cpu TMS9995", "9995", disassembler.cpu_P());

    EQUALS("cpu TMS99105", true,    disassembler.setCpu("TMS99105"));
    EQUALS_P("cpu TMS99105", "99105", disassembler.cpu_P());
}

static void test_inh() {
    TEST(IDLE, "", 0x0340);
    TEST(RSET, "", 0x0360);
    TEST(RTWP, "", 0x0380);
    TEST(CKON, "", 0x03A0);
    TEST(CKOF, "", 0x03C0);
    TEST(LREX, "", 0x03E0);

    if (is99105()) {
        // TMS99105
        TEST(RTWP, "1", 0x0381);
        TEST(RTWP, "2", 0x0382);
        TEST(RTWP, "4", 0x0384);
    } else {
        ERUI(RTWP, "1", 0x0381);
        ERUI(RTWP, "2", 0x0382);
        ERUI(RTWP, "4", 0x0384);
    }
}

static void test_imm() {
    TEST(LWPI, ">1234", 0x02E0, 0x1234);
    TEST(LIMI, ">89AB", 0x0300, 0x89AB);

    symtab.intern(0x1234, "sym1234");
    symtab.intern(0x89AB, "sym89AB");

    TEST(LWPI, "sym1234", 0x02E0, 0x1234);
    TEST(LIMI, "sym89AB", 0x0300, 0x89AB);
}

static void test_reg() {
    TEST(STWP, "R14", 0x02AE);
    TEST(STST, "R15", 0x02CF);

    if (is9900()) {
        ERUI(LST, "R0", 0x0080);
        ERUI(LWP, "R1", 0x0091);
    } else {
        // TMS9995
        TEST(LST, "R0", 0x0080);
        TEST(LWP, "R1", 0x0091);
    }
}

static void test_reg_imm() {
    TEST(LI,   "R0, 0",      0x0200, 0x0000);
    TEST(AI,   "R1, 10",     0x0221, 0x000A);
    TEST(ANDI, "R8, >00FF",   0x0248, 0x00FF);
    TEST(ORI,  "R14, >FF00", 0x026E, 0xFF00);
    TEST(CI,   "R15, >FFFF", 0x028F, 0xFFFF);

    symtab.intern(0x1234, "sym1234");

    TEST(LI,   "R2, sym1234", 0x0202, 0x1234);

    if (is99105()) {
        // TMS99105
        TEST(BLSK, "R3, >4567",   0x00B3, 0x4567);
        TEST(BLSK, "R3, sym1234", 0x00B3, 0x1234);
    } else {
        ERRI(0x00B3);
    }
}

static void test_cnt_reg() {
    TEST(SRA,  "R1, R0",  0x0801);
    TEST(SRL,  "R4, 12",  0x09C4);
    TEST(SLA,  "R8, 4",   0x0A48);
    TEST(SRC,  "R9, 15",  0x0BF9);
}

static void test_src() {
    TEST(BLWP, "@>9876", 0x0420, 0x9876);
    TEST(B,    "R13",    0x044D);
    TEST(X,    "*R10",   0x049A);
    TEST(CLR,  "*R12+",  0x04FC);
    TEST(NEG,  "R0",     0x0500);
    TEST(INV,  "@>1234", 0x0560, 0x1234);
    TEST(INC,  "@2(R7)", 0x05A7, 0x0002);
    TEST(INCT, "R7",     0x05C7);
    TEST(DEC,  "@>FFFE(R7)",0x0627, 0xFFFE);
    TEST(DECT, "R7",     0x0647);
    TEST(BL,   "R13",    0x068D);
    TEST(SWPB, "*R1",    0x06D1);
    TEST(SETO, "R12",    0x070C);
    TEST(ABS,  "@8(R3)", 0x0763, 0x0008);

    if (is9900()) {
        ERUI(DIVS, "R2",     0x0182);
        ERUI(DIVS, "*R3",    0x0193);
        ERUI(DIVS, "@>1234", 0x01A0, 0x1234);
        ERUI(DIVS, "@>1000(R4)", 0x01A4, 0x1000);
        ERUI(DIVS, "*R5+",   0x01B5);
        ERUI(MPYS, "R0",     0x01C0);
        ERUI(MPYS, "@2(R8)", 0x01E8, 0x0002);
        ERUI(MPYS, "*R15+",  0x01FF);
    } else {
        // TMS9995
        TEST(DIVS, "R2",     0x0182);
        TEST(DIVS, "*R3",    0x0193);
        TEST(DIVS, "@>1234", 0x01A0, 0x1234);
        TEST(DIVS, "@>1000(R4)", 0x01A4, 0x1000);
        TEST(DIVS, "*R5+",   0x01B5);
        TEST(MPYS, "R0",     0x01C0);
        TEST(MPYS, "@2(R8)", 0x01E8, 0x0002);
        TEST(MPYS, "*R15+",  0x01FF);
    }

    if (is99105()) {
        // TMS99105
        TEST(TMB,  "@>0123(R15), 7", 0x0C09, 0x01EF, 0x0123);
        TEST(TCMB, "R0, 0",          0x0C0A, 0x0000);
        TEST(TSMB, "*R2, 15",        0x0C0B, 0x03D2);
        TEST(BIND, "@>2223(R1)", 0x0161, 0x2223);
        TEST(EVAD, "R5",         0x0105);
    } else {
        ERUI(TMB,  "@>0123(R15), 7", 0x0C09, 0x01EF, 0x0123);
        ERUI(TCMB, "R0, 0",          0x0C0A, 0x0000);
        ERUI(TSMB, "*R2, 15",        0x0C0B, 0x03D2);
        ERUI(BIND, "@>2223(R1)", 0x0161, 0x2223);
        ERUI(EVAD, "R5",         0x0105);
    }

    symtab.intern(-2, "neg2");
    symtab.intern(0x1000, "sym1000");
    symtab.intern(0x1234, "sym1234");
    symtab.intern(0x9876, "sym9876");

    TEST(BLWP, "@sym9876",     0x0420, 0x9876);
    TEST(DEC,  "@neg2(R7)",    0x0627, 0xFFFE);

    if (is9995() || is99105()) {
        // TMS9995
        TEST(DIVS, "@sym1234",     0x01A0, 0x1234);
        TEST(DIVS, "@sym1000(R4)", 0x01A4, 0x1000);
    }

    if (is99105()) {
        // TMS99105
        TEST(BIND, "@sym1000(R1)", 0x0161, 0x1000);
    }
}

static void test_reg_src() {
    TEST(COC, "R1, R2",         0x2081);
    TEST(CZC, "@>1234(R3), R7", 0x25E3, 0x1234);
    TEST(XOR, "@2(R5), R4", 0x2925, 0x0002);
    TEST(MPY, "R4, R2",         0x3884);
    TEST(DIV, "R14, R12",       0x3F0E);

    symtab.intern(0x1234, "sym1234");
    symtab.intern(0x0002, "offset2");

    TEST(CZC, "@sym1234(R3), R7", 0x25E3, 0x1234);
    TEST(XOR, "@offset2(R5), R4", 0x2925, 0x0002);
}

static void test_cnt_src() {
    TEST(LDCR, "*R13+, 16",  0x303D);
    TEST(STCR, "@2(R4), 15", 0x37E4, 0x0002);

    symtab.intern(7, "size7");
    symtab.intern(2, "offset2");

    TEST(STCR, "@offset2(R4), size7", 0x35E4, 0x0002);
    TEST(STCR, "@offset2(R4), 16",    0x3424, 0x0002);
    TEST(STCR, "@>1000(R4), size7",   0x35E4, 0x1000);

    if (is99105()) {
        // TMS99105
        TEST(SRAM, "@offset2(R4), 15", 0x001C, 0x43E4, 0x0002);
        TEST(SLAM, "R11, R0",          0x001D, 0x400B);
        TEST(SLAM, "*R13+, 1",         0x001D, 0x407D);
    } else {
        ERUI(SRAM, "", 0x001C);
        ERUI(SLAM, "", 0x001D);
    }
}

static void test_xop_src() {
    TEST(XOP,  "@>9876, 0",  0x2C20, 0x9876);
    TEST(XOP,  "@>9876, 15", 0x2FE0, 0x9876);

    symtab.intern(10, "xop10");
    symtab.intern(0x9876, "sym9876");

    TEST(XOP,  "@sym9876, xop10",   0x2EA0, 0x9876);
    TEST(XOP,  "@sym9876(R1), 8",   0x2E21, 0x9876);
    TEST(XOP,  "@>1234(R1), xop10", 0x2EA1, 0x1234);
}

static void test_dst_src() {
    TEST(SZC,  "@>1234(R10), @>5678(R11)", 0x4AEA, 0x1234, 0x5678);
    TEST(SZCB, "@>1234, @>5678",           0x5820, 0x1234, 0x5678);
    TEST(S,    "*R10, *R11",               0x66DA);
    TEST(SB,   "*R10+, *R11+",             0x7EFA);
    TEST(C,    "*R10+, *R10+",             0x8EBA);
    TEST(CB,   "*R10+, *R11+",             0x9EFA);
    TEST(A,    "@>2000, R11",              0xA2E0, 0x2000);
    TEST(AB,   "R10, @>4000(R11)",         0xBACA, 0x4000);
    TEST(MOV,  "@0(R10), @1(R11)",         0xCAEA, 0x0000, 0x0001);
    TEST(MOVB, "R10, R11",                 0xD2CA);
    TEST(SOC,  "@>1234, @>5678(R11)",      0xEAE0, 0x1234, 0x5678);
    TEST(SOCB, "@>1234(R10), @>5678",      0xF82A, 0x1234, 0x5678);

    symtab.intern(0x0000, "zero");
    symtab.intern(0x1234, "sym1234");
    symtab.intern(0x4000, "sym4000");
    symtab.intern(0x5678, "sym5678");

    TEST(SZC,  "@sym1234(R10), @sym5678(R11)", 0x4AEA, 0x1234, 0x5678);
    TEST(SZCB, "@sym1234, @sym5678",           0x5820, 0x1234, 0x5678);
    TEST(AB,   "R10, @sym4000(R11)",           0xBACA, 0x4000);
    TEST(MOV,  "@zero(R10), @1(R11)",          0xCAEA, 0x0000, 0x0001);
    TEST(SOC,  "@sym1234, @sym5678(R11)",      0xEAE0, 0x1234, 0x5678);
    TEST(SOCB, "@sym1234(R10), @sym5678",      0xF82A, 0x1234, 0x5678);

    if (is99105()) {
        // TMS99105
        TEST(SM, "@sym1234(R10), @sym5678(R11)", 0x0029, 0x4AEA, 0x1234, 0x5678);
        TEST(SM, "@sym1234, @sym5678",           0x0029, 0x4820, 0x1234, 0x5678);
        TEST(SM, "R10, @sym4000(R11)",           0x0029, 0x4ACA, 0x4000);
        TEST(AM, "@zero(R10), @1(R11)",          0x002A, 0x4AEA, 0x0000, 0x0001);
        TEST(AM, "@sym1234, @sym5678(R11)",      0x002A, 0x4AE0, 0x1234, 0x5678);
        TEST(AM, "@sym1234(R10), @sym5678",      0x002A, 0x482A, 0x1234, 0x5678);
    } else {
        ERUI(SM, "", 0x0029);
        ERUI(AM, "", 0x002A);
    }
}

static void test_rel() {
    ATEST(0x1000, JMP, ">1002", 0x1000);
    ATEST(0x1000, JLT, ">1000", 0x11FF);
    ATEST(0x1000, JLE, ">1100", 0x127F);
    ATEST(0x1000, JEQ, ">0F02", 0x1380);
    ATEST(0x1000, JHE, ">1004", 0x1401);
    ATEST(0x1000, JGT, ">1006", 0x1502);
    ATEST(0x1000, JNE, ">1008", 0x1603);
    ATEST(0x1000, JNC, ">100A", 0x1704);
    ATEST(0x1000, JOC, ">0FFE", 0x18FE);
    ATEST(0x1000, JNO, ">0FFC", 0x19FD);
    ATEST(0x1000, JL,  ">0FFA", 0x1AFC);
    ATEST(0x1000, JH,  ">0FF8", 0x1BFB);
    ATEST(0x1000, JOP, ">0FF6", 0x1CFA);

    disassembler.setOption("relative", "true");
    ATEST(0x1000, JMP, "$-254", 0x1080);
    ATEST(0x1000, JMP, "$",     0x10FF);
    ATEST(0x1000, JMP, "$+2",   0x1000);
    ATEST(0x1000, JMP, "$+256", 0x107F);

    symtab.intern(0x0F02, "sym0F02");
    symtab.intern(0x1000, "sym1000");
    symtab.intern(0x1002, "sym1002");
    symtab.intern(0x1100, "sym1100");

    ATEST(0x1000, JMP, "sym1002", 0x1000);
    ATEST(0x1000, JLT, "sym1000", 0x11FF);
    ATEST(0x1000, JLE, "sym1100", 0x127F);
    ATEST(0x1000, JEQ, "sym0F02", 0x1380);
}

static void test_cru_off() {
    TEST(SBO, "0",    0x1D00);
    TEST(SBZ, "127",  0x1E7F);
    TEST(TB,  "-128", 0x1F80);

    symtab.intern(0x00, "zero");
    symtab.intern(127, "off127");
    symtab.intern(-128, "off_128");

    TEST(SBO, "zero",    0x1D00);
    TEST(SBZ, "off127",  0x1E7F);
    TEST(TB,  "off_128", 0x1F80);
}

struct mid_range {
    uint16_t start;
    uint16_t end;
};
struct mid_hole {
    uint16_t mask;
    uint16_t start;
    uint16_t end;
    bool contains(uint16_t opc) const {
        opc &= mask;
        return opc >= start && opc <= end;
    }
};

// 2 words instructions
static constexpr uint16_t SRAM = 0x001C;
static constexpr uint16_t SLAM = 0x001D;
static constexpr uint16_t SM   = 0x0029;
static constexpr uint16_t AM   = 0x002A;
static constexpr uint16_t TMB  = 0x0C09;
static constexpr uint16_t TCMB = 0x0C0A;
static constexpr uint16_t TSMB = 0x0C0B;
static uint16_t validPostWord(uint16_t opCode) {
    switch (opCode) {
    case SRAM: case SLAM: case SM: case AM:
        return 0x4000;
    default:
        return 0x0000;
    }
}

// Macro Instruction Detect
static void assert_mid(
    const mid_range *ranges, const mid_range *end,
    const uint16_t prefix = 0, const mid_hole *hole = nullptr) {
    uint16_t words[4] = { 0, 0, 0, 0 };
    Insn insn(0x1000);
    char operands[40], message[40];
    const mid_range *m = ranges;
    if (prefix) words[0] = prefix;
    const uint8_t pos = prefix ? 1 : 0;

    disassembler.setOption("uppercase", "yes");
    for (uint16_t hi = 0; hi < 0x100; hi++) {
        for (uint16_t lo = 0; lo < 0x100; lo++) {
            const uint16_t code = (hi << 8) | lo;
            const uint16_t post = validPostWord(prefix ? prefix : code);
            words[pos] = code;
            words[pos + 1] = post;
            const ArrayMemory memory(0x1000, words, sizeof(words), disassembler.config().endian());
            auto it = memory.iterator();

            insn.reset(insn.address());
            disassembler.decode(it, insn, operands, sizeof(operands));
            if (m && code > m->end) {
                if (++m >= end)
                    m = nullptr;
            }
            if (m && ((code >= m->start && code <= m->end) || (hole && hole->contains(code)))) {
                if (prefix) {
                    sprintf(message, "%04X,%04X is MID", prefix, code);
                } else {
                    sprintf(message, "%04X is MID", code);
                }
                EQUALS(message, UNKNOWN_INSTRUCTION, disassembler.getError());
                EQUALS(message, "MID", insn.name());
            } else {
                if (prefix) {
                    sprintf(message, "%04X,%04X is not MID", prefix, code);
                } else {
                    sprintf(message, "%04X is not MID", code);
                }
                EQUALS(message, OK, disassembler.getError());
                NOT_EQUALS(message, "MID", insn.name());
            }
        }
    }
}

static void test_mid_tms9900() {
    static const mid_range mids[] = {
        { 0x0000, 0x01ff },
        { 0x0210, 0x021f }, { 0x0230, 0x023f }, { 0x0250, 0x025f }, { 0x0270, 0x027f },
        { 0x0290, 0x029f }, { 0x02b0, 0x02bf }, { 0x02d0, 0x02df }, { 0x02e1, 0x02ff },
        { 0x0301, 0x033f }, { 0x0341, 0x035f }, { 0x0361, 0x037f }, { 0x0381, 0x039f },
        { 0x03a1, 0x03bf }, { 0x03c1, 0x03df }, { 0x03e1, 0x03ff },
        { 0x0780, 0x07ff }, { 0x0c00, 0x0fff }
    };

    assert_mid(mids, std::end(mids));
}

static void test_mid_tms9995() {
    static const mid_range mids[] = {
        { 0x0000, 0x007f }, { 0x00A0, 0x017f },
        { 0x0210, 0x021f }, { 0x0230, 0x023f }, { 0x0250, 0x025f }, { 0x0270, 0x027f },
        { 0x0290, 0x029f }, { 0x02b0, 0x02bf }, { 0x02d0, 0x02df }, { 0x02e1, 0x02ff },
        { 0x0301, 0x033f }, { 0x0341, 0x035f }, { 0x0361, 0x037f }, { 0x0381, 0x039f },
        { 0x03a1, 0x03bf }, { 0x03c1, 0x03df }, { 0x03e1, 0x03ff },
        { 0x0780, 0x07ff }, { 0x0c00, 0x0fff }
    };

    assert_mid(mids, std::end(mids));
}

static void test_mid_tms99105() {
    static const mid_range mids[] = {
        { 0x0000, 0x001B }, { 0x001E, 0x0028 }, { 0x002B, 0x007f }, { 0x00A0, 0x00AF },
        { 0x00C0, 0x00FF },
        { 0x0210, 0x021f }, { 0x0230, 0x023f }, { 0x0250, 0x025f }, { 0x0270, 0x027f },
        { 0x0290, 0x029f }, { 0x02b0, 0x02bf }, { 0x02d0, 0x02df }, { 0x02e1, 0x02ff },
        { 0x0301, 0x033f }, { 0x0341, 0x035f }, { 0x0361, 0x037f }, { 0x0383, 0x0383 },
        { 0x0385, 0x039f }, { 0x03a1, 0x03bf }, { 0x03c1, 0x03df }, { 0x03e1, 0x03ff },
        { 0x0780, 0x07ff }, { 0x0c00, 0x0c08 }, { 0x0c0c, 0x0fff }
    };

    assert_mid(mids, std::end(mids));

    static const mid_range dp_arith_2nd[] = {
        { 0x0000, 0x3fff}, {0x5000, 0xffff }
    };
    assert_mid(dp_arith_2nd, std::end(dp_arith_2nd), SM);
    assert_mid(dp_arith_2nd, std::end(dp_arith_2nd), AM);

    static const mid_range dp_shift_2nd[] = {
        { 0x0000, 0x3fff}, { 0x4400, 0xffff }
    };
    assert_mid(dp_shift_2nd, std::end(dp_shift_2nd), SRAM);
    assert_mid(dp_shift_2nd, std::end(dp_shift_2nd), SLAM);

    static const mid_range dp_bit_2nd[] = {
        { 0x0400, 0xffff },
    };
    static const mid_hole dp_bit_hole = { 0x003F, 0x0030, 0x003F };
    assert_mid(dp_bit_2nd, std::end(dp_bit_2nd), TMB,  &dp_bit_hole);
    assert_mid(dp_bit_2nd, std::end(dp_bit_2nd), TCMB, &dp_bit_hole);
    assert_mid(dp_bit_2nd, std::end(dp_bit_2nd), TSMB, &dp_bit_hole);
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
        RUN_TEST(test_mid_tms9900);
    if (is9995())
        RUN_TEST(test_mid_tms9995);
    if (is99105())
        RUN_TEST(test_mid_tms99105);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

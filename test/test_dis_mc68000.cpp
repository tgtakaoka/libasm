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

#include "dis_mc68000.h"
#include "test_dis_helper.h"

using namespace libasm;
using namespace libasm::mc68000;
using namespace libasm::test;

DisMc68000 dis68000;
Disassembler &disassembler(dis68000);

bool mc68k00() {
    return strcmp_P("68000", disassembler.config().cpu_P()) == 0;
}

bool mc68010() {
    return strcmp_P("68010", disassembler.config().cpu_P()) == 0;
}

bool mc68020() {
    return strcmp_P("68020", disassembler.config().cpu_P()) == 0;
}

bool mc68030() {
    return strcmp_P("68030", disassembler.config().cpu_P()) == 0;
}

bool firstGen() {
    return mc68k00() || mc68010();
}

/**
 * Test helper for 32/24-bit address space.
 * Substitute |mark| in |text| with |abs| into |line|.
 */
void subst(char *line, const char *text, const char *mark, const char *abs, bool firstGen,
        bool gnuAs = false) {
    const auto mlen = strlen(mark);
    auto addr = abs;
    while (*text) {
        if (strncmp(text, mark, mlen) == 0) {
            text += 2;
            if (gnuAs) {
                *line++ = '0';
                *line++ = 'x';
            } else {
                *line++ = '$';
            }
            auto next = strchr(addr, ':');
            if (next == nullptr)
                next = addr + strlen(addr);
            const auto len = next - addr;
            // Sign extend address to 32/24-bit.
            const auto pre = (*addr >= '8' && len == 4) ? 'F' : '0';
            for (auto n = (firstGen ? 6 : 8) - len; n > 0; n--)
                *line++ = pre;
            while (addr < next)
                *line++ = *addr++;
            // If |abs| contains ':', use each text for each |mark|.
            addr = (*addr == ':') ? next + 1 : abs;
        } else {
            *line++ = *text++;
        }
    }
    *line = 0;
}

#define ABST(name, opr_, abs, ...)               \
    do {                                         \
        char opr[128];                           \
        subst(opr, opr_, "@A", abs, firstGen()); \
        TEST(name, opr, __VA_ARGS__);            \
    } while (0)

#define EABS(name, opr_, abs, error, errat_, ...)    \
    do {                                             \
        char opr[128];                               \
        char errat[128];                             \
        subst(opr, opr_, "@A", abs, firstGen());     \
        subst(errat, errat_, "@A", abs, firstGen()); \
        ERRT(name, opr, error, errat, __VA_ARGS__);  \
    } while (0)

#define GRELT(name, opr_, abs, ...)                    \
    do {                                               \
        char opr[128];                                 \
        subst(opr, opr_, "@R", abs, firstGen(), true); \
        TEST(name, opr, __VA_ARGS__);                  \
    } while (0)

#define ARELT(addr, name, opr_, abs, ...)        \
    do {                                         \
        char opr[128];                           \
        subst(opr, opr_, "@R", abs, firstGen()); \
        ATEST(addr, name, opr, __VA_ARGS__);     \
    } while (0)

#define ERELT(addr, name, opr_, abs, error, errat_, ...)   \
    do {                                                   \
        char opr[128];                                     \
        char errat[128];                                   \
        subst(opr, opr_, "@R", abs, firstGen());           \
        subst(errat, errat_, "@R", abs, firstGen());       \
        AERRT(addr, name, opr, error, errat, __VA_ARGS__); \
    } while (0)

#if defined(LIBASM_DIS_NOFLOAT)
#define TFLT(insn, opr, hex, ...) ERRT(insn, hex, FLOAT_NOT_SUPPORTED, hex, __VA_ARGS__)
#define EFLT(insn, opr, error, at, hex, ...) ERRT(insn, hex, FLOAT_NOT_SUPPORTED, hex, __VA_ARGS__)
#else
#define TFLT(insn, opr, hex, ...) TEST(insn, opr, __VA_ARGS__)
#define EFLT(insn, opr, error, at, hex, ...) ERRT(insn, opr, error, at, __VA_ARGS__)
#endif

void set_up() {
    disassembler.reset();
    disassembler.setOption("relative", "true");
}

void tear_down() {
    symtab.reset();
}

// clang-format off
void test_cpu() {
    EQUALS("cpu 68000", true,    disassembler.setCpu("68000"));
    EQUALS_P("cpu 68000", "68000", disassembler.config().cpu_P());

    EQUALS("cpu MC68000", true,    disassembler.setCpu("MC68000"));
    EQUALS_P("cpu MC68000", "68000", disassembler.config().cpu_P());

    EQUALS("cpu 68010", true,    disassembler.setCpu("68010"));
    EQUALS_P("cpu 68010", "68010", disassembler.config().cpu_P());

    EQUALS("cpu MC68010", true,    disassembler.setCpu("MC68010"));
    EQUALS_P("cpu MC68010", "68010", disassembler.config().cpu_P());

    EQUALS("cpu 68020", true,    disassembler.setCpu("68020"));
    EQUALS_P("cpu 68020", "68020", disassembler.config().cpu_P());

    EQUALS("cpu MC68020", true,    disassembler.setCpu("MC68020"));
    EQUALS_P("cpu MC68020", "68020", disassembler.config().cpu_P());

    EQUALS("cpu 68030", true,    disassembler.setCpu("68030"));
    EQUALS_P("cpu 68030", "68030", disassembler.config().cpu_P());

    EQUALS("cpu MC68030", true,    disassembler.setCpu("MC68030"));
    EQUALS_P("cpu MC68030", "68030", disassembler.config().cpu_P());
}

void test_data_move() {
    // EXG Rx,Ry
    TEST("EXG", "D1, D2", 0141502); // 014|Dx|50|Dy
    TEST("EXG", "A1, A2", 0141512); // 014|Ax|51|Ay
    TEST("EXG", "D1, A2", 0141612); // 014|Dx|61|Ay

    // LEA ea,Ad: 004|An|7|M|Rn
    UNKN(0041702);
    UNKN(0041712);
    TEST("LEA", "(A2), A1",           0041722);
    UNKN(0041732);
    UNKN(0041742);
    TEST("LEA", "($1234,A2), A1",                      0041752, 0x1234);
    NMEM("LEA", "(0,A2), A1",       "(0,A2), A1",      0041752);
    TEST("LEA", "(18,A2,D3.W), A1",                    0041762, 0x3012);
    NMEM("LEA", "(0,A2,D0.W), A1",  "(0,A2,D0.W), A1", 0041762);
    ABST("LEA", "(@A).W, A1", "FF00",                          0041770, 0xFF00);
    EABS("LEA", "(@A).W, A1", "0000", NO_MEMORY, "(@A).W, A1", 0041770);
    ABST("LEA", "(@A).L, A1", "123458",                          0041771, 0x0012, 0x3458);
    EABS("LEA", "(@A).L, A1", "120000", NO_MEMORY, "(@A).L, A1", 0041771, 0x0012);
    EABS("LEA", "(@A).L, A1", "0000", NO_MEMORY,   "(@A).L, A1", 0041771);
    TEST("LEA", "(*+$1234,PC), A1",                    0041772, 0x1232);
    NMEM("LEA", "(*,PC), A1",       "(*,PC), A1",      0041772);
    TEST("LEA", "(*+18,PC,A3.L), A1",                  0041773, 0xB810);
    NMEM("LEA", "(*,PC,D0.W), A1",  "(*,PC,D0.W), A1", 0041773);
    UNKN(0041774);

    // LINK An, #nn: 004712|An
    TEST("LINK", "A3, #$1234",  0047123, 0x1234);
    TEST("LINK", "A3, #-16",    0047123, 0xFFF0);
    NMEM("LINK", "A3, #0", "0", 0047123);
    if (!firstGen()) {
        TEST("LINK.L", "A2, #$12345678", 0044012, 0x1234, 0x5678);
    }

    // MOVE src,dst: 00|Sz|Rd|Md|Ms|Rs, Sz:B=1/W=3/L=2
    TEST("MOVE.B", "D2, D7",             0017002);
    UNKN(0017012); // A2, D7
    TEST("MOVE.B", "(A2), D7",           0017022);
    TEST("MOVE.B", "(A2)+, D7",          0017032);
    TEST("MOVE.B", "-(A2), D7",          0017042);
    TEST("MOVE.B", "($1234,A2), D7",     0017052, 0x1234);
    TEST("MOVE.B", "(18,A2,D3.L), D7",   0017062, 0x3812);
    ABST("MOVE.B", "(@A).W, D7", "FFFF", 0017070, 0xFFFF);
    ABST("MOVE.B", "(@A).L, D7", "123456", 0017071, 0x0012, 0x3456);
    TEST("MOVE.B", "(*+$1234,PC), D7",   0017072, 0x1232);
    TEST("MOVE.B", "(*+18,PC,D3.W), D7", 0017073, 0x3010);
    TEST("MOVE.B", "#$34, D7",           0017074, 0x0034);
    TEST("MOVE.W", "D2, D7",             0037002);
    TEST("MOVE.W", "A2, D7",             0037012);
    TEST("MOVE.W", "(A2), D7",           0037022);
    TEST("MOVE.W", "(A2)+, D7",          0037032);
    TEST("MOVE.W", "-(A2), D7",          0037042);
    TEST("MOVE.W", "($1234,A2), D7",     0037052, 0x1234);
    TEST("MOVE.W", "(18,A2,D3.L), D7",   0037062, 0x3812);
    ABST("MOVE.W", "(@A).W, D7", "FFFE", 0037070, 0xFFFE);
    ABST("MOVE.W", "(@A).L, D7", "123456", 0037071, 0x0012, 0x3456);
    TEST("MOVE.W", "(*+$1234,PC), D7",   0037072, 0x1232);
    TEST("MOVE.W", "(*+18,PC,D3.W), D7", 0037073, 0x3010);
    TEST("MOVE.W", "#$0034, D7",         0037074, 0x0034);
    TEST("MOVE.L", "D2, D7",             0027002);
    TEST("MOVE.L", "A2, D7",             0027012);
    TEST("MOVE.L", "(A2), D7",           0027022);
    TEST("MOVE.L", "(A2)+, D7",          0027032);
    TEST("MOVE.L", "-(A2), D7",          0027042);
    TEST("MOVE.L", "($1234,A2), D7",     0027052, 0x1234);
    TEST("MOVE.L", "(18,A2,D3.L), D7",   0027062, 0x3812);
    ABST("MOVE.L", "(@A).W, D7", "FFFC",   0027070, 0xFFFC);
    ABST("MOVE.L", "(@A).L, D7", "123454", 0027071, 0x0012, 0x3454);
    TEST("MOVE.L", "(*+$1234,PC), D7",   0027072, 0x1232);
    TEST("MOVE.L", "(*+18,PC,D3.W), D7", 0027073, 0x3010);
    TEST("MOVE.L", "#$00345678, D7",     0027074, 0x0034, 0x5678);
    TEST("MOVE.B", "D2, (A6)",             0016202);
    UNKN(0016212); // A2, (A6)
    TEST("MOVE.B", "(A2), (A6)",           0016222);
    TEST("MOVE.B", "(A2)+, (A6)",          0016232);
    TEST("MOVE.B", "-(A2), (A6)",          0016242);
    TEST("MOVE.B", "($1234,A2), (A6)",     0016252, 0x1234);
    TEST("MOVE.B", "(18,A2,D3.L), (A6)",   0016262, 0x3812);
    ABST("MOVE.B", "(@A).W, (A6)", "FFFF",   0016270, 0xFFFF);
    ABST("MOVE.B", "(@A).L, (A6)", "123456", 0016271, 0x0012, 0x3456);
    TEST("MOVE.B", "(*+$1234,PC), (A6)",   0016272, 0x1232);
    TEST("MOVE.B", "(*+18,PC,D3.W), (A6)", 0016273, 0x3010);
    TEST("MOVE.B", "#$34, (A6)",           0016274, 0x0034);
    TEST("MOVE.W", "D2, (A6)",             0036202);
    TEST("MOVE.W", "A2, (A6)",             0036212);
    TEST("MOVE.W", "(A2), (A6)",           0036222);
    TEST("MOVE.W", "(A2)+, (A6)",          0036232);
    TEST("MOVE.W", "-(A2), (A6)",          0036242);
    TEST("MOVE.W", "($1234,A2), (A6)",     0036252, 0x1234);
    TEST("MOVE.W", "(18,A2,D3.L), (A6)",   0036262, 0x3812);
    ABST("MOVE.W", "(@A).W, (A6)", "FFFE",   0036270, 0xFFFE);
    ABST("MOVE.W", "(@A).L, (A6)", "123456", 0036271, 0x0012, 0x3456);
    TEST("MOVE.W", "(*+$1234,PC), (A6)",   0036272, 0x1232);
    TEST("MOVE.W", "(*+18,PC,D3.W), (A6)", 0036273, 0x3010);
    TEST("MOVE.W", "#$0034, (A6)",         0036274, 0x0034);
    TEST("MOVE.L", "D2, (A6)",             0026202);
    TEST("MOVE.L", "A2, (A6)",             0026212);
    TEST("MOVE.L", "(A2), (A6)",           0026222);
    TEST("MOVE.L", "(A2)+, (A6)",          0026232);
    TEST("MOVE.L", "-(A2), (A6)",          0026242);
    TEST("MOVE.L", "($1234,A2), (A6)",     0026252, 0x1234);
    TEST("MOVE.L", "(18,A2,D3.L), (A6)",   0026262, 0x3812);
    ABST("MOVE.L", "(@A).W, (A6)", "FFFC",   0026270, 0xFFFC);
    ABST("MOVE.L", "(@A).L, (A6)", "123454", 0026271, 0x0012, 0x3454);
    TEST("MOVE.L", "(*+$1234,PC), (A6)",   0026272, 0x1232);
    TEST("MOVE.L", "(*+18,PC,D3.W), (A6)", 0026273, 0x3010);
    TEST("MOVE.L", "#$00345678, (A6)",     0026274, 0x0034, 0x5678);
    TEST("MOVE.B", "D2, (A6)+",             0016302);
    UNKN(0016312); // A2, (A6)+
    TEST("MOVE.B", "(A2), (A6)+",           0016322);
    TEST("MOVE.B", "(A2)+, (A6)+",          0016332);
    TEST("MOVE.B", "-(A2), (A6)+",          0016342);
    TEST("MOVE.B", "($1234,A2), (A6)+",     0016352, 0x1234);
    TEST("MOVE.B", "(18,A2,D3.L), (A6)+",   0016362, 0x3812);
    ABST("MOVE.B", "(@A).W, (A6)+", "FFFF",   0016370, 0xFFFF);
    ABST("MOVE.B", "(@A).L, (A6)+", "123456", 0016371, 0x0012, 0x3456);
    TEST("MOVE.B", "(*+$1234,PC), (A6)+",   0016372, 0x1232);
    TEST("MOVE.B", "(*+18,PC,D3.W), (A6)+", 0016373, 0x3010);
    TEST("MOVE.B", "#$34, (A6)+",           0016374, 0x0034);
    TEST("MOVE.W", "D2, (A6)+",             0036302);
    TEST("MOVE.W", "A2, (A6)+",             0036312);
    TEST("MOVE.W", "(A2), (A6)+",           0036322);
    TEST("MOVE.W", "(A2)+, (A6)+",          0036332);
    TEST("MOVE.W", "-(A2), (A6)+",          0036342);
    TEST("MOVE.W", "($1234,A2), (A6)+",     0036352, 0x1234);
    TEST("MOVE.W", "(18,A2,D3.L), (A6)+",   0036362, 0x3812);
    ABST("MOVE.W", "(@A).W, (A6)+", "FFFE",   0036370, 0xFFFE);
    ABST("MOVE.W", "(@A).L, (A6)+", "123456", 0036371, 0x0012, 0x3456);
    TEST("MOVE.W", "(*+$1234,PC), (A6)+",   0036372, 0x1232);
    TEST("MOVE.W", "(*+18,PC,D3.W), (A6)+", 0036373, 0x3010);
    TEST("MOVE.W", "#$0034, (A6)+",         0036374, 0x0034);
    TEST("MOVE.L", "D2, (A6)+",             0026302);
    TEST("MOVE.L", "A2, (A6)+",             0026312);
    TEST("MOVE.L", "(A2), (A6)+",           0026322);
    TEST("MOVE.L", "(A2)+, (A6)+",          0026332);
    TEST("MOVE.L", "-(A2), (A6)+",          0026342);
    TEST("MOVE.L", "($1234,A2), (A6)+",     0026352, 0x1234);
    TEST("MOVE.L", "(18,A2,D3.L), (A6)+",   0026362, 0x3812);
    ABST("MOVE.L", "(@A).W, (A6)+", "FFFC",   0026370, 0xFFFC);
    ABST("MOVE.L", "(@A).L, (A6)+", "123454", 0026371, 0x0012, 0x3454);
    TEST("MOVE.L", "(*+$1234,PC), (A6)+",   0026372, 0x1232);
    TEST("MOVE.L", "(*+18,PC,D3.W), (A6)+", 0026373, 0x3010);
    TEST("MOVE.L", "#$00345678, (A6)+",     0026374, 0x0034, 0x5678);
    TEST("MOVE.B", "D2, -(A6)",             0016402);
    UNKN(0016412); // A2, -(A6)
    TEST("MOVE.B", "(A2), -(A6)",           0016422);
    TEST("MOVE.B", "(A2)+, -(A6)",          0016432);
    TEST("MOVE.B", "-(A2), -(A6)",          0016442);
    TEST("MOVE.B", "($1234,A2), -(A6)",     0016452, 0x1234);
    TEST("MOVE.B", "(18,A2,D3.L), -(A6)",   0016462, 0x3812);
    ABST("MOVE.B", "(@A).W, -(A6)", "FFFF",   0016470, 0xFFFF);
    ABST("MOVE.B", "(@A).L, -(A6)", "123456", 0016471, 0x0012, 0x3456);
    TEST("MOVE.B", "(*+$1234,PC), -(A6)",   0016472, 0x1232);
    TEST("MOVE.B", "(*+18,PC,D3.W), -(A6)", 0016473, 0x3010);
    TEST("MOVE.B", "#$34, -(A6)",           0016474, 0x0034);
    TEST("MOVE.W", "D2, -(A6)",             0036402);
    TEST("MOVE.W", "A2, -(A6)",             0036412);
    TEST("MOVE.W", "(A2), -(A6)",           0036422);
    TEST("MOVE.W", "(A2)+, -(A6)",          0036432);
    TEST("MOVE.W", "-(A2), -(A6)",          0036442);
    TEST("MOVE.W", "($1234,A2), -(A6)",     0036452, 0x1234);
    TEST("MOVE.W", "(18,A2,D3.L), -(A6)",   0036462, 0x3812);
    ABST("MOVE.W", "(@A).W, -(A6)", "FFFE",   0036470, 0xFFFE);
    ABST("MOVE.W", "(@A).L, -(A6)", "123456", 0036471, 0x0012, 0x3456);
    TEST("MOVE.W", "(*+$1234,PC), -(A6)",   0036472, 0x1232);
    TEST("MOVE.W", "(*+18,PC,D3.W), -(A6)", 0036473, 0x3010);
    TEST("MOVE.W", "#$0034, -(A6)",         0036474, 0x0034);
    TEST("MOVE.L", "D2, -(A6)",             0026402);
    TEST("MOVE.L", "A2, -(A6)",             0026412);
    TEST("MOVE.L", "(A2), -(A6)",           0026422);
    TEST("MOVE.L", "(A2)+, -(A6)",          0026432);
    TEST("MOVE.L", "-(A2), -(A6)",          0026442);
    TEST("MOVE.L", "($1234,A2), -(A6)",     0026452, 0x1234);
    TEST("MOVE.L", "(18,A2,D3.L), -(A6)",   0026462, 0x3812);
    ABST("MOVE.L", "(@A).W, -(A6)", "FFFC",   0026470, 0xFFFC);
    ABST("MOVE.L", "(@A).L, -(A6)", "123454", 0026471, 0x0012, 0x3454);
    TEST("MOVE.L", "(*+$1234,PC), -(A6)",   0026472, 0x1232);
    TEST("MOVE.L", "(*+18,PC,D3.W), -(A6)", 0026473, 0x3010);
    TEST("MOVE.L", "#$00345678, -(A6)",     0026474, 0x0034, 0x5678);
    TEST("MOVE.B", "D2, ($5678,A6)",             0016502, 0x5678);
    UNKN(0016512); // A2, ($5678,A6)
    TEST("MOVE.B", "(A2), ($5678,A6)",           0016522, 0x5678);
    TEST("MOVE.B", "(A2)+, ($5678,A6)",          0016532, 0x5678);
    TEST("MOVE.B", "-(A2), ($5678,A6)",          0016542, 0x5678);
    TEST("MOVE.B", "($1234,A2), ($5678,A6)",     0016552, 0x1234, 0x5678);
    TEST("MOVE.B", "(18,A2,D3.L), ($5678,A6)",   0016562, 0x3812, 0x5678);
    ABST("MOVE.B", "(@A).W, ($5678,A6)", "FFFF",   0016570, 0xFFFF, 0x5678);
    ABST("MOVE.B", "(@A).L, ($5678,A6)", "123456", 0016571, 0x0012, 0x3456, 0x5678);
    TEST("MOVE.B", "(*+$1234,PC), ($5678,A6)",   0016572, 0x1232, 0x5678);
    TEST("MOVE.B", "(*+18,PC,D3.W), ($5678,A6)", 0016573, 0x3010, 0x5678);
    TEST("MOVE.B", "#$34, ($5678,A6)",           0016574, 0x0034, 0x5678);
    TEST("MOVE.W", "D2, ($5678,A6)",             0036502, 0x5678);
    TEST("MOVE.W", "A2, ($5678,A6)",             0036512, 0x5678);
    TEST("MOVE.W", "(A2), ($5678,A6)",           0036522, 0x5678);
    TEST("MOVE.W", "(A2)+, ($5678,A6)",          0036532, 0x5678);
    TEST("MOVE.W", "-(A2), ($5678,A6)",          0036542, 0x5678);
    TEST("MOVE.W", "($1234,A2), ($5678,A6)",     0036552, 0x1234, 0x5678);
    TEST("MOVE.W", "(18,A2,D3.L), ($5678,A6)",   0036562, 0x3812, 0x5678);
    ABST("MOVE.W", "(@A).W, ($5678,A6)", "FFFE",   0036570, 0xFFFE, 0x5678);
    ABST("MOVE.W", "(@A).L, ($5678,A6)", "123456", 0036571, 0x0012, 0x3456, 0x5678);
    TEST("MOVE.W", "(*+$1234,PC), ($5678,A6)",   0036572, 0x1232, 0x5678);
    TEST("MOVE.W", "(*+18,PC,D3.W), ($5678,A6)", 0036573, 0x3010, 0x5678);
    TEST("MOVE.W", "#$0034, ($5678,A6)",         0036574, 0x0034, 0x5678);
    TEST("MOVE.L", "D2, ($5678,A6)",             0026502, 0x5678);
    TEST("MOVE.L", "A2, ($5678,A6)",             0026512, 0x5678);
    TEST("MOVE.L", "(A2), ($5678,A6)",           0026522, 0x5678);
    TEST("MOVE.L", "(A2)+, ($5678,A6)",          0026532, 0x5678);
    TEST("MOVE.L", "-(A2), ($5678,A6)",          0026542, 0x5678);
    TEST("MOVE.L", "($1234,A2), ($5678,A6)",     0026552, 0x1234, 0x5678);
    TEST("MOVE.L", "(18,A2,D3.L), ($5678,A6)",   0026562, 0x3812, 0x5678);
    ABST("MOVE.L", "(@A).W, ($5678,A6)", "FFFC",   0026570, 0xFFFC, 0x5678);
    ABST("MOVE.L", "(@A).L, ($5678,A6)", "123454", 0026571, 0x0012, 0x3454, 0x5678);
    TEST("MOVE.L", "(*+$1234,PC), ($5678,A6)",   0026572, 0x1232, 0x5678);
    TEST("MOVE.L", "(*+18,PC,D3.W), ($5678,A6)", 0026573, 0x3010, 0x5678);
    TEST("MOVE.L", "#$00345678, ($5678,A6)",     0026574, 0x0034, 0x5678, 0x5678);
    TEST("MOVE.B", "D2, ($56,A6,D5.W)",             0016602, 0x5056);
    UNKN(0016612); // A2, ($56,A6,D5.W)
    TEST("MOVE.B", "(A2), ($56,A6,D5.W)",           0016622, 0x5056);
    TEST("MOVE.B", "(A2)+, ($56,A6,D5.W)",          0016632, 0x5056);
    TEST("MOVE.B", "-(A2), ($56,A6,D5.W)",          0016642, 0x5056);
    TEST("MOVE.B", "($1234,A2), ($56,A6,D5.W)",     0016652, 0x1234, 0x5056);
    TEST("MOVE.B", "(18,A2,D3.L), ($56,A6,D5.W)",   0016662, 0x3812, 0x5056);
    ABST("MOVE.B", "(@A).W, ($56,A6,D5.W)", "FFFF",   0016670, 0xFFFF, 0x5056);
    ABST("MOVE.B", "(@A).L, ($56,A6,D5.W)", "123456", 0016671, 0x0012, 0x3456, 0x5056);
    TEST("MOVE.B", "(*+$1234,PC), ($56,A6,D5.W)",   0016672, 0x1232, 0x5056);
    TEST("MOVE.B", "(*+18,PC,D3.W), ($56,A6,D5.W)", 0016673, 0x3010, 0x5056);
    TEST("MOVE.B", "#$34, ($56,A6,D5.W)",           0016674, 0x0034, 0x5056);
    TEST("MOVE.W", "D2, ($56,A6,D5.W)",             0036602, 0x5056);
    TEST("MOVE.W", "A2, ($56,A6,D5.W)",             0036612, 0x5056);
    TEST("MOVE.W", "(A2), ($56,A6,D5.W)",           0036622, 0x5056);
    TEST("MOVE.W", "(A2)+, ($56,A6,D5.W)",          0036632, 0x5056);
    TEST("MOVE.W", "-(A2), ($56,A6,D5.W)",          0036642, 0x5056);
    TEST("MOVE.W", "($1234,A2), ($56,A6,D5.W)",     0036652, 0x1234, 0x5056);
    TEST("MOVE.W", "(18,A2,D3.L), ($56,A6,D5.W)",   0036662, 0x3812, 0x5056);
    ABST("MOVE.W", "(@A).W, ($56,A6,D5.W)", "FFFE",   0036670, 0xFFFE, 0x5056);
    ABST("MOVE.W", "(@A).L, ($56,A6,D5.W)", "123456", 0036671, 0x0012, 0x3456, 0x5056);
    TEST("MOVE.W", "(*+$1234,PC), ($56,A6,D5.W)",   0036672, 0x1232, 0x5056);
    TEST("MOVE.W", "(*+18,PC,D3.W), ($56,A6,D5.W)", 0036673, 0x3010, 0x5056);
    TEST("MOVE.W", "#$0034, ($56,A6,D5.W)",         0036674, 0x0034, 0x5056);
    TEST("MOVE.L", "D2, ($56,A6,D5.W)",             0026602, 0x5056);
    TEST("MOVE.L", "A2, ($56,A6,D5.W)",             0026612, 0x5056);
    TEST("MOVE.L", "(A2), ($56,A6,D5.W)",           0026622, 0x5056);
    TEST("MOVE.L", "(A2)+, ($56,A6,D5.W)",          0026632, 0x5056);
    TEST("MOVE.L", "-(A2), ($56,A6,D5.W)",          0026642, 0x5056);
    TEST("MOVE.L", "($1234,A2), ($56,A6,D5.W)",     0026652, 0x1234, 0x5056);
    TEST("MOVE.L", "(18,A2,D3.L), ($56,A6,D5.W)",   0026662, 0x3812, 0x5056);
    ABST("MOVE.L", "(@A).W, ($56,A6,D5.W)", "FFFC",   0026670, 0xFFFC, 0x5056);
    ABST("MOVE.L", "(@A).L, ($56,A6,D5.W)", "123454", 0026671, 0x0012, 0x3454, 0x5056);
    TEST("MOVE.L", "(*+$1234,PC), ($56,A6,D5.W)",   0026672, 0x1232, 0x5056);
    TEST("MOVE.L", "(*+18,PC,D3.W), ($56,A6,D5.W)", 0026673, 0x3010, 0x5056);
    TEST("MOVE.L", "#$00345678, ($56,A6,D5.W)",     0026674, 0x0034, 0x5678, 0x5056);
    ABST("MOVE.B", "D2, (@A).W", "5678",            0010702, 0x5678);
    UNKN(0010712); // A2, ($005678).W
    ABST("MOVE.B", "(A2), (@A).W", "5678",        0010722, 0x5678);
    ABST("MOVE.B", "(A2)+, (@A).W", "5678",       0010732, 0x5678);
    ABST("MOVE.B", "-(A2), (@A).W", "5678",       0010742, 0x5678);
    ABST("MOVE.B", "($1234,A2), (@A).W", "5678",  0010752, 0x1234, 0x5678);
    ABST("MOVE.B", "(18,A2,D3.L), (@A).W", "5678", 0010762, 0x3812, 0x5678);
    ABST("MOVE.B", "(@A).W, (@A).W", "FFFF:5678",  0010770, 0xFFFF, 0x5678);
    ABST("MOVE.B", "(@A).L, (@A).W", "123456:5678", 0010771, 0x0012, 0x3456, 0x5678);
    ABST("MOVE.B", "(*+$1234,PC), (@A).W", "5678",  0010772, 0x1232, 0x5678);
    ABST("MOVE.B", "(*+18,PC,D3.W), (@A).W", "5678", 0010773, 0x3010, 0x5678);
    ABST("MOVE.B", "#$34, (@A).W", "5678",           0010774, 0x0034, 0x5678);
    ABST("MOVE.W", "D2, (@A).W", "5678",             0030702, 0x5678);
    ABST("MOVE.W", "A2, (@A).W", "5678",             0030712, 0x5678);
    ABST("MOVE.W", "(A2), (@A).W", "5678",           0030722, 0x5678);
    ABST("MOVE.W", "(A2)+, (@A).W", "5678",          0030732, 0x5678);
    ABST("MOVE.W", "-(A2), (@A).W", "5678",          0030742, 0x5678);
    ABST("MOVE.W", "($1234,A2), (@A).W", "5678",     0030752, 0x1234, 0x5678);
    ABST("MOVE.W", "(18,A2,D3.L), (@A).W", "5678",   0030762, 0x3812, 0x5678);
    ABST("MOVE.W", "(@A).W, (@A).W", "FFFE:5678",    0030770, 0xFFFE, 0x5678);
    ABST("MOVE.W", "(@A).L, (@A).W", "123456:5678",  0030771, 0x0012, 0x3456, 0x5678);
    ABST("MOVE.W", "(*+$1234,PC), (@A).W", "5678",   0030772, 0x1232, 0x5678);
    ABST("MOVE.W", "(*+18,PC,D3.W), (@A).W", "5678", 0030773, 0x3010, 0x5678);
    ABST("MOVE.W", "#$0034, (@A).W", "5678",         0030774, 0x0034, 0x5678);
    ABST("MOVE.L", "D2, (@A).W", "5678",             0020702, 0x5678);
    ABST("MOVE.L", "A2, (@A).W", "5678",            0020712, 0x5678);
    ABST("MOVE.L", "(A2), (@A).W", "5678",          0020722, 0x5678);
    ABST("MOVE.L", "(A2)+, (@A).W", "5678",         0020732, 0x5678);
    ABST("MOVE.L", "-(A2), (@A).W", "5678",         0020742, 0x5678);
    ABST("MOVE.L", "($1234,A2), (@A).W", "5678",    0020752, 0x1234, 0x5678);
    ABST("MOVE.L", "(18,A2,D3.L), (@A).W", "5678",  0020762, 0x3812, 0x5678);
    ABST("MOVE.L", "(@A).W, (@A).W", "FFFC:5678",   0020770, 0xFFFC, 0x5678);
    ABST("MOVE.L", "(@A).L, (@A).W", "123454:5678", 0020771, 0x0012, 0x3454, 0x5678);
    ABST("MOVE.L", "(*+$1234,PC), (@A).W", "5678",  0020772, 0x1232, 0x5678);
    ABST("MOVE.L", "(*+18,PC,D3.W), (@A).W", "5678",0020773, 0x3010, 0x5678);
    ABST("MOVE.L", "#$00345678, (@A).W", "5678",    0020774, 0x0034, 0x5678, 0x5678);
    ABST("MOVE.B", "D2, (@A).L", "56789A",          0011702, 0x0056, 0x789A);
    UNKN(0011712); // A2, ($56789A).L
    ABST("MOVE.B", "(A2), (@A).L", "56789A",          0011722, 0x0056, 0x789A);
    ABST("MOVE.B", "(A2)+, (@A).L", "56789A",         0011732, 0x0056, 0x789A);
    ABST("MOVE.B", "-(A2), (@A).L", "56789A",         0011742, 0x0056, 0x789A);
    ABST("MOVE.B", "($1234,A2), (@A).L", "56789A",    0011752, 0x1234, 0x0056, 0x789A);
    ABST("MOVE.B", "(18,A2,D3.L), (@A).L", "56789A",  0011762, 0x3812, 0x0056, 0x789A);
    ABST("MOVE.B", "(@A).W, (@A).L", "FFFF:56789A",   0011770, 0xFFFF, 0x0056, 0x789A);
    ABST("MOVE.B", "(@A).L, (@A).L", "123456:56789A", 0011771, 0x0012, 0x3456, 0x0056, 0x789A);
    ABST("MOVE.B", "(*+$1234,PC), (@A).L", "56789A",  0011772, 0x1232, 0x0056, 0x789A);
    ABST("MOVE.B", "(*+18,PC,D3.W), (@A).L", "56789A",0011773, 0x3010, 0x0056, 0x789A);
    ABST("MOVE.B", "#$34, (@A).L", "56789A",          0011774, 0x0034, 0x0056, 0x789A);
    ABST("MOVE.W", "D2, (@A).L", "56789A",            0031702, 0x0056, 0x789A);
    ABST("MOVE.W", "A2, (@A).L", "56789A",            0031712, 0x0056, 0x789A);
    ABST("MOVE.W", "(A2), (@A).L", "56789A",          0031722, 0x0056, 0x789A);
    ABST("MOVE.W", "(A2)+, (@A).L", "56789A",         0031732, 0x0056, 0x789A);
    ABST("MOVE.W", "-(A2), (@A).L", "56789A",         0031742, 0x0056, 0x789A);
    ABST("MOVE.W", "($1234,A2), (@A).L", "56789A",    0031752, 0x1234, 0x0056, 0x789A);
    ABST("MOVE.W", "(18,A2,D3.L), (@A).L", "56789A",  0031762, 0x3812, 0x0056, 0x789A);
    ABST("MOVE.W", "(@A).W, (@A).L", "FFFE:56789A",   0031770, 0xFFFE, 0x0056, 0x789A);
    ABST("MOVE.W", "(@A).L, (@A).L", "123456:56789A", 0031771, 0x0012, 0x3456, 0x0056, 0x789A);
    ABST("MOVE.W", "(*+$1234,PC), (@A).L", "56789A",  0031772, 0x1232, 0x0056, 0x789A);
    ABST("MOVE.W", "(*+18,PC,D3.W), (@A).L", "56789A",0031773, 0x3010, 0x0056, 0x789A);
    ABST("MOVE.W", "#$0034, (@A).L", "56789A",        0031774, 0x0034, 0x0056, 0x789A);
    ABST("MOVE.L", "D2, (@A).L", "567898",            0021702, 0x0056, 0x7898);
    ABST("MOVE.L", "A2, (@A).L", "567898",            0021712, 0x0056, 0x7898);
    ABST("MOVE.L", "(A2), (@A).L", "567898",          0021722, 0x0056, 0x7898);
    ABST("MOVE.L", "(A2)+, (@A).L", "567898",         0021732, 0x0056, 0x7898);
    ABST("MOVE.L", "-(A2), (@A).L", "567898",         0021742, 0x0056, 0x7898);
    ABST("MOVE.L", "($1234,A2), (@A).L", "567898",    0021752, 0x1234, 0x0056, 0x7898);
    ABST("MOVE.L", "(18,A2,D3.L), (@A).L", "567898",  0021762, 0x3812, 0x0056, 0x7898);
    ABST("MOVE.L", "(@A).W, (@A).L", "FFFC:567898",   0021770, 0xFFFC, 0x0056, 0x7898);
    ABST("MOVE.L", "(@A).L, (@A).L", "123454:567898", 0021771, 0x0012, 0x3454, 0x0056, 0x7898);
    ABST("MOVE.L", "(*+$1234,PC), (@A).L", "567898",  0021772, 0x1232, 0x0056, 0x7898);
    ABST("MOVE.L", "(*+18,PC,D3.W), (@A).L", "567898",0021773, 0x3010, 0x0056, 0x7898);
    ABST("MOVE.L", "#$00345678, (@A).L", "567898",    0021774, 0x0034, 0x5678, 0x0056, 0x7898);
    UNKN(0012702); // D2, (*+$1234,PC)
    UNKN(0033702); // D2, (*+3,PC,D0.W)
    UNKN(0024702); // D2, #$12345678

    // MOVEA src,An: 00|Sz|An|1|M|Rn, Sz:W=3/L=2
    TEST("MOVEA.W", "D2, A6",             0036102);
    TEST("MOVEA.W", "A2, A6",             0036112);
    TEST("MOVEA.W", "(A2), A6",           0036122);
    TEST("MOVEA.W", "(A2)+, A6",          0036132);
    TEST("MOVEA.W", "-(A2), A6",          0036142);
    TEST("MOVEA.W", "($1234,A2), A6",     0036152, 0x1234);
    TEST("MOVEA.W", "(18,A2,D3.L), A6",   0036162, 0x3812);
    ABST("MOVEA.W", "(@A).W, A6", "FFFE",   0036170, 0xFFFE);
    ABST("MOVEA.W", "(@A).L, A6", "123456", 0036171, 0x0012, 0x3456);
    TEST("MOVEA.W", "(*+$1234,PC), A6",   0036172, 0x1232);
    TEST("MOVEA.W", "(*+18,PC,D3.W), A6", 0036173, 0x3010);
    TEST("MOVEA.W", "#$0034, A6",         0036174, 0x0034);
    TEST("MOVEA.L", "D2, A6",             0026102);
    TEST("MOVEA.L", "A2, A6",             0026112);
    TEST("MOVEA.L", "(A2), A6",           0026122);
    TEST("MOVEA.L", "(A2)+, A6",          0026132);
    TEST("MOVEA.L", "-(A2), A6",          0026142);
    TEST("MOVEA.L", "($1234,A2), A6",     0026152, 0x1234);
    TEST("MOVEA.L", "(18,A2,D3.L), A6",   0026162, 0x3812);
    ABST("MOVEA.L", "(@A).W, A6", "FFFC",   0026170, 0xFFFC);
    ABST("MOVEA.L", "(@A).L, A6", "123454", 0026171, 0x0012, 0x3454);
    TEST("MOVEA.L", "(*+$1234,PC), A6",   0026172, 0x1232);
    TEST("MOVEA.L", "(*+18,PC,D3.W), A6", 0026173, 0x3010);
    TEST("MOVEA.L", "#$00345678, A6",     0026174, 0x0034, 0x5678);

    // MOVEM list,dst: 0044|Sz|Md|Rd, Sz:W=2/L=3, list=A7|...|D0, -(An)=D0|...|A7
    TEST("EXT.W",   "D2",                  0044202); // EXT.W
    UNKN(0044212); // D0-D6, A2
    TEST("MOVEM.W", "D0-D6, (A2)",         0044222, 0x007F);
    UNKN(0044232); // D0-D6, (A2)+
    TEST("MOVEM.W", "D0-D6, -(A2)",        0044242, 0xFE00);
    TEST("MOVEM.W", "D0-D6, ($1234,A2)",   0044252, 0x007F, 0x1234);
    TEST("MOVEM.W", "D0-D6, (18,A2,A3.L)", 0044262, 0x007F, 0xB812);
    ABST("MOVEM.W", "D0-D6, (@A).W", "FEDC",   0044270, 0x007F, 0xFEDC);
    ABST("MOVEM.W", "D0-D6, (@A).L", "123456", 0044271, 0x007F, 0x0012, 0x3456);
    UNKN(0044272); // D0-D6, (*+$1234,PC)
    UNKN(0044273); // D0-D6, (*+10,PC,A3.L)
    UNKN(0044274); // D0-D6, #$1234
    TEST("MOVEM.W", "D0, (A2)",            0044222, 0x0001);
    TEST("MOVEM.W", "A0, (A2)",            0044222, 0x0100);
    TEST("MOVEM.W", "D0, -(A2)",           0044242, 0x8000);
    TEST("MOVEM.W", "A0, -(A2)",           0044242, 0x0080);
    TEST("EXT.L",   "D2",                  0044302); // EXT.L
    UNKN(0044312); // D1/A0, A2
    TEST("MOVEM.L", "D1/A0, (A2)",         0044322, 0x0102);
    UNKN(0044332); // D1/A0, (A2)+
    TEST("MOVEM.L", "D1/A0, -(A2)",        0044342, 0x4080);
    TEST("MOVEM.L", "D1/A0, ($1234,A2)",   0044352, 0x0102, 0x1234);
    TEST("MOVEM.L", "D1/A0, (18,A2,A3.L)", 0044362, 0x0102, 0xB812);
    ABST("MOVEM.L", "D1/A0, (@A).W", "FEDC",   0044370, 0x0102, 0xFEDC);
    ABST("MOVEM.L", "D1/A0, (@A).L", "123454", 0044371, 0x0102, 0x0012, 0x3454);
    UNKN(0044372); // D1/A0, (*+$1234,PC)
    UNKN(0044373); // D1/A0, (*+10,PC,A3.L)
    UNKN(0044374); // D1/A0, #$12345678
    TEST("MOVEM.L", "D0, (A2)",            0044322, 0x0001);
    TEST("MOVEM.L", "A0, (A2)",            0044322, 0x0100);
    TEST("MOVEM.L", "D0, -(A2)",           0044342, 0x8000);
    TEST("MOVEM.L", "A0, -(A2)",           0044342, 0x0080);
    NMEM("MOVEM.L", ", -(A2)", ", -(A2)",  0044342);

    // MOVEM src,list: 0046|Sz|Md|Rd, Sz:W=2/L=3, list=A7|...|D0
    UNKN(0046202); // D2, A3-A6
    UNKN(0046212); // A2, A3-A6
    TEST("MOVEM.W", "(A2), A3-A6",           0046222, 0x7800);
    TEST("MOVEM.W", "(A2)+, A3-A6",          0046232, 0x7800);
    UNKN(0046242); // -(A2), D1-D4
    TEST("MOVEM.W", "($1234,A2), A3-A6",     0046252, 0x7800, 0x1234);
    TEST("MOVEM.W", "(18,A2,D3.W), A3-A6",   0046262, 0x7800, 0x3012);
    ABST("MOVEM.W", "(@A).W, A3-A6", "FEDC",   0046270, 0x7800, 0xFEDC);
    ABST("MOVEM.W", "(@A).L, A3-A6", "123456", 0046271, 0x7800, 0x0012, 0x3456);
    TEST("MOVEM.W", "(*+$1234,PC), A3-A6",   0046272, 0x7800, 0x1230);
    TEST("MOVEM.W", "(*+18,PC,D3.L), A3-A6", 0046273, 0x7800, 0x380E);
    UNKN(0046274); // #$1234, A3-A6
    TEST("MOVEM.W", "(A2), D0",              0046222, 0x0001);
    TEST("MOVEM.W", "(A2), A0",              0046222, 0x0100);
    TEST("MOVEM.W", "(A2)+, D0",             0046232, 0x0001);
    TEST("MOVEM.W", "(A2)+, A0",             0046232, 0x0100);
    UNKN(0046302); // D2, A3-A6
    UNKN(0046312); // A2, A3-A6
    TEST("MOVEM.L", "(A2), A3-A6",           0046322, 0x7800);
    TEST("MOVEM.L", "(A2)+, A3-A6",          0046332, 0x7800);
    UNKN(0046342); // -(A2), D1-D4
    TEST("MOVEM.L", "($1234,A2), A3-A6",     0046352, 0x7800, 0x1234);
    TEST("MOVEM.L", "(18,A2,D3.W), A3-A6",   0046362, 0x7800, 0x3012);
    ABST("MOVEM.L", "(@A).W, A3-A6", "FEDC",   0046370, 0x7800, 0xFEDC);
    ABST("MOVEM.L", "(@A).L, A3-A6", "123454", 0046371, 0x7800, 0x0012, 0x3454);
    TEST("MOVEM.L", "(*+$1234,PC), A3-A6",   0046372, 0x7800, 0x1230);
    TEST("MOVEM.L", "(*+18,PC,D3.L), A3-A6", 0046373, 0x7800, 0x380E);
    UNKN(0046374); // #$12345678, A3-A6
    TEST("MOVEM.L", "(A2), D0",              0046322, 0x0001);
    TEST("MOVEM.L", "(A2), A0",              0046322, 0x0100);
    TEST("MOVEM.L", "(A2)+, D0",             0046332, 0x0001);
    TEST("MOVEM.L", "(A2)+, A0",             0046332, 0x0100);
    NMEM("MOVEM.L", "(A2)+, ",           "", 0046332);

    // MOVEP Dn, (d16,An): 000|Dn|Sz|1|An, SZ:W=6/L=7
    TEST("MOVEP.W", "D7, ($1234,A2)", 0007612, 0x1234);
    TEST("MOVEP.L", "D7, ($1234,A2)", 0007712, 0x1234);

    // MOVEP (d16,An),Dn: 000|Dn|Sz|1|An, SZ:W=4/L=5
    TEST("MOVEP.W", "($1234,A2), D7", 0007412, 0x1234);
    TEST("MOVEP.L", "($1234,A2), D7", 0007512, 0x1234);

    if (mc68k00()) {
        UNKN(007000);
        UNKN(007100);
        UNKN(007200);
    } else {
        UNKN(/* MOVES.B D0, */              007000);
        UNKN(/* MOVES.B A1, */              007011);
        TEST("MOVES.B", "(A2), D1",         007022, 0x1000);
        TEST("MOVES.B", "(A3)+, A2",        007033, 0xA000);
        TEST("MOVES.B", "-(A4), D3",        007044, 0x3000);
        TEST("MOVES.B", "($1234,A5), A4",   007055, 0xC000, 0x1234);
        TEST("MOVES.B", "(18,A6,D3.W), D5", 007066, 0x5000, 0x3012);
        ABST("MOVES.B", "(@A).W, A6", "1234",   007070, 0xE000, 0x1234);
        ABST("MOVES.B", "(@A).L, D7", "123456", 007071, 0x7000, 0x0012, 0x3456);
        UNKN(/* MOVES.B (d16,PC), */        007072);
        UNKN(/* MOVES.B (d8,PC,Xn), */      007073);
        UNKN(/* MOVES.B #nnn, */            007074);
        UNKN(/* MOVES.W D0, */              007100);
        UNKN(/* MOVES.W A1, */              007111);
        TEST("MOVES.W", "(A2), D1",         007122, 0x1000);
        TEST("MOVES.W", "(A3)+, A2",        007133, 0xA000);
        TEST("MOVES.W", "-(A4), D3",        007144, 0x3000);
        TEST("MOVES.W", "($1234,A5), A4",   007155, 0xC000, 0x1234);
        TEST("MOVES.W", "(18,A6,D3.W), D5", 007166, 0x5000, 0x3012);
        ABST("MOVES.W", "(@A).W, A6", "1234",   007170, 0xE000, 0x1234);
        ABST("MOVES.W", "(@A).L, D7", "123456", 007171, 0x7000, 0x0012, 0x3456);
        UNKN(/* MOVES.W (d16,PC), */        007172);
        UNKN(/* MOVES.W (d8,PC,Xn), */      007173);
        UNKN(/* MOVES.W #nnn, */            007174);
        UNKN(/* MOVES.L D0, */              007200);
        UNKN(/* MOVES.L A1, */              007211);
        TEST("MOVES.L", "(A2), D1",         007222, 0x1000);
        TEST("MOVES.L", "(A3)+, A2",        007233, 0xA000);
        TEST("MOVES.L", "-(A4), D3",        007244, 0x3000);
        TEST("MOVES.L", "($1234,A5), A4",   007255, 0xC000, 0x1234);
        TEST("MOVES.L", "(18,A6,D3.W), D5", 007266, 0x5000, 0x3012);
        ABST("MOVES.L", "(@A).W, A6", "1234",   007270, 0xE000, 0x1234);
        ABST("MOVES.L", "(@A).L, D7", "123456", 007271, 0x7000, 0x0012, 0x3456);
        UNKN(/* MOVES.L (d16,PC), */        007272);
        UNKN(/* MOVES.L (d8,PC,Xn), */      007273);
        UNKN(/* MOVES.L #nnn, */            007274);

        UNKN(/* MOVES.B , D0 */             007000);
        UNKN(/* MOVES.B , A1 */             007011);
        TEST("MOVES.B", "D1, (A2)",         007022, 0x1800);
        TEST("MOVES.B", "A2, (A3)+",        007033, 0xA800);
        TEST("MOVES.B", "D3, -(A4)",        007044, 0x3800);
        TEST("MOVES.B", "A4, ($1234,A5)",   007055, 0xC800, 0x1234);
        TEST("MOVES.B", "D5, (18,A6,D3.W)", 007066, 0x5800, 0x3012);
        ABST("MOVES.B", "A6, (@A).W", "1234",   007070, 0xE800, 0x1234);
        ABST("MOVES.B", "D7, (@A).L", "123456", 007071, 0x7800, 0x0012, 0x3456);
        UNKN(/* MOVES.B , (d16,PC) */       007072);
        UNKN(/* MOVES.B , (d8,PC,Xn) */     007073);
        UNKN(/* MOVES.B , #nnn */           007074);
        UNKN(/* MOVES.W , D0 */             007000);
        UNKN(/* MOVES.W , A1 */             007011);
        TEST("MOVES.W", "D1, (A2)",         007122, 0x1800);
        TEST("MOVES.W", "A2, (A3)+",        007133, 0xA800);
        TEST("MOVES.W", "D3, -(A4)",        007144, 0x3800);
        TEST("MOVES.W", "A4, ($1234,A5)",   007155, 0xC800, 0x1234);
        TEST("MOVES.W", "D5, (18,A6,D3.W)", 007166, 0x5800, 0x3012);
        ABST("MOVES.W", "A6, (@A).W", "1234",   007170, 0xE800, 0x1234);
        ABST("MOVES.W", "D7, (@A).L", "123456", 007171, 0x7800, 0x0012, 0x3456);
        UNKN(/* MOVES.W , (d16,PC) */       007172);
        UNKN(/* MOVES.W , (d8,PC,Xn) */     007173);
        UNKN(/* MOVES.W , #nnn */           007174);
        UNKN(/* MOVES.L , D0 */             007200);
        UNKN(/* MOVES.L , A1 */             007211);
        TEST("MOVES.L", "D1, (A2)",         007222, 0x1800);
        TEST("MOVES.L", "A2, (A3)+",        007233, 0xA800);
        TEST("MOVES.L", "D3, -(A4)",        007244, 0x3800);
        TEST("MOVES.L", "A4, ($1234,A5)",   007255, 0xC800, 0x1234);
        TEST("MOVES.L", "D5, (18,A6,D3.W)", 007266, 0x5800, 0x3012);
        ABST("MOVES.L", "A6, (@A).W", "1234",   007270, 0xE800, 0x1234);
        ABST("MOVES.L", "D7, (@A).L", "123456", 007271, 0x7800, 0x0012, 0x3456);
        UNKN(/* MOVES.L , (d16,PC) */       007272);
        UNKN(/* MOVES.L , (d8,PC,Xn) */     007273);
        UNKN(/* MOVES.L , #nnn */           007274);
    }

    // MOVEQ #nn,Dn: 007|Dn|000 + nn
    TEST("MOVEQ", "#0, D0",    0070000 | 0x00);
    TEST("MOVEQ", "#$7F, D0",  0070000 | 0x7F);
    TEST("MOVEQ", "#-$80, D0", 0070000 | 0x80);
    TEST("MOVEQ", "#-1, D7",   0077000 | 0xFF);

    // PEA src: 00441|M|Rn
    TEST("SWAP", "D2",            0044102); // SWAP
    if (mc68k00()) {
        UNKN(0044112); // A2
    } else {
        TEST("BKPT", "#2",        0044112);
    }
    TEST("PEA", "(A2)",           0044122);
    UNKN(0044132); // (A2)+
    UNKN(0044142); // -(A2)
    TEST("PEA", "($1234,A2)",     0044152, 0x1234);
    TEST("PEA", "(18,A2,D3.W)",   0044162, 0x3012);
    ABST("PEA", "(@A).W", "FF00", 0044170, 0xFF00);
    ABST("PEA", "(@A).L", "1234", 0044171, 0x0000, 0x1234);
    TEST("PEA", "(*+$1234,PC)",   0044172, 0x1232);
    TEST("PEA", "(*+18,PC,A3.L)", 0044173, 0xB810);
    UNKN(0044174); // #$12345678

    // UNLK An: 004713|An
    TEST("UNLK", "A0", 0047130);
    TEST("UNLK", "A7", 0047137);
}

void test_integer() {
    // ADD Dn,dst: 015|Dn|Sz|M|Rn, Sz:B=4/W=5/L=6
    TEST("ADDX.B", "D2, D7",          0157402); // ADDX.B
    TEST("ADDX.B", "-(A2), -(A7)",    0157412); // ADDX.B
    TEST("ADD.B", "D7, (A2)",         0157422);
    TEST("ADD.B", "D7, (A2)+",        0157432);
    TEST("ADD.B", "D7, -(A2)",        0157442);
    TEST("ADD.B", "D7, ($1234,A2)",   0157452, 0x1234);
    TEST("ADD.B", "D7, (18,A2,D3.L)", 0157462, 0x3812);
    ABST("ADD.B", "D7, (@A).W", "FFFF",   0157470, 0xFFFF);
    ABST("ADD.B", "D7, (@A).L", "123456", 0157471, 0x0012, 0x3456);
    UNKN(0157472); // D7, (*+$1234,PC)
    UNKN(0157473); // D7, (*+10,PC,A3.W)
    UNKN(0157474); // D7, #$34
    TEST("ADDX.W", "D2, D7",          0157502); // ADDX.W
    TEST("ADDX.W", "-(A2), -(A7)",    0157512); // ADDX.L
    TEST("ADD.W", "D7, (A2)",         0157522);
    TEST("ADD.W", "D7, (A2)+",        0157532);
    TEST("ADD.W", "D7, -(A2)",        0157542);
    TEST("ADD.W", "D7, ($1234,A2)",   0157552, 0x1234);
    TEST("ADD.W", "D7, (18,A2,D3.L)", 0157562, 0x3812);
    ABST("ADD.W", "D7, (@A).W", "FFFE",   0157570, 0xFFFE);
    ABST("ADD.W", "D7, (@A).L", "123456", 0157571, 0x0012, 0x3456);
    UNKN(0157572); // D7, (*+$1234,PC)
    UNKN(0157573); // D7, (*+10,PC,A3.L)
    UNKN(0157574); // D7, #$1234
    TEST("ADDX.L", "D2, D7",          0157602); // ADDX.L
    TEST("ADDX.L", "-(A2), -(A7)",    0157612); // ADDX.L
    TEST("ADD.L", "D7, (A2)",         0157622);
    TEST("ADD.L", "D7, (A2)+",        0157632);
    TEST("ADD.L", "D7, -(A2)",        0157642);
    TEST("ADD.L", "D7, ($1234,A2)",   0157652, 0x1234);
    TEST("ADD.L", "D7, (18,A2,D3.L)", 0157662, 0x3812);
    ABST("ADD.L", "D7, (@A).W", "FFFC",   0157670, 0xFFFC);
    ABST("ADD.L", "D7, (@A).L", "123454", 0157671, 0x0012, 0x3454);
    UNKN(0157672); // D7, (*+$1234,PC)
    UNKN(0157673); // D7, (*+10,PC,D3.L)
    UNKN(0157674); // D7, #$12345678

    // ADD src,Dn: 015|Dn|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("ADD.B", "D2, D7",             0157002);
    UNKN(0157012); // A2, D7
    TEST("ADD.B", "(A2), D7",           0157022);
    TEST("ADD.B", "(A2)+, D7",          0157032);
    TEST("ADD.B", "-(A2), D7",          0157042);
    TEST("ADD.B", "($1234,A2), D7",     0157052, 0x1234);
    TEST("ADD.B", "(18,A2,D3.L), D7",   0157062, 0x3812);
    ABST("ADD.B", "(@A).W, D7", "FFFF",   0157070, 0xFFFF);
    ABST("ADD.B", "(@A).L, D7", "123456", 0157071, 0x0012, 0x3456);
    TEST("ADD.B", "(*+$1234,PC), D7",   0157072, 0x1232);
    TEST("ADD.B", "(*+18,PC,D3.W), D7", 0157073, 0x3010);
    TEST("ADD.B", "#$34, D7",           0157074, 0x0034);
    TEST("ADD.W", "D2, D7",             0157102);
    TEST("ADD.W", "A2, D7",             0157112);
    TEST("ADD.W", "(A2), D7",           0157122);
    TEST("ADD.W", "(A2)+, D7",          0157132);
    TEST("ADD.W", "-(A2), D7",          0157142);
    TEST("ADD.W", "($1234,A2), D7",     0157152, 0x1234);
    TEST("ADD.W", "(18,A2,D3.L), D7",   0157162, 0x3812);
    ABST("ADD.W", "(@A).W, D7", "FFFE",   0157170, 0xFFFE);
    ABST("ADD.W", "(@A).L, D7", "123456", 0157171, 0x0012, 0x3456);
    TEST("ADD.W", "(*+$1234,PC), D7",   0157172, 0x1232);
    TEST("ADD.W", "(*+18,PC,D3.W), D7", 0157173, 0x3010);
    TEST("ADD.W", "#$0034, D7",         0157174, 0x0034);
    TEST("ADD.L", "D2, D7",             0157202);
    TEST("ADD.L", "A2, D7",             0157212);
    TEST("ADD.L", "(A2), D7",           0157222);
    TEST("ADD.L", "(A2)+, D7",          0157232);
    TEST("ADD.L", "-(A2), D7",          0157242);
    TEST("ADD.L", "($1234,A2), D7",     0157252, 0x1234);
    TEST("ADD.L", "(18,A2,D3.L), D7",   0157262, 0x3812);
    ABST("ADD.L", "(@A).W, D7", "FFFC",   0157270, 0xFFFC);
    ABST("ADD.L", "(@A).L, D7", "123454", 0157271, 0x0012, 0x3454);
    TEST("ADD.L", "(*+$1234,PC), D7",   0157272, 0x1232);
    TEST("ADD.L", "(*+18,PC,D3.W), D7", 0157273, 0x3010);
    TEST("ADD.L", "#$00345678, D7",     0157274, 0x0034, 0x5678);

    // ADDA src,An: 015|An|Sz|M|Rn, Sz:W=3/L=7
    TEST("ADDA.W", "D2, A6",             0156302);
    TEST("ADDA.W", "A2, A6",             0156312);
    TEST("ADDA.W", "(A2), A6",           0156322);
    TEST("ADDA.W", "(A2)+, A6",          0156332);
    TEST("ADDA.W", "-(A2), A6",          0156342);
    TEST("ADDA.W", "($1234,A2), A6",     0156352, 0x1234);
    TEST("ADDA.W", "(18,A2,D3.L), A6",   0156362, 0x3812);
    ABST("ADDA.W", "(@A).W, A6", "FFFE",   0156370, 0xFFFE);
    ABST("ADDA.W", "(@A).L, A6", "123456", 0156371, 0x0012, 0x3456);
    TEST("ADDA.W", "(*+$1234,PC), A6",   0156372, 0x1232);
    TEST("ADDA.W", "(*+18,PC,D3.W), A6", 0156373, 0x3010);
    TEST("ADDA.W", "#$0034, A6",         0156374, 0x0034);
    TEST("ADDA.L", "D2, A6",             0156702);
    TEST("ADDA.L", "A2, A6",             0156712);
    TEST("ADDA.L", "(A2), A6",           0156722);
    TEST("ADDA.L", "(A2)+, A6",          0156732);
    TEST("ADDA.L", "-(A2), A6",          0156742);
    TEST("ADDA.L", "($1234,A2), A6",     0156752, 0x1234);
    TEST("ADDA.L", "(18,A2,D3.L), A6",   0156762, 0x3812);
    ABST("ADDA.L", "(@A).W, A6", "FFFC",   0156770, 0xFFFC);
    ABST("ADDA.L", "(@A).L, A6", "123454", 0156771, 0x0012, 0x3454);
    TEST("ADDA.L", "(*+$1234,PC), A6",   0156772, 0x1232);
    TEST("ADDA.L", "(*+18,PC,D3.W), A6", 0156773, 0x3010);
    TEST("ADDA.L", "#$00345678, A6",     0156774, 0x0034, 0x5678);

    // ADDI #xxxx,dst: 0003|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("ADDI.B", "#18, D2",           0003002, 0x0012);
    UNKN(0003012); // #18, A2
    TEST("ADDI.B", "#18, (A2)",         0003022, 0x0012);
    TEST("ADDI.B", "#18, (A2)+",        0003032, 0x0012);
    TEST("ADDI.B", "#18, -(A2)",        0003042, 0x0012);
    TEST("ADDI.B", "#18, ($1234,A2)",   0003052, 0x0012, 0x1234);
    TEST("ADDI.B", "#18, (18,A2,D3.W)", 0003062, 0x0012, 0x3012);
    ABST("ADDI.B", "#18, (@A).W", "1234",   0003070, 0x0012, 0x1234);
    ABST("ADDI.B", "#18, (@A).L", "123456", 0003071, 0x0012, 0x0012, 0x3456);
    UNKN(0003072); // #18, (*+$1234,PC)
    UNKN(0003073); // #18, (*+10,PC,D3.W)
    UNKN(0003074); // #18, #$34
    TEST("ADDI.W", "#$5678, D2",           0003102, 0x5678);
    UNKN(0003112); // #$5678, A2
    TEST("ADDI.W", "#$5678, (A2)",         0003122, 0x5678);
    TEST("ADDI.W", "#$5678, (A2)+",        0003132, 0x5678);
    TEST("ADDI.W", "#$5678, -(A2)",        0003142, 0x5678);
    TEST("ADDI.W", "#$5678, ($1234,A2)",   0003152, 0x5678, 0x1234);
    TEST("ADDI.W", "#$5678, (18,A2,D3.W)", 0003162, 0x5678, 0x3012);
    ABST("ADDI.W", "#$5678, (@A).W", "1234",   0003170, 0x5678, 0x1234);
    ABST("ADDI.W", "#$5678, (@A).L", "123456", 0003171, 0x5678, 0x0012, 0x3456);
    UNKN(0003172); // #$5678, (*+$1234,PC)
    UNKN(0003173); // #$5678, (*+10,PC,D3.W)
    UNKN(0003174); // #$5678, #$1234
    TEST("ADDI.L", "#$3456789A, D2",           0003202, 0x3456, 0x789A);
    UNKN(0003212); // #$3456789A, A2
    TEST("ADDI.L", "#$3456789A, (A2)",         0003222, 0x3456, 0x789A);
    TEST("ADDI.L", "#$3456789A, (A2)+",        0003232, 0x3456, 0x789A);
    TEST("ADDI.L", "#$3456789A, -(A2)",        0003242, 0x3456, 0x789A);
    TEST("ADDI.L", "#$3456789A, ($1234,A2)",   0003252, 0x3456, 0x789A, 0x1234);
    TEST("ADDI.L", "#$3456789A, (18,A2,D3.W)", 0003262, 0x3456, 0x789A, 0x3012);
    ABST("ADDI.L", "#$3456789A, (@A).W", "1234",   0003270, 0x3456, 0x789A, 0x1234);
    ABST("ADDI.L", "#$34567898, (@A).L", "123454", 0003271, 0x3456, 0x7898, 0x0012, 0x3454);
    UNKN(0003272); // #$3456789A, (*+$1234,PC)
    UNKN(0003273); // #$3456789A, (*+10,PC,D3.W)
    UNKN(0003274); // #$3456789A, #$12345678

    // ADDQ #nn,dst: 005|nn|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("ADDQ.B", "#8, D2",           0050002);
    UNKN(0050012); // #8, A2
    TEST("ADDQ.B", "#8, (A2)",         0050022);
    TEST("ADDQ.B", "#8, (A2)+",        0050032);
    TEST("ADDQ.B", "#8, -(A2)",        0050042);
    TEST("ADDQ.B", "#8, ($1234,A2)",   0050052, 0x1234);
    TEST("ADDQ.B", "#8, (18,A2,D3.W)", 0050062, 0x3012);
    ABST("ADDQ.B", "#8, (@A).W", "1234",   0050070, 0x1234);
    ABST("ADDQ.B", "#8, (@A).L", "123456", 0050071, 0x0012, 0x3456);
    UNKN(0050072); // #8, (*+$1234,PC)
    UNKN(0050073); // #8, (*+10,PC,D3.W)
    UNKN(0050074); // #8, #$34
    TEST("ADDQ.W", "#8, D2",           0050102);
    TEST("ADDQ.W", "#8, A2",           0050112);
    TEST("ADDQ.W", "#8, (A2)",         0050122);
    TEST("ADDQ.W", "#8, (A2)+",        0050132);
    TEST("ADDQ.W", "#8, -(A2)",        0050142);
    TEST("ADDQ.W", "#8, ($1234,A2)",   0050152, 0x1234);
    TEST("ADDQ.W", "#8, (18,A2,D3.W)", 0050162, 0x3012);
    ABST("ADDQ.W", "#8, (@A).W", "1234",   0050170, 0x1234);
    ABST("ADDQ.W", "#8, (@A).L", "123456", 0050171, 0x0012, 0x3456);
    UNKN(0050172); // #8, (*+$1234,PC)
    UNKN(0050173); // #8, (*+10,PC,D3.W)
    UNKN(0050174); // #8, #$1234
    TEST("ADDQ.L", "#8, D2",           0050202);
    TEST("ADDQ.L", "#8, A2",           0050212);
    TEST("ADDQ.L", "#8, (A2)",         0050222);
    TEST("ADDQ.L", "#8, (A2)+",        0050232);
    TEST("ADDQ.L", "#8, -(A2)",        0050242);
    TEST("ADDQ.L", "#8, ($1234,A2)",   0050252, 0x1234);
    TEST("ADDQ.L", "#8, (18,A2,D3.W)", 0050262, 0x3012);
    ABST("ADDQ.L", "#8, (@A).W", "1234",   0050270, 0x1234);
    ABST("ADDQ.L", "#8, (@A).L", "123454", 0050271, 0x0012, 0x3454);
    UNKN(0050272); // #8, (*+$1234,PC)
    UNKN(0050273); // #8, (*+10,PC,D3.W)
    UNKN(0050274); // #8, #$12345678

    // ADDX Dx,Dy: 015|Dy|Sz|0|Dx, Sz:B=4/W=5/L=6
    TEST("ADDX.B", "D2, D3", 0153402);
    TEST("ADDX.W", "D2, D3", 0153502);
    TEST("ADDX.L", "D2, D3", 0153602);

    // ADDX -(Ax), -(Ay), 015|Ay|Sz|1|Ax, Sz:B=4/W=5/L=6
    TEST("ADDX.B", "-(A2), -(A3)", 0153412);
    TEST("ADDX.W", "-(A2), -(A3)", 0153512);
    TEST("ADDX.L", "-(A2), -(A3)", 0153612);

    // CLR dst: 0041|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("CLR.B", "D2",           0041002);
    UNKN(0041012); // A2
    TEST("CLR.B", "(A2)",         0041022);
    TEST("CLR.B", "(A2)+",        0041032);
    TEST("CLR.B", "-(A2)",        0041042);
    TEST("CLR.B", "($1234,A2)",   0041052, 0x1234);
    TEST("CLR.B", "(18,A2,D3.W)", 0041062, 0x3012);
    ABST("CLR.B", "(@A).W", "1234",   0041070, 0x1234);
    ABST("CLR.B", "(@A).L", "123456", 0041071, 0x0012, 0x3456);
    UNKN(0041072); // (*+$1234,PC)
    UNKN(0041073); // (*+10,PC,D3.W)
    UNKN(0041074); // #$34
    TEST("CLR.W", "D2",           0041102);
    UNKN(0041112); // A2
    TEST("CLR.W", "(A2)",         0041122);
    TEST("CLR.W", "(A2)+",        0041132);
    TEST("CLR.W", "-(A2)",        0041142);
    TEST("CLR.W", "($1234,A2)",   0041152, 0x1234);
    TEST("CLR.W", "(18,A2,D3.W)", 0041162, 0x3012);
    ABST("CLR.W", "(@A).W", "1234",   0041170, 0x1234);
    ABST("CLR.W", "(@A).L", "123456", 0041171, 0x0012, 0x3456);
    UNKN(0041172); // (*+$1234,PC)
    UNKN(0041173); // (*+10,PC,D3.W)
    UNKN(0041174); // #$1234
    TEST("CLR.L", "D2",           0041202);
    UNKN(0041212); // A2
    TEST("CLR.L", "(A2)",         0041222);
    TEST("CLR.L", "(A2)+",        0041232);
    TEST("CLR.L", "-(A2)",        0041242);
    TEST("CLR.L", "($1234,A2)",   0041252, 0x1234);
    TEST("CLR.L", "(18,A2,D3.W)", 0041262, 0x3012);
    ABST("CLR.L", "(@A).W", "1234",   0041270, 0x1234);
    ABST("CLR.L", "(@A).L", "123454", 0041271, 0x0012, 0x3454);
    UNKN(0041272); // (*+$1234,PC)
    UNKN(0041273);; // (*+10,PC,D3.W)
    UNKN(0041274); // #$12345678

    // CMP src,Dn: 013|Dn|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("CMP.B", "D2, D7",             0137002);
    UNKN(0137012); // A2, D7
    TEST("CMP.B", "(A2), D7",           0137022);
    TEST("CMP.B", "(A2)+, D7",          0137032);
    TEST("CMP.B", "-(A2), D7",          0137042);
    TEST("CMP.B", "($1234,A2), D7",     0137052, 0x1234);
    TEST("CMP.B", "(18,A2,D3.L), D7",   0137062, 0x3812);
    ABST("CMP.B", "(@A).W, D7", "FFFF",   0137070, 0xFFFF);
    ABST("CMP.B", "(@A).L, D7", "123456", 0137071, 0x0012, 0x3456);
    TEST("CMP.B", "(*+$1234,PC), D7",   0137072, 0x1232);
    TEST("CMP.B", "(*+18,PC,D3.W), D7", 0137073, 0x3010);
    TEST("CMP.B", "#$34, D7",           0137074, 0x0034);
    TEST("CMP.W", "D2, D7",             0137102);
    TEST("CMP.W", "A2, D7",             0137112);
    TEST("CMP.W", "(A2), D7",           0137122);
    TEST("CMP.W", "(A2)+, D7",          0137132);
    TEST("CMP.W", "-(A2), D7",          0137142);
    TEST("CMP.W", "($1234,A2), D7",     0137152, 0x1234);
    TEST("CMP.W", "(18,A2,D3.L), D7",   0137162, 0x3812);
    ABST("CMP.W", "(@A).W, D7", "FFFE",   0137170, 0xFFFE);
    ABST("CMP.W", "(@A).L, D7", "123456", 0137171, 0x0012, 0x3456);
    TEST("CMP.W", "(*+$1234,PC), D7",   0137172, 0x1232);
    TEST("CMP.W", "(*+18,PC,D3.W), D7", 0137173, 0x3010);
    TEST("CMP.W", "#$0034, D7",         0137174, 0x0034);
    TEST("CMP.L", "D2, D7",             0137202);
    TEST("CMP.L", "A2, D7",             0137212);
    TEST("CMP.L", "(A2), D7",           0137222);
    TEST("CMP.L", "(A2)+, D7",          0137232);
    TEST("CMP.L", "-(A2), D7",          0137242);
    TEST("CMP.L", "($1234,A2), D7",     0137252, 0x1234);
    TEST("CMP.L", "(18,A2,D3.L), D7",   0137262, 0x3812);
    ABST("CMP.L", "(@A).W, D7", "FFFC",   0137270, 0xFFFC);
    ABST("CMP.L", "(@A).L, D7", "123454", 0137271, 0x0012, 0x3454);
    TEST("CMP.L", "(*+$1234,PC), D7",   0137272, 0x1232);
    TEST("CMP.L", "(*+18,PC,D3.W), D7", 0137273, 0x3010);
    TEST("CMP.L", "#$00345678, D7",     0137274, 0x0034, 0x5678);

    // CMPA src,An: 013|An|Sz|M|Rn, Sz:W=3/L=7
    TEST("CMPA.W", "D2, A6",             0136302);
    TEST("CMPA.W", "A2, A6",             0136312);
    TEST("CMPA.W", "(A2), A6",           0136322);
    TEST("CMPA.W", "(A2)+, A6",          0136332);
    TEST("CMPA.W", "-(A2), A6",          0136342);
    TEST("CMPA.W", "($1234,A2), A6",     0136352, 0x1234);
    TEST("CMPA.W", "(18,A2,D3.L), A6",   0136362, 0x3812);
    ABST("CMPA.W", "(@A).W, A6", "FFFE",   0136370, 0xFFFE);
    ABST("CMPA.W", "(@A).L, A6", "123456", 0136371, 0x0012, 0x3456);
    TEST("CMPA.W", "(*+$1234,PC), A6",   0136372, 0x1232);
    TEST("CMPA.W", "(*+18,PC,D3.W), A6", 0136373, 0x3010);
    TEST("CMPA.W", "#$0034, A6",         0136374, 0x0034);
    TEST("CMPA.L", "D2, A6",             0136702);
    TEST("CMPA.L", "A2, A6",             0136712);
    TEST("CMPA.L", "(A2), A6",           0136722);
    TEST("CMPA.L", "(A2)+, A6",          0136732);
    TEST("CMPA.L", "-(A2), A6",          0136742);
    TEST("CMPA.L", "($1234,A2), A6",     0136752, 0x1234);
    TEST("CMPA.L", "(18,A2,D3.L), A6",   0136762, 0x3812);
    ABST("CMPA.L", "(@A).W, A6", "FFFC",   0136770, 0xFFFC);
    ABST("CMPA.L", "(@A).L, A6", "123454", 0136771, 0x0012, 0x3454);
    TEST("CMPA.L", "(*+$1234,PC), A6",   0136772, 0x1232);
    TEST("CMPA.L", "(*+18,PC,D3.W), A6", 0136773, 0x3010);
    TEST("CMPA.L", "#$00345678, A6",     0136774, 0x0034, 0x5678);

    // CMPI #nn,dst: 0006|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("CMPI.B", "#18, D2",           0006002, 0x0012);
    UNKN(0006012); // #18, A2
    TEST("CMPI.B", "#18, (A2)",         0006022, 0x0012);
    TEST("CMPI.B", "#18, (A2)+",        0006032, 0x0012);
    TEST("CMPI.B", "#18, -(A2)",        0006042, 0x0012);
    TEST("CMPI.B", "#18, ($1234,A2)",   0006052, 0x0012, 0x1234);
    TEST("CMPI.B", "#18, (18,A2,D3.W)", 0006062, 0x0012, 0x3012);
    ABST("CMPI.B", "#18, (@A).W", "1234",   0006070, 0x0012, 0x1234);
    ABST("CMPI.B", "#18, (@A).L", "123456", 0006071, 0x0012, 0x0012, 0x3456);
    UNKN(0006072); // #18, (*+$1234,PC)
    UNKN(0006073); // #18, (*+10,PC,D3.W)
    UNKN(0006074); // #18, #$34
    TEST("CMPI.W", "#$5678, D2",           0006102, 0x5678);
    UNKN(0006112); // #$5678, A2
    TEST("CMPI.W", "#$5678, (A2)",         0006122, 0x5678);
    TEST("CMPI.W", "#$5678, (A2)+",        0006132, 0x5678);
    TEST("CMPI.W", "#$5678, -(A2)",        0006142, 0x5678);
    TEST("CMPI.W", "#$5678, ($1234,A2)",   0006152, 0x5678, 0x1234);
    TEST("CMPI.W", "#$5678, (18,A2,D3.W)", 0006162, 0x5678, 0x3012);
    ABST("CMPI.W", "#$5678, (@A).W", "1234",   0006170, 0x5678, 0x1234);
    ABST("CMPI.W", "#$5678, (@A).L", "123456", 0006171, 0x5678, 0x0012, 0x3456);
    UNKN(0006172); // #$5678, (*+$1234,PC)
    UNKN(0006173); // #$5678, (*+10,PC,D3.W)
    UNKN(0006174); // #$5678, #$1234
    TEST("CMPI.L", "#$3456789A, D2",           0006202, 0x3456, 0x789A);
    UNKN(0006212); // #$3456789A, A2
    TEST("CMPI.L", "#$3456789A, (A2)",         0006222, 0x3456, 0x789A);
    TEST("CMPI.L", "#$3456789A, (A2)+",        0006232, 0x3456, 0x789A);
    TEST("CMPI.L", "#$3456789A, -(A2)",        0006242, 0x3456, 0x789A);
    TEST("CMPI.L", "#$3456789A, ($1234,A2)",   0006252, 0x3456, 0x789A, 0x1234);
    TEST("CMPI.L", "#$3456789A, (18,A2,D3.W)", 0006262, 0x3456, 0x789A, 0x3012);
    ABST("CMPI.L", "#$3456789A, (@A).W", "1234",   0006270, 0x3456, 0x789A, 0x1234);
    ABST("CMPI.L", "#$3456789A, (@A).L", "123454", 0006271, 0x3456, 0x789A, 0x0012, 0x3454);
    UNKN(0006272); // #$3456789A, (*+$1234,PC)
    UNKN(0006273); // #$3456789A, (*+10,PC,D3.W)
    UNKN(0006274); // #$3456789A, #$12345678

    // CMPM (Ay)+, (Ax)+: 013|Ax|Sz|1|Ay, Sz:B=4/W=5/L=6
    TEST("CMPM.B", "(A2)+, (A7)+", 0137412);
    TEST("CMPM.W", "(A2)+, (A7)+", 0137512);
    TEST("CMPM.L", "(A2)+, (A7)+", 0137612);

    if (!firstGen()) {
        // CMP2 src,Rd, 00|Sz|3|M|Rn, Rd|00000, Sz:B=0/W=1/L=2
        UNKN(0000300);
        UNKN(0000310);
        TEST("CMP2.B", "(A2), D3",             0000322, 0030000);
        UNKN(0000332);
        UNKN(0000342);
        TEST("CMP2.B", "($1234,A2), A6",       0000352, 0160000, 0x1234);
        TEST("CMP2.B", "(18,A2,D3.L), D7",     0000362, 0070000, 0x3812);
        ABST("CMP2.B", "(@A).W, A0", "FFFE",   0000370, 0100000, 0xFFFE);
        ABST("CMP2.B", "(@A).L, D0", "123456", 0000371, 0000000, 0x0012, 0x3456);
        TEST("CMP2.B", "(*+$1234,PC), A1",     0000372, 0110000, 0x1230);
        TEST("CMP2.B", "(*+18,PC,D3.W), D2",   0000373, 0020000, 0x300E);
        UNKN(0000374);
        UNKN(0001300);
        UNKN(0001310);
        TEST("CMP2.W", "(A2), D3",             0001322, 0030000);
        UNKN(0001332);
        UNKN(0001342);
        TEST("CMP2.W", "($1234,A2), A6",       0001352, 0160000, 0x1234);
        TEST("CMP2.W", "(18,A2,D3.L), D7",     0001362, 0070000, 0x3812);
        ABST("CMP2.W", "(@A).W, A0", "FFFE",   0001370, 0100000, 0xFFFE);
        ABST("CMP2.W", "(@A).L, D0", "123456", 0001371, 0000000, 0x0012, 0x3456);
        TEST("CMP2.W", "(*+$1234,PC), A1",     0001372, 0110000, 0x1230);
        TEST("CMP2.W", "(*+18,PC,D3.W), D2",   0001373, 0020000, 0x300E);
        UNKN(0001374);
        UNKN(0002300);
        UNKN(0002310);
        TEST("CMP2.L", "(A2), D3",             0002322, 0030000);
        UNKN(0002332);
        UNKN(0002342);
        TEST("CMP2.L", "($1234,A2), A6",       0002352, 0160000, 0x1234);
        TEST("CMP2.L", "(18,A2,D3.L), D7",     0002362, 0070000, 0x3812);
        ABST("CMP2.L", "(@A).W, A0", "FFFE",   0002370, 0100000, 0xFFFE);
        ABST("CMP2.L", "(@A).L, D0", "123456", 0002371, 0000000, 0x0012, 0x3456);
        TEST("CMP2.L", "(*+$1234,PC), A1",     0002372, 0110000, 0x1230);
        TEST("CMP2.L", "(*+18,PC,D3.W), D2",   0002373, 0020000, 0x300E);
        UNKN(0002374);
    }

    // DIVS.W src,Dn: 010|Dn|7|M|Rn
    TEST("DIVS.W", "D2, D7",               0107702);
    UNKN(0107712); // A2, D7
    TEST("DIVS.W", "(A2), D7",             0107722);
    TEST("DIVS.W", "(A2)+, D7",            0107732);
    TEST("DIVS.W", "-(A2), D7",            0107742);
    TEST("DIVS.W", "($1234,A2), D7",       0107752, 0x1234);
    TEST("DIVS.W", "(18,A2,D3.L), D7",     0107762, 0x3812);
    ABST("DIVS.W", "(@A).W, D7", "FFFE",   0107770, 0xFFFE);
    ABST("DIVS.W", "(@A).L, D7", "123456", 0107771, 0x0012, 0x3456);
    TEST("DIVS.W", "(*+$1234,PC), D7",     0107772, 0x1232);
    TEST("DIVS.W", "(*+18,PC,D3.W), D7",   0107773, 0x3010);
    TEST("DIVS.W", "#$5678, D7",           0107774, 0x5678);

    if (!firstGen()) {
        // DIVS.L src,Dq: 00461|M|Rn, 0|Dq|400|Dq
        TEST("DIVS.L", "D2, D7",               0046102, 0074007);
        UNKN(0046112); // A2, D7
        TEST("DIVS.L", "(A2), D7",             0046122, 0074007);
        TEST("DIVS.L", "(A2)+, D7",            0046132, 0074007);
        TEST("DIVS.L", "-(A2), D7",            0046142, 0074007);
        TEST("DIVS.L", "($1234,A2), D7",       0046152, 0074007, 0x1234);
        TEST("DIVS.L", "(18,A2,D3.L), D7",     0046162, 0074007, 0x3812);
        ABST("DIVS.L", "(@A).W, D7", "FFFE",   0046170, 0074007, 0xFFFE);
        ABST("DIVS.L", "(@A).L, D7", "123456", 0046171, 0074007, 0x0012, 0x3456);
        TEST("DIVS.L", "(*+$1234,PC), D7",     0046172, 0074007, 0x1230);
        TEST("DIVS.L", "(*+18,PC,D3.W), D7",   0046173, 0074007, 0x300E);
        TEST("DIVS.L", "#$12345678, D7",       0046174, 0074007, 0x1234, 0x5678);
        // DIVSL.L src,Dr:Dq: 00461|M|Rn, 0|Dq|400|Dr
        TEST("DIVSL.L", "D2, D6:D7",               0046102, 0074006);
        UNKN(0046112); // A2, D7
        TEST("DIVSL.L", "(A2), D6:D7",             0046122, 0074006);
        TEST("DIVSL.L", "(A2)+, D6:D7",            0046132, 0074006);
        TEST("DIVSL.L", "-(A2), D6:D7",            0046142, 0074006);
        TEST("DIVSL.L", "($1234,A2), D6:D7",       0046152, 0074006, 0x1234);
        TEST("DIVSL.L", "(18,A2,D3.L), D6:D7",     0046162, 0074006, 0x3812);
        ABST("DIVSL.L", "(@A).W, D6:D7", "FFFE",   0046170, 0074006, 0xFFFE);
        ABST("DIVSL.L", "(@A).L, D6:D7", "123456", 0046171, 0074006, 0x0012, 0x3456);
        TEST("DIVSL.L", "(*+$1234,PC), D6:D7",     0046172, 0074006, 0x1230);
        TEST("DIVSL.L", "(*+18,PC,D3.W), D6:D7",   0046173, 0074006, 0x300E);
        TEST("DIVSL.L", "#$12345678, D6:D7",       0046174, 0074006, 0x1234, 0x5678);
        // DIVS.L src,Dr:Dq: 00461|M|Rn, 0|Dq|600|Dr
        TEST("DIVS.L", "D2, D6:D7",               0046102, 0076006);
        UNKN(0046112); // A2, D7
        TEST("DIVS.L", "(A2), D6:D7",             0046122, 0076006);
        TEST("DIVS.L", "(A2)+, D6:D7",            0046132, 0076006);
        TEST("DIVS.L", "-(A2), D6:D7",            0046142, 0076006);
        TEST("DIVS.L", "($1234,A2), D6:D7",       0046152, 0076006, 0x1234);
        TEST("DIVS.L", "(18,A2,D3.L), D6:D7",     0046162, 0076006, 0x3812);
        ABST("DIVS.L", "(@A).W, D6:D7", "FFFE",   0046170, 0076006, 0xFFFE);
        ABST("DIVS.L", "(@A).L, D6:D7", "123456", 0046171, 0076006, 0x0012, 0x3456);
        TEST("DIVS.L", "(*+$1234,PC), D6:D7",     0046172, 0076006, 0x1230);
        TEST("DIVS.L", "(*+18,PC,D3.W), D6:D7",   0046173, 0076006, 0x300E);
        TEST("DIVS.L", "#$12345678, D6:D7",       0046174, 0076006, 0x1234, 0x5678);
    }

    // DIVU.W src,Dn: 010|Dn|3|M|Rn
    TEST("DIVU.W", "D2, D7",             0107302);
    UNKN(0107312); // A2, D7
    TEST("DIVU.W", "(A2), D7",           0107322);
    TEST("DIVU.W", "(A2)+, D7",          0107332);
    TEST("DIVU.W", "-(A2), D7",          0107342);
    TEST("DIVU.W", "($1234,A2), D7",     0107352, 0x1234);
    TEST("DIVU.W", "(18,A2,D3.L), D7",   0107362, 0x3812);
    ABST("DIVU.W", "(@A).W, D7", "FFFE",   0107370, 0xFFFE);
    ABST("DIVU.W", "(@A).L, D7", "123456", 0107371, 0x0012, 0x3456);
    TEST("DIVU.W", "(*+$1234,PC), D7",   0107372, 0x1232);
    TEST("DIVU.W", "(*+18,PC,D3.W), D7", 0107373, 0x3010);
    TEST("DIVU.W", "#$5678, D7",         0107374, 0x5678);

    if (!firstGen()) {
        // DIVU.L src,Dq: 00461|M|Rn, 0|Dq|000|Dq
        TEST("DIVU.L", "D2, D7",               0046102, 0070007);
        UNKN(0046112); // A2, D7
        TEST("DIVU.L", "(A2), D7",             0046122, 0070007);
        TEST("DIVU.L", "(A2)+, D7",            0046132, 0070007);
        TEST("DIVU.L", "-(A2), D7",            0046142, 0070007);
        TEST("DIVU.L", "($1234,A2), D7",       0046152, 0070007, 0x1234);
        TEST("DIVU.L", "(18,A2,D3.L), D7",     0046162, 0070007, 0x3812);
        ABST("DIVU.L", "(@A).W, D7", "FFFE",   0046170, 0070007, 0xFFFE);
        ABST("DIVU.L", "(@A).L, D7", "123456", 0046171, 0070007, 0x0012, 0x3456);
        TEST("DIVU.L", "(*+$1234,PC), D7",     0046172, 0070007, 0x1230);
        TEST("DIVU.L", "(*+18,PC,D3.W), D7",   0046173, 0070007, 0x300E);
        TEST("DIVU.L", "#$12345678, D7",       0046174, 0070007, 0x1234, 0x5678);
        // DIVUL.L src,Dr:Dq: 00461|M|Rn, 0|Dq|000|Dr
        TEST("DIVUL.L", "D2, D6:D7",               0046102, 0070006);
        UNKN(0046112); // A2, D7
        TEST("DIVUL.L", "(A2), D6:D7",             0046122, 0070006);
        TEST("DIVUL.L", "(A2)+, D6:D7",            0046132, 0070006);
        TEST("DIVUL.L", "-(A2), D6:D7",            0046142, 0070006);
        TEST("DIVUL.L", "($1234,A2), D6:D7",       0046152, 0070006, 0x1234);
        TEST("DIVUL.L", "(18,A2,D3.L), D6:D7",     0046162, 0070006, 0x3812);
        ABST("DIVUL.L", "(@A).W, D6:D7", "FFFE",   0046170, 0070006, 0xFFFE);
        ABST("DIVUL.L", "(@A).L, D6:D7", "123456", 0046171, 0070006, 0x0012, 0x3456);
        TEST("DIVUL.L", "(*+$1234,PC), D6:D7",     0046172, 0070006, 0x1230);
        TEST("DIVUL.L", "(*+18,PC,D3.W), D6:D7",   0046173, 0070006, 0x300E);
        TEST("DIVUL.L", "#$12345678, D6:D7",       0046174, 0070006, 0x1234, 0x5678);
        // DIVU.L src,Dr:Dq: 00461|M|Rn, 0|Dq|200|Dr
        TEST("DIVU.L", "D2, D6:D7",               0046102, 0072006);
        UNKN(0046112); // A2, D7
        TEST("DIVU.L", "(A2), D6:D7",             0046122, 0072006);
        TEST("DIVU.L", "(A2)+, D6:D7",            0046132, 0072006);
        TEST("DIVU.L", "-(A2), D6:D7",            0046142, 0072006);
        TEST("DIVU.L", "($1234,A2), D6:D7",       0046152, 0072006, 0x1234);
        TEST("DIVU.L", "(18,A2,D3.L), D6:D7",     0046162, 0072006, 0x3812);
        ABST("DIVU.L", "(@A).W, D6:D7", "FFFE",   0046170, 0072006, 0xFFFE);
        ABST("DIVU.L", "(@A).L, D6:D7", "123456", 0046171, 0072006, 0x0012, 0x3456);
        TEST("DIVU.L", "(*+$1234,PC), D6:D7",     0046172, 0072006, 0x1230);
        TEST("DIVU.L", "(*+18,PC,D3.W), D6:D7",   0046173, 0072006, 0x300E);
        TEST("DIVU.L", "#$12345678, D6:D7",       0046174, 0072006, 0x1234, 0x5678);
    }

    // EXT Dn: 0044|Sz|0|Dn, Sz:W=2/L=3
    TEST("EXT.W", "D2", 0044202);
    TEST("EXT.L", "D2", 0044302);

    // MULS.W src,Dn: 014|Dn|7|M|Rn
    TEST("MULS.W", "D2, D7",               0147702);
    UNKN(0147712); // A2, D7
    TEST("MULS.W", "(A2), D7",             0147722);
    TEST("MULS.W", "(A2)+, D7",            0147732);
    TEST("MULS.W", "-(A2), D7",            0147742);
    TEST("MULS.W", "($1234,A2), D7",       0147752, 0x1234);
    TEST("MULS.W", "(18,A2,D3.L), D7",     0147762, 0x3812);
    ABST("MULS.W", "(@A).W, D7", "FFFE",   0147770, 0xFFFE);
    ABST("MULS.W", "(@A).L, D7", "123456", 0147771, 0x0012, 0x3456);
    TEST("MULS.W", "(*+$1234,PC), D7",     0147772, 0x1232);
    TEST("MULS.W", "(*+18,PC,D3.W), D7",   0147773, 0x3010);
    TEST("MULS.W", "#$5678, D7",           0147774, 0x5678);

    if (!firstGen()) {
        // MULS.L src,Dl: 00460|M|Rn, 0|Dl|4000
        TEST("MULS.L", "D2, D7",               0046002, 074000);
        UNKN(0046012); // A2, D7
        TEST("MULS.L", "(A2), D6",             0046022, 064000);
        TEST("MULS.L", "(A2)+, D5",            0046032, 054000);
        TEST("MULS.L", "-(A2), D4",            0046042, 044000);
        TEST("MULS.L", "($1234,A2), D7",       0046052, 074000, 0x1234);
        TEST("MULS.L", "(18,A2,D3.L), D7",     0046062, 074000, 0x3812);
        ABST("MULS.L", "(@A).W, D7", "FFFE",   0046070, 074000, 0xFFFE);
        ABST("MULS.L", "(@A).L, D7", "123456", 0046071, 074000, 0x0012, 0x3456);
        TEST("MULS.L", "(*+$1234,PC), D7",     0046072, 074000, 0x1230);
        TEST("MULS.L", "(*+18,PC,D3.W), D7",   0046073, 074000, 0x300E);
        TEST("MULS.L", "#$12345678, D7",       0046074, 074000, 0x1234, 0x5678);
        // MULS.L src,Dh:Dl: 00460|M|Rn, 0|Dl|600Dh
        TEST("MULS.L", "D2, D6:D7",               0046002, 076006);
        UNKN(0046012); // A2, D7
        TEST("MULS.L", "(A2), D6:D7",             0046022, 076006);
        TEST("MULS.L", "(A2)+, D6:D7",            0046032, 076006);
        TEST("MULS.L", "-(A2), D6:D7",            0046042, 076006);
        TEST("MULS.L", "($1234,A2), D6:D7",       0046052, 076006, 0x1234);
        TEST("MULS.L", "(18,A2,D3.L), D6:D7",     0046062, 076006, 0x3812);
        ABST("MULS.L", "(@A).W, D6:D7", "FFFE",   0046070, 076006, 0xFFFE);
        ABST("MULS.L", "(@A).L, D6:D7", "123456", 0046071, 076006, 0x0012, 0x3456);
        TEST("MULS.L", "(*+$1234,PC), D6:D7",     0046072, 076006, 0x1230);
        TEST("MULS.L", "(*+18,PC,D3.W), D6:D7",   0046073, 076006, 0x300E);
        TEST("MULS.L", "#$12345678, D6:D7",       0046074, 076006, 0x1234, 0x5678);
    }

    // MULU src,Dn: 014|Dn|3|M|Rn
    TEST("MULU.W", "D2, D7",             0147302);
    UNKN(0147312); // A2, D7
    TEST("MULU.W", "(A2), D7",           0147322);
    TEST("MULU.W", "(A2)+, D7",          0147332);
    TEST("MULU.W", "-(A2), D7",          0147342);
    TEST("MULU.W", "($1234,A2), D7",     0147352, 0x1234);
    TEST("MULU.W", "(18,A2,D3.L), D7",   0147362, 0x3812);
    ABST("MULU.W", "(@A).W, D7", "FFFE",   0147370, 0xFFFE);
    ABST("MULU.W", "(@A).L, D7", "123456", 0147371, 0x0012, 0x3456);
    TEST("MULU.W", "(*+$1234,PC), D7",   0147372, 0x1232);
    TEST("MULU.W", "(*+18,PC,D3.W), D7", 0147373, 0x3010);
    TEST("MULU.W", "#$5678, D7",         0147374, 0x5678);

    if (!firstGen()) {
        // MULU.L src,Dl: 00460|M|Rn, 0|Dl|0000
        TEST("MULU.L", "D2, D7",               0046002, 070000);
        UNKN(0046012); // A2, D7
        TEST("MULU.L", "(A2), D6",             0046022, 060000);
        TEST("MULU.L", "(A2)+, D5",            0046032, 050000);
        TEST("MULU.L", "-(A2), D4",            0046042, 040000);
        TEST("MULU.L", "($1234,A2), D7",       0046052, 070000, 0x1234);
        TEST("MULU.L", "(18,A2,D3.L), D7",     0046062, 070000, 0x3812);
        ABST("MULU.L", "(@A).W, D7", "FFFE",   0046070, 070000, 0xFFFE);
        ABST("MULU.L", "(@A).L, D7", "123456", 0046071, 070000, 0x0012, 0x3456);
        TEST("MULU.L", "(*+$1234,PC), D7",     0046072, 070000, 0x1230);
        TEST("MULU.L", "(*+18,PC,D3.W), D7",   0046073, 070000, 0x300E);
        TEST("MULU.L", "#$12345678, D7",       0046074, 070000, 0x1234, 0x5678);
        // MULU.L src,Dh:Dl: 00460|M|Rn, 0|Dl|200Dh
        TEST("MULU.L", "D2, D6:D7",               0046002, 072006);
        UNKN(0046012); // A2, D7
        TEST("MULU.L", "(A2), D6:D7",             0046022, 072006);
        TEST("MULU.L", "(A2)+, D6:D7",            0046032, 072006);
        TEST("MULU.L", "-(A2), D6:D7",            0046042, 072006);
        TEST("MULU.L", "($1234,A2), D6:D7",       0046052, 072006, 0x1234);
        TEST("MULU.L", "(18,A2,D3.L), D6:D7",     0046062, 072006, 0x3812);
        ABST("MULU.L", "(@A).W, D6:D7", "FFFE",   0046070, 072006, 0xFFFE);
        ABST("MULU.L", "(@A).L, D6:D7", "123456", 0046071, 072006, 0x0012, 0x3456);
        TEST("MULU.L", "(*+$1234,PC), D6:D7",     0046072, 072006, 0x1230);
        TEST("MULU.L", "(*+18,PC,D3.W), D6:D7",   0046073, 072006, 0x300E);
        TEST("MULU.L", "#$12345678, D6:D7",       0046074, 072006, 0x1234, 0x5678);
    }

    // NEG dst: 0042|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("NEG.B", "D2",           0042002);
    UNKN(0042012); // A2
    TEST("NEG.B", "(A2)",         0042022);
    TEST("NEG.B", "(A2)+",        0042032);
    TEST("NEG.B", "-(A2)",        0042042);
    TEST("NEG.B", "($1234,A2)",   0042052, 0x1234);
    TEST("NEG.B", "(18,A2,D3.W)", 0042062, 0x3012);
    ABST("NEG.B", "(@A).W", "1234",   0042070, 0x1234);
    ABST("NEG.B", "(@A).L", "123456", 0042071, 0x0012, 0x3456);
    UNKN(0042072); // (*+$1234,PC)
    UNKN(0042073); // (*+10,PC,D3.W)
    UNKN(0042074); // #$34
    TEST("NEG.W", "D2",           0042102);
    UNKN(0042112); // A2
    TEST("NEG.W", "(A2)",         0042122);
    TEST("NEG.W", "(A2)+",        0042132);
    TEST("NEG.W", "-(A2)",        0042142);
    TEST("NEG.W", "($1234,A2)",   0042152, 0x1234);
    TEST("NEG.W", "(18,A2,D3.W)", 0042162, 0x3012);
    ABST("NEG.W", "(@A).W", "1234",   0042170, 0x1234);
    ABST("NEG.W", "(@A).L", "123456", 0042171, 0x0012, 0x3456);
    UNKN(0042172); // (*+$1234,PC)
    UNKN(0042173); // (*+10,PC,D3.W)
    UNKN(0042174); // #$1234
    TEST("NEG.L", "D2",           0042202);
    UNKN(0042212); // A2
    TEST("NEG.L", "(A2)",         0042222);
    TEST("NEG.L", "(A2)+",        0042232);
    TEST("NEG.L", "-(A2)",        0042242);
    TEST("NEG.L", "($1234,A2)",   0042252, 0x1234);
    TEST("NEG.L", "(18,A2,D3.W)", 0042262, 0x3012);
    ABST("NEG.L", "(@A).W", "1234",   0042270, 0x1234);
    ABST("NEG.L", "(@A).L", "123454", 0042271, 0x0012, 0x3454);
    UNKN(0042272); // (*+$1234,PC)
    UNKN(0042273); // (*+10,PC,D3.W)
    UNKN(0042274); // #$12345678

    // NEGX dst: 0040|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("NEGX.B", "D2",          0040002);
    UNKN(0040012); // A2
    TEST("NEGX.B", "(A2)",        0040022);
    TEST("NEGX.B", "(A2)+",       0040032);
    TEST("NEGX.B", "-(A2)",       0040042);
    TEST("NEGX.B", "($1234,A2)",  0040052, 0x1234);
    TEST("NEGX.B", "(18,A2,D3.W)",0040062, 0x3012);
    ABST("NEGX.B", "(@A).W", "1234",   0040070, 0x1234);
    ABST("NEGX.B", "(@A).L", "123456", 0040071, 0x0012, 0x3456);
    UNKN(0040072); // (*+$1234,PC)
    UNKN(0040073); // (*+10,PC,D3.W)
    UNKN(0040074); // #$34
    TEST("NEGX.W", "D2",          0040102);
    UNKN(0040112); // A2
    TEST("NEGX.W", "(A2)",        0040122);
    TEST("NEGX.W", "(A2)+",       0040132);
    TEST("NEGX.W", "-(A2)",       0040142);
    TEST("NEGX.W", "($1234,A2)",  0040152, 0x1234);
    TEST("NEGX.W", "(18,A2,D3.W)",0040162, 0x3012);
    ABST("NEGX.W", "(@A).W", "1234",   0040170, 0x1234);
    ABST("NEGX.W", "(@A).L", "123456", 0040171, 0x0012, 0x3456);
    UNKN(0040172); // (*+$1234,PC)
    UNKN(0040173); // (*+10,PC,D3.W)
    UNKN(0040174); // #$1234
    TEST("NEGX.L", "D2",          0040202);
    UNKN(0040212); // A2
    TEST("NEGX.L", "(A2)",        0040222);
    TEST("NEGX.L", "(A2)+",       0040232);
    TEST("NEGX.L", "-(A2)",       0040242);
    TEST("NEGX.L", "($1234,A2)",  0040252, 0x1234);
    TEST("NEGX.L", "(18,A2,D3.W)",0040262, 0x3012);
    ABST("NEGX.L", "(@A).W", "1234",   0040270, 0x1234);
    ABST("NEGX.L", "(@A).L", "123454", 0040271, 0x0012, 0x3454);
    UNKN(0040272); // (*+$1234,PC)
    UNKN(0040273); // (*+10,PC,D3.W)
    UNKN(0040274); // #$12345678

    // SUB src,Dn: 011|Dn|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("SUB.B", "D2, D7",             0117002);
    UNKN(0117012); // A2, D7
    TEST("SUB.B", "(A2), D7",           0117022);
    TEST("SUB.B", "(A2)+, D7",          0117032);
    TEST("SUB.B", "-(A2), D7",          0117042);
    TEST("SUB.B", "($1234,A2), D7",     0117052, 0x1234);
    TEST("SUB.B", "(18,A2,D3.L), D7",   0117062, 0x3812);
    ABST("SUB.B", "(@A).W, D7", "FFFF",   0117070, 0xFFFF);
    ABST("SUB.B", "(@A).L, D7", "123456", 0117071, 0x0012, 0x3456);
    TEST("SUB.B", "(*+$1234,PC), D7",   0117072, 0x1232);
    TEST("SUB.B", "(*+18,PC,D3.W), D7", 0117073, 0x3010);
    TEST("SUB.B", "#$34, D7",           0117074, 0x0034);
    TEST("SUB.W", "D2, D7",             0117102);
    TEST("SUB.W", "A2, D7",             0117112);
    TEST("SUB.W", "(A2), D7",           0117122);
    TEST("SUB.W", "(A2)+, D7",          0117132);
    TEST("SUB.W", "-(A2), D7",          0117142);
    TEST("SUB.W", "($1234,A2), D7",     0117152, 0x1234);
    TEST("SUB.W", "(18,A2,D3.L), D7",   0117162, 0x3812);
    ABST("SUB.W", "(@A).W, D7", "FFFE",   0117170, 0xFFFE);
    ABST("SUB.W", "(@A).L, D7", "123456", 0117171, 0x0012, 0x3456);
    TEST("SUB.W", "(*+$1234,PC), D7",   0117172, 0x1232);
    TEST("SUB.W", "(*+18,PC,D3.W), D7", 0117173, 0x3010);
    TEST("SUB.W", "#$0034, D7",         0117174, 0x0034);
    TEST("SUB.L", "D2, D7",             0117202);
    TEST("SUB.L", "A2, D7",             0117212);
    TEST("SUB.L", "(A2), D7",           0117222);
    TEST("SUB.L", "(A2)+, D7",          0117232);
    TEST("SUB.L", "-(A2), D7",          0117242);
    TEST("SUB.L", "($1234,A2), D7",     0117252, 0x1234);
    TEST("SUB.L", "(18,A2,D3.L), D7",   0117262, 0x3812);
    ABST("SUB.L", "(@A).W, D7", "FFFC",   0117270, 0xFFFC);
    ABST("SUB.L", "(@A).L, D7", "123454", 0117271, 0x0012, 0x3454);
    TEST("SUB.L", "(*+$1234,PC), D7",   0117272, 0x1232);
    TEST("SUB.L", "(*+18,PC,D3.W), D7", 0117273, 0x3010);
    TEST("SUB.L", "#$00345678, D7",     0117274, 0x0034, 0x5678);

    // SUB Dn,dst: 011|Dn|Sz|M|Rn, Sz:B=4/W=5/L=6
    TEST("SUBX.B", "D2, D7",          0117402); // SUBX.B
    TEST("SUBX.B", "-(A2), -(A7)",    0117412); // SUBX.B
    TEST("SUB.B", "D7, (A2)",         0117422);
    TEST("SUB.B", "D7, (A2)+",        0117432);
    TEST("SUB.B", "D7, -(A2)",        0117442);
    TEST("SUB.B", "D7, ($1234,A2)",   0117452, 0x1234);
    TEST("SUB.B", "D7, (18,A2,D3.L)", 0117462, 0x3812);
    ABST("SUB.B", "D7, (@A).W", "FFFF",   0117470, 0xFFFF);
    ABST("SUB.B", "D7, (@A).L", "123456", 0117471, 0x0012, 0x3456);
    UNKN(0117472); // D7, (*+$1234,PC)
    UNKN(0117473); // D7, (*+10,PC,D3.W)
    UNKN(0117474); // D7, #$34
    TEST("SUBX.W", "D2, D7",          0117502); // SUBX.W
    TEST("SUBX.W", "-(A2), -(A7)",    0117512); // SUBX.L
    TEST("SUB.W", "D7, (A2)",         0117522);
    TEST("SUB.W", "D7, (A2)+",        0117532);
    TEST("SUB.W", "D7, -(A2)",        0117542);
    TEST("SUB.W", "D7, ($1234,A2)",   0117552, 0x1234);
    TEST("SUB.W", "D7, (18,A2,D3.L)", 0117562, 0x3812);
    ABST("SUB.W", "D7, (@A).W", "FFFE",   0117570, 0xFFFE);
    ABST("SUB.W", "D7, (@A).L", "123456", 0117571, 0x0012, 0x3456);
    UNKN(0117572); // D7, (*+$1234,PC)
    UNKN(0117573); // D7, (*+10,PC,D3.W)
    UNKN(0117574); // D7, #$1234
    TEST("SUBX.L", "D2, D7",          0117602); // SUBX.L
    TEST("SUBX.L", "-(A2), -(A7)",    0117612); // SUBX.L
    TEST("SUB.L", "D7, (A2)",         0117622);
    TEST("SUB.L", "D7, (A2)+",        0117632);
    TEST("SUB.L", "D7, -(A2)",        0117642);
    TEST("SUB.L", "D7, ($1234,A2)",   0117652, 0x1234);
    TEST("SUB.L", "D7, (18,A2,D3.L)", 0117662, 0x3812);
    ABST("SUB.L", "D7, (@A).W", "FFFC",   0117670, 0xFFFC);
    ABST("SUB.L", "D7, (@A).L", "123454", 0117671, 0x0012, 0x3454);
    UNKN(0117672); // D7, (*+$1234,PC)
    UNKN(0117673); // D7, (*+10,PC,D3.W)
    UNKN(0117674); // D7, #$12345678

    // SUBA src,An: 011|An|Sz|M|Rn, Sz:W=3/L=7
    TEST("SUBA.W", "D2, A6",             0116302);
    TEST("SUBA.W", "A2, A6",             0116312);
    TEST("SUBA.W", "(A2), A6",           0116322);
    TEST("SUBA.W", "(A2)+, A6",          0116332);
    TEST("SUBA.W", "-(A2), A6",          0116342);
    TEST("SUBA.W", "($1234,A2), A6",     0116352, 0x1234);
    TEST("SUBA.W", "(18,A2,D3.L), A6",   0116362, 0x3812);
    ABST("SUBA.W", "(@A).W, A6", "FFFE",   0116370, 0xFFFE);
    ABST("SUBA.W", "(@A).L, A6", "123456", 0116371, 0x0012, 0x3456);
    TEST("SUBA.W", "(*+$1234,PC), A6",   0116372, 0x1232);
    TEST("SUBA.W", "(*+18,PC,D3.W), A6", 0116373, 0x3010);
    TEST("SUBA.W", "#$0034, A6",         0116374, 0x0034);
    TEST("SUBA.L", "D2, A6",             0116702);
    TEST("SUBA.L", "A2, A6",             0116712);
    TEST("SUBA.L", "(A2), A6",           0116722);
    TEST("SUBA.L", "(A2)+, A6",          0116732);
    TEST("SUBA.L", "-(A2), A6",          0116742);
    TEST("SUBA.L", "($1234,A2), A6",     0116752, 0x1234);
    TEST("SUBA.L", "(18,A2,D3.L), A6",   0116762, 0x3812);
    ABST("SUBA.L", "(@A).W, A6", "FFFC",   0116770, 0xFFFC);
    ABST("SUBA.L", "(@A).L, A6", "123454", 0116771, 0x0012, 0x3454);
    TEST("SUBA.L", "(*+$1234,PC), A6",   0116772, 0x1232);
    TEST("SUBA.L", "(*+18,PC,D3.W), A6", 0116773, 0x3010);
    TEST("SUBA.L", "#$00345678, A6",     0116774, 0x0034, 0x5678);

    // SUBI #nn,dst: 0002|Sz|M|Rn, SZ:B=0/W=1/L=2
    TEST("SUBI.B", "#18, D2",           0002002, 0x0012);
    UNKN(0002012); // #18, A2
    TEST("SUBI.B", "#18, (A2)",         0002022, 0x0012);
    TEST("SUBI.B", "#18, (A2)+",        0002032, 0x0012);
    TEST("SUBI.B", "#18, -(A2)",        0002042, 0x0012);
    TEST("SUBI.B", "#18, ($1234,A2)",   0002052, 0x0012, 0x1234);
    TEST("SUBI.B", "#18, (18,A2,D3.W)", 0002062, 0x0012, 0x3012);
    ABST("SUBI.B", "#18, (@A).W", "1234",   0002070, 0x0012, 0x1234);
    ABST("SUBI.B", "#18, (@A).L", "123456", 0002071, 0x0012, 0x0012, 0x3456);
    UNKN(0002072); // #18, (*+$1234,PC)
    UNKN(0002073); // #18, (*+10,PC,D3.W)
    UNKN(0002074); // #18, #$34
    TEST("SUBI.W", "#$5678, D2",           0002102, 0x5678);
    UNKN(0002112); // #$5678, A2
    TEST("SUBI.W", "#$5678, (A2)",         0002122, 0x5678);
    TEST("SUBI.W", "#$5678, (A2)+",        0002132, 0x5678);
    TEST("SUBI.W", "#$5678, -(A2)",        0002142, 0x5678);
    TEST("SUBI.W", "#$5678, ($1234,A2)",   0002152, 0x5678, 0x1234);
    TEST("SUBI.W", "#$5678, (18,A2,D3.W)", 0002162, 0x5678, 0x3012);
    ABST("SUBI.W", "#$5678, (@A).W", "1234",   0002170, 0x5678, 0x1234);
    ABST("SUBI.W", "#$5678, (@A).L", "123456", 0002171, 0x5678, 0x0012, 0x3456);
    UNKN(0002172); // #$5678, (*+$1234,PC)
    UNKN(0002173); // #$5678, (*+10,PC,D3.W)
    UNKN(0002174); // #$5678, #$1234
    TEST("SUBI.L", "#$3456789A, D2",           0002202, 0x3456, 0x789A);
    UNKN(0002212); // #$3456789A, A2
    TEST("SUBI.L", "#$3456789A, (A2)",         0002222, 0x3456, 0x789A);
    TEST("SUBI.L", "#$3456789A, (A2)+",        0002232, 0x3456, 0x789A);
    TEST("SUBI.L", "#$3456789A, -(A2)",        0002242, 0x3456, 0x789A);
    TEST("SUBI.L", "#$3456789A, ($1234,A2)",   0002252, 0x3456, 0x789A, 0x1234);
    TEST("SUBI.L", "#$3456789A, (18,A2,D3.W)", 0002262, 0x3456, 0x789A, 0x3012);
    ABST("SUBI.L", "#$3456789A, (@A).W", "1234",   0002270, 0x3456, 0x789A, 0x1234);
    ABST("SUBI.L", "#$3456789A, (@A).L", "123454", 0002271, 0x3456, 0x789A, 0x0012, 0x3454);
    UNKN(0002272); // #$3456789A, (*+$1234,PC)
    UNKN(0002273); // #$3456789A, (*+10,PC,D3.W)
    UNKN(0002274); // #$3456789A, #$12345678

    // SUBQ #nn,dst: 005|nn|Sz|M\En, Sz:B=4/W=5/L=6
    TEST("SUBQ.B", "#8, D2",           0050402);
    UNKN(0050412); // #8, A2
    TEST("SUBQ.B", "#8, (A2)",         0050422);
    TEST("SUBQ.B", "#8, (A2)+",        0050432);
    TEST("SUBQ.B", "#8, -(A2)",        0050442);
    TEST("SUBQ.B", "#8, ($1234,A2)",   0050452, 0x1234);
    TEST("SUBQ.B", "#8, (18,A2,D3.W)", 0050462, 0x3012);
    ABST("SUBQ.B", "#8, (@A).W", "1234",   0050470, 0x1234);
    ABST("SUBQ.B", "#8, (@A).L", "123456", 0050471, 0x0012, 0x3456);
    UNKN(0050472); // #8, (*+$1234,PC)
    UNKN(0050473); // #8, (*+10,PC,D3.W)
    UNKN(0050474); // #8, #$34
    TEST("SUBQ.W", "#8, D2",           0050502);
    TEST("SUBQ.W", "#8, A2",           0050512);
    TEST("SUBQ.W", "#8, (A2)",         0050522);
    TEST("SUBQ.W", "#8, (A2)+",        0050532);
    TEST("SUBQ.W", "#8, -(A2)",        0050542);
    TEST("SUBQ.W", "#8, ($1234,A2)",   0050552, 0x1234);
    TEST("SUBQ.W", "#8, (18,A2,D3.W)", 0050562, 0x3012);
    ABST("SUBQ.W", "#8, (@A).W", "1234",   0050570, 0x1234);
    ABST("SUBQ.W", "#8, (@A).L", "123456", 0050571, 0x0012, 0x3456);
    UNKN(0050572); // #8, (*+$1234,PC)
    UNKN(0050573); // #8, (*+10,PC,D3.W)
    UNKN(0050574); // #8, #$1234
    TEST("SUBQ.L", "#8, D2",           0050602);
    TEST("SUBQ.L", "#8, A2",           0050612);
    TEST("SUBQ.L", "#8, (A2)",         0050622);
    TEST("SUBQ.L", "#8, (A2)+",        0050632);
    TEST("SUBQ.L", "#8, -(A2)",        0050642);
    TEST("SUBQ.L", "#8, ($1234,A2)",   0050652, 0x1234);
    TEST("SUBQ.L", "#8, (18,A2,D3.W)", 0050662, 0x3012);
    ABST("SUBQ.L", "#8, (@A).W", "1234",   0050670, 0x1234);
    ABST("SUBQ.L", "#8, (@A).L", "123454", 0050671, 0x0012, 0x3454);
    UNKN(0050672); // #8, (*+$1234,PC)
    UNKN(0050673); // #8, (*+10,PC,D3.W)
    UNKN(0050674); // #8, #$12345678

    // SUBX Dx,Dy: 011|Dy|Sz|0|Dx, Sz:B=4/W=5/L=6
    TEST("SUBX.B", "D2, D3", 0113402);
    TEST("SUBX.W", "D2, D3", 0113502);
    TEST("SUBX.L", "D2, D3", 0113602);

    // SUBX -(Ax), -(Ay): 011|Ay|Sz|1|Ax, Sz:B=4/W=5/L=6
    TEST("SUBX.B", "-(A2), -(A3)", 0113412);
    TEST("SUBX.W", "-(A2), -(A3)", 0113512);
    TEST("SUBX.L", "-(A2), -(A3)", 0113612);
}

void test_logical() {
    // AND src,Dn: 014|Dn|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("AND.B", "D2, D7",             0147002);
    UNKN(0147012); // A2, D7
    TEST("AND.B", "(A2), D7",           0147022);
    TEST("AND.B", "(A2)+, D7",          0147032);
    TEST("AND.B", "-(A2), D7",          0147042);
    TEST("AND.B", "($1234,A2), D7",     0147052, 0x1234);
    TEST("AND.B", "(18,A2,D3.L), D7",   0147062, 0x3812);
    ABST("AND.B", "(@A).W, D7", "FFFF",   0147070, 0xFFFF);
    ABST("AND.B", "(@A).L, D7", "123456", 0147071, 0x0012, 0x3456);
    TEST("AND.B", "(*+$1234,PC), D7",   0147072, 0x1232);
    TEST("AND.B", "(*+18,PC,D3.W), D7", 0147073, 0x3010);
    TEST("AND.B", "#$34, D7",           0147074, 0x0034);
    TEST("AND.W", "D2, D7",             0147102);
    UNKN(0147112); // A2, D7
    TEST("AND.W", "(A2), D7",           0147122);
    TEST("AND.W", "(A2)+, D7",          0147132);
    TEST("AND.W", "-(A2), D7",          0147142);
    TEST("AND.W", "($1234,A2), D7",     0147152, 0x1234);
    TEST("AND.W", "(18,A2,D3.L), D7",   0147162, 0x3812);
    ABST("AND.W", "(@A).W, D7", "FFFE",   0147170, 0xFFFE);
    ABST("AND.W", "(@A).L, D7", "123456", 0147171, 0x0012, 0x3456);
    TEST("AND.W", "(*+$1234,PC), D7",   0147172, 0x1232);
    TEST("AND.W", "(*+18,PC,D3.W), D7", 0147173, 0x3010);
    TEST("AND.W", "#$0034, D7",         0147174, 0x0034);
    TEST("AND.L", "D2, D7",             0147202);
    UNKN(0147212); // A2, D7
    TEST("AND.L", "(A2), D7",           0147222);
    TEST("AND.L", "(A2)+, D7",          0147232);
    TEST("AND.L", "-(A2), D7",          0147242);
    TEST("AND.L", "($1234,A2), D7",     0147252, 0x1234);
    TEST("AND.L", "(18,A2,D3.L), D7",   0147262, 0x3812);
    ABST("AND.L", "(@A).W, D7", "FFFC",   0147270, 0xFFFC);
    ABST("AND.L", "(@A).L, D7", "123454", 0147271, 0x0012, 0x3454);
    TEST("AND.L", "(*+$1234,PC), D7",   0147272, 0x1232);
    TEST("AND.L", "(*+18,PC,D3.W), D7", 0147273, 0x3010);
    TEST("AND.L", "#$00345678, D7",     0147274, 0x0034, 0x5678);

    // AND Dn.dst: 014|Dn|Sz|M|Rn, Sz:B=4/W=5/L=6
    TEST("ABCD",  "D2, D7",           0147402); // ABCD
    TEST("ABCD",  "-(A2), -(A7)",     0147412); // ABCD
    TEST("AND.B", "D7, (A2)",         0147422);
    TEST("AND.B", "D7, (A2)+",        0147432);
    TEST("AND.B", "D7, -(A2)",        0147442);
    TEST("AND.B", "D7, ($1234,A2)",   0147452, 0x1234);
    TEST("AND.B", "D7, (18,A2,D3.L)", 0147462, 0x3812);
    ABST("AND.B", "D7, (@A).W", "FFFF",   0147470, 0xFFFF);
    ABST("AND.B", "D7, (@A).L", "123456", 0147471, 0x0012, 0x3456);
    UNKN(0147472); // D7, (*+$1234,PC)
    UNKN(0147473); // D7, (*+10,PC,D3.W)
    UNKN(0147474); // D7, #$34
    TEST("EXG",   "D7, D2",           0147502); // EXG
    TEST("EXG",   "A7, A2",           0147512); // EXG
    TEST("AND.W", "D7, (A2)",         0147522);
    TEST("AND.W", "D7, (A2)+",        0147532);
    TEST("AND.W", "D7, -(A2)",        0147542);
    TEST("AND.W", "D7, ($1234,A2)",   0147552, 0x1234);
    TEST("AND.W", "D7, (18,A2,D3.L)", 0147562, 0x3812);
    ABST("AND.W", "D7, (@A).W", "FFFE",   0147570, 0xFFFE);
    ABST("AND.W", "D7, (@A).L", "123456", 0147571, 0x0012, 0x3456);
    UNKN(0147572); // D7, (*+$1234,PC)
    UNKN(0147573); // D7, (*+10,PC,D3.W)
    UNKN(0147574); // D7, #$1234
    UNKN(0147602); // D7, D2
    TEST("EXG",   "D7, A2",           0147612); // EXG
    TEST("AND.L", "D7, (A2)",         0147622);
    TEST("AND.L", "D7, (A2)+",        0147632);
    TEST("AND.L", "D7, -(A2)",        0147642);
    TEST("AND.L", "D7, ($1234,A2)",   0147652, 0x1234);
    TEST("AND.L", "D7, (18,A2,D3.L)", 0147662, 0x3812);
    ABST("AND.L", "D7, (@A).W", "FFFC",   0147670, 0xFFFC);
    ABST("AND.L", "D7, (@A).L", "123454", 0147671, 0x0012, 0x3454);
    UNKN(0147672); // D7, (*+$1234,PC)
    UNKN(0147673); // D7, (*+10,PC,D3.W)
    UNKN(0147674); // D7, #$12345678

    // ANDI #nn,dst: 0001|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("ANDI.B", "#18, D2",           0001002, 0x0012);
    UNKN(0001012); // #18, A2
    TEST("ANDI.B", "#18, (A2)",         0001022, 0x0012);
    TEST("ANDI.B", "#18, (A2)+",        0001032, 0x0012);
    TEST("ANDI.B", "#18, -(A2)",        0001042, 0x0012);
    TEST("ANDI.B", "#18, ($1234,A2)",   0001052, 0x0012, 0x1234);
    TEST("ANDI.B", "#18, (18,A2,D3.W)", 0001062, 0x0012, 0x3012);
    ABST("ANDI.B", "#18, (@A).W", "1234",   0001070, 0x0012, 0x1234);
    ABST("ANDI.B", "#18, (@A).L", "123456", 0001071, 0x0012, 0x0012, 0x3456);
    UNKN(0001072); // #18, (*+$1234,PC)
    UNKN(0001073); // #18, (*+10,PC,D3.W)
    TEST("ANDI",   "#18, CCR",          0001074, 0x0012); // ANDI CCR
    TEST("ANDI.W", "#$5678, D2",           0001102, 0x5678);
    UNKN(0001112); // #$5678, A2
    TEST("ANDI.W", "#$5678, (A2)",         0001122, 0x5678);
    TEST("ANDI.W", "#$5678, (A2)+",        0001132, 0x5678);
    TEST("ANDI.W", "#$5678, -(A2)",        0001142, 0x5678);
    TEST("ANDI.W", "#$5678, ($1234,A2)",   0001152, 0x5678, 0x1234);
    TEST("ANDI.W", "#$5678, (18,A2,D3.W)", 0001162, 0x5678, 0x3012);
    ABST("ANDI.W", "#$5678, (@A).W", "1234",   0001170, 0x5678, 0x1234);
    ABST("ANDI.W", "#$5678, (@A).L", "123456", 0001171, 0x5678, 0x0012, 0x3456);
    UNKN(0001172); // #$5678, (*+$1234,PC)
    UNKN(0001173); // #$5678, (*+10,PC,D3.W)
    TEST("ANDI",   "#$5678, SR",           0001174, 0x5678); // ANDI SR
    TEST("ANDI.L", "#$3456789A, D2",           0001202, 0x3456, 0x789A);
    UNKN(0001212); // #$3456789A, A2
    TEST("ANDI.L", "#$3456789A, (A2)",         0001222, 0x3456, 0x789A);
    TEST("ANDI.L", "#$3456789A, (A2)+",        0001232, 0x3456, 0x789A);
    TEST("ANDI.L", "#$3456789A, -(A2)",        0001242, 0x3456, 0x789A);
    TEST("ANDI.L", "#$3456789A, ($1234,A2)",   0001252, 0x3456, 0x789A, 0x1234);
    TEST("ANDI.L", "#$3456789A, (18,A2,D3.W)", 0001262, 0x3456, 0x789A, 0x3012);
    ABST("ANDI.L", "#$3456789A, (@A).W", "1234",   0001270, 0x3456, 0x789A, 0x1234);
    ABST("ANDI.L", "#$34567898, (@A).L", "123454", 0001271, 0x3456, 0x7898, 0x0012, 0x3454);
    UNKN(0001272); // #$3456789A, (*+$1234,PC)
    UNKN(0001273); // #$3456789A, (*+10,PC,D3.W)
    UNKN(0001274); // #$3456789A, #$12345678

    // EOR Dn,dst: 013|Dn|Sz|M|Rn, Sz:B=4/W=5/L=6
    TEST("EOR.B", "D7, D2",           0137402);
    TEST("CMPM.B", "(A2)+, (A7)+",    0137412); // CMPM.B
    TEST("EOR.B", "D7, (A2)",         0137422);
    TEST("EOR.B", "D7, (A2)+",        0137432);
    TEST("EOR.B", "D7, -(A2)",        0137442);
    TEST("EOR.B", "D7, ($1234,A2)",   0137452, 0x1234);
    TEST("EOR.B", "D7, (18,A2,D3.L)", 0137462, 0x3812);
    ABST("EOR.B", "D7, (@A).W", "FFFF",   0137470, 0xFFFF);
    ABST("EOR.B", "D7, (@A).L", "123456", 0137471, 0x0012, 0x3456);
    UNKN(0137472); // D7, (*+$1234,PC)
    UNKN(0137473); // D7, (*+10,PC,D3.W)
    UNKN(0137474); // D7, #$34
    TEST("EOR.W",  "D7, D2",          0137502);
    TEST("CMPM.W", "(A2)+, (A7)+",    0137512); // CMPM.W
    TEST("EOR.W", "D7, (A2)",         0137522);
    TEST("EOR.W", "D7, (A2)+",        0137532);
    TEST("EOR.W", "D7, -(A2)",        0137542);
    TEST("EOR.W", "D7, ($1234,A2)",   0137552, 0x1234);
    TEST("EOR.W", "D7, (18,A2,D3.L)", 0137562, 0x3812);
    ABST("EOR.W", "D7, (@A).W", "FFFE",   0137570, 0xFFFE);
    ABST("EOR.W", "D7, (@A).L", "123456", 0137571, 0x0012, 0x3456);
    UNKN(0137572); // D7, (*+$1234,PC)
    UNKN(0137573); // D7, (*+10,PC,D3.W)
    UNKN(0137574); // D7, #$1234
    TEST("EOR.L",  "D7, D2",          0137602);
    TEST("CMPM.L", "(A2)+, (A7)+",    0137612); // CMPM.L
    TEST("EOR.L", "D7, (A2)",         0137622);
    TEST("EOR.L", "D7, (A2)+",        0137632);
    TEST("EOR.L", "D7, -(A2)",        0137642);
    TEST("EOR.L", "D7, ($1234,A2)",   0137652, 0x1234);
    TEST("EOR.L", "D7, (18,A2,D3.L)", 0137662, 0x3812);
    ABST("EOR.L", "D7, (@A).W", "FFFC",   0137670, 0xFFFC);
    ABST("EOR.L", "D7, (@A).L", "123454", 0137671, 0x0012, 0x3454);
    UNKN(0137672); // D7, (*+$1234,PC)
    UNKN(0137673); // D7, (*+10,PC,D3.W)
    UNKN(0137674); // D7, #$12345678

    // EORI #nn,dst: 0005|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("EORI.B", "#18, D2",           0005002, 0x0012);
    UNKN(0005012); // #18, A2
    TEST("EORI.B", "#18, (A2)",         0005022, 0x0012);
    TEST("EORI.B", "#18, (A2)+",        0005032, 0x0012);
    TEST("EORI.B", "#18, -(A2)",        0005042, 0x0012);
    TEST("EORI.B", "#18, ($1234,A2)",   0005052, 0x0012, 0x1234);
    TEST("EORI.B", "#18, (18,A2,D3.W)", 0005062, 0x0012, 0x3012);
    ABST("EORI.B", "#18, (@A).W", "1234",   0005070, 0x0012, 0x1234);
    ABST("EORI.B", "#18, (@A).L", "123456", 0005071, 0x0012, 0x0012, 0x3456);
    UNKN(0005072); // #18, (*+$1234,PC)
    UNKN(0005073); // #18, (*+10,PC,D3.W)
    TEST("EORI",   "#18, CCR",          0005074, 0x0012); // EORI CCR
    TEST("EORI.W", "#$5678, D2",           0005102, 0x5678);
    UNKN(0005112); // #$5678, A2
    TEST("EORI.W", "#$5678, (A2)",         0005122, 0x5678);
    TEST("EORI.W", "#$5678, (A2)+",        0005132, 0x5678);
    TEST("EORI.W", "#$5678, -(A2)",        0005142, 0x5678);
    TEST("EORI.W", "#$5678, ($1234,A2)",   0005152, 0x5678, 0x1234);
    TEST("EORI.W", "#$5678, (18,A2,D3.W)", 0005162, 0x5678, 0x3012);
    ABST("EORI.W", "#$5678, (@A).W", "1234",   0005170, 0x5678, 0x1234);
    ABST("EORI.W", "#$5678, (@A).L", "123456", 0005171, 0x5678, 0x0012, 0x3456);
    UNKN(0005172); // #$5678, (*+$1234,PC)
    UNKN(0005173); // #$5678, (*+10,PC,D3.W)
    TEST("EORI",   "#$5678, SR",           0005174, 0x5678); // EORI SR
    TEST("EORI.L", "#$3456789A, D2",           0005202, 0x3456, 0x789A);
    UNKN(0005212); // #$3456789A, A2
    TEST("EORI.L", "#$3456789A, (A2)",         0005222, 0x3456, 0x789A);
    TEST("EORI.L", "#$3456789A, (A2)+",        0005232, 0x3456, 0x789A);
    TEST("EORI.L", "#$3456789A, -(A2)",        0005242, 0x3456, 0x789A);
    TEST("EORI.L", "#$3456789A, ($1234,A2)",   0005252, 0x3456, 0x789A, 0x1234);
    TEST("EORI.L", "#$3456789A, (18,A2,D3.W)", 0005262, 0x3456, 0x789A, 0x3012);
    ABST("EORI.L", "#$3456789A, (@A).W", "1234",   0005270, 0x3456, 0x789A, 0x1234);
    ABST("EORI.L", "#$34567898, (@A).L", "123454", 0005271, 0x3456, 0x7898, 0x0012, 0x3454);
    UNKN(0005272); // #$3456789A, (*+$1234,PC)
    UNKN(0005273); // #$3456789A, (*+10,PC,D3.W)
    UNKN(0005274); // #$3456789A, #$12345678

    // NOT dst: 0043|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("NOT.B", "D2",               0043002);
    UNKN(0043012); // A2
    TEST("NOT.B", "(A2)",             0043022);
    TEST("NOT.B", "(A2)+",            0043032);
    TEST("NOT.B", "-(A2)",            0043042);
    TEST("NOT.B", "($1234,A2)",       0043052, 0x1234);
    TEST("NOT.B", "(18,A2,D3.W)",     0043062, 0x3012);
    ABST("NOT.B", "(@A).W", "1234",   0043070, 0x1234);
    ABST("NOT.B", "(@A).L", "123456", 0043071, 0x0012, 0x3456);
    UNKN(0043072); // (*+$1234,PC)
    UNKN(0043073); // (*+10,PC,D3.W)
    UNKN(0043074); // #$34
    TEST("NOT.W", "D2",               0043102);
    UNKN(0043112); // A2
    TEST("NOT.W", "(A2)",             0043122);
    TEST("NOT.W", "(A2)+",            0043132);
    TEST("NOT.W", "-(A2)",            0043142);
    TEST("NOT.W", "($1234,A2)",       0043152, 0x1234);
    TEST("NOT.W", "(18,A2,D3.W)",     0043162, 0x3012);
    ABST("NOT.W", "(@A).W", "1234",   0043170, 0x1234);
    ABST("NOT.W", "(@A).L", "123456", 0043171, 0x0012, 0x3456);
    UNKN(0043172); // (*+$1234,PC)
    UNKN(0043173); // (*+10,PC,D3.W)
    UNKN(0043174); // #$1234
    TEST("NOT.L", "D2",               0043202);
    UNKN(0043212); // A2
    TEST("NOT.L", "(A2)",             0043222);
    TEST("NOT.L", "(A2)+",            0043232);
    TEST("NOT.L", "-(A2)",            0043242);
    TEST("NOT.L", "($1234,A2)",       0043252, 0x1234);
    TEST("NOT.L", "(18,A2,D3.W)",     0043262, 0x3012);
    ABST("NOT.L", "(@A).W", "1234",   0043270, 0x1234);
    ABST("NOT.L", "(@A).L", "123454", 0043271, 0x0012, 0x3454);
    UNKN(0043272); // (*+$1234,PC)
    UNKN(0043273); // (*+10,PC,D3.W)
    UNKN(0043274); // #$12345678

    // OR src,Dn, 010|Dn|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("OR.B", "D2, D7",             0107002);
    UNKN(0107012); // A2, D7
    TEST("OR.B", "(A2), D7",           0107022);
    TEST("OR.B", "(A2)+, D7",          0107032);
    TEST("OR.B", "-(A2), D7",          0107042);
    TEST("OR.B", "($1234,A2), D7",     0107052, 0x1234);
    TEST("OR.B", "(18,A2,D3.L), D7",   0107062, 0x3812);
    ABST("OR.B", "(@A).W, D7", "FFFF",   0107070, 0xFFFF);
    ABST("OR.B", "(@A).L, D7", "123456", 0107071, 0x0012, 0x3456);
    TEST("OR.B", "(*+$1234,PC), D7",   0107072, 0x1232);
    TEST("OR.B", "(*+18,PC,D3.W), D7", 0107073, 0x3010);
    TEST("OR.B", "#$34, D7",           0107074, 0x0034);
    TEST("OR.W", "D2, D7",             0107102);
    UNKN(0107112); // A2, D7
    TEST("OR.W", "(A2), D7",           0107122);
    TEST("OR.W", "(A2)+, D7",          0107132);
    TEST("OR.W", "-(A2), D7",          0107142);
    TEST("OR.W", "($1234,A2), D7",     0107152, 0x1234);
    TEST("OR.W", "(18,A2,D3.L), D7",   0107162, 0x3812);
    ABST("OR.W", "(@A).W, D7", "FFFE",   0107170, 0xFFFE);
    ABST("OR.W", "(@A).L, D7", "123456", 0107171, 0x0012, 0x3456);
    TEST("OR.W", "(*+$1234,PC), D7",   0107172, 0x1232);
    TEST("OR.W", "(*+18,PC,D3.W), D7", 0107173, 0x3010);
    TEST("OR.W", "#$0034, D7",         0107174, 0x0034);
    TEST("OR.L", "D2, D7",             0107202);
    UNKN(0107212); // A2, D7
    TEST("OR.L", "(A2), D7",           0107222);
    TEST("OR.L", "(A2)+, D7",          0107232);
    TEST("OR.L", "-(A2), D7",          0107242);
    TEST("OR.L", "($1234,A2), D7",     0107252, 0x1234);
    TEST("OR.L", "(18,A2,D3.L), D7",   0107262, 0x3812);
    ABST("OR.L", "(@A).W, D7", "FFFC",   0107270, 0xFFFC);
    ABST("OR.L", "(@A).L, D7", "123454", 0107271, 0x0012, 0x3454);
    TEST("OR.L", "(*+$1234,PC), D7",   0107272, 0x1232);
    TEST("OR.L", "(*+18,PC,D3.W), D7", 0107273, 0x3010);
    TEST("OR.L", "#$00345678, D7",     0107274, 0x0034, 0x5678);

    // OR Dn,dst: 010|Dn|Sz|M|Rn, Sz:B=4/W=5/L=6
    TEST("SBCD", "D2, D7",            0107402); // SBCD
    TEST("SBCD", "-(A2), -(A7)",      0107412); // SBCD
    TEST("OR.B", "D7, (A2)",          0107422);
    TEST("OR.B", "D7, (A2)+",         0107432);
    TEST("OR.B", "D7, -(A2)",         0107442);
    TEST("OR.B", "D7, ($1234,A2)",    0107452, 0x1234);
    TEST("OR.B", "D7, (18,A2,D3.L)",  0107462, 0x3812);
    ABST("OR.B", "D7, (@A).W", "FFFF",   0107470, 0xFFFF);
    ABST("OR.B", "D7, (@A).L", "123456", 0107471, 0x0012, 0x3456);
    UNKN(0107472); // D7, (*+$1234,PC)
    UNKN(0107473); // D7, (*+10,PC,D3.W)
    UNKN(0107474); // D7, #$34
    if (firstGen()) {
        UNKN(0107502); // D7, D2
        UNKN(0107512); // D7, A2
    } else {
        // PACK Dn,Dm, #nn:       010|Dm|50|Dn
        // PACK -(An),-(Am), #nn: 010|Am|51|An
        TEST("PACK", "D2, D7, #$1234",       0107502, 0x1234);
        TEST("PACK", "-(A2), -(A7), #$1234", 0107512, 0x1234);
    }
    TEST("OR.W", "D7, (A2)",          0107522);
    TEST("OR.W", "D7, (A2)+",         0107532);
    TEST("OR.W", "D7, -(A2)",         0107542);
    TEST("OR.W", "D7, ($1234,A2)",    0107552, 0x1234);
    TEST("OR.W", "D7, (18,A2,D3.L)",  0107562, 0x3812);
    ABST("OR.W", "D7, (@A).W", "FFFE",   0107570, 0xFFFE);
    ABST("OR.W", "D7, (@A).L", "123456", 0107571, 0x0012, 0x3456);
    UNKN(0107572); // D7, (*+$1234,PC)
    UNKN(0107573); // D7, (*+10,PC,D3.W)
    UNKN(0107574); // D7, #$1234
    if (firstGen()) {
        UNKN(0107602); // D7, D2
        UNKN(0107612); // D7, A2
    } else {
        // UNPK Dn,Dm,#nn;       010|Dm|60|Dn
        // UNPK -(An),-(Am),#nn: 010|Am|61|An
        TEST("UNPK", "D2, D7, #$1234",       0107602, 0x1234);
        TEST("UNPK", "-(A2), -(A7), #$1234", 0107612, 0x1234);
    }
    TEST("OR.L", "D7, (A2)",          0107622);
    TEST("OR.L", "D7, (A2)+",         0107632);
    TEST("OR.L", "D7, -(A2)",         0107642);
    TEST("OR.L", "D7, ($1234,A2)",    0107652, 0x1234);
    TEST("OR.L", "D7, (18,A2,D3.L)",  0107662, 0x3812);
    ABST("OR.L", "D7, (@A).W", "FFFC",   0107670, 0xFFFC);
    ABST("OR.L", "D7, (@A).L", "123454", 0107671, 0x0012, 0x3454);
    UNKN(0107672); // D7, (*+$1234,PC)
    UNKN(0107673); // D7, (*+10,PC,D3.W)
    UNKN(0107674); // D7, #$12345678

    // ORI #nn,dst: 0000|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("ORI.B", "#18, D2",           0000002, 0x0012);
    UNKN(0000012); // #18, A2
    TEST("ORI.B", "#18, (A2)",         0000022, 0x0012);
    TEST("ORI.B", "#18, (A2)+",        0000032, 0x0012);
    TEST("ORI.B", "#18, -(A2)",        0000042, 0x0012);
    TEST("ORI.B", "#18, ($1234,A2)",   0000052, 0x0012, 0x1234);
    TEST("ORI.B", "#18, (18,A2,D3.W)", 0000062, 0x0012, 0x3012);
    ABST("ORI.B", "#18, (@A).W", "1234",   0000070, 0x0012, 0x1234);
    ABST("ORI.B", "#18, (@A).L", "123456", 0000071, 0x0012, 0x0012, 0x3456);
    UNKN(0000072); // #18, (*+$1234,PC)
    UNKN(0000073); // #18, (*+10,PC,D3.W)
    TEST("ORI",   "#18, CCR",          0000074, 0x0012); // ORI CCR
    TEST("ORI.W", "#$5678, D2",           0000102, 0x5678);
    UNKN(0000112); // #$5678, A2
    TEST("ORI.W", "#$5678, (A2)",         0000122, 0x5678);
    TEST("ORI.W", "#$5678, (A2)+",        0000132, 0x5678);
    TEST("ORI.W", "#$5678, -(A2)",        0000142, 0x5678);
    TEST("ORI.W", "#$5678, ($1234,A2)",   0000152, 0x5678, 0x1234);
    TEST("ORI.W", "#$5678, (18,A2,D3.W)", 0000162, 0x5678, 0x3012);
    ABST("ORI.W", "#$5678, (@A).W", "1234",   0000170, 0x5678, 0x1234);
    ABST("ORI.W", "#$5678, (@A).L", "123456", 0000171, 0x5678, 0x0012, 0x3456);
    UNKN(0000172); // #$5678, (*+$1234,PC)
    UNKN(0000173); // #$5678, (*+10,PC,D3.W)
    TEST("ORI",   "#$5678, SR",           0000174, 0x5678); // ORI SR
    TEST("ORI.L", "#$3456789A, D2",           0000202, 0x3456, 0x789A);
    UNKN(0000212); // #$3456789A, A2
    TEST("ORI.L", "#$3456789A, (A2)",         0000222, 0x3456, 0x789A);
    TEST("ORI.L", "#$3456789A, (A2)+",        0000232, 0x3456, 0x789A);
    TEST("ORI.L", "#$3456789A, -(A2)",        0000242, 0x3456, 0x789A);
    TEST("ORI.L", "#$3456789A, ($1234,A2)",   0000252, 0x3456, 0x789A, 0x1234);
    TEST("ORI.L", "#$3456789A, (18,A2,D3.W)", 0000262, 0x3456, 0x789A, 0x3012);
    ABST("ORI.L", "#$3456789A, (@A).W", "1234",   0000270, 0x3456, 0x789A, 0x1234);
    ABST("ORI.L", "#$34567898, (@A).L", "123454", 0000271, 0x3456, 0x7898, 0x0012, 0x3454);
    UNKN(0000272); // #$3456789A, (*+$1234,PC)
    UNKN(0000273); // #$3456789A, (*+10,PC,D3.W)
    UNKN(0000274); // #$3456789A, #$12345678
}

void test_shift_rotate() {
    // ASL Dx,Dy: 016|Dx|Sz|4|Dy, Sz:B=4/W=5/L=6
    TEST("ASL.B", "D2, D7", 0162447);
    TEST("ASL.W", "D2, D7", 0162547);
    TEST("ASL.L", "D2, D7", 0162647);

    // ASL #nn,Dy: 016|nn|Sz|0|Dy, Sz:B=4/W=5/L=6
    TEST("ASL.B", "#1, D7", 0161407);
    TEST("ASL.W", "#4, D7", 0164507);
    TEST("ASL.L", "#8, D7", 0160607);

    // ASL dst: 01607|M|Rn
    UNKN(0160702); // D2
    UNKN(0160712); // A2
    TEST("ASL.W", "(A2)",          0160722);
    TEST("ASL.W", "(A2)+",         0160732);
    TEST("ASL.W", "-(A2)",         0160742);
    TEST("ASL.W", "($2345,A2)",    0160752, 0x2345);
    TEST("ASL.W", "($23,A2,D3.L)", 0160762, 0x3823);
    ABST("ASL.W", "(@A).W", "2346",   0160770, 0x2346);
    ABST("ASL.W", "(@A).L", "234568", 0160771, 0x0023, 0x4568);
    UNKN(0160772); // (*+$1234,PC)
    UNKN(0160773); // (*+10,PC,D3.W)
    UNKN(0160774); // #$1234

    // ASR Dx,Dy: 016|Dx|Sz|4|Dy, Sz:B=0/W=1/L=2
    TEST("ASR.B", "D2, D7", 0162047);
    TEST("ASR.W", "D2, D7", 0162147);
    TEST("ASR.L", "D2, D7", 0162247);

    // ASR #nn,Dy: 016|Dx|Sz|0|Dy, Sz:B=0/W=1/L=2
    TEST("ASR.B", "#1, D7", 0161007);
    TEST("ASR.W", "#4, D7", 0164107);
    TEST("ASR.L", "#8, D7", 0160207);

    // ASR dst: 01603|M|Rn
    UNKN(0160302); // D2
    UNKN(0160312); // A2
    TEST("ASR.W", "(A2)",          0160322);
    TEST("ASR.W", "(A2)+",         0160332);
    TEST("ASR.W", "-(A2)",         0160342);
    TEST("ASR.W", "($2345,A2)",    0160352, 0x2345);
    TEST("ASR.W", "($23,A2,D3.L)", 0160362, 0x3823);
    ABST("ASR.W", "(@A).W", "2346",   0160370, 0x2346);
    ABST("ASR.W", "(@A).L", "234568", 0160371, 0x0023, 0x4568);
    UNKN(0160372); // (*+$1234,PC)
    UNKN(0160373); // (*+10,PC,D3.W)
    UNKN(0160374); // #$1234

    // LSL Dx,Dy: 016|Dx|Sz|5|Dy, Sz:B=4/W=5/L=6
    TEST("LSL.B", "D2, D7", 0162457);
    TEST("LSL.W", "D2, D7", 0162557);
    TEST("LSL.L", "D2, D7", 0162657);

    // LSL #n,Dy: 016|nn|Sz|1|Dy: Sz:B=4/W=5/L=6
    TEST("LSL.B", "#1, D7", 0161417);
    TEST("LSL.W", "#4, D7", 0164517);
    TEST("LSL.L", "#8, D7", 0160617);

    // LSL dst: 01617|M|Rn
    UNKN(0161702); // D2
    UNKN(0161712); // A2
    TEST("LSL.W", "(A2)",          0161722);
    TEST("LSL.W", "(A2)+",         0161732);
    TEST("LSL.W", "-(A2)",         0161742);
    TEST("LSL.W", "($2345,A2)",    0161752, 0x2345);
    TEST("LSL.W", "($23,A2,D3.L)", 0161762, 0x3823);
    ABST("LSL.W", "(@A).W", "2346",   0161770, 0x2346);
    ABST("LSL.W", "(@A).L", "234568", 0161771, 0x0023, 0x4568);
    UNKN(0161772); // (*+$1234,PC)
    UNKN(0161773); // (*+10,PC,D3.W)
    UNKN(0161774); // #$1234

    // LSR Dx,Dy: 016|Dx|Sz|5|Dy, Sz:B=0/W=1/L=2
    TEST("LSR.B", "D2, D7", 0162057);
    TEST("LSR.W", "D2, D7", 0162157);
    TEST("LSR.L", "D2, D7", 0162257);

    // LSR #n,Dy: 016|nn|Sz|1|Dy, Sz:B=0/W=1/L=2
    TEST("LSR.B", "#1, D7", 0161017);
    TEST("LSR.W", "#4, D7", 0164117);
    TEST("LSR.L", "#8, D7", 0160217);

    // LSR dst: 01613|M|Rn
    UNKN(0161302); // D2
    UNKN(0161312); // A2
    TEST("LSR.W", "(A2)",          0161322);
    TEST("LSR.W", "(A2)+",         0161332);
    TEST("LSR.W", "-(A2)",         0161342);
    TEST("LSR.W", "($2345,A2)",    0161352, 0x2345);
    TEST("LSR.W", "($23,A2,D3.L)", 0161362, 0x3823);
    ABST("LSR.W", "(@A).W", "2346",   0161370, 0x2346);
    ABST("LSR.W", "(@A).L", "234568", 0161371, 0x0023, 0x4568);
    UNKN(0161372); // (*+$1234,PC)
    UNKN(0161373); // (*+10,PC,D3.W)
    UNKN(0161374); // #$1234

    // ROL Dx,Dy: 016|Dx|Sz|7|Dy, Sz:B=4/W=5/L=6
    TEST("ROL.B", "D2, D7", 0162477);
    TEST("ROL.W", "D2, D7", 0162577);
    TEST("ROL.L", "D2, D7", 0162677);

    // ROL #n,Dy: 016|nn|Sz|3|Dy, Sz:B=4/W=5/L=6
    TEST("ROL.B", "#1, D7", 0161437);
    TEST("ROL.W", "#4, D7", 0164537);
    TEST("ROL.L", "#8, D7", 0160637);

    // ROL dst: 01637|M|Rn
    UNKN(0163702); // D2
    UNKN(0163712); // A2
    TEST("ROL.W", "(A2)",          0163722);
    TEST("ROL.W", "(A2)+",         0163732);
    TEST("ROL.W", "-(A2)",         0163742);
    TEST("ROL.W", "($2345,A2)",    0163752, 0x2345);
    TEST("ROL.W", "($23,A2,D3.L)", 0163762, 0x3823);
    ABST("ROL.W", "(@A).W", "2346",   0163770, 0x2346);
    ABST("ROL.W", "(@A).L", "234568", 0163771, 0x0023, 0x4568);
    UNKN(0163772); // (*+$1234,PC)
    UNKN(0163773); // (*+10,PC,D3.W)
    UNKN(0163774); // #$1234

    // ROR Dx,Dy: 016|Dx|Sz|7|Dy, Sz:B=0/W=1/L=2
    TEST("ROR.B", "D2, D7", 0162077);
    TEST("ROR.W", "D2, D7", 0162177);
    TEST("ROR.L", "D2, D7", 0162277);

    // ROR #n,Dy: 016|Dx|Sz|3|Dy, Sz:B=0/W=1/L=2
    TEST("ROR.B", "#1, D7", 0161037);
    TEST("ROR.W", "#4, D7", 0164137);
    TEST("ROR.L", "#8, D7", 0160237);

    // ROR dst: 01633|M|Rn
    UNKN(0163302); // D2
    UNKN(0163312); // A2
    TEST("ROR.W", "(A2)",          0163322);
    TEST("ROR.W", "(A2)+",         0163332);
    TEST("ROR.W", "-(A2)",         0163342);
    TEST("ROR.W", "($2345,A2)",    0163352, 0x2345);
    TEST("ROR.W", "($23,A2,D3.L)", 0163362, 0x3823);
    ABST("ROR.W", "(@A).W", "2346",   0163370, 0x2346);
    ABST("ROR.W", "(@A).L", "234568", 0163371, 0x0023, 0x4568);
    UNKN(0163372); // (*+$1234,PC)
    UNKN(0163373); // (*+10,PC,D3.W)
    UNKN(0163374); // #$1234

    // ROXL Dx,Dy: 016|Dx|Sz|6|Dy, Sz:B=4/W=5/L=6
    TEST("ROXL.B", "D2, D7", 0162467);
    TEST("ROXL.W", "D2, D7", 0162567);
    TEST("ROXL.L", "D2, D7", 0162667);

    // ROXL #n,Dy: 016|nn|Sz|2|Dy, Sz:B=4/W=5/L=6
    TEST("ROXL.B", "#1, D7", 0161427);
    TEST("ROXL.W", "#4, D7", 0164527);
    TEST("ROXL.L", "#8, D7", 0160627);

    // ROXL dst: 01627|M|Rn
    UNKN(0162702); // D2
    UNKN(0162712); // A2
    TEST("ROXL.W", "(A2)",          0162722);
    TEST("ROXL.W", "(A2)+",         0162732);
    TEST("ROXL.W", "-(A2)",         0162742);
    TEST("ROXL.W", "($2345,A2)",    0162752, 0x2345);
    TEST("ROXL.W", "($23,A2,D3.L)", 0162762, 0x3823);
    ABST("ROXL.W", "(@A).W", "2346",   0162770, 0x2346);
    ABST("ROXL.W", "(@A).L", "234568", 0162771, 0x0023, 0x4568);
    UNKN(0162772); // (*+$1234,PC)
    UNKN(0162773); // (*+10,PC,D3.W)
    UNKN(0162774); // #$1234

    // ROXR Dx,Dy: 016|Dx|Sz|6|Dy, Sz:B=0/W=1/L=2
    TEST("ROXR.B", "D2, D7", 0162067);
    TEST("ROXR.W", "D2, D7", 0162167);
    TEST("ROXR.L", "D2, D7", 0162267);

    // ROXR #n,Dy: 016|nn|Sz|2|Dy, Sz:B=0/W=1/L=2
    TEST("ROXR.B", "#1, D7", 0161027);
    TEST("ROXR.W", "#4, D7", 0164127);
    TEST("ROXR.L", "#8, D7", 0160227);

    // ROXR dst: 01623|M|Rn
    UNKN(0162302); // D2
    UNKN(0162312); // A2
    TEST("ROXR.W", "(A2)",          0162322);
    TEST("ROXR.W", "(A2)+",         0162332);
    TEST("ROXR.W", "-(A2)",         0162342);
    TEST("ROXR.W", "($2345,A2)",    0162352, 0x2345);
    TEST("ROXR.W", "($23,A2,D3.L)", 0162362, 0x3823);
    ABST("ROXR.W", "(@A).W", "2346",   0162370, 0x2346);
    ABST("ROXR.W", "(@A).L", "234568", 0162371, 0x0023, 0x4568);
    UNKN(0162372); // (*+$1234,PC)
    UNKN(0162373); // (*+10,PC,D3.W)
    UNKN(0162374); // #$1234

    // SWAP Dn: 004410|Dn
    TEST("SWAP", "D0", 0044100);
    TEST("SWAP", "D7", 0044107);
}

void test_bit() {
    // BCHG Dx,dst: 000|Dx|5|M|Rn
    TEST("BCHG.L", "D7, D2",           0007502);
    TEST("MOVEP.L", "($1234,A2), D7",  0007512, 0x1234); // MOVEP.L
    TEST("BCHG.B", "D7, (A2)",         0007522);
    TEST("BCHG.B", "D7, (A2)+",        0007532);
    TEST("BCHG.B", "D7, -(A2)",        0007542);
    TEST("BCHG.B", "D7, ($1234,A2)",   0007552, 0x1234);
    TEST("BCHG.B", "D7, (18,A2,D3.W)", 0007562, 0x3012);
    ABST("BCHG.B", "D7, (@A).W", "1234",   0007570, 0x1234);
    ABST("BCHG.B", "D7, (@A).L", "012345", 0007571, 0x0001, 0x2345);
    UNKN(0007572); // D7, (*+$1234,PC)
    UNKN(0007573); // D7, (*+10,PC,D3.W)
    UNKN(0007574); // D7, #$34

    // BCHG #n,dst: 00041|M|Rn
    TEST("BCHG.L", "#0, D2",                                 0004102, 0x0000);
    TEST("BCHG.L", "#31, D2",                                0004102, 0x001F);
    ERRT("BCHG.L", "#32, D2",  ILLEGAL_BIT_NUMBER, "32, D2", 0004102, 0x0020);
    UNKN(0004112); // #6, A2
    TEST("BCHG.B", "#7, (A2)",                                0004122, 0x0007);
    ERRT("BCHG.B", "#8, (A2)", ILLEGAL_BIT_NUMBER, "8, (A2)", 0004122, 0x0008);
    TEST("BCHG.B", "#6, (A2)+",                    0004132, 0x0006);
    TEST("BCHG.B", "#5, -(A2)",                    0004142, 0x0005);
    TEST("BCHG.B", "#4, ($1234,A2)",               0004152, 0x0004, 0x1234);
    TEST("BCHG.B", "#3, (18,A2,D3.W)",             0004162, 0x0003, 0x3012);
    ABST("BCHG.B", "#2, (@A).W", "1234",           0004170, 0x0002, 0x1234);
    ABST("BCHG.B", "#1, (@A).L", "012345",         0004171, 0x0001, 0x0001, 0x2345);
    EABS("BCHG.B", "#1, (@A).L", "010000", NO_MEMORY, "(@A).L", 0004171, 0x0001, 0x0001);
    EABS("BCHG.B", "#1, (@A).L", "010000", NO_MEMORY, "(@A).L", 0004171, 0x0001, 0x0001);
    EABS("BCHG.B", "#1, (@A).L", "000000", NO_MEMORY, "(@A).L", 0004171, 0x0001);
    EABS("BCHG.B", "#0, (@A).L", "000000", NO_MEMORY, "0, (@A).L", 0004171);
    UNKN(0004172); // #0, (*+$1234,PC)
    UNKN(0004173); // #7, (*+10,PC,D3.W)
    UNKN(0004174); // #6, #$34

    // BCLR Dx,dst: 000|Dx|6|M|Rn
    TEST("BCLR.L", "D7, D2",           0007602);
    TEST("MOVEP.W", "D7, ($1234,A2)",  0007612, 0x1234); // MOVEP.W
    TEST("BCLR.B", "D7, (A2)",         0007622);
    TEST("BCLR.B", "D7, (A2)+",        0007632);
    TEST("BCLR.B", "D7, -(A2)",        0007642);
    TEST("BCLR.B", "D7, ($1234,A2)",   0007652, 0x1234);
    TEST("BCLR.B", "D7, (18,A2,D3.W)", 0007662, 0x3012);
    ABST("BCLR.B", "D7, (@A).W", "1234",   0007670, 0x1234);
    ABST("BCLR.B", "D7, (@A).L", "012345", 0007671, 0x0001, 0x2345);
    UNKN(0007672); // D7, (*+$1234,PC)
    UNKN(0007673); // D7, (*+10,PC,D3.W)
    UNKN(0007674); // D7, #$34

    // BCLR #n,dst: 00042|M|Rn
    TEST("BCLR.L", "#0, D2",           0004202, 0x0000);
    UNKN(0004212); // #1, A2
    TEST("BCLR.B", "#7, (A2)",         0004222, 0x0007);
    TEST("BCLR.B", "#6, (A2)+",        0004232, 0x0006);
    TEST("BCLR.B", "#5, -(A2)",        0004242, 0x0005);
    TEST("BCLR.B", "#4, ($1234,A2)",   0004252, 0x0004, 0x1234);
    TEST("BCLR.B", "#3, (18,A2,D3.W)", 0004262, 0x0003, 0x3012);
    ABST("BCLR.B", "#2, (@A).W", "1234",   0004270, 0x0002, 0x1234);
    ABST("BCLR.B", "#1, (@A).L", "012345", 0004271, 0x0001, 0x0001, 0x2345);
    UNKN(0004272); // #0, (*+$1234,PC)
    UNKN(0004273); // #7, (*+10,PC,D3.W)
    UNKN(0004274); // #6, #$34

    // BSET Dx,dst: 000|Dx|7|M|Rn
    TEST("BSET.L", "D7, D2",           0007702);
    TEST("MOVEP.L", "D7, ($1234,A2)",  0007712, 0x1234); // MOVEP.L
    TEST("BSET.B", "D7, (A2)",         0007722);
    TEST("BSET.B", "D7, (A2)+",        0007732);
    TEST("BSET.B", "D7, -(A2)",        0007742);
    TEST("BSET.B", "D7, ($1234,A2)",   0007752, 0x1234);
    TEST("BSET.B", "D7, (18,A2,D3.W)", 0007762, 0x3012);
    ABST("BSET.B", "D7, (@A).W", "1234",   0007770, 0x1234);
    ABST("BSET.B", "D7, (@A).L", "012345", 0007771, 0x0001, 0x2345);
    UNKN(0007772); // D7, (*+$1234,PC)
    UNKN(0007773); // D7, (*+10,PC,D3.W)
    UNKN(0007774); // D7, #$34

    // BSET #n,dst: 00043|M|Rn
    TEST("BSET.L", "#0, D2",           0004302, 0x0000);
    UNKN(0004312); // #1, A2
    TEST("BSET.B", "#7, (A2)",         0004322, 0x0007);
    TEST("BSET.B", "#6, (A2)+",        0004332, 0x0006);
    TEST("BSET.B", "#5, -(A2)",        0004342, 0x0005);
    TEST("BSET.B", "#4, ($1234,A2)",   0004352, 0x0004, 0x1234);
    TEST("BSET.B", "#3, (18,A2,D3.W)", 0004362, 0x0003, 0x3012);
    ABST("BSET.B", "#2, (@A).W", "1234",   0004370, 0x0002, 0x1234);
    ABST("BSET.B", "#1, (@A).L", "012345", 0004371, 0x0001, 0x0001, 0x2345);
    UNKN(0004372); // #0, (*+$1234,PC)
    UNKN(0004373); // #7, (*+10,PC,D3.W)
    UNKN(0004374); // #6, #$34

    // BTST Dx,dst: 000|Dx|4|M|Rn
    TEST("BTST.L", "D7, D2",             0007402);
    TEST("MOVEP.W", "($1234,A2), D7",    0007412, 0x1234); // MOVEP.W
    TEST("BTST.B", "D7, (A2)",           0007422);
    TEST("BTST.B", "D7, (A2)+",          0007432);
    TEST("BTST.B", "D7, -(A2)",          0007442);
    TEST("BTST.B", "D7, ($1234,A2)",     0007452, 0x1234);
    TEST("BTST.B", "D7, (18,A2,D3.W)",   0007462, 0x3012);
    ABST("BTST.B", "D7, (@A).W", "1234",   0007470, 0x1234);
    ABST("BTST.B", "D7, (@A).L", "012345", 0007471, 0x0001, 0x2345);
    TEST("BTST.B", "D7, (*+$1234,PC)",   0007472, 0x1232);
    TEST("BTST.B", "D7, (*+35,PC,D3.L)", 0007473, 0x3821);
    UNKN(0007474); // D7, #$34

    // BTST #n,dst: 00040|M|Rn
    TEST("BTST.L", "#0, D2",             0004002, 0x0000);
    UNKN(0004012); // #1, A2
    TEST("BTST.B", "#7, (A2)",           0004022, 0x0007);
    TEST("BTST.B", "#6, (A2)+",          0004032, 0x0006);
    TEST("BTST.B", "#5, -(A2)",          0004042, 0x0005);
    TEST("BTST.B", "#4, ($1234,A2)",     0004052, 0x0004, 0x1234);
    TEST("BTST.B", "#3, (18,A2,D3.W)",   0004062, 0x0003, 0x3012);
    ABST("BTST.B", "#2, (@A).W", "1234",   0004070, 0x0002, 0x1234);
    ABST("BTST.B", "#1, (@A).L", "012345", 0004071, 0x0001, 0x0001, 0x2345);
    TEST("BTST.B", "#0, (*+$1234,PC)",   0004072, 0x0000, 0x1230);
    TEST("BTST.B", "#7, (*+35,PC,D3.L)", 0004073, 0x0007, 0x381F);
    UNKN(0004074); // #6, #$34
}

void test_bcd() {
    // ABCD Dx,Dy: 014|Dy|40|Dx
    TEST("ABCD", "D2, D7", 0147402);

    // ABCD -(Ax), -(Ay)" 014|Ay|41|Ax
    TEST("ABCD", "-(A2), -(A7)", 0147412);

    // NBCD dst: 00440|M|Rn
    TEST("NBCD", "D2",                0044002);
    if (firstGen()) {
        UNKN(0044012); // A2
    } else {
        TEST("LINK.L", "A2, #$12345678", 0044012, 0x1234, 0x5678);
    }
    TEST("NBCD", "(A2)",              0044022);
    TEST("NBCD", "(A2)+",             0044032);
    TEST("NBCD", "-(A2)",             0044042);
    TEST("NBCD", "($1234,A2)",        0044052, 0x1234);
    TEST("NBCD", "(18,A2,D3.W)",      0044062, 0x3012);
    ABST("NBCD", "(@A).W", "1234",    0044070, 0x1234);
    ABST("NBCD", "(@A).L", "123456",  0044071, 0x0012, 0x3456);
    UNKN(0044072); // (*+$1234,PC)
    UNKN(0044073); // (*+10,PC,D3.W)
    UNKN(0044074); // #$34

    // SBCD Dx,Dy: 010|Dy|40|Dx
    TEST("SBCD", "D2, D7", 0107402);

    // SBCD -(Ax), -(Ay): 010|Ay|41|Ax
    TEST("SBCD", "-(A2), -(A7)", 0107412);

    if (!firstGen()) {
        // PACK Dn,Dm, #nn:       010|Dm|50|Dn
        // PACK -(An),-(Am), #nn: 010|Am|51|An
        TEST("PACK", "D2, D7, #$1234",       0107502, 0x1234);
        TEST("PACK", "-(A2), -(A7), #$1234", 0107512, 0x1234);

        // UNPK Dn,Dm,#nn;       010|Dm|60|Dn
        // UNPK -(An),-(Am),#nn: 010|Am|61|An
        TEST("UNPK", "D2, D7, #$1234",       0107602, 0x1234);
        TEST("UNPK", "-(A2), -(A7), #$1234", 0107612, 0x1234);
    }
}

void test_program() {
    disassembler.setOption("relative", "true");

    // Bcc label: 006|cc|disp
    ATEST(0x10000, "BRA", "*", 0060000 | 0x000 | 0xFE);
    ATEST(0x10000, "BSR", "*", 0060000 | 0x100 | 0xFE);
    ATEST(0x10000, "BHI", "*", 0060000 | 0x200 | 0xFE);
    ATEST(0x10000, "BLS", "*", 0060000 | 0x300 | 0xFE);
    ATEST(0x10000, "BCC", "*", 0060000 | 0x400 | 0xFE);
    ATEST(0x10000, "BCS", "*", 0060000 | 0x500 | 0xFE);
    ATEST(0x10000, "BNE", "*", 0060000 | 0x600 | 0xFE);
    ATEST(0x10000, "BEQ", "*", 0060000 | 0x700 | 0xFE);
    ATEST(0x10000, "BVC", "*", 0060000 | 0x800 | 0xFE);
    ATEST(0x10000, "BVS", "*", 0060000 | 0x900 | 0xFE);
    ATEST(0x10000, "BPL", "*", 0060000 | 0xA00 | 0xFE);
    ATEST(0x10000, "BMI", "*", 0060000 | 0xB00 | 0xFE);
    ATEST(0x10000, "BGE", "*", 0060000 | 0xC00 | 0xFE);
    ATEST(0x10000, "BLT", "*", 0060000 | 0xD00 | 0xFE);
    ATEST(0x10000, "BGT", "*", 0060000 | 0xE00 | 0xFE);
    ATEST(0x10000, "BLE", "*", 0060000 | 0xF00 | 0xFE);
    ATEST(0x10000, "BRA.W", "*",       0060000 | 0x000, 0xFFFE);
    ATEST(0x10000, "BRA.W", "*+$0080", 0060000 | 0x000, 0x007E);
    ATEST(0x10000, "BRA.W", "*-$007E", 0060000 | 0x000, 0xFF80);
    ATEST(0x10000, "BRA",   "*+$8000", 0060000 | 0x000, 0x7FFE);
    ATEST(0x10000, "BRA",   "*-$7FFE", 0060000 | 0x000, 0x8000);
    disassembler.setOption("gnu-as", "on");
    ATEST(0x10000, "BRAS", ".",       0060000 | 0x000 | 0xFE);
    ATEST(0x10000, "BRAS", ".+128",   0060000 | 0x000 | 0x7E);
    ATEST(0x10000, "BRAS", ".-126",   0060000 | 0x000 | 0x80);
    ATEST(0x10000, "BRA",  ".",       0060000 | 0x000, 0xFFFE);
    ATEST(0x10000, "BRA", ".+0x0080", 0060000 | 0x000, 0x007E);
    ATEST(0x10000, "BRA", ".-0x007E", 0060000 | 0x000, 0xFF80);
    ATEST(0x10000, "BRA", ".+0x8000", 0060000 | 0x000, 0x7FFE);
    ATEST(0x10000, "BRA", ".-0x7FFE", 0060000 | 0x000, 0x8000);
    disassembler.setOption("gnu-as", "off");

    if (firstGen()) {
        AERRT(0x10000, "BRA", "*+1", OPERAND_NOT_ALIGNED, "*+1", 0060000 | 0x000 | 0xFF);
    } else {
        ATEST(0x10000, "BRA.X", "*+2", 0060000 | 0x000 | 0xFF, 0x0000, 0x0000);
    }

    // DBcc Dn,labelL 005|cc|31|Dn
    ATEST(0x10000, "DBRA", "D2, *-$7FFE",                      0050312 | 0x100, 0x8000);
    ATEST(0x10000, "DBRA", "D2, *-$007E",                      0050312 | 0x100, 0xFF80);
    ATEST(0x10000, "DBRA", "D2, *",                            0050312 | 0x100, 0xFFFE);
    ATEST(0x10000, "DBRA", "D2, *+2",                          0050312 | 0x100, 0x0000);
    ATEST(0x10000, "DBRA", "D2, *+$0080",                      0050312 | 0x100, 0x007E);
    ATEST(0x10000, "DBRA", "D2, *+$8000",                      0050312 | 0x100, 0x7FFE);
    AERRT(0x10000, "DBRA", "D2, *-$7FFD", OPERAND_NOT_ALIGNED, "*-$7FFD", 0050312 | 0x100, 0x8001);
    AERRT(0x10000, "DBRA", "D2, *+$8001", OPERAND_NOT_ALIGNED, "*+$8001", 0050312 | 0x100, 0x7FFF);
    ATEST(0x10000, "DBT",  "D2, *",                            0050312 | 0x000, 0xFFFE);
    ATEST(0x10000, "DBHI", "D2, *",                            0050312 | 0x200, 0xFFFE);
    ATEST(0x10000, "DBLS", "D2, *",                            0050312 | 0x300, 0xFFFE);
    ATEST(0x10000, "DBCC", "D2, *",                            0050312 | 0x400, 0xFFFE);
    ATEST(0x10000, "DBCS", "D2, *",                            0050312 | 0x500, 0xFFFE);
    ATEST(0x10000, "DBNE", "D2, *",                            0050312 | 0x600, 0xFFFE);
    ATEST(0x10000, "DBEQ", "D2, *",                            0050312 | 0x700, 0xFFFE);
    ATEST(0x10000, "DBVC", "D2, *",                            0050312 | 0x800, 0xFFFE);
    ATEST(0x10000, "DBVS", "D2, *",                            0050312 | 0x900, 0xFFFE);
    ATEST(0x10000, "DBPL", "D2, *",                            0050312 | 0xA00, 0xFFFE);
    ATEST(0x10000, "DBMI", "D2, *",                            0050312 | 0xB00, 0xFFFE);
    ATEST(0x10000, "DBGE", "D2, *",                            0050312 | 0xC00, 0xFFFE);
    ATEST(0x10000, "DBLT", "D2, *",                            0050312 | 0xD00, 0xFFFE);
    ATEST(0x10000, "DBGT", "D2, *",                            0050312 | 0xE00, 0xFFFE);
    ATEST(0x10000, "DBLE", "D2, *",                            0050312 | 0xF00, 0xFFFE);

    // Scc dst: 005|cc|3|M|Rn
    TEST("ST", "D2",           0050302 | 0x000);
    TEST("DBT", "D2, *+2",     0050312 | 0x000, 0); // DBT
    TEST("ST", "(A2)",         0050322 | 0x000);
    TEST("ST", "(A2)+",        0050332 | 0x000);
    TEST("ST", "-(A2)",        0050342 | 0x000);
    TEST("ST", "($1234,A2)",   0050352 | 0x000, 0x1234);
    TEST("ST", "(18,A2,D3.W)", 0050362 | 0x000, 0x3012);
    ABST("ST", "(@A).W", "1233",   0050370 | 0x000, 0x1233);
    ABST("ST", "(@A).L", "123456", 0050371 | 0x000, 0x0012, 0x3456);
    if (firstGen()) {
        UNKN(0050372 | 0x000); // (*+$1234,PC)
        UNKN(0050373 | 0x000); // (*+10,PC,D3.W)
        UNKN(0050374 | 0x000); // #$34
    } else {
        TEST("TRAPT.W", "#$1234",     0050372 | 0x000, 0x1234);
        TEST("TRAPT.L", "#$12345678", 0050373 | 0x000, 0x1234, 0x5678);
        TEST("TRAPT",   "",           0050374 | 0x000);
    }
    TEST("SF", "D2",           0050302 | 0x100);
    TEST("DBRA", "D2, *+2",    0050312 | 0x100, 0); // DBRA
    TEST("SF", "(A2)",         0050322 | 0x100);
    TEST("SF", "(A2)+",        0050332 | 0x100);
    TEST("SF", "-(A2)",        0050342 | 0x100);
    TEST("SF", "($1234,A2)",   0050352 | 0x100, 0x1234);
    TEST("SF", "(18,A2,D3.W)", 0050362 | 0x100, 0x3012);
    ABST("SF", "(@A).W", "1234",   0050370 | 0x100, 0x1234);
    ABST("SF", "(@A).L", "123456", 0050371 | 0x100, 0x0012, 0x3456);
    if (firstGen()) {
        UNKN(0050372 | 0x100); // (*+$1234,PC)
        UNKN(0050373 | 0x100); // (*+10,PC,D3.W)
        UNKN(0050374 | 0x100); // #$34
    } else {
        TEST("TRAPF.W", "#$1234",     0050372 | 0x100, 0x1234);
        TEST("TRAPF.L", "#$12345678", 0050373 | 0x100, 0x1234, 0x5678);
        TEST("TRAPF",   "",           0050374 | 0x100);
    }
    TEST("SHI", "D2",           0050302 | 0x200);
    TEST("DBHI", "D2, *+2",     0050312 | 0x200, 0); // DBHI
    TEST("SHI", "(A2)",         0050322 | 0x200);
    TEST("SHI", "(A2)+",        0050332 | 0x200);
    TEST("SHI", "-(A2)",        0050342 | 0x200);
    TEST("SHI", "($1234,A2)",   0050352 | 0x200, 0x1234);
    TEST("SHI", "(18,A2,D3.W)", 0050362 | 0x200, 0x3012);
    ABST("SHI", "(@A).W", "1234",   0050370 | 0x200, 0x1234);
    ABST("SHI", "(@A).L", "123456", 0050371 | 0x200, 0x0012, 0x3456);
    if (firstGen()) {
        UNKN(0050372 | 0x200); // (*+$1234,PC)
        UNKN(0050373 | 0x200); // (*+10,PC,D3.W)
        UNKN(0050374 | 0x200); // #$34
    } else {
        TEST("TRAPHI.W", "#$1234",     0050372 | 0x200, 0x1234);
        TEST("TRAPHI.L", "#$12345678", 0050373 | 0x200, 0x1234, 0x5678);
        TEST("TRAPHI",   "",           0050374 | 0x200);
    }
    TEST("SLS", "D2",           0050302 | 0x300);
    TEST("DBLS", "D2, *+2",     0050312 | 0x300, 0); // DBLS
    TEST("SLS", "(A2)",         0050322 | 0x300);
    TEST("SLS", "(A2)+",        0050332 | 0x300);
    TEST("SLS", "-(A2)",        0050342 | 0x300);
    TEST("SLS", "($1234,A2)",   0050352 | 0x300, 0x1234);
    TEST("SLS", "(18,A2,D3.W)", 0050362 | 0x300, 0x3012);
    ABST("SLS", "(@A).W", "1234",   0050370 | 0x300, 0x1234);
    ABST("SLS", "(@A).L", "123456", 0050371 | 0x300, 0x0012, 0x3456);
    if (firstGen()) {
        UNKN(0050372 | 0x300); // (*+$1234,PC)
        UNKN(0050373 | 0x300); // (*+10,PC,D3.W)
        UNKN(0050374 | 0x300); // #$34
    } else {
        TEST("TRAPLS.W", "#$1234",     0050372 | 0x300, 0x1234);
        TEST("TRAPLS.L", "#$12345678", 0050373 | 0x300, 0x1234, 0x5678);
        TEST("TRAPLS",   "",           0050374 | 0x300);
    }
    TEST("SCC", "D2",           0050302 | 0x400);
    TEST("DBCC", "D2, *+2",     0050312 | 0x400, 0); // DBCC
    TEST("SCC", "(A2)",         0050322 | 0x400);
    TEST("SCC", "(A2)+",        0050332 | 0x400);
    TEST("SCC", "-(A2)",        0050342 | 0x400);
    TEST("SCC", "($1234,A2)",   0050352 | 0x400, 0x1234);
    TEST("SCC", "(18,A2,D3.W)", 0050362 | 0x400, 0x3012);
    ABST("SCC", "(@A).W", "1234",   0050370 | 0x400, 0x1234);
    ABST("SCC", "(@A).L", "123456", 0050371 | 0x400, 0x0012, 0x3456);
    if (firstGen()) {
        UNKN(0050372 | 0x400); // (*+$1234,PC)
        UNKN(0050373 | 0x400); // (*+10,PC,D3.W)
        UNKN(0050374 | 0x400); // #$34
    } else {
        TEST("TRAPCC.W", "#$1234",     0050372 | 0x400, 0x1234);
        TEST("TRAPCC.L", "#$12345678", 0050373 | 0x400, 0x1234, 0x5678);
        TEST("TRAPCC",   "",           0050374 | 0x400);
    }
    TEST("SCS", "D2",           0050302 | 0x500);
    TEST("DBCS", "D2, *+2",     0050312 | 0x500, 0); // DBCS
    TEST("SCS", "(A2)",         0050322 | 0x500);
    TEST("SCS", "(A2)+",        0050332 | 0x500);
    TEST("SCS", "-(A2)",        0050342 | 0x500);
    TEST("SCS", "($1234,A2)",   0050352 | 0x500, 0x1234);
    TEST("SCS", "(18,A2,D3.W)", 0050362 | 0x500, 0x3012);
    ABST("SCS", "(@A).W", "1234",   0050370 | 0x500, 0x1234);
    ABST("SCS", "(@A).L", "123456", 0050371 | 0x500, 0x0012, 0x3456);
    if (firstGen()) {
        UNKN(0050372 | 0x500); // (*+$1234,PC)
        UNKN(0050373 | 0x500); // (*+10,PC,D3.W)
        UNKN(0050374 | 0x500); // #$34
    } else {
        TEST("TRAPCS.W", "#$1234",     0050372 | 0x500, 0x1234);
        TEST("TRAPCS.L", "#$12345678", 0050373 | 0x500, 0x1234, 0x5678);
        TEST("TRAPCS",   "",           0050374 | 0x500);
    }
    TEST("SNE", "D2",           0050302 | 0x600);
    TEST("DBNE", "D2, *+2",     0050312 | 0x600, 0); // DBNE
    TEST("SNE", "(A2)",         0050322 | 0x600);
    TEST("SNE", "(A2)+",        0050332 | 0x600);
    TEST("SNE", "-(A2)",        0050342 | 0x600);
    TEST("SNE", "($1234,A2)",   0050352 | 0x600, 0x1234);
    TEST("SNE", "(18,A2,D3.W)", 0050362 | 0x600, 0x3012);
    ABST("SNE", "(@A).W", "1234",   0050370 | 0x600, 0x1234);
    ABST("SNE", "(@A).L", "123456", 0050371 | 0x600, 0x0012, 0x3456);
    if (firstGen()) {
        UNKN(0050372 | 0x600); // (*+$1234,PC)
        UNKN(0050373 | 0x600); // (*+10,PC,D3.W)
        UNKN(0050374 | 0x600); // #$34
    } else {
        TEST("TRAPNE.W", "#$1234",     0050372 | 0x600, 0x1234);
        TEST("TRAPNE.L", "#$12345678", 0050373 | 0x600, 0x1234, 0x5678);
        TEST("TRAPNE",   "",           0050374 | 0x600);
    }
    TEST("SEQ", "D2",           0050302 | 0x700);
    TEST("DBEQ", "D2, *+2",     0050312 | 0x700, 0); // DBEQ
    TEST("SEQ", "(A2)",         0050322 | 0x700);
    TEST("SEQ", "(A2)+",        0050332 | 0x700);
    TEST("SEQ", "-(A2)",        0050342 | 0x700);
    TEST("SEQ", "($1234,A2)",   0050352 | 0x700, 0x1234);
    TEST("SEQ", "(18,A2,D3.W)", 0050362 | 0x700, 0x3012);
    ABST("SEQ", "(@A).W", "1234",   0050370 | 0x700, 0x1234);
    ABST("SEQ", "(@A).L", "123456", 0050371 | 0x700, 0x0012, 0x3456);
    if (firstGen()) {
        UNKN(0050372 | 0x700); // (*+$1234,PC)
        UNKN(0050373 | 0x700); // (*+10,PC,D3.W)
        UNKN(0050374 | 0x700); // #$34
    } else {
        TEST("TRAPEQ.W", "#$1234",     0050372 | 0x700, 0x1234);
        TEST("TRAPEQ.L", "#$12345678", 0050373 | 0x700, 0x1234, 0x5678);
        TEST("TRAPEQ",   "",           0050374 | 0x700);
    }
    TEST("SVC", "D2",           0050302 | 0x800);
    TEST("DBVC", "D2, *+2",     0050312 | 0x800, 0); // DBVC
    TEST("SVC", "(A2)",         0050322 | 0x800);
    TEST("SVC", "(A2)+",        0050332 | 0x800);
    TEST("SVC", "-(A2)",        0050342 | 0x800);
    TEST("SVC", "($1234,A2)",   0050352 | 0x800, 0x1234);
    TEST("SVC", "(18,A2,D3.W)", 0050362 | 0x800, 0x3012);
    ABST("SVC", "(@A).W", "1234",   0050370 | 0x800, 0x1234);
    ABST("SVC", "(@A).L", "123456", 0050371 | 0x800, 0x0012, 0x3456);
    if (firstGen()) {
        UNKN(0050372 | 0x800); // (*+$1234,PC)
        UNKN(0050373 | 0x800); // (*+10,PC,D3.W)
        UNKN(0050374 | 0x800); // #$34
    } else {
        TEST("TRAPVC.W", "#$1234",     0050372 | 0x800, 0x1234);
        TEST("TRAPVC.L", "#$12345678", 0050373 | 0x800, 0x1234, 0x5678);
        TEST("TRAPVC",   "",           0050374 | 0x800);
    }
    TEST("SVS", "D2",           0050302 | 0x900);
    TEST("DBVS", "D2, *+2",     0050312 | 0x900, 0); // DBVS
    TEST("SVS", "(A2)",         0050322 | 0x900);
    TEST("SVS", "(A2)+",        0050332 | 0x900);
    TEST("SVS", "-(A2)",        0050342 | 0x900);
    TEST("SVS", "($1234,A2)",   0050352 | 0x900, 0x1234);
    TEST("SVS", "(18,A2,D3.W)", 0050362 | 0x900, 0x3012);
    ABST("SVS", "(@A).W", "1234",   0050370 | 0x900, 0x1234);
    ABST("SVS", "(@A).L", "123456", 0050371 | 0x900, 0x0012, 0x3456);
    if (firstGen()) {
        UNKN(0050372 | 0x900); // (*+$1234,PC)
        UNKN(0050373 | 0x900); // (*+10,PC,D3.W)
        UNKN(0050374 | 0x900); // #$34
    } else {
        TEST("TRAPVS.W", "#$1234",     0050372 | 0x900, 0x1234);
        TEST("TRAPVS.L", "#$12345678", 0050373 | 0x900, 0x1234, 0x5678);
        TEST("TRAPVS",   "",           0050374 | 0x900);
    }
    TEST("SPL", "D2",           0050302 | 0xA00);
    TEST("DBPL", "D2, *+2",     0050312 | 0xA00, 0); // DBPL
    TEST("SPL", "(A2)",         0050322 | 0xA00);
    TEST("SPL", "(A2)+",        0050332 | 0xA00);
    TEST("SPL", "-(A2)",        0050342 | 0xA00);
    TEST("SPL", "($1234,A2)",   0050352 | 0xA00, 0x1234);
    TEST("SPL", "(18,A2,D3.W)", 0050362 | 0xA00, 0x3012);
    ABST("SPL", "(@A).W", "1234",   0050370 | 0xA00, 0x1234);
    ABST("SPL", "(@A).L", "123457", 0050371 | 0xA00, 0x0012, 0x3457);
    if (firstGen()) {
        UNKN(0050372 | 0xA00); // (*+$1234,PC)
        UNKN(0050373 | 0xA00); // (*+10,PC,D3.W)
        UNKN(0050374 | 0xA00); // #$34
    } else {
        TEST("TRAPPL.W", "#$1234",     0050372 | 0xA00, 0x1234);
        TEST("TRAPPL.L", "#$12345678", 0050373 | 0xA00, 0x1234, 0x5678);
        TEST("TRAPPL",   "",           0050374 | 0xA00);
    }
    TEST("SMI", "D2",           0050302 | 0xB00);
    TEST("DBMI", "D2, *+2",     0050312 | 0xB00, 0); // DBMI
    TEST("SMI", "(A2)",         0050322 | 0xB00);
    TEST("SMI", "(A2)+",        0050332 | 0xB00);
    TEST("SMI", "-(A2)",        0050342 | 0xB00);
    TEST("SMI", "($1234,A2)",   0050352 | 0xB00, 0x1234);
    TEST("SMI", "(18,A2,D3.W)", 0050362 | 0xB00, 0x3012);
    ABST("SMI", "(@A).W", "1234",   0050370 | 0xB00, 0x1234);
    ABST("SMI", "(@A).L", "123456", 0050371 | 0xB00, 0x0012, 0x3456);
    if (firstGen()) {
        UNKN(0050372 | 0xB00); // (*+$1234,PC)
        UNKN(0050373 | 0xB00); // (*+10,PC,D3.W)
        UNKN(0050374 | 0xB00); // #$34
    } else {
        TEST("TRAPMI.W", "#$1234",     0050372 | 0xB00, 0x1234);
        TEST("TRAPMI.L", "#$12345678", 0050373 | 0xB00, 0x1234, 0x5678);
        TEST("TRAPMI",   "",           0050374 | 0xB00);
    }
    TEST("SGE", "D2",           0050302 | 0xC00);
    TEST("DBGE", "D2, *+2",     0050312 | 0xC00, 0); // DBGE
    TEST("SGE", "(A2)",         0050322 | 0xC00);
    TEST("SGE", "(A2)+",        0050332 | 0xC00);
    TEST("SGE", "-(A2)",        0050342 | 0xC00);
    TEST("SGE", "($1234,A2)",   0050352 | 0xC00, 0x1234);
    TEST("SGE", "(18,A2,D3.W)", 0050362 | 0xC00, 0x3012);
    ABST("SGE", "(@A).W", "1234",   0050370 | 0xC00, 0x1234);
    ABST("SGE", "(@A).L", "123456", 0050371 | 0xC00, 0x0012, 0x3456);
    if (firstGen()) {
        UNKN(0050372 | 0xC00); // (*+$1234,PC)
        UNKN(0050373 | 0xC00); // (*+10,PC,D3.W)
        UNKN(0050374 | 0xC00); // #$34
    } else {
        TEST("TRAPGE.W", "#$1234",     0050372 | 0xC00, 0x1234);
        TEST("TRAPGE.L", "#$12345678", 0050373 | 0xC00, 0x1234, 0x5678);
        TEST("TRAPGE",   "",           0050374 | 0xC00);
    }
    TEST("SLT", "D2",           0050302 | 0xD00);
    TEST("DBLT", "D2, *+2",     0050312 | 0xD00, 0); // DBLT
    TEST("SLT", "(A2)",         0050322 | 0xD00);
    TEST("SLT", "(A2)+",        0050332 | 0xD00);
    TEST("SLT", "-(A2)",        0050342 | 0xD00);
    TEST("SLT", "($1234,A2)",   0050352 | 0xD00, 0x1234);
    TEST("SLT", "(18,A2,D3.W)", 0050362 | 0xD00, 0x3012);
    ABST("SLT", "(@A).W", "1234",   0050370 | 0xD00, 0x1234);
    ABST("SLT", "(@A).L", "123456", 0050371 | 0xD00, 0x0012, 0x3456);
    if (firstGen()) {
        UNKN(0050372 | 0xD00); // (*+$1234,PC)
        UNKN(0050373 | 0xD00); // (*+10,PC,D3.W)
        UNKN(0050374 | 0xD00); // #$34
    } else {
        TEST("TRAPLT.W", "#$1234",     0050372 | 0xD00, 0x1234);
        TEST("TRAPLT.L", "#$12345678", 0050373 | 0xD00, 0x1234, 0x5678);
        TEST("TRAPLT",   "",           0050374 | 0xD00);
    }
    TEST("SGT", "D2",           0050302 | 0xE00);
    TEST("DBGT", "D2, *+2",     0050312 | 0xE00, 0); // DBGT
    TEST("SGT", "(A2)",         0050322 | 0xE00);
    TEST("SGT", "(A2)+",        0050332 | 0xE00);
    TEST("SGT", "-(A2)",        0050342 | 0xE00);
    TEST("SGT", "($1234,A2)",   0050352 | 0xE00, 0x1234);
    TEST("SGT", "(18,A2,D3.W)", 0050362 | 0xE00, 0x3012);
    ABST("SGT", "(@A).W", "1234",   0050370 | 0xE00, 0x1234);
    ABST("SGT", "(@A).L", "123456", 0050371 | 0xE00, 0x0012, 0x3456);
    if (firstGen()) {
        UNKN(0050372 | 0xE00); // (*+$1234,PC)
        UNKN(0050373 | 0xE00); // (*+10,PC,D3.W)
        UNKN(0050374 | 0xE00); // #$34
    } else {
        TEST("TRAPGT.W", "#$1234",     0050372 | 0xE00, 0x1234);
        TEST("TRAPGT.L", "#$12345678", 0050373 | 0xE00, 0x1234, 0x5678);
        TEST("TRAPGT",   "",           0050374 | 0xE00);
    }
    TEST("SLE", "D2",           0050302 | 0xF00);
    TEST("DBLE", "D2, *+2",     0050312 | 0xF00, 0); // DBLE
    TEST("SLE", "(A2)",         0050322 | 0xF00);
    TEST("SLE", "(A2)+",        0050332 | 0xF00);
    TEST("SLE", "-(A2)",        0050342 | 0xF00);
    TEST("SLE", "($1234,A2)",   0050352 | 0xF00, 0x1234);
    TEST("SLE", "(18,A2,D3.W)", 0050362 | 0xF00, 0x3012);
    ABST("SLE", "(@A).W", "1234",   0050370 | 0xF00, 0x1234);
    ABST("SLE", "(@A).L", "123456", 0050371 | 0xF00, 0x0012, 0x3456);
    if (firstGen()) {
        UNKN(0050372 | 0xF00); // (*+$1234,PC)
        UNKN(0050373 | 0xF00); // (*+10,PC,D3.W)
        UNKN(0050374 | 0xF00); // #$34
    } else {
        TEST("TRAPLE.W", "#$1234",     0050372 | 0xF00, 0x1234);
        TEST("TRAPLE.L", "#$12345678", 0050373 | 0xF00, 0x1234, 0x5678);
        TEST("TRAPLE",   "",           0050374 | 0xF00);
    }

    // BRA label: 00600|disp
    ATEST(0x10000, "BRA",   "*-$7FFE", 0060000, 0x8000);
    ATEST(0x10000, "BRA.W", "*-$007E", 0060000, 0xFF80);
    ATEST(0x10000, "BRA",   "*-126",   0060000 | 0x80);
    ATEST(0x10000, "BRA",   "*",       0060000 | 0xFE);
    ATEST(0x10000, "BRA.W", "*",       0060000, 0xFFFE);
    ATEST(0x10000, "BRA.W", "*+2",     0060000, 0x0000);
    ATEST(0x10000, "BRA",   "*+128",   0060000 | 0x7E);
    ATEST(0x10000, "BRA.W", "*+$0080", 0060000, 0x007E);
    ATEST(0x10000, "BRA", "*+$8000",   0060000, 0x7FFE);
    AERRT(0x10000, "BRA", "*-$7FFD", OPERAND_NOT_ALIGNED, "*-$7FFD", 0060000, 0x8001);
    AERRT(0x10000, "BRA", "*-125",   OPERAND_NOT_ALIGNED, "*-125",   0060000 | 0x81);
    if (firstGen()) {
        AERRT(0x10000, "BRA", "*+1", OPERAND_NOT_ALIGNED, "*+1",     0060000 | 0xFF);
    } else {
        ATEST(0x10000, "BRA.X", "*", 0060000 | 0xFF, 0xFFFF, 0xFFFE);
    }
    AERRT(0x10000, "BRA", "*+3",     OPERAND_NOT_ALIGNED, "*+3",     0060000 | 0x01);
    AERRT(0x10000, "BRA", "*+129",   OPERAND_NOT_ALIGNED, "*+129",   0060000 | 0x7F);
    AERRT(0x10000, "BRA", "*+$8001", OPERAND_NOT_ALIGNED, "*+$8001", 0060000, 0x7FFF);

    // BSR label: 00604|disp
    ATEST(0x10000, "BSR"  , "*-$7FFE", 0060400, 0x8000);
    ATEST(0x10000, "BSR.W", "*-$007E", 0060400, 0xFF80);
    ATEST(0x10000, "BSR",   "*-126",   0060400 | 0x80);
    ATEST(0x10000, "BSR",   "*",       0060400 | 0xFE);
    ATEST(0x10000, "BSR.W", "*",       0060400, 0xFFFE);
    ATEST(0x10000, "BSR.W", "*+2",     0060400, 0x0000);
    ATEST(0x10000, "BSR",   "*+128",   0060400 | 0x7E);
    ATEST(0x10000, "BSR.W", "*+$0080", 0060400, 0x007E);
    ATEST(0x10000, "BSR",   "*+$8000", 0060400, 0x7FFE);
    AERRT(0x10000, "BSR", "*-$7FFD", OPERAND_NOT_ALIGNED, "*-$7FFD", 0060400, 0x8001);
    AERRT(0x10000, "BSR", "*-125",   OPERAND_NOT_ALIGNED, "*-125",   0060400 | 0x81);
    if (firstGen()) {
        AERRT(0x10000, "BSR", "*+1", OPERAND_NOT_ALIGNED, "*+1",     0060400 | 0xFF);
    } else {
        ATEST(0x10000, "BSR.X", "*+6", 0060400 | 0xFF, 0x0000, 0x0004);
    }
    AERRT(0x10000, "BSR", "*+3",     OPERAND_NOT_ALIGNED, "*+3",     0060400 | 0x01);
    AERRT(0x10000, "BSR", "*+129",   OPERAND_NOT_ALIGNED, "*+129",   0060400 | 0x7F);
    AERRT(0x10000, "BSR", "*+$8001", OPERAND_NOT_ALIGNED, "*+$8001", 0060400, 0x7FFF);

    disassembler.setOption("gnu-as", "on");

    ATEST(0x10000, "BRA",  ".-0x7FFE", 0060000, 0x8000);
    ATEST(0x10000, "BRA",  ".-0x007E", 0060000, 0xFF80);
    ATEST(0x10000, "BRAS", ".-126",    0060000 | 0x80);
    ATEST(0x10000, "BRAS", ".",        0060000 | 0xFE);
    ATEST(0x10000, "BRA",  ".",        0060000, 0xFFFE);
    ATEST(0x10000, "BRA",  ".+2",      0060000, 0x0000);
    ATEST(0x10000, "BRAS", ".+128",    0060000 | 0x7E);
    ATEST(0x10000, "BRA",  ".+0x0080", 0060000, 0x007E);
    ATEST(0x10000, "BRA",  ".+0x8000", 0060000, 0x7FFE);
    if (!firstGen()) {
        ATEST(0x10000, "BRAL", ".", 0060000 | 0xFF, 0xFFFF, 0xFFFE);
    }

    ATEST(0x10000, "BSR",  ".-0x7FFE", 0060400, 0x8000);
    ATEST(0x10000, "BSR",  ".-0x007E", 0060400, 0xFF80);
    ATEST(0x10000, "BSRS", ".-126",    0060400 | 0x80);
    ATEST(0x10000, "BSRS", ".",        0060400 | 0xFE);
    ATEST(0x10000, "BSR",  ".",        0060400, 0xFFFE);
    ATEST(0x10000, "BSR",  ".+2",      0060400, 0x0000);
    ATEST(0x10000, "BSRS", ".+128",    0060400 | 0x7E);
    ATEST(0x10000, "BSR",  ".+0x0080", 0060400, 0x007E);
    ATEST(0x10000, "BSR",  ".+0x8000", 0060400, 0x7FFE);
    if (!firstGen()) {
        ATEST(0x10000, "BSRL", ".+6", 0060400 | 0xFF, 0x0000, 0x0004);
    }

    disassembler.setOption("gnu-as", "off");

    // JMP dst: 00473|M|Rn
    UNKN(0047302); // D2
    UNKN(0047312); // A2
    TEST("JMP", "(A2)",           0047322);
    UNKN(0047332); // (A2)+
    UNKN(0047342); // -(A2)
    TEST("JMP", "($1234,A2)",     0047352, 0x1234);
    TEST("JMP", "(18,A2,D3.W)",   0047362, 0x3012);
    ABST("JMP", "(@A).W", "1234",   0047370, 0x1234);
    ABST("JMP", "(@A).L", "123456", 0047371, 0x0012, 0x3456);
    TEST("JMP", "(*+$1234,PC)",   0047372, 0x1232);
    TEST("JMP", "(*+18,PC,D3.L)", 0047373, 0x3810);
    UNKN(0047374); // #$1234

    // JSR dst, 00472|M|Rn
    UNKN(0047202); // D2
    UNKN(0047212); // A2
    TEST("JSR", "(A2)",           0047222);
    UNKN(0047232); // (A2)+
    UNKN(0047242); // -(A2)
    TEST("JSR", "($1234,A2)",     0047252, 0x1234);
    TEST("JSR", "(18,A2,D3.W)",   0047262, 0x3012);
    ABST("JSR", "(@A).W", "1234",   0047270, 0x1234);
    ABST("JSR", "(@A).L", "123456", 0047271, 0x0012, 0x3456);
    TEST("JSR", "(*+$1234,PC)",   0047272, 0x1232);
    TEST("JSR", "(*+18,PC,D3.L)", 0047273, 0x3810);
    UNKN(0047274); // #$1234

    if (mc68020()) {
        // RETM Dn:       00033|0|Dn
        // RETM An:       00033|1|An
        // CALLM #n8,dst: 00033|M|Rn
        TEST("RTM", "D2",           0003302);
        TEST("RTM", "A2",           0003312);
        TEST("CALLM", "#$FF, (A2)", 0003322, 0x00FF);
        UNKN(0003332); // (A2)+
        UNKN(0003342); // -(A2)
        TEST("CALLM", "#0, ($1234,A2)",       0003352, 0x0000, 0x1234);
        TEST("CALLM", "#1, (18,A2,D3.W)",     0003362, 0x0001, 0x3012);
        ABST("CALLM", "#2, (@A).W", "1234",   0003370, 0x0002, 0x1234);
        ABST("CALLM", "#3, (@A).L", "123456", 0003371, 0x0003, 0x0012, 0x3456);
        TEST("CALLM", "#4, (*+$1234,PC)",     0003372, 0x0004, 0x1230);
        TEST("CALLM", "#5, (*+18,PC,D3.L)",   0003373, 0x0005, 0x380E);
        UNKN(0003374); // #$1234
    }

    // TST dst: 0045|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("TST.B", "D2",           0045002);
    UNKN(0045012); // A2
    TEST("TST.B", "(A2)",         0045022);
    TEST("TST.B", "(A2)+",        0045032);
    TEST("TST.B", "-(A2)",        0045042);
    TEST("TST.B", "($1234,A2)",   0045052, 0x1234);
    TEST("TST.B", "(18,A2,D3.W)", 0045062, 0x3012);
    ABST("TST.B", "(@A).W", "1234",   0045070, 0x1234);
    ABST("TST.B", "(@A).L", "123456", 0045071, 0x0012, 0x3456);
    UNKN(0045072); // (*+$1234,PC)
    UNKN(0045073); // (*+10,PC,D3.W)
    UNKN(0045074); // #$34
    TEST("TST.W", "D2",           0045102);
    UNKN(0045112); // A2
    TEST("TST.W", "(A2)",         0045122);
    TEST("TST.W", "(A2)+",        0045132);
    TEST("TST.W", "-(A2)",        0045142);
    TEST("TST.W", "($1234,A2)",   0045152, 0x1234);
    TEST("TST.W", "(18,A2,D3.W)", 0045162, 0x3012);
    ABST("TST.W", "(@A).W", "1234",   0045170, 0x1234);
    ABST("TST.W", "(@A).L", "123456", 0045171, 0x0012, 0x3456);
    UNKN(0045172); // (*+$1234,PC)
    UNKN(0045173); // (*+10,PC,D3.W)
    UNKN(0045174); // #$1234
    TEST("TST.L", "D2",           0045202);
    UNKN(0045212); // A2
    TEST("TST.L", "(A2)",         0045222);
    TEST("TST.L", "(A2)+",        0045232);
    TEST("TST.L", "-(A2)",        0045242);
    TEST("TST.L", "($1234,A2)",   0045252, 0x1234);
    TEST("TST.L", "(18,A2,D3.W)", 0045262, 0x3012);
    ABST("TST.L", "(@A).W", "1234",   0045270, 0x1234);
    ABST("TST.L", "(@A).L", "123454", 0045271, 0x0012, 0x3454);
    UNKN(0045272); // (*+$1234,PC)
    UNKN(0045273); // (*+10,PC,D3.W)
    UNKN(0045274); // #$12345678
}

void test_system() {
    // ANDI #nn,SR
    TEST("ANDI", "#$1234, SR", 0001174, 0x1234);

    // EORI #nn,SR
    TEST("EORI", "#$1234, SR", 0005174, 0x1234);

    // MOVE src,SR: 00433|M|Rn
    TEST("MOVE.W", "D2, SR",                                0043302);
    UNKN(0043312); // A2, SR
    TEST("MOVE.W", "(A2), SR",                              0043322);
    TEST("MOVE.W", "(A2)+, SR",                             0043332);
    TEST("MOVE.W", "-(A2), SR",                             0043342);
    TEST("MOVE.W", "($1234,A2), SR",                        0043352, 0x1234);
    TEST("MOVE.W", "(18,A2,D3.W), SR",                      0043362, 0x3012);
    ABST("MOVE.W", "(@A).W, SR", "1234",                                    0043370, 0x1234);
    EABS("MOVE.W", "(@A).W, SR", "1233", OPERAND_NOT_ALIGNED, "(@A).W, SR", 0043370, 0x1233);
    ABST("MOVE.W", "(@A).L, SR", "234568",                                    0043371, 0x0023, 0x4568);
    EABS("MOVE.W", "(@A).L, SR", "234567", OPERAND_NOT_ALIGNED, "(@A).L, SR", 0043371, 0x0023, 0x4567);
    TEST("MOVE.W", "(*+$1234,PC), SR",                                          0043372, 0x1232);
    ERRT("MOVE.W", "(*+$1235,PC), SR", OPERAND_NOT_ALIGNED, "(*+$1235,PC), SR", 0043372, 0x1233);
    TEST("MOVE.W", "(*-16,PC,D3.L), SR",                    0043373, 0x38EE);
    TEST("MOVE.W", "#$3456, SR",                            0043374, 0x3456);

    // MOVE SR,dst: 00403|M|Rn
    TEST("MOVE.W", "SR, D2",                               0040302);
    UNKN(0040312); // SR, A2
    TEST("MOVE.W", "SR, (A2)",                             0040322);
    TEST("MOVE.W", "SR, (A2)+",                            0040332);
    TEST("MOVE.W", "SR, -(A2)",                            0040342);
    TEST("MOVE.W", "SR, ($1234,A2)",                       0040352, 0x1234);
    TEST("MOVE.W", "SR, (18,A2,D3.W)",                     0040362, 0x3012);
    ABST("MOVE.W", "SR, (@A).W", "1234",                                0040370, 0x1234);
    EABS("MOVE.W", "SR, (@A).W", "1233", OPERAND_NOT_ALIGNED, "(@A).W", 0040370, 0x1233);
    ABST("MOVE.W", "SR, (@A).L", "234568",                                0040371, 0x0023, 0x4568);
    EABS("MOVE.W", "SR, (@A).L", "234567", OPERAND_NOT_ALIGNED, "(@A).L", 0040371, 0x0023, 0x4567);
    UNKN(0040372); // SR, (*+$1234,PC)
    UNKN(0040373); // SR, (*+10,PC,D3.W)
    UNKN(0040374); // SR, #$1234

    // MOVE USP,An: 004715|An
    TEST("MOVE", "USP, A2", 0047152);

    // MOVE An,USP: 004714|An
    TEST("MOVE", "A2, USP", 0047142);

    // ORI #nn,SR
    TEST("ORI", "#$1234, SR", 0000174, 0x1234);

    // RESET
    TEST("RESET", "", 047160);

    // NOP
    TEST("NOP", "", 047161);

    // STOP #nn
    TEST("STOP", "#$1234", 047162, 0x1234);

    // RTE
    TEST("RTE", "", 047163);

    // RTD #nn
    if (mc68k00()) {
        UNKN(047164);
    } else {
        TEST("RTD", "#$1234", 047164, 0x1234);
    }

    // RTS
    TEST("RTS", "", 047165);

    // TRAPV
    TEST("TRAPV", "", 047166);

    // RTR
    TEST("RTR", "", 047167);

    // MOVEC
    if (mc68k00()) {
        UNKN(047172);
        UNKN(047173);
    } else if (mc68010()) {
        TEST("MOVEC", "SFC, D1", 0047172, 0010000 | 0x000);
        TEST("MOVEC", "DFC, A2", 0047172, 0120000 | 0x001);
        ERRT("MOVEC", ", D2",    ILLEGAL_REGISTER, ", D2", 0047172, 0020000 | 0x002);
        ERRT("MOVEC", ", A2",    ILLEGAL_REGISTER, ", A2", 0047172, 0120000 | 0x003);
        TEST("MOVEC", "USP, D3", 0047172, 0030000 | 0x800);
        TEST("MOVEC", "VBR, A4", 0047172, 0140000 | 0x801);
        ERRT("MOVEC", ", D4",    ILLEGAL_REGISTER, ", D4", 0047172, 0040000 | 0x802);
        ERRT("MOVEC", ", A5",    ILLEGAL_REGISTER, ", A5", 0047172, 0150000 | 0x803);
        ERRT("MOVEC", ", D5",    ILLEGAL_REGISTER, ", D5", 0047172, 0050000 | 0x804);
        ERRT("MOVEC", ", A6",    ILLEGAL_REGISTER, ", A6", 0047172, 0160000 | 0x805);
        TEST("MOVEC", "D5, SFC", 0047173, 0050000 | 0x000);
        TEST("MOVEC", "A6, DFC", 0047173, 0160000 | 0x001);
        ERRT("MOVEC", "D7, ",    ILLEGAL_REGISTER, "", 0047173, 0070000 | 0x002);
        ERRT("MOVEC", "A7, ",    ILLEGAL_REGISTER, "", 0047173, 0170000 | 0x003);
        TEST("MOVEC", "A0, USP", 0047173, 0100000 | 0x800);
        TEST("MOVEC", "D1, VBR", 0047173, 0010000 | 0x801);
        ERRT("MOVEC", "A2, ",    ILLEGAL_REGISTER, "", 0047173, 0120000 | 0x802);
        ERRT("MOVEC", "D2, ",    ILLEGAL_REGISTER, "", 0047173, 0020000 | 0x803);
        ERRT("MOVEC", "A3, ",    ILLEGAL_REGISTER, "", 0047173, 0130000 | 0x804);
        ERRT("MOVEC", "D3, ",    ILLEGAL_REGISTER, "", 0047173, 0030000 | 0x805);
    } else {
        TEST("MOVEC", "SFC, D1",  0047172, 0010000 | 0x000);
        TEST("MOVEC", "DFC, A2",  0047172, 0120000 | 0x001);
        TEST("MOVEC", "CACR, D2", 0047172, 0020000 | 0x002);
        ERRT("MOVEC", ", A2",     ILLEGAL_REGISTER, ", A2", 0047172, 0120000 | 0x003);
        TEST("MOVEC", "USP, D3",  0047172, 0030000 | 0x800);
        TEST("MOVEC", "VBR, A4",  0047172, 0140000 | 0x801);
        TEST("MOVEC", "CAAR, D4", 0047172, 0040000 | 0x802);
        TEST("MOVEC", "MSP, A5",  0047172, 0150000 | 0x803);
        TEST("MOVEC", "ISP, D5",  0047172, 0050000 | 0x804);
        ERRT("MOVEC", ", A6",     ILLEGAL_REGISTER, ", A6", 0047172, 0160000 | 0x805);
        TEST("MOVEC", "D5, SFC",  0047173, 0050000 | 0x000);
        TEST("MOVEC", "A6, DFC",  0047173, 0160000 | 0x001);
        TEST("MOVEC", "D7, CACR", 0047173, 0070000 | 0x002);
        ERRT("MOVEC", "A7, ",     ILLEGAL_REGISTER, "", 0047173, 0170000 | 0x003);
        TEST("MOVEC", "A0, USP",  0047173, 0100000 | 0x800);
        TEST("MOVEC", "D1, VBR",  0047173, 0010000 | 0x801);
        TEST("MOVEC", "A2, CAAR", 0047173, 0120000 | 0x802);
        TEST("MOVEC", "D2, MSP",  0047173, 0020000 | 0x803);
        TEST("MOVEC", "A3, ISP",  0047173, 0130000 | 0x804);
        ERRT("MOVEC", "D3, ",     ILLEGAL_REGISTER, "", 0047173, 0030000 | 0x805);
    }

    // CHK src,Dn: 004|Dn|Sz|M|Rn, Sz:W=6/L=7
    TEST("CHK.W", "D2, D7",             0047602);
    UNKN(0047612); // A2, D7
    TEST("CHK.W", "(A2), D7",           0047622);
    TEST("CHK.W", "(A2)+, D7",          0047632);
    TEST("CHK.W", "-(A2), D7",          0047642);
    TEST("CHK.W", "($1234,A2), D7",     0047652, 0x1234);
    TEST("CHK.W", "(18,A2,D3.L), D7",   0047662, 0x3812);
    ABST("CHK.W", "(@A).W, D7", "FFFE",   0047670, 0xFFFE);
    ABST("CHK.W", "(@A).L, D7", "123456", 0047671, 0x0012, 0x3456);
    TEST("CHK.W", "(*+$1234,PC), D7",   0047672, 0x1232);
    TEST("CHK.W", "(*+18,PC,D3.W), D7", 0047673, 0x3010);
    TEST("CHK.W", "#$0034, D7",         0047674, 0x0034);
    if (!firstGen()) {
        TEST("CHK.L", "D2, D7",               0047402);
        UNKN(0047412); // A2, D7
        TEST("CHK.L", "(A2), D7",             0047422);
        TEST("CHK.L", "(A2)+, D7",            0047432);
        TEST("CHK.L", "-(A2), D7",            0047442);
        TEST("CHK.L", "($1234,A2), D7",       0047452, 0x1234);
        TEST("CHK.L", "(18,A2,D3.L), D7",     0047462, 0x3812);
        ABST("CHK.L", "(@A).W, D7", "FFFE",   0047470, 0xFFFE);
        ABST("CHK.L", "(@A).L, D7", "123456", 0047471, 0x0012, 0x3456);
        TEST("CHK.L", "(*+$1234,PC), D7",     0047472, 0x1232);
        TEST("CHK.L", "(*+18,PC,D3.W), D7",   0047473, 0x3010);
        TEST("CHK.L", "#$12345678, D7",       0047474, 0x1234, 0x5678);
    }

    if (!firstGen()) {
        // CHK2 src,Rd, 00|Sz|3|M|Rn, Rd|40000, Sz:B=0/W=1/L=2
        UNKN(0000300);
        UNKN(0000310);
        TEST("CHK2.B", "(A2), D3",             0000322, 0034000);
        UNKN(0000332);
        UNKN(0000342);
        TEST("CHK2.B", "($1234,A2), A6",       0000352, 0164000, 0x1234);
        TEST("CHK2.B", "(18,A2,D3.L), D7",     0000362, 0074000, 0x3812);
        ABST("CHK2.B", "(@A).W, A0", "FFFE",   0000370, 0104000, 0xFFFE);
        ABST("CHK2.B", "(@A).L, D0", "123456", 0000371, 0004000, 0x0012, 0x3456);
        TEST("CHK2.B", "(*+$1234,PC), A1",     0000372, 0114000, 0x1230);
        TEST("CHK2.B", "(*+18,PC,D3.W), D2",   0000373, 0024000, 0x300E);
        UNKN(0000374);
        UNKN(0001300);
        UNKN(0001310);
        TEST("CHK2.W", "(A2), D3",             0001322, 0034000);
        UNKN(0001332);
        UNKN(0001342);
        TEST("CHK2.W", "($1234,A2), A6",       0001352, 0164000, 0x1234);
        TEST("CHK2.W", "(18,A2,D3.L), D7",     0001362, 0074000, 0x3812);
        ABST("CHK2.W", "(@A).W, A0", "FFFE",   0001370, 0104000, 0xFFFE);
        ABST("CHK2.W", "(@A).L, D0", "123456", 0001371, 0004000, 0x0012, 0x3456);
        TEST("CHK2.W", "(*+$1234,PC), A1",     0001372, 0114000, 0x1230);
        TEST("CHK2.W", "(*+18,PC,D3.W), D2",   0001373, 0024000, 0x300E);
        UNKN(0001374);
        UNKN(0002300);
        UNKN(0002310);
        TEST("CHK2.L", "(A2), D3",             0002322, 0034000);
        UNKN(0002332);
        UNKN(0002342);
        TEST("CHK2.L", "($1234,A2), A6",       0002352, 0164000, 0x1234);
        TEST("CHK2.L", "(18,A2,D3.L), D7",     0002362, 0074000, 0x3812);
        ABST("CHK2.L", "(@A).W, A0", "FFFE",   0002370, 0104000, 0xFFFE);
        ABST("CHK2.L", "(@A).L, D0", "123456", 0002371, 0004000, 0x0012, 0x3456);
        TEST("CHK2.L", "(*+$1234,PC), A1",     0002372, 0114000, 0x1230);
        TEST("CHK2.L", "(*+18,PC,D3.W), D2",   0002373, 0024000, 0x300E);
        UNKN(0002374);
    }

    // ILLEGAL
    TEST("ILLEGAL", "", 0045374);

    // TRAP #nn
    TEST("TRAP", "#0",  0047100);
    TEST("TRAP", "#15", 0047117);

    // ANDI #nn,CCR
    TEST("ANDI", "#$34, CCR", 0001074, 0x0034);

    // EORI #nn,CCR
    TEST("EORI", "#$34, CCR", 0005074, 0x0034);

    // MOVE src,CCR: 00423|M|R
    TEST("MOVE.W", "D2, CCR",                                0042302);
    UNKN(0042312); // A2, CCR
    TEST("MOVE.W", "(A2), CCR",                              0042322);
    TEST("MOVE.W", "(A2)+, CCR",                             0042332);
    TEST("MOVE.W", "-(A2), CCR",                             0042342);
    TEST("MOVE.W", "($1234,A2), CCR",                        0042352, 0x1234);
    TEST("MOVE.W", "(18,A2,D3.W), CCR",                      0042362, 0x3012);
    ABST("MOVE.W", "(@A).W, CCR", "1234",                                     0042370, 0x1234);
    EABS("MOVE.W", "(@A).W, CCR", "1235", OPERAND_NOT_ALIGNED, "(@A).W, CCR", 0042370, 0x1235);
    ABST("MOVE.W", "(@A).L, CCR", "234568",                                     0042371, 0x0023, 0x4568);
    EABS("MOVE.W", "(@A).L, CCR", "234569", OPERAND_NOT_ALIGNED, "(@A).L, CCR", 0042371, 0x0023, 0x4569);
    TEST("MOVE.W", "(*+$1234,PC), CCR",                                           0042372, 0x1232);
    ERRT("MOVE.W", "(*+$1235,PC), CCR", OPERAND_NOT_ALIGNED, "(*+$1235,PC), CCR", 0042372, 0x1233);
    TEST("MOVE.W", "(*-16,PC,D3.L), CCR",                    0042373, 0x38EE);
    TEST("MOVE.W", "#$34, CCR",                              0042374, 0x0034);

    // MOVE CCR,dst: 00413|M|R
    if (mc68k00()) {
        UNKN(0041302);
        UNKN(0041312);
        UNKN(0041332);
        UNKN(0041342);
        UNKN(0041352);
        UNKN(0041362);
        UNKN(0041370);
        UNKN(0041371);
    } else {
        TEST("MOVE.W", "CCR, D2",           0041302);
        UNKN(0041312);          // A2
        TEST("MOVE.W", "CCR, (A2)",         0041322);
        TEST("MOVE.W", "CCR, (A2)+",        0041332);
        TEST("MOVE.W", "CCR, -(A2)",        0041342);
        TEST("MOVE.W", "CCR, ($1234,A2)",   0041352, 0x1234);
        TEST("MOVE.W", "CCR, (18,A2,D3.W)", 0041362, 0x3012);
        ABST("MOVE.W", "CCR, (@A).W", "1234",   0041370, 0x1234);
        ABST("MOVE.W", "CCR, (@A).L", "234566", 0041371, 0x0023, 0x4566);
        UNKN(0041372);          // (disp,PC)
        UNKN(0041373);          // (disp,PC,Xn)
        UNKN(0041374);          // #imm
    }

    // ORI #nn,CCR
    TEST("ORI", "#$34, CCR", 0000074, 0x0034);
}

void test_multiproc() {
    // TAS dst: 00453|M|Rn
    TEST("TAS", "D2",           0045302);
    UNKN(0045312); // A2
    TEST("TAS", "(A2)",         0045322);
    TEST("TAS", "(A2)+",        0045332);
    TEST("TAS", "-(A2)",        0045342);
    TEST("TAS", "($1234,A2)",   0045352, 0x1234);
    TEST("TAS", "(18,A2,D3.W)", 0045362, 0x3012);
    ABST("TAS", "(@A).W", "1235",   0045370, 0x1235);
    ABST("TAS", "(@A).L", "123457", 0045371, 0x0012, 0x3457);
    UNKN(0045372); // (*+$1234,PC)
    UNKN(0045373); // (*+10,PC,D3.W)
    TEST("ILLEGAL", "",         0045374); // ILLEGAL

    if (!firstGen()) {
        // CAS Dc,Du,dst: 00|Sz|3|M|Rn, 000|Du|0|Dc, Sz:B=5/W=6/L=7
        UNKN(0005302); // Dc,Du,D2
        UNKN(0005312); // Dc,Du,A2
        TEST("CAS.B", "D2, D3, (A4)",             0005324, 0000302);
        TEST("CAS.B", "D3, D4, (A2)+",            0005332, 0000403);
        TEST("CAS.B", "D4, D5, -(A2)",            0005342, 0000504);
        TEST("CAS.B", "D5, D6, ($1234,A2)",       0005352, 0000605, 0x1234);
        TEST("CAS.B", "D6, D7, (18,A2,D3.W)",     0005362, 0000706, 0x3012);
        ABST("CAS.B", "D7, D0, (@A).W", "1234",   0005370, 0000007, 0x1234);
        ABST("CAS.B", "D0, D1, (@A).L", "123456", 0005371, 0000100, 0x0012, 0x3456);
        UNKN(0005372); // Dc,Du,(*+$1234,PC)
        UNKN(0005373); // Dc,Du,(*+10,PC,D3.W)
        UNKN(0005374); // Dc,Du,#nnnnnnnn
        UNKN(0006302); // Dc,Du,D2
        UNKN(0006312); // Dc,Du,A2
        TEST("CAS.W", "D2, D3, (A4)",             0006324, 0000302);
        TEST("CAS.W", "D3, D4, (A2)+",            0006332, 0000403);
        TEST("CAS.W", "D4, D5, -(A2)",            0006342, 0000504);
        TEST("CAS.W", "D5, D6, ($1234,A2)",       0006352, 0000605, 0x1234);
        TEST("CAS.W", "D6, D7, (18,A2,D3.W)",     0006362, 0000706, 0x3012);
        ABST("CAS.W", "D7, D0, (@A).W", "1234",   0006370, 0000007, 0x1234);
        ABST("CAS.W", "D0, D1, (@A).L", "123456", 0006371, 0000100, 0x0012, 0x3456);
        UNKN(0006372); // Dc,Du,(*+$1234,PC)
        UNKN(0006373); // Dc,Du,(*+10,PC,D3.W)
        UNKN(0007302); // Dc,Du,D2
        UNKN(0007312); // Dc,Du,A2
        TEST("CAS.L", "D2, D3, (A4)",             0007324, 0000302);
        TEST("CAS.L", "D3, D4, (A2)+",            0007332, 0000403);
        TEST("CAS.L", "D4, D5, -(A2)",            0007342, 0000504);
        TEST("CAS.L", "D5, D6, ($1234,A2)",       0007352, 0000605, 0x1234);
        TEST("CAS.L", "D6, D7, (18,A2,D3.W)",     0007362, 0000706, 0x3012);
        ABST("CAS.L", "D7, D0, (@A).W", "1234",   0007370, 0000007, 0x1234);
        ABST("CAS.L", "D0, D1, (@A).L", "123454", 0007371, 0000100, 0x0012, 0x3454);
        UNKN(0007372); // Dc,Du,(*+$1234,PC)
        UNKN(0007373); // Dc,Du,(*+10,PC,D3.W)

        // CAS2 Dc1:Dc2, Du1:Du2, (Rn1:Rn2), 00|Sz|3|M|Rn, R1|0|Du1|0|Dc1, R2|0|Du2|0|Dc2
        // Sz:W=6/L=7
        TEST("CAS2.W", "D1:D2, D3:D4, (D5):(D6)", 0006374, 0050301, 0060402);
        TEST("CAS2.W", "D1:D2, D3:D4, (A5):(D6)", 0006374, 0150301, 0060402);
        TEST("CAS2.W", "D1:D2, D3:D4, (D5):(A6)", 0006374, 0050301, 0160402);
        TEST("CAS2.W", "D1:D2, D3:D4, (A5):(A6)", 0006374, 0150301, 0160402);
        TEST("CAS2.L", "D1:D2, D3:D4, (D5):(D6)", 0007374, 0050301, 0060402);
        TEST("CAS2.L", "D1:D2, D3:D4, (A5):(D6)", 0007374, 0150301, 0060402);
        TEST("CAS2.L", "D1:D2, D3:D4, (D5):(A6)", 0007374, 0050301, 0160402);
        TEST("CAS2.L", "D1:D2, D3:D4, (A5):(A6)", 0007374, 0150301, 0160402);
    }
}

void test_addressing() {
    // MOVE.L src, D2: 00220|M|Rn
#define TIDX(opr, mr, ...) TEST("MOVE.L", opr ", D2", 0022000 | mr,  __VA_ARGS__)
#define EIDX(opr, mr, ...)                                               \
    ERRT("MOVE.L", opr ", D2", UNKNOWN_POSTBYTE, opr ", D2", 0022000 | mr, __VA_ARGS__)
#define IIDX(opr, mr, ...)                                               \
    ERRT("MOVE.L", opr ", D2", ILLEGAL_OPERAND_MODE, opr ", D2", 0022000 | mr, __VA_ARGS__)

    TIDX("D1",             001);
    TIDX("A2",             012);
    TIDX("(A3)",           023);
    TIDX("(A4)+",          034);
    TIDX("-(A5)",          045);
    TIDX("($1234,A6)",     056, 0x1234);
    TIDX("(*+$1236,PC)",   072, 0x1234);
    TIDX("#$00123456",     074, 0x0012, 0x3456);
    // Brief extension: a|Xn|Sz|disp8 a:Dn=0,An=1 Sz:W=0,L=4
    TIDX("($34,A7,D1.W)",  067, 0010000 | 0x34);
    TIDX("($34,A7,D1.L)",  067, 0014000 | 0x34);
    TIDX("($34,A7,A2.W)",  067, 0120000 | 0x34);
    TIDX("($34,A7,A2.L)",  067, 0124000 | 0x34);
    TIDX("(*+54,PC,D1.W)", 073, 0010000 | 0x34);
    TIDX("(*+54,PC,D1.L)", 073, 0014000 | 0x34);
    TIDX("(*+54,PC,A2.W)", 073, 0120000 | 0x34);
    TIDX("(*+54,PC,A2.L)", 073, 0124000 | 0x34);

    if (firstGen()) {
        // 24-bit absolute address
        TIDX("($001234).W",    070, 0x1234);
        TIDX("($FFFEDC).W",    070, 0xFEDC);
        TIDX("($345678).L",    071, 0x0034, 0x5678);

        TIDX("($23,A7,D1.W)",  067, 0010000 | 0x23);
        EIDX("($23,A7,D1.W)",  067, 0011000 | 0x23);
        EIDX("($23,A7,D1.W)",  067, 0012000 | 0x23);
        EIDX("($23,A7,D1.W)",  067, 0013000 | 0x23);
        TIDX("($23,A7,A1.L)",  067, 0114000 | 0x23);
        EIDX("($23,A7,A1.L)",  067, 0115000 | 0x23);
        EIDX("($23,A7,A1.L)",  067, 0116000 | 0x23);
        EIDX("($23,A7,A1.L)",  067, 0117000 | 0x23);
        TIDX("(*+54,PC,D1.W)", 073, 0010000 | 0x34);
        EIDX("(*+54,PC,D1.W)", 073, 0011000 | 0x34);
        EIDX("(*+54,PC,D1.W)", 073, 0012000 | 0x34);
        EIDX("(*+54,PC,D1.W)", 073, 0013000 | 0x34);
        TIDX("(*+54,PC,A1.L)", 073, 0114000 | 0x34);
        EIDX("(*+54,PC,A1.L)", 073, 0115000 | 0x34);
        EIDX("(*+54,PC,A1.L)", 073, 0116000 | 0x34);
        EIDX("(*+54,PC,A1.L)", 073, 0117000 | 0x34);

        // Full extension is not supported
        EIDX("(0,A0,D0.W)", 060, 0000400);

    } else {
        // 32-bit absolute address
        TIDX("($00001234).W",    070, 0x1234);
        TIDX("($FFFFFEDC).W",    070, 0xFEDC);
        TIDX("($12345678).L",    071, 0x1234, 0x5678);

        // Brief extension: a|Xn|SzSc|disp8 a:Dn=0,An=1 Sz:W=0,L=4 Sc=*2=1,*4=2,*8=3
        TIDX("($23,A7,D1.W)",    067, 0010000 | 0x23);
        TIDX("($23,A7,D1.W*2)",  067, 0011000 | 0x23);
        TIDX("($23,A7,D1.W*4)",  067, 0012000 | 0x23);
        TIDX("($23,A7,D1.W*8)",  067, 0013000 | 0x23);
        TIDX("($23,A7,A1.L)",    067, 0114000 | 0x23);
        TIDX("($23,A7,A1.L*2)",  067, 0115000 | 0x23);
        TIDX("($23,A7,A1.L*4)",  067, 0116000 | 0x23);
        TIDX("($23,A7,A1.L*8)",  067, 0117000 | 0x23);
        TIDX("(*+54,PC,D1.W)",   073, 0010000 | 0x34);
        TIDX("(*+54,PC,D1.W*2)", 073, 0011000 | 0x34);
        TIDX("(*+54,PC,D1.W*4)", 073, 0012000 | 0x34);
        TIDX("(*+54,PC,D1.W*8)", 073, 0013000 | 0x34);
        TIDX("(*+54,PC,A1.L)",   073, 0114000 | 0x34);
        TIDX("(*+54,PC,A1.L*2)", 073, 0115000 | 0x34);
        TIDX("(*+54,PC,A1.L*4)", 073, 0116000 | 0x34);
        TIDX("(*+54,PC,A1.L*8)", 073, 0117000 | 0x34);

        // Full extension: a|Xn|SzSc|Sp|Bd|I
        // Sp=1|BS|IS, base,index=4 base=5 index=6 no=7
        // Bd: null=2 W=4 L=6 (base displacement size)
        // IS,I
        //  0,0 (bd,An,Xn), (bd,Xn)
        //  0,1 ([bd,An,Xn])
        //  0,2 ([bd,An,Xn],od.W)
        //  0,3 ([bd,An,Xn],od.L)
        //  0,4 reserved
        //  0,5 ([bd,An],Xn)
        //  0,6 ([bd,An],Xn,od.W)
        //  0,7 ([bd,An],Xn,od.L)
        //  1,0 (bd,An), (bd)
        //  1,1 ([bd,An])
        //  1,2 ([bd,An],od.W)
        //  1,3 ([bd,An],od.L)
        //  1,4-1,7 reserved

        // Bd variants
        EIDX("(A7,D1.W)",           067, 0010400);                 // reserved
        EIDX("(A7,D1.W)",           067, 0010410);                 // reserved
        TIDX("(A7,D1.W)",           067, 0010420);                 // null base displacemend
        EIDX("(A7,D1.W)",           067, 0010430);                 // reserved
        TIDX("($1234,A7,D1.W)",     067, 0010440, 0x1234);         // 16-bit base displacement
        EIDX("($1234,A7,D1.W)",     067, 0010450, 0x1234);         // reserved
        TIDX("($12345678,A7,D1.W)", 067, 0010460, 0x1234, 0x5678); // 32-bit base displacement
        EIDX("($12345678,A7,D1.W)", 067, 0010470, 0x1234, 0x5678); // reserved

        // Null base displacement and base and index suppressed
        IIDX("()",             060, 0010720);
        IIDX("([])",           060, 0010721);
        IIDX("([],$1234)",     060, 0010722, 0x1234);
        IIDX("([],$12345678)", 060, 0010723, 0x1234, 0x5678);
        IIDX("([])",           060, 0010724);
        IIDX("([])",           060, 0010725);
        IIDX("([],$1234)",     060, 0010726, 0x1234);
        IIDX("([],$12345678)", 060, 0010727, 0x1234, 0x5678);
        // Null base displacement and base suppressed, post indexed
        TIDX("(D1.W)",              060, 0010620);
        TIDX("([D1.W])",            060, 0010621);
        TIDX("([D1.W],$1234)",      060, 0010622, 0x1234);
        TIDX("([D1.W],$12345678)",  060, 0010623, 0x1234, 0x5678);
        EIDX("([],D1.W)",           060, 0010624);
        IIDX("([],D1.W)",           060, 0010625);
        IIDX("([],D1.W,$1234)",     060, 0010626, 0x1234);
        IIDX("([],D1.W,$12345678)", 060, 0010627, 0x1234, 0x5678);

        // IS=0, I variants
        TIDX("(A7,D1.W)",             067, 0010420);                 // no indirect
        TIDX("([A7,D1.W])",           067, 0010421);                 // pre-indexed, null outer
        TIDX("([A7,D1.W],$1234)",     067, 0010422, 0x1234);         // pre-indexed, 16-bit outer
        TIDX("([A7,D1.W],$12345678)", 067, 0010423, 0x1234, 0x5678); // pre-indexed, 32-bit outer
        EIDX("([A7],D1.W)",           067, 0010424);                 // reserved
        TIDX("([A7],D1.W)",           067, 0010425);                 // post-indexed, null outer
        TIDX("([A7],D1.W,$1234)",     067, 0010426, 0x1234);         // post-indexed, 16-bit outer
        TIDX("([A7],D1.W,$12345678)", 067, 0010427, 0x1234, 0x5678); // post-indexed, 32-bit outer

        // IS=1, I variants
        TIDX("(A7)",             067, 0010520);                 // no indirect
        TIDX("([A7])",           067, 0010521);                 // null outer displacement
        TIDX("([A7],$1234)",     067, 0010522, 0x1234);         // 16-bit outer displacement
        TIDX("([A7],$12345678)", 067, 0010523, 0x1234, 0x5678); // 32-bit outer displacement
        EIDX("([A7])",           067, 0010524);                 // reserved
        EIDX("([A7])",           067, 0010525);                 // reserved
        EIDX("([A7],$1234)",     067, 0010526, 0x1234);         // reserved
        EIDX("([A7],$12345678)", 067, 0010527, 0x1234, 0x5678); // reserved

        // BS=0, IS=0
        TIDX("($1234,A7,D1.W)",             067, 0010440, 0x1234);
        TIDX("([$1234,A7,D1.W])",           067, 0010441, 0x1234);
        TIDX("([$1234,A7,D1.W],$5678)",     067, 0010442, 0x1234, 0x5678);
        TIDX("([$1234,A7,D1.W],$56789ABC)", 067, 0010443, 0x1234, 0x5678, 0x9ABC);
        TIDX("([$1234,A7],D1.W)",           067, 0010445, 0x1234);
        TIDX("([$1234,A7],D1.W,$5678)",     067, 0010446, 0x1234, 0x5678);
        TIDX("([$1234,A7],D1.W,$56789ABC)", 067, 0010447, 0x1234, 0x5678, 0x9ABC);

        TIDX("($12345678,A7,D1.W)",             067, 0010460, 0x1234, 0x5678);
        TIDX("([$12345678,A7,D1.W])",           067, 0010461, 0x1234, 0x5678);
        TIDX("([$12345678,A7,D1.W],$7654)",     067, 0010462, 0x1234, 0x5678, 0x7654);
        TIDX("([$12345678,A7,D1.W],$76543210)", 067, 0010463, 0x1234, 0x5678, 0x7654, 0x3210);
        TIDX("([$12345678,A7],D1.W)",           067, 0010465, 0x1234, 0x5678);
        TIDX("([$12345678,A7],D1.W,$7654)",     067, 0010466, 0x1234, 0x5678, 0x7654);
        TIDX("([$12345678,A7],D1.W,$76543210)", 067, 0010467, 0x1234, 0x5678, 0x7654, 0x3210);

        // BS=0, IS=1
        TIDX("(A7)",           067, 0010520);
        TIDX("($1235,A7)",     067, 0010540, 0x1235);
        TIDX("($12345678,A7)", 067, 0010560, 0x1234, 0x5678);
        EIDX("($12345678,A7)", 067, 0010570, 0x1234, 0x5678);

        TIDX("($1234,A7)",             067, 0010540, 0x1234);
        TIDX("([$1234,A7])",           067, 0010541, 0x1234);
        TIDX("([$1234,A7],$5678)",     067, 0010542, 0x1234, 0x5678);
        TIDX("([$1234,A7],$56789ABC)", 067, 0010543, 0x1234, 0x5678, 0x9ABC);

        TIDX("($12345678,A7)",             067, 0010560, 0x1234, 0x5678);
        TIDX("([$12345679,A7])",           067, 0010561, 0x1234, 0x5679);
        TIDX("([$12345678,A7],$7654)",     067, 0010562, 0x1234, 0x5678, 0x7654);
        TIDX("([$12345678,A7],$76543210)", 067, 0010563, 0x1234, 0x5678, 0x7654, 0x3210);

        // BS=1, IS=0
        TIDX("($12345678,D1.W)",             060, 0010660, 0x1234, 0x5678);
        TIDX("([$12345678,D1.W])",           060, 0010661, 0x1234, 0x5678);
        TIDX("([$12345678,D1.W],$7654)",     060, 0010662, 0x1234, 0x5678, 0x7654);
        TIDX("([$12345678,D1.W],$76543210)", 060, 0010663, 0x1234, 0x5678, 0x7654, 0x3210);
        TIDX("([$12345678],D1.W)",           060, 0010665, 0x1234, 0x5678);
        TIDX("([$12345678],D1.W,$7654)",     060, 0010666, 0x1234, 0x5678, 0x7654);
        TIDX("([$12345678],D1.W,$76543210)", 060, 0010667, 0x1234, 0x5678, 0x7654, 0x3210);

        TIDX("($12345678,D1.L)",             060, 0014660, 0x1234, 0x5678);
        TIDX("([$12345678,D1.L])",           060, 0014661, 0x1234, 0x5678);
        TIDX("([$12345678,D1.L],$7654)",     060, 0014662, 0x1234, 0x5678, 0x7654);
        TIDX("([$12345678,D1.L],$76543210)", 060, 0014663, 0x1234, 0x5678, 0x7654, 0x3210);
        TIDX("([$12345678],D1.L)",           060, 0014665, 0x1234, 0x5678);
        TIDX("([$12345678],D1.L,$7654)",     060, 0014666, 0x1234, 0x5678, 0x7654);
        TIDX("([$12345678],D1.L,$76543210)", 060, 0014667, 0x1234, 0x5678, 0x7654, 0x3210);

        TIDX("($12345678,A1.W)",             060, 0110660, 0x1234, 0x5678);
        TIDX("([$12345678,A1.W])",           060, 0110661, 0x1234, 0x5678);
        TIDX("([$12345678,A1.W],$7654)",     060, 0110662, 0x1234, 0x5678, 0x7654);
        TIDX("([$12345678,A1.W],$76543210)", 060, 0110663, 0x1234, 0x5678, 0x7654, 0x3210);
        TIDX("([$12345678],A1.W)",           060, 0110665, 0x1234, 0x5678);
        TIDX("([$12345678],A1.W,$7654)",     060, 0110666, 0x1234, 0x5678, 0x7654);
        TIDX("([$12345678],A1.W,$76543210)", 060, 0110667, 0x1234, 0x5678, 0x7654, 0x3210);

        TIDX("($12345678,A1.L)",             060, 0114660, 0x1234, 0x5678);
        TIDX("([$12345678,A1.L])",           060, 0114661, 0x1234, 0x5678);
        TIDX("([$12345678,A1.L],$7654)",     060, 0114662, 0x1234, 0x5678, 0x7654);
        TIDX("([$12345678,A1.L],$76543210)", 060, 0114663, 0x1234, 0x5678, 0x7654, 0x3210);
        TIDX("([$12345678],A1.L)",           060, 0114665, 0x1234, 0x5678);
        TIDX("([$12345678],A1.L,$7654)",     060, 0114666, 0x1234, 0x5678, 0x7654);
        TIDX("([$12345678],A1.L,$76543210)", 060, 0114667, 0x1234, 0x5678, 0x7654, 0x3210);

        // BS=1, IS=1
        TIDX("($12345678)",             060, 0010760, 0x1234, 0x5678);
        TIDX("([$12345678])",           060, 0010761, 0x1234, 0x5678);
        TIDX("([$12345678],$7654)",     060, 0010762, 0x1234, 0x5678, 0x7654);
        TIDX("([$12345678],$76543210)", 060, 0010763, 0x1234, 0x5678, 0x7654, 0x3210);

        EIDX("(*+2,PC,D1.W)",         073, 0010400);
        EIDX("(*+2,PC,D1.W)",         073, 0010410);
        TIDX("(*+2,PC,D1.W)",         073, 0010420);
        EIDX("(*+2,PC,D1.W)",         073, 0010430);
        TIDX("(*+$1236,PC,D1.W)",     073, 0010440, 0x1234);
        EIDX("(*+$1236,PC,D1.W)",     073, 0010450, 0x1234);
        TIDX("(*+$1234567A,PC,D1.W)", 073, 0010460, 0x1234, 0x5678);
        EIDX("(*+$1234567A,PC,D1.W)", 073, 0010470, 0x1234, 0x5678);

        TIDX("(*+2,PC,D1.W)",             073, 0010420);
        TIDX("([*+2,PC,D1.W])",           073, 0010421);
        TIDX("([*+2,PC,D1.W],$1234)",     073, 0010422, 0x1234);
        TIDX("([*+2,PC,D1.W],$12345678)", 073, 0010423, 0x1234, 0x5678);
        EIDX("([*+2,PC],D1.W)",           073, 0010424);
        TIDX("([*+2,PC],D1.W)",           073, 0010425);
        TIDX("([*+2,PC],D1.W,$1234)",     073, 0010426, 0x1234);
        TIDX("([*+2,PC],D1.W,$12345678)", 073, 0010427, 0x1234, 0x5678);

        TIDX("(*+$1236,PC,D1.W)",             073, 0010440, 0x1234);
        TIDX("([*+$1236,PC,D1.W])",           073, 0010441, 0x1234);
        TIDX("([*+$1236,PC,D1.W],$5678)",     073, 0010442, 0x1234, 0x5678);
        TIDX("([*+$1236,PC,D1.W],$56789ABC)", 073, 0010443, 0x1234, 0x5678, 0x9ABC);
        TIDX("([*+$1236,PC],D1.W)",           073, 0010445, 0x1234);
        TIDX("([*+$1236,PC],D1.W,$5678)",     073, 0010446, 0x1234, 0x5678);
        TIDX("([*+$1236,PC],D1.W,$56789ABC)", 073, 0010447, 0x1234, 0x5678, 0x9ABC);

        TIDX("(*+$1234567A,PC,D1.W)",             073, 0010460, 0x1234, 0x5678);
        TIDX("([*+$1234567B,PC,D1.W])",           073, 0010461, 0x1234, 0x5679);
        TIDX("([*+$1234567A,PC,D1.W],$7654)",     073, 0010462, 0x1234, 0x5678, 0x7654);
        TIDX("([*+$1234567A,PC,D1.W],$76543210)", 073, 0010463, 0x1234, 0x5678, 0x7654, 0x3210);
        TIDX("([*+$1234567A,PC],D1.W)",           073, 0010465, 0x1234, 0x5678);
        TIDX("([*+$1234567A,PC],D1.W,$7654)",     073, 0010466, 0x1234, 0x5678, 0x7654);
        TIDX("([*+$1234567A,PC],D1.W,$76543210)", 073, 0010467, 0x1234, 0x5678, 0x7654, 0x3210);

        TIDX("(*+2,PC)",         073, 0010520);
        TIDX("(*+$1236,PC)",     073, 0010540, 0x1234);
        TIDX("(*+$1234567A,PC)", 073, 0010560, 0x1234, 0x5678);
        EIDX("(*+$1234567A,PC)", 073, 0010570, 0x1234, 0x5678);

        TIDX("(*+2,PC)",             073, 0010520);
        TIDX("([*+2,PC])",           073, 0010521);
        TIDX("([*+2,PC],$1234)",     073, 0010522, 0x1234);
        TIDX("([*+2,PC],$12345678)", 073, 0010523, 0x1234, 0x5678);
        EIDX("([*+2,PC])",           073, 0010524);
        EIDX("([*+2,PC])",           073, 0010525);
        EIDX("([*+2,PC],$1234)",     073, 0010526, 0x1234);
        EIDX("([*+2,PC],$12345678)", 073, 0010527, 0x1234, 0x5678);

        TIDX("(*+$1236,PC)",             073, 0010540, 0x1234);
        TIDX("([*+$1236,PC])",           073, 0010541, 0x1234);
        TIDX("([*+$1236,PC],$5678)",     073, 0010542, 0x1234, 0x5678);
        TIDX("([*+$1236,PC],$56789ABC)", 073, 0010543, 0x1234, 0x5678, 0x9ABC);

        TIDX("(*+$1234567A,PC)",             073, 0010560, 0x1234, 0x5678);
        TIDX("([*+$1234567A,PC])",           073, 0010561, 0x1234, 0x5678);
        TIDX("([*+$1234567A,PC],$7654)",     073, 0010562, 0x1234, 0x5678, 0x7654);
        TIDX("([*+$1234567A,PC],$76543210)", 073, 0010563, 0x1234, 0x5678, 0x7654, 0x3210);

        TIDX("(*+$1234567A,D1.W)",             073, 0010660, 0x1234, 0x5678);
        TIDX("([*+$1234567B,D1.W])",           073, 0010661, 0x1234, 0x5679);
        TIDX("([*+$1234567A,D1.W],$7654)",     073, 0010662, 0x1234, 0x5678, 0x7654);
        TIDX("([*+$1234567A,D1.W],$76543210)", 073, 0010663, 0x1234, 0x5678, 0x7654, 0x3210);
        TIDX("([*+$1234567A],D1.W)",           073, 0010665, 0x1234, 0x5678);
        TIDX("([*+$1234567A],D1.W,$7654)",     073, 0010666, 0x1234, 0x5678, 0x7654);
        TIDX("([*+$1234567A],D1.W,$76543210)", 073, 0010667, 0x1234, 0x5678, 0x7654, 0x3210);

        TIDX("(*+$1234567A)",             073, 0010760, 0x1234, 0x5678);
        TIDX("([*+$1234567A])",           073, 0010761, 0x1234, 0x5678);
        TIDX("([*+$1234567A],$7654)",     073, 0010762, 0x1234, 0x5678, 0x7654);
        TIDX("([*+$1234567A],$76543210)", 073, 0010763, 0x1234, 0x5678, 0x7654, 0x3210);

        // BS=1, IS=1
        TEST("MOVE.B", "($1235), D2",            0012000|060, 0010740, 0x1235);
        ERRT("MOVE.W", "($1235), D2",
             OPERAND_NOT_ALIGNED, "($1235), D2", 0032000|060, 0010740, 0x1235);
        ERRT("MOVE.L", "($1235), D2",
             OPERAND_NOT_ALIGNED, "($1235), D2", 0022000|060, 0010740, 0x1235);
        ERRT("MOVE.B", "([$1235]), D2",
             OPERAND_NOT_ALIGNED, "([$1235]), D2", 0012000|060, 0010741, 0x1235);
        ERRT("MOVE.W", "([$1235]), D2",
             OPERAND_NOT_ALIGNED, "([$1235]), D2", 0032000|060, 0010741, 0x1235);
        ERRT("MOVE.L", "([$1235]), D2",
             OPERAND_NOT_ALIGNED, "([$1235]), D2", 0022000|060, 0010741, 0x1235);

        // BASE=PC, IS=1
        TEST("MOVE.B", "(*+$1235,PC), D2",            0012000|073, 0010540, 0x1233);
        ERRT("MOVE.W", "(*+$1235,PC), D2",
             OPERAND_NOT_ALIGNED, "(*+$1235,PC), D2", 0032000|073, 0010540, 0x1233);
        ERRT("MOVE.L", "(*+$1235,PC), D2",
             OPERAND_NOT_ALIGNED, "(*+$1235,PC), D2", 0022000|073, 0010540, 0x1233);
        ERRT("MOVE.B", "([*+$1235,PC]), D2",
             OPERAND_NOT_ALIGNED, "([*+$1235,PC]), D2", 0012000|073, 0010541, 0x1233);
        ERRT("MOVE.W", "([*+$1235,PC]), D2",
             OPERAND_NOT_ALIGNED, "([*+$1235,PC]), D2", 0032000|073, 0010541, 0x1233);
        ERRT("MOVE.L", "([*+$1235,PC]), D2",
             OPERAND_NOT_ALIGNED, "([*+$1235,PC]), D2", 0022000|073, 0010541, 0x1233);
    }
}

void test_bitfield(){
    // BFTST src{o:w}:   01643|M|Rn, |0|oooooo|0|wwwwww|
    // BFTST src(Do:Dw}: 01643|M|Rn, |1|000ooo|1|000www|
    TEST("BFTST", "D1{31:32}",   0164301, 0003700);
    UNKN(0164312);              // A2
    TEST("BFTST", "(A2){1:2}",   0164322, 0000102);
    TEST("BFTST", "(A2){D1:3}",  0164322, 0004103);
    TEST("BFTST", "(A2){1:D3}",  0164322, 0000143);
    TEST("BFTST", "(A2){D1:D3}", 0164322, 0004143);
    UNKN(0164332);              // (A2)+
    UNKN(0164342);              // -(A2)
    TEST("BFTST", "($1234,A2){1:2}",       0164352, 0000102, 0x1234);
    TEST("BFTST", "(18,A2,D3.W){1:2}",     0164362, 0000102, 0x3012);
    ABST("BFTST", "(@A).W{1:2}", "1234",   0164370, 0000102, 0x1234);
    ABST("BFTST", "(@A).L{1:2}", "012346", 0164371, 0000102, 0x0001, 0x2346);
    TEST("BFTST", "(*+$1234,PC){1:2}",     0164372, 0000102, 0x1230);
    TEST("BFTST", "(*+35,PC,D3.L){1:2}",   0164373, 0000102, 0x381F);
    UNKN(0164374);              // #nn

    // BFEXTU src{o:w},Dn:   01647|M|Rn, 0|Dn|0|oooooo|0|wwwwww|
    // BFEXTU src(Do:Dw},Dn: 01647|M|Rn, 0|Dn|1|000ooo|1|000www|
    TEST("BFEXTU", "D1{1:2}, D4",     0164701, 0040102);
    UNKN(0164712);              // A2
    TEST("BFEXTU", "(A2){D1:3}, D4",  0164722, 0044103);
    UNKN(0164732);              // (A2)+
    UNKN(0164742);              // -(A2)
    TEST("BFEXTU", "($1234,A2){1:2}, D4",       0164752, 0040102, 0x1234);
    TEST("BFEXTU", "(18,A2,D3.W){1:2}, D4",     0164762, 0040102, 0x3012);
    ABST("BFEXTU", "(@A).W{1:2}, D4", "1234",   0164770, 0040102, 0x1234);
    ABST("BFEXTU", "(@A).L{1:2}, D4", "012346", 0164771, 0040102, 0x0001, 0x2346);
    TEST("BFEXTU", "(*+$1234,PC){1:2}, D4",     0164772, 0040102, 0x1230);
    TEST("BFEXTU", "(*+35,PC,D3.L){1:2}, D4",   0164773, 0040102, 0x381F);
    UNKN(0164774);              // #nn

    // BFCHG src{o:w}: 01653|M|Rn, |0|oooooo|0|wwwwww|
    TEST("BFCHG", "D1{1:2}",     0165301, 00102);
    UNKN(0165312);              // A2
    TEST("BFCHG", "(A2){1:D3}",  0165322, 00143);
    UNKN(0165332);              // (A2)+
    UNKN(0165342);              // -(A2)
    TEST("BFCHG", "($1234,A2){1:2}",       0165352, 00102, 0x1234);
    TEST("BFCHG", "(18,A2,D3.W){1:2}",     0165362, 00102, 0x3012);
    ABST("BFCHG", "(@A).W{1:2}", "1234",   0165370, 00102, 0x1234);
    ABST("BFCHG", "(@A).L{1:2}", "012346", 0165371, 00102, 0x0001, 0x2346);
    UNKN(0165372);              // (d16,PC)
    UNKN(0165373);              // (d8,PC,Xn)
    UNKN(0165374);              // #nn

    // BFEXTS src{o:w},Dn:   01657|M|Rn, 0|Dn|0|oooooo|0|wwwwww|
    TEST("BFEXTS", "D1{1:2}, D4",     0165701, 0040102);
    UNKN(0165712);              // A2
    TEST("BFEXTS", "(A2){D1:D3}, D4", 0165722, 0044143);
    UNKN(0165732);              // (A2)+
    UNKN(0165742);              // -(A2)
    TEST("BFEXTS", "($1234,A2){1:2}, D4",       0165752, 0040102, 0x1234);
    TEST("BFEXTS", "(18,A2,D3.W){1:2}, D4",     0165762, 0040102, 0x3012);
    ABST("BFEXTS", "(@A).W{1:2}, D4", "1234",   0165770, 0040102, 0x1234);
    ABST("BFEXTS", "(@A).L{1:2}, D4", "012346", 0165771, 0040102, 0x0001, 0x2346);
    TEST("BFEXTS", "(*+$1234,PC){1:2}, D4",     0165772, 0040102, 0x1230);
    TEST("BFEXTS", "(*+35,PC,D3.L){1:2}, D4",   0165773, 0040102, 0x381F);
    UNKN(0165774);              // #nn

    // BFCLR src{o:w}: 01663|M|Rn, |0|oooooo|0|wwwwww|
    TEST("BFCLR", "D1{1:2}",     0166301, 00102);
    UNKN(0166312);              // A2
    TEST("BFCLR", "(A2){1:2}",   0166322, 00102);
    UNKN(0166332);              // (A2)+
    UNKN(0166342);              // -(A2)
    TEST("BFCLR", "($1234,A2){1:2}",       0166352, 00102, 0x1234);
    TEST("BFCLR", "(18,A2,D3.W){1:2}",     0166362, 00102, 0x3012);
    ABST("BFCLR", "(@A).W{1:2}", "1234",   0166370, 00102, 0x1234);
    ABST("BFCLR", "(@A).L{1:2}", "012346", 0166371, 00102, 0x0001, 0x2346);
    UNKN(0166372);              // (d16,PC)
    UNKN(0166373);              // (d8,PC,Xn)
    UNKN(0166374);              // #nn

    // BFFFO src{o:w},Dn: 01667|M|Rn, 0|Dn|0|oooooo|0|wwwwww|
    TEST("BFFFO", "D1{1:2}, D4",     0166701, 0040102);
    UNKN(0166712);              // A2
    TEST("BFFFO", "(A2){D1:D3}, D4", 0166722, 0044143);
    UNKN(0166732);              // (A2)+
    UNKN(0166742);              // -(A2)
    TEST("BFFFO", "($1234,A2){1:2}, D4",       0166752, 0040102, 0x1234);
    TEST("BFFFO", "(18,A2,D3.W){1:2}, D4",     0166762, 0040102, 0x3012);
    ABST("BFFFO", "(@A).W{1:2}, D4", "1234",   0166770, 0040102, 0x1234);
    ABST("BFFFO", "(@A).L{1:2}, D4", "012346", 0166771, 0040102, 0x0001, 0x2346);
    TEST("BFFFO", "(*+$1234,PC){1:2}, D4",     0166772, 0040102, 0x1230);
    TEST("BFFFO", "(*+35,PC,D3.L){1:2}, D4",   0166773, 0040102, 0x381F);
    UNKN(0166774);              // #nn

    // BFSET src{o:w}: 01673|M|Rn, |0|oooooo|0|wwwwww|
    TEST("BFSET", "D1{0:32}",    0167301, 00000);
    UNKN(0167312);              // (A2)
    TEST("BFSET", "(A2){31:31}", 0167322, 03737);
    UNKN(0167332);              // (A2)+
    UNKN(0167342);              // -(A2)
    TEST("BFSET", "($1234,A2){1:2}",       0167352, 00102, 0x1234);
    TEST("BFSET", "(18,A2,D3.W){1:2}",     0167362, 00102, 0x3012);
    ABST("BFSET", "(@A).W{1:2}", "1234",   0167370, 00102, 0x1234);
    ABST("BFSET", "(@A).L{1:2}", "012346", 0167371, 00102, 0x0001, 0x2346);
    UNKN(0167372);              // (d16,PC)
    UNKN(0167373);              // (d8,PC,Xn)
    UNKN(0167374);              // #nn

    // BFINS Dn,dst{o:w}: 01677|M|Rn, 0|Dn|0|oooooo|0|wwwwww|
    TEST("BFINS", "D4, D1{1:2}",     0167701, 0040102);
    UNKN(0167712);              // A2
    TEST("BFINS", "D4, (A2){D1:D3}", 0167722, 0044143);
    UNKN(0167732);              // (A2)+
    UNKN(0167742);              // -(A2)
    TEST("BFINS", "D4, ($1234,A2){1:2}",       0167752, 0040102, 0x1234);
    TEST("BFINS", "D4, (18,A2,D3.W){1:2}",     0167762, 0040102, 0x3012);
    ABST("BFINS", "D4, (@A).W{1:2}", "1234",   0167770, 0040102, 0x1234);
    ABST("BFINS", "D4, (@A).L{1:2}", "012346", 0167771, 0040102, 0x0001, 0x2346);
    UNKN(0167772);              // (d16,PC)
    UNKN(0167773);              // (d8,PC,Xn)
    UNKN(0167774);              // #nn
}

#if !defined(LIBASM_MC68000_NOFPU)

void test_float_move() {
    TEST("FMOVE.X", "FP0, FP1",            0xF200,     0x0000|(0<<10)|(1<<7));
    TEST("FMOVE.X", "FP2, FP2",            0xF200,     0x0000|(2<<10)|(2<<7));

    TEST("FMOVE.L", "D2, FP3",             0xF200|002, 0x4000|(0<<10)|(3<<7));
    TEST("FMOVE.S", "D2, FP3",             0xF200|002, 0x4000|(1<<10)|(3<<7));
    ERRT("FMOVE.X", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                           0xF200|002, 0x4000|(2<<10)|(3<<7));
    ERRT("FMOVE.P", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                           0xF200|002, 0x4000|(3<<10)|(3<<7));
    TEST("FMOVE.W", "D2, FP3",             0xF200|002, 0x4000|(4<<10)|(3<<7));
    ERRT("FMOVE.D", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                           0xF200|002, 0x4000|(5<<10)|(3<<7));
    TEST("FMOVE.B", "D2, FP3",             0xF200|002, 0x4000|(6<<10)|(3<<7));
    UNKN(/* FMOVE.P A4{D3}, FP5 */         0xF200|002, 0x4000|(7<<10)|(5<<7));
    UNKN(/* FMOVE.L A4, FP5 */             0xF200|014, 0x4000|(0<<10)|(5<<7));
    UNKN(/* FMOVE.S A4, FP5 */             0xF200|014, 0x4000|(1<<10)|(5<<7));
    TEST("FMOVE.X", "(A6), FP7",           0xF200|026, 0x4000|(2<<10)|(7<<7));
    TEST("FMOVE.P", "(A0)+, FP1",          0xF200|030, 0x4000|(3<<10)|(1<<7));
    TEST("FMOVE.W", "-(A2), FP3",          0xF200|042, 0x4000|(4<<10)|(3<<7));
    TEST("FMOVE.D", "($1234,A4), FP5",     0xF200|054, 0x4000|(5<<10)|(5<<7), 0x1234);
    TEST("FMOVE.B", "($23,A6,D7.W), FP0",  0xF200|066, 0x4000|(6<<10)|(0<<7), 0x7023);
    ABST("FMOVE.L", "(@A).W, FP1", "4566",   0xF200|070, 0x4000|(0<<10)|(1<<7), 0x4566);
    ABST("FMOVE.S", "(@A).L, FP2", "56789A", 0xF200|071, 0x4000|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FMOVE.X", "(*+$1234,PC), FP3",   0xF200|072, 0x4000|(2<<10)|(3<<7), 0x1230);
    TEST("FMOVE.P", "(*+90,PC,A4.L), FP5", 0xF200|073, 0x4000|(3<<10)|(5<<7), 0xC856);
    TEST("FMOVE.L", "#$6789ABCD, FP6",     0xF200|074, 0x4000|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FMOVE.W", "#$1234, FP2",         0xF200|074, 0x4000|(4<<10)|(2<<7), 0x1234);
    TEST("FMOVE.B", "#$23, FP4",           0xF200|074, 0x4000|(6<<10)|(4<<7), 0x0023);
    TFLT("FMOVE.S", "#7.88999976E-10, FP7", "#$3058E0F0, FP7",
         0xF200|074, 0x4000|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    TFLT("FMOVE.X", "#-89000000032, FP0",  "#$C0230000A5C681D100000000, FP0",
         0xF200|074, 0x4000|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    TFLT("FMOVE.P", "#0.0912, FP1",        "#$400200091200000000000000, FP1",
         0xF200|074, 0x4000|(3<<10)|(1<<7), 0x4002, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    TFLT("FMOVE.D", "#-8.25, FP3",         "#$C020800000000000, FP3",
         0xF200|074, 0x4000|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FMOVE.L", "FP3, D2",             0xF200|002, 0x6000|(0<<10)|(3<<7));
    TEST("FMOVE.S", "FP3, D2",             0xF200|002, 0x6000|(1<<10)|(3<<7));
    ERRT("FMOVE.X", "FP3, D2", ILLEGAL_SIZE, "D2",
                                        0xF200|002, 0x6000|(2<<10)|(3<<7));
    ERRT("FMOVE.P", "FP3, D2", ILLEGAL_SIZE, "D2",
                                        0xF200|002, 0x6000|(3<<10)|(3<<7));
    TEST("FMOVE.W", "FP3, D2",             0xF200|002, 0x6000|(4<<10)|(3<<7));
    ERRT("FMOVE.D", "FP3, D2", ILLEGAL_SIZE, "D2",
                                        0xF200|002, 0x6000|(5<<10)|(3<<7));
    TEST("FMOVE.B", "FP3, D2",             0xF200|002, 0x6000|(6<<10)|(3<<7));
    UNKN(/* FMOVE.L FP5, A4 */             0xF200|014, 0x4000|(0<<10)|(5<<7));
    UNKN(/* FMOVE.S FP5, A4 */             0xF200|014, 0x4000|(1<<10)|(5<<7));
    TEST("FMOVE.X", "FP7, (A6)",           0xF200|026, 0x6000|(2<<10)|(7<<7));
    TEST("FMOVE.P", "FP1, (A0)+{#63}",     0xF200|030, 0x6000|(3<<10)|(1<<7)|0x3F);
    TEST("FMOVE.P", "FP1, (A0)+{#-64}",    0xF200|030, 0x6000|(3<<10)|(1<<7)|0x40);
    TEST("FMOVE.W", "FP3, -(A2)",          0xF200|042, 0x6000|(4<<10)|(3<<7));
    TEST("FMOVE.D", "FP5, ($1234,A4)",     0xF200|054, 0x6000|(5<<10)|(5<<7), 0x1234);
    TEST("FMOVE.B", "FP0, ($23,A6,D7.W)",  0xF200|066, 0x6000|(6<<10)|(0<<7), 0x7023);
    TEST("FMOVE.P", "FP1, (A0)+{D5}",      0xF200|030, 0x6000|(7<<10)|(1<<7)|(5<<4));
    ABST("FMOVE.L", "FP1, (@A).W", "4566",   0xF200|070, 0x6000|(0<<10)|(1<<7), 0x4566);
    ABST("FMOVE.S", "FP2, (@A).L", "56789A", 0xF200|071, 0x6000|(1<<10)|(2<<7), 0x0056, 0x789A);
    UNKN(/* FMOVE.X FP3, (*+$1234,PC) */   0xF200|072, 0x6000|(2<<10)|(3<<7));
    UNKN(/* FMOVE.P FP5, (*+90,PC,A4.L) */ 0xF200|073, 0x6000|(3<<10)|(5<<7));
    UNKN(/* FMOVE.L FP6, #$6789ABCD */     0xF200|074, 0x6000|(0<<10)|(6<<7));
    UNKN(/* FMOVE.S FP7, #7.88999976E-10 */0xF200|074, 0x6000|(1<<10)|(7<<7));
    UNKN(/* FMOVE.X FP0, #-89000000032 */  0xF200|074, 0x6000|(2<<10)|(0<<7));
    UNKN(/* FMOVE.P FP1, #9.12E20 */       0xF200|074, 0x6000|(3<<10)|(1<<7));
    UNKN(/* FMOVE.W FP2, #$1234 */         0xF200|074, 0x6000|(4<<10)|(2<<7));
    UNKN(/* FMOVE.D FP3, #-8.25 */         0xF200|074, 0x6000|(5<<10)|(3<<7));
    UNKN(/* FMOVE.B FP4, #$23 */           0xF200|074, 0x6000|(6<<10)|(4<<7));
    UNKN(/* FMOVE.P FP5, #$23 */           0xF200|074, 0x6000|(7<<10)|(5<<7));

    UNKN(/* FMOVEM.X Dn, list */            0xF200|000, 0xD000|0x01);
    UNKN(/* FMOVEM.X An, list */            0xF200|010, 0xD000|0x02);
    TEST("FMOVEM.X", "(A6), FP0/FP4-FP7",   0xF200|026, 0xD000|0x8F);
    TEST("FMOVEM.X", "(A0)+, FP0/FP4-FP7",  0xF200|030, 0xD000|0x8F);
    UNKN(/* FMOVEM.X -(An), list */         0xF200|042, 0xD000|0x10);
    TEST("FMOVEM.X", "($1234,A4), FP3-FP5", 0xF200|054, 0xD000|0x1C, 0x1234);
    TEST("FMOVEM.X", "($23,A6,D7.W), FP3",  0xF200|066, 0xD000|0x10, 0x7023);
    ABST("FMOVEM.X", "(@A).W, FP4", "4566",   0xF200|070, 0xD000|0x08, 0x4566);
    ABST("FMOVEM.X", "(@A).L, FP5", "56789A", 0xF200|071, 0xD000|0x04, 0x0056, 0x789A);
    TEST("FMOVEM.X", "(*+$1234,PC), FP6",   0xF200|072, 0xD000|0x02, 0x1230);
    TEST("FMOVEM.X", "(*+90,PC,A4.L), FP7", 0xF200|073, 0xD000|0x01, 0xC856);
    UNKN(/* FMOVEM.X #nn, list */           0xF200|074, 0xD000);
    ERRT("FMOVEM.X", "(A6), ",     OPCODE_HAS_NO_EFFECT, "",           0xF200|026, 0xD000);
    ERRT("FMOVEM.X", "(A6), FP7",  ILLEGAL_OPERAND_MODE, "(A6), FP7",  0xF200|026, 0xC000|0x01);
    ERRT("FMOVEM.X", "(A6)+, FP7", ILLEGAL_OPERAND_MODE, "(A6)+, FP7", 0xF200|036, 0xC000|0x01);

    UNKN(/* FMOVEM.X Dn, Dn */             0xF200|000, 0xD800);
    UNKN(/* FMOVEM.X An, Dn */             0xF200|010, 0xD800);
    TEST("FMOVEM.X", "(A6), D1",           0xF200|026, 0xD800|(1<<4));
    TEST("FMOVEM.X", "(A0)+, D2",          0xF200|030, 0xD800|(2<<4));
    UNKN(/* FMOVEM.X -(An), Dn */          0xF200|042, 0xD800);
    TEST("FMOVEM.X", "($1234,A4), D3",     0xF200|054, 0xD800|(3<<4), 0x1234);
    TEST("FMOVEM.X", "($23,A6,D7.W), D4",  0xF200|066, 0xD800|(4<<4), 0x7023);
    ABST("FMOVEM.X", "(@A).W, D5", "4566",   0xF200|070, 0xD800|(5<<4), 0x4566);
    ABST("FMOVEM.X", "(@A).L, D6", "56789A", 0xF200|071, 0xD800|(6<<4), 0x0056, 0x789A);
    TEST("FMOVEM.X", "(*+$1234,PC), D7",   0xF200|072, 0xD800|(7<<4), 0x1230);
    TEST("FMOVEM.X", "(*+90,PC,A4.L), D0", 0xF200|073, 0xD800|(0<<0), 0xC856);
    UNKN(/* FMOVEM.X #nn, Dn */            0xF200|074, 0xD800);
    ERRT("FMOVEM.X", "(A6), D1",  ILLEGAL_OPERAND_MODE, "(A6), D1",  0xF200|026, 0xC800|(1<<4));
    ERRT("FMOVEM.X", "(A6)+, D2", ILLEGAL_OPERAND_MODE, "(A6)+, D2", 0xF200|036, 0xC800|(2<<4));

    UNKN(/* FMOVEM.X list, Dn */            0xF200|007, 0xF000|0x80);
    UNKN(/* FMOVEM.X list, An */            0xF200|014, 0xF000|0x40);
    TEST("FMOVEM.X", "FP0/FP4-FP7, (A6)",   0xF200|026, 0xF000|0x8F);
    UNKN(/* FMOVEM.X list, (An)+ */         0xF200|030, 0xF000|0x10);
    TEST("FMOVEM.X", "FP0/FP4-FP7, -(A2)",  0xF200|042, 0xE000|0xF1);
    TEST("FMOVEM.X", "FP3-FP5, ($1234,A4)", 0xF200|054, 0xF000|0x1C, 0x1234);
    TEST("FMOVEM.X", "FP2, ($23,A6,D7.W)",  0xF200|066, 0xF000|0x20, 0x7023);
    ABST("FMOVEM.X", "FP1, (@A).W", "4566",   0xF200|070, 0xF000|0x40, 0x4566);
    ABST("FMOVEM.X", "FP0, (@A).L", "56789A", 0xF200|071, 0xF000|0x80, 0x0056, 0x789A);
    UNKN(/* FMOVEM.X list, (nn,PC) */       0xF200|072, 0xF000);
    UNKN(/* FMOVEM.X list, (nn,PC,Xn) */    0xF200|073, 0xF000);
    UNKN(/* FMOVEM.X list, #nn */           0xF200|074, 0xF000);
    ERRT("FMOVEM.X", ", (A6)",    OPCODE_HAS_NO_EFFECT, ", (A6)", 0xF200|026, 0xF000);
    ERRT("FMOVEM.X", "FP0, (A6)", ILLEGAL_OPERAND_MODE, "(A6)",   0xF200|026, 0xE000|0x01);
    ERRT("FMOVEM.X", "FP7, -(A2)", ILLEGAL_OPERAND_MODE, "-(A2)", 0xF200|042, 0xF000|0x01);

    UNKN(/* FMOVEM.X Dn, Dn */            0xF200|007, 0xF800);
    UNKN(/* FMOVEM.X Dn, An */            0xF200|014, 0xF800);
    TEST("FMOVEM.X", "D1, (A6)",          0xF200|026, 0xF800|(1<<4));
    UNKN(/* FMOVEM.X Dn, (An)+ */         0xF200|030, 0xF800);
    TEST("FMOVEM.X", "D2, -(A2)",         0xF200|042, 0xE800|(2<<4));
    TEST("FMOVEM.X", "D3, ($1234,A4)",    0xF200|054, 0xF800|(3<<4), 0x1234);
    TEST("FMOVEM.X", "D4, ($23,A6,D7.W)", 0xF200|066, 0xF800|(4<<4), 0x7023);
    ABST("FMOVEM.X", "D5, (@A).W", "4566",   0xF200|070, 0xF800|(5<<4), 0x4566);
    ABST("FMOVEM.X", "D6, (@A).L", "56789A", 0xF200|071, 0xF800|(6<<4), 0x0056, 0x789A);
    UNKN(/* FMOVEM.X Dn, (nn,PC) */      0xF200|072, 0xF800);
    UNKN(/* FMOVEM.X Dn, (nn,PC,Xn) */   0xF200|073, 0xF800);
    UNKN(/* FMOVEM.X Dn, #nn */          0xF200|074, 0xF800);
    ERRT("FMOVEM.X", "D7, (A6)", ILLEGAL_OPERAND_MODE, "(A6)",   0xF200|026, 0xE800|(7<<4));
    ERRT("FMOVEM.X", "D0, -(A2)", ILLEGAL_OPERAND_MODE, "-(A2)", 0xF200|042, 0xF800|(0<<4));

    TEST("FMOVE.L", "D0, FPCR",               0xF200|000, 0x9000);
    TEST("FMOVE.L", "D1, FPSR",               0xF200|001, 0x8800);
    TEST("FMOVE.L", "D2, FPIAR",              0xF200|002, 0x8400);
    ERRT("FMOVE.L", "A3, FPCR",  ILLEGAL_OPERAND_MODE, "A3, FPCR",  0xF200|013, 0x9000);
    ERRT("FMOVE.L", "A4, FPSR",  ILLEGAL_OPERAND_MODE, "A4, FPSR",  0xF200|014, 0x8800);
    TEST("FMOVE.L", "A5, FPIAR",              0xF200|015, 0x8400);
    TEST("FMOVE.L", "(A6), FPCR",             0xF200|026, 0x9000);
    TEST("FMOVE.L", "(A7), FPSR",             0xF200|027, 0x8800);
    TEST("FMOVE.L", "(A0), FPIAR",            0xF200|020, 0x8400);
    TEST("FMOVE.L", "(A1)+, FPCR",            0xF200|031, 0x9000);
    TEST("FMOVE.L", "(A2)+, FPSR",            0xF200|032, 0x8800);
    TEST("FMOVE.L", "(A3)+, FPIAR",           0xF200|033, 0x8400);
    TEST("FMOVE.L", "-(A4), FPCR",            0xF200|044, 0x9000);
    TEST("FMOVE.L", "-(A5), FPSR",            0xF200|045, 0x8800);
    TEST("FMOVE.L", "-(A6), FPIAR",           0xF200|046, 0x8400);
    TEST("FMOVE.L", "($1234,A7), FPCR",       0xF200|057, 0x9000, 0x1234);
    TEST("FMOVE.L", "($2345,A0), FPSR",       0xF200|050, 0x8800, 0x2345);
    TEST("FMOVE.L", "($3456,A1), FPIAR",      0xF200|051, 0x8400, 0x3456);
    TEST("FMOVE.L", "($45,A2,D1.W), FPCR",    0xF200|062, 0x9000, 0x1045);
    TEST("FMOVE.L", "($56,A3,D2.L), FPSR",    0xF200|063, 0x8800, 0x2856);
    TEST("FMOVE.L", "($67,A4,A3.W), FPIAR",   0xF200|064, 0x8400, 0xB067);
    ABST("FMOVE.L", "(@A).W, FPCR", "789A",    0xF200|070, 0x9000, 0x789A);
    ABST("FMOVE.L", "(@A).W, FPSR", "89AA",    0xF200|070, 0x8800, 0x89AA);
    ABST("FMOVE.L", "(@A).W, FPIAR", "9ABC",   0xF200|070, 0x8400, 0x9ABC);
    ABST("FMOVE.L", "(@A).L, FPCR", "ABCDEE",  0xF200|071, 0x9000, 0x00AB, 0xCDEE);
    ABST("FMOVE.L", "(@A).L, FPSR", "BCDEF0",  0xF200|071, 0x8800, 0x00BC, 0xDEF0);
    ABST("FMOVE.L", "(@A).L, FPIAR", "CDEF00", 0xF200|071, 0x8400, 0x00CD, 0xEF00);
    TEST("FMOVE.L", "(*+$1234,PC), FPCR",     0xF200|072, 0x9000, 0x1230);
    TEST("FMOVE.L", "(*+$2344,PC), FPSR",     0xF200|072, 0x8800, 0x2340);
    TEST("FMOVE.L", "(*+$3456,PC), FPIAR",    0xF200|072, 0x8400, 0x3452);
    TEST("FMOVE.L", "(*+73,PC,D1.W), FPCR",   0xF200|073, 0x9000, 0x1045);
    TEST("FMOVE.L", "(*+90,PC,D2.L), FPSR",   0xF200|073, 0x8800, 0x2856);
    TEST("FMOVE.L", "(*+107,PC,A3.W), FPIAR", 0xF200|073, 0x8400, 0xB067);
    TEST("FMOVE.L", "#$12345678, FPCR",       0xF200|074, 0x9000, 0x1234, 0x5678);
    TEST("FMOVE.L", "#$23456789, FPSR",       0xF200|074, 0x8800, 0x2345, 0x6789);
    TEST("FMOVE.L", "#$3456789A, FPIAR",      0xF200|074, 0x8400, 0x3456, 0x789A);

    TEST("FMOVE.L", "FPCR, D0",             0xF200|000, 0xB000);
    TEST("FMOVE.L", "FPSR, D1",             0xF200|001, 0xA800);
    TEST("FMOVE.L", "FPIAR, D2",            0xF200|002, 0xA400);
    ERRT("FMOVE.L", "FPCR, A3",  ILLEGAL_OPERAND_MODE, "A3", 0xF200|013, 0xB000);
    ERRT("FMOVE.L", "FPSR, A4",  ILLEGAL_OPERAND_MODE, "A4", 0xF200|014, 0xA800);
    TEST("FMOVE.L", "FPIAR, A5",            0xF200|015, 0xA400);
    TEST("FMOVE.L", "FPCR, (A6)",           0xF200|026, 0xB000);
    TEST("FMOVE.L", "FPSR, (A7)",           0xF200|027, 0xA800);
    TEST("FMOVE.L", "FPIAR, (A0)",          0xF200|020, 0xA400);
    TEST("FMOVE.L", "FPCR, (A1)+",          0xF200|031, 0xB000);
    TEST("FMOVE.L", "FPSR, (A2)+",          0xF200|032, 0xA800);
    TEST("FMOVE.L", "FPIAR, (A3)+",         0xF200|033, 0xA400);
    TEST("FMOVE.L", "FPCR, -(A4)",          0xF200|044, 0xB000);
    TEST("FMOVE.L", "FPSR, -(A5)",          0xF200|045, 0xA800);
    TEST("FMOVE.L", "FPIAR, -(A6)",         0xF200|046, 0xA400);
    TEST("FMOVE.L", "FPCR, ($1234,A7)",     0xF200|057, 0xB000, 0x1234);
    TEST("FMOVE.L", "FPSR, ($2345,A0)",     0xF200|050, 0xA800, 0x2345);
    TEST("FMOVE.L", "FPIAR, ($3456,A1)",    0xF200|051, 0xA400, 0x3456);
    TEST("FMOVE.L", "FPCR, ($45,A2,D1.W)",  0xF200|062, 0xB000, 0x1045);
    TEST("FMOVE.L", "FPSR, ($56,A3,D2.L)",  0xF200|063, 0xA800, 0x2856);
    TEST("FMOVE.L", "FPIAR, ($67,A4,A3.W)", 0xF200|064, 0xA400, 0xB067);
    ABST("FMOVE.L", "FPCR, (@A).W", "789A",    0xF200|070, 0xB000, 0x789A);
    ABST("FMOVE.L", "FPSR, (@A).W", "89AA",    0xF200|070, 0xA800, 0x89AA);
    ABST("FMOVE.L", "FPIAR, (@A).W", "9ABC",   0xF200|070, 0xA400, 0x9ABC);
    ABST("FMOVE.L", "FPCR, (@A).L", "ABCDEE",  0xF200|071, 0xB000, 0x00AB, 0xCDEE);
    ABST("FMOVE.L", "FPSR, (@A).L", "BCDEF0",  0xF200|071, 0xA800, 0x00BC, 0xDEF0);
    ABST("FMOVE.L", "FPIAR, (@A).L", "CDEF00", 0xF200|071, 0xA400, 0x00CD, 0xEF00);
    UNKN(/* FMOVE.L FPCR,  (nn,PC) */       0xF200|072, 0xB000);
    UNKN(/* FMOVE.L FPSR,  (nn,PC) */       0xF200|072, 0xA800);
    UNKN(/* FMOVE.L FPIAR, (nn,PC) */       0xF200|072, 0xA400);
    UNKN(/* FMOVE.L FPCR,  (nn,PC,Xn) */    0xF200|073, 0xB000);
    UNKN(/* FMOVE.L FPSR,  (nn,PC,Xn) */    0xF200|073, 0xA800);
    UNKN(/* FMOVE.L FPIAR, (nn,PC,Xn) */    0xF200|073, 0xA400);
    UNKN(/* FMOVE.L FPCR,  #nn */           0xF200|074, 0xB000);
    UNKN(/* FMOVE.L FPSR,  #nn */           0xF200|074, 0xA800);
    UNKN(/* FMOVE.L FPIAR, #nn */           0xF200|074, 0xA400);

    ERRT("FMOVEM.L", "FPCR/FPSR, D1",  ILLEGAL_OPERAND_MODE, "D1", 0xF200|001, 0xB800);
    ERRT("FMOVEM.L", "FPSR/FPIAR, A2", ILLEGAL_OPERAND_MODE, "A2", 0xF200|012, 0xAC00);
    TEST("FMOVEM.L", "FPCR/FPIAR, (A3)",             0xF200|023, 0xB400);
    TEST("FMOVEM.L", "FPCR/FPSR/FPIAR, (A4)+",       0xF200|034, 0xBC00);
    TEST("FMOVEM.L", "FPCR/FPSR, -(A5)",             0xF200|045, 0xB800);
    TEST("FMOVEM.L", "FPSR/FPIAR, ($1234,A6)",       0xF200|056, 0xAC00, 0x1234);
    TEST("FMOVEM.L", "FPCR/FPIAR, ($34,A7,D2.W)",    0xF200|067, 0xB400, 0x2034);
    ABST("FMOVEM.L", "FPCR/FPSR/FPIAR, (@A).W", "5678", 0xF200|070, 0xBC00, 0x5678);
    ABST("FMOVEM.L", "FPCR/FPSR, (@A).L", "6789AA",     0xF200|071, 0xB800, 0x0067, 0x89AA);
    UNKN(/* FMOVEM.L FPSR/FPIAR, (nn,PC) */          0xF200|072, 0xAC00);
    UNKN(/* FMOVEM.L FPCR/FPIAR, (nn,PC,Xn) */       0xF200|073, 0xB400);
    UNKN(/* FMOVEM.L FPCR/FPSR/FPIAR, #nn */         0xF200|074, 0xBC00);

    ERRT("FMOVEM.L", "D1, FPCR/FPSR",  ILLEGAL_OPERAND_MODE, "D1, FPCR/FPSR",  0xF200|001, 0x9800);
    ERRT("FMOVEM.L", "A2, FPSR/FPIAR", ILLEGAL_OPERAND_MODE, "A2, FPSR/FPIAR", 0xF200|012, 0x8C00);
    TEST("FMOVEM.L", "(A3), FPCR/FPIAR",             0xF200|023, 0x9400);
    TEST("FMOVEM.L", "(A4)+, FPCR/FPSR/FPIAR",       0xF200|034, 0x9C00);
    TEST("FMOVEM.L", "-(A5), FPCR/FPSR",             0xF200|045, 0x9800);
    TEST("FMOVEM.L", "($1234,A6), FPSR/FPIAR",       0xF200|056, 0x8C00, 0x1234);
    TEST("FMOVEM.L", "($34,A7,D2.W), FPCR/FPIAR",    0xF200|067, 0x9400, 0x2034);
    ABST("FMOVEM.L", "(@A).W, FPCR/FPSR/FPIAR", "5678", 0xF200|070, 0x9C00, 0x5678);
    ABST("FMOVEM.L", "(@A).L, FPCR/FPSR", "6789AA",     0xF200|071, 0x9800, 0x0067, 0x89AA);
    TEST("FMOVEM.L", "(*+$1234,PC), FPSR/FPIAR",     0xF200|072, 0x8C00, 0x1230);
    TEST("FMOVEM.L", "(*+73,PC,D1.W), FPCR/FPIAR",   0xF200|073, 0x9400, 0x1045);
    ERRT("FMOVEM.L", "#$12345678, FPCR/FPSR/FPIAR", ILLEGAL_OPERAND_MODE,
         "#$12345678, FPCR/FPSR/FPIAR", 0xF200|074, 0x9C00, 0x1234, 0x5678);

    TEST("FMOVECR.X", "#$00, FP0", 0xF200, 0x5C00|(0<<7)|0x00);
    TEST("FMOVECR.X", "#$10, FP2", 0xF200, 0x5C00|(2<<7)|0x10);
    TEST("FMOVECR.X", "#$20, FP4", 0xF200, 0x5C00|(4<<7)|0x20);
    TEST("FMOVECR.X", "#$7F, FP7", 0xF200, 0x5C00|(7<<7)|0x7F);

    TFLT("FMOVE.X", "#INF, FP0",           "#$7FFF00008000000000000000, FP0",
         0xF200|074, 0x4000|(2<<10)|(0<<7), 0x7FFF, 0x0000, 0x8000, 0x0000, 0x0000, 0x0000);
    TFLT("FMOVE.X", "#-INF, FP0",          "#$FFFF00008000000000000000, FP0",
         0xF200|074, 0x4000|(2<<10)|(0<<7), 0xFFFF, 0x0000, 0x8000, 0x0000, 0x0000, 0x0000);
    TFLT("FMOVE.X", "#NAN, FP0",           "#$7FFF0000C000000000000000, FP0",
         0xF200|074, 0x4000|(2<<10)|(0<<7), 0x7FFF, 0x0000, 0xC000, 0x0000, 0x0000, 0x0000);
    TFLT("FMOVE.X", "#NAN, FP0",           "#$7FFF0000C000000000000000, FP0",
         0xF200|074, 0x4000|(2<<10)|(0<<7), 0x7FFF, 0x0000, 0xC000, 0x0000, 0x0000, 0x0000);
    TFLT("FMOVE.X", "#-NAN, FP0",          "#$FFFF10008000000000000001, FP0",
         0xF200|074, 0x4000|(2<<10)|(0<<7), 0xFFFF, 0x1000, 0x8000, 0x0000, 0x0000, 0x0001);
    TFLT("FMOVE.X", "#-INF, FP0",          "#$FFFF00018000000000000000, FP0",
         0xF200|074, 0x4000|(2<<10)|(0<<7), 0xFFFF, 0x0001, 0x8000, 0x0000, 0x0000, 0x0000);
    TFLT("FMOVE.X", "#-89000000032, FP0",  "#$C0230001A5C681D100000000, FP0",
         0xF200|074, 0x4000|(2<<10)|(0<<7), 0xC023, 0x0001, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    EFLT("FMOVE.X", "#-NAN, FP0", ILLEGAL_CONSTANT,
         "-NAN, FP0",                      "#$C023000025C681D100000000, FP0",
         0xF200|074, 0x4000|(2<<10)|(0<<7), 0xC023, 0x0000, 0x25C6, 0x81D1, 0x0000, 0x0000);
    EFLT("FMOVE.X", "#-NAN, FP0", ILLEGAL_CONSTANT,
         "-NAN, FP0",                      "#$80000000A5C681D100000000, FP0",
         0xF200|074, 0x4000|(2<<10)|(0<<7), 0x8000, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);

    TFLT("FMOVE.P", "#INF, FP1",           "#$7FFF00000000000000000000, FP1",
         0xF200|074, 0x4000|(3<<10)|(1<<7), 0x7FFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000);
    TFLT("FMOVE.P", "#-INF, FP1",          "#$FFFF00000000000000000000, FP1",
         0xF200|074, 0x4000|(3<<10)|(1<<7), 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000);
    TFLT("FMOVE.P", "#NAN, FP1",           "#$7FFF00008000000000000000, FP1",
         0xF200|074, 0x4000|(3<<10)|(1<<7), 0x7FFF, 0x0000, 0x8000, 0x0000, 0x0000, 0x0000);
    TFLT("FMOVE.P", "#-NAN, FP1",          "#$FFFF00000000000000000001, FP1",
         0xF200|074, 0x4000|(3<<10)|(1<<7), 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0001);
    EFLT("FMOVE.P", "#9.12E+20, FP1", ILLEGAL_CONSTANT,
         "9.12E+20, FP1",                  "#$002010091200000000000000, FP1",
         0xF200|074, 0x4000|(3<<10)|(1<<7), 0x0020, 0x1009, 0x1200, 0x0000, 0x0000, 0x0000);
    EFLT("FMOVE.P", "#9.12E+20, FP1", ILLEGAL_CONSTANT,
         "9.12E+20, FP1",                  "#$002002091200000000000000, FP1",
         0xF200|074, 0x4000|(3<<10)|(1<<7), 0x0020, 0x0209, 0x1200, 0x0000, 0x0000, 0x0000);
    EFLT("FMOVE.P", "#9.12E+20, FP1", ILLEGAL_CONSTANT,
         "9.12E+20, FP1",                  "#$002000391200000000000000, FP1",
         0xF200|074, 0x4000|(3<<10)|(1<<7), 0x0020, 0x0039, 0x1200, 0x0000, 0x0000, 0x0000);
    EFLT("FMOVE.P", "#9.12E+20, FP1", ILLEGAL_CONSTANT,
         "9.12E+20, FP1",                  "#$001A00091200000000000000, FP1",
         0xF200|074, 0x4000|(3<<10)|(1<<7), 0x001A, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    EFLT("FMOVE.P", "#9.12E+20, FP1", ILLEGAL_CONSTANT,
         "9.12E+20, FP1",                  "#$00200008B200000000000000, FP1",
         0xF200|074, 0x4000|(3<<10)|(1<<7), 0x0020, 0x0008, 0xB200, 0x0000, 0x0000, 0x0000);
    EFLT("FMOVE.P", "#1.012E+20, FP1", ILLEGAL_CONSTANT,
         "1.012E+20, FP1",                 "#$0019000A1200000000000000, FP1",
         0xF200|074, 0x4000|(3<<10)|(1<<7), 0x0019, 0x000A, 0x1200, 0x0000, 0x0000, 0x0000);

    EQUALS("gnu-as", OK, disassembler.setOption("gnu-as", "on"));
    EQUALS("upper-hex", OK, disassembler.setOption("upper-hex", "off"));
    EQUALS("upper-case", OK, disassembler.setOption("upper-case", "off"));

    TEST("fmovel", "#0x6789abcd, fp6",
         0xF200|074, 0x4000|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("fmoves", "#0x3058e0f0, fp7",
         0xF200|074, 0x4000|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    TEST("fmovex", "#0xffff00008000000000000000, fp0",
         0xF200|074, 0x4000|(2<<10)|(0<<7), 0xFFFF, 0x0000, 0x8000, 0x0000, 0x0000, 0x0000);
    TEST("fmovex", "#0xc0230000a5c681d100000000, fp0",
         0xF200|074, 0x4000|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    TEST("fmovep", "#0xffff00000000000000000001, fp1",
         0xF200|074, 0x4000|(3<<10)|(1<<7), 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0001);
    TEST("fmovep", "#0x002000091200000000000000, fp1",
         0xF200|074, 0x4000|(3<<10)|(1<<7), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    TEST("fmovew", "#0x1234, fp2",
         0xF200|074, 0x4000|(4<<10)|(2<<7), 0x1234);
    TEST("fmoved", "#0xc020800000000000, fp3",
         0xF200|074, 0x4000|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);
    TEST("fmoveb", "#0x23, fp4",
         0xF200|074, 0x4000|(6<<10)|(4<<7), 0x0023);
}

void test_float_arithmetic() {
    TEST("FINT.X", "FP0, FP1",            0xF200,     0x0001|(0<<10)|(1<<7));
    TEST("FINT.X", "FP2",                 0xF200,     0x0001|(2<<10)|(2<<7));
    TEST("FINT.L", "D2, FP3",             0xF200|002, 0x4001|(0<<10)|(3<<7));
    TEST("FINT.S", "D2, FP3",             0xF200|002, 0x4001|(1<<10)|(3<<7));
    ERRT("FINT.X", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                          0xF200|002, 0x4001|(2<<10)|(3<<7));
    ERRT("FINT.P", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                          0xF200|002, 0x4001|(3<<10)|(3<<7));
    TEST("FINT.W", "D2, FP3",             0xF200|002, 0x4001|(4<<10)|(3<<7));
    ERRT("FINT.D", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                          0xF200|002, 0x4001|(5<<10)|(3<<7));
    TEST("FINT.B", "D2, FP3",             0xF200|002, 0x4001|(6<<10)|(3<<7));
    UNKN(/* FINT.P D2, FP3 */             0xF200|002, 0x4001|(7<<10)|(3<<7));
    UNKN(/* FINT.S A4, FP5 */             0xF200|014, 0x4001|(1<<10)|(5<<7));
    TEST("FINT.X", "(A6), FP7",           0xF200|026, 0x4001|(2<<10)|(7<<7));
    TEST("FINT.P", "(A0)+, FP1",          0xF200|030, 0x4001|(3<<10)|(1<<7));
    TEST("FINT.W", "-(A2), FP3",          0xF200|042, 0x4001|(4<<10)|(3<<7));
    TEST("FINT.D", "($1234,A4), FP5",     0xF200|054, 0x4001|(5<<10)|(5<<7), 0x1234);
    TEST("FINT.B", "($23,A6,D7.W), FP0",  0xF200|066, 0x4001|(6<<10)|(0<<7), 0x7023);
    ABST("FINT.L", "(@A).W, FP1", "4566",   0xF200|070, 0x4001|(0<<10)|(1<<7), 0x4566);
    ABST("FINT.S", "(@A).L, FP2", "56789A", 0xF200|071, 0x4001|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FINT.X", "(*+$1234,PC), FP3",   0xF200|072, 0x4001|(2<<10)|(3<<7), 0x1230);
    TEST("FINT.P", "(*+90,PC,A4.L), FP5", 0xF200|073, 0x4001|(3<<10)|(5<<7), 0xC856);
    TEST("FINT.L", "#$6789ABCD, FP6",     0xF200|074, 0x4001|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FINT.W", "#$1234, FP2",         0xF200|074, 0x4001|(4<<10)|(2<<7), 0x1234);
    TEST("FINT.B", "#$23, FP4",           0xF200|074, 0x4001|(6<<10)|(4<<7), 0x0023);
    TFLT("FINT.S", "#7.88999976E-10, FP7", "#$3058E0F0, FP7",
         0xF200|074, 0x4001|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    TFLT("FINT.X", "#-89000000032, FP0",   "#$C0230000A5C681D100000000, FP0",
         0xF200|074, 0x4001|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    TFLT("FINT.P", "#912, FP1",            "#$000200091200000000000000, FP1",
         0xF200|074, 0x4001|(3<<10)|(1<<7), 0x0002, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    TFLT("FINT.D", "#-8.25, FP3",          "#$C020800000000000, FP3",
         0xF200|074, 0x4001|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    UNKN(/* FSINH.P D2, FP3 */             0xF200|002, 0x4002|(7<<10)|(3<<7));
    UNKN(/* FSINH.S A4, FP5 */             0xF200|014, 0x4002|(1<<10)|(5<<7));
    TEST("FSINH.X", "(A6), FP7",           0xF200|026, 0x4002|(2<<10)|(7<<7));
    TEST("FSINH.P", "(A0)+, FP1",          0xF200|030, 0x4002|(3<<10)|(1<<7));
    TEST("FSINH.W", "-(A2), FP3",          0xF200|042, 0x4002|(4<<10)|(3<<7));
    TEST("FSINH.D", "($1234,A4), FP5",     0xF200|054, 0x4002|(5<<10)|(5<<7), 0x1234);
    TEST("FSINH.B", "($23,A6,D7.W), FP0",  0xF200|066, 0x4002|(6<<10)|(0<<7), 0x7023);
    ABST("FSINH.L", "(@A).W, FP1", "4566",   0xF200|070, 0x4002|(0<<10)|(1<<7), 0x4566);
    ABST("FSINH.S", "(@A).L, FP2", "56789A", 0xF200|071, 0x4002|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FSINH.X", "(*+$1234,PC), FP3",   0xF200|072, 0x4002|(2<<10)|(3<<7), 0x1230);
    TEST("FSINH.P", "(*+90,PC,A4.L), FP5", 0xF200|073, 0x4002|(3<<10)|(5<<7), 0xC856);
    TEST("FSINH.L", "#$6789ABCD, FP6",     0xF200|074, 0x4002|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TFLT("FSINH.S", "#7.88999976E-10, FP7", "#$3058E0F0, FP7",
         0xF200|074, 0x4002|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    TFLT("FSINH.X", "#-89000000032, FP0",  "#$C0230000A5C681D100000000, FP0",
         0xF200|074, 0x4002|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    TFLT("FSINH.P", "#-9.12E+20, FP1",     "#$802000091200000000000000, FP1",
         0xF200|074, 0x4002|(3<<10)|(1<<7), 0x8020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    TEST("FSINH.W", "#$1234, FP2",
         0xF200|074, 0x4002|(4<<10)|(2<<7), 0x1234);
    TFLT("FSINH.D", "#-8.25, FP3",         "#$C020800000000000, FP3",
         0xF200|074, 0x4002|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);
    TEST("FSINH.B", "#$23, FP4",
         0xF200|074, 0x4002|(6<<10)|(4<<7), 0x0023);

    TEST("FINTRZ.X", "FP0, FP1",            0xF200,     0x0003|(0<<10)|(1<<7));
    TEST("FINTRZ.X", "FP2",                 0xF200,     0x0003|(2<<10)|(2<<7));
    TEST("FINTRZ.L", "D2, FP3",             0xF200|002, 0x4003|(0<<10)|(3<<7));
    TEST("FINTRZ.S", "D2, FP3",             0xF200|002, 0x4003|(1<<10)|(3<<7));
    ERRT("FINTRZ.X", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                            0xF200|002, 0x4003|(2<<10)|(3<<7));
    ERRT("FINTRZ.P", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                            0xF200|002, 0x4003|(3<<10)|(3<<7));
    TEST("FINTRZ.W", "D2, FP3",             0xF200|002, 0x4003|(4<<10)|(3<<7));
    ERRT("FINTRZ.D", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                            0xF200|002, 0x4003|(5<<10)|(3<<7));
    TEST("FINTRZ.B", "D2, FP3",             0xF200|002, 0x4003|(6<<10)|(3<<7));
    UNKN(/* FINTRZ.P D2, FP3 */             0xF200|002, 0x4003|(7<<10)|(3<<7));
    UNKN(/* FINTRZ.S A4, FP5 */             0xF200|014, 0x4003|(1<<10)|(5<<7));
    TEST("FINTRZ.X", "(A6), FP7",           0xF200|026, 0x4003|(2<<10)|(7<<7));
    TEST("FINTRZ.P", "(A0)+, FP1",          0xF200|030, 0x4003|(3<<10)|(1<<7));
    TEST("FINTRZ.W", "-(A2), FP3",          0xF200|042, 0x4003|(4<<10)|(3<<7));
    TEST("FINTRZ.D", "($1234,A4), FP5",     0xF200|054, 0x4003|(5<<10)|(5<<7), 0x1234);
    TEST("FINTRZ.B", "($23,A6,D7.W), FP0",  0xF200|066, 0x4003|(6<<10)|(0<<7), 0x7023);
    ABST("FINTRZ.L", "(@A).W, FP1", "4566",   0xF200|070, 0x4003|(0<<10)|(1<<7), 0x4566);
    ABST("FINTRZ.S", "(@A).L, FP2", "56789A", 0xF200|071, 0x4003|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FINTRZ.X", "(*+$1234,PC), FP3",   0xF200|072, 0x4003|(2<<10)|(3<<7), 0x1230);
    TEST("FINTRZ.P", "(*+90,PC,A4.L), FP5", 0xF200|073, 0x4003|(3<<10)|(5<<7), 0xC856);
    TEST("FINTRZ.L", "#$6789ABCD, FP6",     0xF200|074, 0x4003|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FINTRZ.W", "#$1234, FP2",         0xF200|074, 0x4003|(4<<10)|(2<<7), 0x1234);
    TEST("FINTRZ.B", "#$23, FP4",           0xF200|074, 0x4003|(6<<10)|(4<<7), 0x0023);
    TFLT("FINTRZ.S", "#7.88999976E-10, FP7", "#$3058E0F0, FP7",
         0xF200|074, 0x4003|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    TFLT("FINTRZ.X", "#-89000000032, FP0", "#$C0230000A5C681D100000000, FP0",
         0xF200|074, 0x4003|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    TFLT("FINTRZ.P", "#9.12E-20, FP1",     "#$402000091200000000000000, FP1",
         0xF200|074, 0x4003|(3<<10)|(1<<7), 0x4020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    TFLT("FINTRZ.D", "#-8.25, FP3",        "#$C020800000000000, FP3",
         0xF200|074, 0x4003|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FSQRT.X", "FP0, FP1",            0xF200,     0x0004|(0<<10)|(1<<7));
    TEST("FSQRT.X", "FP2",                 0xF200,     0x0004|(2<<10)|(2<<7));
    TEST("FSQRT.L", "D2, FP3",             0xF200|002, 0x4004|(0<<10)|(3<<7));
    TEST("FSQRT.S", "D2, FP3",             0xF200|002, 0x4004|(1<<10)|(3<<7));
    ERRT("FSQRT.X", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                           0xF200|002, 0x4004|(2<<10)|(3<<7));
    ERRT("FSQRT.P", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                           0xF200|002, 0x4004|(3<<10)|(3<<7));
    TEST("FSQRT.W", "D2, FP3",             0xF200|002, 0x4004|(4<<10)|(3<<7));
    ERRT("FSQRT.D", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                           0xF200|002, 0x4004|(5<<10)|(3<<7));
    TEST("FSQRT.B", "D2, FP3",             0xF200|002, 0x4004|(6<<10)|(3<<7));
    UNKN(/* FSQRT.P D2, FP3 */             0xF200|002, 0x4004|(7<<10)|(3<<7));
    UNKN(/* FSQRT.S A4, FP5 */             0xF200|014, 0x4004|(1<<10)|(5<<7));
    TEST("FSQRT.X", "(A6), FP7",           0xF200|026, 0x4004|(2<<10)|(7<<7));
    TEST("FSQRT.P", "(A0)+, FP1",          0xF200|030, 0x4004|(3<<10)|(1<<7));
    TEST("FSQRT.W", "-(A2), FP3",          0xF200|042, 0x4004|(4<<10)|(3<<7));
    TEST("FSQRT.D", "($1234,A4), FP5",     0xF200|054, 0x4004|(5<<10)|(5<<7), 0x1234);
    TEST("FSQRT.B", "($23,A6,D7.W), FP0",  0xF200|066, 0x4004|(6<<10)|(0<<7), 0x7023);
    ABST("FSQRT.L", "(@A).W, FP1", "4566",   0xF200|070, 0x4004|(0<<10)|(1<<7), 0x4566);
    ABST("FSQRT.S", "(@A).L, FP2", "56789A", 0xF200|071, 0x4004|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FSQRT.X", "(*+$1234,PC), FP3",   0xF200|072, 0x4004|(2<<10)|(3<<7), 0x1230);
    TEST("FSQRT.P", "(*+90,PC,A4.L), FP5", 0xF200|073, 0x4004|(3<<10)|(5<<7), 0xC856);
    TEST("FSQRT.L", "#$6789ABCD, FP6",     0xF200|074, 0x4004|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FSQRT.W", "#$1234, FP2",         0xF200|074, 0x4004|(4<<10)|(2<<7), 0x1234);
    TEST("FSQRT.B", "#$23, FP4",           0xF200|074, 0x4004|(6<<10)|(4<<7), 0x0023);
    TFLT("FSQRT.S", "#7.88999976E-10, FP7", "#$3058E0F0, FP7",
         0xF200|074, 0x4004|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    TFLT("FSQRT.X", "#-89000000032, FP0",  "#$C0230000A5C681D100000000, FP0",
         0xF200|074, 0x4004|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    TFLT("FSQRT.P", "#9.1234567891234567, FP1", "#$000000091234567891234567, FP1",
         0xF200|074, 0x4004|(3<<10)|(1<<7), 0x0000, 0x0009, 0x1234, 0x5678, 0x9123, 0x4567);
    TFLT("FSQRT.D", "#-8.25, FP3",         "#$C020800000000000, FP3",
         0xF200|074, 0x4004|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FLOGNP1.X", "FP0, FP1",            0xF200,     0x0006|(0<<10)|(1<<7));
    TEST("FLOGNP1.X", "FP2",                 0xF200,     0x0006|(2<<10)|(2<<7));
    TEST("FLOGNP1.L", "D2, FP3",             0xF200|002, 0x4006|(0<<10)|(3<<7));
    TEST("FLOGNP1.S", "D2, FP3",             0xF200|002, 0x4006|(1<<10)|(3<<7));
    ERRT("FLOGNP1.X", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                           0xF200|002, 0x4006|(2<<10)|(3<<7));
    ERRT("FLOGNP1.P", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                           0xF200|002, 0x4006|(3<<10)|(3<<7));
    TEST("FLOGNP1.W", "D2, FP3",             0xF200|002, 0x4006|(4<<10)|(3<<7));
    ERRT("FLOGNP1.D", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                           0xF200|002, 0x4006|(5<<10)|(3<<7));
    TEST("FLOGNP1.B", "D2, FP3",             0xF200|002, 0x4006|(6<<10)|(3<<7));
    UNKN(/* FLOGNP1.P D2, FP3 */             0xF200|002, 0x4006|(7<<10)|(3<<7));
    UNKN(/* FLOGNP1.S A4, FP5 */             0xF200|014, 0x4006|(1<<10)|(5<<7));
    TEST("FLOGNP1.X", "(A6), FP7",           0xF200|026, 0x4006|(2<<10)|(7<<7));
    TEST("FLOGNP1.P", "(A0)+, FP1",          0xF200|030, 0x4006|(3<<10)|(1<<7));
    TEST("FLOGNP1.W", "-(A2), FP3",          0xF200|042, 0x4006|(4<<10)|(3<<7));
    TEST("FLOGNP1.D", "($1234,A4), FP5",     0xF200|054, 0x4006|(5<<10)|(5<<7), 0x1234);
    TEST("FLOGNP1.B", "($23,A6,D7.W), FP0",  0xF200|066, 0x4006|(6<<10)|(0<<7), 0x7023);
    ABST("FLOGNP1.L", "(@A).W, FP1", "4566",   0xF200|070, 0x4006|(0<<10)|(1<<7), 0x4566);
    ABST("FLOGNP1.S", "(@A).L, FP2", "56789A", 0xF200|071, 0x4006|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FLOGNP1.X", "(*+$1234,PC), FP3",   0xF200|072, 0x4006|(2<<10)|(3<<7), 0x1230);
    TEST("FLOGNP1.P", "(*+90,PC,A4.L), FP5", 0xF200|073, 0x4006|(3<<10)|(5<<7), 0xC856);
    TEST("FLOGNP1.L", "#$6789ABCD, FP6",     0xF200|074, 0x4006|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FLOGNP1.W", "#$1234, FP2",         0xF200|074, 0x4006|(4<<10)|(2<<7), 0x1234);
    TEST("FLOGNP1.B", "#$23, FP4",           0xF200|074, 0x4006|(6<<10)|(4<<7), 0x0023);
    TFLT("FLOGNP1.S", "#7.88999976E-10, FP7", "#$3058E0F0, FP7",
         0xF200|074, 0x4006|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    TFLT("FLOGNP1.X", "#-89000000032, FP0", "#$C0230000A5C681D100000000, FP0",
         0xF200|074, 0x4006|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    TFLT("FLOGNP1.P", "#-INF, FP1",      "#$FFFF00000000000000000000, FP1",
       0xF200|074, 0x4006|(3<<10)|(1<<7), 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000);
    TFLT("FLOGNP1.D", "#-8.25, FP3",       "#$C020800000000000, FP3",
         0xF200|074, 0x4006|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FETOXM1.X", "FP0, FP1",            0xF200,     0x0008|(0<<10)|(1<<7));
    TEST("FETOXM1.X", "FP2",                 0xF200,     0x0008|(2<<10)|(2<<7));
    TEST("FETOXM1.L", "D2, FP3",             0xF200|002, 0x4008|(0<<10)|(3<<7));
    TEST("FETOXM1.S", "D2, FP3",             0xF200|002, 0x4008|(1<<10)|(3<<7));
    ERRT("FETOXM1.X", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                           0xF200|002, 0x4008|(2<<10)|(3<<7));
    ERRT("FETOXM1.P", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                           0xF200|002, 0x4008|(3<<10)|(3<<7));
    TEST("FETOXM1.W", "D2, FP3",             0xF200|002, 0x4008|(4<<10)|(3<<7));
    ERRT("FETOXM1.D", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                           0xF200|002, 0x4008|(5<<10)|(3<<7));
    TEST("FETOXM1.B", "D2, FP3",             0xF200|002, 0x4008|(6<<10)|(3<<7));
    UNKN(/* FETOXM1.P D2, FP3 */             0xF200|002, 0x4008|(7<<10)|(3<<7));
    UNKN(/* FETOXM1.S A4, FP5 */             0xF200|014, 0x4008|(1<<10)|(5<<7));
    TEST("FETOXM1.X", "(A6), FP7",           0xF200|026, 0x4008|(2<<10)|(7<<7));
    TEST("FETOXM1.P", "(A0)+, FP1",          0xF200|030, 0x4008|(3<<10)|(1<<7));
    TEST("FETOXM1.W", "-(A2), FP3",          0xF200|042, 0x4008|(4<<10)|(3<<7));
    TEST("FETOXM1.D", "($1234,A4), FP5",     0xF200|054, 0x4008|(5<<10)|(5<<7), 0x1234);
    TEST("FETOXM1.B", "($23,A6,D7.W), FP0",  0xF200|066, 0x4008|(6<<10)|(0<<7), 0x7023);
    ABST("FETOXM1.L", "(@A).W, FP1", "4566",   0xF200|070, 0x4008|(0<<10)|(1<<7), 0x4566);
    ABST("FETOXM1.S", "(@A).L, FP2", "56789A", 0xF200|071, 0x4008|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FETOXM1.X", "(*+$1234,PC), FP3",   0xF200|072, 0x4008|(2<<10)|(3<<7), 0x1230);
    TEST("FETOXM1.P", "(*+90,PC,A4.L), FP5", 0xF200|073, 0x4008|(3<<10)|(5<<7), 0xC856);
    TEST("FETOXM1.L", "#$6789ABCD, FP6",     0xF200|074, 0x4008|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FETOXM1.W", "#$1234, FP2",         0xF200|074, 0x4008|(4<<10)|(2<<7), 0x1234);
    TEST("FETOXM1.B", "#$23, FP4",           0xF200|074, 0x4008|(6<<10)|(4<<7), 0x0023);
    TFLT("FETOXM1.S", "#7.88999976E-10, FP7", "#$3058E0F0, FP7",
         0xF200|074, 0x4008|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    TFLT("FETOXM1.X", "#-89000000032, FP0", "#$C0230000A5C681D100000000, FP0",
         0xF200|074, 0x4008|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    TFLT("FETOXM1.P", "#NAN, FP1",           "#$7FFF00091200000000000000, FP1",
           0xF200|074, 0x4008|(3<<10)|(1<<7), 0x7FFF, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    TFLT("FETOXM1.D", "#-8.25, FP3",       "#$C020800000000000, FP3",
         0xF200|074, 0x4008|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FTANH.X", "FP0, FP1",            0xF200,     0x0009|(0<<10)|(1<<7));
    TEST("FTANH.X", "FP2",                 0xF200,     0x0009|(2<<10)|(2<<7));
    TEST("FTANH.L", "D2, FP3",             0xF200|002, 0x4009|(0<<10)|(3<<7));
    TEST("FTANH.S", "D2, FP3",             0xF200|002, 0x4009|(1<<10)|(3<<7));
    ERRT("FTANH.X", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                           0xF200|002, 0x4009|(2<<10)|(3<<7));
    ERRT("FTANH.P", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                           0xF200|002, 0x4009|(3<<10)|(3<<7));
    TEST("FTANH.W", "D2, FP3",             0xF200|002, 0x4009|(4<<10)|(3<<7));
    ERRT("FTANH.D", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                           0xF200|002, 0x4009|(5<<10)|(3<<7));
    TEST("FTANH.B", "D2, FP3",             0xF200|002, 0x4009|(6<<10)|(3<<7));
    UNKN(/* FTANH.P D2, FP3 */             0xF200|002, 0x4009|(7<<10)|(3<<7));
    UNKN(/* FTANH.S A4, FP5 */             0xF200|014, 0x4009|(1<<10)|(5<<7));
    TEST("FTANH.X", "(A6), FP7",           0xF200|026, 0x4009|(2<<10)|(7<<7));
    TEST("FTANH.P", "(A0)+, FP1",          0xF200|030, 0x4009|(3<<10)|(1<<7));
    TEST("FTANH.W", "-(A2), FP3",          0xF200|042, 0x4009|(4<<10)|(3<<7));
    TEST("FTANH.D", "($1234,A4), FP5",     0xF200|054, 0x4009|(5<<10)|(5<<7), 0x1234);
    TEST("FTANH.B", "($23,A6,D7.W), FP0",  0xF200|066, 0x4009|(6<<10)|(0<<7), 0x7023);
    ABST("FTANH.L", "(@A).W, FP1", "4566",   0xF200|070, 0x4009|(0<<10)|(1<<7), 0x4566);
    ABST("FTANH.S", "(@A).L, FP2", "56789A", 0xF200|071, 0x4009|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FTANH.X", "(*+$1234,PC), FP3",   0xF200|072, 0x4009|(2<<10)|(3<<7), 0x1230);
    TEST("FTANH.P", "(*+90,PC,A4.L), FP5", 0xF200|073, 0x4009|(3<<10)|(5<<7), 0xC856);
    TEST("FTANH.L", "#$6789ABCD, FP6",     0xF200|074, 0x4009|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FTANH.W", "#$1234, FP2",         0xF200|074, 0x4009|(4<<10)|(2<<7), 0x1234);
    TEST("FTANH.B", "#$23, FP4",           0xF200|074, 0x4009|(6<<10)|(4<<7), 0x0023);
    TFLT("FTANH.S", "#7.88999976E-10, FP7", "#$3058E0F0, FP7",
         0xF200|074, 0x4009|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    TFLT("FTANH.X", "#-89000000032, FP0",  "#$C0230000A5C681D100000000, FP0",
         0xF200|074, 0x4009|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    TFLT("FTANH.P", "#9.12E+999, FP1",     "#$099900091200000000000000, FP1",
         0xF200|074, 0x4009|(3<<10)|(1<<7), 0x0999, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    TFLT("FTANH.D", "#-8.25, FP3",         "#$C020800000000000, FP3",
         0xF200|074, 0x4009|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FATAN.X", "FP0, FP1",            0xF200,     0x000A|(0<<10)|(1<<7));
    TEST("FATAN.X", "FP2",                 0xF200,     0x000A|(2<<10)|(2<<7));
    TEST("FATAN.L", "D2, FP3",             0xF200|002, 0x400A|(0<<10)|(3<<7));
    TEST("FATAN.S", "D2, FP3",             0xF200|002, 0x400A|(1<<10)|(3<<7));
    ERRT("FATAN.X", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                           0xF200|002, 0x400A|(2<<10)|(3<<7));
    ERRT("FATAN.P", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                           0xF200|002, 0x400A|(3<<10)|(3<<7));
    TEST("FATAN.W", "D2, FP3",             0xF200|002, 0x400A|(4<<10)|(3<<7));
    ERRT("FATAN.D", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                           0xF200|002, 0x400A|(5<<10)|(3<<7));
    TEST("FATAN.B", "D2, FP3",             0xF200|002, 0x400A|(6<<10)|(3<<7));
    UNKN(/* FATAN.P D2, FP3 */             0xF200|002, 0x400A|(7<<10)|(3<<7));
    UNKN(/* FATAN.S A4, FP5 */             0xF200|014, 0x400A|(1<<10)|(5<<7));
    TEST("FATAN.X", "(A6), FP7",           0xF200|026, 0x400A|(2<<10)|(7<<7));
    TEST("FATAN.P", "(A0)+, FP1",          0xF200|030, 0x400A|(3<<10)|(1<<7));
    TEST("FATAN.W", "-(A2), FP3",          0xF200|042, 0x400A|(4<<10)|(3<<7));
    TEST("FATAN.D", "($1234,A4), FP5",     0xF200|054, 0x400A|(5<<10)|(5<<7), 0x1234);
    TEST("FATAN.B", "($23,A6,D7.W), FP0",  0xF200|066, 0x400A|(6<<10)|(0<<7), 0x7023);
    ABST("FATAN.L", "(@A).W, FP1", "4566",   0xF200|070, 0x400A|(0<<10)|(1<<7), 0x4566);
    ABST("FATAN.S", "(@A).L, FP2", "56789A", 0xF200|071, 0x400A|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FATAN.X", "(*+$1234,PC), FP3",   0xF200|072, 0x400A|(2<<10)|(3<<7), 0x1230);
    TEST("FATAN.P", "(*+90,PC,A4.L), FP5", 0xF200|073, 0x400A|(3<<10)|(5<<7), 0xC856);
    TEST("FATAN.L", "#$6789ABCD, FP6",     0xF200|074, 0x400A|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FATAN.W", "#$1234, FP2",         0xF200|074, 0x400A|(4<<10)|(2<<7), 0x1234);
    TEST("FATAN.B", "#$23, FP4",           0xF200|074, 0x400A|(6<<10)|(4<<7), 0x0023);
    TFLT("FATAN.S", "#7.88999976E-10, FP7", "#$3058E0F0, FP7",
         0xF200|074, 0x400A|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    TFLT("FATAN.X", "#-89000000032, FP0",  "#$C0230000A5C681D100000000, FP0",
         0xF200|074, 0x400A|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    TFLT("FATAN.P", "#9.12E-999, FP1",     "#$499900091200000000000000, FP1",
         0xF200|074, 0x400A|(3<<10)|(1<<7), 0x4999, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    TFLT("FATAN.D", "#-8.25, FP3",         "#$C020800000000000, FP3",
         0xF200|074, 0x400A|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FASIN.X", "FP0, FP1",            0xF200,     0x000C|(0<<10)|(1<<7));
    TEST("FASIN.X", "FP2",                 0xF200,     0x000C|(2<<10)|(2<<7));
    TEST("FASIN.L", "D2, FP3",             0xF200|002, 0x400C|(0<<10)|(3<<7));
    TEST("FASIN.S", "D2, FP3",             0xF200|002, 0x400C|(1<<10)|(3<<7));
    ERRT("FASIN.X", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                           0xF200|002, 0x400C|(2<<10)|(3<<7));
    ERRT("FASIN.P", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                           0xF200|002, 0x400C|(3<<10)|(3<<7));
    TEST("FASIN.W", "D2, FP3",             0xF200|002, 0x400C|(4<<10)|(3<<7));
    ERRT("FASIN.D", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                           0xF200|002, 0x400C|(5<<10)|(3<<7));
    TEST("FASIN.B", "D2, FP3",             0xF200|002, 0x400C|(6<<10)|(3<<7));
    UNKN(/* FASIN.P D2, FP3 */             0xF200|002, 0x400C|(7<<10)|(3<<7));
    UNKN(/* FASIN.S A4, FP5 */             0xF200|014, 0x400C|(1<<10)|(5<<7));
    TEST("FASIN.X", "(A6), FP7",           0xF200|026, 0x400C|(2<<10)|(7<<7));
    TEST("FASIN.P", "(A0)+, FP1",          0xF200|030, 0x400C|(3<<10)|(1<<7));
    TEST("FASIN.W", "-(A2), FP3",          0xF200|042, 0x400C|(4<<10)|(3<<7));
    TEST("FASIN.D", "($1234,A4), FP5",     0xF200|054, 0x400C|(5<<10)|(5<<7), 0x1234);
    TEST("FASIN.B", "($23,A6,D7.W), FP0",  0xF200|066, 0x400C|(6<<10)|(0<<7), 0x7023);
    ABST("FASIN.L", "(@A).W, FP1", "4566",   0xF200|070, 0x400C|(0<<10)|(1<<7), 0x4566);
    ABST("FASIN.S", "(@A).L, FP2", "56789A", 0xF200|071, 0x400C|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FASIN.X", "(*+$1234,PC), FP3",   0xF200|072, 0x400C|(2<<10)|(3<<7), 0x1230);
    TEST("FASIN.P", "(*+90,PC,A4.L), FP5", 0xF200|073, 0x400C|(3<<10)|(5<<7), 0xC856);
    TEST("FASIN.L", "#$6789ABCD, FP6",     0xF200|074, 0x400C|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FASIN.W", "#$1234, FP2",         0xF200|074, 0x400C|(4<<10)|(2<<7), 0x1234);
    TEST("FASIN.B", "#$23, FP4",           0xF200|074, 0x400C|(6<<10)|(4<<7), 0x0023);
    TFLT("FASIN.S", "#7.88999976E-10, FP7", "#$3058E0F0, FP7",
         0xF200|074, 0x400C|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    TFLT("FASIN.X", "#-89000000032, FP0",  "#$C0230000A5C681D100000000, FP0",
         0xF200|074, 0x400C|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    TFLT("FASIN.P", "#9.12E+20, FP1",      "#$002000091200000000000000, FP1",
         0xF200|074, 0x400C|(3<<10)|(1<<7), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    TFLT("FASIN.D", "#-8.25, FP3",         "#$C020800000000000, FP3",
         0xF200|074, 0x400C|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FATANH.X", "FP0, FP1",            0xF200,     0x000D|(0<<10)|(1<<7));
    TEST("FATANH.X", "FP2",                 0xF200,     0x000D|(2<<10)|(2<<7));
    TEST("FATANH.L", "D2, FP3",             0xF200|002, 0x400D|(0<<10)|(3<<7));
    UNKN(/* FATANH.S A4, FP5 */             0xF200|014, 0x400D|(1<<10)|(5<<7));
    TEST("FATANH.X", "(A6), FP7",           0xF200|026, 0x400D|(2<<10)|(7<<7));
    TEST("FATANH.P", "(A0)+, FP1",          0xF200|030, 0x400D|(3<<10)|(1<<7));
    TEST("FATANH.W", "-(A2), FP3",          0xF200|042, 0x400D|(4<<10)|(3<<7));
    TEST("FATANH.D", "($1234,A4), FP5",     0xF200|054, 0x400D|(5<<10)|(5<<7), 0x1234);
    TEST("FATANH.B", "($23,A6,D7.W), FP0",  0xF200|066, 0x400D|(6<<10)|(0<<7), 0x7023);
    ABST("FATANH.L", "(@A).W, FP1", "4566",   0xF200|070, 0x400D|(0<<10)|(1<<7), 0x4566);
    ABST("FATANH.S", "(@A).L, FP2", "56789A", 0xF200|071, 0x400D|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FATANH.X", "(*+$1234,PC), FP3",   0xF200|072, 0x400D|(2<<10)|(3<<7), 0x1230);
    TEST("FATANH.P", "(*+90,PC,A4.L), FP5", 0xF200|073, 0x400D|(3<<10)|(5<<7), 0xC856);
    TEST("FATANH.L", "#$6789ABCD, FP6",     0xF200|074, 0x400D|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FATANH.W", "#$1234, FP2",         0xF200|074, 0x400D|(4<<10)|(2<<7), 0x1234);
    TEST("FATANH.B", "#$23, FP4",           0xF200|074, 0x400D|(6<<10)|(4<<7), 0x0023);
    TFLT("FATANH.S", "#7.88999976E-10, FP7", "#$3058E0F0, FP7",
         0xF200|074, 0x400D|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    TFLT("FATANH.X", "#-89000000032, FP0", "#$C0230000A5C681D100000000, FP0",
         0xF200|074, 0x400D|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    TFLT("FATANH.P", "#9.12E+20, FP1",     "#$002000091200000000000000, FP1",
         0xF200|074, 0x400D|(3<<10)|(1<<7), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    TFLT("FATANH.D", "#-8.25, FP3",        "#$C020800000000000, FP3",
         0xF200|074, 0x400D|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FSIN.X", "FP0, FP1",            0xF200,     0x000E|(0<<10)|(1<<7));
    TEST("FSIN.X", "FP2",                 0xF200,     0x000E|(2<<10)|(2<<7));
    TEST("FSIN.L", "D2, FP3",             0xF200|002, 0x400E|(0<<10)|(3<<7));
    TEST("FSIN.S", "D2, FP3",             0xF200|002, 0x400E|(1<<10)|(3<<7));
    ERRT("FSIN.X", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                          0xF200|002, 0x400E|(2<<10)|(3<<7));
    ERRT("FSIN.P", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                          0xF200|002, 0x400E|(3<<10)|(3<<7));
    TEST("FSIN.W", "D2, FP3",             0xF200|002, 0x400E|(4<<10)|(3<<7));
    ERRT("FSIN.D", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                          0xF200|002, 0x400E|(5<<10)|(3<<7));
    TEST("FSIN.B", "D2, FP3",             0xF200|002, 0x400E|(6<<10)|(3<<7));
    UNKN(/* FSIN.P D2, FP3 */             0xF200|002, 0x400E|(7<<10)|(3<<7));
    UNKN(/* FSIN.S A4, FP5 */             0xF200|014, 0x400E|(1<<10)|(5<<7));
    TEST("FSIN.X", "(A6), FP7",           0xF200|026, 0x400E|(2<<10)|(7<<7));
    TEST("FSIN.P", "(A0)+, FP1",          0xF200|030, 0x400E|(3<<10)|(1<<7));
    TEST("FSIN.W", "-(A2), FP3",          0xF200|042, 0x400E|(4<<10)|(3<<7));
    TEST("FSIN.D", "($1234,A4), FP5",     0xF200|054, 0x400E|(5<<10)|(5<<7), 0x1234);
    TEST("FSIN.B", "($23,A6,D7.W), FP0",  0xF200|066, 0x400E|(6<<10)|(0<<7), 0x7023);
    ABST("FSIN.L", "(@A).W, FP1", "4566",   0xF200|070, 0x400E|(0<<10)|(1<<7), 0x4566);
    ABST("FSIN.S", "(@A).L, FP2", "56789A", 0xF200|071, 0x400E|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FSIN.X", "(*+$1234,PC), FP3",   0xF200|072, 0x400E|(2<<10)|(3<<7), 0x1230);
    TEST("FSIN.P", "(*+90,PC,A4.L), FP5", 0xF200|073, 0x400E|(3<<10)|(5<<7), 0xC856);
    TEST("FSIN.L", "#$6789ABCD, FP6",     0xF200|074, 0x400E|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FSIN.W", "#$1234, FP2",         0xF200|074, 0x400E|(4<<10)|(2<<7), 0x1234);
    TEST("FSIN.B", "#$23, FP4",           0xF200|074, 0x400E|(6<<10)|(4<<7), 0x0023);
    TFLT("FSIN.S", "#7.88999976E-10, FP7", "#$3058E0F0, FP7",
         0xF200|074, 0x400E|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    TFLT("FSIN.X", "#-89000000032, FP0",   "#$C0230000A5C681D100000000, FP0",
         0xF200|074, 0x400E|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    TFLT("FSIN.P", "#9.12E+20, FP1",       "#$002000091200000000000000, FP1",
         0xF200|074, 0x400E|(3<<10)|(1<<7), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    TFLT("FSIN.D", "#-8.25, FP3",          "#$C020800000000000, FP3",
         0xF200|074, 0x400E|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FTAN.X", "FP0, FP1",            0xF200,     0x000F|(0<<10)|(1<<7));
    TEST("FTAN.X", "FP2",                 0xF200,     0x000F|(2<<10)|(2<<7));
    TEST("FTAN.L", "D2, FP3",             0xF200|002, 0x400F|(0<<10)|(3<<7));
    TEST("FTAN.S", "D2, FP3",             0xF200|002, 0x400F|(1<<10)|(3<<7));
    ERRT("FTAN.X", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                          0xF200|002, 0x400F|(2<<10)|(3<<7));
    ERRT("FTAN.P", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                          0xF200|002, 0x400F|(3<<10)|(3<<7));
    TEST("FTAN.W", "D2, FP3",             0xF200|002, 0x400F|(4<<10)|(3<<7));
    ERRT("FTAN.D", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                          0xF200|002, 0x400F|(5<<10)|(3<<7));
    TEST("FTAN.B", "D2, FP3",             0xF200|002, 0x400F|(6<<10)|(3<<7));
    UNKN(/* FTAN.P D2, FP3 */             0xF200|002, 0x400F|(7<<10)|(3<<7));
    UNKN(/* FTAN.S A4, FP5 */             0xF200|014, 0x400F|(1<<10)|(5<<7));
    TEST("FTAN.X", "(A6), FP7",           0xF200|026, 0x400F|(2<<10)|(7<<7));
    TEST("FTAN.P", "(A0)+, FP1",          0xF200|030, 0x400F|(3<<10)|(1<<7));
    TEST("FTAN.W", "-(A2), FP3",          0xF200|042, 0x400F|(4<<10)|(3<<7));
    TEST("FTAN.D", "($1234,A4), FP5",     0xF200|054, 0x400F|(5<<10)|(5<<7), 0x1234);
    TEST("FTAN.B", "($23,A6,D7.W), FP0",  0xF200|066, 0x400F|(6<<10)|(0<<7), 0x7023);
    ABST("FTAN.L", "(@A).W, FP1", "4566",   0xF200|070, 0x400F|(0<<10)|(1<<7), 0x4566);
    ABST("FTAN.S", "(@A).L, FP2", "56789A", 0xF200|071, 0x400F|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FTAN.X", "(*+$1234,PC), FP3",   0xF200|072, 0x400F|(2<<10)|(3<<7), 0x1230);
    TEST("FTAN.P", "(*+90,PC,A4.L), FP5", 0xF200|073, 0x400F|(3<<10)|(5<<7), 0xC856);
    TEST("FTAN.L", "#$6789ABCD, FP6",     0xF200|074, 0x400F|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FTAN.W", "#$1234, FP2",         0xF200|074, 0x400F|(4<<10)|(2<<7), 0x1234);
    TEST("FTAN.B", "#$23, FP4",           0xF200|074, 0x400F|(6<<10)|(4<<7), 0x0023);
    TFLT("FTAN.S", "#7.88999976E-10, FP7", "#$3058E0F0, FP7",
         0xF200|074, 0x400F|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    TFLT("FTAN.X", "#-89000000032, FP0",   "#$C0230000A5C681D100000000, FP0",
         0xF200|074, 0x400F|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    TFLT("FTAN.P", "#9.12E+20, FP1",       "#$002000091200000000000000, FP1",
         0xF200|074, 0x400F|(3<<10)|(1<<7), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    TFLT("FTAN.D", "#-8.25, FP3",          "#$C020800000000000, FP3",
         0xF200|074, 0x400F|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FETOX.X", "FP0, FP1",            0xF200,     0x0010|(0<<10)|(1<<7));
    TEST("FETOX.X", "FP2",                 0xF200,     0x0010|(2<<10)|(2<<7));
    TEST("FETOX.L", "D2, FP3",             0xF200|002, 0x4010|(0<<10)|(3<<7));
    TEST("FETOX.S", "D2, FP3",             0xF200|002, 0x4010|(1<<10)|(3<<7));
    ERRT("FETOX.X", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                           0xF200|002, 0x4010|(2<<10)|(3<<7));
    ERRT("FETOX.P", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                           0xF200|002, 0x4010|(3<<10)|(3<<7));
    TEST("FETOX.W", "D2, FP3",             0xF200|002, 0x4010|(4<<10)|(3<<7));
    ERRT("FETOX.D", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                           0xF200|002, 0x4010|(5<<10)|(3<<7));
    TEST("FETOX.B", "D2, FP3",             0xF200|002, 0x4010|(6<<10)|(3<<7));
    UNKN(/* FETOX.P D2, FP3 */             0xF200|002, 0x4010|(7<<10)|(3<<7));
    UNKN(/* FETOX.S A4, FP5 */             0xF200|014, 0x4010|(1<<10)|(5<<7));
    TEST("FETOX.X", "(A6), FP7",           0xF200|026, 0x4010|(2<<10)|(7<<7));
    TEST("FETOX.P", "(A0)+, FP1",          0xF200|030, 0x4010|(3<<10)|(1<<7));
    TEST("FETOX.W", "-(A2), FP3",          0xF200|042, 0x4010|(4<<10)|(3<<7));
    TEST("FETOX.D", "($1234,A4), FP5",     0xF200|054, 0x4010|(5<<10)|(5<<7), 0x1234);
    TEST("FETOX.B", "($23,A6,D7.W), FP0",  0xF200|066, 0x4010|(6<<10)|(0<<7), 0x7023);
    ABST("FETOX.L", "(@A).W, FP1", "4566",   0xF200|070, 0x4010|(0<<10)|(1<<7), 0x4566);
    ABST("FETOX.S", "(@A).L, FP2", "56789A", 0xF200|071, 0x4010|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FETOX.X", "(*+$1234,PC), FP3",   0xF200|072, 0x4010|(2<<10)|(3<<7), 0x1230);
    TEST("FETOX.P", "(*+90,PC,A4.L), FP5", 0xF200|073, 0x4010|(3<<10)|(5<<7), 0xC856);
    TEST("FETOX.L", "#$6789ABCD, FP6",     0xF200|074, 0x4010|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FETOX.W", "#$1234, FP2",         0xF200|074, 0x4010|(4<<10)|(2<<7), 0x1234);
    TEST("FETOX.B", "#$23, FP4",           0xF200|074, 0x4010|(6<<10)|(4<<7), 0x0023);
    TFLT("FETOX.S", "#7.88999976E-10, FP7", "#$3058E0F0, FP7",
         0xF200|074, 0x4010|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    TFLT("FETOX.X", "#-89000000032, FP0",  "#$C0230000A5C681D100000000, FP0",
         0xF200|074, 0x4010|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    TFLT("FETOX.P", "#9.12E+20, FP1",      "#$002000091200000000000000, FP1",
         0xF200|074, 0x4010|(3<<10)|(1<<7), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    TFLT("FETOX.D", "#-8.25, FP3",         "#$C020800000000000, FP3",
         0xF200|074, 0x4010|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FTWOTOX.X", "FP0, FP1",            0xF200,     0x0011|(0<<10)|(1<<7));
    TEST("FTWOTOX.X", "FP2",                 0xF200,     0x0011|(2<<10)|(2<<7));
    TEST("FTWOTOX.L", "D2, FP3",             0xF200|002, 0x4011|(0<<10)|(3<<7));
    TEST("FTWOTOX.S", "D2, FP3",             0xF200|002, 0x4011|(1<<10)|(3<<7));
    ERRT("FTWOTOX.X", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                             0xF200|002, 0x4011|(2<<10)|(3<<7));
    ERRT("FTWOTOX.P", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                             0xF200|002, 0x4011|(3<<10)|(3<<7));
    TEST("FTWOTOX.W", "D2, FP3",             0xF200|002, 0x4011|(4<<10)|(3<<7));
    ERRT("FTWOTOX.D", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                             0xF200|002, 0x4011|(5<<10)|(3<<7));
    TEST("FTWOTOX.B", "D2, FP3",             0xF200|002, 0x4011|(6<<10)|(3<<7));
    UNKN(/* FTWOTOX.P D2, FP3 */             0xF200|002, 0x4011|(7<<10)|(3<<7));
    UNKN(/* FTWOTOX.S A4, FP5 */             0xF200|014, 0x4011|(1<<10)|(5<<7));
    TEST("FTWOTOX.X", "(A6), FP7",           0xF200|026, 0x4011|(2<<10)|(7<<7));
    TEST("FTWOTOX.P", "(A0)+, FP1",          0xF200|030, 0x4011|(3<<10)|(1<<7));
    TEST("FTWOTOX.W", "-(A2), FP3",          0xF200|042, 0x4011|(4<<10)|(3<<7));
    TEST("FTWOTOX.D", "($1234,A4), FP5",     0xF200|054, 0x4011|(5<<10)|(5<<7), 0x1234);
    TEST("FTWOTOX.B", "($23,A6,D7.W), FP0",  0xF200|066, 0x4011|(6<<10)|(0<<7), 0x7023);
    ABST("FTWOTOX.L", "(@A).W, FP1", "4566",   0xF200|070, 0x4011|(0<<10)|(1<<7), 0x4566);
    ABST("FTWOTOX.S", "(@A).L, FP2", "56789A", 0xF200|071, 0x4011|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FTWOTOX.X", "(*+$1234,PC), FP3",   0xF200|072, 0x4011|(2<<10)|(3<<7), 0x1230);
    TEST("FTWOTOX.P", "(*+90,PC,A4.L), FP5", 0xF200|073, 0x4011|(3<<10)|(5<<7), 0xC856);
    TEST("FTWOTOX.L", "#$6789ABCD, FP6",     0xF200|074, 0x4011|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FTWOTOX.W", "#$1234, FP2",         0xF200|074, 0x4011|(4<<10)|(2<<7), 0x1234);
    TEST("FTWOTOX.B", "#$23, FP4",           0xF200|074, 0x4011|(6<<10)|(4<<7), 0x0023);
    TFLT("FTWOTOX.S", "#7.88999976E-10, FP7", "#$3058E0F0, FP7",
         0xF200|074, 0x4011|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    TFLT("FTWOTOX.X", "#-89000000032, FP0", "#$C0230000A5C681D100000000, FP0",
         0xF200|074, 0x4011|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    TFLT("FTWOTOX.P", "#9.12E+20, FP1",    "#$002000091200000000000000, FP1",
         0xF200|074, 0x4011|(3<<10)|(1<<7), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    TFLT("FTWOTOX.D", "#-8.25, FP3",       "#$C020800000000000, FP3",
         0xF200|074, 0x4011|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FTENTOX.X", "FP0, FP1",            0xF200,     0x0012|(0<<10)|(1<<7));
    TEST("FTENTOX.X", "FP2",                 0xF200,     0x0012|(2<<10)|(2<<7));
    TEST("FTENTOX.L", "D2, FP3",             0xF200|002, 0x4012|(0<<10)|(3<<7));
    TEST("FTENTOX.S", "D2, FP3",             0xF200|002, 0x4012|(1<<10)|(3<<7));
    ERRT("FTENTOX.X", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                             0xF200|002, 0x4012|(2<<10)|(3<<7));
    ERRT("FTENTOX.P", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                             0xF200|002, 0x4012|(3<<10)|(3<<7));
    TEST("FTENTOX.W", "D2, FP3",             0xF200|002, 0x4012|(4<<10)|(3<<7));
    ERRT("FTENTOX.D", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                             0xF200|002, 0x4012|(5<<10)|(3<<7));
    TEST("FTENTOX.B", "D2, FP3",             0xF200|002, 0x4012|(6<<10)|(3<<7));
    UNKN(/* FTENTOX.P D2, FP3 */             0xF200|002, 0x4012|(7<<10)|(3<<7));
    UNKN(/* FTENTOX.S A4, FP5 */             0xF200|014, 0x4012|(1<<10)|(5<<7));
    TEST("FTENTOX.X", "(A6), FP7",           0xF200|026, 0x4012|(2<<10)|(7<<7));
    TEST("FTENTOX.P", "(A0)+, FP1",          0xF200|030, 0x4012|(3<<10)|(1<<7));
    TEST("FTENTOX.W", "-(A2), FP3",          0xF200|042, 0x4012|(4<<10)|(3<<7));
    TEST("FTENTOX.D", "($1234,A4), FP5",     0xF200|054, 0x4012|(5<<10)|(5<<7), 0x1234);
    TEST("FTENTOX.B", "($23,A6,D7.W), FP0",  0xF200|066, 0x4012|(6<<10)|(0<<7), 0x7023);
    ABST("FTENTOX.L", "(@A).W, FP1", "4566",   0xF200|070, 0x4012|(0<<10)|(1<<7), 0x4566);
    ABST("FTENTOX.S", "(@A).L, FP2", "56789A", 0xF200|071, 0x4012|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FTENTOX.X", "(*+$1234,PC), FP3",   0xF200|072, 0x4012|(2<<10)|(3<<7), 0x1230);
    TEST("FTENTOX.P", "(*+90,PC,A4.L), FP5", 0xF200|073, 0x4012|(3<<10)|(5<<7), 0xC856);
    TEST("FTENTOX.L", "#$6789ABCD, FP6",     0xF200|074, 0x4012|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FTENTOX.W", "#$1234, FP2",         0xF200|074, 0x4012|(4<<10)|(2<<7), 0x1234);
    TEST("FTENTOX.B", "#$23, FP4",           0xF200|074, 0x4012|(6<<10)|(4<<7), 0x0023);
    TFLT("FTENTOX.S", "#7.88999976E-10, FP7", "#$3058E0F0, FP7",
         0xF200|074, 0x4012|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    TFLT("FTENTOX.X", "#-89000000032, FP0", "#$C0230000A5C681D100000000, FP0",
         0xF200|074, 0x4012|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    TFLT("FTENTOX.P", "#9.12E+20, FP1",    "#$002000091200000000000000, FP1",
         0xF200|074, 0x4012|(3<<10)|(1<<7), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    TFLT("FTENTOX.D", "#-8.25, FP3",       "#$C020800000000000, FP3",
         0xF200|074, 0x4012|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FLOGN.X", "FP0, FP1",            0xF200,     0x0014|(0<<10)|(1<<7));
    TEST("FLOGN.X", "FP2",                 0xF200,     0x0014|(2<<10)|(2<<7));
    TEST("FLOGN.L", "D2, FP3",             0xF200|002, 0x4014|(0<<10)|(3<<7));
    TEST("FLOGN.S", "D2, FP3",             0xF200|002, 0x4014|(1<<10)|(3<<7));
    ERRT("FLOGN.X", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                           0xF200|002, 0x4014|(2<<10)|(3<<7));
    ERRT("FLOGN.P", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                           0xF200|002, 0x4014|(3<<10)|(3<<7));
    TEST("FLOGN.W", "D2, FP3",             0xF200|002, 0x4014|(4<<10)|(3<<7));
    ERRT("FLOGN.D", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                           0xF200|002, 0x4014|(5<<10)|(3<<7));
    TEST("FLOGN.B", "D2, FP3",             0xF200|002, 0x4014|(6<<10)|(3<<7));
    UNKN(/* FLOGN.P D2, FP3 */             0xF200|002, 0x4014|(7<<10)|(3<<7));
    UNKN(/* FLOGN.S A4, FP5 */             0xF200|014, 0x4014|(1<<10)|(5<<7));
    TEST("FLOGN.X", "(A6), FP7",           0xF200|026, 0x4014|(2<<10)|(7<<7));
    TEST("FLOGN.P", "(A0)+, FP1",          0xF200|030, 0x4014|(3<<10)|(1<<7));
    TEST("FLOGN.W", "-(A2), FP3",          0xF200|042, 0x4014|(4<<10)|(3<<7));
    TEST("FLOGN.D", "($1234,A4), FP5",     0xF200|054, 0x4014|(5<<10)|(5<<7), 0x1234);
    TEST("FLOGN.B", "($23,A6,D7.W), FP0",  0xF200|066, 0x4014|(6<<10)|(0<<7), 0x7023);
    ABST("FLOGN.L", "(@A).W, FP1", "4566",   0xF200|070, 0x4014|(0<<10)|(1<<7), 0x4566);
    ABST("FLOGN.S", "(@A).L, FP2", "56789A", 0xF200|071, 0x4014|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FLOGN.X", "(*+$1234,PC), FP3",   0xF200|072, 0x4014|(2<<10)|(3<<7), 0x1230);
    TEST("FLOGN.P", "(*+90,PC,A4.L), FP5", 0xF200|073, 0x4014|(3<<10)|(5<<7), 0xC856);
    TEST("FLOGN.L", "#$6789ABCD, FP6",     0xF200|074, 0x4014|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FLOGN.W", "#$1234, FP2",         0xF200|074, 0x4014|(4<<10)|(2<<7), 0x1234);
    TEST("FLOGN.B", "#$23, FP4",           0xF200|074, 0x4014|(6<<10)|(4<<7), 0x0023);
    TFLT("FLOGN.S", "#7.88999976E-10, FP7", "#$3058E0F0, FP7",
         0xF200|074, 0x4014|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    TFLT("FLOGN.X", "#-89000000032, FP0",  "#$C0230000A5C681D100000000, FP0",
         0xF200|074, 0x4014|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    TFLT("FLOGN.P", "#9.12E+20, FP1",      "#$002000091200000000000000, FP1",
         0xF200|074, 0x4014|(3<<10)|(1<<7), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    TFLT("FLOGN.D", "#-8.25, FP3",         "#$C020800000000000, FP3",
         0xF200|074, 0x4014|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FLOG10.X", "FP0, FP1",            0xF200,     0x0015|(0<<10)|(1<<7));
    TEST("FLOG10.X", "FP2",                 0xF200,     0x0015|(2<<10)|(2<<7));
    TEST("FLOG10.L", "D2, FP3",             0xF200|002, 0x4015|(0<<10)|(3<<7));
    TEST("FLOG10.S", "D2, FP3",             0xF200|002, 0x4015|(1<<10)|(3<<7));
    ERRT("FLOG10.X", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                            0xF200|002, 0x4015|(2<<10)|(3<<7));
    ERRT("FLOG10.P", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                            0xF200|002, 0x4015|(3<<10)|(3<<7));
    TEST("FLOG10.W", "D2, FP3",             0xF200|002, 0x4015|(4<<10)|(3<<7));
    ERRT("FLOG10.D", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                            0xF200|002, 0x4015|(5<<10)|(3<<7));
    TEST("FLOG10.B", "D2, FP3",             0xF200|002, 0x4015|(6<<10)|(3<<7));
    UNKN(/* FLOG10.P D2, FP3 */             0xF200|002, 0x4015|(7<<10)|(3<<7));
    UNKN(/* FLOG10.S A4, FP5 */             0xF200|014, 0x4015|(1<<10)|(5<<7));
    TEST("FLOG10.X", "(A6), FP7",           0xF200|026, 0x4015|(2<<10)|(7<<7));
    TEST("FLOG10.P", "(A0)+, FP1",          0xF200|030, 0x4015|(3<<10)|(1<<7));
    TEST("FLOG10.W", "-(A2), FP3",          0xF200|042, 0x4015|(4<<10)|(3<<7));
    TEST("FLOG10.D", "($1234,A4), FP5",     0xF200|054, 0x4015|(5<<10)|(5<<7), 0x1234);
    TEST("FLOG10.B", "($23,A6,D7.W), FP0",  0xF200|066, 0x4015|(6<<10)|(0<<7), 0x7023);
    ABST("FLOG10.L", "(@A).W, FP1", "4566",   0xF200|070, 0x4015|(0<<10)|(1<<7), 0x4566);
    ABST("FLOG10.S", "(@A).L, FP2", "56789A", 0xF200|071, 0x4015|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FLOG10.X", "(*+$1234,PC), FP3",   0xF200|072, 0x4015|(2<<10)|(3<<7), 0x1230);
    TEST("FLOG10.P", "(*+90,PC,A4.L), FP5", 0xF200|073, 0x4015|(3<<10)|(5<<7), 0xC856);
    TEST("FLOG10.L", "#$6789ABCD, FP6",     0xF200|074, 0x4015|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FLOG10.W", "#$1234, FP2",         0xF200|074, 0x4015|(4<<10)|(2<<7), 0x1234);
    TEST("FLOG10.B", "#$23, FP4",           0xF200|074, 0x4015|(6<<10)|(4<<7), 0x0023);
    TFLT("FLOG10.S", "#7.88999976E-10, FP7", "#$3058E0F0, FP7",
         0xF200|074, 0x4015|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    TFLT("FLOG10.X", "#-89000000032, FP0", "#$C0230000A5C681D100000000, FP0",
         0xF200|074, 0x4015|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    TFLT("FLOG10.P", "#9.12E+20, FP1",     "#$002000091200000000000000, FP1",
         0xF200|074, 0x4015|(3<<10)|(1<<7), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    TFLT("FLOG10.D", "#-8.25, FP3",        "#$C020800000000000, FP3",
         0xF200|074, 0x4015|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FLOG2.X", "FP0, FP1",            0xF200,     0x0016|(0<<10)|(1<<7));
    TEST("FLOG2.X", "FP2",                 0xF200,     0x0016|(2<<10)|(2<<7));
    TEST("FLOG2.L", "D2, FP3",             0xF200|002, 0x4016|(0<<10)|(3<<7));
    TEST("FLOG2.S", "D2, FP3",             0xF200|002, 0x4016|(1<<10)|(3<<7));
    ERRT("FLOG2.X", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                           0xF200|002, 0x4016|(2<<10)|(3<<7));
    ERRT("FLOG2.P", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                           0xF200|002, 0x4016|(3<<10)|(3<<7));
    TEST("FLOG2.W", "D2, FP3",             0xF200|002, 0x4016|(4<<10)|(3<<7));
    ERRT("FLOG2.D", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                           0xF200|002, 0x4016|(5<<10)|(3<<7));
    TEST("FLOG2.B", "D2, FP3",             0xF200|002, 0x4016|(6<<10)|(3<<7));
    UNKN(/* FLOG2.P D2, FP3 */             0xF200|002, 0x4016|(7<<10)|(3<<7));
    UNKN(/* FLOG2.S A4, FP5 */             0xF200|014, 0x4016|(1<<10)|(5<<7));
    TEST("FLOG2.X", "(A6), FP7",           0xF200|026, 0x4016|(2<<10)|(7<<7));
    TEST("FLOG2.P", "(A0)+, FP1",          0xF200|030, 0x4016|(3<<10)|(1<<7));
    TEST("FLOG2.W", "-(A2), FP3",          0xF200|042, 0x4016|(4<<10)|(3<<7));
    TEST("FLOG2.D", "($1234,A4), FP5",     0xF200|054, 0x4016|(5<<10)|(5<<7), 0x1234);
    TEST("FLOG2.B", "($23,A6,D7.W), FP0",  0xF200|066, 0x4016|(6<<10)|(0<<7), 0x7023);
    ABST("FLOG2.L", "(@A).W, FP1", "4566",   0xF200|070, 0x4016|(0<<10)|(1<<7), 0x4566);
    ABST("FLOG2.S", "(@A).L, FP2", "56789A", 0xF200|071, 0x4016|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FLOG2.X", "(*+$1234,PC), FP3",   0xF200|072, 0x4016|(2<<10)|(3<<7), 0x1230);
    TEST("FLOG2.P", "(*+90,PC,A4.L), FP5", 0xF200|073, 0x4016|(3<<10)|(5<<7), 0xC856);
    TEST("FLOG2.L", "#$6789ABCD, FP6",     0xF200|074, 0x4016|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FLOG2.W", "#$1234, FP2",         0xF200|074, 0x4016|(4<<10)|(2<<7), 0x1234);
    TEST("FLOG2.B", "#$23, FP4",           0xF200|074, 0x4016|(6<<10)|(4<<7), 0x0023);
    TFLT("FLOG2.S", "#7.88999976E-10, FP7", "#$3058E0F0, FP7",
         0xF200|074, 0x4016|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    TFLT("FLOG2.X", "#-89000000032, FP0",  "#$C0230000A5C681D100000000, FP0",
         0xF200|074, 0x4016|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    TFLT("FLOG2.P", "#9.12E+20, FP1",      "#$002000091200000000000000, FP1",
         0xF200|074, 0x4016|(3<<10)|(1<<7), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    TFLT("FLOG2.D", "#-8.25, FP3",         "#$C020800000000000, FP3",
         0xF200|074, 0x4016|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FABS.X", "FP0, FP1",            0xF200,     0x0018|(0<<10)|(1<<7));
    TEST("FABS.X", "FP2",                 0xF200,     0x0018|(2<<10)|(2<<7));
    TEST("FABS.L", "D2, FP3",             0xF200|002, 0x4018|(0<<10)|(3<<7));
    TEST("FABS.S", "D2, FP3",             0xF200|002, 0x4018|(1<<10)|(3<<7));
    ERRT("FABS.X", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                           0xF200|002, 0x4018|(2<<10)|(3<<7));
    ERRT("FABS.P", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                           0xF200|002, 0x4018|(3<<10)|(3<<7));
    TEST("FABS.W", "D2, FP3",             0xF200|002, 0x4018|(4<<10)|(3<<7));
    ERRT("FABS.D", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                           0xF200|002, 0x4018|(5<<10)|(3<<7));
    TEST("FABS.B", "D2, FP3",             0xF200|002, 0x4018|(6<<10)|(3<<7));
    UNKN(/* FABS.P D2, FP3 */             0xF200|002, 0x4018|(7<<10)|(3<<7));
    UNKN(/* FABS.S A4, FP5 */             0xF200|014, 0x4018|(1<<10)|(5<<7));
    TEST("FABS.X", "(A6), FP7",           0xF200|026, 0x4018|(2<<10)|(7<<7));
    TEST("FABS.P", "(A0)+, FP1",          0xF200|030, 0x4018|(3<<10)|(1<<7));
    TEST("FABS.W", "-(A2), FP3",          0xF200|042, 0x4018|(4<<10)|(3<<7));
    TEST("FABS.D", "($1234,A4), FP5",     0xF200|054, 0x4018|(5<<10)|(5<<7), 0x1234);
    TEST("FABS.B", "($23,A6,D7.W), FP0",  0xF200|066, 0x4018|(6<<10)|(0<<7), 0x7023);
    ABST("FABS.L", "(@A).W, FP1", "4566",   0xF200|070, 0x4018|(0<<10)|(1<<7), 0x4566);
    ABST("FABS.S", "(@A).L, FP2", "56789A", 0xF200|071, 0x4018|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FABS.X", "(*+$1234,PC), FP3",   0xF200|072, 0x4018|(2<<10)|(3<<7), 0x1230);
    TEST("FABS.P", "(*+90,PC,A4.L), FP5", 0xF200|073, 0x4018|(3<<10)|(5<<7), 0xC856);
    TEST("FABS.L", "#$6789ABCD, FP6",     0xF200|074, 0x4018|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FABS.W", "#$1234, FP2",         0xF200|074, 0x4018|(4<<10)|(2<<7), 0x1234);
    TEST("FABS.B", "#$23, FP4",           0xF200|074, 0x4018|(6<<10)|(4<<7), 0x0023);
    TFLT("FABS.S", "#7.88999976E-10, FP7", "#$3058E0F0, FP7",
         0xF200|074, 0x4018|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    TFLT("FABS.X", "#-89000000032, FP0",   "#$C0230000A5C681D100000000, FP0",
         0xF200|074, 0x4018|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    TFLT("FABS.P", "#9.12E+20, FP1",       "#$002000091200000000000000, FP1",
         0xF200|074, 0x4018|(3<<10)|(1<<7), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    TFLT("FABS.D", "#-8.25, FP3",          "#$C020800000000000, FP3",
         0xF200|074, 0x4018|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FCOSH.X", "FP0, FP1",            0xF200,     0x0019|(0<<10)|(1<<7));
    TEST("FCOSH.X", "FP2",                 0xF200,     0x0019|(2<<10)|(2<<7));
    TEST("FCOSH.L", "D2, FP3",             0xF200|002, 0x4019|(0<<10)|(3<<7));
    TEST("FCOSH.S", "D2, FP3",             0xF200|002, 0x4019|(1<<10)|(3<<7));
    ERRT("FCOSH.X", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                           0xF200|002, 0x4019|(2<<10)|(3<<7));
    ERRT("FCOSH.P", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                           0xF200|002, 0x4019|(3<<10)|(3<<7));
    TEST("FCOSH.W", "D2, FP3",             0xF200|002, 0x4019|(4<<10)|(3<<7));
    ERRT("FCOSH.D", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                           0xF200|002, 0x4019|(5<<10)|(3<<7));
    TEST("FCOSH.B", "D2, FP3",             0xF200|002, 0x4019|(6<<10)|(3<<7));
    UNKN(/* FCOSH.P D2, FP3 */             0xF200|002, 0x4019|(7<<10)|(3<<7));
    UNKN(/* FCOSH.S A4, FP5 */             0xF200|014, 0x4019|(1<<10)|(5<<7));
    TEST("FCOSH.X", "(A6), FP7",           0xF200|026, 0x4019|(2<<10)|(7<<7));
    TEST("FCOSH.P", "(A0)+, FP1",          0xF200|030, 0x4019|(3<<10)|(1<<7));
    TEST("FCOSH.W", "-(A2), FP3",          0xF200|042, 0x4019|(4<<10)|(3<<7));
    TEST("FCOSH.D", "($1234,A4), FP5",     0xF200|054, 0x4019|(5<<10)|(5<<7), 0x1234);
    TEST("FCOSH.B", "($23,A6,D7.W), FP0",  0xF200|066, 0x4019|(6<<10)|(0<<7), 0x7023);
    ABST("FCOSH.L", "(@A).W, FP1", "4566",   0xF200|070, 0x4019|(0<<10)|(1<<7), 0x4566);
    ABST("FCOSH.S", "(@A).L, FP2", "56789A", 0xF200|071, 0x4019|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FCOSH.X", "(*+$1234,PC), FP3",   0xF200|072, 0x4019|(2<<10)|(3<<7), 0x1230);
    TEST("FCOSH.P", "(*+90,PC,A4.L), FP5", 0xF200|073, 0x4019|(3<<10)|(5<<7), 0xC856);
    TEST("FCOSH.L", "#$6789ABCD, FP6",     0xF200|074, 0x4019|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FCOSH.W", "#$1234, FP2",         0xF200|074, 0x4019|(4<<10)|(2<<7), 0x1234);
    TEST("FCOSH.B", "#$23, FP4",           0xF200|074, 0x4019|(6<<10)|(4<<7), 0x0023);
    TFLT("FCOSH.S", "#7.88999976E-10, FP7", "#$3058E0F0, FP7",
         0xF200|074, 0x4019|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    TFLT("FCOSH.X", "#-89000000032, FP0",  "#$C0230000A5C681D100000000, FP0",
         0xF200|074, 0x4019|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    TFLT("FCOSH.P", "#9.12E+20, FP1",      "#$002000091200000000000000, FP1",
         0xF200|074, 0x4019|(3<<10)|(1<<7), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    TFLT("FCOSH.D", "#-8.25, FP3",         "#$C020800000000000, FP3",
         0xF200|074, 0x4019|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FNEG.X", "FP0, FP1",            0xF200,     0x001A|(0<<10)|(1<<7));
    TEST("FNEG.X", "FP2",                 0xF200,     0x001A|(2<<10)|(2<<7));
    TEST("FNEG.L", "D2, FP3",             0xF200|002, 0x401A|(0<<10)|(3<<7));
    TEST("FNEG.S", "D2, FP3",             0xF200|002, 0x401A|(1<<10)|(3<<7));
    ERRT("FNEG.X", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                          0xF200|002, 0x401A|(2<<10)|(3<<7));
    ERRT("FNEG.P", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                          0xF200|002, 0x401A|(3<<10)|(3<<7));
    TEST("FNEG.W", "D2, FP3",             0xF200|002, 0x401A|(4<<10)|(3<<7));
    ERRT("FNEG.D", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                          0xF200|002, 0x401A|(5<<10)|(3<<7));
    TEST("FNEG.B", "D2, FP3",             0xF200|002, 0x401A|(6<<10)|(3<<7));
    UNKN(/* FNEG.P D2, FP3 */             0xF200|002, 0x401A|(7<<10)|(3<<7));
    UNKN(/* FNEG.S A4, FP5 */             0xF200|014, 0x401A|(1<<10)|(5<<7));
    TEST("FNEG.X", "(A6), FP7",           0xF200|026, 0x401A|(2<<10)|(7<<7));
    TEST("FNEG.P", "(A0)+, FP1",          0xF200|030, 0x401A|(3<<10)|(1<<7));
    TEST("FNEG.W", "-(A2), FP3",          0xF200|042, 0x401A|(4<<10)|(3<<7));
    TEST("FNEG.D", "($1234,A4), FP5",     0xF200|054, 0x401A|(5<<10)|(5<<7), 0x1234);
    TEST("FNEG.B", "($23,A6,D7.W), FP0",  0xF200|066, 0x401A|(6<<10)|(0<<7), 0x7023);
    ABST("FNEG.L", "(@A).W, FP1", "4566",   0xF200|070, 0x401A|(0<<10)|(1<<7), 0x4566);
    ABST("FNEG.S", "(@A).L, FP2", "56789A", 0xF200|071, 0x401A|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FNEG.X", "(*+$1234,PC), FP3",   0xF200|072, 0x401A|(2<<10)|(3<<7), 0x1230);
    TEST("FNEG.P", "(*+90,PC,A4.L), FP5", 0xF200|073, 0x401A|(3<<10)|(5<<7), 0xC856);
    TEST("FNEG.L", "#$6789ABCD, FP6",     0xF200|074, 0x401A|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FNEG.W", "#$1234, FP2",         0xF200|074, 0x401A|(4<<10)|(2<<7), 0x1234);
    TEST("FNEG.B", "#$23, FP4",           0xF200|074, 0x401A|(6<<10)|(4<<7), 0x0023);
    TFLT("FNEG.S", "#7.88999976E-10, FP7", "#$3058E0F0, FP7",
         0xF200|074, 0x401A|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    TFLT("FNEG.X", "#-89000000032, FP0",   "#$C0230000A5C681D100000000, FP0",
         0xF200|074, 0x401A|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    TFLT("FNEG.P", "#9.12E+20, FP1",       "#$002000091200000000000000, FP1",
         0xF200|074, 0x401A|(3<<10)|(1<<7), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    TFLT("FNEG.D", "#-8.25, FP3",          "#$C020800000000000, FP3",
         0xF200|074, 0x401A|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FACOS.X", "FP0, FP1",            0xF200,     0x001C|(0<<10)|(1<<7));
    TEST("FACOS.X", "FP2",                 0xF200,     0x001C|(2<<10)|(2<<7));
    TEST("FACOS.L", "D2, FP3",             0xF200|002, 0x401C|(0<<10)|(3<<7));
    TEST("FACOS.S", "D2, FP3",             0xF200|002, 0x401C|(1<<10)|(3<<7));
    ERRT("FACOS.X", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                           0xF200|002, 0x401C|(2<<10)|(3<<7));
    ERRT("FACOS.P", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                           0xF200|002, 0x401C|(3<<10)|(3<<7));
    TEST("FACOS.W", "D2, FP3",             0xF200|002, 0x401C|(4<<10)|(3<<7));
    ERRT("FACOS.D", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                           0xF200|002, 0x401C|(5<<10)|(3<<7));
    TEST("FACOS.B", "D2, FP3",             0xF200|002, 0x401C|(6<<10)|(3<<7));
    UNKN(/* FACOS.P D2, FP3 */             0xF200|002, 0x401C|(7<<10)|(3<<7));
    UNKN(/* FACOS.S A4, FP5 */             0xF200|014, 0x401C|(1<<10)|(5<<7));
    TEST("FACOS.X", "(A6), FP7",           0xF200|026, 0x401C|(2<<10)|(7<<7));
    TEST("FACOS.P", "(A0)+, FP1",          0xF200|030, 0x401C|(3<<10)|(1<<7));
    TEST("FACOS.W", "-(A2), FP3",          0xF200|042, 0x401C|(4<<10)|(3<<7));
    TEST("FACOS.D", "($1234,A4), FP5",     0xF200|054, 0x401C|(5<<10)|(5<<7), 0x1234);
    TEST("FACOS.B", "($23,A6,D7.W), FP0",  0xF200|066, 0x401C|(6<<10)|(0<<7), 0x7023);
    ABST("FACOS.L", "(@A).W, FP1", "4566",   0xF200|070, 0x401C|(0<<10)|(1<<7), 0x4566);
    ABST("FACOS.S", "(@A).L, FP2", "56789A", 0xF200|071, 0x401C|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FACOS.X", "(*+$1234,PC), FP3",   0xF200|072, 0x401C|(2<<10)|(3<<7), 0x1230);
    TEST("FACOS.P", "(*+90,PC,A4.L), FP5", 0xF200|073, 0x401C|(3<<10)|(5<<7), 0xC856);
    TEST("FACOS.L", "#$6789ABCD, FP6",     0xF200|074, 0x401C|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FACOS.W", "#$1234, FP2",         0xF200|074, 0x401C|(4<<10)|(2<<7), 0x1234);
    TEST("FACOS.B", "#$23, FP4",           0xF200|074, 0x401C|(6<<10)|(4<<7), 0x0023);
    TFLT("FACOS.S", "#7.88999976E-10, FP7", "#$3058E0F0, FP7",
         0xF200|074, 0x401C|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    TFLT("FACOS.X", "#-89000000032, FP0",  "#$C0230000A5C681D100000000, FP0",
         0xF200|074, 0x401C|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    TFLT("FACOS.P", "#9.12E+20, FP1",      "#$002000091200000000000000, FP1",
         0xF200|074, 0x401C|(3<<10)|(1<<7), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    TFLT("FACOS.D", "#-8.25, FP3",         "#$C020800000000000, FP3",
         0xF200|074, 0x401C|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FCOS.X", "FP0, FP1",            0xF200,     0x001D|(0<<10)|(1<<7));
    TEST("FCOS.X", "FP2",                 0xF200,     0x001D|(2<<10)|(2<<7));
    TEST("FCOS.L", "D2, FP3",             0xF200|002, 0x401D|(0<<10)|(3<<7));
    TEST("FCOS.S", "D2, FP3",             0xF200|002, 0x401D|(1<<10)|(3<<7));
    ERRT("FCOS.X", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                          0xF200|002, 0x401D|(2<<10)|(3<<7));
    ERRT("FCOS.P", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                          0xF200|002, 0x401D|(3<<10)|(3<<7));
    TEST("FCOS.W", "D2, FP3",             0xF200|002, 0x401D|(4<<10)|(3<<7));
    ERRT("FCOS.D", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                          0xF200|002, 0x401D|(5<<10)|(3<<7));
    TEST("FCOS.B", "D2, FP3",             0xF200|002, 0x401D|(6<<10)|(3<<7));
    UNKN(/* FCOS.P D2, FP3 */             0xF200|002, 0x401D|(7<<10)|(3<<7));
    UNKN(/* FCOS.S A4, FP5 */             0xF200|014, 0x401D|(1<<10)|(5<<7));
    TEST("FCOS.X", "(A6), FP7",           0xF200|026, 0x401D|(2<<10)|(7<<7));
    TEST("FCOS.P", "(A0)+, FP1",          0xF200|030, 0x401D|(3<<10)|(1<<7));
    TEST("FCOS.W", "-(A2), FP3",          0xF200|042, 0x401D|(4<<10)|(3<<7));
    TEST("FCOS.D", "($1234,A4), FP5",     0xF200|054, 0x401D|(5<<10)|(5<<7), 0x1234);
    TEST("FCOS.B", "($23,A6,D7.W), FP0",  0xF200|066, 0x401D|(6<<10)|(0<<7), 0x7023);
    ABST("FCOS.L", "(@A).W, FP1", "4566",   0xF200|070, 0x401D|(0<<10)|(1<<7), 0x4566);
    ABST("FCOS.S", "(@A).L, FP2", "56789A", 0xF200|071, 0x401D|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FCOS.X", "(*+$1234,PC), FP3",   0xF200|072, 0x401D|(2<<10)|(3<<7), 0x1230);
    TEST("FCOS.P", "(*+90,PC,A4.L), FP5", 0xF200|073, 0x401D|(3<<10)|(5<<7), 0xC856);
    TEST("FCOS.L", "#$6789ABCD, FP6",     0xF200|074, 0x401D|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FCOS.W", "#$1234, FP2",         0xF200|074, 0x401D|(4<<10)|(2<<7), 0x1234);
    TEST("FCOS.B", "#$23, FP4",           0xF200|074, 0x401D|(6<<10)|(4<<7), 0x0023);
    TFLT("FCOS.S", "#7.88999976E-10, FP7", "#$3058E0F0, FP7",
         0xF200|074, 0x401D|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    TFLT("FCOS.X", "#-89000000032, FP0",   "#$C0230000A5C681D100000000, FP0",
         0xF200|074, 0x401D|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    TFLT("FCOS.P", "#9.12E+20, FP1",       "#$002000091200000000000000, FP1",
         0xF200|074, 0x401D|(3<<10)|(1<<7), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    TFLT("FCOS.D", "#-8.25, FP3",          "#$C020800000000000, FP3",
         0xF200|074, 0x401D|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FGETEXP.X", "FP0, FP1",            0xF200,     0x001E|(0<<10)|(1<<7));
    TEST("FGETEXP.X", "FP2",                 0xF200,     0x001E|(2<<10)|(2<<7));
    TEST("FGETEXP.L", "D2, FP3",             0xF200|002, 0x401E|(0<<10)|(3<<7));
    TEST("FGETEXP.S", "D2, FP3",             0xF200|002, 0x401E|(1<<10)|(3<<7));
    ERRT("FGETEXP.X", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                             0xF200|002, 0x401E|(2<<10)|(3<<7));
    ERRT("FGETEXP.P", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                             0xF200|002, 0x401E|(3<<10)|(3<<7));
    TEST("FGETEXP.W", "D2, FP3",             0xF200|002, 0x401E|(4<<10)|(3<<7));
    ERRT("FGETEXP.D", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                             0xF200|002, 0x401E|(5<<10)|(3<<7));
    TEST("FGETEXP.B", "D2, FP3",             0xF200|002, 0x401E|(6<<10)|(3<<7));
    UNKN(/* FGETEXP.P D2, FP3 */             0xF200|002, 0x401E|(7<<10)|(3<<7));
    UNKN(/* FGETEXP.S A4, FP5 */             0xF200|014, 0x401E|(1<<10)|(5<<7));
    TEST("FGETEXP.X", "(A6), FP7",           0xF200|026, 0x401E|(2<<10)|(7<<7));
    TEST("FGETEXP.P", "(A0)+, FP1",          0xF200|030, 0x401E|(3<<10)|(1<<7));
    TEST("FGETEXP.W", "-(A2), FP3",          0xF200|042, 0x401E|(4<<10)|(3<<7));
    TEST("FGETEXP.D", "($1234,A4), FP5",     0xF200|054, 0x401E|(5<<10)|(5<<7), 0x1234);
    TEST("FGETEXP.B", "($23,A6,D7.W), FP0",  0xF200|066, 0x401E|(6<<10)|(0<<7), 0x7023);
    ABST("FGETEXP.L", "(@A).W, FP1", "4566",   0xF200|070, 0x401E|(0<<10)|(1<<7), 0x4566);
    ABST("FGETEXP.S", "(@A).L, FP2", "56789A", 0xF200|071, 0x401E|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FGETEXP.X", "(*+$1234,PC), FP3",   0xF200|072, 0x401E|(2<<10)|(3<<7), 0x1230);
    TEST("FGETEXP.P", "(*+90,PC,A4.L), FP5", 0xF200|073, 0x401E|(3<<10)|(5<<7), 0xC856);
    TEST("FGETEXP.L", "#$6789ABCD, FP6",     0xF200|074, 0x401E|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FGETEXP.W", "#$1234, FP2",         0xF200|074, 0x401E|(4<<10)|(2<<7), 0x1234);
    TEST("FGETEXP.B", "#$23, FP4",           0xF200|074, 0x401E|(6<<10)|(4<<7), 0x0023);
    TFLT("FGETEXP.S", "#7.88999976E-10, FP7", "#$3058E0F0, FP7",
         0xF200|074, 0x401E|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    TFLT("FGETEXP.X", "#-89000000032, FP0", "#$C0230000A5C681D100000000, FP0",
         0xF200|074, 0x401E|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    TFLT("FGETEXP.P", "#9.12E+20, FP1",    "#$002000091200000000000000, FP1",
         0xF200|074, 0x401E|(3<<10)|(1<<7), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    TFLT("FGETEXP.D", "#-8.25, FP3",       "#$C020800000000000, FP3",
         0xF200|074, 0x401E|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FGETMAN.X", "FP0, FP1",            0xF200,     0x001F|(0<<10)|(1<<7));
    TEST("FGETMAN.X", "FP2",                 0xF200,     0x001F|(2<<10)|(2<<7));
    TEST("FGETMAN.L", "D2, FP3",             0xF200|002, 0x401F|(0<<10)|(3<<7));
    TEST("FGETMAN.S", "D2, FP3",             0xF200|002, 0x401F|(1<<10)|(3<<7));
    ERRT("FGETMAN.X", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                             0xF200|002, 0x401F|(2<<10)|(3<<7));
    ERRT("FGETMAN.P", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                             0xF200|002, 0x401F|(3<<10)|(3<<7));
    TEST("FGETMAN.W", "D2, FP3",             0xF200|002, 0x401F|(4<<10)|(3<<7));
    ERRT("FGETMAN.D", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                             0xF200|002, 0x401F|(5<<10)|(3<<7));
    TEST("FGETMAN.B", "D2, FP3",             0xF200|002, 0x401F|(6<<10)|(3<<7));
    UNKN(/* FGETMAN.P D2, FP3 */             0xF200|002, 0x401F|(7<<10)|(3<<7));
    UNKN(/* FGETMAN.S A4, FP5 */             0xF200|014, 0x401F|(1<<10)|(5<<7));
    TEST("FGETMAN.X", "(A6), FP7",           0xF200|026, 0x401F|(2<<10)|(7<<7));
    TEST("FGETMAN.P", "(A0)+, FP1",          0xF200|030, 0x401F|(3<<10)|(1<<7));
    TEST("FGETMAN.W", "-(A2), FP3",          0xF200|042, 0x401F|(4<<10)|(3<<7));
    TEST("FGETMAN.D", "($1234,A4), FP5",     0xF200|054, 0x401F|(5<<10)|(5<<7), 0x1234);
    TEST("FGETMAN.B", "($23,A6,D7.W), FP0",  0xF200|066, 0x401F|(6<<10)|(0<<7), 0x7023);
    ABST("FGETMAN.L", "(@A).W, FP1", "4566",   0xF200|070, 0x401F|(0<<10)|(1<<7), 0x4566);
    ABST("FGETMAN.S", "(@A).L, FP2", "56789A", 0xF200|071, 0x401F|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FGETMAN.X", "(*+$1234,PC), FP3",   0xF200|072, 0x401F|(2<<10)|(3<<7), 0x1230);
    TEST("FGETMAN.P", "(*+90,PC,A4.L), FP5", 0xF200|073, 0x401F|(3<<10)|(5<<7), 0xC856);
    TEST("FGETMAN.L", "#$6789ABCD, FP6",     0xF200|074, 0x401F|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FGETMAN.W", "#$1234, FP2",         0xF200|074, 0x401F|(4<<10)|(2<<7), 0x1234);
    TEST("FGETMAN.B", "#$23, FP4",           0xF200|074, 0x401F|(6<<10)|(4<<7), 0x0023);
    TFLT("FGETMAN.S", "#7.88999976E-10, FP7", "#$3058E0F0, FP7",
         0xF200|074, 0x401F|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    TFLT("FGETMAN.X", "#-89000000032, FP0", "#$C0230000A5C681D100000000, FP0",
         0xF200|074, 0x401F|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    TFLT("FGETMAN.P", "#9.12E+20, FP1",    "#$002000091200000000000000, FP1",
         0xF200|074, 0x401F|(3<<10)|(1<<7), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    TFLT("FGETMAN.D", "#-8.25, FP3",       "#$C020800000000000, FP3",
         0xF200|074, 0x401F|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FDIV.X", "FP0, FP1",            0xF200,     0x0020|(0<<10)|(1<<7));
    TEST("FDIV.X", "FP2, FP2",            0xF200,     0x0020|(2<<10)|(2<<7));
    TEST("FDIV.L", "D2, FP3",             0xF200|002, 0x4020|(0<<10)|(3<<7));
    TEST("FDIV.S", "D2, FP3",             0xF200|002, 0x4020|(1<<10)|(3<<7));
    ERRT("FDIV.X", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                          0xF200|002, 0x4020|(2<<10)|(3<<7));
    ERRT("FDIV.P", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                          0xF200|002, 0x4020|(3<<10)|(3<<7));
    TEST("FDIV.W", "D2, FP3",             0xF200|002, 0x4020|(4<<10)|(3<<7));
    ERRT("FDIV.D", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                          0xF200|002, 0x4020|(5<<10)|(3<<7));
    TEST("FDIV.B", "D2, FP3",             0xF200|002, 0x4020|(6<<10)|(3<<7));
    UNKN(/* FDIV.P D2, FP3 */             0xF200|002, 0x4020|(7<<10)|(3<<7));
    UNKN(/* FDIV.S A4, FP5 */             0xF200|014, 0x4020|(1<<10)|(5<<7));
    TEST("FDIV.X", "(A6), FP7",           0xF200|026, 0x4020|(2<<10)|(7<<7));
    TEST("FDIV.P", "(A0)+, FP1",          0xF200|030, 0x4020|(3<<10)|(1<<7));
    TEST("FDIV.W", "-(A2), FP3",          0xF200|042, 0x4020|(4<<10)|(3<<7));
    TEST("FDIV.D", "($1234,A4), FP5",     0xF200|054, 0x4020|(5<<10)|(5<<7), 0x1234);
    TEST("FDIV.B", "($23,A6,D7.W), FP0",  0xF200|066, 0x4020|(6<<10)|(0<<7), 0x7023);
    ABST("FDIV.L", "(@A).W, FP1", "4566",   0xF200|070, 0x4020|(0<<10)|(1<<7), 0x4566);
    ABST("FDIV.S", "(@A).L, FP2", "56789A", 0xF200|071, 0x4020|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FDIV.X", "(*+$1234,PC), FP3",   0xF200|072, 0x4020|(2<<10)|(3<<7), 0x1230);
    TEST("FDIV.P", "(*+90,PC,A4.L), FP5", 0xF200|073, 0x4020|(3<<10)|(5<<7), 0xC856);
    TEST("FDIV.L", "#$6789ABCD, FP6",     0xF200|074, 0x4020|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FDIV.W", "#$1234, FP2",         0xF200|074, 0x4020|(4<<10)|(2<<7), 0x1234);
    TEST("FDIV.B", "#$23, FP4",           0xF200|074, 0x4020|(6<<10)|(4<<7), 0x0023);
    TFLT("FDIV.S", "#7.88999976E-10, FP7", "#$3058E0F0, FP7",
         0xF200|074, 0x4020|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    TFLT("FDIV.X", "#-89000000032, FP0",   "#$C0230000A5C681D100000000, FP0",
         0xF200|074, 0x4020|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    TFLT("FDIV.P", "#9.12E+20, FP1",       "#$002000091200000000000000, FP1",
         0xF200|074, 0x4020|(3<<10)|(1<<7), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    TFLT("FDIV.D", "#-8.25, FP3",          "#$C020800000000000, FP3",
         0xF200|074, 0x4020|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FMOD.X", "FP0, FP1",            0xF200,     0x0021|(0<<10)|(1<<7));
    TEST("FMOD.X", "FP2, FP2",            0xF200,     0x0021|(2<<10)|(2<<7));
    TEST("FMOD.L", "D2, FP3",             0xF200|002, 0x4021|(0<<10)|(3<<7));
    TEST("FMOD.S", "D2, FP3",             0xF200|002, 0x4021|(1<<10)|(3<<7));
    ERRT("FMOD.X", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                          0xF200|002, 0x4021|(2<<10)|(3<<7));
    ERRT("FMOD.P", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                          0xF200|002, 0x4021|(3<<10)|(3<<7));
    TEST("FMOD.W", "D2, FP3",             0xF200|002, 0x4021|(4<<10)|(3<<7));
    ERRT("FMOD.D", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                          0xF200|002, 0x4021|(5<<10)|(3<<7));
    TEST("FMOD.B", "D2, FP3",             0xF200|002, 0x4021|(6<<10)|(3<<7));
    UNKN(/* FMOD.P D2, FP3 */             0xF200|002, 0x4021|(7<<10)|(3<<7));
    UNKN(/* FMOD.S A4, FP5 */             0xF200|014, 0x4021|(1<<10)|(5<<7));
    TEST("FMOD.X", "(A6), FP7",           0xF200|026, 0x4021|(2<<10)|(7<<7));
    TEST("FMOD.P", "(A0)+, FP1",          0xF200|030, 0x4021|(3<<10)|(1<<7));
    TEST("FMOD.W", "-(A2), FP3",          0xF200|042, 0x4021|(4<<10)|(3<<7));
    TEST("FMOD.D", "($1234,A4), FP5",     0xF200|054, 0x4021|(5<<10)|(5<<7), 0x1234);
    TEST("FMOD.B", "($23,A6,D7.W), FP0",  0xF200|066, 0x4021|(6<<10)|(0<<7), 0x7023);
    ABST("FMOD.L", "(@A).W, FP1", "4566",   0xF200|070, 0x4021|(0<<10)|(1<<7), 0x4566);
    ABST("FMOD.S", "(@A).L, FP2", "56789A", 0xF200|071, 0x4021|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FMOD.X", "(*+$1234,PC), FP3",   0xF200|072, 0x4021|(2<<10)|(3<<7), 0x1230);
    TEST("FMOD.P", "(*+90,PC,A4.L), FP5", 0xF200|073, 0x4021|(3<<10)|(5<<7), 0xC856);
    TEST("FMOD.L", "#$6789ABCD, FP6",     0xF200|074, 0x4021|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FMOD.W", "#$1234, FP2",         0xF200|074, 0x4021|(4<<10)|(2<<7), 0x1234);
    TEST("FMOD.B", "#$23, FP4",           0xF200|074, 0x4021|(6<<10)|(4<<7), 0x0023);
    TFLT("FMOD.S", "#7.88999976E-10, FP7", "#$3058E0F0, FP7",
         0xF200|074, 0x4021|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    TFLT("FMOD.X", "#-89000000032, FP0",   "#$C0230000A5C681D100000000, FP0",
         0xF200|074, 0x4021|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    TFLT("FMOD.P", "#9.12E+20, FP1",       "#$002000091200000000000000, FP1",
         0xF200|074, 0x4021|(3<<10)|(1<<7), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    TFLT("FMOD.D", "#-8.25, FP3",          "#$C020800000000000, FP3",
         0xF200|074, 0x4021|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FADD.X", "FP0, FP1",            0xF200,     0x0022|(0<<10)|(1<<7));
    TEST("FADD.X", "FP2, FP2",            0xF200,     0x0022|(2<<10)|(2<<7));
    TEST("FADD.L", "D2, FP3",             0xF200|002, 0x4022|(0<<10)|(3<<7));
    TEST("FADD.S", "D2, FP3",             0xF200|002, 0x4022|(1<<10)|(3<<7));
    ERRT("FADD.X", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                          0xF200|002, 0x4022|(2<<10)|(3<<7));
    ERRT("FADD.P", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                          0xF200|002, 0x4022|(3<<10)|(3<<7));
    TEST("FADD.W", "D2, FP3",             0xF200|002, 0x4022|(4<<10)|(3<<7));
    ERRT("FADD.D", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                          0xF200|002, 0x4022|(5<<10)|(3<<7));
    TEST("FADD.B", "D2, FP3",             0xF200|002, 0x4022|(6<<10)|(3<<7));
    UNKN(/* FADD.P D2, FP3 */             0xF200|002, 0x4022|(7<<10)|(3<<7));
    UNKN(/* FADD.S A4, FP5 */             0xF200|014, 0x4022|(1<<10)|(5<<7));
    TEST("FADD.X", "(A6), FP7",           0xF200|026, 0x4022|(2<<10)|(7<<7));
    TEST("FADD.P", "(A0)+, FP1",          0xF200|030, 0x4022|(3<<10)|(1<<7));
    TEST("FADD.W", "-(A2), FP3",          0xF200|042, 0x4022|(4<<10)|(3<<7));
    TEST("FADD.D", "($1234,A4), FP5",     0xF200|054, 0x4022|(5<<10)|(5<<7), 0x1234);
    TEST("FADD.B", "($23,A6,D7.W), FP0",  0xF200|066, 0x4022|(6<<10)|(0<<7), 0x7023);
    ABST("FADD.L", "(@A).W, FP1", "4566",   0xF200|070, 0x4022|(0<<10)|(1<<7), 0x4566);
    ABST("FADD.S", "(@A).L, FP2", "56789A", 0xF200|071, 0x4022|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FADD.X", "(*+$1234,PC), FP3",   0xF200|072, 0x4022|(2<<10)|(3<<7), 0x1230);
    TEST("FADD.P", "(*+90,PC,A4.L), FP5", 0xF200|073, 0x4022|(3<<10)|(5<<7), 0xC856);
    TEST("FADD.L", "#$6789ABCD, FP6",     0xF200|074, 0x4022|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FADD.W", "#$1234, FP2",         0xF200|074, 0x4022|(4<<10)|(2<<7), 0x1234);
    TEST("FADD.B", "#$23, FP4",           0xF200|074, 0x4022|(6<<10)|(4<<7), 0x0023);
    TFLT("FADD.S", "#7.88999976E-10, FP7", "#$3058E0F0, FP7",
         0xF200|074, 0x4022|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    TFLT("FADD.X", "#-89000000032, FP0",   "#$C0230000A5C681D100000000, FP0",
         0xF200|074, 0x4022|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    TFLT("FADD.P", "#9.12E+20, FP1",       "#$002000091200000000000000, FP1",
         0xF200|074, 0x4022|(3<<10)|(1<<7), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    TFLT("FADD.D", "#-8.25, FP3",          "#$C020800000000000, FP3",
         0xF200|074, 0x4022|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FMUL.X", "FP0, FP1",            0xF200,     0x0023|(0<<10)|(1<<7));
    TEST("FMUL.X", "FP2, FP2",            0xF200,     0x0023|(2<<10)|(2<<7));
    TEST("FMUL.L", "D2, FP3",             0xF200|002, 0x4023|(0<<10)|(3<<7));
    TEST("FMUL.S", "D2, FP3",             0xF200|002, 0x4023|(1<<10)|(3<<7));
    ERRT("FMUL.X", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                          0xF200|002, 0x4023|(2<<10)|(3<<7));
    ERRT("FMUL.P", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                          0xF200|002, 0x4023|(3<<10)|(3<<7));
    TEST("FMUL.W", "D2, FP3",             0xF200|002, 0x4023|(4<<10)|(3<<7));
    ERRT("FMUL.D", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                          0xF200|002, 0x4023|(5<<10)|(3<<7));
    TEST("FMUL.B", "D2, FP3",             0xF200|002, 0x4023|(6<<10)|(3<<7));
    UNKN(/* FMUL.P D2, FP3 */             0xF200|002, 0x4023|(7<<10)|(3<<7));
    UNKN(/* FMUL.S A4, FP5 */             0xF200|014, 0x4023|(1<<10)|(5<<7));
    TEST("FMUL.X", "(A6), FP7",           0xF200|026, 0x4023|(2<<10)|(7<<7));
    TEST("FMUL.P", "(A0)+, FP1",          0xF200|030, 0x4023|(3<<10)|(1<<7));
    TEST("FMUL.W", "-(A2), FP3",          0xF200|042, 0x4023|(4<<10)|(3<<7));
    TEST("FMUL.D", "($1234,A4), FP5",     0xF200|054, 0x4023|(5<<10)|(5<<7), 0x1234);
    TEST("FMUL.B", "($23,A6,D7.W), FP0",  0xF200|066, 0x4023|(6<<10)|(0<<7), 0x7023);
    ABST("FMUL.L", "(@A).W, FP1", "4566",   0xF200|070, 0x4023|(0<<10)|(1<<7), 0x4566);
    ABST("FMUL.S", "(@A).L, FP2", "56789A", 0xF200|071, 0x4023|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FMUL.X", "(*+$1234,PC), FP3",   0xF200|072, 0x4023|(2<<10)|(3<<7), 0x1230);
    TEST("FMUL.P", "(*+90,PC,A4.L), FP5", 0xF200|073, 0x4023|(3<<10)|(5<<7), 0xC856);
    TEST("FMUL.L", "#$6789ABCD, FP6",     0xF200|074, 0x4023|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FMUL.W", "#$1234, FP2",         0xF200|074, 0x4023|(4<<10)|(2<<7), 0x1234);
    TEST("FMUL.B", "#$23, FP4",           0xF200|074, 0x4023|(6<<10)|(4<<7), 0x0023);
    TFLT("FMUL.S", "#7.88999976E-10, FP7", "#$3058E0F0, FP7",
         0xF200|074, 0x4023|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    TFLT("FMUL.X", "#-89000000032, FP0",   "#$C0230000A5C681D100000000, FP0",
         0xF200|074, 0x4023|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    TFLT("FMUL.P", "#9.12E+20, FP1",       "#$002000091200000000000000, FP1",
         0xF200|074, 0x4023|(3<<10)|(1<<7), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    TFLT("FMUL.D", "#-8.25, FP3",          "#$C020800000000000, FP3",
         0xF200|074, 0x4023|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FSGLDIV.X", "FP0, FP1",            0xF200,     0x0024|(0<<10)|(1<<7));
    TEST("FSGLDIV.X", "FP2, FP2",            0xF200,     0x0024|(2<<10)|(2<<7));
    TEST("FSGLDIV.L", "D2, FP3",             0xF200|002, 0x4024|(0<<10)|(3<<7));
    TEST("FSGLDIV.S", "D2, FP3",             0xF200|002, 0x4024|(1<<10)|(3<<7));
    ERRT("FSGLDIV.X", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                             0xF200|002, 0x4024|(2<<10)|(3<<7));
    ERRT("FSGLDIV.P", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                             0xF200|002, 0x4024|(3<<10)|(3<<7));
    TEST("FSGLDIV.W", "D2, FP3",             0xF200|002, 0x4024|(4<<10)|(3<<7));
    ERRT("FSGLDIV.D", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                             0xF200|002, 0x4024|(5<<10)|(3<<7));
    TEST("FSGLDIV.B", "D2, FP3",             0xF200|002, 0x4024|(6<<10)|(3<<7));
    UNKN(/* FSGLDIV.P D2, FP3 */             0xF200|002, 0x4024|(7<<10)|(3<<7));
    UNKN(/* FSGLDIV.S A4, FP5 */             0xF200|014, 0x4024|(1<<10)|(5<<7));
    TEST("FSGLDIV.X", "(A6), FP7",           0xF200|026, 0x4024|(2<<10)|(7<<7));
    TEST("FSGLDIV.P", "(A0)+, FP1",          0xF200|030, 0x4024|(3<<10)|(1<<7));
    TEST("FSGLDIV.W", "-(A2), FP3",          0xF200|042, 0x4024|(4<<10)|(3<<7));
    TEST("FSGLDIV.D", "($1234,A4), FP5",     0xF200|054, 0x4024|(5<<10)|(5<<7), 0x1234);
    TEST("FSGLDIV.B", "($23,A6,D7.W), FP0",  0xF200|066, 0x4024|(6<<10)|(0<<7), 0x7023);
    ABST("FSGLDIV.L", "(@A).W, FP1", "4566",   0xF200|070, 0x4024|(0<<10)|(1<<7), 0x4566);
    ABST("FSGLDIV.S", "(@A).L, FP2", "56789A", 0xF200|071, 0x4024|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FSGLDIV.X", "(*+$1234,PC), FP3",   0xF200|072, 0x4024|(2<<10)|(3<<7), 0x1230);
    TEST("FSGLDIV.P", "(*+90,PC,A4.L), FP5", 0xF200|073, 0x4024|(3<<10)|(5<<7), 0xC856);
    TEST("FSGLDIV.L", "#$6789ABCD, FP6",     0xF200|074, 0x4024|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FSGLDIV.W", "#$1234, FP2",         0xF200|074, 0x4024|(4<<10)|(2<<7), 0x1234);
    TEST("FSGLDIV.B", "#$23, FP4",           0xF200|074, 0x4024|(6<<10)|(4<<7), 0x0023);
    TFLT("FSGLDIV.S", "#7.88999976E-10, FP7", "#$3058E0F0, FP7",
         0xF200|074, 0x4024|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    TFLT("FSGLDIV.X", "#-89000000032, FP0", "#$C0230000A5C681D100000000, FP0",
         0xF200|074, 0x4024|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    TFLT("FSGLDIV.P", "#9.12E+20, FP1",    "#$002000091200000000000000, FP1",
         0xF200|074, 0x4024|(3<<10)|(1<<7), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    TFLT("FSGLDIV.D", "#-8.25, FP3",       "#$C020800000000000, FP3",
         0xF200|074, 0x4024|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FREM.X", "FP0, FP1",            0xF200,     0x0025|(0<<10)|(1<<7));
    TEST("FREM.X", "FP2, FP2",            0xF200,     0x0025|(2<<10)|(2<<7));
    TEST("FREM.L", "D2, FP3",             0xF200|002, 0x4025|(0<<10)|(3<<7));
    TEST("FREM.S", "D2, FP3",             0xF200|002, 0x4025|(1<<10)|(3<<7));
    ERRT("FREM.X", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                          0xF200|002, 0x4025|(2<<10)|(3<<7));
    ERRT("FREM.P", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                          0xF200|002, 0x4025|(3<<10)|(3<<7));
    TEST("FREM.W", "D2, FP3",             0xF200|002, 0x4025|(4<<10)|(3<<7));
    ERRT("FREM.D", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                          0xF200|002, 0x4025|(5<<10)|(3<<7));
    TEST("FREM.B", "D2, FP3",             0xF200|002, 0x4025|(6<<10)|(3<<7));
    UNKN(/* FREM.P D2, FP3 */             0xF200|002, 0x4025|(7<<10)|(3<<7));
    UNKN(/* FREM.S A4, FP5 */             0xF200|014, 0x4025|(1<<10)|(5<<7));
    TEST("FREM.X", "(A6), FP7",           0xF200|026, 0x4025|(2<<10)|(7<<7));
    TEST("FREM.P", "(A0)+, FP1",          0xF200|030, 0x4025|(3<<10)|(1<<7));
    TEST("FREM.W", "-(A2), FP3",          0xF200|042, 0x4025|(4<<10)|(3<<7));
    TEST("FREM.D", "($1234,A4), FP5",     0xF200|054, 0x4025|(5<<10)|(5<<7), 0x1234);
    TEST("FREM.B", "($23,A6,D7.W), FP0",  0xF200|066, 0x4025|(6<<10)|(0<<7), 0x7023);
    ABST("FREM.L", "(@A).W, FP1", "4566",   0xF200|070, 0x4025|(0<<10)|(1<<7), 0x4566);
    ABST("FREM.S", "(@A).L, FP2", "56789A", 0xF200|071, 0x4025|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FREM.X", "(*+$1234,PC), FP3",   0xF200|072, 0x4025|(2<<10)|(3<<7), 0x1230);
    TEST("FREM.P", "(*+90,PC,A4.L), FP5", 0xF200|073, 0x4025|(3<<10)|(5<<7), 0xC856);
    TEST("FREM.L", "#$6789ABCD, FP6",     0xF200|074, 0x4025|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FREM.W", "#$1234, FP2",         0xF200|074, 0x4025|(4<<10)|(2<<7), 0x1234);
    TEST("FREM.B", "#$23, FP4",           0xF200|074, 0x4025|(6<<10)|(4<<7), 0x0023);
    TFLT("FREM.S", "#7.88999976E-10, FP7", "#$3058E0F0, FP7",
         0xF200|074, 0x4025|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    TFLT("FREM.X", "#-89000000032, FP0",   "#$C0230000A5C681D100000000, FP0",
         0xF200|074, 0x4025|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    TFLT("FREM.P", "#9.12E+20, FP1",       "#$002000091200000000000000, FP1",
         0xF200|074, 0x4025|(3<<10)|(1<<7), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    TFLT("FREM.D", "#-8.25, FP3",          "#$C020800000000000, FP3",
         0xF200|074, 0x4025|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FSCALE.X", "FP0, FP1",            0xF200,     0x0026|(0<<10)|(1<<7));
    TEST("FSCALE.X", "FP2, FP2",            0xF200,     0x0026|(2<<10)|(2<<7));
    TEST("FSCALE.L", "D2, FP3",             0xF200|002, 0x4026|(0<<10)|(3<<7));
    TEST("FSCALE.S", "D2, FP3",             0xF200|002, 0x4026|(1<<10)|(3<<7));
    ERRT("FSCALE.X", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                            0xF200|002, 0x4026|(2<<10)|(3<<7));
    ERRT("FSCALE.P", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                            0xF200|002, 0x4026|(3<<10)|(3<<7));
    TEST("FSCALE.W", "D2, FP3",             0xF200|002, 0x4026|(4<<10)|(3<<7));
    ERRT("FSCALE.D", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                            0xF200|002, 0x4026|(5<<10)|(3<<7));
    TEST("FSCALE.B", "D2, FP3",             0xF200|002, 0x4026|(6<<10)|(3<<7));
    UNKN(/* FSCALE.P D2, FP3 */             0xF200|002, 0x4026|(7<<10)|(3<<7));
    UNKN(/* FSCALE.S A4, FP5 */             0xF200|014, 0x4026|(1<<10)|(5<<7));
    TEST("FSCALE.X", "(A6), FP7",           0xF200|026, 0x4026|(2<<10)|(7<<7));
    TEST("FSCALE.P", "(A0)+, FP1",          0xF200|030, 0x4026|(3<<10)|(1<<7));
    TEST("FSCALE.W", "-(A2), FP3",          0xF200|042, 0x4026|(4<<10)|(3<<7));
    TEST("FSCALE.D", "($1234,A4), FP5",     0xF200|054, 0x4026|(5<<10)|(5<<7), 0x1234);
    TEST("FSCALE.B", "($23,A6,D7.W), FP0",  0xF200|066, 0x4026|(6<<10)|(0<<7), 0x7023);
    ABST("FSCALE.L", "(@A).W, FP1", "4566",   0xF200|070, 0x4026|(0<<10)|(1<<7), 0x4566);
    ABST("FSCALE.S", "(@A).L, FP2", "56789A", 0xF200|071, 0x4026|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FSCALE.X", "(*+$1234,PC), FP3",   0xF200|072, 0x4026|(2<<10)|(3<<7), 0x1230);
    TEST("FSCALE.P", "(*+90,PC,A4.L), FP5", 0xF200|073, 0x4026|(3<<10)|(5<<7), 0xC856);
    TEST("FSCALE.L", "#$6789ABCD, FP6",     0xF200|074, 0x4026|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FSCALE.W", "#$1234, FP2",         0xF200|074, 0x4026|(4<<10)|(2<<7), 0x1234);
    TEST("FSCALE.B", "#$23, FP4",           0xF200|074, 0x4026|(6<<10)|(4<<7), 0x0023);
    TFLT("FSCALE.S", "#7.88999976E-10, FP7", "#$3058E0F0, FP7",
         0xF200|074, 0x4026|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    TFLT("FSCALE.X", "#-89000000032, FP0", "#$C0230000A5C681D100000000, FP0",
         0xF200|074, 0x4026|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    TFLT("FSCALE.P", "#9.12E+20, FP1",     "#$002000091200000000000000, FP1",
         0xF200|074, 0x4026|(3<<10)|(1<<7), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    TFLT("FSCALE.D", "#-8.25, FP3",        "#$C020800000000000, FP3",
         0xF200|074, 0x4026|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FSGLMUL.X", "FP0, FP1",            0xF200,     0x0027|(0<<10)|(1<<7));
    TEST("FSGLMUL.X", "FP2, FP2",            0xF200,     0x0027|(2<<10)|(2<<7));
    TEST("FSGLMUL.L", "D2, FP3",             0xF200|002, 0x4027|(0<<10)|(3<<7));
    TEST("FSGLMUL.S", "D2, FP3",             0xF200|002, 0x4027|(1<<10)|(3<<7));
    ERRT("FSGLMUL.X", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                             0xF200|002, 0x4027|(2<<10)|(3<<7));
    ERRT("FSGLMUL.P", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                             0xF200|002, 0x4027|(3<<10)|(3<<7));
    TEST("FSGLMUL.W", "D2, FP3",             0xF200|002, 0x4027|(4<<10)|(3<<7));
    ERRT("FSGLMUL.D", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                             0xF200|002, 0x4027|(5<<10)|(3<<7));
    TEST("FSGLMUL.B", "D2, FP3",             0xF200|002, 0x4027|(6<<10)|(3<<7));
    UNKN(/* FSGLMUL.P D2, FP3 */             0xF200|002, 0x4027|(7<<10)|(3<<7));
    UNKN(/* FSGLMUL.S A4, FP5 */             0xF200|014, 0x4027|(1<<10)|(5<<7));
    TEST("FSGLMUL.X", "(A6), FP7",           0xF200|026, 0x4027|(2<<10)|(7<<7));
    TEST("FSGLMUL.P", "(A0)+, FP1",          0xF200|030, 0x4027|(3<<10)|(1<<7));
    TEST("FSGLMUL.W", "-(A2), FP3",          0xF200|042, 0x4027|(4<<10)|(3<<7));
    TEST("FSGLMUL.D", "($1234,A4), FP5",     0xF200|054, 0x4027|(5<<10)|(5<<7), 0x1234);
    TEST("FSGLMUL.B", "($23,A6,D7.W), FP0",  0xF200|066, 0x4027|(6<<10)|(0<<7), 0x7023);
    ABST("FSGLMUL.L", "(@A).W, FP1", "4566",   0xF200|070, 0x4027|(0<<10)|(1<<7), 0x4566);
    ABST("FSGLMUL.S", "(@A).L, FP2", "56789A", 0xF200|071, 0x4027|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FSGLMUL.X", "(*+$1234,PC), FP3",   0xF200|072, 0x4027|(2<<10)|(3<<7), 0x1230);
    TEST("FSGLMUL.P", "(*+90,PC,A4.L), FP5", 0xF200|073, 0x4027|(3<<10)|(5<<7), 0xC856);
    TEST("FSGLMUL.L", "#$6789ABCD, FP6",     0xF200|074, 0x4027|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FSGLMUL.W", "#$1234, FP2",         0xF200|074, 0x4027|(4<<10)|(2<<7), 0x1234);
    TEST("FSGLMUL.B", "#$23, FP4",           0xF200|074, 0x4027|(6<<10)|(4<<7), 0x0023);
    TFLT("FSGLMUL.S", "#7.88999976E-10, FP7", "#$3058E0F0, FP7",
         0xF200|074, 0x4027|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    TFLT("FSGLMUL.X", "#-89000000032, FP0", "#$C0230000A5C681D100000000, FP0",
         0xF200|074, 0x4027|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    TFLT("FSGLMUL.P", "#9.12E+20, FP1",    "#$002000091200000000000000, FP1",
         0xF200|074, 0x4027|(3<<10)|(1<<7), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    TFLT("FSGLMUL.D", "#-8.25, FP3",       "#$C020800000000000, FP3",
         0xF200|074, 0x4027|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FSUB.X", "FP0, FP1",            0xF200,     0x0028|(0<<10)|(1<<7));
    TEST("FSUB.X", "FP2, FP2",            0xF200,     0x0028|(2<<10)|(2<<7));
    TEST("FSUB.L", "D2, FP3",             0xF200|002, 0x4028|(0<<10)|(3<<7));
    TEST("FSUB.S", "D2, FP3",             0xF200|002, 0x4028|(1<<10)|(3<<7));
    ERRT("FSUB.X", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                          0xF200|002, 0x4028|(2<<10)|(3<<7));
    ERRT("FSUB.P", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                          0xF200|002, 0x4028|(3<<10)|(3<<7));
    TEST("FSUB.W", "D2, FP3",             0xF200|002, 0x4028|(4<<10)|(3<<7));
    ERRT("FSUB.D", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                          0xF200|002, 0x4028|(5<<10)|(3<<7));
    TEST("FSUB.B", "D2, FP3",             0xF200|002, 0x4028|(6<<10)|(3<<7));
    UNKN(/* FSUB.P D2, FP3 */             0xF200|002, 0x4028|(7<<10)|(3<<7));
    UNKN(/* FSUB.S A4, FP5 */             0xF200|014, 0x4028|(1<<10)|(5<<7));
    TEST("FSUB.X", "(A6), FP7",           0xF200|026, 0x4028|(2<<10)|(7<<7));
    TEST("FSUB.P", "(A0)+, FP1",          0xF200|030, 0x4028|(3<<10)|(1<<7));
    TEST("FSUB.W", "-(A2), FP3",          0xF200|042, 0x4028|(4<<10)|(3<<7));
    TEST("FSUB.D", "($1234,A4), FP5",     0xF200|054, 0x4028|(5<<10)|(5<<7), 0x1234);
    TEST("FSUB.B", "($23,A6,D7.W), FP0",  0xF200|066, 0x4028|(6<<10)|(0<<7), 0x7023);
    ABST("FSUB.L", "(@A).W, FP1", "8566",   0xF200|070, 0x4028|(0<<10)|(1<<7), 0x8566);
    ABST("FSUB.S", "(@A).L, FP2", "56789A", 0xF200|071, 0x4028|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FSUB.X", "(*+$1234,PC), FP3",   0xF200|072, 0x4028|(2<<10)|(3<<7), 0x1230);
    TEST("FSUB.P", "(*+90,PC,A4.L), FP5", 0xF200|073, 0x4028|(3<<10)|(5<<7), 0xC856);
    TEST("FSUB.L", "#$6789ABCD, FP6",     0xF200|074, 0x4028|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FSUB.W", "#$1234, FP2",         0xF200|074, 0x4028|(4<<10)|(2<<7), 0x1234);
    TEST("FSUB.B", "#$23, FP4",           0xF200|074, 0x4028|(6<<10)|(4<<7), 0x0023);
    TFLT("FSUB.S", "#7.88999976E-10, FP7", "#$3058E0F0, FP7",
         0xF200|074, 0x4028|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    TFLT("FSUB.X", "#-89000000032, FP0",   "#$C0230000A5C681D100000000, FP0",
         0xF200|074, 0x4028|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    TFLT("FSUB.P", "#9.12E+20, FP1",       "#$002000091200000000000000, FP1",
         0xF200|074, 0x4028|(3<<10)|(1<<7), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    TFLT("FSUB.D", "#-8.25, FP3",          "#$C020800000000000, FP3",
         0xF200|074, 0x4028|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FSINCOS.X", "FP0, FP2:FP1",            0xF200,     0x0032|(0<<10)|(1<<7));
    TEST("FSINCOS.X", "FP1, FP1:FP1",            0xF200,     0x0031|(1<<10)|(1<<7));
    TEST("FSINCOS.L", "D2, FP4:FP3",             0xF200|002, 0x4034|(0<<10)|(3<<7));
    TEST("FSINCOS.S", "D2, FP4:FP3",             0xF200|002, 0x4034|(1<<10)|(3<<7));
    ERRT("FSINCOS.X", "D2, FP4:FP3", ILLEGAL_SIZE, "D2, FP4:FP3",
                                                 0xF200|002, 0x4034|(2<<10)|(3<<7));
    ERRT("FSINCOS.P", "D2, FP4:FP3", ILLEGAL_SIZE, "D2, FP4:FP3",
                                                 0xF200|002, 0x4034|(3<<10)|(3<<7));
    TEST("FSINCOS.W", "D2, FP4:FP3",             0xF200|002, 0x4034|(4<<10)|(3<<7));
    ERRT("FSINCOS.D", "D2, FP4:FP3", ILLEGAL_SIZE, "D2, FP4:FP3",
                                                 0xF200|002, 0x4034|(5<<10)|(3<<7));
    TEST("FSINCOS.B", "D2, FP4:FP3",             0xF200|002, 0x4034|(6<<10)|(3<<7));
    UNKN(/* FSINCOS.P D2, FP4:FP3 */             0xF200|002, 0x4034|(7<<10)|(3<<7));
    UNKN(/* FSINCOS.S A4, FP2:FP5 */             0xF200|014, 0x4032|(1<<10)|(5<<7));
    TEST("FSINCOS.X", "(A6), FP0:FP7",           0xF200|026, 0x4030|(2<<10)|(7<<7));
    TEST("FSINCOS.P", "(A0)+, FP7:FP1",          0xF200|030, 0x4037|(3<<10)|(1<<7));
    TEST("FSINCOS.W", "-(A2), FP6:FP3",          0xF200|042, 0x4036|(4<<10)|(3<<7));
    TEST("FSINCOS.D", "($1234,A4), FP5:FP5",     0xF200|054, 0x4035|(5<<10)|(5<<7), 0x1234);
    TEST("FSINCOS.B", "($23,A6,D7.W), FP4:FP0",  0xF200|066, 0x4034|(6<<10)|(0<<7), 0x7023);
    ABST("FSINCOS.L", "(@A).W, FP3:FP1", "F566",   0xF200|070, 0x4033|(0<<10)|(1<<7), 0xF566);
    ABST("FSINCOS.S", "(@A).L, FP1:FP2", "56789A", 0xF200|071, 0x4031|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FSINCOS.X", "(*+$1234,PC), FP0:FP3",   0xF200|072, 0x4030|(2<<10)|(3<<7), 0x1230);
    TEST("FSINCOS.P", "(*+90,PC,A4.L), FP7:FP5", 0xF200|073, 0x4037|(3<<10)|(5<<7), 0xC856);
    TEST("FSINCOS.L", "#$6789ABCD, FP5:FP6",     0xF200|074, 0x4035|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FSINCOS.W", "#$1234, FP1:FP2",         0xF200|074, 0x4031|(4<<10)|(2<<7), 0x1234);
    TEST("FSINCOS.B", "#$23, FP7:FP4",           0xF200|074, 0x4037|(6<<10)|(4<<7), 0x0023);
    TFLT("FSINCOS.S", "#7.88999976E-10, FP6:FP7", "#$3058E0F0, FP6:FP7",
         0xF200|074, 0x4036|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    TFLT("FSINCOS.X", "#-89000000032, FP4:FP0", "#$C0230000A5C681D100000000, FP4:FP0",
         0xF200|074, 0x4034|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    TFLT("FSINCOS.P", "#9.12E+20, FP3:FP1", "#$002000091200000000000000, FP3:FP1",
         0xF200|074, 0x4033|(3<<10)|(1<<7), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    TFLT("FSINCOS.D", "#-8.25, FP0:FP3",   "#$C020800000000000, FP0:FP3",
         0xF200|074, 0x4030|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FCMP.X", "FP0, FP1",            0xF200,     0x0038|(0<<10)|(1<<7));
    TEST("FCMP.X", "FP2, FP2",            0xF200,     0x0038|(2<<10)|(2<<7));
    TEST("FCMP.L", "D2, FP3",             0xF200|002, 0x4038|(0<<10)|(3<<7));
    TEST("FCMP.S", "D2, FP3",             0xF200|002, 0x4038|(1<<10)|(3<<7));
    ERRT("FCMP.X", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                          0xF200|002, 0x4038|(2<<10)|(3<<7));
    ERRT("FCMP.P", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                          0xF200|002, 0x4038|(3<<10)|(3<<7));
    TEST("FCMP.W", "D2, FP3",             0xF200|002, 0x4038|(4<<10)|(3<<7));
    ERRT("FCMP.D", "D2, FP3", ILLEGAL_SIZE, "D2, FP3",
                                          0xF200|002, 0x4038|(5<<10)|(3<<7));
    TEST("FCMP.B", "D2, FP3",             0xF200|002, 0x4038|(6<<10)|(3<<7));
    UNKN(/* FCMP.P D2, FP3 */             0xF200|002, 0x4038|(7<<10)|(3<<7));
    UNKN(/* FCMP.S A4, FP5 */             0xF200|014, 0x4038|(1<<10)|(5<<7));
    TEST("FCMP.X", "(A6), FP7",           0xF200|026, 0x4038|(2<<10)|(7<<7));
    TEST("FCMP.P", "(A0)+, FP1",          0xF200|030, 0x4038|(3<<10)|(1<<7));
    TEST("FCMP.W", "-(A2), FP3",          0xF200|042, 0x4038|(4<<10)|(3<<7));
    TEST("FCMP.D", "($1234,A4), FP5",     0xF200|054, 0x4038|(5<<10)|(5<<7), 0x1234);
    TEST("FCMP.B", "($23,A6,D7.W), FP0",  0xF200|066, 0x4038|(6<<10)|(0<<7), 0x7023);
    ABST("FCMP.L", "(@A).W, FP1", "4566",   0xF200|070, 0x4038|(0<<10)|(1<<7), 0x4566);
    ABST("FCMP.S", "(@A).L, FP2", "56789A", 0xF200|071, 0x4038|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FCMP.X", "(*+$1234,PC), FP3",   0xF200|072, 0x4038|(2<<10)|(3<<7), 0x1230);
    TEST("FCMP.P", "(*+90,PC,A4.L), FP5", 0xF200|073, 0x4038|(3<<10)|(5<<7), 0xC856);
    TEST("FCMP.L", "#$6789ABCD, FP6",     0xF200|074, 0x4038|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FCMP.W", "#$1234, FP2",         0xF200|074, 0x4038|(4<<10)|(2<<7), 0x1234);
    TEST("FCMP.B", "#$23, FP4",           0xF200|074, 0x4038|(6<<10)|(4<<7), 0x0023);
    TFLT("FCMP.S", "#7.88999976E-10, FP7", "#$3058E0F0, FP7",
         0xF200|074, 0x4038|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    TFLT("FCMP.X", "#-89000000032, FP0",   "#$C0230000A5C681D100000000, FP0",
         0xF200|074, 0x4038|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    TFLT("FCMP.P", "#9.12E+20, FP1",       "#$002000091200000000000000, FP1",
         0xF200|074, 0x4038|(3<<10)|(1<<7), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    TFLT("FCMP.D", "#-8.25, FP3",          "#$C020800000000000, FP3",
         0xF200|074, 0x4038|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FTST.X", "FP0",            0xF200,     0x003A|(0<<10));
    TEST("FTST.L", "D2",             0xF200|002, 0x403A|(0<<10));
    TEST("FTST.S", "D2",             0xF200|002, 0x403A|(1<<10));
    ERRT("FTST.X", "D2", ILLEGAL_SIZE, "D2",
                                     0xF200|002, 0x403A|(2<<10));
    ERRT("FTST.P", "D2", ILLEGAL_SIZE, "D2",
                                     0xF200|002, 0x403A|(3<<10));
    TEST("FTST.W", "D2",             0xF200|002, 0x403A|(4<<10));
    ERRT("FTST.D", "D2", ILLEGAL_SIZE, "D2",
                                     0xF200|002, 0x403A|(5<<10));
    TEST("FTST.B", "D2",             0xF200|002, 0x403A|(6<<10));
    UNKN(/* FTST.P D2 */             0xF200|002, 0x403A|(7<<10));
    UNKN(/* FTST.S A4, FP5 */        0xF200|014, 0x403A|(1<<10));
    TEST("FTST.X", "(A6)",           0xF200|026, 0x403A|(2<<10));
    TEST("FTST.P", "(A0)+",          0xF200|030, 0x403A|(3<<10));
    TEST("FTST.W", "-(A2)",          0xF200|042, 0x403A|(4<<10));
    TEST("FTST.D", "($1234,A4)",     0xF200|054, 0x403A|(5<<10), 0x1234);
    TEST("FTST.B", "($23,A6,D7.W)",  0xF200|066, 0x403A|(6<<10), 0x7023);
    ABST("FTST.L", "(@A).W", "4566",   0xF200|070, 0x403A|(0<<10), 0x4566);
    ABST("FTST.S", "(@A).L", "86789A", 0xF200|071, 0x403A|(1<<10), 0x0086, 0x789A);
    TEST("FTST.X", "(*+$1234,PC)",   0xF200|072, 0x403A|(2<<10), 0x1230);
    TEST("FTST.P", "(*+90,PC,A4.L)", 0xF200|073, 0x403A|(3<<10), 0xC856);
    TEST("FTST.L", "#$6789ABCD",     0xF200|074, 0x403A|(0<<10), 0x6789, 0xABCD);
    TEST("FTST.W", "#$1234",         0xF200|074, 0x403A|(4<<10), 0x1234);
    TEST("FTST.B", "#$23",           0xF200|074, 0x403A|(6<<10), 0x0023);
    TFLT("FTST.S", "#7.88999976E-10", "#$3058E0F0",
         0xF200|074, 0x403A|(1<<10), 0x3058, 0xE0F0);
    TFLT("FTST.X", "#-89000000032", "#$C0230000A5C681D100000000",
         0xF200|074, 0x403A|(2<<10), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    TFLT("FTST.P", "#9.12E+20",     "#$002000091200000000000000",
         0xF200|074, 0x403A|(3<<10), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    TFLT("FTST.D", "#-8.25",        "#$C020800000000000",
         0xF200|074, 0x403A|(5<<10), 0xC020, 0x8000, 0x0000, 0x0000);
}

void test_float_branch() {
    TEST("FNOP", "",                       0xF280, 0x0000);
    ATEST(0x10000, "FBF",    "*+$1234",    0xF280, 0x1232);
    ATEST(0x10000, "FBEQ",   "*+$1234",    0xF281, 0x1232);
    ATEST(0x10000, "FBOGT",  "*-$1234",    0xF282, 0xEDCA);
    ATEST(0x10000, "FBOGE",  "*+$1234",    0xF283, 0x1232);
    ATEST(0x10000, "FBOLT",  "*+$1234",    0xF284, 0x1232);
    ATEST(0x10000, "FBOLE",  "*+$1234",    0xF285, 0x1232);
    ATEST(0x10000, "FBOGL",  "*+$1234",    0xF286, 0x1232);
    ATEST(0x10000, "FBOR",   "*+$1234",    0xF287, 0x1232);
    ATEST(0x10000, "FBUN",   "*+$1234",    0xF288, 0x1232);
    ATEST(0x10000, "FBUEQ",  "*+$1234",    0xF289, 0x1232);
    ATEST(0x10000, "FBUGT",  "*+$1234",    0xF28A, 0x1232);
    ATEST(0x10000, "FBUGE",  "*+$1234",    0xF28B, 0x1232);
    ATEST(0x10000, "FBULT",  "*+$1234",    0xF28C, 0x1232);
    ATEST(0x10000, "FBULE",  "*+$1234",    0xF28D, 0x1232);
    ATEST(0x10000, "FBNE",   "*+$1234",    0xF28E, 0x1232);
    ATEST(0x10000, "FBT",    "*+$1234",    0xF28F, 0x1232);
    ATEST(0x10000, "FBSF",   "*+$1234",    0xF290, 0x1232);
    ATEST(0x10000, "FBSEQ",  "*+$1234",    0xF291, 0x1232);
    ATEST(0x10000, "FBGT",   "*+$1234",    0xF292, 0x1232);
    ATEST(0x10000, "FBGE",   "*+$1234",    0xF293, 0x1232);
    ATEST(0x10000, "FBLT",   "*+$1234",    0xF294, 0x1232);
    ATEST(0x10000, "FBLE",   "*+$1234",    0xF295, 0x1232);
    ATEST(0x10000, "FBGL",   "*+$1234",    0xF296, 0x1232);
    ATEST(0x10000, "FBGLE",  "*+$1234",    0xF297, 0x1232);
    ATEST(0x10000, "FBNGLE", "*+$1234",    0xF298, 0x1232);
    ATEST(0x10000, "FBNGL",  "*+$1234",    0xF299, 0x1232);
    ATEST(0x10000, "FBNLE",  "*+$1234",    0xF29A, 0x1232);
    ATEST(0x10000, "FBNLT",  "*+$1234",    0xF29B, 0x1232);
    ATEST(0x10000, "FBNGE",  "*+$1234",    0xF29C, 0x1232);
    ATEST(0x10000, "FBNGT",  "*+$1234",    0xF29D, 0x1232);
    ATEST(0x10000, "FBSNE",  "*+$1234",    0xF29E, 0x1232);
    ATEST(0x10000, "FBST",   "*+$1234",    0xF29F, 0x1232);
    AERRT(0x01000, "FBST",   "*-$00002000", OVERFLOW_RANGE, "*-$00002000", 0xF29F, 0xDFFE);

    ARELT(0x200000, "FBF",    "*+@R", "123456", 0xF2C0, 0x0012, 0x3454);
    ARELT(0x200000, "FBEQ.X", "*+@R", "1234",   0xF2C1, 0x0000, 0x1232);
    ARELT(0x200000, "FBOGT",  "*-@R", "123456", 0xF2C2, 0xFFED, 0xCBA8);
    ARELT(0x200000, "FBOGE",  "*+@R", "123456", 0xF2C3, 0x0012, 0x3454);
    ARELT(0x200000, "FBOLT",  "*+@R", "123456", 0xF2C4, 0x0012, 0x3454);
    ARELT(0x200000, "FBOLE",  "*+@R", "123456", 0xF2C5, 0x0012, 0x3454);
    ARELT(0x200000, "FBOGL",  "*+@R", "123456", 0xF2C6, 0x0012, 0x3454);
    ARELT(0x200000, "FBOR",   "*+@R", "123456", 0xF2C7, 0x0012, 0x3454);
    ARELT(0x200000, "FBUN",   "*+@R", "123456", 0xF2C8, 0x0012, 0x3454);
    ARELT(0x200000, "FBUEQ",  "*+@R", "123456", 0xF2C9, 0x0012, 0x3454);
    ARELT(0x200000, "FBUGT",  "*+@R", "123456", 0xF2CA, 0x0012, 0x3454);
    ARELT(0x200000, "FBUGE",  "*+@R", "123456", 0xF2CB, 0x0012, 0x3454);
    ARELT(0x200000, "FBULT",  "*+@R", "123456", 0xF2CC, 0x0012, 0x3454);
    ARELT(0x200000, "FBULE",  "*+@R", "123456", 0xF2CD, 0x0012, 0x3454);
    ARELT(0x200000, "FBNE",   "*+@R", "123456", 0xF2CE, 0x0012, 0x3454);
    ARELT(0x200000, "FBT",    "*+@R", "123456", 0xF2CF, 0x0012, 0x3454);
    ARELT(0x200000, "FBSF",   "*+@R", "123456", 0xF2D0, 0x0012, 0x3454);
    ARELT(0x200000, "FBSEQ",  "*+@R", "123456", 0xF2D1, 0x0012, 0x3454);
    ARELT(0x200000, "FBGT",   "*+@R", "123456", 0xF2D2, 0x0012, 0x3454);
    ARELT(0x200000, "FBGE",   "*+@R", "123456", 0xF2D3, 0x0012, 0x3454);
    ARELT(0x200000, "FBLT",   "*+@R", "123456", 0xF2D4, 0x0012, 0x3454);
    ARELT(0x200000, "FBLE",   "*+@R", "123456", 0xF2D5, 0x0012, 0x3454);
    ARELT(0x200000, "FBGL",   "*+@R", "123456", 0xF2D6, 0x0012, 0x3454);
    ARELT(0x200000, "FBGLE",  "*+@R", "123456", 0xF2D7, 0x0012, 0x3454);
    ARELT(0x200000, "FBNGLE", "*+@R", "123456", 0xF2D8, 0x0012, 0x3454);
    ARELT(0x200000, "FBNGL",  "*+@R", "123456", 0xF2D9, 0x0012, 0x3454);
    ARELT(0x200000, "FBNLE",  "*+@R", "123456", 0xF2DA, 0x0012, 0x3454);
    ARELT(0x200000, "FBNLT",  "*+@R", "123456", 0xF2DB, 0x0012, 0x3454);
    ARELT(0x200000, "FBNGE",  "*+@R", "123456", 0xF2DC, 0x0012, 0x3454);
    ARELT(0x200000, "FBNGT",  "*+@R", "123456", 0xF2DD, 0x0012, 0x3454);
    ARELT(0x200000, "FBSNE",  "*+@R", "123456", 0xF2DE, 0x0012, 0x3454);
    ARELT(0x200000, "FBST",   "*+@R", "123456", 0xF2DF, 0x0012, 0x3454);
    if (firstGen()) {
        ERELT(0xF00000, "FBST",   "*+@R", "12345678", OVERFLOW_RANGE, "*+@R", 0xF2DF, 0x1234, 0x5676);
    } else {
        ARELT(0xF00000, "FBST",   "*+@R", "12345678", 0xF2DF, 0x1234, 0x5676);
    }

    disassembler.setOption("gnu-as", "on");
    GRELT("FBFL",  ".+@R", "123456", 0xF2C0, 0x0012, 0x3454);
    GRELT("FBEQL", ".+@R", "1234",   0xF2C1, 0x0000, 0x1232);
}

void test_float_trap() {
    TEST("FSF", "D2",              0xF240|002, 0x0000);
    TEST("FDBF", "D2, *+$1234",    0xF240|012, 0x0000, 0x1230);
    TEST("FSF", "(A2)",            0xF240|022, 0x0000);
    TEST("FSF", "(A2)+",           0xF240|032, 0x0000);
    TEST("FSF", "-(A2)",           0xF240|042, 0x0000);
    TEST("FSF", "($1234,A2)",      0xF240|052, 0x0000, 0x1234);
    TEST("FSF", "(18,A2,D3.W)",    0xF240|062, 0x0000, 0x3012);
    ABST("FSF", "(@A).W", "1234",   0xF240|070, 0x0000, 0x1234);
    ABST("FSF", "(@A).L", "123456", 0xF240|071, 0x0000, 0x0012, 0x3456);
    TEST("FTRAPF.W", "#$1234",     0xF240|072, 0x0000, 0x1234);
    TEST("FTRAPF.L", "#$12345678", 0xF240|073, 0x0000, 0x1234, 0x5678);
    TEST("FTRAPF",   "",           0xF240|074, 0x0000);

    TEST("FSEQ", "D2",              0xF240|002, 0x0001);
    TEST("FDBEQ", "D2, *+$1234",    0xF240|012, 0x0001, 0x1230);
    TEST("FSEQ", "(A2)",            0xF240|022, 0x0001);
    TEST("FSEQ", "(A2)+",           0xF240|032, 0x0001);
    TEST("FSEQ", "-(A2)",           0xF240|042, 0x0001);
    TEST("FSEQ", "($1234,A2)",      0xF240|052, 0x0001, 0x1234);
    TEST("FSEQ", "(18,A2,D3.W)",    0xF240|062, 0x0001, 0x3012);
    ABST("FSEQ", "(@A).W", "1234",   0xF240|070, 0x0001, 0x1234);
    ABST("FSEQ", "(@A).L", "123456", 0xF240|071, 0x0001, 0x0012, 0x3456);
    TEST("FTRAPEQ.W", "#$1234",     0xF240|072, 0x0001, 0x1234);
    TEST("FTRAPEQ.L", "#$12345678", 0xF240|073, 0x0001, 0x1234, 0x5678);
    TEST("FTRAPEQ",   "",           0xF240|074, 0x0001);

    TEST("FSOGT", "D2",              0xF240|002, 0x0002);
    TEST("FDBOGT", "D2, *+$1234",    0xF240|012, 0x0002, 0x1230);
    TEST("FSOGT", "(A2)",            0xF240|022, 0x0002);
    TEST("FSOGT", "(A2)+",           0xF240|032, 0x0002);
    TEST("FSOGT", "-(A2)",           0xF240|042, 0x0002);
    TEST("FSOGT", "($1234,A2)",      0xF240|052, 0x0002, 0x1234);
    TEST("FSOGT", "(18,A2,D3.W)",    0xF240|062, 0x0002, 0x3012);
    ABST("FSOGT", "(@A).W", "1234",   0xF240|070, 0x0002, 0x1234);
    ABST("FSOGT", "(@A).L", "123456", 0xF240|071, 0x0002, 0x0012, 0x3456);
    TEST("FTRAPOGT.W", "#$1234",     0xF240|072, 0x0002, 0x1234);
    TEST("FTRAPOGT.L", "#$12345678", 0xF240|073, 0x0002, 0x1234, 0x5678);
    TEST("FTRAPOGT",   "",           0xF240|074, 0x0002);

    TEST("FSOGE", "D2",              0xF240|002, 0x0003);
    TEST("FDBOGE", "D2, *+$1234",    0xF240|012, 0x0003, 0x1230);
    TEST("FSOGE", "(A2)",            0xF240|022, 0x0003);
    TEST("FSOGE", "(A2)+",           0xF240|032, 0x0003);
    TEST("FSOGE", "-(A2)",           0xF240|042, 0x0003);
    TEST("FSOGE", "($1234,A2)",      0xF240|052, 0x0003, 0x1234);
    TEST("FSOGE", "(18,A2,D3.W)",    0xF240|062, 0x0003, 0x3012);
    ABST("FSOGE", "(@A).W", "1234",   0xF240|070, 0x0003, 0x1234);
    ABST("FSOGE", "(@A).L", "123456", 0xF240|071, 0x0003, 0x0012, 0x3456);
    TEST("FTRAPOGE.W", "#$1234",     0xF240|072, 0x0003, 0x1234);
    TEST("FTRAPOGE.L", "#$12345678", 0xF240|073, 0x0003, 0x1234, 0x5678);
    TEST("FTRAPOGE",   "",           0xF240|074, 0x0003);

    TEST("FSOLT", "D2",              0xF240|002, 0x0004);
    TEST("FDBOLT", "D2, *+$1234",    0xF240|012, 0x0004, 0x1230);
    TEST("FSOLT", "(A2)",            0xF240|022, 0x0004);
    TEST("FSOLT", "(A2)+",           0xF240|032, 0x0004);
    TEST("FSOLT", "-(A2)",           0xF240|042, 0x0004);
    TEST("FSOLT", "($1234,A2)",      0xF240|052, 0x0004, 0x1234);
    TEST("FSOLT", "(18,A2,D3.W)",    0xF240|062, 0x0004, 0x3012);
    ABST("FSOLT", "(@A).W", "1234",   0xF240|070, 0x0004, 0x1234);
    ABST("FSOLT", "(@A).L", "123456", 0xF240|071, 0x0004, 0x0012, 0x3456);
    TEST("FTRAPOLT.W", "#$1234",     0xF240|072, 0x0004, 0x1234);
    TEST("FTRAPOLT.L", "#$12345678", 0xF240|073, 0x0004, 0x1234, 0x5678);
    TEST("FTRAPOLT",   "",           0xF240|074, 0x0004);

    TEST("FSOLE", "D2",              0xF240|002, 0x0005);
    TEST("FDBOLE", "D2, *+$1234",    0xF240|012, 0x0005, 0x1230);
    TEST("FSOLE", "(A2)",            0xF240|022, 0x0005);
    TEST("FSOLE", "(A2)+",           0xF240|032, 0x0005);
    TEST("FSOLE", "-(A2)",           0xF240|042, 0x0005);
    TEST("FSOLE", "($1234,A2)",      0xF240|052, 0x0005, 0x1234);
    TEST("FSOLE", "(18,A2,D3.W)",    0xF240|062, 0x0005, 0x3012);
    ABST("FSOLE", "(@A).W", "1234",   0xF240|070, 0x0005, 0x1234);
    ABST("FSOLE", "(@A).L", "123456", 0xF240|071, 0x0005, 0x0012, 0x3456);
    TEST("FTRAPOLE.W", "#$1234",     0xF240|072, 0x0005, 0x1234);
    TEST("FTRAPOLE.L", "#$12345678", 0xF240|073, 0x0005, 0x1234, 0x5678);
    TEST("FTRAPOLE",   "",           0xF240|074, 0x0005);

    TEST("FSOGL", "D2",              0xF240|002, 0x0006);
    TEST("FDBOGL", "D2, *+$1234",    0xF240|012, 0x0006, 0x1230);
    TEST("FSOGL", "(A2)",            0xF240|022, 0x0006);
    TEST("FSOGL", "(A2)+",           0xF240|032, 0x0006);
    TEST("FSOGL", "-(A2)",           0xF240|042, 0x0006);
    TEST("FSOGL", "($1234,A2)",      0xF240|052, 0x0006, 0x1234);
    TEST("FSOGL", "(18,A2,D3.W)",    0xF240|062, 0x0006, 0x3012);
    ABST("FSOGL", "(@A).W", "1234",   0xF240|070, 0x0006, 0x1234);
    ABST("FSOGL", "(@A).L", "123456", 0xF240|071, 0x0006, 0x0012, 0x3456);
    TEST("FTRAPOGL.W", "#$1234",     0xF240|072, 0x0006, 0x1234);
    TEST("FTRAPOGL.L", "#$12345678", 0xF240|073, 0x0006, 0x1234, 0x5678);
    TEST("FTRAPOGL",   "",           0xF240|074, 0x0006);

    TEST("FSOR", "D2",              0xF240|002, 0x0007);
    TEST("FDBOR", "D2, *+$1234",    0xF240|012, 0x0007, 0x1230);
    TEST("FSOR", "(A2)",            0xF240|022, 0x0007);
    TEST("FSOR", "(A2)+",           0xF240|032, 0x0007);
    TEST("FSOR", "-(A2)",           0xF240|042, 0x0007);
    TEST("FSOR", "($1234,A2)",      0xF240|052, 0x0007, 0x1234);
    TEST("FSOR", "(18,A2,D3.W)",    0xF240|062, 0x0007, 0x3012);
    ABST("FSOR", "(@A).W", "1234",   0xF240|070, 0x0007, 0x1234);
    ABST("FSOR", "(@A).L", "123456", 0xF240|071, 0x0007, 0x0012, 0x3456);
    TEST("FTRAPOR.W", "#$1234",     0xF240|072, 0x0007, 0x1234);
    TEST("FTRAPOR.L", "#$12345678", 0xF240|073, 0x0007, 0x1234, 0x5678);
    TEST("FTRAPOR",   "",           0xF240|074, 0x0007);

    TEST("FSUN", "D2",              0xF240|002, 0x0008);
    TEST("FDBUN", "D2, *+$1234",    0xF240|012, 0x0008, 0x1230);
    TEST("FSUN", "(A2)",            0xF240|022, 0x0008);
    TEST("FSUN", "(A2)+",           0xF240|032, 0x0008);
    TEST("FSUN", "-(A2)",           0xF240|042, 0x0008);
    TEST("FSUN", "($1234,A2)",      0xF240|052, 0x0008, 0x1234);
    TEST("FSUN", "(18,A2,D3.W)",    0xF240|062, 0x0008, 0x3012);
    ABST("FSUN", "(@A).W", "1234",   0xF240|070, 0x0008, 0x1234);
    ABST("FSUN", "(@A).L", "123456", 0xF240|071, 0x0008, 0x0012, 0x3456);
    TEST("FTRAPUN.W", "#$1234",     0xF240|072, 0x0008, 0x1234);
    TEST("FTRAPUN.L", "#$12345678", 0xF240|073, 0x0008, 0x1234, 0x5678);
    TEST("FTRAPUN",   "",           0xF240|074, 0x0008);

    TEST("FSUEQ", "D2",              0xF240|002, 0x0009);
    TEST("FDBUEQ", "D2, *+$1234",    0xF240|012, 0x0009, 0x1230);
    TEST("FSUEQ", "(A2)",            0xF240|022, 0x0009);
    TEST("FSUEQ", "(A2)+",           0xF240|032, 0x0009);
    TEST("FSUEQ", "-(A2)",           0xF240|042, 0x0009);
    TEST("FSUEQ", "($1234,A2)",      0xF240|052, 0x0009, 0x1234);
    TEST("FSUEQ", "(18,A2,D3.W)",    0xF240|062, 0x0009, 0x3012);
    ABST("FSUEQ", "(@A).W", "1234",   0xF240|070, 0x0009, 0x1234);
    ABST("FSUEQ", "(@A).L", "123456", 0xF240|071, 0x0009, 0x0012, 0x3456);
    TEST("FTRAPUEQ.W", "#$1234",     0xF240|072, 0x0009, 0x1234);
    TEST("FTRAPUEQ.L", "#$12345678", 0xF240|073, 0x0009, 0x1234, 0x5678);
    TEST("FTRAPUEQ",   "",           0xF240|074, 0x0009);

    TEST("FSUGT", "D2",              0xF240|002, 0x000A);
    TEST("FDBUGT", "D2, *+$1234",    0xF240|012, 0x000A, 0x1230);
    TEST("FSUGT", "(A2)",            0xF240|022, 0x000A);
    TEST("FSUGT", "(A2)+",           0xF240|032, 0x000A);
    TEST("FSUGT", "-(A2)",           0xF240|042, 0x000A);
    TEST("FSUGT", "($1234,A2)",      0xF240|052, 0x000A, 0x1234);
    TEST("FSUGT", "(18,A2,D3.W)",    0xF240|062, 0x000A, 0x3012);
    ABST("FSUGT", "(@A).W", "1234",   0xF240|070, 0x000A, 0x1234);
    ABST("FSUGT", "(@A).L", "123456", 0xF240|071, 0x000A, 0x0012, 0x3456);
    TEST("FTRAPUGT.W", "#$1234",     0xF240|072, 0x000A, 0x1234);
    TEST("FTRAPUGT.L", "#$12345678", 0xF240|073, 0x000A, 0x1234, 0x5678);
    TEST("FTRAPUGT",   "",           0xF240|074, 0x000A);

    TEST("FSUGE", "D2",              0xF240|002, 0x000B);
    TEST("FDBUGE", "D2, *+$1234",    0xF240|012, 0x000B, 0x1230);
    TEST("FSUGE", "(A2)",            0xF240|022, 0x000B);
    TEST("FSUGE", "(A2)+",           0xF240|032, 0x000B);
    TEST("FSUGE", "-(A2)",           0xF240|042, 0x000B);
    TEST("FSUGE", "($1234,A2)",      0xF240|052, 0x000B, 0x1234);
    TEST("FSUGE", "(18,A2,D3.W)",    0xF240|062, 0x000B, 0x3012);
    ABST("FSUGE", "(@A).W", "1234",   0xF240|070, 0x000B, 0x1234);
    ABST("FSUGE", "(@A).L", "123456", 0xF240|071, 0x000B, 0x0012, 0x3456);
    TEST("FTRAPUGE.W", "#$1234",     0xF240|072, 0x000B, 0x1234);
    TEST("FTRAPUGE.L", "#$12345678", 0xF240|073, 0x000B, 0x1234, 0x5678);
    TEST("FTRAPUGE",   "",           0xF240|074, 0x000B);

    TEST("FSULT", "D2",              0xF240|002, 0x000C);
    TEST("FDBULT", "D2, *+$1234",    0xF240|012, 0x000C, 0x1230);
    TEST("FSULT", "(A2)",            0xF240|022, 0x000C);
    TEST("FSULT", "(A2)+",           0xF240|032, 0x000C);
    TEST("FSULT", "-(A2)",           0xF240|042, 0x000C);
    TEST("FSULT", "($1234,A2)",      0xF240|052, 0x000C, 0x1234);
    TEST("FSULT", "(18,A2,D3.W)",    0xF240|062, 0x000C, 0x3012);
    ABST("FSULT", "(@A).W", "1234",   0xF240|070, 0x000C, 0x1234);
    ABST("FSULT", "(@A).L", "123456", 0xF240|071, 0x000C, 0x0012, 0x3456);
    TEST("FTRAPULT.W", "#$1234",     0xF240|072, 0x000C, 0x1234);
    TEST("FTRAPULT.L", "#$12345678", 0xF240|073, 0x000C, 0x1234, 0x5678);
    TEST("FTRAPULT",   "",           0xF240|074, 0x000C);

    TEST("FSULE", "D2",              0xF240|002, 0x000D);
    TEST("FDBULE", "D2, *+$1234",    0xF240|012, 0x000D, 0x1230);
    TEST("FSULE", "(A2)",            0xF240|022, 0x000D);
    TEST("FSULE", "(A2)+",           0xF240|032, 0x000D);
    TEST("FSULE", "-(A2)",           0xF240|042, 0x000D);
    TEST("FSULE", "($1234,A2)",      0xF240|052, 0x000D, 0x1234);
    TEST("FSULE", "(18,A2,D3.W)",    0xF240|062, 0x000D, 0x3012);
    ABST("FSULE", "(@A).W", "1234",   0xF240|070, 0x000D, 0x1234);
    ABST("FSULE", "(@A).L", "123456", 0xF240|071, 0x000D, 0x0012, 0x3456);
    TEST("FTRAPULE.W", "#$1234",     0xF240|072, 0x000D, 0x1234);
    TEST("FTRAPULE.L", "#$12345678", 0xF240|073, 0x000D, 0x1234, 0x5678);
    TEST("FTRAPULE",   "",           0xF240|074, 0x000D);

    TEST("FSNE", "D2",              0xF240|002, 0x000E);
    TEST("FDBNE", "D2, *+$1234",    0xF240|012, 0x000E, 0x1230);
    TEST("FSNE", "(A2)",            0xF240|022, 0x000E);
    TEST("FSNE", "(A2)+",           0xF240|032, 0x000E);
    TEST("FSNE", "-(A2)",           0xF240|042, 0x000E);
    TEST("FSNE", "($1234,A2)",      0xF240|052, 0x000E, 0x1234);
    TEST("FSNE", "(18,A2,D3.W)",    0xF240|062, 0x000E, 0x3012);
    ABST("FSNE", "(@A).W", "1234",   0xF240|070, 0x000E, 0x1234);
    ABST("FSNE", "(@A).L", "123456", 0xF240|071, 0x000E, 0x0012, 0x3456);
    TEST("FTRAPNE.W", "#$1234",     0xF240|072, 0x000E, 0x1234);
    TEST("FTRAPNE.L", "#$12345678", 0xF240|073, 0x000E, 0x1234, 0x5678);
    TEST("FTRAPNE",   "",           0xF240|074, 0x000E);

    TEST("FST", "D2",              0xF240|002, 0x000F);
    TEST("FDBT", "D2, *+$1234",    0xF240|012, 0x000F, 0x1230);
    TEST("FST", "(A2)",            0xF240|022, 0x000F);
    TEST("FST", "(A2)+",           0xF240|032, 0x000F);
    TEST("FST", "-(A2)",           0xF240|042, 0x000F);
    TEST("FST", "($1234,A2)",      0xF240|052, 0x000F, 0x1234);
    TEST("FST", "(18,A2,D3.W)",    0xF240|062, 0x000F, 0x3012);
    ABST("FST", "(@A).W", "1234",   0xF240|070, 0x000F, 0x1234);
    ABST("FST", "(@A).L", "123456", 0xF240|071, 0x000F, 0x0012, 0x3456);
    TEST("FTRAPT.W", "#$1234",     0xF240|072, 0x000F, 0x1234);
    TEST("FTRAPT.L", "#$12345678", 0xF240|073, 0x000F, 0x1234, 0x5678);
    TEST("FTRAPT",   "",           0xF240|074, 0x000F);

    TEST("FSSF", "D2",              0xF240|002, 0x0010);
    TEST("FDBSF", "D2, *+$1234",    0xF240|012, 0x0010, 0x1230);
    TEST("FSSF", "(A2)",            0xF240|022, 0x0010);
    TEST("FSSF", "(A2)+",           0xF240|032, 0x0010);
    TEST("FSSF", "-(A2)",           0xF240|042, 0x0010);
    TEST("FSSF", "($1234,A2)",      0xF240|052, 0x0010, 0x1234);
    TEST("FSSF", "(18,A2,D3.W)",    0xF240|062, 0x0010, 0x3012);
    ABST("FSSF", "(@A).W", "1234",   0xF240|070, 0x0010, 0x1234);
    ABST("FSSF", "(@A).L", "123456", 0xF240|071, 0x0010, 0x0012, 0x3456);
    TEST("FTRAPSF.W", "#$1234",     0xF240|072, 0x0010, 0x1234);
    TEST("FTRAPSF.L", "#$12345678", 0xF240|073, 0x0010, 0x1234, 0x5678);
    TEST("FTRAPSF",   "",           0xF240|074, 0x0010);

    TEST("FSSEQ", "D2",              0xF240|002, 0x0011);
    TEST("FDBSEQ", "D2, *+$1234",    0xF240|012, 0x0011, 0x1230);
    TEST("FSSEQ", "(A2)",            0xF240|022, 0x0011);
    TEST("FSSEQ", "(A2)+",           0xF240|032, 0x0011);
    TEST("FSSEQ", "-(A2)",           0xF240|042, 0x0011);
    TEST("FSSEQ", "($1234,A2)",      0xF240|052, 0x0011, 0x1234);
    TEST("FSSEQ", "(18,A2,D3.W)",    0xF240|062, 0x0011, 0x3012);
    ABST("FSSEQ", "(@A).W", "1234",   0xF240|070, 0x0011, 0x1234);
    ABST("FSSEQ", "(@A).L", "123456", 0xF240|071, 0x0011, 0x0012, 0x3456);
    TEST("FTRAPSEQ.W", "#$1234",     0xF240|072, 0x0011, 0x1234);
    TEST("FTRAPSEQ.L", "#$12345678", 0xF240|073, 0x0011, 0x1234, 0x5678);
    TEST("FTRAPSEQ",   "",           0xF240|074, 0x0011);

    TEST("FSGT", "D2",              0xF240|002, 0x0012);
    TEST("FDBGT", "D2, *+$1234",    0xF240|012, 0x0012, 0x1230);
    TEST("FSGT", "(A2)",            0xF240|022, 0x0012);
    TEST("FSGT", "(A2)+",           0xF240|032, 0x0012);
    TEST("FSGT", "-(A2)",           0xF240|042, 0x0012);
    TEST("FSGT", "($1234,A2)",      0xF240|052, 0x0012, 0x1234);
    TEST("FSGT", "(18,A2,D3.W)",    0xF240|062, 0x0012, 0x3012);
    ABST("FSGT", "(@A).W", "1234",   0xF240|070, 0x0012, 0x1234);
    ABST("FSGT", "(@A).L", "123456", 0xF240|071, 0x0012, 0x0012, 0x3456);
    TEST("FTRAPGT.W", "#$1234",     0xF240|072, 0x0012, 0x1234);
    TEST("FTRAPGT.L", "#$12345678", 0xF240|073, 0x0012, 0x1234, 0x5678);
    TEST("FTRAPGT",   "",           0xF240|074, 0x0012);

    TEST("FSGE", "D2",              0xF240|002, 0x0013);
    TEST("FDBGE", "D2, *+$1234",    0xF240|012, 0x0013, 0x1230);
    TEST("FSGE", "(A2)",            0xF240|022, 0x0013);
    TEST("FSGE", "(A2)+",           0xF240|032, 0x0013);
    TEST("FSGE", "-(A2)",           0xF240|042, 0x0013);
    TEST("FSGE", "($1234,A2)",      0xF240|052, 0x0013, 0x1234);
    TEST("FSGE", "(18,A2,D3.W)",    0xF240|062, 0x0013, 0x3012);
    ABST("FSGE", "(@A).W", "1234",   0xF240|070, 0x0013, 0x1234);
    ABST("FSGE", "(@A).L", "123456", 0xF240|071, 0x0013, 0x0012, 0x3456);
    TEST("FTRAPGE.W", "#$1234",     0xF240|072, 0x0013, 0x1234);
    TEST("FTRAPGE.L", "#$12345678", 0xF240|073, 0x0013, 0x1234, 0x5678);
    TEST("FTRAPGE",   "",           0xF240|074, 0x0013);

    TEST("FSLT", "D2",              0xF240|002, 0x0014);
    TEST("FDBLT", "D2, *+$1234",    0xF240|012, 0x0014, 0x1230);
    TEST("FSLT", "(A2)",            0xF240|022, 0x0014);
    TEST("FSLT", "(A2)+",           0xF240|032, 0x0014);
    TEST("FSLT", "-(A2)",           0xF240|042, 0x0014);
    TEST("FSLT", "($1234,A2)",      0xF240|052, 0x0014, 0x1234);
    TEST("FSLT", "(18,A2,D3.W)",    0xF240|062, 0x0014, 0x3012);
    ABST("FSLT", "(@A).W", "1234",   0xF240|070, 0x0014, 0x1234);
    ABST("FSLT", "(@A).L", "123456", 0xF240|071, 0x0014, 0x0012, 0x3456);
    TEST("FTRAPLT.W", "#$1234",     0xF240|072, 0x0014, 0x1234);
    TEST("FTRAPLT.L", "#$12345678", 0xF240|073, 0x0014, 0x1234, 0x5678);
    TEST("FTRAPLT",   "",           0xF240|074, 0x0014);

    TEST("FSLE", "D2",              0xF240|002, 0x0015);
    TEST("FDBLE", "D2, *+$1234",    0xF240|012, 0x0015, 0x1230);
    TEST("FSLE", "(A2)",            0xF240|022, 0x0015);
    TEST("FSLE", "(A2)+",           0xF240|032, 0x0015);
    TEST("FSLE", "-(A2)",           0xF240|042, 0x0015);
    TEST("FSLE", "($1234,A2)",      0xF240|052, 0x0015, 0x1234);
    TEST("FSLE", "(18,A2,D3.W)",    0xF240|062, 0x0015, 0x3012);
    ABST("FSLE", "(@A).W", "1234",   0xF240|070, 0x0015, 0x1234);
    ABST("FSLE", "(@A).L", "123456", 0xF240|071, 0x0015, 0x0012, 0x3456);
    TEST("FTRAPLE.W", "#$1234",     0xF240|072, 0x0015, 0x1234);
    TEST("FTRAPLE.L", "#$12345678", 0xF240|073, 0x0015, 0x1234, 0x5678);
    TEST("FTRAPLE",   "",           0xF240|074, 0x0015);

    TEST("FSGL", "D2",              0xF240|002, 0x0016);
    TEST("FDBGL", "D2, *+$1234",    0xF240|012, 0x0016, 0x1230);
    TEST("FSGL", "(A2)",            0xF240|022, 0x0016);
    TEST("FSGL", "(A2)+",           0xF240|032, 0x0016);
    TEST("FSGL", "-(A2)",           0xF240|042, 0x0016);
    TEST("FSGL", "($1234,A2)",      0xF240|052, 0x0016, 0x1234);
    TEST("FSGL", "(18,A2,D3.W)",    0xF240|062, 0x0016, 0x3012);
    ABST("FSGL", "(@A).W", "1234",   0xF240|070, 0x0016, 0x1234);
    ABST("FSGL", "(@A).L", "123456", 0xF240|071, 0x0016, 0x0012, 0x3456);
    TEST("FTRAPGL.W", "#$1234",     0xF240|072, 0x0016, 0x1234);
    TEST("FTRAPGL.L", "#$12345678", 0xF240|073, 0x0016, 0x1234, 0x5678);
    TEST("FTRAPGL",   "",           0xF240|074, 0x0016);

    TEST("FSGLE", "D2",              0xF240|002, 0x0017);
    TEST("FDBGLE", "D2, *+$1234",    0xF240|012, 0x0017, 0x1230);
    TEST("FSGLE", "(A2)",            0xF240|022, 0x0017);
    TEST("FSGLE", "(A2)+",           0xF240|032, 0x0017);
    TEST("FSGLE", "-(A2)",           0xF240|042, 0x0017);
    TEST("FSGLE", "($1234,A2)",      0xF240|052, 0x0017, 0x1234);
    TEST("FSGLE", "(18,A2,D3.W)",    0xF240|062, 0x0017, 0x3012);
    ABST("FSGLE", "(@A).W", "1234",   0xF240|070, 0x0017, 0x1234);
    ABST("FSGLE", "(@A).L", "123456", 0xF240|071, 0x0017, 0x0012, 0x3456);
    TEST("FTRAPGLE.W", "#$1234",     0xF240|072, 0x0017, 0x1234);
    TEST("FTRAPGLE.L", "#$12345678", 0xF240|073, 0x0017, 0x1234, 0x5678);
    TEST("FTRAPGLE",   "",           0xF240|074, 0x0017);

    TEST("FSNGLE", "D2",              0xF240|002, 0x0018);
    TEST("FDBNGLE", "D2, *+$1234",    0xF240|012, 0x0018, 0x1230);
    TEST("FSNGLE", "(A2)",            0xF240|022, 0x0018);
    TEST("FSNGLE", "(A2)+",           0xF240|032, 0x0018);
    TEST("FSNGLE", "-(A2)",           0xF240|042, 0x0018);
    TEST("FSNGLE", "($1234,A2)",      0xF240|052, 0x0018, 0x1234);
    TEST("FSNGLE", "(18,A2,D3.W)",    0xF240|062, 0x0018, 0x3012);
    ABST("FSNGLE", "(@A).W", "1234",   0xF240|070, 0x0018, 0x1234);
    ABST("FSNGLE", "(@A).L", "123456", 0xF240|071, 0x0018, 0x0012, 0x3456);
    TEST("FTRAPNGLE.W", "#$1234",     0xF240|072, 0x0018, 0x1234);
    TEST("FTRAPNGLE.L", "#$12345678", 0xF240|073, 0x0018, 0x1234, 0x5678);
    TEST("FTRAPNGLE",   "",           0xF240|074, 0x0018);

    TEST("FSNGL", "D2",              0xF240|002, 0x0019);
    TEST("FDBNGL", "D2, *+$1234",    0xF240|012, 0x0019, 0x1230);
    TEST("FSNGL", "(A2)",            0xF240|022, 0x0019);
    TEST("FSNGL", "(A2)+",           0xF240|032, 0x0019);
    TEST("FSNGL", "-(A2)",           0xF240|042, 0x0019);
    TEST("FSNGL", "($1234,A2)",      0xF240|052, 0x0019, 0x1234);
    TEST("FSNGL", "(18,A2,D3.W)",    0xF240|062, 0x0019, 0x3012);
    ABST("FSNGL", "(@A).W", "1234",   0xF240|070, 0x0019, 0x1234);
    ABST("FSNGL", "(@A).L", "123456", 0xF240|071, 0x0019, 0x0012, 0x3456);
    TEST("FTRAPNGL.W", "#$1234",     0xF240|072, 0x0019, 0x1234);
    TEST("FTRAPNGL.L", "#$12345678", 0xF240|073, 0x0019, 0x1234, 0x5678);
    TEST("FTRAPNGL",   "",           0xF240|074, 0x0019);

    TEST("FSNLE", "D2",              0xF240|002, 0x001A);
    TEST("FDBNLE", "D2, *+$1234",    0xF240|012, 0x001A, 0x1230);
    TEST("FSNLE", "(A2)",            0xF240|022, 0x001A);
    TEST("FSNLE", "(A2)+",           0xF240|032, 0x001A);
    TEST("FSNLE", "-(A2)",           0xF240|042, 0x001A);
    TEST("FSNLE", "($1234,A2)",      0xF240|052, 0x001A, 0x1234);
    TEST("FSNLE", "(18,A2,D3.W)",    0xF240|062, 0x001A, 0x3012);
    ABST("FSNLE", "(@A).W", "1234",   0xF240|070, 0x001A, 0x1234);
    ABST("FSNLE", "(@A).L", "123456", 0xF240|071, 0x001A, 0x0012, 0x3456);
    TEST("FTRAPNLE.W", "#$1234",     0xF240|072, 0x001A, 0x1234);
    TEST("FTRAPNLE.L", "#$12345678", 0xF240|073, 0x001A, 0x1234, 0x5678);
    TEST("FTRAPNLE",   "",           0xF240|074, 0x001A);

    TEST("FSNLT", "D2",              0xF240|002, 0x001B);
    TEST("FDBNLT", "D2, *+$1234",    0xF240|012, 0x001B, 0x1230);
    TEST("FSNLT", "(A2)",            0xF240|022, 0x001B);
    TEST("FSNLT", "(A2)+",           0xF240|032, 0x001B);
    TEST("FSNLT", "-(A2)",           0xF240|042, 0x001B);
    TEST("FSNLT", "($1234,A2)",      0xF240|052, 0x001B, 0x1234);
    TEST("FSNLT", "(18,A2,D3.W)",    0xF240|062, 0x001B, 0x3012);
    ABST("FSNLT", "(@A).W", "1234",   0xF240|070, 0x001B, 0x1234);
    ABST("FSNLT", "(@A).L", "123456", 0xF240|071, 0x001B, 0x0012, 0x3456);
    TEST("FTRAPNLT.W", "#$1234",     0xF240|072, 0x001B, 0x1234);
    TEST("FTRAPNLT.L", "#$12345678", 0xF240|073, 0x001B, 0x1234, 0x5678);
    TEST("FTRAPNLT",   "",           0xF240|074, 0x001B);

    TEST("FSNGE", "D2",              0xF240|002, 0x001C);
    TEST("FDBNGE", "D2, *+$1234",    0xF240|012, 0x001C, 0x1230);
    TEST("FSNGE", "(A2)",            0xF240|022, 0x001C);
    TEST("FSNGE", "(A2)+",           0xF240|032, 0x001C);
    TEST("FSNGE", "-(A2)",           0xF240|042, 0x001C);
    TEST("FSNGE", "($1234,A2)",      0xF240|052, 0x001C, 0x1234);
    TEST("FSNGE", "(18,A2,D3.W)",    0xF240|062, 0x001C, 0x3012);
    ABST("FSNGE", "(@A).W", "1234",   0xF240|070, 0x001C, 0x1234);
    ABST("FSNGE", "(@A).L", "123456", 0xF240|071, 0x001C, 0x0012, 0x3456);
    TEST("FTRAPNGE.W", "#$1234",     0xF240|072, 0x001C, 0x1234);
    TEST("FTRAPNGE.L", "#$12345678", 0xF240|073, 0x001C, 0x1234, 0x5678);
    TEST("FTRAPNGE",   "",           0xF240|074, 0x001C);

    TEST("FSNGT", "D2",              0xF240|002, 0x001D);
    TEST("FDBNGT", "D2, *+$1234",    0xF240|012, 0x001D, 0x1230);
    TEST("FSNGT", "(A2)",            0xF240|022, 0x001D);
    TEST("FSNGT", "(A2)+",           0xF240|032, 0x001D);
    TEST("FSNGT", "-(A2)",           0xF240|042, 0x001D);
    TEST("FSNGT", "($1234,A2)",      0xF240|052, 0x001D, 0x1234);
    TEST("FSNGT", "(18,A2,D3.W)",    0xF240|062, 0x001D, 0x3012);
    ABST("FSNGT", "(@A).W", "1234",   0xF240|070, 0x001D, 0x1234);
    ABST("FSNGT", "(@A).L", "123456", 0xF240|071, 0x001D, 0x0012, 0x3456);
    TEST("FTRAPNGT.W", "#$1234",     0xF240|072, 0x001D, 0x1234);
    TEST("FTRAPNGT.L", "#$12345678", 0xF240|073, 0x001D, 0x1234, 0x5678);
    TEST("FTRAPNGT",   "",           0xF240|074, 0x001D);

    TEST("FSSNE", "D2",              0xF240|002, 0x001E);
    TEST("FDBSNE", "D2, *+$1234",    0xF240|012, 0x001E, 0x1230);
    TEST("FSSNE", "(A2)",            0xF240|022, 0x001E);
    TEST("FSSNE", "(A2)+",           0xF240|032, 0x001E);
    TEST("FSSNE", "-(A2)",           0xF240|042, 0x001E);
    TEST("FSSNE", "($1234,A2)",      0xF240|052, 0x001E, 0x1234);
    TEST("FSSNE", "(18,A2,D3.W)",    0xF240|062, 0x001E, 0x3012);
    ABST("FSSNE", "(@A).W", "1234",   0xF240|070, 0x001E, 0x1234);
    ABST("FSSNE", "(@A).L", "123456", 0xF240|071, 0x001E, 0x0012, 0x3456);
    TEST("FTRAPSNE.W", "#$1234",     0xF240|072, 0x001E, 0x1234);
    TEST("FTRAPSNE.L", "#$12345678", 0xF240|073, 0x001E, 0x1234, 0x5678);
    TEST("FTRAPSNE",   "",           0xF240|074, 0x001E);

    TEST("FSST", "D2",              0xF240|002, 0x001F);
    TEST("FDBST", "D2, *+$1234",    0xF240|012, 0x001F, 0x1230);
    TEST("FSST", "(A2)",            0xF240|022, 0x001F);
    TEST("FSST", "(A2)+",           0xF240|032, 0x001F);
    TEST("FSST", "-(A2)",           0xF240|042, 0x001F);
    TEST("FSST", "($1234,A2)",      0xF240|052, 0x001F, 0x1234);
    TEST("FSST", "(18,A2,D3.W)",    0xF240|062, 0x001F, 0x3012);
    ABST("FSST", "(@A).W", "1234",   0xF240|070, 0x001F, 0x1234);
    ABST("FSST", "(@A).L", "123456", 0xF240|071, 0x001F, 0x0012, 0x3456);
    TEST("FTRAPST.W", "#$1234",     0xF240|072, 0x001F, 0x1234);
    TEST("FTRAPST.L", "#$12345678", 0xF240|073, 0x001F, 0x1234, 0x5678);
    TEST("FTRAPST",   "",           0xF240|074, 0x001F);
}

void test_float_system() {
    UNKN(0xF340|002);
    UNKN(0xF340|014);
    TEST("FRESTORE", "(A6)",           0xF340|026);
    TEST("FRESTORE", "(A0)+",          0xF340|030);
    UNKN(0xF340|042);
    TEST("FRESTORE", "($1234,A4)",     0xF340|054, 0x1234);
    TEST("FRESTORE", "($23,A6,D7.W)",  0xF340|066, 0x7023);
    ABST("FRESTORE", "(@A).W", "4566",   0xF340|070, 0x4566);
    ABST("FRESTORE", "(@A).L", "56789A", 0xF340|071, 0x0056, 0x789A);
    TEST("FRESTORE", "(*+$1234,PC)",   0xF340|072, 0x1232);
    TEST("FRESTORE", "(*+90,PC,A4.L)", 0xF340|073, 0xC858);
    UNKN(0xF340|074);

    UNKN(0xF300|002);
    UNKN(0xF300|014);
    TEST("FSAVE", "(A6)",           0xF300|026);
    UNKN(0xF300|030);
    TEST("FSAVE", "-(A2)",          0xF300|042);
    TEST("FSAVE", "($1234,A4)",     0xF300|054, 0x1234);
    TEST("FSAVE", "($23,A6,D7.W)",  0xF300|066, 0x7023);
    ABST("FSAVE", "(@A).W", "4566",   0xF300|070, 0x4566);
    ABST("FSAVE", "(@A).L", "56789A", 0xF300|071, 0x0056, 0x789A);
    UNKN(0xF300|072);
    UNKN(0xF300|073);
    UNKN(0xF300|074);

    TEST("FADD.X", "FP0, FP1", 0171000, 0x0022|(0<<10)|(1<<7));
    dis68000.setFpuCid(0);
    TEST("FADD.X", "FP0, FP1", 0171000, 0x0022|(0<<10)|(1<<7));
    dis68000.setFpuCid(8);
    TEST("FADD.X", "FP0, FP1", 0171000, 0x0022|(0<<10)|(1<<7));
    dis68000.setFpuCid(2);
    TEST("FADD.X", "FP0, FP1", 0172000, 0x0022|(0<<10)|(1<<7));
    dis68000.setFpuCid(7);
    TEST("FADD.X", "FP0, FP1", 0177000, 0x0022|(0<<10)|(1<<7));
}

#endif

#if !defined(LIBASM_MC68000_NOPMMU)
void test_pmove_mc68030() {
    disassembler.setOption("pmmu", "off");
    // MC68EC030
    TEST("PMOVE.W", "(A2), ACUSR", 0xF000|022, 0x6000|(8<<10));
    TEST("PMOVE.L", "(A2), AC0",   0xF000|022, 0x0000|(1<<10));
    TEST("PMOVE.L", "(A2), AC1",   0xF000|022, 0x0000|(3<<10));
    TEST("PMOVE.W", "($1234,A2), ACUSR", 0xF000|052, 0x6000|(8<<10), 0x1234);
    TEST("PMOVE.L", "($1234,A2), AC0",   0xF000|052, 0x0000|(1<<10), 0x1234);
    TEST("PMOVE.L", "($1234,A2), AC1",   0xF000|052, 0x0000|(3<<10), 0x1234);
    TEST("PMOVE.W", "($34,A2,D3.W), ACUSR", 0xF000|062, 0x6000|(8<<10), 0x3034);
    TEST("PMOVE.L", "($34,A2,D3.W), AC0",   0xF000|062, 0x0000|(1<<10), 0x3034);
    TEST("PMOVE.L", "($34,A2,D3.W), AC1",   0xF000|062, 0x0000|(3<<10), 0x3034);
    TEST("PMOVE.W", "($00001234).W, ACUSR", 0xF000|070, 0x6000|(8<<10), 0x1234);
    TEST("PMOVE.L", "($00001234).W, AC0",   0xF000|070, 0x0000|(1<<10), 0x1234);
    TEST("PMOVE.L", "($00001234).W, AC1",   0xF000|070, 0x0000|(3<<10), 0x1234);
    TEST("PMOVE.W", "($12345678).L, ACUSR", 0xF000|071, 0x6000|(8<<10), 0x1234, 0x5678);
    TEST("PMOVE.L", "($12345678).L, AC0",   0xF000|071, 0x0000|(1<<10), 0x1234, 0x5678);
    TEST("PMOVE.L", "($12345678).L, AC1",   0xF000|071, 0x0000|(3<<10), 0x1234, 0x5678);

    TEST("PMOVE.W", "ACUSR, (A2)", 0xF000|022, 0x6200|(8<<10));
    TEST("PMOVE.L", "AC0, (A2)",   0xF000|022, 0x0200|(1<<10));
    TEST("PMOVE.L", "AC1, (A2)",   0xF000|022, 0x0200|(3<<10));
    TEST("PMOVE.W", "ACUSR, ($1234,A2)", 0xF000|052, 0x6200|(8<<10), 0x1234);
    TEST("PMOVE.L", "AC0, ($1234,A2)"  , 0xF000|052, 0x0200|(1<<10), 0x1234);
    TEST("PMOVE.L", "AC1, ($1234,A2)",   0xF000|052, 0x0200|(3<<10), 0x1234);
    TEST("PMOVE.W", "ACUSR, ($34,A2,D3.W)", 0xF000|062, 0x6200|(8<<10), 0x3034);
    TEST("PMOVE.L", "AC0, ($34,A2,D3.W)",   0xF000|062, 0x0200|(1<<10), 0x3034);
    TEST("PMOVE.L", "AC1, ($34,A2,D3.W)",   0xF000|062, 0x0200|(3<<10), 0x3034);
    TEST("PMOVE.W", "ACUSR, ($00001234).W", 0xF000|070, 0x6200|(8<<10), 0x1234);
    TEST("PMOVE.L", "AC0, ($00001234).W",   0xF000|070, 0x0200|(1<<10), 0x1234);
    TEST("PMOVE.L", "AC1, ($00001234).W",   0xF000|070, 0x0200|(3<<10), 0x1234);
    TEST("PMOVE.W", "ACUSR, ($12345678).L", 0xF000|071, 0x6200|(8<<10), 0x1234, 0x5678);
    TEST("PMOVE.L", "AC0, ($12345678).L",   0xF000|071, 0x0200|(1<<10), 0x1234, 0x5678);
    TEST("PMOVE.L", "AC1, ($12345678).L",   0xF000|071, 0x0200|(3<<10), 0x1234, 0x5678);

    UNKN(0xF000|002);   // D2
    UNKN(0xF000|012);   // A2
    UNKN(0xF000|032);   // (A2)+
    UNKN(0xF000|042);   // -(A2)
    UNKN(0xF000|072);   // (d16,PC)
    UNKN(0xF000|073);   // (d8,PC,Xn)
    UNKN(0xF000|074);   // #n

    TEST("PTESTR", "#7, (A2)",           0xF000|022, 0x8210|7);
    TEST("PTESTR", "D3, ($1234,A2)",     0xF000|052, 0x8208|3, 0x1234);
    TEST("PTESTR", "SFC, ($34,A2,D3.W)", 0xF000|062, 0x8200|0, 0x3034);
    TEST("PTESTR", "DFC, ($00001234).W", 0xF000|070, 0x8200|1, 0x1234);
    TEST("PTESTR", "#7, ($12345678).L",  0xF000|071, 0x8210|7, 0x1234, 0x5678);

    TEST("PTESTW", "#7, (A2)",           0xF000|022, 0x8010|7);
    TEST("PTESTW", "D3, ($1234,A2)",     0xF000|052, 0x8008|3, 0x1234);
    TEST("PTESTW", "SFC, ($34,A2,D3.W)", 0xF000|062, 0x8000|0, 0x3034);
    TEST("PTESTW", "DFC, ($00001234).W", 0xF000|070, 0x8000|1, 0x1234);
    TEST("PTESTW", "#3, ($12345678).L",  0xF000|071, 0x8010|3, 0x1234, 0x5678);

    disassembler.setOption("pmmu", "on");
    // MC68030
    UNKN(0xF000|002, 0x4000|(0<<10)); // Dn
    UNKN(0xF000|012, 0x4000|(0<<10)); // An
    UNKN(0xF000|032, 0x4000|(0<<10)); // (An)+
    UNKN(0xF000|042, 0x4000|(0<<10)); // -(An)
    UNKN(0xF000|072, 0x4000|(0<<10)); // (d16,pc)
    UNKN(0xF000|073, 0x4000|(0<<10)); // (d8,pc,Xn)
    UNKN(0xF000|074, 0x4000|(0<<10)); // #nn

    TEST("PMOVE.L", "(A2), TC",   0xF000|022, 0x4000|(0<<10));
    TEST("PMOVE.Q", "(A2), SRP",  0xF000|022, 0x4000|(2<<10));
    TEST("PMOVE.Q", "(A2), CRP",  0xF000|022, 0x4000|(3<<10));
    TEST("PMOVE.W", "(A2), PSR",  0xF000|022, 0x6000|(8<<10));
    TEST("PMOVE.L", "(A2), TT0",  0xF000|022, 0x0000|(2<<10));
    TEST("PMOVE.L", "(A2), TT1",  0xF000|022, 0x0000|(3<<10));

    TEST("PMOVE.L", "($1234,A2), TC",  0xF000|052, 0x4000|(0<<10), 0x1234);
    TEST("PMOVE.Q", "($1234,A2), SRP", 0xF000|052, 0x4000|(2<<10), 0x1234);
    TEST("PMOVE.Q", "($1234,A2), CRP", 0xF000|052, 0x4000|(3<<10), 0x1234);
    TEST("PMOVE.W", "($1234,A2), PSR", 0xF000|052, 0x6000|(8<<10), 0x1234);
    TEST("PMOVE.L", "($1234,A2), TT0", 0xF000|052, 0x0000|(2<<10), 0x1234);
    TEST("PMOVE.L", "($1234,A2), TT1", 0xF000|052, 0x0000|(3<<10), 0x1234);

    TEST("PMOVE.L", "($34,A2,D3.W), TC",  0xF000|062, 0x4000|(0<<10), 0x3034);
    TEST("PMOVE.Q", "($34,A2,D3.W), SRP", 0xF000|062, 0x4000|(2<<10), 0x3034);
    TEST("PMOVE.Q", "($34,A2,D3.W), CRP", 0xF000|062, 0x4000|(3<<10), 0x3034);
    TEST("PMOVE.W", "($34,A2,D3.W), PSR", 0xF000|062, 0x6000|(8<<10), 0x3034);
    TEST("PMOVE.L", "($34,A2,D3.W), TT0", 0xF000|062, 0x0000|(2<<10), 0x3034);
    TEST("PMOVE.L", "($34,A2,D3.W), TT1", 0xF000|062, 0x0000|(3<<10), 0x3034);

    TEST("PMOVE.L", "($00001234).W, TC",  0xF000|070, 0x4000|(0<<10), 0x1234);
    TEST("PMOVE.Q", "($00001234).W, SRP", 0xF000|070, 0x4000|(2<<10), 0x1234);
    TEST("PMOVE.Q", "($00001234).W, CRP", 0xF000|070, 0x4000|(3<<10), 0x1234);
    TEST("PMOVE.W", "($00001234).W, PSR", 0xF000|070, 0x6000|(8<<10), 0x1234);
    TEST("PMOVE.L", "($00001234).W, TT0", 0xF000|070, 0x0000|(2<<10), 0x1234);
    TEST("PMOVE.L", "($00001234).W, TT1", 0xF000|070, 0x0000|(3<<10), 0x1234);

    TEST("PMOVE.L", "($12345678).L, TC",  0xF000|071, 0x4000|(0<<10), 0x1234, 0x5678);
    TEST("PMOVE.Q", "($12345678).L, SRP", 0xF000|071, 0x4000|(2<<10), 0x1234, 0x5678);
    TEST("PMOVE.Q", "($12345678).L, CRP", 0xF000|071, 0x4000|(3<<10), 0x1234, 0x5678);
    TEST("PMOVE.W", "($12345678).L, PSR", 0xF000|071, 0x6000|(8<<10), 0x1234, 0x5678);
    TEST("PMOVE.L", "($12345678).L, TT0", 0xF000|071, 0x0000|(2<<10), 0x1234, 0x5678);
    TEST("PMOVE.L", "($12345678).L, TT1", 0xF000|071, 0x0000|(3<<10), 0x1234, 0x5678);

    UNKN(0xF000|002, 0x4100|(0<<10)); // Dn
    UNKN(0xF000|012, 0x4100|(0<<10)); // An
    UNKN(0xF000|032, 0x4100|(0<<10)); // Dn
    UNKN(0xF000|042, 0x4100|(0<<10)); // An
    UNKN(0xF000|072, 0x4100|(0<<10)); // (d16,PC)
    UNKN(0xF000|073, 0x4100|(0<<10)); // (D8,PC,Xn)
    UNKN(0xF000|074, 0x4100|(0<<10)); // #nn

    TEST("PMOVEFD.L", "(A2), TC",  0xF000|022, 0x4100|(0<<10));
    TEST("PMOVEFD.Q", "(A2), SRP", 0xF000|022, 0x4100|(2<<10));
    TEST("PMOVEFD.Q", "(A2), CRP", 0xF000|022, 0x4100|(3<<10));
    UNKN(0xF000|022, 0x6100|(8<<10));
    TEST("PMOVEFD.L", "(A2), TT0", 0xF000|022, 0x0100|(2<<10));
    TEST("PMOVEFD.L", "(A2), TT1", 0xF000|022, 0x0100|(3<<10));

    TEST("PMOVEFD.L", "($1234,A2), TC",  0xF000|052, 0x4100|(0<<10), 0x1234);
    TEST("PMOVEFD.Q", "($1234,A2), SRP", 0xF000|052, 0x4100|(2<<10), 0x1234);
    TEST("PMOVEFD.Q", "($1234,A2), CRP", 0xF000|052, 0x4100|(3<<10), 0x1234);
    UNKN(0xF000|052, 0x6100|(8<<10));
    TEST("PMOVEFD.L", "($1234,A2), TT0", 0xF000|052, 0x0100|(2<<10), 0x1234);
    TEST("PMOVEFD.L", "($1234,A2), TT1", 0xF000|052, 0x0100|(3<<10), 0x1234);

    TEST("PMOVEFD.L", "($34,A2,D3.W), TC",  0xF000|062, 0x4100|(0<<10), 0x3034);
    TEST("PMOVEFD.Q", "($34,A2,D3.W), SRP", 0xF000|062, 0x4100|(2<<10), 0x3034);
    TEST("PMOVEFD.Q", "($34,A2,D3.W), CRP", 0xF000|062, 0x4100|(3<<10), 0x3034);
    UNKN(0xF000|062, 0x6100|(8<<10));
    TEST("PMOVEFD.L", "($34,A2,D3.W), TT0", 0xF000|062, 0x0100|(2<<10), 0x3034);
    TEST("PMOVEFD.L", "($34,A2,D3.W), TT1", 0xF000|062, 0x0100|(3<<10), 0x3034);

    TEST("PMOVEFD.L", "($00001234).W, TC",  0xF000|070, 0x4100|(0<<10), 0x1234);
    TEST("PMOVEFD.Q", "($00001234).W, SRP", 0xF000|070, 0x4100|(2<<10), 0x1234);
    TEST("PMOVEFD.Q", "($00001234).W, CRP", 0xF000|070, 0x4100|(3<<10), 0x1234);
    UNKN(0xF000|070, 0x6100|(8<<10));
    TEST("PMOVEFD.L", "($00001234).W, TT0", 0xF000|070, 0x0100|(2<<10), 0x1234);
    TEST("PMOVEFD.L", "($00001234).W, TT1", 0xF000|070, 0x0100|(3<<10), 0x1234);

    TEST("PMOVEFD.L", "($12345678).L, TC",  0xF000|071, 0x4100|(0<<10), 0x1234, 0x5678);
    TEST("PMOVEFD.Q", "($12345678).L, SRP", 0xF000|071, 0x4100|(2<<10), 0x1234, 0x5678);
    TEST("PMOVEFD.Q", "($12345678).L, CRP", 0xF000|071, 0x4100|(3<<10), 0x1234, 0x5678);
    UNKN(0xF000|071, 0x6100|(8<<10));
    TEST("PMOVEFD.L", "($12345678).L, TT0", 0xF000|071, 0x0100|(2<<10), 0x1234, 0x5678);
    TEST("PMOVEFD.L", "($12345678).L, TT1", 0xF000|071, 0x0100|(3<<10), 0x1234, 0x5678);

    UNKN(0xF000|002, 0x4200|(0<<10)); // Dn
    UNKN(0xF000|012, 0x4200|(0<<10)); // An
    UNKN(0xF000|032, 0x4200|(0<<10)); // (An)+
    UNKN(0xF000|042, 0x4200|(0<<10)); // -(An)
    UNKN(0xF000|072, 0x4200|(0<<10)); // (d16,PC)
    UNKN(0xF000|073, 0x4200|(0<<10)); // (d8,PC,Xn)
    UNKN(0xF000|074, 0x4200|(0<<10)); // #nn

    TEST("PMOVE.L", "TC, (A2)",  0xF000|022, 0x4200|(0<<10));
    TEST("PMOVE.Q", "SRP, (A2)", 0xF000|022, 0x4200|(2<<10));
    TEST("PMOVE.Q", "CRP, (A2)", 0xF000|022, 0x4200|(3<<10));
    TEST("PMOVE.W", "PSR, (A2)", 0xF000|022, 0x6200|(8<<10));
    TEST("PMOVE.L", "TT0, (A2)", 0xF000|022, 0x0200|(2<<10));
    TEST("PMOVE.L", "TT1, (A2)", 0xF000|022, 0x0200|(3<<10));

    TEST("PMOVE.L", "TC, ($1234,A2)",  0xF000|052, 0x4200|(0<<10), 0x1234);
    TEST("PMOVE.Q", "SRP, ($1234,A2)", 0xF000|052, 0x4200|(2<<10), 0x1234);
    TEST("PMOVE.Q", "CRP, ($1234,A2)", 0xF000|052, 0x4200|(3<<10), 0x1234);
    TEST("PMOVE.W", "PSR, ($1234,A2)", 0xF000|052, 0x6200|(8<<10), 0x1234);
    TEST("PMOVE.L", "TT0, ($1234,A2)", 0xF000|052, 0x0200|(2<<10), 0x1234);
    TEST("PMOVE.L", "TT1, ($1234,A2)", 0xF000|052, 0x0200|(3<<10), 0x1234);

    TEST("PMOVE.L", "TC, ($34,A2,D3.W)",  0xF000|062, 0x4200|(0<<10), 0x3034);
    TEST("PMOVE.Q", "SRP, ($34,A2,D3.W)", 0xF000|062, 0x4200|(2<<10), 0x3034);
    TEST("PMOVE.Q", "CRP, ($34,A2,D3.W)", 0xF000|062, 0x4200|(3<<10), 0x3034);
    TEST("PMOVE.W", "PSR, ($34,A2,D3.W)", 0xF000|062, 0x6200|(8<<10), 0x3034);
    TEST("PMOVE.L", "TT0, ($34,A2,D3.W)", 0xF000|062, 0x0200|(2<<10), 0x3034);
    TEST("PMOVE.L", "TT1, ($34,A2,D3.W)", 0xF000|062, 0x0200|(3<<10), 0x3034);

    TEST("PMOVE.L", "TC, ($00001234).W",  0xF000|070, 0x4200|(0<<10), 0x1234);
    TEST("PMOVE.Q", "SRP, ($00001234).W", 0xF000|070, 0x4200|(2<<10), 0x1234);
    TEST("PMOVE.Q", "CRP, ($00001234).W", 0xF000|070, 0x4200|(3<<10), 0x1234);
    TEST("PMOVE.W", "PSR, ($00001234).W", 0xF000|070, 0x6200|(8<<10), 0x1234);
    TEST("PMOVE.L", "TT0, ($00001234).W", 0xF000|070, 0x0200|(2<<10), 0x1234);
    TEST("PMOVE.L", "TT1, ($00001234).W", 0xF000|070, 0x0200|(3<<10), 0x1234);

    TEST("PMOVE.L", "TC, ($12345678).L",  0xF000|071, 0x4200|(0<<10), 0x1234, 0x5678);
    TEST("PMOVE.Q", "SRP, ($12345678).L", 0xF000|071, 0x4200|(2<<10), 0x1234, 0x5678);
    TEST("PMOVE.Q", "CRP, ($12345678).L", 0xF000|071, 0x4200|(3<<10), 0x1234, 0x5678);
    TEST("PMOVE.W", "PSR, ($12345678).L", 0xF000|071, 0x6200|(8<<10), 0x1234, 0x5678);
    TEST("PMOVE.L", "TT0, ($12345678).L", 0xF000|071, 0x0200|(2<<10), 0x1234, 0x5678);
    TEST("PMOVE.L", "TT1, ($12345678).L", 0xF000|071, 0x0200|(3<<10), 0x1234, 0x5678);
}

void test_pmmu_mc68030() {
    TEST("PFLUSHA", "",        0xF000, 0x2400);

    TEST("PFLUSH",  "#7, #4",  0xF000, 0x3010|(4<<5)|7);
    ERRT("PFLUSH",  "#8, #4",  OVERFLOW_RANGE, "#8, #4", 0xF000, 0x3010|(4<<5)|8);
    TEST("PFLUSH",  "D4, #7",  0xF000, 0x3008|(7<<5)|4);
    ERRT("PFLUSH",  "D4, #8",  OVERFLOW_RANGE, "#8",     0xF000, 0x3008|(8<<5)|4);
    TEST("PFLUSH",  "SFC, #1", 0xF000, 0x3000|(1<<5)|0);
    TEST("PFLUSH",  "DFC, #7", 0xF000, 0x3000|(7<<5)|1);

    UNKN(0xF000|002, 0x3800);   // D2
    UNKN(0xF000|012, 0x3800);   // A2
    TEST("PFLUSH",  "#7, #4, (A2)",       0xF000|022, 0x3810|(4<<5)|7);
    ERRT("PFLUSH",  "#15, #4, (A2)",
         OVERFLOW_RANGE, "#15, #4, (A2)", 0xF000|022, 0x3810|(4<<5)|15);
    UNKN(0xF000|032, 0x3800);   // (A2)+
    UNKN(0xF000|042, 0x3800);   // -(A2)
    TEST("PFLUSH",  "D4, #7, ($1234,A2)",     0xF000|052, 0x3808|(7<<5)|4, 0x1234);
    TEST("PFLUSH",  "SFC, #1, ($34,A2,D3.W)", 0xF000|062, 0x3800|(1<<5)|0, 0x3034);
    TEST("PFLUSH",  "DFC, #1, ($00001234).W", 0xF000|070, 0x3800|(1<<5)|1, 0x1234);
    TEST("PFLUSH",  "#7, #4, ($12345678).L",  0xF000|071, 0x3810|(4<<5)|7, 0x1234, 0x5678);
    UNKN(0xF000|072, 0x3800);   // (d16,PC)
    UNKN(0xF000|073, 0x3800);   // (d8,PC,Xn)
    UNKN(0xF000|074, 0x3800);   // #n

    UNKN(0xF000|002, 0x2200);   // D2
    UNKN(0xF000|012, 0x2200);   // A2
    TEST("PLOADR", "#4, (A2)",           0xF000|022, 0x2210|4);
    UNKN(0xF000|032, 0x2200);   // (A2)+
    UNKN(0xF000|042, 0x2200);   // -(A2)
    TEST("PLOADR", "D3, ($1234,A2)",     0xF000|052, 0x2208|3, 0x1234);
    TEST("PLOADR", "SFC, ($34,A2,D3.W)", 0xF000|062, 0x2200|0, 0x3034);
    TEST("PLOADR", "DFC, ($00001234).W", 0xF000|070, 0x2200|1, 0x1234);
    TEST("PLOADR", "#4, ($12345678).L",  0xF000|071, 0x2210|4, 0x1234, 0x5678);
    UNKN(0xF000|072, 0x2200);   // (d16,PC)
    UNKN(0xF000|073, 0x2200);   // (d8,PC,Xn)
    UNKN(0xF000|074, 0x2200);   // #n

    UNKN(0xF000|002, 0x2000);   // D2
    UNKN(0xF000|012, 0x2000);   // A2
    TEST("PLOADW", "#7, (A2)",           0xF000|022, 0x2010|7);
    UNKN(0xF000|032, 0x2000);   // (A2)+
    UNKN(0xF000|042, 0x2000);   // -(A2)
    TEST("PLOADW", "D3, ($1234,A2)",     0xF000|052, 0x2008|3, 0x1234);
    TEST("PLOADW", "SFC, ($34,A2,D3.W)", 0xF000|062, 0x2000|0, 0x3034);
    TEST("PLOADW", "DFC, ($00001234).W", 0xF000|070, 0x2000|1, 0x1234);
    TEST("PLOADW", "#4, ($12345678).L",  0xF000|071, 0x2010|4, 0x1234, 0x5678);
    UNKN(0xF000|072, 0x2000);   // (d16,PC)
    UNKN(0xF000|073, 0x2000);   // (d8,PC,Xn)
    UNKN(0xF000|074, 0x2000);   // #n

    UNKN(0xF000|002, 0x8200);   // D2
    UNKN(0xF000|012, 0x8200);   // A2
    TEST("PTESTR", "#7, (A2), #4",           0xF000|022, 0x8210|(4<<10)|7);
    UNKN(0xF000|032, 0x8200);   // (A2)+
    UNKN(0xF000|042, 0x8200);   // -(A2)
    TEST("PTESTR", "D3, ($1234,A2), #7",     0xF000|052, 0x8208|(7<<10)|3, 0x1234);
    TEST("PTESTR", "SFC, ($34,A2,D3.W), #1", 0xF000|062, 0x8200|(1<<10)|0, 0x3034);
    TEST("PTESTR", "DFC, ($00001234).W, #5", 0xF000|070, 0x8200|(5<<10)|1, 0x1234);
    TEST("PTESTR", "#7, ($12345678).L, #2",  0xF000|071, 0x8210|(2<<10)|7, 0x1234, 0x5678);
    UNKN(0xF000|072, 0x8200);   // (d16,PC)
    UNKN(0xF000|073, 0x8200);   // (d8,PC,Xn)
    UNKN(0xF000|074, 0x8200);   // #n

    UNKN(0xF000|002, 0x8300);   // D2
    UNKN(0xF000|012, 0x8300);   // A2
    TEST("PTESTR", "#7, (A2), #4, A3",           0xF000|022, 0x8310|(4<<10)|7|(3<<5));
    UNKN(0xF000|032, 0x8300);   // (A2)+
    UNKN(0xF000|042, 0x8300);   // -(A2)
    TEST("PTESTR", "D3, ($1234,A2), #7, A4",     0xF000|052, 0x8308|(7<<10)|3|(4<<5), 0x1234);
    TEST("PTESTR", "SFC, ($34,A2,D3.W), #1, A3", 0xF000|062, 0x8300|(1<<10)|0|(3<<5), 0x3034);
    TEST("PTESTR", "DFC, ($00001234).W, #5, A3", 0xF000|070, 0x8300|(5<<10)|1|(3<<5), 0x1234);
    TEST("PTESTR", "#6, ($12345678).L, #2, A3",  0xF000|071, 0x8310|(2<<10)|6|(3<<5),
         0x1234, 0x5678);
    UNKN(0xF000|072, 0x8300);   // (d16,PC)
    UNKN(0xF000|073, 0x8300);   // (d8,PC,Xn)
    UNKN(0xF000|074, 0x8300);   // #n

    UNKN(0xF000|002, 0x8000);   // D2
    UNKN(0xF000|012, 0x8000);   // A2
    TEST("PTESTW", "#7, (A2), #4",           0xF000|022, 0x8010|(4<<10)|7);
    UNKN(0xF000|032, 0x8000);   // (A2)+
    UNKN(0xF000|042, 0x8000);   // -(A2)
    TEST("PTESTW", "D3, ($1234,A2), #7",     0xF000|052, 0x8008|(7<<10)|3, 0x1234);
    TEST("PTESTW", "SFC, ($34,A2,D3.W), #1", 0xF000|062, 0x8000|(1<<10)|0, 0x3034);
    TEST("PTESTW", "DFC, ($00001234).W, #5", 0xF000|070, 0x8000|(5<<10)|1, 0x1234);
    TEST("PTESTW", "#3, ($12345678).L, #2",  0xF000|071, 0x8010|(2<<10)|3, 0x1234, 0x5678);
    UNKN(0xF000|072, 0x8000);   // (d16,PC)
    UNKN(0xF000|073, 0x8000);   // (d8,PC,Xn)
    UNKN(0xF000|074, 0x8000);   // #n

    UNKN(0xF000|002, 0x8100);   // D2
    UNKN(0xF000|012, 0x8100);   // A2
    TEST("PTESTW", "#7, (A2), #4, A3",           0xF000|022, 0x8110|(4<<10)|7|(3<<5));
    UNKN(0xF000|032, 0x8100);   // (A2)+
    UNKN(0xF000|042, 0x8100);   // -(A2)
    TEST("PTESTW", "D3, ($1234,A2), #7, A4",     0xF000|052, 0x8108|(7<<10)|3|(4<<5), 0x1234);
    TEST("PTESTW", "SFC, ($34,A2,D3.W), #1, A3", 0xF000|062, 0x8100|(1<<10)|0|(3<<5), 0x3034);
    TEST("PTESTW", "DFC, ($00001234).W, #5, A3", 0xF000|070, 0x8100|(5<<10)|1|(3<<5), 0x1234);
    TEST("PTESTW", "#1, ($12345678).L, #2, A3",  0xF000|071, 0x8110|(2<<10)|1|(3<<5),
         0x1234, 0x5678);
    UNKN(0xF000|072, 0x8100);   // (d16,PC)
    UNKN(0xF000|073, 0x8100);   // (d8,PC,Xn)
    UNKN(0xF000|074, 0x8100);   // #n
}

void test_pmove_mc68851() {
    TEST("PMOVE.L", "D2, TC",   0xF000|002, 0x4000|(0<<10));
    ERRT("PMOVE.Q", "D2, DRP", ILLEGAL_SIZE, "D2, DRP", 0xF000|002, 0x4000|(1<<10));
    ERRT("PMOVE.Q", "D2, SRP", ILLEGAL_SIZE, "D2, SRP", 0xF000|002, 0x4000|(2<<10));
    ERRT("PMOVE.Q", "D2, CRP", ILLEGAL_SIZE, "D2, CRP", 0xF000|002, 0x4000|(3<<10));
    TEST("PMOVE.B", "D2, CAL",  0xF000|002, 0x4000|(4<<10));
    TEST("PMOVE.B", "D2, VAL",  0xF000|002, 0x4000|(5<<10));
    TEST("PMOVE.B", "D2, SCC",  0xF000|002, 0x4000|(6<<10));
    TEST("PMOVE.W", "D2, AC",   0xF000|002, 0x4000|(7<<10));
    TEST("PMOVE.W", "D2, PSR",  0xF000|002, 0x4000|(8<<10));
    TEST("PMOVE.W", "D2, PCSR", 0xF000|002, 0x4000|(9<<10));
    ERRT("PMOVE",   "D2, ", ILLEGAL_REGISTER, "", 0xF000|002, 0x4000|(10<<10));
    ERRT("PMOVE",   "D2, ", ILLEGAL_REGISTER, "", 0xF000|002, 0x4000|(11<<10));
    TEST("PMOVE.W", "D2, BAD0", 0xF000|002, 0x4000|(12<<10)|(0<<2));
    TEST("PMOVE.W", "D2, BAD7", 0xF000|002, 0x4000|(12<<10)|(7<<2));
    TEST("PMOVE.W", "D2, BAC0", 0xF000|002, 0x4000|(13<<10)|(0<<2));
    TEST("PMOVE.W", "D2, BAC7", 0xF000|002, 0x4000|(13<<10)|(7<<2));

    TEST("PMOVE.L", "A2, TC",   0xF000|012, 0x4000|(0<<10));
    ERRT("PMOVE.Q", "A2, DRP", ILLEGAL_SIZE, "A2, DRP", 0xF000|012, 0x4000|(1<<10));
    ERRT("PMOVE.Q", "A2, SRP", ILLEGAL_SIZE, "A2, SRP", 0xF000|012, 0x4000|(2<<10));
    ERRT("PMOVE.Q", "A2, CRP", ILLEGAL_SIZE, "A2, CRP", 0xF000|012, 0x4000|(3<<10));
    TEST("PMOVE.B", "A2, CAL",  0xF000|012, 0x4000|(4<<10));
    TEST("PMOVE.B", "A2, VAL",  0xF000|012, 0x4000|(5<<10));
    TEST("PMOVE.B", "A2, SCC",  0xF000|012, 0x4000|(6<<10));
    TEST("PMOVE.W", "A2, AC",   0xF000|012, 0x4000|(7<<10));
    TEST("PMOVE.W", "A2, PSR",  0xF000|012, 0x4000|(8<<10));
    TEST("PMOVE.W", "A2, PCSR", 0xF000|012, 0x4000|(9<<10));
    TEST("PMOVE.W", "A2, BAD0", 0xF000|012, 0x4000|(12<<10)|(0<<2));
    TEST("PMOVE.W", "A2, BAC7", 0xF000|012, 0x4000|(13<<10)|(7<<2));

    TEST("PMOVE.L", "(A2), TC",   0xF000|022, 0x4000|(0<<10));
    TEST("PMOVE.Q", "(A2), DRP",  0xF000|022, 0x4000|(1<<10));
    TEST("PMOVE.Q", "(A2), SRP",  0xF000|022, 0x4000|(2<<10));
    TEST("PMOVE.Q", "(A2), CRP",  0xF000|022, 0x4000|(3<<10));
    TEST("PMOVE.B", "(A2), CAL",  0xF000|022, 0x4000|(4<<10));
    TEST("PMOVE.B", "(A2), VAL",  0xF000|022, 0x4000|(5<<10));
    TEST("PMOVE.B", "(A2), SCC",  0xF000|022, 0x4000|(6<<10));
    TEST("PMOVE.W", "(A2), AC",   0xF000|022, 0x4000|(7<<10));
    TEST("PMOVE.W", "(A2), PSR",  0xF000|022, 0x4000|(8<<10));
    TEST("PMOVE.W", "(A2), PCSR", 0xF000|022, 0x4000|(9<<10));
    TEST("PMOVE.W", "(A2), BAD0", 0xF000|022, 0x4000|(12<<10)|(0<<2));
    TEST("PMOVE.W", "(A2), BAC7", 0xF000|022, 0x4000|(13<<10)|(7<<2));

    TEST("PMOVE.L", "(A2)+, TC",   0xF000|032, 0x4000|(0<<10));
    TEST("PMOVE.Q", "(A2)+, DRP",  0xF000|032, 0x4000|(1<<10));
    TEST("PMOVE.Q", "(A2)+, SRP",  0xF000|032, 0x4000|(2<<10));
    TEST("PMOVE.Q", "(A2)+, CRP",  0xF000|032, 0x4000|(3<<10));
    TEST("PMOVE.B", "(A2)+, CAL",  0xF000|032, 0x4000|(4<<10));
    TEST("PMOVE.B", "(A2)+, VAL",  0xF000|032, 0x4000|(5<<10));
    TEST("PMOVE.B", "(A2)+, SCC",  0xF000|032, 0x4000|(6<<10));
    TEST("PMOVE.W", "(A2)+, AC",   0xF000|032, 0x4000|(7<<10));
    TEST("PMOVE.W", "(A2)+, PSR",  0xF000|032, 0x4000|(8<<10));
    TEST("PMOVE.W", "(A2)+, PCSR", 0xF000|032, 0x4000|(9<<10));
    TEST("PMOVE.W", "(A2)+, BAD0", 0xF000|032, 0x4000|(12<<10)|(0<<2));
    TEST("PMOVE.W", "(A2)+, BAC7", 0xF000|032, 0x4000|(13<<10)|(7<<2));

    TEST("PMOVE.L", "-(A2), TC",   0xF000|042, 0x4000|(0<<10));
    TEST("PMOVE.Q", "-(A2), DRP",  0xF000|042, 0x4000|(1<<10));
    TEST("PMOVE.Q", "-(A2), SRP",  0xF000|042, 0x4000|(2<<10));
    TEST("PMOVE.Q", "-(A2), CRP",  0xF000|042, 0x4000|(3<<10));
    TEST("PMOVE.B", "-(A2), CAL",  0xF000|042, 0x4000|(4<<10));
    TEST("PMOVE.B", "-(A2), VAL",  0xF000|042, 0x4000|(5<<10));
    TEST("PMOVE.B", "-(A2), SCC",  0xF000|042, 0x4000|(6<<10));
    TEST("PMOVE.W", "-(A2), AC",   0xF000|042, 0x4000|(7<<10));
    TEST("PMOVE.W", "-(A2), PSR",  0xF000|042, 0x4000|(8<<10));
    TEST("PMOVE.W", "-(A2), PCSR", 0xF000|042, 0x4000|(9<<10));
    TEST("PMOVE.W", "-(A2), BAD0", 0xF000|042, 0x4000|(12<<10)|(0<<2));
    TEST("PMOVE.W", "-(A2), BAC7", 0xF000|042, 0x4000|(13<<10)|(7<<2));

    TEST("PMOVE.L", "($1234,A2), TC",   0xF000|052, 0x4000|(0<<10), 0x1234);
    TEST("PMOVE.Q", "($1234,A2), DRP",  0xF000|052, 0x4000|(1<<10), 0x1234);
    TEST("PMOVE.Q", "($1234,A2), SRP",  0xF000|052, 0x4000|(2<<10), 0x1234);
    TEST("PMOVE.Q", "($1234,A2), CRP",  0xF000|052, 0x4000|(3<<10), 0x1234);
    TEST("PMOVE.B", "($1234,A2), CAL",  0xF000|052, 0x4000|(4<<10), 0x1234);
    TEST("PMOVE.B", "($1234,A2), VAL",  0xF000|052, 0x4000|(5<<10), 0x1234);
    TEST("PMOVE.B", "($1234,A2), SCC",  0xF000|052, 0x4000|(6<<10), 0x1234);
    TEST("PMOVE.W", "($1234,A2), AC",   0xF000|052, 0x4000|(7<<10), 0x1234);
    TEST("PMOVE.W", "($1234,A2), PSR",  0xF000|052, 0x4000|(8<<10), 0x1234);
    TEST("PMOVE.W", "($1234,A2), PCSR", 0xF000|052, 0x4000|(9<<10), 0x1234);
    TEST("PMOVE.W", "($1234,A2), BAD0", 0xF000|052, 0x4000|(12<<10)|(0<<2), 0x1234);
    TEST("PMOVE.W", "($1234,A2), BAC7", 0xF000|052, 0x4000|(13<<10)|(7<<2), 0x1234);

    TEST("PMOVE.L", "($34,A2,D3.W), TC",   0xF000|062, 0x4000|(0<<10), 0x3034);
    TEST("PMOVE.Q", "($34,A2,D3.W), DRP",  0xF000|062, 0x4000|(1<<10), 0x3034);
    TEST("PMOVE.Q", "($34,A2,D3.W), SRP",  0xF000|062, 0x4000|(2<<10), 0x3034);
    TEST("PMOVE.Q", "($34,A2,D3.W), CRP",  0xF000|062, 0x4000|(3<<10), 0x3034);
    TEST("PMOVE.B", "($34,A2,D3.W), CAL",  0xF000|062, 0x4000|(4<<10), 0x3034);
    TEST("PMOVE.B", "($34,A2,D3.W), VAL",  0xF000|062, 0x4000|(5<<10), 0x3034);
    TEST("PMOVE.B", "($34,A2,D3.W), SCC",  0xF000|062, 0x4000|(6<<10), 0x3034);
    TEST("PMOVE.W", "($34,A2,D3.W), AC",   0xF000|062, 0x4000|(7<<10), 0x3034);
    TEST("PMOVE.W", "($34,A2,D3.W), PSR",  0xF000|062, 0x4000|(8<<10), 0x3034);
    TEST("PMOVE.W", "($34,A2,D3.W), PCSR", 0xF000|062, 0x4000|(9<<10), 0x3034);
    TEST("PMOVE.W", "($34,A2,D3.W), BAD0", 0xF000|062, 0x4000|(12<<10)|(0<<2), 0x3034);
    TEST("PMOVE.W", "($34,A2,D3.W), BAC7", 0xF000|062, 0x4000|(13<<10)|(7<<2), 0x3034);

    TEST("PMOVE.L", "($00001234).W, TC",   0xF000|070, 0x4000|(0<<10), 0x1234);
    TEST("PMOVE.Q", "($00001234).W, DRP",  0xF000|070, 0x4000|(1<<10), 0x1234);
    TEST("PMOVE.Q", "($00001234).W, SRP",  0xF000|070, 0x4000|(2<<10), 0x1234);
    TEST("PMOVE.Q", "($00001234).W, CRP",  0xF000|070, 0x4000|(3<<10), 0x1234);
    TEST("PMOVE.B", "($00001234).W, CAL",  0xF000|070, 0x4000|(4<<10), 0x1234);
    TEST("PMOVE.B", "($00001234).W, VAL",  0xF000|070, 0x4000|(5<<10), 0x1234);
    TEST("PMOVE.B", "($00001234).W, SCC",  0xF000|070, 0x4000|(6<<10), 0x1234);
    TEST("PMOVE.W", "($00001234).W, AC",   0xF000|070, 0x4000|(7<<10), 0x1234);
    TEST("PMOVE.W", "($00001234).W, PSR",  0xF000|070, 0x4000|(8<<10), 0x1234);
    TEST("PMOVE.W", "($00001234).W, PCSR", 0xF000|070, 0x4000|(9<<10), 0x1234);
    TEST("PMOVE.W", "($00001234).W, BAD0", 0xF000|070, 0x4000|(12<<10)|(0<<2), 0x1234);
    TEST("PMOVE.W", "($00001234).W, BAC7", 0xF000|070, 0x4000|(13<<10)|(7<<2), 0x1234);

    TEST("PMOVE.L", "($12345678).L, TC",   0xF000|071, 0x4000|(0<<10), 0x1234, 0x5678);
    TEST("PMOVE.Q", "($12345678).L, DRP",  0xF000|071, 0x4000|(1<<10), 0x1234, 0x5678);
    TEST("PMOVE.Q", "($12345678).L, SRP",  0xF000|071, 0x4000|(2<<10), 0x1234, 0x5678);
    TEST("PMOVE.Q", "($12345678).L, CRP",  0xF000|071, 0x4000|(3<<10), 0x1234, 0x5678);
    TEST("PMOVE.B", "($12345678).L, CAL",  0xF000|071, 0x4000|(4<<10), 0x1234, 0x5678);
    TEST("PMOVE.B", "($12345678).L, VAL",  0xF000|071, 0x4000|(5<<10), 0x1234, 0x5678);
    TEST("PMOVE.B", "($12345678).L, SCC",  0xF000|071, 0x4000|(6<<10), 0x1234, 0x5678);
    TEST("PMOVE.W", "($12345678).L, AC",   0xF000|071, 0x4000|(7<<10), 0x1234, 0x5678);
    TEST("PMOVE.W", "($12345678).L, PSR",  0xF000|071, 0x4000|(8<<10), 0x1234, 0x5678);
    TEST("PMOVE.W", "($12345678).L, PCSR", 0xF000|071, 0x4000|(9<<10), 0x1234, 0x5678);
    TEST("PMOVE.W", "($12345678).L, BAD0", 0xF000|071, 0x4000|(12<<10)|(0<<2), 0x1234, 0x5678);
    TEST("PMOVE.W", "($12345678).L, BAC7", 0xF000|071, 0x4000|(13<<10)|(7<<2), 0x1234, 0x5678);

    TEST("PMOVE.L", "(*+$1234,PC), TC",   0xF000|072, 0x4000|(0<<10), 0x1230);
    TEST("PMOVE.Q", "(*+$1234,PC), DRP",  0xF000|072, 0x4000|(1<<10), 0x1230);
    TEST("PMOVE.Q", "(*+$1234,PC), SRP",  0xF000|072, 0x4000|(2<<10), 0x1230);
    TEST("PMOVE.Q", "(*+$1234,PC), CRP",  0xF000|072, 0x4000|(3<<10), 0x1230);
    TEST("PMOVE.B", "(*+$1234,PC), CAL",  0xF000|072, 0x4000|(4<<10), 0x1230);
    TEST("PMOVE.B", "(*+$1234,PC), VAL",  0xF000|072, 0x4000|(5<<10), 0x1230);
    TEST("PMOVE.B", "(*+$1234,PC), SCC",  0xF000|072, 0x4000|(6<<10), 0x1230);
    TEST("PMOVE.W", "(*+$1234,PC), AC",   0xF000|072, 0x4000|(7<<10), 0x1230);
    TEST("PMOVE.W", "(*+$1234,PC), PSR",  0xF000|072, 0x4000|(8<<10), 0x1230);
    TEST("PMOVE.W", "(*+$1234,PC), PCSR", 0xF000|072, 0x4000|(9<<10), 0x1230);
    TEST("PMOVE.W", "(*+$1234,PC), BAD0", 0xF000|072, 0x4000|(12<<10)|(0<<2), 0x1230);
    TEST("PMOVE.W", "(*+$1234,PC), BAC7", 0xF000|072, 0x4000|(13<<10)|(7<<2), 0x1230);

    TEST("PMOVE.L", "(*+52,PC,D3.W), TC",   0xF000|073, 0x4000|(0<<10), 0x3030);
    TEST("PMOVE.Q", "(*+52,PC,D3.W), DRP",  0xF000|073, 0x4000|(1<<10), 0x3030);
    TEST("PMOVE.Q", "(*+52,PC,D3.W), SRP",  0xF000|073, 0x4000|(2<<10), 0x3030);
    TEST("PMOVE.Q", "(*+52,PC,D3.W), CRP",  0xF000|073, 0x4000|(3<<10), 0x3030);
    TEST("PMOVE.B", "(*+52,PC,D3.W), CAL",  0xF000|073, 0x4000|(4<<10), 0x3030);
    TEST("PMOVE.B", "(*+52,PC,D3.W), VAL",  0xF000|073, 0x4000|(5<<10), 0x3030);
    TEST("PMOVE.B", "(*+52,PC,D3.W), SCC",  0xF000|073, 0x4000|(6<<10), 0x3030);
    TEST("PMOVE.W", "(*+52,PC,D3.W), AC",   0xF000|073, 0x4000|(7<<10), 0x3030);
    TEST("PMOVE.W", "(*+52,PC,D3.W), PSR",  0xF000|073, 0x4000|(8<<10), 0x3030);
    TEST("PMOVE.W", "(*+52,PC,D3.W), PCSR", 0xF000|073, 0x4000|(9<<10), 0x3030);
    TEST("PMOVE.W", "(*+52,PC,D3.W), BAD0", 0xF000|073, 0x4000|(12<<10)|(0<<2), 0x3030);
    TEST("PMOVE.W", "(*+52,PC,D3.W), BAC7", 0xF000|073, 0x4000|(13<<10)|(7<<2), 0x3030);

    TEST("PMOVE.L", "#$12345678, TC",          0xF000|074, 0x4000|(0<<10), 0x1234, 0x5678);
    TEST("PMOVE.Q", "#$123456789ABCDEF0, DRP", 0xF000|074, 0x4000|(1<<10),
         0x1234, 0x5678, 0x9ABC, 0xDEF0);
    TEST("PMOVE.Q", "#$123456789ABCDEF0, SRP", 0xF000|074, 0x4000|(2<<10),
         0x1234, 0x5678, 0x9ABC, 0xDEF0);
    TEST("PMOVE.Q", "#$123456789ABCDEF0, CRP", 0xF000|074, 0x4000|(3<<10),
         0x1234, 0x5678, 0x9ABC, 0xDEF0);
    TEST("PMOVE.B", "#$34, CAL",    0xF000|074, 0x4000|(4<<10), 0x1234);
    TEST("PMOVE.B", "#$34, VAL",    0xF000|074, 0x4000|(5<<10), 0x1234);
    TEST("PMOVE.B", "#$34, SCC",    0xF000|074, 0x4000|(6<<10), 0x1234);
    TEST("PMOVE.W", "#$1234, AC",   0xF000|074, 0x4000|(7<<10), 0x1234);
    TEST("PMOVE.W", "#$1234, PSR",  0xF000|074, 0x4000|(8<<10), 0x1234);
    TEST("PMOVE.W", "#$1234, PCSR", 0xF000|074, 0x4000|(9<<10), 0x1234);
    TEST("PMOVE.W", "#$1234, BAD0", 0xF000|074, 0x4000|(12<<10)|(0<<2), 0x1234);
    TEST("PMOVE.W", "#$1234, BAC7", 0xF000|074, 0x4000|(13<<10)|(7<<2), 0x1234);

    TEST("PMOVE.L", "TC, D2",   0xF000|002, 0x4200|(0<<10));
    ERRT("PMOVE.Q", "DRP, D2", ILLEGAL_SIZE, "D2", 0xF000|002, 0x4200|(1<<10));
    ERRT("PMOVE.Q", "SRP, D2", ILLEGAL_SIZE, "D2", 0xF000|002, 0x4200|(2<<10));
    ERRT("PMOVE.Q", "CRP, D2", ILLEGAL_SIZE, "D2", 0xF000|002, 0x4200|(3<<10));
    TEST("PMOVE.B", "CAL, D2",  0xF000|002, 0x4200|(4<<10));
    TEST("PMOVE.B", "VAL, D2",  0xF000|002, 0x4200|(5<<10));
    TEST("PMOVE.B", "SCC, D2",  0xF000|002, 0x4200|(6<<10));
    TEST("PMOVE.W", "AC, D2",   0xF000|002, 0x4200|(7<<10));
    TEST("PMOVE.W", "PSR, D2",  0xF000|002, 0x4200|(8<<10));
    TEST("PMOVE.W", "PCSR, D2", 0xF000|002, 0x4200|(9<<10));
    ERRT("PMOVE",   ", D2", ILLEGAL_REGISTER, ", D2", 0xF000|002, 0x4200|(10<<10));
    ERRT("PMOVE",   ", D2", ILLEGAL_REGISTER, ", D2", 0xF000|002, 0x4200|(11<<10));
    TEST("PMOVE.W", "BAD0, D2", 0xF000|002, 0x4200|(12<<10)|(0<<2));
    TEST("PMOVE.W", "BAD7, D2", 0xF000|002, 0x4200|(12<<10)|(7<<2));
    TEST("PMOVE.W", "BAC0, D2", 0xF000|002, 0x4200|(13<<10)|(0<<2));
    TEST("PMOVE.W", "BAC7, D2", 0xF000|002, 0x4200|(13<<10)|(7<<2));

    TEST("PMOVE.L", "TC, A2",   0xF000|012, 0x4200|(0<<10));
    ERRT("PMOVE.Q", "DRP, A2", ILLEGAL_SIZE, "A2", 0xF000|012, 0x4200|(1<<10));
    ERRT("PMOVE.Q", "SRP, A2", ILLEGAL_SIZE, "A2", 0xF000|012, 0x4200|(2<<10));
    ERRT("PMOVE.Q", "CRP, A2", ILLEGAL_SIZE, "A2", 0xF000|012, 0x4200|(3<<10));
    TEST("PMOVE.B", "CAL, A2",  0xF000|012, 0x4200|(4<<10));
    TEST("PMOVE.B", "VAL, A2",  0xF000|012, 0x4200|(5<<10));
    TEST("PMOVE.B", "SCC, A2",  0xF000|012, 0x4200|(6<<10));
    TEST("PMOVE.W", "AC, A2",   0xF000|012, 0x4200|(7<<10));
    TEST("PMOVE.W", "PSR, A2",  0xF000|012, 0x4200|(8<<10));
    TEST("PMOVE.W", "PCSR, A2", 0xF000|012, 0x4200|(9<<10));
    TEST("PMOVE.W", "BAD0, A2", 0xF000|012, 0x4200|(12<<10)|(0<<2));
    TEST("PMOVE.W", "BAC7, A2", 0xF000|012, 0x4200|(13<<10)|(7<<2));

    TEST("PMOVE.L", "TC, (A2)",   0xF000|022, 0x4200|(0<<10));
    TEST("PMOVE.Q", "DRP, (A2)",  0xF000|022, 0x4200|(1<<10));
    TEST("PMOVE.Q", "SRP, (A2)",  0xF000|022, 0x4200|(2<<10));
    TEST("PMOVE.Q", "CRP, (A2)",  0xF000|022, 0x4200|(3<<10));
    TEST("PMOVE.B", "CAL, (A2)",  0xF000|022, 0x4200|(4<<10));
    TEST("PMOVE.B", "VAL, (A2)",  0xF000|022, 0x4200|(5<<10));
    TEST("PMOVE.B", "SCC, (A2)",  0xF000|022, 0x4200|(6<<10));
    TEST("PMOVE.W", "AC, (A2)",   0xF000|022, 0x4200|(7<<10));
    TEST("PMOVE.W", "PSR, (A2)",  0xF000|022, 0x4200|(8<<10));
    TEST("PMOVE.W", "PCSR, (A2)", 0xF000|022, 0x4200|(9<<10));
    TEST("PMOVE.W", "BAD0, (A2)", 0xF000|022, 0x4200|(12<<10)|(0<<2));
    TEST("PMOVE.W", "BAC7, (A2)", 0xF000|022, 0x4200|(13<<10)|(7<<2));

    TEST("PMOVE.L", "TC, (A2)+",   0xF000|032, 0x4200|(0<<10));
    TEST("PMOVE.Q", "DRP, (A2)+",  0xF000|032, 0x4200|(1<<10));
    TEST("PMOVE.Q", "SRP, (A2)+",  0xF000|032, 0x4200|(2<<10));
    TEST("PMOVE.Q", "CRP, (A2)+",  0xF000|032, 0x4200|(3<<10));
    TEST("PMOVE.B", "CAL, (A2)+",  0xF000|032, 0x4200|(4<<10));
    TEST("PMOVE.B", "VAL, (A2)+",  0xF000|032, 0x4200|(5<<10));
    TEST("PMOVE.B", "SCC, (A2)+",  0xF000|032, 0x4200|(6<<10));
    TEST("PMOVE.W", "AC, (A2)+",   0xF000|032, 0x4200|(7<<10));
    TEST("PMOVE.W", "PSR, (A2)+",  0xF000|032, 0x4200|(8<<10));
    TEST("PMOVE.W", "PCSR, (A2)+", 0xF000|032, 0x4200|(9<<10));
    TEST("PMOVE.W", "BAD0, (A2)+", 0xF000|032, 0x4200|(12<<10)|(0<<2));
    TEST("PMOVE.W", "BAC7, (A2)+", 0xF000|032, 0x4200|(13<<10)|(7<<2));

    TEST("PMOVE.L", "TC, -(A2)",   0xF000|042, 0x4200|(0<<10));
    TEST("PMOVE.Q", "DRP, -(A2)",  0xF000|042, 0x4200|(1<<10));
    TEST("PMOVE.Q", "SRP, -(A2)",  0xF000|042, 0x4200|(2<<10));
    TEST("PMOVE.Q", "CRP, -(A2)",  0xF000|042, 0x4200|(3<<10));
    TEST("PMOVE.B", "CAL, -(A2)",  0xF000|042, 0x4200|(4<<10));
    TEST("PMOVE.B", "VAL, -(A2)",  0xF000|042, 0x4200|(5<<10));
    TEST("PMOVE.B", "SCC, -(A2)",  0xF000|042, 0x4200|(6<<10));
    TEST("PMOVE.W", "AC, -(A2)",   0xF000|042, 0x4200|(7<<10));
    TEST("PMOVE.W", "PSR, -(A2)",  0xF000|042, 0x4200|(8<<10));
    TEST("PMOVE.W", "PCSR, -(A2)", 0xF000|042, 0x4200|(9<<10));
    TEST("PMOVE.W", "BAD0, -(A2)", 0xF000|042, 0x4200|(12<<10)|(0<<2));
    TEST("PMOVE.W", "BAC7, -(A2)", 0xF000|042, 0x4200|(13<<10)|(7<<2));

    TEST("PMOVE.L", "TC, ($1234,A2)",   0xF000|052, 0x4200|(0<<10), 0x1234);
    TEST("PMOVE.Q", "DRP, ($1234,A2)",  0xF000|052, 0x4200|(1<<10), 0x1234);
    TEST("PMOVE.Q", "SRP, ($1234,A2)",  0xF000|052, 0x4200|(2<<10), 0x1234);
    TEST("PMOVE.Q", "CRP, ($1234,A2)",  0xF000|052, 0x4200|(3<<10), 0x1234);
    TEST("PMOVE.B", "CAL, ($1234,A2)",  0xF000|052, 0x4200|(4<<10), 0x1234);
    TEST("PMOVE.B", "VAL, ($1234,A2)",  0xF000|052, 0x4200|(5<<10), 0x1234);
    TEST("PMOVE.B", "SCC, ($1234,A2)",  0xF000|052, 0x4200|(6<<10), 0x1234);
    TEST("PMOVE.W", "AC, ($1234,A2)",   0xF000|052, 0x4200|(7<<10), 0x1234);
    TEST("PMOVE.W", "PSR, ($1234,A2)",  0xF000|052, 0x4200|(8<<10), 0x1234);
    TEST("PMOVE.W", "PCSR, ($1234,A2)", 0xF000|052, 0x4200|(9<<10), 0x1234);
    TEST("PMOVE.W", "BAD0, ($1234,A2)", 0xF000|052, 0x4200|(12<<10)|(0<<2), 0x1234);
    TEST("PMOVE.W", "BAC7, ($1234,A2)", 0xF000|052, 0x4200|(13<<10)|(7<<2), 0x1234);

    TEST("PMOVE.L", "TC, ($34,A2,D3.W)",   0xF000|062, 0x4200|(0<<10), 0x3034);
    TEST("PMOVE.Q", "DRP, ($34,A2,D3.W)",  0xF000|062, 0x4200|(1<<10), 0x3034);
    TEST("PMOVE.Q", "SRP, ($34,A2,D3.W)",  0xF000|062, 0x4200|(2<<10), 0x3034);
    TEST("PMOVE.Q", "CRP, ($34,A2,D3.W)",  0xF000|062, 0x4200|(3<<10), 0x3034);
    TEST("PMOVE.B", "CAL, ($34,A2,D3.W)",  0xF000|062, 0x4200|(4<<10), 0x3034);
    TEST("PMOVE.B", "VAL, ($34,A2,D3.W)",  0xF000|062, 0x4200|(5<<10), 0x3034);
    TEST("PMOVE.B", "SCC, ($34,A2,D3.W)",  0xF000|062, 0x4200|(6<<10), 0x3034);
    TEST("PMOVE.W", "AC, ($34,A2,D3.W)",   0xF000|062, 0x4200|(7<<10), 0x3034);
    TEST("PMOVE.W", "PSR, ($34,A2,D3.W)",  0xF000|062, 0x4200|(8<<10), 0x3034);
    TEST("PMOVE.W", "PCSR, ($34,A2,D3.W)", 0xF000|062, 0x4200|(9<<10), 0x3034);
    TEST("PMOVE.W", "BAD0, ($34,A2,D3.W)", 0xF000|062, 0x4200|(12<<10)|(0<<2), 0x3034);
    TEST("PMOVE.W", "BAC7, ($34,A2,D3.W)", 0xF000|062, 0x4200|(13<<10)|(7<<2), 0x3034);

    TEST("PMOVE.L", "TC, ($00001234).W",   0xF000|070, 0x4200|(0<<10), 0x1234);
    TEST("PMOVE.Q", "DRP, ($00001234).W",  0xF000|070, 0x4200|(1<<10), 0x1234);
    TEST("PMOVE.Q", "SRP, ($00001234).W",  0xF000|070, 0x4200|(2<<10), 0x1234);
    TEST("PMOVE.Q", "CRP, ($00001234).W",  0xF000|070, 0x4200|(3<<10), 0x1234);
    TEST("PMOVE.B", "CAL, ($00001234).W",  0xF000|070, 0x4200|(4<<10), 0x1234);
    TEST("PMOVE.B", "VAL, ($00001234).W",  0xF000|070, 0x4200|(5<<10), 0x1234);
    TEST("PMOVE.B", "SCC, ($00001234).W",  0xF000|070, 0x4200|(6<<10), 0x1234);
    TEST("PMOVE.W", "AC, ($00001234).W",   0xF000|070, 0x4200|(7<<10), 0x1234);
    TEST("PMOVE.W", "PSR, ($00001234).W",  0xF000|070, 0x4200|(8<<10), 0x1234);
    TEST("PMOVE.W", "PCSR, ($00001234).W", 0xF000|070, 0x4200|(9<<10), 0x1234);
    TEST("PMOVE.W", "BAD0, ($00001234).W", 0xF000|070, 0x4200|(12<<10)|(0<<2), 0x1234);
    TEST("PMOVE.W", "BAC7, ($00001234).W", 0xF000|070, 0x4200|(13<<10)|(7<<2), 0x1234);

    TEST("PMOVE.L", "TC, ($12345678).L",   0xF000|071, 0x4200|(0<<10), 0x1234, 0x5678);
    TEST("PMOVE.Q", "DRP, ($12345678).L",  0xF000|071, 0x4200|(1<<10), 0x1234, 0x5678);
    TEST("PMOVE.Q", "SRP, ($12345678).L",  0xF000|071, 0x4200|(2<<10), 0x1234, 0x5678);
    TEST("PMOVE.Q", "CRP, ($12345678).L",  0xF000|071, 0x4200|(3<<10), 0x1234, 0x5678);
    TEST("PMOVE.B", "CAL, ($12345678).L",  0xF000|071, 0x4200|(4<<10), 0x1234, 0x5678);
    TEST("PMOVE.B", "VAL, ($12345678).L",  0xF000|071, 0x4200|(5<<10), 0x1234, 0x5678);
    TEST("PMOVE.B", "SCC, ($12345678).L",  0xF000|071, 0x4200|(6<<10), 0x1234, 0x5678);
    TEST("PMOVE.W", "AC, ($12345678).L",   0xF000|071, 0x4200|(7<<10), 0x1234, 0x5678);
    TEST("PMOVE.W", "PSR, ($12345678).L",  0xF000|071, 0x4200|(8<<10), 0x1234, 0x5678);
    TEST("PMOVE.W", "PCSR, ($12345678).L", 0xF000|071, 0x4200|(9<<10), 0x1234, 0x5678);
    TEST("PMOVE.W", "BAD0, ($12345678).L", 0xF000|071, 0x4200|(12<<10)|(0<<2), 0x1234, 0x5678);
    TEST("PMOVE.W", "BAC7, ($12345678).L", 0xF000|071, 0x4200|(13<<10)|(7<<2), 0x1234, 0x5678);

    UNKN(0xF000|072, 0x4200);   // (d16,PC)
    UNKN(0xF000|073, 0x4200);   // (d8,PC,Xn)
    UNKN(0xF000|074, 0x4200);   // #n
}

void test_branch_mc68851() {
    ATEST(0x10000, "PBBS", "*+$1234", 0xF080, 0x1232);
    ATEST(0x10000, "PBBC", "*+$1234", 0xF081, 0x1232);
    ATEST(0x10000, "PBLS", "*-$1234", 0xF082, 0xEDCA);
    ATEST(0x10000, "PBLC", "*+$1234", 0xF083, 0x1232);
    ATEST(0x10000, "PBSS", "*+$1234", 0xF084, 0x1232);
    ATEST(0x10000, "PBSC", "*+$1234", 0xF085, 0x1232);
    ATEST(0x10000, "PBAS", "*+$1234", 0xF086, 0x1232);
    ATEST(0x10000, "PBAC", "*+$1234", 0xF087, 0x1232);
    ATEST(0x10000, "PBWS", "*+$1234", 0xF088, 0x1232);
    ATEST(0x10000, "PBWC", "*+$1234", 0xF089, 0x1232);
    ATEST(0x10000, "PBIS", "*+$1234", 0xF08A, 0x1232);
    ATEST(0x10000, "PBIC", "*+$1234", 0xF08B, 0x1232);
    ATEST(0x10000, "PBGS", "*+$1234", 0xF08C, 0x1232);
    ATEST(0x10000, "PBGC", "*+$1234", 0xF08D, 0x1232);
    ATEST(0x10000, "PBCS", "*+$1234", 0xF08E, 0x1232);
    ATEST(0x10000, "PBCC", "*+$1234", 0xF08F, 0x1232);
    AERRT(0x01000, "PBCC", "*-$00002000", OVERFLOW_RANGE, "*-$00002000", 0xF08F, 0xDFFE);

    ATEST(0x200000, "PBBS",  "*+$00123456", 0xF0C0, 0x0012, 0x3454);
    ATEST(0x200000, "PBBC.X","*+$00001234", 0xF0C1, 0x0000, 0x1232);
    ATEST(0x200000, "PBLS",  "*-$00123456", 0xF0C2, 0xFFED, 0xCBA8);
    ATEST(0x200000, "PBLC",  "*+$00123456", 0xF0C3, 0x0012, 0x3454);
    ATEST(0x200000, "PBSS",  "*+$00123456", 0xF0C4, 0x0012, 0x3454);
    ATEST(0x200000, "PBSC",  "*+$00123456", 0xF0C5, 0x0012, 0x3454);
    ATEST(0x200000, "PBAS",  "*+$00123456", 0xF0C6, 0x0012, 0x3454);
    ATEST(0x200000, "PBAC",  "*+$00123456", 0xF0C7, 0x0012, 0x3454);
    ATEST(0x200000, "PBWS",  "*+$00123456", 0xF0C8, 0x0012, 0x3454);
    ATEST(0x200000, "PBWC",  "*+$00123456", 0xF0C9, 0x0012, 0x3454);
    ATEST(0x200000, "PBIS",  "*+$00123456", 0xF0CA, 0x0012, 0x3454);
    ATEST(0x200000, "PBIC",  "*+$00123456", 0xF0CB, 0x0012, 0x3454);
    ATEST(0x200000, "PBGS",  "*+$00123456", 0xF0CC, 0x0012, 0x3454);
    ATEST(0x200000, "PBGC",  "*+$00123456", 0xF0CD, 0x0012, 0x3454);
    ATEST(0x200000, "PBCS",  "*+$00123456", 0xF0CE, 0x0012, 0x3454);
    ATEST(0x200000, "PBCC",  "*+$00123456", 0xF0CF, 0x0012, 0x3454);
    AERRT(0xF0000000, "PBCC", "*+$12345678", OVERFLOW_RANGE, "*+$12345678", 0xF0CF, 0x1234, 0x5676);

    TEST("PDBBS", "D2, *+$1234", 0xF048|012, 0x0000, 0x1230);
    TEST("PDBBC", "D2, *+$1234", 0xF048|012, 0x0001, 0x1230);
    TEST("PDBLS", "D2, *+$1234", 0xF048|012, 0x0002, 0x1230);
    TEST("PDBLC", "D2, *+$1234", 0xF048|012, 0x0003, 0x1230);
    TEST("PDBSS", "D2, *+$1234", 0xF048|012, 0x0004, 0x1230);
    TEST("PDBSC", "D2, *+$1234", 0xF048|012, 0x0005, 0x1230);
    TEST("PDBAS", "D2, *+$1234", 0xF048|012, 0x0006, 0x1230);
    TEST("PDBAC", "D2, *+$1234", 0xF048|012, 0x0007, 0x1230);
    TEST("PDBWS", "D2, *+$1234", 0xF048|012, 0x0008, 0x1230);
    TEST("PDBWC", "D2, *+$1234", 0xF048|012, 0x0009, 0x1230);
    TEST("PDBIS", "D2, *+$1234", 0xF048|012, 0x000A, 0x1230);
    TEST("PDBIC", "D2, *+$1234", 0xF048|012, 0x000B, 0x1230);
    TEST("PDBGS", "D2, *+$1234", 0xF048|012, 0x000C, 0x1230);
    TEST("PDBGC", "D2, *+$1234", 0xF048|012, 0x000D, 0x1230);
    TEST("PDBCS", "D2, *+$1234", 0xF048|012, 0x000E, 0x1230);
    TEST("PDBCC", "D2, *+$1234", 0xF048|012, 0x000F, 0x1230);

    disassembler.setOption("gnu-as", "on");
    TEST("PBBC",  ".+0x1234",     0xF081, 0x1232);
    TEST("PBBCL", ".+0x00001234", 0xF0C1, 0x0000, 0x1232);
}

void test_trap_mc68851() {
    TEST("PTRAPBS.W", "#$1234", 0xF07A, 0x0000, 0x1234);
    TEST("PTRAPBC.W", "#$1234", 0xF07A, 0x0001, 0x1234);
    TEST("PTRAPLS.W", "#$1234", 0xF07A, 0x0002, 0x1234);
    TEST("PTRAPLC.W", "#$1234", 0xF07A, 0x0003, 0x1234);
    TEST("PTRAPSS.W", "#$1234", 0xF07A, 0x0004, 0x1234);
    TEST("PTRAPSC.W", "#$1234", 0xF07A, 0x0005, 0x1234);
    TEST("PTRAPAS.W", "#$1234", 0xF07A, 0x0006, 0x1234);
    TEST("PTRAPAC.W", "#$1234", 0xF07A, 0x0007, 0x1234);
    TEST("PTRAPWS.W", "#$1234", 0xF07A, 0x0008, 0x1234);
    TEST("PTRAPWC.W", "#$1234", 0xF07A, 0x0009, 0x1234);
    TEST("PTRAPIS.W", "#$1234", 0xF07A, 0x000A, 0x1234);
    TEST("PTRAPIC.W", "#$1234", 0xF07A, 0x000B, 0x1234);
    TEST("PTRAPGS.W", "#$1234", 0xF07A, 0x000C, 0x1234);
    TEST("PTRAPGC.W", "#$1234", 0xF07A, 0x000D, 0x1234);
    TEST("PTRAPCS.W", "#$1234", 0xF07A, 0x000E, 0x1234);
    TEST("PTRAPCC.W", "#$1234", 0xF07A, 0x000F, 0x1234);

    TEST("PTRAPBS.L", "#$12345678", 0xF07B, 0x0000, 0x1234, 0x5678);
    TEST("PTRAPBC.L", "#$12345678", 0xF07B, 0x0001, 0x1234, 0x5678);
    TEST("PTRAPLS.L", "#$12345678", 0xF07B, 0x0002, 0x1234, 0x5678);
    TEST("PTRAPLC.L", "#$12345678", 0xF07B, 0x0003, 0x1234, 0x5678);
    TEST("PTRAPSS.L", "#$12345678", 0xF07B, 0x0004, 0x1234, 0x5678);
    TEST("PTRAPSC.L", "#$12345678", 0xF07B, 0x0005, 0x1234, 0x5678);
    TEST("PTRAPAS.L", "#$12345678", 0xF07B, 0x0006, 0x1234, 0x5678);
    TEST("PTRAPAC.L", "#$12345678", 0xF07B, 0x0007, 0x1234, 0x5678);
    TEST("PTRAPWS.L", "#$12345678", 0xF07B, 0x0008, 0x1234, 0x5678);
    TEST("PTRAPWC.L", "#$12345678", 0xF07B, 0x0009, 0x1234, 0x5678);
    TEST("PTRAPIS.L", "#$12345678", 0xF07B, 0x000A, 0x1234, 0x5678);
    TEST("PTRAPIC.L", "#$12345678", 0xF07B, 0x000B, 0x1234, 0x5678);
    TEST("PTRAPGS.L", "#$12345678", 0xF07B, 0x000C, 0x1234, 0x5678);
    TEST("PTRAPGC.L", "#$12345678", 0xF07B, 0x000D, 0x1234, 0x5678);
    TEST("PTRAPCS.L", "#$12345678", 0xF07B, 0x000E, 0x1234, 0x5678);
    TEST("PTRAPCC.L", "#$12345678", 0xF07B, 0x000F, 0x1234, 0x5678);

    TEST("PTRAPBS", "", 0xF07C, 0x0000);
    TEST("PTRAPBC", "", 0xF07C, 0x0001);
    TEST("PTRAPLS", "", 0xF07C, 0x0002);
    TEST("PTRAPLC", "", 0xF07C, 0x0003);
    TEST("PTRAPSS", "", 0xF07C, 0x0004);
    TEST("PTRAPSC", "", 0xF07C, 0x0005);
    TEST("PTRAPAS", "", 0xF07C, 0x0006);
    TEST("PTRAPAC", "", 0xF07C, 0x0007);
    TEST("PTRAPWS", "", 0xF07C, 0x0008);
    TEST("PTRAPWC", "", 0xF07C, 0x0009);
    TEST("PTRAPIS", "", 0xF07C, 0x000A);
    TEST("PTRAPIC", "", 0xF07C, 0x000B);
    TEST("PTRAPGS", "", 0xF07C, 0x000C);
    TEST("PTRAPGC", "", 0xF07C, 0x000D);
    TEST("PTRAPCS", "", 0xF07C, 0x000E);
    TEST("PTRAPCC", "", 0xF07C, 0x000F);

    TEST("PSBS", "D2",              0xF040|002, 0x0000);
    TEST("PSBS", "(A2)",            0xF040|022, 0x0000);
    TEST("PSBS", "(A2)+",           0xF040|032, 0x0000);
    TEST("PSBS", "-(A2)",           0xF040|042, 0x0000);
    TEST("PSBS", "($1234,A2)",      0xF040|052, 0x0000, 0x1234);
    TEST("PSBS", "(18,A2,D3.W)",    0xF040|062, 0x0000, 0x3012);
    TEST("PSBS", "($00001234).W",   0xF040|070, 0x0000, 0x1234);
    TEST("PSBS", "($12345678).L",   0xF040|071, 0x0000, 0x1234, 0x5678);

    TEST("PSBC", "D2",              0xF040|002, 0x0001);
    TEST("PSBC", "(A2)",            0xF040|022, 0x0001);
    TEST("PSBC", "(A2)+",           0xF040|032, 0x0001);
    TEST("PSBC", "-(A2)",           0xF040|042, 0x0001);
    TEST("PSBC", "($1234,A2)",      0xF040|052, 0x0001, 0x1234);
    TEST("PSBC", "(18,A2,D3.W)",    0xF040|062, 0x0001, 0x3012);
    TEST("PSBC", "($00001234).W",   0xF040|070, 0x0001, 0x1234);
    TEST("PSBC", "($12345678).L",   0xF040|071, 0x0001, 0x1234, 0x5678);

    TEST("PSLS", "D2",              0xF040|002, 0x0002);
    TEST("PSLS", "(A2)",            0xF040|022, 0x0002);
    TEST("PSLS", "(A2)+",           0xF040|032, 0x0002);
    TEST("PSLS", "-(A2)",           0xF040|042, 0x0002);
    TEST("PSLS", "($1234,A2)",      0xF040|052, 0x0002, 0x1234);
    TEST("PSLS", "(18,A2,D3.W)",    0xF040|062, 0x0002, 0x3012);
    TEST("PSLS", "($00001234).W",   0xF040|070, 0x0002, 0x1234);
    TEST("PSLS", "($12345678).L",   0xF040|071, 0x0002, 0x1234, 0x5678);

    TEST("PSLC", "D2",              0xF040|002, 0x0003);
    TEST("PSLC", "(A2)",            0xF040|022, 0x0003);
    TEST("PSLC", "(A2)+",           0xF040|032, 0x0003);
    TEST("PSLC", "-(A2)",           0xF040|042, 0x0003);
    TEST("PSLC", "($1234,A2)",      0xF040|052, 0x0003, 0x1234);
    TEST("PSLC", "(18,A2,D3.W)",    0xF040|062, 0x0003, 0x3012);
    TEST("PSLC", "($00001234).W",   0xF040|070, 0x0003, 0x1234);
    TEST("PSLC", "($12345678).L",   0xF040|071, 0x0003, 0x1234, 0x5678);

    TEST("PSSS", "D2",              0xF040|002, 0x0004);
    TEST("PSSS", "(A2)",            0xF040|022, 0x0004);
    TEST("PSSS", "(A2)+",           0xF040|032, 0x0004);
    TEST("PSSS", "-(A2)",           0xF040|042, 0x0004);
    TEST("PSSS", "($1234,A2)",      0xF040|052, 0x0004, 0x1234);
    TEST("PSSS", "(18,A2,D3.W)",    0xF040|062, 0x0004, 0x3012);
    TEST("PSSS", "($00001234).W",   0xF040|070, 0x0004, 0x1234);
    TEST("PSSS", "($12345678).L",   0xF040|071, 0x0004, 0x1234, 0x5678);

    TEST("PSSC", "D2",              0xF040|002, 0x0005);
    TEST("PSSC", "(A2)",            0xF040|022, 0x0005);
    TEST("PSSC", "(A2)+",           0xF040|032, 0x0005);
    TEST("PSSC", "-(A2)",           0xF040|042, 0x0005);
    TEST("PSSC", "($1234,A2)",      0xF040|052, 0x0005, 0x1234);
    TEST("PSSC", "(18,A2,D3.W)",    0xF040|062, 0x0005, 0x3012);
    TEST("PSSC", "($00001234).W",   0xF040|070, 0x0005, 0x1234);
    TEST("PSSC", "($12345678).L",   0xF040|071, 0x0005, 0x1234, 0x5678);

    TEST("PSAS", "D2",              0xF040|002, 0x0006);
    TEST("PSAS", "(A2)",            0xF040|022, 0x0006);
    TEST("PSAS", "(A2)+",           0xF040|032, 0x0006);
    TEST("PSAS", "-(A2)",           0xF040|042, 0x0006);
    TEST("PSAS", "($1234,A2)",      0xF040|052, 0x0006, 0x1234);
    TEST("PSAS", "(18,A2,D3.W)",    0xF040|062, 0x0006, 0x3012);
    TEST("PSAS", "($00001234).W",   0xF040|070, 0x0006, 0x1234);
    TEST("PSAS", "($12345678).L",   0xF040|071, 0x0006, 0x1234, 0x5678);

    TEST("PSAC", "D2",              0xF040|002, 0x0007);
    TEST("PSAC", "(A2)",            0xF040|022, 0x0007);
    TEST("PSAC", "(A2)+",           0xF040|032, 0x0007);
    TEST("PSAC", "-(A2)",           0xF040|042, 0x0007);
    TEST("PSAC", "($1234,A2)",      0xF040|052, 0x0007, 0x1234);
    TEST("PSAC", "(18,A2,D3.W)",    0xF040|062, 0x0007, 0x3012);
    TEST("PSAC", "($00001234).W",   0xF040|070, 0x0007, 0x1234);
    TEST("PSAC", "($12345678).L",   0xF040|071, 0x0007, 0x1234, 0x5678);

    TEST("PSWS", "D2",              0xF040|002, 0x0008);
    TEST("PSWS", "(A2)",            0xF040|022, 0x0008);
    TEST("PSWS", "(A2)+",           0xF040|032, 0x0008);
    TEST("PSWS", "-(A2)",           0xF040|042, 0x0008);
    TEST("PSWS", "($1234,A2)",      0xF040|052, 0x0008, 0x1234);
    TEST("PSWS", "(18,A2,D3.W)",    0xF040|062, 0x0008, 0x3012);
    TEST("PSWS", "($00001234).W",   0xF040|070, 0x0008, 0x1234);
    TEST("PSWS", "($12345678).L",   0xF040|071, 0x0008, 0x1234, 0x5678);

    TEST("PSWC", "D2",              0xF040|002, 0x0009);
    TEST("PSWC", "(A2)",            0xF040|022, 0x0009);
    TEST("PSWC", "(A2)+",           0xF040|032, 0x0009);
    TEST("PSWC", "-(A2)",           0xF040|042, 0x0009);
    TEST("PSWC", "($1234,A2)",      0xF040|052, 0x0009, 0x1234);
    TEST("PSWC", "(18,A2,D3.W)",    0xF040|062, 0x0009, 0x3012);
    TEST("PSWC", "($00001234).W",   0xF040|070, 0x0009, 0x1234);
    TEST("PSWC", "($12345678).L",   0xF040|071, 0x0009, 0x1234, 0x5678);

    TEST("PSIS", "D2",              0xF040|002, 0x000A);
    TEST("PSIS", "(A2)",            0xF040|022, 0x000A);
    TEST("PSIS", "(A2)+",           0xF040|032, 0x000A);
    TEST("PSIS", "-(A2)",           0xF040|042, 0x000A);
    TEST("PSIS", "($1234,A2)",      0xF040|052, 0x000A, 0x1234);
    TEST("PSIS", "(18,A2,D3.W)",    0xF040|062, 0x000A, 0x3012);
    TEST("PSIS", "($00001234).W",   0xF040|070, 0x000A, 0x1234);
    TEST("PSIS", "($12345678).L",   0xF040|071, 0x000A, 0x1234, 0x5678);

    TEST("PSIC", "D2",              0xF040|002, 0x000B);
    TEST("PSIC", "(A2)",            0xF040|022, 0x000B);
    TEST("PSIC", "(A2)+",           0xF040|032, 0x000B);
    TEST("PSIC", "-(A2)",           0xF040|042, 0x000B);
    TEST("PSIC", "($1234,A2)",      0xF040|052, 0x000B, 0x1234);
    TEST("PSIC", "(18,A2,D3.W)",    0xF040|062, 0x000B, 0x3012);
    TEST("PSIC", "($00001234).W",   0xF040|070, 0x000B, 0x1234);
    TEST("PSIC", "($12345678).L",   0xF040|071, 0x000B, 0x1234, 0x5678);

    TEST("PSGS", "D2",              0xF040|002, 0x000C);
    TEST("PSGS", "(A2)",            0xF040|022, 0x000C);
    TEST("PSGS", "(A2)+",           0xF040|032, 0x000C);
    TEST("PSGS", "-(A2)",           0xF040|042, 0x000C);
    TEST("PSGS", "($1234,A2)",      0xF040|052, 0x000C, 0x1234);
    TEST("PSGS", "(18,A2,D3.W)",    0xF040|062, 0x000C, 0x3012);
    TEST("PSGS", "($00001234).W",   0xF040|070, 0x000C, 0x1234);
    TEST("PSGS", "($12345678).L",   0xF040|071, 0x000C, 0x1234, 0x5678);

    TEST("PSGC", "D2",              0xF040|002, 0x000D);
    TEST("PSGC", "(A2)",            0xF040|022, 0x000D);
    TEST("PSGC", "(A2)+",           0xF040|032, 0x000D);
    TEST("PSGC", "-(A2)",           0xF040|042, 0x000D);
    TEST("PSGC", "($1234,A2)",      0xF040|052, 0x000D, 0x1234);
    TEST("PSGC", "(18,A2,D3.W)",    0xF040|062, 0x000D, 0x3012);
    TEST("PSGC", "($00001234).W",   0xF040|070, 0x000D, 0x1234);
    TEST("PSGC", "($12345678).L",   0xF040|071, 0x000D, 0x1234, 0x5678);

    TEST("PSCS", "D2",              0xF040|002, 0x000E);
    TEST("PSCS", "(A2)",            0xF040|022, 0x000E);
    TEST("PSCS", "(A2)+",           0xF040|032, 0x000E);
    TEST("PSCS", "-(A2)",           0xF040|042, 0x000E);
    TEST("PSCS", "($1234,A2)",      0xF040|052, 0x000E, 0x1234);
    TEST("PSCS", "(18,A2,D3.W)",    0xF040|062, 0x000E, 0x3012);
    TEST("PSCS", "($00001234).W",   0xF040|070, 0x000E, 0x1234);
    TEST("PSCS", "($12345678).L",   0xF040|071, 0x000E, 0x1234, 0x5678);

    TEST("PSCC", "D2",              0xF040|002, 0x000F);
    TEST("PSCC", "(A2)",            0xF040|022, 0x000F);
    TEST("PSCC", "(A2)+",           0xF040|032, 0x000F);
    TEST("PSCC", "-(A2)",           0xF040|042, 0x000F);
    TEST("PSCC", "($1234,A2)",      0xF040|052, 0x000F, 0x1234);
    TEST("PSCC", "(18,A2,D3.W)",    0xF040|062, 0x000F, 0x3012);
    TEST("PSCC", "($00001234).W",   0xF040|070, 0x000F, 0x1234);
    TEST("PSCC", "($12345678).L",   0xF040|071, 0x000F, 0x1234, 0x5678);
}

void test_pmmu_mc68851() {
    TEST("PFLUSHA", "",        0xF000, 0x2400);

    TEST("PFLUSH",  "#15, #4", 0xF000, 0x3010|(4<<5)|15);
    TEST("PFLUSH",  "D4, #15", 0xF000, 0x3008|(15<<5)|4);
    TEST("PFLUSH",  "SFC, #1", 0xF000, 0x3000|(1<<5)|0);
    TEST("PFLUSH",  "DFC, #7", 0xF000, 0x3000|(7<<5)|1);

    TEST("PFLUSHS", "#15, #4", 0xF000, 0x3410|(4<<5)|15);
    TEST("PFLUSHS", "D4, #15", 0xF000, 0x3408|(15<<5)|4);
    TEST("PFLUSHS", "SFC, #2", 0xF000, 0x3400|(2<<5)|0);
    TEST("PFLUSHS", "DFC, #4", 0xF000, 0x3400|(4<<5)|1);

    UNKN(0xF000|002, 0x3800);   // D2
    UNKN(0xF000|012, 0x3800);   // A2
    TEST("PFLUSH",  "#15, #4, (A2)",          0xF000|022, 0x3810|(4<<5)|15);
    UNKN(0xF000|032, 0x3800);   // (A2)+
    UNKN(0xF000|042, 0x3800);   // -(A2)
    TEST("PFLUSH",  "D4, #15, ($1234,A2)",    0xF000|052, 0x3808|(15<<5)|4, 0x1234);
    TEST("PFLUSH",  "SFC, #1, ($34,A2,D3.W)", 0xF000|062, 0x3800|(1<<5)|0, 0x3034);
    TEST("PFLUSH",  "DFC, #1, ($00001234).W", 0xF000|070, 0x3800|(1<<5)|1, 0x1234);
    TEST("PFLUSH",  "#15, #4, ($12345678).L", 0xF000|071, 0x3810|(4<<5)|15, 0x1234, 0x5678);
    UNKN(0xF000|072, 0x3800);   // (d16,PC)
    UNKN(0xF000|073, 0x3800);   // (d8,PC,Xn)
    UNKN(0xF000|074, 0x3800);   // #n

    UNKN(0xF000|002, 0x3C00);   // D2
    UNKN(0xF000|012, 0x3C00);   // A2
    TEST("PFLUSHS",  "#7, #4, (A2)",           0xF000|022, 0x3C10|(4<<5)|7);
    UNKN(0xF000|032, 0x3C00);   // (A2)+
    UNKN(0xF000|042, 0x3C00);   // -(A2)
    TEST("PFLUSHS",  "D4, #15, ($1234,A2)",    0xF000|052, 0x3C08|(15<<5)|4, 0x1234);
    TEST("PFLUSHS",  "SFC, #1, ($34,A2,D3.W)", 0xF000|062, 0x3C00|(1<<5)|0, 0x3034);
    TEST("PFLUSHS",  "DFC, #1, ($00001234).W", 0xF000|070, 0x3C00|(1<<5)|1, 0x1234);
    TEST("PFLUSHS",  "#4, #4, ($12345678).L",  0xF000|071, 0x3C10|(4<<5)|4, 0x1234, 0x5678);
    UNKN(0xF000|072, 0x3C00);   // (d16,PC)
    UNKN(0xF000|073, 0x3C00);   // (d8,PC,Xn)
    UNKN(0xF000|074, 0x3C00);   // #n

    UNKN(0xF000|002, 0xA000);   // D2
    UNKN(0xF000|012, 0xA000);   // A2
    TEST("PFLUSHR", "(A2)",               0xF000|022, 0xA000);
    TEST("PFLUSHR", "(A2)+",              0xF000|032, 0xA000);
    TEST("PFLUSHR", "-(A2)",              0xF000|042, 0xA000);
    TEST("PFLUSHR", "($1234,A2)",         0xF000|052, 0xA000, 0x1234);
    TEST("PFLUSHR", "($34,A2,D3.W)",      0xF000|062, 0xA000, 0x3034);
    TEST("PFLUSHR", "($00001234).W",      0xF000|070, 0xA000, 0x1234);
    TEST("PFLUSHR", "($12345678).L",      0xF000|071, 0xA000, 0x1234, 0x5678);
    TEST("PFLUSHR", "(*+$1234,PC)",       0xF000|072, 0xA000, 0x1230);
    TEST("PFLUSHR", "(*+52,PC,D3.L)",     0xF000|073, 0xA000, 0x3830);
    TEST("PFLUSHR", "#$123456789ABCDEF0", 0xF000|074, 0xA000, 0x1234, 0x5678, 0x9ABC, 0xDEF0);

    UNKN(0xF000|002, 0x2200);   // D2
    UNKN(0xF000|012, 0x2200);   // A2
    TEST("PLOADR", "#15, (A2)",          0xF000|022, 0x2210|15);
    UNKN(0xF000|032, 0x2200);   // (A2)+
    UNKN(0xF000|042, 0x2200);   // -(A2)
    TEST("PLOADR", "D3, ($1234,A2)",     0xF000|052, 0x2208|3, 0x1234);
    TEST("PLOADR", "SFC, ($34,A2,D3.W)", 0xF000|062, 0x2200|0, 0x3034);
    TEST("PLOADR", "DFC, ($00001234).W", 0xF000|070, 0x2200|1, 0x1234);
    TEST("PLOADR", "#10, ($12345678).L", 0xF000|071, 0x2210|10, 0x1234, 0x5678);
    UNKN(0xF000|072, 0x2200);   // (d16,PC)
    UNKN(0xF000|073, 0x2200);   // (d8,PC,Xn)
    UNKN(0xF000|074, 0x2200);   // #n

    UNKN(0xF000|002, 0x2000);   // D2
    UNKN(0xF000|012, 0x2000);   // A2
    TEST("PLOADW", "#15, (A2)",          0xF000|022, 0x2010|15);
    UNKN(0xF000|032, 0x2000);   // (A2)+
    UNKN(0xF000|042, 0x2000);   // -(A2)
    TEST("PLOADW", "D3, ($1234,A2)",     0xF000|052, 0x2008|3, 0x1234);
    TEST("PLOADW", "SFC, ($34,A2,D3.W)", 0xF000|062, 0x2000|0, 0x3034);
    TEST("PLOADW", "DFC, ($00001234).W", 0xF000|070, 0x2000|1, 0x1234);
    TEST("PLOADW", "#10, ($12345678).L", 0xF000|071, 0x2010|10, 0x1234, 0x5678);
    UNKN(0xF000|072, 0x2000);   // (d16,PC)
    UNKN(0xF000|073, 0x2000);   // (d8,PC,Xn)
    UNKN(0xF000|074, 0x2000);   // #n

    UNKN(0xF140|002);           // D2
    UNKN(0xF140|012);           // A2
    TEST("PRESTORE", "(A2)",           0xF140|022);
    TEST("PRESTORE", "(A2)+",          0xF140|032);
    UNKN(0xF140|042);           // -(A2)
    TEST("PRESTORE", "($1234,A2)",     0xF140|052, 0x1234);
    TEST("PRESTORE", "($34,A2,D3.W)",  0xF140|062, 0x3034);
    TEST("PRESTORE", "($00001234).W",  0xF140|070, 0x1234);
    TEST("PRESTORE", "($12345678).L",  0xF140|071, 0x1234, 0x5678);
    TEST("PRESTORE", "(*+$1234,PC)",   0xF140|072, 0x1232);
    TEST("PRESTORE", "(*+52,PC,D3.W)", 0xF140|073, 0x3032);
    UNKN(0xF140|074);           // #n

    UNKN(0xF100|002);           // D2
    UNKN(0xF100|012);           // A2
    TEST("PSAVE", "(A2)",           0xF100|022);
    UNKN(0xF100|032);           // (A2)+
    TEST("PSAVE", "-(A2)",          0xF100|042);
    TEST("PSAVE", "($1234,A2)",     0xF100|052, 0x1234);
    TEST("PSAVE", "($34,A2,D3.W)",  0xF100|062, 0x3034);
    TEST("PSAVE", "($00001234).W",  0xF100|070, 0x1234);
    TEST("PSAVE", "($12345678).L",  0xF100|071, 0x1234, 0x5678);
    UNKN(0xF100|072);           // (d16,PC)
    UNKN(0xF100|073);           // (d8,PC,Xn)
    UNKN(0xF100|074);           // #n

    UNKN(0xF000|002, 0x8200);   // D2
    UNKN(0xF000|012, 0x8200);   // A2
    TEST("PTESTR", "#7, (A2), #4",           0xF000|022, 0x8210|(4<<10)|7);
    UNKN(0xF000|032, 0x8200);   // (A2)+
    UNKN(0xF000|042, 0x8200);   // -(A2)
    TEST("PTESTR", "D3, ($1234,A2), #7",     0xF000|052, 0x8208|(7<<10)|3, 0x1234);
    TEST("PTESTR", "SFC, ($34,A2,D3.W), #1", 0xF000|062, 0x8200|(1<<10)|0, 0x3034);
    TEST("PTESTR", "DFC, ($00001234).W, #5", 0xF000|070, 0x8200|(5<<10)|1, 0x1234);
    TEST("PTESTR", "#15, ($12345678).L, #2", 0xF000|071, 0x8210|(2<<10)|15, 0x1234, 0x5678);
    UNKN(0xF000|072, 0x8200);   // (d16,PC)
    UNKN(0xF000|073, 0x8200);   // (d8,PC,Xn)
    UNKN(0xF000|074, 0x8200);   // #n

    UNKN(0xF000|002, 0x8300);   // D2
    UNKN(0xF000|012, 0x8300);   // A2
    TEST("PTESTR", "#7, (A2), #4, A3",           0xF000|022, 0x8310|(4<<10)|7|(3<<5));
    UNKN(0xF000|032, 0x8300);   // (A2)+
    UNKN(0xF000|042, 0x8300);   // -(A2)
    TEST("PTESTR", "D3, ($1234,A2), #7, A4",     0xF000|052, 0x8308|(7<<10)|3|(4<<5), 0x1234);
    TEST("PTESTR", "SFC, ($34,A2,D3.W), #1, A3", 0xF000|062, 0x8300|(1<<10)|0|(3<<5), 0x3034);
    TEST("PTESTR", "DFC, ($00001234).W, #5, A3", 0xF000|070, 0x8300|(5<<10)|1|(3<<5), 0x1234);
    TEST("PTESTR", "#9, ($12345678).L, #2, A3",  0xF000|071, 0x8310|(2<<10)|9|(3<<5),
         0x1234, 0x5678);
    UNKN(0xF000|072, 0x8300);   // (d16,PC)
    UNKN(0xF000|073, 0x8300);   // (d8,PC,Xn)
    UNKN(0xF000|074, 0x8300);   // #n

    UNKN(0xF000|002, 0x8000);   // D2
    UNKN(0xF000|012, 0x8000);   // A2
    TEST("PTESTW", "#7, (A2), #4",           0xF000|022, 0x8010|(4<<10)|7);
    UNKN(0xF000|032, 0x8000);   // (A2)+
    UNKN(0xF000|042, 0x8000);   // -(A2)
    TEST("PTESTW", "D3, ($1234,A2), #7",     0xF000|052, 0x8008|(7<<10)|3, 0x1234);
    TEST("PTESTW", "SFC, ($34,A2,D3.W), #1", 0xF000|062, 0x8000|(1<<10)|0, 0x3034);
    TEST("PTESTW", "DFC, ($00001234).W, #5", 0xF000|070, 0x8000|(5<<10)|1, 0x1234);
    TEST("PTESTW", "#15, ($12345678).L, #2", 0xF000|071, 0x8010|(2<<10)|15, 0x1234, 0x5678);
    UNKN(0xF000|072, 0x8000);   // (d16,PC)
    UNKN(0xF000|073, 0x8000);   // (d8,PC,Xn)
    UNKN(0xF000|074, 0x8000);   // #n

    UNKN(0xF000|002, 0x8100);   // D2
    UNKN(0xF000|012, 0x8100);   // A2
    TEST("PTESTW", "#7, (A2), #4, A3",           0xF000|022, 0x8110|(4<<10)|7|(3<<5));
    UNKN(0xF000|032, 0x8100);   // (A2)+
    UNKN(0xF000|042, 0x8100);   // -(A2)
    TEST("PTESTW", "D3, ($1234,A2), #7, A4",     0xF000|052, 0x8108|(7<<10)|3|(4<<5), 0x1234);
    TEST("PTESTW", "SFC, ($34,A2,D3.W), #1, A3", 0xF000|062, 0x8100|(1<<10)|0|(3<<5), 0x3034);
    TEST("PTESTW", "DFC, ($00001234).W, #5, A3", 0xF000|070, 0x8100|(5<<10)|1|(3<<5), 0x1234);
    TEST("PTESTW", "#9, ($12345678).L, #2, A3",  0xF000|071, 0x8110|(2<<10)|9|(3<<5),
         0x1234, 0x5678);
    UNKN(0xF000|072, 0x8100);   // (d16,PC)
    UNKN(0xF000|073, 0x8100);   // (d8,PC,Xn)
    UNKN(0xF000|074, 0x8100);   // #n

    UNKN(0xF000|002, 0x2800);   // D2
    UNKN(0xF000|012, 0x2800);   // A2
    TEST("PVALID", "VAL, (A2)",          0xF000|022, 0x2800);
    UNKN(0xF000|032, 0x2800);   // (A2)+
    UNKN(0xF000|042, 0x2800);   // -(A2)
    TEST("PVALID", "VAL, ($1234,A2)",    0xF000|052, 0x2800, 0x1234);
    TEST("PVALID", "VAL, ($34,A2,D3.W)", 0xF000|062, 0x2800, 0x3034);
    TEST("PVALID", "VAL, ($00001234).W", 0xF000|070, 0x2800, 0x1234);
    TEST("PVALID", "VAL, ($12345678).L", 0xF000|071, 0x2800, 0x1234, 0x5678);
    UNKN(0xF000|072, 0x2800);   // (d16,PC)
    UNKN(0xF000|073, 0x2800);   // (d8,PC,Xn)
    UNKN(0xF000|074, 0x2800);   // #n

    UNKN(0xF000|002, 0x2C00);   // D2
    UNKN(0xF000|012, 0x2C00);   // A2
    TEST("PVALID", "A3, (A2)",          0xF000|022, 0x2C00|3);
    UNKN(0xF000|032, 0x2C00);   // (A2)+
    UNKN(0xF000|042, 0x2C00);   // -(A2)
    TEST("PVALID", "A4, ($1234,A2)",    0xF000|052, 0x2C00|4, 0x1234);
    TEST("PVALID", "A5, ($34,A2,D3.W)", 0xF000|062, 0x2C00|5, 0x3034);
    TEST("PVALID", "A6, ($00001234).W", 0xF000|070, 0x2C00|6, 0x1234);
    TEST("PVALID", "A7, ($12345678).L", 0xF000|071, 0x2C00|7, 0x1234, 0x5678);
    UNKN(0xF000|072, 0x2C00);   // (d16,PC)
    UNKN(0xF000|073, 0x2C00);   // (d8,PC,Xn)
    UNKN(0xF000|074, 0x2C00);   // #n
}
#endif

// clang-format on

void run_tests(const char *cpu) {
    disassembler.setCpu(cpu);
    RUN_TEST(test_data_move);
    RUN_TEST(test_integer);
    RUN_TEST(test_logical);
    RUN_TEST(test_shift_rotate);
    RUN_TEST(test_bit);
    RUN_TEST(test_bcd);
    RUN_TEST(test_program);
    RUN_TEST(test_system);
    RUN_TEST(test_multiproc);
    RUN_TEST(test_addressing);
    if (!firstGen())
        RUN_TEST(test_bitfield);
#if !defined(LIBASM_MC68000_NOFPU)
    RUN_TEST(test_float_move);
    RUN_TEST(test_float_arithmetic);
    RUN_TEST(test_float_branch);
    RUN_TEST(test_float_trap);
    RUN_TEST(test_float_system);
#endif
#if !defined(LIBASM_MC68000_NOPMMU)
    if (mc68020()) {
        RUN_TEST(test_pmove_mc68851);
        RUN_TEST(test_branch_mc68851);
        RUN_TEST(test_trap_mc68851);
        RUN_TEST(test_pmmu_mc68851);
    }
    if (mc68030()) {
        RUN_TEST(test_pmove_mc68030);
        RUN_TEST(test_pmmu_mc68030);
    }
#endif
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

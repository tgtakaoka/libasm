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

static void set_up() {
    disassembler.reset();
    disassembler.setOption("relative", "true");
}

static void tear_down() {
    symtab.reset();
}

// clang-format off
void test_cpu() {
    EQUALS("cpu 68000", true,    disassembler.setCpu("68000"));
    EQUALS_P("cpu 68000", "68000", disassembler.cpu_P());

    EQUALS("cpu MC68000", true,    disassembler.setCpu("MC68000"));
    EQUALS_P("cpu MC68000", "68000", disassembler.cpu_P());
}

static void test_data_move() {
    // EXG Rx,Ry
    TEST("EXG", "D1, D2", 0141502); // 014|Dx|50|Dy
    TEST("EXG", "A1, A2", 0141512); // 014|Ax|51|Ay
    TEST("EXG", "D1, A2", 0141612); // 014|Dx|61|Ay

    // LEA ea,Ad: 004|An|7|M|Rn
    UNKN(                             0041702);
    UNKN(                             0041712);
    TEST("LEA", "(A2), A1",           0041722);
    UNKN(                             0041732);
    UNKN(                             0041742);
    TEST("LEA", "($1234,A2), A1",     0041752, 0x1234);
    TEST("LEA", "(18,A2,D3.W), A1",   0041762, 0x3012);
    TEST("LEA", "($FFFF00).W, A1",    0041770, 0xFF00);
    TEST("LEA", "($001234).L, A1",    0041771, 0x0000, 0x1234);
    TEST("LEA", "(*+$1234,PC), A1",   0041772, 0x1232);
    TEST("LEA", "(*+18,PC,A3.L), A1", 0041773, 0xB810);
    UNKN(                             0041774);

    // LINK An, #nn: 004712|An
    TEST("LINK", "A3, #$1234", 0047123, 0x1234);
    TEST("LINK", "A3, #-16",   0047123, 0xFFF0);

    // MOVE src,dst: 00|Sz|Rd|Md|Ms|Rs, Sz:B=1/W=3/L=2
    TEST("MOVE.B", "D2, D7",             0017002);
    UNKN(                                0017012); // A2, D7
    TEST("MOVE.B", "(A2), D7",           0017022);
    TEST("MOVE.B", "(A2)+, D7",          0017032);
    TEST("MOVE.B", "-(A2), D7",          0017042);
    TEST("MOVE.B", "($1234,A2), D7",     0017052, 0x1234);
    TEST("MOVE.B", "(18,A2,D3.L), D7",   0017062, 0x3812);
    TEST("MOVE.B", "($FFFFFF).W, D7",    0017070, 0xFFFF);
    TEST("MOVE.B", "($123456).L, D7",    0017071, 0x0012, 0x3456);
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
    TEST("MOVE.W", "($FFFFFE).W, D7",    0037070, 0xFFFE);
    TEST("MOVE.W", "($123456).L, D7",    0037071, 0x0012, 0x3456);
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
    TEST("MOVE.L", "($FFFFFC).W, D7",    0027070, 0xFFFC);
    TEST("MOVE.L", "($123454).L, D7",    0027071, 0x0012, 0x3454);
    TEST("MOVE.L", "(*+$1234,PC), D7",   0027072, 0x1232);
    TEST("MOVE.L", "(*+18,PC,D3.W), D7", 0027073, 0x3010);
    TEST("MOVE.L", "#$00345678, D7",     0027074, 0x0034, 0x5678);
    TEST("MOVE.B", "D2, (A6)",             0016202);
    UNKN(                                  0016212); // A2, (A6)
    TEST("MOVE.B", "(A2), (A6)",           0016222);
    TEST("MOVE.B", "(A2)+, (A6)",          0016232);
    TEST("MOVE.B", "-(A2), (A6)",          0016242);
    TEST("MOVE.B", "($1234,A2), (A6)",     0016252, 0x1234);
    TEST("MOVE.B", "(18,A2,D3.L), (A6)",   0016262, 0x3812);
    TEST("MOVE.B", "($FFFFFF).W, (A6)",    0016270, 0xFFFF);
    TEST("MOVE.B", "($123456).L, (A6)",    0016271, 0x0012, 0x3456);
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
    TEST("MOVE.W", "($FFFFFE).W, (A6)",    0036270, 0xFFFE);
    TEST("MOVE.W", "($123456).L, (A6)",    0036271, 0x0012, 0x3456);
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
    TEST("MOVE.L", "($FFFFFC).W, (A6)",    0026270, 0xFFFC);
    TEST("MOVE.L", "($123454).L, (A6)",    0026271, 0x0012, 0x3454);
    TEST("MOVE.L", "(*+$1234,PC), (A6)",   0026272, 0x1232);
    TEST("MOVE.L", "(*+18,PC,D3.W), (A6)", 0026273, 0x3010);
    TEST("MOVE.L", "#$00345678, (A6)",     0026274, 0x0034, 0x5678);
    TEST("MOVE.B", "D2, (A6)+",             0016302);
    UNKN(                                   0016312); // A2, (A6)+
    TEST("MOVE.B", "(A2), (A6)+",           0016322);
    TEST("MOVE.B", "(A2)+, (A6)+",          0016332);
    TEST("MOVE.B", "-(A2), (A6)+",          0016342);
    TEST("MOVE.B", "($1234,A2), (A6)+",     0016352, 0x1234);
    TEST("MOVE.B", "(18,A2,D3.L), (A6)+",   0016362, 0x3812);
    TEST("MOVE.B", "($FFFFFF).W, (A6)+",    0016370, 0xFFFF);
    TEST("MOVE.B", "($123456).L, (A6)+",    0016371, 0x0012, 0x3456);
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
    TEST("MOVE.W", "($FFFFFE).W, (A6)+",    0036370, 0xFFFE);
    TEST("MOVE.W", "($123456).L, (A6)+",    0036371, 0x0012, 0x3456);
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
    TEST("MOVE.L", "($FFFFFC).W, (A6)+",    0026370, 0xFFFC);
    TEST("MOVE.L", "($123454).L, (A6)+",    0026371, 0x0012, 0x3454);
    TEST("MOVE.L", "(*+$1234,PC), (A6)+",   0026372, 0x1232);
    TEST("MOVE.L", "(*+18,PC,D3.W), (A6)+", 0026373, 0x3010);
    TEST("MOVE.L", "#$00345678, (A6)+",     0026374, 0x0034, 0x5678);
    TEST("MOVE.B", "D2, -(A6)",             0016402);
    UNKN(                                   0016412); // A2, -(A6)
    TEST("MOVE.B", "(A2), -(A6)",           0016422);
    TEST("MOVE.B", "(A2)+, -(A6)",          0016432);
    TEST("MOVE.B", "-(A2), -(A6)",          0016442);
    TEST("MOVE.B", "($1234,A2), -(A6)",     0016452, 0x1234);
    TEST("MOVE.B", "(18,A2,D3.L), -(A6)",   0016462, 0x3812);
    TEST("MOVE.B", "($FFFFFF).W, -(A6)",    0016470, 0xFFFF);
    TEST("MOVE.B", "($123456).L, -(A6)",    0016471, 0x0012, 0x3456);
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
    TEST("MOVE.W", "($FFFFFE).W, -(A6)",    0036470, 0xFFFE);
    TEST("MOVE.W", "($123456).L, -(A6)",    0036471, 0x0012, 0x3456);
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
    TEST("MOVE.L", "($FFFFFC).W, -(A6)",    0026470, 0xFFFC);
    TEST("MOVE.L", "($123454).L, -(A6)",    0026471, 0x0012, 0x3454);
    TEST("MOVE.L", "(*+$1234,PC), -(A6)",   0026472, 0x1232);
    TEST("MOVE.L", "(*+18,PC,D3.W), -(A6)", 0026473, 0x3010);
    TEST("MOVE.L", "#$00345678, -(A6)",     0026474, 0x0034, 0x5678);
    TEST("MOVE.B", "D2, ($5678,A6)",             0016502, 0x5678);
    UNKN(                                        0016512); // A2, ($5678,A6)
    TEST("MOVE.B", "(A2), ($5678,A6)",           0016522, 0x5678);
    TEST("MOVE.B", "(A2)+, ($5678,A6)",          0016532, 0x5678);
    TEST("MOVE.B", "-(A2), ($5678,A6)",          0016542, 0x5678);
    TEST("MOVE.B", "($1234,A2), ($5678,A6)",     0016552, 0x1234, 0x5678);
    TEST("MOVE.B", "(18,A2,D3.L), ($5678,A6)",   0016562, 0x3812, 0x5678);
    TEST("MOVE.B", "($FFFFFF).W, ($5678,A6)",    0016570, 0xFFFF, 0x5678);
    TEST("MOVE.B", "($123456).L, ($5678,A6)",    0016571, 0x0012, 0x3456, 0x5678);
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
    TEST("MOVE.W", "($FFFFFE).W, ($5678,A6)",    0036570, 0xFFFE, 0x5678);
    TEST("MOVE.W", "($123456).L, ($5678,A6)",    0036571, 0x0012, 0x3456, 0x5678);
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
    TEST("MOVE.L", "($FFFFFC).W, ($5678,A6)",    0026570, 0xFFFC, 0x5678);
    TEST("MOVE.L", "($123454).L, ($5678,A6)",    0026571, 0x0012, 0x3454, 0x5678);
    TEST("MOVE.L", "(*+$1234,PC), ($5678,A6)",   0026572, 0x1232, 0x5678);
    TEST("MOVE.L", "(*+18,PC,D3.W), ($5678,A6)", 0026573, 0x3010, 0x5678);
    TEST("MOVE.L", "#$00345678, ($5678,A6)",     0026574, 0x0034, 0x5678, 0x5678);
    TEST("MOVE.B", "D2, ($56,A6,D5.W)",             0016602, 0x5056);
    UNKN(                                           0016612); // A2, ($56,A6,D5.W)
    TEST("MOVE.B", "(A2), ($56,A6,D5.W)",           0016622, 0x5056);
    TEST("MOVE.B", "(A2)+, ($56,A6,D5.W)",          0016632, 0x5056);
    TEST("MOVE.B", "-(A2), ($56,A6,D5.W)",          0016642, 0x5056);
    TEST("MOVE.B", "($1234,A2), ($56,A6,D5.W)",     0016652, 0x1234, 0x5056);
    TEST("MOVE.B", "(18,A2,D3.L), ($56,A6,D5.W)",   0016662, 0x3812, 0x5056);
    TEST("MOVE.B", "($FFFFFF).W, ($56,A6,D5.W)",    0016670, 0xFFFF, 0x5056);
    TEST("MOVE.B", "($123456).L, ($56,A6,D5.W)",    0016671, 0x0012, 0x3456, 0x5056);
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
    TEST("MOVE.W", "($FFFFFE).W, ($56,A6,D5.W)",    0036670, 0xFFFE, 0x5056);
    TEST("MOVE.W", "($123456).L, ($56,A6,D5.W)",    0036671, 0x0012, 0x3456, 0x5056);
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
    TEST("MOVE.L", "($FFFFFC).W, ($56,A6,D5.W)",    0026670, 0xFFFC, 0x5056);
    TEST("MOVE.L", "($123454).L, ($56,A6,D5.W)",    0026671, 0x0012, 0x3454, 0x5056);
    TEST("MOVE.L", "(*+$1234,PC), ($56,A6,D5.W)",   0026672, 0x1232, 0x5056);
    TEST("MOVE.L", "(*+18,PC,D3.W), ($56,A6,D5.W)", 0026673, 0x3010, 0x5056);
    TEST("MOVE.L", "#$00345678, ($56,A6,D5.W)",     0026674, 0x0034, 0x5678, 0x5056);
    TEST("MOVE.B", "D2, ($005678).W",             0010702, 0x5678);
    UNKN(                                         0010712); // A2, ($005678).W
    TEST("MOVE.B", "(A2), ($005678).W",           0010722, 0x5678);
    TEST("MOVE.B", "(A2)+, ($005678).W",          0010732, 0x5678);
    TEST("MOVE.B", "-(A2), ($005678).W",          0010742, 0x5678);
    TEST("MOVE.B", "($1234,A2), ($005678).W",     0010752, 0x1234, 0x5678);
    TEST("MOVE.B", "(18,A2,D3.L), ($005678).W",   0010762, 0x3812, 0x5678);
    TEST("MOVE.B", "($FFFFFF).W, ($005678).W",    0010770, 0xFFFF, 0x5678);
    TEST("MOVE.B", "($123456).L, ($005678).W",    0010771, 0x0012, 0x3456, 0x5678);
    TEST("MOVE.B", "(*+$1234,PC), ($005678).W",   0010772, 0x1232, 0x5678);
    TEST("MOVE.B", "(*+18,PC,D3.W), ($005678).W", 0010773, 0x3010, 0x5678);
    TEST("MOVE.B", "#$34, ($005678).W",           0010774, 0x0034, 0x5678);
    TEST("MOVE.W", "D2, ($005678).W",             0030702, 0x5678);
    TEST("MOVE.W", "A2, ($005678).W",             0030712, 0x5678);
    TEST("MOVE.W", "(A2), ($005678).W",           0030722, 0x5678);
    TEST("MOVE.W", "(A2)+, ($005678).W",          0030732, 0x5678);
    TEST("MOVE.W", "-(A2), ($005678).W",          0030742, 0x5678);
    TEST("MOVE.W", "($1234,A2), ($005678).W",     0030752, 0x1234, 0x5678);
    TEST("MOVE.W", "(18,A2,D3.L), ($005678).W",   0030762, 0x3812, 0x5678);
    TEST("MOVE.W", "($FFFFFE).W, ($005678).W",    0030770, 0xFFFE, 0x5678);
    TEST("MOVE.W", "($123456).L, ($005678).W",    0030771, 0x0012, 0x3456, 0x5678);
    TEST("MOVE.W", "(*+$1234,PC), ($005678).W",   0030772, 0x1232, 0x5678);
    TEST("MOVE.W", "(*+18,PC,D3.W), ($005678).W", 0030773, 0x3010, 0x5678);
    TEST("MOVE.W", "#$0034, ($005678).W",         0030774, 0x0034, 0x5678);
    TEST("MOVE.L", "D2, ($005678).W",             0020702, 0x5678);
    TEST("MOVE.L", "A2, ($005678).W",             0020712, 0x5678);
    TEST("MOVE.L", "(A2), ($005678).W",           0020722, 0x5678);
    TEST("MOVE.L", "(A2)+, ($005678).W",          0020732, 0x5678);
    TEST("MOVE.L", "-(A2), ($005678).W",          0020742, 0x5678);
    TEST("MOVE.L", "($1234,A2), ($005678).W",     0020752, 0x1234, 0x5678);
    TEST("MOVE.L", "(18,A2,D3.L), ($005678).W",   0020762, 0x3812, 0x5678);
    TEST("MOVE.L", "($FFFFFC).W, ($005678).W",    0020770, 0xFFFC, 0x5678);
    TEST("MOVE.L", "($123454).L, ($005678).W",    0020771, 0x0012, 0x3454, 0x5678);
    TEST("MOVE.L", "(*+$1234,PC), ($005678).W",   0020772, 0x1232, 0x5678);
    TEST("MOVE.L", "(*+18,PC,D3.W), ($005678).W", 0020773, 0x3010, 0x5678);
    TEST("MOVE.L", "#$00345678, ($005678).W",     0020774, 0x0034, 0x5678, 0x5678);
    TEST("MOVE.B", "D2, ($56789A).L",             0011702, 0x0056, 0x789A);
    UNKN(                                         0011712); // A2, ($56789A).L
    TEST("MOVE.B", "(A2), ($56789A).L",           0011722, 0x0056, 0x789A);
    TEST("MOVE.B", "(A2)+, ($56789A).L",          0011732, 0x0056, 0x789A);
    TEST("MOVE.B", "-(A2), ($56789A).L",          0011742, 0x0056, 0x789A);
    TEST("MOVE.B", "($1234,A2), ($56789A).L",     0011752, 0x1234, 0x0056, 0x789A);
    TEST("MOVE.B", "(18,A2,D3.L), ($56789A).L",   0011762, 0x3812, 0x0056, 0x789A);
    TEST("MOVE.B", "($FFFFFF).W, ($56789A).L",    0011770, 0xFFFF, 0x0056, 0x789A);
    TEST("MOVE.B", "($123456).L, ($56789A).L",    0011771, 0x0012, 0x3456, 0x0056, 0x789A);
    TEST("MOVE.B", "(*+$1234,PC), ($56789A).L",   0011772, 0x1232, 0x0056, 0x789A);
    TEST("MOVE.B", "(*+18,PC,D3.W), ($56789A).L", 0011773, 0x3010, 0x0056, 0x789A);
    TEST("MOVE.B", "#$34, ($56789A).L",           0011774, 0x0034, 0x0056, 0x789A);
    TEST("MOVE.W", "D2, ($56789A).L",             0031702, 0x0056, 0x789A);
    TEST("MOVE.W", "A2, ($56789A).L",             0031712, 0x0056, 0x789A);
    TEST("MOVE.W", "(A2), ($56789A).L",           0031722, 0x0056, 0x789A);
    TEST("MOVE.W", "(A2)+, ($56789A).L",          0031732, 0x0056, 0x789A);
    TEST("MOVE.W", "-(A2), ($56789A).L",          0031742, 0x0056, 0x789A);
    TEST("MOVE.W", "($1234,A2), ($56789A).L",     0031752, 0x1234, 0x0056, 0x789A);
    TEST("MOVE.W", "(18,A2,D3.L), ($56789A).L",   0031762, 0x3812, 0x0056, 0x789A);
    TEST("MOVE.W", "($FFFFFE).W, ($56789A).L",    0031770, 0xFFFE, 0x0056, 0x789A);
    TEST("MOVE.W", "($123456).L, ($56789A).L",    0031771, 0x0012, 0x3456, 0x0056, 0x789A);
    TEST("MOVE.W", "(*+$1234,PC), ($56789A).L",   0031772, 0x1232, 0x0056, 0x789A);
    TEST("MOVE.W", "(*+18,PC,D3.W), ($56789A).L", 0031773, 0x3010, 0x0056, 0x789A);
    TEST("MOVE.W", "#$0034, ($56789A).L",         0031774, 0x0034, 0x0056, 0x789A);
    TEST("MOVE.L", "D2, ($567898).L",             0021702, 0x0056, 0x7898);
    TEST("MOVE.L", "A2, ($567898).L",             0021712, 0x0056, 0x7898);
    TEST("MOVE.L", "(A2), ($567898).L",           0021722, 0x0056, 0x7898);
    TEST("MOVE.L", "(A2)+, ($567898).L",          0021732, 0x0056, 0x7898);
    TEST("MOVE.L", "-(A2), ($567898).L",          0021742, 0x0056, 0x7898);
    TEST("MOVE.L", "($1234,A2), ($567898).L",     0021752, 0x1234, 0x0056, 0x7898);
    TEST("MOVE.L", "(18,A2,D3.L), ($567898).L",   0021762, 0x3812, 0x0056, 0x7898);
    TEST("MOVE.L", "($FFFFFC).W, ($567898).L",    0021770, 0xFFFC, 0x0056, 0x7898);
    TEST("MOVE.L", "($123454).L, ($567898).L",    0021771, 0x0012, 0x3454, 0x0056, 0x7898);
    TEST("MOVE.L", "(*+$1234,PC), ($567898).L",   0021772, 0x1232, 0x0056, 0x7898);
    TEST("MOVE.L", "(*+18,PC,D3.W), ($567898).L", 0021773, 0x3010, 0x0056, 0x7898);
    TEST("MOVE.L", "#$00345678, ($567898).L",     0021774, 0x0034, 0x5678, 0x0056, 0x7898);
    UNKN(                                         0012702); // D2, (*+$1234,PC)
    UNKN(                                         0033702); // D2, (*+3,PC,D0.W)
    UNKN(                                         0024702); // D2, #$12345678

    // MOVEA src,An: 00|Sz|An|1|M|Rn, Sz:W=3/L=2
    TEST("MOVEA.W", "D2, A6",             0036102);
    TEST("MOVEA.W", "A2, A6",             0036112);
    TEST("MOVEA.W", "(A2), A6",           0036122);
    TEST("MOVEA.W", "(A2)+, A6",          0036132);
    TEST("MOVEA.W", "-(A2), A6",          0036142);
    TEST("MOVEA.W", "($1234,A2), A6",     0036152, 0x1234);
    TEST("MOVEA.W", "(18,A2,D3.L), A6",   0036162, 0x3812);
    TEST("MOVEA.W", "($FFFFFE).W, A6",    0036170, 0xFFFE);
    TEST("MOVEA.W", "($123456).L, A6",    0036171, 0x0012, 0x3456);
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
    TEST("MOVEA.L", "($FFFFFC).W, A6",    0026170, 0xFFFC);
    TEST("MOVEA.L", "($123454).L, A6",    0026171, 0x0012, 0x3454);
    TEST("MOVEA.L", "(*+$1234,PC), A6",   0026172, 0x1232);
    TEST("MOVEA.L", "(*+18,PC,D3.W), A6", 0026173, 0x3010);
    TEST("MOVEA.L", "#$00345678, A6",     0026174, 0x0034, 0x5678);

    // MOVEM list,dst: 0044|Sz|Md|Rd, Sz:W=2/L=3, list=A7|...|D0, -(An)=D0|...|A7
    TEST("EXT.W",   "D2",                  0044202); // EXT.W
    UNKN(                                  0044212); // D0-D6, A2
    TEST("MOVEM.W", "D0-D6, (A2)",         0044222, 0x007F);
    UNKN(                                  0044232); // D0-D6, (A2)+
    TEST("MOVEM.W", "D0-D6, -(A2)",        0044242, 0xFE00);
    TEST("MOVEM.W", "D0-D6, ($1234,A2)",   0044252, 0x007F, 0x1234);
    TEST("MOVEM.W", "D0-D6, (18,A2,A3.L)", 0044262, 0x007F, 0xB812);
    TEST("MOVEM.W", "D0-D6, ($FFFEDC).W",  0044270, 0x007F, 0xFEDC);
    TEST("MOVEM.W", "D0-D6, ($123456).L",  0044271, 0x007F, 0x0012, 0x3456);
    UNKN(                                  0044272); // D0-D6, (*+$1234,PC)
    UNKN(                                  0044273); // D0-D6, (*+10,PC,A3.L)
    UNKN(                                  0044274); // D0-D6, #$1234
    TEST("MOVEM.W", "D0, (A2)",            0044222, 0x0001);
    TEST("MOVEM.W", "A0, (A2)",            0044222, 0x0100);
    TEST("MOVEM.W", "D0, -(A2)",           0044242, 0x8000);
    TEST("MOVEM.W", "A0, -(A2)",           0044242, 0x0080);
    TEST("EXT.L",   "D2",                  0044302); // EXT.L
    UNKN(                                  0044312); // D1/A0, A2
    TEST("MOVEM.L", "D1/A0, (A2)",         0044322, 0x0102);
    UNKN(                                  0044332); // D1/A0, (A2)+
    TEST("MOVEM.L", "D1/A0, -(A2)",        0044342, 0x4080);
    TEST("MOVEM.L", "D1/A0, ($1234,A2)",   0044352, 0x0102, 0x1234);
    TEST("MOVEM.L", "D1/A0, (18,A2,A3.L)", 0044362, 0x0102, 0xB812);
    TEST("MOVEM.L", "D1/A0, ($FFFEDC).W",  0044370, 0x0102, 0xFEDC);
    TEST("MOVEM.L", "D1/A0, ($123454).L",  0044371, 0x0102, 0x0012, 0x3454);
    UNKN(                                  0044372); // D1/A0, (*+$1234,PC)
    UNKN(                                  0044373); // D1/A0, (*+10,PC,A3.L)
    UNKN(                                  0044374); // D1/A0, #$12345678
    TEST("MOVEM.L", "D0, (A2)",            0044322, 0x0001);
    TEST("MOVEM.L", "A0, (A2)",            0044322, 0x0100);
    TEST("MOVEM.L", "D0, -(A2)",           0044342, 0x8000);
    TEST("MOVEM.L", "A0, -(A2)",           0044342, 0x0080);

    // MOVEM src,list: 0046|Sz|Md|Rd, Sz:W=2/L=3, list=A7|...|D0
    UNKN(                                    0046202); // D2, A3-A6
    UNKN(                                    0046212); // A2, A3-A6
    TEST("MOVEM.W", "(A2), A3-A6",           0046222, 0x7800);
    TEST("MOVEM.W", "(A2)+, A3-A6",          0046232, 0x7800);
    UNKN(                                    0046242); // -(A2), D1-D4
    TEST("MOVEM.W", "($1234,A2), A3-A6",     0046252, 0x7800, 0x1234);
    TEST("MOVEM.W", "(18,A2,D3.W), A3-A6",   0046262, 0x7800, 0x3012);
    TEST("MOVEM.W", "($FFFEDC).W, A3-A6",    0046270, 0x7800, 0xFEDC);
    TEST("MOVEM.W", "($123456).L, A3-A6",    0046271, 0x7800, 0x0012, 0x3456);
    TEST("MOVEM.W", "(*+$1234,PC), A3-A6",   0046272, 0x7800, 0x1230);
    TEST("MOVEM.W", "(*+18,PC,D3.L), A3-A6", 0046273, 0x7800, 0x380E);
    UNKN(                                    0046274); // #$1234, A3-A6
    TEST("MOVEM.W", "(A2), D0",              0046222, 0x0001);
    TEST("MOVEM.W", "(A2), A0",              0046222, 0x0100);
    TEST("MOVEM.W", "(A2)+, D0",             0046232, 0x0001);
    TEST("MOVEM.W", "(A2)+, A0",             0046232, 0x0100);
    UNKN(                                    0046302); // D2, A3-A6
    UNKN(                                    0046312); // A2, A3-A6
    TEST("MOVEM.L", "(A2), A3-A6",           0046322, 0x7800);
    TEST("MOVEM.L", "(A2)+, A3-A6",          0046332, 0x7800);
    UNKN(                                    0046342); // -(A2), D1-D4
    TEST("MOVEM.L", "($1234,A2), A3-A6",     0046352, 0x7800, 0x1234);
    TEST("MOVEM.L", "(18,A2,D3.W), A3-A6",   0046362, 0x7800, 0x3012);
    TEST("MOVEM.L", "($FFFEDC).W, A3-A6",    0046370, 0x7800, 0xFEDC);
    TEST("MOVEM.L", "($123454).L, A3-A6",    0046371, 0x7800, 0x0012, 0x3454);
    TEST("MOVEM.L", "(*+$1234,PC), A3-A6",   0046372, 0x7800, 0x1230);
    TEST("MOVEM.L", "(*+18,PC,D3.L), A3-A6", 0046373, 0x7800, 0x380E);
    UNKN(                                    0046374); // #$12345678, A3-A6
    TEST("MOVEM.L", "(A2), D0",              0046322, 0x0001);
    TEST("MOVEM.L", "(A2), A0",              0046322, 0x0100);
    TEST("MOVEM.L", "(A2)+, D0",             0046332, 0x0001);
    TEST("MOVEM.L", "(A2)+, A0",             0046332, 0x0100);

    // MOVEP Dn, (d16,An): 000|Dn|Sz|1|An, SZ:W=6/L=7
    TEST("MOVEP.W", "D7, ($1234,A2)", 0007612, 0x1234);
    TEST("MOVEP.L", "D7, ($1234,A2)", 0007712, 0x1234);

    // MOVEP (d16,An),Dn: 000|Dn|Sz|1|An, SZ:W=4/L=5
    TEST("MOVEP.W", "($1234,A2), D7", 0007412, 0x1234);
    TEST("MOVEP.L", "($1234,A2), D7", 0007512, 0x1234);

    // MOVEQ #nn,Dn: 007|Dn|000 + nn
    TEST("MOVEQ", "#0, D0",    0070000 | 0x00);
    TEST("MOVEQ", "#$7F, D0",  0070000 | 0x7F);
    TEST("MOVEQ", "#-$80, D0", 0070000 | 0x80);
    TEST("MOVEQ", "#-1, D7",   0077000 | 0xFF);

    // PEA src: 00441|M|Rn
    TEST("SWAP", "D2",            0044102); // SWAP
    UNKN(                         0044112); // A2
    TEST("PEA", "(A2)",           0044122);
    UNKN(                         0044132); // (A2)+
    UNKN(                         0044142); // -(A2)
    TEST("PEA", "($1234,A2)",     0044152, 0x1234);
    TEST("PEA", "(18,A2,D3.W)",   0044162, 0x3012);
    TEST("PEA", "($FFFF00).W",    0044170, 0xFF00);
    TEST("PEA", "($001234).L",    0044171, 0x0000, 0x1234);
    TEST("PEA", "(*+$1234,PC)",   0044172, 0x1232);
    TEST("PEA", "(*+18,PC,A3.L)", 0044173, 0xB810);
    UNKN(                         0044174); // #$12345678

    // UNLK An: 004713|An
    TEST("UNLK", "A0", 0047130);
    TEST("UNLK", "A7", 0047137);
}

static void test_integer() {
    // ADD Dn,dst: 015|Dn|Sz|M|Rn, Sz:B=4/W=5/L=6
    TEST("ADDX.B", "D2, D7",          0157402); // ADDX.B
    TEST("ADDX.B", "-(A2), -(A7)",    0157412); // ADDX.B
    TEST("ADD.B", "D7, (A2)",         0157422);
    TEST("ADD.B", "D7, (A2)+",        0157432);
    TEST("ADD.B", "D7, -(A2)",        0157442);
    TEST("ADD.B", "D7, ($1234,A2)",   0157452, 0x1234);
    TEST("ADD.B", "D7, (18,A2,D3.L)", 0157462, 0x3812);
    TEST("ADD.B", "D7, ($FFFFFF).W",  0157470, 0xFFFF);
    TEST("ADD.B", "D7, ($123456).L",  0157471, 0x0012, 0x3456);
    UNKN(                             0157472); // D7, (*+$1234,PC)
    UNKN(                             0157473); // D7, (*+10,PC,A3.W)
    UNKN(                             0157474); // D7, #$34
    TEST("ADDX.W", "D2, D7",          0157502); // ADDX.W
    TEST("ADDX.W", "-(A2), -(A7)",    0157512); // ADDX.L
    TEST("ADD.W", "D7, (A2)",         0157522);
    TEST("ADD.W", "D7, (A2)+",        0157532);
    TEST("ADD.W", "D7, -(A2)",        0157542);
    TEST("ADD.W", "D7, ($1234,A2)",   0157552, 0x1234);
    TEST("ADD.W", "D7, (18,A2,D3.L)", 0157562, 0x3812);
    TEST("ADD.W", "D7, ($FFFFFE).W",  0157570, 0xFFFE);
    TEST("ADD.W", "D7, ($123456).L",  0157571, 0x0012, 0x3456);
    UNKN(                             0157572); // D7, (*+$1234,PC)
    UNKN(                             0157573); // D7, (*+10,PC,A3.L)
    UNKN(                             0157574); // D7, #$1234
    TEST("ADDX.L", "D2, D7",          0157602); // ADDX.L
    TEST("ADDX.L", "-(A2), -(A7)",    0157612); // ADDX.L
    TEST("ADD.L", "D7, (A2)",         0157622);
    TEST("ADD.L", "D7, (A2)+",        0157632);
    TEST("ADD.L", "D7, -(A2)",        0157642);
    TEST("ADD.L", "D7, ($1234,A2)",   0157652, 0x1234);
    TEST("ADD.L", "D7, (18,A2,D3.L)", 0157662, 0x3812);
    TEST("ADD.L", "D7, ($FFFFFC).W",  0157670, 0xFFFC);
    TEST("ADD.L", "D7, ($123454).L",  0157671, 0x0012, 0x3454);
    UNKN(                             0157672); // D7, (*+$1234,PC)
    UNKN(                             0157673); // D7, (*+10,PC,D3.L)
    UNKN(                             0157674); // D7, #$12345678

    // ADD src,Dn: 015|Dn|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("ADD.B", "D2, D7",             0157002);
    UNKN(                               0157012); // A2, D7
    TEST("ADD.B", "(A2), D7",           0157022);
    TEST("ADD.B", "(A2)+, D7",          0157032);
    TEST("ADD.B", "-(A2), D7",          0157042);
    TEST("ADD.B", "($1234,A2), D7",     0157052, 0x1234);
    TEST("ADD.B", "(18,A2,D3.L), D7",   0157062, 0x3812);
    TEST("ADD.B", "($FFFFFF).W, D7",    0157070, 0xFFFF);
    TEST("ADD.B", "($123456).L, D7",    0157071, 0x0012, 0x3456);
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
    TEST("ADD.W", "($FFFFFE).W, D7",    0157170, 0xFFFE);
    TEST("ADD.W", "($123456).L, D7",    0157171, 0x0012, 0x3456);
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
    TEST("ADD.L", "($FFFFFC).W, D7",    0157270, 0xFFFC);
    TEST("ADD.L", "($123454).L, D7",    0157271, 0x0012, 0x3454);
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
    TEST("ADDA.W", "($FFFFFE).W, A6",    0156370, 0xFFFE);
    TEST("ADDA.W", "($123456).L, A6",    0156371, 0x0012, 0x3456);
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
    TEST("ADDA.L", "($FFFFFC).W, A6",    0156770, 0xFFFC);
    TEST("ADDA.L", "($123454).L, A6",    0156771, 0x0012, 0x3454);
    TEST("ADDA.L", "(*+$1234,PC), A6",   0156772, 0x1232);
    TEST("ADDA.L", "(*+18,PC,D3.W), A6", 0156773, 0x3010);
    TEST("ADDA.L", "#$00345678, A6",     0156774, 0x0034, 0x5678);

    // ADDI #xxxx,dst: 0003|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("ADDI.B", "#18, D2",           0003002, 0x0012);
    UNKN(                               0003012); // #18, A2
    TEST("ADDI.B", "#18, (A2)",         0003022, 0x0012);
    TEST("ADDI.B", "#18, (A2)+",        0003032, 0x0012);
    TEST("ADDI.B", "#18, -(A2)",        0003042, 0x0012);
    TEST("ADDI.B", "#18, ($1234,A2)",   0003052, 0x0012, 0x1234);
    TEST("ADDI.B", "#18, (18,A2,D3.W)", 0003062, 0x0012, 0x3012);
    TEST("ADDI.B", "#18, ($001234).W",  0003070, 0x0012, 0x1234);
    TEST("ADDI.B", "#18, ($123456).L",  0003071, 0x0012, 0x0012, 0x3456);
    UNKN(                               0003072); // #18, (*+$1234,PC)
    UNKN(                               0003073); // #18, (*+10,PC,D3.W)
    UNKN(                               0003074); // #18, #$34
    TEST("ADDI.W", "#$5678, D2",           0003102, 0x5678);
    UNKN(                                  0003112); // #$5678, A2
    TEST("ADDI.W", "#$5678, (A2)",         0003122, 0x5678);
    TEST("ADDI.W", "#$5678, (A2)+",        0003132, 0x5678);
    TEST("ADDI.W", "#$5678, -(A2)",        0003142, 0x5678);
    TEST("ADDI.W", "#$5678, ($1234,A2)",   0003152, 0x5678, 0x1234);
    TEST("ADDI.W", "#$5678, (18,A2,D3.W)", 0003162, 0x5678, 0x3012);
    TEST("ADDI.W", "#$5678, ($001234).W",  0003170, 0x5678, 0x1234);
    TEST("ADDI.W", "#$5678, ($123456).L",  0003171, 0x5678, 0x0012, 0x3456);
    UNKN(                                  0003172); // #$5678, (*+$1234,PC)
    UNKN(                                  0003173); // #$5678, (*+10,PC,D3.W)
    UNKN(                                  0003174); // #$5678, #$1234
    TEST("ADDI.L", "#$3456789A, D2",           0003202, 0x3456, 0x789A);
    UNKN(                                      0003212); // #$3456789A, A2
    TEST("ADDI.L", "#$3456789A, (A2)",         0003222, 0x3456, 0x789A);
    TEST("ADDI.L", "#$3456789A, (A2)+",        0003232, 0x3456, 0x789A);
    TEST("ADDI.L", "#$3456789A, -(A2)",        0003242, 0x3456, 0x789A);
    TEST("ADDI.L", "#$3456789A, ($1234,A2)",   0003252, 0x3456, 0x789A, 0x1234);
    TEST("ADDI.L", "#$3456789A, (18,A2,D3.W)", 0003262, 0x3456, 0x789A, 0x3012);
    TEST("ADDI.L", "#$3456789A, ($001234).W",  0003270, 0x3456, 0x789A, 0x1234);
    TEST("ADDI.L", "#$34567898, ($123454).L",  0003271, 0x3456, 0x7898, 0x0012, 0x3454);
    UNKN(                                      0003272); // #$3456789A, (*+$1234,PC)
    UNKN(                                      0003273); // #$3456789A, (*+10,PC,D3.W)
    UNKN(                                      0003274); // #$3456789A, #$12345678

    // ADDQ #nn,dst: 005|nn|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("ADDQ.B", "#8, D2",           0050002);
    UNKN(                              0050012); // #8, A2
    TEST("ADDQ.B", "#8, (A2)",         0050022);
    TEST("ADDQ.B", "#8, (A2)+",        0050032);
    TEST("ADDQ.B", "#8, -(A2)",        0050042);
    TEST("ADDQ.B", "#8, ($1234,A2)",   0050052, 0x1234);
    TEST("ADDQ.B", "#8, (18,A2,D3.W)", 0050062, 0x3012);
    TEST("ADDQ.B", "#8, ($001234).W",  0050070, 0x1234);
    TEST("ADDQ.B", "#8, ($123456).L",  0050071, 0x0012, 0x3456);
    UNKN(                              0050072); // #8, (*+$1234,PC)
    UNKN(                              0050073); // #8, (*+10,PC,D3.W)
    UNKN(                              0050074); // #8, #$34
    TEST("ADDQ.W", "#8, D2",           0050102);
    TEST("ADDQ.W", "#8, A2",           0050112);
    TEST("ADDQ.W", "#8, (A2)",         0050122);
    TEST("ADDQ.W", "#8, (A2)+",        0050132);
    TEST("ADDQ.W", "#8, -(A2)",        0050142);
    TEST("ADDQ.W", "#8, ($1234,A2)",   0050152, 0x1234);
    TEST("ADDQ.W", "#8, (18,A2,D3.W)", 0050162, 0x3012);
    TEST("ADDQ.W", "#8, ($001234).W",  0050170, 0x1234);
    TEST("ADDQ.W", "#8, ($123456).L",  0050171, 0x0012, 0x3456);
    UNKN(                              0050172); // #8, (*+$1234,PC)
    UNKN(                              0050173); // #8, (*+10,PC,D3.W)
    UNKN(                              0050174); // #8, #$1234
    TEST("ADDQ.L", "#8, D2",           0050202);
    TEST("ADDQ.L", "#8, A2",           0050212);
    TEST("ADDQ.L", "#8, (A2)",         0050222);
    TEST("ADDQ.L", "#8, (A2)+",        0050232);
    TEST("ADDQ.L", "#8, -(A2)",        0050242);
    TEST("ADDQ.L", "#8, ($1234,A2)",   0050252, 0x1234);
    TEST("ADDQ.L", "#8, (18,A2,D3.W)", 0050262, 0x3012);
    TEST("ADDQ.L", "#8, ($001234).W",  0050270, 0x1234);
    TEST("ADDQ.L", "#8, ($123454).L",  0050271, 0x0012, 0x3454);
    UNKN(                              0050272); // #8, (*+$1234,PC)
    UNKN(                              0050273); // #8, (*+10,PC,D3.W)
    UNKN(                              0050274); // #8, #$12345678

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
    UNKN(                         0041012); // A2
    TEST("CLR.B", "(A2)",         0041022);
    TEST("CLR.B", "(A2)+",        0041032);
    TEST("CLR.B", "-(A2)",        0041042);
    TEST("CLR.B", "($1234,A2)",   0041052, 0x1234);
    TEST("CLR.B", "(18,A2,D3.W)", 0041062, 0x3012);
    TEST("CLR.B", "($001234).W",  0041070, 0x1234);
    TEST("CLR.B", "($123456).L",  0041071, 0x0012, 0x3456);
    UNKN(                         0041072); // (*+$1234,PC)
    UNKN(                         0041073); // (*+10,PC,D3.W)
    UNKN(                         0041074); // #$34
    TEST("CLR.W", "D2",           0041102);
    UNKN(                         0041112); // A2
    TEST("CLR.W", "(A2)",         0041122);
    TEST("CLR.W", "(A2)+",        0041132);
    TEST("CLR.W", "-(A2)",        0041142);
    TEST("CLR.W", "($1234,A2)",   0041152, 0x1234);
    TEST("CLR.W", "(18,A2,D3.W)", 0041162, 0x3012);
    TEST("CLR.W", "($001234).W",  0041170, 0x1234);
    TEST("CLR.W", "($123456).L",  0041171, 0x0012, 0x3456);
    UNKN(                         0041172); // (*+$1234,PC)
    UNKN(                         0041173); // (*+10,PC,D3.W)
    UNKN(                         0041174); // #$1234
    TEST("CLR.L", "D2",           0041202);
    UNKN(                         0041212); // A2
    TEST("CLR.L", "(A2)",         0041222);
    TEST("CLR.L", "(A2)+",        0041232);
    TEST("CLR.L", "-(A2)",        0041242);
    TEST("CLR.L", "($1234,A2)",   0041252, 0x1234);
    TEST("CLR.L", "(18,A2,D3.W)", 0041262, 0x3012);
    TEST("CLR.L", "($001234).W",  0041270, 0x1234);
    TEST("CLR.L", "($123454).L",  0041271, 0x0012, 0x3454);
    UNKN(                         0041272); // (*+$1234,PC)
    UNKN(                         0041273);; // (*+10,PC,D3.W)
    UNKN(                         0041274); // #$12345678

    // CMP src,Dn: 013|Dn|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("CMP.B", "D2, D7",             0137002);
    UNKN(                               0137012); // A2, D7
    TEST("CMP.B", "(A2), D7",           0137022);
    TEST("CMP.B", "(A2)+, D7",          0137032);
    TEST("CMP.B", "-(A2), D7",          0137042);
    TEST("CMP.B", "($1234,A2), D7",     0137052, 0x1234);
    TEST("CMP.B", "(18,A2,D3.L), D7",   0137062, 0x3812);
    TEST("CMP.B", "($FFFFFF).W, D7",    0137070, 0xFFFF);
    TEST("CMP.B", "($123456).L, D7",    0137071, 0x0012, 0x3456);
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
    TEST("CMP.W", "($FFFFFE).W, D7",    0137170, 0xFFFE);
    TEST("CMP.W", "($123456).L, D7",    0137171, 0x0012, 0x3456);
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
    TEST("CMP.L", "($FFFFFC).W, D7",    0137270, 0xFFFC);
    TEST("CMP.L", "($123454).L, D7",    0137271, 0x0012, 0x3454);
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
    TEST("CMPA.W", "($FFFFFE).W, A6",    0136370, 0xFFFE);
    TEST("CMPA.W", "($123456).L, A6",    0136371, 0x0012, 0x3456);
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
    TEST("CMPA.L", "($FFFFFC).W, A6",    0136770, 0xFFFC);
    TEST("CMPA.L", "($123454).L, A6",    0136771, 0x0012, 0x3454);
    TEST("CMPA.L", "(*+$1234,PC), A6",   0136772, 0x1232);
    TEST("CMPA.L", "(*+18,PC,D3.W), A6", 0136773, 0x3010);
    TEST("CMPA.L", "#$00345678, A6",     0136774, 0x0034, 0x5678);

    // CMPI #nn,dst: 0006|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("CMPI.B", "#18, D2",           0006002, 0x0012);
    UNKN(                               0006012); // #18, A2
    TEST("CMPI.B", "#18, (A2)",         0006022, 0x0012);
    TEST("CMPI.B", "#18, (A2)+",        0006032, 0x0012);
    TEST("CMPI.B", "#18, -(A2)",        0006042, 0x0012);
    TEST("CMPI.B", "#18, ($1234,A2)",   0006052, 0x0012, 0x1234);
    TEST("CMPI.B", "#18, (18,A2,D3.W)", 0006062, 0x0012, 0x3012);
    TEST("CMPI.B", "#18, ($001234).W",  0006070, 0x0012, 0x1234);
    TEST("CMPI.B", "#18, ($123456).L",  0006071, 0x0012, 0x0012, 0x3456);
    UNKN(                               0006072); // #18, (*+$1234,PC)
    UNKN(                               0006073); // #18, (*+10,PC,D3.W)
    UNKN(                               0006074); // #18, #$34
    TEST("CMPI.W", "#$5678, D2",           0006102, 0x5678);
    UNKN(                                  0006112); // #$5678, A2
    TEST("CMPI.W", "#$5678, (A2)",         0006122, 0x5678);
    TEST("CMPI.W", "#$5678, (A2)+",        0006132, 0x5678);
    TEST("CMPI.W", "#$5678, -(A2)",        0006142, 0x5678);
    TEST("CMPI.W", "#$5678, ($1234,A2)",   0006152, 0x5678, 0x1234);
    TEST("CMPI.W", "#$5678, (18,A2,D3.W)", 0006162, 0x5678, 0x3012);
    TEST("CMPI.W", "#$5678, ($001234).W",  0006170, 0x5678, 0x1234);
    TEST("CMPI.W", "#$5678, ($123456).L",  0006171, 0x5678, 0x0012, 0x3456);
    UNKN(                                  0006172); // #$5678, (*+$1234,PC)
    UNKN(                                  0006173); // #$5678, (*+10,PC,D3.W)
    UNKN(                                  0006174); // #$5678, #$1234
    TEST("CMPI.L", "#$3456789A, D2",           0006202, 0x3456, 0x789A);
    UNKN(                                      0006212); // #$3456789A, A2
    TEST("CMPI.L", "#$3456789A, (A2)",         0006222, 0x3456, 0x789A);
    TEST("CMPI.L", "#$3456789A, (A2)+",        0006232, 0x3456, 0x789A);
    TEST("CMPI.L", "#$3456789A, -(A2)",        0006242, 0x3456, 0x789A);
    TEST("CMPI.L", "#$3456789A, ($1234,A2)",   0006252, 0x3456, 0x789A, 0x1234);
    TEST("CMPI.L", "#$3456789A, (18,A2,D3.W)", 0006262, 0x3456, 0x789A, 0x3012);
    TEST("CMPI.L", "#$3456789A, ($001234).W",  0006270, 0x3456, 0x789A, 0x1234);
    TEST("CMPI.L", "#$3456789A, ($123454).L",  0006271, 0x3456, 0x789A, 0x0012, 0x3454);
    UNKN(                                      0006272); // #$3456789A, (*+$1234,PC)
    UNKN(                                      0006273); // #$3456789A, (*+10,PC,D3.W)
    UNKN(                                      0006274); // #$3456789A, #$12345678

    // CMPM (Ay)+, (Ax)+: 013|Ax|Sz|1|Ay, Sz:B=4/W=5/L=6
    TEST("CMPM.B", "(A2)+, (A7)+", 0137412);
    TEST("CMPM.W", "(A2)+, (A7)+", 0137512);
    TEST("CMPM.L", "(A2)+, (A7)+", 0137612);

    // DIVS src,Dn: 010|Dn|7|M|Rn
    TEST("DIVS.W", "D2, D7",             0107702);
    UNKN(                                0107712); // A2, D7
    TEST("DIVS.W", "(A2), D7",           0107722);
    TEST("DIVS.W", "(A2)+, D7",          0107732);
    TEST("DIVS.W", "-(A2), D7",          0107742);
    TEST("DIVS.W", "($1234,A2), D7",     0107752, 0x1234);
    TEST("DIVS.W", "(18,A2,D3.L), D7",   0107762, 0x3812);
    TEST("DIVS.W", "($FFFFFE).W, D7",    0107770, 0xFFFE);
    TEST("DIVS.W", "($123456).L, D7",    0107771, 0x0012, 0x3456);
    TEST("DIVS.W", "(*+$1234,PC), D7",   0107772, 0x1232);
    TEST("DIVS.W", "(*+18,PC,D3.W), D7", 0107773, 0x3010);
    TEST("DIVS.W", "#$5678, D7",         0107774, 0x5678);

    // DIVU src,Dn: 010|Dn|3|M|Rn
    TEST("DIVU.W", "D2, D7",             0107302);
    UNKN(                                0107312); // A2, D7
    TEST("DIVU.W", "(A2), D7",           0107322);
    TEST("DIVU.W", "(A2)+, D7",          0107332);
    TEST("DIVU.W", "-(A2), D7",          0107342);
    TEST("DIVU.W", "($1234,A2), D7",     0107352, 0x1234);
    TEST("DIVU.W", "(18,A2,D3.L), D7",   0107362, 0x3812);
    TEST("DIVU.W", "($FFFFFE).W, D7",    0107370, 0xFFFE);
    TEST("DIVU.W", "($123456).L, D7",    0107371, 0x0012, 0x3456);
    TEST("DIVU.W", "(*+$1234,PC), D7",   0107372, 0x1232);
    TEST("DIVU.W", "(*+18,PC,D3.W), D7", 0107373, 0x3010);
    TEST("DIVU.W", "#$5678, D7",         0107374, 0x5678);

    // EXT Dn: 0044|Sz|0|Dn, Sz:W=2/L=3
    TEST("EXT.W", "D2", 0044202);
    TEST("EXT.L", "D2", 0044302);

    // MULS src,Dn: 014|Dn|7|M|Rn
    TEST("MULS.W", "D2, D7",             0147702);
    UNKN(                                0147712); // A2, D7
    TEST("MULS.W", "(A2), D7",           0147722);
    TEST("MULS.W", "(A2)+, D7",          0147732);
    TEST("MULS.W", "-(A2), D7",          0147742);
    TEST("MULS.W", "($1234,A2), D7",     0147752, 0x1234);
    TEST("MULS.W", "(18,A2,D3.L), D7",   0147762, 0x3812);
    TEST("MULS.W", "($FFFFFE).W, D7",    0147770, 0xFFFE);
    TEST("MULS.W", "($123456).L, D7",    0147771, 0x0012, 0x3456);
    TEST("MULS.W", "(*+$1234,PC), D7",   0147772, 0x1232);
    TEST("MULS.W", "(*+18,PC,D3.W), D7", 0147773, 0x3010);
    TEST("MULS.W", "#$5678, D7",         0147774, 0x5678);

    // MULU src,Dn: 014|Dn|3|M|Rn
    TEST("MULU.W", "D2, D7",             0147302);
    UNKN(                                0147312); // A2, D7
    TEST("MULU.W", "(A2), D7",           0147322);
    TEST("MULU.W", "(A2)+, D7",          0147332);
    TEST("MULU.W", "-(A2), D7",          0147342);
    TEST("MULU.W", "($1234,A2), D7",     0147352, 0x1234);
    TEST("MULU.W", "(18,A2,D3.L), D7",   0147362, 0x3812);
    TEST("MULU.W", "($FFFFFE).W, D7",    0147370, 0xFFFE);
    TEST("MULU.W", "($123456).L, D7",    0147371, 0x0012, 0x3456);
    TEST("MULU.W", "(*+$1234,PC), D7",   0147372, 0x1232);
    TEST("MULU.W", "(*+18,PC,D3.W), D7", 0147373, 0x3010);
    TEST("MULU.W", "#$5678, D7",         0147374, 0x5678);

    // NEG dst: 0042|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("NEG.B", "D2",           0042002);
    UNKN(                         0042012); // A2
    TEST("NEG.B", "(A2)",         0042022);
    TEST("NEG.B", "(A2)+",        0042032);
    TEST("NEG.B", "-(A2)",        0042042);
    TEST("NEG.B", "($1234,A2)",   0042052, 0x1234);
    TEST("NEG.B", "(18,A2,D3.W)", 0042062, 0x3012);
    TEST("NEG.B", "($001234).W",  0042070, 0x1234);
    TEST("NEG.B", "($123456).L",  0042071, 0x0012, 0x3456);
    UNKN(                         0042072); // (*+$1234,PC)
    UNKN(                         0042073); // (*+10,PC,D3.W)
    UNKN(                         0042074); // #$34
    TEST("NEG.W", "D2",           0042102);
    UNKN(                         0042112); // A2
    TEST("NEG.W", "(A2)",         0042122);
    TEST("NEG.W", "(A2)+",        0042132);
    TEST("NEG.W", "-(A2)",        0042142);
    TEST("NEG.W", "($1234,A2)",   0042152, 0x1234);
    TEST("NEG.W", "(18,A2,D3.W)", 0042162, 0x3012);
    TEST("NEG.W", "($001234).W",  0042170, 0x1234);
    TEST("NEG.W", "($123456).L",  0042171, 0x0012, 0x3456);
    UNKN(                         0042172); // (*+$1234,PC)
    UNKN(                         0042173); // (*+10,PC,D3.W)
    UNKN(                         0042174); // #$1234
    TEST("NEG.L", "D2",           0042202);
    UNKN(                         0042212); // A2
    TEST("NEG.L", "(A2)",         0042222);
    TEST("NEG.L", "(A2)+",        0042232);
    TEST("NEG.L", "-(A2)",        0042242);
    TEST("NEG.L", "($1234,A2)",   0042252, 0x1234);
    TEST("NEG.L", "(18,A2,D3.W)", 0042262, 0x3012);
    TEST("NEG.L", "($001234).W",  0042270, 0x1234);
    TEST("NEG.L", "($123454).L",  0042271, 0x0012, 0x3454);
    UNKN(                         0042272); // (*+$1234,PC)
    UNKN(                         0042273); // (*+10,PC,D3.W)
    UNKN(                         0042274); // #$12345678

    // NEGX dst: 0040|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("NEGX.B", "D2",          0040002);
    UNKN(                         0040012); // A2
    TEST("NEGX.B", "(A2)",        0040022);
    TEST("NEGX.B", "(A2)+",       0040032);
    TEST("NEGX.B", "-(A2)",       0040042);
    TEST("NEGX.B", "($1234,A2)",  0040052, 0x1234);
    TEST("NEGX.B", "(18,A2,D3.W)",0040062, 0x3012);
    TEST("NEGX.B", "($001234).W", 0040070, 0x1234);
    TEST("NEGX.B", "($123456).L", 0040071, 0x0012, 0x3456);
    UNKN(                         0040072); // (*+$1234,PC)
    UNKN(                         0040073); // (*+10,PC,D3.W)
    UNKN(                         0040074); // #$34
    TEST("NEGX.W", "D2",          0040102);
    UNKN(                         0040112); // A2
    TEST("NEGX.W", "(A2)",        0040122);
    TEST("NEGX.W", "(A2)+",       0040132);
    TEST("NEGX.W", "-(A2)",       0040142);
    TEST("NEGX.W", "($1234,A2)",  0040152, 0x1234);
    TEST("NEGX.W", "(18,A2,D3.W)",0040162, 0x3012);
    TEST("NEGX.W", "($001234).W", 0040170, 0x1234);
    TEST("NEGX.W", "($123456).L", 0040171, 0x0012, 0x3456);
    UNKN(                         0040172); // (*+$1234,PC)
    UNKN(                         0040173); // (*+10,PC,D3.W)
    UNKN(                         0040174); // #$1234
    TEST("NEGX.L", "D2",          0040202);
    UNKN(                         0040212); // A2
    TEST("NEGX.L", "(A2)",        0040222);
    TEST("NEGX.L", "(A2)+",       0040232);
    TEST("NEGX.L", "-(A2)",       0040242);
    TEST("NEGX.L", "($1234,A2)",  0040252, 0x1234);
    TEST("NEGX.L", "(18,A2,D3.W)",0040262, 0x3012);
    TEST("NEGX.L", "($001234).W", 0040270, 0x1234);
    TEST("NEGX.L", "($123454).L", 0040271, 0x0012, 0x3454);
    UNKN(                         0040272); // (*+$1234,PC)
    UNKN(                         0040273); // (*+10,PC,D3.W)
    UNKN(                         0040274); // #$12345678

    // SUB src,Dn: 011|Dn|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("SUB.B", "D2, D7",             0117002);
    UNKN(                               0117012); // A2, D7
    TEST("SUB.B", "(A2), D7",           0117022);
    TEST("SUB.B", "(A2)+, D7",          0117032);
    TEST("SUB.B", "-(A2), D7",          0117042);
    TEST("SUB.B", "($1234,A2), D7",     0117052, 0x1234);
    TEST("SUB.B", "(18,A2,D3.L), D7",   0117062, 0x3812);
    TEST("SUB.B", "($FFFFFF).W, D7",    0117070, 0xFFFF);
    TEST("SUB.B", "($123456).L, D7",    0117071, 0x0012, 0x3456);
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
    TEST("SUB.W", "($FFFFFE).W, D7",    0117170, 0xFFFE);
    TEST("SUB.W", "($123456).L, D7",    0117171, 0x0012, 0x3456);
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
    TEST("SUB.L", "($FFFFFC).W, D7",    0117270, 0xFFFC);
    TEST("SUB.L", "($123454).L, D7",    0117271, 0x0012, 0x3454);
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
    TEST("SUB.B", "D7, ($FFFFFF).W",  0117470, 0xFFFF);
    TEST("SUB.B", "D7, ($123456).L",  0117471, 0x0012, 0x3456);
    UNKN(                             0117472); // D7, (*+$1234,PC)
    UNKN(                             0117473); // D7, (*+10,PC,D3.W)
    UNKN(                             0117474); // D7, #$34
    TEST("SUBX.W", "D2, D7",          0117502); // SUBX.W
    TEST("SUBX.W", "-(A2), -(A7)",    0117512); // SUBX.L
    TEST("SUB.W", "D7, (A2)",         0117522);
    TEST("SUB.W", "D7, (A2)+",        0117532);
    TEST("SUB.W", "D7, -(A2)",        0117542);
    TEST("SUB.W", "D7, ($1234,A2)",   0117552, 0x1234);
    TEST("SUB.W", "D7, (18,A2,D3.L)", 0117562, 0x3812);
    TEST("SUB.W", "D7, ($FFFFFE).W",  0117570, 0xFFFE);
    TEST("SUB.W", "D7, ($123456).L",  0117571, 0x0012, 0x3456);
    UNKN(                             0117572); // D7, (*+$1234,PC)
    UNKN(                             0117573); // D7, (*+10,PC,D3.W)
    UNKN(                             0117574); // D7, #$1234
    TEST("SUBX.L", "D2, D7",          0117602); // SUBX.L
    TEST("SUBX.L", "-(A2), -(A7)",    0117612); // SUBX.L
    TEST("SUB.L", "D7, (A2)",         0117622);
    TEST("SUB.L", "D7, (A2)+",        0117632);
    TEST("SUB.L", "D7, -(A2)",        0117642);
    TEST("SUB.L", "D7, ($1234,A2)",   0117652, 0x1234);
    TEST("SUB.L", "D7, (18,A2,D3.L)", 0117662, 0x3812);
    TEST("SUB.L", "D7, ($FFFFFC).W",  0117670, 0xFFFC);
    TEST("SUB.L", "D7, ($123454).L",  0117671, 0x0012, 0x3454);
    UNKN(                             0117672); // D7, (*+$1234,PC)
    UNKN(                             0117673); // D7, (*+10,PC,D3.W)
    UNKN(                             0117674); // D7, #$12345678

    // SUBA src,An: 011|An|Sz|M|Rn, Sz:W=3/L=7
    TEST("SUBA.W", "D2, A6",             0116302);
    TEST("SUBA.W", "A2, A6",             0116312);
    TEST("SUBA.W", "(A2), A6",           0116322);
    TEST("SUBA.W", "(A2)+, A6",          0116332);
    TEST("SUBA.W", "-(A2), A6",          0116342);
    TEST("SUBA.W", "($1234,A2), A6",     0116352, 0x1234);
    TEST("SUBA.W", "(18,A2,D3.L), A6",   0116362, 0x3812);
    TEST("SUBA.W", "($FFFFFE).W, A6",    0116370, 0xFFFE);
    TEST("SUBA.W", "($123456).L, A6",    0116371, 0x0012, 0x3456);
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
    TEST("SUBA.L", "($FFFFFC).W, A6",    0116770, 0xFFFC);
    TEST("SUBA.L", "($123454).L, A6",    0116771, 0x0012, 0x3454);
    TEST("SUBA.L", "(*+$1234,PC), A6",   0116772, 0x1232);
    TEST("SUBA.L", "(*+18,PC,D3.W), A6", 0116773, 0x3010);
    TEST("SUBA.L", "#$00345678, A6",     0116774, 0x0034, 0x5678);

    // SUBI #nn,dst: 0002|Sz|M|Rn, SZ:B=0/W=1/L=2
    TEST("SUBI.B", "#18, D2",           0002002, 0x0012);
    UNKN(                               0002012); // #18, A2
    TEST("SUBI.B", "#18, (A2)",         0002022, 0x0012);
    TEST("SUBI.B", "#18, (A2)+",        0002032, 0x0012);
    TEST("SUBI.B", "#18, -(A2)",        0002042, 0x0012);
    TEST("SUBI.B", "#18, ($1234,A2)",   0002052, 0x0012, 0x1234);
    TEST("SUBI.B", "#18, (18,A2,D3.W)", 0002062, 0x0012, 0x3012);
    TEST("SUBI.B", "#18, ($001234).W",  0002070, 0x0012, 0x1234);
    TEST("SUBI.B", "#18, ($123456).L",  0002071, 0x0012, 0x0012, 0x3456);
    UNKN(                               0002072); // #18, (*+$1234,PC)
    UNKN(                               0002073); // #18, (*+10,PC,D3.W)
    UNKN(                               0002074); // #18, #$34
    TEST("SUBI.W", "#$5678, D2",           0002102, 0x5678);
    UNKN(                                  0002112); // #$5678, A2
    TEST("SUBI.W", "#$5678, (A2)",         0002122, 0x5678);
    TEST("SUBI.W", "#$5678, (A2)+",        0002132, 0x5678);
    TEST("SUBI.W", "#$5678, -(A2)",        0002142, 0x5678);
    TEST("SUBI.W", "#$5678, ($1234,A2)",   0002152, 0x5678, 0x1234);
    TEST("SUBI.W", "#$5678, (18,A2,D3.W)", 0002162, 0x5678, 0x3012);
    TEST("SUBI.W", "#$5678, ($001234).W",  0002170, 0x5678, 0x1234);
    TEST("SUBI.W", "#$5678, ($123456).L",  0002171, 0x5678, 0x0012, 0x3456);
    UNKN(                                  0002172); // #$5678, (*+$1234,PC)
    UNKN(                                  0002173); // #$5678, (*+10,PC,D3.W)
    UNKN(                                  0002174); // #$5678, #$1234
    TEST("SUBI.L", "#$3456789A, D2",           0002202, 0x3456, 0x789A);
    UNKN(                                      0002212); // #$3456789A, A2
    TEST("SUBI.L", "#$3456789A, (A2)",         0002222, 0x3456, 0x789A);
    TEST("SUBI.L", "#$3456789A, (A2)+",        0002232, 0x3456, 0x789A);
    TEST("SUBI.L", "#$3456789A, -(A2)",        0002242, 0x3456, 0x789A);
    TEST("SUBI.L", "#$3456789A, ($1234,A2)",   0002252, 0x3456, 0x789A, 0x1234);
    TEST("SUBI.L", "#$3456789A, (18,A2,D3.W)", 0002262, 0x3456, 0x789A, 0x3012);
    TEST("SUBI.L", "#$3456789A, ($001234).W",  0002270, 0x3456, 0x789A, 0x1234);
    TEST("SUBI.L", "#$3456789A, ($123454).L",  0002271, 0x3456, 0x789A, 0x0012, 0x3454);
    UNKN(                                      0002272); // #$3456789A, (*+$1234,PC)
    UNKN(                                      0002273); // #$3456789A, (*+10,PC,D3.W)
    UNKN(                                      0002274); // #$3456789A, #$12345678

    // SUBQ #nn,dst: 005|nn|Sz|M\En, Sz:B=4/W=5/L=6
    TEST("SUBQ.B", "#8, D2",           0050402);
    UNKN(                              0050412); // #8, A2
    TEST("SUBQ.B", "#8, (A2)",         0050422);
    TEST("SUBQ.B", "#8, (A2)+",        0050432);
    TEST("SUBQ.B", "#8, -(A2)",        0050442);
    TEST("SUBQ.B", "#8, ($1234,A2)",   0050452, 0x1234);
    TEST("SUBQ.B", "#8, (18,A2,D3.W)", 0050462, 0x3012);
    TEST("SUBQ.B", "#8, ($001234).W",  0050470, 0x1234);
    TEST("SUBQ.B", "#8, ($123456).L",  0050471, 0x0012, 0x3456);
    UNKN(                              0050472); // #8, (*+$1234,PC)
    UNKN(                              0050473); // #8, (*+10,PC,D3.W)
    UNKN(                              0050474); // #8, #$34
    TEST("SUBQ.W", "#8, D2",           0050502);
    TEST("SUBQ.W", "#8, A2",           0050512);
    TEST("SUBQ.W", "#8, (A2)",         0050522);
    TEST("SUBQ.W", "#8, (A2)+",        0050532);
    TEST("SUBQ.W", "#8, -(A2)",        0050542);
    TEST("SUBQ.W", "#8, ($1234,A2)",   0050552, 0x1234);
    TEST("SUBQ.W", "#8, (18,A2,D3.W)", 0050562, 0x3012);
    TEST("SUBQ.W", "#8, ($001234).W",  0050570, 0x1234);
    TEST("SUBQ.W", "#8, ($123456).L",  0050571, 0x0012, 0x3456);
    UNKN(                              0050572); // #8, (*+$1234,PC)
    UNKN(                              0050573); // #8, (*+10,PC,D3.W)
    UNKN(                              0050574); // #8, #$1234
    TEST("SUBQ.L", "#8, D2",           0050602);
    TEST("SUBQ.L", "#8, A2",           0050612);
    TEST("SUBQ.L", "#8, (A2)",         0050622);
    TEST("SUBQ.L", "#8, (A2)+",        0050632);
    TEST("SUBQ.L", "#8, -(A2)",        0050642);
    TEST("SUBQ.L", "#8, ($1234,A2)",   0050652, 0x1234);
    TEST("SUBQ.L", "#8, (18,A2,D3.W)", 0050662, 0x3012);
    TEST("SUBQ.L", "#8, ($001234).W",  0050670, 0x1234);
    TEST("SUBQ.L", "#8, ($123454).L",  0050671, 0x0012, 0x3454);
    UNKN(                              0050672); // #8, (*+$1234,PC)
    UNKN(                              0050673); // #8, (*+10,PC,D3.W)
    UNKN(                              0050674); // #8, #$12345678

    // SUBX Dx,Dy: 011|Dy|Sz|0|Dx, Sz:B=4/W=5/L=6
    TEST("SUBX.B", "D2, D3", 0113402);
    TEST("SUBX.W", "D2, D3", 0113502);
    TEST("SUBX.L", "D2, D3", 0113602);

    // SUBX -(Ax), -(Ay): 011|Ay|Sz|1|Ax, Sz:B=4/W=5/L=6
    TEST("SUBX.B", "-(A2), -(A3)", 0113412);
    TEST("SUBX.W", "-(A2), -(A3)", 0113512);
    TEST("SUBX.L", "-(A2), -(A3)", 0113612);
}

static void test_logical() {
    // AND src,Dn: 014|Dn|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("AND.B", "D2, D7",             0147002);
    UNKN(                               0147012); // A2, D7
    TEST("AND.B", "(A2), D7",           0147022);
    TEST("AND.B", "(A2)+, D7",          0147032);
    TEST("AND.B", "-(A2), D7",          0147042);
    TEST("AND.B", "($1234,A2), D7",     0147052, 0x1234);
    TEST("AND.B", "(18,A2,D3.L), D7",   0147062, 0x3812);
    TEST("AND.B", "($FFFFFF).W, D7",    0147070, 0xFFFF);
    TEST("AND.B", "($123456).L, D7",    0147071, 0x0012, 0x3456);
    TEST("AND.B", "(*+$1234,PC), D7",   0147072, 0x1232);
    TEST("AND.B", "(*+18,PC,D3.W), D7", 0147073, 0x3010);
    TEST("AND.B", "#$34, D7",           0147074, 0x0034);
    TEST("AND.W", "D2, D7",             0147102);
    UNKN(                               0147112); // A2, D7
    TEST("AND.W", "(A2), D7",           0147122);
    TEST("AND.W", "(A2)+, D7",          0147132);
    TEST("AND.W", "-(A2), D7",          0147142);
    TEST("AND.W", "($1234,A2), D7",     0147152, 0x1234);
    TEST("AND.W", "(18,A2,D3.L), D7",   0147162, 0x3812);
    TEST("AND.W", "($FFFFFE).W, D7",    0147170, 0xFFFE);
    TEST("AND.W", "($123456).L, D7",    0147171, 0x0012, 0x3456);
    TEST("AND.W", "(*+$1234,PC), D7",   0147172, 0x1232);
    TEST("AND.W", "(*+18,PC,D3.W), D7", 0147173, 0x3010);
    TEST("AND.W", "#$0034, D7",         0147174, 0x0034);
    TEST("AND.L", "D2, D7",             0147202);
    UNKN(                               0147212); // A2, D7
    TEST("AND.L", "(A2), D7",           0147222);
    TEST("AND.L", "(A2)+, D7",          0147232);
    TEST("AND.L", "-(A2), D7",          0147242);
    TEST("AND.L", "($1234,A2), D7",     0147252, 0x1234);
    TEST("AND.L", "(18,A2,D3.L), D7",   0147262, 0x3812);
    TEST("AND.L", "($FFFFFC).W, D7",    0147270, 0xFFFC);
    TEST("AND.L", "($123454).L, D7",    0147271, 0x0012, 0x3454);
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
    TEST("AND.B", "D7, ($FFFFFF).W",  0147470, 0xFFFF);
    TEST("AND.B", "D7, ($123456).L",  0147471, 0x0012, 0x3456);
    UNKN(                             0147472); // D7, (*+$1234,PC)
    UNKN(                             0147473); // D7, (*+10,PC,D3.W)
    UNKN(                             0147474); // D7, #$34
    TEST("EXG",   "D7, D2",           0147502); // EXG
    TEST("EXG",   "A7, A2",           0147512); // EXG
    TEST("AND.W", "D7, (A2)",         0147522);
    TEST("AND.W", "D7, (A2)+",        0147532);
    TEST("AND.W", "D7, -(A2)",        0147542);
    TEST("AND.W", "D7, ($1234,A2)",   0147552, 0x1234);
    TEST("AND.W", "D7, (18,A2,D3.L)", 0147562, 0x3812);
    TEST("AND.W", "D7, ($FFFFFE).W",  0147570, 0xFFFE);
    TEST("AND.W", "D7, ($123456).L",  0147571, 0x0012, 0x3456);
    UNKN(                             0147572); // D7, (*+$1234,PC)
    UNKN(                             0147573); // D7, (*+10,PC,D3.W)
    UNKN(                             0147574); // D7, #$1234
    UNKN(                             0147602); // D7, D2
    TEST("EXG",   "D7, A2",           0147612); // EXG
    TEST("AND.L", "D7, (A2)",         0147622);
    TEST("AND.L", "D7, (A2)+",        0147632);
    TEST("AND.L", "D7, -(A2)",        0147642);
    TEST("AND.L", "D7, ($1234,A2)",   0147652, 0x1234);
    TEST("AND.L", "D7, (18,A2,D3.L)", 0147662, 0x3812);
    TEST("AND.L", "D7, ($FFFFFC).W",  0147670, 0xFFFC);
    TEST("AND.L", "D7, ($123454).L",  0147671, 0x0012, 0x3454);
    UNKN(                             0147672); // D7, (*+$1234,PC)
    UNKN(                             0147673); // D7, (*+10,PC,D3.W)
    UNKN(                             0147674); // D7, #$12345678

    // ANDI #nn,dst: 0001|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("ANDI.B", "#18, D2",           0001002, 0x0012);
    UNKN(                               0001012); // #18, A2
    TEST("ANDI.B", "#18, (A2)",         0001022, 0x0012);
    TEST("ANDI.B", "#18, (A2)+",        0001032, 0x0012);
    TEST("ANDI.B", "#18, -(A2)",        0001042, 0x0012);
    TEST("ANDI.B", "#18, ($1234,A2)",   0001052, 0x0012, 0x1234);
    TEST("ANDI.B", "#18, (18,A2,D3.W)", 0001062, 0x0012, 0x3012);
    TEST("ANDI.B", "#18, ($001234).W",  0001070, 0x0012, 0x1234);
    TEST("ANDI.B", "#18, ($123456).L",  0001071, 0x0012, 0x0012, 0x3456);
    UNKN(                               0001072); // #18, (*+$1234,PC)
    UNKN(                               0001073); // #18, (*+10,PC,D3.W)
    TEST("ANDI",   "#18, CCR",          0001074, 0x0012); // ANDI CCR
    TEST("ANDI.W", "#$5678, D2",           0001102, 0x5678);
    UNKN(                                  0001112); // #$5678, A2
    TEST("ANDI.W", "#$5678, (A2)",         0001122, 0x5678);
    TEST("ANDI.W", "#$5678, (A2)+",        0001132, 0x5678);
    TEST("ANDI.W", "#$5678, -(A2)",        0001142, 0x5678);
    TEST("ANDI.W", "#$5678, ($1234,A2)",   0001152, 0x5678, 0x1234);
    TEST("ANDI.W", "#$5678, (18,A2,D3.W)", 0001162, 0x5678, 0x3012);
    TEST("ANDI.W", "#$5678, ($001234).W",  0001170, 0x5678, 0x1234);
    TEST("ANDI.W", "#$5678, ($123456).L",  0001171, 0x5678, 0x0012, 0x3456);
    UNKN(                                  0001172); // #$5678, (*+$1234,PC)
    UNKN(                                  0001173); // #$5678, (*+10,PC,D3.W)
    TEST("ANDI",   "#$5678, SR",           0001174, 0x5678); // ANDI SR
    TEST("ANDI.L", "#$3456789A, D2",           0001202, 0x3456, 0x789A);
    UNKN(                                      0001212); // #$3456789A, A2
    TEST("ANDI.L", "#$3456789A, (A2)",         0001222, 0x3456, 0x789A);
    TEST("ANDI.L", "#$3456789A, (A2)+",        0001232, 0x3456, 0x789A);
    TEST("ANDI.L", "#$3456789A, -(A2)",        0001242, 0x3456, 0x789A);
    TEST("ANDI.L", "#$3456789A, ($1234,A2)",   0001252, 0x3456, 0x789A, 0x1234);
    TEST("ANDI.L", "#$3456789A, (18,A2,D3.W)", 0001262, 0x3456, 0x789A, 0x3012);
    TEST("ANDI.L", "#$3456789A, ($001234).W",  0001270, 0x3456, 0x789A, 0x1234);
    TEST("ANDI.L", "#$34567898, ($123454).L",  0001271, 0x3456, 0x7898, 0x0012, 0x3454);
    UNKN(                                      0001272); // #$3456789A, (*+$1234,PC)
    UNKN(                                      0001273); // #$3456789A, (*+10,PC,D3.W)
    UNKN(                                      0001274); // #$3456789A, #$12345678

    // EOR Dn,dst: 013|Dn|Sz|M|Rn, Sz:B=4/W=5/L=6
    TEST("EOR.B", "D7, D2",           0137402);
    TEST("CMPM.B", "(A2)+, (A7)+",    0137412); // CMPM.B
    TEST("EOR.B", "D7, (A2)",         0137422);
    TEST("EOR.B", "D7, (A2)+",        0137432);
    TEST("EOR.B", "D7, -(A2)",        0137442);
    TEST("EOR.B", "D7, ($1234,A2)",   0137452, 0x1234);
    TEST("EOR.B", "D7, (18,A2,D3.L)", 0137462, 0x3812);
    TEST("EOR.B", "D7, ($FFFFFF).W",  0137470, 0xFFFF);
    TEST("EOR.B", "D7, ($123456).L",  0137471, 0x0012, 0x3456);
    UNKN(                             0137472); // D7, (*+$1234,PC)
    UNKN(                             0137473); // D7, (*+10,PC,D3.W)
    UNKN(                             0137474); // D7, #$34
    TEST("EOR.W",  "D7, D2",          0137502);
    TEST("CMPM.W", "(A2)+, (A7)+",    0137512); // CMPM.W
    TEST("EOR.W", "D7, (A2)",         0137522);
    TEST("EOR.W", "D7, (A2)+",        0137532);
    TEST("EOR.W", "D7, -(A2)",        0137542);
    TEST("EOR.W", "D7, ($1234,A2)",   0137552, 0x1234);
    TEST("EOR.W", "D7, (18,A2,D3.L)", 0137562, 0x3812);
    TEST("EOR.W", "D7, ($FFFFFE).W",  0137570, 0xFFFE);
    TEST("EOR.W", "D7, ($123456).L",  0137571, 0x0012, 0x3456);
    UNKN(                             0137572); // D7, (*+$1234,PC)
    UNKN(                             0137573); // D7, (*+10,PC,D3.W)
    UNKN(                             0137574); // D7, #$1234
    TEST("EOR.L",  "D7, D2",          0137602);
    TEST("CMPM.L", "(A2)+, (A7)+",    0137612); // CMPM.L
    TEST("EOR.L", "D7, (A2)",         0137622);
    TEST("EOR.L", "D7, (A2)+",        0137632);
    TEST("EOR.L", "D7, -(A2)",        0137642);
    TEST("EOR.L", "D7, ($1234,A2)",   0137652, 0x1234);
    TEST("EOR.L", "D7, (18,A2,D3.L)", 0137662, 0x3812);
    TEST("EOR.L", "D7, ($FFFFFC).W",  0137670, 0xFFFC);
    TEST("EOR.L", "D7, ($123454).L",  0137671, 0x0012, 0x3454);
    UNKN(                             0137672); // D7, (*+$1234,PC)
    UNKN(                             0137673); // D7, (*+10,PC,D3.W)
    UNKN(                             0137674); // D7, #$12345678

    // EORI #nn,dst: 0005|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("EORI.B", "#18, D2",           0005002, 0x0012);
    UNKN(                               0005012); // #18, A2
    TEST("EORI.B", "#18, (A2)",         0005022, 0x0012);
    TEST("EORI.B", "#18, (A2)+",        0005032, 0x0012);
    TEST("EORI.B", "#18, -(A2)",        0005042, 0x0012);
    TEST("EORI.B", "#18, ($1234,A2)",   0005052, 0x0012, 0x1234);
    TEST("EORI.B", "#18, (18,A2,D3.W)", 0005062, 0x0012, 0x3012);
    TEST("EORI.B", "#18, ($001234).W",  0005070, 0x0012, 0x1234);
    TEST("EORI.B", "#18, ($123456).L",  0005071, 0x0012, 0x0012, 0x3456);
    UNKN(                               0005072); // #18, (*+$1234,PC)
    UNKN(                               0005073); // #18, (*+10,PC,D3.W)
    TEST("EORI",   "#18, CCR",          0005074, 0x0012); // EORI CCR
    TEST("EORI.W", "#$5678, D2",           0005102, 0x5678);
    UNKN(                                  0005112); // #$5678, A2
    TEST("EORI.W", "#$5678, (A2)",         0005122, 0x5678);
    TEST("EORI.W", "#$5678, (A2)+",        0005132, 0x5678);
    TEST("EORI.W", "#$5678, -(A2)",        0005142, 0x5678);
    TEST("EORI.W", "#$5678, ($1234,A2)",   0005152, 0x5678, 0x1234);
    TEST("EORI.W", "#$5678, (18,A2,D3.W)", 0005162, 0x5678, 0x3012);
    TEST("EORI.W", "#$5678, ($001234).W",  0005170, 0x5678, 0x1234);
    TEST("EORI.W", "#$5678, ($123456).L",  0005171, 0x5678, 0x0012, 0x3456);
    UNKN(                                  0005172); // #$5678, (*+$1234,PC)
    UNKN(                                  0005173); // #$5678, (*+10,PC,D3.W)
    TEST("EORI",   "#$5678, SR",           0005174, 0x5678); // EORI SR
    TEST("EORI.L", "#$3456789A, D2",           0005202, 0x3456, 0x789A);
    UNKN(                                      0005212); // #$3456789A, A2
    TEST("EORI.L", "#$3456789A, (A2)",         0005222, 0x3456, 0x789A);
    TEST("EORI.L", "#$3456789A, (A2)+",        0005232, 0x3456, 0x789A);
    TEST("EORI.L", "#$3456789A, -(A2)",        0005242, 0x3456, 0x789A);
    TEST("EORI.L", "#$3456789A, ($1234,A2)",   0005252, 0x3456, 0x789A, 0x1234);
    TEST("EORI.L", "#$3456789A, (18,A2,D3.W)", 0005262, 0x3456, 0x789A, 0x3012);
    TEST("EORI.L", "#$3456789A, ($001234).W",  0005270, 0x3456, 0x789A, 0x1234);
    TEST("EORI.L", "#$34567898, ($123454).L",  0005271, 0x3456, 0x7898, 0x0012, 0x3454);
    UNKN(                                      0005272); // #$3456789A, (*+$1234,PC)
    UNKN(                                      0005273); // #$3456789A, (*+10,PC,D3.W)
    UNKN(                                      0005274); // #$3456789A, #$12345678

    // NOT dst: 0043|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("NOT.B", "D2",               0043002);
    UNKN(                             0043012); // A2
    TEST("NOT.B", "(A2)",             0043022);
    TEST("NOT.B", "(A2)+",            0043032);
    TEST("NOT.B", "-(A2)",            0043042);
    TEST("NOT.B", "($1234,A2)",       0043052, 0x1234);
    TEST("NOT.B", "(18,A2,D3.W)",     0043062, 0x3012);
    TEST("NOT.B", "($001234).W",      0043070, 0x1234);
    TEST("NOT.B", "($123456).L",      0043071, 0x0012, 0x3456);
    UNKN(                             0043072); // (*+$1234,PC)
    UNKN(                             0043073); // (*+10,PC,D3.W)
    UNKN(                             0043074); // #$34
    TEST("NOT.W", "D2",               0043102);
    UNKN(                             0043112); // A2
    TEST("NOT.W", "(A2)",             0043122);
    TEST("NOT.W", "(A2)+",            0043132);
    TEST("NOT.W", "-(A2)",            0043142);
    TEST("NOT.W", "($1234,A2)",       0043152, 0x1234);
    TEST("NOT.W", "(18,A2,D3.W)",     0043162, 0x3012);
    TEST("NOT.W", "($001234).W",      0043170, 0x1234);
    TEST("NOT.W", "($123456).L",      0043171, 0x0012, 0x3456);
    UNKN(                             0043172); // (*+$1234,PC)
    UNKN(                             0043173); // (*+10,PC,D3.W)
    UNKN(                             0043174); // #$1234
    TEST("NOT.L", "D2",               0043202);
    UNKN(                             0043212); // A2
    TEST("NOT.L", "(A2)",             0043222);
    TEST("NOT.L", "(A2)+",            0043232);
    TEST("NOT.L", "-(A2)",            0043242);
    TEST("NOT.L", "($1234,A2)",       0043252, 0x1234);
    TEST("NOT.L", "(18,A2,D3.W)",     0043262, 0x3012);
    TEST("NOT.L", "($001234).W",      0043270, 0x1234);
    TEST("NOT.L", "($123454).L",      0043271, 0x0012, 0x3454);
    UNKN(                             0043272); // (*+$1234,PC)
    UNKN(                             0043273); // (*+10,PC,D3.W)
    UNKN(                             0043274); // #$12345678

    // OR src,Dn, 010|Dn|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("OR.B", "D2, D7",             0107002);
    UNKN(                              0107012); // A2, D7
    TEST("OR.B", "(A2), D7",           0107022);
    TEST("OR.B", "(A2)+, D7",          0107032);
    TEST("OR.B", "-(A2), D7",          0107042);
    TEST("OR.B", "($1234,A2), D7",     0107052, 0x1234);
    TEST("OR.B", "(18,A2,D3.L), D7",   0107062, 0x3812);
    TEST("OR.B", "($FFFFFF).W, D7",    0107070, 0xFFFF);
    TEST("OR.B", "($123456).L, D7",    0107071, 0x0012, 0x3456);
    TEST("OR.B", "(*+$1234,PC), D7",   0107072, 0x1232);
    TEST("OR.B", "(*+18,PC,D3.W), D7", 0107073, 0x3010);
    TEST("OR.B", "#$34, D7",           0107074, 0x0034);
    TEST("OR.W", "D2, D7",             0107102);
    UNKN(                              0107112); // A2, D7
    TEST("OR.W", "(A2), D7",           0107122);
    TEST("OR.W", "(A2)+, D7",          0107132);
    TEST("OR.W", "-(A2), D7",          0107142);
    TEST("OR.W", "($1234,A2), D7",     0107152, 0x1234);
    TEST("OR.W", "(18,A2,D3.L), D7",   0107162, 0x3812);
    TEST("OR.W", "($FFFFFE).W, D7",    0107170, 0xFFFE);
    TEST("OR.W", "($123456).L, D7",    0107171, 0x0012, 0x3456);
    TEST("OR.W", "(*+$1234,PC), D7",   0107172, 0x1232);
    TEST("OR.W", "(*+18,PC,D3.W), D7", 0107173, 0x3010);
    TEST("OR.W", "#$0034, D7",         0107174, 0x0034);
    TEST("OR.L", "D2, D7",             0107202);
    UNKN(                              0107212); // A2, D7
    TEST("OR.L", "(A2), D7",           0107222);
    TEST("OR.L", "(A2)+, D7",          0107232);
    TEST("OR.L", "-(A2), D7",          0107242);
    TEST("OR.L", "($1234,A2), D7",     0107252, 0x1234);
    TEST("OR.L", "(18,A2,D3.L), D7",   0107262, 0x3812);
    TEST("OR.L", "($FFFFFC).W, D7",    0107270, 0xFFFC);
    TEST("OR.L", "($123454).L, D7",    0107271, 0x0012, 0x3454);
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
    TEST("OR.B", "D7, ($FFFFFF).W",   0107470, 0xFFFF);
    TEST("OR.B", "D7, ($123456).L",   0107471, 0x0012, 0x3456);
    UNKN(                             0107472); // D7, (*+$1234,PC)
    UNKN(                             0107473); // D7, (*+10,PC,D3.W)
    UNKN(                             0107474); // D7, #$34
    UNKN(                             0107502); // D7, D2
    UNKN(                             0107512); // D7, A2
    TEST("OR.W", "D7, (A2)",          0107522);
    TEST("OR.W", "D7, (A2)+",         0107532);
    TEST("OR.W", "D7, -(A2)",         0107542);
    TEST("OR.W", "D7, ($1234,A2)",    0107552, 0x1234);
    TEST("OR.W", "D7, (18,A2,D3.L)",  0107562, 0x3812);
    TEST("OR.W", "D7, ($FFFFFE).W",   0107570, 0xFFFE);
    TEST("OR.W", "D7, ($123456).L",   0107571, 0x0012, 0x3456);
    UNKN(                             0107572); // D7, (*+$1234,PC)
    UNKN(                             0107573); // D7, (*+10,PC,D3.W)
    UNKN(                             0107574); // D7, #$1234
    UNKN(                             0107602); // D7, D2
    UNKN(                             0107612); // D7, A2
    TEST("OR.L", "D7, (A2)",          0107622);
    TEST("OR.L", "D7, (A2)+",         0107632);
    TEST("OR.L", "D7, -(A2)",         0107642);
    TEST("OR.L", "D7, ($1234,A2)",    0107652, 0x1234);
    TEST("OR.L", "D7, (18,A2,D3.L)",  0107662, 0x3812);
    TEST("OR.L", "D7, ($FFFFFC).W",   0107670, 0xFFFC);
    TEST("OR.L", "D7, ($123454).L",   0107671, 0x0012, 0x3454);
    UNKN(                             0107672); // D7, (*+$1234,PC)
    UNKN(                             0107673); // D7, (*+10,PC,D3.W)
    UNKN(                             0107674); // D7, #$12345678

    // ORI #nn,dst: 0000|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("ORI.B", "#18, D2",           0000002, 0x0012);
    UNKN(                              0000012); // #18, A2
    TEST("ORI.B", "#18, (A2)",         0000022, 0x0012);
    TEST("ORI.B", "#18, (A2)+",        0000032, 0x0012);
    TEST("ORI.B", "#18, -(A2)",        0000042, 0x0012);
    TEST("ORI.B", "#18, ($1234,A2)",   0000052, 0x0012, 0x1234);
    TEST("ORI.B", "#18, (18,A2,D3.W)", 0000062, 0x0012, 0x3012);
    TEST("ORI.B", "#18, ($001234).W",  0000070, 0x0012, 0x1234);
    TEST("ORI.B", "#18, ($123456).L",  0000071, 0x0012, 0x0012, 0x3456);
    UNKN(                              0000072); // #18, (*+$1234,PC)
    UNKN(                              0000073); // #18, (*+10,PC,D3.W)
    TEST("ORI",   "#18, CCR",          0000074, 0x0012); // ORI CCR
    TEST("ORI.W", "#$5678, D2",           0000102, 0x5678);
    UNKN(                                 0000112); // #$5678, A2
    TEST("ORI.W", "#$5678, (A2)",         0000122, 0x5678);
    TEST("ORI.W", "#$5678, (A2)+",        0000132, 0x5678);
    TEST("ORI.W", "#$5678, -(A2)",        0000142, 0x5678);
    TEST("ORI.W", "#$5678, ($1234,A2)",   0000152, 0x5678, 0x1234);
    TEST("ORI.W", "#$5678, (18,A2,D3.W)", 0000162, 0x5678, 0x3012);
    TEST("ORI.W", "#$5678, ($001234).W",  0000170, 0x5678, 0x1234);
    TEST("ORI.W", "#$5678, ($123456).L",  0000171, 0x5678, 0x0012, 0x3456);
    UNKN(                                 0000172); // #$5678, (*+$1234,PC)
    UNKN(                                 0000173); // #$5678, (*+10,PC,D3.W)
    TEST("ORI",   "#$5678, SR",           0000174, 0x5678); // ORI SR
    TEST("ORI.L", "#$3456789A, D2",           0000202, 0x3456, 0x789A);
    UNKN(                                     0000212); // #$3456789A, A2
    TEST("ORI.L", "#$3456789A, (A2)",         0000222, 0x3456, 0x789A);
    TEST("ORI.L", "#$3456789A, (A2)+",        0000232, 0x3456, 0x789A);
    TEST("ORI.L", "#$3456789A, -(A2)",        0000242, 0x3456, 0x789A);
    TEST("ORI.L", "#$3456789A, ($1234,A2)",   0000252, 0x3456, 0x789A, 0x1234);
    TEST("ORI.L", "#$3456789A, (18,A2,D3.W)", 0000262, 0x3456, 0x789A, 0x3012);
    TEST("ORI.L", "#$3456789A, ($001234).W",  0000270, 0x3456, 0x789A, 0x1234);
    TEST("ORI.L", "#$34567898, ($123454).L",  0000271, 0x3456, 0x7898, 0x0012, 0x3454);
    UNKN(                                     0000272); // #$3456789A, (*+$1234,PC)
    UNKN(                                     0000273); // #$3456789A, (*+10,PC,D3.W)
    UNKN(                                     0000274); // #$3456789A, #$12345678
}

static void test_shift_rotate() {
    // ASL Dx,Dy: 016|Dx|Sz|4|Dy, Sz:B=4/W=5/L=6
    TEST("ASL.B", "D2, D7", 0162447);
    TEST("ASL.W", "D2, D7", 0162547);
    TEST("ASL.L", "D2, D7", 0162647);

    // ASL #nn,Dy: 016|nn|Sz|0|Dy, Sz:B=4/W=5/L=6
    TEST("ASL.B", "#1, D7", 0161407);
    TEST("ASL.W", "#4, D7", 0164507);
    TEST("ASL.L", "#8, D7", 0160607);

    // ASL dst: 01607|M|Rn
    UNKN(                          0160702); // D2
    UNKN(                          0160712); // A2
    TEST("ASL.W", "(A2)",          0160722);
    TEST("ASL.W", "(A2)+",         0160732);
    TEST("ASL.W", "-(A2)",         0160742);
    TEST("ASL.W", "($2345,A2)",    0160752, 0x2345);
    TEST("ASL.W", "($23,A2,D3.L)", 0160762, 0x3823);
    TEST("ASL.W", "($002346).W",   0160770, 0x2346);
    TEST("ASL.W", "($234568).L",   0160771, 0x0023, 0x4568);
    UNKN(                          0160772); // (*+$1234,PC)
    UNKN(                          0160773); // (*+10,PC,D3.W)
    UNKN(                          0160774); // #$1234

    // ASR Dx,Dy: 016|Dx|Sz|4|Dy, Sz:B=0/W=1/L=2
    TEST("ASR.B", "D2, D7", 0162047);
    TEST("ASR.W", "D2, D7", 0162147);
    TEST("ASR.L", "D2, D7", 0162247);

    // ASR #nn,Dy: 016|Dx|Sz|0|Dy, Sz:B=0/W=1/L=2
    TEST("ASR.B", "#1, D7", 0161007);
    TEST("ASR.W", "#4, D7", 0164107);
    TEST("ASR.L", "#8, D7", 0160207);

    // ASR dst: 01603|M|Rn
    UNKN(                          0160302); // D2
    UNKN(                          0160312); // A2
    TEST("ASR.W", "(A2)",          0160322);
    TEST("ASR.W", "(A2)+",         0160332);
    TEST("ASR.W", "-(A2)",         0160342);
    TEST("ASR.W", "($2345,A2)",    0160352, 0x2345);
    TEST("ASR.W", "($23,A2,D3.L)", 0160362, 0x3823);
    TEST("ASR.W", "($002346).W",   0160370, 0x2346);
    TEST("ASR.W", "($234568).L",   0160371, 0x0023, 0x4568);
    UNKN(                          0160372); // (*+$1234,PC)
    UNKN(                          0160373); // (*+10,PC,D3.W)
    UNKN(                          0160374); // #$1234

    // LSL Dx,Dy: 016|Dx|Sz|5|Dy, Sz:B=4/W=5/L=6
    TEST("LSL.B", "D2, D7", 0162457);
    TEST("LSL.W", "D2, D7", 0162557);
    TEST("LSL.L", "D2, D7", 0162657);

    // LSL #n,Dy: 016|nn|Sz|1|Dy: Sz:B=4/W=5/L=6
    TEST("LSL.B", "#1, D7", 0161417);
    TEST("LSL.W", "#4, D7", 0164517);
    TEST("LSL.L", "#8, D7", 0160617);

    // LSL dst: 01617|M|Rn
    UNKN(                          0161702); // D2
    UNKN(                          0161712); // A2
    TEST("LSL.W", "(A2)",          0161722);
    TEST("LSL.W", "(A2)+",         0161732);
    TEST("LSL.W", "-(A2)",         0161742);
    TEST("LSL.W", "($2345,A2)",    0161752, 0x2345);
    TEST("LSL.W", "($23,A2,D3.L)", 0161762, 0x3823);
    TEST("LSL.W", "($002346).W",   0161770, 0x2346);
    TEST("LSL.W", "($234568).L",   0161771, 0x0023, 0x4568);
    UNKN(                          0161772); // (*+$1234,PC)
    UNKN(                          0161773); // (*+10,PC,D3.W)
    UNKN(                          0161774); // #$1234

    // LSR Dx,Dy: 016|Dx|Sz|5|Dy, Sz:B=0/W=1/L=2
    TEST("LSR.B", "D2, D7", 0162057);
    TEST("LSR.W", "D2, D7", 0162157);
    TEST("LSR.L", "D2, D7", 0162257);

    // LSR #n,Dy: 016|nn|Sz|1|Dy, Sz:B=0/W=1/L=2
    TEST("LSR.B", "#1, D7", 0161017);
    TEST("LSR.W", "#4, D7", 0164117);
    TEST("LSR.L", "#8, D7", 0160217);

    // LSR dst: 01613|M|Rn
    UNKN(                          0161302); // D2
    UNKN(                          0161312); // A2
    TEST("LSR.W", "(A2)",          0161322);
    TEST("LSR.W", "(A2)+",         0161332);
    TEST("LSR.W", "-(A2)",         0161342);
    TEST("LSR.W", "($2345,A2)",    0161352, 0x2345);
    TEST("LSR.W", "($23,A2,D3.L)", 0161362, 0x3823);
    TEST("LSR.W", "($002346).W",   0161370, 0x2346);
    TEST("LSR.W", "($234568).L",   0161371, 0x0023, 0x4568);
    UNKN(                          0161372); // (*+$1234,PC)
    UNKN(                          0161373); // (*+10,PC,D3.W)
    UNKN(                          0161374); // #$1234

    // ROL Dx,Dy: 016|Dx|Sz|7|Dy, Sz:B=4/W=5/L=6
    TEST("ROL.B", "D2, D7", 0162477);
    TEST("ROL.W", "D2, D7", 0162577);
    TEST("ROL.L", "D2, D7", 0162677);

    // ROL #n,Dy: 016|nn|Sz|3|Dy, Sz:B=4/W=5/L=6
    TEST("ROL.B", "#1, D7", 0161437);
    TEST("ROL.W", "#4, D7", 0164537);
    TEST("ROL.L", "#8, D7", 0160637);

    // ROL dst: 01637|M|Rn
    UNKN(                          0163702); // D2
    UNKN(                          0163712); // A2
    TEST("ROL.W", "(A2)",          0163722);
    TEST("ROL.W", "(A2)+",         0163732);
    TEST("ROL.W", "-(A2)",         0163742);
    TEST("ROL.W", "($2345,A2)",    0163752, 0x2345);
    TEST("ROL.W", "($23,A2,D3.L)", 0163762, 0x3823);
    TEST("ROL.W", "($002346).W",   0163770, 0x2346);
    TEST("ROL.W", "($234568).L",   0163771, 0x0023, 0x4568);
    UNKN(                          0163772); // (*+$1234,PC)
    UNKN(                          0163773); // (*+10,PC,D3.W)
    UNKN(                          0163774); // #$1234

    // ROR Dx,Dy: 016|Dx|Sz|7|Dy, Sz:B=0/W=1/L=2
    TEST("ROR.B", "D2, D7", 0162077);
    TEST("ROR.W", "D2, D7", 0162177);
    TEST("ROR.L", "D2, D7", 0162277);

    // ROR #n,Dy: 016|Dx|Sz|3|Dy, Sz:B=0/W=1/L=2
    TEST("ROR.B", "#1, D7", 0161037);
    TEST("ROR.W", "#4, D7", 0164137);
    TEST("ROR.L", "#8, D7", 0160237);

    // ROR dst: 01633|M|Rn
    UNKN(                          0163302); // D2
    UNKN(                          0163312); // A2
    TEST("ROR.W", "(A2)",          0163322);
    TEST("ROR.W", "(A2)+",         0163332);
    TEST("ROR.W", "-(A2)",         0163342);
    TEST("ROR.W", "($2345,A2)",    0163352, 0x2345);
    TEST("ROR.W", "($23,A2,D3.L)", 0163362, 0x3823);
    TEST("ROR.W", "($002346).W",   0163370, 0x2346);
    TEST("ROR.W", "($234568).L",   0163371, 0x0023, 0x4568);
    UNKN(                          0163372); // (*+$1234,PC)
    UNKN(                          0163373); // (*+10,PC,D3.W)
    UNKN(                          0163374); // #$1234

    // ROXL Dx,Dy: 016|Dx|Sz|6|Dy, Sz:B=4/W=5/L=6
    TEST("ROXL.B", "D2, D7", 0162467);
    TEST("ROXL.W", "D2, D7", 0162567);
    TEST("ROXL.L", "D2, D7", 0162667);

    // ROXL #n,Dy: 016|nn|Sz|2|Dy, Sz:B=4/W=5/L=6
    TEST("ROXL.B", "#1, D7", 0161427);
    TEST("ROXL.W", "#4, D7", 0164527);
    TEST("ROXL.L", "#8, D7", 0160627);

    // ROXL dst: 01627|M|Rn
    UNKN(                           0162702); // D2
    UNKN(                           0162712); // A2
    TEST("ROXL.W", "(A2)",          0162722);
    TEST("ROXL.W", "(A2)+",         0162732);
    TEST("ROXL.W", "-(A2)",         0162742);
    TEST("ROXL.W", "($2345,A2)",    0162752, 0x2345);
    TEST("ROXL.W", "($23,A2,D3.L)", 0162762, 0x3823);
    TEST("ROXL.W", "($002346).W",   0162770, 0x2346);
    TEST("ROXL.W", "($234568).L",   0162771, 0x0023, 0x4568);
    UNKN(                           0162772); // (*+$1234,PC)
    UNKN(                           0162773); // (*+10,PC,D3.W)
    UNKN(                           0162774); // #$1234

    // ROXR Dx,Dy: 016|Dx|Sz|6|Dy, Sz:B=0/W=1/L=2
    TEST("ROXR.B", "D2, D7", 0162067);
    TEST("ROXR.W", "D2, D7", 0162167);
    TEST("ROXR.L", "D2, D7", 0162267);

    // ROXR #n,Dy: 016|nn|Sz|2|Dy, Sz:B=0/W=1/L=2
    TEST("ROXR.B", "#1, D7", 0161027);
    TEST("ROXR.W", "#4, D7", 0164127);
    TEST("ROXR.L", "#8, D7", 0160227);

    // ROXR dst: 01623|M|Rn
    UNKN(                           0162302); // D2
    UNKN(                           0162312); // A2
    TEST("ROXR.W", "(A2)",          0162322);
    TEST("ROXR.W", "(A2)+",         0162332);
    TEST("ROXR.W", "-(A2)",         0162342);
    TEST("ROXR.W", "($2345,A2)",    0162352, 0x2345);
    TEST("ROXR.W", "($23,A2,D3.L)", 0162362, 0x3823);
    TEST("ROXR.W", "($002346).W",   0162370, 0x2346);
    TEST("ROXR.W", "($234568).L",   0162371, 0x0023, 0x4568);
    UNKN(                           0162372); // (*+$1234,PC)
    UNKN(                           0162373); // (*+10,PC,D3.W)
    UNKN(                           0162374); // #$1234

    // SWAP Dn: 004410|Dn
    TEST("SWAP", "D0", 0044100);
    TEST("SWAP", "D7", 0044107);
}

static void test_bit() {
    // BCHG Dx,dst: 000|Dx|5|M|Rn
    TEST("BCHG.L", "D7, D2",           0007502);
    TEST("MOVEP.L", "($1234,A2), D7",  0007512, 0x1234); // MOVEP.L
    TEST("BCHG.B", "D7, (A2)",         0007522);
    TEST("BCHG.B", "D7, (A2)+",        0007532);
    TEST("BCHG.B", "D7, -(A2)",        0007542);
    TEST("BCHG.B", "D7, ($1234,A2)",   0007552, 0x1234);
    TEST("BCHG.B", "D7, (18,A2,D3.W)", 0007562, 0x3012);
    TEST("BCHG.B", "D7, ($001234).W",  0007570, 0x1234);
    TEST("BCHG.B", "D7, ($012345).L",  0007571, 0x0001, 0x2345);
    UNKN(                              0007572); // D7, (*+$1234,PC)
    UNKN(                              0007573); // D7, (*+10,PC,D3.W)
    UNKN(                              0007574); // D7, #$34

    // BCHG #n,dst: 00041|M|Rn
    TEST("BCHG.L", "#0, D2",                       0004102, 0x0000);
    TEST("BCHG.L", "#31, D2",                      0004102, 0x001F);
    ERRT("BCHG.L", "#32, D2",  ILLEGAL_BIT_NUMBER, 0004102, 0x0020);
    UNKN(                                          0004112); // #6, A2
    TEST("BCHG.B", "#7, (A2)",                     0004122, 0x0007);
    ERRT("BCHG.B", "#8, (A2)", ILLEGAL_BIT_NUMBER, 0004122, 0x0008);
    TEST("BCHG.B", "#6, (A2)+",                    0004132, 0x0006);
    TEST("BCHG.B", "#5, -(A2)",                    0004142, 0x0005);
    TEST("BCHG.B", "#4, ($1234,A2)",               0004152, 0x0004, 0x1234);
    TEST("BCHG.B", "#3, (18,A2,D3.W)",             0004162, 0x0003, 0x3012);
    TEST("BCHG.B", "#2, ($001234).W",              0004170, 0x0002, 0x1234);
    TEST("BCHG.B", "#1, ($012345).L",              0004171, 0x0001, 0x0001, 0x2345);
    UNKN(                                          0004172); // #0, (*+$1234,PC)
    UNKN(                                          0004173); // #7, (*+10,PC,D3.W)
    UNKN(                                          0004174); // #6, #$34

    // BCLR Dx,dst: 000|Dx|6|M|Rn
    TEST("BCLR.L", "D7, D2",           0007602);
    TEST("MOVEP.W", "D7, ($1234,A2)",  0007612, 0x1234); // MOVEP.W
    TEST("BCLR.B", "D7, (A2)",         0007622);
    TEST("BCLR.B", "D7, (A2)+",        0007632);
    TEST("BCLR.B", "D7, -(A2)",        0007642);
    TEST("BCLR.B", "D7, ($1234,A2)",   0007652, 0x1234);
    TEST("BCLR.B", "D7, (18,A2,D3.W)", 0007662, 0x3012);
    TEST("BCLR.B", "D7, ($001234).W",  0007670, 0x1234);
    TEST("BCLR.B", "D7, ($012345).L",  0007671, 0x0001, 0x2345);
    UNKN(                              0007672); // D7, (*+$1234,PC)
    UNKN(                              0007673); // D7, (*+10,PC,D3.W)
    UNKN(                              0007674); // D7, #$34

    // BCLR #n,dst: 00042|M|Rn
    TEST("BCLR.L", "#0, D2",           0004202, 0x0000);
    UNKN(                              0004212); // #1, A2
    TEST("BCLR.B", "#7, (A2)",         0004222, 0x0007);
    TEST("BCLR.B", "#6, (A2)+",        0004232, 0x0006);
    TEST("BCLR.B", "#5, -(A2)",        0004242, 0x0005);
    TEST("BCLR.B", "#4, ($1234,A2)",   0004252, 0x0004, 0x1234);
    TEST("BCLR.B", "#3, (18,A2,D3.W)", 0004262, 0x0003, 0x3012);
    TEST("BCLR.B", "#2, ($001234).W",  0004270, 0x0002, 0x1234);
    TEST("BCLR.B", "#1, ($012345).L",  0004271, 0x0001, 0x0001, 0x2345);
    UNKN(                              0004272); // #0, (*+$1234,PC)
    UNKN(                              0004273); // #7, (*+10,PC,D3.W)
    UNKN(                              0004274); // #6, #$34

    // BSET Dx,dst: 000|Dx|7|M|Rn
    TEST("BSET.L", "D7, D2",           0007702);
    TEST("MOVEP.L", "D7, ($1234,A2)",  0007712, 0x1234); // MOVEP.L
    TEST("BSET.B", "D7, (A2)",         0007722);
    TEST("BSET.B", "D7, (A2)+",        0007732);
    TEST("BSET.B", "D7, -(A2)",        0007742);
    TEST("BSET.B", "D7, ($1234,A2)",   0007752, 0x1234);
    TEST("BSET.B", "D7, (18,A2,D3.W)", 0007762, 0x3012);
    TEST("BSET.B", "D7, ($001234).W",  0007770, 0x1234);
    TEST("BSET.B", "D7, ($012345).L",  0007771, 0x0001, 0x2345);
    UNKN(                              0007772); // D7, (*+$1234,PC)
    UNKN(                              0007773); // D7, (*+10,PC,D3.W)
    UNKN(                              0007774); // D7, #$34

    // BSET #n,dst: 00043|M|Rn
    TEST("BSET.L", "#0, D2",           0004302, 0x0000);
    UNKN(                              0004312); // #1, A2
    TEST("BSET.B", "#7, (A2)",         0004322, 0x0007);
    TEST("BSET.B", "#6, (A2)+",        0004332, 0x0006);
    TEST("BSET.B", "#5, -(A2)",        0004342, 0x0005);
    TEST("BSET.B", "#4, ($1234,A2)",   0004352, 0x0004, 0x1234);
    TEST("BSET.B", "#3, (18,A2,D3.W)", 0004362, 0x0003, 0x3012);
    TEST("BSET.B", "#2, ($001234).W",  0004370, 0x0002, 0x1234);
    TEST("BSET.B", "#1, ($012345).L",  0004371, 0x0001, 0x0001, 0x2345);
    UNKN(                              0004372); // #0, (*+$1234,PC)
    UNKN(                              0004373); // #7, (*+10,PC,D3.W)
    UNKN(                              0004374); // #6, #$34

    // BTST Dx,dst: 000|Dx|4|M|Rn
    TEST("BTST.L", "D7, D2",             0007402);
    TEST("MOVEP.W", "($1234,A2), D7",    0007412, 0x1234); // MOVEP.W
    TEST("BTST.B", "D7, (A2)",           0007422);
    TEST("BTST.B", "D7, (A2)+",          0007432);
    TEST("BTST.B", "D7, -(A2)",          0007442);
    TEST("BTST.B", "D7, ($1234,A2)",     0007452, 0x1234);
    TEST("BTST.B", "D7, (18,A2,D3.W)",   0007462, 0x3012);
    TEST("BTST.B", "D7, ($001234).W",    0007470, 0x1234);
    TEST("BTST.B", "D7, ($012345).L",    0007471, 0x0001, 0x2345);
    TEST("BTST.B", "D7, (*+$1234,PC)",   0007472, 0x1232);
    TEST("BTST.B", "D7, (*+35,PC,D3.L)", 0007473, 0x3821);
    UNKN(                                0007474); // D7, #$34

    // BTST #n,dst: 00040|M|Rn
    TEST("BTST.L", "#0, D2",             0004002, 0x0000);
    UNKN(                                0004012); // #1, A2
    TEST("BTST.B", "#7, (A2)",           0004022, 0x0007);
    TEST("BTST.B", "#6, (A2)+",          0004032, 0x0006);
    TEST("BTST.B", "#5, -(A2)",          0004042, 0x0005);
    TEST("BTST.B", "#4, ($1234,A2)",     0004052, 0x0004, 0x1234);
    TEST("BTST.B", "#3, (18,A2,D3.W)",   0004062, 0x0003, 0x3012);
    TEST("BTST.B", "#2, ($001234).W",    0004070, 0x0002, 0x1234);
    TEST("BTST.B", "#1, ($012345).L",    0004071, 0x0001, 0x0001, 0x2345);
    TEST("BTST.B", "#0, (*+$1234,PC)",   0004072, 0x0000, 0x1230);
    TEST("BTST.B", "#7, (*+35,PC,D3.L)", 0004073, 0x0007, 0x381F);
    UNKN(                                0004074); // #6, #$34
}

static void test_bcd() {
    // ABCD Dx,Dy: 014|Dy|40|Dx
    TEST("ABCD", "D2, D7", 0147402);

    // ABCD -(Ax), -(Ay)" 014|Ay|41|Ax
    TEST("ABCD", "-(A2), -(A7)", 0147412);

    // NBCD dst: 00440|M|Rn
    TEST("NBCD", "D2",                0044002);
    UNKN(                             0044012); // A2
    TEST("NBCD", "(A2)",              0044022);
    TEST("NBCD", "(A2)+",             0044032);
    TEST("NBCD", "-(A2)",             0044042);
    TEST("NBCD", "($1234,A2)",        0044052, 0x1234);
    TEST("NBCD", "(18,A2,D3.W)",      0044062, 0x3012);
    TEST("NBCD", "($001234).W",       0044070, 0x1234);
    TEST("NBCD", "($123456).L",       0044071, 0x0012, 0x3456);
    UNKN(                             0044072); // (*+$1234,PC)
    UNKN(                             0044073); // (*+10,PC,D3.W)
    UNKN(                             0044074); // #$34

    // SBCD Dx,Dy: 010|Dy|40|Dx
    TEST("SBCD", "D2, D7", 0107402);

    // SBCD -(Ax), -(Ay): 010|Ay|41|Ax
    TEST("SBCD", "-(A2), -(A7)", 0107412);
}

static void test_program() {
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

    // DBcc Dn,labelL 005|cc|31|Dn
    ATEST(0x10000, "DBRA", "D2, *-$7FFE",                      0050312 | 0x100, 0x8000);
    ATEST(0x10000, "DBRA", "D2, *-$007E",                      0050312 | 0x100, 0xFF80);
    ATEST(0x10000, "DBRA", "D2, *",                            0050312 | 0x100, 0xFFFE);
    ATEST(0x10000, "DBRA", "D2, *+2",                          0050312 | 0x100, 0x0000);
    ATEST(0x10000, "DBRA", "D2, *+$0080",                      0050312 | 0x100, 0x007E);
    ATEST(0x10000, "DBRA", "D2, *+$8000",                      0050312 | 0x100, 0x7FFE);
    AERRT(0x10000, "DBRA", "D2, *-$7FFD", OPERAND_NOT_ALIGNED, 0050312 | 0x100, 0x8001);
    AERRT(0x10000, "DBRA", "D2, *+$8001", OPERAND_NOT_ALIGNED, 0050312 | 0x100, 0x7FFF);
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
    TEST("ST", "($001233).W",  0050370 | 0x000, 0x1233);
    TEST("ST", "($123456).L",  0050371 | 0x000, 0x0012, 0x3456);
    UNKN(                      0050372 | 0x000); // (*+$1234,PC)
    UNKN(                      0050373 | 0x000); // (*+10,PC,D3.W)
    UNKN(                      0050374 | 0x000); // #$34
    TEST("SF", "D2",           0050302 | 0x100);
    TEST("DBRA", "D2, *+2",    0050312 | 0x100, 0); // DBRA
    TEST("SF", "(A2)",         0050322 | 0x100);
    TEST("SF", "(A2)+",        0050332 | 0x100);
    TEST("SF", "-(A2)",        0050342 | 0x100);
    TEST("SF", "($1234,A2)",   0050352 | 0x100, 0x1234);
    TEST("SF", "(18,A2,D3.W)", 0050362 | 0x100, 0x3012);
    TEST("SF", "($001234).W",  0050370 | 0x100, 0x1234);
    TEST("SF", "($123456).L",  0050371 | 0x100, 0x0012, 0x3456);
    UNKN(                      0050372 | 0x100); // (*+$1234,PC)
    UNKN(                      0050373 | 0x100); // (*+10,PC,D3.W)
    UNKN(                      0050374 | 0x100); // #$34
    TEST("SHI", "D2",           0050302 | 0x200);
    TEST("DBHI", "D2, *+2",     0050312 | 0x200, 0); // DBHI
    TEST("SHI", "(A2)",         0050322 | 0x200);
    TEST("SHI", "(A2)+",        0050332 | 0x200);
    TEST("SHI", "-(A2)",        0050342 | 0x200);
    TEST("SHI", "($1234,A2)",   0050352 | 0x200, 0x1234);
    TEST("SHI", "(18,A2,D3.W)", 0050362 | 0x200, 0x3012);
    TEST("SHI", "($001234).W",  0050370 | 0x200, 0x1234);
    TEST("SHI", "($123456).L",  0050371 | 0x200, 0x0012, 0x3456);
    UNKN(                       0050372 | 0x200); // (*+$1234,PC)
    UNKN(                       0050373 | 0x200); // (*+10,PC,D3.W)
    UNKN(                       0050374 | 0x200); // #$34
    TEST("SLS", "D2",           0050302 | 0x300);
    TEST("DBLS", "D2, *+2",     0050312 | 0x300, 0); // DBLS
    TEST("SLS", "(A2)",         0050322 | 0x300);
    TEST("SLS", "(A2)+",        0050332 | 0x300);
    TEST("SLS", "-(A2)",        0050342 | 0x300);
    TEST("SLS", "($1234,A2)",   0050352 | 0x300, 0x1234);
    TEST("SLS", "(18,A2,D3.W)", 0050362 | 0x300, 0x3012);
    TEST("SLS", "($001234).W",  0050370 | 0x300, 0x1234);
    TEST("SLS", "($123456).L",  0050371 | 0x300, 0x0012, 0x3456);
    UNKN(                       0050372 | 0x300); // (*+$1234,PC)
    UNKN(                       0050373 | 0x300); // (*+10,PC,D3.W)
    UNKN(                       0050374 | 0x300); // #$34
    TEST("SCC", "D2",           0050302 | 0x400);
    TEST("DBCC", "D2, *+2",     0050312 | 0x400, 0); // DBCC
    TEST("SCC", "(A2)",         0050322 | 0x400);
    TEST("SCC", "(A2)+",        0050332 | 0x400);
    TEST("SCC", "-(A2)",        0050342 | 0x400);
    TEST("SCC", "($1234,A2)",   0050352 | 0x400, 0x1234);
    TEST("SCC", "(18,A2,D3.W)", 0050362 | 0x400, 0x3012);
    TEST("SCC", "($001234).W",  0050370 | 0x400, 0x1234);
    TEST("SCC", "($123456).L",  0050371 | 0x400, 0x0012, 0x3456);
    UNKN(                       0050372 | 0x400); // (*+$1234,PC)
    UNKN(                       0050373 | 0x400); // (*+10,PC,D3.W)
    UNKN(                       0050374 | 0x400); // #$34
    TEST("SCS", "D2",           0050302 | 0x500);
    TEST("DBCS", "D2, *+2",     0050312 | 0x500, 0); // DBCS
    TEST("SCS", "(A2)",         0050322 | 0x500);
    TEST("SCS", "(A2)+",        0050332 | 0x500);
    TEST("SCS", "-(A2)",        0050342 | 0x500);
    TEST("SCS", "($1234,A2)",   0050352 | 0x500, 0x1234);
    TEST("SCS", "(18,A2,D3.W)", 0050362 | 0x500, 0x3012);
    TEST("SCS", "($001234).W",  0050370 | 0x500, 0x1234);
    TEST("SCS", "($123456).L",  0050371 | 0x500, 0x0012, 0x3456);
    UNKN(                       0050372 | 0x500); // (*+$1234,PC)
    UNKN(                       0050373 | 0x500); // (*+10,PC,D3.W)
    UNKN(                       0050374 | 0x500); // #$34
    TEST("SNE", "D2",           0050302 | 0x600);
    TEST("DBNE", "D2, *+2",     0050312 | 0x600, 0); // DBNE
    TEST("SNE", "(A2)",         0050322 | 0x600);
    TEST("SNE", "(A2)+",        0050332 | 0x600);
    TEST("SNE", "-(A2)",        0050342 | 0x600);
    TEST("SNE", "($1234,A2)",   0050352 | 0x600, 0x1234);
    TEST("SNE", "(18,A2,D3.W)", 0050362 | 0x600, 0x3012);
    TEST("SNE", "($001234).W",  0050370 | 0x600, 0x1234);
    TEST("SNE", "($123456).L",  0050371 | 0x600, 0x0012, 0x3456);
    UNKN(                       0050372 | 0x600); // (*+$1234,PC)
    UNKN(                       0050373 | 0x600); // (*+10,PC,D3.W)
    UNKN(                       0050374 | 0x600); // #$34
    TEST("SEQ", "D2",           0050302 | 0x700);
    TEST("DBEQ", "D2, *+2",     0050312 | 0x700, 0); // DBEQ
    TEST("SEQ", "(A2)",         0050322 | 0x700);
    TEST("SEQ", "(A2)+",        0050332 | 0x700);
    TEST("SEQ", "-(A2)",        0050342 | 0x700);
    TEST("SEQ", "($1234,A2)",   0050352 | 0x700, 0x1234);
    TEST("SEQ", "(18,A2,D3.W)", 0050362 | 0x700, 0x3012);
    TEST("SEQ", "($001234).W",  0050370 | 0x700, 0x1234);
    TEST("SEQ", "($123456).L",  0050371 | 0x700, 0x0012, 0x3456);
    UNKN(                       0050372 | 0x700); // (*+$1234,PC)
    UNKN(                       0050373 | 0x700); // (*+10,PC,D3.W)
    UNKN(                       0050374 | 0x700); // #$34
    TEST("SVC", "D2",           0050302 | 0x800);
    TEST("DBVC", "D2, *+2",     0050312 | 0x800, 0); // DBVC
    TEST("SVC", "(A2)",         0050322 | 0x800);
    TEST("SVC", "(A2)+",        0050332 | 0x800);
    TEST("SVC", "-(A2)",        0050342 | 0x800);
    TEST("SVC", "($1234,A2)",   0050352 | 0x800, 0x1234);
    TEST("SVC", "(18,A2,D3.W)", 0050362 | 0x800, 0x3012);
    TEST("SVC", "($001234).W",  0050370 | 0x800, 0x1234);
    TEST("SVC", "($123456).L",  0050371 | 0x800, 0x0012, 0x3456);
    UNKN(                       0050372 | 0x800); // (*+$1234,PC)
    UNKN(                       0050373 | 0x800); // (*+10,PC,D3.W)
    UNKN(                       0050374 | 0x800); // #$34
    TEST("SVS", "D2",           0050302 | 0x900);
    TEST("DBVS", "D2, *+2",     0050312 | 0x900, 0); // DBVS
    TEST("SVS", "(A2)",         0050322 | 0x900);
    TEST("SVS", "(A2)+",        0050332 | 0x900);
    TEST("SVS", "-(A2)",        0050342 | 0x900);
    TEST("SVS", "($1234,A2)",   0050352 | 0x900, 0x1234);
    TEST("SVS", "(18,A2,D3.W)", 0050362 | 0x900, 0x3012);
    TEST("SVS", "($001234).W",  0050370 | 0x900, 0x1234);
    TEST("SVS", "($123456).L",  0050371 | 0x900, 0x0012, 0x3456);
    UNKN(                       0050372 | 0x900); // (*+$1234,PC)
    UNKN(                       0050373 | 0x900); // (*+10,PC,D3.W)
    UNKN(                       0050374 | 0x900); // #$34
    TEST("SPL", "D2",           0050302 | 0xA00);
    TEST("DBPL", "D2, *+2",     0050312 | 0xA00, 0); // DBPL
    TEST("SPL", "(A2)",         0050322 | 0xA00);
    TEST("SPL", "(A2)+",        0050332 | 0xA00);
    TEST("SPL", "-(A2)",        0050342 | 0xA00);
    TEST("SPL", "($1234,A2)",   0050352 | 0xA00, 0x1234);
    TEST("SPL", "(18,A2,D3.W)", 0050362 | 0xA00, 0x3012);
    TEST("SPL", "($001234).W",  0050370 | 0xA00, 0x1234);
    TEST("SPL", "($123457).L",  0050371 | 0xA00, 0x0012, 0x3457);
    UNKN(                       0050372 | 0xA00); // (*+$1234,PC)
    UNKN(                       0050373 | 0xA00); // (*+10,PC,D3.W)
    UNKN(                       0050374 | 0xA00); // #$34
    TEST("SMI", "D2",           0050302 | 0xB00);
    TEST("DBMI", "D2, *+2",     0050312 | 0xB00, 0); // DBMI
    TEST("SMI", "(A2)",         0050322 | 0xB00);
    TEST("SMI", "(A2)+",        0050332 | 0xB00);
    TEST("SMI", "-(A2)",        0050342 | 0xB00);
    TEST("SMI", "($1234,A2)",   0050352 | 0xB00, 0x1234);
    TEST("SMI", "(18,A2,D3.W)", 0050362 | 0xB00, 0x3012);
    TEST("SMI", "($001234).W",  0050370 | 0xB00, 0x1234);
    TEST("SMI", "($123456).L",  0050371 | 0xB00, 0x0012, 0x3456);
    UNKN(                       0050372 | 0xB00); // (*+$1234,PC)
    UNKN(                       0050373 | 0xB00); // (*+10,PC,D3.W)
    UNKN(                       0050374 | 0xB00); // #$34
    TEST("SGE", "D2",           0050302 | 0xC00);
    TEST("DBGE", "D2, *+2",     0050312 | 0xC00, 0); // DBGE
    TEST("SGE", "(A2)",         0050322 | 0xC00);
    TEST("SGE", "(A2)+",        0050332 | 0xC00);
    TEST("SGE", "-(A2)",        0050342 | 0xC00);
    TEST("SGE", "($1234,A2)",   0050352 | 0xC00, 0x1234);
    TEST("SGE", "(18,A2,D3.W)", 0050362 | 0xC00, 0x3012);
    TEST("SGE", "($001234).W",  0050370 | 0xC00, 0x1234);
    TEST("SGE", "($123456).L",  0050371 | 0xC00, 0x0012, 0x3456);
    UNKN(                       0050372 | 0xC00); // (*+$1234,PC)
    UNKN(                       0050373 | 0xC00); // (*+10,PC,D3.W)
    UNKN(                       0050374 | 0xC00); // #$34
    TEST("SLT", "D2",           0050302 | 0xD00);
    TEST("DBLT", "D2, *+2",     0050312 | 0xD00, 0); // DBLT
    TEST("SLT", "(A2)",         0050322 | 0xD00);
    TEST("SLT", "(A2)+",        0050332 | 0xD00);
    TEST("SLT", "-(A2)",        0050342 | 0xD00);
    TEST("SLT", "($1234,A2)",   0050352 | 0xD00, 0x1234);
    TEST("SLT", "(18,A2,D3.W)", 0050362 | 0xD00, 0x3012);
    TEST("SLT", "($001234).W",  0050370 | 0xD00, 0x1234);
    TEST("SLT", "($123456).L",  0050371 | 0xD00, 0x0012, 0x3456);
    UNKN(                       0050372 | 0xD00); // (*+$1234,PC)
    UNKN(                       0050373 | 0xD00); // (*+10,PC,D3.W)
    UNKN(                       0050374 | 0xD00); // #$34
    TEST("SGT", "D2",           0050302 | 0xE00);
    TEST("DBGT", "D2, *+2",     0050312 | 0xE00, 0); // DBGT
    TEST("SGT", "(A2)",         0050322 | 0xE00);
    TEST("SGT", "(A2)+",        0050332 | 0xE00);
    TEST("SGT", "-(A2)",        0050342 | 0xE00);
    TEST("SGT", "($1234,A2)",   0050352 | 0xE00, 0x1234);
    TEST("SGT", "(18,A2,D3.W)", 0050362 | 0xE00, 0x3012);
    TEST("SGT", "($001234).W",  0050370 | 0xE00, 0x1234);
    TEST("SGT", "($123456).L",  0050371 | 0xE00, 0x0012, 0x3456);
    UNKN(                       0050372 | 0xE00); // (*+$1234,PC)
    UNKN(                       0050373 | 0xE00); // (*+10,PC,D3.W)
    UNKN(                       0050374 | 0xE00); // #$34
    TEST("SLE", "D2",           0050302 | 0xF00);
    TEST("DBLE", "D2, *+2",     0050312 | 0xF00, 0); // DBLE
    TEST("SLE", "(A2)",         0050322 | 0xF00);
    TEST("SLE", "(A2)+",        0050332 | 0xF00);
    TEST("SLE", "-(A2)",        0050342 | 0xF00);
    TEST("SLE", "($1234,A2)",   0050352 | 0xF00, 0x1234);
    TEST("SLE", "(18,A2,D3.W)", 0050362 | 0xF00, 0x3012);
    TEST("SLE", "($001234).W",  0050370 | 0xF00, 0x1234);
    TEST("SLE", "($123456).L",  0050371 | 0xF00, 0x0012, 0x3456);
    UNKN(                       0050372 | 0xF00); // (*+$1234,PC)
    UNKN(                       0050373 | 0xF00); // (*+10,PC,D3.W)
    UNKN(                       0050374 | 0xF00); // #$34

    // BRA label: 00600|disp
    ATEST(0x10000, "BRA", "*-$7FFE",                      0060000, 0x8000);
    ATEST(0x10000, "BRA", "*-$007E",                      0060000, 0xFF80);
    ATEST(0x10000, "BRA", "*-126",                        0060000 | 0x80);
    ATEST(0x10000, "BRA", "*",                            0060000 | 0xFE);
    ATEST(0x10000, "BRA", "*",                            0060000, 0xFFFE);
    ATEST(0x10000, "BRA", "*+2",                          0060000, 0x0000);
    ATEST(0x10000, "BRA", "*+128",                        0060000 | 0x7E);
    ATEST(0x10000, "BRA", "*+$0080",                      0060000, 0x007E);
    ATEST(0x10000, "BRA", "*+$8000",                      0060000, 0x7FFE);
    AERRT(0x10000, "BRA", "*-$7FFD", OPERAND_NOT_ALIGNED, 0060000, 0x8001);
    AERRT(0x10000, "BRA", "*-125",   OPERAND_NOT_ALIGNED, 0060000 | 0x81);
    AERRT(0x10000, "BRA", "*+1",     OPERAND_NOT_ALIGNED, 0060000 | 0xFF);
    AERRT(0x10000, "BRA", "*+3",     OPERAND_NOT_ALIGNED, 0060000 | 0x01);
    AERRT(0x10000, "BRA", "*+129",   OPERAND_NOT_ALIGNED, 0060000 | 0x7F);
    AERRT(0x10000, "BRA", "*+$8001", OPERAND_NOT_ALIGNED, 0060000, 0x7FFF);

    // BSR label: 00604|disp
    ATEST(0x10000, "BSR", "*-$7FFE",                      0060400, 0x8000);
    ATEST(0x10000, "BSR", "*-$007E",                      0060400, 0xFF80);
    ATEST(0x10000, "BSR", "*-126",                        0060400 | 0x80);
    ATEST(0x10000, "BSR", "*",                            0060400 | 0xFE);
    ATEST(0x10000, "BSR", "*",                            0060400, 0xFFFE);
    ATEST(0x10000, "BSR", "*+2",                          0060400, 0x0000);
    ATEST(0x10000, "BSR", "*+128",                        0060400 | 0x7E);
    ATEST(0x10000, "BSR", "*+$0080",                      0060400, 0x007E);
    ATEST(0x10000, "BSR", "*+$8000",                      0060400, 0x7FFE);
    AERRT(0x10000, "BSR", "*-$7FFD", OPERAND_NOT_ALIGNED, 0060400, 0x8001);
    AERRT(0x10000, "BSR", "*-125",   OPERAND_NOT_ALIGNED, 0060400 | 0x81);
    AERRT(0x10000, "BSR", "*+1",     OPERAND_NOT_ALIGNED, 0060400 | 0xFF);
    AERRT(0x10000, "BSR", "*+3",     OPERAND_NOT_ALIGNED, 0060400 | 0x01);
    AERRT(0x10000, "BSR", "*+129",   OPERAND_NOT_ALIGNED, 0060400 | 0x7F);
    AERRT(0x10000, "BSR", "*+$8001", OPERAND_NOT_ALIGNED, 0060400, 0x7FFF);

    // JMP dst: 00473|M|Rn
    UNKN(                         0047302); // D2
    UNKN(                         0047312); // A2
    TEST("JMP", "(A2)",           0047322);
    UNKN(                         0047332); // (A2)+
    UNKN(                         0047342); // -(A2)
    TEST("JMP", "($1234,A2)",     0047352, 0x1234);
    TEST("JMP", "(18,A2,D3.W)",   0047362, 0x3012);
    TEST("JMP", "($001234).W",    0047370, 0x1234);
    TEST("JMP", "($123456).L",    0047371, 0x0012, 0x3456);
    TEST("JMP", "(*+$1234,PC)",   0047372, 0x1232);
    TEST("JMP", "(*+18,PC,D3.L)", 0047373, 0x3810);
    UNKN(                         0047374); // #$1234

    // JSR dst, 00472|M|Rn
    UNKN(                         0047202); // D2
    UNKN(                         0047212); // A2
    TEST("JSR", "(A2)",           0047222);
    UNKN(                         0047232); // (A2)+
    UNKN(                         0047242); // -(A2)
    TEST("JSR", "($1234,A2)",     0047252, 0x1234);
    TEST("JSR", "(18,A2,D3.W)",   0047262, 0x3012);
    TEST("JSR", "($001234).W",    0047270, 0x1234);
    TEST("JSR", "($123456).L",    0047271, 0x0012, 0x3456);
    TEST("JSR", "(*+$1234,PC)",   0047272, 0x1232);
    TEST("JSR", "(*+18,PC,D3.L)", 0047273, 0x3810);
    UNKN(                         0047274); // #$1234

    // NOP
    TEST("NOP", "", 047161);

    // RTR
    TEST("RTR", "", 047167);

    // RTS
    TEST("RTS", "", 047165);

    // TST dst: 0045|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("TST.B", "D2",           0045002);
    UNKN(                         0045012); // A2
    TEST("TST.B", "(A2)",         0045022);
    TEST("TST.B", "(A2)+",        0045032);
    TEST("TST.B", "-(A2)",        0045042);
    TEST("TST.B", "($1234,A2)",   0045052, 0x1234);
    TEST("TST.B", "(18,A2,D3.W)", 0045062, 0x3012);
    TEST("TST.B", "($001234).W",  0045070, 0x1234);
    TEST("TST.B", "($123456).L",  0045071, 0x0012, 0x3456);
    UNKN(                         0045072); // (*+$1234,PC)
    UNKN(                         0045073); // (*+10,PC,D3.W)
    UNKN(                         0045074); // #$34
    TEST("TST.W", "D2",           0045102);
    UNKN(                         0045112); // A2
    TEST("TST.W", "(A2)",         0045122);
    TEST("TST.W", "(A2)+",        0045132);
    TEST("TST.W", "-(A2)",        0045142);
    TEST("TST.W", "($1234,A2)",   0045152, 0x1234);
    TEST("TST.W", "(18,A2,D3.W)", 0045162, 0x3012);
    TEST("TST.W", "($001234).W",  0045170, 0x1234);
    TEST("TST.W", "($123456).L",  0045171, 0x0012, 0x3456);
    UNKN(                         0045172); // (*+$1234,PC)
    UNKN(                         0045173); // (*+10,PC,D3.W)
    UNKN(                         0045174); // #$1234
    TEST("TST.L", "D2",           0045202);
    UNKN(                         0045212); // A2
    TEST("TST.L", "(A2)",         0045222);
    TEST("TST.L", "(A2)+",        0045232);
    TEST("TST.L", "-(A2)",        0045242);
    TEST("TST.L", "($1234,A2)",   0045252, 0x1234);
    TEST("TST.L", "(18,A2,D3.W)", 0045262, 0x3012);
    TEST("TST.L", "($001234).W",  0045270, 0x1234);
    TEST("TST.L", "($123454).L",  0045271, 0x0012, 0x3454);
    UNKN(                         0045272); // (*+$1234,PC)
    UNKN(                         0045273); // (*+10,PC,D3.W)
    UNKN(                         0045274); // #$12345678
}

static void test_system() {
    // ANDI #nn,SR
    TEST("ANDI", "#$1234, SR", 0001174, 0x1234);

    // EORI #nn,SR
    TEST("EORI", "#$1234, SR", 0005174, 0x1234);

    // MOVE src,SR: 00433|M|Rn
    TEST("MOVE.W", "D2, SR",                                0043302);
    UNKN(                                                   0043312); // A2, SR
    TEST("MOVE.W", "(A2), SR",                              0043322);
    TEST("MOVE.W", "(A2)+, SR",                             0043332);
    TEST("MOVE.W", "-(A2), SR",                             0043342);
    TEST("MOVE.W", "($1234,A2), SR",                        0043352, 0x1234);
    TEST("MOVE.W", "(18,A2,D3.W), SR",                      0043362, 0x3012);
    TEST("MOVE.W", "($001234).W, SR",                       0043370, 0x1234);
    ERRT("MOVE.W", "($001233).W, SR",  OPERAND_NOT_ALIGNED, 0043370, 0x1233);
    TEST("MOVE.W", "($234568).L, SR",                       0043371, 0x0023, 0x4568);
    ERRT("MOVE.W", "($234567).L, SR",  OPERAND_NOT_ALIGNED, 0043371, 0x0023, 0x4567);
    TEST("MOVE.W", "(*+$1234,PC), SR",                      0043372, 0x1232);
    ERRT("MOVE.W", "(*+$1235,PC), SR", OPERAND_NOT_ALIGNED, 0043372, 0x1233);
    TEST("MOVE.W", "(*-16,PC,D3.L), SR",                    0043373, 0x38EE);
    TEST("MOVE.W", "#$3456, SR",                            0043374, 0x3456);

    // MOVE SR,dst: 00403|M|Rn
    TEST("MOVE.W", "SR, D2",                               0040302);
    UNKN(                                                  0040312); // SR, A2
    TEST("MOVE.W", "SR, (A2)",                             0040322);
    TEST("MOVE.W", "SR, (A2)+",                            0040332);
    TEST("MOVE.W", "SR, -(A2)",                            0040342);
    TEST("MOVE.W", "SR, ($1234,A2)",                       0040352, 0x1234);
    TEST("MOVE.W", "SR, (18,A2,D3.W)",                     0040362, 0x3012);
    TEST("MOVE.W", "SR, ($001234).W",                      0040370, 0x1234);
    ERRT("MOVE.W", "SR, ($001233).W", OPERAND_NOT_ALIGNED, 0040370, 0x1233);
    TEST("MOVE.W", "SR, ($234568).L",                      0040371, 0x0023, 0x4568);
    ERRT("MOVE.W", "SR, ($234567).L", OPERAND_NOT_ALIGNED, 0040371, 0x0023, 0x4567);
    UNKN(                                                  0040372); // SR, (*+$1234,PC)
    UNKN(                                                  0040373); // SR, (*+10,PC,D3.W)
    UNKN(                                                  0040374); // SR, #$1234

    // MOVE USP,An: 004715|An
    TEST("MOVE", "USP, A2", 0047152);

    // MOVE An,USP: 004714|An
    TEST("MOVE", "A2, USP", 0047142);

    // ORI #nn,SR
    TEST("ORI", "#$1234, SR", 0000174, 0x1234);

    // RESET
    TEST("RESET", "", 047160);

    // RTE
    TEST("RTE", "", 047163);

    // STOP #nn
    TEST("STOP", "#$1234", 047162, 0x1234);

    // CHK src,Dn: 004|Dn|Sz|M|Rn, Sz:W=6/L=7
    TEST("CHK.W", "D2, D7",             0047602);
    UNKN(                               0047612); // A2, D7
    TEST("CHK.W", "(A2), D7",           0047622);
    TEST("CHK.W", "(A2)+, D7",          0047632);
    TEST("CHK.W", "-(A2), D7",          0047642);
    TEST("CHK.W", "($1234,A2), D7",     0047652, 0x1234);
    TEST("CHK.W", "(18,A2,D3.L), D7",   0047662, 0x3812);
    TEST("CHK.W", "($FFFFFE).W, D7",    0047670, 0xFFFE);
    TEST("CHK.W", "($123456).L, D7",    0047671, 0x0012, 0x3456);
    TEST("CHK.W", "(*+$1234,PC), D7",   0047672, 0x1232);
    TEST("CHK.W", "(*+18,PC,D3.W), D7", 0047673, 0x3010);
    TEST("CHK.W", "#$0034, D7",         0047674, 0x0034);

    // ILLEGAL
    TEST("ILLEGAL", "", 0045374);

    // TRAP #nn
    TEST("TRAP", "#0",  0047100);
    TEST("TRAP", "#15", 0047117);

    // TRAPV
    TEST("TRAPV", "", 047166);

    // ANDI #nn,CCR
    TEST("ANDI", "#$34, CCR", 0001074, 0x0034);

    // EORI #nn,CCR
    TEST("EORI", "#$34, CCR", 0005074, 0x0034);

    // MOVE src,CCR: 00423|M|R
    TEST("MOVE.W", "D2, CCR",                                0042302);
    UNKN(                                                    0042312); // A2, CCR
    TEST("MOVE.W", "(A2), CCR",                              0042322);
    TEST("MOVE.W", "(A2)+, CCR",                             0042332);
    TEST("MOVE.W", "-(A2), CCR",                             0042342);
    TEST("MOVE.W", "($1234,A2), CCR",                        0042352, 0x1234);
    TEST("MOVE.W", "(18,A2,D3.W), CCR",                      0042362, 0x3012);
    TEST("MOVE.W", "($001234).W, CCR",                       0042370, 0x1234);
    ERRT("MOVE.W", "($001235).W, CCR",  OPERAND_NOT_ALIGNED, 0042370, 0x1235);
    TEST("MOVE.W", "($234568).L, CCR",                       0042371, 0x0023, 0x4568);
    ERRT("MOVE.W", "($234569).L, CCR",  OPERAND_NOT_ALIGNED, 0042371, 0x0023, 0x4569);
    TEST("MOVE.W", "(*+$1234,PC), CCR",                      0042372, 0x1232);
    ERRT("MOVE.W", "(*+$1235,PC), CCR", OPERAND_NOT_ALIGNED, 0042372, 0x1233);
    TEST("MOVE.W", "(*-16,PC,D3.L), CCR",                    0042373, 0x38EE);
    TEST("MOVE.W", "#$34, CCR",                              0042374, 0x0034);

    // ORI #nn,CCR
    TEST("ORI", "#$34, CCR", 0000074, 0x0034);
}

static void test_multiproc() {
    // TAS dst: 00453|M|Rn
    TEST("TAS", "D2",           0045302);
    UNKN(                       0045312); // A2
    TEST("TAS", "(A2)",         0045322);
    TEST("TAS", "(A2)+",        0045332);
    TEST("TAS", "-(A2)",        0045342);
    TEST("TAS", "($1234,A2)",   0045352, 0x1234);
    TEST("TAS", "(18,A2,D3.W)", 0045362, 0x3012);
    TEST("TAS", "($001235).W",  0045370, 0x1235);
    TEST("TAS", "($123457).L",  0045371, 0x0012, 0x3457);
    UNKN(                       0045372); // (*+$1234,PC)
    UNKN(                       0045373); // (*+10,PC,D3.W)
    TEST("ILLEGAL", "",         0045374); // ILLEGAL
}
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
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

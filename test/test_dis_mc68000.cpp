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
    disassembler.setRelativeTarget(true);
}

static void tear_down() {
    symtab.reset();
}

// clang-format off
static void test_cpu() {
    EQUALS("cpu 68000", true,    disassembler.setCpu("68000"));
    EQUALS("cpu 68000", "68000", disassembler.getCpu());

    EQUALS("cpu MC68000", true,    disassembler.setCpu("MC68000"));
    EQUALS("cpu MC68000", "68000", disassembler.getCpu());
}

static void test_data_move() {
    // EXG Rx,Ry
    TEST(EXG, "D1,D2", 0141502); // 014|Dx|50|Dy
    TEST(EXG, "A1,A2", 0141512); // 014|Ax|51|Ay
    TEST(EXG, "D1,A2", 0141612); // 014|Dx|61|Ay

    // LEA ea,Ad: 004|An|7|M|Rn
    EROA(LEA, "D2,A1",             0041702);
    EROA(LEA, "A3,A1",             0041712);
    TEST(LEA, "(A2),A1",           0041722);
    EROA(LEA, "(A2)+,A1",          0041732);
    EROA(LEA, "-(A2),A1",          0041742);
    TEST(LEA, "($1234,A2),A1",     0041752, 0x1234);
    TEST(LEA, "(18,A2,D3.W),A1",   0041762, 0x3012);
    TEST(LEA, "($FFFF00).W,A1",    0041770, 0xFF00);
    TEST(LEA, "($001234).L,A1",    0041771, 0x0000, 0x1234);
    TEST(LEA, "(*+$1234,PC),A1",   0041772, 0x1232);
    TEST(LEA, "(*+18,PC,A3.L),A1", 0041773, 0xB810);
    EROA(LEA, "#$5678",            0041774, 0x5678);

    // LINK An,#nn: 004712|An
    TEST(LINK, "A3,#$1234", 0047123, 0x1234);
    TEST(LINK, "A3,#-16",   0047123, 0xFFF0);

    // MOVE src,dst: 00|Sz|Rd|Md|Ms|Rs, Sz:B=1/W=3/L=2
    TEST(MOVE_B, "D2,D7",              0017002);
    EROA(MOVE_B, "A2,D7",              0017012);
    TEST(MOVE_B, "(A2),D7",            0017022);
    TEST(MOVE_B, "(A2)+,D7",           0017032);
    TEST(MOVE_B, "-(A2),D7",           0017042);
    TEST(MOVE_B, "($1234,A2),D7",      0017052, 0x1234);
    TEST(MOVE_B, "(18,A2,D3.L),D7",    0017062, 0x3812);
    TEST(MOVE_B, "($FFFFFF).W,D7",     0017070, 0xFFFF);
    TEST(MOVE_B, "($123456).L,D7",     0017071, 0x0012, 0x3456);
    TEST(MOVE_B, "(*+$1234,PC),D7",    0017072, 0x1232);
    TEST(MOVE_B, "(*+18,PC,D3.W),D7",  0017073, 0x3010);
    TEST(MOVE_B, "#$34,D7",            0017074, 0x0034);
    TEST(MOVE_W, "D2,D7",              0037002);
    TEST(MOVE_W, "A2,D7",              0037012);
    TEST(MOVE_W, "(A2),D7",            0037022);
    TEST(MOVE_W, "(A2)+,D7",           0037032);
    TEST(MOVE_W, "-(A2),D7",           0037042);
    TEST(MOVE_W, "($1234,A2),D7",      0037052, 0x1234);
    TEST(MOVE_W, "(18,A2,D3.L),D7",    0037062, 0x3812);
    TEST(MOVE_W, "($FFFFFE).W,D7",     0037070, 0xFFFE);
    TEST(MOVE_W, "($123456).L,D7",     0037071, 0x0012, 0x3456);
    TEST(MOVE_W, "(*+$1234,PC),D7",    0037072, 0x1232);
    TEST(MOVE_W, "(*+18,PC,D3.W),D7",  0037073, 0x3010);
    TEST(MOVE_W, "#$0034,D7",          0037074, 0x0034);
    TEST(MOVE_L, "D2,D7",              0027002);
    TEST(MOVE_L, "A2,D7",              0027012);
    TEST(MOVE_L, "(A2),D7",            0027022);
    TEST(MOVE_L, "(A2)+,D7",           0027032);
    TEST(MOVE_L, "-(A2),D7",           0027042);
    TEST(MOVE_L, "($1234,A2),D7",      0027052, 0x1234);
    TEST(MOVE_L, "(18,A2,D3.L),D7",    0027062, 0x3812);
    TEST(MOVE_L, "($FFFFFC).W,D7",     0027070, 0xFFFC);
    TEST(MOVE_L, "($123454).L,D7",     0027071, 0x0012, 0x3454);
    TEST(MOVE_L, "(*+$1234,PC),D7",    0027072, 0x1232);
    TEST(MOVE_L, "(*+18,PC,D3.W),D7",  0027073, 0x3010);
    TEST(MOVE_L, "#$00345678,D7",      0027074, 0x0034, 0x5678);
    TEST(MOVE_B, "D2,(A6)",              0016202);
    EROA(MOVE_B, "A2,(A6)",              0016212);
    TEST(MOVE_B, "(A2),(A6)",            0016222);
    TEST(MOVE_B, "(A2)+,(A6)",           0016232);
    TEST(MOVE_B, "-(A2),(A6)",           0016242);
    TEST(MOVE_B, "($1234,A2),(A6)",      0016252, 0x1234);
    TEST(MOVE_B, "(18,A2,D3.L),(A6)",    0016262, 0x3812);
    TEST(MOVE_B, "($FFFFFF).W,(A6)",     0016270, 0xFFFF);
    TEST(MOVE_B, "($123456).L,(A6)",     0016271, 0x0012, 0x3456);
    TEST(MOVE_B, "(*+$1234,PC),(A6)",    0016272, 0x1232);
    TEST(MOVE_B, "(*+18,PC,D3.W),(A6)",  0016273, 0x3010);
    TEST(MOVE_B, "#$34,(A6)",            0016274, 0x0034);
    TEST(MOVE_W, "D2,(A6)",              0036202);
    TEST(MOVE_W, "A2,(A6)",              0036212);
    TEST(MOVE_W, "(A2),(A6)",            0036222);
    TEST(MOVE_W, "(A2)+,(A6)",           0036232);
    TEST(MOVE_W, "-(A2),(A6)",           0036242);
    TEST(MOVE_W, "($1234,A2),(A6)",      0036252, 0x1234);
    TEST(MOVE_W, "(18,A2,D3.L),(A6)",    0036262, 0x3812);
    TEST(MOVE_W, "($FFFFFE).W,(A6)",     0036270, 0xFFFE);
    TEST(MOVE_W, "($123456).L,(A6)",     0036271, 0x0012, 0x3456);
    TEST(MOVE_W, "(*+$1234,PC),(A6)",    0036272, 0x1232);
    TEST(MOVE_W, "(*+18,PC,D3.W),(A6)",  0036273, 0x3010);
    TEST(MOVE_W, "#$0034,(A6)",          0036274, 0x0034);
    TEST(MOVE_L, "D2,(A6)",              0026202);
    TEST(MOVE_L, "A2,(A6)",              0026212);
    TEST(MOVE_L, "(A2),(A6)",            0026222);
    TEST(MOVE_L, "(A2)+,(A6)",           0026232);
    TEST(MOVE_L, "-(A2),(A6)",           0026242);
    TEST(MOVE_L, "($1234,A2),(A6)",      0026252, 0x1234);
    TEST(MOVE_L, "(18,A2,D3.L),(A6)",    0026262, 0x3812);
    TEST(MOVE_L, "($FFFFFC).W,(A6)",     0026270, 0xFFFC);
    TEST(MOVE_L, "($123454).L,(A6)",     0026271, 0x0012, 0x3454);
    TEST(MOVE_L, "(*+$1234,PC),(A6)",    0026272, 0x1232);
    TEST(MOVE_L, "(*+18,PC,D3.W),(A6)",  0026273, 0x3010);
    TEST(MOVE_L, "#$00345678,(A6)",      0026274, 0x0034, 0x5678);
    TEST(MOVE_B, "D2,(A6)+",              0016302);
    EROA(MOVE_B, "A2,(A6)+",              0016312);
    TEST(MOVE_B, "(A2),(A6)+",            0016322);
    TEST(MOVE_B, "(A2)+,(A6)+",           0016332);
    TEST(MOVE_B, "-(A2),(A6)+",           0016342);
    TEST(MOVE_B, "($1234,A2),(A6)+",      0016352, 0x1234);
    TEST(MOVE_B, "(18,A2,D3.L),(A6)+",    0016362, 0x3812);
    TEST(MOVE_B, "($FFFFFF).W,(A6)+",     0016370, 0xFFFF);
    TEST(MOVE_B, "($123456).L,(A6)+",     0016371, 0x0012, 0x3456);
    TEST(MOVE_B, "(*+$1234,PC),(A6)+",    0016372, 0x1232);
    TEST(MOVE_B, "(*+18,PC,D3.W),(A6)+",  0016373, 0x3010);
    TEST(MOVE_B, "#$34,(A6)+",            0016374, 0x0034);
    TEST(MOVE_W, "D2,(A6)+",              0036302);
    TEST(MOVE_W, "A2,(A6)+",              0036312);
    TEST(MOVE_W, "(A2),(A6)+",            0036322);
    TEST(MOVE_W, "(A2)+,(A6)+",           0036332);
    TEST(MOVE_W, "-(A2),(A6)+",           0036342);
    TEST(MOVE_W, "($1234,A2),(A6)+",      0036352, 0x1234);
    TEST(MOVE_W, "(18,A2,D3.L),(A6)+",    0036362, 0x3812);
    TEST(MOVE_W, "($FFFFFE).W,(A6)+",     0036370, 0xFFFE);
    TEST(MOVE_W, "($123456).L,(A6)+",     0036371, 0x0012, 0x3456);
    TEST(MOVE_W, "(*+$1234,PC),(A6)+",    0036372, 0x1232);
    TEST(MOVE_W, "(*+18,PC,D3.W),(A6)+",  0036373, 0x3010);
    TEST(MOVE_W, "#$0034,(A6)+",          0036374, 0x0034);
    TEST(MOVE_L, "D2,(A6)+",              0026302);
    TEST(MOVE_L, "A2,(A6)+",              0026312);
    TEST(MOVE_L, "(A2),(A6)+",            0026322);
    TEST(MOVE_L, "(A2)+,(A6)+",           0026332);
    TEST(MOVE_L, "-(A2),(A6)+",           0026342);
    TEST(MOVE_L, "($1234,A2),(A6)+",      0026352, 0x1234);
    TEST(MOVE_L, "(18,A2,D3.L),(A6)+",    0026362, 0x3812);
    TEST(MOVE_L, "($FFFFFC).W,(A6)+",     0026370, 0xFFFC);
    TEST(MOVE_L, "($123454).L,(A6)+",     0026371, 0x0012, 0x3454);
    TEST(MOVE_L, "(*+$1234,PC),(A6)+",    0026372, 0x1232);
    TEST(MOVE_L, "(*+18,PC,D3.W),(A6)+",  0026373, 0x3010);
    TEST(MOVE_L, "#$00345678,(A6)+",      0026374, 0x0034, 0x5678);
    TEST(MOVE_B, "D2,-(A6)",              0016402);
    EROA(MOVE_B, "A2,-(A6)",              0016412);
    TEST(MOVE_B, "(A2),-(A6)",            0016422);
    TEST(MOVE_B, "(A2)+,-(A6)",           0016432);
    TEST(MOVE_B, "-(A2),-(A6)",           0016442);
    TEST(MOVE_B, "($1234,A2),-(A6)",      0016452, 0x1234);
    TEST(MOVE_B, "(18,A2,D3.L),-(A6)",    0016462, 0x3812);
    TEST(MOVE_B, "($FFFFFF).W,-(A6)",     0016470, 0xFFFF);
    TEST(MOVE_B, "($123456).L,-(A6)",     0016471, 0x0012, 0x3456);
    TEST(MOVE_B, "(*+$1234,PC),-(A6)",    0016472, 0x1232);
    TEST(MOVE_B, "(*+18,PC,D3.W),-(A6)",  0016473, 0x3010);
    TEST(MOVE_B, "#$34,-(A6)",            0016474, 0x0034);
    TEST(MOVE_W, "D2,-(A6)",              0036402);
    TEST(MOVE_W, "A2,-(A6)",              0036412);
    TEST(MOVE_W, "(A2),-(A6)",            0036422);
    TEST(MOVE_W, "(A2)+,-(A6)",           0036432);
    TEST(MOVE_W, "-(A2),-(A6)",           0036442);
    TEST(MOVE_W, "($1234,A2),-(A6)",      0036452, 0x1234);
    TEST(MOVE_W, "(18,A2,D3.L),-(A6)",    0036462, 0x3812);
    TEST(MOVE_W, "($FFFFFE).W,-(A6)",     0036470, 0xFFFE);
    TEST(MOVE_W, "($123456).L,-(A6)",     0036471, 0x0012, 0x3456);
    TEST(MOVE_W, "(*+$1234,PC),-(A6)",    0036472, 0x1232);
    TEST(MOVE_W, "(*+18,PC,D3.W),-(A6)",  0036473, 0x3010);
    TEST(MOVE_W, "#$0034,-(A6)",          0036474, 0x0034);
    TEST(MOVE_L, "D2,-(A6)",              0026402);
    TEST(MOVE_L, "A2,-(A6)",              0026412);
    TEST(MOVE_L, "(A2),-(A6)",            0026422);
    TEST(MOVE_L, "(A2)+,-(A6)",           0026432);
    TEST(MOVE_L, "-(A2),-(A6)",           0026442);
    TEST(MOVE_L, "($1234,A2),-(A6)",      0026452, 0x1234);
    TEST(MOVE_L, "(18,A2,D3.L),-(A6)",    0026462, 0x3812);
    TEST(MOVE_L, "($FFFFFC).W,-(A6)",     0026470, 0xFFFC);
    TEST(MOVE_L, "($123454).L,-(A6)",     0026471, 0x0012, 0x3454);
    TEST(MOVE_L, "(*+$1234,PC),-(A6)",    0026472, 0x1232);
    TEST(MOVE_L, "(*+18,PC,D3.W),-(A6)",  0026473, 0x3010);
    TEST(MOVE_L, "#$00345678,-(A6)",      0026474, 0x0034, 0x5678);
    TEST(MOVE_B, "D2,($5678,A6)",              0016502, 0x5678);
    EROA(MOVE_B, "A2,($5678,A6)",              0016512, 0x5678);
    TEST(MOVE_B, "(A2),($5678,A6)",            0016522, 0x5678);
    TEST(MOVE_B, "(A2)+,($5678,A6)",           0016532, 0x5678);
    TEST(MOVE_B, "-(A2),($5678,A6)",           0016542, 0x5678);
    TEST(MOVE_B, "($1234,A2),($5678,A6)",      0016552, 0x1234, 0x5678);
    TEST(MOVE_B, "(18,A2,D3.L),($5678,A6)",    0016562, 0x3812, 0x5678);
    TEST(MOVE_B, "($FFFFFF).W,($5678,A6)",     0016570, 0xFFFF, 0x5678);
    TEST(MOVE_B, "($123456).L,($5678,A6)",     0016571, 0x0012, 0x3456, 0x5678);
    TEST(MOVE_B, "(*+$1234,PC),($5678,A6)",    0016572, 0x1232, 0x5678);
    TEST(MOVE_B, "(*+18,PC,D3.W),($5678,A6)",  0016573, 0x3010, 0x5678);
    TEST(MOVE_B, "#$34,($5678,A6)",            0016574, 0x0034, 0x5678);
    TEST(MOVE_W, "D2,($5678,A6)",              0036502, 0x5678);
    TEST(MOVE_W, "A2,($5678,A6)",              0036512, 0x5678);
    TEST(MOVE_W, "(A2),($5678,A6)",            0036522, 0x5678);
    TEST(MOVE_W, "(A2)+,($5678,A6)",           0036532, 0x5678);
    TEST(MOVE_W, "-(A2),($5678,A6)",           0036542, 0x5678);
    TEST(MOVE_W, "($1234,A2),($5678,A6)",      0036552, 0x1234, 0x5678);
    TEST(MOVE_W, "(18,A2,D3.L),($5678,A6)",    0036562, 0x3812, 0x5678);
    TEST(MOVE_W, "($FFFFFE).W,($5678,A6)",     0036570, 0xFFFE, 0x5678);
    TEST(MOVE_W, "($123456).L,($5678,A6)",     0036571, 0x0012, 0x3456, 0x5678);
    TEST(MOVE_W, "(*+$1234,PC),($5678,A6)",    0036572, 0x1232, 0x5678);
    TEST(MOVE_W, "(*+18,PC,D3.W),($5678,A6)",  0036573, 0x3010, 0x5678);
    TEST(MOVE_W, "#$0034,($5678,A6)",          0036574, 0x0034, 0x5678);
    TEST(MOVE_L, "D2,($5678,A6)",              0026502, 0x5678);
    TEST(MOVE_L, "A2,($5678,A6)",              0026512, 0x5678);
    TEST(MOVE_L, "(A2),($5678,A6)",            0026522, 0x5678);
    TEST(MOVE_L, "(A2)+,($5678,A6)",           0026532, 0x5678);
    TEST(MOVE_L, "-(A2),($5678,A6)",           0026542, 0x5678);
    TEST(MOVE_L, "($1234,A2),($5678,A6)",      0026552, 0x1234, 0x5678);
    TEST(MOVE_L, "(18,A2,D3.L),($5678,A6)",    0026562, 0x3812, 0x5678);
    TEST(MOVE_L, "($FFFFFC).W,($5678,A6)",     0026570, 0xFFFC, 0x5678);
    TEST(MOVE_L, "($123454).L,($5678,A6)",     0026571, 0x0012, 0x3454, 0x5678);
    TEST(MOVE_L, "(*+$1234,PC),($5678,A6)",    0026572, 0x1232, 0x5678);
    TEST(MOVE_L, "(*+18,PC,D3.W),($5678,A6)",  0026573, 0x3010, 0x5678);
    TEST(MOVE_L, "#$00345678,($5678,A6)",      0026574, 0x0034, 0x5678, 0x5678);
    TEST(MOVE_B, "D2,($56,A6,D5.W)",              0016602, 0x5056);
    EROA(MOVE_B, "A2,($56,A6,D5.W)",              0016612, 0x5056);
    TEST(MOVE_B, "(A2),($56,A6,D5.W)",            0016622, 0x5056);
    TEST(MOVE_B, "(A2)+,($56,A6,D5.W)",           0016632, 0x5056);
    TEST(MOVE_B, "-(A2),($56,A6,D5.W)",           0016642, 0x5056);
    TEST(MOVE_B, "($1234,A2),($56,A6,D5.W)",      0016652, 0x1234, 0x5056);
    TEST(MOVE_B, "(18,A2,D3.L),($56,A6,D5.W)",    0016662, 0x3812, 0x5056);
    TEST(MOVE_B, "($FFFFFF).W,($56,A6,D5.W)",     0016670, 0xFFFF, 0x5056);
    TEST(MOVE_B, "($123456).L,($56,A6,D5.W)",     0016671, 0x0012, 0x3456, 0x5056);
    TEST(MOVE_B, "(*+$1234,PC),($56,A6,D5.W)",    0016672, 0x1232, 0x5056);
    TEST(MOVE_B, "(*+18,PC,D3.W),($56,A6,D5.W)",  0016673, 0x3010, 0x5056);
    TEST(MOVE_B, "#$34,($56,A6,D5.W)",            0016674, 0x0034, 0x5056);
    TEST(MOVE_W, "D2,($56,A6,D5.W)",              0036602, 0x5056);
    TEST(MOVE_W, "A2,($56,A6,D5.W)",              0036612, 0x5056);
    TEST(MOVE_W, "(A2),($56,A6,D5.W)",            0036622, 0x5056);
    TEST(MOVE_W, "(A2)+,($56,A6,D5.W)",           0036632, 0x5056);
    TEST(MOVE_W, "-(A2),($56,A6,D5.W)",           0036642, 0x5056);
    TEST(MOVE_W, "($1234,A2),($56,A6,D5.W)",      0036652, 0x1234, 0x5056);
    TEST(MOVE_W, "(18,A2,D3.L),($56,A6,D5.W)",    0036662, 0x3812, 0x5056);
    TEST(MOVE_W, "($FFFFFE).W,($56,A6,D5.W)",     0036670, 0xFFFE, 0x5056);
    TEST(MOVE_W, "($123456).L,($56,A6,D5.W)",     0036671, 0x0012, 0x3456, 0x5056);
    TEST(MOVE_W, "(*+$1234,PC),($56,A6,D5.W)",    0036672, 0x1232, 0x5056);
    TEST(MOVE_W, "(*+18,PC,D3.W),($56,A6,D5.W)",  0036673, 0x3010, 0x5056);
    TEST(MOVE_W, "#$0034,($56,A6,D5.W)",          0036674, 0x0034, 0x5056);
    TEST(MOVE_L, "D2,($56,A6,D5.W)",              0026602, 0x5056);
    TEST(MOVE_L, "A2,($56,A6,D5.W)",              0026612, 0x5056);
    TEST(MOVE_L, "(A2),($56,A6,D5.W)",            0026622, 0x5056);
    TEST(MOVE_L, "(A2)+,($56,A6,D5.W)",           0026632, 0x5056);
    TEST(MOVE_L, "-(A2),($56,A6,D5.W)",           0026642, 0x5056);
    TEST(MOVE_L, "($1234,A2),($56,A6,D5.W)",      0026652, 0x1234, 0x5056);
    TEST(MOVE_L, "(18,A2,D3.L),($56,A6,D5.W)",    0026662, 0x3812, 0x5056);
    TEST(MOVE_L, "($FFFFFC).W,($56,A6,D5.W)",     0026670, 0xFFFC, 0x5056);
    TEST(MOVE_L, "($123454).L,($56,A6,D5.W)",     0026671, 0x0012, 0x3454, 0x5056);
    TEST(MOVE_L, "(*+$1234,PC),($56,A6,D5.W)",    0026672, 0x1232, 0x5056);
    TEST(MOVE_L, "(*+18,PC,D3.W),($56,A6,D5.W)",  0026673, 0x3010, 0x5056);
    TEST(MOVE_L, "#$00345678,($56,A6,D5.W)",      0026674, 0x0034, 0x5678, 0x5056);
    TEST(MOVE_B, "D2,($005678).W",              0010702, 0x5678);
    EROA(MOVE_B, "A2,($005678).W",              0010712, 0x5678);
    TEST(MOVE_B, "(A2),($005678).W",            0010722, 0x5678);
    TEST(MOVE_B, "(A2)+,($005678).W",           0010732, 0x5678);
    TEST(MOVE_B, "-(A2),($005678).W",           0010742, 0x5678);
    TEST(MOVE_B, "($1234,A2),($005678).W",      0010752, 0x1234, 0x5678);
    TEST(MOVE_B, "(18,A2,D3.L),($005678).W",    0010762, 0x3812, 0x5678);
    TEST(MOVE_B, "($FFFFFF).W,($005678).W",     0010770, 0xFFFF, 0x5678);
    TEST(MOVE_B, "($123456).L,($005678).W",     0010771, 0x0012, 0x3456, 0x5678);
    TEST(MOVE_B, "(*+$1234,PC),($005678).W",    0010772, 0x1232, 0x5678);
    TEST(MOVE_B, "(*+18,PC,D3.W),($005678).W",  0010773, 0x3010, 0x5678);
    TEST(MOVE_B, "#$34,($005678).W",            0010774, 0x0034, 0x5678);
    TEST(MOVE_W, "D2,($005678).W",              0030702, 0x5678);
    TEST(MOVE_W, "A2,($005678).W",              0030712, 0x5678);
    TEST(MOVE_W, "(A2),($005678).W",            0030722, 0x5678);
    TEST(MOVE_W, "(A2)+,($005678).W",           0030732, 0x5678);
    TEST(MOVE_W, "-(A2),($005678).W",           0030742, 0x5678);
    TEST(MOVE_W, "($1234,A2),($005678).W",      0030752, 0x1234, 0x5678);
    TEST(MOVE_W, "(18,A2,D3.L),($005678).W",    0030762, 0x3812, 0x5678);
    TEST(MOVE_W, "($FFFFFE).W,($005678).W",     0030770, 0xFFFE, 0x5678);
    TEST(MOVE_W, "($123456).L,($005678).W",     0030771, 0x0012, 0x3456, 0x5678);
    TEST(MOVE_W, "(*+$1234,PC),($005678).W",    0030772, 0x1232, 0x5678);
    TEST(MOVE_W, "(*+18,PC,D3.W),($005678).W",  0030773, 0x3010, 0x5678);
    TEST(MOVE_W, "#$0034,($005678).W",          0030774, 0x0034, 0x5678);
    TEST(MOVE_L, "D2,($005678).W",              0020702, 0x5678);
    TEST(MOVE_L, "A2,($005678).W",              0020712, 0x5678);
    TEST(MOVE_L, "(A2),($005678).W",            0020722, 0x5678);
    TEST(MOVE_L, "(A2)+,($005678).W",           0020732, 0x5678);
    TEST(MOVE_L, "-(A2),($005678).W",           0020742, 0x5678);
    TEST(MOVE_L, "($1234,A2),($005678).W",      0020752, 0x1234, 0x5678);
    TEST(MOVE_L, "(18,A2,D3.L),($005678).W",    0020762, 0x3812, 0x5678);
    TEST(MOVE_L, "($FFFFFC).W,($005678).W",     0020770, 0xFFFC, 0x5678);
    TEST(MOVE_L, "($123454).L,($005678).W",     0020771, 0x0012, 0x3454, 0x5678);
    TEST(MOVE_L, "(*+$1234,PC),($005678).W",    0020772, 0x1232, 0x5678);
    TEST(MOVE_L, "(*+18,PC,D3.W),($005678).W",  0020773, 0x3010, 0x5678);
    TEST(MOVE_L, "#$00345678,($005678).W",      0020774, 0x0034, 0x5678, 0x5678);
    TEST(MOVE_B, "D2,($56789A).L",              0011702, 0x0056, 0x789A);
    EROA(MOVE_B, "A2,($56789A).L",              0011712, 0x0056, 0x789A);
    TEST(MOVE_B, "(A2),($56789A).L",            0011722, 0x0056, 0x789A);
    TEST(MOVE_B, "(A2)+,($56789A).L",           0011732, 0x0056, 0x789A);
    TEST(MOVE_B, "-(A2),($56789A).L",           0011742, 0x0056, 0x789A);
    TEST(MOVE_B, "($1234,A2),($56789A).L",      0011752, 0x1234, 0x0056, 0x789A);
    TEST(MOVE_B, "(18,A2,D3.L),($56789A).L",    0011762, 0x3812, 0x0056, 0x789A);
    TEST(MOVE_B, "($FFFFFF).W,($56789A).L",     0011770, 0xFFFF, 0x0056, 0x789A);
    TEST(MOVE_B, "($123456).L,($56789A).L",     0011771, 0x0012, 0x3456, 0x0056, 0x789A);
    TEST(MOVE_B, "(*+$1234,PC),($56789A).L",    0011772, 0x1232, 0x0056, 0x789A);
    TEST(MOVE_B, "(*+18,PC,D3.W),($56789A).L",  0011773, 0x3010, 0x0056, 0x789A);
    TEST(MOVE_B, "#$34,($56789A).L",            0011774, 0x0034, 0x0056, 0x789A);
    TEST(MOVE_W, "D2,($56789A).L",              0031702, 0x0056, 0x789A);
    TEST(MOVE_W, "A2,($56789A).L",              0031712, 0x0056, 0x789A);
    TEST(MOVE_W, "(A2),($56789A).L",            0031722, 0x0056, 0x789A);
    TEST(MOVE_W, "(A2)+,($56789A).L",           0031732, 0x0056, 0x789A);
    TEST(MOVE_W, "-(A2),($56789A).L",           0031742, 0x0056, 0x789A);
    TEST(MOVE_W, "($1234,A2),($56789A).L",      0031752, 0x1234, 0x0056, 0x789A);
    TEST(MOVE_W, "(18,A2,D3.L),($56789A).L",    0031762, 0x3812, 0x0056, 0x789A);
    TEST(MOVE_W, "($FFFFFE).W,($56789A).L",     0031770, 0xFFFE, 0x0056, 0x789A);
    TEST(MOVE_W, "($123456).L,($56789A).L",     0031771, 0x0012, 0x3456, 0x0056, 0x789A);
    TEST(MOVE_W, "(*+$1234,PC),($56789A).L",    0031772, 0x1232, 0x0056, 0x789A);
    TEST(MOVE_W, "(*+18,PC,D3.W),($56789A).L",  0031773, 0x3010, 0x0056, 0x789A);
    TEST(MOVE_W, "#$0034,($56789A).L",          0031774, 0x0034, 0x0056, 0x789A);
    TEST(MOVE_L, "D2,($567898).L",              0021702, 0x0056, 0x7898);
    TEST(MOVE_L, "A2,($567898).L",              0021712, 0x0056, 0x7898);
    TEST(MOVE_L, "(A2),($567898).L",            0021722, 0x0056, 0x7898);
    TEST(MOVE_L, "(A2)+,($567898).L",           0021732, 0x0056, 0x7898);
    TEST(MOVE_L, "-(A2),($567898).L",           0021742, 0x0056, 0x7898);
    TEST(MOVE_L, "($1234,A2),($567898).L",      0021752, 0x1234, 0x0056, 0x7898);
    TEST(MOVE_L, "(18,A2,D3.L),($567898).L",    0021762, 0x3812, 0x0056, 0x7898);
    TEST(MOVE_L, "($FFFFFC).W,($567898).L",     0021770, 0xFFFC, 0x0056, 0x7898);
    TEST(MOVE_L, "($123454).L,($567898).L",     0021771, 0x0012, 0x3454, 0x0056, 0x7898);
    TEST(MOVE_L, "(*+$1234,PC),($567898).L",    0021772, 0x1232, 0x0056, 0x7898);
    TEST(MOVE_L, "(*+18,PC,D3.W),($567898).L",  0021773, 0x3010, 0x0056, 0x7898);
    TEST(MOVE_L, "#$00345678,($567898).L",      0021774, 0x0034, 0x5678, 0x0056, 0x7898);
    EROA(MOVE_B, "D2,(d16,PC)",                 0012702);
    EROA(MOVE_W, "D2,(d8,PC,Xn)",               0033702);
    EROA(MOVE_L, "D2,#nnnn",                    0024702);

    // MOVEA src,An: 00|Sz|An|1|M|Rn, Sz:W=3/L=2
    TEST(MOVEA_W, "D2,A6",              0036102);
    TEST(MOVEA_W, "A2,A6",              0036112);
    TEST(MOVEA_W, "(A2),A6",            0036122);
    TEST(MOVEA_W, "(A2)+,A6",           0036132);
    TEST(MOVEA_W, "-(A2),A6",           0036142);
    TEST(MOVEA_W, "($1234,A2),A6",      0036152, 0x1234);
    TEST(MOVEA_W, "(18,A2,D3.L),A6",    0036162, 0x3812);
    TEST(MOVEA_W, "($FFFFFE).W,A6",     0036170, 0xFFFE);
    TEST(MOVEA_W, "($123456).L,A6",     0036171, 0x0012, 0x3456);
    TEST(MOVEA_W, "(*+$1234,PC),A6",    0036172, 0x1232);
    TEST(MOVEA_W, "(*+18,PC,D3.W),A6",  0036173, 0x3010);
    TEST(MOVEA_W, "#$0034,A6",          0036174, 0x0034);
    TEST(MOVEA_L, "D2,A6",              0026102);
    TEST(MOVEA_L, "A2,A6",              0026112);
    TEST(MOVEA_L, "(A2),A6",            0026122);
    TEST(MOVEA_L, "(A2)+,A6",           0026132);
    TEST(MOVEA_L, "-(A2),A6",           0026142);
    TEST(MOVEA_L, "($1234,A2),A6",      0026152, 0x1234);
    TEST(MOVEA_L, "(18,A2,D3.L),A6",    0026162, 0x3812);
    TEST(MOVEA_L, "($FFFFFC).W,A6",     0026170, 0xFFFC);
    TEST(MOVEA_L, "($123454).L,A6",     0026171, 0x0012, 0x3454);
    TEST(MOVEA_L, "(*+$1234,PC),A6",    0026172, 0x1232);
    TEST(MOVEA_L, "(*+18,PC,D3.W),A6",  0026173, 0x3010);
    TEST(MOVEA_L, "#$00345678,A6",      0026174, 0x0034, 0x5678);

    // MOVEM list,dst: 0044|Sz|Md|Rd, Sz:W=2/L=3, list=A7|...|D0, -(An)=D0|...|A7
    TEST(EXT_W,   "D2",                  0044202); // EXT.W
    EROA(MOVEM_W, "D0-D6,A2",            0044212, 0x007F);
    TEST(MOVEM_W, "D0-D6,(A2)",          0044222, 0x007F);
    EROA(MOVEM_W, "D0-D6,(A2)+",         0044232, 0x007F);
    TEST(MOVEM_W, "D0-D6,-(A2)",         0044242, 0xFE00);
    TEST(MOVEM_W, "D0-D6,($1234,A2)",    0044252, 0x007F, 0x1234);
    TEST(MOVEM_W, "D0-D6,(18,A2,A3.L)",  0044262, 0x007F, 0xB812);
    TEST(MOVEM_W, "D0-D6,($FFFEDC).W",   0044270, 0x007F, 0xFEDC);
    TEST(MOVEM_W, "D0-D6,($123456).L",   0044271, 0x007F, 0x0012, 0x3456);
    EROA(MOVEM_W, "D0-D6,(d16,PC)",      0044272);
    EROA(MOVEM_W, "D0-D6,(d8,PC,Xn)",    0044273);
    EROA(MOVEM_W, "D0-D6,#nnnn",         0044274);
    TEST(MOVEM_W, "D0,(A2)",             0044222, 0x0001);
    TEST(MOVEM_W, "A0,(A2)",             0044222, 0x0100);
    TEST(MOVEM_W, "D0,-(A2)",            0044242, 0x8000);
    TEST(MOVEM_W, "A0,-(A2)",            0044242, 0x0080);
    TEST(EXT_L,   "D2",                  0044302); // EXT.L
    EROA(MOVEM_L, "D1/A0,A2",            0044312, 0x0102);
    TEST(MOVEM_L, "D1/A0,(A2)",          0044322, 0x0102);
    EROA(MOVEM_L, "D1/A0,(A2)+",         0044332, 0x0102);
    TEST(MOVEM_L, "D1/A0,-(A2)",         0044342, 0x4080);
    TEST(MOVEM_L, "D1/A0,($1234,A2)",    0044352, 0x0102, 0x1234);
    TEST(MOVEM_L, "D1/A0,(18,A2,A3.L)",  0044362, 0x0102, 0xB812);
    TEST(MOVEM_L, "D1/A0,($FFFEDC).W",   0044370, 0x0102, 0xFEDC);
    TEST(MOVEM_L, "D1/A0,($123454).L",   0044371, 0x0102, 0x0012, 0x3454);
    EROA(MOVEM_L, "D1/A0,(d16,PC)",      0044372);
    EROA(MOVEM_L, "D1/A0,(d8,PC,Xn)",    0044373);
    EROA(MOVEM_L, "D1/A0,#nnnn",         0044374);
    TEST(MOVEM_L, "D0,(A2)",             0044322, 0x0001);
    TEST(MOVEM_L, "A0,(A2)",             0044322, 0x0100);
    TEST(MOVEM_L, "D0,-(A2)",            0044342, 0x8000);
    TEST(MOVEM_L, "A0,-(A2)",            0044342, 0x0080);

    // MOVEM src,list: 0046|Sz|Md|Rd, Sz:W=2/L=3, list=A7|...|D0
    EROA(MOVEM_W, "D2,A3-A6",              0046202, 0x7800);
    EROA(MOVEM_W, "A2,A3-A6",              0046212, 0x7800);
    TEST(MOVEM_W, "(A2),A3-A6",            0046222, 0x7800);
    TEST(MOVEM_W, "(A2)+,A3-A6",           0046232, 0x7800);
    EROA(MOVEM_W, "-(A2),A3-A6",           0046242, 0x7800);
    TEST(MOVEM_W, "($1234,A2),A3-A6",      0046252, 0x7800, 0x1234);
    TEST(MOVEM_W, "(18,A2,D3.W),A3-A6",    0046262, 0x7800, 0x3012);
    TEST(MOVEM_W, "($FFFEDC).W,A3-A6",     0046270, 0x7800, 0xFEDC);
    TEST(MOVEM_W, "($123456).L,A3-A6",     0046271, 0x7800, 0x0012, 0x3456);
    TEST(MOVEM_W, "(*+$1234,PC),A3-A6",    0046272, 0x7800, 0x1230);
    TEST(MOVEM_W, "(*+18,PC,D3.L),A3-A6",  0046273, 0x7800, 0x380E);
    EROA(MOVEM_W, "#nnnn,A3-A6",           0046274);
    TEST(MOVEM_W, "(A2),D0",               0046222, 0x0001);
    TEST(MOVEM_W, "(A2),A0",               0046222, 0x0100);
    TEST(MOVEM_W, "(A2)+,D0",              0046232, 0x0001);
    TEST(MOVEM_W, "(A2)+,A0",              0046232, 0x0100);
    EROA(MOVEM_L, "D2,A3-A6",              0046302, 0x7800);
    EROA(MOVEM_L, "A2,A3-A6",              0046312, 0x7800);
    TEST(MOVEM_L, "(A2),A3-A6",            0046322, 0x7800);
    TEST(MOVEM_L, "(A2)+,A3-A6",           0046332, 0x7800);
    EROA(MOVEM_L, "-(A2),A3-A6",           0046342, 0x7800);
    TEST(MOVEM_L, "($1234,A2),A3-A6",      0046352, 0x7800, 0x1234);
    TEST(MOVEM_L, "(18,A2,D3.W),A3-A6",    0046362, 0x7800, 0x3012);
    TEST(MOVEM_L, "($FFFEDC).W,A3-A6",     0046370, 0x7800, 0xFEDC);
    TEST(MOVEM_L, "($123454).L,A3-A6",     0046371, 0x7800, 0x0012, 0x3454);
    TEST(MOVEM_L, "(*+$1234,PC),A3-A6",    0046372, 0x7800, 0x1230);
    TEST(MOVEM_L, "(*+18,PC,D3.L),A3-A6",  0046373, 0x7800, 0x380E);
    EROA(MOVEM_L, "#nnnn,A3-A6",           0046374);
    TEST(MOVEM_L, "(A2),D0",               0046322, 0x0001);
    TEST(MOVEM_L, "(A2),A0",               0046322, 0x0100);
    TEST(MOVEM_L, "(A2)+,D0",              0046332, 0x0001);
    TEST(MOVEM_L, "(A2)+,A0",              0046332, 0x0100);

    // MOVEP Dn,(d16,An): 000|Dn|Sz|1|An, SZ:W=6/L=7
    TEST(MOVEP_W, "D7,($1234,A2)", 0007612, 0x1234);
    TEST(MOVEP_L, "D7,($1234,A2)", 0007712, 0x1234);

    // MOVEP (d16,An),Dn: 000|Dn|Sz|1|An, SZ:W=4/L=5
    TEST(MOVEP_W, "($1234,A2),D7", 0007412, 0x1234);
    TEST(MOVEP_L, "($1234,A2),D7", 0007512, 0x1234);

    // MOVEQ #nn,Dn: 007|Dn|000 + nn
    TEST(MOVEQ, "#0,D0",    0070000 | 0x00);
    TEST(MOVEQ, "#$7F,D0",  0070000 | 0x7F);
    TEST(MOVEQ, "#-$80,D0", 0070000 | 0x80);
    TEST(MOVEQ, "#-1,D7",   0077000 | 0xFF);

    // PEA src: 00441|M|Rn
    TEST(SWAP, "D2",            0044102); // SWAP
    EROA(PEA, "A2",             0044112);
    TEST(PEA, "(A2)",           0044122);
    EROA(PEA, "(A2)+",          0044132);
    EROA(PEA, "-(A2)",          0044142);
    TEST(PEA, "($1234,A2)",     0044152, 0x1234);
    TEST(PEA, "(18,A2,D3.W)",   0044162, 0x3012);
    TEST(PEA, "($FFFF00).W",    0044170, 0xFF00);
    TEST(PEA, "($001234).L",    0044171, 0x0000, 0x1234);
    TEST(PEA, "(*+$1234,PC)",   0044172, 0x1232);
    TEST(PEA, "(*+18,PC,A3.L)", 0044173, 0xB810);
    EROA(PEA, "#$5678",         0044174, 0x5678);

    // UNLK An: 004713|An
    TEST(UNLK, "A0", 0047130);
    TEST(UNLK, "A7", 0047137);
}

static void test_integer() {
    // ADD Dn,dst: 015|Dn|Sz|M|Rn, Sz:B=4/W=5/L=6
    TEST(ADDX_B, "D2,D7",          0157402); // ADDX.B
    TEST(ADDX_B, "-(A2),-(A7)",    0157412); // ADDX.B
    TEST(ADD_B, "D7,(A2)",         0157422);
    TEST(ADD_B, "D7,(A2)+",        0157432);
    TEST(ADD_B, "D7,-(A2)",        0157442);
    TEST(ADD_B, "D7,($1234,A2)",   0157452, 0x1234);
    TEST(ADD_B, "D7,(18,A2,D3.L)", 0157462, 0x3812);
    TEST(ADD_B, "D7,($FFFFFF).W",  0157470, 0xFFFF);
    TEST(ADD_B, "D7,($123456).L",  0157471, 0x0012, 0x3456);
    EROA(ADD_B, "D7,(d16,PC)",     0157472);
    EROA(ADD_B, "D7,(d8,PC,Xn)",   0157473);
    EROA(ADD_B, "D7,#nnnn",        0157474);
    TEST(ADDX_W, "D2,D7",          0157502); // ADDX.W
    TEST(ADDX_W, "-(A2),-(A7)",    0157512); // ADDX.L
    TEST(ADD_W, "D7,(A2)",         0157522);
    TEST(ADD_W, "D7,(A2)+",        0157532);
    TEST(ADD_W, "D7,-(A2)",        0157542);
    TEST(ADD_W, "D7,($1234,A2)",   0157552, 0x1234);
    TEST(ADD_W, "D7,(18,A2,D3.L)", 0157562, 0x3812);
    TEST(ADD_W, "D7,($FFFFFE).W",  0157570, 0xFFFE);
    TEST(ADD_W, "D7,($123456).L",  0157571, 0x0012, 0x3456);
    EROA(ADD_W, "D7,(d16,PC)",     0157572);
    EROA(ADD_W, "D7,(d8,PC,Xn)",   0157573);
    EROA(ADD_W, "D7,#nnnn",        0157574);
    TEST(ADDX_L, "D2,D7",          0157602); // ADDX.L
    TEST(ADDX_L, "-(A2),-(A7)",    0157612); // ADDX.L
    TEST(ADD_L, "D7,(A2)",         0157622);
    TEST(ADD_L, "D7,(A2)+",        0157632);
    TEST(ADD_L, "D7,-(A2)",        0157642);
    TEST(ADD_L, "D7,($1234,A2)",   0157652, 0x1234);
    TEST(ADD_L, "D7,(18,A2,D3.L)", 0157662, 0x3812);
    TEST(ADD_L, "D7,($FFFFFC).W",  0157670, 0xFFFC);
    TEST(ADD_L, "D7,($123454).L",  0157671, 0x0012, 0x3454);
    EROA(ADD_L, "D7,(d16,PC)",     0157672);
    EROA(ADD_L, "D7,(d8,PC,Xn)",   0157673);
    EROA(ADD_L, "D7,#nnnn",        0157674);

    // ADD src,Dn: 015|Dn|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST(ADD_B, "D2,D7",             0157002);
    EROA(ADD_B, "A2,D7",             0157012);
    TEST(ADD_B, "(A2),D7",           0157022);
    TEST(ADD_B, "(A2)+,D7",          0157032);
    TEST(ADD_B, "-(A2),D7",          0157042);
    TEST(ADD_B, "($1234,A2),D7",     0157052, 0x1234);
    TEST(ADD_B, "(18,A2,D3.L),D7",   0157062, 0x3812);
    TEST(ADD_B, "($FFFFFF).W,D7",    0157070, 0xFFFF);
    TEST(ADD_B, "($123456).L,D7",    0157071, 0x0012, 0x3456);
    TEST(ADD_B, "(*+$1234,PC),D7",   0157072, 0x1232);
    TEST(ADD_B, "(*+18,PC,D3.W),D7", 0157073, 0x3010);
    TEST(ADD_B, "#$34,D7",           0157074, 0x0034);
    TEST(ADD_W, "D2,D7",             0157102);
    TEST(ADD_W, "A2,D7",             0157112);
    TEST(ADD_W, "(A2),D7",           0157122);
    TEST(ADD_W, "(A2)+,D7",          0157132);
    TEST(ADD_W, "-(A2),D7",          0157142);
    TEST(ADD_W, "($1234,A2),D7",     0157152, 0x1234);
    TEST(ADD_W, "(18,A2,D3.L),D7",   0157162, 0x3812);
    TEST(ADD_W, "($FFFFFE).W,D7",    0157170, 0xFFFE);
    TEST(ADD_W, "($123456).L,D7",    0157171, 0x0012, 0x3456);
    TEST(ADD_W, "(*+$1234,PC),D7",   0157172, 0x1232);
    TEST(ADD_W, "(*+18,PC,D3.W),D7", 0157173, 0x3010);
    TEST(ADD_W, "#$0034,D7",         0157174, 0x0034);
    TEST(ADD_L, "D2,D7",             0157202);
    TEST(ADD_L, "A2,D7",             0157212);
    TEST(ADD_L, "(A2),D7",           0157222);
    TEST(ADD_L, "(A2)+,D7",          0157232);
    TEST(ADD_L, "-(A2),D7",          0157242);
    TEST(ADD_L, "($1234,A2),D7",     0157252, 0x1234);
    TEST(ADD_L, "(18,A2,D3.L),D7",   0157262, 0x3812);
    TEST(ADD_L, "($FFFFFC).W,D7",    0157270, 0xFFFC);
    TEST(ADD_L, "($123454).L,D7",    0157271, 0x0012, 0x3454);
    TEST(ADD_L, "(*+$1234,PC),D7",   0157272, 0x1232);
    TEST(ADD_L, "(*+18,PC,D3.W),D7", 0157273, 0x3010);
    TEST(ADD_L, "#$00345678,D7",     0157274, 0x0034, 0x5678);

    // ADDA src,An: 015|An|Sz|M|Rn, Sz:W=3/L=7
    TEST(ADDA_W, "D2,A6",             0156302);
    TEST(ADDA_W, "A2,A6",             0156312);
    TEST(ADDA_W, "(A2),A6",           0156322);
    TEST(ADDA_W, "(A2)+,A6",          0156332);
    TEST(ADDA_W, "-(A2),A6",          0156342);
    TEST(ADDA_W, "($1234,A2),A6",     0156352, 0x1234);
    TEST(ADDA_W, "(18,A2,D3.L),A6",   0156362, 0x3812);
    TEST(ADDA_W, "($FFFFFE).W,A6",    0156370, 0xFFFE);
    TEST(ADDA_W, "($123456).L,A6",    0156371, 0x0012, 0x3456);
    TEST(ADDA_W, "(*+$1234,PC),A6",   0156372, 0x1232);
    TEST(ADDA_W, "(*+18,PC,D3.W),A6", 0156373, 0x3010);
    TEST(ADDA_W, "#$0034,A6",         0156374, 0x0034);
    TEST(ADDA_L, "D2,A6",             0156702);
    TEST(ADDA_L, "A2,A6",             0156712);
    TEST(ADDA_L, "(A2),A6",           0156722);
    TEST(ADDA_L, "(A2)+,A6",          0156732);
    TEST(ADDA_L, "-(A2),A6",          0156742);
    TEST(ADDA_L, "($1234,A2),A6",     0156752, 0x1234);
    TEST(ADDA_L, "(18,A2,D3.L),A6",   0156762, 0x3812);
    TEST(ADDA_L, "($FFFFFC).W,A6",    0156770, 0xFFFC);
    TEST(ADDA_L, "($123454).L,A6",    0156771, 0x0012, 0x3454);
    TEST(ADDA_L, "(*+$1234,PC),A6",   0156772, 0x1232);
    TEST(ADDA_L, "(*+18,PC,D3.W),A6", 0156773, 0x3010);
    TEST(ADDA_L, "#$00345678,A6",     0156774, 0x0034, 0x5678);

    // ADDI #xxxx,dst: 0003|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST(ADDI_B, "#18,D2",            0003002, 0x0012);
    EROA(ADDI_B, "#18,A2",            0003012, 0x0012);
    TEST(ADDI_B, "#18,(A2)",          0003022, 0x0012);
    TEST(ADDI_B, "#18,(A2)+",         0003032, 0x0012);
    TEST(ADDI_B, "#18,-(A2)",         0003042, 0x0012);
    TEST(ADDI_B, "#18,($1234,A2)",    0003052, 0x0012, 0x1234);
    TEST(ADDI_B, "#18,(18,A2,D3.W)",  0003062, 0x0012, 0x3012);
    TEST(ADDI_B, "#18,($001234).W",   0003070, 0x0012, 0x1234);
    TEST(ADDI_B, "#18,($123456).L",   0003071, 0x0012, 0x0012, 0x3456);
    EROA(ADDI_B, "#18,(d16,PC)",      0003072, 0x0012);
    EROA(ADDI_B, "#18,(d8,PC,Xn)",    0003073, 0x0012);
    EROA(ADDI_B, "#18,#nnnn",         0003074, 0x0012);
    TEST(ADDI_W, "#$5678,D2",           0003102, 0x5678);
    EROA(ADDI_W, "#$5678,A2",           0003112, 0x5678);
    TEST(ADDI_W, "#$5678,(A2)",         0003122, 0x5678);
    TEST(ADDI_W, "#$5678,(A2)+",        0003132, 0x5678);
    TEST(ADDI_W, "#$5678,-(A2)",        0003142, 0x5678);
    TEST(ADDI_W, "#$5678,($1234,A2)",   0003152, 0x5678, 0x1234);
    TEST(ADDI_W, "#$5678,(18,A2,D3.W)", 0003162, 0x5678, 0x3012);
    TEST(ADDI_W, "#$5678,($001234).W",  0003170, 0x5678, 0x1234);
    TEST(ADDI_W, "#$5678,($123456).L",  0003171, 0x5678, 0x0012, 0x3456);
    EROA(ADDI_W, "#$5678,(d16,PC)",     0003172, 0x5678);
    EROA(ADDI_W, "#$5678,(d8,PC,Xn)",   0003173, 0x5678);
    EROA(ADDI_W, "#$5678,#nnnn",        0003174, 0x5678);
    TEST(ADDI_L, "#$3456789A,D2",           0003202, 0x3456, 0x789A);
    EROA(ADDI_L, "#$3456789A,A2",           0003212, 0x3456, 0x789A);
    TEST(ADDI_L, "#$3456789A,(A2)",         0003222, 0x3456, 0x789A);
    TEST(ADDI_L, "#$3456789A,(A2)+",        0003232, 0x3456, 0x789A);
    TEST(ADDI_L, "#$3456789A,-(A2)",        0003242, 0x3456, 0x789A);
    TEST(ADDI_L, "#$3456789A,($1234,A2)",   0003252, 0x3456, 0x789A, 0x1234);
    TEST(ADDI_L, "#$3456789A,(18,A2,D3.W)", 0003262, 0x3456, 0x789A, 0x3012);
    TEST(ADDI_L, "#$3456789A,($001234).W",  0003270, 0x3456, 0x789A, 0x1234);
    TEST(ADDI_L, "#$34567898,($123454).L",  0003271, 0x3456, 0x7898, 0x0012, 0x3454);
    EROA(ADDI_L, "#$3456789A,(d16,PC)",     0003272, 0x3456, 0x789A);
    EROA(ADDI_L, "#$3456789A,(d8,PC,Xn)",   0003273, 0x3456, 0x789A);
    EROA(ADDI_L, "#$3456789A,#nnnn",        0003274, 0x3456, 0x789A);

    // ADDQ #nn,dst: 005|nn|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST(ADDQ_B, "#8,D2",           0050002);
    EROA(ADDQ_B, "#8,A2",           0050012);
    TEST(ADDQ_B, "#8,(A2)",         0050022);
    TEST(ADDQ_B, "#8,(A2)+",        0050032);
    TEST(ADDQ_B, "#8,-(A2)",        0050042);
    TEST(ADDQ_B, "#8,($1234,A2)",   0050052, 0x1234);
    TEST(ADDQ_B, "#8,(18,A2,D3.W)", 0050062, 0x3012);
    TEST(ADDQ_B, "#8,($001234).W",  0050070, 0x1234);
    TEST(ADDQ_B, "#8,($123456).L",  0050071, 0x0012, 0x3456);
    EROA(ADDQ_B, "#8,(d16,PC)",     0050072);
    EROA(ADDQ_B, "#8,(d8,PC,Xn)",   0050073);
    EROA(ADDQ_B, "#8,#nnnn",        0050074);
    TEST(ADDQ_W, "#8,D2",           0050102);
    TEST(ADDQ_W, "#8,A2",           0050112);
    TEST(ADDQ_W, "#8,(A2)",         0050122);
    TEST(ADDQ_W, "#8,(A2)+",        0050132);
    TEST(ADDQ_W, "#8,-(A2)",        0050142);
    TEST(ADDQ_W, "#8,($1234,A2)",   0050152, 0x1234);
    TEST(ADDQ_W, "#8,(18,A2,D3.W)", 0050162, 0x3012);
    TEST(ADDQ_W, "#8,($001234).W",  0050170, 0x1234);
    TEST(ADDQ_W, "#8,($123456).L",  0050171, 0x0012, 0x3456);
    EROA(ADDQ_W, "#8,(d16,PC)",     0050172);
    EROA(ADDQ_W, "#8,(d8,PC,Xn)",   0050173);
    EROA(ADDQ_W, "#8,#nnnn",        0050174);
    TEST(ADDQ_L, "#8,D2",           0050202);
    TEST(ADDQ_L, "#8,A2",           0050212);
    TEST(ADDQ_L, "#8,(A2)",         0050222);
    TEST(ADDQ_L, "#8,(A2)+",        0050232);
    TEST(ADDQ_L, "#8,-(A2)",        0050242);
    TEST(ADDQ_L, "#8,($1234,A2)",   0050252, 0x1234);
    TEST(ADDQ_L, "#8,(18,A2,D3.W)", 0050262, 0x3012);
    TEST(ADDQ_L, "#8,($001234).W",  0050270, 0x1234);
    TEST(ADDQ_L, "#8,($123454).L",  0050271, 0x0012, 0x3454);
    EROA(ADDQ_L, "#8,(d16,PC)",     0050272);
    EROA(ADDQ_L, "#8,(d8,pC,Xn)",   0050273);
    EROA(ADDQ_L, "#8,#nnnn",        0050274);

    // ADDX Dx,Dy: 015|Dy|Sz|0|Dx, Sz:B=4/W=5/L=6
    TEST(ADDX_B, "D2,D3", 0153402);
    TEST(ADDX_W, "D2,D3", 0153502);
    TEST(ADDX_L, "D2,D3", 0153602);

    // ADDX -(Ax),-(Ay), 015|Ay|Sz|1|Ax, Sz:B=4/W=5/L=6
    TEST(ADDX_B, "-(A2),-(A3)", 0153412);
    TEST(ADDX_W, "-(A2),-(A3)", 0153512);
    TEST(ADDX_L, "-(A2),-(A3)", 0153612);

    // CLR dst: 0041|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST(CLR_B, "D2",           0041002);
    EROA(CLR_B, "A2",           0041012);
    TEST(CLR_B, "(A2)",         0041022);
    TEST(CLR_B, "(A2)+",        0041032);
    TEST(CLR_B, "-(A2)",        0041042);
    TEST(CLR_B, "($1234,A2)",   0041052, 0x1234);
    TEST(CLR_B, "(18,A2,D3.W)", 0041062, 0x3012);
    TEST(CLR_B, "($001234).W",  0041070, 0x1234);
    TEST(CLR_B, "($123456).L",  0041071, 0x0012, 0x3456);
    EROA(CLR_B, "(d16,PC)",     0041072);
    EROA(CLR_B, "(d8,PC,Xn)",   0041073);
    EROA(CLR_B, "#nnnn",        0041074);
    TEST(CLR_W, "D2",           0041102);
    EROA(CLR_W, "A2",           0041112);
    TEST(CLR_W, "(A2)",         0041122);
    TEST(CLR_W, "(A2)+",        0041132);
    TEST(CLR_W, "-(A2)",        0041142);
    TEST(CLR_W, "($1234,A2)",   0041152, 0x1234);
    TEST(CLR_W, "(18,A2,D3.W)", 0041162, 0x3012);
    TEST(CLR_W, "($001234).W",  0041170, 0x1234);
    TEST(CLR_W, "($123456).L",  0041171, 0x0012, 0x3456);
    EROA(CLR_W, "(d16,PC)",     0041172);
    EROA(CLR_W, "(d8,PC,Xn)",   0041173);
    EROA(CLR_W, "#nnnn",        0041174);
    TEST(CLR_L, "D2",           0041202);
    EROA(CLR_L, "A2",           0041212);
    TEST(CLR_L, "(A2)",         0041222);
    TEST(CLR_L, "(A2)+",        0041232);
    TEST(CLR_L, "-(A2)",        0041242);
    TEST(CLR_L, "($1234,A2)",   0041252, 0x1234);
    TEST(CLR_L, "(18,A2,D3.W)", 0041262, 0x3012);
    TEST(CLR_L, "($001234).W",  0041270, 0x1234);
    TEST(CLR_L, "($123454).L",  0041271, 0x0012, 0x3454);
    EROA(CLR_B, "(d16,PC)",     0041272);
    EROA(CLR_B, "(d8,PC,Xn)",   0041273);
    EROA(CLR_B, "#nnnn",        0041274);

    // CMP src,Dn: 013|Dn|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST(CMP_B, "D2,D7",             0137002);
    EROA(CMP_B, "A2",                0137012);
    TEST(CMP_B, "(A2),D7",           0137022);
    TEST(CMP_B, "(A2)+,D7",          0137032);
    TEST(CMP_B, "-(A2),D7",          0137042);
    TEST(CMP_B, "($1234,A2),D7",     0137052, 0x1234);
    TEST(CMP_B, "(18,A2,D3.L),D7",   0137062, 0x3812);
    TEST(CMP_B, "($FFFFFF).W,D7",    0137070, 0xFFFF);
    TEST(CMP_B, "($123456).L,D7",    0137071, 0x0012, 0x3456);
    TEST(CMP_B, "(*+$1234,PC),D7",   0137072, 0x1232);
    TEST(CMP_B, "(*+18,PC,D3.W),D7", 0137073, 0x3010);
    TEST(CMP_B, "#$34,D7",           0137074, 0x0034);
    TEST(CMP_W, "D2,D7",             0137102);
    TEST(CMP_W, "A2,D7",             0137112);
    TEST(CMP_W, "(A2),D7",           0137122);
    TEST(CMP_W, "(A2)+,D7",          0137132);
    TEST(CMP_W, "-(A2),D7",          0137142);
    TEST(CMP_W, "($1234,A2),D7",     0137152, 0x1234);
    TEST(CMP_W, "(18,A2,D3.L),D7",   0137162, 0x3812);
    TEST(CMP_W, "($FFFFFE).W,D7",    0137170, 0xFFFE);
    TEST(CMP_W, "($123456).L,D7",    0137171, 0x0012, 0x3456);
    TEST(CMP_W, "(*+$1234,PC),D7",   0137172, 0x1232);
    TEST(CMP_W, "(*+18,PC,D3.W),D7", 0137173, 0x3010);
    TEST(CMP_W, "#$0034,D7",         0137174, 0x0034);
    TEST(CMP_L, "D2,D7",             0137202);
    TEST(CMP_L, "A2,D7",             0137212);
    TEST(CMP_L, "(A2),D7",           0137222);
    TEST(CMP_L, "(A2)+,D7",          0137232);
    TEST(CMP_L, "-(A2),D7",          0137242);
    TEST(CMP_L, "($1234,A2),D7",     0137252, 0x1234);
    TEST(CMP_L, "(18,A2,D3.L),D7",   0137262, 0x3812);
    TEST(CMP_L, "($FFFFFC).W,D7",    0137270, 0xFFFC);
    TEST(CMP_L, "($123454).L,D7",    0137271, 0x0012, 0x3454);
    TEST(CMP_L, "(*+$1234,PC),D7",   0137272, 0x1232);
    TEST(CMP_L, "(*+18,PC,D3.W),D7", 0137273, 0x3010);
    TEST(CMP_L, "#$00345678,D7",     0137274, 0x0034, 0x5678);

    // CMPA src,An: 013|An|Sz|M|Rn, Sz:W=3/L=7
    TEST(CMPA_W, "D2,A6",             0136302);
    TEST(CMPA_W, "A2,A6",             0136312);
    TEST(CMPA_W, "(A2),A6",           0136322);
    TEST(CMPA_W, "(A2)+,A6",          0136332);
    TEST(CMPA_W, "-(A2),A6",          0136342);
    TEST(CMPA_W, "($1234,A2),A6",     0136352, 0x1234);
    TEST(CMPA_W, "(18,A2,D3.L),A6",   0136362, 0x3812);
    TEST(CMPA_W, "($FFFFFE).W,A6",    0136370, 0xFFFE);
    TEST(CMPA_W, "($123456).L,A6",    0136371, 0x0012, 0x3456);
    TEST(CMPA_W, "(*+$1234,PC),A6",   0136372, 0x1232);
    TEST(CMPA_W, "(*+18,PC,D3.W),A6", 0136373, 0x3010);
    TEST(CMPA_W, "#$0034,A6",         0136374, 0x0034);
    TEST(CMPA_L, "D2,A6",             0136702);
    TEST(CMPA_L, "A2,A6",             0136712);
    TEST(CMPA_L, "(A2),A6",           0136722);
    TEST(CMPA_L, "(A2)+,A6",          0136732);
    TEST(CMPA_L, "-(A2),A6",          0136742);
    TEST(CMPA_L, "($1234,A2),A6",     0136752, 0x1234);
    TEST(CMPA_L, "(18,A2,D3.L),A6",   0136762, 0x3812);
    TEST(CMPA_L, "($FFFFFC).W,A6",    0136770, 0xFFFC);
    TEST(CMPA_L, "($123454).L,A6",    0136771, 0x0012, 0x3454);
    TEST(CMPA_L, "(*+$1234,PC),A6",   0136772, 0x1232);
    TEST(CMPA_L, "(*+18,PC,D3.W),A6", 0136773, 0x3010);
    TEST(CMPA_L, "#$00345678,A6",     0136774, 0x0034, 0x5678);

    // CMPI #nn,dst: 0006|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST(CMPI_B, "#18,D2",           0006002, 0x0012);
    EROA(CMPI_B, "#18,A2",           0006012, 0x0012);
    TEST(CMPI_B, "#18,(A2)",         0006022, 0x0012);
    TEST(CMPI_B, "#18,(A2)+",        0006032, 0x0012);
    TEST(CMPI_B, "#18,-(A2)",        0006042, 0x0012);
    TEST(CMPI_B, "#18,($1234,A2)",   0006052, 0x0012, 0x1234);
    TEST(CMPI_B, "#18,(18,A2,D3.W)", 0006062, 0x0012, 0x3012);
    TEST(CMPI_B, "#18,($001234).W",  0006070, 0x0012, 0x1234);
    TEST(CMPI_B, "#18,($123456).L",  0006071, 0x0012, 0x0012, 0x3456);
    EROA(CMPI_B, "#18,(d16,PC)",     0006072, 0x0012);
    EROA(CMPI_B, "#18,(d8,PC,Xn)",   0006073, 0x0012);
    EROA(CMPI_B, "#18,#nnnn",        0006074, 0x0012);
    TEST(CMPI_W, "#$5678,D2",           0006102, 0x5678);
    EROA(CMPI_W, "#$5678,A2",           0006112, 0x5678);
    TEST(CMPI_W, "#$5678,(A2)",         0006122, 0x5678);
    TEST(CMPI_W, "#$5678,(A2)+",        0006132, 0x5678);
    TEST(CMPI_W, "#$5678,-(A2)",        0006142, 0x5678);
    TEST(CMPI_W, "#$5678,($1234,A2)",   0006152, 0x5678, 0x1234);
    TEST(CMPI_W, "#$5678,(18,A2,D3.W)", 0006162, 0x5678, 0x3012);
    TEST(CMPI_W, "#$5678,($001234).W",  0006170, 0x5678, 0x1234);
    TEST(CMPI_W, "#$5678,($123456).L",  0006171, 0x5678, 0x0012, 0x3456);
    EROA(CMPI_W, "#$5678,(d16,PC)",     0006172, 0x5678);
    EROA(CMPI_W, "#$5678,(d8,PC,Xn)",   0006173, 0x5678);
    EROA(CMPI_W, "#$5678,#nnnn",        0006174, 0x5678);
    TEST(CMPI_L, "#$3456789A,D2",           0006202, 0x3456, 0x789A);
    EROA(CMPI_L, "#$3456789A,A2",           0006212, 0x3456, 0x789A);
    TEST(CMPI_L, "#$3456789A,(A2)",         0006222, 0x3456, 0x789A);
    TEST(CMPI_L, "#$3456789A,(A2)+",        0006232, 0x3456, 0x789A);
    TEST(CMPI_L, "#$3456789A,-(A2)",        0006242, 0x3456, 0x789A);
    TEST(CMPI_L, "#$3456789A,($1234,A2)",   0006252, 0x3456, 0x789A, 0x1234);
    TEST(CMPI_L, "#$3456789A,(18,A2,D3.W)", 0006262, 0x3456, 0x789A, 0x3012);
    TEST(CMPI_L, "#$3456789A,($001234).W",  0006270, 0x3456, 0x789A, 0x1234);
    TEST(CMPI_L, "#$3456789A,($123454).L",  0006271, 0x3456, 0x789A, 0x0012, 0x3454);
    EROA(CMPI_L, "#$3456789A,(d16,PC)",     0006272, 0x3456, 0x789A);
    EROA(CMPI_L, "#$3456789A,(d8,PC,Xn)",   0006273, 0x3456, 0x789A);
    EROA(CMPI_L, "#$3456789A,#nnnn",        0006274, 0x3456, 0x789A);

    // CMPM (Ay)+,(Ax)+: 013|Ax|Sz|1|Ay, Sz:B=4/W=5/L=6
    TEST(CMPM_B, "(A2)+,(A7)+", 0137412);
    TEST(CMPM_W, "(A2)+,(A7)+", 0137512);
    TEST(CMPM_L, "(A2)+,(A7)+", 0137612);

    // DIVS src,Dn: 010|Dn|7|M|Rn
    TEST(DIVS_W, "D2,D7",             0107702);
    EROA(DIVS_W, "A2,D7",             0107712);
    TEST(DIVS_W, "(A2),D7",           0107722);
    TEST(DIVS_W, "(A2)+,D7",          0107732);
    TEST(DIVS_W, "-(A2),D7",          0107742);
    TEST(DIVS_W, "($1234,A2),D7",     0107752, 0x1234);
    TEST(DIVS_W, "(18,A2,D3.L),D7",   0107762, 0x3812);
    TEST(DIVS_W, "($FFFFFE).W,D7",    0107770, 0xFFFE);
    TEST(DIVS_W, "($123456).L,D7",    0107771, 0x0012, 0x3456);
    TEST(DIVS_W, "(*+$1234,PC),D7",   0107772, 0x1232);
    TEST(DIVS_W, "(*+18,PC,D3.W),D7", 0107773, 0x3010);
    TEST(DIVS_W, "#$5678,D7",         0107774, 0x5678);

    // DIVU src,Dn: 010|Dn|3|M|Rn
    TEST(DIVU_W, "D2,D7",             0107302);
    EROA(DIVU_W, "A2,D7",             0107312);
    TEST(DIVU_W, "(A2),D7",           0107322);
    TEST(DIVU_W, "(A2)+,D7",          0107332);
    TEST(DIVU_W, "-(A2),D7",          0107342);
    TEST(DIVU_W, "($1234,A2),D7",     0107352, 0x1234);
    TEST(DIVU_W, "(18,A2,D3.L),D7",   0107362, 0x3812);
    TEST(DIVU_W, "($FFFFFE).W,D7",    0107370, 0xFFFE);
    TEST(DIVU_W, "($123456).L,D7",    0107371, 0x0012, 0x3456);
    TEST(DIVU_W, "(*+$1234,PC),D7",   0107372, 0x1232);
    TEST(DIVU_W, "(*+18,PC,D3.W),D7", 0107373, 0x3010);
    TEST(DIVU_W, "#$5678,D7",         0107374, 0x5678);

    // EXT Dn: 0044|Sz|0|Dn, Sz:W=2/L=3
    TEST(EXT_W, "D2", 0044202);
    TEST(EXT_L, "D2", 0044302);

    // MULS src,Dn: 014|Dn|7|M|Rn
    TEST(MULS_W, "D2,D7",             0147702);
    EROA(MULS_W, "A2,D7",             0147712);
    TEST(MULS_W, "(A2),D7",           0147722);
    TEST(MULS_W, "(A2)+,D7",          0147732);
    TEST(MULS_W, "-(A2),D7",          0147742);
    TEST(MULS_W, "($1234,A2),D7",     0147752, 0x1234);
    TEST(MULS_W, "(18,A2,D3.L),D7",   0147762, 0x3812);
    TEST(MULS_W, "($FFFFFE).W,D7",    0147770, 0xFFFE);
    TEST(MULS_W, "($123456).L,D7",    0147771, 0x0012, 0x3456);
    TEST(MULS_W, "(*+$1234,PC),D7",   0147772, 0x1232);
    TEST(MULS_W, "(*+18,PC,D3.W),D7", 0147773, 0x3010);
    TEST(MULS_W, "#$5678,D7",         0147774, 0x5678);

    // MULU src,Dn: 014|Dn|3|M|Rn
    TEST(MULU_W, "D2,D7",             0147302);
    EROA(MULU_W, "A2,D7",             0147312);
    TEST(MULU_W, "(A2),D7",           0147322);
    TEST(MULU_W, "(A2)+,D7",          0147332);
    TEST(MULU_W, "-(A2),D7",          0147342);
    TEST(MULU_W, "($1234,A2),D7",     0147352, 0x1234);
    TEST(MULU_W, "(18,A2,D3.L),D7",   0147362, 0x3812);
    TEST(MULU_W, "($FFFFFE).W,D7",    0147370, 0xFFFE);
    TEST(MULU_W, "($123456).L,D7",    0147371, 0x0012, 0x3456);
    TEST(MULU_W, "(*+$1234,PC),D7",   0147372, 0x1232);
    TEST(MULU_W, "(*+18,PC,D3.W),D7", 0147373, 0x3010);
    TEST(MULU_W, "#$5678,D7",         0147374, 0x5678);

    // NEG dst: 0042|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST(NEG_B, "D2",           0042002);
    EROA(NEG_B, "A2",           0042012);
    TEST(NEG_B, "(A2)",         0042022);
    TEST(NEG_B, "(A2)+",        0042032);
    TEST(NEG_B, "-(A2)",        0042042);
    TEST(NEG_B, "($1234,A2)",   0042052, 0x1234);
    TEST(NEG_B, "(18,A2,D3.W)", 0042062, 0x3012);
    TEST(NEG_B, "($001234).W",  0042070, 0x1234);
    TEST(NEG_B, "($123456).L",  0042071, 0x0012, 0x3456);
    EROA(NEG_B, "(d16,PC)",     0042072);
    EROA(NEG_B, "(d8,PC,Xn)",   0042073);
    EROA(NEG_B, "#nnnn",        0042074);
    TEST(NEG_W, "D2",           0042102);
    EROA(NEG_W, "A2",           0042112);
    TEST(NEG_W, "(A2)",         0042122);
    TEST(NEG_W, "(A2)+",        0042132);
    TEST(NEG_W, "-(A2)",        0042142);
    TEST(NEG_W, "($1234,A2)",   0042152, 0x1234);
    TEST(NEG_W, "(18,A2,D3.W)", 0042162, 0x3012);
    TEST(NEG_W, "($001234).W",  0042170, 0x1234);
    TEST(NEG_W, "($123456).L",  0042171, 0x0012, 0x3456);
    EROA(NEG_W, "(d16,PC)",     0042172);
    EROA(NEG_W, "(d8,PC,Xn)",   0042173);
    EROA(NEG_W, "#nnnn",        0042174);
    TEST(NEG_L, "D2",           0042202);
    EROA(NEG_L, "A2",           0042212);
    TEST(NEG_L, "(A2)",         0042222);
    TEST(NEG_L, "(A2)+",        0042232);
    TEST(NEG_L, "-(A2)",        0042242);
    TEST(NEG_L, "($1234,A2)",   0042252, 0x1234);
    TEST(NEG_L, "(18,A2,D3.W)", 0042262, 0x3012);
    TEST(NEG_L, "($001234).W",  0042270, 0x1234);
    TEST(NEG_L, "($123454).L",  0042271, 0x0012, 0x3454);
    EROA(NEG_L, "(d16,PC)",     0042272);
    EROA(NEG_L, "(d8,PC,Xn)",   0042273);
    EROA(NEG_L, "#nnnn",        0042274);

    // NEGX dst: 0040|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST(NEGX_B, "D2",           0040002);
    EROA(NEGX_B, "A2",           0040012);
    TEST(NEGX_B, "(A2)",         0040022);
    TEST(NEGX_B, "(A2)+",        0040032);
    TEST(NEGX_B, "-(A2)",        0040042);
    TEST(NEGX_B, "($1234,A2)",   0040052, 0x1234);
    TEST(NEGX_B, "(18,A2,D3.W)", 0040062, 0x3012);
    TEST(NEGX_B, "($001234).W",  0040070, 0x1234);
    TEST(NEGX_B, "($123456).L",  0040071, 0x0012, 0x3456);
    EROA(NEGX_B, "(d16,PC)",     0040072);
    EROA(NEGX_B, "(d8,PC,Xn)",   0040073);
    EROA(NEGX_B, "#nnnn",        0040074);
    TEST(NEGX_W, "D2",           0040102);
    EROA(NEGX_W, "A2",           0040112);
    TEST(NEGX_W, "(A2)",         0040122);
    TEST(NEGX_W, "(A2)+",        0040132);
    TEST(NEGX_W, "-(A2)",        0040142);
    TEST(NEGX_W, "($1234,A2)",   0040152, 0x1234);
    TEST(NEGX_W, "(18,A2,D3.W)", 0040162, 0x3012);
    TEST(NEGX_W, "($001234).W",  0040170, 0x1234);
    TEST(NEGX_W, "($123456).L",  0040171, 0x0012, 0x3456);
    EROA(NEGX_W, "(d16,PC)",     0040172);
    EROA(NEGX_W, "(d8,PC,Xn)",   0040173);
    EROA(NEGX_W, "#nnnn",        0040174);
    TEST(NEGX_L, "D2",           0040202);
    EROA(NEGX_L, "A2",           0040212);
    TEST(NEGX_L, "(A2)",         0040222);
    TEST(NEGX_L, "(A2)+",        0040232);
    TEST(NEGX_L, "-(A2)",        0040242);
    TEST(NEGX_L, "($1234,A2)",   0040252, 0x1234);
    TEST(NEGX_L, "(18,A2,D3.W)", 0040262, 0x3012);
    TEST(NEGX_L, "($001234).W",  0040270, 0x1234);
    TEST(NEGX_L, "($123454).L",  0040271, 0x0012, 0x3454);
    EROA(NEGX_L, "(d16,PC)",     0040272);
    EROA(NEGX_L, "(d8,PC,Xn)",   0040273);
    EROA(NEGX_L, "#nnnn",        0040274);

    // SUB src,Dn: 011|Dn|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST(SUB_B, "D2,D7",             0117002);
    EROA(SUB_B, "A2,D7",             0117012);
    TEST(SUB_B, "(A2),D7",           0117022);
    TEST(SUB_B, "(A2)+,D7",          0117032);
    TEST(SUB_B, "-(A2),D7",          0117042);
    TEST(SUB_B, "($1234,A2),D7",     0117052, 0x1234);
    TEST(SUB_B, "(18,A2,D3.L),D7",   0117062, 0x3812);
    TEST(SUB_B, "($FFFFFF).W,D7",    0117070, 0xFFFF);
    TEST(SUB_B, "($123456).L,D7",    0117071, 0x0012, 0x3456);
    TEST(SUB_B, "(*+$1234,PC),D7",   0117072, 0x1232);
    TEST(SUB_B, "(*+18,PC,D3.W),D7", 0117073, 0x3010);
    TEST(SUB_B, "#$34,D7",           0117074, 0x0034);
    TEST(SUB_W, "D2,D7",             0117102);
    TEST(SUB_W, "A2,D7",             0117112);
    TEST(SUB_W, "(A2),D7",           0117122);
    TEST(SUB_W, "(A2)+,D7",          0117132);
    TEST(SUB_W, "-(A2),D7",          0117142);
    TEST(SUB_W, "($1234,A2),D7",     0117152, 0x1234);
    TEST(SUB_W, "(18,A2,D3.L),D7",   0117162, 0x3812);
    TEST(SUB_W, "($FFFFFE).W,D7",    0117170, 0xFFFE);
    TEST(SUB_W, "($123456).L,D7",    0117171, 0x0012, 0x3456);
    TEST(SUB_W, "(*+$1234,PC),D7",   0117172, 0x1232);
    TEST(SUB_W, "(*+18,PC,D3.W),D7", 0117173, 0x3010);
    TEST(SUB_W, "#$0034,D7",         0117174, 0x0034);
    TEST(SUB_L, "D2,D7",             0117202);
    TEST(SUB_L, "A2,D7",             0117212);
    TEST(SUB_L, "(A2),D7",           0117222);
    TEST(SUB_L, "(A2)+,D7",          0117232);
    TEST(SUB_L, "-(A2),D7",          0117242);
    TEST(SUB_L, "($1234,A2),D7",     0117252, 0x1234);
    TEST(SUB_L, "(18,A2,D3.L),D7",   0117262, 0x3812);
    TEST(SUB_L, "($FFFFFC).W,D7",    0117270, 0xFFFC);
    TEST(SUB_L, "($123454).L,D7",    0117271, 0x0012, 0x3454);
    TEST(SUB_L, "(*+$1234,PC),D7",   0117272, 0x1232);
    TEST(SUB_L, "(*+18,PC,D3.W),D7", 0117273, 0x3010);
    TEST(SUB_L, "#$00345678,D7",     0117274, 0x0034, 0x5678);

    // SUB Dn,dst: 011|Dn|Sz|M|Rn, Sz:B=4/W=5/L=6
    TEST(SUBX_B, "D2,D7",          0117402); // SUBX.B
    TEST(SUBX_B, "-(A2),-(A7)",    0117412); // SUBX.B
    TEST(SUB_B, "D7,(A2)",         0117422);
    TEST(SUB_B, "D7,(A2)+",        0117432);
    TEST(SUB_B, "D7,-(A2)",        0117442);
    TEST(SUB_B, "D7,($1234,A2)",   0117452, 0x1234);
    TEST(SUB_B, "D7,(18,A2,D3.L)", 0117462, 0x3812);
    TEST(SUB_B, "D7,($FFFFFF).W",  0117470, 0xFFFF);
    TEST(SUB_B, "D7,($123456).L",  0117471, 0x0012, 0x3456);
    EROA(SUB_B, "D7,(d16,PC)",     0117472);
    EROA(SUB_B, "D7,(d8,PC,Xn)",   0117473);
    EROA(SUB_B, "D7,#nnnn",        0117474);
    TEST(SUBX_W, "D2,D7",          0117502); // SUBX.W
    TEST(SUBX_W, "-(A2),-(A7)",    0117512); // SUBX.L
    TEST(SUB_W, "D7,(A2)",         0117522);
    TEST(SUB_W, "D7,(A2)+",        0117532);
    TEST(SUB_W, "D7,-(A2)",        0117542);
    TEST(SUB_W, "D7,($1234,A2)",   0117552, 0x1234);
    TEST(SUB_W, "D7,(18,A2,D3.L)", 0117562, 0x3812);
    TEST(SUB_W, "D7,($FFFFFE).W",  0117570, 0xFFFE);
    TEST(SUB_W, "D7,($123456).L",  0117571, 0x0012, 0x3456);
    EROA(SUB_W, "D7,(d16,PC)",     0117572);
    EROA(SUB_W, "D7,(d8,PC,Xn)",   0117573);
    EROA(SUB_W, "D7,#nnnn",        0117574);
    TEST(SUBX_L, "D2,D7",          0117602); // SUBX.L
    TEST(SUBX_L, "-(A2),-(A7)",    0117612); // SUBX.L
    TEST(SUB_L, "D7,(A2)",         0117622);
    TEST(SUB_L, "D7,(A2)+",        0117632);
    TEST(SUB_L, "D7,-(A2)",        0117642);
    TEST(SUB_L, "D7,($1234,A2)",   0117652, 0x1234);
    TEST(SUB_L, "D7,(18,A2,D3.L)", 0117662, 0x3812);
    TEST(SUB_L, "D7,($FFFFFC).W",  0117670, 0xFFFC);
    TEST(SUB_L, "D7,($123454).L",  0117671, 0x0012, 0x3454);
    EROA(SUB_L, "D7,(d16,PC)",     0117672);
    EROA(SUB_L, "D7,(d8,PC,Xn)",   0117673);
    EROA(SUB_L, "D7,#nnnn",        0117674);

    // SUBA src,An: 011|An|Sz|M|Rn, Sz:W=3/L=7
    TEST(SUBA_W, "D2,A6",             0116302);
    TEST(SUBA_W, "A2,A6",             0116312);
    TEST(SUBA_W, "(A2),A6",           0116322);
    TEST(SUBA_W, "(A2)+,A6",          0116332);
    TEST(SUBA_W, "-(A2),A6",          0116342);
    TEST(SUBA_W, "($1234,A2),A6",     0116352, 0x1234);
    TEST(SUBA_W, "(18,A2,D3.L),A6",   0116362, 0x3812);
    TEST(SUBA_W, "($FFFFFE).W,A6",    0116370, 0xFFFE);
    TEST(SUBA_W, "($123456).L,A6",    0116371, 0x0012, 0x3456);
    TEST(SUBA_W, "(*+$1234,PC),A6",   0116372, 0x1232);
    TEST(SUBA_W, "(*+18,PC,D3.W),A6", 0116373, 0x3010);
    TEST(SUBA_W, "#$0034,A6",         0116374, 0x0034);
    TEST(SUBA_L, "D2,A6",             0116702);
    TEST(SUBA_L, "A2,A6",             0116712);
    TEST(SUBA_L, "(A2),A6",           0116722);
    TEST(SUBA_L, "(A2)+,A6",          0116732);
    TEST(SUBA_L, "-(A2),A6",          0116742);
    TEST(SUBA_L, "($1234,A2),A6",     0116752, 0x1234);
    TEST(SUBA_L, "(18,A2,D3.L),A6",   0116762, 0x3812);
    TEST(SUBA_L, "($FFFFFC).W,A6",    0116770, 0xFFFC);
    TEST(SUBA_L, "($123454).L,A6",    0116771, 0x0012, 0x3454);
    TEST(SUBA_L, "(*+$1234,PC),A6",   0116772, 0x1232);
    TEST(SUBA_L, "(*+18,PC,D3.W),A6", 0116773, 0x3010);
    TEST(SUBA_L, "#$00345678,A6",     0116774, 0x0034, 0x5678);

    // SUBI #nn,dst: 0002|Sz|M|Rn, SZ:B=0/W=1/L=2
    TEST(SUBI_B, "#18,D2",           0002002, 0x0012);
    EROA(SUBI_B, "#18,A2",           0002012, 0x0012);
    TEST(SUBI_B, "#18,(A2)",         0002022, 0x0012);
    TEST(SUBI_B, "#18,(A2)+",        0002032, 0x0012);
    TEST(SUBI_B, "#18,-(A2)",        0002042, 0x0012);
    TEST(SUBI_B, "#18,($1234,A2)",   0002052, 0x0012, 0x1234);
    TEST(SUBI_B, "#18,(18,A2,D3.W)", 0002062, 0x0012, 0x3012);
    TEST(SUBI_B, "#18,($001234).W",  0002070, 0x0012, 0x1234);
    TEST(SUBI_B, "#18,($123456).L",  0002071, 0x0012, 0x0012, 0x3456);
    EROA(SUBI_B, "#18,(d16,PC)",     0002072, 0x0012);
    EROA(SUBI_B, "#18,(d8,PC,Xn)",   0002073, 0x0012);
    EROA(SUBI_B, "#18,#nnnn",        0002074, 0x0012);
    TEST(SUBI_W, "#$5678,D2",           0002102, 0x5678);
    EROA(SUBI_W, "#$5678,A2",           0002112, 0x5678);
    TEST(SUBI_W, "#$5678,(A2)",         0002122, 0x5678);
    TEST(SUBI_W, "#$5678,(A2)+",        0002132, 0x5678);
    TEST(SUBI_W, "#$5678,-(A2)",        0002142, 0x5678);
    TEST(SUBI_W, "#$5678,($1234,A2)",   0002152, 0x5678, 0x1234);
    TEST(SUBI_W, "#$5678,(18,A2,D3.W)", 0002162, 0x5678, 0x3012);
    TEST(SUBI_W, "#$5678,($001234).W",  0002170, 0x5678, 0x1234);
    TEST(SUBI_W, "#$5678,($123456).L",  0002171, 0x5678, 0x0012, 0x3456);
    EROA(SUBI_W, "#$5678,(d16,PC)",     0002172, 0x5678);
    EROA(SUBI_W, "#$5678,(d8,PC,Xn)",   0002173, 0x5678);
    EROA(SUBI_W, "#$5678,#nnnn",        0002174, 0x5678);
    TEST(SUBI_L, "#$3456789A,D2",           0002202, 0x3456, 0x789A);
    EROA(SUBI_L, "#$3456789A,A2,",          0002212, 0x3456, 0x789A);
    TEST(SUBI_L, "#$3456789A,(A2)",         0002222, 0x3456, 0x789A);
    TEST(SUBI_L, "#$3456789A,(A2)+",        0002232, 0x3456, 0x789A);
    TEST(SUBI_L, "#$3456789A,-(A2)",        0002242, 0x3456, 0x789A);
    TEST(SUBI_L, "#$3456789A,($1234,A2)",   0002252, 0x3456, 0x789A, 0x1234);
    TEST(SUBI_L, "#$3456789A,(18,A2,D3.W)", 0002262, 0x3456, 0x789A, 0x3012);
    TEST(SUBI_L, "#$3456789A,($001234).W",  0002270, 0x3456, 0x789A, 0x1234);
    TEST(SUBI_L, "#$3456789A,($123454).L",  0002271, 0x3456, 0x789A, 0x0012, 0x3454);
    EROA(SUBI_L, "#$3456789A,(d16,PC)",     0002272, 0x3456, 0x789A);
    EROA(SUBI_L, "#$3456789A,(d8,PC,Xn)",   0002273, 0x3456, 0x789A);
    EROA(SUBI_L, "#$3456789A,#nnnn",        0002274, 0x3456, 0x789A);

    // SUBQ #nn,dst: 005|nn|Sz|M\En, Sz:B=4/W=5/L=6
    TEST(SUBQ_B, "#8,D2",           0050402);
    EROA(SUBQ_B, "#8,A2",           0050412);
    TEST(SUBQ_B, "#8,(A2)",         0050422);
    TEST(SUBQ_B, "#8,(A2)+",        0050432);
    TEST(SUBQ_B, "#8,-(A2)",        0050442);
    TEST(SUBQ_B, "#8,($1234,A2)",   0050452, 0x1234);
    TEST(SUBQ_B, "#8,(18,A2,D3.W)", 0050462, 0x3012);
    TEST(SUBQ_B, "#8,($001234).W",  0050470, 0x1234);
    TEST(SUBQ_B, "#8,($123456).L",  0050471, 0x0012, 0x3456);
    EROA(SUBQ_B, "#8,(d16,PC)",     0050472);
    EROA(SUBQ_B, "#8,(d8,PC,Xn)",   0050473);
    EROA(SUBQ_B, "#8,#nnnn",        0050474);
    TEST(SUBQ_W, "#8,D2",           0050502);
    TEST(SUBQ_W, "#8,A2",           0050512);
    TEST(SUBQ_W, "#8,(A2)",         0050522);
    TEST(SUBQ_W, "#8,(A2)+",        0050532);
    TEST(SUBQ_W, "#8,-(A2)",        0050542);
    TEST(SUBQ_W, "#8,($1234,A2)",   0050552, 0x1234);
    TEST(SUBQ_W, "#8,(18,A2,D3.W)", 0050562, 0x3012);
    TEST(SUBQ_W, "#8,($001234).W",  0050570, 0x1234);
    TEST(SUBQ_W, "#8,($123456).L",  0050571, 0x0012, 0x3456);
    EROA(SUBQ_W, "#8,(d16,PC)",     0050572);
    EROA(SUBQ_W, "#8,(d8,PC,Xn)",   0050573);
    EROA(SUBQ_W, "#8,#nnnn",        0050574);
    TEST(SUBQ_L, "#8,D2",           0050602);
    TEST(SUBQ_L, "#8,A2",           0050612);
    TEST(SUBQ_L, "#8,(A2)",         0050622);
    TEST(SUBQ_L, "#8,(A2)+",        0050632);
    TEST(SUBQ_L, "#8,-(A2)",        0050642);
    TEST(SUBQ_L, "#8,($1234,A2)",   0050652, 0x1234);
    TEST(SUBQ_L, "#8,(18,A2,D3.W)", 0050662, 0x3012);
    TEST(SUBQ_L, "#8,($001234).W",  0050670, 0x1234);
    TEST(SUBQ_L, "#8,($123454).L",  0050671, 0x0012, 0x3454);
    EROA(SUBQ_L, "#8,(d16,PC)",     0050672);
    EROA(SUBQ_L, "#8,(d8,PC,Xn)",   0050673);
    EROA(SUBQ_L, "#8,#nnnn",        0050674);

    // SUBX Dx,Dy: 011|Dy|Sz|0|Dx, Sz:B=4/W=5/L=6
    TEST(SUBX_B, "D2,D3", 0113402);
    TEST(SUBX_W, "D2,D3", 0113502);
    TEST(SUBX_L, "D2,D3", 0113602);

    // SUBX -(Ax),-(Ay): 011|Ay|Sz|1|Ax, Sz:B=4/W=5/L=6
    TEST(SUBX_B, "-(A2),-(A3)", 0113412);
    TEST(SUBX_W, "-(A2),-(A3)", 0113512);
    TEST(SUBX_L, "-(A2),-(A3)", 0113612);
}

static void test_logical() {
    // AND src,Dn: 014|Dn|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST(AND_B, "D2,D7",             0147002);
    EROA(AND_B, "A2,D7",             0147012);
    TEST(AND_B, "(A2),D7",           0147022);
    TEST(AND_B, "(A2)+,D7",          0147032);
    TEST(AND_B, "-(A2),D7",          0147042);
    TEST(AND_B, "($1234,A2),D7",     0147052, 0x1234);
    TEST(AND_B, "(18,A2,D3.L),D7",   0147062, 0x3812);
    TEST(AND_B, "($FFFFFF).W,D7",    0147070, 0xFFFF);
    TEST(AND_B, "($123456).L,D7",    0147071, 0x0012, 0x3456);
    TEST(AND_B, "(*+$1234,PC),D7",   0147072, 0x1232);
    TEST(AND_B, "(*+18,PC,D3.W),D7", 0147073, 0x3010);
    TEST(AND_B, "#$34,D7",           0147074, 0x0034);
    TEST(AND_W, "D2,D7",             0147102);
    EROA(AND_W, "A2,D7",             0147112);
    TEST(AND_W, "(A2),D7",           0147122);
    TEST(AND_W, "(A2)+,D7",          0147132);
    TEST(AND_W, "-(A2),D7",          0147142);
    TEST(AND_W, "($1234,A2),D7",     0147152, 0x1234);
    TEST(AND_W, "(18,A2,D3.L),D7",   0147162, 0x3812);
    TEST(AND_W, "($FFFFFE).W,D7",    0147170, 0xFFFE);
    TEST(AND_W, "($123456).L,D7",    0147171, 0x0012, 0x3456);
    TEST(AND_W, "(*+$1234,PC),D7",   0147172, 0x1232);
    TEST(AND_W, "(*+18,PC,D3.W),D7", 0147173, 0x3010);
    TEST(AND_W, "#$0034,D7",         0147174, 0x0034);
    TEST(AND_L, "D2,D7",             0147202);
    EROA(AND_L, "A2,D7",             0147212);
    TEST(AND_L, "(A2),D7",           0147222);
    TEST(AND_L, "(A2)+,D7",          0147232);
    TEST(AND_L, "-(A2),D7",          0147242);
    TEST(AND_L, "($1234,A2),D7",     0147252, 0x1234);
    TEST(AND_L, "(18,A2,D3.L),D7",   0147262, 0x3812);
    TEST(AND_L, "($FFFFFC).W,D7",    0147270, 0xFFFC);
    TEST(AND_L, "($123454).L,D7",    0147271, 0x0012, 0x3454);
    TEST(AND_L, "(*+$1234,PC),D7",   0147272, 0x1232);
    TEST(AND_L, "(*+18,PC,D3.W),D7", 0147273, 0x3010);
    TEST(AND_L, "#$00345678,D7",     0147274, 0x0034, 0x5678);

    // AND Dn.dst: 014|Dn|Sz|M|Rn, Sz:B=4/W=5/L=6
    TEST(ABCD,  "D2,D7",           0147402); // ABCD
    TEST(ABCD,  "-(A2),-(A7)",     0147412); // ABCD
    TEST(AND_B, "D7,(A2)",         0147422);
    TEST(AND_B, "D7,(A2)+",        0147432);
    TEST(AND_B, "D7,-(A2)",        0147442);
    TEST(AND_B, "D7,($1234,A2)",   0147452, 0x1234);
    TEST(AND_B, "D7,(18,A2,D3.L)", 0147462, 0x3812);
    TEST(AND_B, "D7,($FFFFFF).W",  0147470, 0xFFFF);
    TEST(AND_B, "D7,($123456).L",  0147471, 0x0012, 0x3456);
    EROA(AND_B, "D7,(d16,PC)",     0147472);
    EROA(AND_B, "D7,(d8,PC,Xn)",   0147473);
    EROA(AND_B, "D7,#nnnn",        0147474);
    TEST(EXG,   "D7,D2",           0147502); // EXG
    TEST(EXG,   "A7,A2",           0147512); // EXG
    TEST(AND_W, "D7,(A2)",         0147522);
    TEST(AND_W, "D7,(A2)+",        0147532);
    TEST(AND_W, "D7,-(A2)",        0147542);
    TEST(AND_W, "D7,($1234,A2)",   0147552, 0x1234);
    TEST(AND_W, "D7,(18,A2,D3.L)", 0147562, 0x3812);
    TEST(AND_W, "D7,($FFFFFE).W",  0147570, 0xFFFE);
    TEST(AND_W, "D7,($123456).L",  0147571, 0x0012, 0x3456);
    EROA(AND_W, "D7,(d16,PC)",     0147572);
    EROA(AND_W, "D7,(d8,PC,Xn)",   0147573);
    EROA(AND_W, "D7, #nnnn",       0147574);
    EROA(AND_L, "D7,D2",           0147602);
    TEST(EXG,   "D7,A2",           0147612); // EXG
    TEST(AND_L, "D7,(A2)",         0147622);
    TEST(AND_L, "D7,(A2)+",        0147632);
    TEST(AND_L, "D7,-(A2)",        0147642);
    TEST(AND_L, "D7,($1234,A2)",   0147652, 0x1234);
    TEST(AND_L, "D7,(18,A2,D3.L)", 0147662, 0x3812);
    TEST(AND_L, "D7,($FFFFFC).W",  0147670, 0xFFFC);
    TEST(AND_L, "D7,($123454).L",  0147671, 0x0012, 0x3454);
    EROA(AND_L, "D7,(d16,PC)",     0147672);
    EROA(AND_L, "D7,(d8,PC,Xn)",   0147673);
    EROA(AND_L, "D7,#nnnn",        0147674);

    // ANDI #nn,dst: 0001|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST(ANDI_B, "#18,D2",           0001002, 0x0012);
    EROA(ANDI_B, "#18,A2",           0001012, 0x0012);
    TEST(ANDI_B, "#18,(A2)",         0001022, 0x0012);
    TEST(ANDI_B, "#18,(A2)+",        0001032, 0x0012);
    TEST(ANDI_B, "#18,-(A2)",        0001042, 0x0012);
    TEST(ANDI_B, "#18,($1234,A2)",   0001052, 0x0012, 0x1234);
    TEST(ANDI_B, "#18,(18,A2,D3.W)", 0001062, 0x0012, 0x3012);
    TEST(ANDI_B, "#18,($001234).W",  0001070, 0x0012, 0x1234);
    TEST(ANDI_B, "#18,($123456).L",  0001071, 0x0012, 0x0012, 0x3456);
    EROA(ANDI_B, "#18,(d16,PC)",     0001072, 0x0012);
    EROA(ANDI_B, "#18,(d8,PC,Xn)",   0001073, 0x0012);
    TEST(ANDI,   "#18,CCR",          0001074, 0x0012); // ANDI CCR
    TEST(ANDI_W, "#$5678,D2",           0001102, 0x5678);
    EROA(ANDI_W, "#$5678,A2",           0001112, 0x5678);
    TEST(ANDI_W, "#$5678,(A2)",         0001122, 0x5678);
    TEST(ANDI_W, "#$5678,(A2)+",        0001132, 0x5678);
    TEST(ANDI_W, "#$5678,-(A2)",        0001142, 0x5678);
    TEST(ANDI_W, "#$5678,($1234,A2)",   0001152, 0x5678, 0x1234);
    TEST(ANDI_W, "#$5678,(18,A2,D3.W)", 0001162, 0x5678, 0x3012);
    TEST(ANDI_W, "#$5678,($001234).W",  0001170, 0x5678, 0x1234);
    TEST(ANDI_W, "#$5678,($123456).L",  0001171, 0x5678, 0x0012, 0x3456);
    EROA(ANDI_W, "#$5678,m(d16,PC)",    0001172, 0x5678);
    EROA(ANDI_W, "#$5678,(d8,PC,Xn)",   0001173, 0x5678);
    TEST(ANDI,   "#$5678,SR",           0001174, 0x5678); // ANDI SR
    TEST(ANDI_L, "#$3456789A,D2",           0001202, 0x3456, 0x789A);
    EROA(ANDI_L, "#$3456789A,A2",           0001212, 0x3456, 0x789A);
    TEST(ANDI_L, "#$3456789A,(A2)",         0001222, 0x3456, 0x789A);
    TEST(ANDI_L, "#$3456789A,(A2)+",        0001232, 0x3456, 0x789A);
    TEST(ANDI_L, "#$3456789A,-(A2)",        0001242, 0x3456, 0x789A);
    TEST(ANDI_L, "#$3456789A,($1234,A2)",   0001252, 0x3456, 0x789A, 0x1234);
    TEST(ANDI_L, "#$3456789A,(18,A2,D3.W)", 0001262, 0x3456, 0x789A, 0x3012);
    TEST(ANDI_L, "#$3456789A,($001234).W",  0001270, 0x3456, 0x789A, 0x1234);
    TEST(ANDI_L, "#$34567898,($123454).L",  0001271, 0x3456, 0x7898, 0x0012, 0x3454);
    EROA(ANDI_L, "#$3456789A,(d16,PC)",     0001272, 0x3456, 0x789A);
    EROA(ANDI_L, "#$3456789A,(d8,PC,Xn)",   0001273, 0x3456, 0x789A);
    EROA(ANDI_L, "#$3456789A,#nnnn",        0001274, 0x3456, 0x789A);

    // EOR Dn,dst: 013|Dn|Sz|M|Rn, Sz:B=4/W=5/L=6
    TEST(EOR_B, "D7,D2",           0137402);
    TEST(CMPM_B, "(A2)+,(A7)+",    0137412); // CMPM.B
    TEST(EOR_B, "D7,(A2)",         0137422);
    TEST(EOR_B, "D7,(A2)+",        0137432);
    TEST(EOR_B, "D7,-(A2)",        0137442);
    TEST(EOR_B, "D7,($1234,A2)",   0137452, 0x1234);
    TEST(EOR_B, "D7,(18,A2,D3.L)", 0137462, 0x3812);
    TEST(EOR_B, "D7,($FFFFFF).W",  0137470, 0xFFFF);
    TEST(EOR_B, "D7,($123456).L",  0137471, 0x0012, 0x3456);
    EROA(EOR_B, "D7,(d16,PC)",     0137472);
    EROA(EOR_B, "D7,(d8,PC,Xn)",   0137473);
    EROA(EOR_B, "D7,#nnnn",        0137474);
    TEST(EOR_W,  "D7,D2",          0137502);
    TEST(CMPM_W, "(A2)+,(A7)+",    0137512); // CMPM.W
    TEST(EOR_W, "D7,(A2)",         0137522);
    TEST(EOR_W, "D7,(A2)+",        0137532);
    TEST(EOR_W, "D7,-(A2)",        0137542);
    TEST(EOR_W, "D7,($1234,A2)",   0137552, 0x1234);
    TEST(EOR_W, "D7,(18,A2,D3.L)", 0137562, 0x3812);
    TEST(EOR_W, "D7,($FFFFFE).W",  0137570, 0xFFFE);
    TEST(EOR_W, "D7,($123456).L",  0137571, 0x0012, 0x3456);
    EROA(EOR_W, "D7,(d16,PC)",     0137572);
    EROA(EOR_W, "D7,(d8,PC,Xn)",   0137573);
    EROA(EOR_W, "D7,#nnnn",        0137574);
    TEST(EOR_L,  "D7,D2",          0137602);
    TEST(CMPM_L, "(A2)+,(A7)+",    0137612); // CMPM.L
    TEST(EOR_L, "D7,(A2)",         0137622);
    TEST(EOR_L, "D7,(A2)+",        0137632);
    TEST(EOR_L, "D7,-(A2)",        0137642);
    TEST(EOR_L, "D7,($1234,A2)",   0137652, 0x1234);
    TEST(EOR_L, "D7,(18,A2,D3.L)", 0137662, 0x3812);
    TEST(EOR_L, "D7,($FFFFFC).W",  0137670, 0xFFFC);
    TEST(EOR_L, "D7,($123454).L",  0137671, 0x0012, 0x3454);
    EROA(EOR_L, "D7,(d16,PC)",     0137672);
    EROA(EOR_L, "D7,(d8,PC,Xn)",   0137673);
    EROA(EOR_L, "D7,#nnnn",        0137674);

    // EORI #nn,dst: 0005|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST(EORI_B, "#18,D2",           0005002, 0x0012);
    EROA(EORI_B, "#18,A2",           0005012, 0x0012);
    TEST(EORI_B, "#18,(A2)",         0005022, 0x0012);
    TEST(EORI_B, "#18,(A2)+",        0005032, 0x0012);
    TEST(EORI_B, "#18,-(A2)",        0005042, 0x0012);
    TEST(EORI_B, "#18,($1234,A2)",   0005052, 0x0012, 0x1234);
    TEST(EORI_B, "#18,(18,A2,D3.W)", 0005062, 0x0012, 0x3012);
    TEST(EORI_B, "#18,($001234).W",  0005070, 0x0012, 0x1234);
    TEST(EORI_B, "#18,($123456).L",  0005071, 0x0012, 0x0012, 0x3456);
    EROA(EORI_B, "#18,(d16,PC)",     0005072, 0x0012);
    EROA(EORI_B, "#18,(d8,PC,Xn)",   0005073, 0x0012);
    TEST(EORI,   "#18,CCR",          0005074, 0x0012); // EORI CCR
    TEST(EORI_W, "#$5678,D2",           0005102, 0x5678);
    EROA(EORI_W, "#$5678,A2",           0005112, 0x5678);
    TEST(EORI_W, "#$5678,(A2)",         0005122, 0x5678);
    TEST(EORI_W, "#$5678,(A2)+",        0005132, 0x5678);
    TEST(EORI_W, "#$5678,-(A2)",        0005142, 0x5678);
    TEST(EORI_W, "#$5678,($1234,A2)",   0005152, 0x5678, 0x1234);
    TEST(EORI_W, "#$5678,(18,A2,D3.W)", 0005162, 0x5678, 0x3012);
    TEST(EORI_W, "#$5678,($001234).W",  0005170, 0x5678, 0x1234);
    TEST(EORI_W, "#$5678,($123456).L",  0005171, 0x5678, 0x0012, 0x3456);
    EROA(EORI_W, "#$5678,(d16,PC)",     0005172, 0x5678);
    EROA(EORI_W, "#$5678,(d8,PC,Xn)",   0005173, 0x5678);
    TEST(EORI,   "#$5678,SR",           0005174, 0x5678); // EORI SR
    TEST(EORI_L, "#$3456789A,D2",           0005202, 0x3456, 0x789A);
    EROA(EORI_L, "#$3456789A,A2",           0005212, 0x3456, 0x789A);
    TEST(EORI_L, "#$3456789A,(A2)",         0005222, 0x3456, 0x789A);
    TEST(EORI_L, "#$3456789A,(A2)+",        0005232, 0x3456, 0x789A);
    TEST(EORI_L, "#$3456789A,-(A2)",        0005242, 0x3456, 0x789A);
    TEST(EORI_L, "#$3456789A,($1234,A2)",   0005252, 0x3456, 0x789A, 0x1234);
    TEST(EORI_L, "#$3456789A,(18,A2,D3.W)", 0005262, 0x3456, 0x789A, 0x3012);
    TEST(EORI_L, "#$3456789A,($001234).W",  0005270, 0x3456, 0x789A, 0x1234);
    TEST(EORI_L, "#$34567898,($123454).L",  0005271, 0x3456, 0x7898, 0x0012, 0x3454);
    EROA(EORI_L, "#$3456789A,(d16,PC)",     0005272, 0x3456, 0x789A);
    EROA(EORI_L, "#$3456789A,(d8,PC,Xn)",   0005273, 0x3456, 0x789A);
    EROA(EORI_L, "#$3456789A,#nnnn",        0005274, 0x3456, 0x789A);

    // NOT dst: 0043|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST(NOT_B, "D2",           0043002);
    EROA(NOT_B, "A2",           0043012);
    TEST(NOT_B, "(A2)",         0043022);
    TEST(NOT_B, "(A2)+",        0043032);
    TEST(NOT_B, "-(A2)",        0043042);
    TEST(NOT_B, "($1234,A2)",   0043052, 0x1234);
    TEST(NOT_B, "(18,A2,D3.W)", 0043062, 0x3012);
    TEST(NOT_B, "($001234).W",  0043070, 0x1234);
    TEST(NOT_B, "($123456).L",  0043071, 0x0012, 0x3456);
    EROA(NOT_B, "(d16,PC)",     0043072);
    EROA(NOT_B, "(d8,PC,Xn)",   0043073);
    EROA(NOT_B, "#nnnn",        0043074);
    TEST(NOT_W, "D2",           0043102);
    EROA(NOT_W, "A2",           0043112);
    TEST(NOT_W, "(A2)",         0043122);
    TEST(NOT_W, "(A2)+",        0043132);
    TEST(NOT_W, "-(A2)",        0043142);
    TEST(NOT_W, "($1234,A2)",   0043152, 0x1234);
    TEST(NOT_W, "(18,A2,D3.W)", 0043162, 0x3012);
    TEST(NOT_W, "($001234).W",  0043170, 0x1234);
    TEST(NOT_W, "($123456).L",  0043171, 0x0012, 0x3456);
    EROA(NOT_W, "(d16,PC)",     0043172);
    EROA(NOT_W, "(d8,PC,Xn)",   0043173);
    EROA(NOT_W, "#nnnn",        0043174);
    TEST(NOT_L, "D2",           0043202);
    EROA(NOT_L, "A2",           0043212);
    TEST(NOT_L, "(A2)",         0043222);
    TEST(NOT_L, "(A2)+",        0043232);
    TEST(NOT_L, "-(A2)",        0043242);
    TEST(NOT_L, "($1234,A2)",   0043252, 0x1234);
    TEST(NOT_L, "(18,A2,D3.W)", 0043262, 0x3012);
    TEST(NOT_L, "($001234).W",  0043270, 0x1234);
    TEST(NOT_L, "($123454).L",  0043271, 0x0012, 0x3454);
    EROA(NOT_L, "(d16,PC)",     0043272);
    EROA(NOT_L, "(d8,PC,Xn)",   0043273);
    EROA(NOT_L, "#nnnn",        0043274);

    // OR src,Dn, 010|Dn|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST(OR_B, "D2,D7",             0107002);
    EROA(OR_B, "A2,D7",             0107012);
    TEST(OR_B, "(A2),D7",           0107022);
    TEST(OR_B, "(A2)+,D7",          0107032);
    TEST(OR_B, "-(A2),D7",          0107042);
    TEST(OR_B, "($1234,A2),D7",     0107052, 0x1234);
    TEST(OR_B, "(18,A2,D3.L),D7",   0107062, 0x3812);
    TEST(OR_B, "($FFFFFF).W,D7",    0107070, 0xFFFF);
    TEST(OR_B, "($123456).L,D7",    0107071, 0x0012, 0x3456);
    TEST(OR_B, "(*+$1234,PC),D7",   0107072, 0x1232);
    TEST(OR_B, "(*+18,PC,D3.W),D7", 0107073, 0x3010);
    TEST(OR_B, "#$34,D7",           0107074, 0x0034);
    TEST(OR_W, "D2,D7",             0107102);
    EROA(OR_W, "A2,D7",             0107112);
    TEST(OR_W, "(A2),D7",           0107122);
    TEST(OR_W, "(A2)+,D7",          0107132);
    TEST(OR_W, "-(A2),D7",          0107142);
    TEST(OR_W, "($1234,A2),D7",     0107152, 0x1234);
    TEST(OR_W, "(18,A2,D3.L),D7",   0107162, 0x3812);
    TEST(OR_W, "($FFFFFE).W,D7",    0107170, 0xFFFE);
    TEST(OR_W, "($123456).L,D7",    0107171, 0x0012, 0x3456);
    TEST(OR_W, "(*+$1234,PC),D7",   0107172, 0x1232);
    TEST(OR_W, "(*+18,PC,D3.W),D7", 0107173, 0x3010);
    TEST(OR_W, "#$0034,D7",         0107174, 0x0034);
    TEST(OR_L, "D2,D7",             0107202);
    EROA(OR_L, "A2,D7",             0107212);
    TEST(OR_L, "(A2),D7",           0107222);
    TEST(OR_L, "(A2)+,D7",          0107232);
    TEST(OR_L, "-(A2),D7",          0107242);
    TEST(OR_L, "($1234,A2),D7",     0107252, 0x1234);
    TEST(OR_L, "(18,A2,D3.L),D7",   0107262, 0x3812);
    TEST(OR_L, "($FFFFFC).W,D7",    0107270, 0xFFFC);
    TEST(OR_L, "($123454).L,D7",    0107271, 0x0012, 0x3454);
    TEST(OR_L, "(*+$1234,PC),D7",   0107272, 0x1232);
    TEST(OR_L, "(*+18,PC,D3.W),D7", 0107273, 0x3010);
    TEST(OR_L, "#$00345678,D7",     0107274, 0x0034, 0x5678);

    // OR Dn,dst: 010|Dn|Sz|M|Rn, Sz:B=4/W=5/L=6
    TEST(SBCD, "D2,D7",           0107402); // SBCD
    TEST(SBCD, "-(A2),-(A7)",     0107412); // SBCD
    TEST(OR_B, "D7,(A2)",         0107422);
    TEST(OR_B, "D7,(A2)+",        0107432);
    TEST(OR_B, "D7,-(A2)",        0107442);
    TEST(OR_B, "D7,($1234,A2)",   0107452, 0x1234);
    TEST(OR_B, "D7,(18,A2,D3.L)", 0107462, 0x3812);
    TEST(OR_B, "D7,($FFFFFF).W",  0107470, 0xFFFF);
    TEST(OR_B, "D7,($123456).L",  0107471, 0x0012, 0x3456);
    EROA(OR_B, "D7,(d16,PC)",     0107472);
    EROA(OR_B, "D7,(d8,PC,Xn)",   0107473);
    EROA(OR_B, "D7,#nnnn",        0107474);
    EROA(OR_W, "D7,D2",           0107502);
    EROA(OR_W, "D7,A2",           0107512);
    TEST(OR_W, "D7,(A2)",         0107522);
    TEST(OR_W, "D7,(A2)+",        0107532);
    TEST(OR_W, "D7,-(A2)",        0107542);
    TEST(OR_W, "D7,($1234,A2)",   0107552, 0x1234);
    TEST(OR_W, "D7,(18,A2,D3.L)", 0107562, 0x3812);
    TEST(OR_W, "D7,($FFFFFE).W",  0107570, 0xFFFE);
    TEST(OR_W, "D7,($123456).L",  0107571, 0x0012, 0x3456);
    EROA(OR_W, "D7,(d16,PC)",     0107572);
    EROA(OR_W, "D7,(d8,PC,Xn)",   0107573);
    EROA(OR_W, "D7,#nnnn",        0107574);
    EROA(OR_L, "D7,D2",           0107602);
    EROA(OR_L, "D7,A2",           0107612);
    TEST(OR_L, "D7,(A2)",         0107622);
    TEST(OR_L, "D7,(A2)+",        0107632);
    TEST(OR_L, "D7,-(A2)",        0107642);
    TEST(OR_L, "D7,($1234,A2)",   0107652, 0x1234);
    TEST(OR_L, "D7,(18,A2,D3.L)", 0107662, 0x3812);
    TEST(OR_L, "D7,($FFFFFC).W",  0107670, 0xFFFC);
    TEST(OR_L, "D7,($123454).L",  0107671, 0x0012, 0x3454);
    EROA(OR_L, "D7,(d16,PC)",     0107672);
    EROA(OR_L, "D7,(d8,PC,Xn)",   0107673);
    EROA(OR_L, "D7,#nnnn",        0107674);

    // ORI #nn,dst: 0000|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST(ORI_B, "#18,D2",           0000002, 0x0012);
    EROA(ORI_B, "#18,A2",           0000012, 0x0012);
    TEST(ORI_B, "#18,(A2)",         0000022, 0x0012);
    TEST(ORI_B, "#18,(A2)+",        0000032, 0x0012);
    TEST(ORI_B, "#18,-(A2)",        0000042, 0x0012);
    TEST(ORI_B, "#18,($1234,A2)",   0000052, 0x0012, 0x1234);
    TEST(ORI_B, "#18,(18,A2,D3.W)", 0000062, 0x0012, 0x3012);
    TEST(ORI_B, "#18,($001234).W",  0000070, 0x0012, 0x1234);
    TEST(ORI_B, "#18,($123456).L",  0000071, 0x0012, 0x0012, 0x3456);
    EROA(ORI_B, "#18,(d16,PC)",     0000072, 0x0012);
    EROA(ORI_B, "#18,(d8,PC,Xn)",   0000073, 0x0012);
    TEST(ORI,   "#18,CCR",          0000074, 0x0012); // ORI CCR
    TEST(ORI_W, "#$5678,D2",           0000102, 0x5678);
    EROA(ORI_W, "#$5678,A2",           0000112, 0x5678);
    TEST(ORI_W, "#$5678,(A2)",         0000122, 0x5678);
    TEST(ORI_W, "#$5678,(A2)+",        0000132, 0x5678);
    TEST(ORI_W, "#$5678,-(A2)",        0000142, 0x5678);
    TEST(ORI_W, "#$5678,($1234,A2)",   0000152, 0x5678, 0x1234);
    TEST(ORI_W, "#$5678,(18,A2,D3.W)", 0000162, 0x5678, 0x3012);
    TEST(ORI_W, "#$5678,($001234).W",  0000170, 0x5678, 0x1234);
    TEST(ORI_W, "#$5678,($123456).L",  0000171, 0x5678, 0x0012, 0x3456);
    EROA(ORI_W, "#$5678,(d16,PC)",     0000172, 0x5678);
    EROA(ORI_W, "#$5678,(d8,PC,Xn)",   0000173, 0x5678);
    TEST(ORI,   "#$5678,SR",           0000174, 0x5678); // ORI SR
    TEST(ORI_L, "#$3456789A,D2",           0000202, 0x3456, 0x789A);
    EROA(ORI_L, "#$3456789A,A2",           0000212, 0x3456, 0x789A);
    TEST(ORI_L, "#$3456789A,(A2)",         0000222, 0x3456, 0x789A);
    TEST(ORI_L, "#$3456789A,(A2)+",        0000232, 0x3456, 0x789A);
    TEST(ORI_L, "#$3456789A,-(A2)",        0000242, 0x3456, 0x789A);
    TEST(ORI_L, "#$3456789A,($1234,A2)",   0000252, 0x3456, 0x789A, 0x1234);
    TEST(ORI_L, "#$3456789A,(18,A2,D3.W)", 0000262, 0x3456, 0x789A, 0x3012);
    TEST(ORI_L, "#$3456789A,($001234).W",  0000270, 0x3456, 0x789A, 0x1234);
    TEST(ORI_L, "#$34567898,($123454).L",  0000271, 0x3456, 0x7898, 0x0012, 0x3454);
    EROA(ORI_L, "#$3456789A,(d16,PC)",     0000272, 0x3456, 0x789A);
    EROA(ORI_L, "#$3456789A,(d8,PC,Xn)",   0000273, 0x3456, 0x789A);
    EROA(ORI_L, "#$3456789A,#nnnn",        0000274, 0x3456, 0x789A);
}

static void test_shift_rotate() {
    // ASL Dx,Dy: 016|Dx|Sz|4|Dy, Sz:B=4/W=5/L=6
    TEST(ASL_B, "D2,D7", 0162447);
    TEST(ASL_W, "D2,D7", 0162547);
    TEST(ASL_L, "D2,D7", 0162647);

    // ASL #nn,Dy: 016|nn|Sz|0|Dy, Sz:B=4/W=5/L=6
    TEST(ASL_B, "#1,D7", 0161407);
    TEST(ASL_W, "#4,D7", 0164507);
    TEST(ASL_L, "#8,D7", 0160607);

    // ASL dst: 01607|M|Rn
    EROA(ASL, "D2",            0160702);
    EROA(ASL, "A2",            0160712);
    TEST(ASL, "(A2)",          0160722);
    TEST(ASL, "(A2)+",         0160732);
    TEST(ASL, "-(A2)",         0160742);
    TEST(ASL, "($2345,A2)",    0160752, 0x2345);
    TEST(ASL, "($23,A2,D3.L)", 0160762, 0x3823);
    TEST(ASL, "($002346).W",   0160770, 0x2346);
    TEST(ASL, "($234568).L",   0160771, 0x0023, 0x4568);
    EROA(ASL, "(d16,PC)",      0160772);
    EROA(ASL, "(d8,PC,Xn)",    0160773);
    EROA(ASL, "#nnnn",         0160774);

    // ASR Dx,Dy: 016|Dx|Sz|4|Dy, Sz:B=0/W=1/L=2
    TEST(ASR_B, "D2,D7", 0162047);
    TEST(ASR_W, "D2,D7", 0162147);
    TEST(ASR_L, "D2,D7", 0162247);

    // ASR #nn,Dy: 016|Dx|Sz|0|Dy, Sz:B=0/W=1/L=2
    TEST(ASR_B, "#1,D7", 0161007);
    TEST(ASR_W, "#4,D7", 0164107);
    TEST(ASR_L, "#8,D7", 0160207);

    // ASR dst: 01603|M|Rn
    EROA(ASR, "D2",            0160302);
    EROA(ASR, "A2",            0160312);
    TEST(ASR, "(A2)",          0160322);
    TEST(ASR, "(A2)+",         0160332);
    TEST(ASR, "-(A2)",         0160342);
    TEST(ASR, "($2345,A2)",    0160352, 0x2345);
    TEST(ASR, "($23,A2,D3.L)", 0160362, 0x3823);
    TEST(ASR, "($002346).W",   0160370, 0x2346);
    TEST(ASR, "($234568).L",   0160371, 0x0023, 0x4568);
    EROA(ASR, "(d16,PC)",      0160372);
    EROA(ASR, "(d8,PC,Xn)",    0160373);
    EROA(ASR, "#nnnn",         0160374);

    // LSL Dx,Dy: 016|Dx|Sz|5|Dy, Sz:B=4/W=5/L=6
    TEST(LSL_B, "D2,D7", 0162457);
    TEST(LSL_W, "D2,D7", 0162557);
    TEST(LSL_L, "D2,D7", 0162657);

    // LSL #n,Dy: 016|nn|Sz|1|Dy: Sz:B=4/W=5/L=6
    TEST(LSL_B, "#1,D7", 0161417);
    TEST(LSL_W, "#4,D7", 0164517);
    TEST(LSL_L, "#8,D7", 0160617);

    // LSL dst: 01617|M|Rn
    EROA(LSL, "D2",            0161702);
    EROA(LSL, "A2",            0161712);
    TEST(LSL, "(A2)",          0161722);
    TEST(LSL, "(A2)+",         0161732);
    TEST(LSL, "-(A2)",         0161742);
    TEST(LSL, "($2345,A2)",    0161752, 0x2345);
    TEST(LSL, "($23,A2,D3.L)", 0161762, 0x3823);
    TEST(LSL, "($002346).W",   0161770, 0x2346);
    TEST(LSL, "($234568).L",   0161771, 0x0023, 0x4568);
    EROA(LSL, "(d16,PC)",      0161772);
    EROA(LSL, "(d8,PC,Xn)",    0161773);
    EROA(LSL, "#nnnn",         0161774);

    // LSR Dx,Dy: 016|Dx|Sz|5|Dy, Sz:B=0/W=1/L=2
    TEST(LSR_B, "D2,D7", 0162057);
    TEST(LSR_W, "D2,D7", 0162157);
    TEST(LSR_L, "D2,D7", 0162257);

    // LSR #n,Dy: 016|nn|Sz|1|Dy, Sz:B=0/W=1/L=2
    TEST(LSR_B, "#1,D7", 0161017);
    TEST(LSR_W, "#4,D7", 0164117);
    TEST(LSR_L, "#8,D7", 0160217);

    // LSR dst: 01613|M|Rn
    EROA(LSR, "D2",            0161302);
    EROA(LSR, "A2",            0161312);
    TEST(LSR, "(A2)",          0161322);
    TEST(LSR, "(A2)+",         0161332);
    TEST(LSR, "-(A2)",         0161342);
    TEST(LSR, "($2345,A2)",    0161352, 0x2345);
    TEST(LSR, "($23,A2,D3.L)", 0161362, 0x3823);
    TEST(LSR, "($002346).W",   0161370, 0x2346);
    TEST(LSR, "($234568).L",   0161371, 0x0023, 0x4568);
    EROA(LSR, "(d16,PC)",      0161372);
    EROA(LSR, "(d8,PC,Xn)",    0161373);
    EROA(LSR, "#nnnn",         0161374);

    // ROL Dx,Dy: 016|Dx|Sz|7|Dy, Sz:B=4/W=5/L=6
    TEST(ROL_B, "D2,D7", 0162477);
    TEST(ROL_W, "D2,D7", 0162577);
    TEST(ROL_L, "D2,D7", 0162677);

    // ROL #n,Dy: 016|nn|Sz|3|Dy, Sz:B=4/W=5/L=6
    TEST(ROL_B, "#1,D7", 0161437);
    TEST(ROL_W, "#4,D7", 0164537);
    TEST(ROL_L, "#8,D7", 0160637);

    // ROL dst: 01637|M|Rn
    EROA(ROL, "D2",            0163702);
    EROA(ROL, "A2",            0163712);
    TEST(ROL, "(A2)",          0163722);
    TEST(ROL, "(A2)+",         0163732);
    TEST(ROL, "-(A2)",         0163742);
    TEST(ROL, "($2345,A2)",    0163752, 0x2345);
    TEST(ROL, "($23,A2,D3.L)", 0163762, 0x3823);
    TEST(ROL, "($002346).W",   0163770, 0x2346);
    TEST(ROL, "($234568).L",   0163771, 0x0023, 0x4568);
    EROA(ROL, "(d16,PC)",      0163772);
    EROA(ROL, "(d8,PC,Xn)",    0163773);
    EROA(ROL, "#nnnn",         0163774);

    // ROR Dx,Dy: 016|Dx|Sz|7|Dy, Sz:B=0/W=1/L=2
    TEST(ROR_B, "D2,D7", 0162077);
    TEST(ROR_W, "D2,D7", 0162177);
    TEST(ROR_L, "D2,D7", 0162277);

    // ROR #n,Dy: 016|Dx|Sz|3|Dy, Sz:B=0/W=1/L=2
    TEST(ROR_B, "#1,D7", 0161037);
    TEST(ROR_W, "#4,D7", 0164137);
    TEST(ROR_L, "#8,D7", 0160237);

    // ROR dst: 01633|M|Rn
    EROA(ROR, "D2",            0163302);
    EROA(ROR, "A2",            0163312);
    TEST(ROR, "(A2)",          0163322);
    TEST(ROR, "(A2)+",         0163332);
    TEST(ROR, "-(A2)",         0163342);
    TEST(ROR, "($2345,A2)",    0163352, 0x2345);
    TEST(ROR, "($23,A2,D3.L)", 0163362, 0x3823);
    TEST(ROR, "($002346).W",   0163370, 0x2346);
    TEST(ROR, "($234568).L",   0163371, 0x0023, 0x4568);
    EROA(ROR, "(d16,PC)",      0163372);
    EROA(ROR, "(d8,PC,Xn)",    0163373);
    EROA(ROR, "#nnnn",         0163374);

    // ROXL Dx,Dy: 016|Dx|Sz|6|Dy, Sz:B=4/W=5/L=6
    TEST(ROXL_B, "D2,D7", 0162467);
    TEST(ROXL_W, "D2,D7", 0162567);
    TEST(ROXL_L, "D2,D7", 0162667);

    // ROXL #n,Dy: 016|nn|Sz|2|Dy, Sz:B=4/W=5/L=6
    TEST(ROXL_B, "#1,D7", 0161427);
    TEST(ROXL_W, "#4,D7", 0164527);
    TEST(ROXL_L, "#8,D7", 0160627);

    // ROXL dst: 01627|M|Rn
    EROA(ROXL, "D2",            0162702);
    EROA(ROXL, "A2",            0162712);
    TEST(ROXL, "(A2)",          0162722);
    TEST(ROXL, "(A2)+",         0162732);
    TEST(ROXL, "-(A2)",         0162742);
    TEST(ROXL, "($2345,A2)",    0162752, 0x2345);
    TEST(ROXL, "($23,A2,D3.L)", 0162762, 0x3823);
    TEST(ROXL, "($002346).W",   0162770, 0x2346);
    TEST(ROXL, "($234568).L",   0162771, 0x0023, 0x4568);
    EROA(ROXL, "(d16,PC)",      0162772);
    EROA(ROXL, "(d8,PC,Xn)",    0162773);
    EROA(ROXL, "#nnnn",         0162774);

    // ROXR Dx,Dy: 016|Dx|Sz|6|Dy, Sz:B=0/W=1/L=2
    TEST(ROXR_B, "D2,D7", 0162067);
    TEST(ROXR_W, "D2,D7", 0162167);
    TEST(ROXR_L, "D2,D7", 0162267);

    // ROXR #n,Dy: 016|nn|Sz|2|Dy, Sz:B=0/W=1/L=2
    TEST(ROXR_B, "#1,D7", 0161027);
    TEST(ROXR_W, "#4,D7", 0164127);
    TEST(ROXR_L, "#8,D7", 0160227);

    // ROXR dst: 01623|M|Rn
    EROA(ROXR, "D2",            0162302);
    EROA(ROXR, "A2",            0162312);
    TEST(ROXR, "(A2)",          0162322);
    TEST(ROXR, "(A2)+",         0162332);
    TEST(ROXR, "-(A2)",         0162342);
    TEST(ROXR, "($2345,A2)",    0162352, 0x2345);
    TEST(ROXR, "($23,A2,D3.L)", 0162362, 0x3823);
    TEST(ROXR, "($002346).W",   0162370, 0x2346);
    TEST(ROXR, "($234568).L",   0162371, 0x0023, 0x4568);
    EROA(ROXR, "(d16,PC)",      0162372);
    EROA(ROXR, "(d8,PC,Xn)",    0162373);
    EROA(ROXR, "#nnnn",         0162374);

    // SWAP Dn: 004410|Dn
    TEST(SWAP, "D0", 0044100);
    TEST(SWAP, "D7", 0044107);
}

static void test_bit() {
    // BCHG Dx,dst: 000|Dx|5|M|Rn
    TEST(BCHG_L, "D7,D2",           0007502);
    TEST(MOVEP_L, "($1234,A2),D7",  0007512, 0x1234); // MOVEP.L
    TEST(BCHG_B, "D7,(A2)",         0007522);
    TEST(BCHG_B, "D7,(A2)+",        0007532);
    TEST(BCHG_B, "D7,-(A2)",        0007542);
    TEST(BCHG_B, "D7,($1234,A2)",   0007552, 0x1234);
    TEST(BCHG_B, "D7,(18,A2,D3.W)", 0007562, 0x3012);
    TEST(BCHG_B, "D7,($001234).W",  0007570, 0x1234);
    TEST(BCHG_B, "D7,($012345).L",  0007571, 0x0001, 0x2345);
    EROA(BCHG_B, "D7,(d16,PC)",     0007572);
    EROA(BCHG_B, "D7,(d8,PC,Xn)",   0007573);
    EROA(BCHG_B, "D7,#nnnn",        0007574);

    // BCHG #n,dst: 00041|M|Rn
    TEST(BCHG_L, "#0,D2",           0004102, 0x0000);
    TEST(BCHG_L, "#31,D2",          0004102, 0x001F);
    ERBN(BCHG_L, "#32,D2",          0004102, 0x0020);
    EROA(BCHG_L, "#6,A2",           0004112, 0x0006);
    TEST(BCHG_B, "#7,(A2)",         0004122, 0x0007);
    ERBN(BCHG_B, "#8,(A2)",         0004122, 0x0008);
    TEST(BCHG_B, "#6,(A2)+",        0004132, 0x0006);
    TEST(BCHG_B, "#5,-(A2)",        0004142, 0x0005);
    TEST(BCHG_B, "#4,($1234,A2)",   0004152, 0x0004, 0x1234);
    TEST(BCHG_B, "#3,(18,A2,D3.W)", 0004162, 0x0003, 0x3012);
    TEST(BCHG_B, "#2,($001234).W",  0004170, 0x0002, 0x1234);
    TEST(BCHG_B, "#1,($012345).L",  0004171, 0x0001, 0x0001, 0x2345);
    EROA(BCHG_B, "#0,(d16,PC)",     0004172, 0x0000);
    EROA(BCHG_B, "#7,(d8,PC,Xn)",   0004173, 0x0007);
    EROA(BCHG_B, "#6,#nnnn",        0004174, 0x0006);

    // BCLR Dx,dst: 000|Dx|6|M|Rn
    TEST(BCLR_L, "D7,D2",           0007602);
    TEST(MOVEP_W, "D7,($1234,A2)",  0007612, 0x1234); // MOVEP.W
    TEST(BCLR_B, "D7,(A2)",         0007622);
    TEST(BCLR_B, "D7,(A2)+",        0007632);
    TEST(BCLR_B, "D7,-(A2)",        0007642);
    TEST(BCLR_B, "D7,($1234,A2)",   0007652, 0x1234);
    TEST(BCLR_B, "D7,(18,A2,D3.W)", 0007662, 0x3012);
    TEST(BCLR_B, "D7,($001234).W",  0007670, 0x1234);
    TEST(BCLR_B, "D7,($012345).L",  0007671, 0x0001, 0x2345);
    EROA(BCLR_B, "D7,(d16,PC)",     0007672);
    EROA(BCLR_B, "D7,(d8,PC,Xn)",   0007673);
    EROA(BCLR_B, "D7,#nnnn",        0007674);

    // BCLR #n,dst: 00042|M|Rn
    TEST(BCLR_L, "#0,D2",           0004202, 0x0000);
    EROA(BCLR_L, "#1,A2",           0004212, 0x0001);
    TEST(BCLR_B, "#7,(A2)",         0004222, 0x0007);
    TEST(BCLR_B, "#6,(A2)+",        0004232, 0x0006);
    TEST(BCLR_B, "#5,-(A2)",        0004242, 0x0005);
    TEST(BCLR_B, "#4,($1234,A2)",   0004252, 0x0004, 0x1234);
    TEST(BCLR_B, "#3,(18,A2,D3.W)", 0004262, 0x0003, 0x3012);
    TEST(BCLR_B, "#2,($001234).W",  0004270, 0x0002, 0x1234);
    TEST(BCLR_B, "#1,($012345).L",  0004271, 0x0001, 0x0001, 0x2345);
    EROA(BCLR_B, "#0,(d16,PC)",     0004272, 0x0000);
    EROA(BCLR_B, "#7,(d8,PC,Xn)",   0004273, 0x0007);
    EROA(BCLR_B, "#6,#nnnn",        0004274, 0x0006);

    // BSET Dx,dst: 000|Dx|7|M|Rn
    TEST(BSET_L, "D7,D2",           0007702);
    TEST(MOVEP_L, "D7,($1234,A2)",  0007712, 0x1234); // MOVEP.L
    TEST(BSET_B, "D7,(A2)",         0007722);
    TEST(BSET_B, "D7,(A2)+",        0007732);
    TEST(BSET_B, "D7,-(A2)",        0007742);
    TEST(BSET_B, "D7,($1234,A2)",   0007752, 0x1234);
    TEST(BSET_B, "D7,(18,A2,D3.W)", 0007762, 0x3012);
    TEST(BSET_B, "D7,($001234).W",  0007770, 0x1234);
    TEST(BSET_B, "D7,($012345).L",  0007771, 0x0001, 0x2345);
    EROA(BSET_B, "D7,(d16,PC)",     0007772);
    EROA(BSET_B, "D7,(d8,PC,Xn)",   0007773);
    EROA(BSET_B, "D7,#nnnn",        0007774);

    // BSET #n,dst: 00043|M|Rn
    TEST(BSET_L, "#0,D2",           0004302, 0x0000);
    EROA(BSET_L, "#1,A2",           0004312, 0x0001);
    TEST(BSET_B, "#7,(A2)",         0004322, 0x0007);
    TEST(BSET_B, "#6,(A2)+",        0004332, 0x0006);
    TEST(BSET_B, "#5,-(A2)",        0004342, 0x0005);
    TEST(BSET_B, "#4,($1234,A2)",   0004352, 0x0004, 0x1234);
    TEST(BSET_B, "#3,(18,A2,D3.W)", 0004362, 0x0003, 0x3012);
    TEST(BSET_B, "#2,($001234).W",  0004370, 0x0002, 0x1234);
    TEST(BSET_B, "#1,($012345).L",  0004371, 0x0001, 0x0001, 0x2345);
    EROA(BSET_B, "#0,(d16,PC)",     0004372, 0x0000);
    EROA(BSET_B, "#7,(d8,PC,Xn)",   0004373, 0x0007);
    EROA(BSET_B, "#6,#nnnn",        0004374, 0x0006);

    // BTST Dx,dst: 000|Dx|4|M|Rn
    TEST(BTST_L, "D7,D2",             0007402);
    TEST(MOVEP_W, "($1234,A2),D7",    0007412, 0x1234); // MOVEP.W
    TEST(BTST_B, "D7,(A2)",           0007422);
    TEST(BTST_B, "D7,(A2)+",          0007432);
    TEST(BTST_B, "D7,-(A2)",          0007442);
    TEST(BTST_B, "D7,($1234,A2)",     0007452, 0x1234);
    TEST(BTST_B, "D7,(18,A2,D3.W)",   0007462, 0x3012);
    TEST(BTST_B, "D7,($001234).W",    0007470, 0x1234);
    TEST(BTST_B, "D7,($012345).L",    0007471, 0x0001, 0x2345);
    TEST(BTST_B, "D7,(*+$1234,PC)",   0007472, 0x1232);
    TEST(BTST_B, "D7,(*+35,PC,D3.L)", 0007473, 0x3821);
    EROA(BTST_B, "D7,#nnnn",          0007474);

    // BTST #n,dst: 00040|M|Rn
    TEST(BTST_L, "#0,D2",             0004002, 0x0000);
    EROA(BTST_L, "#1,A2",             0004012, 0x0001);
    TEST(BTST_B, "#7,(A2)",           0004022, 0x0007);
    TEST(BTST_B, "#6,(A2)+",          0004032, 0x0006);
    TEST(BTST_B, "#5,-(A2)",          0004042, 0x0005);
    TEST(BTST_B, "#4,($1234,A2)",     0004052, 0x0004, 0x1234);
    TEST(BTST_B, "#3,(18,A2,D3.W)",   0004062, 0x0003, 0x3012);
    TEST(BTST_B, "#2,($001234).W",    0004070, 0x0002, 0x1234);
    TEST(BTST_B, "#1,($012345).L",    0004071, 0x0001, 0x0001, 0x2345);
    TEST(BTST_B, "#0,(*+$1234,PC)",   0004072, 0x0000, 0x1230);
    TEST(BTST_B, "#7,(*+35,PC,D3.L)", 0004073, 0x0007, 0x381F);
    EROA(BTST_B, "#6,#nnnn",          0004074, 0x0006);
}

static void test_bcd() {
    // ABCD Dx,Dy: 014|Dy|40|Dx
    TEST(ABCD, "D2,D7", 0147402);

    // ABCD -(Ax),-(Ay)" 014|Ay|41|Ax
    TEST(ABCD, "-(A2),-(A7)", 0147412);

    // NBCD dst: 00440|M|Rn
    TEST(NBCD, "D2",           0044002);
    EROA(NBCD, "A2",           0044012);
    TEST(NBCD, "(A2)",         0044022);
    TEST(NBCD, "(A2)+",        0044032);
    TEST(NBCD, "-(A2)",        0044042);
    TEST(NBCD, "($1234,A2)",   0044052, 0x1234);
    TEST(NBCD, "(18,A2,D3.W)", 0044062, 0x3012);
    TEST(NBCD, "($001234).W",  0044070, 0x1234);
    TEST(NBCD, "($123456).L",  0044071, 0x0012, 0x3456);
    EROA(NBCD, "(d16,PC)",     0044072);
    EROA(NBCD, "(d8,PC,Xn)",   0044073);
    EROA(NBCD, "#nnnn",        0044074);

    // SBCD Dx,Dy: 010|Dy|40|Dx
    TEST(SBCD, "D2,D7", 0107402);

    // SBCD -(Ax),-(Ay): 010|Ay|41|Ax
    TEST(SBCD, "-(A2),-(A7)", 0107412);
}

static void test_program() {
    disassembler.setRelativeTarget(true);

    // Bcc label: 006|cc|disp
    TEST(BRA, "*", 0060000 | 0x000 | 0xFE);
    TEST(BSR, "*", 0060000 | 0x100 | 0xFE);
    TEST(BHI, "*", 0060000 | 0x200 | 0xFE);
    TEST(BLS, "*", 0060000 | 0x300 | 0xFE);
    TEST(BCC, "*", 0060000 | 0x400 | 0xFE);
    TEST(BCS, "*", 0060000 | 0x500 | 0xFE);
    TEST(BNE, "*", 0060000 | 0x600 | 0xFE);
    TEST(BEQ, "*", 0060000 | 0x700 | 0xFE);
    TEST(BVC, "*", 0060000 | 0x800 | 0xFE);
    TEST(BVS, "*", 0060000 | 0x900 | 0xFE);
    TEST(BPL, "*", 0060000 | 0xA00 | 0xFE);
    TEST(BMI, "*", 0060000 | 0xB00 | 0xFE);
    TEST(BGE, "*", 0060000 | 0xC00 | 0xFE);
    TEST(BLT, "*", 0060000 | 0xD00 | 0xFE);
    TEST(BGT, "*", 0060000 | 0xE00 | 0xFE);
    TEST(BLE, "*", 0060000 | 0xF00 | 0xFE);

    // DBcc Dn,labelL 005|cc|31|Dn
    TEST(DBRA, "D2,*-$7FFE", 0050312 | 0x100, 0x8000);
    TEST(DBRA, "D2,*-$007E", 0050312 | 0x100, 0xFF80);
    TEST(DBRA, "D2,*",       0050312 | 0x100, 0xFFFE);
    TEST(DBRA, "D2,*+2",     0050312 | 0x100, 0x0000);
    TEST(DBRA, "D2,*+$0080", 0050312 | 0x100, 0x007E);
    TEST(DBRA, "D2,*+$8000", 0050312 | 0x100, 0x7FFE);
    ERNA(DBRA, "D2,*-$7FFF", 0050312 | 0x100, 0x8001);
    ERNA(DBRA, "D2,*+$8001", 0050312 | 0x100, 0x7FFF);
    TEST(DBT,  "D2,*",       0050312 | 0x000, 0xFFFE);
    TEST(DBHI, "D2,*",       0050312 | 0x200, 0xFFFE);
    TEST(DBLS, "D2,*",       0050312 | 0x300, 0xFFFE);
    TEST(DBCC, "D2,*",       0050312 | 0x400, 0xFFFE);
    TEST(DBCS, "D2,*",       0050312 | 0x500, 0xFFFE);
    TEST(DBNE, "D2,*",       0050312 | 0x600, 0xFFFE);
    TEST(DBEQ, "D2,*",       0050312 | 0x700, 0xFFFE);
    TEST(DBVC, "D2,*",       0050312 | 0x800, 0xFFFE);
    TEST(DBVS, "D2,*",       0050312 | 0x900, 0xFFFE);
    TEST(DBPL, "D2,*",       0050312 | 0xA00, 0xFFFE);
    TEST(DBMI, "D2,*",       0050312 | 0xB00, 0xFFFE);
    TEST(DBGE, "D2,*",       0050312 | 0xC00, 0xFFFE);
    TEST(DBLT, "D2,*",       0050312 | 0xD00, 0xFFFE);
    TEST(DBGT, "D2,*",       0050312 | 0xE00, 0xFFFE);
    TEST(DBLE, "D2,*",       0050312 | 0xF00, 0xFFFE);

    // Scc dst: 005|cc|3|M|Rn
    TEST(ST, "D2",           0050302 | 0x000);
    TEST(DBT, "D2,*+2",      0050312 | 0x000, 0); // DBT
    TEST(ST, "(A2)",         0050322 | 0x000);
    TEST(ST, "(A2)+",        0050332 | 0x000);
    TEST(ST, "-(A2)",        0050342 | 0x000);
    TEST(ST, "($1234,A2)",   0050352 | 0x000, 0x1234);
    TEST(ST, "(18,A2,D3.W)", 0050362 | 0x000, 0x3012);
    TEST(ST, "($001234).W",  0050370 | 0x000, 0x1234);
    TEST(ST, "($123456).L",  0050371 | 0x000, 0x0012, 0x3456);
    EROA(ST, "(d16,PC)",     0050372 | 0x000);
    EROA(ST, "(d8,PC,Xn)",   0050373 | 0x000);
    EROA(ST, "#nnnn",        0050374 | 0x000);
    TEST(SF, "D2",           0050302 | 0x100);
    TEST(DBRA, "D2,*+2",     0050312 | 0x100, 0); // DBRA
    TEST(SF, "(A2)",         0050322 | 0x100);
    TEST(SF, "(A2)+",        0050332 | 0x100);
    TEST(SF, "-(A2)",        0050342 | 0x100);
    TEST(SF, "($1234,A2)",   0050352 | 0x100, 0x1234);
    TEST(SF, "(18,A2,D3.W)", 0050362 | 0x100, 0x3012);
    TEST(SF, "($001234).W",  0050370 | 0x100, 0x1234);
    TEST(SF, "($123456).L",  0050371 | 0x100, 0x0012, 0x3456);
    EROA(SF, "(d16,PC)",     0050372 | 0x100);
    EROA(SF, "(d8,PC,Xn)",   0050373 | 0x100);
    EROA(SF, "#nnnn",        0050374 | 0x100);
    TEST(SHI, "D2",           0050302 | 0x200);
    TEST(DBHI, "D2,*+2",      0050312 | 0x200, 0); // DBHI
    TEST(SHI, "(A2)",         0050322 | 0x200);
    TEST(SHI, "(A2)+",        0050332 | 0x200);
    TEST(SHI, "-(A2)",        0050342 | 0x200);
    TEST(SHI, "($1234,A2)",   0050352 | 0x200, 0x1234);
    TEST(SHI, "(18,A2,D3.W)", 0050362 | 0x200, 0x3012);
    TEST(SHI, "($001234).W",  0050370 | 0x200, 0x1234);
    TEST(SHI, "($123456).L",  0050371 | 0x200, 0x0012, 0x3456);
    EROA(SHI, "(d16,PC)",     0050372 | 0x200);
    EROA(SHI, "(d8,PC,Xn)",   0050373 | 0x200);
    EROA(SHI, "#nnnn",        0050374 | 0x200);
    TEST(SLS, "D2",           0050302 | 0x300);
    TEST(DBLS, "D2,*+2",      0050312 | 0x300, 0); // DBLS
    TEST(SLS, "(A2)",         0050322 | 0x300);
    TEST(SLS, "(A2)+",        0050332 | 0x300);
    TEST(SLS, "-(A2)",        0050342 | 0x300);
    TEST(SLS, "($1234,A2)",   0050352 | 0x300, 0x1234);
    TEST(SLS, "(18,A2,D3.W)", 0050362 | 0x300, 0x3012);
    TEST(SLS, "($001234).W",  0050370 | 0x300, 0x1234);
    TEST(SLS, "($123456).L",  0050371 | 0x300, 0x0012, 0x3456);
    EROA(SLS, "(d16,PC)",     0050372 | 0x300);
    EROA(SLS, "(d8,PC,Xn)",   0050373 | 0x300);
    EROA(SLS, "#nnnn",        0050374 | 0x300);
    TEST(SCC, "D2",           0050302 | 0x400);
    TEST(DBCC, "D2,*+2",      0050312 | 0x400, 0); // DBCC
    TEST(SCC, "(A2)",         0050322 | 0x400);
    TEST(SCC, "(A2)+",        0050332 | 0x400);
    TEST(SCC, "-(A2)",        0050342 | 0x400);
    TEST(SCC, "($1234,A2)",   0050352 | 0x400, 0x1234);
    TEST(SCC, "(18,A2,D3.W)", 0050362 | 0x400, 0x3012);
    TEST(SCC, "($001234).W",  0050370 | 0x400, 0x1234);
    TEST(SCC, "($123456).L",  0050371 | 0x400, 0x0012, 0x3456);
    EROA(SCC, "(d16,PC)",     0050372 | 0x400);
    EROA(SCC, "(d8,PC,Xn)",   0050373 | 0x400);
    EROA(SCC, "#nnnn",        0050374 | 0x400);
    TEST(SCS, "D2",           0050302 | 0x500);
    TEST(DBCS, "D2,*+2",      0050312 | 0x500, 0); // DBCS
    TEST(SCS, "(A2)",         0050322 | 0x500);
    TEST(SCS, "(A2)+",        0050332 | 0x500);
    TEST(SCS, "-(A2)",        0050342 | 0x500);
    TEST(SCS, "($1234,A2)",   0050352 | 0x500, 0x1234);
    TEST(SCS, "(18,A2,D3.W)", 0050362 | 0x500, 0x3012);
    TEST(SCS, "($001234).W",  0050370 | 0x500, 0x1234);
    TEST(SCS, "($123456).L",  0050371 | 0x500, 0x0012, 0x3456);
    EROA(SCS, "(d16,PC)",     0050372 | 0x500);
    EROA(SCS, "(d8,PC,Xn)",   0050373 | 0x500);
    EROA(SCS, "#nnnn",        0050374 | 0x500);
    TEST(SNE, "D2",           0050302 | 0x600);
    TEST(DBNE, "D2,*+2",      0050312 | 0x600, 0); // DBNE
    TEST(SNE, "(A2)",         0050322 | 0x600);
    TEST(SNE, "(A2)+",        0050332 | 0x600);
    TEST(SNE, "-(A2)",        0050342 | 0x600);
    TEST(SNE, "($1234,A2)",   0050352 | 0x600, 0x1234);
    TEST(SNE, "(18,A2,D3.W)", 0050362 | 0x600, 0x3012);
    TEST(SNE, "($001234).W",  0050370 | 0x600, 0x1234);
    TEST(SNE, "($123456).L",  0050371 | 0x600, 0x0012, 0x3456);
    EROA(SNE, "(d16,PC)",     0050372 | 0x600);
    EROA(SNE, "(d8,PC,Xn)",   0050373 | 0x600);
    EROA(SNE, "#nnnn",        0050374 | 0x600);
    TEST(SEQ, "D2",           0050302 | 0x700);
    TEST(DBEQ, "D2,*+2",      0050312 | 0x700, 0); // DBEQ
    TEST(SEQ, "(A2)",         0050322 | 0x700);
    TEST(SEQ, "(A2)+",        0050332 | 0x700);
    TEST(SEQ, "-(A2)",        0050342 | 0x700);
    TEST(SEQ, "($1234,A2)",   0050352 | 0x700, 0x1234);
    TEST(SEQ, "(18,A2,D3.W)", 0050362 | 0x700, 0x3012);
    TEST(SEQ, "($001234).W",  0050370 | 0x700, 0x1234);
    TEST(SEQ, "($123456).L",  0050371 | 0x700, 0x0012, 0x3456);
    EROA(SEQ, "(d16,PC)",     0050372 | 0x700);
    EROA(SEQ, "(d8,PC,Xn)",   0050373 | 0x700);
    EROA(SEQ, "#nnnn",        0050374 | 0x700);
    TEST(SVC, "D2",           0050302 | 0x800);
    TEST(DBVC, "D2,*+2",      0050312 | 0x800, 0); // DBVC
    TEST(SVC, "(A2)",         0050322 | 0x800);
    TEST(SVC, "(A2)+",        0050332 | 0x800);
    TEST(SVC, "-(A2)",        0050342 | 0x800);
    TEST(SVC, "($1234,A2)",   0050352 | 0x800, 0x1234);
    TEST(SVC, "(18,A2,D3.W)", 0050362 | 0x800, 0x3012);
    TEST(SVC, "($001234).W",  0050370 | 0x800, 0x1234);
    TEST(SVC, "($123456).L",  0050371 | 0x800, 0x0012, 0x3456);
    EROA(SVC, "(d16,PC)",     0050372 | 0x800);
    EROA(SVC, "(d8,PC,Xn)",   0050373 | 0x800);
    EROA(SVC, "#nnnn",        0050374 | 0x800);
    TEST(SVS, "D2",           0050302 | 0x900);
    TEST(DBVS, "D2,*+2",      0050312 | 0x900, 0); // DBVS
    TEST(SVS, "(A2)",         0050322 | 0x900);
    TEST(SVS, "(A2)+",        0050332 | 0x900);
    TEST(SVS, "-(A2)",        0050342 | 0x900);
    TEST(SVS, "($1234,A2)",   0050352 | 0x900, 0x1234);
    TEST(SVS, "(18,A2,D3.W)", 0050362 | 0x900, 0x3012);
    TEST(SVS, "($001234).W",  0050370 | 0x900, 0x1234);
    TEST(SVS, "($123456).L",  0050371 | 0x900, 0x0012, 0x3456);
    EROA(SVS, "(d16,PC)",     0050372 | 0x900);
    EROA(SVS, "(d8,PC,Xn)",   0050373 | 0x900);
    EROA(SVS, "#nnnn",        0050374 | 0x900);
    TEST(SPL, "D2",           0050302 | 0xA00);
    TEST(DBPL, "D2,*+2",      0050312 | 0xA00, 0); // DBPL
    TEST(SPL, "(A2)",         0050322 | 0xA00);
    TEST(SPL, "(A2)+",        0050332 | 0xA00);
    TEST(SPL, "-(A2)",        0050342 | 0xA00);
    TEST(SPL, "($1234,A2)",   0050352 | 0xA00, 0x1234);
    TEST(SPL, "(18,A2,D3.W)", 0050362 | 0xA00, 0x3012);
    TEST(SPL, "($001234).W",  0050370 | 0xA00, 0x1234);
    TEST(SPL, "($123456).L",  0050371 | 0xA00, 0x0012, 0x3456);
    EROA(SPL, "(d16,PC)",     0050372 | 0xA00);
    EROA(SPL, "(d8,PC,Xn)",   0050373 | 0xA00);
    EROA(SPL, "#nnnn",        0050374 | 0xA00);
    TEST(SMI, "D2",           0050302 | 0xB00);
    TEST(DBMI, "D2,*+2",      0050312 | 0xB00, 0); // DBMI
    TEST(SMI, "(A2)",         0050322 | 0xB00);
    TEST(SMI, "(A2)+",        0050332 | 0xB00);
    TEST(SMI, "-(A2)",        0050342 | 0xB00);
    TEST(SMI, "($1234,A2)",   0050352 | 0xB00, 0x1234);
    TEST(SMI, "(18,A2,D3.W)", 0050362 | 0xB00, 0x3012);
    TEST(SMI, "($001234).W",  0050370 | 0xB00, 0x1234);
    TEST(SMI, "($123456).L",  0050371 | 0xB00, 0x0012, 0x3456);
    EROA(SMI, "(d16,PC)",     0050372 | 0xB00);
    EROA(SMI, "(d8,PC,Xn)",   0050373 | 0xB00);
    EROA(SMI, "#nnnn",        0050374 | 0xB00);
    TEST(SGE, "D2",           0050302 | 0xC00);
    TEST(DBGE, "D2,*+2",      0050312 | 0xC00, 0); // DBGE
    TEST(SGE, "(A2)",         0050322 | 0xC00);
    TEST(SGE, "(A2)+",        0050332 | 0xC00);
    TEST(SGE, "-(A2)",        0050342 | 0xC00);
    TEST(SGE, "($1234,A2)",   0050352 | 0xC00, 0x1234);
    TEST(SGE, "(18,A2,D3.W)", 0050362 | 0xC00, 0x3012);
    TEST(SGE, "($001234).W",  0050370 | 0xC00, 0x1234);
    TEST(SGE, "($123456).L",  0050371 | 0xC00, 0x0012, 0x3456);
    EROA(SGE, "(d16,PC)",     0050372 | 0xC00);
    EROA(SGE, "(d8,PC,Xn)",   0050373 | 0xC00);
    EROA(SGE, "#nnnn",        0050374 | 0xC00);
    TEST(SLT, "D2",           0050302 | 0xD00);
    TEST(DBLT, "D2,*+2",      0050312 | 0xD00, 0); // DBLT
    TEST(SLT, "(A2)",         0050322 | 0xD00);
    TEST(SLT, "(A2)+",        0050332 | 0xD00);
    TEST(SLT, "-(A2)",        0050342 | 0xD00);
    TEST(SLT, "($1234,A2)",   0050352 | 0xD00, 0x1234);
    TEST(SLT, "(18,A2,D3.W)", 0050362 | 0xD00, 0x3012);
    TEST(SLT, "($001234).W",  0050370 | 0xD00, 0x1234);
    TEST(SLT, "($123456).L",  0050371 | 0xD00, 0x0012, 0x3456);
    EROA(SLT, "(d16,PC)",     0050372 | 0xD00);
    EROA(SLT, "(d8,PC,Xn)",   0050373 | 0xD00);
    EROA(SLT, "#nnnn",        0050374 | 0xD00);
    TEST(SGT, "D2",           0050302 | 0xE00);
    TEST(DBGT, "D2,*+2",      0050312 | 0xE00, 0); // DBGT
    TEST(SGT, "(A2)",         0050322 | 0xE00);
    TEST(SGT, "(A2)+",        0050332 | 0xE00);
    TEST(SGT, "-(A2)",        0050342 | 0xE00);
    TEST(SGT, "($1234,A2)",   0050352 | 0xE00, 0x1234);
    TEST(SGT, "(18,A2,D3.W)", 0050362 | 0xE00, 0x3012);
    TEST(SGT, "($001234).W",  0050370 | 0xE00, 0x1234);
    TEST(SGT, "($123456).L",  0050371 | 0xE00, 0x0012, 0x3456);
    EROA(SGT, "(d16,PC)",     0050372 | 0xE00);
    EROA(SGT, "(d8,PC,Xn)",   0050373 | 0xE00);
    EROA(SGT, "#nnnn",        0050374 | 0xE00);
    TEST(SLE, "D2",           0050302 | 0xF00);
    TEST(DBLE, "D2,*+2",      0050312 | 0xF00, 0); // DBLE
    TEST(SLE, "(A2)",         0050322 | 0xF00);
    TEST(SLE, "(A2)+",        0050332 | 0xF00);
    TEST(SLE, "-(A2)",        0050342 | 0xF00);
    TEST(SLE, "($1234,A2)",   0050352 | 0xF00, 0x1234);
    TEST(SLE, "(18,A2,D3.W)", 0050362 | 0xF00, 0x3012);
    TEST(SLE, "($001234).W",  0050370 | 0xF00, 0x1234);
    TEST(SLE, "($123456).L",  0050371 | 0xF00, 0x0012, 0x3456);
    EROA(SLE, "(d16,PC)",     0050372 | 0xF00);
    EROA(SLE, "(d8,PC,Xn)",   0050373 | 0xF00);
    EROA(SLE, "#nnnn",        0050374 | 0xF00);

    // BRA label: 00600|disp
    TEST(BRA, "*-$7FFE", 0060000, 0x8000);
    TEST(BRA, "*-$007E", 0060000, 0xFF80);
    TEST(BRA, "*-126",   0060000 | 0x80);
    TEST(BRA, "*",       0060000 | 0xFE);
    TEST(BRA, "*",       0060000, 0xFFFE);
    TEST(BRA, "*+2",     0060000, 0x0000);
    TEST(BRA, "*+128",   0060000 | 0x7E);
    TEST(BRA, "*+$0080", 0060000, 0x007E);
    TEST(BRA, "*+$8000", 0060000, 0x7FFE);
    ERNA(BRA, "*-$7FFF", 0060000, 0x8001);
    ERNA(BRA, "*-127",   0060000 | 0x81);
    ERNA(BRA, "*+1",     0060000 | 0xFF);
    ERNA(BRA, "*+3",     0060000 | 0x01);
    ERNA(BRA, "*+129",   0060000 | 0x7F);
    ERNA(BRA, "*+$8001", 0060000, 0x7FFF);

    // BSR label: 00604|disp
    TEST(BSR, "*-$7FFE", 0060400, 0x8000);
    TEST(BSR, "*-$007E", 0060400, 0xFF80);
    TEST(BSR, "*-126",   0060400 | 0x80);
    TEST(BSR, "*",       0060400 | 0xFE);
    TEST(BSR, "*",       0060400, 0xFFFE);
    TEST(BSR, "*+2",     0060400, 0x0000);
    TEST(BSR, "*+128",   0060400 | 0x7E);
    TEST(BSR, "*+$0080", 0060400, 0x007E);
    TEST(BSR, "*+$8000", 0060400, 0x7FFE);
    ERNA(BSR, "*-$7FFF", 0060400, 0x8001);
    ERNA(BSR, "*-127",   0060400 | 0x81);
    ERNA(BSR, "*+1",     0060400 | 0xFF);
    ERNA(BSR, "*+3",     0060400 | 0x01);
    ERNA(BSR, "*+129",   0060400 | 0x7F);
    ERNA(BSR, "*+$8001", 0060400, 0x7FFF);

    // JMP dst: 00473|M|Rn
    EROA(JMP, "D2",             0047302);
    EROA(JMP, "A2",             0047312);
    TEST(JMP, "(A2)",           0047322);
    EROA(JMP, "(A2)+",          0047332);
    EROA(JMP, "-(A2)",          0047342);
    TEST(JMP, "($1234,A2)",     0047352, 0x1234);
    TEST(JMP, "(18,A2,D3.W)",   0047362, 0x3012);
    TEST(JMP, "($001234).W",    0047370, 0x1234);
    TEST(JMP, "($123456).L",    0047371, 0x0012, 0x3456);
    TEST(JMP, "(*+$1234,PC)",   0047372, 0x1232);
    TEST(JMP, "(*+18,PC,D3.L)", 0047373, 0x3810);
    EROA(JMP, "#nnnn",          0047374);

    // JSR dst, 00472|M|Rn
    EROA(JSR, "D2",             0047202);
    EROA(JSR, "A2",             0047212);
    TEST(JSR, "(A2)",           0047222);
    EROA(JSR, "(A2)+",          0047232);
    EROA(JSR, "-(A2)",          0047242);
    TEST(JSR, "($1234,A2)",     0047252, 0x1234);
    TEST(JSR, "(18,A2,D3.W)",   0047262, 0x3012);
    TEST(JSR, "($001234).W",    0047270, 0x1234);
    TEST(JSR, "($123456).L",    0047271, 0x0012, 0x3456);
    TEST(JSR, "(*+$1234,PC)",   0047272, 0x1232);
    TEST(JSR, "(*+18,PC,D3.L)", 0047273, 0x3810);
    EROA(JSR, "#nnnn",          0047274);

    // NOP
    TEST(NOP, "", 047161);

    // RTR
    TEST(RTR, "", 047167);

    // RTS
    TEST(RTS, "", 047165);

    // TST dst: 0045|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST(TST_B, "D2",           0045002);
    EROA(TST_B, "A2",           0045012);
    TEST(TST_B, "(A2)",         0045022);
    TEST(TST_B, "(A2)+",        0045032);
    TEST(TST_B, "-(A2)",        0045042);
    TEST(TST_B, "($1234,A2)",   0045052, 0x1234);
    TEST(TST_B, "(18,A2,D3.W)", 0045062, 0x3012);
    TEST(TST_B, "($001234).W",  0045070, 0x1234);
    TEST(TST_B, "($123456).L",  0045071, 0x0012, 0x3456);
    EROA(TST_B, "(d16,PC)",     0045072);
    EROA(TST_B, "(d8,PC,Xn)",   0045073);
    EROA(TST_B, "#nnnn",        0045074);
    TEST(TST_W, "D2",           0045102);
    EROA(TST_W, "A2",           0045112);
    TEST(TST_W, "(A2)",         0045122);
    TEST(TST_W, "(A2)+",        0045132);
    TEST(TST_W, "-(A2)",        0045142);
    TEST(TST_W, "($1234,A2)",   0045152, 0x1234);
    TEST(TST_W, "(18,A2,D3.W)", 0045162, 0x3012);
    TEST(TST_W, "($001234).W",  0045170, 0x1234);
    TEST(TST_W, "($123456).L",  0045171, 0x0012, 0x3456);
    EROA(TST_W, "(d16,PC)",     0045172);
    EROA(TST_W, "(d8,PC,Xn)",   0045173);
    EROA(TST_W, "#nnnn",        0045174);
    TEST(TST_L, "D2",           0045202);
    EROA(TST_L, "A2",           0045212);
    TEST(TST_L, "(A2)",         0045222);
    TEST(TST_L, "(A2)+",        0045232);
    TEST(TST_L, "-(A2)",        0045242);
    TEST(TST_L, "($1234,A2)",   0045252, 0x1234);
    TEST(TST_L, "(18,A2,D3.W)", 0045262, 0x3012);
    TEST(TST_L, "($001234).W",  0045270, 0x1234);
    TEST(TST_L, "($123454).L",  0045271, 0x0012, 0x3454);
    EROA(TST_L, "(d16,PC)",     0045272);
    EROA(TST_L, "(d8,PC,Xn)",   0045273);
    EROA(TST_L, "#nnnn",        0045274);
}

static void test_system() {
    // ANDI #nn,SR
    TEST(ANDI, "#$1234,SR", 0001174, 0x1234);

    // EORI #nn,SR
    TEST(EORI, "#$1234,SR", 0005174, 0x1234);

    // MOVE src,SR: 00433|M|Rn
    TEST(MOVE, "D2,SR",             0043302);
    EROA(MOVE, "A2,SR",             0043312);
    TEST(MOVE, "(A2),SR",           0043322);
    TEST(MOVE, "(A2)+,SR",          0043332);
    TEST(MOVE, "-(A2),SR",          0043342);
    TEST(MOVE, "($1234,A2),SR",     0043352, 0x1234);
    TEST(MOVE, "(18,A2,D3.W),SR",   0043362, 0x3012);
    TEST(MOVE, "($001234).W,SR",    0043370, 0x1234);
    ERNA(MOVE, "($001233).W,SR",    0043370, 0x1233);
    TEST(MOVE, "($234568).L,SR",    0043371, 0x0023, 0x4568);
    ERNA(MOVE, "($234567).L,SR",    0043371, 0x0023, 0x4567);
    TEST(MOVE, "(*+$1234,PC),SR",   0043372, 0x1232);
    ERNA(MOVE, "(*+$1235,PC),SR",   0043372, 0x1233);
    TEST(MOVE, "(*-16,PC,D3.L),SR", 0043373, 0x38EE);
    TEST(MOVE, "#$3456,SR",         0043374, 0x3456);

    // MOVE SR,dst: 00403|M|Rn
    TEST(MOVE, "SR,D2",           0040302);
    EROA(MOVE, "SR,A2",           0040312);
    TEST(MOVE, "SR,(A2)",         0040322);
    TEST(MOVE, "SR,(A2)+",        0040332);
    TEST(MOVE, "SR,-(A2)",        0040342);
    TEST(MOVE, "SR,($1234,A2)",   0040352, 0x1234);
    TEST(MOVE, "SR,(18,A2,D3.W)", 0040362, 0x3012);
    TEST(MOVE, "SR,($001234).W",  0040370, 0x1234);
    ERNA(MOVE, "SR,($001233).W",  0040370, 0x1233);
    TEST(MOVE, "SR,($234568).L",  0040371, 0x0023, 0x4568);
    ERNA(MOVE, "SR,($234567).L",  0040371, 0x0023, 0x4567);
    EROA(MOVE, "SR,(d16,PC)",     0040372, 0x1232);
    EROA(MOVE, "SR,(d8,PC,Xn)",   0040373);
    EROA(MOVE, "SR,#nnnn",        0040374);

    // MOVE USP,An: 004715|An
    TEST(MOVE, "USP,A2", 0047152);

    // MOVE An,USP: 004714|An
    TEST(MOVE, "A2,USP", 0047142);

    // ORI #nn,SR
    TEST(ORI, "#$1234,SR", 0000174, 0x1234);

    // RESET
    TEST(RESET, "", 047160);

    // RTE
    TEST(RTE, "", 047163);

    // STOP #nn
    TEST(STOP, "#$1234", 047162, 0x1234);

    // CHK src,Dn: 004|Dn|Sz|M|Rn, Sz:W=6/L=7
    TEST(CHK_W, "D2,D7",             0047602);
    EROA(CHK_W, "A2,D7",             0047612);
    TEST(CHK_W, "(A2),D7",           0047622);
    TEST(CHK_W, "(A2)+,D7",          0047632);
    TEST(CHK_W, "-(A2),D7",          0047642);
    TEST(CHK_W, "($1234,A2),D7",     0047652, 0x1234);
    TEST(CHK_W, "(18,A2,D3.L),D7",   0047662, 0x3812);
    TEST(CHK_W, "($FFFFFE).W,D7",    0047670, 0xFFFE);
    TEST(CHK_W, "($123456).L,D7",    0047671, 0x0012, 0x3456);
    TEST(CHK_W, "(*+$1234,PC),D7",   0047672, 0x1232);
    TEST(CHK_W, "(*+18,PC,D3.W),D7", 0047673, 0x3010);
    TEST(CHK_W, "#$0034,D7",         0047674, 0x0034);

    // ILLEGAL
    TEST(ILLEGAL, "", 0045374);

    // TRAP #nn
    TEST(TRAP, "#0",  0047100);
    TEST(TRAP, "#15", 0047117);

    // TRAPV
    TEST(TRAPV, "", 047166);

    // ANDI #nn,CCR
    TEST(ANDI, "#$34,CCR", 0001074, 0x0034);

    // EORI #nn,CCR
    TEST(EORI, "#$34,CCR", 0005074, 0x0034);

    // MOVE src,CCR: 00423|M|R
    TEST(MOVE, "D2,CCR",             0042302);
    EROA(MOVE, "A2,CCR",             0042312);
    TEST(MOVE, "(A2),CCR",           0042322);
    TEST(MOVE, "(A2)+,CCR",          0042332);
    TEST(MOVE, "-(A2),CCR",          0042342);
    TEST(MOVE, "($1234,A2),CCR",     0042352, 0x1234);
    TEST(MOVE, "(18,A2,D3.W),CCR",   0042362, 0x3012);
    TEST(MOVE, "($001234).W,CCR",    0042370, 0x1234);
    ERNA(MOVE, "($001233).W,CCR",    0042370, 0x1235);
    TEST(MOVE, "($234568).L,CCR",    0042371, 0x0023, 0x4568);
    ERNA(MOVE, "($234567).L,CCR",    0042371, 0x0023, 0x4569);
    TEST(MOVE, "(*+$1234,PC),CCR",   0042372, 0x1232);
    ERNA(MOVE, "(*+$1233,PC),CCR",   0042372, 0x1233);
    TEST(MOVE, "(*-16,PC,D3.L),CCR", 0042373, 0x38EE);
    TEST(MOVE, "#$34,CCR",           0042374, 0x0034);

    // ORI #nn,CCR
    TEST(ORI, "#$34,CCR", 0000074, 0x0034);
}

static void test_multiproc() {
    // TAS dst: 00453|M|Rn
    TEST(TAS, "D2",           0045302);
    EROA(TAS, "A2",           0045312);
    TEST(TAS, "(A2)",         0045322);
    TEST(TAS, "(A2)+",        0045332);
    TEST(TAS, "-(A2)",        0045342);
    TEST(TAS, "($1234,A2)",   0045352, 0x1234);
    TEST(TAS, "(18,A2,D3.W)", 0045362, 0x3012);
    TEST(TAS, "($001235).W",  0045370, 0x1235);
    TEST(TAS, "($123457).L",  0045371, 0x0012, 0x3457);
    EROA(TAS, "(d16,PC)",     0045372);
    EROA(TAS, "(d8,PC,Xn)",   0045373);
    TEST(ILLEGAL, "",         0045374); // ILLEGAL
}
// clang-format on

const char *run_cpu_test() {
    RUN_TEST(test_cpu);
    return disassembler.listCpu();
}

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

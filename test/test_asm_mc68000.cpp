/* Copyright 2020 Tadashi G. Takaoka
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

#include "asm_mc68000.h"
#include "test_asm_helper.h"

using namespace libasm;
using namespace libasm::mc68000;
using namespace libasm::test;

AsmMc68000 as68000;
Assembler &assembler(as68000);

bool mc68k00() {
    return strcmp_P("68000", assembler.config().cpu_P()) == 0;
}

bool mc68010() {
    return strcmp_P("68010", assembler.config().cpu_P()) == 0;
}

bool mc68020() {
    return strcmp_P("68020", assembler.config().cpu_P()) == 0;
}

bool mc68030() {
    return strcmp_P("68030", assembler.config().cpu_P()) == 0;
}

bool mc68040() {
    return strcmp_P("68040", assembler.config().cpu_P()) == 0;
}

bool firstGen() {
    return mc68k00() || mc68010();
}

/**
 * Test helper for Absolute Short addressing mode.
 * Substitute |mark| in |text| with |word| into |line|.
 */
void subst(char *line, const char *text, const char *mark, const char *word, bool firstGen) {
    const auto len = strlen(mark);
    while (*text) {
        if (strncmp(text, mark, len) == 0) {
            text += 2;
            *line = 0;
            // Sign extend short address to full width address.
            if (*word >= '8') {
                strcat(line, firstGen ? "$FF" : "$FFFF");
            } else {
                strcat(line, firstGen ? "$00" : "$0000");
            }
            strcat(line, word);
            line += strlen(line);
        } else {
            *line++ = *text++;
        }
    }
    *line = 0;
}

#define ABSW(text_, word, ...)                      \
    do {                                            \
        char text[128];                             \
        subst(text, text_, "@W", word, firstGen()); \
        TEST(text, __VA_ARGS__);                    \
    } while (0)

#define ERRW(text_, word, error, errat_, ...)         \
    do {                                              \
        char text[128];                               \
        char errat[128];                              \
        subst(text, text_, "@W", word, firstGen());   \
        subst(errat, errat_, "@W", word, firstGen()); \
        ERRT(text, error, errat, __VA_ARGS__);        \
    } while (0)

#if defined(LIBASM_ASM_NOFLOAT)
#define FLTS(insn, opr, opc, post, ...) \
    ERRT(insn " " opr, FLOAT_NOT_SUPPORTED, opr, opc, post, 0, 0)
#define FLTD(insn, opr, opc, post, ...) \
    ERRT(insn " " opr, FLOAT_NOT_SUPPORTED, opr, opc, post, 0, 0, 0, 0)
#define FLTX(insn, opr, opc, post, ...) \
    ERRT(insn " " opr, FLOAT_NOT_SUPPORTED, opr, opc, post, 0, 0, 0, 0, 0, 0)
#define FLTP(insn, opr, opc, post, ...) \
    ERRT(insn " " opr, FLOAT_NOT_SUPPORTED, opr, opc, post, 0, 0, 0, 0, 0, 0)
#else
#define FLTS(insn, opr, opc, post, ...) TEST(insn " " opr, opc, post, __VA_ARGS__)
#define FLTD(insn, opr, opc, post, ...) TEST(insn " " opr, opc, post, __VA_ARGS__)
#define FLTX(insn, opr, opc, post, ...) TEST(insn " " opr, opc, post, __VA_ARGS__)
#define FLTP(insn, opr, opc, post, ...) TEST(insn " " opr, opc, post, __VA_ARGS__)
#endif

void set_up() {
    assembler.reset();
}

void tear_down() {
    symtab.reset();
}

// clang-format off
void test_cpu() {
    EQUALS("cpu 68000", true,    assembler.setCpu("68000"));
    EQUALS_P("cpu 68000", "68000", assembler.config().cpu_P());

    EQUALS("cpu mc68000", true,    assembler.setCpu("mc68000"));
    EQUALS_P("cpu mc68000", "68000", assembler.config().cpu_P());

    EQUALS("cpu 68010", true,    assembler.setCpu("68010"));
    EQUALS_P("cpu 68010", "68010", assembler.config().cpu_P());

    EQUALS("cpu mc68010", true,    assembler.setCpu("mc68010"));
    EQUALS_P("cpu mc68010", "68010", assembler.config().cpu_P());

    EQUALS("cpu 68020", true,    assembler.setCpu("68020"));
    EQUALS_P("cpu 68020", "68020", assembler.config().cpu_P());

    EQUALS("cpu mc68020", true,    assembler.setCpu("mc68020"));
    EQUALS_P("cpu mc68020", "68020", assembler.config().cpu_P());

    EQUALS("cpu 68030", true,    assembler.setCpu("68030"));
    EQUALS_P("cpu 68030", "68030", assembler.config().cpu_P());

    EQUALS("cpu mc68030", true,    assembler.setCpu("mc68030"));
    EQUALS_P("cpu mc68030", "68030", assembler.config().cpu_P());

    EQUALS("cpu 68040", true,    assembler.setCpu("68040"));
    EQUALS_P("cpu 68040", "68040", assembler.config().cpu_P());

    EQUALS("cpu mc68040", true,    assembler.setCpu("mc68040"));
    EQUALS_P("cpu mc68040", "68040", assembler.config().cpu_P());
}

void test_data_move() {
    // EXG Rx,Ry
    TEST("exg   d1,d2", 0141502); // 014|Dx|50|Dy
    ERRT("EXG.B D1,D2", OPERAND_NOT_ALLOWED, "D1,D2");
    ERRT("EXG.W D1,D2", OPERAND_NOT_ALLOWED, "D1,D2");
    TEST("EXG.L D1,D2", 0141502); // 014|Dx|50|Dy
    TEST("EXG   A1,A2", 0141512); // 014|Ax|51|Ay
    ERRT("EXG.B A1,A2", OPERAND_NOT_ALLOWED, "A1,A2");
    ERRT("EXG.W A1,A2", OPERAND_NOT_ALLOWED, "A1,A2");
    TEST("exg.l a1,a2", 0141512); // 014|Ax|51|Ay
    TEST("EXG   D1,A2", 0141612); // 014|Dx|61|Ay
    ERRT("EXG.B D1,A2", OPERAND_NOT_ALLOWED, "D1,A2");
    ERRT("EXG.W D1,A2", OPERAND_NOT_ALLOWED, "D1,A2");
    TEST("EXG.L D1,A2", 0141612); // 014|Dx|61|Ay
    TEST("EXG   A2,D1", 0141612); // 014|Dx|61|Ay
    ERRT("EXG.B A2,D1", OPERAND_NOT_ALLOWED, "A2,D1");
    ERRT("EXG.W A2,D1", OPERAND_NOT_ALLOWED, "A2,D1");
    TEST("EXG.L A2,D1", 0141612); // 014|Dx|61|Ay

    // LEA ea,Ad: 004|An|7|M|Rn
    ERRT("LEA D2,A1",              OPERAND_NOT_ALLOWED, "D2,A1", 0041700);
    ERRT("LEA A2,A1",              OPERAND_NOT_ALLOWED, "A2,A1", 0041700);
    TEST("LEA (A2),A1",            0041722);
    TEST("LEA.L (A2),A1",          0041722);
    ERRT("LEA (A2)+,A1",           OPERAND_NOT_ALLOWED, "(A2)+,A1", 0041700);
    ERRT("LEA -(A2),A1",           OPERAND_NOT_ALLOWED, "-(A2),A1", 0041700);
    TEST("LEA ($1234,A2),A1",      0041752, 0x1234);
    TEST("LEA ($12,A2,D3.W),A1",   0041762, 0x3012);
    ABSW("LEA (@W).W,A1", "FF00",  0041770, 0xFF00);
    TEST("LEA ($001234).L,A1",     0041771, 0x0000, 0x1234);
    TEST("LEA (*+$1234,PC),A1",    0041772, 0x1232);
    TEST("lea (*+$12,pc,a3.l),a1", 0041773, 0xB810);
    ERRT("LEA #$1234,A1",          OPERAND_NOT_ALLOWED, "#$1234,A1", 0041700);

    // LINK An,#nn: 004712|An
    ERRT("LINK A3,D2",     OPERAND_NOT_ALLOWED, "A3,D2");
    TEST("LINK A3,#$1234", 0047123, 0x1234);
    TEST("LINK A3,#-16",   0047123, 0xFFF0);
    if (!firstGen()) {
        TEST("LINK.L A3,#$1234", 0044013, 0x0000, 0x1234);
        TEST("LINK.L A3,#-16",   0044013, 0xFFFF, 0xFFF0);
    }

    // MOVE src,dst: 00|Sz|Rd|Md|Ms|Rs, Sz:B=1/W=3/L=2
    TEST("MOVE.B D2,D7",              0017002);
    ERRT("MOVE.B A2,D7",              OPERAND_NOT_ALLOWED, "A2,D7", 0017000);
    TEST("MOVE.B (A2),D7",            0017022);
    TEST("MOVE.B (A2)+,D7",           0017032);
    TEST("MOVE.B -(A2),D7",           0017042);
    TEST("MOVE.B ($1234,A2),D7",      0017052, 0x1234);
    TEST("MOVE.B (-$8000,A2),D7",     0017052, 0x8000);
    TEST("MOVE.B ($7FFF,A2),D7",      0017052, 0x7FFF);
    if (firstGen()) {
        ERRT("MOVE.B (-$8001,A2),D7",
             OVERFLOW_RANGE, "-$8001,A2),D7", 0017052, 0x7FFF);
        ERRT("MOVE.B ($8000,A2),D7",
             OVERFLOW_RANGE, "$8000,A2),D7",  0017052, 0x8000);
    } else {
        TEST("MOVE.B (-$8001,A2),D7",         0017062, 0x0170, 0xFFFF, 0x7FFF);
        TEST("MOVE.B ($8000,A2),D7",          0017062, 0x0170, 0x0000, 0x8000);
    }
    TEST("MOVE.B ($12,A2,D3.L),D7",   0017062, 0x3812);
    TEST("MOVE.B (-$80,A2,D3.L),D7",  0017062, 0x3880);
    TEST("MOVE.B ($7F,A2,D3.L),D7",   0017062, 0x387F);
    if (firstGen()) {
        ERRT("MOVE.B (-$81,A2,D3.L),D7",
             OVERFLOW_RANGE, "-$81,A2,D3.L),D7", 0017062, 0x387F);
        ERRT("MOVE.B ($80,A2,D3.L),D7",
             OVERFLOW_RANGE, "$80,A2,D3.L),D7",  0017062, 0x3880);
    } else {
        TEST("MOVE.B (-$81,A2,D3.L),D7",         0017062, 0x3920, 0xFF7F);
        TEST("MOVE.B ($80,A2,D3.L),D7",          0017062, 0x3920, 0x0080);
    }
    TEST("MOVE.B ($1234).W,D7",       0017070, 0x1234);
    TEST("MOVE.B ($123456).L,D7",     0017071, 0x0012, 0x3456);
    TEST("MOVE.B (32767),D7",         0017070, 0x7FFF);
    TEST("MOVE.B (32767).W,D7",       0017070, 0x7FFF);
    TEST("MOVE.B (32767).L,D7",       0017071, 0x0000, 0x7FFF);
    TEST("MOVE.B (32768),D7",         0017071, 0x0000, 0x8000);
    ERRT("MOVE.B (32768).W,D7",       OVERFLOW_RANGE, "(32768).W,D7", 0017070, 0x8000);
    TEST("MOVE.B (32768).L,D7",       0017071, 0x0000, 0x8000);
    TEST("MOVE.B (-1),D7",            0017070, 0xFFFF);
    TEST("MOVE.B (-1).W,D7",          0017070, 0xFFFF);
    TEST("MOVE.B (-1).L,D7",          0017071, 0xFFFF, 0xFFFF);
    TEST("MOVE.B (-32768),D7",        0017070, 0x8000);
    TEST("MOVE.B (-32768).W,D7",      0017070, 0x8000);
    TEST("MOVE.B (-32768).L,D7",      0017071, 0xFFFF, 0x8000);
    TEST("MOVE.B (-32769),D7",        0017071, 0xFFFF, 0x7FFF);
    ERRT("MOVE.B (-32769).W,D7",      OVERFLOW_RANGE, "(-32769).W,D7", 0017070, 0x7FFF);
    TEST("MOVE.B (-32769).L,D7",      0017071, 0xFFFF, 0x7FFF);
    ABSW("MOVE.B (@W),D7", "FFFF",    0017070, 0xFFFF);
    ABSW("MOVE.B (@W).W,D7", "FFFF",  0017070, 0xFFFF);
    TEST("MOVE.B ($FFFFFF).L,D7",     0017071, 0x00FF, 0xFFFF);
    ABSW("MOVE.B (@W),D7", "8000",    0017070, 0x8000);
    ABSW("MOVE.B (@W).W,D7", "8000",  0017070, 0x8000);
    TEST("MOVE.B ($FF8000).L,D7",     0017071, 0x00FF, 0x8000);
    TEST("MOVE.B ($FF7FFF),D7",       0017071, 0x00FF, 0x7FFF);
    ABSW("MOVE.B (@W).W,D7", "7FFF",  0017070, 0x7FFF);
    TEST("MOVE.B ($FF7FFF).L,D7",     0017071, 0x00FF, 0x7FFF);
    TEST("MOVE.B ($1000000),D7",      0017071, 0x0100, 0x0000);
    ERRT("MOVE.B ($1000000).W,D7",    OVERFLOW_RANGE, "($1000000).W,D7", 017070, 0x0000);
    TEST("MOVE.B ($1000000).L,D7",    0017071, 0x0100, 0x0000);
    TEST("MOVE.B (*+$1234,PC),D7",    0017072, 0x1232);
    TEST("MOVE.B (*+$12,PC,D3.W),D7", 0017073, 0x3010);
    TEST("MOVE.B (*-$7FFE,PC),D7",    0017072, 0x8000);
    TEST("MOVE.B (*+$8001,PC),D7",    0017072, 0x7FFF);
    if (firstGen()) {
        ERRT("MOVE.B (*-$7FFF,PC),D7",
             OVERFLOW_RANGE, "*-$7FFF,PC),D7", 0017072, 0x7FFF);
        ERRT("MOVE.B (*+$8002,PC),D7",
             OVERFLOW_RANGE, "*+$8002,PC),D7", 0017072, 0x8000);
    } else {
        TEST("MOVE.B (*-$7FFF,PC),D7",         0017073, 0x0170, 0xFFFF, 0x7FFF);
        TEST("MOVE.B (*+$8002,PC),D7",         0017073, 0x0170, 0x0000, 0x8000);
    }
    TEST("MOVE.B (*-$7E,PC,D3),D7",   0017073, 0x3080);
    TEST("MOVE.B (*+$81,PC,D3),D7",   0017073, 0x307F);
    if (firstGen()) {
        ERRT("MOVE.B (*-$7F,PC,D3),D7",
             OVERFLOW_RANGE, "*-$7F,PC,D3),D7", 0017073, 0x307F);
        ERRT("MOVE.B (*+$82,PC,D3),D7",
             OVERFLOW_RANGE, "*+$82,PC,D3),D7", 0017073, 0x3080);
    } else {
        TEST("MOVE.B (*-$7F,PC,D3),D7",         0017073, 0x3120, 0xFF7F);
        TEST("MOVE.B (*+$82,PC,D3),D7",         0017073, 0x3120, 0x0080);
    }
    TEST("MOVE.B #$34,D7",            0017074, 0x0034);
    TEST("MOVE.B #$FF,D7",            0017074, 0x00FF);
    TEST("MOVE.B #-$80,D7",           0017074, 0x0080);
    ERRT("MOVE.B #$100,D7",           OVERFLOW_RANGE, "#$100,D7", 0017074, 0x0000);
    ERRT("MOVE.B #-$81,D7",           OVERFLOW_RANGE, "#-$81,D7", 0017074, 0x007F);
    TEST("MOVE.W D2,D7",              0037002);
    TEST("MOVE.W A2,D7",              0037012);
    TEST("MOVE.W (A2),D7",            0037022);
    TEST("MOVE.W (A2)+,D7",           0037032);
    TEST("MOVE.W -(A2),D7",           0037042);
    TEST("MOVE.W ($1234,A2),D7",      0037052, 0x1234);
    TEST("MOVE.W ($12,A2,D3.L),D7",   0037062, 0x3812);
    ABSW("MOVE.W (@W).W,D7", "FFFE",  0037070, 0xFFFE);
    TEST("MOVE.W ($123456).L,D7",     0037071, 0x0012, 0x3456);
    TEST("MOVE.W (*+$1234,PC),D7",    0037072, 0x1232);
    TEST("MOVE.W (*+$12,PC,D3.W),D7", 0037073, 0x3010);
    TEST("MOVE.W #$0034,D7",          0037074, 0x0034);
    TEST("MOVE.W #$FFFF,D7",          0037074, 0xFFFF);
    TEST("MOVE.W #-$8000,D7",         0037074, 0x8000);
    ERRT("MOVE.W #$10000,D7",         OVERFLOW_RANGE, "#$10000,D7", 0037074, 0x0000);
    ERRT("MOVE.W #-$8001,D7",         OVERFLOW_RANGE, "#-$8001,D7", 0037074, 0x7FFF);
    TEST("MOVE.L D2,D7",              0027002);
    TEST("MOVE.L A2,D7",              0027012);
    TEST("MOVE.L (A2),D7",            0027022);
    TEST("MOVE.L (A2)+,D7",           0027032);
    TEST("MOVE.L -(A2),D7",           0027042);
    TEST("MOVE.L ($1234,A2),D7",      0027052, 0x1234);
    TEST("MOVE.L ($12,A2,D3.L),D7",   0027062, 0x3812);
    ABSW("MOVE.L (@W).W,D7", "FFFC",  0027070, 0xFFFC);
    TEST("MOVE.L ($123454).L,D7",     0027071, 0x0012, 0x3454);
    TEST("MOVE.L (*+$1234,PC),D7",    0027072, 0x1232);
    TEST("MOVE.L (*+$12,PC,D3.W),D7", 0027073, 0x3010);
    TEST("MOVE.L #$00345678,D7",      0027074, 0x0034, 0x5678);
    TEST("MOVE.B D2,(A6)",              0016202);
    ERRT("MOVE.B A2,(A6)",              OPERAND_NOT_ALLOWED, "A2,(A6)", 0016200);
    TEST("MOVE.B (A2),(A6)",            0016222);
    TEST("MOVE.B (A2)+,(A6)",           0016232);
    TEST("MOVE.B -(A2),(A6)",           0016242);
    TEST("MOVE.B ($1234,A2),(A6)",      0016252, 0x1234);
    TEST("MOVE.B ($12,A2,D3.L),(A6)",   0016262, 0x3812);
    ABSW("MOVE.B (@W).W,(A6)", "FFFF",  0016270, 0xFFFF);
    TEST("MOVE.B ($123456).L,(A6)",     0016271, 0x0012, 0x3456);
    TEST("MOVE.B (*+$1234,PC),(A6)",    0016272, 0x1232);
    TEST("MOVE.B (*+$12,PC,D3.W),(A6)", 0016273, 0x3010);
    TEST("MOVE.B #$34,(A6)",            0016274, 0x0034);
    TEST("MOVE.W D2,(A6)",              0036202);
    TEST("MOVE.W A2,(A6)",              0036212);
    TEST("MOVE.W (A2),(A6)",            0036222);
    TEST("MOVE.W (A2)+,(A6)",           0036232);
    TEST("MOVE.W -(A2),(A6)",           0036242);
    TEST("MOVE.W ($1234,A2),(A6)",      0036252, 0x1234);
    TEST("MOVE.W ($12,A2,D3.L),(A6)",   0036262, 0x3812);
    ABSW("MOVE.W (@W).W,(A6)", "FFFE",  0036270, 0xFFFE);
    TEST("MOVE.W ($123456).L,(A6)",     0036271, 0x0012, 0x3456);
    TEST("MOVE.W (*+$1234,PC),(A6)",    0036272, 0x1232);
    TEST("MOVE.W (*+$12,PC,D3.W),(A6)", 0036273, 0x3010);
    TEST("MOVE.W #$0034,(A6)",          0036274, 0x0034);
    TEST("MOVE.L D2,(A6)",              0026202);
    TEST("MOVE.L A2,(A6)",              0026212);
    TEST("MOVE.L (A2),(A6)",            0026222);
    TEST("MOVE.L (A2)+,(A6)",           0026232);
    TEST("MOVE.L -(A2),(A6)",           0026242);
    TEST("MOVE.L ($1234,A2),(A6)",      0026252, 0x1234);
    TEST("MOVE.L ($12,A2,D3.L),(A6)",   0026262, 0x3812);
    ABSW("MOVE.L (@W).W,(A6)", "FFFC",  0026270, 0xFFFC);
    TEST("MOVE.L ($123454).L,(A6)",     0026271, 0x0012, 0x3454);
    TEST("MOVE.L (*+$1234,PC),(A6)",    0026272, 0x1232);
    TEST("MOVE.L (*+$12,PC,D3.W),(A6)", 0026273, 0x3010);
    TEST("MOVE.L #$00345678,(A6)",      0026274, 0x0034, 0x5678);
    TEST("MOVE.B D2,(A6)+",              0016302);
    ERRT("MOVE.B A2,(A6)+",              OPERAND_NOT_ALLOWED, "A2,(A6)+", 0016300);
    TEST("MOVE.B (A2),(A6)+",            0016322);
    TEST("MOVE.B (A2)+,(A6)+",           0016332);
    TEST("MOVE.B -(A2),(A6)+",           0016342);
    TEST("MOVE.B ($1234,A2),(A6)+",      0016352, 0x1234);
    TEST("MOVE.B ($12,A2,D3.L),(A6)+",   0016362, 0x3812);
    ABSW("MOVE.B (@W).W,(A6)+", "FFFF",  0016370, 0xFFFF);
    TEST("MOVE.B ($123456).L,(A6)+",     0016371, 0x0012, 0x3456);
    TEST("MOVE.B (*+$1234,PC),(A6)+",    0016372, 0x1232);
    TEST("MOVE.B (*+$12,PC,D3.W),(A6)+", 0016373, 0x3010);
    TEST("MOVE.B #$34,(A6)+",            0016374, 0x0034);
    TEST("MOVE.W D2,(A6)+",              0036302);
    TEST("MOVE.W A2,(A6)+",              0036312);
    TEST("MOVE.W (A2),(A6)+",            0036322);
    TEST("MOVE.W (A2)+,(A6)+",           0036332);
    TEST("MOVE.W -(A2),(A6)+",           0036342);
    TEST("MOVE.W ($1234,A2),(A6)+",      0036352, 0x1234);
    TEST("MOVE.W ($12,A2,D3.L),(A6)+",   0036362, 0x3812);
    ABSW("MOVE.W (@W).W,(A6)+", "FFFE",  0036370, 0xFFFE);
    TEST("MOVE.W ($123456).L,(A6)+",     0036371, 0x0012, 0x3456);
    TEST("MOVE.W (*+$1234,PC),(A6)+",    0036372, 0x1232);
    TEST("MOVE.W (*+$12,PC,D3.W),(A6)+", 0036373, 0x3010);
    TEST("MOVE.W #$0034,(A6)+",          0036374, 0x0034);
    TEST("MOVE.L D2,(A6)+",              0026302);
    TEST("MOVE.L A2,(A6)+",              0026312);
    TEST("MOVE.L (A2),(A6)+",            0026322);
    TEST("MOVE.L (A2)+,(A6)+",           0026332);
    TEST("MOVE.L -(A2),(A6)+",           0026342);
    TEST("MOVE.L ($1234,A2),(A6)+",      0026352, 0x1234);
    TEST("MOVE.L ($12,A2,D3.L),(A6)+",   0026362, 0x3812);
    ABSW("MOVE.L (@W).W,(A6)+", "FFFC",  0026370, 0xFFFC);
    TEST("MOVE.L ($123454).L,(A6)+",     0026371, 0x0012, 0x3454);
    TEST("MOVE.L (*+$1234,PC),(A6)+",    0026372, 0x1232);
    TEST("MOVE.L (*+$12,PC,D3.W),(A6)+", 0026373, 0x3010);
    TEST("MOVE.L #$00345678,(A6)+",      0026374, 0x0034, 0x5678);
    TEST("MOVE.B D2,-(A6)",              0016402);
    ERRT("MOVE.B A2,-(A6)",              OPERAND_NOT_ALLOWED, "A2,-(A6)", 0016400);
    TEST("MOVE.B (A2),-(A6)",            0016422);
    TEST("MOVE.B (A2)+,-(A6)",           0016432);
    TEST("MOVE.B -(A2),-(A6)",           0016442);
    TEST("MOVE.B ($1234,A2),-(A6)",      0016452, 0x1234);
    TEST("MOVE.B ($12,A2,D3.L),-(A6)",   0016462, 0x3812);
    ABSW("MOVE.B (@W).W,-(A6)", "FFFF",  0016470, 0xFFFF);
    TEST("MOVE.B ($123456).L,-(A6)",     0016471, 0x0012, 0x3456);
    TEST("MOVE.B (*+$1234,PC),-(A6)",    0016472, 0x1232);
    TEST("MOVE.B (*+$12,PC,D3.W),-(A6)", 0016473, 0x3010);
    TEST("MOVE.B #$34,-(A6)",            0016474, 0x0034);
    TEST("MOVE.W D2,-(A6)",              0036402);
    TEST("MOVE.W A2,-(A6)",              0036412);
    TEST("MOVE.W (A2),-(A6)",            0036422);
    TEST("MOVE.W (A2)+,-(A6)",           0036432);
    TEST("MOVE.W -(A2),-(A6)",           0036442);
    TEST("MOVE.W ($1234,A2),-(A6)",      0036452, 0x1234);
    TEST("MOVE.W ($12,A2,D3.L),-(A6)",   0036462, 0x3812);
    ABSW("MOVE.W (@W).W,-(A6)", "FFFE",  0036470, 0xFFFE);
    TEST("MOVE.W ($123456).L,-(A6)",     0036471, 0x0012, 0x3456);
    TEST("MOVE.W (*+$1234,PC),-(A6)",    0036472, 0x1232);
    TEST("MOVE.W (*+$12,PC,D3.W),-(A6)", 0036473, 0x3010);
    TEST("MOVE.W #$0034,-(A6)",          0036474, 0x0034);
    TEST("MOVE.L D2,-(A6)",              0026402);
    TEST("MOVE.L A2,-(A6)",              0026412);
    TEST("MOVE.L (A2),-(A6)",            0026422);
    TEST("MOVE.L (A2)+,-(A6)",           0026432);
    TEST("MOVE.L -(A2),-(A6)",           0026442);
    TEST("MOVE.L ($1234,A2),-(A6)",      0026452, 0x1234);
    TEST("MOVE.L ($12,A2,D3.L),-(A6)",   0026462, 0x3812);
    ABSW("MOVE.L (@W).W,-(A6)", "FFFC",  0026470, 0xFFFC);
    TEST("MOVE.L ($123454).L,-(A6)",     0026471, 0x0012, 0x3454);
    TEST("MOVE.L (*+$1234,PC),-(A6)",    0026472, 0x1232);
    TEST("MOVE.L (*+$14,PC,D3.W),-(A6)", 0026473, 0x3012);
    TEST("MOVE.L #$00345678,-(A6)",      0026474, 0x0034, 0x5678);
    TEST("MOVE.B D2,($5678,A6)",              0016502, 0x5678);
    ERRT("MOVE.B A2,($5678,A6)",              OPERAND_NOT_ALLOWED, "A2,($5678,A6)", 0016500, 0x5678);
    TEST("MOVE.B (A2),($5678,A6)",            0016522, 0x5678);
    TEST("MOVE.B (A2)+,($5678,A6)",           0016532, 0x5678);
    TEST("MOVE.B -(A2),($5678,A6)",           0016542, 0x5678);
    TEST("MOVE.B ($1234,A2),($5678,A6)",      0016552, 0x1234, 0x5678);
    TEST("MOVE.B ($12,A2,D3.L),($5678,A6)",   0016562, 0x3812, 0x5678);
    ABSW("MOVE.B (@W),($5678,A6)", "FFFF",    0016570, 0xFFFF, 0x5678);
    TEST("MOVE.B ($123456).L,($5678,A6)",     0016571, 0x0012, 0x3456, 0x5678);
    TEST("MOVE.B (*+$1234,PC),($5678,A6)",    0016572, 0x1232, 0x5678);
    TEST("MOVE.B (*+$12,PC,D3.W),($5678,A6)", 0016573, 0x3010, 0x5678);
    TEST("MOVE.B #$34,($5678,A6)",            0016574, 0x0034, 0x5678);
    TEST("MOVE.W D2,($5678,A6)",              0036502, 0x5678);
    TEST("MOVE.W A2,($5678,A6)",              0036512, 0x5678);
    TEST("MOVE.W (A2),($5678,A6)",            0036522, 0x5678);
    TEST("MOVE.W (A2)+,($5678,A6)",           0036532, 0x5678);
    TEST("MOVE.W -(A2),($5678,A6)",           0036542, 0x5678);
    TEST("MOVE.W ($1234,A2),($5678,A6)",      0036552, 0x1234, 0x5678);
    TEST("MOVE.W ($12,A2,D3.L),($5678,A6)",   0036562, 0x3812, 0x5678);
    ABSW("MOVE.W (@W).W,($5678,A6)", "FFFE",  0036570, 0xFFFE, 0x5678);
    TEST("MOVE.W ($123456).L,($5678,A6)",     0036571, 0x0012, 0x3456, 0x5678);
    TEST("MOVE.W (*+$1234,PC),($5678,A6)",    0036572, 0x1232, 0x5678);
    TEST("MOVE.W (*+$12,PC,D3.W),($5678,A6)", 0036573, 0x3010, 0x5678);
    TEST("MOVE.W #$0034,($5678,A6)",          0036574, 0x0034, 0x5678);
    TEST("MOVE.L D2,($5678,A6)",              0026502, 0x5678);
    TEST("MOVE.L A2,($5678,A6)",              0026512, 0x5678);
    TEST("MOVE.L (A2),($5678,A6)",            0026522, 0x5678);
    TEST("MOVE.L (A2)+,($5678,A6)",           0026532, 0x5678);
    TEST("MOVE.L -(A2),($5678,A6)",           0026542, 0x5678);
    TEST("MOVE.L ($1234,A2),($5678,A6)",      0026552, 0x1234, 0x5678);
    TEST("MOVE.L ($12,A2,D3.L),($5678,A6)",   0026562, 0x3812, 0x5678);
    ABSW("MOVE.L (@W).W,($5678,A6)", "FFFC",  0026570, 0xFFFC, 0x5678);
    TEST("MOVE.L ($123454).L,($5678,A6)",     0026571, 0x0012, 0x3454, 0x5678);
    TEST("MOVE.L (*+$1234,PC),($5678,A6)",    0026572, 0x1232, 0x5678);
    TEST("MOVE.L (*+$12,PC,D3.W),($5678,A6)", 0026573, 0x3010, 0x5678);
    TEST("MOVE.L #$00345678,($5678,A6)",      0026574, 0x0034, 0x5678, 0x5678);
    TEST("MOVE.B D2,($56,A6,D5.W)",              0016602, 0x5056);
    ERRT("MOVE.B A2,($56,A6,D5.W)",              OPERAND_NOT_ALLOWED, "A2,($56,A6,D5.W)", 0016600, 0x5056);
    TEST("MOVE.B (A2),($56,A6,D5.W)",            0016622, 0x5056);
    TEST("MOVE.B (A2)+,($56,A6,D5.W)",           0016632, 0x5056);
    TEST("MOVE.B -(A2),($56,A6,D5.W)",           0016642, 0x5056);
    TEST("MOVE.B ($1234,A2),($56,A6,D5.W)",      0016652, 0x1234, 0x5056);
    TEST("MOVE.B ($12,A2,D3.L),($56,A6,D5.W)",   0016662, 0x3812, 0x5056);
    ABSW("MOVE.B (@W).W,($56,A6,D5.W)", "FFFF",  0016670, 0xFFFF, 0x5056);
    TEST("MOVE.B ($123456).L,($56,A6,D5.W)",     0016671, 0x0012, 0x3456, 0x5056);
    TEST("MOVE.B (*+$1234,PC),($56,A6,D5.W)",    0016672, 0x1232, 0x5056);
    TEST("MOVE.B (*+$12,PC,D3.W),($56,A6,D5.W)", 0016673, 0x3010, 0x5056);
    TEST("MOVE.B #$34,($56,A6,D5.W)",            0016674, 0x0034, 0x5056);
    TEST("MOVE.W D2,($56,A6,D5.W)",              0036602, 0x5056);
    TEST("MOVE.W A2,($56,A6,D5.W)",              0036612, 0x5056);
    TEST("MOVE.W (A2),($56,A6,D5.W)",            0036622, 0x5056);
    TEST("MOVE.W (A2)+,($56,A6,D5.W)",           0036632, 0x5056);
    TEST("MOVE.W -(A2),($56,A6,D5.W)",           0036642, 0x5056);
    TEST("MOVE.W ($1234,A2),($56,A6,D5.W)",      0036652, 0x1234, 0x5056);
    TEST("MOVE.W ($12,A2,D3.L),($56,A6,D5.W)",   0036662, 0x3812, 0x5056);
    ABSW("MOVE.W (@W).W,($56,A6,D5.W)", "FFFE",  0036670, 0xFFFE, 0x5056);
    TEST("MOVE.W ($123456).L,($56,A6,D5.W)",     0036671, 0x0012, 0x3456, 0x5056);
    TEST("MOVE.W (*+$1234,PC),($56,A6,D5.W)",    0036672, 0x1232, 0x5056);
    TEST("MOVE.W (*+$12,PC,D3.W),($56,A6,D5.W)", 0036673, 0x3010, 0x5056);
    TEST("MOVE.W #$0034,($56,A6,D5.W)",          0036674, 0x0034, 0x5056);
    TEST("MOVE.L D2,($56,A6,D5.W)",              0026602, 0x5056);
    TEST("MOVE.L A2,($56,A6,D5.W)",              0026612, 0x5056);
    TEST("MOVE.L (A2),($56,A6,D5.W)",            0026622, 0x5056);
    TEST("MOVE.L (A2)+,($56,A6,D5.W)",           0026632, 0x5056);
    TEST("MOVE.L -(A2),($56,A6,D5.W)",           0026642, 0x5056);
    TEST("MOVE.L ($1234,A2),($56,A6,D5.W)",      0026652, 0x1234, 0x5056);
    TEST("MOVE.L ($12,A2,D3.L),($56,A6,D5.W)",   0026662, 0x3812, 0x5056);
    ABSW("MOVE.L (@W).W,($56,A6,D5.W)", "FFFC",  0026670, 0xFFFC, 0x5056);
    TEST("MOVE.L ($123454).L,($56,A6,D5.W)",     0026671, 0x0012, 0x3454, 0x5056);
    TEST("MOVE.L (*+$1234,PC),($56,A6,D5.W)",    0026672, 0x1232, 0x5056);
    TEST("MOVE.L (*+$12,PC,D3.W),($56,A6,D5.W)", 0026673, 0x3010, 0x5056);
    TEST("MOVE.L #$00345678,($56,A6,D5.W)",      0026674, 0x0034, 0x5678, 0x5056);
    TEST("MOVE.B D2,($005678).W",              0010702, 0x5678);
    ERRT("MOVE.B A2,($005678).W",              OPERAND_NOT_ALLOWED, "A2,($005678).W", 0010700, 0x5678);
    TEST("MOVE.B (A2),($005678).W",            0010722, 0x5678);
    TEST("MOVE.B (A2)+,($005678).W",           0010732, 0x5678);
    TEST("MOVE.B -(A2),($005678).W",           0010742, 0x5678);
    TEST("MOVE.B ($1234,A2),($005678).W",      0010752, 0x1234, 0x5678);
    TEST("MOVE.B ($12,A2,D3.L),($005678).W",   0010762, 0x3812, 0x5678);
    ABSW("MOVE.B (@W).W,($005678).W", "FFFF",  0010770, 0xFFFF, 0x5678);
    TEST("MOVE.B ($123456).L,($005678).W",     0010771, 0x0012, 0x3456, 0x5678);
    TEST("MOVE.B (*+$1234,PC),($005678).W",    0010772, 0x1232, 0x5678);
    TEST("MOVE.B (*+$12,PC,D3.W),($005678).W", 0010773, 0x3010, 0x5678);
    TEST("MOVE.B #$34,($005678).W",            0010774, 0x0034, 0x5678);
    TEST("MOVE.W D2,($005678).W",              0030702, 0x5678);
    TEST("MOVE.W A2,($005678).W",              0030712, 0x5678);
    TEST("MOVE.W (A2),($005678).W",            0030722, 0x5678);
    TEST("MOVE.W (A2)+,($005678).W",           0030732, 0x5678);
    TEST("MOVE.W -(A2),($005678).W",           0030742, 0x5678);
    TEST("MOVE.W ($1234,A2),($005678).W",      0030752, 0x1234, 0x5678);
    TEST("MOVE.W ($12,A2,D3.L),($005678).W",   0030762, 0x3812, 0x5678);
    ABSW("MOVE.W (@W).W,($005678).W", "FFFE",  0030770, 0xFFFE, 0x5678);
    TEST("MOVE.W ($123456).L,($005678).W",     0030771, 0x0012, 0x3456, 0x5678);
    TEST("MOVE.W (*+$1234,PC),($005678).W",    0030772, 0x1232, 0x5678);
    TEST("MOVE.W (*+$12,PC,D3.W),($005678).W", 0030773, 0x3010, 0x5678);
    TEST("MOVE.W #$0034,($005678).W",          0030774, 0x0034, 0x5678);
    TEST("MOVE.L D2,($005678).W",              0020702, 0x5678);
    TEST("MOVE.L A2,($005678).W",              0020712, 0x5678);
    TEST("MOVE.L (A2),($005678).W",            0020722, 0x5678);
    TEST("MOVE.L (A2)+,($005678).W",           0020732, 0x5678);
    TEST("MOVE.L -(A2),($005678).W",           0020742, 0x5678);
    TEST("MOVE.L ($1234,A2),($005678).W",      0020752, 0x1234, 0x5678);
    TEST("MOVE.L ($12,A2,D3.L),($005678).W",   0020762, 0x3812, 0x5678);
    ABSW("MOVE.L (@W).W,($005678).W", "FFFC",  0020770, 0xFFFC, 0x5678);
    TEST("MOVE.L ($123454).L,($005678).W",     0020771, 0x0012, 0x3454, 0x5678);
    TEST("MOVE.L (*+$1234,PC),($005678).W",    0020772, 0x1232, 0x5678);
    TEST("MOVE.L (*+$12,PC,D3.W),($005678).W", 0020773, 0x3010, 0x5678);
    TEST("MOVE.L #$00345678,($005678).W",      0020774, 0x0034, 0x5678, 0x5678);
    TEST("MOVE.B D2,($56789A).L",              0011702, 0x0056, 0x789A);
    ERRT("MOVE.B A2,D2",                       OPERAND_NOT_ALLOWED, "A2,D2", 0012000);
    TEST("MOVE.B (A2),($56789A).L",            0011722, 0x0056, 0x789A);
    TEST("MOVE.B (A2)+,($56789A).L",           0011732, 0x0056, 0x789A);
    TEST("MOVE.B -(A2),($56789A).L",           0011742, 0x0056, 0x789A);
    TEST("MOVE.B ($1234,A2),($56789A).L",      0011752, 0x1234, 0x0056, 0x789A);
    TEST("MOVE.B ($12,A2,D3.L),($56789A).L",   0011762, 0x3812, 0x0056, 0x789A);
    ABSW("MOVE.B (@W).W,($56789A).L", "FFFF",  0011770, 0xFFFF, 0x0056, 0x789A);
    TEST("MOVE.B ($123456).L,($56789A).L",     0011771, 0x0012, 0x3456, 0x0056, 0x789A);
    TEST("MOVE.B (*+$1234,PC),($56789A).L",    0011772, 0x1232, 0x0056, 0x789A);
    TEST("MOVE.B (*+$12,PC,D3.W),($56789A).L", 0011773, 0x3010, 0x0056, 0x789A);
    TEST("MOVE.B #$34,($56789A).L",            0011774, 0x0034, 0x0056, 0x789A);
    TEST("MOVE.W D2,($56789A).L",              0031702, 0x0056, 0x789A);
    TEST("MOVE.W A2,($56789A).L",              0031712, 0x0056, 0x789A);
    TEST("MOVE.W (A2),($56789A).L",            0031722, 0x0056, 0x789A);
    TEST("MOVE.W (A2)+,($56789A).L",           0031732, 0x0056, 0x789A);
    TEST("MOVE.W -(A2),($56789A).L",           0031742, 0x0056, 0x789A);
    TEST("MOVE.W ($1234,A2),($56789A).L",      0031752, 0x1234, 0x0056, 0x789A);
    TEST("MOVE.W ($12,A2,D3.L),($56789A).L",   0031762, 0x3812, 0x0056, 0x789A);
    ABSW("MOVE.W (@W).W,($56789A).L", "FFFE",  0031770, 0xFFFE, 0x0056, 0x789A);
    TEST("MOVE.W ($123456).L,($56789A).L",     0031771, 0x0012, 0x3456, 0x0056, 0x789A);
    TEST("MOVE.W (*+$1234,PC),($56789A).L",    0031772, 0x1232, 0x0056, 0x789A);
    TEST("MOVE.W (*+$12,PC,D3.W),($56789A).L", 0031773, 0x3010, 0x0056, 0x789A);
    TEST("MOVE.W #$0034,($56789A).L",          0031774, 0x0034, 0x0056, 0x789A);
    TEST("MOVE.L D2,($567898).L",              0021702, 0x0056, 0x7898);
    TEST("MOVE.L A2,($567898).L",              0021712, 0x0056, 0x7898);
    TEST("MOVE.L (A2),($567898).L",            0021722, 0x0056, 0x7898);
    TEST("MOVE.L (A2)+,($567898).L",           0021732, 0x0056, 0x7898);
    TEST("MOVE.L -(A2),($567898).L",           0021742, 0x0056, 0x7898);
    TEST("MOVE.L ($1234,A2),($567898).L",      0021752, 0x1234, 0x0056, 0x7898);
    TEST("MOVE.L ($12,A2,D3.L),($567898).L",   0021762, 0x3812, 0x0056, 0x7898);
    ABSW("MOVE.L (@W).W,($567898).L", "FFFC",  0021770, 0xFFFC, 0x0056, 0x7898);
    TEST("MOVE.L ($123454).L,($567898).L",     0021771, 0x0012, 0x3454, 0x0056, 0x7898);
    TEST("MOVE.L (*+$1234,PC),($567898).L",    0021772, 0x1232, 0x0056, 0x7898);
    TEST("MOVE.L (*+$12,PC,D3.W),($567898).L", 0021773, 0x3010, 0x0056, 0x7898);
    TEST("MOVE.L #$00345678,($567898).L",      0021774, 0x0034, 0x5678, 0x0056, 0x7898);
    ERRT("MOVE.B D2,($1234,PC)",  OPERAND_NOT_ALLOWED, "($1234,PC)",  0010002);
    ERRT("MOVE.B D2,($12,PC,D3)", OPERAND_NOT_ALLOWED, "($12,PC,D3)", 0010002);
    ERRT("MOVE.B D2,#$34",        OPERAND_NOT_ALLOWED, "#$34",        0010002);

    // MOVEA src,An: 00|Sz|An|1|M|Rn, Sz:W=3/L=2
    TEST("MOVEA.W D2,A6",              0036102);
    TEST("MOVEA.W A2,A6",              0036112);
    TEST("MOVEA.W (A2),A6",            0036122);
    TEST("MOVEA.W (A2)+,A6",           0036132);
    TEST("MOVEA.W -(A2),A6",           0036142);
    TEST("MOVEA.W ($1234,A2),A6",      0036152, 0x1234);
    TEST("MOVEA.W ($12,A2,D3.L),A6",   0036162, 0x3812);
    ABSW("MOVEA.W (@W).W,A6", "FFFE",  0036170, 0xFFFE);
    TEST("MOVEA.W ($123456).L,A6",     0036171, 0x0012, 0x3456);
    TEST("MOVEA.W (*+$1234,PC),A6",    0036172, 0x1232);
    TEST("MOVEA.W (*+$12,PC,D3.W),A6", 0036173, 0x3010);
    TEST("MOVEA.W #$0034,A6",          0036174, 0x0034);
    TEST("MOVEA.L D2,A6",              0026102);
    TEST("MOVEA.L A2,A6",              0026112);
    TEST("MOVEA.L (A2),A6",            0026122);
    TEST("MOVEA.L (A2)+,A6",           0026132);
    TEST("MOVEA.L -(A2),A6",           0026142);
    TEST("MOVEA.L ($1234,A2),A6",      0026152, 0x1234);
    TEST("MOVEA.L ($12,A2,D3.L),A6",   0026162, 0x3812);
    ABSW("MOVEA.L (@W).W,A6", "FFFC",  0026170, 0xFFFC);
    TEST("MOVEA.L ($123454).L,A6",     0026171, 0x0012, 0x3454);
    TEST("MOVEA.L (*+$1234,PC),A6",    0026172, 0x1232);
    TEST("MOVEA.L (*+$12,PC,D3.W),A6", 0026173, 0x3010);
    TEST("MOVEA.L #$00345678,A6",      0026174, 0x0034, 0x5678);

    // MOVEM list,dst: 0044|Sz|Md|Rd, Sz:W=2/L=3, list=A7|...|D0, -(An)=D0|...|A7
    ERRT("MOVEM.W D0-D6,D2",            OPERAND_NOT_ALLOWED, "D2", 0044200);
    ERRT("MOVEM.W D0-D6,A2",            OPERAND_NOT_ALLOWED, "A2", 0044200);
    TEST("MOVEM.W D0-D6,(A2)",          0044222, 0x007F);
    ERRT("MOVEM.W D0-D6,(A2)+",         OPERAND_NOT_ALLOWED, "(A2)+", 0044200);
    TEST("MOVEM.W D0-D6,-(A2)",         0044242, 0xFE00);
    TEST("MOVEM.W D0-D6,($1234,A2)",    0044252, 0x007F, 0x1234);
    TEST("MOVEM.W D0-D6,($12,A2,A3.L)", 0044262, 0x007F, 0xB812);
    ABSW("MOVEM.W D0-D6,(@W).W", "FEDC", 0044270, 0x007F, 0xFEDC);
    TEST("MOVEM.W D0-D6,($123456).L",   0044271, 0x007F, 0x0012, 0x3456);
    ERRT("MOVEM.W D0-D6,($1234,PC)",    OPERAND_NOT_ALLOWED, "($1234,PC)",  0044200);
    ERRT("MOVEM.W D0-D6,($12,PC,D3)",   OPERAND_NOT_ALLOWED, "($12,PC,D3)", 0044200);
    ERRT("MOVEM.W D0-D6,#$1234",        OPERAND_NOT_ALLOWED, "#$1234",      0044200);
    TEST("MOVEM.W D0,(A2)",             0044222, 0x0001);
    TEST("MOVEM.W A0,(A2)",             0044222, 0x0100);
    TEST("MOVEM.W D0,-(A2)",            0044242, 0x8000);
    TEST("MOVEM.W A0,-(A2)",            0044242, 0x0080);
    ERRT("MOVEM.L D1/A0,D2",            OPERAND_NOT_ALLOWED, "D2", 0044300);
    ERRT("MOVEM.L D1/A0,A2",            OPERAND_NOT_ALLOWED, "A2", 0044300);
    TEST("MOVEM.L D1/A0,(A2)",          0044322, 0x0102);
    ERRT("MOVEM.L D1/A0,(A2)+",         OPERAND_NOT_ALLOWED, "(A2)+", 0044300);
    TEST("MOVEM.L D1/A0,-(A2)",         0044342, 0x4080);
    TEST("MOVEM.L D1/A0,($1234,A2)",    0044352, 0x0102, 0x1234);
    TEST("MOVEM.L D1/A0,($12,A2,A3.L)", 0044362, 0x0102, 0xB812);
    ABSW("MOVEM.L D1/A0,(@W).W", "FEDC", 0044370, 0x0102, 0xFEDC);
    TEST("MOVEM.L D1/A0,($123454).L",   0044371, 0x0102, 0x0012, 0x3454);
    ERRT("MOVEM.L D1/A0,($1234,PC)",    OPERAND_NOT_ALLOWED, "($1234,PC)",  0044300);
    ERRT("MOVEM.L D1/A0,($12,PC,A3)",   OPERAND_NOT_ALLOWED, "($12,PC,A3)", 0044300);
    ERRT("MOVEM.L D1/A0,#$1234",        OPERAND_NOT_ALLOWED, "#$1234",      0044300);
    TEST("MOVEM.L D0,(A2)",             0044322, 0x0001);
    TEST("MOVEM.L A0,(A2)",             0044322, 0x0100);
    TEST("MOVEM.L D0,-(A2)",            0044342, 0x8000);
    TEST("MOVEM.L A0,-(A2)",            0044342, 0x0080);

    // MOVEM src,list: 0046|Sz|Md|Rd, Sz:W=2/L=3, list=A7|...|D0
    ERRT("MOVEM.W D2,A3-A6",              OPERAND_NOT_ALLOWED, "D2,A3-A6", 0046200);
    ERRT("MOVEM.W A2,A3-A6",              OPERAND_NOT_ALLOWED, "A2,A3-A6", 0046200);
    TEST("MOVEM.W (A2),A3-A6",            0046222, 0x7800);
    TEST("MOVEM.W (A2)+,A3-A6",           0046232, 0x7800);
    ERRT("MOVEM.W -(A2),A3-A6",           OPERAND_NOT_ALLOWED, "-(A2),A3-A6", 0046200);
    TEST("MOVEM.W ($1234,A2),A3-A6",      0046252, 0x7800, 0x1234);
    TEST("MOVEM.W ($12,A2,D3.W),A3-A6",   0046262, 0x7800, 0x3012);
    ABSW("MOVEM.W (@W).W,A3-A6", "FEDC",  0046270, 0x7800, 0xFEDC);
    TEST("MOVEM.W ($123456).L,A3-A6",     0046271, 0x7800, 0x0012, 0x3456);
    TEST("MOVEM.W (*+$1234,PC),A3-A6",    0046272, 0x7800, 0x1230);
    TEST("MOVEM.W (*+$12,PC,D3.L),A3-A6", 0046273, 0x7800, 0x380E);
    ERRT("MOVEM.W #$1234,A3-A6",          OPERAND_NOT_ALLOWED, "#$1234,A3-A6", 0046200);
    TEST("MOVEM.W (A2),D0",               0046222, 0x0001);
    TEST("MOVEM.W (A2),A0",               0046222, 0x0100);
    TEST("MOVEM.W (A2)+,D0",              0046232, 0x0001);
    TEST("MOVEM.W (A2)+,A0",              0046232, 0x0100);
    ERRT("MOVEM.L D2,A3-A6",              OPERAND_NOT_ALLOWED, "D2,A3-A6", 0046300);
    ERRT("MOVEM.L A2,A3-A6",              OPERAND_NOT_ALLOWED, "A2,A3-A6", 0046300);
    TEST("MOVEM.L (A2),A3-A6",            0046322, 0x7800);
    TEST("MOVEM.L (A2)+,A3-A6",           0046332, 0x7800);
    ERRT("MOVEM.L -(A2),A3-A6",           OPERAND_NOT_ALLOWED, "-(A2),A3-A6", 0046300);
    TEST("MOVEM.L ($1234,A2),A3-A6",      0046352, 0x7800, 0x1234);
    TEST("MOVEM.L ($12,A2,D3.W),A3-A6",   0046362, 0x7800, 0x3012);
    ABSW("MOVEM.L (@W).W,A3-A6", "FEDC",  0046370, 0x7800, 0xFEDC);
    TEST("MOVEM.L ($123454).L,A3-A6",     0046371, 0x7800, 0x0012, 0x3454);
    TEST("MOVEM.L (*+$1234,PC),A3-A6",    0046372, 0x7800, 0x1230);
    TEST("MOVEM.L (*+$12,PC,D3.L),A3-A6", 0046373, 0x7800, 0x380E);
    ERRT("MOVEM.L #$1234,A3-A6",          OPERAND_NOT_ALLOWED, "#$1234,A3-A6", 0046300);
    TEST("MOVEM.L (A2),D0",               0046322, 0x0001);
    TEST("MOVEM.L (A2),A0",               0046322, 0x0100);
    TEST("MOVEM.L (A2)+,D0",              0046332, 0x0001);
    TEST("MOVEM.L (A2)+,A0",              0046332, 0x0100);

    // MOVEP Dn,(d16,An): 000|Dn|Sz|1|An, SZ:W=6/L=7
    ERRT("MOVEP.W D7,D2",            OPERAND_NOT_ALLOWED, "D7,D2");
    ERRT("MOVEP.W D7,A2",            OPERAND_NOT_ALLOWED, "D7,A2");
    ERRT("MOVEP.W D7,(A2)",          OPERAND_NOT_ALLOWED, "D7,(A2)");
    ERRT("MOVEP.W D7,(A2)+",         OPERAND_NOT_ALLOWED, "D7,(A2)+");
    ERRT("MOVEP.W D7,-(A2)",         OPERAND_NOT_ALLOWED, "D7,-(A2)");
    TEST("MOVEP.W D7,($1234,A2)",    0007612, 0x1234);
    ERRT("MOVEP.W D7,($12,A2,D3)",   OPERAND_NOT_ALLOWED, "D7,($12,A2,D3)");
    ERRT("MOVEP.W D7,($001234).W",   OPERAND_NOT_ALLOWED, "D7,($001234).W");
    ERRT("MOVEP.W D7,($123456).L",   OPERAND_NOT_ALLOWED, "D7,($123456).L");
    ERRT("MOVEP.W D7,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "D7,(*+$1234,PC)");
    ERRT("MOVEP.W D7,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "D7,(*+$12,PC,D3)");
    ERRT("MOVEP.W D7,#$1234",        OPERAND_NOT_ALLOWED, "D7,#$1234");
    ERRT("MOVEP.W D7,D2",            OPERAND_NOT_ALLOWED, "D7,D2");
    ERRT("MOVEP.W D7,A2",            OPERAND_NOT_ALLOWED, "D7,A2");
    ERRT("MOVEP.W D7,(A2)",          OPERAND_NOT_ALLOWED, "D7,(A2)");
    ERRT("MOVEP.W D7,(A2)+",         OPERAND_NOT_ALLOWED, "D7,(A2)+");
    ERRT("MOVEP.W D7,-(A2)",         OPERAND_NOT_ALLOWED, "D7,-(A2)");
    TEST("MOVEP.L D7,($1234,A2)",    0007712, 0x1234);
    ERRT("MOVEP.W D7,($12,A2,D3)",   OPERAND_NOT_ALLOWED, "D7,($12,A2,D3)");
    ERRT("MOVEP.W D7,($001234).W",   OPERAND_NOT_ALLOWED, "D7,($001234).W");
    ERRT("MOVEP.W D7,($123456).L",   OPERAND_NOT_ALLOWED, "D7,($123456).L");
    ERRT("MOVEP.W D7,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "D7,(*+$1234,PC)");
    ERRT("MOVEP.W D7,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "D7,(*+$12,PC,D3)");
    ERRT("MOVEP.W D7,#$1234",        OPERAND_NOT_ALLOWED, "D7,#$1234");

    // MOVEP (d16,An),Dn: 000|Dn|Sz|1|An, SZ:W=4/L=5
    ERRT("MOVEP.W D2,D7",            OPERAND_NOT_ALLOWED, "D2,D7");
    ERRT("MOVEP.W A2,D7",            OPERAND_NOT_ALLOWED, "A2,D7");
    ERRT("MOVEP.W (A2),D7",          OPERAND_NOT_ALLOWED, "(A2),D7");
    ERRT("MOVEP.W (A2)+,D7",         OPERAND_NOT_ALLOWED, "(A2)+,D7");
    ERRT("MOVEP.W -(A2),D7",         OPERAND_NOT_ALLOWED, "-(A2),D7");
    TEST("MOVEP.W ($1234,A2),D7",    0007412, 0x1234);
    ERRT("MOVEP.W ($12,A2,D3),D7",   OPERAND_NOT_ALLOWED, "($12,A2,D3),D7");
    ERRT("MOVEP.W ($001234).W,D7",   OPERAND_NOT_ALLOWED, "($001234).W,D7");
    ERRT("MOVEP.W ($123456).L,D7",   OPERAND_NOT_ALLOWED, "($123456).L,D7");
    ERRT("MOVEP.W (*+$1234,PC),D7",  OPERAND_NOT_ALLOWED, "(*+$1234,PC),D7");
    ERRT("MOVEP.W (*+$12,PC,D3),D7", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3),D7");
    ERRT("MOVEP.W #$1234,D7",        OPERAND_NOT_ALLOWED, "#$1234,D7");
    ERRT("MOVEP.L D2,D7",            OPERAND_NOT_ALLOWED, "D2,D7");
    ERRT("MOVEP.L A2,D7",            OPERAND_NOT_ALLOWED, "A2,D7");
    ERRT("MOVEP.L (A2),D7",          OPERAND_NOT_ALLOWED, "(A2),D7");
    ERRT("MOVEP.L (A2)+,D7",         OPERAND_NOT_ALLOWED, "(A2)+,D7");
    ERRT("MOVEP.L -(A2),D7",         OPERAND_NOT_ALLOWED, "-(A2),D7");
    TEST("MOVEP.L ($1234,A2),D7",    0007512, 0x1234);
    ERRT("MOVEP.L ($12,A2,D3),D7",   OPERAND_NOT_ALLOWED, "($12,A2,D3),D7");
    ERRT("MOVEP.L ($001234).W,D7",   OPERAND_NOT_ALLOWED, "($001234).W,D7");
    ERRT("MOVEP.L ($123456).L,D7",   OPERAND_NOT_ALLOWED, "($123456).L,D7");
    ERRT("MOVEP.L (*+$1234,PC),D7",  OPERAND_NOT_ALLOWED, "(*+$1234,PC),D7");
    ERRT("MOVEP.L (*+$12,PC,D3),D7", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3),D7");
    ERRT("MOVEP.L #$1234,D7",        OPERAND_NOT_ALLOWED, "#$1234,D7");

    if (mc68k00()) {
        ERUI("MOVES.B D3, -(A4)");
        ERUI("MOVES.W D3, -(A4)");
        ERUI("MOVES.L D3, -(A4)");
    } else {
        ERRT("MOVES.B D0, D1", OPERAND_NOT_ALLOWED, "D0, D1", 0007000, 0010000);
        ERRT("MOVES.B A1, A2", OPERAND_NOT_ALLOWED, "A1, A2", 0007000, 0120000);
        TEST("MOVES.B (A2), D1",         0007022, 0010000);
        TEST("MOVES.B (A3)+, A2",        0007033, 0120000);
        TEST("MOVES.B -(A4), D3",        0007044, 0030000);
        TEST("MOVES.B ($1234,A5), A4",   0007055, 0140000, 0x1234);
        TEST("MOVES.B (18,A6,D3.W), D5", 0007066, 0050000, 0x3012);
        TEST("MOVES.B ($001234).W, A6",  0007070, 0160000, 0x1234);
        TEST("MOVES.B ($123456).L, D7",  0007071, 0070000, 0x0012, 0x3456);
        ERRT("MOVES.B ($1234,PC), A0",   OPERAND_NOT_ALLOWED, "($1234,PC), A0",  0007000, 0100000);
        ERRT("MOVES.B ($12,PC,A2), D1",  OPERAND_NOT_ALLOWED, "($12,PC,A2), D1", 0007000, 0010000);
        ERRT("MOVES.B #$1234, A2",       OPERAND_NOT_ALLOWED, "#$1234, A2",      0007000, 0120000);
        ERRT("MOVES.W D0, D1", OPERAND_NOT_ALLOWED, "D0, D1", 0007100, 0010000);
        ERRT("MOVES.W A1, A2", OPERAND_NOT_ALLOWED, "A1, A2", 0007100, 0120000);
        TEST("MOVES.W (A2), D1",         0007122, 0010000);
        TEST("MOVES.W (A3)+, A2",        0007133, 0120000);
        TEST("MOVES.W -(A4), D3",        0007144, 0030000);
        TEST("MOVES.W ($1234,A5), A4",   0007155, 0140000, 0x1234);
        TEST("MOVES.W (18,A6,D3.W), D5", 0007166, 0050000, 0x3012);
        TEST("MOVES.W ($001234).W, A6",  0007170, 0160000, 0x1234);
        TEST("MOVES.W ($123456).L, D7",  0007171, 0070000, 0x0012, 0x3456);
        ERRT("MOVES.W ($1234,PC), A0",  OPERAND_NOT_ALLOWED, "($1234,PC), A0",  0007100, 0100000);
        ERRT("MOVES.W ($12,PC,A2), D1", OPERAND_NOT_ALLOWED, "($12,PC,A2), D1", 0007100, 0010000);
        ERRT("MOVES.W #$1234, A2",      OPERAND_NOT_ALLOWED, "#$1234, A2",      0007100, 0120000);
        ERRT("MOVES.L D0, D1", OPERAND_NOT_ALLOWED, "D0, D1", 0007200, 0010000);
        ERRT("MOVES.L A1, A2", OPERAND_NOT_ALLOWED, "A1, A2", 0007200, 0120000);
        TEST("MOVES.L (A2), D1",         0007222, 0010000);
        TEST("MOVES.L (A3)+, A2",        0007233, 0120000);
        TEST("MOVES.L -(A4), D3",        0007244, 0030000);
        TEST("MOVES.L ($1234,A5), A4",   0007255, 0140000, 0x1234);
        TEST("MOVES.L (18,A6,D3.W), D5", 0007266, 0050000, 0x3012);
        TEST("MOVES.L ($001234).W, A6",  0007270, 0160000, 0x1234);
        TEST("MOVES.L ($123456).L, D7",  0007271, 0070000, 0x0012, 0x3456);
        ERRT("MOVES.L ($1234,PC), A0",  OPERAND_NOT_ALLOWED, "($1234,PC), A0",  0007200, 0100000);
        ERRT("MOVES.L ($12,PC,A2), D1", OPERAND_NOT_ALLOWED, "($12,PC,A2), D1", 0007200, 0010000);
        ERRT("MOVES.L #$1234, A2",      OPERAND_NOT_ALLOWED, "#$1234, A2",      0007200, 0120000);

        TEST("MOVES.B D1, (A2)",         0007022, 0014000);
        TEST("MOVES.B A2, (A3)+",        0007033, 0124000);
        TEST("MOVES.B D3, -(A4)",        0007044, 0034000);
        TEST("MOVES.B A4, ($1234,A5)",   0007055, 0144000, 0x1234);
        TEST("MOVES.B D5, (18,A6,D3.W)", 0007066, 0054000, 0x3012);
        TEST("MOVES.B A6, ($001234).W",  0007070, 0164000, 0x1234);
        TEST("MOVES.B D7, ($123456).L",  0007071, 0074000, 0x0012, 0x3456);
        ERRT("MOVES.B A0, ($1234,PC)",  OPERAND_NOT_ALLOWED, "($1234,PC)",  0007000, 0104000);
        ERRT("MOVES.B D1, ($12,PC,A2)", OPERAND_NOT_ALLOWED, "($12,PC,A2)", 0007000, 0014000);
        ERRT("MOVES.B A2, #$1234",      OPERAND_NOT_ALLOWED, "#$1234",      0007000, 0124000);
        TEST("MOVES.W D1, (A2)",         0007122, 0014000);
        TEST("MOVES.W A2, (A3)+",        0007133, 0124000);
        TEST("MOVES.W D3, -(A4)",        0007144, 0034000);
        TEST("MOVES.W A4, ($1234,A5)",   0007155, 0144000, 0x1234);
        TEST("MOVES.W D5, (18,A6,D3.W)", 0007166, 0054000, 0x3012);
        TEST("MOVES.W A6, ($001234).W",  0007170, 0164000, 0x1234);
        TEST("MOVES.W D7, ($123456).L",  0007171, 0074000, 0x0012, 0x3456);
        ERRT("MOVES.W A0, ($1234,PC)",   OPERAND_NOT_ALLOWED, "($1234,PC)",  0007100, 0104000);
        ERRT("MOVES.W D1, ($12,PC,A2)",  OPERAND_NOT_ALLOWED, "($12,PC,A2)", 0007100, 0014000);
        ERRT("MOVES.W A2, #$1234",       OPERAND_NOT_ALLOWED, "#$1234",      0007100, 0124000);
        TEST("MOVES.L D1, (A2)",         0007222, 0014000);
        TEST("MOVES.L A2, (A3)+",        0007233, 0124000);
        TEST("MOVES.L D3, -(A4)",        0007244, 0034000);
        TEST("MOVES.L A4, ($1234,A5)",   0007255, 0144000, 0x1234);
        TEST("MOVES.L D5, (18,A6,D3.W)", 0007266, 0054000, 0x3012);
        TEST("MOVES.L A6, ($001234).W",  0007270, 0164000, 0x1234);
        TEST("MOVES.L D7, ($123456).L",  0007271, 0074000, 0x0012, 0x3456);
        ERRT("MOVES.L A0, ($1234,PC)",  OPERAND_NOT_ALLOWED, "($1234,PC)",  0007200, 0104000);
        ERRT("MOVES.L D1, ($12,PC,A2)", OPERAND_NOT_ALLOWED, "($12,PC,A2)", 0007200, 0014000);
        ERRT("MOVES.L A2, #$1234",      OPERAND_NOT_ALLOWED, "#$1234",      0007200, 0124000);
    }

    // MOVEQ #nn,Dn: 007|Dn|000 + nn
    ERRT("MOVEQ D2,D7",            OPERAND_NOT_ALLOWED, "D2,D7");
    ERRT("MOVEQ A2,D7",            OPERAND_NOT_ALLOWED, "A2,D7");
    ERRT("MOVEQ (A2),D7",          OPERAND_NOT_ALLOWED, "(A2),D7");
    ERRT("MOVEQ (A2)+,D7",         OPERAND_NOT_ALLOWED, "(A2)+,D7");
    ERRT("MOVEQ -(A2),D7",         OPERAND_NOT_ALLOWED, "-(A2),D7");
    ERRT("MOVEQ ($12,A2,D3),D7",   OPERAND_NOT_ALLOWED, "($12,A2,D3),D7");
    ERRT("MOVEQ ($001234).W,D7",   OPERAND_NOT_ALLOWED, "($001234).W,D7");
    ERRT("MOVEQ ($123456).L,D7",   OPERAND_NOT_ALLOWED, "($123456).L,D7");
    ERRT("MOVEQ (*+$1234,PC),D7",  OPERAND_NOT_ALLOWED, "(*+$1234,PC),D7");
    ERRT("MOVEQ (*+$12,PC,D3),D7", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3),D7");
    TEST("MOVEQ #0,D0",            0070000 | 0x00);
    TEST("MOVEQ #$7F,D0",          0070000 | 0x7F);
    ERRT("MOVEQ #$80,D0",          OVERFLOW_RANGE, "#$80,D0", 0070000 | 0x80);
    TEST("MOVEQ #-1,D7",           0077000 | 0xFF);
    TEST("MOVEQ #-$80,D0",         0070000 | 0x80);
    ERRT("MOVEQ #-$81,D0",         OVERFLOW_RANGE, "#-$81,D0", 0070000 | 0x7F);

    if (mc68040()) {
        // MOVE16 (Ax)+, (Ay)+: 0173040|Ax + 1|Ay|00000
        TEST("MOVE16 (A1)+, (A2)+",         0173040 | 1, 0100000 | (2 << 12));
        // MOVE16 (Ax)+, (n).L: 0173000|Ax
        TEST("MOVE16 (A1)+, ($12345678).L", 0173000 | 1, 0x1234, 0x5678);
        // MOVE16 (n).L, (Ax)+: 0173010|Ax
        TEST("MOVE16 ($12345678).L, (A1)+", 0173010 | 1, 0x1234, 0x5678);
        // MOVE16 (Ax)+, (n).L: 0173020|Ax
        TEST("MOVE16 (A1), ($12345678).L",  0173020 | 1, 0x1234, 0x5678);
        // MOVE16 (Ax)+, (n).L: 0173030|Ax
        TEST("MOVE16 ($12345678).L, (A1)",  0173030 | 1, 0x1234, 0x5678);
    }

    // PEA src: 00441|M|Rn
    ERRT("PEA D2",              OPERAND_NOT_ALLOWED, "D2", 0044100);
    ERRT("PEA A2",              OPERAND_NOT_ALLOWED, "A2", 0044100);
    TEST("PEA (A2)",            0044122);
    ERRT("PEA (A2)+",           OPERAND_NOT_ALLOWED, "(A2)+", 0044100);
    ERRT("PEA -(A2)",           OPERAND_NOT_ALLOWED, "-(A2)", 0044100);
    TEST("PEA ($1234,A2)",      0044152, 0x1234);
    TEST("PEA ($12,A2,D3.W)",   0044162, 0x3012);
    ABSW("PEA (@W).W", "FF00",  0044170, 0xFF00);
    TEST("PEA ($001234).L",     0044171, 0x0000, 0x1234);
    TEST("PEA (*+$1234,PC)",    0044172, 0x1232);
    TEST("PEA (*+$12,PC,A3.L)", 0044173, 0xB810);
    ERRT("PEA #$1234",          OPERAND_NOT_ALLOWED, "#$1234", 0044100);

    // UNLK An: 004713|An
    ERRT("UNLK D2",              OPERAND_NOT_ALLOWED, "D2");
    TEST("UNLK A2",              0047132);
    ERRT("UNLK (A2)",            OPERAND_NOT_ALLOWED, "(A2)");
    ERRT("UNLK (A2)+",           OPERAND_NOT_ALLOWED, "(A2)+");
    ERRT("UNLK -(A2)",           OPERAND_NOT_ALLOWED, "-(A2)");
    ERRT("UNLK ($1234,A2)",      OPERAND_NOT_ALLOWED, "($1234,A2)");
    ERRT("UNLK ($12,A2,D3.W)",   OPERAND_NOT_ALLOWED, "($12,A2,D3.W)");
    ERRT("UNLK ($7F00).W",       OPERAND_NOT_ALLOWED, "($7F00).W");
    ERRT("UNLK ($001234).L",     OPERAND_NOT_ALLOWED, "($001234).L");
    ERRT("UNLK (*+$1234,PC)",    OPERAND_NOT_ALLOWED, "(*+$1234,PC)");
    ERRT("UNLK (*+$12,PC,A3.L)", OPERAND_NOT_ALLOWED, "(*+$12,PC,A3.L)");
    ERRT("UNLK #$1234",          OPERAND_NOT_ALLOWED, "#$1234");

    symtab.intern(0x00000034, "data$34");
    symtab.intern(0x00345678, "long_data");
    symtab.intern(0x00005678, ".offset");

    TEST("MOVE.B #data$34,D7",           0017074, 0x0034);
    TEST("MOVE.W #data$34,(A6)",         0036274, 0x0034);
    TEST("MOVE.L #long_data,(A6)+",      0026374, 0x0034, 0x5678);
    TEST("MOVE.W #data$34,(.offset,A6)", 0036574, 0x0034, 0x5678);
}

void test_integer() {
    // ADD Dn,dst: 015|Dn|Sz|M|Rn, Sz:B=4/W=5/L=6
    TEST("ADD.B D7,D2",            0152007);
    ERRT("ADD.B D7,A2",            OPERAND_NOT_ALLOWED, "A2", 0157400);
    TEST("ADD.B D7,(A2)",          0157422);
    TEST("ADD.B D7,(A2)+",         0157432);
    TEST("ADD.B D7,-(A2)",         0157442);
    TEST("ADD.B D7,($1234,A2)",    0157452, 0x1234);
    TEST("ADD.B D7,($12,A2,D3.L)", 0157462, 0x3812);
    ABSW("ADD.B D7,(@W).W", "FFFF", 0157470, 0xFFFF);
    TEST("ADD.B D7,($123456).L",   0157471, 0x0012, 0x3456);
    ERRT("ADD.B D7,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0157400);
    ERRT("ADD.B D7,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0157400);
    ERRT("ADD.B D7,#$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0157400);
    TEST("ADD.W D7,D2",            0152107);
    TEST("ADD.W D7,A2",            0152307); // ADDA.W
    TEST("ADD.W D7,(A2)",          0157522);
    TEST("ADD.W D7,(A2)+",         0157532);
    TEST("ADD.W D7,-(A2)",         0157542);
    TEST("ADD.W D7,($1234,A2)",    0157552, 0x1234);
    TEST("ADD.W D7,($12,A2,D3.L)", 0157562, 0x3812);
    ABSW("ADD.W D7,(@W).W", "FFFE", 0157570, 0xFFFE);
    TEST("ADD.W D7,($123456).L",   0157571, 0x0012, 0x3456);
    ERRT("ADD.W D7,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0157500);
    ERRT("ADD.W D7,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0157500);
    ERRT("ADD.W D7,#$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0157500);
    TEST("ADD.L D7,D2",            0152207);
    TEST("ADD.L D7,A2",            0152707); // ADDA.L;
    TEST("ADD.L D7,(A2)",          0157622);
    TEST("ADD.L D7,(A2)+",         0157632);
    TEST("ADD.L D7,-(A2)",         0157642);
    TEST("ADD.L D7,($1234,A2)",    0157652, 0x1234);
    TEST("ADD.L D7,($12,A2,D3.L)", 0157662, 0x3812);
    ABSW("ADD.L D7,(@W).W", "FFFC", 0157670, 0xFFFC);
    TEST("ADD.L D7,($123454).L",   0157671, 0x0012, 0x3454);
    ERRT("ADD.L D7,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0157600);
    ERRT("ADD.L D7,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0157600);
    ERRT("ADD.L D7,#$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0157600);

    // ADD src,Dn: 015|Dn|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("ADD.B D2,D7",              0157002);
    ERRT("ADD.B A2,D7",              OPERAND_NOT_ALLOWED, "A2,D7", 0157012);
    TEST("ADD.B (A2),D7",            0157022);
    TEST("ADD.B (A2)+,D7",           0157032);
    TEST("ADD.B -(A2),D7",           0157042);
    TEST("ADD.B ($1234,A2),D7",      0157052, 0x1234);
    TEST("ADD.B ($12,A2,D3.L),D7",   0157062, 0x3812);
    ABSW("ADD.B (@W).W,D7", "FFFF",  0157070, 0xFFFF);
    TEST("ADD.B ($123456).L,D7",     0157071, 0x0012, 0x3456);
    TEST("ADD.B (*+$1234,PC),D7",    0157072, 0x1232);
    TEST("ADD.B (*+$12,PC,D3.W),D7", 0157073, 0x3010);
    TEST("ADD.B #$34,D7",            0157074, 0x0034);
    TEST("ADD.W D2,D7",              0157102);
    TEST("ADD.W A2,D7",              0157112);
    TEST("ADD.W (A2),D7",            0157122);
    TEST("ADD.W (A2)+,D7",           0157132);
    TEST("ADD.W -(A2),D7",           0157142);
    TEST("ADD.W ($1234,A2),D7",      0157152, 0x1234);
    TEST("ADD.W ($12,A2,D3.L),D7",   0157162, 0x3812);
    ABSW("ADD.W (@W).W,D7", "FFFE",  0157170, 0xFFFE);
    TEST("ADD.W ($123456).L,D7",     0157171, 0x0012, 0x3456);
    TEST("ADD.W (*+$1234,PC),D7",    0157172, 0x1232);
    TEST("ADD.W (*+$12,PC,D3.W),D7", 0157173, 0x3010);
    TEST("ADD.W #$0034,D7",          0157174, 0x0034);
    TEST("ADD.L D2,D7",              0157202);
    TEST("ADD.L A2,D7",              0157212);
    TEST("ADD.L (A2),D7",            0157222);
    TEST("ADD.L (A2)+,D7",           0157232);
    TEST("ADD.L -(A2),D7",           0157242);
    TEST("ADD.L ($1234,A2),D7",      0157252, 0x1234);
    TEST("ADD.L ($12,A2,D3.L),D7",   0157262, 0x3812);
    ABSW("ADD.L (@W).W,D7", "FFFC",  0157270, 0xFFFC);
    TEST("ADD.L ($123454).L,D7",     0157271, 0x0012, 0x3454);
    TEST("ADD.L (*+$1234,PC),D7",    0157272, 0x1232);
    TEST("ADD.L (*+$12,PC,D3.W),D7", 0157273, 0x3010);
    TEST("ADD.L #$00345678,D7",      0157274, 0x0034, 0x5678);

    // ADDA src,An: 015|An|Sz|M|Rn, Sz:W=3/L=7
    TEST("ADDA.W D2,A6",              0156302);
    TEST("ADDA.W A2,A6",              0156312);
    TEST("ADDA.W (A2),A6",            0156322);
    TEST("ADDA.W (A2)+,A6",           0156332);
    TEST("ADDA.W -(A2),A6",           0156342);
    TEST("ADDA.W ($1234,A2),A6",      0156352, 0x1234);
    TEST("ADDA.W ($12,A2,D3.L),A6",   0156362, 0x3812);
    ABSW("ADDA.W (@W).W,A6", "FFFE",  0156370, 0xFFFE);
    TEST("ADDA.W ($123456).L,A6",     0156371, 0x0012, 0x3456);
    TEST("ADDA.W (*+$1234,PC),A6",    0156372, 0x1232);
    TEST("ADDA.W (*+$12,PC,D3.W),A6", 0156373, 0x3010);
    TEST("ADDA.W #$0034,A6",          0156374, 0x0034);
    TEST("ADDA.L D2,A6",              0156702);
    TEST("ADDA.L A2,A6",              0156712);
    TEST("ADDA.L (A2),A6",            0156722);
    TEST("ADDA.L (A2)+,A6",           0156732);
    TEST("ADDA.L -(A2),A6",           0156742);
    TEST("ADDA.L ($1234,A2),A6",      0156752, 0x1234);
    TEST("ADDA.L ($12,A2,D3.L),A6",   0156762, 0x3812);
    ABSW("ADDA.L (@W).W,A6", "FFFC",  0156770, 0xFFFC);
    TEST("ADDA.L ($123454).L,A6",     0156771, 0x0012, 0x3454);
    TEST("ADDA.L (*+$1234,PC),A6",    0156772, 0x1232);
    TEST("ADDA.L (*+$12,PC,D3.W),A6", 0156773, 0x3010);
    TEST("ADDA.L #$00345678,A6",      0156774, 0x0034, 0x5678);

    // ADDI #$1234,dst: 0003|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("ADDI.B #$12,D2",            0003002, 0x0012);
    ERRT("ADDI.B #$12,A2",            OPERAND_NOT_ALLOWED, "A2", 0003000);
    TEST("ADDI.B #$12,(A2)",          0003022, 0x0012);
    TEST("ADDI.B #$12,(A2)+",         0003032, 0x0012);
    TEST("ADDI.B #$12,-(A2)",         0003042, 0x0012);
    TEST("ADDI.B #$12,($1234,A2)",    0003052, 0x0012, 0x1234);
    TEST("ADDI.B #$12,($12,A2,D3.W)", 0003062, 0x0012, 0x3012);
    TEST("ADDI.B #$12,($001234).W",   0003070, 0x0012, 0x1234);
    TEST("ADDI.B #$12,($123456).L",   0003071, 0x0012, 0x0012, 0x3456);
    ERRT("ADDI.B #$12,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0003000);
    ERRT("ADDI.B #$12,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0003000);
    ERRT("ADDI.B #$12,#$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0003000);
    TEST("ADDI.W #$5678,D2",            0003102, 0x5678);
    TEST("ADDI.W #$5678,A2",            0152374, 0x5678); // ADDA.W
    TEST("ADDI.W #$5678,(A2)",          0003122, 0x5678);
    TEST("ADDI.W #$5678,(A2)+",         0003132, 0x5678);
    TEST("ADDI.W #$5678,-(A2)",         0003142, 0x5678);
    TEST("ADDI.W #$5678,($1234,A2)",    0003152, 0x5678, 0x1234);
    TEST("ADDI.W #$5678,($12,A2,D3.W)", 0003162, 0x5678, 0x3012);
    TEST("ADDI.W #$5678,($001234).W",   0003170, 0x5678, 0x1234);
    TEST("ADDI.W #$5678,($123456).L",   0003171, 0x5678, 0x0012, 0x3456);
    ERRT("ADDI.W #$5678,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0003100);
    ERRT("ADDI.W #$5678,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0003100);
    ERRT("ADDI.W #$5678,#$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0003100);
    TEST("ADDI.L #$3456789A,D2",            0003202, 0x3456, 0x789A);
    TEST("ADDI.L #$3456789A,A2",            0152774, 0x3456, 0x789A); // ADDA.L
    TEST("ADDI.L #$3456789A,(A2)",          0003222, 0x3456, 0x789A);
    TEST("ADDI.L #$3456789A,(A2)+",         0003232, 0x3456, 0x789A);
    TEST("ADDI.L #$3456789A,-(A2)",         0003242, 0x3456, 0x789A);
    TEST("ADDI.L #$3456789A,($1234,A2)",    0003252, 0x3456, 0x789A, 0x1234);
    TEST("ADDI.L #$3456789A,($12,A2,D3.W)", 0003262, 0x3456, 0x789A, 0x3012);
    TEST("ADDI.L #$3456789A,($001234).W",   0003270, 0x3456, 0x789A, 0x1234);
    TEST("ADDI.L #$34567898,($123454).L",   0003271, 0x3456, 0x7898, 0x0012, 0x3454);
    ERRT("ADDI.L #$3456789A,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0003200);
    ERRT("ADDI.L #$3456789A,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0003200);
    ERRT("ADDI.L #$3456789A,#$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0003200);

    // ADDQ #nn,dst: 005|nn|Sz|M|Rn, Sz:B=0/W=1/L=2
    ERRT("ADDQ.B #0,D2",            OPERAND_NOT_ALLOWED, "#0,D2", 0050002);
    TEST("ADDQ.B #1,D2",            0051002);
    TEST("ADDQ.B #2,D2",            0052002);
    TEST("ADDQ.B #3,D2",            0053002);
    TEST("ADDQ.B #4,D2",            0054002);
    TEST("ADDQ.B #5,D2",            0055002);
    TEST("ADDQ.B #6,D2",            0056002);
    TEST("ADDQ.B #7,D2",            0057002);
    TEST("ADDQ.B #8,D2",            0050002);
    ERRT("ADDQ.B #9,D2",            OVERFLOW_RANGE, "#9,D2",  0051002);
    ERRT("ADDQ.B #-1,D2",           OVERFLOW_RANGE, "#-1,D2", 0057002);
    ERRT("ADDQ.B #8,A2",            OPERAND_NOT_ALLOWED, "A2", 0050012);
    TEST("ADDQ.B #8,(A2)",          0050022);
    TEST("ADDQ.B #8,(A2)+",         0050032);
    TEST("ADDQ.B #8,-(A2)",         0050042);
    TEST("ADDQ.B #8,($1234,A2)",    0050052, 0x1234);
    TEST("ADDQ.B #8,($12,A2,D3.W)", 0050062, 0x3012);
    TEST("ADDQ.B #8,($001234).W",   0050070, 0x1234);
    TEST("ADDQ.B #8,($123456).L",   0050071, 0x0012, 0x3456);
    ERRT("ADDQ.B #8,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0050000);
    ERRT("ADDQ.B #8,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0050000);
    ERRT("ADDQ.B #8,#$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0050000);
    TEST("ADDQ.W #8,D2",            0050102);
    TEST("ADDQ.W #8,A2",            0050112);
    TEST("ADDQ.W #8,(A2)",          0050122);
    TEST("ADDQ.W #8,(A2)+",         0050132);
    TEST("ADDQ.W #8,-(A2)",         0050142);
    TEST("ADDQ.W #8,($1234,A2)",    0050152, 0x1234);
    TEST("ADDQ.W #8,($12,A2,D3.W)", 0050162, 0x3012);
    TEST("ADDQ.W #8,($001234).W",   0050170, 0x1234);
    TEST("ADDQ.W #8,($123456).L",   0050171, 0x0012, 0x3456);
    ERRT("ADDQ.W #8,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0050100);
    ERRT("ADDQ.W #8,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0050100);
    ERRT("ADDQ.W #8,#$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0050100);
    TEST("ADDQ.L #8,D2",            0050202);
    TEST("ADDQ.L #8,A2",            0050212);
    TEST("ADDQ.L #8,(A2)",          0050222);
    TEST("ADDQ.L #8,(A2)+",         0050232);
    TEST("ADDQ.L #8,-(A2)",         0050242);
    TEST("ADDQ.L #8,($1234,A2)",    0050252, 0x1234);
    TEST("ADDQ.L #8,($12,A2,D3.W)", 0050262, 0x3012);
    TEST("ADDQ.L #8,($001234).W",   0050270, 0x1234);
    TEST("ADDQ.L #8,($123454).L",   0050271, 0x0012, 0x3454);
    ERRT("ADDQ.L #8,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0050200);
    ERRT("ADDQ.L #8,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0050200);
    ERRT("ADDQ.L #8,#$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0050200);

    // ADDX Dx,Dy: 015|Dx|Sz|0|Dy, Sz:B=4/W=5/L=6
    TEST("ADDX.B D2,D3", 0153402);
    TEST("ADDX.W D2,D3", 0153502);
    TEST("ADDX.L D2,D3", 0153602);

    // ADDX -(Ax),-(Ay), 015|Ax|Sz|1|Ay, Sz:B=4/W=5/L=6
    TEST("ADDX.B -(A2),-(A3)", 0153412);
    TEST("ADDX.W -(A2),-(A3)", 0153512);
    TEST("ADDX.L -(A2),-(A3)", 0153612);

    // CLR dst: 0041|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("CLR.B D2",            0041002);
    ERRT("CLR.B A2",            OPERAND_NOT_ALLOWED, "A2", 0041000);
    TEST("CLR.B (A2)",          0041022);
    TEST("CLR.B (A2)+",         0041032);
    TEST("CLR.B -(A2)",         0041042);
    TEST("CLR.B ($1234,A2)",    0041052, 0x1234);
    TEST("CLR.B ($12,A2,D3.W)", 0041062, 0x3012);
    TEST("CLR.B ($001234).W",   0041070, 0x1234);
    TEST("CLR.B ($123456).L",   0041071, 0x0012, 0x3456);
    ERRT("CLR.B (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0041000);
    ERRT("CLR.B (*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0041000);
    ERRT("CLR.B #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0041000);
    TEST("CLR.W D2",            0041102);
    ERRT("CLR.W A2",            OPERAND_NOT_ALLOWED, "A2", 0041100);
    TEST("CLR.W (A2)",          0041122);
    TEST("CLR.W (A2)+",         0041132);
    TEST("CLR.W -(A2)",         0041142);
    TEST("CLR.W ($1234,A2)",    0041152, 0x1234);
    TEST("CLR.W ($12,A2,D3.W)", 0041162, 0x3012);
    TEST("CLR.W ($001234).W",   0041170, 0x1234);
    TEST("CLR.W ($123456).L",   0041171, 0x0012, 0x3456);
    ERRT("CLR.W (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0041100);
    ERRT("CLR.W (*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0041100);
    ERRT("CLR.W #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0041100);
    TEST("CLR.L D2",            0041202);
    ERRT("CLR.L A2",            OPERAND_NOT_ALLOWED, "A2", 0041200);
    TEST("CLR.L (A2)",          0041222);
    TEST("CLR.L (A2)+",         0041232);
    TEST("CLR.L -(A2)",         0041242);
    TEST("CLR.L ($1234,A2)",    0041252, 0x1234);
    TEST("CLR.L ($12,A2,D3.W)", 0041262, 0x3012);
    TEST("CLR.L ($001234).W",   0041270, 0x1234);
    TEST("CLR.L ($123454).L",   0041271, 0x0012, 0x3454);
    ERRT("CLR.L (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0041200);
    ERRT("CLR.L (*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0041200);
    ERRT("CLR.L #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0041200);

    // CMP src,Dn: 013|Dn|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("CMP.B D2,D7",              0137002);
    ERRT("CMP.B A2,D7",              OPERAND_NOT_ALLOWED, "A2,D7", 0137012);
    TEST("CMP.B (A2),D7",            0137022);
    TEST("CMP.B (A2)+,D7",           0137032);
    TEST("CMP.B -(A2),D7",           0137042);
    TEST("CMP.B ($1234,A2),D7",      0137052, 0x1234);
    TEST("CMP.B ($12,A2,D3.L),D7",   0137062, 0x3812);
    ABSW("CMP.B (@W).W,D7", "FFFF",  0137070, 0xFFFF);
    TEST("CMP.B ($123456).L,D7",     0137071, 0x0012, 0x3456);
    TEST("CMP.B (*+$1234,PC),D7",    0137072, 0x1232);
    TEST("CMP.B (*+$12,PC,D3.W),D7", 0137073, 0x3010);
    TEST("CMP.B #$34,D7",            0137074, 0x0034);
    TEST("CMP.W D2,D7",              0137102);
    TEST("CMP.W A2,D7",              0137112);
    TEST("CMP.W (A2),D7",            0137122);
    TEST("CMP.W (A2)+,D7",           0137132);
    TEST("CMP.W -(A2),D7",           0137142);
    TEST("CMP.W ($1234,A2),D7",      0137152, 0x1234);
    TEST("CMP.W ($12,A2,D3.L),D7",   0137162, 0x3812);
    ABSW("CMP.W (@W).W,D7", "FFFE",  0137170, 0xFFFE);
    TEST("CMP.W ($123456).L,D7",     0137171, 0x0012, 0x3456);
    TEST("CMP.W (*+$1234,PC),D7",    0137172, 0x1232);
    TEST("CMP.W (*+$12,PC,D3.W),D7", 0137173, 0x3010);
    TEST("CMP.W #$0034,D7",          0137174, 0x0034);
    TEST("CMP.L D2,D7",              0137202);
    TEST("CMP.L A2,D7",              0137212);
    TEST("CMP.L (A2),D7",            0137222);
    TEST("CMP.L (A2)+,D7",           0137232);
    TEST("CMP.L -(A2),D7",           0137242);
    TEST("CMP.L ($1234,A2),D7",      0137252, 0x1234);
    TEST("CMP.L ($12,A2,D3.L),D7",   0137262, 0x3812);
    ABSW("CMP.L (@W).W,D7", "FFFC",  0137270, 0xFFFC);
    TEST("CMP.L ($123454).L,D7",     0137271, 0x0012, 0x3454);
    TEST("CMP.L (*+$1234,PC),D7",    0137272, 0x1232);
    TEST("CMP.L (*+$12,PC,D3.W),D7", 0137273, 0x3010);
    TEST("CMP.L #$00345678,D7",      0137274, 0x0034, 0x5678);

    // CMPA src,An: 013|An|Sz|M|Rn, Sz:W=3/L=7
    TEST("CMPA.W D2,A6",              0136302);
    TEST("CMPA.W A2,A6",              0136312);
    TEST("CMPA.W (A2),A6",            0136322);
    TEST("CMPA.W (A2)+,A6",           0136332);
    TEST("CMPA.W -(A2),A6",           0136342);
    TEST("CMPA.W ($1234,A2),A6",      0136352, 0x1234);
    TEST("CMPA.W ($12,A2,D3.L),A6",   0136362, 0x3812);
    ABSW("CMPA.W (@W).W,A6", "FFFE",  0136370, 0xFFFE);
    TEST("CMPA.W ($123456).L,A6",     0136371, 0x0012, 0x3456);
    TEST("CMPA.W (*+$1234,PC),A6",    0136372, 0x1232);
    TEST("CMPA.W (*+$12,PC,D3.W),A6", 0136373, 0x3010);
    TEST("CMPA.W #$0034,A6",          0136374, 0x0034);
    TEST("CMPA.L D2,A6",              0136702);
    TEST("CMPA.L A2,A6",              0136712);
    TEST("CMPA.L (A2),A6",            0136722);
    TEST("CMPA.L (A2)+,A6",           0136732);
    TEST("CMPA.L -(A2),A6",           0136742);
    TEST("CMPA.L ($1234,A2),A6",      0136752, 0x1234);
    TEST("CMPA.L ($12,A2,D3.L),A6",   0136762, 0x3812);
    ABSW("CMPA.L (@W).W,A6", "FFFC",  0136770, 0xFFFC);
    TEST("CMPA.L ($123454).L,A6",     0136771, 0x0012, 0x3454);
    TEST("CMPA.L (*+$1234,PC),A6",    0136772, 0x1232);
    TEST("CMPA.L (*+$12,PC,D3.W),A6", 0136773, 0x3010);
    TEST("CMPA.L #$00345678,A6",      0136774, 0x0034, 0x5678);

    // CMPI #nn,dst: 0006|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("CMPI.B #$12,D2",            0006002, 0x0012);
    ERRT("CMPI.B #$12,A2",            OPERAND_NOT_ALLOWED, "A2", 0006000);
    TEST("CMPI.B #$12,(A2)",          0006022, 0x0012);
    TEST("CMPI.B #$12,(A2)+",         0006032, 0x0012);
    TEST("CMPI.B #$12,-(A2)",         0006042, 0x0012);
    TEST("CMPI.B #$12,($1234,A2)",    0006052, 0x0012, 0x1234);
    TEST("CMPI.B #$12,($12,A2,D3.W)", 0006062, 0x0012, 0x3012);
    TEST("CMPI.B #$12,($001234).W",   0006070, 0x0012, 0x1234);
    TEST("CMPI.B #$12,($123456).L",   0006071, 0x0012, 0x0012, 0x3456);
    ERRT("CMPI.B #$12,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0006000);
    ERRT("CMPI.B #$12,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0006000);
    ERRT("CMPI.B #$12,#$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0006000);
    TEST("CMPI.W #$5678,D2",               0006102, 0x5678);
    TEST("CMPI.W #$5678,A2",               0132374, 0x5678); // CMPA.W
    TEST("CMPI.W #$5678,(A2)",             0006122, 0x5678);
    TEST("CMPI.W #$5678,(A2)+",            0006132, 0x5678);
    TEST("CMPI.W #$5678,-(A2)",            0006142, 0x5678);
    TEST("CMPI.W #$5678,($1234,A2)",       0006152, 0x5678, 0x1234);
    TEST("CMPI.W #$5678,($12,A2,D3.W)",    0006162, 0x5678, 0x3012);
    TEST("CMPI.W #$5678,($001234).W",      0006170, 0x5678, 0x1234);
    TEST("CMPI.W #$5678,($123456).L",      0006171, 0x5678, 0x0012, 0x3456);
    ERRT("CMPI.L #$3456789A,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0006200);
    ERRT("CMPI.L #$3456789A,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0006200);
    ERRT("CMPI.L #$3456789A,#$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0006200);
    TEST("CMPI.L #$3456789A,D2",            0006202, 0x3456, 0x789A);
    TEST("CMPI.L #$3456789A,A2",            0132774, 0x3456, 0x789A); // CMPA.L
    TEST("CMPI.L #$3456789A,(A2)",          0006222, 0x3456, 0x789A);
    TEST("CMPI.L #$3456789A,(A2)+",         0006232, 0x3456, 0x789A);
    TEST("CMPI.L #$3456789A,-(A2)",         0006242, 0x3456, 0x789A);
    TEST("CMPI.L #$3456789A,($1234,A2)",    0006252, 0x3456, 0x789A, 0x1234);
    TEST("CMPI.L #$3456789A,($12,A2,D3.W)", 0006262, 0x3456, 0x789A, 0x3012);
    TEST("CMPI.L #$3456789A,($001234).W",   0006270, 0x3456, 0x789A, 0x1234);
    TEST("CMPI.L #$3456789A,($123454).L",   0006271, 0x3456, 0x789A, 0x0012, 0x3454);
    ERRT("CMPI.L #$3456789A,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0006200);
    ERRT("CMPI.L #$3456789A,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0006200);
    ERRT("CMPI.L #$3456789A,#$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0006200);

    // CMPM (Ay)+,(Ax)+: 013|Ax|Sz|1|Ay, Sz:B=4/W=5/L=6
    TEST("CMPM.B (A2)+,(A7)+", 0137412);
    TEST("CMPM.W (A2)+,(A7)+", 0137512);
    TEST("CMPM.L (A2)+,(A7)+", 0137612);
    ERRT("CMPM.B -(A2),-(A7)", OPERAND_NOT_ALLOWED, "-(A2),-(A7)");

    if (!firstGen()) {
        // CMP2 src,Rd, 00|Sz|3|M|Rn, Rd|00000, Sz:B=0/W=1/L=2
        ERRT("CMP2.B D2, D3", OPERAND_NOT_ALLOWED, "D2, D3", 0000300, 0030000);
        ERRT("CMP2.B A2, D3", OPERAND_NOT_ALLOWED, "A2, D3", 0000300, 0030000);
        TEST("CMP2.B (A2), D3",           0000322, 0030000);
        ERRT("CMP2.B (A2)+, D7", OPERAND_NOT_ALLOWED, "(A2)+, D7", 0000300, 0070000);
        ERRT("CMP2.B -(A2), A7", OPERAND_NOT_ALLOWED, "-(A2), A7", 0000300, 0170000);
        TEST("CMP2.B ($1234,A2), A6",     0000352, 0160000, 0x1234);
        TEST("CMP2.B (18,A2,D3.L), D7",   0000362, 0070000, 0x3812);
        ABSW("CMP2.B (@W).W, A0", "FFFE", 0000370, 0100000, 0xFFFE);
        TEST("CMP2.B ($123456).L, D0",    0000371, 0000000, 0x0012, 0x3456);
        TEST("CMP2.B (*+$1234,PC), A1",   0000372, 0110000, 0x1230);
        TEST("CMP2.B (*+18,PC,D3.W), D2", 0000373, 0020000, 0x300E);
        ERRT("CMP2.B #$34, D7", OPERAND_NOT_ALLOWED, "#$34, D7", 0000300, 0070000);
        ERRT("CMP2.W D2, D3", OPERAND_NOT_ALLOWED, "D2, D3", 0001300, 0030000);
        ERRT("CMP2.W A2, D3", OPERAND_NOT_ALLOWED, "A2, D3", 0001300, 0030000);
        TEST("CMP2.W (A2), D3",           0001322, 0030000);
        ERRT("CMP2.W (A2)+, D7", OPERAND_NOT_ALLOWED, "(A2)+, D7", 0001300, 0070000);
        ERRT("CMP2.W -(A2), A7", OPERAND_NOT_ALLOWED, "-(A2), A7", 0001300, 0170000);
        TEST("CMP2.W ($1234,A2), A6",     0001352, 0160000, 0x1234);
        TEST("CMP2.W (18,A2,D3.L), D7",   0001362, 0070000, 0x3812);
        ABSW("CMP2.W (@W).W, A0", "FFFE", 0001370, 0100000, 0xFFFE);
        TEST("CMP2.W ($123456).L, D0",    0001371, 0000000, 0x0012, 0x3456);
        TEST("CMP2.W (*+$1234,PC), A1",   0001372, 0110000, 0x1230);
        TEST("CMP2.W (*+18,PC,D3.W), D2", 0001373, 0020000, 0x300E);
        ERRT("CMP2.W #$1234, D7", OPERAND_NOT_ALLOWED, "#$1234, D7", 0001300, 0070000);
        ERRT("CMP2.L D2, D3", OPERAND_NOT_ALLOWED, "D2, D3", 0002300, 0030000);
        ERRT("CMP2.L A2, D3", OPERAND_NOT_ALLOWED, "A2, D3", 0002300, 0030000);
        TEST("CMP2.L (A2), D3",           0002322, 0030000);
        ERRT("CMP2.L (A2)+, D7", OPERAND_NOT_ALLOWED, "(A2)+, D7", 0002300, 0070000);
        ERRT("CMP2.L -(A2), A7", OPERAND_NOT_ALLOWED, "-(A2), A7", 0002300, 0170000);
        TEST("CMP2.L ($1234,A2), A6",     0002352, 0160000, 0x1234);
        TEST("CMP2.L (18,A2,D3.L), D7",   0002362, 0070000, 0x3812);
        ABSW("CMP2.L (@W).W, A0", "FFFE", 0002370, 0100000, 0xFFFE);
        TEST("CMP2.L ($123456).L, D0",    0002371, 0000000, 0x0012, 0x3456);
        TEST("CMP2.L (*+$1234,PC), A1",   0002372, 0110000, 0x1230);
        TEST("CMP2.L (*+18,PC,D3.W), D2", 0002373, 0020000, 0x300E);
        ERRT("CMP2.L #$12345678, D7", OPERAND_NOT_ALLOWED, "#$12345678, D7", 0002300, 0070000);
    }

    // DIVS src,Dn: 010|Dn|7|M|Rn
    TEST("DIVS.W D2,D7",              0107702);
    ERRT("DIVS.W A2,D7",              OPERAND_NOT_ALLOWED, "A2,D7", 0107700);
    TEST("DIVS.W (A2),D7",            0107722);
    TEST("DIVS.W (A2)+,D7",           0107732);
    TEST("DIVS.W -(A2),D7",           0107742);
    TEST("DIVS.W ($1234,A2),D7",      0107752, 0x1234);
    TEST("DIVS.W ($12,A2,D3.L),D7",   0107762, 0x3812);
    ABSW("DIVS.W (@W).W,D7", "FFFE",  0107770, 0xFFFE);
    TEST("DIVS.W ($123456).L,D7",     0107771, 0x0012, 0x3456);
    TEST("DIVS.W (*+$1234,PC),D7",    0107772, 0x1232);
    TEST("DIVS.W (*+$12,PC,D3.W),D7", 0107773, 0x3010);
    TEST("DIVS.W #$5678,D7",          0107774, 0x5678);

    if (!firstGen()) {
        // DIVS.L src,Dq: 00461|M|Rn, 0|Dq|400|Dq
        TEST("DIVS.L D2, D7",             0046102, 0074007);
        ERRT("DIVS.L A2, D7", OPERAND_NOT_ALLOWED, "A2, D7", 0046100, 0074007);
        TEST("DIVS.L (A2), D7",           0046122, 0074007);
        TEST("DIVS.L (A2)+, D7",          0046132, 0074007);
        TEST("DIVS.L -(A2), D7",          0046142, 0074007);
        TEST("DIVS.L ($1234,A2), D7",     0046152, 0074007, 0x1234);
        TEST("DIVS.L (18,A2,D3.L), D7",   0046162, 0074007, 0x3812);
        ABSW("DIVS.L (@W).W, D7", "FFFE", 0046170, 0074007, 0xFFFE);
        TEST("DIVS.L ($123456).L, D7",    0046171, 0074007, 0x0012, 0x3456);
        TEST("DIVS.L (*+$1234,PC), D7",   0046172, 0074007, 0x1230);
        TEST("DIVS.L (*+18,PC,D3.W), D7", 0046173, 0074007, 0x300E);
        TEST("DIVS.L #$12345678, D7",     0046174, 0074007, 0x1234, 0x5678);
        // DIVSL.L src,Dr:Dq: 00461|M|Rn, 0|Dq|400|Dr
        TEST("DIVSL.L D2, D6:D7",             0046102, 0074006);
        ERRT("DIVSL.L A2, D6:D7", OPERAND_NOT_ALLOWED, "A2, D6:D7", 0046100, 0074006);
        TEST("DIVSL.L (A2), D6:D7",           0046122, 0074006);
        TEST("DIVSL.L (A2)+, D6:D7",          0046132, 0074006);
        TEST("DIVSL.L -(A2), D6:D7",          0046142, 0074006);
        TEST("DIVSL.L ($1234,A2), D6:D7",     0046152, 0074006, 0x1234);
        TEST("DIVSL.L (18,A2,D3.L), D6:D7",   0046162, 0074006, 0x3812);
        ABSW("DIVSL.L (@W).W, D6:D7", "FFFE", 0046170, 0074006, 0xFFFE);
        TEST("DIVSL.L ($123456).L, D6:D7",    0046171, 0074006, 0x0012, 0x3456);
        TEST("DIVSL.L (*+$1234,PC), D6:D7",   0046172, 0074006, 0x1230);
        TEST("DIVSL.L (*+18,PC,D3.W), D6:D7", 0046173, 0074006, 0x300E);
        TEST("DIVSL.L #$12345678, D6:D7",     0046174, 0074006, 0x1234, 0x5678);
        // DIVS.L src,Dr:Dq: 00461|M|Rn, 0|Dq|600|Dr
        TEST("DIVS.L D2, D6:D7",             0046102, 0076006);
        ERRT("DIVS.L A2, D6:D7", OPERAND_NOT_ALLOWED, "A2, D6:D7", 0046100, 0076006);
        TEST("DIVS.L (A2), D6:D7",           0046122, 0076006);
        TEST("DIVS.L (A2)+, D6:D7",          0046132, 0076006);
        TEST("DIVS.L -(A2), D6:D7",          0046142, 0076006);
        TEST("DIVS.L ($1234,A2), D6:D7",     0046152, 0076006, 0x1234);
        TEST("DIVS.L (18,A2,D3.L), D6:D7",   0046162, 0076006, 0x3812);
        ABSW("DIVS.L (@W).W, D6:D7", "FFFE", 0046170, 0076006, 0xFFFE);
        TEST("DIVS.L ($123456).L, D6:D7",    0046171, 0076006, 0x0012, 0x3456);
        TEST("DIVS.L (*+$1234,PC), D6:D7",   0046172, 0076006, 0x1230);
        TEST("DIVS.L (*+18,PC,D3.W), D6:D7", 0046173, 0076006, 0x300E);
        TEST("DIVS.L #$12345678, D6:D7",     0046174, 0076006, 0x1234, 0x5678);
    }

    // DIVU src,Dn: 010|Dn|3|M|Rn
    TEST("DIVU.W D2,D7",              0107302);
    ERRT("DIVU.W A2,D7",              OPERAND_NOT_ALLOWED, "A2,D7", 0107300);
    TEST("DIVU.W (A2),D7",            0107322);
    TEST("DIVU.W (A2)+,D7",           0107332);
    TEST("DIVU.W -(A2),D7",           0107342);
    TEST("DIVU.W ($1234,A2),D7",      0107352, 0x1234);
    TEST("DIVU.W ($12,A2,D3.L),D7",   0107362, 0x3812);
    ABSW("DIVU.W (@W).W,D7", "FFFE",  0107370, 0xFFFE);
    TEST("DIVU.W ($123456).L,D7",     0107371, 0x0012, 0x3456);
    TEST("DIVU.W (*+$1234,PC),D7",    0107372, 0x1232);
    TEST("DIVU.W (*+$12,PC,D3.W),D7", 0107373, 0x3010);
    TEST("DIVU.W #$5678,D7",          0107374, 0x5678);

    if (!firstGen()) {
        // DIVU.L src,Dq: 00461|M|Rn, 0|Dq|000|Dq
        TEST("DIVU.L D2, D7",             0046102, 0070007);
        ERRT("DIVU.L A2, D7", OPERAND_NOT_ALLOWED, "A2, D7", 0046100, 0070007);
        TEST("DIVU.L (A2), D7",           0046122, 0070007);
        TEST("DIVU.L (A2)+, D7",          0046132, 0070007);
        TEST("DIVU.L -(A2), D7",          0046142, 0070007);
        TEST("DIVU.L ($1234,A2), D7",     0046152, 0070007, 0x1234);
        TEST("DIVU.L (18,A2,D3.L), D7",   0046162, 0070007, 0x3812);
        ABSW("DIVU.L (@W).W, D7", "FFFE", 0046170, 0070007, 0xFFFE);
        TEST("DIVU.L ($123456).L, D7",    0046171, 0070007, 0x0012, 0x3456);
        TEST("DIVU.L (*+$1234,PC), D7",   0046172, 0070007, 0x1230);
        TEST("DIVU.L (*+18,PC,D3.W), D7", 0046173, 0070007, 0x300E);
        TEST("DIVU.L #$12345678, D7",     0046174, 0070007, 0x1234, 0x5678);
        // DIVUL.L src,Dr:Dq: 00461|M|Rn, 0|Dq|000|Dr
        TEST("DIVUL.L D2, D6:D7",             0046102, 0070006);
        ERRT("DIVUL.L A2, D6:D7", OPERAND_NOT_ALLOWED, "A2, D6:D7", 0046100, 0070006);
        TEST("DIVUL.L (A2), D6:D7",           0046122, 0070006);
        TEST("DIVUL.L (A2)+, D6:D7",          0046132, 0070006);
        TEST("DIVUL.L -(A2), D6:D7",          0046142, 0070006);
        TEST("DIVUL.L ($1234,A2), D6:D7",     0046152, 0070006, 0x1234);
        TEST("DIVUL.L (18,A2,D3.L), D6:D7",   0046162, 0070006, 0x3812);
        ABSW("DIVUL.L (@W).W, D6:D7", "FFFE", 0046170, 0070006, 0xFFFE);
        TEST("DIVUL.L ($123456).L, D6:D7",    0046171, 0070006, 0x0012, 0x3456);
        TEST("DIVUL.L (*+$1234,PC), D6:D7",   0046172, 0070006, 0x1230);
        TEST("DIVUL.L (*+18,PC,D3.W), D6:D7", 0046173, 0070006, 0x300E);
        TEST("DIVUL.L #$12345678, D6:D7",     0046174, 0070006, 0x1234, 0x5678);
        // DIVU.L src,Dr:Dq: 00461|M|Rn, 0|Dq|200|Dr
        TEST("DIVU.L D2, D6:D7",             0046102, 0072006);
        ERRT("DIVU.L A2, D6:D7", OPERAND_NOT_ALLOWED, "A2, D6:D7", 0046100, 0072006);
        TEST("DIVU.L (A2), D6:D7",           0046122, 0072006);
        TEST("DIVU.L (A2)+, D6:D7",          0046132, 0072006);
        TEST("DIVU.L -(A2), D6:D7",          0046142, 0072006);
        TEST("DIVU.L ($1234,A2), D6:D7",     0046152, 0072006, 0x1234);
        TEST("DIVU.L (18,A2,D3.L), D6:D7",   0046162, 0072006, 0x3812);
        ABSW("DIVU.L (@W).W, D6:D7", "FFFE", 0046170, 0072006, 0xFFFE);
        TEST("DIVU.L ($123456).L, D6:D7",    0046171, 0072006, 0x0012, 0x3456);
        TEST("DIVU.L (*+$1234,PC), D6:D7",   0046172, 0072006, 0x1230);
        TEST("DIVU.L (*+18,PC,D3.W), D6:D7", 0046173, 0072006, 0x300E);
        TEST("DIVU.L #$12345678, D6:D7",     0046174, 0072006, 0x1234, 0x5678);
    }

    // EXT Dn: 0044|Sz|0|Dn, Sz:W=2/L=3
    TEST("EXT.W D2", 0044202);
    TEST("EXT.L D2", 0044302);
    ERRT("EXT.L A2", OPERAND_NOT_ALLOWED, "A2");

    // MULS src,Dn: 014|Dn|7|M|Rn
    TEST("MULS.W D2,D7",              0147702);
    ERRT("MULS.W A2,D7",              OPERAND_NOT_ALLOWED, "A2,D7", 0147700);
    TEST("MULS.W (A2),D7",            0147722);
    TEST("MULS.W (A2)+,D7",           0147732);
    TEST("MULS.W -(A2),D7",           0147742);
    TEST("MULS.W ($1234,A2),D7",      0147752, 0x1234);
    TEST("MULS.W ($12,A2,D3.L),D7",   0147762, 0x3812);
    ABSW("MULS.W (@W).W,D7", "FFFE",  0147770, 0xFFFE);
    TEST("MULS.W ($123456).L,D7",     0147771, 0x0012, 0x3456);
    TEST("MULS.W (*+$1234,PC),D7",    0147772, 0x1232);
    TEST("MULS.W (*+$12,PC,D3.W),D7", 0147773, 0x3010);
    TEST("MULS.W #$5678,D7",          0147774, 0x5678);

    if (!firstGen()) {
        // MULS.L src,Dl: 00460|M|Rn, 0|Dl|4000
        TEST("MULS.L D2, D7",               0046002, 074000);
        ERRT("MULS.L A2, D7", OPERAND_NOT_ALLOWED, "A2, D7", 0046000, 074000);
        TEST("MULS.L (A2), D6",             0046022, 064000);
        TEST("MULS.L (A2)+, D5",            0046032, 054000);
        TEST("MULS.L -(A2), D4",            0046042, 044000);
        TEST("MULS.L ($1234,A2), D7",       0046052, 074000, 0x1234);
        TEST("MULS.L (18,A2,D3.L), D7",     0046062, 074000, 0x3812);
        ABSW("MULS.L (@W).W, D7", "FFFE",   0046070, 074000, 0xFFFE);
        TEST("MULS.L ($123456).L, D7",      0046071, 074000, 0x0012, 0x3456);
        TEST("MULS.L (*+$1234,PC), D7",     0046072, 074000, 0x1230);
        TEST("MULS.L (*+18,PC,D3.W), D7",   0046073, 074000, 0x300E);
        TEST("MULS.L #$12345678, D7",       0046074, 074000, 0x1234, 0x5678);
        // MULS.L src,Dh:Dl: 00460|M|Rn, 0|Dl|600Dh
        TEST("MULS.L D2, D6:D7",               0046002, 076006);
        ERRT("MULS.L A2, D6:D7", OPERAND_NOT_ALLOWED, "A2, D6:D7", 0046000, 076006);
        TEST("MULS.L (A2), D6:D7",             0046022, 076006);
        TEST("MULS.L (A2)+, D6:D7",            0046032, 076006);
        TEST("MULS.L -(A2), D6:D7",            0046042, 076006);
        TEST("MULS.L ($1234,A2), D6:D7",       0046052, 076006, 0x1234);
        TEST("MULS.L (18,A2,D3.L), D6:D7",     0046062, 076006, 0x3812);
        ABSW("MULS.L (@W).W, D6:D7", "FFFE",   0046070, 076006, 0xFFFE);
        TEST("MULS.L ($123456).L, D6:D7",      0046071, 076006, 0x0012, 0x3456);
        TEST("MULS.L (*+$1234,PC), D6:D7",     0046072, 076006, 0x1230);
        TEST("MULS.L (*+18,PC,D3.W), D6:D7",   0046073, 076006, 0x300E);
        TEST("MULS.L #$12345678, D6:D7",       0046074, 076006, 0x1234, 0x5678);
    }

    // MULU src,Dn: 014|Dn|3|M|Rn
    TEST("MULU.W D2,D7",              0147302);
    ERRT("MULU.W A2,D7",              OPERAND_NOT_ALLOWED, "A2,D7", 0147300);
    TEST("MULU.W (A2),D7",            0147322);
    TEST("MULU.W (A2)+,D7",           0147332);
    TEST("MULU.W -(A2),D7",           0147342);
    TEST("MULU.W ($1234,A2),D7",      0147352, 0x1234);
    TEST("MULU.W ($12,A2,D3.L),D7",   0147362, 0x3812);
    ABSW("MULU.W (@W).W,D7", "FFFE",  0147370, 0xFFFE);
    TEST("MULU.W ($123456).L,D7",     0147371, 0x0012, 0x3456);
    TEST("MULU.W (*+$1234,PC),D7",    0147372, 0x1232);
    TEST("MULU.W (*+$12,PC,D3.W),D7", 0147373, 0x3010);
    TEST("MULU.W #$5678,D7",          0147374, 0x5678);

    if (!firstGen()) {
        // MULU.L src,Dl: 00460|M|Rn, 0|Dl|0000
        TEST("MULU.L D2, D7",               0046002, 070000);
        ERRT("MULU.L A2, D7", OPERAND_NOT_ALLOWED, "A2, D7", 0046000, 070000);
        TEST("MULU.L (A2), D6",             0046022, 060000);
        TEST("MULU.L (A2)+, D5",            0046032, 050000);
        TEST("MULU.L -(A2), D4",            0046042, 040000);
        TEST("MULU.L ($1234,A2), D7",       0046052, 070000, 0x1234);
        TEST("MULU.L (18,A2,D3.L), D7",     0046062, 070000, 0x3812);
        ABSW("MULU.L (@W).W, D7", "FFFE",   0046070, 070000, 0xFFFE);
        TEST("MULU.L ($123456).L, D7",      0046071, 070000, 0x0012, 0x3456);
        TEST("MULU.L (*+$1234,PC), D7",     0046072, 070000, 0x1230);
        TEST("MULU.L (*+18,PC,D3.W), D7",   0046073, 070000, 0x300E);
        TEST("MULU.L #$12345678, D7",       0046074, 070000, 0x1234, 0x5678);
        // MULU.L src,Dh:Dl: 00460|M|Rn, 0|Dl|200Dh
        TEST("MULU.L D2, D6:D7",               0046002, 072006);
        ERRT("MULU.L A2, D6:D7", OPERAND_NOT_ALLOWED, "A2, D6:D7", 0046000, 072006);
        TEST("MULU.L (A2), D6:D7",             0046022, 072006);
        TEST("MULU.L (A2)+, D6:D7",            0046032, 072006);
        TEST("MULU.L -(A2), D6:D7",            0046042, 072006);
        TEST("MULU.L ($1234,A2), D6:D7",       0046052, 072006, 0x1234);
        TEST("MULU.L (18,A2,D3.L), D6:D7",     0046062, 072006, 0x3812);
        ABSW("MULU.L (@W).W, D6:D7", "FFFE",   0046070, 072006, 0xFFFE);
        TEST("MULU.L ($123456).L, D6:D7",      0046071, 072006, 0x0012, 0x3456);
        TEST("MULU.L (*+$1234,PC), D6:D7",     0046072, 072006, 0x1230);
        TEST("MULU.L (*+18,PC,D3.W), D6:D7",   0046073, 072006, 0x300E);
        TEST("MULU.L #$12345678, D6:D7",       0046074, 072006, 0x1234, 0x5678);
    }

    // NEG dst: 0042|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("NEG.B D2",            0042002);
    ERRT("NEG.B A2",            OPERAND_NOT_ALLOWED, "A2", 0042000);
    TEST("NEG.B (A2)",          0042022);
    TEST("NEG.B (A2)+",         0042032);
    TEST("NEG.B -(A2)",         0042042);
    TEST("NEG.B ($1234,A2)",    0042052, 0x1234);
    TEST("NEG.B ($12,A2,D3.W)", 0042062, 0x3012);
    TEST("NEG.B ($001234).W",   0042070, 0x1234);
    TEST("NEG.B ($123456).L",   0042071, 0x0012, 0x3456);
    ERRT("NEG.B (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0042000);
    ERRT("NEG.B (*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0042000);
    ERRT("NEG.B #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0042000);
    TEST("NEG.W D2",            0042102);
    ERRT("NEG.W A2",            OPERAND_NOT_ALLOWED, "A2", 0042100);
    TEST("NEG.W (A2)",          0042122);
    TEST("NEG.W (A2)+",         0042132);
    TEST("NEG.W -(A2)",         0042142);
    TEST("NEG.W ($1234,A2)",    0042152, 0x1234);
    TEST("NEG.W ($12,A2,D3.W)", 0042162, 0x3012);
    TEST("NEG.W ($001234).W",   0042170, 0x1234);
    TEST("NEG.W ($123456).L",   0042171, 0x0012, 0x3456);
    ERRT("NEG.W (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0042100);
    ERRT("NEG.W (*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0042100);
    ERRT("NEG.W #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0042100);
    TEST("NEG.L D2",            0042202);
    ERRT("NEG.L A2",            OPERAND_NOT_ALLOWED, "A2", 0042200);
    TEST("NEG.L (A2)",          0042222);
    TEST("NEG.L (A2)+",         0042232);
    TEST("NEG.L -(A2)",         0042242);
    TEST("NEG.L ($1234,A2)",    0042252, 0x1234);
    TEST("NEG.L ($12,A2,D3.W)", 0042262, 0x3012);
    TEST("NEG.L ($001234).W",   0042270, 0x1234);
    TEST("NEG.L ($123454).L",   0042271, 0x0012, 0x3454);
    ERRT("NEG.L (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0042200);
    ERRT("NEG.L (*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0042200);
    ERRT("NEG.L #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0042200);

    // NEGX dst: 0040|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("NEGX.B D2",            0040002);
    ERRT("NEGX.B A2",            OPERAND_NOT_ALLOWED, "A2", 0040000);
    TEST("NEGX.B (A2)",          0040022);
    TEST("NEGX.B (A2)+",         0040032);
    TEST("NEGX.B -(A2)",         0040042);
    TEST("NEGX.B ($1234,A2)",    0040052, 0x1234);
    TEST("NEGX.B ($12,A2,D3.W)", 0040062, 0x3012);
    TEST("NEGX.B ($001234).W",   0040070, 0x1234);
    TEST("NEGX.B ($123456).L",   0040071, 0x0012, 0x3456);
    ERRT("NEGX.B (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0040000);
    ERRT("NEGX.B (*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0040000);
    ERRT("NEGX.B #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0040000);
    TEST("NEGX.W D2",            0040102);
    ERRT("NEGX.W A2",            OPERAND_NOT_ALLOWED, "A2", 0040100);
    TEST("NEGX.W (A2)",          0040122);
    TEST("NEGX.W (A2)+",         0040132);
    TEST("NEGX.W -(A2)",         0040142);
    TEST("NEGX.W ($1234,A2)",    0040152, 0x1234);
    TEST("NEGX.W ($12,A2,D3.W)", 0040162, 0x3012);
    TEST("NEGX.W ($001234).W",   0040170, 0x1234);
    TEST("NEGX.W ($123456).L",   0040171, 0x0012, 0x3456);
    ERRT("NEGX.W (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0040100);
    ERRT("NEGX.W (*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0040100);
    ERRT("NEGX.W #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0040100);
    TEST("NEGX.L D2",            0040202);
    ERRT("NEGX.L A2",            OPERAND_NOT_ALLOWED, "A2", 0040200);
    TEST("NEGX.L (A2)",          0040222);
    TEST("NEGX.L (A2)+",         0040232);
    TEST("NEGX.L -(A2)",         0040242);
    TEST("NEGX.L ($1234,A2)",    0040252, 0x1234);
    TEST("NEGX.L ($12,A2,D3.W)", 0040262, 0x3012);
    TEST("NEGX.L ($001234).W",   0040270, 0x1234);
    TEST("NEGX.L ($123454).L",   0040271, 0x0012, 0x3454);
    ERRT("NEGX.L (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0040200);
    ERRT("NEGX.L (*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0040200);
    ERRT("NEGX.L #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0040200);

    // SUB src,Dn: 011|Dn|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("SUB.B D2,D7",              0117002);
    ERRT("SUB.B A2,D7",              OPERAND_NOT_ALLOWED, "A2,D7", 0117012);
    TEST("SUB.B (A2),D7",            0117022);
    TEST("SUB.B (A2)+,D7",           0117032);
    TEST("SUB.B -(A2),D7",           0117042);
    TEST("SUB.B ($1234,A2),D7",      0117052, 0x1234);
    TEST("SUB.B ($12,A2,D3.L),D7",   0117062, 0x3812);
    ABSW("SUB.B (@W).W,D7", "FFFF",  0117070, 0xFFFF);
    TEST("SUB.B ($123456).L,D7",     0117071, 0x0012, 0x3456);
    TEST("SUB.B (*+$1234,PC),D7",    0117072, 0x1232);
    TEST("SUB.B (*+$12,PC,D3.W),D7", 0117073, 0x3010);
    TEST("SUB.B #$34,D7",            0117074, 0x0034);
    TEST("SUB.W D2,D7",              0117102);
    TEST("SUB.W A2,D7",              0117112);
    TEST("SUB.W (A2),D7",            0117122);
    TEST("SUB.W (A2)+,D7",           0117132);
    TEST("SUB.W -(A2),D7",           0117142);
    TEST("SUB.W ($1234,A2),D7",      0117152, 0x1234);
    TEST("SUB.W ($12,A2,D3.L),D7",   0117162, 0x3812);
    ABSW("SUB.W (@W).W,D7", "FFFE",  0117170, 0xFFFE);
    TEST("SUB.W ($123456).L,D7",     0117171, 0x0012, 0x3456);
    TEST("SUB.W (*+$1234,PC),D7",    0117172, 0x1232);
    TEST("SUB.W (*+$12,PC,D3.W),D7", 0117173, 0x3010);
    TEST("SUB.W #$0034,D7",          0117174, 0x0034);
    TEST("SUB.L D2,D7",              0117202);
    TEST("SUB.L A2,D7",              0117212);
    TEST("SUB.L (A2),D7",            0117222);
    TEST("SUB.L (A2)+,D7",           0117232);
    TEST("SUB.L -(A2),D7",           0117242);
    TEST("SUB.L ($1234,A2),D7",      0117252, 0x1234);
    TEST("SUB.L ($12,A2,D3.L),D7",   0117262, 0x3812);
    ABSW("SUB.L (@W).W,D7", "FFFC",  0117270, 0xFFFC);
    TEST("SUB.L ($123454).L,D7",     0117271, 0x0012, 0x3454);
    TEST("SUB.L (*+$1234,PC),D7",    0117272, 0x1232);
    TEST("SUB.L (*+$12,PC,D3.W),D7", 0117273, 0x3010);
    TEST("SUB.L #$00345678,D7",      0117274, 0x0034, 0x5678);

    // SUB Dn,dst: 011|Dn|Sz|M|Rn, Sz:B=4/W=5/L=6
    TEST("SUB.B D7,D2",            0112007);
    ERRT("SUB.B D7,A2",            OPERAND_NOT_ALLOWED, "A2", 0117400);
    TEST("SUB.B D7,(A2)",          0117422);
    TEST("SUB.B D7,(A2)+",         0117432);
    TEST("SUB.B D7,-(A2)",         0117442);
    TEST("SUB.B D7,($1234,A2)",    0117452, 0x1234);
    TEST("SUB.B D7,($12,A2,D3.L)", 0117462, 0x3812);
    ABSW("SUB.B D7,(@W).W", "FFFF", 0117470, 0xFFFF);
    TEST("SUB.B D7,($123456).L",   0117471, 0x0012, 0x3456);
    ERRT("SUB.B D7,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0117400);
    ERRT("SUB.B D7,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0117400);
    ERRT("SUB.B D7,#$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0117400);
    TEST("SUB.W D7,D2",            0112107);
    TEST("SUB.W D7,A2",            0112307); // SUBA.W
    TEST("SUB.W D7,(A2)",          0117522);
    TEST("SUB.W D7,(A2)+",         0117532);
    TEST("SUB.W D7,-(A2)",         0117542);
    TEST("SUB.W D7,($1234,A2)",    0117552, 0x1234);
    TEST("SUB.W D7,($12,A2,D3.L)", 0117562, 0x3812);
    ABSW("SUB.W D7,(@W).W", "FFFE", 0117570, 0xFFFE);
    TEST("SUB.W D7,($123456).L",   0117571, 0x0012, 0x3456);
    ERRT("SUB.W (*+$1234,PC)",     OPERAND_NOT_ALLOWED, "(*+$1234,PC)");
    ERRT("SUB.W (*+$12,PC,D3)",    OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)");
    ERRT("SUB.W #$1234",           OPERAND_NOT_ALLOWED, "#$1234");
    TEST("SUB.L D7,D2",            0112207);
    TEST("SUB.L D7,A2",            0112707); // SUBA.L
    TEST("SUB.L D7,(A2)",          0117622);
    TEST("SUB.L D7,(A2)+",         0117632);
    TEST("SUB.L D7,-(A2)",         0117642);
    TEST("SUB.L D7,($1234,A2)",    0117652, 0x1234);
    TEST("SUB.L D7,($12,A2,D3.L)", 0117662, 0x3812);
    ABSW("SUB.L D7,(@W).W", "FFFC", 0117670, 0xFFFC);
    TEST("SUB.L D7,($123454).L",   0117671, 0x0012, 0x3454);
    ERRT("SUB.L D7,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0117600);
    ERRT("SUB.L D7,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0117600);
    ERRT("SUB.L D7,#$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0117600);

    // SUBA src,An: 011|An|Sz|M|Rn, Sz:W=3/L=7
    TEST("SUBA.W D2,A6",              0116302);
    TEST("SUBA.W A2,A6",              0116312);
    TEST("SUBA.W (A2),A6",            0116322);
    TEST("SUBA.W (A2)+,A6",           0116332);
    TEST("SUBA.W -(A2),A6",           0116342);
    TEST("SUBA.W ($1234,A2),A6",      0116352, 0x1234);
    TEST("SUBA.W ($12,A2,D3.L),A6",   0116362, 0x3812);
    ABSW("SUBA.W (@W).W,A6", "FFFE",  0116370, 0xFFFE);
    TEST("SUBA.W ($123456).L,A6",     0116371, 0x0012, 0x3456);
    TEST("SUBA.W (*+$1234,PC),A6",    0116372, 0x1232);
    TEST("SUBA.W (*+$12,PC,D3.W),A6", 0116373, 0x3010);
    TEST("SUBA.W #$0034,A6",          0116374, 0x0034);
    TEST("SUBA.L D2,A6",              0116702);
    TEST("SUBA.L A2,A6",              0116712);
    TEST("SUBA.L (A2),A6",            0116722);
    TEST("SUBA.L (A2)+,A6",           0116732);
    TEST("SUBA.L -(A2),A6",           0116742);
    TEST("SUBA.L ($1234,A2),A6",      0116752, 0x1234);
    TEST("SUBA.L ($12,A2,D3.L),A6",   0116762, 0x3812);
    ABSW("SUBA.L (@W).W,A6", "FFFC",  0116770, 0xFFFC);
    TEST("SUBA.L ($123454).L,A6",     0116771, 0x0012, 0x3454);
    TEST("SUBA.L (*+$1234,PC),A6",    0116772, 0x1232);
    TEST("SUBA.L (*+$12,PC,D3.W),A6", 0116773, 0x3010);
    TEST("SUBA.L #$00345678,A6",      0116774, 0x0034, 0x5678);

    // SUBI #nn,dst: 0002|Sz|M|Rn, SZ:B=0/W=1/L=2
    TEST("SUBI.B #$12,D2",            0002002, 0x0012);
    ERRT("SUBI.B #$12,A2",            OPERAND_NOT_ALLOWED, "A2", 0002000);
    TEST("SUBI.B #$12,(A2)",          0002022, 0x0012);
    TEST("SUBI.B #$12,(A2)+",         0002032, 0x0012);
    TEST("SUBI.B #$12,-(A2)",         0002042, 0x0012);
    TEST("SUBI.B #$12,($1234,A2)",    0002052, 0x0012, 0x1234);
    TEST("SUBI.B #$12,($12,A2,D3.W)", 0002062, 0x0012, 0x3012);
    TEST("SUBI.B #$12,($001234).W",   0002070, 0x0012, 0x1234);
    TEST("SUBI.B #$12,($123456).L",   0002071, 0x0012, 0x0012, 0x3456);
    ERRT("SUBI.B #$12,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0002000);
    ERRT("SUBI.B #$12,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0002000);
    ERRT("SUBI.B #$12,#$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0002000);
    TEST("SUBI.W #$5678,D2",            0002102, 0x5678);
    TEST("SUBI.W #$5678,A2",            0112374, 0x5678); // SUBA.W
    TEST("SUBI.W #$5678,(A2)",          0002122, 0x5678);
    TEST("SUBI.W #$5678,(A2)+",         0002132, 0x5678);
    TEST("SUBI.W #$5678,-(A2)",         0002142, 0x5678);
    TEST("SUBI.W #$5678,($1234,A2)",    0002152, 0x5678, 0x1234);
    TEST("SUBI.W #$5678,($12,A2,D3.W)", 0002162, 0x5678, 0x3012);
    TEST("SUBI.W #$5678,($001234).W",   0002170, 0x5678, 0x1234);
    TEST("SUBI.W #$5678,($123456).L",   0002171, 0x5678, 0x0012, 0x3456);
    ERRT("SUBI.W #$5678,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0002100);
    ERRT("SUBI.W #$5678,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0002100);
    ERRT("SUBI.W #$5678,#$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0002100);
    TEST("SUBI.L #$3456789A,D2",            0002202, 0x3456, 0x789A);
    TEST("SUBI.L #$3456789A,A2",            0112774, 0x3456, 0x789A); // SUBA.L
    TEST("SUBI.L #$3456789A,(A2)",          0002222, 0x3456, 0x789A);
    TEST("SUBI.L #$3456789A,(A2)+",         0002232, 0x3456, 0x789A);
    TEST("SUBI.L #$3456789A,-(A2)",         0002242, 0x3456, 0x789A);
    TEST("SUBI.L #$3456789A,($1234,A2)",    0002252, 0x3456, 0x789A, 0x1234);
    TEST("SUBI.L #$3456789A,($12,A2,D3.W)", 0002262, 0x3456, 0x789A, 0x3012);
    TEST("SUBI.L #$3456789A,($001234).W",   0002270, 0x3456, 0x789A, 0x1234);
    TEST("SUBI.L #$3456789A,($123454).L",   0002271, 0x3456, 0x789A, 0x0012, 0x3454);
    ERRT("SUBI.L #$3456789A,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0002200);
    ERRT("SUBI.L #$3456789A,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0002200);
    ERRT("SUBI.L #$3456789A,#$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0002200);

    // SUBQ #nn,dst: 005|nn|Sz|M\En, Sz:B=4/W=5/L=6
    TEST("SUBQ.B #8,D2",            0050402);
    ERRT("SUBQ.B #8,A2",            OPERAND_NOT_ALLOWED, "A2", 0050412);
    TEST("SUBQ.B #8,(A2)",          0050422);
    TEST("SUBQ.B #8,(A2)+",         0050432);
    TEST("SUBQ.B #8,-(A2)",         0050442);
    TEST("SUBQ.B #8,($1234,A2)",    0050452, 0x1234);
    TEST("SUBQ.B #8,($12,A2,D3.W)", 0050462, 0x3012);
    TEST("SUBQ.B #8,($001234).W",   0050470, 0x1234);
    TEST("SUBQ.B #8,($123456).L",   0050471, 0x0012, 0x3456);
    ERRT("SUBQ.B #8,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0050400);
    ERRT("SUBQ.B #8,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0050400);
    ERRT("SUBQ.B #8,#$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0050400);
    TEST("SUBQ.W #8,D2",            0050502);
    TEST("SUBQ.W #8,A2",            0050512);
    TEST("SUBQ.W #8,(A2)",          0050522);
    TEST("SUBQ.W #8,(A2)+",         0050532);
    TEST("SUBQ.W #8,-(A2)",         0050542);
    TEST("SUBQ.W #8,($1234,A2)",    0050552, 0x1234);
    TEST("SUBQ.W #8,($12,A2,D3.W)", 0050562, 0x3012);
    TEST("SUBQ.W #8,($001234).W",   0050570, 0x1234);
    TEST("SUBQ.W #8,($123456).L",   0050571, 0x0012, 0x3456);
    ERRT("SUBQ.W #8,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0050500);
    ERRT("SUBQ.W #8,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0050500);
    ERRT("SUBQ.W #8,#$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0050500);
    TEST("SUBQ.L #8,D2",            0050602);
    TEST("SUBQ.L #8,A2",            0050612);
    TEST("SUBQ.L #8,(A2)",          0050622);
    TEST("SUBQ.L #8,(A2)+",         0050632);
    TEST("SUBQ.L #8,-(A2)",         0050642);
    TEST("SUBQ.L #8,($1234,A2)",    0050652, 0x1234);
    TEST("SUBQ.L #8,($12,A2,D3.W)", 0050662, 0x3012);
    TEST("SUBQ.L #8,($001234).W",   0050670, 0x1234);
    TEST("SUBQ.L #8,($123454).L",   0050671, 0x0012, 0x3454);
    ERRT("SUBQ.L #8,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0050600);
    ERRT("SUBQ.L #8,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0050600);
    ERRT("SUBQ.L #8,#$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0050600);

    // SUBX Dx,Dy: 011|Dy|Sz|0|Dx, Sz:B=4/W=5/L=6
    TEST("SUBX.B D2,D3", 0113402);
    TEST("SUBX.W D2,D3", 0113502);
    TEST("SUBX.L D2,D3", 0113602);

    // SUBX -(Ax),-(Ay): 011|Ay|Sz|1|Ax, Sz:B=4/W=5/L=6
    TEST("SUBX.B -(A2),-(A3)", 0113412);
    TEST("SUBX.W -(A2),-(A3)", 0113512);
    TEST("SUBX.L -(A2),-(A3)", 0113612);
}

void test_logical() {
    // AND src,Dn: 014|Dn|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("AND.B D2,D7",              0147002);
    ERRT("AND.B A2",                 OPERAND_NOT_ALLOWED, "A2");
    TEST("AND.B (A2),D7",            0147022);
    TEST("AND.B (A2)+,D7",           0147032);
    TEST("AND.B -(A2),D7",           0147042);
    TEST("AND.B ($1234,A2),D7",      0147052, 0x1234);
    TEST("AND.B ($12,A2,D3.L),D7",   0147062, 0x3812);
    ABSW("AND.B (@W).W,D7", "FFFF",  0147070, 0xFFFF);
    TEST("AND.B ($123456).L,D7",     0147071, 0x0012, 0x3456);
    TEST("AND.B (*+$1234,PC),D7",    0147072, 0x1232);
    TEST("AND.B (*+$12,PC,D3.W),D7", 0147073, 0x3010);
    TEST("AND.B #$34,D7",            0147074, 0x0034);
    TEST("AND.W D2,D7",              0147102);
    ERRT("AND.W A2,D7",              OPERAND_NOT_ALLOWED, "A2,D7", 0147100);
    TEST("AND.W (A2),D7",            0147122);
    TEST("AND.W (A2)+,D7",           0147132);
    TEST("AND.W -(A2),D7",           0147142);
    TEST("AND.W ($1234,A2),D7",      0147152, 0x1234);
    TEST("AND.W ($12,A2,D3.L),D7",   0147162, 0x3812);
    ABSW("AND.W (@W).W,D7", "FFFE",  0147170, 0xFFFE);
    TEST("AND.W ($123456).L,D7",     0147171, 0x0012, 0x3456);
    TEST("AND.W (*+$1234,PC),D7",    0147172, 0x1232);
    TEST("AND.W (*+$12,PC,D3.W),D7", 0147173, 0x3010);
    TEST("AND.W #$0034,D7",          0147174, 0x0034);
    TEST("AND.L D2,D7",              0147202);
    ERRT("AND.L A2,D7",              OPERAND_NOT_ALLOWED, "A2,D7", 0147200);
    TEST("AND.L (A2),D7",            0147222);
    TEST("AND.L (A2)+,D7",           0147232);
    TEST("AND.L -(A2),D7",           0147242);
    TEST("AND.L ($1234,A2),D7",      0147252, 0x1234);
    TEST("AND.L ($12,A2,D3.L),D7",   0147262, 0x3812);
    ABSW("AND.L (@W).W,D7", "FFFC",  0147270, 0xFFFC);
    TEST("AND.L ($123454).L,D7",     0147271, 0x0012, 0x3454);
    TEST("AND.L (*+$1234,PC),D7",    0147272, 0x1232);
    TEST("AND.L (*+$12,PC,D3.W),D7", 0147273, 0x3010);
    TEST("AND.L #$00345678,D7",      0147274, 0x0034, 0x5678);

    // AND Dn.dst: 014|Dn|Sz|M|Rn, Sz:B=4/W=5/L=6
    TEST("AND.B D7,D2",            0142007);
    ERRT("AND.B D7,A2",            OPERAND_NOT_ALLOWED, "A2", 0147400);
    TEST("AND.B D7,(A2)",          0147422);
    TEST("AND.B D7,(A2)+",         0147432);
    TEST("AND.B D7,-(A2)",         0147442);
    TEST("AND.B D7,($1234,A2)",    0147452, 0x1234);
    TEST("AND.B D7,($12,A2,D3.L)", 0147462, 0x3812);
    ABSW("AND.B D7,(@W).W", "FFFF", 0147470, 0xFFFF);
    TEST("AND.B D7,($123456).L",   0147471, 0x0012, 0x3456);
    ERRT("AND.B D7,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0147400);
    ERRT("AND.B D7,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0147400);
    ERRT("AND.B D7,#$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0147400);
    TEST("AND.W D7,D2",            0142107);
    ERRT("AND.W D7,A7",            OPERAND_NOT_ALLOWED, "A7", 0147500);
    TEST("AND.W D7,(A2)",          0147522);
    TEST("AND.W D7,(A2)+",         0147532);
    TEST("AND.W D7,-(A2)",         0147542);
    TEST("AND.W D7,($1234,A2)",    0147552, 0x1234);
    TEST("AND.W D7,($12,A2,D3.L)", 0147562, 0x3812);
    ABSW("AND.W D7,(@W).W", "FFFE", 0147570, 0xFFFE);
    TEST("AND.W D7,($123456).L",   0147571, 0x0012, 0x3456);
    ERRT("AND.W D7,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0147500);
    ERRT("AND.W D7,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0147500);
    ERRT("AND.W D7,#$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0147500);
    TEST("AND.L D7,D2",            0142207);
    ERRT("AND.L D7,A2",            OPERAND_NOT_ALLOWED, "A2", 0147600);
    TEST("AND.L D7,(A2)",          0147622);
    TEST("AND.L D7,(A2)+",         0147632);
    TEST("AND.L D7,-(A2)",         0147642);
    TEST("AND.L D7,($1234,A2)",    0147652, 0x1234);
    TEST("AND.L D7,($12,A2,D3.L)", 0147662, 0x3812);
    ABSW("AND.L D7,(@W).W", "FFFC", 0147670, 0xFFFC);
    TEST("AND.L D7,($123454).L",   0147671, 0x0012, 0x3454);
    ERRT("AND.L D7,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0147600);
    ERRT("AND.L D7,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0147600);
    ERRT("AND.L D7,#$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0147600);

    // ANDI #nn,dst: 0001|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("ANDI.B #$12,D2",            0001002, 0x0012);
    ERRT("ANDI.B #$12,A2",            OPERAND_NOT_ALLOWED, "A2", 0001000);
    TEST("ANDI.B #$12,(A2)",          0001022, 0x0012);
    TEST("ANDI.B #$12,(A2)+",         0001032, 0x0012);
    TEST("ANDI.B #$12,-(A2)",         0001042, 0x0012);
    TEST("ANDI.B #$12,($1234,A2)",    0001052, 0x0012, 0x1234);
    TEST("ANDI.B #$12,($12,A2,D3.W)", 0001062, 0x0012, 0x3012);
    TEST("ANDI.B #$12,($001234).W",   0001070, 0x0012, 0x1234);
    TEST("ANDI.B #$12,($123456).L",   0001071, 0x0012, 0x0012, 0x3456);
    ERRT("ANDI.B #$12,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0001000);
    ERRT("ANDI.B #$12,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0001000);
    ERRT("ANDI.B #$12,#$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0001000);
    TEST("ANDI.W #$5678,D2",            0001102, 0x5678);
    ERRT("ANDI.W #$5678,A2",            OPERAND_NOT_ALLOWED, "A2", 0001100);
    TEST("ANDI.W #$5678,(A2)",          0001122, 0x5678);
    TEST("ANDI.W #$5678,(A2)+",         0001132, 0x5678);
    TEST("ANDI.W #$5678,-(A2)",         0001142, 0x5678);
    TEST("ANDI.W #$5678,($1234,A2)",    0001152, 0x5678, 0x1234);
    TEST("ANDI.W #$5678,($12,A2,D3.W)", 0001162, 0x5678, 0x3012);
    TEST("ANDI.W #$5678,($001234).W",   0001170, 0x5678, 0x1234);
    TEST("ANDI.W #$5678,($123456).L",   0001171, 0x5678, 0x0012, 0x3456);
    ERRT("ANDI.W #$5678,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0001100);
    ERRT("ANDI.W #$5678,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0001100);
    ERRT("ANDI.W #$5678,#$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0001100);
    TEST("ANDI.L #$3456789A,D2",            0001202, 0x3456, 0x789A);
    ERRT("ANDI.L #$3456789A,A2",            OPERAND_NOT_ALLOWED, "A2", 0001200);
    TEST("ANDI.L #$3456789A,(A2)",          0001222, 0x3456, 0x789A);
    TEST("ANDI.L #$3456789A,(A2)+",         0001232, 0x3456, 0x789A);
    TEST("ANDI.L #$3456789A,-(A2)",         0001242, 0x3456, 0x789A);
    TEST("ANDI.L #$3456789A,($1234,A2)",    0001252, 0x3456, 0x789A, 0x1234);
    TEST("ANDI.L #$3456789A,($12,A2,D3.W)", 0001262, 0x3456, 0x789A, 0x3012);
    TEST("ANDI.L #$3456789A,($001234).W",   0001270, 0x3456, 0x789A, 0x1234);
    TEST("ANDI.L #$34567898,($123454).L",   0001271, 0x3456, 0x7898, 0x0012, 0x3454);
    ERRT("ANDI.L #$3456789A,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0001200);
    ERRT("ANDI.L #$3456789A,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0001200);
    ERRT("ANDI.L #$3456789A,#$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0001200);

    // EOR Dn,dst: 013|Dn|Sz|M|Rn, Sz:B=4/W=5/L=6
    TEST("EOR.B D7,D2",            0137402);
    ERRT("EOR.B D7,A2",            OPERAND_NOT_ALLOWED, "A2", 0137400);
    TEST("EOR.B D7,(A2)",          0137422);
    TEST("EOR.B D7,(A2)+",         0137432);
    TEST("EOR.B D7,-(A2)",         0137442);
    TEST("EOR.B D7,($1234,A2)",    0137452, 0x1234);
    TEST("EOR.B D7,($12,A2,D3.L)", 0137462, 0x3812);
    ABSW("EOR.B D7,(@W).W", "FFFF", 0137470, 0xFFFF);
    TEST("EOR.B D7,($123456).L",   0137471, 0x0012, 0x3456);
    ERRT("EOR.B D7,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0137400);
    ERRT("EOR.B D7,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0137400);
    ERRT("EOR.B D7,#$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0137400);
    TEST("EOR.W D7,D2",            0137502);
    ERRT("EOR.W D7,A2",            OPERAND_NOT_ALLOWED, "A2", 0137500);
    TEST("EOR.W D7,(A2)",          0137522);
    TEST("EOR.W D7,(A2)+",         0137532);
    TEST("EOR.W D7,-(A2)",         0137542);
    TEST("EOR.W D7,($1234,A2)",    0137552, 0x1234);
    TEST("EOR.W D7,($12,A2,D3.L)", 0137562, 0x3812);
    ABSW("EOR.W D7,(@W).W", "FFFE", 0137570, 0xFFFE);
    TEST("EOR.W D7,($123456).L",   0137571, 0x0012, 0x3456);
    ERRT("EOR.W D7,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0137500);
    ERRT("EOR.W D7,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0137500);
    ERRT("EOR.W D7,#$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0137500);
    TEST("EOR.L D7,D2",            0137602);
    ERRT("EOR.L D7,A2",            OPERAND_NOT_ALLOWED, "A2", 0137600);
    TEST("EOR.L D7,(A2)",          0137622);
    TEST("EOR.L D7,(A2)+",         0137632);
    TEST("EOR.L D7,-(A2)",         0137642);
    TEST("EOR.L D7,($1234,A2)",    0137652, 0x1234);
    TEST("EOR.L D7,($12,A2,D3.L)", 0137662, 0x3812);
    ABSW("EOR.L D7,(@W).W", "FFFC", 0137670, 0xFFFC);
    TEST("EOR.L D7,($123454).L",   0137671, 0x0012, 0x3454);
    ERRT("EOR.L D7,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0137600);
    ERRT("EOR.L D7,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0137600);
    ERRT("EOR.L D7,#$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0137600);

    // EORI #nn,dst: 0005|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("EORI.B #$12,D2",            0005002, 0x0012);
    ERRT("EORI.B #$12,A2",            OPERAND_NOT_ALLOWED, "A2", 0005000);
    TEST("EORI.B #$12,(A2)",          0005022, 0x0012);
    TEST("EORI.B #$12,(A2)+",         0005032, 0x0012);
    TEST("EORI.B #$12,-(A2)",         0005042, 0x0012);
    TEST("EORI.B #$12,($1234,A2)",    0005052, 0x0012, 0x1234);
    TEST("EORI.B #$12,($12,A2,D3.W)", 0005062, 0x0012, 0x3012);
    TEST("EORI.B #$12,($001234).W",   0005070, 0x0012, 0x1234);
    TEST("EORI.B #$12,($123456).L",   0005071, 0x0012, 0x0012, 0x3456);
    ERRT("EORI.B #$12,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0005000);
    ERRT("EORI.B #$12,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0005000);
    ERRT("EORI.B #$12,#$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0005000);
    TEST("EORI.W #$5678,D2",            0005102, 0x5678);
    ERRT("EORI.W #$5678,A2",            OPERAND_NOT_ALLOWED, "A2", 0005100);
    TEST("EORI.W #$5678,(A2)",          0005122, 0x5678);
    TEST("EORI.W #$5678,(A2)+",         0005132, 0x5678);
    TEST("EORI.W #$5678,-(A2)",         0005142, 0x5678);
    TEST("EORI.W #$5678,($1234,A2)",    0005152, 0x5678, 0x1234);
    TEST("EORI.W #$5678,($12,A2,D3.W)", 0005162, 0x5678, 0x3012);
    TEST("EORI.W #$5678,($001234).W",   0005170, 0x5678, 0x1234);
    TEST("EORI.W #$5678,($123456).L",   0005171, 0x5678, 0x0012, 0x3456);
    ERRT("EORI.W #$5678,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0005100);
    ERRT("EORI.W #$5678,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0005100);
    ERRT("EORI.W #$5678,#$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0005100);
    TEST("EORI.L #$3456789A,D2",            0005202, 0x3456, 0x789A);
    ERRT("EORI.L #$3456789A,A2",            OPERAND_NOT_ALLOWED, "A2", 0005200);
    TEST("EORI.L #$3456789A,(A2)",          0005222, 0x3456, 0x789A);
    TEST("EORI.L #$3456789A,(A2)+",         0005232, 0x3456, 0x789A);
    TEST("EORI.L #$3456789A,-(A2)",         0005242, 0x3456, 0x789A);
    TEST("EORI.L #$3456789A,($1234,A2)",    0005252, 0x3456, 0x789A, 0x1234);
    TEST("EORI.L #$3456789A,($12,A2,D3.W)", 0005262, 0x3456, 0x789A, 0x3012);
    TEST("EORI.L #$3456789A,($001234).W",   0005270, 0x3456, 0x789A, 0x1234);
    TEST("EORI.L #$34567898,($123454).L",   0005271, 0x3456, 0x7898, 0x0012, 0x3454);
    ERRT("EORI.L #$3456789A,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0005200);
    ERRT("EORI.L #$3456789A,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0005200);
    ERRT("EORI.L #$3456789A,#$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0005200);

    // NOT dst: 0043|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("NOT.B D2",            0043002);
    ERRT("NOT.B A2",            OPERAND_NOT_ALLOWED, "A2", 0043000);
    TEST("NOT.B (A2)",          0043022);
    TEST("NOT.B (A2)+",         0043032);
    TEST("NOT.B -(A2)",         0043042);
    TEST("NOT.B ($1234,A2)",    0043052, 0x1234);
    TEST("NOT.B ($12,A2,D3.W)", 0043062, 0x3012);
    TEST("NOT.B ($001234).W",   0043070, 0x1234);
    TEST("NOT.B ($123456).L",   0043071, 0x0012, 0x3456);
    ERRT("NOT.B (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0043000);
    ERRT("NOT.B (*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0043000);
    ERRT("NOT.B #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0043000);
    TEST("NOT.W D2",            0043102);
    ERRT("NOT.W A2",            OPERAND_NOT_ALLOWED, "A2", 0043100);
    TEST("NOT.W (A2)",          0043122);
    TEST("NOT.W (A2)+",         0043132);
    TEST("NOT.W -(A2)",         0043142);
    TEST("NOT.W ($1234,A2)",    0043152, 0x1234);
    TEST("NOT.W ($12,A2,D3.W)", 0043162, 0x3012);
    TEST("NOT.W ($001234).W",   0043170, 0x1234);
    TEST("NOT.W ($123456).L",   0043171, 0x0012, 0x3456);
    ERRT("NOT.W (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0043100);
    ERRT("NOT.W (*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0043100);
    ERRT("NOT.W #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0043100);
    TEST("NOT.L D2",            0043202);
    ERRT("NOT.L A2",            OPERAND_NOT_ALLOWED, "A2", 0043200);
    TEST("NOT.L (A2)",          0043222);
    TEST("NOT.L (A2)+",         0043232);
    TEST("NOT.L -(A2)",         0043242);
    TEST("NOT.L ($1234,A2)",    0043252, 0x1234);
    TEST("NOT.L ($12,A2,D3.W)", 0043262, 0x3012);
    TEST("NOT.L ($001234).W",   0043270, 0x1234);
    TEST("NOT.L ($123454).L",   0043271, 0x0012, 0x3454);
    ERRT("NOT.L (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0043200);
    ERRT("NOT.L (*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0043200);
    ERRT("NOT.L #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0043200);

    // OR src,Dn, 010|Dn|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("OR.B D2,D7",              0107002);
    ERRT("OR.B D2,A2",              OPERAND_NOT_ALLOWED, "A2", 0102400);
    TEST("OR.B (A2),D7",            0107022);
    TEST("OR.B (A2)+,D7",           0107032);
    TEST("OR.B -(A2),D7",           0107042);
    TEST("OR.B ($1234,A2),D7",      0107052, 0x1234);
    TEST("OR.B ($12,A2,D3.L),D7",   0107062, 0x3812);
    ABSW("OR.B (@W).W,D7", "FFFF",  0107070, 0xFFFF);
    TEST("OR.B ($123456).L,D7",     0107071, 0x0012, 0x3456);
    TEST("OR.B (*+$1234,PC),D7",    0107072, 0x1232);
    TEST("OR.B (*+$12,PC,D3.W),D7", 0107073, 0x3010);
    TEST("OR.B #$34,D7",            0107074, 0x0034);
    TEST("OR.W D2,D7",              0107102);
    ERRT("OR.W A2,D7",              OPERAND_NOT_ALLOWED, "A2,D7", 0107100);
    TEST("OR.W (A2),D7",            0107122);
    TEST("OR.W (A2)+,D7",           0107132);
    TEST("OR.W -(A2),D7",           0107142);
    TEST("OR.W ($1234,A2),D7",      0107152, 0x1234);
    TEST("OR.W ($12,A2,D3.L),D7",   0107162, 0x3812);
    ABSW("OR.W (@W).W,D7", "FFFE",  0107170, 0xFFFE);
    TEST("OR.W ($123456).L,D7",     0107171, 0x0012, 0x3456);
    TEST("OR.W (*+$1234,PC),D7",    0107172, 0x1232);
    TEST("OR.W (*+$12,PC,D3.W),D7", 0107173, 0x3010);
    TEST("OR.W #$0034,D7",          0107174, 0x0034);
    TEST("OR.L D2,D7",              0107202);
    ERRT("OR.L A2,D7",              OPERAND_NOT_ALLOWED, "A2,D7", 0107200);
    TEST("OR.L (A2),D7",            0107222);
    TEST("OR.L (A2)+,D7",           0107232);
    TEST("OR.L -(A2),D7",           0107242);
    TEST("OR.L ($1234,A2),D7",      0107252, 0x1234);
    TEST("OR.L ($12,A2,D3.L),D7",   0107262, 0x3812);
    ABSW("OR.L (@W).W,D7", "FFFC",  0107270, 0xFFFC);
    TEST("OR.L ($123454).L,D7",     0107271, 0x0012, 0x3454);
    TEST("OR.L (*+$1234,PC),D7",    0107272, 0x1232);
    TEST("OR.L (*+$12,PC,D3.W),D7", 0107273, 0x3010);
    TEST("OR.L #$00345678,D7",      0107274, 0x0034, 0x5678);

    // OR Dn,dst: 010|Dn|Sz|M|Rn, Sz:B=4/W=5/L=6
    TEST("OR.B D7,D2",            0102007);
    ERRT("OR.B D7,A2",            OPERAND_NOT_ALLOWED, "A2", 0107400);
    TEST("OR.B D7,(A2)",          0107422);
    TEST("OR.B D7,(A2)+",         0107432);
    TEST("OR.B D7,-(A2)",         0107442);
    TEST("OR.B D7,($1234,A2)",    0107452, 0x1234);
    TEST("OR.B D7,($12,A2,D3.L)", 0107462, 0x3812);
    ABSW("OR.B D7,(@W).W", "FFFF", 0107470, 0xFFFF);
    TEST("OR.B D7,($123456).L",   0107471, 0x0012, 0x3456);
    ERRT("OR.B D7,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0107400);
    ERRT("OR.B D7,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0107400);
    ERRT("OR.B D7,#$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0107400);
    TEST("OR.W D7,D2",            0102107);
    ERRT("OR.W D7,A2",            OPERAND_NOT_ALLOWED, "A2", 0107500);
    TEST("OR.W D7,(A2)",          0107522);
    TEST("OR.W D7,(A2)+",         0107532);
    TEST("OR.W D7,-(A2)",         0107542);
    TEST("OR.W D7,($1234,A2)",    0107552, 0x1234);
    TEST("OR.W D7,($12,A2,D3.L)", 0107562, 0x3812);
    ABSW("OR.W D7,(@W).W", "FFFE", 0107570, 0xFFFE);
    TEST("OR.W D7,($123456).L",   0107571, 0x0012, 0x3456);
    ERRT("OR.W D7,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0107500);
    ERRT("OR.W D7,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0107500);
    ERRT("OR.W D7,#$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0107500);
    TEST("OR.L D7,D2",            0102207);
    ERRT("OR.L D7,A2",            OPERAND_NOT_ALLOWED, "A2", 0107600);
    TEST("OR.L D7,(A2)",          0107622);
    TEST("OR.L D7,(A2)+",         0107632);
    TEST("OR.L D7,-(A2)",         0107642);
    TEST("OR.L D7,($1234,A2)",    0107652, 0x1234);
    TEST("OR.L D7,($12,A2,D3.L)", 0107662, 0x3812);
    ABSW("OR.L D7,(@W).W", "FFFC", 0107670, 0xFFFC);
    TEST("OR.L D7,($123454).L",   0107671, 0x0012, 0x3454);
    ERRT("OR.L D7,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0107600);
    ERRT("OR.L D7,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0107600);
    ERRT("OR.L D7,#$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0107600);

    // ORI #nn,dst: 0000|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("ORI.B #$12,D2",            0000002, 0x0012);
    ERRT("ORI.B #$12,A2",            OPERAND_NOT_ALLOWED, "A2", 0000000);
    TEST("ORI.B #$12,(A2)",          0000022, 0x0012);
    TEST("ORI.B #$12,(A2)+",         000032, 0x0012);
    TEST("ORI.B #$12,-(A2)",         0000042, 0x0012);
    TEST("ORI.B #$12,($1234,A2)",    0000052, 0x0012, 0x1234);
    TEST("ORI.B #$12,($12,A2,D3.W)", 0000062, 0x0012, 0x3012);
    TEST("ORI.B #$12,($001234).W",   0000070, 0x0012, 0x1234);
    TEST("ORI.B #$12,($123456).L",   0000071, 0x0012, 0x0012, 0x3456);
    ERRT("ORI.B #$12,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0000000);
    ERRT("ORI.B #$12,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0000000);
    ERRT("ORI.B #$12,#$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0000000);
    TEST("ORI.W #$5678,D2",            0000102, 0x5678);
    ERRT("ORI.W #$5678,A2",            OPERAND_NOT_ALLOWED, "A2", 0000100);
    TEST("ORI.W #$5678,(A2)",          0000122, 0x5678);
    TEST("ORI.W #$5678,(A2)+",         0000132, 0x5678);
    TEST("ORI.W #$5678,-(A2)",         0000142, 0x5678);
    TEST("ORI.W #$5678,($1234,A2)",    0000152, 0x5678, 0x1234);
    TEST("ORI.W #$5678,($12,A2,D3.W)", 0000162, 0x5678, 0x3012);
    TEST("ORI.W #$5678,($001234).W",   0000170, 0x5678, 0x1234);
    TEST("ORI.W #$5678,($123456).L",   0000171, 0x5678, 0x0012, 0x3456);
    ERRT("ORI.W #$5678,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0000100);
    ERRT("ORI.W #$5678,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0000100);
    ERRT("ORI.W #$5678,#$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0000100);
    TEST("ORI.L #$3456789A,D2",            0000202, 0x3456, 0x789A);
    ERRT("ORI.L #$3456789A,A2",            OPERAND_NOT_ALLOWED, "A2", 0000200);
    TEST("ORI.L #$3456789A,(A2)",          0000222, 0x3456, 0x789A);
    TEST("ORI.L #$3456789A,(A2)+",         0000232, 0x3456, 0x789A);
    TEST("ORI.L #$3456789A,-(A2)",         0000242, 0x3456, 0x789A);
    TEST("ORI.L #$3456789A,($1234,A2)",    0000252, 0x3456, 0x789A, 0x1234);
    TEST("ORI.L #$3456789A,($12,A2,D3.W)", 0000262, 0x3456, 0x789A, 0x3012);
    TEST("ORI.L #$3456789A,($001234).W",   0000270, 0x3456, 0x789A, 0x1234);
    TEST("ORI.L #$34567898,($123454).L",   0000271, 0x3456, 0x7898, 0x0012, 0x3454);
    ERRT("ORI.L #$3456789A,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0000200);
    ERRT("ORI.L #$3456789A,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0000200);
    ERRT("ORI.L #$3456789A,#$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0000200);
}

void test_shift_rotate() {
    // ASL Dx,Dy: 016|Dx|Sz|4|Dy, Sz:B=4/W=5/L=6
    TEST("ASL.B D2,D7", 0162447);
    TEST("ASL.W D2,D7", 0162547);
    TEST("ASL.L D2,D7", 0162647);

    // ASL #nn,Dy: 016|nn|Sz|0|Dy, Sz:B=4/W=5/L=6
    ERRT("ASL.B #0,D7",  OPERAND_NOT_ALLOWED, "#0,D7", 0160407);
    TEST("ASL.B #1,D7",  0161407);
    TEST("ASL.B #2,D7",  0162407);
    TEST("ASL.W #4,D7",  0164507);
    TEST("ASL.L #8,D7",  0160607);
    ERRT("ASL.B #9,D7",  OVERFLOW_RANGE, "#9,D7",  0161407);
    ERRT("ASL.B #-1,D7", OVERFLOW_RANGE, "#-1,D7", 0167407);

    // ASL dst: 01607|M|Rn
    ERRT("ASL D2",            OPERAND_NOT_ALLOWED, "D2", 0160700);
    ERRT("ASL A2",            OPERAND_NOT_ALLOWED, "A2", 0160700);
    TEST("ASL   (A2)",        0160722);
    TEST("ASL.W (A2)",        0160722);
    TEST("ASL (A2)+",         0160732);
    TEST("ASL -(A2)",         0160742);
    TEST("ASL ($2345,A2)",    0160752, 0x2345);
    TEST("ASL ($23,A2,D3.L)", 0160762, 0x3823);
    TEST("ASL ($002346).W",   0160770, 0x2346);
    TEST("ASL ($234568).L",   0160771, 0x0023, 0x4568);
    ERRT("ASL (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0160700);
    ERRT("ASL (*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0160700);
    ERRT("ASL #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0160700);

    // ASR Dx,Dy: 016|Dx|Sz|4|Dy, Sz:B=0/W=1/L=2
    TEST("ASR.B D2,D7", 0162047);
    TEST("ASR.W D2,D7", 0162147);
    TEST("ASR.L D2,D7", 0162247);

    // ASR #nn,Dy: 016|Dx|Sz|0|Dy, Sz:B=0/W=1/L=2
    TEST("ASR.B #1,D7", 0161007);
    TEST("ASR.W #4,D7", 0164107);
    TEST("ASR.L #8,D7", 0160207);

    // ASR dst: 01603|M|Rn
    ERRT("ASR D2",            OPERAND_NOT_ALLOWED, "D2", 0160300);
    ERRT("ASR A2",            OPERAND_NOT_ALLOWED, "A2", 0160300);
    TEST("ASR (A2)",          0160322);
    TEST("ASR   (A2)+",       0160332);
    TEST("ASR.W (A2)+",       0160332);
    TEST("ASR -(A2)",         0160342);
    TEST("ASR ($2345,A2)",    0160352, 0x2345);
    TEST("ASR ($23,A2,D3.L)", 0160362, 0x3823);
    TEST("ASR ($002346).W",   0160370, 0x2346);
    TEST("ASR ($234568).L",   0160371, 0x0023, 0x4568);
    ERRT("ASR (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0160300);
    ERRT("ASR (*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0160300);
    ERRT("ASR #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0160300);

    // LSL Dx,Dy: 016|Dx|Sz|5|Dy, Sz:B=4/W=5/L=6
    TEST("LSL.B D2,D7", 0162457);
    TEST("LSL.W D2,D7", 0162557);
    TEST("LSL.L D2,D7", 0162657);

    // LSL #n,Dy: 016|nn|Sz|1|Dy: Sz:B=4/W=5/L=6
    TEST("LSL.B #1,D7", 0161417);
    TEST("LSL.W #4,D7", 0164517);
    TEST("LSL.L #8,D7", 0160617);

    // LSL dst: 01617|M|Rn
    ERRT("LSL D2",            OPERAND_NOT_ALLOWED, "D2", 0161700);
    ERRT("LSL A2",            OPERAND_NOT_ALLOWED, "A2", 0161700);
    TEST("LSL (A2)",          0161722);
    TEST("LSL (A2)+",         0161732);
    TEST("LSL   -(A2)",       0161742);
    TEST("LSL.W -(A2)",       0161742);
    TEST("LSL ($2345,A2)",    0161752, 0x2345);
    TEST("LSL ($23,A2,D3.L)", 0161762, 0x3823);
    TEST("LSL ($002346).W",   0161770, 0x2346);
    TEST("LSL ($234568).L",   0161771, 0x0023, 0x4568);
    ERRT("LSL (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0161700);
    ERRT("LSL (*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0161700);
    ERRT("LSL #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0161700);

    // LSR Dx,Dy: 016|Dx|Sz|5|Dy, Sz:B=0/W=1/L=2
    TEST("LSR.B D2,D7", 0162057);
    TEST("LSR.W D2,D7", 0162157);
    TEST("LSR.L D2,D7", 0162257);

    // LSR #n,Dy: 016|nn|Sz|1|Dy, Sz:B=0/W=1/L=2
    TEST("LSR.B #1,D7", 0161017);
    TEST("LSR.W #4,D7", 0164117);
    TEST("LSR.L #8,D7", 0160217);

    // LSR dst: 01613|M|Rn
    ERRT("LSR D2",            OPERAND_NOT_ALLOWED, "D2", 0161300);
    ERRT("LSR A2",            OPERAND_NOT_ALLOWED, "A2", 0161300);
    TEST("LSR (A2)",          0161322);
    TEST("LSR (A2)+",         0161332);
    TEST("LSR -(A2)",         0161342);
    TEST("LSR   ($2345,A2)",  0161352, 0x2345);
    TEST("LSR.W ($2345,A2)",  0161352, 0x2345);
    TEST("LSR ($23,A2,D3.L)", 0161362, 0x3823);
    TEST("LSR ($002346).W",   0161370, 0x2346);
    TEST("LSR ($234568).L",   0161371, 0x0023, 0x4568);
    ERRT("LSR (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0161300);
    ERRT("LSR (*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0161300);
    ERRT("LSR #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0161300);

    // ROL Dx,Dy: 016|Dx|Sz|7|Dy, Sz:B=4/W=5/L=6
    TEST("ROL.B D2,D7", 0162477);
    TEST("ROL.W D2,D7", 0162577);
    TEST("ROL.L D2,D7", 0162677);

    // ROL #n,Dy: 016|nn|Sz|3|Dy, Sz:B=4/W=5/L=6
    TEST("ROL.B #1,D7", 0161437);
    TEST("ROL.W #4,D7", 0164537);
    TEST("ROL.L #8,D7", 0160637);

    // ROL dst: 01637|M|Rn
    ERRT("ROL D2",            OPERAND_NOT_ALLOWED, "D2", 0163700);
    ERRT("ROL A2",            OPERAND_NOT_ALLOWED, "A2", 0163700);
    TEST("ROL   (A2)",        0163722);
    TEST("ROL.W (A2)",        0163722);
    TEST("ROL (A2)+",         0163732);
    TEST("ROL -(A2)",         0163742);
    TEST("ROL ($2345,A2)",    0163752, 0x2345);
    TEST("ROL ($23,A2,D3.L)", 0163762, 0x3823);
    TEST("ROL ($002346).W",   0163770, 0x2346);
    TEST("ROL ($234568).L",   0163771, 0x0023, 0x4568);
    ERRT("ROL (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",   0163700);
    ERRT("ROL (*+$12,PC,D3)",  OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0163700);
    ERRT("ROL #$1234",        OPERAND_NOT_ALLOWED, "#$1234",         0163700);

    // ROR Dx,Dy: 016|Dx|Sz|7|Dy, Sz:B=0/W=1/L=2
    TEST("ROR.B D2,D7", 0162077);
    TEST("ROR.W D2,D7", 0162177);
    TEST("ROR.L D2,D7", 0162277);

    // ROR #n,Dy: 016|Dx|Sz|3|Dy, Sz:B=0/W=1/L=2
    TEST("ROR.B #1,D7", 0161037);
    TEST("ROR.W #4,D7", 0164137);
    TEST("ROR.L #8,D7", 0160237);

    // ROR dst: 01633|M|Rn
    ERRT("ROR D2",            OPERAND_NOT_ALLOWED, "D2", 0163300);
    ERRT("ROR A2",            OPERAND_NOT_ALLOWED, "A2", 0163300);
    TEST("ROR (A2)",          0163322);
    TEST("ROR   (A2)+",       0163332);
    TEST("ROR.W (A2)+",       0163332);
    TEST("ROR -(A2)",         0163342);
    TEST("ROR ($2345,A2)",    0163352, 0x2345);
    TEST("ROR ($23,A2,D3.L)", 0163362, 0x3823);
    TEST("ROR ($002346).W",   0163370, 0x2346);
    TEST("ROR ($234568).L",   0163371, 0x0023, 0x4568);
    ERRT("ROR (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0163300);
    ERRT("ROR (*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0163300);
    ERRT("ROR #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0163300);

    // ROXL Dx,Dy: 016|Dx|Sz|6|Dy, Sz:B=4/W=5/L=6
    TEST("ROXL.B D2,D7", 0162467);
    TEST("ROXL.W D2,D7", 0162567);
    TEST("ROXL.L D2,D7", 0162667);

    // ROXL #n,Dy: 016|nn|Sz|2|Dy, Sz:B=4/W=5/L=6
    TEST("ROXL.B #1,D7", 0161427);
    TEST("ROXL.W #4,D7", 0164527);
    TEST("ROXL.L #8,D7", 0160627);

    // ROXL dst: 01627|M|Rn
    ERRT("ROXL D2",            OPERAND_NOT_ALLOWED, "D2", 0162700);
    ERRT("ROXL A2",            OPERAND_NOT_ALLOWED, "A2", 0162700);
    TEST("ROXL (A2)",          0162722);
    TEST("ROXL (A2)+",         0162732);
    TEST("ROXL   -(A2)",       0162742);
    TEST("ROXL.W -(A2)",       0162742);
    TEST("ROXL ($2345,A2)",    0162752, 0x2345);
    TEST("ROXL ($23,A2,D3.L)", 0162762, 0x3823);
    TEST("ROXL ($002346).W",   0162770, 0x2346);
    TEST("ROXL ($234568).L",   0162771, 0x0023, 0x4568);
    ERRT("ROXL (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0162700);
    ERRT("ROXL (*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0162700);
    ERRT("ROXL #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0162700);

    // ROXR Dx,Dy: 016|Dx|Sz|6|Dy, Sz:B=0/W=1/L=2
    TEST("ROXR.B D2,D7", 0162067);
    TEST("ROXR.W D2,D7", 0162167);
    TEST("ROXR.L D2,D7", 0162267);

    // ROXR #n,Dy: 016|nn|Sz|2|Dy, Sz:B=0/W=1/L=2
    TEST("ROXR.B #1,D7", 0161027);
    TEST("ROXR.W #4,D7", 0164127);
    TEST("ROXR.L #8,D7", 0160227);

    // ROXR dst: 01623|M|Rn
    ERRT("ROXR D2",            OPERAND_NOT_ALLOWED, "D2", 0162300);
    ERRT("ROXR A2",            OPERAND_NOT_ALLOWED, "A2", 0162300);
    TEST("ROXR   (A2)",        0162322);
    TEST("ROXR.W (A2)",        0162322);
    TEST("ROXR (A2)+",         0162332);
    TEST("ROXR -(A2)",         0162342);
    TEST("ROXR ($2345,A2)",    0162352, 0x2345);
    TEST("ROXR ($23,A2,D3.L)", 0162362, 0x3823);
    TEST("ROXR ($002346).W",   0162370, 0x2346);
    TEST("ROXR ($234568).L",   0162371, 0x0023, 0x4568);
    ERRT("ROXR (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0162300);
    ERRT("ROXR (*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0162300);
    ERRT("ROXR #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0162300);

    // SWAP Dn: 004410|Dn
    TEST("SWAP   D0", 0044100);
    TEST("SWAP   D7", 0044107);
    TEST("SWAP.L D7", 0044107);
    ERRT("SWAP A0", OPERAND_NOT_ALLOWED, "A0");
}

void test_bit() {
    // BCHG Dx,dst: 000|Dx|5|M|Rn
    TEST("BCHG   D7,D2",            0007502);
    ERRT("BCHG.B D7,D2",            OPERAND_NOT_ALLOWED, "D2", 0007500);
    ERRT("BCHG.W D7,D2",            OPERAND_NOT_ALLOWED, "D7,D2");
    TEST("BCHG.L D7,D2",            0007502);
    ERRT("BCHG   D7,A2",            OPERAND_NOT_ALLOWED, "A2", 0007500);
    ERRT("BCHG.B D7,A2",            OPERAND_NOT_ALLOWED, "A2", 0007500);
    ERRT("BCHG.W D7,A2",            OPERAND_NOT_ALLOWED, "D7,A2");
    ERRT("BCHG.L D7,A2",            OPERAND_NOT_ALLOWED, "D7,A2");
    TEST("BCHG   D7,(A2)",          0007522);
    TEST("BCHG.B D7,(A2)",          0007522);
    ERRT("BCHG.W D7,(A2)",          OPERAND_NOT_ALLOWED, "D7,(A2)");
    ERRT("BCHG.L D7,(A2)",          OPERAND_NOT_ALLOWED, "D7,(A2)");
    TEST("BCHG   D7,(A2)+",         0007532);
    TEST("BCHG.B D7,(A2)+",         0007532);
    ERRT("BCHG.W D7,(A2)+",         OPERAND_NOT_ALLOWED, "D7,(A2)+");
    ERRT("BCHG.L D7,(A2)+",         OPERAND_NOT_ALLOWED, "D7,(A2)+");
    TEST("BCHG   D7,-(A2)",         0007542);
    TEST("BCHG.B D7,-(A2)",         0007542);
    ERRT("BCHG.W D7,-(A2)",         OPERAND_NOT_ALLOWED, "D7,-(A2)");
    ERRT("BCHG.L D7,-(A2)",         OPERAND_NOT_ALLOWED, "D7,-(A2)");
    TEST("BCHG   D7,($1234,A2)",    0007552, 0x1234);
    TEST("BCHG.B D7,($1234,A2)",    0007552, 0x1234);
    ERRT("BCHG.W D7,($1234,A2)",    OPERAND_NOT_ALLOWED, "D7,($1234,A2)");
    ERRT("BCHG.L D7,($1234,A2)",    OPERAND_NOT_ALLOWED, "D7,($1234,A2)");
    TEST("BCHG   D7,($12,A2,D3.W)", 0007562, 0x3012);
    TEST("BCHG.B D7,($12,A2,D3.W)", 0007562, 0x3012);
    ERRT("BCHG.W D7,($12,A2,D3.W)", OPERAND_NOT_ALLOWED, "D7,($12,A2,D3.W)");
    ERRT("BCHG.L D7,($12,A2,D3.W)", OPERAND_NOT_ALLOWED, "D7,($12,A2,D3.W)");
    TEST("BCHG   D7,($001234).W",   0007570, 0x1234);
    TEST("BCHG.B D7,($001234).W",   0007570, 0x1234);
    ERRT("BCHG.W D7,($001234).W",   OPERAND_NOT_ALLOWED, "D7,($001234).W");
    ERRT("BCHG.L D7,($001234).W",   OPERAND_NOT_ALLOWED, "D7,($001234).W");
    TEST("BCHG   D7,($012345).L",   0007571, 0x0001, 0x2345);
    TEST("BCHG.B D7,($012345).L",   0007571, 0x0001, 0x2345);
    ERRT("BCHG.W D7,($012345).L",   OPERAND_NOT_ALLOWED, "D7,($012345).L");
    ERRT("BCHG.L D7,($012345).L",   OPERAND_NOT_ALLOWED, "D7,($012345).L");
    ERRT("BCHG   D7,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)", 0007500);
    ERRT("BCHG.B D7,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)", 0007500);
    ERRT("BCHG.W D7,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "D7,(*+$1234,PC)");
    ERRT("BCHG.L D7,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "D7,(*+$1234,PC)");
    ERRT("BCHG   D7,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0007500);
    ERRT("BCHG.W D7,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "D7,(*+$12,PC,D3)");
    ERRT("BCHG.L D7,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "D7,(*+$12,PC,D3)");
    ERRT("BCHG   D7,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0007500);
    ERRT("BCHG   D7,#$1234",        OPERAND_NOT_ALLOWED, "#$1234", 0007500);
    ERRT("BCHG.B D7,#$1234",        OPERAND_NOT_ALLOWED, "#$1234", 0007500);
    ERRT("BCHG.W D7,#$1234",        OPERAND_NOT_ALLOWED, "D7,#$1234");
    ERRT("BCHG.L D7,#$1234",        OPERAND_NOT_ALLOWED, "D7,#$1234");

    // BCHG #n,dst: 00041|M|Rn
    TEST("BCHG   #0,D2",            0004102, 0x0000);
    ERRT("BCHG.B #0,D2",            OPERAND_NOT_ALLOWED, "D2", 0004100);
    ERRT("BCHG.W #0,D2",            OPERAND_NOT_ALLOWED, "#0,D2");
    TEST("BCHG.L #0,D2",            0004102, 0x0000);
    TEST("BCHG.L #31,D2",           0004102, 0x001F);
    ERRT("BCHG.L #32,D2",           ILLEGAL_BIT_NUMBER,  "#32,D2", 0004102, 0x0000);
    ERRT("BCHG   #0,A2",            OPERAND_NOT_ALLOWED, "A2", 0004100);
    ERRT("BCHG.L #0,A2",            OPERAND_NOT_ALLOWED, "#0,A2");
    ERRT("BCHG.W #0,A2",            OPERAND_NOT_ALLOWED, "#0,A2");
    ERRT("BCHG.L #0,A2",            OPERAND_NOT_ALLOWED, "#0,A2");
    TEST("BCHG   #7,(A2)",          0004122, 0x0007);
    TEST("BCHG.B #7,(A2)",          0004122, 0x0007);
    ERRT("BCHG.B #8,(A2)",          ILLEGAL_BIT_NUMBER,  "#8,(A2)", 0004122, 0x0000);
    ERRT("BCHG.W #7,(A2)",          OPERAND_NOT_ALLOWED, "#7,(A2)");
    ERRT("BCHG.L #7,(A2)",          OPERAND_NOT_ALLOWED, "#7,(A2)");
    TEST("BCHG   #6,(A2)+",         0004132, 0x0006);
    TEST("BCHG.B #6,(A2)+",         0004132, 0x0006);
    ERRT("BCHG.W #6,(A2)+",         OPERAND_NOT_ALLOWED, "#6,(A2)+");
    ERRT("BCHG.L #6,(A2)+",         OPERAND_NOT_ALLOWED, "#6,(A2)+");
    TEST("BCHG   #5,-(A2)",         0004142, 0x0005);
    TEST("BCHG.B #5,-(A2)",         0004142, 0x0005);
    ERRT("BCHG.W #5,-(A2)",         OPERAND_NOT_ALLOWED, "#5,-(A2)");
    ERRT("BCHG.L #5,-(A2)",         OPERAND_NOT_ALLOWED, "#5,-(A2)");
    TEST("BCHG   #4,($1234,A2)",    0004152, 0x0004, 0x1234);
    TEST("BCHG.B #4,($1234,A2)",    0004152, 0x0004, 0x1234);
    ERRT("BCHG.W #4,($1234,A2)",    OPERAND_NOT_ALLOWED, "#4,($1234,A2)");
    ERRT("BCHG.L #4,($1234,A2)",    OPERAND_NOT_ALLOWED, "#4,($1234,A2)");
    TEST("BCHG   #3,($12,A2,D3.W)", 0004162, 0x0003, 0x3012);
    TEST("BCHG.B #3,($12,A2,D3.W)", 0004162, 0x0003, 0x3012);
    ERRT("BCHG.W #3,($12,A2,D3.W)", OPERAND_NOT_ALLOWED, "#3,($12,A2,D3.W)");
    ERRT("BCHG.L #3,($12,A2,D3.W)", OPERAND_NOT_ALLOWED, "#3,($12,A2,D3.W)");
    TEST("BCHG   #2,($001234).W",   0004170, 0x0002, 0x1234);
    TEST("BCHG.B #2,($001234).W",   0004170, 0x0002, 0x1234);
    ERRT("BCHG.W #2,($001234).W",   OPERAND_NOT_ALLOWED, "#2,($001234).W");
    ERRT("BCHG.L #2,($001234).W",   OPERAND_NOT_ALLOWED, "#2,($001234).W");
    TEST("BCHG   #1,($012345).L",   0004171, 0x0001, 0x0001, 0x2345);
    TEST("BCHG.B #1,($012345).L",   0004171, 0x0001, 0x0001, 0x2345);
    ERRT("BCHG.W #1,($012345).L",   OPERAND_NOT_ALLOWED, "#1,($012345).L");
    ERRT("BCHG.L #1,($012345).L",   OPERAND_NOT_ALLOWED, "#1,($012345).L");
    ERRT("BCHG   #1,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)", 0004100);
    ERRT("BCHG.L #1,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "#1,(*+$1234,PC)");
    ERRT("BCHG.W #1,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "#1,(*+$1234,PC)");
    ERRT("BCHG.L #1,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "#1,(*+$1234,PC)");
    ERRT("BCHG   #2,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0004100);
    ERRT("BCHG.L #2,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "#2,(*+$12,PC,D3)");
    ERRT("BCHG.W #2,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "#2,(*+$12,PC,D3)");
    ERRT("BCHG.L #2,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "#2,(*+$12,PC,D3)");
    ERRT("BCHG   #3,#$1234",        OPERAND_NOT_ALLOWED, "#$1234", 0004100);
    ERRT("BCHG.L #3,#$1234",        OPERAND_NOT_ALLOWED, "#3,#$1234");
    ERRT("BCHG.W #3,#$1234",        OPERAND_NOT_ALLOWED, "#3,#$1234");
    ERRT("BCHG.L #3,#$1234",        OPERAND_NOT_ALLOWED, "#3,#$1234");

    // BCLR Dx,dst: 000|Dx|6|M|Rn
    TEST("BCLR   D7,D2",            0007602);
    ERRT("BCLR.B D7,D2",            OPERAND_NOT_ALLOWED, "D2", 0007600);
    ERRT("BCLR.W D7,D2",            OPERAND_NOT_ALLOWED, "D7,D2");
    TEST("BCLR.L D7,D2",            0007602);
    ERRT("BCLR   D7,A2",            OPERAND_NOT_ALLOWED, "A2", 0007600);
    ERRT("BCLR.B D7,A2",            OPERAND_NOT_ALLOWED, "A2", 0007600);
    ERRT("BCLR.W D7,A2",            OPERAND_NOT_ALLOWED, "D7,A2");
    ERRT("BCLR.L D7,A2",            OPERAND_NOT_ALLOWED, "D7,A2");
    ERRT("BCLR.L D7,A2",            OPERAND_NOT_ALLOWED, "D7,A2");
    TEST("BCLR   D7,(A2)",          0007622);
    TEST("BCLR.B D7,(A2)",          0007622);
    ERRT("BCLR.W D7,(A2)",          OPERAND_NOT_ALLOWED, "D7,(A2)");
    ERRT("BCLR.L D7,(A2)",          OPERAND_NOT_ALLOWED, "D7,(A2)");
    TEST("BCLR   D7,(A2)+",         0007632);
    TEST("BCLR.B D7,(A2)+",         0007632);
    ERRT("BCLR.W D7,(A2)+",         OPERAND_NOT_ALLOWED, "D7,(A2)+");
    ERRT("BCLR.L D7,(A2)+",         OPERAND_NOT_ALLOWED, "D7,(A2)+");
    TEST("BCLR   D7,-(A2)",         0007642);
    TEST("BCLR.B D7,-(A2)",         0007642);
    ERRT("BCLR.W D7,-(A2)",         OPERAND_NOT_ALLOWED, "D7,-(A2)");
    ERRT("BCLR.L D7,-(A2)",         OPERAND_NOT_ALLOWED, "D7,-(A2)");
    TEST("BCLR   D7,($1234,A2)",    0007652, 0x1234);
    TEST("BCLR.B D7,($1234,A2)",    0007652, 0x1234);
    ERRT("BCLR.W D7,($1234,A2)",    OPERAND_NOT_ALLOWED, "D7,($1234,A2)");
    ERRT("BCLR.L D7,($1234,A2)",    OPERAND_NOT_ALLOWED, "D7,($1234,A2)");
    TEST("BCLR   D7,($12,A2,D3.W)", 0007662, 0x3012);
    TEST("BCLR.B D7,($12,A2,D3.W)", 0007662, 0x3012);
    ERRT("BCLR.W D7,($12,A2,D3.W)", OPERAND_NOT_ALLOWED, "D7,($12,A2,D3.W)");
    ERRT("BCLR.L D7,($12,A2,D3.W)", OPERAND_NOT_ALLOWED, "D7,($12,A2,D3.W)");
    TEST("BCLR   D7,($001234).W",   0007670, 0x1234);
    TEST("BCLR.B D7,($001234).W",   0007670, 0x1234);
    ERRT("BCLR.W D7,($001234).W",   OPERAND_NOT_ALLOWED, "D7,($001234).W");
    ERRT("BCLR.L D7,($001234).W",   OPERAND_NOT_ALLOWED, "D7,($001234).W");
    TEST("BCLR   D7,($012345).L",   0007671, 0x0001, 0x2345);
    TEST("BCLR.B D7,($012345).L",   0007671, 0x0001, 0x2345);
    ERRT("BCLR.W D7,($012345).L",   OPERAND_NOT_ALLOWED, "D7,($012345).L");
    ERRT("BCLR.L D7,($012345).L",   OPERAND_NOT_ALLOWED, "D7,($012345).L");
    ERRT("BCLR   D7,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)", 0007600);
    ERRT("BCLR.B D7,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)", 0007600);
    ERRT("BCLR.W D7,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "D7,(*+$1234,PC)");
    ERRT("BCLR.L D7,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "D7,(*+$1234,PC)");
    ERRT("BCLR   D7,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0007600);
    ERRT("BCLR.B D7,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0007600);
    ERRT("BCLR.W D7,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "D7,(*+$12,PC,D3)");
    ERRT("BCLR.L D7,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "D7,(*+$12,PC,D3)");
    ERRT("BCLR   D7,#$1234",        OPERAND_NOT_ALLOWED, "#$1234", 0007600);
    ERRT("BCLR.B D7,#$1234",        OPERAND_NOT_ALLOWED, "#$1234", 0007600);
    ERRT("BCLR.W D7,#$1234",        OPERAND_NOT_ALLOWED, "D7,#$1234");
    ERRT("BCLR.L D7,#$1234",        OPERAND_NOT_ALLOWED, "D7,#$1234");

    // BCLR #n,dst: 00042|M|Rn
    TEST("BCLR   #0,D2",            0004202, 0x0000);
    ERRT("BCLR.B #0,D2",            OPERAND_NOT_ALLOWED, "D2", 0004200);
    ERRT("BCLR.W #0,D2",            OPERAND_NOT_ALLOWED, "#0,D2");
    TEST("BCLR.L #0,D2",            0004202, 0x0000);
    ERRT("BCLR   #1,A2",            OPERAND_NOT_ALLOWED, "A2", 0004200);
    ERRT("BCLR.B #1,A2",            OPERAND_NOT_ALLOWED, "A2", 0004200);
    ERRT("BCLR.W #1,A2",            OPERAND_NOT_ALLOWED, "#1,A2");
    ERRT("BCLR.L #1,A2",            OPERAND_NOT_ALLOWED, "#1,A2");
    TEST("BCLR   #7,(A2)",          0004222, 0x0007);
    TEST("BCLR.B #7,(A2)",          0004222, 0x0007);
    ERRT("BCLR.W #7,(A2)",          OPERAND_NOT_ALLOWED, "#7,(A2)");
    ERRT("BCLR.L #7,(A2)",          OPERAND_NOT_ALLOWED, "#7,(A2)");
    TEST("BCLR   #6,(A2)+",         0004232, 0x0006);
    TEST("BCLR.B #6,(A2)+",         0004232, 0x0006);
    ERRT("BCLR.W #6,(A2)+",         OPERAND_NOT_ALLOWED, "#6,(A2)+");
    ERRT("BCLR.L #6,(A2)+",         OPERAND_NOT_ALLOWED, "#6,(A2)+");
    TEST("BCLR   #5,-(A2)",         0004242, 0x0005);
    TEST("BCLR.B #5,-(A2)",         0004242, 0x0005);
    ERRT("BCLR.W #5,-(A2)",         OPERAND_NOT_ALLOWED, "#5,-(A2)");
    ERRT("BCLR.L #5,-(A2)",         OPERAND_NOT_ALLOWED, "#5,-(A2)");
    TEST("BCLR   #4,($1234,A2)",    0004252, 0x0004, 0x1234);
    TEST("BCLR.B #4,($1234,A2)",    0004252, 0x0004, 0x1234);
    ERRT("BCLR.W #4,($1234,A2)",    OPERAND_NOT_ALLOWED, "#4,($1234,A2)");
    ERRT("BCLR.L #4,($1234,A2)",    OPERAND_NOT_ALLOWED, "#4,($1234,A2)");
    TEST("BCLR   #3,($12,A2,D3.W)", 0004262, 0x0003, 0x3012);
    TEST("BCLR.B #3,($12,A2,D3.W)", 0004262, 0x0003, 0x3012);
    ERRT("BCLR.W #3,($12,A2,D3.W)", OPERAND_NOT_ALLOWED, "#3,($12,A2,D3.W)");
    ERRT("BCLR.L #3,($12,A2,D3.W)", OPERAND_NOT_ALLOWED, "#3,($12,A2,D3.W)");
    TEST("BCLR   #2,($001234).W",   0004270, 0x0002, 0x1234);
    TEST("BCLR.B #2,($001234).W",   0004270, 0x0002, 0x1234);
    ERRT("BCLR.W #2,($001234).W",   OPERAND_NOT_ALLOWED, "#2,($001234).W");
    ERRT("BCLR.L #2,($001234).W",   OPERAND_NOT_ALLOWED, "#2,($001234).W");
    TEST("BCLR   #1,($012345).L",   0004271, 0x0001, 0x0001, 0x2345);
    TEST("BCLR.B #1,($012345).L",   0004271, 0x0001, 0x0001, 0x2345);
    ERRT("BCLR.W #1,($012345).L",   OPERAND_NOT_ALLOWED, "#1,($012345).L");
    ERRT("BCLR.L #1,($012345).L",   OPERAND_NOT_ALLOWED, "#1,($012345).L");
    ERRT("BCLR   #4,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)", 0004200);
    ERRT("BCLR.B #4,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)", 0004200);
    ERRT("BCLR.W #4,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "#4,(*+$1234,PC)");
    ERRT("BCLR.L #4,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "#4,(*+$1234,PC)");
    ERRT("BCLR   #5,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0004200);
    ERRT("BCLR.B #5,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0004200);
    ERRT("BCLR.W #5,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "#5,(*+$12,PC,D3)");
    ERRT("BCLR.L #5,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "#5,(*+$12,PC,D3)");
    ERRT("BCLR   #6,#$1234",        OPERAND_NOT_ALLOWED, "#$1234", 0004200);
    ERRT("BCLR.B #6,#$1234",        OPERAND_NOT_ALLOWED, "#$1234", 0004200);
    ERRT("BCLR.W #6,#$1234",        OPERAND_NOT_ALLOWED, "#6,#$1234");
    ERRT("BCLR.L #6,#$1234",        OPERAND_NOT_ALLOWED, "#6,#$1234");

    // BSET Dx,dst: 000|Dx|7|M|Rn
    TEST("BSET   D7,D2",            0007702);
    ERRT("BSET.B D7,D2",            OPERAND_NOT_ALLOWED, "D2", 0007700);
    ERRT("BSET.W D7,D2",            OPERAND_NOT_ALLOWED, "D7,D2");
    TEST("BSET.L D7,D2",            0007702);
    ERRT("BSET   D7,A2",            OPERAND_NOT_ALLOWED, "A2", 0007700);
    ERRT("BSET.B D7,A2",            OPERAND_NOT_ALLOWED, "A2", 0007700);
    ERRT("BSET.W D7,A2",            OPERAND_NOT_ALLOWED, "D7,A2");
    ERRT("BSET.L D7,A2",            OPERAND_NOT_ALLOWED, "D7,A2");
    TEST("BSET   D7,(A2)",          0007722);
    TEST("BSET.B D7,(A2)",          0007722);
    ERRT("BSET.W D7,(A2)",          OPERAND_NOT_ALLOWED, "D7,(A2)");
    ERRT("BSET.L D7,(A2)",          OPERAND_NOT_ALLOWED, "D7,(A2)");
    TEST("BSET   D7,(A2)+",         0007732);
    TEST("BSET.B D7,(A2)+",         0007732);
    ERRT("BSET.W D7,(A2)+",         OPERAND_NOT_ALLOWED, "D7,(A2)+");
    ERRT("BSET.L D7,(A2)+",         OPERAND_NOT_ALLOWED, "D7,(A2)+");
    TEST("BSET   D7,-(A2)",         0007742);
    TEST("BSET.B D7,-(A2)",         0007742);
    ERRT("BSET.W D7,-(A2)",         OPERAND_NOT_ALLOWED, "D7,-(A2)");
    ERRT("BSET.L D7,-(A2)",         OPERAND_NOT_ALLOWED, "D7,-(A2)");
    TEST("BSET   D7,($1234,A2)",    0007752, 0x1234);
    TEST("BSET.B D7,($1234,A2)",    0007752, 0x1234);
    ERRT("BSET.W D7,($1234,A2)",    OPERAND_NOT_ALLOWED, "D7,($1234,A2)");
    ERRT("BSET.L D7,($1234,A2)",    OPERAND_NOT_ALLOWED, "D7,($1234,A2)");
    TEST("BSET   D7,($12,A2,D3.W)", 0007762, 0x3012);
    TEST("BSET.B D7,($12,A2,D3.W)", 0007762, 0x3012);
    ERRT("BSET.W D7,($12,A2,D3.W)", OPERAND_NOT_ALLOWED, "D7,($12,A2,D3.W)");
    ERRT("BSET.L D7,($12,A2,D3.W)", OPERAND_NOT_ALLOWED, "D7,($12,A2,D3.W)");
    TEST("BSET   D7,($001234).W",   0007770, 0x1234);
    TEST("BSET.B D7,($001234).W",   0007770, 0x1234);
    ERRT("BSET.W D7,($001234).W",   OPERAND_NOT_ALLOWED, "D7,($001234).W");
    ERRT("BSET.L D7,($001234).W",   OPERAND_NOT_ALLOWED, "D7,($001234).W");
    TEST("BSET   D7,($012345).L",   0007771, 0x0001, 0x2345);
    TEST("BSET.B D7,($012345).L",   0007771, 0x0001, 0x2345);
    ERRT("BSET.W D7,($012345).L",   OPERAND_NOT_ALLOWED, "D7,($012345).L");
    ERRT("BSET.L D7,($012345).L",   OPERAND_NOT_ALLOWED, "D7,($012345).L");
    ERRT("BSET   D7,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)", 0007700);
    ERRT("BSET.B D7,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)", 0007700);
    ERRT("BSET.W D7,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "D7,(*+$1234,PC)");
    ERRT("BSET.L D7,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "D7,(*+$1234,PC)");
    ERRT("BSET   D7,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0007700);
    ERRT("BSET.B D7,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0007700);
    ERRT("BSET.W D7,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "D7,(*+$12,PC,D3)");
    ERRT("BSET.L D7,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "D7,(*+$12,PC,D3)");
    ERRT("BSET   D7,#$1234",        OPERAND_NOT_ALLOWED, "#$1234", 0007700);
    ERRT("BSET.B D7,#$1234",        OPERAND_NOT_ALLOWED, "#$1234", 0007700);
    ERRT("BSET.W D7,#$1234",        OPERAND_NOT_ALLOWED, "D7,#$1234");
    ERRT("BSET.L D7,#$1234",        OPERAND_NOT_ALLOWED, "D7,#$1234");

    // BSET #n,dst: 00043|M|Rn
    TEST("BSET   #0,D2",            0004302, 0x0000);
    ERRT("BSET.B #0,D2",            OPERAND_NOT_ALLOWED, "D2", 0004300);
    ERRT("BSET.W #0,D2",            OPERAND_NOT_ALLOWED, "#0,D2");
    TEST("BSET.L #0,D2",            0004302, 0x0000);
    ERRT("BSET   #1,A2",            OPERAND_NOT_ALLOWED, "A2", 0004300);
    ERRT("BSET.B #1,A2",            OPERAND_NOT_ALLOWED, "A2", 0004300);
    ERRT("BSET.W #1,A2",            OPERAND_NOT_ALLOWED, "#1,A2");
    ERRT("BSET.L #1,A2",            OPERAND_NOT_ALLOWED, "#1,A2");
    TEST("BSET   #7,(A2)",          0004322, 0x0007);
    TEST("BSET.B #7,(A2)",          0004322, 0x0007);
    ERRT("BSET.W #7,(A2)",          OPERAND_NOT_ALLOWED, "#7,(A2)");
    ERRT("BSET.L #7,(A2)",          OPERAND_NOT_ALLOWED, "#7,(A2)");
    TEST("BSET   #6,(A2)+",         0004332, 0x0006);
    TEST("BSET.B #6,(A2)+",         0004332, 0x0006);
    ERRT("BSET.W #6,(A2)+",         OPERAND_NOT_ALLOWED, "#6,(A2)+");
    ERRT("BSET.L #6,(A2)+",         OPERAND_NOT_ALLOWED, "#6,(A2)+");
    TEST("BSET   #5,-(A2)",         0004342, 0x0005);
    TEST("BSET.B #5,-(A2)",         0004342, 0x0005);
    ERRT("BSET.W #5,-(A2)",         OPERAND_NOT_ALLOWED, "#5,-(A2)");
    ERRT("BSET.L #5,-(A2)",         OPERAND_NOT_ALLOWED, "#5,-(A2)");
    TEST("BSET   #4,($1234,A2)",    0004352, 0x0004, 0x1234);
    TEST("BSET.B #4,($1234,A2)",    0004352, 0x0004, 0x1234);
    ERRT("BSET.W #4,($1234,A2)",    OPERAND_NOT_ALLOWED, "#4,($1234,A2)");
    ERRT("BSET.L #4,($1234,A2)",    OPERAND_NOT_ALLOWED, "#4,($1234,A2)");
    TEST("BSET   #3,($12,A2,D3.W)", 0004362, 0x0003, 0x3012);
    TEST("BSET.B #3,($12,A2,D3.W)", 0004362, 0x0003, 0x3012);
    ERRT("BSET.W #3,($12,A2,D3.W)", OPERAND_NOT_ALLOWED, "#3,($12,A2,D3.W)");
    ERRT("BSET.L #3,($12,A2,D3.W)", OPERAND_NOT_ALLOWED, "#3,($12,A2,D3.W)");
    TEST("BSET   #2,($001234).W",   0004370, 0x0002, 0x1234);
    TEST("BSET.B #2,($001234).W",   0004370, 0x0002, 0x1234);
    ERRT("BSET.W #2,($001234).W",   OPERAND_NOT_ALLOWED, "#2,($001234).W");
    ERRT("BSET.L #2,($001234).W",   OPERAND_NOT_ALLOWED, "#2,($001234).W");
    TEST("BSET   #1,($012345).L",   0004371, 0x0001, 0x0001, 0x2345);
    TEST("BSET.B #1,($012345).L",   0004371, 0x0001, 0x0001, 0x2345);
    ERRT("BSET.W #1,($012345).L",   OPERAND_NOT_ALLOWED, "#1,($012345).L");
    ERRT("BSET.L #1,($012345).L",   OPERAND_NOT_ALLOWED, "#1,($012345).L");
    ERRT("BSET   #3,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)", 0004300);
    ERRT("BSET.B #3,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)", 0004300);
    ERRT("BSET.W #3,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "#3,(*+$1234,PC)");
    ERRT("BSET.L #3,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "#3,(*+$1234,PC)");
    ERRT("BSET   #4,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0004300);
    ERRT("BSET.B #4,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0004300);
    ERRT("BSET.W #4,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "#4,(*+$12,PC,D3)");
    ERRT("BSET.L #4,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "#4,(*+$12,PC,D3)");
    ERRT("BSET   #5,#$1234",        OPERAND_NOT_ALLOWED, "#$1234", 0004300);
    ERRT("BSET.B #5,#$1234",        OPERAND_NOT_ALLOWED, "#$1234", 0004300);
    ERRT("BSET.W #5,#$1234",        OPERAND_NOT_ALLOWED, "#5,#$1234");
    ERRT("BSET.L #5,#$1234",        OPERAND_NOT_ALLOWED, "#5,#$1234");

    // BTST Dx,dst: 000|Dx|4|M|Rn
    TEST("BTST   D7,D2",              0007402);
    ERRT("BTST.B D7,D2",              OPERAND_NOT_ALLOWED, "D2", 0007400);
    ERRT("BTST.W D7,D2",              OPERAND_NOT_ALLOWED, "D7,D2");
    TEST("BTST.L D7,D2",              0007402);
    ERRT("BTST   D7,A2",              OPERAND_NOT_ALLOWED, "A2", 0007400);
    ERRT("BTST.B D7,A2",              OPERAND_NOT_ALLOWED, "A2", 0007400);
    ERRT("BTST.W D7,A2",              OPERAND_NOT_ALLOWED, "D7,A2");
    ERRT("BTST.L D7,A2",              OPERAND_NOT_ALLOWED, "D7,A2");
    TEST("BTST   D7,(A2)",            0007422);
    TEST("BTST.B D7,(A2)",            0007422);
    ERRT("BTST.W D7,(A2)",            OPERAND_NOT_ALLOWED, "D7,(A2)");
    ERRT("BTST.L D7,(A2)",            OPERAND_NOT_ALLOWED, "D7,(A2)");
    TEST("BTST   D7,(A2)+",           0007432);
    TEST("BTST.B D7,(A2)+",           0007432);
    ERRT("BTST.W D7,(A2)+",           OPERAND_NOT_ALLOWED, "D7,(A2)+");
    ERRT("BTST.L D7,(A2)+",           OPERAND_NOT_ALLOWED, "D7,(A2)+");
    TEST("BTST   D7,-(A2)",           0007442);
    TEST("BTST.B D7,-(A2)",           0007442);
    ERRT("BTST.W D7,-(A2)",           OPERAND_NOT_ALLOWED, "D7,-(A2)");
    ERRT("BTST.L D7,-(A2)",           OPERAND_NOT_ALLOWED, "D7,-(A2)");
    TEST("BTST   D7,($1234,A2)",      0007452, 0x1234);
    TEST("BTST.B D7,($1234,A2)",      0007452, 0x1234);
    ERRT("BTST.W D7,($1234,A2)",      OPERAND_NOT_ALLOWED, "D7,($1234,A2)");
    ERRT("BTST.L D7,($1234,A2)",      OPERAND_NOT_ALLOWED, "D7,($1234,A2)");
    TEST("BTST   D7,($12,A2,D3.W)",   0007462, 0x3012);
    TEST("BTST.B D7,($12,A2,D3.W)",   0007462, 0x3012);
    ERRT("BTST.W D7,($12,A2,D3.W)",   OPERAND_NOT_ALLOWED, "D7,($12,A2,D3.W)");
    ERRT("BTST.L D7,($12,A2,D3.W)",   OPERAND_NOT_ALLOWED, "D7,($12,A2,D3.W)");
    TEST("BTST   D7,($001234).W",     0007470, 0x1234);
    TEST("BTST.B D7,($001234).W",     0007470, 0x1234);
    ERRT("BTST.W D7,($001234).W",     OPERAND_NOT_ALLOWED, "D7,($001234).W");
    ERRT("BTST.L D7,($001234).W",     OPERAND_NOT_ALLOWED, "D7,($001234).W");
    TEST("BTST   D7,($012345).L",     0007471, 0x0001, 0x2345);
    TEST("BTST.B D7,($012345).L",     0007471, 0x0001, 0x2345);
    ERRT("BTST.W D7,($012345).L",     OPERAND_NOT_ALLOWED, "D7,($012345).L");
    ERRT("BTST.L D7,($012345).L",     OPERAND_NOT_ALLOWED, "D7,($012345).L");
    TEST("BTST   D7,(*+$1234,PC)",    0007472, 0x1232);
    TEST("BTST.B D7,(*+$1234,PC)",    0007472, 0x1232);
    ERRT("BTST.W D7,(*+$1234,PC)",    OPERAND_NOT_ALLOWED, "D7,(*+$1234,PC)");
    ERRT("BTST.L D7,(*+$1234,PC)",    OPERAND_NOT_ALLOWED, "D7,(*+$1234,PC)");
    TEST("BTST   D7,(*+$23,PC,D3.L)", 0007473, 0x3821);
    TEST("BTST.B D7,(*+$23,PC,D3.L)", 0007473, 0x3821);
    ERRT("BTST.W D7,(*+$23,PC,D3.L)", OPERAND_NOT_ALLOWED, "D7,(*+$23,PC,D3.L)");
    ERRT("BTST.L D7,(*+$23,PC,D3.L)", OPERAND_NOT_ALLOWED, "D7,(*+$23,PC,D3.L)");
    ERRT("BTST   D7,#$12",            OPERAND_NOT_ALLOWED, "#$12", 0007400);
    ERRT("BTST.B D7,#$12",            OPERAND_NOT_ALLOWED, "#$12", 0007400);
    ERRT("BTST.W D7,#$12",            OPERAND_NOT_ALLOWED, "D7,#$12");
    ERRT("BTST.L D7,#$12",            OPERAND_NOT_ALLOWED, "D7,#$12");

    // BTST #n,dst: 00040|M|Rn
    TEST("BTST   #0,D2",              0004002, 0x0000);
    ERRT("BTST.B #0,D2",              OPERAND_NOT_ALLOWED, "D2", 0004000);
    ERRT("BTST.W #0,D2",              OPERAND_NOT_ALLOWED, "#0,D2");
    TEST("BTST.L #0,D2",              0004002, 0x0000);
    ERRT("BTST   #1,A2",              OPERAND_NOT_ALLOWED, "A2", 0004000);
    ERRT("BTST.B #1,A2",              OPERAND_NOT_ALLOWED, "A2", 0004000);
    ERRT("BTST.W #1,A2",              OPERAND_NOT_ALLOWED, "#1,A2");
    ERRT("BTST.B #1,A2",              OPERAND_NOT_ALLOWED, "A2", 0004000);
    TEST("BTST   #7,(A2)",            0004022, 0x0007);
    TEST("BTST.B #7,(A2)",            0004022, 0x0007);
    ERRT("BTST.W #7,(A2)",            OPERAND_NOT_ALLOWED, "#7,(A2)");
    ERRT("BTST.L #7,(A2)",            OPERAND_NOT_ALLOWED, "#7,(A2)");
    TEST("BTST   #6,(A2)+",           0004032, 0x0006);
    TEST("BTST.B #6,(A2)+",           0004032, 0x0006);
    ERRT("BTST.W #6,(A2)+",           OPERAND_NOT_ALLOWED, "#6,(A2)+");
    ERRT("BTST.L #6,(A2)+",           OPERAND_NOT_ALLOWED, "#6,(A2)+");
    TEST("BTST   #5,-(A2)",           0004042, 0x0005);
    TEST("BTST.B #5,-(A2)",           0004042, 0x0005);
    ERRT("BTST.W #5,-(A2)",           OPERAND_NOT_ALLOWED, "#5,-(A2)");
    ERRT("BTST.L #5,-(A2)",           OPERAND_NOT_ALLOWED, "#5,-(A2)");
    TEST("BTST   #4,($1234,A2)",      0004052, 0x0004, 0x1234);
    TEST("BTST.B #4,($1234,A2)",      0004052, 0x0004, 0x1234);
    ERRT("BTST.W #4,($1234,A2)",      OPERAND_NOT_ALLOWED, "#4,($1234,A2)");
    ERRT("BTST.L #4,($1234,A2)",      OPERAND_NOT_ALLOWED, "#4,($1234,A2)");
    TEST("BTST   #3,($12,A2,D3.W)",   0004062, 0x0003, 0x3012);
    TEST("BTST.B #3,($12,A2,D3.W)",   0004062, 0x0003, 0x3012);
    ERRT("BTST.W #3,($12,A2,D3.W)",   OPERAND_NOT_ALLOWED, "#3,($12,A2,D3.W)");
    ERRT("BTST.L #3,($12,A2,D3.W)",   OPERAND_NOT_ALLOWED, "#3,($12,A2,D3.W)");
    TEST("BTST   #2,($001234).W",     0004070, 0x0002, 0x1234);
    TEST("BTST.B #2,($001234).W",     0004070, 0x0002, 0x1234);
    ERRT("BTST.W #2,($001234).W",     OPERAND_NOT_ALLOWED, "#2,($001234).W");
    ERRT("BTST.L #2,($001234).W",     OPERAND_NOT_ALLOWED, "#2,($001234).W");
    TEST("BTST   #1,($012345).L",     0004071, 0x0001, 0x0001, 0x2345);
    TEST("BTST.B #1,($012345).L",     0004071, 0x0001, 0x0001, 0x2345);
    ERRT("BTST.W #1,($012345).L",     OPERAND_NOT_ALLOWED, "#1,($012345).L");
    ERRT("BTST.L #1,($012345).L",     OPERAND_NOT_ALLOWED, "#1,($012345).L");
    TEST("BTST   #0,(*+$1234,PC)",    0004072, 0x0000, 0x1230);
    TEST("BTST.B #0,(*+$1234,PC)",    0004072, 0x0000, 0x1230);
    ERRT("BTST.W #0,(*+$1234,PC)",    OPERAND_NOT_ALLOWED, "#0,(*+$1234,PC)");
    ERRT("BTST.L #0,(*+$1234,PC)",    OPERAND_NOT_ALLOWED, "#0,(*+$1234,PC)");
    TEST("BTST   #7,(*+$23,PC,D3.L)", 0004073, 0x0007, 0x381F);
    TEST("BTST.B #7,(*+$23,PC,D3.L)", 0004073, 0x0007, 0x381F);
    ERRT("BTST.W #7,(*+$23,PC,D3.L)", OPERAND_NOT_ALLOWED, "#7,(*+$23,PC,D3.L)");
    ERRT("BTST.L #7,(*+$23,PC,D3.L)", OPERAND_NOT_ALLOWED, "#7,(*+$23,PC,D3.L)");
    ERRT("BTST   #6,#$1234",          OPERAND_NOT_ALLOWED, "#$1234", 0004000);
    ERRT("BTST.B #6,#$1234",          OPERAND_NOT_ALLOWED, "#$1234", 0004000);
    ERRT("BTST.W #6,#$1234",          OPERAND_NOT_ALLOWED, "#6,#$1234");
    ERRT("BTST.L #6,#$1234",          OPERAND_NOT_ALLOWED, "#6,#$1234");
}

void test_bcd() {
    // ABCD Dx,Dy: 014|Dy|40|Dx
    TEST("ABCD D2,D7", 0147402);

    // ABCD -(Ax),-(Ay)" 014|Ay|41|Ax
    TEST("ABCD -(A2),-(A7)", 0147412);

    // NBCD dst: 00440|M|Rn
    TEST("NBCD D2",            0044002);
    ERRT("NBCD A2",            OPERAND_NOT_ALLOWED, "A2", 0044000);
    TEST("NBCD (A2)",          0044022);
    TEST("NBCD (A2)+",         0044032);
    TEST("NBCD -(A2)",         0044042);
    TEST("NBCD ($1234,A2)",    0044052, 0x1234);
    TEST("NBCD ($12,A2,D3.W)", 0044062, 0x3012);
    TEST("NBCD ($001234).W",   0044070, 0x1234);
    TEST("NBCD ($123456).L",   0044071, 0x0012, 0x3456);
    ERRT("NBCD (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0044000);
    ERRT("NBCD (*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0044000);
    ERRT("NBCD #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0044000);

    // SBCD Dx,Dy: 010|Dy|40|Dx
    TEST("SBCD D2,D7", 0107402);

    // SBCD -(Ax),-(Ay): 010|Ay|41|Ax
    TEST("SBCD -(A2),-(A7)", 0107412);


    if (!firstGen()) {
        // PACK Dn,Dm, #nn:       010|Dm|50|Dn
        // PACK -(An),-(Am), #nn: 010|Am|51|An
        TEST("PACK D2, D7, #$1234",       0107502, 0x1234);
        TEST("PACK -(A2), -(A7), #$1234", 0107512, 0x1234);

        // UNPK Dn,Dm,#nn;       010|Dm|60|Dn
        // UNPK -(An),-(Am),#nn: 010|Am|61|An
        TEST("UNPK D2, D7, #$1234",       0107602, 0x1234);
        TEST("UNPK -(A2), -(A7), #$1234", 0107612, 0x1234);
    }
}

void test_program() {
    // Bcc label: 006|cc|disp
    TEST("BRA *", 0060000 | 0x000 | 0xFE);
    TEST("BSR *", 0060000 | 0x100 | 0xFE);
    TEST("BHI *", 0060000 | 0x200 | 0xFE);
    TEST("BLS *", 0060000 | 0x300 | 0xFE);
    TEST("BCC *", 0060000 | 0x400 | 0xFE);
    TEST("BCS *", 0060000 | 0x500 | 0xFE);
    TEST("BNE *", 0060000 | 0x600 | 0xFE);
    TEST("BEQ *", 0060000 | 0x700 | 0xFE);
    TEST("BVC *", 0060000 | 0x800 | 0xFE);
    TEST("BVS *", 0060000 | 0x900 | 0xFE);
    TEST("BPL *", 0060000 | 0xA00 | 0xFE);
    TEST("BMI *", 0060000 | 0xB00 | 0xFE);
    TEST("BGE *", 0060000 | 0xC00 | 0xFE);
    TEST("BLT *", 0060000 | 0xD00 | 0xFE);
    TEST("BGT *", 0060000 | 0xE00 | 0xFE);
    ATEST(0x010000, "BLE *+2",     0060000 | 0xF00, 0x0000);
    ATEST(0x010000, "BLE *-$7E",   0060000 | 0xF00 | 0x80);
    ATEST(0x010000, "BLE *+$80",   0060000 | 0xF00 | 0x7E);
    ATEST(0x010000, "BLE.W *+2",   0060000 | 0xF00, 0x0000);
    ATEST(0x010000, "BLE.W *-$7E", 0060000 | 0xF00, 0xFF80);
    ATEST(0x010000, "BLE.L *+2",   0060000 | 0xF00, 0x0000);
    ATEST(0x010000, "BLE.L *+$80", 0060000 | 0xF00, 0x007E);
    ATEST(0x010000, "BLE *-$80",   0060000 | 0xF00, 0xFF7E);
    ATEST(0x010000, "BLE *+$82",   0060000 | 0xF00, 0x0080);
    AERRT(0x010000, "BLE.B *-$80", OPERAND_TOO_FAR, "*-$80", 0060000 | 0xF00 | 0x7E);
    AERRT(0x010000, "BLE.S *+$82", OPERAND_TOO_FAR, "*+$82", 0060000 | 0xF00 | 0x80);
    if (firstGen()) {
        AERRT(0x010000, "BLE *+1",     OPERAND_NOT_ALIGNED, "*+1", 0060000 | 0xF00 | 0xFF);
        AERRT(0x010000, "BLE.X *+$80", ILLEGAL_SIZE, "BLE.X *+$80", 0060000 | 0xF00 | 0x7E);
        AERRT(0x010000, "BLE *-$8000", OPERAND_TOO_FAR, "*-$8000", 0060000 | 0xF00 | 0xFE);
        AERRT(0x010000, "BLE *+$8002", OPERAND_TOO_FAR, "*+$8002", 0060000 | 0xF00 | 0x00);
    } else {
        AERRT(0x010000, "BLE *+1",     OPERAND_NOT_ALIGNED, "*+1", 0060000 | 0xF00, 0x0000);
        ATEST(0x010000, "BLE.X *+2",   0060000 | 0xF00 | 0xFF, 0x0000, 0x0000);
        ATEST(0x010000, "BLE.X *+$80", 0060000 | 0xF00 | 0xFF, 0x0000, 0x007E);
        ATEST(0x010000, "BLE *-$8000", 0060000 | 0xF00 | 0xFF, 0xFFFF, 0x7FFE);
        ATEST(0x010000, "BLE *+$8002", 0060000 | 0xF00 | 0xFF, 0x0000, 0x8000);
    }
    AERRT(0x001000, "BLE *-$1002", OVERFLOW_RANGE,  "*-$1002", 0060000 | 0xF00, 0xEFFC);
    if (firstGen()) {
        AERRT(0xFFF000, "BLE *+$1000", OVERFLOW_RANGE,  "*+$1000", 0060000 | 0xF00, 0x0FFE);
    } else {
        ATEST(0xFFF000, "BLE *+$1000", 0060000 | 0xF00, 0x0FFE);
    }

    // DBcc Dn,labelL 005|cc|31|Dn
    ATEST(0x010000, "DBRA   D2,*-$7FFE", 0050312 | 0x100, 0x8000);
    ATEST(0x010000, "DBRA.W D2,*-$007E", 0050312 | 0x100, 0xFF80);
    ATEST(0x010000, "DBRA.L D2,*",       0050312 | 0x100, 0xFFFE);
    ATEST(0x010000, "DBRA   D2,*+2",     0050312 | 0x100, 0x0000);
    ATEST(0x010000, "DBRA   D2,*+$0080", 0050312 | 0x100, 0x007E);
    ATEST(0x010000, "DBRA   D2,*+$8000", 0050312 | 0x100, 0x7FFE);
    AERRT(0x010000, "DBRA   D2,*-$8000", OPERAND_TOO_FAR, "*-$8000", 0050312 | 0x100, 0x7FFE);
    AERRT(0x010000, "DBRA   D2,*+$8002", OPERAND_TOO_FAR, "*+$8002", 0050312 | 0x100, 0x8000);
    AERRT(0x001000, "DBRA   D2,*-$1002", OVERFLOW_RANGE,  "*-$1002", 0050312 | 0x100, 0xEFFC);
    if (firstGen()) {
        AERRT(0xFFF000, "DBRA   D2,*+$1000", OVERFLOW_RANGE,  "*+$1000", 0050312 | 0x100, 0x0FFE);
    } else {
        ATEST(0xFFF000, "DBRA   D2,*+$1000", 0050312 | 0x100, 0x0FFE);
    }
    AERRT(0x010000, "DBRA   D2,*+3",     OPERAND_NOT_ALIGNED, "*+3", 0050312 | 0x100, 0x0001);
    AERRT(0x010000, "DBRA   D2,*-3",     OPERAND_NOT_ALIGNED, "*-3", 0050312 | 0x100, 0xFFFB);
    AERRT(0x010000, "DBRA.B D2,*-2",  ILLEGAL_SIZE, "DBRA.B D2,*-2", 0050312 | 0x100, 0xFFFC);
    AERRT(0x010000, "DBRA.S D2,*-2",  ILLEGAL_SIZE, "DBRA.S D2,*-2", 0050312 | 0x100, 0xFFFC);
    AERRT(0x010000, "DBRA.X D2,*-2",  ILLEGAL_SIZE, "DBRA.X D2,*-2", 0050312 | 0x100, 0xFFFC);
    TEST("DBT  D2,*", 0050312 | 0x000, 0xFFFE);
    TEST("DBHI D2,*", 0050312 | 0x200, 0xFFFE);
    TEST("DBLS D2,*", 0050312 | 0x300, 0xFFFE);
    TEST("DBCC D2,*", 0050312 | 0x400, 0xFFFE);
    TEST("DBCS D2,*", 0050312 | 0x500, 0xFFFE);
    TEST("DBNE D2,*", 0050312 | 0x600, 0xFFFE);
    TEST("DBEQ D2,*", 0050312 | 0x700, 0xFFFE);
    TEST("DBVC D2,*", 0050312 | 0x800, 0xFFFE);
    TEST("DBVS D2,*", 0050312 | 0x900, 0xFFFE);
    TEST("DBPL D2,*", 0050312 | 0xA00, 0xFFFE);
    TEST("DBMI D2,*", 0050312 | 0xB00, 0xFFFE);
    TEST("DBGE D2,*", 0050312 | 0xC00, 0xFFFE);
    TEST("DBLT D2,*", 0050312 | 0xD00, 0xFFFE);
    TEST("DBGT D2,*", 0050312 | 0xE00, 0xFFFE);
    TEST("DBLE D2,*", 0050312 | 0xF00, 0xFFFE);

    // Scc dst: 005|cc|3|M|Rn
    TEST("ST D2",            0050302 | 0x000);
    ERRT("ST A2",            OPERAND_NOT_ALLOWED, "A2", 0050300);
    TEST("ST (A2)",          0050322 | 0x000);
    TEST("ST (A2)+",         0050332 | 0x000);
    TEST("ST -(A2)",         0050342 | 0x000);
    TEST("ST ($1234,A2)",    0050352 | 0x000, 0x1234);
    TEST("ST ($12,A2,D3.W)", 0050362 | 0x000, 0x3012);
    TEST("ST ($001234).W",   0050370 | 0x000, 0x1234);
    TEST("ST ($123456).L",   0050371 | 0x000, 0x0012, 0x3456);
    ERRT("ST (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0050300);
    ERRT("ST (*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0050300);
    ERRT("ST #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0050300);
    TEST("SF D2",            0050302 | 0x100);
    ERRT("SF A2",            OPERAND_NOT_ALLOWED, "A2", 0050700);
    TEST("SF (A2)",          0050322 | 0x100);
    TEST("SF (A2)+",         0050332 | 0x100);
    TEST("SF -(A2)",         0050342 | 0x100);
    TEST("SF ($1234,A2)",    0050352 | 0x100, 0x1234);
    TEST("SF ($12,A2,D3.W)", 0050362 | 0x100, 0x3012);
    TEST("SF ($001234).W",   0050370 | 0x100, 0x1234);
    TEST("SF ($123456).L",   0050371 | 0x100, 0x0012, 0x3456);
    ERRT("SF (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0050700);
    ERRT("SF (*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0050700);
    ERRT("SF #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0050700);
    TEST("SHI D2",            0050302 | 0x200);
    ERRT("SHI A2",            OPERAND_NOT_ALLOWED, "A2", 0051300);
    TEST("SHI (A2)",          0050322 | 0x200);
    TEST("SHI (A2)+",         0050332 | 0x200);
    TEST("SHI -(A2)",         0050342 | 0x200);
    TEST("SHI ($1234,A2)",    0050352 | 0x200, 0x1234);
    TEST("SHI ($12,A2,D3.W)", 0050362 | 0x200, 0x3012);
    TEST("SHI ($001234).W",   0050370 | 0x200, 0x1234);
    TEST("SHI ($123456).L",   0050371 | 0x200, 0x0012, 0x3456);
    ERRT("SHI (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0051300);
    ERRT("SHI (*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0051300);
    ERRT("SHI #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0051300);
    TEST("SLS D2",            0050302 | 0x300);
    ERRT("SLS A2",            OPERAND_NOT_ALLOWED, "A2", 0051700);
    TEST("SLS (A2)",          0050322 | 0x300);
    TEST("SLS (A2)+",         0050332 | 0x300);
    TEST("SLS -(A2)",         0050342 | 0x300);
    TEST("SLS ($1234,A2)",    0050352 | 0x300, 0x1234);
    TEST("SLS ($12,A2,D3.W)", 0050362 | 0x300, 0x3012);
    TEST("SLS ($001234).W",   0050370 | 0x300, 0x1234);
    TEST("SLS ($123456).L",   0050371 | 0x300, 0x0012, 0x3456);
    ERRT("SLS (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0051700);
    ERRT("SLS (*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0051700);
    ERRT("SLS #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0051700);
    TEST("SCC D2",            0050302 | 0x400);
    ERRT("SCC A2",            OPERAND_NOT_ALLOWED, "A2", 0052300);
    TEST("SCC (A2)",          0050322 | 0x400);
    TEST("SCC (A2)+",         0050332 | 0x400);
    TEST("SCC -(A2)",         0050342 | 0x400);
    TEST("SCC ($1234,A2)",    0050352 | 0x400, 0x1234);
    TEST("SCC ($12,A2,D3.W)", 0050362 | 0x400, 0x3012);
    TEST("SCC ($001234).W",   0050370 | 0x400, 0x1234);
    TEST("SCC ($123456).L",   0050371 | 0x400, 0x0012, 0x3456);
    ERRT("SCC (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0052300);
    ERRT("SCC (*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0052300);
    ERRT("SCC #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0052300);
    TEST("SCS D2",            0050302 | 0x500);
    ERRT("SCS A2",            OPERAND_NOT_ALLOWED, "A2", 0052700);
    TEST("SCS (A2)",          0050322 | 0x500);
    TEST("SCS (A2)+",         0050332 | 0x500);
    TEST("SCS -(A2)",         0050342 | 0x500);
    TEST("SCS ($1234,A2)",    0050352 | 0x500, 0x1234);
    TEST("SCS ($12,A2,D3.W)", 0050362 | 0x500, 0x3012);
    TEST("SCS ($001234).W",   0050370 | 0x500, 0x1234);
    TEST("SCS ($123456).L",   0050371 | 0x500, 0x0012, 0x3456);
    ERRT("SCS (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0052700);
    ERRT("SCS (*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0052700);
    ERRT("SCS #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0052700);
    TEST("SNE D2",            0050302 | 0x600);
    ERRT("SNE A2",            OPERAND_NOT_ALLOWED, "A2", 0053300);
    TEST("SNE (A2)",          0050322 | 0x600);
    TEST("SNE (A2)+",         0050332 | 0x600);
    TEST("SNE -(A2)",         0050342 | 0x600);
    TEST("SNE ($1234,A2)",    0050352 | 0x600, 0x1234);
    TEST("SNE ($12,A2,D3.W)", 0050362 | 0x600, 0x3012);
    TEST("SNE ($001234).W",   0050370 | 0x600, 0x1234);
    TEST("SNE ($123456).L",   0050371 | 0x600, 0x0012, 0x3456);
    ERRT("SNE (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0053300);
    ERRT("SNE (*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0053300);
    ERRT("SNE #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0053300);
    TEST("SEQ D2",            0050302 | 0x700);
    ERRT("SEQ A2",            OPERAND_NOT_ALLOWED, "A2", 0053700);
    TEST("SEQ (A2)",          0050322 | 0x700);
    TEST("SEQ (A2)+",         0050332 | 0x700);
    TEST("SEQ -(A2)",         0050342 | 0x700);
    TEST("SEQ ($1234,A2)",    0050352 | 0x700, 0x1234);
    TEST("SEQ ($12,A2,D3.W)", 0050362 | 0x700, 0x3012);
    TEST("SEQ ($001234).W",   0050370 | 0x700, 0x1234);
    TEST("SEQ ($123456).L",   0050371 | 0x700, 0x0012, 0x3456);
    ERRT("SEQ (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0053700);
    ERRT("SEQ (*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0053700);
    ERRT("SEQ #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0053700);
    TEST("SVC D2",            0050302 | 0x800);
    ERRT("SVC A2",            OPERAND_NOT_ALLOWED, "A2", 0054300);
    TEST("SVC (A2)",          0050322 | 0x800);
    TEST("SVC (A2)+",         0050332 | 0x800);
    TEST("SVC -(A2)",         0050342 | 0x800);
    TEST("SVC ($1234,A2)",    0050352 | 0x800, 0x1234);
    TEST("SVC ($12,A2,D3.W)", 0050362 | 0x800, 0x3012);
    TEST("SVC ($001234).W",   0050370 | 0x800, 0x1234);
    TEST("SVC ($123456).L",   0050371 | 0x800, 0x0012, 0x3456);
    ERRT("SVC (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0054300);
    ERRT("SVC (*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0054300);
    ERRT("SVC #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0054300);
    TEST("SVS D2",            0050302 | 0x900);
    ERRT("SVS A2",            OPERAND_NOT_ALLOWED, "A2", 0054700);
    TEST("SVS (A2)",          0050322 | 0x900);
    TEST("SVS (A2)+",         0050332 | 0x900);
    TEST("SVS -(A2)",         0050342 | 0x900);
    TEST("SVS ($1234,A2)",    0050352 | 0x900, 0x1234);
    TEST("SVS ($12,A2,D3.W)", 0050362 | 0x900, 0x3012);
    TEST("SVS ($001234).W",   0050370 | 0x900, 0x1234);
    TEST("SVS ($123456).L",   0050371 | 0x900, 0x0012, 0x3456);
    ERRT("SVS (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0054700);
    ERRT("SVS (*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0054700);
    ERRT("SVS #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0054700);
    TEST("SPL D2",            0050302 | 0xA00);
    ERRT("SPL A2",            OPERAND_NOT_ALLOWED, "A2", 0055300);
    TEST("SPL (A2)",          0050322 | 0xA00);
    TEST("SPL (A2)+",         0050332 | 0xA00);
    TEST("SPL -(A2)",         0050342 | 0xA00);
    TEST("SPL ($1234,A2)",    0050352 | 0xA00, 0x1234);
    TEST("SPL ($12,A2,D3.W)", 0050362 | 0xA00, 0x3012);
    TEST("SPL ($001234).W",   0050370 | 0xA00, 0x1234);
    TEST("SPL ($123456).L",   0050371 | 0xA00, 0x0012, 0x3456);
    ERRT("SPL (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0055300);
    ERRT("SPL (*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0055300);
    ERRT("SPL #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0055300);
    TEST("SMI D2",            0050302 | 0xB00);
    ERRT("SMI A2",            OPERAND_NOT_ALLOWED, "A2", 0055700);
    TEST("SMI (A2)",          0050322 | 0xB00);
    TEST("SMI (A2)+",         0050332 | 0xB00);
    TEST("SMI -(A2)",         0050342 | 0xB00);
    TEST("SMI ($1234,A2)",    0050352 | 0xB00, 0x1234);
    TEST("SMI ($12,A2,D3.W)", 0050362 | 0xB00, 0x3012);
    TEST("SMI ($001234).W",   0050370 | 0xB00, 0x1234);
    TEST("SMI ($123456).L",   0050371 | 0xB00, 0x0012, 0x3456);
    ERRT("SMI (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0055700);
    ERRT("SMI (*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0055700);
    ERRT("SMI #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0055700);
    TEST("SGE D2",            0050302 | 0xC00);
    ERRT("SGE A2",            OPERAND_NOT_ALLOWED, "A2", 0056300);
    TEST("SGE (A2)",          0050322 | 0xC00);
    TEST("SGE (A2)+",         0050332 | 0xC00);
    TEST("SGE -(A2)",         0050342 | 0xC00);
    TEST("SGE ($1234,A2)",    0050352 | 0xC00, 0x1234);
    TEST("SGE ($12,A2,D3.W)", 0050362 | 0xC00, 0x3012);
    TEST("SGE ($001234).W",   0050370 | 0xC00, 0x1234);
    TEST("SGE ($123456).L",   0050371 | 0xC00, 0x0012, 0x3456);
    ERRT("SGE (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0056300);
    ERRT("SGE (*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0056300);
    ERRT("SGE #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0056300);
    TEST("SLT D2",            0050302 | 0xD00);
    ERRT("SLT A2",            OPERAND_NOT_ALLOWED, "A2", 0056700);
    TEST("SLT (A2)",          0050322 | 0xD00);
    TEST("SLT (A2)+",         0050332 | 0xD00);
    TEST("SLT -(A2)",         0050342 | 0xD00);
    TEST("SLT ($1234,A2)",    0050352 | 0xD00, 0x1234);
    TEST("SLT ($12,A2,D3.W)", 0050362 | 0xD00, 0x3012);
    TEST("SLT ($001234).W",   0050370 | 0xD00, 0x1234);
    TEST("SLT ($123456).L",   0050371 | 0xD00, 0x0012, 0x3456);
    ERRT("SLT (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0056700);
    ERRT("SLT (*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0056700);
    ERRT("SLT #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0056700);
    TEST("SGT D2",            0050302 | 0xE00);
    ERRT("SGT A2",            OPERAND_NOT_ALLOWED, "A2", 0057300);
    TEST("SGT (A2)",          0050322 | 0xE00);
    TEST("SGT (A2)+",         0050332 | 0xE00);
    TEST("SGT -(A2)",         0050342 | 0xE00);
    TEST("SGT ($1234,A2)",    0050352 | 0xE00, 0x1234);
    TEST("SGT ($12,A2,D3.W)", 0050362 | 0xE00, 0x3012);
    TEST("SGT ($001234).W",   0050370 | 0xE00, 0x1234);
    TEST("SGT ($123456).L",   0050371 | 0xE00, 0x0012, 0x3456);
    ERRT("SGT (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0057300);
    ERRT("SGT (*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0057300);
    ERRT("SGT #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0057300);
    TEST("SLE D2",            0050302 | 0xF00);
    ERRT("SLE A2",            OPERAND_NOT_ALLOWED, "A2", 0057700);
    TEST("SLE (A2)",          0050322 | 0xF00);
    TEST("SLE (A2)+",         0050332 | 0xF00);
    TEST("SLE -(A2)",         0050342 | 0xF00);
    TEST("SLE ($1234,A2)",    0050352 | 0xF00, 0x1234);
    TEST("SLE ($12,A2,D3.W)", 0050362 | 0xF00, 0x3012);
    TEST("SLE ($001234).W",   0050370 | 0xF00, 0x1234);
    TEST("SLE ($123456).L",   0050371 | 0xF00, 0x0012, 0x3456);
    ERRT("SLE (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0057700);
    ERRT("SLE (*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0057700);
    ERRT("SLE #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0057700);

    // BRA label: 00600|disp
    ATEST(0x10000, "BRA   *-$7FFE", 0060000, 0x8000);
    ATEST(0x10000, "BRA.W *-$007E", 0060000, 0xFF80);
    ATEST(0x10000, "BRA.L *-$007E", 0060000, 0xFF80);
    ATEST(0x10000, "BRA.B *-$7E",   0060000 | 0x80);
    ATEST(0x10000, "BRA.S *-$7E",   0060000 | 0x80);
    ATEST(0x10000, "BRA   *",       0060000 | 0xFE);
    ATEST(0x10000, "BRA.W *",       0060000, 0xFFFE);
    ATEST(0x10000, "BRA.W *+2",     0060000, 0x0000);
    ATEST(0x10000, "BRA   *+$80",   0060000 | 0x7E);
    ATEST(0x10000, "BRA.W *+$0080", 0060000, 0x007E);
    ATEST(0x10000, "BRA.L *+$0080", 0060000, 0x007E);
    ATEST(0x10000, "BRA   *+$8000", 0060000, 0x7FFE);
    AERRT(0x10000, "BRA *+3",       OPERAND_NOT_ALIGNED, "*+3", 0060000 | 0x01);
    AERRT(0x10000, "BRA *-3",       OPERAND_NOT_ALIGNED, "*-3", 0060000 | 0xFB);
    AERRT(0x10000, "BRA.B *-$80",   OPERAND_TOO_FAR,   "*-$80", 0060000 | 0x7E);
    AERRT(0x10000, "BRA.B *+$82",   OPERAND_TOO_FAR,   "*+$82", 0060000 | 0x80);
    AERRT(0x10000, "BRA.S *+$82",   OPERAND_TOO_FAR,   "*+$82", 0060000 | 0x80);
    if (firstGen()) {
        AERRT(0x10000, "BRA.X *+$82",  ILLEGAL_SIZE, "BRA.X *+$82", 0060000 | 0x80);
    } else {
        ATEST(0x10000, "BRA.X *+$82",  0060000 | 0xFF, 0x0000, 0x0080);
    }

    // BSR label: 00604|disp
    if (firstGen()) {
        AERRT(0x010000, "BSR *-$8000", OPERAND_TOO_FAR, "*-$8000", 0060400 | 0xFE);
    } else {
        ATEST(0x010000, "BSR *-$8000", 0060400 | 0xFF, 0xFFFF, 0x7FFE);
    }
    ATEST(0x010000, "BSR   *-$7FFE", 0060400, 0x8000);
    ATEST(0x010000, "BSR   *-$80",   0060400, 0xFF7E);
    ATEST(0x010000, "BSR.W *-$007E", 0060400, 0xFF80);
    ATEST(0x010000, "BSR   *-$7E",   0060400  | 0x80);
    ATEST(0x010000, "BSR   *",       0060400  | 0xFE);
    ATEST(0x010000, "BSR.W *",       0060400, 0xFFFE);
    ATEST(0x010000, "BSR.W *+2",     0060400, 0x0000);
    ATEST(0x010000, "BSR   *+2",     0060400  | 0x00, 0x0000);
    ATEST(0x010000, "BSR   *+$80",   0060400  | 0x7E);
    ATEST(0x010000, "BSR.W *+$80"  , 0060400, 0x007E);
    ATEST(0x010000, "BSR   *+$82",   0060400, 0x0080);
    ATEST(0x010000, "BSR.W *+$8000", 0060400, 0x7FFE);
    if (firstGen()) {
        AERRT(0x010000, "BSR *+$8002", OPERAND_TOO_FAR, "*+$8002", 0060400 | 0x00);
    } else {
        ATEST(0x010000, "BSR *+$8002", 0060400 | 0xFF, 0x0000, 0x8000);
    }
    AERRT(0x001000, "BSR   *-$1002", OVERFLOW_RANGE,  "*-$1002", 0060400, 0xEFFC);
    if (firstGen()) {
        AERRT(0xFFF000, "BSR *+$1000", OVERFLOW_RANGE,  "*+$1000", 0060400, 0x0FFE);
    } else {
        ATEST(0xFFF000, "BSR *+$1000", 0060400, 0x0FFE);
    }
    AERRT(0x010000, "BSR   *+3",     OPERAND_NOT_ALIGNED, "*+3", 0060400 | 0x01);
    AERRT(0x010000, "BSR   *-3",     OPERAND_NOT_ALIGNED, "*-3", 0060400 | 0xFB);

    // JMP dst: 00473|M|Rn
    ERRT("JMP D2",              OPERAND_NOT_ALLOWED, "D2", 0047300);
    ERRT("JMP A2",              OPERAND_NOT_ALLOWED, "A2", 0047300);
    TEST("JMP (A2)",            0047322);
    ERRT("JMP (A2)+",           OPERAND_NOT_ALLOWED, "(A2)+", 0047300);
    ERRT("JMP -(A2)",           OPERAND_NOT_ALLOWED, "-(A2)", 0047300);
    TEST("JMP ($1234,A2)",      0047352, 0x1234);
    TEST("JMP ($12,A2,D3.W)",   0047362, 0x3012);
    TEST("JMP ($001234).W",     0047370, 0x1234);
    TEST("JMP ($123456).L",     0047371, 0x0012, 0x3456);
    TEST("JMP (*+$1234,PC)",    0047372, 0x1232);
    TEST("JMP (*+$12,PC,D3.L)", 0047373, 0x3810);
    ERRT("JMP #$1234",          OPERAND_NOT_ALLOWED, "#$1234", 0047300);
    ERRT("JMP ($003455).W",     OPERAND_NOT_ALIGNED, "($003455).W",  0047370, 0x3455);
    ERRT("JMP ($123455).L",     OPERAND_NOT_ALIGNED, "($123455).L",  0047371, 0x0012, 0x3455);
    ERRT("JMP (*+$1235,PC)",    OPERAND_NOT_ALIGNED, "(*+$1235,PC)", 0047372, 0x1233);

    // JSR dst, 00472|M|Rn
    ERRT("JSR D2",              OPERAND_NOT_ALLOWED, "D2", 0047200);
    ERRT("JSR A2",              OPERAND_NOT_ALLOWED, "A2", 0047200);
    TEST("JSR (A2)",            0047222);
    ERRT("JSR (A2)+",           OPERAND_NOT_ALLOWED, "(A2)+", 0047200);
    ERRT("JSR -(A2)",           OPERAND_NOT_ALLOWED, "-(A2)", 0047200);
    TEST("JSR ($1234,A2)",      0047252, 0x1234);
    TEST("JSR ($12,A2,D3.W)",   0047262, 0x3012);
    TEST("JSR ($001234).W",     0047270, 0x1234);
    TEST("JSR ($123456).L",     0047271, 0x0012, 0x3456);
    TEST("JSR (*+$1234,PC)",    0047272, 0x1232);
    TEST("JSR (*+$12,PC,D3.L)", 0047273, 0x3810);
    ERRT("JSR #$1234",          OPERAND_NOT_ALLOWED, "#$1234", 0047200);
    ERRT("JSR ($003455).W",     OPERAND_NOT_ALIGNED, "($003455).W",  0047270, 0x3455);
    ERRT("JSR ($123455).L",     OPERAND_NOT_ALIGNED, "($123455).L",  0047271, 0x0012, 0x3455);
    ERRT("JSR (*+$1235,PC)",    OPERAND_NOT_ALIGNED, "(*+$1235,PC)", 0047272, 0x1233);

    if (mc68020()) {
        // RETM Dn:       00033|0|Dn
        // RETM An:       00033|1|An
        // CALLM #n8,dst: 00033|M|Rn
        TEST("RTM D2",           0003302);
        TEST("RTM A2",           0003312);
        TEST("CALLM #$FF, (A2)", 0003322, 0x00FF);
        ERRT("CALLM #-1, (A2)+",  OPERAND_NOT_ALLOWED, "(A2)+", 0003300);
        ERRT("CALLM #-1, -(A2)",  OPERAND_NOT_ALLOWED, "-(A2)", 0003300);
        TEST("CALLM #0, ($1234,A2)",     0003352, 0x0000, 0x1234);
        TEST("CALLM #1, (18,A2,D3.W)",   0003362, 0x0001, 0x3012);
        ABSW("CALLM #2, (@W).W", "FF00", 0003370, 0x0002, 0xFF00);
        TEST("CALLM #3, ($123456).L",    0003371, 0x0003, 0x0012, 0x3456);
        TEST("CALLM #4, (*+$1234,PC)",   0003372, 0x0004, 0x1230);
        TEST("CALLM #5, (*+18,PC,D3.L)", 0003373, 0x0005, 0x380E);
        ERRT("CALLM #5, #$1234", OPERAND_NOT_ALLOWED, "#$1234", 0003300);
    }

    // TST dst: 0045|Sz|M|Rn, Sz:B=0/W=1/L=2
    TEST("TST.B D2",             0045002);
    ERRT("TST.B A2",             OPERAND_NOT_ALLOWED, "A2", 0045000);
    TEST("TST.B (A2)",           0045022);
    TEST("TST.B (A2)+",          0045032);
    TEST("TST.B -(A2)",          0045042);
    TEST("TST.B ($1234,A2)",     0045052, 0x1234);
    TEST("TST.B ($12,A2,D3.W)",  0045062, 0x3012);
    TEST("TST.B ($001234).W",    0045070, 0x1234);
    TEST("TST.B ($123456).L",    0045071, 0x0012, 0x3456);
    ERRT("TST.B (*+$1234,PC)",   OPERAND_NOT_ALLOWED, "(*+$1234,PC)",   0045000);
    ERRT("TST.B (*-16,PC,D3.L)", OPERAND_NOT_ALLOWED, "(*-16,PC,D3.L)", 0045000);
    ERRT("TST.B #$12",           OPERAND_NOT_ALLOWED, "#$12",           0045000);
    TEST("TST.W D2",             0045102);
    ERRT("TST.W A2",             OPERAND_NOT_ALLOWED, "A2", 0045100);
    TEST("TST.W (A2)",           0045122);
    TEST("TST.W (A2)+",          0045132);
    TEST("TST.W -(A2)",          0045142);
    TEST("TST.W ($1234,A2)",     0045152, 0x1234);
    TEST("TST.W ($12,A2,D3.W)",  0045162, 0x3012);
    TEST("TST.W ($001234).W",    0045170, 0x1234);
    TEST("TST.W ($123456).L",    0045171, 0x0012, 0x3456);
    ERRT("TST.W (*+$1234,PC)",   OPERAND_NOT_ALLOWED, "(*+$1234,PC)",   0045100);
    ERRT("TST.W (*-16,PC,D3.L)", OPERAND_NOT_ALLOWED, "(*-16,PC,D3.L)", 0045100);
    ERRT("TST.W #$1234",         OPERAND_NOT_ALLOWED, "#$1234",         0045100);
    TEST("TST.L D2",             0045202);
    ERRT("TST.L A2",             OPERAND_NOT_ALLOWED, "A2", 0045200);
    TEST("TST.L (A2)",           0045222);
    TEST("TST.L (A2)+",          0045232);
    TEST("TST.L -(A2)",          0045242);
    TEST("TST.L ($1234,A2)",     0045252, 0x1234);
    TEST("TST.L ($12,A2,D3.W)",  0045262, 0x3012);
    TEST("TST.L ($001234).W",    0045270, 0x1234);
    TEST("TST.L ($123454).L",    0045271, 0x0012, 0x3454);
    ERRT("TST.L (*+$1234,PC)",   OPERAND_NOT_ALLOWED, "(*+$1234,PC)",   0045200);
    ERRT("TST.L (*-16,PC,D3.L)", OPERAND_NOT_ALLOWED, "(*-16,PC,D3.L)", 0045200);
    ERRT("TST.L #$1234",         OPERAND_NOT_ALLOWED, "#$1234",         0045200);
}

void test_system() {
    // ANDI #nn,SR
    TEST("andi #$1234,sr", 0001174, 0x1234);

    // EORI #nn,SR
    TEST("EORI #$1234,SR", 0005174, 0x1234);

    // MOVE src,SR: 00433|M|Rn
    TEST("MOVE D2,SR",             0043302);
    ERRT("MOVE A2,SR",             OPERAND_NOT_ALLOWED, "A2,SR", 0043300);
    TEST("MOVE (A2),SR",           0043322);
    TEST("MOVE (A2)+,SR",          0043332);
    TEST("MOVE -(A2),SR",          0043342);
    TEST("MOVE ($1234,A2),SR",     0043352, 0x1234);
    TEST("MOVE ($12,A2,D3.W),SR",  0043362, 0x3012);
    TEST("MOVE ($001234).W,SR",    0043370, 0x1234);
    TEST("MOVE ($234568).L,SR",    0043371, 0x0023, 0x4568);
    TEST("MOVE (*+$1234,PC),SR",   0043372, 0x1232);
    TEST("MOVE (*-16,PC,D3.L),SR", 0043373, 0x38EE);
    TEST("MOVE #$3456,SR",         0043374, 0x3456);
    ERRT("MOVE ($001235).W,SR",    OPERAND_NOT_ALIGNED, "($001235).W,SR",  0043370, 0x1235);
    ERRT("MOVE ($234567).L,SR",    OPERAND_NOT_ALIGNED, "($234567).L,SR",  0043371, 0x0023, 0x4567);
    ERRT("MOVE (*+$1233,PC),SR",   OPERAND_NOT_ALIGNED, "(*+$1233,PC),SR", 0043372, 0x1231);

    // MOVE SR,dst: 00403|M|Rn
    TEST("MOVE SR,D2",            0040302);
    ERRT("MOVE SR,A2",            OPERAND_NOT_ALLOWED, "A2", 0040300);
    TEST("MOVE SR,(A2)",          0040322);
    TEST("MOVE SR,(A2)+",         0040332);
    TEST("MOVE SR,-(A2)",         0040342);
    TEST("MOVE SR,($1234,A2)",    0040352, 0x1234);
    TEST("MOVE SR,($12,A2,D3.W)", 0040362, 0x3012);
    TEST("MOVE SR,($001234).W",   0040370, 0x1234);
    TEST("MOVE SR,($234568).L",   0040371, 0x0023, 0x4568);
    ERRT("MOVE SR,(*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0040300);
    ERRT("MOVE SR,(*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0040300);
    ERRT("MOVE SR,#$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0040300);
    ERRT("MOVE SR,($001235).W",   OPERAND_NOT_ALIGNED, "($001235).W",  0040370, 0x1235);
    ERRT("MOVE SR,($234567).L",   OPERAND_NOT_ALIGNED, "($234567).L",  0040371, 0x0023, 0x4567);

    // MOVE USP,An: 004715|An
    TEST("move   usp,a2", 0047152);
    TEST("MOVE.L USP,A2", 0047152);

    // MOVE An,USP: 004714|An
    TEST("MOVE   A2,USP", 0047142);
    TEST("MOVE.L A2,USP", 0047142);

    // ORI #nn,SR
    TEST("ORI #$1234,SR", 0000174, 0x1234);

    // RESET
    TEST("RESET", 047160);

    // NOP
    TEST("NOP", 047161);

    // STOP #nn
    TEST("STOP #$1234", 047162, 0x1234);

    // RTE
    TEST("RTE", 047163);

    // RTD
    if (mc68k00()) {
        ERUI("RTD #$1234");
    } else {
        TEST("RTD #$1234", 047164, 0x1234);
    }

    // RTS
    TEST("RTS", 047165);

    // TRAPV
    TEST("TRAPV", 047166);

    // TRAPcc
    if (!firstGen()) {
        TEST("TRAPT",  0050374 | 0x000);
        TEST("TRAPF",  0050374 | 0x100);
        TEST("TRAPHI", 0050374 | 0x200);
        TEST("TRAPLS", 0050374 | 0x300);
        TEST("TRAPCC", 0050374 | 0x400);
        TEST("TRAPCS", 0050374 | 0x500);
        TEST("TRAPNE", 0050374 | 0x600);
        TEST("TRAPEQ", 0050374 | 0x700);
        TEST("TRAPVC", 0050374 | 0x800);
        TEST("TRAPVS", 0050374 | 0x900);
        TEST("TRAPPL", 0050374 | 0xA00);
        TEST("TRAPMI", 0050374 | 0xB00);
        TEST("TRAPGE", 0050374 | 0xC00);
        TEST("TRAPLT", 0050374 | 0xD00);
        TEST("TRAPGT", 0050374 | 0xE00);
        TEST("TRAPLE", 0050374 | 0xF00);

        TEST("TRAPT.W  #$1234", 0050372 | 0x000, 0x1234);
        TEST("TRAPF.W  #$1234", 0050372 | 0x100, 0x1234);
        TEST("TRAPHI.W #$1234", 0050372 | 0x200, 0x1234);
        TEST("TRAPLS.W #$1234", 0050372 | 0x300, 0x1234);
        TEST("TRAPCC.W #$1234", 0050372 | 0x400, 0x1234);
        TEST("TRAPCS.W #$1234", 0050372 | 0x500, 0x1234);
        TEST("TRAPNE.W #$1234", 0050372 | 0x600, 0x1234);
        TEST("TRAPEQ.W #$1234", 0050372 | 0x700, 0x1234);
        TEST("TRAPVC.W #$1234", 0050372 | 0x800, 0x1234);
        TEST("TRAPVS.W #$1234", 0050372 | 0x900, 0x1234);
        TEST("TRAPPL.W #$1234", 0050372 | 0xA00, 0x1234);
        TEST("TRAPMI.W #$1234", 0050372 | 0xB00, 0x1234);
        TEST("TRAPGE.W #$1234", 0050372 | 0xC00, 0x1234);
        TEST("TRAPLT.W #$1234", 0050372 | 0xD00, 0x1234);
        TEST("TRAPGT.W #$1234", 0050372 | 0xE00, 0x1234);
        TEST("TRAPLE.W #$1234", 0050372 | 0xF00, 0x1234);

        TEST("TRAPT.L  #$12345678", 0050373 | 0x000, 0x1234, 0x5678);
        TEST("TRAPF.L  #$12345678", 0050373 | 0x100, 0x1234, 0x5678);
        TEST("TRAPHI.L #$12345678", 0050373 | 0x200, 0x1234, 0x5678);
        TEST("TRAPLS.L #$12345678", 0050373 | 0x300, 0x1234, 0x5678);
        TEST("TRAPCC.L #$12345678", 0050373 | 0x400, 0x1234, 0x5678);
        TEST("TRAPCS.L #$12345678", 0050373 | 0x500, 0x1234, 0x5678);
        TEST("TRAPNE.L #$12345678", 0050373 | 0x600, 0x1234, 0x5678);
        TEST("TRAPEQ.L #$12345678", 0050373 | 0x700, 0x1234, 0x5678);
        TEST("TRAPVC.L #$12345678", 0050373 | 0x800, 0x1234, 0x5678);
        TEST("TRAPVS.L #$12345678", 0050373 | 0x900, 0x1234, 0x5678);
        TEST("TRAPPL.L #$12345678", 0050373 | 0xA00, 0x1234, 0x5678);
        TEST("TRAPMI.L #$12345678", 0050373 | 0xB00, 0x1234, 0x5678);
        TEST("TRAPGE.L #$12345678", 0050373 | 0xC00, 0x1234, 0x5678);
        TEST("TRAPLT.L #$12345678", 0050373 | 0xD00, 0x1234, 0x5678);
        TEST("TRAPGT.L #$12345678", 0050373 | 0xE00, 0x1234, 0x5678);
        TEST("TRAPLE.L #$12345678", 0050373 | 0xF00, 0x1234, 0x5678);
    }

    // RTR
    TEST("RTR", 047167);

    // MOVEC
    if (mc68k00()) {
        ERUI("MOVEC VBR, A4");
    } else if (mc68010()) {
        TEST("MOVEC SFC, D1", 047172, 0010000 | 0x000);
        TEST("MOVEC DFC, A2", 047172, 0120000 | 0x001);
        TEST("MOVEC USP, D3", 047172, 0030000 | 0x800);
        TEST("MOVEC VBR, A4", 047172, 0140000 | 0x801);
        ERRT("MOVEC SR, D5", OPERAND_NOT_ALLOWED, "SR, D5");
        TEST("MOVEC D5, SFC", 047173, 0050000 | 0x000);
        TEST("MOVEC A6, DFC", 047173, 0160000 | 0x001);
        TEST("MOVEC D7, USP", 047173, 0070000 | 0x800);
        TEST("MOVEC A0, VBR", 047173, 0100000 | 0x801);
        ERRT("MOVEC D1, CCR", OPERAND_NOT_ALLOWED, "D1, CCR");
    } else {
        TEST("MOVEC SFC, D1",  0047172, 0010000 | 0x000);
        TEST("MOVEC DFC, A2",  0047172, 0120000 | 0x001);
        TEST("MOVEC CACR, D2", 0047172, 0020000 | 0x002);
        TEST("MOVEC USP, D3",  0047172, 0030000 | 0x800);
        TEST("MOVEC VBR, A4",  0047172, 0140000 | 0x801);
        if (mc68040()) {
            ERRT("MOVEC CAAR, D4", UNKNOWN_REGISTER,
                 "CAAR, D4",       0047172, 0040000 | 0x802);
        } else {
            TEST("MOVEC CAAR, D4", 0047172, 0040000 | 0x802);
        }
        TEST("MOVEC MSP, A5",  0047172, 0150000 | 0x803);
        TEST("MOVEC ISP, D5",  0047172, 0050000 | 0x804);
        ERRT("MOVEC SR, D5", OPERAND_NOT_ALLOWED, "SR, D5");
        TEST("MOVEC D5, SFC",  0047173, 0050000 | 0x000);
        TEST("MOVEC A6, DFC",  0047173, 0160000 | 0x001);
        TEST("MOVEC D7, CACR", 0047173, 0070000 | 0x002);
        TEST("MOVEC A0, USP",  0047173, 0100000 | 0x800);
        TEST("MOVEC D1, VBR",  0047173, 0010000 | 0x801);
        if (mc68040()) {
            ERRT("MOVEC A2, CAAR", UNKNOWN_REGISTER,
                 "CAAR",           0047173, 0120000 | 0x802);
        } else {
            TEST("MOVEC A2, CAAR", 0047173, 0120000 | 0x802);
        }
        TEST("MOVEC D2, MSP",  0047173, 0020000 | 0x803);
        TEST("MOVEC A3, ISP",  0047173, 0130000 | 0x804);
        ERRT("MOVEC D1, CCR", OPERAND_NOT_ALLOWED, "D1, CCR");
    }
    if (mc68040()) {
        // MC68040
        TEST("PMMU ON");
        TEST("PMMU MC68040");
        TEST("MOVEC TC, A3",    0047172, 0130000 | 0x003);
        TEST("MOVEC ITT0, D3",  0047172, 0030000 | 0x004);
        TEST("MOVEC ITT1, A4",  0047172, 0140000 | 0x005);
        TEST("MOVEC DTT0, D4",  0047172, 0040000 | 0x006);
        TEST("MOVEC DTT1, A5",  0047172, 0150000 | 0x007);
        TEST("MOVEC MMUSR, A6", 0047172, 0160000 | 0x805);
        TEST("MOVEC URP, D6",   0047172, 0060000 | 0x806);
        TEST("MOVEC SRP, A7",   0047172, 0170000 | 0x807);
        TEST("MOVEC A7, TC",    0047173, 0170000 | 0x003);
        TEST("MOVEC D0, ITT0",  0047173, 0000000 | 0x004);
        TEST("MOVEC A0, ITT1",  0047173, 0100000 | 0x005);
        TEST("MOVEC D1, DTT0",  0047173, 0010000 | 0x006);
        TEST("MOVEC A1, DTT1",  0047173, 0110000 | 0x007);
        TEST("MOVEC D3, MMUSR", 0047173, 0030000 | 0x805);
        TEST("MOVEC A4, URP",   0047173, 0140000 | 0x806);
        TEST("MOVEC D4, SRP",   0047173, 0040000 | 0x807);
        ERRT("MOVEC D0, IACR0",  UNKNOWN_REGISTER, "IACR0",  0047173, 0000000 | 0x004);
        ERRT("MOVEC A0, IACR1",  UNKNOWN_REGISTER, "IACR1",  0047173, 0100000 | 0x005);
        ERRT("MOVEC D1, DACR0",  UNKNOWN_REGISTER, "DACR0",  0047173, 0010000 | 0x006);
        ERRT("MOVEC A1, DACR1",  UNKNOWN_REGISTER, "DACR1",  0047173, 0110000 | 0x007);

        // MC68EC040
        TEST("PMMU OFF");
        ERRT("MOVEC TC, A3",    UNKNOWN_REGISTER, "TC, A3",    0047172, 0130000 | 0x003);
        ERRT("MOVEC ITT0, D3",  UNKNOWN_REGISTER, "ITT0, D3",  0047172, 0030000 | 0x004);
        ERRT("MOVEC ITT1, A4",  UNKNOWN_REGISTER, "ITT1, A4",  0047172, 0140000 | 0x005);
        ERRT("MOVEC DTT0, D4",  UNKNOWN_REGISTER, "DTT0, D4",  0047172, 0040000 | 0x006);
        ERRT("MOVEC DTT1, A5",  UNKNOWN_REGISTER, "DTT1, A5",  0047172, 0150000 | 0x007);
        ERRT("MOVEC MMUSR, A6", UNKNOWN_REGISTER, "MMUSR, A6", 0047172, 0160000 | 0x805);
        ERRT("MOVEC URP, D6",   UNKNOWN_REGISTER, "URP, D6",   0047172, 0060000 | 0x806);
        ERRT("MOVEC SRP, A7",   UNKNOWN_REGISTER, "SRP, A7",   0047172, 0170000 | 0x807);
        ERRT("MOVEC A7, TC",    UNKNOWN_REGISTER, "TC",    0047173, 0170000 | 0x003);
        ERRT("MOVEC D0, ITT0",  UNKNOWN_REGISTER, "ITT0",  0047173, 0000000 | 0x004);
        ERRT("MOVEC A0, ITT1",  UNKNOWN_REGISTER, "ITT1",  0047173, 0100000 | 0x005);
        ERRT("MOVEC D1, DTT0",  UNKNOWN_REGISTER, "DTT0",  0047173, 0010000 | 0x006);
        ERRT("MOVEC A1, DTT1",  UNKNOWN_REGISTER, "DTT1",  0047173, 0110000 | 0x007);
        ERRT("MOVEC D3, MMUSR", UNKNOWN_REGISTER, "MMUSR", 0047173, 0030000 | 0x805);
        ERRT("MOVEC A4, URP",   UNKNOWN_REGISTER, "URP",   0047173, 0140000 | 0x806);
        ERRT("MOVEC D4, SRP",   UNKNOWN_REGISTER, "SRP",   0047173, 0040000 | 0x807);
        TEST("MOVEC D0, IACR0",  0047173, 0000000 | 0x004);
        TEST("MOVEC A0, IACR1",  0047173, 0100000 | 0x005);
        TEST("MOVEC D1, DACR0",  0047173, 0010000 | 0x006);
        TEST("MOVEC A1, DACR1",  0047173, 0110000 | 0x007);
    }

    if (mc68k00()) {
        ERUI("BKPT #0");
    } else {
        TEST("BKPT #0", 0044110);
        TEST("BKPT #7", 0044117);
        ERRT("BKPT #8", OVERFLOW_RANGE, "#8", 0044110);
    }

    // CHK src,Dn: 004|Dn|Sz|M|Rn, Sz:W=6/L=7
    TEST("CHK   D2,D7",              0047602);
    TEST("CHK.W D2,D7",              0047602);
    ERRT("CHK.W A2,D7",              OPERAND_NOT_ALLOWED, "A2,D7", 0047600);
    TEST("CHK.W (A2),D7",            0047622);
    TEST("CHK.W (A2)+,D7",           0047632);
    TEST("CHK.W -(A2),D7",           0047642);
    TEST("CHK.W ($1234,A2),D7",      0047652, 0x1234);
    TEST("CHK.W ($12,A2,D3.L),D7",   0047662, 0x3812);
    ABSW("CHK.W (@W).W,D7", "FFFE",  0047670, 0xFFFE);
    TEST("CHK.W ($123456).L,D7",     0047671, 0x0012, 0x3456);
    TEST("CHK.W (*+$1234,PC),D7",    0047672, 0x1232);
    TEST("CHK.W (*+$12,PC,D3.W),D7", 0047673, 0x3010);
    TEST("CHK.W #$0034,D7",          0047674, 0x0034);
    if (!firstGen()) {
        TEST("CHK.L D2,D7",              0047402);
        ERRT("CHK.L A2,D7",              OPERAND_NOT_ALLOWED, "A2,D7", 0047400);
        TEST("CHK.L (A2),D7",            0047422);
        TEST("CHK.L (A2)+,D7",           0047432);
        TEST("CHK.L -(A2),D7",           0047442);
        TEST("CHK.L ($1234,A2),D7",      0047452, 0x1234);
        TEST("CHK.L ($12,A2,D3.L),D7",   0047462, 0x3812);
        ABSW("CHK.L (@W).W,D7", "FFFE",  0047470, 0xFFFE);
        TEST("CHK.L ($123456).L,D7",     0047471, 0x0012, 0x3456);
        TEST("CHK.L (*+$1234,PC),D7",    0047472, 0x1232);
        TEST("CHK.L (*+$12,PC,D3.W),D7", 0047473, 0x3010);
    }

    if (!firstGen()) {
        // CHK2 src,Rd, 00|Sz|3|M|Rn, Rd|40000, Sz:B=0/W=1/L=2
        ERRT("CHK2.B D2, D7",    OPERAND_NOT_ALLOWED, "D2, D7", 0000300, 0074000);
        ERRT("CHK2.B A2, A5",    OPERAND_NOT_ALLOWED, "A2, A5", 0000300, 0154000);
        TEST("CHK2.B (A2), D3",           0000322, 0034000);
        ERRT("CHK2.B (A2)+, D6", OPERAND_NOT_ALLOWED, "(A2)+, D6", 0000300, 0064000);
        ERRT("CHK2.B-(A2), A3",  OPERAND_NOT_ALLOWED, "-(A2), A3", 0000300, 0134000);
        TEST("CHK2.B ($1234,A2), A6",     0000352, 0164000, 0x1234);
        TEST("CHK2.B (18,A2,D3.L), D7",   0000362, 0074000, 0x3812);
        ABSW("CHK2.B (@W).W, A0", "FFFE", 0000370, 0104000, 0xFFFE);
        TEST("CHK2.B ($123456).L, D0",    0000371, 0004000, 0x0012, 0x3456);
        TEST("CHK2.B (*+$1234,PC), A1",   0000372, 0114000, 0x1230);
        TEST("CHK2.B (*+18,PC,D3.W), D2", 0000373, 0024000, 0x300E);
        ERRT("CHK2.B #$34, D3",  OPERAND_NOT_ALLOWED, "#$34, D3", 0000300, 0034000);
        ERRT("CHK2.W D2, D7",    OPERAND_NOT_ALLOWED, "D2, D7", 0001300, 0074000);
        ERRT("CHK2.W A2, A5",    OPERAND_NOT_ALLOWED, "A2, A5", 0001300, 0154000);
        TEST("CHK2.W (A2), D3",           0001322, 0034000);
        ERRT("CHK2.W (A2)+, D6", OPERAND_NOT_ALLOWED, "(A2)+, D6", 0001300, 0064000);
        ERRT("CHK2.W-(A2), A3",  OPERAND_NOT_ALLOWED, "-(A2), A3", 0001300, 0134000);
        TEST("CHK2.W ($1234,A2), A6",     0001352, 0164000, 0x1234);
        TEST("CHK2.W (18,A2,D3.L), D7",   0001362, 0074000, 0x3812);
        ABSW("CHK2.W (@W).W, A0", "FFFE", 0001370, 0104000, 0xFFFE);
        TEST("CHK2.W ($123456).L, D0",    0001371, 0004000, 0x0012, 0x3456);
        TEST("CHK2.W (*+$1234,PC), A1",   0001372, 0114000, 0x1230);
        TEST("CHK2.W (*+18,PC,D3.W), D2", 0001373, 0024000, 0x300E);
        ERRT("CHK2.W #$1234, D3",  OPERAND_NOT_ALLOWED, "#$1234, D3", 0001300, 0034000);
        ERRT("CHK2.L D2, D7",    OPERAND_NOT_ALLOWED, "D2, D7", 0002300, 0074000);
        ERRT("CHK2.L A2, A5",    OPERAND_NOT_ALLOWED, "A2, A5", 0002300, 0154000);
        TEST("CHK2.L (A2), D3",           0002322, 0034000);
        ERRT("CHK2.L (A2)+, D6", OPERAND_NOT_ALLOWED, "(A2)+, D6", 0002300, 0064000);
        ERRT("CHK2.L-(A2), A3",  OPERAND_NOT_ALLOWED, "-(A2), A3", 0002300, 0134000);
        TEST("CHK2.L ($1234,A2), A6",     0002352, 0164000, 0x1234);
        TEST("CHK2.L (18,A2,D3.L), D7",   0002362, 0074000, 0x3812);
        ABSW("CHK2.L (@W).W, A0", "FFFE", 0002370, 0104000, 0xFFFE);
        TEST("CHK2.L ($123456).L, D0",    0002371, 0004000, 0x0012, 0x3456);
        TEST("CHK2.L (*+$1234,PC), A1",   0002372, 0114000, 0x1230);
        TEST("CHK2.L (*+18,PC,D3.W), D2", 0002373, 0024000, 0x300E);
        ERRT("CHK2.L #$123456, D3",  OPERAND_NOT_ALLOWED, "#$123456, D3", 0002300, 0034000);
    }

    // ILLEGAL
    TEST("ILLEGAL", 0045374);

    // TRAP #nn
    TEST("TRAP #0",  0047100);
    TEST("TRAP #15", 0047117);

    // ANDI #nn,CCR
    TEST("andi   #$34,ccr", 0001074, 0x0034);
    TEST("ANDI.B #$34,CCR", 0001074, 0x0034);

    // EORI #nn,CCR
    TEST("EORI   #$34,CCR", 0005074, 0x0034);
    TEST("EORI.B #$34,CCR", 0005074, 0x0034);

    // MOVE src,CCR: 00423|M|R
    TEST("MOVE D2,CCR",             0042302);
    ERRT("MOVE A2,CCR",             OPERAND_NOT_ALLOWED, "A2,CCR", 0042300);
    TEST("MOVE   (A2),CCR",         0042322);
    TEST("MOVE.W (A2),CCR",         0042322);
    TEST("MOVE (A2)+,CCR",          0042332);
    TEST("MOVE -(A2),CCR",          0042342);
    TEST("MOVE ($1234,A2),CCR",     0042352, 0x1234);
    TEST("MOVE ($12,A2,D3.W),CCR",  0042362, 0x3012);
    TEST("MOVE ($001234).W,CCR",    0042370, 0x1234);
    TEST("MOVE ($234566).L,CCR",    0042371, 0x0023, 0x4566);
    TEST("MOVE (*+$1234,PC),CCR",   0042372, 0x1232);
    TEST("MOVE (*-16,PC,D3.L),CCR", 0042373, 0x38EE);
    TEST("MOVE #$34,CCR",           0042374, 0x0034);
    ERRT("MOVE ($001235).W,CCR",    OPERAND_NOT_ALIGNED, "($001235).W,CCR",  0042370, 0x1235);
    ERRT("MOVE ($234567).L,CCR",    OPERAND_NOT_ALIGNED, "($234567).L,CCR",  0042371, 0x0023, 0x4567);
    ERRT("MOVE (*+$1235,PC),CCR",   OPERAND_NOT_ALIGNED, "(*+$1235,PC),CCR", 0042372, 0x1233);

    // MOVE CCR,dst: 00413|M|R
    if (mc68k00()) {
        ERRT("MOVE CCR,D2",             OPERAND_NOT_ALLOWED, "CCR,D2");
        ERRT("MOVE CCR,A2",             OPERAND_NOT_ALLOWED, "CCR,A2");
        ERRT("MOVE CCR,(A2)",           OPERAND_NOT_ALLOWED, "CCR,(A2)");
        ERRT("MOVE CCR,(A2)+",          OPERAND_NOT_ALLOWED, "CCR,(A2)+");
        ERRT("MOVE CCR,-(A2)",          OPERAND_NOT_ALLOWED, "CCR,-(A2)");
        ERRT("MOVE CCR,($1234,A2)",     OPERAND_NOT_ALLOWED, "CCR,($1234,A2)");
        ERRT("MOVE CCR,($12,A2,D3.W)",  OPERAND_NOT_ALLOWED, "CCR,($12,A2,D3.W)");
        ERRT("MOVE CCR,($001234).W",    OPERAND_NOT_ALLOWED, "CCR,($001234).W");
        ERRT("MOVE CCR,($234566).L",    OPERAND_NOT_ALLOWED, "CCR,($234566).L");
        ERRT("MOVE CCR,(*+$1234,PC)",   OPERAND_NOT_ALLOWED, "CCR,(*+$1234,PC)");
        ERRT("MOVE CCR,(*-16,PC,D3.L)", OPERAND_NOT_ALLOWED, "CCR,(*-16,PC,D3.L)");
        ERRT("MOVE CCR,#$1234",         OPERAND_NOT_ALLOWED, "CCR,#$1234");
    } else {
        TEST("MOVE CCR,D2",             0041302);
        ERRT("MOVE CCR,A2",             OPERAND_NOT_ALLOWED, "A2", 0041300);
        TEST("MOVE CCR,(A2)",           0041322);
        TEST("MOVE CCR,(A2)+",          0041332);
        TEST("MOVE CCR,-(A2)",          0041342);
        TEST("MOVE CCR,($1234,A2)",     0041352, 0x1234);
        TEST("MOVE CCR,($12,A2,D3.W)",  0041362, 0x3012);
        TEST("MOVE CCR,($001234).W",    0041370, 0x1234);
        TEST("MOVE CCR,($234566).L",    0041371, 0x0023, 0x4566);
        ERRT("MOVE CCR,(*+$1234,PC)",   OPERAND_NOT_ALLOWED, "(*+$1234,PC)",   0041300);
        ERRT("MOVE CCR,(*-16,PC,D3.L)", OPERAND_NOT_ALLOWED, "(*-16,PC,D3.L)", 0041300);
        ERRT("MOVE CCR,#$1234",         OPERAND_NOT_ALLOWED, "#$1234",         0041300);
    }

    // ORI #nn,CCR
    TEST("ORI   #$34,CCR", 0000074, 0x0034);
    TEST("ORI.B #$34,CCR", 0000074, 0x0034);

    if (mc68040()) {
        TEST("CINVL NC, (A2)",    0172012);
        TEST("CINVL DC, (A2)",    0172112);
        TEST("CINVL IC, (A2)",    0172212);
        TEST("CINVL BC, (A2)",    0172312);
        TEST("CINVL DC/IC, (A2)", 0172312);
        TEST("CINVP NC, (A2)",    0172022);
        TEST("CINVP DC, (A2)",    0172122);
        TEST("CINVP IC, (A2)",    0172222);
        TEST("CINVP BC, (A2)",    0172322);
        TEST("CINVP DC/IC, (A2)", 0172322);
        TEST("CINVA NC",          0172030);
        TEST("CINVA DC",          0172130);
        TEST("CINVA IC",          0172230);
        TEST("CINVA BC",          0172330);
        TEST("CINVA DC/IC",       0172330);

        TEST("CPUSHL NC, (A2)",    0172052);
        TEST("CPUSHL DC, (A2)",    0172152);
        TEST("CPUSHL IC, (A2)",    0172252);
        TEST("CPUSHL BC, (A2)",    0172352);
        TEST("CPUSHL DC/IC, (A2)", 0172352);
        TEST("CPUSHP NC, (A2)",    0172062);
        TEST("CPUSHP DC, (A2)",    0172162);
        TEST("CPUSHP IC, (A2)",    0172262);
        TEST("CPUSHP BC, (A2)",    0172362);
        TEST("CPUSHP DC/IC, (A2)", 0172362);
        TEST("CPUSHA NC",          0172070);
        TEST("CPUSHA DC",          0172170);
        TEST("CPUSHA IC",          0172270);
        TEST("CPUSHA BC",          0172370);
        TEST("CPUSHA DC/IC",       0172370);
    }
}

void test_multiproc() {
    // TAS dst: 00453|M|Rn
    TEST("TAS D2",            0045302);
    ERRT("TAS A2",            OPERAND_NOT_ALLOWED, "A2", 0045300);
    TEST("TAS (A2)",          0045322);
    TEST("TAS (A2)+",         0045332);
    TEST("TAS -(A2)",         0045342);
    TEST("TAS ($1234,A2)",    0045352, 0x1234);
    TEST("TAS ($12,A2,D3.W)", 0045362, 0x3012);
    TEST("TAS ($001235).W",   0045370, 0x1235);
    TEST("TAS ($123457).L",   0045371, 0x0012, 0x3457);
    ERRT("TAS (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0045300);
    ERRT("TAS (*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0045300);
    ERRT("TAS #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0045300);

    if (!firstGen()) {
        // CAS Dc,Du,dst: 00|Sz|3|M|Rn, 000|Du|0|Dc, Sz:B=5/W=6/L=7
        ERRT("CAS.B D2, D3, D4", OPERAND_NOT_ALLOWED, "D4", 0005300, 0000302);
        ERRT("CAS.B D2, D3, A4", OPERAND_NOT_ALLOWED, "A4", 0005300, 0000302);
        TEST("CAS.B D2, D3, (A4)",           0005324, 0000302);
        TEST("CAS.B D3, D4, (A2)+",          0005332, 0000403);
        TEST("CAS.B D4, D5, -(A2)",          0005342, 0000504);
        TEST("CAS.B D5, D6, ($1234,A2)",     0005352, 0000605, 0x1234);
        TEST("CAS.B D6, D7, (18,A2,D3.W)",   0005362, 0000706, 0x3012);
        ABSW("CAS.B D7, D0, (@W).W", "1234", 0005370, 0000007, 0x1234);
        TEST("CAS.B D0, D1, ($123456).L",    0005371, 0000100, 0x0012, 0x3456);
        ERRT("CAS.B D2, D3, (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0005300, 0000302);
        ERRT("CAS.B D2, D3, (*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0005300, 0000302);
        ERRT("CAS.B D2, D3, #$34",          OPERAND_NOT_ALLOWED, "#$34",          0005300, 0000302);
        ERRT("CAS.W D2, D3, D4", OPERAND_NOT_ALLOWED, "D4", 0006300, 0000302);
        ERRT("CAS.W D2, D3, A4", OPERAND_NOT_ALLOWED, "A4", 0006300, 0000302);
        TEST("CAS.W D2, D3, (A4)",           0006324, 0000302);
        TEST("CAS.W D3, D4, (A2)+",          0006332, 0000403);
        TEST("CAS.W D4, D5, -(A2)",          0006342, 0000504);
        TEST("CAS.W D5, D6, ($1234,A2)",     0006352, 0000605, 0x1234);
        TEST("CAS.W D6, D7, (18,A2,D3.W)",   0006362, 0000706, 0x3012);
        ABSW("CAS.W D7, D0, (@W).W", "FEDC", 0006370, 0000007, 0xFEDC);
        TEST("CAS.W D0, D1, ($123456).L",    0006371, 0000100, 0x0012, 0x3456);
        ERRT("CAS.W D2, D3, (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0006300, 0000302);
        ERRT("CAS.W D2, D3, (*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0006300, 0000302);
        ERRT("CAS.W D2, D3, #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0006300, 0000302);
        ERRT("CAS.L D2, D3, D4", OPERAND_NOT_ALLOWED, "D4", 0007300, 0000302);
        ERRT("CAS.L D2, D3, A4", OPERAND_NOT_ALLOWED, "A4", 0007300, 0000302);
        TEST("CAS.L D2, D3, (A4)",           0007324, 0000302);
        TEST("CAS.L D3, D4, (A2)+",          0007332, 0000403);
        TEST("CAS.L D4, D5, -(A2)",          0007342, 0000504);
        TEST("CAS.L D5, D6, ($1234,A2)",     0007352, 0000605, 0x1234);
        TEST("CAS.L D6, D7, (18,A2,D3.W)",   0007362, 0000706, 0x3012);
        ABSW("CAS.L D7, D0, (@W).W", "1234", 0007370, 0000007, 0x1234);
        TEST("CAS.L D0, D1, ($123454).L",    0007371, 0000100, 0x0012, 0x3454);
        ERRT("CAS.L D2, D3, (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0007300, 0000302);
        ERRT("CAS.L D2, D3, (*+$12,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$12,PC,D3)", 0007300, 0000302);
        ERRT("CAS.L D2, D3, #$12345678",    OPERAND_NOT_ALLOWED, "#$12345678",    0007300, 0000302);

        // CAS2 Dc1:Dc2, Du1:Du2, (Rn1:Rn2), 00|Sz|3|M|Rn, R1|0|Du1|0|Dc1, R2|0|Du2|0|Dc2
        // Sz:W=6/L=7
        TEST("CAS2.W D1:D2, D3:D4, (D5):(D6)", 0006374, 0050301, 0060402);
        TEST("CAS2.W D1:D2, D3:D4, (A5):(D6)", 0006374, 0150301, 0060402);
        TEST("CAS2.W D1:D2, D3:D4, (D5):(A6)", 0006374, 0050301, 0160402);
        TEST("CAS2.W D1:D2, D3:D4, (A5):(A6)", 0006374, 0150301, 0160402);
        TEST("CAS2.L D1:D2, D3:D4, (D5):(D6)", 0007374, 0050301, 0060402);
        TEST("CAS2.L D1:D2, D3:D4, (A5):(D6)", 0007374, 0150301, 0060402);
        TEST("CAS2.L D1:D2, D3:D4, (D5):(A6)", 0007374, 0050301, 0160402);
        TEST("CAS2.L D1:D2, D3:D4, (A5):(A6)", 0007374, 0150301, 0160402);
    }
}

void test_areg_alias() {
    TEST("ADDI.W #0,A0", 0150374, 0x0000);          // ADDA.W #0,A0
    TEST("ADDI.L #0,A0", 0150774, 0x0000, 0x0000);  // ADDA.L #0,A0
    TEST("SUBI.W #0,A0", 0110374, 0x0000);          // SUBA.W #0,A0
    TEST("SUBI.L #0,A0", 0110774, 0x0000, 0x0000);  // SUBA.L #0,A0
    TEST("CMPI.W #0,A0", 0130374, 0x0000);          // CMPA.W #0,A0
    TEST("CMPI.L #0,A0", 0130774, 0x0000, 0x0000);  // CMPA.L #0,A0
    TEST("ADD.W  #0,A0", 0150374, 0x0000);          // ADDA.W #0,A0
    TEST("ADD.L  #0,A0", 0150774, 0x0000, 0x0000);  // ADDA.L #0,A0
    TEST("SUB.W  #0,A0", 0110374, 0x0000);          // SUBA.W #0,A0
    TEST("SUB.L  #0,A0", 0110774, 0x0000, 0x0000);  // SUBA.L #0,A0
    TEST("MOVE.W D2,A6",              0036102);     // MOVEA.W D2,A6
    TEST("MOVE.W A2,A6",              0036112);     // MOVEA.W A2,A6
    TEST("MOVE.W (A2),A6",            0036122);
    TEST("MOVE.W (A2)+,A6",           0036132);
    TEST("MOVE.W -(A2),A6",           0036142);
    TEST("MOVE.W ($1234,A2),A6",      0036152, 0x1234);
    TEST("MOVE.W ($12,A2,D3.L),A6",   0036162, 0x3812);
    ABSW("MOVE.W (@W).W,A6", "FFFE",  0036170, 0xFFFE);
    TEST("MOVE.W ($123456).L,A6",     0036171, 0x0012, 0x3456);
    TEST("MOVE.W (*+$1234,PC),A6",    0036172, 0x1232);
    TEST("MOVE.W (*+$12,PC,D3.W),A6", 0036173, 0x3010);
    TEST("MOVE.W #$0034,A6",          0036174, 0x0034);
    TEST("MOVE.L D2,A6",              0026102);     // MOVEA.L D2,A6
    TEST("MOVE.L A2,A6",              0026112);     // MOVEA.L A2,A6
    TEST("MOVE.L (A2),A6",            0026122);
    TEST("MOVE.L (A2)+,A6",           0026132);
    TEST("MOVE.L -(A2),A6",           0026142);
    TEST("MOVE.L ($1234,A2),A6",      0026152, 0x1234);
    TEST("MOVE.L ($12,A2,D3.L),A6",   0026162, 0x3812);
    ABSW("MOVE.L (@W).W,A6", "FFFC",  0026170, 0xFFFC);
    TEST("MOVE.L ($123454).L,A6",     0026171, 0x0012, 0x3454);
    TEST("MOVE.L (*+$1234,PC),A6",    0026172, 0x1232);
    TEST("MOVE.L (*+$12,PC,D3.W),A6", 0026173, 0x3010);
    TEST("MOVE.L #$00345678,A6",      0026174, 0x0034, 0x5678);
}

void test_addressing() {

#define TIDX(opr, mr, ...) TEST("MOVE.L " opr ", D2", 0022000 | mr,  __VA_ARGS__)

    // MOVE.L src, D2: 00220|M|Rn
    TIDX("D1",             001);
    TIDX("A2",             012);
    TIDX("(A3)",           023);
    TIDX("(A4)+",          034);
    TIDX("-(A5)",          045);
    TIDX("($1234,A6)",     056, 0x1234);
    TIDX("($1234).W",      070, 0x1234);
    TIDX("($123456).L",    071, 0x0012, 0x3456);
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

    if (firstGen())
        return;

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

    TIDX("($1234)",     070, 0x1234);
    TIDX("($12345678)", 071, 0x1234, 0x5678);

    TIDX("([$1234])",           060, 0000741, 0x1234);
    TIDX("([$1234],$5678)",     060, 0000742, 0x1234, 0x5678);
    TIDX("([$1234],$56789ABC)", 060, 0000743, 0x1234, 0x5678, 0x9ABC);

    TIDX("([$12345678])",           060, 0000761, 0x1234, 0x5678);
    TIDX("([$12345678],$7654)",     060, 0000762, 0x1234, 0x5678, 0x7654);
    TIDX("([$12345678],$76543210)", 060, 0000763, 0x1234, 0x5678, 0x7654, 0x3210);

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

    TIDX("(A7)",           027);
    TIDX("($78,A7)",       057, 0x0078);
    TIDX("($5678,A7)",     057, 0x5678);
    TIDX("($12345678,A7)", 067, 0000560, 0x1234, 0x5678);

    TIDX("(A7.L)",           027);
    TIDX("($78,A7.L)",       057, 0x0078);
    TIDX("($5678,A7.L)",     057, 0x5678);
    TIDX("($12345678,A7.L)", 067, 0000560, 0x1234, 0x5678);

    TIDX("(A7.W)",           060, 0170620);
    TIDX("($78,A7.W)",       060, 0170640, 0x0078);
    TIDX("($5678,A7.W)",     060, 0170640, 0x5678);
    TIDX("($12345678,A7.W)", 060, 0170660, 0x1234, 0x5678);

    TIDX("(D1)",           060, 0010620);
    TIDX("($5678,D1)",     060, 0010640, 0x5678);
    TIDX("($12345678,D1)", 060, 0010660, 0x1234, 0x5678);

    TIDX("(D1.L)",           060, 0014620);
    TIDX("($5678,D1.L)",     060, 0014640, 0x5678);
    TIDX("($12345678,D1.L)", 060, 0014660, 0x1234, 0x5678);

    TIDX("(D1.W)",           060, 0010620);
    TIDX("($5678,D1.W)",     060, 0010640, 0x5678);
    TIDX("($12345678,D1.W)", 060, 0010660, 0x1234, 0x5678);

    TIDX("([A7])",           067, 0000521);
    TIDX("([A7],$1234)",     067, 0000522, 0x1234);
    TIDX("([A7],$12345678)", 067, 0000523, 0x1234, 0x5678);

    TIDX("([A7.W])",           060, 0170621);
    TIDX("([A7.W],$1234)",     060, 0170622, 0x1234);
    TIDX("([A7.W],$12345678)", 060, 0170623, 0x1234, 0x5678);

    TIDX("([D1])",           060, 0010621);
    TIDX("([D1],$1234)",     060, 0010622, 0x1234);
    TIDX("([D1],$12345678)", 060, 0010623, 0x1234, 0x5678);

    TIDX("([D1.L])",           060, 0014621);
    TIDX("([D1.L],$1234)",     060, 0014622, 0x1234);
    TIDX("([D1.L],$12345678)", 060, 0014623, 0x1234, 0x5678);

    TIDX("([$1234,A7])",           067, 0000541, 0x1234);
    TIDX("([$1234,A7],$5678)",     067, 0000542, 0x1234, 0x5678);
    TIDX("([$1234,A7],$56789ABC)", 067, 0000543, 0x1234, 0x5678, 0x9ABC);

    TIDX("([$1234,D1.W])",           060, 0010641, 0x1234);
    TIDX("([$1234,D1.W],$5678)",     060, 0010642, 0x1234, 0x5678);
    TIDX("([$1234,D1.W],$56789ABC)", 060, 0010643, 0x1234, 0x5678, 0x9ABC);

    TIDX("([$12345678,A7])",           067, 0000561, 0x1234, 0x5678);
    TIDX("([$12345678,A7],$7654)",     067, 0000562, 0x1234, 0x5678, 0x7654);
    TIDX("([$12345678,A7],$76543210)", 067, 0000563, 0x1234, 0x5678, 0x7654, 0x3210);

    TIDX("([$12345678,D1.W])",           060, 0010661, 0x1234, 0x5678);
    TIDX("([$12345678,D1.W],$7654)",     060, 0010662, 0x1234, 0x5678, 0x7654);
    TIDX("([$12345678,D1.W],$76543210)", 060, 0010663, 0x1234, 0x5678, 0x7654, 0x3210);

    TIDX("([$1234],A7)",           060, 0170645, 0x1234);
    TIDX("([$1234],A7,$5678)",     060, 0170646, 0x1234, 0x5678);
    TIDX("([$1234],A7,$56789ABC)", 060, 0170647, 0x1234, 0x5678, 0x9ABC);

    TIDX("([$1234],D1)",           060, 0010645, 0x1234);
    TIDX("([$1234],D1,$5678)",     060, 0010646, 0x1234, 0x5678);
    TIDX("([$1234],D1,$56789ABC)", 060, 0010647, 0x1234, 0x5678, 0x9ABC);

    TIDX("([$12345678],A7)",           060, 0170665, 0x1234, 0x5678);
    TIDX("([$12345678],A7,$7654)",     060, 0170666, 0x1234, 0x5678, 0x7654);
    TIDX("([$12345678],A7,$76543210)", 060, 0170667, 0x1234, 0x5678, 0x7654, 0x3210);

    TIDX("([$12345678],D1)",           060, 0010665, 0x1234, 0x5678);
    TIDX("([$12345678],D1,$7654)",     060, 0010666, 0x1234, 0x5678, 0x7654);
    TIDX("([$12345678],D1,$76543210)", 060, 0010667, 0x1234, 0x5678, 0x7654, 0x3210);

    TIDX("(A7,D1.W)",           067, 0010000);
    TIDX("($34,A7,D1.W)",       067, 0010000 | 0x34);
    TIDX("($1234,A7,D1.W)",     067, 0010440, 0x1234);
    TIDX("($12345678,A7,D1.W)", 067, 0010460, 0x1234, 0x5678);

    TIDX("([A7,D1.W])",           067, 0010421);
    TIDX("([A7,D1.W],$34)",       067, 0010422, 0x0034);
    TIDX("([A7,D1.W],$1234)",     067, 0010422, 0x1234);
    TIDX("([A7,D1.W],$12345678)", 067, 0010423, 0x1234, 0x5678);
    TIDX("([A7],D1.W)",           067, 0010425);
    TIDX("([A7],D1.W,$34)",       067, 0010426, 0x0034);
    TIDX("([A7],D1.W,$1234)",     067, 0010426, 0x1234);
    TIDX("([A7],D1.W,$12345678)", 067, 0010427, 0x1234, 0x5678);

    TIDX("([$34,A7,D1.W])",           067, 0010441, 0x0034);
    TIDX("([$34,A7,D1.W],$78)",       067, 0010442, 0x0034, 0x0078);
    TIDX("([$34,A7,D1.W],$5678)",     067, 0010442, 0x0034, 0x5678);
    TIDX("([$34,A7,D1.W],$56789ABC)", 067, 0010443, 0x0034, 0x5678, 0x9ABC);
    TIDX("([$34,A7],D1.W)",           067, 0010445, 0x0034);
    TIDX("([$34,A7],D1.W,$78)",       067, 0010446, 0x0034, 0x0078);
    TIDX("([$34,A7],D1.W,$5678)",     067, 0010446, 0x0034, 0x5678);
    TIDX("([$34,A7],D1.W,$56789ABC)", 067, 0010447, 0x0034, 0x5678, 0x9ABC);

    TIDX("([$1234,A7,D1.W])",           067, 0010441, 0x1234);
    TIDX("([$1234,A7,D1.W],$78)",       067, 0010442, 0x1234, 0x0078);
    TIDX("([$1234,A7,D1.W],$5678)",     067, 0010442, 0x1234, 0x5678);
    TIDX("([$1234,A7,D1.W],$56789ABC)", 067, 0010443, 0x1234, 0x5678, 0x9ABC);
    TIDX("([$1234,A7],D1.W)",           067, 0010445, 0x1234);
    TIDX("([$1234,A7],D1.W,$78)",       067, 0010446, 0x1234, 0x0078);
    TIDX("([$1234,A7],D1.W,$5678)",     067, 0010446, 0x1234, 0x5678);
    TIDX("([$1234,A7],D1.W,$56789ABC)", 067, 0010447, 0x1234, 0x5678, 0x9ABC);

    TIDX("([$12345678,A7,D1.W])",           067, 0010461, 0x1234, 0x5678);
    TIDX("([$12345678,A7,D1.W],$54)",       067, 0010462, 0x1234, 0x5678, 0x0054);
    TIDX("([$12345678,A7,D1.W],$7654)",     067, 0010462, 0x1234, 0x5678, 0x7654);
    TIDX("([$12345678,A7,D1.W],$76543210)", 067, 0010463, 0x1234, 0x5678, 0x7654, 0x3210);
    TIDX("([$12345678,A7],D1.W)",           067, 0010465, 0x1234, 0x5678);
    TIDX("([$12345678,A7],D1.W,$54)",       067, 0010466, 0x1234, 0x5678, 0x0054);
    TIDX("([$12345678,A7],D1.W,$7654)",     067, 0010466, 0x1234, 0x5678, 0x7654);
    TIDX("([$12345678,A7],D1.W,$76543210)", 067, 0010467, 0x1234, 0x5678, 0x7654, 0x3210);

    TIDX("(*+2,PC,D1.W)",         073, 0010000);
    TIDX("(*+$36,PC,D1.W)",       073, 0010000 | 0x34);
    TIDX("(*+$1236,PC,D1.W)",     073, 0010440, 0x1234);
    TIDX("(*+$1234567A,PC,D1.W)", 073, 0010460, 0x1234, 0x5678);

    TIDX("([*+2,PC,D1.W])",           073, 0010421);
    TIDX("([*+2,PC,D1.W],$34)",       073, 0010422, 0x0034);
    TIDX("([*+2,PC,D1.W],$1234)",     073, 0010422, 0x1234);
    TIDX("([*+2,PC,D1.W],$12345678)", 073, 0010423, 0x1234, 0x5678);
    TIDX("([*+2,PC],D1.W)",           073, 0010425);
    TIDX("([*+2,PC],D1.W,$34)",       073, 0010426, 0x0034);
    TIDX("([*+2,PC],D1.W,$1234)",     073, 0010426, 0x1234);
    TIDX("([*+2,PC],D1.W,$12345678)", 073, 0010427, 0x1234, 0x5678);

    TIDX("([*+$36,PC,D1.W])",           073, 0010441, 0x0034);
    TIDX("([*+$36,PC,D1.W],$78)",       073, 0010442, 0x0034, 0x0078);
    TIDX("([*+$36,PC,D1.W],$5678)",     073, 0010442, 0x0034, 0x5678);
    TIDX("([*+$36,PC,D1.W],$56789ABC)", 073, 0010443, 0x0034, 0x5678, 0x9ABC);
    TIDX("([*+$36,PC],D1.W)",           073, 0010445, 0x0034);
    TIDX("([*+$36,PC],D1.W,$78)",       073, 0010446, 0x0034, 0x0078);
    TIDX("([*+$36,PC],D1.W,$5678)",     073, 0010446, 0x0034, 0x5678);
    TIDX("([*+$36,PC],D1.W,$56789ABC)", 073, 0010447, 0x0034, 0x5678, 0x9ABC);

    TIDX("([*+$1236,PC,D1.W])",           073, 0010441, 0x1234);
    TIDX("([*+$1236,PC,D1.W],$5678)",     073, 0010442, 0x1234, 0x5678);
    TIDX("([*+$1236,PC,D1.W],$56789ABC)", 073, 0010443, 0x1234, 0x5678, 0x9ABC);
    TIDX("([*+$1236,PC],D1.W)",           073, 0010445, 0x1234);
    TIDX("([*+$1236,PC],D1.W,$5678)",     073, 0010446, 0x1234, 0x5678);
    TIDX("([*+$1236,PC],D1.W,$56789ABC)", 073, 0010447, 0x1234, 0x5678, 0x9ABC);

    TIDX("([*+$1234567A,PC,D1.W])",           073, 0010461, 0x1234, 0x5678);
    TIDX("([*+$1234567A,PC,D1.W],$54)",       073, 0010462, 0x1234, 0x5678, 0x0054);
    TIDX("([*+$1234567A,PC,D1.W],$7654)",     073, 0010462, 0x1234, 0x5678, 0x7654);
    TIDX("([*+$1234567A,PC,D1.W],$76543210)", 073, 0010463, 0x1234, 0x5678, 0x7654, 0x3210);
    TIDX("([*+$1234567A,PC],D1.W)",           073, 0010465, 0x1234, 0x5678);
    TIDX("([*+$1234567A,PC],D1.W,$54)",       073, 0010466, 0x1234, 0x5678, 0x0054);
    TIDX("([*+$1234567A,PC],D1.W,$7654)",     073, 0010466, 0x1234, 0x5678, 0x7654);
    TIDX("([*+$1234567A,PC],D1.W,$76543210)", 073, 0010467, 0x1234, 0x5678, 0x7654, 0x3210);

    TIDX("(*+2,PC)",         072, 0x0000);
    TIDX("(*+$36,PC)",       072, 0x0034);
    TIDX("(*+$1236,PC)",     072, 0x1234);
    TIDX("(*+$1234567A,PC)", 073, 0000560, 0x1234, 0x5678);

    TIDX("([*+2,PC])",           073, 0000521);
    TIDX("([*+2,PC],$34)",       073, 0000522, 0x0034);
    TIDX("([*+2,PC],$1234)",     073, 0000522, 0x1234);
    TIDX("([*+2,PC],$12345678)", 073, 0000523, 0x1234, 0x5678);

    TIDX("([*+$36,PC])",           073, 0000541, 0x0034);
    TIDX("([*+$36,PC],$78)",       073, 0000542, 0x0034, 0x0078);
    TIDX("([*+$36,PC],$5678)",     073, 0000542, 0x0034, 0x5678);
    TIDX("([*+$36,PC],$56789ABC)", 073, 0000543, 0x0034, 0x5678, 0x9ABC);

    TIDX("([*+$1236,PC])",           073, 0000541, 0x1234);
    TIDX("([*+$1236,PC],$78)",       073, 0000542, 0x1234, 0x0078);
    TIDX("([*+$1236,PC],$5678)",     073, 0000542, 0x1234, 0x5678);
    TIDX("([*+$1236,PC],$56789ABC)", 073, 0000543, 0x1234, 0x5678, 0x9ABC);

    TIDX("([*+$1234567A,PC])",           073, 0000561, 0x1234, 0x5678);
    TIDX("([*+$1234567A,PC],$54)",       073, 0000562, 0x1234, 0x5678, 0x0054);
    TIDX("([*+$1234567A,PC],$7654)",     073, 0000562, 0x1234, 0x5678, 0x7654);
    TIDX("([*+$1234567A,PC],$76543210)", 073, 0000563, 0x1234, 0x5678, 0x7654, 0x3210);
}

void test_bitfield(){
    // BFTST src{o:w}:   01643|M|Rn, |0|oooooo|0|wwwwww|
    // BFTST src(Do:Dw}: 01643|M|Rn, |1|000ooo|1|000www|
    TEST("BFTST D1{31:32}",   0164301, 0003700);
    ERRT("BFTST A2{0:4}", OPERAND_NOT_ALLOWED, "A2{0:4}", 0164300, 0000004);
    TEST("BFTST (A2){1:2}",   0164322, 0000102);
    TEST("BFTST (A2){D1:3}",  0164322, 0004103);
    TEST("BFTST (A2){1:D3}",  0164322, 0000143);
    ERRT("BFTST (A2)+{D1:D3}", OPERAND_NOT_ALLOWED, "(A2)+{D1:D3}", 0164300, 0004143);
    ERRT("BFTST -(A2){D1:D3}", OPERAND_NOT_ALLOWED, "-(A2){D1:D3}", 0164300, 0004143);
    TEST("BFTST ($1234,A2){1:2}",     0164352, 0000102, 0x1234);
    TEST("BFTST (18,A2,D3.W){1:2}",   0164362, 0000102, 0x3012);
    ABSW("BFTST (@W).W{1:2}", "1234", 0164370, 0000102, 0x1234);
    TEST("BFTST ($12346).L{1:2}",     0164371, 0000102, 0x0001, 0x2346);
    TEST("BFTST (*+$1234,PC){1:2}",   0164372, 0000102, 0x1230);
    TEST("BFTST (*+35,PC,D3.L){1:2}", 0164373, 0000102, 0x381F);
    ERRT("BFTST #$1234{1:2}", OPERAND_NOT_ALLOWED, "#$1234{1:2}", 0164300, 0000102);

    // BFEXTU src{o:w},Dn:   01647|M|Rn, 0|Dn|0|oooooo|0|wwwwww|
    // BFEXTU src(Do:Dw},Dn: 01647|M|Rn, 0|Dn|1|000ooo|1|000www|
    TEST("BFEXTU D1{1:2}, D4",     0164701, 0040102);
    ERRT("BFEXTU A2{0:4}, D4", OPERAND_NOT_ALLOWED, "A2{0:4}, D4", 0164700, 0040004);
    TEST("BFEXTU (A2){D1:3}, D4",  0164722, 0044103);
    ERRT("BFEXTU (A2)+{D1:D3}, D4", OPERAND_NOT_ALLOWED, "(A2)+{D1:D3}, D4", 0164700, 0044143);
    ERRT("BFEXTU -(A2){D1:D3}, D4", OPERAND_NOT_ALLOWED, "-(A2){D1:D3}, D4", 0164700, 0044143);
    TEST("BFEXTU ($1234,A2){1:2}, D4",       0164752, 0040102, 0x1234);
    TEST("BFEXTU (18,A2,D3.W){1:2}, D4",     0164762, 0040102, 0x3012);
    ABSW("BFEXTU (@W).W{1:2}, D4", "1234",   0164770, 0040102, 0x1234);
    TEST("BFEXTU ($12346).L{1:2}, D4",       0164771, 0040102, 0x0001, 0x2346);
    TEST("BFEXTU (*+$1234,PC){1:2}, D4",     0164772, 0040102, 0x1230);
    TEST("BFEXTU (*+35,PC,D3.L){1:2}, D4",   0164773, 0040102, 0x381F);
    ERRT("BFEXTU #$1234{1:2}, D4", OPERAND_NOT_ALLOWED, "#$1234{1:2}, D4", 0164700, 0040102);

    // BFCHG src{o:w}: 01653|M|Rn, |0|oooooo|0|wwwwww|
    TEST("BFCHG D1{1:2}",     0165301, 00102);
    ERRT("BFCHG A2{0:4}", OPERAND_NOT_ALLOWED, "A2{0:4}", 0165300, 0000004);
    TEST("BFCHG (A2){1:D3}",  0165322, 00143);
    ERRT("BFCHG (A2)+{D1:D3}", OPERAND_NOT_ALLOWED, "(A2)+{D1:D3}", 0165300, 0004143);
    ERRT("BFCHG -(A2){D1:D3}", OPERAND_NOT_ALLOWED, "-(A2){D1:D3}", 0165300, 0004143);
    TEST("BFCHG ($1234,A2){1:2}",     0165352, 00102, 0x1234);
    TEST("BFCHG (18,A2,D3.W){1:2}",   0165362, 00102, 0x3012);
    ABSW("BFCHG (@W).W{1:2}", "1234", 0165370, 00102, 0x1234);
    TEST("BFCHG ($12346).L{1:2}",     0165371, 0000102, 0x0001, 0x2346);
    ERRT("BFCHG (*+$1234,PC){1:2}",
         OPERAND_NOT_ALLOWED, "(*+$1234,PC){1:2}",   0165300, 0000102);
    ERRT("BFCHG (*+35,PC,D3.L){1:2}",
         OPERAND_NOT_ALLOWED, "(*+35,PC,D3.L){1:2}", 0165300, 0000102);
    ERRT("BFCHG #$1234{1:2}", OPERAND_NOT_ALLOWED, "#$1234{1:2}", 0165300, 0000102);

    // BFEXTS src{o:w},Dn:   01657|M|Rn, 0|Dn|0|oooooo|0|wwwwww|
    TEST("BFEXTS D1{1:2}, D4",     0165701, 0040102);
    ERRT("BFEXTS A2{0:4}, D4", OPERAND_NOT_ALLOWED, "A2{0:4}, D4", 0165700, 0040004);
    TEST("BFEXTS (A2){D1:D3}, D4", 0165722, 0044143);
    ERRT("BFEXTS (A2)+{D1:D3}, D4", OPERAND_NOT_ALLOWED, "(A2)+{D1:D3}, D4", 0165700, 0044143);
    ERRT("BFEXTS -(A2){D1:D3}, D4", OPERAND_NOT_ALLOWED, "-(A2){D1:D3}, D4", 0165700, 0044143);
    TEST("BFEXTS ($1234,A2){1:2}, D4",     0165752, 0040102, 0x1234);
    TEST("BFEXTS (18,A2,D3.W){1:2}, D4",   0165762, 0040102, 0x3012);
    ABSW("BFEXTS (@W).W{1:2}, D4", "1234", 0165770, 0040102, 0x1234);
    TEST("BFEXTS ($12346).L{1:2}, D4",     0165771, 0040102, 0x0001, 0x2346);
    TEST("BFEXTS (*+$1234,PC){1:2}, D4",   0165772, 0040102, 0x1230);
    TEST("BFEXTS (*+35,PC,D3.L){1:2}, D4", 0165773, 0040102, 0x381F);
    ERRT("BFEXTS #$1234{1:2}, D4", OPERAND_NOT_ALLOWED, "#$1234{1:2}, D4", 0165700, 0040102);

    // BFCLR src{o:w}: 01663|M|Rn, |0|oooooo|0|wwwwww|
    TEST("BFCLR D1{1:2}",     0166301, 00102);
    ERRT("BFCLR A2{0:4}", OPERAND_NOT_ALLOWED, "A2{0:4}", 0166300, 0000004);
    TEST("BFCLR (A2){1:2}",   0166322, 00102);
    ERRT("BFCLR (A2)+{D1:D3}", OPERAND_NOT_ALLOWED, "(A2)+{D1:D3}", 0166300, 0004143);
    ERRT("BFCLR -(A2){D1:D3}", OPERAND_NOT_ALLOWED, "-(A2){D1:D3}", 0166300, 0004143);
    TEST("BFCLR ($1234,A2){1:2}",     0166352, 00102, 0x1234);
    TEST("BFCLR (18,A2,D3.W){1:2}",   0166362, 00102, 0x3012);
    ABSW("BFCLR (@W).W{1:2}", "1234", 0166370, 00102, 0x1234);
    TEST("BFCLR ($12346).L{1:2}",     0166371, 0000102, 0x0001, 0x2346);
    ERRT("BFCLR (*+$1234,PC){1:2}",
         OPERAND_NOT_ALLOWED, "(*+$1234,PC){1:2}",   0166300, 0000102);
    ERRT("BFCLR (*+35,PC,D3.L){1:2}",
         OPERAND_NOT_ALLOWED, "(*+35,PC,D3.L){1:2}", 0166300, 0000102);
    ERRT("BFCLR #$1234{1:2}", OPERAND_NOT_ALLOWED, "#$1234{1:2}", 0166300, 0000102);

    // BFFFO src{o:w},Dn: 01667|M|Rn, 0|Dn|0|oooooo|0|wwwwww|
    TEST("BFFFO D1{1:2}, D4",     0166701, 0040102);
    ERRT("BFFFO A2{0:4}, D4", OPERAND_NOT_ALLOWED, "A2{0:4}, D4", 0166700, 0040004);
    TEST("BFFFO (A2){D1:D3}, D4", 0166722, 0044143);
    ERRT("BFFFO (A2)+{D1:D3}, D4", OPERAND_NOT_ALLOWED, "(A2)+{D1:D3}, D4", 0166700, 0044143);
    ERRT("BFFFO -(A2){D1:D3}, D4", OPERAND_NOT_ALLOWED, "-(A2){D1:D3}, D4", 0166700, 0044143);
    TEST("BFFFO ($1234,A2){1:2}, D4",     0166752, 0040102, 0x1234);
    TEST("BFFFO (18,A2,D3.W){1:2}, D4",   0166762, 0040102, 0x3012);
    ABSW("BFFFO (@W).W{1:2}, D4", "1234", 0166770, 0040102, 0x1234);
    TEST("BFFFO ($12346).L{1:2}, D4",     0166771, 0040102, 0x0001, 0x2346);
    TEST("BFFFO (*+$1234,PC){1:2}, D4",   0166772, 0040102, 0x1230);
    TEST("BFFFO (*+35,PC,D3.L){1:2}, D4", 0166773, 0040102, 0x381F);
    ERRT("BFFFO #$1234{1:2}, D4", OPERAND_NOT_ALLOWED, "#$1234{1:2}, D4", 0166700, 0040102);

    // BFSET src{o:w}: 01673|M|Rn, |0|oooooo|0|wwwwww|
    TEST("BFSET D1{0:32}",    0167301, 00000);
    ERRT("BFSET A2{0:4}", OPERAND_NOT_ALLOWED, "A2{0:4}", 0167300, 0000004);
    TEST("BFSET (A2){31:31}", 0167322, 03737);
    ERRT("BFSET (A2)+{D1:D3}", OPERAND_NOT_ALLOWED, "(A2)+{D1:D3}", 0167300, 0004143);
    ERRT("BFSET -(A2){D1:D3}", OPERAND_NOT_ALLOWED, "-(A2){D1:D3}", 0167300, 0004143);
    TEST("BFSET ($1234,A2){1:2}",     0167352, 00102, 0x1234);
    TEST("BFSET (18,A2,D3.W){1:2}",   0167362, 00102, 0x3012);
    ABSW("BFSET (@W).W{1:2}", "1234", 0167370, 00102, 0x1234);
    TEST("BFSET ($12346).L{1:2}",     0167371, 0000102, 0x0001, 0x2346);
    ERRT("BFSET (*+$1234,PC){1:2}",
         OPERAND_NOT_ALLOWED, "(*+$1234,PC){1:2}",   0167300, 0000102);
    ERRT("BFSET (*+35,PC,D3.L){1:2}",
         OPERAND_NOT_ALLOWED, "(*+35,PC,D3.L){1:2}", 0167300, 0000102);
    ERRT("BFSET #$1234{1:2}", OPERAND_NOT_ALLOWED, "#$1234{1:2}", 0167300, 0000102);

    // BFINS Dn,dst{o:w}: 01677|M|Rn, 0|Dn|0|oooooo|0|wwwwww|
    TEST("BFINS D4, D1{1:2}",     0167701, 0040102);
    ERRT("BFINS D4, A2{0:4}", OPERAND_NOT_ALLOWED, "A2{0:4}", 0167700, 0040004);
    TEST("BFINS D4, (A2){D1:D3}", 0167722, 0044143);
    ERRT("BFINS D4, (A2)+{D1:D3}", OPERAND_NOT_ALLOWED, "(A2)+{D1:D3}", 0167700, 0044143);
    ERRT("BFINS D4, -(A2){D1:D3}", OPERAND_NOT_ALLOWED, "-(A2){D1:D3}", 0167700, 0044143);
    TEST("BFINS D4, ($1234,A2){1:2}",     0167752, 0040102, 0x1234);
    TEST("BFINS D4, (18,A2,D3.W){1:2}",   0167762, 0040102, 0x3012);
    ABSW("BFINS D4, (@W).W{1:2}", "1234", 0167770, 0040102, 0x1234);
    TEST("BFINS D4, ($12346).L{1:2}",     0167771, 0040102, 0x0001, 0x2346);
    ERRT("BFINS D4, (*+$1234,PC){1:2}",
         OPERAND_NOT_ALLOWED, "(*+$1234,PC){1:2}",   0167700, 0040102);
    ERRT("BFINS D4, (*+35,PC,D3.L){1:2}",
         OPERAND_NOT_ALLOWED, "(*+35,PC,D3.L){1:2}", 0167700, 0040102);
    ERRT("BFINS D4, #$1234{1:2}", OPERAND_NOT_ALLOWED, "#$1234{1:2}", 0167700, 0040102);
}

#if !defined(LIBASM_MC68000_NOFPU)

void test_float_move() {
    TEST("FPU ON");

    TEST("FMOVE.X FP0, FP1",            0xF200,     0x0000|(0<<10)|(1<<7));
    ERRT("FMOVE.X FP2",                 OPERAND_NOT_ALLOWED, "FP2");

    TEST("FMOVE.L D2, FP3",             0xF200|002, 0x4000|(0<<10)|(3<<7));
    TEST("FMOVE.S D2, FP3",             0xF200|002, 0x4000|(1<<10)|(3<<7));
    ERRT("FMOVE.X D2, FP3", ILLEGAL_SIZE, "FMOVE.X D2, FP3",
                                        0xF200|002, 0x4000|(2<<10)|(3<<7));
    ERRT("FMOVE.P D2, FP3", ILLEGAL_SIZE, "FMOVE.P D2, FP3",
                                        0xF200|002, 0x4000|(3<<10)|(3<<7));
    TEST("FMOVE.W D2, FP3",             0xF200|002, 0x4000|(4<<10)|(3<<7));
    ERRT("FMOVE.D D2, FP3", ILLEGAL_SIZE, "FMOVE.D D2, FP3",
                                        0xF200|002, 0x4000|(5<<10)|(3<<7));
    TEST("FMOVE.B D2, FP3",             0xF200|002, 0x4000|(6<<10)|(3<<7));
    ERRT("FMOVE.L A4, FP5",             OPERAND_NOT_ALLOWED, "A4, FP5", 0xF200, 0x4280);
    ERRT("FMOVE.S A4, FP5",             OPERAND_NOT_ALLOWED, "A4, FP5", 0xF200, 0x4680);
    TEST("FMOVE.X (A6), FP7",           0xF200|026, 0x4000|(2<<10)|(7<<7));
    TEST("FMOVE.P (A0)+, FP1",          0xF200|030, 0x4000|(3<<10)|(1<<7));
    TEST("FMOVE.W -(A2), FP3",          0xF200|042, 0x4000|(4<<10)|(3<<7));
    TEST("FMOVE.D ($1234,A4), FP5",     0xF200|054, 0x4000|(5<<10)|(5<<7), 0x1234);
    TEST("FMOVE.B ($23,A6,D7.W), FP0",  0xF200|066, 0x4000|(6<<10)|(0<<7), 0x7023);
    TEST("FMOVE.L ($004566).W, FP1",    0xF200|070, 0x4000|(0<<10)|(1<<7), 0x4566);
    TEST("FMOVE.S ($56789A).L, FP2",    0xF200|071, 0x4000|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FMOVE.X (*+$1234,PC), FP3",   0xF200|072, 0x4000|(2<<10)|(3<<7), 0x1230);
    TEST("FMOVE.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x4000|(3<<10)|(5<<7), 0xC856);
    TEST("FMOVE.L #$6789ABCD, FP6",     0xF200|074, 0x4000|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FMOVE.W #$1234, FP2",         0xF200|074, 0x4000|(4<<10)|(2<<7), 0x1234);
    TEST("FMOVE.B #$23, FP4",           0xF200|074, 0x4000|(6<<10)|(4<<7), 0x0023);
    FLTS("FMOVE.S", "#7.88999976E-10, FP7", 0xF200|074, 0x4000|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FMOVE.X", "#-89000000032, FP0",   0xF200|074, 0x4000|(2<<10)|(0<<7),
         0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FMOVE.P", "#9.12E20, FP1",        0xF200|074, 0x4000|(3<<10)|(1<<7),
         0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FMOVE.D", "#-8.25, FP3",          0xF200|074, 0x4000|(5<<10)|(3<<7),
         0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FMOVE.L FP3, D2",             0xF200|002, 0x6000|(0<<10)|(3<<7));
    TEST("FMOVE.W FP3, D2",             0xF200|002, 0x6000|(4<<10)|(3<<7));
    TEST("FMOVE.B FP3, D2",             0xF200|002, 0x6000|(6<<10)|(3<<7));
    TEST("FMOVE.S FP3, D2",             0xF200|002, 0x6000|(1<<10)|(3<<7));
    ERRT("FMOVE.X FP3, D2", ILLEGAL_SIZE, "FMOVE.X FP3, D2",
                                        0xF200|002, 0x6000|(2<<10)|(3<<7));
    ERRT("FMOVE.P FP3, D2", ILLEGAL_SIZE, "FMOVE.P FP3, D2",
                                        0xF200|002, 0x6000|(3<<10)|(3<<7));
    ERRT("FMOVE.D FP3, D2", ILLEGAL_SIZE, "FMOVE.D FP3, D2",
                                        0xF200|002, 0x6000|(5<<10)|(3<<7));
    ERRT("FMOVE.L FP5, A4",             OPERAND_NOT_ALLOWED, "A4", 0xF200, 0x6280);
    ERRT("FMOVE.S FP5, A4",             OPERAND_NOT_ALLOWED, "A4", 0XF200, 0x6680);
    TEST("FMOVE.X FP7, (A6)",           0xF200|026, 0x6000|(2<<10)|(7<<7));
    TEST("FMOVE.P FP1, (A0)+",          0xF200|030, 0x6000|(3<<10)|(1<<7)|17);
    TEST("FMOVE.W FP3, -(A2)",          0xF200|042, 0x6000|(4<<10)|(3<<7));
    TEST("FMOVE.D FP5, ($1234,A4)",     0xF200|054, 0x6000|(5<<10)|(5<<7), 0x1234);
    TEST("FMOVE.B FP0, ($23,A6,D7.W)",  0xF200|066, 0x6000|(6<<10)|(0<<7), 0x7023);
    TEST("FMOVE.L FP1, ($004566).W",    0xF200|070, 0x6000|(0<<10)|(1<<7), 0x4566);
    TEST("FMOVE.S FP2, ($56789A).L",    0xF200|071, 0x6000|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FMOVE.P FP7, (A6){D1}",           0xF200|026, 0x7000|(3<<10)|(7<<7)|(1<<4));
    TEST("FMOVE.P FP1, (A0)+{D2}",          0xF200|030, 0x7000|(3<<10)|(1<<7)|(2<<4));
    TEST("FMOVE.P FP3, -(A2){D3}",          0xF200|042, 0x7000|(3<<10)|(3<<7)|(3<<4));
    TEST("FMOVE.P FP5, ($1234,A4){D4}",     0xF200|054, 0x7000|(3<<10)|(5<<7)|(4<<4), 0x1234);
    TEST("FMOVE.P FP0, ($23,A6,D7.W){D5}",  0xF200|066, 0x7000|(3<<10)|(0<<7)|(5<<4), 0x7023);
    TEST("FMOVE.P FP1, ($004566).W{D6}",    0xF200|070, 0x7000|(3<<10)|(1<<7)|(6<<4), 0x4566);
    TEST("FMOVE.P FP2, ($56789A).L{D7}",    0xF200|071, 0x7000|(3<<10)|(2<<7)|(7<<4), 0x0056, 0x789A);
    TEST("FMOVE.P FP7, (A6){#-64}",         0xF200|026, 0x6000|(3<<10)|(7<<7)|(-64 & 0x7F));
    TEST("FMOVE.P FP1, (A0)+{#63}",         0xF200|030, 0x6000|(3<<10)|(1<<7)|63);
    ERRT("FMOVE.P FP3, -(A2){#-65}",        OVERFLOW_RANGE, "#-65}",
                                            0xF200|042, 0x6000|(3<<10)|(3<<7)|17);
    ERRT("FMOVE.P FP5, ($1234,A4){#64}",    OVERFLOW_RANGE, "#64}",
                                            0xF200|054, 0x6000|(3<<10)|(5<<7)|17, 0x1234);
    ERUS("FMOVE.P FP0, ($23,A6,D7.W){#UNDEF}", "UNDEF}",
                                            0xF200|066, 0x6000|(3<<10)|(0<<7)|17, 0x7023);
    TEST("FMOVE.P FP1, ($004566).W{#-10}",  0xF200|070, 0x6000|(3<<10)|(1<<7)|(-10 & 0x7F), 0x4566);
    TEST("FMOVE.P FP2, ($56789A).L{#10}",   0xF200|071, 0x6000|(3<<10)|(2<<7)|10, 0x0056, 0x789A);
    ERRT("FMOVE.X FP3, (*+$1234,PC)",    OPERAND_NOT_ALLOWED, "(*+$1234,PC)",   0xF200, 0x6980);
    ERRT("FMOVE.P FP5, (*+90,PC,A4.L)",  OPERAND_NOT_ALLOWED, "(*+90,PC,A4.L)", 0xF200, 0x6E80);
    ERRT("FMOVE.L FP6, #$6789ABCD",      OPERAND_NOT_ALLOWED, "#$6789ABCD",     0xF200, 0x6300);
    ERRT("FMOVE.W FP2, #$1234",          OPERAND_NOT_ALLOWED, "#$1234",         0xF200, 0x7100);
    ERRT("FMOVE.B FP4, #$23",            OPERAND_NOT_ALLOWED, "#$23",           0xF200, 0x7A00);
    ERRT("FMOVE.S FP7, #7.88999976E-10", OPERAND_NOT_ALLOWED, "#7.88999976E-10", 0xF200, 0x6780);
    ERRT("FMOVE.X FP0, #-89000000032",   OPERAND_NOT_ALLOWED, "#-89000000032",  0xF200, 0x6800);
    ERRT("FMOVE.P FP1, #9.12E20",        OPERAND_NOT_ALLOWED, "#9.12E20",       0xF200, 0x6C80);
    ERRT("FMOVE.D FP3, #-8.25",          OPERAND_NOT_ALLOWED, "#-8.25",         0xF200, 0x7580);

    ERRT("FMOVEM.X D0, FP0",             OPERAND_NOT_ALLOWED, "D0, FP0",         0xF200, 0xD080);
    ERRT("FMOVEM.X D0, FP0/FP4-FP7",     OPERAND_NOT_ALLOWED, "D0, FP0/FP4-FP7", 0xF200, 0xD08F);
    ERRT("FMOVEM.X A1, FP0",             OPERAND_NOT_ALLOWED, "A1, FP0",         0xF200, 0xD080);
    ERRT("FMOVEM.X A1, FP0/FP4-FP7",     OPERAND_NOT_ALLOWED, "A1, FP0/FP4-FP7", 0xF200, 0xD08F);
    TEST("FMOVEM.X (A6), FP0",           0xF200|026, 0xD000|0x80);
    TEST("FMOVEM.X (A6), FP0/FP4-FP7",   0xF200|026, 0xD000|0x8F);
    TEST("FMOVEM.X (A0)+, FP7",          0xF200|030, 0xD000|0x01);
    TEST("FMOVEM.X (A0)+, FP0/FP4-FP7",  0xF200|030, 0xD000|0x8F);
    ERRT("FMOVEM.X -(A2), FP3",          OPERAND_NOT_ALLOWED, "-(A2), FP3",         0xF200, 0xD010);
    ERRT("FMOVEM.X -(A2), FP0/FP4-FP7",  OPERAND_NOT_ALLOWED, "-(A2), FP0/FP4-FP7", 0xF200, 0xD08F);
    TEST("FMOVEM.X ($1234,A4), FP3",     0xF200|054, 0xD000|0x10, 0x1234);
    TEST("FMOVEM.X ($1234,A4), FP3-FP5", 0xF200|054, 0xD000|0x1C, 0x1234);
    TEST("FMOVEM.X ($23,A6,D7.W), FP3",  0xF200|066, 0xD000|0x10, 0x7023);
    TEST("FMOVEM.X ($004566).W, FP4",    0xF200|070, 0xD000|0x08, 0x4566);
    TEST("FMOVEM.X ($56789A).L, FP5",    0xF200|071, 0xD000|0x04, 0x0056, 0x789A);
    TEST("FMOVEM.X (*+$1234,PC), FP6",   0xF200|072, 0xD000|0x02, 0x1230);
    TEST("FMOVEM.X (*+90,PC,A4.L), FP7", 0xF200|073, 0xD000|0x01, 0xC856);
    ERRT("FMOVEM.X #$1234, FP3",         OPERAND_NOT_ALLOWED, "#$1234, FP3", 0xF200, 0xD010);

    ERRT("FMOVEM.X D0, D1",             OPERAND_NOT_ALLOWED, "D1",     0xF200, 0xF800);
    ERRT("FMOVEM.X A1, D2",             OPERAND_NOT_ALLOWED, "A1, D2", 0xF200, 0xD820);
    TEST("FMOVEM.X (A6), D1",           0xF200|026, 0xD800|(1<<4));
    TEST("FMOVEM.X (A0)+, D2",          0xF200|030, 0xD800|(2<<4));
    ERRT("FMOVEM.X -(A2), D2",          OPERAND_NOT_ALLOWED, "-(A2), D2", 0xF200, 0xD820);
    TEST("FMOVEM.X ($1234,A4), D3",     0xF200|054, 0xD800|(3<<4), 0x1234);
    TEST("FMOVEM.X ($23,A6,D7.W), D4",  0xF200|066, 0xD800|(4<<4), 0x7023);
    TEST("FMOVEM.X ($004566).W, D5",    0xF200|070, 0xD800|(5<<4), 0x4566);
    TEST("FMOVEM.X ($56789A).L, D6",    0xF200|071, 0xD800|(6<<4), 0x0056, 0x789A);
    TEST("FMOVEM.X (*+$1234,PC), D7",   0xF200|072, 0xD800|(7<<4), 0x1230);
    TEST("FMOVEM.X (*+90,PC,A4.L), D0", 0xF200|073, 0xD800|(0<<0), 0xC856);
    ERRT("FMOVEM.X #$1234, D2",         OPERAND_NOT_ALLOWED, "#$1234, D2", 0xF200, 0xD820);

    ERRT("FMOVEM.X FP0, D7",             OPERAND_NOT_ALLOWED, "D7", 0xF200, 0xF080);
    ERRT("FMOVEM.X FP0-FP3, D7",         OPERAND_NOT_ALLOWED, "D7", 0xF200, 0xF0F0);
    ERRT("FMOVEM.X FP1, A4",             OPERAND_NOT_ALLOWED, "A4", 0xF200, 0xF040);
    ERRT("FMOVEM.X FP1/FP4, A4",         OPERAND_NOT_ALLOWED, "A4", 0xF200, 0xF048);
    TEST("FMOVEM.X FP0/FP6, (A6)",       0xF200|026, 0xF000|0x82);
    TEST("FMOVEM.X FP0/FP4-FP7, (A6)",   0xF200|026, 0xF000|0x8F);
    ERRT("FMOVEM.X FP3, (A3)+",          OPERAND_NOT_ALLOWED, "(A3)+", 0xF200, 0xF010);
    ERRT("FMOVEM.X FP0/FP4-FP7, (A3)+",  OPERAND_NOT_ALLOWED, "(A3)+", 0xF200, 0xF08F);
    TEST("FMOVEM.X FP0/FP4-FP7, -(A2)",  0xF200|042, 0xE000|0xF1);
    TEST("FMOVEM.X FP0, -(A2)",          0xF200|042, 0xE000|0x01);
    TEST("FMOVEM.X FP3-FP5, ($1234,A4)", 0xF200|054, 0xF000|0x1C, 0x1234);
    TEST("FMOVEM.X FP2, ($23,A6,D7.W)",  0xF200|066, 0xF000|0x20, 0x7023);
    TEST("FMOVEM.X FP1, ($004566).W",    0xF200|070, 0xF000|0x40, 0x4566);
    TEST("FMOVEM.X FP0, ($56789A).L",    0xF200|071, 0xF000|0x80, 0x0056, 0x789A);
    ERRT("FMOVEM.X FP3, (*+$1234,PC)",   OPERAND_NOT_ALLOWED, "(*+$1234,PC)",   0xF200, 0xF010);
    ERRT("FMOVEM.X FP3, (*+76,PC,D4.W)", OPERAND_NOT_ALLOWED, "(*+76,PC,D4.W)", 0xF200, 0xF010);
    ERRT("FMOVEM.X FP3, #$1234",         OPERAND_NOT_ALLOWED, "#$1234",         0xF200, 0xF010);

    ERRT("FMOVEM.X D1, D7",             OPERAND_NOT_ALLOWED, "D7", 0xF200, 0xF810);
    ERRT("FMOVEM.X D1, A4",             OPERAND_NOT_ALLOWED, "A4", 0xF200, 0xF810);
    TEST("FMOVEM.X D1, (A6)",           0xF200|026, 0xF800|(1<<4));
    ERRT("FMOVEM.X D1, (A0)+",          OPERAND_NOT_ALLOWED, "(A0)+", 0xF200, 0xF810);
    TEST("FMOVEM.X D2, -(A2)",          0xF200|042, 0xE800|(2<<4));
    TEST("FMOVEM.X D3, ($1234,A4)",     0xF200|054, 0xF800|(3<<4), 0x1234);
    TEST("FMOVEM.X D4, ($23,A6,D7.W)",  0xF200|066, 0xF800|(4<<4), 0x7023);
    TEST("FMOVEM.X D5, ($004566).W",    0xF200|070, 0xF800|(5<<4), 0x4566);
    TEST("FMOVEM.X D6, ($56789A).L",    0xF200|071, 0xF800|(6<<4), 0x0056, 0x789A);
    ERRT("FMOVEM.X D1, (*+$1234,PC)",   OPERAND_NOT_ALLOWED, "(*+$1234,PC)",   0xF200, 0xF810);
    ERRT("FMOVEM.X D1, (*+76,PC,D4.L)", OPERAND_NOT_ALLOWED, "(*+76,PC,D4.L)", 0xF200, 0xF810);
    ERRT("FMOVEM.X D1, #$1234",         OPERAND_NOT_ALLOWED, "#$1234",         0xF200, 0xF810);

    TEST("FMOVE.L D0, FPCR",     0xF200|000, 0x9000);
    TEST("FMOVE.L D1, FPSR",     0xF200|001, 0x8800);
    TEST("FMOVE.L D2, FPIAR",    0xF200|002, 0x8400);
    ERRT("FMOVE.L A3, FPCR",  ILLEGAL_OPERAND_MODE, "A3, FPCR",  0xF200|013, 0x9000);
    ERRT("FMOVE.L A4, FPSR",  ILLEGAL_OPERAND_MODE, "A4, FPSR",  0xF200|014, 0x8800);
    TEST("FMOVE.L A5, FPIAR",    0xF200|015, 0x8400);
    TEST("FMOVE.L (A6), FPCR",   0xF200|026, 0x9000);
    TEST("FMOVE.L (A7), FPSR",   0xF200|027, 0x8800);
    TEST("FMOVE.L (A0), FPIAR",  0xF200|020, 0x8400);
    TEST("FMOVE.L (A1)+, FPCR",  0xF200|031, 0x9000);
    TEST("FMOVE.L (A2)+, FPSR",  0xF200|032, 0x8800);
    TEST("FMOVE.L (A3)+, FPIAR", 0xF200|033, 0x8400);
    TEST("FMOVE.L -(A4), FPCR",  0xF200|044, 0x9000);
    TEST("FMOVE.L -(A5), FPSR",  0xF200|045, 0x8800);
    TEST("FMOVE.L -(A6), FPIAR", 0xF200|046, 0x8400);
    TEST("FMOVE.L ($1234,A7), FPCR",  0xF200|057, 0x9000, 0x1234);
    TEST("FMOVE.L ($2345,A0), FPSR",  0xF200|050, 0x8800, 0x2345);
    TEST("FMOVE.L ($3456,A1), FPIAR", 0xF200|051, 0x8400, 0x3456);
    TEST("FMOVE.L ($45,A2,D1.W), FPCR",  0xF200|062, 0x9000, 0x1045);
    TEST("FMOVE.L ($56,A3,D2.L), FPSR",  0xF200|063, 0x8800, 0x2856);
    TEST("FMOVE.L ($67,A4,A3.W), FPIAR", 0xF200|064, 0x8400, 0xB067);
    TEST("FMOVE.L ($00789A).W, FPCR",  0xF200|070, 0x9000, 0x789A);
    ABSW("FMOVE.L (@W).W, FPSR", "89AA", 0xF200|070, 0x8800, 0x89AA);
    ABSW("FMOVE.L (@W).W, FPIAR", "9ABC", 0xF200|070, 0x8400, 0x9ABC);
    TEST("FMOVE.L ($ABCDEE).L, FPCR",  0xF200|071, 0x9000, 0x00AB, 0xCDEE);
    TEST("FMOVE.L ($BCDEF0).L, FPSR",  0xF200|071, 0x8800, 0x00BC, 0xDEF0);
    TEST("FMOVE.L ($CDEF00).L, FPIAR", 0xF200|071, 0x8400, 0x00CD, 0xEF00);
    TEST("FMOVE.L (*+$1234,PC), FPCR",  0xF200|072, 0x9000, 0x1230);
    TEST("FMOVE.L (*+$2344,PC), FPSR",  0xF200|072, 0x8800, 0x2340);
    TEST("FMOVE.L (*+$3456,PC), FPIAR", 0xF200|072, 0x8400, 0x3452);
    TEST("FMOVE.L (*+73,PC,D1.W), FPCR",   0xF200|073, 0x9000, 0x1045);
    TEST("FMOVE.L (*+90,PC,D2.L), FPSR",   0xF200|073, 0x8800, 0x2856);
    TEST("FMOVE.L (*+107,PC,A3.W), FPIAR", 0xF200|073, 0x8400, 0xB067);
    TEST("FMOVE.L #$12345678, FPCR",  0xF200|074, 0x9000, 0x1234, 0x5678);
    TEST("FMOVE.L #$23456789, FPSR",  0xF200|074, 0x8800, 0x2345, 0x6789);
    TEST("FMOVE.L #$3456789A, FPIAR", 0xF200|074, 0x8400, 0x3456, 0x789A);

    TEST("FMOVE.L FPCR, D0",     0xF200|000, 0xB000);
    TEST("FMOVE.L FPSR, D1",     0xF200|001, 0xA800);
    TEST("FMOVE.L FPIAR, D2",    0xF200|002, 0xA400);
    ERRT("FMOVE.L FPCR, A3",  ILLEGAL_OPERAND_MODE, "A3", 0xF200|013, 0xB000);
    ERRT("FMOVE.L FPSR, A4",  ILLEGAL_OPERAND_MODE, "A4", 0xF200|014, 0xA800);
    TEST("FMOVE.L FPIAR, A5",    0xF200|015, 0xA400);
    TEST("FMOVE.L FPCR, (A6)",   0xF200|026, 0xB000);
    TEST("FMOVE.L FPSR, (A7)",   0xF200|027, 0xA800);
    TEST("FMOVE.L FPIAR, (A0)",  0xF200|020, 0xA400);
    TEST("FMOVE.L FPCR, (A1)+",  0xF200|031, 0xB000);
    TEST("FMOVE.L FPSR, (A2)+",  0xF200|032, 0xA800);
    TEST("FMOVE.L FPIAR, (A3)+", 0xF200|033, 0xA400);
    TEST("FMOVE.L FPCR, -(A4)",  0xF200|044, 0xB000);
    TEST("FMOVE.L FPSR, -(A5)",  0xF200|045, 0xA800);
    TEST("FMOVE.L FPIAR, -(A6)", 0xF200|046, 0xA400);
    TEST("FMOVE.L FPCR, ($1234,A7)",     0xF200|057, 0xB000, 0x1234);
    TEST("FMOVE.L FPSR, ($2345,A0)",     0xF200|050, 0xA800, 0x2345);
    TEST("FMOVE.L FPIAR, ($3456,A1)",    0xF200|051, 0xA400, 0x3456);
    TEST("FMOVE.L FPCR, ($45,A2,D1.W)",  0xF200|062, 0xB000, 0x1045);
    TEST("FMOVE.L FPSR, ($56,A3,D2.L)",  0xF200|063, 0xA800, 0x2856);
    TEST("FMOVE.L FPIAR, ($67,A4,A3.W)", 0xF200|064, 0xA400, 0xB067);
    TEST("FMOVE.L FPCR, ($00789A).W",    0xF200|070, 0xB000, 0x789A);
    ABSW("FMOVE.L FPSR, (@W).W", "89AA", 0xF200|070, 0xA800, 0x89AA);
    ABSW("FMOVE.L FPIAR, (@W).W", "9ABC", 0xF200|070, 0xA400, 0x9ABC);
    TEST("FMOVE.L FPCR, ($ABCDEE).L",    0xF200|071, 0xB000, 0x00AB, 0xCDEE);
    TEST("FMOVE.L FPSR, ($BCDEF0).L",    0xF200|071, 0xA800, 0x00BC, 0xDEF0);
    TEST("FMOVE.L FPIAR, ($CDEF00).L",   0xF200|071, 0xA400, 0x00CD, 0xEF00);
    ERRT("FMOVE.L FPCR, (*+$1234,PC)",   OPERAND_NOT_ALLOWED, "(*+$1234,PC)", 0xF200, 0xB000);
    ERRT("FMOVE.L FPSR, (*+$1234,PC)",   OPERAND_NOT_ALLOWED, "(*+$1234,PC)", 0xF200, 0xA800);
    ERRT("FMOVE.L FPIAR, (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)", 0xF200, 0xA400);
    ERRT("FMOVE.L FPCR, (*+76,PC,D4.W)", OPERAND_NOT_ALLOWED, "(*+76,PC,D4.W)", 0xF200, 0xB000);
    ERRT("FMOVE.L FPSR, (*+76,PC,D4.L)", OPERAND_NOT_ALLOWED, "(*+76,PC,D4.L)", 0xF200, 0xA800);
    ERRT("FMOVE.L FPIAR,(*+76,PC,A4.W)", OPERAND_NOT_ALLOWED, "(*+76,PC,A4.W)", 0xF200, 0xA400);
    ERRT("FMOVE.L FPCR, #$1234",         OPERAND_NOT_ALLOWED, "#$1234", 0xF200, 0xB000);
    ERRT("FMOVE.L FPSR, #$1234",         OPERAND_NOT_ALLOWED, "#$1234", 0xF200, 0xA800);
    ERRT("FMOVE.L FPIAR, #$1234",        OPERAND_NOT_ALLOWED, "#$1234", 0xF200, 0xA400);

    TEST("FMOVEM.L D0, FPCR",       0xF200|000, 0x9000);
    TEST("FMOVEM.L D1, FPSR",       0xF200|001, 0x8800);
    TEST("FMOVEM.L D2, FPIAR",      0xF200|002, 0x8400);
    ERRT("FMOVEM.L A3, FPCR",       ILLEGAL_OPERAND_MODE, "A3, FPCR",  0xF200|013, 0x9000);
    ERRT("FMOVEM.L A4, FPSR",       ILLEGAL_OPERAND_MODE, "A4, FPSR",  0xF200|014, 0x8800);
    TEST("FMOVEM.L A5, FPIAR",      0xF200|015, 0x8400);
    ERRT("FMOVEM.L FPCR/FPSR, D1",  ILLEGAL_OPERAND_MODE, "D1", 0xF200|001, 0xB800);
    ERRT("FMOVEM.L FPSR/FPIAR, A2", ILLEGAL_OPERAND_MODE, "A2", 0xF200|012, 0xAC00);
    TEST("FMOVEM.L FPCR/FPIAR, (A3)",             0xF200|023, 0xB400);
    TEST("FMOVEM.L FPCR/FPSR/FPIAR, (A4)+",       0xF200|034, 0xBC00);
    TEST("FMOVEM.L FPCR/FPSR, -(A5)",             0xF200|045, 0xB800);
    TEST("FMOVEM.L FPSR/FPIAR, ($1234,A6)",       0xF200|056, 0xAC00, 0x1234);
    TEST("FMOVEM.L FPCR/FPIAR, ($34,A7,D2.W)",    0xF200|067, 0xB400, 0x2034);
    TEST("FMOVEM.L FPCR/FPSR/FPIAR, ($005678).W", 0xF200|070, 0xBC00, 0x5678);
    TEST("FMOVEM.L FPCR/FPSR, ($6789AA).L",       0xF200|071, 0xB800, 0x0067, 0x89AA);
    ERRT("FMOVEM.L FPSR/FPIAR, (*+$1234,PC)",   OPERAND_NOT_ALLOWED, "(*+$1234,PC)",   0xF200, 0xAC00);
    ERRT("FMOVEM.L FPCR/FPIAR, (*+76,PC,A4.L)", OPERAND_NOT_ALLOWED, "(*+76,PC,A4.L)", 0xF200, 0xB400);
    ERRT("FMOVEM.L FPCR/FPSR/FPIAR, #$1234",    OPERAND_NOT_ALLOWED, "#$1234",         0xF200, 0xBC00);

    TEST("FMOVEM.L D0, FPCR",       0xF200|000, 0x9000);
    TEST("FMOVEM.L D1, FPSR",       0xF200|001, 0x8800);
    TEST("FMOVEM.L D2, FPIAR",      0xF200|002, 0x8400);
    ERRT("FMOVEM.L A3, FPCR",       ILLEGAL_OPERAND_MODE, "A3, FPCR",  0xF200|013, 0x9000);
    ERRT("FMOVEM.L A4, FPSR",       ILLEGAL_OPERAND_MODE, "A4, FPSR",  0xF200|014, 0x8800);
    TEST("FMOVEM.L A5, FPIAR",      0xF200|015, 0x8400);
    ERRT("FMOVEM.L D1, FPCR/FPSR",  ILLEGAL_OPERAND_MODE, "D1, FPCR/FPSR",  0xF200|001, 0x9800);
    ERRT("FMOVEM.L A2, FPSR/FPIAR", ILLEGAL_OPERAND_MODE, "A2, FPSR/FPIAR", 0xF200|012, 0x8C00);
    TEST("FMOVEM.L (A3), FPCR/FPIAR",             0xF200|023, 0x9400);
    TEST("FMOVEM.L (A4)+, FPCR/FPSR/FPIAR",       0xF200|034, 0x9C00);
    TEST("FMOVEM.L -(A5), FPCR/FPSR",             0xF200|045, 0x9800);
    TEST("FMOVEM.L ($1234,A6), FPSR/FPIAR",       0xF200|056, 0x8C00, 0x1234);
    TEST("FMOVEM.L ($34,A7,D2.W), FPCR/FPIAR",    0xF200|067, 0x9400, 0x2034);
    TEST("FMOVEM.L ($005678).W, FPCR/FPSR/FPIAR", 0xF200|070, 0x9C00, 0x5678);
    TEST("FMOVEM.L ($6789AA).L, FPCR/FPSR",       0xF200|071, 0x9800, 0x0067, 0x89AA);
    TEST("FMOVEM.L (*+$1234,PC), FPSR/FPIAR",     0xF200|072, 0x8C00, 0x1230);
    TEST("FMOVEM.L (*+73,PC,D1.W), FPCR/FPIAR",   0xF200|073, 0x9400, 0x1045);
    ERRT("FMOVEM.L #$12345678, FPCR/FPSR/FPIAR",  ILLEGAL_OPERAND_MODE,
         "#$12345678, FPCR/FPSR/FPIAR", 0xF200|074, 0x9C00, 0x1234, 0x5678);
}

void test_float_common() {
    TEST("FPU ON");

    TEST("FSQRT.X FP0, FP1",            0xF200,     0x0004|(0<<10)|(1<<7));
    TEST("FSQRT.X FP2",                 0xF200,     0x0004|(2<<10)|(2<<7));
    TEST("FSQRT.L D2, FP3",             0xF200|002, 0x4004|(0<<10)|(3<<7));
    TEST("FSQRT.W D2, FP3",             0xF200|002, 0x4004|(4<<10)|(3<<7));
    TEST("FSQRT.B D2, FP3",             0xF200|002, 0x4004|(6<<10)|(3<<7));
    TEST("FSQRT.S D2, FP3",             0xF200|002, 0x4004|(1<<10)|(3<<7));
    ERRT("FSQRT.X D2, FP3", ILLEGAL_SIZE, "FSQRT.X D2, FP3",
                                        0xF200|002, 0x4004|(2<<10)|(3<<7));
    ERRT("FSQRT.P D2, FP3", ILLEGAL_SIZE, "FSQRT.P D2, FP3",
                                        0xF200|002, 0x4004|(3<<10)|(3<<7));
    ERRT("FSQRT.D D2, FP3", ILLEGAL_SIZE, "FSQRT.D D2, FP3",
                                        0xF200|002, 0x4004|(5<<10)|(3<<7));
    ERRT("FSQRT.S A4, FP5",             OPERAND_NOT_ALLOWED, "A4, FP5", 0xF200, 0x4684);
    TEST("FSQRT.X (A6), FP7",           0xF200|026, 0x4004|(2<<10)|(7<<7));
    TEST("FSQRT.P (A0)+, FP1",          0xF200|030, 0x4004|(3<<10)|(1<<7));
    TEST("FSQRT.W -(A2), FP3",          0xF200|042, 0x4004|(4<<10)|(3<<7));
    TEST("FSQRT.D ($1234,A4), FP5",     0xF200|054, 0x4004|(5<<10)|(5<<7), 0x1234);
    TEST("FSQRT.B ($23,A6,D7.W), FP0",  0xF200|066, 0x4004|(6<<10)|(0<<7), 0x7023);
    TEST("FSQRT.L ($004566).W, FP1",    0xF200|070, 0x4004|(0<<10)|(1<<7), 0x4566);
    TEST("FSQRT.S ($56789A).L, FP2",    0xF200|071, 0x4004|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FSQRT.X (*+$1234,PC), FP3",   0xF200|072, 0x4004|(2<<10)|(3<<7), 0x1230);
    TEST("FSQRT.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x4004|(3<<10)|(5<<7), 0xC856);
    TEST("FSQRT.L #$6789ABCD, FP6",     0xF200|074, 0x4004|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FSQRT.W #$1234, FP2",         0xF200|074, 0x4004|(4<<10)|(2<<7), 0x1234);
    TEST("FSQRT.B #$23, FP4",           0xF200|074, 0x4004|(6<<10)|(4<<7), 0x0023);
    FLTS("FSQRT.S", "#7.88999976E-10, FP7", 0xF200|074, 0x4004|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FSQRT.X", "#-89000000032, FP0",   0xF200|074, 0x4004|(2<<10)|(0<<7),
         0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FSQRT.P", "#9.12E20, FP1",        0xF200|074, 0x4004|(3<<10)|(1<<7),
         0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FSQRT.D", "#-8.25, FP3",          0xF200|074, 0x4004|(5<<10)|(3<<7),
         0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FABS.X FP0, FP1",            0xF200,     0x0018|(0<<10)|(1<<7));
    TEST("FABS.X FP2",                 0xF200,     0x0018|(2<<10)|(2<<7));
    TEST("FABS.L D2, FP3",             0xF200|002, 0x4018|(0<<10)|(3<<7));
    TEST("FABS.S D2, FP3",             0xF200|002, 0x4018|(1<<10)|(3<<7));
    ERRT("FABS.X D2, FP3", ILLEGAL_SIZE, "FABS.X D2, FP3",
                                       0xF200|002, 0x4018|(2<<10)|(3<<7));
    ERRT("FABS.P D2, FP3", ILLEGAL_SIZE, "FABS.P D2, FP3",
                                       0xF200|002, 0x4018|(3<<10)|(3<<7));
    TEST("FABS.W D2, FP3",             0xF200|002, 0x4018|(4<<10)|(3<<7));
    ERRT("FABS.D D2, FP3", ILLEGAL_SIZE, "FABS.D D2, FP3",
                                       0xF200|002, 0x4018|(5<<10)|(3<<7));
    TEST("FABS.B D2, FP3",             0xF200|002, 0x4018|(6<<10)|(3<<7));
    ERRT("FABS.S A4, FP5",             OPERAND_NOT_ALLOWED, "A4, FP5", 0xF200, 0x4698);
    TEST("FABS.X (A6), FP7",           0xF200|026, 0x4018|(2<<10)|(7<<7));
    TEST("FABS.P (A0)+, FP1",          0xF200|030, 0x4018|(3<<10)|(1<<7));
    TEST("FABS.W -(A2), FP3",          0xF200|042, 0x4018|(4<<10)|(3<<7));
    TEST("FABS.D ($1234,A4), FP5",     0xF200|054, 0x4018|(5<<10)|(5<<7), 0x1234);
    TEST("FABS.B ($23,A6,D7.W), FP0",  0xF200|066, 0x4018|(6<<10)|(0<<7), 0x7023);
    TEST("FABS.L ($004566).W, FP1",    0xF200|070, 0x4018|(0<<10)|(1<<7), 0x4566);
    TEST("FABS.S ($56789A).L, FP2",    0xF200|071, 0x4018|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FABS.X (*+$1234,PC), FP3",   0xF200|072, 0x4018|(2<<10)|(3<<7), 0x1230);
    TEST("FABS.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x4018|(3<<10)|(5<<7), 0xC856);
    TEST("FABS.L #$6789ABCD, FP6",     0xF200|074, 0x4018|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FABS.W #$1234, FP2",         0xF200|074, 0x4018|(4<<10)|(2<<7), 0x1234);
    TEST("FABS.B #$23, FP4",           0xF200|074, 0x4018|(6<<10)|(4<<7), 0x0023);
    FLTS("FABS.S", "#7.88999976E-10, FP7", 0xF200|074, 0x4018|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FABS.X", "#-89000000032, FP0",   0xF200|074, 0x4018|(2<<10)|(0<<7),
         0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FABS.P", "#9.12E20, FP1",        0xF200|074, 0x4018|(3<<10)|(1<<7),
         0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FABS.D", "#-8.25, FP3",          0xF200|074, 0x4018|(5<<10)|(3<<7),
         0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FNEG.X FP0, FP1",            0xF200,     0x001A|(0<<10)|(1<<7));
    TEST("FNEG.X FP2",                 0xF200,     0x001A|(2<<10)|(2<<7));
    TEST("FNEG.L D2, FP3",             0xF200|002, 0x401A|(0<<10)|(3<<7));
    TEST("FNEG.S D2, FP3",             0xF200|002, 0x401A|(1<<10)|(3<<7));
    ERRT("FNEG.X D2, FP3", ILLEGAL_SIZE, "FNEG.X D2, FP3",
                                       0xF200|002, 0x401A|(2<<10)|(3<<7));
    ERRT("FNEG.P D2, FP3", ILLEGAL_SIZE, "FNEG.P D2, FP3",
                                       0xF200|002, 0x401A|(3<<10)|(3<<7));
    TEST("FNEG.W D2, FP3",             0xF200|002, 0x401A|(4<<10)|(3<<7));
    ERRT("FNEG.D D2, FP3", ILLEGAL_SIZE, "FNEG.D D2, FP3",
                                       0xF200|002, 0x401A|(5<<10)|(3<<7));
    TEST("FNEG.B D2, FP3",             0xF200|002, 0x401A|(6<<10)|(3<<7));
    ERRT("FNEG.S A4, FP5",             OPERAND_NOT_ALLOWED, "A4, FP5", 0xF200, 0x469A);
    TEST("FNEG.X (A6), FP7",           0xF200|026, 0x401A|(2<<10)|(7<<7));
    TEST("FNEG.P (A0)+, FP1",          0xF200|030, 0x401A|(3<<10)|(1<<7));
    TEST("FNEG.W -(A2), FP3",          0xF200|042, 0x401A|(4<<10)|(3<<7));
    TEST("FNEG.D ($1234,A4), FP5",     0xF200|054, 0x401A|(5<<10)|(5<<7), 0x1234);
    TEST("FNEG.B ($23,A6,D7.W), FP0",  0xF200|066, 0x401A|(6<<10)|(0<<7), 0x7023);
    TEST("FNEG.L ($004566).W, FP1",    0xF200|070, 0x401A|(0<<10)|(1<<7), 0x4566);
    TEST("FNEG.S ($56789A).L, FP2",    0xF200|071, 0x401A|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FNEG.X (*+$1234,PC), FP3",   0xF200|072, 0x401A|(2<<10)|(3<<7), 0x1230);
    TEST("FNEG.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x401A|(3<<10)|(5<<7), 0xC856);
    TEST("FNEG.L #$6789ABCD, FP6",     0xF200|074, 0x401A|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FNEG.W #$1234, FP2",         0xF200|074, 0x401A|(4<<10)|(2<<7), 0x1234);
    TEST("FNEG.B #$23, FP4",           0xF200|074, 0x401A|(6<<10)|(4<<7), 0x0023);
    FLTS("FNEG.S", "#7.88999976E-10, FP7", 0xF200|074, 0x401A|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FNEG.X", "#-89000000032, FP0",   0xF200|074, 0x401A|(2<<10)|(0<<7),
         0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FNEG.P", "#9.12E20, FP1",        0xF200|074, 0x401A|(3<<10)|(1<<7),
         0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FNEG.D", "#-8.25, FP3",          0xF200|074, 0x401A|(5<<10)|(3<<7),
         0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FDIV.X FP0, FP1",            0xF200,     0x0020|(0<<10)|(1<<7));
    ERRT("FDIV.X FP0",                 OPERAND_NOT_ALLOWED, "FP0");
    TEST("FDIV.L D2, FP3",             0xF200|002, 0x4020|(0<<10)|(3<<7));
    TEST("FDIV.S D2, FP3",             0xF200|002, 0x4020|(1<<10)|(3<<7));
    ERRT("FDIV.X D2, FP3", ILLEGAL_SIZE, "FDIV.X D2, FP3",
                                       0xF200|002, 0x4020|(2<<10)|(3<<7));
    ERRT("FDIV.P D2, FP3", ILLEGAL_SIZE, "FDIV.P D2, FP3",
                                       0xF200|002, 0x4020|(3<<10)|(3<<7));
    TEST("FDIV.W D2, FP3",             0xF200|002, 0x4020|(4<<10)|(3<<7));
    ERRT("FDIV.D D2, FP3", ILLEGAL_SIZE, "FDIV.D D2, FP3",
                                       0xF200|002, 0x4020|(5<<10)|(3<<7));
    TEST("FDIV.B D2, FP3",             0xF200|002, 0x4020|(6<<10)|(3<<7));
    ERRT("FDIV.S A4, FP5",             OPERAND_NOT_ALLOWED, "A4, FP5", 0xF200, 0x46A0);
    TEST("FDIV.X (A6), FP7",           0xF200|026, 0x4020|(2<<10)|(7<<7));
    TEST("FDIV.P (A0)+, FP1",          0xF200|030, 0x4020|(3<<10)|(1<<7));
    TEST("FDIV.W -(A2), FP3",          0xF200|042, 0x4020|(4<<10)|(3<<7));
    TEST("FDIV.D ($1234,A4), FP5",     0xF200|054, 0x4020|(5<<10)|(5<<7), 0x1234);
    TEST("FDIV.B ($23,A6,D7.W), FP0",  0xF200|066, 0x4020|(6<<10)|(0<<7), 0x7023);
    TEST("FDIV.L ($004566).W, FP1",    0xF200|070, 0x4020|(0<<10)|(1<<7), 0x4566);
    TEST("FDIV.S ($56789A).L, FP2",    0xF200|071, 0x4020|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FDIV.X (*+$1234,PC), FP3",   0xF200|072, 0x4020|(2<<10)|(3<<7), 0x1230);
    TEST("FDIV.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x4020|(3<<10)|(5<<7), 0xC856);
    TEST("FDIV.L #$6789ABCD, FP6",     0xF200|074, 0x4020|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FDIV.W #$1234, FP2",         0xF200|074, 0x4020|(4<<10)|(2<<7), 0x1234);
    TEST("FDIV.B #$23, FP4",           0xF200|074, 0x4020|(6<<10)|(4<<7), 0x0023);
    FLTS("FDIV.S", "#7.88999976E-10, FP7", 0xF200|074, 0x4020|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FDIV.X", "#-89000000032, FP0",   0xF200|074, 0x4020|(2<<10)|(0<<7),
         0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FDIV.P", "#9.12E20, FP1",        0xF200|074, 0x4020|(3<<10)|(1<<7),
         0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FDIV.D", "#-8.25, FP3",          0xF200|074, 0x4020|(5<<10)|(3<<7),
         0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FADD.X FP0, FP1",            0xF200,     0x0022|(0<<10)|(1<<7));
    ERRT("FADD.X FP0",                 OPERAND_NOT_ALLOWED, "FP0");
    TEST("FADD.L D2, FP3",             0xF200|002, 0x4022|(0<<10)|(3<<7));
    TEST("FADD.S D2, FP3",             0xF200|002, 0x4022|(1<<10)|(3<<7));
    ERRT("FADD.X D2, FP3", ILLEGAL_SIZE, "FADD.X D2, FP3",
                                       0xF200|002, 0x4022|(2<<10)|(3<<7));
    ERRT("FADD.P D2, FP3", ILLEGAL_SIZE, "FADD.P D2, FP3",
                                       0xF200|002, 0x4022|(3<<10)|(3<<7));
    TEST("FADD.W D2, FP3",             0xF200|002, 0x4022|(4<<10)|(3<<7));
    ERRT("FADD.D D2, FP3", ILLEGAL_SIZE, "FADD.D D2, FP3",
                                       0xF200|002, 0x4022|(5<<10)|(3<<7));
    TEST("FADD.B D2, FP3",             0xF200|002, 0x4022|(6<<10)|(3<<7));
    ERRT("FADD.S A4, FP5",             OPERAND_NOT_ALLOWED, "A4, FP5", 0xF200, 0x46A2);
    TEST("FADD.X (A6), FP7",           0xF200|026, 0x4022|(2<<10)|(7<<7));
    TEST("FADD.P (A0)+, FP1",          0xF200|030, 0x4022|(3<<10)|(1<<7));
    TEST("FADD.W -(A2), FP3",          0xF200|042, 0x4022|(4<<10)|(3<<7));
    TEST("FADD.D ($1234,A4), FP5",     0xF200|054, 0x4022|(5<<10)|(5<<7), 0x1234);
    TEST("FADD.B ($23,A6,D7.W), FP0",  0xF200|066, 0x4022|(6<<10)|(0<<7), 0x7023);
    TEST("FADD.L ($004566).W, FP1",    0xF200|070, 0x4022|(0<<10)|(1<<7), 0x4566);
    TEST("FADD.S ($56789A).L, FP2",    0xF200|071, 0x4022|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FADD.X (*+$1234,PC), FP3",   0xF200|072, 0x4022|(2<<10)|(3<<7), 0x1230);
    TEST("FADD.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x4022|(3<<10)|(5<<7), 0xC856);
    TEST("FADD.L #$6789ABCD, FP6",     0xF200|074, 0x4022|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FADD.W #$1234, FP2",         0xF200|074, 0x4022|(4<<10)|(2<<7), 0x1234);
    TEST("FADD.B #$23, FP4",           0xF200|074, 0x4022|(6<<10)|(4<<7), 0x0023);
    FLTS("FADD.S", "#7.88999976E-10, FP7", 0xF200|074, 0x4022|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FADD.X", "#-89000000032, FP0",   0xF200|074, 0x4022|(2<<10)|(0<<7),
         0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FADD.P", "#9.12E20, FP1",        0xF200|074, 0x4022|(3<<10)|(1<<7),
         0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FADD.D", "#-8.25, FP3",          0xF200|074, 0x4022|(5<<10)|(3<<7),
         0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FMUL.X FP0, FP1",            0xF200,     0x0023|(0<<10)|(1<<7));
    ERRT("FMUL.X FP0",                 OPERAND_NOT_ALLOWED, "FP0");
    TEST("FMUL.L D2, FP3",             0xF200|002, 0x4023|(0<<10)|(3<<7));
    TEST("FMUL.S D2, FP3",             0xF200|002, 0x4023|(1<<10)|(3<<7));
    ERRT("FMUL.X D2, FP3", ILLEGAL_SIZE, "FMUL.X D2, FP3",
                                       0xF200|002, 0x4023|(2<<10)|(3<<7));
    ERRT("FMUL.P D2, FP3", ILLEGAL_SIZE, "FMUL.P D2, FP3",
                                       0xF200|002, 0x4023|(3<<10)|(3<<7));
    TEST("FMUL.W D2, FP3",             0xF200|002, 0x4023|(4<<10)|(3<<7));
    ERRT("FMUL.D D2, FP3", ILLEGAL_SIZE, "FMUL.D D2, FP3",
                                       0xF200|002, 0x4023|(5<<10)|(3<<7));
    TEST("FMUL.B D2, FP3",             0xF200|002, 0x4023|(6<<10)|(3<<7));
    ERRT("FMUL.S A4, FP5",             OPERAND_NOT_ALLOWED, "A4, FP5", 0xF200, 0x46A3);
    TEST("FMUL.X (A6), FP7",           0xF200|026, 0x4023|(2<<10)|(7<<7));
    TEST("FMUL.P (A0)+, FP1",          0xF200|030, 0x4023|(3<<10)|(1<<7));
    TEST("FMUL.W -(A2), FP3",          0xF200|042, 0x4023|(4<<10)|(3<<7));
    TEST("FMUL.D ($1234,A4), FP5",     0xF200|054, 0x4023|(5<<10)|(5<<7), 0x1234);
    TEST("FMUL.B ($23,A6,D7.W), FP0",  0xF200|066, 0x4023|(6<<10)|(0<<7), 0x7023);
    TEST("FMUL.L ($004566).W, FP1",    0xF200|070, 0x4023|(0<<10)|(1<<7), 0x4566);
    TEST("FMUL.S ($56789A).L, FP2",    0xF200|071, 0x4023|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FMUL.X (*+$1234,PC), FP3",   0xF200|072, 0x4023|(2<<10)|(3<<7), 0x1230);
    TEST("FMUL.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x4023|(3<<10)|(5<<7), 0xC856);
    TEST("FMUL.L #$6789ABCD, FP6",     0xF200|074, 0x4023|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FMUL.W #$1234, FP2",         0xF200|074, 0x4023|(4<<10)|(2<<7), 0x1234);
    TEST("FMUL.B #$23, FP4",           0xF200|074, 0x4023|(6<<10)|(4<<7), 0x0023);
    FLTS("FMUL.S", "#7.88999976E-10, FP7", 0xF200|074, 0x4023|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FMUL.X", "#-89000000032, FP0",   0xF200|074, 0x4023|(2<<10)|(0<<7),
         0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FMUL.P", "#9.12E20, FP1",        0xF200|074, 0x4023|(3<<10)|(1<<7),
         0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FMUL.D", "#-8.25, FP3",          0xF200|074, 0x4023|(5<<10)|(3<<7),
         0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FSGLDIV.X FP0, FP1",            0xF200,     0x0024|(0<<10)|(1<<7));
    ERRT("FSGLDIV.X FP0",                 OPERAND_NOT_ALLOWED, "FP0");
    TEST("FSGLDIV.L D2, FP3",             0xF200|002, 0x4024|(0<<10)|(3<<7));
    TEST("FSGLDIV.S D2, FP3",             0xF200|002, 0x4024|(1<<10)|(3<<7));
    ERRT("FSGLDIV.X D2, FP3", ILLEGAL_SIZE, "FSGLDIV.X D2, FP3",
                                          0xF200|002, 0x4024|(2<<10)|(3<<7));
    ERRT("FSGLDIV.P D2, FP3", ILLEGAL_SIZE, "FSGLDIV.P D2, FP3",
                                          0xF200|002, 0x4024|(3<<10)|(3<<7));
    TEST("FSGLDIV.W D2, FP3",             0xF200|002, 0x4024|(4<<10)|(3<<7));
    ERRT("FSGLDIV.D D2, FP3", ILLEGAL_SIZE, "FSGLDIV.D D2, FP3",
                                          0xF200|002, 0x4024|(5<<10)|(3<<7));
    TEST("FSGLDIV.B D2, FP3",             0xF200|002, 0x4024|(6<<10)|(3<<7));
    ERRT("FSGLDIV.S A4, FP5",             OPERAND_NOT_ALLOWED, "A4, FP5", 0xF200, 0x46A4);
    TEST("FSGLDIV.X (A6), FP7",           0xF200|026, 0x4024|(2<<10)|(7<<7));
    TEST("FSGLDIV.P (A0)+, FP1",          0xF200|030, 0x4024|(3<<10)|(1<<7));
    TEST("FSGLDIV.W -(A2), FP3",          0xF200|042, 0x4024|(4<<10)|(3<<7));
    TEST("FSGLDIV.D ($1234,A4), FP5",     0xF200|054, 0x4024|(5<<10)|(5<<7), 0x1234);
    TEST("FSGLDIV.B ($23,A6,D7.W), FP0",  0xF200|066, 0x4024|(6<<10)|(0<<7), 0x7023);
    TEST("FSGLDIV.L ($004566).W, FP1",    0xF200|070, 0x4024|(0<<10)|(1<<7), 0x4566);
    TEST("FSGLDIV.S ($56789A).L, FP2",    0xF200|071, 0x4024|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FSGLDIV.X (*+$1234,PC), FP3",   0xF200|072, 0x4024|(2<<10)|(3<<7), 0x1230);
    TEST("FSGLDIV.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x4024|(3<<10)|(5<<7), 0xC856);
    TEST("FSGLDIV.L #$6789ABCD, FP6",     0xF200|074, 0x4024|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FSGLDIV.W #$1234, FP2",         0xF200|074, 0x4024|(4<<10)|(2<<7), 0x1234);
    TEST("FSGLDIV.B #$23, FP4",           0xF200|074, 0x4024|(6<<10)|(4<<7), 0x0023);
    FLTS("FSGLDIV.S", "#7.88999976E-10, FP7", 0xF200|074, 0x4024|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FSGLDIV.X", "#-89000000032, FP0",   0xF200|074, 0x4024|(2<<10)|(0<<7),
         0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FSGLDIV.P", "#9.12E20, FP1",        0xF200|074, 0x4024|(3<<10)|(1<<7),
         0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FSGLDIV.D", "#-8.25, FP3",          0xF200|074, 0x4024|(5<<10)|(3<<7),
         0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FSGLMUL.X FP0, FP1",            0xF200,     0x0027|(0<<10)|(1<<7));
    ERRT("FSGLMUL.X FP0",                 OPERAND_NOT_ALLOWED, "FP0");
    TEST("FSGLMUL.L D2, FP3",             0xF200|002, 0x4027|(0<<10)|(3<<7));
    TEST("FSGLMUL.S D2, FP3",             0xF200|002, 0x4027|(1<<10)|(3<<7));
    ERRT("FSGLMUL.X D2, FP3", ILLEGAL_SIZE, "FSGLMUL.X D2, FP3",
                                          0xF200|002, 0x4027|(2<<10)|(3<<7));
    ERRT("FSGLMUL.P D2, FP3", ILLEGAL_SIZE, "FSGLMUL.P D2, FP3",
                                          0xF200|002, 0x4027|(3<<10)|(3<<7));
    TEST("FSGLMUL.W D2, FP3",             0xF200|002, 0x4027|(4<<10)|(3<<7));
    ERRT("FSGLMUL.D D2, FP3", ILLEGAL_SIZE, "FSGLMUL.D D2, FP3",
                                          0xF200|002, 0x4027|(5<<10)|(3<<7));
    TEST("FSGLMUL.B D2, FP3",             0xF200|002, 0x4027|(6<<10)|(3<<7));
    ERRT("FSGLMUL.S A4, FP5",             OPERAND_NOT_ALLOWED, "A4, FP5", 0xF200, 0x46A7);
    TEST("FSGLMUL.X (A6), FP7",           0xF200|026, 0x4027|(2<<10)|(7<<7));
    TEST("FSGLMUL.P (A0)+, FP1",          0xF200|030, 0x4027|(3<<10)|(1<<7));
    TEST("FSGLMUL.W -(A2), FP3",          0xF200|042, 0x4027|(4<<10)|(3<<7));
    TEST("FSGLMUL.D ($1234,A4), FP5",     0xF200|054, 0x4027|(5<<10)|(5<<7), 0x1234);
    TEST("FSGLMUL.B ($23,A6,D7.W), FP0",  0xF200|066, 0x4027|(6<<10)|(0<<7), 0x7023);
    TEST("FSGLMUL.L ($004566).W, FP1",    0xF200|070, 0x4027|(0<<10)|(1<<7), 0x4566);
    TEST("FSGLMUL.S ($56789A).L, FP2",    0xF200|071, 0x4027|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FSGLMUL.X (*+$1234,PC), FP3",   0xF200|072, 0x4027|(2<<10)|(3<<7), 0x1230);
    TEST("FSGLMUL.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x4027|(3<<10)|(5<<7), 0xC856);
    TEST("FSGLMUL.L #$6789ABCD, FP6",     0xF200|074, 0x4027|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FSGLMUL.W #$1234, FP2",         0xF200|074, 0x4027|(4<<10)|(2<<7), 0x1234);
    TEST("FSGLMUL.B #$23, FP4",           0xF200|074, 0x4027|(6<<10)|(4<<7), 0x0023);
    FLTS("FSGLMUL.S", "#7.88999976E-10, FP7", 0xF200|074, 0x4027|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FSGLMUL.X", "#-89000000032, FP0",   0xF200|074, 0x4027|(2<<10)|(0<<7),
         0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FSGLMUL.P", "#9.12E20, FP1",        0xF200|074, 0x4027|(3<<10)|(1<<7),
         0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FSGLMUL.D", "#-8.25, FP3",          0xF200|074, 0x4027|(5<<10)|(3<<7),
         0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FSUB.X FP0, FP1",            0xF200,     0x0028|(0<<10)|(1<<7));
    ERRT("FSUB.X FP0",                 OPERAND_NOT_ALLOWED, "FP0");
    TEST("FSUB.L D2, FP3",             0xF200|002, 0x4028|(0<<10)|(3<<7));
    TEST("FSUB.S D2, FP3",             0xF200|002, 0x4028|(1<<10)|(3<<7));
    ERRT("FSUB.X D2, FP3", ILLEGAL_SIZE, "FSUB.X D2, FP3",
                                       0xF200|002, 0x4028|(2<<10)|(3<<7));
    ERRT("FSUB.P D2, FP3", ILLEGAL_SIZE, "FSUB.P D2, FP3",
                                       0xF200|002, 0x4028|(3<<10)|(3<<7));
    TEST("FSUB.W D2, FP3",             0xF200|002, 0x4028|(4<<10)|(3<<7));
    ERRT("FSUB.D D2, FP3", ILLEGAL_SIZE, "FSUB.D D2, FP3",
                                       0xF200|002, 0x4028|(5<<10)|(3<<7));
    TEST("FSUB.B D2, FP3",             0xF200|002, 0x4028|(6<<10)|(3<<7));
    ERRT("FSUB.S A4, FP5",             OPERAND_NOT_ALLOWED, "A4, FP5", 0xF200, 0x46A8);
    TEST("FSUB.X (A6), FP7",           0xF200|026, 0x4028|(2<<10)|(7<<7));
    TEST("FSUB.P (A0)+, FP1",          0xF200|030, 0x4028|(3<<10)|(1<<7));
    TEST("FSUB.W -(A2), FP3",          0xF200|042, 0x4028|(4<<10)|(3<<7));
    TEST("FSUB.D ($1234,A4), FP5",     0xF200|054, 0x4028|(5<<10)|(5<<7), 0x1234);
    TEST("FSUB.B ($23,A6,D7.W), FP0",  0xF200|066, 0x4028|(6<<10)|(0<<7), 0x7023);
    TEST("FSUB.L ($004566).W, FP1",    0xF200|070, 0x4028|(0<<10)|(1<<7), 0x4566);
    TEST("FSUB.S ($56789A).L, FP2",    0xF200|071, 0x4028|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FSUB.X (*+$1234,PC), FP3",   0xF200|072, 0x4028|(2<<10)|(3<<7), 0x1230);
    TEST("FSUB.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x4028|(3<<10)|(5<<7), 0xC856);
    TEST("FSUB.L #$6789ABCD, FP6",     0xF200|074, 0x4028|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FSUB.W #$1234, FP2",         0xF200|074, 0x4028|(4<<10)|(2<<7), 0x1234);
    TEST("FSUB.B #$23, FP4",           0xF200|074, 0x4028|(6<<10)|(4<<7), 0x0023);
    FLTS("FSUB.S", "#7.88999976E-10, FP7", 0xF200|074, 0x4028|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FSUB.X", "#-89000000032, FP0",   0xF200|074, 0x4028|(2<<10)|(0<<7),
         0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FSUB.P", "#9.12E20, FP1",        0xF200|074, 0x4028|(3<<10)|(1<<7),
         0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FSUB.D", "#-8.25, FP3",          0xF200|074, 0x4028|(5<<10)|(3<<7),
         0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FCMP.X FP0, FP1",            0xF200,     0x0038|(0<<10)|(1<<7));
    ERRT("FCMP.X FP0",                 OPERAND_NOT_ALLOWED, "FP0");
    TEST("FCMP.L D2, FP3",             0xF200|002, 0x4038|(0<<10)|(3<<7));
    TEST("FCMP.S D2, FP3",             0xF200|002, 0x4038|(1<<10)|(3<<7));
    ERRT("FCMP.X D2, FP3", ILLEGAL_SIZE, "FCMP.X D2, FP3",
                                       0xF200|002, 0x4038|(2<<10)|(3<<7));
    ERRT("FCMP.P D2, FP3", ILLEGAL_SIZE, "FCMP.P D2, FP3",
                                       0xF200|002, 0x4038|(3<<10)|(3<<7));
    TEST("FCMP.W D2, FP3",             0xF200|002, 0x4038|(4<<10)|(3<<7));
    ERRT("FCMP.D D2, FP3", ILLEGAL_SIZE, "FCMP.D D2, FP3",
                                       0xF200|002, 0x4038|(5<<10)|(3<<7));
    TEST("FCMP.B D2, FP3",             0xF200|002, 0x4038|(6<<10)|(3<<7));
    ERRT("FCMP.S A4, FP5",             OPERAND_NOT_ALLOWED, "A4, FP5", 0xF200, 0x46B8);
    TEST("FCMP.X (A6), FP7",           0xF200|026, 0x4038|(2<<10)|(7<<7));
    TEST("FCMP.P (A0)+, FP1",          0xF200|030, 0x4038|(3<<10)|(1<<7));
    TEST("FCMP.W -(A2), FP3",          0xF200|042, 0x4038|(4<<10)|(3<<7));
    TEST("FCMP.D ($1234,A4), FP5",     0xF200|054, 0x4038|(5<<10)|(5<<7), 0x1234);
    TEST("FCMP.B ($23,A6,D7.W), FP0",  0xF200|066, 0x4038|(6<<10)|(0<<7), 0x7023);
    TEST("FCMP.L ($004566).W, FP1",    0xF200|070, 0x4038|(0<<10)|(1<<7), 0x4566);
    TEST("FCMP.S ($56789A).L, FP2",    0xF200|071, 0x4038|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FCMP.X (*+$1234,PC), FP3",   0xF200|072, 0x4038|(2<<10)|(3<<7), 0x1230);
    TEST("FCMP.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x4038|(3<<10)|(5<<7), 0xC856);
    TEST("FCMP.L #$6789ABCD, FP6",     0xF200|074, 0x4038|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FCMP.W #$1234, FP2",         0xF200|074, 0x4038|(4<<10)|(2<<7), 0x1234);
    TEST("FCMP.B #$23, FP4",           0xF200|074, 0x4038|(6<<10)|(4<<7), 0x0023);
    FLTS("FCMP.S", "#7.88999976E-10, FP7", 0xF200|074, 0x4038|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FCMP.X", "#-89000000032, FP0",   0xF200|074, 0x4038|(2<<10)|(0<<7),
         0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FCMP.P", "#9.12E20, FP1",        0xF200|074, 0x4038|(3<<10)|(1<<7),
         0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FCMP.D", "#-8.25, FP3",          0xF200|074, 0x4038|(5<<10)|(3<<7),
         0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FTST.X FP0",            0xF200,     0x003A|(0<<10));
    ERRT("FTST.X FP0, FP1",       OPERAND_NOT_ALLOWED, "FP0, FP1");
    TEST("FTST.L D2",             0xF200|002, 0x403A|(0<<10));
    TEST("FTST.S D2",             0xF200|002, 0x403A|(1<<10));
    ERRT("FTST.X D2", ILLEGAL_SIZE, "FTST.X D2",
                                  0xF200|002, 0x403A|(2<<10));
    ERRT("FTST.P D2", ILLEGAL_SIZE, "FTST.P D2",
                                  0xF200|002, 0x403A|(3<<10));
    TEST("FTST.W D2",             0xF200|002, 0x403A|(4<<10));
    ERRT("FTST.D D2", ILLEGAL_SIZE, "FTST.D D2",
                                  0xF200|002, 0x403A|(5<<10));
    TEST("FTST.B D2",             0xF200|002, 0x403A|(6<<10));
    ERRT("FTST.S A4",             OPERAND_NOT_ALLOWED, "A4", 0xF200, 0x443A);
    TEST("FTST.X (A6)",           0xF200|026, 0x403A|(2<<10));
    TEST("FTST.P (A0)+",          0xF200|030, 0x403A|(3<<10));
    TEST("FTST.W -(A2)",          0xF200|042, 0x403A|(4<<10));
    TEST("FTST.D ($1234,A4)",     0xF200|054, 0x403A|(5<<10), 0x1234);
    TEST("FTST.B ($23,A6,D7.W)",  0xF200|066, 0x403A|(6<<10), 0x7023);
    TEST("FTST.L ($004566).W",    0xF200|070, 0x403A|(0<<10), 0x4566);
    TEST("FTST.S ($56789A).L",    0xF200|071, 0x403A|(1<<10), 0x0056, 0x789A);
    TEST("FTST.X (*+$1234,PC)",   0xF200|072, 0x403A|(2<<10), 0x1230);
    TEST("FTST.P (*+90,PC,A4.L)", 0xF200|073, 0x403A|(3<<10), 0xC856);
    TEST("FTST.L #$6789ABCD",     0xF200|074, 0x403A|(0<<10), 0x6789, 0xABCD);
    TEST("FTST.W #$1234",         0xF200|074, 0x403A|(4<<10), 0x1234);
    TEST("FTST.B #$23",           0xF200|074, 0x403A|(6<<10), 0x0023);
    FLTS("FTST.S", "#7.88999976E-10", 0xF200|074, 0x403A|(1<<10), 0x3058, 0xE0F0);
    FLTX("FTST.X", "#-89000000032",   0xF200|074, 0x403A|(2<<10),
         0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FTST.P", "#9.12E20",        0xF200|074, 0x403A|(3<<10),
         0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FTST.D", "#-8.25",          0xF200|074, 0x403A|(5<<10),
         0xC020, 0x8000, 0x0000, 0x0000);
}

void test_float_mc68040() {
    TEST("FPU ON");

    TEST("FSMOVE.X FP0, FP1",            0xF200,     0x0040|(0<<10)|(1<<7));
    TEST("FSMOVE.X FP2, FP2",            0xF200,     0x0040|(2<<10)|(2<<7));

    TEST("FSMOVE.L D2, FP3",             0xF200|002, 0x4040|(0<<10)|(3<<7));
    TEST("FSMOVE.S D2, FP3",             0xF200|002, 0x4040|(1<<10)|(3<<7));
    ERRT("FSMOVE.X D2, FP3", ILLEGAL_SIZE,
         "FSMOVE.X D2, FP3",             0xF200|002, 0x4040|(2<<10)|(3<<7));
    ERRT("FSMOVE.P D2, FP3", ILLEGAL_SIZE,
         "FSMOVE.P D2, FP3",             0xF200|002, 0x4040|(3<<10)|(3<<7));
    TEST("FSMOVE.W D2, FP3",             0xF200|002, 0x4040|(4<<10)|(3<<7));
    ERRT("FSMOVE.D D2, FP3", ILLEGAL_SIZE,
         "FSMOVE.D D2, FP3",             0xF200|002, 0x4040|(5<<10)|(3<<7));
    TEST("FSMOVE.B D2, FP3",             0xF200|002, 0x4040|(6<<10)|(3<<7));
    ERRT("FSMOVE.P A4{D3}, FP5", OPERAND_NOT_ALLOWED, "A4{D3}, FP5");
    ERRT("FSMOVE.L A4, FP5",     OPERAND_NOT_ALLOWED,
         "A4, FP5",                      0xF200,     0x4040|(0<<10)|(5<<7));
    ERRT("FSMOVE.S A4, FP5",     OPERAND_NOT_ALLOWED,
         "A4, FP5",                      0xF200,     0x4040|(1<<10)|(5<<7));
    TEST("FSMOVE.X (A6), FP7",           0xF200|026, 0x4040|(2<<10)|(7<<7));
    TEST("FSMOVE.P (A0)+, FP1",          0xF200|030, 0x4040|(3<<10)|(1<<7));
    TEST("FSMOVE.W -(A2), FP3",          0xF200|042, 0x4040|(4<<10)|(3<<7));
    TEST("FSMOVE.D ($1234,A4), FP5",     0xF200|054, 0x4040|(5<<10)|(5<<7), 0x1234);
    TEST("FSMOVE.B ($23,A6,D7.W), FP0",  0xF200|066, 0x4040|(6<<10)|(0<<7), 0x7023);
    TEST("FSMOVE.L ($4566).W, FP1",      0xF200|070, 0x4040|(0<<10)|(1<<7), 0x4566);
    TEST("FSMOVE.S ($56789A).L, FP2",    0xF200|071, 0x4040|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FSMOVE.X (*+$1234,PC), FP3",   0xF200|072, 0x4040|(2<<10)|(3<<7), 0x1230);
    TEST("FSMOVE.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x4040|(3<<10)|(5<<7), 0xC856);
    TEST("FSMOVE.L #$6789ABCD, FP6",     0xF200|074, 0x4040|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FSMOVE.W #$1234, FP2",         0xF200|074, 0x4040|(4<<10)|(2<<7), 0x1234);
    TEST("FSMOVE.B #$23, FP4",           0xF200|074, 0x4040|(6<<10)|(4<<7), 0x0023);
    FLTS("FSMOVE.S", "#7.88999976E-10, FP7",
         0xF200|074, 0x4040|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FSMOVE.X", "#-89000000032, FP0",
         0xF200|074, 0x4040|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FSMOVE.P", "#0.0912, FP1",
         0xF200|074, 0x4040|(3<<10)|(1<<7), 0x4002, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FSMOVE.D", "#-8.25, FP3",
         0xF200|074, 0x4040|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FDMOVE.X FP0, FP1",            0xF200,     0x0044|(0<<10)|(1<<7));
    TEST("FDMOVE.X FP2, FP2",            0xF200,     0x0044|(2<<10)|(2<<7));

    TEST("FDMOVE.L D2, FP3",             0xF200|002, 0x4044|(0<<10)|(3<<7));
    TEST("FDMOVE.S D2, FP3",             0xF200|002, 0x4044|(1<<10)|(3<<7));
    ERRT("FDMOVE.X D2, FP3", ILLEGAL_SIZE,
         "FDMOVE.X D2, FP3",             0xF200|002, 0x4044|(2<<10)|(3<<7));
    ERRT("FDMOVE.P D2, FP3", ILLEGAL_SIZE,
         "FDMOVE.P D2, FP3",             0xF200|002, 0x4044|(3<<10)|(3<<7));
    TEST("FDMOVE.W D2, FP3",             0xF200|002, 0x4044|(4<<10)|(3<<7));
    ERRT("FDMOVE.D D2, FP3", ILLEGAL_SIZE,
         "FDMOVE.D D2, FP3",             0xF200|002, 0x4044|(5<<10)|(3<<7));
    TEST("FDMOVE.B D2, FP3",             0xF200|002, 0x4044|(6<<10)|(3<<7));
    ERRT("FDMOVE.P A4{D3}, FP5", OPERAND_NOT_ALLOWED, "A4{D3}, FP5");
    ERRT("FDMOVE.L A4, FP5", OPERAND_NOT_ALLOWED,
         "A4, FP5",                      0xF200,     0x4044|(0<<10)|(5<<7));
    ERRT("FDMOVE.S A4, FP5", OPERAND_NOT_ALLOWED,
         "A4, FP5",                      0xF200,      0x4044|(1<<10)|(5<<7));
    TEST("FDMOVE.X (A6), FP7",           0xF200|026, 0x4044|(2<<10)|(7<<7));
    TEST("FDMOVE.P (A0)+, FP1",          0xF200|030, 0x4044|(3<<10)|(1<<7));
    TEST("FDMOVE.W -(A2), FP3",          0xF200|042, 0x4044|(4<<10)|(3<<7));
    TEST("FDMOVE.D ($1234,A4), FP5",     0xF200|054, 0x4044|(5<<10)|(5<<7), 0x1234);
    TEST("FDMOVE.B ($23,A6,D7.W), FP0",  0xF200|066, 0x4044|(6<<10)|(0<<7), 0x7023);
    TEST("FDMOVE.L ($4566).W, FP1",      0xF200|070, 0x4044|(0<<10)|(1<<7), 0x4566);
    TEST("FDMOVE.S ($56789A).L, FP2",    0xF200|071, 0x4044|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FDMOVE.X (*+$1234,PC), FP3",   0xF200|072, 0x4044|(2<<10)|(3<<7), 0x1230);
    TEST("FDMOVE.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x4044|(3<<10)|(5<<7), 0xC856);
    TEST("FDMOVE.L #$6789ABCD, FP6",     0xF200|074, 0x4044|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FDMOVE.W #$1234, FP2",         0xF200|074, 0x4044|(4<<10)|(2<<7), 0x1234);
    TEST("FDMOVE.B #$23, FP4",           0xF200|074, 0x4044|(6<<10)|(4<<7), 0x0023);
    FLTS("FDMOVE.S", "#7.88999976E-10, FP7",
         0xF200|074, 0x4044|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FDMOVE.X", "#-89000000032, FP0",
         0xF200|074, 0x4044|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FDMOVE.P", "#0.0912, FP1",
         0xF200|074, 0x4044|(3<<10)|(1<<7), 0x4002, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FDMOVE.D", "#-8.25, FP3",
         0xF200|074, 0x4044|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FSSQRT.X FP0, FP1",            0xF200,     0x0041|(0<<10)|(1<<7));
    TEST("FSSQRT.X FP2",                 0xF200,     0x0041|(2<<10)|(2<<7));
    TEST("FSSQRT.L D2, FP3",             0xF200|002, 0x4041|(0<<10)|(3<<7));
    TEST("FSSQRT.S D2, FP3",             0xF200|002, 0x4041|(1<<10)|(3<<7));
    ERRT("FSSQRT.X D2, FP3", ILLEGAL_SIZE,
         "FSSQRT.X D2, FP3",             0xF200|002, 0x4041|(2<<10)|(3<<7));
    ERRT("FSSQRT.P D2, FP3", ILLEGAL_SIZE,
         "FSSQRT.P D2, FP3",             0xF200|002, 0x4041|(3<<10)|(3<<7));
    TEST("FSSQRT.W D2, FP3",             0xF200|002, 0x4041|(4<<10)|(3<<7));
    ERRT("FSSQRT.D D2, FP3", ILLEGAL_SIZE,
         "FSSQRT.D D2, FP3",             0xF200|002, 0x4041|(5<<10)|(3<<7));
    TEST("FSSQRT.B D2, FP3",             0xF200|002, 0x4041|(6<<10)|(3<<7));
    ERRT("FSSQRT.P D2, FP3", ILLEGAL_SIZE,
         "FSSQRT.P D2, FP3",             0xF200|002, 0x4041|(3<<10)|(3<<7));
    ERRT("FSSQRT.S A4, FP5", OPERAND_NOT_ALLOWED,
         "A4, FP5",                      0xF200,     0x4041|(1<<10)|(5<<7));
    TEST("FSSQRT.X (A6), FP7",           0xF200|026, 0x4041|(2<<10)|(7<<7));
    TEST("FSSQRT.P (A0)+, FP1",          0xF200|030, 0x4041|(3<<10)|(1<<7));
    TEST("FSSQRT.W -(A2), FP3",          0xF200|042, 0x4041|(4<<10)|(3<<7));
    TEST("FSSQRT.D ($1234,A4), FP5",     0xF200|054, 0x4041|(5<<10)|(5<<7), 0x1234);
    TEST("FSSQRT.B ($23,A6,D7.W), FP0",  0xF200|066, 0x4041|(6<<10)|(0<<7), 0x7023);
    TEST("FSSQRT.L ($4566).W, FP1",      0xF200|070, 0x4041|(0<<10)|(1<<7), 0x4566);
    TEST("FSSQRT.S ($56789A).L, FP2",    0xF200|071, 0x4041|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FSSQRT.X (*+$1234,PC), FP3",   0xF200|072, 0x4041|(2<<10)|(3<<7), 0x1230);
    TEST("FSSQRT.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x4041|(3<<10)|(5<<7), 0xC856);
    TEST("FSSQRT.L #$6789ABCD, FP6",     0xF200|074, 0x4041|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FSSQRT.W #$1234, FP2",         0xF200|074, 0x4041|(4<<10)|(2<<7), 0x1234);
    TEST("FSSQRT.B #$23, FP4",           0xF200|074, 0x4041|(6<<10)|(4<<7), 0x0023);
    FLTS("FSSQRT.S", "#7.88999976E-10, FP7",
         0xF200|074, 0x4041|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FSSQRT.X", "#-89000000032, FP0",
         0xF200|074, 0x4041|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FSSQRT.P", "#9.1234567891234567, FP1",
         0xF200|074, 0x4041|(3<<10)|(1<<7), 0x0000, 0x0009, 0x1234, 0x5678, 0x9123, 0x4567);
    FLTD("FSSQRT.D", "#-8.25, FP3",
         0xF200|074, 0x4041|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FDSQRT.X FP0, FP1",            0xF200,     0x0045|(0<<10)|(1<<7));
    TEST("FDSQRT.X FP2",                 0xF200,     0x0045|(2<<10)|(2<<7));
    TEST("FDSQRT.L D2, FP3",             0xF200|002, 0x4045|(0<<10)|(3<<7));
    TEST("FDSQRT.S D2, FP3",             0xF200|002, 0x4045|(1<<10)|(3<<7));
    ERRT("FDSQRT.X D2, FP3", ILLEGAL_SIZE,
         "FDSQRT.X D2, FP3",             0xF200|002, 0x4045|(2<<10)|(3<<7));
    ERRT("FDSQRT.P D2, FP3", ILLEGAL_SIZE,
         "FDSQRT.P D2, FP3",             0xF200|002, 0x4045|(3<<10)|(3<<7));
    TEST("FDSQRT.W D2, FP3",             0xF200|002, 0x4045|(4<<10)|(3<<7));
    ERRT("FDSQRT.D D2, FP3", ILLEGAL_SIZE,
         "FDSQRT.D D2, FP3",             0xF200|002, 0x4045|(5<<10)|(3<<7));
    TEST("FDSQRT.B D2, FP3",             0xF200|002, 0x4045|(6<<10)|(3<<7));
    ERRT("FDSQRT.P D2, FP3", ILLEGAL_SIZE,
         "FDSQRT.P D2, FP3",             0xF200|002, 0x4045|(3<<10)|(3<<7));
    ERRT("FDSQRT.S A4, FP5", OPERAND_NOT_ALLOWED,
         "A4, FP5",                      0xF200,     0x4045|(1<<10)|(5<<7));
    TEST("FDSQRT.X (A6), FP7",           0xF200|026, 0x4045|(2<<10)|(7<<7));
    TEST("FDSQRT.P (A0)+, FP1",          0xF200|030, 0x4045|(3<<10)|(1<<7));
    TEST("FDSQRT.W -(A2), FP3",          0xF200|042, 0x4045|(4<<10)|(3<<7));
    TEST("FDSQRT.D ($1234,A4), FP5",     0xF200|054, 0x4045|(5<<10)|(5<<7), 0x1234);
    TEST("FDSQRT.B ($23,A6,D7.W), FP0",  0xF200|066, 0x4045|(6<<10)|(0<<7), 0x7023);
    TEST("FDSQRT.L ($4566).W, FP1",      0xF200|070, 0x4045|(0<<10)|(1<<7), 0x4566);
    TEST("FDSQRT.S ($56789A).L, FP2",    0xF200|071, 0x4045|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FDSQRT.X (*+$1234,PC), FP3",   0xF200|072, 0x4045|(2<<10)|(3<<7), 0x1230);
    TEST("FDSQRT.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x4045|(3<<10)|(5<<7), 0xC856);
    TEST("FDSQRT.L #$6789ABCD, FP6",     0xF200|074, 0x4045|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FDSQRT.W #$1234, FP2",         0xF200|074, 0x4045|(4<<10)|(2<<7), 0x1234);
    TEST("FDSQRT.B #$23, FP4",           0xF200|074, 0x4045|(6<<10)|(4<<7), 0x0023);
    FLTS("FDSQRT.S", "#7.88999976E-10, FP7",
         0xF200|074, 0x4045|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FDSQRT.X", "#-89000000032, FP0",
         0xF200|074, 0x4045|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FDSQRT.P", "#9.1234567891234567, FP1",
         0xF200|074, 0x4045|(3<<10)|(1<<7), 0x0000, 0x0009, 0x1234, 0x5678, 0x9123, 0x4567);
    FLTD("FDSQRT.D", "#-8.25, FP3",
         0xF200|074, 0x4045|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FSABS.X FP0, FP1",            0xF200,     0x0058|(0<<10)|(1<<7));
    TEST("FSABS.X FP2",                 0xF200,     0x0058|(2<<10)|(2<<7));
    TEST("FSABS.L D2, FP3",             0xF200|002, 0x4058|(0<<10)|(3<<7));
    TEST("FSABS.S D2, FP3",             0xF200|002, 0x4058|(1<<10)|(3<<7));
    ERRT("FSABS.X D2, FP3", ILLEGAL_SIZE,
         "FSABS.X D2, FP3",             0xF200|002, 0x4058|(2<<10)|(3<<7));
    ERRT("FSABS.P D2, FP3", ILLEGAL_SIZE,
         "FSABS.P D2, FP3",             0xF200|002, 0x4058|(3<<10)|(3<<7));
    TEST("FSABS.W D2, FP3",             0xF200|002, 0x4058|(4<<10)|(3<<7));
    ERRT("FSABS.D D2, FP3", ILLEGAL_SIZE,
         "FSABS.D D2, FP3",             0xF200|002, 0x4058|(5<<10)|(3<<7));
    TEST("FSABS.B D2, FP3",             0xF200|002, 0x4058|(6<<10)|(3<<7));
    ERRT("FSABS.P D2, FP3", ILLEGAL_SIZE,
         "FSABS.P D2, FP3",             0xF200|002, 0x4058|(3<<10)|(3<<7));
    ERRT("FSABS.S A4, FP5", OPERAND_NOT_ALLOWED,
         "A4, FP5",                     0xF200,     0x4058|(1<<10)|(5<<7));
    TEST("FSABS.X (A6), FP7",           0xF200|026, 0x4058|(2<<10)|(7<<7));
    TEST("FSABS.P (A0)+, FP1",          0xF200|030, 0x4058|(3<<10)|(1<<7));
    TEST("FSABS.W -(A2), FP3",          0xF200|042, 0x4058|(4<<10)|(3<<7));
    TEST("FSABS.D ($1234,A4), FP5",     0xF200|054, 0x4058|(5<<10)|(5<<7), 0x1234);
    TEST("FSABS.B ($23,A6,D7.W), FP0",  0xF200|066, 0x4058|(6<<10)|(0<<7), 0x7023);
    TEST("FSABS.L ($4566).W, FP1",      0xF200|070, 0x4058|(0<<10)|(1<<7), 0x4566);
    TEST("FSABS.S ($56789A).L, FP2",    0xF200|071, 0x4058|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FSABS.X (*+$1234,PC), FP3",   0xF200|072, 0x4058|(2<<10)|(3<<7), 0x1230);
    TEST("FSABS.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x4058|(3<<10)|(5<<7), 0xC856);
    TEST("FSABS.L #$6789ABCD, FP6",     0xF200|074, 0x4058|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FSABS.W #$1234, FP2",         0xF200|074, 0x4058|(4<<10)|(2<<7), 0x1234);
    TEST("FSABS.B #$23, FP4",           0xF200|074, 0x4058|(6<<10)|(4<<7), 0x0023);
    FLTS("FSABS.S", "#7.88999976E-10, FP7",
         0xF200|074, 0x4058|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FSABS.X", "#-89000000032, FP0",
         0xF200|074, 0x4058|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FSABS.P", "#9.12E+20, FP1",
         0xF200|074, 0x4058|(3<<10)|(1<<7), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FSABS.D", "#-8.25, FP3",
         0xF200|074, 0x4058|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FDABS.X FP0, FP1",            0xF200,     0x005C|(0<<10)|(1<<7));
    TEST("FDABS.X FP2",                 0xF200,     0x005C|(2<<10)|(2<<7));
    TEST("FDABS.L D2, FP3",             0xF200|002, 0x405C|(0<<10)|(3<<7));
    TEST("FDABS.S D2, FP3",             0xF200|002, 0x405C|(1<<10)|(3<<7));
    ERRT("FDABS.X D2, FP3", ILLEGAL_SIZE,
         "FDABS.X D2, FP3",             0xF200|002, 0x405C|(2<<10)|(3<<7));
    ERRT("FDABS.P D2, FP3", ILLEGAL_SIZE,
         "FDABS.P D2, FP3",             0xF200|002, 0x405C|(3<<10)|(3<<7));
    TEST("FDABS.W D2, FP3",             0xF200|002, 0x405C|(4<<10)|(3<<7));
    ERRT("FDABS.D D2, FP3", ILLEGAL_SIZE,
         "FDABS.D D2, FP3",             0xF200|002, 0x405C|(5<<10)|(3<<7));
    TEST("FDABS.B D2, FP3",             0xF200|002, 0x405C|(6<<10)|(3<<7));
    ERRT("FDABS.P D2, FP3", ILLEGAL_SIZE,
         "FDABS.P D2, FP3",             0xF200|002, 0x405C|(3<<10)|(3<<7));
    ERRT("FDABS.S A4, FP5", OPERAND_NOT_ALLOWED,
         "A4, FP5",                     0xF200,      0x405C|(1<<10)|(5<<7));
    TEST("FDABS.X (A6), FP7",           0xF200|026, 0x405C|(2<<10)|(7<<7));
    TEST("FDABS.P (A0)+, FP1",          0xF200|030, 0x405C|(3<<10)|(1<<7));
    TEST("FDABS.W -(A2), FP3",          0xF200|042, 0x405C|(4<<10)|(3<<7));
    TEST("FDABS.D ($1234,A4), FP5",     0xF200|054, 0x405C|(5<<10)|(5<<7), 0x1234);
    TEST("FDABS.B ($23,A6,D7.W), FP0",  0xF200|066, 0x405C|(6<<10)|(0<<7), 0x7023);
    TEST("FDABS.L ($4566).W, FP1",      0xF200|070, 0x405C|(0<<10)|(1<<7), 0x4566);
    TEST("FDABS.S ($56789A).L, FP2",    0xF200|071, 0x405C|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FDABS.X (*+$1234,PC), FP3",   0xF200|072, 0x405C|(2<<10)|(3<<7), 0x1230);
    TEST("FDABS.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x405C|(3<<10)|(5<<7), 0xC856);
    TEST("FDABS.L #$6789ABCD, FP6",     0xF200|074, 0x405C|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FDABS.W #$1234, FP2",         0xF200|074, 0x405C|(4<<10)|(2<<7), 0x1234);
    TEST("FDABS.B #$23, FP4",           0xF200|074, 0x405C|(6<<10)|(4<<7), 0x0023);
    FLTS("FDABS.S", "#7.88999976E-10, FP7",
         0xF200|074, 0x405C|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FDABS.X", "#-89000000032, FP0",
         0xF200|074, 0x405C|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FDABS.P", "#9.12E+20, FP1",
         0xF200|074, 0x405C|(3<<10)|(1<<7), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FDABS.D", "#-8.25, FP3",
         0xF200|074, 0x405C|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FSNEG.X FP0, FP1",            0xF200,     0x005A|(0<<10)|(1<<7));
    TEST("FSNEG.X FP2",                 0xF200,     0x005A|(2<<10)|(2<<7));
    TEST("FSNEG.L D2, FP3",             0xF200|002, 0x405A|(0<<10)|(3<<7));
    TEST("FSNEG.S D2, FP3",             0xF200|002, 0x405A|(1<<10)|(3<<7));
    ERRT("FSNEG.X D2, FP3", ILLEGAL_SIZE,
         "FSNEG.X D2, FP3",             0xF200|002, 0x405A|(2<<10)|(3<<7));
    ERRT("FSNEG.P D2, FP3", ILLEGAL_SIZE,
         "FSNEG.P D2, FP3",             0xF200|002, 0x405A|(3<<10)|(3<<7));
    TEST("FSNEG.W D2, FP3",             0xF200|002, 0x405A|(4<<10)|(3<<7));
    ERRT("FSNEG.D D2, FP3", ILLEGAL_SIZE,
         "FSNEG.D D2, FP3",             0xF200|002, 0x405A|(5<<10)|(3<<7));
    TEST("FSNEG.B D2, FP3",             0xF200|002, 0x405A|(6<<10)|(3<<7));
    ERRT("FSNEG.P D2, FP3", ILLEGAL_SIZE,
         "FSNEG.P D2, FP3",             0xF200|002, 0x405A|(3<<10)|(3<<7));
    ERRT("FSNEG.S A4, FP5", OPERAND_NOT_ALLOWED,
         "A4, FP5",                     0xF200,     0x405A|(1<<10)|(5<<7));
    TEST("FSNEG.X (A6), FP7",           0xF200|026, 0x405A|(2<<10)|(7<<7));
    TEST("FSNEG.P (A0)+, FP1",          0xF200|030, 0x405A|(3<<10)|(1<<7));
    TEST("FSNEG.W -(A2), FP3",          0xF200|042, 0x405A|(4<<10)|(3<<7));
    TEST("FSNEG.D ($1234,A4), FP5",     0xF200|054, 0x405A|(5<<10)|(5<<7), 0x1234);
    TEST("FSNEG.B ($23,A6,D7.W), FP0",  0xF200|066, 0x405A|(6<<10)|(0<<7), 0x7023);
    TEST("FSNEG.L ($4566).W, FP1",      0xF200|070, 0x405A|(0<<10)|(1<<7), 0x4566);
    TEST("FSNEG.S ($56789A).L, FP2",    0xF200|071, 0x405A|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FSNEG.X (*+$1234,PC), FP3",   0xF200|072, 0x405A|(2<<10)|(3<<7), 0x1230);
    TEST("FSNEG.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x405A|(3<<10)|(5<<7), 0xC856);
    TEST("FSNEG.L #$6789ABCD, FP6",     0xF200|074, 0x405A|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FSNEG.W #$1234, FP2",         0xF200|074, 0x405A|(4<<10)|(2<<7), 0x1234);
    TEST("FSNEG.B #$23, FP4",           0xF200|074, 0x405A|(6<<10)|(4<<7), 0x0023);
    FLTS("FSNEG.S", "#7.88999976E-10, FP7",
         0xF200|074, 0x405A|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FSNEG.X", "#-89000000032, FP0",
         0xF200|074, 0x405A|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FSNEG.P", "#9.12E+20, FP1",
         0xF200|074, 0x405A|(3<<10)|(1<<7), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FSNEG.D", "#-8.25, FP3",
         0xF200|074, 0x405A|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FDNEG.X FP0, FP1",            0xF200,     0x005E|(0<<10)|(1<<7));
    TEST("FDNEG.X FP2",                 0xF200,     0x005E|(2<<10)|(2<<7));
    TEST("FDNEG.L D2, FP3",             0xF200|002, 0x405E|(0<<10)|(3<<7));
    TEST("FDNEG.S D2, FP3",             0xF200|002, 0x405E|(1<<10)|(3<<7));
    ERRT("FDNEG.X D2, FP3", ILLEGAL_SIZE,
         "FDNEG.X D2, FP3",             0xF200|002, 0x405E|(2<<10)|(3<<7));
    ERRT("FDNEG.P D2, FP3", ILLEGAL_SIZE,
         "FDNEG.P D2, FP3",             0xF200|002, 0x405E|(3<<10)|(3<<7));
    TEST("FDNEG.W D2, FP3",             0xF200|002, 0x405E|(4<<10)|(3<<7));
    ERRT("FDNEG.D D2, FP3", ILLEGAL_SIZE,
         "FDNEG.D D2, FP3",             0xF200|002, 0x405E|(5<<10)|(3<<7));
    TEST("FDNEG.B D2, FP3",             0xF200|002, 0x405E|(6<<10)|(3<<7));
    ERRT("FDNEG.P D2, FP3", ILLEGAL_SIZE,
         "FDNEG.P D2, FP3",             0xF200|002, 0x405E|(3<<10)|(3<<7));
    ERRT("FDNEG.S A4, FP5", OPERAND_NOT_ALLOWED,
         "A4, FP5",                     0xF200,     0x405E|(1<<10)|(5<<7));
    TEST("FDNEG.X (A6), FP7",           0xF200|026, 0x405E|(2<<10)|(7<<7));
    TEST("FDNEG.P (A0)+, FP1",          0xF200|030, 0x405E|(3<<10)|(1<<7));
    TEST("FDNEG.W -(A2), FP3",          0xF200|042, 0x405E|(4<<10)|(3<<7));
    TEST("FDNEG.D ($1234,A4), FP5",     0xF200|054, 0x405E|(5<<10)|(5<<7), 0x1234);
    TEST("FDNEG.B ($23,A6,D7.W), FP0",  0xF200|066, 0x405E|(6<<10)|(0<<7), 0x7023);
    TEST("FDNEG.L ($4566).W, FP1",      0xF200|070, 0x405E|(0<<10)|(1<<7), 0x4566);
    TEST("FDNEG.S ($56789A).L, FP2",    0xF200|071, 0x405E|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FDNEG.X (*+$1234,PC), FP3",   0xF200|072, 0x405E|(2<<10)|(3<<7), 0x1230);
    TEST("FDNEG.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x405E|(3<<10)|(5<<7), 0xC856);
    TEST("FDNEG.L #$6789ABCD, FP6",     0xF200|074, 0x405E|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FDNEG.W #$1234, FP2",         0xF200|074, 0x405E|(4<<10)|(2<<7), 0x1234);
    TEST("FDNEG.B #$23, FP4",           0xF200|074, 0x405E|(6<<10)|(4<<7), 0x0023);
    FLTS("FDNEG.S", "#7.88999976E-10, FP7",
         0xF200|074, 0x405E|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FDNEG.X", "#-89000000032, FP0",
         0xF200|074, 0x405E|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FDNEG.P", "#9.12E+20, FP1",
         0xF200|074, 0x405E|(3<<10)|(1<<7), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FDNEG.D", "#-8.25, FP3",
         0xF200|074, 0x405E|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FSDIV.X FP0, FP1",            0xF200,     0x0060|(0<<10)|(1<<7));
    TEST("FSDIV.X FP2, FP2",            0xF200,     0x0060|(2<<10)|(2<<7));
    TEST("FSDIV.L D2, FP3",             0xF200|002, 0x4060|(0<<10)|(3<<7));
    TEST("FSDIV.S D2, FP3",             0xF200|002, 0x4060|(1<<10)|(3<<7));
    ERRT("FSDIV.X D2, FP3", ILLEGAL_SIZE,
         "FSDIV.X D2, FP3",             0xF200|002, 0x4060|(2<<10)|(3<<7));
    ERRT("FSDIV.P D2, FP3", ILLEGAL_SIZE,
         "FSDIV.P D2, FP3",             0xF200|002, 0x4060|(3<<10)|(3<<7));
    TEST("FSDIV.W D2, FP3",             0xF200|002, 0x4060|(4<<10)|(3<<7));
    ERRT("FSDIV.D D2, FP3", ILLEGAL_SIZE,
         "FSDIV.D D2, FP3",             0xF200|002, 0x4060|(5<<10)|(3<<7));
    TEST("FSDIV.B D2, FP3",             0xF200|002, 0x4060|(6<<10)|(3<<7));
    ERRT("FSDIV.P D2, FP3", ILLEGAL_SIZE,
         "FSDIV.P D2, FP3",             0xF200|002, 0x4060|(3<<10)|(3<<7));
    ERRT("FSDIV.S A4, FP5", OPERAND_NOT_ALLOWED,
         "A4, FP5",                     0xF200,     0x4060|(1<<10)|(5<<7));
    TEST("FSDIV.X (A6), FP7",           0xF200|026, 0x4060|(2<<10)|(7<<7));
    TEST("FSDIV.P (A0)+, FP1",          0xF200|030, 0x4060|(3<<10)|(1<<7));
    TEST("FSDIV.W -(A2), FP3",          0xF200|042, 0x4060|(4<<10)|(3<<7));
    TEST("FSDIV.D ($1234,A4), FP5",     0xF200|054, 0x4060|(5<<10)|(5<<7), 0x1234);
    TEST("FSDIV.B ($23,A6,D7.W), FP0",  0xF200|066, 0x4060|(6<<10)|(0<<7), 0x7023);
    TEST("FSDIV.L ($4566).W, FP1",      0xF200|070, 0x4060|(0<<10)|(1<<7), 0x4566);
    TEST("FSDIV.S ($56789A).L, FP2",    0xF200|071, 0x4060|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FSDIV.X (*+$1234,PC), FP3",   0xF200|072, 0x4060|(2<<10)|(3<<7), 0x1230);
    TEST("FSDIV.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x4060|(3<<10)|(5<<7), 0xC856);
    TEST("FSDIV.L #$6789ABCD, FP6",     0xF200|074, 0x4060|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FSDIV.W #$1234, FP2",         0xF200|074, 0x4060|(4<<10)|(2<<7), 0x1234);
    TEST("FSDIV.B #$23, FP4",           0xF200|074, 0x4060|(6<<10)|(4<<7), 0x0023);
    FLTS("FSDIV.S", "#7.88999976E-10, FP7",
         0xF200|074, 0x4060|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FSDIV.X", "#-89000000032, FP0",
         0xF200|074, 0x4060|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FSDIV.P", "#9.12E+20, FP1",
         0xF200|074, 0x4060|(3<<10)|(1<<7), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FSDIV.D", "#-8.25, FP3",
         0xF200|074, 0x4060|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FDDIV.X FP0, FP1",            0xF200,     0x0064|(0<<10)|(1<<7));
    TEST("FDDIV.X FP2, FP2",            0xF200,     0x0064|(2<<10)|(2<<7));
    TEST("FDDIV.L D2, FP3",             0xF200|002, 0x4064|(0<<10)|(3<<7));
    TEST("FDDIV.S D2, FP3",             0xF200|002, 0x4064|(1<<10)|(3<<7));
    ERRT("FDDIV.X D2, FP3", ILLEGAL_SIZE,
         "FDDIV.X D2, FP3",             0xF200|002, 0x4064|(2<<10)|(3<<7));
    ERRT("FDDIV.P D2, FP3", ILLEGAL_SIZE,
         "FDDIV.P D2, FP3",             0xF200|002, 0x4064|(3<<10)|(3<<7));
    TEST("FDDIV.W D2, FP3",             0xF200|002, 0x4064|(4<<10)|(3<<7));
    ERRT("FDDIV.D D2, FP3", ILLEGAL_SIZE,
         "FDDIV.D D2, FP3",             0xF200|002, 0x4064|(5<<10)|(3<<7));
    TEST("FDDIV.B D2, FP3",             0xF200|002, 0x4064|(6<<10)|(3<<7));
    ERRT("FDDIV.P D2, FP3", ILLEGAL_SIZE,
         "FDDIV.P D2, FP3",             0xF200|002, 0x4064|(3<<10)|(3<<7));
    ERRT("FDDIV.S A4, FP5", OPERAND_NOT_ALLOWED,
         "A4, FP5",                     0xF200,     0x4064|(1<<10)|(5<<7));
    TEST("FDDIV.X (A6), FP7",           0xF200|026, 0x4064|(2<<10)|(7<<7));
    TEST("FDDIV.P (A0)+, FP1",          0xF200|030, 0x4064|(3<<10)|(1<<7));
    TEST("FDDIV.W -(A2), FP3",          0xF200|042, 0x4064|(4<<10)|(3<<7));
    TEST("FDDIV.D ($1234,A4), FP5",     0xF200|054, 0x4064|(5<<10)|(5<<7), 0x1234);
    TEST("FDDIV.B ($23,A6,D7.W), FP0",  0xF200|066, 0x4064|(6<<10)|(0<<7), 0x7023);
    TEST("FDDIV.L ($4566).W, FP1",      0xF200|070, 0x4064|(0<<10)|(1<<7), 0x4566);
    TEST("FDDIV.S ($56789A).L, FP2",    0xF200|071, 0x4064|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FDDIV.X (*+$1234,PC), FP3",   0xF200|072, 0x4064|(2<<10)|(3<<7), 0x1230);
    TEST("FDDIV.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x4064|(3<<10)|(5<<7), 0xC856);
    TEST("FDDIV.L #$6789ABCD, FP6",     0xF200|074, 0x4064|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FDDIV.W #$1234, FP2",         0xF200|074, 0x4064|(4<<10)|(2<<7), 0x1234);
    TEST("FDDIV.B #$23, FP4",           0xF200|074, 0x4064|(6<<10)|(4<<7), 0x0023);
    FLTS("FDDIV.S", "#7.88999976E-10, FP7",
         0xF200|074, 0x4064|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FDDIV.X", "#-89000000032, FP0",
         0xF200|074, 0x4064|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FDDIV.P", "#9.12E+20, FP1",
         0xF200|074, 0x4064|(3<<10)|(1<<7), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FDDIV.D", "#-8.25, FP3",
         0xF200|074, 0x4064|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FSADD.X FP0, FP1",            0xF200,     0x0062|(0<<10)|(1<<7));
    TEST("FSADD.X FP2, FP2",            0xF200,     0x0062|(2<<10)|(2<<7));
    TEST("FSADD.L D2, FP3",             0xF200|002, 0x4062|(0<<10)|(3<<7));
    TEST("FSADD.S D2, FP3",             0xF200|002, 0x4062|(1<<10)|(3<<7));
    ERRT("FSADD.X D2, FP3", ILLEGAL_SIZE,
         "FSADD.X D2, FP3",             0xF200|002, 0x4062|(2<<10)|(3<<7));
    ERRT("FSADD.P D2, FP3", ILLEGAL_SIZE,
         "FSADD.P D2, FP3",             0xF200|002, 0x4062|(3<<10)|(3<<7));
    TEST("FSADD.W D2, FP3",             0xF200|002, 0x4062|(4<<10)|(3<<7));
    ERRT("FSADD.D D2, FP3", ILLEGAL_SIZE,
         "FSADD.D D2, FP3",             0xF200|002, 0x4062|(5<<10)|(3<<7));
    TEST("FSADD.B D2, FP3",             0xF200|002, 0x4062|(6<<10)|(3<<7));
    ERRT("FSADD.P D2, FP3", ILLEGAL_SIZE,
         "FSADD.P D2, FP3",             0xF200|002, 0x4062|(3<<10)|(3<<7));
    ERRT("FSADD.S A4, FP5", OPERAND_NOT_ALLOWED,
         "A4, FP5",                     0xF200,     0x4062|(1<<10)|(5<<7));
    TEST("FSADD.X (A6), FP7",           0xF200|026, 0x4062|(2<<10)|(7<<7));
    TEST("FSADD.P (A0)+, FP1",          0xF200|030, 0x4062|(3<<10)|(1<<7));
    TEST("FSADD.W -(A2), FP3",          0xF200|042, 0x4062|(4<<10)|(3<<7));
    TEST("FSADD.D ($1234,A4), FP5",     0xF200|054, 0x4062|(5<<10)|(5<<7), 0x1234);
    TEST("FSADD.B ($23,A6,D7.W), FP0",  0xF200|066, 0x4062|(6<<10)|(0<<7), 0x7023);
    TEST("FSADD.L ($4566).W, FP1",      0xF200|070, 0x4062|(0<<10)|(1<<7), 0x4566);
    TEST("FSADD.S ($56789A).L, FP2",    0xF200|071, 0x4062|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FSADD.X (*+$1234,PC), FP3",   0xF200|072, 0x4062|(2<<10)|(3<<7), 0x1230);
    TEST("FSADD.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x4062|(3<<10)|(5<<7), 0xC856);
    TEST("FSADD.L #$6789ABCD, FP6",     0xF200|074, 0x4062|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FSADD.W #$1234, FP2",         0xF200|074, 0x4062|(4<<10)|(2<<7), 0x1234);
    TEST("FSADD.B #$23, FP4",           0xF200|074, 0x4062|(6<<10)|(4<<7), 0x0023);
    FLTS("FSADD.S", "#7.88999976E-10, FP7",
         0xF200|074, 0x4062|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FSADD.X", "#-89000000032, FP0",
         0xF200|074, 0x4062|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FSADD.P", "#9.12E+20, FP1",
         0xF200|074, 0x4062|(3<<10)|(1<<7), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FSADD.D", "#-8.25, FP3",
         0xF200|074, 0x4062|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FDADD.X FP0, FP1",            0xF200,     0x0066|(0<<10)|(1<<7));
    TEST("FDADD.X FP2, FP2",            0xF200,     0x0066|(2<<10)|(2<<7));
    TEST("FDADD.L D2, FP3",             0xF200|002, 0x4066|(0<<10)|(3<<7));
    TEST("FDADD.S D2, FP3",             0xF200|002, 0x4066|(1<<10)|(3<<7));
    ERRT("FDADD.X D2, FP3", ILLEGAL_SIZE,
         "FDADD.X D2, FP3",             0xF200|002, 0x4066|(2<<10)|(3<<7));
    ERRT("FDADD.P D2, FP3", ILLEGAL_SIZE,
         "FDADD.P D2, FP3",             0xF200|002, 0x4066|(3<<10)|(3<<7));
    TEST("FDADD.W D2, FP3",             0xF200|002, 0x4066|(4<<10)|(3<<7));
    ERRT("FDADD.D D2, FP3", ILLEGAL_SIZE,
         "FDADD.D D2, FP3",             0xF200|002, 0x4066|(5<<10)|(3<<7));
    TEST("FDADD.B D2, FP3",             0xF200|002, 0x4066|(6<<10)|(3<<7));
    ERRT("FDADD.P D2, FP3", ILLEGAL_SIZE,
         "FDADD.P D2, FP3",             0xF200|002, 0x4066|(3<<10)|(3<<7));
    ERRT("FDADD.S A4, FP5", OPERAND_NOT_ALLOWED,
         "A4, FP5",                     0xF200,     0x4066|(1<<10)|(5<<7));
    TEST("FDADD.X (A6), FP7",           0xF200|026, 0x4066|(2<<10)|(7<<7));
    TEST("FDADD.P (A0)+, FP1",          0xF200|030, 0x4066|(3<<10)|(1<<7));
    TEST("FDADD.W -(A2), FP3",          0xF200|042, 0x4066|(4<<10)|(3<<7));
    TEST("FDADD.D ($1234,A4), FP5",     0xF200|054, 0x4066|(5<<10)|(5<<7), 0x1234);
    TEST("FDADD.B ($23,A6,D7.W), FP0",  0xF200|066, 0x4066|(6<<10)|(0<<7), 0x7023);
    TEST("FDADD.L ($4566).W, FP1",      0xF200|070, 0x4066|(0<<10)|(1<<7), 0x4566);
    TEST("FDADD.S ($56789A).L, FP2",    0xF200|071, 0x4066|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FDADD.X (*+$1234,PC), FP3",   0xF200|072, 0x4066|(2<<10)|(3<<7), 0x1230);
    TEST("FDADD.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x4066|(3<<10)|(5<<7), 0xC856);
    TEST("FDADD.L #$6789ABCD, FP6",     0xF200|074, 0x4066|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FDADD.W #$1234, FP2",         0xF200|074, 0x4066|(4<<10)|(2<<7), 0x1234);
    TEST("FDADD.B #$23, FP4",           0xF200|074, 0x4066|(6<<10)|(4<<7), 0x0023);
    FLTS("FDADD.S", "#7.88999976E-10, FP7",
         0xF200|074, 0x4066|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FDADD.X", "#-89000000032, FP0",
         0xF200|074, 0x4066|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FDADD.P", "#9.12E+20, FP1",
         0xF200|074, 0x4066|(3<<10)|(1<<7), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FDADD.D", "#-8.25, FP3",
         0xF200|074, 0x4066|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FSMUL.X FP0, FP1",            0xF200,     0x0063|(0<<10)|(1<<7));
    TEST("FSMUL.X FP2, FP2",            0xF200,     0x0063|(2<<10)|(2<<7));
    TEST("FSMUL.L D2, FP3",             0xF200|002, 0x4063|(0<<10)|(3<<7));
    TEST("FSMUL.S D2, FP3",             0xF200|002, 0x4063|(1<<10)|(3<<7));
    ERRT("FSMUL.X D2, FP3", ILLEGAL_SIZE,
         "FSMUL.X D2, FP3",             0xF200|002, 0x4063|(2<<10)|(3<<7));
    ERRT("FSMUL.P D2, FP3", ILLEGAL_SIZE,
         "FSMUL.P D2, FP3",             0xF200|002, 0x4063|(3<<10)|(3<<7));
    TEST("FSMUL.W D2, FP3",             0xF200|002, 0x4063|(4<<10)|(3<<7));
    ERRT("FSMUL.D D2, FP3", ILLEGAL_SIZE,
         "FSMUL.D D2, FP3",             0xF200|002, 0x4063|(5<<10)|(3<<7));
    TEST("FSMUL.B D2, FP3",             0xF200|002, 0x4063|(6<<10)|(3<<7));
    ERRT("FSMUL.P D2, FP3", ILLEGAL_SIZE,
         "FSMUL.P D2, FP3",             0xF200|002, 0x4063|(3<<10)|(3<<7));
    ERRT("FSMUL.S A4, FP5", OPERAND_NOT_ALLOWED,
         "A4, FP5",                     0xF200,     0x4063|(1<<10)|(5<<7));
    TEST("FSMUL.X (A6), FP7",           0xF200|026, 0x4063|(2<<10)|(7<<7));
    TEST("FSMUL.P (A0)+, FP1",          0xF200|030, 0x4063|(3<<10)|(1<<7));
    TEST("FSMUL.W -(A2), FP3",          0xF200|042, 0x4063|(4<<10)|(3<<7));
    TEST("FSMUL.D ($1234,A4), FP5",     0xF200|054, 0x4063|(5<<10)|(5<<7), 0x1234);
    TEST("FSMUL.B ($23,A6,D7.W), FP0",  0xF200|066, 0x4063|(6<<10)|(0<<7), 0x7023);
    TEST("FSMUL.L ($4566).W, FP1",      0xF200|070, 0x4063|(0<<10)|(1<<7), 0x4566);
    TEST("FSMUL.S ($56789A).L, FP2",    0xF200|071, 0x4063|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FSMUL.X (*+$1234,PC), FP3",   0xF200|072, 0x4063|(2<<10)|(3<<7), 0x1230);
    TEST("FSMUL.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x4063|(3<<10)|(5<<7), 0xC856);
    TEST("FSMUL.L #$6789ABCD, FP6",     0xF200|074, 0x4063|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FSMUL.W #$1234, FP2",         0xF200|074, 0x4063|(4<<10)|(2<<7), 0x1234);
    TEST("FSMUL.B #$23, FP4",           0xF200|074, 0x4063|(6<<10)|(4<<7), 0x0023);
    FLTS("FSMUL.S", "#7.88999976E-10, FP7",
         0xF200|074, 0x4063|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FSMUL.X", "#-89000000032, FP0",
         0xF200|074, 0x4063|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FSMUL.P", "#9.12E+20, FP1",
         0xF200|074, 0x4063|(3<<10)|(1<<7), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FSMUL.D", "#-8.25, FP3",
         0xF200|074, 0x4063|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FDMUL.X FP0, FP1",            0xF200,     0x0067|(0<<10)|(1<<7));
    TEST("FDMUL.X FP2, FP2",            0xF200,     0x0067|(2<<10)|(2<<7));
    TEST("FDMUL.L D2, FP3",             0xF200|002, 0x4067|(0<<10)|(3<<7));
    TEST("FDMUL.S D2, FP3",             0xF200|002, 0x4067|(1<<10)|(3<<7));
    ERRT("FDMUL.X D2, FP3", ILLEGAL_SIZE,
         "FDMUL.X D2, FP3",             0xF200|002, 0x4067|(2<<10)|(3<<7));
    ERRT("FDMUL.P D2, FP3", ILLEGAL_SIZE,
         "FDMUL.P D2, FP3",             0xF200|002, 0x4067|(3<<10)|(3<<7));
    TEST("FDMUL.W D2, FP3",             0xF200|002, 0x4067|(4<<10)|(3<<7));
    ERRT("FDMUL.D D2, FP3", ILLEGAL_SIZE,
         "FDMUL.D D2, FP3",             0xF200|002, 0x4067|(5<<10)|(3<<7));
    TEST("FDMUL.B D2, FP3",             0xF200|002, 0x4067|(6<<10)|(3<<7));
    ERRT("FDMUL.P D2, FP3", ILLEGAL_SIZE,
         "FDMUL.P D2, FP3",             0xF200|002, 0x4067|(3<<10)|(3<<7));
    ERRT("FDMUL.S A4, FP5", OPERAND_NOT_ALLOWED,
         "A4, FP5",                     0xF200,     0x4067|(1<<10)|(5<<7));
    TEST("FDMUL.X (A6), FP7",           0xF200|026, 0x4067|(2<<10)|(7<<7));
    TEST("FDMUL.P (A0)+, FP1",          0xF200|030, 0x4067|(3<<10)|(1<<7));
    TEST("FDMUL.W -(A2), FP3",          0xF200|042, 0x4067|(4<<10)|(3<<7));
    TEST("FDMUL.D ($1234,A4), FP5",     0xF200|054, 0x4067|(5<<10)|(5<<7), 0x1234);
    TEST("FDMUL.B ($23,A6,D7.W), FP0",  0xF200|066, 0x4067|(6<<10)|(0<<7), 0x7023);
    TEST("FDMUL.L ($4566).W, FP1",      0xF200|070, 0x4067|(0<<10)|(1<<7), 0x4566);
    TEST("FDMUL.S ($56789A).L, FP2",    0xF200|071, 0x4067|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FDMUL.X (*+$1234,PC), FP3",   0xF200|072, 0x4067|(2<<10)|(3<<7), 0x1230);
    TEST("FDMUL.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x4067|(3<<10)|(5<<7), 0xC856);
    TEST("FDMUL.L #$6789ABCD, FP6",     0xF200|074, 0x4067|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FDMUL.W #$1234, FP2",         0xF200|074, 0x4067|(4<<10)|(2<<7), 0x1234);
    TEST("FDMUL.B #$23, FP4",           0xF200|074, 0x4067|(6<<10)|(4<<7), 0x0023);
    FLTS("FDMUL.S", "#7.88999976E-10, FP7",
         0xF200|074, 0x4067|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FDMUL.X", "#-89000000032, FP0",
         0xF200|074, 0x4067|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FDMUL.P", "#9.12E+20, FP1",
         0xF200|074, 0x4067|(3<<10)|(1<<7), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FDMUL.D", "#-8.25, FP3",
         0xF200|074, 0x4067|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FSSUB.X FP0, FP1",            0xF200,     0x0068|(0<<10)|(1<<7));
    TEST("FSSUB.X FP2, FP2",            0xF200,     0x0068|(2<<10)|(2<<7));
    TEST("FSSUB.L D2, FP3",             0xF200|002, 0x4068|(0<<10)|(3<<7));
    TEST("FSSUB.S D2, FP3",             0xF200|002, 0x4068|(1<<10)|(3<<7));
    ERRT("FSSUB.X D2, FP3", ILLEGAL_SIZE,
         "FSSUB.X D2, FP3",             0xF200|002, 0x4068|(2<<10)|(3<<7));
    ERRT("FSSUB.P D2, FP3", ILLEGAL_SIZE,
         "FSSUB.P D2, FP3",             0xF200|002, 0x4068|(3<<10)|(3<<7));
    TEST("FSSUB.W D2, FP3",             0xF200|002, 0x4068|(4<<10)|(3<<7));
    ERRT("FSSUB.D D2, FP3", ILLEGAL_SIZE,
         "FSSUB.D D2, FP3",             0xF200|002, 0x4068|(5<<10)|(3<<7));
    TEST("FSSUB.B D2, FP3",             0xF200|002, 0x4068|(6<<10)|(3<<7));
    ERRT("FSSUB.P D2, FP3", ILLEGAL_SIZE,
         "FSSUB.P D2, FP3",             0xF200|002, 0x4068|(3<<10)|(3<<7));
    ERRT("FSSUB.S A4, FP5", OPERAND_NOT_ALLOWED,
         "A4, FP5",                     0xF200,     0x4068|(1<<10)|(5<<7));
    TEST("FSSUB.X (A6), FP7",           0xF200|026, 0x4068|(2<<10)|(7<<7));
    TEST("FSSUB.P (A0)+, FP1",          0xF200|030, 0x4068|(3<<10)|(1<<7));
    TEST("FSSUB.W -(A2), FP3",          0xF200|042, 0x4068|(4<<10)|(3<<7));
    TEST("FSSUB.D ($1234,A4), FP5",     0xF200|054, 0x4068|(5<<10)|(5<<7), 0x1234);
    TEST("FSSUB.B ($23,A6,D7.W), FP0",  0xF200|066, 0x4068|(6<<10)|(0<<7), 0x7023);
    TEST("FSSUB.L ($FFFF8566).W, FP1",  0xF200|070, 0x4068|(0<<10)|(1<<7), 0x8566);
    TEST("FSSUB.S ($56789A).L, FP2",    0xF200|071, 0x4068|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FSSUB.X (*+$1234,PC), FP3",   0xF200|072, 0x4068|(2<<10)|(3<<7), 0x1230);
    TEST("FSSUB.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x4068|(3<<10)|(5<<7), 0xC856);
    TEST("FSSUB.L #$6789ABCD, FP6",     0xF200|074, 0x4068|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FSSUB.W #$1234, FP2",         0xF200|074, 0x4068|(4<<10)|(2<<7), 0x1234);
    TEST("FSSUB.B #$23, FP4",           0xF200|074, 0x4068|(6<<10)|(4<<7), 0x0023);
    FLTS("FSSUB.S", "#7.88999976E-10, FP7",
         0xF200|074, 0x4068|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FSSUB.X", "#-89000000032, FP0",
         0xF200|074, 0x4068|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FSSUB.P", "#9.12E+20, FP1",
         0xF200|074, 0x4068|(3<<10)|(1<<7), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FSSUB.D", "#-8.25, FP3",
         0xF200|074, 0x4068|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FDSUB.X FP0, FP1",            0xF200,     0x006C|(0<<10)|(1<<7));
    TEST("FDSUB.X FP2, FP2",            0xF200,     0x006C|(2<<10)|(2<<7));
    TEST("FDSUB.L D2, FP3",             0xF200|002, 0x406C|(0<<10)|(3<<7));
    TEST("FDSUB.S D2, FP3",             0xF200|002, 0x406C|(1<<10)|(3<<7));
    ERRT("FDSUB.X D2, FP3", ILLEGAL_SIZE,
         "FDSUB.X D2, FP3",             0xF200|002, 0x406C|(2<<10)|(3<<7));
    ERRT("FDSUB.P D2, FP3", ILLEGAL_SIZE,
         "FDSUB.P D2, FP3",             0xF200|002, 0x406C|(3<<10)|(3<<7));
    TEST("FDSUB.W D2, FP3",             0xF200|002, 0x406C|(4<<10)|(3<<7));
    ERRT("FDSUB.D D2, FP3", ILLEGAL_SIZE,
         "FDSUB.D D2, FP3",             0xF200|002, 0x406C|(5<<10)|(3<<7));
    TEST("FDSUB.B D2, FP3",             0xF200|002, 0x406C|(6<<10)|(3<<7));
    ERRT("FDSUB.P D2, FP3", ILLEGAL_SIZE,
         "FDSUB.P D2, FP3",             0xF200|002, 0x406C|(3<<10)|(3<<7));
    ERRT("FDSUB.S A4, FP5", OPERAND_NOT_ALLOWED,
         "A4, FP5",                     0xF200,     0x406C|(1<<10)|(5<<7));
    TEST("FDSUB.X (A6), FP7",           0xF200|026, 0x406C|(2<<10)|(7<<7));
    TEST("FDSUB.P (A0)+, FP1",          0xF200|030, 0x406C|(3<<10)|(1<<7));
    TEST("FDSUB.W -(A2), FP3",          0xF200|042, 0x406C|(4<<10)|(3<<7));
    TEST("FDSUB.D ($1234,A4), FP5",     0xF200|054, 0x406C|(5<<10)|(5<<7), 0x1234);
    TEST("FDSUB.B ($23,A6,D7.W), FP0",  0xF200|066, 0x406C|(6<<10)|(0<<7), 0x7023);
    TEST("FDSUB.L ($FFFF8566).W, FP1",  0xF200|070, 0x406C|(0<<10)|(1<<7), 0x8566);
    TEST("FDSUB.S ($56789A).L, FP2",    0xF200|071, 0x406C|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FDSUB.X (*+$1234,PC), FP3",   0xF200|072, 0x406C|(2<<10)|(3<<7), 0x1230);
    TEST("FDSUB.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x406C|(3<<10)|(5<<7), 0xC856);
    TEST("FDSUB.L #$6789ABCD, FP6",     0xF200|074, 0x406C|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FDSUB.W #$1234, FP2",         0xF200|074, 0x406C|(4<<10)|(2<<7), 0x1234);
    TEST("FDSUB.B #$23, FP4",           0xF200|074, 0x406C|(6<<10)|(4<<7), 0x0023);
    FLTS("FDSUB.S", "#7.88999976E-10, FP7",
         0xF200|074, 0x406C|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FDSUB.X", "#-89000000032, FP0",
         0xF200|074, 0x406C|(2<<10)|(0<<7), 0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FDSUB.P", "#9.12E+20, FP1",
         0xF200|074, 0x406C|(3<<10)|(1<<7), 0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FDSUB.D", "#-8.25, FP3",
         0xF200|074, 0x406C|(5<<10)|(3<<7), 0xC020, 0x8000, 0x0000, 0x0000);
}

void test_float_mc68881() {
    TEST("FPU ON");

    TEST("FMOVECR.X #$00, FP0", 0xF200, 0x5C00|(0<<7)|0x00);
    TEST("FMOVECR.X #$10, FP2", 0xF200, 0x5C00|(2<<7)|0x10);
    TEST("FMOVECR.X #$20, FP4", 0xF200, 0x5C00|(4<<7)|0x20);
    TEST("FMOVECR.X #$7F, FP7", 0xF200, 0x5C00|(7<<7)|0x7F);
    ERRT("FMOVECR.X #$81, FP7", OVERFLOW_RANGE, "#$81, FP7", 0xF200, 0x5C00|(7<<7)|0x01);

    TEST("FINT.X FP0, FP1",            0xF200,     0x0001|(0<<10)|(1<<7));
    TEST("FINT.X FP2",                 0xF200,     0x0001|(2<<10)|(2<<7));
    TEST("FINT.L D2, FP3",             0xF200|002, 0x4001|(0<<10)|(3<<7));
    TEST("FINT.S D2, FP3",             0xF200|002, 0x4001|(1<<10)|(3<<7));
    ERRT("FINT.X D2, FP3", ILLEGAL_SIZE, "FINT.X D2, FP3",
         0xF200|002, 0x4001|(2<<10)|(3<<7));
    ERRT("FINT.P D2, FP3", ILLEGAL_SIZE, "FINT.P D2, FP3",
         0xF200|002, 0x4001|(3<<10)|(3<<7));
    TEST("FINT.W D2, FP3",             0xF200|002, 0x4001|(4<<10)|(3<<7));
    ERRT("FINT.D D2, FP3", ILLEGAL_SIZE, "FINT.D D2, FP3",
         0xF200|002, 0x4001|(5<<10)|(3<<7));
    TEST("FINT.B D2, FP3",             0xF200|002, 0x4001|(6<<10)|(3<<7));
    ERRT("FINT.S A4, FP5",             OPERAND_NOT_ALLOWED, "A4, FP5", 0xF200, 0x4681);
    TEST("FINT.X (A6), FP7",           0xF200|026, 0x4001|(2<<10)|(7<<7));
    TEST("FINT.P (A0)+, FP1",          0xF200|030, 0x4001|(3<<10)|(1<<7));
    TEST("FINT.W -(A2), FP3",          0xF200|042, 0x4001|(4<<10)|(3<<7));
    TEST("FINT.D ($1234,A4), FP5",     0xF200|054, 0x4001|(5<<10)|(5<<7), 0x1234);
    TEST("FINT.B ($23,A6,D7.W), FP0",  0xF200|066, 0x4001|(6<<10)|(0<<7), 0x7023);
    TEST("FINT.L ($004566).W, FP1",    0xF200|070, 0x4001|(0<<10)|(1<<7), 0x4566);
    TEST("FINT.S ($56789A).L, FP2",    0xF200|071, 0x4001|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FINT.X (*+$1234,PC), FP3",   0xF200|072, 0x4001|(2<<10)|(3<<7), 0x1230);
    TEST("FINT.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x4001|(3<<10)|(5<<7), 0xC856);
    TEST("FINT.L #$6789ABCD, FP6",     0xF200|074, 0x4001|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FINT.W #$1234, FP2",         0xF200|074, 0x4001|(4<<10)|(2<<7), 0x1234);
    TEST("FINT.B #$23, FP4",           0xF200|074, 0x4001|(6<<10)|(4<<7), 0x0023);
    FLTS("FINT.S", "#7.88999976E-10, FP7", 0xF200|074, 0x4001|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FINT.X", "#-89000000032, FP0",   0xF200|074, 0x4001|(2<<10)|(0<<7),
         0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FINT.P", "#9.12E20, FP1",        0xF200|074, 0x4001|(3<<10)|(1<<7),
         0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FINT.D", "#-8.25, FP3",          0xF200|074, 0x4001|(5<<10)|(3<<7),
         0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FSINH.X FP0, FP1",            0xF200,     0x0002|(0<<10)|(1<<7));
    TEST("FSINH.X FP2",                 0xF200,     0x0002|(2<<10)|(2<<7));
    TEST("FSINH.L D2, FP3",             0xF200|002, 0x4002|(0<<10)|(3<<7));
    TEST("FSINH.S D2, FP3",             0xF200|002, 0x4002|(1<<10)|(3<<7));
    ERRT("FSINH.X D2, FP3", ILLEGAL_SIZE, "FSINH.X D2, FP3",
         0xF200|002, 0x4002|(2<<10)|(3<<7));
    ERRT("FSINH.P D2, FP3", ILLEGAL_SIZE, "FSINH.P D2, FP3",
         0xF200|002, 0x4002|(3<<10)|(3<<7));
    TEST("FSINH.W D2, FP3",             0xF200|002, 0x4002|(4<<10)|(3<<7));
    ERRT("FSINH.D D2, FP3", ILLEGAL_SIZE, "FSINH.D D2, FP3",
         0xF200|002, 0x4002|(5<<10)|(3<<7));
    TEST("FSINH.B D2, FP3",             0xF200|002, 0x4002|(6<<10)|(3<<7));
    ERRT("FSINH.S A4, FP5",             OPERAND_NOT_ALLOWED, "A4, FP5", 0xF200, 0x4682);
    TEST("FSINH.X (A6), FP7",           0xF200|026, 0x4002|(2<<10)|(7<<7));
    TEST("FSINH.P (A0)+, FP1",          0xF200|030, 0x4002|(3<<10)|(1<<7));
    TEST("FSINH.W -(A2), FP3",          0xF200|042, 0x4002|(4<<10)|(3<<7));
    TEST("FSINH.D ($1234,A4), FP5",     0xF200|054, 0x4002|(5<<10)|(5<<7), 0x1234);
    TEST("FSINH.B ($23,A6,D7.W), FP0",  0xF200|066, 0x4002|(6<<10)|(0<<7), 0x7023);
    TEST("FSINH.L ($004566).W, FP1",    0xF200|070, 0x4002|(0<<10)|(1<<7), 0x4566);
    TEST("FSINH.S ($56789A).L, FP2",    0xF200|071, 0x4002|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FSINH.X (*+$1234,PC), FP3",   0xF200|072, 0x4002|(2<<10)|(3<<7), 0x1230);
    TEST("FSINH.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x4002|(3<<10)|(5<<7), 0xC856);
    TEST("FSINH.L #$6789ABCD, FP6",     0xF200|074, 0x4002|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FSINH.W #$1234, FP2",         0xF200|074, 0x4002|(4<<10)|(2<<7), 0x1234);
    TEST("FSINH.B #$23, FP4",           0xF200|074, 0x4002|(6<<10)|(4<<7), 0x0023);
    FLTS("FSINH.S", "#7.88999976E-10, FP7", 0xF200|074, 0x4002|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FSINH.X", "#-8, FP0",             0xF200|074, 0x4002|(2<<10)|(0<<7),
         0xC002, 0x0000, 0x8000, 0x0000, 0x0000, 0x0000);
    FLTP("FSINH.P", "#9.12E20, FP1",        0xF200|074, 0x4002|(3<<10)|(1<<7),
         0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FSINH.D", "#-8.25, FP3",          0xF200|074, 0x4002|(5<<10)|(3<<7),
         0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FINTRZ.X FP0, FP1",            0xF200,     0x0003|(0<<10)|(1<<7));
    TEST("FINTRZ.X FP2",                 0xF200,     0x0003|(2<<10)|(2<<7));
    TEST("FINTRZ.L D2, FP3",             0xF200|002, 0x4003|(0<<10)|(3<<7));
    TEST("FINTRZ.S D2, FP3",             0xF200|002, 0x4003|(1<<10)|(3<<7));
    ERRT("FINTRZ.X D2, FP3", ILLEGAL_SIZE, "FINTRZ.X D2, FP3",
         0xF200|002, 0x4003|(2<<10)|(3<<7));
    ERRT("FINTRZ.P D2, FP3", ILLEGAL_SIZE, "FINTRZ.P D2, FP3",
         0xF200|002, 0x4003|(3<<10)|(3<<7));
    TEST("FINTRZ.W D2, FP3",             0xF200|002, 0x4003|(4<<10)|(3<<7));
    ERRT("FINTRZ.D D2, FP3", ILLEGAL_SIZE, "FINTRZ.D D2, FP3",
         0xF200|002, 0x4003|(5<<10)|(3<<7));
    TEST("FINTRZ.B D2, FP3",             0xF200|002, 0x4003|(6<<10)|(3<<7));
    ERRT("FINTRZ.S A4, FP5",             OPERAND_NOT_ALLOWED, "A4, FP5", 0xF200, 0x4683);
    TEST("FINTRZ.X (A6), FP7",           0xF200|026, 0x4003|(2<<10)|(7<<7));
    TEST("FINTRZ.P (A0)+, FP1",          0xF200|030, 0x4003|(3<<10)|(1<<7));
    TEST("FINTRZ.W -(A2), FP3",          0xF200|042, 0x4003|(4<<10)|(3<<7));
    TEST("FINTRZ.D ($1234,A4), FP5",     0xF200|054, 0x4003|(5<<10)|(5<<7), 0x1234);
    TEST("FINTRZ.B ($23,A6,D7.W), FP0",  0xF200|066, 0x4003|(6<<10)|(0<<7), 0x7023);
    TEST("FINTRZ.L ($004566).W, FP1",    0xF200|070, 0x4003|(0<<10)|(1<<7), 0x4566);
    TEST("FINTRZ.S ($56789A).L, FP2",    0xF200|071, 0x4003|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FINTRZ.X (*+$1234,PC), FP3",   0xF200|072, 0x4003|(2<<10)|(3<<7), 0x1230);
    TEST("FINTRZ.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x4003|(3<<10)|(5<<7), 0xC856);
    TEST("FINTRZ.L #$6789ABCD, FP6",     0xF200|074, 0x4003|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FINTRZ.W #$1234, FP2",         0xF200|074, 0x4003|(4<<10)|(2<<7), 0x1234);
    TEST("FINTRZ.B #$23, FP4",           0xF200|074, 0x4003|(6<<10)|(4<<7), 0x0023);
    FLTS("FINTRZ.S", "#7.88999976E-10, FP7", 0xF200|074, 0x4003|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FINTRZ.X", "#-89000000032, FP0",   0xF200|074, 0x4003|(2<<10)|(0<<7),
         0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FINTRZ.P", "#9.12E20, FP1",        0xF200|074, 0x4003|(3<<10)|(1<<7),
         0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FINTRZ.D", "#-8.25, FP3",          0xF200|074, 0x4003|(5<<10)|(3<<7),
         0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FLOGNP1.X FP0, FP1",            0xF200,     0x0006|(0<<10)|(1<<7));
    TEST("FLOGNP1.X FP2",                 0xF200,     0x0006|(2<<10)|(2<<7));
    TEST("FLOGNP1.L D2, FP3",             0xF200|002, 0x4006|(0<<10)|(3<<7));
    TEST("FLOGNP1.S D2, FP3",             0xF200|002, 0x4006|(1<<10)|(3<<7));
    ERRT("FLOGNP1.X D2, FP3", ILLEGAL_SIZE, "FLOGNP1.X D2, FP3",
         0xF200|002, 0x4006|(2<<10)|(3<<7));
    ERRT("FLOGNP1.P D2, FP3", ILLEGAL_SIZE, "FLOGNP1.P D2, FP3",
         0xF200|002, 0x4006|(3<<10)|(3<<7));
    TEST("FLOGNP1.W D2, FP3",             0xF200|002, 0x4006|(4<<10)|(3<<7));
    ERRT("FLOGNP1.D D2, FP3", ILLEGAL_SIZE, "FLOGNP1.D D2, FP3",
         0xF200|002, 0x4006|(5<<10)|(3<<7));
    TEST("FLOGNP1.B D2, FP3",             0xF200|002, 0x4006|(6<<10)|(3<<7));
    ERRT("FLOGNP1.S A4, FP5",             OPERAND_NOT_ALLOWED, "A4, FP5", 0xF200, 0x4686);
    TEST("FLOGNP1.X (A6), FP7",           0xF200|026, 0x4006|(2<<10)|(7<<7));
    TEST("FLOGNP1.P (A0)+, FP1",          0xF200|030, 0x4006|(3<<10)|(1<<7));
    TEST("FLOGNP1.W -(A2), FP3",          0xF200|042, 0x4006|(4<<10)|(3<<7));
    TEST("FLOGNP1.D ($1234,A4), FP5",     0xF200|054, 0x4006|(5<<10)|(5<<7), 0x1234);
    TEST("FLOGNP1.B ($23,A6,D7.W), FP0",  0xF200|066, 0x4006|(6<<10)|(0<<7), 0x7023);
    TEST("FLOGNP1.L ($004566).W, FP1",    0xF200|070, 0x4006|(0<<10)|(1<<7), 0x4566);
    TEST("FLOGNP1.S ($56789A).L, FP2",    0xF200|071, 0x4006|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FLOGNP1.X (*+$1234,PC), FP3",   0xF200|072, 0x4006|(2<<10)|(3<<7), 0x1230);
    TEST("FLOGNP1.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x4006|(3<<10)|(5<<7), 0xC856);
    TEST("FLOGNP1.L #$6789ABCD, FP6",     0xF200|074, 0x4006|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FLOGNP1.W #$1234, FP2",         0xF200|074, 0x4006|(4<<10)|(2<<7), 0x1234);
    TEST("FLOGNP1.B #$23, FP4",           0xF200|074, 0x4006|(6<<10)|(4<<7), 0x0023);
    FLTS("FLOGNP1.S", "#7.88999976E-10, FP7", 0xF200|074, 0x4006|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FLOGNP1.X", "#-89000000032, FP0",   0xF200|074, 0x4006|(2<<10)|(0<<7),
         0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FLOGNP1.P", "#9.12E20, FP1",        0xF200|074, 0x4006|(3<<10)|(1<<7),
         0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FLOGNP1.D", "#-8.25, FP3",          0xF200|074, 0x4006|(5<<10)|(3<<7),
         0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FETOXM1.X FP0, FP1",            0xF200,     0x0008|(0<<10)|(1<<7));
    TEST("FETOXM1.X FP2",                 0xF200,     0x0008|(2<<10)|(2<<7));
    TEST("FETOXM1.L D2, FP3",             0xF200|002, 0x4008|(0<<10)|(3<<7));
    TEST("FETOXM1.S D2, FP3",             0xF200|002, 0x4008|(1<<10)|(3<<7));
    ERRT("FETOXM1.X D2, FP3", ILLEGAL_SIZE, "FETOXM1.X D2, FP3",
         0xF200|002, 0x4008|(2<<10)|(3<<7));
    ERRT("FETOXM1.P D2, FP3", ILLEGAL_SIZE, "FETOXM1.P D2, FP3",
         0xF200|002, 0x4008|(3<<10)|(3<<7));
    TEST("FETOXM1.W D2, FP3",             0xF200|002, 0x4008|(4<<10)|(3<<7));
    ERRT("FETOXM1.D D2, FP3", ILLEGAL_SIZE, "FETOXM1.D D2, FP3",
         0xF200|002, 0x4008|(5<<10)|(3<<7));
    TEST("FETOXM1.B D2, FP3",             0xF200|002, 0x4008|(6<<10)|(3<<7));
    ERRT("FETOXM1.S A4, FP5",             OPERAND_NOT_ALLOWED, "A4, FP5", 0xF200, 0x4688);
    TEST("FETOXM1.X (A6), FP7",           0xF200|026, 0x4008|(2<<10)|(7<<7));
    TEST("FETOXM1.P (A0)+, FP1",          0xF200|030, 0x4008|(3<<10)|(1<<7));
    TEST("FETOXM1.W -(A2), FP3",          0xF200|042, 0x4008|(4<<10)|(3<<7));
    TEST("FETOXM1.D ($1234,A4), FP5",     0xF200|054, 0x4008|(5<<10)|(5<<7), 0x1234);
    TEST("FETOXM1.B ($23,A6,D7.W), FP0",  0xF200|066, 0x4008|(6<<10)|(0<<7), 0x7023);
    TEST("FETOXM1.L ($004566).W, FP1",    0xF200|070, 0x4008|(0<<10)|(1<<7), 0x4566);
    TEST("FETOXM1.S ($56789A).L, FP2",    0xF200|071, 0x4008|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FETOXM1.X (*+$1234,PC), FP3",   0xF200|072, 0x4008|(2<<10)|(3<<7), 0x1230);
    TEST("FETOXM1.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x4008|(3<<10)|(5<<7), 0xC856);
    TEST("FETOXM1.L #$6789ABCD, FP6",     0xF200|074, 0x4008|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FETOXM1.W #$1234, FP2",         0xF200|074, 0x4008|(4<<10)|(2<<7), 0x1234);
    TEST("FETOXM1.B #$23, FP4",           0xF200|074, 0x4008|(6<<10)|(4<<7), 0x0023);
    FLTS("FETOXM1.S", "#7.88999976E-10, FP7", 0xF200|074, 0x4008|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FETOXM1.X", "#-89000000032, FP0",   0xF200|074, 0x4008|(2<<10)|(0<<7),
         0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FETOXM1.P", "#9.12E20, FP1",        0xF200|074, 0x4008|(3<<10)|(1<<7),
         0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FETOXM1.D", "#-8.25, FP3",          0xF200|074, 0x4008|(5<<10)|(3<<7),
         0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FTANH.X FP0, FP1",            0xF200,     0x0009|(0<<10)|(1<<7));
    TEST("FTANH.X FP2",                 0xF200,     0x0009|(2<<10)|(2<<7));
    TEST("FTANH.L D2, FP3",             0xF200|002, 0x4009|(0<<10)|(3<<7));
    TEST("FTANH.S D2, FP3",             0xF200|002, 0x4009|(1<<10)|(3<<7));
    ERRT("FTANH.X D2, FP3", ILLEGAL_SIZE, "FTANH.X D2, FP3",
         0xF200|002, 0x4009|(2<<10)|(3<<7));
    ERRT("FTANH.P D2, FP3", ILLEGAL_SIZE, "FTANH.P D2, FP3",
         0xF200|002, 0x4009|(3<<10)|(3<<7));
    TEST("FTANH.W D2, FP3",             0xF200|002, 0x4009|(4<<10)|(3<<7));
    ERRT("FTANH.D D2, FP3", ILLEGAL_SIZE, "FTANH.D D2, FP3",
         0xF200|002, 0x4009|(5<<10)|(3<<7));
    TEST("FTANH.B D2, FP3",             0xF200|002, 0x4009|(6<<10)|(3<<7));
    ERRT("FTANH.S A4, FP5",             OPERAND_NOT_ALLOWED, "A4, FP5", 0xF200, 0x4689);
    TEST("FTANH.X (A6), FP7",           0xF200|026, 0x4009|(2<<10)|(7<<7));
    TEST("FTANH.P (A0)+, FP1",          0xF200|030, 0x4009|(3<<10)|(1<<7));
    TEST("FTANH.W -(A2), FP3",          0xF200|042, 0x4009|(4<<10)|(3<<7));
    TEST("FTANH.D ($1234,A4), FP5",     0xF200|054, 0x4009|(5<<10)|(5<<7), 0x1234);
    TEST("FTANH.B ($23,A6,D7.W), FP0",  0xF200|066, 0x4009|(6<<10)|(0<<7), 0x7023);
    TEST("FTANH.L ($004566).W, FP1",    0xF200|070, 0x4009|(0<<10)|(1<<7), 0x4566);
    TEST("FTANH.S ($56789A).L, FP2",    0xF200|071, 0x4009|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FTANH.X (*+$1234,PC), FP3",   0xF200|072, 0x4009|(2<<10)|(3<<7), 0x1230);
    TEST("FTANH.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x4009|(3<<10)|(5<<7), 0xC856);
    TEST("FTANH.L #$6789ABCD, FP6",     0xF200|074, 0x4009|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FTANH.W #$1234, FP2",         0xF200|074, 0x4009|(4<<10)|(2<<7), 0x1234);
    TEST("FTANH.B #$23, FP4",           0xF200|074, 0x4009|(6<<10)|(4<<7), 0x0023);
    FLTS("FTANH.S", "#7.88999976E-10, FP7", 0xF200|074, 0x4009|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FTANH.X", "#-89000000032, FP0",   0xF200|074, 0x4009|(2<<10)|(0<<7),
         0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FTANH.P", "#9.12E20, FP1",        0xF200|074, 0x4009|(3<<10)|(1<<7),
         0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FTANH.D", "#-8.25, FP3",          0xF200|074, 0x4009|(5<<10)|(3<<7),
         0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FATAN.X FP0, FP1",            0xF200,     0x000A|(0<<10)|(1<<7));
    TEST("FATAN.X FP2",                 0xF200,     0x000A|(2<<10)|(2<<7));
    TEST("FATAN.L D2, FP3",             0xF200|002, 0x400A|(0<<10)|(3<<7));
    TEST("FATAN.S D2, FP3",             0xF200|002, 0x400A|(1<<10)|(3<<7));
    ERRT("FATAN.X D2, FP3", ILLEGAL_SIZE, "FATAN.X D2, FP3",
         0xF200|002, 0x400A|(2<<10)|(3<<7));
    ERRT("FATAN.P D2, FP3", ILLEGAL_SIZE, "FATAN.P D2, FP3",
         0xF200|002, 0x400A|(3<<10)|(3<<7));
    TEST("FATAN.W D2, FP3",             0xF200|002, 0x400A|(4<<10)|(3<<7));
    ERRT("FATAN.D D2, FP3", ILLEGAL_SIZE, "FATAN.D D2, FP3",
         0xF200|002, 0x400A|(5<<10)|(3<<7));
    TEST("FATAN.B D2, FP3",             0xF200|002, 0x400A|(6<<10)|(3<<7));
    ERRT("FATAN.S A4, FP5",             OPERAND_NOT_ALLOWED, "A4, FP5", 0xF200, 0x468A);
    TEST("FATAN.X (A6), FP7",           0xF200|026, 0x400A|(2<<10)|(7<<7));
    TEST("FATAN.P (A0)+, FP1",          0xF200|030, 0x400A|(3<<10)|(1<<7));
    TEST("FATAN.W -(A2), FP3",          0xF200|042, 0x400A|(4<<10)|(3<<7));
    TEST("FATAN.D ($1234,A4), FP5",     0xF200|054, 0x400A|(5<<10)|(5<<7), 0x1234);
    TEST("FATAN.B ($23,A6,D7.W), FP0",  0xF200|066, 0x400A|(6<<10)|(0<<7), 0x7023);
    TEST("FATAN.L ($004566).W, FP1",    0xF200|070, 0x400A|(0<<10)|(1<<7), 0x4566);
    TEST("FATAN.S ($56789A).L, FP2",    0xF200|071, 0x400A|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FATAN.X (*+$1234,PC), FP3",   0xF200|072, 0x400A|(2<<10)|(3<<7), 0x1230);
    TEST("FATAN.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x400A|(3<<10)|(5<<7), 0xC856);
    TEST("FATAN.L #$6789ABCD, FP6",     0xF200|074, 0x400A|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FATAN.W #$1234, FP2",         0xF200|074, 0x400A|(4<<10)|(2<<7), 0x1234);
    TEST("FATAN.B #$23, FP4",           0xF200|074, 0x400A|(6<<10)|(4<<7), 0x0023);
    FLTS("FATAN.S", "#7.88999976E-10, FP7", 0xF200|074, 0x400A|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FATAN.X", "#-89000000032, FP0",   0xF200|074, 0x400A|(2<<10)|(0<<7),
         0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FATAN.P", "#9.12E20, FP1",        0xF200|074, 0x400A|(3<<10)|(1<<7),
         0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FATAN.D", "#-8.25, FP3",          0xF200|074, 0x400A|(5<<10)|(3<<7),
         0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FASIN.X FP0, FP1",            0xF200,     0x000C|(0<<10)|(1<<7));
    TEST("FASIN.X FP2",                 0xF200,     0x000C|(2<<10)|(2<<7));
    TEST("FASIN.L D2, FP3",             0xF200|002, 0x400C|(0<<10)|(3<<7));
    TEST("FASIN.S D2, FP3",             0xF200|002, 0x400C|(1<<10)|(3<<7));
    ERRT("FASIN.X D2, FP3", ILLEGAL_SIZE, "FASIN.X D2, FP3",
         0xF200|002, 0x400C|(2<<10)|(3<<7));
    ERRT("FASIN.P D2, FP3", ILLEGAL_SIZE, "FASIN.P D2, FP3",
         0xF200|002, 0x400C|(3<<10)|(3<<7));
    TEST("FASIN.W D2, FP3",             0xF200|002, 0x400C|(4<<10)|(3<<7));
    ERRT("FASIN.D D2, FP3", ILLEGAL_SIZE, "FASIN.D D2, FP3",
         0xF200|002, 0x400C|(5<<10)|(3<<7));
    TEST("FASIN.B D2, FP3",             0xF200|002, 0x400C|(6<<10)|(3<<7));
    ERRT("FASIN.S A4, FP5",             OPERAND_NOT_ALLOWED, "A4, FP5", 0xF200, 0x468C);
    TEST("FASIN.X (A6), FP7",           0xF200|026, 0x400C|(2<<10)|(7<<7));
    TEST("FASIN.P (A0)+, FP1",          0xF200|030, 0x400C|(3<<10)|(1<<7));
    TEST("FASIN.W -(A2), FP3",          0xF200|042, 0x400C|(4<<10)|(3<<7));
    TEST("FASIN.D ($1234,A4), FP5",     0xF200|054, 0x400C|(5<<10)|(5<<7), 0x1234);
    TEST("FASIN.B ($23,A6,D7.W), FP0",  0xF200|066, 0x400C|(6<<10)|(0<<7), 0x7023);
    TEST("FASIN.L ($004566).W, FP1",    0xF200|070, 0x400C|(0<<10)|(1<<7), 0x4566);
    TEST("FASIN.S ($56789A).L, FP2",    0xF200|071, 0x400C|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FASIN.X (*+$1234,PC), FP3",   0xF200|072, 0x400C|(2<<10)|(3<<7), 0x1230);
    TEST("FASIN.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x400C|(3<<10)|(5<<7), 0xC856);
    TEST("FASIN.L #$6789ABCD, FP6",     0xF200|074, 0x400C|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FASIN.W #$1234, FP2",         0xF200|074, 0x400C|(4<<10)|(2<<7), 0x1234);
    TEST("FASIN.B #$23, FP4",           0xF200|074, 0x400C|(6<<10)|(4<<7), 0x0023);
    FLTS("FASIN.S", "#7.88999976E-10, FP7", 0xF200|074, 0x400C|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FASIN.X", "#-89000000032, FP0",   0xF200|074, 0x400C|(2<<10)|(0<<7),
         0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FASIN.P", "#9.12E20, FP1",        0xF200|074, 0x400C|(3<<10)|(1<<7),
         0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FASIN.D", "#-8.25, FP3",          0xF200|074, 0x400C|(5<<10)|(3<<7),
         0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FATANH.X FP0, FP1",            0xF200,     0x000D|(0<<10)|(1<<7));
    TEST("FATANH.X FP2",                 0xF200,     0x000D|(2<<10)|(2<<7));
    TEST("FATANH.L D2, FP3",             0xF200|002, 0x400D|(0<<10)|(3<<7));
    TEST("FATANH.S D2, FP3",             0xF200|002, 0x400D|(1<<10)|(3<<7));
    ERRT("FATANH.X D2, FP3", ILLEGAL_SIZE, "FATANH.X D2, FP3",
         0xF200|002, 0x400D|(2<<10)|(3<<7));
    ERRT("FATANH.P D2, FP3", ILLEGAL_SIZE, "FATANH.P D2, FP3",
         0xF200|002, 0x400D|(3<<10)|(3<<7));
    TEST("FATANH.W D2, FP3",             0xF200|002, 0x400D|(4<<10)|(3<<7));
    ERRT("FATANH.D D2, FP3", ILLEGAL_SIZE, "FATANH.D D2, FP3",
         0xF200|002, 0x400D|(5<<10)|(3<<7));
    TEST("FATANH.B D2, FP3",             0xF200|002, 0x400D|(6<<10)|(3<<7));
    ERRT("FATANH.S A4, FP5",             OPERAND_NOT_ALLOWED, "A4, FP5", 0xF200, 0x468D);
    TEST("FATANH.X (A6), FP7",           0xF200|026, 0x400D|(2<<10)|(7<<7));
    TEST("FATANH.P (A0)+, FP1",          0xF200|030, 0x400D|(3<<10)|(1<<7));
    TEST("FATANH.W -(A2), FP3",          0xF200|042, 0x400D|(4<<10)|(3<<7));
    TEST("FATANH.D ($1234,A4), FP5",     0xF200|054, 0x400D|(5<<10)|(5<<7), 0x1234);
    TEST("FATANH.B ($23,A6,D7.W), FP0",  0xF200|066, 0x400D|(6<<10)|(0<<7), 0x7023);
    TEST("FATANH.L ($004566).W, FP1",    0xF200|070, 0x400D|(0<<10)|(1<<7), 0x4566);
    TEST("FATANH.S ($56789A).L, FP2",    0xF200|071, 0x400D|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FATANH.X (*+$1234,PC), FP3",   0xF200|072, 0x400D|(2<<10)|(3<<7), 0x1230);
    TEST("FATANH.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x400D|(3<<10)|(5<<7), 0xC856);
    TEST("FATANH.L #$6789ABCD, FP6",     0xF200|074, 0x400D|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FATANH.W #$1234, FP2",         0xF200|074, 0x400D|(4<<10)|(2<<7), 0x1234);
    TEST("FATANH.B #$23, FP4",           0xF200|074, 0x400D|(6<<10)|(4<<7), 0x0023);
    FLTS("FATANH.S", "#7.88999976E-10, FP7", 0xF200|074, 0x400D|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FATANH.X", "#-89000000032, FP0",   0xF200|074, 0x400D|(2<<10)|(0<<7),
         0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FATANH.P", "#9.12E20, FP1",        0xF200|074, 0x400D|(3<<10)|(1<<7),
         0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FATANH.D", "#-8.25, FP3",          0xF200|074, 0x400D|(5<<10)|(3<<7),
         0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FSIN.X FP0, FP1",            0xF200,     0x000E|(0<<10)|(1<<7));
    TEST("FSIN.X FP2",                 0xF200,     0x000E|(2<<10)|(2<<7));
    TEST("FSIN.L D2, FP3",             0xF200|002, 0x400E|(0<<10)|(3<<7));
    TEST("FSIN.S D2, FP3",             0xF200|002, 0x400E|(1<<10)|(3<<7));
    ERRT("FSIN.X D2, FP3", ILLEGAL_SIZE, "FSIN.X D2, FP3",
         0xF200|002, 0x400E|(2<<10)|(3<<7));
    ERRT("FSIN.P D2, FP3", ILLEGAL_SIZE, "FSIN.P D2, FP3",
         0xF200|002, 0x400E|(3<<10)|(3<<7));
    TEST("FSIN.W D2, FP3",             0xF200|002, 0x400E|(4<<10)|(3<<7));
    ERRT("FSIN.D D2, FP3", ILLEGAL_SIZE, "FSIN.D D2, FP3",
         0xF200|002, 0x400E|(5<<10)|(3<<7));
    TEST("FSIN.B D2, FP3",             0xF200|002, 0x400E|(6<<10)|(3<<7));
    ERRT("FSIN.S A4, FP5",             OPERAND_NOT_ALLOWED, "A4, FP5", 0xF200, 0x468E);
    TEST("FSIN.X (A6), FP7",           0xF200|026, 0x400E|(2<<10)|(7<<7));
    TEST("FSIN.P (A0)+, FP1",          0xF200|030, 0x400E|(3<<10)|(1<<7));
    TEST("FSIN.W -(A2), FP3",          0xF200|042, 0x400E|(4<<10)|(3<<7));
    TEST("FSIN.D ($1234,A4), FP5",     0xF200|054, 0x400E|(5<<10)|(5<<7), 0x1234);
    TEST("FSIN.B ($23,A6,D7.W), FP0",  0xF200|066, 0x400E|(6<<10)|(0<<7), 0x7023);
    TEST("FSIN.L ($004566).W, FP1",    0xF200|070, 0x400E|(0<<10)|(1<<7), 0x4566);
    TEST("FSIN.S ($56789A).L, FP2",    0xF200|071, 0x400E|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FSIN.X (*+$1234,PC), FP3",   0xF200|072, 0x400E|(2<<10)|(3<<7), 0x1230);
    TEST("FSIN.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x400E|(3<<10)|(5<<7), 0xC856);
    TEST("FSIN.L #$6789ABCD, FP6",     0xF200|074, 0x400E|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FSIN.W #$1234, FP2",         0xF200|074, 0x400E|(4<<10)|(2<<7), 0x1234);
    TEST("FSIN.B #$23, FP4",           0xF200|074, 0x400E|(6<<10)|(4<<7), 0x0023);
    FLTS("FSIN.S", "#7.88999976E-10, FP7", 0xF200|074, 0x400E|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FSIN.X", "#-89000000032, FP0",   0xF200|074, 0x400E|(2<<10)|(0<<7),
         0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FSIN.P", "#9.12E20, FP1",        0xF200|074, 0x400E|(3<<10)|(1<<7),
         0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FSIN.D", "#-8.25, FP3",          0xF200|074, 0x400E|(5<<10)|(3<<7),
         0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FTAN.X FP0, FP1",            0xF200,     0x000F|(0<<10)|(1<<7));
    TEST("FTAN.X FP2",                 0xF200,     0x000F|(2<<10)|(2<<7));
    TEST("FTAN.L D2, FP3",             0xF200|002, 0x400F|(0<<10)|(3<<7));
    TEST("FTAN.S D2, FP3",             0xF200|002, 0x400F|(1<<10)|(3<<7));
    ERRT("FTAN.X D2, FP3", ILLEGAL_SIZE, "FTAN.X D2, FP3",
         0xF200|002, 0x400F|(2<<10)|(3<<7));
    ERRT("FTAN.P D2, FP3", ILLEGAL_SIZE, "FTAN.P D2, FP3",
         0xF200|002, 0x400F|(3<<10)|(3<<7));
    TEST("FTAN.W D2, FP3",             0xF200|002, 0x400F|(4<<10)|(3<<7));
    ERRT("FTAN.D D2, FP3", ILLEGAL_SIZE, "FTAN.D D2, FP3",
         0xF200|002, 0x400F|(5<<10)|(3<<7));
    TEST("FTAN.B D2, FP3",             0xF200|002, 0x400F|(6<<10)|(3<<7));
    ERRT("FTAN.S A4, FP5",             OPERAND_NOT_ALLOWED, "A4, FP5", 0xF200, 0x468F);
    TEST("FTAN.X (A6), FP7",           0xF200|026, 0x400F|(2<<10)|(7<<7));
    TEST("FTAN.P (A0)+, FP1",          0xF200|030, 0x400F|(3<<10)|(1<<7));
    TEST("FTAN.W -(A2), FP3",          0xF200|042, 0x400F|(4<<10)|(3<<7));
    TEST("FTAN.D ($1234,A4), FP5",     0xF200|054, 0x400F|(5<<10)|(5<<7), 0x1234);
    TEST("FTAN.B ($23,A6,D7.W), FP0",  0xF200|066, 0x400F|(6<<10)|(0<<7), 0x7023);
    TEST("FTAN.L ($004566).W, FP1",    0xF200|070, 0x400F|(0<<10)|(1<<7), 0x4566);
    TEST("FTAN.S ($56789A).L, FP2",    0xF200|071, 0x400F|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FTAN.X (*+$1234,PC), FP3",   0xF200|072, 0x400F|(2<<10)|(3<<7), 0x1230);
    TEST("FTAN.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x400F|(3<<10)|(5<<7), 0xC856);
    TEST("FTAN.L #$6789ABCD, FP6",     0xF200|074, 0x400F|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FTAN.W #$1234, FP2",         0xF200|074, 0x400F|(4<<10)|(2<<7), 0x1234);
    TEST("FTAN.B #$23, FP4",           0xF200|074, 0x400F|(6<<10)|(4<<7), 0x0023);
    FLTS("FTAN.S", "#7.88999976E-10, FP7", 0xF200|074, 0x400F|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FTAN.X", "#-89000000032, FP0",   0xF200|074, 0x400F|(2<<10)|(0<<7),
         0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FTAN.P", "#9.12E20, FP1",        0xF200|074, 0x400F|(3<<10)|(1<<7),
         0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FTAN.D", "#-8.25, FP3",          0xF200|074, 0x400F|(5<<10)|(3<<7),
         0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FETOX.X FP0, FP1",            0xF200,     0x0010|(0<<10)|(1<<7));
    TEST("FETOX.X FP2",                 0xF200,     0x0010|(2<<10)|(2<<7));
    TEST("FETOX.L D2, FP3",             0xF200|002, 0x4010|(0<<10)|(3<<7));
    TEST("FETOX.S D2, FP3",             0xF200|002, 0x4010|(1<<10)|(3<<7));
    ERRT("FETOX.X D2, FP3", ILLEGAL_SIZE, "FETOX.X D2, FP3",
         0xF200|002, 0x4010|(2<<10)|(3<<7));
    ERRT("FETOX.P D2, FP3", ILLEGAL_SIZE, "FETOX.P D2, FP3",
         0xF200|002, 0x4010|(3<<10)|(3<<7));
    TEST("FETOX.W D2, FP3",             0xF200|002, 0x4010|(4<<10)|(3<<7));
    ERRT("FETOX.D D2, FP3", ILLEGAL_SIZE, "FETOX.D D2, FP3",
         0xF200|002, 0x4010|(5<<10)|(3<<7));
    TEST("FETOX.B D2, FP3",             0xF200|002, 0x4010|(6<<10)|(3<<7));
    ERRT("FETOX.S A4, FP5",             OPERAND_NOT_ALLOWED, "A4, FP5", 0xF200, 0x4690);
    TEST("FETOX.X (A6), FP7",           0xF200|026, 0x4010|(2<<10)|(7<<7));
    TEST("FETOX.P (A0)+, FP1",          0xF200|030, 0x4010|(3<<10)|(1<<7));
    TEST("FETOX.W -(A2), FP3",          0xF200|042, 0x4010|(4<<10)|(3<<7));
    TEST("FETOX.D ($1234,A4), FP5",     0xF200|054, 0x4010|(5<<10)|(5<<7), 0x1234);
    TEST("FETOX.B ($23,A6,D7.W), FP0",  0xF200|066, 0x4010|(6<<10)|(0<<7), 0x7023);
    TEST("FETOX.L ($004566).W, FP1",    0xF200|070, 0x4010|(0<<10)|(1<<7), 0x4566);
    TEST("FETOX.S ($56789A).L, FP2",    0xF200|071, 0x4010|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FETOX.X (*+$1234,PC), FP3",   0xF200|072, 0x4010|(2<<10)|(3<<7), 0x1230);
    TEST("FETOX.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x4010|(3<<10)|(5<<7), 0xC856);
    TEST("FETOX.L #$6789ABCD, FP6",     0xF200|074, 0x4010|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FETOX.W #$1234, FP2",         0xF200|074, 0x4010|(4<<10)|(2<<7), 0x1234);
    TEST("FETOX.B #$23, FP4",           0xF200|074, 0x4010|(6<<10)|(4<<7), 0x0023);
    FLTS("FETOX.S", "#7.88999976E-10, FP7", 0xF200|074, 0x4010|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FETOX.X", "#-89000000032, FP0",   0xF200|074, 0x4010|(2<<10)|(0<<7),
         0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FETOX.P", "#9.12E20, FP1",        0xF200|074, 0x4010|(3<<10)|(1<<7),
         0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FETOX.D", "#-8.25, FP3",          0xF200|074, 0x4010|(5<<10)|(3<<7),
         0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FTWOTOX.X FP0, FP1",            0xF200,     0x0011|(0<<10)|(1<<7));
    TEST("FTWOTOX.X FP2",                 0xF200,     0x0011|(2<<10)|(2<<7));
    TEST("FTWOTOX.L D2, FP3",             0xF200|002, 0x4011|(0<<10)|(3<<7));
    TEST("FTWOTOX.S D2, FP3",             0xF200|002, 0x4011|(1<<10)|(3<<7));
    ERRT("FTWOTOX.X D2, FP3", ILLEGAL_SIZE, "FTWOTOX.X D2, FP3",
         0xF200|002, 0x4011|(2<<10)|(3<<7));
    ERRT("FTWOTOX.P D2, FP3", ILLEGAL_SIZE, "FTWOTOX.P D2, FP3",
         0xF200|002, 0x4011|(3<<10)|(3<<7));
    TEST("FTWOTOX.W D2, FP3",             0xF200|002, 0x4011|(4<<10)|(3<<7));
    ERRT("FTWOTOX.D D2, FP3", ILLEGAL_SIZE, "FTWOTOX.D D2, FP3",
         0xF200|002, 0x4011|(5<<10)|(3<<7));
    TEST("FTWOTOX.B D2, FP3",             0xF200|002, 0x4011|(6<<10)|(3<<7));
    ERRT("FTWOTOX.S A4, FP5",             OPERAND_NOT_ALLOWED, "A4, FP5", 0xF200, 0x4691);
    TEST("FTWOTOX.X (A6), FP7",           0xF200|026, 0x4011|(2<<10)|(7<<7));
    TEST("FTWOTOX.P (A0)+, FP1",          0xF200|030, 0x4011|(3<<10)|(1<<7));
    TEST("FTWOTOX.W -(A2), FP3",          0xF200|042, 0x4011|(4<<10)|(3<<7));
    TEST("FTWOTOX.D ($1234,A4), FP5",     0xF200|054, 0x4011|(5<<10)|(5<<7), 0x1234);
    TEST("FTWOTOX.B ($23,A6,D7.W), FP0",  0xF200|066, 0x4011|(6<<10)|(0<<7), 0x7023);
    TEST("FTWOTOX.L ($004566).W, FP1",    0xF200|070, 0x4011|(0<<10)|(1<<7), 0x4566);
    TEST("FTWOTOX.S ($56789A).L, FP2",    0xF200|071, 0x4011|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FTWOTOX.X (*+$1234,PC), FP3",   0xF200|072, 0x4011|(2<<10)|(3<<7), 0x1230);
    TEST("FTWOTOX.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x4011|(3<<10)|(5<<7), 0xC856);
    TEST("FTWOTOX.L #$6789ABCD, FP6",     0xF200|074, 0x4011|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FTWOTOX.W #$1234, FP2",         0xF200|074, 0x4011|(4<<10)|(2<<7), 0x1234);
    TEST("FTWOTOX.B #$23, FP4",           0xF200|074, 0x4011|(6<<10)|(4<<7), 0x0023);
    FLTS("FTWOTOX.S", "#7.88999976E-10, FP7", 0xF200|074, 0x4011|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FTWOTOX.X", "#-89000000032, FP0",   0xF200|074, 0x4011|(2<<10)|(0<<7),
         0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FTWOTOX.P", "#9.12E20, FP1",        0xF200|074, 0x4011|(3<<10)|(1<<7),
         0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FTWOTOX.D", "#-8.25, FP3",          0xF200|074, 0x4011|(5<<10)|(3<<7),
         0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FTENTOX.X FP0, FP1",            0xF200,     0x0012|(0<<10)|(1<<7));
    TEST("FTENTOX.X FP2",                 0xF200,     0x0012|(2<<10)|(2<<7));
    TEST("FTENTOX.L D2, FP3",             0xF200|002, 0x4012|(0<<10)|(3<<7));
    TEST("FTENTOX.S D2, FP3",             0xF200|002, 0x4012|(1<<10)|(3<<7));
    ERRT("FTENTOX.X D2, FP3", ILLEGAL_SIZE, "FTENTOX.X D2, FP3",
         0xF200|002, 0x4012|(2<<10)|(3<<7));
    ERRT("FTENTOX.P D2, FP3", ILLEGAL_SIZE, "FTENTOX.P D2, FP3",
         0xF200|002, 0x4012|(3<<10)|(3<<7));
    TEST("FTENTOX.W D2, FP3",             0xF200|002, 0x4012|(4<<10)|(3<<7));
    ERRT("FTENTOX.D D2, FP3", ILLEGAL_SIZE, "FTENTOX.D D2, FP3",
         0xF200|002, 0x4012|(5<<10)|(3<<7));
    TEST("FTENTOX.B D2, FP3",             0xF200|002, 0x4012|(6<<10)|(3<<7));
    ERRT("FTENTOX.S A4, FP5",             OPERAND_NOT_ALLOWED, "A4, FP5", 0xF200, 0x4692);
    TEST("FTENTOX.X (A6), FP7",           0xF200|026, 0x4012|(2<<10)|(7<<7));
    TEST("FTENTOX.P (A0)+, FP1",          0xF200|030, 0x4012|(3<<10)|(1<<7));
    TEST("FTENTOX.W -(A2), FP3",          0xF200|042, 0x4012|(4<<10)|(3<<7));
    TEST("FTENTOX.D ($1234,A4), FP5",     0xF200|054, 0x4012|(5<<10)|(5<<7), 0x1234);
    TEST("FTENTOX.B ($23,A6,D7.W), FP0",  0xF200|066, 0x4012|(6<<10)|(0<<7), 0x7023);
    TEST("FTENTOX.L ($004566).W, FP1",    0xF200|070, 0x4012|(0<<10)|(1<<7), 0x4566);
    TEST("FTENTOX.S ($56789A).L, FP2",    0xF200|071, 0x4012|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FTENTOX.X (*+$1234,PC), FP3",   0xF200|072, 0x4012|(2<<10)|(3<<7), 0x1230);
    TEST("FTENTOX.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x4012|(3<<10)|(5<<7), 0xC856);
    TEST("FTENTOX.L #$6789ABCD, FP6",     0xF200|074, 0x4012|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FTENTOX.W #$1234, FP2",         0xF200|074, 0x4012|(4<<10)|(2<<7), 0x1234);
    TEST("FTENTOX.B #$23, FP4",           0xF200|074, 0x4012|(6<<10)|(4<<7), 0x0023);
    FLTS("FTENTOX.S", "#7.88999976E-10, FP7", 0xF200|074, 0x4012|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FTENTOX.X", "#-89000000032, FP0",   0xF200|074, 0x4012|(2<<10)|(0<<7),
         0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FTENTOX.P", "#9.12E20, FP1",        0xF200|074, 0x4012|(3<<10)|(1<<7),
         0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FTENTOX.D", "#-8.25, FP3",          0xF200|074, 0x4012|(5<<10)|(3<<7),
         0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FLOGN.X FP0, FP1",            0xF200,     0x0014|(0<<10)|(1<<7));
    TEST("FLOGN.X FP2",                 0xF200,     0x0014|(2<<10)|(2<<7));
    TEST("FLOGN.L D2, FP3",             0xF200|002, 0x4014|(0<<10)|(3<<7));
    TEST("FLOGN.S D2, FP3",             0xF200|002, 0x4014|(1<<10)|(3<<7));
    ERRT("FLOGN.X D2, FP3", ILLEGAL_SIZE, "FLOGN.X D2, FP3",
         0xF200|002, 0x4014|(2<<10)|(3<<7));
    ERRT("FLOGN.P D2, FP3", ILLEGAL_SIZE, "FLOGN.P D2, FP3",
         0xF200|002, 0x4014|(3<<10)|(3<<7));
    TEST("FLOGN.W D2, FP3",             0xF200|002, 0x4014|(4<<10)|(3<<7));
    ERRT("FLOGN.D D2, FP3", ILLEGAL_SIZE, "FLOGN.D D2, FP3",
         0xF200|002, 0x4014|(5<<10)|(3<<7));
    TEST("FLOGN.B D2, FP3",             0xF200|002, 0x4014|(6<<10)|(3<<7));
    ERRT("FLOGN.S A4, FP5",             OPERAND_NOT_ALLOWED, "A4, FP5", 0xF200, 0x4694);
    TEST("FLOGN.X (A6), FP7",           0xF200|026, 0x4014|(2<<10)|(7<<7));
    TEST("FLOGN.P (A0)+, FP1",          0xF200|030, 0x4014|(3<<10)|(1<<7));
    TEST("FLOGN.W -(A2), FP3",          0xF200|042, 0x4014|(4<<10)|(3<<7));
    TEST("FLOGN.D ($1234,A4), FP5",     0xF200|054, 0x4014|(5<<10)|(5<<7), 0x1234);
    TEST("FLOGN.B ($23,A6,D7.W), FP0",  0xF200|066, 0x4014|(6<<10)|(0<<7), 0x7023);
    TEST("FLOGN.L ($004566).W, FP1",    0xF200|070, 0x4014|(0<<10)|(1<<7), 0x4566);
    TEST("FLOGN.S ($56789A).L, FP2",    0xF200|071, 0x4014|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FLOGN.X (*+$1234,PC), FP3",   0xF200|072, 0x4014|(2<<10)|(3<<7), 0x1230);
    TEST("FLOGN.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x4014|(3<<10)|(5<<7), 0xC856);
    TEST("FLOGN.L #$6789ABCD, FP6",     0xF200|074, 0x4014|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FLOGN.W #$1234, FP2",         0xF200|074, 0x4014|(4<<10)|(2<<7), 0x1234);
    TEST("FLOGN.B #$23, FP4",           0xF200|074, 0x4014|(6<<10)|(4<<7), 0x0023);
    FLTS("FLOGN.S", "#7.88999976E-10, FP7", 0xF200|074, 0x4014|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FLOGN.X", "#-89000000032, FP0",   0xF200|074, 0x4014|(2<<10)|(0<<7),
         0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FLOGN.P", "#9.12E20, FP1",        0xF200|074, 0x4014|(3<<10)|(1<<7),
         0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FLOGN.D", "#-8.25, FP3",          0xF200|074, 0x4014|(5<<10)|(3<<7),
         0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FLOG10.X FP0, FP1",            0xF200,     0x0015|(0<<10)|(1<<7));
    TEST("FLOG10.X FP2",                 0xF200,     0x0015|(2<<10)|(2<<7));
    TEST("FLOG10.L D2, FP3",             0xF200|002, 0x4015|(0<<10)|(3<<7));
    TEST("FLOG10.S D2, FP3",             0xF200|002, 0x4015|(1<<10)|(3<<7));
    ERRT("FLOG10.X D2, FP3", ILLEGAL_SIZE, "FLOG10.X D2, FP3",
         0xF200|002, 0x4015|(2<<10)|(3<<7));
    ERRT("FLOG10.P D2, FP3", ILLEGAL_SIZE, "FLOG10.P D2, FP3",
         0xF200|002, 0x4015|(3<<10)|(3<<7));
    TEST("FLOG10.W D2, FP3",             0xF200|002, 0x4015|(4<<10)|(3<<7));
    ERRT("FLOG10.D D2, FP3", ILLEGAL_SIZE, "FLOG10.D D2, FP3",
         0xF200|002, 0x4015|(5<<10)|(3<<7));
    TEST("FLOG10.B D2, FP3",             0xF200|002, 0x4015|(6<<10)|(3<<7));
    ERRT("FLOG10.S A4, FP5",             OPERAND_NOT_ALLOWED, "A4, FP5", 0xF200, 0x4695);
    TEST("FLOG10.X (A6), FP7",           0xF200|026, 0x4015|(2<<10)|(7<<7));
    TEST("FLOG10.P (A0)+, FP1",          0xF200|030, 0x4015|(3<<10)|(1<<7));
    TEST("FLOG10.W -(A2), FP3",          0xF200|042, 0x4015|(4<<10)|(3<<7));
    TEST("FLOG10.D ($1234,A4), FP5",     0xF200|054, 0x4015|(5<<10)|(5<<7), 0x1234);
    TEST("FLOG10.B ($23,A6,D7.W), FP0",  0xF200|066, 0x4015|(6<<10)|(0<<7), 0x7023);
    TEST("FLOG10.L ($004566).W, FP1",    0xF200|070, 0x4015|(0<<10)|(1<<7), 0x4566);
    TEST("FLOG10.S ($56789A).L, FP2",    0xF200|071, 0x4015|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FLOG10.X (*+$1234,PC), FP3",   0xF200|072, 0x4015|(2<<10)|(3<<7), 0x1230);
    TEST("FLOG10.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x4015|(3<<10)|(5<<7), 0xC856);
    TEST("FLOG10.L #$6789ABCD, FP6",     0xF200|074, 0x4015|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FLOG10.W #$1234, FP2",         0xF200|074, 0x4015|(4<<10)|(2<<7), 0x1234);
    TEST("FLOG10.B #$23, FP4",           0xF200|074, 0x4015|(6<<10)|(4<<7), 0x0023);
    FLTS("FLOG10.S", "#7.88999976E-10, FP7", 0xF200|074, 0x4015|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FLOG10.X", "#-89000000032, FP0",   0xF200|074, 0x4015|(2<<10)|(0<<7),
         0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FLOG10.P", "#9.12E20, FP1",        0xF200|074, 0x4015|(3<<10)|(1<<7),
         0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FLOG10.D", "#-8.25, FP3",          0xF200|074, 0x4015|(5<<10)|(3<<7),
         0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FLOG2.X FP0, FP1",            0xF200,     0x0016|(0<<10)|(1<<7));
    TEST("FLOG2.X FP2",                 0xF200,     0x0016|(2<<10)|(2<<7));
    TEST("FLOG2.L D2, FP3",             0xF200|002, 0x4016|(0<<10)|(3<<7));
    TEST("FLOG2.S D2, FP3",             0xF200|002, 0x4016|(1<<10)|(3<<7));
    ERRT("FLOG2.X D2, FP3", ILLEGAL_SIZE, "FLOG2.X D2, FP3",
         0xF200|002, 0x4016|(2<<10)|(3<<7));
    ERRT("FLOG2.P D2, FP3", ILLEGAL_SIZE, "FLOG2.P D2, FP3",
         0xF200|002, 0x4016|(3<<10)|(3<<7));
    TEST("FLOG2.W D2, FP3",             0xF200|002, 0x4016|(4<<10)|(3<<7));
    ERRT("FLOG2.D D2, FP3", ILLEGAL_SIZE, "FLOG2.D D2, FP3",
         0xF200|002, 0x4016|(5<<10)|(3<<7));
    TEST("FLOG2.B D2, FP3",             0xF200|002, 0x4016|(6<<10)|(3<<7));
    ERRT("FLOG2.S A4, FP5",             OPERAND_NOT_ALLOWED, "A4, FP5", 0xF200, 0x4696);
    TEST("FLOG2.X (A6), FP7",           0xF200|026, 0x4016|(2<<10)|(7<<7));
    TEST("FLOG2.P (A0)+, FP1",          0xF200|030, 0x4016|(3<<10)|(1<<7));
    TEST("FLOG2.W -(A2), FP3",          0xF200|042, 0x4016|(4<<10)|(3<<7));
    TEST("FLOG2.D ($1234,A4), FP5",     0xF200|054, 0x4016|(5<<10)|(5<<7), 0x1234);
    TEST("FLOG2.B ($23,A6,D7.W), FP0",  0xF200|066, 0x4016|(6<<10)|(0<<7), 0x7023);
    TEST("FLOG2.L ($004566).W, FP1",    0xF200|070, 0x4016|(0<<10)|(1<<7), 0x4566);
    TEST("FLOG2.S ($56789A).L, FP2",    0xF200|071, 0x4016|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FLOG2.X (*+$1234,PC), FP3",   0xF200|072, 0x4016|(2<<10)|(3<<7), 0x1230);
    TEST("FLOG2.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x4016|(3<<10)|(5<<7), 0xC856);
    TEST("FLOG2.L #$6789ABCD, FP6",     0xF200|074, 0x4016|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FLOG2.W #$1234, FP2",         0xF200|074, 0x4016|(4<<10)|(2<<7), 0x1234);
    TEST("FLOG2.B #$23, FP4",           0xF200|074, 0x4016|(6<<10)|(4<<7), 0x0023);
    FLTS("FLOG2.S", "#7.88999976E-10, FP7", 0xF200|074, 0x4016|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FLOG2.X", "#-89000000032, FP0",   0xF200|074, 0x4016|(2<<10)|(0<<7),
         0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FLOG2.P", "#9.12E20, FP1",        0xF200|074, 0x4016|(3<<10)|(1<<7),
         0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FLOG2.D", "#-8.25, FP3",          0xF200|074, 0x4016|(5<<10)|(3<<7),
         0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FCOSH.X FP0, FP1",            0xF200,     0x0019|(0<<10)|(1<<7));
    TEST("FCOSH.X FP2",                 0xF200,     0x0019|(2<<10)|(2<<7));
    TEST("FCOSH.L D2, FP3",             0xF200|002, 0x4019|(0<<10)|(3<<7));
    TEST("FCOSH.S D2, FP3",             0xF200|002, 0x4019|(1<<10)|(3<<7));
    ERRT("FCOSH.X D2, FP3", ILLEGAL_SIZE, "FCOSH.X D2, FP3",
         0xF200|002, 0x4019|(2<<10)|(3<<7));
    ERRT("FCOSH.P D2, FP3", ILLEGAL_SIZE, "FCOSH.P D2, FP3",
         0xF200|002, 0x4019|(3<<10)|(3<<7));
    TEST("FCOSH.W D2, FP3",             0xF200|002, 0x4019|(4<<10)|(3<<7));
    ERRT("FCOSH.D D2, FP3", ILLEGAL_SIZE, "FCOSH.D D2, FP3",
         0xF200|002, 0x4019|(5<<10)|(3<<7));
    TEST("FCOSH.B D2, FP3",             0xF200|002, 0x4019|(6<<10)|(3<<7));
    ERRT("FCOSH.S A4, FP5",             OPERAND_NOT_ALLOWED, "A4, FP5", 0xF200, 0x4699);
    TEST("FCOSH.X (A6), FP7",           0xF200|026, 0x4019|(2<<10)|(7<<7));
    TEST("FCOSH.P (A0)+, FP1",          0xF200|030, 0x4019|(3<<10)|(1<<7));
    TEST("FCOSH.W -(A2), FP3",          0xF200|042, 0x4019|(4<<10)|(3<<7));
    TEST("FCOSH.D ($1234,A4), FP5",     0xF200|054, 0x4019|(5<<10)|(5<<7), 0x1234);
    TEST("FCOSH.B ($23,A6,D7.W), FP0",  0xF200|066, 0x4019|(6<<10)|(0<<7), 0x7023);
    TEST("FCOSH.L ($004566).W, FP1",    0xF200|070, 0x4019|(0<<10)|(1<<7), 0x4566);
    TEST("FCOSH.S ($56789A).L, FP2",    0xF200|071, 0x4019|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FCOSH.X (*+$1234,PC), FP3",   0xF200|072, 0x4019|(2<<10)|(3<<7), 0x1230);
    TEST("FCOSH.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x4019|(3<<10)|(5<<7), 0xC856);
    TEST("FCOSH.L #$6789ABCD, FP6",     0xF200|074, 0x4019|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FCOSH.W #$1234, FP2",         0xF200|074, 0x4019|(4<<10)|(2<<7), 0x1234);
    TEST("FCOSH.B #$23, FP4",           0xF200|074, 0x4019|(6<<10)|(4<<7), 0x0023);
    FLTS("FCOSH.S", "#7.88999976E-10, FP7", 0xF200|074, 0x4019|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FCOSH.X", "#-89000000032, FP0",   0xF200|074, 0x4019|(2<<10)|(0<<7),
         0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FCOSH.P", "#9.12E20, FP1",        0xF200|074, 0x4019|(3<<10)|(1<<7),
         0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FCOSH.D", "#-8.25, FP3",          0xF200|074, 0x4019|(5<<10)|(3<<7),
         0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FACOS.X FP0, FP1",            0xF200,     0x001C|(0<<10)|(1<<7));
    TEST("FACOS.X FP2",                 0xF200,     0x001C|(2<<10)|(2<<7));
    TEST("FACOS.L D2, FP3",             0xF200|002, 0x401C|(0<<10)|(3<<7));
    TEST("FACOS.S D2, FP3",             0xF200|002, 0x401C|(1<<10)|(3<<7));
    ERRT("FACOS.X D2, FP3", ILLEGAL_SIZE, "FACOS.X D2, FP3",
         0xF200|002, 0x401C|(2<<10)|(3<<7));
    ERRT("FACOS.P D2, FP3", ILLEGAL_SIZE, "FACOS.P D2, FP3",
         0xF200|002, 0x401C|(3<<10)|(3<<7));
    TEST("FACOS.W D2, FP3",             0xF200|002, 0x401C|(4<<10)|(3<<7));
    ERRT("FACOS.D D2, FP3", ILLEGAL_SIZE, "FACOS.D D2, FP3",
         0xF200|002, 0x401C|(5<<10)|(3<<7));
    TEST("FACOS.B D2, FP3",             0xF200|002, 0x401C|(6<<10)|(3<<7));
    ERRT("FACOS.S A4, FP5",             OPERAND_NOT_ALLOWED, "A4, FP5", 0xF200, 0x469C);
    TEST("FACOS.X (A6), FP7",           0xF200|026, 0x401C|(2<<10)|(7<<7));
    TEST("FACOS.P (A0)+, FP1",          0xF200|030, 0x401C|(3<<10)|(1<<7));
    TEST("FACOS.W -(A2), FP3",          0xF200|042, 0x401C|(4<<10)|(3<<7));
    TEST("FACOS.D ($1234,A4), FP5",     0xF200|054, 0x401C|(5<<10)|(5<<7), 0x1234);
    TEST("FACOS.B ($23,A6,D7.W), FP0",  0xF200|066, 0x401C|(6<<10)|(0<<7), 0x7023);
    TEST("FACOS.L ($004566).W, FP1",    0xF200|070, 0x401C|(0<<10)|(1<<7), 0x4566);
    TEST("FACOS.S ($56789A).L, FP2",    0xF200|071, 0x401C|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FACOS.X (*+$1234,PC), FP3",   0xF200|072, 0x401C|(2<<10)|(3<<7), 0x1230);
    TEST("FACOS.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x401C|(3<<10)|(5<<7), 0xC856);
    TEST("FACOS.L #$6789ABCD, FP6",     0xF200|074, 0x401C|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FACOS.W #$1234, FP2",         0xF200|074, 0x401C|(4<<10)|(2<<7), 0x1234);
    TEST("FACOS.B #$23, FP4",           0xF200|074, 0x401C|(6<<10)|(4<<7), 0x0023);
    FLTS("FACOS.S", "#7.88999976E-10, FP7", 0xF200|074, 0x401C|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FACOS.X", "#-89000000032, FP0",   0xF200|074, 0x401C|(2<<10)|(0<<7),
         0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FACOS.P", "#9.12E20, FP1",        0xF200|074, 0x401C|(3<<10)|(1<<7),
         0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FACOS.D", "#-8.25, FP3",          0xF200|074, 0x401C|(5<<10)|(3<<7),
         0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FCOS.X FP0, FP1",            0xF200,     0x001D|(0<<10)|(1<<7));
    TEST("FCOS.X FP2",                 0xF200,     0x001D|(2<<10)|(2<<7));
    TEST("FCOS.L D2, FP3",             0xF200|002, 0x401D|(0<<10)|(3<<7));
    TEST("FCOS.S D2, FP3",             0xF200|002, 0x401D|(1<<10)|(3<<7));
    ERRT("FCOS.X D2, FP3", ILLEGAL_SIZE, "FCOS.X D2, FP3",
         0xF200|002, 0x401D|(2<<10)|(3<<7));
    ERRT("FCOS.P D2, FP3", ILLEGAL_SIZE, "FCOS.P D2, FP3",
         0xF200|002, 0x401D|(3<<10)|(3<<7));
    TEST("FCOS.W D2, FP3",             0xF200|002, 0x401D|(4<<10)|(3<<7));
    ERRT("FCOS.D D2, FP3", ILLEGAL_SIZE, "FCOS.D D2, FP3",
         0xF200|002, 0x401D|(5<<10)|(3<<7));
    TEST("FCOS.B D2, FP3",             0xF200|002, 0x401D|(6<<10)|(3<<7));
    ERRT("FCOS.S A4, FP5",             OPERAND_NOT_ALLOWED, "A4, FP5", 0xF200, 0x469D);
    TEST("FCOS.X (A6), FP7",           0xF200|026, 0x401D|(2<<10)|(7<<7));
    TEST("FCOS.P (A0)+, FP1",          0xF200|030, 0x401D|(3<<10)|(1<<7));
    TEST("FCOS.W -(A2), FP3",          0xF200|042, 0x401D|(4<<10)|(3<<7));
    TEST("FCOS.D ($1234,A4), FP5",     0xF200|054, 0x401D|(5<<10)|(5<<7), 0x1234);
    TEST("FCOS.B ($23,A6,D7.W), FP0",  0xF200|066, 0x401D|(6<<10)|(0<<7), 0x7023);
    TEST("FCOS.L ($004566).W, FP1",    0xF200|070, 0x401D|(0<<10)|(1<<7), 0x4566);
    TEST("FCOS.S ($56789A).L, FP2",    0xF200|071, 0x401D|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FCOS.X (*+$1234,PC), FP3",   0xF200|072, 0x401D|(2<<10)|(3<<7), 0x1230);
    TEST("FCOS.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x401D|(3<<10)|(5<<7), 0xC856);
    TEST("FCOS.L #$6789ABCD, FP6",     0xF200|074, 0x401D|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FCOS.W #$1234, FP2",         0xF200|074, 0x401D|(4<<10)|(2<<7), 0x1234);
    TEST("FCOS.B #$23, FP4",           0xF200|074, 0x401D|(6<<10)|(4<<7), 0x0023);
    FLTS("FCOS.S", "#7.88999976E-10, FP7", 0xF200|074, 0x401D|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FCOS.X", "#-89000000032, FP0",   0xF200|074, 0x401D|(2<<10)|(0<<7),
         0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FCOS.P", "#9.12E20, FP1",        0xF200|074, 0x401D|(3<<10)|(1<<7),
         0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FCOS.D", "#-8.25, FP3",          0xF200|074, 0x401D|(5<<10)|(3<<7),
         0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FGETEXP.X FP0, FP1",            0xF200,     0x001E|(0<<10)|(1<<7));
    TEST("FGETEXP.X FP2",                 0xF200,     0x001E|(2<<10)|(2<<7));
    TEST("FGETEXP.L D2, FP3",             0xF200|002, 0x401E|(0<<10)|(3<<7));
    TEST("FGETEXP.S D2, FP3",             0xF200|002, 0x401E|(1<<10)|(3<<7));
    ERRT("FGETEXP.X D2, FP3", ILLEGAL_SIZE, "FGETEXP.X D2, FP3",
         0xF200|002, 0x401E|(2<<10)|(3<<7));
    ERRT("FGETEXP.P D2, FP3", ILLEGAL_SIZE, "FGETEXP.P D2, FP3",
         0xF200|002, 0x401E|(3<<10)|(3<<7));
    TEST("FGETEXP.W D2, FP3",             0xF200|002, 0x401E|(4<<10)|(3<<7));
    ERRT("FGETEXP.D D2, FP3", ILLEGAL_SIZE, "FGETEXP.D D2, FP3",
         0xF200|002, 0x401E|(5<<10)|(3<<7));
    TEST("FGETEXP.B D2, FP3",             0xF200|002, 0x401E|(6<<10)|(3<<7));
    ERRT("FGETEXP.S A4, FP5",             OPERAND_NOT_ALLOWED, "A4, FP5", 0xF200, 0x469E);
    TEST("FGETEXP.X (A6), FP7",           0xF200|026, 0x401E|(2<<10)|(7<<7));
    TEST("FGETEXP.P (A0)+, FP1",          0xF200|030, 0x401E|(3<<10)|(1<<7));
    TEST("FGETEXP.W -(A2), FP3",          0xF200|042, 0x401E|(4<<10)|(3<<7));
    TEST("FGETEXP.D ($1234,A4), FP5",     0xF200|054, 0x401E|(5<<10)|(5<<7), 0x1234);
    TEST("FGETEXP.B ($23,A6,D7.W), FP0",  0xF200|066, 0x401E|(6<<10)|(0<<7), 0x7023);
    TEST("FGETEXP.L ($004566).W, FP1",    0xF200|070, 0x401E|(0<<10)|(1<<7), 0x4566);
    TEST("FGETEXP.S ($56789A).L, FP2",    0xF200|071, 0x401E|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FGETEXP.X (*+$1234,PC), FP3",   0xF200|072, 0x401E|(2<<10)|(3<<7), 0x1230);
    TEST("FGETEXP.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x401E|(3<<10)|(5<<7), 0xC856);
    TEST("FGETEXP.L #$6789ABCD, FP6",     0xF200|074, 0x401E|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FGETEXP.W #$1234, FP2",         0xF200|074, 0x401E|(4<<10)|(2<<7), 0x1234);
    TEST("FGETEXP.B #$23, FP4",           0xF200|074, 0x401E|(6<<10)|(4<<7), 0x0023);
    FLTS("FGETEXP.S", "#7.88999976E-10, FP7", 0xF200|074, 0x401E|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FGETEXP.X", "#-89000000032, FP0",   0xF200|074, 0x401E|(2<<10)|(0<<7),
         0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FGETEXP.P", "#9.12E20, FP1",        0xF200|074, 0x401E|(3<<10)|(1<<7),
         0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FGETEXP.D", "#-8.25, FP3",          0xF200|074, 0x401E|(5<<10)|(3<<7),
         0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FGETMAN.X FP0, FP1",            0xF200,     0x001F|(0<<10)|(1<<7));
    TEST("FGETMAN.X FP2",                 0xF200,     0x001F|(2<<10)|(2<<7));
    TEST("FGETMAN.L D2, FP3",             0xF200|002, 0x401F|(0<<10)|(3<<7));
    TEST("FGETMAN.S D2, FP3",             0xF200|002, 0x401F|(1<<10)|(3<<7));
    ERRT("FGETMAN.X D2, FP3", ILLEGAL_SIZE, "FGETMAN.X D2, FP3",
         0xF200|002, 0x401F|(2<<10)|(3<<7));
    ERRT("FGETMAN.P D2, FP3", ILLEGAL_SIZE, "FGETMAN.P D2, FP3",
         0xF200|002, 0x401F|(3<<10)|(3<<7));
    TEST("FGETMAN.W D2, FP3",             0xF200|002, 0x401F|(4<<10)|(3<<7));
    ERRT("FGETMAN.D D2, FP3", ILLEGAL_SIZE, "FGETMAN.D D2, FP3",
         0xF200|002, 0x401F|(5<<10)|(3<<7));
    TEST("FGETMAN.B D2, FP3",             0xF200|002, 0x401F|(6<<10)|(3<<7));
    ERRT("FGETMAN.S A4, FP5",             OPERAND_NOT_ALLOWED, "A4, FP5", 0xF200, 0x469F);
    TEST("FGETMAN.X (A6), FP7",           0xF200|026, 0x401F|(2<<10)|(7<<7));
    TEST("FGETMAN.P (A0)+, FP1",          0xF200|030, 0x401F|(3<<10)|(1<<7));
    TEST("FGETMAN.W -(A2), FP3",          0xF200|042, 0x401F|(4<<10)|(3<<7));
    TEST("FGETMAN.D ($1234,A4), FP5",     0xF200|054, 0x401F|(5<<10)|(5<<7), 0x1234);
    TEST("FGETMAN.B ($23,A6,D7.W), FP0",  0xF200|066, 0x401F|(6<<10)|(0<<7), 0x7023);
    TEST("FGETMAN.L ($004566).W, FP1",    0xF200|070, 0x401F|(0<<10)|(1<<7), 0x4566);
    TEST("FGETMAN.S ($56789A).L, FP2",    0xF200|071, 0x401F|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FGETMAN.X (*+$1234,PC), FP3",   0xF200|072, 0x401F|(2<<10)|(3<<7), 0x1230);
    TEST("FGETMAN.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x401F|(3<<10)|(5<<7), 0xC856);
    TEST("FGETMAN.L #$6789ABCD, FP6",     0xF200|074, 0x401F|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FGETMAN.W #$1234, FP2",         0xF200|074, 0x401F|(4<<10)|(2<<7), 0x1234);
    TEST("FGETMAN.B #$23, FP4",           0xF200|074, 0x401F|(6<<10)|(4<<7), 0x0023);
    FLTS("FGETMAN.S", "#7.88999976E-10, FP7", 0xF200|074, 0x401F|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FGETMAN.X", "#-89000000032, FP0",   0xF200|074, 0x401F|(2<<10)|(0<<7),
         0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FGETMAN.P", "#9.12E20, FP1",        0xF200|074, 0x401F|(3<<10)|(1<<7),
         0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FGETMAN.D", "#-8.25, FP3",          0xF200|074, 0x401F|(5<<10)|(3<<7),
         0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FMOD.X FP0, FP1",            0xF200,     0x0021|(0<<10)|(1<<7));
    ERRT("FMOD.X FP0",                 OPERAND_NOT_ALLOWED, "FP0");
    TEST("FMOD.L D2, FP3",             0xF200|002, 0x4021|(0<<10)|(3<<7));
    TEST("FMOD.S D2, FP3",             0xF200|002, 0x4021|(1<<10)|(3<<7));
    ERRT("FMOD.X D2, FP3", ILLEGAL_SIZE, "FMOD.X D2, FP3",
         0xF200|002, 0x4021|(2<<10)|(3<<7));
    ERRT("FMOD.P D2, FP3", ILLEGAL_SIZE, "FMOD.P D2, FP3",
         0xF200|002, 0x4021|(3<<10)|(3<<7));
    TEST("FMOD.W D2, FP3",             0xF200|002, 0x4021|(4<<10)|(3<<7));
    ERRT("FMOD.D D2, FP3", ILLEGAL_SIZE, "FMOD.D D2, FP3",
         0xF200|002, 0x4021|(5<<10)|(3<<7));
    TEST("FMOD.B D2, FP3",             0xF200|002, 0x4021|(6<<10)|(3<<7));
    ERRT("FMOD.S A4, FP5",             OPERAND_NOT_ALLOWED, "A4, FP5", 0xF200, 0x46A1);
    TEST("FMOD.X (A6), FP7",           0xF200|026, 0x4021|(2<<10)|(7<<7));
    TEST("FMOD.P (A0)+, FP1",          0xF200|030, 0x4021|(3<<10)|(1<<7));
    TEST("FMOD.W -(A2), FP3",          0xF200|042, 0x4021|(4<<10)|(3<<7));
    TEST("FMOD.D ($1234,A4), FP5",     0xF200|054, 0x4021|(5<<10)|(5<<7), 0x1234);
    TEST("FMOD.B ($23,A6,D7.W), FP0",  0xF200|066, 0x4021|(6<<10)|(0<<7), 0x7023);
    TEST("FMOD.L ($004566).W, FP1",    0xF200|070, 0x4021|(0<<10)|(1<<7), 0x4566);
    TEST("FMOD.S ($56789A).L, FP2",    0xF200|071, 0x4021|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FMOD.X (*+$1234,PC), FP3",   0xF200|072, 0x4021|(2<<10)|(3<<7), 0x1230);
    TEST("FMOD.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x4021|(3<<10)|(5<<7), 0xC856);
    TEST("FMOD.L #$6789ABCD, FP6",     0xF200|074, 0x4021|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FMOD.W #$1234, FP2",         0xF200|074, 0x4021|(4<<10)|(2<<7), 0x1234);
    TEST("FMOD.B #$23, FP4",           0xF200|074, 0x4021|(6<<10)|(4<<7), 0x0023);
    FLTS("FMOD.S", "#7.88999976E-10, FP7", 0xF200|074, 0x4021|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FMOD.X", "#-89000000032, FP0",   0xF200|074, 0x4021|(2<<10)|(0<<7),
         0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FMOD.P", "#9.12E20, FP1",        0xF200|074, 0x4021|(3<<10)|(1<<7),
         0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FMOD.D", "#-8.25, FP3",          0xF200|074, 0x4021|(5<<10)|(3<<7),
         0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FREM.X FP0, FP1",            0xF200,     0x0025|(0<<10)|(1<<7));
    ERRT("FREM.X FP0",                 OPERAND_NOT_ALLOWED, "FP0");
    TEST("FREM.L D2, FP3",             0xF200|002, 0x4025|(0<<10)|(3<<7));
    TEST("FREM.S D2, FP3",             0xF200|002, 0x4025|(1<<10)|(3<<7));
    ERRT("FREM.X D2, FP3", ILLEGAL_SIZE, "FREM.X D2, FP3",
         0xF200|002, 0x4025|(2<<10)|(3<<7));
    ERRT("FREM.P D2, FP3", ILLEGAL_SIZE, "FREM.P D2, FP3",
         0xF200|002, 0x4025|(3<<10)|(3<<7));
    TEST("FREM.W D2, FP3",             0xF200|002, 0x4025|(4<<10)|(3<<7));
    ERRT("FREM.D D2, FP3", ILLEGAL_SIZE, "FREM.D D2, FP3",
         0xF200|002, 0x4025|(5<<10)|(3<<7));
    TEST("FREM.B D2, FP3",             0xF200|002, 0x4025|(6<<10)|(3<<7));
    ERRT("FREM.S A4, FP5",             OPERAND_NOT_ALLOWED, "A4, FP5", 0xF200, 0x46A5);
    TEST("FREM.X (A6), FP7",           0xF200|026, 0x4025|(2<<10)|(7<<7));
    TEST("FREM.P (A0)+, FP1",          0xF200|030, 0x4025|(3<<10)|(1<<7));
    TEST("FREM.W -(A2), FP3",          0xF200|042, 0x4025|(4<<10)|(3<<7));
    TEST("FREM.D ($1234,A4), FP5",     0xF200|054, 0x4025|(5<<10)|(5<<7), 0x1234);
    TEST("FREM.B ($23,A6,D7.W), FP0",  0xF200|066, 0x4025|(6<<10)|(0<<7), 0x7023);
    TEST("FREM.L ($004566).W, FP1",    0xF200|070, 0x4025|(0<<10)|(1<<7), 0x4566);
    TEST("FREM.S ($56789A).L, FP2",    0xF200|071, 0x4025|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FREM.X (*+$1234,PC), FP3",   0xF200|072, 0x4025|(2<<10)|(3<<7), 0x1230);
    TEST("FREM.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x4025|(3<<10)|(5<<7), 0xC856);
    TEST("FREM.L #$6789ABCD, FP6",     0xF200|074, 0x4025|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FREM.W #$1234, FP2",         0xF200|074, 0x4025|(4<<10)|(2<<7), 0x1234);
    TEST("FREM.B #$23, FP4",           0xF200|074, 0x4025|(6<<10)|(4<<7), 0x0023);
    FLTS("FREM.S", "#7.88999976E-10, FP7", 0xF200|074, 0x4025|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FREM.X", "#-89000000032, FP0",   0xF200|074, 0x4025|(2<<10)|(0<<7),
         0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FREM.P", "#9.12E20, FP1",        0xF200|074, 0x4025|(3<<10)|(1<<7),
         0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FREM.D", "#-8.25, FP3",          0xF200|074, 0x4025|(5<<10)|(3<<7),
         0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FSCALE.X FP0, FP1",            0xF200,     0x0026|(0<<10)|(1<<7));
    ERRT("FSCALE.X FP0",                 OPERAND_NOT_ALLOWED, "FP0");
    TEST("FSCALE.L D2, FP3",             0xF200|002, 0x4026|(0<<10)|(3<<7));
    TEST("FSCALE.S D2, FP3",             0xF200|002, 0x4026|(1<<10)|(3<<7));
    ERRT("FSCALE.X D2, FP3", ILLEGAL_SIZE, "FSCALE.X D2, FP3",
         0xF200|002, 0x4026|(2<<10)|(3<<7));
    ERRT("FSCALE.P D2, FP3", ILLEGAL_SIZE, "FSCALE.P D2, FP3",
         0xF200|002, 0x4026|(3<<10)|(3<<7));
    TEST("FSCALE.W D2, FP3",             0xF200|002, 0x4026|(4<<10)|(3<<7));
    ERRT("FSCALE.D D2, FP3", ILLEGAL_SIZE, "FSCALE.D D2, FP3",
         0xF200|002, 0x4026|(5<<10)|(3<<7));
    TEST("FSCALE.B D2, FP3",             0xF200|002, 0x4026|(6<<10)|(3<<7));
    ERRT("FSCALE.S A4, FP5",             OPERAND_NOT_ALLOWED, "A4, FP5", 0xF200, 0x46A6);
    TEST("FSCALE.X (A6), FP7",           0xF200|026, 0x4026|(2<<10)|(7<<7));
    TEST("FSCALE.P (A0)+, FP1",          0xF200|030, 0x4026|(3<<10)|(1<<7));
    TEST("FSCALE.W -(A2), FP3",          0xF200|042, 0x4026|(4<<10)|(3<<7));
    TEST("FSCALE.D ($1234,A4), FP5",     0xF200|054, 0x4026|(5<<10)|(5<<7), 0x1234);
    TEST("FSCALE.B ($23,A6,D7.W), FP0",  0xF200|066, 0x4026|(6<<10)|(0<<7), 0x7023);
    TEST("FSCALE.L ($004566).W, FP1",    0xF200|070, 0x4026|(0<<10)|(1<<7), 0x4566);
    TEST("FSCALE.S ($56789A).L, FP2",    0xF200|071, 0x4026|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FSCALE.X (*+$1234,PC), FP3",   0xF200|072, 0x4026|(2<<10)|(3<<7), 0x1230);
    TEST("FSCALE.P (*+90,PC,A4.L), FP5", 0xF200|073, 0x4026|(3<<10)|(5<<7), 0xC856);
    TEST("FSCALE.L #$6789ABCD, FP6",     0xF200|074, 0x4026|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FSCALE.W #$1234, FP2",         0xF200|074, 0x4026|(4<<10)|(2<<7), 0x1234);
    TEST("FSCALE.B #$23, FP4",           0xF200|074, 0x4026|(6<<10)|(4<<7), 0x0023);
    FLTS("FSCALE.S", "#7.88999976E-10, FP7", 0xF200|074, 0x4026|(1<<10)|(7<<7), 0x3058, 0xE0F0);
    FLTX("FSCALE.X", "#-89000000032, FP0",   0xF200|074, 0x4026|(2<<10)|(0<<7),
         0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FSCALE.P", "#9.12E20, FP1",        0xF200|074, 0x4026|(3<<10)|(1<<7),
         0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FSCALE.D", "#-8.25, FP3",          0xF200|074, 0x4026|(5<<10)|(3<<7),
         0xC020, 0x8000, 0x0000, 0x0000);

    TEST("FSINCOS.X FP0, FP2:FP1",            0xF200,     0x0032|(0<<10)|(1<<7));
    TEST("FSINCOS.X FP0, FP0:FP0",            0xF200,     0x0030|(0<<10)|(0<<7));
    TEST("FSINCOS.L D2, FP1:FP3",             0xF200|002, 0x4031|(0<<10)|(3<<7));
    TEST("FSINCOS.S D2, FP4:FP3",             0xF200|002, 0x4034|(1<<10)|(3<<7));
    ERRT("FSINCOS.X D2, FP4:FP3", ILLEGAL_SIZE, "FSINCOS.X D2, FP4:FP3",
         0xF200|002, 0x4034|(2<<10)|(3<<7));
    ERRT("FSINCOS.P D2, FP4:FP3", ILLEGAL_SIZE, "FSINCOS.P D2, FP4:FP3",
         0xF200|002, 0x4034|(3<<10)|(3<<7));
    TEST("FSINCOS.W D2, FP4:FP3",             0xF200|002, 0x4034|(4<<10)|(3<<7));
    ERRT("FSINCOS.D D2, FP4:FP3", ILLEGAL_SIZE, "FSINCOS.D D2, FP4:FP3",
         0xF200|002, 0x4034|(5<<10)|(3<<7));
    TEST("FSINCOS.B D2, FP4:FP3",             0xF200|002, 0x4034|(6<<10)|(3<<7));
    ERRT("FSINCOS.S A4, FP2:FP5",             OPERAND_NOT_ALLOWED, "A4, FP2:FP5", 0xF200, 0x46B2);
    TEST("FSINCOS.X (A6), FP0:FP7",           0xF200|026, 0x4030|(2<<10)|(7<<7));
    TEST("FSINCOS.P (A0)+, FP7:FP1",          0xF200|030, 0x4037|(3<<10)|(1<<7));
    TEST("FSINCOS.W -(A2), FP6:FP3",          0xF200|042, 0x4036|(4<<10)|(3<<7));
    TEST("FSINCOS.D ($1234,A4), FP5:FP5",     0xF200|054, 0x4035|(5<<10)|(5<<7), 0x1234);
    TEST("FSINCOS.B ($23,A6,D7.W), FP4:FP0",  0xF200|066, 0x4034|(6<<10)|(0<<7), 0x7023);
    TEST("FSINCOS.L ($004566).W, FP3:FP1",    0xF200|070, 0x4033|(0<<10)|(1<<7), 0x4566);
    TEST("FSINCOS.S ($56789A).L, FP1:FP2",    0xF200|071, 0x4031|(1<<10)|(2<<7), 0x0056, 0x789A);
    TEST("FSINCOS.X (*+$1234,PC), FP0:FP3",   0xF200|072, 0x4030|(2<<10)|(3<<7), 0x1230);
    TEST("FSINCOS.P (*+90,PC,A4.L), FP7:FP5", 0xF200|073, 0x4037|(3<<10)|(5<<7), 0xC856);
    TEST("FSINCOS.L #$6789ABCD, FP5:FP6",     0xF200|074, 0x4035|(0<<10)|(6<<7), 0x6789, 0xABCD);
    TEST("FSINCOS.W #$1234, FP1:FP2",         0xF200|074, 0x4031|(4<<10)|(2<<7), 0x1234);
    TEST("FSINCOS.B #$23, FP7:FP4",           0xF200|074, 0x4037|(6<<10)|(4<<7), 0x0023);
    FLTS("FSINCOS.S", "#7.88999976E-10, FP6:FP7", 0xF200|074, 0x4036|(1<<10)|(7<<7),
         0x3058, 0xE0F0);
    FLTX("FSINCOS.X", "#-89000000032, FP4:FP0",   0xF200|074, 0x4034|(2<<10)|(0<<7),
         0xC023, 0x0000, 0xA5C6, 0x81D1, 0x0000, 0x0000);
    FLTP("FSINCOS.P", "#9.12E20, FP3:FP1",        0xF200|074, 0x4033|(3<<10)|(1<<7),
         0x0020, 0x0009, 0x1200, 0x0000, 0x0000, 0x0000);
    FLTD("FSINCOS.D", "#-8.25, FP0:FP3",          0xF200|074, 0x4030|(5<<10)|(3<<7),
         0xC020, 0x8000, 0x0000, 0x0000);
}

void test_float_branch() {
    TEST("FPU ON");

    TEST("FNOP",              0xF280, 0x0000);
    TEST("FBF    *+$1234",    0xF280, 0x1232);
    TEST("FBEQ.W *+$1234",    0xF281, 0x1232);
    TEST("FBEQ.L *+$1234",    0xF281, 0x1232);
    TEST("FBOGT  *+$1234",    0xF282, 0x1232);
    TEST("FBOGE  *+$1234",    0xF283, 0x1232);
    TEST("FBOLT  *+$1234",    0xF284, 0x1232);
    TEST("FBOLE  *+$1234",    0xF285, 0x1232);
    TEST("FBOGL  *+$1234",    0xF286, 0x1232);
    TEST("FBOR   *+$1234",    0xF287, 0x1232);
    TEST("FBUN   *+$1234",    0xF288, 0x1232);
    TEST("FBUEQ  *+$1234",    0xF289, 0x1232);
    TEST("FBUGT  *+$1234",    0xF28A, 0x1232);
    TEST("FBUGE  *+$1234",    0xF28B, 0x1232);
    TEST("FBULT  *+$1234",    0xF28C, 0x1232);
    TEST("FBULE  *+$1234",    0xF28D, 0x1232);
    TEST("FBNE   *+$1234",    0xF28E, 0x1232);
    TEST("FBT.X  *+$1234",    0xF2CF, 0x0000, 0x1232);
    TEST("FBSF   *+$1234",    0xF290, 0x1232);
    TEST("FBSEQ  *+$1234",    0xF291, 0x1232);
    TEST("FBGT   *+$1234",    0xF292, 0x1232);
    TEST("FBGE   *+$1234",    0xF293, 0x1232);
    TEST("FBLT   *+$1234",    0xF294, 0x1232);
    TEST("FBLE   *+$1234",    0xF295, 0x1232);
    TEST("FBGL   *+$1234",    0xF296, 0x1232);
    TEST("FBGLE  *+$1234",    0xF297, 0x1232);
    TEST("FBNGLE *+$1234",    0xF298, 0x1232);
    TEST("FBNGL  *+$1234",    0xF299, 0x1232);
    TEST("FBNLE  *+$1234",    0xF29A, 0x1232);
    TEST("FBNLT  *+$1234",    0xF29B, 0x1232);
    TEST("FBNGE  *+$1234",    0xF29C, 0x1232);
    TEST("FBNGT  *+$1234",    0xF29D, 0x1232);
    TEST("FBSNE  *+$1234",    0xF29E, 0x1232);
    TEST("FBST   *+$1234",    0xF29F, 0x1232);
    ERRT("FBST   *-$00FE",  OVERFLOW_RANGE, "*-$00FE", 0xF29F, 0xFF00);
    TEST("FBST   *+$123456",  0xF2DF, 0x0012, 0x3454);
    TEST("FBST.W *+$70",      0xF29F, 0x006E);
    TEST("FBST.L *+$70",      0xF29F, 0x006E);
    TEST("FBST.X *+$70",      0xF2DF, 0x0000, 0x006E);
    ERRT("FBST.B *+$70", ILLEGAL_SIZE, "FBST.B *+$70", 0xF2DF, 0x0000, 0x006E);
    ERRT("FBST.S *+$70", ILLEGAL_SIZE, "FBST.S *+$70", 0xF2DF, 0x0000, 0x006E);

    TEST("FBF    *+$123456",    0xF2C0, 0x0012, 0x3454);
    TEST("FBEQ.X *+$001234",    0xF2C1, 0x0000, 0x1232);
    TEST("FBOGT  *+$123456",    0xF2C2, 0x0012, 0x3454);
    TEST("FBOGE  *+$123456",    0xF2C3, 0x0012, 0x3454);
    TEST("FBOLT  *+$123456",    0xF2C4, 0x0012, 0x3454);
    TEST("FBOLE  *+$123456",    0xF2C5, 0x0012, 0x3454);
    TEST("FBOGL  *+$123456",    0xF2C6, 0x0012, 0x3454);
    TEST("FBOR   *+$123456",    0xF2C7, 0x0012, 0x3454);
    TEST("FBUN   *+$123456",    0xF2C8, 0x0012, 0x3454);
    TEST("FBUEQ  *+$123456",    0xF2C9, 0x0012, 0x3454);
    TEST("FBUGT  *+$123456",    0xF2CA, 0x0012, 0x3454);
    TEST("FBUGE  *+$123456",    0xF2CB, 0x0012, 0x3454);
    TEST("FBULT  *+$123456",    0xF2CC, 0x0012, 0x3454);
    TEST("FBULE  *+$123456",    0xF2CD, 0x0012, 0x3454);
    TEST("FBNE   *+$123456",    0xF2CE, 0x0012, 0x3454);
    TEST("FBT    *+$123456",    0xF2CF, 0x0012, 0x3454);
    ERRT("FBT.W  *+$123456", OPERAND_TOO_FAR, "*+$123456", 0xF28F, 0x3454);
    ERRT("FBT.L  *+$123456", OPERAND_TOO_FAR, "*+$123456", 0xF28F, 0x3454);
    TEST("FBSF   *+$123456",    0xF2D0, 0x0012, 0x3454);
    TEST("FBSEQ  *+$123456",    0xF2D1, 0x0012, 0x3454);
    TEST("FBGT   *+$123456",    0xF2D2, 0x0012, 0x3454);
    TEST("FBGE   *+$123456",    0xF2D3, 0x0012, 0x3454);
    TEST("FBLT   *+$123456",    0xF2D4, 0x0012, 0x3454);
    TEST("FBLE   *+$123456",    0xF2D5, 0x0012, 0x3454);
    TEST("FBGL   *+$123456",    0xF2D6, 0x0012, 0x3454);
    TEST("FBGLE  *+$123456",    0xF2D7, 0x0012, 0x3454);
    TEST("FBNGLE *+$123456",    0xF2D8, 0x0012, 0x3454);
    TEST("FBNGL  *+$123456",    0xF2D9, 0x0012, 0x3454);
    TEST("FBNLE  *+$123456",    0xF2DA, 0x0012, 0x3454);
    TEST("FBNLT  *+$123456",    0xF2DB, 0x0012, 0x3454);
    TEST("FBNGE  *+$123456",    0xF2DC, 0x0012, 0x3454);
    TEST("FBNGT  *+$123456",    0xF2DD, 0x0012, 0x3454);
    TEST("FBSNE  *+$123456",    0xF2DE, 0x0012, 0x3454);
    TEST("FBST   *+$123456",    0xF2DF, 0x0012, 0x3454);
    ATEST(0x10000, "FBST   $723456",  0xF2DF, 0x0071, 0x3454);
    ATEST(0x10000, "FBST   $F23456",  0xF2DF, 0x00F1, 0x3454);
    if (firstGen()) {
        AERRT(0x10000, "FBST $12345678", OVERFLOW_RANGE, "$12345678", 0xF2DF, 0x1233, 0x5676);
    } else {
        ATEST(0x10000, "FBST $12345678", 0xF2DF, 0x1233, 0x5676);
    }
}

void test_float_trap() {
    TEST("FPU ON");

    TEST("FSF D2",              0xF240|002, 0x0000);
    TEST("FDBF D2, *+$1234",    0xF240|012, 0x0000, 0x1230);
    TEST("FSF (A2)",            0xF240|022, 0x0000);
    TEST("FSF (A2)+",           0xF240|032, 0x0000);
    TEST("FSF -(A2)",           0xF240|042, 0x0000);
    TEST("FSF ($1234,A2)",      0xF240|052, 0x0000, 0x1234);
    TEST("FSF (18,A2,D3.W)",    0xF240|062, 0x0000, 0x3012);
    TEST("FSF ($001234).W",     0xF240|070, 0x0000, 0x1234);
    TEST("FSF ($123456).L",     0xF240|071, 0x0000, 0x0012, 0x3456);
    TEST("FTRAPF.W #$1234",     0xF240|072, 0x0000, 0x1234);
    TEST("FTRAPF.L #$12345678", 0xF240|073, 0x0000, 0x1234, 0x5678);
    TEST("FTRAPF",              0xF240|074, 0x0000);

    TEST("FSEQ D2",              0xF240|002, 0x0001);
    TEST("FDBEQ D2, *+$1234",    0xF240|012, 0x0001, 0x1230);
    TEST("FDBEQ.L D2, *+$1234",  0xF240|012, 0x0001, 0x1230);
    TEST("FDBEQ.W D2, *+$1234",  0xF240|012, 0x0001, 0x1230);
    ERRT("FDBEQ.B D2, *+$1234",  ILLEGAL_SIZE, "FDBEQ.B D2, *+$1234", 0xF240|012, 0x0001, 0x1230);
    ERRT("FDBEQ.S D2, *+$1234",  ILLEGAL_SIZE, "FDBEQ.S D2, *+$1234", 0xF240|012, 0x0001, 0x1230);
    ERRT("FDBEQ.X D2, *+$1234",  ILLEGAL_SIZE, "FDBEQ.X D2, *+$1234", 0xF240|012, 0x0001, 0x1230);
    TEST("FSEQ (A2)",            0xF240|022, 0x0001);
    TEST("FSEQ (A2)+",           0xF240|032, 0x0001);
    TEST("FSEQ -(A2)",           0xF240|042, 0x0001);
    TEST("FSEQ ($1234,A2)",      0xF240|052, 0x0001, 0x1234);
    TEST("FSEQ (18,A2,D3.W)",    0xF240|062, 0x0001, 0x3012);
    TEST("FSEQ ($001234).W",     0xF240|070, 0x0001, 0x1234);
    TEST("FSEQ ($123456).L",     0xF240|071, 0x0001, 0x0012, 0x3456);
    TEST("FTRAPEQ.W #$1234",     0xF240|072, 0x0001, 0x1234);
    TEST("FTRAPEQ.L #$12345678", 0xF240|073, 0x0001, 0x1234, 0x5678);
    TEST("FTRAPEQ",              0xF240|074, 0x0001);

    TEST("FSOGT D2",              0xF240|002, 0x0002);
    TEST("FDBOGT D2, *+$1234",    0xF240|012, 0x0002, 0x1230);
    TEST("FSOGT (A2)",            0xF240|022, 0x0002);
    TEST("FSOGT (A2)+",           0xF240|032, 0x0002);
    TEST("FSOGT -(A2)",           0xF240|042, 0x0002);
    TEST("FSOGT ($1234,A2)",      0xF240|052, 0x0002, 0x1234);
    TEST("FSOGT (18,A2,D3.W)",    0xF240|062, 0x0002, 0x3012);
    TEST("FSOGT ($001234).W",     0xF240|070, 0x0002, 0x1234);
    TEST("FSOGT ($123456).L",     0xF240|071, 0x0002, 0x0012, 0x3456);
    TEST("FTRAPOGT.W #$1234",     0xF240|072, 0x0002, 0x1234);
    TEST("FTRAPOGT.L #$12345678", 0xF240|073, 0x0002, 0x1234, 0x5678);
    TEST("FTRAPOGT",              0xF240|074, 0x0002);

    TEST("FSOGE D2",              0xF240|002, 0x0003);
    TEST("FDBOGE D2, *+$1234",    0xF240|012, 0x0003, 0x1230);
    TEST("FSOGE (A2)",            0xF240|022, 0x0003);
    TEST("FSOGE (A2)+",           0xF240|032, 0x0003);
    TEST("FSOGE -(A2)",           0xF240|042, 0x0003);
    TEST("FSOGE ($1234,A2)",      0xF240|052, 0x0003, 0x1234);
    TEST("FSOGE (18,A2,D3.W)",    0xF240|062, 0x0003, 0x3012);
    TEST("FSOGE ($001234).W",     0xF240|070, 0x0003, 0x1234);
    TEST("FSOGE ($123456).L",     0xF240|071, 0x0003, 0x0012, 0x3456);
    TEST("FTRAPOGE.W #$1234",     0xF240|072, 0x0003, 0x1234);
    TEST("FTRAPOGE.L #$12345678", 0xF240|073, 0x0003, 0x1234, 0x5678);
    TEST("FTRAPOGE",              0xF240|074, 0x0003);

    TEST("FSOLT D2",              0xF240|002, 0x0004);
    TEST("FDBOLT D2, *+$1234",    0xF240|012, 0x0004, 0x1230);
    TEST("FSOLT (A2)",            0xF240|022, 0x0004);
    TEST("FSOLT (A2)+",           0xF240|032, 0x0004);
    TEST("FSOLT -(A2)",           0xF240|042, 0x0004);
    TEST("FSOLT ($1234,A2)",      0xF240|052, 0x0004, 0x1234);
    TEST("FSOLT (18,A2,D3.W)",    0xF240|062, 0x0004, 0x3012);
    TEST("FSOLT ($001234).W",     0xF240|070, 0x0004, 0x1234);
    TEST("FSOLT ($123456).L",     0xF240|071, 0x0004, 0x0012, 0x3456);
    TEST("FTRAPOLT.W #$1234",     0xF240|072, 0x0004, 0x1234);
    TEST("FTRAPOLT.L #$12345678", 0xF240|073, 0x0004, 0x1234, 0x5678);
    TEST("FTRAPOLT",              0xF240|074, 0x0004);

    TEST("FSOLE D2",              0xF240|002, 0x0005);
    TEST("FDBOLE D2, *+$1234",    0xF240|012, 0x0005, 0x1230);
    TEST("FSOLE (A2)",            0xF240|022, 0x0005);
    TEST("FSOLE (A2)+",           0xF240|032, 0x0005);
    TEST("FSOLE -(A2)",           0xF240|042, 0x0005);
    TEST("FSOLE ($1234,A2)",      0xF240|052, 0x0005, 0x1234);
    TEST("FSOLE (18,A2,D3.W)",    0xF240|062, 0x0005, 0x3012);
    TEST("FSOLE ($001234).W",     0xF240|070, 0x0005, 0x1234);
    TEST("FSOLE ($123456).L",     0xF240|071, 0x0005, 0x0012, 0x3456);
    TEST("FTRAPOLE.W #$1234",     0xF240|072, 0x0005, 0x1234);
    TEST("FTRAPOLE.L #$12345678", 0xF240|073, 0x0005, 0x1234, 0x5678);
    TEST("FTRAPOLE",              0xF240|074, 0x0005);

    TEST("FSOGL D2",              0xF240|002, 0x0006);
    TEST("FDBOGL D2, *+$1234",    0xF240|012, 0x0006, 0x1230);
    TEST("FSOGL (A2)",            0xF240|022, 0x0006);
    TEST("FSOGL (A2)+",           0xF240|032, 0x0006);
    TEST("FSOGL -(A2)",           0xF240|042, 0x0006);
    TEST("FSOGL ($1234,A2)",      0xF240|052, 0x0006, 0x1234);
    TEST("FSOGL (18,A2,D3.W)",    0xF240|062, 0x0006, 0x3012);
    TEST("FSOGL ($001234).W",     0xF240|070, 0x0006, 0x1234);
    TEST("FSOGL ($123456).L",     0xF240|071, 0x0006, 0x0012, 0x3456);
    TEST("FTRAPOGL.W #$1234",     0xF240|072, 0x0006, 0x1234);
    TEST("FTRAPOGL.L #$12345678", 0xF240|073, 0x0006, 0x1234, 0x5678);
    TEST("FTRAPOGL",              0xF240|074, 0x0006);

    TEST("FSOR D2",              0xF240|002, 0x0007);
    TEST("FDBOR D2, *+$1234",    0xF240|012, 0x0007, 0x1230);
    TEST("FSOR (A2)",            0xF240|022, 0x0007);
    TEST("FSOR (A2)+",           0xF240|032, 0x0007);
    TEST("FSOR -(A2)",           0xF240|042, 0x0007);
    TEST("FSOR ($1234,A2)",      0xF240|052, 0x0007, 0x1234);
    TEST("FSOR (18,A2,D3.W)",    0xF240|062, 0x0007, 0x3012);
    TEST("FSOR ($001234).W",     0xF240|070, 0x0007, 0x1234);
    TEST("FSOR ($123456).L",     0xF240|071, 0x0007, 0x0012, 0x3456);
    TEST("FTRAPOR.W #$1234",     0xF240|072, 0x0007, 0x1234);
    TEST("FTRAPOR.L #$12345678", 0xF240|073, 0x0007, 0x1234, 0x5678);
    TEST("FTRAPOR",              0xF240|074, 0x0007);

    TEST("FSUN D2",              0xF240|002, 0x0008);
    TEST("FDBUN D2, *+$1234",    0xF240|012, 0x0008, 0x1230);
    TEST("FSUN (A2)",            0xF240|022, 0x0008);
    TEST("FSUN (A2)+",           0xF240|032, 0x0008);
    TEST("FSUN -(A2)",           0xF240|042, 0x0008);
    TEST("FSUN ($1234,A2)",      0xF240|052, 0x0008, 0x1234);
    TEST("FSUN (18,A2,D3.W)",    0xF240|062, 0x0008, 0x3012);
    TEST("FSUN ($001234).W",     0xF240|070, 0x0008, 0x1234);
    TEST("FSUN ($123456).L",     0xF240|071, 0x0008, 0x0012, 0x3456);
    TEST("FTRAPUN.W #$1234",     0xF240|072, 0x0008, 0x1234);
    TEST("FTRAPUN.L #$12345678", 0xF240|073, 0x0008, 0x1234, 0x5678);
    TEST("FTRAPUN",              0xF240|074, 0x0008);

    TEST("FSUEQ D2",              0xF240|002, 0x0009);
    TEST("FDBUEQ D2, *+$1234",    0xF240|012, 0x0009, 0x1230);
    TEST("FSUEQ (A2)",            0xF240|022, 0x0009);
    TEST("FSUEQ (A2)+",           0xF240|032, 0x0009);
    TEST("FSUEQ -(A2)",           0xF240|042, 0x0009);
    TEST("FSUEQ ($1234,A2)",      0xF240|052, 0x0009, 0x1234);
    TEST("FSUEQ (18,A2,D3.W)",    0xF240|062, 0x0009, 0x3012);
    TEST("FSUEQ ($001234).W",     0xF240|070, 0x0009, 0x1234);
    TEST("FSUEQ ($123456).L",     0xF240|071, 0x0009, 0x0012, 0x3456);
    TEST("FTRAPUEQ.W #$1234",     0xF240|072, 0x0009, 0x1234);
    TEST("FTRAPUEQ.L #$12345678", 0xF240|073, 0x0009, 0x1234, 0x5678);
    TEST("FTRAPUEQ",              0xF240|074, 0x0009);

    TEST("FSUGT D2",              0xF240|002, 0x000A);
    TEST("FDBUGT D2, *+$1234",    0xF240|012, 0x000A, 0x1230);
    TEST("FSUGT (A2)",            0xF240|022, 0x000A);
    TEST("FSUGT (A2)+",           0xF240|032, 0x000A);
    TEST("FSUGT -(A2)",           0xF240|042, 0x000A);
    TEST("FSUGT ($1234,A2)",      0xF240|052, 0x000A, 0x1234);
    TEST("FSUGT (18,A2,D3.W)",    0xF240|062, 0x000A, 0x3012);
    TEST("FSUGT ($001234).W",     0xF240|070, 0x000A, 0x1234);
    TEST("FSUGT ($123456).L",     0xF240|071, 0x000A, 0x0012, 0x3456);
    TEST("FTRAPUGT.W #$1234",     0xF240|072, 0x000A, 0x1234);
    TEST("FTRAPUGT.L #$12345678", 0xF240|073, 0x000A, 0x1234, 0x5678);
    TEST("FTRAPUGT",              0xF240|074, 0x000A);

    TEST("FSUGE D2",              0xF240|002, 0x000B);
    TEST("FDBUGE D2, *+$1234",    0xF240|012, 0x000B, 0x1230);
    TEST("FSUGE (A2)",            0xF240|022, 0x000B);
    TEST("FSUGE (A2)+",           0xF240|032, 0x000B);
    TEST("FSUGE -(A2)",           0xF240|042, 0x000B);
    TEST("FSUGE ($1234,A2)",      0xF240|052, 0x000B, 0x1234);
    TEST("FSUGE (18,A2,D3.W)",    0xF240|062, 0x000B, 0x3012);
    TEST("FSUGE ($001234).W",     0xF240|070, 0x000B, 0x1234);
    TEST("FSUGE ($123456).L",     0xF240|071, 0x000B, 0x0012, 0x3456);
    TEST("FTRAPUGE.W #$1234",     0xF240|072, 0x000B, 0x1234);
    TEST("FTRAPUGE.L #$12345678", 0xF240|073, 0x000B, 0x1234, 0x5678);
    TEST("FTRAPUGE",              0xF240|074, 0x000B);

    TEST("FSULT D2",              0xF240|002, 0x000C);
    TEST("FDBULT D2, *+$1234",    0xF240|012, 0x000C, 0x1230);
    TEST("FSULT (A2)",            0xF240|022, 0x000C);
    TEST("FSULT (A2)+",           0xF240|032, 0x000C);
    TEST("FSULT -(A2)",           0xF240|042, 0x000C);
    TEST("FSULT ($1234,A2)",      0xF240|052, 0x000C, 0x1234);
    TEST("FSULT (18,A2,D3.W)",    0xF240|062, 0x000C, 0x3012);
    TEST("FSULT ($001234).W",     0xF240|070, 0x000C, 0x1234);
    TEST("FSULT ($123456).L",     0xF240|071, 0x000C, 0x0012, 0x3456);
    TEST("FTRAPULT.W #$1234",     0xF240|072, 0x000C, 0x1234);
    TEST("FTRAPULT.L #$12345678", 0xF240|073, 0x000C, 0x1234, 0x5678);
    TEST("FTRAPULT",              0xF240|074, 0x000C);

    TEST("FSULE D2",              0xF240|002, 0x000D);
    TEST("FDBULE D2, *+$1234",    0xF240|012, 0x000D, 0x1230);
    TEST("FSULE (A2)",            0xF240|022, 0x000D);
    TEST("FSULE (A2)+",           0xF240|032, 0x000D);
    TEST("FSULE -(A2)",           0xF240|042, 0x000D);
    TEST("FSULE ($1234,A2)",      0xF240|052, 0x000D, 0x1234);
    TEST("FSULE (18,A2,D3.W)",    0xF240|062, 0x000D, 0x3012);
    TEST("FSULE ($001234).W",     0xF240|070, 0x000D, 0x1234);
    TEST("FSULE ($123456).L",     0xF240|071, 0x000D, 0x0012, 0x3456);
    TEST("FTRAPULE.W #$1234",     0xF240|072, 0x000D, 0x1234);
    TEST("FTRAPULE.L #$12345678", 0xF240|073, 0x000D, 0x1234, 0x5678);
    TEST("FTRAPULE",              0xF240|074, 0x000D);

    TEST("FSNE D2",              0xF240|002, 0x000E);
    TEST("FDBNE D2, *+$1234",    0xF240|012, 0x000E, 0x1230);
    TEST("FSNE (A2)",            0xF240|022, 0x000E);
    TEST("FSNE (A2)+",           0xF240|032, 0x000E);
    TEST("FSNE -(A2)",           0xF240|042, 0x000E);
    TEST("FSNE ($1234,A2)",      0xF240|052, 0x000E, 0x1234);
    TEST("FSNE (18,A2,D3.W)",    0xF240|062, 0x000E, 0x3012);
    TEST("FSNE ($001234).W",     0xF240|070, 0x000E, 0x1234);
    TEST("FSNE ($123456).L",     0xF240|071, 0x000E, 0x0012, 0x3456);
    TEST("FTRAPNE.W #$1234",     0xF240|072, 0x000E, 0x1234);
    TEST("FTRAPNE.L #$12345678", 0xF240|073, 0x000E, 0x1234, 0x5678);
    TEST("FTRAPNE",              0xF240|074, 0x000E);

    TEST("FST D2",              0xF240|002, 0x000F);
    TEST("FDBT D2, *+$1234",    0xF240|012, 0x000F, 0x1230);
    TEST("FST (A2)",            0xF240|022, 0x000F);
    TEST("FST (A2)+",           0xF240|032, 0x000F);
    TEST("FST -(A2)",           0xF240|042, 0x000F);
    TEST("FST ($1234,A2)",      0xF240|052, 0x000F, 0x1234);
    TEST("FST (18,A2,D3.W)",    0xF240|062, 0x000F, 0x3012);
    TEST("FST ($001234).W",     0xF240|070, 0x000F, 0x1234);
    TEST("FST ($123456).L",     0xF240|071, 0x000F, 0x0012, 0x3456);
    TEST("FTRAPT.W #$1234",     0xF240|072, 0x000F, 0x1234);
    TEST("FTRAPT.L #$12345678", 0xF240|073, 0x000F, 0x1234, 0x5678);
    TEST("FTRAPT",              0xF240|074, 0x000F);

    TEST("FSSF D2",              0xF240|002, 0x0010);
    TEST("FDBSF D2, *+$1234",    0xF240|012, 0x0010, 0x1230);
    TEST("FSSF (A2)",            0xF240|022, 0x0010);
    TEST("FSSF (A2)+",           0xF240|032, 0x0010);
    TEST("FSSF -(A2)",           0xF240|042, 0x0010);
    TEST("FSSF ($1234,A2)",      0xF240|052, 0x0010, 0x1234);
    TEST("FSSF (18,A2,D3.W)",    0xF240|062, 0x0010, 0x3012);
    TEST("FSSF ($001234).W",     0xF240|070, 0x0010, 0x1234);
    TEST("FSSF ($123456).L",     0xF240|071, 0x0010, 0x0012, 0x3456);
    TEST("FTRAPSF.W #$1234",     0xF240|072, 0x0010, 0x1234);
    TEST("FTRAPSF.L #$12345678", 0xF240|073, 0x0010, 0x1234, 0x5678);
    TEST("FTRAPSF",              0xF240|074, 0x0010);

    TEST("FSSEQ D2",              0xF240|002, 0x0011);
    TEST("FDBSEQ D2, *+$1234",    0xF240|012, 0x0011, 0x1230);
    TEST("FSSEQ (A2)",            0xF240|022, 0x0011);
    TEST("FSSEQ (A2)+",           0xF240|032, 0x0011);
    TEST("FSSEQ -(A2)",           0xF240|042, 0x0011);
    TEST("FSSEQ ($1234,A2)",      0xF240|052, 0x0011, 0x1234);
    TEST("FSSEQ (18,A2,D3.W)",    0xF240|062, 0x0011, 0x3012);
    TEST("FSSEQ ($001234).W",     0xF240|070, 0x0011, 0x1234);
    TEST("FSSEQ ($123456).L",     0xF240|071, 0x0011, 0x0012, 0x3456);
    TEST("FTRAPSEQ.W #$1234",     0xF240|072, 0x0011, 0x1234);
    TEST("FTRAPSEQ.L #$12345678", 0xF240|073, 0x0011, 0x1234, 0x5678);
    TEST("FTRAPSEQ",              0xF240|074, 0x0011);

    TEST("FSGT D2",              0xF240|002, 0x0012);
    TEST("FDBGT D2, *+$1234",    0xF240|012, 0x0012, 0x1230);
    TEST("FSGT (A2)",            0xF240|022, 0x0012);
    TEST("FSGT (A2)+",           0xF240|032, 0x0012);
    TEST("FSGT -(A2)",           0xF240|042, 0x0012);
    TEST("FSGT ($1234,A2)",      0xF240|052, 0x0012, 0x1234);
    TEST("FSGT (18,A2,D3.W)",    0xF240|062, 0x0012, 0x3012);
    TEST("FSGT ($001234).W",     0xF240|070, 0x0012, 0x1234);
    TEST("FSGT ($123456).L",     0xF240|071, 0x0012, 0x0012, 0x3456);
    TEST("FTRAPGT.W #$1234",     0xF240|072, 0x0012, 0x1234);
    TEST("FTRAPGT.L #$12345678", 0xF240|073, 0x0012, 0x1234, 0x5678);
    TEST("FTRAPGT",              0xF240|074, 0x0012);

    TEST("FSGE D2",              0xF240|002, 0x0013);
    TEST("FDBGE D2, *+$1234",    0xF240|012, 0x0013, 0x1230);
    TEST("FSGE (A2)",            0xF240|022, 0x0013);
    TEST("FSGE (A2)+",           0xF240|032, 0x0013);
    TEST("FSGE -(A2)",           0xF240|042, 0x0013);
    TEST("FSGE ($1234,A2)",      0xF240|052, 0x0013, 0x1234);
    TEST("FSGE (18,A2,D3.W)",    0xF240|062, 0x0013, 0x3012);
    TEST("FSGE ($001234).W",     0xF240|070, 0x0013, 0x1234);
    TEST("FSGE ($123456).L",     0xF240|071, 0x0013, 0x0012, 0x3456);
    TEST("FTRAPGE.W #$1234",     0xF240|072, 0x0013, 0x1234);
    TEST("FTRAPGE.L #$12345678", 0xF240|073, 0x0013, 0x1234, 0x5678);
    TEST("FTRAPGE",              0xF240|074, 0x0013);

    TEST("FSLT D2",              0xF240|002, 0x0014);
    TEST("FDBLT D2, *+$1234",    0xF240|012, 0x0014, 0x1230);
    TEST("FSLT (A2)",            0xF240|022, 0x0014);
    TEST("FSLT (A2)+",           0xF240|032, 0x0014);
    TEST("FSLT -(A2)",           0xF240|042, 0x0014);
    TEST("FSLT ($1234,A2)",      0xF240|052, 0x0014, 0x1234);
    TEST("FSLT (18,A2,D3.W)",    0xF240|062, 0x0014, 0x3012);
    TEST("FSLT ($001234).W",     0xF240|070, 0x0014, 0x1234);
    TEST("FSLT ($123456).L",     0xF240|071, 0x0014, 0x0012, 0x3456);
    TEST("FTRAPLT.W #$1234",     0xF240|072, 0x0014, 0x1234);
    TEST("FTRAPLT.L #$12345678", 0xF240|073, 0x0014, 0x1234, 0x5678);
    TEST("FTRAPLT",              0xF240|074, 0x0014);

    TEST("FSLE D2",              0xF240|002, 0x0015);
    TEST("FDBLE D2, *+$1234",    0xF240|012, 0x0015, 0x1230);
    TEST("FSLE (A2)",            0xF240|022, 0x0015);
    TEST("FSLE (A2)+",           0xF240|032, 0x0015);
    TEST("FSLE -(A2)",           0xF240|042, 0x0015);
    TEST("FSLE ($1234,A2)",      0xF240|052, 0x0015, 0x1234);
    TEST("FSLE (18,A2,D3.W)",    0xF240|062, 0x0015, 0x3012);
    TEST("FSLE ($001234).W",     0xF240|070, 0x0015, 0x1234);
    TEST("FSLE ($123456).L",     0xF240|071, 0x0015, 0x0012, 0x3456);
    TEST("FTRAPLE.W #$1234",     0xF240|072, 0x0015, 0x1234);
    TEST("FTRAPLE.L #$12345678", 0xF240|073, 0x0015, 0x1234, 0x5678);
    TEST("FTRAPLE",              0xF240|074, 0x0015);

    TEST("FSGL D2",              0xF240|002, 0x0016);
    TEST("FDBGL D2, *+$1234",    0xF240|012, 0x0016, 0x1230);
    TEST("FSGL (A2)",            0xF240|022, 0x0016);
    TEST("FSGL (A2)+",           0xF240|032, 0x0016);
    TEST("FSGL -(A2)",           0xF240|042, 0x0016);
    TEST("FSGL ($1234,A2)",      0xF240|052, 0x0016, 0x1234);
    TEST("FSGL (18,A2,D3.W)",    0xF240|062, 0x0016, 0x3012);
    TEST("FSGL ($001234).W",     0xF240|070, 0x0016, 0x1234);
    TEST("FSGL ($123456).L",     0xF240|071, 0x0016, 0x0012, 0x3456);
    TEST("FTRAPGL.W #$1234",     0xF240|072, 0x0016, 0x1234);
    TEST("FTRAPGL.L #$12345678", 0xF240|073, 0x0016, 0x1234, 0x5678);
    TEST("FTRAPGL",              0xF240|074, 0x0016);

    TEST("FSGLE D2",              0xF240|002, 0x0017);
    TEST("FDBGLE D2, *+$1234",    0xF240|012, 0x0017, 0x1230);
    TEST("FSGLE (A2)",            0xF240|022, 0x0017);
    TEST("FSGLE (A2)+",           0xF240|032, 0x0017);
    TEST("FSGLE -(A2)",           0xF240|042, 0x0017);
    TEST("FSGLE ($1234,A2)",      0xF240|052, 0x0017, 0x1234);
    TEST("FSGLE (18,A2,D3.W)",    0xF240|062, 0x0017, 0x3012);
    TEST("FSGLE ($001234).W",     0xF240|070, 0x0017, 0x1234);
    TEST("FSGLE ($123456).L",     0xF240|071, 0x0017, 0x0012, 0x3456);
    TEST("FTRAPGLE.W #$1234",     0xF240|072, 0x0017, 0x1234);
    TEST("FTRAPGLE.L #$12345678", 0xF240|073, 0x0017, 0x1234, 0x5678);
    TEST("FTRAPGLE",              0xF240|074, 0x0017);

    TEST("FSNGLE D2",              0xF240|002, 0x0018);
    TEST("FDBNGLE D2, *+$1234",    0xF240|012, 0x0018, 0x1230);
    TEST("FSNGLE (A2)",            0xF240|022, 0x0018);
    TEST("FSNGLE (A2)+",           0xF240|032, 0x0018);
    TEST("FSNGLE -(A2)",           0xF240|042, 0x0018);
    TEST("FSNGLE ($1234,A2)",      0xF240|052, 0x0018, 0x1234);
    TEST("FSNGLE (18,A2,D3.W)",    0xF240|062, 0x0018, 0x3012);
    TEST("FSNGLE ($001234).W",     0xF240|070, 0x0018, 0x1234);
    TEST("FSNGLE ($123456).L",     0xF240|071, 0x0018, 0x0012, 0x3456);
    TEST("FTRAPNGLE.W #$1234",     0xF240|072, 0x0018, 0x1234);
    TEST("FTRAPNGLE.L #$12345678", 0xF240|073, 0x0018, 0x1234, 0x5678);
    TEST("FTRAPNGLE",              0xF240|074, 0x0018);

    TEST("FSNGL D2",              0xF240|002, 0x0019);
    TEST("FDBNGL D2, *+$1234",    0xF240|012, 0x0019, 0x1230);
    TEST("FSNGL (A2)",            0xF240|022, 0x0019);
    TEST("FSNGL (A2)+",           0xF240|032, 0x0019);
    TEST("FSNGL -(A2)",           0xF240|042, 0x0019);
    TEST("FSNGL ($1234,A2)",      0xF240|052, 0x0019, 0x1234);
    TEST("FSNGL (18,A2,D3.W)",    0xF240|062, 0x0019, 0x3012);
    TEST("FSNGL ($001234).W",     0xF240|070, 0x0019, 0x1234);
    TEST("FSNGL ($123456).L",     0xF240|071, 0x0019, 0x0012, 0x3456);
    TEST("FTRAPNGL.W #$1234",     0xF240|072, 0x0019, 0x1234);
    TEST("FTRAPNGL.L #$12345678", 0xF240|073, 0x0019, 0x1234, 0x5678);
    TEST("FTRAPNGL",              0xF240|074, 0x0019);

    TEST("FSNLE D2",              0xF240|002, 0x001A);
    TEST("FDBNLE D2, *+$1234",    0xF240|012, 0x001A, 0x1230);
    TEST("FSNLE (A2)",            0xF240|022, 0x001A);
    TEST("FSNLE (A2)+",           0xF240|032, 0x001A);
    TEST("FSNLE -(A2)",           0xF240|042, 0x001A);
    TEST("FSNLE ($1234,A2)",      0xF240|052, 0x001A, 0x1234);
    TEST("FSNLE (18,A2,D3.W)",    0xF240|062, 0x001A, 0x3012);
    TEST("FSNLE ($001234).W",     0xF240|070, 0x001A, 0x1234);
    TEST("FSNLE ($123456).L",     0xF240|071, 0x001A, 0x0012, 0x3456);
    TEST("FTRAPNLE.W #$1234",     0xF240|072, 0x001A, 0x1234);
    TEST("FTRAPNLE.L #$12345678", 0xF240|073, 0x001A, 0x1234, 0x5678);
    TEST("FTRAPNLE",              0xF240|074, 0x001A);

    TEST("FSNLT D2",              0xF240|002, 0x001B);
    TEST("FDBNLT D2, *+$1234",    0xF240|012, 0x001B, 0x1230);
    TEST("FSNLT (A2)",            0xF240|022, 0x001B);
    TEST("FSNLT (A2)+",           0xF240|032, 0x001B);
    TEST("FSNLT -(A2)",           0xF240|042, 0x001B);
    TEST("FSNLT ($1234,A2)",      0xF240|052, 0x001B, 0x1234);
    TEST("FSNLT (18,A2,D3.W)",    0xF240|062, 0x001B, 0x3012);
    TEST("FSNLT ($001234).W",     0xF240|070, 0x001B, 0x1234);
    TEST("FSNLT ($123456).L",     0xF240|071, 0x001B, 0x0012, 0x3456);
    TEST("FTRAPNLT.W #$1234",     0xF240|072, 0x001B, 0x1234);
    TEST("FTRAPNLT.L #$12345678", 0xF240|073, 0x001B, 0x1234, 0x5678);
    TEST("FTRAPNLT",              0xF240|074, 0x001B);

    TEST("FSNGE D2",              0xF240|002, 0x001C);
    TEST("FDBNGE D2, *+$1234",    0xF240|012, 0x001C, 0x1230);
    TEST("FSNGE (A2)",            0xF240|022, 0x001C);
    TEST("FSNGE (A2)+",           0xF240|032, 0x001C);
    TEST("FSNGE -(A2)",           0xF240|042, 0x001C);
    TEST("FSNGE ($1234,A2)",      0xF240|052, 0x001C, 0x1234);
    TEST("FSNGE (18,A2,D3.W)",    0xF240|062, 0x001C, 0x3012);
    TEST("FSNGE ($001234).W",     0xF240|070, 0x001C, 0x1234);
    TEST("FSNGE ($123456).L",     0xF240|071, 0x001C, 0x0012, 0x3456);
    TEST("FTRAPNGE.W #$1234",     0xF240|072, 0x001C, 0x1234);
    TEST("FTRAPNGE.L #$12345678", 0xF240|073, 0x001C, 0x1234, 0x5678);
    TEST("FTRAPNGE",              0xF240|074, 0x001C);

    TEST("FSNGT D2",              0xF240|002, 0x001D);
    TEST("FDBNGT D2, *+$1234",    0xF240|012, 0x001D, 0x1230);
    TEST("FSNGT (A2)",            0xF240|022, 0x001D);
    TEST("FSNGT (A2)+",           0xF240|032, 0x001D);
    TEST("FSNGT -(A2)",           0xF240|042, 0x001D);
    TEST("FSNGT ($1234,A2)",      0xF240|052, 0x001D, 0x1234);
    TEST("FSNGT (18,A2,D3.W)",    0xF240|062, 0x001D, 0x3012);
    TEST("FSNGT ($001234).W",     0xF240|070, 0x001D, 0x1234);
    TEST("FSNGT ($123456).L",     0xF240|071, 0x001D, 0x0012, 0x3456);
    TEST("FTRAPNGT.W #$1234",     0xF240|072, 0x001D, 0x1234);
    TEST("FTRAPNGT.L #$12345678", 0xF240|073, 0x001D, 0x1234, 0x5678);
    TEST("FTRAPNGT",              0xF240|074, 0x001D);

    TEST("FSSNE D2",              0xF240|002, 0x001E);
    TEST("FDBSNE D2, *+$1234",    0xF240|012, 0x001E, 0x1230);
    TEST("FSSNE (A2)",            0xF240|022, 0x001E);
    TEST("FSSNE (A2)+",           0xF240|032, 0x001E);
    TEST("FSSNE -(A2)",           0xF240|042, 0x001E);
    TEST("FSSNE ($1234,A2)",      0xF240|052, 0x001E, 0x1234);
    TEST("FSSNE (18,A2,D3.W)",    0xF240|062, 0x001E, 0x3012);
    TEST("FSSNE ($001234).W",     0xF240|070, 0x001E, 0x1234);
    TEST("FSSNE ($123456).L",     0xF240|071, 0x001E, 0x0012, 0x3456);
    TEST("FTRAPSNE.W #$1234",     0xF240|072, 0x001E, 0x1234);
    TEST("FTRAPSNE.L #$12345678", 0xF240|073, 0x001E, 0x1234, 0x5678);
    TEST("FTRAPSNE",              0xF240|074, 0x001E);

    TEST("FSST D2",              0xF240|002, 0x001F);
    TEST("FDBST D2, *+$1234",    0xF240|012, 0x001F, 0x1230);
    TEST("FSST (A2)",            0xF240|022, 0x001F);
    TEST("FSST (A2)+",           0xF240|032, 0x001F);
    TEST("FSST -(A2)",           0xF240|042, 0x001F);
    TEST("FSST ($1234,A2)",      0xF240|052, 0x001F, 0x1234);
    TEST("FSST (18,A2,D3.W)",    0xF240|062, 0x001F, 0x3012);
    TEST("FSST ($001234).W",     0xF240|070, 0x001F, 0x1234);
    TEST("FSST ($123456).L",     0xF240|071, 0x001F, 0x0012, 0x3456);
    TEST("FTRAPST.W #$1234",     0xF240|072, 0x001F, 0x1234);
    TEST("FTRAPST.L #$12345678", 0xF240|073, 0x001F, 0x1234, 0x5678);
    TEST("FTRAPST",              0xF240|074, 0x001F);
}

void test_float_system() {
    TEST("FPU ON");

    ERRT("FRESTORE D2",             OPERAND_NOT_ALLOWED, "D2", 0xF340);
    ERRT("FRESTORE A4",             OPERAND_NOT_ALLOWED, "A4", 0xF340);
    TEST("FRESTORE (A6)",           0xF340|026);
    TEST("FRESTORE (A0)+",          0xF340|030);
    ERRT("FRESTORE -(A2)",          OPERAND_NOT_ALLOWED, "-(A2)", 0xF340);
    TEST("FRESTORE ($1234,A4)",     0xF340|054, 0x1234);
    TEST("FRESTORE ($23,A6,D7.W)",  0xF340|066, 0x7023);
    TEST("FRESTORE ($004566).W",    0xF340|070, 0x4566);
    TEST("FRESTORE ($56789A).L",    0xF340|071, 0x0056, 0x789A);
    TEST("FRESTORE (*+$1234,PC)",   0xF340|072, 0x1232);
    TEST("FRESTORE (*+90,PC,A4.L)", 0xF340|073, 0xC858);
    ERRT("FRESTORE #$6789ABCD",     OPERAND_NOT_ALLOWED, "#$6789ABCD", 0xF340);

    ERRT("FSAVE D2",             OPERAND_NOT_ALLOWED, "D2", 0xF300);
    ERRT("FSAVE A4",             OPERAND_NOT_ALLOWED, "A4", 0xF300);
    TEST("FSAVE (A6)",           0xF300|026);
    ERRT("FSAVE (A0)+",          OPERAND_NOT_ALLOWED, "(A0)+", 0xF300);
    TEST("FSAVE -(A2)",          0xF300|042);
    TEST("FSAVE ($1234,A4)",     0xF300|054, 0x1234);
    TEST("FSAVE ($23,A6,D7.W)",  0xF300|066, 0x7023);
    TEST("FSAVE ($004566).W",    0xF300|070, 0x4566);
    TEST("FSAVE ($56789A).L",    0xF300|071, 0x0056, 0x789A);
    ERRT("FSAVE (*+$1234,PC)",   OPERAND_NOT_ALLOWED, "(*+$1234,PC)",   0xF300);
    ERRT("FSAVE (*+90,PC,A4.L)", OPERAND_NOT_ALLOWED, "(*+90,PC,A4.L)", 0xF300);
    ERRT("FSAVE #$6789ABCD",     OPERAND_NOT_ALLOWED, "#$6789ABCD",     0xF300);

    TEST("FADD.X FP0, FP1", 0171000, 0x0022|(0<<10)|(1<<7));
    as68000.setFpuCid(0);
    TEST("FADD.X FP0, FP1", 0171000, 0x0022|(0<<10)|(1<<7));
    as68000.setFpuCid(8);
    TEST("FADD.X FP0, FP1", 0171000, 0x0022|(0<<10)|(1<<7));
    as68000.setFpuCid(2);
    TEST("FADD.X FP0, FP1", 0172000, 0x0022|(0<<10)|(1<<7));
    as68000.setFpuCid(7);
    TEST("FADD.X FP0, FP1", 0177000, 0x0022|(0<<10)|(1<<7));
}

#endif

void test_extension() {
    if (firstGen()) {
        ERRT("LEA ($8000,A2),A1",      OVERFLOW_RANGE, "$8000,A2),A1",      0041752, 0x8000);
        ERRT("LEA (-$81,A2,D3.W),A1",  OVERFLOW_RANGE, "-$81,A2,D3.W),A1",  0041762, 0x307F);
        ERRT("LEA (*-$8000,PC),A1",    OVERFLOW_RANGE, "*-$8000,PC),A1",    0041772, 0x7FFE);
        ERRT("LEA (*+$82,PC,A3.L),A1", OVERFLOW_RANGE, "*+$82,PC,A3.L),A1", 0041773, 0xB880);
        TEST("LEA (18,A2,D3.W),A1",                                     0041762, 0x3012);
        ERRT("LEA (18,A2,D3.W*2),A1", ADDRESSING_NOT_ALLOWED, "*2),A1", 0041762, 0x3212);
        ERRT("LEA (18,A2,D3.W*4),A1", ADDRESSING_NOT_ALLOWED, "*4),A1", 0041762, 0x3412);
        ERRT("LEA (18,A2,D3.W*8),A1", ADDRESSING_NOT_ALLOWED, "*8),A1", 0041762, 0x3612);
        TEST("LEA (18,A2,D3.L),A1",                                     0041762, 0x3812);
        ERRT("LEA (18,A2,D3.L*2),A1", ADDRESSING_NOT_ALLOWED, "*2),A1", 0041762, 0x3A12);
        ERRT("LEA (18,A2,D3.L*4),A1", ADDRESSING_NOT_ALLOWED, "*4),A1", 0041762, 0x3C12);
        ERRT("LEA (18,A2,D3.L*8),A1", ADDRESSING_NOT_ALLOWED, "*8),A1", 0041762, 0x3E12);
        TEST("LEA (18,A2,A3.W),A1",                                     0041762, 0xB012);
        ERRT("LEA (18,A2,A3.W*2),A1", ADDRESSING_NOT_ALLOWED, "*2),A1", 0041762, 0xB212);
        ERRT("LEA (18,A2,A3.W*4),A1", ADDRESSING_NOT_ALLOWED, "*4),A1", 0041762, 0xB412);
        ERRT("LEA (18,A2,A3.W*8),A1", ADDRESSING_NOT_ALLOWED, "*8),A1", 0041762, 0xB612);
        TEST("LEA (18,A2,A3.L),A1",                                     0041762, 0xB812);
        ERRT("LEA (18,A2,A3.L*2),A1", ADDRESSING_NOT_ALLOWED, "*2),A1", 0041762, 0xBA12);
        ERRT("LEA (18,A2,A3.L*4),A1", ADDRESSING_NOT_ALLOWED, "*4),A1", 0041762, 0xBC12);
        ERRT("LEA (18,A2,A3.L*8),A1", ADDRESSING_NOT_ALLOWED, "*8),A1", 0041762, 0xBE12);
        ERRT("LEA (D3.W),A1",       ADDRESSING_NOT_ALLOWED, "D3.W),A1", 0041760, 0x3000);
        ERRT("LEA (A3.W),A1",       ADDRESSING_NOT_ALLOWED, "A3.W),A1", 0041760, 0xB000);
        ERRT("LEA ($1234,D3.W),A1", ADDRESSING_NOT_ALLOWED, "D3.W),A1", 0041760, 0x3034);
        ERRT("LEA ($1234,A3.W),A1", ADDRESSING_NOT_ALLOWED, "A3.W),A1", 0041760, 0xB034);
        TEST("LEA (A2,D3.W),A1",                                        0041762, 0x3000);
        TEST("LEA (A2,A3.L),A1",                                        0041762, 0xB800);
        ERRT("LEA ([D3.W]),A1",  ADDRESSING_NOT_ALLOWED, "[D3.W]),A1",  0041760, 0x3000);
        ERRT("LEA ([A3.W]),A1",  ADDRESSING_NOT_ALLOWED, "[A3.W]),A1",  0041760, 0xB000);
        ERRT("LEA ([D3.W],$5678),A1",
             ADDRESSING_NOT_ALLOWED,                "[D3.W],$5678),A1", 0041760, 0x3000);
        ERRT("LEA ([$1234,A3.W]),A1",
             ADDRESSING_NOT_ALLOWED,                "[$1234,A3.W]),A1", 0041760, 0xB034);
        ERRT("LEA ([$1234,D3.W],$5678),A1",
             ADDRESSING_NOT_ALLOWED,          "[$1234,D3.W],$5678),A1", 0041760, 0x3034);
        ERRT("LEA ([A2,D3.W]),A1",
             ADDRESSING_NOT_ALLOWED,                   "[A2,D3.W]),A1", 0041762, 0x3000);
        ERRT("LEA ([A2,A3.W],$5678),A1",
             ADDRESSING_NOT_ALLOWED,             "[A2,A3.W],$5678),A1", 0041762, 0xB000);
        ERRT("LEA ([$1234,A2,D3.W]),A1",
             ADDRESSING_NOT_ALLOWED,             "[$1234,A2,D3.W]),A1", 0041762, 0x3034);
        ERRT("LEA ([$1234,A2,A3.W],$5678),A1",
             ADDRESSING_NOT_ALLOWED,       "[$1234,A2,A3.W],$5678),A1", 0041762, 0xB034);
        ERRT("LEA ([*+$1234,PC,D3.W]),A1",
             ADDRESSING_NOT_ALLOWED,           "[*+$1234,PC,D3.W]),A1", 0041773, 0x3032);
        ERRT("LEA ([*+$1234,PC,A3.W],$5678),A1",
             ADDRESSING_NOT_ALLOWED,     "[*+$1234,PC,A3.W],$5678),A1", 0041773, 0xB032);
        ERRT("LEA ([$1234]),A1", ADDRESSING_NOT_ALLOWED, "[$1234]),A1", 0041760, 0x0034);
        ERRT("LEA ([$1234],$5678),A1",
             ADDRESSING_NOT_ALLOWED,               "[$1234],$5678),A1", 0041760, 0x0034);
        ERRT("LEA ([$1234],D3.W),A1",
             ADDRESSING_NOT_ALLOWED,                "[$1234],D3.W),A1", 0041760, 0x3034);
        ERRT("LEA ([$1234],A3.W,$5678),A1",
             ADDRESSING_NOT_ALLOWED,          "[$1234],A3.W,$5678),A1", 0041760, 0xB034);
        ERRT("LEA ([A2]),A1",       ADDRESSING_NOT_ALLOWED, "[A2]),A1", 0041762, 0x0000);
        ERRT("LEA ([A2],$5678),A1",
             ADDRESSING_NOT_ALLOWED,                  "[A2],$5678),A1", 0041762, 0x0000);
        ERRT("LEA ([A2],D3.W),A1",
             ADDRESSING_NOT_ALLOWED,                   "[A2],D3.W),A1", 0041762, 0x3000);
        ERRT("LEA ([A2],A3.W,$5678),A1",
             ADDRESSING_NOT_ALLOWED,             "[A2],A3.W,$5678),A1", 0041762, 0xB000);
        ERRT("LEA ([$1234,A2]),A1",
             ADDRESSING_NOT_ALLOWED,                  "[$1234,A2]),A1", 0041762, 0x0034);
        ERRT("LEA ([$1234,A2],$5678),A1",
             ADDRESSING_NOT_ALLOWED,            "[$1234,A2],$5678),A1", 0041762, 0x0034);
        ERRT("LEA ([$1234,A2],D3.W),A1",
             ADDRESSING_NOT_ALLOWED,             "[$1234,A2],D3.W),A1", 0041762, 0x3034);
        ERRT("LEA ([$1234,A2],A3.W,$5678),A1",
             ADDRESSING_NOT_ALLOWED,       "[$1234,A2],A3.W,$5678),A1", 0041762, 0xB034);
        ERRT("LEA ([*+$1234,PC]),A1",
             ADDRESSING_NOT_ALLOWED,                "[*+$1234,PC]),A1", 0041773, 0x0032);
        ERRT("LEA ([*+$1234,PC],$5678),A1",
             ADDRESSING_NOT_ALLOWED,          "[*+$1234,PC],$5678),A1", 0041773, 0x0032);
        ERRT("LEA ([*+$1234,PC],D3.W),A1",
             ADDRESSING_NOT_ALLOWED,           "[*+$1234,PC],D3.W),A1", 0041773, 0x3032);
        ERRT("LEA ([*+$1234,PC],A3.W,$5678),A1",
             ADDRESSING_NOT_ALLOWED,     "[*+$1234,PC],A3.W,$5678),A1", 0041773, 0xB032);
    } else {
        TEST("LEA (18,A2,D3.W*1),A1", 0041762, 0x3012);
        TEST("LEA (18,A2,D3.W*2),A1", 0041762, 0x3212);
        TEST("LEA (18,A2,D3.W*4),A1", 0041762, 0x3412);
        TEST("LEA (18,A2,D3.W*8),A1", 0041762, 0x3612);
        TEST("LEA (18,A2,D3.L*1),A1", 0041762, 0x3812);
        TEST("LEA (18,A2,D3.L*2),A1", 0041762, 0x3A12);
        TEST("LEA (18,A2,D3.L*4),A1", 0041762, 0x3C12);
        TEST("LEA (18,A2,D3.L*8),A1", 0041762, 0x3E12);
        TEST("LEA (18,A2,A3.W*1),A1", 0041762, 0xB012);
        TEST("LEA (18,A2,A3.W*2),A1", 0041762, 0xB212);
        TEST("LEA (18,A2,A3.W*4),A1", 0041762, 0xB412);
        TEST("LEA (18,A2,A3.W*8),A1", 0041762, 0xB612);
        TEST("LEA (18,A2,A3.L*1),A1", 0041762, 0xB812);
        TEST("LEA (18,A2,A3.L*2),A1", 0041762, 0xBA12);
        TEST("LEA (18,A2,A3.L*4),A1", 0041762, 0xBC12);
        TEST("LEA (18,A2,A3.L*8),A1", 0041762, 0xBE12);
        TEST("LEA (D3.W),A1",                    0041760, 0x3190);
        TEST("LEA (A3.W),A1",                    0041760, 0xB190);
        TEST("LEA ($1234,D3.W),A1",              0041760, 0x31A0, 0x1234);
        TEST("LEA ($1234,A3.W),A1",              0041760, 0xB1A0, 0x1234);
        TEST("LEA (A2,D3),A1",                   0041762, 0x3000);
        TEST("LEA (A2,A3),A1",                   0041762, 0xB000);
        TEST("LEA (A2,D3*2),A1",                 0041762, 0x3200);
        TEST("LEA (A2,A3*8),A1",                 0041762, 0xB600);
        TEST("LEA ($1234,A2,D3.W),A1",           0041762, 0x3120, 0x1234);
        TEST("LEA ($1234,A2,A3.W),A1",           0041762, 0xB120, 0x1234);
        TEST("LEA (*+2,PC),A1",                  0041772, 0x0000);
        TEST("LEA (*+$1234,PC),A1",              0041772, 0x1232);
        TEST("LEA (*+$1234,PC,D3.W),A1",         0041773, 0x3120, 0x1232);
        TEST("LEA (*+$1234,PC,A3.W),A1",         0041773, 0xB120, 0x1232);
        TEST("LEA ([D3.W]),A1",                  0041760, 0x3191);
        TEST("LEA ([A3.W]),A1",                  0041760, 0xB191);
        TEST("LEA ([D3.W],$5678),A1",            0041760, 0x3192, 0x5678);
        TEST("LEA ([A3.W],$5678),A1",            0041760, 0xB192, 0x5678);
        TEST("LEA ([$1234,D3.W]),A1",            0041760, 0x31A1, 0x1234);
        TEST("LEA ([$1234,A3.W]),A1",            0041760, 0xB1A1, 0x1234);
        TEST("LEA ([$1234,D3.W],$5678),A1",      0041760, 0x31A2, 0x1234, 0x5678);
        TEST("LEA ([$1234,A3.W],$5678),A1",      0041760, 0xB1A2, 0x1234, 0x5678);
        TEST("LEA ([A2,D3.W]),A1",               0041762, 0x3111);
        TEST("LEA ([A2,A3.W]),A1",               0041762, 0xB111);
        TEST("LEA ([A2,D3.W],$5678),A1",         0041762, 0x3112, 0x5678);
        TEST("LEA ([A2,A3.W],$5678),A1",         0041762, 0xB112, 0x5678);
        TEST("LEA ([$1234,A2,D3.W]),A1",         0041762, 0x3121, 0x1234);
        TEST("LEA ([$1234,A2,A3.W]),A1",         0041762, 0xB121, 0x1234);
        TEST("LEA ([$1234,A2,D3.W],$5678),A1",   0041762, 0x3122, 0x1234, 0x5678);
        TEST("LEA ([$1234,A2,A3.W],$5678),A1",   0041762, 0xB122, 0x1234, 0x5678);
        TEST("LEA ([*+2,PC,D3.W]),A1",           0041773, 0x3111);
        TEST("LEA ([*+2,PC,A3.W]),A1",           0041773, 0xB111);
        TEST("LEA ([*+2,PC,D3.W],$5678),A1",     0041773, 0x3112, 0x5678);
        TEST("LEA ([*+2,PC,A3.W],$5678),A1",     0041773, 0xB112, 0x5678);
        TEST("LEA ([*+$1234,PC,D3.W]),A1",       0041773, 0x3121, 0x1232);
        TEST("LEA ([*+$1234,PC,A3.W]),A1",       0041773, 0xB121, 0x1232);
        TEST("LEA ([*+$1234,PC,D3.W],$5678),A1", 0041773, 0x3122, 0x1232, 0x5678);
        TEST("LEA ([*+$1234,PC,A3.W],$5678),A1", 0041773, 0xB122, 0x1232, 0x5678);
        TEST("LEA ([$1234]),A1",                 0041760, 0x01E1, 0x1234);
        TEST("LEA ([$1234],$5678),A1",           0041760, 0x01E2, 0x1234, 0x5678);
        TEST("LEA ([$1234],D3.W),A1",            0041760, 0x31A5, 0x1234);
        TEST("LEA ([$1234],A3.W),A1",            0041760, 0xB1A5, 0x1234);
        TEST("LEA ([$1234],D3.W,$5678),A1",      0041760, 0x31A6, 0x1234, 0x5678);
        TEST("LEA ([$1234],A3.W,$5678),A1",      0041760, 0xB1A6, 0x1234, 0x5678);
        TEST("LEA ([A2]),A1",                    0041762, 0x0151);
        TEST("LEA ([A2],$5678),A1",              0041762, 0x0152, 0x5678);
        TEST("LEA ([A2],D3.W),A1",               0041762, 0x3115);
        TEST("LEA ([A2],A3.W),A1",               0041762, 0xB115);
        TEST("LEA ([A2],D3.W,$5678),A1",         0041762, 0x3116, 0x5678);
        TEST("LEA ([A2],A3.W,$5678),A1",         0041762, 0xB116, 0x5678);
        TEST("LEA ([$1234,A2]),A1",              0041762, 0x0161, 0x1234);
        TEST("LEA ([$1234,A2],$5678),A1",        0041762, 0x0162, 0x1234, 0x5678);
        TEST("LEA ([$1234,A2],D3.W),A1",         0041762, 0x3125, 0x1234);
        TEST("LEA ([$1234,A2],A3.W),A1",         0041762, 0xB125, 0x1234);
        TEST("LEA ([$1234,A2],D3.W,$5678),A1",   0041762, 0x3126, 0x1234, 0x5678);
        TEST("LEA ([$1234,A2],A3.W,$5678),A1",   0041762, 0xB126, 0x1234, 0x5678);
        TEST("LEA ([*+2,PC]),A1",                0041773, 0x0151);
        TEST("LEA ([*+2,PC],$5678),A1",          0041773, 0x0152, 0x5678);
        TEST("LEA ([*+2,PC],D3.W),A1",           0041773, 0x3115);
        TEST("LEA ([*+2,PC],A3.W),A1",           0041773, 0xB115);
        TEST("LEA ([*+2,PC],D3.W,$5678),A1",     0041773, 0x3116, 0x5678);
        TEST("LEA ([*+2,PC],A3.W,$5678),A1",     0041773, 0xB116, 0x5678);
        TEST("LEA ([*+$1234,PC]),A1",            0041773, 0x0161, 0x1232);
        TEST("LEA ([*+$1234,PC],$5678),A1",      0041773, 0x0162, 0x1232, 0x5678);
        TEST("LEA ([*+$1234,PC],D3.W),A1",       0041773, 0x3125, 0x1232);
        TEST("LEA ([*+$1234,PC],A3.W),A1",       0041773, 0xB125, 0x1232);
        TEST("LEA ([*+$1234,PC],D3.W,$5678),A1", 0041773, 0x3126, 0x1232, 0x5678);
        TEST("LEA ([*+$1234,PC],A3.W,$5678),A1", 0041773, 0xB126, 0x1232, 0x5678);
    }
}

#if !defined(LIBASM_MC68000_NOPMMU)
void test_pmove_mc68030() {
    // MC68EC030
    TEST("PMMU OFF");

    TEST("PMOVE.W (A2), ACUSR",          0xF000|022, 0x6000);
    TEST("PMOVE.L (A2), AC0",            0xF000|022, 0x0000|(1<<10));
    TEST("PMOVE.L (A2), AC1",            0xF000|022, 0x0000|(3<<10));
    TEST("PMOVE.W ($1234,A2), ACUSR",    0xF000|052, 0x6000, 0x1234);
    TEST("PMOVE.L ($1234,A2), AC0",      0xF000|052, 0x0000|(1<<10), 0x1234);
    TEST("PMOVE.L ($1234,A2), AC1",      0xF000|052, 0x0000|(3<<10), 0x1234);
    TEST("PMOVE.W ($34,A2,D3.W), ACUSR", 0xF000|062, 0x6000, 0x3034);
    TEST("PMOVE.L ($34,A2,D3.W), AC0",   0xF000|062, 0x0000|(1<<10), 0x3034);
    TEST("PMOVE.L ($34,A2,D3.W), AC1",   0xF000|062, 0x0000|(3<<10), 0x3034);
    TEST("PMOVE.W ($00001234).W, ACUSR", 0xF000|070, 0x6000, 0x1234);
    TEST("PMOVE.L ($00001234).W, AC0",   0xF000|070, 0x0000|(1<<10), 0x1234);
    TEST("PMOVE.L ($00001234).W, AC1",   0xF000|070, 0x0000|(3<<10), 0x1234);
    TEST("PMOVE.W ($12345678).L, ACUSR", 0xF000|071, 0x6000, 0x1234, 0x5678);
    TEST("PMOVE.L ($12345678).L, AC0",   0xF000|071, 0x0000|(1<<10), 0x1234, 0x5678);
    TEST("PMOVE.L ($12345678).L, AC1",   0xF000|071, 0x0000|(3<<10), 0x1234, 0x5678);

    TEST("PMOVE.W ACUSR, (A2)",          0xF000|022, 0x6200);
    TEST("PMOVE.L AC0, (A2)",            0xF000|022, 0x0200|(1<<10));
    TEST("PMOVE.L AC1, (A2)",            0xF000|022, 0x0200|(3<<10));
    TEST("PMOVE.W ACUSR, ($1234,A2)",    0xF000|052, 0x6200, 0x1234);
    TEST("PMOVE.L AC0, ($1234,A2)"  ,    0xF000|052, 0x0200|(1<<10), 0x1234);
    TEST("PMOVE.L AC1, ($1234,A2)",      0xF000|052, 0x0200|(3<<10), 0x1234);
    TEST("PMOVE.W ACUSR, ($34,A2,D3.W)", 0xF000|062, 0x6200, 0x3034);
    TEST("PMOVE.L AC0, ($34,A2,D3.W)",   0xF000|062, 0x0200|(1<<10), 0x3034);
    TEST("PMOVE.L AC1, ($34,A2,D3.W)",   0xF000|062, 0x0200|(3<<10), 0x3034);
    TEST("PMOVE.W ACUSR, ($00001234).W", 0xF000|070, 0x6200, 0x1234);
    TEST("PMOVE.L AC0, ($00001234).W",   0xF000|070, 0x0200|(1<<10), 0x1234);
    TEST("PMOVE.L AC1, ($00001234).W",   0xF000|070, 0x0200|(3<<10), 0x1234);
    TEST("PMOVE.W ACUSR, ($12345678).L", 0xF000|071, 0x6200, 0x1234, 0x5678);
    TEST("PMOVE.L AC0, ($12345678).L",   0xF000|071, 0x0200|(1<<10), 0x1234, 0x5678);
    TEST("PMOVE.L AC1, ($12345678).L",   0xF000|071, 0x0200|(3<<10), 0x1234, 0x5678);

    TEST("PTESTR #7, (A2)",           0xF000|022, 0x8210|7);
    TEST("PTESTR D3, ($1234,A2)",     0xF000|052, 0x8208|3, 0x1234);
    TEST("PTESTR SFC, ($34,A2,D3.W)", 0xF000|062, 0x8200|0, 0x3034);
    TEST("PTESTR DFC, ($00001234).W", 0xF000|070, 0x8200|1, 0x1234);
    TEST("PTESTR #7, ($12345678).L",  0xF000|071, 0x8210|7, 0x1234, 0x5678);

    TEST("PTESTW #7, (A2)",           0xF000|022, 0x8010|7);
    TEST("PTESTW D3, ($1234,A2)",     0xF000|052, 0x8008|3, 0x1234);
    TEST("PTESTW SFC, ($34,A2,D3.W)", 0xF000|062, 0x8000|0, 0x3034);
    TEST("PTESTW DFC, ($00001234).W", 0xF000|070, 0x8000|1, 0x1234);
    TEST("PTESTW #3, ($12345678).L",  0xF000|071, 0x8010|3, 0x1234, 0x5678);

    // MC68030
    TEST("PMMU ON");
    TEST("PMMU MC68030");

    ERRT("PMOVE.L D2, TC",
         OPERAND_NOT_ALLOWED, "D2, TC",               0xF000, 0x4000|(0<<10));
    ERRT("PMOVE.Q A2, SRP",
         OPERAND_NOT_ALLOWED, "A2, SRP",              0xF000, 0x4000|(2<<10));
    ERRT("PMOVE.Q (A2)+, CRP",
         OPERAND_NOT_ALLOWED, "(A2)+, CRP",           0xF000, 0x4000|(3<<10));
    ERRT("PMOVE.W -(A2), PSR",
         OPERAND_NOT_ALLOWED, "-(A2), PSR",           0xF000, 0x6000|(8<<10));
    ERRT("PMOVE.L (*+$1234,PC), TT0",
         OPERAND_NOT_ALLOWED, "(*+$1234,PC), TT0",    0xF000, 0x0000|(2<<10));
    ERRT("PMOVE.L (*+52,PC,D3.W), TT1",
         OPERAND_NOT_ALLOWED, "(*+52,PC,D3.W), TT1",  0xF000, 0x0000|(3<<10));
    ERRT("PMOVE.L #$12345678, TC",
         OPERAND_NOT_ALLOWED, "#$12345678, TC",       0xF000, 0x4000|(0<<10));

    TEST("PMOVE.L (A2), TC",   0xF000|022, 0x4000|(0<<10));
    TEST("PMOVE.Q (A2), SRP",  0xF000|022, 0x4000|(2<<10));
    TEST("PMOVE.Q (A2), CRP",  0xF000|022, 0x4000|(3<<10));
    TEST("PMOVE.W (A2), PSR",  0xF000|022, 0x6000|(8<<10));
    TEST("PMOVE.L (A2), TT0",  0xF000|022, 0x0000|(2<<10));
    TEST("PMOVE.L (A2), TT1",  0xF000|022, 0x0000|(3<<10));

    TEST("PMOVE.L ($1234,A2), TC",  0xF000|052, 0x4000|(0<<10), 0x1234);
    TEST("PMOVE.Q ($1234,A2), SRP", 0xF000|052, 0x4000|(2<<10), 0x1234);
    TEST("PMOVE.Q ($1234,A2), CRP", 0xF000|052, 0x4000|(3<<10), 0x1234);
    TEST("PMOVE.W ($1234,A2), PSR", 0xF000|052, 0x6000|(8<<10), 0x1234);
    TEST("PMOVE.L ($1234,A2), TT0", 0xF000|052, 0x0000|(2<<10), 0x1234);
    TEST("PMOVE.L ($1234,A2), TT1", 0xF000|052, 0x0000|(3<<10), 0x1234);

    TEST("PMOVE.L ($34,A2,D3.W), TC",  0xF000|062, 0x4000|(0<<10), 0x3034);
    TEST("PMOVE.Q ($34,A2,D3.W), SRP", 0xF000|062, 0x4000|(2<<10), 0x3034);
    TEST("PMOVE.Q ($34,A2,D3.W), CRP", 0xF000|062, 0x4000|(3<<10), 0x3034);
    TEST("PMOVE.W ($34,A2,D3.W), PSR", 0xF000|062, 0x6000|(8<<10), 0x3034);
    TEST("PMOVE.L ($34,A2,D3.W), TT0", 0xF000|062, 0x0000|(2<<10), 0x3034);
    TEST("PMOVE.L ($34,A2,D3.W), TT1", 0xF000|062, 0x0000|(3<<10), 0x3034);

    TEST("PMOVE.L ($00001234).W, TC",  0xF000|070, 0x4000|(0<<10), 0x1234);
    TEST("PMOVE.Q ($00001234).W, SRP", 0xF000|070, 0x4000|(2<<10), 0x1234);
    TEST("PMOVE.Q ($00001234).W, CRP", 0xF000|070, 0x4000|(3<<10), 0x1234);
    TEST("PMOVE.W ($00001234).W, PSR", 0xF000|070, 0x6000|(8<<10), 0x1234);
    TEST("PMOVE.L ($00001234).W, TT0", 0xF000|070, 0x0000|(2<<10), 0x1234);
    TEST("PMOVE.L ($00001234).W, TT1", 0xF000|070, 0x0000|(3<<10), 0x1234);

    TEST("PMOVE.L ($12345678).L, TC",  0xF000|071, 0x4000|(0<<10), 0x1234, 0x5678);
    TEST("PMOVE.Q ($12345678).L, SRP", 0xF000|071, 0x4000|(2<<10), 0x1234, 0x5678);
    TEST("PMOVE.Q ($12345678).L, CRP", 0xF000|071, 0x4000|(3<<10), 0x1234, 0x5678);
    TEST("PMOVE.W ($12345678).L, PSR", 0xF000|071, 0x6000|(8<<10), 0x1234, 0x5678);
    TEST("PMOVE.L ($12345678).L, TT0", 0xF000|071, 0x0000|(2<<10), 0x1234, 0x5678);
    TEST("PMOVE.L ($12345678).L, TT1", 0xF000|071, 0x0000|(3<<10), 0x1234, 0x5678);

    ERRT("PMOVEFD.L D2, TC",
         OPERAND_NOT_ALLOWED, "D2, TC",              0xF000, 0x4100|(0<<10));
    ERRT("PMOVEFD.Q A2, SRP",
         OPERAND_NOT_ALLOWED, "A2, SRP",             0xF000, 0x4100|(2<<10));
    ERRT("PMOVEFD.Q (A2)+, CRP",
         OPERAND_NOT_ALLOWED, "(A2)+, CRP",          0xF000, 0x4100|(3<<10));
    ERRT("PMOVEFD.W -(A2), PSR",
         OPERAND_NOT_ALLOWED, "-(A2), PSR",          0xF000, 0x6100|(8<<10));
    ERRT("PMOVEFD.L (*+$1234,PC), TT0",
         OPERAND_NOT_ALLOWED, "(*+$1234,PC), TT0",   0xF000, 0x0100|(2<<10));
    ERRT("PMOVEFD.L (*+52,PC,D3.W), TT1",
         OPERAND_NOT_ALLOWED, "(*+52,PC,D3.W), TT1", 0xF000, 0x0100|(3<<10));
    ERRT("PMOVEFD.L #$12345678, TC",
         OPERAND_NOT_ALLOWED, "#$12345678, TC",      0xF000, 0x4100|(0<<10));

    TEST("PMOVEFD.L (A2), TC",  0xF000|022, 0x4100|(0<<10));
    TEST("PMOVEFD.Q (A2), SRP", 0xF000|022, 0x4100|(2<<10));
    TEST("PMOVEFD.Q (A2), CRP", 0xF000|022, 0x4100|(3<<10));
    ERRT("PMOVEFD.W (A2), PSR", REGISTER_NOT_ALLOWED, "PSR", 0xF000|022, 0x6100|(8<<10));
    TEST("PMOVEFD.L (A2), TT0", 0xF000|022, 0x0100|(2<<10));
    TEST("PMOVEFD.L (A2), TT1", 0xF000|022, 0x0100|(3<<10));

    TEST("PMOVEFD.L ($1234,A2), TC",  0xF000|052, 0x4100|(0<<10), 0x1234);
    TEST("PMOVEFD.Q ($1234,A2), SRP", 0xF000|052, 0x4100|(2<<10), 0x1234);
    TEST("PMOVEFD.Q ($1234,A2), CRP", 0xF000|052, 0x4100|(3<<10), 0x1234);
    ERRT("PMOVEFD.W ($1234,A2), PSR",
         REGISTER_NOT_ALLOWED, "PSR", 0xF000|052, 0x6100|(8<<10), 0x1234);
    TEST("PMOVEFD.L ($1234,A2), TT0", 0xF000|052, 0x0100|(2<<10), 0x1234);
    TEST("PMOVEFD.L ($1234,A2), TT1", 0xF000|052, 0x0100|(3<<10), 0x1234);

    TEST("PMOVEFD.L ($34,A2,D3.W), TC",  0xF000|062, 0x4100|(0<<10), 0x3034);
    TEST("PMOVEFD.Q ($34,A2,D3.W), SRP", 0xF000|062, 0x4100|(2<<10), 0x3034);
    TEST("PMOVEFD.Q ($34,A2,D3.W), CRP", 0xF000|062, 0x4100|(3<<10), 0x3034);
    ERRT("PMOVEFD.W ($34,A2,D3.W), PSR",
         REGISTER_NOT_ALLOWED, "PSR",    0xF000|062, 0x6100|(8<<10), 0x3034);
    TEST("PMOVEFD.L ($34,A2,D3.W), TT0", 0xF000|062, 0x0100|(2<<10), 0x3034);
    TEST("PMOVEFD.L ($34,A2,D3.W), TT1", 0xF000|062, 0x0100|(3<<10), 0x3034);

    TEST("PMOVEFD.L ($00001234).W, TC",  0xF000|070, 0x4100|(0<<10), 0x1234);
    TEST("PMOVEFD.Q ($00001234).W, SRP", 0xF000|070, 0x4100|(2<<10), 0x1234);
    TEST("PMOVEFD.Q ($00001234).W, CRP", 0xF000|070, 0x4100|(3<<10), 0x1234);
    ERRT("PMOVEFD.W ($00001234).W, PSR",
         REGISTER_NOT_ALLOWED, "PSR",    0xF000|070, 0x6100|(8<<10), 0x1234);
    TEST("PMOVEFD.L ($00001234).W, TT0", 0xF000|070, 0x0100|(2<<10), 0x1234);
    TEST("PMOVEFD.L ($00001234).W, TT1", 0xF000|070, 0x0100|(3<<10), 0x1234);

    TEST("PMOVEFD.L ($12345678).L, TC",  0xF000|071, 0x4100|(0<<10), 0x1234, 0x5678);
    TEST("PMOVEFD.Q ($12345678).L, SRP", 0xF000|071, 0x4100|(2<<10), 0x1234, 0x5678);
    TEST("PMOVEFD.Q ($12345678).L, CRP", 0xF000|071, 0x4100|(3<<10), 0x1234, 0x5678);
    ERRT("PMOVEFD.W ($12345678).L, PSR",
         REGISTER_NOT_ALLOWED, "PSR",    0xF000|071, 0x6100|(8<<10), 0x1234, 0x5678);
    TEST("PMOVEFD.L ($12345678).L, TT0", 0xF000|071, 0x0100|(2<<10), 0x1234, 0x5678);
    TEST("PMOVEFD.L ($12345678).L, TT1", 0xF000|071, 0x0100|(3<<10), 0x1234, 0x5678);

    ERRT("PMOVE.L TC, D2",     OPERAND_NOT_ALLOWED, "D2", 0xF000, 0x4200|(0<<10));
    ERRT("PMOVE.Q SRP, A2",    OPERAND_NOT_ALLOWED, "A2", 0xF000, 0x4200|(2<<10));
    ERRT("PMOVE.Q CRP, (A2)+", OPERAND_NOT_ALLOWED, "(A2)+", 0xF000, 0x4200|(3<<10));
    ERRT("PMOVE.W PSR, -(A2)", OPERAND_NOT_ALLOWED, "-(A2)", 0xF000, 0x6200|(8<<10));
    ERRT("PMOVE.L TT0, (*+$1234,PC)",
         OPERAND_NOT_ALLOWED, "(*+$1234,PC)",   0xF000, 0x0200|(2<<10));
    ERRT("PMOVE.L TT1, (*+52,PC,D3.W)",
         OPERAND_NOT_ALLOWED, "(*+52,PC,D3.W)", 0xF000, 0x0200|(3<<10));

    TEST("PMOVE.L TC, (A2)",  0xF000|022, 0x4200|(0<<10));
    TEST("PMOVE.Q SRP, (A2)", 0xF000|022, 0x4200|(2<<10));
    TEST("PMOVE.Q CRP, (A2)", 0xF000|022, 0x4200|(3<<10));
    TEST("PMOVE.W PSR, (A2)", 0xF000|022, 0x6200|(8<<10));
    TEST("PMOVE.L TT0, (A2)", 0xF000|022, 0x0200|(2<<10));
    TEST("PMOVE.L TT1, (A2)", 0xF000|022, 0x0200|(3<<10));

    TEST("PMOVE.L TC, ($1234,A2)",  0xF000|052, 0x4200|(0<<10), 0x1234);
    TEST("PMOVE.Q SRP, ($1234,A2)", 0xF000|052, 0x4200|(2<<10), 0x1234);
    TEST("PMOVE.Q CRP, ($1234,A2)", 0xF000|052, 0x4200|(3<<10), 0x1234);
    TEST("PMOVE.W PSR, ($1234,A2)", 0xF000|052, 0x6200|(8<<10), 0x1234);
    TEST("PMOVE.L TT0, ($1234,A2)", 0xF000|052, 0x0200|(2<<10), 0x1234);
    TEST("PMOVE.L TT1, ($1234,A2)", 0xF000|052, 0x0200|(3<<10), 0x1234);

    TEST("PMOVE.L TC, ($34,A2,D3.W)",  0xF000|062, 0x4200|(0<<10), 0x3034);
    TEST("PMOVE.Q SRP, ($34,A2,D3.W)", 0xF000|062, 0x4200|(2<<10), 0x3034);
    TEST("PMOVE.Q CRP, ($34,A2,D3.W)", 0xF000|062, 0x4200|(3<<10), 0x3034);
    TEST("PMOVE.W PSR, ($34,A2,D3.W)", 0xF000|062, 0x6200|(8<<10), 0x3034);
    TEST("PMOVE.L TT0, ($34,A2,D3.W)", 0xF000|062, 0x0200|(2<<10), 0x3034);
    TEST("PMOVE.L TT1, ($34,A2,D3.W)", 0xF000|062, 0x0200|(3<<10), 0x3034);

    TEST("PMOVE.L TC, ($00001234).W",  0xF000|070, 0x4200|(0<<10), 0x1234);
    TEST("PMOVE.Q SRP, ($00001234).W", 0xF000|070, 0x4200|(2<<10), 0x1234);
    TEST("PMOVE.Q CRP, ($00001234).W", 0xF000|070, 0x4200|(3<<10), 0x1234);
    TEST("PMOVE.W PSR, ($00001234).W", 0xF000|070, 0x6200|(8<<10), 0x1234);
    TEST("PMOVE.L TT0, ($00001234).W", 0xF000|070, 0x0200|(2<<10), 0x1234);
    TEST("PMOVE.L TT1, ($00001234).W", 0xF000|070, 0x0200|(3<<10), 0x1234);

    TEST("PMOVE.L TC, ($12345678).L",  0xF000|071, 0x4200|(0<<10), 0x1234, 0x5678);
    TEST("PMOVE.Q SRP, ($12345678).L", 0xF000|071, 0x4200|(2<<10), 0x1234, 0x5678);
    TEST("PMOVE.Q CRP, ($12345678).L", 0xF000|071, 0x4200|(3<<10), 0x1234, 0x5678);
    TEST("PMOVE.W PSR, ($12345678).L", 0xF000|071, 0x6200|(8<<10), 0x1234, 0x5678);
    TEST("PMOVE.L TT0, ($12345678).L", 0xF000|071, 0x0200|(2<<10), 0x1234, 0x5678);
    TEST("PMOVE.L TT1, ($12345678).L", 0xF000|071, 0x0200|(3<<10), 0x1234, 0x5678);
}

void test_pmmu_mc68030() {
    TEST("PMMU ON");
    TEST("PMMU MC68030");

    TEST("PFLUSHA ",        0xF000, 0x2400);

    TEST("PFLUSH #7, #4",  0xF000, 0x3010|(4<<5)|7);
    ERRT("PFLUSH #8, #4",  OVERFLOW_RANGE, "#8, #4", 0xF000, 0x3010|(4<<5)|8);
    TEST("PFLUSH D4, #7",  0xF000, 0x3008|(7<<5)|4);
    ERRT("PFLUSH D4, #8",  OVERFLOW_RANGE, "#8",     0xF000, 0x3008|(8<<5)|4);
    TEST("PFLUSH SFC, #1", 0xF000, 0x3000|(1<<5)|0);
    TEST("PFLUSH DFC, #7", 0xF000, 0x3000|(7<<5)|1);

    ERRT("PFLUSH #3, #4, D2", OPERAND_NOT_ALLOWED, "D2", 0xF000, 0x3810|(4<<5)|3);
    ERRT("PFLUSH #3, #4, A2", OPERAND_NOT_ALLOWED, "A2", 0xF000, 0x3810|(4<<5)|3);
    TEST("PFLUSH #7, #4, (A2)",       0xF000|022, 0x3810|(4<<5)|7);
    ERRT("PFLUSH #15, #4, (A2)",
         OVERFLOW_RANGE, "#15, #4, (A2)", 0xF000|022, 0x3810|(4<<5)|15);
    TEST("PFLUSH D4, #7, ($1234,A2)",     0xF000|052, 0x3808|(7<<5)|4, 0x1234);
    TEST("PFLUSH SFC, #1, ($34,A2,D3.W)", 0xF000|062, 0x3800|(1<<5)|0, 0x3034);
    TEST("PFLUSH DFC, #1, ($00001234).W", 0xF000|070, 0x3800|(1<<5)|1, 0x1234);
    TEST("PFLUSH #7, #4, ($12345678).L",  0xF000|071, 0x3810|(4<<5)|7, 0x1234, 0x5678);
    ERRT("PFLUSH D4, #15, (*+$1234,PC)",
         OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0xF000, 0x3808|(15<<5)|4);
    ERRT("PFLUSH SFC, #1, (*+$34,PC,D3)",
         OPERAND_NOT_ALLOWED, "(*+$34,PC,D3)", 0xF000, 0x3800|(1<<5)|0);
    ERRT("PFLUSH #15, #4, #$12345678",
         OPERAND_NOT_ALLOWED, "#$12345678",   0xF000, 0x3810|(4<<5)|15);

    ERRT("PLOADR #7, D2", OPERAND_NOT_ALLOWED, "D2", 0xF000, 0x2210|7);
    ERRT("PLOADR #6, A2", OPERAND_NOT_ALLOWED, "A2", 0xF000, 0x2210|6);
    TEST("PLOADR #4, (A2)",           0xF000|022, 0x2210|4);
    ERRT("PLOADR #5, (A2)+", OPERAND_NOT_ALLOWED, "(A2)+", 0xF000, 0x2210|5);
    ERRT("PLOADR #4, -(A2)", OPERAND_NOT_ALLOWED, "-(A2)", 0xF000, 0x2210|4);
    TEST("PLOADR D3, ($1234,A2)",     0xF000|052, 0x2208|3, 0x1234);
    TEST("PLOADR SFC, ($34,A2,D3.W)", 0xF000|062, 0x2200|0, 0x3034);
    TEST("PLOADR DFC, ($00001234).W", 0xF000|070, 0x2200|1, 0x1234);
    TEST("PLOADR #4, ($12345678).L",  0xF000|071, 0x2210|4, 0x1234, 0x5678);
    ERRT("PLOADR D3, (*+$1234,PC)",   OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0xF000, 0x2208|3);
    ERRT("PLOADR SFC, (*+$34,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$34,PC,D3)", 0xF000, 0x2200|0);
    ERRT("PLOADR DFC, #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0xF000, 0x2200|1);

    ERRT("PLOADW #3, D2", OPERAND_NOT_ALLOWED, "D2", 0xF000, 0x2010|3);
    ERRT("PLOADW #2, A2", OPERAND_NOT_ALLOWED, "A2", 0xF000, 0x2010|2);
    TEST("PLOADW #7, (A2)",           0xF000|022, 0x2010|7);
    ERRT("PLOADW #1, (A2)+", OPERAND_NOT_ALLOWED, "(A2)+", 0xF000, 0x2010|1);
    ERRT("PLOADW #0, -(A2)", OPERAND_NOT_ALLOWED, "-(A2)", 0xF000, 0x2010|0);
    TEST("PLOADW D3, ($1234,A2)",     0xF000|052, 0x2008|3, 0x1234);
    TEST("PLOADW SFC, ($34,A2,D3.W)", 0xF000|062, 0x2000|0, 0x3034);
    TEST("PLOADW DFC, ($00001234).W", 0xF000|070, 0x2000|1, 0x1234);
    TEST("PLOADW #4, ($12345678).L",  0xF000|071, 0x2010|4, 0x1234, 0x5678);
    ERRT("PLOADW D3, (*+$1234,PC)",   OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0xF000, 0x2008|3);
    ERRT("PLOADW SFC, (*+$34,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$34,PC,D3)", 0xF000, 0x2000|0);
    ERRT("PLOADW DFC, #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0xF000, 0x2000|1);

    ERRT("PTESTR #7, D2, #4", OPERAND_NOT_ALLOWED, "D2, #4", 0xF000, 0x8210|(4<<10)|7);
    ERRT("PTESTR #7, A2, #4", OPERAND_NOT_ALLOWED, "A2, #4", 0xF000, 0x8210|(4<<10)|7);
    TEST("PTESTR #7, (A2), #4",           0xF000|022, 0x8210|(4<<10)|7);
    ERRT("PTESTR #7, (A2)+, #4", OPERAND_NOT_ALLOWED, "(A2)+, #4", 0xF000, 0x8210|(4<<10)|7);
    ERRT("PTESTR #7, -(A2), #4", OPERAND_NOT_ALLOWED, "-(A2), #4", 0xF000, 0x8210|(4<<10)|7);
    TEST("PTESTR D3, ($1234,A2), #7",     0xF000|052, 0x8208|(7<<10)|3, 0x1234);
    TEST("PTESTR SFC, ($34,A2,D3.W), #1", 0xF000|062, 0x8200|(1<<10)|0, 0x3034);
    TEST("PTESTR DFC, ($00001234).W, #5", 0xF000|070, 0x8200|(5<<10)|1, 0x1234);
    TEST("PTESTR #7, ($12345678).L, #2",  0xF000|071, 0x8210|(2<<10)|7, 0x1234, 0x5678);
    ERRT("PTESTR D3,  ($1234,PC), #7",
         OPERAND_NOT_ALLOWED, "($1234,PC), #7",  0xF000, 0x8208|(7<<10)|3);
    ERRT("PTESTR SFC, ($34,PC,D3), #1",
         OPERAND_NOT_ALLOWED, "($34,PC,D3), #1", 0xF000, 0x8200|(1<<10)|0);
    ERRT("PTESTR DFC, #$1234, #5",
         OPERAND_NOT_ALLOWED, "#$1234, #5",      0xF000, 0x8200|(5<<10)|1);

    TEST("PTESTR #7, (A2), #4, A3",           0xF000|022, 0x8310|(4<<10)|7|(3<<5));
    ERRT("PTESTR #7, (A2), #4, D3", OPERAND_NOT_ALLOWED, "#7, (A2), #4, D3");
    TEST("PTESTR D3, ($1234,A2), #7, A4",     0xF000|052, 0x8308|(7<<10)|3|(4<<5), 0x1234);
    TEST("PTESTR SFC, ($34,A2,D3.W), #1, A3", 0xF000|062, 0x8300|(1<<10)|0|(3<<5), 0x3034);
    TEST("PTESTR DFC, ($00001234).W, #5, A3", 0xF000|070, 0x8300|(5<<10)|1|(3<<5), 0x1234);
    TEST("PTESTR #6, ($12345678).L, #2, A3",  0xF000|071, 0x8310|(2<<10)|6|(3<<5),
         0x1234, 0x5678);

    TEST("PTESTW #7, (A2), #4",           0xF000|022, 0x8010|(4<<10)|7);
    TEST("PTESTW D3, ($1234,A2), #7",     0xF000|052, 0x8008|(7<<10)|3, 0x1234);
    TEST("PTESTW SFC, ($34,A2,D3.W), #1", 0xF000|062, 0x8000|(1<<10)|0, 0x3034);
    TEST("PTESTW DFC, ($00001234).W, #5", 0xF000|070, 0x8000|(5<<10)|1, 0x1234);
    TEST("PTESTW #1, ($12345678).L, #2",  0xF000|071, 0x8010|(2<<10)|1,
         0x1234, 0x5678);

    ERRT("PTESTW #7, D2, #4, A3",
         OPERAND_NOT_ALLOWED, "D2, #4, A3", 0xF000, 0x8110|(4<<10)|7|(3<<5));
    ERRT("PTESTW #7, A2, #4, A3",
         OPERAND_NOT_ALLOWED, "A2, #4, A3", 0xF000, 0x8110|(4<<10)|7|(3<<5));
    TEST("PTESTW #7, (A2), #4",           0xF000|022, 0x8010|(4<<10)|7);
    ERRT("PTESTW #7, D2, #4, A3",
         OPERAND_NOT_ALLOWED, "D2, #4, A3", 0xF000, 0x8110|(4<<10)|7|(3<<5));
    ERRT("PTESTW #7, D2, #4, A3",
         OPERAND_NOT_ALLOWED, "D2, #4, A3", 0xF000, 0x8110|(4<<10)|7|(3<<5));
    TEST("PTESTW D3, ($1234,A2), #7",     0xF000|052, 0x8008|(7<<10)|3, 0x1234);
    TEST("PTESTW SFC, ($34,A2,D3.W), #1", 0xF000|062, 0x8000|(1<<10)|0, 0x3034);
    TEST("PTESTW DFC, ($00001234).W, #5", 0xF000|070, 0x8000|(5<<10)|1, 0x1234);
    TEST("PTESTW #3, ($12345678).L, #2",  0xF000|071, 0x8010|(2<<10)|3, 0x1234, 0x5678);
    ERRT("PTESTW D3,  (*+$1234,PC), #7, A4",
         OPERAND_NOT_ALLOWED, "(*+$1234,PC), #7, A4",  0xF000, 0x8108|(7<<10)|3|(4<<5));
    ERRT("PTESTW SFC, (*+$34,PC,D3), #1, A3",
         OPERAND_NOT_ALLOWED, "(*+$34,PC,D3), #1, A3", 0xF000, 0x8100|(1<<10)|0|(3<<5));
    ERRT("PTESTW DFC, #$1234, #5, A3",
         OPERAND_NOT_ALLOWED, "#$1234, #5, A3",        0xF000, 0x8100|(5<<10)|1|(3<<5));
}

void test_pmmu_mc68040() {
    TEST("PMMU ON");
    TEST("PMMU MC68040");

    TEST("PFLUSHN (A2)", 0172402);
    TEST("PFLUSH  (A2)", 0172412);
    TEST("PFLUSHAN ",    0172420);
    TEST("PFLUSHA ",     0172430);

    TEST("PTESTR (A2)", 0172552);
    TEST("PTESTW (A2)", 0172512);
}

void test_pmove_mc68851() {
    if (mc68020()) {
        TEST("PMMU MC68851");
        TEST("PMMU ON");
    } else {
        ERRT("PMMU MC68851", UNKNOWN_OPERAND, "MC68851");
        return;
    }

    TEST("PMOVE D2, TC",   0xF000|002, 0x4000|(0<<10));
    ERRT("PMOVE D2, DRP", REGISTER_NOT_ALLOWED, "D2, DRP", 0xF000|002, 0x4000|(1<<10));
    ERRT("PMOVE D2, SRP", REGISTER_NOT_ALLOWED, "D2, SRP", 0xF000|002, 0x4000|(2<<10));
    ERRT("PMOVE D2, CRP", REGISTER_NOT_ALLOWED, "D2, CRP", 0xF000|002, 0x4000|(3<<10));
    TEST("PMOVE D2, CAL",  0xF000|002, 0x4000|(4<<10));
    TEST("PMOVE D2, VAL",  0xF000|002, 0x4000|(5<<10));
    TEST("PMOVE D2, SCC",  0xF000|002, 0x4000|(6<<10));
    TEST("PMOVE D2, AC",   0xF000|002, 0x4000|(7<<10));
    TEST("PMOVE D2, PSR",  0xF000|002, 0x4000|(8<<10));
    TEST("PMOVE D2, PCSR", 0xF000|002, 0x4000|(9<<10));
    TEST("PMOVE D2, BAD0", 0xF000|002, 0x4000|(12<<10)|(0<<2));
    TEST("PMOVE D2, BAD7", 0xF000|002, 0x4000|(12<<10)|(7<<2));
    TEST("PMOVE D2, BAC0", 0xF000|002, 0x4000|(13<<10)|(0<<2));
    TEST("PMOVE D2, BAC7", 0xF000|002, 0x4000|(13<<10)|(7<<2));

    TEST("PMOVE.L A2, TC",   0xF000|012, 0x4000|(0<<10));
    ERRT("PMOVE.Q A2, DRP", REGISTER_NOT_ALLOWED, "A2, DRP", 0xF000|012, 0x4000|(1<<10));
    ERRT("PMOVE.Q A2, SRP", REGISTER_NOT_ALLOWED, "A2, SRP", 0xF000|012, 0x4000|(2<<10));
    ERRT("PMOVE.Q A2, CRP", REGISTER_NOT_ALLOWED, "A2, CRP", 0xF000|012, 0x4000|(3<<10));
    TEST("PMOVE.B A2, CAL",  0xF000|012, 0x4000|(4<<10));
    TEST("PMOVE.B A2, VAL",  0xF000|012, 0x4000|(5<<10));
    TEST("PMOVE.B A2, SCC",  0xF000|012, 0x4000|(6<<10));
    TEST("PMOVE.W A2, AC",   0xF000|012, 0x4000|(7<<10));
    TEST("PMOVE.W A2, PSR",  0xF000|012, 0x4000|(8<<10));
    TEST("PMOVE.W A2, PCSR", 0xF000|012, 0x4000|(9<<10));
    TEST("PMOVE.W A2, BAD0", 0xF000|012, 0x4000|(12<<10)|(0<<2));
    TEST("PMOVE.W A2, BAC7", 0xF000|012, 0x4000|(13<<10)|(7<<2));

    TEST("PMOVE.L (A2), TC",   0xF000|022, 0x4000|(0<<10));
    TEST("PMOVE.Q (A2), DRP",  0xF000|022, 0x4000|(1<<10));
    TEST("PMOVE.Q (A2), SRP",  0xF000|022, 0x4000|(2<<10));
    TEST("PMOVE.Q (A2), CRP",  0xF000|022, 0x4000|(3<<10));
    TEST("PMOVE.B (A2), CAL",  0xF000|022, 0x4000|(4<<10));
    TEST("PMOVE.B (A2), VAL",  0xF000|022, 0x4000|(5<<10));
    TEST("PMOVE.B (A2), SCC",  0xF000|022, 0x4000|(6<<10));
    TEST("PMOVE.W (A2), AC",   0xF000|022, 0x4000|(7<<10));
    TEST("PMOVE.W (A2), PSR",  0xF000|022, 0x4000|(8<<10));
    TEST("PMOVE.W (A2), PCSR", 0xF000|022, 0x4000|(9<<10));
    TEST("PMOVE.W (A2), BAD0", 0xF000|022, 0x4000|(12<<10)|(0<<2));
    TEST("PMOVE.W (A2), BAC7", 0xF000|022, 0x4000|(13<<10)|(7<<2));

    TEST("PMOVE.L (A2)+, TC",   0xF000|032, 0x4000|(0<<10));
    TEST("PMOVE.Q (A2)+, DRP",  0xF000|032, 0x4000|(1<<10));
    TEST("PMOVE.Q (A2)+, SRP",  0xF000|032, 0x4000|(2<<10));
    TEST("PMOVE.Q (A2)+, CRP",  0xF000|032, 0x4000|(3<<10));
    TEST("PMOVE.B (A2)+, CAL",  0xF000|032, 0x4000|(4<<10));
    TEST("PMOVE.B (A2)+, VAL",  0xF000|032, 0x4000|(5<<10));
    TEST("PMOVE.B (A2)+, SCC",  0xF000|032, 0x4000|(6<<10));
    TEST("PMOVE.W (A2)+, AC",   0xF000|032, 0x4000|(7<<10));
    TEST("PMOVE.W (A2)+, PSR",  0xF000|032, 0x4000|(8<<10));
    TEST("PMOVE.W (A2)+, PCSR", 0xF000|032, 0x4000|(9<<10));
    TEST("PMOVE.W (A2)+, BAD0", 0xF000|032, 0x4000|(12<<10)|(0<<2));
    TEST("PMOVE.W (A2)+, BAC7", 0xF000|032, 0x4000|(13<<10)|(7<<2));

    TEST("PMOVE.L -(A2), TC",   0xF000|042, 0x4000|(0<<10));
    TEST("PMOVE.Q -(A2), DRP",  0xF000|042, 0x4000|(1<<10));
    TEST("PMOVE.Q -(A2), SRP",  0xF000|042, 0x4000|(2<<10));
    TEST("PMOVE.Q -(A2), CRP",  0xF000|042, 0x4000|(3<<10));
    TEST("PMOVE.B -(A2), CAL",  0xF000|042, 0x4000|(4<<10));
    TEST("PMOVE.B -(A2), VAL",  0xF000|042, 0x4000|(5<<10));
    TEST("PMOVE.B -(A2), SCC",  0xF000|042, 0x4000|(6<<10));
    TEST("PMOVE.W -(A2), AC",   0xF000|042, 0x4000|(7<<10));
    TEST("PMOVE.W -(A2), PSR",  0xF000|042, 0x4000|(8<<10));
    TEST("PMOVE.W -(A2), PCSR", 0xF000|042, 0x4000|(9<<10));
    TEST("PMOVE.W -(A2), BAD0", 0xF000|042, 0x4000|(12<<10)|(0<<2));
    TEST("PMOVE.W -(A2), BAC7", 0xF000|042, 0x4000|(13<<10)|(7<<2));

    TEST("PMOVE.L ($1234,A2), TC",   0xF000|052, 0x4000|(0<<10), 0x1234);
    TEST("PMOVE.Q ($1234,A2), DRP",  0xF000|052, 0x4000|(1<<10), 0x1234);
    TEST("PMOVE.Q ($1234,A2), SRP",  0xF000|052, 0x4000|(2<<10), 0x1234);
    TEST("PMOVE.Q ($1234,A2), CRP",  0xF000|052, 0x4000|(3<<10), 0x1234);
    TEST("PMOVE.B ($1234,A2), CAL",  0xF000|052, 0x4000|(4<<10), 0x1234);
    TEST("PMOVE.B ($1234,A2), VAL",  0xF000|052, 0x4000|(5<<10), 0x1234);
    TEST("PMOVE.B ($1234,A2), SCC",  0xF000|052, 0x4000|(6<<10), 0x1234);
    TEST("PMOVE.W ($1234,A2), AC",   0xF000|052, 0x4000|(7<<10), 0x1234);
    TEST("PMOVE.W ($1234,A2), PSR",  0xF000|052, 0x4000|(8<<10), 0x1234);
    TEST("PMOVE.W ($1234,A2), PCSR", 0xF000|052, 0x4000|(9<<10), 0x1234);
    TEST("PMOVE.W ($1234,A2), BAD0", 0xF000|052, 0x4000|(12<<10)|(0<<2), 0x1234);
    TEST("PMOVE.W ($1234,A2), BAC7", 0xF000|052, 0x4000|(13<<10)|(7<<2), 0x1234);

    TEST("PMOVE.L ($34,A2,D3.W), TC",   0xF000|062, 0x4000|(0<<10), 0x3034);
    TEST("PMOVE.Q ($34,A2,D3.W), DRP",  0xF000|062, 0x4000|(1<<10), 0x3034);
    TEST("PMOVE.Q ($34,A2,D3.W), SRP",  0xF000|062, 0x4000|(2<<10), 0x3034);
    TEST("PMOVE.Q ($34,A2,D3.W), CRP",  0xF000|062, 0x4000|(3<<10), 0x3034);
    TEST("PMOVE.B ($34,A2,D3.W), CAL",  0xF000|062, 0x4000|(4<<10), 0x3034);
    TEST("PMOVE.B ($34,A2,D3.W), VAL",  0xF000|062, 0x4000|(5<<10), 0x3034);
    TEST("PMOVE.B ($34,A2,D3.W), SCC",  0xF000|062, 0x4000|(6<<10), 0x3034);
    TEST("PMOVE.W ($34,A2,D3.W), AC",   0xF000|062, 0x4000|(7<<10), 0x3034);
    TEST("PMOVE.W ($34,A2,D3.W), PSR",  0xF000|062, 0x4000|(8<<10), 0x3034);
    TEST("PMOVE.W ($34,A2,D3.W), PCSR", 0xF000|062, 0x4000|(9<<10), 0x3034);
    TEST("PMOVE.W ($34,A2,D3.W), BAD0", 0xF000|062, 0x4000|(12<<10)|(0<<2), 0x3034);
    TEST("PMOVE.W ($34,A2,D3.W), BAC7", 0xF000|062, 0x4000|(13<<10)|(7<<2), 0x3034);

    TEST("PMOVE.L ($00001234).W, TC",   0xF000|070, 0x4000|(0<<10), 0x1234);
    TEST("PMOVE.Q ($00001234).W, DRP",  0xF000|070, 0x4000|(1<<10), 0x1234);
    TEST("PMOVE.Q ($00001234).W, SRP",  0xF000|070, 0x4000|(2<<10), 0x1234);
    TEST("PMOVE.Q ($00001234).W, CRP",  0xF000|070, 0x4000|(3<<10), 0x1234);
    TEST("PMOVE.B ($00001234).W, CAL",  0xF000|070, 0x4000|(4<<10), 0x1234);
    TEST("PMOVE.B ($00001234).W, VAL",  0xF000|070, 0x4000|(5<<10), 0x1234);
    TEST("PMOVE.B ($00001234).W, SCC",  0xF000|070, 0x4000|(6<<10), 0x1234);
    TEST("PMOVE.W ($00001234).W, AC",   0xF000|070, 0x4000|(7<<10), 0x1234);
    TEST("PMOVE.W ($00001234).W, PSR",  0xF000|070, 0x4000|(8<<10), 0x1234);
    TEST("PMOVE.W ($00001234).W, PCSR", 0xF000|070, 0x4000|(9<<10), 0x1234);
    TEST("PMOVE.W ($00001234).W, BAD0", 0xF000|070, 0x4000|(12<<10)|(0<<2), 0x1234);
    TEST("PMOVE.W ($00001234).W, BAC7", 0xF000|070, 0x4000|(13<<10)|(7<<2), 0x1234);

    TEST("PMOVE.L ($12345678).L, TC",   0xF000|071, 0x4000|(0<<10), 0x1234, 0x5678);
    TEST("PMOVE.Q ($12345678).L, DRP",  0xF000|071, 0x4000|(1<<10), 0x1234, 0x5678);
    TEST("PMOVE.Q ($12345678).L, SRP",  0xF000|071, 0x4000|(2<<10), 0x1234, 0x5678);
    TEST("PMOVE.Q ($12345678).L, CRP",  0xF000|071, 0x4000|(3<<10), 0x1234, 0x5678);
    TEST("PMOVE.B ($12345678).L, CAL",  0xF000|071, 0x4000|(4<<10), 0x1234, 0x5678);
    TEST("PMOVE.B ($12345678).L, VAL",  0xF000|071, 0x4000|(5<<10), 0x1234, 0x5678);
    TEST("PMOVE.B ($12345678).L, SCC",  0xF000|071, 0x4000|(6<<10), 0x1234, 0x5678);
    TEST("PMOVE.W ($12345678).L, AC",   0xF000|071, 0x4000|(7<<10), 0x1234, 0x5678);
    TEST("PMOVE.W ($12345678).L, PSR",  0xF000|071, 0x4000|(8<<10), 0x1234, 0x5678);
    TEST("PMOVE.W ($12345678).L, PCSR", 0xF000|071, 0x4000|(9<<10), 0x1234, 0x5678);
    TEST("PMOVE.W ($12345678).L, BAD0", 0xF000|071, 0x4000|(12<<10)|(0<<2), 0x1234, 0x5678);
    TEST("PMOVE.W ($12345678).L, BAC7", 0xF000|071, 0x4000|(13<<10)|(7<<2), 0x1234, 0x5678);

    TEST("PMOVE.L (*+$1234,PC), TC",   0xF000|072, 0x4000|(0<<10), 0x1230);
    TEST("PMOVE.Q (*+$1234,PC), DRP",  0xF000|072, 0x4000|(1<<10), 0x1230);
    TEST("PMOVE.Q (*+$1234,PC), SRP",  0xF000|072, 0x4000|(2<<10), 0x1230);
    TEST("PMOVE.Q (*+$1234,PC), CRP",  0xF000|072, 0x4000|(3<<10), 0x1230);
    TEST("PMOVE.B (*+$1234,PC), CAL",  0xF000|072, 0x4000|(4<<10), 0x1230);
    TEST("PMOVE.B (*+$1234,PC), VAL",  0xF000|072, 0x4000|(5<<10), 0x1230);
    TEST("PMOVE.B (*+$1234,PC), SCC",  0xF000|072, 0x4000|(6<<10), 0x1230);
    TEST("PMOVE.W (*+$1234,PC), AC",   0xF000|072, 0x4000|(7<<10), 0x1230);
    TEST("PMOVE.W (*+$1234,PC), PSR",  0xF000|072, 0x4000|(8<<10), 0x1230);
    TEST("PMOVE.W (*+$1234,PC), PCSR", 0xF000|072, 0x4000|(9<<10), 0x1230);
    TEST("PMOVE.W (*+$1234,PC), BAD0", 0xF000|072, 0x4000|(12<<10)|(0<<2), 0x1230);
    TEST("PMOVE.W (*+$1234,PC), BAC7", 0xF000|072, 0x4000|(13<<10)|(7<<2), 0x1230);

    TEST("PMOVE.L (*+52,PC,D3.W), TC",   0xF000|073, 0x4000|(0<<10), 0x3030);
    TEST("PMOVE.Q (*+52,PC,D3.W), DRP",  0xF000|073, 0x4000|(1<<10), 0x3030);
    TEST("PMOVE.Q (*+52,PC,D3.W), SRP",  0xF000|073, 0x4000|(2<<10), 0x3030);
    TEST("PMOVE.Q (*+52,PC,D3.W), CRP",  0xF000|073, 0x4000|(3<<10), 0x3030);
    TEST("PMOVE.B (*+52,PC,D3.W), CAL",  0xF000|073, 0x4000|(4<<10), 0x3030);
    TEST("PMOVE.B (*+52,PC,D3.W), VAL",  0xF000|073, 0x4000|(5<<10), 0x3030);
    TEST("PMOVE.B (*+52,PC,D3.W), SCC",  0xF000|073, 0x4000|(6<<10), 0x3030);
    TEST("PMOVE.W (*+52,PC,D3.W), AC",   0xF000|073, 0x4000|(7<<10), 0x3030);
    TEST("PMOVE.W (*+52,PC,D3.W), PSR",  0xF000|073, 0x4000|(8<<10), 0x3030);
    TEST("PMOVE.W (*+52,PC,D3.W), PCSR", 0xF000|073, 0x4000|(9<<10), 0x3030);
    TEST("PMOVE.W (*+52,PC,D3.W), BAD0", 0xF000|073, 0x4000|(12<<10)|(0<<2), 0x3030);
    TEST("PMOVE.W (*+52,PC,D3.W), BAC7", 0xF000|073, 0x4000|(13<<10)|(7<<2), 0x3030);

    TEST("PMOVE.L #$12345678, TC",          0xF000|074, 0x4000|(0<<10), 0x1234, 0x5678);
    TEST("PMOVE.Q #$123456789ABCDEF0, DRP", 0xF000|074, 0x4000|(1<<10),
         0x1234, 0x5678, 0x9ABC, 0xDEF0);
    TEST("PMOVE.Q #$123456789ABCDEF0, SRP", 0xF000|074, 0x4000|(2<<10),
         0x1234, 0x5678, 0x9ABC, 0xDEF0);
    TEST("PMOVE.Q #$123456789ABCDEF0, CRP", 0xF000|074, 0x4000|(3<<10),
         0x1234, 0x5678, 0x9ABC, 0xDEF0);
    TEST("PMOVE.B #$34, CAL",    0xF000|074, 0x4000|(4<<10), 0x0034);
    TEST("PMOVE.B #$34, VAL",    0xF000|074, 0x4000|(5<<10), 0x0034);
    TEST("PMOVE.B #$34, SCC",    0xF000|074, 0x4000|(6<<10), 0x0034);
    TEST("PMOVE.W #$1234, AC",   0xF000|074, 0x4000|(7<<10), 0x1234);
    TEST("PMOVE.W #$1234, PSR",  0xF000|074, 0x4000|(8<<10), 0x1234);
    TEST("PMOVE.W #$1234, PCSR", 0xF000|074, 0x4000|(9<<10), 0x1234);
    TEST("PMOVE.W #$1234, BAD0", 0xF000|074, 0x4000|(12<<10)|(0<<2), 0x1234);
    TEST("PMOVE.W #$1234, BAC7", 0xF000|074, 0x4000|(13<<10)|(7<<2), 0x1234);

    TEST("PMOVE.L TC, D2",   0xF000|002, 0x4200|(0<<10));
    ERRT("PMOVE.Q DRP, D2", REGISTER_NOT_ALLOWED, "D2", 0xF000|002, 0x4200|(1<<10));
    ERRT("PMOVE.Q SRP, D2", REGISTER_NOT_ALLOWED, "D2", 0xF000|002, 0x4200|(2<<10));
    ERRT("PMOVE.Q CRP, D2", REGISTER_NOT_ALLOWED, "D2", 0xF000|002, 0x4200|(3<<10));
    TEST("PMOVE.B CAL, D2",  0xF000|002, 0x4200|(4<<10));
    TEST("PMOVE.B VAL, D2",  0xF000|002, 0x4200|(5<<10));
    TEST("PMOVE.B SCC, D2",  0xF000|002, 0x4200|(6<<10));
    TEST("PMOVE.W AC, D2",   0xF000|002, 0x4200|(7<<10));
    TEST("PMOVE.W PSR, D2",  0xF000|002, 0x4200|(8<<10));
    TEST("PMOVE.W PCSR, D2", 0xF000|002, 0x4200|(9<<10));
    TEST("PMOVE.W BAD0, D2", 0xF000|002, 0x4200|(12<<10)|(0<<2));
    TEST("PMOVE.W BAD7, D2", 0xF000|002, 0x4200|(12<<10)|(7<<2));
    TEST("PMOVE.W BAC0, D2", 0xF000|002, 0x4200|(13<<10)|(0<<2));
    TEST("PMOVE.W BAC7, D2", 0xF000|002, 0x4200|(13<<10)|(7<<2));

    TEST("PMOVE TC, A2",   0xF000|012, 0x4200|(0<<10));
    ERRT("PMOVE DRP, A2", REGISTER_NOT_ALLOWED, "A2", 0xF000|012, 0x4200|(1<<10));
    ERRT("PMOVE SRP, A2", REGISTER_NOT_ALLOWED, "A2", 0xF000|012, 0x4200|(2<<10));
    ERRT("PMOVE CRP, A2", REGISTER_NOT_ALLOWED, "A2", 0xF000|012, 0x4200|(3<<10));
    TEST("PMOVE CAL, A2",  0xF000|012, 0x4200|(4<<10));
    TEST("PMOVE VAL, A2",  0xF000|012, 0x4200|(5<<10));
    TEST("PMOVE SCC, A2",  0xF000|012, 0x4200|(6<<10));
    TEST("PMOVE AC, A2",   0xF000|012, 0x4200|(7<<10));
    TEST("PMOVE PSR, A2",  0xF000|012, 0x4200|(8<<10));
    TEST("PMOVE PCSR, A2", 0xF000|012, 0x4200|(9<<10));
    TEST("PMOVE BAD0, A2", 0xF000|012, 0x4200|(12<<10)|(0<<2));
    TEST("PMOVE BAC7, A2", 0xF000|012, 0x4200|(13<<10)|(7<<2));

    TEST("PMOVE.L TC, (A2)",   0xF000|022, 0x4200|(0<<10));
    TEST("PMOVE.Q DRP, (A2)",  0xF000|022, 0x4200|(1<<10));
    TEST("PMOVE.Q SRP, (A2)",  0xF000|022, 0x4200|(2<<10));
    TEST("PMOVE.Q CRP, (A2)",  0xF000|022, 0x4200|(3<<10));
    TEST("PMOVE.B CAL, (A2)",  0xF000|022, 0x4200|(4<<10));
    TEST("PMOVE.B VAL, (A2)",  0xF000|022, 0x4200|(5<<10));
    TEST("PMOVE.B SCC, (A2)",  0xF000|022, 0x4200|(6<<10));
    TEST("PMOVE.W AC, (A2)",   0xF000|022, 0x4200|(7<<10));
    TEST("PMOVE.W PSR, (A2)",  0xF000|022, 0x4200|(8<<10));
    TEST("PMOVE.W PCSR, (A2)", 0xF000|022, 0x4200|(9<<10));
    TEST("PMOVE.W BAD0, (A2)", 0xF000|022, 0x4200|(12<<10)|(0<<2));
    TEST("PMOVE.W BAC7, (A2)", 0xF000|022, 0x4200|(13<<10)|(7<<2));

    TEST("PMOVE.L TC, (A2)+",   0xF000|032, 0x4200|(0<<10));
    TEST("PMOVE.Q DRP, (A2)+",  0xF000|032, 0x4200|(1<<10));
    TEST("PMOVE.Q SRP, (A2)+",  0xF000|032, 0x4200|(2<<10));
    TEST("PMOVE.Q CRP, (A2)+",  0xF000|032, 0x4200|(3<<10));
    TEST("PMOVE.B CAL, (A2)+",  0xF000|032, 0x4200|(4<<10));
    TEST("PMOVE.B VAL, (A2)+",  0xF000|032, 0x4200|(5<<10));
    TEST("PMOVE.B SCC, (A2)+",  0xF000|032, 0x4200|(6<<10));
    TEST("PMOVE.W AC, (A2)+",   0xF000|032, 0x4200|(7<<10));
    TEST("PMOVE.W PSR, (A2)+",  0xF000|032, 0x4200|(8<<10));
    TEST("PMOVE.W PCSR, (A2)+", 0xF000|032, 0x4200|(9<<10));
    TEST("PMOVE.W BAD0, (A2)+", 0xF000|032, 0x4200|(12<<10)|(0<<2));
    TEST("PMOVE.W BAC7, (A2)+", 0xF000|032, 0x4200|(13<<10)|(7<<2));

    TEST("PMOVE.L TC, -(A2)",   0xF000|042, 0x4200|(0<<10));
    TEST("PMOVE.Q DRP, -(A2)",  0xF000|042, 0x4200|(1<<10));
    TEST("PMOVE.Q SRP, -(A2)",  0xF000|042, 0x4200|(2<<10));
    TEST("PMOVE.Q CRP, -(A2)",  0xF000|042, 0x4200|(3<<10));
    TEST("PMOVE.B CAL, -(A2)",  0xF000|042, 0x4200|(4<<10));
    TEST("PMOVE.B VAL, -(A2)",  0xF000|042, 0x4200|(5<<10));
    TEST("PMOVE.B SCC, -(A2)",  0xF000|042, 0x4200|(6<<10));
    TEST("PMOVE.W AC, -(A2)",   0xF000|042, 0x4200|(7<<10));
    TEST("PMOVE.W PSR, -(A2)",  0xF000|042, 0x4200|(8<<10));
    TEST("PMOVE.W PCSR, -(A2)", 0xF000|042, 0x4200|(9<<10));
    TEST("PMOVE.W BAD0, -(A2)", 0xF000|042, 0x4200|(12<<10)|(0<<2));
    TEST("PMOVE.W BAC7, -(A2)", 0xF000|042, 0x4200|(13<<10)|(7<<2));

    TEST("PMOVE.L TC, ($1234,A2)",   0xF000|052, 0x4200|(0<<10), 0x1234);
    TEST("PMOVE.Q DRP, ($1234,A2)",  0xF000|052, 0x4200|(1<<10), 0x1234);
    TEST("PMOVE.Q SRP, ($1234,A2)",  0xF000|052, 0x4200|(2<<10), 0x1234);
    TEST("PMOVE.Q CRP, ($1234,A2)",  0xF000|052, 0x4200|(3<<10), 0x1234);
    TEST("PMOVE.B CAL, ($1234,A2)",  0xF000|052, 0x4200|(4<<10), 0x1234);
    TEST("PMOVE.B VAL, ($1234,A2)",  0xF000|052, 0x4200|(5<<10), 0x1234);
    TEST("PMOVE.B SCC, ($1234,A2)",  0xF000|052, 0x4200|(6<<10), 0x1234);
    TEST("PMOVE.W AC, ($1234,A2)",   0xF000|052, 0x4200|(7<<10), 0x1234);
    TEST("PMOVE.W PSR, ($1234,A2)",  0xF000|052, 0x4200|(8<<10), 0x1234);
    TEST("PMOVE.W PCSR, ($1234,A2)", 0xF000|052, 0x4200|(9<<10), 0x1234);
    TEST("PMOVE.W BAD0, ($1234,A2)", 0xF000|052, 0x4200|(12<<10)|(0<<2), 0x1234);
    TEST("PMOVE.W BAC7, ($1234,A2)", 0xF000|052, 0x4200|(13<<10)|(7<<2), 0x1234);

    TEST("PMOVE.L TC, ($34,A2,D3.W)",   0xF000|062, 0x4200|(0<<10), 0x3034);
    TEST("PMOVE.Q DRP, ($34,A2,D3.W)",  0xF000|062, 0x4200|(1<<10), 0x3034);
    TEST("PMOVE.Q SRP, ($34,A2,D3.W)",  0xF000|062, 0x4200|(2<<10), 0x3034);
    TEST("PMOVE.Q CRP, ($34,A2,D3.W)",  0xF000|062, 0x4200|(3<<10), 0x3034);
    TEST("PMOVE.B CAL, ($34,A2,D3.W)",  0xF000|062, 0x4200|(4<<10), 0x3034);
    TEST("PMOVE.B VAL, ($34,A2,D3.W)",  0xF000|062, 0x4200|(5<<10), 0x3034);
    TEST("PMOVE.B SCC, ($34,A2,D3.W)",  0xF000|062, 0x4200|(6<<10), 0x3034);
    TEST("PMOVE.W AC, ($34,A2,D3.W)",   0xF000|062, 0x4200|(7<<10), 0x3034);
    TEST("PMOVE.W PSR, ($34,A2,D3.W)",  0xF000|062, 0x4200|(8<<10), 0x3034);
    TEST("PMOVE.W PCSR, ($34,A2,D3.W)", 0xF000|062, 0x4200|(9<<10), 0x3034);
    TEST("PMOVE.W BAD0, ($34,A2,D3.W)", 0xF000|062, 0x4200|(12<<10)|(0<<2), 0x3034);
    TEST("PMOVE.W BAC7, ($34,A2,D3.W)", 0xF000|062, 0x4200|(13<<10)|(7<<2), 0x3034);

    TEST("PMOVE.L TC, ($00001234).W",   0xF000|070, 0x4200|(0<<10), 0x1234);
    TEST("PMOVE.Q DRP, ($00001234).W",  0xF000|070, 0x4200|(1<<10), 0x1234);
    TEST("PMOVE.Q SRP, ($00001234).W",  0xF000|070, 0x4200|(2<<10), 0x1234);
    TEST("PMOVE.Q CRP, ($00001234).W",  0xF000|070, 0x4200|(3<<10), 0x1234);
    TEST("PMOVE.B CAL, ($00001234).W",  0xF000|070, 0x4200|(4<<10), 0x1234);
    TEST("PMOVE.B VAL, ($00001234).W",  0xF000|070, 0x4200|(5<<10), 0x1234);
    TEST("PMOVE.B SCC, ($00001234).W",  0xF000|070, 0x4200|(6<<10), 0x1234);
    TEST("PMOVE.W AC, ($00001234).W",   0xF000|070, 0x4200|(7<<10), 0x1234);
    TEST("PMOVE.W PSR, ($00001234).W",  0xF000|070, 0x4200|(8<<10), 0x1234);
    TEST("PMOVE.W PCSR, ($00001234).W", 0xF000|070, 0x4200|(9<<10), 0x1234);
    TEST("PMOVE.W BAD0, ($00001234).W", 0xF000|070, 0x4200|(12<<10)|(0<<2), 0x1234);
    TEST("PMOVE.W BAC7, ($00001234).W", 0xF000|070, 0x4200|(13<<10)|(7<<2), 0x1234);

    TEST("PMOVE.L TC, ($12345678).L",   0xF000|071, 0x4200|(0<<10), 0x1234, 0x5678);
    TEST("PMOVE.Q DRP, ($12345678).L",  0xF000|071, 0x4200|(1<<10), 0x1234, 0x5678);
    TEST("PMOVE.Q SRP, ($12345678).L",  0xF000|071, 0x4200|(2<<10), 0x1234, 0x5678);
    TEST("PMOVE.Q CRP, ($12345678).L",  0xF000|071, 0x4200|(3<<10), 0x1234, 0x5678);
    TEST("PMOVE.B CAL, ($12345678).L",  0xF000|071, 0x4200|(4<<10), 0x1234, 0x5678);
    TEST("PMOVE.B VAL, ($12345678).L",  0xF000|071, 0x4200|(5<<10), 0x1234, 0x5678);
    TEST("PMOVE.B SCC, ($12345678).L",  0xF000|071, 0x4200|(6<<10), 0x1234, 0x5678);
    TEST("PMOVE.W AC, ($12345678).L",   0xF000|071, 0x4200|(7<<10), 0x1234, 0x5678);
    TEST("PMOVE.W PSR, ($12345678).L",  0xF000|071, 0x4200|(8<<10), 0x1234, 0x5678);
    TEST("PMOVE.W PCSR, ($12345678).L", 0xF000|071, 0x4200|(9<<10), 0x1234, 0x5678);
    TEST("PMOVE.W BAD0, ($12345678).L", 0xF000|071, 0x4200|(12<<10)|(0<<2), 0x1234, 0x5678);
    TEST("PMOVE.W BAC7, ($12345678).L", 0xF000|071, 0x4200|(13<<10)|(7<<2), 0x1234, 0x5678);

    ERRT("PMOVE.L TC, (*+$1234,PC)",
         OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0xF000, 0x4200|(0<<10)); // (d16,PC)
    ERRT("PMOVE.Q DRP, (*+$34,PC,D3)",
         OPERAND_NOT_ALLOWED, "(*+$34,PC,D3)", 0xF000, 0x4200|(1<<10)); // (d8,PC,Xn)
    ERRT("PMOVE.B CAL, #$34",
         OPERAND_NOT_ALLOWED, "#$34",          0xF000, 0x4200|(4<<10)); // #n
}

void test_branch_mc68851() {
    if (mc68020()) {
        TEST("PMMU MC68851");
    } else {
        ERRT("PMMU MC68851", UNKNOWN_OPERAND, "MC68851");
        return;
    }

    ATEST(0x10000, "PBBS *+$1234", 0xF080, 0x1232);
    ATEST(0x10000, "PBBC *+$1234", 0xF081, 0x1232);
    ATEST(0x10000, "PBLS *-$1234", 0xF082, 0xEDCA);
    ATEST(0x10000, "PBLC *+$1234", 0xF083, 0x1232);
    ATEST(0x10000, "PBSS *+$1234", 0xF084, 0x1232);
    ATEST(0x10000, "PBSC *+$1234", 0xF085, 0x1232);
    ATEST(0x10000, "PBAS *+$1234", 0xF086, 0x1232);
    ATEST(0x10000, "PBAC *+$1234", 0xF087, 0x1232);
    ATEST(0x10000, "PBWS *+$1234", 0xF088, 0x1232);
    ATEST(0x10000, "PBWC *+$1234", 0xF089, 0x1232);
    ATEST(0x10000, "PBIS *+$1234", 0xF08A, 0x1232);
    ATEST(0x10000, "PBIC *+$1234", 0xF08B, 0x1232);
    ATEST(0x10000, "PBGS *+$1234", 0xF08C, 0x1232);
    ATEST(0x10000, "PBGC *+$1234", 0xF08D, 0x1232);
    ATEST(0x10000, "PBCS *+$1234", 0xF08E, 0x1232);
    ATEST(0x10000, "PBCC *+$1234", 0xF08F, 0x1232);
    AERRT(0x01000, "PBCC *-$00002000", OVERFLOW_RANGE, "*-$00002000", 0xF08F, 0xDFFE);

    ATEST(0x200000, "PBBS  *+$00123456", 0xF0C0, 0x0012, 0x3454);
    ATEST(0x200000, "PBBC.X *+$00001234", 0xF0C1, 0x0000, 0x1232);
    ATEST(0x200000, "PBLS  *-$00123456", 0xF0C2, 0xFFED, 0xCBA8);
    ATEST(0x200000, "PBLC  *+$00123456", 0xF0C3, 0x0012, 0x3454);
    ATEST(0x200000, "PBSS  *+$00123456", 0xF0C4, 0x0012, 0x3454);
    ATEST(0x200000, "PBSC  *+$00123456", 0xF0C5, 0x0012, 0x3454);
    ATEST(0x200000, "PBAS  *+$00123456", 0xF0C6, 0x0012, 0x3454);
    ATEST(0x200000, "PBAC  *+$00123456", 0xF0C7, 0x0012, 0x3454);
    ATEST(0x200000, "PBWS  *+$00123456", 0xF0C8, 0x0012, 0x3454);
    ATEST(0x200000, "PBWC  *+$00123456", 0xF0C9, 0x0012, 0x3454);
    ATEST(0x200000, "PBIS  *+$00123456", 0xF0CA, 0x0012, 0x3454);
    ATEST(0x200000, "PBIC  *+$00123456", 0xF0CB, 0x0012, 0x3454);
    ATEST(0x200000, "PBGS  *+$00123456", 0xF0CC, 0x0012, 0x3454);
    ATEST(0x200000, "PBGC  *+$00123456", 0xF0CD, 0x0012, 0x3454);
    ATEST(0x200000, "PBCS  *+$00123456", 0xF0CE, 0x0012, 0x3454);
    ATEST(0x200000, "PBCC  *+$00123456", 0xF0CF, 0x0012, 0x3454);
    AERRT(0xF0000000, "PBCC *+$12345678", OVERFLOW_RANGE, "*+$12345678", 0xF0CF, 0x1234, 0x5676);

    TEST("PDBBS D2, *+$1234", 0xF048|012, 0x0000, 0x1230);
    TEST("PDBBC D2, *+$1234", 0xF048|012, 0x0001, 0x1230);
    TEST("PDBLS D2, *+$1234", 0xF048|012, 0x0002, 0x1230);
    TEST("PDBLC D2, *+$1234", 0xF048|012, 0x0003, 0x1230);
    TEST("PDBSS D2, *+$1234", 0xF048|012, 0x0004, 0x1230);
    TEST("PDBSC D2, *+$1234", 0xF048|012, 0x0005, 0x1230);
    TEST("PDBAS D2, *+$1234", 0xF048|012, 0x0006, 0x1230);
    TEST("PDBAC D2, *+$1234", 0xF048|012, 0x0007, 0x1230);
    TEST("PDBWS D2, *+$1234", 0xF048|012, 0x0008, 0x1230);
    TEST("PDBWC D2, *+$1234", 0xF048|012, 0x0009, 0x1230);
    TEST("PDBIS D2, *+$1234", 0xF048|012, 0x000A, 0x1230);
    TEST("PDBIC D2, *+$1234", 0xF048|012, 0x000B, 0x1230);
    TEST("PDBGS D2, *+$1234", 0xF048|012, 0x000C, 0x1230);
    TEST("PDBGC D2, *+$1234", 0xF048|012, 0x000D, 0x1230);
    TEST("PDBCS D2, *+$1234", 0xF048|012, 0x000E, 0x1230);
    TEST("PDBCC D2, *+$1234", 0xF048|012, 0x000F, 0x1230);
}

void test_trap_mc68851() {
    if (mc68020()) {
        TEST("PMMU MC68851");
    } else {
        ERRT("PMMU MC68851", UNKNOWN_OPERAND, "MC68851");
        return;
    }

    TEST("PTRAPBS.W #$1234", 0xF07A, 0x0000, 0x1234);
    TEST("PTRAPBC.W #$1234", 0xF07A, 0x0001, 0x1234);
    TEST("PTRAPLS.W #$1234", 0xF07A, 0x0002, 0x1234);
    TEST("PTRAPLC.W #$1234", 0xF07A, 0x0003, 0x1234);
    TEST("PTRAPSS.W #$1234", 0xF07A, 0x0004, 0x1234);
    TEST("PTRAPSC.W #$1234", 0xF07A, 0x0005, 0x1234);
    TEST("PTRAPAS.W #$1234", 0xF07A, 0x0006, 0x1234);
    TEST("PTRAPAC.W #$1234", 0xF07A, 0x0007, 0x1234);
    TEST("PTRAPWS.W #$1234", 0xF07A, 0x0008, 0x1234);
    TEST("PTRAPWC.W #$1234", 0xF07A, 0x0009, 0x1234);
    TEST("PTRAPIS.W #$1234", 0xF07A, 0x000A, 0x1234);
    TEST("PTRAPIC.W #$1234", 0xF07A, 0x000B, 0x1234);
    TEST("PTRAPGS.W #$1234", 0xF07A, 0x000C, 0x1234);
    TEST("PTRAPGC.W #$1234", 0xF07A, 0x000D, 0x1234);
    TEST("PTRAPCS.W #$1234", 0xF07A, 0x000E, 0x1234);
    TEST("PTRAPCC.W #$1234", 0xF07A, 0x000F, 0x1234);

    TEST("PTRAPBS.L #$12345678", 0xF07B, 0x0000, 0x1234, 0x5678);
    TEST("PTRAPBC.L #$12345678", 0xF07B, 0x0001, 0x1234, 0x5678);
    TEST("PTRAPLS.L #$12345678", 0xF07B, 0x0002, 0x1234, 0x5678);
    TEST("PTRAPLC.L #$12345678", 0xF07B, 0x0003, 0x1234, 0x5678);
    TEST("PTRAPSS.L #$12345678", 0xF07B, 0x0004, 0x1234, 0x5678);
    TEST("PTRAPSC.L #$12345678", 0xF07B, 0x0005, 0x1234, 0x5678);
    TEST("PTRAPAS.L #$12345678", 0xF07B, 0x0006, 0x1234, 0x5678);
    TEST("PTRAPAC.L #$12345678", 0xF07B, 0x0007, 0x1234, 0x5678);
    TEST("PTRAPWS.L #$12345678", 0xF07B, 0x0008, 0x1234, 0x5678);
    TEST("PTRAPWC.L #$12345678", 0xF07B, 0x0009, 0x1234, 0x5678);
    TEST("PTRAPIS.L #$12345678", 0xF07B, 0x000A, 0x1234, 0x5678);
    TEST("PTRAPIC.L #$12345678", 0xF07B, 0x000B, 0x1234, 0x5678);
    TEST("PTRAPGS.L #$12345678", 0xF07B, 0x000C, 0x1234, 0x5678);
    TEST("PTRAPGC.L #$12345678", 0xF07B, 0x000D, 0x1234, 0x5678);
    TEST("PTRAPCS.L #$12345678", 0xF07B, 0x000E, 0x1234, 0x5678);
    TEST("PTRAPCC.L #$12345678", 0xF07B, 0x000F, 0x1234, 0x5678);

    TEST("PTRAPBS ", 0xF07C, 0x0000);
    TEST("PTRAPBC ", 0xF07C, 0x0001);
    TEST("PTRAPLS ", 0xF07C, 0x0002);
    TEST("PTRAPLC ", 0xF07C, 0x0003);
    TEST("PTRAPSS ", 0xF07C, 0x0004);
    TEST("PTRAPSC ", 0xF07C, 0x0005);
    TEST("PTRAPAS ", 0xF07C, 0x0006);
    TEST("PTRAPAC ", 0xF07C, 0x0007);
    TEST("PTRAPWS ", 0xF07C, 0x0008);
    TEST("PTRAPWC ", 0xF07C, 0x0009);
    TEST("PTRAPIS ", 0xF07C, 0x000A);
    TEST("PTRAPIC ", 0xF07C, 0x000B);
    TEST("PTRAPGS ", 0xF07C, 0x000C);
    TEST("PTRAPGC ", 0xF07C, 0x000D);
    TEST("PTRAPCS ", 0xF07C, 0x000E);
    TEST("PTRAPCC ", 0xF07C, 0x000F);

    TEST("PSBS D2",              0xF040|002, 0x0000);
    TEST("PSBS (A2)",            0xF040|022, 0x0000);
    TEST("PSBS (A2)+",           0xF040|032, 0x0000);
    TEST("PSBS -(A2)",           0xF040|042, 0x0000);
    TEST("PSBS ($1234,A2)",      0xF040|052, 0x0000, 0x1234);
    TEST("PSBS (18,A2,D3.W)",    0xF040|062, 0x0000, 0x3012);
    TEST("PSBS ($00001234).W",   0xF040|070, 0x0000, 0x1234);
    TEST("PSBS ($12345678).L",   0xF040|071, 0x0000, 0x1234, 0x5678);

    TEST("PSBC D2",              0xF040|002, 0x0001);
    TEST("PSBC (A2)",            0xF040|022, 0x0001);
    TEST("PSBC (A2)+",           0xF040|032, 0x0001);
    TEST("PSBC -(A2)",           0xF040|042, 0x0001);
    TEST("PSBC ($1234,A2)",      0xF040|052, 0x0001, 0x1234);
    TEST("PSBC (18,A2,D3.W)",    0xF040|062, 0x0001, 0x3012);
    TEST("PSBC ($00001234).W",   0xF040|070, 0x0001, 0x1234);
    TEST("PSBC ($12345678).L",   0xF040|071, 0x0001, 0x1234, 0x5678);

    TEST("PSLS D2",              0xF040|002, 0x0002);
    TEST("PSLS (A2)",            0xF040|022, 0x0002);
    TEST("PSLS (A2)+",           0xF040|032, 0x0002);
    TEST("PSLS -(A2)",           0xF040|042, 0x0002);
    TEST("PSLS ($1234,A2)",      0xF040|052, 0x0002, 0x1234);
    TEST("PSLS (18,A2,D3.W)",    0xF040|062, 0x0002, 0x3012);
    TEST("PSLS ($00001234).W",   0xF040|070, 0x0002, 0x1234);
    TEST("PSLS ($12345678).L",   0xF040|071, 0x0002, 0x1234, 0x5678);

    TEST("PSLC D2",              0xF040|002, 0x0003);
    TEST("PSLC (A2)",            0xF040|022, 0x0003);
    TEST("PSLC (A2)+",           0xF040|032, 0x0003);
    TEST("PSLC -(A2)",           0xF040|042, 0x0003);
    TEST("PSLC ($1234,A2)",      0xF040|052, 0x0003, 0x1234);
    TEST("PSLC (18,A2,D3.W)",    0xF040|062, 0x0003, 0x3012);
    TEST("PSLC ($00001234).W",   0xF040|070, 0x0003, 0x1234);
    TEST("PSLC ($12345678).L",   0xF040|071, 0x0003, 0x1234, 0x5678);

    TEST("PSSS D2",              0xF040|002, 0x0004);
    TEST("PSSS (A2)",            0xF040|022, 0x0004);
    TEST("PSSS (A2)+",           0xF040|032, 0x0004);
    TEST("PSSS -(A2)",           0xF040|042, 0x0004);
    TEST("PSSS ($1234,A2)",      0xF040|052, 0x0004, 0x1234);
    TEST("PSSS (18,A2,D3.W)",    0xF040|062, 0x0004, 0x3012);
    TEST("PSSS ($00001234).W",   0xF040|070, 0x0004, 0x1234);
    TEST("PSSS ($12345678).L",   0xF040|071, 0x0004, 0x1234, 0x5678);

    TEST("PSSC D2",              0xF040|002, 0x0005);
    TEST("PSSC (A2)",            0xF040|022, 0x0005);
    TEST("PSSC (A2)+",           0xF040|032, 0x0005);
    TEST("PSSC -(A2)",           0xF040|042, 0x0005);
    TEST("PSSC ($1234,A2)",      0xF040|052, 0x0005, 0x1234);
    TEST("PSSC (18,A2,D3.W)",    0xF040|062, 0x0005, 0x3012);
    TEST("PSSC ($00001234).W",   0xF040|070, 0x0005, 0x1234);
    TEST("PSSC ($12345678).L",   0xF040|071, 0x0005, 0x1234, 0x5678);

    TEST("PSAS D2",              0xF040|002, 0x0006);
    TEST("PSAS (A2)",            0xF040|022, 0x0006);
    TEST("PSAS (A2)+",           0xF040|032, 0x0006);
    TEST("PSAS -(A2)",           0xF040|042, 0x0006);
    TEST("PSAS ($1234,A2)",      0xF040|052, 0x0006, 0x1234);
    TEST("PSAS (18,A2,D3.W)",    0xF040|062, 0x0006, 0x3012);
    TEST("PSAS ($00001234).W",   0xF040|070, 0x0006, 0x1234);
    TEST("PSAS ($12345678).L",   0xF040|071, 0x0006, 0x1234, 0x5678);

    TEST("PSAC D2",              0xF040|002, 0x0007);
    TEST("PSAC (A2)",            0xF040|022, 0x0007);
    TEST("PSAC (A2)+",           0xF040|032, 0x0007);
    TEST("PSAC -(A2)",           0xF040|042, 0x0007);
    TEST("PSAC ($1234,A2)",      0xF040|052, 0x0007, 0x1234);
    TEST("PSAC (18,A2,D3.W)",    0xF040|062, 0x0007, 0x3012);
    TEST("PSAC ($00001234).W",   0xF040|070, 0x0007, 0x1234);
    TEST("PSAC ($12345678).L",   0xF040|071, 0x0007, 0x1234, 0x5678);

    TEST("PSWS D2",              0xF040|002, 0x0008);
    TEST("PSWS (A2)",            0xF040|022, 0x0008);
    TEST("PSWS (A2)+",           0xF040|032, 0x0008);
    TEST("PSWS -(A2)",           0xF040|042, 0x0008);
    TEST("PSWS ($1234,A2)",      0xF040|052, 0x0008, 0x1234);
    TEST("PSWS (18,A2,D3.W)",    0xF040|062, 0x0008, 0x3012);
    TEST("PSWS ($00001234).W",   0xF040|070, 0x0008, 0x1234);
    TEST("PSWS ($12345678).L",   0xF040|071, 0x0008, 0x1234, 0x5678);

    TEST("PSWC D2",              0xF040|002, 0x0009);
    TEST("PSWC (A2)",            0xF040|022, 0x0009);
    TEST("PSWC (A2)+",           0xF040|032, 0x0009);
    TEST("PSWC -(A2)",           0xF040|042, 0x0009);
    TEST("PSWC ($1234,A2)",      0xF040|052, 0x0009, 0x1234);
    TEST("PSWC (18,A2,D3.W)",    0xF040|062, 0x0009, 0x3012);
    TEST("PSWC ($00001234).W",   0xF040|070, 0x0009, 0x1234);
    TEST("PSWC ($12345678).L",   0xF040|071, 0x0009, 0x1234, 0x5678);

    TEST("PSIS D2",              0xF040|002, 0x000A);
    TEST("PSIS (A2)",            0xF040|022, 0x000A);
    TEST("PSIS (A2)+",           0xF040|032, 0x000A);
    TEST("PSIS -(A2)",           0xF040|042, 0x000A);
    TEST("PSIS ($1234,A2)",      0xF040|052, 0x000A, 0x1234);
    TEST("PSIS (18,A2,D3.W)",    0xF040|062, 0x000A, 0x3012);
    TEST("PSIS ($00001234).W",   0xF040|070, 0x000A, 0x1234);
    TEST("PSIS ($12345678).L",   0xF040|071, 0x000A, 0x1234, 0x5678);

    TEST("PSIC D2",              0xF040|002, 0x000B);
    TEST("PSIC (A2)",            0xF040|022, 0x000B);
    TEST("PSIC (A2)+",           0xF040|032, 0x000B);
    TEST("PSIC -(A2)",           0xF040|042, 0x000B);
    TEST("PSIC ($1234,A2)",      0xF040|052, 0x000B, 0x1234);
    TEST("PSIC (18,A2,D3.W)",    0xF040|062, 0x000B, 0x3012);
    TEST("PSIC ($00001234).W",   0xF040|070, 0x000B, 0x1234);
    TEST("PSIC ($12345678).L",   0xF040|071, 0x000B, 0x1234, 0x5678);

    TEST("PSGS D2",              0xF040|002, 0x000C);
    TEST("PSGS (A2)",            0xF040|022, 0x000C);
    TEST("PSGS (A2)+",           0xF040|032, 0x000C);
    TEST("PSGS -(A2)",           0xF040|042, 0x000C);
    TEST("PSGS ($1234,A2)",      0xF040|052, 0x000C, 0x1234);
    TEST("PSGS (18,A2,D3.W)",    0xF040|062, 0x000C, 0x3012);
    TEST("PSGS ($00001234).W",   0xF040|070, 0x000C, 0x1234);
    TEST("PSGS ($12345678).L",   0xF040|071, 0x000C, 0x1234, 0x5678);

    TEST("PSGC D2",              0xF040|002, 0x000D);
    TEST("PSGC (A2)",            0xF040|022, 0x000D);
    TEST("PSGC (A2)+",           0xF040|032, 0x000D);
    TEST("PSGC -(A2)",           0xF040|042, 0x000D);
    TEST("PSGC ($1234,A2)",      0xF040|052, 0x000D, 0x1234);
    TEST("PSGC (18,A2,D3.W)",    0xF040|062, 0x000D, 0x3012);
    TEST("PSGC ($00001234).W",   0xF040|070, 0x000D, 0x1234);
    TEST("PSGC ($12345678).L",   0xF040|071, 0x000D, 0x1234, 0x5678);

    TEST("PSCS D2",              0xF040|002, 0x000E);
    TEST("PSCS (A2)",            0xF040|022, 0x000E);
    TEST("PSCS (A2)+",           0xF040|032, 0x000E);
    TEST("PSCS -(A2)",           0xF040|042, 0x000E);
    TEST("PSCS ($1234,A2)",      0xF040|052, 0x000E, 0x1234);
    TEST("PSCS (18,A2,D3.W)",    0xF040|062, 0x000E, 0x3012);
    TEST("PSCS ($00001234).W",   0xF040|070, 0x000E, 0x1234);
    TEST("PSCS ($12345678).L",   0xF040|071, 0x000E, 0x1234, 0x5678);

    TEST("PSCC D2",              0xF040|002, 0x000F);
    TEST("PSCC (A2)",            0xF040|022, 0x000F);
    TEST("PSCC (A2)+",           0xF040|032, 0x000F);
    TEST("PSCC -(A2)",           0xF040|042, 0x000F);
    TEST("PSCC ($1234,A2)",      0xF040|052, 0x000F, 0x1234);
    TEST("PSCC (18,A2,D3.W)",    0xF040|062, 0x000F, 0x3012);
    TEST("PSCC ($00001234).W",   0xF040|070, 0x000F, 0x1234);
    TEST("PSCC ($12345678).L",   0xF040|071, 0x000F, 0x1234, 0x5678);
}

void test_pmmu_mc68851() {
    if (mc68020()) {
        TEST("PMMU MC68851");
    } else {
        ERRT("PMMU MC68851", UNKNOWN_OPERAND, "MC68851");
        return;
    }

    TEST("PFLUSHA",        0xF000, 0x2400);

    TEST("PFLUSH #15, #4", 0xF000, 0x3010|(4<<5)|15);
    TEST("PFLUSH D4, #15", 0xF000, 0x3008|(15<<5)|4);
    TEST("PFLUSH SFC, #1", 0xF000, 0x3000|(1<<5)|0);
    TEST("PFLUSH DFC, #7", 0xF000, 0x3000|(7<<5)|1);

    TEST("PFLUSHS #15, #4", 0xF000, 0x3410|(4<<5)|15);
    TEST("PFLUSHS D4, #15", 0xF000, 0x3408|(15<<5)|4);
    TEST("PFLUSHS SFC, #2", 0xF000, 0x3400|(2<<5)|0);
    TEST("PFLUSHS DFC, #4", 0xF000, 0x3400|(4<<5)|1);

    ERRT("PFLUSH #3, #4, D2", OPERAND_NOT_ALLOWED, "D2", 0xF000, 0x3810|(4<<5)|3);
    ERRT("PFLUSH #3, #4, A2", OPERAND_NOT_ALLOWED, "A2", 0xF000, 0x3810|(4<<5)|3);
    TEST("PFLUSH #15, #4, (A2)",          0xF000|022, 0x3810|(4<<5)|15);
    ERRT("PFLUSH #15, #4, (A2)+", OPERAND_NOT_ALLOWED, "(A2)+", 0xF000, 0x3810|(4<<5)|15);
    ERRT("PFLUSH #15, #4, -(A2)", OPERAND_NOT_ALLOWED, "-(A2)", 0xF000, 0x3810|(4<<5)|15);
    TEST("PFLUSH D4, #15, ($1234,A2)",    0xF000|052, 0x3808|(15<<5)|4, 0x1234);
    TEST("PFLUSH SFC, #1, ($34,A2,D3.W)", 0xF000|062, 0x3800|(1<<5)|0, 0x3034);
    TEST("PFLUSH DFC, #1, ($00001234).W", 0xF000|070, 0x3800|(1<<5)|1, 0x1234);
    TEST("PFLUSH #15, #4, ($12345678).L", 0xF000|071, 0x3810|(4<<5)|15, 0x1234, 0x5678);
    ERRT("PFLUSH D4, #15, (*+$1234,PC)",
         OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0xF000, 0x3808|(15<<5)|4);
    ERRT("PFLUSH SFC, #1, (*+$34,PC,D3)",
         OPERAND_NOT_ALLOWED, "(*+$34,PC,D3)", 0xF000, 0x3800|(1<<5)|0);
    ERRT("PFLUSH #15, #4, #$12345678",
         OPERAND_NOT_ALLOWED, "#$12345678",   0xF000, 0x3810|(4<<5)|15);

    TEST("PFLUSHS #7, #4, (A2)",           0xF000|022, 0x3C10|(4<<5)|7);
    TEST("PFLUSHS D4, #15, ($1234,A2)",    0xF000|052, 0x3C08|(15<<5)|4, 0x1234);
    TEST("PFLUSHS SFC, #1, ($34,A2,D3.W)", 0xF000|062, 0x3C00|(1<<5)|0, 0x3034);
    TEST("PFLUSHS DFC, #1, ($00001234).W", 0xF000|070, 0x3C00|(1<<5)|1, 0x1234);
    TEST("PFLUSHS #4, #4, ($12345678).L",  0xF000|071, 0x3C10|(4<<5)|4, 0x1234, 0x5678);

    ERRT("PFLUSHR D2", OPERAND_NOT_ALLOWED, "D2", 0xF000, 0xA000);
    ERRT("PFLUSHR A2", OPERAND_NOT_ALLOWED, "A2", 0xF000, 0xA000);
    TEST("PFLUSHR (A2)",               0xF000|022, 0xA000);
    TEST("PFLUSHR (A2)+",              0xF000|032, 0xA000);
    TEST("PFLUSHR -(A2)",              0xF000|042, 0xA000);
    TEST("PFLUSHR ($1234,A2)",         0xF000|052, 0xA000, 0x1234);
    TEST("PFLUSHR ($34,A2,D3.W)",      0xF000|062, 0xA000, 0x3034);
    TEST("PFLUSHR ($00001234).W",      0xF000|070, 0xA000, 0x1234);
    TEST("PFLUSHR ($12345678).L",      0xF000|071, 0xA000, 0x1234, 0x5678);
    TEST("PFLUSHR (*+$1234,PC)",       0xF000|072, 0xA000, 0x1230);
    TEST("PFLUSHR (*+52,PC,D3.L)",     0xF000|073, 0xA000, 0x3830);
    TEST("PFLUSHR #$123456789ABCDEF0", 0xF000|074, 0xA000, 0x1234, 0x5678, 0x9ABC, 0xDEF0);

    ERRT("PLOADR #15, D2", OPERAND_NOT_ALLOWED, "D2", 0xF000, 0x2210|15);
    ERRT("PLOADR #15, A2", OPERAND_NOT_ALLOWED, "A2", 0xF000, 0x2210|15);
    TEST("PLOADR #15, (A2)",          0xF000|022, 0x2210|15);
    ERRT("PLOADR #15, (A2)+", OPERAND_NOT_ALLOWED, "(A2)+", 0xF000, 0x2210|15);
    ERRT("PLOADR #15, -(A2)", OPERAND_NOT_ALLOWED, "-(A2)", 0xF000, 0x2210|15);
    TEST("PLOADR D3, ($1234,A2)",     0xF000|052, 0x2208|3, 0x1234);
    TEST("PLOADR SFC, ($34,A2,D3.W)", 0xF000|062, 0x2200|0, 0x3034);
    TEST("PLOADR DFC, ($00001234).W", 0xF000|070, 0x2200|1, 0x1234);
    TEST("PLOADR #10, ($12345678).L", 0xF000|071, 0x2210|10, 0x1234, 0x5678);
    ERRT("PLOADR D3, (*+$1234,PC)",   OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0xF000, 0x2208|3);
    ERRT("PLOADR SFC, (*+$34,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$34,PC,D3)", 0xF000, 0x2200|0);
    ERRT("PLOADR DFC, #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0xF000, 0x2200|1);

    ERRT("PLOADW #15, D2", OPERAND_NOT_ALLOWED, "D2", 0xF000, 0x2010|15);
    ERRT("PLOADW #15, A2", OPERAND_NOT_ALLOWED, "A2", 0xF000, 0x2010|15);
    TEST("PLOADW #15, (A2)",          0xF000|022, 0x2010|15);
    ERRT("PLOADW #15, (A2)+", OPERAND_NOT_ALLOWED, "(A2)+", 0xF000, 0x2010|15);
    ERRT("PLOADW #15, -(A2)", OPERAND_NOT_ALLOWED, "-(A2)", 0xF000, 0x2010|15);
    TEST("PLOADW D3, ($1234,A2)",     0xF000|052, 0x2008|3, 0x1234);
    TEST("PLOADW SFC, ($34,A2,D3.W)", 0xF000|062, 0x2000|0, 0x3034);
    TEST("PLOADW DFC, ($00001234).W", 0xF000|070, 0x2000|1, 0x1234);
    TEST("PLOADW #10, ($12345678).L", 0xF000|071, 0x2010|10, 0x1234, 0x5678);
    ERRT("PLOADW D3, (*+$1234,PC)",   OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0xF000, 0x2008|3);
    ERRT("PLOADW SFC, (*+$34,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$34,PC,D3)", 0xF000, 0x2000|0);
    ERRT("PLOADW DFC, #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0xF000, 0x2000|1);

    ERRT("PRESTORE D2", OPERAND_NOT_ALLOWED, "D2", 0xF140);
    ERRT("PRESTORE A2", OPERAND_NOT_ALLOWED, "A2", 0xF140);
    TEST("PRESTORE (A2)",           0xF140|022);
    TEST("PRESTORE (A2)+",          0xF140|032);
    ERRT("PRESTORE -(A2)", OPERAND_NOT_ALLOWED, "-(A2)", 0xF140);
    TEST("PRESTORE ($1234,A2)",     0xF140|052, 0x1234);
    TEST("PRESTORE ($34,A2,D3.W)",  0xF140|062, 0x3034);
    TEST("PRESTORE ($00001234).W",  0xF140|070, 0x1234);
    TEST("PRESTORE ($12345678).L",  0xF140|071, 0x1234, 0x5678);
    TEST("PRESTORE (*+$1234,PC)",   0xF140|072, 0x1232);
    TEST("PRESTORE (*+52,PC,D3.W)", 0xF140|073, 0x3032);
    ERRT("PRESTORE #$1234", OPERAND_NOT_ALLOWED, "#$1234", 0xF140);

    ERRT("PSAVE D2", OPERAND_NOT_ALLOWED, "D2", 0xF100);
    ERRT("PSAVE A2", OPERAND_NOT_ALLOWED, "A2", 0xF100);
    TEST("PSAVE (A2)",           0xF100|022);
    ERRT("PSAVE (A2)+", OPERAND_NOT_ALLOWED, "(A2)+", 0xF100);
    TEST("PSAVE -(A2)",          0xF100|042);
    TEST("PSAVE ($1234,A2)",     0xF100|052, 0x1234);
    TEST("PSAVE ($34,A2,D3.W)",  0xF100|062, 0x3034);
    TEST("PSAVE ($00001234).W",  0xF100|070, 0x1234);
    TEST("PSAVE ($12345678).L",  0xF100|071, 0x1234, 0x5678);
    ERRT("PSAVE (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0xF100);
    ERRT("PSAVE (*+$34,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$34,PC,D3)", 0xF100);
    ERRT("PSAVE #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0xF100);

    ERRT("PTESTR #7, D2, #4", OPERAND_NOT_ALLOWED, "D2, #4", 0xF000, 0x8210|(4<<10)|7);
    ERRT("PTESTR #7, A2, #4", OPERAND_NOT_ALLOWED, "A2, #4", 0xF000, 0x8210|(4<<10)|7);
    TEST("PTESTR #7, (A2), #4",           0xF000|022, 0x8210|(4<<10)|7);
    ERRT("PTESTR #7, (A2)+, #4", OPERAND_NOT_ALLOWED, "(A2)+, #4", 0xF000, 0x8210|(4<<10)|7);
    ERRT("PTESTR #7, -(A2), #4", OPERAND_NOT_ALLOWED, "-(A2), #4", 0xF000, 0x8210|(4<<10)|7);
    TEST("PTESTR D3,  ($1234,A2), #7",    0xF000|052, 0x8208|(7<<10)|3, 0x1234);
    TEST("PTESTR SFC, ($34,A2,D3.W), #1", 0xF000|062, 0x8200|(1<<10)|0, 0x3034);
    TEST("PTESTR DFC, ($00001234).W, #5", 0xF000|070, 0x8200|(5<<10)|1, 0x1234);
    TEST("PTESTR #15, ($12345678).L, #2", 0xF000|071, 0x8210|(2<<10)|15, 0x1234, 0x5678);
    ERRT("PTESTR D3,  ($1234,PC), #7",
         OPERAND_NOT_ALLOWED, "($1234,PC), #7",  0xF000, 0x8208|(7<<10)|3);
    ERRT("PTESTR SFC, ($34,PC,D3), #1",
         OPERAND_NOT_ALLOWED, "($34,PC,D3), #1", 0xF000, 0x8200|(1<<10)|0);
    ERRT("PTESTR DFC, #$1234, #5",
         OPERAND_NOT_ALLOWED, "#$1234, #5",      0xF000, 0x8200|(5<<10)|1);

    TEST("PTESTR #7, (A2), #4, A3", 0xF000|022, 0x8310|(4<<10)|7|(3<<5));
    ERRT("PTESTR #7, (A2), #4, D3", OPERAND_NOT_ALLOWED, "#7, (A2), #4, D3");
    TEST("PTESTR D3, ($1234,A2), #7, A4",     0xF000|052, 0x8308|(7<<10)|3|(4<<5), 0x1234);
    TEST("PTESTR SFC, ($34,A2,D3.W), #1, A3", 0xF000|062, 0x8300|(1<<10)|0|(3<<5), 0x3034);
    TEST("PTESTR DFC, ($00001234).W, #5, A3", 0xF000|070, 0x8300|(5<<10)|1|(3<<5), 0x1234);
    TEST("PTESTR #9, ($12345678).L, #2, A3",  0xF000|071, 0x8310|(2<<10)|9|(3<<5),
         0x1234, 0x5678);

    TEST("PTESTW #7, (A2), #4",           0xF000|022, 0x8010|(4<<10)|7);
    TEST("PTESTW D3, ($1234,A2), #7",     0xF000|052, 0x8008|(7<<10)|3, 0x1234);
    TEST("PTESTW SFC, ($34,A2,D3.W), #1", 0xF000|062, 0x8000|(1<<10)|0, 0x3034);
    TEST("PTESTW DFC, ($00001234).W, #5", 0xF000|070, 0x8000|(5<<10)|1, 0x1234);
    TEST("PTESTW #15, ($12345678).L, #2", 0xF000|071, 0x8010|(2<<10)|15, 0x1234, 0x5678);

    ERRT("PTESTW #7, D2, #4, A3",
         OPERAND_NOT_ALLOWED, "D2, #4, A3", 0xF000, 0x8110|(4<<10)|7|(3<<5));
    ERRT("PTESTW #7, A2, #4, A3",
         OPERAND_NOT_ALLOWED, "A2, #4, A3", 0xF000, 0x8110|(4<<10)|7|(3<<5));
    TEST("PTESTW #7, (A2), #4, A3",         0xF000|022, 0x8110|(4<<10)|7|(3<<5));
    ERRT("PTESTW #7, D2, #4, A3",
         OPERAND_NOT_ALLOWED, "D2, #4, A3", 0xF000, 0x8110|(4<<10)|7|(3<<5));
    ERRT("PTESTW #7, D2, #4, A3",
         OPERAND_NOT_ALLOWED, "D2, #4, A3", 0xF000, 0x8110|(4<<10)|7|(3<<5));
    TEST("PTESTW D3, ($1234,A2), #7, A4",   0xF000|052, 0x8108|(7<<10)|3|(4<<5), 0x1234);
    TEST("PTESTW SFC, ($34,A2,D3.W), #1, A3", 0xF000|062, 0x8100|(1<<10)|0|(3<<5), 0x3034);
    TEST("PTESTW DFC, ($00001234).W, #5, A3", 0xF000|070, 0x8100|(5<<10)|1|(3<<5), 0x1234);
    TEST("PTESTW #9, ($12345678).L, #2, A3",  0xF000|071, 0x8110|(2<<10)|9|(3<<5),
         0x1234, 0x5678);
    ERRT("PTESTW D3,  (*+$1234,PC), #7, A4",
         OPERAND_NOT_ALLOWED, "(*+$1234,PC), #7, A4",  0xF000, 0x8108|(7<<10)|3|(4<<5));
    ERRT("PTESTW SFC, (*+$34,PC,D3), #1, A3",
         OPERAND_NOT_ALLOWED, "(*+$34,PC,D3), #1, A3", 0xF000, 0x8100|(1<<10)|0|(3<<5));
    ERRT("PTESTW DFC, #$1234, #5, A3",
         OPERAND_NOT_ALLOWED, "#$1234, #5, A3",        0xF000, 0x8100|(5<<10)|1|(3<<5));

    TEST("PVALID VAL, (A2)",          0xF000|022, 0x2800);
    TEST("PVALID VAL, ($1234,A2)",    0xF000|052, 0x2800, 0x1234);
    TEST("PVALID VAL, ($34,A2,D3.W)", 0xF000|062, 0x2800, 0x3034);
    TEST("PVALID VAL, ($00001234).W", 0xF000|070, 0x2800, 0x1234);
    TEST("PVALID VAL, ($12345678).L", 0xF000|071, 0x2800, 0x1234, 0x5678);

    ERRT("PVALID A3, D2", OPERAND_NOT_ALLOWED, "D2", 0xF000, 0x2C00|3);
    ERRT("PVALID A3, A2", OPERAND_NOT_ALLOWED, "A2", 0xF000, 0x2C00|3);
    TEST("PVALID A3, (A2)",          0xF000|022, 0x2C00|3);
    ERRT("PVALID D3, (A2)",  OPERAND_NOT_ALLOWED, "D3, (A2)");
    ERRT("PVALID A3, (A2)+", OPERAND_NOT_ALLOWED, "(A2)+",   0xF000, 0x2C00|3);
    ERRT("PVALID A3, -(A2)", OPERAND_NOT_ALLOWED, "-(A2)",   0xF000, 0x2C00|3);
    TEST("PVALID A4, ($1234,A2)",    0xF000|052, 0x2C00|4, 0x1234);
    TEST("PVALID A5, ($34,A2,D3.W)", 0xF000|062, 0x2C00|5, 0x3034);
    TEST("PVALID A6, ($00001234).W", 0xF000|070, 0x2C00|6, 0x1234);
    TEST("PVALID A7, ($12345678).L", 0xF000|071, 0x2C00|7, 0x1234, 0x5678);
    ERRT("PVALID A4, (*+$1234,PC)",  OPERAND_NOT_ALLOWED, "(*+$1234,PC)",  0xF000, 0x2C00|4);
    ERRT("PVALID A5, (*+$34,PC,D3)", OPERAND_NOT_ALLOWED, "(*+$34,PC,D3)", 0xF000, 0x2C00|5);
    ERRT("PVALID A6, #$1234",        OPERAND_NOT_ALLOWED, "#$1234",        0xF000, 0x2C00|6);
}
#endif

void test_comment() {
    COMM("NOP           ; comment", "; comment", 0047161);
    COMM("ORI  # 0 , CCR; comment", "; comment", 0000074, 0x0000);
    COMM("NOP             comment", "comment", 0047161);
    COMM("ORI  # 0 , CCR  comment", "comment", 0000074, 0x0000);
    COMM("ANDI # 0 , SR   comment", "comment", 0001174, 0x0000);
    COMM("MOVE ( 0 ) , D1 comment", "comment", 0031070, 0x0000);
    COMM("MOVE # 0 , D1   comment", "comment", 0031074, 0x0000);
    COMM("MOVE D1 , ( 0 ) comment", "comment", 0030701, 0x0000);
    COMM("MOVE D2 , ( 0 , A3 )      comment", "comment", 0033502, 0x0000);
    COMM("MOVE ( 0 , A1 , D2 ) , D3 comment", "comment", 0033061, 0x2000);
    COMM("MOVEP D1 , ( 0 , A1 ) comment",     "comment", 0001611, 0x0000);
    COMM("MOVEP ( 0 , A1 ) , D1 comment",     "comment", 0001411, 0x0000);
    COMM("MOVEQ # 0 , D1        comment",     "comment", 0071000);
    COMM("ADDQ  # 8 , D1        comment",     "comment", 0050101);
    COMM("ADDQ  # 1 , ( 0 , A1 ) comment",    "comment", 0051151, 0x0000);
    COMM("LSR   # 8 , D1       comment",      "comment", 0160111);
    COMM("ROR   ( 0 , A1 )     comment",      "comment", 0163351, 0x0000);

    ACOMM(0x1000, "BRA   *      comment", "comment", 0060000 | 0xFE);
    ACOMM(0x1000, "DBRA  D0 , * comment", "comment", 0050710, 0xFFFE);
    ACOMM(0x1000, "MOVEA ( * , PC ) , A1        comment", "comment", 0031172, 0xFFFE);
    ACOMM(0x1000, "MOVEA ( * , PC , D1.L ) , A1 comment", "comment", 0031173, 0x18FE);

    COMM("DC.B -128, 255 comment", "comment", 0x80FF);
    COMM("DC.B 'TEXT'    comment", "comment", 0x5445, 0x5854);
    COMM("DC.W -128, 255 comment", "comment", 0xFF80, 0x00FF);
    COMM("DC.L -128, 255 comment", "comment", 0xFFFF, 0xFF80, 0x0000, 0x00FF);
#if defined(LIBASM_MC68000_NOFPU)
    ERUI("DC.S 1.0   comment");
    ERUI("DC.D -1.0  comment");
    ERUI("DC.X -2.25 comment");
    ERUI("DC.P -2    comment");
#elif defined(LIBASM_ASM_NOFLOAT)
    ERRT("DC.S 1.0   comment", FLOAT_NOT_SUPPORTED, "1.0   comment", 0, 0);
    ERRT("DC.D -1.0  comment", FLOAT_NOT_SUPPORTED, "-1.0  comment", 0, 0, 0, 0);
    ERRT("DC.X -2.25 comment", FLOAT_NOT_SUPPORTED, "-2.25 comment", 0, 0, 0, 0, 0, 0);
    ERRT("DC.P -2    comment", FLOAT_NOT_SUPPORTED, "-2    comment", 0, 0, 0, 0, 0, 0);
#else
    COMM("DC.S 1.0   comment", "comment", 0x3F80, 0x0000);
    COMM("DC.D -1.0  comment", "comment", 0xBFF0, 0x0000, 0x0000, 0x0000);
    COMM("DC.X -2.25 comment", "comment", 0xC000, 0x0000, 0x9000, 0x0000, 0x0000, 0x0000);
    COMM("DC.P -2.25 comment", "comment", 0x8000, 0x0002, 0x2500, 0x0000, 0x0000, 0x0000);
#endif
}

void test_undef() {
    ERUS("ORI  #UNDEF,CCR",       "UNDEF,CCR",       0000074, 0x0000);
    ERUS("ANDI #UNDEF,SR",        "UNDEF,SR",        0001174, 0x0000);
    ERUS("MOVE (UNDEF),D1",       "UNDEF),D1",       0031070, 0x0000);
    ERUS("MOVE #UNDEF,D1",        "UNDEF,D1",        0031074, 0x0000);
    ERUS("MOVE D1,(UNDEF)",       "UNDEF)",          0030701, 0x0000);
    ERUS("MOVE D2,(UNDEF,A3)",    "UNDEF,A3)",       0033502, 0x0000);
    ERUS("MOVE (UNDEF,A1,D2),D3", "UNDEF,A1,D2),D3", 0033061, 0x2000);
    ERUS("MOVEP D1,(UNDEF,A1)",   "UNDEF,A1)",       0001611, 0x0000);
    ERUS("MOVEP (UNDEF,A1),D1",   "UNDEF,A1),D1",    0001411, 0x0000);
    ERUS("MOVEQ #UNDEF,D1",       "UNDEF,D1",        0071000);
    ERUS("ADDQ  #UNDEF,D1",       "UNDEF,D1",        0050101);
    ERUS("ADDQ  #1,(UNDEF,A1)",   "UNDEF,A1)",       0051151, 0x0000);
    ERUS("LSR   #UNDEF,D1",       "UNDEF,D1",        0160111);
    ERUS("ROR   (UNDEF,A1)",      "UNDEF,A1)",       0163351, 0x0000);
    if (!firstGen()) {
        ERUS("BFSET (4){1:UNDEF}",     "UNDEF}",       0167370, 0000100, 0x0004);
        ERUS("BFSET (4){UNDEF:2}",     "UNDEF:2}",     0167370, 0000002, 0x0004);
        ERUS("BFSET (4){UNDEF:UNDEF}", "UNDEF:UNDEF}", 0167370, 0000000, 0x0004);
        ERUS("BFSET (UNDEF){1:2}",         "UNDEF){1:2}",         0167370, 0000102, 0x0000);
        ERUS("BFSET (UNDEF){1:UNDEF}",     "UNDEF){1:UNDEF}",     0167370, 0000100, 0x0000);
        ERUS("BFSET (UNDEF){UNDEF:2}",     "UNDEF){UNDEF:2}",     0167370, 0000002, 0x0000);
        ERUS("BFSET (UNDEF){UNDEF:UNDEF}", "UNDEF){UNDEF:UNDEF}", 0167370, 0000000, 0x0000);
    }

    AERUS(0x1000, "BRA UNDEF",                "UNDEF",             0060000, 0x0000);
    AERUS(0x1000, "DBRA  D0,UNDEF",           "UNDEF",             0050710, 0x0000);
    AERUS(0x1000, "MOVEA (UNDEF,PC),A1",      "UNDEF,PC),A1",      0031172, 0x0000);
    AERUS(0x1000, "MOVEA (UNDEF,PC,D1.L),A1", "UNDEF,PC,D1.L),A1", 0031173, 0x1800);
    if (!firstGen()) {
        AERUS(0x1000, "MOVE ([UNDEF,PC],D1),D2", "UNDEF,PC],D1),D2", 0032073, 0x1125, 0x0000);
        AERUS(0x1000, "MOVE ([PC,D1],UNDEF),D2", "UNDEF),D2",        0032073, 0x1112, 0x0000);
        AERUS(0x1000, "MOVE ([PC],D1,UNDEF),D2", "UNDEF),D2",        0032073, 0x1116, 0x0000);
        AERUS(0x1000, "MOVE ([UNDEF]),D2", "UNDEF]),D2",             0032060, 0x01E1, 0x0000);
    }
}

void test_data_constant() {
    BTEST("DC.B -128, 255", 0x80, 0xFF);
    BTEST(R"(DC.B 'A', '"')", 0x41, 0x22);
    BTEST("DC.B '9'-'0'",   0x09);
    BTEST("DC.B ''''",      0x27);
    ERRT("DC.B '''",        MISSING_CLOSING_QUOTE, "'''");
    BTEST("DC.B 'A''B',0",  0x41, 0x27, 0x42, 0x00);
    ERRT("DC.B 'A''B,0",    MISSING_CLOSING_QUOTE, "'A''B,0");
    BTEST("DC.W -128, 255", 0xFF, 0x80, 0x00, 0xFF);
    BTEST("DC.W 'X'",       0x58, 0x00);
    BTEST("DC.W 'X'+0",     0x00, 0x58);
    BTEST("DC.W 'A''B'",    0x41, 0x27, 0x42, 0x00);
    ERRT("DC.W 'A''B",      MISSING_CLOSING_QUOTE, "'A''B");
    BTEST("DC 'X'+0, 'Y'",  0x00, 0x58, 0x59, 0x00);
    BTEST("DC   -128, 255", 0xFF, 0x80, 0x00, 0xFF);
    BTEST("DC   'A''B'",    0x41, 0x27, 0x42, 0x00);
    ERRT("DC   'A''B",      MISSING_CLOSING_QUOTE, "'A''B");
    BTEST("DC.L $1234",     0x00, 0x00, 0x12, 0x34);
    BTEST("DC.L $12345678", 0x12, 0x34, 0x56, 0x78);
    BTEST("DC.L 'X'",       0x58, 0x00, 0x00, 0x00);
    BTEST("DC.L 'X'+0",     0x00, 0x00, 0x00, 0x58);
    BTEST(R"(DC.L 'A''B"C')", 0x41, 0x27, 0x42, 0x22, 0x43, 0x00, 0x00, 0x00);
    ERRT(R"(DC.L 'A''B"C)",   MISSING_CLOSING_QUOTE, "'A''B\"C");
    BERRT("DC.B 1, UNDEF, 2", UNDEFINED_SYMBOL, "UNDEF, 2", 0x01, 0x00, 0x02);
    ERUS( "DC.W 1, UNDEF, 2", "UNDEF, 2", 0x0001, 0x0000, 0x0002);
    ERUS( "DC.L 1, UNDEF, 2", "UNDEF, 2", 0x0000, 0x0001, 0x0000, 0x0000, 0x0000, 0x0002);

    ERRT("DC.B '"
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

    ERRT("DC.L "
         "$12345678, $9ABCDEF0, $12345678, $9ABCDEF0, "
         "$12345678, $9ABCDEF0, $12345678, $9ABCDEF0, "
         "$12345678, $9ABCDEF0, $12345678, $9ABCDEF0, "
         "$12345678, $9ABCDEF0, $12345678, $9ABCDE, "
         "$AAAABBBB",
         NO_MEMORY,
         "$AAAABBBB",
         0x1234, 0x5678, 0x9ABC, 0xDEF0, 0x1234, 0x5678, 0x9ABC, 0xDEF0,
         0x1234, 0x5678, 0x9ABC, 0xDEF0, 0x1234, 0x5678, 0x9ABC, 0xDEF0,
         0x1234, 0x5678, 0x9ABC, 0xDEF0, 0x1234, 0x5678, 0x9ABC, 0xDEF0,
         0x1234, 0x5678, 0x9ABC, 0xDEF0, 0x1234, 0x5678, 0x009A, 0xBCDE);

#if defined(LIBASM_MC68000_NOFPU)
    ERUI("DC.S  1.0, -inf, +nan");
    ERUI("DC.D -1.0, -inf, +nan");
    ERUI("DC.X -1.0, -inf, +nan");
    ERUI("DC.P -1.0, -inf, +nan");
#elif defined(LIBASM_ASM_NOFLOAT)
    ERRT("DC.S  1.0, -inf, +nan", FLOAT_NOT_SUPPORTED, "1.0, -inf, +nan",
         0, 0, 0, 0, 0, 0);
    ERRT("DC.S  -2.25e2",         FLOAT_NOT_SUPPORTED, "-2.25e2",
         0, 0);
    ERRT("DC.D -1.0, +inf, -nan", FLOAT_NOT_SUPPORTED, "-1.0, +inf, -nan",
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    ERRT("DC.D -2.25e2",          FLOAT_NOT_SUPPORTED, "-2.25e2",
         0, 0, 0, 0);
    ERRT("DC.X -1.0, -inf, +nan", FLOAT_NOT_SUPPORTED, "-1.0, -inf, +nan",
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    ERRT("DC.X -2.25",            FLOAT_NOT_SUPPORTED, "-2.25",
         0, 0, 0, 0, 0, 0);
    ERRT("DC.X -9.87e123",        FLOAT_NOT_SUPPORTED, "-9.87e123",
         0, 0, 0, 0, 0, 0);
    ERRT("DC.P -1.0, +inf, -nan", FLOAT_NOT_SUPPORTED, "-1.0, +inf, -nan",
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    ERRT("DC.P -2.25",            FLOAT_NOT_SUPPORTED, "-2.25",
         0, 0, 0, 0, 0, 0);
    ERRT("DC.P -22.5",            FLOAT_NOT_SUPPORTED, "-22.5",
         0, 0, 0, 0, 0, 0);
    ERRT("DC.P -0.225",           FLOAT_NOT_SUPPORTED, "-0.225",
         0, 0, 0, 0, 0, 0);

    ERRT( "DC.S 1, UNDEF, 2",     FLOAT_NOT_SUPPORTED, "1, UNDEF, 2",
          0, 0, 0, 0, 0, 0);
    ERRT( "DC.D 1, UNDEF, 2",     FLOAT_NOT_SUPPORTED, "1, UNDEF, 2",
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    ERRT( "DC.X 1, UNDEF, 2",     FLOAT_NOT_SUPPORTED, "1, UNDEF, 2",
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    ERRT( "DC.P 1, UNDEF, 2",     FLOAT_NOT_SUPPORTED, "1, UNDEF, 2",
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
#else
    TEST("DC.S  1.0, -inf, +nan",
         0x3F80, 0x0000,
         0xFF80, 0x0000,
         0x7FC0, 0x0000);
    TEST("DC.S  -2.25e2", 0xC361, 0x0000);
    TEST("DC.D -1.0, +inf, -nan",
         0xBFF0, 0x0000, 0x0000, 0x0000,
         0x7FF0, 0x0000, 0x0000, 0x0000,
         0xFFF8, 0x0000, 0x0000, 0x0000);
    TEST("DC.D -2.25e2", 0xC06C, 0x2000, 0x0000, 0x0000);
    TEST("DC.X -1.0, -inf, +nan",
         0xBFFF, 0x0000, 0x8000, 0x0000, 0x0000, 0x0000,
         0xFFFF, 0x0000, 0x8000, 0x0000, 0x0000, 0x0000,
         0x7FFF, 0x0000, 0xC000, 0x0000, 0x0000, 0x0000);
    TEST("DC.X -2.25",
         0xC000, 0x0000, 0x9000, 0x0000, 0x0000, 0x0000);
    TEST("DC.X -9.87e123",
         0xC19A, 0x0000, 0xEEE3, 0xF8B5, 0x7D26, 0xCC2B);
    TEST("DC.P -1.0, +inf, -nan",
         0x8000, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000,
         0x7FFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
         0xFFFF, 0x0000, 0xC000, 0x0000, 0x0000, 0x0000);
    TEST("DC.P -2.25",
         0x8000, 0x0002, 0x2500, 0x0000, 0x0000, 0x0000);
    TEST("DC.P -22.5",
         0x8001, 0x0002, 0x2500, 0x0000, 0x0000, 0x0000);
    TEST("DC.P -0.225",
         0xC001, 0x0002, 0x2500, 0x0000, 0x0000, 0x0000);

    ERUS( "DC.S 1, UNDEF, 2", "UNDEF, 2",
          0x3F80, 0x0000, 0x0000, 0x0000, 0x4000, 0x0000);
    ERUS( "DC.D 1, UNDEF, 2", "UNDEF, 2",
          0x3FF0, 0x0000, 0x0000, 0x0000,
          0x0000, 0x0000, 0x0000, 0x0000,
          0x4000, 0x0000, 0x0000, 0x0000);
    ERUS( "DC.X 1, UNDEF, 2", "UNDEF, 2",
          0x3FFF, 0x0000, 0x8000, 0x0000, 0x0000, 0x0000,
          0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
          0x4000, 0x0000, 0x8000, 0x0000, 0x0000, 0x0000);
    ERUS( "DC.P 1, UNDEF, 2", "UNDEF, 2",
          0x0000, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000,
          0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
          0x0000, 0x0002, 0x0000, 0x0000, 0x0000, 0x0000);
#endif
}

// clang-format on

void run_tests(const char *cpu) {
    assembler.setCpu(cpu);
    RUN_TEST(test_data_move);
    RUN_TEST(test_integer);
    RUN_TEST(test_logical);
    RUN_TEST(test_shift_rotate);
    RUN_TEST(test_bit);
    RUN_TEST(test_bcd);
    RUN_TEST(test_program);
    RUN_TEST(test_system);
    RUN_TEST(test_multiproc);
    RUN_TEST(test_areg_alias);
    RUN_TEST(test_addressing);
    if (!firstGen())
        RUN_TEST(test_bitfield);
#if !defined(LIBASM_MC68000_NOFPU)
    RUN_TEST(test_float_move);
    RUN_TEST(test_float_common);
    if (mc68040()) {
        RUN_TEST(test_float_mc68040);
    } else {
        RUN_TEST(test_float_mc68881);
    }
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
    if (mc68040()) {
        RUN_TEST(test_pmmu_mc68040);
    }
#endif
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

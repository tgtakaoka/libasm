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

#include "asm_pdp11.h"
#include "test_asm_helper.h"

using namespace libasm;
using namespace libasm::pdp11;
using namespace libasm::test;

AsmPdp11 asmpdp11;
Assembler &assembler(asmpdp11);

#if defined(LIBASM_ASM_NOFLOAT)
#define FLTF(insn, opr, opc, ...) ERRT(insn " " opr, FLOAT_NOT_SUPPORTED, opr, opc, 0, 0)
#define FLTD(insn, opr, opc, ...) ERRT(insn " " opr, FLOAT_NOT_SUPPORTED, opr, opc, 0, 0, 0, 0)

#else
#define FLTF(insn, opr, opc, ...) TEST(insn " " opr, opc, __VA_ARGS__)
#define FLTD(insn, opr, opc, ...) TEST(insn " " opr, opc, __VA_ARGS__)
#endif
static bool dcj11() {
    return strcasecmp_P("J11", assembler.config().cpu_P()) == 0;
}

static void set_up() {
    assembler.reset();
    assembler.setOption("relative", "true");
}

static void tear_down() {
    symtab.reset();
}

// clang-format off
void test_cpu() {
    EQUALS("cpu DCJ11", true,    assembler.setCpu("DCJ11"));
    EQUALS_P("cpu DCJ11", "J11", assembler.config().cpu_P());

    EQUALS("cpu DCT11", true,    assembler.setCpu("dct11"));
    EQUALS_P("cpu DCT11", "T11", assembler.config().cpu_P());

    EQUALS("cpu J11", true,    assembler.setCpu("J11"));
    EQUALS_P("cpu J11", "J11", assembler.config().cpu_P());

    EQUALS("cpu T11", true,    assembler.setCpu("T11"));
    EQUALS_P("cpu T11", "T11", assembler.config().cpu_P());
}

void test_double_operands() {
    // Rn
    TEST("MOV R0, R1",          0010001);
    TEST("MOV %0, %1",          0010001);
    TEST("MOV R1, (R2)",        0010112);
    TEST("MOV %2-1, (%1+1)",    0010112);
    TEST("MOV R2, (R3)+",       0010223);
    TEST("MOV %<2>, (%<6-3>)+", 0010223);
    ERRT("MOV R2, #012345",     OPERAND_NOT_ALLOWED,
         "#012345",             0010227, 012345);
    TEST("CMP R2, #012345",     0020227, 012345);
    TEST("MOV R3, @(R4)+",      0010334);
    ERRT("MOV R3, @#012345",    OPERAND_NOT_ALIGNED,
         "@#012345",            0010337, 012345);
    TEST("MOV R3, @#012346",    0010337, 012346);
    TEST("MOV R4, -(R5)",       0010445);
    TEST("MOV R5, @-(SP)",      0010556);
    ERRT("MOV SP, .+012345",    OPERAND_NOT_ALIGNED,
         ".+012345",            0010667, 012341);
    TEST("MOV SP, .+012346",    0010667, 012342);
    TEST("MOV PC, @012345(R0)", 0010770, 012345);
    ERRT("MOV PC, @.+012345",   OPERAND_NOT_ALIGNED,
         "@.+012345",           0010777, 012341);
    TEST("MOV PC, @.+012346",   0010777, 012342);

    // (Rn)
    TEST("MOV (R2), R1",          0011201);
    TEST("MOV @R2, R1",           0011201);
    TEST("MOV (R1), @R2",         0011112);
    TEST("MOV (R1), (R2)",        0011112);
    TEST("MOV @R1, @R2",          0011112);
    TEST("MOV (R2), (R3)+",       0011223);
    ERRT("MOV (R2), #012345",     OPERAND_NOT_ALLOWED,
         "#012345",               0011227, 012345);
    TEST("CMP (R2), #012345",     0021227, 012345);
    TEST("MOV (R3), @(R4)+",      0011334);
    ERRT("MOV (R3), @#012345",    OPERAND_NOT_ALIGNED,
         "@#012345",              0011337, 012345);
    TEST("MOV (R3), @#012346",    0011337, 012346);
    TEST("MOV (R4), -(R5)",       0011445);
    TEST("MOV (R5), @-(SP)",      0011556);
    ERRT("MOV (SP), .+012345",    OPERAND_NOT_ALIGNED,
         ".+012345",              0011667, 012341);
    TEST("MOV (SP), .+012346",    0011667, 012342);
    TEST("MOV (PC), @012345(R0)", 0011770, 012345);
    ERRT("MOV (PC), @.+012345",   OPERAND_NOT_ALIGNED,
         "@.+012345",             0011777, 012341);
    TEST("MOV (PC), @.+012346",   0011777, 012342);

    // (Rn)+
    TEST("MOV (R3)+, R2",          0012302);
    TEST("MOV (R1)+, (R2)",        0012112);
    TEST("MOV (R2)+, (R3)+",       0012223);
    ERRT("MOV (R2)+, #012345",     OPERAND_NOT_ALLOWED,
         "#012345",                0012227, 012345);
    TEST("CMP (R2)+, #012345",     0022227, 012345);
    TEST("MOV (R3)+, @(R4)+",      0012334);
    ERRT("MOV (R3)+, @#012345",    OPERAND_NOT_ALIGNED,
         "@#012345",               0012337, 012345);
    TEST("MOV (R3)+, @#012346",    0012337, 012346);
    TEST("MOV (R4)+, -(R5)",       0012445);
    TEST("MOV (R5)+, @-(SP)",      0012556);
    ERRT("MOV (SP)+, .+012345",    OPERAND_NOT_ALIGNED,
         ".+012345",               0012667, 012341);
    TEST("MOV (SP)+, .+012346",    0012667, 012342);
    ERRT("MOV (PC)+, @012345(R0)", NOT_AN_EXPECTED,
         "(PC)+, @012345(R0)",     0012770, 000000, 012345);
    ERRT("MOV (PC)+, @.+012345",   NOT_AN_EXPECTED,
         "(PC)+, @.+012345",       0012777, 000000, 012337);
    ERRT("MOV (PC)+, @.+012346",   NOT_AN_EXPECTED,
         "(PC)+, @.+012346",       0012777, 000000, 012340);

    // #imm
    TEST("MOV #034567, R2",          0012702, 034567);
    TEST("MOV #034567, (R2)",        0012712, 034567);
    TEST("MOV #034567, (R3)+",       0012723, 034567);
    ERRT("MOV #034567, #012345",     OPERAND_NOT_ALLOWED,
         "#012345",                  0012727, 034567, 012345);
    TEST("CMP #034567, #012345",     0022727, 034567, 012345);
    TEST("MOV #034567, @(R4)+",      0012734, 034567);
    ERRT("MOV #034567, @#012345",    OPERAND_NOT_ALIGNED,
         "@#012345",                 0012737, 034567, 012345);
    TEST("MOV #034567, @#012346",    0012737, 034567, 012346);
    TEST("MOV #034567, -(R5)",       0012745, 034567);
    TEST("MOV #034567, @-(SP)",      0012756, 034567);
    ERRT("MOV #034567, .+012345",    OPERAND_NOT_ALIGNED,
         ".+012345",                 0012767, 034567, 012337);
    TEST("MOV #034567, .+012346",    0012767, 034567, 012340);
    TEST("MOV #034567, @012345(R0)", 0012770, 034567, 012345);
    ERRT("MOV #034567, @.+012345",   OPERAND_NOT_ALIGNED,
         "@.+012345",                0012777, 034567, 012337);
    TEST("MOV #034567, @.+012346",   0012777, 034567, 012340);

    // @(Rn)+
    TEST("MOV @(R4)+, R3",          0013403);
    TEST("MOV @(R1)+, (R2)",        0013112);
    TEST("MOV @(R2)+, (R3)+",       0013223);
    ERRT("MOV @(R2)+, #012345",     OPERAND_NOT_ALLOWED,
         "#012345",                 0013227, 012345);
    TEST("CMP @(R2)+, #012345",     0023227, 012345);
    TEST("MOV @(R3)+, @(R4)+",      0013334);
    ERRT("MOV @(R3)+, @#012345",    OPERAND_NOT_ALIGNED,
         "@#012345",                0013337, 012345);
    TEST("MOV @(R3)+, @#012346",    0013337, 012346);
    TEST("MOV @(R4)+, -(R5)",       0013445);
    TEST("MOV @(R5)+, @-(SP)",      0013556);
    ERRT("MOV @(SP)+, .+012345",    OPERAND_NOT_ALIGNED,
         ".+012345",                0013667, 012341);
    TEST("MOV @(SP)+, .+012346",    0013667, 012342);
    ERRT("MOV @(PC)+, @012345(R0)", NOT_AN_EXPECTED,
         "@(PC)+, @012345(R0)",     0013770, 000000, 012345);
    ERRT("MOV @(PC)+, @.+012345",   NOT_AN_EXPECTED,
         "@(PC)+, @.+012345",       0013777, 000000, 012337);

    // @#abs
    TEST("MOV @#034566, R3",          0013703, 034566);
    ERRT("MOV @#034567, R3",          OPERAND_NOT_ALIGNED,
         "@#034567, R3",              0013703, 034567);
    TEST("MOV @#034566, (R2)",        0013712, 034566);
    TEST("MOV @#034566, (R3)+",       0013723, 034566);
    ERRT("MOV @#034566, #012345",     OPERAND_NOT_ALLOWED,
         "#012345",                   0013727, 034566, 012345);
    TEST("CMP @#034566, #012345",     0023727, 034566, 012345);
    TEST("MOV @#034566, @(R4)+",      0013734, 034566);
    TEST("MOV @#034566, @#012346",    0013737, 034566, 012346);
    ERRT("MOV @#034566, @#012345",    OPERAND_NOT_ALIGNED,
         "@#012345",                  0013737, 034566, 012345);
    ERRT("MOV @#034567, @#012346",    OPERAND_NOT_ALIGNED,
         "@#034567, @#012346",        0013737, 034567, 012346);
    TEST("MOV @#034566, -(R5)",       0013745, 034566);
    TEST("MOV @#034566, @-(SP)",      0013756, 034566);
    ERRT("MOV @#034566, .+012345",    OPERAND_NOT_ALIGNED,
         ".+012345",                  0013767, 034566, 012337);
    TEST("MOV @#034566, .+012346",    0013767, 034566, 012340);
    TEST("MOV @#034566, @012345(R0)", 0013770, 034566, 012345);
    ERRT("MOV @#034566, @.+012345",   OPERAND_NOT_ALIGNED,
         "@.+012345",                 0013777, 034566, 012337);
    TEST("MOV @#034566, @.+012346",   0013777, 034566, 012340);

    // -(Rn)
    TEST("MOV -(R5), R4",          0014504);
    TEST("MOV -(R1), (R2)",        0014112);
    TEST("MOV -(R2), (R3)+",       0014223);
    ERRT("MOV -(R2), #012345",     OPERAND_NOT_ALLOWED,
         "#012345",                0014227, 012345);
    TEST("CMP -(R2), #012345",     0024227, 012345);
    TEST("MOV -(R3), @(R4)+",      0014334);
    ERRT("MOV -(R3), @#012345",    OPERAND_NOT_ALIGNED,
         "@#012345",               0014337, 012345);
    TEST("MOV -(R3), @#012346",    0014337, 012346);
    TEST("MOV -(R4), -(R5)",       0014445);
    TEST("MOV -(R5), @-(SP)",      0014556);
    ERRT("MOV -(SP), .+012345",    OPERAND_NOT_ALIGNED,
         ".+012345",               0014667, 012341);
    TEST("MOV -(SP), .+012346",    0014667, 012342);
    ERRT("MOV -(PC), @012345(R0)", NOT_AN_EXPECTED,
         "-(PC), @012345(R0)",     0014770, 012345);
    ERRT("MOV -(PC), @.+012345",   NOT_AN_EXPECTED,
         "-(PC), @.+012345",       0014777, 012341);
    ERRT("MOV -(PC), @.+012346",   NOT_AN_EXPECTED,
         "-(PC), @.+012346",       0014777, 012342);

    // @-(Rn)
    TEST("MOV @-(SP), R5",          0015605);
    TEST("MOV @-(R1), (R2)",        0015112);
    TEST("MOV @-(R2), (R3)+",       0015223);
    ERRT("MOV @-(R2), #012345",     OPERAND_NOT_ALLOWED,
         "#012345",                 0015227, 012345);
    TEST("CMP @-(R2), #012345",     0025227, 012345);
    TEST("MOV @-(R3), @(R4)+",      0015334);
    ERRT("MOV @-(R3), @#012345",    OPERAND_NOT_ALIGNED,
         "@#012345",                0015337, 012345);
    TEST("MOV @-(R3), @#012346",    0015337, 012346);
    TEST("MOV @-(R4), -(R5)",       0015445);
    TEST("MOV @-(R5), @-(SP)",      0015556);
    ERRT("MOV @-(SP), .+012345",    OPERAND_NOT_ALIGNED,
         ".+012345",                0015667, 012341);
    TEST("MOV @-(SP), .+012346",    0015667, 012342);
    ERRT("MOV @-(PC), @012345(R0)", NOT_AN_EXPECTED,
         "@-(PC), @012345(R0)",     0015770, 012345);
    ERRT("MOV @-(PC), @.+012345",   NOT_AN_EXPECTED,
         "@-(PC), @.+012345",       0015777, 012341);
    ERRT("MOV @-(PC), @.+012346",   NOT_AN_EXPECTED,
         "@-(PC), @.+012346",       0015777, 012342);

    // idx(Rn)
    TEST("MOV 023456(SP), R5",          0016605, 023456);
    TEST("MOV 023456(R1), (R2)",        0016112, 023456);
    TEST("MOV 023456(R2), (R3)+",       0016223, 023456);
    ERRT("MOV 023456(R2), #012345",     OPERAND_NOT_ALLOWED,
         "#012345",                     0016227, 023456, 012345);
    TEST("CMP 023456(R2), #012345",     0026227, 023456, 012345);
    TEST("MOV 023456(R3), @(R4)+",      0016334, 023456);
    ERRT("MOV 023456(R3), @#012345",    OPERAND_NOT_ALIGNED,
         "@#012345",                    0016337, 023456, 012345);
    TEST("MOV 023456(R3), @#012346",    0016337, 023456, 012346);
    TEST("MOV 023456(R4), -(R5)",       0016445, 023456);
    TEST("MOV 023456(R5), @-(SP)",      0016556, 023456);
    ERRT("MOV 023456(SP), .+012345",    OPERAND_NOT_ALIGNED,
         ".+012345",                    0016667, 023456, 012337);
    TEST("MOV 023456(SP), .+012346",    0016667, 023456, 012340);
    TEST("MOV 023456(R1), @012345(R0)", 0016170, 023456, 012345);
    ERRT("MOV 023456(R1), @.+012345",   OPERAND_NOT_ALIGNED,
         "@.+012345",                   0016177, 023456, 012337);
    TEST("MOV 023456(R1), @.+012346",   0016177, 023456, 012340);
    TEST("MOV 023456(PC), @012345(R0)", 0016770, 023456, 012345);
    ERRT("MOV 023455(PC), @.+012346",   OPERAND_NOT_ALIGNED,
         "023455(PC), @.+012346",       0016777, 023455, 012340);
    TEST("MOV 023456(PC), @.+012346",   0016777, 023456, 012340);
    TEST("MOV .+023456, @012345(R0)",   0016770, 023452, 012345);
    ERRT("MOV .+023455, @.+012346",     OPERAND_NOT_ALIGNED,
         ".+023455, @.+012346",         0016777, 023451, 012340);
    TEST("MOV .+023456, @.+012346",     0016777, 023452, 012340);

    // @idx(Rn)
    TEST("MOV @023456(SP), R5",          0017605, 023456);
    TEST("MOV @023456(R1), (R2)",        0017112, 023456);
    TEST("MOV @023456(R2), (R3)+",       0017223, 023456);
    ERRT("MOV @023456(R2), #012345",     OPERAND_NOT_ALLOWED,
         "#012345",                      0017227, 023456, 012345);
    TEST("CMP @023456(R2), #012345",     0027227, 023456, 012345);
    TEST("MOV @023456(R3), @(R4)+",      0017334, 023456);
    ERRT("MOV @023456(R3), @#012345",    OPERAND_NOT_ALIGNED,
         "@#012345",                     0017337, 023456, 012345);
    TEST("MOV @023456(R3), @#012346",    0017337, 023456, 012346);
    TEST("MOV @023456(R4), -(R5)",       0017445, 023456);
    TEST("MOV @023456(R5), @-(SP)",      0017556, 023456);
    ERRT("MOV @023456(SP), .+012345",    OPERAND_NOT_ALIGNED,
         ".+012345",                     0017667, 023456, 012337);
    TEST("MOV @023456(SP), .+012346",    0017667, 023456, 012340);
    TEST("MOV @023456(R1), @012345(R0)", 0017170, 023456, 012345);
    ERRT("MOV @023456(R1), @.+012345",   OPERAND_NOT_ALIGNED,
         "@.+012345",                    0017177, 023456, 012337);
    TEST("MOV @023456(R1), @.+012346",   0017177, 023456, 012340);
    TEST("MOV @023456(PC), @012345(R0)", 0017770, 023456, 012345);
    ERRT("MOV @023455(PC), @.+012346",   OPERAND_NOT_ALIGNED,
         "@023455(PC), @.+012346",       0017777, 023455, 012340);
    TEST("MOV @023456(PC), @.+012346",   0017777, 023456, 012340);
    TEST("MOV @.+023456, @012345(R0)",   0017770, 023452, 012345);
    ERRT("MOV @.+023455, @.+012346",     OPERAND_NOT_ALIGNED,
         "@.+023455, @.+012346",         0017777, 023451, 012340);
    TEST("MOV @.+023456, @.+012346",     0017777, 023452, 012340);

    TEST("MOVB R0, R1",          0110001);
    TEST("MOVB R1, (R2)",        0110112);
    TEST("MOVB R2, (R3)+",       0110223);
    ERRT("MOVB R2, #377",        OPERAND_NOT_ALLOWED,
         "#377",                 0110227, 000377);
    TEST("CMPB R2, #377",        0120227, 000377);
    TEST("MOVB R3, @(R4)+",      0110334);
    TEST("MOVB R3, @#012345",    0110337, 012345);
    TEST("MOVB R4, -(R5)",       0110445);
    TEST("MOVB R5, @-(SP)",      0110556);
    TEST("MOVB SP, .+012345",    0110667, 012341);
    TEST("MOVB PC, @012345(R0)", 0110770, 012345);
    ERRT("MOVB PC, @012345(PC)", OPERAND_NOT_ALIGNED,
         "@012345(PC)",          0110777, 012345);
    TEST("MOVB PC, @012346(PC)", 0110777, 012346);
    ERRT("MOVB PC, @.+012345",   OPERAND_NOT_ALIGNED,
         "@.+012345",            0110777, 012341);
    TEST("MOVB PC, @.+012346",   0110777, 012342);
    TEST("MOVB R1, R0",          0110100);
    TEST("MOVB (R2), R1",        0111201);
    TEST("MOVB (R3)+, R2",       0112302);
    TEST("MOVB #377, R2",        0112702, 000377);
    TEST("MOVB @(R4)+, R3",      0113403);
    TEST("MOVB @#012345, R3",    0113703, 012345);
    TEST("MOVB -(R5), R4",       0114504);
    TEST("MOVB @-(SP), R5",      0115605);
    TEST("MOVB 034567(R1), R5",  0116105, 034567);
    TEST("MOVB .+012345, SP",    0116706, 012341);
    TEST("MOVB @012345(R0), PC", 0117007, 012345);
    ERRT("MOVB @012345(PC), PC", OPERAND_NOT_ALIGNED,
         "@012345(PC), PC",      0117707, 012345);
    TEST("MOVB @012346(PC), PC", 0117707, 012346);
    ERRT("MOVB @.+012345, PC",   OPERAND_NOT_ALIGNED,
         "@.+012345, PC",        0117707, 012341);
    TEST("MOVB @.+012346, PC",   0117707, 012342);

    TEST("CMP R0, R1",           0020001);
    TEST("BIT R1, (R2)",         0030112);
    TEST("BIC R2, (R3)+",        0040223);
    TEST("CMP R3, #012345",      0020327, 012345);
    TEST("BIS R3, @(R4)+",       0050334);
    TEST("ADD R4, @#023456",     0060437, 023456);
    TEST("SUB R4, -(R5)",        0160445);
    TEST("CMP R5, @-(SP)",       0020556);
    TEST("BIT R6, 012345(R0)",   0030660, 012345);
    TEST("BIC R7, .+023456",     0040767, 023452);
    TEST("BIS R0, @012345(R1)",  0050071, 012345);
    TEST("ADD R1, @.+023456",    0060177, 023452);
    TEST("SUB (R2), R4",         0161204);
    TEST("CMP (R3)+, R5",        0022305);
    TEST("BIT #012345, R6",      0032706, 012345);
    TEST("BIC @(R4)+, R7",       0043407);
    TEST("BIS @#023456, R0",     0053700, 023456);
    TEST("ADD -(R5), R1",        0064501);
    TEST("SUB @-(R6), R2",       0165602);
    TEST("CMP 012346(PC), R3",   0026703, 012346);
    ERRT("CMP 012345(PC), R3",   OPERAND_NOT_ALIGNED,
         "012345(PC), R3",       0026703, 012345);
    TEST("CMP .+012346, R3",     0026703, 012342);
    ERRT("CMP .+012345, R3",     OPERAND_NOT_ALIGNED,
         ".+012345, R3",         0026703, 012341);
    TEST("BIT @012345(R0), R4",  0037004, 012345);
    TEST("BIT @012346(PC), R4",  0037704, 012346);
    ERRT("BIT @012345(PC), R4",  OPERAND_NOT_ALIGNED,
         "@012345(PC), R4",      0037704, 012345);
    TEST("BIT @.+012346, R4",    0037704, 012342);
    ERRT("BIT @.+012345, R4",    OPERAND_NOT_ALIGNED,
         "@.+012345, R4"  ,      0037704, 012341);

    TEST("CMPB R0, R1",          0120001);
    TEST("BITB R1, (R2)",        0130112);
    TEST("BICB R2, (R3)+",       0140223);
    TEST("CMPB R3, #0345",       0120327, 000345);
    TEST("BISB R3, @(R4)+",      0150334);
    ERUI("ADDB R4, @#023456");
    TEST("BISB R4, @#023456",    0150437, 023456);
    ERUI("SUBB R4, -(R5)");
    TEST("BISB R4, -(R5)",       0150445);
    TEST("CMPB R5, @-(SP)",      0120556);
    TEST("BITB R6, 012345(R0)",  0130660, 012345);
    TEST("BICB R7, .+023456",    0140767, 023452);
    TEST("BISB R0, @012345(R1)", 0150071, 012345);
    ERUI("ADDB R1, @.+023456");
    TEST("BISB R1, @.+023456",   0150177, 023452);
    ERUI("SUBB (R2), R4");
    TEST("BISB (R2), R4",        0151204);
    TEST("CMPB (R3)+, R5",       0122305);
    TEST("BITB #0345, R6",       0132706, 000345);
    TEST("BICB @(R4)+, R7",      0143407);
    TEST("BISB @#023456, R0",    0153700, 023456);
    ERUI("ADDB -(R5), R1");
    TEST("BISB -(R5), R1",       0154501);
    ERUI("SUBB @-(R6), R2");
    TEST("BISB @-(R6), R2",      0155602);
    TEST("CMPB 012346(PC), R3",  0126703, 012346);
    TEST("CMPB 012345(PC), R3",  0126703, 012345);
    TEST("CMPB .+012346, R3",    0126703, 012342);
    TEST("CMPB .+012345, R3",    0126703, 012341);
    TEST("BITB @012345(R0), R4", 0137004, 012345);
    TEST("BITB @012346(PC), R4", 0137704, 012346);
    ERRT("BITB @012345(PC), R4", OPERAND_NOT_ALIGNED,
         "@012345(PC), R4",      0137704, 012345);
    TEST("BITB @.+012346, R4",   0137704, 012342);
    ERRT("BITB @.+012345, R4",   OPERAND_NOT_ALIGNED,
         "@.+012345, R4"  ,      0137704, 012341);

    TEST("XOR R2, R1",          074201);
    TEST("XOR R2, (R1)",        074211);
    TEST("XOR R2, @R1",         074211);
    TEST("XOR R2, (R1)+",       074221);
    ERRT("XOR R2, (PC)+",       NOT_AN_EXPECTED,
         "(PC)+",               074227, 000000);
    ERRT("XOR R2, #012345",     OPERAND_NOT_ALLOWED,
         "#012345",             074227, 012345);
    TEST("XOR R2, @(R1)+",      074231);
    ERRT("XOR R2, @(PC)+",      NOT_AN_EXPECTED,
         "@(PC)+",              074237, 000000);
    ERRT("XOR R2, @#012345",    OPERAND_NOT_ALIGNED,
         "@#012345",            074237, 012345);
    TEST("XOR R2, @#012346",    074237, 012346);
    TEST("XOR R2, -(R1)",       074241);
    TEST("XOR R2, @-(R1)",      074251);
    ERRT("XOR R2, -(PC)",       NOT_AN_EXPECTED,
         "-(PC)",               074247);
    ERRT("XOR R2, @-(PC)",      NOT_AN_EXPECTED,
         "@-(PC)",              074257);
    TEST("XOR R2, 012345(R1)",  074261, 012345);
    ERRT("XOR R2, 012345(PC)",  OPERAND_NOT_ALIGNED,
         "012345(PC)",          074267, 012345);
    TEST("XOR R2, 012346(PC)",  074267, 012346);
    ERRT("XOR R2, .+012345",    OPERAND_NOT_ALIGNED,
         ".+012345",            074267, 012341);
    TEST("XOR R2, .+012346",    074267, 012342);
    TEST("XOR R2, @012345(R1)", 074271, 012345);
    ERRT("XOR R2, @012345(PC)", OPERAND_NOT_ALIGNED,
         "@012345(PC)",         074277, 012345);
    TEST("XOR R2, @012346(PC)", 074277, 012346);
    ERRT("XOR R2, @.+012345",   OPERAND_NOT_ALIGNED,
         "@.+012345",           074277, 012341);
    TEST("XOR R2, @.+012346",   074277, 012342);

    ERRT("XOR (R2), R1",        OPERAND_NOT_ALLOWED, "(R2), R1");
    ERRT("XOR @R2, R1",         OPERAND_NOT_ALLOWED, "@R2, R1");
    ERRT("XOR (R2)+, R1",       OPERAND_NOT_ALLOWED, "(R2)+, R1");
    ERRT("XOR (PC)+, R1",       OPERAND_NOT_ALLOWED, "(PC)+, R1");
    ERRT("XOR #012345, R1",     OPERAND_NOT_ALLOWED, "#012345, R1");
    ERRT("XOR @(R2)+, R1",      OPERAND_NOT_ALLOWED, "@(R2)+, R1");
    ERRT("XOR @(PC)+, R1",      OPERAND_NOT_ALLOWED, "@(PC)+, R1");
    ERRT("XOR @#012345, R1",    OPERAND_NOT_ALLOWED, "@#012345, R1");
    ERRT("XOR -(R2), R1",       OPERAND_NOT_ALLOWED, "-(R2), R1");
    ERRT("XOR -(PC), R1",       OPERAND_NOT_ALLOWED, "-(PC), R1");
    ERRT("XOR @-(R2), R1",      OPERAND_NOT_ALLOWED, "@-(R2), R1");
    ERRT("XOR @-(PC), R1",      OPERAND_NOT_ALLOWED, "@-(PC), R1");
    ERRT("XOR 012345(R2), R1",  OPERAND_NOT_ALLOWED, "012345(R2), R1");
    ERRT("XOR 012345(PC), R1",  OPERAND_NOT_ALLOWED, "012345(PC), R1");
    ERRT("XOR .+012345, R1",    OPERAND_NOT_ALLOWED, ".+012345, R1");
    ERRT("XOR @012345(R2), R1", OPERAND_NOT_ALLOWED, "@012345(R2), R1");
    ERRT("XOR @012345(PC), R1", OPERAND_NOT_ALLOWED, "@012345(PC), R1");
    ERRT("XOR @.+012345, R1",   OPERAND_NOT_ALLOWED, "@.+012345, R1");

    if (dcj11()) {
        TEST("MUL  R1, R2",        070201);
        TEST("DIV  #^D5349, R3",   071327, 012345);
        TEST("ASH  (R5), R4",      072415);
        TEST("ASHC @#012346, SP",  073637, 012346);
    }

    symtab.intern(5, "FP");
    TEST("MOV %FP, @-(SP)", 0010556);
    ERRT("MOV %8, %FP",     UNKNOWN_REGISTER, "%8, %FP");
    ERRT("MOV %FP+3, %1",   UNKNOWN_REGISTER, "%FP+3, %1");
}

void test_single_operand() {
    TEST("SWAB (R0)",        000310);
    TEST("CLR  (R1)+",       005021);
    ERRT("CLR  (PC)+",       NOT_AN_EXPECTED,
         "(PC)+",            005027, 000000);
    ERRT("CLR  #012345",     OPERAND_NOT_ALLOWED,
         "#012345",          005027, 012345);
    TEST("TST  #012345",     005727, 012345);
    TEST("COM  @(R2)+",      005132);
    ERRT("COM  @(PC)+",      NOT_AN_EXPECTED,
         "@(PC)+",           005137, 000000);
    ERRT("COM  @#012345",    OPERAND_NOT_ALIGNED,
         "@#012345",         005137, 012345);
    TEST("COM  @#012346",    005137, 012346);
    TEST("INC  -(R3)",       005243);
    ERRT("INC  -(PC)",       NOT_AN_EXPECTED,
         "-(PC)",            005247);
    TEST("DEC  @-(R4)",      005354);
    ERRT("DEC  @-(PC)",      NOT_AN_EXPECTED,
         "@-(PC)",           005357);
    TEST("NEG  012345(R5)",  005465, 012345);
    ERRT("NEG  012345(PC)",  OPERAND_NOT_ALIGNED,
         "012345(PC)",       005467, 012345);
    TEST("NEG  012346(PC)",  005467, 012346);
    TEST("ADC  @012345(SP)", 005576, 012345);
    ERRT("ADC  @012345(PC)", OPERAND_NOT_ALIGNED,
         "@012345(PC)",      005577, 012345);
    TEST("ADC  @012346(PC)", 005577, 012346);
    ERRT("SBC  .+012345",    OPERAND_NOT_ALIGNED,
         ".+012345",         005667, 012341);
    TEST("SBC  .+012346",    005667, 012342);
    ERRT("TST  @.+012345",   OPERAND_NOT_ALIGNED,
         "@.+012345",        005777, 012341);
    TEST("TST  @.+012346",   005777, 012342);
    TEST("ROL  (R1)+",       006121);
    TEST("ASR  @(R2)+",      006232);
    TEST("ASL  -(R3)",       006343);
    TEST("SXT  @-(R4)",      006754);

    if (dcj11()) {
        TEST("TSTSET 012345(R5)",  007265, 012345);
        TEST("WRTLCK @012345(SP)", 007376, 012345);
        ERRT("TSTSET R5", REGISTER_NOT_ALLOWED,
             "R5",                 007205);
        ERRT("WRTLCK SP", REGISTER_NOT_ALLOWED,
             "SP",                 007306);
    }

    TEST("CLRB R0",          0105000);
    TEST("CLRB (R1)",        0105011);
    TEST("CLRB (R1)+",       0105021);
    ERRT("CLRB (PC)+",       NOT_AN_EXPECTED,
         "(PC)+",            0105027, 000000);
    ERRT("CLRB #0345",       OPERAND_NOT_ALLOWED,
         "#0345",            0105027, 000345);
    TEST("TSTB #0345",       0105727, 000345);
    TEST("COMB @(R2)+",      0105132);
    ERRT("COMB @(PC)+",      NOT_AN_EXPECTED,
         "@(PC)+",           0105137, 000000);
    TEST("INCB -(R3)",       0105243);
    ERRT("INCB -(PC)",       NOT_AN_EXPECTED,
         "-(PC)",            0105247);
    TEST("DECB @-(R4)",      0105354);
    ERRT("DECB @-(PC)",      NOT_AN_EXPECTED,
         "@-(PC)",           0105357);
    TEST("NEGB 012345(R5)",  0105465, 012345);
    TEST("NEGB 012345(PC)",  0105467, 012345);
    TEST("ADCB @012345(SP)", 0105576, 012345);
    ERRT("ADCB @012345(PC)", OPERAND_NOT_ALIGNED,
         "@012345(PC)",      0105577, 012345);
    TEST("ADCB @012346(PC)", 0105577, 012346);
    TEST("SBCB .+012345",    0105667, 012341);
    ERRT("TSTB @.+012345",   OPERAND_NOT_ALIGNED,
         "@.+012345",        0105777, 012341);
    TEST("TSTB @.+012346",   0105777, 012342);
    TEST("ROLB (R1)+",       0106121);
    TEST("ASRB @(R2)+",      0106232);
    TEST("ASLB -(R3)",       0106343);

    ATEST(0000100, "TST 0000100", 0005767, 0177774);
    ATEST(0000100, "TST 0000104", 0005767, 0000000);
    ATEST(0000100, "TST 0000000", 0005767, 0177674);
    AERRT(0000100, "TST 0177776", OVERFLOW_RANGE,
          "0177776",              0005767, 0177672);
    ATEST(0177700, "TST 0177776", 0005767, 0000072);
    AERRT(0177700, "TST 0000000", OVERFLOW_RANGE,
          "0000000",              0005767, 0000074);
}

void test_program_control() {
    TEST("BPT",       0000003);
    TEST("IOT",       0000004);
    TEST("EMT  377",  0104377);
    TEST("TRAP 377",  0104777);
    TEST("RTS  R2",   0000202);

    ERRT("JMP R3",          REGISTER_NOT_ALLOWED,
         "R3",              000103);
    TEST("JMP (R3)",        000113);
    TEST("JMP (R3)+",       000123);
    ERRT("JMP (PC)+",       NOT_AN_EXPECTED,
         "(PC)+",           000127, 000000);
    ERRT("JMP #012345",     OPERAND_NOT_ALLOWED,
         "#012345",         000127, 012345);
    TEST("JMP @(R3)+",      000133);
    ERRT("JMP @(PC)+",      NOT_AN_EXPECTED,
         "@(PC)+",          000137, 000000);
    ERRT("JMP @#012345",    OPERAND_NOT_ALIGNED,
         "@#012345",        000137, 012345);
    TEST("JMP @#012346",    000137, 012346);
    TEST("JMP -(R3)",       000143);
    ERRT("JMP -(PC)",       NOT_AN_EXPECTED,
         "-(PC)",           000147);
    TEST("JMP @-(R3)",      000153);
    ERRT("JMP @-(PC)",      NOT_AN_EXPECTED,
         "@-(PC)",          000157);
    TEST("JMP 012345(R3)",  000163, 012345);
    ERRT("JMP 012345(PC)",  OPERAND_NOT_ALIGNED,
         "012345(PC)",      000167, 012345);
    TEST("JMP 012346(PC)",  000167, 012346);
    ERRT("JMP .+012345",    OPERAND_NOT_ALIGNED,
         ".+012345",        000167, 012341);
    TEST("JMP .+012346",    000167, 012342);
    ERRT("JMP @012345(PC)", OPERAND_NOT_ALIGNED,
         "@012345(PC)",     000177, 012345);
    TEST("JMP @012346(PC)", 000177, 012346);
    ERRT("JMP @.+012345",   OPERAND_NOT_ALIGNED,
         "@.+012345",       000177, 012341);
    TEST("JMP @.+012346",   000177, 012342);

    ERRT("JSR R2, R3",          REGISTER_NOT_ALLOWED,
         "R3",                  004203);
    TEST("JSR R2, (R3)",        004213);
    TEST("JSR R2, (R3)+",       004223);
    ERRT("JSR R2, #012345",     OPERAND_NOT_ALLOWED,
         "#012345",             004227, 012345);
    TEST("JSR R2, @(R3)+",      004233);
    ERRT("JSR R2, @#012345",    OPERAND_NOT_ALIGNED,
         "@#012345",            004237, 012345);
    TEST("JSR R2, @#012346",    004237, 012346);
    TEST("JSR R2, -(R3)",       004243);
    ERRT("JSR R2, -(PC)",       NOT_AN_EXPECTED,
         "-(PC)",               004247);
    TEST("JSR R2, @-(R3)",      004253);
    ERRT("JSR R2, @-(PC)",      NOT_AN_EXPECTED,
         "@-(PC)",              004257);
    TEST("JSR R2, 012345(R3)",  004263, 012345);
    ERRT("JSR R2, .+012345",    OPERAND_NOT_ALIGNED,
         ".+012345",            004267, 012341);
    TEST("JSR R2, .+012346",    004267, 012342);
    TEST("JSR R2, @012345(R3)", 004273, 012345);
    ERRT("JSR R2, @.+012345",   OPERAND_NOT_ALIGNED,
         "@.+012345",           004277, 012341);
    TEST("JSR R2, @.+012346",   004277, 012342);

    if (dcj11()) {
        TEST("SPL  3",   000233);
        TEST("MARK 04",  006404);

        TEST("CSM R3",          007003);
        TEST("CSM (R3)",        007013);
        TEST("CSM (R3)+",       007023);
        ERRT("CSM (PC)+",       NOT_AN_EXPECTED,
             "(PC)+",           007027, 000000);
        TEST("CSM #012345",     007027, 012345);
        TEST("CSM @(R3)+",      007033);
        ERRT("CSM @(PC)+",      NOT_AN_EXPECTED,
             "@(PC)+",          007037, 000000);
        ERRT("CSM @#012345",    OPERAND_NOT_ALIGNED,
             "@#012345",        007037, 012345);
        TEST("CSM @#012346",    007037, 012346);
        TEST("CSM -(R3)",       007043);
        ERRT("CSM -(PC)",       NOT_AN_EXPECTED,
             "-(PC)",           007047);
        TEST("CSM @-(R3)",      007053);
        ERRT("CSM @-(PC)",      NOT_AN_EXPECTED,
             "@-(PC)",          007057);
        TEST("CSM 012345(R3)",  007063, 012345);
        ERRT("CSM 012345(PC)",  OPERAND_NOT_ALIGNED,
             "012345(PC)",      007067, 012345);
        TEST("CSM 012346(PC)",  007067, 012346);
        ERRT("CSM .+012345",    OPERAND_NOT_ALIGNED,
             ".+012345",        007067, 012341);
        TEST("CSM .+012346",    007067, 012342);
        ERRT("CSM @012345(PC)", OPERAND_NOT_ALIGNED,
             "@012345(PC)",     007077, 012345);
        TEST("CSM @012346(PC)", 007077, 012346);
        ERRT("CSM @.+012345",   OPERAND_NOT_ALIGNED,
             "@.+012345",       007077, 012341);
        TEST("CSM @.+012346",   007077, 012342);
    }

    ATEST(0001000, "SOB  R2, .-174",   0077277);
    ATEST(0001000, "SOB  R2, .+2",     0077200);
    AERRT(0001000, "SOB  R2, .-176",
          OPERAND_TOO_FAR,  ".-176",   0077200);
    AERRT(0001000, "SOB  R2, .+4",
          OPERAND_TOO_FAR,  ".+4",     0077277);
    AERRT(0001000, "SOB  R2, .-175",
          OPERAND_NOT_ALIGNED,".-175", 0077277);
    ATEST(0001000, "BR   .-376", 0000600);
    ATEST(0001000, "BNE  .",     0001377);
    ATEST(0001000, "BEQ  .+400", 0001577);
    ATEST(0001000, "BGE  .+002", 0002000);
    ATEST(0001000, "BLT  .-376", 0002600);
    ATEST(0001000, "BGT  .",     0003377);
    ATEST(0001000, "BLE  .+400", 0003577);
    ATEST(0001000, "BLT  .+002", 0002400);
    ATEST(0001000, "BPL  .-376", 0100200);
    ATEST(0001000, "BMI  .+002", 0100400);
    ATEST(0001000, "BHI  .",     0101377);
    ATEST(0001000, "BLOS .+400", 0101577);
    ATEST(0001000, "BVC  .+002", 0102000);
    ATEST(0001000, "BVS  .-376", 0102600);
    ATEST(0001000, "BCC  .",     0103377);
    ATEST(0001000, "BCS  .+400", 0103577);

    AERRT(0001000, "BCS  .+402",
          OPERAND_TOO_FAR, ".+402",     0103600);
    AERRT(0001000, "BCS  .-400",
          OPERAND_TOO_FAR, ".-400",     0103577);
    AERRT(0001000, "BCS  .-375",
          OPERAND_NOT_ALIGNED, ".-375", 0103601);
    ATEST(0000100, "BCS  .-100",        0103737);
    AERRT(0000100, "BCS  .-102",
          OVERFLOW_RANGE,  ".-102",     0103736);
    ATEST(0177700, "BCS  .+076",        0103436);
    AERRT(0177700, "BCS  .+100",
          OVERFLOW_RANGE,  ".+100",     0103437);
}

void test_condition_code() {
    TEST("NOP", 000240);
    TEST("CLC", 000241);
    TEST("CLV", 000242);
    TEST("CLZ", 000244);
    TEST("CLN", 000250);
    TEST("CCC", 000257);
    TEST("SEC", 000261);
    TEST("SEV", 000262);
    TEST("SEZ", 000264);
    TEST("SEN", 000270);
    TEST("SCC", 000277);
}

void test_miscellaneous() {
    TEST("HALT",  000000);
    TEST("WAIT",  000001);
    TEST("RTI",   000002);
    TEST("RESET", 000005);
    TEST("RTT",   000006);
    TEST("MFPT",  000007);
    TEST("MTPS R1",   0106401);
    TEST("MTPS #377", 0106427, 000377);
    ERRT("MTPS #400", OVERFLOW_RANGE,
         "#400",      0106427, 000400);
    TEST("MFPS R1",   0106701);

    if (dcj11()) {
        TEST("MFPI (R1)+",  0006521);
        TEST("MTPI -(R2)",  0006642);
        TEST("MFPD @(R3)+", 0106533);
        TEST("MTPD @-(R4)", 0106654);
        ERRT("MFPI R1",     REGISTER_NOT_ALLOWED,
             "R1",          0006501);
        ERRT("MTPI R2",     REGISTER_NOT_ALLOWED,
             "R2",          0006602);
        ERRT("MFPD R3",     REGISTER_NOT_ALLOWED,
             "R3",          0106503);
        ERRT("MTPD R4",     REGISTER_NOT_ALLOWED,
             "R4",          0106604);
    }
}

void test_floating_point() {
    TEST("CFCC", 0170000);
    TEST("SETF", 0170001);
    TEST("SETI", 0170002);
    TEST("SETD", 0170011);
    TEST("SETL", 0170012);

    TEST("LDFPS (R1)",         0170111);
    TEST("LDFPS #123456",      0170127, 0123456);
    TEST("LDEXP (R2)+, AC3",   0176722);
    TEST("LDEXP #000020, AC3", 0176727, 000020);
    TEST("STFPS -(SP)",   0170246);
    TEST("STST  (R4)",    0170314);
    TEST("CLRD  AC5",     0170405);
    TEST("TSTD  (R4)",    0170514);
    TEST("ABSD  (R3)+",   0170623);
    TEST("NEGD  -(R2)",   0170742);
    TEST("MULD  AC1, AC2",    0171201);
    TEST("MODD  (R1), AC3",   0171711);
    TEST("ADDD  (R2)+, AC1",  0172122);
    TEST("LDD   -(R3), AC0",  0172443);
    TEST("SUBD  @(R4)+, AC3", 0173334);
    TEST("STD   AC1, -(SP)",  0174146);
    TEST("DIVD  (SP)+, AC0",  0174426);
    TEST("STEXP AC3, (R1)",   0175311);
    TEST("STCDL AC2, -(SP)",  0175646);
    TEST("STCDL AC2, R5",     0175605);
    TEST("STCDF AC1, (R2)+",  0176122);
    ERRT("STCDF AC1, R4",
         REGISTER_NOT_ALLOWED, "R4", 0176104);
    TEST("LDEXP (R3), AC0",    0176413);
    TEST("LDEXP #000020, AC3", 0176727, 000020);
    TEST("LDCLD (SP)+, AC1",   0177126);
    ERRT("LDCLD (SP)+, R1",
         REGISTER_NOT_ALLOWED, "R1", 0177126);
    TEST("LDCLD #^D1234567890, AC2",  0177227, 1234567890 >> 16, 1234567890 & 0xFFFF);
    TEST("LDCLF #^D 1234567890, AC2", 0177227, 1234567890 >> 16, 1234567890 & 0xFFFF);
    ERRT("LDCLF (R4)+, R2",
         REGISTER_NOT_ALLOWED, "R2",  0177224);
    TEST("LDCID #12345., AC2",        0177227, 12345);
    ERRT("LDCID #^D(12345), R0",
         REGISTER_NOT_ALLOWED, "R0",  0177027, 12345);
    TEST("LDCIF #^D<12345>, AC2",     0177227, 12345);
    ERRT("LDCIF #^D<12345>, AC5",
         REGISTER_NOT_ALLOWED, "AC5", 0177127, 12345);
    TEST("LDCDF (R4)+, AC2",          0177624);
    ERRT("LDCDF (R4)+, AC4",
         REGISTER_NOT_ALLOWED, "AC4", 0177424);
    TEST("LDCFD (R4)+, AC2",          0177624);
    ERRT("LDCFD (R4)+, R7",
         REGISTER_NOT_ALLOWED, "R7",  0177724);

    FLTD("LDCDF", "#-2.5, AC1", 0177527, 0xC120, 0x0000, 0x0000, 0x0000);
    FLTF("LDCFD", "#-2.5, AC1", 0177527, 0xC120, 0x0000);

    FLTD("LDD", "#1.70141183460469229371E+38, AC0",  0172427, 0x7FFF, 0xFFFF, 0xFFFF, 0xFFFF);
    FLTD("LDD", "#1, AC0",                           0172427, 0x4080, 0x0000, 0x0000, 0x0000);
    FLTD("LDD", "#2.93873587705571876992E-39, AC0",  0172427, 0x0080, 0x0000, 0x0000, 0x0000);
    FLTD("LDD", "#0, AC0",                           0172427, 0x0000, 0x0000, 0x0000, 0x0000);
    FLTD("LDD", "#-1.70141183460469229371E+38, AC0", 0172427, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF);
    FLTD("LDD", "#-1, AC0",                          0172427, 0xC080, 0x0000, 0x0000, 0x0000);
    FLTD("LDD", "#-2.93873587705571876992E-39, AC0", 0172427, 0x8080, 0x0000, 0x0000, 0x0000);
    FLTD("LDD", "#-0.0, AC0",                        0172427, 0x8000, 0x0000, 0x0000, 0x0000);

    FLTF("LDF", "#1.7014117838E+38, AC0",  0172427, 0x7FFF, 0xFFFF);
    FLTF("LDF", "#1, AC0",                 0172427, 0x4080, 0x0000);
    FLTF("LDF", "#2.9387358770E-39, AC0",  0172427, 0x0080, 0x0000);
    FLTF("LDF", "#0, AC0",                 0172427, 0x0000, 0x0000);
    FLTF("LDF", "#-1.7014117838E+38, AC0", 0172427, 0xFFFF, 0xFFFF);
    FLTF("LDF", "#-1, AC0",                0172427, 0xC080, 0x0000);
    FLTF("LDF", "#-2.9387358770E-39, AC0", 0172427, 0x8080, 0x0000);
    FLTF("LDF", "#-0.0, AC0",              0172427, 0x8000, 0x0000);

    FLTD("MULD", "#-2, AC2",  0171227, 0xC100, 0x0000, 0x0000, 0x0000);
    FLTF("MULF", "#-2, AC2",  0171227, 0xC100, 0x0000);
    FLTD("CMPD", "#0.5, AC2", 0173627, 0x4000, 0x0000, 0x0000, 0x0000);
    FLTF("CMPF", "#0.5, AC2", 0173627, 0x4000, 0x0000);
}

static void test_undef() {
    ERUS("MOV %UNDEF, %1",       "UNDEF, %1", 0010001);
    ERUS("MOV %2-1, (%UNDEF+1)", "UNDEF+1)",  0010110);
    ERUS("CMP R2, #UNDEF",       "UNDEF",     0020227, 000000);
    ERUS("MOV R3, @#UNDEF",      "UNDEF",     0010337, 000000);
    ERUS("MOV SP, UNDEF",        "UNDEF",     0010667, 000000);
    ERUS("MOV PC, @UNDEF(R0)",   "UNDEF(R0)", 0010770, 000000);
    ERUS("MOV PC, @UNDEF",       "UNDEF",     0010777, 000000);

    ERUS("MOV #UNDEF+1, R2",     "UNDEF+1, R2",   0012702, 000001);
    ERUS("MOV @#UNDEF, R3",      "UNDEF, R3",     0013703, 000000);
    ERUS("MOV UNDEF, SP",        "UNDEF, SP",     0016706, 000000);
    ERUS("MOV @UNDEF(R0), PC",   "UNDEF(R0), PC", 0017007, 000000);
    ERUS("MOV @UNDEF, PC",       "UNDEF, PC",     0017707, 000000);

    ERUS("MOV %UNDEF-1, %UNDEF+1", "UNDEF-1, %UNDEF+1", 0010000);
    ERUS("CMP #UNDEF+2, #UNDEF-1", "UNDEF+2, #UNDEF-1", 0022727, 0000002, 0177777);
    ERUS("MOV UNDEF, @#UNDEF",     "UNDEF, @#UNDEF",    0016737, 0000000, 0000000);
    ERUS("MOV @UNDEF(R0), @UNDEF", "UNDEF(R0), @UNDEF", 0017077, 0000000, 0000000);

    AERRT(0100000, "BR UNDEF+2", UNDEFINED_SYMBOL, "UNDEF+2", 0000400);

    if (dcj11()) {
        ERUS("LDF #UNDEF, AC0", "UNDEF, AC0", 0172427, 0x0000, 0x0000);
        ERUS("LDD #UNDEF, AC0", "UNDEF, AC0", 0172427, 0x0000, 0x0000, 0x0000, 0x0000);
    }
}

static void test_data_constant() {
    BTEST(".byte   -127., -127, 255, 255.", 0x81, (uint8_t)-0127, 0255, 0xFF);
    BTEST(R"(.byte 'A, '")",     0x41, 0x22);
    BTEST(".byte   '9-'0",       0x09);
    BTEST(R"(.byte '', '")",     0x27, 0x22);
    BTEST(R"(.byte 'A,'",'B,0)", 0x41, 0x22, 0x42, 0x00);
    BTEST(R"(.ascii /A'B"C/)",   0x41, 0x27, 0x42, 0x22, 0x43);
    BTEST(".ascii ;A;/C/",       0x41, 0x43);
    BCOMM(".ascii /A/;C;",       ";C;", 0x41);
    BTEST(".ascii //");
    BTEST(".ascii /ABC/<15><12>/DEF/", 0x41, 0x42, 0x43, 0x0D, 0x0A, 0x44, 0x45, 0x46);
    BTEST(".ascii /A<15>B/",           0x41, 0x3C, 0x31, 0x35, 0x3E, 0x42);
    BERRT(".ascii /;", MISSING_CLOSING_DELIMITER, "/;", 0x3B);
    BERRT(".ascii '/", MISSING_CLOSING_DELIMITER, "'/", 0x2F);
    TEST(".word ^D-128, 255, 255., ^B100",  (uint16_t)-128, 0255, 255, 0b100);
    TEST(".word 'X",           0x0058);
    TEST(".word 'X+1",         0x0059);
    TEST(".word ^C151, ^C^D25, ^C2+6, <^C2>+6",
         0177626, 0177746, 000003, 000003);
#if defined(LIBASM_ASM_NOFLOAT)
    ERRT(".word ^F1.0, ^F-1.0, -^F1.0, -^F-1.0", FLOAT_NOT_SUPPORTED,
         "^F1.0, ^F-1.0, -^F1.0, -^F-1.0");
#else
    TEST(".word ^F1.0, ^F-1.0, -^F1.0, -^F-1.0",
         0040200, 0140200, 0137600, 0037600);
#endif
    TEST(R"(.word "AB, 'C, "DE)",
         0x4241, 0x0043, 0x4544);
    BERRT(".byte 1, UNDEF, 2", UNDEFINED_SYMBOL, "UNDEF, 2",
          0x01, 0x00, 0x02);
    ERUS(".word 1, UNDEF, 2", "UNDEF, 2",
         0x0001, 0x0000, 0x0002);

    ERRT(".ascii "
         ";1234567890;" "/1234567890/" "!1234567890!" "|1234567890|" "?1234567890?"
         "'1234567890'" "/1234567890/",
         NO_MEMORY,
         "567890/",
         0x3231, 0x3433, 0x3635, 0x3837, 0x3039,
         0x3231, 0x3433, 0x3635, 0x3837, 0x3039,
         0x3231, 0x3433, 0x3635, 0x3837, 0x3039,
         0x3231, 0x3433, 0x3635, 0x3837, 0x3039,
         0x3231, 0x3433, 0x3635, 0x3837, 0x3039,
         0x3231, 0x3433, 0x3635, 0x3837, 0x3039,
         0x3231, 0x3433);

    ERRT(".word "
         "012345, 076543, 021012, 034567, 012345, 076543, 021012, 034567,"
         "012345, 076543, 021012, 034567, 012345, 076543, 021012, 034567,"
         "012345, 076543, 021012, 034567, 012345, 076543, 021012, 034567,"
         "012345, 076543, 021012, 034567, 012345, 076543, 021012, 034567,"
         "012345",
         NO_MEMORY,
         "012345",
         012345, 076543, 021012, 034567, 012345, 076543, 021012, 034567,
         012345, 076543, 021012, 034567, 012345, 076543, 021012, 034567,
         012345, 076543, 021012, 034567, 012345, 076543, 021012, 034567,
         012345, 076543, 021012, 034567, 012345, 076543, 021012, 034567);

#if defined(LIBASM_ASM_NOFLOAT)
    ERRT(".flt2 1.7014117838E+38", FLOAT_NOT_SUPPORTED,
         "1.7014117838E+38", 0, 0);
    ERRT(".flt4 1.70141183460469229371E+38", FLOAT_NOT_SUPPORTED,
         "1.70141183460469229371E+38", 0, 0, 0, 0);
#else
    TEST(".flt2 0.0",      0x0000, 0x0000); // +0
    TEST(".flt2 -0.0",     0x8000, 0x0000); // -0
    TEST(".flt2 -1.0",     0xC080, 0x0000);
    TEST(".flt2 -2.25e2",  0xC461, 0x0000);
    ERRT(".flt2 inf",      OVERFLOW_RANGE, "inf",  0x0000, 0x0000);
    ERRT(".flt2 -inf",     OVERFLOW_RANGE, "-inf", 0x8000, 0x0000);
    ERRT(".flt2 nan",     NOT_AN_EXPECTED, "nan",  0x0000, 0x0000);
    ERRT(".flt2 -nan",    NOT_AN_EXPECTED, "-nan", 0x8000, 0x0000);
    TEST(".flt2 1.7014117838E+38",   0x7FFF, 0xFFFF); // FLT2_MAX

    ERRT(".flt2 1.7014117839E+38",   OVERFLOW_RANGE,
         "1.7014117839E+38",         0x0000, 0x0000);
    TEST(".flt2 -1.7014117838E+38",  0xFFFF, 0xFFFF); // -FLT2_MAX
    ERRT(".flt2 -1.7014117839E+38",  OVERFLOW_RANGE,
         "-1.7014117839E+38",        0x8000, 0x0000); // -0
    TEST(".flt2 2.9387357895E-39",   0x0080, 0x0000); // FLT2_MIN
    TEST(".flt2 2.9387357894E-39",   0x0000, 0x0000); // +0
    TEST(".flt2 -2.9387357895E-39",  0x8080, 0x0000); // -FLT2_MIN
    TEST(".flt2 -2.9387357894E-39",  0x8000, 0x0000); // -0

    TEST(".flt4 0.0",      0x0000, 0x0000, 0x0000, 0x0000);
    TEST(".flt4 -0.0",     0x8000, 0x0000, 0x0000, 0x0000);
    TEST(".flt4 -1.0",     0xC080, 0x0000, 0x0000, 0x0000);
    TEST(".flt4 2.25e2",   0x4461, 0x0000, 0x0000, 0x0000);
    ERRT(".flt4 inf",  OVERFLOW_RANGE, "inf",   0x0000, 0x0000, 0x0000, 0x0000);
    ERRT(".flt4 -inf", OVERFLOW_RANGE, "-inf",  0x8000, 0x0000, 0x0000, 0x0000);
    ERRT(".flt4 nan",  NOT_AN_EXPECTED, "nan",  0x0000, 0x0000, 0x0000, 0x0000);
    ERRT(".flt4 -nan", NOT_AN_EXPECTED, "-nan", 0x8000, 0x0000, 0x0000, 0x0000);
    TEST(".flt4 1.70141183460469229371E+38",   0x7FFF, 0xFFFF, 0xFFFF, 0xFFFF); // FLT4_MAX
    ERRT(".flt4 1.70141183460469230547E+38",   OVERFLOW_RANGE,
         "1.70141183460469230547E+38",         0x0000, 0x0000, 0x0000, 0x0000); // FLT4_MAX
    TEST(".flt4 -1.70141183460469229371E+38",  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF); // -FLT4_MAX
    ERRT(".flt4 -1.70141183460469230547E+38",  OVERFLOW_RANGE,
         "-1.70141183460469230547E+38",        0x8000, 0x0000, 0x0000, 0x0000); // FLT4_MAX
    TEST(".flt4 2.93873587705571876992E-39",   0x0080, 0x0000, 0x0000, 0x0000); // FLT4_MIN
    TEST(".flt4 2.93873587705571874945E-39",   0x0000, 0x0000, 0x0000, 0x0000); // +0
    TEST(".flt4 -2.93873587705571876992E-39",  0x8080, 0x0000, 0x0000, 0x0000); // -FLT4_MIN
    TEST(".flt4 -2.93873587705571874945E-39",  0x8000, 0x0000, 0x0000, 0x0000); // -0

    ERUS(".flt2 1, UNDEF, 2", "UNDEF, 2",
         0x4080, 0x0000, 0x0000, 0x0000, 0x4100, 0x0000);
    ERUS(".flt4 1, UNDEF, 2", "UNDEF, 2",
         0x4080, 0x0000, 0x0000, 0x0000,
         0x0000, 0x0000, 0x0000, 0x0000,
         0x4100, 0x0000, 0x0000, 0x0000);
#endif

    TEST(R"(option "implicit-word", "on")");
    TEST( "   0", 00000);
    TEST( "  10", 00010);
    TEST( " 7+5", 00014);
    TEST( " 7-5", 00002);
    TEST( " 7&5", 00005);
    TEST( " 2!5", 00007);
    TEST(".radix 10");
    TEST( "  10", 10);
    TEST( " 7+5", 7+5);
    TEST( "17-5", 17-5);
    TEST( "15&5", 15&5);
    TEST( "16!5", 16|5);
    TEST( "4095", 07777);
    ERUS("UNDEF", "UNDEF", 00000);
    ERUS("UNDEF+1", "UNDEF+1", 00001);

    symtab.intern(01234, "unknown_instruction");
    TEST("unknown_instruction", 01234);

}

// clang-format on

void run_tests(const char *cpu) {
    assembler.setCpu(cpu);
    RUN_TEST(test_double_operands);
    RUN_TEST(test_single_operand);
    RUN_TEST(test_program_control);
    RUN_TEST(test_condition_code);
    RUN_TEST(test_miscellaneous);
    if (dcj11())
        RUN_TEST(test_floating_point);
    RUN_TEST(test_undef);
    RUN_TEST(test_data_constant);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

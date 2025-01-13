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

#if defined(LIBASM_DIS_NOFLOAT)
#define TFLT(insn, opr, oct, ...) ERRT(insn, oct, FLOAT_NOT_SUPPORTED, oct, __VA_ARGS__)
#else
#define TFLT(insn, opr, oct, ...) TEST(insn, opr, __VA_ARGS__)
#endif

bool dcj11() {
    return strcmp_P("J11", disassembler.config().cpu_P()) == 0;
}

void set_up() {
    disassembler.reset();
    disassembler.setOption("relative", "true");
}

void tear_down() {
    symtab.reset();
}

// clang-format off
void test_cpu() {
    EQUALS("cpu DCJ11", true,    disassembler.setCpu("DCJ11"));
    EQUALS_P("cpu DCJ11", "J11", disassembler.config().cpu_P());

    EQUALS("cpu DCT11", true,    disassembler.setCpu("dct11"));
    EQUALS_P("cpu DCT11", "T11", disassembler.config().cpu_P());

    EQUALS("cpu J11", true,    disassembler.setCpu("J11"));
    EQUALS_P("cpu J11", "J11", disassembler.config().cpu_P());

    EQUALS("cpu T11", true,    disassembler.setCpu("T11"));
    EQUALS_P("cpu T11", "T11", disassembler.config().cpu_P());
}

void test_double_operands() {
    // Rn
    TEST("MOV", "R0, R1",          0010001);
    TEST("MOV", "R1, (R2)",        0010112);
    TEST("MOV", "R2, (R3)+",       0010223);
    ERRT("MOV", "R2, #012345",     ILLEGAL_OPERAND_MODE,
         "#012345",                0010227, 012345);
    TEST("CMP", "R2, #012345",     0020227, 012345);
    TEST("MOV", "R3, @(R4)+",      0010334);
    ERRT("MOV", "R3, @#012345",    OPERAND_NOT_ALIGNED,
         "@#012345",               0010337, 012345);
    TEST("MOV", "R3, @#012346",    0010337, 012346);
    TEST("MOV", "R4, -(R5)",       0010445);
    TEST("MOV", "R5, @-(SP)",      0010556);
    ERRT("MOV", "SP, .+012345",    OPERAND_NOT_ALIGNED,
         ".+012345",               0010667, 012341);
    TEST("MOV", "SP, .+012346",    0010667, 012342);
    TEST("MOV", "PC, @012345(R0)", 0010770, 012345);
    ERRT("MOV", "PC, @.+012345",   OPERAND_NOT_ALIGNED,
         "@.+012345",              0010777, 012341);
    TEST("MOV", "PC, @.+012346",   0010777, 012342);

    // (Rn)
    TEST("MOV", "(R2), R1",          0011201);
    TEST("MOV", "(R1), (R2)",        0011112);
    TEST("MOV", "(R2), (R3)+",       0011223);
    ERRT("MOV", "(R2), #012345",     ILLEGAL_OPERAND_MODE,
         "#012345",                  0011227, 012345);
    TEST("CMP", "(R2), #012345",     0021227, 012345);
    TEST("MOV", "(R3), @(R4)+",      0011334);
    ERRT("MOV", "(R3), @#012345",    OPERAND_NOT_ALIGNED,
         "@#012345",                 0011337, 012345);
    TEST("MOV", "(R3), @#012346",    0011337, 012346);
    TEST("MOV", "(R4), -(R5)",       0011445);
    TEST("MOV", "(R5), @-(SP)",      0011556);
    ERRT("MOV", "(SP), .+012345",    OPERAND_NOT_ALIGNED,
         ".+012345",                 0011667, 012341);
    TEST("MOV", "(SP), .+012346",    0011667, 012342);
    TEST("MOV", "(PC), @012345(R0)", 0011770, 012345);
    ERRT("MOV", "(PC), @.+012345",   OPERAND_NOT_ALIGNED,
         "@.+012345",                0011777, 012341);
    TEST("MOV", "(PC), @.+012346",   0011777, 012342);

    // (Rn)+
    TEST("MOV", "(R3)+, R2",          0012302);
    TEST("MOV", "(R1)+, (R2)",        0012112);
    TEST("MOV", "(R2)+, (R3)+",       0012223);
    ERRT("MOV", "(R2)+, #012345",     ILLEGAL_OPERAND_MODE,
         "#012345",                   0012227, 012345);
    TEST("CMP", "(R2)+, #012345",     0022227, 012345);
    TEST("MOV", "(R3)+, @(R4)+",      0012334);
    ERRT("MOV", "(R3)+, @#012345",    OPERAND_NOT_ALIGNED,
         "@#012345",                  0012337, 012345);
    TEST("MOV", "(R3)+, @#012346",    0012337, 012346);
    TEST("MOV", "(R4)+, -(R5)",       0012445);
    TEST("MOV", "(R5)+, @-(SP)",      0012556);
    ERRT("MOV", "(SP)+, .+012345",    OPERAND_NOT_ALIGNED,
         ".+012345",                  0012667, 012341);
    TEST("MOV", "(SP)+, .+012346",    0012667, 012342);

    // #imm
    TEST("MOV", "#034567, R2",          0012702, 034567);
    TEST("MOV", "#034567, (R2)",        0012712, 034567);
    TEST("MOV", "#034567, (R3)+",       0012723, 034567);
    ERRT("MOV", "#034567, #012345",     ILLEGAL_OPERAND_MODE,
         "#012345",                     0012727, 034567, 012345);
    TEST("CMP", "#034567, #012345",     0022727, 034567, 012345);
    TEST("MOV", "#034567, @(R4)+",      0012734, 034567);
    ERRT("MOV", "#034567, @#012345",    OPERAND_NOT_ALIGNED,
         "@#012345",                    0012737, 034567, 012345);
    TEST("MOV", "#034567, @#012346",    0012737, 034567, 012346);
    TEST("MOV", "#034567, -(R5)",       0012745, 034567);
    TEST("MOV", "#034567, @-(SP)",      0012756, 034567);
    ERRT("MOV", "#034567, .+012345",    OPERAND_NOT_ALIGNED,
         ".+012345",                    0012767, 034567, 012337);
    TEST("MOV", "#034567, .+012346",    0012767, 034567, 012340);
    TEST("MOV", "#034567, @012345(R0)", 0012770, 034567, 012345);
    ERRT("MOV", "#034567, @.+012345",   OPERAND_NOT_ALIGNED,
         "@.+012345",                   0012777, 034567, 012337);
    TEST("MOV", "#034567, @.+012346",   0012777, 034567, 012340);

    // @(Rn)+
    TEST("MOV", "@(R4)+, R3",          0013403);
    TEST("MOV", "@(R1)+, (R2)",        0013112);
    TEST("MOV", "@(R2)+, (R3)+",       0013223);
    ERRT("MOV", "@(R2)+, #012345",     ILLEGAL_OPERAND_MODE,
         "#012345",                    0013227, 012345);
    TEST("CMP", "@(R2)+, #012345",     0023227, 012345);
    TEST("MOV", "@(R3)+, @(R4)+",      0013334);
    ERRT("MOV", "@(R3)+, @#012345",    OPERAND_NOT_ALIGNED,
         "@#012345",                   0013337, 012345);
    TEST("MOV", "@(R3)+, @#012346",    0013337, 012346);
    TEST("MOV", "@(R4)+, -(R5)",       0013445);
    TEST("MOV", "@(R5)+, @-(SP)",      0013556);
    ERRT("MOV", "@(SP)+, .+012345",    OPERAND_NOT_ALIGNED,
         ".+012345",                   0013667, 012341);
    TEST("MOV", "@(SP)+, .+012346",    0013667, 012342);

    // @#abs
    TEST("MOV", "@#034566, R3",          0013703, 034566);
    ERRT("MOV", "@#034567, R3",          OPERAND_NOT_ALIGNED,
         "@#034567, R3",                 0013703, 034567);
    TEST("MOV", "@#034566, (R2)",        0013712, 034566);
    TEST("MOV", "@#034566, (R3)+",       0013723, 034566);
    ERRT("MOV", "@#034566, #012345",     ILLEGAL_OPERAND_MODE,
         "#012345",                      0013727, 034566, 012345);
    TEST("CMP", "@#034566, #012345",     0023727, 034566, 012345);
    TEST("MOV", "@#034566, @(R4)+",      0013734, 034566);
    TEST("MOV", "@#034566, @#012346",    0013737, 034566, 012346);
    ERRT("MOV", "@#034566, @#012345",    OPERAND_NOT_ALIGNED,
         "@#012345",                     0013737, 034566, 012345);
    ERRT("MOV", "@#034567, @#012346",    OPERAND_NOT_ALIGNED,
         "@#034567, @#012346",           0013737, 034567, 012346);
    TEST("MOV", "@#034566, -(R5)",       0013745, 034566);
    TEST("MOV", "@#034566, @-(SP)",      0013756, 034566);
    ERRT("MOV", "@#034566, .+012345",    OPERAND_NOT_ALIGNED,
         ".+012345",                     0013767, 034566, 012337);
    TEST("MOV", "@#034566, .+012346",    0013767, 034566, 012340);
    TEST("MOV", "@#034566, @012345(R0)", 0013770, 034566, 012345);
    ERRT("MOV", "@#034566, @.+012345",   OPERAND_NOT_ALIGNED,
         "@.+012345",                    0013777, 034566, 012337);
    TEST("MOV", "@#034566, @.+012346",   0013777, 034566, 012340);

    // -(Rn)
    TEST("MOV", "-(R5), R4",          0014504);
    TEST("MOV", "-(R1), (R2)",        0014112);
    TEST("MOV", "-(R2), (R3)+",       0014223);
    ERRT("MOV", "-(R2), #012345",     ILLEGAL_OPERAND_MODE,
         "#012345",                   0014227, 012345);
    TEST("MOV", "-(R3), @(R4)+",      0014334);
    ERRT("MOV", "-(R3), @#012345",    OPERAND_NOT_ALIGNED,
         "@#012345",                  0014337, 012345);
    TEST("MOV", "-(R3), @#012346",    0014337, 012346);
    TEST("MOV", "-(R4), -(R5)",       0014445);
    TEST("MOV", "-(R5), @-(SP)",      0014556);
    ERRT("MOV", "-(SP), .+012345",    OPERAND_NOT_ALIGNED,
         ".+012345",                  0014667, 012341);
    TEST("MOV", "-(SP), .+012346",    0014667, 012342);
    ERRT("MOV", "-(PC), @012345(R0)", NOT_AN_EXPECTED,
         "-(PC), @012345(R0)",        0014770, 012345);
    ERRT("MOV", "-(PC), @.+012345",   NOT_AN_EXPECTED,
         "-(PC), @.+012345",          0014777, 012341);
    ERRT("MOV", "-(PC), @.+012346",   NOT_AN_EXPECTED,
         "-(PC), @.+012346",          0014777, 012342);

    // @-(Rn)
    TEST("MOV", "@-(SP), R5",          0015605);
    TEST("MOV", "@-(R1), (R2)",        0015112);
    TEST("MOV", "@-(R2), (R3)+",       0015223);
    ERRT("MOV", "@-(R2), #012345",     ILLEGAL_OPERAND_MODE,
         "#012345",                    0015227, 012345);
    TEST("CMP", "-(R2), #012345",      0024227, 012345);
    TEST("MOV", "@-(R3), @(R4)+",      0015334);
    ERRT("MOV", "@-(R3), @#012345",    OPERAND_NOT_ALIGNED,
         "@#012345",                   0015337, 012345);
    TEST("MOV", "@-(R3), @#012346",    0015337, 012346);
    TEST("MOV", "@-(R4), -(R5)",       0015445);
    TEST("MOV", "@-(R5), @-(SP)",      0015556);
    ERRT("MOV", "@-(SP), .+012345",    OPERAND_NOT_ALIGNED,
         ".+012345",                   0015667, 012341);
    TEST("MOV", "@-(SP), .+012346",    0015667, 012342);
    ERRT("MOV", "@-(PC), @012345(R0)", NOT_AN_EXPECTED,
         "@-(PC), @012345(R0)",        0015770, 012345);
    ERRT("MOV", "@-(PC), @.+012345",   NOT_AN_EXPECTED,
         "@-(PC), @.+012345",          0015777, 012341);
    ERRT("MOV", "@-(PC), @.+012346",   NOT_AN_EXPECTED,
         "@-(PC), @.+012346",          0015777, 012342);

    // idx(Rn)
    TEST("MOV", "023456(SP), R5",          0016605, 023456);
    TEST("MOV", "023456(R1), (R2)",        0016112, 023456);
    TEST("MOV", "023456(R2), (R3)+",       0016223, 023456);
    ERRT("MOV", "023456(R2), #012345",     ILLEGAL_OPERAND_MODE,
         "#012345",                        0016227, 023456, 012345);
    TEST("CMP", "023456(R2), #012345",     0026227, 023456, 012345);
    TEST("MOV", "023456(R3), @(R4)+",      0016334, 023456);
    ERRT("MOV", "023456(R3), @#012345",    OPERAND_NOT_ALIGNED,
         "@#012345",                       0016337, 023456, 012345);
    TEST("MOV", "023456(R3), @#012346",    0016337, 023456, 012346);
    TEST("MOV", "023456(R4), -(R5)",       0016445, 023456);
    TEST("MOV", "023456(R5), @-(SP)",      0016556, 023456);
    ERRT("MOV", "023456(SP), .+012345",    OPERAND_NOT_ALIGNED,
         ".+012345",                       0016667, 023456, 012337);
    TEST("MOV", "023456(SP), .+012346",    0016667, 023456, 012340);
    TEST("MOV", "023456(R1), @012345(R0)", 0016170, 023456, 012345);
    ERRT("MOV", "023456(R1), @.+012345",   OPERAND_NOT_ALIGNED,
         "@.+012345",                      0016177, 023456, 012337);
    TEST("MOV", "023456(R1), @.+012346",   0016177, 023456, 012340);
    TEST("MOV", ".+023456, @012345(R0)",   0016770, 023452, 012345);
    ERRT("MOV", ".+023455, @.+012346",     OPERAND_NOT_ALIGNED,
         ".+023455, @.+012346",            0016777, 023451, 012340);
    TEST("MOV", ".+023456, @.+012346",     0016777, 023452, 012340);

    // @idx(Rn)
    TEST("MOV", "@023456(SP), R5",          0017605, 023456);
    TEST("MOV", "@023456(R1), (R2)",        0017112, 023456);
    TEST("MOV", "@023456(R2), (R3)+",       0017223, 023456);
    ERRT("MOV", "@023456(R2), #012345",     ILLEGAL_OPERAND_MODE,
         "#012345",                         0017227, 023456, 012345);
    TEST("CMP", "@023456(R2), #012345",     0027227, 023456, 012345);
    TEST("MOV", "@023456(R3), @(R4)+",      0017334, 023456);
    ERRT("MOV", "@023456(R3), @#012345",    OPERAND_NOT_ALIGNED,
         "@#012345",                        0017337, 023456, 012345);
    TEST("MOV", "@023456(R3), @#012346",    0017337, 023456, 012346);
    TEST("MOV", "@023456(R4), -(R5)",       0017445, 023456);
    TEST("MOV", "@023456(R5), @-(SP)",      0017556, 023456);
    ERRT("MOV", "@023456(SP), .+012345",    OPERAND_NOT_ALIGNED,
         ".+012345",                        0017667, 023456, 012337);
    TEST("MOV", "@023456(SP), .+012346",    0017667, 023456, 012340);
    TEST("MOV", "@023456(R1), @012345(R0)", 0017170, 023456, 012345);
    ERRT("MOV", "@023456(R1), @.+012345",   OPERAND_NOT_ALIGNED,
         "@.+012345",                       0017177, 023456, 012337);
    TEST("MOV", "@023456(R1), @.+012346",   0017177, 023456, 012340);
    TEST("MOV", "@.+023456, @012345(R0)",   0017770, 023452, 012345);
    ERRT("MOV", "@.+023455, @.+012346",     OPERAND_NOT_ALIGNED,
         "@.+023455, @.+012346",            0017777, 023451, 012340);
    TEST("MOV", "@.+023456, @.+012346",     0017777, 023452, 012340);

    TEST("MOVB", "R0, R1",          0110001);
    TEST("MOVB", "R1, (R2)",        0110112);
    TEST("MOVB", "R2, (R3)+",       0110223);
    ERRT("MOVB", "R2, #377",        ILLEGAL_OPERAND_MODE,
         "#377",                    0110227, 000377);
    TEST("CMPB", "R2, #377",        0120227, 000377);
    TEST("MOVB", "R3, @(R4)+",      0110334);
    TEST("MOVB", "R3, @#012345",    0110337, 012345);
    TEST("MOVB", "R4, -(R5)",       0110445);
    TEST("MOVB", "R5, @-(SP)",      0110556);
    TEST("MOVB", "SP, .+012345",    0110667, 012341);
    TEST("MOVB", "PC, @012345(R0)", 0110770, 012345);
    ERRT("MOVB", "PC, @.+012345",   OPERAND_NOT_ALIGNED,
         "@.+012345",               0110777, 012341);
    TEST("MOVB", "PC, @.+012346",   0110777, 012342);
    TEST("MOVB", "R1, R0",          0110100);
    TEST("MOVB", "(R2), R1",        0111201);
    TEST("MOVB", "(R3)+, R2",       0112302);
    TEST("MOVB", "#377, R2",        0112702, 000377);
    TEST("MOVB", "@(R4)+, R3",      0113403);
    TEST("MOVB", "@#012345, R3",    0113703, 012345);
    TEST("MOVB", "-(R5), R4",       0114504);
    TEST("MOVB", "@-(SP), R5",      0115605);
    TEST("MOVB", "034567(R1), R5",  0116105, 034567);
    TEST("MOVB", ".+012345, SP",    0116706, 012341);
    TEST("MOVB", "@012345(R0), PC", 0117007, 012345);
    ERRT("MOVB", "@.+012345, PC",   OPERAND_NOT_ALIGNED,
         "@.+012345, PC",           0117707, 012341);
    TEST("MOVB", "@.+012346, PC",   0117707, 012342);

    TEST("CMP", "R0, R1",          020001);
    TEST("BIT", "R1, (R2)",        0030112);
    TEST("BIC", "R2, (R3)+",       0040223);
    TEST("CMP", "R3, #012345",     0020327, 012345);
    TEST("BIS", "R3, @(R4)+",      0050334);
    TEST("ADD", "R4, @#023456",    0060437, 023456);
    TEST("SUB", "R4, -(R5)",       0160445);
    TEST("CMP", "R5, @-(SP)",      0020556);
    TEST("BIT", "SP, 012345(R0)",  0030660, 012345);
    TEST("BIC", "PC, .+023456",    0040767, 023452);
    TEST("BIS", "R0, @012345(R1)", 0050071, 012345);
    TEST("ADD", "R1, @.+023456",   0060177, 023452);
    TEST("SUB", "(R2), R4",        0161204);
    TEST("CMP", "(R3)+, R5",       0022305);
    TEST("BIT", "#012345, SP",     0032706, 012345);
    TEST("BIC", "@(R4)+, PC",      0043407);
    TEST("BIS", "@#023456, R0",    0053700, 023456);
    TEST("ADD", "-(R5), R1",       0064501);
    TEST("SUB", "@-(SP), R2",      0165602);
    TEST("CMP", ".+012346, R3",    0026703, 012342);
    ERRT("CMP", ".+012345, R3",    OPERAND_NOT_ALIGNED,
         ".+012345, R3",           0026703, 012341);
    TEST("BIT", "@012345(R0), R4", 0037004, 012345);
    TEST("BIT", "@.+012346, R4",   0037704, 012342);
    ERRT("BIT", "@.+012345, R4",   OPERAND_NOT_ALIGNED,
         "@.+012345, R4",          0037704, 012341);

    TEST("CMPB", "R0, R1",          0120001);
    TEST("BITB", "R1, (R2)",        0130112);
    TEST("BICB", "R2, (R3)+",       0140223);
    TEST("CMPB", "R3, #345",        0120327, 000345);
    TEST("BISB", "R3, @(R4)+",      0150334);
    TEST("BISB", "R4, @#023456",    0150437, 023456);
    TEST("BISB", "R4, -(R5)",       0150445);
    TEST("CMPB", "R5, @-(SP)",      0120556);
    TEST("BITB", "SP, 012345(R0)",  0130660, 012345);
    TEST("BICB", "PC, .+023456",    0140767, 023452);
    TEST("BISB", "R0, @012345(R1)", 0150071, 012345);
    TEST("BISB", "R1, @.+023456",   0150177, 023452);
    TEST("BISB", "(R2), R4",        0151204);
    TEST("CMPB", "(R3)+, R5",       0122305);
    TEST("BITB", "#345, SP",        0132706, 000345);
    TEST("BICB", "@(R4)+, PC",      0143407);
    TEST("BISB", "@#023456, R0",    0153700, 023456);
    TEST("BISB", "-(R5), R1",       0154501);
    TEST("BISB", "@-(SP), R2",      0155602);
    TEST("CMPB", ".+012346, R3",    0126703, 012342);
    TEST("CMPB", ".+012345, R3",    0126703, 012341);
    TEST("BITB", "@012345(R0), R4", 0137004, 012345);
    TEST("BITB", "@.+012346, R4",   0137704, 012342);
    ERRT("BITB", "@.+012345, R4",   OPERAND_NOT_ALIGNED,
         "@.+012345, R4"  ,         0137704, 012341);

    TEST("XOR", "R2, R1",          074201);
    TEST("XOR", "R2, (R1)",        074211);
    TEST("XOR", "R2, (R1)+",       074221);
    ERRT("XOR", "R2, #012345",     ILLEGAL_OPERAND_MODE,
         "#012345",                074227, 012345);
    TEST("XOR", "R2, @(R1)+",      074231);
    ERRT("XOR", "R2, @#012345",    OPERAND_NOT_ALIGNED,
         "@#012345",               074237, 012345);
    TEST("XOR", "R2, @#012346",    074237, 012346);
    TEST("XOR", "R2, -(R1)",       074241);
    TEST("XOR", "R2, @-(R1)",      074251);
    ERRT("XOR", "R2, -(PC)",       NOT_AN_EXPECTED,
         "-(PC)",                  074247);
    ERRT("XOR", "R2, @-(PC)",      NOT_AN_EXPECTED,
         "@-(PC)",                 074257);
    TEST("XOR", "R2, 012345(R1)",  074261, 012345);
    ERRT("XOR", "R2, .+012345",    OPERAND_NOT_ALIGNED,
         ".+012345",               074267, 012341);
    TEST("XOR", "R2, .+012346",    074267, 012342);
    TEST("XOR", "R2, @012345(R1)", 074271, 012345);
    ERRT("XOR", "R2, @.+012345",   OPERAND_NOT_ALIGNED,
         "@.+012345",              074277, 012341);
    TEST("XOR", "R2, @.+012346",   074277, 012342);

    if (dcj11()) {
        TEST("MUL", "R1, R2",      070201);
        TEST("DIV", "#012345, R3", 071327, 012345);
        TEST("ASH", "(R5), R4",    072415);
        TEST("ASHC", "@#012346, SP",  073637, 012346);
    }

    disassembler.setOption("relative", "off");
    ATEST(0000100, "MOV", "000104, 000106", 0016767, 0000000, 0000000);
    ATEST(0000100, "MOV", "000100, 000100", 0016767, 0177774, 0177772);
    ATEST(0000100, "MOV", "000000, 000000", 0016767, 0177674, 0177672);
    AERRT(0000100, "MOV", "177776, 000000",
          OVERFLOW_RANGE, "177776, 000000", 0016767, 0177672, 0177672);
    AERRT(0000100, "MOV", "000000, 177776",
          OVERFLOW_RANGE,         "177776", 0016767, 0177674, 0177670);
    AERRT(0000100, "MOV", "177776, 177776",
          OVERFLOW_RANGE, "177776, 177776", 0016767, 0177672, 0177670);
    ATEST(0177700, "MOV", "177776, 177776", 0016767, 0000072, 0000070);
    AERRT(0177700, "MOV", "000000, 177776",
          OVERFLOW_RANGE, "000000, 177776", 0016767, 0000074, 0000070);
    AERRT(0177700, "MOV", "177776, 000000",
          OVERFLOW_RANGE,         "000000", 0016767, 0000072, 0000072);
    AERRT(0177700, "MOV", "000000, 000000",
          OVERFLOW_RANGE, "000000, 000000", 0016767, 0000074, 0000072);
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
        TEST("TSTSET", "012345(R5)",     007265, 012345);
        TEST("WRTLCK", "@012345(SP)",    007376, 012345);
        ERRT("TSTSET", "R5",
             REGISTER_NOT_ALLOWED, "R5", 007205);
        ERRT("WRTLCK", "SP",
             REGISTER_NOT_ALLOWED, "SP", 007306);
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

    disassembler.setOption("relative", "off");
    ATEST(0000100, "TST", "000104", 0005767, 0000000);
    ATEST(0000100, "TST", "000100", 0005767, 0177774);
    ATEST(0000100, "TST", "000000", 0005767, 0177674);
    AERRT(0000100, "TST", "177776",
          OVERFLOW_RANGE, "177776", 0005767, 0177672);
    ATEST(0177700, "TST", "177776", 0005767, 0000072);
    AERRT(0177700, "TST", "000000",
          OVERFLOW_RANGE, "000000", 0005767, 0000074);
}

void test_program_control() {
    TEST("BPT", "", 000003);
    TEST("IOT", "", 000004);
    TEST("EMT",  "377", 0104377);
    TEST("TRAP", "377", 0104777);
    TEST("RTS", "R2",   000202);

    ERRT("JMP", "R3",          REGISTER_NOT_ALLOWED,
         "R3",                 000103);
    TEST("JMP", "(R3)",        000113);
    TEST("JMP", "(R3)+",       000123);
    ERRT("JMP", "#012345",     ILLEGAL_OPERAND_MODE,
         "#012345",            000127, 012345);
    TEST("JMP", "@(R3)+",      000133);
    ERRT("JMP", "@#012345",    OPERAND_NOT_ALIGNED,
         "@#012345",           000137, 012345);
    TEST("JMP", "@#012346",    000137, 012346);
    TEST("JMP", "-(R3)",       000143);
    ERRT("JMP", "-(PC)",       NOT_AN_EXPECTED,
         "-(PC)",              000147);
    TEST("JMP", "@-(R3)",      000153);
    ERRT("JMP", "@-(PC)",      NOT_AN_EXPECTED,
         "@-(PC)",             000157);
    TEST("JMP", "012345(R3)",  000163, 012345);
    ERRT("JMP", ".+012345",    OPERAND_NOT_ALIGNED,
         ".+012345",           000167, 012341);
    TEST("JMP", ".+012346",    000167, 012342);
    TEST("JMP", "@012345(R3)", 000173, 012345);
    ERRT("JMP", "@.+012345",   OPERAND_NOT_ALIGNED,
         "@.+012345",          000177, 012341);
    TEST("JMP", "@.+012346",   000177, 012342);

    ERRT("JSR", "R2, R3",          REGISTER_NOT_ALLOWED,
         "R3",                     004203);
    TEST("JSR", "R2, (R3)",        004213);
    TEST("JSR", "R2, (R3)+",       004223);
    ERRT("JSR", "R2, #012345",     ILLEGAL_OPERAND_MODE,
         "#012345",                004227, 012345);
    TEST("JSR", "R2, @(R3)+",      004233);
    ERRT("JSR", "R2, @#012345",    OPERAND_NOT_ALIGNED,
         "@#012345",               004237, 012345);
    TEST("JSR", "R2, @#012346",    004237, 012346);
    TEST("JSR", "R2, -(R3)",       004243);
    ERRT("JSR", "R2, -(PC)",       NOT_AN_EXPECTED,
         "-(PC)",                  004247);
    TEST("JSR", "R2, @-(R3)",      004253);
    ERRT("JSR", "R2, @-(PC)",      NOT_AN_EXPECTED,
         "@-(PC)",                 004257);
    TEST("JSR", "R2, 012345(R3)",  004263, 012345);
    ERRT("JSR", "R2, .+012345",    OPERAND_NOT_ALIGNED,
         ".+012345",               004267, 012341);
    TEST("JSR", "R2, .+012346",    004267, 012342);
    TEST("JSR", "R2, @012345(R3)", 004273, 012345);
    ERRT("JSR", "R2, @.+012345",   OPERAND_NOT_ALIGNED,
         "@.+012345",              004277, 012341);
    TEST("JSR", "R2, @.+012346",   004277, 012342);

    if (dcj11()) {
        TEST("SPL",  "3",  000233);
        TEST("MARK", "04", 006404);

        TEST("CSM", "R3",          007003);
        TEST("CSM", "(R3)",        007013);
        TEST("CSM", "(R3)+",       007023);
        TEST("CSM", "#012345",     007027, 012345);
        TEST("CSM", "@(R3)+",      007033);
        ERRT("CSM", "@#012345",    OPERAND_NOT_ALIGNED,
             "@#012345",           007037, 012345);
        TEST("CSM", "@#012346",    007037, 012346);
        TEST("CSM", "-(R3)",       007043);
        ERRT("CSM", "-(PC)",       NOT_AN_EXPECTED,
             "-(PC)",              007047);
        TEST("CSM", "@-(R3)",      007053);
        ERRT("CSM", "@-(PC)",      NOT_AN_EXPECTED,
             "@-(PC)",             007057);
        TEST("CSM", "012345(R3)",  007063, 012345);
        ERRT("CSM", ".+012345",    OPERAND_NOT_ALIGNED,
             ".+012345",           007067, 012341);
        TEST("CSM", ".+012346",    007067, 012342);
        TEST("CSM", "@012345(R3)", 007073, 012345);
        ERRT("CSM", "@.+012345",   OPERAND_NOT_ALIGNED,
             "@.+012345",          007077, 012341);
        TEST("CSM", "@.+012346",   007077, 012342);
    }

    ATEST(0001000, "SOB", "R2, .-174", 0077277);
    ATEST(0001000, "SOB", "R2, .+002", 0077200);
    AERRT(0000010, "SOB", "R2, .-174",
          OVERFLOW_RANGE, ".-174",     0077277);

    ATEST(0001000, "BR",   ".-376", 0000600);
    ATEST(0001000, "BNE",  ".",     0001377);
    ATEST(0001000, "BEQ",  ".+400", 0001577);
    ATEST(0001000, "BGE",  ".+002", 0002000);
    ATEST(0001000, "BLT",  ".-376", 0002600);
    ATEST(0001000, "BGT",  ".",     0003377);
    ATEST(0001000, "BLE",  ".+400", 0003577);
    ATEST(0001000, "BLT",  ".+002", 0002400);
    ATEST(0001000, "BPL",  ".-376", 0100200);
    ATEST(0001000, "BMI",  ".+002", 0100400);
    ATEST(0001000, "BHI",  ".",     0101377);
    ATEST(0001000, "BLOS", ".+400", 0101577);
    ATEST(0001000, "BVC",  ".+002", 0102000);
    ATEST(0001000, "BVS",  ".-376", 0102600);
    ATEST(0001000, "BCC",  ".",     0103377);
    ATEST(0001000, "BCS",  ".+400", 0103577);

    ATEST(0000100, "BCS",  ".-100", 0103737);
    AERRT(0000100, "BCS",  ".-102",
          OVERFLOW_RANGE,  ".-102", 0103736);
    ATEST(0177700, "BCS",  ".+076", 0103436);
    AERRT(0177700, "BCS",  ".+100",
          OVERFLOW_RANGE,  ".+100", 0103437);
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
    TEST("MTPS",  "R1",      0106401);
    TEST("MTPS",  "#377",    0106427, 000377);
    ERRT("MTPS",  "#000400", OVERFLOW_RANGE,
         "000400",           0106427, 000400);
    TEST("MFPS",  "R1",      0106701);

    if (dcj11()) {
        TEST("MFPI", "(R1)+",  0006521);
        TEST("MTPI", "-(R2)",  0006642);
        TEST("MFPD", "@(R3)+", 0106533);
        TEST("MTPD", "@-(R4)", 0106654);
        ERRT("MFPI", "R1",     REGISTER_NOT_ALLOWED,
             "R1",             0006501);
        ERRT("MTPI", "R2",     REGISTER_NOT_ALLOWED,
             "R2",             0006602);
        ERRT("MFPD", "R3",     REGISTER_NOT_ALLOWED,
             "R3",             0106503);
        ERRT("MTPD", "R4",     REGISTER_NOT_ALLOWED,
             "R4",             0106604);
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
    TEST("MODD",  "(R1), AC3",   0171711);
    TEST("ADDD",  "(R2)+, AC1",  0172122);
    TEST("LDD",   "-(R3), AC0",  0172443);
    TEST("SUBD",  "@(R4)+, AC3", 0173334);
    TEST("STD",   "AC1, -(SP)",  0174146);
    ERRT("STD",   "AC1, R1",     REGISTER_NOT_ALLOWED,
         "R1",                   0174101);
    TEST("DIVD",  "(SP)+, AC0",  0174426);
    TEST("STEXP", "AC3, (R1)",   0175311);
    TEST("STCDL", "AC2, -(SP)",  0175646);
    TEST("STCDL", "AC2, R4" ,    0175604);
    TEST("STCDF", "AC1, (R2)+",  0176122);
    TEST("STCDF", "AC1, AC5",    0176105);
    TEST("LDEXP", "(R3), AC0",    0176413);
    TEST("LDEXP", "#000020, AC3", 0176727, 000020);
    TEST("LDCLD", "(SP)+, AC1",        0177126);
    TEST("LDCLD", "#02471267654, AC2", 0177227, 012345, 067654);
    TEST("LDCDF", "(R4)+, AC2",        0177624);

    TFLT("LDCDF",
         "#-2.5, AC1",
         "#1404400000000000000000, AC1",
         0177527, 0xC120, 0x0000, 0x0000, 0x0000);
    TFLT("LDD",
         "#1.70141183460469229371E+38, AC0",
         "#0777777777777777777777, AC0",
         0172427, 0x7FFF, 0xFFFF, 0xFFFF, 0xFFFF);
    TFLT("LDD",
         "#1, AC0",
         "#0402000000000000000000, AC0",
         0172427, 0x4080, 0x0000, 0x0000, 0x0000);
    TFLT("LDD",
         "#2.93873587705571876992E-39, AC0",
         "#0002000000000000000000, AC0",
         0172427, 0x0080, 0x0000, 0x0000, 0x0000);
    TFLT("LDD",
         "#0, AC0",
         "#0000000000000000000000, AC0",
         0172427, 0x0000, 0x0000, 0x0000, 0x0000);
    TFLT("LDD",
         "#-1.70141183460469229371E+38, AC0",
         "#1777777777777777777777, AC0",
         0172427, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF);
    TFLT("LDD",
         "#-1, AC0",
         "#1402000000000000000000, AC0",
         0172427, 0xC080, 0x0000, 0x0000, 0x0000);
    TFLT("LDD",
         "#-2.93873587705571876992E-39, AC0",
         "#1002000000000000000000, AC0",
         0172427, 0x8080, 0x0000, 0x0000, 0x0000);
    TFLT("LDD",
         "#-0, AC0",
         "#1000000000000000000000, AC0",
         0172427, 0x8000, 0x0000, 0x0000, 0x0000);
    TFLT("MULD",
         "#-2, AC2",
         "#1404000000000000000000, AC2",
         0171227, 0xC100, 0x0000, 0x0000, 0x0000);
    TFLT("CMPD",
         "#0.5, AC2",
         "#0400000000000000000000, AC2",
         0173627, 0x4000, 0x0000, 0x0000, 0x0000);
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

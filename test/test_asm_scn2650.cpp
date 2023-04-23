/*
 * Copyright 2022 Tadashi G. Takaoka
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

#include "asm_scn2650.h"
#include "test_asm_helper.h"

using namespace libasm;
using namespace libasm::scn2650;
using namespace libasm::test;

AsmScn2650 asm2650;
Assembler &assembler(asm2650);

static void set_up() {
    assembler.reset();
}

static void tear_down() {
    symtab.reset();
}

// clang-format off
void test_cpu() {
    EQUALS("cpu 2650", true,   assembler.setCpu("2650"));
    EQUALS_P("cpu 2650", "2650", assembler.cpu_P());

    EQUALS("cpu scn2650", true,   assembler.setCpu("scn2650"));
    EQUALS_P("cpu scn2650", "2650", assembler.cpu_P());
}

static void test_load_store() {
    TEST("lodz,r0",             0x60);
    TEST("LODZ,R1",             0x01);
    TEST("LODZ,R2",             0x02);
    TEST("LODZ,R3",             0x03);
    TEST("LODI,R0 H'56'",         0x04, 0x56);
    TEST("LODI,R1 <H'4567'",      0x05, 0x67);
    TEST("LODI,R2 >H'789A'",      0x06, 0x78);
    TEST("LODI,R3 H'89'",         0x07, 0x89);
    TEST("LODR,R0 *$+H'0B'",      0x08, 0x89);
    TEST("LODR,R1  $+H'0C'",      0x09, 0x0A);
    TEST("LODR,R2  $+H'0D'",      0x0A, 0x0B);
    TEST("LODR,R3  $+H'0E'",      0x0B, 0x0C);
    TEST("LODA,R0  H'1123'",      0x0C, 0x11, 0x23);
    TEST("LODA,R0  H'0345',R0,+", 0x0C, 0x23, 0x45);
    TEST("LODA,R0  H'0567',R0,-", 0x0C, 0x45, 0x67);
    TEST("LODA,R0  H'0789',R0",   0x0C, 0x67, 0x89);
    TEST("LODA,R0 *H'19AB'",      0x0C, 0x99, 0xAB);
    TEST("LODA,R0 *H'0BCD',R0,+", 0x0C, 0xAB, 0xCD);
    TEST("LODA,R0 *H'0DEF',R0,-", 0x0C, 0xCD, 0xEF);
    TEST("LODA,R0 *H'0F01',R0",   0x0C, 0xEF, 0x01);
    TEST("LODA,R1  H'0123'",      0x0D, 0x01, 0x23);
    TEST("LODA,R0  H'1345',R1,+", 0x0D, 0x33, 0x45);
    TEST("LODA,R0  H'0567',R1,-", 0x0D, 0x45, 0x67);
    TEST("LODA,R0  H'0789',R1",   0x0D, 0x67, 0x89);
    TEST("LODA,R1 *H'09AB'",      0x0D, 0x89, 0xAB);
    TEST("LODA,R0 *H'1BCD',R1,+", 0x0D, 0xBB, 0xCD);
    TEST("LODA,R0 *H'0DEF',R1,-", 0x0D, 0xCD, 0xEF);
    TEST("LODA,R0 *H'0F01',R1",   0x0D, 0xEF, 0x01);
    TEST("LODA,R2  H'0123'",      0x0E, 0x01, 0x23);
    TEST("LODA,R0  H'0345',R2,+", 0x0E, 0x23, 0x45);
    TEST("LODA,R0  H'1567',R2,-", 0x0E, 0x55, 0x67);
    TEST("LODA,R0  H'0789',R2",   0x0E, 0x67, 0x89);
    TEST("LODA,R2 *H'09AB'",      0x0E, 0x89, 0xAB);
    TEST("LODA,R0 *H'0BCD',R2,+", 0x0E, 0xAB, 0xCD);
    TEST("LODA,R0 *H'1DEF',R2,-", 0x0E, 0xDD, 0xEF);
    TEST("LODA,R0 *H'0F01',R2",   0x0E, 0xEF, 0x01);
    TEST("LODA,R3  H'0123'",      0x0F, 0x01, 0x23);
    TEST("LODA,R0  H'0345',R3,+", 0x0F, 0x23, 0x45);
    TEST("LODA,R0  H'0567',R3,-", 0x0F, 0x45, 0x67);
    TEST("LODA,R0  H'1789',R3",   0x0F, 0x77, 0x89);
    TEST("LODA,R3 *H'09AB'",      0x0F, 0x89, 0xAB);
    TEST("LODA,R0 *H'0BCD',R3,+", 0x0F, 0xAB, 0xCD);
    TEST("LODA,R0 *H'0DEF',R3,-", 0x0F, 0xCD, 0xEF);
    TEST("LODA,R0 *H'1F01',R3",   0x0F, 0xFF, 0x01);

    ERRT("STRZ,R0",             OPERAND_NOT_ALLOWED, "R0");
    TEST("STRZ,R1",             0xC1);
    TEST("STRZ,R2",             0xC2);
    TEST("STRZ,R3",             0xC3);
    TEST("STRR,R0  $+H'2B'",      0xC8, 0x29);
    TEST("STRR,R1 *$+H'2C'",      0xC9, 0xAA);
    TEST("STRR,R2  $+H'2D'",      0xCA, 0x2B);
    TEST("STRR,R3  $+H'2E'",      0xCB, 0x2C);
    TEST("STRA,R0  H'0123'",      0xCC, 0x01, 0x23);
    TEST("STRA,R0  H'1345',R0,+", 0xCC, 0x33, 0x45);
    TEST("STRA,R0  H'0567',R0,-", 0xCC, 0x45, 0x67);
    TEST("STRA,R0  H'0789',R0",   0xCC, 0x67, 0x89);
    TEST("STRA,R0 *H'09AB'",      0xCC, 0x89, 0xAB);
    TEST("STRA,R0 *H'1BCD',R0,+", 0xCC, 0xBB, 0xCD);
    TEST("STRA,R0 *H'0DEF',R0,-", 0xCC, 0xCD, 0xEF);
    TEST("STRA,R0 *H'0F01',R0",   0xCC, 0xEF, 0x01);
    TEST("STRA,R1  H'0123'",      0xCD, 0x01, 0x23);
    TEST("STRA,R0  H'0345',R1,+", 0xCD, 0x23, 0x45);
    TEST("STRA,R0  H'1567',R1,-", 0xCD, 0x55, 0x67);
    TEST("STRA,R0  H'0789',R1",   0xCD, 0x67, 0x89);
    TEST("STRA,R1 *H'09AB'",      0xCD, 0x89, 0xAB);
    TEST("STRA,R0 *H'0BCD',R1,+", 0xCD, 0xAB, 0xCD);
    TEST("STRA,R0 *H'1DEF',R1,-", 0xCD, 0xDD, 0xEF);
    TEST("STRA,R0 *H'0F01',R1",   0xCD, 0xEF, 0x01);
    TEST("STRA,R2  H'0123'",      0xCE, 0x01, 0x23);
    TEST("STRA,R0  H'0345',R2,+", 0xCE, 0x23, 0x45);
    TEST("STRA,R0  H'0567',R2,-", 0xCE, 0x45, 0x67);
    TEST("STRA,R0  H'1789',R2",   0xCE, 0x77, 0x89);
    TEST("STRA,R2 *H'09AB'",      0xCE, 0x89, 0xAB);
    TEST("STRA,R0 *H'0BCD',R2,+", 0xCE, 0xAB, 0xCD);
    TEST("STRA,R0 *H'0DEF',R2,-", 0xCE, 0xCD, 0xEF);
    TEST("STRA,R0 *H'1F01',R2",   0xCE, 0xFF, 0x01);
    TEST("STRA,R3  H'1123'",      0xCF, 0x11, 0x23);
    TEST("STRA,R0  H'0345',R3,+", 0xCF, 0x23, 0x45);
    TEST("STRA,R0  H'0567',R3,-", 0xCF, 0x45, 0x67);
    TEST("STRA,R0  H'0789',R3",   0xCF, 0x67, 0x89);
    TEST("STRA,R3 *H'19AB'",      0xCF, 0x99, 0xAB);
    TEST("STRA,R0 *H'0BCD',R3,+", 0xCF, 0xAB, 0xCD);
    TEST("STRA,R0 *H'0DEF',R3,-", 0xCF, 0xCD, 0xEF);
    TEST("STRA,R0 *H'0F01',R3",   0xCF, 0xEF, 0x01);
}

static void test_arithmetic() {
    TEST("ADDZ,R0",             0x80);
    TEST("ADDZ,R1",             0x81);
    TEST("ADDZ,R2",             0x82);
    TEST("ADDZ,R3",             0x83);
    TEST("ADDI,R0 H'56'",         0x84, 0x56);
    TEST("ADDI,R1 H'67'",         0x85, 0x67);
    TEST("ADDI,R2 H'78'",         0x86, 0x78);
    TEST("ADDI,R3 H'89'",         0x87, 0x89);
    TEST("ADDR,R0 *$+H'0B'",      0x88, 0x89);
    TEST("ADDR,R1  $+H'0C'",      0x89, 0x0A);
    TEST("ADDR,R2  $+H'0D'",      0x8A, 0x0B);
    TEST("ADDR,R3  $+H'0E'",      0x8B, 0x0C);
    TEST("ADDA,R0  H'1123'",      0x8C, 0x11, 0x23);
    TEST("ADDA,R0  H'0345',R0,+", 0x8C, 0x23, 0x45);
    TEST("ADDA,R0  H'0567',R0,-", 0x8C, 0x45, 0x67);
    TEST("ADDA,R0  H'0789',R0",   0x8C, 0x67, 0x89);
    TEST("ADDA,R0 *H'19AB'",      0x8C, 0x99, 0xAB);
    TEST("ADDA,R0 *H'0BCD',R0,+", 0x8C, 0xAB, 0xCD);
    TEST("ADDA,R0 *H'0DEF',R0,-", 0x8C, 0xCD, 0xEF);
    TEST("ADDA,R0 *H'0F01',R0",   0x8C, 0xEF, 0x01);
    TEST("ADDA,R1  H'0123'",      0x8D, 0x01, 0x23);
    TEST("ADDA,R0  H'1345',R1,+", 0x8D, 0x33, 0x45);
    TEST("ADDA,R0  H'0567',R1,-", 0x8D, 0x45, 0x67);
    TEST("ADDA,R0  H'0789',R1",   0x8D, 0x67, 0x89);
    TEST("ADDA,R1 *H'09AB'",      0x8D, 0x89, 0xAB);
    TEST("ADDA,R0 *H'1BCD',R1,+", 0x8D, 0xBB, 0xCD);
    TEST("ADDA,R0 *H'0DEF',R1,-", 0x8D, 0xCD, 0xEF);
    TEST("ADDA,R0 *H'0F01',R1",   0x8D, 0xEF, 0x01);
    TEST("ADDA,R2  H'0123'",      0x8E, 0x01, 0x23);
    TEST("ADDA,R0  H'0345',R2,+", 0x8E, 0x23, 0x45);
    TEST("ADDA,R0  H'1567',R2,-", 0x8E, 0x55, 0x67);
    TEST("ADDA,R0  H'0789',R2",   0x8E, 0x67, 0x89);
    TEST("ADDA,R2 *H'09AB'",      0x8E, 0x89, 0xAB);
    TEST("ADDA,R0 *H'0BCD',R2,+", 0x8E, 0xAB, 0xCD);
    TEST("ADDA,R0 *H'1DEF',R2,-", 0x8E, 0xDD, 0xEF);
    TEST("ADDA,R0 *H'0F01',R2",   0x8E, 0xEF, 0x01);
    TEST("ADDA,R3  H'0123'",      0x8F, 0x01, 0x23);
    TEST("ADDA,R0  H'0345',R3,+", 0x8F, 0x23, 0x45);
    TEST("ADDA,R0  H'0567',R3,-", 0x8F, 0x45, 0x67);
    TEST("ADDA,R0  H'1789',R3",   0x8F, 0x77, 0x89);
    TEST("ADDA,R3 *H'09AB'",      0x8F, 0x89, 0xAB);
    TEST("ADDA,R0 *H'0BCD',R3,+", 0x8F, 0xAB, 0xCD);
    TEST("ADDA,R0 *H'0DEF',R3,-", 0x8F, 0xCD, 0xEF);
    TEST("ADDA,R0 *H'1F01',R3",   0x8F, 0xFF, 0x01);

    TEST("SUBZ,R0",             0xA0);
    TEST("SUBZ,R1",             0xA1);
    TEST("SUBZ,R2",             0xA2);
    TEST("SUBZ,R3",             0xA3);
    TEST("SUBI,R0 H'56'",         0xA4, 0x56);
    TEST("SUBI,R1 H'67'",         0xA5, 0x67);
    TEST("SUBI,R2 H'78'",         0xA6, 0x78);
    TEST("SUBI,R3 H'89'",         0xA7, 0x89);
    TEST("SUBR,R0 *$+H'0B'",      0xA8, 0x89);
    TEST("SUBR,R1  $+H'0C'",      0xA9, 0x0A);
    TEST("SUBR,R2  $+H'0D'",      0xAA, 0x0B);
    TEST("SUBR,R3  $+H'0E'",      0xAB, 0x0C);
    TEST("SUBA,R0  H'1123'",      0xAC, 0x11, 0x23);
    TEST("SUBA,R0  H'0345',R0,+", 0xAC, 0x23, 0x45);
    TEST("SUBA,R0  H'0567',R0,-", 0xAC, 0x45, 0x67);
    TEST("SUBA,R0  H'0789',R0",   0xAC, 0x67, 0x89);
    TEST("SUBA,R0 *H'19AB'",      0xAC, 0x99, 0xAB);
    TEST("SUBA,R0 *H'0BCD',R0,+", 0xAC, 0xAB, 0xCD);
    TEST("SUBA,R0 *H'0DEF',R0,-", 0xAC, 0xCD, 0xEF);
    TEST("SUBA,R0 *H'0F01',R0",   0xAC, 0xEF, 0x01);
    TEST("SUBA,R1  H'0123'",      0xAD, 0x01, 0x23);
    TEST("SUBA,R0  H'1345',R1,+", 0xAD, 0x33, 0x45);
    TEST("SUBA,R0  H'0567',R1,-", 0xAD, 0x45, 0x67);
    TEST("SUBA,R0  H'0789',R1",   0xAD, 0x67, 0x89);
    TEST("SUBA,R1 *H'09AB'",      0xAD, 0x89, 0xAB);
    TEST("SUBA,R0 *H'1BCD',R1,+", 0xAD, 0xBB, 0xCD);
    TEST("SUBA,R0 *H'0DEF',R1,-", 0xAD, 0xCD, 0xEF);
    TEST("SUBA,R0 *H'0F01',R1",   0xAD, 0xEF, 0x01);
    TEST("SUBA,R2  H'0123'",      0xAE, 0x01, 0x23);
    TEST("SUBA,R0  H'0345',R2,+", 0xAE, 0x23, 0x45);
    TEST("SUBA,R0  H'1567',R2,-", 0xAE, 0x55, 0x67);
    TEST("SUBA,R0  H'0789',R2",   0xAE, 0x67, 0x89);
    TEST("SUBA,R2 *H'09AB'",      0xAE, 0x89, 0xAB);
    TEST("SUBA,R0 *H'0BCD',R2,+", 0xAE, 0xAB, 0xCD);
    TEST("SUBA,R0 *H'1DEF',R2,-", 0xAE, 0xDD, 0xEF);
    TEST("SUBA,R0 *H'0F01',R2",   0xAE, 0xEF, 0x01);
    TEST("SUBA,R3  H'0123'",      0xAF, 0x01, 0x23);
    TEST("SUBA,R0  H'0345',R3,+", 0xAF, 0x23, 0x45);
    TEST("SUBA,R0  H'0567',R3,-", 0xAF, 0x45, 0x67);
    TEST("SUBA,R0  H'1789',R3",   0xAF, 0x77, 0x89);
    TEST("SUBA,R3 *H'09AB'",      0xAF, 0x89, 0xAB);
    TEST("SUBA,R0 *H'0BCD',R3,+", 0xAF, 0xAB, 0xCD);
    TEST("SUBA,R0 *H'0DEF',R3,-", 0xAF, 0xCD, 0xEF);
    TEST("SUBA,R0 *H'1F01',R3",   0xAF, 0xFF, 0x01);

    TEST("DAR,R0", 0x94);
    TEST("DAR,R1", 0x95);
    TEST("DAR,R2", 0x96);
    TEST("DAR,R3", 0x97);
}

static void test_logical() {
    ERRT("ANDZ,R0",             OPERAND_NOT_ALLOWED, "R0");
    TEST("ANDZ,R1",             0x41);
    TEST("ANDZ,R2",             0x42);
    TEST("ANDZ,R3",             0x43);
    TEST("ANDI,R0 H'56'",         0x44, 0x56);
    TEST("ANDI,R1 H'67'",         0x45, 0x67);
    TEST("ANDI,R2 H'78'",         0x46, 0x78);
    TEST("ANDI,R3 H'89'",         0x47, 0x89);
    TEST("ANDR,R0 *$+H'0B'",      0x48, 0x89);
    TEST("ANDR,R1  $+H'0C'",      0x49, 0x0A);
    TEST("ANDR,R2  $+H'0D'",      0x4A, 0x0B);
    TEST("ANDR,R3  $+H'0E'",      0x4B, 0x0C);
    TEST("ANDA,R0  H'1123'",      0x4C, 0x11, 0x23);
    TEST("ANDA,R0  H'0345',R0,+", 0x4C, 0x23, 0x45);
    TEST("ANDA,R0  H'0567',R0,-", 0x4C, 0x45, 0x67);
    TEST("ANDA,R0  H'0789',R0",   0x4C, 0x67, 0x89);
    TEST("ANDA,R0 *H'19AB'",      0x4C, 0x99, 0xAB);
    TEST("ANDA,R0 *H'0BCD',R0,+", 0x4C, 0xAB, 0xCD);
    TEST("ANDA,R0 *H'0DEF',R0,-", 0x4C, 0xCD, 0xEF);
    TEST("ANDA,R0 *H'0F01',R0",   0x4C, 0xEF, 0x01);
    TEST("ANDA,R1  H'0123'",      0x4D, 0x01, 0x23);
    TEST("ANDA,R0  H'1345',R1,+", 0x4D, 0x33, 0x45);
    TEST("ANDA,R0  H'0567',R1,-", 0x4D, 0x45, 0x67);
    TEST("ANDA,R0  H'0789',R1",   0x4D, 0x67, 0x89);
    TEST("ANDA,R1 *H'09AB'",      0x4D, 0x89, 0xAB);
    TEST("ANDA,R0 *H'1BCD',R1,+", 0x4D, 0xBB, 0xCD);
    TEST("ANDA,R0 *H'0DEF',R1,-", 0x4D, 0xCD, 0xEF);
    TEST("ANDA,R0 *H'0F01',R1",   0x4D, 0xEF, 0x01);
    TEST("ANDA,R2  H'0123'",      0x4E, 0x01, 0x23);
    TEST("ANDA,R0  H'0345',R2,+", 0x4E, 0x23, 0x45);
    TEST("ANDA,R0  H'1567',R2,-", 0x4E, 0x55, 0x67);
    TEST("ANDA,R0  H'0789',R2",   0x4E, 0x67, 0x89);
    TEST("ANDA,R2 *H'09AB'",      0x4E, 0x89, 0xAB);
    TEST("ANDA,R0 *H'0BCD',R2,+", 0x4E, 0xAB, 0xCD);
    TEST("ANDA,R0 *H'1DEF',R2,-", 0x4E, 0xDD, 0xEF);
    TEST("ANDA,R0 *H'0F01',R2",   0x4E, 0xEF, 0x01);
    TEST("ANDA,R3  H'0123'",      0x4F, 0x01, 0x23);
    TEST("ANDA,R0  H'0345',R3,+", 0x4F, 0x23, 0x45);
    TEST("ANDA,R0  H'0567',R3,-", 0x4F, 0x45, 0x67);
    TEST("ANDA,R0  H'1789',R3",   0x4F, 0x77, 0x89);
    TEST("ANDA,R3 *H'09AB'",      0x4F, 0x89, 0xAB);
    TEST("ANDA,R0 *H'0BCD',R3,+", 0x4F, 0xAB, 0xCD);
    TEST("ANDA,R0 *H'0DEF',R3,-", 0x4F, 0xCD, 0xEF);
    TEST("ANDA,R0 *H'1F01',R3",   0x4F, 0xFF, 0x01);

    TEST("IORZ,R0",             0x60);
    TEST("IORZ,R1",             0x61);
    TEST("IORZ,R2",             0x62);
    TEST("IORZ,R3",             0x63);
    TEST("IORI,R0 H'56'",         0x64, 0x56);
    TEST("IORI,R1 H'67'",         0x65, 0x67);
    TEST("IORI,R2 H'78'",         0x66, 0x78);
    TEST("IORI,R3 H'89'",         0x67, 0x89);
    TEST("IORR,R0 *$+H'0B'",      0x68, 0x89);
    TEST("IORR,R1  $+H'0C'",      0x69, 0x0A);
    TEST("IORR,R2  $+H'0D'",      0x6A, 0x0B);
    TEST("IORR,R3  $+H'0E'",      0x6B, 0x0C);
    TEST("IORA,R0  H'1123'",      0x6C, 0x11, 0x23);
    TEST("IORA,R0  H'0345',R0,+", 0x6C, 0x23, 0x45);
    TEST("IORA,R0  H'0567',R0,-", 0x6C, 0x45, 0x67);
    TEST("IORA,R0  H'0789',R0",   0x6C, 0x67, 0x89);
    TEST("IORA,R0 *H'19AB'",      0x6C, 0x99, 0xAB);
    TEST("IORA,R0 *H'0BCD',R0,+", 0x6C, 0xAB, 0xCD);
    TEST("IORA,R0 *H'0DEF',R0,-", 0x6C, 0xCD, 0xEF);
    TEST("IORA,R0 *H'0F01',R0",   0x6C, 0xEF, 0x01);
    TEST("IORA,R1  H'0123'",      0x6D, 0x01, 0x23);
    TEST("IORA,R0  H'1345',R1,+", 0x6D, 0x33, 0x45);
    TEST("IORA,R0  H'0567',R1,-", 0x6D, 0x45, 0x67);
    TEST("IORA,R0  H'0789',R1",   0x6D, 0x67, 0x89);
    TEST("IORA,R1 *H'09AB'",      0x6D, 0x89, 0xAB);
    TEST("IORA,R0 *H'1BCD',R1,+", 0x6D, 0xBB, 0xCD);
    TEST("IORA,R0 *H'0DEF',R1,-", 0x6D, 0xCD, 0xEF);
    TEST("IORA,R0 *H'0F01',R1",   0x6D, 0xEF, 0x01);
    TEST("IORA,R2  H'0123'",      0x6E, 0x01, 0x23);
    TEST("IORA,R0  H'0345',R2,+", 0x6E, 0x23, 0x45);
    TEST("IORA,R0  H'1567',R2,-", 0x6E, 0x55, 0x67);
    TEST("IORA,R0  H'0789',R2",   0x6E, 0x67, 0x89);
    TEST("IORA,R2 *H'09AB'",      0x6E, 0x89, 0xAB);
    TEST("IORA,R0 *H'0BCD',R2,+", 0x6E, 0xAB, 0xCD);
    TEST("IORA,R0 *H'1DEF',R2,-", 0x6E, 0xDD, 0xEF);
    TEST("IORA,R0 *H'0F01',R2",   0x6E, 0xEF, 0x01);
    TEST("IORA,R3  H'0123'",      0x6F, 0x01, 0x23);
    TEST("IORA,R0  H'0345',R3,+", 0x6F, 0x23, 0x45);
    TEST("IORA,R0  H'0567',R3,-", 0x6F, 0x45, 0x67);
    TEST("IORA,R0  H'1789',R3",   0x6F, 0x77, 0x89);
    TEST("IORA,R3 *H'09AB'",      0x6F, 0x89, 0xAB);
    TEST("IORA,R0 *H'0BCD',R3,+", 0x6F, 0xAB, 0xCD);
    TEST("IORA,R0 *H'0DEF',R3,-", 0x6F, 0xCD, 0xEF);
    TEST("IORA,R0 *H'1F01',R3",   0x6F, 0xFF, 0x01);

    TEST("EORZ,R0",             0x20);
    TEST("EORZ,R1",             0x21);
    TEST("EORZ,R2",             0x22);
    TEST("EORZ,R3",             0x23);
    TEST("EORI,R0 H'56'",         0x24, 0x56);
    TEST("EORI,R1 H'67'",         0x25, 0x67);
    TEST("EORI,R2 H'78'",         0x26, 0x78);
    TEST("EORI,R3 H'89'",         0x27, 0x89);
    TEST("EORR,R0 *$+H'0B'",      0x28, 0x89);
    TEST("EORR,R1  $+H'0C'",      0x29, 0x0A);
    TEST("EORR,R2  $+H'0D'",      0x2A, 0x0B);
    TEST("EORR,R3  $+H'0E'",      0x2B, 0x0C);
    TEST("EORA,R0  H'1123'",      0x2C, 0x11, 0x23);
    TEST("EORA,R0  H'0345',R0,+", 0x2C, 0x23, 0x45);
    TEST("EORA,R0  H'0567',R0,-", 0x2C, 0x45, 0x67);
    TEST("EORA,R0  H'0789',R0",   0x2C, 0x67, 0x89);
    TEST("EORA,R0 *H'19AB'",      0x2C, 0x99, 0xAB);
    TEST("EORA,R0 *H'0BCD',R0,+", 0x2C, 0xAB, 0xCD);
    TEST("EORA,R0 *H'0DEF',R0,-", 0x2C, 0xCD, 0xEF);
    TEST("EORA,R0 *H'0F01',R0",   0x2C, 0xEF, 0x01);
    TEST("EORA,R1  H'0123'",      0x2D, 0x01, 0x23);
    TEST("EORA,R0  H'1345',R1,+", 0x2D, 0x33, 0x45);
    TEST("EORA,R0  H'0567',R1,-", 0x2D, 0x45, 0x67);
    TEST("EORA,R0  H'0789',R1",   0x2D, 0x67, 0x89);
    TEST("EORA,R1 *H'09AB'",      0x2D, 0x89, 0xAB);
    TEST("EORA,R0 *H'1BCD',R1,+", 0x2D, 0xBB, 0xCD);
    TEST("EORA,R0 *H'0DEF',R1,-", 0x2D, 0xCD, 0xEF);
    TEST("EORA,R0 *H'0F01',R1",   0x2D, 0xEF, 0x01);
    TEST("EORA,R2  H'0123'",      0x2E, 0x01, 0x23);
    TEST("EORA,R0  H'0345',R2,+", 0x2E, 0x23, 0x45);
    TEST("EORA,R0  H'1567',R2,-", 0x2E, 0x55, 0x67);
    TEST("EORA,R0  H'0789',R2",   0x2E, 0x67, 0x89);
    TEST("EORA,R2 *H'09AB'",      0x2E, 0x89, 0xAB);
    TEST("EORA,R0 *H'0BCD',R2,+", 0x2E, 0xAB, 0xCD);
    TEST("EORA,R0 *H'1DEF',R2,-", 0x2E, 0xDD, 0xEF);
    TEST("EORA,R0 *H'0F01',R2",   0x2E, 0xEF, 0x01);
    TEST("EORA,R3  H'0123'",      0x2F, 0x01, 0x23);
    TEST("EORA,R0  H'0345',R3,+", 0x2F, 0x23, 0x45);
    TEST("EORA,R0  H'0567',R3,-", 0x2F, 0x45, 0x67);
    TEST("EORA,R0  H'1789',R3",   0x2F, 0x77, 0x89);
    TEST("EORA,R3 *H'09AB'",      0x2F, 0x89, 0xAB);
    TEST("EORA,R0 *H'0BCD',R3,+", 0x2F, 0xAB, 0xCD);
    TEST("EORA,R0 *H'0DEF',R3,-", 0x2F, 0xCD, 0xEF);
    TEST("EORA,R0 *H'1F01',R3",   0x2F, 0xFF, 0x01);
}

static void test_rotate_compare() {
    TEST("COMZ,R0",             0xE0);
    TEST("COMZ,R1",             0xE1);
    TEST("COMZ,R2",             0xE2);
    TEST("COMZ,R3",             0xE3);
    TEST("COMI,R0 H'56'",         0xE4, 0x56);
    TEST("COMI,R1 H'67'",         0xE5, 0x67);
    TEST("COMI,R2 H'78'",         0xE6, 0x78);
    TEST("COMI,R3 H'89'",         0xE7, 0x89);
    TEST("COMR,R0 *$+H'0B'",      0xE8, 0x89);
    TEST("COMR,R1  $+H'0C'",      0xE9, 0x0A);
    TEST("COMR,R2  $+H'0D'",      0xEA, 0x0B);
    TEST("COMR,R3  $+H'0E'",      0xEB, 0x0C);
    TEST("COMA,R0  H'1123'",      0xEC, 0x11, 0x23);
    TEST("COMA,R0  H'0345',R0,+", 0xEC, 0x23, 0x45);
    TEST("COMA,R0  H'0567',R0,-", 0xEC, 0x45, 0x67);
    TEST("COMA,R0  H'0789',R0",   0xEC, 0x67, 0x89);
    TEST("COMA,R0 *H'19AB'",      0xEC, 0x99, 0xAB);
    TEST("COMA,R0 *H'0BCD',R0,+", 0xEC, 0xAB, 0xCD);
    TEST("COMA,R0 *H'0DEF',R0,-", 0xEC, 0xCD, 0xEF);
    TEST("COMA,R0 *H'0F01',R0",   0xEC, 0xEF, 0x01);
    TEST("COMA,R1  H'0123'",      0xED, 0x01, 0x23);
    TEST("COMA,R0  H'1345',R1,+", 0xED, 0x33, 0x45);
    TEST("COMA,R0  H'0567',R1,-", 0xED, 0x45, 0x67);
    TEST("COMA,R0  H'0789',R1",   0xED, 0x67, 0x89);
    TEST("COMA,R1 *H'09AB'",      0xED, 0x89, 0xAB);
    TEST("COMA,R0 *H'1BCD',R1,+", 0xED, 0xBB, 0xCD);
    TEST("COMA,R0 *H'0DEF',R1,-", 0xED, 0xCD, 0xEF);
    TEST("COMA,R0 *H'0F01',R1",   0xED, 0xEF, 0x01);
    TEST("COMA,R2  H'0123'",      0xEE, 0x01, 0x23);
    TEST("COMA,R0  H'0345',R2,+", 0xEE, 0x23, 0x45);
    TEST("COMA,R0  H'1567',R2,-", 0xEE, 0x55, 0x67);
    TEST("COMA,R0  H'0789',R2",   0xEE, 0x67, 0x89);
    TEST("COMA,R2 *H'09AB'",      0xEE, 0x89, 0xAB);
    TEST("COMA,R0 *H'0BCD',R2,+", 0xEE, 0xAB, 0xCD);
    TEST("COMA,R0 *H'1DEF',R2,-", 0xEE, 0xDD, 0xEF);
    TEST("COMA,R0 *H'0F01',R2",   0xEE, 0xEF, 0x01);
    TEST("COMA,R3  H'0123'",      0xEF, 0x01, 0x23);
    TEST("COMA,R0  H'0345',R3,+", 0xEF, 0x23, 0x45);
    TEST("COMA,R0  H'0567',R3,-", 0xEF, 0x45, 0x67);
    TEST("COMA,R0  H'1789',R3",   0xEF, 0x77, 0x89);
    TEST("COMA,R3 *H'09AB'",      0xEF, 0x89, 0xAB);
    TEST("COMA,R0 *H'0BCD',R3,+", 0xEF, 0xAB, 0xCD);
    TEST("COMA,R0 *H'0DEF',R3,-", 0xEF, 0xCD, 0xEF);
    TEST("COMA,R0 *H'1F01',R3",   0xEF, 0xFF, 0x01);

    TEST("RRR,R0", 0x50);
    TEST("RRR,R1", 0x51);
    TEST("RRR,R2", 0x52);
    TEST("RRR,R3", 0x53);
    TEST("RRL,R0", 0xD0);
    TEST("RRL,R1", 0xD1);
    TEST("RRL,R2", 0xD2);
    TEST("RRL,R3", 0xD3);
}

static void test_branch() {
    ATEST(0x1000, "bctr,eq  $+h'1b'", 0x18, 0x19);
    ATEST(0x1000, "BCTR,EQ *$+H'1C'", 0x18, 0x9A);
    ATEST(0x1000, "BCTR,GT  $+H'1C'", 0x19, 0x1A);
    ATEST(0x1000, "BCTR,GT *$+H'2D'", 0x19, 0xAB);
    ATEST(0x1000, "BCTR,LT  $+H'1D'", 0x1A, 0x1B);
    ATEST(0x1000, "BCTR,LT *$+H'3E'", 0x1A, 0xBC);
    ATEST(0x1000, "BCTR,UN  $+H'1E'", 0x1B, 0x1C);
    ATEST(0x1000, "BCTR,UN *$-H'31'", 0x1B, 0xCD);
    ATEST(0x1000, "BCTA,EQ  H'7D1E'", 0x1C, 0x7D, 0x1E);
    ATEST(0x2000, "BCTA,EQ *H'5EF0'", 0x1C, 0xDE, 0xF0);
    ATEST(0x3000, "BCTA,GT  H'1E1F'", 0x1D, 0x1E, 0x1F);
    ATEST(0x4000, "BCTA,GT *H'6F01'", 0x1D, 0xEF, 0x01);
    ATEST(0x5000, "BCTA,LT  H'1F20'", 0x1E, 0x1F, 0x20);
    ATEST(0x6000, "BCTA,LT *H'7012'", 0x1E, 0xF0, 0x12);
    ATEST(0x7000, "BCTA,UN  H'2021'", 0x1F, 0x20, 0x21);
    ATEST(0x0000, "BCTA,UN *H'49AB'", 0x1F, 0xC9, 0xAB);
 
    ATEST(0x1000, "BCFR,EQ  $+H'1B'", 0x98, 0x19);
    ATEST(0x1000, "BCFR,EQ *$+H'1C'", 0x98, 0x9A);
    ATEST(0x1000, "BCFR,GT  $+H'1C'", 0x99, 0x1A);
    ATEST(0x1000, "BCFR,GT *$+H'2D'", 0x99, 0xAB);
    ATEST(0x1000, "BCFR,LT  $+H'1D'", 0x9A, 0x1B);
    ATEST(0x1000, "BCFR,LT *$+H'3E'", 0x9A, 0xBC);
    AERRT(0x1000, "BCFR,UN  $+H'1E'", OPERAND_NOT_ALLOWED, "UN  $+H'1E'");
    AERRT(0x1000, "BCFR,UN *$-H'31'", OPERAND_NOT_ALLOWED, "UN *$-H'31'");
    ATEST(0x7000, "BCFA,EQ  H'1D1E'", 0x9C, 0x1D, 0x1E);
    ATEST(0x6000, "BCFA,EQ *H'7EF0'", 0x9C, 0xFE, 0xF0);
    ATEST(0x5000, "BCFA,GT  H'1E1F'", 0x9D, 0x1E, 0x1F);
    ATEST(0x4000, "BCFA,GT *H'6F01'", 0x9D, 0xEF, 0x01);
    ATEST(0x3000, "BCFA,LT  H'1F20'", 0x9E, 0x1F, 0x20);
    ATEST(0x2000, "BCFA,LT *H'7012'", 0x9E, 0xF0, 0x12);
    AERRT(0x7000, "BCFA,UN  H'2021'", OPERAND_NOT_ALLOWED, "UN  H'2021'");
    AERRT(0x0000, "BCFA,UN *H'49AB'", OPERAND_NOT_ALLOWED, "UN *H'49AB'");

    ATEST(0x1000, "BRNR,R0  $+H'1B'", 0x58, 0x19);
    ATEST(0x1000, "BRNR,R0 *$+H'1C'", 0x58, 0x9A);
    ATEST(0x1000, "BRNR,R1  $+H'1C'", 0x59, 0x1A);
    ATEST(0x1000, "BRNR,R1 *$+H'2D'", 0x59, 0xAB);
    ATEST(0x1000, "BRNR,R2  $+H'1D'", 0x5A, 0x1B);
    ATEST(0x1000, "BRNR,R2 *$+H'3E'", 0x5A, 0xBC);
    ATEST(0x1000, "BRNR,R3  $+H'1E'", 0x5B, 0x1C);
    ATEST(0x1000, "BRNR,R3 *$-H'31'", 0x5B, 0xCD);
    ATEST(0x1000, "BRNA,R0  H'5D5E'", 0x5C, 0x5D, 0x5E);
    ATEST(0x2000, "BRNA,R0 *H'5EF0'", 0x5C, 0xDE, 0xF0);
    ATEST(0x3000, "BRNA,R1  H'5E5F'", 0x5D, 0x5E, 0x5F);
    ATEST(0x4000, "BRNA,R1 *H'6F01'", 0x5D, 0xEF, 0x01);
    ATEST(0x5000, "BRNA,R2  H'5F60'", 0x5E, 0x5F, 0x60);
    ATEST(0x6000, "BRNA,R2 *H'7012'", 0x5E, 0xF0, 0x12);
    ATEST(0x7000, "BRNA,R3  H'6061'", 0x5F, 0x60, 0x61);
    ATEST(0x0000, "BRNA,R3 *H'09AB'", 0x5F, 0x89, 0xAB);

    ATEST(0x1000, "BIRR,R0  $+H'1B'", 0xD8, 0x19);
    ATEST(0x1000, "BIRR,R0 *$+H'1C'", 0xD8, 0x9A);
    ATEST(0x1000, "BIRR,R1  $+H'1C'", 0xD9, 0x1A);
    ATEST(0x1000, "BIRR,R1 *$+H'2D'", 0xD9, 0xAB);
    ATEST(0x1000, "BIRR,R2  $+H'1D'", 0xDA, 0x1B);
    ATEST(0x1000, "BIRR,R2 *$+H'3E'", 0xDA, 0xBC);
    ATEST(0x1000, "BIRR,R3  $+H'1E'", 0xDB, 0x1C);
    ATEST(0x1000, "BIRR,R3 *$-H'31'", 0xDB, 0xCD);
    ATEST(0x0000, "BIRA,R0  H'5D5E'", 0xDC, 0x5D, 0x5E);
    ATEST(0x1000, "BIRA,R0 *H'6F01'", 0xDC, 0xEF, 0x01);
    ATEST(0x2000, "BIRA,R1  H'5E5F'", 0xDD, 0x5E, 0x5F);
    ATEST(0x3000, "BIRA,R1 *H'5EDF'", 0xDD, 0xDE, 0xDF);
    ATEST(0x4000, "BIRA,R2  H'5F60'", 0xDE, 0x5F, 0x60);
    ATEST(0x6000, "BIRA,R2 *H'7012'", 0xDE, 0xF0, 0x12);
    ATEST(0x6000, "BIRA,R3  H'0123'", 0xDF, 0x01, 0x23);
    ATEST(0x7000, "BIRA,R3 *H'5FE0'", 0xDF, 0xDF, 0xE0);

    ATEST(0x1000, "BDRR,R0  $+H'1B'", 0xF8, 0x19);
    ATEST(0x1000, "BDRR,R0 *$+H'1C'", 0xF8, 0x9A);
    ATEST(0x1000, "BDRR,R1  $+H'1C'", 0xF9, 0x1A);
    ATEST(0x1000, "BDRR,R1 *$+H'2D'", 0xF9, 0xAB);
    ATEST(0x1000, "BDRR,R2  $+H'1D'", 0xFA, 0x1B);
    ATEST(0x1000, "BDRR,R2 *$+H'3E'", 0xFA, 0xBC);
    ATEST(0x1000, "BDRR,R3  $+H'1E'", 0xFB, 0x1C);
    ATEST(0x1000, "BDRR,R3 *$-H'31'", 0xFB, 0xCD);
    ATEST(0x7000, "BDRA,R0  H'5D5E'", 0xFC, 0x5D, 0x5E);
    ATEST(0x6000, "BDRA,R0 *H'7DFE'", 0xFC, 0xFD, 0xFE);
    ATEST(0x5000, "BDRA,R1  H'5E5F'", 0xFD, 0x5E, 0x5F);
    ATEST(0x4000, "BDRA,R1 *H'7EFF'", 0xFD, 0xFE, 0xFF);
    ATEST(0x3000, "BDRA,R2  H'5F60'", 0xFE, 0x5F, 0x60);
    ATEST(0x2000, "BDRA,R2 *H'5CBA'", 0xFE, 0xDC, 0xBA);
    ATEST(0x1000, "BDRA,R3  H'0011'", 0xFF, 0x00, 0x11);
    ATEST(0x0000, "BDRA,R3 *H'7EFD'", 0xFF, 0xFE, 0xFD);

    ATEST(0x1000, "ZBRR  H'003F'", 0x9B, 0x3F);
    ATEST(0x1000, "ZBRR  H'1FC0'", 0x9B, 0x40);
    ATEST(0x1000, "ZBRR *H'003F'", 0x9B, 0xBF);
    ATEST(0x1000, "ZBRR *H'1FC0'", 0x9B, 0xC0);

    ATEST(0x1000, "BXA  H'0123'",    0x9F, 0x01, 0x23);
    ATEST(0x1000, "BXA  H'7FFF'",    0x9F, 0x7F, 0xFF);
    ATEST(0x1000, "BXA *H'7FFF'",    0x9F, 0xFF, 0xFF);
    ATEST(0x1000, "BXA  H'7FFF',R3", 0x9F, 0x7F, 0xFF);
    ATEST(0x1000, "BXA *H'7FFF',R3", 0x9F, 0xFF, 0xFF);
}

static void test_subroutine() {
    ATEST(0x1000, "BSTR,EQ  $+H'1B'", 0x38, 0x19);
    ATEST(0x1000, "BSTR,EQ *$+H'1C'", 0x38, 0x9A);
    ATEST(0x1000, "BSTR,GT  $+H'1C'", 0x39, 0x1A);
    ATEST(0x1000, "BSTR,GT *$+H'2D'", 0x39, 0xAB);
    ATEST(0x1000, "BSTR,LT  $+H'1D'", 0x3A, 0x1B);
    ATEST(0x1000, "BSTR,LT *$+H'3E'", 0x3A, 0xBC);
    ATEST(0x1000, "BSTR,UN  $+H'1E'", 0x3B, 0x1C);
    ATEST(0x1000, "BSTR,UN *$-H'31'", 0x3B, 0xCD);
    ATEST(0x1000, "BSTA,EQ  H'7D1E'", 0x3C, 0x7D, 0x1E);
    ATEST(0x2000, "BSTA,EQ *H'5EF0'", 0x3C, 0xDE, 0xF0);
    ATEST(0x3000, "BSTA,GT  H'1E1F'", 0x3D, 0x1E, 0x1F);
    ATEST(0x4000, "BSTA,GT *H'6F01'", 0x3D, 0xEF, 0x01);
    ATEST(0x5000, "BSTA,LT  H'1F20'", 0x3E, 0x1F, 0x20);
    ATEST(0x6000, "BSTA,LT *H'7012'", 0x3E, 0xF0, 0x12);
    ATEST(0x7000, "BSTA,UN  H'2021'", 0x3F, 0x20, 0x21);
    ATEST(0x0000, "BSTA,UN *H'49AB'", 0x3F, 0xC9, 0xAB);

    ATEST(0x1000, "BSFR,EQ  $+H'1B'", 0xB8, 0x19);
    ATEST(0x1000, "BSFR,EQ *$+H'1C'", 0xB8, 0x9A);
    ATEST(0x1000, "BSFR,GT  $+H'1C'", 0xB9, 0x1A);
    ATEST(0x1000, "BSFR,GT *$+H'2D'", 0xB9, 0xAB);
    ATEST(0x1000, "BSFR,LT  $+H'1D'", 0xBA, 0x1B);
    ATEST(0x1000, "BSFR,LT *$+H'3E'", 0xBA, 0xBC);
    AERRT(0x1000, "BSFR,UN  $+H'1E'", OPERAND_NOT_ALLOWED, "UN  $+H'1E'");
    AERRT(0x1000, "BSFR,UN *$-H'31'", OPERAND_NOT_ALLOWED, "UN *$-H'31'");
    ATEST(0x7000, "BSFA,EQ  H'1D1E'", 0xBC, 0x1D, 0x1E);
    ATEST(0x6000, "BSFA,EQ *H'7EF0'", 0xBC, 0xFE, 0xF0);
    ATEST(0x5000, "BSFA,GT  H'1E1F'", 0xBD, 0x1E, 0x1F);
    ATEST(0x4000, "BSFA,GT *H'6F01'", 0xBD, 0xEF, 0x01);
    ATEST(0x3000, "BSFA,LT  H'1F20'", 0xBE, 0x1F, 0x20);
    ATEST(0x2000, "BSFA,LT *H'7012'", 0xBE, 0xF0, 0x12);
    AERRT(0x7000, "BSFA,UN  H'2021'", OPERAND_NOT_ALLOWED, "UN  H'2021'");
    AERRT(0x0000, "BSFA,UN *H'49AB'", OPERAND_NOT_ALLOWED, "UN *H'49AB'");

    ATEST(0x1000, "BSNR,R0  $+H'1B'", 0x78, 0x19);
    ATEST(0x1000, "BSNR,R0 *$+H'1C'", 0x78, 0x9A);
    ATEST(0x1000, "BSNR,R1  $+H'1C'", 0x79, 0x1A);
    ATEST(0x1000, "BSNR,R1 *$+H'2D'", 0x79, 0xAB);
    ATEST(0x1000, "BSNR,R2  $+H'1D'", 0x7A, 0x1B);
    ATEST(0x1000, "BSNR,R2 *$+H'3E'", 0x7A, 0xBC);
    ATEST(0x1000, "BSNR,R3  $+H'1E'", 0x7B, 0x1C);
    ATEST(0x1000, "BSNR,R3 *$-H'31'", 0x7B, 0xCD);
    ATEST(0x1000, "BSNA,R0  H'5D5E'", 0x7C, 0x5D, 0x5E);
    ATEST(0x2000, "BSNA,R0 *H'5EF0'", 0x7C, 0xDE, 0xF0);
    ATEST(0x3000, "BSNA,R1  H'5E5F'", 0x7D, 0x5E, 0x5F);
    ATEST(0x4000, "BSNA,R1 *H'6F01'", 0x7D, 0xEF, 0x01);
    ATEST(0x5000, "BSNA,R2  H'5F60'", 0x7E, 0x5F, 0x60);
    ATEST(0x6000, "BSNA,R2 *H'7012'", 0x7E, 0xF0, 0x12);
    ATEST(0x7000, "BSNA,R3  H'6061'", 0x7F, 0x60, 0x61);
    ATEST(0x0000, "BSNA,R3 *H'09AB'", 0x7F, 0x89, 0xAB);


    ATEST(0x1000, "ZBSR  H'003F'", 0xBB, 0x3F);
    ATEST(0x1000, "ZBSR  H'1FC0'", 0xBB, 0x40);
    ATEST(0x1000, "ZBSR *H'003F'", 0xBB, 0xBF);
    ATEST(0x1000, "ZBSR *H'1FC0'", 0xBB, 0xC0);

    ATEST(0x1000, "BSXA  H'0123'",    0xBF, 0x01, 0x23);
    ATEST(0x1000, "BSXA  H'7FFF'",    0xBF, 0x7F, 0xFF);
    ATEST(0x1000, "BSXA *H'7FFF'",    0xBF, 0xFF, 0xFF);
    ATEST(0x1000, "BSXA  H'7FFF',R3", 0xBF, 0x7F, 0xFF);
    ATEST(0x1000, "BSXA *H'7FFF',R3", 0xBF, 0xFF, 0xFF);

    TEST("RETC,EQ", 0x14);
    TEST("RETC,GT", 0x15);
    TEST("RETC,LT", 0x16);
    TEST("RETC,UN", 0x17);
    TEST("RETE,EQ", 0x34);
    TEST("RETE,GT", 0x35);
    TEST("RETE,LT", 0x36);
    TEST("RETE,UN", 0x37);
}

static void test_io() {
    TEST("WRTD,R0", 0xF0);
    TEST("WRTD,R1", 0xF1);
    TEST("WRTD,R2", 0xF2);
    TEST("WRTD,R3", 0xF3);
    TEST("REDD,R0", 0x70);
    TEST("REDD,R1", 0x71);
    TEST("REDD,R2", 0x72);
    TEST("REDD,R3", 0x73);

    TEST("WRTC,R0", 0xB0);
    TEST("WRTC,R1", 0xB1);
    TEST("WRTC,R2", 0xB2);
    TEST("WRTC,R3", 0xB3);
    TEST("REDC,R0", 0x30);
    TEST("REDC,R1", 0x31);
    TEST("REDC,R2", 0x32);
    TEST("REDC,R3", 0x33);

    TEST("WRTE,R0 H'D5'", 0xD4, 0xD5);
    TEST("WRTE,R1 H'D6'", 0xD5, 0xD6);
    TEST("WRTE,R2 H'D7'", 0xD6, 0xD7);
    TEST("WRTE,R3 H'D8'", 0xD7, 0xD8);
    TEST("REDE,R0 H'55'", 0x54, 0x55);
    TEST("REDE,R1 H'56'", 0x55, 0x56);
    TEST("REDE,R2 H'57'", 0x56, 0x57);
    TEST("REDE,R3 H'58'", 0x57, 0x58);
}

static void test_misc() {
    TEST("HALT", 0x40);
    TEST("NOP",  0xC0);
    TEST("TMI,R0 H'F5'", 0xF4, 0xF5);
    TEST("TMI,R1 H'F6'", 0xF5, 0xF6);
    TEST("TMI,R2 H'F7'", 0xF6, 0xF7);
    TEST("TMI,R3 H'F8'", 0xF7, 0xF8);

    TEST("LPSU",      0x92);
    TEST("LPSL",      0x93);
    TEST("CPSU H'75'", 0x74, 0x75);
    TEST("CPSL H'76'", 0x75, 0x76);
    TEST("PPSU H'77'", 0x76, 0x77);
    TEST("PPSL H'78'", 0x77, 0x78);
    TEST("TPSU H'B5'", 0xB4, 0xB5);
    TEST("TPSL H'B6'", 0xB5, 0xB6);
}

static void test_undefined() {
    ERUS("LODI,R0 UNDEF",       "UNDEF",       0x04, 0x00);
    ERUS("LODR,R1 UNDEF",       "UNDEF",       0x09, 0x00);
    ERUS("LODA,R2 UNDEF",       "UNDEF",       0x0E, 0x00, 0x00);
    ERUS("LODA,R0 UNDEF,R2,+",  "UNDEF,R2,+",  0x0E, 0x20, 0x00);
    ERUS("LODA,R0 UNDEF,R2,-",  "UNDEF,R2,-",  0x0E, 0x40, 0x00);
    ERUS("LODA,R0 UNDEF,R2",    "UNDEF,R2",    0x0E, 0x60, 0x00);
    ERUS("LODA,R2 *UNDEF",      "UNDEF",      0x0E, 0x80, 0x00);
    ERUS("LODA,R0 *UNDEF,R2,+", "UNDEF,R2,+", 0x0E, 0xA0, 0x00);
    ERUS("LODA,R0 *UNDEF,R2,-", "UNDEF,R2,-", 0x0E, 0xC0, 0x00);
    ERUS("LODA,R0 *UNDEF,R2",   "UNDEF,R2",   0x0E, 0xE0, 0x00);

    ERUS("BCTR,EQ  UNDEF", "UNDEF", 0x18, 0x00);
    ERUS("BCTR,EQ *UNDEF", "UNDEF", 0x18, 0x80);
    ERUS("BCTA,GT  UNDEF", "UNDEF", 0x1D, 0x00, 0x00);
    ERUS("BCTA,GT *UNDEF", "UNDEF", 0x1D, 0x80, 0x00);

    ERUS("BRNR,R2  UNDEF", "UNDEF", 0x5A, 0x00);
    ERUS("BRNR,R2 *UNDEF", "UNDEF", 0x5A, 0x80);
    ERUS("BRNA,R2  UNDEF", "UNDEF", 0x5E, 0x00, 0x00);
    ERUS("BRNA,R2 *UNDEF", "UNDEF", 0x5E, 0x80, 0x00);

    ERUS("ZBRR  UNDEF", "UNDEF", 0x9B, 0x00);
    ERUS("ZBRR *UNDEF", "UNDEF", 0x9B, 0x80);

    ERUS("BXA  UNDEF",    "UNDEF",    0x9F, 0x00, 0x00);
    ERUS("BXA *UNDEF",    "UNDEF",    0x9F, 0x80, 0x00);
    ERUS("BXA  UNDEF,R3", "UNDEF,R3", 0x9F, 0x00, 0x00);
    ERUS("BXA *UNDEF,R3", "UNDEF,R3", 0x9F, 0x80, 0x00);

    ERUS("WRTE,R3 UNDEF", "UNDEF", 0xD7, 0x00);
    ERUS("REDE,R0 UNDEF", "UNDEF", 0x54, 0x00);
    ERUS("TMI,R0  UNDEF", "UNDEF", 0xF4, 0x00);
    ERUS("TMI,R1  UNDEF", "UNDEF", 0xF5, 0x00);
    ERUS("PPSL    UNDEF", "UNDEF", 0x77, 0x00);
    ERUS("TPSU    UNDEF", "UNDEF", 0xB4, 0x00);
}

static void test_error() {
    ERRT("LODA,R1 H'1345',R2,+",  OPERAND_NOT_ALLOWED, "R1 H'1345',R2,+");
    ERRT("STRZ,R0",             OPERAND_NOT_ALLOWED, "R0");
    ERRT("ANDZ,R0",             OPERAND_NOT_ALLOWED, "R0");

    ATEST(0x1000, "BCTR,EQ  H'1041'", 0x18, 0x3F);
    ATEST(0x1000, "BCTR,EQ *H'0FC2'", 0x18, 0xC0);
    AERRT(0x1000, "BCTR,EQ  H'1042'", OPERAND_TOO_FAR,  "H'1042'", 0x18, 0x40);
    AERRT(0x1000, "BCTR,EQ *H'0FC1'", OPERAND_TOO_FAR, "*H'0FC1'", 0x18, 0xBF);
    ATEST(0x3FF0, "BCTR,EQ  H'2031'", 0x18, 0x3F);
    ATEST(0x2000, "BCTR,EQ *H'3FC2'", 0x18, 0xC0);
    AERRT(0x3FF0, "BCTR,EQ  H'2032'", OPERAND_TOO_FAR,  "H'2032'", 0x18, 0x40);
    AERRT(0x2000, "BCTR,EQ *H'3FC1'", OPERAND_TOO_FAR, "*H'3FC1'", 0x18, 0xBF);
    AERRT(0x3FF0, "BCTR,EQ  H'4000'", OVERWRAP_PAGE,    "H'4000'", 0x18, 0x0E);
    AERRT(0x2000, "BCTR,EQ *H'1FF0'", OVERWRAP_PAGE,   "*H'1FF0'", 0x18, 0xEE);
    AERRT(0x1000, "BCTA,EQ  H'8000'", OVERFLOW_RANGE,   "H'8000'", 0x1C, 0x80, 0x00);

    ATEST(0x3000, "ZBRR H'003F'", 0x9B, 0x3F);
    AERRT(0x3000, "ZBRR H'0040'", OPERAND_TOO_FAR, "H'0040'", 0x9B, 0x40);
    AERRT(0x3000, "ZBRR H'1FBF'", OPERAND_TOO_FAR, "H'1FBF'", 0x9B, 0x3F);
    ATEST(0x3000, "ZBRR H'1FC0'", 0x9B, 0x40);
    AERRT(0x3000, "ZBRR H'2000'", OVERFLOW_RANGE,  "H'2000'", 0x9B, 0x00);
    ATEST(0x3000, "ZBSR H'003F'", 0xBB, 0x3F);
    AERRT(0x3000, "ZBSR H'0040'", OPERAND_TOO_FAR, "H'0040'", 0xBB, 0x40);
    AERRT(0x3000, "ZBSR H'1FBF'", OPERAND_TOO_FAR, "H'1FBF'", 0xBB, 0x3F);
    ATEST(0x3000, "ZBSR H'1FC0'", 0xBB, 0x40);

    AERRT(0x3000, "LODA,R0 H'1000',R1,+", OVERWRAP_PAGE,  "H'1000',R1,+",  0x0D, 0x30, 0x00);
    AERRT(0x3000, "LODA,R0 H'4000',R1,-", OVERWRAP_PAGE,  "H'4000',R1,-",  0x0D, 0x40, 0x00);
    AERRT(0x3000, "LODA,R0 H'8000',R1,+", OVERFLOW_RANGE, "H'8000',R1,+",  0x0D, 0x20, 0x00);
    AERRT(0x3000, "LODA,R0 H'FFFF',R1,-", OVERFLOW_RANGE, "H'FFFF',R1,-", 0x0D, 0x5F, 0xFF);

    AERRT(0x3000, "BXA  H'1000',R0", OPERAND_NOT_ALLOWED, "H'1000',R0");
    AERRT(0x3000, "BXA  H'1000',R1", OPERAND_NOT_ALLOWED, "H'1000',R1");
    AERRT(0x3000, "BSXA H'4000',R2", OPERAND_NOT_ALLOWED, "H'4000',R2");
    ATEST(0x3000, "BXA  H'1000',R3", 0x9F, 0x10, 0x00);
    ATEST(0x3000, "BSXA H'4000',R3", 0xBF, 0x40, 0x00);
    AERRT(0x3000, "BXA  H'8000',R3", OVERFLOW_RANGE, "H'8000',R3", 0x9F, 0x80, 0x00);
    AERRT(0x3000, "BSXA H'FFFF',R3", OVERFLOW_RANGE, "H'FFFF',R3", 0xBF, 0xFF, 0xFF);
}

static void test_data_constant() {
    TEST("DATA -128,255",  0x80, 0xFF);
    TEST("DATA -129,256",  0x7F, 0x00);
    TEST("ACON 0",         0x00, 0x00);
    TEST("DATA A'TE''XT'", 0x54, 0x45, 0x27, 0x58, 0x54);
    TEST("ALIT A'TE\"XT'", 0x54, 0x45, 0x22, 0x58, 0x54);
    TEST("DATA 'TE\"XT'",  0x54, 0x45, 0x22, 0x58, 0x54);
    TEST("ALIT 'TE''XT'",  0x54, 0x45, 0x27, 0x58, 0x54);
}

// clang-format on

void run_tests(const char *cpu) {
    assembler.setCpu(cpu);
    RUN_TEST(test_load_store);
    RUN_TEST(test_arithmetic);
    RUN_TEST(test_logical);
    RUN_TEST(test_rotate_compare);
    RUN_TEST(test_branch);
    RUN_TEST(test_subroutine);
    RUN_TEST(test_io);
    RUN_TEST(test_misc);
    RUN_TEST(test_undefined);
    RUN_TEST(test_error);
    RUN_TEST(test_data_constant);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

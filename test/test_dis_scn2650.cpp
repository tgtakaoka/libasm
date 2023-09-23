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

#include "dis_scn2650.h"
#include "test_dis_helper.h"

using namespace libasm;
using namespace libasm::scn2650;
using namespace libasm::test;

DisScn2650 dis2650;
Disassembler &disassembler(dis2650);

static void set_up() {
    disassembler.reset();
    disassembler.setCpu("2650");
}

static void tear_down() {
    symtab.reset();
}

// clang-format off
void test_cpu() {
    EQUALS("cpu 2650", true,   disassembler.setCpu("2650"));
    EQUALS_P("cpu 2650", "2650", disassembler.cpu_P());

    EQUALS("cpu Scn2650", true,   disassembler.setCpu("Scn2650"));
    EQUALS_P("cpu Scn2650", "2650", disassembler.cpu_P());
}

void test_load_store() {
    TEST(LODZ_R0, "",              0x60);
    TEST(LODZ_R1, "",              0x01);
    TEST(LODZ_R2, "",              0x02);
    TEST(LODZ_R3, "",              0x03);
    TEST(LODI_R0, "H'56'",           0x04, 0x56);
    TEST(LODI_R1, "H'67'",           0x05, 0x67);
    TEST(LODI_R2, "H'78'",           0x06, 0x78);
    TEST(LODI_R3, "H'89'",           0x07, 0x89);
    TEST(LODR_R0, "*H'000B'",        0x08, 0x89);
    TEST(LODR_R1, "H'000C'",         0x09, 0x0A);
    TEST(LODR_R2, "H'000D'",         0x0A, 0x0B);
    TEST(LODR_R3, "H'000E'",         0x0B, 0x0C);
    TEST(LODA_R0, "H'1123'",         0x0C, 0x11, 0x23);
    TEST(LODA_R0, "H'0345', R0, +",  0x0C, 0x23, 0x45);
    TEST(LODA_R0, "H'0567', R0, -",  0x0C, 0x45, 0x67);
    TEST(LODA_R0, "H'0789', R0",     0x0C, 0x67, 0x89);
    TEST(LODA_R0, "*H'19AB'",        0x0C, 0x99, 0xAB);
    TEST(LODA_R0, "*H'0BCD', R0, +", 0x0C, 0xAB, 0xCD);
    TEST(LODA_R0, "*H'0DEF', R0, -", 0x0C, 0xCD, 0xEF);
    TEST(LODA_R0, "*H'0F01', R0",    0x0C, 0xEF, 0x01);
    TEST(LODA_R1, "H'0123'",         0x0D, 0x01, 0x23);
    TEST(LODA_R0, "H'1345', R1, +",  0x0D, 0x33, 0x45);
    TEST(LODA_R0, "H'0567', R1, -",  0x0D, 0x45, 0x67);
    TEST(LODA_R0, "H'0789', R1",     0x0D, 0x67, 0x89);
    TEST(LODA_R1, "*H'09AB'",        0x0D, 0x89, 0xAB);
    TEST(LODA_R0, "*H'1BCD', R1, +", 0x0D, 0xBB, 0xCD);
    TEST(LODA_R0, "*H'0DEF', R1, -", 0x0D, 0xCD, 0xEF);
    TEST(LODA_R0, "*H'0F01', R1",    0x0D, 0xEF, 0x01);
    TEST(LODA_R2, "H'0123'",         0x0E, 0x01, 0x23);
    TEST(LODA_R0, "H'0345', R2, +",  0x0E, 0x23, 0x45);
    TEST(LODA_R0, "H'1567', R2, -",  0x0E, 0x55, 0x67);
    TEST(LODA_R0, "H'0789', R2",     0x0E, 0x67, 0x89);
    TEST(LODA_R2, "*H'09AB'",        0x0E, 0x89, 0xAB);
    TEST(LODA_R0, "*H'0BCD', R2, +", 0x0E, 0xAB, 0xCD);
    TEST(LODA_R0, "*H'1DEF', R2, -", 0x0E, 0xDD, 0xEF);
    TEST(LODA_R0, "*H'0F01', R2",    0x0E, 0xEF, 0x01);
    TEST(LODA_R3, "H'0123'",         0x0F, 0x01, 0x23);
    TEST(LODA_R0, "H'0345', R3, +",  0x0F, 0x23, 0x45);
    TEST(LODA_R0, "H'0567', R3, -",  0x0F, 0x45, 0x67);
    TEST(LODA_R0, "H'1789', R3",     0x0F, 0x77, 0x89);
    TEST(LODA_R3, "*H'09AB'",        0x0F, 0x89, 0xAB);
    TEST(LODA_R0, "*H'0BCD', R3, +", 0x0F, 0xAB, 0xCD);
    TEST(LODA_R0, "*H'0DEF', R3, -", 0x0F, 0xCD, 0xEF);
    TEST(LODA_R0, "*H'1F01', R3",    0x0F, 0xFF, 0x01);

    TEST(STRZ_R1, "",              0xC1);
    TEST(STRZ_R2, "",              0xC2);
    TEST(STRZ_R3, "",              0xC3);
    TEST(STRR_R0, "H'002B'",         0xC8, 0x29);
    TEST(STRR_R1, "*H'002C'",        0xC9, 0xAA);
    TEST(STRR_R2, "H'002D'",         0xCA, 0x2B);
    TEST(STRR_R3, "H'002E'",         0xCB, 0x2C);
    TEST(STRA_R0, "H'0123'",         0xCC, 0x01, 0x23);
    TEST(STRA_R0, "H'1345', R0, +",  0xCC, 0x33, 0x45);
    TEST(STRA_R0, "H'0567', R0, -",  0xCC, 0x45, 0x67);
    TEST(STRA_R0, "H'0789', R0",     0xCC, 0x67, 0x89);
    TEST(STRA_R0, "*H'09AB'",        0xCC, 0x89, 0xAB);
    TEST(STRA_R0, "*H'1BCD', R0, +", 0xCC, 0xBB, 0xCD);
    TEST(STRA_R0, "*H'0DEF', R0, -", 0xCC, 0xCD, 0xEF);
    TEST(STRA_R0, "*H'0F01', R0",    0xCC, 0xEF, 0x01);
    TEST(STRA_R1, "H'0123'",         0xCD, 0x01, 0x23);
    TEST(STRA_R0, "H'0345', R1, +",  0xCD, 0x23, 0x45);
    TEST(STRA_R0, "H'1567', R1, -",  0xCD, 0x55, 0x67);
    TEST(STRA_R0, "H'0789', R1",     0xCD, 0x67, 0x89);
    TEST(STRA_R1, "*H'09AB'",        0xCD, 0x89, 0xAB);
    TEST(STRA_R0, "*H'0BCD', R1, +", 0xCD, 0xAB, 0xCD);
    TEST(STRA_R0, "*H'1DEF', R1, -", 0xCD, 0xDD, 0xEF);
    TEST(STRA_R0, "*H'0F01', R1",    0xCD, 0xEF, 0x01);
    TEST(STRA_R2, "H'0123'",         0xCE, 0x01, 0x23);
    TEST(STRA_R0, "H'0345', R2, +",  0xCE, 0x23, 0x45);
    TEST(STRA_R0, "H'0567', R2, -",  0xCE, 0x45, 0x67);
    TEST(STRA_R0, "H'1789', R2",     0xCE, 0x77, 0x89);
    TEST(STRA_R2, "*H'09AB'",        0xCE, 0x89, 0xAB);
    TEST(STRA_R0, "*H'0BCD', R2, +", 0xCE, 0xAB, 0xCD);
    TEST(STRA_R0, "*H'0DEF', R2, -", 0xCE, 0xCD, 0xEF);
    TEST(STRA_R0, "*H'1F01', R2",    0xCE, 0xFF, 0x01);
    TEST(STRA_R3, "H'1123'",         0xCF, 0x11, 0x23);
    TEST(STRA_R0, "H'0345', R3, +",  0xCF, 0x23, 0x45);
    TEST(STRA_R0, "H'0567', R3, -",  0xCF, 0x45, 0x67);
    TEST(STRA_R0, "H'0789', R3",     0xCF, 0x67, 0x89);
    TEST(STRA_R3, "*H'19AB'",        0xCF, 0x99, 0xAB);
    TEST(STRA_R0, "*H'0BCD', R3, +", 0xCF, 0xAB, 0xCD);
    TEST(STRA_R0, "*H'0DEF', R3, -", 0xCF, 0xCD, 0xEF);
    TEST(STRA_R0, "*H'0F01', R3",    0xCF, 0xEF, 0x01);
}

void test_arithmetic() {
    TEST(ADDZ_R0, "",              0x80);
    TEST(ADDZ_R1, "",              0x81);
    TEST(ADDZ_R2, "",              0x82);
    TEST(ADDZ_R3, "",              0x83);
    TEST(ADDI_R0, "H'56'",           0x84, 0x56);
    TEST(ADDI_R1, "H'67'",           0x85, 0x67);
    TEST(ADDI_R2, "H'78'",           0x86, 0x78);
    TEST(ADDI_R3, "H'89'",           0x87, 0x89);
    TEST(ADDR_R0, "*H'000B'",        0x88, 0x89);
    TEST(ADDR_R1, "H'000C'",         0x89, 0x0A);
    TEST(ADDR_R2, "H'000D'",         0x8A, 0x0B);
    TEST(ADDR_R3, "H'000E'",         0x8B, 0x0C);
    TEST(ADDA_R0, "H'1123'",         0x8C, 0x11, 0x23);
    TEST(ADDA_R0, "H'0345', R0, +",  0x8C, 0x23, 0x45);
    TEST(ADDA_R0, "H'0567', R0, -",  0x8C, 0x45, 0x67);
    TEST(ADDA_R0, "H'0789', R0",     0x8C, 0x67, 0x89);
    TEST(ADDA_R0, "*H'19AB'",        0x8C, 0x99, 0xAB);
    TEST(ADDA_R0, "*H'0BCD', R0, +", 0x8C, 0xAB, 0xCD);
    TEST(ADDA_R0, "*H'0DEF', R0, -", 0x8C, 0xCD, 0xEF);
    TEST(ADDA_R0, "*H'0F01', R0",    0x8C, 0xEF, 0x01);
    TEST(ADDA_R1, "H'0123'",         0x8D, 0x01, 0x23);
    TEST(ADDA_R0, "H'1345', R1, +",  0x8D, 0x33, 0x45);
    TEST(ADDA_R0, "H'0567', R1, -",  0x8D, 0x45, 0x67);
    TEST(ADDA_R0, "H'0789', R1",     0x8D, 0x67, 0x89);
    TEST(ADDA_R1, "*H'09AB'",        0x8D, 0x89, 0xAB);
    TEST(ADDA_R0, "*H'1BCD', R1, +", 0x8D, 0xBB, 0xCD);
    TEST(ADDA_R0, "*H'0DEF', R1, -", 0x8D, 0xCD, 0xEF);
    TEST(ADDA_R0, "*H'0F01', R1",    0x8D, 0xEF, 0x01);
    TEST(ADDA_R2, "H'0123'",         0x8E, 0x01, 0x23);
    TEST(ADDA_R0, "H'0345', R2, +",  0x8E, 0x23, 0x45);
    TEST(ADDA_R0, "H'1567', R2, -",  0x8E, 0x55, 0x67);
    TEST(ADDA_R0, "H'0789', R2",     0x8E, 0x67, 0x89);
    TEST(ADDA_R2, "*H'09AB'",        0x8E, 0x89, 0xAB);
    TEST(ADDA_R0, "*H'0BCD', R2, +", 0x8E, 0xAB, 0xCD);
    TEST(ADDA_R0, "*H'1DEF', R2, -", 0x8E, 0xDD, 0xEF);
    TEST(ADDA_R0, "*H'0F01', R2",    0x8E, 0xEF, 0x01);
    TEST(ADDA_R3, "H'0123'",         0x8F, 0x01, 0x23);
    TEST(ADDA_R0, "H'0345', R3, +",  0x8F, 0x23, 0x45);
    TEST(ADDA_R0, "H'0567', R3, -",  0x8F, 0x45, 0x67);
    TEST(ADDA_R0, "H'1789', R3",     0x8F, 0x77, 0x89);
    TEST(ADDA_R3, "*H'09AB'",        0x8F, 0x89, 0xAB);
    TEST(ADDA_R0, "*H'0BCD', R3, +", 0x8F, 0xAB, 0xCD);
    TEST(ADDA_R0, "*H'0DEF', R3, -", 0x8F, 0xCD, 0xEF);
    TEST(ADDA_R0, "*H'1F01', R3",    0x8F, 0xFF, 0x01);

    TEST(SUBZ_R0, "",              0xA0);
    TEST(SUBZ_R1, "",              0xA1);
    TEST(SUBZ_R2, "",              0xA2);
    TEST(SUBZ_R3, "",              0xA3);
    TEST(SUBI_R0, "H'56'",         0xA4, 0x56);
    TEST(SUBI_R1, "H'67'",         0xA5, 0x67);
    TEST(SUBI_R2, "H'78'",         0xA6, 0x78);
    TEST(SUBI_R3, "H'89'",         0xA7, 0x89);
    TEST(SUBR_R0, "*H'000B'",      0xA8, 0x89);
    TEST(SUBR_R1, "H'000C'",       0xA9, 0x0A);
    TEST(SUBR_R2, "H'000D'",       0xAA, 0x0B);
    TEST(SUBR_R3, "H'000E'",       0xAB, 0x0C);
    TEST(SUBA_R0, "H'1123'",       0xAC, 0x11, 0x23);
    TEST(SUBA_R0, "H'0345', R0, +",  0xAC, 0x23, 0x45);
    TEST(SUBA_R0, "H'0567', R0, -",  0xAC, 0x45, 0x67);
    TEST(SUBA_R0, "H'0789', R0",    0xAC, 0x67, 0x89);
    TEST(SUBA_R0, "*H'19AB'",      0xAC, 0x99, 0xAB);
    TEST(SUBA_R0, "*H'0BCD', R0, +", 0xAC, 0xAB, 0xCD);
    TEST(SUBA_R0, "*H'0DEF', R0, -", 0xAC, 0xCD, 0xEF);
    TEST(SUBA_R0, "*H'0F01', R0",   0xAC, 0xEF, 0x01);
    TEST(SUBA_R1, "H'0123'",       0xAD, 0x01, 0x23);
    TEST(SUBA_R0, "H'1345', R1, +",  0xAD, 0x33, 0x45);
    TEST(SUBA_R0, "H'0567', R1, -",  0xAD, 0x45, 0x67);
    TEST(SUBA_R0, "H'0789', R1",    0xAD, 0x67, 0x89);
    TEST(SUBA_R1, "*H'09AB'",      0xAD, 0x89, 0xAB);
    TEST(SUBA_R0, "*H'1BCD', R1, +", 0xAD, 0xBB, 0xCD);
    TEST(SUBA_R0, "*H'0DEF', R1, -", 0xAD, 0xCD, 0xEF);
    TEST(SUBA_R0, "*H'0F01', R1",   0xAD, 0xEF, 0x01);
    TEST(SUBA_R2, "H'0123'",       0xAE, 0x01, 0x23);
    TEST(SUBA_R0, "H'0345', R2, +",  0xAE, 0x23, 0x45);
    TEST(SUBA_R0, "H'1567', R2, -",  0xAE, 0x55, 0x67);
    TEST(SUBA_R0, "H'0789', R2",    0xAE, 0x67, 0x89);
    TEST(SUBA_R2, "*H'09AB'",      0xAE, 0x89, 0xAB);
    TEST(SUBA_R0, "*H'0BCD', R2, +", 0xAE, 0xAB, 0xCD);
    TEST(SUBA_R0, "*H'1DEF', R2, -", 0xAE, 0xDD, 0xEF);
    TEST(SUBA_R0, "*H'0F01', R2",   0xAE, 0xEF, 0x01);
    TEST(SUBA_R3, "H'0123'",       0xAF, 0x01, 0x23);
    TEST(SUBA_R0, "H'0345', R3, +",  0xAF, 0x23, 0x45);
    TEST(SUBA_R0, "H'0567', R3, -",  0xAF, 0x45, 0x67);
    TEST(SUBA_R0, "H'1789', R3",    0xAF, 0x77, 0x89);
    TEST(SUBA_R3, "*H'09AB'",      0xAF, 0x89, 0xAB);
    TEST(SUBA_R0, "*H'0BCD', R3, +", 0xAF, 0xAB, 0xCD);
    TEST(SUBA_R0, "*H'0DEF', R3, -", 0xAF, 0xCD, 0xEF);
    TEST(SUBA_R0, "*H'1F01', R3",   0xAF, 0xFF, 0x01);

    TEST(DAR_R0, "", 0x94);
    TEST(DAR_R1, "", 0x95);
    TEST(DAR_R2, "", 0x96);
    TEST(DAR_R3, "", 0x97);
}

void test_logical() {
    TEST(ANDZ_R1, "",             0x41);
    TEST(ANDZ_R2, "",             0x42);
    TEST(ANDZ_R3, "",             0x43);
    TEST(ANDI_R0, "H'56'",         0x44, 0x56);
    TEST(ANDI_R1, "H'67'",         0x45, 0x67);
    TEST(ANDI_R2, "H'78'",         0x46, 0x78);
    TEST(ANDI_R3, "H'89'",         0x47, 0x89);
    TEST(ANDR_R0, "*H'000B'",      0x48, 0x89);
    TEST(ANDR_R1, "H'000C'",       0x49, 0x0A);
    TEST(ANDR_R2, "H'000D'",       0x4A, 0x0B);
    TEST(ANDR_R3, "H'000E'",       0x4B, 0x0C);
    TEST(ANDA_R0, "H'1123'",       0x4C, 0x11, 0x23);
    TEST(ANDA_R0, "H'0345', R0, +",  0x4C, 0x23, 0x45);
    TEST(ANDA_R0, "H'0567', R0, -",  0x4C, 0x45, 0x67);
    TEST(ANDA_R0, "H'0789', R0",    0x4C, 0x67, 0x89);
    TEST(ANDA_R0, "*H'19AB'",      0x4C, 0x99, 0xAB);
    TEST(ANDA_R0, "*H'0BCD', R0, +", 0x4C, 0xAB, 0xCD);
    TEST(ANDA_R0, "*H'0DEF', R0, -", 0x4C, 0xCD, 0xEF);
    TEST(ANDA_R0, "*H'0F01', R0",   0x4C, 0xEF, 0x01);
    TEST(ANDA_R1, "H'0123'",       0x4D, 0x01, 0x23);
    TEST(ANDA_R0, "H'1345', R1, +",  0x4D, 0x33, 0x45);
    TEST(ANDA_R0, "H'0567', R1, -",  0x4D, 0x45, 0x67);
    TEST(ANDA_R0, "H'0789', R1",    0x4D, 0x67, 0x89);
    TEST(ANDA_R1, "*H'09AB'",      0x4D, 0x89, 0xAB);
    TEST(ANDA_R0, "*H'1BCD', R1, +", 0x4D, 0xBB, 0xCD);
    TEST(ANDA_R0, "*H'0DEF', R1, -", 0x4D, 0xCD, 0xEF);
    TEST(ANDA_R0, "*H'0F01', R1",   0x4D, 0xEF, 0x01);
    TEST(ANDA_R2, "H'0123'",       0x4E, 0x01, 0x23);
    TEST(ANDA_R0, "H'0345', R2, +",  0x4E, 0x23, 0x45);
    TEST(ANDA_R0, "H'1567', R2, -",  0x4E, 0x55, 0x67);
    TEST(ANDA_R0, "H'0789', R2",    0x4E, 0x67, 0x89);
    TEST(ANDA_R2, "*H'09AB'",      0x4E, 0x89, 0xAB);
    TEST(ANDA_R0, "*H'0BCD', R2, +", 0x4E, 0xAB, 0xCD);
    TEST(ANDA_R0, "*H'1DEF', R2, -", 0x4E, 0xDD, 0xEF);
    TEST(ANDA_R0, "*H'0F01', R2",   0x4E, 0xEF, 0x01);
    TEST(ANDA_R3, "H'0123'",       0x4F, 0x01, 0x23);
    TEST(ANDA_R0, "H'0345', R3, +",  0x4F, 0x23, 0x45);
    TEST(ANDA_R0, "H'0567', R3, -",  0x4F, 0x45, 0x67);
    TEST(ANDA_R0, "H'1789', R3",    0x4F, 0x77, 0x89);
    TEST(ANDA_R3, "*H'09AB'",      0x4F, 0x89, 0xAB);
    TEST(ANDA_R0, "*H'0BCD', R3, +", 0x4F, 0xAB, 0xCD);
    TEST(ANDA_R0, "*H'0DEF', R3, -", 0x4F, 0xCD, 0xEF);
    TEST(ANDA_R0, "*H'1F01', R3",   0x4F, 0xFF, 0x01);

    TEST(LODZ_R0, "",             0x60);
    TEST(IORZ_R1, "",             0x61);
    TEST(IORZ_R2, "",             0x62);
    TEST(IORZ_R3, "",             0x63);
    TEST(IORI_R0, "H'56'",         0x64, 0x56);
    TEST(IORI_R1, "H'67'",         0x65, 0x67);
    TEST(IORI_R2, "H'78'",         0x66, 0x78);
    TEST(IORI_R3, "H'89'",         0x67, 0x89);
    TEST(IORR_R0, "*H'000B'",      0x68, 0x89);
    TEST(IORR_R1, "H'000C'",       0x69, 0x0A);
    TEST(IORR_R2, "H'000D'",       0x6A, 0x0B);
    TEST(IORR_R3, "H'000E'",       0x6B, 0x0C);
    TEST(IORA_R0, "H'1123'",       0x6C, 0x11, 0x23);
    TEST(IORA_R0, "H'0345', R0, +",  0x6C, 0x23, 0x45);
    TEST(IORA_R0, "H'0567', R0, -",  0x6C, 0x45, 0x67);
    TEST(IORA_R0, "H'0789', R0",    0x6C, 0x67, 0x89);
    TEST(IORA_R0, "*H'19AB'",      0x6C, 0x99, 0xAB);
    TEST(IORA_R0, "*H'0BCD', R0, +", 0x6C, 0xAB, 0xCD);
    TEST(IORA_R0, "*H'0DEF', R0, -", 0x6C, 0xCD, 0xEF);
    TEST(IORA_R0, "*H'0F01', R0",   0x6C, 0xEF, 0x01);
    TEST(IORA_R1, "H'0123'",       0x6D, 0x01, 0x23);
    TEST(IORA_R0, "H'1345', R1, +",  0x6D, 0x33, 0x45);
    TEST(IORA_R0, "H'0567', R1, -",  0x6D, 0x45, 0x67);
    TEST(IORA_R0, "H'0789', R1",    0x6D, 0x67, 0x89);
    TEST(IORA_R1, "*H'09AB'",      0x6D, 0x89, 0xAB);
    TEST(IORA_R0, "*H'1BCD', R1, +", 0x6D, 0xBB, 0xCD);
    TEST(IORA_R0, "*H'0DEF', R1, -", 0x6D, 0xCD, 0xEF);
    TEST(IORA_R0, "*H'0F01', R1",   0x6D, 0xEF, 0x01);
    TEST(IORA_R2, "H'0123'",       0x6E, 0x01, 0x23);
    TEST(IORA_R0, "H'0345', R2, +",  0x6E, 0x23, 0x45);
    TEST(IORA_R0, "H'1567', R2, -",  0x6E, 0x55, 0x67);
    TEST(IORA_R0, "H'0789', R2",    0x6E, 0x67, 0x89);
    TEST(IORA_R2, "*H'09AB'",      0x6E, 0x89, 0xAB);
    TEST(IORA_R0, "*H'0BCD', R2, +", 0x6E, 0xAB, 0xCD);
    TEST(IORA_R0, "*H'1DEF', R2, -", 0x6E, 0xDD, 0xEF);
    TEST(IORA_R0, "*H'0F01', R2",   0x6E, 0xEF, 0x01);
    TEST(IORA_R3, "H'0123'",       0x6F, 0x01, 0x23);
    TEST(IORA_R0, "H'0345', R3, +",  0x6F, 0x23, 0x45);
    TEST(IORA_R0, "H'0567', R3, -",  0x6F, 0x45, 0x67);
    TEST(IORA_R0, "H'1789', R3",    0x6F, 0x77, 0x89);
    TEST(IORA_R3, "*H'09AB'",      0x6F, 0x89, 0xAB);
    TEST(IORA_R0, "*H'0BCD', R3, +", 0x6F, 0xAB, 0xCD);
    TEST(IORA_R0, "*H'0DEF', R3, -", 0x6F, 0xCD, 0xEF);
    TEST(IORA_R0, "*H'1F01', R3",   0x6F, 0xFF, 0x01);

    TEST(EORZ_R0, "",             0x20);
    TEST(EORZ_R1, "",             0x21);
    TEST(EORZ_R2, "",             0x22);
    TEST(EORZ_R3, "",             0x23);
    TEST(EORI_R0, "H'56'",         0x24, 0x56);
    TEST(EORI_R1, "H'67'",         0x25, 0x67);
    TEST(EORI_R2, "H'78'",         0x26, 0x78);
    TEST(EORI_R3, "H'89'",         0x27, 0x89);
    TEST(EORR_R0, "*H'000B'",      0x28, 0x89);
    TEST(EORR_R1, "H'000C'",       0x29, 0x0A);
    TEST(EORR_R2, "H'000D'",       0x2A, 0x0B);
    TEST(EORR_R3, "H'000E'",       0x2B, 0x0C);
    TEST(EORA_R0, "H'1123'",       0x2C, 0x11, 0x23);
    TEST(EORA_R0, "H'0345', R0, +",  0x2C, 0x23, 0x45);
    TEST(EORA_R0, "H'0567', R0, -",  0x2C, 0x45, 0x67);
    TEST(EORA_R0, "H'0789', R0",    0x2C, 0x67, 0x89);
    TEST(EORA_R0, "*H'19AB'",      0x2C, 0x99, 0xAB);
    TEST(EORA_R0, "*H'0BCD', R0, +", 0x2C, 0xAB, 0xCD);
    TEST(EORA_R0, "*H'0DEF', R0, -", 0x2C, 0xCD, 0xEF);
    TEST(EORA_R0, "*H'0F01', R0",   0x2C, 0xEF, 0x01);
    TEST(EORA_R1, "H'0123'",       0x2D, 0x01, 0x23);
    TEST(EORA_R0, "H'1345', R1, +",  0x2D, 0x33, 0x45);
    TEST(EORA_R0, "H'0567', R1, -",  0x2D, 0x45, 0x67);
    TEST(EORA_R0, "H'0789', R1",    0x2D, 0x67, 0x89);
    TEST(EORA_R1, "*H'09AB'",      0x2D, 0x89, 0xAB);
    TEST(EORA_R0, "*H'1BCD', R1, +", 0x2D, 0xBB, 0xCD);
    TEST(EORA_R0, "*H'0DEF', R1, -", 0x2D, 0xCD, 0xEF);
    TEST(EORA_R0, "*H'0F01', R1",   0x2D, 0xEF, 0x01);
    TEST(EORA_R2, "H'0123'",       0x2E, 0x01, 0x23);
    TEST(EORA_R0, "H'0345', R2, +",  0x2E, 0x23, 0x45);
    TEST(EORA_R0, "H'1567', R2, -",  0x2E, 0x55, 0x67);
    TEST(EORA_R0, "H'0789', R2",    0x2E, 0x67, 0x89);
    TEST(EORA_R2, "*H'09AB'",      0x2E, 0x89, 0xAB);
    TEST(EORA_R0, "*H'0BCD', R2, +", 0x2E, 0xAB, 0xCD);
    TEST(EORA_R0, "*H'1DEF', R2, -", 0x2E, 0xDD, 0xEF);
    TEST(EORA_R0, "*H'0F01', R2",   0x2E, 0xEF, 0x01);
    TEST(EORA_R3, "H'0123'",       0x2F, 0x01, 0x23);
    TEST(EORA_R0, "H'0345', R3, +",  0x2F, 0x23, 0x45);
    TEST(EORA_R0, "H'0567', R3, -",  0x2F, 0x45, 0x67);
    TEST(EORA_R0, "H'1789', R3",    0x2F, 0x77, 0x89);
    TEST(EORA_R3, "*H'09AB'",      0x2F, 0x89, 0xAB);
    TEST(EORA_R0, "*H'0BCD', R3, +", 0x2F, 0xAB, 0xCD);
    TEST(EORA_R0, "*H'0DEF', R3, -", 0x2F, 0xCD, 0xEF);
    TEST(EORA_R0, "*H'1F01', R3",   0x2F, 0xFF, 0x01);
}

void test_rotate_compare() {
    TEST(COMZ_R0, "",             0xE0);
    TEST(COMZ_R1, "",             0xE1);
    TEST(COMZ_R2, "",             0xE2);
    TEST(COMZ_R3, "",             0xE3);
    TEST(COMI_R0, "H'56'",         0xE4, 0x56);
    TEST(COMI_R1, "H'67'",         0xE5, 0x67);
    TEST(COMI_R2, "H'78'",         0xE6, 0x78);
    TEST(COMI_R3, "H'89'",         0xE7, 0x89);
    TEST(COMR_R0, "*H'000B'",      0xE8, 0x89);
    TEST(COMR_R1, "H'000C'",       0xE9, 0x0A);
    TEST(COMR_R2, "H'000D'",       0xEA, 0x0B);
    TEST(COMR_R3, "H'000E'",       0xEB, 0x0C);
    TEST(COMA_R0, "H'1123'",       0xEC, 0x11, 0x23);
    TEST(COMA_R0, "H'0345', R0, +",  0xEC, 0x23, 0x45);
    TEST(COMA_R0, "H'0567', R0, -",  0xEC, 0x45, 0x67);
    TEST(COMA_R0, "H'0789', R0",    0xEC, 0x67, 0x89);
    TEST(COMA_R0, "*H'19AB'",      0xEC, 0x99, 0xAB);
    TEST(COMA_R0, "*H'0BCD', R0, +", 0xEC, 0xAB, 0xCD);
    TEST(COMA_R0, "*H'0DEF', R0, -", 0xEC, 0xCD, 0xEF);
    TEST(COMA_R0, "*H'0F01', R0",   0xEC, 0xEF, 0x01);
    TEST(COMA_R1, "H'0123'",       0xED, 0x01, 0x23);
    TEST(COMA_R0, "H'1345', R1, +",  0xED, 0x33, 0x45);
    TEST(COMA_R0, "H'0567', R1, -",  0xED, 0x45, 0x67);
    TEST(COMA_R0, "H'0789', R1",    0xED, 0x67, 0x89);
    TEST(COMA_R1, "*H'09AB'",      0xED, 0x89, 0xAB);
    TEST(COMA_R0, "*H'1BCD', R1, +", 0xED, 0xBB, 0xCD);
    TEST(COMA_R0, "*H'0DEF', R1, -", 0xED, 0xCD, 0xEF);
    TEST(COMA_R0, "*H'0F01', R1",   0xED, 0xEF, 0x01);
    TEST(COMA_R2, "H'0123'",       0xEE, 0x01, 0x23);
    TEST(COMA_R0, "H'0345', R2, +",  0xEE, 0x23, 0x45);
    TEST(COMA_R0, "H'1567', R2, -",  0xEE, 0x55, 0x67);
    TEST(COMA_R0, "H'0789', R2",    0xEE, 0x67, 0x89);
    TEST(COMA_R2, "*H'09AB'",      0xEE, 0x89, 0xAB);
    TEST(COMA_R0, "*H'0BCD', R2, +", 0xEE, 0xAB, 0xCD);
    TEST(COMA_R0, "*H'1DEF', R2, -", 0xEE, 0xDD, 0xEF);
    TEST(COMA_R0, "*H'0F01', R2",   0xEE, 0xEF, 0x01);
    TEST(COMA_R3, "H'0123'",       0xEF, 0x01, 0x23);
    TEST(COMA_R0, "H'0345', R3, +",  0xEF, 0x23, 0x45);
    TEST(COMA_R0, "H'0567', R3, -",  0xEF, 0x45, 0x67);
    TEST(COMA_R0, "H'1789', R3",    0xEF, 0x77, 0x89);
    TEST(COMA_R3, "*H'09AB'",      0xEF, 0x89, 0xAB);
    TEST(COMA_R0, "*H'0BCD', R3, +", 0xEF, 0xAB, 0xCD);
    TEST(COMA_R0, "*H'0DEF', R3, -", 0xEF, 0xCD, 0xEF);
    TEST(COMA_R0, "*H'1F01', R3",   0xEF, 0xFF, 0x01);

    TEST(RRR_R0, "", 0x50);
    TEST(RRR_R1, "", 0x51);
    TEST(RRR_R2, "", 0x52);
    TEST(RRR_R3, "", 0x53);
    TEST(RRL_R0, "", 0xD0);
    TEST(RRL_R1, "", 0xD1);
    TEST(RRL_R2, "", 0xD2);
    TEST(RRL_R3, "", 0xD3);
}

void test_branch() {
    ATEST(0x1000, BCTR_EQ, "H'101B'",  0x18, 0x19);
    ATEST(0x1000, BCTR_EQ, "*H'101C'", 0x18, 0x9A);
    ATEST(0x1000, BCTR_GT, "H'101C'",  0x19, 0x1A);
    ATEST(0x1000, BCTR_GT, "*H'102D'", 0x19, 0xAB);
    ATEST(0x1000, BCTR_LT, "H'101D'",  0x1A, 0x1B);
    ATEST(0x1000, BCTR_LT, "*H'103E'", 0x1A, 0xBC);
    ATEST(0x1000, BCTR_UN, "H'101E'",  0x1B, 0x1C);
    ATEST(0x1000, BCTR_UN, "*H'0FCF'", 0x1B, 0xCD);
    ATEST(0x1000, BCTA_EQ, "H'7D1E'",  0x1C, 0x7D, 0x1E);
    ATEST(0x2000, BCTA_EQ, "*H'5EF0'", 0x1C, 0xDE, 0xF0);
    ATEST(0x3000, BCTA_GT, "H'1E1F'",  0x1D, 0x1E, 0x1F);
    ATEST(0x4000, BCTA_GT, "*H'6F01'", 0x1D, 0xEF, 0x01);
    ATEST(0x5000, BCTA_LT, "H'1F20'",  0x1E, 0x1F, 0x20);
    ATEST(0x6000, BCTA_LT, "*H'7012'", 0x1E, 0xF0, 0x12);
    ATEST(0x7000, BCTA_UN, "H'2021'",  0x1F, 0x20, 0x21);
    ATEST(0x0000, BCTA_UN, "*H'49AB'", 0x1F, 0xC9, 0xAB);

    ATEST(0x1000, BCFR_EQ, "H'101B'",  0x98, 0x19);
    ATEST(0x1000, BCFR_EQ, "*H'101C'", 0x98, 0x9A);
    ATEST(0x1000, BCFR_GT, "H'101C'",  0x99, 0x1A);
    ATEST(0x1000, BCFR_GT, "*H'102D'", 0x99, 0xAB);
    ATEST(0x1000, BCFR_LT, "H'101D'",  0x9A, 0x1B);
    ATEST(0x1000, BCFR_LT, "*H'103E'", 0x9A, 0xBC);
    ATEST(0x7000, BCFA_EQ, "H'1D1E'",  0x9C, 0x1D, 0x1E);
    ATEST(0x6000, BCFA_EQ, "*H'7EF0'", 0x9C, 0xFE, 0xF0);
    ATEST(0x5000, BCFA_GT, "H'1E1F'",  0x9D, 0x1E, 0x1F);
    ATEST(0x4000, BCFA_GT, "*H'6F01'", 0x9D, 0xEF, 0x01);
    ATEST(0x3000, BCFA_LT, "H'1F20'",  0x9E, 0x1F, 0x20);
    ATEST(0x2000, BCFA_LT, "*H'7012'", 0x9E, 0xF0, 0x12);
    ATEST(0x1000, BRNR_R0, "H'101B'",  0x58, 0x19);
    ATEST(0x1000, BRNR_R0, "*H'101C'", 0x58, 0x9A);
    ATEST(0x1000, BRNR_R1, "H'101C'",  0x59, 0x1A);
    ATEST(0x1000, BRNR_R1, "*H'102D'", 0x59, 0xAB);
    ATEST(0x1000, BRNR_R2, "H'101D'",  0x5A, 0x1B);
    ATEST(0x1000, BRNR_R2, "*H'103E'", 0x5A, 0xBC);
    ATEST(0x1000, BRNR_R3, "H'101E'",  0x5B, 0x1C);
    ATEST(0x1000, BRNR_R3, "*H'0FCF'", 0x5B, 0xCD);
    ATEST(0x1000, BRNA_R0, "H'5D5E'",  0x5C, 0x5D, 0x5E);
    ATEST(0x2000, BRNA_R0, "*H'5EF0'", 0x5C, 0xDE, 0xF0);
    ATEST(0x3000, BRNA_R1, "H'5E5F'",  0x5D, 0x5E, 0x5F);
    ATEST(0x4000, BRNA_R1, "*H'6F01'", 0x5D, 0xEF, 0x01);
    ATEST(0x5000, BRNA_R2, "H'5F60'",  0x5E, 0x5F, 0x60);
    ATEST(0x6000, BRNA_R2, "*H'7012'", 0x5E, 0xF0, 0x12);
    ATEST(0x7000, BRNA_R3, "H'6061'",  0x5F, 0x60, 0x61);
    ATEST(0x0000, BRNA_R3, "*H'09AB'", 0x5F, 0x89, 0xAB);

    ATEST(0x1000, BIRR_R0, "H'101B'",  0xD8, 0x19);
    ATEST(0x1000, BIRR_R0, "*H'101C'", 0xD8, 0x9A);
    ATEST(0x1000, BIRR_R1, "H'101C'",  0xD9, 0x1A);
    ATEST(0x1000, BIRR_R1, "*H'102D'", 0xD9, 0xAB);
    ATEST(0x1000, BIRR_R2, "H'101D'",  0xDA, 0x1B);
    ATEST(0x1000, BIRR_R2, "*H'103E'", 0xDA, 0xBC);
    ATEST(0x1000, BIRR_R3, "H'101E'",  0xDB, 0x1C);
    ATEST(0x1000, BIRR_R3, "*H'0FCF'", 0xDB, 0xCD);
    ATEST(0x0000, BIRA_R0, "H'5D5E'",  0xDC, 0x5D, 0x5E);
    ATEST(0x1000, BIRA_R0, "*H'6F01'", 0xDC, 0xEF, 0x01);
    ATEST(0x2000, BIRA_R1, "H'5E5F'",  0xDD, 0x5E, 0x5F);
    ATEST(0x3000, BIRA_R1, "*H'5EDF'", 0xDD, 0xDE, 0xDF);
    ATEST(0x4000, BIRA_R2, "H'5F60'",  0xDE, 0x5F, 0x60);
    ATEST(0x6000, BIRA_R2, "*H'7012'", 0xDE, 0xF0, 0x12);
    ATEST(0x6000, BIRA_R3, "H'0123'",  0xDF, 0x01, 0x23);
    ATEST(0x7000, BIRA_R3, "*H'5FE0'", 0xDF, 0xDF, 0xE0);

    ATEST(0x1000, BDRR_R0, "H'101B'",  0xF8, 0x19);
    ATEST(0x1000, BDRR_R0, "*H'101C'", 0xF8, 0x9A);
    ATEST(0x1000, BDRR_R1, "H'101C'",  0xF9, 0x1A);
    ATEST(0x1000, BDRR_R1, "*H'102D'", 0xF9, 0xAB);
    ATEST(0x1000, BDRR_R2, "H'101D'",  0xFA, 0x1B);
    ATEST(0x1000, BDRR_R2, "*H'103E'", 0xFA, 0xBC);
    ATEST(0x1000, BDRR_R3, "H'101E'",  0xFB, 0x1C);
    ATEST(0x1000, BDRR_R3, "*H'0FCF'", 0xFB, 0xCD);
    ATEST(0x7000, BDRA_R0, "H'5D5E'",  0xFC, 0x5D, 0x5E);
    ATEST(0x6000, BDRA_R0, "*H'7DFE'", 0xFC, 0xFD, 0xFE);
    ATEST(0x5000, BDRA_R1, "H'5E5F'",  0xFD, 0x5E, 0x5F);
    ATEST(0x4000, BDRA_R1, "*H'7EFF'", 0xFD, 0xFE, 0xFF);
    ATEST(0x3000, BDRA_R2, "H'5F60'",  0xFE, 0x5F, 0x60);
    ATEST(0x2000, BDRA_R2, "*H'5CBA'", 0xFE, 0xDC, 0xBA);
    ATEST(0x1000, BDRA_R3, "H'0011'",  0xFF, 0x00, 0x11);
    ATEST(0x0000, BDRA_R3, "*H'7EFD'", 0xFF, 0xFE, 0xFD);

    ATEST(0x1000, ZBRR, "H'003F'",  0x9B, 0x3F);
    ATEST(0x1000, ZBRR, "H'1FC0'",  0x9B, 0x40);
    ATEST(0x1000, ZBRR, "*H'003F'", 0x9B, 0xBF);
    ATEST(0x1000, ZBRR, "*H'1FC0'", 0x9B, 0xC0);

    ATEST(0x1000, BXA, "H'0123', R3",  0x9F, 0x01, 0x23);
    ATEST(0x1000, BXA, "H'7FFF', R3",  0x9F, 0x7F, 0xFF);
    ATEST(0x1000, BXA, "*H'7FFF', R3", 0x9F, 0xFF, 0xFF);
}

void test_subroutine() {
    ATEST(0x1000, BSTR_EQ, "H'101B'",  0x38, 0x19);
    ATEST(0x1000, BSTR_EQ, "*H'101C'", 0x38, 0x9A);
    ATEST(0x1000, BSTR_GT, "H'101C'",  0x39, 0x1A);
    ATEST(0x1000, BSTR_GT, "*H'102D'", 0x39, 0xAB);
    ATEST(0x1000, BSTR_LT, "H'101D'",  0x3A, 0x1B);
    ATEST(0x1000, BSTR_LT, "*H'103E'", 0x3A, 0xBC);
    ATEST(0x1000, BSTR_UN, "H'101E'",  0x3B, 0x1C);
    ATEST(0x1000, BSTR_UN, "*H'0FCF'", 0x3B, 0xCD);
    ATEST(0x1000, BSTA_EQ, "H'7D1E'",  0x3C, 0x7D, 0x1E);
    ATEST(0x2000, BSTA_EQ, "*H'5EF0'", 0x3C, 0xDE, 0xF0);
    ATEST(0x3000, BSTA_GT, "H'1E1F'",  0x3D, 0x1E, 0x1F);
    ATEST(0x4000, BSTA_GT, "*H'6F01'", 0x3D, 0xEF, 0x01);
    ATEST(0x5000, BSTA_LT, "H'1F20'",  0x3E, 0x1F, 0x20);
    ATEST(0x6000, BSTA_LT, "*H'7012'", 0x3E, 0xF0, 0x12);
    ATEST(0x7000, BSTA_UN, "H'2021'",  0x3F, 0x20, 0x21);
    ATEST(0x0000, BSTA_UN, "*H'49AB'", 0x3F, 0xC9, 0xAB);

    ATEST(0x1000, BSFR_EQ, "H'101B'",  0xB8, 0x19);
    ATEST(0x1000, BSFR_EQ, "*H'101C'", 0xB8, 0x9A);
    ATEST(0x1000, BSFR_GT, "H'101C'",  0xB9, 0x1A);
    ATEST(0x1000, BSFR_GT, "*H'102D'", 0xB9, 0xAB);
    ATEST(0x1000, BSFR_LT, "H'101D'",  0xBA, 0x1B);
    ATEST(0x1000, BSFR_LT, "*H'103E'", 0xBA, 0xBC);
    ATEST(0x7000, BSFA_EQ, "H'1D1E'",  0xBC, 0x1D, 0x1E);
    ATEST(0x6000, BSFA_EQ, "*H'7EF0'", 0xBC, 0xFE, 0xF0);
    ATEST(0x5000, BSFA_GT, "H'1E1F'",  0xBD, 0x1E, 0x1F);
    ATEST(0x4000, BSFA_GT, "*H'6F01'", 0xBD, 0xEF, 0x01);
    ATEST(0x3000, BSFA_LT, "H'1F20'",  0xBE, 0x1F, 0x20);
    ATEST(0x2000, BSFA_LT, "*H'7012'", 0xBE, 0xF0, 0x12);

    ATEST(0x1000, BSNR_R0, "H'101B'",  0x78, 0x19);
    ATEST(0x1000, BSNR_R0, "*H'101C'", 0x78, 0x9A);
    ATEST(0x1000, BSNR_R1, "H'101C'",  0x79, 0x1A);
    ATEST(0x1000, BSNR_R1, "*H'102D'", 0x79, 0xAB);
    ATEST(0x1000, BSNR_R2, "H'101D'",  0x7A, 0x1B);
    ATEST(0x1000, BSNR_R2, "*H'103E'", 0x7A, 0xBC);
    ATEST(0x1000, BSNR_R3, "H'101E'",  0x7B, 0x1C);
    ATEST(0x1000, BSNR_R3, "*H'0FCF'", 0x7B, 0xCD);
    ATEST(0x1000, BSNA_R0, "H'5D5E'",  0x7C, 0x5D, 0x5E);
    ATEST(0x2000, BSNA_R0, "*H'5EF0'", 0x7C, 0xDE, 0xF0);
    ATEST(0x3000, BSNA_R1, "H'5E5F'",  0x7D, 0x5E, 0x5F);
    ATEST(0x4000, BSNA_R1, "*H'6F01'", 0x7D, 0xEF, 0x01);
    ATEST(0x5000, BSNA_R2, "H'5F60'",  0x7E, 0x5F, 0x60);
    ATEST(0x6000, BSNA_R2, "*H'7012'", 0x7E, 0xF0, 0x12);
    ATEST(0x7000, BSNA_R3, "H'6061'",  0x7F, 0x60, 0x61);
    ATEST(0x0000, BSNA_R3, "*H'09AB'", 0x7F, 0x89, 0xAB);


    ATEST(0x1000, ZBSR, "H'003F'", 0xBB, 0x3F);
    ATEST(0x1000, ZBSR, "H'1FC0'", 0xBB, 0x40);
    ATEST(0x1000, ZBSR, "*H'003F'", 0xBB, 0xBF);
    ATEST(0x1000, ZBSR, "*H'1FC0'", 0xBB, 0xC0);

    ATEST(0x1000, BSXA, "H'0123', R3",  0xBF, 0x01, 0x23);
    ATEST(0x1000, BSXA, "H'7FFF', R3",  0xBF, 0x7F, 0xFF);
    ATEST(0x1000, BSXA, "*H'7FFF', R3", 0xBF, 0xFF, 0xFF);

    TEST(RETC_EQ, "", 0x14);
    TEST(RETC_GT, "", 0x15);
    TEST(RETC_LT, "", 0x16);
    TEST(RETC_UN, "", 0x17);
    TEST(RETE_EQ, "", 0x34);
    TEST(RETE_GT, "", 0x35);
    TEST(RETE_LT, "", 0x36);
    TEST(RETE_UN, "", 0x37);
}

void test_io() {
    TEST(WRTD_R0, "", 0xF0);
    TEST(WRTD_R1, "", 0xF1);
    TEST(WRTD_R2, "", 0xF2);
    TEST(WRTD_R3, "", 0xF3);
    TEST(REDD_R0, "", 0x70);
    TEST(REDD_R1, "", 0x71);
    TEST(REDD_R2, "", 0x72);
    TEST(REDD_R3, "", 0x73);

    TEST(WRTC_R0, "", 0xB0);
    TEST(WRTC_R1, "", 0xB1);
    TEST(WRTC_R2, "", 0xB2);
    TEST(WRTC_R3, "", 0xB3);
    TEST(REDC_R0, "", 0x30);
    TEST(REDC_R1, "", 0x31);
    TEST(REDC_R2, "", 0x32);
    TEST(REDC_R3, "", 0x33);

    TEST(WRTE_R0, "H'D5'", 0xD4, 0xD5);
    TEST(WRTE_R1, "H'D6'", 0xD5, 0xD6);
    TEST(WRTE_R2, "H'D7'", 0xD6, 0xD7);
    TEST(WRTE_R3, "H'D8'", 0xD7, 0xD8);
    TEST(REDE_R0, "H'55'",  0x54, 0x55);
    TEST(REDE_R1, "H'56'",  0x55, 0x56);
    TEST(REDE_R2, "H'57'",  0x56, 0x57);
    TEST(REDE_R3, "H'58'",  0x57, 0x58);
}

void test_misc() {
    TEST(HALT, "", 0x40);
    TEST(NOP, "",  0xC0);
    TEST(TMI_R0, "H'F5'", 0xF4, 0xF5);
    TEST(TMI_R1, "H'F6'", 0xF5, 0xF6);
    TEST(TMI_R2, "H'F7'", 0xF6, 0xF7);
    TEST(TMI_R3, "H'F8'", 0xF7, 0xF8);

    TEST(LPSU, "",      0x92);
    TEST(LPSL, "",      0x93);
    TEST(CPSU, "H'75'", 0x74, 0x75);
    TEST(CPSL, "H'76'", 0x75, 0x76);
    TEST(PPSU, "H'77'", 0x76, 0x77);
    TEST(PPSL, "H'78'", 0x77, 0x78);
    TEST(TPSU, "H'B5'", 0xB4, 0xB5);
    TEST(TPSL, "H'B6'", 0xB5, 0xB6);
}

static void test_illegal() {
    ERRI(0x00);
    ERRI(0x10);
    ERRI(0x11);
    ERRI(0x90);
    ERRI(0x91);
}
// clang-format on

void run_tests(const char *cpu) {
    disassembler.setCpu(cpu);

    RUN_TEST(test_load_store);
    RUN_TEST(test_arithmetic);
    RUN_TEST(test_logical);
    RUN_TEST(test_rotate_compare);
    RUN_TEST(test_branch);
    RUN_TEST(test_subroutine);
    RUN_TEST(test_io);
    RUN_TEST(test_misc);
    RUN_TEST(test_illegal);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

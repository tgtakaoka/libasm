/*
 * Copyright 2025 Tadashi G. Takaoka
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

#include "dis_tms320f.h"
#include "test_dis_helper.h"

using namespace libasm;
using namespace libasm::tms320f;
using namespace libasm::test;

DisTms320f dis320f;
Disassembler &disassembler(dis320f);

void set_up() {
    disassembler.reset();
}

void tear_down() {
    symtab.reset();
}

void test_cpu() {
    EQUALS("cpu 320C3X", true, disassembler.setCpu("320C3X"));
    EQUALS_P("cpu 320C3X", "320C3X", disassembler.config().cpu_P());

    EQUALS("cpu TMS320C3X", true, disassembler.setCpu("TMS320C3X"));
    EQUALS_P("cpu TMS320C3X", "320C3X", disassembler.config().cpu_P());
}

// clang-format off
void test_load() {
    TEST("LDE", "R0, R1",       0x06810000);
    ERRT("LDE", "AR0, R1", ILLEGAL_REGISTER, "AR0, R1", 0x06810008);
    TEST("LDE", "@000001H, R2",   0x06A20001);
    TEST("LDE", "*+AR1(2), R3",   0x06C30102);
    TEST("LDE", "*-AR2(3), R4",   0x06C40A03);
    TEST("LDE", "*++AR3(4), R5",  0x06C51304);
    TEST("LDE", "*--AR4(5), R6",  0x06C61C05);
    TEST("LDE", "*AR5++(6), R7",  0x06C72506);
    TEST("LDE", "*AR6--(7), R0",  0x06C02E07);
    TEST("LDE", "*AR7++(8)%, R1", 0x06C13708);
    TEST("LDE", "*AR0--(9)%, R2", 0x06C23809);
    TEST("LDE", "*+AR1(IR0), R3",   0x06C34100);
    TEST("LDE", "*-AR2(IR0), R4",   0x06C44A00);
    TEST("LDE", "*++AR3(IR0), R5",  0x06C55300);
    TEST("LDE", "*--AR4(IR0), R6",  0x06C65C00);
    TEST("LDE", "*AR5++(IR0), R7",  0x06C76500);
    TEST("LDE", "*AR6--(IR0), R0",  0x06C06E00);
    TEST("LDE", "*AR7++(IR0)%, R1", 0x06C17700);
    TEST("LDE", "*AR0--(IR0)%, R2", 0x06C27800);
    TEST("LDE", "*+AR1(IR1), R3",   0x06C38100);
    TEST("LDE", "*-AR2(IR1), R4",   0x06C48A00);
    TEST("LDE", "*++AR3(IR1), R5",  0x06C59300);
    TEST("LDE", "*--AR4(IR1), R6",  0x06C69C00);
    TEST("LDE", "*AR5++(IR1), R7",  0x06C7A500);
    TEST("LDE", "*AR6--(IR1), R0",  0x06C0AE00);
    TEST("LDE", "*AR7++(IR1)%, R1", 0x06C1B700);
    TEST("LDE", "*AR0--(IR1)%, R2", 0x06C2B800);
    TEST("LDE", "*AR1, R3",         0x06C3C100);
    TEST("LDE", "*AR2++(IR0)B, R4", 0x06C4CA00);
    TEST("LDE", "0FFH, R5",         0x06E500FF);
    ERRT("LDE", "0100H, R6", OVERFLOW_RANGE, "0100H, R6", 0x06E60100);

    TEST("LDF", "R0, R1",       0x07010000);
    ERRT("LDF", "AR0, R1", ILLEGAL_REGISTER, "AR0, R1", 0x07010008);
    TEST("LDF", "@000001H, R2",   0x07220001);
    TEST("LDF", "*+AR1(2), R3",   0x07430102);
    TEST("LDF", "*-AR2(3), R4",   0x07440A03);
    TEST("LDF", "*++AR3(4), R5",  0x07451304);
    TEST("LDF", "*--AR4(5), R6",  0x07461C05);
    TEST("LDF", "*AR5++(6), R7",  0x07472506);
    TEST("LDF", "*AR6--(7), R0",  0x07402E07);
    TEST("LDF", "*AR7++(8)%, R1", 0x07413708);
    TEST("LDF", "*AR0--(9)%, R2", 0x07423809);
    TEST("LDF", "*+AR1(IR0), R3",   0x07434100);
    TEST("LDF", "*-AR2(IR0), R4",   0x07444A00);
    TEST("LDF", "*++AR3(IR0), R5",  0x07455300);
    TEST("LDF", "*--AR4(IR0), R6",  0x07465C00);
    TEST("LDF", "*AR5++(IR0), R7",  0x07476500);
    TEST("LDF", "*AR6--(IR0), R0",  0x07406E00);
    TEST("LDF", "*AR7++(IR0)%, R1", 0x07417700);
    TEST("LDF", "*AR0--(IR0)%, R2", 0x07427800);
    TEST("LDF", "*+AR1(IR1), R3",   0x07438100);
    TEST("LDF", "*-AR2(IR1), R4",   0x07448A00);
    TEST("LDF", "*++AR3(IR1), R5",  0x07459300);
    TEST("LDF", "*--AR4(IR1), R6",  0x07469C00);
    TEST("LDF", "*AR5++(IR1), R7",  0x0747A500);
    TEST("LDF", "*AR6--(IR1), R0",  0x0740AE00);
    TEST("LDF", "*AR7++(IR1)%, R1", 0x0741B700);
    TEST("LDF", "*AR0--(IR1)%, R2", 0x0742B800);
    TEST("LDF", "*AR1, R3",         0x0743C100);
    TEST("LDF", "*AR2++(IR0)B, R4", 0x0744CA00);
    TEST("LDF", "0FFH, R5",         0x076500FF);
    ERRT("LDF", "0100H, R6", OVERFLOW_RANGE, "0100H, R6", 0x07660100);

    TEST("LDI",  "R0, R0", 0x08000000);
}

void test_store() {
}

void test_2op() {
}

void test_3op() {
}

void test_program() {
}

void test_misc() {
    TEST("IDLE2",    "", 0x06000001);
    TEST("LOPOWER",  "", 0x10800001);
    TEST("MAXSPEED", "", 0x10800000);
    TEST("SIGI",     "", 0x16000000);
}

// clang-format on

void run_tests(const char *cpu) {
    disassembler.setCpu(cpu);
    RUN_TEST(test_load);
    RUN_TEST(test_store);
    RUN_TEST(test_2op);
    RUN_TEST(test_3op);
    RUN_TEST(test_program);
    RUN_TEST(test_misc);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

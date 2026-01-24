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

#define __PVASSERT(file, line, addr, name1, opr1, err1, at1, name2, opr2, err2, at2, ...) \
    do {                                                                                  \
        const Config::opcode_t codes[] = {__VA_ARGS__};                                   \
        const auto endian = disassembler.config().endian();                               \
        const auto unit = disassembler.config().addressUnit();                            \
        const ArrayMemory memory(addr, codes, sizeof(codes), endian, unit);               \
        Insn insn(memory.origin());                                                       \
        ErrorAt error;                                                                    \
        error.setError(at1, err1);                                                        \
        dis_assert(file, line, error, memory, name1, opr1, insn);                         \
        asserter.isTrue(file, line, "1st parallel", insn.hasContinue());                  \
        error.setError(at2, err2);                                                        \
        dis_assert(file, line, error, memory, name2, opr2, insn);                         \
        asserter.isFalse(file, line, "2nd parallel", insn.hasContinue());                 \
    } while (0)
#define PVASSERT(addr, name1, opr1, err1, at1, name2, opr2, err2, at2, ...) \
    __PVASSERT(                                                             \
            __FILE__, __LINE__, addr, name1, opr1, err1, at1, name2, opr2, err2, at2, __VA_ARGS__)
#define PERRT(name1, opr1, err1, at1, name2, opr2, err2, at2, ...) \
    PVASSERT(0, name1, opr1, err1, at1, name2, opr2, err2, at2, __VA_ARGS__)
#define PTEST(name1, opr1, name2, opr2, ...) \
    PERRT(name1, opr1, OK, "", name2, opr2, OK, "", __VA_ARGS__)

using namespace libasm;
using namespace libasm::tms320f;
using namespace libasm::test;

DisTms320f dis320f;
Disassembler &disassembler(dis320f);

bool is320c32() {
    return strcasecmp_P("320C32", disassembler.config().cpu_P()) == 0;
}

bool is320c31() {
    return strcasecmp_P("320C31", disassembler.config().cpu_P()) == 0 || is320c32();
}

void set_up() {
    disassembler.reset();
}

void tear_down() {
    symtab.reset();
}

void test_cpu() {
    EQUALS("cpu 320C30", true, disassembler.setCpu("320C30"));
    EQUALS_P("cpu 320C30", "320C30", disassembler.config().cpu_P());

    EQUALS("cpu 320C31", true, disassembler.setCpu("320C31"));
    EQUALS_P("cpu 320C31", "320C31", disassembler.config().cpu_P());

    EQUALS("cpu TMS320C30", true, disassembler.setCpu("TMS320C30"));
    EQUALS_P("cpu TMS320C30", "320C30", disassembler.config().cpu_P());

    EQUALS("cpu TMS320C31", true, disassembler.setCpu("TMS320C31"));
    EQUALS_P("cpu TMS320C31", "320C31", disassembler.config().cpu_P());
}

// clang-format off
void test_load() {
    TEST("LDE", "R0, R1",           0x06810000);
    ERRT("LDE", "AR0, R1", ILLEGAL_REGISTER, "AR0, R1", 0x06810008);
    TEST("LDE", "@000001H, R2",     0x06A20001);
    TEST("LDE", "*+AR1(2), R3",     0x06C30102);
    TEST("LDE", "*-AR2(3), R4",     0x06C40A03);
    TEST("LDE", "*++AR3, R5",       0x06C51301);
    TEST("LDE", "*++AR3(4), R5",    0x06C51304);
    TEST("LDE", "*--AR4, R6",       0x06C61C01);
    TEST("LDE", "*--AR4(5), R6",    0x06C61C05);
    TEST("LDE", "*AR5++, R7",       0x06C72501);
    TEST("LDE", "*AR5++(6), R7",    0x06C72506);
    TEST("LDE", "*AR6--, R0",       0x06C02E01);
    TEST("LDE", "*AR6--(7), R0",    0x06C02E07);
    TEST("LDE", "*AR7++(8)%, R1",   0x06C13708);
    TEST("LDE", "*AR7++%, R1",      0x06C13701);
    TEST("LDE", "*AR0--(9)%, R2",   0x06C23809);
    TEST("LDE", "*AR0--%, R2",      0x06C23801);
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
    TEST("LDE", "1, R5",            0x06E50000);
    TEST("LDE", "-1.5, R6",         0x06E60C00);
    TEST("LDE", "0, R7",            0x06E78000);
    TEST("LDE", "-0.50024, R0",     0x06E0FFFF);

    TEST("LDF", "R0, R1",           0x07010000);
    ERRT("LDF", "AR0, R1", ILLEGAL_REGISTER, "AR0, R1", 0x07010008);
    TEST("LDF", "@000001H, R2",     0x07220001);
    TEST("LDF", "*+AR1(2), R3",     0x07430102);
    TEST("LDF", "*-AR2(3), R4",     0x07440A03);
    TEST("LDF", "*++AR3, R5",       0x07451301);
    TEST("LDF", "*++AR3(4), R5",    0x07451304);
    TEST("LDF", "*--AR4, R6",       0x07461C01);
    TEST("LDF", "*--AR4(5), R6",    0x07461C05);
    TEST("LDF", "*AR5++, R7",       0x07472501);
    TEST("LDF", "*AR5++(6), R7",    0x07472506);
    TEST("LDF", "*AR6--, R0",       0x07402E01);
    TEST("LDF", "*AR6--(7), R0",    0x07402E07);
    TEST("LDF", "*AR7++(8)%, R1",   0x07413708);
    TEST("LDF", "*AR7++%, R1",      0x07413701);
    TEST("LDF", "*AR0--(9)%, R2",   0x07423809);
    TEST("LDF", "*AR0--%, R2",      0x07423801);
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
    TEST("LDF", "1, R5",            0x07650000);
    TEST("LDF", "-1.5, R6",         0x07660C00);
    TEST("LDF", "0, R7",            0x07678000);
    TEST("LDF", "-0.50024, R0",     0x0760FFFF);

    TEST("LDI", "R0, R1",           0x08010000);
    TEST("LDI", "AR0, R1",          0x08010008);
    TEST("LDI", "@000001H, R2",     0x08220001);
    TEST("LDI", "*+AR1(2), R3",     0x08430102);
    TEST("LDI", "*-AR2(3), R4",     0x08440A03);
    TEST("LDI", "*++AR3(4), R5",    0x08451304);
    TEST("LDI", "*--AR4(5), R6",    0x08461C05);
    TEST("LDI", "*AR5++(6), R7",    0x08472506);
    TEST("LDI", "*AR6--(7), R0",    0x08402E07);
    TEST("LDI", "*AR7++(8)%, R1",   0x08413708);
    TEST("LDI", "*AR0--(9)%, R2",   0x08423809);
    TEST("LDI", "*+AR1(IR0), R3",   0x08434100);
    TEST("LDI", "*-AR2(IR0), R4",   0x08444A00);
    TEST("LDI", "*++AR3(IR0), R5",  0x08455300);
    TEST("LDI", "*--AR4(IR0), R6",  0x08465C00);
    TEST("LDI", "*AR5++(IR0), R7",  0x08476500);
    TEST("LDI", "*AR6--(IR0), R0",  0x08406E00);
    TEST("LDI", "*AR7++(IR0)%, R1", 0x08417700);
    TEST("LDI", "*AR0--(IR0)%, R2", 0x08427800);
    TEST("LDI", "*+AR1(IR1), R3",   0x08438100);
    TEST("LDI", "*-AR2(IR1), R4",   0x08448A00);
    TEST("LDI", "*++AR3(IR1), R5",  0x08459300);
    TEST("LDI", "*--AR4(IR1), R6",  0x08469C00);
    TEST("LDI", "*AR5++(IR1), R7",  0x0847A500);
    TEST("LDI", "*AR6--(IR1), R0",  0x0840AE00);
    TEST("LDI", "*AR7++(IR1)%, R1", 0x0841B700);
    TEST("LDI", "*AR0--(IR1)%, R2", 0x0842B800);
    TEST("LDI", "*AR1, R3",         0x0843C100);
    TEST("LDI", "*AR2++(IR0)B, R4", 0x0844CA00);
    TEST("LDI", "0, R5",            0x08650000);
    TEST("LDI", "0C00H, R6",        0x08660C00);
    TEST("LDI", "-8000H, R7",       0x08678000);
    TEST("LDI", "-1, R0",           0x0860FFFF);

    TEST("LDP", "120000H",          0x08700012);
    ERRT("LDP", "230000H",
         OVERFLOW_RANGE, "230000H", 0x08700123);

    TEST("LDM", "R0, R1",           0x09010000);
    ERRT("LDM", "AR0, R1", ILLEGAL_REGISTER, "AR0, R1", 0x09010008);
    TEST("LDM", "@000001H, R2",     0x09220001);
    TEST("LDM", "*+AR1(2), R3",     0x09430102);
    TEST("LDM", "*-AR2(3), R4",     0x09440A03);
    TEST("LDM", "*++AR3, R5",       0x09451301);
    TEST("LDM", "*++AR3(4), R5",    0x09451304);
    TEST("LDM", "*--AR4, R6",       0x09461C01);
    TEST("LDM", "*--AR4(5), R6",    0x09461C05);
    TEST("LDM", "*AR5++, R7",       0x09472501);
    TEST("LDM", "*AR5++(6), R7",    0x09472506);
    TEST("LDM", "*AR6--, R0",       0x09402E01);
    TEST("LDM", "*AR6--(7), R0",    0x09402E07);
    TEST("LDM", "*AR7++(8)%, R1",   0x09413708);
    TEST("LDM", "*AR7++%, R1",      0x09413701);
    TEST("LDM", "*AR0--(9)%, R2",   0x09423809);
    TEST("LDM", "*AR0--%, R2",      0x09423801);
    TEST("LDM", "*+AR1(IR0), R3",   0x09434100);
    TEST("LDM", "*-AR2(IR0), R4",   0x09444A00);
    TEST("LDM", "*++AR3(IR0), R5",  0x09455300);
    TEST("LDM", "*--AR4(IR0), R6",  0x09465C00);
    TEST("LDM", "*AR5++(IR0), R7",  0x09476500);
    TEST("LDM", "*AR6--(IR0), R0",  0x09406E00);
    TEST("LDM", "*AR7++(IR0)%, R1", 0x09417700);
    TEST("LDM", "*AR0--(IR0)%, R2", 0x09427800);
    TEST("LDM", "*+AR1(IR1), R3",   0x09438100);
    TEST("LDM", "*-AR2(IR1), R4",   0x09448A00);
    TEST("LDM", "*++AR3(IR1), R5",  0x09459300);
    TEST("LDM", "*--AR4(IR1), R6",  0x09469C00);
    TEST("LDM", "*AR5++(IR1), R7",  0x0947A500);
    TEST("LDM", "*AR6--(IR1), R0",  0x0940AE00);
    TEST("LDM", "*AR7++(IR1)%, R1", 0x0941B700);
    TEST("LDM", "*AR0--(IR1)%, R2", 0x0942B800);
    TEST("LDM", "*AR1, R3",         0x0943C100);
    TEST("LDM", "*AR2++(IR0)B, R4", 0x0944CA00);
    TEST("LDM", "1.0005, R5",       0x09650001);
    TEST("LDM", "1.0088, R6",       0x09660012);
    TEST("LDM", "1.1421, R7",       0x09670123);
    ERRT("LDM", "2.5508, R6",
         ILLEGAL_OPERAND, "2.5508, R6", 0x09661234);

    TEST("POP", "R0",  0x0E200000);
    UNKN(0x0E200001);
    UNKN(0x0E200020);
    UNKN(0x0E200400);
    UNKN(0x0E208000);
    TEST("POP", "R1",  0x0E210000);
    TEST("POP", "R2",  0x0E220000);
    TEST("POP", "R3",  0x0E230000);
    TEST("POP", "R4",  0x0E240000);
    TEST("POP", "R5",  0x0E250000);
    TEST("POP", "R6",  0x0E260000);
    TEST("POP", "R7",  0x0E270000);
    TEST("POP", "AR0", 0x0E280000);
    TEST("POP", "AR1", 0x0E290000);
    TEST("POP", "AR2", 0x0E2A0000);
    TEST("POP", "AR3", 0x0E2B0000);
    TEST("POP", "AR4", 0x0E2C0000);
    TEST("POP", "AR5", 0x0E2D0000);
    TEST("POP", "AR6", 0x0E2E0000);
    TEST("POP", "AR7", 0x0E2F0000);
    TEST("POP", "DP",  0x0E300000);
    TEST("POP", "IR0", 0x0E310000);
    TEST("POP", "IR1", 0x0E320000);
    TEST("POP", "BK",  0x0E330000);
    TEST("POP", "SP",  0x0E340000);
    TEST("POP", "ST",  0x0E350000);
    TEST("POP", "IE",  0x0E360000);
    TEST("POP", "IF",  0x0E370000);
    TEST("POP", "IOF", 0x0E380000);
    TEST("POP", "RS",  0x0E390000);
    TEST("POP", "RE",  0x0E3A0000);
    TEST("POP", "RC",  0x0E3B0000);
    ERRT("POP", "",   ILLEGAL_REGISTER, "",  0x0E3C0000);
    ERRT("POP", "",   ILLEGAL_REGISTER, "",  0x0E3D0000);
    ERRT("POP", "",   ILLEGAL_REGISTER, "",  0x0E3E0000);
    ERRT("POP", "",   ILLEGAL_REGISTER, "",  0x0E3F0000);

    TEST("POPF", "R0",  0x0EA00000);
    UNKN(0x0EA00001);
    UNKN(0x0EA00020);
    UNKN(0x0EA00400);
    UNKN(0x0EA08000);
    TEST("POPF", "R1",  0x0EA10000);
    TEST("POPF", "R2",  0x0EA20000);
    TEST("POPF", "R3",  0x0EA30000);
    TEST("POPF", "R4",  0x0EA40000);
    TEST("POPF", "R5",  0x0EA50000);
    TEST("POPF", "R6",  0x0EA60000);
    TEST("POPF", "R7",  0x0EA70000);
    ERRT("POPF", "AR0", ILLEGAL_REGISTER, "AR0", 0x0EA80000);
    ERRT("POPF", "AR1", ILLEGAL_REGISTER, "AR1", 0x0EA90000);
    ERRT("POPF", "AR2", ILLEGAL_REGISTER, "AR2", 0x0EAA0000);
    ERRT("POPF", "AR3", ILLEGAL_REGISTER, "AR3", 0x0EAB0000);
    ERRT("POPF", "AR4", ILLEGAL_REGISTER, "AR4", 0x0EAC0000);
    ERRT("POPF", "AR5", ILLEGAL_REGISTER, "AR5", 0x0EAD0000);
    ERRT("POPF", "AR6", ILLEGAL_REGISTER, "AR6", 0x0EAE0000);
    ERRT("POPF", "AR7", ILLEGAL_REGISTER, "AR7", 0x0EAF0000);
    ERRT("POPF", "DP",  ILLEGAL_REGISTER, "DP",  0x0EB00000);
    ERRT("POPF", "IR0", ILLEGAL_REGISTER, "IR0", 0x0EB10000);
    ERRT("POPF", "IR1", ILLEGAL_REGISTER, "IR1", 0x0EB20000);
    ERRT("POPF", "BK",  ILLEGAL_REGISTER, "BK",  0x0EB30000);
    ERRT("POPF", "SP",  ILLEGAL_REGISTER, "SP",  0x0EB40000);
    ERRT("POPF", "ST",  ILLEGAL_REGISTER, "ST",  0x0EB50000);
    ERRT("POPF", "IE",  ILLEGAL_REGISTER, "IE",  0x0EB60000);
    ERRT("POPF", "IF",  ILLEGAL_REGISTER, "IF",  0x0EB70000);
    ERRT("POPF", "IOF", ILLEGAL_REGISTER, "IOF", 0x0EB80000);
    ERRT("POPF", "RS",  ILLEGAL_REGISTER, "RS",  0x0EB90000);
    ERRT("POPF", "RE",  ILLEGAL_REGISTER, "RE",  0x0EBA0000);
    ERRT("POPF", "RC",  ILLEGAL_REGISTER, "RC",  0x0EBB0000);
    ERRT("POPF", "",    ILLEGAL_REGISTER, "",    0x0EBC0000);
    ERRT("POPF", "",    ILLEGAL_REGISTER, "",    0x0EBD0000);
    ERRT("POPF", "",    ILLEGAL_REGISTER, "",    0x0EBE0000);
    ERRT("POPF", "",    ILLEGAL_REGISTER, "",    0x0EBF0000);

    TEST("LDFU",    "R0, R1",           0x40010000);
    TEST("LDFLO",   "R1, R2",           0x40820001);
    TEST("LDFLS",   "*+AR1(2), R3",     0x41430102);
    TEST("LDFHI",   "*-AR2(3), R4",     0x41C40A03);
    TEST("LDFHS",   "*++AR3(4), R5",    0x42451304);
    TEST("LDFEQ",   "*--AR4(5), R6",    0x42C61C05);
    TEST("LDFNE",   "*AR5++(6), R7",    0x43472506);
    TEST("LDFLT",   "*AR6--(7), R0",    0x43C02E07);
    TEST("LDFLE",   "*AR7++(8)%, R1",   0x44413708);
    TEST("LDFGT",   "*AR0--(9)%, R2",   0x44C23809);
    TEST("LDFGE",   "*+AR1(IR0), R3",   0x45434100);
    UNKN(                               0x45C44A00);
    TEST("LDFNV",   "*++AR3(IR0), R5",  0x46455300);
    TEST("LDFV",    "*--AR4(IR0), R6",  0x46C65C00);
    TEST("LDFNUF",  "*AR5++(IR0), R7",  0x47476500);
    TEST("LDFUF",   "*AR6--(IR0), R0",  0x47C06E00);
    TEST("LDFNLV",  "*AR7++(IR0)%, R1", 0x48417700);
    TEST("LDFLV",   "*AR0--(IR0)%, R2", 0x48C27800);
    TEST("LDFNLUF", "*+AR1(IR1), R3",   0x49438100);
    TEST("LDFLUF",  "*-AR2(IR1), R4",   0x49C48A00);
    TEST("LDFZUF",  "*++AR3(IR1), R5",  0x4A459300);
    TEST("LDFU",    "*--AR4(IR1), R6",  0x40469C00);
    TEST("LDFLO",   "*AR5++(IR1), R7",  0x40C7A500);
    TEST("LDFLS",   "*AR6--(IR1), R0",  0x4140AE00);
    TEST("LDFHI",   "*AR7++(IR1)%, R1", 0x41C1B700);
    TEST("LDFHS",   "*AR0--(IR1)%, R2", 0x4242B800);
    TEST("LDFEQ",   "*AR1, R3",         0x42C3C100);
    TEST("LDFNE",   "*AR2++(IR0)B, R4", 0x4344CA00);
    TEST("LDFLT",   "1, R5",            0x43E50000);
    TEST("LDFLE",   "-1.5, R6",         0x44660C00);
    TEST("LDFGT",   "0, R7",            0x44E78000);
    TEST("LDFGE",   "-0.50024, R0",     0x4560FFFF);

    TEST("LDIU",    "R0, R1",           0x50010000);
    TEST("LDILO",   "AR0, R1",          0x50810008);
    TEST("LDILS",   "@000001H, R2",     0x51220001);
    TEST("LDIHI",   "*+AR1(2), R3",     0x51C30102);
    TEST("LDIHS",   "*-AR2(3), R4",     0x52440A03);
    TEST("LDIEQ",   "*++AR3(4), R5",    0x52C51304);
    TEST("LDINE",   "*--AR4(5), R6",    0x53461C05);
    TEST("LDILT",   "*AR5++(6), R7",    0x53C72506);
    TEST("LDILE",   "*AR6--(7), R0",    0x54402E07);
    TEST("LDIGT",   "*AR7++(8)%, R1",   0x54C13708);
    TEST("LDIGE",   "*AR0--(9)%, R2",   0x55423809);
    UNKN(                               0x55C34100);
    TEST("LDINV",   "*-AR2(IR0), R4",   0x56444A00);
    TEST("LDIV",    "*++AR3(IR0), R5",  0x56C55300);
    TEST("LDINUF",  "*--AR4(IR0), R6",  0x57465C00);
    TEST("LDIUF",   "*AR5++(IR0), R7",  0x57C76500);
    TEST("LDINLV",  "*AR6--(IR0), R0",  0x58406E00);
    TEST("LDILV",   "*AR7++(IR0)%, R1", 0x58C17700);
    TEST("LDINLUF", "*AR0--(IR0)%, R2", 0x59427800);
    TEST("LDILUF",  "*+AR1(IR1), R3",   0x59C38100);
    TEST("LDIZUF",  "*-AR2(IR1), R4",   0x5A448A00);
    TEST("LDIU",    "*++AR3(IR1), R5",  0x50459300);
    TEST("LDILO",   "*--AR4(IR1), R6",  0x50C69C00);
    TEST("LDILS",   "*AR5++(IR1), R7",  0x5147A500);
    TEST("LDIHI",   "*AR6--(IR1), R0",  0x51C0AE00);
    TEST("LDIHS",   "*AR7++(IR1)%, R1", 0x5241B700);
    TEST("LDIEQ",   "*AR0--(IR1)%, R2", 0x52C2B800);
    TEST("LDINE",   "*AR1, R3",         0x5343C100);
    TEST("LDILT",   "*AR2++(IR0)B, R4", 0x53C4CA00);
    TEST("LDILE",   "0, R5",            0x54650000);
    TEST("LDIGT",   "-8000H, R6",       0x54E68000);
    TEST("LDIGE",   "-1, R7",           0x5567FFFF);
}

void test_store() {
    ERRT("STF", "R1, R0",  REGISTER_NOT_ALLOWED, "R0",  0x14010000);
    ERRT("STF", "R1, AR0", REGISTER_NOT_ALLOWED, "AR0", 0x14010008);
    TEST("STF", "R2, @000001H",     0x14220001);
    TEST("STF", "R3, *+AR1(2)",     0x14430102);
    TEST("STF", "R4, *-AR2(3)",     0x14440A03);
    TEST("STF", "R5, *++AR3(4)",    0x14451304);
    TEST("STF", "R6, *--AR4(5)",    0x14461C05);
    TEST("STF", "R7, *AR5++(6)",    0x14472506);
    TEST("STF", "R0, *AR6--(7)",    0x14402E07);
    TEST("STF", "R1, *AR7++(8)%",   0x14413708);
    TEST("STF", "R2, *AR0--(9)%",   0x14423809);
    TEST("STF", "R3, *+AR1(IR0)",   0x14434100);
    TEST("STF", "R4, *-AR2(IR0)",   0x14444A00);
    TEST("STF", "R5, *++AR3(IR0)",  0x14455300);
    TEST("STF", "R6, *--AR4(IR0)",  0x14465C00);
    TEST("STF", "R7, *AR5++(IR0)",  0x14476500);
    TEST("STF", "R0, *AR6--(IR0)",  0x14406E00);
    TEST("STF", "R1, *AR7++(IR0)%", 0x14417700);
    TEST("STF", "R2, *AR0--(IR0)%", 0x14427800);
    TEST("STF", "R3, *+AR1(IR1)",   0x14438100);
    TEST("STF", "R4, *-AR2(IR1)",   0x14448A00);
    TEST("STF", "R5, *++AR3(IR1)",  0x14459300);
    TEST("STF", "R6, *--AR4(IR1)",  0x14469C00);
    TEST("STF", "R7, *AR5++(IR1)",  0x1447A500);
    TEST("STF", "R0, *AR6--(IR1)",  0x1440AE00);
    TEST("STF", "R1, *AR7++(IR1)%", 0x1441B700);
    TEST("STF", "R2, *AR0--(IR1)%", 0x1442B800);
    TEST("STF", "R3, *AR1",         0x1443C100);
    TEST("STF", "R4, *AR2++(IR0)B", 0x1444CA00);
    ERRT("STF", "R5, 1",    ILLEGAL_OPERAND_MODE, "1",    0x14650000);
    ERRT("STF", "R6, -1.5", ILLEGAL_OPERAND_MODE, "-1.5", 0x14660C00);

    ERRT("STI", "R1, R0",  REGISTER_NOT_ALLOWED, "R0",   0x15010000);
    ERRT("STI", "R1, AR0", REGISTER_NOT_ALLOWED, "AR0",  0x15010008);
    TEST("STI", "R2, @000001H",     0x15220001);
    TEST("STI", "R3, *+AR1(2)",     0x15430102);
    TEST("STI", "R4, *-AR2(3)",     0x15440A03);
    TEST("STI", "R5, *++AR3(4)",    0x15451304);
    TEST("STI", "R6, *--AR4(5)",    0x15461C05);
    TEST("STI", "R7, *AR5++(6)",    0x15472506);
    TEST("STI", "R0, *AR6--(7)",    0x15402E07);
    TEST("STI", "R1, *AR7++(8)%",   0x15413708);
    TEST("STI", "R2, *AR0--(9)%",   0x15423809);
    TEST("STI", "R3, *+AR1(IR0)",   0x15434100);
    TEST("STI", "R4, *-AR2(IR0)",   0x15444A00);
    TEST("STI", "R5, *++AR3(IR0)",  0x15455300);
    TEST("STI", "R6, *--AR4(IR0)",  0x15465C00);
    TEST("STI", "R7, *AR5++(IR0)",  0x15476500);
    TEST("STI", "R0, *AR6--(IR0)",  0x15406E00);
    TEST("STI", "R1, *AR7++(IR0)%", 0x15417700);
    TEST("STI", "R2, *AR0--(IR0)%", 0x15427800);
    TEST("STI", "R3, *+AR1(IR1)",   0x15438100);
    TEST("STI", "R4, *-AR2(IR1)",   0x15448A00);
    TEST("STI", "R5, *++AR3(IR1)",  0x15459300);
    TEST("STI", "R6, *--AR4(IR1)",  0x15469C00);
    TEST("STI", "R7, *AR5++(IR1)",  0x1547A500);
    TEST("STI", "R0, *AR6--(IR1)",  0x1540AE00);
    TEST("STI", "R1, *AR7++(IR1)%", 0x1541B700);
    TEST("STI", "R2, *AR0--(IR1)%", 0x1542B800);
    TEST("STI", "R3, *AR1",         0x1543C100);
    TEST("STI", "R4, *AR2++(IR0)B", 0x1544CA00);
    ERRT("STI", "R5, 0",     ILLEGAL_OPERAND_MODE, "0",     0x15650000);
    ERRT("STI", "R6, 0C00H", ILLEGAL_OPERAND_MODE, "0C00H", 0x15660C00);

    TEST("PUSH", "R0",  0x0F200000);
    UNKN(0x0F200001);
    UNKN(0x0F200020);
    UNKN(0x0F200400);
    UNKN(0x0F208000);
    TEST("PUSH", "R1",  0x0F210000);
    TEST("PUSH", "R2",  0x0F220000);
    TEST("PUSH", "R3",  0x0F230000);
    TEST("PUSH", "R4",  0x0F240000);
    TEST("PUSH", "R5",  0x0F250000);
    TEST("PUSH", "R6",  0x0F260000);
    TEST("PUSH", "R7",  0x0F270000);
    TEST("PUSH", "AR0", 0x0F280000);
    TEST("PUSH", "AR1", 0x0F290000);
    TEST("PUSH", "AR2", 0x0F2A0000);
    TEST("PUSH", "AR3", 0x0F2B0000);
    TEST("PUSH", "AR4", 0x0F2C0000);
    TEST("PUSH", "AR5", 0x0F2D0000);
    TEST("PUSH", "AR6", 0x0F2E0000);
    TEST("PUSH", "AR7", 0x0F2F0000);
    TEST("PUSH", "DP",  0x0F300000);
    TEST("PUSH", "IR0", 0x0F310000);
    TEST("PUSH", "IR1", 0x0F320000);
    TEST("PUSH", "BK",  0x0F330000);
    TEST("PUSH", "SP",  0x0F340000);
    TEST("PUSH", "ST",  0x0F350000);
    TEST("PUSH", "IE",  0x0F360000);
    TEST("PUSH", "IF",  0x0F370000);
    TEST("PUSH", "IOF", 0x0F380000);
    TEST("PUSH", "RS",  0x0F390000);
    TEST("PUSH", "RE",  0x0F3A0000);
    TEST("PUSH", "RC",  0x0F3B0000);
    ERRT("PUSH", "",   ILLEGAL_REGISTER, "",  0x0F3C0000);
    ERRT("PUSH", "",   ILLEGAL_REGISTER, "",  0x0F3D0000);
    ERRT("PUSH", "",   ILLEGAL_REGISTER, "",  0x0F3E0000);
    ERRT("PUSH", "",   ILLEGAL_REGISTER, "",  0x0F3F0000);

    TEST("PUSHF", "R0",  0x0FA00000);
    UNKN(0x0FA00001);
    UNKN(0x0FA00020);
    UNKN(0x0FA00400);
    UNKN(0x0FA08000);
    TEST("PUSHF", "R1",  0x0FA10000);
    TEST("PUSHF", "R2",  0x0FA20000);
    TEST("PUSHF", "R3",  0x0FA30000);
    TEST("PUSHF", "R4",  0x0FA40000);
    TEST("PUSHF", "R5",  0x0FA50000);
    TEST("PUSHF", "R6",  0x0FA60000);
    TEST("PUSHF", "R7",  0x0FA70000);
    ERRT("PUSHF", "AR0", ILLEGAL_REGISTER, "AR0", 0x0FA80000);
    ERRT("PUSHF", "AR1", ILLEGAL_REGISTER, "AR1", 0x0FA90000);
    ERRT("PUSHF", "AR2", ILLEGAL_REGISTER, "AR2", 0x0FAA0000);
    ERRT("PUSHF", "AR3", ILLEGAL_REGISTER, "AR3", 0x0FAB0000);
    ERRT("PUSHF", "AR4", ILLEGAL_REGISTER, "AR4", 0x0FAC0000);
    ERRT("PUSHF", "AR5", ILLEGAL_REGISTER, "AR5", 0x0FAD0000);
    ERRT("PUSHF", "AR6", ILLEGAL_REGISTER, "AR6", 0x0FAE0000);
    ERRT("PUSHF", "AR7", ILLEGAL_REGISTER, "AR7", 0x0FAF0000);
    ERRT("PUSHF", "DP",  ILLEGAL_REGISTER, "DP",  0x0FB00000);
    ERRT("PUSHF", "IR0", ILLEGAL_REGISTER, "IR0", 0x0FB10000);
    ERRT("PUSHF", "IR1", ILLEGAL_REGISTER, "IR1", 0x0FB20000);
    ERRT("PUSHF", "BK",  ILLEGAL_REGISTER, "BK",  0x0FB30000);
    ERRT("PUSHF", "SP",  ILLEGAL_REGISTER, "SP",  0x0FB40000);
    ERRT("PUSHF", "ST",  ILLEGAL_REGISTER, "ST",  0x0FB50000);
    ERRT("PUSHF", "IE",  ILLEGAL_REGISTER, "IE",  0x0FB60000);
    ERRT("PUSHF", "IF",  ILLEGAL_REGISTER, "IF",  0x0FB70000);
    ERRT("PUSHF", "IOF", ILLEGAL_REGISTER, "IOF", 0x0FB80000);
    ERRT("PUSHF", "RS",  ILLEGAL_REGISTER, "RS",  0x0FB90000);
    ERRT("PUSHF", "RE",  ILLEGAL_REGISTER, "RE",  0x0FBA0000);
    ERRT("PUSHF", "RC",  ILLEGAL_REGISTER, "RC",  0x0FBB0000);
    ERRT("PUSHF", "",    ILLEGAL_REGISTER, "",    0x0FBC0000);
    ERRT("PUSHF", "",    ILLEGAL_REGISTER, "",    0x0FBD0000);
    ERRT("PUSHF", "",    ILLEGAL_REGISTER, "",    0x0FBE0000);
    ERRT("PUSHF", "",    ILLEGAL_REGISTER, "",    0x0FBF0000);
}

void test_2op() {
    TEST("ABSF", "R0, R1",       0x00010000);
    ERRT("ABSF", "AR0, R1", ILLEGAL_REGISTER, "AR0, R1", 0x00010008);
    TEST("ABSF", "1, R5",        0x00650000);
    TEST("ABSF", "-1.5, R6",     0x00660C00);
    TEST("ABSF", "0, R7",        0x00678000);
    TEST("ABSF", "-0.50024, R0", 0x0060FFFF);

    TEST("ABSI", "R0, R1",     0x00810000);
    TEST("ABSI", "AR0, R1",    0x00810008);
    TEST("ABSI", "0, R5",      0x00E50000);
    TEST("ABSI", "0C00H, R6",  0x00E60C00);
    TEST("ABSI", "-8000H, R5", 0x00E58000);
    TEST("ABSI", "-1, R7",     0x00E7FFFF);

    TEST("ADDC", "R0, R1",     0x01010000);
    TEST("ADDC", "R1, R1",     0x01010001);
    TEST("ADDC", "AR0, R1",    0x01010008);
    TEST("ADDC", "0, R5",      0x01650000);
    TEST("ADDC", "-8000H, R6", 0x01668000);
    TEST("ADDC", "-1, R7",     0x0167FFFF);

    TEST("ADDF", "R0, R1",   0x01810000);
    TEST("ADDF", "R1, R1",   0x01810001);
    ERRT("ADDF", "AR0, R1", ILLEGAL_REGISTER, "AR0, R1", 0x01810008);
    TEST("ADDF", "1, R5",    0x01E50000);
    TEST("ADDF", "-1.5, R6", 0x01E60C00);
    TEST("ADDF", "0, R7",    0x01E78000);

    TEST("ADDI", "R0, R1",     0x02010000);
    TEST("ADDI", "R1, R1",     0x02010001);
    TEST("ADDI", "AR0, R1",    0x02010008);
    TEST("ADDI", "0, R5",      0x02650000);
    TEST("ADDI", "-8000H, R6", 0x02668000);
    TEST("ADDI", "-1, R7",     0x0267FFFF);

    TEST("AND", "R0, R1",     0x02810000);
    TEST("AND", "R1, R1",     0x02810001);
    TEST("AND", "AR0, R1",    0x02810008);
    TEST("AND", "0, R5",      0x02E50000);
    TEST("AND", "8000H, R6",  0x02E68000);
    TEST("AND", "0FFFFH, R7", 0x02E7FFFF);

    TEST("ANDN", "R0, R1",     0x03010000);
    TEST("ANDN", "R1, R1",     0x03010001);
    TEST("ANDN", "AR0, R1",    0x03010008);
    TEST("ANDN", "0, R5",      0x03650000);
    TEST("ANDN", "8000H, R6",  0x03668000);
    TEST("ANDN", "0FFFFH, R7", 0x0367FFFF);

    TEST("ASH", "R0, R1",           0x03810000);
    TEST("ASH", "AR0, R1",          0x03810008);
    TEST("ASH", "@000001H, R2",     0x03A20001);
    TEST("ASH", "*+AR1(2), R3",     0x03C30102);
    TEST("ASH", "*-AR2(3), R4",     0x03C40A03);
    TEST("ASH", "*++AR3(4), R5",    0x03C51304);
    TEST("ASH", "*--AR4(5), R6",    0x03C61C05);
    TEST("ASH", "*AR5++(6), R7",    0x03C72506);
    TEST("ASH", "*AR6--(7), R0",    0x03C02E07);
    TEST("ASH", "*AR7++(8)%, R1",   0x03C13708);
    TEST("ASH", "*AR0--(9)%, R2",   0x03C23809);
    TEST("ASH", "*+AR1(IR0), R3",   0x03C34100);
    TEST("ASH", "*-AR2(IR0), R4",   0x03C44A00);
    TEST("ASH", "*++AR3(IR0), R5",  0x03C55300);
    TEST("ASH", "*--AR4(IR0), R6",  0x03C65C00);
    TEST("ASH", "*AR5++(IR0), R7",  0x03C76500);
    TEST("ASH", "*AR6--(IR0), R0",  0x03C06E00);
    TEST("ASH", "*AR7++(IR0)%, R1", 0x03C17700);
    TEST("ASH", "*AR0--(IR0)%, R2", 0x03C27800);
    TEST("ASH", "*+AR1(IR1), R3",   0x03C38100);
    TEST("ASH", "*-AR2(IR1), R4",   0x03C48A00);
    TEST("ASH", "*++AR3(IR1), R5",  0x03C59300);
    TEST("ASH", "*--AR4(IR1), R6",  0x03C69C00);
    TEST("ASH", "*AR5++(IR1), R7",  0x03C7A500);
    TEST("ASH", "*AR6--(IR1), R0",  0x03C0AE00);
    TEST("ASH", "*AR7++(IR1)%, R1", 0x03C1B700);
    TEST("ASH", "*AR0--(IR1)%, R2", 0x03C2B800);
    TEST("ASH", "*AR1, R3",         0x03C3C100);
    TEST("ASH", "*AR2++(IR0)B, R4", 0x03C4CA00);
    TEST("ASH", "0, R5",            0x03E50000);
    TEST("ASH", "-64, R6",          0x03E60040);
    TEST("ASH", "-64, R6",          0x03E6FFC0);
    TEST("ASH", "63, R7",           0x03E7803F);
    TEST("ASH", "-1, R0",           0x03E0007F);
    TEST("ASH", "-1, R0",           0x03E0FFFF);

    TEST("CMPF", "R0, R1",   0x04010000);
    TEST("CMPF", "R1, R1",   0x04010001);
    ERRT("CMPF", "AR0, R1", ILLEGAL_REGISTER, "AR0, R1", 0x04010008);
    TEST("CMPF", "1, R5",    0x04650000);
    TEST("CMPF", "-1.5, R6", 0x04660C00);
    TEST("CMPF", "0, R7",    0x04678000);

    TEST("CMPI", "R0, R1",     0x04810000);
    TEST("CMPI", "R1, R1",     0x04810001);
    TEST("CMPI", "AR0, R1",    0x04810008);
    TEST("CMPI", "0, R5",      0x04E50000);
    TEST("CMPI", "-8000H, R6", 0x04E68000);
    TEST("CMPI", "-1, R7",     0x04E7FFFF);

    TEST("FIX", "R0, R1",       0x05010000);
    ERRT("FIX", "AR0, R1", ILLEGAL_REGISTER, "AR0, R1", 0x05010008);
    TEST("FIX", "1, R5",        0x05650000);
    TEST("FIX", "-1.5, R6",     0x05660C00);
    TEST("FIX", "0, R7",        0x05678000);
    TEST("FIX", "-0.50024, R0", 0x0560FFFF);

    TEST("FLOAT", "R0, R1",     0x05810000);
    TEST("FLOAT", "AR0, R1",    0x05810008);
    TEST("FLOAT", "0, R5",      0x05E50000);
    TEST("FLOAT", "0C00H, R6",  0x05E60C00);
    TEST("FLOAT", "-8000H, R5", 0x05E58000);
    TEST("FLOAT", "-1, R7",     0x05E7FFFF);

    TEST("LSH", "R0, R1",           0x09810000);
    TEST("LSH", "AR0, R1",          0x09810008);
    TEST("LSH", "@000001H, R2",     0x09A20001);
    TEST("LSH", "*+AR1(2), R3",     0x09C30102);
    TEST("LSH", "*-AR2(3), R4",     0x09C40A03);
    TEST("LSH", "*++AR3(4), R5",    0x09C51304);
    TEST("LSH", "*--AR4(5), R6",    0x09C61C05);
    TEST("LSH", "*AR5++(6), R7",    0x09C72506);
    TEST("LSH", "*AR6--(7), R0",    0x09C02E07);
    TEST("LSH", "*AR7++(8)%, R1",   0x09C13708);
    TEST("LSH", "*AR0--(9)%, R2",   0x09C23809);
    TEST("LSH", "*+AR1(IR0), R3",   0x09C34100);
    TEST("LSH", "*-AR2(IR0), R4",   0x09C44A00);
    TEST("LSH", "*++AR3(IR0), R5",  0x09C55300);
    TEST("LSH", "*--AR4(IR0), R6",  0x09C65C00);
    TEST("LSH", "*AR5++(IR0), R7",  0x09C76500);
    TEST("LSH", "*AR6--(IR0), R0",  0x09C06E00);
    TEST("LSH", "*AR7++(IR0)%, R1", 0x09C17700);
    TEST("LSH", "*AR0--(IR0)%, R2", 0x09C27800);
    TEST("LSH", "*+AR1(IR1), R3",   0x09C38100);
    TEST("LSH", "*-AR2(IR1), R4",   0x09C48A00);
    TEST("LSH", "*++AR3(IR1), R5",  0x09C59300);
    TEST("LSH", "*--AR4(IR1), R6",  0x09C69C00);
    TEST("LSH", "*AR5++(IR1), R7",  0x09C7A500);
    TEST("LSH", "*AR6--(IR1), R0",  0x09C0AE00);
    TEST("LSH", "*AR7++(IR1)%, R1", 0x09C1B700);
    TEST("LSH", "*AR0--(IR1)%, R2", 0x09C2B800);
    TEST("LSH", "*AR1, R3",         0x09C3C100);
    TEST("LSH", "*AR2++(IR0)B, R4", 0x09C4CA00);
    TEST("LSH", "0, R5",            0x09E50000);
    TEST("LSH", "-64, R6",          0x09E60040);
    TEST("LSH", "63, R7",           0x09E7803F);
    TEST("LSH", "-1, R0",           0x09E0FFFF);

    TEST("MPYF", "R0, R1",   0x0A010000);
    TEST("MPYF", "R1, R1",   0x0A010001);
    ERRT("MPYF", "AR0, R1", ILLEGAL_REGISTER, "AR0, R1", 0x0A010008);
    TEST("MPYF", "1, R5",    0x0A650000);
    TEST("MPYF", "-1.5, R6", 0x0A660C00);
    TEST("MPYF", "0, R7",    0x0A678000);

    TEST("MPYI", "R0, R1",     0x0A810000);
    TEST("MPYI", "R1, R1",     0x0A810001);
    TEST("MPYI", "AR0, R1",    0x0A810008);
    TEST("MPYI", "0, R5",      0x0AE50000);
    TEST("MPYI", "-8000H, R6", 0x0AE68000);
    TEST("MPYI", "-1, R7",     0x0AE7FFFF);

    TEST("NEGB", "R0, R1",     0x0B010000);
    TEST("NEGB", "AR0, R1",    0x0B010008);
    TEST("NEGB", "0, R5",      0x0B650000);
    TEST("NEGB", "-8000H, R6", 0x0B668000);
    TEST("NEGB", "-1, R7",     0x0B67FFFF);

    TEST("NEGF", "R0, R1",       0x0B810000);
    ERRT("NEGF", "AR0, R1", ILLEGAL_REGISTER, "AR0, R1", 0x0B810008);
    TEST("NEGF", "1, R5",        0x0BE50000);
    TEST("NEGF", "-1.5, R6",     0x0BE60C00);
    TEST("NEGF", "0, R7",        0x0BE78000);
    TEST("NEGF", "-0.50024, R0", 0x0BE0FFFF);

    TEST("NEGI", "R0, R1",     0x0C010000);
    TEST("NEGI", "AR0, R1",    0x0C010008);
    TEST("NEGI", "0, R5",      0x0C650000);
    TEST("NEGI", "-8000H, R6", 0x0C668000);
    TEST("NEGI", "-1, R7",     0x0C67FFFF);

    TEST("NORM", "R0, R1",       0x0D010000);
    ERRT("NORM", "AR0, R1", ILLEGAL_REGISTER, "AR0, R1", 0x0D010008);
    TEST("NORM", "1, R5",        0x0D650000);
    TEST("NORM", "-1.5, R6",     0x0D660C00);
    TEST("NORM", "0, R7",        0x0D678000);
    TEST("NORM", "-0.50024, R0", 0x0D60FFFF);

    TEST("NOT", "R0, R1",     0x0D810000);
    TEST("NOT", "AR0, R1",    0x0D810008);
    TEST("NOT", "0, R5",      0x0DE50000);
    TEST("NOT", "8000H, R6",  0x0DE68000);
    TEST("NOT", "0FFFFH, R7", 0x0DE7FFFF);

    TEST("OR", "R0, R1",     0x10010000);
    TEST("OR", "R1, R1",     0x10010001);
    TEST("OR", "AR0, R1",    0x10010008);
    TEST("OR", "0, R5",      0x10650000);
    TEST("OR", "8000H, R6",  0x10668000);
    TEST("OR", "0FFFFH, R7", 0x1067FFFF);

    TEST("RND", "R0, R1",       0x11010000);
    ERRT("RND", "AR0, R1", ILLEGAL_REGISTER, "AR0, R1", 0x11010008);
    TEST("RND", "1, R5",        0x11650000);
    TEST("RND", "-1.5, R6",     0x11660C00);
    TEST("RND", "0, R7",        0x11678000);
    TEST("RND", "-0.50024, R0", 0x1160FFFF);

    TEST("ROL", "R0",  0x11E00001);
    TEST("ROL", "AR7", 0x11EF0001);
    TEST("ROL", "IR1", 0x11F20001);
    UNKN(0x11E00000);
    UNKN(0x11E00002);
    UNKN(0x11E00040);
    UNKN(0x11E00800);
    UNKN(0x11E0FFFF);

    TEST("ROLC", "R0",  0x12600001);
    TEST("ROLC", "AR7", 0x126F0001);
    TEST("ROLC", "IR1", 0x12720001);
    UNKN(0x12600000);
    UNKN(0x12600002);
    UNKN(0x12600040);
    UNKN(0x12600800);
    UNKN(0x1260FFFF);

    TEST("ROR", "R0",  0x12E0FFFF);
    TEST("ROR", "AR7", 0x12EFFFFF);
    TEST("ROR", "IR1", 0x12F2FFFF);
    UNKN(0x12E00000);
    UNKN(0x12E00001);
    UNKN(0x12E00020);
    UNKN(0x12E00400);
    UNKN(0x12E08000);

    TEST("RORC", "R0",  0x1360FFFF);
    TEST("RORC", "AR7", 0x136FFFFF);
    TEST("RORC", "IR1", 0x1372FFFF);
    UNKN(0x13600000);
    UNKN(0x13600001);
    UNKN(0x13600020);
    UNKN(0x13600400);
    UNKN(0x13608000);

    TEST("SUBB", "R0, R1",     0x16810000);
    TEST("SUBB", "R1, R1",     0x16810001);
    TEST("SUBB", "AR0, R1",    0x16810008);
    TEST("SUBB", "0, R5",      0x16E50000);
    TEST("SUBB", "-8000H, R6", 0x16E68000);
    TEST("SUBB", "-1, R7",     0x16E7FFFF);

    TEST("SUBC", "R0, R1",     0x17010000);
    TEST("SUBC", "R1, R1",     0x17010001);
    TEST("SUBC", "AR0, R1",    0x17010008);
    TEST("SUBC", "0, R5",      0x17650000);
    TEST("SUBC", "8000H, R6",  0x17668000);
    TEST("SUBC", "0FFFFH, R7", 0x1767FFFF);

    TEST("SUBF", "R0, R1",   0x17810000);
    TEST("SUBF", "R1, R1",   0x17810001);
    ERRT("SUBF", "AR0, R1", ILLEGAL_REGISTER, "AR0, R1", 0x17810008);
    TEST("SUBF", "1, R5",    0x17E50000);
    TEST("SUBF", "-1.5, R6", 0x17E60C00);
    TEST("SUBF", "0, R7",    0x17E78000);

    TEST("SUBI", "R0, R1",     0x18010000);
    TEST("SUBI", "R1, R1",     0x18010001);
    TEST("SUBI", "AR0, R1",    0x18010008);
    TEST("SUBI", "0, R5",      0x18650000);
    TEST("SUBI", "-8000H, R6", 0x18668000);
    TEST("SUBI", "-1, R7",     0x1867FFFF);

    TEST("SUBRB", "R0, R1",     0x18810000);
    TEST("SUBRB", "R1, R1",     0x18810001);
    TEST("SUBRB", "AR0, R1",    0x18810008);
    TEST("SUBRB", "0, R5",      0x18E50000);
    TEST("SUBRB", "-8000H, R6", 0x18E68000);
    TEST("SUBRB", "-1, R7",     0x18E7FFFF);

    TEST("SUBRF", "R0, R1",   0x19010000);
    TEST("SUBRF", "R1, R1",   0x19010001);
    ERRT("SUBRF", "AR0, R1", ILLEGAL_REGISTER, "AR0, R1", 0x19010008);
    TEST("SUBRF", "1, R5",    0x19650000);
    TEST("SUBRF", "-1.5, R6", 0x19660C00);
    TEST("SUBRF", "0, R7",    0x19678000);

    TEST("SUBRI", "R0, R1",     0x19810000);
    TEST("SUBRI", "R1, R1",     0x19810001);
    TEST("SUBRI", "AR0, R1",    0x19810008);
    TEST("SUBRI", "0, R5",      0x19E50000);
    TEST("SUBRI", "-8000H, R6", 0x19E68000);
    TEST("SUBRI", "-1, R7",     0x19E7FFFF);

    TEST("TSTB", "R0, R1",     0x1A010000);
    TEST("TSTB", "R1, R1",     0x1A010001);
    TEST("TSTB", "AR0, R1",    0x1A010008);
    TEST("TSTB", "0, R5",      0x1A650000);
    TEST("TSTB", "8000H, R6",  0x1A668000);
    TEST("TSTB", "0FFFFH, R7", 0x1A67FFFF);

    TEST("XOR", "R0, R1",     0x1A810000);
    TEST("XOR", "R1, R1",     0x1A810001);
    TEST("XOR", "AR0, R1",    0x1A810008);
    TEST("XOR", "0, R5",      0x1AE50000);
    TEST("XOR", "8000H, R6",  0x1AE68000);
    TEST("XOR", "0FFFFH, R7", 0x1AE7FFFF);


    // Unary operator
    disassembler.setOption("gnu-as", "enable");
    TEST("ABSF", "R1, R1",  0x00010001);
    TEST("ABSI", "R1, R1",  0x00810001);
    TEST("FIX", "R1, R1",   0x05010001);
    TEST("FLOAT", "R1, R1", 0x05810001);
    TEST("NEGB", "R1, R1",  0x0B010001);
    TEST("NEGF", "R1, R1",  0x0B810001);
    TEST("NEGI", "R1, R1",  0x0C010001);
    TEST("NORM", "R1, R1",  0x0D010001);
    TEST("NOT", "R1, R1",   0x0D810001);
    TEST("RND", "R1, R1",   0x11010001);
    disassembler.setOption("gnu-as", "disable");
    TEST("ABSF", "R1",  0x00010001);
    TEST("ABSI", "R1",  0x00810001);
    TEST("FIX", "R1",   0x05010001);
    TEST("FLOAT", "R1", 0x05810001);
    TEST("NEGB", "R1",  0x0B010001);
    TEST("NEGF", "R1",  0x0B810001);
    TEST("NEGI", "R1",  0x0C010001);
    TEST("NORM", "R1",  0x0D010001);
    TEST("NOT", "R1",   0x0D810001);
    TEST("RND", "R1",   0x11010001);
}

void test_3op() {
    TEST("ADDC3", "R1, AR0, R2", 0x20020801);
    TEST("ADDC3", "AR1, R0, R2", 0x20020009);
    TEST("ADDC3", "R1, R0, AR2", 0x200A0001);
    TEST("ADDC3", "R2, *+AR1, R3",        0x20230102);
    TEST("ADDC3", "R3, *-AR2, R4",        0x20240A03);
    TEST("ADDC3", "R4, *++AR3, R5",       0x20251304);
    TEST("ADDC3", "R5, *--AR4, R6",       0x20261C05);
    TEST("ADDC3", "R6, *AR5++, R7",       0x20272506);
    TEST("ADDC3", "R7, *AR6--, R0",       0x20202E07);
    TEST("ADDC3", "R0, *AR7++%, R1",      0x20213700);
    TEST("ADDC3", "R1, *AR0--%, R2",      0x20223801);
    TEST("ADDC3", "R2, *+AR1(IR0), R4",   0x20244102);
    TEST("ADDC3", "R3, *-AR2(IR0), R5",   0x20254A03);
    TEST("ADDC3", "R4, *++AR3(IR0), R6",  0x20265304);
    TEST("ADDC3", "R5, *--AR4(IR0), R7",  0x20275C05);
    TEST("ADDC3", "R6, *AR5++(IR0), R0",  0x20206506);
    TEST("ADDC3", "R7, *AR6--(IR0), R1",  0x20216E07);
    TEST("ADDC3", "R0, *AR7++(IR0)%, R2", 0x20227700);
    TEST("ADDC3", "R1, *AR0--(IR0)%, R3", 0x20237801);
    TEST("ADDC3", "R2, *+AR1(IR1), R4",   0x20248102);
    TEST("ADDC3", "R3, *-AR2(IR1), R5",   0x20258A03);
    TEST("ADDC3", "R4, *++AR3(IR1), R6",  0x20269304);
    TEST("ADDC3", "R5, *--AR4(IR1), R7",  0x20279C05);
    TEST("ADDC3", "R6, *AR5++(IR1), R0",  0x2020A506);
    TEST("ADDC3", "R7, *AR6--(IR1), R1",  0x2021AE07);
    TEST("ADDC3", "R0, *AR7++(IR1)%, R2", 0x2022B700);
    TEST("ADDC3", "R1, *AR0--(IR1)%, R3", 0x2023B801);
    TEST("ADDC3", "R2, *AR1, R4",         0x2024C102);
    TEST("ADDC3", "R3, *AR2++(IR0)B, R5", 0x2025CA03);
    TEST("ADDC3", "*+AR1, R2, R3",        0x20430201);
    TEST("ADDC3", "*-AR2, R3, R4",        0x2044030A);
    TEST("ADDC3", "*++AR3, R4, R5",       0x20450413);
    TEST("ADDC3", "*--AR4, R5, R6",       0x2046051C);
    TEST("ADDC3", "*AR5++, R6, R7",       0x20470625);
    TEST("ADDC3", "*AR6--, R7, R0",       0x2040072E);
    TEST("ADDC3", "*AR7++%, R0, R1",      0x20410037);
    TEST("ADDC3", "*AR0--%, R1, R2",      0x20420138);
    TEST("ADDC3", "*+AR1(IR0), R2, R4",   0x20440241);
    TEST("ADDC3", "*-AR2(IR0), R3, R5",   0x2045034A);
    TEST("ADDC3", "*++AR3(IR0), R4, R6",  0x20460453);
    TEST("ADDC3", "*--AR4(IR0), R5, R7",  0x2047055C);
    TEST("ADDC3", "*AR5++(IR0), R6, R0",  0x20400665);
    TEST("ADDC3", "*AR6--(IR0), R7, R1",  0x2041076E);
    TEST("ADDC3", "*AR7++(IR0)%, R0, R2", 0x20420077);
    TEST("ADDC3", "*AR0--(IR0)%, R1, R3", 0x20430178);
    TEST("ADDC3", "*+AR1(IR1), R2, R4",   0x20440281);
    TEST("ADDC3", "*-AR2(IR1), R3, R5",   0x2045038A);
    TEST("ADDC3", "*++AR3(IR1), R4, R6",  0x20460493);
    TEST("ADDC3", "*--AR4(IR1), R5, R7",  0x2047059C);
    TEST("ADDC3", "*AR5++(IR1), R6, R0",  0x204006A5);
    TEST("ADDC3", "*AR6--(IR1), R7, R1",  0x204107AE);
    TEST("ADDC3", "*AR7++(IR1)%, R0, R2", 0x204200B7);
    TEST("ADDC3", "*AR0--(IR1)%, R1, R3", 0x204301B8);
    TEST("ADDC3", "*AR1, R2, R4",         0x204402C1);
    TEST("ADDC3", "*AR2++(IR0)B, R3, R5", 0x204503CA);
    TEST("ADDC3", "*+AR1, *AR2++(IR0)B, R3",       0x2063CA01);
    TEST("ADDC3", "*-AR2, *AR3, R4",               0x2064C30A);
    TEST("ADDC3", "*++AR3, *AR4--(IR1)%, R5",      0x2065BC13);
    TEST("ADDC3", "*--AR4, *AR5++(IR1)%, R6",      0x2066B51C);
    TEST("ADDC3", "*AR5++, *AR6--(IR1), R7",       0x2067AE25);
    TEST("ADDC3", "*AR6--, *AR7++(IR1), R0",       0x2060A72E);
    TEST("ADDC3", "*AR7++%, *--AR0(IR1), R1",      0x20619837);
    TEST("ADDC3", "*AR0--%, *++AR1(IR1), R2",      0x20629138);
    TEST("ADDC3", "*+AR1(IR0), *-AR2(IR1), R4",    0x20648A41);
    TEST("ADDC3", "*-AR2(IR0), *+AR3(IR1), R5",    0x2065834A);
    TEST("ADDC3", "*++AR3(IR0), *AR4--(IR0)%, R6", 0x20667C53);
    TEST("ADDC3", "*--AR4(IR0), *AR5++(IR0)%, R7", 0x2067755C);
    TEST("ADDC3", "*AR5++(IR0), *AR6--(IR0), R0",  0x20606E65);
    TEST("ADDC3", "*AR6--(IR0), *AR7++(IR0), R1",  0x2061676E);
    TEST("ADDC3", "*AR7++(IR0)%, *--AR0(IR0), R2", 0x20625877);
    TEST("ADDC3", "*AR0--(IR0)%, *++AR1(IR0), R3", 0x20635178);
    TEST("ADDC3", "*+AR1(IR1), *-AR2(IR0), R4",    0x20644A81);
    TEST("ADDC3", "*-AR2(IR1), *+AR3(IR0), R5",    0x2065438A);
    TEST("ADDC3", "*++AR3(IR1), *AR4--%, R6",      0x20663C93);
    TEST("ADDC3", "*--AR4(IR1), *AR5++%, R7",      0x2067359C);
    TEST("ADDC3", "*AR5++(IR1), *AR6--, R0",       0x20602EA5);
    TEST("ADDC3", "*AR6--(IR1), *AR7++, R1",       0x206127AE);
    TEST("ADDC3", "*AR7++(IR1)%, *--AR0, R2",      0x206218B7);
    TEST("ADDC3", "*AR0--(IR1)%, *++AR1, R3",      0x206311B8);
    TEST("ADDC3", "*AR1, *-AR2, R4",               0x20640AC1);
    TEST("ADDC3", "*AR2++(IR0)B, *+AR3, R5",       0x206503CA);

    TEST("ADDC3", "*+AR2, *-AR2, R5",    0x20650A02);
    ERRT("ADDC3", "*++AR2, *AR2--, R5", REGISTERS_OVERLAPPED, "*AR2--, R5", 0x20652A12);

    ERRT("ADDF3", "R1, AR0, R2", ILLEGAL_REGISTER, "AR0, R2",     0x20820801);
    ERRT("ADDF3", "AR1, R0, R2", ILLEGAL_REGISTER, "AR1, R0, R2", 0x20820009);
    ERRT("ADDF3", "R1, R0, AR2", ILLEGAL_REGISTER, "AR2",         0x208A0001);
    TEST("ADDF3", "*+AR1, *AR2++(IR0)B, R3",       0x20E3CA01);
    TEST("ADDF3", "*-AR2, *AR3, R4",               0x20E4C30A);
    TEST("ADDF3", "*++AR3, *AR4--(IR1)%, R5",      0x20E5BC13);
    TEST("ADDF3", "*--AR4, *AR5++(IR1)%, R6",      0x20E6B51C);
    TEST("ADDF3", "*AR5++, *AR6--(IR1), R7",       0x20E7AE25);
    TEST("ADDF3", "*AR6--, *AR7++(IR1), R0",       0x20E0A72E);
    TEST("ADDF3", "*AR7++%, *--AR0(IR1), R1",      0x20E19837);
    TEST("ADDF3", "*AR0--%, *++AR1(IR1), R2",      0x20E29138);
    TEST("ADDF3", "*+AR1(IR0), *-AR2(IR1), R4",    0x20E48A41);
    TEST("ADDF3", "*-AR2(IR0), *+AR3(IR1), R5",    0x20E5834A);
    TEST("ADDF3", "*++AR3(IR0), *AR4--(IR0)%, R6", 0x20E67C53);
    TEST("ADDF3", "*--AR4(IR0), *AR5++(IR0)%, R7", 0x20E7755C);
    TEST("ADDF3", "*AR5++(IR0), *AR6--(IR0), R0",  0x20E06E65);
    TEST("ADDF3", "*AR6--(IR0), *AR7++(IR0), R1",  0x20E1676E);
    TEST("ADDF3", "*AR7++(IR0)%, *--AR0(IR0), R2", 0x20E25877);
    TEST("ADDF3", "*AR0--(IR0)%, *++AR1(IR0), R3", 0x20E35178);
    TEST("ADDF3", "*+AR1(IR1), *-AR2(IR0), R4",    0x20E44A81);
    TEST("ADDF3", "*-AR2(IR1), *+AR3(IR0), R5",    0x20E5438A);
    TEST("ADDF3", "*++AR3(IR1), *AR4--%, R6",      0x20E63C93);
    TEST("ADDF3", "*--AR4(IR1), *AR5++%, R7",      0x20E7359C);
    TEST("ADDF3", "*AR5++(IR1), *AR6--, R0",       0x20E02EA5);
    TEST("ADDF3", "*AR6--(IR1), *AR7++, R1",       0x20E127AE);
    TEST("ADDF3", "*AR7++(IR1)%, *--AR0, R2",      0x20E218B7);
    TEST("ADDF3", "*AR0--(IR1)%, *++AR1, R3",      0x20E311B8);
    TEST("ADDF3", "*AR1, *-AR2, R4",               0x20E40AC1);
    TEST("ADDF3", "*AR2++(IR0)B, *+AR3, R5",       0x20E503CA);

    TEST("ADDI3", "R1, AR0, R2", 0x21020801);
    TEST("ADDI3", "AR1, R0, R2", 0x21020009);
    TEST("ADDI3", "R1, R0, AR2", 0x210A0001);
    TEST("ADDI3", "*+AR1, *AR2++(IR0)B, R3",       0x2163CA01);
    TEST("ADDI3", "*-AR2, *AR3, R4",               0x2164C30A);
    TEST("ADDI3", "*++AR3, *AR4--(IR1)%, R5",      0x2165BC13);
    TEST("ADDI3", "*--AR4, *AR5++(IR1)%, R6",      0x2166B51C);
    TEST("ADDI3", "*AR5++, *AR6--(IR1), R7",       0x2167AE25);
    TEST("ADDI3", "*AR6--, *AR7++(IR1), R0",       0x2160A72E);
    TEST("ADDI3", "*AR7++%, *--AR0(IR1), R1",      0x21619837);
    TEST("ADDI3", "*AR0--%, *++AR1(IR1), R2",      0x21629138);
    TEST("ADDI3", "*+AR1(IR0), *-AR2(IR1), R4",    0x21648A41);
    TEST("ADDI3", "*-AR2(IR0), *+AR3(IR1), R5",    0x2165834A);
    TEST("ADDI3", "*++AR3(IR0), *AR4--(IR0)%, R6", 0x21667C53);
    TEST("ADDI3", "*--AR4(IR0), *AR5++(IR0)%, R7", 0x2167755C);
    TEST("ADDI3", "*AR5++(IR0), *AR6--(IR0), R0",  0x21606E65);
    TEST("ADDI3", "*AR6--(IR0), *AR7++(IR0), R1",  0x2161676E);
    TEST("ADDI3", "*AR7++(IR0)%, *--AR0(IR0), R2", 0x21625877);
    TEST("ADDI3", "*AR0--(IR0)%, *++AR1(IR0), R3", 0x21635178);
    TEST("ADDI3", "*+AR1(IR1), *-AR2(IR0), R4",    0x21644A81);
    TEST("ADDI3", "*-AR2(IR1), *+AR3(IR0), R5",    0x2165438A);
    TEST("ADDI3", "*++AR3(IR1), *AR4--%, R6",      0x21663C93);
    TEST("ADDI3", "*--AR4(IR1), *AR5++%, R7",      0x2167359C);
    TEST("ADDI3", "*AR5++(IR1), *AR6--, R0",       0x21602EA5);
    TEST("ADDI3", "*AR6--(IR1), *AR7++, R1",       0x216127AE);
    TEST("ADDI3", "*AR7++(IR1)%, *--AR0, R2",      0x216218B7);
    TEST("ADDI3", "*AR0--(IR1)%, *++AR1, R3",      0x216311B8);
    TEST("ADDI3", "*AR1, *-AR2, R4",               0x21640AC1);
    TEST("ADDI3", "*AR2++(IR0)B, *+AR3, R5",       0x216503CA);

    TEST("AND3", "R1, AR0, R2", 0x21820801);
    TEST("AND3", "AR1, R0, R2", 0x21820009);
    TEST("AND3", "R1, R0, AR2", 0x218A0001);
    TEST("AND3", "*+AR1, *AR2++(IR0)B, R3",       0x21E3CA01);
    TEST("AND3", "*-AR2, *AR3, R4",               0x21E4C30A);
    TEST("AND3", "*++AR3, *AR4--(IR1)%, R5",      0x21E5BC13);
    TEST("AND3", "*--AR4, *AR5++(IR1)%, R6",      0x21E6B51C);
    TEST("AND3", "*AR5++, *AR6--(IR1), R7",       0x21E7AE25);
    TEST("AND3", "*AR6--, *AR7++(IR1), R0",       0x21E0A72E);
    TEST("AND3", "*AR7++%, *--AR0(IR1), R1",      0x21E19837);
    TEST("AND3", "*AR0--%, *++AR1(IR1), R2",      0x21E29138);
    TEST("AND3", "*+AR1(IR0), *-AR2(IR1), R4",    0x21E48A41);
    TEST("AND3", "*-AR2(IR0), *+AR3(IR1), R5",    0x21E5834A);
    TEST("AND3", "*++AR3(IR0), *AR4--(IR0)%, R6", 0x21E67C53);
    TEST("AND3", "*--AR4(IR0), *AR5++(IR0)%, R7", 0x21E7755C);
    TEST("AND3", "*AR5++(IR0), *AR6--(IR0), R0",  0x21E06E65);
    TEST("AND3", "*AR6--(IR0), *AR7++(IR0), R1",  0x21E1676E);
    TEST("AND3", "*AR7++(IR0)%, *--AR0(IR0), R2", 0x21E25877);
    TEST("AND3", "*AR0--(IR0)%, *++AR1(IR0), R3", 0x21E35178);
    TEST("AND3", "*+AR1(IR1), *-AR2(IR0), R4",    0x21E44A81);
    TEST("AND3", "*-AR2(IR1), *+AR3(IR0), R5",    0x21E5438A);
    TEST("AND3", "*++AR3(IR1), *AR4--%, R6",      0x21E63C93);
    TEST("AND3", "*--AR4(IR1), *AR5++%, R7",      0x21E7359C);
    TEST("AND3", "*AR5++(IR1), *AR6--, R0",       0x21E02EA5);
    TEST("AND3", "*AR6--(IR1), *AR7++, R1",       0x21E127AE);
    TEST("AND3", "*AR7++(IR1)%, *--AR0, R2",      0x21E218B7);
    TEST("AND3", "*AR0--(IR1)%, *++AR1, R3",      0x21E311B8);
    TEST("AND3", "*AR1, *-AR2, R4",               0x21E40AC1);
    TEST("AND3", "*AR2++(IR0)B, *+AR3, R5",       0x21E503CA);

    TEST("ANDN3", "R1, AR0, R2", 0x22020801);
    TEST("ANDN3", "AR1, R0, R2", 0x22020009);
    TEST("ANDN3", "R1, R0, AR2", 0x220A0001);
    TEST("ANDN3", "*+AR1, *AR2++(IR0)B, R3",       0x2263CA01);
    TEST("ANDN3", "*-AR2, *AR3, R4",               0x2264C30A);
    TEST("ANDN3", "*++AR3, *AR4--(IR1)%, R5",      0x2265BC13);
    TEST("ANDN3", "*--AR4, *AR5++(IR1)%, R6",      0x2266B51C);
    TEST("ANDN3", "*AR5++, *AR6--(IR1), R7",       0x2267AE25);
    TEST("ANDN3", "*AR6--, *AR7++(IR1), R0",       0x2260A72E);
    TEST("ANDN3", "*AR7++%, *--AR0(IR1), R1",      0x22619837);
    TEST("ANDN3", "*AR0--%, *++AR1(IR1), R2",      0x22629138);
    TEST("ANDN3", "*+AR1(IR0), *-AR2(IR1), R4",    0x22648A41);
    TEST("ANDN3", "*-AR2(IR0), *+AR3(IR1), R5",    0x2265834A);
    TEST("ANDN3", "*++AR3(IR0), *AR4--(IR0)%, R6", 0x22667C53);
    TEST("ANDN3", "*--AR4(IR0), *AR5++(IR0)%, R7", 0x2267755C);
    TEST("ANDN3", "*AR5++(IR0), *AR6--(IR0), R0",  0x22606E65);
    TEST("ANDN3", "*AR6--(IR0), *AR7++(IR0), R1",  0x2261676E);
    TEST("ANDN3", "*AR7++(IR0)%, *--AR0(IR0), R2", 0x22625877);
    TEST("ANDN3", "*AR0--(IR0)%, *++AR1(IR0), R3", 0x22635178);
    TEST("ANDN3", "*+AR1(IR1), *-AR2(IR0), R4",    0x22644A81);
    TEST("ANDN3", "*-AR2(IR1), *+AR3(IR0), R5",    0x2265438A);
    TEST("ANDN3", "*++AR3(IR1), *AR4--%, R6",      0x22663C93);
    TEST("ANDN3", "*--AR4(IR1), *AR5++%, R7",      0x2267359C);
    TEST("ANDN3", "*AR5++(IR1), *AR6--, R0",       0x22602EA5);
    TEST("ANDN3", "*AR6--(IR1), *AR7++, R1",       0x226127AE);
    TEST("ANDN3", "*AR7++(IR1)%, *--AR0, R2",      0x226218B7);
    TEST("ANDN3", "*AR0--(IR1)%, *++AR1, R3",      0x226311B8);
    TEST("ANDN3", "*AR1, *-AR2, R4",               0x22640AC1);
    TEST("ANDN3", "*AR2++(IR0)B, *+AR3, R5",       0x226503CA);

    TEST("ASH3", "R1, AR0, R2", 0x22820801);
    TEST("ASH3", "AR1, R0, R2", 0x22820009);
    TEST("ASH3", "R1, R0, AR2", 0x228A0001);
    TEST("ASH3", "*+AR1, *AR2++(IR0)B, R3",       0x22E3CA01);
    TEST("ASH3", "*-AR2, *AR3, R4",               0x22E4C30A);
    TEST("ASH3", "*++AR3, *AR4--(IR1)%, R5",      0x22E5BC13);
    TEST("ASH3", "*--AR4, *AR5++(IR1)%, R6",      0x22E6B51C);
    TEST("ASH3", "*AR5++, *AR6--(IR1), R7",       0x22E7AE25);
    TEST("ASH3", "*AR6--, *AR7++(IR1), R0",       0x22E0A72E);
    TEST("ASH3", "*AR7++%, *--AR0(IR1), R1",      0x22E19837);
    TEST("ASH3", "*AR0--%, *++AR1(IR1), R2",      0x22E29138);
    TEST("ASH3", "*+AR1(IR0), *-AR2(IR1), R4",    0x22E48A41);
    TEST("ASH3", "*-AR2(IR0), *+AR3(IR1), R5",    0x22E5834A);
    TEST("ASH3", "*++AR3(IR0), *AR4--(IR0)%, R6", 0x22E67C53);
    TEST("ASH3", "*--AR4(IR0), *AR5++(IR0)%, R7", 0x22E7755C);
    TEST("ASH3", "*AR5++(IR0), *AR6--(IR0), R0",  0x22E06E65);
    TEST("ASH3", "*AR6--(IR0), *AR7++(IR0), R1",  0x22E1676E);
    TEST("ASH3", "*AR7++(IR0)%, *--AR0(IR0), R2", 0x22E25877);
    TEST("ASH3", "*AR0--(IR0)%, *++AR1(IR0), R3", 0x22E35178);
    TEST("ASH3", "*+AR1(IR1), *-AR2(IR0), R4",    0x22E44A81);
    TEST("ASH3", "*-AR2(IR1), *+AR3(IR0), R5",    0x22E5438A);
    TEST("ASH3", "*++AR3(IR1), *AR4--%, R6",      0x22E63C93);
    TEST("ASH3", "*--AR4(IR1), *AR5++%, R7",      0x22E7359C);
    TEST("ASH3", "*AR5++(IR1), *AR6--, R0",       0x22E02EA5);
    TEST("ASH3", "*AR6--(IR1), *AR7++, R1",       0x22E127AE);
    TEST("ASH3", "*AR7++(IR1)%, *--AR0, R2",      0x22E218B7);
    TEST("ASH3", "*AR0--(IR1)%, *++AR1, R3",      0x22E311B8);
    TEST("ASH3", "*AR1, *-AR2, R4",               0x22E40AC1);
    TEST("ASH3", "*AR2++(IR0)B, *+AR3, R5",       0x22E503CA);

    ERRT("CMPF3", "R1, AR0", ILLEGAL_REGISTER, "AR0",     0x23000801);
    ERRT("CMPF3", "AR1, R0", ILLEGAL_REGISTER, "AR1, R0", 0x23000009);
    TEST("CMPF3", "*+AR1, *AR2++(IR0)B",       0x2360CA01);
    TEST("CMPF3", "*-AR2, *AR3",               0x2360C30A);
    TEST("CMPF3", "*++AR3, *AR4--(IR1)%",      0x2360BC13);
    TEST("CMPF3", "*--AR4, *AR5++(IR1)%",      0x2360B51C);
    TEST("CMPF3", "*AR5++, *AR6--(IR1)",       0x2360AE25);
    TEST("CMPF3", "*AR6--, *AR7++(IR1)",       0x2360A72E);
    TEST("CMPF3", "*AR7++%, *--AR0(IR1)",      0x23609837);
    TEST("CMPF3", "*AR0--%, *++AR1(IR1)",      0x23609138);
    TEST("CMPF3", "*+AR1(IR0), *-AR2(IR1)",    0x23608A41);
    TEST("CMPF3", "*-AR2(IR0), *+AR3(IR1)",    0x2360834A);
    TEST("CMPF3", "*++AR3(IR0), *AR4--(IR0)%", 0x23607C53);
    TEST("CMPF3", "*--AR4(IR0), *AR5++(IR0)%", 0x2360755C);
    TEST("CMPF3", "*AR5++(IR0), *AR6--(IR0)",  0x23606E65);
    TEST("CMPF3", "*AR6--(IR0), *AR7++(IR0)",  0x2360676E);
    TEST("CMPF3", "*AR7++(IR0)%, *--AR0(IR0)", 0x23605877);
    TEST("CMPF3", "*AR0--(IR0)%, *++AR1(IR0)", 0x23605178);
    TEST("CMPF3", "*+AR1(IR1), *-AR2(IR0)",    0x23604A81);
    TEST("CMPF3", "*-AR2(IR1), *+AR3(IR0)",    0x2360438A);
    TEST("CMPF3", "*++AR3(IR1), *AR4--%",      0x23603C93);
    TEST("CMPF3", "*--AR4(IR1), *AR5++%",      0x2360359C);
    TEST("CMPF3", "*AR5++(IR1), *AR6--",       0x23602EA5);
    TEST("CMPF3", "*AR6--(IR1), *AR7++",       0x236027AE);
    TEST("CMPF3", "*AR7++(IR1)%, *--AR0",      0x236018B7);
    TEST("CMPF3", "*AR0--(IR1)%, *++AR1",      0x236011B8);
    TEST("CMPF3", "*AR1, *-AR2",               0x23600AC1);
    TEST("CMPF3", "*AR2++(IR0)B, *+AR3",       0x236003CA);

    TEST("CMPI3", "R1, AR0", 0x23800801);
    TEST("CMPI3", "AR1, R0", 0x23800009);
    TEST("CMPI3", "*+AR1, *AR2++(IR0)B",       0x23E0CA01);
    TEST("CMPI3", "*-AR2, *AR3",               0x23E0C30A);
    TEST("CMPI3", "*++AR3, *AR4--(IR1)%",      0x23E0BC13);
    TEST("CMPI3", "*--AR4, *AR5++(IR1)%",      0x23E0B51C);
    TEST("CMPI3", "*AR5++, *AR6--(IR1)",       0x23E0AE25);
    TEST("CMPI3", "*AR6--, *AR7++(IR1)",       0x23E0A72E);
    TEST("CMPI3", "*AR7++%, *--AR0(IR1)",      0x23E09837);
    TEST("CMPI3", "*AR0--%, *++AR1(IR1)",      0x23E09138);
    TEST("CMPI3", "*+AR1(IR0), *-AR2(IR1)",    0x23E08A41);
    TEST("CMPI3", "*-AR2(IR0), *+AR3(IR1)",    0x23E0834A);
    TEST("CMPI3", "*++AR3(IR0), *AR4--(IR0)%", 0x23E07C53);
    TEST("CMPI3", "*--AR4(IR0), *AR5++(IR0)%", 0x23E0755C);
    TEST("CMPI3", "*AR5++(IR0), *AR6--(IR0)",  0x23E06E65);
    TEST("CMPI3", "*AR6--(IR0), *AR7++(IR0)",  0x23E0676E);
    TEST("CMPI3", "*AR7++(IR0)%, *--AR0(IR0)", 0x23E05877);
    TEST("CMPI3", "*AR0--(IR0)%, *++AR1(IR0)", 0x23E05178);
    TEST("CMPI3", "*+AR1(IR1), *-AR2(IR0)",    0x23E04A81);
    TEST("CMPI3", "*-AR2(IR1), *+AR3(IR0)",    0x23E0438A);
    TEST("CMPI3", "*++AR3(IR1), *AR4--%",      0x23E03C93);
    TEST("CMPI3", "*--AR4(IR1), *AR5++%",      0x23E0359C);
    TEST("CMPI3", "*AR5++(IR1), *AR6--",       0x23E02EA5);
    TEST("CMPI3", "*AR6--(IR1), *AR7++",       0x23E027AE);
    TEST("CMPI3", "*AR7++(IR1)%, *--AR0",      0x23E018B7);
    TEST("CMPI3", "*AR0--(IR1)%, *++AR1",      0x23E011B8);
    TEST("CMPI3", "*AR1, *-AR2",               0x23E00AC1);
    TEST("CMPI3", "*AR2++(IR0)B, *+AR3",       0x23E003CA);

    TEST("LSH3", "R1, AR0, R2", 0x24020801);
    TEST("LSH3", "AR1, R0, R2", 0x24020009);
    TEST("LSH3", "R1, R0, AR2", 0x240A0001);
    TEST("LSH3", "*+AR1, *AR2++(IR0)B, R3",       0x2463CA01);
    TEST("LSH3", "*-AR2, *AR3, R4",               0x2464C30A);
    TEST("LSH3", "*++AR3, *AR4--(IR1)%, R5",      0x2465BC13);
    TEST("LSH3", "*--AR4, *AR5++(IR1)%, R6",      0x2466B51C);
    TEST("LSH3", "*AR5++, *AR6--(IR1), R7",       0x2467AE25);
    TEST("LSH3", "*AR6--, *AR7++(IR1), R0",       0x2460A72E);
    TEST("LSH3", "*AR7++%, *--AR0(IR1), R1",      0x24619837);
    TEST("LSH3", "*AR0--%, *++AR1(IR1), R2",      0x24629138);
    TEST("LSH3", "*+AR1(IR0), *-AR2(IR1), R4",    0x24648A41);
    TEST("LSH3", "*-AR2(IR0), *+AR3(IR1), R5",    0x2465834A);
    TEST("LSH3", "*++AR3(IR0), *AR4--(IR0)%, R6", 0x24667C53);
    TEST("LSH3", "*--AR4(IR0), *AR5++(IR0)%, R7", 0x2467755C);
    TEST("LSH3", "*AR5++(IR0), *AR6--(IR0), R0",  0x24606E65);
    TEST("LSH3", "*AR6--(IR0), *AR7++(IR0), R1",  0x2461676E);
    TEST("LSH3", "*AR7++(IR0)%, *--AR0(IR0), R2", 0x24625877);
    TEST("LSH3", "*AR0--(IR0)%, *++AR1(IR0), R3", 0x24635178);
    TEST("LSH3", "*+AR1(IR1), *-AR2(IR0), R4",    0x24644A81);
    TEST("LSH3", "*-AR2(IR1), *+AR3(IR0), R5",    0x2465438A);
    TEST("LSH3", "*++AR3(IR1), *AR4--%, R6",      0x24663C93);
    TEST("LSH3", "*--AR4(IR1), *AR5++%, R7",      0x2467359C);
    TEST("LSH3", "*AR5++(IR1), *AR6--, R0",       0x24602EA5);
    TEST("LSH3", "*AR6--(IR1), *AR7++, R1",       0x246127AE);
    TEST("LSH3", "*AR7++(IR1)%, *--AR0, R2",      0x246218B7);
    TEST("LSH3", "*AR0--(IR1)%, *++AR1, R3",      0x246311B8);
    TEST("LSH3", "*AR1, *-AR2, R4",               0x24640AC1);
    TEST("LSH3", "*AR2++(IR0)B, *+AR3, R5",       0x246503CA);

    ERRT("MPYF3", "R1, AR0, R2", ILLEGAL_REGISTER, "AR0, R2",     0x24820801);
    ERRT("MPYF3", "AR1, R0, R2", ILLEGAL_REGISTER, "AR1, R0, R2", 0x24820009);
    ERRT("MPYF3", "R1, R0, AR2", ILLEGAL_REGISTER, "AR2",         0x248A0001);
    TEST("MPYF3", "*+AR1, *AR2++(IR0)B, R3",       0x24E3CA01);
    TEST("MPYF3", "*-AR2, *AR3, R4",               0x24E4C30A);
    TEST("MPYF3", "*++AR3, *AR4--(IR1)%, R5",      0x24E5BC13);
    TEST("MPYF3", "*--AR4, *AR5++(IR1)%, R6",      0x24E6B51C);
    TEST("MPYF3", "*AR5++, *AR6--(IR1), R7",       0x24E7AE25);
    TEST("MPYF3", "*AR6--, *AR7++(IR1), R0",       0x24E0A72E);
    TEST("MPYF3", "*AR7++%, *--AR0(IR1), R1",      0x24E19837);
    TEST("MPYF3", "*AR0--%, *++AR1(IR1), R2",      0x24E29138);
    TEST("MPYF3", "*+AR1(IR0), *-AR2(IR1), R4",    0x24E48A41);
    TEST("MPYF3", "*-AR2(IR0), *+AR3(IR1), R5",    0x24E5834A);
    TEST("MPYF3", "*++AR3(IR0), *AR4--(IR0)%, R6", 0x24E67C53);
    TEST("MPYF3", "*--AR4(IR0), *AR5++(IR0)%, R7", 0x24E7755C);
    TEST("MPYF3", "*AR5++(IR0), *AR6--(IR0), R0",  0x24E06E65);
    TEST("MPYF3", "*AR6--(IR0), *AR7++(IR0), R1",  0x24E1676E);
    TEST("MPYF3", "*AR7++(IR0)%, *--AR0(IR0), R2", 0x24E25877);
    TEST("MPYF3", "*AR0--(IR0)%, *++AR1(IR0), R3", 0x24E35178);
    TEST("MPYF3", "*+AR1(IR1), *-AR2(IR0), R4",    0x24E44A81);
    TEST("MPYF3", "*-AR2(IR1), *+AR3(IR0), R5",    0x24E5438A);
    TEST("MPYF3", "*++AR3(IR1), *AR4--%, R6",      0x24E63C93);
    TEST("MPYF3", "*--AR4(IR1), *AR5++%, R7",      0x24E7359C);
    TEST("MPYF3", "*AR5++(IR1), *AR6--, R0",       0x24E02EA5);
    TEST("MPYF3", "*AR6--(IR1), *AR7++, R1",       0x24E127AE);
    TEST("MPYF3", "*AR7++(IR1)%, *--AR0, R2",      0x24E218B7);
    TEST("MPYF3", "*AR0--(IR1)%, *++AR1, R3",      0x24E311B8);
    TEST("MPYF3", "*AR1, *-AR2, R4",               0x24E40AC1);
    TEST("MPYF3", "*AR2++(IR0)B, *+AR3, R5",       0x24E503CA);

    TEST("MPYI3", "R1, AR0, R2", 0x25020801);
    TEST("MPYI3", "AR1, R0, R2", 0x25020009);
    TEST("MPYI3", "R1, R0, AR2", 0x250A0001);
    TEST("MPYI3", "*+AR1, *AR2++(IR0)B, R3",       0x2563CA01);
    TEST("MPYI3", "*-AR2, *AR3, R4",               0x2564C30A);
    TEST("MPYI3", "*++AR3, *AR4--(IR1)%, R5",      0x2565BC13);
    TEST("MPYI3", "*--AR4, *AR5++(IR1)%, R6",      0x2566B51C);
    TEST("MPYI3", "*AR5++, *AR6--(IR1), R7",       0x2567AE25);
    TEST("MPYI3", "*AR6--, *AR7++(IR1), R0",       0x2560A72E);
    TEST("MPYI3", "*AR7++%, *--AR0(IR1), R1",      0x25619837);
    TEST("MPYI3", "*AR0--%, *++AR1(IR1), R2",      0x25629138);
    TEST("MPYI3", "*+AR1(IR0), *-AR2(IR1), R4",    0x25648A41);
    TEST("MPYI3", "*-AR2(IR0), *+AR3(IR1), R5",    0x2565834A);
    TEST("MPYI3", "*++AR3(IR0), *AR4--(IR0)%, R6", 0x25667C53);
    TEST("MPYI3", "*--AR4(IR0), *AR5++(IR0)%, R7", 0x2567755C);
    TEST("MPYI3", "*AR5++(IR0), *AR6--(IR0), R0",  0x25606E65);
    TEST("MPYI3", "*AR6--(IR0), *AR7++(IR0), R1",  0x2561676E);
    TEST("MPYI3", "*AR7++(IR0)%, *--AR0(IR0), R2", 0x25625877);
    TEST("MPYI3", "*AR0--(IR0)%, *++AR1(IR0), R3", 0x25635178);
    TEST("MPYI3", "*+AR1(IR1), *-AR2(IR0), R4",    0x25644A81);
    TEST("MPYI3", "*-AR2(IR1), *+AR3(IR0), R5",    0x2565438A);
    TEST("MPYI3", "*++AR3(IR1), *AR4--%, R6",      0x25663C93);
    TEST("MPYI3", "*--AR4(IR1), *AR5++%, R7",      0x2567359C);
    TEST("MPYI3", "*AR5++(IR1), *AR6--, R0",       0x25602EA5);
    TEST("MPYI3", "*AR6--(IR1), *AR7++, R1",       0x256127AE);
    TEST("MPYI3", "*AR7++(IR1)%, *--AR0, R2",      0x256218B7);
    TEST("MPYI3", "*AR0--(IR1)%, *++AR1, R3",      0x256311B8);
    TEST("MPYI3", "*AR1, *-AR2, R4",               0x25640AC1);
    TEST("MPYI3", "*AR2++(IR0)B, *+AR3, R5",       0x256503CA);

    TEST("OR3", "R1, AR0, R2", 0x25820801);
    TEST("OR3", "AR1, R0, R2", 0x25820009);
    TEST("OR3", "R1, R0, AR2", 0x258A0001);
    TEST("OR3", "*+AR1, *AR2++(IR0)B, R3",       0x25E3CA01);
    TEST("OR3", "*-AR2, *AR3, R4",               0x25E4C30A);
    TEST("OR3", "*++AR3, *AR4--(IR1)%, R5",      0x25E5BC13);
    TEST("OR3", "*--AR4, *AR5++(IR1)%, R6",      0x25E6B51C);
    TEST("OR3", "*AR5++, *AR6--(IR1), R7",       0x25E7AE25);
    TEST("OR3", "*AR6--, *AR7++(IR1), R0",       0x25E0A72E);
    TEST("OR3", "*AR7++%, *--AR0(IR1), R1",      0x25E19837);
    TEST("OR3", "*AR0--%, *++AR1(IR1), R2",      0x25E29138);
    TEST("OR3", "*+AR1(IR0), *-AR2(IR1), R4",    0x25E48A41);
    TEST("OR3", "*-AR2(IR0), *+AR3(IR1), R5",    0x25E5834A);
    TEST("OR3", "*++AR3(IR0), *AR4--(IR0)%, R6", 0x25E67C53);
    TEST("OR3", "*--AR4(IR0), *AR5++(IR0)%, R7", 0x25E7755C);
    TEST("OR3", "*AR5++(IR0), *AR6--(IR0), R0",  0x25E06E65);
    TEST("OR3", "*AR6--(IR0), *AR7++(IR0), R1",  0x25E1676E);
    TEST("OR3", "*AR7++(IR0)%, *--AR0(IR0), R2", 0x25E25877);
    TEST("OR3", "*AR0--(IR0)%, *++AR1(IR0), R3", 0x25E35178);
    TEST("OR3", "*+AR1(IR1), *-AR2(IR0), R4",    0x25E44A81);
    TEST("OR3", "*-AR2(IR1), *+AR3(IR0), R5",    0x25E5438A);
    TEST("OR3", "*++AR3(IR1), *AR4--%, R6",      0x25E63C93);
    TEST("OR3", "*--AR4(IR1), *AR5++%, R7",      0x25E7359C);
    TEST("OR3", "*AR5++(IR1), *AR6--, R0",       0x25E02EA5);
    TEST("OR3", "*AR6--(IR1), *AR7++, R1",       0x25E127AE);
    TEST("OR3", "*AR7++(IR1)%, *--AR0, R2",      0x25E218B7);
    TEST("OR3", "*AR0--(IR1)%, *++AR1, R3",      0x25E311B8);
    TEST("OR3", "*AR1, *-AR2, R4",               0x25E40AC1);
    TEST("OR3", "*AR2++(IR0)B, *+AR3, R5",       0x25E503CA);

    TEST("SUBB3", "R1, AR0, R2", 0x26020801);
    TEST("SUBB3", "AR1, R0, R2", 0x26020009);
    TEST("SUBB3", "R1, R0, AR2", 0x260A0001);
    TEST("SUBB3", "*+AR1, *AR2++(IR0)B, R3",       0x2663CA01);
    TEST("SUBB3", "*-AR2, *AR3, R4",               0x2664C30A);
    TEST("SUBB3", "*++AR3, *AR4--(IR1)%, R5",      0x2665BC13);
    TEST("SUBB3", "*--AR4, *AR5++(IR1)%, R6",      0x2666B51C);
    TEST("SUBB3", "*AR5++, *AR6--(IR1), R7",       0x2667AE25);
    TEST("SUBB3", "*AR6--, *AR7++(IR1), R0",       0x2660A72E);
    TEST("SUBB3", "*AR7++%, *--AR0(IR1), R1",      0x26619837);
    TEST("SUBB3", "*AR0--%, *++AR1(IR1), R2",      0x26629138);
    TEST("SUBB3", "*+AR1(IR0), *-AR2(IR1), R4",    0x26648A41);
    TEST("SUBB3", "*-AR2(IR0), *+AR3(IR1), R5",    0x2665834A);
    TEST("SUBB3", "*++AR3(IR0), *AR4--(IR0)%, R6", 0x26667C53);
    TEST("SUBB3", "*--AR4(IR0), *AR5++(IR0)%, R7", 0x2667755C);
    TEST("SUBB3", "*AR5++(IR0), *AR6--(IR0), R0",  0x26606E65);
    TEST("SUBB3", "*AR6--(IR0), *AR7++(IR0), R1",  0x2661676E);
    TEST("SUBB3", "*AR7++(IR0)%, *--AR0(IR0), R2", 0x26625877);
    TEST("SUBB3", "*AR0--(IR0)%, *++AR1(IR0), R3", 0x26635178);
    TEST("SUBB3", "*+AR1(IR1), *-AR2(IR0), R4",    0x26644A81);
    TEST("SUBB3", "*-AR2(IR1), *+AR3(IR0), R5",    0x2665438A);
    TEST("SUBB3", "*++AR3(IR1), *AR4--%, R6",      0x26663C93);
    TEST("SUBB3", "*--AR4(IR1), *AR5++%, R7",      0x2667359C);
    TEST("SUBB3", "*AR5++(IR1), *AR6--, R0",       0x26602EA5);
    TEST("SUBB3", "*AR6--(IR1), *AR7++, R1",       0x266127AE);
    TEST("SUBB3", "*AR7++(IR1)%, *--AR0, R2",      0x266218B7);
    TEST("SUBB3", "*AR0--(IR1)%, *++AR1, R3",      0x266311B8);
    TEST("SUBB3", "*AR1, *-AR2, R4",               0x26640AC1);
    TEST("SUBB3", "*AR2++(IR0)B, *+AR3, R5",       0x266503CA);

    ERRT("SUBF3", "R1, AR0, R2", ILLEGAL_REGISTER, "AR0, R2",     0x26820801);
    ERRT("SUBF3", "AR1, R0, R2", ILLEGAL_REGISTER, "AR1, R0, R2", 0x26820009);
    ERRT("SUBF3", "R1, R0, AR2", ILLEGAL_REGISTER, "AR2",         0x268A0001);
    TEST("SUBF3", "*+AR1, *AR2++(IR0)B, R3",       0x26E3CA01);
    TEST("SUBF3", "*-AR2, *AR3, R4",               0x26E4C30A);
    TEST("SUBF3", "*++AR3, *AR4--(IR1)%, R5",      0x26E5BC13);
    TEST("SUBF3", "*--AR4, *AR5++(IR1)%, R6",      0x26E6B51C);
    TEST("SUBF3", "*AR5++, *AR6--(IR1), R7",       0x26E7AE25);
    TEST("SUBF3", "*AR6--, *AR7++(IR1), R0",       0x26E0A72E);
    TEST("SUBF3", "*AR7++%, *--AR0(IR1), R1",      0x26E19837);
    TEST("SUBF3", "*AR0--%, *++AR1(IR1), R2",      0x26E29138);
    TEST("SUBF3", "*+AR1(IR0), *-AR2(IR1), R4",    0x26E48A41);
    TEST("SUBF3", "*-AR2(IR0), *+AR3(IR1), R5",    0x26E5834A);
    TEST("SUBF3", "*++AR3(IR0), *AR4--(IR0)%, R6", 0x26E67C53);
    TEST("SUBF3", "*--AR4(IR0), *AR5++(IR0)%, R7", 0x26E7755C);
    TEST("SUBF3", "*AR5++(IR0), *AR6--(IR0), R0",  0x26E06E65);
    TEST("SUBF3", "*AR6--(IR0), *AR7++(IR0), R1",  0x26E1676E);
    TEST("SUBF3", "*AR7++(IR0)%, *--AR0(IR0), R2", 0x26E25877);
    TEST("SUBF3", "*AR0--(IR0)%, *++AR1(IR0), R3", 0x26E35178);
    TEST("SUBF3", "*+AR1(IR1), *-AR2(IR0), R4",    0x26E44A81);
    TEST("SUBF3", "*-AR2(IR1), *+AR3(IR0), R5",    0x26E5438A);
    TEST("SUBF3", "*++AR3(IR1), *AR4--%, R6",      0x26E63C93);
    TEST("SUBF3", "*--AR4(IR1), *AR5++%, R7",      0x26E7359C);
    TEST("SUBF3", "*AR5++(IR1), *AR6--, R0",       0x26E02EA5);
    TEST("SUBF3", "*AR6--(IR1), *AR7++, R1",       0x26E127AE);
    TEST("SUBF3", "*AR7++(IR1)%, *--AR0, R2",      0x26E218B7);
    TEST("SUBF3", "*AR0--(IR1)%, *++AR1, R3",      0x26E311B8);
    TEST("SUBF3", "*AR1, *-AR2, R4",               0x26E40AC1);
    TEST("SUBF3", "*AR2++(IR0)B, *+AR3, R5",       0x26E503CA);

    TEST("SUBI3", "R1, AR0, R2", 0x27020801);
    TEST("SUBI3", "AR1, R0, R2", 0x27020009);
    TEST("SUBI3", "R1, R0, AR2", 0x270A0001);
    TEST("SUBI3", "*+AR1, *AR2++(IR0)B, R3",       0x2763CA01);
    TEST("SUBI3", "*-AR2, *AR3, R4",               0x2764C30A);
    TEST("SUBI3", "*++AR3, *AR4--(IR1)%, R5",      0x2765BC13);
    TEST("SUBI3", "*--AR4, *AR5++(IR1)%, R6",      0x2766B51C);
    TEST("SUBI3", "*AR5++, *AR6--(IR1), R7",       0x2767AE25);
    TEST("SUBI3", "*AR6--, *AR7++(IR1), R0",       0x2760A72E);
    TEST("SUBI3", "*AR7++%, *--AR0(IR1), R1",      0x27619837);
    TEST("SUBI3", "*AR0--%, *++AR1(IR1), R2",      0x27629138);
    TEST("SUBI3", "*+AR1(IR0), *-AR2(IR1), R4",    0x27648A41);
    TEST("SUBI3", "*-AR2(IR0), *+AR3(IR1), R5",    0x2765834A);
    TEST("SUBI3", "*++AR3(IR0), *AR4--(IR0)%, R6", 0x27667C53);
    TEST("SUBI3", "*--AR4(IR0), *AR5++(IR0)%, R7", 0x2767755C);
    TEST("SUBI3", "*AR5++(IR0), *AR6--(IR0), R0",  0x27606E65);
    TEST("SUBI3", "*AR6--(IR0), *AR7++(IR0), R1",  0x2761676E);
    TEST("SUBI3", "*AR7++(IR0)%, *--AR0(IR0), R2", 0x27625877);
    TEST("SUBI3", "*AR0--(IR0)%, *++AR1(IR0), R3", 0x27635178);
    TEST("SUBI3", "*+AR1(IR1), *-AR2(IR0), R4",    0x27644A81);
    TEST("SUBI3", "*-AR2(IR1), *+AR3(IR0), R5",    0x2765438A);
    TEST("SUBI3", "*++AR3(IR1), *AR4--%, R6",      0x27663C93);
    TEST("SUBI3", "*--AR4(IR1), *AR5++%, R7",      0x2767359C);
    TEST("SUBI3", "*AR5++(IR1), *AR6--, R0",       0x27602EA5);
    TEST("SUBI3", "*AR6--(IR1), *AR7++, R1",       0x276127AE);
    TEST("SUBI3", "*AR7++(IR1)%, *--AR0, R2",      0x276218B7);
    TEST("SUBI3", "*AR0--(IR1)%, *++AR1, R3",      0x276311B8);
    TEST("SUBI3", "*AR1, *-AR2, R4",               0x27640AC1);
    TEST("SUBI3", "*AR2++(IR0)B, *+AR3, R5",       0x276503CA);

    TEST("TSTB3", "R1, AR0", 0x27800801);
    TEST("TSTB3", "AR1, R0", 0x27800009);
    TEST("TSTB3", "*+AR1, *AR2++(IR0)B",       0x27E0CA01);
    TEST("TSTB3", "*-AR2, *AR3",               0x27E0C30A);
    TEST("TSTB3", "*++AR3, *AR4--(IR1)%",      0x27E0BC13);
    TEST("TSTB3", "*--AR4, *AR5++(IR1)%",      0x27E0B51C);
    TEST("TSTB3", "*AR5++, *AR6--(IR1)",       0x27E0AE25);
    TEST("TSTB3", "*AR6--, *AR7++(IR1)",       0x27E0A72E);
    TEST("TSTB3", "*AR7++%, *--AR0(IR1)",      0x27E09837);
    TEST("TSTB3", "*AR0--%, *++AR1(IR1)",      0x27E09138);
    TEST("TSTB3", "*+AR1(IR0), *-AR2(IR1)",    0x27E08A41);
    TEST("TSTB3", "*-AR2(IR0), *+AR3(IR1)",    0x27E0834A);
    TEST("TSTB3", "*++AR3(IR0), *AR4--(IR0)%", 0x27E07C53);
    TEST("TSTB3", "*--AR4(IR0), *AR5++(IR0)%", 0x27E0755C);
    TEST("TSTB3", "*AR5++(IR0), *AR6--(IR0)",  0x27E06E65);
    TEST("TSTB3", "*AR6--(IR0), *AR7++(IR0)",  0x27E0676E);
    TEST("TSTB3", "*AR7++(IR0)%, *--AR0(IR0)", 0x27E05877);
    TEST("TSTB3", "*AR0--(IR0)%, *++AR1(IR0)", 0x27E05178);
    TEST("TSTB3", "*+AR1(IR1), *-AR2(IR0)",    0x27E04A81);
    TEST("TSTB3", "*-AR2(IR1), *+AR3(IR0)",    0x27E0438A);
    TEST("TSTB3", "*++AR3(IR1), *AR4--%",      0x27E03C93);
    TEST("TSTB3", "*--AR4(IR1), *AR5++%",      0x27E0359C);
    TEST("TSTB3", "*AR5++(IR1), *AR6--",       0x27E02EA5);
    TEST("TSTB3", "*AR6--(IR1), *AR7++",       0x27E027AE);
    TEST("TSTB3", "*AR7++(IR1)%, *--AR0",      0x27E018B7);
    TEST("TSTB3", "*AR0--(IR1)%, *++AR1",      0x27E011B8);
    TEST("TSTB3", "*AR1, *-AR2",               0x27E00AC1);
    TEST("TSTB3", "*AR2++(IR0)B, *+AR3",       0x27E003CA);

    TEST("XOR3", "R1, AR0, R2", 0x28020801);
    TEST("XOR3", "AR1, R0, R2", 0x28020009);
    TEST("XOR3", "R1, R0, AR2", 0x280A0001);
    TEST("XOR3", "*+AR1, *AR2++(IR0)B, R3",       0x2863CA01);
    TEST("XOR3", "*-AR2, *AR3, R4",               0x2864C30A);
    TEST("XOR3", "*++AR3, *AR4--(IR1)%, R5",      0x2865BC13);
    TEST("XOR3", "*--AR4, *AR5++(IR1)%, R6",      0x2866B51C);
    TEST("XOR3", "*AR5++, *AR6--(IR1), R7",       0x2867AE25);
    TEST("XOR3", "*AR6--, *AR7++(IR1), R0",       0x2860A72E);
    TEST("XOR3", "*AR7++%, *--AR0(IR1), R1",      0x28619837);
    TEST("XOR3", "*AR0--%, *++AR1(IR1), R2",      0x28629138);
    TEST("XOR3", "*+AR1(IR0), *-AR2(IR1), R4",    0x28648A41);
    TEST("XOR3", "*-AR2(IR0), *+AR3(IR1), R5",    0x2865834A);
    TEST("XOR3", "*++AR3(IR0), *AR4--(IR0)%, R6", 0x28667C53);
    TEST("XOR3", "*--AR4(IR0), *AR5++(IR0)%, R7", 0x2867755C);
    TEST("XOR3", "*AR5++(IR0), *AR6--(IR0), R0",  0x28606E65);
    TEST("XOR3", "*AR6--(IR0), *AR7++(IR0), R1",  0x2861676E);
    TEST("XOR3", "*AR7++(IR0)%, *--AR0(IR0), R2", 0x28625877);
    TEST("XOR3", "*AR0--(IR0)%, *++AR1(IR0), R3", 0x28635178);
    TEST("XOR3", "*+AR1(IR1), *-AR2(IR0), R4",    0x28644A81);
    TEST("XOR3", "*-AR2(IR1), *+AR3(IR0), R5",    0x2865438A);
    TEST("XOR3", "*++AR3(IR1), *AR4--%, R6",      0x28663C93);
    TEST("XOR3", "*--AR4(IR1), *AR5++%, R7",      0x2867359C);
    TEST("XOR3", "*AR5++(IR1), *AR6--, R0",       0x28602EA5);
    TEST("XOR3", "*AR6--(IR1), *AR7++, R1",       0x286127AE);
    TEST("XOR3", "*AR7++(IR1)%, *--AR0, R2",      0x286218B7);
    TEST("XOR3", "*AR0--(IR1)%, *++AR1, R3",      0x286311B8);
    TEST("XOR3", "*AR1, *-AR2, R4",               0x28640AC1);
    TEST("XOR3", "*AR2++(IR0)B, *+AR3, R5",       0x286503CA);
}

void test_program() {
    ATEST(0x10000, "BU",    "R0",  0x68000000);
    ATEST(0x10000, "BLO",   "R1",  0x68010001);
    ATEST(0x10000, "BLS",   "R2",  0x68020002);
    ATEST(0x10000, "BHI",   "R3",  0x68030003);
    ATEST(0x10000, "BHS",   "R4",  0x68040004);
    ATEST(0x10000, "BEQ",   "R5",  0x68050005);
    ATEST(0x10000, "BNE",   "R6",  0x68060006);
    ATEST(0x10000, "BLT",   "R7",  0x68070007);
    ATEST(0x10000, "BLE",   "AR0", 0x68080008);
    ATEST(0x10000, "BGT",   "AR1", 0x68090009);
    ATEST(0x10000, "BGE",   "AR2", 0x680A000A);
    UNKN(                          0x680B000B);
    ATEST(0x10000, "BNV",   "AR4", 0x680C000C);
    ATEST(0x10000, "BV",    "AR5", 0x680D000D);
    ATEST(0x10000, "BNUF",  "AR6", 0x680E000E);
    ATEST(0x10000, "BUF",   "AR7", 0x680F000F);
    ATEST(0x10000, "BNLV",  "DP",  0x68100010);
    ATEST(0x10000, "BLV",   "IR0", 0x68110011);
    ATEST(0x10000, "BNLUF", "IR1", 0x68120012);
    ATEST(0x10000, "BLUF",  "BK",  0x68130013);
    ATEST(0x10000, "BZUF",  "SP",  0x68140014);
    AERRT(0x10000, "BU", "", ILLEGAL_REGISTER, "", 0x6800001C);

    ATEST(0x10000, "BU",    "010001H", 0x6A000000);
    ATEST(0x10000, "BLO",   "010000H", 0x6A01FFFF);
    ATEST(0x10000, "BLS",   "018000H", 0x6A027FFF);
    ATEST(0x10000, "BHI",   "008001H", 0x6A038000);
    ATEST(0x10000, "BHS",   "010801H", 0x6A040800);
    ATEST(0x10000, "BEQ",   "00F801H", 0x6A05F800);
    ATEST(0x10000, "BNE",   "011001H", 0x6A061000);
    ATEST(0x10000, "BLT",   "012001H", 0x6A072000);
    ATEST(0x10000, "BLE",   "013001H", 0x6A083000);
    ATEST(0x10000, "BGT",   "014001H", 0x6A094000);
    ATEST(0x10000, "BGE",   "015001H", 0x6A0A5000);
    UNKN(                              0x6A0B6000);
    ATEST(0x10000, "BNV",   "017001H", 0x6A0C7000);
    ATEST(0x10000, "BV",    "008001H", 0x6A0D8000);
    ATEST(0x10000, "BNUF",  "009001H", 0x6A0E9000);
    ATEST(0x10000, "BUF",   "00A001H", 0x6A0FA000);
    ATEST(0x10000, "BNLV",  "00B001H", 0x6A10B000);
    ATEST(0x10000, "BLV",   "00C001H", 0x6A11C000);
    ATEST(0x10000, "BNLUF", "00D001H", 0x6A12D000);
    ATEST(0x10000, "BLUF",  "00E001H", 0x6A13E000);
    ATEST(0x10000, "BZUF",  "00F001H", 0x6A14F000);

    ATEST(0x10000, "BUD",    "RC",  0x6820001B);
    ATEST(0x10000, "BLOD",   "RE",  0x6821001A);
    ATEST(0x10000, "BLSD",   "RS",  0x68220019);
    ATEST(0x10000, "BHID",   "IOF", 0x68230018);
    ATEST(0x10000, "BHSD",   "IF",  0x68240017);
    ATEST(0x10000, "BEQD",   "IE",  0x68250016);
    ATEST(0x10000, "BNED",   "ST",  0x68260015);
    ATEST(0x10000, "BLTD",   "SP",  0x68270014);
    ATEST(0x10000, "BLED",   "BK",  0x68280013);
    ATEST(0x10000, "BGTD",   "IR1", 0x68290012);
    ATEST(0x10000, "BGED",   "IR0", 0x682A0011);
    UNKN(                           0x682B0010);
    ATEST(0x10000, "BNVD",   "AR7", 0x682C000F);
    ATEST(0x10000, "BVD",    "AR6", 0x682D000E);
    ATEST(0x10000, "BNUFD",  "AR5", 0x682E000D);
    ATEST(0x10000, "BUFD",   "AR4", 0x682F000C);
    ATEST(0x10000, "BNLVD",  "AR3", 0x6830000B);
    ATEST(0x10000, "BLVD",   "AR2", 0x6831000A);
    ATEST(0x10000, "BNLUFD", "AR1", 0x68320009);
    ATEST(0x10000, "BLUFD",  "AR0", 0x68330008);
    ATEST(0x10000, "BZUFD",  "R7",  0x68340007);
    AERRT(0x10000, "BUD", "", ILLEGAL_REGISTER, "", 0x6820001C);

    ATEST(0x10000, "BUD",    "010003H", 0x6A200000);
    ATEST(0x10000, "BLOD",   "010002H", 0x6A21FFFF);
    ATEST(0x10000, "BLSD",   "018002H", 0x6A227FFF);
    ATEST(0x10000, "BHID",   "008003H", 0x6A238000);
    ATEST(0x10000, "BHSD",   "010803H", 0x6A240800);
    ATEST(0x10000, "BEQD",   "00F803H", 0x6A25F800);
    ATEST(0x10000, "BNED",   "011003H", 0x6A261000);
    ATEST(0x10000, "BLTD",   "012003H", 0x6A272000);
    ATEST(0x10000, "BLED",   "013003H", 0x6A283000);
    ATEST(0x10000, "BGTD",   "014003H", 0x6A294000);
    ATEST(0x10000, "BGED",   "015003H", 0x6A2A5000);
    UNKN(                               0x6A2B6000);
    ATEST(0x10000, "BNVD",   "017003H", 0x6A2C7000);
    ATEST(0x10000, "BVD",    "008003H", 0x6A2D8000);
    ATEST(0x10000, "BNUFD",  "009003H", 0x6A2E9000);
    ATEST(0x10000, "BUFD",   "00A003H", 0x6A2FA000);
    ATEST(0x10000, "BNLVD",  "00B003H", 0x6A30B000);
    ATEST(0x10000, "BLVD",   "00C003H", 0x6A31C000);
    ATEST(0x10000, "BNLUFD", "00D003H", 0x6A32D000);
    ATEST(0x10000, "BLUFD",  "00E003H", 0x6A33E000);
    ATEST(0x10000, "BZUFD",  "00F003H", 0x6A34F000);

    TEST("BR",   "123456H", 0x60123456);
    TEST("BRD",  "123456H", 0x61123456);
    TEST("CALL", "123456H", 0x62123456);

    ATEST(0x10000, "CALLU",    "R0",  0x70000000);
    ATEST(0x10000, "CALLLO",   "R1",  0x70010001);
    ATEST(0x10000, "CALLLS",   "R2",  0x70020002);
    ATEST(0x10000, "CALLHI",   "R3",  0x70030003);
    ATEST(0x10000, "CALLHS",   "R4",  0x70040004);
    ATEST(0x10000, "CALLEQ",   "R5",  0x70050005);
    ATEST(0x10000, "CALLNE",   "R6",  0x70060006);
    ATEST(0x10000, "CALLLT",   "R7",  0x70070007);
    ATEST(0x10000, "CALLLE",   "AR0", 0x70080008);
    ATEST(0x10000, "CALLGT",   "AR1", 0x70090009);
    ATEST(0x10000, "CALLGE",   "AR2", 0x700A000A);
    UNKN(                             0x700B000B);
    ATEST(0x10000, "CALLNV",   "AR4", 0x700C000C);
    ATEST(0x10000, "CALLV",    "AR5", 0x700D000D);
    ATEST(0x10000, "CALLNUF",  "AR6", 0x700E000E);
    ATEST(0x10000, "CALLUF",   "AR7", 0x700F000F);
    ATEST(0x10000, "CALLNLV",  "DP",  0x70100010);
    ATEST(0x10000, "CALLLV",   "IR0", 0x70110011);
    ATEST(0x10000, "CALLNLUF", "IR1", 0x70120012);
    ATEST(0x10000, "CALLLUF",  "BK",  0x70130013);
    ATEST(0x10000, "CALLZUF",  "SP",  0x70140014);
    AERRT(0x10000, "CALLU", "", ILLEGAL_REGISTER, "", 0x7000001C);

    ATEST(0x10000, "CALLU",    "010001H", 0x72000000);
    ATEST(0x10000, "CALLLO",   "010000H", 0x7201FFFF);
    ATEST(0x10000, "CALLLS",   "018000H", 0x72027FFF);
    ATEST(0x10000, "CALLHI",   "008001H", 0x72038000);
    ATEST(0x10000, "CALLHS",   "010801H", 0x72040800);
    ATEST(0x10000, "CALLEQ",   "00F801H", 0x7205F800);
    ATEST(0x10000, "CALLNE",   "011001H", 0x72061000);
    ATEST(0x10000, "CALLLT",   "012001H", 0x72072000);
    ATEST(0x10000, "CALLLE",   "013001H", 0x72083000);
    ATEST(0x10000, "CALLGT",   "014001H", 0x72094000);
    ATEST(0x10000, "CALLGE",   "015001H", 0x720A5000);
    UNKN(                                 0x720B6000);
    ATEST(0x10000, "CALLNV",   "017001H", 0x720C7000);
    ATEST(0x10000, "CALLV",    "008001H", 0x720D8000);
    ATEST(0x10000, "CALLNUF",  "009001H", 0x720E9000);
    ATEST(0x10000, "CALLUF",   "00A001H", 0x720FA000);
    ATEST(0x10000, "CALLNLV",  "00B001H", 0x7210B000);
    ATEST(0x10000, "CALLLV",   "00C001H", 0x7211C000);
    ATEST(0x10000, "CALLNLUF", "00D001H", 0x7212D000);
    ATEST(0x10000, "CALLLUF",  "00E001H", 0x7213E000);
    ATEST(0x10000, "CALLZUF",  "00F001H", 0x7214F000);

    ATEST(0x10000, "DBU",    "AR7, R0",  0x6DC00000);
    ATEST(0x10000, "DBLO",   "AR6, R1",  0x6D810001);
    ATEST(0x10000, "DBLS",   "AR5, R2",  0x6D420002);
    ATEST(0x10000, "DBHI",   "AR4, R3",  0x6D030003);
    ATEST(0x10000, "DBHS",   "AR3, R4",  0x6CC40004);
    ATEST(0x10000, "DBEQ",   "AR2, R5",  0x6C850005);
    ATEST(0x10000, "DBNE",   "AR1, R6",  0x6C460006);
    ATEST(0x10000, "DBLT",   "AR0, R7",  0x6C070007);
    ATEST(0x10000, "DBLE",   "AR7, AR0", 0x6DC80008);
    ATEST(0x10000, "DBGT",   "AR6, AR1", 0x6D890009);
    ATEST(0x10000, "DBGE",   "AR5, AR2", 0x6D4A000A);
    UNKN(                                0x6D0B000B);
    ATEST(0x10000, "DBNV",   "AR3, AR4", 0x6CCC000C);
    ATEST(0x10000, "DBV",    "AR2, AR5", 0x6C8D000D);
    ATEST(0x10000, "DBNUF",  "AR1, AR6", 0x6C4E000E);
    ATEST(0x10000, "DBUF",   "AR0, AR7", 0x6C0F000F);
    ATEST(0x10000, "DBNLV",  "AR7, DP",  0x6DD00010);
    ATEST(0x10000, "DBLV",   "AR6, IR0", 0x6D910011);
    ATEST(0x10000, "DBNLUF", "AR5, IR1", 0x6D520012);
    ATEST(0x10000, "DBLUF",  "AR4, BK",  0x6D130013);
    ATEST(0x10000, "DBZUF",  "AR3, SP",  0x6CD40014);
    AERRT(0x10000, "DBU", "AR2, ", ILLEGAL_REGISTER, "", 0x6C80001C);

    ATEST(0x10000, "DBU",    "AR0, 010001H", 0x6E000000);
    ATEST(0x10000, "DBLO",   "AR1, 010000H", 0x6E41FFFF);
    ATEST(0x10000, "DBLS",   "AR2, 018000H", 0x6E827FFF);
    ATEST(0x10000, "DBHI",   "AR3, 008001H", 0x6EC38000);
    ATEST(0x10000, "DBHS",   "AR4, 010801H", 0x6F040800);
    ATEST(0x10000, "DBEQ",   "AR5, 00F801H", 0x6F45F800);
    ATEST(0x10000, "DBNE",   "AR6, 011001H", 0x6F861000);
    ATEST(0x10000, "DBLT",   "AR7, 012001H", 0x6FC72000);
    ATEST(0x10000, "DBLE",   "AR0, 013001H", 0x6E083000);
    ATEST(0x10000, "DBGT",   "AR1, 014001H", 0x6E494000);
    ATEST(0x10000, "DBGE",   "AR2, 015001H", 0x6E8A5000);
    UNKN(                                    0x6ECB6000);
    ATEST(0x10000, "DBNV",   "AR4, 017001H", 0x6F0C7000);
    ATEST(0x10000, "DBV",    "AR5, 008001H", 0x6F4D8000);
    ATEST(0x10000, "DBNUF",  "AR6, 009001H", 0x6F8E9000);
    ATEST(0x10000, "DBUF",   "AR7, 00A001H", 0x6FCFA000);
    ATEST(0x10000, "DBNLV",  "AR0, 00B001H", 0x6E10B000);
    ATEST(0x10000, "DBLV",   "AR1, 00C001H", 0x6E51C000);
    ATEST(0x10000, "DBNLUF", "AR2, 00D001H", 0x6E92D000);
    ATEST(0x10000, "DBLUF",  "AR3, 00E001H", 0x6ED3E000);
    ATEST(0x10000, "DBZUF",  "AR4, 00F001H", 0x6F14F000);

    ATEST(0x10000, "DBUD",    "AR7, R0",  0x6DE00000);
    ATEST(0x10000, "DBLOD",   "AR6, R1",  0x6DA10001);
    ATEST(0x10000, "DBLSD",   "AR5, R2",  0x6D620002);
    ATEST(0x10000, "DBHID",   "AR4, R3",  0x6D230003);
    ATEST(0x10000, "DBHSD",   "AR3, R4",  0x6CE40004);
    ATEST(0x10000, "DBEQD",   "AR2, R5",  0x6CA50005);
    ATEST(0x10000, "DBNED",   "AR1, R6",  0x6C660006);
    ATEST(0x10000, "DBLTD",   "AR0, R7",  0x6C270007);
    ATEST(0x10000, "DBLED",   "AR7, AR0", 0x6DE80008);
    ATEST(0x10000, "DBGTD",   "AR6, AR1", 0x6DA90009);
    ATEST(0x10000, "DBGED",   "AR5, AR2", 0x6D6A000A);
    UNKN(                                 0x6D2B000B);
    ATEST(0x10000, "DBNVD",   "AR3, AR4", 0x6CEC000C);
    ATEST(0x10000, "DBVD",    "AR2, AR5", 0x6CAD000D);
    ATEST(0x10000, "DBNUFD",  "AR1, AR6", 0x6C6E000E);
    ATEST(0x10000, "DBUFD",   "AR0, AR7", 0x6C2F000F);
    ATEST(0x10000, "DBNLVD",  "AR7, DP",  0x6DF00010);
    ATEST(0x10000, "DBLVD",   "AR6, IR0", 0x6DB10011);
    ATEST(0x10000, "DBNLUFD", "AR5, IR1", 0x6D720012);
    ATEST(0x10000, "DBLUFD",  "AR4, BK",  0x6D330013);
    ATEST(0x10000, "DBZUFD",  "AR3, SP",  0x6CF40014);
    AERRT(0x10000, "DBUD", "AR2, ", ILLEGAL_REGISTER, "", 0x6CA0001C);

    ATEST(0x10000, "DBUD",    "AR0, 010003H", 0x6E200000);
    ATEST(0x10000, "DBLOD",   "AR1, 010002H", 0x6E61FFFF);
    ATEST(0x10000, "DBLSD",   "AR2, 018002H", 0x6EA27FFF);
    ATEST(0x10000, "DBHID",   "AR3, 008003H", 0x6EE38000);
    ATEST(0x10000, "DBHSD",   "AR4, 010803H", 0x6F240800);
    ATEST(0x10000, "DBEQD",   "AR5, 00F803H", 0x6F65F800);
    ATEST(0x10000, "DBNED",   "AR6, 011003H", 0x6FA61000);
    ATEST(0x10000, "DBLTD",   "AR7, 012003H", 0x6FE72000);
    ATEST(0x10000, "DBLED",   "AR0, 013003H", 0x6E283000);
    ATEST(0x10000, "DBGTD",   "AR1, 014003H", 0x6E694000);
    ATEST(0x10000, "DBGED",   "AR2, 015003H", 0x6EAA5000);
    UNKN(                                     0x6EEB6000);
    ATEST(0x10000, "DBNVD",   "AR4, 017003H", 0x6F2C7000);
    ATEST(0x10000, "DBVD",    "AR5, 008003H", 0x6F6D8000);
    ATEST(0x10000, "DBNUFD",  "AR6, 009003H", 0x6FAE9000);
    ATEST(0x10000, "DBUFD",   "AR7, 00A003H", 0x6FEFA000);
    ATEST(0x10000, "DBNLVD",  "AR0, 00B003H", 0x6E30B000);
    ATEST(0x10000, "DBLVD",   "AR1, 00C003H", 0x6E71C000);
    ATEST(0x10000, "DBNLUFD", "AR2, 00D003H", 0x6EB2D000);
    ATEST(0x10000, "DBLUFD",  "AR3, 00E003H", 0x6EF3E000);
    ATEST(0x10000, "DBZUFD",  "AR4, 00F003H", 0x6F34F000);

    ERRT("IACK", "R0",  REGISTER_NOT_ALLOWED, "R0",   0x1B000000);
    ERRT("IACK", "AR0", REGISTER_NOT_ALLOWED, "AR0",  0x1B000008);
    TEST("IACK", "@000001H",     0x1B200001);
    TEST("IACK", "*+AR1(2)",     0x1B400102);
    TEST("IACK", "*-AR2(3)",     0x1B400A03);
    TEST("IACK", "*++AR3(4)",    0x1B401304);
    TEST("IACK", "*--AR4(5)",    0x1B401C05);
    TEST("IACK", "*AR5++(6)",    0x1B402506);
    TEST("IACK", "*AR6--(7)",    0x1B402E07);
    TEST("IACK", "*AR7++(8)%",   0x1B403708);
    TEST("IACK", "*AR0--(9)%",   0x1B403809);
    TEST("IACK", "*+AR1(IR0)",   0x1B404100);
    TEST("IACK", "*-AR2(IR0)",   0x1B404A00);
    TEST("IACK", "*++AR3(IR0)",  0x1B405300);
    TEST("IACK", "*--AR4(IR0)",  0x1B405C00);
    TEST("IACK", "*AR5++(IR0)",  0x1B406500);
    TEST("IACK", "*AR6--(IR0)",  0x1B406E00);
    TEST("IACK", "*AR7++(IR0)%", 0x1B407700);
    TEST("IACK", "*AR0--(IR0)%", 0x1B407800);
    TEST("IACK", "*+AR1(IR1)",   0x1B408100);
    TEST("IACK", "*-AR2(IR1)",   0x1B408A00);
    TEST("IACK", "*++AR3(IR1)",  0x1B409300);
    TEST("IACK", "*--AR4(IR1)",  0x1B409C00);
    TEST("IACK", "*AR5++(IR1)",  0x1B40A500);
    TEST("IACK", "*AR6--(IR1)",  0x1B40AE00);
    TEST("IACK", "*AR7++(IR1)%", 0x1B40B700);
    TEST("IACK", "*AR0--(IR1)%", 0x1B40B800);
    TEST("IACK", "*AR1",         0x1B40C100);
    TEST("IACK", "*AR2++(IR0)B", 0x1B40CA00);
    ERRT("IACK", "0",     ILLEGAL_OPERAND_MODE, "0",     0x1B600000);
    ERRT("IACK", "0C00H", ILLEGAL_OPERAND_MODE, "0C00H", 0x1B600C00);

    TEST("IDLE", "", 0x06000000);

    TEST("NOP", "R0",           0x0C800000);
    TEST("NOP", "AR0",          0x0C800008);
    ERRT("NOP", "@000001H", ILLEGAL_OPERAND_MODE, "@000001H", 0x0CA00001);
    TEST("NOP", "*+AR1(2)",     0x0CC00102);
    TEST("NOP", "*-AR2(3)",     0x0CC00A03);
    TEST("NOP", "*++AR3(4)",    0x0CC01304);
    TEST("NOP", "*--AR4(5)",    0x0CC01C05);
    TEST("NOP", "*AR5++(6)",    0x0CC02506);
    TEST("NOP", "*AR6--(7)",    0x0CC02E07);
    TEST("NOP", "*AR7++(8)%",   0x0CC03708);
    TEST("NOP", "*AR0--(9)%",   0x0CC03809);
    TEST("NOP", "*+AR1(IR0)",   0x0CC04100);
    TEST("NOP", "*-AR2(IR0)",   0x0CC04A00);
    TEST("NOP", "*++AR3(IR0)",  0x0CC05300);
    TEST("NOP", "*--AR4(IR0)",  0x0CC05C00);
    TEST("NOP", "*AR5++(IR0)",  0x0CC06500);
    TEST("NOP", "*AR6--(IR0)",  0x0CC06E00);
    TEST("NOP", "*AR7++(IR0)%", 0x0CC07700);
    TEST("NOP", "*AR0--(IR0)%", 0x0CC07800);
    TEST("NOP", "*+AR1(IR1)",   0x0CC08100);
    TEST("NOP", "*-AR2(IR1)",   0x0CC08A00);
    TEST("NOP", "*++AR3(IR1)",  0x0CC09300);
    TEST("NOP", "*--AR4(IR1)",  0x0CC09C00);
    TEST("NOP", "*AR5++(IR1)",  0x0CC0A500);
    TEST("NOP", "*AR6--(IR1)",  0x0CC0AE00);
    TEST("NOP", "*AR7++(IR1)%", 0x0CC0B700);
    TEST("NOP", "*AR0--(IR1)%", 0x0CC0B800);
    TEST("NOP", "*AR1",         0x0CC0C100);
    TEST("NOP", "*AR2++(IR0)B", 0x0CC0CA00);
    ERRT("NOP", "0",     ILLEGAL_OPERAND_MODE, "0",     0x0CE00000);
    ERRT("NOP", "0C00H", ILLEGAL_OPERAND_MODE, "0C00H", 0x0CE00C00);

    TEST("RETIU",    "", 0x78000000);
    TEST("RETILO",   "", 0x78010000);
    TEST("RETILS",   "", 0x78020000);
    TEST("RETIHI",   "", 0x78030000);
    TEST("RETIHS",   "", 0x78040000);
    TEST("RETIEQ",   "", 0x78050000);
    TEST("RETINE",   "", 0x78060000);
    TEST("RETILT",   "", 0x78070000);
    TEST("RETILE",   "", 0x78080000);
    TEST("RETIGT",   "", 0x78090000);
    TEST("RETIGE",   "", 0x780A0000);
    UNKN(                0x780B0000);
    TEST("RETINV",   "", 0x780C0000);
    TEST("RETIV",    "", 0x780D0000);
    TEST("RETINUF",  "", 0x780E0000);
    TEST("RETIUF",   "", 0x780F0000);
    TEST("RETINLV",  "", 0x78100000);
    TEST("RETILV",   "", 0x78110000);
    TEST("RETINLUF", "", 0x78120000);
    TEST("RETILUF",  "", 0x78130000);
    TEST("RETIZUF",  "", 0x78140000);

    TEST("RETSU",    "", 0x78800000);
    TEST("RETSLO",   "", 0x78810000);
    TEST("RETSLS",   "", 0x78820000);
    TEST("RETSHI",   "", 0x78830000);
    TEST("RETSHS",   "", 0x78840000);
    TEST("RETSEQ",   "", 0x78850000);
    TEST("RETSNE",   "", 0x78860000);
    TEST("RETSLT",   "", 0x78870000);
    TEST("RETSLE",   "", 0x78880000);
    TEST("RETSGT",   "", 0x78890000);
    TEST("RETSGE",   "", 0x788A0000);
    UNKN(                0x788B0000);
    TEST("RETSNV",   "", 0x788C0000);
    TEST("RETSV",    "", 0x788D0000);
    TEST("RETSNUF",  "", 0x788E0000);
    TEST("RETSUF",   "", 0x788F0000);
    TEST("RETSNLV",  "", 0x78900000);
    TEST("RETSLV",   "", 0x78910000);
    TEST("RETSNLUF", "", 0x78920000);
    TEST("RETSLUF",  "", 0x78930000);
    TEST("RETSZUF",  "", 0x78940000);

    TEST("RPTB", "123456H", 0x64123456);

    TEST("RPTS", "R0",           0x139B0000);
    TEST("RPTS", "AR0",          0x139B0008);
    TEST("RPTS", "@000001H",     0x13BB0001);
    TEST("RPTS", "*+AR1(2)",     0x13DB0102);
    TEST("RPTS", "*-AR2(3)",     0x13DB0A03);
    TEST("RPTS", "*++AR3(4)",    0x13DB1304);
    TEST("RPTS", "*--AR4(5)",    0x13DB1C05);
    TEST("RPTS", "*AR5++(6)",    0x13DB2506);
    TEST("RPTS", "*AR6--(7)",    0x13DB2E07);
    TEST("RPTS", "*AR7++(8)%",   0x13DB3708);
    TEST("RPTS", "*AR0--(9)%",   0x13DB3809);
    TEST("RPTS", "*+AR1(IR0)",   0x13DB4100);
    TEST("RPTS", "*-AR2(IR0)",   0x13DB4A00);
    TEST("RPTS", "*++AR3(IR0)",  0x13DB5300);
    TEST("RPTS", "*--AR4(IR0)",  0x13DB5C00);
    TEST("RPTS", "*AR5++(IR0)",  0x13DB6500);
    TEST("RPTS", "*AR6--(IR0)",  0x13DB6E00);
    TEST("RPTS", "*AR7++(IR0)%", 0x13DB7700);
    TEST("RPTS", "*AR0--(IR0)%", 0x13DB7800);
    TEST("RPTS", "*+AR1(IR1)",   0x13DB8100);
    TEST("RPTS", "*-AR2(IR1)",   0x13DB8A00);
    TEST("RPTS", "*++AR3(IR1)",  0x13DB9300);
    TEST("RPTS", "*--AR4(IR1)",  0x13DB9C00);
    TEST("RPTS", "*AR5++(IR1)",  0x13DBA500);
    TEST("RPTS", "*AR6--(IR1)",  0x13DBAE00);
    TEST("RPTS", "*AR7++(IR1)%", 0x13DBB700);
    TEST("RPTS", "*AR0--(IR1)%", 0x13DBB800);
    TEST("RPTS", "*AR1",         0x13DBC100);
    TEST("RPTS", "*AR2++(IR0)B", 0x13DBCA00);
    TEST("RPTS", "0",            0x13FB0000);
    TEST("RPTS", "0C00H",        0x13FB0C00);
    TEST("RPTS", "8000H",        0x13FB8000);
    TEST("RPTS", "0FFFFH",       0x13FBFFFF);

    TEST("SWI", "", 0x66000000);

    TEST("TRAPU",    "0",  0x74000020);
    TEST("TRAPLO",   "1",  0x74010021);
    TEST("TRAPLS",   "2",  0x74020022);
    TEST("TRAPHI",   "3",  0x74030023);
    TEST("TRAPHS",   "4",  0x74040024);
    TEST("TRAPEQ",   "5",  0x74050025);
    TEST("TRAPNE",   "6",  0x74060026);
    TEST("TRAPLT",   "7",  0x74070027);
    TEST("TRAPLE",   "8",  0x74080028);
    TEST("TRAPGT",   "9",  0x74090029);
    TEST("TRAPGE",   "10", 0x740A002A);
    UNKN(                  0x740B002B);
    TEST("TRAPNV",   "12", 0x740C002C);
    TEST("TRAPV",    "13", 0x740D002D);
    TEST("TRAPNUF",  "14", 0x740E002E);
    TEST("TRAPUF",   "15", 0x740F002F);
    TEST("TRAPNLV",  "16", 0x74100030);
    TEST("TRAPLV",   "17", 0x74110031);
    TEST("TRAPNLUF", "18", 0x74120032);
    TEST("TRAPLUF",  "19", 0x74130033);
    TEST("TRAPZUF",  "31", 0x7414003F);
    ERRT("TRAPU", "0", ILLEGAL_OPERAND, "0", 0x74000000);
    ERRT("TRAPU", "32768", ILLEGAL_OPERAND, "32768", 0x74008000);
}

void test_misc() {
    if (is320c31()) {
        TEST("IDLE2",    "", 0x06000001);
        TEST("LOPOWER",  "", 0x10800001);
        TEST("MAXSPEED", "", 0x10800000);
    } else {
        UNKN(0x06000001);
        UNKN(0x10800001);
        UNKN(0x10800000);
    }
}

void test_interlock() {
    TEST("SIGI",     "", 0x16000000);

    ERRT("LDFI", "R0, R1",  REGISTER_NOT_ALLOWED, "R0, R1",  0x07810000);
    ERRT("LDFI", "AR0, R1", REGISTER_NOT_ALLOWED, "AR0, R1", 0x07810008);
    TEST("LDFI", "@000001H, R2",   0x07A20001);
    TEST("LDFI", "*+AR1(2), R3",   0x07C30102);
    TEST("LDFI", "*-AR2(3), R4",   0x07C40A03);
    TEST("LDFI", "*++AR3(4), R5",  0x07C51304);
    TEST("LDFI", "*--AR4(5), R6",  0x07C61C05);
    TEST("LDFI", "*AR5++(6), R7",  0x07C72506);
    TEST("LDFI", "*AR6--(7), R0",  0x07C02E07);
    TEST("LDFI", "*AR7++(8)%, R1", 0x07C13708);
    TEST("LDFI", "*AR0--(9)%, R2", 0x07C23809);
    TEST("LDFI", "*+AR1(IR0), R3",   0x07C34100);
    TEST("LDFI", "*-AR2(IR0), R4",   0x07C44A00);
    TEST("LDFI", "*++AR3(IR0), R5",  0x07C55300);
    TEST("LDFI", "*--AR4(IR0), R6",  0x07C65C00);
    TEST("LDFI", "*AR5++(IR0), R7",  0x07C76500);
    TEST("LDFI", "*AR6--(IR0), R0",  0x07C06E00);
    TEST("LDFI", "*AR7++(IR0)%, R1", 0x07C17700);
    TEST("LDFI", "*AR0--(IR0)%, R2", 0x07C27800);
    TEST("LDFI", "*+AR1(IR1), R3",   0x07C38100);
    TEST("LDFI", "*-AR2(IR1), R4",   0x07C48A00);
    TEST("LDFI", "*++AR3(IR1), R5",  0x07C59300);
    TEST("LDFI", "*--AR4(IR1), R6",  0x07C69C00);
    TEST("LDFI", "*AR5++(IR1), R7",  0x07C7A500);
    TEST("LDFI", "*AR6--(IR1), R0",  0x07C0AE00);
    TEST("LDFI", "*AR7++(IR1)%, R1", 0x07C1B700);
    TEST("LDFI", "*AR0--(IR1)%, R2", 0x07C2B800);
    TEST("LDFI", "*AR1, R3",         0x07C3C100);
    TEST("LDFI", "*AR2++(IR0)B, R4", 0x07C4CA00);
    ERRT("LDFI", "1, R5",    ILLEGAL_OPERAND_MODE, "1, R5",    0x07E50000);
    ERRT("LDFI", "-1.5, R6", ILLEGAL_OPERAND_MODE, "-1.5, R6", 0x07E60C00);

    ERRT("LDII", "R0, R1",  REGISTER_NOT_ALLOWED, "R0, R1",  0x08810000);
    ERRT("LDII", "AR0, R1", REGISTER_NOT_ALLOWED, "AR0, R1", 0x08810008);
    TEST("LDII", "@000001H, R2",   0x08A20001);
    TEST("LDII", "*+AR1(2), R3",   0x08C30102);
    TEST("LDII", "*-AR2(3), R4",   0x08C40A03);
    TEST("LDII", "*++AR3(4), R5",  0x08C51304);
    TEST("LDII", "*--AR4(5), R6",  0x08C61C05);
    TEST("LDII", "*AR5++(6), R7",  0x08C72506);
    TEST("LDII", "*AR6--(7), R0",  0x08C02E07);
    TEST("LDII", "*AR7++(8)%, R1", 0x08C13708);
    TEST("LDII", "*AR0--(9)%, R2", 0x08C23809);
    TEST("LDII", "*+AR1(IR0), R3",   0x08C34100);
    TEST("LDII", "*-AR2(IR0), R4",   0x08C44A00);
    TEST("LDII", "*++AR3(IR0), R5",  0x08C55300);
    TEST("LDII", "*--AR4(IR0), R6",  0x08C65C00);
    TEST("LDII", "*AR5++(IR0), R7",  0x08C76500);
    TEST("LDII", "*AR6--(IR0), R0",  0x08C06E00);
    TEST("LDII", "*AR7++(IR0)%, R1", 0x08C17700);
    TEST("LDII", "*AR0--(IR0)%, R2", 0x08C27800);
    TEST("LDII", "*+AR1(IR1), R3",   0x08C38100);
    TEST("LDII", "*-AR2(IR1), R4",   0x08C48A00);
    TEST("LDII", "*++AR3(IR1), R5",  0x08C59300);
    TEST("LDII", "*--AR4(IR1), R6",  0x08C69C00);
    TEST("LDII", "*AR5++(IR1), R7",  0x08C7A500);
    TEST("LDII", "*AR6--(IR1), R0",  0x08C0AE00);
    TEST("LDII", "*AR7++(IR1)%, R1", 0x08C1B700);
    TEST("LDII", "*AR0--(IR1)%, R2", 0x08C2B800);
    TEST("LDII", "*AR1, R3",         0x08C3C100);
    TEST("LDII", "*AR2++(IR0)B, R4", 0x08C4CA00);
    ERRT("LDII", "0, R5",      ILLEGAL_OPERAND_MODE, "0, R5",      0x08E50000);
    ERRT("LDII", "-8000H, R6", ILLEGAL_OPERAND_MODE, "-8000H, R6", 0x08E68000);

    ERRT("STFI", "R1, R0",  REGISTER_NOT_ALLOWED, "R0",  0x14810000);
    ERRT("STFI", "R1, AR0", REGISTER_NOT_ALLOWED, "AR0", 0x14810008);
    TEST("STFI", "R2, @000001H",   0x14A20001);
    TEST("STFI", "R3, *+AR1(2)",   0x14C30102);
    TEST("STFI", "R4, *-AR2(3)",   0x14C40A03);
    TEST("STFI", "R5, *++AR3(4)",  0x14C51304);
    TEST("STFI", "R6, *--AR4(5)",  0x14C61C05);
    TEST("STFI", "R7, *AR5++(6)",  0x14C72506);
    TEST("STFI", "R0, *AR6--(7)",  0x14C02E07);
    TEST("STFI", "R1, *AR7++(8)%", 0x14C13708);
    TEST("STFI", "R2, *AR0--(9)%", 0x14C23809);
    TEST("STFI", "R3, *+AR1(IR0)",   0x14C34100);
    TEST("STFI", "R4, *-AR2(IR0)",   0x14C44A00);
    TEST("STFI", "R5, *++AR3(IR0)",  0x14C55300);
    TEST("STFI", "R6, *--AR4(IR0)",  0x14C65C00);
    TEST("STFI", "R7, *AR5++(IR0)",  0x14C76500);
    TEST("STFI", "R0, *AR6--(IR0)",  0x14C06E00);
    TEST("STFI", "R1, *AR7++(IR0)%", 0x14C17700);
    TEST("STFI", "R2, *AR0--(IR0)%", 0x14C27800);
    TEST("STFI", "R3, *+AR1(IR1)",   0x14C38100);
    TEST("STFI", "R4, *-AR2(IR1)",   0x14C48A00);
    TEST("STFI", "R5, *++AR3(IR1)",  0x14C59300);
    TEST("STFI", "R6, *--AR4(IR1)",  0x14C69C00);
    TEST("STFI", "R7, *AR5++(IR1)",  0x14C7A500);
    TEST("STFI", "R0, *AR6--(IR1)",  0x14C0AE00);
    TEST("STFI", "R1, *AR7++(IR1)%", 0x14C1B700);
    TEST("STFI", "R2, *AR0--(IR1)%", 0x14C2B800);
    TEST("STFI", "R3, *AR1",         0x14C3C100);
    TEST("STFI", "R4, *AR2++(IR0)B", 0x14C4CA00);
    ERRT("STFI", "R5, 1",    ILLEGAL_OPERAND_MODE, "1",    0x14E50000);
    ERRT("STFI", "R6, -1.5", ILLEGAL_OPERAND_MODE, "-1.5", 0x14E60C00);

    ERRT("STII", "R1, R0",  REGISTER_NOT_ALLOWED, "R0",   0x15810000);
    ERRT("STII", "R1, AR0", REGISTER_NOT_ALLOWED, "AR0",  0x15810008);
    TEST("STII", "R2, @000001H",   0x15A20001);
    TEST("STII", "R3, *+AR1(2)",   0x15C30102);
    TEST("STII", "R4, *-AR2(3)",   0x15C40A03);
    TEST("STII", "R5, *++AR3(4)",  0x15C51304);
    TEST("STII", "R6, *--AR4(5)",  0x15C61C05);
    TEST("STII", "R7, *AR5++(6)",  0x15C72506);
    TEST("STII", "R0, *AR6--(7)",  0x15C02E07);
    TEST("STII", "R1, *AR7++(8)%", 0x15C13708);
    TEST("STII", "R2, *AR0--(9)%", 0x15C23809);
    TEST("STII", "R3, *+AR1(IR0)",   0x15C34100);
    TEST("STII", "R4, *-AR2(IR0)",   0x15C44A00);
    TEST("STII", "R5, *++AR3(IR0)",  0x15C55300);
    TEST("STII", "R6, *--AR4(IR0)",  0x15C65C00);
    TEST("STII", "R7, *AR5++(IR0)",  0x15C76500);
    TEST("STII", "R0, *AR6--(IR0)",  0x15C06E00);
    TEST("STII", "R1, *AR7++(IR0)%", 0x15C17700);
    TEST("STII", "R2, *AR0--(IR0)%", 0x15C27800);
    TEST("STII", "R3, *+AR1(IR1)",   0x15C38100);
    TEST("STII", "R4, *-AR2(IR1)",   0x15C48A00);
    TEST("STII", "R5, *++AR3(IR1)",  0x15C59300);
    TEST("STII", "R6, *--AR4(IR1)",  0x15C69C00);
    TEST("STII", "R7, *AR5++(IR1)",  0x15C7A500);
    TEST("STII", "R0, *AR6--(IR1)",  0x15C0AE00);
    TEST("STII", "R1, *AR7++(IR1)%", 0x15C1B700);
    TEST("STII", "R2, *AR0--(IR1)%", 0x15C2B800);
    TEST("STII", "R3, *AR1",         0x15C3C100);
    TEST("STII", "R4, *AR2++(IR0)B", 0x15C4CA00);
    ERRT("STII", "R5, 0",     ILLEGAL_OPERAND_MODE, "0",     0x15E50000);
    ERRT("STII", "R6, 0C00H", ILLEGAL_OPERAND_MODE, "0C00H", 0x15E60C00);
}

void test_parallel() {
    PTEST("ABSF",  "*+AR0, R1",
          "STF",   "R2, *-AR3",            0xC8420B00);
    PTEST("ABSI",  "*++AR4, R5",
          "STI",   "R6, *--AR7",           0xCB461F14);
    PTEST("ADDF3", "*AR0++, R1, R2",
          "STF",   "R3, *AR4--",           0xCC8B2C20);
    PTEST("ADDI3", "*AR5++%, R6, R7",
          "STI",   "R0, *AR1--%",          0xCFF03935);
    PTEST("AND3",  "*+AR2(IR0), R3, R4",
          "STI",   "R5, *-AR6(IR0)",       0xD11D4E42);
    PTEST("ASH3",  "R6, *++AR7(IR0), R0",
          "STI",   "R1, *--AR2(IR0)",      0xD2315A57);
    PTEST("FIX",   "*AR3++(IR0), R4",
          "STI",   "R5, *AR6--(IR0)",      0xD5056E63);
    PTEST("FLOAT", "*AR7++(IR0)%, R0",
          "STF",   "R1, *AR2--(IR0)%",     0xD6017A77);
    PTEST("LDF",   "*+AR3(IR1), R4",
          "LDF",   "*-AR5(IR1), R6",       0xC5308D83);
    PTEST("LDF",   "*++AR7(IR1), R0",
          "STF",   "R1, *--AR2(IR1)",      0xD8019A97);
    PTEST("LDI",   "*AR3++(IR1), R4",
          "LDI",   "*AR5--(IR1), R6",      0xC730ADA3);
    PTEST("LDI",   "*AR4++(IR1)%, R5",
          "STI",   "R6, *AR7--(IR1)%",     0xDB46BFB4);
    PTEST("LSH3",  "R0, *AR1++(IR0)B, R2",
          "STI",   "R3, *AR4",             0xDC83C4C9);
    PTEST("MPYF3", "*+AR5, R6, R7",
          "STF",   "R0, *-AR1",            0xDFF00905);
    PTEST("MPYI3", "*++AR2, R3, R4",
          "STI",   "R5, *--AR6",           0xE11D1E12);
    PTEST("NEGF",  "*AR7++, R0",
          "STF",   "R1, *AR2--",           0xE2012A27);
    PTEST("NEGI",  "*AR3++%, R4",
          "STI",   "R5, *AR6--%",          0xE5053E33);
    PTEST("NOT",   "*+AR7(IR0), R0",
          "STI",   "R1, *-AR2(IR0)",       0xE6014A47);
    PTEST("OR3",   "*++AR3(IR0), R4, R5",
          "STI",   "R6, *--AR7(IR0)",      0xE9665F53);
    PTEST("STF",   "R0, *AR1++(IR0)",
          "STF",   "R2, *AR3--(IR0)",      0xC0026B61);
    PTEST("STI",   "R4, *AR5++(IR0)%",
          "STI",   "R6, *AR7--(IR0)%",     0xC3067F75);
    PTEST("SUBF3", "R0, *+AR1(IR1), R2",
          "STF",   "R3, *-AR4(IR1)",       0xEA838C81);
    PTEST("SUBI3", "R5, *++AR6(IR1), R7",
          "STI",   "R0, *--AR1(IR1)",      0xEDE89996);
    PTEST("XOR3",  "*AR2++(IR1), R3, R4",
          "STI",   "R5, *AR6--(IR1)",      0xEF1DAEA2);
    // duplicat destination registers
    PERRT("LDF",   "*+AR3(IR1), R4", OK, "",
          "LDF",   "*-AR5(IR1), R4", DUPLICATE_REGISTER, "R4", 0xC5208D83);
    PERRT("LDI",   "*AR3++(IR1), R4", OK, "",
          "LDI",   "*AR5--(IR1), R4", DUPLICATE_REGISTER, "R4", 0xC720ADA3);

    PTEST("MPYF3", "*AR1, *AR2, R0",
          "ADDF3", "R4, R5, R2",     0x802CC2C1);
    PTEST("MPYF3", "*AR1, R2, R0",
          "ADDF3", "R4, *AR5, R3",   0x8154C1C5);
    PTEST("MPYF3", "R1, R2, R1",
          "ADDF3", "*AR4, *AR5, R2", 0x8291C5C4);
    PTEST("MPYF3", "*AR1, R2, R1",
          "ADDF3", "*AR4, R5, R3",   0x83D5C1C4);

    PTEST("MPYF3", "*AR1, *AR2, R0",
          "SUBF3", "R4, R5, R2",     0x842CC2C1);
    PTEST("MPYF3", "*AR1, R2, R0",
          "SUBF3", "R4, *AR5, R3",   0x8554C1C5);
    PTEST("MPYF3", "R1, R2, R1",
          "SUBF3", "*AR4, *AR5, R2", 0x8691C5C4);
    PTEST("MPYF3", "*AR1, R2, R1",
          "SUBF3", "*AR4, R5, R3",   0x87D5C1C4);

    PTEST("MPYI3", "*AR1, *AR2, R0",
          "ADDI3", "R4, R5, R2",     0x882CC2C1);
    PTEST("MPYI3", "*AR1, R2, R0",
          "ADDI3", "R4, *AR5, R3",   0x8954C1C5);
    PTEST("MPYI3", "R1, R2, R1",
          "ADDI3", "*AR4, *AR5, R2", 0x8A91C5C4);
    PTEST("MPYI3", "*AR1, R2, R1",
          "ADDI3", "*AR4, R5, R3",   0x8BD5C1C4);

    PTEST("MPYI3", "*AR1, *AR2, R0",
          "SUBI3", "R4, R5, R2",     0x8C2CC2C1);
    PTEST("MPYI3", "*AR1, R2, R0",
          "SUBI3", "R4, *AR5, R3",   0x8D54C1C5);
    PTEST("MPYI3", "R1, R2, R1",
          "SUBI3", "*AR4, *AR5, R2", 0x8E91C5C4);
    PTEST("MPYI3", "*AR1, R2, R1",
          "SUBI3", "*AR4, R5, R3",   0x8FD5C1C4);

    if (is320c32()) {
        PTEST("ABSF",  "R0, R1",
              "STF",   "R2, *-AR3",        0xC8420BE0);
        PTEST("ABSI",  "AR4, R5",
              "STI",   "R6, *--AR7",       0xCB461FEC);
        PTEST("ADDF3", "R5, R1, R2",
              "STF",   "R3, *AR4--",       0xCC8B2CE5);
        PTEST("ADDI3", "AR5, R6, R7",
              "STI",   "R0, *AR1--%",      0xCFF039ED);
        PTEST("AND3",  "AR2, R3, R4",
              "STI",   "R5, *-AR6(IR0)",   0xD11D4EEA);
        PTEST("ASH3",  "R6, AR7, R0",
              "STI",   "R1, *--AR2(IR0)",  0xD2315AEF);
        PTEST("FIX",   "R3, R4",
              "STI",   "R5, *AR6--(IR0)",  0xD5056EE3);
        PTEST("FLOAT", "R7, R0",
              "STF",   "R1, *AR2--(IR0)%", 0xD6017AE7);
        PTEST("LDF",   "R3, R4",
              "LDF",   "*-AR5(IR1), R6",   0xC5308DE3);
        PTEST("LDF",   "R7, R0",
              "STF",   "R1, *--AR2(IR1)",  0xD8019AE7);
        PTEST("LDI",   "AR3, R4",
              "LDI",   "*AR5--(IR1), R6",  0xC730ADEB);
        PTEST("LDI",   "AR4, R5",
              "STI",   "R6, *AR7--(IR1)%", 0xDB46BFEC);
        PTEST("LSH3",  "R0, AR1, R2",
              "STI",   "R3, *AR4",         0xDC83C4E9);
        PTEST("MPYF3", "R5, R6, R7",
              "STF",   "R0, *-AR1",        0xDFF009E5);
        PTEST("MPYI3", "AR2, R3, R4",
              "STI",   "R5, *--AR6",       0xE11D1EEA);
        PTEST("NEGF",  "R7, R0",
              "STF",   "R1, *AR2--",       0xE2012AE7);
        PTEST("NEGI",  "AR3, R4",
              "STI",   "R5, *AR6--%",      0xE5053EEB);
        PTEST("NOT",   "AR7, R0",
              "STI",   "R1, *-AR2(IR0)",   0xE6014AEF);
        PTEST("OR3",   "AR3, R4, R5",
              "STI",   "R6, *--AR7(IR0)",  0xE9665FEB);
#if 0
        PTEST("STF",   "R0, R1",
              "STF",   "R2, *AR3--(IR0)",  0xC0026BE1);
        PTEST("STI",   "R4, AR5",
              "STI",   "R6, *AR7--(IR0)%", 0xC3067FED);
#endif
        PTEST("SUBF3", "R0, R1, R2",
              "STF",   "R3, *-AR4(IR1)",   0xEA838CE1);
        PTEST("SUBI3", "R5, AR6, R7",
              "STI",   "R0, *--AR1(IR1)",  0xEDE899EE);
        PTEST("XOR3",  "AR2, R3, R4",
              "STI",   "R5, *AR6--(IR1)",  0xEF1DAEEA);

        PTEST("MPYF3", "R1, *AR2, R0",
              "ADDF3", "R4, R5, R2",   0x802CC2E1);
        PTEST("MPYF3", "*AR1, R2, R0",
              "ADDF3", "R4, R5, R2",   0x802CE2C1);
        PTEST("MPYF3", "R1, R2, R0",
              "ADDF3", "R4, R5, R2",   0x802CE2E1);
        PTEST("MPYF3", "*AR1, R2, R0",
              "ADDF3", "R4, R5, R3",   0x8154C1E5);
        PTEST("MPYF3", "R1, R2, R0",
              "ADDF3", "R4, *AR5, R3", 0x8154E1C5);
        PTEST("MPYF3", "R1, R2, R0",
              "ADDF3", "R4, R5, R3",   0x8154E1E5);
        PTEST("MPYF3", "R1, R2, R1",
              "ADDF3", "R4, *AR5, R2", 0x8291C5E4);
        PTEST("MPYF3", "R1, R2, R1",
              "ADDF3", "*AR4, R5, R2", 0x8291E5C4);
        PTEST("MPYF3", "R1, R2, R1",
              "ADDF3", "R4, R5, R2",   0x8291E5E4);
        PTEST("MPYF3", "*AR1, R2, R1",
              "ADDF3", "R4, R5, R3",   0x83D5C1E4);
        PTEST("MPYF3", "R1, R2, R1",
              "ADDF3", "*AR4, R5, R3", 0x83D5E1C4);
        PTEST("MPYF3", "R1, R2, R1",
              "ADDF3", "R4, R5, R3",   0x83D5E1E4);

        PTEST("MPYF3", "R1, *AR2, R0",
              "SUBF3", "R4, R5, R2",   0x842CC2E1);
        PTEST("MPYF3", "*AR1, R2, R0",
              "SUBF3", "R4, R5, R2",   0x842CE2C1);
        PTEST("MPYF3", "R1, R2, R0",
              "SUBF3", "R4, R5, R2",   0x842CE2E1);
        PTEST("MPYF3", "*AR1, R2, R0",
              "SUBF3", "R4, R5, R3",   0x8554C1E5);
        PTEST("MPYF3", "R1, R2, R0",
              "SUBF3", "R4, *AR5, R3", 0x8554E1C5);
        PTEST("MPYF3", "R1, R2, R0",
              "SUBF3", "R4, R5, R3",   0x8554E1E5);
        PTEST("MPYF3", "R1, R2, R1",
              "SUBF3", "R4, *AR5, R2", 0x8691C5E4);
        PTEST("MPYF3", "R1, R2, R1",
              "SUBF3", "*AR4, R5, R2", 0x8691E5C4);
        PTEST("MPYF3", "R1, R2, R1",
              "SUBF3", "R4, R5, R2",   0x8691E5E4);
        PTEST("MPYF3", "*AR1, R2, R1",
              "SUBF3", "R4, R5, R3",   0x87D5C1E4);
        PTEST("MPYF3", "R1, R2, R1",
              "SUBF3", "*AR4, R5, R3", 0x87D5E1C4);
        PTEST("MPYF3", "R1, R2, R1",
              "SUBF3", "R4, R5, R3",   0x87D5E1E4);

        PTEST("MPYI3", "AR1, *AR2, R0",
              "ADDI3", "R4, R5, R2",    0x882CC2E9);
        PTEST("MPYI3", "*AR1, AR2, R0",
              "ADDI3", "R4, R5, R2",    0x882CEAC1);
        PTEST("MPYI3", "R1, R2, R0",
              "ADDI3", "R4, R5, R2",    0x882CE2E1);
        PTEST("MPYI3", "*AR1, R2, R0",
              "ADDI3", "R4, AR5, R3",   0x8954C1ED);
        PTEST("MPYI3", "AR1, R2, R0",
              "ADDI3", "R4, *AR5, R3",  0x8954E9C5);
        PTEST("MPYI3", "R1, R2, R0",
              "ADDI3", "R4, R5, R3",    0x8954E1E5);
        PTEST("MPYI3", "R1, R2, R1",
              "ADDI3", "AR4, *AR5, R2", 0x8A91C5EC);
        PTEST("MPYI3", "R1, R2, R1",
              "ADDI3", "*AR4, AR5, R2", 0x8A91EDC4);
        PTEST("MPYI3", "R1, R2, R1",
              "ADDI3", "R4, R5, R2",    0x8A91E5E4);
        PTEST("MPYI3", "*AR1, R2, R1",
              "ADDI3", "AR4, R5, R3",   0x8BD5C1EC);
        PTEST("MPYI3", "AR1, R2, R1",
              "ADDI3", "*AR4, R5, R3",  0x8BD5E9C4);
        PTEST("MPYI3", "R1, R2, R1",
              "ADDI3", "R4, R5, R3",    0x8BD5E1E4);

        PTEST("MPYI3", "AR1, *AR2, R0",
              "SUBI3", "R4, R5, R2",    0x8C2CC2E9);
        PTEST("MPYI3", "*AR1, AR2, R0",
              "SUBI3", "R4, R5, R2",    0x8C2CEAC1);
        PTEST("MPYI3", "R1, R2, R0",
              "SUBI3", "R4, R5, R2",    0x8C2CE2E1);
        PTEST("MPYI3", "*AR1, R2, R0",
              "SUBI3", "R4, AR5, R3",   0x8D54C1ED);
        PTEST("MPYI3", "AR1, R2, R0",
              "SUBI3", "R4, *AR5, R3",  0x8D54E9C5);
        PTEST("MPYI3", "R1, R2, R0",
              "SUBI3", "R4, R5, R3",    0x8D54E1E5);
        PTEST("MPYI3", "R1, R2, R1",
              "SUBI3", "AR4, *AR5, R2", 0x8E91C5EC);
        PTEST("MPYI3", "R1, R2, R1",
              "SUBI3", "*AR4, AR5, R2", 0x8E91EDC4);
        PTEST("MPYI3", "R1, R2, R1",
              "SUBI3", "R4, R5, R2",    0x8E91E5E4);
        PTEST("MPYI3", "*AR1, R2, R1",
              "SUBI3", "AR4, R5, R3",   0x8FD5C1EC);
        PTEST("MPYI3", "AR1, R2, R1",
              "SUBI3", "*AR4, R5, R3",  0x8FD5E9C4);
        PTEST("MPYI3", "R1, R2, R1",
              "SUBI3", "R4, R5, R3",    0x8FD5E1E4);
    } else {
        PERRT("ABSF",  ", R1",      ILLEGAL_OPERAND_MODE, ", R1",
              "STF",   "R2, *-AR3", OK, "",
              0xC8420BE0);

        PERRT("MPYI3", "*AR1, R2, R1", OK, "",
              "SUBI3", ", R5, R3",     ILLEGAL_OPERAND_MODE, ", R5, R3",
              0x8FD5C1EC);
        PERRT("MPYI3", ", R2, R1",     ILLEGAL_OPERAND_MODE, ", R2, R1",
              "SUBI3", "*AR4, R5, R3", OK, "",
              0x8FD5E9C4);
        PERRT("MPYI3", ", R2, R1", ILLEGAL_OPERAND_MODE, ", R2, R1",
              "SUBI3", ", R5, R3", ILLEGAL_OPERAND_MODE, ", R5, R3",
              0x8FD5E1E4);
    }
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
    RUN_TEST(test_interlock);
    RUN_TEST(test_parallel);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

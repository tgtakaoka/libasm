/*
 * Copyright 2026 Tadashi G. Takaoka
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

#include "dis_cp1600.h"
#include "test_dis_helper.h"

// DTEST/DERRT(name1, name2, opr2, opc1, ...) asserts a two-step decode:
// the first call decodes the prefix word |opc1| into mnemonic |name1| (typically
// "SDBD"); the second call decodes the prefixed instruction into |name2|/|opr2|.
// DERRT additionally asserts PREFIX_HAS_NO_EFFECT on the continuation.
#define __DTEST_ERR(file, line, err, name1, name2, opr2, opc1, ...)                     \
    do {                                                                                \
        const Config::opcode_t codes[] = {opc1, __VA_ARGS__};                           \
        const auto endian = disassembler.config().endian();                             \
        const auto unit = disassembler.config().addressUnit();                          \
        const ArrayMemory memory(0x0000, codes, sizeof(codes), endian, unit);           \
        auto reader = memory.iterator();                                                \
        Insn insn(memory.origin());                                                     \
        char actual_opr[64];                                                            \
        disassembler.decode(reader, insn, actual_opr, sizeof(actual_opr), &symtab);     \
        asserter.equals(file, line, name1, name1, insn);                                \
        asserter.equals(file, line, name1, "", actual_opr);                             \
        insn.reset(insn.address() + insn.length() / unit, 0);                           \
        disassembler.decode(reader, insn, actual_opr, sizeof(actual_opr), &symtab);     \
        ErrorAt expected;                                                               \
        expected.setError(err);                                                         \
        asserter.equals(file, line, name2, expected.errorText_P(), insn.errorText_P()); \
        asserter.equals(file, line, name2, name2, insn);                                \
        asserter.equals(file, line, name2, opr2, actual_opr);                           \
    } while (0)
#define DTEST(name1, name2, opr2, opc1, ...) \
    __DTEST_ERR(__FILE__, __LINE__, OK, name1, name2, opr2, opc1, __VA_ARGS__)
#define DERRT(name1, name2, opr2, opc1, ...) \
    __DTEST_ERR(__FILE__, __LINE__, PREFIX_HAS_NO_EFFECT, name1, name2, opr2, opc1, __VA_ARGS__)

using namespace libasm;
using namespace libasm::cp1600;
using namespace libasm::test;

DisCp1600 dis1600;
Disassembler &disassembler(dis1600);

void set_up() {
    disassembler.reset();
    dis1600.setSpAlias(true);
    dis1600.setPcAlias(true);
}

void tear_down() {
    symtab.reset();
}

void test_cpu() {
    EQUALS("cpu CP1600", true, disassembler.setCpu("CP1600"));
    EQUALS_P("cpu CP1600", "CP1600", disassembler.config().cpu_P());
}

// Internal control (exact opcode match)
void test_internal_control() {
    TEST("HLT", "", 0x0000);
    TEST("EIS", "", 0x0002);
    TEST("DIS", "", 0x0003);
    TEST("TCI", "", 0x0005);
    TEST("CLRC", "", 0x0006);
    TEST("SETC", "", 0x0007);
    // SDBD last: it sets _sdbdPrefix=true; set_up() resets before the next test.
    TEST("SDBD", "", 0x0001);
}

// Single-register operations (bits[2:0] = DDD)
void test_single_register() {
    TEST("INCR", "R0", 0x0008);
    TEST("INCR", "PC", 0x000F);
    TEST("DECR", "R3", 0x0013);
    TEST("COMR", "R0", 0x0018);
    TEST("NEGR", "PC", 0x0027);
    TEST("ADCR", "R0", 0x0028);
    TEST("GSWD", "R0", 0x0030);
    TEST("GSWD", "R3", 0x0033);
    TEST("NOP", "", 0x0034);
    TEST("NOP", "", 0x0035);
    TEST("SIN", "", 0x0036);
    TEST("SIN", "", 0x0037);
    TEST("RSWD", "R0", 0x0038);
    TEST("RSWD", "PC", 0x003F);
}

// Register shifts (bit[2]=count, bits[1:0]=R0-R3)
void test_register_shift() {
    TEST("SWAP", "R0", 0x0040);
    TEST("SWAP", "R3, 2", 0x0047);
    TEST("SLL", "R0", 0x0048);
    TEST("RLC", "R2", 0x0052);
    TEST("SLLC", "R0", 0x0058);
    TEST("SLR", "R1", 0x0061);
    TEST("SAR", "R2", 0x006A);
    TEST("RRC", "R3", 0x0073);
    TEST("SARC", "R0, 2", 0x007C);
}

// Register-to-register (bits[5:3]=SSS, bits[2:0]=DDD).  Canonical mnemonics win
// over TSTR/CLRR/JR aliases at decode time.
void test_register_to_register() {
    TEST("MOVR", "R0, R1", 0x0081);
    TEST("ADDR", "R2, R3", 0x00D3);
    TEST("SUBR", "R4, R5", 0x0125);
    TEST("CMPR", "SP, PC", 0x0177);
    TEST("ANDR", "R0, PC", 0x0187);
    TEST("XORR", "R1, R2", 0x01CA);
    TEST("TSTR", "R3", 0x009B);
    TEST("CLRR", "R0", 0x01C0);
    TEST("JR", "R5", 0x00AF);
}

// Data access: direct (MMM=000), indirect (MMM=1..6), immediate (MMM=111).
void test_data_access() {
    // Direct (MMM=000)
    TEST("MVO", "R0, X'1234'", 0x0240, 0x1234);
    TEST("MVI", "X'5678', R1", 0x0281, 0x5678);
    TEST("ADD", "X'9ABC', R2", 0x02C2, 0x9ABC);
    TEST("SUB", "X'DEF0', R3", 0x0303, 0xDEF0);
    TEST("CMP", "X'1234', R4", 0x0344, 0x1234);
    TEST("AND", "X'5678', R5", 0x0385, 0x5678);
    TEST("XOR", "X'9ABC', SP", 0x03C6, 0x9ABC);
    // Indirect (MMM=1..6)
    TEST("MVO@", "R0, R1", 0x0248);
    TEST("MVI@", "R3, R2", 0x029A);
    TEST("ADD@", "R4, R0", 0x02E0);
    TEST("SUB@", "R5, R3", 0x032B);
    TEST("CMP@", "R2, R4", 0x0354);
    TEST("AND@", "R1, R5", 0x038D);
    TEST("XOR@", "R3, SP", 0x03DE);
    // Immediate (MMM=111)
    TEST("MVOI", "R0, X'00'", 0x0278, 0x0000);
    TEST("MVII", "X'FF', PC", 0x02BF, 0x00FF);
    TEST("ADDI", "X'55', R1", 0x02F9, 0x0055);
    TEST("SUBI", "X'AA', R2", 0x033A, 0x00AA);
    TEST("CMPI", "X'FF', R3", 0x037B, 0x00FF);
    TEST("ANDI", "X'3C', R4", 0x03BC, 0x003C);
    TEST("XORI", "X'C3', R5", 0x03FD, 0x00C3);
}

// Branches (bit5=S direction, bits[2:0]=condition)
void test_branch() {
    // Forward: target = (PC+2) + disp; here PC=0x100 -> target = 0x105 (disp=3)
    ATEST(0x0100, "B", "X'0105'", 0x0200, 0x0003);
    ATEST(0x0100, "BC", "X'0105'", 0x0201, 0x0003);
    ATEST(0x0100, "BOV", "X'0105'", 0x0202, 0x0003);
    ATEST(0x0100, "BPL", "X'0105'", 0x0203, 0x0003);
    ATEST(0x0100, "BZE", "X'0105'", 0x0204, 0x0003);
    ATEST(0x0100, "BLT", "X'0105'", 0x0205, 0x0003);
    ATEST(0x0100, "BLE", "X'0105'", 0x0206, 0x0003);
    ATEST(0x0100, "BUSC", "X'0105'", 0x0207, 0x0003);
    ATEST(0x0100, "BNC", "X'0105'", 0x0209, 0x0003);
    ATEST(0x0100, "BNOV", "X'0105'", 0x020A, 0x0003);
    ATEST(0x0100, "BMI", "X'0105'", 0x020B, 0x0003);
    ATEST(0x0100, "BNZE", "X'0105'", 0x020C, 0x0003);
    ATEST(0x0100, "BGE", "X'0105'", 0x020D, 0x0003);
    ATEST(0x0100, "BGT", "X'0105'", 0x020E, 0x0003);
    ATEST(0x0100, "BESC", "X'0105'", 0x020F, 0x0003);
    // Backward: target = (PC+2) - disp - 1; here PC=0x100, disp=1 -> target=0x100
    ATEST(0x0100, "B", "X'0100'", 0x0220, 0x0001);
    // NOPP: never-taken; displacement word always 0, not displayed.
    ATEST(0x0100, "NOPP", "", 0x0208, 0x0000);
    // BEXT: external condition number 0..15 in bits[3:0] (list radix is octal)
    ATEST(0x0100, "BEXT", "X'0105', 0", 0x0210, 0x0003);
    ATEST(0x0100, "BEXT", "X'0105', 17", 0x021F, 0x0003);
}

// Jump and JSR (3-word instructions; word2 carries register/control bits)
void test_jump() {
    // J/JE/JD (BB=11 means no register save; ctrl in word2 bits[1:0])
    ATEST(0x0100, "J", "X'0124'", 0x0004, 0x0300, 0x0124);
    ATEST(0x0100, "JE", "X'0124'", 0x0004, 0x0301, 0x0124);
    ATEST(0x0100, "JD", "X'0124'", 0x0004, 0x0302, 0x0124);
    // JSR Rn for R4/R5/SP: BB = reg - 4
    ATEST(0x0100, "JSR", "R4, X'0124'", 0x0004, 0x0000, 0x0124);
    ATEST(0x0100, "JSR", "R5, X'0124'", 0x0004, 0x0100, 0x0124);
    ATEST(0x0100, "JSR", "SP, X'0124'", 0x0004, 0x0200, 0x0124);
    ATEST(0x0100, "JSRE", "R4, X'0124'", 0x0004, 0x0001, 0x0124);
    ATEST(0x0100, "JSRD", "SP, X'0124'", 0x0004, 0x0202, 0x0124);
}

// SDBD (double-byte data) prefix changes how the next instruction reads its
// immediate or indirect operand.  SDBD is also flagged when it has no effect.
// Standalone SDBD is covered by test_internal_control.
void test_sdbd() {
    // Immediate: 16-bit value formed from two words (low byte / high byte).
    DTEST("SDBD", "MVII", "X'1234', R0", 0x0001, 0x02B8, 0x0034, 0x0012);
    DTEST("SDBD", "ADDI", "X'ABCD', R1", 0x0001, 0x02F9, 0x00CD, 0x00AB);
    DTEST("SDBD", "SUBI", "X'5678', R2", 0x0001, 0x033A, 0x0078, 0x0056);
    DTEST("SDBD", "CMPI", "X'DEF0', R3", 0x0001, 0x037B, 0x00F0, 0x00DE);
    DTEST("SDBD", "ANDI", "X'1234', R4", 0x0001, 0x03BC, 0x0034, 0x0012);
    DTEST("SDBD", "XORI", "X'FFFF', R5", 0x0001, 0x03FD, 0x00FF, 0x00FF);
    // Indirect: instruction encoding unchanged; CPU reads two words via pointer.
    DTEST("SDBD", "MVI@", "R1, R0", 0x0001, 0x0288);
    DTEST("SDBD", "ADD@", "R4, R2", 0x0001, 0x02E2);
    DTEST("SDBD", "XOR@", "R5, PC", 0x0001, 0x03EF);
    // No-effect: SDBD before an unsupported instruction is flagged.
    DERRT("SDBD", "EIS", "", 0x0001, 0x0002);
    DERRT("SDBD", "MVO", "R0, X'1234'", 0x0001, 0x0240, 0x1234);
    DERRT("SDBD", "MVO@", "R0, R1", 0x0001, 0x0248);
    DERRT("SDBD", "MVOI", "R0, X'0034'", 0x0001, 0x0278, 0x0034);
    DERRT("SDBD", "MVI", "X'1234', R0", 0x0001, 0x0280, 0x1234);
    DERRT("SDBD", "PULR", "R0", 0x0001, 0x02B0);
}

// CP1600 uses only the low 10 bits of each opcode word; non-zero upper bits
// are flagged INVALID_INSTRUCTION.  Jump word2 ctrl=11 is reserved (UNKNOWN).
void test_illegal_cp1600() {
    ERRT("", "", INVALID_INSTRUCTION, "", 0xFC00);
    ERRT("", "", INVALID_INSTRUCTION, "", 0xFC01);
    ERRT("", "", INVALID_INSTRUCTION, "", 0xFC08);
    ERRT("", "", INVALID_INSTRUCTION, "", 0xFEBF);
    AERRT(0x0100, "", "", INVALID_INSTRUCTION, "", 0x0004, 0xFC00);
    AERRT(0x0100, "", "", INVALID_INSTRUCTION, "", 0x0004, 0xFFFF);
    AERRT(0x0100, "", "", UNKNOWN_INSTRUCTION, "", 0x0004, 0x0303);
    AERRT(0x0100, "", "", UNKNOWN_INSTRUCTION, "", 0x0004, 0x0003);
}

void run_tests(const char *cpu) {
    disassembler.setCpu(cpu);
    RUN_TEST(test_cpu);
    RUN_TEST(test_internal_control);
    RUN_TEST(test_single_register);
    RUN_TEST(test_register_shift);
    RUN_TEST(test_register_to_register);
    RUN_TEST(test_data_access);
    RUN_TEST(test_branch);
    RUN_TEST(test_jump);
    RUN_TEST(test_sdbd);
    RUN_TEST(test_illegal_cp1600);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

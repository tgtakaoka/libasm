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

#include "asm_cp1600.h"
#include "test_asm_helper.h"

// DTEST/DERRT(addr1, src1, opc1, addr2, eaddr2, src2, ...) asserts a two-step
// encode: |src1| (typically "SDBD") emits |opc1|, then |src2| is the prefixed
// instruction whose bytes are given by varargs.  DERRT additionally asserts
// the continuation reports PREFIX_HAS_NO_EFFECT.  Parameter order matches Z380's
// DTEST/DERRT so the two arches read the same way.
//
// SDBD cross-instruction state is carried on the Insn (Insn::state<T>), so
// both encodes must share the same Insn -- we hold it locally and pass it to
// the asm_assert / cont_assert helpers.
#define DTEST(addr1, src1, opc1, addr2, eaddr2, src2, ...)               \
    do {                                                                 \
        const Config::opcode_t e1[] = {opc1};                            \
        const Config::opcode_t e2[] = {__VA_ARGS__};                     \
        const auto endian = assembler.config().endian();                 \
        const auto unit = assembler.config().addressUnit();              \
        const ArrayMemory m1((addr1), e1, sizeof(e1), endian, unit);     \
        const ArrayMemory m2((addr2), e2, sizeof(e2), endian, unit);     \
        Insn insn(m1.origin());                                          \
        ErrorAt err0;                                                    \
        asm_assert(__FILE__, __LINE__, err0, src1, m1, insn);            \
        ErrorAt err1;                                                    \
        cont_assert(__FILE__, __LINE__, err1, src2, (eaddr2), m2, insn); \
    } while (0)
#define DERRT(addr1, src1, opc1, addr2, eaddr2, src2, ...)               \
    do {                                                                 \
        const Config::opcode_t e1[] = {opc1};                            \
        const Config::opcode_t e2[] = {__VA_ARGS__};                     \
        const auto endian = assembler.config().endian();                 \
        const auto unit = assembler.config().addressUnit();              \
        const ArrayMemory m1((addr1), e1, sizeof(e1), endian, unit);     \
        const ArrayMemory m2((addr2), e2, sizeof(e2), endian, unit);     \
        Insn insn(m1.origin());                                          \
        ErrorAt err0;                                                    \
        asm_assert(__FILE__, __LINE__, err0, src1, m1, insn);            \
        ErrorAt err1;                                                    \
        err1.setError("", PREFIX_HAS_NO_EFFECT);                         \
        cont_assert(__FILE__, __LINE__, err1, src2, (eaddr2), m2, insn); \
    } while (0)
// Like DTEST but asserts the prefixed instruction reports UNDEFINED_SYMBOL at
// |eat2| (used when its operand references an undefined symbol).
#define DERUS(addr1, src1, opc1, addr2, eaddr2, src2, eat2, ...)         \
    do {                                                                 \
        const Config::opcode_t e1[] = {opc1};                            \
        const Config::opcode_t e2[] = {__VA_ARGS__};                     \
        const auto endian = assembler.config().endian();                 \
        const auto unit = assembler.config().addressUnit();              \
        const ArrayMemory m1((addr1), e1, sizeof(e1), endian, unit);     \
        const ArrayMemory m2((addr2), e2, sizeof(e2), endian, unit);     \
        Insn insn(m1.origin());                                          \
        ErrorAt err0;                                                    \
        asm_assert(__FILE__, __LINE__, err0, src1, m1, insn);            \
        ErrorAt err1;                                                    \
        err1.setError((eat2), UNDEFINED_SYMBOL);                         \
        cont_assert(__FILE__, __LINE__, err1, src2, (eaddr2), m2, insn); \
    } while (0)

using namespace libasm;
using namespace libasm::cp1600;
using namespace libasm::test;

AsmCp1600 asm1600;
Assembler &assembler(asm1600);

void set_up() {
    assembler.reset();
}

void tear_down() {
    symtab.reset();
}

void test_cpu() {
    EQUALS("cpu CP1600", true, assembler.setCpu("CP1600"));
    EQUALS_P("cpu CP1600", "CP1600", assembler.config().cpu_P());
}

void test_internal_control() {
    TEST("HLT", 0x0000);
    TEST("EIS", 0x0002);
    TEST("DIS", 0x0003);
    TEST("TCI", 0x0005);
    TEST("CLRC", 0x0006);
    TEST("SETC", 0x0007);
}

// Single-register operations (bits[2:0] = DDD)
void test_single_register() {
    TEST("INCR R0", 0x0008);
    TEST("INCR R7", 0x000F);
    TEST("DECR R3", 0x0013);
    TEST("COMR R0", 0x0018);
    TEST("NEGR R7", 0x0027);
    TEST("ADCR R0", 0x0028);
    TEST("GSWD R0", 0x0030);
    TEST("GSWD R3", 0x0033);
    TEST("NOP", 0x0034);
    TEST("NOP   2", 0x0035);
    TEST("SIN", 0x0036);
    TEST("SIN   2", 0x0037);
    TEST("RSWD R0", 0x0038);
    TEST("RSWD R7", 0x003F);
}

// Register shifts (bit[2]=count, bits[1:0]=R0-R3)
void test_register_shift() {
    TEST("SWAP R0", 0x0040);
    TEST("SWAP R3, 2", 0x0047);
    TEST("SLL  R0", 0x0048);
    TEST("RLC  R2", 0x0052);
    TEST("SLLC R0", 0x0058);
    TEST("SLR  R1", 0x0061);
    TEST("SAR  R2", 0x006A);
    TEST("RRC  R3", 0x0073);
    TEST("SARC R0, 2", 0x007C);
}

// Register-to-register (bits[5:3]=SSS, bits[2:0]=DDD) plus aliases.
void test_register_to_register() {
    TEST("MOVR R0, R1", 0x0081);
    TEST("ADDR R2, R3", 0x00D3);
    TEST("SUBR R4, R5", 0x0125);
    TEST("CMPR R6, R7", 0x0177);
    TEST("ANDR R0, R7", 0x0187);
    TEST("XORR R1, R2", 0x01CA);
    TEST("TSTR R3", 0x009B);  // = MOVR R3, R3
    TEST("CLRR R0", 0x01C0);  // = XORR R0, R0
    TEST("JR   R5", 0x00AF);  // = MOVR R5, R7
}

// Data access: direct (MMM=000), indirect (MMM=1..6), immediate (MMM=111).
// PSHR/PULR are the R6-indexed indirect aliases.
void test_data_access() {
    // Direct (MMM=000)
    TEST("MVO R0, X'1234'", 0x0240, 0x1234);
    TEST("MVI X'5678', R1", 0x0281, 0x5678);
    TEST("ADD X'9ABC', R2", 0x02C2, 0x9ABC);
    TEST("SUB X'DEF0', R3", 0x0303, 0xDEF0);
    TEST("CMP X'1234', R4", 0x0344, 0x1234);
    TEST("AND X'5678', R5", 0x0385, 0x5678);
    TEST("XOR X'9ABC', R6", 0x03C6, 0x9ABC);
    // Indirect (MMM=1..6)
    TEST("MVO@ R0, R1", 0x0248);
    TEST("MVI@ R3, R2", 0x029A);
    TEST("ADD@ R4, R0", 0x02E0);
    TEST("SUB@ R5, R3", 0x032B);
    TEST("CMP@ R2, R4", 0x0354);
    TEST("AND@ R1, R5", 0x038D);
    TEST("XOR@ R3, R6", 0x03DE);
    TEST("PSHR R0", 0x0270);  // = MVO@ R0, R6
    TEST("PULR R7", 0x02B7);  // = MVI@ R6, R7
    // Immediate (MMM=111)
    TEST("MVOI R0, X'00'", 0x0278, 0x0000);
    TEST("MVII X'FF', R7", 0x02BF, 0x00FF);
    TEST("ADDI X'55', R1", 0x02F9, 0x0055);
    TEST("SUBI X'AA', R2", 0x033A, 0x00AA);
    TEST("CMPI X'FF', R3", 0x037B, 0x00FF);
    TEST("ANDI X'3C', R4", 0x03BC, 0x003C);
    TEST("XORI X'C3', R5", 0x03FD, 0x00C3);
}

// Branches; the assembler picks forward/backward direction automatically.
void test_branch() {
    // Forward to X'0105' from origin 0x0100: PC_after=0x102, disp=0x03
    ATEST(0x0100, "B    X'0105'", 0x0200, 0x0003);
    ATEST(0x0100, "BC   X'0105'", 0x0201, 0x0003);
    ATEST(0x0100, "BNC  X'0105'", 0x0209, 0x0003);
    ATEST(0x0100, "BGE  X'0105'", 0x020D, 0x0003);
    // Backward to X'0100' from origin 0x0100: PC_after=0x102, disp=0x01 (S=1)
    ATEST(0x0100, "B    X'0100'", 0x0220, 0x0001);
    // NOPP: never-taken (no operand)
    ATEST(0x0100, "NOPP", 0x0208, 0x0000);
    // BEXT condition 0..15 (input radix is octal: 17 = decimal 15)
    ATEST(0x0100, "BEXT X'0105', 0", 0x0210, 0x0003);
    ATEST(0x0100, "BEXT X'0105', 17", 0x021F, 0x0003);
}

// Jump and JSR (3-word instructions)
void test_jump() {
    ATEST(0x0100, "J     X'0124'", 0x0004, 0x0300, 0x0124);
    ATEST(0x0100, "JE    X'0124'", 0x0004, 0x0301, 0x0124);
    ATEST(0x0100, "JD    X'0124'", 0x0004, 0x0302, 0x0124);
    ATEST(0x0100, "JSR   R4, X'0124'", 0x0004, 0x0000, 0x0124);
    ATEST(0x0100, "JSR   R5, X'0124'", 0x0004, 0x0100, 0x0124);
    ATEST(0x0100, "JSR   R6, X'0124'", 0x0004, 0x0200, 0x0124);
    ATEST(0x0100, "JSRE  R4, X'0124'", 0x0004, 0x0001, 0x0124);
    ATEST(0x0100, "JSRD  R6, X'0124'", 0x0004, 0x0202, 0x0124);
}

// Data constant pseudos:
//   BYTE val      -> SDBD-style 16-bit split: lo byte word then hi byte word.
//   WORD val      -> one 16-bit word per value, big-endian storage.
//   RES n         -> n words of allocated space (no emission).
//   ZERO n        -> n zero-valued 16-bit words.
//   TEXT 1,'...'  -> one char per word, char in low byte.
//   TEXT 2,'...'  -> chars packed two per word, first in low byte.
void test_data_constant() {
    // BYTE: SDBD-format 16-bit (low byte word, high byte word).
    TEST("BYTE X'12'", 0x0012, 0x0000);
    TEST("BYTE X'1234'", 0x0034, 0x0012);
    TEST("BYTE X'12', X'34'", 0x0012, 0x0000, 0x0034, 0x0000);
    // Negative gets sign-extension in the high storage word.
    TEST("BYTE -1", 0x00FF, 0xFFFF);
    // WORD: straight 16-bit storage.
    TEST("WORD 0", 0x0000);
    TEST("WORD X'1234'", 0x1234);
    TEST("WORD X'1234', X'5678', X'9ABC'", 0x1234, 0x5678, 0x9ABC);
    // RES n: leaves n words uninitialised (assembler advances the location).
    TEST("RES 3");
    // ZERO n: emit n zero words.
    TEST("ZERO 3", 0x0000, 0x0000, 0x0000);
    // TEXT mode 1: each char in own word's low byte.
    TEST("TEXT 1, 'AB'", 0x0041, 0x0042);
    // TEXT mode 2: chars packed two per word, first in low byte.
    TEST("TEXT 2, 'AB'", 0x4241);
    TEST("TEXT 2, 'ABCD'", 0x4241, 0x4443);
    // TEXT mode 2 with odd-length string pads the tail with 0.
    TEST("TEXT 2, 'ABC'", 0x4241, 0x0043);
}

// SDBD (double-byte data) prefix changes how the next instruction reads its
// immediate or indirect operand.  SDBD is also flagged when it has no effect.
void test_sdbd() {
    // Immediate: 16-bit value emitted as two words holding low/high byte.
    DTEST(0x0100, "SDBD", 0x0001, 0x0101, 0x0101, "MVII X'1234', R0", 0x02B8, 0x0034, 0x0012);
    DTEST(0x0100, "SDBD", 0x0001, 0x0101, 0x0101, "ADDI X'ABCD', R1", 0x02F9, 0x00CD, 0x00AB);
    DTEST(0x0100, "SDBD", 0x0001, 0x0101, 0x0101, "SUBI X'5678', R2", 0x033A, 0x0078, 0x0056);
    DTEST(0x0100, "SDBD", 0x0001, 0x0101, 0x0101, "CMPI X'DEF0', R3", 0x037B, 0x00F0, 0x00DE);
    DTEST(0x0100, "SDBD", 0x0001, 0x0101, 0x0101, "ANDI X'1234', R4", 0x03BC, 0x0034, 0x0012);
    DTEST(0x0100, "SDBD", 0x0001, 0x0101, 0x0101, "XORI X'FFFF', R5", 0x03FD, 0x00FF, 0x00FF);
    // Indirect: instruction encoding unchanged; CPU reads two words via pointer.
    DTEST(0x0100, "SDBD", 0x0001, 0x0101, 0x0101, "MVI@ R1, R0", 0x0288);
    DTEST(0x0100, "SDBD", 0x0001, 0x0101, 0x0101, "ADD@ R4, R2", 0x02E2);
    DTEST(0x0100, "SDBD", 0x0001, 0x0101, 0x0101, "XOR@ R5, R7", 0x03EF);
    // No-effect: SDBD before an unsupported instruction is flagged.
    DERRT(0x0100, "SDBD", 0x0001, 0x0101, 0x0101, "EIS", 0x0002);
    DERRT(0x0100, "SDBD", 0x0001, 0x0101, 0x0101, "MVO R0, X'1234'", 0x0240, 0x1234);
    DERRT(0x0100, "SDBD", 0x0001, 0x0101, 0x0101, "MVO@ R0, R1", 0x0248);
    DERRT(0x0100, "SDBD", 0x0001, 0x0101, 0x0101, "MVOI R0, X'34'", 0x0278, 0x0034);
    DERRT(0x0100, "SDBD", 0x0001, 0x0101, 0x0101, "MVI X'1234', R0", 0x0280, 0x1234);
    DERRT(0x0100, "SDBD", 0x0001, 0x0101, 0x0101, "PULR R0", 0x02B0);
}

// Operand-validity errors the assembler should catch even when the mnemonic is
// well-formed.
void test_illegal_cp1600() {
    // Shift register must be R0-R3 (only 2 bits available for the reg index).
    ERRT("SWAP R4", REGISTER_NOT_ALLOWED, "R4", 0x0040);
    // Indirect register must be R1-R6 for MVO@ (mode 0 = direct, mode 7 = MVOI).
    ERRT("MVO@ R0, R0", REGISTER_NOT_ALLOWED, "R0", 0x0240);
    // JSR return register must be R4, R5, or R6.
    ERRT("JSR R3, X'0124'", REGISTER_NOT_ALLOWED, "R3, X'0124'", 0x0004, 0xFF00, 0x0124);
    ERRT("JSR R7, X'0124'", REGISTER_NOT_ALLOWED, "R7, X'0124'", 0x0004, 0x0300, 0x0124);
    // Trailing text after the operands is not an implicit comment (CP1600 uses
    // ';'), so it is rejected, not silently ignored.
    ERRT("TSTR R0 junk",   GARBAGE_AT_END, "junk");
    ERRT("MVI X'1234', R0 junk", GARBAGE_AT_END, "junk");
}

// Undefined symbol in every value-bearing operand position.  The assembler must
// report UNDEFINED_SYMBOL (pointing at the offending symbol) yet still emit the
// correct instruction length with the symbol taken as zero, so a first pass can
// size the code before the symbol is resolved.
void test_undef() {
    // M_DADDR -- direct address, source and destination positions.
    ERUS("MVI UNDEF, R1", "UNDEF, R1", 0x0281, 0x0000);
    ERUS("MVO R0, UNDEF", "UNDEF",     0x0240, 0x0000);
    // M_IMM16 -- immediate.
    ERUS("MVII UNDEF, R7", "UNDEF, R7", 0x02BF, 0x0000);
    ERUS("ADDI UNDEF, R1", "UNDEF, R1", 0x02F9, 0x0000);
    // M_IMM16 under an SDBD prefix: still one report, two operand words.
    DERUS(0x0100, "SDBD", 0x0001, 0x0101, 0x0101, "MVII UNDEF, R0", "UNDEF, R0",
            0x02B8, 0x0000, 0x0000);
    // M_JADDR -- jump / JSR target.
    AERUS(0x0100, "J        UNDEF", "UNDEF",     0x0004, 0x0300, 0x0000);
    AERUS(0x0100, "JSR  R4, UNDEF", "UNDEF",     0x0004, 0x0000, 0x0000);
    // M_BDISP -- branch displacement (undefined target encodes as zero offset).
    AERUS(0x0100, "B    UNDEF", "UNDEF", 0x0200, 0x0000);
    AERUS(0x0100, "BC   UNDEF", "UNDEF", 0x0201, 0x0000);
    // M_BCOND -- BEXT target and/or condition.
    AERUS(0x0100, "BEXT X'0105', UNDEF", "UNDEF",         0x0210, 0x0003);
    AERUS(0x0100, "BEXT UNDEF, 0",       "UNDEF, 0",      0x0210, 0x0000);
    AERUS(0x0100, "BEXT UNDEF, UNDEF",   "UNDEF, UNDEF",  0x0210, 0x0000);
    // M_SHCNT / M_BIT0 -- count operands (a symbol there is degenerate but must
    // still be diagnosed as undefined rather than silently accepted).
    ERUS("SWAP R0, UNDEF", "UNDEF", 0x0040);
    ERUS("NOP  UNDEF",     "UNDEF", 0x0034);
    ERUS("SIN  UNDEF",     "UNDEF", 0x0036);
}

void run_tests(const char *cpu) {
    assembler.setCpu(cpu);
    RUN_TEST(test_cpu);
    RUN_TEST(test_internal_control);
    RUN_TEST(test_single_register);
    RUN_TEST(test_register_shift);
    RUN_TEST(test_register_to_register);
    RUN_TEST(test_data_access);
    RUN_TEST(test_branch);
    RUN_TEST(test_jump);
    RUN_TEST(test_data_constant);
    RUN_TEST(test_sdbd);
    RUN_TEST(test_illegal_cp1600);
    RUN_TEST(test_undef);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

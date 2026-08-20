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

#include "asm_am29000.h"
#include "test_asm_helper.h"

using namespace libasm;
using namespace libasm::am29000;
using namespace libasm::test;

AsmAm29000 asm29000;
Assembler &assembler(asm29000);

void set_up() {
    assembler.reset();
}

void tear_down() {
    symtab.reset();
}

void test_cpu() {
    EQUALS("cpu AM29200", true, assembler.setCpu("AM29200"));
    EQUALS_P("cpu AM29200", "AM29200", assembler.config().cpu_P());

    EQUALS("cpu AM29245", true, assembler.setCpu("AM29245"));
    EQUALS_P("cpu AM29245", "AM29245", assembler.config().cpu_P());

    EQUALS("cpu AM29030", true, assembler.setCpu("AM29030"));
    EQUALS_P("cpu AM29030", "AM29030", assembler.config().cpu_P());

    EQUALS("cpu AM29240", true, assembler.setCpu("AM29240"));
    EQUALS_P("cpu AM29240", "AM29240", assembler.config().cpu_P());

    EQUALS("cpu AM29040", true, assembler.setCpu("AM29040"));
    EQUALS_P("cpu AM29040", "AM29040", assembler.config().cpu_P());

    EQUALS("cpu AM29050", true, assembler.setCpu("AM29050"));
    EQUALS_P("cpu AM29050", "AM29050", assembler.config().cpu_P());
}

// A register operand keeps the M bit of the operation code clear; an 8-bit
// constant raises it.
void test_integer_arithmetic() {
    TEST("ADD    gr96, gr97, gr98", 0x14606162);
    TEST("ADD    gr96, gr97, 15",   0x1560610F);
    TEST("ADD    lr0, lr1, lr2",    0x14808182);
    TEST("ADDS   gr96, gr97, gr98", 0x10606162);
    TEST("ADDU   gr96, gr97, gr98", 0x12606162);
    TEST("ADDC   gr96, gr97, gr98", 0x1C606162);
    TEST("ADDCS  gr96, gr97, gr98", 0x18606162);
    TEST("ADDCU  gr96, gr97, gr98", 0x1A606162);
    TEST("SUB    gr96, gr97, gr98", 0x24606162);
    TEST("SUBS   gr96, gr97, gr98", 0x20606162);
    TEST("SUBU   gr96, gr97, gr98", 0x22606162);
    TEST("SUBC   gr96, gr97, gr98", 0x2C606162);
    TEST("SUBCS  gr96, gr97, gr98", 0x28606162);
    TEST("SUBCU  gr96, gr97, gr98", 0x2A606162);
    TEST("SUBR   gr96, gr97, gr98", 0x34606162);
    TEST("SUBRS  gr96, gr97, gr98", 0x30606162);
    TEST("SUBRU  gr96, gr97, gr98", 0x32606162);
    TEST("SUBRC  gr96, gr97, gr98", 0x3C606162);
    TEST("SUBRCS gr96, gr97, gr98", 0x38606162);
    TEST("SUBRCU gr96, gr97, gr98", 0x3A606162);
}

// The RA field of DIV0 is reserved and is left clear.
void test_multiply_divide_step() {
    TEST("MUL    gr96, gr97, gr98", 0x64606162);
    TEST("MULL   gr96, gr97, gr98", 0x66606162);
    TEST("MULU   gr96, gr97, gr98", 0x74606162);
    TEST("DIV0   gr96, gr98",       0x68600062);
    TEST("DIV0   gr96, 15",         0x6960000F);
    TEST("DIV    gr96, gr97, gr98", 0x6A606162);
    TEST("DIVL   gr96, gr97, gr98", 0x6C606162);
    TEST("DIVREM gr96, gr97, gr98", 0x6E606162);
}

void test_compare() {
    TEST("CPEQ   gr96, gr97, gr98", 0x60606162);
    TEST("CPNEQ  gr96, gr97, gr98", 0x62606162);
    TEST("CPLT   gr96, gr97, gr98", 0x40606162);
    TEST("CPLTU  gr96, gr97, gr98", 0x42606162);
    TEST("CPLE   gr96, gr97, gr98", 0x44606162);
    TEST("CPLEU  gr96, gr97, gr98", 0x46606162);
    TEST("CPGT   gr96, gr97, gr98", 0x48606162);
    TEST("CPGTU  gr96, gr97, gr98", 0x4A606162);
    TEST("CPGE   gr96, gr97, gr98", 0x4C606162);
    TEST("CPGEU  gr96, gr97, gr98", 0x4E606162);
    TEST("CPBYTE gr96, gr97, gr98", 0x2E606162);
}

void test_assert() {
    TEST("ASEQ   64, gr97, gr98",   0x70406162);
    TEST("ASNEQ  65, gr97, gr98",   0x72416162);
    TEST("ASLT   66, gr97, gr98",   0x50426162);
    TEST("ASLTU  67, gr97, gr98",   0x52436162);
    TEST("ASLE   68, gr97, gr98",   0x54446162);
    TEST("ASLEU  69, gr97, gr98",   0x56456162);
    TEST("ASGT   70, gr97, gr98",   0x58466162);
    TEST("ASGTU  71, gr97, gr98",   0x5A476162);
    TEST("ASGE   72, gr97, gr98",   0x5C486162);
    TEST("ASGEU  255, gr97, 15",    0x5FFF610F);
}

void test_logical_shift() {
    TEST("AND     gr96, gr97, gr98", 0x90606162);
    TEST("ANDN    gr96, gr97, gr98", 0x9C606162);
    TEST("NAND    gr96, gr97, gr98", 0x9A606162);
    TEST("OR      gr96, gr97, gr98", 0x92606162);
    TEST("NOR     gr96, gr97, gr98", 0x98606162);
    TEST("XOR     gr96, gr97, gr98", 0x94606162);
    TEST("XNOR    gr96, gr97, gr98", 0x96606162);
    TEST("SLL     gr96, gr97, gr98", 0x80606162);
    TEST("SLL     gr96, gr97, 31",   0x8160611F);
    TEST("SRL     gr96, gr97, gr98", 0x82606162);
    TEST("SRA     gr96, gr97, gr98", 0x86606162);
    TEST("EXTRACT gr96, gr97, gr98", 0x7A606162);
}

void test_data_movement() {
    TEST("LOAD    0, 0, gr96, gr97",    0x16006061);
    TEST("LOAD    0, 0x40, gr96, 15",   0x1740600F);
    TEST("LOADL   0, 0, gr96, gr97",    0x06006061);
    TEST("LOADSET 0, 0, gr96, gr97",    0x26006061);
    TEST("LOADM   0, 0, gr96, gr97",    0x36006061);
    TEST("STORE   0, 0, gr96, gr97",    0x1E006061);
    TEST("STOREL  0, 0, gr96, gr97",    0x0E006061);
    TEST("STOREM  0, 0, gr96, gr97",    0x3E006061);
    TEST("EXBYTE  gr96, gr97, gr98",    0x0A606162);
    TEST("EXHW    gr96, gr97, gr98",    0x7C606162);
    TEST("EXHWS   gr96, gr97",          0x7E606100);
    TEST("INBYTE  gr96, gr97, gr98",    0x0C606162);
    TEST("INHW    gr96, gr97, gr98",    0x78606162);
    TEST("MFTLB   gr96, gr97",          0xB6606100);
    TEST("MTTLB   gr96, gr97",          0xBE006061);
}

void test_special_register() {
    TEST("MFSR   gr96, VAB",  0xC6600000);
    TEST("MFSR   gr96, CPS",  0xC6600200);
    TEST("MFSR   gr96, Q",    0xC6608300);
    TEST("MFSR   gr96, CR",   0xC6608700);
    TEST("MFSR   gr96, FPS",  0xC660A200);
    TEST("MTSR   VAB, gr96",  0xCE000060);
    TEST("MTSR   Q, gr96",    0xCE008360);
    TEST("MTSR   CR, gr96",   0xCE008760);
    TEST("MTSRIM Q, 0x1234",  0x04128334);
    TEST("MTSRIM CR, 0",      0x04008700);
    // A register number may stand in for a name, so long as it names a
    // register this CPU implements.
    TEST("MFSR   gr96, 0",    0xC6600000);
    TEST("MTSR   0x83, gr96", 0xCE008360);
    // A number no family member implements is refused, the same as the name of
    // a register which is not there.
    ERRT("MFSR   gr96, 0x7F", OPERAND_NOT_ALLOWED, "0x7F", 0xC6607F00);
    ERRT("MTSR   200, gr96",  OPERAND_NOT_ALLOWED, "200, gr96", 0xCE00C860);
    ERRT("MFSR   gr96, 256",  OVERFLOW_RANGE,      "256",       0xC6600000);
}

void test_constant() {
    TEST("CONST  gr96, 0x1234", 0x03126034);
    TEST("CONSTH gr96, 0x5678", 0x02566078);
    TEST("CONSTN gr96, 0x1234", 0x01126034);
}

// The source names the target address itself; a Program-Counter relative
// encoding is used whenever the target is in range.
void test_branch() {
    ATEST(0x1000, "JMP     0x1000",       0xA0000000);
    ATEST(0x1000, "JMP     0x0FF8",       0xA0FF00FE);
    ATEST(0x1000, "JMP     0x1008",       0xA0000002);
    ATEST(0x1000, "JMPF    gr97, 0x1000", 0xA4006100);
    ATEST(0x1000, "JMPT    gr97, 0x1000", 0xAC006100);
    ATEST(0x1000, "JMPFDEC gr97, 0x1000", 0xB4006100);
    ATEST(0x1000, "CALL    gr96, 0x1000", 0xA8006000);
    TEST("JMPI  gr98",       0xC0000062);
    TEST("JMPFI gr97, gr98", 0xC4006162);
    TEST("JMPTI gr97, gr98", 0xCC006162);
    TEST("CALLI gr96, gr98", 0xC8006062);
}

void test_miscellaneous() {
    TEST("CLZ     gr96, gr98",       0x08600062);
    TEST("CLZ     gr96, 15",         0x0960000F);
    TEST("SETIP   gr96, gr97, gr98", 0x9E606162);
    TEST("EMULATE 64, gr97, gr98",   0xD7406162);
    TEST("INV",                      0x9F000000);
    TEST("INV     0",                0x9F000000);
    TEST("INV     2",                0x9F020000);
    TEST("IRET",                     0x88000000);
    TEST("IRETINV",                  0x8C000000);
    TEST("IRETINV 1",                0x8C010000);
    TEST("HALT",                     0x89000000);
}

void test_mmu_register() {
    // Where the register exists, its number is accepted too.
    TEST("MFSR gr96, 13", 0xC6600D00);
    TEST("MFSR gr96, MMU", 0xC6600D00);
    TEST("MFSR gr96, LRU", 0xC6600E00);
    TEST("MTSR MMU, gr96", 0xCE000D60);
}

void test_no_mmu_register() {
    // The number must be gated exactly as the name is, or it would be a way
    // around the check.
    ERRT("MFSR gr96, 13", OPERAND_NOT_ALLOWED, "13", 0xC6600D00);
    ERRT("MTSR 14, gr96", OPERAND_NOT_ALLOWED, "14, gr96", 0xCE000E60);
    // The Am29200 leaves those register numbers unimplemented, so the names are
    // not known and are read as an ordinary symbol instead.
    ERUS("MFSR gr96, MMU", "MMU", 0xC6600000);
    ERUS("MTSR LRU, gr96", "LRU, gr96", 0xCE000060);
}

void test_cache_register() {
    TEST("MFSR gr96, CIR", 0xC6601D00);
    TEST("MFSR gr96, CDR", 0xC6601E00);
    TEST("MTSR CDR, gr96", 0xCE001E60);
}

void test_no_cache_register() {
    ERUS("MFSR gr96, CIR", "CIR", 0xC6600000);
    ERUS("MTSR CDR, gr96", "CDR, gr96", 0xCE000060);
}

// Region mapping, shadow program counters, instruction breakpoints and the
// exception opcode, which the Am29050 alone implements.
void test_am29050_register() {
    TEST("MFSR gr96, RSN",  0xC6600F00);
    TEST("MFSR gr96, RMA0", 0xC6601000);
    TEST("MFSR gr96, RMC0", 0xC6601100);
    TEST("MFSR gr96, RMA1", 0xC6601200);
    TEST("MFSR gr96, RMC1", 0xC6601300);
    TEST("MFSR gr96, SPC0", 0xC6601400);
    TEST("MFSR gr96, SPC1", 0xC6601500);
    TEST("MFSR gr96, SPC2", 0xC6601600);
    TEST("MFSR gr96, IBA0", 0xC6601700);
    TEST("MFSR gr96, IBC0", 0xC6601800);
    TEST("MFSR gr96, IBA1", 0xC6601900);
    TEST("MFSR gr96, IBC1", 0xC6601A00);
    TEST("MFSR gr96, EXOP", 0xC660A400);
    TEST("MTSR RSN, gr96",  0xCE000F60);
    TEST("MTSR EXOP, gr96", 0xCE00A460);
}

void test_no_am29050_register() {
    ERUS("MFSR gr96, RSN",  "RSN",  0xC6600000);
    ERUS("MFSR gr96, EXOP", "EXOP", 0xC6600000);
}

void test_integer_multiply() {
    TEST("MULTIPLY gr96, gr97, gr98", 0xE0606162);
    TEST("MULTIPLU gr96, gr97, gr98", 0xE2606162);
    TEST("MULTM    gr96, gr97, gr98", 0xDE606162);
    TEST("MULTMU   gr96, gr97, gr98", 0xDF606162);
    TEST("DIVIDE   gr96, gr97, gr98", 0xE1606162);
    TEST("DIVIDU   gr96, gr97, gr98", 0xE3606162);
}

void test_no_integer_multiply() {
    ERUI("MULTIPLY gr96, gr97, gr98");
    ERUI("MULTM    gr96, gr97, gr98");
    ERUI("DIVIDE   gr96, gr97, gr98");
}

void test_float() {
    TEST("FADD    gr96, gr97, gr98", 0xF0606162);
    TEST("DADD    gr96, gr97, gr98", 0xF1606162);
    TEST("FSUB    gr96, gr97, gr98", 0xF2606162);
    TEST("DSUB    gr96, gr97, gr98", 0xF3606162);
    TEST("FMUL    gr96, gr97, gr98", 0xF4606162);
    TEST("DMUL    gr96, gr97, gr98", 0xF5606162);
    TEST("FDMUL   gr96, gr97, gr98", 0xF9606162);
    TEST("FDIV    gr96, gr97, gr98", 0xF6606162);
    TEST("DDIV    gr96, gr97, gr98", 0xF7606162);
    TEST("FEQ     gr96, gr97, gr98", 0xEA606162);
    TEST("DEQ     gr96, gr97, gr98", 0xEB606162);
    TEST("FGT     gr96, gr97, gr98", 0xEC606162);
    TEST("DGT     gr96, gr97, gr98", 0xED606162);
    TEST("FGE     gr96, gr97, gr98", 0xEE606162);
    TEST("DGE     gr96, gr97, gr98", 0xEF606162);
    TEST("CONVERT gr96, gr97, 0, 0, 0, 0", 0xE4606100);
    TEST("CONVERT gr96, gr97, 1, 4, 2, 2", 0xE46061CA);
    TEST("SQRT    gr96, gr97, 2", 0xE5606102);
    TEST("CLASS   gr96, gr97, 1", 0xE6606101);
    // A format code which does not fit its two-bit field.
    ERRT("SQRT    gr96, gr97, 4", OVERFLOW_RANGE, "4", 0xE5606100);
}

void test_no_float() {
    ERUI("FADD    gr96, gr97, gr98");
    ERUI("FGE     gr96, gr97, gr98");
    ERUI("CONVERT gr96, gr97, 0, 0, 0, 0");
    ERUI("SQRT    gr96, gr97, 0");
}

// Only the Am29050 has a coprocessor interface.
void test_coprocessor() {
    TEST("LOAD  1, 0x40, gr96, gr97", 0x16C06061);
    TEST("STORE 1, 0, gr96, gr97",    0x1E806061);
}

// Elsewhere the coprocessor-enable bit is reserved and only zero fits.
void test_no_coprocessor() {
    TEST("LOAD  0, 0x40, gr96, gr97", 0x16406061);
    ERRT("LOAD  1, 0x40, gr96, gr97", OVERFLOW_RANGE, "1, 0x40, gr96, gr97", 0x16406061);
    ERRT("STORE 1, 0, gr96, gr97",    OVERFLOW_RANGE, "1, 0, gr96, gr97",    0x1E006061);
}

void test_am29050() {
    TEST("ORN   gr96, gr97, gr98", 0xAA606162);
    TEST("ORN   gr96, gr97, 15",   0xAB60610F);
    TEST("FMAC  0, 0, gr97, gr98", 0xD8006162);
    TEST("FMAC  1, 3, gr97, gr98", 0xD8076162);
    // FUNC is four bits wide; the table runs to 15.
    TEST("FMAC  4, 0, gr97, gr98",  0xD8106162);
    TEST("FMAC  15, 3, gr97, gr98", 0xD83F6162);
    TEST("DMAC  8, 2, gr97, gr98",  0xD9226162);
    ERRT("FMAC  16, 0, gr97, gr98", OVERFLOW_RANGE, "16, 0, gr97, gr98", 0xD8006162);
    // The manual reserves RND 5-7, FD/FS 3, SQRT and CLASS format 0 and 3,
    // and MTACC/MFACC format 3.
    ERRT("CONVERT gr96, gr97, 0, 5, 0, 0", OPERAND_NOT_ALLOWED, "5, 0, 0", 0xE4606150);
    ERRT("CONVERT gr96, gr97, 0, 0, 3, 0", OPERAND_NOT_ALLOWED, "3, 0",    0xE460610C);
    ERRT("CONVERT gr96, gr97, 0, 0, 0, 3", OPERAND_NOT_ALLOWED, "3",       0xE4606103);
    ERRT("SQRT    gr96, gr97, 0",          OPERAND_NOT_ALLOWED, "0",       0xE5606100);
    ERRT("SQRT    gr96, gr97, 3",          OPERAND_NOT_ALLOWED, "3",       0xE5606103);
    ERRT("CLASS   gr96, gr97, 3",          OPERAND_NOT_ALLOWED, "3",       0xE6606103);
    ERRT("MTACC   gr97, 3, 0",             OPERAND_NOT_ALLOWED, "3, 0",    0xE801610C);
    TEST("DMAC  2, 1, gr97, gr98", 0xD9096162);
    TEST("FMSM  gr96, gr97, gr98", 0xDA606162);
    TEST("DMSM  gr96, gr97, gr98", 0xDB606162);
    TEST("MTACC gr97, 1, 0",       0xE8016104);
    TEST("MTACC gr97, 2, 3",       0xE801610B);
    TEST("MFACC gr96, 1, 0",       0xE9600104);
    TEST("MFACC gr96, 2, 3",       0xE960010B);
}

void test_no_am29050() {
    ERUI("ORN   gr96, gr97, gr98");
    ERUI("FMSM  gr96, gr97, gr98");
    ERUI("MTACC gr97, 1, 0");
}

// Arithmetic a chip traps to software may be turned on to assemble for a
// system which provides it, and off again.
void test_no_fpu_option() {
    asm29000.setOption("fpu", "on");
    TEST("FADD gr96, gr97, gr98", 0xF0606162);
    TEST("SQRT gr96, gr97, 1",    0xE5606101);
    asm29000.setOption("fpu", "off");
    ERUI("FADD gr96, gr97, gr98");
}

void test_no_intmult_option() {
    asm29000.setOption("intmult", "on");
    TEST("MULTIPLY gr96, gr97, gr98", 0xE0606162);
    TEST("DIVIDE   gr96, gr97, gr98", 0xE1606162);
    asm29000.setOption("intmult", "off");
    ERUI("MULTIPLY gr96, gr97, gr98");
}

// A chip which has the hardware cannot have it turned off.
void test_fpu_always_on() {
    EQUALS("fpu off", OPERAND_NOT_ALLOWED, asm29000.setOption("fpu", "off"));
    TEST("FADD gr96, gr97, gr98", 0xF0606162);
}

void test_intmult_always_on() {
    EQUALS("intmult off", OPERAND_NOT_ALLOWED, asm29000.setOption("intmult", "off"));
    TEST("MULTIPLY gr96, gr97, gr98", 0xE0606162);
}

void test_data_constant() {
    BTEST(".byte  -128, 255",   0x80, 0xFF);
    BTEST(".byte  'A', '\"'",   0x41, 0x22);
    BTEST(".byte  '9'-'0'",     0x09);
    BTEST(R"(.ascii "AB")",     0x41, 0x42);
    BTEST(".hword -128, 255",   0xFF, 0x80, 0x00, 0xFF);
    BTEST(".hword 0x1234",      0x12, 0x34);
    BTEST(".word  0x12345678",  0x12, 0x34, 0x56, 0x78);
    BTEST(".word  -1",          0xFF, 0xFF, 0xFF, 0xFF);
    BTEST(".space 4");
}

void test_errors() {
    ERUI("NOSUCH gr96, gr97, gr98");
    // A register number outside the file is read as a symbol, so it cannot
    // satisfy a register operand.
    ERRT("ADD  gr128, gr97, gr98", OPERAND_NOT_ALLOWED, "gr128, gr97, gr98");
    ERRT("ADD  lr128, gr97, gr98", OPERAND_NOT_ALLOWED, "lr128, gr97, gr98");
    // An 8-bit constant which does not fit.
    ERRT("ADD  gr96, gr97, 256",  OVERFLOW_RANGE, "256",  0x15606100);
    // A 16-bit constant which does not fit.
    ERRT("CONST gr96, 0x10000",   OVERFLOW_RANGE, "0x10000", 0x03006000);
    // Control fields which do not fit.
    ERRT("LOAD  2, 0, gr96, gr97",   OVERFLOW_RANGE, "2, 0, gr96, gr97",
            0x16006061);
    ERRT("LOAD  0, 128, gr96, gr97", OVERFLOW_RANGE, "128, gr96, gr97",
            0x16006061);
    ERRT("ASEQ  256, gr97, gr98",    OVERFLOW_RANGE, "256, gr97, gr98",
            0x70006162);
    // A branch target must be on a word boundary.
    AERRT(0x1000, "JMP 0x1002", OPERAND_NOT_ALIGNED, "0x1002", 0xA0000000);
    // Out of reach of both the relative and the absolute form.
    AERRT(0x1000, "JMP 0x400000", OPERAND_TOO_FAR, "0x400000", 0xA0000000);
    // The arithmetic directives take "on" or "off".
    ERRT("FPU     maybe", UNKNOWN_OPERAND, "maybe");
    ERRT("INTMULT maybe", UNKNOWN_OPERAND, "maybe");
}

void run_tests(const char *cpu) {
    assembler.setCpu(cpu);
    RUN_TEST(test_integer_arithmetic);
    RUN_TEST(test_multiply_divide_step);
    RUN_TEST(test_compare);
    RUN_TEST(test_assert);
    RUN_TEST(test_logical_shift);
    RUN_TEST(test_data_movement);
    RUN_TEST(test_special_register);
    RUN_TEST(test_constant);
    RUN_TEST(test_branch);
    RUN_TEST(test_miscellaneous);
    if (asm29000.cpuType() == AM29200) {
        RUN_TEST(test_no_mmu_register);
    } else {
        RUN_TEST(test_mmu_register);
    }
    if (asm29000.cpuType() == AM29200 || asm29000.cpuType() == AM29050) {
        RUN_TEST(test_no_cache_register);
    } else {
        RUN_TEST(test_cache_register);
    }
    if (asm29000.cpuType() == AM29050) {
        RUN_TEST(test_am29050_register);
    } else {
        RUN_TEST(test_no_am29050_register);
    }
    if (asm29000.hasIntMul()) {
        RUN_TEST(test_integer_multiply);
    } else {
        RUN_TEST(test_no_integer_multiply);
    }
    if (asm29000.hasFpu()) {
        RUN_TEST(test_float);
    } else {
        RUN_TEST(test_no_float);
    }
    if (asm29000.cpuType() == AM29050) {
        RUN_TEST(test_am29050);
        RUN_TEST(test_coprocessor);
    } else {
        RUN_TEST(test_no_am29050);
        RUN_TEST(test_no_coprocessor);
    }
    if (Config::cpuHasFpu(asm29000.cpuType())) {
        RUN_TEST(test_fpu_always_on);
    } else {
        RUN_TEST(test_no_fpu_option);
    }
    if (Config::cpuHasIntMul(asm29000.cpuType())) {
        RUN_TEST(test_intmult_always_on);
    } else {
        RUN_TEST(test_no_intmult_option);
    }
    RUN_TEST(test_data_constant);
    RUN_TEST(test_errors);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

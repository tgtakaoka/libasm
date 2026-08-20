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

#include "dis_am29000.h"
#include "test_dis_helper.h"

using namespace libasm;
using namespace libasm::am29000;
using namespace libasm::test;

DisAm29000 dis29000;
Disassembler &disassembler(dis29000);

void set_up() {
    disassembler.reset();
}

void tear_down() {
    symtab.reset();
}

void test_cpu() {
    EQUALS("cpu AM29200", true, disassembler.setCpu("AM29200"));
    EQUALS_P("cpu AM29200", "AM29200", disassembler.config().cpu_P());

    EQUALS("cpu AM29245", true, disassembler.setCpu("AM29245"));
    EQUALS_P("cpu AM29245", "AM29245", disassembler.config().cpu_P());

    EQUALS("cpu AM29030", true, disassembler.setCpu("AM29030"));
    EQUALS_P("cpu AM29030", "AM29030", disassembler.config().cpu_P());

    EQUALS("cpu AM29240", true, disassembler.setCpu("AM29240"));
    EQUALS_P("cpu AM29240", "AM29240", disassembler.config().cpu_P());

    EQUALS("cpu AM29040", true, disassembler.setCpu("AM29040"));
    EQUALS_P("cpu AM29040", "AM29040", disassembler.config().cpu_P());

    EQUALS("cpu AM29050", true, disassembler.setCpu("AM29050"));
    EQUALS_P("cpu AM29050", "AM29050", disassembler.config().cpu_P());
}

// The M bit of the operation code selects a register or an 8-bit constant.
void test_integer_arithmetic() {
    TEST("ADD",    "GR96, GR97, GR98", 0x14606162);
    TEST("ADD",    "GR96, GR97, 15", 0x1560610F);
    TEST("ADD",    "LR0, LR1, LR2",    0x14808182);
    TEST("ADDS",   "GR96, GR97, GR98", 0x10606162);
    TEST("ADDU",   "GR96, GR97, GR98", 0x12606162);
    TEST("ADDC",   "GR96, GR97, GR98", 0x1C606162);
    TEST("ADDCS",  "GR96, GR97, GR98", 0x18606162);
    TEST("ADDCU",  "GR96, GR97, GR98", 0x1A606162);
    TEST("SUB",    "GR96, GR97, GR98", 0x24606162);
    TEST("SUBS",   "GR96, GR97, GR98", 0x20606162);
    TEST("SUBU",   "GR96, GR97, GR98", 0x22606162);
    TEST("SUBC",   "GR96, GR97, GR98", 0x2C606162);
    TEST("SUBCS",  "GR96, GR97, GR98", 0x28606162);
    TEST("SUBCU",  "GR96, GR97, GR98", 0x2A606162);
    TEST("SUBR",   "GR96, GR97, GR98", 0x34606162);
    TEST("SUBRS",  "GR96, GR97, GR98", 0x30606162);
    TEST("SUBRU",  "GR96, GR97, GR98", 0x32606162);
    TEST("SUBRC",  "GR96, GR97, GR98", 0x3C606162);
    TEST("SUBRCS", "GR96, GR97, GR98", 0x38606162);
    TEST("SUBRCU", "GR96, GR97, GR98", 0x3A606162);
}

// Multiply and divide steps run on every family member.
void test_multiply_divide_step() {
    TEST("MUL",    "GR96, GR97, GR98", 0x64606162);
    TEST("MULL",   "GR96, GR97, GR98", 0x66606162);
    TEST("MULU",   "GR96, GR97, GR98", 0x74606162);
    TEST("DIV0",   "GR96, GR98",       0x68600062);
    TEST("DIV0",   "GR96, 15",       0x6960000F);
    TEST("DIV",    "GR96, GR97, GR98", 0x6A606162);
    TEST("DIVL",   "GR96, GR97, GR98", 0x6C606162);
    TEST("DIVREM", "GR96, GR97, GR98", 0x6E606162);
}

void test_compare() {
    TEST("CPEQ",   "GR96, GR97, GR98", 0x60606162);
    TEST("CPNEQ",  "GR96, GR97, GR98", 0x62606162);
    TEST("CPLT",   "GR96, GR97, GR98", 0x40606162);
    TEST("CPLTU",  "GR96, GR97, GR98", 0x42606162);
    TEST("CPLE",   "GR96, GR97, GR98", 0x44606162);
    TEST("CPLEU",  "GR96, GR97, GR98", 0x46606162);
    TEST("CPGT",   "GR96, GR97, GR98", 0x48606162);
    TEST("CPGTU",  "GR96, GR97, GR98", 0x4A606162);
    TEST("CPGE",   "GR96, GR97, GR98", 0x4C606162);
    TEST("CPGEU",  "GR96, GR97, GR98", 0x4E606162);
    TEST("CPBYTE", "GR96, GR97, GR98", 0x2E606162);
}

// An assert names a trap vector number rather than a destination register.
void test_assert() {
    TEST("ASEQ",   "0x40, GR97, GR98", 0x70406162);
    TEST("ASNEQ",  "0x41, GR97, GR98", 0x72416162);
    TEST("ASLT",   "0x42, GR97, GR98", 0x50426162);
    TEST("ASLTU",  "0x43, GR97, GR98", 0x52436162);
    TEST("ASLE",   "0x44, GR97, GR98", 0x54446162);
    TEST("ASLEU",  "0x45, GR97, GR98", 0x56456162);
    TEST("ASGT",   "0x46, GR97, GR98", 0x58466162);
    TEST("ASGTU",  "0x47, GR97, GR98", 0x5A476162);
    TEST("ASGE",   "0x48, GR97, GR98", 0x5C486162);
    TEST("ASGEU",  "0xFF, GR97, 15", 0x5FFF610F);
}

void test_logical_shift() {
    TEST("AND",     "GR96, GR97, GR98", 0x90606162);
    TEST("ANDN",    "GR96, GR97, GR98", 0x9C606162);
    TEST("NAND",    "GR96, GR97, GR98", 0x9A606162);
    TEST("OR",      "GR96, GR97, GR98", 0x92606162);
    TEST("NOR",     "GR96, GR97, GR98", 0x98606162);
    TEST("XOR",     "GR96, GR97, GR98", 0x94606162);
    TEST("XNOR",    "GR96, GR97, GR98", 0x96606162);
    TEST("SLL",     "GR96, GR97, GR98", 0x80606162);
    TEST("SLL",     "GR96, GR97, 31", 0x8160611F);
    TEST("SRL",     "GR96, GR97, GR98", 0x82606162);
    TEST("SRA",     "GR96, GR97, GR98", 0x86606162);
    TEST("EXTRACT", "GR96, GR97, GR98", 0x7A606162);
}

// A load or store names the coprocessor enable bit and the control field
// before its registers.
void test_data_movement() {
    TEST("LOAD",    "0, 0, GR96, GR97", 0x16006061);
    TEST("LOAD",    "0, 0x40, GR96, 15", 0x1740600F);
    TEST("LOADL",   "0, 0, GR96, GR97", 0x06006061);
    TEST("LOADSET", "0, 0, GR96, GR97", 0x26006061);
    TEST("LOADM",   "0, 0, GR96, GR97", 0x36006061);
    TEST("STORE",   "0, 0, GR96, GR97", 0x1E006061);
    TEST("STOREL",  "0, 0, GR96, GR97", 0x0E006061);
    TEST("STOREM",  "0, 0, GR96, GR97", 0x3E006061);
    TEST("EXBYTE",  "GR96, GR97, GR98",    0x0A606162);
    TEST("EXHW",    "GR96, GR97, GR98",    0x7C606162);
    TEST("EXHWS",   "GR96, GR97",          0x7E606100);
    TEST("INBYTE",  "GR96, GR97, GR98",    0x0C606162);
    TEST("INHW",    "GR96, GR97, GR98",    0x78606162);
    TEST("MFTLB",   "GR96, GR97",          0xB6606100);
    TEST("MTTLB",   "GR96, GR97",          0xBE006061);
}

void test_special_register() {
    TEST("MFSR",   "GR96, VAB",    0xC6600000);
    TEST("MFSR",   "GR96, CPS",    0xC6600200);
    TEST("MFSR",   "GR96, Q",      0xC6608300);
    TEST("MFSR",   "GR96, CR",     0xC6608700);
    TEST("MFSR",   "GR96, FPS",    0xC660A200);
    TEST("MTSR",   "VAB, GR96",    0xCE000060);
    TEST("MTSR",   "Q, GR96",      0xCE008360);
    TEST("MTSR",   "CR, GR96",     0xCE008760);
    TEST("MTSRIM", "Q, 0x1234",    0x04128334);
    TEST("MTSRIM", "CR, 0",   0x04008700);
    // A register number no family member implements is rejected, the same as a
    // reserved field which is not zero.
    UNKN(0xC6607F00);  // MFSR GR96, 127
    UNKN(0xCE006460);  // MTSR 100, GR96
    UNKN(0x0400C800);  // MTSRIM 200, 0
}

// GNU as reads the 16-bit word address of an absolute branch as signed and
// rejects a target over 0x1FFFC, where the manual zero-extends it, so the
// target is named and left to the linker.  A relative branch keeps its own
// form.  The name stays in lower case whichever case the listing is in.
void test_branch_extern_symbol() {
    disassembler.setOption("extern-symbol", "on");
    disassembler.setOption("relative", "on");
    disassembler.setOption("origin-char", ".");
    ATEST(0x1000, "JMP",  "A_00000040", 0xA1000010);
    ATEST(0x1000, "JMP",  "A_0003fffc", 0xA1ff00ff);
    ATEST(0x1000, "CALL", "GR96, A_00004a68", 0xA912609a);
    ATEST(0x1000, "JMP",  ".+8",        0xA0000002);
    ATEST(0x1000, "JMP",  ".-8",        0xA0ff00fe);
    // GNU as keeps the high half of a 32-bit value for CONSTH.
    ATEST(0x1000, "CONSTH", "GR96, 0x56780000", 0x02566078);
    disassembler.setOption("extern-symbol", "off");
    disassembler.setOption("relative", "off");
    ATEST(0x1000, "JMP",  "0x00000040", 0xA1000010);
    ATEST(0x1000, "CONSTH", "GR96, 0x5678", 0x02566078);
}

// An instruction is one 32-bit word, so anything shorter is truncated rather
// than illegal.  The table search starts by clearing the error a short read
// raises, so the disassembler has to report it before searching.
void test_short_input() {
    static const uint8_t bytes[] = {0x14, 0x60, 0x61, 0x62};  // ADD GR96, GR97, GR98
    for (uint8_t len = 1; len < sizeof(bytes); len++) {
        const ArrayMemory memory(0x1000, bytes, len);
        auto it = memory.iterator();
        Insn insn(0x1000);
        char operands[40];
        EQUALS("truncated instruction", NO_MEMORY,
                disassembler.decode(it, insn, operands, sizeof(operands)));
    }
    const ArrayMemory whole(0x1000, bytes, sizeof(bytes));
    auto it = whole.iterator();
    Insn insn(0x1000);
    char operands[40];
    EQUALS("whole instruction", OK, disassembler.decode(it, insn, operands, sizeof(operands)));
}

void test_constant() {
    TEST("CONST",  "GR96, 0x1234", 0x03126034);
    TEST("CONSTH", "GR96, 0x5678", 0x02566078);
    TEST("CONSTN", "GR96, 0x1234", 0x01126034);
}

// A branch target is a word address; the A bit of the operation code selects
// an absolute target from one relative to the Program Counter.
void test_branch() {
    ATEST(0x1000, "JMP",     "0x00001000", 0xA0000000);
    ATEST(0x1000, "JMP",     "0x00000FF8", 0xA0FF00FE);
    ATEST(0x1000, "JMP",     "0x00001008", 0xA0000002);
    ATEST(0x1000, "JMP",     "0x00004A68", 0xA112009A);
    ATEST(0x1000, "JMPF",    "GR97, 0x00001000", 0xA4006100);
    ATEST(0x1000, "JMPT",    "GR97, 0x00001000", 0xAC006100);
    ATEST(0x1000, "JMPFDEC", "GR97, 0x00001000", 0xB4006100);
    ATEST(0x1000, "CALL",    "GR96, 0x00001000", 0xA8006000);
    TEST("JMPI",  "GR98",       0xC0000062);
    TEST("JMPFI", "GR97, GR98", 0xC4006162);
    TEST("JMPTI", "GR97, GR98", 0xCC006162);
    TEST("CALLI", "GR96, GR98", 0xC8006062);
}

void test_miscellaneous() {
    TEST("CLZ",     "GR96, GR98",       0x08600062);
    TEST("CLZ",     "GR96, 15",       0x0960000F);
    TEST("SETIP",   "GR96, GR97, GR98", 0x9E606162);
    TEST("EMULATE", "0x40, GR97, GR98", 0xD7406162);
    TEST("INV",     "0",                0x9F000000);
    TEST("INV",     "1",                0x9F010000);
    TEST("INV",     "2",                0x9F020000);
    TEST("IRET",    "",                 0x88000000);
    TEST("IRETINV", "0",                0x8C000000);
    TEST("HALT",    "",                 0x89000000);
}

// The memory management unit is absent from the Am29200 only.
void test_mmu_register() {
    TEST("MFSR", "GR96, MMU", 0xC6600D00);
    TEST("MFSR", "GR96, LRU", 0xC6600E00);
    TEST("MTSR", "MMU, GR96", 0xCE000D60);
}

// The cache interface registers.  The Am29200 has no cache, and the Am29050
// does not reach its cache through a special-purpose register.
void test_cache_register() {
    TEST("MFSR", "GR96, CIR", 0xC6601D00);
    TEST("MFSR", "GR96, CDR", 0xC6601E00);
    TEST("MTSR", "CDR, GR96", 0xCE001E60);
}

void test_no_cache_register() {
    UNKN(0xC6601D00);  // MFSR GR96, CIR
    UNKN(0xC6601E00);  // MFSR GR96, CDR
}

// Region mapping, shadow program counters, instruction breakpoints and the
// exception opcode, which the Am29050 alone implements.
void test_am29050_register() {
    TEST("MFSR", "GR96, RSN",  0xC6600F00);
    TEST("MFSR", "GR96, RMA0", 0xC6601000);
    TEST("MFSR", "GR96, RMC0", 0xC6601100);
    TEST("MFSR", "GR96, RMA1", 0xC6601200);
    TEST("MFSR", "GR96, RMC1", 0xC6601300);
    TEST("MFSR", "GR96, SPC0", 0xC6601400);
    TEST("MFSR", "GR96, SPC1", 0xC6601500);
    TEST("MFSR", "GR96, SPC2", 0xC6601600);
    TEST("MFSR", "GR96, IBA0", 0xC6601700);
    TEST("MFSR", "GR96, IBC0", 0xC6601800);
    TEST("MFSR", "GR96, IBA1", 0xC6601900);
    TEST("MFSR", "GR96, IBC1", 0xC6601A00);
    TEST("MFSR", "GR96, EXOP", 0xC660A400);
    TEST("MTSR", "RSN, GR96",  0xCE000F60);
    TEST("MTSR", "EXOP, GR96", 0xCE00A460);
}

void test_no_am29050_register() {
    UNKN(0xC6600F00);  // MFSR GR96, RSN
    UNKN(0xC6601A00);  // MFSR GR96, IBC1
    UNKN(0xC660A400);  // MFSR GR96, EXOP
}

void test_no_mmu_register() {
    // The Am29200 leaves register numbers 13 and 14 unimplemented.
    UNKN(0xC6600D00);  // MFSR GR96, MMU
    UNKN(0xC6600E00);  // MFSR GR96, LRU
}

void test_integer_multiply() {
    TEST("MULTIPLY", "GR96, GR97, GR98", 0xE0606162);
    TEST("MULTIPLU", "GR96, GR97, GR98", 0xE2606162);
    TEST("MULTM",    "GR96, GR97, GR98", 0xDE606162);
    TEST("MULTMU",   "GR96, GR97, GR98", 0xDF606162);
    TEST("DIVIDE",   "GR96, GR97, GR98", 0xE1606162);
    TEST("DIVIDU",   "GR96, GR97, GR98", 0xE3606162);
}

// A chip which traps the integer multiply and divide instructions decodes
// none of them until the option is turned on.
void test_no_integer_multiply() {
    UNKN(0xE0606162);
    UNKN(0xE2606162);
    UNKN(0xDE606162);
    UNKN(0xDF606162);
    UNKN(0xE1606162);
    UNKN(0xE3606162);

    dis29000.setOption("intmult", "on");
    TEST("MULTIPLY", "GR96, GR97, GR98", 0xE0606162);
    TEST("MULTIPLU", "GR96, GR97, GR98", 0xE2606162);
    TEST("MULTM",    "GR96, GR97, GR98", 0xDE606162);
    TEST("MULTMU",   "GR96, GR97, GR98", 0xDF606162);
    TEST("DIVIDE",   "GR96, GR97, GR98", 0xE1606162);
    TEST("DIVIDU",   "GR96, GR97, GR98", 0xE3606162);
    dis29000.setOption("intmult", "off");
    UNKN(0xE0606162);
}

// The Am29240, Am29040 and Am29050 have the multiplier, so it cannot be
// turned off there.
void test_integer_multiply_always_on() {
    EQUALS("intmult off", OPERAND_NOT_ALLOWED, dis29000.setOption("intmult", "off"));
    TEST("MULTIPLY", "GR96, GR97, GR98", 0xE0606162);
}

void test_float() {
    TEST("FADD",    "GR96, GR97, GR98", 0xF0606162);
    TEST("DADD",    "GR96, GR97, GR98", 0xF1606162);
    TEST("FSUB",    "GR96, GR97, GR98", 0xF2606162);
    TEST("DSUB",    "GR96, GR97, GR98", 0xF3606162);
    TEST("FMUL",    "GR96, GR97, GR98", 0xF4606162);
    TEST("DMUL",    "GR96, GR97, GR98", 0xF5606162);
    TEST("FDMUL",   "GR96, GR97, GR98", 0xF9606162);
    TEST("FDIV",    "GR96, GR97, GR98", 0xF6606162);
    TEST("DDIV",    "GR96, GR97, GR98", 0xF7606162);
    TEST("FEQ",     "GR96, GR97, GR98", 0xEA606162);
    TEST("DEQ",     "GR96, GR97, GR98", 0xEB606162);
    TEST("FGT",     "GR96, GR97, GR98", 0xEC606162);
    TEST("DGT",     "GR96, GR97, GR98", 0xED606162);
    TEST("FGE",     "GR96, GR97, GR98", 0xEE606162);
    TEST("DGE",     "GR96, GR97, GR98", 0xEF606162);
    TEST("CONVERT", "GR96, GR97, 0, 0, 0, 0", 0xE4606100);
    TEST("CONVERT", "GR96, GR97, 1, 4, 2, 2", 0xE46061CA);
    TEST("SQRT",    "GR96, GR97, 2",    0xE5606102);
    TEST("CLASS",   "GR96, GR97, 1",    0xE6606101);
}

// A chip which traps the floating-point instructions decodes none of them
// until the option is turned on.
void test_no_float() {
    UNKN(0xF0606162);
    UNKN(0xEE606162);
    UNKN(0xE4606100);
    UNKN(0xE5606100);
    UNKN(0xE6606100);

    dis29000.setOption("fpu", "on");
    TEST("FADD",    "GR96, GR97, GR98", 0xF0606162);
    TEST("FGE",     "GR96, GR97, GR98", 0xEE606162);
    TEST("CONVERT", "GR96, GR97, 0, 0, 0, 0", 0xE4606100);
    TEST("SQRT",    "GR96, GR97, 1",    0xE5606101);
    TEST("CLASS",   "GR96, GR97, 2",    0xE6606102);
    dis29000.setOption("fpu", "off");
    UNKN(0xF0606162);
}

// The Am29050 has the floating-point unit, so it cannot be turned off there.
void test_float_always_on() {
    EQUALS("fpu off", OPERAND_NOT_ALLOWED, dis29000.setOption("fpu", "off"));
    TEST("FADD", "GR96, GR97, GR98", 0xF0606162);
}

// Instructions found only on the Am29050.
// Only the Am29050 has a coprocessor interface.
void test_coprocessor() {
    TEST("LOAD",  "1, 0x40, GR96, GR97", 0x16C06061);
    TEST("STORE", "1, 0, GR96, GR97",    0x1E806061);
}

// Elsewhere the coprocessor-enable bit is reserved and must read as zero.
void test_no_coprocessor() {
    UNKN(0x16C06061);
    UNKN(0x1E806061);
}

void test_am29050() {
    TEST("ORN",   "GR96, GR97, GR98", 0xAA606162);
    TEST("ORN",   "GR96, GR97, 15", 0xAB60610F);
    TEST("FMAC",  "0, 0, GR97, GR98", 0xD8006162);
    TEST("FMAC",  "1, 3, GR97, GR98", 0xD8076162);
    // FUNC is four bits wide; the table runs to 15.
    TEST("FMAC",  "4, 0, GR97, GR98",  0xD8106162);
    TEST("FMAC",  "15, 3, GR97, GR98", 0xD83F6162);
    TEST("DMAC",  "8, 2, GR97, GR98",  0xD9226162);
    // A reserved control value leaves the encoding undefined.
    UNKN(0xE4606150);  // CONVERT with RND 5
    UNKN(0xE460610C);  // CONVERT with FD 3
    UNKN(0xE4606103);  // CONVERT with FS 3
    UNKN(0xE5606100);  // SQRT with format 0
    UNKN(0xE6606103);  // CLASS with format 3
    UNKN(0xE801610C);  // MTACC with format 3
    TEST("DMAC",  "2, 1, GR97, GR98", 0xD9096162);
    TEST("FMSM",  "GR96, GR97, GR98", 0xDA606162);
    TEST("DMSM",  "GR96, GR97, GR98", 0xDB606162);
    TEST("MTACC", "GR97, 1, 0",       0xE8016104);
    TEST("MTACC", "GR97, 2, 3",       0xE801610B);
    TEST("MFACC", "GR96, 1, 0",       0xE9600104);
    TEST("MFACC", "GR96, 2, 3",       0xE960010B);
}

void test_no_am29050() {
    UNKN(0xAA606162);
    UNKN(0xD8006162);
    UNKN(0xD9006162);
    UNKN(0xDA606162);
    UNKN(0xDB606162);
    UNKN(0xE8016104);
    UNKN(0xE9600104);
}

// The operation codes below are absent from the instruction index of the
// Am29200, Am29240 series and Am29030 manuals, or are listed there as reserved
// for instruction emulation.
void test_illegal() {
    UNKN(0x00000000);
    UNKN(0x05000000);
    UNKN(0x76000000);
    UNKN(0x77000000);
    UNKN(0x7F000000);
    UNKN(0x84000000);
    UNKN(0x8A000000);
    UNKN(0x8D000000);
    UNKN(0xA2000000);
    UNKN(0xA6000000);
    UNKN(0xAE000000);
    UNKN(0xB7000000);
    UNKN(0xBF000000);
    UNKN(0xC1000000);
    UNKN(0xC5000000);
    UNKN(0xC7000000);
    UNKN(0xC9000000);
    UNKN(0xCD000000);
    UNKN(0xCF000000);
    UNKN(0xD6000000);
    UNKN(0xDC000000);
    UNKN(0xDD000000);
    UNKN(0xE7000000);
    UNKN(0xF8000000);
    UNKN(0xFA000000);
    UNKN(0xFF000000);
}

// A field an instruction does not use is reserved and must read as zero.
void test_reserved_field() {
    UNKN(0x68606062);  // DIV0 with a register number in the reserved RA field
    UNKN(0x7E606161);  // EXHWS with a register number in the reserved RB field
    UNKN(0x88000001);  // IRET with a non-zero reserved field
    UNKN(0x89000001);  // HALT with a non-zero reserved field
    UNKN(0xC0006200);  // JMPI with a register number in the reserved RA field
    UNKN(0xB6606101);  // MFTLB with a non-zero reserved RB field
}

void run_tests(const char *cpu) {
    disassembler.setCpu(cpu);
    RUN_TEST(test_integer_arithmetic);
    RUN_TEST(test_multiply_divide_step);
    RUN_TEST(test_compare);
    RUN_TEST(test_assert);
    RUN_TEST(test_logical_shift);
    RUN_TEST(test_data_movement);
    RUN_TEST(test_special_register);
    RUN_TEST(test_branch_extern_symbol);
    RUN_TEST(test_short_input);
    RUN_TEST(test_constant);
    RUN_TEST(test_branch);
    RUN_TEST(test_miscellaneous);
    if (dis29000.cpuType() == AM29200) {
        RUN_TEST(test_no_mmu_register);
    } else {
        RUN_TEST(test_mmu_register);
    }
    if (dis29000.cpuType() == AM29200 || dis29000.cpuType() == AM29050) {
        RUN_TEST(test_no_cache_register);
    } else {
        RUN_TEST(test_cache_register);
    }
    if (dis29000.cpuType() == AM29050) {
        RUN_TEST(test_am29050_register);
    } else {
        RUN_TEST(test_no_am29050_register);
    }
    if (dis29000.hasIntMul()) {
        RUN_TEST(test_integer_multiply);
        RUN_TEST(test_integer_multiply_always_on);
    } else {
        RUN_TEST(test_no_integer_multiply);
    }
    if (dis29000.hasFpu()) {
        RUN_TEST(test_float);
        RUN_TEST(test_float_always_on);
    } else {
        RUN_TEST(test_no_float);
    }
    if (dis29000.cpuType() == AM29050) {
        RUN_TEST(test_am29050);
        RUN_TEST(test_coprocessor);
    } else {
        RUN_TEST(test_no_am29050);
        RUN_TEST(test_no_coprocessor);
    }
    RUN_TEST(test_illegal);
    RUN_TEST(test_reserved_field);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

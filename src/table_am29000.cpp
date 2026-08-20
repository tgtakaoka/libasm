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

#include "table_am29000.h"

#include "entry_am29000.h"
#include "entry_table.h"
#include "text_am29000.h"

using namespace libasm::text::am29000;

namespace libasm {
namespace am29000 {

#define E0(_opc, _name) {_opc, Entry::Flags::create(), _name}
#define E1(_opc, _name, _o1) {_opc, Entry::Flags::create(_o1), _name}
#define E2(_opc, _name, _o1, _o2) {_opc, Entry::Flags::create(_o1, _o2), _name}
#define E3(_opc, _name, _o1, _o2, _o3) {_opc, Entry::Flags::create(_o1, _o2, _o3), _name}
#define E4(_opc, _name, _o1, _o2, _o3, _o4) \
    {_opc, Entry::Flags::create(_o1, _o2, _o3, _o4), _name}
#define E6(_opc, _name, _o1, _o2, _o3, _o4, _o5, _o6) \
    {_opc, Entry::Flags::create(_o1, _o2, _o3, _o4, _o5, _o6), _name}
#define X3(_opc, _name, _o1, _o2, _o3, _ext) \
    {_opc, Entry::Flags::create(_o1, _o2, _o3, M_NONE, M_NONE, M_NONE, _ext), _name}
#define X4(_opc, _name, _o1, _o2, _o3, _o4, _ext) \
    {_opc, Entry::Flags::create(_o1, _o2, _o3, _o4, M_NONE, M_NONE, _ext), _name}
#define X6(_opc, _name, _o1, _o2, _o3, _o4, _o5, _o6, _ext) \
    {_opc, Entry::Flags::create(_o1, _o2, _o3, _o4, _o5, _o6, _ext), _name}

// clang-format off
// An entry holds the whole 32-bit instruction with its operand fields zeroed.
// Where the low bit of the operation code is the M or the A bit, the table
// carries the M=0 / A=0 form and the operand encoder raises the bit.
constexpr Entry TABLE_AM29000[] PROGMEM = {
    // Integer arithmetic
    E3(0x14000000, TEXT_ADD,     M_RC, M_RA, M_RBI),
    E3(0x10000000, TEXT_ADDS,    M_RC, M_RA, M_RBI),
    E3(0x12000000, TEXT_ADDU,    M_RC, M_RA, M_RBI),
    E3(0x1C000000, TEXT_ADDC,    M_RC, M_RA, M_RBI),
    E3(0x18000000, TEXT_ADDCS,   M_RC, M_RA, M_RBI),
    E3(0x1A000000, TEXT_ADDCU,   M_RC, M_RA, M_RBI),
    E3(0x24000000, TEXT_SUB,     M_RC, M_RA, M_RBI),
    E3(0x20000000, TEXT_SUBS,    M_RC, M_RA, M_RBI),
    E3(0x22000000, TEXT_SUBU,    M_RC, M_RA, M_RBI),
    E3(0x2C000000, TEXT_SUBC,    M_RC, M_RA, M_RBI),
    E3(0x28000000, TEXT_SUBCS,   M_RC, M_RA, M_RBI),
    E3(0x2A000000, TEXT_SUBCU,   M_RC, M_RA, M_RBI),
    E3(0x34000000, TEXT_SUBR,    M_RC, M_RA, M_RBI),
    E3(0x30000000, TEXT_SUBRS,   M_RC, M_RA, M_RBI),
    E3(0x32000000, TEXT_SUBRU,   M_RC, M_RA, M_RBI),
    E3(0x3C000000, TEXT_SUBRC,   M_RC, M_RA, M_RBI),
    E3(0x38000000, TEXT_SUBRCS,  M_RC, M_RA, M_RBI),
    E3(0x3A000000, TEXT_SUBRCU,  M_RC, M_RA, M_RBI),
    // Multiply and divide steps, executed by every family member
    E3(0x64000000, TEXT_MUL,     M_RC, M_RA, M_RBI),
    E3(0x66000000, TEXT_MULL,    M_RC, M_RA, M_RBI),
    E3(0x74000000, TEXT_MULU,    M_RC, M_RA, M_RBI),
    E2(0x68000000, TEXT_DIV0,    M_RC, M_RBI),
    E3(0x6A000000, TEXT_DIV,     M_RC, M_RA, M_RBI),
    E3(0x6C000000, TEXT_DIVL,    M_RC, M_RA, M_RBI),
    E3(0x6E000000, TEXT_DIVREM,  M_RC, M_RA, M_RBI),
    // Integer multiply and divide.  The Am29240 series manual groups these six
    // as the instructions a chip without the multiplier traps to software.
    X3(0xE0000000, TEXT_MULTIPLY, M_RC, M_RA, M_RB, EXT_INTMUL),
    X3(0xE2000000, TEXT_MULTIPLU, M_RC, M_RA, M_RB, EXT_INTMUL),
    X3(0xDE000000, TEXT_MULTM,    M_RC, M_RA, M_RB, EXT_INTMUL),
    X3(0xDF000000, TEXT_MULTMU,   M_RC, M_RA, M_RB, EXT_INTMUL),
    X3(0xE1000000, TEXT_DIVIDE,   M_RC, M_RA, M_RB, EXT_INTMUL),
    X3(0xE3000000, TEXT_DIVIDU,   M_RC, M_RA, M_RB, EXT_INTMUL),
    // Compare
    E3(0x60000000, TEXT_CPEQ,    M_RC, M_RA, M_RBI),
    E3(0x62000000, TEXT_CPNEQ,   M_RC, M_RA, M_RBI),
    E3(0x40000000, TEXT_CPLT,    M_RC, M_RA, M_RBI),
    E3(0x42000000, TEXT_CPLTU,   M_RC, M_RA, M_RBI),
    E3(0x44000000, TEXT_CPLE,    M_RC, M_RA, M_RBI),
    E3(0x46000000, TEXT_CPLEU,   M_RC, M_RA, M_RBI),
    E3(0x48000000, TEXT_CPGT,    M_RC, M_RA, M_RBI),
    E3(0x4A000000, TEXT_CPGTU,   M_RC, M_RA, M_RBI),
    E3(0x4C000000, TEXT_CPGE,    M_RC, M_RA, M_RBI),
    E3(0x4E000000, TEXT_CPGEU,   M_RC, M_RA, M_RBI),
    E3(0x2E000000, TEXT_CPBYTE,  M_RC, M_RA, M_RBI),
    // Assert
    E3(0x70000000, TEXT_ASEQ,    M_VN, M_RA, M_RBI),
    E3(0x72000000, TEXT_ASNEQ,   M_VN, M_RA, M_RBI),
    E3(0x50000000, TEXT_ASLT,    M_VN, M_RA, M_RBI),
    E3(0x52000000, TEXT_ASLTU,   M_VN, M_RA, M_RBI),
    E3(0x54000000, TEXT_ASLE,    M_VN, M_RA, M_RBI),
    E3(0x56000000, TEXT_ASLEU,   M_VN, M_RA, M_RBI),
    E3(0x58000000, TEXT_ASGT,    M_VN, M_RA, M_RBI),
    E3(0x5A000000, TEXT_ASGTU,   M_VN, M_RA, M_RBI),
    E3(0x5C000000, TEXT_ASGE,    M_VN, M_RA, M_RBI),
    E3(0x5E000000, TEXT_ASGEU,   M_VN, M_RA, M_RBI),
    // Logical
    E3(0x90000000, TEXT_AND,     M_RC, M_RA, M_RBI),
    E3(0x9C000000, TEXT_ANDN,    M_RC, M_RA, M_RBI),
    E3(0x9A000000, TEXT_NAND,    M_RC, M_RA, M_RBI),
    E3(0x92000000, TEXT_OR,      M_RC, M_RA, M_RBI),
    E3(0x98000000, TEXT_NOR,     M_RC, M_RA, M_RBI),
    E3(0x94000000, TEXT_XOR,     M_RC, M_RA, M_RBI),
    E3(0x96000000, TEXT_XNOR,    M_RC, M_RA, M_RBI),
    // Shift
    E3(0x80000000, TEXT_SLL,     M_RC, M_RA, M_RBI),
    E3(0x82000000, TEXT_SRL,     M_RC, M_RA, M_RBI),
    E3(0x86000000, TEXT_SRA,     M_RC, M_RA, M_RBI),
    E3(0x7A000000, TEXT_EXTRACT, M_RC, M_RA, M_RBI),
    // Data movement
    E4(0x16000000, TEXT_LOAD,    M_CE, M_CNTL, M_RA, M_RBI),
    E4(0x06000000, TEXT_LOADL,   M_CE, M_CNTL, M_RA, M_RBI),
    E4(0x26000000, TEXT_LOADSET, M_CE, M_CNTL, M_RA, M_RBI),
    E4(0x36000000, TEXT_LOADM,   M_CE, M_CNTL, M_RA, M_RBI),
    E4(0x1E000000, TEXT_STORE,   M_CE, M_CNTL, M_RA, M_RBI),
    E4(0x0E000000, TEXT_STOREL,  M_CE, M_CNTL, M_RA, M_RBI),
    E4(0x3E000000, TEXT_STOREM,  M_CE, M_CNTL, M_RA, M_RBI),
    E3(0x0A000000, TEXT_EXBYTE,  M_RC, M_RA, M_RBI),
    E3(0x7C000000, TEXT_EXHW,    M_RC, M_RA, M_RBI),
    E2(0x7E000000, TEXT_EXHWS,   M_RC, M_RA),
    E3(0x0C000000, TEXT_INBYTE,  M_RC, M_RA, M_RBI),
    E3(0x78000000, TEXT_INHW,    M_RC, M_RA, M_RBI),
    E2(0xC6000000, TEXT_MFSR,    M_RC, M_SA),
    E2(0xCE000000, TEXT_MTSR,    M_SA, M_RB),
    E2(0x04000000, TEXT_MTSRIM,  M_SA, M_IM16),
    E2(0xB6000000, TEXT_MFTLB,   M_RC, M_RA),
    E2(0xBE000000, TEXT_MTTLB,   M_RA, M_RB),
    // Constant
    E2(0x03000000, TEXT_CONST,   M_RA, M_IM16),
    E2(0x02000000, TEXT_CONSTH,  M_RA, M_IMH),
    E2(0x01000000, TEXT_CONSTN,  M_RA, M_IM16),
    // Floating point
    X3(0xF0000000, TEXT_FADD,    M_RC, M_RA, M_RB, EXT_FPU),
    X3(0xF1000000, TEXT_DADD,    M_RC, M_RA, M_RB, EXT_FPU),
    X3(0xF2000000, TEXT_FSUB,    M_RC, M_RA, M_RB, EXT_FPU),
    X3(0xF3000000, TEXT_DSUB,    M_RC, M_RA, M_RB, EXT_FPU),
    X3(0xF4000000, TEXT_FMUL,    M_RC, M_RA, M_RB, EXT_FPU),
    X3(0xF5000000, TEXT_DMUL,    M_RC, M_RA, M_RB, EXT_FPU),
    X3(0xF9000000, TEXT_FDMUL,   M_RC, M_RA, M_RB, EXT_FPU),
    X3(0xF6000000, TEXT_FDIV,    M_RC, M_RA, M_RB, EXT_FPU),
    X3(0xF7000000, TEXT_DDIV,    M_RC, M_RA, M_RB, EXT_FPU),
    X3(0xEA000000, TEXT_FEQ,     M_RC, M_RA, M_RB, EXT_FPU),
    X3(0xEB000000, TEXT_DEQ,     M_RC, M_RA, M_RB, EXT_FPU),
    X3(0xEC000000, TEXT_FGT,     M_RC, M_RA, M_RB, EXT_FPU),
    X3(0xED000000, TEXT_DGT,     M_RC, M_RA, M_RB, EXT_FPU),
    X3(0xEE000000, TEXT_FGE,     M_RC, M_RA, M_RB, EXT_FPU),
    X3(0xEF000000, TEXT_DGE,     M_RC, M_RA, M_RB, EXT_FPU),
    X6(0xE4000000, TEXT_CONVERT, M_RC, M_RA, M_UI, M_RND, M_FD, M_FS, EXT_FPU),
    X3(0xE5000000, TEXT_SQRT,    M_RC, M_RA, M_FMT, EXT_FPU),
    X3(0xE6000000, TEXT_CLASS,   M_RC, M_RA, M_FMT, EXT_FPU),
    // Branch
    E1(0xA0000000, TEXT_JMP,     M_TGT),
    E2(0xA4000000, TEXT_JMPF,    M_RA, M_TGT),
    E2(0xAC000000, TEXT_JMPT,    M_RA, M_TGT),
    E2(0xB4000000, TEXT_JMPFDEC, M_RA, M_TGT),
    E2(0xA8000000, TEXT_CALL,    M_RA, M_TGT),
    E1(0xC0000000, TEXT_JMPI,    M_RB),
    E2(0xC4000000, TEXT_JMPFI,   M_RA, M_RB),
    E2(0xCC000000, TEXT_JMPTI,   M_RA, M_RB),
    E2(0xC8000000, TEXT_CALLI,   M_RA, M_RB),
    // Miscellaneous
    E2(0x08000000, TEXT_CLZ,     M_RC, M_RBI),
    E3(0x9E000000, TEXT_SETIP,   M_RC, M_RA, M_RB),
    E3(0xD7000000, TEXT_EMULATE, M_VN, M_RA, M_RB),
    E1(0x9F000000, TEXT_INV,     M_ID),
    E0(0x88000000, TEXT_IRET),
    E1(0x8C000000, TEXT_IRETINV, M_ID),
    E0(0x89000000, TEXT_HALT),
};

// Instructions found only on the Am29050.  Every other family member reserves
// these operation codes for instruction emulation.
constexpr Entry TABLE_AM29050[] PROGMEM = {
    E3(0xAA000000, TEXT_ORN,     M_RC, M_RA, M_RBI),
    X4(0xD8000000, TEXT_FMAC,    M_FUNC, M_ACNH, M_RA, M_RB, EXT_FPU),
    X4(0xD9000000, TEXT_DMAC,    M_FUNC, M_ACNH, M_RA, M_RB, EXT_FPU),
    X3(0xDA000000, TEXT_FMSM,    M_RC, M_RA, M_RB, EXT_FPU),
    X3(0xDB000000, TEXT_DMSM,    M_RC, M_RA, M_RB, EXT_FPU),
    X3(0xE8010000, TEXT_MTACC,   M_RA, M_TFMT, M_ACN, EXT_FPU),
    X3(0xE9000100, TEXT_MFACC,   M_RC, M_TFMT, M_ACN, EXT_FPU),
};

// clang-format on

using EntryPage = entry::TableBase<Entry>;

constexpr EntryPage AM29000_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_AM29000), nullptr, nullptr},
};

constexpr EntryPage AM29050_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_AM29000), nullptr, nullptr},
        {ARRAY_RANGE(TABLE_AM29050), nullptr, nullptr},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {AM29200, TEXT_CPU_AM29200, ARRAY_RANGE(AM29000_PAGES)},
        {AM29245, TEXT_CPU_AM29245, ARRAY_RANGE(AM29000_PAGES)},
        {AM29030, TEXT_CPU_AM29030, ARRAY_RANGE(AM29000_PAGES)},
        {AM29240, TEXT_CPU_AM29240, ARRAY_RANGE(AM29000_PAGES)},
        {AM29040, TEXT_CPU_AM29040, ARRAY_RANGE(AM29000_PAGES)},
        {AM29050, TEXT_CPU_AM29050, ARRAY_RANGE(AM29050_PAGES)},
};

namespace {

const Cpu *cpu(CpuType cpuType) {
    return Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
}

// A special-purpose register number which this CPU does not implement reads
// back an unpredictable value, and the manual says to avoid it for the same
// upward-compatibility reason it gives for a reserved field.  Turn the entry
// down, so the encoding comes out unknown rather than naming a register which
// does not exist.
bool spRegOk(const DisInsn &insn, const Entry::Flags &flags) {
    for (uint_fast8_t pos = 0; pos < MAX_OPERANDS; pos++) {
        if (flags.mode(pos) == M_SA)
            return reg::decodeSpReg((insn.opCode() >> 8) & 0xFF, insn.cpuType()) != REG_UNDEF;
    }
    return true;
}

// An encoding which selects a reserved value of a floating-point control field
// is turned down, so it comes out unknown rather than naming an operation the
// manual does not define.
bool controlOk(const DisInsn &insn, const Entry::Flags &flags) {
    const auto opc = insn.opCode();
    for (uint_fast8_t pos = 0; pos < MAX_OPERANDS; pos++) {
        const auto mode = flags.mode(pos);
        uint8_t value;
        switch (mode) {
        case M_RND:
            value = (opc >> 4) & 7;
            break;
        case M_FD:
        case M_TFMT:
            value = (opc >> 2) & 3;
            break;
        case M_FS:
        case M_FMT:
            value = opc & 3;
            break;
        default:
            continue;
        }
        if (!controlValueOk(mode, value))
            return false;
    }
    return true;
}

bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *) {
    const auto flags = entry->readFlags();
    if (!insn.allows(flags.extension()))
        return false;
    if (!spRegOk(insn, flags))
        return false;
    if (!controlOk(insn, flags))
        return false;
    auto mask = flags.mask();
    if (flags.hasCe() && !insn.hasCopro())
        mask &= ~BIT_CE;  // reserved without a coprocessor, so it reads as zero
    return (insn.opCode() & ~mask) == entry->readOpCode();
}

}  // namespace

Error searchOpCode(CpuType cpuType, bool fpu, bool intMul, DisInsn &insn, StrBuffer &out) {
    insn.setExtensions(fpu, intMul, cpuType);
    cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    return insn.getError();
}

const /*PROGMEM*/ char *TableAm29000::listCpu_P() const {
    return TEXT_AM29000_LIST;
}

const /*PROGMEM*/ char *TableAm29000::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableAm29000::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    const auto *t = Cpu::search(name, ARRAY_RANGE(CPU_TABLE));
    if (t == nullptr)
        return UNSUPPORTED_CPU;
    cpuType = t->readCpuType();
    return OK;
}

const TableAm29000 TABLE;

}  // namespace am29000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

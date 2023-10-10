/*
 * Copyright 2020 Tadashi G. Takaoka
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

#include "table_ins8070.h"

#include "entry_ins8070.h"
#include "entry_table.h"
#include "text_ins8070.h"

using namespace libasm::text::ins8070;

namespace libasm {
namespace ins8070 {

#define E2(_opc, _name, _dst, _src, _size) \
    { _opc, Entry::Flags::create(_dst, _src, SZ_##_size), _name }
#define E1(_opc, _name, _dst, _size) E2(_opc, _name, _dst, M_NONE, _size)
#define E0(_opc, _name) E1(_opc, _name, M_NONE, NONE)
#define X1(_opc, _name, _dst) \
    { _opc, Entry::Flags::exec(_dst), _name }
#define U2(_opc, _name, _dst, _src) \
    { _opc, Entry::Flags::undef(_dst, _src), _name }
#define U1(_opc, _name, _dst) U2(_opc, _name, _dst, M_NONE)

// clang-format off
static constexpr Entry TABLE_INS8070[] PROGMEM = {
    E0(0x00, TEXT_NOP),
    E2(0x01, TEXT_XCH,  M_AR,   M_ER,   BYTE),
    E2(0x01, TEXT_XCH,  M_ER,   M_AR,   BYTE),
    E2(0x06, TEXT_LD,   M_AR,   M_SR,   BYTE),
    E2(0x07, TEXT_LD,   M_SR,   M_AR,   BYTE),
    E1(0x08, TEXT_PUSH, M_EA,   WORD),
    E2(0x09, TEXT_LD,   M_TR,   M_EA,   WORD),
    E1(0x0A, TEXT_PUSH, M_AR,   BYTE),
    E2(0x0B, TEXT_LD,   M_EA,   M_TR,   WORD),
    E1(0x0C, TEXT_SR,   M_EA,   WORD),
    E2(0x0D, TEXT_DIV,  M_EA,   M_TR,   WORD),
    E1(0x0E, TEXT_SL,   M_AR,   BYTE),
    E1(0x0F, TEXT_SL,   M_EA,   WORD),
    E1(0x10, TEXT_CALL, M_VEC,  NONE),
    X1(0x20, TEXT_JSR,  M_ADR),
    E2(0x22, TEXT_PLI,  M_P23,  M_IMM,  WORD),
    X1(0x24, TEXT_JMP,  M_ADR),
    E2(0x24, TEXT_LD,   M_PTR,  M_IMM,  WORD),
    E2(0x2C, TEXT_MPY,  M_EA,   M_TR,   WORD),
    X1(0x2D, TEXT_BND,  M_PCR),
    E1(0x2E, TEXT_SSM,  M_P23,  BYTE),
    E2(0x30, TEXT_LD,   M_EA,   M_PTR,  WORD),
    E1(0x38, TEXT_POP,  M_AR,   BYTE),
    E2(0x39, TEXT_AND,  M_SR,   M_IMM,  BYTE),
    E1(0x3A, TEXT_POP,  M_EA,   WORD),
    E2(0x3B, TEXT_OR,   M_SR,   M_IMM,  BYTE),
    E1(0x3C, TEXT_SR,   M_AR,   BYTE),
    E1(0x3D, TEXT_SRL,  M_AR,   BYTE),
    E1(0x3E, TEXT_RR,   M_AR,   BYTE),
    E1(0x3F, TEXT_RRL,  M_AR,   BYTE),
    E2(0x40, TEXT_LD,   M_AR,   M_ER,   BYTE),
    E2(0x44, TEXT_LD,   M_PTR,  M_EA,   WORD),
    E2(0x48, TEXT_LD,   M_ER,   M_AR,   BYTE),
    E2(0x4C, TEXT_XCH,  M_EA,   M_PTR,  WORD),
    E2(0x4C, TEXT_XCH,  M_PTR,  M_EA,   WORD),
    E2(0x50, TEXT_AND,  M_AR,   M_ER,   BYTE),
    U1(0x55, TEXT_PUSH, M_SP), // undefined PUSH SP
    E1(0x54, TEXT_PUSH, M_PTR,  WORD),
    E2(0x58, TEXT_OR,   M_AR,   M_ER,   BYTE),
    E0(0x5C, TEXT_RET),
    E1(0x5E, TEXT_POP,  M_P23,  WORD),
    E2(0x60, TEXT_XOR,  M_AR,   M_ER,   BYTE),
    X1(0x64, TEXT_BP,   M_PCR),
    X1(0x66, TEXT_BP,   M_IDX),
    X1(0x6C, TEXT_BZ,   M_PCR),
    X1(0x6E, TEXT_BZ,   M_IDX),
    E2(0x70, TEXT_ADD,  M_AR,   M_ER,   BYTE),
    X1(0x74, TEXT_BRA,  M_PCR),
    X1(0x76, TEXT_BRA,  M_IDX),
    E2(0x78, TEXT_SUB,  M_AR,   M_ER,   BYTE),
    X1(0x7C, TEXT_BNZ,  M_PCR),
    X1(0x7E, TEXT_BNZ,  M_IDX),
    E2(0x80, TEXT_LD,   M_EA,   M_GEN,  WORD),
    U2(0x8C, TEXT_ST,   M_EA,   M_IMM), // undefined ST EA immediate
    E2(0x88, TEXT_ST,   M_EA,   M_GEN,  WORD),
    U2(0x94, TEXT_ILD,  M_AR,   M_IMM), // undefined ILD immediate
    E2(0x90, TEXT_ILD,  M_AR,   M_GEN,  BYTE),
    U2(0x9C, TEXT_DLD,  M_AR,   M_IMM), // undefined DLD immediate
    E2(0x98, TEXT_DLD,  M_AR,   M_GEN,  BYTE),
    E2(0xA0, TEXT_LD,   M_TR,   M_GEN,  WORD),
    E2(0xB0, TEXT_ADD,  M_EA,   M_GEN,  WORD),
    E2(0xB8, TEXT_SUB,  M_EA,   M_GEN,  WORD),
    E2(0xC0, TEXT_LD,   M_AR,   M_GEN,  BYTE),
    U2(0xCC, TEXT_ST,   M_AR,   M_IMM), // undefined ST A immediate
    E2(0xC8, TEXT_ST,   M_AR,   M_GEN,  BYTE),
    E2(0xD0, TEXT_AND,  M_AR,   M_GEN,  BYTE),
    E2(0xD8, TEXT_OR,   M_AR,   M_GEN,  BYTE),
    E2(0xE0, TEXT_XOR,  M_AR,   M_GEN,  BYTE),
    E2(0xF0, TEXT_ADD,  M_AR,   M_GEN,  BYTE),
    E2(0xF8, TEXT_SUB,  M_AR,   M_GEN,  BYTE),
};

static constexpr uint8_t INDEX_INS8070[] PROGMEM = {
     46,  // TEXT_ADD
     60,  // TEXT_ADD
     68,  // TEXT_ADD
     23,  // TEXT_AND
     35,  // TEXT_AND
     65,  // TEXT_AND
     19,  // TEXT_BND
     50,  // TEXT_BNZ
     51,  // TEXT_BNZ
     42,  // TEXT_BP
     43,  // TEXT_BP
     47,  // TEXT_BRA
     48,  // TEXT_BRA
     44,  // TEXT_BZ
     45,  // TEXT_BZ
     13,  // TEXT_CALL
     10,  // TEXT_DIV
     57,  // TEXT_DLD
     58,  // TEXT_DLD
     55,  // TEXT_ILD
     56,  // TEXT_ILD
     16,  // TEXT_JMP
     14,  // TEXT_JSR
      3,  // TEXT_LD
      4,  // TEXT_LD
      6,  // TEXT_LD
      8,  // TEXT_LD
     17,  // TEXT_LD
     21,  // TEXT_LD
     30,  // TEXT_LD
     31,  // TEXT_LD
     32,  // TEXT_LD
     52,  // TEXT_LD
     59,  // TEXT_LD
     62,  // TEXT_LD
     18,  // TEXT_MPY
      0,  // TEXT_NOP
     25,  // TEXT_OR
     38,  // TEXT_OR
     66,  // TEXT_OR
     15,  // TEXT_PLI
     22,  // TEXT_POP
     24,  // TEXT_POP
     40,  // TEXT_POP
      5,  // TEXT_PUSH
      7,  // TEXT_PUSH
     36,  // TEXT_PUSH
     37,  // TEXT_PUSH
     39,  // TEXT_RET
     28,  // TEXT_RR
     29,  // TEXT_RRL
     11,  // TEXT_SL
     12,  // TEXT_SL
      9,  // TEXT_SR
     26,  // TEXT_SR
     27,  // TEXT_SRL
     20,  // TEXT_SSM
     53,  // TEXT_ST
     54,  // TEXT_ST
     63,  // TEXT_ST
     64,  // TEXT_ST
     49,  // TEXT_SUB
     61,  // TEXT_SUB
     69,  // TEXT_SUB
      1,  // TEXT_XCH
      2,  // TEXT_XCH
     33,  // TEXT_XCH
     34,  // TEXT_XCH
     41,  // TEXT_XOR
     67,  // TEXT_XOR
};
// clang-format on

using EntryPage = entry::TableBase<Entry>;

static constexpr EntryPage INS8070_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_INS8070), ARRAY_RANGE(INDEX_INS8070)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

static constexpr Cpu CPU_TABLE[] PROGMEM = {
        {INS8070, TEXT_CPU_8070, ARRAY_RANGE(INS8070_PAGES)},
};

static const Cpu *cpu(CpuType cpuType) {
    UNUSED(cpuType);
    return &CPU_TABLE[0];
}

static bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_ADR)
        return table == M_GEN || table == M_PCR;
    if (opr == M_VEC)
        return table == M_GEN || table == M_PCR || table == M_ADR;
    if (opr == M_IDX)
        return table == M_GEN;
    if (opr == M_IMM)
        return table == M_GEN;
    if (opr == M_P23)
        return table == M_PTR;
    if (opr == M_SP)
        return table == M_PTR;
    return false;
}

static bool acceptModes(AsmInsn &insn, const Entry *entry) {
    auto flags = insn.flags();
    auto table = entry->flags();
    if (acceptMode(flags.dst(), table.dst()) && acceptMode(flags.src(), table.src())) {
        if (table.undefined())
            insn.setErrorIf(OPERAND_NOT_ALLOWED);
        return true;
    }
    return false;
}

Error TableIns8070::searchName(CpuType cpuType, AsmInsn &insn) const {
    cpu(cpuType)->searchName(insn, acceptModes);
    return insn.getError();
}

static Config::opcode_t maskCode(AddrMode mode) {
    switch (mode) {
    case M_VEC:
        return ~0x0F;
    case M_IDX:
    case M_P23:
        return ~0x01;
    case M_PTR:
        return ~0x03;
    case M_GEN:
        return ~0x07;
    default:
        return ~0;
    }
}

static bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *page) {
    UNUSED(page);
    auto opCode = insn.opCode();
    const auto flags = entry->flags();
    opCode &= maskCode(flags.dst());
    opCode &= maskCode(flags.src());
    return opCode == entry->opCode();
}

Error TableIns8070::searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) const {
    auto entry = cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    if (entry && entry->flags().undefined()) {
        insn.nameBuffer().reset();
        insn.setErrorIf(UNKNOWN_INSTRUCTION);
    }
    return insn.getError();
}

const /*PROGMEM*/ char *TableIns8070::listCpu_P() const {
    return TEXT_CPU_INS8070;
}

const /*PROGMEM*/ char *TableIns8070::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableIns8070::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    if (name.iequals_P(TEXT_CPU_INS8070) || name.iequals_P(TEXT_CPU_INS8070 + 3)) {
        cpuType = INS8070;
        return OK;
    }
    return UNSUPPORTED_CPU;
}

const TableIns8070 TABLE;

}  // namespace ins8070
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

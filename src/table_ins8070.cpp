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

#define E2(_opc, _cf, _name, _dst, _src, _size) \
    {_opc, Entry::Flags::create(_cf, _dst, _src, _size), _name}
#define E1(_opc, _cf, _name, _dst, _size) E2(_opc, _cf, _name, _dst, M_NONE, _size)
#define E0(_opc, _cf, _name) E1(_opc, _cf, _name, M_NONE, SZ_NONE)
#define X1(_opc, _cf, _name, _dst) {_opc, Entry::Flags::exec(_cf, _dst), _name}
#define U2(_opc, _cf, _name, _dst, _src) {_opc, Entry::Flags::undef(_cf, _dst, _src), _name}
#define U1(_opc, _cf, _name, _dst) U2(_opc, _cf, _name, _dst, M_NONE)

// clang-format off
constexpr Entry TABLE_INS8070[] PROGMEM = {
    E0(0x00, CF_00, TEXT_NOP),
    E2(0x01, CF_00, TEXT_XCH,  M_AR,   M_ER,   SZ_BYTE),
    E2(0x01, CF_00, TEXT_XCH,  M_ER,   M_AR,   SZ_BYTE),
    E2(0x06, CF_00, TEXT_LD,   M_AR,   M_SR,   SZ_BYTE),
    E2(0x07, CF_00, TEXT_LD,   M_SR,   M_AR,   SZ_BYTE),
    E1(0x08, CF_00, TEXT_PUSH, M_EA,           SZ_WORD),
    E2(0x09, CF_00, TEXT_LD,   M_TR,   M_EA,   SZ_WORD),
    E1(0x0A, CF_00, TEXT_PUSH, M_AR,           SZ_BYTE),
    E2(0x0B, CF_00, TEXT_LD,   M_EA,   M_TR,   SZ_WORD),
    E1(0x0C, CF_00, TEXT_SR,   M_EA,           SZ_WORD),
    E2(0x0D, CF_00, TEXT_DIV,  M_EA,   M_TR,   SZ_WORD),
    E1(0x0E, CF_00, TEXT_SL,   M_AR,           SZ_BYTE),
    E1(0x0F, CF_00, TEXT_SL,   M_EA,           SZ_WORD),
    E1(0x10, CF_0F, TEXT_CALL, M_VEC,          SZ_NONE),
    X1(0x20, CF_00, TEXT_JSR,  M_ADR),
    E2(0x22, CF_01, TEXT_PLI,  M_P23,  M_IMM,  SZ_WORD),
    X1(0x24, CF_00, TEXT_JMP,  M_ADR),
    E2(0x24, CF_03, TEXT_LD,   M_PTR,  M_IMM,  SZ_WORD),
    E2(0x2C, CF_00, TEXT_MPY,  M_EA,   M_TR,   SZ_WORD),
    X1(0x2D, CF_00, TEXT_BND,  M_PCR),
    E1(0x2E, CF_01, TEXT_SSM,  M_P23,          SZ_BYTE),
    E2(0x30, CF_03, TEXT_LD,   M_EA,   M_PTR,  SZ_WORD),
    E1(0x38, CF_00, TEXT_POP,  M_AR,           SZ_BYTE),
    E2(0x39, CF_00, TEXT_AND,  M_SR,   M_IMM,  SZ_BYTE),
    E1(0x3A, CF_00, TEXT_POP,  M_EA,           SZ_WORD),
    E2(0x3B, CF_00, TEXT_OR,   M_SR,   M_IMM,  SZ_BYTE),
    E1(0x3C, CF_00, TEXT_SR,   M_AR,           SZ_BYTE),
    E1(0x3D, CF_00, TEXT_SRL,  M_AR,           SZ_BYTE),
    E1(0x3E, CF_00, TEXT_RR,   M_AR,           SZ_BYTE),
    E1(0x3F, CF_00, TEXT_RRL,  M_AR,           SZ_BYTE),
    E2(0x40, CF_00, TEXT_LD,   M_AR,   M_ER,   SZ_BYTE),
    E2(0x44, CF_03, TEXT_LD,   M_PTR,  M_EA,   SZ_WORD),
    E2(0x48, CF_00, TEXT_LD,   M_ER,   M_AR,   SZ_BYTE),
    E2(0x4C, CF_03, TEXT_XCH,  M_EA,   M_PTR,  SZ_WORD),
    E2(0x4C, CF_03, TEXT_XCH,  M_PTR,  M_EA,   SZ_WORD),
    E2(0x50, CF_00, TEXT_AND,  M_AR,   M_ER,   SZ_BYTE),
    U1(0x55, CF_00, TEXT_PUSH, M_SP), // undefined PUSH SP
    E1(0x54, CF_03, TEXT_PUSH, M_PTR,          SZ_WORD),
    E2(0x58, CF_00, TEXT_OR,   M_AR,   M_ER,   SZ_BYTE),
    E0(0x5C, CF_00, TEXT_RET),
    E1(0x5E, CF_01, TEXT_POP,  M_P23,          SZ_WORD),
    E2(0x60, CF_00, TEXT_XOR,  M_AR,   M_ER,   SZ_BYTE),
    X1(0x64, CF_00, TEXT_BP,   M_PCR),
    X1(0x66, CF_01, TEXT_BP,   M_IDX),
    X1(0x6C, CF_00, TEXT_BZ,   M_PCR),
    X1(0x6E, CF_01, TEXT_BZ,   M_IDX),
    E2(0x70, CF_00, TEXT_ADD,  M_AR,   M_ER,   SZ_BYTE),
    X1(0x74, CF_00, TEXT_BRA,  M_PCR),
    X1(0x76, CF_01, TEXT_BRA,  M_IDX),
    E2(0x78, CF_00, TEXT_SUB,  M_AR,   M_ER,   SZ_BYTE),
    X1(0x7C, CF_00, TEXT_BNZ,  M_PCR),
    X1(0x7E, CF_01, TEXT_BNZ,  M_IDX),
    E2(0x80, CF_07, TEXT_LD,   M_EA,   M_GEN,  SZ_WORD),
    U2(0x8C, CF_00, TEXT_ST,   M_EA,   M_IMM), // undefined ST EA immediate
    E2(0x88, CF_07, TEXT_ST,   M_EA,   M_GEN,  SZ_WORD),
    U2(0x94, CF_00, TEXT_ILD,  M_AR,   M_IMM), // undefined ILD immediate
    E2(0x90, CF_07, TEXT_ILD,  M_AR,   M_GEN,  SZ_BYTE),
    U2(0x9C, CF_00, TEXT_DLD,  M_AR,   M_IMM), // undefined DLD immediate
    E2(0x98, CF_07, TEXT_DLD,  M_AR,   M_GEN,  SZ_BYTE),
    E2(0xA0, CF_07, TEXT_LD,   M_TR,   M_GEN,  SZ_WORD),
    E2(0xB0, CF_07, TEXT_ADD,  M_EA,   M_GEN,  SZ_WORD),
    E2(0xB8, CF_07, TEXT_SUB,  M_EA,   M_GEN,  SZ_WORD),
    E2(0xC0, CF_07, TEXT_LD,   M_AR,   M_GEN,  SZ_BYTE),
    U2(0xCC, CF_00, TEXT_ST,   M_AR,   M_IMM), // undefined ST A immediate
    E2(0xC8, CF_07, TEXT_ST,   M_AR,   M_GEN,  SZ_BYTE),
    E2(0xD0, CF_07, TEXT_AND,  M_AR,   M_GEN,  SZ_BYTE),
    E2(0xD8, CF_07, TEXT_OR,   M_AR,   M_GEN,  SZ_BYTE),
    E2(0xE0, CF_07, TEXT_XOR,  M_AR,   M_GEN,  SZ_BYTE),
    E2(0xF0, CF_07, TEXT_ADD,  M_AR,   M_GEN,  SZ_BYTE),
    E2(0xF8, CF_07, TEXT_SUB,  M_AR,   M_GEN,  SZ_BYTE),
};

constexpr uint8_t INDEX_INS8070[] PROGMEM = {
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

constexpr EntryPage INS8070_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_INS8070), ARRAY_RANGE(INDEX_INS8070)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {INS8070, TEXT_CPU_8070, ARRAY_RANGE(INS8070_PAGES)},
};

const Cpu *cpu(CpuType) {
    return &CPU_TABLE[0];
}

bool acceptMode(AddrMode opr, AddrMode table) {
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

bool acceptModes(AsmInsn &insn, const Entry *entry) {
    const auto table = entry->readFlags();
    if (acceptMode(insn.dstOp.mode, table.dst()) && acceptMode(insn.srcOp.mode, table.src())) {
        if (table.undefined())
            insn.setErrorIf(OPERAND_NOT_ALLOWED);
        return true;
    }
    return false;
}

Error searchName(CpuType cpuType, AsmInsn &insn) {
    cpu(cpuType)->searchName(insn, acceptModes);
    return insn.getError();
}

bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *) {
    auto opc = insn.opCode();
    opc &= ~entry->readFlags().mask();
    return opc == entry->readOpCode();
}

Error searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) {
    auto entry = cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    if (entry && entry->readFlags().undefined()) {
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

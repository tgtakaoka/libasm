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

#include "table_ins8060.h"
#include "entry_ins8060.h"
#include "entry_table.h"
#include "text_ins8060.h"

using namespace libasm::text::ins8060;

namespace libasm {
namespace ins8060 {

#define E1(_opc, _name, _mode) {_opc, Entry::Flags::create(_mode), _name}
#define E0(_opc, _name) E1(_opc, _name, M_NONE)

// clang-format off
constexpr Entry TABLE_INS8060[] PROGMEM = {
    E0(0x00, TEXT_HALT),
    E0(0x01, TEXT_XAE),
    E0(0x02, TEXT_CCL),
    E0(0x03, TEXT_SCL),
    E0(0x04, TEXT_DINT),
    E0(0x05, TEXT_IEN),
    E0(0x06, TEXT_CSA),
    E0(0x07, TEXT_CAS),
    E0(0x08, TEXT_NOP),
    E0(0x19, TEXT_SIO),
    E0(0x1C, TEXT_SR),
    E0(0x1D, TEXT_SRL),
    E0(0x1E, TEXT_RR),
    E0(0x1F, TEXT_RRL),
    E1(0x30, TEXT_XPAL, M_PNTR),
    E1(0x34, TEXT_XPAH, M_PNTR),
    E1(0x3C, TEXT_XPPC, M_PNTR),
    E0(0x40, TEXT_LDE),
    E0(0x50, TEXT_ANE),
    E0(0x58, TEXT_ORE),
    E0(0x60, TEXT_XRE),
    E0(0x68, TEXT_DAE),
    E0(0x70, TEXT_ADE),
    E0(0x78, TEXT_CAE),
    E1(0x8F, TEXT_DLY,  M_IMM8),
    E1(0x90, TEXT_JMP,  M_REL8),
    E1(0x94, TEXT_JP,   M_REL8),
    E1(0x98, TEXT_JZ,   M_REL8),
    E1(0x9C, TEXT_JNZ,  M_REL8),
    E1(0xA8, TEXT_ILD,  M_DISP),
    E1(0xB8, TEXT_DLD,  M_DISP),
    E1(0xC4, TEXT_LDI,  M_IMM8),
    E1(0xC0, TEXT_LD,   M_INDX),
    E1(0xCC, TEXT_ST,   M_UNDEF),  // undefined ST immediate instruction
    E1(0xC8, TEXT_ST,   M_INDX),
    E1(0xD4, TEXT_ANI,  M_IMM8),
    E1(0xD0, TEXT_AND,  M_INDX),
    E1(0xDC, TEXT_ORI,  M_IMM8),
    E1(0xD8, TEXT_OR,   M_INDX),
    E1(0xE4, TEXT_XRI,  M_IMM8),
    E1(0xE0, TEXT_XOR,  M_INDX),
    E1(0xEC, TEXT_DAI,  M_IMM8),
    E1(0xE8, TEXT_DAD,  M_INDX),
    E1(0xF4, TEXT_ADI,  M_IMM8),
    E1(0xF0, TEXT_ADD,  M_INDX),
    E1(0xFC, TEXT_CAI,  M_IMM8),
    E1(0xF8, TEXT_CAD,  M_INDX),
};

constexpr uint8_t INDEX_INS8060[] PROGMEM = {
     44,  // TEXT_ADD
     22,  // TEXT_ADE
     43,  // TEXT_ADI
     36,  // TEXT_AND
     18,  // TEXT_ANE
     35,  // TEXT_ANI
     46,  // TEXT_CAD
     23,  // TEXT_CAE
     45,  // TEXT_CAI
      7,  // TEXT_CAS
      2,  // TEXT_CCL
      6,  // TEXT_CSA
     42,  // TEXT_DAD
     21,  // TEXT_DAE
     41,  // TEXT_DAI
      4,  // TEXT_DINT
     30,  // TEXT_DLD
     24,  // TEXT_DLY
      0,  // TEXT_HALT
      5,  // TEXT_IEN
     29,  // TEXT_ILD
     25,  // TEXT_JMP
     28,  // TEXT_JNZ
     26,  // TEXT_JP
     27,  // TEXT_JZ
     32,  // TEXT_LD
     17,  // TEXT_LDE
     31,  // TEXT_LDI
      8,  // TEXT_NOP
     38,  // TEXT_OR
     19,  // TEXT_ORE
     37,  // TEXT_ORI
     12,  // TEXT_RR
     13,  // TEXT_RRL
      3,  // TEXT_SCL
      9,  // TEXT_SIO
     10,  // TEXT_SR
     11,  // TEXT_SRL
     33,  // TEXT_ST
     34,  // TEXT_ST
      1,  // TEXT_XAE
     40,  // TEXT_XOR
     15,  // TEXT_XPAH
     14,  // TEXT_XPAL
     16,  // TEXT_XPPC
     20,  // TEXT_XRE
     39,  // TEXT_XRI
};
// clang-format on

using EntryPage = entry::TableBase<Entry>;

constexpr EntryPage INS8060_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_INS8060), ARRAY_RANGE(INDEX_INS8060)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {INS8060, TEXT_CPU_SCMP, ARRAY_RANGE(INS8060_PAGES)},
};

const Cpu *cpu(CpuType) {
    return &CPU_TABLE[0];
}

bool acceptMode(AsmInsn &insn, const Entry *entry) {
    const auto opr = insn.op.mode;
    const auto table = entry->readFlags().mode();
    if (opr == table)
        return true;
    if (opr == M_REL8)
        return table == M_IMM8 || table == M_DISP || table == M_INDX;
    if (opr == M_DISP)
        return table == M_REL8 || table == M_INDX;
    return false;
}

Error searchName(CpuType cpuType, AsmInsn &insn) {
    cpu(cpuType)->searchName(insn, acceptMode);
    return insn.getError();
}

bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *) {
    auto opc = insn.opCode();
    const auto mode = entry->readFlags().mode();
    if (mode == M_INDX) {
        opc &= ~0x07;
    } else if (mode == M_PNTR || mode == M_REL8 || mode == M_DISP) {
        opc &= ~0x03;
    }
    return opc == entry->readOpCode();
}

Error searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) {
    cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    if (insn.addrMode() == M_UNDEF) {
        insn.nameBuffer().reset();
        insn.setError(UNKNOWN_INSTRUCTION);
    }
    return insn.getError();
}

const /*PROGMEM*/ char *TableIns8060::listCpu_P() const {
    return TEXT_CPU_INS8060;
}

const /*PROGMEM*/ char *TableIns8060::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableIns8060::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    if (name.iequals_P(TEXT_CPU_SCMP) || name.iequals_P(TEXT_CPU_INS8060) ||
            name.iequals_P(TEXT_CPU_INS8060 + 3)) {
        cpuType = INS8060;
        return OK;
    }
    return UNSUPPORTED_CPU;
}

const TableIns8060 TABLE;

}  // namespace ins8060
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

/*
 * Copyright 2023 Tadashi G. Takaoka
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

#include "table_f3850.h"
#include "entry_f3850.h"
#include "entry_table.h"
#include "text_f3850.h"

using namespace libasm::text::f3850;

namespace libasm {
namespace f3850 {

#define E2(_opc, _cf, _name, _mode1, _mode2) \
    {_opc, Entry::Flags::create(_cf, _mode1, _mode2), _name}
#define E1(_opc, _cf, _name, _mode1) E2(_opc, _cf, _name, _mode1, M_NONE)
#define E0(_opc, _cf, _name) E1(_opc, _cf, _name, M_NONE)

// clang-format off
constexpr Entry TABLE_3850[] PROGMEM = {
    E2(0x00, CF_00, TEXT_LR,  M_A,    M_KU),
    E2(0x01, CF_00, TEXT_LR,  M_A,    M_KL),
    E2(0x02, CF_00, TEXT_LR,  M_A,    M_QU),
    E2(0x03, CF_00, TEXT_LR,  M_A,    M_QL),
    E2(0x04, CF_00, TEXT_LR,  M_KU,   M_A),
    E2(0x05, CF_00, TEXT_LR,  M_KL,   M_A),
    E2(0x06, CF_00, TEXT_LR,  M_QU,   M_A),
    E2(0x07, CF_00, TEXT_LR,  M_QL,   M_A),
    E2(0x08, CF_00, TEXT_LR,  M_K,    M_P),
    E2(0x09, CF_00, TEXT_LR,  M_P,    M_K),
    E2(0x0A, CF_00, TEXT_LR,  M_A,    M_IS),
    E2(0x0B, CF_00, TEXT_LR,  M_IS,   M_A),
    E0(0x0C, CF_00, TEXT_PK),
    E2(0x0D, CF_00, TEXT_LR,  M_P0,   M_Q),
    E2(0x0E, CF_00, TEXT_LR,  M_Q,    M_DC),
    E2(0x0F, CF_00, TEXT_LR,  M_DC,   M_Q),
    E2(0x10, CF_00, TEXT_LR,  M_DC,   M_H),
    E2(0x11, CF_00, TEXT_LR,  M_H,    M_DC),
    E1(0x12, CF_00, TEXT_SR,  M_C1),
    E1(0x13, CF_00, TEXT_SL,  M_C1),
    E1(0x14, CF_00, TEXT_SR,  M_C4),
    E1(0x15, CF_00, TEXT_SL,  M_C4),
    E0(0x16, CF_00, TEXT_LM),
    E0(0x17, CF_00, TEXT_ST),
    E0(0x18, CF_00, TEXT_COM),
    E0(0x19, CF_00, TEXT_LNK),
    E0(0x1A, CF_00, TEXT_DI),
    E0(0x1B, CF_00, TEXT_EI),
    E0(0x1C, CF_00, TEXT_POP),
    E2(0x1D, CF_00, TEXT_LR,  M_W,   M_J),
    E2(0x1E, CF_00, TEXT_LR,  M_J,   M_W),
    E0(0x1F, CF_00, TEXT_INC),
    E1(0x20, CF_00, TEXT_LI,  M_IM8),
    E1(0x21, CF_00, TEXT_NI,  M_IM8),
    E1(0x22, CF_00, TEXT_OI,  M_IM8),
    E1(0x23, CF_00, TEXT_XI,  M_IM8),
    E1(0x24, CF_00, TEXT_AI,  M_IM8),
    E1(0x25, CF_00, TEXT_CI,  M_IM8),
    E1(0x26, CF_00, TEXT_IN,  M_IOA),
    E1(0x27, CF_00, TEXT_OUT, M_IOA),
    E1(0x28, CF_00, TEXT_PI,  M_ADDR),
    E1(0x29, CF_00, TEXT_JMP, M_ADDR),
    E1(0x2A, CF_00, TEXT_DCI, M_ADDR),
    E0(0x2B, CF_00, TEXT_NOP),
    E0(0x2C, CF_00, TEXT_XDC),
    E1(0x30, CF_0F, TEXT_DS,   M_REG),
    E2(0x40, CF_0F, TEXT_LR,   M_A,   M_REG),
    E2(0x50, CF_0F, TEXT_LR,   M_REG, M_A),
    E1(0x60, CF_07, TEXT_LISU, M_IM3),
    E1(0x68, CF_07, TEXT_LISL, M_IM3),
    E0(0x70, CF_00, TEXT_CLR),
    E1(0x70, CF_0F, TEXT_LIS,  M_IM4),
    E1(0x81, CF_00, TEXT_BP,   M_REL),
    E1(0x82, CF_00, TEXT_BC,   M_REL),
    E1(0x84, CF_00, TEXT_BZ,   M_REL),
    E0(0x88, CF_00, TEXT_AM),
    E0(0x89, CF_00, TEXT_AMD),
    E0(0x8A, CF_00, TEXT_NM),
    E0(0x8B, CF_00, TEXT_OM),
    E0(0x8C, CF_00, TEXT_XM),
    E0(0x8D, CF_00, TEXT_CM),
    E0(0x8E, CF_00, TEXT_ADC),
    E1(0x8F, CF_00, TEXT_BR7,  M_REL),
    E2(0x80, CF_07, TEXT_BT,   M_IM3,  M_REL),
    E1(0x90, CF_00, TEXT_BR,   M_REL),
    E1(0x91, CF_00, TEXT_BM,   M_REL),
    E1(0x92, CF_00, TEXT_BNC,  M_REL),
    E1(0x94, CF_00, TEXT_BNZ,  M_REL),
    E1(0x98, CF_00, TEXT_BNO,  M_REL),
    E2(0x90, CF_0F, TEXT_BF,   M_IM4,  M_REL),
    E1(0xA0, CF_0F, TEXT_INS,  M_IOS),
    E1(0xB0, CF_0F, TEXT_OUTS, M_IOS),
    E1(0xC0, CF_0F, TEXT_AS,   M_REG),
    E1(0xD0, CF_0F, TEXT_ASD,  M_REG),
    E1(0xE0, CF_0F, TEXT_XS,   M_REG),
    E1(0xF0, CF_0F, TEXT_NS,   M_REG),
};

constexpr uint8_t INDEX_3850[] PROGMEM = {
     61,  // TEXT_ADC
     36,  // TEXT_AI
     55,  // TEXT_AM
     56,  // TEXT_AMD
     72,  // TEXT_AS
     73,  // TEXT_ASD
     53,  // TEXT_BC
     69,  // TEXT_BF
     65,  // TEXT_BM
     66,  // TEXT_BNC
     68,  // TEXT_BNO
     67,  // TEXT_BNZ
     52,  // TEXT_BP
     64,  // TEXT_BR
     62,  // TEXT_BR7
     63,  // TEXT_BT
     54,  // TEXT_BZ
     37,  // TEXT_CI
     50,  // TEXT_CLR
     60,  // TEXT_CM
     24,  // TEXT_COM
     42,  // TEXT_DCI
     26,  // TEXT_DI
     45,  // TEXT_DS
     27,  // TEXT_EI
     38,  // TEXT_IN
     31,  // TEXT_INC
     70,  // TEXT_INS
     41,  // TEXT_JMP
     32,  // TEXT_LI
     51,  // TEXT_LIS
     49,  // TEXT_LISL
     48,  // TEXT_LISU
     22,  // TEXT_LM
     25,  // TEXT_LNK
      0,  // TEXT_LR
      1,  // TEXT_LR
      2,  // TEXT_LR
      3,  // TEXT_LR
      4,  // TEXT_LR
      5,  // TEXT_LR
      6,  // TEXT_LR
      7,  // TEXT_LR
      8,  // TEXT_LR
      9,  // TEXT_LR
     10,  // TEXT_LR
     11,  // TEXT_LR
     13,  // TEXT_LR
     14,  // TEXT_LR
     15,  // TEXT_LR
     16,  // TEXT_LR
     17,  // TEXT_LR
     29,  // TEXT_LR
     30,  // TEXT_LR
     46,  // TEXT_LR
     47,  // TEXT_LR
     33,  // TEXT_NI
     57,  // TEXT_NM
     43,  // TEXT_NOP
     75,  // TEXT_NS
     34,  // TEXT_OI
     58,  // TEXT_OM
     39,  // TEXT_OUT
     71,  // TEXT_OUTS
     40,  // TEXT_PI
     12,  // TEXT_PK
     28,  // TEXT_POP
     19,  // TEXT_SL
     21,  // TEXT_SL
     18,  // TEXT_SR
     20,  // TEXT_SR
     23,  // TEXT_ST
     44,  // TEXT_XDC
     35,  // TEXT_XI
     59,  // TEXT_XM
     74,  // TEXT_XS
};

// clang-format on

using EntryPage = entry::TableBase<Entry>;

constexpr EntryPage F3850_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_3850), ARRAY_RANGE(INDEX_3850)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {F3850, TEXT_CPU_3850, ARRAY_RANGE(F3850_PAGES)},
};

const Cpu *cpu(CpuType) {
    return &CPU_TABLE[0];
}

bool hasOperand(CpuType cpuType, AsmInsn &insn) {
    cpu(cpuType)->searchName(insn, Cpu::acceptAll<AsmInsn, Entry>);
    return insn.isOK() && insn.mode1() != M_NONE;
}

bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_J)
        return table == M_REG;
    if (opr == M_ADDR)
        return table == M_C1 || table == M_C4 || table == M_IM3 || table == M_IM4 ||
               table == M_IM8 || table == M_ADDR || table == M_REL || table == M_REG ||
               table == M_IOS || table == M_IOA;
    return false;
}

bool acceptModes(AsmInsn &insn, const Entry *entry) {
    const auto table = entry->readFlags();
    return acceptMode(insn.op1.mode, table.mode1()) && acceptMode(insn.op2.mode, table.mode2());
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
    cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    return insn.getError();
}

const /*PROGMEM*/ char *TableF3850::listCpu_P() const {
    return TEXT_CPU_F3850;
}

const /*PROGMEM*/ char *TableF3850::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableF3850::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    if (name.iequals(TEXT_CPU_3850) || name.iequals(TEXT_CPU_F3850)) {
        cpuType = F3850;
        return OK;
    }
    return UNSUPPORTED_CPU;
}

const TableF3850 TABLE;

}  // namespace f3850
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

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

#define E2(_opc, _name, _mode1, _mode2) \
    { _opc, Entry::Flags::create(_mode1, _mode2), _name }
#define E1(_opc, _name, _mode1) E2(_opc, _name, _mode1, M_NONE)
#define E0(_opc, _name) E1(_opc, _name, M_NONE)
#define U0(_opc, _name) \
    { _opc, Entry::Flags::undef(), _name }

// clang-format off
static constexpr Entry TABLE_3850[] PROGMEM = {
    E2(0x00, TEXT_LR,  M_A,    M_KU),
    E2(0x01, TEXT_LR,  M_A,    M_KL),
    E2(0x02, TEXT_LR,  M_A,    M_QU),
    E2(0x03, TEXT_LR,  M_A,    M_QL),
    E2(0x04, TEXT_LR,  M_KU,   M_A),
    E2(0x05, TEXT_LR,  M_KL,   M_A),
    E2(0x06, TEXT_LR,  M_QU,   M_A),
    E2(0x07, TEXT_LR,  M_QL,   M_A),
    E2(0x08, TEXT_LR,  M_K,    M_P),
    E2(0x09, TEXT_LR,  M_P,    M_K),
    E2(0x0A, TEXT_LR,  M_A,    M_IS),
    E2(0x0B, TEXT_LR,  M_IS,   M_A),
    E0(0x0C, TEXT_PK),
    E2(0x0D, TEXT_LR,  M_P0,   M_Q),
    E2(0x0E, TEXT_LR,  M_Q,    M_DC),
    E2(0x0F, TEXT_LR,  M_DC,   M_Q),
    E2(0x10, TEXT_LR,  M_DC,   M_H),
    E2(0x11, TEXT_LR,  M_H,    M_DC),
    E1(0x12, TEXT_SR,  M_C1),
    E1(0x13, TEXT_SL,  M_C1),
    E1(0x14, TEXT_SR,  M_C4),
    E1(0x15, TEXT_SL,  M_C4),
    E0(0x16, TEXT_LM),
    E0(0x17, TEXT_ST),
    E0(0x18, TEXT_COM),
    E0(0x19, TEXT_LNK),
    E0(0x1A, TEXT_DI),
    E0(0x1B, TEXT_EI),
    E0(0x1C, TEXT_POP),
    E2(0x1D, TEXT_LR,  M_W,   M_J),
    E2(0x1E, TEXT_LR,  M_J,   M_W),
    E0(0x1F, TEXT_INC),
    E1(0x20, TEXT_LI,  M_IM8),
    E1(0x21, TEXT_NI,  M_IM8),
    E1(0x22, TEXT_OI,  M_IM8),
    E1(0x23, TEXT_XI,  M_IM8),
    E1(0x24, TEXT_AI,  M_IM8),
    E1(0x25, TEXT_CI,  M_IM8),
    E1(0x26, TEXT_IN,  M_IM8),
    E1(0x27, TEXT_OUT, M_IM8),
    E1(0x28, TEXT_PI,  M_ADDR),
    E1(0x29, TEXT_JMP, M_ADDR),
    E1(0x2A, TEXT_DCI, M_ADDR),
    E0(0x2B, TEXT_NOP),
    E0(0x2C, TEXT_XDC),
    U0(0x2D, TEXT_null),
    U0(0x2E, TEXT_null),
    U0(0x2F, TEXT_null),
    E1(0x30, TEXT_DS,   M_REG),
    E2(0x40, TEXT_LR,   M_A,   M_REG),
    E2(0x50, TEXT_LR,   M_REG, M_A),
    E1(0x60, TEXT_LISU, M_IM3),
    E1(0x68, TEXT_LISL, M_IM3),
    E0(0x70, TEXT_CLR),
    E1(0x70, TEXT_LIS,  M_IM4),
    E1(0x81, TEXT_BP,   M_REL),
    E1(0x82, TEXT_BC,   M_REL),
    E1(0x84, TEXT_BZ,   M_REL),
    E0(0x88, TEXT_AM),
    E0(0x89, TEXT_AMD),
    E0(0x8A, TEXT_NM),
    E0(0x8B, TEXT_OM),
    E0(0x8C, TEXT_XM),
    E0(0x8D, TEXT_CM),
    E0(0x8E, TEXT_ADC),
    E1(0x8F, TEXT_BR7,  M_REL),
    E2(0x80, TEXT_BT,   M_IM3,  M_REL),
    E1(0x90, TEXT_BR,   M_REL),
    E1(0x91, TEXT_BM,   M_REL),
    E1(0x92, TEXT_BNC,  M_REL),
    E1(0x94, TEXT_BNZ,  M_REL),
    E1(0x98, TEXT_BNO,  M_REL),
    E2(0x90, TEXT_BF,   M_IM4,  M_REL),
    E1(0xA0, TEXT_INS,  M_IM4),
    E1(0xB0, TEXT_OUTS, M_IM4),
    E1(0xC0, TEXT_AS,   M_REG),
    E1(0xD0, TEXT_ASD,  M_REG),
    E1(0xE0, TEXT_XS,   M_REG),
    E1(0xF0, TEXT_NS,   M_REG),
};

static constexpr uint8_t INDEX_3850[] PROGMEM = {
     45,  // TEXT_null
     46,  // TEXT_null
     47,  // TEXT_null
     64,  // TEXT_ADC
     36,  // TEXT_AI
     58,  // TEXT_AM
     59,  // TEXT_AMD
     75,  // TEXT_AS
     76,  // TEXT_ASD
     56,  // TEXT_BC
     72,  // TEXT_BF
     68,  // TEXT_BM
     69,  // TEXT_BNC
     71,  // TEXT_BNO
     70,  // TEXT_BNZ
     55,  // TEXT_BP
     67,  // TEXT_BR
     65,  // TEXT_BR7
     66,  // TEXT_BT
     57,  // TEXT_BZ
     37,  // TEXT_CI
     53,  // TEXT_CLR
     63,  // TEXT_CM
     24,  // TEXT_COM
     42,  // TEXT_DCI
     26,  // TEXT_DI
     48,  // TEXT_DS
     27,  // TEXT_EI
     38,  // TEXT_IN
     31,  // TEXT_INC
     73,  // TEXT_INS
     41,  // TEXT_JMP
     32,  // TEXT_LI
     54,  // TEXT_LIS
     52,  // TEXT_LISL
     51,  // TEXT_LISU
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
     49,  // TEXT_LR
     50,  // TEXT_LR
     33,  // TEXT_NI
     60,  // TEXT_NM
     43,  // TEXT_NOP
     78,  // TEXT_NS
     34,  // TEXT_OI
     61,  // TEXT_OM
     39,  // TEXT_OUT
     74,  // TEXT_OUTS
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
     62,  // TEXT_XM
     77,  // TEXT_XS
};

// clang-format on

using EntryPage = entry::TableBase<Entry>;

static constexpr EntryPage F3850_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_3850), ARRAY_RANGE(INDEX_3850)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

static constexpr Cpu CPU_TABLE[] PROGMEM = {
        {F3850, TEXT_CPU_3850, ARRAY_RANGE(F3850_PAGES)},
};

static const Cpu *cpu(CpuType cpuType) {
    UNUSED(cpuType);
    return &CPU_TABLE[0];
}

static bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_J)
        return table == M_REG;
    if (opr == M_C1 || opr == M_C4 || opr == M_IM3 || opr == M_IM4 || opr == M_IM8 || opr == M_ADDR)
        return table == M_C1 || table == M_C4 || table == M_IM3 || table == M_IM4 ||
               table == M_IM8 || table == M_ADDR || table == M_REL || table == M_REG;
    return false;
}

static bool acceptModes(AsmInsn &insn, const Entry *entry) {
    auto flags = insn.flags();
    auto table = entry->flags();
    return acceptMode(flags.mode1(), table.mode1()) && acceptMode(flags.mode2(), table.mode2());
}

Error TableF3850::searchName(CpuType cpuType, AsmInsn &insn) const {
    cpu(cpuType)->searchName(insn, acceptModes);
    return insn.getError();
}

static bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *page) {
    UNUSED(page);
    auto opCode = insn.opCode();
    const auto flags = entry->flags();
    const auto mode1 = flags.mode1();
    const auto mode2 = flags.mode2();
    if (mode1 == M_REG || mode2 == M_REG || mode1 == M_IM4) {
        opCode &= ~0x0F;
    } else if (mode1 == M_IM3) {
        opCode &= ~0x07;
    }
    return opCode == entry->opCode();
}

Error TableF3850::searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) const {
    auto entry = cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    if (entry && entry->flags().undefined())
        insn.setErrorIf(UNKNOWN_INSTRUCTION);
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

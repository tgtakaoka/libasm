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

#include "table_tms9900.h"

#include "entry_table.h"
#include "entry_tms9900.h"
#include "text_tms9900.h"

using namespace libasm::text::tms9900;

namespace libasm {
namespace tms9900 {

#define E2(_opc, _name, _src, _dst) \
    { _opc, Entry::Flags::create(_src, _dst), _name }
#define E1(_opc, _name, _src) E2(_opc, _name, _src, M_NONE)
#define E0(_opc, _name) E1(_opc, _name, M_NONE)

// clang-format off
static constexpr Entry TABLE_TMS9900[] PROGMEM = {
    E2(0x0200, TEXT_LI,   M_REG,  M_IMM),
    E2(0x0220, TEXT_AI,   M_REG,  M_IMM),
    E2(0x0240, TEXT_ANDI, M_REG,  M_IMM),
    E2(0x0260, TEXT_ORI,  M_REG,  M_IMM),
    E2(0x0280, TEXT_CI,   M_REG,  M_IMM),
    E1(0x02A0, TEXT_STWP, M_REG),
    E1(0x02C0, TEXT_STST, M_REG),
    E1(0x02E0, TEXT_LWPI, M_IMM),
    E1(0x0300, TEXT_LIMI, M_IMM),
    E0(0x0340, TEXT_IDLE),
    E0(0x0360, TEXT_RSET),
    E0(0x0380, TEXT_RTWP),
    E0(0x03A0, TEXT_CKON),
    E0(0x03C0, TEXT_CKOF),
    E0(0x03E0, TEXT_LREX),
    E1(0x0400, TEXT_BLWP, M_SRC),
    E1(0x0440, TEXT_B,    M_SRC),
    E1(0x0480, TEXT_X,    M_SRC),
    E1(0x04C0, TEXT_CLR,  M_SRC),
    E1(0x0500, TEXT_NEG,  M_SRC),
    E1(0x0540, TEXT_INV,  M_SRC),
    E1(0x0580, TEXT_INC,  M_SRC),
    E1(0x05C0, TEXT_INCT, M_SRC),
    E1(0x0600, TEXT_DEC,  M_SRC),
    E1(0x0640, TEXT_DECT, M_SRC),
    E1(0x0680, TEXT_BL,   M_SRC),
    E1(0x06C0, TEXT_SWPB, M_SRC),
    E1(0x0700, TEXT_SETO, M_SRC),
    E1(0x0740, TEXT_ABS,  M_SRC),
    E2(0x0800, TEXT_SRA,  M_REG,  M_SCNT),
    E2(0x0900, TEXT_SRL,  M_REG,  M_SCNT),
    E2(0x0A00, TEXT_SLA,  M_REG,  M_SCNT),
    E2(0x0B00, TEXT_SRC,  M_REG,  M_SCNT),
    E1(0x1000, TEXT_JMP,  M_REL),
    E1(0x1100, TEXT_JLT,  M_REL),
    E1(0x1200, TEXT_JLE,  M_REL),
    E1(0x1300, TEXT_JEQ,  M_REL),
    E1(0x1400, TEXT_JHE,  M_REL),
    E1(0x1500, TEXT_JGT,  M_REL),
    E1(0x1600, TEXT_JNE,  M_REL),
    E1(0x1700, TEXT_JNC,  M_REL),
    E1(0x1800, TEXT_JOC,  M_REL),
    E1(0x1900, TEXT_JNO,  M_REL),
    E1(0x1A00, TEXT_JL,   M_REL),
    E1(0x1B00, TEXT_JH,   M_REL),
    E1(0x1C00, TEXT_JOP,  M_REL),
    E1(0x1D00, TEXT_SBO,  M_CRU),
    E1(0x1E00, TEXT_SBZ,  M_CRU),
    E1(0x1F00, TEXT_TB,   M_CRU),
    E2(0x2000, TEXT_COC,  M_SRC,  M_DREG),
    E2(0x2400, TEXT_CZC,  M_SRC,  M_DREG),
    E2(0x2800, TEXT_XOR,  M_SRC,  M_DREG),
    E2(0x2C00, TEXT_XOP,  M_SRC,  M_XOP),
    E2(0x3000, TEXT_LDCR, M_SRC,  M_CNT),
    E2(0x3400, TEXT_STCR, M_SRC,  M_CNT),
    E2(0x3800, TEXT_MPY,  M_SRC,  M_DREG),
    E2(0x3C00, TEXT_DIV,  M_SRC,  M_DREG),
    E2(0x4000, TEXT_SZC,  M_SRC,  M_DST),
    E2(0x5000, TEXT_SZCB, M_SRC,  M_DST),
    E2(0x6000, TEXT_S,    M_SRC,  M_DST),
    E2(0x7000, TEXT_SB,   M_SRC,  M_DST),
    E2(0x8000, TEXT_C,    M_SRC,  M_DST),
    E2(0x9000, TEXT_CB,   M_SRC,  M_DST),
    E2(0xA000, TEXT_A,    M_SRC,  M_DST),
    E2(0xB000, TEXT_AB,   M_SRC,  M_DST),
    E2(0xC000, TEXT_MOV,  M_SRC,  M_DST),
    E2(0xD000, TEXT_MOVB, M_SRC,  M_DST),
    E2(0xE000, TEXT_SOC,  M_SRC,  M_DST),
    E2(0xF000, TEXT_SOCB, M_SRC,  M_DST),
};

static constexpr uint8_t INDEX_TMS9900[] PROGMEM = {
     63,  // TEXT_A
     64,  // TEXT_AB
     28,  // TEXT_ABS
      1,  // TEXT_AI
      2,  // TEXT_ANDI
     16,  // TEXT_B
     25,  // TEXT_BL
     15,  // TEXT_BLWP
     61,  // TEXT_C
     62,  // TEXT_CB
      4,  // TEXT_CI
     13,  // TEXT_CKOF
     12,  // TEXT_CKON
     18,  // TEXT_CLR
     49,  // TEXT_COC
     50,  // TEXT_CZC
     23,  // TEXT_DEC
     24,  // TEXT_DECT
     56,  // TEXT_DIV
      9,  // TEXT_IDLE
     21,  // TEXT_INC
     22,  // TEXT_INCT
     20,  // TEXT_INV
     36,  // TEXT_JEQ
     38,  // TEXT_JGT
     44,  // TEXT_JH
     37,  // TEXT_JHE
     43,  // TEXT_JL
     35,  // TEXT_JLE
     34,  // TEXT_JLT
     33,  // TEXT_JMP
     40,  // TEXT_JNC
     39,  // TEXT_JNE
     42,  // TEXT_JNO
     41,  // TEXT_JOC
     45,  // TEXT_JOP
     53,  // TEXT_LDCR
      0,  // TEXT_LI
      8,  // TEXT_LIMI
     14,  // TEXT_LREX
      7,  // TEXT_LWPI
     65,  // TEXT_MOV
     66,  // TEXT_MOVB
     55,  // TEXT_MPY
     19,  // TEXT_NEG
      3,  // TEXT_ORI
     10,  // TEXT_RSET
     11,  // TEXT_RTWP
     59,  // TEXT_S
     60,  // TEXT_SB
     46,  // TEXT_SBO
     47,  // TEXT_SBZ
     27,  // TEXT_SETO
     31,  // TEXT_SLA
     67,  // TEXT_SOC
     68,  // TEXT_SOCB
     29,  // TEXT_SRA
     32,  // TEXT_SRC
     30,  // TEXT_SRL
     54,  // TEXT_STCR
      6,  // TEXT_STST
      5,  // TEXT_STWP
     26,  // TEXT_SWPB
     57,  // TEXT_SZC
     58,  // TEXT_SZCB
     48,  // TEXT_TB
     17,  // TEXT_X
     52,  // TEXT_XOP
     51,  // TEXT_XOR
};

static constexpr Entry TABLE_TMS9995[] PROGMEM = {
    E1(0x0080, TEXT_LST,  M_REG),
    E1(0x0090, TEXT_LWP,  M_REG),
    E1(0x0180, TEXT_DIVS, M_SRC),
    E1(0x01C0, TEXT_MPYS, M_SRC),
};

static constexpr uint8_t INDEX_TMS9995[] PROGMEM = {
      2,  // TEXT_DIVS
      0,  // TEXT_LST
      1,  // TEXT_LWP
      3,  // TEXT_MPYS
};

static constexpr Entry TABLE_TMS99105[] PROGMEM = {
    E2(0x001C, TEXT_SRAM, M_SRC2, M_CNT2),
    E2(0x001D, TEXT_SLAM, M_SRC2, M_CNT2),
    E2(0x0029, TEXT_SM,   M_SRC2, M_DST2),
    E2(0x002A, TEXT_AM,   M_SRC2, M_DST2),
    E2(0x00B0, TEXT_BLSK, M_REG,  M_IMM),
    E1(0x0140, TEXT_BIND, M_SRC),
    E2(0x0C09, TEXT_TMB,  M_SRC2, M_BIT2),
    E2(0x0C0A, TEXT_TCMB, M_SRC2, M_BIT2),
    E2(0x0C0B, TEXT_TSMB, M_SRC2, M_BIT2),
    E1(0x0100, TEXT_EVAD, M_SRC),
    E1(0x0380, TEXT_RTWP, M_RTWP),
};

static constexpr uint8_t INDEX_TMS99105[] PROGMEM = {
      3,  // TEXT_AM
      5,  // TEXT_BIND
      4,  // TEXT_BLSK
      9,  // TEXT_EVAD
     10,  // TEXT_RTWP
      1,  // TEXT_SLAM
      2,  // TEXT_SM
      0,  // TEXT_SRAM
      7,  // TEXT_TCMB
      6,  // TEXT_TMB
      8,  // TEXT_TSMB
};
// clang-format on

using EntryPage = entry::TableBase<Entry>;

static constexpr EntryPage TMS9900_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_TMS9900), ARRAY_RANGE(INDEX_TMS9900)},
};

static constexpr EntryPage TMS9995_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_TMS9900), ARRAY_RANGE(INDEX_TMS9900)},
        {ARRAY_RANGE(TABLE_TMS9995), ARRAY_RANGE(INDEX_TMS9995)},
};

static constexpr EntryPage TMS99105_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_TMS99105), ARRAY_RANGE(INDEX_TMS99105)},
        {ARRAY_RANGE(TABLE_TMS9900), ARRAY_RANGE(INDEX_TMS9900)},
        {ARRAY_RANGE(TABLE_TMS9995), ARRAY_RANGE(INDEX_TMS9995)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

static constexpr Cpu CPU_TABLE[] PROGMEM = {
        {TMS9900, TEXT_CPU_9900, ARRAY_RANGE(TMS9900_PAGES)},
        {TMS9995, TEXT_CPU_9995, ARRAY_RANGE(TMS9995_PAGES)},
        {TMS99105, TEXT_CPU_99105, ARRAY_RANGE(TMS99105_PAGES)},
};

static const Cpu *cpu(CpuType cpuType) {
    return Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
}

static bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_IREG || opr == M_INCR || opr == M_SYBL || opr == M_INDX)
        return table == M_SRC || table == M_DST || table == M_SRC2 || table == M_DST2;
    if (opr == M_REG)
        return table == M_SRC || table == M_DST || table == M_SRC2 || table == M_DST2 ||
               table == M_DREG || table == M_SCNT || table == M_CNT2;
    if (opr == M_IMM)
        return table == M_REL || table == M_SCNT || table == M_CNT || table == M_CNT2 ||
               table == M_CRU || table == M_BIT2 || table == M_XOP || table == M_RTWP;
    return false;
}

static bool acceptModes(AsmInsn &insn, const Entry *entry) {
    auto flags = insn.flags();
    auto table = entry->flags();
    return acceptMode(flags.src(), table.src()) && acceptMode(flags.dst(), table.dst());
}

Error TableTms9900::searchName(CpuType cpuType, AsmInsn &insn) const {
    cpu(cpuType)->searchName(insn, acceptModes);
    return insn.getError();
}

static bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *page) {
    auto opCode = insn.opCode();
    const auto flags = entry->flags();
    const auto src = flags.src();
    const auto dst = flags.dst();
    if (src == M_REG || dst == M_REG) {
        opCode &= ~0xF;
    } else if (src == M_REL || src == M_CRU) {
        opCode &= ~0xFF;
    }
    if (src == M_SRC)
        opCode &= ~0x3F;
    if (dst == M_DST)
        opCode &= ~0xFC0;
    if (dst == M_CNT || dst == M_XOP || dst == M_DREG)
        opCode &= ~0x3C0;
    if (dst == M_SCNT)
        opCode &= ~0xF0;
    if (src == M_RTWP)
        opCode &= ~7;
    return opCode == entry->opCode();
}

Error TableTms9900::searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) const {
    cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    return insn.getError();
}

const /*PROGMEM*/ char *TableTms9900::listCpu_P() const {
    return TEXT_CPU_LIST;
}

const /*PROGMEM*/ char *TableTms9900::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableTms9900::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    if (name.istarts_P(TEXT_CPU_LIST, 3))
        name += 3;
    auto t = Cpu::search(name, ARRAY_RANGE(CPU_TABLE));
    if (t) {
        cpuType = t->cpuType();
        return OK;
    }
    return UNSUPPORTED_CPU;
}

const TableTms9900 TABLE;

}  // namespace tms9900
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

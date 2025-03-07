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

#define E2(_opc, _name, _src, _dst) {_opc, Entry::Flags::create(_src, _dst), _name}
#define B2(_opc, _name, _src, _dst) {_opc, Entry::Flags::create(_src, _dst, true), _name}
#define E1(_opc, _name, _src) E2(_opc, _name, _src, M_NONE)
#define E0(_opc, _name) E1(_opc, _name, M_NONE)

// clang-format off
constexpr Entry TABLE_TMS9900[] PROGMEM = {
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
    E0(0x045B, TEXT_RT),
    E0(0x1000, TEXT_NOP),
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
    B2(0x5000, TEXT_SZCB, M_SRC,  M_DST),
    E2(0x6000, TEXT_S,    M_SRC,  M_DST),
    B2(0x7000, TEXT_SB,   M_SRC,  M_DST),
    E2(0x8000, TEXT_C,    M_SRC,  M_DST),
    B2(0x9000, TEXT_CB,   M_SRC,  M_DST),
    E2(0xA000, TEXT_A,    M_SRC,  M_DST),
    B2(0xB000, TEXT_AB,   M_SRC,  M_DST),
    E2(0xC000, TEXT_MOV,  M_SRC,  M_DST),
    B2(0xD000, TEXT_MOVB, M_SRC,  M_DST),
    E2(0xE000, TEXT_SOC,  M_SRC,  M_DST),
    B2(0xF000, TEXT_SOCB, M_SRC,  M_DST),
};

constexpr uint8_t INDEX_TMS9900[] PROGMEM = {
     65,  // TEXT_A
     66,  // TEXT_AB
     30,  // TEXT_ABS
      1,  // TEXT_AI
      2,  // TEXT_ANDI
     18,  // TEXT_B
     27,  // TEXT_BL
     17,  // TEXT_BLWP
     63,  // TEXT_C
     64,  // TEXT_CB
      4,  // TEXT_CI
     13,  // TEXT_CKOF
     12,  // TEXT_CKON
     20,  // TEXT_CLR
     51,  // TEXT_COC
     52,  // TEXT_CZC
     25,  // TEXT_DEC
     26,  // TEXT_DECT
     58,  // TEXT_DIV
      9,  // TEXT_IDLE
     23,  // TEXT_INC
     24,  // TEXT_INCT
     22,  // TEXT_INV
     38,  // TEXT_JEQ
     40,  // TEXT_JGT
     46,  // TEXT_JH
     39,  // TEXT_JHE
     45,  // TEXT_JL
     37,  // TEXT_JLE
     36,  // TEXT_JLT
     35,  // TEXT_JMP
     42,  // TEXT_JNC
     41,  // TEXT_JNE
     44,  // TEXT_JNO
     43,  // TEXT_JOC
     47,  // TEXT_JOP
     55,  // TEXT_LDCR
      0,  // TEXT_LI
      8,  // TEXT_LIMI
     14,  // TEXT_LREX
      7,  // TEXT_LWPI
     67,  // TEXT_MOV
     68,  // TEXT_MOVB
     57,  // TEXT_MPY
     21,  // TEXT_NEG
     16,  // TEXT_NOP
      3,  // TEXT_ORI
     10,  // TEXT_RSET
     15,  // TEXT_RT
     11,  // TEXT_RTWP
     61,  // TEXT_S
     62,  // TEXT_SB
     48,  // TEXT_SBO
     49,  // TEXT_SBZ
     29,  // TEXT_SETO
     33,  // TEXT_SLA
     69,  // TEXT_SOC
     70,  // TEXT_SOCB
     31,  // TEXT_SRA
     34,  // TEXT_SRC
     32,  // TEXT_SRL
     56,  // TEXT_STCR
      6,  // TEXT_STST
      5,  // TEXT_STWP
     28,  // TEXT_SWPB
     59,  // TEXT_SZC
     60,  // TEXT_SZCB
     50,  // TEXT_TB
     19,  // TEXT_X
     54,  // TEXT_XOP
     53,  // TEXT_XOR
};

constexpr Entry TABLE_TMS9995[] PROGMEM = {
    E1(0x0080, TEXT_LST,  M_REG),
    E1(0x0090, TEXT_LWP,  M_REG),
    E1(0x0180, TEXT_DIVS, M_SRC),
    E1(0x01C0, TEXT_MPYS, M_SRC),
};

constexpr uint8_t INDEX_TMS9995[] PROGMEM = {
      2,  // TEXT_DIVS
      0,  // TEXT_LST
      1,  // TEXT_LWP
      3,  // TEXT_MPYS
};

constexpr Entry TABLE_TMS99105[] PROGMEM = {
    E2(0x001C, TEXT_SRAM, M_SRC2, M_CNT4),
    E2(0x001D, TEXT_SLAM, M_SRC2, M_CNT4),
    E2(0x0029, TEXT_SM,   M_SRC2, M_DST4),
    E2(0x002A, TEXT_AM,   M_SRC2, M_DST4),
    E2(0x00B0, TEXT_BLSK, M_REG,  M_IMM),
    E2(0x00B0, TEXT_BLSK, M_REG,  M_SYBL),
    E1(0x0140, TEXT_BIND, M_SRC),
    E2(0x0C09, TEXT_TMB,  M_SRC2, M_BIT0),
    E2(0x0C0A, TEXT_TCMB, M_SRC2, M_BIT0),
    E2(0x0C0B, TEXT_TSMB, M_SRC2, M_BIT0),
    E1(0x0100, TEXT_EVAD, M_SRC),
    E1(0x0380, TEXT_RTWP, M_RTWP),
    // Extra
    E1(0x0381, TEXT_RTWP, M_RTWP),
    E1(0x0382, TEXT_RTWP, M_RTWP),
    E1(0x0384, TEXT_RTWP, M_RTWP),
};

constexpr uint8_t INDEX_TMS99105[] PROGMEM = {
      3,  // TEXT_AM
      6,  // TEXT_BIND
      4,  // TEXT_BLSK
      5,  // TEXT_BLSK
     10,  // TEXT_EVAD
     11,  // TEXT_RTWP
      1,  // TEXT_SLAM
      2,  // TEXT_SM
      0,  // TEXT_SRAM
      8,  // TEXT_TCMB
      7,  // TEXT_TMB
      9,  // TEXT_TSMB
};

constexpr Entry TABLE_TMS99110[] PROGMEM = {
    E0(0x0780, TEXT_LDS),
    E0(0x07C0, TEXT_LDD),
    E0(0x0C00, TEXT_CRI),
    E0(0x0C02, TEXT_NEGR),
    E0(0x0C04, TEXT_CRE),
    E0(0x0C06, TEXT_CER),
    E1(0x0C40, TEXT_AR,  M_SRC),
    E1(0x0C80, TEXT_CIR, M_SRC),
    E1(0x0CC0, TEXT_SR,  M_SRC),
    E1(0x0D00, TEXT_MR,  M_SRC),
    E1(0x0D40, TEXT_DR,  M_SRC),
    E1(0x0D80, TEXT_LR,  M_SRC),
    E1(0x0DC0, TEXT_STR, M_SRC),
    E2(0x0301, TEXT_CR,  M_SRC2, M_DST0),
    E2(0x0302, TEXT_MM,  M_SRC2, M_DST0),
};

constexpr uint8_t INDEX_TMS99110[] PROGMEM = {
      6,  // TEXT_AR
      5,  // TEXT_CER
      7,  // TEXT_CIR
     13,  // TEXT_CR
      4,  // TEXT_CRE
      2,  // TEXT_CRI
     10,  // TEXT_DR
      1,  // TEXT_LDD
      0,  // TEXT_LDS
     11,  // TEXT_LR
     14,  // TEXT_MM
      9,  // TEXT_MR
      3,  // TEXT_NEGR
      8,  // TEXT_SR
     12,  // TEXT_STR
};

// clang-format on

using EntryPage = entry::TableBase<Entry>;

constexpr EntryPage TMS9900_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_TMS9900), ARRAY_RANGE(INDEX_TMS9900)},
};

constexpr EntryPage TMS9995_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_TMS9900), ARRAY_RANGE(INDEX_TMS9900)},
        {ARRAY_RANGE(TABLE_TMS9995), ARRAY_RANGE(INDEX_TMS9995)},
};

constexpr EntryPage TMS99105_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_TMS99105), ARRAY_RANGE(INDEX_TMS99105)},
        {ARRAY_RANGE(TABLE_TMS9900), ARRAY_RANGE(INDEX_TMS9900)},
        {ARRAY_RANGE(TABLE_TMS9995), ARRAY_RANGE(INDEX_TMS9995)},
};

constexpr EntryPage TMS99110_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_TMS99105), ARRAY_RANGE(INDEX_TMS99105)},
        {ARRAY_RANGE(TABLE_TMS9900), ARRAY_RANGE(INDEX_TMS9900)},
        {ARRAY_RANGE(TABLE_TMS9995), ARRAY_RANGE(INDEX_TMS9995)},
        {ARRAY_RANGE(TABLE_TMS99110), ARRAY_RANGE(INDEX_TMS99110)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {TMS9900, TEXT_CPU_9900, ARRAY_RANGE(TMS9900_PAGES)},
        {TMS9980, TEXT_CPU_9980, ARRAY_RANGE(TMS9900_PAGES)},
        {TMS9995, TEXT_CPU_9995, ARRAY_RANGE(TMS9995_PAGES)},
        {TMS99105, TEXT_CPU_99105, ARRAY_RANGE(TMS99105_PAGES)},
        {TMS99110, TEXT_CPU_99110, ARRAY_RANGE(TMS99110_PAGES)},
};

const Cpu *cpu(CpuType cpuType) {
    return Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
}

bool hasOperand(CpuType cpuType, AsmInsn &insn) {
    cpu(cpuType)->searchName(insn, Cpu::acceptAll<AsmInsn, Entry>);
    return insn.isOK() && insn.src() != M_NONE;
}

bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_IREG || opr == M_INCR || opr == M_SYBL || opr == M_INDX)
        return table == M_SRC || table == M_DST || table == M_SRC2 || table == M_DST0 ||
               table == M_DST4;
    if (opr == M_REG)
        return table == M_SRC || table == M_DST || table == M_SRC2 || table == M_DST0 ||
               table == M_DST4 || table == M_DREG || table == M_SCNT || table == M_CNT4;
    if (opr == M_IMM)
        return table == M_REL || table == M_SCNT || table == M_CNT || table == M_CNT4 ||
               table == M_CRU || table == M_BIT0 || table == M_XOP || table == M_RTWP;
    return false;
}

bool acceptModes(AsmInsn &insn, const Entry *entry) {
    const auto table = entry->readFlags();
    return acceptMode(insn.srcOp.mode, table.src()) && acceptMode(insn.dstOp.mode, table.dst());
}

Error searchName(CpuType cpuType, AsmInsn &insn) {
    cpu(cpuType)->searchName(insn, acceptModes);
    return insn.getError();
}

bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *) {
    auto opCode = insn.opCode();
    const auto flags = entry->readFlags();
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
    return opCode == entry->readOpCode();
}

Error searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) {
    cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    return insn.getError();
}

const /*PROGMEM*/ char *TableTms9900::listCpu_P() const {
    return TEXT_TMS9900_LIST;
}

const /*PROGMEM*/ char *TableTms9900::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableTms9900::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    name.iexpectText_P(TEXT_TMS9900_LIST, 3);
    auto t = Cpu::search(name, ARRAY_RANGE(CPU_TABLE));
    if (t) {
        cpuType = t->readCpuType();
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

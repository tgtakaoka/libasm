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

#define E2(_opc, _cf, _name, _src, _dst) {_opc, Entry::Flags::create(_cf, _src, _dst), _name}
#define B2(_opc, _cf, _name, _src, _dst) {_opc, Entry::Flags::create(_cf, _src, _dst, true), _name}
#define E1(_opc, _cf, _name, _src) E2(_opc, _cf, _name, _src, M_NONE)
#define E0(_opc, _cf, _name) E1(_opc, _cf, _name, M_NONE)

// clang-format off
constexpr Entry TABLE_TMS9900[] PROGMEM = {
    E2(0x0200, CF_000F, TEXT_LI,   M_REG,  M_IMM),
    E2(0x0220, CF_000F, TEXT_AI,   M_REG,  M_IMM),
    E2(0x0240, CF_000F, TEXT_ANDI, M_REG,  M_IMM),
    E2(0x0260, CF_000F, TEXT_ORI,  M_REG,  M_IMM),
    E2(0x0280, CF_000F, TEXT_CI,   M_REG,  M_IMM),
    E1(0x02A0, CF_000F, TEXT_STWP, M_REG),
    E1(0x02C0, CF_000F, TEXT_STST, M_REG),
    E1(0x02E0, CF_0000, TEXT_LWPI, M_IMM),
    E1(0x0300, CF_0000, TEXT_LIMI, M_IMM),
    E0(0x0340, CF_0000, TEXT_IDLE),
    E0(0x0360, CF_0000, TEXT_RSET),
    E0(0x0380, CF_0000, TEXT_RTWP),
    E0(0x03A0, CF_0000, TEXT_CKON),
    E0(0x03C0, CF_0000, TEXT_CKOF),
    E0(0x03E0, CF_0000, TEXT_LREX),
    E1(0x0400, CF_003F, TEXT_BLWP, M_SRC),
    E1(0x0440, CF_003F, TEXT_B,    M_SRC),
    E1(0x0480, CF_003F, TEXT_X,    M_SRC),
    E1(0x04C0, CF_003F, TEXT_CLR,  M_SRC),
    E1(0x0500, CF_003F, TEXT_NEG,  M_SRC),
    E1(0x0540, CF_003F, TEXT_INV,  M_SRC),
    E1(0x0580, CF_003F, TEXT_INC,  M_SRC),
    E1(0x05C0, CF_003F, TEXT_INCT, M_SRC),
    E1(0x0600, CF_003F, TEXT_DEC,  M_SRC),
    E1(0x0640, CF_003F, TEXT_DECT, M_SRC),
    E1(0x0680, CF_003F, TEXT_BL,   M_SRC),
    E1(0x06C0, CF_003F, TEXT_SWPB, M_SRC),
    E1(0x0700, CF_003F, TEXT_SETO, M_SRC),
    E1(0x0740, CF_003F, TEXT_ABS,  M_SRC),
    E2(0x0800, CF_00FF, TEXT_SRA,  M_REG,  M_SCNT),
    E2(0x0900, CF_00FF, TEXT_SRL,  M_REG,  M_SCNT),
    E2(0x0A00, CF_00FF, TEXT_SLA,  M_REG,  M_SCNT),
    E2(0x0B00, CF_00FF, TEXT_SRC,  M_REG,  M_SCNT),
    E1(0x1000, CF_00FF, TEXT_JMP,  M_REL),
    E1(0x1100, CF_00FF, TEXT_JLT,  M_REL),
    E1(0x1200, CF_00FF, TEXT_JLE,  M_REL),
    E1(0x1300, CF_00FF, TEXT_JEQ,  M_REL),
    E1(0x1400, CF_00FF, TEXT_JHE,  M_REL),
    E1(0x1500, CF_00FF, TEXT_JGT,  M_REL),
    E1(0x1600, CF_00FF, TEXT_JNE,  M_REL),
    E1(0x1700, CF_00FF, TEXT_JNC,  M_REL),
    E1(0x1800, CF_00FF, TEXT_JOC,  M_REL),
    E1(0x1900, CF_00FF, TEXT_JNO,  M_REL),
    E1(0x1A00, CF_00FF, TEXT_JL,   M_REL),
    E1(0x1B00, CF_00FF, TEXT_JH,   M_REL),
    E1(0x1C00, CF_00FF, TEXT_JOP,  M_REL),
    E1(0x1D00, CF_00FF, TEXT_SBO,  M_CRU),
    E1(0x1E00, CF_00FF, TEXT_SBZ,  M_CRU),
    E1(0x1F00, CF_00FF, TEXT_TB,   M_CRU),
    E2(0x2000, CF_03FF, TEXT_COC,  M_SRC,  M_DREG),
    E2(0x2400, CF_03FF, TEXT_CZC,  M_SRC,  M_DREG),
    E2(0x2800, CF_03FF, TEXT_XOR,  M_SRC,  M_DREG),
    E2(0x2C00, CF_03FF, TEXT_XOP,  M_SRC,  M_XOP),
    E2(0x3000, CF_03FF, TEXT_LDCR, M_SRC,  M_CNT),
    E2(0x3400, CF_03FF, TEXT_STCR, M_SRC,  M_CNT),
    E2(0x3800, CF_03FF, TEXT_MPY,  M_SRC,  M_DREG),
    E2(0x3C00, CF_03FF, TEXT_DIV,  M_SRC,  M_DREG),
    E2(0x4000, CF_0FFF, TEXT_SZC,  M_SRC,  M_DST),
    B2(0x5000, CF_0FFF, TEXT_SZCB, M_SRC,  M_DST),
    E2(0x6000, CF_0FFF, TEXT_S,    M_SRC,  M_DST),
    B2(0x7000, CF_0FFF, TEXT_SB,   M_SRC,  M_DST),
    E2(0x8000, CF_0FFF, TEXT_C,    M_SRC,  M_DST),
    B2(0x9000, CF_0FFF, TEXT_CB,   M_SRC,  M_DST),
    E2(0xA000, CF_0FFF, TEXT_A,    M_SRC,  M_DST),
    B2(0xB000, CF_0FFF, TEXT_AB,   M_SRC,  M_DST),
    E2(0xC000, CF_0FFF, TEXT_MOV,  M_SRC,  M_DST),
    B2(0xD000, CF_0FFF, TEXT_MOVB, M_SRC,  M_DST),
    E2(0xE000, CF_0FFF, TEXT_SOC,  M_SRC,  M_DST),
    B2(0xF000, CF_0FFF, TEXT_SOCB, M_SRC,  M_DST),
    E0(0x045B, CF_0000, TEXT_RT),
    E0(0x1000, CF_0000, TEXT_NOP),
};

constexpr uint8_t INDEX_TMS9900[] PROGMEM = {
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
     70,  // TEXT_NOP
      3,  // TEXT_ORI
     10,  // TEXT_RSET
     69,  // TEXT_RT
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

constexpr Entry TABLE_TMS9995[] PROGMEM = {
    E1(0x0080, CF_000F, TEXT_LST,  M_REG),
    E1(0x0090, CF_000F, TEXT_LWP,  M_REG),
    E1(0x0180, CF_003F, TEXT_DIVS, M_SRC),
    E1(0x01C0, CF_003F, TEXT_MPYS, M_SRC),
};

constexpr uint8_t INDEX_TMS9995[] PROGMEM = {
      2,  // TEXT_DIVS
      0,  // TEXT_LST
      1,  // TEXT_LWP
      3,  // TEXT_MPYS
};

constexpr Entry TABLE_TMS99105[] PROGMEM = {
    E2(0x001C, CF_0000, TEXT_SRAM, M_SRC2, M_CNT4),
    E2(0x001D, CF_0000, TEXT_SLAM, M_SRC2, M_CNT4),
    E2(0x0029, CF_0000, TEXT_SM,   M_SRC2, M_DST4),
    E2(0x002A, CF_0000, TEXT_AM,   M_SRC2, M_DST4),
    E2(0x00B0, CF_000F, TEXT_BLSK, M_REG,  M_IMM),
    E2(0x00B0, CF_000F, TEXT_BLSK, M_REG,  M_SYBL),
    E1(0x0140, CF_003F, TEXT_BIND, M_SRC),
    E2(0x0C09, CF_0000, TEXT_TMB,  M_SRC2, M_BIT0),
    E2(0x0C0A, CF_0000, TEXT_TCMB, M_SRC2, M_BIT0),
    E2(0x0C0B, CF_0000, TEXT_TSMB, M_SRC2, M_BIT0),
    E1(0x0100, CF_003F, TEXT_EVAD, M_SRC),
    E1(0x0380, CF_0000, TEXT_RTWP, M_RTWP),
    // Extra
    E1(0x0381, CF_0000, TEXT_RTWP, M_RTWP),
    E1(0x0382, CF_0000, TEXT_RTWP, M_RTWP),
    E1(0x0384, CF_0000, TEXT_RTWP, M_RTWP),
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
    E0(0x0780, CF_0000, TEXT_LDS),
    E0(0x07C0, CF_0000, TEXT_LDD),
    E0(0x0C00, CF_0000, TEXT_CRI),
    E0(0x0C02, CF_0000, TEXT_NEGR),
    E0(0x0C04, CF_0000, TEXT_CRE),
    E0(0x0C06, CF_0000, TEXT_CER),
    E1(0x0C40, CF_003F, TEXT_AR,  M_SRC),
    E1(0x0C80, CF_003F, TEXT_CIR, M_SRC),
    E1(0x0CC0, CF_003F, TEXT_SR,  M_SRC),
    E1(0x0D00, CF_003F, TEXT_MR,  M_SRC),
    E1(0x0D40, CF_003F, TEXT_DR,  M_SRC),
    E1(0x0D80, CF_003F, TEXT_LR,  M_SRC),
    E1(0x0DC0, CF_003F, TEXT_STR, M_SRC),
    E2(0x0301, CF_0000, TEXT_CR,  M_SRC2, M_DST0),
    E2(0x0302, CF_0000, TEXT_MM,  M_SRC2, M_DST0),
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
    auto opc = insn.opCode();
    opc &= ~entry->readFlags().mask();
    return opc == entry->readOpCode();
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

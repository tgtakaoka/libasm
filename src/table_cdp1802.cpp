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

#include "table_cdp1802.h"

#include "entry_cdp1802.h"
#include "entry_table.h"
#include "text_cdp1802.h"

using namespace libasm::text::cdp1802;

namespace libasm {
namespace cdp1802 {

#define E2(_opc, _name, _op1, _op2) \
    { _opc, Entry::Flags::create(_op1, _op2), _name }
#define E1(_opc, _name, _op1) E2(_opc, _name, _op1, M_NONE)
#define E0(_opc, _name) E1(_opc, _name, M_NONE)
#define U0(_opc, _name) \
    { _opc, Entry::Flags::undef(), _name }

// clang-format off
static constexpr Entry TABLE_CDP1802[] PROGMEM = {
    E0(0x00, TEXT_IDL),
    E1(0x00, TEXT_LDN,  M_REG1),
    E1(0x10, TEXT_INC,  M_REGN),
    E1(0x20, TEXT_DEC,  M_REGN),
    E1(0x30, TEXT_BR,   M_PAGE),
    E1(0x31, TEXT_BQ,   M_PAGE),
    E1(0x32, TEXT_BZ,   M_PAGE),
    E1(0x33, TEXT_BDF,  M_PAGE),
    E1(0x33, TEXT_BPZ,  M_PAGE),
    E1(0x33, TEXT_BGE,  M_PAGE),
    E1(0x34, TEXT_B1,   M_PAGE),
    E1(0x35, TEXT_B2,   M_PAGE),
    E1(0x36, TEXT_B3,   M_PAGE),
    E1(0x37, TEXT_B4,   M_PAGE),
    E0(0x38, TEXT_SKP),
    E1(0x38, TEXT_NBR,  M_PAGE),
    E1(0x39, TEXT_BNQ,  M_PAGE),
    E1(0x3A, TEXT_BNZ,  M_PAGE),
    E1(0x3B, TEXT_BNF,  M_PAGE),
    E1(0x3B, TEXT_BM,   M_PAGE),
    E1(0x3B, TEXT_BL,   M_PAGE),
    E1(0x3C, TEXT_BN1,  M_PAGE),
    E1(0x3D, TEXT_BN2,  M_PAGE),
    E1(0x3E, TEXT_BN3,  M_PAGE),
    E1(0x3F, TEXT_BN4,  M_PAGE),
    E1(0x40, TEXT_LDA,  M_REGN),
    E1(0x50, TEXT_STR,  M_REGN),
    E0(0x60, TEXT_IRX),
    E1(0x60, TEXT_OUT,  M_IOAD),
    U0(0x68, TEXT_null),   // undefined
    E1(0x68, TEXT_INP,  M_IOAD),
    E0(0x70, TEXT_RET),
    E0(0x71, TEXT_DIS),
    E0(0x72, TEXT_LDXA),
    E0(0x73, TEXT_STXD),
    E0(0x74, TEXT_ADC),
    E0(0x75, TEXT_SDB),
    E0(0x76, TEXT_SHRC),
    E0(0x76, TEXT_RSHR),
    E0(0x77, TEXT_SMB),
    E0(0x78, TEXT_SAV),
    E0(0x79, TEXT_MARK),
    E0(0x7A, TEXT_REQ),
    E0(0x7B, TEXT_SEQ),
    E1(0x7C, TEXT_ADCI, M_IMM8),
    E1(0x7D, TEXT_SDBI, M_IMM8),
    E0(0x7E, TEXT_SHLC),
    E0(0x7E, TEXT_RSHL),
    E1(0x7F, TEXT_SMBI, M_IMM8),
    E1(0x80, TEXT_GLO,  M_REGN),
    E1(0x90, TEXT_GHI,  M_REGN),
    E1(0xA0, TEXT_PLO,  M_REGN),
    E1(0xB0, TEXT_PHI,  M_REGN),
    E1(0xC0, TEXT_LBR,  M_ADDR),
    E1(0xC1, TEXT_LBQ,  M_ADDR),
    E1(0xC2, TEXT_LBZ,  M_ADDR),
    E1(0xC3, TEXT_LBDF, M_ADDR),
    E1(0xC3, TEXT_LBPZ, M_ADDR),
    E1(0xC3, TEXT_LBGE, M_ADDR),
    E0(0xC4, TEXT_NOP),
    E0(0xC5, TEXT_LSNQ),
    E0(0xC6, TEXT_LSNZ),
    E0(0xC7, TEXT_LSNF),
    E0(0xC8, TEXT_LSKP),
    E1(0xC8, TEXT_NLBR, M_ADDR),
    E1(0xC9, TEXT_LBNQ, M_ADDR),
    E1(0xCA, TEXT_LBNZ, M_ADDR),
    E1(0xCB, TEXT_LBNF, M_ADDR),
    E1(0xCB, TEXT_LBM,  M_ADDR),
    E1(0xCB, TEXT_LBL,  M_ADDR),
    E0(0xCC, TEXT_LSIE),
    E0(0xCD, TEXT_LSQ),
    E0(0xCE, TEXT_LSZ),
    E0(0xCF, TEXT_LSDF),
    E1(0xD0, TEXT_SEP,  M_REGN),
    E1(0xE0, TEXT_SEX,  M_REGN),
    E0(0xF0, TEXT_LDX),
    E0(0xF1, TEXT_OR),
    E0(0xF2, TEXT_AND),
    E0(0xF3, TEXT_XOR),
    E0(0xF4, TEXT_ADD),
    E0(0xF5, TEXT_SD),
    E0(0xF6, TEXT_SHR),
    E0(0xF7, TEXT_SM),
    E1(0xF8, TEXT_LDI,  M_IMM8),
    E1(0xF9, TEXT_ORI,  M_IMM8),
    E1(0xFA, TEXT_ANI,  M_IMM8),
    E1(0xFB, TEXT_XRI,  M_IMM8),
    E1(0xFC, TEXT_ADI,  M_IMM8),
    E1(0xFD, TEXT_SDI,  M_IMM8),
    E0(0xFE, TEXT_SHL),
    E1(0xFF, TEXT_SMI,  M_IMM8),
};

static constexpr uint8_t INDEX_CDP1802[] PROGMEM = {
     29,  // TEXT_null
     35,  // TEXT_ADC
     44,  // TEXT_ADCI
     80,  // TEXT_ADD
     88,  // TEXT_ADI
     78,  // TEXT_AND
     86,  // TEXT_ANI
     10,  // TEXT_B1
     11,  // TEXT_B2
     12,  // TEXT_B3
     13,  // TEXT_B4
      7,  // TEXT_BDF
      9,  // TEXT_BGE
     20,  // TEXT_BL
     19,  // TEXT_BM
     21,  // TEXT_BN1
     22,  // TEXT_BN2
     23,  // TEXT_BN3
     24,  // TEXT_BN4
     18,  // TEXT_BNF
     16,  // TEXT_BNQ
     17,  // TEXT_BNZ
      8,  // TEXT_BPZ
      5,  // TEXT_BQ
      4,  // TEXT_BR
      6,  // TEXT_BZ
      3,  // TEXT_DEC
     32,  // TEXT_DIS
     50,  // TEXT_GHI
     49,  // TEXT_GLO
      0,  // TEXT_IDL
      2,  // TEXT_INC
     30,  // TEXT_INP
     27,  // TEXT_IRX
     56,  // TEXT_LBDF
     58,  // TEXT_LBGE
     69,  // TEXT_LBL
     68,  // TEXT_LBM
     67,  // TEXT_LBNF
     65,  // TEXT_LBNQ
     66,  // TEXT_LBNZ
     57,  // TEXT_LBPZ
     54,  // TEXT_LBQ
     53,  // TEXT_LBR
     55,  // TEXT_LBZ
     25,  // TEXT_LDA
     84,  // TEXT_LDI
      1,  // TEXT_LDN
     76,  // TEXT_LDX
     33,  // TEXT_LDXA
     73,  // TEXT_LSDF
     70,  // TEXT_LSIE
     63,  // TEXT_LSKP
     62,  // TEXT_LSNF
     60,  // TEXT_LSNQ
     61,  // TEXT_LSNZ
     71,  // TEXT_LSQ
     72,  // TEXT_LSZ
     41,  // TEXT_MARK
     15,  // TEXT_NBR
     64,  // TEXT_NLBR
     59,  // TEXT_NOP
     77,  // TEXT_OR
     85,  // TEXT_ORI
     28,  // TEXT_OUT
     52,  // TEXT_PHI
     51,  // TEXT_PLO
     42,  // TEXT_REQ
     31,  // TEXT_RET
     47,  // TEXT_RSHL
     38,  // TEXT_RSHR
     40,  // TEXT_SAV
     81,  // TEXT_SD
     36,  // TEXT_SDB
     45,  // TEXT_SDBI
     89,  // TEXT_SDI
     74,  // TEXT_SEP
     43,  // TEXT_SEQ
     75,  // TEXT_SEX
     90,  // TEXT_SHL
     46,  // TEXT_SHLC
     82,  // TEXT_SHR
     37,  // TEXT_SHRC
     14,  // TEXT_SKP
     83,  // TEXT_SM
     39,  // TEXT_SMB
     48,  // TEXT_SMBI
     91,  // TEXT_SMI
     26,  // TEXT_STR
     34,  // TEXT_STXD
     79,  // TEXT_XOR
     87,  // TEXT_XRI
};

static constexpr Entry TABLE_CDP1804[] PROGMEM = {
    E0(0x00, TEXT_STPC),
    E0(0x01, TEXT_DTC),
    E0(0x02, TEXT_SPM2),
    E0(0x03, TEXT_SCM2),
    E0(0x04, TEXT_SPM1),
    E0(0x05, TEXT_SCM1),
    E0(0x06, TEXT_LDC),
    E0(0x07, TEXT_STM),
    E0(0x08, TEXT_GEC),
    E0(0x09, TEXT_ETQ),
    E0(0x0A, TEXT_XIE),
    E0(0x0B, TEXT_XID),
    E0(0x0C, TEXT_CIE),
    E0(0x0D, TEXT_CID),
    E1(0x3E, TEXT_BCI,  M_PAGE),
    E1(0x3F, TEXT_BXI,  M_PAGE),
    E1(0x60, TEXT_RLXA, M_REGN),
    E2(0x80, TEXT_SCAL, M_REGN, M_ADDR),
    E1(0x90, TEXT_SRET, M_REGN),
    E1(0xA0, TEXT_RSXD, M_REGN),
    E1(0xB0, TEXT_RNX,  M_REGN),
    E2(0xC0, TEXT_RLDI, M_REGN, M_ADDR),
};

static constexpr uint8_t INDEX_CDP1804[] PROGMEM = {
     14,  // TEXT_BCI
     15,  // TEXT_BXI
     13,  // TEXT_CID
     12,  // TEXT_CIE
      1,  // TEXT_DTC
      9,  // TEXT_ETQ
      8,  // TEXT_GEC
      6,  // TEXT_LDC
     21,  // TEXT_RLDI
     16,  // TEXT_RLXA
     20,  // TEXT_RNX
     19,  // TEXT_RSXD
     17,  // TEXT_SCAL
      5,  // TEXT_SCM1
      3,  // TEXT_SCM2
      4,  // TEXT_SPM1
      2,  // TEXT_SPM2
     18,  // TEXT_SRET
      7,  // TEXT_STM
      0,  // TEXT_STPC
     11,  // TEXT_XID
     10,  // TEXT_XIE
};

static constexpr Entry TABLE_CDP1804A[] PROGMEM = {
    E2(0x20, TEXT_DBNZ, M_REGN, M_ADDR),
    E0(0x74, TEXT_DADC),
    E0(0x76, TEXT_DSAV),
    E0(0x77, TEXT_DSMB),
    E1(0x7C, TEXT_DACI, M_IMM8),
    E1(0x7F, TEXT_DSBI, M_IMM8),
    E0(0xF4, TEXT_DADD),
    E0(0xF7, TEXT_DSM),
    E1(0xFC, TEXT_DADI, M_IMM8),
    E1(0xFF, TEXT_DSMI, M_IMM8),
};

static constexpr uint8_t INDEX_CDP1804A[] PROGMEM = {
      4,  // TEXT_DACI
      1,  // TEXT_DADC
      6,  // TEXT_DADD
      8,  // TEXT_DADI
      0,  // TEXT_DBNZ
      2,  // TEXT_DSAV
      5,  // TEXT_DSBI
      7,  // TEXT_DSM
      3,  // TEXT_DSMB
      9,  // TEXT_DSMI
};
// clang-format on

using EntryPage = entry::TableBase<Entry>;

static constexpr EntryPage CDP1802_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(TABLE_CDP1802), ARRAY_RANGE(INDEX_CDP1802)},
};

static constexpr EntryPage CDP1804_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(TABLE_CDP1802), ARRAY_RANGE(INDEX_CDP1802)},
        {0x68, ARRAY_RANGE(TABLE_CDP1804), ARRAY_RANGE(INDEX_CDP1804)},
};

static constexpr EntryPage CDP1804A_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(TABLE_CDP1802), ARRAY_RANGE(INDEX_CDP1802)},
        {0x68, ARRAY_RANGE(TABLE_CDP1804), ARRAY_RANGE(INDEX_CDP1804)},
        {0x68, ARRAY_RANGE(TABLE_CDP1804A), ARRAY_RANGE(INDEX_CDP1804A)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

static constexpr Cpu CPU_TABLE[] PROGMEM = {
        {CDP1802, TEXT_CPU_1802, ARRAY_RANGE(CDP1802_PAGES)},
        {CDP1804, TEXT_CPU_1804, ARRAY_RANGE(CDP1804_PAGES)},
        {CDP1804A, TEXT_CPU_1804A, ARRAY_RANGE(CDP1804A_PAGES)},
};

static const Cpu *cpu(CpuType cpuType) {
    return Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
}

static bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_REGN)
        return table == M_REG1;
    if (opr == M_ADDR)
        return table == M_REGN || table == M_REG1 || table == M_IMM8 || table == M_PAGE ||
               table == M_IOAD;
    return false;
}

static bool acceptModes(AsmInsn &insn, const Entry *entry) {
    auto flags = insn.flags();
    auto table = entry->flags();
    return acceptMode(flags.mode1(), table.mode1()) && acceptMode(flags.mode2(), table.mode2());
}

Error TableCdp1802::searchName(CpuType cpuType, AsmInsn &insn) const {
    cpu(cpuType)->searchName(insn, acceptModes);
    return insn.getError();
}

static bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *page) {
    auto opCode = insn.opCode();
    auto flags = entry->flags();
    auto mode = flags.mode1();
    if (mode == M_REGN || mode == M_REG1) {
        opCode &= ~0x0F;
    } else if (mode == M_IOAD) {
        opCode &= ~7;
    }
    return opCode == entry->opCode();
}

Error TableCdp1802::searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) const {
    auto entry = cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    if (entry && entry->flags().undefined())
        insn.setError(UNKNOWN_INSTRUCTION);
    return insn.getError();
}

bool TableCdp1802::isPrefix(CpuType cpuType, Config::opcode_t code) const {
    return cpu(cpuType)->isPrefix(code);
}

const /*PROGMEM*/ char *TableCdp1802::listCpu_P() const {
    return TEXT_CPU_LIST;
}

const /*PROGMEM*/ char *TableCdp1802::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableCdp1802::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    name.iexpectText_P(TEXT_CPU_LIST, 3);
    const auto t = Cpu::search(name, ARRAY_RANGE(CPU_TABLE));
    if (t) {
        cpuType = t->cpuType();
        return OK;
    }
    return UNSUPPORTED_CPU;
}

const TableCdp1802 TABLE;

}  // namespace cdp1802
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

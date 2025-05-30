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

#define E1(_opc, _cf, _name, _mode) {_opc, Entry::Flags::create(_cf, _mode), _name}
#define E0(_opc, _cf, _name) E1(_opc, _cf, _name, M_NONE)

// clang-format off
constexpr Entry TABLE_CDP1802[] PROGMEM = {
    E0(0x00, CF_00, TEXT_IDL),
    E1(0x00, CF_0F, TEXT_LDN,  M_REG1),
    E1(0x10, CF_0F, TEXT_INC,  M_REGN),
    E1(0x20, CF_0F, TEXT_DEC,  M_REGN),
    E1(0x30, CF_00, TEXT_BR,   M_SHRT),
    E1(0x31, CF_00, TEXT_BQ,   M_SHRT),
    E1(0x32, CF_00, TEXT_BZ,   M_SHRT),
    E1(0x33, CF_00, TEXT_BDF,  M_SHRT),
    E1(0x33, CF_00, TEXT_BPZ,  M_SHRT),
    E1(0x33, CF_00, TEXT_BGE,  M_SHRT),
    E1(0x34, CF_00, TEXT_B1,   M_PAGE),
    E1(0x35, CF_00, TEXT_B2,   M_PAGE),
    E1(0x36, CF_00, TEXT_B3,   M_PAGE),
    E1(0x37, CF_00, TEXT_B4,   M_PAGE),
    E0(0x38, CF_00, TEXT_SKP),
    E1(0x38, CF_00, TEXT_NBR,  M_PAGE),
    E1(0x39, CF_00, TEXT_BNQ,  M_SHRT),
    E1(0x3A, CF_00, TEXT_BNZ,  M_SHRT),
    E1(0x3B, CF_00, TEXT_BNF,  M_SHRT),
    E1(0x3B, CF_00, TEXT_BM,   M_SHRT),
    E1(0x3B, CF_00, TEXT_BL,   M_SHRT),
    E1(0x3C, CF_00, TEXT_BN1,  M_PAGE),
    E1(0x3D, CF_00, TEXT_BN2,  M_PAGE),
    E1(0x3E, CF_00, TEXT_BN3,  M_PAGE),
    E1(0x3F, CF_00, TEXT_BN4,  M_PAGE),
    E1(0x40, CF_0F, TEXT_LDA,  M_REGN),
    E1(0x50, CF_0F, TEXT_STR,  M_REGN),
    E0(0x60, CF_00, TEXT_IRX),
    E1(0x60, CF_07, TEXT_OUT,  M_IOAD),
    E1(0x68, CF_07, TEXT_INP,  M_IOAD),
    E0(0x70, CF_00, TEXT_RET),
    E0(0x71, CF_00, TEXT_DIS),
    E0(0x72, CF_00, TEXT_LDXA),
    E0(0x73, CF_00, TEXT_STXD),
    E0(0x74, CF_00, TEXT_ADC),
    E0(0x75, CF_00, TEXT_SDB),
    E0(0x76, CF_00, TEXT_SHRC),
    E0(0x76, CF_00, TEXT_RSHR),
    E0(0x77, CF_00, TEXT_SMB),
    E0(0x78, CF_00, TEXT_SAV),
    E0(0x79, CF_00, TEXT_MARK),
    E0(0x7A, CF_00, TEXT_REQ),
    E0(0x7B, CF_00, TEXT_SEQ),
    E1(0x7C, CF_00, TEXT_ADCI, M_IMM8),
    E1(0x7D, CF_00, TEXT_SDBI, M_IMM8),
    E0(0x7E, CF_00, TEXT_SHLC),
    E0(0x7E, CF_00, TEXT_RSHL),
    E1(0x7F, CF_00, TEXT_SMBI, M_IMM8),
    E1(0x80, CF_0F, TEXT_GLO,  M_REGN),
    E1(0x90, CF_0F, TEXT_GHI,  M_REGN),
    E1(0xA0, CF_0F, TEXT_PLO,  M_REGN),
    E1(0xB0, CF_0F, TEXT_PHI,  M_REGN),
    E1(0xC0, CF_00, TEXT_LBR,  M_LONG),
    E1(0xC1, CF_00, TEXT_LBQ,  M_LONG),
    E1(0xC2, CF_00, TEXT_LBZ,  M_LONG),
    E1(0xC3, CF_00, TEXT_LBDF, M_LONG),
    E1(0xC3, CF_00, TEXT_LBPZ, M_LONG),
    E1(0xC3, CF_00, TEXT_LBGE, M_LONG),
    E0(0xC4, CF_00, TEXT_NOP),
    E0(0xC5, CF_00, TEXT_LSNQ),
    E0(0xC6, CF_00, TEXT_LSNZ),
    E0(0xC7, CF_00, TEXT_LSNF),
    E0(0xC8, CF_00, TEXT_LSKP),
    E1(0xC8, CF_00, TEXT_NLBR, M_ADDR),
    E1(0xC9, CF_00, TEXT_LBNQ, M_LONG),
    E1(0xCA, CF_00, TEXT_LBNZ, M_LONG),
    E1(0xCB, CF_00, TEXT_LBNF, M_LONG),
    E1(0xCB, CF_00, TEXT_LBM,  M_LONG),
    E1(0xCB, CF_00, TEXT_LBL,  M_LONG),
    E0(0xCC, CF_00, TEXT_LSIE),
    E0(0xCD, CF_00, TEXT_LSQ),
    E0(0xCE, CF_00, TEXT_LSZ),
    E0(0xCF, CF_00, TEXT_LSDF),
    E1(0xD0, CF_0F, TEXT_SEP,  M_REGN),
    E1(0xE0, CF_0F, TEXT_SEX,  M_REGN),
    E0(0xF0, CF_00, TEXT_LDX),
    E0(0xF1, CF_00, TEXT_OR),
    E0(0xF2, CF_00, TEXT_AND),
    E0(0xF3, CF_00, TEXT_XOR),
    E0(0xF4, CF_00, TEXT_ADD),
    E0(0xF5, CF_00, TEXT_SD),
    E0(0xF6, CF_00, TEXT_SHR),
    E0(0xF7, CF_00, TEXT_SM),
    E1(0xF8, CF_00, TEXT_LDI,  M_IMM8),
    E1(0xF9, CF_00, TEXT_ORI,  M_IMM8),
    E1(0xFA, CF_00, TEXT_ANI,  M_IMM8),
    E1(0xFB, CF_00, TEXT_XRI,  M_IMM8),
    E1(0xFC, CF_00, TEXT_ADI,  M_IMM8),
    E1(0xFD, CF_00, TEXT_SDI,  M_IMM8),
    E0(0xFE, CF_00, TEXT_SHL),
    E1(0xFF, CF_00, TEXT_SMI,  M_IMM8),
};

constexpr uint8_t INDEX_CDP1802[] PROGMEM = {
     34,  // TEXT_ADC
     43,  // TEXT_ADCI
     79,  // TEXT_ADD
     87,  // TEXT_ADI
     77,  // TEXT_AND
     85,  // TEXT_ANI
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
     31,  // TEXT_DIS
     49,  // TEXT_GHI
     48,  // TEXT_GLO
      0,  // TEXT_IDL
      2,  // TEXT_INC
     29,  // TEXT_INP
     27,  // TEXT_IRX
     55,  // TEXT_LBDF
     57,  // TEXT_LBGE
     68,  // TEXT_LBL
     67,  // TEXT_LBM
     66,  // TEXT_LBNF
     64,  // TEXT_LBNQ
     65,  // TEXT_LBNZ
     56,  // TEXT_LBPZ
     53,  // TEXT_LBQ
     52,  // TEXT_LBR
     54,  // TEXT_LBZ
     25,  // TEXT_LDA
     83,  // TEXT_LDI
      1,  // TEXT_LDN
     75,  // TEXT_LDX
     32,  // TEXT_LDXA
     72,  // TEXT_LSDF
     69,  // TEXT_LSIE
     62,  // TEXT_LSKP
     61,  // TEXT_LSNF
     59,  // TEXT_LSNQ
     60,  // TEXT_LSNZ
     70,  // TEXT_LSQ
     71,  // TEXT_LSZ
     40,  // TEXT_MARK
     15,  // TEXT_NBR
     63,  // TEXT_NLBR
     58,  // TEXT_NOP
     76,  // TEXT_OR
     84,  // TEXT_ORI
     28,  // TEXT_OUT
     51,  // TEXT_PHI
     50,  // TEXT_PLO
     41,  // TEXT_REQ
     30,  // TEXT_RET
     46,  // TEXT_RSHL
     37,  // TEXT_RSHR
     39,  // TEXT_SAV
     80,  // TEXT_SD
     35,  // TEXT_SDB
     44,  // TEXT_SDBI
     88,  // TEXT_SDI
     73,  // TEXT_SEP
     42,  // TEXT_SEQ
     74,  // TEXT_SEX
     89,  // TEXT_SHL
     45,  // TEXT_SHLC
     81,  // TEXT_SHR
     36,  // TEXT_SHRC
     14,  // TEXT_SKP
     82,  // TEXT_SM
     38,  // TEXT_SMB
     47,  // TEXT_SMBI
     90,  // TEXT_SMI
     26,  // TEXT_STR
     33,  // TEXT_STXD
     78,  // TEXT_XOR
     86,  // TEXT_XRI
};

constexpr Entry TABLE_CDP1804[] PROGMEM = {
    E0(0x00, CF_00, TEXT_STPC),
    E0(0x01, CF_00, TEXT_DTC),
    E0(0x02, CF_00, TEXT_SPM2),
    E0(0x03, CF_00, TEXT_SCM2),
    E0(0x04, CF_00, TEXT_SPM1),
    E0(0x05, CF_00, TEXT_SCM1),
    E0(0x06, CF_00, TEXT_LDC),
    E0(0x07, CF_00, TEXT_STM),
    E0(0x08, CF_00, TEXT_GEC),
    E0(0x09, CF_00, TEXT_ETQ),
    E0(0x0A, CF_00, TEXT_XIE),
    E0(0x0B, CF_00, TEXT_XID),
    E0(0x0C, CF_00, TEXT_CIE),
    E0(0x0D, CF_00, TEXT_CID),
    E1(0x3E, CF_00, TEXT_BCI,  M_PAGE),
    E1(0x3F, CF_00, TEXT_BXI,  M_PAGE),
    E1(0x60, CF_0F, TEXT_RLXA, M_REGN),
    E1(0x80, CF_0F, TEXT_SCAL, M_REGN_ADDR),
    E1(0x90, CF_0F, TEXT_SRET, M_REGN),
    E1(0xA0, CF_0F, TEXT_RSXD, M_REGN),
    E1(0xB0, CF_0F, TEXT_RNX,  M_REGN),
    E1(0xC0, CF_0F, TEXT_RLDI, M_REGN_ADDR),
};

constexpr uint8_t INDEX_CDP1804[] PROGMEM = {
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

constexpr Entry TABLE_CDP1804A[] PROGMEM = {
    E1(0x20, CF_0F, TEXT_DBNZ, M_REGN_ADDR),
    E0(0x74, CF_00, TEXT_DADC),
    E0(0x76, CF_00, TEXT_DSAV),
    E0(0x77, CF_00, TEXT_DSMB),
    E1(0x7C, CF_00, TEXT_DACI, M_IMM8),
    E1(0x7F, CF_00, TEXT_DSBI, M_IMM8),
    E0(0xF4, CF_00, TEXT_DADD),
    E0(0xF7, CF_00, TEXT_DSM),
    E1(0xFC, CF_00, TEXT_DADI, M_IMM8),
    E1(0xFF, CF_00, TEXT_DSMI, M_IMM8),
};

constexpr uint8_t INDEX_CDP1804A[] PROGMEM = {
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

using EntryPage = entry::PrefixTableBase<Entry>;

constexpr EntryPage CDP1802_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(TABLE_CDP1802), ARRAY_RANGE(INDEX_CDP1802)},
};

constexpr EntryPage CDP1804_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(TABLE_CDP1802), ARRAY_RANGE(INDEX_CDP1802)},
        {0x68, ARRAY_RANGE(TABLE_CDP1804), ARRAY_RANGE(INDEX_CDP1804)},
};

constexpr EntryPage CDP1804A_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(TABLE_CDP1802), ARRAY_RANGE(INDEX_CDP1802)},
        {0x68, ARRAY_RANGE(TABLE_CDP1804), ARRAY_RANGE(INDEX_CDP1804)},
        {0x68, ARRAY_RANGE(TABLE_CDP1804A), ARRAY_RANGE(INDEX_CDP1804A)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {CDP1802, TEXT_CPU_1802, ARRAY_RANGE(CDP1802_PAGES)},
        {CDP1804, TEXT_CPU_1804, ARRAY_RANGE(CDP1804_PAGES)},
        {CDP1804A, TEXT_CPU_1804A, ARRAY_RANGE(CDP1804A_PAGES)},
};

const Cpu *cpu(CpuType cpuType) {
    return Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
}

bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_REGN)
        return table == M_REG1;
    if (opr == M_ADDR)
        return table == M_REGN || table == M_REG1 || table == M_IMM8 || table == M_PAGE ||
               table == M_IOAD || table == M_SHRT || table == M_LONG;
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
    const auto flags = entry->readFlags();
    if (flags.mode1() == M_IOAD && (opc & 7) == 0)
        return false;
    opc &= ~flags.mask();
    return opc == entry->readOpCode();
}

Error searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) {
    cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    return insn.getError();
}

bool isPrefix(CpuType cpuType, Config::opcode_t code) {
    return cpu(cpuType)->isPrefix(code);
}

const /*PROGMEM*/ char *TableCdp1802::listCpu_P() const {
    return TEXT_CDP1802_LIST;
}

const /*PROGMEM*/ char *TableCdp1802::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableCdp1802::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    name.iexpectText_P(TEXT_CDP1802_LIST, 3);
    const auto t = Cpu::search(name, ARRAY_RANGE(CPU_TABLE));
    if (t) {
        cpuType = t->readCpuType();
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

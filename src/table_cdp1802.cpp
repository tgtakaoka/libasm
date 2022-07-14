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

#include <ctype.h>
#include <string.h>

#include "config_cdp1802.h"
#include "entry_cdp1802.h"
#include "text_cdp1802.h"

namespace libasm {
namespace cdp1802 {

#define X(_opc, _name, _op1, _op2) \
    { _opc, Entry::Flags::create(_op1, _op2), _name }
#define E(_opc, _name, _op1) X(_opc, _name, _op1, NONE)

// clang-format off
static constexpr Entry TABLE_CDP1802[] PROGMEM = {
    E(0x00, TEXT_IDL,  NONE),
    E(0x00, TEXT_LDN,  REG1),
    E(0x10, TEXT_INC,  REGN),
    E(0x20, TEXT_DEC,  REGN),
    E(0x30, TEXT_BR,   PAGE),
    E(0x31, TEXT_BQ,   PAGE),
    E(0x32, TEXT_BZ,   PAGE),
    E(0x33, TEXT_BDF,  PAGE),
    E(0x33, TEXT_BPZ,  PAGE),
    E(0x33, TEXT_BGE,  PAGE),
    E(0x34, TEXT_B1,   PAGE),
    E(0x35, TEXT_B2,   PAGE),
    E(0x36, TEXT_B3,   PAGE),
    E(0x37, TEXT_B4,   PAGE),
    E(0x38, TEXT_SKP,  NONE),
    E(0x38, TEXT_NBR,  PAGE),
    E(0x39, TEXT_BNQ,  PAGE),
    E(0x3A, TEXT_BNZ,  PAGE),
    E(0x3B, TEXT_BNF,  PAGE),
    E(0x3B, TEXT_BM,   PAGE),
    E(0x3B, TEXT_BL,   PAGE),
    E(0x3C, TEXT_BN1,  PAGE),
    E(0x3D, TEXT_BN2,  PAGE),
    E(0x3E, TEXT_BN3,  PAGE),
    E(0x3F, TEXT_BN4,  PAGE),
    E(0x40, TEXT_LDA,  REGN),
    E(0x50, TEXT_STR,  REGN),
    E(0x60, TEXT_IRX,  NONE),
    E(0x60, TEXT_OUT,  IOAD),
    E(0x68, TEXT_null, UNDF),   // undefined
    E(0x68, TEXT_INP,  IOAD),
    E(0x70, TEXT_RET,  NONE),
    E(0x71, TEXT_DIS,  NONE),
    E(0x72, TEXT_LDXA, NONE),
    E(0x73, TEXT_STXD, NONE),
    E(0x74, TEXT_ADC,  NONE),
    E(0x75, TEXT_SDB,  NONE),
    E(0x76, TEXT_SHRC, NONE),
    E(0x76, TEXT_RSHR, NONE),
    E(0x77, TEXT_SMB,  NONE),
    E(0x78, TEXT_SAV,  NONE),
    E(0x79, TEXT_MARK, NONE),
    E(0x7A, TEXT_REQ,  NONE),
    E(0x7B, TEXT_SEQ,  NONE),
    E(0x7C, TEXT_ADCI, IMM8),
    E(0x7D, TEXT_SDBI, IMM8),
    E(0x7E, TEXT_SHLC, NONE),
    E(0x7E, TEXT_RSHL, NONE),
    E(0x7F, TEXT_SMBI, IMM8),
    E(0x80, TEXT_GLO,  REGN),
    E(0x90, TEXT_GHI,  REGN),
    E(0xA0, TEXT_PLO,  REGN),
    E(0xB0, TEXT_PHI,  REGN),
    E(0xC0, TEXT_LBR,  ADDR),
    E(0xC1, TEXT_LBQ,  ADDR),
    E(0xC2, TEXT_LBZ,  ADDR),
    E(0xC3, TEXT_LBDF, ADDR),
    E(0xC3, TEXT_LBPZ, ADDR),
    E(0xC3, TEXT_LBGE, ADDR),
    E(0xC4, TEXT_NOP,  NONE),
    E(0xC5, TEXT_LSNQ, NONE),
    E(0xC6, TEXT_LSNZ, NONE),
    E(0xC7, TEXT_LSNF, NONE),
    E(0xC8, TEXT_LSKP, NONE),
    E(0xC8, TEXT_NLBR, ADDR),
    E(0xC9, TEXT_LBNQ, ADDR),
    E(0xCA, TEXT_LBNZ, ADDR),
    E(0xCB, TEXT_LBNF, ADDR),
    E(0xCB, TEXT_LBM,  ADDR),
    E(0xCB, TEXT_LBL,  ADDR),
    E(0xCC, TEXT_LSIE, NONE),
    E(0xCD, TEXT_LSQ,  NONE),
    E(0xCE, TEXT_LSZ,  NONE),
    E(0xCF, TEXT_LSDF, NONE),
    E(0xD0, TEXT_SEP,  REGN),
    E(0xE0, TEXT_SEX,  REGN),
    E(0xF0, TEXT_LDX,  NONE),
    E(0xF1, TEXT_OR,   NONE),
    E(0xF2, TEXT_AND,  NONE),
    E(0xF3, TEXT_XOR,  NONE),
    E(0xF4, TEXT_ADD,  NONE),
    E(0xF5, TEXT_SD,   NONE),
    E(0xF6, TEXT_SHR,  NONE),
    E(0xF7, TEXT_SM,   NONE),
    E(0xF8, TEXT_LDI,  IMM8),
    E(0xF9, TEXT_ORI,  IMM8),
    E(0xFA, TEXT_ANI,  IMM8),
    E(0xFB, TEXT_XRI,  IMM8),
    E(0xFC, TEXT_ADI,  IMM8),
    E(0xFD, TEXT_SDI,  IMM8),
    E(0xFE, TEXT_SHL,  NONE),
    E(0xFF, TEXT_SMI,  IMM8),
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
    E(0x00, TEXT_STPC, NONE),
    E(0x01, TEXT_DTC,  NONE),
    E(0x02, TEXT_SPM2, NONE),
    E(0x03, TEXT_SCM2, NONE),
    E(0x04, TEXT_SPM1, NONE),
    E(0x05, TEXT_SCM1, NONE),
    E(0x06, TEXT_LDC,  NONE),
    E(0x07, TEXT_STM,  NONE),
    E(0x08, TEXT_GEC,  NONE),
    E(0x09, TEXT_ETQ,  NONE),
    E(0x0A, TEXT_XIE,  NONE),
    E(0x0B, TEXT_XID,  NONE),
    E(0x0C, TEXT_CIE,  NONE),
    E(0x0D, TEXT_CID,  NONE),
    E(0x3E, TEXT_BCI,  PAGE),
    E(0x3F, TEXT_BXI,  PAGE),
    E(0x60, TEXT_RLXA, REGN),
    X(0x80, TEXT_SCAL, REGN, ADDR),
    E(0x90, TEXT_SRET, REGN),
    E(0xA0, TEXT_RSXD, REGN),
    E(0xB0, TEXT_RNX,  REGN),
    X(0xC0, TEXT_RLDI, REGN, ADDR),
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
    X(0x20, TEXT_DBNZ, REGN, ADDR),
    E(0x74, TEXT_DADC, NONE),
    E(0x76, TEXT_DSAV, NONE),
    E(0x77, TEXT_DSMB, NONE),
    E(0x7C, TEXT_DACI, IMM8),
    E(0x7F, TEXT_DSBI, IMM8),
    E(0xF4, TEXT_DADD, NONE),
    E(0xF7, TEXT_DSM,  NONE),
    E(0xFC, TEXT_DADI, IMM8),
    E(0xFF, TEXT_DSMI, IMM8),
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

static constexpr TableCdp1802::EntryPage CDP1802_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(TABLE_CDP1802), ARRAY_RANGE(INDEX_CDP1802)},
};

static constexpr TableCdp1802::EntryPage CDP1804_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(TABLE_CDP1802), ARRAY_RANGE(INDEX_CDP1802)},
        {0x68, ARRAY_RANGE(TABLE_CDP1804), ARRAY_RANGE(INDEX_CDP1804)},
};

static constexpr TableCdp1802::EntryPage CDP1804A_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(TABLE_CDP1802), ARRAY_RANGE(INDEX_CDP1802)},
        {0x68, ARRAY_RANGE(TABLE_CDP1804), ARRAY_RANGE(INDEX_CDP1804)},
        {0x68, ARRAY_RANGE(TABLE_CDP1804A), ARRAY_RANGE(INDEX_CDP1804A)},
};

static constexpr TableCdp1802::Cpu CPU_TABLES[] PROGMEM = {
        {CDP1802, TEXT_CPU_1802, ARRAY_RANGE(CDP1802_PAGES)},
        {CDP1804, TEXT_CPU_1804, ARRAY_RANGE(CDP1804_PAGES)},
        {CDP1804A, TEXT_CPU_1804A, ARRAY_RANGE(CDP1804A_PAGES)},
};

bool TableCdp1802::isPrefix(Config::opcode_t opCode) const {
    return _cpu->cpuType() != CDP1802 && opCode == 0x68;
}

static bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == REGN)
        return table == REG1;
    if (opr == ADDR)
        return table == REGN || table == REG1 || table == IMM8 || table == PAGE || table == IOAD;
    return false;
}

static bool acceptModes(Entry::Flags flags, const Entry *entry) {
    const Entry::Flags table = entry->flags();
    return acceptMode(flags.mode1(), table.mode1()) && acceptMode(flags.mode2(), table.mode2());
}

Error TableCdp1802::searchName(InsnCdp1802 &insn) {
    uint8_t count = 0;
    for (auto page = _cpu->table(); page < _cpu->end(); page++) {
        auto entry = searchEntry(insn.name(), insn.flags(), page, acceptModes, count);
        if (entry) {
            insn.setOpCode(entry->opCode(), page->prefix());
            insn.setFlags(entry->flags());
            return setOK();
        }
    }
    return setError(count == 0 ? UNKNOWN_INSTRUCTION : OPERAND_NOT_ALLOWED);
}

static Config::opcode_t tableCode(Config::opcode_t opCode, const Entry *entry) {
    auto mode = entry->flags().mode1();
    if (mode == REGN || mode == REG1)
        return opCode & ~0x0F;
    if (mode == IOAD)
        return opCode & ~7;
    return opCode;
}

Error TableCdp1802::searchOpCode(InsnCdp1802 &insn) {
    for (auto page = _cpu->table(); page < _cpu->end(); page++) {
        auto prefix = page->prefix();
        if (insn.prefix() != prefix)
            continue;
        auto entry = searchEntry(insn.opCode(), page->table(), page->end(), tableCode);
        if (entry) {
            insn.setFlags(entry->flags());
            if (insn.mode1() == UNDF)
                break;
            insn.setName_P(entry->name_P());
            return setOK();
        }
    }
    return setError(UNKNOWN_INSTRUCTION);
}

TableCdp1802::TableCdp1802() {
    setCpu(CDP1802);
}

bool TableCdp1802::setCpu(CpuType cpuType) {
    auto t = Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLES));
    if (t == nullptr)
        return false;
    _cpu = t;
    return true;
}

const /* PROGMEM */ char *TableCdp1802::listCpu_P() const {
    return TEXT_CPU_LIST;
}

const /* PROGMEM */ char *TableCdp1802::cpu_P() const {
    return _cpu->name_P();
}

bool TableCdp1802::setCpu(const char *cpu) {
    if (strncasecmp_P(cpu, TEXT_CPU_CDP, 3) == 0)
        cpu += 3;
    auto t = Cpu::search(cpu, ARRAY_RANGE(CPU_TABLES));
    if (t)
        return setCpu(t->cpuType());
    return false;
}

TableCdp1802 TableCdp1802::TABLE;

}  // namespace cdp1802
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

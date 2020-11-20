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

#define E(_opc, _name, _amode) \
    { _opc, Entry::Flags::create(_amode), _name }

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
    E(0x68, TEXT_IDL,  UNDF),   // reuse IDL for undefined instruction
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
    E(0xC4, TEXT_NOP,  NONE),
    E(0xC5, TEXT_LSNQ, NONE),
    E(0xC6, TEXT_LSNZ, NONE),
    E(0xC7, TEXT_LSNF, NONE),
    E(0xC8, TEXT_LSKP, NONE),
    E(0xC8, TEXT_NLBR, ADDR),
    E(0xC9, TEXT_LBNQ, ADDR),
    E(0xCA, TEXT_LBNZ, ADDR),
    E(0xCB, TEXT_LBNF, ADDR),
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
// clang-format on

static bool acceptMode(Entry::Flags flags, const Entry *entry) {
    const AddrMode opr = flags.mode();
    const AddrMode table = entry->flags().mode();
    if (opr == table)
        return true;
    if (opr == ADDR)
        return table == REGN || table == REG1 || table == IMM8 ||
               table == PAGE || table == IOAD;
    return false;
}

Error TableCdp1802::searchName(InsnCdp1802 &insn) const {
    uint8_t count = 0;
    const Entry *entry = TableBase::searchName<Entry, Entry::Flags>(insn.name(),
            insn.flags(), ARRAY_RANGE(TABLE_CDP1802), acceptMode, count);
    if (entry) {
        insn.setOpCode(entry->opCode());
        insn.setFlags(entry->flags());
        return _error.setOK();
    }
    return _error.setError(
            count == 0 ? UNKNOWN_INSTRUCTION : OPERAND_NOT_ALLOWED);
}

static Config::opcode_t tableCode(Config::opcode_t opCode, const Entry *entry) {
    const AddrMode mode = entry->flags().mode();
    if (mode == REGN || mode == REG1)
        return opCode & ~0x0F;
    if (mode == IOAD)
        return opCode & ~7;
    return opCode;
}

Error TableCdp1802::searchOpCode(InsnCdp1802 &insn) const {
    const Config::opcode_t opCode = insn.opCode();
    const Entry *entry = TableBase::searchCode<Entry, Config::opcode_t>(
            opCode, ARRAY_RANGE(TABLE_CDP1802), tableCode);
    if (!entry)
        return _error.setError(UNKNOWN_INSTRUCTION);
    insn.setFlags(entry->flags());
    if (insn.addrMode() == UNDF)
        return _error.setError(UNKNOWN_INSTRUCTION);
    insn.setName_P(entry->name());
    return _error.setOK();
}

const char *TableCdp1802::listCpu() const {
    return TEXT_CPU_CDP1802;
}

const char *TableCdp1802::getCpu() const {
    return TEXT_CPU_1802;
}

bool TableCdp1802::setCpu(const char *cpu) {
    return strcasecmp_P(cpu, TEXT_CPU_1802) == 0 ||
           strcasecmp_P(cpu, TEXT_CPU_CDP1802) == 0;
}

class TableCdp1802 TableCdp1802;

}  // namespace cdp1802
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

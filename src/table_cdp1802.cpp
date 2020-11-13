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

#include "config_cdp1802.h"

#include "entry_cdp1802.h"
#include "table_cdp1802.h"
#include "text_cdp1802.h"

#include <ctype.h>
#include <string.h>

namespace libasm {
namespace cdp1802 {

#define E(_opc, _name, _amode)                          \
    { _opc, Entry::Flags::create(_amode), TEXT_##_name },

static constexpr Entry TABLE_CDP1802[] PROGMEM = {
    E(0x00, IDL,  NONE)
    E(0x00, LDN,  REG1)
    E(0x10, INC,  REGN)
    E(0x20, DEC,  REGN)
    E(0x30, BR,   PAGE)
    E(0x31, BQ,   PAGE)
    E(0x32, BZ,   PAGE)
    E(0x33, BDF,  PAGE)
    E(0x33, BPZ,  PAGE)
    E(0x33, BGE,  PAGE)
    E(0x34, B1,   PAGE)
    E(0x35, B2,   PAGE)
    E(0x36, B3,   PAGE)
    E(0x37, B4,   PAGE)
    E(0x38, SKP,  NONE)
    E(0x38, NBR,  PAGE)
    E(0x39, BNQ,  PAGE)
    E(0x3A, BNZ,  PAGE)
    E(0x3B, BNF,  PAGE)
    E(0x3B, BM,   PAGE)
    E(0x3B, BL,   PAGE)
    E(0x3C, BN1,  PAGE)
    E(0x3D, BN2,  PAGE)
    E(0x3E, BN3,  PAGE)
    E(0x3F, BN4,  PAGE)
    E(0x40, LDA,  REGN)
    E(0x50, STR,  REGN)
    E(0x60, IRX,  NONE)
    E(0x60, OUT,  IOAD)
    E(0x68, IDL,  UNDF)   // reuse IDL for undefined instruction
    E(0x68, INP,  IOAD)
    E(0x70, RET,  NONE)
    E(0x71, DIS,  NONE)
    E(0x72, LDXA, NONE)
    E(0x73, STXD, NONE)
    E(0x74, ADC,  NONE)
    E(0x75, SDB,  NONE)
    E(0x76, SHRC, NONE)
    E(0x76, RSHR, NONE)
    E(0x77, SMB,  NONE)
    E(0x78, SAV,  NONE)
    E(0x79, MARK, NONE)
    E(0x7A, REQ,  NONE)
    E(0x7B, SEQ,  NONE)
    E(0x7C, ADCI, IMM8)
    E(0x7D, SDBI, IMM8)
    E(0x7E, SHLC, NONE)
    E(0x7E, RSHL, NONE)
    E(0x7F, SMBI, IMM8)
    E(0x80, GLO,  REGN)
    E(0x90, GHI,  REGN)
    E(0xA0, PLO,  REGN)
    E(0xB0, PHI,  REGN)
    E(0xC0, LBR,  ADDR)
    E(0xC1, LBQ,  ADDR)
    E(0xC2, LBZ,  ADDR)
    E(0xC3, LBDF, ADDR)
    E(0xC4, NOP,  NONE)
    E(0xC5, LSNQ, NONE)
    E(0xC6, LSNZ, NONE)
    E(0xC7, LSNF, NONE)
    E(0xC8, LSKP, NONE)
    E(0xC8, NLBR, ADDR)
    E(0xC9, LBNQ, ADDR)
    E(0xCA, LBNZ, ADDR)
    E(0xCB, LBNF, ADDR)
    E(0xCC, LSIE, NONE)
    E(0xCD, LSQ,  NONE)
    E(0xCE, LSZ,  NONE)
    E(0xCF, LSDF, NONE)
    E(0xD0, SEP,  REGN)
    E(0xE0, SEX,  REGN)
    E(0xF0, LDX,  NONE)
    E(0xF1, OR,   NONE)
    E(0xF2, AND,  NONE)
    E(0xF3, XOR,  NONE)
    E(0xF4, ADD,  NONE)
    E(0xF5, SD,   NONE)
    E(0xF6, SHR,  NONE)
    E(0xF7, SM,   NONE)
    E(0xF8, LDI,  IMM8)
    E(0xF9, ORI,  IMM8)
    E(0xFA, ANI,  IMM8)
    E(0xFB, XRI,  IMM8)
    E(0xFC, ADI,  IMM8)
    E(0xFD, SDI,  IMM8)
    E(0xFE, SHL,  NONE)
    E(0xFF, SMI,  IMM8)
};

static bool acceptMode(Entry::Flags flags, const Entry *entry) {
    const AddrMode opr = flags.mode();
    const AddrMode table = entry->flags().mode();
    if (opr == table) return true;
    if (opr == ADDR)
        return table == REGN || table == REG1 || table == IMM8
            || table == PAGE || table == IOAD;
    return false;
}

Error TableCdp1802::searchName(InsnCdp1802 &insn) const {
    uint8_t count = 0;
    const Entry *entry = TableBase::searchName<Entry, Entry::Flags>(
        insn.name(), insn.flags(), ARRAY_RANGE(TABLE_CDP1802), acceptMode, count);
    if (entry) {
        insn.setOpCode(entry->opCode());
        insn.setFlags(entry->flags());
        return _error.setOK();
    }
    return _error.setError(count == 0 ? UNKNOWN_INSTRUCTION : OPERAND_NOT_ALLOWED);
}

static Config::opcode_t tableCode(
    Config::opcode_t opCode, const Entry *entry) {
    const AddrMode mode = entry->flags().mode();
    if (mode == REGN || mode == REG1)
        return opCode & ~0x0F;
    if (mode == IOAD)
        return opCode & ~7;
    return opCode;
}

Error TableCdp1802::searchOpCode(InsnCdp1802 &insn) const {
    const Config::opcode_t opCode = insn.opCode();
    const Entry *entry =
        TableBase::searchCode<Entry, Config::opcode_t>(
            opCode, ARRAY_RANGE(TABLE_CDP1802), tableCode);
    if (!entry) return _error.setError(UNKNOWN_INSTRUCTION);
    insn.setFlags(entry->flags());
    if (insn.addrMode() == UNDF) return _error.setError(UNKNOWN_INSTRUCTION);
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
    return strcasecmp_P(cpu, TEXT_CPU_1802) == 0
        || strcasecmp_P(cpu, TEXT_CPU_CDP1802) == 0;
}

class TableCdp1802 TableCdp1802;

} // namespace cdp1802
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

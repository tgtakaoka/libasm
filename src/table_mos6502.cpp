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

#include "config_mos6502.h"

#include "entry_mos6502.h"
#include "table_mos6502.h"
#include "text_mos6502.h"

#include <string.h>

namespace libasm {
namespace mos6502 {

#define E(_opc, _name, _amode) \
    { _opc, Entry::Flags::create(_amode), _name }

// clang-format off
static constexpr Entry MOS6502_TABLE[] PROGMEM = {
    E(0x00, TEXT_BRK, IMPL),
    E(0x40, TEXT_RTI, IMPL),
    E(0x60, TEXT_RTS, IMPL),
    E(0xEA, TEXT_NOP, IMPL),
    E(0x08, TEXT_PHP, IMPL),
    E(0x28, TEXT_PLP, IMPL),
    E(0x48, TEXT_PHA, IMPL),
    E(0x68, TEXT_PLA, IMPL),
    E(0x88, TEXT_DEY, IMPL),
    E(0xC8, TEXT_INY, IMPL),
    E(0xCA, TEXT_DEX, IMPL),
    E(0xE8, TEXT_INX, IMPL),
    E(0x98, TEXT_TYA, IMPL),
    E(0xA8, TEXT_TAY, IMPL),
    E(0x8A, TEXT_TXA, IMPL),
    E(0xAA, TEXT_TAX, IMPL),
    E(0x9A, TEXT_TXS, IMPL),
    E(0xBA, TEXT_TSX, IMPL),
    E(0x18, TEXT_CLC, IMPL),
    E(0x38, TEXT_SEC, IMPL),
    E(0x58, TEXT_CLI, IMPL),
    E(0x78, TEXT_SEI, IMPL),
    E(0xB8, TEXT_CLV, IMPL),
    E(0xD8, TEXT_CLD, IMPL),
    E(0xF8, TEXT_SED, IMPL),
    E(0x0A, TEXT_ASL, ACCM),
    E(0x2A, TEXT_ROL, ACCM),
    E(0x4A, TEXT_LSR, ACCM),
    E(0x6A, TEXT_ROR, ACCM),
    E(0xA0, TEXT_LDY, IMMX),
    E(0xA2, TEXT_LDX, IMMX),
    E(0xC0, TEXT_CPY, IMMX),
    E(0xE0, TEXT_CPX, IMMX),
    E(0x09, TEXT_ORA, IMMA),
    E(0x29, TEXT_AND, IMMA),
    E(0x49, TEXT_EOR, IMMA),
    E(0x69, TEXT_ADC, IMMA),
    E(0xA9, TEXT_LDA, IMMA),
    E(0xC9, TEXT_CMP, IMMA),
    E(0xE9, TEXT_SBC, IMMA),
    E(0x24, TEXT_BIT, ZPG),
    E(0x05, TEXT_ORA, ZPG),
    E(0x25, TEXT_AND, ZPG),
    E(0x45, TEXT_EOR, ZPG),
    E(0x65, TEXT_ADC, ZPG),
    E(0x85, TEXT_STA, ZPG),
    E(0xA5, TEXT_LDA, ZPG),
    E(0xC5, TEXT_CMP, ZPG),
    E(0xE5, TEXT_SBC, ZPG),
    E(0x84, TEXT_STY, ZPG),
    E(0xA4, TEXT_LDY, ZPG),
    E(0xC4, TEXT_CPY, ZPG),
    E(0x86, TEXT_STX, ZPG),
    E(0xA6, TEXT_LDX, ZPG),
    E(0xE4, TEXT_CPX, ZPG),
    E(0xC6, TEXT_DEC, ZPG),
    E(0xE6, TEXT_INC, ZPG),
    E(0x06, TEXT_ASL, ZPG),
    E(0x26, TEXT_ROL, ZPG),
    E(0x46, TEXT_LSR, ZPG),
    E(0x66, TEXT_ROR, ZPG),
    E(0x15, TEXT_ORA, ZPG_IDX),
    E(0x35, TEXT_AND, ZPG_IDX),
    E(0x55, TEXT_EOR, ZPG_IDX),
    E(0x75, TEXT_ADC, ZPG_IDX),
    E(0x95, TEXT_STA, ZPG_IDX),
    E(0xB5, TEXT_LDA, ZPG_IDX),
    E(0xD5, TEXT_CMP, ZPG_IDX),
    E(0xF5, TEXT_SBC, ZPG_IDX),
    E(0x94, TEXT_STY, ZPG_IDX),
    E(0xB4, TEXT_LDY, ZPG_IDX),
    E(0x96, TEXT_STX, ZPG_IDY),
    E(0xB6, TEXT_LDX, ZPG_IDY),
    E(0xD6, TEXT_DEC, ZPG_IDX),
    E(0xF6, TEXT_INC, ZPG_IDX),
    E(0x16, TEXT_ASL, ZPG_IDX),
    E(0x36, TEXT_ROL, ZPG_IDX),
    E(0x56, TEXT_LSR, ZPG_IDX),
    E(0x76, TEXT_ROR, ZPG_IDX),
    E(0x2C, TEXT_BIT, ABS),
    E(0x0D, TEXT_ORA, ABS),
    E(0x2D, TEXT_AND, ABS),
    E(0x4D, TEXT_EOR, ABS),
    E(0x6D, TEXT_ADC, ABS),
    E(0x8D, TEXT_STA, ABS),
    E(0xAD, TEXT_LDA, ABS),
    E(0xCD, TEXT_CMP, ABS),
    E(0xED, TEXT_SBC, ABS),
    E(0x8C, TEXT_STY, ABS),
    E(0xAC, TEXT_LDY, ABS),
    E(0xCC, TEXT_CPY, ABS),
    E(0x8E, TEXT_STX, ABS),
    E(0xAE, TEXT_LDX, ABS),
    E(0xEC, TEXT_CPX, ABS),
    E(0xCE, TEXT_DEC, ABS),
    E(0xEE, TEXT_INC, ABS),
    E(0x0E, TEXT_ASL, ABS),
    E(0x2E, TEXT_ROL, ABS),
    E(0x4E, TEXT_LSR, ABS),
    E(0x6E, TEXT_ROR, ABS),
    E(0x4C, TEXT_JMP, ABS),
    E(0x20, TEXT_JSR, ABS),
    E(0x1D, TEXT_ORA, ABS_IDX),
    E(0x3D, TEXT_AND, ABS_IDX),
    E(0x5D, TEXT_EOR, ABS_IDX),
    E(0x7D, TEXT_ADC, ABS_IDX),
    E(0x9D, TEXT_STA, ABS_IDX),
    E(0xBD, TEXT_LDA, ABS_IDX),
    E(0xDD, TEXT_CMP, ABS_IDX),
    E(0xFD, TEXT_SBC, ABS_IDX),
    E(0x19, TEXT_ORA, ABS_IDY),
    E(0x39, TEXT_AND, ABS_IDY),
    E(0x59, TEXT_EOR, ABS_IDY),
    E(0x79, TEXT_ADC, ABS_IDY),
    E(0x99, TEXT_STA, ABS_IDY),
    E(0xB9, TEXT_LDA, ABS_IDY),
    E(0xD9, TEXT_CMP, ABS_IDY),
    E(0xF9, TEXT_SBC, ABS_IDY),
    E(0xBC, TEXT_LDY, ABS_IDX),
    E(0xBE, TEXT_LDX, ABS_IDY),
    E(0xDE, TEXT_DEC, ABS_IDX),
    E(0xFE, TEXT_INC, ABS_IDX),
    E(0x1E, TEXT_ASL, ABS_IDX),
    E(0x3E, TEXT_ROL, ABS_IDX),
    E(0x5E, TEXT_LSR, ABS_IDX),
    E(0x7E, TEXT_ROR, ABS_IDX),
    E(0x6C, TEXT_JMP, ABS_IDIR),
    E(0x01, TEXT_ORA, ZPG_IDX_IDIR),
    E(0x21, TEXT_AND, ZPG_IDX_IDIR),
    E(0x41, TEXT_EOR, ZPG_IDX_IDIR),
    E(0x61, TEXT_ADC, ZPG_IDX_IDIR),
    E(0x81, TEXT_STA, ZPG_IDX_IDIR),
    E(0xA1, TEXT_LDA, ZPG_IDX_IDIR),
    E(0xC1, TEXT_CMP, ZPG_IDX_IDIR),
    E(0xE1, TEXT_SBC, ZPG_IDX_IDIR),
    E(0x11, TEXT_ORA, ZPG_IDIR_IDY),
    E(0x31, TEXT_AND, ZPG_IDIR_IDY),
    E(0x51, TEXT_EOR, ZPG_IDIR_IDY),
    E(0x71, TEXT_ADC, ZPG_IDIR_IDY),
    E(0x91, TEXT_STA, ZPG_IDIR_IDY),
    E(0xB1, TEXT_LDA, ZPG_IDIR_IDY),
    E(0xD1, TEXT_CMP, ZPG_IDIR_IDY),
    E(0xF1, TEXT_SBC, ZPG_IDIR_IDY),
    E(0x10, TEXT_BPL, REL),
    E(0x30, TEXT_BMI, REL),
    E(0x50, TEXT_BVC, REL),
    E(0x70, TEXT_BVS, REL),
    E(0x90, TEXT_BCC, REL),
    E(0xB0, TEXT_BCS, REL),
    E(0xD0, TEXT_BNE, REL),
    E(0xF0, TEXT_BEQ, REL),
};

static constexpr Entry G65SC02_TABLE[] PROGMEM = {
    E(0x5A, TEXT_PHY, IMPL),
    E(0x7A, TEXT_PLY, IMPL),
    E(0xDA, TEXT_PHX, IMPL),
    E(0xFA, TEXT_PLX, IMPL),
    E(0x1A, TEXT_INC, ACCM),
    E(0x3A, TEXT_DEC, ACCM),
    E(0x89, TEXT_BIT, IMMA),
    E(0x04, TEXT_TSB, ZPG),
    E(0x14, TEXT_TRB, ZPG),
    E(0x64, TEXT_STZ, ZPG),
    E(0x34, TEXT_BIT, ZPG_IDX),
    E(0x74, TEXT_STZ, ZPG_IDX),
    E(0x0C, TEXT_TSB, ABS),
    E(0x1C, TEXT_TRB, ABS),
    E(0x9C, TEXT_STZ, ABS),
    E(0x3C, TEXT_BIT, ABS_IDX),
    E(0x9E, TEXT_STZ, ABS_IDX),
    E(0x7C, TEXT_JMP, ABS_IDX_IDIR),
    E(0x12, TEXT_ORA, ZPG_IDIR),
    E(0x32, TEXT_AND, ZPG_IDIR),
    E(0x52, TEXT_EOR, ZPG_IDIR),
    E(0x72, TEXT_ADC, ZPG_IDIR),
    E(0x92, TEXT_STA, ZPG_IDIR),
    E(0xB2, TEXT_LDA, ZPG_IDIR),
    E(0xD2, TEXT_CMP, ZPG_IDIR),
    E(0xF2, TEXT_SBC, ZPG_IDIR),
    E(0x80, TEXT_BRA, REL),
};

static constexpr Entry R65C02_TABLE[] PROGMEM = {
    E(0x0F, TEXT_BBR0, ZPG_REL),
    E(0x1F, TEXT_BBR1, ZPG_REL),
    E(0x2F, TEXT_BBR2, ZPG_REL),
    E(0x3F, TEXT_BBR3, ZPG_REL),
    E(0x4F, TEXT_BBR4, ZPG_REL),
    E(0x5F, TEXT_BBR5, ZPG_REL),
    E(0x6F, TEXT_BBR6, ZPG_REL),
    E(0x7F, TEXT_BBR7, ZPG_REL),
    E(0x8F, TEXT_BBS0, ZPG_REL),
    E(0x9F, TEXT_BBS1, ZPG_REL),
    E(0xAF, TEXT_BBS2, ZPG_REL),
    E(0xBF, TEXT_BBS3, ZPG_REL),
    E(0xCF, TEXT_BBS4, ZPG_REL),
    E(0xDF, TEXT_BBS5, ZPG_REL),
    E(0xEF, TEXT_BBS6, ZPG_REL),
    E(0xFF, TEXT_BBS7, ZPG_REL),
    E(0x07, TEXT_RMB0, ZPG),
    E(0x17, TEXT_RMB1, ZPG),
    E(0x27, TEXT_RMB2, ZPG),
    E(0x37, TEXT_RMB3, ZPG),
    E(0x47, TEXT_RMB4, ZPG),
    E(0x57, TEXT_RMB5, ZPG),
    E(0x67, TEXT_RMB6, ZPG),
    E(0x77, TEXT_RMB7, ZPG),
    E(0x87, TEXT_SMB0, ZPG),
    E(0x97, TEXT_SMB1, ZPG),
    E(0xA7, TEXT_SMB2, ZPG),
    E(0xB7, TEXT_SMB3, ZPG),
    E(0xC7, TEXT_SMB4, ZPG),
    E(0xD7, TEXT_SMB5, ZPG),
    E(0xE7, TEXT_SMB6, ZPG),
    E(0xF7, TEXT_SMB7, ZPG),
};

static constexpr Entry W65C02S_TABLE[] PROGMEM = {
    E(0xCB, TEXT_WAI, IMPL),
    E(0xDB, TEXT_STP, IMPL),
};

static constexpr Entry W65C816_TABLE[] PROGMEM = {
    E(0xCB, TEXT_WAI, IMPL),
    E(0xDB, TEXT_STP, IMPL),
    E(0x0B, TEXT_PHD, IMPL),
    E(0x1B, TEXT_TCS, IMPL),
    E(0x2B, TEXT_PLD, IMPL),
    E(0x3B, TEXT_TSC, IMPL),
    E(0x4B, TEXT_PHK, IMPL),
    E(0x5B, TEXT_TCD, IMPL),
    E(0x6B, TEXT_RTL, IMPL),
    E(0x7B, TEXT_TDC, IMPL),
    E(0x8B, TEXT_PHB, IMPL),
    E(0x9B, TEXT_TXY, IMPL),
    E(0xAB, TEXT_PLB, IMPL),
    E(0xBB, TEXT_TYX, IMPL),
    E(0xEB, TEXT_XBA, IMPL),
    E(0xFB, TEXT_XCE, IMPL),
    E(0x02, TEXT_COP, IMM8),
    E(0x42, TEXT_WDM, IMM8),
    E(0xC2, TEXT_REP, IMM8),
    E(0xE2, TEXT_SEP, IMM8),
    E(0x5C, TEXT_JMP, ABS_LONG),
    E(0xDC, TEXT_JMP, ABS_IDIR_LONG),
    E(0xFC, TEXT_JSR, ABS_IDX_IDIR),
    E(0x22, TEXT_JSL, ABS_LONG),
    E(0x62, TEXT_PER, REL_LONG),
    E(0x82, TEXT_BRL, REL_LONG),
    E(0xD4, TEXT_PEI, ZPG_IDIR),
    E(0xF4, TEXT_PEA, ABS),
    E(0x44, TEXT_MVP, BLOCK_MOVE),
    E(0x54, TEXT_MVN, BLOCK_MOVE),
    E(0x03, TEXT_ORA, SP_REL),
    E(0x13, TEXT_ORA, SP_REL_IDIR_IDY),
    E(0x07, TEXT_ORA, ZPG_IDIR_LONG),
    E(0x17, TEXT_ORA, ZPG_IDIR_LONG_IDY),
    E(0x0F, TEXT_ORA, ABS_LONG),
    E(0x1F, TEXT_ORA, ABS_LONG_IDX),
    E(0x23, TEXT_AND, SP_REL),
    E(0x33, TEXT_AND, SP_REL_IDIR_IDY),
    E(0x27, TEXT_AND, ZPG_IDIR_LONG),
    E(0x37, TEXT_AND, ZPG_IDIR_LONG_IDY),
    E(0x2F, TEXT_AND, ABS_LONG),
    E(0x3F, TEXT_AND, ABS_LONG_IDX),
    E(0x43, TEXT_EOR, SP_REL),
    E(0x53, TEXT_EOR, SP_REL_IDIR_IDY),
    E(0x47, TEXT_EOR, ZPG_IDIR_LONG),
    E(0x57, TEXT_EOR, ZPG_IDIR_LONG_IDY),
    E(0x4F, TEXT_EOR, ABS_LONG),
    E(0x5F, TEXT_EOR, ABS_LONG_IDX),
    E(0x63, TEXT_ADC, SP_REL),
    E(0x73, TEXT_ADC, SP_REL_IDIR_IDY),
    E(0x67, TEXT_ADC, ZPG_IDIR_LONG),
    E(0x77, TEXT_ADC, ZPG_IDIR_LONG_IDY),
    E(0x6F, TEXT_ADC, ABS_LONG),
    E(0x7F, TEXT_ADC, ABS_LONG_IDX),
    E(0x83, TEXT_STA, SP_REL),
    E(0x93, TEXT_STA, SP_REL_IDIR_IDY),
    E(0x87, TEXT_STA, ZPG_IDIR_LONG),
    E(0x97, TEXT_STA, ZPG_IDIR_LONG_IDY),
    E(0x8F, TEXT_STA, ABS_LONG),
    E(0x9F, TEXT_STA, ABS_LONG_IDX),
    E(0xA3, TEXT_LDA, SP_REL),
    E(0xB3, TEXT_LDA, SP_REL_IDIR_IDY),
    E(0xA7, TEXT_LDA, ZPG_IDIR_LONG),
    E(0xB7, TEXT_LDA, ZPG_IDIR_LONG_IDY),
    E(0xAF, TEXT_LDA, ABS_LONG),
    E(0xBF, TEXT_LDA, ABS_LONG_IDX),
    E(0xC3, TEXT_CMP, SP_REL),
    E(0xD3, TEXT_CMP, SP_REL_IDIR_IDY),
    E(0xC7, TEXT_CMP, ZPG_IDIR_LONG),
    E(0xD7, TEXT_CMP, ZPG_IDIR_LONG_IDY),
    E(0xCF, TEXT_CMP, ABS_LONG),
    E(0xDF, TEXT_CMP, ABS_LONG_IDX),
    E(0xE3, TEXT_SBC, SP_REL),
    E(0xF3, TEXT_SBC, SP_REL_IDIR_IDY),
    E(0xE7, TEXT_SBC, ZPG_IDIR_LONG),
    E(0xF7, TEXT_SBC, ZPG_IDIR_LONG_IDY),
    E(0xEF, TEXT_SBC, ABS_LONG),
    E(0xFF, TEXT_SBC, ABS_LONG_IDX),

    E(0xDC, TEXT_JMPL, ABS_IDIR),
    E(0x07, TEXT_ORAL, ZPG_IDIR),
    E(0x17, TEXT_ORAL, ZPG_IDIR_IDY),
    E(0x27, TEXT_ANDL, ZPG_IDIR),
    E(0x37, TEXT_ANDL, ZPG_IDIR_IDY),
    E(0x47, TEXT_EORL, ZPG_IDIR),
    E(0x57, TEXT_EORL, ZPG_IDIR_IDY),
    E(0x67, TEXT_ADCL, ZPG_IDIR),
    E(0x77, TEXT_ADCL, ZPG_IDIR_IDY),
    E(0x87, TEXT_STAL, ZPG_IDIR),
    E(0x97, TEXT_STAL, ZPG_IDIR_IDY),
    E(0xA7, TEXT_LDAL, ZPG_IDIR),
    E(0xB7, TEXT_LDAL, ZPG_IDIR_IDY),
    E(0xC7, TEXT_CMPL, ZPG_IDIR),
    E(0xD7, TEXT_CMPL, ZPG_IDIR_IDY),
    E(0xE7, TEXT_SBCL, ZPG_IDIR),
    E(0xF7, TEXT_SBCL, ZPG_IDIR_IDY),
};
// clang-format off

class TableMos6502::EntryPage : public EntryPageBase<Entry> {
public:
    constexpr EntryPage(const Entry *table, const Entry *end)
        : EntryPageBase(table, end) {}
};

static constexpr TableMos6502::EntryPage PAGES_MOS6502[] PROGMEM = {
        {ARRAY_RANGE(MOS6502_TABLE)},
};
static constexpr TableMos6502::EntryPage PAGES_G65SC02[] PROGMEM = {
        {ARRAY_RANGE(MOS6502_TABLE)},
        {ARRAY_RANGE(G65SC02_TABLE)},
};
static constexpr TableMos6502::EntryPage PAGES_R65C02[] PROGMEM = {
        {ARRAY_RANGE(MOS6502_TABLE)},
        {ARRAY_RANGE(G65SC02_TABLE)},
        {ARRAY_RANGE(R65C02_TABLE)},
};
static constexpr TableMos6502::EntryPage PAGES_W65C02S[] PROGMEM = {
        {ARRAY_RANGE(MOS6502_TABLE)},
        {ARRAY_RANGE(G65SC02_TABLE)},
        {ARRAY_RANGE(R65C02_TABLE)},
        {ARRAY_RANGE(W65C02S_TABLE)},
};
static constexpr TableMos6502::EntryPage PAGES_W65C816[] PROGMEM = {
        {ARRAY_RANGE(MOS6502_TABLE)},
        {ARRAY_RANGE(G65SC02_TABLE)},
        {ARRAY_RANGE(W65C02S_TABLE)},
        {ARRAY_RANGE(W65C816_TABLE)},
};

static bool acceptAddrMode(AddrMode opr, const Entry *entry) {
    const AddrMode table = entry->flags().mode();
    if (opr == table)
        return true;
    if (opr == IMMA)
        return table == IMMX || table == IMM8;
    if (opr == ZPG)
        return table == ABS || table == REL || table == ABS_LONG ||
               table == REL_LONG;
    if (opr == ZPG_IDX_IDIR)
        return table == ABS_IDX_IDIR;
    if (opr == ZPG_IDIR)
        return table == ABS_IDIR;
    if (opr == ZPG_IDIR_LONG)
        return table == ABS_IDIR_LONG;
    if (opr == ZPG_IDX)
        return table == ABS_IDX;
    if (opr == ZPG_IDY)
        return table == ABS_IDY;
    if (opr == ABS)
        return table == REL || table == ABS_LONG || table == REL_LONG;
    if (opr == ABS_LONG)
        return table == REL || table == REL_LONG;
    if (opr == ZPG_REL)
        return table == BLOCK_MOVE;
    return false;
}

Error TableMos6502::searchName(
        InsnMos6502 &insn, const EntryPage *pages, const EntryPage *end) const {
    uint8_t count = 0;
    for (const EntryPage *page = pages; page < end; page++) {
        const Entry *entry = TableBase::searchName<Entry, AddrMode>(insn.name(),
                insn.addrMode(), page->table(), page->end(), acceptAddrMode,
                count);
        if (entry) {
            insn.setFlags(entry->flags());
            insn.setOpCode(entry->opCode());
            return OK;
        }
    }
    return count == 0 ? UNKNOWN_INSTRUCTION : OPERAND_NOT_ALLOWED;
}

static bool acceptAddrMode(AddrMode addrMode, bool useIndirectLong) {
    if (addrMode == ABS_IDIR_LONG || addrMode == ZPG_IDIR_LONG ||
            addrMode == ZPG_IDIR_LONG_IDY)
        return useIndirectLong;
    return true;
}

Error TableMos6502::searchOpCode(InsnMos6502 &insn, bool useIndirectLong,
        const EntryPage *pages, const EntryPage *end) const {
    const Config::opcode_t opCode = insn.opCode();
    for (const EntryPage *page = pages; page < end; page++) {
        for (const Entry *entry = page->table();
                entry < page->end() &&
                (entry = TableBase::searchCode<Entry, Config::opcode_t>(
                         opCode, entry, page->end()));
                entry++) {
            insn.setFlags(entry->flags());
            if (!acceptAddrMode(insn.addrMode(), useIndirectLong))
                continue;
            insn.setName_P(entry->name());
            return OK;
        }
    }
    return UNKNOWN_INSTRUCTION;
}

Error TableMos6502::searchName(InsnMos6502 &insn) const {
    return setError(searchName(insn, _table, _end));
}

Error TableMos6502::searchOpCode(InsnMos6502 &insn) const {
    return setError(searchOpCode(insn, _useIndirectLong, _table, _end));
}

TableMos6502::TableMos6502() {
    setCpu(MOS6502);
    _useIndirectLong = true;
    _longAccumulator = _longIndex = false;
}

bool TableMos6502::setCpu(CpuType cpuType) {
    _cpuType = cpuType;
    if (cpuType == MOS6502) {
        _table = ARRAY_BEGIN(PAGES_MOS6502);
        _end = ARRAY_END(PAGES_MOS6502);
        return true;
    }
    if (cpuType == G65SC02) {
        _table = ARRAY_BEGIN(PAGES_G65SC02);
        _end = ARRAY_END(PAGES_G65SC02);
        return true;
    }
    if (cpuType == R65C02) {
        _table = ARRAY_BEGIN(PAGES_R65C02);
        _end = ARRAY_END(PAGES_R65C02);
        return true;
    }
    if (cpuType == W65C02S) {
        _table = ARRAY_BEGIN(PAGES_W65C02S);
        _end = ARRAY_END(PAGES_W65C02S);
        return true;
    }
    if (cpuType == W65C816) {
        _table = ARRAY_BEGIN(PAGES_W65C816);
        _end = ARRAY_END(PAGES_W65C816);
        return true;
    }
    return false;
}

AddressWidth TableMos6502::addressWidth() const {
    return _cpuType == W65C816 ? ADDRESS_24BIT : ADDRESS_16BIT;
}

const char *TableMos6502::listCpu() const {
    return TEXT_CPU_LIST;
}

const char *TableMos6502::getCpu() const {
    if (_cpuType == MOS6502)
        return TEXT_CPU_6502;
    if (_cpuType == G65SC02)
        return TEXT_CPU_65SC02;
    if (_cpuType == R65C02)
        return TEXT_CPU_65C02;
    return _cpuType == W65C02S ? TEXT_CPU_W65C02S : TEXT_CPU_65816;
}

bool TableMos6502::setCpu(const char *cpu) {
    const char *p;
    p = cpu;
    if (strncasecmp_P(cpu, TEXT_CPU_MOS, 3) == 0)
        p += 3;
    if (strcmp_P(p, TEXT_CPU_6502) == 0)
        return setCpu(MOS6502);
    p = cpu + (toupper(*cpu) == 'R' ? 1 : 0);
    if (strcasecmp_P(p, TEXT_CPU_65C02) == 0)
        return setCpu(R65C02);
    p = cpu + (toupper(*cpu) == 'G' ? 1 : 0);
    if (strcasecmp_P(p, TEXT_CPU_65SC02) == 0)
        return setCpu(G65SC02);
    p = cpu + (toupper(*cpu) == 'W' ? 1 : 0);
    if (strcasecmp_P(p, TEXT_CPU_65C02) == 0 ||
            strcasecmp_P(p, TEXT_CPU_65C02S) == 0)
        return setCpu(W65C02S);
    if (strcasecmp_P(p, TEXT_CPU_65816) == 0 ||
            strcasecmp_P(p, TEXT_CPU_65C816) == 0 ||
            strcasecmp_P(p, TEXT_CPU_65C816S) == 0)
        return setCpu(W65C816);
    return false;
}

void TableMos6502::useIndirectLong(bool enable) {
    _useIndirectLong = enable;
}

void TableMos6502::longAccumlator(bool on) {
    _longAccumulator = on;
}

void TableMos6502::longIndex(bool on) {
    _longIndex = on;
}

bool TableMos6502::longImmediate(AddrMode addrMode) const {
    if (_cpuType == W65C816) {
        if (addrMode == IMMA)
            return _longAccumulator;
        if (addrMode == IMMX)
            return _longIndex;
    }
    return false;
}

class TableMos6502 TableMos6502;

}  // namespace mos6502
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

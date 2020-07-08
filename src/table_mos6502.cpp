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

#define E(_opc, _name, _amode)                      \
    { _opc, Entry::_flags(_amode), TEXT_##_name },

static constexpr Entry MOS6502_TABLE[] PROGMEM = {
    E(0x00, BRK,  IMPL)
    E(0x40, RTI,  IMPL)
    E(0x60, RTS,  IMPL)
    E(0xEA, NOP,  IMPL)
    E(0x08, PHP,  IMPL)
    E(0x28, PLP,  IMPL)
    E(0x48, PHA,  IMPL)
    E(0x68, PLA,  IMPL)
    E(0x88, DEY,  IMPL)
    E(0xC8, INY,  IMPL)
    E(0xCA, DEX,  IMPL)
    E(0xE8, INX,  IMPL)
    E(0x98, TYA,  IMPL)
    E(0xA8, TAY,  IMPL)
    E(0x8A, TXA,  IMPL)
    E(0xAA, TAX,  IMPL)
    E(0x9A, TXS,  IMPL)
    E(0xBA, TSX,  IMPL)
    E(0x18, CLC,  IMPL)
    E(0x38, SEC,  IMPL)
    E(0x58, CLI,  IMPL)
    E(0x78, SEI,  IMPL)
    E(0xB8, CLV,  IMPL)
    E(0xD8, CLD,  IMPL)
    E(0xF8, SED,  IMPL)
    E(0x0A, ASL,  ACCM)
    E(0x2A, ROL,  ACCM)
    E(0x4A, LSR,  ACCM)
    E(0x6A, ROR,  ACCM)
    E(0xA0, LDY,  IMMX)
    E(0xA2, LDX,  IMMX)
    E(0xC0, CPY,  IMMX)
    E(0xE0, CPX,  IMMX)
    E(0x09, ORA,  IMMA)
    E(0x29, AND,  IMMA)
    E(0x49, EOR,  IMMA)
    E(0x69, ADC,  IMMA)
    E(0xA9, LDA,  IMMA)
    E(0xC9, CMP,  IMMA)
    E(0xE9, SBC,  IMMA)
    E(0x24, BIT,  ZPG)
    E(0x05, ORA,  ZPG)
    E(0x25, AND,  ZPG)
    E(0x45, EOR,  ZPG)
    E(0x65, ADC,  ZPG)
    E(0x85, STA,  ZPG)
    E(0xA5, LDA,  ZPG)
    E(0xC5, CMP,  ZPG)
    E(0xE5, SBC,  ZPG)
    E(0x84, STY,  ZPG)
    E(0xA4, LDY,  ZPG)
    E(0xC4, CPY,  ZPG)
    E(0x86, STX,  ZPG)
    E(0xA6, LDX,  ZPG)
    E(0xE4, CPX,  ZPG)
    E(0xC6, DEC,  ZPG)
    E(0xE6, INC,  ZPG)
    E(0x06, ASL,  ZPG)
    E(0x26, ROL,  ZPG)
    E(0x46, LSR,  ZPG)
    E(0x66, ROR,  ZPG)
    E(0x15, ORA,  ZPG_IDX)
    E(0x35, AND,  ZPG_IDX)
    E(0x55, EOR,  ZPG_IDX)
    E(0x75, ADC,  ZPG_IDX)
    E(0x95, STA,  ZPG_IDX)
    E(0xB5, LDA,  ZPG_IDX)
    E(0xD5, CMP,  ZPG_IDX)
    E(0xF5, SBC,  ZPG_IDX)
    E(0x94, STY,  ZPG_IDX)
    E(0xB4, LDY,  ZPG_IDX)
    E(0x96, STX,  ZPG_IDY)
    E(0xB6, LDX,  ZPG_IDY)
    E(0xD6, DEC,  ZPG_IDX)
    E(0xF6, INC,  ZPG_IDX)
    E(0x16, ASL,  ZPG_IDX)
    E(0x36, ROL,  ZPG_IDX)
    E(0x56, LSR,  ZPG_IDX)
    E(0x76, ROR,  ZPG_IDX)
    E(0x2C, BIT,  ABS)
    E(0x0D, ORA,  ABS)
    E(0x2D, AND,  ABS)
    E(0x4D, EOR,  ABS)
    E(0x6D, ADC,  ABS)
    E(0x8D, STA,  ABS)
    E(0xAD, LDA,  ABS)
    E(0xCD, CMP,  ABS)
    E(0xED, SBC,  ABS)
    E(0x8C, STY,  ABS)
    E(0xAC, LDY,  ABS)
    E(0xCC, CPY,  ABS)
    E(0x8E, STX,  ABS)
    E(0xAE, LDX,  ABS)
    E(0xEC, CPX,  ABS)
    E(0xCE, DEC,  ABS)
    E(0xEE, INC,  ABS)
    E(0x0E, ASL,  ABS)
    E(0x2E, ROL,  ABS)
    E(0x4E, LSR,  ABS)
    E(0x6E, ROR,  ABS)
    E(0x4C, JMP,  ABS)
    E(0x20, JSR,  ABS)
    E(0x1D, ORA,  ABS_IDX)
    E(0x3D, AND,  ABS_IDX)
    E(0x5D, EOR,  ABS_IDX)
    E(0x7D, ADC,  ABS_IDX)
    E(0x9D, STA,  ABS_IDX)
    E(0xBD, LDA,  ABS_IDX)
    E(0xDD, CMP,  ABS_IDX)
    E(0xFD, SBC,  ABS_IDX)
    E(0x19, ORA,  ABS_IDY)
    E(0x39, AND,  ABS_IDY)
    E(0x59, EOR,  ABS_IDY)
    E(0x79, ADC,  ABS_IDY)
    E(0x99, STA,  ABS_IDY)
    E(0xB9, LDA,  ABS_IDY)
    E(0xD9, CMP,  ABS_IDY)
    E(0xF9, SBC,  ABS_IDY)
    E(0xBC, LDY,  ABS_IDX)
    E(0xBE, LDX,  ABS_IDY)
    E(0xDE, DEC,  ABS_IDX)
    E(0xFE, INC,  ABS_IDX)
    E(0x1E, ASL,  ABS_IDX)
    E(0x3E, ROL,  ABS_IDX)
    E(0x5E, LSR,  ABS_IDX)
    E(0x7E, ROR,  ABS_IDX)
    E(0x6C, JMP,  ABS_IDIR)
    E(0x01, ORA,  ZPG_IDX_IDIR)
    E(0x21, AND,  ZPG_IDX_IDIR)
    E(0x41, EOR,  ZPG_IDX_IDIR)
    E(0x61, ADC,  ZPG_IDX_IDIR)
    E(0x81, STA,  ZPG_IDX_IDIR)
    E(0xA1, LDA,  ZPG_IDX_IDIR)
    E(0xC1, CMP,  ZPG_IDX_IDIR)
    E(0xE1, SBC,  ZPG_IDX_IDIR)
    E(0x11, ORA,  ZPG_IDIR_IDY)
    E(0x31, AND,  ZPG_IDIR_IDY)
    E(0x51, EOR,  ZPG_IDIR_IDY)
    E(0x71, ADC,  ZPG_IDIR_IDY)
    E(0x91, STA,  ZPG_IDIR_IDY)
    E(0xB1, LDA,  ZPG_IDIR_IDY)
    E(0xD1, CMP,  ZPG_IDIR_IDY)
    E(0xF1, SBC,  ZPG_IDIR_IDY)
    E(0x10, BPL,  REL)
    E(0x30, BMI,  REL)
    E(0x50, BVC,  REL)
    E(0x70, BVS,  REL)
    E(0x90, BCC,  REL)
    E(0xB0, BCS,  REL)
    E(0xD0, BNE,  REL)
    E(0xF0, BEQ,  REL)
};

static constexpr Entry W65SC02_TABLE[] PROGMEM = {
    E(0x5A, PHY,  IMPL)
    E(0x7A, PLY,  IMPL)
    E(0xDA, PHX,  IMPL)
    E(0xFA, PLX,  IMPL)
    E(0x1A, INC,  ACCM)
    E(0x3A, DEC,  ACCM)
    E(0x89, BIT,  IMMA)
    E(0x04, TSB,  ZPG)
    E(0x14, TRB,  ZPG)
    E(0x64, STZ,  ZPG)
    E(0x34, BIT,  ZPG_IDX)
    E(0x74, STZ,  ZPG_IDX)
    E(0x0C, TSB,  ABS)
    E(0x1C, TRB,  ABS)
    E(0x9C, STZ,  ABS)
    E(0x3C, BIT,  ABS_IDX)
    E(0x9E, STZ,  ABS_IDX)
    E(0x7C, JMP,  ABS_IDX_IDIR)
    E(0x12, ORA,  ZPG_IDIR)
    E(0x32, AND,  ZPG_IDIR)
    E(0x52, EOR,  ZPG_IDIR)
    E(0x72, ADC,  ZPG_IDIR)
    E(0x92, STA,  ZPG_IDIR)
    E(0xB2, LDA,  ZPG_IDIR)
    E(0xD2, CMP,  ZPG_IDIR)
    E(0xF2, SBC,  ZPG_IDIR)
    E(0x80, BRA,  REL)
};

static constexpr Entry R65C02_TABLE[] PROGMEM = {
    E(0x0F, BBR0, ZPG_REL)
    E(0x1F, BBR1, ZPG_REL)
    E(0x2F, BBR2, ZPG_REL)
    E(0x3F, BBR3, ZPG_REL)
    E(0x4F, BBR4, ZPG_REL)
    E(0x5F, BBR5, ZPG_REL)
    E(0x6F, BBR6, ZPG_REL)
    E(0x7F, BBR7, ZPG_REL)
    E(0x8F, BBS0, ZPG_REL)
    E(0x9F, BBS1, ZPG_REL)
    E(0xAF, BBS2, ZPG_REL)
    E(0xBF, BBS3, ZPG_REL)
    E(0xCF, BBS4, ZPG_REL)
    E(0xDF, BBS5, ZPG_REL)
    E(0xEF, BBS6, ZPG_REL)
    E(0xFF, BBS7, ZPG_REL)
    E(0x07, RMB0, ZPG)
    E(0x17, RMB1, ZPG)
    E(0x27, RMB2, ZPG)
    E(0x37, RMB3, ZPG)
    E(0x47, RMB4, ZPG)
    E(0x57, RMB5, ZPG)
    E(0x67, RMB6, ZPG)
    E(0x77, RMB7, ZPG)
    E(0x87, SMB0, ZPG)
    E(0x97, SMB1, ZPG)
    E(0xA7, SMB2, ZPG)
    E(0xB7, SMB3, ZPG)
    E(0xC7, SMB4, ZPG)
    E(0xD7, SMB5, ZPG)
    E(0xE7, SMB6, ZPG)
    E(0xF7, SMB7, ZPG)
};

static constexpr Entry W65C02S_TABLE[] PROGMEM = {
    E(0xCB, WAI,  IMPL)
    E(0xDB, STP,  IMPL)
};

static constexpr Entry W65C816_TABLE[] PROGMEM = {
    E(0xCB, WAI,  IMPL)
    E(0xDB, STP,  IMPL)
    E(0x0B, PHD,  IMPL)
    E(0x1B, TCS,  IMPL)
    E(0x2B, PLD,  IMPL)
    E(0x3B, TSC,  IMPL)
    E(0x4B, PHK,  IMPL)
    E(0x5B, TCD,  IMPL)
    E(0x6B, RTL,  IMPL)
    E(0x7B, TDC,  IMPL)
    E(0x8B, PHB,  IMPL)
    E(0x9B, TXY,  IMPL)
    E(0xAB, PLB,  IMPL)
    E(0xBB, TYX,  IMPL)
    E(0xEB, XBA,  IMPL)
    E(0xFB, XCE,  IMPL)
    E(0x02, COP,  IMM8)
    E(0x42, WDM,  IMM8)
    E(0xC2, REP,  IMM8)
    E(0xE2, SEP,  IMM8)
    E(0x5C, JMP,  ABS_LONG)
    E(0xDC, JMP,  ABS_IDIR_LONG)
    E(0xFC, JSR,  ABS_IDX_IDIR)
    E(0x22, JSL,  ABS_LONG)
    E(0x62, PER,  REL_LONG)
    E(0x82, BRL,  REL_LONG)
    E(0xD4, PEI,  ZPG_IDIR)
    E(0xF4, PEA,  ABS)
    E(0x44, MVP,  BLOCK_MOVE)
    E(0x54, MVN,  BLOCK_MOVE)
    E(0x03, ORA,  SP_REL)
    E(0x13, ORA,  SP_REL_IDIR_IDY)
    E(0x07, ORA,  ZPG_IDIR_LONG)
    E(0x17, ORA,  ZPG_IDIR_LONG_IDY)
    E(0x0F, ORA,  ABS_LONG)
    E(0x1F, ORA,  ABS_LONG_IDX)
    E(0x23, AND,  SP_REL)
    E(0x33, AND,  SP_REL_IDIR_IDY)
    E(0x27, AND,  ZPG_IDIR_LONG)
    E(0x37, AND,  ZPG_IDIR_LONG_IDY)
    E(0x2F, AND,  ABS_LONG)
    E(0x3F, AND,  ABS_LONG_IDX)
    E(0x43, EOR,  SP_REL)
    E(0x53, EOR,  SP_REL_IDIR_IDY)
    E(0x47, EOR,  ZPG_IDIR_LONG)
    E(0x57, EOR,  ZPG_IDIR_LONG_IDY)
    E(0x4F, EOR,  ABS_LONG)
    E(0x5F, EOR,  ABS_LONG_IDX)
    E(0x63, ADC,  SP_REL)
    E(0x73, ADC,  SP_REL_IDIR_IDY)
    E(0x67, ADC,  ZPG_IDIR_LONG)
    E(0x77, ADC,  ZPG_IDIR_LONG_IDY)
    E(0x6F, ADC,  ABS_LONG)
    E(0x7F, ADC,  ABS_LONG_IDX)
    E(0x83, STA,  SP_REL)
    E(0x93, STA,  SP_REL_IDIR_IDY)
    E(0x87, STA,  ZPG_IDIR_LONG)
    E(0x97, STA,  ZPG_IDIR_LONG_IDY)
    E(0x8F, STA,  ABS_LONG)
    E(0x9F, STA,  ABS_LONG_IDX)
    E(0xA3, LDA,  SP_REL)
    E(0xB3, LDA,  SP_REL_IDIR_IDY)
    E(0xA7, LDA,  ZPG_IDIR_LONG)
    E(0xB7, LDA,  ZPG_IDIR_LONG_IDY)
    E(0xAF, LDA,  ABS_LONG)
    E(0xBF, LDA,  ABS_LONG_IDX)
    E(0xC3, CMP,  SP_REL)
    E(0xD3, CMP,  SP_REL_IDIR_IDY)
    E(0xC7, CMP,  ZPG_IDIR_LONG)
    E(0xD7, CMP,  ZPG_IDIR_LONG_IDY)
    E(0xCF, CMP,  ABS_LONG)
    E(0xDF, CMP,  ABS_LONG_IDX)
    E(0xE3, SBC,  SP_REL)
    E(0xF3, SBC,  SP_REL_IDIR_IDY)
    E(0xE7, SBC,  ZPG_IDIR_LONG)
    E(0xF7, SBC,  ZPG_IDIR_LONG_IDY)
    E(0xEF, SBC,  ABS_LONG)
    E(0xFF, SBC,  ABS_LONG_IDX)

    E(0xDC, JMPL, ABS_IDIR)
    E(0x07, ORAL, ZPG_IDIR)
    E(0x17, ORAL, ZPG_IDIR_IDY)
    E(0x27, ANDL, ZPG_IDIR)
    E(0x37, ANDL, ZPG_IDIR_IDY)
    E(0x47, EORL, ZPG_IDIR)
    E(0x57, EORL, ZPG_IDIR_IDY)
    E(0x67, ADCL, ZPG_IDIR)
    E(0x77, ADCL, ZPG_IDIR_IDY)
    E(0x87, STAL, ZPG_IDIR)
    E(0x97, STAL, ZPG_IDIR_IDY)
    E(0xA7, LDAL, ZPG_IDIR)
    E(0xB7, LDAL, ZPG_IDIR_IDY)
    E(0xC7, CMPL, ZPG_IDIR)
    E(0xD7, CMPL, ZPG_IDIR_IDY)
    E(0xE7, SBCL, ZPG_IDIR)
    E(0xF7, SBCL, ZPG_IDIR_IDY)
};

struct TableMos6502::EntryPage {
    const Entry *const table;
    const Entry *const end;
};

static constexpr TableMos6502::EntryPage PAGES_MOS6502[] PROGMEM = {
    { ARRAY_RANGE(MOS6502_TABLE) },
};
static constexpr TableMos6502::EntryPage PAGES_W65SC02[] PROGMEM = {
    { ARRAY_RANGE(MOS6502_TABLE) },
    { ARRAY_RANGE(W65SC02_TABLE) },
};
static constexpr TableMos6502::EntryPage PAGES_R65C02[] PROGMEM = {
    { ARRAY_RANGE(MOS6502_TABLE) },
    { ARRAY_RANGE(W65SC02_TABLE) },
    { ARRAY_RANGE(R65C02_TABLE) },
};
static constexpr TableMos6502::EntryPage PAGES_W65C02S[] PROGMEM = {
    { ARRAY_RANGE(MOS6502_TABLE) },
    { ARRAY_RANGE(W65SC02_TABLE) },
    { ARRAY_RANGE(R65C02_TABLE) },
    { ARRAY_RANGE(W65C02S_TABLE) },
};
static constexpr TableMos6502::EntryPage PAGES_W65C816[] PROGMEM = {
    { ARRAY_RANGE(MOS6502_TABLE) },
    { ARRAY_RANGE(W65SC02_TABLE) },
    { ARRAY_RANGE(W65C02S_TABLE) },
    { ARRAY_RANGE(W65C816_TABLE) },
};

static bool acceptAddrMode(AddrMode opr, const Entry *entry) {
    const AddrMode table = Entry::_addrMode(pgm_read_byte(&entry->flags));
    if (opr == table) return true;
    if (opr == IMMA) return table == IMMX || table == IMM8;
    if (opr == ZPG) return table == ABS || table == REL
                        || table == ABS_LONG || table == REL_LONG;
    if (opr == ZPG_IDX_IDIR) return table == ABS_IDX_IDIR;
    if (opr == ZPG_IDIR) return table == ABS_IDIR;
    if (opr == ZPG_IDIR_LONG) return table == ABS_IDIR_LONG;
    if (opr == ZPG_IDX) return table == ABS_IDX;
    if (opr == ZPG_IDY) return table == ABS_IDY;
    if (opr == ABS) return table == REL
                        || table == ABS_LONG || table == REL_LONG;
    if (opr == ABS_LONG) return table == REL || table == REL_LONG;
    if (opr == ZPG_REL) return table == BLOCK_MOVE;
    return false;
}

Error TableMos6502::searchName(
    InsnMos6502 &insn, const EntryPage *pages, const EntryPage *end) {
    const char *name = insn.name();
    const AddrMode addrMode = insn.addrMode();
    uint8_t count = 0;
    for (const EntryPage *page = pages; page < end; page++) {
        const Entry *table = reinterpret_cast<Entry *>(pgm_read_ptr(&page->table));
        const Entry *end = reinterpret_cast<Entry *>(pgm_read_ptr(&page->end));
        const Entry *entry = TableBase::searchName<Entry, AddrMode>(
            name, addrMode, table, end, acceptAddrMode, count);
        if (entry) {
            insn.setFlags(pgm_read_byte(&entry->flags));
            insn.setOpCode(pgm_read_byte(&entry->opCode));
            return OK;
        }
    }
    return count == 0 ? UNKNOWN_INSTRUCTION : UNKNOWN_OPERAND;
}

static bool acceptAddrMode(AddrMode addrMode, bool useIndirectLong) {
    if (addrMode == ABS_IDIR_LONG
        || addrMode == ZPG_IDIR_LONG
        || addrMode == ZPG_IDIR_LONG_IDY)
        return useIndirectLong;
    return true;
}

Error TableMos6502::searchOpCode(
    InsnMos6502 &insn, bool useIndirectLong,
    const EntryPage *pages, const EntryPage *end) {
    const Config::opcode_t opCode = insn.opCode();
    for (const EntryPage *page = pages; page < end; page++) {
        const Entry *table = reinterpret_cast<Entry *>(pgm_read_ptr(&page->table));
        const Entry *end = reinterpret_cast<Entry *>(pgm_read_ptr(&page->end));
        for (const Entry *entry = table;
             entry < end
                 && (entry = TableBase::searchCode<Entry,Config::opcode_t>(
                         opCode, entry, end));
             entry++) {
            insn.setFlags(pgm_read_byte(&entry->flags));
            if (!acceptAddrMode(insn.addrMode(), useIndirectLong))
                continue;
            const char *name =
                reinterpret_cast<const char *>(pgm_read_ptr(&entry->name));
            TableBase::setName(insn.insn(), name, Config::NAME_MAX);
            return OK;
        }
    }
    return UNKNOWN_INSTRUCTION;
}

Error TableMos6502::searchName(InsnMos6502 &insn) const {
    return _error.setError(searchName(insn, _table, _end));
}

Error TableMos6502::searchOpCode(InsnMos6502 &insn) const {
    return _error.setError(
        searchOpCode(insn, _useIndirectLong, _table, _end));
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
    if (cpuType == W65SC02) {
        _table = ARRAY_BEGIN(PAGES_W65SC02);
        _end = ARRAY_END(PAGES_W65SC02);
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

int8_t TableMos6502::addressBits() const {
    return _cpuType == W65C816 ? 24 : 16;
}

const char *TableMos6502::listCpu() {
    return "6502, 65SC02, 65C02, W65C02S, 65816";
}

const char *TableMos6502::getCpu() {
    if (_cpuType == MOS6502) return "6502";
    if (_cpuType == W65SC02) return "65SC02";
    if (_cpuType == R65C02)  return "65C02";
    return _cpuType == W65C02S ? "W65C02S" : "65816";
}

bool TableMos6502::setCpu(const char *cpu) {
    const char *p;
    p = cpu + (strncasecmp(cpu, "MOS", 3) ? 0 : 3);
    if (strcmp(p, "6502") == 0)
        return setCpu(MOS6502);
    p = cpu + (toupper(*cpu) == 'R' ? 1 : 0);
    if (strcasecmp(p, "65C02") == 0)
        return setCpu(R65C02);
    p = cpu + (toupper(*cpu) == 'W' ? 1 : 0);
    if (strcasecmp(p, "65SC02") == 0)
        return setCpu(W65SC02);
    if (strcasecmp(p, "65C02") == 0
        || strcasecmp(p, "65C02S") == 0)
        return setCpu(W65C02S);
    if (strcmp(p, "65816") == 0
        || strcasecmp(p, "65C816") == 0
        || strcasecmp(p, "65C816S") == 0)
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
        if (addrMode == IMMA) return _longAccumulator;
        if (addrMode == IMMX) return _longIndex;
    }
    return false;
}

class TableMos6502 TableMos6502;

} // namespace mos6502
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

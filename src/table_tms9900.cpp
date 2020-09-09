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

#include "config_tms9900.h"

#include "entry_tms9900.h"
#include "table_tms9900.h"
#include "text_tms9900.h"

#include <ctype.h>
#include <string.h>

namespace libasm {
namespace tms9900 {

#define E(_insn, _name, _amode)                     \
    { _insn, Entry::_flags(_amode), TEXT_##_name },

static constexpr Entry TABLE_TMS9900[] PROGMEM = {
    E(0x0200, LI,   REG_IMM)
    E(0x0220, AI,   REG_IMM)
    E(0x0240, ANDI, REG_IMM)
    E(0x0260, ORI,  REG_IMM)
    E(0x0280, CI,   REG_IMM)
    E(0x02A0, STWP, REG)
    E(0x02C0, STST, REG)
    E(0x02E0, LWPI, IMM)
    E(0x0300, LIMI, IMM)
    E(0x0340, IDLE, INH)
    E(0x0360, RSET, INH)
    E(0x0380, RTWP, INH)
    E(0x03A0, CKON, INH)
    E(0x03C0, CKOF, INH)
    E(0x03E0, LREX, INH)
    E(0x0400, BLWP, SRC)
    E(0x0440, B,    SRC)
    E(0x0480, X,    SRC)
    E(0x04C0, CLR,  SRC)
    E(0x0500, NEG,  SRC)
    E(0x0540, INV,  SRC)
    E(0x0580, INC,  SRC)
    E(0x05C0, INCT, SRC)
    E(0x0600, DEC,  SRC)
    E(0x0640, DECT, SRC)
    E(0x0680, BL,   SRC)
    E(0x06C0, SWPB, SRC)
    E(0x0700, SETO, SRC)
    E(0X0740, ABS,  SRC)
    E(0x0800, SRA,  CNT_REG)
    E(0x0900, SRL,  CNT_REG)
    E(0x0A00, SLA,  CNT_REG)
    E(0x0B00, SRC,  CNT_REG)
    E(0x1000, JMP,  REL)
    E(0x1100, JLT,  REL)
    E(0x1200, JLE,  REL)
    E(0x1300, JEQ,  REL)
    E(0x1400, JHE,  REL)
    E(0x1500, JGT,  REL)
    E(0x1600, JNE,  REL)
    E(0x1700, JNC,  REL)
    E(0x1800, JOC,  REL)
    E(0x1900, JNO,  REL)
    E(0x1A00, JL,   REL)
    E(0x1B00, JH,   REL)
    E(0x1C00, JOP,  REL)
    E(0x1D00, SBO,  CRU_OFF)
    E(0x1E00, SBZ,  CRU_OFF)
    E(0x1F00, TB,   CRU_OFF)
    E(0x2000, COC,  REG_SRC)
    E(0x2400, CZC,  REG_SRC)
    E(0x2800, XOR,  REG_SRC)
    E(0x2C00, XOP,  XOP_SRC)
    E(0x3000, LDCR, CNT_SRC)
    E(0x3400, STCR, CNT_SRC)
    E(0x3800, MPY,  REG_SRC)
    E(0x3C00, DIV,  REG_SRC)
    E(0x4000, SZC,  DST_SRC)
    E(0x5000, SZCB, DST_SRC)
    E(0x6000, S,    DST_SRC)
    E(0x7000, SB,   DST_SRC)
    E(0x8000, C,    DST_SRC)
    E(0x9000, CB,   DST_SRC)
    E(0xA000, A,    DST_SRC)
    E(0xB000, AB,   DST_SRC)
    E(0xC000, MOV,  DST_SRC)
    E(0xD000, MOVB, DST_SRC)
    E(0xE000, SOC,  DST_SRC)
    E(0xF000, SOCB, DST_SRC)
};

static constexpr Entry TABLE_TMS9995[] PROGMEM = {
    E(0x0080, LST,  REG)
    E(0x0090, LWP,  REG)
    E(0x0180, DIVS, SRC)
    E(0x01C0, MPYS, SRC)
};

static constexpr Entry TABLE_TMS99105[] PROGMEM = {
    E(0x001C, SRAM, DW_CNT_SRC)
    E(0x001D, SLAM, DW_CNT_SRC)
    E(0x0029, SM,   DW_DST_SRC)
    E(0x002A, AM,   DW_DST_SRC)
    E(0x00B0, BLSK, REG_IMM)
    E(0x0140, BIND, SRC)
    E(0x0C09, TMB,  DW_BIT_SRC)
    E(0x0C0A, TCMB, DW_BIT_SRC)
    E(0x0C0B, TSMB, DW_BIT_SRC)
    E(0x0100, EVAD, SRC)
    E(0x0380, RTWP, IMM_MOD)
    E(0x0381, RTWP, IMM_MOD)
    E(0x0382, RTWP, IMM_MOD)
    E(0x0384, RTWP, IMM_MOD)
};

struct TableTms9900::EntryPage {
    const Entry *table;
    const Entry *end;
};

static constexpr TableTms9900::EntryPage TMS9900_PAGES[] PROGMEM = {
    { ARRAY_RANGE(TABLE_TMS9900) },
};

static constexpr TableTms9900::EntryPage TMS9995_PAGES[] PROGMEM = {
    { ARRAY_RANGE(TABLE_TMS9900) },
    { ARRAY_RANGE(TABLE_TMS9995) },
};

static constexpr TableTms9900::EntryPage TMS99105_PAGES[] PROGMEM = {
    { ARRAY_RANGE(TABLE_TMS99105) },
    { ARRAY_RANGE(TABLE_TMS9900) },
    { ARRAY_RANGE(TABLE_TMS9995) },
};

Error TableTms9900::searchName(
    InsnTms9900 &insn, const EntryPage *pages, const EntryPage *end) {
    for (const EntryPage *page = pages; page < end; page++) {
        const Entry *table = reinterpret_cast<Entry *>(pgm_read_ptr(&page->table));
        const Entry *end = reinterpret_cast<Entry *>(pgm_read_ptr(&page->end));
        const Entry *entry = TableBase::searchName<Entry>(insn.name(), table, end);
        if (entry) {
            insn.setOpCode(pgm_read_word(&entry->opCode));
            insn.setFlags(pgm_read_byte(&entry->flags));
            return OK;
        }
    }
    return UNKNOWN_INSTRUCTION;
}

static Config::opcode_t maskCode(
    Config::opcode_t opCode, const Entry *entry) {
    const AddrMode addrMode = Entry::_addrMode(pgm_read_byte(&entry->flags));
    switch (addrMode) {
    case REG:
    case REG_IMM:
        return opCode & ~0x000f;
    case SRC:
        return opCode & ~0x003f;
    case REL:
    case CNT_REG:
    case CRU_OFF:
        return opCode & ~0x00ff;
    case CNT_SRC:
    case XOP_SRC:
    case REG_SRC:
        return opCode & ~0x03ff;
    case DST_SRC:
        return opCode & ~0x0fff;
    default:
        return opCode;
    }
}

Error TableTms9900::searchOpCode(
    InsnTms9900 &insn, const EntryPage *pages, const EntryPage *end) {
    for (const EntryPage *page = pages; page < end; page++) {
        const Entry *table = reinterpret_cast<Entry *>(pgm_read_ptr(&page->table));
        const Entry *end = reinterpret_cast<Entry *>(pgm_read_ptr(&page->end));
        const Entry *entry = TableBase::searchCode<Entry,Config::opcode_t>(
            insn.opCode(), table, end, maskCode);
        if (entry) {
            insn.setFlags(pgm_read_byte(&entry->flags));
            const char *name =
                reinterpret_cast<const char *>(pgm_read_ptr(&entry->name));
            TableBase::setName(insn.insn(), name, Config::NAME_MAX);
            return OK;
        }
    }
    return UNKNOWN_INSTRUCTION;
}

Error TableTms9900::searchName(InsnTms9900 &insn) const {
    return _error.setError(searchName(insn, _table, _end));
}

Error TableTms9900::searchOpCode(InsnTms9900 &insn) const {
    return _error.setError(searchOpCode(insn, _table, _end));
}

TableTms9900::TableTms9900() {
    setCpu(TMS9900);
}

bool TableTms9900::setCpu(CpuType cpuType) {
    _cpuType = cpuType;
    if (cpuType == TMS9900) {
        _table = ARRAY_BEGIN(TMS9900_PAGES);
        _end = ARRAY_END(TMS9900_PAGES);
        return true;
    }
    if (cpuType == TMS9995) {
        _table = ARRAY_BEGIN(TMS9995_PAGES);
        _end = ARRAY_END(TMS9995_PAGES);
        return true;
    }
    if (cpuType == TMS99105) {
        _table = ARRAY_BEGIN(TMS99105_PAGES);
        _end =  ARRAY_END(TMS99105_PAGES);
        return true;
    }
    return false;
}

const char *TableTms9900::listCpu() const {
    return "TMS9900, TMS9995, TMS99105";
}

const char *TableTms9900::getCpu() const {
    if (_cpuType == TMS99105) return "99105";
    return _cpuType == TMS9900 ? "9900" : "9995";
}

bool TableTms9900::setCpu(const char *cpu) {
    if (strncasecmp(cpu, "tms", 3) == 0)
        cpu += 3;
    if (strcmp(cpu, "9900") == 0)
        return setCpu(TMS9900);
    if (strcmp(cpu, "9995") == 0)
        return setCpu(TMS9995);
    if (strcmp(cpu, "99105") == 0)
        return setCpu(TMS99105);
    return false;
}

class TableTms9900 TableTms9900;

} // namespace tms9900
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

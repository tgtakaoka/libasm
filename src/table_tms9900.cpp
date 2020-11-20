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

#define E(_opc, _name, _src, _dst) \
    { _opc, Entry::Flags::create(_src, _dst), _name }

// clang-format off
static constexpr Entry TABLE_TMS9900[] PROGMEM = {
    E(0x0200, TEXT_LI,   M_REG,  M_IMM),
    E(0x0220, TEXT_AI,   M_REG,  M_IMM),
    E(0x0240, TEXT_ANDI, M_REG,  M_IMM),
    E(0x0260, TEXT_ORI,  M_REG,  M_IMM),
    E(0x0280, TEXT_CI,   M_REG,  M_IMM),
    E(0x02A0, TEXT_STWP, M_REG,  M_NO),
    E(0x02C0, TEXT_STST, M_REG,  M_NO),
    E(0x02E0, TEXT_LWPI, M_IMM,  M_NO),
    E(0x0300, TEXT_LIMI, M_IMM,  M_NO),
    E(0x0340, TEXT_IDLE, M_NO,   M_NO),
    E(0x0360, TEXT_RSET, M_NO,   M_NO),
    E(0x0380, TEXT_RTWP, M_NO,   M_NO),
    E(0x03A0, TEXT_CKON, M_NO,   M_NO),
    E(0x03C0, TEXT_CKOF, M_NO,   M_NO),
    E(0x03E0, TEXT_LREX, M_NO,   M_NO),
    E(0x0400, TEXT_BLWP, M_SRC,  M_NO),
    E(0x0440, TEXT_B,    M_SRC,  M_NO),
    E(0x0480, TEXT_X,    M_SRC,  M_NO),
    E(0x04C0, TEXT_CLR,  M_SRC,  M_NO),
    E(0x0500, TEXT_NEG,  M_SRC,  M_NO),
    E(0x0540, TEXT_INV,  M_SRC,  M_NO),
    E(0x0580, TEXT_INC,  M_SRC,  M_NO),
    E(0x05C0, TEXT_INCT, M_SRC,  M_NO),
    E(0x0600, TEXT_DEC,  M_SRC,  M_NO),
    E(0x0640, TEXT_DECT, M_SRC,  M_NO),
    E(0x0680, TEXT_BL,   M_SRC,  M_NO),
    E(0x06C0, TEXT_SWPB, M_SRC,  M_NO),
    E(0x0700, TEXT_SETO, M_SRC,  M_NO),
    E(0x0740, TEXT_ABS,  M_SRC,  M_NO),
    E(0x0800, TEXT_SRA,  M_REG,  M_SCNT),
    E(0x0900, TEXT_SRL,  M_REG,  M_SCNT),
    E(0x0A00, TEXT_SLA,  M_REG,  M_SCNT),
    E(0x0B00, TEXT_SRC,  M_REG,  M_SCNT),
    E(0x1000, TEXT_JMP,  M_REL,  M_NO),
    E(0x1100, TEXT_JLT,  M_REL,  M_NO),
    E(0x1200, TEXT_JLE,  M_REL,  M_NO),
    E(0x1300, TEXT_JEQ,  M_REL,  M_NO),
    E(0x1400, TEXT_JHE,  M_REL,  M_NO),
    E(0x1500, TEXT_JGT,  M_REL,  M_NO),
    E(0x1600, TEXT_JNE,  M_REL,  M_NO),
    E(0x1700, TEXT_JNC,  M_REL,  M_NO),
    E(0x1800, TEXT_JOC,  M_REL,  M_NO),
    E(0x1900, TEXT_JNO,  M_REL,  M_NO),
    E(0x1A00, TEXT_JL,   M_REL,  M_NO),
    E(0x1B00, TEXT_JH,   M_REL,  M_NO),
    E(0x1C00, TEXT_JOP,  M_REL,  M_NO),
    E(0x1D00, TEXT_SBO,  M_CRU,  M_NO),
    E(0x1E00, TEXT_SBZ,  M_CRU,  M_NO),
    E(0x1F00, TEXT_TB,   M_CRU,  M_NO),
    E(0x2000, TEXT_COC,  M_SRC,  M_DREG),
    E(0x2400, TEXT_CZC,  M_SRC,  M_DREG),
    E(0x2800, TEXT_XOR,  M_SRC,  M_DREG),
    E(0x2C00, TEXT_XOP,  M_SRC,  M_XOP),
    E(0x3000, TEXT_LDCR, M_SRC,  M_CNT),
    E(0x3400, TEXT_STCR, M_SRC,  M_CNT),
    E(0x3800, TEXT_MPY,  M_SRC,  M_DREG),
    E(0x3C00, TEXT_DIV,  M_SRC,  M_DREG),
    E(0x4000, TEXT_SZC,  M_SRC,  M_DST),
    E(0x5000, TEXT_SZCB, M_SRC,  M_DST),
    E(0x6000, TEXT_S,    M_SRC,  M_DST),
    E(0x7000, TEXT_SB,   M_SRC,  M_DST),
    E(0x8000, TEXT_C,    M_SRC,  M_DST),
    E(0x9000, TEXT_CB,   M_SRC,  M_DST),
    E(0xA000, TEXT_A,    M_SRC,  M_DST),
    E(0xB000, TEXT_AB,   M_SRC,  M_DST),
    E(0xC000, TEXT_MOV,  M_SRC,  M_DST),
    E(0xD000, TEXT_MOVB, M_SRC,  M_DST),
    E(0xE000, TEXT_SOC,  M_SRC,  M_DST),
    E(0xF000, TEXT_SOCB, M_SRC,  M_DST),
};

static constexpr Entry TABLE_TMS9995[] PROGMEM = {
    E(0x0080, TEXT_LST,  M_REG,  M_NO),
    E(0x0090, TEXT_LWP,  M_REG,  M_NO),
    E(0x0180, TEXT_DIVS, M_SRC,  M_NO),
    E(0x01C0, TEXT_MPYS, M_SRC,  M_NO),
};

static constexpr Entry TABLE_TMS99105[] PROGMEM = {
    E(0x001C, TEXT_SRAM, M_SRC2, M_CNT2),
    E(0x001D, TEXT_SLAM, M_SRC2, M_CNT2),
    E(0x0029, TEXT_SM,   M_SRC2, M_DST2),
    E(0x002A, TEXT_AM,   M_SRC2, M_DST2),
    E(0x00B0, TEXT_BLSK, M_REG,  M_IMM),
    E(0x0140, TEXT_BIND, M_SRC,  M_NO),
    E(0x0C09, TEXT_TMB,  M_SRC2, M_BIT2),
    E(0x0C0A, TEXT_TCMB, M_SRC2, M_BIT2),
    E(0x0C0B, TEXT_TSMB, M_SRC2, M_BIT2),
    E(0x0100, TEXT_EVAD, M_SRC,  M_NO),
    E(0x0380, TEXT_RTWP, M_RTWP, M_NO),
};
// clang-format on

class TableTms9900::EntryPage : public EntryPageBase<Entry> {
public:
    constexpr EntryPage(const Entry *table, const Entry *end)
        : EntryPageBase(table, end) {}
};

static constexpr TableTms9900::EntryPage TMS9900_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_TMS9900)},
};

static constexpr TableTms9900::EntryPage TMS9995_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_TMS9900)},
        {ARRAY_RANGE(TABLE_TMS9995)},
};

static constexpr TableTms9900::EntryPage TMS99105_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_TMS99105)},
        {ARRAY_RANGE(TABLE_TMS9900)},
        {ARRAY_RANGE(TABLE_TMS9995)},
};

static bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_IREG || opr == M_INCR || opr == M_SYBL || opr == M_INDX)
        return table == M_SRC || table == M_DST || table == M_SRC2 ||
               table == M_DST2;
    if (opr == M_REG)
        return table == M_SRC || table == M_DST || table == M_SRC2 ||
               table == M_DST2 || table == M_DREG || table == M_SCNT ||
               table == M_CNT2;
    if (opr == M_IMM)
        return table == M_REL || table == M_SCNT || table == M_CNT ||
               table == M_CNT2 || table == M_CRU || table == M_BIT2 ||
               table == M_XOP || table == M_RTWP;
    return false;
}

static bool acceptModes(Entry::Flags flags, const Entry *entry) {
    const Entry::Flags table = entry->flags();
    return acceptMode(flags.srcMode(), table.srcMode()) &&
           acceptMode(flags.dstMode(), table.dstMode());
}

Error TableTms9900::searchName(
        InsnTms9900 &insn, const EntryPage *pages, const EntryPage *end) const {
    uint8_t count = 0;
    for (const EntryPage *page = pages; page < end; page++) {
        const Entry *entry = TableBase::searchName<Entry, Entry::Flags>(
                insn.name(), insn.flags(), page->table(), page->end(),
                acceptModes, count);
        if (entry) {
            insn.setOpCode(entry->opCode());
            insn.setFlags(entry->flags());
            return OK;
        }
    }
    return count == 0 ? UNKNOWN_INSTRUCTION : OPERAND_NOT_ALLOWED;
}

static Config::opcode_t maskCode(Config::opcode_t opCode, const Entry *entry) {
    const AddrMode src = entry->flags().srcMode();
    const AddrMode dst = entry->flags().dstMode();
    Config::opcode_t mask = 0;
    if (src == M_REG || dst == M_REG)
        mask |= 0xF;
    if (src == M_SRC)
        mask |= 0x3F;
    if (dst == M_DST)
        mask |= 0xFC0;
    if (dst == M_SCNT)
        mask |= 0xF0;
    if (dst == M_CNT || dst == M_XOP || dst == M_DREG)
        mask |= 0x3C0;
    if (src == M_REL || src == M_CRU)
        mask |= 0xFF;
    if (src == M_RTWP)
        mask |= 7;
    return opCode & ~mask;
}

Error TableTms9900::searchOpCode(
        InsnTms9900 &insn, const EntryPage *pages, const EntryPage *end) const {
    for (const EntryPage *page = pages; page < end; page++) {
        const Entry *entry = TableBase::searchCode<Entry, Config::opcode_t>(
                insn.opCode(), page->table(), page->end(), maskCode);
        if (entry) {
            insn.setFlags(entry->flags());
            insn.setName_P(entry->name());
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
        _end = ARRAY_END(TMS99105_PAGES);
        return true;
    }
    return false;
}

const char *TableTms9900::listCpu() const {
    return TEXT_CPU_LIST;
}

const char *TableTms9900::getCpu() const {
    if (_cpuType == TMS99105)
        return TEXT_CPU_99105;
    return _cpuType == TMS9900 ? TEXT_CPU_9900 : TEXT_CPU_9995;
}

bool TableTms9900::setCpu(const char *cpu) {
    if (strncasecmp_P(cpu, TEXT_CPU_TMS, 3) == 0)
        cpu += 3;
    if (strcmp_P(cpu, TEXT_CPU_9900) == 0)
        return setCpu(TMS9900);
    if (strcmp_P(cpu, TEXT_CPU_9995) == 0)
        return setCpu(TMS9995);
    if (strcmp_P(cpu, TEXT_CPU_99105) == 0)
        return setCpu(TMS99105);
    return false;
}

class TableTms9900 TableTms9900;

}  // namespace tms9900
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

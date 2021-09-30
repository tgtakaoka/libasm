/*
 * Copyright 2021 Tadashi G. Takaoka
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

#include "table_i8048.h"

#include <ctype.h>
#include <string.h>

#include "config_i8048.h"
#include "entry_i8048.h"
#include "text_i8048.h"

#include <stdio.h>

namespace libasm {
namespace i8048 {

#define E2(_opc, _name, _dst, _src) \
    { _opc, Entry::Flags::create(_dst, _src), _name }
#define E1(_opc, _name, _dst) E2(_opc, _name, _dst, M_NONE)
#define E0(_opc, _name) E1(_opc, _name, M_NONE)

// clang-format off
static constexpr Entry TABLE_I8039[] PROGMEM = {
    E2(0x68, TEXT_ADD,  M_A,    M_R),
    E2(0x60, TEXT_ADD,  M_A,    M_IR),
    E2(0x03, TEXT_ADD,  M_A,    M_IMM8),
    E2(0x78, TEXT_ADDC, M_A,    M_R),
    E2(0x70, TEXT_ADDC, M_A,    M_IR),
    E2(0x13, TEXT_ADDC, M_A,    M_IMM8),
    E2(0x58, TEXT_ANL,  M_A,    M_R),
    E2(0x50, TEXT_ANL,  M_A,    M_IR),
    E2(0x53, TEXT_ANL,  M_A,    M_BIT8),
    E2(0x48, TEXT_ORL,  M_A,    M_R),
    E2(0x40, TEXT_ORL,  M_A,    M_IR),
    E2(0x43, TEXT_ORL,  M_A,    M_BIT8),
    E2(0xD8, TEXT_XRL,  M_A,    M_R),
    E2(0xD0, TEXT_XRL,  M_A,    M_IR),
    E2(0xD3, TEXT_XRL,  M_A,    M_BIT8),
    E1(0x17, TEXT_INC,  M_A),
    E1(0x07, TEXT_DEC,  M_A),
    E1(0x27, TEXT_CLR,  M_A),
    E1(0x37, TEXT_CPL,  M_A),
    E1(0x57, TEXT_DA,   M_A),
    E1(0x47, TEXT_SWAP, M_A),
    E1(0xE7, TEXT_RL,   M_A),
    E1(0xF7, TEXT_RLC,  M_A),
    E1(0x77, TEXT_RR,   M_A),
    E1(0x67, TEXT_RRC,  M_A),
    E2(0x08, TEXT_IN,   M_A,    M_P12),
    E2(0x38, TEXT_OUTL, M_P12,  M_A),
    E2(0x98, TEXT_ANL,  M_P12,  M_BIT8),
    E2(0x88, TEXT_ORL,  M_P12,  M_BIT8),
    E2(0x0C, TEXT_MOVD, M_A,    M_PEXT),
    E2(0x3C, TEXT_MOVD, M_PEXT, M_A),
    E2(0x9C, TEXT_ANLD, M_PEXT, M_A),
    E2(0x8C, TEXT_ORLD, M_PEXT, M_A),
    E1(0x18, TEXT_INC,  M_R),
    E1(0x10, TEXT_INC,  M_IR),
    E1(0xC8, TEXT_DEC,  M_R),
    E1(0x04, TEXT_JMP,  M_AD11),
    E1(0xB3, TEXT_JMPP, M_IA),
    E2(0xE8, TEXT_DJNZ, M_R,    M_AD08),
    E1(0xF6, TEXT_JC,   M_AD08),
    E1(0xE6, TEXT_JNC,  M_AD08),
    E1(0xC6, TEXT_JZ,   M_AD08),
    E1(0x96, TEXT_JNZ,  M_AD08),
    E1(0x36, TEXT_JT0,  M_AD08),
    E1(0x26, TEXT_JNT0, M_AD08),
    E1(0x56, TEXT_JT1,  M_AD08),
    E1(0x46, TEXT_JNT1, M_AD08),
    E1(0xB6, TEXT_JF0,  M_AD08),
    E1(0x76, TEXT_JF1,  M_AD08),
    E1(0x16, TEXT_JTF,  M_AD08),
    E1(0x86, TEXT_JNI,  M_AD08),
    E2(0x12, TEXT_JB,   M_BITN,  M_AD08),
    E1(0x14, TEXT_CALL, M_AD11),
    E0(0x83, TEXT_RET),
    E0(0x93, TEXT_RETR),
    E1(0x97, TEXT_CLR,  M_C),
    E1(0xA7, TEXT_CPL,  M_C),
    E1(0x85, TEXT_CLR,  M_F),
    E1(0x95, TEXT_CPL,  M_F),
    E2(0xF8, TEXT_MOV,  M_A,    M_R),
    E2(0xF0, TEXT_MOV,  M_A,    M_IR),
    E2(0x23, TEXT_MOV,  M_A,    M_IMM8),
    E2(0xA8, TEXT_MOV,  M_R,    M_A),
    E2(0xA0, TEXT_MOV,  M_IR,   M_A),
    E2(0xB8, TEXT_MOV,  M_R,    M_IMM8),
    E2(0xB0, TEXT_MOV,  M_IR,   M_IMM8),
    E2(0xC7, TEXT_MOV,  M_A,    M_PSW),
    E2(0xD7, TEXT_MOV,  M_PSW,  M_A),
    E2(0x28, TEXT_XCH,  M_A,    M_R),
    E2(0x20, TEXT_XCH,  M_A,    M_IR),
    E2(0x30, TEXT_XCHD, M_A,    M_IR),
    E2(0x90, TEXT_MOVX, M_IR,   M_A),
    E2(0x80, TEXT_MOVX, M_A,    M_IR),
    E2(0xA3, TEXT_MOVP, M_A,    M_IA),
    E2(0xE3, TEXT_MOVP3, M_A,   M_IA),
    E2(0x42, TEXT_MOV,  M_A,    M_T),
    E2(0x62, TEXT_MOV,  M_T,    M_A),
    E1(0x55, TEXT_STRT, M_T),
    E1(0x45, TEXT_STRT, M_CNT),
    E1(0x65, TEXT_STOP, M_TCNT),
    E1(0x25, TEXT_EN,   M_TCNTI),
    E1(0x35, TEXT_DIS,  M_TCNTI),
    E1(0x05, TEXT_EN,   M_I),
    E1(0x15, TEXT_DIS,  M_I),
    E1(0xC5, TEXT_SEL,  M_RB),
    E1(0xE5, TEXT_SEL,  M_MB),
    E1(0x75, TEXT_ENT0, M_CLK),
    E0(0x00, TEXT_NOP),
};

static constexpr Entry TABLE_I8048[] PROGMEM = {
    E2(0x08, TEXT_INS,  M_A,    M_BUS),
    E2(0x02, TEXT_OUTL, M_BUS,  M_A),
    E2(0x98, TEXT_ANL,  M_BUS,  M_BIT8),
    E2(0x88, TEXT_ORL,  M_BUS,  M_BIT8),
};

static constexpr Entry TABLE_I80C48[] PROGMEM = {
    E0(0x01, TEXT_HALT),
};

static constexpr Entry TABLE_MSM80C48[] PROGMEM = {
    E2(0x63, TEXT_MOV,   M_A,  M_P1),
    E2(0x73, TEXT_MOV,   M_A,  M_P2),
    E1(0xC0, TEXT_DEC,   M_IR),
    E2(0xE0, TEXT_DJNZ,  M_IR, M_AD08),
    E2(0xC3, TEXT_MOVP1, M_P,  M_IR3),
    E2(0xF3, TEXT_MOV,   M_P1, M_IR3),
    E0(0x82, TEXT_HLTS),
    E0(0xA2, TEXT_FLT),
    E0(0xC2, TEXT_FLTT),
    E0(0xE2, TEXT_FRES),
};
// clang-format on

class TableI8048::EntryPage : public EntryPageBase<Entry> {
public:
    constexpr EntryPage(const Entry *table, const Entry *end) : EntryPageBase(table, end) {}
};

static constexpr TableI8048::EntryPage I8039_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_I8039)},
};

static constexpr TableI8048::EntryPage I8048_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_I8048)},
        {ARRAY_RANGE(TABLE_I8039)},
};

static constexpr TableI8048::EntryPage I80C39_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_I8039)},
        {ARRAY_RANGE(TABLE_I80C48)},
};

static constexpr TableI8048::EntryPage I80C48_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_I8048)},
        {ARRAY_RANGE(TABLE_I8039)},
        {ARRAY_RANGE(TABLE_I80C48)},
};

static constexpr TableI8048::EntryPage MSM80C39_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_I8039)},
        {ARRAY_RANGE(TABLE_I80C48)},
        {ARRAY_RANGE(TABLE_MSM80C48)},
};

static constexpr TableI8048::EntryPage MSM80C48_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_I8048)},
        {ARRAY_RANGE(TABLE_I8039)},
        {ARRAY_RANGE(TABLE_I80C48)},
        {ARRAY_RANGE(TABLE_MSM80C48)},
};

static bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_IMM8)
        return table == M_BIT8;
    if (opr == M_AD11)
        return table == M_AD08 || table == M_BITN;
    if (opr == M_IR3)
        return table == M_IR;
    if (opr == M_P1 || opr == M_P2)
        return table == M_P12;
    return false;
}

static bool acceptModes(Entry::Flags flags, const Entry *entry) {
    const Entry::Flags table = entry->flags();
    return acceptMode(flags.dstMode(), table.dstMode()) &&
           acceptMode(flags.srcMode(), table.srcMode());
}

Error TableI8048::searchName(InsnI8048 &insn, const EntryPage *pages, const EntryPage *end) const {
    uint8_t count = 0;
    for (const auto *page = pages; page < end; page++) {
        const Entry *entry = TableBase::searchName<Entry, Entry::Flags>(
                insn.name(), insn.flags(), page->table(), page->end(), acceptModes, count);
        if (entry) {
            insn.setOpCode(entry->opCode());
            insn.setFlags(entry->flags());
            return OK;
        }
    }
    return count == 0 ? UNKNOWN_INSTRUCTION : OPERAND_NOT_ALLOWED;
}

static Config::opcode_t tableCode(Config::opcode_t opCode, const Entry *entry) {
    const Entry::Flags flags = entry->flags();
    const AddrMode dst = flags.dstMode();
    const AddrMode src = flags.srcMode();
    Config::opcode_t mask = 0;
    if (dst == M_R || src == M_R)
        mask |= 7;
    if (dst == M_IR || src == M_IR)
        mask |= 1;
    if (dst == M_P12 || src == M_P12 || dst == M_PEXT || src == M_PEXT)
        mask |= 3;
    if (dst == M_AD11 || dst == M_BITN)
        mask |= 0xE0;
    if (dst == M_F)
        mask |= 0x20;
    if (dst == M_RB || dst == M_MB)
        mask |= 0x10;
    return opCode & ~mask;
}

const Entry *TableI8048::searchOpCode(
        InsnI8048 &insn, const EntryPage *pages, const EntryPage *end) const {
    for (const auto *page = pages; page < end; page++) {
        const Entry *entry = TableBase::searchCode<Entry, Config::opcode_t>(
                insn.opCode(), page->table(), page->end(), tableCode);
        if (entry) {
            insn.setFlags(entry->flags());
            insn.setName_P(entry->name());
            return entry;
        }
    }
    return nullptr;
}

Error TableI8048::searchName(InsnI8048 &insn) const {
    return setError(searchName(insn, _table, _end));
}

Error TableI8048::searchOpCode(InsnI8048 &insn) const {
    const Entry *entry = searchOpCode(insn, _table, _end);
    return setError(entry ? OK : UNKNOWN_INSTRUCTION);
}

class CpuTable : public CpuTableBase<CpuType, TableI8048::EntryPage> {
public:
    constexpr CpuTable(CpuType cpuType, const char *name, const TableI8048::EntryPage *table,
            const TableI8048::EntryPage *end)
        : CpuTableBase(cpuType, name, table, end) {}
};

static constexpr CpuTable CPU_TABLES[] PROGMEM = {
        {I8039, TEXT_CPU_8039, ARRAY_RANGE(I8039_PAGES)},
        {I8048, TEXT_CPU_8048, ARRAY_RANGE(I8048_PAGES)},
        {I80C39, TEXT_CPU_80C39, ARRAY_RANGE(I80C39_PAGES)},
        {I80C48, TEXT_CPU_80C48, ARRAY_RANGE(I80C48_PAGES)},
        {MSM80C39, TEXT_CPU_MSM80C39, ARRAY_RANGE(MSM80C39_PAGES)},
        {MSM80C48, TEXT_CPU_MSM80C48, ARRAY_RANGE(MSM80C48_PAGES)},
};

TableI8048::TableI8048() {
    setCpu(I8048);
}

bool TableI8048::setCpu(CpuType cpuType) {
    const auto *t = CpuTable::search(cpuType, ARRAY_RANGE(CPU_TABLES));
    if (t == nullptr)
        return false;
    _cpuType = cpuType;
    _table = t->table();
    _end = t->end();
    return true;
}

const char *TableI8048::listCpu() const {
    return TEXT_CPU_LIST;
}

const char *TableI8048::getCpu() const {
    return CpuTable::search(_cpuType, ARRAY_RANGE(CPU_TABLES))->name();
}

bool TableI8048::setCpu(const char *cpu) {
    const char *p = cpu;
    if (toupper(*p) == 'I')
        p++;
    const auto *t = CpuTable::search(p, ARRAY_RANGE(CPU_TABLES));
    if (t == nullptr)
        return false;
    if (strncasecmp_P(cpu + 1, TEXT_CPU_MSM, 3) == 0)
        return false;
    return setCpu(t->cpuType());
}

class TableI8048 TableI8048;

}  // namespace i8048
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

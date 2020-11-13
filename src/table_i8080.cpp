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

#include "config_i8080.h"

#include "entry_i8080.h"
#include "table_i8080.h"
#include "text_i8080.h"

#include <ctype.h>
#include <string.h>

namespace libasm {
namespace i8080 {

#define E(_opc, _name, _dst, _src)                              \
    { _opc, Entry::Flags::create(_dst, _src), TEXT_##_name },

static constexpr Entry TABLE_I8080[] PROGMEM = {
    E(0x00, NOP,  M_NO,   M_NO)
    E(0x01, LXI,  M_PTR,  M_IM16)
    E(0x09, DAD,  M_PTR,  M_NO)
    E(0x02, STAX, M_IDX,  M_NO)
    E(0x22, SHLD, M_ABS,  M_NO)
    E(0x32, STA,  M_ABS,  M_NO)
    E(0x0A, LDAX, M_IDX,  M_NO)
    E(0x2A, LHLD, M_ABS,  M_NO)
    E(0x3A, LDA,  M_ABS,  M_NO)
    E(0x03, INX,  M_PTR,  M_NO)
    E(0x0B, DCX,  M_PTR,  M_NO)
    E(0x04, INR,  M_DST,  M_NO)
    E(0x05, DCR,  M_DST,  M_NO)
    E(0x06, MVI,  M_DST,  M_IM8)
    E(0x07, RLC,  M_NO,   M_NO)
    E(0x0F, RRC,  M_NO,   M_NO)
    E(0x17, RAL,  M_NO,   M_NO)
    E(0x1F, RAR,  M_NO,   M_NO)
    E(0x27, DAA,  M_NO,   M_NO)
    E(0x2F, CMA,  M_NO,   M_NO)
    E(0x37, STC,  M_NO,   M_NO)
    E(0x3F, CMC,  M_NO,   M_NO)
    E(0x76, HLT,  M_NO,   M_NO)
    E(0x40, MOV,  M_DST,  M_REG)
    E(0x80, ADD,  M_REG,  M_NO)
    E(0x88, ADC,  M_REG,  M_NO)
    E(0x90, SUB,  M_REG,  M_NO)
    E(0x98, SBB,  M_REG,  M_NO)
    E(0xA0, ANA,  M_REG,  M_NO)
    E(0xA8, XRA,  M_REG,  M_NO)
    E(0xB0, ORA,  M_REG,  M_NO)
    E(0xB8, CMP,  M_REG,  M_NO)
    E(0xC0, RNZ,  M_NO,   M_NO)
    E(0xC8, RZ,   M_NO,   M_NO)
    E(0xD0, RNC,  M_NO,   M_NO)
    E(0xD8, RC,   M_NO,   M_NO)
    E(0xE0, RPO,  M_NO,   M_NO)
    E(0xE8, RPE,  M_NO,   M_NO)
    E(0xF0, RP,   M_NO,   M_NO)
    E(0xF8, RM,   M_NO,   M_NO)
    E(0xC1, POP,  M_STK,  M_NO)
    E(0xC9, RET,  M_NO,   M_NO)
    E(0xE9, PCHL, M_NO,   M_NO)
    E(0xF9, SPHL, M_NO,   M_NO)
    E(0xC2, JNZ,  M_ABS,  M_NO)
    E(0xCA, JZ,   M_ABS,  M_NO)
    E(0xD2, JNC,  M_ABS,  M_NO)
    E(0xDA, JC,   M_ABS,  M_NO)
    E(0xE2, JPO,  M_ABS,  M_NO)
    E(0xEA, JPE,  M_ABS,  M_NO)
    E(0xF2, JP,   M_ABS,  M_NO)
    E(0xFA, JM,   M_ABS,  M_NO)
    E(0xC3, JMP,  M_ABS,  M_NO)
    E(0xD3, OUT,  M_IOA,  M_NO)
    E(0xDB, IN,   M_IOA,  M_NO)
    E(0xE3, XTHL, M_NO,   M_NO)
    E(0xEB, XCHG, M_NO,   M_NO)
    E(0xF3, DI,   M_NO,   M_NO)
    E(0xFB, EI,   M_NO,   M_NO)
    E(0xC4, CNZ,  M_ABS,  M_NO)
    E(0xCC, CZ,   M_ABS,  M_NO)
    E(0xD4, CNC,  M_ABS,  M_NO)
    E(0xDC, CC,   M_ABS,  M_NO)
    E(0xE4, CPO,  M_ABS,  M_NO)
    E(0xEC, CPE,  M_ABS,  M_NO)
    E(0xF4, CP,   M_ABS,  M_NO)
    E(0xFC, CM,   M_ABS,  M_NO)
    E(0xC5, PUSH, M_STK,  M_NO)
    E(0xCD, CALL, M_ABS,  M_NO)
    E(0xC6, ADI,  M_IM8,  M_NO)
    E(0xCE, ACI,  M_IM8,  M_NO)
    E(0xD6, SUI,  M_IM8,  M_NO)
    E(0xDE, SBI,  M_IM8,  M_NO)
    E(0xE6, ANI,  M_IM8,  M_NO)
    E(0xEE, XRI,  M_IM8,  M_NO)
    E(0xF6, ORI,  M_IM8,  M_NO)
    E(0xFE, CPI,  M_IM8,  M_NO)
    E(0xC7, RST,  M_VEC,  M_NO)
};

static constexpr Entry TABLE_I8085[] PROGMEM = {
    E(0x20, RIM,  M_NO,   M_NO)
    E(0x30, SIM,  M_NO,   M_NO)
};

class TableI8080::EntryPage : public EntryPageBase<Entry> {
public:
    constexpr EntryPage(const Entry *table, const Entry *end)
        : EntryPageBase(table, end) {}
};

static constexpr TableI8080::EntryPage I8080_PAGES[] PROGMEM = {
    { ARRAY_RANGE(TABLE_I8080) },
};
static constexpr TableI8080::EntryPage I8085_PAGES[] PROGMEM = {
    { ARRAY_RANGE(TABLE_I8080) },
    { ARRAY_RANGE(TABLE_I8085) },
};

static bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_REG)
        return table == M_DST;
    if (opr == M_IDX || opr == M_REGH)
        return table == M_PTR || table == M_STK
            || table == M_REG || table == M_DST;
    if (opr == M_IM16)
        return table == M_IM8 || table == M_ABS || table == M_IOA
            || table == M_VEC;
    return false;
}

static bool acceptModes(Entry::Flags flags, const Entry *entry) {
    const Entry::Flags table = entry->flags();
    return acceptMode(flags.dstMode(), table.dstMode())
        && acceptMode(flags.srcMode(), table.srcMode());
}

Error TableI8080::searchName(
    InsnI8080 &insn, const EntryPage *pages, const EntryPage *end) const {
    uint8_t count = 0;
    for (const EntryPage *page = pages; page < end; page++) {
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

static Config::opcode_t tableCode(
    Config::opcode_t opCode, const Entry *entry) {
    const Entry::Flags flags = entry->flags();
    const AddrMode dst = flags.dstMode();
    const AddrMode src = flags.srcMode();
    Config::opcode_t mask = 0;
    if (dst == M_REG || src == M_REG) mask |= 07;
    if (dst == M_DST || dst == M_VEC) mask |= 070;
    if (dst == M_PTR || dst == M_STK) mask |= 0x30;
    if (dst == M_IDX) mask |= 0x10;
    return opCode & ~mask;
}

Error TableI8080::searchOpCode(
    InsnI8080 &insn, const EntryPage *pages, const EntryPage *end) const {
    for (const EntryPage *page = pages; page < end; page++) {
        const Entry *entry = TableBase::searchCode<Entry,Config::opcode_t>(
            insn.opCode(), page->table(), page->end(), tableCode);
        if (entry) {
            insn.setFlags(entry->flags());
            insn.setName_P(entry->name());
            return OK;
        }
    }
    return UNKNOWN_INSTRUCTION;
}

Error TableI8080::searchName(InsnI8080 &insn) const {
    return _error.setError(searchName(insn, _table, _end));
}

Error TableI8080::searchOpCode(InsnI8080 &insn) const {
    return _error.setError(searchOpCode(insn, _table, _end));
}

TableI8080::TableI8080() {
    setCpu(I8080);
}

bool TableI8080::setCpu(CpuType cpuType) {
    _cpuType = cpuType;
    _table = (cpuType == I8080)
        ? ARRAY_BEGIN(I8080_PAGES) : ARRAY_BEGIN(I8085_PAGES);
    _end = (cpuType == I8080)
        ? ARRAY_END(I8080_PAGES) : ARRAY_END(I8085_PAGES);
    return true;
}

const char *TableI8080::listCpu() const {
    return TEXT_CPU_LIST;
}

const char *TableI8080::getCpu() const {
    return _cpuType == I8080 ? TEXT_CPU_8080 : TEXT_CPU_8085;
}

bool TableI8080::setCpu(const char *cpu) {
    if (toupper(*cpu) == 'I') cpu++;
    if (strcmp_P(cpu, TEXT_CPU_8080) == 0)
        return setCpu(I8080);
    if (strcmp_P(cpu, TEXT_CPU_8085) == 0)
        return setCpu(I8085);
    return false;
}

class TableI8080 TableI8080;

} // namespace i8080
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

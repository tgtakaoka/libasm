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

#define E(_opc, _name, _amode, _iformat)                        \
    { _opc, Entry::_flags(_amode, _iformat), TEXT_##_name },

static constexpr Entry TABLE_I8080[] PROGMEM = {
    E(0x00, NOP,  INHR,   NO_FORMAT)
    E(0x01, LXI,  IMM16,  POINTER_REG)
    E(0x09, DAD,  INHR,   POINTER_REG)
    E(0x02, STAX, INHR,   INDEX_REG)
    E(0x22, SHLD, DIRECT, NO_FORMAT)
    E(0x32, STA,  DIRECT, NO_FORMAT)
    E(0x0A, LDAX, INHR,   INDEX_REG)
    E(0x2A, LHLD, DIRECT, NO_FORMAT)
    E(0x3A, LDA,  DIRECT, NO_FORMAT)
    E(0x03, INX,  INHR,   POINTER_REG)
    E(0x0B, DCX,  INHR,   POINTER_REG)
    E(0x04, INR,  INHR,   DATA_REG)
    E(0x05, DCR,  INHR,   DATA_REG)
    E(0x06, MVI,  IMM8,   DATA_REG)
    E(0x07, RLC,  INHR,   NO_FORMAT)
    E(0x0F, RRC,  INHR,   NO_FORMAT)
    E(0x17, RAL,  INHR,   NO_FORMAT)
    E(0x1F, RAR,  INHR,   NO_FORMAT)
    E(0x27, DAA,  INHR,   NO_FORMAT)
    E(0x2F, CMA,  INHR,   NO_FORMAT)
    E(0x37, STC,  INHR,   NO_FORMAT)
    E(0x3F, CMC,  INHR,   NO_FORMAT)
    E(0x76, HLT,  INHR,   NO_FORMAT)
    E(0x40, MOV,  INHR,   DATA_DATA_REG)
    E(0x80, ADD,  INHR,   LOW_DATA_REG)
    E(0x88, ADC,  INHR,   LOW_DATA_REG)
    E(0x90, SUB,  INHR,   LOW_DATA_REG)
    E(0x98, SBB,  INHR,   LOW_DATA_REG)
    E(0xA0, ANA,  INHR,   LOW_DATA_REG)
    E(0xA8, XRA,  INHR,   LOW_DATA_REG)
    E(0xB0, ORA,  INHR,   LOW_DATA_REG)
    E(0xB8, CMP,  INHR,   LOW_DATA_REG)
    E(0xC0, RNZ,  INHR,   NO_FORMAT)
    E(0xC8, RZ,   INHR,   NO_FORMAT)
    E(0xD0, RNC,  INHR,   NO_FORMAT)
    E(0xD8, RC,   INHR,   NO_FORMAT)
    E(0xE0, RPO,  INHR,   NO_FORMAT)
    E(0xE8, RPE,  INHR,   NO_FORMAT)
    E(0xF0, RP,   INHR,   NO_FORMAT)
    E(0xF8, RM,   INHR,   NO_FORMAT)
    E(0xC1, POP,  INHR,   STACK_REG)
    E(0xC9, RET,  INHR,   NO_FORMAT)
    E(0xE9, PCHL, INHR,   NO_FORMAT)
    E(0xF9, SPHL, INHR,   NO_FORMAT)
    E(0xC2, JNZ,  DIRECT, NO_FORMAT)
    E(0xCA, JZ,   DIRECT, NO_FORMAT)
    E(0xD2, JNC,  DIRECT, NO_FORMAT)
    E(0xDA, JC,   DIRECT, NO_FORMAT)
    E(0xE2, JPO,  DIRECT, NO_FORMAT)
    E(0xEA, JPE,  DIRECT, NO_FORMAT)
    E(0xF2, JP,   DIRECT, NO_FORMAT)
    E(0xFA, JM,   DIRECT, NO_FORMAT)
    E(0xC3, JMP,  DIRECT, NO_FORMAT)
    E(0xD3, OUT,  IOADR,  NO_FORMAT)
    E(0xDB, IN,   IOADR,  NO_FORMAT)
    E(0xE3, XTHL, INHR,   NO_FORMAT)
    E(0xEB, XCHG, INHR,   NO_FORMAT)
    E(0xF3, DI,   INHR,   NO_FORMAT)
    E(0xFB, EI,   INHR,   NO_FORMAT)
    E(0xC4, CNZ,  DIRECT, NO_FORMAT)
    E(0xCC, CZ,   DIRECT, NO_FORMAT)
    E(0xD4, CNC,  DIRECT, NO_FORMAT)
    E(0xDC, CC,   DIRECT, NO_FORMAT)
    E(0xE4, CPO,  DIRECT, NO_FORMAT)
    E(0xEC, CPE,  DIRECT, NO_FORMAT)
    E(0xF4, CP,   DIRECT, NO_FORMAT)
    E(0xFC, CM,   DIRECT, NO_FORMAT)
    E(0xC5, PUSH, INHR,   STACK_REG)
    E(0xCD, CALL, DIRECT, NO_FORMAT)
    E(0xC6, ADI,  IMM8,   NO_FORMAT)
    E(0xCE, ACI,  IMM8,   NO_FORMAT)
    E(0xD6, SUI,  IMM8,   NO_FORMAT)
    E(0xDE, SBI,  IMM8,   NO_FORMAT)
    E(0xE6, ANI,  IMM8,   NO_FORMAT)
    E(0xEE, XRI,  IMM8,   NO_FORMAT)
    E(0xF6, ORI,  IMM8,   NO_FORMAT)
    E(0xFE, CPI,  IMM8,   NO_FORMAT)
    E(0xC7, RST,  INHR,   VECTOR_NO)
};

static constexpr Entry TABLE_I8085[] PROGMEM = {
    E(0x20, RIM,  INHR,   NO_FORMAT)
    E(0x30, SIM,  INHR,   NO_FORMAT)
};

struct TableI8080::EntryPage {
    const Entry *table;
    const Entry *end;
};

static constexpr TableI8080::EntryPage I8080_PAGES[] PROGMEM = {
    { ARRAY_RANGE(TABLE_I8080) },
};
static constexpr TableI8080::EntryPage I8085_PAGES[] PROGMEM = {
    { ARRAY_RANGE(TABLE_I8080) },
    { ARRAY_RANGE(TABLE_I8085) },
};

Error TableI8080::searchName(
    InsnI8080 &insn, const EntryPage *pages, const EntryPage *end) {
    for (const EntryPage *page = pages; page < end; page++) {
        const Entry *table = reinterpret_cast<Entry *>(pgm_read_ptr(&page->table));
        const Entry *end = reinterpret_cast<Entry *>(pgm_read_ptr(&page->end));
        const Entry *entry =
            TableBase::searchName<Entry>(insn.name(), table, end);
        if (entry) {
            insn.setOpCode(pgm_read_byte(&entry->opCode));
            insn.setFlags(pgm_read_byte(&entry->flags));
            return OK;
        }
    }
    return UNKNOWN_INSTRUCTION;
}

static Config::opcode_t tableCode(
    Config::opcode_t opCode, const Entry *entry) {
    const InsnFormat iformat =
        Entry::_insnFormat(pgm_read_byte(&entry->flags));
    switch (iformat) {
    case INDEX_REG:
        return opCode & ~0x10;
    case POINTER_REG:
    case STACK_REG:
        return opCode & ~0x30;
    case DATA_REG:
    case VECTOR_NO:
        return opCode & ~0x38;
    case LOW_DATA_REG:
        return opCode & ~0x07;
    case DATA_DATA_REG:
        return opCode & ~0x3F;
    default:
        return opCode;
    }
}

Error TableI8080::searchOpCode(
    InsnI8080 &insn, const EntryPage *pages, const EntryPage *end) {
    for (const EntryPage *page = pages; page < end; page++) {
        const Entry *table = reinterpret_cast<Entry *>(pgm_read_ptr(&page->table));
        const Entry *end = reinterpret_cast<Entry *>(pgm_read_ptr(&page->end));
        const Entry *entry = TableBase::searchCode<Entry,Config::opcode_t>(
            insn.opCode(), table, end, tableCode);
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
    return "i8080, i8085";
}

const char *TableI8080::getCpu() const {
    return _cpuType == I8080 ? "8080" : "8085";
}

bool TableI8080::setCpu(const char *cpu) {
    if (toupper(*cpu) == 'I') cpu++;
    if (strcmp(cpu, "8080") == 0)
        return setCpu(I8080);
    if (strcmp(cpu, "8085") == 0)
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

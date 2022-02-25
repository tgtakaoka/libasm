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

#include "table_mc6805.h"

#include <string.h>

#include "config_mc6805.h"
#include "entry_mc6805.h"
#include "text_mc6800.h"
#include "text_mc6805.h"

using namespace libasm::mc6800;

namespace libasm {
namespace mc6805 {

#define E3(_opc, _name, _size, _op1, _op2, _op3) \
    { _opc, Entry::Flags::create(_op1, _op2, _op3, SZ_##_size), _name }
#define E2(_opc, _name, _size, _op1, _op2) E3(_opc, _name, _size, _op1, _op2, M_NO)
#define E1(_opc, _name, _size, _op1) E3(_opc, _name, _size, _op1, M_NO, M_NO)
#define E0(_opc, _name) E3(_opc, _name, NONE, M_NO, M_NO, M_NO)

// clang-format off
static constexpr Entry MC6805_TABLE[] PROGMEM = {
    E1(0x20, TEXT_BRA,  BYTE, M_REL),
    E1(0x21, TEXT_BRN,  BYTE, M_REL),
    E1(0x22, TEXT_BHI,  BYTE, M_REL),
    E1(0x23, TEXT_BLS,  BYTE, M_REL),
    E1(0x24, TEXT_BHS,  BYTE, M_REL),
    E1(0x24, TEXT_BCC,  BYTE, M_REL),
    E1(0x25, TEXT_BLO,  BYTE, M_REL),
    E1(0x25, TEXT_BCS,  BYTE, M_REL),
    E1(0x26, TEXT_BNE,  BYTE, M_REL),
    E1(0x27, TEXT_BEQ,  BYTE, M_REL),
    E1(0x28, TEXT_BHCC, BYTE, M_REL),
    E1(0x29, TEXT_BHCS, BYTE, M_REL),
    E1(0x2A, TEXT_BPL,  BYTE, M_REL),
    E1(0x2B, TEXT_BMI,  BYTE, M_REL),
    E1(0x2C, TEXT_BMC,  BYTE, M_REL),
    E1(0x2D, TEXT_BMS,  BYTE, M_REL),
    E1(0x2E, TEXT_BIL,  BYTE, M_REL),
    E1(0x2F, TEXT_BIH,  BYTE, M_REL),
    E1(0x30, TEXT_NEG,  BYTE, M_DIR),
    E1(0x33, TEXT_COM,  BYTE, M_DIR),
    E1(0x34, TEXT_LSR,  BYTE, M_DIR),
    E1(0x36, TEXT_ROR,  BYTE, M_DIR),
    E1(0x37, TEXT_ASR,  BYTE, M_DIR),
    E1(0x38, TEXT_ASL,  BYTE, M_DIR),
    E1(0x38, TEXT_LSL,  BYTE, M_DIR),
    E1(0x39, TEXT_ROL,  BYTE, M_DIR),
    E1(0x3A, TEXT_DEC,  BYTE, M_DIR),
    E1(0x3C, TEXT_INC,  BYTE, M_DIR),
    E1(0x3D, TEXT_TST,  BYTE, M_DIR),
    E1(0x3F, TEXT_CLR,  BYTE, M_DIR),
    E0(0x40, TEXT_NEGA),
    E0(0x43, TEXT_COMA),
    E0(0x44, TEXT_LSRA),
    E0(0x46, TEXT_RORA),
    E0(0x47, TEXT_ASRA),
    E0(0x48, TEXT_ASLA),
    E0(0x48, TEXT_LSLA),
    E0(0x49, TEXT_ROLA),
    E0(0x4A, TEXT_DECA),
    E0(0x4C, TEXT_INCA),
    E0(0x4D, TEXT_TSTA),
    E0(0x4F, TEXT_CLRA),
    E0(0x50, TEXT_NEGX),
    E0(0x53, TEXT_COMX),
    E0(0x54, TEXT_LSRX),
    E0(0x56, TEXT_RORX),
    E0(0x57, TEXT_ASRX),
    E0(0x58, TEXT_ASLX),
    E0(0x58, TEXT_LSLX),
    E0(0x59, TEXT_ROLX),
    E0(0x5A, TEXT_DECX),
    E0(0x5C, TEXT_INCX),
    E0(0x5D, TEXT_TSTX),
    E0(0x5F, TEXT_CLRX),
    E1(0x60, TEXT_NEG,  BYTE, M_IDX),
    E1(0x63, TEXT_COM,  BYTE, M_IDX),
    E1(0x64, TEXT_LSR,  BYTE, M_IDX),
    E1(0x66, TEXT_ROR,  BYTE, M_IDX),
    E1(0x67, TEXT_ASR,  BYTE, M_IDX),
    E1(0x68, TEXT_ASL,  BYTE, M_IDX),
    E1(0x68, TEXT_LSL,  BYTE, M_IDX),
    E1(0x69, TEXT_ROL,  BYTE, M_IDX),
    E1(0x6A, TEXT_DEC,  BYTE, M_IDX),
    E1(0x6C, TEXT_INC,  BYTE, M_IDX),
    E1(0x6D, TEXT_TST,  BYTE, M_IDX),
    E1(0x6F, TEXT_CLR,  BYTE, M_IDX),
    E1(0x70, TEXT_NEG,  NONE, M_IX0),
    E1(0x73, TEXT_COM,  NONE, M_IX0),
    E1(0x74, TEXT_LSR,  NONE, M_IX0),
    E1(0x76, TEXT_ROR,  NONE, M_IX0),
    E1(0x77, TEXT_ASR,  NONE, M_IX0),
    E1(0x78, TEXT_ASL,  NONE, M_IX0),
    E1(0x78, TEXT_LSL,  NONE, M_IX0),
    E1(0x79, TEXT_ROL,  NONE, M_IX0),
    E1(0x7A, TEXT_DEC,  NONE, M_IX0),
    E1(0x7C, TEXT_INC,  NONE, M_IX0),
    E1(0x7D, TEXT_TST,  NONE, M_IX0),
    E1(0x7F, TEXT_CLR,  NONE, M_IX0),
    E1(0xA0, TEXT_SUB,  BYTE, M_IMM),
    E1(0xA1, TEXT_CMP,  BYTE, M_IMM),
    E1(0xA2, TEXT_SBC,  BYTE, M_IMM),
    E1(0xA3, TEXT_CPX,  BYTE, M_IMM),
    E1(0xA4, TEXT_AND,  BYTE, M_IMM),
    E1(0xA5, TEXT_BIT,  BYTE, M_IMM),
    E1(0xA6, TEXT_LDA,  BYTE, M_IMM),
    E1(0xA8, TEXT_EOR,  BYTE, M_IMM),
    E1(0xA9, TEXT_ADC,  BYTE, M_IMM),
    E1(0xAA, TEXT_ORA,  BYTE, M_IMM),
    E1(0xAB, TEXT_ADD,  BYTE, M_IMM),
    E1(0xAD, TEXT_BSR,  BYTE, M_REL),
    E1(0xAE, TEXT_LDX,  BYTE, M_IMM),
    E1(0xB0, TEXT_SUB,  BYTE, M_DIR),
    E1(0xB1, TEXT_CMP,  BYTE, M_DIR),
    E1(0xB2, TEXT_SBC,  BYTE, M_DIR),
    E1(0xB3, TEXT_CPX,  BYTE, M_DIR),
    E1(0xB4, TEXT_AND,  BYTE, M_DIR),
    E1(0xB5, TEXT_BIT,  BYTE, M_DIR),
    E1(0xB6, TEXT_LDA,  BYTE, M_DIR),
    E1(0xB7, TEXT_STA,  BYTE, M_DIR),
    E1(0xB8, TEXT_EOR,  BYTE, M_DIR),
    E1(0xB9, TEXT_ADC,  BYTE, M_DIR),
    E1(0xBA, TEXT_ORA,  BYTE, M_DIR),
    E1(0xBB, TEXT_ADD,  BYTE, M_DIR),
    E1(0xBC, TEXT_JMP,  BYTE, M_DIR),
    E1(0xBD, TEXT_JSR,  BYTE, M_DIR),
    E1(0xBE, TEXT_LDX,  BYTE, M_DIR),
    E1(0xBF, TEXT_STX,  BYTE, M_DIR),
    E1(0xC0, TEXT_SUB,  WORD, M_EXT),
    E1(0xC1, TEXT_CMP,  WORD, M_EXT),
    E1(0xC2, TEXT_SBC,  WORD, M_EXT),
    E1(0xC3, TEXT_CPX,  WORD, M_EXT),
    E1(0xC4, TEXT_AND,  WORD, M_EXT),
    E1(0xC5, TEXT_BIT,  WORD, M_EXT),
    E1(0xC6, TEXT_LDA,  WORD, M_EXT),
    E1(0xC7, TEXT_STA,  WORD, M_EXT),
    E1(0xC8, TEXT_EOR,  WORD, M_EXT),
    E1(0xC9, TEXT_ADC,  WORD, M_EXT),
    E1(0xCA, TEXT_ORA,  WORD, M_EXT),
    E1(0xCB, TEXT_ADD,  WORD, M_EXT),
    E1(0xCC, TEXT_JMP,  WORD, M_EXT),
    E1(0xCD, TEXT_JSR,  WORD, M_EXT),
    E1(0xCE, TEXT_LDX,  WORD, M_EXT),
    E1(0xCF, TEXT_STX,  WORD, M_EXT),
    E1(0xE0, TEXT_SUB,  BYTE, M_IDX),
    E1(0xE1, TEXT_CMP,  BYTE, M_IDX),
    E1(0xE2, TEXT_SBC,  BYTE, M_IDX),
    E1(0xE3, TEXT_CPX,  BYTE, M_IDX),
    E1(0xE4, TEXT_AND,  BYTE, M_IDX),
    E1(0xE5, TEXT_BIT,  BYTE, M_IDX),
    E1(0xE6, TEXT_LDA,  BYTE, M_IDX),
    E1(0xE7, TEXT_STA,  BYTE, M_IDX),
    E1(0xE8, TEXT_EOR,  BYTE, M_IDX),
    E1(0xE9, TEXT_ADC,  BYTE, M_IDX),
    E1(0xEA, TEXT_ORA,  BYTE, M_IDX),
    E1(0xEB, TEXT_ADD,  BYTE, M_IDX),
    E1(0xEC, TEXT_JMP,  BYTE, M_IDX),
    E1(0xED, TEXT_JSR,  BYTE, M_IDX),
    E1(0xEE, TEXT_LDX,  BYTE, M_IDX),
    E1(0xEF, TEXT_STX,  BYTE, M_IDX),
    E1(0xD0, TEXT_SUB,  WORD, M_IX2),
    E1(0xD1, TEXT_CMP,  WORD, M_IX2),
    E1(0xD2, TEXT_SBC,  WORD, M_IX2),
    E1(0xD3, TEXT_CPX,  WORD, M_IX2),
    E1(0xD4, TEXT_AND,  WORD, M_IX2),
    E1(0xD5, TEXT_BIT,  WORD, M_IX2),
    E1(0xD6, TEXT_LDA,  WORD, M_IX2),
    E1(0xD7, TEXT_STA,  WORD, M_IX2),
    E1(0xD8, TEXT_EOR,  WORD, M_IX2),
    E1(0xD9, TEXT_ADC,  WORD, M_IX2),
    E1(0xDA, TEXT_ORA,  WORD, M_IX2),
    E1(0xDB, TEXT_ADD,  WORD, M_IX2),
    E1(0xDC, TEXT_JMP,  WORD, M_IX2),
    E1(0xDD, TEXT_JSR,  WORD, M_IX2),
    E1(0xDE, TEXT_LDX,  WORD, M_IX2),
    E1(0xDF, TEXT_STX,  WORD, M_IX2),
    E1(0xF0, TEXT_SUB,  NONE, M_IX0),
    E1(0xF1, TEXT_CMP,  NONE, M_IX0),
    E1(0xF2, TEXT_SBC,  NONE, M_IX0),
    E1(0xF3, TEXT_CPX,  NONE, M_IX0),
    E1(0xF4, TEXT_AND,  NONE, M_IX0),
    E1(0xF5, TEXT_BIT,  NONE, M_IX0),
    E1(0xF6, TEXT_LDA,  NONE, M_IX0),
    E1(0xF7, TEXT_STA,  NONE, M_IX0),
    E1(0xF8, TEXT_EOR,  NONE, M_IX0),
    E1(0xF9, TEXT_ADC,  NONE, M_IX0),
    E1(0xFA, TEXT_ORA,  NONE, M_IX0),
    E1(0xFB, TEXT_ADD,  NONE, M_IX0),
    E1(0xFC, TEXT_JMP,  NONE, M_IX0),
    E1(0xFD, TEXT_JSR,  NONE, M_IX0),
    E1(0xFE, TEXT_LDX,  NONE, M_IX0),
    E1(0xFF, TEXT_STX,  NONE, M_IX0),
    E0(0x80, TEXT_RTI),
    E0(0x81, TEXT_RTS),
    E0(0x83, TEXT_SWI),
    E0(0x97, TEXT_TAX),
    E0(0x98, TEXT_CLC),
    E0(0x99, TEXT_SEC),
    E0(0x9A, TEXT_CLI),
    E0(0x9B, TEXT_SEI),
    E0(0x9C, TEXT_RSP),
    E0(0x9D, TEXT_NOP),
    E0(0x9F, TEXT_TXA),
    E2(0x10, TEXT_BSET,  BYTE, M_BNO, M_DIR),
    E2(0x11, TEXT_BCLR,  BYTE, M_BNO, M_DIR),
    E3(0x00, TEXT_BRSET, BYTE, M_BNO, M_DIR, M_REL),
    E3(0x01, TEXT_BRCLR, BYTE, M_BNO, M_DIR, M_REL),
};

static constexpr Entry MC146805_TABLE[] PROGMEM = {
    E0(0x8E, TEXT_STOP),
    E0(0x8F, TEXT_WAIT),
};

static constexpr Entry MC68HC05_TABLE[] PROGMEM = {
    E0(0x42, TEXT_MUL),
};
// clang-format on

struct TableMc6805::EntryPage : EntryPageBase<Entry> {
    constexpr EntryPage(const Entry *table, const Entry *end) : EntryPageBase(table, end) {}
};

static constexpr TableMc6805::EntryPage MC6805_PAGES[] PROGMEM = {
        {ARRAY_RANGE(MC6805_TABLE)},
};

static constexpr TableMc6805::EntryPage MC146805_PAGES[] PROGMEM = {
        {ARRAY_RANGE(MC6805_TABLE)},
        {ARRAY_RANGE(MC146805_TABLE)},
};

static constexpr TableMc6805::EntryPage MC68HC05_PAGES[] PROGMEM = {
        {ARRAY_RANGE(MC6805_TABLE)},
        {ARRAY_RANGE(MC146805_TABLE)},
        {ARRAY_RANGE(MC68HC05_TABLE)},
};

static bool acceptAddrMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_EXT)
        return table == M_REL;
    if (opr == M_DIR)
        return table == M_REL || table == M_EXT;
    if (opr == M_BNO)
        return table == M_REL || table == M_DIR || table == M_EXT;
    return false;
}

static bool acceptAddrMode(Entry::Flags flags, const Entry *entry) {
    auto table = entry->flags();
    return acceptAddrMode(flags.mode1(), table.mode1()) &&
           acceptAddrMode(flags.mode2(), table.mode2()) &&
           acceptAddrMode(flags.mode3(), table.mode3());
}

Error TableMc6805::searchName(
        InsnMc6805 &insn, const EntryPage *pages, const EntryPage *end) const {
    uint8_t count = 0;
    for (auto page = pages; page < end; page++) {
        auto entry = TableBase::searchName<Entry, Entry::Flags>(
                insn.name(), insn.flags(), page->table(), page->end(), acceptAddrMode, count);
        if (entry) {
            insn.setOpCode(entry->opCode());
            insn.setFlags(entry->flags());
            return OK;
        }
    }
    return count == 0 ? UNKNOWN_INSTRUCTION : OPERAND_NOT_ALLOWED;
}

static Config::opcode_t maskCode(Config::opcode_t code, const Entry *entry) {
    return entry->flags().mode1() == M_BNO ? code & ~0x0E : code;
}

const Entry *TableMc6805::searchOpCode(
        InsnMc6805 &insn, const EntryPage *pages, const EntryPage *end) const {
    for (auto page = pages; page < end; page++) {
        auto entry = TableBase::searchCode<Entry, Config::opcode_t>(
                insn.opCode(), page->table(), page->end(), maskCode);
        if (entry) {
            insn.setFlags(entry->flags());
            insn.setName_P(entry->name_P());
            return entry;
        }
    }
    return nullptr;
}

Error TableMc6805::searchName(InsnMc6805 &insn) const {
    return setError(searchName(insn, _table, _end));
}

Error TableMc6805::searchOpCode(InsnMc6805 &insn) const {
    auto entry = searchOpCode(insn, _table, _end);
    return setError(entry ? OK : UNKNOWN_INSTRUCTION);
}

class CpuTable : public CpuTableBase<CpuType, TableMc6805::EntryPage> {
public:
    constexpr CpuTable(CpuType cpuType, const char *name, const TableMc6805::EntryPage *table,
            const TableMc6805::EntryPage *end)
        : CpuTableBase(cpuType, name, table, end) {}
};

static constexpr CpuTable CPU_TABLES[] PROGMEM = {
        {MC6805, TEXT_CPU_6805, ARRAY_RANGE(MC6805_PAGES)},
        {MC146805, TEXT_CPU_146805, ARRAY_RANGE(MC146805_PAGES)},
        {MC68HC05, TEXT_CPU_68HC05, ARRAY_RANGE(MC68HC05_PAGES)},
};

TableMc6805::TableMc6805() {
    setCpu(MC6805);
}

bool TableMc6805::setCpu(CpuType cpuType) {
    auto t = CpuTable::search(cpuType, ARRAY_RANGE(CPU_TABLES));
    if (t == nullptr)
        return false;
    _cpuType = cpuType;
    _table = t->table();
    _end = t->end();
    return true;
}

const /* PROGMEM */ char *TableMc6805::listCpu_P() const {
    return TEXT_CPU_LIST_6805;
}

const /* PROGMEM */ char *TableMc6805::cpu_P() const {
    return CpuTable::search(_cpuType, ARRAY_RANGE(CPU_TABLES))->name_P();
}

bool TableMc6805::setCpu(const char *cpu) {
    auto p = cpu;
    if (strncasecmp_P(p, TEXT_CPU_6805_MC, 2) == 0)
        p += 2;
    auto t = CpuTable::search(p, ARRAY_RANGE(CPU_TABLES));
    return t ? setCpu(t->cpuType()) : false;
}

class TableMc6805 TableMc6805;

}  // namespace mc6805
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

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

#include "config_ins8070.h"

#include "entry_ins8070.h"
#include "table_ins8070.h"
#include "text_ins8070.h"

#include <ctype.h>
#include <string.h>

namespace libasm {
namespace ins8070 {

#define E(_opc, _name, _dst, _src, _size, _mode)    \
    {                                               \
        _opc,                                       \
        Entry::Flags::create(                       \
            _mode, _dst, _src, SZ_##_size),         \
        TEXT_##_name                                \
    },

static constexpr Entry TABLE_INS8070[] PROGMEM = {
    E(0x00, NOP,  OPR_NO, OPR_NO, NONE, IMPLIED)
    E(0x01, XCH,  OPR_A,  OPR_E,  BYTE, IMPLIED)
    E(0x06, LD,   OPR_A,  OPR_S,  BYTE, IMPLIED)
    E(0x07, LD,   OPR_S,  OPR_A,  BYTE, IMPLIED)
    E(0x08, PUSH, OPR_EA, OPR_NO, WORD, IMPLIED)
    E(0x09, LD,   OPR_T,  OPR_EA, WORD, IMPLIED)
    E(0x0A, PUSH, OPR_A,  OPR_NO, BYTE, IMPLIED)
    E(0x0B, LD,   OPR_EA, OPR_T,  WORD, IMPLIED)
    E(0x0C, SR,   OPR_EA, OPR_NO, WORD, IMPLIED)
    E(0x0D, DIV,  OPR_EA, OPR_T,  WORD, IMPLIED)
    E(0x0E, SL,   OPR_A,  OPR_NO, BYTE, IMPLIED)
    E(0x0F, SL,   OPR_EA, OPR_NO, WORD, IMPLIED)
    E(0x10, CALL, OPR_4,  OPR_NO, NONE, IMPLIED)
    E(0x20, JSR,  OPR_16, OPR_NO, WORD, ABSOLUTE)
    E(0x22, PLI,  OPR_PN, OPR_IM, WORD, IMMEDIATE)
    E(0x24, JMP,  OPR_16, OPR_NO, WORD, ABSOLUTE)
    E(0x24, LD,   OPR_BR, OPR_IM, WORD, IMMEDIATE)
    E(0x2C, MPY,  OPR_EA, OPR_T,  WORD, IMPLIED)
    E(0x2D, BND,  OPR_RL, OPR_NO, NONE, RELATIVE)
    E(0x2E, SSM,  OPR_PN, OPR_NO, BYTE, IMPLIED)
    E(0x30, LD,   OPR_EA, OPR_BR, WORD, IMPLIED)
    E(0x38, POP,  OPR_A,  OPR_NO, BYTE, IMPLIED)
    E(0x39, AND,  OPR_S,  OPR_IM, BYTE, IMMEDIATE)
    E(0x3A, POP,  OPR_EA, OPR_NO, WORD, IMPLIED)
    E(0x3B, OR,   OPR_S,  OPR_IM, BYTE, IMMEDIATE)
    E(0x3C, SR,   OPR_A,  OPR_NO, BYTE, IMPLIED)
    E(0x3D, SRL,  OPR_A,  OPR_NO, BYTE, IMPLIED)
    E(0x3E, RR,   OPR_A,  OPR_NO, BYTE, IMPLIED)
    E(0x3F, RRL,  OPR_A,  OPR_NO, BYTE, IMPLIED)
    E(0x40, LD,   OPR_A,  OPR_E,  BYTE, IMPLIED)
    E(0x44, LD,   OPR_BR, OPR_EA, WORD, IMPLIED)
    E(0x48, LD,   OPR_E,  OPR_A,  BYTE, IMPLIED)
    E(0x4C, XCH,  OPR_EA, OPR_BR, WORD, IMPLIED)
    E(0x50, AND,  OPR_A,  OPR_E,  BYTE, IMPLIED)
    E(0x55, PUSH, OPR_SP, OPR_NO, NONE, UNDEF)
    E(0x54, PUSH, OPR_BR, OPR_NO, WORD, IMPLIED)
    E(0x58, OR,   OPR_A,  OPR_E,  BYTE, IMPLIED)
    E(0x5C, RET,  OPR_NO, OPR_NO, NONE, IMPLIED)
    E(0x5E, POP,  OPR_PN, OPR_NO, WORD, IMPLIED)
    E(0x60, XOR,  OPR_A,  OPR_E,  BYTE, IMPLIED)
    E(0x64, BP,   OPR_RL, OPR_NO, NONE, RELATIVE)
    E(0x66, BP,   OPR_PR, OPR_NO, NONE, RELATIVE)
    E(0x6C, BZ,   OPR_RL, OPR_NO, NONE, RELATIVE)
    E(0x6E, BZ,   OPR_PR, OPR_NO, NONE, RELATIVE)
    E(0x70, ADD,  OPR_A,  OPR_E,  BYTE, IMPLIED)
    E(0x74, BRA,  OPR_RL, OPR_NO, NONE, RELATIVE)
    E(0x76, BRA,  OPR_PR, OPR_NO, NONE, RELATIVE)
    E(0x78, SUB,  OPR_A,  OPR_E,  BYTE, IMPLIED)
    E(0x7C, BNZ,  OPR_RL, OPR_NO, NONE, RELATIVE)
    E(0x7E, BNZ,  OPR_PR, OPR_NO, NONE, RELATIVE)
    E(0x80, LD,   OPR_EA, OPR_GN, WORD, GENERIC)
    E(0x8C, ST,   OPR_EA, OPR_IM, NONE, UNDEF)
    E(0x88, ST,   OPR_EA, OPR_GN, WORD, GENERIC)
    E(0x94, ILD,  OPR_A,  OPR_IM, NONE, UNDEF)
    E(0x90, ILD,  OPR_A,  OPR_GN, BYTE, GENERIC)
    E(0x9C, DLD,  OPR_A,  OPR_IM, NONE, UNDEF)
    E(0x98, DLD,  OPR_A,  OPR_GN, BYTE, GENERIC)
    E(0xA0, LD,   OPR_T,  OPR_GN, WORD, GENERIC)
    E(0xB0, ADD,  OPR_EA, OPR_GN, WORD, GENERIC)
    E(0xB8, SUB,  OPR_EA, OPR_GN, WORD, GENERIC)
    E(0xC0, LD,   OPR_A,  OPR_GN, BYTE, GENERIC)
    E(0xCC, ST,   OPR_A,  OPR_IM, NONE, UNDEF)
    E(0xC8, ST,   OPR_A,  OPR_GN, BYTE, GENERIC)
    E(0xD0, AND,  OPR_A,  OPR_GN, BYTE, GENERIC)
    E(0xD8, OR,   OPR_A,  OPR_GN, BYTE, GENERIC)
    E(0xE0, XOR,  OPR_A,  OPR_GN, BYTE, GENERIC)
    E(0xF0, ADD,  OPR_A,  OPR_GN, BYTE, GENERIC)
    E(0xF8, SUB,  OPR_A,  OPR_GN, BYTE, GENERIC)
};

static bool acceptOprFormat(OprFormat opr, OprFormat table) {
    if (opr == table) return true;
    if (opr == OPR_16)
        return table == OPR_GN || table == OPR_RL;
    if (opr == OPR_4)
        return table == OPR_GN || table == OPR_RL || table == OPR_16;
    if (opr == OPR_PR)
        return table == OPR_GN;
    if (opr == OPR_IM)
        return table == OPR_GN;
    if (opr == OPR_PN)
        return table == OPR_BR;
    if (opr == OPR_SP)
        return table == OPR_BR;
    return false;
}

static bool acceptOprFormats(Entry::Flags flags, const Entry *entry) {
    const Entry::Flags table = entry->flags();
    return acceptOprFormat(flags.dstOpr(), table.dstOpr())
        && acceptOprFormat(flags.srcOpr(), table.srcOpr());
}

Error TableIns8070::searchName(InsnIns8070 &insn) const {
    uint8_t count = 0;
    const Entry *entry = TableBase::searchName<Entry, Entry::Flags>(
        insn.name(), insn.flags(), ARRAY_RANGE(TABLE_INS8070), acceptOprFormats, count);
    if (entry) {
        insn.setOpCode(entry->opCode());
        insn.setFlags(entry->flags());
        return _error.setOK();
    }
    return _error.setError(count == 0 ? UNKNOWN_INSTRUCTION : OPERAND_NOT_ALLOWED);
}

static Config::opcode_t maskCode(OprFormat format) {
    switch (format) {
    case OPR_4:  return 0x0F;
    case OPR_PR:
    case OPR_PN: return 0x01;
    case OPR_BR: return 0x03;
    case OPR_GN: return 0x07;
    default:     return 0;
    }
}

static Config::opcode_t tableCode(Config::opcode_t opCode, const Entry *entry) {
    const Entry::Flags flags = entry->flags();
    opCode &= ~maskCode(flags.dstOpr());
    opCode &= ~maskCode(flags.srcOpr());
    return opCode;
}

Error TableIns8070::searchOpCode(InsnIns8070 &insn) const {
    const Entry *entry = TableBase::searchCode<Entry, Config::opcode_t>(
        insn.opCode(), ARRAY_RANGE(TABLE_INS8070), tableCode);
    if (!entry) return _error.setError(UNKNOWN_INSTRUCTION);
    insn.setFlags(entry->flags());
    if (insn.addrMode() == UNDEF) return _error.setError(UNKNOWN_INSTRUCTION);
    insn.setName_P(entry->name());
    return _error.setOK();
}

const char *TableIns8070::listCpu() const {
    return TEXT_CPU_INS8070;
}
const char *TableIns8070::getCpu() const {
    return TEXT_CPU_8070;
}

bool TableIns8070::setCpu(const char *cpu) {
    return strcasecmp_P(cpu, TEXT_CPU_INS8070) == 0
        || strcasecmp_P(cpu, TEXT_CPU_8070) == 0;
}

class TableIns8070 TableIns8070;

} // namespace ins8070
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

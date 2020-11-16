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

#include "config_ins8060.h"

#include "entry_ins8060.h"
#include "table_ins8060.h"
#include "text_ins8060.h"

#include <ctype.h>
#include <string.h>

namespace libasm {
namespace ins8060 {

#define E(_opc, _name, _mode)                       \
    { _opc, Entry::Flags::create(_mode), _name },

static constexpr Entry TABLE_INS8060[] PROGMEM = {
    E(0x00, TEXT_HALT, INHR)
    E(0x01, TEXT_XAE,  INHR)
    E(0x02, TEXT_CCL,  INHR)
    E(0x03, TEXT_SCL,  INHR)
    E(0x04, TEXT_DINT, INHR)
    E(0x05, TEXT_IEN,  INHR)
    E(0x06, TEXT_CSA,  INHR)
    E(0x07, TEXT_CAS,  INHR)
    E(0x08, TEXT_NOP,  INHR)
    E(0x19, TEXT_SIO,  INHR)
    E(0x1C, TEXT_SR,   INHR)
    E(0x1D, TEXT_SRL,  INHR)
    E(0x1E, TEXT_RR,   INHR)
    E(0x1F, TEXT_RRL,  INHR)
    E(0x30, TEXT_XPAL, PNTR)
    E(0x34, TEXT_XPAH, PNTR)
    E(0x3C, TEXT_XPPC, PNTR)
    E(0x40, TEXT_LDE,  INHR)
    E(0x50, TEXT_ANE,  INHR)
    E(0x58, TEXT_ORE,  INHR)
    E(0x60, TEXT_XRE,  INHR)
    E(0x68, TEXT_DAE,  INHR)
    E(0x70, TEXT_ADE,  INHR)
    E(0x78, TEXT_CAE,  INHR)
    E(0x8F, TEXT_DLY,  IMM8)
    E(0x90, TEXT_JMP,  REL8)
    E(0x94, TEXT_JP,   REL8)
    E(0x98, TEXT_JZ,   REL8)
    E(0x9C, TEXT_JNZ,  REL8)
    E(0xA8, TEXT_ILD,  DISP)
    E(0xB8, TEXT_DLD,  DISP)
    E(0xC4, TEXT_LDI,  IMM8)
    E(0xC0, TEXT_LD,   INDX)
    E(0xCC, TEXT_NOP,  UNDEF)        // Use NOP as dummy name
    E(0xC8, TEXT_ST,   INDX)
    E(0xD4, TEXT_ANI,  IMM8)
    E(0xD0, TEXT_AND,  INDX)
    E(0xDC, TEXT_ORI,  IMM8)
    E(0xD8, TEXT_OR,   INDX)
    E(0xE4, TEXT_XRI,  IMM8)
    E(0xE0, TEXT_XOR,  INDX)
    E(0xEC, TEXT_DAI,  IMM8)
    E(0xE8, TEXT_DAD,  INDX)
    E(0xF4, TEXT_ADI,  IMM8)
    E(0xF0, TEXT_ADD,  INDX)
    E(0xFC, TEXT_CAI,  IMM8)
    E(0xF8, TEXT_CAD,  INDX)
};

static bool acceptAddrMode(Entry::Flags flags, const Entry *entry) {
    const AddrMode opr = flags.mode();
    const AddrMode table = entry->flags().mode();
    if (opr == table) return true;
    if (opr == REL8)
        return table == IMM8 || table == DISP || table == INDX;
    if (opr == DISP)
        return table == REL8 || table == INDX;
    return false;
}

Error TableIns8060::searchName(InsnIns8060 &insn) const {
    uint8_t count = 0;
    const Entry *entry = TableBase::searchName<Entry, Entry::Flags>(
        insn.name(), insn.flags(), ARRAY_RANGE(TABLE_INS8060),
        acceptAddrMode, count);
    if (entry) {
        insn.setOpCode(entry->opCode());
        insn.setFlags(entry->flags());
        return _error.setOK();
    }
    return _error.setError(count == 0 ? UNKNOWN_INSTRUCTION : OPERAND_NOT_ALLOWED);
}

static Config::opcode_t tableCode(Config::opcode_t opCode, const Entry *entry) {
    switch (entry->flags().mode()) {
    case PNTR: case REL8: case DISP:
        return opCode & ~0x03;
    case INDX:
        return opCode & ~0x07;
    default:
        return opCode;
    }
}

Error TableIns8060::searchOpCode(InsnIns8060 &insn) const {
    const Entry *entry = TableBase::searchCode<Entry, Config::opcode_t>(
        insn.opCode(), ARRAY_RANGE(TABLE_INS8060), tableCode);
    if (!entry) return _error.setError(UNKNOWN_INSTRUCTION);
    insn.setFlags(entry->flags());
    if (insn.addrMode() == UNDEF) return _error.setError(UNKNOWN_INSTRUCTION);
    insn.setName_P(entry->name());
    return _error.setOK();
}

const char *TableIns8060::listCpu() const {
    return TEXT_CPU_INS8060;
}

const char *TableIns8060::getCpu() const {
    return TEXT_CPU_SCMP;
}

bool TableIns8060::setCpu(const char *cpu) {
    return strcasecmp_P(cpu, TEXT_CPU_SCMP) == 0
        || strcasecmp_P(cpu, TEXT_CPU_INS8060) == 0
        || strcasecmp_P(cpu, TEXT_CPU_8060) == 0;
}

class TableIns8060 TableIns8060;

} // namespace ins8060
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

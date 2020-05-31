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

#define E(_opc, _name, _amode)                      \
    { _opc, Entry::_flags(_amode), TEXT_##_name },

static constexpr Entry TABLE_INS8060[] PROGMEM = {
    E(0x00, HALT, INHR)
    E(0x01, XAE,  INHR)
    E(0x02, CCL,  INHR)
    E(0x03, SCL,  INHR)
    E(0x04, DINT, INHR)
    E(0x05, IEN,  INHR)
    E(0x06, CSA,  INHR)
    E(0x07, CAS,  INHR)
    E(0x08, NOP,  INHR)
    E(0x19, SIO,  INHR)
    E(0x1C, SR,   INHR)
    E(0x1D, SRL,  INHR)
    E(0x1E, RR,   INHR)
    E(0x1F, RRL,  INHR)
    E(0x30, XPAL, PNTR)
    E(0x34, XPAH, PNTR)
    E(0x3C, XPPC, PNTR)
    E(0x40, LDE,  INHR)
    E(0x50, ANE,  INHR)
    E(0x58, ORE,  INHR)
    E(0x60, XRE,  INHR)
    E(0x68, DAE,  INHR)
    E(0x70, ADE,  INHR)
    E(0x78, CAE,  INHR)
    E(0x8F, DLY,  IMM8)
    E(0x90, JMP,  REL8)
    E(0x94, JP,   REL8)
    E(0x98, JZ,   REL8)
    E(0x9C, JNZ,  REL8)
    E(0xA8, ILD,  DISP)
    E(0xB8, DLD,  DISP)
    E(0xC4, LDI,  IMM8)
    E(0xC0, LD,   INDX)
    E(0xCC, NOP,  UNDEF)        // Use NOP as dummy name
    E(0xC8, ST,   INDX)
    E(0xD4, ANI,  IMM8)
    E(0xD0, AND,  INDX)
    E(0xDC, ORI,  IMM8)
    E(0xD8, OR,   INDX)
    E(0xE4, XRI,  IMM8)
    E(0xE0, XOR,  INDX)
    E(0xEC, DAI,  IMM8)
    E(0xE8, DAD,  INDX)
    E(0xF4, ADI,  IMM8)
    E(0xF0, ADD,  INDX)
    E(0xFC, CAI,  IMM8)
    E(0xF8, CAD,  INDX)
};

static bool acceptAddrMode(AddrMode opr, const Entry *entry) {
    AddrMode table = Entry::_addrMode(pgm_read_byte(&entry->flags));
    if (opr == table) return true;
    if (opr == REL8)
        return table == IMM8 || table == DISP || table == INDX;
    if (opr == DISP)
        return table == REL8 || table == INDX;
    return false;
}

Error TableIns8060::searchName(InsnIns8060 &insn) const {
    const char *name = insn.name();
    const AddrMode addrMode = insn.addrMode();
    const Entry *entry = TableBase::searchName<Entry,AddrMode>(
        name, addrMode, ARRAY_RANGE(TABLE_INS8060), acceptAddrMode);
    if (!entry) return UNKNOWN_INSTRUCTION;
    insn.setOpCode(pgm_read_byte(&entry->opCode));
    insn.setFlags(pgm_read_byte(&entry->flags));
    return OK;
}

static Config::opcode_t tableCode(
    Config::opcode_t opCode, const Entry *entry) {
    const AddrMode addrMode = Entry::_addrMode(pgm_read_byte(&entry->flags));
    switch (addrMode) {
    case PNTR:
    case REL8:
    case DISP:
        return opCode & ~0x03;
    case INDX:
        return opCode & ~0x07;
    default:
        return opCode;
    }
}

Error TableIns8060::searchOpCode(InsnIns8060 &insn) const {
    const Config::opcode_t opCode = insn.opCode();
    const Entry *entry =
        TableBase::searchCode<Entry, Config::opcode_t>(
            opCode, ARRAY_RANGE(TABLE_INS8060), tableCode);
    if (!entry) return UNKNOWN_INSTRUCTION;
    insn.setFlags(pgm_read_byte(&entry->flags));
    if (insn.addrMode() == UNDEF) return UNKNOWN_INSTRUCTION;
    TableBase::setName(insn.insn(), entry->name, Config::NAME_MAX);
    return OK;
}

bool TableIns8060::setCpu(const char *cpu) {
    if (strcasecmp(cpu, "sc/mp") == 0)
        return true;
    if (strncasecmp(cpu, "ins", 3) == 0)
        cpu += 3;
    return strcasecmp(cpu, "8060") == 0;
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

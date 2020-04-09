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

#define E(_insn, _name, _amode, _mcu)                       \
    { _insn, Entry::_flags(_mcu, _amode), TEXT_##_name },

static constexpr Entry TABLE_TMS9900[] PROGMEM = {
    E(0x0080, LST,  REG, TMS9995)
    E(0x0090, LWP,  REG, TMS9995)
    E(0x0180, DIVS, SRC, TMS9995)
    E(0x01C0, MPYS, SRC, TMS9995)
    E(0x0200, LI,   REG_IMM, TMS9900)
    E(0x0220, AI,   REG_IMM, TMS9900)
    E(0x0240, ANDI, REG_IMM, TMS9900)
    E(0x0260, ORI,  REG_IMM, TMS9900)
    E(0x0280, CI,   REG_IMM, TMS9900)
    E(0x02A0, STWP, REG, TMS9900)
    E(0x02C0, STST, REG, TMS9900)
    E(0x02E0, LWPI, IMM, TMS9900)
    E(0x0300, LIMI, IMM, TMS9900)
    E(0x0340, IDLE, INH, TMS9900)
    E(0x0360, RSET, INH, TMS9900)
    E(0x0380, RTWP, INH, TMS9900)
    E(0x03A0, CKON, INH, TMS9900)
    E(0x03C0, CKOF, INH, TMS9900)
    E(0x03E0, LREX, INH, TMS9900)
    E(0x0400, BLWP, SRC, TMS9900)
    E(0x0440, B,    SRC, TMS9900)
    E(0x0480, X,    SRC, TMS9900)
    E(0x04C0, CLR,  SRC, TMS9900)
    E(0x0500, NEG,  SRC, TMS9900)
    E(0x0540, INV,  SRC, TMS9900)
    E(0x0580, INC,  SRC, TMS9900)
    E(0x05C0, INCT, SRC, TMS9900)
    E(0x0600, DEC,  SRC, TMS9900)
    E(0x0640, DECT, SRC, TMS9900)
    E(0x0680, BL,   SRC, TMS9900)
    E(0x06C0, SWPB, SRC, TMS9900)
    E(0x0700, SETO, SRC, TMS9900)
    E(0X0740, ABS,  SRC, TMS9900)
    E(0x0800, SRA,  CNT_REG, TMS9900)
    E(0x0900, SRL,  CNT_REG, TMS9900)
    E(0x0A00, SLA,  CNT_REG, TMS9900)
    E(0x0B00, SRC,  CNT_REG, TMS9900)
    E(0x1000, JMP,  REL, TMS9900)
    E(0x1100, JLT,  REL, TMS9900)
    E(0x1200, JLE,  REL, TMS9900)
    E(0x1300, JEQ,  REL, TMS9900)
    E(0x1400, JHE,  REL, TMS9900)
    E(0x1500, JGT,  REL, TMS9900)
    E(0x1600, JNE,  REL, TMS9900)
    E(0x1700, JNC,  REL, TMS9900)
    E(0x1800, JOC,  REL, TMS9900)
    E(0x1900, JNO,  REL, TMS9900)
    E(0x1A00, JL,   REL, TMS9900)
    E(0x1B00, JH,   REL, TMS9900)
    E(0x1C00, JOP,  REL, TMS9900)
    E(0x1D00, SBO,  CRU_OFF, TMS9900)
    E(0x1E00, SBZ,  CRU_OFF, TMS9900)
    E(0x1F00, TB,   CRU_OFF, TMS9900)
    E(0x2000, COC,  REG_SRC, TMS9900)
    E(0x2400, CZC,  REG_SRC, TMS9900)
    E(0x2800, XOR,  REG_SRC, TMS9900)
    E(0x2C00, XOP,  XOP_SRC, TMS9900)
    E(0x3000, LDCR, CNT_SRC, TMS9900)
    E(0x3400, STCR, CNT_SRC, TMS9900)
    E(0x3800, MPY,  REG_SRC, TMS9900)
    E(0x3C00, DIV,  REG_SRC, TMS9900)
    E(0x4000, SZC,  DST_SRC, TMS9900)
    E(0x5000, SZCB, DST_SRC, TMS9900)
    E(0x6000, S,    DST_SRC, TMS9900)
    E(0x7000, SB,   DST_SRC, TMS9900)
    E(0x8000, C,    DST_SRC, TMS9900)
    E(0x9000, CB,   DST_SRC, TMS9900)
    E(0xA000, A,    DST_SRC, TMS9900)
    E(0xB000, AB,   DST_SRC, TMS9900)
    E(0xC000, MOV,  DST_SRC, TMS9900)
    E(0xD000, MOVB, DST_SRC, TMS9900)
    E(0xE000, SOC,  DST_SRC, TMS9900)
    E(0xF000, SOCB, DST_SRC, TMS9900)
};

static const Entry *searchEntry(
    const char *name,
    const Entry *table, const Entry *end) {
    for (const Entry *entry = table; entry < end; entry++) {
        if (pgm_strcasecmp(name, entry->name) == 0)
            return entry;
    }
    return nullptr;
}

static const Entry *searchEntry(
    const Config::insn_t insnCode,
    const Entry *table, const Entry *end) {
    for (const Entry *entry = table; entry < end; entry++) {
        Config::insn_t i = insnCode;
        const AddrMode addrMode = Entry::_addrMode(pgm_read_byte(&entry->flags));
        switch (addrMode) {
        case REG:
        case REG_IMM: i &= ~0x000f; break;
        case SRC:     i &= ~0x003f; break;
        case REL:
        case CNT_REG:
        case CRU_OFF: i &= ~0x00ff; break;
        case CNT_SRC:
        case XOP_SRC:
        case REG_SRC: i &= ~0x03ff; break;
        case DST_SRC: i &= ~0x0fff; break;
        default: break;
        }
        if (i == pgm_read_word(&entry->insnCode))
            return entry;
    }
    return nullptr;
}

Error TableTms9900::searchName(InsnTms9900 &insn) const {
    const char *name = insn.name();
    const Entry *entry = searchEntry(name, ARRAY_RANGE(TABLE_TMS9900));
    if (!entry)
        return UNKNOWN_INSTRUCTION;
    insn.setInsnCode(pgm_read_word(&entry->insnCode));
    insn.setFlags(pgm_read_byte(&entry->flags));
    if (insn.is9995() && !is9995())
        return UNKNOWN_INSTRUCTION;
    return OK;
}

Error TableTms9900::searchInsnCode(InsnTms9900 &insn) const {
    const Config::insn_t insnCode = insn.insnCode();
    const Entry *entry = searchEntry(insnCode, ARRAY_RANGE(TABLE_TMS9900));
    if (!entry)
        return UNKNOWN_INSTRUCTION;
    insn.setFlags(pgm_read_byte(&entry->flags));
    if (insn.is9995() && !is9995())
        return UNKNOWN_INSTRUCTION;
    char name[Config::NAME_MAX + 1];
    pgm_strncpy(name, entry->name, sizeof(name));
    insn.setName(name);
    return OK;
}

const char *TableTms9900::listCpu() {
    return "[tms]9900, [tms]9995";
}

bool TableTms9900::setCpu(const char *cpu) {
    if (strncasecmp(cpu, "tms", 3) == 0)
        cpu += 3;
    if (strcmp(cpu, "9900") == 0) {
        _cpuType = TMS9900;
        return true;
    }
    if (strcmp(cpu, "9995") == 0) {
        _cpuType = TMS9995;
        return true;
    }
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

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
1 */

#include "config_w65c816.h"
#include "entry_w65c816.h"
#include "table_mos6502.h"
#include "table_w65c816.h"
#include "text_w65c816.h"

#include <string.h>

namespace libasm {

namespace w65c816 {

#define E(_opc, _name, _mcu, _amode)                        \
    { _opc, Entry::_flags(_mcu, _amode), TEXT_##_name },

static const Entry W65C816_TABLE[] PROGMEM = {
    E(0xCB, WAI,  W65C816, IMPL)
    E(0xDB, STP,  W65C816, IMPL)

    E(0x0B, PHD,  W65C816, IMPL)
    E(0x1B, TCS,  W65C816, IMPL)
    E(0x2B, PLD,  W65C816, IMPL)
    E(0x3B, TSC,  W65C816, IMPL)
    E(0x4B, PHK,  W65C816, IMPL)
    E(0x5B, TCD,  W65C816, IMPL)
    E(0x6B, RTL,  W65C816, IMPL)
    E(0x7B, TDC,  W65C816, IMPL)
    E(0x8B, PHB,  W65C816, IMPL)
    E(0x9B, TXY,  W65C816, IMPL)
    E(0xAB, PLB,  W65C816, IMPL)
    E(0xBB, TYX,  W65C816, IMPL)
    E(0xEB, XBA,  W65C816, IMPL)
    E(0xFB, XCE,  W65C816, IMPL)
    E(0x02, COP,  W65C816, IMM8)
    E(0x42, WDM,  W65C816, IMM8)
    E(0xC2, REP,  W65C816, IMM8)
    E(0xE2, SEP,  W65C816, IMM8)
    E(0x5C, JMP,  W65C816, ABS_LONG)
    E(0xDC, JMP,  W65C816, ABS_IDIR_LONG)
    E(0xFC, JSR,  W65C816, ABS_IDX_IDIR)
    E(0x22, JSL,  W65C816, ABS_LONG)
    E(0x62, PER,  W65C816, REL_LONG)
    E(0x82, BRL,  W65C816, REL_LONG)
    E(0xD4, PEI,  W65C816, ZPG_IDIR)
    E(0xF4, PEA,  W65C816, ABS)
    E(0x44, MVP,  W65C816, BLOCK_MOVE)
    E(0x54, MVN,  W65C816, BLOCK_MOVE)
    E(0x03, ORA,  W65C816, SP_REL)
    E(0x13, ORA,  W65C816, SP_REL_IDIR_IDY)
    E(0x07, ORA,  W65C816, ZPG_IDIR_LONG)
    E(0x17, ORA,  W65C816, ZPG_IDIR_LONG_IDY)
    E(0x0F, ORA,  W65C816, ABS_LONG)
    E(0x1F, ORA,  W65C816, ABS_LONG_IDX)
    E(0x23, AND,  W65C816, SP_REL)
    E(0x33, AND,  W65C816, SP_REL_IDIR_IDY)
    E(0x27, AND,  W65C816, ZPG_IDIR_LONG)
    E(0x37, AND,  W65C816, ZPG_IDIR_LONG_IDY)
    E(0x2F, AND,  W65C816, ABS_LONG)
    E(0x3F, AND,  W65C816, ABS_LONG_IDX)
    E(0x43, EOR,  W65C816, SP_REL)
    E(0x53, EOR,  W65C816, SP_REL_IDIR_IDY)
    E(0x47, EOR,  W65C816, ZPG_IDIR_LONG)
    E(0x57, EOR,  W65C816, ZPG_IDIR_LONG_IDY)
    E(0x4F, EOR,  W65C816, ABS_LONG)
    E(0x5F, EOR,  W65C816, ABS_LONG_IDX)
    E(0x63, ADC,  W65C816, SP_REL)
    E(0x73, ADC,  W65C816, SP_REL_IDIR_IDY)
    E(0x67, ADC,  W65C816, ZPG_IDIR_LONG)
    E(0x77, ADC,  W65C816, ZPG_IDIR_LONG_IDY)
    E(0x6F, ADC,  W65C816, ABS_LONG)
    E(0x7F, ADC,  W65C816, ABS_LONG_IDX)
    E(0x83, STA,  W65C816, SP_REL)
    E(0x93, STA,  W65C816, SP_REL_IDIR_IDY)
    E(0x87, STA,  W65C816, ZPG_IDIR_LONG)
    E(0x97, STA,  W65C816, ZPG_IDIR_LONG_IDY)
    E(0x8F, STA,  W65C816, ABS_LONG)
    E(0x9F, STA,  W65C816, ABS_LONG_IDX)
    E(0xA3, LDA,  W65C816, SP_REL)
    E(0xB3, LDA,  W65C816, SP_REL_IDIR_IDY)
    E(0xA7, LDA,  W65C816, ZPG_IDIR_LONG)
    E(0xB7, LDA,  W65C816, ZPG_IDIR_LONG_IDY)
    E(0xAF, LDA,  W65C816, ABS_LONG)
    E(0xBF, LDA,  W65C816, ABS_LONG_IDX)
    E(0xC3, CMP,  W65C816, SP_REL)
    E(0xD3, CMP,  W65C816, SP_REL_IDIR_IDY)
    E(0xC7, CMP,  W65C816, ZPG_IDIR_LONG)
    E(0xD7, CMP,  W65C816, ZPG_IDIR_LONG_IDY)
    E(0xCF, CMP,  W65C816, ABS_LONG)
    E(0xDF, CMP,  W65C816, ABS_LONG_IDX)
    E(0xE3, SBC,  W65C816, SP_REL)
    E(0xF3, SBC,  W65C816, SP_REL_IDIR_IDY)
    E(0xE7, SBC,  W65C816, ZPG_IDIR_LONG)
    E(0xF7, SBC,  W65C816, ZPG_IDIR_LONG_IDY)
    E(0xEF, SBC,  W65C816, ABS_LONG)
    E(0xFF, SBC,  W65C816, ABS_LONG_IDX)

    E(0xDC, JMPL, W65C816, ABS_IDIR)
    E(0x07, ORAL, W65C816, ZPG_IDIR)
    E(0x17, ORAL, W65C816, ZPG_IDIR_IDY)
    E(0x27, ANDL, W65C816, ZPG_IDIR)
    E(0x37, ANDL, W65C816, ZPG_IDIR_IDY)
    E(0x47, EORL, W65C816, ZPG_IDIR)
    E(0x57, EORL, W65C816, ZPG_IDIR_IDY)
    E(0x67, ADCL, W65C816, ZPG_IDIR)
    E(0x77, ADCL, W65C816, ZPG_IDIR_IDY)
    E(0x87, STAL, W65C816, ZPG_IDIR)
    E(0x97, STAL, W65C816, ZPG_IDIR_IDY)
    E(0xA7, LDAL, W65C816, ZPG_IDIR)
    E(0xB7, LDAL, W65C816, ZPG_IDIR_IDY)
    E(0xC7, CMPL, W65C816, ZPG_IDIR)
    E(0xD7, CMPL, W65C816, ZPG_IDIR_IDY)
    E(0xE7, SBCL, W65C816, ZPG_IDIR)
    E(0xF7, SBCL, W65C816, ZPG_IDIR_IDY)

    // Pseudo instruction to control M and X bits.
    E(PSEUDO_LONGA,  LONGA,  W65C816, PSEUDO)
    E(PSEUDO_LONGI,  LONGI,  W65C816, PSEUDO)
    E(PSEUDO_ASSUME, ASSUME, W65C816, PSEUDO)
};

Error TableW65C816::searchName(
    InsnW65C816 &insn, const Entry *table, const Entry *end) const {
    const char *name = insn.name();
    for (const Entry *entry = table;
         entry < end && (entry = TableBase::searchName<Entry>(name, table, end));
         entry++) {
        insn.setFlags(pgm_read_byte(&entry->flags));
        insn.setOpCode(pgm_read_byte(&entry->opCode));
        return OK;
    }
    return UNKNOWN_INSTRUCTION;
}

static bool acceptAddrMode(AddrMode opr, const Entry *entry) {
    const AddrMode table = Entry::_addrMode(pgm_read_byte(&entry->flags));
    if (opr == table) return true;
    if (opr == IMMA) return table == IMM8;
    if (opr == ZPG) return table == ABS;
    if (opr == ZPG_IDX_IDIR) return table == ABS_IDX_IDIR;
    if (opr == ZPG_IDIR) return table == ABS_IDIR;
    if (opr == ZPG_IDIR_LONG) return table == ABS_IDIR_LONG;
    if (opr == ZPG_IDX) return table == ABS_IDX;
    if (opr == ZPG_IDY) return table == ABS_IDY;
    return false;
}

Error TableW65C816::searchNameAndAddrMode(
    InsnW65C816 &insn, const Entry *table, const Entry *end) const {
    const char *name = insn.name();
    const AddrMode addrMode = insn.addrMode();
    const Entry *entry = TableBase::searchName<Entry,AddrMode>(
        name, addrMode, table, end, acceptAddrMode);
    if (entry) {
        insn.setFlags(pgm_read_byte(&entry->flags));
        insn.setOpCode(pgm_read_byte(&entry->opCode));
        return OK;
    }
    return UNKNOWN_INSTRUCTION;
}

static bool acceptAddrMode(AddrMode addrMode, bool acceptIndirectLong) {
    if (addrMode == ABS_IDIR_LONG
        || addrMode == ZPG_IDIR_LONG
        || addrMode == ZPG_IDIR_LONG_IDY)
        return acceptIndirectLong;
    return true;
}

Error TableW65C816::searchOpCode(
    InsnW65C816 &insn, bool acceptIndirectLong,
    const Entry *table, const Entry *end) const {
    const Config::opcode_t opCode = insn.opCode();
    for (const Entry *entry = table;
         entry < end
             && (entry = TableBase::searchCode<Entry,Config::opcode_t>(
                     opCode, entry, end));
         entry++) {
        insn.setFlags(pgm_read_byte(&entry->flags));
        if (!acceptAddrMode(insn.addrMode(), acceptIndirectLong))
            continue;
        TableBase::setName(insn.insn(), entry->name, Config::NAME_MAX);
        return OK;
    }
    return UNKNOWN_INSTRUCTION;
}

Error TableW65C816::searchName(InsnW65C816 &insn) const {
    mos6502::InsnMos6502 mos6502(insn.insn());
    mos6502::TableMos6502.setCpu("65SC02");
    if (mos6502::TableMos6502.searchName(mos6502) == OK) {
        insn.setFlags(mos6502.flags());
        insn.setOpCode(mos6502.opCode());
        return OK;
    }
    return searchName(insn, ARRAY_RANGE(W65C816_TABLE));
}

Error TableW65C816::searchNameAndAddrMode(InsnW65C816 &insn) const {
    mos6502::InsnMos6502 mos6502(insn.insn());
    mos6502.setFlags(insn.flags());
    mos6502::TableMos6502.setCpu("65SC02");
    if (mos6502::TableMos6502.searchNameAndAddrMode(mos6502) == OK) {
        insn.setFlags(mos6502.flags());
        insn.setOpCode(mos6502.opCode());
        return OK;
    }
    return searchNameAndAddrMode(insn, ARRAY_RANGE(W65C816_TABLE));
}

Error TableW65C816::searchOpCode(
    InsnW65C816 &insn, bool acceptIndirectLong) const {
    mos6502::InsnMos6502 mos6502(insn.insn());
    mos6502.setOpCode(insn.opCode());
    mos6502::TableMos6502.setCpu("65SC02");
    if (mos6502::TableMos6502.searchOpCode(mos6502) == OK) {
        insn.setFlags(mos6502.flags());
        return OK;
    }
    return searchOpCode(insn, acceptIndirectLong, ARRAY_RANGE(W65C816_TABLE));
}

bool TableW65C816::setCpu(const char *cpu) {
    const char *p;
    p = cpu + (toupper(*cpu) == 'W' ? 1 : 0);
    if (strcmp(p, "65816") == 0
        || strcasecmp(p, "65C816") == 0
        || strcasecmp(p, "65C816S") == 0) {
        return true;
    }
    return false;
}

class TableW65C816 TableW65C816;

} // namespace w65c816
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

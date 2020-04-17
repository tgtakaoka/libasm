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

#include "config_mos6502.h"

#include "entry_mos6502.h"
#include "table_mos6502.h"
#include "text_mos6502.h"

#include <string.h>

namespace libasm {
namespace mos6502 {

#define E(_opc, _name, _mcu, _amode)                        \
    { _opc, Entry::_flags(_mcu, _amode), TEXT_##_name },

static constexpr Entry MOS6502_TABLE[] PROGMEM = {
    E(0x00, BRK,  MOS6502, IMPL)
    E(0x40, RTI,  MOS6502, IMPL)
    E(0x60, RTS,  MOS6502, IMPL)
    E(0xEA, NOP,  MOS6502, IMPL)
    E(0x08, PHP,  MOS6502, IMPL)
    E(0x28, PLP,  MOS6502, IMPL)
    E(0x48, PHA,  MOS6502, IMPL)
    E(0x68, PLA,  MOS6502, IMPL)
    E(0x88, DEY,  MOS6502, IMPL)
    E(0xC8, INY,  MOS6502, IMPL)
    E(0xCA, DEX,  MOS6502, IMPL)
    E(0xE8, INX,  MOS6502, IMPL)
    E(0x98, TYA,  MOS6502, IMPL)
    E(0xA8, TAY,  MOS6502, IMPL)
    E(0x8A, TXA,  MOS6502, IMPL)
    E(0xAA, TAX,  MOS6502, IMPL)
    E(0x9A, TXS,  MOS6502, IMPL)
    E(0xBA, TSX,  MOS6502, IMPL)
    E(0x18, CLC,  MOS6502, IMPL)
    E(0x38, SEC,  MOS6502, IMPL)
    E(0x58, CLI,  MOS6502, IMPL)
    E(0x78, SEI,  MOS6502, IMPL)
    E(0xB8, CLV,  MOS6502, IMPL)
    E(0xD8, CLD,  MOS6502, IMPL)
    E(0xF8, SED,  MOS6502, IMPL)
    E(0x0A, ASL,  MOS6502, ACCM)
    E(0x2A, ROL,  MOS6502, ACCM)
    E(0x4A, LSR,  MOS6502, ACCM)
    E(0x6A, ROR,  MOS6502, ACCM)
    E(0xA0, LDY,  MOS6502, IMM)
    E(0xA2, LDX,  MOS6502, IMM)
    E(0xC0, CPY,  MOS6502, IMM)
    E(0xE0, CPX,  MOS6502, IMM)
    E(0x09, ORA,  MOS6502, IMM)
    E(0x29, AND,  MOS6502, IMM)
    E(0x49, EOR,  MOS6502, IMM)
    E(0x69, ADC,  MOS6502, IMM)
    E(0xA9, LDA,  MOS6502, IMM)
    E(0xC9, CMP,  MOS6502, IMM)
    E(0xE9, SBC,  MOS6502, IMM)
    E(0x24, BIT,  MOS6502, ZPG)
    E(0x05, ORA,  MOS6502, ZPG)
    E(0x25, AND,  MOS6502, ZPG)
    E(0x45, EOR,  MOS6502, ZPG)
    E(0x65, ADC,  MOS6502, ZPG)
    E(0x85, STA,  MOS6502, ZPG)
    E(0xA5, LDA,  MOS6502, ZPG)
    E(0xC5, CMP,  MOS6502, ZPG)
    E(0xE5, SBC,  MOS6502, ZPG)
    E(0x84, STY,  MOS6502, ZPG)
    E(0xA4, LDY,  MOS6502, ZPG)
    E(0xC4, CPY,  MOS6502, ZPG)
    E(0x86, STX,  MOS6502, ZPG)
    E(0xA6, LDX,  MOS6502, ZPG)
    E(0xE4, CPX,  MOS6502, ZPG)
    E(0xC6, DEC,  MOS6502, ZPG)
    E(0xE6, INC,  MOS6502, ZPG)
    E(0x06, ASL,  MOS6502, ZPG)
    E(0x26, ROL,  MOS6502, ZPG)
    E(0x46, LSR,  MOS6502, ZPG)
    E(0x66, ROR,  MOS6502, ZPG)
    E(0x15, ORA,  MOS6502, ZPG_IDX)
    E(0x35, AND,  MOS6502, ZPG_IDX)
    E(0x55, EOR,  MOS6502, ZPG_IDX)
    E(0x75, ADC,  MOS6502, ZPG_IDX)
    E(0x95, STA,  MOS6502, ZPG_IDX)
    E(0xB5, LDA,  MOS6502, ZPG_IDX)
    E(0xD5, CMP,  MOS6502, ZPG_IDX)
    E(0xF5, SBC,  MOS6502, ZPG_IDX)
    E(0x94, STY,  MOS6502, ZPG_IDX)
    E(0xB4, LDY,  MOS6502, ZPG_IDX)
    E(0x96, STX,  MOS6502, ZPG_IDY)
    E(0xB6, LDX,  MOS6502, ZPG_IDY)
    E(0xD6, DEC,  MOS6502, ZPG_IDX)
    E(0xF6, INC,  MOS6502, ZPG_IDX)
    E(0x16, ASL,  MOS6502, ZPG_IDX)
    E(0x36, ROL,  MOS6502, ZPG_IDX)
    E(0x56, LSR,  MOS6502, ZPG_IDX)
    E(0x76, ROR,  MOS6502, ZPG_IDX)
    E(0x2C, BIT,  MOS6502, ABS)
    E(0x0D, ORA,  MOS6502, ABS)
    E(0x2D, AND,  MOS6502, ABS)
    E(0x4D, EOR,  MOS6502, ABS)
    E(0x6D, ADC,  MOS6502, ABS)
    E(0x8D, STA,  MOS6502, ABS)
    E(0xAD, LDA,  MOS6502, ABS)
    E(0xCD, CMP,  MOS6502, ABS)
    E(0xED, SBC,  MOS6502, ABS)
    E(0x8C, STY,  MOS6502, ABS)
    E(0xAC, LDY,  MOS6502, ABS)
    E(0xCC, CPY,  MOS6502, ABS)
    E(0x8E, STX,  MOS6502, ABS)
    E(0xAE, LDX,  MOS6502, ABS)
    E(0xEC, CPX,  MOS6502, ABS)
    E(0xCE, DEC,  MOS6502, ABS)
    E(0xEE, INC,  MOS6502, ABS)
    E(0x0E, ASL,  MOS6502, ABS)
    E(0x2E, ROL,  MOS6502, ABS)
    E(0x4E, LSR,  MOS6502, ABS)
    E(0x6E, ROR,  MOS6502, ABS)
    E(0x4C, JMP,  MOS6502, ABS)
    E(0x20, JSR,  MOS6502, ABS)
    E(0x1D, ORA,  MOS6502, ABS_IDX)
    E(0x3D, AND,  MOS6502, ABS_IDX)
    E(0x5D, EOR,  MOS6502, ABS_IDX)
    E(0x7D, ADC,  MOS6502, ABS_IDX)
    E(0x9D, STA,  MOS6502, ABS_IDX)
    E(0xBD, LDA,  MOS6502, ABS_IDX)
    E(0xDD, CMP,  MOS6502, ABS_IDX)
    E(0xFD, SBC,  MOS6502, ABS_IDX)
    E(0x19, ORA,  MOS6502, ABS_IDY)
    E(0x39, AND,  MOS6502, ABS_IDY)
    E(0x59, EOR,  MOS6502, ABS_IDY)
    E(0x79, ADC,  MOS6502, ABS_IDY)
    E(0x99, STA,  MOS6502, ABS_IDY)
    E(0xB9, LDA,  MOS6502, ABS_IDY)
    E(0xD9, CMP,  MOS6502, ABS_IDY)
    E(0xF9, SBC,  MOS6502, ABS_IDY)
    E(0xBC, LDY,  MOS6502, ABS_IDX)
    E(0xBE, LDX,  MOS6502, ABS_IDY)
    E(0xDE, DEC,  MOS6502, ABS_IDX)
    E(0xFE, INC,  MOS6502, ABS_IDX)
    E(0x1E, ASL,  MOS6502, ABS_IDX)
    E(0x3E, ROL,  MOS6502, ABS_IDX)
    E(0x5E, LSR,  MOS6502, ABS_IDX)
    E(0x7E, ROR,  MOS6502, ABS_IDX)
    E(0x6C, JMP,  MOS6502, ABS_IDIR)
    E(0x01, ORA,  MOS6502, ZPG_IDX_IDIR)
    E(0x21, AND,  MOS6502, ZPG_IDX_IDIR)
    E(0x41, EOR,  MOS6502, ZPG_IDX_IDIR)
    E(0x61, ADC,  MOS6502, ZPG_IDX_IDIR)
    E(0x81, STA,  MOS6502, ZPG_IDX_IDIR)
    E(0xA1, LDA,  MOS6502, ZPG_IDX_IDIR)
    E(0xC1, CMP,  MOS6502, ZPG_IDX_IDIR)
    E(0xE1, SBC,  MOS6502, ZPG_IDX_IDIR)
    E(0x11, ORA,  MOS6502, ZPG_IDIR_IDY)
    E(0x31, AND,  MOS6502, ZPG_IDIR_IDY)
    E(0x51, EOR,  MOS6502, ZPG_IDIR_IDY)
    E(0x71, ADC,  MOS6502, ZPG_IDIR_IDY)
    E(0x91, STA,  MOS6502, ZPG_IDIR_IDY)
    E(0xB1, LDA,  MOS6502, ZPG_IDIR_IDY)
    E(0xD1, CMP,  MOS6502, ZPG_IDIR_IDY)
    E(0xF1, SBC,  MOS6502, ZPG_IDIR_IDY)
    E(0x10, BPL,  MOS6502, REL)
    E(0x30, BMI,  MOS6502, REL)
    E(0x50, BVC,  MOS6502, REL)
    E(0x70, BVS,  MOS6502, REL)
    E(0x90, BCC,  MOS6502, REL)
    E(0xB0, BCS,  MOS6502, REL)
    E(0xD0, BNE,  MOS6502, REL)
    E(0xF0, BEQ,  MOS6502, REL)
    E(0x5A, PHY,  W65SC02, IMPL)
    E(0x7A, PLY,  W65SC02, IMPL)
    E(0xDA, PHX,  W65SC02, IMPL)
    E(0xFA, PLX,  W65SC02, IMPL)
    E(0x1A, INC,  W65SC02, ACCM)
    E(0x3A, DEC,  W65SC02, ACCM)
    E(0x89, BIT,  W65SC02, IMM)
    E(0x04, TSB,  W65SC02, ZPG)
    E(0x14, TRB,  W65SC02, ZPG)
    E(0x64, STZ,  W65SC02, ZPG)
    E(0x34, BIT,  W65SC02, ZPG_IDX)
    E(0x74, STZ,  W65SC02, ZPG_IDX)
    E(0x0C, TSB,  W65SC02, ABS)
    E(0x1C, TRB,  W65SC02, ABS)
    E(0x9C, STZ,  W65SC02, ABS)
    E(0x3C, BIT,  W65SC02, ABS_IDX)
    E(0x9E, STZ,  W65SC02, ABS_IDX)
    E(0x7C, JMP,  W65SC02, ABS_IDX_IDIR)
    E(0x12, ORA,  W65SC02, ZPG_IDIR)
    E(0x32, AND,  W65SC02, ZPG_IDIR)
    E(0x52, EOR,  W65SC02, ZPG_IDIR)
    E(0x72, ADC,  W65SC02, ZPG_IDIR)
    E(0x92, STA,  W65SC02, ZPG_IDIR)
    E(0xB2, LDA,  W65SC02, ZPG_IDIR)
    E(0xD2, CMP,  W65SC02, ZPG_IDIR)
    E(0xF2, SBC,  W65SC02, ZPG_IDIR)
    E(0x80, BRA,  W65SC02, REL)
    E(0x0F, BBR0, R65C02,  ZPG_REL)
    E(0x1F, BBR1, R65C02,  ZPG_REL)
    E(0x2F, BBR2, R65C02,  ZPG_REL)
    E(0x3F, BBR3, R65C02,  ZPG_REL)
    E(0x4F, BBR4, R65C02,  ZPG_REL)
    E(0x5F, BBR5, R65C02,  ZPG_REL)
    E(0x6F, BBR6, R65C02,  ZPG_REL)
    E(0x7F, BBR7, R65C02,  ZPG_REL)
    E(0x8F, BBS0, R65C02,  ZPG_REL)
    E(0x9F, BBS1, R65C02,  ZPG_REL)
    E(0xAF, BBS2, R65C02,  ZPG_REL)
    E(0xBF, BBS3, R65C02,  ZPG_REL)
    E(0xCF, BBS4, R65C02,  ZPG_REL)
    E(0xDF, BBS5, R65C02,  ZPG_REL)
    E(0xEF, BBS6, R65C02,  ZPG_REL)
    E(0xFF, BBS7, R65C02,  ZPG_REL)
    E(0xCB, WAI,  W65C02S, IMPL)
    E(0xDB, STP,  W65C02S, IMPL)
    E(0x07, RMB0, R65C02,  ZPG)
    E(0x17, RMB1, R65C02,  ZPG)
    E(0x27, RMB2, R65C02,  ZPG)
    E(0x37, RMB3, R65C02,  ZPG)
    E(0x47, RMB4, R65C02,  ZPG)
    E(0x57, RMB5, R65C02,  ZPG)
    E(0x67, RMB6, R65C02,  ZPG)
    E(0x77, RMB7, R65C02,  ZPG)
    E(0x87, SMB0, R65C02,  ZPG)
    E(0x97, SMB1, R65C02,  ZPG)
    E(0xA7, SMB2, R65C02,  ZPG)
    E(0xB7, SMB3, R65C02,  ZPG)
    E(0xC7, SMB4, R65C02,  ZPG)
    E(0xD7, SMB5, R65C02,  ZPG)
    E(0xE7, SMB6, R65C02,  ZPG)
    E(0xF7, SMB7, R65C02,  ZPG)
    E(0xCB, WAI,  W65C02S, IMPL)
    E(0xDB, STP,  W65C02S, IMPL)
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
    E(0x02, COP,  W65C816, IMM)
    E(0x42, WDM,  W65C816, IMM)
    E(0xC2, REP,  W65C816, IMM)
    E(0xE2, SEP,  W65C816, IMM)
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
};

Error TableMos6502::searchName(
    InsnMos6502 &insn, const Entry *table, const Entry *end) const {
    const char *name = insn.name();
    for (const Entry *entry = table;
         entry < end && (entry = TableBase::searchName<Entry>(name, table, end));
         entry++) {
        insn.setFlags(pgm_read_byte(&entry->flags));
        if (!insn.supported(_cpuType)) continue;
        insn.setInsnCode(pgm_read_byte(&entry->opCode));
        return OK;
    }
    return UNKNOWN_INSTRUCTION;
}

static bool acceptAddrMode(AddrMode opr, const Entry *entry) {
    AddrMode table = Entry::_addrMode(pgm_read_byte(&entry->flags));
    if (opr == table) return true;
    if (opr == ZPG) return table == ABS;
    if (opr == ZPG_IDX_IDIR) return table == ABS_IDX_IDIR;
    if (opr == ZPG_IDIR) return table == ABS_IDIR
                             || table == ZPG_IDIR_LONG;
    if (opr == ZPG_IDIR_IDY) return table == ZPG_IDIR_LONG_IDY;
    if (opr == ZPG_IDIR_LONG) return table == ABS_IDIR_LONG;
    if (opr == ZPG_IDX) return table == ABS_IDX;
    if (opr == ZPG_IDY) return table == ABS_IDY;
    return false;
}

Error TableMos6502::searchNameAndAddrMode(
    InsnMos6502 &insn, const Entry *table, const Entry *end) const {
    const char *name = insn.name();
    const AddrMode addrMode = insn.addrMode();
    for (const Entry *entry = table;
         entry < end
             && (entry = TableBase::searchName<Entry>(
                     name, addrMode, entry, end, acceptAddrMode));
         entry++) {
        insn.setFlags(pgm_read_byte(&entry->flags));
        if (!insn.supported(_cpuType)) continue;
        insn.setInsnCode(pgm_read_byte(&entry->opCode));
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

Error TableMos6502::searchInsnCode(
    InsnMos6502 &insn, bool acceptIndirectLong,
    const Entry *table, const Entry *end) const {
    const Config::insn_t insnCode = insn.insnCode();
    for (const Entry *entry = table;
         entry < end
             && (entry = TableBase::searchCode<Entry,Config::opcode_t>(
                     insnCode, entry, end));
         entry++) {
        insn.setFlags(pgm_read_byte(&entry->flags));
        if (!insn.supported(_cpuType)) continue;
        if (!acceptAddrMode(insn.addrMode(), acceptIndirectLong))
            continue;
        TableBase::setName(insn.insn(), entry->name, Config::NAME_MAX);
        return OK;
    }
    return UNKNOWN_INSTRUCTION;
}

Error TableMos6502::searchName(InsnMos6502 &insn) const {
    return searchName(insn, ARRAY_RANGE(MOS6502_TABLE));
}

Error TableMos6502::searchNameAndAddrMode(InsnMos6502 &insn) const {
    return searchNameAndAddrMode(insn, ARRAY_RANGE(MOS6502_TABLE));
}

Error TableMos6502::searchInsnCode(
    InsnMos6502 &insn, bool acceptIndirectLong) const {
    return searchInsnCode(
        insn, acceptIndirectLong, ARRAY_RANGE(MOS6502_TABLE));
}

const char *TableMos6502::listCpu() {
    return "6502, 65SC02, 65C02, W65C02S, 65816";
}

bool TableMos6502::setCpu(const char *cpu) {
    const char *p;
    p = cpu + (strncasecmp(cpu, "MOS", 3) ? 0 : 3);
    if (strcmp(p, "6502") == 0) {
        _cpuType = MOS6502;
        return true;
    }
    p = cpu + (toupper(*cpu) == 'W' ? 1 : 0);
    if (strcasecmp(p, "65SC02") == 0) {
        _cpuType = W65SC02;
        return true;
    }
    p = cpu + (toupper(*cpu) == 'R' ? 1 : 0);
    if (strcasecmp(p, "65C02") == 0) {
        _cpuType = R65C02;
        return true;
    }
    if (strncasecmp(cpu, "W65C02", 6) == 0
        && (cpu[6] == 0 || toupper(cpu[6]) == 'S')) {
        _cpuType = W65C02S;
        return true;
    }
    p = cpu + (toupper(*cpu) == 'W' ? 1 : 0);
    if (strcmp(p, "65816") == 0
        || strcasecmp(p, "65C816") == 0
        || strcasecmp(p, "65C816S") == 0) {
        _cpuType = W65C816;
        return true;
    }
    return false;
}

class TableMos6502 TableMos6502;

} // namespace mos6502
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

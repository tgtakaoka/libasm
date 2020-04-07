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

#include "config_m6502.h"

#include "entry_m6502.h"
#include "table_m6502.h"
#include "text_m6502.h"

#include <string.h>

namespace libasm {
namespace m6502 {

#define E(_opc, _name, _mcu, _amode)                        \
    { _opc, Entry::_flags(_mcu, _amode), TEXT_##_name },

static constexpr Entry M6502_TABLE[] PROGMEM = {
    E(0x00, BRK, M6502, IMPL)
    E(0x40, RTI, M6502, IMPL)
    E(0x60, RTS, M6502, IMPL)
    E(0xEA, NOP, M6502, IMPL)
    E(0x08, PHP, M6502, IMPL)
    E(0x28, PLP, M6502, IMPL)
    E(0x48, PHA, M6502, IMPL)
    E(0x68, PLA, M6502, IMPL)
    E(0x88, DEY, M6502, IMPL)
    E(0xC8, INY, M6502, IMPL)
    E(0xCA, DEX, M6502, IMPL)
    E(0xE8, INX, M6502, IMPL)
    E(0x98, TYA, M6502, IMPL)
    E(0xA8, TAY, M6502, IMPL)
    E(0x8A, TXA, M6502, IMPL)
    E(0xAA, TAX, M6502, IMPL)
    E(0x9A, TXS, M6502, IMPL)
    E(0xBA, TSX, M6502, IMPL)
    E(0x18, CLC, M6502, IMPL)
    E(0x38, SEC, M6502, IMPL)
    E(0x58, CLI, M6502, IMPL)
    E(0x78, SEI, M6502, IMPL)
    E(0xB8, CLV, M6502, IMPL)
    E(0xD8, CLD, M6502, IMPL)
    E(0xF8, SED, M6502, IMPL)
    E(0x0A, ASL, M6502, ACCM)
    E(0x2A, ROL, M6502, ACCM)
    E(0x4A, LSR, M6502, ACCM)
    E(0x6A, ROR, M6502, ACCM)
    E(0xA0, LDY, M6502, IMM)
    E(0xA2, LDX, M6502, IMM)
    E(0xC0, CPY, M6502, IMM)
    E(0xE0, CPX, M6502, IMM)
    E(0x09, ORA, M6502, IMM)
    E(0x29, AND, M6502, IMM)
    E(0x49, EOR, M6502, IMM)
    E(0x69, ADC, M6502, IMM)
    E(0xA9, LDA, M6502, IMM)
    E(0xC9, CMP, M6502, IMM)
    E(0xE9, SBC, M6502, IMM)
    E(0x24, BIT, M6502, ZPG)
    E(0x05, ORA, M6502, ZPG)
    E(0x25, AND, M6502, ZPG)
    E(0x45, EOR, M6502, ZPG)
    E(0x65, ADC, M6502, ZPG)
    E(0x85, STA, M6502, ZPG)
    E(0xA5, LDA, M6502, ZPG)
    E(0xC5, CMP, M6502, ZPG)
    E(0xE5, SBC, M6502, ZPG)
    E(0x84, STY, M6502, ZPG)
    E(0xA4, LDY, M6502, ZPG)
    E(0xC4, CPY, M6502, ZPG)
    E(0x86, STX, M6502, ZPG)
    E(0xA6, LDX, M6502, ZPG)
    E(0xE4, CPX, M6502, ZPG)
    E(0xC6, DEC, M6502, ZPG)
    E(0xE6, INC, M6502, ZPG)
    E(0x06, ASL, M6502, ZPG)
    E(0x26, ROL, M6502, ZPG)
    E(0x46, LSR, M6502, ZPG)
    E(0x66, ROR, M6502, ZPG)
    E(0x15, ORA, M6502, ZPG_IDX)
    E(0x35, AND, M6502, ZPG_IDX)
    E(0x55, EOR, M6502, ZPG_IDX)
    E(0x75, ADC, M6502, ZPG_IDX)
    E(0x95, STA, M6502, ZPG_IDX)
    E(0xB5, LDA, M6502, ZPG_IDX)
    E(0xD5, CMP, M6502, ZPG_IDX)
    E(0xF5, SBC, M6502, ZPG_IDX)
    E(0x94, STY, M6502, ZPG_IDX)
    E(0xB4, LDY, M6502, ZPG_IDX)
    E(0x96, STX, M6502, ZPG_IDY)
    E(0xB6, LDX, M6502, ZPG_IDY)
    E(0xD6, DEC, M6502, ZPG_IDX)
    E(0xF6, INC, M6502, ZPG_IDX)
    E(0x16, ASL, M6502, ZPG_IDX)
    E(0x36, ROL, M6502, ZPG_IDX)
    E(0x56, LSR, M6502, ZPG_IDX)
    E(0x76, ROR, M6502, ZPG_IDX)
    E(0x2C, BIT, M6502, ABS)
    E(0x0D, ORA, M6502, ABS)
    E(0x2D, AND, M6502, ABS)
    E(0x4D, EOR, M6502, ABS)
    E(0x6D, ADC, M6502, ABS)
    E(0x8D, STA, M6502, ABS)
    E(0xAD, LDA, M6502, ABS)
    E(0xCD, CMP, M6502, ABS)
    E(0xED, SBC, M6502, ABS)
    E(0x8C, STY, M6502, ABS)
    E(0xAC, LDY, M6502, ABS)
    E(0xCC, CPY, M6502, ABS)
    E(0x8E, STX, M6502, ABS)
    E(0xAE, LDX, M6502, ABS)
    E(0xEC, CPX, M6502, ABS)
    E(0xCE, DEC, M6502, ABS)
    E(0xEE, INC, M6502, ABS)
    E(0x0E, ASL, M6502, ABS)
    E(0x2E, ROL, M6502, ABS)
    E(0x4E, LSR, M6502, ABS)
    E(0x6E, ROR, M6502, ABS)
    E(0x4C, JMP, M6502, ABS)
    E(0x20, JSR, M6502, ABS)
    E(0x1D, ORA, M6502, ABS_IDX)
    E(0x3D, AND, M6502, ABS_IDX)
    E(0x5D, EOR, M6502, ABS_IDX)
    E(0x7D, ADC, M6502, ABS_IDX)
    E(0x9D, STA, M6502, ABS_IDX)
    E(0xBD, LDA, M6502, ABS_IDX)
    E(0xDD, CMP, M6502, ABS_IDX)
    E(0xFD, SBC, M6502, ABS_IDX)
    E(0x19, ORA, M6502, ABS_IDY)
    E(0x39, AND, M6502, ABS_IDY)
    E(0x59, EOR, M6502, ABS_IDY)
    E(0x79, ADC, M6502, ABS_IDY)
    E(0x99, STA, M6502, ABS_IDY)
    E(0xB9, LDA, M6502, ABS_IDY)
    E(0xD9, CMP, M6502, ABS_IDY)
    E(0xF9, SBC, M6502, ABS_IDY)
    E(0xBC, LDY, M6502, ABS_IDX)
    E(0xBE, LDX, M6502, ABS_IDY)
    E(0xDE, DEC, M6502, ABS_IDX)
    E(0xFE, INC, M6502, ABS_IDX)
    E(0x1E, ASL, M6502, ABS_IDX)
    E(0x3E, ROL, M6502, ABS_IDX)
    E(0x5E, LSR, M6502, ABS_IDX)
    E(0x7E, ROR, M6502, ABS_IDX)
    E(0x6C, JMP, M6502, ABS_IDIR)
    E(0x01, ORA, M6502, ZPG_IDX_IDIR)
    E(0x21, AND, M6502, ZPG_IDX_IDIR)
    E(0x41, EOR, M6502, ZPG_IDX_IDIR)
    E(0x61, ADC, M6502, ZPG_IDX_IDIR)
    E(0x81, STA, M6502, ZPG_IDX_IDIR)
    E(0xA1, LDA, M6502, ZPG_IDX_IDIR)
    E(0xC1, CMP, M6502, ZPG_IDX_IDIR)
    E(0xE1, SBC, M6502, ZPG_IDX_IDIR)
    E(0x11, ORA, M6502, ZPG_IDIR_IDY)
    E(0x31, AND, M6502, ZPG_IDIR_IDY)
    E(0x51, EOR, M6502, ZPG_IDIR_IDY)
    E(0x71, ADC, M6502, ZPG_IDIR_IDY)
    E(0x91, STA, M6502, ZPG_IDIR_IDY)
    E(0xB1, LDA, M6502, ZPG_IDIR_IDY)
    E(0xD1, CMP, M6502, ZPG_IDIR_IDY)
    E(0xF1, SBC, M6502, ZPG_IDIR_IDY)
    E(0x10, BPL, M6502, REL)
    E(0x30, BMI, M6502, REL)
    E(0x50, BVC, M6502, REL)
    E(0x70, BVS, M6502, REL)
    E(0x90, BCC, M6502, REL)
    E(0xB0, BCS, M6502, REL)
    E(0xD0, BNE, M6502, REL)
    E(0xF0, BEQ, M6502, REL)
    E(0x5A, PHY, W65SC02, IMPL)
    E(0x7A, PLY, W65SC02, IMPL)
    E(0xDA, PHX, W65SC02, IMPL)
    E(0xFA, PLX, W65SC02, IMPL)
    E(0x1A, INC, W65SC02, ACCM)
    E(0x3A, DEC, W65SC02, ACCM)
    E(0x89, BIT, W65SC02, IMM)
    E(0x04, TSB, W65SC02, ZPG)
    E(0x14, TRB, W65SC02, ZPG)
    E(0x64, STZ, W65SC02, ZPG)
    E(0x34, BIT, W65SC02, ZPG_IDX)
    E(0x74, STZ, W65SC02, ZPG_IDX)
    E(0x0C, TSB, W65SC02, ABS)
    E(0x1C, TRB, W65SC02, ABS)
    E(0x9C, STZ, W65SC02, ABS)
    E(0x3C, BIT, W65SC02, ABS_IDX)
    E(0x9E, STZ, W65SC02, ABS_IDX)
    E(0x7C, JMP, W65SC02, ABS_IDX_IDIR)
    E(0x12, ORA, W65SC02, ZPG_IDIR)
    E(0x32, AND, W65SC02, ZPG_IDIR)
    E(0x52, EOR, W65SC02, ZPG_IDIR)
    E(0x72, ADC, W65SC02, ZPG_IDIR)
    E(0x92, STA, W65SC02, ZPG_IDIR)
    E(0xB2, LDA, W65SC02, ZPG_IDIR)
    E(0xD2, CMP, W65SC02, ZPG_IDIR)
    E(0xF2, SBC, W65SC02, ZPG_IDIR)
    E(0x80, BRA, W65SC02, REL)
    E(0x0F, BBR0, R65C02BIT, ZPG_REL)
    E(0x1F, BBR1, R65C02BIT, ZPG_REL)
    E(0x2F, BBR2, R65C02BIT, ZPG_REL)
    E(0x3F, BBR3, R65C02BIT, ZPG_REL)
    E(0x4F, BBR4, R65C02BIT, ZPG_REL)
    E(0x5F, BBR5, R65C02BIT, ZPG_REL)
    E(0x6F, BBR6, R65C02BIT, ZPG_REL)
    E(0x7F, BBR7, R65C02BIT, ZPG_REL)
    E(0x8F, BBS0, R65C02BIT, ZPG_REL)
    E(0x9F, BBS1, R65C02BIT, ZPG_REL)
    E(0xAF, BBS2, R65C02BIT, ZPG_REL)
    E(0xBF, BBS3, R65C02BIT, ZPG_REL)
    E(0xCF, BBS4, R65C02BIT, ZPG_REL)
    E(0xDF, BBS5, R65C02BIT, ZPG_REL)
    E(0xEF, BBS6, R65C02BIT, ZPG_REL)
    E(0xFF, BBS7, R65C02BIT, ZPG_REL)
    E(0xCB, WAI, W65C02S, IMPL)
    E(0xDB, STP, W65C02S, IMPL)
    E(0x07, RMB0, R65C02BIT, ZPG)
    E(0x17, RMB1, R65C02BIT, ZPG)
    E(0x27, RMB2, R65C02BIT, ZPG)
    E(0x37, RMB3, R65C02BIT, ZPG)
    E(0x47, RMB4, R65C02BIT, ZPG)
    E(0x57, RMB5, R65C02BIT, ZPG)
    E(0x67, RMB6, R65C02BIT, ZPG)
    E(0x77, RMB7, R65C02BIT, ZPG)
    E(0x87, SMB0, R65C02BIT, ZPG)
    E(0x97, SMB1, R65C02BIT, ZPG)
    E(0xA7, SMB2, R65C02BIT, ZPG)
    E(0xB7, SMB3, R65C02BIT, ZPG)
    E(0xC7, SMB4, R65C02BIT, ZPG)
    E(0xD7, SMB5, R65C02BIT, ZPG)
    E(0xE7, SMB6, R65C02BIT, ZPG)
    E(0xF7, SMB7, R65C02BIT, ZPG)
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

const Entry *TableM6502::searchEntry(
    const char *name,
    const Entry *table, const Entry *end) {
    for (const Entry *entry = table; entry < end; entry++) {
        if (pgm_strcasecmp(name, entry->name) == 0)
            return entry;
    }
    return nullptr;
}

const Entry *TableM6502::searchEntry(
    const Config::insn_t insnCode, const Entry *table, const Entry *end) {
    for (const Entry *entry = table; entry < end; entry++) {
        if (insnCode == pgm_read_byte(&entry->insnCode))
            return entry;
    }
    return nullptr;
}

Error TableM6502::searchName(
    InsnM6502 &insn, const Entry *table, const Entry *end) const {
    const char *name = insn.name();
    for (const Entry *entry = table;
         entry < end && (entry = searchEntry(name, table, end)) != nullptr;
         entry++) {
        insn.setFlags(pgm_read_byte(&entry->flags));
        if (!insn.supported(_cpuType)) continue;
        insn.setInsnCode(pgm_read_byte(&entry->insnCode));
        return OK;
    }
    return UNKNOWN_INSTRUCTION;
}

static bool acceptAddrMode(AddrMode opr, AddrMode table) {
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

Error TableM6502::searchNameAndAddrMode(
    InsnM6502 &insn, const Entry *table, const Entry *end) const {
    const char *name = insn.name();
    const AddrMode addrMode = insn.addrMode();
    for (const Entry *entry = table;
         entry < end && (entry = searchEntry(name, entry, end)) != nullptr;
         entry++) {
        insn.setFlags(pgm_read_byte(&entry->flags));
        if (!insn.supported(_cpuType)) continue;
        if (acceptAddrMode(addrMode, insn.addrMode())) {
            insn.setInsnCode(pgm_read_byte(&entry->insnCode));
            return OK;
        }
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

Error TableM6502::searchInsnCode(
    InsnM6502 &insn, bool acceptIndirectLong,
    const Entry *table, const Entry *end) const {
    const Config::insn_t insnCode = insn.insnCode();
    for (const Entry *entry = table;
         entry < end && (entry = searchEntry(insnCode, entry, end)) != nullptr;
         entry++) {
        insn.setFlags(pgm_read_byte(&entry->flags));
        if (!insn.supported(_cpuType)) continue;
        if (!acceptAddrMode(insn.addrMode(), acceptIndirectLong))
            continue;
        char name[Config::name_max + 1];
        pgm_strncpy(name, entry->name, sizeof(name));
        insn.setName(name);
        return OK;
    }
    return UNKNOWN_INSTRUCTION;
}

Error TableM6502::searchName(InsnM6502 &insn) const {
    return searchName(insn, ARRAY_RANGE(M6502_TABLE));
}

Error TableM6502::searchNameAndAddrMode(InsnM6502 &insn) const {
    return searchNameAndAddrMode(insn, ARRAY_RANGE(M6502_TABLE));
}

Error TableM6502::searchInsnCode(
    InsnM6502 &insn, bool acceptIndirectLong) const {
    return searchInsnCode(
        insn, acceptIndirectLong, ARRAY_RANGE(M6502_TABLE));
}

const char *TableM6502::listCpu() {
    return "6502, 65sc02, 65c02, w65c02s, 65816";
}

bool TableM6502::setCpu(const char *cpu) {
    if (strcmp(cpu, "6502") == 0) {
        _cpuType = M6502;
        return true;
    }
    if (strcasecmp(cpu, "65SC02") == 0) {
        _cpuType = W65SC02;
        return true;
    }
    if (strcasecmp(cpu, "65C02") == 0) {
        _cpuType = R65C02;
        return true;
    }
    if (strcasecmp(cpu, "W65C02S") == 0) {
        _cpuType = W65C02S;
        return true;
    }
    if (strcmp(cpu, "65816") == 0) {
        _cpuType = W65C816;
        return true;
    }
    return false;
}

class TableM6502 TableM6502;

} // namespace m6502
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

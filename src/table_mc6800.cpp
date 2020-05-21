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

#include "config_mc6800.h"

#include "entry_mc6800.h"
#include "table_mc6800.h"
#include "text_mc6800.h"

#include <string.h>

namespace libasm {
namespace mc6800 {

#define E(_opc, _name, _mode, _adjust, _size)                           \
    { _opc, Entry::_flags(_mode, ADJ_##_adjust, SZ_##_size), TEXT_##_name },

static constexpr Entry MC6800_TABLE[] PROGMEM = {
    E(0x01, NOP,  INH, ZERO, NONE)
    E(0x06, TAP,  INH, ZERO, NONE)
    E(0x07, TPA,  INH, ZERO, NONE)
    E(0x08, INX,  INH, ZERO, NONE)
    E(0x09, DEX,  INH, ZERO, NONE)
    E(0x0A, CLV,  INH, ZERO, NONE)
    E(0x0B, SEV,  INH, ZERO, NONE)
    E(0x0C, CLC,  INH, ZERO, NONE)
    E(0x0D, SEC,  INH, ZERO, NONE)
    E(0x0E, CLI,  INH, ZERO, NONE)
    E(0x0F, SEI,  INH, ZERO, NONE)
    E(0x10, SBA,  INH, ZERO, NONE)
    E(0x11, CBA,  INH, ZERO, NONE)
    E(0x16, TAB,  INH, ZERO, NONE)
    E(0x17, TBA,  INH, ZERO, NONE)
    E(0x19, DAA,  INH, ZERO, NONE)
    E(0x1B, ABA,  INH, ZERO, NONE)
    E(0x20, BRA,  REL, ZERO, BYTE)
    E(0x22, BHI,  REL, ZERO, BYTE)
    E(0x23, BLS,  REL, ZERO, BYTE)
    E(0x24, BHS,  REL, ZERO, BYTE)
    E(0x24, BCC,  REL, ZERO, BYTE)
    E(0x25, BLO,  REL, ZERO, BYTE)
    E(0x25, BCS,  REL, ZERO, BYTE)
    E(0x26, BNE,  REL, ZERO, BYTE)
    E(0x27, BEQ,  REL, ZERO, BYTE)
    E(0x28, BVC,  REL, ZERO, BYTE)
    E(0x29, BVS,  REL, ZERO, BYTE)
    E(0x2A, BPL,  REL, ZERO, BYTE)
    E(0x2B, BMI,  REL, ZERO, BYTE)
    E(0x2C, BGE,  REL, ZERO, BYTE)
    E(0x2D, BLT,  REL, ZERO, BYTE)
    E(0x2E, BGT,  REL, ZERO, BYTE)
    E(0x2F, BLE,  REL, ZERO, BYTE)
    E(0x30, TSX,  INH, ZERO, NONE)
    E(0x31, INS,  INH, ZERO, NONE)
    E(0x32, PUL,  INH, AB01, NONE)
    E(0x32, PULA, INH, ZERO, NONE)
    E(0x33, PULB, INH, ZERO, NONE)
    E(0x34, DES,  INH, ZERO, NONE)
    E(0x35, TXS,  INH, ZERO, NONE)
    E(0x36, PSH,  INH, AB01, NONE)
    E(0x36, PSHA, INH, ZERO, NONE)
    E(0x37, PSHB, INH, ZERO, NONE)
    E(0x39, RTS,  INH, ZERO, NONE)
    E(0x3B, RTI,  INH, ZERO, NONE)
    E(0x3E, WAI,  INH, ZERO, NONE)
    E(0x3F, SWI,  INH, ZERO, NONE)
    E(0x40, NEG,  INH, AB16, NONE)
    E(0x40, NEGA, INH, ZERO, NONE)
    E(0x50, NEGB, INH, ZERO, NONE)
    E(0x43, COM,  INH, AB16, NONE)
    E(0x43, COMA, INH, ZERO, NONE)
    E(0x53, COMB, INH, ZERO, NONE)
    E(0x44, LSR,  INH, AB16, NONE)
    E(0x44, LSRA, INH, ZERO, NONE)
    E(0x54, LSRB, INH, ZERO, NONE)
    E(0x46, ROR,  INH, AB16, NONE)
    E(0x46, RORA, INH, ZERO, NONE)
    E(0x56, RORB, INH, ZERO, NONE)
    E(0x47, ASR,  INH, AB16, NONE)
    E(0x47, ASRA, INH, ZERO, NONE)
    E(0x57, ASRB, INH, ZERO, NONE)
    E(0x48, ASL,  INH, AB16, NONE)
    E(0x48, ASLA, INH, ZERO, NONE)
    E(0x58, ASLB, INH, ZERO, NONE)
    E(0x48, LSL,  INH, AB16, NONE)
    E(0x48, LSLA, INH, ZERO, NONE)
    E(0x58, LSLB, INH, ZERO, NONE)
    E(0x49, ROL,  INH, AB16, NONE)
    E(0x49, ROLA, INH, ZERO, NONE)
    E(0x59, ROLB, INH, ZERO, NONE)
    E(0x4A, DEC,  INH, AB16, NONE)
    E(0x4A, DECA, INH, ZERO, NONE)
    E(0x5A, DECB, INH, ZERO, NONE)
    E(0x4C, INC,  INH, AB16, NONE)
    E(0x4C, INCA, INH, ZERO, NONE)
    E(0x5C, INCB, INH, ZERO, NONE)
    E(0x4D, TST,  INH, AB16, NONE)
    E(0x4D, TSTA, INH, ZERO, NONE)
    E(0x5D, TSTB, INH, ZERO, NONE)
    E(0x4F, CLR,  INH, AB16, NONE)
    E(0x4F, CLRA, INH, ZERO, NONE)
    E(0x5F, CLRB, INH, ZERO, NONE)
    E(0x60, NEG,  IDX, ZERO, BYTE)
    E(0x63, COM,  IDX, ZERO, BYTE)
    E(0x64, LSR,  IDX, ZERO, BYTE)
    E(0x66, ROR,  IDX, ZERO, BYTE)
    E(0x67, ASR,  IDX, ZERO, BYTE)
    E(0x68, ASL,  IDX, ZERO, BYTE)
    E(0x68, LSL,  IDX, ZERO, BYTE)
    E(0x69, ROL,  IDX, ZERO, BYTE)
    E(0x6A, DEC,  IDX, ZERO, BYTE)
    E(0x6C, INC,  IDX, ZERO, BYTE)
    E(0x6D, TST,  IDX, ZERO, BYTE)
    E(0x6E, JMP,  IDX, ZERO, BYTE)
    E(0x6F, CLR,  IDX, ZERO, BYTE)
    E(0x70, NEG,  EXT, ZERO, WORD)
    E(0x73, COM,  EXT, ZERO, WORD)
    E(0x74, LSR,  EXT, ZERO, WORD)
    E(0x76, ROR,  EXT, ZERO, WORD)
    E(0x77, ASR,  EXT, ZERO, WORD)
    E(0x78, ASL,  EXT, ZERO, WORD)
    E(0x78, LSL,  EXT, ZERO, WORD)
    E(0x79, ROL,  EXT, ZERO, WORD)
    E(0x7A, DEC,  EXT, ZERO, WORD)
    E(0x7C, INC,  EXT, ZERO, WORD)
    E(0x7D, TST,  EXT, ZERO, WORD)
    E(0x7E, JMP,  EXT, ZERO, WORD)
    E(0x7F, CLR,  EXT, ZERO, WORD)
    E(0x80, SUB,  IMM, AB64, BYTE)
    E(0x80, SUBA, IMM, ZERO, BYTE)
    E(0xC0, SUBB, IMM, ZERO, BYTE)
    E(0x81, CMP,  IMM, AB64, BYTE)
    E(0x81, CMPA, IMM, ZERO, BYTE)
    E(0xC1, CMPB, IMM, ZERO, BYTE)
    E(0x82, SBC,  IMM, AB64, BYTE)
    E(0x82, SBCA, IMM, ZERO, BYTE)
    E(0xC2, SBCB, IMM, ZERO, BYTE)
    E(0x84, AND,  IMM, AB64, BYTE)
    E(0x84, ANDA, IMM, ZERO, BYTE)
    E(0xC4, ANDB, IMM, ZERO, BYTE)
    E(0x85, BIT,  IMM, AB64, BYTE)
    E(0x85, BITA, IMM, ZERO, BYTE)
    E(0xC5, BITB, IMM, ZERO, BYTE)
    E(0x86, LDA,  IMM, AB64, BYTE)
    E(0x86, LDAA, IMM, ZERO, BYTE)
    E(0xC6, LDAB, IMM, ZERO, BYTE)
    E(0x88, EOR,  IMM, AB64, BYTE)
    E(0x88, EORA, IMM, ZERO, BYTE)
    E(0xC8, EORB, IMM, ZERO, BYTE)
    E(0x89, ADC,  IMM, AB64, BYTE)
    E(0x89, ADCA, IMM, ZERO, BYTE)
    E(0xC9, ADCB, IMM, ZERO, BYTE)
    E(0x8A, ORA,  IMM, AB64, BYTE)
    E(0x8A, ORAA, IMM, ZERO, BYTE)
    E(0xCA, ORAB, IMM, ZERO, BYTE)
    E(0x8B, ADD,  IMM, AB64, BYTE)
    E(0x8B, ADDA, IMM, ZERO, BYTE)
    E(0xCB, ADDB, IMM, ZERO, BYTE)
    E(0x8C, CPX,  IMM, ZERO, WORD)
    E(0x8D, BSR,  REL, ZERO, BYTE)
    E(0x8E, LDS,  IMM, ZERO, WORD)
    E(0x90, SUB,  DIR, AB64, BYTE)
    E(0x90, SUBA, DIR, ZERO, BYTE)
    E(0xD0, SUBB, DIR, ZERO, BYTE)
    E(0x91, CMP,  DIR, AB64, BYTE)
    E(0x91, CMPA, DIR, ZERO, BYTE)
    E(0xD1, CMPB, DIR, ZERO, BYTE)
    E(0x92, SBC,  DIR, AB64, BYTE)
    E(0x92, SBCA, DIR, ZERO, BYTE)
    E(0xD2, SBCB, DIR, ZERO, BYTE)
    E(0x94, AND,  DIR, AB64, BYTE)
    E(0x94, ANDA, DIR, ZERO, BYTE)
    E(0xD4, ANDB, DIR, ZERO, BYTE)
    E(0x95, BIT,  DIR, AB64, BYTE)
    E(0x95, BITA, DIR, ZERO, BYTE)
    E(0xD5, BITB, DIR, ZERO, BYTE)
    E(0x96, LDA,  DIR, AB64, BYTE)
    E(0x96, LDAA, DIR, ZERO, BYTE)
    E(0xD6, LDAB, DIR, ZERO, BYTE)
    E(0x97, STA,  DIR, AB64, BYTE)
    E(0x97, STAA, DIR, ZERO, BYTE)
    E(0xD7, STAB, DIR, ZERO, BYTE)
    E(0x98, EOR,  DIR, AB64, BYTE)
    E(0x98, EORA, DIR, ZERO, BYTE)
    E(0xD8, EORB, DIR, ZERO, BYTE)
    E(0x99, ADC,  DIR, AB64, BYTE)
    E(0x99, ADCA, DIR, ZERO, BYTE)
    E(0xD9, ADCB, DIR, ZERO, BYTE)
    E(0x9A, ORA,  DIR, AB64, BYTE)
    E(0x9A, ORAA, DIR, ZERO, BYTE)
    E(0xDA, ORAB, DIR, ZERO, BYTE)
    E(0x9B, ADD,  DIR, AB64, BYTE)
    E(0x9B, ADDA, DIR, ZERO, BYTE)
    E(0xDB, ADDB, DIR, ZERO, BYTE)
    E(0x9C, CPX,  DIR, ZERO, BYTE)
    E(0x9E, LDS,  DIR, ZERO, BYTE)
    E(0x9F, STS,  DIR, ZERO, BYTE)
    E(0xA0, SUB,  IDX, AB64, BYTE)
    E(0xA0, SUBA, IDX, ZERO, BYTE)
    E(0xE0, SUBB, IDX, ZERO, BYTE)
    E(0xA1, CMP,  IDX, AB64, BYTE)
    E(0xA1, CMPA, IDX, ZERO, BYTE)
    E(0xE1, CMPB, IDX, ZERO, BYTE)
    E(0xA2, SBC,  IDX, AB64, BYTE)
    E(0xA2, SBCA, IDX, ZERO, BYTE)
    E(0xE2, SBCB, IDX, ZERO, BYTE)
    E(0xA4, AND,  IDX, AB64, BYTE)
    E(0xA4, ANDA, IDX, ZERO, BYTE)
    E(0xE4, ANDB, IDX, ZERO, BYTE)
    E(0xA5, BIT,  IDX, AB64, BYTE)
    E(0xA5, BITA, IDX, ZERO, BYTE)
    E(0xE5, BITB, IDX, ZERO, BYTE)
    E(0xA6, LDA,  IDX, AB64, BYTE)
    E(0xA6, LDAA, IDX, ZERO, BYTE)
    E(0xE6, LDAB, IDX, ZERO, BYTE)
    E(0xA7, STA,  IDX, AB64, BYTE)
    E(0xA7, STAA, IDX, ZERO, BYTE)
    E(0xE7, STAB, IDX, ZERO, BYTE)
    E(0xA8, EOR,  IDX, AB64, BYTE)
    E(0xA8, EORA, IDX, ZERO, BYTE)
    E(0xE8, EORB, IDX, ZERO, BYTE)
    E(0xA9, ADC,  IDX, AB64, BYTE)
    E(0xA9, ADCA, IDX, ZERO, BYTE)
    E(0xE9, ADCB, IDX, ZERO, BYTE)
    E(0xAA, ORA,  IDX, AB64, BYTE)
    E(0xAA, ORAA, IDX, ZERO, BYTE)
    E(0xEA, ORAB, IDX, ZERO, BYTE)
    E(0xAB, ADD,  IDX, AB64, BYTE)
    E(0xAB, ADDA, IDX, ZERO, BYTE)
    E(0xEB, ADDB, IDX, ZERO, BYTE)
    E(0xAC, CPX,  IDX, ZERO, BYTE)
    E(0xAD, JSR,  IDX, ZERO, BYTE)
    E(0xAE, LDS,  IDX, ZERO, BYTE)
    E(0xAF, STS,  IDX, ZERO, BYTE)
    E(0xB0, SUB,  EXT, AB64, WORD)
    E(0xB0, SUBA, EXT, ZERO, WORD)
    E(0xF0, SUBB, EXT, ZERO, WORD)
    E(0xB1, CMP,  EXT, AB64, WORD)
    E(0xB1, CMPA, EXT, ZERO, WORD)
    E(0xF1, CMPB, EXT, ZERO, WORD)
    E(0xB2, SBC,  EXT, AB64, WORD)
    E(0xB2, SBCA, EXT, ZERO, WORD)
    E(0xF2, SBCB, EXT, ZERO, WORD)
    E(0xB4, AND,  EXT, AB64, WORD)
    E(0xB4, ANDA, EXT, ZERO, WORD)
    E(0xF4, ANDB, EXT, ZERO, WORD)
    E(0xB5, BIT,  EXT, AB64, WORD)
    E(0xB5, BITA, EXT, ZERO, WORD)
    E(0xF5, BITB, EXT, ZERO, WORD)
    E(0xB6, LDA,  EXT, AB64, WORD)
    E(0xB6, LDAA, EXT, ZERO, WORD)
    E(0xF6, LDAB, EXT, ZERO, WORD)
    E(0xB7, STA,  EXT, AB64, WORD)
    E(0xB7, STAA, EXT, ZERO, WORD)
    E(0xF7, STAB, EXT, ZERO, WORD)
    E(0xB8, EOR,  EXT, AB64, WORD)
    E(0xB8, EORA, EXT, ZERO, WORD)
    E(0xF8, EORB, EXT, ZERO, WORD)
    E(0xB9, ADC,  EXT, AB64, WORD)
    E(0xB9, ADCA, EXT, ZERO, WORD)
    E(0xF9, ADCB, EXT, ZERO, WORD)
    E(0xBA, ORA,  EXT, AB64, WORD)
    E(0xBA, ORAA, EXT, ZERO, WORD)
    E(0xFA, ORAB, EXT, ZERO, WORD)
    E(0xBB, ADD,  EXT, AB64, WORD)
    E(0xBB, ADDA, EXT, ZERO, WORD)
    E(0xFB, ADDB, EXT, ZERO, WORD)
    E(0xBC, CPX,  EXT, ZERO, WORD)
    E(0xBD, JSR,  EXT, ZERO, WORD)
    E(0xBE, LDS,  EXT, ZERO, WORD)
    E(0xBF, STS,  EXT, ZERO, WORD)
    E(0xCE, LDX,  IMM, ZERO, WORD)
    E(0xDE, LDX,  DIR, ZERO, BYTE)
    E(0xDF, STX,  DIR, ZERO, BYTE)
    E(0xEE, LDX,  IDX, ZERO, BYTE)
    E(0xEF, STX,  IDX, ZERO, BYTE)
    E(0xFE, LDX,  EXT, ZERO, WORD)
    E(0xFF, STX,  EXT, ZERO, WORD)
};

static constexpr Entry MC6801_TABLE[] PROGMEM = {
    E(0x04, LSRD, INH, ZERO, NONE)
    E(0x05, ASLD, INH, ZERO, NONE)
    E(0x05, LSLD, INH, ZERO, NONE)
    E(0x21, BRN,  REL, ZERO, BYTE)
    E(0x38, PULX, INH, ZERO, NONE)
    E(0x3A, ABX,  INH, ZERO, NONE)
    E(0x3C, PSHX, INH, ZERO, NONE)
    E(0x3D, MUL,  INH, ZERO, NONE)
    E(0x83, SUBD, IMM, ZERO, WORD)
    E(0x93, SUBD, DIR, ZERO, BYTE)
    E(0x9D, JSR,  DIR, ZERO, NONE)
    E(0xA3, SUBD, IDX, ZERO, BYTE)
    E(0xB3, SUBD, EXT, ZERO, WORD)
    E(0xC3, ADDD, IMM, ZERO, WORD)
    E(0xCC, LDD,  IMM, ZERO, WORD)
    E(0xD3, ADDD, DIR, ZERO, BYTE)
    E(0xDC, LDD,  DIR, ZERO, BYTE)
    E(0xDD, STD,  DIR, ZERO, BYTE)
    E(0xE3, ADDD, IDX, ZERO, BYTE)
    E(0xEC, LDD,  IDX, ZERO, BYTE)
    E(0xED, STD,  IDX, ZERO, BYTE)
    E(0xF3, ADDD, EXT, ZERO, WORD)
    E(0xFC, LDD,  EXT, ZERO, WORD)
    E(0xFD, STD,  EXT, ZERO, WORD)
};

static constexpr Entry HD6301_TABLE[] PROGMEM = {
    E(0x18, XGDX, INH,     ZERO, NONE)
    E(0x1A, SLP,  INH,     ZERO, NONE)
    E(0x61, AIM,  IMM_IDX, ZERO, BYTE)
    E(0x61, BCLR, BIT_IDX, ZERO, BYTE)
    E(0x62, OIM,  IMM_IDX, ZERO, BYTE)
    E(0x62, BSET, BIT_IDX, ZERO, BYTE)
    E(0x65, EIM,  IMM_IDX, ZERO, BYTE)
    E(0x65, BTGL, BIT_IDX, ZERO, BYTE)
    E(0x6B, TIM,  IMM_IDX, ZERO, BYTE)
    E(0x6B, BTST, BIT_IDX, ZERO, BYTE)
    E(0x71, AIM,  IMM_DIR, ZERO, BYTE)
    E(0x71, BCLR, BIT_DIR, ZERO, BYTE)
    E(0x72, OIM,  IMM_DIR, ZERO, BYTE)
    E(0x72, BSET, BIT_DIR, ZERO, BYTE)
    E(0x75, EIM,  IMM_DIR, ZERO, BYTE)
    E(0x75, BTGL, BIT_DIR, ZERO, BYTE)
    E(0x7B, TIM,  IMM_DIR, ZERO, BYTE)
    E(0x7B, BTST, BIT_DIR, ZERO, BYTE)
};

struct TableMc6800::EntryPage {
    const Entry *const table;
    const Entry *const end;
};

static constexpr TableMc6800::EntryPage MC6800_PAGES[] PROGMEM = {
    { ARRAY_RANGE(MC6800_TABLE) },
};

static constexpr TableMc6800::EntryPage MC6801_PAGES[] PROGMEM = {
    { ARRAY_RANGE(MC6801_TABLE) },
    { ARRAY_RANGE(MC6800_TABLE) },
};

static constexpr TableMc6800::EntryPage HD6301_PAGES[] PROGMEM = {
    { ARRAY_RANGE(HD6301_TABLE) },
    { ARRAY_RANGE(MC6801_TABLE) },
    { ARRAY_RANGE(MC6800_TABLE) },
};

static bool acceptAddrMode(AddrMode opr, const Entry *entry) {
    AddrMode table = Entry::_addrMode(pgm_read_byte(&entry->flags));
    if (opr == table) return true;
    if (opr == DIR) return table == EXT || table == REL;
    if (opr == EXT) return table == REL;
    if (opr == BIT_IDX) return table == IDX;
    return false;
}

Error TableMc6800::searchNameAndAddrMode(
    InsnMc6800 &insn, const EntryPage *pages, const EntryPage *end) {
    const AddrMode addrMode = insn.addrMode();
    for (const EntryPage *page = pages; page < end; page++) {
        const Entry *table = reinterpret_cast<Entry *>(pgm_read_ptr(&page->table));
        const Entry *end = reinterpret_cast<Entry *>(pgm_read_ptr(&page->end));
        const Entry *entry = TableBase::searchName<Entry,AddrMode>(
            insn.name(), addrMode, table, end, acceptAddrMode);
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
    const InsnAdjust iAdjust =
        Entry::_insnAdjust(pgm_read_byte(&entry->flags));
    switch (iAdjust) {
    case ADJ_AB01: return opCode & ~1;
    case ADJ_AB16: return opCode & ~0x10;
    case ADJ_AB64: return opCode & ~0x40;
    default: return opCode;
    }
}

const Entry *TableMc6800::searchOpCode(
    InsnMc6800 &insn, const EntryPage *pages, const EntryPage *end) {
    for (const EntryPage *page = pages; page < end; page++) {
        const Entry *table = reinterpret_cast<Entry *>(pgm_read_ptr(&page->table));
        const Entry *end = reinterpret_cast<Entry *>(pgm_read_ptr(&page->end));
        const Entry *entry = TableBase::searchCode<Entry,Config::opcode_t>(
            insn.opCode(), table, end, tableCode);
        if (entry) return entry;
    }
    return nullptr;
}

Error TableMc6800::searchNameAndAddrMode(InsnMc6800 &insn) const {
    return searchNameAndAddrMode(insn, _table, _end);
}

Error TableMc6800::searchOpCode(InsnMc6800 &insn) const {
    const Entry *entry = searchOpCode(insn, _table, _end);
    if (!entry) return UNKNOWN_INSTRUCTION;
    insn.setFlags(pgm_read_byte(&entry->flags));
    TableBase::setName(insn.insn(), entry->name, Config::NAME_MAX);
    return OK;
}

Error TableMc6800::searchOpCodeAlias(InsnMc6800 &insn) const {
    const Entry *entry = searchOpCode(insn, _table, _end);
    if (!entry) return UNKNOWN_INSTRUCTION;
    entry += 1;
    if (pgm_read_byte(&entry->opCode) != insn.opCode())
        return UNKNOWN_INSTRUCTION;
    insn.setFlags(pgm_read_byte(&entry->flags));
    TableBase::setName(insn.insn(), entry->name, Config::NAME_MAX);
    return OK;
}

TableMc6800::TableMc6800() {
    setCpu(MC6800);
}

bool TableMc6800::setCpu(CpuType cpuType) {
    _cpuType = cpuType;
    if (cpuType == MC6800) {
        _table = ARRAY_BEGIN(MC6800_PAGES);
        _end = ARRAY_END(MC6800_PAGES);
    } else if (cpuType == MC6801) {
        _table = ARRAY_BEGIN(MC6801_PAGES);
        _end = ARRAY_END(MC6801_PAGES);
    } else {
        _table = ARRAY_BEGIN(HD6301_PAGES);
        _end = ARRAY_END(HD6301_PAGES);
    }
    return true;
}

const char *TableMc6800::getCpu() {
    if (_cpuType == MC6800)
        return "6800";
    return _cpuType == MC6801 ? "6801" : "6301";
}

bool TableMc6800::setCpu(const char *cpu) {
    const char *p;
    p = cpu + (strncasecmp(cpu, "MC", 2) ? 0 : 2);
    if (strcmp(p, "6800") == 0)
        return setCpu(MC6800);
    if (strcmp(p, "6801") == 0)
        return setCpu(MC6801);
    p = cpu + (strncasecmp(cpu, "HD", 2) ? 0 : 2);
    if (strcmp(p, "6301") == 0)
        return setCpu(HD6301);
    return false;
}

class TableMc6800 TableMc6800;

} // namespace mc6800
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

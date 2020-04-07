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
    E(0x01, NOP, INH, ZERO, NONE)
    E(0x06, TAP, INH, ZERO, NONE)
    E(0x07, TPA, INH, ZERO, NONE)
    E(0x08, INX, INH, ZERO, NONE)
    E(0x09, DEX, INH, ZERO, NONE)
    E(0x0A, CLV, INH, ZERO, NONE)
    E(0x0B, SEV, INH, ZERO, NONE)
    E(0x0C, CLC, INH, ZERO, NONE)
    E(0x0D, SEC, INH, ZERO, NONE)
    E(0x0E, CLI, INH, ZERO, NONE)
    E(0x0F, SEI, INH, ZERO, NONE)
    E(0x10, SBA, INH, ZERO, NONE)
    E(0x11, CBA, INH, ZERO, NONE)
    E(0x16, TAB, INH, ZERO, NONE)
    E(0x17, TBA, INH, ZERO, NONE)
    E(0x19, DAA, INH, ZERO, NONE)
    E(0x1B, ABA, INH, ZERO, NONE)
    E(0x20, BRA, REL, ZERO, BYTE)
    E(0x22, BHI, REL, ZERO, BYTE)
    E(0x23, BLS, REL, ZERO, BYTE)
    E(0x24, BHS, REL, ZERO, BYTE)
    E(0x24, BCC, REL, ZERO, BYTE)
    E(0x25, BLO, REL, ZERO, BYTE)
    E(0x25, BCS, REL, ZERO, BYTE)
    E(0x26, BNE, REL, ZERO, BYTE)
    E(0x27, BEQ, REL, ZERO, BYTE)
    E(0x28, BVC, REL, ZERO, BYTE)
    E(0x29, BVS, REL, ZERO, BYTE)
    E(0x2A, BPL, REL, ZERO, BYTE)
    E(0x2B, BMI, REL, ZERO, BYTE)
    E(0x2C, BGE, REL, ZERO, BYTE)
    E(0x2D, BLT, REL, ZERO, BYTE)
    E(0x2E, BGT, REL, ZERO, BYTE)
    E(0x2F, BLE, REL, ZERO, BYTE)
    E(0x30, TSX, INH, ZERO, NONE)
    E(0x31, INS, INH, ZERO, NONE)
    E(0x32, PUL, ACC, AB01, NONE)
    E(0x34, DES, INH, ZERO, NONE)
    E(0x35, TXS, INH, ZERO, NONE)
    E(0x36, PSH, ACC, AB01, NONE)
    E(0x39, RTS, INH, ZERO, NONE)
    E(0x3B, RTI, INH, ZERO, NONE)
    E(0x3E, WAI, INH, ZERO, NONE)
    E(0x3F, SWI, INH, ZERO, NONE)
    E(0x40, NEG, ACC, AB16, NONE)
    E(0x43, COM, ACC, AB16, NONE)
    E(0x44, LSR, ACC, AB16, NONE)
    E(0x46, ROR, ACC, AB16, NONE)
    E(0x47, ASR, ACC, AB16, NONE)
    E(0x48, ASL, ACC, AB16, NONE)
    E(0x48, LSL, ACC, AB16, NONE)
    E(0x49, ROL, ACC, AB16, NONE)
    E(0x4A, DEC, ACC, AB16, NONE)
    E(0x4C, INC, ACC, AB16, NONE)
    E(0x4D, TST, ACC, AB16, NONE)
    E(0x4F, CLR, ACC, AB16, NONE)
    E(0x60, NEG, IDX, ZERO, BYTE)
    E(0x63, COM, IDX, ZERO, BYTE)
    E(0x64, LSR, IDX, ZERO, BYTE)
    E(0x66, ROR, IDX, ZERO, BYTE)
    E(0x67, ASR, IDX, ZERO, BYTE)
    E(0x68, ASL, IDX, ZERO, BYTE)
    E(0x68, LSL, IDX, ZERO, BYTE)
    E(0x69, ROL, IDX, ZERO, BYTE)
    E(0x6A, DEC, IDX, ZERO, BYTE)
    E(0x6C, INC, IDX, ZERO, BYTE)
    E(0x6D, TST, IDX, ZERO, BYTE)
    E(0x6E, JMP, IDX, ZERO, BYTE)
    E(0x6F, CLR, IDX, ZERO, BYTE)
    E(0x70, NEG, EXT, ZERO, WORD)
    E(0x73, COM, EXT, ZERO, WORD)
    E(0x74, LSR, EXT, ZERO, WORD)
    E(0x76, ROR, EXT, ZERO, WORD)
    E(0x77, ASR, EXT, ZERO, WORD)
    E(0x78, ASL, EXT, ZERO, WORD)
    E(0x78, LSL, EXT, ZERO, WORD)
    E(0x79, ROL, EXT, ZERO, WORD)
    E(0x7A, DEC, EXT, ZERO, WORD)
    E(0x7C, INC, EXT, ZERO, WORD)
    E(0x7D, TST, EXT, ZERO, WORD)
    E(0x7E, JMP, EXT, ZERO, WORD)
    E(0x7F, CLR, EXT, ZERO, WORD)
    E(0x80, SUB, IMM, AB64, BYTE)
    E(0x81, CMP, IMM, AB64, BYTE)
    E(0x82, SBC, IMM, AB64, BYTE)
    E(0x84, AND, IMM, AB64, BYTE)
    E(0x85, BIT, IMM, AB64, BYTE)
    E(0x86, LDA, IMM, AB64, BYTE)
    E(0x88, EOR, IMM, AB64, BYTE)
    E(0x89, ADC, IMM, AB64, BYTE)
    E(0x8A, ORA, IMM, AB64, BYTE)
    E(0x8B, ADD, IMM, AB64, BYTE)
    E(0x8C, CPX, IMM, ZERO, WORD)
    E(0x8D, BSR, REL, ZERO, BYTE)
    E(0x8E, LDS, IMM, ZERO, WORD)
    E(0x90, SUB, DIR, AB64, BYTE)
    E(0x91, CMP, DIR, AB64, BYTE)
    E(0x92, SBC, DIR, AB64, BYTE)
    E(0x94, AND, DIR, AB64, BYTE)
    E(0x95, BIT, DIR, AB64, BYTE)
    E(0x96, LDA, DIR, AB64, BYTE)
    E(0x97, STA, DIR, AB64, BYTE)
    E(0x98, EOR, DIR, AB64, BYTE)
    E(0x99, ADC, DIR, AB64, BYTE)
    E(0x9A, ORA, DIR, AB64, BYTE)
    E(0x9B, ADD, DIR, AB64, BYTE)
    E(0x9C, CPX, DIR, ZERO, BYTE)
    E(0x9E, LDS, DIR, ZERO, BYTE)
    E(0x9F, STS, DIR, ZERO, BYTE)
    E(0xA0, SUB, IDX, AB64, BYTE)
    E(0xA1, CMP, IDX, AB64, BYTE)
    E(0xA2, SBC, IDX, AB64, BYTE)
    E(0xA4, AND, IDX, AB64, BYTE)
    E(0xA5, BIT, IDX, AB64, BYTE)
    E(0xA6, LDA, IDX, AB64, BYTE)
    E(0xA7, STA, IDX, AB64, BYTE)
    E(0xA8, EOR, IDX, AB64, BYTE)
    E(0xA9, ADC, IDX, AB64, BYTE)
    E(0xAA, ORA, IDX, AB64, BYTE)
    E(0xAB, ADD, IDX, AB64, BYTE)
    E(0xAC, CPX, IDX, ZERO, BYTE)
    E(0xAD, JSR, IDX, ZERO, BYTE)
    E(0xAE, LDS, IDX, ZERO, BYTE)
    E(0xAF, STS, IDX, ZERO, BYTE)
    E(0xB0, SUB, EXT, AB64, WORD)
    E(0xB1, CMP, EXT, AB64, WORD)
    E(0xB2, SBC, EXT, AB64, WORD)
    E(0xB4, AND, EXT, AB64, WORD)
    E(0xB5, BIT, EXT, AB64, WORD)
    E(0xB6, LDA, EXT, AB64, WORD)
    E(0xB7, STA, EXT, AB64, WORD)
    E(0xB8, EOR, EXT, AB64, WORD)
    E(0xB9, ADC, EXT, AB64, WORD)
    E(0xBA, ORA, EXT, AB64, WORD)
    E(0xBB, ADD, EXT, AB64, WORD)
    E(0xBC, CPX, EXT, ZERO, WORD)
    E(0xBD, JSR, EXT, ZERO, WORD)
    E(0xBE, LDS, EXT, ZERO, WORD)
    E(0xBF, STS, EXT, ZERO, WORD)
    E(0xCE, LDX, IMM, ZERO, WORD)
    E(0xDE, LDX, DIR, ZERO, BYTE)
    E(0xDF, STX, DIR, ZERO, BYTE)
    E(0xEE, LDX, IDX, ZERO, BYTE)
    E(0xEF, STX, IDX, ZERO, BYTE)
    E(0xFE, LDX, EXT, ZERO, WORD)
    E(0xFF, STX, EXT, ZERO, WORD)
};

const Entry *TableMc6800::searchEntry(
    const char *name,
    const Entry *table, const Entry *end) {
    for (const Entry *entry = table; entry < end; entry++) {
        if (pgm_strcasecmp(name, entry->name) == 0)
            return entry;
    }
    return nullptr;
}

const Entry *TableMc6800::searchEntry(
    const target::insn_t insnCode,
    const Entry *table, const Entry *end) {
    for (const Entry *entry = table; entry < end; entry++) {
        target::insn_t opc = insnCode;
        const InsnAdjust iAdjust = Entry::_insnAdjust(pgm_read_byte(&entry->flags));
        switch (iAdjust) {
        case ADJ_AB01: opc &= ~1; break;
        case ADJ_AB16: opc &= ~0x10; break;
        case ADJ_AB64: opc &= ~0x40; break;
        default: break;
        }
        if (opc == pgm_read_byte(&entry->opc))
            return entry;
    }
    return nullptr;
}

Error TableMc6800::searchName(InsnMc6800 &insn) const {
    const Entry *entry = searchEntry(insn.name(), ARRAY_RANGE(MC6800_TABLE));
    if (!entry) return UNKNOWN_INSTRUCTION;
    insn.setInsnCode(pgm_read_byte(&entry->opc));
    insn.setFlags(pgm_read_byte(&entry->flags));
    return OK;
}

static bool acceptAddrMode(AddrMode opr, AddrMode table) {
    if (opr == table) return true;
    if (opr == DIR) return table == EXT;
    return false;
}

Error TableMc6800::searchNameAndAddrMode(InsnMc6800 &insn) const {
    const AddrMode addrMode = insn.addrMode();
    const Entry *end = ARRAY_END(MC6800_TABLE);
    for (const Entry *entry = ARRAY_BEGIN(MC6800_TABLE);
         entry < end && (entry = searchEntry(insn.name(), entry, end));
         entry++) {
        const host::uint_t flags = pgm_read_byte(&entry->flags);
        if (acceptAddrMode(addrMode, Entry::_addrMode(flags))) {
            insn.setInsnCode(pgm_read_byte(&entry->opc));
            insn.setFlags(flags);
            return OK;
        }
    }
    return UNKNOWN_INSTRUCTION;
}

Error TableMc6800::searchInsnCode(InsnMc6800 &insn) const {
    const Entry *entry = searchEntry(insn.insnCode(), ARRAY_RANGE(MC6800_TABLE));
    if (!entry) return UNKNOWN_INSTRUCTION;
    insn.setFlags(pgm_read_byte(&entry->flags));
    char name[Entry::name_max + 1];
    pgm_strncpy(name, entry->name, sizeof(name));
    insn.setName(name);
    return OK;
}

const char *TableMc6800::listCpu() {
    return "6800, 6802";
}

bool TableMc6800::setCpu(const char *cpu) {
    return strcmp(cpu, "6800") == 0
        || strcmp(cpu, "6802") == 0;
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

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

#include "config_z8.h"

#include "entry_z8.h"
#include "table_z8.h"
#include "text_z8.h"

#include <ctype.h>
#include <string.h>

namespace libasm {
namespace z8 {

#define E(_opc, _name, _dst, _src)                                  \
    { _opc, Entry::_flags(_dst, _src, M_NO, P0), TEXT_##_name },
#define P(_opc, _post, _name, _dst, _src, _ext)                     \
    { _opc, Entry::_flags(_dst, _src, _ext, _post), TEXT_##_name },

static constexpr Entry TABLE_COMMON[] PROGMEM = {
    E(0x08, LD,   M_r,   M_R)
    E(0x09, LD,   M_R,   M_r)
    E(0x0C, LD,   M_r,   M_IM)
    E(0xE4, LD,   M_R,   M_R)
    E(0xE5, LD,   M_R,   M_IR)
    E(0xE6, LD,   M_R,   M_IM)
    E(0xF5, LD,   M_IR,  M_R)
    E(0x0A, DJNZ, M_r,   M_RA)
    E(0x8B, JR,   M_RA,  M_NO)
    E(0x0B, JR,   M_cc,  M_RA)
    E(0x8D, JP,   M_DA,  M_NO)
    E(0x0D, JP,   M_cc,  M_DA)
    E(0x30, JP,   M_IRR, M_NO)
    E(0x02, ADD,  M_r,   M_r)
    E(0x03, ADD,  M_r,   M_Ir)
    E(0x04, ADD,  M_R,   M_R)
    E(0x05, ADD,  M_R,   M_IR)
    E(0x06, ADD,  M_R,   M_IM)
    E(0x12, ADC,  M_r,   M_r)
    E(0x13, ADC,  M_r,   M_Ir)
    E(0x14, ADC,  M_R,   M_R)
    E(0x15, ADC,  M_R,   M_IR)
    E(0x16, ADC,  M_R,   M_IM)
    E(0x22, SUB,  M_r,   M_r)
    E(0x23, SUB,  M_r,   M_Ir)
    E(0x24, SUB,  M_R,   M_R)
    E(0x25, SUB,  M_R,   M_IR)
    E(0x26, SUB,  M_R,   M_IM)
    E(0x32, SBC,  M_r,   M_r)
    E(0x33, SBC,  M_r,   M_Ir)
    E(0x34, SBC,  M_R,   M_R)
    E(0x35, SBC,  M_R,   M_IR)
    E(0x36, SBC,  M_R,   M_IM)
    E(0x42, OR,   M_r,   M_r)
    E(0x43, OR,   M_r,   M_Ir)
    E(0x44, OR,   M_R,   M_R)
    E(0x45, OR,   M_R,   M_IR)
    E(0x46, OR,   M_R,   M_IM)
    E(0x52, AND,  M_r,   M_r)
    E(0x53, AND,  M_r,   M_Ir)
    E(0x54, AND,  M_R,   M_R)
    E(0x55, AND,  M_R,   M_IR)
    E(0x56, AND,  M_R,   M_IM)
    E(0x62, TCM,  M_r,   M_r)
    E(0x63, TCM,  M_r,   M_Ir)
    E(0x64, TCM,  M_R,   M_R)
    E(0x65, TCM,  M_R,   M_IR)
    E(0x66, TCM,  M_R,   M_IM)
    E(0x72, TM,   M_r,   M_r)
    E(0x73, TM,   M_r,   M_Ir)
    E(0x74, TM,   M_R,   M_R)
    E(0x75, TM,   M_R,   M_IR)
    E(0x76, TM,   M_R,   M_IM)
    E(0xA2, CP,   M_r,   M_r)
    E(0xA3, CP,   M_r,   M_Ir)
    E(0xA4, CP,   M_R,   M_R)
    E(0xA5, CP,   M_R,   M_IR)
    E(0xA6, CP,   M_R,   M_IM)
    E(0xB2, XOR,  M_r,   M_r)
    E(0xB3, XOR,  M_r,   M_Ir)
    E(0xB4, XOR,  M_R,   M_R)
    E(0xB5, XOR,  M_R,   M_IR)
    E(0xB6, XOR,  M_R,   M_IM)
    E(0x00, DEC,  M_R,   M_NO)
    E(0x01, DEC,  M_IR,  M_NO)
    E(0x10, RLC,  M_R,   M_NO)
    E(0x11, RLC,  M_IR,  M_NO)
    E(0x0E, INC,  M_r,   M_NO)
    E(0x20, INC,  M_R,   M_NO)
    E(0x21, INC,  M_IR,  M_NO)
    E(0x40, DA,   M_R,   M_NO)
    E(0x41, DA,   M_IR,  M_NO)
    E(0x50, POP,  M_R,   M_NO)
    E(0x51, POP,  M_IR,  M_NO)
    E(0x60, COM,  M_R,   M_NO)
    E(0x61, COM,  M_IR,  M_NO)
    E(0x70, PUSH, M_R,   M_NO)
    E(0x71, PUSH, M_IR,  M_NO)
    E(0x80, DECW, M_RR,  M_NO)
    E(0x81, DECW, M_IR,  M_NO)
    E(0x90, RL,   M_R,   M_NO)
    E(0x91, RL,   M_IR,  M_NO)
    E(0xA0, INCW, M_RR,  M_NO)
    E(0xA1, INCW, M_IR,  M_NO)
    E(0xB0, CLR,  M_R,   M_NO)
    E(0xB1, CLR,  M_IR,  M_NO)
    E(0xC0, RRC,  M_R,   M_NO)
    E(0xC1, RRC,  M_IR,  M_NO)
    E(0xD0, SRA,  M_R,   M_NO)
    E(0xD1, SRA,  M_IR,  M_NO)
    E(0xE0, RR,   M_R,   M_NO)
    E(0xE1, RR,   M_IR,  M_NO)
    E(0xF0, SWAP, M_R,   M_NO)
    E(0xF1, SWAP, M_IR,  M_NO)
    E(0x8F, DI,   M_NO,  M_NO)
    E(0x9F, EI,   M_NO,  M_NO)
    E(0xAF, RET,  M_NO,  M_NO)
    E(0xBF, IRET, M_NO,  M_NO)
    E(0xCF, RCF,  M_NO,  M_NO)
    E(0xDF, SCF,  M_NO,  M_NO)
    E(0xEF, CCF,  M_NO,  M_NO)
    E(0xFF, NOP,  M_NO,  M_NO)
};

static constexpr Entry TABLE_Z8[] PROGMEM = {
    E(0x31, SRP,  M_IM,  M_NO)
    E(0x07, ADD,  M_IR,  M_IM)
    E(0x17, ADC,  M_IR,  M_IM)
    E(0x27, SUB,  M_IR,  M_IM)
    E(0x37, SBC,  M_IR,  M_IM)
    E(0x47, OR,   M_IR,  M_IM)
    E(0x57, AND,  M_IR,  M_IM)
    E(0x67, TCM,  M_IR,  M_IM)
    E(0x77, TM,   M_IR,  M_IM)
    E(0xA7, CP,   M_IR,  M_IM)
    E(0xB7, XOR,  M_IR,  M_IM)
    E(0xC7, LD,   M_r,   M_X)
    E(0xD7, LD,   M_X,   M_r)
    E(0xE3, LD,   M_r,   M_Ir)
    E(0xE7, LD,   M_IR,  M_IM)
    E(0xF3, LD,   M_Ir,  M_r)
    E(0x82, LDE,  M_r,   M_Irr)
    E(0x92, LDE,  M_Irr, M_r)
    E(0x83, LDEI, M_Ir,  M_Irr)
    E(0x93, LDEI, M_Irr, M_Ir)
    E(0xC2, LDC,  M_r,   M_Irr)
    E(0xD2, LDC,  M_Irr, M_r)
    E(0xC3, LDCI, M_Ir,  M_Irr)
    E(0xD3, LDCI, M_Irr, M_Ir)
    E(0xD6, CALL, M_DA,  M_NO)
    E(0xD4, CALL, M_IRR, M_NO)
};

static constexpr Entry TABLE_Z86C[] PROGMEM = {
    E(0x6F, STOP, M_NO,  M_NO)
    E(0x7F, HALT, M_NO,  M_NO)
};

static constexpr Entry TABLE_SUPER8[] PROGMEM = {
    E(0x0F, NEXT, M_NO,  M_NO)
    E(0x1F, ENTER,M_NO,  M_NO)
    E(0x2F, EXIT, M_NO,  M_NO)
    E(0x3F, WFI,  M_NO,  M_NO)
    E(0x4F, SB0,  M_NO,  M_NO)
    E(0x5F, SB1,  M_NO,  M_NO)
    E(0xF6, CALL, M_DA,  M_NO)
    E(0xF4, CALL, M_IRR, M_NO)
    E(0xD4, CALL, M_IM,  M_NO)
    E(0x87, LD,   M_r,   M_X)
    E(0x97, LD,   M_X,   M_r)
    E(0xC7, LD,   M_r,   M_Ir)
    E(0xD7, LD,   M_Ir,  M_r)
    E(0xD6, LD,   M_IR,  M_IM)
    E(0xC4, LDW,  M_RR,  M_RR)
    E(0xC5, LDW,  M_RR,  M_IR)
    E(0xC6, LDW,  M_RR,  M_IML)
    E(0x94, DIV,  M_RR,  M_R)
    E(0x95, DIV,  M_RR,  M_IR)
    E(0x96, DIV,  M_RR,  M_IM)
    E(0x84, MULT, M_RR,  M_R)
    E(0x85, MULT, M_RR,  M_IR)
    E(0x86, MULT, M_RR,  M_IM)
    E(0x82, PUSHUD, M_IR, M_R)
    E(0x83, PUSHUI, M_IR, M_R)
    E(0x92, POPUD,  M_R,  M_IR)
    E(0x93, POPUI,  M_R,  M_IR)
    P(0xC2, P0, CPIJE,  M_r, M_Ir, M_RA)
    P(0xD2, P0, CPIJNE, M_r, M_Ir, M_RA)
};

static constexpr Entry TABLE_SUPER8_POST[] PROGMEM {
    P(0xA7, P4_0, LDC,   M_r,   M_DA,  M_NO)
    P(0xA7, P4_1, LDE,   M_r,   M_DA,  M_NO)
    P(0xB7, P4_0, LDC,   M_DA,  M_r,   M_NO)
    P(0xB7, P4_1, LDE,   M_DA,  M_r,   M_NO)
    P(0xE7, P1_0, LDC,   M_r,   M_XS,  M_NO)
    P(0xE7, P1_1, LDE,   M_r,   M_XS,  M_NO)
    P(0xF7, P1_0, LDC,   M_XS,  M_r,   M_NO)
    P(0xF7, P1_1, LDE,   M_XS,  M_r,   M_NO)
    P(0xA7, P1_0, LDC,   M_r,   M_XL,  M_NO)
    P(0xA7, P1_1, LDE,   M_r,   M_XL,  M_NO)
    P(0xB7, P1_0, LDC,   M_XL,  M_r,   M_NO)
    P(0xB7, P1_1, LDE,   M_XL,  M_r,   M_NO)
    P(0x31, P2_0, SRP,   M_IM,  M_NO,  M_NO)
    P(0x31, P2_2, SRP0,  M_IM,  M_NO,  M_NO)
    P(0x31, P2_1, SRP1,  M_IM,  M_NO,  M_NO)
    P(0xC3, P1_0, LDC,   M_r,   M_Irr, M_NO)
    P(0xC3, P1_1, LDE,   M_r,   M_Irr, M_NO)
    P(0xD3, P1_0, LDC,   M_Irr, M_r,   M_NO)
    P(0xD3, P1_1, LDE,   M_Irr, M_r,   M_NO)
    P(0xE2, P1_0, LDCD,  M_r,   M_Irr, M_NO)
    P(0xE2, P1_1, LDED,  M_r,   M_Irr, M_NO)
    P(0xE3, P1_0, LDCI,  M_r,   M_Irr, M_NO)
    P(0xE3, P1_1, LDEI,  M_r,   M_Irr, M_NO)
    P(0xF2, P1_0, LDCPD, M_Irr, M_r,   M_NO)
    P(0xF2, P1_1, LDEPD, M_Irr, M_r,   M_NO)
    P(0xF3, P1_0, LDCPI, M_Irr, M_r,   M_NO)
    P(0xF3, P1_1, LDEPI, M_Irr, M_r,   M_NO)
    P(0x17, P1_0, BCP,   M_r,   M_R,   M_IMb)
    P(0x57, P1_0, BITC,  M_r,   M_IMb, M_NO)
    P(0x77, P1_0, BITR,  M_r,   M_IMb, M_NO)
    P(0x77, P1_1, BITS,  M_r,   M_IMb, M_NO)
    P(0x07, P1_0, BOR,   M_r,   M_R,   M_IMb)
    P(0x07, P1_1, BOR,   M_R,   M_IMb, M_r)
    P(0x27, P1_0, BXOR,  M_r,   M_R,   M_IMb)
    P(0x27, P1_1, BXOR,  M_R,   M_IMb, M_r)
    P(0x47, P1_0, LDB,   M_r,   M_R,   M_IMb)
    P(0x47, P1_1, LDB,   M_R,   M_IMb, M_r)
    P(0x67, P1_0, BAND,  M_r,   M_R,   M_IMb)
    P(0x67, P1_1, BAND,  M_R,   M_IMb, M_r)
    P(0x37, P1_0, BTJRF, M_RA,  M_r,   M_IMb)
    P(0x37, P1_1, BTJRT, M_RA,  M_r,   M_IMb)
};

struct TableZ8::EntryPage {
    const Entry *const table;
    const Entry *const end;
};

static constexpr TableZ8::EntryPage Z8_PAGES[] PROGMEM = {
    { ARRAY_RANGE(TABLE_Z8) },
    { ARRAY_RANGE(TABLE_COMMON) },
};

static constexpr TableZ8::EntryPage Z86C_PAGES[] PROGMEM = {
    { ARRAY_RANGE(TABLE_Z8) },
    { ARRAY_RANGE(TABLE_COMMON) },
    { ARRAY_RANGE(TABLE_Z86C) },
};

static constexpr TableZ8::EntryPage SUPER8_PAGES[] PROGMEM = {
    { ARRAY_RANGE(TABLE_SUPER8) },
    { ARRAY_RANGE(TABLE_SUPER8_POST) },
    { ARRAY_RANGE(TABLE_COMMON) },
};

static bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table) return true;
    if (opr == M_IM)  return table == M_IMb || table == M_IML;
    if (opr == M_DA)  return table == M_RA;
    if (opr == M_R)   return table == M_DA || table == M_RA;
    if (opr == M_RR)  return table == M_DA || table == M_RA
                          || table == M_R;
    if (opr == M_IRR) return table == M_IR;
    if (opr == M_r)   return table == M_R;
    if (opr == M_rr)  return table == M_RR;
    if (opr == M_Ir)  return table == M_IR;
    if (opr == M_Irr) return table == M_IRR;
    if (opr == M_W)   return table == M_DA || table == M_RA
                          || table == M_r  || table == M_R;
    if (opr == M_WW)  return table == M_DA || table == M_RA
                          || table == M_rr || table == M_RR
                          || table == M_r  || table == M_R;
    if (opr == M_IW)  return table == M_Ir || table == M_IR;
    if (opr == M_IWW) return table == M_Irr || table == M_IRR
                          || table == M_Ir  || table == M_IR;
    if (opr == M_XS)  return table == M_X;
    if (opr == M_XL)  return table == M_X;
    return false;
}

static bool acceptModes(uint16_t flags, const Entry *entry) {
    const uint16_t table = pgm_read_word(&entry->flags);
    return acceptMode(Entry::_dstMode(flags), Entry::_dstMode(table))
        && acceptMode(Entry::_srcMode(flags), Entry::_srcMode(table))
        && acceptMode(Entry::_extMode(flags), Entry::_extMode(table));
}

Error TableZ8::searchName(
    InsnZ8 &insn, const EntryPage *pages, const EntryPage *end) const {
    uint8_t count = 0;
    for (const EntryPage *page = pages; page < end; page++) {
        const Entry *table =
            reinterpret_cast<Entry *>(pgm_read_ptr(&page->table));
        const Entry *end = reinterpret_cast<Entry *>(pgm_read_ptr(&page->end));
        const uint16_t flags = Entry::_flags(
            insn.dstMode(), insn.srcMode(), insn.extMode(), insn.postFormat());
        const Entry *entry = TableBase::searchName<Entry,uint16_t>(
            insn.name(), flags, table, end, acceptModes, count);
        if (entry) {
            insn.setOpCode(pgm_read_byte(&entry->opCode));
            insn.setFlags(pgm_read_word(&entry->flags));
            return OK;
        }
    }
    return count == 0 ? UNKNOWN_INSTRUCTION : OPERAND_NOT_ALLOWED;
}

static Config::opcode_t maskCode(
    Config::opcode_t opCode, const Entry *entry) {
    const Config::opcode_t table = pgm_read_byte(&entry->opCode);
    return InsnZ8::operandInOpCode(table) ? opCode & 0x0f : opCode;
}

static bool matchPostByte(const InsnZ8 &insn) {
    const uint8_t post = insn.post();
    switch (insn.postFormat()) {
    case P1_0:  return (post & 1) == 0;
    case P1_1:  return (post & 1) == 1;
    case P2_0: return (post & 3) == 0;
    case P2_1: return (post & 3) == 1;
    case P2_2: return (post & 3) == 2;
    case P4_0: return (post & 0xF) == 0;
    case P4_1: return (post & 0xF) == 1;
    default:   return true;
    }
}

Error TableZ8::searchOpCode(
    InsnZ8 &insn, DisMemory &memory,
    const EntryPage *pages, const EntryPage *end) const {
    for (const EntryPage *page = pages; page < end; page++) {
        const Entry *end = reinterpret_cast<Entry *>(pgm_read_ptr(&page->end));
        for (const Entry *entry =
                 reinterpret_cast<Entry *>(pgm_read_ptr(&page->table));
             entry < end
                 && (entry = TableBase::searchCode<Entry,Config::opcode_t>(
                         insn.opCode(), entry, end, maskCode)) != nullptr;
             entry++) {
            insn.setFlags(pgm_read_word(&entry->flags));
            if (insn.postFormat()) {
                if (insn.length() < 2) {
                    insn.readPost(memory);
                    if (insn.getError()) return NO_MEMORY;
                }
                if (!matchPostByte(insn)) continue;
            }
            const /*PROGMEM*/ char *name =
                reinterpret_cast<const char *>(pgm_read_ptr(&entry->name));
            insn.setName_P(name);
            return OK;
        }
    }
    return UNKNOWN_INSTRUCTION;
}

Error TableZ8::searchName(InsnZ8 &insn) const {
    return _error.setError(searchName(insn, _table, _end));
}

Error TableZ8::searchOpCode(InsnZ8 &insn, DisMemory &memory) const {
    return _error.setError(searchOpCode(insn, memory, _table, _end));
}

TableZ8::TableZ8() {
    setCpu(Z8);
}

bool TableZ8::isSuper8() const {
    return _cpuType == SUPER8;
}

bool TableZ8::setCpu(CpuType cpuType) {
    _cpuType = cpuType;
    if (cpuType == Z8) {
        _table = ARRAY_BEGIN(Z8_PAGES);
        _end = ARRAY_END(Z8_PAGES);
        return true;
    }
    if (cpuType == Z86C) {
        _table = ARRAY_BEGIN(Z86C_PAGES);
        _end = ARRAY_END(Z86C_PAGES);
        return true;
    }
    if (cpuType == SUPER8) {
        _table = ARRAY_BEGIN(SUPER8_PAGES);
        _end = ARRAY_END(SUPER8_PAGES);
        return true;
    }
    return false;
}

const char *TableZ8::listCpu() const {
    return TEXT_CPU_LIST;
}

const char *TableZ8::getCpu() const {
    if (_cpuType == SUPER8) return TEXT_CPU_Z88;
    return _cpuType == Z8 ? TEXT_CPU_Z8 : TEXT_CPU_Z86C;
}

bool TableZ8::setCpu(const char *cpu) {
    if (strncasecmp_P(cpu, TEXT_CPU_Z86C, 4) == 0)
        return setCpu(Z86C);
    if (strncasecmp_P(cpu, TEXT_CPU_Z86, 3) == 0
        || strcasecmp_P(cpu, TEXT_CPU_Z8) == 0)
        return setCpu(Z8);
    if (strncasecmp_P(cpu, TEXT_CPU_Z88, 3) == 0
        || strcasecmp_P(cpu, TEXT_CPU_SUPER8) == 0)
        return setCpu(SUPER8);
    return false;
}

class TableZ8 TableZ8;

} // namespace z8
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

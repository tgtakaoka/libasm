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

#include "config_z80.h"
#include "entry_z80.h"
#include "table_z80.h"
#include "text_z80.h"

#include <ctype.h>
#include <string.h>

namespace libasm {
namespace z80 {

#define E(_opc, _name, _dst, _src)              \
    { _opc,                                     \
      Entry::_flags(_dst, _src),                \
      TEXT_##_name                              \
    },
#define IXB(_opc, _name, _dst, _src)            \
    { _opc,                                     \
      Entry::_flags(_dst, _src, true),          \
      TEXT_##_name                              \
    },

static constexpr Entry TABLE_I8080[] PROGMEM = {
    E(0x00, NOP,  M_NO,   M_NO)
    E(0x01, LD,   M_PTR,  M_IM16)
    E(0x09, ADD,  R_HL,   M_PTR)
    E(0x02, LD,   I_BCDE, R_A)
    E(0x0A, LD,   R_A,    I_BCDE)
    E(0x22, LD,   M_ABS,  R_HL)
    E(0x2A, LD,   R_HL,   M_ABS)
    E(0x32, LD,   M_ABS,  R_A)
    E(0x3A, LD,   R_A,    M_ABS)
    E(0x03, INC,  M_PTR,  M_NO)
    E(0x0B, DEC,  M_PTR,  M_NO)
    E(0x04, INC,  M_DST,  M_NO)
    E(0x05, DEC,  M_DST,  M_NO)
    E(0x06, LD,   M_DST,  M_IM8)
    E(0x07, RLCA, M_NO,   M_NO)
    E(0x0F, RRCA, M_NO,   M_NO)
    E(0x17, RLA,  M_NO,   M_NO)
    E(0x1F, RRA,  M_NO,   M_NO)
    E(0x27, DAA,  M_NO,   M_NO)
    E(0x2F, CPL,  M_NO,   M_NO)
    E(0x37, SCF,  M_NO,   M_NO)
    E(0x3F, CCF,  M_NO,   M_NO)
    E(0x76, HALT, M_NO,   M_NO)
    E(0x40, LD,   M_DST,  M_REG)
    E(0x80, ADD,  R_A,    M_REG)
    E(0x88, ADC,  R_A,    M_REG)
    E(0x90, SUB,  R_A,    M_REG)
    E(0x90, SUB,  M_REG,  M_NO)
    E(0x98, SBC,  R_A,    M_REG)
    E(0xA0, AND,  R_A,    M_REG)
    E(0xA0, AND,  M_REG,  M_NO)
    E(0xA8, XOR,  R_A,    M_REG)
    E(0xA8, XOR,  M_REG,  M_NO)
    E(0xB0, OR,   R_A,    M_REG)
    E(0xB0, OR,   M_REG,  M_NO)
    E(0xB8, CP,   R_A,    M_REG)
    E(0xB8, CP,   M_REG,  M_NO)
    E(0xC0, RET,  M_CC8,  M_NO)
    E(0xC1, POP,  M_STK,  M_NO)
    E(0xC9, RET,  M_NO,   M_NO)
    E(0xF9, LD,   R_SP,   R_HL)
    E(0xC2, JP,   M_CC8,  M_IM16)
    E(0xC3, JP,   M_IM16, M_NO)
    E(0xE9, JP,   I_HL,   M_NO)
    E(0xD3, OUT,  M_IOA,  R_A)
    E(0xDB, IN,   R_A,    M_IOA)
    E(0xE3, EX,   I_SP,   R_HL)
    E(0xE3, EX,   R_HL,   I_SP)
    E(0xEB, EX,   R_DE,   R_HL)
    E(0xEB, EX,   R_HL,   R_DE)
    E(0xF3, DI,   M_NO,   M_NO)
    E(0xFB, EI,   M_NO,   M_NO)
    E(0xC4, CALL, M_CC8,  M_IM16)
    E(0xC5, PUSH, M_STK,  M_NO)
    E(0xCD, CALL, M_IM16, M_NO)
    E(0xC6, ADD,  R_A,    M_IM8)
    E(0xCE, ADC,  R_A,    M_IM8)
    E(0xD6, SUB,  R_A,    M_IM8)
    E(0xD6, SUB,  M_IM8,  M_NO)
    E(0xDE, SBC,  R_A,    M_IM8)
    E(0xE6, AND,  R_A,    M_IM8)
    E(0xE6, AND,  M_IM8,  M_NO)
    E(0xEE, XOR,  R_A,    M_IM8)
    E(0xEE, XOR,  M_IM8,  M_NO)
    E(0xF6, OR,   R_A,    M_IM8)
    E(0xF6, OR,   M_IM8,  M_NO)
    E(0xFE, CP,   R_A,    M_IM8)
    E(0xFE, CP,   M_IM8,  M_NO)
    E(0xC7, RST,  M_VEC,  M_NO)
};
static constexpr Entry TABLE_I8085[] PROGMEM = {
    E(0x20, LD,   R_A,    R_IM)
    E(0x30, LD,   R_IM,   R_A)
};
static constexpr Entry TABLE_Z80[] PROGMEM = {
    E(0x08, EX,   R_AF,   R_AFP)
    E(0x10, DJNZ, M_REL,  M_NO)
    E(0x20, JR,   M_CC4,  M_REL)
    E(0x18, JR,   M_REL,  M_NO)
    E(0xD9, EXX,  M_NO,   M_NO)
};
static constexpr Config::opcode_t PREFIX_00 = 0x00;

static constexpr Entry TABLE_CB[] PROGMEM = {
    E(0x00, RLC,  M_REG,  M_NO)
    E(0x08, RRC,  M_REG,  M_NO)
    E(0x10, RL,   M_REG,  M_NO)
    E(0x18, RR,   M_REG,  M_NO)
    E(0x20, SLA,  M_REG,  M_NO)
    E(0x28, SRA,  M_REG,  M_NO)
    E(0x38, SRL,  M_REG,  M_NO)
    E(0x40, BIT,  M_BIT,  M_REG)
    E(0x80, RES,  M_BIT,  M_REG)
    E(0xC0, SET,  M_BIT,  M_REG)
    IXB(0x06, RLC,  M_INDX, M_NO)
    IXB(0x0E, RRC,  M_INDX, M_NO)
    IXB(0x16, RL,   M_INDX, M_NO)
    IXB(0x1E, RR,   M_INDX, M_NO)
    IXB(0x26, SLA,  M_INDX, M_NO)
    IXB(0x2E, SRA,  M_INDX, M_NO)
    IXB(0x3E, SRL,  M_INDX, M_NO)
    IXB(0x46, BIT,  M_BIT,  M_INDX)
    IXB(0x86, RES,  M_BIT,  M_INDX)
    IXB(0xC6, SET,  M_BIT,  M_INDX)
};
static constexpr Config::opcode_t PREFIX_CB = 0xCB;

static constexpr Entry TABLE_ED[] PROGMEM = {
    E(0x70, IN,   M_UNKI, M_UNKI) // IN (HL),(C)
    E(0x40, IN,   M_DST,  I_C)
    E(0x71, OUT,  M_UNKI, M_UNKI) // OUT (C),(H)
    E(0x41, OUT,  I_C,    M_DST)
    E(0x42, SBC,  R_HL,   M_PTR)
    E(0x4A, ADC,  R_HL,   M_PTR)
    E(0x63, LD,   M_UNKI, M_UNKI) // LD (ABS),HL
    E(0x43, LD,   M_ABS,  M_PTR)
    E(0x6B, LD,   M_UNKI, M_UNKI) // LD HL,(ABS)
    E(0x4B, LD,   M_PTR,  M_ABS)
    E(0x44, NEG,  M_NO,   M_NO)
    E(0x45, RETN, M_NO,   M_NO)
    E(0x4D, RETI, M_NO,   M_NO)
    E(0x46, IM,   M_IMMD, M_NO)
    E(0x47, LD,   R_IR,   R_A)
    E(0x57, LD,   R_A,    R_IR)
    E(0x67, RRD,  M_NO,   M_NO)
    E(0x6F, RLD,  M_NO,   M_NO)
    E(0xA0, LDI,  M_NO,   M_NO)
    E(0xA8, LDD,  M_NO,   M_NO)
    E(0xB0, LDIR, M_NO,   M_NO)
    E(0xB8, LDDR, M_NO,   M_NO)
    E(0xA1, CPI,  M_NO,   M_NO)
    E(0xA9, CPD,  M_NO,   M_NO)
    E(0xB1, CPIR, M_NO,   M_NO)
    E(0xB9, CPDR, M_NO,   M_NO)
    E(0xA2, INI,  M_NO,   M_NO)
    E(0xAA, IND,  M_NO,   M_NO)
    E(0xB2, INIR, M_NO,   M_NO)
    E(0xBA, INDR, M_NO,   M_NO)
    E(0xA3, OUTI, M_NO,   M_NO)
    E(0xAB, OUTD, M_NO,   M_NO)
    E(0xB3, OTIR, M_NO,   M_NO)
    E(0xBB, OTDR, M_NO,   M_NO)
};
static constexpr Config::opcode_t PREFIX_ED = 0xED;

static constexpr Entry TABLE_IX[] PROGMEM = {
    E(0x09, ADD,  R_IXIY, M_PIX)
    E(0x21, LD,   R_IXIY, M_IM16)
    E(0x22, LD,   M_ABS,  R_IXIY)
    E(0x2A, LD,   R_IXIY, M_ABS)
    E(0x23, INC,  R_IXIY, M_NO)
    E(0x2B, DEC,  R_IXIY, M_NO)
    E(0x34, INC,  M_INDX, M_NO)
    E(0x35, DEC,  M_INDX, M_NO)
    E(0x36, LD,   M_INDX, M_IM8)
    E(0x76, LD,   M_UNKI, M_UNKI) // LD (Ix+nn),(HL)
    E(0x46, LD,   M_DST,  M_INDX)
    E(0x70, LD,   M_INDX, M_REG)
    E(0x86, ADD,  R_A,    M_INDX)
    E(0x8E, ADC,  R_A,    M_INDX)
    E(0x96, SUB,  R_A,    M_INDX)
    E(0x96, SUB,  M_INDX, M_NO)
    E(0x9E, SBC,  R_A,    M_INDX)
    E(0xA6, AND,  R_A,    M_INDX)
    E(0xA6, AND,  M_INDX, M_NO)
    E(0xAE, XOR,  R_A,    M_INDX)
    E(0xAE, XOR,  M_INDX, M_NO)
    E(0xB6, OR,   R_A,    M_INDX)
    E(0xB6, OR,   M_INDX, M_NO)
    E(0xBE, CP,   R_A,    M_INDX)
    E(0xBE, CP,   M_INDX, M_NO)
    E(0xCB, BIT,  T_IXB,  T_IXB)  // to TABLE_CB[]
    E(0xE1, POP,  R_IXIY, M_NO)
    E(0xE9, JP,   I_IXIY, M_NO)
    E(0xF9, LD,   R_SP,   R_IXIY)
    E(0xE3, EX,   I_SP,   R_IXIY)
    E(0xE5, PUSH, R_IXIY, M_NO)
};

struct TableZ80::EntryPage {
    const Config::opcode_t prefix;
    const Entry *const table;
    const Entry *const end;
};

static constexpr TableZ80::EntryPage PAGES_I8080[] PROGMEM = {
    { PREFIX_00, ARRAY_RANGE(TABLE_I8080) },
};
static constexpr TableZ80::EntryPage PAGES_I8085[] PROGMEM = {
    { PREFIX_00, ARRAY_RANGE(TABLE_I8080) },
    { PREFIX_00, ARRAY_RANGE(TABLE_I8085) },
};
static constexpr TableZ80:: EntryPage PAGES_Z80[] PROGMEM = {
    { PREFIX_00, ARRAY_RANGE(TABLE_Z80) },
    { PREFIX_00, ARRAY_RANGE(TABLE_I8080) },
    { PREFIX_CB, ARRAY_RANGE(TABLE_CB) },
    { PREFIX_ED, ARRAY_RANGE(TABLE_ED) },
    { TableZ80::PREFIX_IX, ARRAY_RANGE(TABLE_IX) },
    { TableZ80::PREFIX_IY, ARRAY_RANGE(TABLE_IX) },
};

static bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table) return true;
    if (opr == M_REG)
        return table == M_DST;
    if (opr == R_A || opr == I_HL)
        return table == M_REG || table == M_DST;
    if (opr == R_C)
        return table == M_REG || table == M_DST
            || table == M_CC4 || table == M_CC8;
    if (opr == R_BC || opr == R_DE)
        return table == M_PTR || table == M_PIX || table == M_STK;
    if (opr == R_HL)
        return table == M_PTR || table == M_STK;
    if (opr == R_SP)
        return table == M_PTR || table == M_PIX;
    if (opr == R_AF)
        return table == M_STK;
    if (opr == R_IXIY)
        return table == M_PIX;
    if (opr == M_IM16)
        return table == M_IM8 || table == M_REL || table == M_BIT
            || table == M_VEC || table == M_IMMD;
    if (opr == M_ABS)
        return table == M_IOA;
    if (opr == M_CC4)
        return table == M_CC8;
    return false;
}

static bool acceptModes(uint16_t flags, const Entry *entry) {
    const uint16_t table = pgm_read_word(&entry->flags);
    return acceptMode(Entry::_dstMode(flags), Entry::_dstMode(table))
        && acceptMode(Entry::_srcMode(flags), Entry::_srcMode(table));
}

Error TableZ80::searchName(
    InsnZ80 &insn,  const EntryPage *pages, const EntryPage *end) const {
    const char *name = insn.name();
    const uint16_t flags = Entry::_flags(insn.dstMode(), insn.srcMode());
    uint8_t count = 0;
    for (const EntryPage *page = pages; page < end; page++) {
        const Entry *table =
            reinterpret_cast<Entry *>(pgm_read_ptr(&page->table));
        const Entry *end = reinterpret_cast<Entry *>(pgm_read_ptr(&page->end));
        const Entry *entry = TableBase::searchName<Entry,uint16_t>(
            name, flags, table, end, acceptModes, count);
        if (entry) {
            const Config::opcode_t prefix = pgm_read_byte(&page->prefix);
            insn.setOpCode(pgm_read_byte(&entry->opCode), prefix);
            insn.setFlags(pgm_read_word(&entry->flags));
            return OK;
        }
    }
    return count == 0 ? UNKNOWN_INSTRUCTION : OPERAND_NOT_ALLOWED;
}

static Config::opcode_t maskCode(Config::opcode_t opCode, const Entry *entry) {
    const uint16_t flags = pgm_read_word(&entry->flags);
    const AddrMode dst = Entry::_dstMode(flags);
    const AddrMode src = Entry::_srcMode(flags);
    Config::opcode_t mask = 0;
    if (dst == M_REG || src == M_REG)
        mask |= 7;
    if (dst == M_CC8 || src == M_DST || dst == M_DST || dst == M_VEC || dst == M_BIT)
        mask |= (7 << 3);
    if (dst == M_PTR || src == M_PTR || dst == M_PIX || src == M_PIX
        || dst == M_STK || src == M_STK)
        mask |= (3 << 4);
    if (dst == I_BCDE || src == I_BCDE)
        mask |= (1 << 4);
    if (dst == M_CC4 || dst == M_IMMD)
        mask |= (3 << 3);
    if (dst == R_IR || src == R_IR)
        mask |= (1 << 3);
    return opCode & ~mask;
}

Error TableZ80::searchOpCode(
    InsnZ80 &insn, const EntryPage *pages, const EntryPage *end) const {
    for (const EntryPage *page = pages; page < end; page++) {
        Config::opcode_t prefix = pgm_read_byte(&page->prefix);
        if (insn.prefix() != prefix) continue;
        const Entry *table =
            reinterpret_cast<Entry *>(pgm_read_ptr(&page->table));
        const Entry *end = reinterpret_cast<Entry *>(pgm_read_ptr(&page->end));
        const Entry *entry = TableBase::searchCode<Entry,Config::opcode_t>(
            insn.opCode(), table, end, maskCode);
        if (entry) {
            insn.setFlags(pgm_read_word(&entry->flags));
            const /*PROGMEM*/ char *name =
                reinterpret_cast<const char *>(pgm_read_ptr(&entry->name));
            insn.setName_P(name);
            return OK;
        }
    }
    return UNKNOWN_INSTRUCTION;
}

bool TableZ80::isPrefix(Config::opcode_t opCode) const {
    if (_cpuType != Z80) return false;
    return opCode == PREFIX_CB || opCode == PREFIX_ED
        || opCode == TableZ80::PREFIX_IX || opCode == TableZ80::PREFIX_IY;
}

Error TableZ80::searchName(InsnZ80 &insn) const {
    return _error.setError(searchName(insn, _table, _end));
}

Error TableZ80::searchOpCode(InsnZ80 &insn) const {
    return _error.setError(searchOpCode(insn, _table, _end));
}

TableZ80::TableZ80() {
    setCpu(Z80);
}

bool TableZ80::setCpu(CpuType cpuType) {
    _cpuType = cpuType;
    if (cpuType == Z80) {
        _table = ARRAY_BEGIN(PAGES_Z80);
        _end = ARRAY_END(PAGES_Z80);
        return true;
    }
    if (cpuType == I8080) {
        _table = ARRAY_BEGIN(PAGES_I8080);
        _end = ARRAY_END(PAGES_I8080);
        return true;
    }
    if (cpuType == I8085) {
        _table = ARRAY_BEGIN(PAGES_I8085);
        _end = ARRAY_END(PAGES_I8085);
        return true;
    }
    return false;
}

const char *TableZ80::listCpu() const {
    return TEXT_CPU_Z80;
}

const char *TableZ80::getCpu() const {
    if (_cpuType == Z80) return TEXT_CPU_Z80;
    return _cpuType == I8080 ? TEXT_CPU_8080 : TEXT_CPU_8085;
}

bool TableZ80::setCpu(const char *cpu) {
    if (strcasecmp_P(cpu, TEXT_CPU_Z80) == 0)
        return setCpu(Z80);
    if (toupper(*cpu) == 'I') cpu++;
    if (strcmp_P(cpu, TEXT_CPU_8080) == 0)
        return setCpu(I8080);
    if (strcmp_P(cpu, TEXT_CPU_8085) == 0)
        return setCpu(I8085);
    return false;
}

class TableZ80 TableZ80;

} // namespace z80
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

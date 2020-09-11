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

#define E(_opc, _name, _iformat, _dstOpr, _srcOpr, _amode)   \
    { _opc,                                                     \
      Entry::_flags(_iformat, _amode, _dstOpr, _srcOpr),     \
      TEXT_##_name                                              \
    },

static constexpr Entry TABLE_I8080[] PROGMEM = {
    E(0x00, NOP,  NO_FMT,  NO_OPR, NO_OPR, INHR)
    E(0x01, LD,   PTR_FMT, REG_16, IMM_16, IMM16)
    E(0x09, ADD,  PTR_FMT, HL_REG, REG_16, INHR)
    E(0x02, LD,   IDX_FMT, BC_PTR, A_REG,  INHR)
    E(0x0A, LD,   IDX_FMT, A_REG,  BC_PTR, INHR)
    E(0x22, LD,   NO_FMT,  ADDR_16,HL_REG, DIRECT)
    E(0x2A, LD,   NO_FMT,  HL_REG, ADDR_16,DIRECT)
    E(0x32, LD,   NO_FMT,  ADDR_16,A_REG,  DIRECT)
    E(0x3A, LD,   NO_FMT,  A_REG,  ADDR_16,DIRECT)
    E(0x03, INC,  PTR_FMT, REG_16, NO_OPR, INHR)
    E(0x0B, DEC,  PTR_FMT, REG_16, NO_OPR, INHR)
    E(0x04, INC,  DST_FMT, REG_8,  NO_OPR, INHR)
    E(0x05, DEC,  DST_FMT, REG_8,  NO_OPR, INHR)
    E(0x06, LD,   DST_FMT, REG_8,  IMM_8,  IMM8)
    E(0x07, RLCA, NO_FMT,  NO_OPR, NO_OPR, INHR)
    E(0x0F, RRCA, NO_FMT,  NO_OPR, NO_OPR, INHR)
    E(0x17, RLA,  NO_FMT,  NO_OPR, NO_OPR, INHR)
    E(0x1F, RRA,  NO_FMT,  NO_OPR, NO_OPR, INHR)
    E(0x27, DAA,  NO_FMT,  NO_OPR, NO_OPR, INHR)
    E(0x2F, CPL,  NO_FMT,  NO_OPR, NO_OPR, INHR)
    E(0x37, SCF,  NO_FMT,  NO_OPR, NO_OPR, INHR)
    E(0x3F, CCF,  NO_FMT,  NO_OPR, NO_OPR, INHR)
    E(0x76, HALT, NO_FMT,  NO_OPR, NO_OPR, INHR)
    E(0x40, LD,   DST_SRC_FMT, REG_8,REG_8,INHR)
    E(0x80, ADD,  SRC_FMT, A_REG,  REG_8,  INHR)
    E(0x88, ADC,  SRC_FMT, A_REG,  REG_8,  INHR)
    E(0x90, SUB,  SRC_FMT, A_REG,  REG_8,  INHR)
    E(0x90, SUB,  SRC_FMT, REG_8,  NO_OPR, INHR)
    E(0x98, SBC,  SRC_FMT, A_REG,  REG_8,  INHR)
    E(0xA0, AND,  SRC_FMT, A_REG,  REG_8,  INHR)
    E(0xA0, AND,  SRC_FMT, REG_8,  NO_OPR, INHR)
    E(0xA8, XOR,  SRC_FMT, A_REG,  REG_8,  INHR)
    E(0xA8, XOR,  SRC_FMT, REG_8,  NO_OPR, INHR)
    E(0xB0, OR,   SRC_FMT, A_REG,  REG_8,  INHR)
    E(0xB0, OR,   SRC_FMT, REG_8,  NO_OPR, INHR)
    E(0xB8, CP,   SRC_FMT, A_REG,  REG_8,  INHR)
    E(0xB8, CP,   SRC_FMT, REG_8,  NO_OPR, INHR)
    E(0xC0, RET,  DST_FMT, COND_8, NO_OPR, INHR)
    E(0xC1, POP,  PTR_FMT, STK_16, NO_OPR, INHR)
    E(0xC9, RET,  NO_FMT,  NO_OPR, NO_OPR, INHR)
    E(0xF9, LD,   NO_FMT,  SP_REG, HL_REG, INHR)
    E(0xC2, JP,   DST_FMT, COND_8, IMM_16, DIRECT)
    E(0xC3, JP,   NO_FMT,  IMM_16, NO_OPR, DIRECT)
    E(0xE9, JP,   NO_FMT,  HL_PTR, NO_OPR, INHR)
    E(0xD3, OUT,  NO_FMT,  ADDR_8, A_REG,  IOADR)
    E(0xDB, IN,   NO_FMT,  A_REG,  ADDR_8, IOADR)
    E(0xE3, EX,   NO_FMT,  SP_PTR, HL_REG, INHR)
    E(0xEB, EX,   NO_FMT,  DE_REG, HL_REG, INHR)
    E(0xF3, DI,   NO_FMT,  NO_OPR, NO_OPR, INHR)
    E(0xFB, EI,   NO_FMT,  NO_OPR, NO_OPR, INHR)
    E(0xC4, CALL, DST_FMT, COND_8, IMM_16, DIRECT)
    E(0xC5, PUSH, PTR_FMT, STK_16, NO_OPR, INHR)
    E(0xCD, CALL, NO_FMT,  IMM_16, NO_OPR, DIRECT)
    E(0xC6, ADD,  NO_FMT,  A_REG,  IMM_8,  IMM8)
    E(0xCE, ADC,  NO_FMT,  A_REG,  IMM_8,  IMM8)
    E(0xD6, SUB,  NO_FMT,  A_REG,  IMM_8,  IMM8)
    E(0xD6, SUB,  NO_FMT,  IMM_8,  NO_OPR, IMM8)
    E(0xDE, SBC,  NO_FMT,  A_REG,  IMM_8,  IMM8)
    E(0xE6, AND,  NO_FMT,  A_REG,  IMM_8,  IMM8)
    E(0xE6, AND,  NO_FMT,  IMM_8,  NO_OPR, IMM8)
    E(0xEE, XOR,  NO_FMT,  A_REG,  IMM_8,  IMM8)
    E(0xEE, XOR,  NO_FMT,  IMM_8,  NO_OPR, IMM8)
    E(0xF6, OR,   NO_FMT,  A_REG,  IMM_8,  IMM8)
    E(0xF6, OR,   NO_FMT,  IMM_8,  NO_OPR, IMM8)
    E(0xFE, CP,   NO_FMT,  A_REG,  IMM_8,  IMM8)
    E(0xFE, CP,   NO_FMT,  IMM_8,  NO_OPR, IMM8)
    E(0xC7, RST,  DST_FMT, VEC_NO, NO_OPR, INHR)
};
static constexpr Entry TABLE_I8085[] PROGMEM = {
    E(0x20, LD,   NO_FMT,  A_REG,  IM_REG, INHR)
    E(0x30, LD,   NO_FMT,  IM_REG, A_REG,  INHR)
};
static constexpr Entry TABLE_Z80[] PROGMEM = {
    E(0x08, EX,   NO_FMT,  AF_REG, AFPREG, INHR)
    E(0x10, DJNZ, NO_FMT,  IMM_16, NO_OPR, REL8)
    E(0x20, JR,   CC4_FMT, COND_4, IMM_16, REL8)
    E(0x18, JR,   NO_FMT,  IMM_16, NO_OPR, REL8)
    E(0xD9, EXX,  NO_FMT,  NO_OPR, NO_OPR, INHR)
};
static constexpr Config::opcode_t PREFIX_00 = 0x00;

static constexpr Entry TABLE_CB[] PROGMEM = {
    E(0x00, RLC,  SRC_FMT, REG_8,  NO_OPR, INHR)
    E(0x08, RRC,  SRC_FMT, REG_8,  NO_OPR, INHR)
    E(0x10, RL,   SRC_FMT, REG_8,  NO_OPR, INHR)
    E(0x18, RR,   SRC_FMT, REG_8,  NO_OPR, INHR)
    E(0x20, SLA,  SRC_FMT, REG_8,  NO_OPR, INHR)
    E(0x28, SRA,  SRC_FMT, REG_8,  NO_OPR, INHR)
    E(0x38, SRL,  SRC_FMT, REG_8,  NO_OPR, INHR)
    E(0x40, BIT,  DST_SRC_FMT,BIT_NO,REG_8,INHR)
    E(0x80, RES,  DST_SRC_FMT,BIT_NO,REG_8,INHR)
    E(0xC0, SET,  DST_SRC_FMT,BIT_NO,REG_8,INHR)
    E(0x06, RLC,  NO_FMT,  IX_OFF, NO_OPR, INDX_IMM8)
    E(0x0E, RRC,  NO_FMT,  IX_OFF, NO_OPR, INDX_IMM8)
    E(0x16, RL,   NO_FMT,  IX_OFF, NO_OPR, INDX_IMM8)
    E(0x1E, RR,   NO_FMT,  IX_OFF, NO_OPR, INDX_IMM8)
    E(0x26, SLA,  NO_FMT,  IX_OFF, NO_OPR, INDX_IMM8)
    E(0x2E, SRA,  NO_FMT,  IX_OFF, NO_OPR, INDX_IMM8)
    E(0x3E, SRL,  NO_FMT,  IX_OFF, NO_OPR, INDX_IMM8)
    E(0x46, BIT,  DST_FMT, BIT_NO, IX_OFF, INDX_IMM8)
    E(0x86, RES,  DST_FMT, BIT_NO, IX_OFF, INDX_IMM8)
    E(0xC6, SET,  DST_FMT, BIT_NO, IX_OFF, INDX_IMM8)
};
static constexpr Config::opcode_t PREFIX_CB = 0xCB;

static constexpr Entry TABLE_ED[] PROGMEM = {
    E(0x40, IN,   DST_FMT, REG_8,  C_PTR,  INHR)
    E(0x41, OUT,  DST_FMT, C_PTR,  REG_8,  INHR)
    E(0x42, SBC,  PTR_FMT, HL_REG, REG_16, INHR)
    E(0x4A, ADC,  PTR_FMT, HL_REG, REG_16, INHR)
    E(0x43, LD,   PTR_FMT, ADDR_16,REG_16, DIRECT)
    E(0x4B, LD,   PTR_FMT, REG_16, ADDR_16,DIRECT)
    E(0x44, NEG,  NO_FMT,  NO_OPR, NO_OPR, INHR)
    E(0x45, RETN, NO_FMT,  NO_OPR, NO_OPR, INHR)
    E(0x4D, RETI, NO_FMT,  NO_OPR, NO_OPR, INHR)
    E(0x46, IM,   DST_FMT, IMM_NO, NO_OPR, INHR)
    E(0x47, LD,   IR_FMT,  IR_REG, A_REG,  INHR)
    E(0x57, LD,   IR_FMT,  A_REG,  IR_REG, INHR)
    E(0x67, RRD,  NO_FMT,  NO_OPR, NO_OPR, INHR)
    E(0x6F, RLD,  NO_FMT,  NO_OPR, NO_OPR, INHR)
    E(0xA0, LDI,  NO_FMT,  NO_OPR, NO_OPR, INHR)
    E(0xA8, LDD,  NO_FMT,  NO_OPR, NO_OPR, INHR)
    E(0xB0, LDIR, NO_FMT,  NO_OPR, NO_OPR, INHR)
    E(0xB8, LDDR, NO_FMT,  NO_OPR, NO_OPR, INHR)
    E(0xA1, CPI,  NO_FMT,  NO_OPR, NO_OPR, INHR)
    E(0xA9, CPD,  NO_FMT,  NO_OPR, NO_OPR, INHR)
    E(0xB1, CPIR, NO_FMT,  NO_OPR, NO_OPR, INHR)
    E(0xB9, CPDR, NO_FMT,  NO_OPR, NO_OPR, INHR)
    E(0xA2, INI,  NO_FMT,  NO_OPR, NO_OPR, INHR)
    E(0xAA, IND,  NO_FMT,  NO_OPR, NO_OPR, INHR)
    E(0xB2, INIR, NO_FMT,  NO_OPR, NO_OPR, INHR)
    E(0xBA, INDR, NO_FMT,  NO_OPR, NO_OPR, INHR)
    E(0xA3, OUTI, NO_FMT,  NO_OPR, NO_OPR, INHR)
    E(0xAB, OUTD, NO_FMT,  NO_OPR, NO_OPR, INHR)
    E(0xB3, OTIR, NO_FMT,  NO_OPR, NO_OPR, INHR)
    E(0xBB, OTDR, NO_FMT,  NO_OPR, NO_OPR, INHR)
};
static constexpr Config::opcode_t PREFIX_ED = 0xED;

static constexpr Entry TABLE_IX[] PROGMEM = {
    E(0x09, ADD,  PTR_FMT, IX_REG, REG_16X,INHR)
    E(0x21, LD,   NO_FMT,  IX_REG, IMM_16, IMM16)
    E(0x22, LD,   NO_FMT,  ADDR_16,IX_REG, DIRECT)
    E(0x2A, LD,   NO_FMT,  IX_REG, ADDR_16,DIRECT)
    E(0x23, INC,  NO_FMT,  IX_REG, NO_OPR, INHR)
    E(0x2B, DEC,  NO_FMT,  IX_REG, NO_OPR, INHR)
    E(0x34, INC,  NO_FMT,  IX_OFF, NO_OPR, INDX)
    E(0x35, DEC,  NO_FMT,  IX_OFF, NO_OPR, INDX)
    E(0x36, LD,   NO_FMT,  IX_OFF, IMM_8,  INDX_IMM8)
    E(0x46, LD,   DST_FMT, REG_8,  IX_OFF, INDX)
    E(0x70, LD,   SRC_FMT, IX_OFF, REG_8,  INDX)
    E(0x86, ADD,  NO_FMT,  A_REG,  IX_OFF, INDX)
    E(0x8E, ADC,  NO_FMT,  A_REG,  IX_OFF, INDX)
    E(0x96, SUB,  NO_FMT,  A_REG,  IX_OFF, INDX)
    E(0x96, SUB,  NO_FMT,  IX_OFF, NO_OPR, INDX)
    E(0x9E, SBC,  NO_FMT,  A_REG,  IX_OFF, INDX)
    E(0xA6, AND,  NO_FMT,  A_REG,  IX_OFF, INDX)
    E(0xA6, AND,  NO_FMT,  IX_OFF, NO_OPR, INDX)
    E(0xAE, XOR,  NO_FMT,  A_REG,  IX_OFF, INDX)
    E(0xAE, XOR,  NO_FMT,  IX_OFF, NO_OPR, INDX)
    E(0xB6, OR,   NO_FMT,  A_REG,  IX_OFF, INDX)
    E(0xB6, OR,   NO_FMT,  IX_OFF, NO_OPR, INDX)
    E(0xBE, CP,   NO_FMT,  A_REG,  IX_OFF, INDX)
    E(0xBE, CP,   NO_FMT,  IX_OFF, NO_OPR, INDX)
    E(0xCB, BIT,  NO_FMT,  IX_BIT, IX_BIT, INDX_IMM8) // to TABLE_CB[]
    E(0xE1, POP,  NO_FMT,  IX_REG, NO_OPR, INHR)
    E(0xE9, JP,   NO_FMT,  IX_PTR, NO_OPR, INHR)
    E(0xF9, LD,   NO_FMT,  SP_REG, IX_REG, INHR)
    E(0xE3, EX,   NO_FMT,  SP_PTR, IX_REG, INHR)
    E(0xE5, PUSH, NO_FMT,  IX_REG, NO_OPR, INHR)
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

static bool acceptOprFormat(OprFormat opr, OprFormat table) {
    if (opr == table) return true;
    if (opr == A_REG) return table == REG_8;
    if (opr == C_REG) return table == REG_8
                          || table == COND_4 || table == COND_8;
    if (opr == HL_PTR) return table == REG_8;
    if (opr == BC_REG || opr == DE_REG)
        return table == REG_16 || table == REG_16X || table == STK_16;
    if (opr == HL_REG) return table == REG_16 || table == STK_16;
    if (opr == SP_REG) return table == REG_16 || table == REG_16X;
    if (opr == AF_REG) return table == STK_16;
    if (opr == IX_REG) return table == REG_16X;
    if (opr == IMM_16)
        return table == IMM_8 || table == BIT_NO || table == VEC_NO
            || table == IMM_NO;
    if (opr == ADDR_16) return table == ADDR_8;
    if (opr == COND_4) return table == COND_8;
    return false;
}

static bool acceptOprFormats(uint16_t flags, const Entry *entry) {
    const uint16_t table = pgm_read_word(&entry->flags);
    return acceptOprFormat(Entry::_dstFormat(flags), Entry::_dstFormat(table))
        && acceptOprFormat(Entry::_srcFormat(flags), Entry::_srcFormat(table));
}

Error TableZ80::searchName(
    InsnZ80 &insn,  const EntryPage *pages, const EntryPage *end) {
    const char *name = insn.name();
    const uint16_t flags =
        Entry::_flags(NO_FMT, INHR, insn.dstFormat(), insn.srcFormat());
    uint8_t count = 0;
    for (const EntryPage *page = pages; page < end; page++) {
        const Entry *table = reinterpret_cast<Entry *>(pgm_read_ptr(&page->table));
        const Entry *end = reinterpret_cast<Entry *>(pgm_read_ptr(&page->end));
        const Entry *entry = TableBase::searchName<Entry,uint16_t>(
            name, flags, table, end, acceptOprFormats, count);
        if (entry) {
            const Config::opcode_t prefix = pgm_read_byte(&page->prefix);
            insn.setInsnCode(prefix, pgm_read_byte(&entry->opCode));
            insn.setFlags(pgm_read_word(&entry->flags));
            return OK;
        }
    }
    return count == 0 ? UNKNOWN_INSTRUCTION : OPERAND_NOT_ALLOWED;
}

static Config::opcode_t maskCode(
    Config::opcode_t opCode, const Entry *entry) {
    const InsnFormat iformat = Entry::_insnFormat(pgm_read_word(&entry->flags));
    switch (iformat) {
    case PTR_FMT: return opCode & ~0x30;
    case CC4_FMT: return opCode & ~0x18;
    case IDX_FMT: return opCode & ~0x10;
    case IR_FMT:  return opCode & ~0x08;
    case DST_FMT: return opCode & ~0x38;
    case SRC_FMT: return opCode & ~0x07;
    case DST_SRC_FMT: return opCode & ~0x3F;
    default: return opCode;
    }
}

Error TableZ80::searchOpCode(
    InsnZ80 &insn, const EntryPage *pages, const EntryPage *end) {
    for (const EntryPage *page = pages; page < end; page++) {
        Config::opcode_t prefix = pgm_read_byte(&page->prefix);
        if (insn.prefixCode() != prefix) continue;
        const Entry *table = reinterpret_cast<Entry *>(pgm_read_ptr(&page->table));
        const Entry *end = reinterpret_cast<Entry *>(pgm_read_ptr(&page->end));
        const Entry *entry = TableBase::searchCode<Entry,Config::opcode_t>(
            insn.opCode(), table, end, maskCode);
        if (entry) {
            insn.setFlags(pgm_read_word(&entry->flags));
            const char *name =
                reinterpret_cast<const char *>(pgm_read_ptr(&entry->name));
            TableBase::setName(insn.insn(), name, Config::NAME_MAX);
            return OK;
        }
    }
    return UNKNOWN_INSTRUCTION;
}

bool TableZ80::isPrefixCode(Config::opcode_t opCode) {
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

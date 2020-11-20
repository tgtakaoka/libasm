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

#include "table_z80.h"
#include "config_z80.h"
#include "entry_z80.h"
#include "text_z80.h"

#include <ctype.h>
#include <string.h>

namespace libasm {
namespace z80 {

#define X(_opc, _name, _dst, _src, _ixb) \
    { _opc, Entry::Flags::create(_dst, _src, _ixb), _name }
#define E(_opc, _name, _dst, _src) X(_opc, _name, _dst, _src, false)
#define IXB(_opc, _name, _dst, _src) X(_opc, _name, _dst, _src, true)

// clang-format off
static constexpr Entry TABLE_I8080[] PROGMEM = {
    E(0x00, TEXT_NOP,  M_NO,   M_NO),
    E(0x01, TEXT_LD,   M_PTR,  M_IM16),
    E(0x09, TEXT_ADD,  R_HL,   M_PTR),
    E(0x02, TEXT_LD,   I_BCDE, R_A),
    E(0x0A, TEXT_LD,   R_A,    I_BCDE),
    E(0x22, TEXT_LD,   M_ABS,  R_HL),
    E(0x2A, TEXT_LD,   R_HL,   M_ABS),
    E(0x32, TEXT_LD,   M_ABS,  R_A),
    E(0x3A, TEXT_LD,   R_A,    M_ABS),
    E(0x03, TEXT_INC,  M_PTR,  M_NO),
    E(0x0B, TEXT_DEC,  M_PTR,  M_NO),
    E(0x04, TEXT_INC,  M_DST,  M_NO),
    E(0x05, TEXT_DEC,  M_DST,  M_NO),
    E(0x06, TEXT_LD,   M_DST,  M_IM8),
    E(0x07, TEXT_RLCA, M_NO,   M_NO),
    E(0x0F, TEXT_RRCA, M_NO,   M_NO),
    E(0x17, TEXT_RLA,  M_NO,   M_NO),
    E(0x1F, TEXT_RRA,  M_NO,   M_NO),
    E(0x27, TEXT_DAA,  M_NO,   M_NO),
    E(0x2F, TEXT_CPL,  M_NO,   M_NO),
    E(0x37, TEXT_SCF,  M_NO,   M_NO),
    E(0x3F, TEXT_CCF,  M_NO,   M_NO),
    E(0x76, TEXT_HALT, M_NO,   M_NO),
    E(0x40, TEXT_LD,   M_DST,  M_REG),
    E(0x80, TEXT_ADD,  R_A,    M_REG),
    E(0x88, TEXT_ADC,  R_A,    M_REG),
    E(0x90, TEXT_SUB,  R_A,    M_REG),
    E(0x90, TEXT_SUB,  M_REG,  M_NO),
    E(0x98, TEXT_SBC,  R_A,    M_REG),
    E(0xA0, TEXT_AND,  R_A,    M_REG),
    E(0xA0, TEXT_AND,  M_REG,  M_NO),
    E(0xA8, TEXT_XOR,  R_A,    M_REG),
    E(0xA8, TEXT_XOR,  M_REG,  M_NO),
    E(0xB0, TEXT_OR,   R_A,    M_REG),
    E(0xB0, TEXT_OR,   M_REG,  M_NO),
    E(0xB8, TEXT_CP,   R_A,    M_REG),
    E(0xB8, TEXT_CP,   M_REG,  M_NO),
    E(0xC0, TEXT_RET,  M_CC8,  M_NO),
    E(0xC1, TEXT_POP,  M_STK,  M_NO),
    E(0xC9, TEXT_RET,  M_NO,   M_NO),
    E(0xF9, TEXT_LD,   R_SP,   R_HL),
    E(0xC2, TEXT_JP,   M_CC8,  M_IM16),
    E(0xC3, TEXT_JP,   M_IM16, M_NO),
    E(0xE9, TEXT_JP,   I_HL,   M_NO),
    E(0xD3, TEXT_OUT,  M_IOA,  R_A),
    E(0xDB, TEXT_IN,   R_A,    M_IOA),
    E(0xE3, TEXT_EX,   I_SP,   R_HL),
    E(0xE3, TEXT_EX,   R_HL,   I_SP),
    E(0xEB, TEXT_EX,   R_DE,   R_HL),
    E(0xEB, TEXT_EX,   R_HL,   R_DE),
    E(0xF3, TEXT_DI,   M_NO,   M_NO),
    E(0xFB, TEXT_EI,   M_NO,   M_NO),
    E(0xC4, TEXT_CALL, M_CC8,  M_IM16),
    E(0xC5, TEXT_PUSH, M_STK,  M_NO),
    E(0xCD, TEXT_CALL, M_IM16, M_NO),
    E(0xC6, TEXT_ADD,  R_A,    M_IM8),
    E(0xCE, TEXT_ADC,  R_A,    M_IM8),
    E(0xD6, TEXT_SUB,  R_A,    M_IM8),
    E(0xD6, TEXT_SUB,  M_IM8,  M_NO),
    E(0xDE, TEXT_SBC,  R_A,    M_IM8),
    E(0xE6, TEXT_AND,  R_A,    M_IM8),
    E(0xE6, TEXT_AND,  M_IM8,  M_NO),
    E(0xEE, TEXT_XOR,  R_A,    M_IM8),
    E(0xEE, TEXT_XOR,  M_IM8,  M_NO),
    E(0xF6, TEXT_OR,   R_A,    M_IM8),
    E(0xF6, TEXT_OR,   M_IM8,  M_NO),
    E(0xFE, TEXT_CP,   R_A,    M_IM8),
    E(0xFE, TEXT_CP,   M_IM8,  M_NO),
    E(0xC7, TEXT_RST,  M_VEC,  M_NO),
};
static constexpr Entry TABLE_I8085[] PROGMEM = {
    E(0x20, TEXT_LD,   R_A,    R_IM),
    E(0x30, TEXT_LD,   R_IM,   R_A),
};
static constexpr Entry TABLE_Z80[] PROGMEM = {
    E(0x08, TEXT_EX,   R_AF,   R_AFP),
    E(0x10, TEXT_DJNZ, M_REL,  M_NO),
    E(0x20, TEXT_JR,   M_CC4,  M_REL),
    E(0x18, TEXT_JR,   M_REL,  M_NO),
    E(0xD9, TEXT_EXX,  M_NO,   M_NO),
};
static constexpr Config::opcode_t PREFIX_00 = 0x00;

static constexpr Entry TABLE_CB[] PROGMEM = {
    E(0x00, TEXT_RLC,  M_REG,  M_NO),
    E(0x08, TEXT_RRC,  M_REG,  M_NO),
    E(0x10, TEXT_RL,   M_REG,  M_NO),
    E(0x18, TEXT_RR,   M_REG,  M_NO),
    E(0x20, TEXT_SLA,  M_REG,  M_NO),
    E(0x28, TEXT_SRA,  M_REG,  M_NO),
    E(0x38, TEXT_SRL,  M_REG,  M_NO),
    E(0x40, TEXT_BIT,  M_BIT,  M_REG),
    E(0x80, TEXT_RES,  M_BIT,  M_REG),
    E(0xC0, TEXT_SET,  M_BIT,  M_REG),
    IXB(0x06, TEXT_RLC,  M_INDX, M_NO),
    IXB(0x0E, TEXT_RRC,  M_INDX, M_NO),
    IXB(0x16, TEXT_RL,   M_INDX, M_NO),
    IXB(0x1E, TEXT_RR,   M_INDX, M_NO),
    IXB(0x26, TEXT_SLA,  M_INDX, M_NO),
    IXB(0x2E, TEXT_SRA,  M_INDX, M_NO),
    IXB(0x3E, TEXT_SRL,  M_INDX, M_NO),
    IXB(0x46, TEXT_BIT,  M_BIT,  M_INDX),
    IXB(0x86, TEXT_RES,  M_BIT,  M_INDX),
    IXB(0xC6, TEXT_SET,  M_BIT,  M_INDX),
};
static constexpr Config::opcode_t PREFIX_CB = 0xCB;

static constexpr Entry TABLE_ED[] PROGMEM = {
    E(0x70, TEXT_IN,   M_UNKI, M_UNKI), // IN (HL),,(C),
    E(0x40, TEXT_IN,   M_DST,  I_C),
    E(0x71, TEXT_OUT,  M_UNKI, M_UNKI), // OUT (C),,(H),
    E(0x41, TEXT_OUT,  I_C,    M_DST),
    E(0x42, TEXT_SBC,  R_HL,   M_PTR),
    E(0x4A, TEXT_ADC,  R_HL,   M_PTR),
    E(0x63, TEXT_LD,   M_UNKI, M_UNKI), // LD (ABS),,HL
    E(0x43, TEXT_LD,   M_ABS,  M_PTR),
    E(0x6B, TEXT_LD,   M_UNKI, M_UNKI), // LD HL,(ABS),
    E(0x4B, TEXT_LD,   M_PTR,  M_ABS),
    E(0x44, TEXT_NEG,  M_NO,   M_NO),
    E(0x45, TEXT_RETN, M_NO,   M_NO),
    E(0x4D, TEXT_RETI, M_NO,   M_NO),
    E(0x46, TEXT_IM,   M_IMMD, M_NO),
    E(0x47, TEXT_LD,   R_IR,   R_A),
    E(0x57, TEXT_LD,   R_A,    R_IR),
    E(0x67, TEXT_RRD,  M_NO,   M_NO),
    E(0x6F, TEXT_RLD,  M_NO,   M_NO),
    E(0xA0, TEXT_LDI,  M_NO,   M_NO),
    E(0xA8, TEXT_LDD,  M_NO,   M_NO),
    E(0xB0, TEXT_LDIR, M_NO,   M_NO),
    E(0xB8, TEXT_LDDR, M_NO,   M_NO),
    E(0xA1, TEXT_CPI,  M_NO,   M_NO),
    E(0xA9, TEXT_CPD,  M_NO,   M_NO),
    E(0xB1, TEXT_CPIR, M_NO,   M_NO),
    E(0xB9, TEXT_CPDR, M_NO,   M_NO),
    E(0xA2, TEXT_INI,  M_NO,   M_NO),
    E(0xAA, TEXT_IND,  M_NO,   M_NO),
    E(0xB2, TEXT_INIR, M_NO,   M_NO),
    E(0xBA, TEXT_INDR, M_NO,   M_NO),
    E(0xA3, TEXT_OUTI, M_NO,   M_NO),
    E(0xAB, TEXT_OUTD, M_NO,   M_NO),
    E(0xB3, TEXT_OTIR, M_NO,   M_NO),
    E(0xBB, TEXT_OTDR, M_NO,   M_NO),
};
static constexpr Config::opcode_t PREFIX_ED = 0xED;

static constexpr Entry TABLE_IX[] PROGMEM = {
    E(0x09, TEXT_ADD,  R_IXIY, M_PIX),
    E(0x21, TEXT_LD,   R_IXIY, M_IM16),
    E(0x22, TEXT_LD,   M_ABS,  R_IXIY),
    E(0x2A, TEXT_LD,   R_IXIY, M_ABS),
    E(0x23, TEXT_INC,  R_IXIY, M_NO),
    E(0x2B, TEXT_DEC,  R_IXIY, M_NO),
    E(0x34, TEXT_INC,  M_INDX, M_NO),
    E(0x35, TEXT_DEC,  M_INDX, M_NO),
    E(0x36, TEXT_LD,   M_INDX, M_IM8),
    E(0x76, TEXT_LD,   M_UNKI, M_UNKI), // LD (Ix+nn),,(HL),
    E(0x46, TEXT_LD,   M_DST,  M_INDX),
    E(0x70, TEXT_LD,   M_INDX, M_REG),
    E(0x86, TEXT_ADD,  R_A,    M_INDX),
    E(0x8E, TEXT_ADC,  R_A,    M_INDX),
    E(0x96, TEXT_SUB,  R_A,    M_INDX),
    E(0x96, TEXT_SUB,  M_INDX, M_NO),
    E(0x9E, TEXT_SBC,  R_A,    M_INDX),
    E(0xA6, TEXT_AND,  R_A,    M_INDX),
    E(0xA6, TEXT_AND,  M_INDX, M_NO),
    E(0xAE, TEXT_XOR,  R_A,    M_INDX),
    E(0xAE, TEXT_XOR,  M_INDX, M_NO),
    E(0xB6, TEXT_OR,   R_A,    M_INDX),
    E(0xB6, TEXT_OR,   M_INDX, M_NO),
    E(0xBE, TEXT_CP,   R_A,    M_INDX),
    E(0xBE, TEXT_CP,   M_INDX, M_NO),
    E(0xCB, TEXT_BIT,  T_IXB,  T_IXB),  // to TABLE_CB[]
    E(0xE1, TEXT_POP,  R_IXIY, M_NO),
    E(0xE9, TEXT_JP,   I_IXIY, M_NO),
    E(0xF9, TEXT_LD,   R_SP,   R_IXIY),
    E(0xE3, TEXT_EX,   I_SP,   R_IXIY),
    E(0xE5, TEXT_PUSH, R_IXIY, M_NO),
};
// clang-format on

class TableZ80::EntryPage : public EntryPageBase<Entry> {
public:
    constexpr EntryPage(
            Config::opcode_t prefix, const Entry *table, const Entry *end)
        : EntryPageBase(table, end), _prefix(prefix) {}

    Config::opcode_t prefix() const { return pgm_read_byte(&_prefix); }

private:
    Config::opcode_t _prefix;
};

static constexpr TableZ80::EntryPage PAGES_I8080[] PROGMEM = {
        {PREFIX_00, ARRAY_RANGE(TABLE_I8080)},
};
static constexpr TableZ80::EntryPage PAGES_I8085[] PROGMEM = {
        {PREFIX_00, ARRAY_RANGE(TABLE_I8080)},
        {PREFIX_00, ARRAY_RANGE(TABLE_I8085)},
};
static constexpr TableZ80::EntryPage PAGES_Z80[] PROGMEM = {
        {PREFIX_00, ARRAY_RANGE(TABLE_Z80)},
        {PREFIX_00, ARRAY_RANGE(TABLE_I8080)},
        {PREFIX_CB, ARRAY_RANGE(TABLE_CB)},
        {PREFIX_ED, ARRAY_RANGE(TABLE_ED)},
        {TableZ80::PREFIX_IX, ARRAY_RANGE(TABLE_IX)},
        {TableZ80::PREFIX_IY, ARRAY_RANGE(TABLE_IX)},
};

static bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_REG)
        return table == M_DST;
    if (opr == R_A || opr == I_HL)
        return table == M_REG || table == M_DST;
    if (opr == R_C)
        return table == M_REG || table == M_DST || table == M_CC4 ||
               table == M_CC8;
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
        return table == M_IM8 || table == M_REL || table == M_BIT ||
               table == M_VEC || table == M_IMMD;
    if (opr == M_ABS)
        return table == M_IOA;
    if (opr == M_CC4)
        return table == M_CC8;
    return false;
}

static bool acceptModes(Entry::Flags flags, const Entry *entry) {
    const Entry::Flags table = entry->flags();
    return acceptMode(flags.dstMode(), table.dstMode()) &&
           acceptMode(flags.srcMode(), table.srcMode());
}

Error TableZ80::searchName(
        InsnZ80 &insn, const EntryPage *pages, const EntryPage *end) const {
    uint8_t count = 0;
    for (const EntryPage *page = pages; page < end; page++) {
        const Entry *entry = TableBase::searchName<Entry, Entry::Flags>(
                insn.name(), insn.flags(), page->table(), page->end(),
                acceptModes, count);
        if (entry) {
            insn.setOpCode(entry->opCode(), page->prefix());
            insn.setFlags(entry->flags());
            return OK;
        }
    }
    return count == 0 ? UNKNOWN_INSTRUCTION : OPERAND_NOT_ALLOWED;
}

static Config::opcode_t maskCode(Config::opcode_t opCode, const Entry *entry) {
    const AddrMode dst = entry->flags().dstMode();
    const AddrMode src = entry->flags().srcMode();
    Config::opcode_t mask = 0;
    if (dst == M_REG || src == M_REG)
        mask |= 7;
    if (dst == M_CC8 || src == M_DST || dst == M_DST || dst == M_VEC ||
            dst == M_BIT)
        mask |= (7 << 3);
    if (dst == M_PTR || src == M_PTR || dst == M_PIX || src == M_PIX ||
            dst == M_STK || src == M_STK)
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
        if (insn.prefix() != page->prefix())
            continue;
        const Entry *entry = TableBase::searchCode<Entry, Config::opcode_t>(
                insn.opCode(), page->table(), page->end(), maskCode);
        if (entry) {
            insn.setFlags(entry->flags());
            insn.setName_P(entry->name());
            return OK;
        }
    }
    return UNKNOWN_INSTRUCTION;
}

bool TableZ80::isPrefix(Config::opcode_t opCode) const {
    if (_cpuType != Z80)
        return false;
    return opCode == PREFIX_CB || opCode == PREFIX_ED ||
           opCode == TableZ80::PREFIX_IX || opCode == TableZ80::PREFIX_IY;
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
    if (_cpuType == Z80)
        return TEXT_CPU_Z80;
    return _cpuType == I8080 ? TEXT_CPU_8080 : TEXT_CPU_8085;
}

bool TableZ80::setCpu(const char *cpu) {
    if (strcasecmp_P(cpu, TEXT_CPU_Z80) == 0)
        return setCpu(Z80);
    if (toupper(*cpu) == 'I')
        cpu++;
    if (strcmp_P(cpu, TEXT_CPU_8080) == 0)
        return setCpu(I8080);
    if (strcmp_P(cpu, TEXT_CPU_8085) == 0)
        return setCpu(I8085);
    return false;
}

class TableZ80 TableZ80;

}  // namespace z80
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

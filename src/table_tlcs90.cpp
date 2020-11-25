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

#include "table_tlcs90.h"
#include "config_tlcs90.h"
#include "entry_tlcs90.h"
#include "insn_tlcs90.h"
#include "reg_tlcs90.h"
#include "text_tlcs90.h"

#include <ctype.h>
#include <string.h>

namespace libasm {
namespace tlcs90 {

#define X(_opc, _name, _dst, _src, _emit) \
    { _opc, Entry::Flags::create(_dst, _src, _emit), _name }
#define E(_opc, _name, _dst, _src) X(_opc, _name, _dst, _src, false)
#define I(_opc, _name, _dst, _src) X(_opc, _name, _dst, _src, true)

// clang-format off
static constexpr Entry TABLE_TLCS90[] PROGMEM = {
    E(0x00, TEXT_NOP,  M_NO,    M_NO),
    E(0x01, TEXT_HALT, M_NO,    M_NO),
    E(0x02, TEXT_DI,   M_NO,    M_NO),
    E(0x03, TEXT_EI,   M_NO,    M_NO),
    E(0x07, TEXT_INCX, M_DIR,   M_NO),
    E(0x08, TEXT_EX,   R_DE,    R_HL),
    E(0x08, TEXT_EX,   R_HL,    R_DE),
    E(0x09, TEXT_EX,   R_AF,    R_AFP),
    E(0x0A, TEXT_EXX,  M_NO,    M_NO),
    E(0x0B, TEXT_DAA,  R_A,     M_NO),
    E(0x0C, TEXT_RCF,  M_NO,    M_NO),
    E(0x0D, TEXT_SCF,  M_NO,    M_NO),
    E(0x0E, TEXT_CCF,  M_NO,    M_NO),
    E(0x0F, TEXT_DECX, M_DIR,   M_NO),
    E(0x10, TEXT_CPL,  R_A,     M_NO),
    E(0x11, TEXT_NEG,  R_A,     M_NO),
    E(0x12, TEXT_MUL,  R_HL,    M_IMM8),
    E(0x13, TEXT_DIV,  R_HL,    M_IMM8),
    E(0x17, TEXT_LDAR, R_HL,    M_REL16),
    E(0x14, TEXT_ADD,  M_REGIX, M_IMM16),
    E(0x18, TEXT_DJNZ, M_REL8,  M_NO),
    E(0x19, TEXT_DJNZ, R_BC,    M_REL8),
    E(0x1A, TEXT_JP,   M_EXT,   M_NO),
    E(0x1B, TEXT_JRL,  M_REL16, M_NO),
    E(0x1C, TEXT_CALL, M_EXT,   M_NO),
    E(0x1D, TEXT_CALR, M_REL16, M_NO),
    E(0x1E, TEXT_RET,  M_NO,    M_NO),
    E(0x1F, TEXT_RETI, M_NO,    M_NO),
    I(0x27, TEXT_LD,   R_A,     M_DIR),
    E(0x20, TEXT_LD,   R_A,     M_REG8),
    I(0x2F, TEXT_LD,   M_DIR,   R_A),
    E(0x28, TEXT_LD,   M_REG8,  R_A),
    I(0x37, TEXT_LD,   M_DIR,   M_IMM8),
    E(0x30, TEXT_LD,   M_REG8,  M_IMM8),
    E(0x3F, TEXT_LDW,  M_DIR,   M_IMM16),
    E(0x38, TEXT_LD,   M_REG16, M_IMM16),
    I(0x47, TEXT_LD,   R_HL,    M_DIR),
    E(0x40, TEXT_LD,   R_HL,    M_REG16),
    I(0x4F, TEXT_LD,   M_DIR,   R_HL),
    E(0x48, TEXT_LD,   M_REG16, R_HL),
    E(0x50, TEXT_PUSH, M_STACK, M_NO),
    E(0x58, TEXT_POP,  M_STACK, M_NO),
    I(0x60, TEXT_ADD,  R_A,     M_DIR),
    I(0x61, TEXT_ADC,  R_A,     M_DIR),
    I(0x62, TEXT_SUB,  R_A,     M_DIR),
    I(0x63, TEXT_SBC,  R_A,     M_DIR),
    I(0x64, TEXT_AND,  R_A,     M_DIR),
    I(0x65, TEXT_XOR,  R_A,     M_DIR),
    I(0x66, TEXT_OR,   R_A,     M_DIR),
    I(0x67, TEXT_CP,   R_A,     M_DIR),
    I(0x68, TEXT_ADD,  R_A,     M_IMM8),
    I(0x69, TEXT_ADC,  R_A,     M_IMM8),
    I(0x6A, TEXT_SUB,  R_A,     M_IMM8),
    I(0x6B, TEXT_SBC,  R_A,     M_IMM8),
    I(0x6C, TEXT_AND,  R_A,     M_IMM8),
    I(0x6D, TEXT_XOR,  R_A,     M_IMM8),
    I(0x6E, TEXT_OR,   R_A,     M_IMM8),
    I(0x6F, TEXT_CP,   R_A,     M_IMM8),
    E(0x70, TEXT_ADD,  R_HL,    M_DIR),
    E(0x71, TEXT_ADC,  R_HL,    M_DIR),
    E(0x72, TEXT_SUB,  R_HL,    M_DIR),
    E(0x73, TEXT_SBC,  R_HL,    M_DIR),
    E(0x74, TEXT_AND,  R_HL,    M_DIR),
    E(0x75, TEXT_XOR,  R_HL,    M_DIR),
    E(0x76, TEXT_OR,   R_HL,    M_DIR),
    E(0x77, TEXT_CP,   R_HL,    M_DIR),
    E(0x78, TEXT_ADD,  R_HL,    M_IMM16),
    E(0x79, TEXT_ADC,  R_HL,    M_IMM16),
    E(0x7A, TEXT_SUB,  R_HL,    M_IMM16),
    E(0x7B, TEXT_SBC,  R_HL,    M_IMM16),
    E(0x7C, TEXT_AND,  R_HL,    M_IMM16),
    E(0x7D, TEXT_XOR,  R_HL,    M_IMM16),
    E(0x7E, TEXT_OR,   R_HL,    M_IMM16),
    E(0x7F, TEXT_CP,   R_HL,    M_IMM16),
    E(0x87, TEXT_INC,  M_DIR,   M_NO),
    E(0x80, TEXT_INC,  M_REG8,  M_NO),
    E(0x8F, TEXT_DEC,  M_DIR,   M_NO),
    E(0x88, TEXT_DEC,  M_REG8,  M_NO),
    E(0x97, TEXT_INCW, M_DIR,   M_NO),
    E(0x90, TEXT_INC,  M_REG16, M_NO),
    E(0x9F, TEXT_DECW, M_DIR,   M_NO),
    E(0x98, TEXT_DEC,  M_REG16, M_NO),
    E(0xA0, TEXT_RLCA, M_NO,    M_NO),
    E(0xA1, TEXT_RRCA, M_NO,    M_NO),
    E(0xA2, TEXT_RLA,  M_NO,    M_NO),
    E(0xA3, TEXT_RRA,  M_NO,    M_NO),
    E(0xA4, TEXT_SLAA, M_NO,    M_NO),
    E(0xA5, TEXT_SRAA, M_NO,    M_NO),
    E(0xA6, TEXT_SLLA, M_NO,    M_NO),
    E(0xA7, TEXT_SRLA, M_NO,    M_NO),
#if 0
    E(0xA0, TEXT_RLC,  R_A,     M_NO),
    E(0xA1, TEXT_RRC,  R_A,     M_NO),
    E(0xA2, TEXT_RL,   R_A,     M_NO),
    E(0xA3, TEXT_RR,   R_A,     M_NO),
    E(0xA4, TEXT_SLA,  R_A,     M_NO),
    E(0xA5, TEXT_SRA,  R_A,     M_NO),
    E(0xA6, TEXT_SLL,  R_A,     M_NO),
    E(0xA7, TEXT_SRL,  R_A,     M_NO),
#endif
    E(0xA8, TEXT_BIT,  M_BIT,   M_DIR),
    E(0xB0, TEXT_RES,  M_BIT,   M_DIR),
    E(0xB8, TEXT_SET,  M_BIT,   M_DIR),
    E(0xC8, TEXT_JR,   M_REL8,  M_NO),
    E(0xC0, TEXT_JR,   M_CC,    M_REL8),
    E(0xFF, TEXT_SWI,  M_NO,    M_NO),
};

static constexpr Entry TABLE_SRC[] PROGMEM = {
    E(0x10, TEXT_RLD,  M_DST,   M_NO),
    E(0x11, TEXT_RRD,  M_DST,   M_NO),
    E(0x12, TEXT_MUL,  R_HL,    M_SRC),
    E(0x13, TEXT_DIV,  R_HL,    M_SRC),
    E(0x14, TEXT_ADD,  M_REGIX, M_SRC),
    E(0x18, TEXT_TSET, M_BIT,   M_SRC),
    I(0x28, TEXT_LD,   M_REG8,  M_SRC),
    E(0x48, TEXT_LD,   M_REG16, M_SRC),
    E(0x50, TEXT_EX,   M_DST,   M_REG16),
    E(0x50, TEXT_EX,   M_REG16, M_SRC),
    E(0x60, TEXT_ADD,  R_A,     M_SRC),
    E(0x61, TEXT_ADC,  R_A,     M_SRC),
    E(0x62, TEXT_SUB,  R_A,     M_SRC),
    E(0x63, TEXT_SBC,  R_A,     M_SRC),
    E(0x64, TEXT_AND,  R_A,     M_SRC),
    E(0x65, TEXT_XOR,  R_A,     M_SRC),
    E(0x66, TEXT_OR,   R_A,     M_SRC),
    E(0x67, TEXT_CP,   R_A,     M_SRC),
    E(0x70, TEXT_ADD,  R_HL,    M_SRC),
    E(0x71, TEXT_ADC,  R_HL,    M_SRC),
    E(0x72, TEXT_SUB,  R_HL,    M_SRC),
    E(0x73, TEXT_SBC,  R_HL,    M_SRC),
    E(0x74, TEXT_AND,  R_HL,    M_SRC),
    E(0x75, TEXT_XOR,  R_HL,    M_SRC),
    E(0x76, TEXT_OR,   R_HL,    M_SRC),
    E(0x77, TEXT_CP,   R_HL,    M_SRC),
    E(0x87, TEXT_INC,  M_DST,   M_NO),
    E(0x8F, TEXT_DEC,  M_DST,   M_NO),
    E(0x97, TEXT_INCW, M_DST,   M_NO),
    E(0x9F, TEXT_DECW, M_DST,   M_NO),
    E(0xA0, TEXT_RLC,  M_DST,   M_NO),
    E(0xA1, TEXT_RRC,  M_DST,   M_NO),
    E(0xA2, TEXT_RL,   M_DST,   M_NO),
    E(0xA3, TEXT_RR,   M_DST,   M_NO),
    E(0xA4, TEXT_SLA,  M_DST,   M_NO),
    E(0xA5, TEXT_SRA,  M_DST,   M_NO),
    E(0xA6, TEXT_SLL,  M_DST,   M_NO),
    E(0xA7, TEXT_SRL,  M_DST,   M_NO),
    E(0xA8, TEXT_BIT,  M_BIT,   M_SRC),
    E(0xB0, TEXT_RES,  M_BIT,   M_SRC),
    E(0xB8, TEXT_SET,  M_BIT,   M_SRC),
};

static constexpr Entry TABLE_DST[] PROGMEM = {
    E(0x20, TEXT_LD,   M_DST,   M_REG8),
    E(0x37, TEXT_LD,   M_DST,   M_IMM8),
    E(0x3F, TEXT_LDW,  M_DST,   M_IMM16),
    E(0x40, TEXT_LD,   M_DST,   M_REG16),
    E(0x68, TEXT_ADD,  M_DST,   M_IMM8),
    E(0x69, TEXT_ADC,  M_DST,   M_IMM8),
    E(0x6A, TEXT_SUB,  M_DST,   M_IMM8),
    E(0x6B, TEXT_SBC,  M_DST,   M_IMM8),
    E(0x6C, TEXT_AND,  M_DST,   M_IMM8),
    E(0x6D, TEXT_XOR,  M_DST,   M_IMM8),
    E(0x6E, TEXT_OR,   M_DST,   M_IMM8),
    E(0x6F, TEXT_CP,   M_DST,   M_IMM8),
};

static constexpr Entry TABLE_REG[] PROGMEM = {
    E(0x12, TEXT_MUL,  R_HL,    M_SRC),
    E(0x13, TEXT_DIV,  R_HL,    M_SRC),
    E(0x18, TEXT_TSET, M_BIT,   M_SRC),
    I(0x30, TEXT_LD,   M_REG8,  M_SRC),
    E(0x60, TEXT_ADD,  R_A,     M_SRC),
    E(0x61, TEXT_ADC,  R_A,     M_SRC),
    E(0x62, TEXT_SUB,  R_A,     M_SRC),
    E(0x63, TEXT_SBC,  R_A,     M_SRC),
    E(0x64, TEXT_AND,  R_A,     M_SRC),
    E(0x65, TEXT_XOR,  R_A,     M_SRC),
    E(0x66, TEXT_OR,   R_A,     M_SRC),
    E(0x67, TEXT_CP,   R_A,     M_SRC),
    E(0x68, TEXT_ADD,  M_DST,   M_IMM8),
    E(0x69, TEXT_ADC,  M_DST,   M_IMM8),
    E(0x6A, TEXT_SUB,  M_DST,   M_IMM8),
    E(0x6B, TEXT_SBC,  M_DST,   M_IMM8),
    E(0x6C, TEXT_AND,  M_DST,   M_IMM8),
    E(0x6D, TEXT_XOR,  M_DST,   M_IMM8),
    E(0x6E, TEXT_OR,   M_DST,   M_IMM8),
    E(0x6F, TEXT_CP,   M_DST,   M_IMM8),
    E(0xA0, TEXT_RLC,  M_DST,   M_NO),
    E(0xA1, TEXT_RRC,  M_DST,   M_NO),
    E(0xA2, TEXT_RL,   M_DST,   M_NO),
    E(0xA3, TEXT_RR,   M_DST,   M_NO),
    E(0xA4, TEXT_SLA,  M_DST,   M_NO),
    E(0xA5, TEXT_SRA,  M_DST,   M_NO),
    E(0xA6, TEXT_SLL,  M_DST,   M_NO),
    E(0xA7, TEXT_SRL,  M_DST,   M_NO),
    E(0xA8, TEXT_BIT,  M_BIT,   M_SRC),
    E(0xB0, TEXT_RES,  M_BIT,   M_SRC),
    E(0xB8, TEXT_SET,  M_BIT,   M_SRC),
    E(0x14, TEXT_ADD,  M_REGIX, M_SRC16),
    E(0x38, TEXT_LD,   M_REG16, M_SRC16),
    E(0x70, TEXT_ADD,  R_HL,    M_SRC16),
    E(0x71, TEXT_ADC,  R_HL,    M_SRC16),
    E(0x72, TEXT_SUB,  R_HL,    M_SRC16),
    E(0x73, TEXT_SBC,  R_HL,    M_SRC16),
    E(0x74, TEXT_AND,  R_HL,    M_SRC16),
    E(0x75, TEXT_XOR,  R_HL,    M_SRC16),
    E(0x76, TEXT_OR,   R_HL,    M_SRC16),
    E(0x77, TEXT_CP,   R_HL,    M_SRC16),
};

static constexpr Entry TABLE_LDA[] PROGMEM = {
    E(0x38, TEXT_LDA,  M_REG16, M_SRC),
};

static constexpr Entry TABLE_JP_CALL[] PROGMEM = {
    E(0xC0, TEXT_JP,   M_CC,    M_SRC),
    E(0xC8, TEXT_JP,   M_DST,   M_NO),
    E(0xD0, TEXT_CALL, M_CC,    M_SRC),
    E(0xD8, TEXT_CALL, M_DST,   M_NO),
};

static constexpr Entry TABLE_COND[] PROGMEM = {
    E(0xD8, TEXT_RET,  M_NO,    M_NO),
    E(0xD0, TEXT_RET,  M_CC,    M_NO),
};

static constexpr Entry TABLE_BLOCK[] PROGMEM = {
    E(0x58, TEXT_LDI,  M_NO,    M_NO),
    E(0x59, TEXT_LDIR, M_NO,    M_NO),
    E(0x5A, TEXT_LDD,  M_NO,    M_NO),
    E(0x5B, TEXT_LDDR, M_NO,    M_NO),
    E(0x5C, TEXT_CPI,  M_NO,    M_NO),
    E(0x5D, TEXT_CPIR, M_NO,    M_NO),
    E(0x5E, TEXT_CPD,  M_NO,    M_NO),
    E(0x5F, TEXT_CPDR, M_NO,    M_NO),
};

// clang-format on

class TableTlcs90::EntryPage : public EntryPageBase<Entry> {
public:
    constexpr EntryPage(Config::opcode_t prefix, AddrMode mode,
            const Entry *table, const Entry *end)
        : EntryPageBase(table, end), _prefix(prefix), _mode(uint8_t(mode)) {}

    Config::opcode_t prefix() const { return pgm_read_byte(&_prefix); }
    AddrMode mode() const { return AddrMode(pgm_read_byte(&_mode)); }
    bool prefixMatch(Config::opcode_t code) const {
        const Config::opcode_t pre = prefix();
        const Config::opcode_t reg = code & 7;
        switch (mode()) {
        case M_IND:
            return (code & ~7) == pre && reg != 3 && reg != 7;
        case M_IDX:
            return (code & ~3) == pre && reg != 7;
        case M_REG8:
            return (code & ~7) == pre && reg != 7;
        case M_CC:
            return (code & ~0xF) == pre;
        default:
            return code == pre;
        }
    }

private:
    Config::opcode_t _prefix;
    uint8_t _mode;
};

static constexpr TableTlcs90::EntryPage PAGES_TLCS90[] PROGMEM = {
        {0x00, M_NO, ARRAY_RANGE(TABLE_TLCS90)},
        {0xE7, M_DIR, ARRAY_RANGE(TABLE_SRC)},       // src (FFnn)
        {0xE3, M_EXT, ARRAY_RANGE(TABLE_SRC)},       // src (nnnn)
        {0xE0, M_IND, ARRAY_RANGE(TABLE_SRC)},       // src (rr)
        {0xF3, M_BASE, ARRAY_RANGE(TABLE_SRC)},      // src (HL+A)
        {0xF0, M_IDX, ARRAY_RANGE(TABLE_SRC)},       // src (ix+d)
        {0xEF, M_DIR, ARRAY_RANGE(TABLE_DST)},       // dst (FFnn)
        {0xEB, M_EXT, ARRAY_RANGE(TABLE_DST)},       // dst (nnnn)
        {0xE8, M_IND, ARRAY_RANGE(TABLE_DST)},       // dst (rr)
        {0xF7, M_BASE, ARRAY_RANGE(TABLE_DST)},      // dst (HL+A)
        {0xF4, M_IDX, ARRAY_RANGE(TABLE_DST)},       // dst (ix+d)
        {0xEB, M_EXT, ARRAY_RANGE(TABLE_JP_CALL)},   // JP/CALL
        {0xE8, M_IND, ARRAY_RANGE(TABLE_JP_CALL)},   // JP/CALL
        {0xF7, M_BASE, ARRAY_RANGE(TABLE_JP_CALL)},  // JP/CALL
        {0xF4, M_IDX, ARRAY_RANGE(TABLE_JP_CALL)},   // JP/CALL
        {0xF8, M_REG8, ARRAY_RANGE(TABLE_REG)},      // r, rr
        {0xF7, M_BASE, ARRAY_RANGE(TABLE_LDA)},      // LDA
        {0xF4, M_IDX, ARRAY_RANGE(TABLE_LDA)},       // LDA
        {0xFE, M_NO, ARRAY_RANGE(TABLE_COND)},
        {0xFE, M_NO, ARRAY_RANGE(TABLE_BLOCK)},
};

Error TableTlcs90::readInsn(
        DisMemory &memory, InsnTlcs90 &insn, Operand &op) const {
    Config::opcode_t code = insn.readByte(memory);
    for (const EntryPage *page = ARRAY_BEGIN(PAGES_TLCS90) + 1;
            page < ARRAY_END(PAGES_TLCS90); page++) {
        if (!page->prefixMatch(code))
            continue;
        op.mode = page->mode();
        switch (op.mode) {
        case M_EXT:
            op.val16 = insn.readUint16(memory);
            break;
        case M_DIR:
            op.val16 = insn.readByte(memory);
            break;
        case M_IND:
            op.reg = RegTlcs90::decodeReg16(code);
            break;
        case M_IDX:
            op.reg = RegTlcs90::decodeIndexReg(code);
            op.val16 = static_cast<int8_t>(insn.readByte(memory));
            break;
        case M_REG8:
            op.reg = RegTlcs90::decodeReg8(code);
            break;
        case M_CC:
            op.cc = RegTlcs90::decodeCcName(code);
            break;
        default:
            break;
        }
        insn.setOpCode(insn.readByte(memory), code);
        return _error.setError(insn);
    }
    insn.setOpCode(code);
    return _error.setError(insn);
}

static bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == R_A)
        return table == M_REG8;
    if (opr == R_C)
        return table == M_REG8 || table == M_CC;
    if (opr == R_BC || opr == R_DE || opr == R_HL)
        return table == M_REG16 || table == M_STACK;
    if (opr == M_REGIX || opr == R_SP)
        return table == M_REG16 || table == M_STACK || table == M_REGIX;
    if (opr == M_IMM16)
        return table == M_IMM8 || table == M_BIT || table == M_REL8 ||
               table == M_REL16;
    if (opr == M_DIR)
        return table == M_EXT;
    if (opr == R_AF)
        return table == M_STACK;
    return false;
}

static bool acceptModes(InsnTlcs90 &insn, const Entry *entry) {
    const Entry::Flags table = entry->flags();
    const AddrMode tableDst = table.dstMode();
    const AddrMode tableSrc = table.srcMode();
    const AddrMode dst = (tableDst == M_DST) ? insn.preMode() : tableDst;
    const AddrMode src = (tableSrc == M_SRC)
                                 ? insn.preMode()
                                 : (tableSrc == M_SRC16 ? M_REG16 : tableSrc);
    if (acceptMode(insn.dstMode(), dst) && acceptMode(insn.srcMode(), src)) {
        insn.setAddrMode(dst, src, table.emit());
        // Update prefix mode.
        if (tableDst == M_DST) {
            insn.setPreMode(M_DST);
        } else if (tableSrc == M_SRC || tableSrc == M_SRC16) {
            insn.setPreMode(M_SRC);
        } else {
            insn.setPreMode(M_NO);
        }
        return true;
    }
    return false;
}

Error TableTlcs90::searchName(
        InsnTlcs90 &insn, const EntryPage *pages, const EntryPage *end) const {
    uint8_t count = 0;
    for (const EntryPage *page = pages; page < end; page++) {
        insn.setPreMode(page->mode());
        const Entry *entry =
                TableBase::searchName<Entry, InsnTlcs90 &>(insn.name(), insn,
                        page->table(), page->end(), acceptModes, count);
        if (entry) {
            insn.setOpCode(entry->opCode(), page->prefix());
            return OK;
        }
    }
    return count == 0 ? UNKNOWN_INSTRUCTION : OPERAND_NOT_ALLOWED;
}

static Config::opcode_t maskCode(Config::opcode_t opCode, const Entry *entry) {
    const AddrMode dst = entry->flags().dstMode();
    Config::opcode_t mask = 0;
    switch (dst) {
    case M_REGIX:
        mask |= 3;
        break;
    case M_BIT:
    case M_STACK:
    case M_REG8:
    case M_REG16:
        mask |= 7;
        break;
    case M_CC:
        mask |= 0xF;
        break;
    default:
        break;
    }
    const AddrMode src = entry->flags().srcMode();
    switch (src) {
    case M_REG8:
    case M_REG16:
        mask |= 7;
    default:
        break;
    }
    return opCode & ~mask;
}

Error TableTlcs90::searchOpCode(
        InsnTlcs90 &insn, const EntryPage *pages, const EntryPage *end) const {
    for (const EntryPage *page = pages; page < end; page++) {
        if (!page->prefixMatch(insn.prefix()))
            continue;
        const Entry *entry = TableBase::searchCode<Entry, Config::opcode_t>(
                insn.opCode(), page->table(), page->end(), maskCode);
        if (entry) {
            insn.setFlags(entry->flags());
            insn.setName_P(entry->name());
            if (insn.srcMode() == M_UNKI)
                return UNKNOWN_INSTRUCTION;
            return OK;
        }
    }
    return UNKNOWN_INSTRUCTION;
}

Error TableTlcs90::searchName(InsnTlcs90 &insn) const {
    return _error.setError(searchName(insn, ARRAY_RANGE(PAGES_TLCS90)));
}

Error TableTlcs90::searchOpCode(InsnTlcs90 &insn) const {
    return _error.setError(searchOpCode(insn, ARRAY_RANGE(PAGES_TLCS90)));
}

const char *TableTlcs90::listCpu() const {
    return getCpu();
}

const char *TableTlcs90::getCpu() const {
    return TEXT_CPU_TLCS90;
}

bool TableTlcs90::setCpu(const char *cpu) {
    return strcasecmp_P(cpu, TEXT_CPU_TLCS90) == 0;
}

class TableTlcs90 TableTlcs90;

}  // namespace tlcs90
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

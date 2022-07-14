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

static constexpr uint8_t INDEX_I8080[] PROGMEM = {
     25,  // TEXT_ADC
     56,  // TEXT_ADC
      2,  // TEXT_ADD
     24,  // TEXT_ADD
     55,  // TEXT_ADD
     29,  // TEXT_AND
     30,  // TEXT_AND
     60,  // TEXT_AND
     61,  // TEXT_AND
     52,  // TEXT_CALL
     54,  // TEXT_CALL
     21,  // TEXT_CCF
     35,  // TEXT_CP
     36,  // TEXT_CP
     66,  // TEXT_CP
     67,  // TEXT_CP
     19,  // TEXT_CPL
     18,  // TEXT_DAA
     10,  // TEXT_DEC
     12,  // TEXT_DEC
     50,  // TEXT_DI
     51,  // TEXT_EI
     46,  // TEXT_EX
     47,  // TEXT_EX
     48,  // TEXT_EX
     49,  // TEXT_EX
     22,  // TEXT_HALT
     45,  // TEXT_IN
      9,  // TEXT_INC
     11,  // TEXT_INC
     41,  // TEXT_JP
     42,  // TEXT_JP
     43,  // TEXT_JP
      1,  // TEXT_LD
      3,  // TEXT_LD
      4,  // TEXT_LD
      5,  // TEXT_LD
      6,  // TEXT_LD
      7,  // TEXT_LD
      8,  // TEXT_LD
     13,  // TEXT_LD
     23,  // TEXT_LD
     40,  // TEXT_LD
      0,  // TEXT_NOP
     33,  // TEXT_OR
     34,  // TEXT_OR
     64,  // TEXT_OR
     65,  // TEXT_OR
     44,  // TEXT_OUT
     38,  // TEXT_POP
     53,  // TEXT_PUSH
     37,  // TEXT_RET
     39,  // TEXT_RET
     16,  // TEXT_RLA
     14,  // TEXT_RLCA
     17,  // TEXT_RRA
     15,  // TEXT_RRCA
     68,  // TEXT_RST
     28,  // TEXT_SBC
     59,  // TEXT_SBC
     20,  // TEXT_SCF
     26,  // TEXT_SUB
     27,  // TEXT_SUB
     57,  // TEXT_SUB
     58,  // TEXT_SUB
     31,  // TEXT_XOR
     32,  // TEXT_XOR
     62,  // TEXT_XOR
     63,  // TEXT_XOR
};

static constexpr Entry TABLE_I8085[] PROGMEM = {
    E(0x20, TEXT_LD,   R_A,    R_IM),
    E(0x30, TEXT_LD,   R_IM,   R_A),
};
static constexpr uint8_t INDEX_I8085[] PROGMEM = {
      0,  // TEXT_LD
      1,  // TEXT_LD
};

static constexpr Config::opcode_t PREFIX_00 = 0x00;
static constexpr Entry TABLE_Z80[] PROGMEM = {
    E(0x08, TEXT_EX,   R_AF,   R_AFP),
    E(0x10, TEXT_DJNZ, M_REL,  M_NO),
    E(0x20, TEXT_JR,   M_CC4,  M_REL),
    E(0x18, TEXT_JR,   M_REL,  M_NO),
    E(0xD9, TEXT_EXX,  M_NO,   M_NO),
};
static constexpr uint8_t INDEX_Z80[] PROGMEM = {
      1,  // TEXT_DJNZ
      0,  // TEXT_EX
      4,  // TEXT_EXX
      2,  // TEXT_JR
      3,  // TEXT_JR
};

static constexpr Config::opcode_t PREFIX_CB = 0xCB;
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
static constexpr uint8_t INDEX_CB[] PROGMEM = {
      7,  // TEXT_BIT
     17,  // TEXT_BIT
      8,  // TEXT_RES
     18,  // TEXT_RES
      2,  // TEXT_RL
     12,  // TEXT_RL
      0,  // TEXT_RLC
     10,  // TEXT_RLC
      3,  // TEXT_RR
     13,  // TEXT_RR
      1,  // TEXT_RRC
     11,  // TEXT_RRC
      9,  // TEXT_SET
     19,  // TEXT_SET
      4,  // TEXT_SLA
     14,  // TEXT_SLA
      5,  // TEXT_SRA
     15,  // TEXT_SRA
      6,  // TEXT_SRL
     16,  // TEXT_SRL
};

static constexpr Config::opcode_t PREFIX_ED = 0xED;
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
static constexpr uint8_t INDEX_ED[] PROGMEM = {
      5,  // TEXT_ADC
     23,  // TEXT_CPD
     25,  // TEXT_CPDR
     22,  // TEXT_CPI
     24,  // TEXT_CPIR
     13,  // TEXT_IM
      0,  // TEXT_IN
      1,  // TEXT_IN
     27,  // TEXT_IND
     29,  // TEXT_INDR
     26,  // TEXT_INI
     28,  // TEXT_INIR
      6,  // TEXT_LD
      7,  // TEXT_LD
      8,  // TEXT_LD
      9,  // TEXT_LD
     14,  // TEXT_LD
     15,  // TEXT_LD
     19,  // TEXT_LDD
     21,  // TEXT_LDDR
     18,  // TEXT_LDI
     20,  // TEXT_LDIR
     10,  // TEXT_NEG
     33,  // TEXT_OTDR
     32,  // TEXT_OTIR
      2,  // TEXT_OUT
      3,  // TEXT_OUT
     31,  // TEXT_OUTD
     30,  // TEXT_OUTI
     12,  // TEXT_RETI
     11,  // TEXT_RETN
     17,  // TEXT_RLD
     16,  // TEXT_RRD
      4,  // TEXT_SBC
};

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
static constexpr uint8_t INDEX_IX[] PROGMEM = {
     13,  // TEXT_ADC
      0,  // TEXT_ADD
     12,  // TEXT_ADD
     17,  // TEXT_AND
     18,  // TEXT_AND
     25,  // TEXT_BIT
     23,  // TEXT_CP
     24,  // TEXT_CP
      5,  // TEXT_DEC
      7,  // TEXT_DEC
     29,  // TEXT_EX
      4,  // TEXT_INC
      6,  // TEXT_INC
     27,  // TEXT_JP
      1,  // TEXT_LD
      2,  // TEXT_LD
      3,  // TEXT_LD
      8,  // TEXT_LD
      9,  // TEXT_LD
     10,  // TEXT_LD
     11,  // TEXT_LD
     28,  // TEXT_LD
     21,  // TEXT_OR
     22,  // TEXT_OR
     26,  // TEXT_POP
     30,  // TEXT_PUSH
     16,  // TEXT_SBC
     14,  // TEXT_SUB
     15,  // TEXT_SUB
     19,  // TEXT_XOR
     20,  // TEXT_XOR
};

static constexpr Entry TABLE_V30EMU[] PROGMEM = {
    E(0xED, TEXT_CALLN, M_IM8, M_NO),
    E(0xFD, TEXT_RETEM, M_NO,  M_NO),
};

static constexpr uint8_t INDEX_V30EMU[] PROGMEM = {
      0,  // TEXT_CALLN
      1,  // TEXT_RETEM
};
// clang-format on

static constexpr TableZ80::EntryPage I8080_PAGES[] PROGMEM = {
        {PREFIX_00, ARRAY_RANGE(TABLE_I8080), ARRAY_RANGE(INDEX_I8080)},
};

static constexpr TableZ80::EntryPage I8085_PAGES[] PROGMEM = {
        {PREFIX_00, ARRAY_RANGE(TABLE_I8080), ARRAY_RANGE(INDEX_I8080)},
        {PREFIX_00, ARRAY_RANGE(TABLE_I8085), ARRAY_RANGE(INDEX_I8085)},
};

static constexpr TableZ80::EntryPage Z80_PAGES[] PROGMEM = {
        {PREFIX_00, ARRAY_RANGE(TABLE_Z80), ARRAY_RANGE(INDEX_Z80)},
        {PREFIX_00, ARRAY_RANGE(TABLE_I8080), ARRAY_RANGE(INDEX_I8080)},
        {PREFIX_CB, ARRAY_RANGE(TABLE_CB), ARRAY_RANGE(INDEX_CB)},
        {PREFIX_ED, ARRAY_RANGE(TABLE_ED), ARRAY_RANGE(INDEX_ED)},
        {TableZ80::PREFIX_IX, ARRAY_RANGE(TABLE_IX), ARRAY_RANGE(INDEX_IX)},
        {TableZ80::PREFIX_IY, ARRAY_RANGE(TABLE_IX), ARRAY_RANGE(INDEX_IX)},
};

static constexpr TableZ80::EntryPage V30EMU_PAGES[] PROGMEM = {
        {PREFIX_ED, ARRAY_RANGE(TABLE_V30EMU), ARRAY_RANGE(INDEX_V30EMU)},
        {PREFIX_00, ARRAY_RANGE(TABLE_I8080), ARRAY_RANGE(INDEX_I8080)},
};

static constexpr TableZ80::Cpu CPU_TABLES[] PROGMEM = {
        {Z80, TEXT_CPU_Z80, ARRAY_RANGE(Z80_PAGES)},
        {I8080, TEXT_CPU_8080, ARRAY_RANGE(I8080_PAGES)},
        {I8085, TEXT_CPU_8085, ARRAY_RANGE(I8085_PAGES)},
        {V30EMU, TEXT_CPU_V30EMU, ARRAY_RANGE(V30EMU_PAGES)},
};

static bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_REG)
        return table == M_DST;
    if (opr == R_A || opr == I_HL)
        return table == M_REG || table == M_DST;
    if (opr == R_C)
        return table == M_REG || table == M_DST || table == M_CC4 || table == M_CC8;
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
        return table == M_IM8 || table == M_REL || table == M_BIT || table == M_VEC ||
               table == M_IMMD;
    if (opr == M_ABS)
        return table == M_IOA;
    if (opr == M_CC4)
        return table == M_CC8;
    return false;
}

static bool acceptModes(Entry::Flags flags, const Entry *entry) {
    auto table = entry->flags();
    return acceptMode(flags.dstMode(), table.dstMode()) &&
           acceptMode(flags.srcMode(), table.srcMode());
}

Error TableZ80::searchName(InsnZ80 &insn) {
    uint8_t count = 0;
    for (auto page = _cpu->table(); page < _cpu->end(); page++) {
        auto entry = searchEntry(insn.name(), insn.flags(), page, acceptModes, count);
        if (entry) {
            insn.setOpCode(entry->opCode(), page->prefix());
            insn.setFlags(entry->flags());
            return setOK();
        }
    }
    return setError(count == 0 ? UNKNOWN_INSTRUCTION : OPERAND_NOT_ALLOWED);
}

static Config::opcode_t maskCode(Config::opcode_t opCode, const Entry *entry) {
    auto dst = entry->flags().dstMode();
    auto src = entry->flags().srcMode();
    Config::opcode_t mask = 0;
    if (dst == M_REG || src == M_REG)
        mask |= 7;
    if (dst == M_CC8 || src == M_DST || dst == M_DST || dst == M_VEC || dst == M_BIT)
        mask |= (7 << 3);
    if (dst == M_PTR || src == M_PTR || dst == M_PIX || src == M_PIX || dst == M_STK ||
            src == M_STK)
        mask |= (3 << 4);
    if (dst == I_BCDE || src == I_BCDE)
        mask |= (1 << 4);
    if (dst == M_CC4 || dst == M_IMMD)
        mask |= (3 << 3);
    if (dst == R_IR || src == R_IR)
        mask |= (1 << 3);
    return opCode & ~mask;
}

Error TableZ80::searchOpCode(InsnZ80 &insn) {
    for (auto page = _cpu->table(); page < _cpu->end(); page++) {
        if (insn.prefix() != page->prefix())
            continue;
        auto entry = searchEntry(insn.opCode(), page->table(), page->end(), maskCode);
        if (entry) {
            insn.setFlags(entry->flags());
            insn.setName_P(entry->name_P());
            return setOK();
        }
    }
    return setError(UNKNOWN_INSTRUCTION);
}

bool TableZ80::isPrefix(Config::opcode_t opCode) const {
    for (auto page = _cpu->table(); page < _cpu->end(); page++) {
        const auto prefix = page->prefix();
        if (prefix == 0)
            continue;
        if (prefix == opCode)
            return true;
    }
    return false;
}

TableZ80::TableZ80() {
    setCpu(Z80);
}

bool TableZ80::setCpu(CpuType cpuType) {
    auto t = Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLES));
    if (t == nullptr)
        return false;
    _cpu = t;
    return true;
}

const /* PROGMEM */ char *TableZ80::listCpu_P() const {
    return TEXT_CPU_LIST;
}

const /* PROGMEM */ char *TableZ80::cpu_P() const {
    return _cpu->name_P();
}

bool TableZ80::setCpu(const char *cpu) {
    auto t = Cpu::search(cpu, ARRAY_RANGE(CPU_TABLES));
    if (t)
        return setCpu(t->cpuType());
    if (strcasecmp_P(cpu, TEXT_CPU_I8080) == 0)
        return setCpu(I8080);
    if (strcasecmp_P(cpu, TEXT_CPU_I8085) == 0)
        return setCpu(I8085);
    return false;
}

TableZ80 TableZ80::TABLE;

}  // namespace z80
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

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

#include "entry_table.h"
#include "entry_tlcs90.h"
#include "text_tlcs90.h"

using namespace libasm::text::tlcs90;
using namespace libasm::tlcs90::reg;

namespace libasm {
namespace tlcs90 {

#define E2(_opc, _name, _dst, _src) \
    { _opc, Entry::Flags::create(_dst, _src), _name }
#define E1(_opc, _name, _dst) E2(_opc, _name, _dst, M_NONE)
#define E0(_opc, _name) E1(_opc, _name, M_NONE)

// clang-format off
static constexpr Entry TABLE_TLCS90[] PROGMEM = {
    E0(0x00, TEXT_NOP),
    E0(0x01, TEXT_HALT),
    E0(0x02, TEXT_DI),
    E0(0x03, TEXT_EI),
    E1(0x07, TEXT_INCX, M_DIR),
    E2(0x08, TEXT_EX,   R_DE,    R_HL),
    E2(0x08, TEXT_EX,   R_HL,    R_DE),
    E2(0x09, TEXT_EX,   R_AF,    R_AFP),
    E0(0x0A, TEXT_EXX),
    E1(0x0B, TEXT_DAA,  R_A),
    E0(0x0C, TEXT_RCF),
    E0(0x0D, TEXT_SCF),
    E0(0x0E, TEXT_CCF),
    E1(0x0F, TEXT_DECX, M_DIR),
    E1(0x10, TEXT_CPL,  R_A),
    E1(0x11, TEXT_NEG,  R_A),
    E2(0x12, TEXT_MUL,  R_HL,    M_IMM8),
    E2(0x13, TEXT_DIV,  R_HL,    M_IMM8),
    E2(0x17, TEXT_LDAR, R_HL,    M_REL16),
    E2(0x14, TEXT_ADD,  M_REGIX, M_IMM16),
    E1(0x18, TEXT_DJNZ, M_REL8),
    E2(0x19, TEXT_DJNZ, R_BC,    M_REL8),
    E1(0x1A, TEXT_JP,   M_EXT),
    E1(0x1B, TEXT_JRL,  M_REL16),
    E1(0x1C, TEXT_CALL, M_EXT),
    E1(0x1D, TEXT_CALR, M_REL16),
    E0(0x1E, TEXT_RET),
    E0(0x1F, TEXT_RETI),
    E2(0x27, TEXT_LD,   R_A,     M_DIR),
    E2(0x20, TEXT_LD,   R_A,     M_REG8),
    E2(0x2F, TEXT_LD,   M_DIR,   R_A),
    E2(0x28, TEXT_LD,   M_REG8,  R_A),
    E2(0x37, TEXT_LD,   M_DIR,   M_IMM8),
    E2(0x30, TEXT_LD,   M_REG8,  M_IMM8),
    E2(0x3F, TEXT_LDW,  M_DIR,   M_IMM16),
    E2(0x38, TEXT_LD,   M_REG16, M_IMM16),
    E2(0x47, TEXT_LD,   R_HL,    M_DIR),
    E2(0x40, TEXT_LD,   R_HL,    M_REG16),
    E2(0x4F, TEXT_LD,   M_DIR,   R_HL),
    E2(0x48, TEXT_LD,   M_REG16, R_HL),
    E1(0x50, TEXT_PUSH, M_STACK),
    E1(0x58, TEXT_POP,  M_STACK),
    E2(0x60, TEXT_ADD,  R_A,     M_DIR),
    E2(0x61, TEXT_ADC,  R_A,     M_DIR),
    E2(0x62, TEXT_SUB,  R_A,     M_DIR),
    E2(0x63, TEXT_SBC,  R_A,     M_DIR),
    E2(0x64, TEXT_AND,  R_A,     M_DIR),
    E2(0x65, TEXT_XOR,  R_A,     M_DIR),
    E2(0x66, TEXT_OR,   R_A,     M_DIR),
    E2(0x67, TEXT_CP,   R_A,     M_DIR),
    E2(0x68, TEXT_ADD,  R_A,     M_IMM8),
    E2(0x69, TEXT_ADC,  R_A,     M_IMM8),
    E2(0x6A, TEXT_SUB,  R_A,     M_IMM8),
    E2(0x6B, TEXT_SBC,  R_A,     M_IMM8),
    E2(0x6C, TEXT_AND,  R_A,     M_IMM8),
    E2(0x6D, TEXT_XOR,  R_A,     M_IMM8),
    E2(0x6E, TEXT_OR,   R_A,     M_IMM8),
    E2(0x6F, TEXT_CP,   R_A,     M_IMM8),
    E2(0x70, TEXT_ADD,  R_HL,    M_DIR),
    E2(0x71, TEXT_ADC,  R_HL,    M_DIR),
    E2(0x72, TEXT_SUB,  R_HL,    M_DIR),
    E2(0x73, TEXT_SBC,  R_HL,    M_DIR),
    E2(0x74, TEXT_AND,  R_HL,    M_DIR),
    E2(0x75, TEXT_XOR,  R_HL,    M_DIR),
    E2(0x76, TEXT_OR,   R_HL,    M_DIR),
    E2(0x77, TEXT_CP,   R_HL,    M_DIR),
    E2(0x78, TEXT_ADD,  R_HL,    M_IMM16),
    E2(0x79, TEXT_ADC,  R_HL,    M_IMM16),
    E2(0x7A, TEXT_SUB,  R_HL,    M_IMM16),
    E2(0x7B, TEXT_SBC,  R_HL,    M_IMM16),
    E2(0x7C, TEXT_AND,  R_HL,    M_IMM16),
    E2(0x7D, TEXT_XOR,  R_HL,    M_IMM16),
    E2(0x7E, TEXT_OR,   R_HL,    M_IMM16),
    E2(0x7F, TEXT_CP,   R_HL,    M_IMM16),
    E1(0x87, TEXT_INC,  M_DIR),
    E1(0x80, TEXT_INC,  M_REG8),
    E1(0x8F, TEXT_DEC,  M_DIR),
    E1(0x88, TEXT_DEC,  M_REG8),
    E1(0x97, TEXT_INCW, M_DIR),
    E1(0x90, TEXT_INC,  M_REG16),
    E1(0x9F, TEXT_DECW, M_DIR),
    E1(0x98, TEXT_DEC,  M_REG16),
    E0(0xA0, TEXT_RLCA),
    E0(0xA1, TEXT_RRCA),
    E0(0xA2, TEXT_RLA),
    E0(0xA3, TEXT_RRA),
    E0(0xA4, TEXT_SLAA),
    E0(0xA5, TEXT_SRAA),
    E0(0xA6, TEXT_SLLA),
    E0(0xA7, TEXT_SRLA),
    E2(0xA8, TEXT_BIT,  M_BIT,   M_DIR),
    E2(0xB0, TEXT_RES,  M_BIT,   M_DIR),
    E2(0xB8, TEXT_SET,  M_BIT,   M_DIR),
    E1(0xC8, TEXT_JR,   M_REL8),
    E2(0xC0, TEXT_JR,   M_CC,    M_REL8),
    E0(0xFF, TEXT_SWI),
};

static constexpr uint8_t INDEX_TLCS90[] PROGMEM = {
     43,  // TEXT_ADC
     51,  // TEXT_ADC
     59,  // TEXT_ADC
     67,  // TEXT_ADC
     19,  // TEXT_ADD
     42,  // TEXT_ADD
     50,  // TEXT_ADD
     58,  // TEXT_ADD
     66,  // TEXT_ADD
     46,  // TEXT_AND
     54,  // TEXT_AND
     62,  // TEXT_AND
     70,  // TEXT_AND
     90,  // TEXT_BIT
     24,  // TEXT_CALL
     25,  // TEXT_CALR
     12,  // TEXT_CCF
     49,  // TEXT_CP
     57,  // TEXT_CP
     65,  // TEXT_CP
     73,  // TEXT_CP
     14,  // TEXT_CPL
      9,  // TEXT_DAA
     76,  // TEXT_DEC
     77,  // TEXT_DEC
     81,  // TEXT_DEC
     80,  // TEXT_DECW
     13,  // TEXT_DECX
      2,  // TEXT_DI
     17,  // TEXT_DIV
     20,  // TEXT_DJNZ
     21,  // TEXT_DJNZ
      3,  // TEXT_EI
      5,  // TEXT_EX
      6,  // TEXT_EX
      7,  // TEXT_EX
      8,  // TEXT_EXX
      1,  // TEXT_HALT
     74,  // TEXT_INC
     75,  // TEXT_INC
     79,  // TEXT_INC
     78,  // TEXT_INCW
      4,  // TEXT_INCX
     22,  // TEXT_JP
     93,  // TEXT_JR
     94,  // TEXT_JR
     23,  // TEXT_JRL
     28,  // TEXT_LD
     29,  // TEXT_LD
     30,  // TEXT_LD
     31,  // TEXT_LD
     32,  // TEXT_LD
     33,  // TEXT_LD
     35,  // TEXT_LD
     36,  // TEXT_LD
     37,  // TEXT_LD
     38,  // TEXT_LD
     39,  // TEXT_LD
     18,  // TEXT_LDAR
     34,  // TEXT_LDW
     16,  // TEXT_MUL
     15,  // TEXT_NEG
      0,  // TEXT_NOP
     48,  // TEXT_OR
     56,  // TEXT_OR
     64,  // TEXT_OR
     72,  // TEXT_OR
     41,  // TEXT_POP
     40,  // TEXT_PUSH
     10,  // TEXT_RCF
     91,  // TEXT_RES
     26,  // TEXT_RET
     27,  // TEXT_RETI
     84,  // TEXT_RLA
     82,  // TEXT_RLCA
     85,  // TEXT_RRA
     83,  // TEXT_RRCA
     45,  // TEXT_SBC
     53,  // TEXT_SBC
     61,  // TEXT_SBC
     69,  // TEXT_SBC
     11,  // TEXT_SCF
     92,  // TEXT_SET
     86,  // TEXT_SLAA
     88,  // TEXT_SLLA
     87,  // TEXT_SRAA
     89,  // TEXT_SRLA
     44,  // TEXT_SUB
     52,  // TEXT_SUB
     60,  // TEXT_SUB
     68,  // TEXT_SUB
     95,  // TEXT_SWI
     47,  // TEXT_XOR
     55,  // TEXT_XOR
     63,  // TEXT_XOR
     71,  // TEXT_XOR
};

static constexpr Entry TABLE_SRC[] PROGMEM = {
    E1(0x10, TEXT_RLD,  M_DST),
    E1(0x11, TEXT_RRD,  M_DST),
    E2(0x12, TEXT_MUL,  R_HL,    M_SRC),
    E2(0x13, TEXT_DIV,  R_HL,    M_SRC),
    E2(0x14, TEXT_ADD,  M_REGIX, M_SRC),
    E2(0x18, TEXT_TSET, M_BIT,   M_SRC),
    E2(0x28, TEXT_LD,   M_REG8,  M_SRC),
    E2(0x48, TEXT_LD,   M_REG16, M_SRC),
    E2(0x50, TEXT_EX,   M_DST,   M_REG16),
    E2(0x50, TEXT_EX,   M_REG16, M_SRC),
    E2(0x60, TEXT_ADD,  R_A,     M_SRC),
    E2(0x61, TEXT_ADC,  R_A,     M_SRC),
    E2(0x62, TEXT_SUB,  R_A,     M_SRC),
    E2(0x63, TEXT_SBC,  R_A,     M_SRC),
    E2(0x64, TEXT_AND,  R_A,     M_SRC),
    E2(0x65, TEXT_XOR,  R_A,     M_SRC),
    E2(0x66, TEXT_OR,   R_A,     M_SRC),
    E2(0x67, TEXT_CP,   R_A,     M_SRC),
    E2(0x70, TEXT_ADD,  R_HL,    M_SRC),
    E2(0x71, TEXT_ADC,  R_HL,    M_SRC),
    E2(0x72, TEXT_SUB,  R_HL,    M_SRC),
    E2(0x73, TEXT_SBC,  R_HL,    M_SRC),
    E2(0x74, TEXT_AND,  R_HL,    M_SRC),
    E2(0x75, TEXT_XOR,  R_HL,    M_SRC),
    E2(0x76, TEXT_OR,   R_HL,    M_SRC),
    E2(0x77, TEXT_CP,   R_HL,    M_SRC),
    E1(0x87, TEXT_INC,  M_DST),
    E1(0x8F, TEXT_DEC,  M_DST),
    E1(0x97, TEXT_INCW, M_DST),
    E1(0x9F, TEXT_DECW, M_DST),
    E1(0xA0, TEXT_RLC,  M_DST),
    E1(0xA1, TEXT_RRC,  M_DST),
    E1(0xA2, TEXT_RL,   M_DST),
    E1(0xA3, TEXT_RR,   M_DST),
    E1(0xA4, TEXT_SLA,  M_DST),
    E1(0xA5, TEXT_SRA,  M_DST),
    E1(0xA6, TEXT_SLL,  M_DST),
    E1(0xA7, TEXT_SRL,  M_DST),
    E2(0xA8, TEXT_BIT,  M_BIT,   M_SRC),
    E2(0xB0, TEXT_RES,  M_BIT,   M_SRC),
    E2(0xB8, TEXT_SET,  M_BIT,   M_SRC),
};

static constexpr uint8_t INDEX_SRC[] PROGMEM = {
     11,  // TEXT_ADC
     19,  // TEXT_ADC
      4,  // TEXT_ADD
     10,  // TEXT_ADD
     18,  // TEXT_ADD
     14,  // TEXT_AND
     22,  // TEXT_AND
     38,  // TEXT_BIT
     17,  // TEXT_CP
     25,  // TEXT_CP
     27,  // TEXT_DEC
     29,  // TEXT_DECW
      3,  // TEXT_DIV
      8,  // TEXT_EX
      9,  // TEXT_EX
     26,  // TEXT_INC
     28,  // TEXT_INCW
      6,  // TEXT_LD
      7,  // TEXT_LD
      2,  // TEXT_MUL
     16,  // TEXT_OR
     24,  // TEXT_OR
     39,  // TEXT_RES
     32,  // TEXT_RL
     30,  // TEXT_RLC
      0,  // TEXT_RLD
     33,  // TEXT_RR
     31,  // TEXT_RRC
      1,  // TEXT_RRD
     13,  // TEXT_SBC
     21,  // TEXT_SBC
     40,  // TEXT_SET
     34,  // TEXT_SLA
     36,  // TEXT_SLL
     35,  // TEXT_SRA
     37,  // TEXT_SRL
     12,  // TEXT_SUB
     20,  // TEXT_SUB
      5,  // TEXT_TSET
     15,  // TEXT_XOR
     23,  // TEXT_XOR
};

static constexpr Entry TABLE_DST[] PROGMEM = {
    E2(0x20, TEXT_LD,   M_DST,   M_REG8),
    E2(0x37, TEXT_LD,   M_DST,   M_IMM8),
    E2(0x3F, TEXT_LDW,  M_DST,   M_IMM16),
    E2(0x40, TEXT_LD,   M_DST,   M_REG16),
    E2(0x68, TEXT_ADD,  M_DST,   M_IMM8),
    E2(0x69, TEXT_ADC,  M_DST,   M_IMM8),
    E2(0x6A, TEXT_SUB,  M_DST,   M_IMM8),
    E2(0x6B, TEXT_SBC,  M_DST,   M_IMM8),
    E2(0x6C, TEXT_AND,  M_DST,   M_IMM8),
    E2(0x6D, TEXT_XOR,  M_DST,   M_IMM8),
    E2(0x6E, TEXT_OR,   M_DST,   M_IMM8),
    E2(0x6F, TEXT_CP,   M_DST,   M_IMM8),
};

static constexpr uint8_t INDEX_DST[] PROGMEM = {
      5,  // TEXT_ADC
      4,  // TEXT_ADD
      8,  // TEXT_AND
     11,  // TEXT_CP
      0,  // TEXT_LD
      1,  // TEXT_LD
      3,  // TEXT_LD
      2,  // TEXT_LDW
     10,  // TEXT_OR
      7,  // TEXT_SBC
      6,  // TEXT_SUB
      9,  // TEXT_XOR
};

static constexpr Entry TABLE_REG[] PROGMEM = {
    E2(0x12, TEXT_MUL,  R_HL,    M_SRC),
    E2(0x13, TEXT_DIV,  R_HL,    M_SRC),
    E2(0x18, TEXT_TSET, M_BIT,   M_SRC),
    E2(0x30, TEXT_LD,   M_REG8,  M_SRC),
    E2(0x60, TEXT_ADD,  R_A,     M_SRC),
    E2(0x61, TEXT_ADC,  R_A,     M_SRC),
    E2(0x62, TEXT_SUB,  R_A,     M_SRC),
    E2(0x63, TEXT_SBC,  R_A,     M_SRC),
    E2(0x64, TEXT_AND,  R_A,     M_SRC),
    E2(0x65, TEXT_XOR,  R_A,     M_SRC),
    E2(0x66, TEXT_OR,   R_A,     M_SRC),
    E2(0x67, TEXT_CP,   R_A,     M_SRC),
    E2(0x68, TEXT_ADD,  M_DST,   M_IMM8),
    E2(0x69, TEXT_ADC,  M_DST,   M_IMM8),
    E2(0x6A, TEXT_SUB,  M_DST,   M_IMM8),
    E2(0x6B, TEXT_SBC,  M_DST,   M_IMM8),
    E2(0x6C, TEXT_AND,  M_DST,   M_IMM8),
    E2(0x6D, TEXT_XOR,  M_DST,   M_IMM8),
    E2(0x6E, TEXT_OR,   M_DST,   M_IMM8),
    E2(0x6F, TEXT_CP,   M_DST,   M_IMM8),
    E1(0xA0, TEXT_RLC,  M_DST),
    E1(0xA1, TEXT_RRC,  M_DST),
    E1(0xA2, TEXT_RL,   M_DST),
    E1(0xA3, TEXT_RR,   M_DST),
    E1(0xA4, TEXT_SLA,  M_DST),
    E1(0xA5, TEXT_SRA,  M_DST),
    E1(0xA6, TEXT_SLL,  M_DST),
    E1(0xA7, TEXT_SRL,  M_DST),
    E2(0xA8, TEXT_BIT,  M_BIT,   M_SRC),
    E2(0xB0, TEXT_RES,  M_BIT,   M_SRC),
    E2(0xB8, TEXT_SET,  M_BIT,   M_SRC),
    E2(0x14, TEXT_ADD,  M_REGIX, M_SRC16),
    E2(0x38, TEXT_LD,   M_REG16, M_SRC16),
    E2(0x70, TEXT_ADD,  R_HL,    M_SRC16),
    E2(0x71, TEXT_ADC,  R_HL,    M_SRC16),
    E2(0x72, TEXT_SUB,  R_HL,    M_SRC16),
    E2(0x73, TEXT_SBC,  R_HL,    M_SRC16),
    E2(0x74, TEXT_AND,  R_HL,    M_SRC16),
    E2(0x75, TEXT_XOR,  R_HL,    M_SRC16),
    E2(0x76, TEXT_OR,   R_HL,    M_SRC16),
    E2(0x77, TEXT_CP,   R_HL,    M_SRC16),
};

static constexpr uint8_t INDEX_REG[] PROGMEM = {
      5,  // TEXT_ADC
     13,  // TEXT_ADC
     34,  // TEXT_ADC
      4,  // TEXT_ADD
     12,  // TEXT_ADD
     31,  // TEXT_ADD
     33,  // TEXT_ADD
      8,  // TEXT_AND
     16,  // TEXT_AND
     37,  // TEXT_AND
     28,  // TEXT_BIT
     11,  // TEXT_CP
     19,  // TEXT_CP
     40,  // TEXT_CP
      1,  // TEXT_DIV
      3,  // TEXT_LD
     32,  // TEXT_LD
      0,  // TEXT_MUL
     10,  // TEXT_OR
     18,  // TEXT_OR
     39,  // TEXT_OR
     29,  // TEXT_RES
     22,  // TEXT_RL
     20,  // TEXT_RLC
     23,  // TEXT_RR
     21,  // TEXT_RRC
      7,  // TEXT_SBC
     15,  // TEXT_SBC
     36,  // TEXT_SBC
     30,  // TEXT_SET
     24,  // TEXT_SLA
     26,  // TEXT_SLL
     25,  // TEXT_SRA
     27,  // TEXT_SRL
      6,  // TEXT_SUB
     14,  // TEXT_SUB
     35,  // TEXT_SUB
      2,  // TEXT_TSET
      9,  // TEXT_XOR
     17,  // TEXT_XOR
     38,  // TEXT_XOR
};

static constexpr Entry TABLE_LDA[] PROGMEM = {
    E2(0x38, TEXT_LDA,  M_REG16, M_SRC),
};


static constexpr uint8_t INDEX_LDA[] PROGMEM = {
      0,  // TEXT_LDA
};

static constexpr Entry TABLE_JMP[] PROGMEM = {
    E2(0xC0, TEXT_JP,   M_CC,    M_SRC),
    E1(0xC8, TEXT_JP,   M_DST),
    E2(0xD0, TEXT_CALL, M_CC,    M_SRC),
    E1(0xD8, TEXT_CALL, M_DST),
};

static constexpr uint8_t INDEX_JMP[] PROGMEM = {
      2,  // TEXT_CALL
      3,  // TEXT_CALL
      0,  // TEXT_JP
      1,  // TEXT_JP
};

static constexpr Entry TABLE_COND[] PROGMEM = {
    E0(0xD8, TEXT_RET),
    E1(0xD0, TEXT_RET,  M_CC),
};

static constexpr uint8_t INDEX_COND[] PROGMEM = {
      0,  // TEXT_RET
      1,  // TEXT_RET
};

static constexpr Entry TABLE_BLOCK[] PROGMEM = {
    E0(0x58, TEXT_LDI),
    E0(0x59, TEXT_LDIR),
    E0(0x5A, TEXT_LDD),
    E0(0x5B, TEXT_LDDR),
    E0(0x5C, TEXT_CPI),
    E0(0x5D, TEXT_CPIR),
    E0(0x5E, TEXT_CPD),
    E0(0x5F, TEXT_CPDR),
};

static constexpr uint8_t INDEX_BLOCK[] PROGMEM = {
      6,  // TEXT_CPD
      7,  // TEXT_CPDR
      4,  // TEXT_CPI
      5,  // TEXT_CPIR
      2,  // TEXT_LDD
      3,  // TEXT_LDDR
      0,  // TEXT_LDI
      1,  // TEXT_LDIR
};
// clang-format on

struct EntryPage : entry::PrefixTableBase<Entry> {
    AddrMode prefixMode() const { return AddrMode(pgm_read_byte(&_mode)); }
    bool prefixMatcher(Config::opcode_t code) const {
        const auto pre = prefix();
        const auto reg = code & 7;
        switch (prefixMode()) {
        case M_IND:
            return (code & ~7) == pre && reg != 3 && reg != 7;
        case M_IDX:
            return (code & ~3) == pre && reg != 3;
        case M_REG8:
            return (code & ~7) == pre && reg != 7;
        case M_CC:
            return (code & ~0xF) == pre;
        default:
            return code == pre;
        }
    }

    constexpr EntryPage(Config::opcode_t prefix, AddrMode mode, const Entry *table,
            const Entry *end, const uint8_t *index, const uint8_t *iend)
        : PrefixTableBase(prefix, table, end, index, iend), _mode(mode) {}

private:
    const AddrMode _mode;
};

// clang-format off
static constexpr EntryPage TLCS90_PAGES[] PROGMEM = {
    {0x00, M_NONE,  ARRAY_RANGE(TABLE_TLCS90), ARRAY_RANGE(INDEX_TLCS90)},
    {0xE7, M_DIR,   ARRAY_RANGE(TABLE_SRC),    ARRAY_RANGE(INDEX_SRC)}, // src (FFnn)
    {0xE3, M_EXT,   ARRAY_RANGE(TABLE_SRC),    ARRAY_RANGE(INDEX_SRC)}, // src (nnnn)
    {0xE0, M_IND,   ARRAY_RANGE(TABLE_SRC),    ARRAY_RANGE(INDEX_SRC)}, // src (rr)
    {0xF3, M_BASE,  ARRAY_RANGE(TABLE_SRC),    ARRAY_RANGE(INDEX_SRC)}, // src (HL+A)
    {0xF0, M_IDX,   ARRAY_RANGE(TABLE_SRC),    ARRAY_RANGE(INDEX_SRC)}, // src (ix+d)
    {0xEF, M_DIR,   ARRAY_RANGE(TABLE_DST),    ARRAY_RANGE(INDEX_DST)}, // dst (FFnn)
    {0xEB, M_EXT,   ARRAY_RANGE(TABLE_DST),    ARRAY_RANGE(INDEX_DST)}, // dst (nnnn)
    {0xE8, M_IND,   ARRAY_RANGE(TABLE_DST),    ARRAY_RANGE(INDEX_DST)}, // dst (rr)
    {0xF7, M_BASE,  ARRAY_RANGE(TABLE_DST),    ARRAY_RANGE(INDEX_DST)}, // dst (HL+A)
    {0xF4, M_IDX,   ARRAY_RANGE(TABLE_DST),    ARRAY_RANGE(INDEX_DST)}, // dst (ix+d)
    {0xEB, M_EXT,   ARRAY_RANGE(TABLE_JMP),    ARRAY_RANGE(INDEX_JMP)}, // JP/CALL
    {0xE8, M_IND,   ARRAY_RANGE(TABLE_JMP),    ARRAY_RANGE(INDEX_JMP)}, // JP/CALL
    {0xF7, M_BASE,  ARRAY_RANGE(TABLE_JMP),    ARRAY_RANGE(INDEX_JMP)}, // JP/CALL
    {0xF4, M_IDX,   ARRAY_RANGE(TABLE_JMP),    ARRAY_RANGE(INDEX_JMP)}, // JP/CALL
    {0xF8, M_REG8,  ARRAY_RANGE(TABLE_REG),    ARRAY_RANGE(INDEX_REG)}, // r/rr
    {0xF7, M_BASE,  ARRAY_RANGE(TABLE_LDA),    ARRAY_RANGE(INDEX_LDA)}, // LDA
    {0xF4, M_IDX,   ARRAY_RANGE(TABLE_LDA),    ARRAY_RANGE(INDEX_LDA)}, // LDA
    {0xFE, M_NONE,  ARRAY_RANGE(TABLE_COND),   ARRAY_RANGE(INDEX_COND)},
    {0xFE, M_NONE,  ARRAY_RANGE(TABLE_BLOCK),  ARRAY_RANGE(INDEX_BLOCK)},
};
// clang-format on

struct Cpu : entry::CpuBase<CpuType, EntryPage> {
    constexpr Cpu(CpuType cpuType, const /*PROGMEM*/ char *name_P, const EntryPage *table,
            const EntryPage *end)
        : CpuBase(cpuType, name_P, table, end) {}

    bool isPrefix(Config::opcode_t code, AddrMode &prefixMode) const {
        for (auto page = _pages.table(); page < _pages.end(); page++) {
            if (page->prefix() && page->prefixMatcher(code)) {
                prefixMode = page->prefixMode();
                return true;
            }
        }
        return false;
    }
};

static constexpr Cpu CPU_TABLE[] PROGMEM = {
        {TLCS90, TEXT_CPU_TLCS90, ARRAY_RANGE(TLCS90_PAGES)},
};

static const Cpu *cpu(CpuType cpuType) {
    UNUSED(cpuType);
    return &CPU_TABLE[0];
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
        return table == M_IMM8 || table == M_BIT || table == M_REL8 || table == M_REL16;
    if (opr == M_DIR)
        return table == M_EXT;
    if (opr == M_SYM)
        return table == M_EXT || table == M_DIR;
    if (opr == R_AF)
        return table == M_STACK;
    return false;
}

static void pageSetup(AsmInsn &insn, const EntryPage *page) {
    insn.setPrefixMode(page->prefixMode());
}

static bool acceptModes(AsmInsn &insn, const Entry *entry) {
    const auto table = entry->flags();
    const auto tableDst = table.dst();
    const auto tableSrc = table.src();
    const auto dst = (tableDst == M_DST) ? insn.pre() : tableDst;
    const auto src = (tableSrc == M_SRC) ? insn.pre() : (tableSrc == M_SRC16 ? M_REG16 : tableSrc);
    return acceptMode(insn.dstOp.mode, dst) && acceptMode(insn.srcOp.mode, src);
}

static void readCode(AsmInsn &insn, const Entry *entry, const EntryPage *page) {
    Cpu::defaultReadCode(insn, entry, page);

    // Update prefix mode.
    auto tableDst = entry->flags().dst();
    auto tableSrc = entry->flags().src();
    auto dst = (tableDst == M_DST) ? insn.pre() : tableDst;
    auto src = (tableSrc == M_SRC) ? insn.pre() : (tableSrc == M_SRC16 ? M_REG16 : tableSrc);
    insn.setAddrMode(dst, src);
    if (tableDst == M_DST) {
        insn.setPrefixMode(M_DST);
    } else if (tableSrc == M_SRC || tableSrc == M_SRC16) {
        insn.setPrefixMode(M_SRC);
    } else {
        insn.setPrefixMode(M_NONE);
    }
}

Error TableTlcs90::searchName(CpuType cpuType, AsmInsn &insn) const {
    cpu(cpuType)->searchName(insn, acceptModes, pageSetup, readCode);
    return insn.getError();
}

static bool prefixMatcher(DisInsn &insn, const EntryPage *page) {
    return page->prefixMatcher(insn.prefix());
}

static bool invalidPrefixCode(Config::opcode_t prefix, AddrMode mode) {
    const auto reg = prefix & 7;
    if (mode == M_REG8 && reg == 7)
        return true;  // accept B,C,D,E,H,L,A only
    if ((mode == M_REG16 || mode == M_STACK) && (reg == 3 || reg == 7))
        return true;  // accept BC,DE,HL,IX,IY,SP/AF only
    if (mode == M_REGIX && (prefix & 3) == 3)
        return true;  // accept IX,IY,SP only
    return false;
}

static bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *page) {
    UNUSED(page);
    auto opc = insn.opCode();
    const auto flags = entry->flags();
    const auto dst = flags.dst();
    const auto src = flags.src();
    if (invalidPrefixCode(opc, dst) || invalidPrefixCode(opc, src))
        return false;
    if (dst == M_REG8 || src == M_REG8 || dst == M_REG16 || src == M_REG16 || dst == M_BIT ||
            dst == M_STACK) {
        opc &= ~7;
    } else if (dst == M_CC) {
        opc &= ~0xF;
    } else if (dst == M_REGIX) {
        opc &= ~3;
    }
    return opc == entry->opCode();
}

Error TableTlcs90::searchOpCode(
        CpuType cpuType, DisInsn &insn, Operand &prefixOp, StrBuffer &out) const {
    cpu(cpuType)->searchOpCode(insn, out, matchOpCode, prefixMatcher);
    // Resolve shared prefix for M_REG16 with M_REG8 here.
    if (insn.src() == M_SRC16) {
        if (invalidPrefixCode(insn.prefix(), M_REG16))
            insn.setError(UNKNOWN_INSTRUCTION);
        insn.setAddrMode(insn.dst(), M_SRC);
        prefixOp.mode = M_REG16;
        prefixOp.reg = decodeReg16(insn.prefix());
    }
    if (insn.getError() == UNKNOWN_INSTRUCTION)
        insn.nameBuffer().reset();
    return insn.getError();
}

bool TableTlcs90::isPrefix(CpuType cpuType, Config::opcode_t code, AddrMode &mode) const {
    return cpu(cpuType)->isPrefix(code, mode);
}

const /*PROGMEM*/ char *TableTlcs90::listCpu_P() const {
    return TEXT_CPU_TLCS90;
}

const /*PROGMEM*/ char *TableTlcs90::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableTlcs90::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    if (name.iequals(TEXT_CPU_TLCS90)) {
        cpuType = TLCS90;
        return OK;
    }
    return UNSUPPORTED_CPU;
}

const TableTlcs90 TABLE;

}  // namespace tlcs90
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

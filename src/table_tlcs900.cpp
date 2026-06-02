/*
 * Copyright 2026 Tadashi G. Takaoka
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

#include "table_tlcs900.h"
#include "entry_table.h"
#include "entry_tlcs900.h"
#include "insn_tlcs900.h"
#include "reg_tlcs900.h"
#include "text_tlcs900.h"

using namespace libasm::text::tlcs900;
using namespace libasm::tlcs900::reg;

namespace libasm {
namespace tlcs900 {

#define E2(_opc, _cf, _name, _dst, _src) {_opc, Entry::Flags::create(_cf, _dst, _src), _name}
#define E1(_opc, _cf, _name, _dst) E2(_opc, _cf, _name, _dst, M_NONE)
#define E0(_opc, _cf, _name) E1(_opc, _cf, _name, M_NONE)

// clang-format off

constexpr Entry TABLE_OPC[] PROGMEM = {
    E0(0x00, CF_00, TEXT_NOP),
    E1(0x02, CF_00, TEXT_PUSH,  R_SR),
    E1(0x03, CF_00, TEXT_POP,   R_SR),
    E0(0x05, CF_00, TEXT_HALT),
    E1(0x06, CF_00, TEXT_EI,    M_INTLVL),
    E0(0x07, CF_00, TEXT_RETI),
    E2(0x08, CF_00, TEXT_LD,    M_ABS8,  M_IMM8),
    E1(0x09, CF_00, TEXT_PUSH,  M_IMM8),
    E2(0x0A, CF_00, TEXT_LDW,   M_ABS8,  M_IMM16),
    E1(0x0B, CF_00, TEXT_PUSHW, M_IMM16),
    E0(0x0C, CF_00, TEXT_INCF),
    E0(0x0D, CF_00, TEXT_DECF),
    E0(0x0E, CF_00, TEXT_RET),
    E1(0x0F, CF_00, TEXT_RETD,  M_IMM16),
    E0(0x10, CF_00, TEXT_RCF),
    E0(0x11, CF_00, TEXT_SCF),
    E0(0x12, CF_00, TEXT_CCF),
    E0(0x13, CF_00, TEXT_ZCF),
    E1(0x14, CF_00, TEXT_PUSH,  R_A),
    E1(0x15, CF_00, TEXT_POP,   R_A),
    E2(0x16, CF_00, TEXT_EX,    R_F,     R_FP),
    E1(0x17, CF_00, TEXT_LDF,   M_LDF),
    E1(0x18, CF_00, TEXT_PUSH,  R_F),
    E1(0x19, CF_00, TEXT_POP,   R_F),
    E1(0x1A, CF_00, TEXT_JP,    M_ABS16),
    E1(0x1B, CF_00, TEXT_JP,    M_ABS24),
    E1(0x1C, CF_00, TEXT_CALL,  M_ABS16),
    E1(0x1D, CF_00, TEXT_CALL,  M_ABS24),
    E1(0x1E, CF_00, TEXT_CALR,  M_REL16),
    E2(0x20, CF_07, TEXT_LD,    M_REG8,  M_IMM8),
    E1(0x28, CF_07, TEXT_PUSH,  M_REG16),
    E2(0x30, CF_07, TEXT_LD,    M_REG16, M_IMM16),
    E1(0x38, CF_07, TEXT_PUSH,  M_REG32),
    E2(0x40, CF_07, TEXT_LD,    M_REG32, M_IMM32),
    E1(0x48, CF_07, TEXT_POP,   M_REG16),
    E1(0x58, CF_07, TEXT_POP,   M_REG32),
    E2(0x60, CF_0F, TEXT_JR,    M_CC,    M_REL8),
    E2(0x70, CF_0F, TEXT_JRL,   M_CC,    M_REL16),
    E1(0xF8, CF_07, TEXT_SWI,   M_SWI),
    // Bare-operand aliases: placed after the operand-bearing entries so dis still
    // matches the explicit-operand form first (first table match wins).
    E0(0xFF, CF_00, TEXT_SWI),
    E1(0x06, CF_00, TEXT_EI,    M_ZERO),
    E1(0x06, CF_00, TEXT_DI,    M_DISUF),
};
constexpr uint8_t INDEX_OPC[] PROGMEM = {
     26,  // TEXT_CALL
     27,  // TEXT_CALL
     28,  // TEXT_CALR
     16,  // TEXT_CCF
     11,  // TEXT_DECF
     41,  // TEXT_DI
      4,  // TEXT_EI
     40,  // TEXT_EI
     20,  // TEXT_EX
      3,  // TEXT_HALT
     10,  // TEXT_INCF
     24,  // TEXT_JP
     25,  // TEXT_JP
     36,  // TEXT_JR
     37,  // TEXT_JRL
      6,  // TEXT_LD
     29,  // TEXT_LD
     31,  // TEXT_LD
     33,  // TEXT_LD
     21,  // TEXT_LDF
      8,  // TEXT_LDW
      0,  // TEXT_NOP
      2,  // TEXT_POP
     19,  // TEXT_POP
     23,  // TEXT_POP
     34,  // TEXT_POP
     35,  // TEXT_POP
      1,  // TEXT_PUSH
      7,  // TEXT_PUSH
     18,  // TEXT_PUSH
     22,  // TEXT_PUSH
     30,  // TEXT_PUSH
     32,  // TEXT_PUSH
      9,  // TEXT_PUSHW
     14,  // TEXT_RCF
     12,  // TEXT_RET
     13,  // TEXT_RETD
      5,  // TEXT_RETI
     15,  // TEXT_SCF
     38,  // TEXT_SWI
     39,  // TEXT_SWI
     17,  // TEXT_ZCF
};

constexpr Entry TABLE_OPC_TLCS900[] PROGMEM = {
        E0(0x01, CF_00, TEXT_NORMAL),
        E0(0x04, CF_00, TEXT_MAX),
};
constexpr uint8_t INDEX_OPC_TLCS900[] PROGMEM = {
        1,  // TEXT_MAX
        0,  // TEXT_NORMAL
};

constexpr Entry TABLE_OPC_TLCS900L[] PROGMEM = {
    E0(0x04, CF_00, TEXT_MIN),
};
constexpr uint8_t INDEX_OPC_TLCS900L[] PROGMEM = {
      0,  // TEXT_MIN
};

constexpr Entry TABLE_SRC_REG[] PROGMEM = {
    E1(0x06, CF_00, TEXT_CPL,   M_SRC),
    E1(0x07, CF_00, TEXT_NEG,   M_SRC),
    E2(0x08, CF_00, TEXT_MUL,   R_WA,    M_IMM8),
    E2(0x09, CF_00, TEXT_MULS,  R_WA,    M_IMM8),
    E2(0x0C, CF_00, TEXT_LINK,  M_SRC,   M_IMM16),
    E1(0x0D, CF_00, TEXT_UNLK,  M_SRC),
    E2(0x0E, CF_00, TEXT_BS1F,  R_A,     M_SRC),
    E2(0x0F, CF_00, TEXT_BS1B,  R_A,     M_SRC),
    E1(0x10, CF_00, TEXT_DAA,   M_SRC),
    E1(0x12, CF_00, TEXT_EXTZ,  M_SRC),
    E1(0x13, CF_00, TEXT_EXTS,  M_SRC),
    E1(0x14, CF_00, TEXT_PAA,   M_SRC),
    E1(0x16, CF_00, TEXT_MIRR,  M_SRC),
    E1(0x19, CF_00, TEXT_MULA,  M_SRC),
    E1(0x19, CF_00, TEXT_MULA,  M_R32SRC),
    E2(0x1C, CF_00, TEXT_DJNZ,  M_SRC,   M_REL8),
    E2(0x20, CF_00, TEXT_ANDCF, M_BIT,   M_SRC),
    E2(0x21, CF_00, TEXT_ORCF,  M_BIT,   M_SRC),
    E2(0x22, CF_00, TEXT_XORCF, M_BIT,   M_SRC),
    E2(0x23, CF_00, TEXT_LDCF,  M_BIT,   M_SRC),
    E2(0x24, CF_00, TEXT_STCF,  M_BIT,   M_SRC),
    E2(0x2E, CF_00, TEXT_LDC,   M_CREG,  M_SRC),
    E2(0x2F, CF_00, TEXT_LDC,   M_SRC,   M_CREG),
    E2(0x30, CF_00, TEXT_RES,   M_BIT,   M_SRC),
    E2(0x31, CF_00, TEXT_SET,   M_BIT,   M_SRC),
    E2(0x32, CF_00, TEXT_CHG,   M_BIT,   M_SRC),
    E2(0x33, CF_00, TEXT_BIT,   M_BIT,   M_SRC),
    E2(0x34, CF_00, TEXT_TSET,  M_BIT,   M_SRC),
    E2(0x41, CF_00, TEXT_MUL,   R_WA,    M_SRC),
    E2(0x49, CF_00, TEXT_MULS,  R_WA,    M_SRC),
    E2(0x51, CF_00, TEXT_DIV,   R_WA,    M_SRC),
    E2(0x59, CF_00, TEXT_DIVS,  R_WA,    M_SRC),
    E2(0x61, CF_00, TEXT_INC,   M_STEP1, M_SRC),
    E2(0x62, CF_00, TEXT_INC,   M_STEP2, M_SRC),
    E2(0x64, CF_00, TEXT_INC,   M_STEP4, M_SRC),
    E2(0x69, CF_00, TEXT_DEC,   M_STEP1, M_SRC),
    E2(0x6A, CF_00, TEXT_DEC,   M_STEP2, M_SRC),
    E2(0x6C, CF_00, TEXT_DEC,   M_STEP4, M_SRC),
    E2(0x70, CF_0F, TEXT_SCC,   M_CC,    M_SRC),
    E2(0x80, CF_07, TEXT_ADD,   M_DST,   M_SRC),
    E2(0x88, CF_07, TEXT_LD,    M_DST,   M_SRC),
    E2(0x90, CF_07, TEXT_ADC,   M_DST,   M_SRC),
    E2(0xA0, CF_07, TEXT_SUB,   M_DST,   M_SRC),
    E2(0xB0, CF_07, TEXT_SBC,   M_DST,   M_SRC),
    E2(0xB8, CF_07, TEXT_EX,    M_DST,   M_SRC),
    E2(0xC0, CF_07, TEXT_AND,   M_DST,   M_SRC),
    E2(0xC8, CF_00, TEXT_ADD,   M_SRC,   M_IMMX),
    E2(0xC9, CF_00, TEXT_ADC,   M_SRC,   M_IMMX),
    E2(0xCA, CF_00, TEXT_SUB,   M_SRC,   M_IMMX),
    E2(0xCB, CF_00, TEXT_SBC,   M_SRC,   M_IMMX),
    E2(0xCC, CF_00, TEXT_AND,   M_SRC,   M_IMMX),
    E2(0xCD, CF_00, TEXT_XOR,   M_SRC,   M_IMMX),
    E2(0xCE, CF_00, TEXT_OR,    M_SRC,   M_IMMX),
    E2(0xCF, CF_00, TEXT_CP,    M_SRC,   M_IMMX),
    E2(0xD0, CF_07, TEXT_XOR,   M_DST,   M_SRC),
    E2(0xE0, CF_07, TEXT_OR,    M_DST,   M_SRC),
    E2(0xE8, CF_00, TEXT_RLC,   M_RCOUNT, M_SRC),
    E2(0xE9, CF_00, TEXT_RRC,   M_RCOUNT, M_SRC),
    E2(0xEA, CF_00, TEXT_RL,    M_RCOUNT, M_SRC),
    E2(0xEB, CF_00, TEXT_RR,    M_RCOUNT, M_SRC),
    E2(0xEC, CF_00, TEXT_SLA,   M_RCOUNT, M_SRC),
    E2(0xED, CF_00, TEXT_SRA,   M_RCOUNT, M_SRC),
    E2(0xEE, CF_00, TEXT_SLL,   M_RCOUNT, M_SRC),
    E2(0xEF, CF_00, TEXT_SRL,   M_RCOUNT, M_SRC),
    E2(0xF0, CF_07, TEXT_CP,    M_DST,   M_SRC),
    E2(0x38, CF_00, TEXT_MINC1, M_BUF,   M_SRC),
    E2(0x39, CF_00, TEXT_MINC2, M_BUF,   M_SRC),
    E2(0x3A, CF_00, TEXT_MINC4, M_BUF,   M_SRC),
    E2(0x3C, CF_00, TEXT_MDEC1, M_BUF,   M_SRC),
    E2(0x3D, CF_00, TEXT_MDEC2, M_BUF,   M_SRC),
    E2(0x3E, CF_00, TEXT_MDEC4, M_BUF,   M_SRC),
};
constexpr uint8_t INDEX_SRC_REG[] PROGMEM = {
     41,  // TEXT_ADC
     47,  // TEXT_ADC
     39,  // TEXT_ADD
     46,  // TEXT_ADD
     45,  // TEXT_AND
     50,  // TEXT_AND
     16,  // TEXT_ANDCF
     26,  // TEXT_BIT
      7,  // TEXT_BS1B
      6,  // TEXT_BS1F
     25,  // TEXT_CHG
     53,  // TEXT_CP
     64,  // TEXT_CP
      0,  // TEXT_CPL
      8,  // TEXT_DAA
     35,  // TEXT_DEC
     36,  // TEXT_DEC
     37,  // TEXT_DEC
     30,  // TEXT_DIV
     31,  // TEXT_DIVS
     15,  // TEXT_DJNZ
     44,  // TEXT_EX
     10,  // TEXT_EXTS
      9,  // TEXT_EXTZ
     32,  // TEXT_INC
     33,  // TEXT_INC
     34,  // TEXT_INC
     40,  // TEXT_LD
     21,  // TEXT_LDC
     22,  // TEXT_LDC
     19,  // TEXT_LDCF
      4,  // TEXT_LINK
     68,  // TEXT_MDEC1
     69,  // TEXT_MDEC2
     70,  // TEXT_MDEC4
     65,  // TEXT_MINC1
     66,  // TEXT_MINC2
     67,  // TEXT_MINC4
     12,  // TEXT_MIRR
      2,  // TEXT_MUL
     28,  // TEXT_MUL
     13,  // TEXT_MULA
     14,  // TEXT_MULA
      3,  // TEXT_MULS
     29,  // TEXT_MULS
      1,  // TEXT_NEG
     52,  // TEXT_OR
     55,  // TEXT_OR
     17,  // TEXT_ORCF
     11,  // TEXT_PAA
     23,  // TEXT_RES
     58,  // TEXT_RL
     56,  // TEXT_RLC
     59,  // TEXT_RR
     57,  // TEXT_RRC
     43,  // TEXT_SBC
     49,  // TEXT_SBC
     38,  // TEXT_SCC
     24,  // TEXT_SET
     60,  // TEXT_SLA
     62,  // TEXT_SLL
     61,  // TEXT_SRA
     63,  // TEXT_SRL
     20,  // TEXT_STCF
     42,  // TEXT_SUB
     48,  // TEXT_SUB
     27,  // TEXT_TSET
      5,  // TEXT_UNLK
     51,  // TEXT_XOR
     54,  // TEXT_XOR
     18,  // TEXT_XORCF
};

constexpr Entry TABLE_SRC_ABREG[] PROGMEM = {
    E2(0x03, CF_00, TEXT_LD,    M_ABSDST, M_IMMX),
    E2(0x98, CF_07, TEXT_LD,    M_ABSDST, M_DST),
};
constexpr uint8_t INDEX_SRC_ABREG[] PROGMEM = {
      0,  // TEXT_LD
      1,  // TEXT_LD
};

constexpr Entry TABLE_SRC_MEM[] PROGMEM = {
    E1(0x04, CF_00, TEXT_PUSH,  M_SRC),
    E2(0x06, CF_00, TEXT_RLD,   R_A,     M_SRC),
    E2(0x07, CF_00, TEXT_RRD,   R_A,     M_SRC),
    E2(0x41, CF_00, TEXT_MUL,   R_WA,    M_SRC),
    E2(0x49, CF_00, TEXT_MULS,  R_WA,    M_SRC),
    E2(0x51, CF_00, TEXT_DIV,   R_WA,    M_SRC),
    E2(0x59, CF_00, TEXT_DIVS,  R_WA,    M_SRC),
    E2(0x61, CF_00, TEXT_INC,   M_STEP1, M_SRC),
    E2(0x62, CF_00, TEXT_INC,   M_STEP2, M_SRC),
    E2(0x64, CF_00, TEXT_INC,   M_STEP4, M_SRC),
    E2(0x69, CF_00, TEXT_DEC,   M_STEP1, M_SRC),
    E2(0x6A, CF_00, TEXT_DEC,   M_STEP2, M_SRC),
    E2(0x6C, CF_00, TEXT_DEC,   M_STEP4, M_SRC),
    E2(0x19, CF_00, TEXT_LD,    M_ABS16, M_SRC),
    E2(0x20, CF_07, TEXT_LD,    M_DST,   M_SRC),
    E2(0x80, CF_07, TEXT_ADD,   M_DST,   M_SRC),
    E2(0x90, CF_07, TEXT_ADC,   M_DST,   M_SRC),
    E2(0xA0, CF_07, TEXT_SUB,   M_DST,   M_SRC),
    E2(0xB0, CF_07, TEXT_SBC,   M_DST,   M_SRC),
    E2(0xC0, CF_07, TEXT_AND,   M_DST,   M_SRC),
    E2(0x30, CF_07, TEXT_EX,    M_DST,   M_SRC),
    E2(0x3F, CF_00, TEXT_CP,    M_SRC,   M_IMM8),
    E2(0xD0, CF_07, TEXT_XOR,   M_DST,   M_SRC),
    E2(0xE0, CF_07, TEXT_OR,    M_DST,   M_SRC),
    E2(0xE8, CF_00, TEXT_RLC,   M_RCOUNT, M_SRC),
    E2(0xE9, CF_00, TEXT_RRC,   M_RCOUNT, M_SRC),
    E2(0xEA, CF_00, TEXT_RL,    M_RCOUNT, M_SRC),
    E2(0xEB, CF_00, TEXT_RR,    M_RCOUNT, M_SRC),
    E2(0xEC, CF_00, TEXT_SLA,   M_RCOUNT, M_SRC),
    E2(0xED, CF_00, TEXT_SRA,   M_RCOUNT, M_SRC),
    E2(0xEE, CF_00, TEXT_SLL,   M_RCOUNT, M_SRC),
    E2(0xEF, CF_00, TEXT_SRL,   M_RCOUNT, M_SRC),
    E2(0xF0, CF_07, TEXT_CP,    M_DST,   M_SRC),
};
constexpr uint8_t INDEX_SRC_MEM[] PROGMEM = {
     16,  // TEXT_ADC
     15,  // TEXT_ADD
     19,  // TEXT_AND
     21,  // TEXT_CP
     32,  // TEXT_CP
     10,  // TEXT_DEC
     11,  // TEXT_DEC
     12,  // TEXT_DEC
      5,  // TEXT_DIV
      6,  // TEXT_DIVS
     20,  // TEXT_EX
      7,  // TEXT_INC
      8,  // TEXT_INC
      9,  // TEXT_INC
     13,  // TEXT_LD
     14,  // TEXT_LD
      3,  // TEXT_MUL
      4,  // TEXT_MULS
     23,  // TEXT_OR
      0,  // TEXT_PUSH
     26,  // TEXT_RL
     24,  // TEXT_RLC
      1,  // TEXT_RLD
     27,  // TEXT_RR
     25,  // TEXT_RRC
      2,  // TEXT_RRD
     18,  // TEXT_SBC
     28,  // TEXT_SLA
     30,  // TEXT_SLL
     29,  // TEXT_SRA
     31,  // TEXT_SRL
     17,  // TEXT_SUB
     22,  // TEXT_XOR
};


constexpr Entry TABLE_DST_MEM[] PROGMEM = {
    E2(0x00, CF_00, TEXT_LD,    M_DST,   M_IMM8),
    E2(0x01, CF_00, TEXT_LD,    M_DST,   M_IMM16),
    E2(0x02, CF_00, TEXT_LD,    M_DST,   M_IMM32),
    E1(0x04, CF_00, TEXT_POP,   M_DST),
    E2(0x14, CF_00, TEXT_LD,    M_DST,   M_ABS16),
    E2(0x20, CF_07, TEXT_LDA,   M_REG16, M_DST),
    E2(0x30, CF_07, TEXT_LDA,   M_REG32, M_DST),
    E2(0x40, CF_07, TEXT_LD,    M_DST,   M_REG8),
    E2(0x50, CF_07, TEXT_LD,    M_DST,   M_REG16),
    E2(0x60, CF_07, TEXT_LD,    M_DST,   M_REG32),
    E2(0x80, CF_07, TEXT_ANDCF, M_BIT,   M_DST),
    E2(0x88, CF_07, TEXT_ORCF,  M_BIT,   M_DST),
    E2(0x90, CF_07, TEXT_XORCF, M_BIT,   M_DST),
    E2(0x98, CF_07, TEXT_LDCF,  M_BIT,   M_DST),
    E2(0xA0, CF_07, TEXT_STCF,  M_BIT,   M_DST),
    E2(0xA8, CF_07, TEXT_TSET,  M_BIT,   M_DST),
    E2(0xB0, CF_07, TEXT_RES,   M_BIT,   M_DST),
    E2(0xB8, CF_07, TEXT_SET,   M_BIT,   M_DST),
    E2(0xC0, CF_07, TEXT_CHG,   M_BIT,   M_DST),
    E2(0xC8, CF_07, TEXT_BIT,   M_BIT,   M_DST),
    E2(0xD0, CF_0F, TEXT_JP,    M_CC,    M_DST),
    E2(0xE0, CF_0F, TEXT_CALL,  M_CC,    M_DST),
};
constexpr uint8_t INDEX_DST_MEM[] PROGMEM = {
     10,  // TEXT_ANDCF
     19,  // TEXT_BIT
     21,  // TEXT_CALL
     18,  // TEXT_CHG
     20,  // TEXT_JP
      0,  // TEXT_LD
      1,  // TEXT_LD
      2,  // TEXT_LD
      4,  // TEXT_LD
      7,  // TEXT_LD
      8,  // TEXT_LD
      9,  // TEXT_LD
      5,  // TEXT_LDA
      6,  // TEXT_LDA
     13,  // TEXT_LDCF
     11,  // TEXT_ORCF
      3,  // TEXT_POP
     16,  // TEXT_RES
     17,  // TEXT_SET
     14,  // TEXT_STCF
     15,  // TEXT_TSET
     12,  // TEXT_XORCF
};

constexpr Entry TABLE_LDX[] PROGMEM = {
    E2(0xF7, CF_00, TEXT_LDX,   M_LDXDST, M_IMM16),
};
constexpr uint8_t INDEX_LDX[] PROGMEM = {
      0,  // TEXT_LDX
};


constexpr Entry TABLE_BLOCK[] PROGMEM = {
    E0(0x10, CF_00, TEXT_LDI),
    E0(0x11, CF_00, TEXT_LDIR),
    E0(0x12, CF_00, TEXT_LDD),
    E0(0x13, CF_00, TEXT_LDDR),
    E0(0x14, CF_00, TEXT_CPI),
    E0(0x15, CF_00, TEXT_CPIR),
    E0(0x16, CF_00, TEXT_CPD),
    E0(0x17, CF_00, TEXT_CPDR),
};

constexpr uint8_t INDEX_BLOCK[] PROGMEM = {
      6,  // TEXT_CPD
      7,  // TEXT_CPDR
      4,  // TEXT_CPI
      5,  // TEXT_CPIR
      2,  // TEXT_LDD
      3,  // TEXT_LDDR
      0,  // TEXT_LDI
      1,  // TEXT_LDIR
};

constexpr Entry TABLE_RETCC[] PROGMEM = {
    E1(0xF0, CF_0F, TEXT_RET,   M_CC),
};

constexpr uint8_t INDEX_RETCC[] PROGMEM = {
      0,  // TEXT_RET
};

// LDAR layout: F3 13 rel16 reg_byte (rel16 between prefix and opcode).
constexpr Entry TABLE_LDAR[] PROGMEM = {
    E2(0x20, CF_07, TEXT_LDAR,  M_REG16, M_LDARREL),
    E2(0x30, CF_07, TEXT_LDAR,  M_REG32, M_LDARREL),
};

constexpr uint8_t INDEX_LDAR[] PROGMEM = {
      0,  // TEXT_LDAR
      1,  // TEXT_LDAR
};

// clang-format on

static constexpr uint16_t PM_BIT(PrefixMode pm) {
    return uint16_t(1) << pm;
}
static constexpr uint16_t PM_BITS_REG = PM_BIT(PM_REG8) | PM_BIT(PM_REG16) | PM_BIT(PM_REG32);
static constexpr uint16_t PM_BITS_ABREG =
        PM_BIT(PM_ABREG8) | PM_BIT(PM_ABREG16) | PM_BIT(PM_ABREG32);
static constexpr uint16_t PM_BITS_MEM = PM_BIT(PM_MEM8) | PM_BIT(PM_MEM16) | PM_BIT(PM_MEM32);

// EntryPage carries a PrefixMode bitmask so a single page can match multiple
// PMs (e.g. PM_REG8/16/32 share the same table) and inherits PrefixTableBase
// for an optional fixed prefix byte.
struct EntryPage : entry::PrefixTableBase<Entry> {
    uint16_t readPmMask() const { return pgm_read_word(&_pm_mask_P); }
    bool prefixMatcher(PrefixMode pm) const { return readPmMask() & PM_BIT(pm); }
    constexpr EntryPage(uint16_t pm_mask, uint16_t prefix, const Entry *head_P, const Entry *tail_P,
            const uint8_t *index_P, const uint8_t *itail_P)
        : PrefixTableBase(prefix, head_P, tail_P, index_P, itail_P), _pm_mask_P(pm_mask) {}

private:
    const uint16_t _pm_mask_P;
};

struct Cpu : entry::CpuBase<CpuType, EntryPage> {
    constexpr Cpu(CpuType cpuType, const /*PROGMEM*/ char *name_P, const EntryPage *head_P,
            const EntryPage *tail_P)
        : CpuBase(cpuType, name_P, head_P, tail_P) {}
    const EntryPage *pagesBegin() const { return _pages.readHead(); }
    const EntryPage *pagesEnd() const { return _pages.readTail(); }
};

// clang-format off
constexpr EntryPage TLCS900_PAGES[] PROGMEM = {
    {PM_BIT(PM_NONE),  0,    ARRAY_RANGE(TABLE_OPC_TLCS900), ARRAY_RANGE(INDEX_OPC_TLCS900)},
    {PM_BIT(PM_NONE),  0,    ARRAY_RANGE(TABLE_LDX),         ARRAY_RANGE(INDEX_LDX)},
    {PM_BIT(PM_NONE),  0,    ARRAY_RANGE(TABLE_OPC),         ARRAY_RANGE(INDEX_OPC)},
    {PM_BITS_REG | PM_BITS_ABREG, 0,
                             ARRAY_RANGE(TABLE_SRC_REG),     ARRAY_RANGE(INDEX_SRC_REG)},
    {PM_BITS_ABREG,    0,    ARRAY_RANGE(TABLE_SRC_ABREG),   ARRAY_RANGE(INDEX_SRC_ABREG)},
    // TABLE_DST_MEM precedes TABLE_SRC_MEM so the encoder prefers the ASL-style
    // PM_MEMD (destination-prefixed) form for LD (mem),(mem) ambiguities.
    {PM_BIT(PM_MEMD),  0,    ARRAY_RANGE(TABLE_DST_MEM),     ARRAY_RANGE(INDEX_DST_MEM)},
    {PM_BITS_MEM,      0,    ARRAY_RANGE(TABLE_SRC_MEM),     ARRAY_RANGE(INDEX_SRC_MEM)},
    {PM_BIT(PM_BLOCK), 0x83,   ARRAY_RANGE(TABLE_BLOCK),     ARRAY_RANGE(INDEX_BLOCK)},
    {PM_BIT(PM_RETCC), 0xB0,   ARRAY_RANGE(TABLE_RETCC),     ARRAY_RANGE(INDEX_RETCC)},
    {PM_BIT(PM_LDAR),  0xF313, ARRAY_RANGE(TABLE_LDAR),      ARRAY_RANGE(INDEX_LDAR)},
};

constexpr EntryPage TLCS900L_PAGES[] PROGMEM = {
    {PM_BIT(PM_NONE),  0,    ARRAY_RANGE(TABLE_OPC_TLCS900L), ARRAY_RANGE(INDEX_OPC_TLCS900L)},
    {PM_BIT(PM_NONE),  0,    ARRAY_RANGE(TABLE_LDX),          ARRAY_RANGE(INDEX_LDX)},
    {PM_BIT(PM_NONE),  0,    ARRAY_RANGE(TABLE_OPC),          ARRAY_RANGE(INDEX_OPC)},
    {PM_BITS_REG | PM_BITS_ABREG, 0,
                             ARRAY_RANGE(TABLE_SRC_REG),      ARRAY_RANGE(INDEX_SRC_REG)},
    {PM_BITS_ABREG,    0,    ARRAY_RANGE(TABLE_SRC_ABREG),    ARRAY_RANGE(INDEX_SRC_ABREG)},
    {PM_BIT(PM_MEMD),  0,    ARRAY_RANGE(TABLE_DST_MEM),      ARRAY_RANGE(INDEX_DST_MEM)},
    {PM_BITS_MEM,      0,    ARRAY_RANGE(TABLE_SRC_MEM),      ARRAY_RANGE(INDEX_SRC_MEM)},
    {PM_BIT(PM_BLOCK), 0x83,   ARRAY_RANGE(TABLE_BLOCK),      ARRAY_RANGE(INDEX_BLOCK)},
    {PM_BIT(PM_RETCC), 0xB0,   ARRAY_RANGE(TABLE_RETCC),      ARRAY_RANGE(INDEX_RETCC)},
    {PM_BIT(PM_LDAR),  0xF313, ARRAY_RANGE(TABLE_LDAR),       ARRAY_RANGE(INDEX_LDAR)},
};

constexpr Cpu CPU_TABLE[] PROGMEM = {
    {TLCS900,   TEXT_CPU_TLCS900,   ARRAY_RANGE(TLCS900_PAGES)},
    {TLCS900L,  TEXT_CPU_TLCS900L,  ARRAY_RANGE(TLCS900L_PAGES)},
};
// clang-format on

static const Cpu *cpu(CpuType cpuType) {
    return Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
}

struct PrefixPage {
    uint8_t lo_P;
    uint8_t hi_P;
    PrefixMode pm_P;

    bool contains(Config::opcode_t code) const {
        return pgm_read_byte(&lo_P) <= code && code <= pgm_read_byte(&hi_P);
    }
    PrefixMode readPm() const { return static_cast<PrefixMode>(pgm_read_byte(&pm_P)); }
};

// clang-format off
// Maps prefix byte ranges to PrefixMode. Single-byte specials (0x83, 0xB0)
// must precede the ranges that contain them (0x80-0x8F, 0xB1-0xBF).
constexpr PrefixPage PREFIX_PAGES[] PROGMEM = {
    {0x83, 0x83, PM_BLOCK},
    {0x80, 0x8F, PM_MEM8},
    {0x90, 0x9F, PM_MEM16},
    {0xA0, 0xAF, PM_MEM32},
    {0xB0, 0xB0, PM_RETCC},
    {0xB1, 0xBF, PM_MEMD},
    {0xC0, 0xC5, PM_MEM8},
    {0xC7, 0xC7, PM_ABREG8},
    {0xC8, 0xCF, PM_REG8},
    {0xD0, 0xD5, PM_MEM16},
    {0xD7, 0xD7, PM_ABREG16},
    {0xD8, 0xDF, PM_REG16},
    {0xE0, 0xE5, PM_MEM32},
    {0xE7, 0xE7, PM_ABREG32},
    {0xE8, 0xEF, PM_REG32},
    {0xF0, 0xF5, PM_MEMD},
};
// clang-format on

bool isPrefix(CpuType /*cpuType*/, Config::opcode_t code, PrefixMode &pm) {
    for (const auto *p = ARRAY_BEGIN(PREFIX_PAGES); p < ARRAY_END(PREFIX_PAGES); p++) {
        if (p->contains(code)) {
            pm = p->readPm();
            return true;
        }
    }
    return false;
}

static bool matchOpCode(const Entry *entry, Config::opcode_t opc) {
    const auto flags = entry->readFlags();
    return (opc & ~flags.mask()) == entry->readOpCode();
}

static bool isMem(AddrMode mode) {
    return mode == M_IND || mode == M_PRDC || mode == M_PINC || mode == M_IDX8 || mode == M_IDX16 ||
           mode == M_IDXR || mode == M_ABS8 || mode == M_ABS16 || mode == M_ABS24;
}

static bool modeMatch(AddrMode actual, AddrMode expected, RegName reg) {
    if (actual == expected)
        return true;
    if (expected == M_NONE)
        return actual == M_NONE;
    if (actual == R_C)
        return expected == M_REG8 || expected == M_CC;
    if (expected == M_IMM16 && actual == M_IMM8)
        return true;
    if (expected == M_IMM32 && (actual == M_IMM8 || actual == M_IMM16))
        return true;
    if (expected == M_REL16 && actual == M_REL8)
        return true;
    if (expected == R_WA && actual == M_REG16 && reg == REG_WA)
        return true;
    if (expected == R_A && actual == M_REG8 && reg == REG_A)
        return true;
    if (expected == R_SR && actual == R_SR)
        return true;
    if ((expected == M_LDF || expected == M_INTLVL) && actual == M_IMM8)
        return true;
    if (expected == M_SWI && actual == M_IMM8)
        return true;
    if (expected == M_BIT && actual == M_IMM8)
        return true;
    if (expected == M_RCOUNT && actual == M_IMM8)
        return true;
    if (expected == M_BUF)
        return actual == M_IMM8 || actual == M_IMM16;
    if (expected == M_LDXDST)
        return actual == M_ABS8 || actual == M_ABS16;
    if (expected == M_ZERO)
        return actual == M_NONE;
    if (expected == M_DISUF)
        return actual == M_NONE;
    if (expected == M_R32SRC)
        return actual == M_REG32;
    if (expected == M_LDARREG)
        return actual == M_REG16 || actual == M_REG32;
    if (expected == M_LDARREL)
        return actual == M_IMM8 || actual == M_IMM16 || actual == M_IMM32;
    if (expected == M_SRC || expected == M_DST)
        return isMem(actual);
    if (expected == M_REL8 || expected == M_REL16)
        return actual == M_IMM8 || actual == M_IMM16 || actual == M_IMM32;
    if (expected == M_STEP1 || expected == M_STEP2 || expected == M_STEP4)
        return actual == M_IMM8;
    return false;
}

// Resolve a context-relative AddrMode (M_DST/M_SRC/M_ABSDST/M_IMMX) to a
// concrete AddrMode for the current PrefixMode. Returns expected unchanged
// when no resolution applies.
static AddrMode resolveMode(AddrMode expected, PrefixMode pm, bool dstSlot, const Operand &op) {
    switch (expected) {
    case M_DST:
        switch (pm) {
        case PM_REG8:
        case PM_MEM8:
        case PM_ABREG8:
            return M_REG8;
        case PM_REG16:
        case PM_MEM16:
        case PM_ABREG16:
            return M_REG16;
        case PM_REG32:
        case PM_MEM32:
        case PM_ABREG32:
            return M_REG32;
        default:
            return expected;
        }
    case M_SRC:
        switch (pm) {
        case PM_REG8:
            return M_REG8;
        case PM_REG16:
            return M_REG16;
        case PM_REG32:
            return M_REG32;
        case PM_ABREG8:
            return M_ABREG8;
        case PM_ABREG16:
            return M_ABREG16;
        case PM_ABREG32:
            return M_ABREG32;
        case PM_MEM8:
        case PM_MEM16:
        case PM_MEM32:
            // src slot keeps the operand's actual memory mode; dst slot
            // never reaches PM_MEM* with M_SRC in real entries.
            return dstSlot ? expected : (isMem(op.mode) ? op.mode : M_NONE);
        default:
            return expected;
        }
    case M_ABSDST:
        switch (pm) {
        case PM_ABREG8:
            return M_ABREG8;
        case PM_ABREG16:
            return M_ABREG16;
        case PM_ABREG32:
            return M_ABREG32;
        default:
            return expected;
        }
    case M_IMMX:
        switch (pm) {
        case PM_REG8:
        case PM_MEM8:
        case PM_ABREG8:
            return M_IMM8;
        case PM_REG16:
        case PM_MEM16:
        case PM_ABREG16:
            return M_IMM16;
        case PM_REG32:
        case PM_MEM32:
        case PM_ABREG32:
            return M_IMM32;
        default:
            return M_IMM8;
        }
    default:
        return expected;
    }
}

static bool acceptMode(const Operand &op, AddrMode expected, PrefixMode pm, bool dstSlot) {
    const auto resolved = resolveMode(expected, pm, dstSlot, op);
    if (resolved == R_F)
        return op.mode == M_CC && op.cc == CC_F;
    return modeMatch(op.mode, resolved, op.reg);
}

static bool acceptModes(AsmInsn &insn, const Entry *entry, PrefixMode pm) {
    const auto flags = entry->readFlags();
    if (!acceptMode(insn.dstOp, flags.dst(), pm, /*dstSlot=*/true))
        return false;
    if (!acceptMode(insn.srcOp, flags.src(), pm, /*dstSlot=*/false))
        return false;
    const auto dst = flags.dst();
    if (dst == M_STEP1 && insn.dstOp.val.getUnsigned() != 1)
        return false;
    if (dst == M_STEP2 && insn.dstOp.val.getUnsigned() != 2)
        return false;
    if (dst == M_STEP4 && insn.dstOp.val.getUnsigned() != 4)
        return false;
    return true;
}

// searchName callbacks (CpuBase::searchName).

// Store the page's PrefixMode mask on insn so acceptOperands can iterate it.
static void pageSetup(AsmInsn &insn, const EntryPage *page) {
    insn.setPmMask(page->readPmMask());
}

// Try each PrefixMode in the page's mask; on first match, record the PM on
// insn so readCode can finalize the entry. Called by binarySearch once per
// candidate entry with the current page's mask already stored on insn.
static bool acceptOperands(AsmInsn &insn, const Entry *entry) {
    const auto mask = insn.pmMask();
    for (uint8_t bit = 0; bit <= PM_LDAR; bit++) {
        const auto pm = PrefixMode(bit);
        if (!(mask & PM_BIT(pm)))
            continue;
        if (acceptModes(insn, entry, pm)) {
            insn.setPrefixMode(pm);
            return true;
        }
    }
    return false;
}

Error searchName(CpuType cpuType, AsmInsn &insn) {
    cpu(cpuType)->searchName(insn, acceptOperands, pageSetup);
    return insn.getError();
}

// searchOpCode callbacks (CpuBase::searchOpCode).

// Set the page's PrefixMode bitmask on insn and pick the PM (derived from
// the recorded prefix byte) that this page accepts. Returns false if no
// PM in the page's mask matches.
static bool prefixMatcher(DisInsn &insn, const EntryPage *page) {
    const auto mask = page->readPmMask();
    insn.setPmMask(mask);
    PrefixMode pm = PM_NONE;
    const auto prefix = insn.prefix();
    if (prefix > UINT8_MAX) {
        // Only PM_LDAR uses a 2-byte prefix (F3 13).
        pm = PM_LDAR;
    } else if (prefix != 0 && !isPrefix(TLCS900, static_cast<Config::opcode_t>(prefix), pm)) {
        return false;
    }
    if ((mask & PM_BIT(pm)) == 0)
        return false;
    insn.setPrefixMode(pm);
    return true;
}

static bool opcMatchCode(DisInsn &insn, const Entry *entry, const EntryPage *) {
    return matchOpCode(entry, insn.opCode());
}

Error searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) {
    cpu(cpuType)->searchOpCode(insn, out, opcMatchCode, prefixMatcher);
    return insn.getError();
}

const /*PROGMEM*/ char *TableTlcs900::listCpu_P() const {
    return TEXT_TLCS900_LIST;
}

const /*PROGMEM*/ char *TableTlcs900::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableTlcs900::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    const auto *t = Cpu::search(name, ARRAY_RANGE(CPU_TABLE));
    if (t) {
        cpuType = t->readCpuType();
        return OK;
    }
    return UNSUPPORTED_CPU;
}

const TableTlcs900 TABLE;

}  // namespace tlcs900
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

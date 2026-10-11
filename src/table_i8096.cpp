/*
 * Copyright 2022 Tadashi G. Takaoka
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

#include "table_i8096.h"
#include "entry_i8096.h"
#include "entry_table.h"
#include "reg_i8096.h"
#include "text_i8096.h"

using namespace libasm::text::i8096;

namespace libasm {
namespace i8096 {

#define E3(_opc, _cf, _name, _dst, _src1, _src2) \
    {_opc, Entry::Flags::create(_cf, _dst, _src1, _src2), _name}
#define E2(_opc, _cf, _name, _dst, _src1) E3(_opc, _cf, _name, _dst, _src1, M_NONE)
#define E1(_opc, _cf, _name, _dst) E2(_opc, _cf, _name, _dst, M_NONE)
#define E0(_opc, _cf, _name) E1(_opc, _cf, _name, M_NONE)
#define D3(_opc, _cf, _name, _dst, _src1, _src2, _post) \
    {_opc, Entry::Flags::create(_cf, _dst, _src1, _src2), _post, _name}
#define U2(_opc, _cf, _name, _dst, _src1) {_opc, Entry::Flags::undef(_cf, _dst, _src1), _name}
#define U1(_opc, _cf, _name, _dst) U2(_opc, _cf, _name, _dst, M_NONE)

// clang-format off
constexpr Entry TABLE_00[] PROGMEM = {
    E2(0x64, CF_03, TEXT_ADD,   M_WREG,  M_WAOP),
    E3(0x44, CF_03, TEXT_ADD,   M_WREG,  M_WREG,  M_WAOP),
    E2(0x74, CF_03, TEXT_ADDB,  M_BREG,  M_BAOP),
    E3(0x54, CF_03, TEXT_ADDB,  M_BREG,  M_BREG,  M_BAOP),
    E2(0xA4, CF_03, TEXT_ADDC,  M_WREG,  M_WAOP),
    E2(0xB4, CF_03, TEXT_ADDCB, M_BREG,  M_BAOP),
    E2(0x60, CF_03, TEXT_AND,   M_WREG,  M_WAOP),
    E3(0x40, CF_03, TEXT_AND,   M_WREG,  M_WREG,  M_WAOP),
    E2(0x70, CF_03, TEXT_ANDB,  M_BREG,  M_BAOP),
    E3(0x50, CF_03, TEXT_ANDB,  M_BREG,  M_BREG,  M_BAOP),
    E1(0xE3, CF_00, TEXT_BR,    M_INDIR),
    E1(0x01, CF_00, TEXT_CLR,   M_WREG),
    E1(0x11, CF_00, TEXT_CLRB,  M_BREG),
    E0(0xF8, CF_00, TEXT_CLRC),
    E0(0xFC, CF_00, TEXT_CLRVT),
    E2(0x88, CF_03, TEXT_CMP,   M_WREG,  M_WAOP),
    E2(0x98, CF_03, TEXT_CMPB,  M_BREG,  M_BAOP),
    E1(0x05, CF_00, TEXT_DEC,   M_WREG),
    E1(0x15, CF_00, TEXT_DECB,  M_BREG),
    E0(0xFA, CF_00, TEXT_DI),
    E2(0x8C, CF_03, TEXT_DIVU,  M_LREG,  M_WAOP),
    E2(0x9C, CF_03, TEXT_DIVUB, M_WREG,  M_BAOP),
    E2(0xE0, CF_00, TEXT_DJNZ,  M_BREG,  M_REL8),
    E0(0xFB, CF_00, TEXT_EI),
    E1(0x06, CF_00, TEXT_EXT,   M_LREG),
    E1(0x16, CF_00, TEXT_EXTB,  M_WREG),
    E1(0x07, CF_00, TEXT_INC,   M_WREG),
    E1(0x17, CF_00, TEXT_INCB,  M_BREG),
    E3(0x30, CF_07, TEXT_JBC,   M_BREG,  M_BITNO, M_REL8),
    E3(0x38, CF_07, TEXT_JBS,   M_BREG,  M_BITNO, M_REL8),
    E1(0xDB, CF_00, TEXT_JC,    M_REL8),
    E1(0xDF, CF_00, TEXT_JE,    M_REL8),
    E1(0xD6, CF_00, TEXT_JGE,   M_REL8),
    E1(0xD2, CF_00, TEXT_JGT,   M_REL8),
    E1(0xD9, CF_00, TEXT_JH,    M_REL8),
    E1(0xDA, CF_00, TEXT_JLE,   M_REL8),
    E1(0xDE, CF_00, TEXT_JLT,   M_REL8),
    E1(0xD3, CF_00, TEXT_JNC,   M_REL8),
    E1(0xD7, CF_00, TEXT_JNE,   M_REL8),
    E1(0xD1, CF_00, TEXT_JNH,   M_REL8),
    E1(0xD0, CF_00, TEXT_JNST,  M_REL8),
    E1(0xD5, CF_00, TEXT_JNV,   M_REL8),
    E1(0xD4, CF_00, TEXT_JNVT,  M_REL8),
    E1(0xD8, CF_00, TEXT_JST,   M_REL8),
    E1(0xDD, CF_00, TEXT_JV,    M_REL8),
    E1(0xDC, CF_00, TEXT_JVT,   M_REL8),
    E1(0xEF, CF_00, TEXT_LCALL, M_REL16),
    E2(0xA0, CF_03, TEXT_LD,    M_WREG,  M_WAOP),
    E2(0xB0, CF_03, TEXT_LDB,   M_BREG,  M_BAOP),
    E2(0xBC, CF_03, TEXT_LDBSE, M_WREG,  M_BAOP),
    E2(0xAC, CF_03, TEXT_LDBZE, M_WREG,  M_BAOP),
    E1(0xE7, CF_00, TEXT_LJMP,  M_REL16),
    E2(0x6C, CF_03, TEXT_MULU,  M_LREG,  M_WAOP),
    E3(0x4C, CF_03, TEXT_MULU,  M_LREG,  M_WREG,  M_WAOP),
    E2(0x7C, CF_03, TEXT_MULUB, M_WREG,  M_BAOP),
    E3(0x5C, CF_03, TEXT_MULUB, M_WREG,  M_BREG,  M_BAOP),
    E1(0x03, CF_00, TEXT_NEG,   M_WREG),
    E1(0x13, CF_00, TEXT_NEGB,  M_BREG),
    E0(0xFD, CF_00, TEXT_NOP),
    E2(0x0F, CF_00, TEXT_NORML, M_LREG,  M_BREG),
    E1(0x02, CF_00, TEXT_NOT,   M_WREG),
    E1(0x12, CF_00, TEXT_NOTB,  M_BREG),
    E2(0x80, CF_03, TEXT_OR,    M_WREG,  M_WAOP),
    E2(0x90, CF_03, TEXT_ORB,   M_BREG,  M_BAOP),
    U1(0xCD, CF_00, TEXT_POP,   M_IMM16), // undefined POP immediate instruction
    E1(0xCC, CF_03, TEXT_POP,   M_WAOP),
    E0(0xF3, CF_00, TEXT_POPF),
    E1(0xC8, CF_03, TEXT_PUSH,  M_WAOP),
    E0(0xF2, CF_00, TEXT_PUSHF),
    E0(0xF0, CF_00, TEXT_RET),
    E0(0xFF, CF_00, TEXT_RST),
    E1(0x28, CF_07, TEXT_SCALL, M_REL11),
    E0(0xF9, CF_00, TEXT_SETC),
    E2(0x09, CF_00, TEXT_SHL,   M_WREG,  M_COUNT),
    E2(0x19, CF_00, TEXT_SHLB,  M_BREG,  M_COUNT),
    E2(0x0D, CF_00, TEXT_SHLL,  M_LREG,  M_COUNT),
    E2(0x08, CF_00, TEXT_SHR,   M_WREG,  M_COUNT),
    E2(0x0A, CF_00, TEXT_SHRA,  M_WREG,  M_COUNT),
    E2(0x1A, CF_00, TEXT_SHRAB, M_BREG,  M_COUNT),
    E2(0x0E, CF_00, TEXT_SHRAL, M_LREG,  M_COUNT),
    E2(0x18, CF_00, TEXT_SHRB,  M_BREG,  M_COUNT),
    E2(0x0C, CF_00, TEXT_SHRL,  M_LREG,  M_COUNT),
    E1(0x20, CF_07, TEXT_SJMP,  M_REL11),
    E1(0x00, CF_00, TEXT_SKIP,  M_BREG),
    U2(0xC1, CF_00, TEXT_ST,    M_WREG,  M_IMM16), // undefined ST immediate instruction
    E2(0xC0, CF_03, TEXT_ST,    M_WREG,  M_WAOP),
    U2(0xC5, CF_00, TEXT_STB,   M_BREG,  M_IMM8), // undefined STB immediate instruction
    E2(0xC4, CF_03, TEXT_STB,   M_BREG,  M_BAOP),
    E2(0x68, CF_03, TEXT_SUB,   M_WREG,  M_WAOP),
    E3(0x48, CF_03, TEXT_SUB,   M_WREG,  M_WREG,  M_WAOP),
    E2(0x78, CF_03, TEXT_SUBB,  M_BREG,  M_BAOP),
    E3(0x58, CF_03, TEXT_SUBB,  M_BREG,  M_BREG,  M_BAOP),
    E2(0xA8, CF_03, TEXT_SUBC,  M_WREG,  M_WAOP),
    E2(0xB8, CF_03, TEXT_SUBCB, M_BREG,  M_BAOP),
    E0(0xF7, CF_00, TEXT_TRAP),
    E2(0x84, CF_03, TEXT_XOR,   M_WREG,  M_WAOP),
    E2(0x94, CF_03, TEXT_XORB,  M_BREG,  M_BAOP),
};

constexpr uint8_t INDEX_00[] PROGMEM = {
      0,  // TEXT_ADD
      1,  // TEXT_ADD
      2,  // TEXT_ADDB
      3,  // TEXT_ADDB
      4,  // TEXT_ADDC
      5,  // TEXT_ADDCB
      6,  // TEXT_AND
      7,  // TEXT_AND
      8,  // TEXT_ANDB
      9,  // TEXT_ANDB
     10,  // TEXT_BR
     11,  // TEXT_CLR
     12,  // TEXT_CLRB
     13,  // TEXT_CLRC
     14,  // TEXT_CLRVT
     15,  // TEXT_CMP
     16,  // TEXT_CMPB
     17,  // TEXT_DEC
     18,  // TEXT_DECB
     19,  // TEXT_DI
     20,  // TEXT_DIVU
     21,  // TEXT_DIVUB
     22,  // TEXT_DJNZ
     23,  // TEXT_EI
     24,  // TEXT_EXT
     25,  // TEXT_EXTB
     26,  // TEXT_INC
     27,  // TEXT_INCB
     28,  // TEXT_JBC
     29,  // TEXT_JBS
     30,  // TEXT_JC
     31,  // TEXT_JE
     32,  // TEXT_JGE
     33,  // TEXT_JGT
     34,  // TEXT_JH
     35,  // TEXT_JLE
     36,  // TEXT_JLT
     37,  // TEXT_JNC
     38,  // TEXT_JNE
     39,  // TEXT_JNH
     40,  // TEXT_JNST
     41,  // TEXT_JNV
     42,  // TEXT_JNVT
     43,  // TEXT_JST
     44,  // TEXT_JV
     45,  // TEXT_JVT
     46,  // TEXT_LCALL
     47,  // TEXT_LD
     48,  // TEXT_LDB
     49,  // TEXT_LDBSE
     50,  // TEXT_LDBZE
     51,  // TEXT_LJMP
     52,  // TEXT_MULU
     53,  // TEXT_MULU
     54,  // TEXT_MULUB
     55,  // TEXT_MULUB
     56,  // TEXT_NEG
     57,  // TEXT_NEGB
     58,  // TEXT_NOP
     59,  // TEXT_NORML
     60,  // TEXT_NOT
     61,  // TEXT_NOTB
     62,  // TEXT_OR
     63,  // TEXT_ORB
     64,  // TEXT_POP
     65,  // TEXT_POP
     66,  // TEXT_POPF
     67,  // TEXT_PUSH
     68,  // TEXT_PUSHF
     69,  // TEXT_RET
     70,  // TEXT_RST
     71,  // TEXT_SCALL
     72,  // TEXT_SETC
     73,  // TEXT_SHL
     74,  // TEXT_SHLB
     75,  // TEXT_SHLL
     76,  // TEXT_SHR
     77,  // TEXT_SHRA
     78,  // TEXT_SHRAB
     79,  // TEXT_SHRAL
     80,  // TEXT_SHRB
     81,  // TEXT_SHRL
     82,  // TEXT_SJMP
     83,  // TEXT_SKIP
     84,  // TEXT_ST
     85,  // TEXT_ST
     86,  // TEXT_STB
     87,  // TEXT_STB
     88,  // TEXT_SUB
     89,  // TEXT_SUB
     90,  // TEXT_SUBB
     91,  // TEXT_SUBB
     92,  // TEXT_SUBC
     93,  // TEXT_SUBCB
     94,  // TEXT_TRAP
     95,  // TEXT_XOR
     96,  // TEXT_XORB
};

constexpr Entry TABLE_FE[] PROGMEM = {
    E2(0x8C, CF_03, TEXT_DIV,   M_LREG,  M_WAOP),
    E2(0x9C, CF_03, TEXT_DIVB,  M_WREG,  M_BAOP),
    E2(0x6C, CF_03, TEXT_MUL,   M_LREG,  M_WAOP),
    E3(0x4C, CF_03, TEXT_MUL,   M_LREG,  M_WREG,  M_WAOP),
    E2(0x7C, CF_03, TEXT_MULB,  M_WREG,  M_BAOP),
    E3(0x5C, CF_03, TEXT_MULB,  M_WREG,  M_BREG,  M_BAOP),
};

constexpr uint8_t INDEX_FE[] PROGMEM = {
      0,  // TEXT_DIV
      1,  // TEXT_DIVB
      2,  // TEXT_MUL
      3,  // TEXT_MUL
      4,  // TEXT_MULB
      5,  // TEXT_MULB
};

constexpr Entry I80196KB_TABLE[] PROGMEM = {
    E2(0xC1, CF_00, TEXT_BMOV,  M_LREG, M_WREG),
    E2(0xC5, CF_00, TEXT_CMPL,  M_LREG, M_LREG),
    E2(0xE1, CF_00, TEXT_DJNZW, M_WREG, M_REL8),
    E0(0xF4, CF_00, TEXT_PUSHA),
    E0(0xF5, CF_00, TEXT_POPA),
    E1(0xF6, CF_00, TEXT_IDLPD, M_IMM8),
};

constexpr uint8_t I80196KB_INDEX[] PROGMEM = {
      0,  // TEXT_BMOV
      1,  // TEXT_BMOV
      2,  // TEXT_DJNZW
      5,  // TEXT_IDLPD
      4,  // TEXT_POPA
      3,  // TEXT_PUSHA
};

constexpr Entry I80196KC_TABLE[] PROGMEM = {
    E2(0x04, CF_00, TEXT_XCH,   M_WREG, M_WREG),
    E2(0x0B, CF_00, TEXT_XCH,   M_WREG, M_WIDX),
    E2(0x14, CF_00, TEXT_XCHB,  M_BREG, M_BREG),
    E2(0x1B, CF_00, TEXT_XCHB,  M_BREG, M_BIDX),
    E2(0xCD, CF_00, TEXT_BMOVI, M_LREG, M_WREG),
    E3(0xE2, CF_00, TEXT_TIJMP, M_WREG, M_WREG, M_IMM8),
};

constexpr uint8_t I80196KC_INDEX[] PROGMEM = {
      4,  // TEXT_BMOVI
      5,  // TEXT_TIJMP
      0,  // TEXT_XCH
      1,  // TEXT_XCH
      2,  // TEXT_XCHB
      3,  // TEXT_XCHB
};

// The PTS instructions are a page of their own because the 80296 has no
// peripheral transaction server and drops them.
constexpr Entry PTS_TABLE[] PROGMEM = {
    E0(0xEC, CF_00, TEXT_DPTS),
    E0(0xED, CF_00, TEXT_EPTS),
};

constexpr uint8_t PTS_INDEX[] PROGMEM = {
      0,  // TEXT_DPTS
      1,  // TEXT_EPTS
};

// 80296: the return-from-interrupt the earlier parts leave reserved, the DSP
// set, and the prefixless signed multiply and divide.  None of the DSP
// instructions has an opcode of its own; each shares one with MUL/MULU, AND or
// SHLL and carries a selector, which is why they are the only entries with a
// |_post| (manual 3.3.2, 3.3.4, A-6).
constexpr Entry I80296_TABLE[] PROGMEM = {
    E0(0xE5, CF_00, TEXT_RETI),
    D3(0x0D, CF_00, TEXT_MVAC,    M_ACCSEL, M_POINTR, M_PACC, 0x01),
    D3(0x0D, CF_00, TEXT_MSAC,    M_ACCSEL, M_POINTR, M_PACC, 0x03),
    D3(0x40, CF_03, TEXT_RPT,     M_WAOP,   M_NONE,   M_PRPT, 0x00),
    D3(0x40, CF_03, TEXT_RPTNST,  M_WAOP,   M_NONE,   M_PRPT, 0x10),
    D3(0x40, CF_03, TEXT_RPTNH,   M_WAOP,   M_NONE,   M_PRPT, 0x11),
    D3(0x40, CF_03, TEXT_RPTGT,   M_WAOP,   M_NONE,   M_PRPT, 0x12),
    D3(0x40, CF_03, TEXT_RPTNC,   M_WAOP,   M_NONE,   M_PRPT, 0x13),
    D3(0x40, CF_03, TEXT_RPTNVT,  M_WAOP,   M_NONE,   M_PRPT, 0x14),
    D3(0x40, CF_03, TEXT_RPTNV,   M_WAOP,   M_NONE,   M_PRPT, 0x15),
    D3(0x40, CF_03, TEXT_RPTGE,   M_WAOP,   M_NONE,   M_PRPT, 0x16),
    D3(0x40, CF_03, TEXT_RPTNE,   M_WAOP,   M_NONE,   M_PRPT, 0x17),
    D3(0x40, CF_03, TEXT_RPTST,   M_WAOP,   M_NONE,   M_PRPT, 0x18),
    D3(0x40, CF_03, TEXT_RPTH,    M_WAOP,   M_NONE,   M_PRPT, 0x19),
    D3(0x40, CF_03, TEXT_RPTLE,   M_WAOP,   M_NONE,   M_PRPT, 0x1A),
    D3(0x40, CF_03, TEXT_RPTC,    M_WAOP,   M_NONE,   M_PRPT, 0x1B),
    D3(0x40, CF_03, TEXT_RPTVT,   M_WAOP,   M_NONE,   M_PRPT, 0x1C),
    D3(0x40, CF_03, TEXT_RPTV,    M_WAOP,   M_NONE,   M_PRPT, 0x1D),
    D3(0x40, CF_03, TEXT_RPTLT,   M_WAOP,   M_NONE,   M_PRPT, 0x1E),
    D3(0x40, CF_03, TEXT_RPTE,    M_WAOP,   M_NONE,   M_PRPT, 0x1F),
    D3(0x40, CF_03, TEXT_RPTI,    M_WAOP,   M_NONE,   M_PRPT, 0x20),
    D3(0x40, CF_03, TEXT_RPTINST, M_WAOP,   M_NONE,   M_PRPT, 0x30),
    D3(0x40, CF_03, TEXT_RPTINH,  M_WAOP,   M_NONE,   M_PRPT, 0x31),
    D3(0x40, CF_03, TEXT_RPTIGT,  M_WAOP,   M_NONE,   M_PRPT, 0x32),
    D3(0x40, CF_03, TEXT_RPTINC,  M_WAOP,   M_NONE,   M_PRPT, 0x33),
    D3(0x40, CF_03, TEXT_RPTINVT, M_WAOP,   M_NONE,   M_PRPT, 0x34),
    D3(0x40, CF_03, TEXT_RPTINV,  M_WAOP,   M_NONE,   M_PRPT, 0x35),
    D3(0x40, CF_03, TEXT_RPTIGE,  M_WAOP,   M_NONE,   M_PRPT, 0x36),
    D3(0x40, CF_03, TEXT_RPTINE,  M_WAOP,   M_NONE,   M_PRPT, 0x37),
    D3(0x40, CF_03, TEXT_RPTIST,  M_WAOP,   M_NONE,   M_PRPT, 0x38),
    D3(0x40, CF_03, TEXT_RPTIH,   M_WAOP,   M_NONE,   M_PRPT, 0x39),
    D3(0x40, CF_03, TEXT_RPTILE,  M_WAOP,   M_NONE,   M_PRPT, 0x3A),
    D3(0x40, CF_03, TEXT_RPTIC,   M_WAOP,   M_NONE,   M_PRPT, 0x3B),
    D3(0x40, CF_03, TEXT_RPTIVT,  M_WAOP,   M_NONE,   M_PRPT, 0x3C),
    D3(0x40, CF_03, TEXT_RPTIV,   M_WAOP,   M_NONE,   M_PRPT, 0x3D),
    D3(0x40, CF_03, TEXT_RPTILT,  M_WAOP,   M_NONE,   M_PRPT, 0x3E),
    D3(0x40, CF_03, TEXT_RPTIE,   M_WAOP,   M_NONE,   M_PRPT, 0x3F),
    D3(0x4C, CF_03, TEXT_MAC,     M_WREG,   M_WAOP,   M_PMAC, 0x00),
    D3(0x4C, CF_03, TEXT_SMAC,    M_WREG,   M_WAOP,   M_PMAC, 0x01),
    D3(0x4C, CF_03, TEXT_MACR,    M_WREG,   M_WAOP,   M_PMAC, 0x04),
    D3(0x4C, CF_03, TEXT_SMACR,   M_WREG,   M_WAOP,   M_PMAC, 0x05),
    D3(0x4C, CF_03, TEXT_MACZ,    M_WREG,   M_WAOP,   M_PMAC, 0x08),
    D3(0x4C, CF_03, TEXT_SMACZ,   M_WREG,   M_WAOP,   M_PMAC, 0x09),
    D3(0x4C, CF_03, TEXT_MACRZ,   M_WREG,   M_WAOP,   M_PMAC, 0x0C),
    D3(0x4C, CF_03, TEXT_SMACRZ,  M_WREG,   M_WAOP,   M_PMAC, 0x0D),
    D3(0x6C, CF_03, TEXT_MAC,     M_WAOP,   M_NONE,   M_PMAC, 0x00),
    D3(0x6C, CF_03, TEXT_SMAC,    M_WAOP,   M_NONE,   M_PMAC, 0x01),
    D3(0x6C, CF_03, TEXT_MACR,    M_WAOP,   M_NONE,   M_PMAC, 0x04),
    D3(0x6C, CF_03, TEXT_SMACR,   M_WAOP,   M_NONE,   M_PMAC, 0x05),
    D3(0x6C, CF_03, TEXT_MACZ,    M_WAOP,   M_NONE,   M_PMAC, 0x08),
    D3(0x6C, CF_03, TEXT_SMACZ,   M_WAOP,   M_NONE,   M_PMAC, 0x09),
    D3(0x6C, CF_03, TEXT_MACRZ,   M_WAOP,   M_NONE,   M_PMAC, 0x0C),
    D3(0x6C, CF_03, TEXT_SMACRZ,  M_WAOP,   M_NONE,   M_PMAC, 0x0D),
    // See M_SLREG.  These come after the MAC entries because a destination
    // below 10H is a selector rather than a register (manual 3.3.2).
    E2(0x8C, CF_03, TEXT_DIV,     M_SLREG, M_WAOP),
    E2(0x9C, CF_03, TEXT_DIVB,    M_SWREG, M_BAOP),
    E2(0x6C, CF_03, TEXT_MUL,     M_SLREG, M_WAOP),
    E3(0x4C, CF_03, TEXT_MUL,     M_SLREG, M_WREG,  M_WAOP),
    E2(0x7C, CF_03, TEXT_MULB,    M_SWREG, M_BAOP),
    E3(0x5C, CF_03, TEXT_MULB,    M_SWREG, M_BREG,  M_BAOP),
};

constexpr uint8_t I80296_INDEX[] PROGMEM = {
     53,  // TEXT_DIV
     54,  // TEXT_DIVB
     37,  // TEXT_MAC
     45,  // TEXT_MAC
     39,  // TEXT_MACR
     47,  // TEXT_MACR
     43,  // TEXT_MACRZ
     51,  // TEXT_MACRZ
     41,  // TEXT_MACZ
     49,  // TEXT_MACZ
      2,  // TEXT_MSAC
     55,  // TEXT_MUL
     56,  // TEXT_MUL
     57,  // TEXT_MULB
     58,  // TEXT_MULB
      1,  // TEXT_MVAC
      0,  // TEXT_RETI
      3,  // TEXT_RPT
     15,  // TEXT_RPTC
     19,  // TEXT_RPTE
     10,  // TEXT_RPTGE
      6,  // TEXT_RPTGT
     13,  // TEXT_RPTH
     20,  // TEXT_RPTI
     32,  // TEXT_RPTIC
     36,  // TEXT_RPTIE
     27,  // TEXT_RPTIGE
     23,  // TEXT_RPTIGT
     30,  // TEXT_RPTIH
     31,  // TEXT_RPTILE
     35,  // TEXT_RPTILT
     24,  // TEXT_RPTINC
     28,  // TEXT_RPTINE
     22,  // TEXT_RPTINH
     21,  // TEXT_RPTINST
     26,  // TEXT_RPTINV
     25,  // TEXT_RPTINVT
     29,  // TEXT_RPTIST
     34,  // TEXT_RPTIV
     33,  // TEXT_RPTIVT
     14,  // TEXT_RPTLE
     18,  // TEXT_RPTLT
      7,  // TEXT_RPTNC
     11,  // TEXT_RPTNE
      5,  // TEXT_RPTNH
      4,  // TEXT_RPTNST
      9,  // TEXT_RPTNV
      8,  // TEXT_RPTNVT
     12,  // TEXT_RPTST
     17,  // TEXT_RPTV
     16,  // TEXT_RPTVT
     38,  // TEXT_SMAC
     46,  // TEXT_SMAC
     40,  // TEXT_SMACR
     48,  // TEXT_SMACR
     44,  // TEXT_SMACRZ
     52,  // TEXT_SMACRZ
     42,  // TEXT_SMACZ
     50,  // TEXT_SMACZ
};

// clang-format on

using EntryPage = entry::PrefixTableBase<Entry>;

constexpr EntryPage I8096_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(TABLE_00), ARRAY_RANGE(INDEX_00)},
        {0xFE, ARRAY_RANGE(TABLE_FE), ARRAY_RANGE(INDEX_FE)},
};

// 80196N: the 1-Mbyte parts.  EBR shares opcode E3 with BR and is told apart
// by the least-significant bit of the second byte (manual A-7, EBR).
constexpr Entry I80196N_TABLE[] PROGMEM = {
    E2(0x1C, CF_00, TEXT_EST,    M_WREG, M_EINDIR),
    E2(0x1D, CF_00, TEXT_EST,    M_WREG, M_EIDX),
    E2(0x1E, CF_00, TEXT_ESTB,   M_BREG, M_EINDIR),
    E2(0x1F, CF_00, TEXT_ESTB,   M_BREG, M_EIDX),
    E1(0xE3, CF_00, TEXT_EBR,    M_EBIND),
    E2(0xE4, CF_00, TEXT_EBMOVI, M_QREG, M_WREG),
    E1(0xE6, CF_00, TEXT_EJMP,   M_REL24),
    E2(0xE8, CF_00, TEXT_ELD,    M_WREG, M_EINDIR),
    E2(0xE9, CF_00, TEXT_ELD,    M_WREG, M_EIDX),
    E2(0xEA, CF_00, TEXT_ELDB,   M_BREG, M_EINDIR),
    E2(0xEB, CF_00, TEXT_ELDB,   M_BREG, M_EIDX),
    E1(0xF1, CF_00, TEXT_ECALL,  M_REL24),
};

constexpr uint8_t I80196N_INDEX[] PROGMEM = {
      5,  // TEXT_EBMOVI
      4,  // TEXT_EBR
     11,  // TEXT_ECALL
      6,  // TEXT_EJMP
      7,  // TEXT_ELD
      8,  // TEXT_ELD
      9,  // TEXT_ELDB
     10,  // TEXT_ELDB
      0,  // TEXT_EST
      1,  // TEXT_EST
      2,  // TEXT_ESTB
      3,  // TEXT_ESTB
};

constexpr EntryPage I80196KB_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(I80196KB_TABLE), ARRAY_RANGE(I80196KB_INDEX)},
        {0x00, ARRAY_RANGE(TABLE_00), ARRAY_RANGE(INDEX_00)},
        {0xFE, ARRAY_RANGE(TABLE_FE), ARRAY_RANGE(INDEX_FE)},
};

constexpr EntryPage I80196N_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(I80196N_TABLE), ARRAY_RANGE(I80196N_INDEX)},
        {0x00, ARRAY_RANGE(I80196KB_TABLE), ARRAY_RANGE(I80196KB_INDEX)},
        {0x00, ARRAY_RANGE(I80196KC_TABLE), ARRAY_RANGE(I80196KC_INDEX)},
        {0x00, ARRAY_RANGE(PTS_TABLE), ARRAY_RANGE(PTS_INDEX)},
        {0x00, ARRAY_RANGE(TABLE_00), ARRAY_RANGE(INDEX_00)},
        {0xFE, ARRAY_RANGE(TABLE_FE), ARRAY_RANGE(INDEX_FE)},
};

constexpr EntryPage I80196_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(I80196KB_TABLE), ARRAY_RANGE(I80196KB_INDEX)},
        {0x00, ARRAY_RANGE(I80196KC_TABLE), ARRAY_RANGE(I80196KC_INDEX)},
        {0x00, ARRAY_RANGE(PTS_TABLE), ARRAY_RANGE(PTS_INDEX)},
        {0x00, ARRAY_RANGE(TABLE_00), ARRAY_RANGE(INDEX_00)},
        {0xFE, ARRAY_RANGE(TABLE_FE), ARRAY_RANGE(INDEX_FE)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr EntryPage I80296_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(I80296_TABLE), ARRAY_RANGE(I80296_INDEX)},
        {0x00, ARRAY_RANGE(I80196N_TABLE), ARRAY_RANGE(I80196N_INDEX)},
        {0x00, ARRAY_RANGE(I80196KB_TABLE), ARRAY_RANGE(I80196KB_INDEX)},
        {0x00, ARRAY_RANGE(I80196KC_TABLE), ARRAY_RANGE(I80196KC_INDEX)},
        {0x00, ARRAY_RANGE(TABLE_00), ARRAY_RANGE(INDEX_00)},
        {0xFE, ARRAY_RANGE(TABLE_FE), ARRAY_RANGE(INDEX_FE)},
};

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {I8096, TEXT_CPU_8096, ARRAY_RANGE(I8096_PAGES)},
        {I80196, TEXT_CPU_80196, ARRAY_RANGE(I80196_PAGES)},
        {I80196KB, TEXT_CPU_80196KB, ARRAY_RANGE(I80196KB_PAGES)},
        {I80196N, TEXT_CPU_80196N, ARRAY_RANGE(I80196N_PAGES)},
        {I80296, TEXT_CPU_80296, ARRAY_RANGE(I80296_PAGES)},
};

const Cpu *cpu(CpuType cpuType) {
    return Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
}

bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    // A DSP selector is never written out, so nothing is parsed for it.
    if (table == M_PMAC || table == M_PRPT || table == M_PACC)
        return opr == M_NONE;
    if (opr == M_REGNO)
        return table == M_BREG || table == M_WREG || table == M_LREG || table == M_QREG ||
               table == M_BAOP || table == M_WAOP || table == M_REL8 || table == M_REL11 ||
               table == M_REL16 || table == M_REL24 || table == M_COUNT || table == M_BITNO ||
               table == M_ACCSEL || table == M_POINTR || table == M_SLREG ||
               table == M_SWREG;
    if (opr == M_ADDR)
        return table == M_BAOP || table == M_WAOP || table == M_REL8 || table == M_REL11 ||
               table == M_REL16 || table == M_REL24 || table == M_COUNT || table == M_BITNO ||
               table == M_BIDX || table == M_WIDX;
    if (opr == M_IMM16)
        return table == M_BAOP || table == M_WAOP || table == M_COUNT || table == M_IMM8 ||
               table == M_POINTR;
    if (opr == M_INDIR || opr == M_AINC)
        return table == M_BAOP || table == M_WAOP || table == M_BIDX || table == M_WIDX ||
               table == M_EINDIR || table == M_EBIND;
    if (opr == M_IDX16)
        return table == M_BAOP || table == M_WAOP || table == M_BIDX || table == M_WIDX ||
               table == M_EIDX;
    return false;
}

bool acceptModes(AsmInsn &insn, const Entry *entry) {
    const auto table = entry->readFlags();
    const auto dst = table.dst();
    // The sign bit in the destination is what the 80296 prefers, but the FE
    // prefix assembles for the whole family, so it takes an option to ask for
    // the bit; see M_SLREG.
    if ((dst == M_SLREG || dst == M_SWREG) && insn.signedPrefix())
        return false;
    if (acceptMode(insn.dstOp.mode, table.dst()) && acceptMode(insn.src1Op.mode, table.src1()) &&
            acceptMode(insn.src2Op.mode, table.src2())) {
        if (table.undefined())
            insn.setErrorIf(OPERAND_NOT_ALLOWED);
        return true;
    }
    return false;
}

namespace {

// The DSP selector has to come across with the opcode; see M_PMAC.
void readCode(AsmInsn &insn, const Entry *entry, const EntryPage *page) {
    Cpu::defaultReadCode(insn, entry, page);
    insn.setPostfix(entry->readPost());
}

}  // namespace

Error searchName(CpuType cpuType, AsmInsn &insn) {
    cpu(cpuType)->searchName(insn, acceptModes, Cpu::defaultPageSetup, readCode);
    return insn.getError();
}

namespace {

// Whichever operand position holds the |aa|-selected one.
AddrMode aopMode(Entry::Flags flags) {
    const auto dst = flags.dst();
    if (dst == M_BAOP || dst == M_WAOP)
        return dst;
    const auto src1 = flags.src1();
    if (src1 == M_BAOP || src1 == M_WAOP)
        return src1;
    return flags.src2();
}

// How many bytes that operand takes.  Immediate is one byte wide for a byte
// operation; indexed is two or three, decided by the index register just as
// |readIndexed| decides it.
uint8_t aopLen(DisInsn &insn, Entry::Flags flags) {
    const auto aa = insn.aa();
    if (aa == AA_IMM)
        return aopMode(flags) == M_BAOP ? 1 : 2;
    if (aa == AA_IDX)
        return reg::isWreg(insn.peekOperand(0)) ? 2 : 3;
    return 1;
}

// Where the destination sits, counting from the first operand byte.  It comes
// last, after the |aa|-selected operand, which a 3-operand form takes as SRC2
// and follows with a register.
uint8_t dstPos(DisInsn &insn, Entry::Flags flags) {
    const auto len = aopLen(insn, flags);
    return flags.src2() == M_NONE ? len : len + 1;
}

// Where the DSP selector sits, counting from the first operand byte.
uint8_t postPos(DisInsn &insn, Entry::Flags flags) {
    if (flags.src2() == M_PACC)
        return 1;  // after the bit pointer
    const auto len = aopLen(insn, flags);
    // A 3-operand MAC has its second register between the operand and the
    // selector; everything else puts the selector straight after.
    return flags.src1() == M_WAOP ? len + 1 : len;
}

}  // namespace

bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *) {
    auto opc = insn.opCode();
    const auto flags = entry->readFlags();
    opc &= ~flags.mask();
    if (opc != entry->readOpCode())
        return false;
    // BR and EBR share opcode E3; EBR is the one with bit 0 of the operand
    // byte set.  BR is left alone, so the older CPUs decode as before.
    if (flags.dst() == M_EBIND)
        return insn.peekOperand(0) & 1;
    // On the 80296 bit 0 of the destination can carry the sign of a multiply or
    // divide in place of the FE prefix; see M_SLREG.
    const auto dst = flags.dst();
    if (dst == M_SLREG || dst == M_SWREG) {
        const auto reg = insn.peekOperand(dstPos(insn, flags));
        if (!insn.isOK())
            return false;
        return dst == M_SLREG ? (reg & 3) == 1 : (reg & 1) == 1;
    }
    const auto postMode = flags.src2();
    if (postMode < M_PMAC)
        return true;
    // Manual A-6 footnotes: no RPT has an indexed form, and the relocating
    // MACs have no immediate one.
    const auto post = entry->readPost();
    if (postMode == M_PRPT && insn.aa() == AA_IDX)
        return false;
    if (postMode == M_PMAC && insn.aa() == AA_IMM && (post & MAC_RELOC))
        return false;
    const auto selector = insn.peekOperand(postPos(insn, flags));
    if (!insn.isOK())
        return false;
    // MVAC and MSAC carry the selector in the low bits of their destination.
    if (postMode == M_PACC)
        return (selector & 3) == post;
    if (selector != post)
        return false;
    // A repeat also has to name RPT_CNT, or it is just an AND.
    return postMode == M_PMAC || insn.peekOperand(postPos(insn, flags) + 1) == RPT_CNT;
}

Error searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) {
    const auto entry = cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    if (entry && entry->readFlags().undefined()) {
        insn.nameBuffer().reset();
        insn.setErrorIf(UNKNOWN_INSTRUCTION);
    }
    return insn.getError();
}

bool isPrefix(CpuType cpuType, Config::opcode_t code) {
    return cpu(cpuType)->isPrefix(code);
}

const /*PROGMEM*/ char *TableI8096::listCpu_P() const {
    return TEXT_CPU_LIST;
}

const /*PROGMEM*/ char *TableI8096::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableI8096::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    name.iexpect('i');
    auto t = Cpu::search(name, ARRAY_RANGE(CPU_TABLE));
    if (t) {
        cpuType = t->readCpuType();
        return OK;
    }
    return UNSUPPORTED_CPU;
}

bool EntryInsn::mulDivInsn() const {
    const auto pre = prefix();
    if (pre == 0 || pre == 0xFE) {
        const auto opc = opCode();
        const auto opc_lo = opc & 0x0F;
        const auto opc_hi = opc & 0xF0;
        return opc_lo >= 0x0C && (opc_hi >= 0x40 && opc_hi <= 0x90);
    }
    return false;
}

bool EntryInsn::mulWordInsn() const {
    const auto opc_hi = opCode() & 0xF0;
    return mulDivInsn() && (opc_hi == 0x40 || opc_hi == 0x60);
}

const TableI8096 TABLE;

}  // namespace i8096
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

/*
 * Copyright 2025 Tadashi G. Takaoka
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

#include "table_z280.h"
#include "table_z80.hpp"
#include "text_z280.h"

namespace libasm {
namespace z280 {

using namespace libasm::text::z280;
using namespace libasm::z80::common;

// clang-format off
constexpr Entry TABLE_Z280[] PROGMEM = {
    E1(0x03, 0060, TEXT_INCW, M_R16),
    E1(0x0B, 0060, TEXT_DECW, M_R16),
    E1(0x80, 0007, TEXT_ADD,  M_SRC),
    E1(0x88, 0007, TEXT_ADC,  M_SRC),
    E1(0x98, 0007, TEXT_SBC,  M_SRC),
    E1(0xC6, 0000, TEXT_ADD,  M_IM8),
    E1(0xCE, 0000, TEXT_ADC,  M_IM8),
    E1(0xDE, 0000, TEXT_SBC,  M_IM8),
    E1(0x27, 0000, TEXT_DAA,  R_A),
    E1(0x2F, 0000, TEXT_CPL,  R_A),
    E2(0xF9, 0000, TEXT_LDW,  R_SP,  R_HL),
};

constexpr uint8_t INDEX_Z280[] PROGMEM = {
      3,  // TEXT_ADC
      6,  // TEXT_ADC
      2,  // TEXT_ADD
      5,  // TEXT_ADD
      9,  // TEXT_CPL
      8,  // TEXT_DAA
      1,  // TEXT_DECW
      0,  // TEXT_INCW
     10,  // TEXT_LDW
      4,  // TEXT_SBC
      7,  // TEXT_SBC
};

// Z280 ED:xx
constexpr Entry TABLE_Z280_EXT[] PROGMEM = {
    E2(0x02, 0070, TEXT_LDA,    R_HL,   M_FIDX),
    E2(0x03, 0070, TEXT_LD,     M_FIDX, R_A),
    E2(0x04, 0070, TEXT_LDW,    R_HL,   M_FIDX),
    E2(0x04, 0070, TEXT_LD,     R_HL,   M_FIDX),
    E2(0x05, 0070, TEXT_LDW,    M_FIDX, R_HL),
    E2(0x05, 0070, TEXT_LD,     M_FIDX, R_HL),
    E2(0x06, 0060, TEXT_LDW,    M_R16,  I_HL),
    E2(0x07, 0070, TEXT_EX,     R_A,    M_DST),
    E2(0x07, 0070, TEXT_EX,     M_DST,  R_A),
    E2(0x06, 0060, TEXT_LDW,    M_R16,  I_HL),
    E2(0x06, 0060, TEXT_LD,     M_R16,  I_HL),
    E2(0x0E, 0060, TEXT_LDW,    I_HL,   M_R16),
    E2(0x0E, 0060, TEXT_LD,     I_HL,   M_R16),
    E2(0x43, 0020, TEXT_LD,     M_ABS,  R_PTRH),
    E2(0x73, 0000, TEXT_LD,     M_ABS,  R_SP),
    E2(0x43, 0020, TEXT_LDW,    M_ABS,  R_PTRH),
    E2(0x73, 0000, TEXT_LDW,    M_ABS,  R_SP),
    E1(0x44, 0000, TEXT_NEG,    R_A),
    E2(0x4B, 0020, TEXT_LD,     R_PTRH, M_ABS),
    E2(0x7B, 0000, TEXT_LD,     R_SP,   M_ABS),
    E2(0x4B, 0020, TEXT_LDW,    R_PTRH, M_ABS),
    E2(0x7B, 0000, TEXT_LDW,    R_SP,   M_ABS),
    E1(0x4C, 0000, TEXT_NEG,    R_HL),
    E2(0x63, 0000, TEXT_LDW,    M_ABS,  R_HL),
    I2(0x6B, 0000, TEXT_LDW,    R_HL,   M_DABS),
    E2(0xC0, 0070, TEXT_MULT,   R_A,    M_DST),
    E2(0xC1, 0070, TEXT_MULTU,  R_A,    M_DST),
    E2(0xC2, 0060, TEXT_MULTW,  R_HL,   M_R16),
    E2(0xC3, 0060, TEXT_MULTUW, R_HL,   M_R16),
    E2(0xC4, 0070, TEXT_DIV,    R_HL,   M_DST),
    E2(0xC5, 0070, TEXT_DIVU,   R_HL,   M_DST),
    E2(0xC6, 0060, TEXT_ADDW,   R_HL,   M_R16),
    E2(0xC7, 0060, TEXT_CPW,    R_HL,   M_R16),
    E2(0xCA, 0060, TEXT_DIVW,   R_DEHL, M_R16),
    E2(0xCB, 0060, TEXT_DIVUW,  R_DEHL, M_R16),
    E2(0xCE, 0060, TEXT_SUBW,   R_HL,   M_R16),
    E2(0xEF, 0000, TEXT_EX,     R_H,    R_L),
    E2(0xEF, 0000, TEXT_EX,     R_L,    R_H),
    E1(0xC0, 0070, TEXT_MULT,   M_DST),
    E1(0xC1, 0070, TEXT_MULTU,  M_DST),
    E1(0xC2, 0060, TEXT_MULTW,  M_R16),
    E1(0xC3, 0060, TEXT_MULTUW, M_R16),
    E1(0xC4, 0070, TEXT_DIV,    M_DST),
    E1(0xC5, 0070, TEXT_DIVU,   M_DST),
    E1(0xC6, 0060, TEXT_ADDW,   M_R16),
    E1(0xC7, 0060, TEXT_CPW,    M_R16),
    E1(0xCA, 0060, TEXT_DIVW,   M_R16),
    E1(0xCB, 0060, TEXT_DIVUW,  M_R16),
    E1(0xCE, 0060, TEXT_SUBW,   M_R16),
    E0(0x55, 0000, TEXT_RETIL),
    E1(0x64, 0000, TEXT_EXTS,   R_A),
    E0(0x64, 0000, TEXT_EXTS),
    E0(0x65, 0000, TEXT_PCACHE),
    E2(0x66, 0000, TEXT_LDCTL,  R_HL,   I_C),
    E1(0x6C, 0000, TEXT_EXTS,   R_HL),
    E2(0x6D, 0000, TEXT_ADD,    R_HL,   R_A),
    E2(0x6E, 0000, TEXT_LDCTL,  I_C,    R_HL),
    E1(0x70, 0000, TEXT_TSTI,   I_C),
    E1(0x71, 0000, TEXT_SC,     M_IM16),
    E1(0x77, 0000, TEXT_DI,     M_IM8),
    E1(0x7F, 0000, TEXT_EI,     M_IM8),
    E0(0x82, 0000, TEXT_INIW),
    E0(0x83, 0000, TEXT_OUTIW),
    E2(0x86, 0000, TEXT_LDUD,   R_A,    I_HL),
    E2(0x87, 0000, TEXT_LDCTL,  R_HL,   R_USP),
    E0(0x8A, 0000, TEXT_INDW),
    E0(0x8B, 0000, TEXT_OUTDW),
    E2(0x8E, 0000, TEXT_LDUD,   I_HL,   R_A),
    E2(0x8F, 0000, TEXT_LDCTL,  R_USP,  R_HL),
    E0(0x92, 0000, TEXT_INIRW),
    E0(0x93, 0000, TEXT_OTIRW),
    E2(0x96, 0000, TEXT_LDUP,   R_A,    I_HL),
    E0(0x9A, 0000, TEXT_INDRW),
    E0(0x9B, 0000, TEXT_OTDRW),
    E2(0x9E, 0000, TEXT_LDUP,   I_HL,   R_A),
    E2(0xB7, 0000, TEXT_INW,    R_HL,   I_C),
    E2(0xB7, 0000, TEXT_IN,     R_HL,   I_C),
    E2(0xBF, 0000, TEXT_OUTW,   I_C,    R_HL),
    E2(0xBF, 0000, TEXT_OUT,    I_C,    R_HL),
    E1(0x97, 0000, TEXT_EPUF,   M_EPU),
    E1(0x9F, 0000, TEXT_EPUI,   M_EPU),
    E2(0x84, 0070, TEXT_EPUM,   M_FIDX, M_EPU),
    E2(0x85, 0070, TEXT_MEPU,   M_FIDX, M_EPU),
    E2(0xA6, 0000, TEXT_EPUM,   I_HL,   M_EPU),
    E2(0xA7, 0000, TEXT_EPUM,   M_ABS,  M_EPU),
    E2(0xAE, 0000, TEXT_MEPU,   I_HL,   M_EPU),
    E2(0xAF, 0000, TEXT_MEPU,   M_ABS,  M_EPU),
};

constexpr uint8_t INDEX_Z280_EXT[] PROGMEM = {
     55,  // TEXT_ADD
     31,  // TEXT_ADDW
     44,  // TEXT_ADDW
     32,  // TEXT_CPW
     45,  // TEXT_CPW
     59,  // TEXT_DI
     29,  // TEXT_DIV
     42,  // TEXT_DIV
     30,  // TEXT_DIVU
     43,  // TEXT_DIVU
     34,  // TEXT_DIVUW
     47,  // TEXT_DIVUW
     33,  // TEXT_DIVW
     46,  // TEXT_DIVW
     60,  // TEXT_EI
     79,  // TEXT_EPUF
     80,  // TEXT_EPUI
     81,  // TEXT_EPUM
     83,  // TEXT_EPUM
     84,  // TEXT_EPUM
      7,  // TEXT_EX
      8,  // TEXT_EX
     36,  // TEXT_EX
     37,  // TEXT_EX
     50,  // TEXT_EXTS
     51,  // TEXT_EXTS
     54,  // TEXT_EXTS
     76,  // TEXT_IN
     72,  // TEXT_INDRW
     65,  // TEXT_INDW
     69,  // TEXT_INIRW
     61,  // TEXT_INIW
     75,  // TEXT_INW
      1,  // TEXT_LD
      3,  // TEXT_LD
      5,  // TEXT_LD
     10,  // TEXT_LD
     12,  // TEXT_LD
     13,  // TEXT_LD
     14,  // TEXT_LD
     18,  // TEXT_LD
     19,  // TEXT_LD
      0,  // TEXT_LDA
     53,  // TEXT_LDCTL
     56,  // TEXT_LDCTL
     64,  // TEXT_LDCTL
     68,  // TEXT_LDCTL
     63,  // TEXT_LDUD
     67,  // TEXT_LDUD
     71,  // TEXT_LDUP
     74,  // TEXT_LDUP
      2,  // TEXT_LDW
      4,  // TEXT_LDW
      6,  // TEXT_LDW
      9,  // TEXT_LDW
     11,  // TEXT_LDW
     15,  // TEXT_LDW
     16,  // TEXT_LDW
     20,  // TEXT_LDW
     21,  // TEXT_LDW
     23,  // TEXT_LDW
     24,  // TEXT_LDW
     82,  // TEXT_MEPU
     85,  // TEXT_MEPU
     86,  // TEXT_MEPU
     25,  // TEXT_MULT
     38,  // TEXT_MULT
     26,  // TEXT_MULTU
     39,  // TEXT_MULTU
     28,  // TEXT_MULTUW
     41,  // TEXT_MULTUW
     27,  // TEXT_MULTW
     40,  // TEXT_MULTW
     17,  // TEXT_NEG
     22,  // TEXT_NEG
     73,  // TEXT_OTDRW
     70,  // TEXT_OTIRW
     78,  // TEXT_OUT
     66,  // TEXT_OUTDW
     62,  // TEXT_OUTIW
     77,  // TEXT_OUTW
     52,  // TEXT_PCACHE
     49,  // TEXT_RETIL
     58,  // TEXT_SC
     35,  // TEXT_SUBW
     48,  // TEXT_SUBW
     57,  // TEXT_TSTI
};

// Z280 DD:xx
constexpr Entry TABLE_Z280_IX[] PROGMEM = {
    E2(0x01, 0020, TEXT_LDW,  M_MPTR, M_IM16),
    E1(0x03, 0020, TEXT_INCW, M_MPTR),
    E1(0x04, 0030, TEXT_INC,  M_BIXH),
    E1(0x05, 0030, TEXT_DEC,  M_BIXH),
    E2(0x06, 0030, TEXT_LD,   M_BIXH,  M_IM8),
    E1(0x0B, 0020, TEXT_DECW, M_MPTR),
    E1(0x20, 0000, TEXT_JAR,  M_REL8),
    E1(0x28, 0000, TEXT_JAF,  M_REL8),
    E2(0x31, 0000, TEXT_LDW,  M_REL16, M_IM16),
    E1(0x33, 0000, TEXT_INCW, M_REL16),
    E1(0x3B, 0000, TEXT_DECW, M_REL16),
    E1(0x3C, 0000, TEXT_INC,  M_ABS),
    E1(0x3D, 0000, TEXT_DEC,  M_ABS),
    E2(0x3E, 0000, TEXT_LD,   M_ABS,   M_IM8),
    E2(0x78, 0003, TEXT_LD,   R_A,     M_BIXL),
    E2(0x80, 0003, TEXT_ADD,  R_A,     M_BIXL),
    E2(0x88, 0003, TEXT_ADC,  R_A,     M_BIXL),
    E2(0x90, 0003, TEXT_SUB,  R_A,     M_BIXL),
    E2(0x98, 0003, TEXT_SBC,  R_A,     M_BIXL),
    E2(0xA0, 0003, TEXT_AND,  R_A,     M_BIXL),
    E2(0xA8, 0003, TEXT_XOR,  R_A,     M_BIXL),
    E2(0xB0, 0003, TEXT_OR,   R_A,     M_BIXL),
    E2(0xB8, 0003, TEXT_CP,   R_A,     M_BIXL),
    E1(0x78, 0003, TEXT_LD,   M_BIXL),
    E1(0x80, 0003, TEXT_ADD,  M_BIXL),
    E1(0x88, 0003, TEXT_ADC,  M_BIXL),
    E1(0x90, 0003, TEXT_SUB,  M_BIXL),
    E1(0x98, 0003, TEXT_SBC,  M_BIXL),
    E1(0xA0, 0003, TEXT_AND,  M_BIXL),
    E1(0xA8, 0003, TEXT_XOR,  M_BIXL),
    E1(0xB0, 0003, TEXT_OR,   M_BIXL),
    E1(0xB8, 0003, TEXT_CP,   M_BIXL),
    E1(0xC1, 0020, TEXT_POP,  M_MPTR),
    E2(0xC2, 0070, TEXT_JP,   M_CC8,   I_HL),
    E2(0xC4, 0070, TEXT_CALL, M_CC8,   I_HL),
    E1(0xC5, 0020, TEXT_PUSH, M_MPTR),
    E1(0xCD, 0000, TEXT_CALL, I_HL),
    E1(0xF1, 0000, TEXT_POP,  M_REL16),
    E1(0xF5, 0000, TEXT_PUSH, M_REL16),
};

constexpr uint8_t INDEX_Z280_IX[] PROGMEM = {
     16,  // TEXT_ADC
     25,  // TEXT_ADC
     15,  // TEXT_ADD
     24,  // TEXT_ADD
     19,  // TEXT_AND
     28,  // TEXT_AND
     34,  // TEXT_CALL
     36,  // TEXT_CALL
     22,  // TEXT_CP
     31,  // TEXT_CP
      3,  // TEXT_DEC
     12,  // TEXT_DEC
      5,  // TEXT_DECW
     10,  // TEXT_DECW
      2,  // TEXT_INC
     11,  // TEXT_INC
      1,  // TEXT_INCW
      9,  // TEXT_INCW
      7,  // TEXT_JAF
      6,  // TEXT_JAR
     33,  // TEXT_JP
      4,  // TEXT_LD
     13,  // TEXT_LD
     14,  // TEXT_LD
     23,  // TEXT_LD
      0,  // TEXT_LDW
      8,  // TEXT_LDW
     21,  // TEXT_OR
     30,  // TEXT_OR
     32,  // TEXT_POP
     37,  // TEXT_POP
     35,  // TEXT_PUSH
     38,  // TEXT_PUSH
     18,  // TEXT_SBC
     27,  // TEXT_SBC
     17,  // TEXT_SUB
     26,  // TEXT_SUB
     20,  // TEXT_XOR
     29,  // TEXT_XOR
};

// Z280 FD:xx
constexpr Entry TABLE_Z280_IY[] PROGMEM = {
    E1(0x03, 0020, TEXT_INCW, M_IDX16),
    E1(0x04, 0030, TEXT_INC,  M_PIXH),
    E1(0x05, 0030, TEXT_DEC,  M_PIXH),
    E2(0x06, 0030, TEXT_LD,   M_PIXH, M_IM8),
    E1(0x0B, 0020, TEXT_DECW, M_IDX16),
    E2(0x78, 0003, TEXT_LD,   R_A,    M_PIXL),
    E2(0x80, 0003, TEXT_ADD,  R_A,    M_PIXL),
    E2(0x88, 0003, TEXT_ADC,  R_A,    M_PIXL),
    E2(0x90, 0003, TEXT_SUB,  R_A,    M_PIXL),
    E2(0x98, 0003, TEXT_SBC,  R_A,    M_PIXL),
    E2(0xA0, 0003, TEXT_AND,  R_A,    M_PIXL),
    E2(0xA8, 0003, TEXT_XOR,  R_A,    M_PIXL),
    E2(0xB0, 0003, TEXT_OR,   R_A,    M_PIXL),
    E2(0xB8, 0003, TEXT_CP,   R_A,    M_PIXL),
    E1(0x78, 0003, TEXT_LD,   M_PIXL),
    E1(0x80, 0003, TEXT_ADD,  M_PIXL),
    E1(0x88, 0003, TEXT_ADC,  M_PIXL),
    E1(0x90, 0003, TEXT_SUB,  M_PIXL),
    E1(0x98, 0003, TEXT_SBC,  M_PIXL),
    E1(0xA0, 0003, TEXT_AND,  M_PIXL),
    E1(0xA8, 0003, TEXT_XOR,  M_PIXL),
    E1(0xB0, 0003, TEXT_OR,   M_PIXL),
    E1(0xB8, 0003, TEXT_CP,   M_PIXL),
    E2(0xC2, 0070, TEXT_JP,   M_CC8,  M_REL16),
    E1(0xC3, 0000, TEXT_JP,   M_REL16),
    E2(0xC4, 0070, TEXT_CALL, M_CC8,  M_REL16),
    E1(0xCD, 0000, TEXT_CALL, M_REL16),
    E1(0xF5, 0000, TEXT_PUSH, M_IM16),
};

constexpr uint8_t INDEX_Z280_IY[] PROGMEM = {
      7,  // TEXT_ADC
     16,  // TEXT_ADC
      6,  // TEXT_ADD
     15,  // TEXT_ADD
     10,  // TEXT_AND
     19,  // TEXT_AND
     25,  // TEXT_CALL
     26,  // TEXT_CALL
     13,  // TEXT_CP
     22,  // TEXT_CP
      2,  // TEXT_DEC
      4,  // TEXT_DECW
      1,  // TEXT_INC
      0,  // TEXT_INCW
     23,  // TEXT_JP
     24,  // TEXT_JP
      3,  // TEXT_LD
      5,  // TEXT_LD
     14,  // TEXT_LD
     12,  // TEXT_OR
     21,  // TEXT_OR
     27,  // TEXT_PUSH
      9,  // TEXT_SBC
     18,  // TEXT_SBC
      8,  // TEXT_SUB
     17,  // TEXT_SUB
     11,  // TEXT_XOR
     20,  // TEXT_XOR
};

// Z280 CB:xx
constexpr Entry TABLE_Z280_BIT[] PROGMEM = {
    E1(0x30, 0007, TEXT_TSET, M_SRC),
};

constexpr uint8_t INDEX_Z280_BIT[] PROGMEM = {
      0,  // TEXT_TSET
};

// Z280 DD:xx, FD:xx
constexpr Entry TABLE_Z280_IDX[] PROGMEM = {
    E1(0x24, 0010, TEXT_INC,  R_DXY),
    E1(0x25, 0010, TEXT_DEC,  R_DXY),
    E1(0x23, 0000, TEXT_INCW, R_IDX),
    E1(0x2B, 0000, TEXT_DECW, R_IDX),
    E2(0x26, 0010, TEXT_LD,   R_DXY,  M_IM8),
    E2(0x64, 0011, TEXT_LD,   R_DXY,  R_SXY),
    E2(0x44, 0071, TEXT_LD,   M_DR8X, R_SXY),
    E2(0x60, 0017, TEXT_LD,   R_DXY,  M_SR8X),
    E2(0xEB, 0000, TEXT_EX,   R_IDX,  R_HL),
    E2(0x84, 0001, TEXT_ADD,  R_A,    R_SXY),
    E1(0x86, 0000, TEXT_ADD,  M_IDX8),
    E1(0x8E, 0000, TEXT_ADC,  M_IDX8),
    E1(0x9E, 0000, TEXT_SBC,  M_IDX8),
    E2(0x8C, 0001, TEXT_ADC,  R_A,    R_SXY),
    E2(0x94, 0001, TEXT_SUB,  R_A,    R_SXY),
    E2(0x9C, 0001, TEXT_SBC,  R_A,    R_SXY),
    E2(0xA4, 0001, TEXT_AND,  R_A,    R_SXY),
    E2(0xAC, 0001, TEXT_XOR,  R_A,    R_SXY),
    E2(0xB4, 0001, TEXT_OR,   R_A,    R_SXY),
    E2(0xBC, 0001, TEXT_CP,   R_A,    R_SXY),
    E1(0x84, 0001, TEXT_ADD,  R_SXY),
    E1(0x8C, 0001, TEXT_ADC,  R_SXY),
    E1(0x94, 0001, TEXT_SUB,  R_SXY),
    E1(0x9C, 0001, TEXT_SBC,  R_SXY),
    E1(0xA4, 0001, TEXT_AND,  R_SXY),
    E1(0xAC, 0001, TEXT_XOR,  R_SXY),
    E1(0xB4, 0001, TEXT_OR,   R_SXY),
    E1(0xBC, 0001, TEXT_CP,   R_SXY),
    E2(0xEB, 0000, TEXT_EX,   R_HL,   R_IDX),
    E2(0xF9, 0000, TEXT_LDW,  R_SP,   R_IDX),
};

constexpr uint8_t INDEX_Z280_IDX[] PROGMEM = {
     11,  // TEXT_ADC
     13,  // TEXT_ADC
     21,  // TEXT_ADC
      9,  // TEXT_ADD
     10,  // TEXT_ADD
     20,  // TEXT_ADD
     16,  // TEXT_AND
     24,  // TEXT_AND
     19,  // TEXT_CP
     27,  // TEXT_CP
      1,  // TEXT_DEC
      3,  // TEXT_DECW
      8,  // TEXT_EX
     28,  // TEXT_EX
      0,  // TEXT_INC
      2,  // TEXT_INCW
      4,  // TEXT_LD
      5,  // TEXT_LD
      6,  // TEXT_LD
      7,  // TEXT_LD
     29,  // TEXT_LDW
     18,  // TEXT_OR
     26,  // TEXT_OR
     12,  // TEXT_SBC
     15,  // TEXT_SBC
     23,  // TEXT_SBC
     14,  // TEXT_SUB
     22,  // TEXT_SUB
     17,  // TEXT_XOR
     25,  // TEXT_XOR
};

// Z280 DD:ED:xx, FD:ED:xx
constexpr Entry TABLE_Z280_IDXEXT[] PROGMEM = {
    E2(0x02, 0070, TEXT_LDA,    R_IDX,  M_FIDX),
    E2(0x04, 0070, TEXT_LDW,    R_IDX,  M_FIDX),
    E2(0x04, 0070, TEXT_LD,     R_IDX,  M_FIDX),
    E2(0x05, 0070, TEXT_LDW,    M_FIDX, R_IDX),
    E2(0x05, 0070, TEXT_LD,     M_FIDX, R_IDX),
    E2(0x06, 0060, TEXT_LDW,    M_R16,  M_IDX8),
    E2(0x06, 0060, TEXT_LD,     M_R16,  M_IDX8),
    E2(0x0E, 0060, TEXT_LDW,    M_IDX8, M_R16),
    E2(0x0E, 0060, TEXT_LD,     M_IDX8, M_R16),
    E2(0x27, 0010, TEXT_EX,     R_A,    R_DXY),
    E2(0x27, 0010, TEXT_EX,     R_DXY,  R_A),
    E2(0x37, 0000, TEXT_EX,     R_A,    M_IDX8),
    E2(0x37, 0000, TEXT_EX,     M_IDX8, R_A),
    E2(0x42, 0060, TEXT_SBC,    R_IDX,  M_R16X),
    E2(0x4A, 0060, TEXT_ADC,    R_IDX,  M_R16X),
    E2(0x60, 0010, TEXT_IN,     R_DXY,  I_C),
    E2(0x61, 0010, TEXT_OUT,    I_C,    R_DXY),
    E2(0x66, 0000, TEXT_LDCTL,  R_IDX,  I_C),
    E2(0x6D, 0000, TEXT_ADD,    R_IDX,  R_A),
    E2(0x6E, 0000, TEXT_LDCTL,  I_C,    R_IDX),
    E2(0x86, 0000, TEXT_LDUD,   R_A,    M_IDX8),
    E2(0x87, 0000, TEXT_LDCTL,  R_IDX,  R_USP),
    E2(0x8E, 0000, TEXT_LDUD,   M_IDX8, R_A),
    E2(0x8F, 0000, TEXT_LDCTL,  R_USP,  R_IDX),
    E2(0x96, 0000, TEXT_LDUP,   R_A,    M_IDX8),
    E2(0x9E, 0000, TEXT_LDUP,   M_IDX8, R_A),
    E2(0xE0, 0010, TEXT_MULT,   R_A,    R_DXY),
    E2(0xE1, 0010, TEXT_MULTU,  R_A,    R_DXY),
    E2(0xE2, 0000, TEXT_MULTW,  R_HL,   R_IDX),
    E2(0xE3, 0000, TEXT_MULTUW, R_HL,   R_IDX),
    E2(0xE4, 0010, TEXT_DIV,    R_HL,   R_DXY),
    E2(0xE5, 0010, TEXT_DIVU,   R_HL,   R_DXY),
    E2(0xE6, 0000, TEXT_ADDW,   R_HL,   R_IDX),
    E2(0xE7, 0000, TEXT_CPW,    R_HL,   R_IDX),
    E2(0xEA, 0000, TEXT_DIVW,   R_DEHL, R_IDX),
    E2(0xEB, 0000, TEXT_DIVUW,  R_DEHL, R_IDX),
    E2(0xEE, 0000, TEXT_SUBW,   R_HL,   R_IDX),
    E2(0xF0, 0000, TEXT_MULT,   R_A,    M_IDX8),
    E2(0xF1, 0000, TEXT_MULTU,  R_A,    M_IDX8),
    E2(0xF4, 0000, TEXT_DIV,    R_HL,   M_IDX8),
    E2(0xF5, 0000, TEXT_DIVU,   R_HL,   M_IDX8),
    E1(0xE0, 0010, TEXT_MULT,   R_DXY),
    E1(0xE1, 0010, TEXT_MULTU,  R_DXY),
    E1(0xE2, 0000, TEXT_MULTW,  R_IDX),
    E1(0xE3, 0000, TEXT_MULTUW, R_IDX),
    E1(0xE4, 0010, TEXT_DIV,    R_DXY),
    E1(0xE5, 0010, TEXT_DIVU,   R_DXY),
    E1(0xE6, 0000, TEXT_ADDW,   R_IDX),
    E1(0xE7, 0000, TEXT_CPW,    R_IDX),
    E1(0xEA, 0000, TEXT_DIVW,   R_IDX),
    E1(0xEB, 0000, TEXT_DIVUW,  R_IDX),
    E1(0xEE, 0000, TEXT_SUBW,   R_IDX),
    E1(0xF0, 0000, TEXT_MULT,   M_IDX8),
    E1(0xF1, 0000, TEXT_MULTU,  M_IDX8),
    E1(0xF4, 0000, TEXT_DIV,    M_IDX8),
    E1(0xF5, 0000, TEXT_DIVU,   M_IDX8),
};

constexpr uint8_t INDEX_Z280_IDXEXT[] PROGMEM = {
     14,  // TEXT_ADC
     18,  // TEXT_ADD
     32,  // TEXT_ADDW
     47,  // TEXT_ADDW
     33,  // TEXT_CPW
     48,  // TEXT_CPW
     30,  // TEXT_DIV
     39,  // TEXT_DIV
     45,  // TEXT_DIV
     54,  // TEXT_DIV
     31,  // TEXT_DIVU
     40,  // TEXT_DIVU
     46,  // TEXT_DIVU
     55,  // TEXT_DIVU
     35,  // TEXT_DIVUW
     50,  // TEXT_DIVUW
     34,  // TEXT_DIVW
     49,  // TEXT_DIVW
      9,  // TEXT_EX
     10,  // TEXT_EX
     11,  // TEXT_EX
     12,  // TEXT_EX
     15,  // TEXT_IN
      2,  // TEXT_LD
      4,  // TEXT_LD
      6,  // TEXT_LD
      8,  // TEXT_LD
      0,  // TEXT_LDA
     17,  // TEXT_LDCTL
     19,  // TEXT_LDCTL
     21,  // TEXT_LDCTL
     23,  // TEXT_LDCTL
     20,  // TEXT_LDUD
     22,  // TEXT_LDUD
     24,  // TEXT_LDUP
     25,  // TEXT_LDUP
      1,  // TEXT_LDW
      3,  // TEXT_LDW
      5,  // TEXT_LDW
      7,  // TEXT_LDW
     26,  // TEXT_MULT
     37,  // TEXT_MULT
     41,  // TEXT_MULT
     52,  // TEXT_MULT
     27,  // TEXT_MULTU
     38,  // TEXT_MULTU
     42,  // TEXT_MULTU
     53,  // TEXT_MULTU
     29,  // TEXT_MULTUW
     44,  // TEXT_MULTUW
     28,  // TEXT_MULTW
     43,  // TEXT_MULTW
     16,  // TEXT_OUT
     13,  // TEXT_SBC
     36,  // TEXT_SUBW
     51,  // TEXT_SUBW
};

constexpr Entry TABLE_Z280_IXEXT[] PROGMEM = {
    E2(0x07, 0030, TEXT_EX,     R_A,    M_BIXH),
    E2(0x07, 0000, TEXT_EX,     M_BIXH, R_A),
    E2(0x3F, 0000, TEXT_EX,     R_A,    M_ABS),
    E2(0x3F, 0000, TEXT_EX,     M_ABS,  R_A),
    E2(0x40, 0030, TEXT_IN,     M_BIXH, I_C),
    E2(0x41, 0030, TEXT_OUT,    I_C,    M_BIXH),
    E2(0x78, 0000, TEXT_IN,     M_ABS,  I_C),
    E2(0x79, 0000, TEXT_OUT,    I_C,    M_ABS),
    E2(0xC0, 0030, TEXT_MULT,   R_A,    M_BIXH),
    E2(0xC1, 0030, TEXT_MULTU,  R_A,    M_BIXH),
    E2(0xC2, 0020, TEXT_MULTW,  R_HL,   M_MPTR),
    E2(0xC3, 0020, TEXT_MULTUW, R_HL,   M_MPTR),
    E2(0xC4, 0030, TEXT_DIV,    R_HL,   M_BIXH),
    E2(0xC5, 0030, TEXT_DIVU,   R_HL,   M_BIXH),
    E2(0xC6, 0020, TEXT_ADDW,   R_HL,   M_MPTR),
    E2(0xC7, 0020, TEXT_CPW,    R_HL,   M_MPTR),
    E2(0xCA, 0020, TEXT_DIVW,   R_DEHL, M_MPTR),
    E2(0xCB, 0020, TEXT_DIVUW,  R_DEHL, M_MPTR),
    E2(0xCE, 0020, TEXT_SUBW,   R_HL,   M_MPTR),
    E2(0xF2, 0000, TEXT_MULTW,  R_HL,   M_REL16),
    E2(0xF3, 0000, TEXT_MULTUW, R_HL,   M_REL16),
    E2(0xF6, 0000, TEXT_ADDW,   R_HL,   M_REL16),
    E2(0xF7, 0000, TEXT_CPW,    R_HL,   M_REL16),
    E2(0xF8, 0000, TEXT_MULT,   R_A,    M_ABS),
    E2(0xF9, 0000, TEXT_MULTU,  R_A,    M_ABS),
    E2(0xFA, 0000, TEXT_DIVW,   R_DEHL, M_REL16),
    E2(0xFB, 0000, TEXT_DIVUW,  R_DEHL, M_REL16),
    E2(0xFC, 0000, TEXT_DIV,    R_HL,   M_ABS),
    E2(0xFD, 0000, TEXT_DIVU,   R_HL,   M_ABS),
    E2(0xFE, 0000, TEXT_SUBW,   R_HL,   M_REL16),
    E1(0xC0, 0030, TEXT_MULT,   M_BIXH),
    E1(0xC1, 0030, TEXT_MULTU,  M_BIXH),
    E1(0xC2, 0020, TEXT_MULTW,  M_MPTR),
    E1(0xC3, 0020, TEXT_MULTUW, M_MPTR),
    E1(0xC4, 0030, TEXT_DIV,    M_BIXH),
    E1(0xC5, 0030, TEXT_DIVU,   M_BIXH),
    E1(0xC6, 0020, TEXT_ADDW,   M_MPTR),
    E1(0xC7, 0020, TEXT_CPW,    M_MPTR),
    E1(0xCA, 0020, TEXT_DIVW,   M_MPTR),
    E1(0xCB, 0020, TEXT_DIVUW,  M_MPTR),
    E1(0xCE, 0020, TEXT_SUBW,   M_MPTR),
    E1(0xF2, 0000, TEXT_MULTW,  M_REL16),
    E1(0xF3, 0000, TEXT_MULTUW, M_REL16),
    E1(0xF6, 0000, TEXT_ADDW,   M_REL16),
    E1(0xF7, 0000, TEXT_CPW,    M_REL16),
    E1(0xF8, 0000, TEXT_MULT,   M_ABS),
    E1(0xF9, 0000, TEXT_MULTU,  M_ABS),
    E1(0xFA, 0000, TEXT_DIVW,   M_REL16),
    E1(0xFB, 0000, TEXT_DIVUW,  M_REL16),
    E1(0xFC, 0000, TEXT_DIV,    M_ABS),
    E1(0xFD, 0000, TEXT_DIVU,   M_ABS),
    E1(0xFE, 0000, TEXT_SUBW,   M_REL16),
};

constexpr uint8_t INDEX_Z280_IXEXT[] PROGMEM = {
     14,  // TEXT_ADDW
     21,  // TEXT_ADDW
     36,  // TEXT_ADDW
     43,  // TEXT_ADDW
     15,  // TEXT_CPW
     22,  // TEXT_CPW
     37,  // TEXT_CPW
     44,  // TEXT_CPW
     12,  // TEXT_DIV
     27,  // TEXT_DIV
     34,  // TEXT_DIV
     49,  // TEXT_DIV
     13,  // TEXT_DIVU
     28,  // TEXT_DIVU
     35,  // TEXT_DIVU
     50,  // TEXT_DIVU
     17,  // TEXT_DIVUW
     26,  // TEXT_DIVUW
     39,  // TEXT_DIVUW
     48,  // TEXT_DIVUW
     16,  // TEXT_DIVW
     25,  // TEXT_DIVW
     38,  // TEXT_DIVW
     47,  // TEXT_DIVW
      0,  // TEXT_EX
      1,  // TEXT_EX
      2,  // TEXT_EX
      3,  // TEXT_EX
      4,  // TEXT_IN
      6,  // TEXT_IN
      8,  // TEXT_MULT
     23,  // TEXT_MULT
     30,  // TEXT_MULT
     45,  // TEXT_MULT
      9,  // TEXT_MULTU
     24,  // TEXT_MULTU
     31,  // TEXT_MULTU
     46,  // TEXT_MULTU
     11,  // TEXT_MULTUW
     20,  // TEXT_MULTUW
     33,  // TEXT_MULTUW
     42,  // TEXT_MULTUW
     10,  // TEXT_MULTW
     19,  // TEXT_MULTW
     32,  // TEXT_MULTW
     41,  // TEXT_MULTW
      5,  // TEXT_OUT
      7,  // TEXT_OUT
     18,  // TEXT_SUBW
     29,  // TEXT_SUBW
     40,  // TEXT_SUBW
     51,  // TEXT_SUBW
};

constexpr Entry TABLE_Z280_IYEXT[] PROGMEM = {
    E2(0x07, 0030, TEXT_EX,     R_A,    M_PIXH),
    E2(0x40, 0030, TEXT_IN,     M_PIXH, I_C),
    E2(0x41, 0030, TEXT_OUT,    I_C,    M_PIXH),
    E2(0xC0, 0030, TEXT_MULT,   R_A,    M_PIXH),
    E2(0xC1, 0030, TEXT_MULTU,  R_A,    M_PIXH),
    E2(0xC2, 0020, TEXT_MULTW,  R_HL,   M_IDX16),
    E2(0xC3, 0020, TEXT_MULTUW, R_HL,   M_IDX16),
    E2(0xC4, 0030, TEXT_DIV,    R_HL,   M_PIXH),
    E2(0xC5, 0030, TEXT_DIVU,   R_HL,   M_PIXH),
    E2(0xC6, 0020, TEXT_ADDW,   R_HL,   M_IDX16),
    E2(0xC7, 0020, TEXT_CPW,    R_HL,   M_IDX16),
    E2(0xCA, 0020, TEXT_DIVW,   R_DEHL, M_IDX16),
    E2(0xCB, 0020, TEXT_DIVUW,  R_DEHL, M_IDX16),
    E2(0xCE, 0020, TEXT_SUBW,   R_HL,   M_IDX16),
    E2(0xF2, 0000, TEXT_MULTW,  R_HL,   M_IM16),
    E2(0xF3, 0000, TEXT_MULTUW, R_HL,   M_IM16),
    E2(0xF6, 0000, TEXT_ADDW,   R_HL,   M_IM16),
    E2(0xF7, 0000, TEXT_CPW,    R_HL,   M_IM16),
    E2(0xF8, 0000, TEXT_MULT,   R_A,    M_IM8),
    E2(0xF9, 0000, TEXT_MULTU,  R_A,    M_IM8),
    E2(0xFA, 0000, TEXT_DIVW,   R_DEHL, M_IM16),
    E2(0xFB, 0000, TEXT_DIVUW,  R_DEHL, M_IM16),
    E2(0xFC, 0000, TEXT_DIV,    R_HL,   M_IM8),
    E2(0xFD, 0000, TEXT_DIVU,   R_HL,   M_IM8),
    E2(0xFE, 0000, TEXT_SUBW,   R_HL,   M_IM16),
    E1(0xC0, 0030, TEXT_MULT,   M_PIXH),
    E1(0xC1, 0030, TEXT_MULTU,  M_PIXH),
    E1(0xC2, 0020, TEXT_MULTW,  M_IDX16),
    E1(0xC3, 0020, TEXT_MULTUW, M_IDX16),
    E1(0xC4, 0030, TEXT_DIV,    M_PIXH),
    E1(0xC5, 0030, TEXT_DIVU,   M_PIXH),
    E1(0xC6, 0020, TEXT_ADDW,   M_IDX16),
    E1(0xC7, 0020, TEXT_CPW,    M_IDX16),
    E1(0xCA, 0020, TEXT_DIVW,   M_IDX16),
    E1(0xCB, 0020, TEXT_DIVUW,  M_IDX16),
    E1(0xCE, 0020, TEXT_SUBW,   M_IDX16),
    E1(0xF2, 0000, TEXT_MULTW,  M_IM16),
    E1(0xF3, 0000, TEXT_MULTUW, M_IM16),
    E1(0xF6, 0000, TEXT_ADDW,   M_IM16),
    E1(0xF7, 0000, TEXT_CPW,    M_IM16),
    E1(0xF8, 0000, TEXT_MULT,   M_IM8),
    E1(0xF9, 0000, TEXT_MULTU,  M_IM8),
    E1(0xFA, 0000, TEXT_DIVW,   M_IM16),
    E1(0xFB, 0000, TEXT_DIVUW,  M_IM16),
    E1(0xFC, 0000, TEXT_DIV,    M_IM8),
    E1(0xFD, 0000, TEXT_DIVU,   M_IM8),
    E1(0xFE, 0000, TEXT_SUBW,   M_IM16),
};

constexpr uint8_t INDEX_Z280_IYEXT[] PROGMEM = {
      9,  // TEXT_ADDW
     16,  // TEXT_ADDW
     31,  // TEXT_ADDW
     38,  // TEXT_ADDW
     10,  // TEXT_CPW
     17,  // TEXT_CPW
     32,  // TEXT_CPW
     39,  // TEXT_CPW
      7,  // TEXT_DIV
     22,  // TEXT_DIV
     29,  // TEXT_DIV
     44,  // TEXT_DIV
      8,  // TEXT_DIVU
     23,  // TEXT_DIVU
     30,  // TEXT_DIVU
     45,  // TEXT_DIVU
     12,  // TEXT_DIVUW
     21,  // TEXT_DIVUW
     34,  // TEXT_DIVUW
     43,  // TEXT_DIVUW
     11,  // TEXT_DIVW
     20,  // TEXT_DIVW
     33,  // TEXT_DIVW
     42,  // TEXT_DIVW
      0,  // TEXT_EX
      1,  // TEXT_IN
      3,  // TEXT_MULT
     18,  // TEXT_MULT
     25,  // TEXT_MULT
     40,  // TEXT_MULT
      4,  // TEXT_MULTU
     19,  // TEXT_MULTU
     26,  // TEXT_MULTU
     41,  // TEXT_MULTU
      6,  // TEXT_MULTUW
     15,  // TEXT_MULTUW
     28,  // TEXT_MULTUW
     37,  // TEXT_MULTUW
      5,  // TEXT_MULTW
     14,  // TEXT_MULTW
     27,  // TEXT_MULTW
     36,  // TEXT_MULTW
      2,  // TEXT_OUT
     13,  // TEXT_SUBW
     24,  // TEXT_SUBW
     35,  // TEXT_SUBW
     46,  // TEXT_SUBW
};

// Z280 DD:CB:dd:xx, FD:CB:dd:xx
constexpr Entry TABLE_Z280_IDXBIT[] PROGMEM = {
    E1(0x36, 0000, TEXT_TSET, M_IDX),
};

constexpr uint8_t INDEX_Z280_IDXBIT[] PROGMEM = {
      0,  // TEXT_TSET
};

// clang-format on

using EntryPage = entry::PrefixTableBase<Entry>;

constexpr EntryPage Z280_PAGES[] PROGMEM = {
        {0x0000, ARRAY_RANGE(TABLE_Z80), ARRAY_RANGE(INDEX_Z80)},
        {0x0000, ARRAY_RANGE(TABLE_Z280), ARRAY_RANGE(INDEX_Z280)},
        {Entry::IX, ARRAY_RANGE(TABLE_Z80_IDX), ARRAY_RANGE(INDEX_Z80_IDX)},
        {Entry::IY, ARRAY_RANGE(TABLE_Z80_IDX), ARRAY_RANGE(INDEX_Z80_IDX)},
        {Entry::IX, ARRAY_RANGE(TABLE_Z280_IDX), ARRAY_RANGE(INDEX_Z280_IDX)},
        {Entry::IY, ARRAY_RANGE(TABLE_Z280_IDX), ARRAY_RANGE(INDEX_Z280_IDX)},
        {Entry::IX, ARRAY_RANGE(TABLE_Z280_IX), ARRAY_RANGE(INDEX_Z280_IX)},
        {Entry::IY, ARRAY_RANGE(TABLE_Z280_IY), ARRAY_RANGE(INDEX_Z280_IY)},
        {Entry::EXT, ARRAY_RANGE(TABLE_Z80_EXT), ARRAY_RANGE(INDEX_Z80_EXT)},
        {Entry::EXT, ARRAY_RANGE(TABLE_Z280_EXT), ARRAY_RANGE(INDEX_Z280_EXT)},
        {Entry::IXEXT, ARRAY_RANGE(TABLE_Z280_IDXEXT), ARRAY_RANGE(INDEX_Z280_IDXEXT)},
        {Entry::IYEXT, ARRAY_RANGE(TABLE_Z280_IDXEXT), ARRAY_RANGE(INDEX_Z280_IDXEXT)},
        {Entry::IXEXT, ARRAY_RANGE(TABLE_Z280_IXEXT), ARRAY_RANGE(INDEX_Z280_IXEXT)},
        {Entry::IYEXT, ARRAY_RANGE(TABLE_Z280_IYEXT), ARRAY_RANGE(INDEX_Z280_IYEXT)},
        {Entry::BIT, ARRAY_RANGE(TABLE_Z80_BIT), ARRAY_RANGE(INDEX_Z80_BIT)},
        {Entry::BIT, ARRAY_RANGE(TABLE_Z280_BIT), ARRAY_RANGE(INDEX_Z280_BIT)},
        {Entry::IXBIT, ARRAY_RANGE(TABLE_Z80_IDXBIT), ARRAY_RANGE(INDEX_Z80_IDXBIT)},
        {Entry::IYBIT, ARRAY_RANGE(TABLE_Z80_IDXBIT), ARRAY_RANGE(INDEX_Z80_IDXBIT)},
        {Entry::IXBIT, ARRAY_RANGE(TABLE_Z280_IDXBIT), ARRAY_RANGE(INDEX_Z280_IDXBIT)},
        {Entry::IYBIT, ARRAY_RANGE(TABLE_Z280_IDXBIT), ARRAY_RANGE(INDEX_Z280_IDXBIT)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {Z80, TEXT_CPU_Z80, ARRAY_RANGE(Z80_PAGES)},
        {Z280, TEXT_CPU_Z280, ARRAY_RANGE(Z280_PAGES)},
};

const Cpu *cpu(CpuType cpuType) {
    return Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
}

bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_SR8 || opr == R_A)
        return table == M_SRC || table == M_DST || table == M_SR8 || table == M_DR8 ||
               table == M_SR8X || table == M_DR8X || table == R_H || table == R_L;
    if (opr == I_HL)
        return table == M_SRC || table == M_DST || table == M_MPTR;
    if (opr == R_C)
        return table == M_SRC || table == M_DST || table == M_SR8 || table == M_DR8 ||
               table == M_SR8X || table == M_DR8X || table == M_CC4 || table == M_CC8;
    if (opr == R_H || opr == R_L)
        return table == M_SRC || table == M_DST || table == M_SR8 || table == M_DR8 ||
               table == M_SR8X || table == M_DR8X;
    if (opr == R_DXY)
        return table == R_SXY;
    if (opr == R_BC || opr == R_DE)
        return table == M_R16 || table == M_R16X || table == R_PTRH;
    if (opr == R_HL)
        return table == M_R16 || table == M_R16X;
    if (opr == R_SP)
        return table == M_R16 || table == M_R16X;
    if (opr == R_IX || opr == R_IY)
        return table == R_IDX || table == M_R16X;
    if (opr == M_REL16 || opr == M_PCDX)
        return table == M_FIDX || table == M_PIXH || table == M_PIXL;
    if (opr == M_IDX8)
        return table == M_FIDX || table == M_PIXH || table == M_PIXL || table == M_IDX;
    if (opr == M_IDX16)
        return table == M_FIDX || table == M_PIXH || table == M_PIXL || table == M_IDX;
    if (opr == M_PDX)
        return table == M_FIDX || table == M_PIXH || table == M_PIXL;
    if (opr == M_SPDX)
        return table == M_FIDX || table == M_BIXH || table == M_BIXL;
    if (opr == M_BDX)
        return table == M_FIDX || table == M_BIXH || table == M_BIXL;
    if (opr == M_IM16)
        return table == M_IM8 || table == M_REL8 || table == M_BIT || table == M_VEC ||
               table == M_IMMD || table == M_EPU || table == M_LM16 || table == M_JABS;
    if (opr == M_ABS)
        return table == M_IOA || table == M_MPTR || table == M_DABS;
    if (opr == M_CC4)
        return table == M_CC8;
    return false;
}

bool acceptModes(AsmInsn &insn, const Entry *entry) {
    const auto table = entry->readFlags();
    return acceptMode(insn.dstOp.mode, table.dst()) && acceptMode(insn.srcOp.mode, table.src());
}

Error searchName(CpuType cpuType, AsmInsn &insn) {
    cpu(cpuType)->searchName(insn, acceptModes);
    return insn.getError();
}

bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *) {
    auto opc = insn.opCode();
    const auto flags = entry->readFlags();
    const auto dst = flags.dst();
    const auto src = flags.src();
    if (dst == M_DR8 || src == M_DR8 || dst == M_DR8X) {
        const auto reg = (opc >> 3) & 7;
        if (reg == 6 || (dst == M_DR8X && (reg == 4 || reg == 5)))
            return false;
    }
    if (src == M_SR8 || src == M_SR8X || dst == M_SR8) {
        const auto reg = opc & 7;
        if (reg == 6 || (src == M_SR8X && (reg == 4 || reg == 5)))
            return false;
    }
    opc &= ~flags.mask();
    return opc == entry->readOpCode();
}

Error searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) {
    cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    return insn.getError();
}

bool isPrefix(CpuType cpuType, Config::prefix_t code) {
    return cpu(cpuType)->isPrefix(code);
}

bool EntryInsn::ixBit() const {
    return (prefix() == Entry::IXBIT || prefix() == Entry::IYBIT);
}

const /*PROGMEM*/ char *TableZ280::listCpu_P() const {
    return TEXT_Z280_LIST;
}

const /*PROGMEM*/ char *TableZ280::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableZ280::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    auto t = Cpu::search(name, ARRAY_RANGE(CPU_TABLE));
    if (t) {
        cpuType = t->readCpuType();
        return OK;
    }
    return UNSUPPORTED_CPU;
}

const TableZ280 TABLE;

}  // namespace z280
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

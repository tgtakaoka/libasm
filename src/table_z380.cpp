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

#include "table_z380.h"
#include "table_z180.hpp"
#include "text_z380.h"

namespace libasm {
namespace z380 {

using namespace libasm::text::z380;
using namespace libasm::z80::common;

constexpr Entry TABLE_Z380[] PROGMEM = {
    E1(0x0B, 0060, TEXT_DECW, M_R16),
    E1(0x03, 0060, TEXT_INCW, M_R16),
    E1(0x2F, 0000, TEXT_CPL,  R_A),
    E1(0x10, 0000, TEXT_DJNZ, M_REL16),
    E1(0x18, 0000, TEXT_JR,   M_REL16),
    E2(0x20, 0030, TEXT_JR,   M_CC4,  M_REL16),
    D2(0x22, 0000, TEXT_LDW,  M_DABS, R_HL),
    D2(0x2A, 0000, TEXT_LDW,  R_HL,   M_DABS),
    E1(0x80, 0007, TEXT_ADD,  M_SRC),
    E1(0x88, 0007, TEXT_ADC,  M_SRC),
    E1(0x98, 0007, TEXT_SBC,  M_SRC),
    E1(0xC6, 0000, TEXT_ADD,  M_IM8),
    E1(0xCE, 0000, TEXT_ADC,  M_IM8),
    E1(0xDE, 0000, TEXT_SBC,  M_IM8),
    E1(0x27, 0000, TEXT_DAA,  R_A),
    E1(0x2F, 0000, TEXT_CPL,  R_A),
};

constexpr uint8_t INDEX_Z380[] PROGMEM = {
      9,  // TEXT_ADC
     12,  // TEXT_ADC
      8,  // TEXT_ADD
     11,  // TEXT_ADD
      2,  // TEXT_CPL
     15,  // TEXT_CPL
     14,  // TEXT_DAA
      0,  // TEXT_DECW
      3,  // TEXT_DJNZ
      1,  // TEXT_INCW
      4,  // TEXT_JR
      5,  // TEXT_JR
      6,  // TEXT_LDW
      7,  // TEXT_LDW
     10,  // TEXT_SBC
     13,  // TEXT_SBC
};

// Z380 ED:xx
constexpr Entry TABLE_Z380_EXT[] PROGMEM = {
    E2(0xC4, 0070, TEXT_CALR,  M_CC8,   M_REL8),
    E1(0xCD, 0000, TEXT_CALR,  M_REL8),
    L2(0x05, 0000, TEXT_EX,    R_BC,    R_DE),
    L2(0x05, 0000, TEXT_EX,    R_DE,    R_BC),
    E2(0x07, 0070, TEXT_EX,    R_A,     M_DST),
    E2(0x07, 0070, TEXT_EX,    M_DR8,   R_A),
    L2(0x0D, 0000, TEXT_EX,    R_BC,    R_HL),
    L2(0x0D, 0000, TEXT_EX,    R_HL,    R_BC),
    D2(0x06, 0020, TEXT_LDW,   I_PTR,   M_DM16),
    D2(0x36, 0000, TEXT_LDW,   I_HL,    M_DM16),
    L2(0x02, 0020, TEXT_LDW,   R_PTRH,  R_BC),
    L2(0x32, 0000, TEXT_LDW,   R_HL,    R_BC),
    L2(0x02, 0020, TEXT_LD,    R_PTRH,  R_BC),
    L2(0x32, 0000, TEXT_LD,    R_HL,    R_BC),
    L2(0x03, 0020, TEXT_EX,    R_PTRH,  R_IX),
    L2(0x03, 0020, TEXT_EX,    R_IX,    R_PTRH),
    L2(0x33, 0000, TEXT_EX,    R_HL,    R_IX),
    L2(0x33, 0000, TEXT_EX,    R_IX,    R_HL),
    L2(0x0B, 0020, TEXT_EX,    R_PTRH,  R_IY),
    L2(0x0B, 0020, TEXT_EX,    R_IY,    R_PTRH),
    L2(0x3B, 0000, TEXT_EX,    R_HL,    R_IY),
    L2(0x3B, 0000, TEXT_EX,    R_IY,    R_HL),
    L2(0x2B, 0000, TEXT_EX,    R_IX,    R_IY),
    L2(0x2B, 0000, TEXT_EX,    R_IY,    R_IX),
    E1(0x0E, 0020, TEXT_SWAP,  R_PTRH),
    E1(0x3E, 0000, TEXT_SWAP,  R_HL),
    I2(0x63, 0000, TEXT_LDW,   M_DABS,  R_HL),
    L1(0x65, 0000, TEXT_EXTS,  R_A),
    E1(0x75, 0000, TEXT_EXTSW, R_HL),
    E1(0x44, 0000, TEXT_NEG,   R_A),
    E1(0x54, 0000, TEXT_NEGW,  R_HL),
    D2(0x6B, 0000, TEXT_LDW,   R_HL,    M_DABS),
    E2(0x84, 0001, TEXT_ADDW,  R_HL,    R_PTRL),
    E2(0x86, 0000, TEXT_ADDW,  R_HL,    M_IM16),
    E2(0x87, 0000, TEXT_ADDW,  R_HL,    R_HL),
    E2(0x8C, 0001, TEXT_ADCW,  R_HL,    R_PTRL),
    E2(0x8E, 0000, TEXT_ADCW,  R_HL,    M_IM16),
    E2(0x8F, 0000, TEXT_ADCW,  R_HL,    R_HL),
    E2(0x94, 0001, TEXT_SUBW,  R_HL,    R_PTRL),
    E2(0x96, 0000, TEXT_SUBW,  R_HL,    M_IM16),
    E2(0x97, 0000, TEXT_SUBW,  R_HL,    R_HL),
    E2(0x9C, 0001, TEXT_SBCW,  R_HL,    R_PTRL),
    E2(0x9E, 0000, TEXT_SBCW,  R_HL,    M_IM16),
    E2(0x9F, 0000, TEXT_SBCW,  R_HL,    R_HL),
    E2(0xA4, 0001, TEXT_ANDW,  R_HL,    R_PTRL),
    E2(0xA6, 0000, TEXT_ANDW,  R_HL,    M_IM16),
    E2(0xA7, 0000, TEXT_ANDW,  R_HL,    R_HL),
    E2(0xAC, 0001, TEXT_XORW,  R_HL,    R_PTRL),
    E2(0xAE, 0000, TEXT_XORW,  R_HL,    M_IM16),
    E2(0xAF, 0000, TEXT_XORW,  R_HL,    R_HL),
    E2(0xB4, 0001, TEXT_ORW,   R_HL,    R_PTRL),
    E2(0xB6, 0000, TEXT_ORW,   R_HL,    M_IM16),
    E2(0xB7, 0000, TEXT_ORW,   R_HL,    R_HL),
    E2(0xBC, 0001, TEXT_CPW,   R_HL,    R_PTRL),
    E2(0xBE, 0000, TEXT_CPW,   R_HL,    M_IM16),
    E2(0xBF, 0000, TEXT_CPW,   R_HL,    R_HL),
    I2(0x82, 0000, TEXT_ADD,   R_SP,    M_XM16),
    I2(0x92, 0000, TEXT_SUB,   R_SP,    M_XM16),
    L2(0xC0, 0000, TEXT_LDCTL, R_HL,    R_SR),
    L2(0xC8, 0000, TEXT_LDCTL, R_SR,    R_HL),
    I2(0xC6, 0000, TEXT_ADD,   R_HL,    M_XABS),
    I2(0xD6, 0000, TEXT_SUB,   R_HL,    M_XABS),
    E2(0xC4, 0070, TEXT_CALR,  M_CC8,   M_REL24),
    E1(0xCD, 0000, TEXT_CALR,  M_REL24),
    E1(0xF7, 0000, TEXT_SETC,  M_LW),
    E1(0xFF, 0000, TEXT_RESC,  M_LW),
    E0(0x55, 0000, TEXT_RETB),
    E0(0x8B, 0000, TEXT_OTDM),
    E0(0x9B, 0000, TEXT_OTDMR),
    E0(0xCF, 0000, TEXT_BTEST),
    L1(0xC1, 0000, TEXT_POP,   R_SR),
    L1(0xC5, 0000, TEXT_PUSH,  R_SR),
    E2(0xD0, 0000, TEXT_LDCTL, R_A,     R_DSR),
    E2(0xD8, 0000, TEXT_LDCTL, R_DSR,   R_A),
    E2(0xDA, 0000, TEXT_LDCTL, R_DSR,   M_IM8),
    E0(0xD9, 0000, TEXT_EXALL),
    E1(0x30, 0000, TEXT_IN0,   M_IOA),
    E2(0x71, 0000, TEXT_OUT,   I_C,     M_IM8),
    I2(0xD3, 0000, TEXT_OUTA,  M_IO16,  R_A),
    I2(0xDB, 0000, TEXT_INA,   R_A,     M_IO16),
    E0(0x76, 0000, TEXT_SLP),
    L0(0xE0, 0000, TEXT_LDIW),
    E0(0xE2, 0000, TEXT_INIW),
    E0(0xE3, 0000, TEXT_OUTIW),
    L0(0xE8, 0000, TEXT_LDDW),
    E0(0xEB, 0000, TEXT_OUTDW),
    L0(0xF0, 0000, TEXT_LDIRW),
    E0(0xF2, 0000, TEXT_INIRW),
    E0(0xF3, 0000, TEXT_OTIRW),
    L0(0xF8, 0000, TEXT_LDDRW),
    E0(0xFB, 0000, TEXT_OTDRW),
    E0(0xEA, 0000, TEXT_INDW),
    E0(0xFA, 0000, TEXT_INDRW),
    L0(0x65, 0000, TEXT_EXTS),
    E0(0x75, 0000, TEXT_EXTSW),
    E0(0x54, 0000, TEXT_NEGW),
    E1(0x84, 0001, TEXT_ADDW,  R_PTRL),
    E1(0x86, 0000, TEXT_ADDW,  M_IM16),
    E1(0x87, 0000, TEXT_ADDW,  R_HL),
    E1(0x8C, 0001, TEXT_ADCW,  R_PTRL),
    E1(0x8E, 0000, TEXT_ADCW,  M_IM16),
    E1(0x8F, 0000, TEXT_ADCW,  R_HL),
    E1(0x94, 0001, TEXT_SUBW,  R_PTRL),
    E1(0x96, 0000, TEXT_SUBW,  M_IM16),
    E1(0x97, 0000, TEXT_SUBW,  R_HL),
    E1(0x9C, 0001, TEXT_SBCW,  R_PTRL),
    E1(0x9E, 0000, TEXT_SBCW,  M_IM16),
    E1(0x9F, 0000, TEXT_SBCW,  R_HL),
    E1(0xA4, 0001, TEXT_ANDW,  R_PTRL),
    E1(0xA6, 0000, TEXT_ANDW,  M_IM16),
    E1(0xA7, 0000, TEXT_ANDW,  R_HL),
    E1(0xAC, 0001, TEXT_XORW,  R_PTRL),
    E1(0xAE, 0000, TEXT_XORW,  M_IM16),
    E1(0xAF, 0000, TEXT_XORW,  R_HL),
    E1(0xB4, 0001, TEXT_ORW,   R_PTRL),
    E1(0xB6, 0000, TEXT_ORW,   M_IM16),
    E1(0xB7, 0000, TEXT_ORW,   R_HL),
    E1(0xBC, 0001, TEXT_CPW,   R_PTRL),
    E1(0xBE, 0000, TEXT_CPW,   M_IM16),
    E1(0xBF, 0000, TEXT_CPW,   R_HL),
};

constexpr uint8_t INDEX_Z380_EXT[] PROGMEM = {
     35,  // TEXT_ADCW
     36,  // TEXT_ADCW
     37,  // TEXT_ADCW
     99,  // TEXT_ADCW
    100,  // TEXT_ADCW
    101,  // TEXT_ADCW
     56,  // TEXT_ADD
     60,  // TEXT_ADD
     32,  // TEXT_ADDW
     33,  // TEXT_ADDW
     34,  // TEXT_ADDW
     96,  // TEXT_ADDW
     97,  // TEXT_ADDW
     98,  // TEXT_ADDW
     44,  // TEXT_ANDW
     45,  // TEXT_ANDW
     46,  // TEXT_ANDW
    108,  // TEXT_ANDW
    109,  // TEXT_ANDW
    110,  // TEXT_ANDW
     69,  // TEXT_BTEST
      0,  // TEXT_CALR
      1,  // TEXT_CALR
     62,  // TEXT_CALR
     63,  // TEXT_CALR
     53,  // TEXT_CPW
     54,  // TEXT_CPW
     55,  // TEXT_CPW
    117,  // TEXT_CPW
    118,  // TEXT_CPW
    119,  // TEXT_CPW
      2,  // TEXT_EX
      3,  // TEXT_EX
      4,  // TEXT_EX
      5,  // TEXT_EX
      6,  // TEXT_EX
      7,  // TEXT_EX
     14,  // TEXT_EX
     15,  // TEXT_EX
     16,  // TEXT_EX
     17,  // TEXT_EX
     18,  // TEXT_EX
     19,  // TEXT_EX
     20,  // TEXT_EX
     21,  // TEXT_EX
     22,  // TEXT_EX
     23,  // TEXT_EX
     75,  // TEXT_EXALL
     27,  // TEXT_EXTS
     93,  // TEXT_EXTS
     28,  // TEXT_EXTSW
     94,  // TEXT_EXTSW
     76,  // TEXT_IN0
     79,  // TEXT_INA
     92,  // TEXT_INDRW
     91,  // TEXT_INDW
     87,  // TEXT_INIRW
     82,  // TEXT_INIW
     12,  // TEXT_LD
     13,  // TEXT_LD
     58,  // TEXT_LDCTL
     59,  // TEXT_LDCTL
     72,  // TEXT_LDCTL
     73,  // TEXT_LDCTL
     74,  // TEXT_LDCTL
     89,  // TEXT_LDDRW
     84,  // TEXT_LDDW
     86,  // TEXT_LDIRW
     81,  // TEXT_LDIW
      8,  // TEXT_LDW
      9,  // TEXT_LDW
     10,  // TEXT_LDW
     11,  // TEXT_LDW
     26,  // TEXT_LDW
     31,  // TEXT_LDW
     29,  // TEXT_NEG
     30,  // TEXT_NEGW
     95,  // TEXT_NEGW
     50,  // TEXT_ORW
     51,  // TEXT_ORW
     52,  // TEXT_ORW
    114,  // TEXT_ORW
    115,  // TEXT_ORW
    116,  // TEXT_ORW
     67,  // TEXT_OTDM
     68,  // TEXT_OTDMR
     90,  // TEXT_OTDRW
     88,  // TEXT_OTIRW
     77,  // TEXT_OUT
     78,  // TEXT_OUTA
     85,  // TEXT_OUTDW
     83,  // TEXT_OUTIW
     70,  // TEXT_POP
     71,  // TEXT_PUSH
     65,  // TEXT_RESC
     66,  // TEXT_RETB
     41,  // TEXT_SBCW
     42,  // TEXT_SBCW
     43,  // TEXT_SBCW
    105,  // TEXT_SBCW
    106,  // TEXT_SBCW
    107,  // TEXT_SBCW
     64,  // TEXT_SETC
     80,  // TEXT_SLP
     57,  // TEXT_SUB
     61,  // TEXT_SUB
     38,  // TEXT_SUBW
     39,  // TEXT_SUBW
     40,  // TEXT_SUBW
    102,  // TEXT_SUBW
    103,  // TEXT_SUBW
    104,  // TEXT_SUBW
     24,  // TEXT_SWAP
     25,  // TEXT_SWAP
     47,  // TEXT_XORW
     48,  // TEXT_XORW
     49,  // TEXT_XORW
    111,  // TEXT_XORW
    112,  // TEXT_XORW
    113,  // TEXT_XORW
};

// Z380 DD:xx, FD:xx
constexpr Entry TABLE_Z380_IDX[] PROGMEM = {
    I1(0x34, 0000, TEXT_INC,  M_IDX),
    I1(0x35, 0000, TEXT_DEC,  M_IDX),
    I2(0x36, 0000, TEXT_LD,   M_IDX,  M_IM8),
    I2(0x46, 0070, TEXT_LD,   M_DR8,  M_IDX),
    I2(0x70, 0007, TEXT_LD,   M_IDX,  M_SR8),
    I2(0x86, 0000, TEXT_ADD,  R_A,    M_IDX),
    I2(0x8E, 0000, TEXT_ADC,  R_A,    M_IDX),
    I2(0x96, 0000, TEXT_SUB,  R_A,    M_IDX),
    I2(0x9E, 0000, TEXT_SBC,  R_A,    M_IDX),
    I2(0xA6, 0000, TEXT_AND,  R_A,    M_IDX),
    I2(0xAE, 0000, TEXT_XOR,  R_A,    M_IDX),
    I2(0xB6, 0000, TEXT_OR,   R_A,    M_IDX),
    I2(0xBE, 0000, TEXT_CP,   R_A,    M_IDX),
    L2(0x01, 0020, TEXT_LDW,  I_PTR,  R_IDX),
    L2(0x31, 0000, TEXT_LDW,  I_HL,   R_IDX),
    L2(0x03, 0020, TEXT_LDW,  R_IDX,  I_PTR),
    L2(0x33, 0000, TEXT_LDW,  R_IDX,  I_HL),
    L2(0x07, 0020, TEXT_LDW,  R_IDX,  R_PTRH),
    L2(0x37, 0000, TEXT_LDW,  R_IDX,  R_HL),
    L2(0x0B, 0020, TEXT_LDW,  R_PTRH, R_IDX),
    L2(0x3B, 0000, TEXT_LDW,  R_HL,   R_IDX),
    L2(0x01, 0020, TEXT_LD,   I_PTR,  R_IDX),
    L2(0x31, 0000, TEXT_LD,   I_HL,   R_IDX),
    L2(0x03, 0020, TEXT_LD,   R_IDX,  I_PTR),
    L2(0x33, 0000, TEXT_LD,   R_IDX,  I_HL),
    L2(0x07, 0020, TEXT_LD,   R_IDX,  R_PTRH),
    L2(0x37, 0000, TEXT_LD,   R_IDX,  R_HL),
    L2(0x0B, 0020, TEXT_LD,   R_PTRH, R_IDX),
    L2(0x3B, 0000, TEXT_LD,   R_HL,   R_IDX),
    E1(0x23, 0000, TEXT_INC,  R_IDX),
    E1(0x23, 0000, TEXT_INCW, R_IDX),
    E1(0x24, 0010, TEXT_INC,  R_DXY),
    E2(0x26, 0010, TEXT_LD,   R_DXY,  M_IM8),
    E2(0x44, 0071, TEXT_LD,   M_DR8X, R_SXY),
    E2(0x64, 0011, TEXT_LD,   R_DXY,  R_SXY),
    E2(0x60, 0017, TEXT_LD,   R_DXY,  M_SR8X),
    E2(0x67, 0010, TEXT_LD,   R_DXY,  R_A),
    E2(0x7C, 0001, TEXT_LD,   R_A,    R_SXY),
    E2(0x84, 0001, TEXT_ADD,  R_A,    R_SXY),
    E2(0x8C, 0001, TEXT_ADC,  R_A,    R_SXY),
    E2(0x94, 0001, TEXT_SUB,  R_A,    R_SXY),
    E2(0x9C, 0001, TEXT_SBC,  R_A,    R_SXY),
    E2(0xA4, 0001, TEXT_AND,  R_A,    R_SXY),
    E2(0xAC, 0001, TEXT_XOR,  R_A,    R_SXY),
    E2(0xB4, 0001, TEXT_OR,   R_A,    R_SXY),
    E2(0xBC, 0001, TEXT_CP,   R_A,    R_SXY),
    E1(0x25, 0010, TEXT_DEC,  R_DXY),
    E1(0x2B, 0000, TEXT_DEC,  R_IDX),
    E1(0x2B, 0000, TEXT_DECW, R_IDX),
    E2(0x87, 0000, TEXT_ADDW, R_HL,   R_IDX),
    E2(0x8F, 0000, TEXT_ADCW, R_HL,   R_IDX),
    E2(0x97, 0000, TEXT_SUBW, R_HL,   R_IDX),
    E2(0x9F, 0000, TEXT_SBCW, R_HL,   R_IDX),
    E2(0xA7, 0000, TEXT_ANDW, R_HL,   R_IDX),
    E2(0xAF, 0000, TEXT_XORW, R_HL,   R_IDX),
    E2(0xB7, 0000, TEXT_ORW,  R_HL,   R_IDX),
    E2(0xBF, 0000, TEXT_CPW,  R_HL,   R_IDX),
    I2(0xC6, 0000, TEXT_ADDW, R_HL,   M_IDX),
    I2(0xCE, 0000, TEXT_ADCW, R_HL,   M_IDX),
    I2(0xD6, 0000, TEXT_SUBW, R_HL,   M_IDX),
    I2(0xDE, 0000, TEXT_SBCW, R_HL,   M_IDX),
    I2(0xE6, 0000, TEXT_ANDW, R_HL,   M_IDX),
    I2(0xEE, 0000, TEXT_XORW, R_HL,   M_IDX),
    I2(0xF6, 0000, TEXT_ORW,  R_HL,   M_IDX),
    I2(0xFE, 0000, TEXT_CPW,  R_HL,   M_IDX),
    I1(0x86, 0000, TEXT_ADD,  M_IDX),
    I1(0x8E, 0000, TEXT_ADC,  M_IDX),
    I1(0x96, 0000, TEXT_SUB,  M_IDX),
    I1(0x9E, 0000, TEXT_SBC,  M_IDX),
    I1(0x96, 0000, TEXT_SUB,  M_IDX),
    I1(0xA6, 0000, TEXT_AND,  M_IDX),
    I1(0xAE, 0000, TEXT_XOR,  M_IDX),
    I1(0xB6, 0000, TEXT_OR,   M_IDX),
    I1(0xBE, 0000, TEXT_CP,   M_IDX),
    E1(0x84, 0001, TEXT_ADD,  R_SXY),
    E1(0x8C, 0001, TEXT_ADC,  R_SXY),
    E1(0x94, 0001, TEXT_SUB,  R_SXY),
    E1(0x9C, 0001, TEXT_SBC,  R_SXY),
    E1(0xA4, 0001, TEXT_AND,  R_SXY),
    E1(0xAC, 0001, TEXT_XOR,  R_SXY),
    E1(0xB4, 0001, TEXT_OR,   R_SXY),
    E1(0xBC, 0001, TEXT_CP,   R_SXY),
    E1(0x3E, 0000, TEXT_SWAP, R_IDX),
    E1(0x8F, 0000, TEXT_ADCW, R_IDX),
    E1(0x87, 0000, TEXT_ADDW, R_IDX),
    E1(0x97, 0000, TEXT_SUBW, R_IDX),
    E1(0x9F, 0000, TEXT_SBCW, R_IDX),
    E1(0xA7, 0000, TEXT_ANDW, R_IDX),
    E1(0xAF, 0000, TEXT_XORW, R_IDX),
    E1(0xB7, 0000, TEXT_ORW,  R_IDX),
    E1(0xBF, 0000, TEXT_CPW,  R_IDX),
    I1(0xC6, 0000, TEXT_ADDW, M_IDX),
    I1(0xCE, 0000, TEXT_ADCW, M_IDX),
    I1(0xD6, 0000, TEXT_SUBW, M_IDX),
    I1(0xDE, 0000, TEXT_SBCW, M_IDX),
    I1(0xE6, 0000, TEXT_ANDW, M_IDX),
    I1(0xEE, 0000, TEXT_XORW, M_IDX),
    I1(0xF6, 0000, TEXT_ORW,  M_IDX),
    I1(0xFE, 0000, TEXT_CPW,  M_IDX),
};

constexpr uint8_t INDEX_Z380_IDX[] PROGMEM = {
      6,  // TEXT_ADC
     39,  // TEXT_ADC
     66,  // TEXT_ADC
     75,  // TEXT_ADC
     50,  // TEXT_ADCW
     58,  // TEXT_ADCW
     83,  // TEXT_ADCW
     92,  // TEXT_ADCW
      5,  // TEXT_ADD
     38,  // TEXT_ADD
     65,  // TEXT_ADD
     74,  // TEXT_ADD
     49,  // TEXT_ADDW
     57,  // TEXT_ADDW
     84,  // TEXT_ADDW
     91,  // TEXT_ADDW
      9,  // TEXT_AND
     42,  // TEXT_AND
     70,  // TEXT_AND
     78,  // TEXT_AND
     53,  // TEXT_ANDW
     61,  // TEXT_ANDW
     87,  // TEXT_ANDW
     95,  // TEXT_ANDW
     12,  // TEXT_CP
     45,  // TEXT_CP
     73,  // TEXT_CP
     81,  // TEXT_CP
     56,  // TEXT_CPW
     64,  // TEXT_CPW
     90,  // TEXT_CPW
     98,  // TEXT_CPW
      1,  // TEXT_DEC
     46,  // TEXT_DEC
     47,  // TEXT_DEC
     48,  // TEXT_DECW
      0,  // TEXT_INC
     29,  // TEXT_INC
     31,  // TEXT_INC
     30,  // TEXT_INCW
      2,  // TEXT_LD
      3,  // TEXT_LD
      4,  // TEXT_LD
     21,  // TEXT_LD
     22,  // TEXT_LD
     23,  // TEXT_LD
     24,  // TEXT_LD
     25,  // TEXT_LD
     26,  // TEXT_LD
     27,  // TEXT_LD
     28,  // TEXT_LD
     32,  // TEXT_LD
     33,  // TEXT_LD
     34,  // TEXT_LD
     35,  // TEXT_LD
     36,  // TEXT_LD
     37,  // TEXT_LD
     13,  // TEXT_LDW
     14,  // TEXT_LDW
     15,  // TEXT_LDW
     16,  // TEXT_LDW
     17,  // TEXT_LDW
     18,  // TEXT_LDW
     19,  // TEXT_LDW
     20,  // TEXT_LDW
     11,  // TEXT_OR
     44,  // TEXT_OR
     72,  // TEXT_OR
     80,  // TEXT_OR
     55,  // TEXT_ORW
     63,  // TEXT_ORW
     89,  // TEXT_ORW
     97,  // TEXT_ORW
      8,  // TEXT_SBC
     41,  // TEXT_SBC
     68,  // TEXT_SBC
     77,  // TEXT_SBC
     52,  // TEXT_SBCW
     60,  // TEXT_SBCW
     86,  // TEXT_SBCW
     94,  // TEXT_SBCW
      7,  // TEXT_SUB
     40,  // TEXT_SUB
     67,  // TEXT_SUB
     69,  // TEXT_SUB
     76,  // TEXT_SUB
     51,  // TEXT_SUBW
     59,  // TEXT_SUBW
     85,  // TEXT_SUBW
     93,  // TEXT_SUBW
     82,  // TEXT_SWAP
     10,  // TEXT_XOR
     43,  // TEXT_XOR
     71,  // TEXT_XOR
     79,  // TEXT_XOR
     54,  // TEXT_XORW
     62,  // TEXT_XORW
     88,  // TEXT_XORW
     96,  // TEXT_XORW
};

// Z380 DD:xx
constexpr Entry TABLE_Z380_IX[] PROGMEM = {
    E2(0x20, 0030, TEXT_JR,    M_CC4,   M_REL16),
    E1(0x18, 0000, TEXT_JR,    M_REL16),
    E2(0xC4, 0070, TEXT_CALR,  M_CC8,   M_REL16),
    E1(0xCD, 0000, TEXT_CALR,  M_REL16),
    E1(0x10, 0000, TEXT_DJNZ,  M_REL16),
    L2(0x02, 0020, TEXT_LDW,   R_PTRH, R_DE),
    L2(0x32, 0000, TEXT_LDW,   R_HL,   R_DE),
    L2(0x0C, 0021, TEXT_LDW,   R_PTRH, I_PTRL),
    L2(0x0F, 0020, TEXT_LDW,   R_PTRH, I_HL),
    L2(0x27, 0000, TEXT_LDW,   R_IX,   R_IY),
    L2(0x3C, 0001, TEXT_LDW,   R_HL,   I_PTRL),
    L2(0x3F, 0000, TEXT_LDW,   R_HL,   I_HL),
    L2(0x02, 0020, TEXT_LD,    R_PTRH, R_DE),
    L2(0x32, 0000, TEXT_LD,    R_HL,   R_DE),
    L2(0x0C, 0021, TEXT_LD,    R_PTRH, I_PTRL),
    L2(0x0F, 0020, TEXT_LD,    R_PTRH, I_HL),
    L2(0x27, 0000, TEXT_LD,    R_IX,   R_IY),
    L2(0x3C, 0001, TEXT_LD,    R_HL,   I_PTRL),
    L2(0x3F, 0000, TEXT_LD,    R_HL,   I_HL),
    E1(0x2F, 0000, TEXT_CPLW,  R_HL),
    E1(0xC0, 0003, TEXT_DDIR,  M_DD),
    E2(0xC0, 0003, TEXT_DDIR,  M_DD,   M_DD),
    E1(0xF3, 0000, TEXT_DI,    M_IM8),
    E1(0xFB, 0000, TEXT_EI,    M_IM8),
    E1(0xF7, 0000, TEXT_SETC,  M_LCK),
    E1(0xFF, 0000, TEXT_RESC,  M_LCK),
    L2(0x47, 0000, TEXT_LDW,   R_I,    R_HL),
    L2(0x57, 0000, TEXT_LDW,   R_HL,   R_I),
    L2(0x47, 0000, TEXT_LD,    R_I,    R_HL),
    L2(0x57, 0000, TEXT_LD,    R_HL,   R_I),
    E2(0x40, 0020, TEXT_INW,   R_PTRH, I_C),
    E2(0x41, 0020, TEXT_OUTW,  I_C,    R_PTRH),
    E2(0x78, 0000, TEXT_INW,   R_HL,   I_C),
    E2(0x79, 0000, TEXT_OUTW,  I_C,    R_HL),
    E2(0xC8, 0000, TEXT_LDCTL, R_SR,   R_A),
    E2(0xCA, 0000, TEXT_LDCTL, R_SR,   M_IM8),
    E2(0xD0, 0000, TEXT_LDCTL, R_A,    R_XSR),
    E2(0xD8, 0000, TEXT_LDCTL, R_XSR,  R_A),
    E2(0xDA, 0000, TEXT_LDCTL, R_XSR,  M_IM8),
    E0(0xCF, 0000, TEXT_MTEST),
    E0(0x2F, 0000, TEXT_CPLW),
    E0(0xC0, 0000, TEXT_MTEST),
    E0(0xD9, 0000, TEXT_EXXX),
};

constexpr uint8_t INDEX_Z380_IX[] PROGMEM = {
      2,  // TEXT_CALR
      3,  // TEXT_CALR
     19,  // TEXT_CPLW
     40,  // TEXT_CPLW
     20,  // TEXT_DDIR
     21,  // TEXT_DDIR
     22,  // TEXT_DI
      4,  // TEXT_DJNZ
     23,  // TEXT_EI
     42,  // TEXT_EXXX
     30,  // TEXT_INW
     32,  // TEXT_INW
      0,  // TEXT_JR
      1,  // TEXT_JR
     12,  // TEXT_LD
     13,  // TEXT_LD
     14,  // TEXT_LD
     15,  // TEXT_LD
     16,  // TEXT_LD
     17,  // TEXT_LD
     18,  // TEXT_LD
     28,  // TEXT_LD
     29,  // TEXT_LD
     34,  // TEXT_LDCTL
     35,  // TEXT_LDCTL
     36,  // TEXT_LDCTL
     37,  // TEXT_LDCTL
     38,  // TEXT_LDCTL
      5,  // TEXT_LDW
      6,  // TEXT_LDW
      7,  // TEXT_LDW
      8,  // TEXT_LDW
      9,  // TEXT_LDW
     10,  // TEXT_LDW
     11,  // TEXT_LDW
     26,  // TEXT_LDW
     27,  // TEXT_LDW
     39,  // TEXT_MTEST
     41,  // TEXT_MTEST
     31,  // TEXT_OUTW
     33,  // TEXT_OUTW
     25,  // TEXT_RESC
     24,  // TEXT_SETC
};

// Z380 FD:xx
constexpr Entry TABLE_Z380_IY[] PROGMEM = {
    E2(0x20, 0030, TEXT_JR,    M_CC4,   M_REL24),
    E1(0x18, 0000, TEXT_JR,    M_REL24),
    E2(0xC4, 0070, TEXT_CALR,  M_CC8,   M_REL24),
    E1(0xCD, 0000, TEXT_CALR,  M_REL24),
    E2(0x20, 0030, TEXT_JR,    M_CC4,  M_REL24),
    E1(0x10, 0000, TEXT_DJNZ,  M_REL24),
    L2(0x02, 0020, TEXT_LDW,   R_PTRH, R_HL),
    L2(0x27, 0000, TEXT_LDW,   R_IY,   R_IX),
    L2(0x32, 0000, TEXT_LDW,   R_HL,   R_HL),
    L2(0x0C, 0021, TEXT_LDW,   I_PTRL, R_PTRH),
    L2(0x3C, 0001, TEXT_LDW,   I_PTRL, R_HL),
    L2(0x0F, 0020, TEXT_LDW,   I_HL,   R_PTRH),
    L2(0x3F, 0000, TEXT_LDW,   I_HL,   R_HL),
    L2(0x02, 0020, TEXT_LD,    R_PTRH, R_HL),
    L2(0x27, 0000, TEXT_LD,    R_IY,   R_IX),
    L2(0x32, 0000, TEXT_LD,    R_HL,   R_HL),
    L2(0x0C, 0021, TEXT_LD,    I_PTRL, R_PTRH),
    L2(0x3C, 0001, TEXT_LD,    I_PTRL, R_HL),
    L2(0x0F, 0020, TEXT_LD,    I_HL,   R_PTRH),
    L2(0x3F, 0000, TEXT_LD,    I_HL,   R_HL),
    E2(0x79, 0000, TEXT_OUTW,  I_C,    M_IM16),
    E1(0xC0, 0003, TEXT_DDIR,  M_DD),
    E2(0xD0, 0000, TEXT_LDCTL, R_A,    R_YSR),
    E2(0xD8, 0000, TEXT_LDCTL, R_YSR,  R_A),
    E2(0xDA, 0000, TEXT_LDCTL, R_YSR,  M_IM8),
    E0(0xD9, 0000, TEXT_EXXY),
    I2(0xD3, 0000, TEXT_OUTAW, M_IO16, R_HL),
    I2(0xDB, 0000, TEXT_INAW,  R_HL,   M_IO16),
    D1(0xF5, 0000, TEXT_PUSH,  M_LM16),
    E1(0xF7, 0000, TEXT_SETC,  M_XM),
};

constexpr uint8_t INDEX_Z380_IY[] PROGMEM = {
      2,  // TEXT_CALR
      3,  // TEXT_CALR
     21,  // TEXT_DDIR
      5,  // TEXT_DJNZ
     25,  // TEXT_EXXY
     27,  // TEXT_INAW
      0,  // TEXT_JR
      1,  // TEXT_JR
      4,  // TEXT_JR
     13,  // TEXT_LD
     14,  // TEXT_LD
     15,  // TEXT_LD
     16,  // TEXT_LD
     17,  // TEXT_LD
     18,  // TEXT_LD
     19,  // TEXT_LD
     22,  // TEXT_LDCTL
     23,  // TEXT_LDCTL
     24,  // TEXT_LDCTL
      6,  // TEXT_LDW
      7,  // TEXT_LDW
      8,  // TEXT_LDW
      9,  // TEXT_LDW
     10,  // TEXT_LDW
     11,  // TEXT_LDW
     12,  // TEXT_LDW
     26,  // TEXT_OUTAW
     20,  // TEXT_OUTW
     28,  // TEXT_PUSH
     29,  // TEXT_SETC
};

// Z380 CB
constexpr Entry TABLE_Z380_BIT[] PROGMEM = {
    L2(0x30, 0007, TEXT_EX,     M_SR8,  R_ALT),
};

constexpr uint8_t INDEX_Z380_BIT[] PROGMEM = {
      0,  // TEXT_EX
};

// Z380 ED:CB
constexpr Entry TABLE_Z380_EXTBIT[] PROGMEM = {
    L2(0x30, 0001, TEXT_EX,     R_PTRL, R_ALT),
    L2(0x33, 0000, TEXT_EX,     R_HL,   R_ALT),
    L2(0x34, 0001, TEXT_EX,     R_IDXL, R_ALT),
    E1(0x00, 0001, TEXT_RLCW,   R_PTRL),
    E1(0x02, 0000, TEXT_RLCW,   I_HL),
    E1(0x03, 0000, TEXT_RLCW,   R_HL),
    E1(0x04, 0001, TEXT_RLCW,   R_IDXL),
    E1(0x08, 0001, TEXT_RRCW,   R_PTRL),
    E1(0x0A, 0000, TEXT_RRCW,   I_HL),
    E1(0x0B, 0000, TEXT_RRCW,   R_HL),
    E1(0x0C, 0001, TEXT_RRCW,   R_IDXL),
    E1(0x10, 0001, TEXT_RLW,    R_PTRL),
    E1(0x12, 0000, TEXT_RLW,    I_HL),
    E1(0x13, 0000, TEXT_RLW,    R_HL),
    E1(0x14, 0001, TEXT_RLW,    R_IDXL),
    E1(0x18, 0001, TEXT_RRW,    R_PTRL),
    E1(0x1A, 0000, TEXT_RRW,    I_HL),
    E1(0x1B, 0000, TEXT_RRW,    R_HL),
    E1(0x1C, 0001, TEXT_RRW,    R_IDXL),
    E1(0x20, 0001, TEXT_SLAW,   R_PTRL),
    E1(0x22, 0000, TEXT_SLAW,   I_HL),
    E1(0x23, 0000, TEXT_SLAW,   R_HL),
    E1(0x24, 0001, TEXT_SLAW,   R_IDXL),
    E1(0x28, 0001, TEXT_SRAW,   R_PTRL),
    E1(0x2A, 0000, TEXT_SRAW,   I_HL),
    E1(0x2B, 0000, TEXT_SRAW,   R_HL),
    E1(0x2C, 0001, TEXT_SRAW,   R_IDXL),
    E1(0x38, 0001, TEXT_SRLW,   R_PTRL),
    E1(0x3A, 0000, TEXT_SRLW,   I_HL),
    E1(0x3B, 0000, TEXT_SRLW,   R_HL),
    E1(0x3C, 0001, TEXT_SRLW,   R_IDXL),
    E2(0x90, 0001, TEXT_MULTW,  R_HL,   R_PTRL),
    E2(0x93, 0000, TEXT_MULTW,  R_HL,   R_HL),
    E2(0x94, 0001, TEXT_MULTW,  R_HL,   R_IDXL),
    E2(0x97, 0000, TEXT_MULTW,  R_HL,   M_IM16),
    E2(0x98, 0001, TEXT_MULTUW, R_HL,   R_PTRL),
    E2(0x9B, 0000, TEXT_MULTUW, R_HL,   R_HL),
    E2(0x9C, 0001, TEXT_MULTUW, R_HL,   R_IDXL),
    E2(0x9F, 0000, TEXT_MULTUW, R_HL,   M_IM16),
    E2(0xB8, 0001, TEXT_DIVUW,  R_HL,   R_PTRL),
    E2(0xBB, 0000, TEXT_DIVUW,  R_HL,   R_HL),
    E2(0xBC, 0001, TEXT_DIVUW,  R_HL,   R_IDXL),
    E2(0xBF, 0000, TEXT_DIVUW,  R_HL,   M_IM16),
    E1(0x90, 0001, TEXT_MULTW,  R_PTRL),
    E1(0x93, 0000, TEXT_MULTW,  R_HL),
    E1(0x94, 0001, TEXT_MULTW,  R_IDXL),
    E1(0x97, 0000, TEXT_MULTW,  M_IM16),
    E1(0x98, 0001, TEXT_MULTUW, R_PTRL),
    E1(0x9B, 0000, TEXT_MULTUW, R_HL),
    E1(0x9C, 0001, TEXT_MULTUW, R_IDXL),
    E1(0x9F, 0000, TEXT_MULTUW, M_IM16),
    E1(0xB8, 0001, TEXT_DIVUW,  R_PTRL),
    E1(0xBB, 0000, TEXT_DIVUW,  R_HL),
    E1(0xBC, 0001, TEXT_DIVUW,  R_IDXL),
    E1(0xBF, 0000, TEXT_DIVUW,  M_IM16),
};

constexpr uint8_t INDEX_Z380_EXTBIT[] PROGMEM = {
     39,  // TEXT_DIVUW
     40,  // TEXT_DIVUW
     41,  // TEXT_DIVUW
     42,  // TEXT_DIVUW
     51,  // TEXT_DIVUW
     52,  // TEXT_DIVUW
     53,  // TEXT_DIVUW
     54,  // TEXT_DIVUW
      0,  // TEXT_EX
      1,  // TEXT_EX
      2,  // TEXT_EX
     35,  // TEXT_MULTUW
     36,  // TEXT_MULTUW
     37,  // TEXT_MULTUW
     38,  // TEXT_MULTUW
     47,  // TEXT_MULTUW
     48,  // TEXT_MULTUW
     49,  // TEXT_MULTUW
     50,  // TEXT_MULTUW
     31,  // TEXT_MULTW
     32,  // TEXT_MULTW
     33,  // TEXT_MULTW
     34,  // TEXT_MULTW
     43,  // TEXT_MULTW
     44,  // TEXT_MULTW
     45,  // TEXT_MULTW
     46,  // TEXT_MULTW
      3,  // TEXT_RLCW
      4,  // TEXT_RLCW
      5,  // TEXT_RLCW
      6,  // TEXT_RLCW
     11,  // TEXT_RLW
     12,  // TEXT_RLW
     13,  // TEXT_RLW
     14,  // TEXT_RLW
      7,  // TEXT_RRCW
      8,  // TEXT_RRCW
      9,  // TEXT_RRCW
     10,  // TEXT_RRCW
     15,  // TEXT_RRW
     16,  // TEXT_RRW
     17,  // TEXT_RRW
     18,  // TEXT_RRW
     19,  // TEXT_SLAW
     20,  // TEXT_SLAW
     21,  // TEXT_SLAW
     22,  // TEXT_SLAW
     23,  // TEXT_SRAW
     24,  // TEXT_SRAW
     25,  // TEXT_SRAW
     26,  // TEXT_SRAW
     27,  // TEXT_SRLW
     28,  // TEXT_SRLW
     29,  // TEXT_SRLW
     30,  // TEXT_SRLW
};

// Z380 DD:CB, FD:CB
constexpr Entry TABLE_Z380_IDXBIT[] PROGMEM = {
    D2(0x01, 0020, TEXT_LDW,    R_PTRH, M_SPX),
    D2(0x21, 0000, TEXT_LDW,    R_IDX,  M_SPX),
    D2(0x31, 0000, TEXT_LDW,    R_HL,   M_SPX),
    D2(0x09, 0020, TEXT_LDW,    M_SPX,  R_PTRH),
    D2(0x29, 0000, TEXT_LDW,    M_SPX,  R_IDX),
    D2(0x39, 0000, TEXT_LDW,    M_SPX,  R_HL),
    D2(0x03, 0020, TEXT_LDW,    R_PTRH, M_IDX),
    D2(0x33, 0000, TEXT_LDW,    R_HL,   M_IDX),
    D2(0x0B, 0020, TEXT_LDW,    M_IDX,  R_PTRH),
    D2(0x2B, 0000, TEXT_LDW,    M_IDX,  R_IY),
    D2(0x3B, 0000, TEXT_LDW,    M_IDX,  R_HL),
    D2(0x01, 0020, TEXT_LD,     R_PTRH, M_SPX),
    D2(0x21, 0000, TEXT_LD,     R_IDX,  M_SPX),
    D2(0x31, 0000, TEXT_LD,     R_HL,   M_SPX),
    D2(0x09, 0020, TEXT_LD,     M_SPX,  R_PTRH),
    D2(0x29, 0000, TEXT_LD,     M_SPX,  R_IDX),
    D2(0x39, 0000, TEXT_LD,     M_SPX,  R_HL),
    D2(0x03, 0020, TEXT_LD,     R_PTRH, M_IDX),
    D2(0x33, 0000, TEXT_LD,     R_HL,   M_IDX),
    D2(0x0B, 0020, TEXT_LD,     M_IDX,  R_PTRH),
    D2(0x2B, 0000, TEXT_LD,     M_IDX,  R_IY),
    D2(0x3B, 0000, TEXT_LD,     M_IDX,  R_HL),
    I1(0x02, 0000, TEXT_RLCW,   M_IDX),
    I1(0x0A, 0000, TEXT_RRCW,   M_IDX),
    I1(0x12, 0000, TEXT_RLW,    M_IDX),
    I1(0x1A, 0000, TEXT_RRW,    M_IDX),
    I1(0x22, 0000, TEXT_SLAW,   M_IDX),
    I1(0x2A, 0000, TEXT_SRAW,   M_IDX),
    I1(0x3A, 0000, TEXT_SRLW,   M_IDX),
    I2(0x92, 0000, TEXT_MULTW,  R_HL,   M_IDX),
    I2(0x9A, 0000, TEXT_MULTUW, R_HL,   M_IDX),
    I2(0xBA, 0000, TEXT_DIVUW,  R_HL,   M_IDX),
    I1(0x92, 0000, TEXT_MULTW,  M_IDX),
    I1(0x9A, 0000, TEXT_MULTUW, M_IDX),
    I1(0xBA, 0000, TEXT_DIVUW,  M_IDX),
};

constexpr uint8_t INDEX_Z380_IDXBIT[] PROGMEM = {
     31,  // TEXT_DIVUW
     34,  // TEXT_DIVUW
     11,  // TEXT_LD
     12,  // TEXT_LD
     13,  // TEXT_LD
     14,  // TEXT_LD
     15,  // TEXT_LD
     16,  // TEXT_LD
     17,  // TEXT_LD
     18,  // TEXT_LD
     19,  // TEXT_LD
     20,  // TEXT_LD
     21,  // TEXT_LD
      0,  // TEXT_LDW
      1,  // TEXT_LDW
      2,  // TEXT_LDW
      3,  // TEXT_LDW
      4,  // TEXT_LDW
      5,  // TEXT_LDW
      6,  // TEXT_LDW
      7,  // TEXT_LDW
      8,  // TEXT_LDW
      9,  // TEXT_LDW
     10,  // TEXT_LDW
     30,  // TEXT_MULTUW
     33,  // TEXT_MULTUW
     29,  // TEXT_MULTW
     32,  // TEXT_MULTW
     22,  // TEXT_RLCW
     24,  // TEXT_RLW
     23,  // TEXT_RRCW
     25,  // TEXT_RRW
     26,  // TEXT_SLAW
     27,  // TEXT_SRAW
     28,  // TEXT_SRLW
};

// Z380 DD:CB
constexpr Entry TABLE_Z380_IXBIT[] PROGMEM = {
    D2(0x23, 0000, TEXT_LDW,   R_IY,   M_IDX),
    D2(0x2B, 0000, TEXT_LDW,   M_IDX,  R_IY),
    D2(0x23, 0000, TEXT_LD,    R_IY,   M_IDX),
    D2(0x2B, 0000, TEXT_LD,    M_IDX,  R_IY),
};

constexpr uint8_t INDEX_Z380_IXBIT[] PROGMEM = {
      2,  // TEXT_LD
      3,  // TEXT_LD
      0,  // TEXT_LDW
      1,  // TEXT_LDW
};

// Z380 FD:CB
constexpr Entry TABLE_Z380_IYBIT[] PROGMEM = {
    D2(0x23, 0000, TEXT_LDW,   R_IX,   M_IDX),
    D2(0x2B, 0000, TEXT_LDW,   M_IDX,  R_IX),
    D2(0x23, 0000, TEXT_LD,    R_IX,   M_IDX),
    D2(0x2B, 0000, TEXT_LD,    M_IDX,  R_IX),
};

constexpr uint8_t INDEX_Z380_IYBIT[] PROGMEM = {
      2,  // TEXT_LD
      3,  // TEXT_LD
      0,  // TEXT_LDW
      1,  // TEXT_LDW
};

// clang-format on

constexpr EntryPage Z380_PAGES[] PROGMEM = {
        {0x0000, ARRAY_RANGE(TABLE_Z80), ARRAY_RANGE(INDEX_Z80)},
        {0x0000, ARRAY_RANGE(TABLE_Z380), ARRAY_RANGE(INDEX_Z380)},
        {Entry::EXT, ARRAY_RANGE(TABLE_Z80_EXT), ARRAY_RANGE(INDEX_Z80_EXT)},
        {Entry::EXT, ARRAY_RANGE(TABLE_Z180_EXT), ARRAY_RANGE(INDEX_Z180_EXT)},
        {Entry::EXT, ARRAY_RANGE(TABLE_Z380_EXT), ARRAY_RANGE(INDEX_Z380_EXT)},
        {Entry::EXTBIT, ARRAY_RANGE(TABLE_Z380_EXTBIT), ARRAY_RANGE(INDEX_Z380_EXTBIT)},
        {Entry::BIT, ARRAY_RANGE(TABLE_Z80_BIT), ARRAY_RANGE(INDEX_Z80_BIT)},
        {Entry::BIT, ARRAY_RANGE(TABLE_Z380_BIT), ARRAY_RANGE(INDEX_Z380_BIT)},
        {Entry::IX, ARRAY_RANGE(TABLE_Z80_IDX), ARRAY_RANGE(INDEX_Z80_IDX)},
        {Entry::IY, ARRAY_RANGE(TABLE_Z80_IDX), ARRAY_RANGE(INDEX_Z80_IDX)},
        {Entry::IX, ARRAY_RANGE(TABLE_Z380_IDX), ARRAY_RANGE(INDEX_Z380_IDX)},
        {Entry::IY, ARRAY_RANGE(TABLE_Z380_IDX), ARRAY_RANGE(INDEX_Z380_IDX)},
        {Entry::IX, ARRAY_RANGE(TABLE_Z380_IX), ARRAY_RANGE(INDEX_Z380_IX)},
        {Entry::IY, ARRAY_RANGE(TABLE_Z380_IY), ARRAY_RANGE(INDEX_Z380_IY)},
        {Entry::IXBIT, ARRAY_RANGE(TABLE_Z80_IDXBIT), ARRAY_RANGE(INDEX_Z80_IDXBIT)},
        {Entry::IYBIT, ARRAY_RANGE(TABLE_Z80_IDXBIT), ARRAY_RANGE(INDEX_Z80_IDXBIT)},
        {Entry::IXBIT, ARRAY_RANGE(TABLE_Z380_IXBIT), ARRAY_RANGE(INDEX_Z380_IXBIT)},
        {Entry::IYBIT, ARRAY_RANGE(TABLE_Z380_IYBIT), ARRAY_RANGE(INDEX_Z380_IYBIT)},
        {Entry::IXBIT, ARRAY_RANGE(TABLE_Z380_IDXBIT), ARRAY_RANGE(INDEX_Z380_IDXBIT)},
        {Entry::IYBIT, ARRAY_RANGE(TABLE_Z380_IDXBIT), ARRAY_RANGE(INDEX_Z380_IDXBIT)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {Z80, TEXT_CPU_Z80, ARRAY_RANGE(Z80_PAGES)},
        {Z180, TEXT_CPU_Z180, ARRAY_RANGE(Z180_PAGES)},
        {Z380, TEXT_CPU_Z380, ARRAY_RANGE(Z380_PAGES)},
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
        return table == M_SRC || table == M_DST;
    if (opr == I_PTR)
        return table == I_PTRL;
    if (opr == R_C)
        return table == M_SRC || table == M_DST || table == M_SR8 || table == M_DR8 ||
               table == M_SR8X || table == M_DR8X || table == M_CC4 || table == M_CC8;
    if (opr == R_H || opr == R_L)
        return table == M_SRC || table == M_DST || table == M_SR8 || table == M_DR8 ||
               table == M_SR8X || table == M_DR8X;
    if (opr == R_DXY)
        return table == R_SXY;
    if (opr == R_BC || opr == R_DE)
        return table == M_R16 || table == M_R16X || table == R_PTRL || table == R_PTRH;
    if (opr == R_HL)
        return table == M_R16 || table == M_R16X;
    if (opr == R_SP)
        return table == M_R16 || table == M_R16X;
    if (opr == R_IX || opr == R_IY)
        return table == R_IDX || table == R_IDXL || table == M_R16X;
    if (opr == M_IDX8)
        return table == M_IDX;
    if (opr == M_IM16)
        return table == M_IM8 || table == M_REL8 || table == M_BIT || table == M_VEC ||
               table == M_IMMD || table == M_DM16 || table == M_XM16 || table == M_LM16 ||
               table == M_JABS;
    if (opr == M_ABS)
        return table == M_IOA || table == M_IO16 || table == M_DABS || table == M_XABS;
    if (opr == M_CC4)
        return table == M_CC8;
    if (opr == M_LW)
        return table == M_DD;
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

const /*PROGMEM*/ char *TableZ380::listCpu_P() const {
    return TEXT_Z380_LIST;
}

const /*PROGMEM*/ char *TableZ380::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableZ380::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    auto t = Cpu::search(name, ARRAY_RANGE(CPU_TABLE));
    if (t) {
        cpuType = t->readCpuType();
        return OK;
    }
    return UNSUPPORTED_CPU;
}

const TableZ380 TABLE;

}  // namespace z380
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

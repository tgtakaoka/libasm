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

#include "table_tms370.h"
#include "entry_table.h"
#include "entry_tms370.h"
#include "text_tms370.h"

using namespace libasm::text::tms370;

namespace libasm {
namespace tms370 {

#define E3(_opc, _name, _opr1, _opr2, _opr3) \
    {_opc, Entry::Flags::create(_opr1, _opr2, _opr3), _name}
#define E2(_opc, _name, _opr1, _opr2) E3(_opc, _name, _opr1, _opr2, M_NONE)
#define E1(_opc, _name, _opr1) E2(_opc, _name, _opr1, M_NONE)
#define E0(_opc, _name) E1(_opc, _name, M_NONE)

// clang-format off
constexpr Entry TMS370_TABLE[] PROGMEM = {
    E0(0xFF, TEXT_NOP),
    E0(0xF6, TEXT_IDLE),
    E1(0xF0, TEXT_LDST,  M_IM8),
    E0(0xF8, TEXT_SETC),
    E1(0xFC, TEXT_POP,   M_ST),
    E0(0xFE, TEXT_STSP),
    E0(0xF9, TEXT_RTS),
    E0(0xFA, TEXT_RTI),
    E0(0xFD, TEXT_LDSP),
    E1(0xFB, TEXT_PUSH,  M_ST),
    E2(0xC0, TEXT_MOV,   M_A,    M_B),
    E2(0x62, TEXT_MOV,   M_B,    M_A),
    E2(0xD0, TEXT_MOV,   M_A,    M_RN),
    E2(0xD1, TEXT_MOV,   M_B,    M_RN),
    E2(0x63, TEXT_AND,   M_B,    M_A),
    E2(0x64, TEXT_OR,    M_B,    M_A),
    E2(0x65, TEXT_XOR,   M_B,    M_A),
    E3(0x66, TEXT_BTJO,  M_B,    M_A,   M_REL),
    E3(0x67, TEXT_BTJZ,  M_B,    M_A,   M_REL),
    E2(0x68, TEXT_ADD,   M_B,    M_A),
    E2(0x69, TEXT_ADC,   M_B,    M_A),
    E2(0x6A, TEXT_SUB,   M_B,    M_A),
    E2(0x6B, TEXT_SBB,   M_B,    M_A),
    E2(0x6C, TEXT_MPY,   M_B,    M_A),
    E2(0x6D, TEXT_CMP,   M_B,    M_A),
    E2(0x6E, TEXT_DAC,   M_B,    M_A),
    E2(0x6F, TEXT_DSB,   M_B,    M_A),
    E2(0x12, TEXT_MOV,   M_RN,   M_A),
    E2(0xF1, TEXT_MOV,   M_SIDX, M_A),
    E2(0x13, TEXT_AND,   M_RN,   M_A),
    E2(0x14, TEXT_OR,    M_RN,   M_A),
    E2(0x15, TEXT_XOR,   M_RN,   M_A),
    E3(0x16, TEXT_BTJO,  M_RN,   M_A,   M_REL),
    E3(0x17, TEXT_BTJZ,  M_RN,   M_A,   M_REL),
    E2(0x18, TEXT_ADD,   M_RN,   M_A),
    E2(0x19, TEXT_ADC,   M_RN,   M_A),
    E2(0x1A, TEXT_SUB,   M_RN,   M_A),
    E2(0x1B, TEXT_SBB,   M_RN,   M_A),
    E2(0x1C, TEXT_MPY,   M_RN,   M_A),
    E2(0x1D, TEXT_CMP,   M_RN,   M_A),
    E2(0xF3, TEXT_CMP,   M_SIDX, M_A),
    E2(0x1E, TEXT_DAC,   M_RN,   M_A),
    E2(0x1F, TEXT_DSB,   M_RN,   M_A),
    E2(0x22, TEXT_MOV,   M_IM8,  M_A),
    E2(0xF2, TEXT_MOV,   M_A,    M_SIDX),
    E2(0x23, TEXT_AND,   M_IM8,  M_A),
    E2(0x24, TEXT_OR,    M_IM8,  M_A),
    E2(0x25, TEXT_XOR,   M_IM8,  M_A),
    E3(0x26, TEXT_BTJO,  M_IM8,  M_A,   M_REL),
    E3(0x27, TEXT_BTJZ,  M_IM8,  M_A,   M_REL),
    E2(0x28, TEXT_ADD,   M_IM8,  M_A),
    E2(0x29, TEXT_ADC,   M_IM8,  M_A),
    E2(0x2A, TEXT_SUB,   M_IM8,  M_A),
    E2(0x2B, TEXT_SBB,   M_IM8,  M_A),
    E2(0x2C, TEXT_MPY,   M_IM8,  M_A),
    E2(0x2D, TEXT_CMP,   M_IM8,  M_A),
    E2(0x2E, TEXT_DAC,   M_IM8,  M_A),
    E2(0x2F, TEXT_DSB,   M_IM8,  M_A),
    E2(0x32, TEXT_MOV,   M_RN,   M_B),
    E2(0x33, TEXT_AND,   M_RN,   M_B),
    E2(0x34, TEXT_OR,    M_RN,   M_B),
    E2(0x35, TEXT_XOR,   M_RN,   M_B),
    E3(0x36, TEXT_BTJO,  M_RN,   M_B,   M_REL),
    E3(0x37, TEXT_BTJZ,  M_RN,   M_B,   M_REL),
    E2(0x38, TEXT_ADD,   M_RN,   M_B),
    E2(0x39, TEXT_ADC,   M_RN,   M_B),
    E2(0x3A, TEXT_SUB,   M_RN,   M_B),
    E2(0x3B, TEXT_SBB,   M_RN,   M_B),
    E2(0x3C, TEXT_MPY,   M_RN,   M_B),
    E2(0x3D, TEXT_CMP,   M_RN,   M_B),
    E2(0x3E, TEXT_DAC,   M_RN,   M_B),
    E2(0x3F, TEXT_DSB,   M_RN,   M_B),
    E2(0x42, TEXT_MOV,   M_RN,   M_RN),
    E2(0x43, TEXT_AND,   M_RN,   M_RN),
    E2(0x44, TEXT_OR,    M_RN,   M_RN),
    E2(0x45, TEXT_XOR,   M_RN,   M_RN),
    E3(0x46, TEXT_BTJO,  M_RN,   M_RN,  M_REL),
    E3(0x47, TEXT_BTJZ,  M_RN,   M_RN,  M_REL),
    E2(0x48, TEXT_ADD,   M_RN,   M_RN),
    E2(0x49, TEXT_ADC,   M_RN,   M_RN),
    E2(0x4A, TEXT_SUB,   M_RN,   M_RN),
    E2(0x4B, TEXT_SBB,   M_RN,   M_RN),
    E2(0x4C, TEXT_MPY,   M_RN,   M_RN),
    E2(0x4D, TEXT_CMP,   M_RN,   M_RN),
    E2(0x4E, TEXT_DAC,   M_RN,   M_RN),
    E2(0x4F, TEXT_DSB,   M_RN,   M_RN),
    E2(0x52, TEXT_MOV,   M_IM8,  M_B),
    E2(0x53, TEXT_AND,   M_IM8,  M_B),
    E2(0x54, TEXT_OR,    M_IM8,  M_B),
    E2(0x55, TEXT_XOR,   M_IM8,  M_B),
    E3(0x56, TEXT_BTJO,  M_IM8,  M_B,   M_REL),
    E3(0x57, TEXT_BTJZ,  M_IM8,  M_B,   M_REL),
    E2(0x58, TEXT_ADD,   M_IM8,  M_B),
    E2(0x59, TEXT_ADC,   M_IM8,  M_B),
    E2(0x5A, TEXT_SUB,   M_IM8,  M_B),
    E2(0x5B, TEXT_SBB,   M_IM8,  M_B),
    E2(0x5C, TEXT_MPY,   M_IM8,  M_B),
    E2(0x5D, TEXT_CMP,   M_IM8,  M_B),
    E2(0x5E, TEXT_DAC,   M_IM8,  M_B),
    E2(0x5F, TEXT_DSB,   M_IM8,  M_B),
    E2(0x70, TEXT_INCW,  M_SIM8, M_RN),
    E2(0x72, TEXT_MOV,   M_IM8,  M_RN),
    E2(0x73, TEXT_AND,   M_IM8,  M_RN),
    E2(0x74, TEXT_OR,    M_IM8,  M_RN),
    E2(0x75, TEXT_XOR,   M_IM8,  M_RN),
    E3(0x76, TEXT_BTJO,  M_IM8,  M_RN,   M_REL),
    E3(0x77, TEXT_BTJZ,  M_IM8,  M_RN,   M_REL),
    E2(0x78, TEXT_ADD,   M_IM8,  M_RN),
    E2(0x79, TEXT_ADC,   M_IM8,  M_RN),
    E2(0x7A, TEXT_SUB,   M_IM8,  M_RN),
    E2(0x7B, TEXT_SBB,   M_IM8,  M_RN),
    E2(0x7C, TEXT_MPY,   M_IM8,  M_RN),
    E2(0x7D, TEXT_CMP,   M_IM8,  M_RN),
    E2(0x7E, TEXT_DAC,   M_IM8,  M_RN),
    E2(0x7F, TEXT_DSB,   M_IM8,  M_RN),
    E2(0x21, TEXT_MOV,   M_A,    M_PN),
    E2(0x51, TEXT_MOV,   M_B,    M_PN),
    E2(0x71, TEXT_MOV,   M_RN,   M_PN),
    E2(0x80, TEXT_MOV,   M_PN,   M_A),
    E2(0x91, TEXT_MOV,   M_PN,   M_B),
    E2(0xA2, TEXT_MOV,   M_PN,   M_RN),
    E2(0x83, TEXT_AND,   M_A,    M_PN),
    E2(0x84, TEXT_OR,    M_A,    M_PN),
    E2(0x85, TEXT_XOR,   M_A,    M_PN),
    E3(0x86, TEXT_BTJO,  M_A,    M_PN,  M_REL),
    E3(0x87, TEXT_BTJZ,  M_A,    M_PN,  M_REL),
    E2(0x88, TEXT_MOVW,  M_IM16, M_RN),
    E1(0x89, TEXT_JMPL,  M_LREL),
    E2(0x8A, TEXT_MOV,   M_ABS,  M_A),
    E2(0x8B, TEXT_MOV,   M_A,    M_ABS),
    E1(0x8C, TEXT_BR,    M_ABS),
    E2(0x8D, TEXT_CMP,   M_ABS,  M_A),
    E1(0x8E, TEXT_CALL,  M_ABS),
    E1(0x8F, TEXT_CALLR, M_LREL),
    E2(0x93, TEXT_AND,   M_B,    M_PN),
    E2(0x94, TEXT_OR,    M_B,    M_PN),
    E2(0x95, TEXT_XOR,   M_B,    M_PN),
    E3(0x96, TEXT_BTJO,  M_B,    M_PN,  M_REL),
    E3(0x97, TEXT_BTJZ,  M_B,    M_PN,  M_REL),
    E2(0x98, TEXT_MOVW,  M_RN,   M_RN),
    E1(0x99, TEXT_JMPL,  M_IDIR),
    E2(0x9A, TEXT_MOV,   M_IDIR, M_A),
    E2(0x9B, TEXT_MOV,   M_A,    M_IDIR),
    E1(0x9C, TEXT_BR,    M_IDIR),
    E2(0x9D, TEXT_CMP,   M_IDIR, M_A),
    E1(0x9E, TEXT_CALL,  M_IDIR),
    E1(0x9F, TEXT_CALLR, M_IDIR),
    E2(0xF7, TEXT_MOV,   M_IM8,  M_PN),
    E2(0xA3, TEXT_AND,   M_IM8,  M_PN),
    E2(0xA4, TEXT_OR,    M_IM8,  M_PN),
    E2(0xA5, TEXT_XOR,   M_IM8,  M_PN),
    E3(0xA6, TEXT_BTJO,  M_IM8,  M_PN,  M_REL),
    E3(0xA7, TEXT_BTJZ,  M_IM8,  M_PN,  M_REL),
    E2(0xA8, TEXT_MOVW,  M_BIMM, M_RN),
    E1(0xA9, TEXT_JMPL,  M_BREL),
    E2(0xAA, TEXT_MOV,   M_BIDX, M_A),
    E2(0xAB, TEXT_MOV,   M_A,    M_BIDX),
    E1(0xAC, TEXT_BR,    M_BIDX),
    E2(0xAD, TEXT_CMP,   M_BIDX, M_A),
    E1(0xAE, TEXT_CALL,  M_BIDX),
    E1(0xAF, TEXT_CALLR, M_BREL),
    E1(0xB0, TEXT_TST,   M_A),
    E0(0xB0, TEXT_CLRC),
    E1(0xB2, TEXT_DEC,   M_A),
    E1(0xB3, TEXT_INC,   M_A),
    E1(0xB4, TEXT_INV,   M_A),
    E1(0xB5, TEXT_CLR,   M_A),
    E1(0xB6, TEXT_XCHB,  M_A),
    E1(0xB7, TEXT_SWAP,  M_A),
    E1(0xB8, TEXT_PUSH,  M_A),
    E1(0xB9, TEXT_POP,   M_A),
    E2(0xBA, TEXT_DJNZ,  M_A,    M_REL),
    E1(0xBB, TEXT_COMPL, M_A),
    E1(0xBC, TEXT_RR,    M_A),
    E1(0xBD, TEXT_RRC,   M_A),
    E1(0xBE, TEXT_RL,    M_A),
    E1(0xBF, TEXT_RLC,   M_A),
    E1(0xC6, TEXT_TST,   M_B),
    E1(0xC2, TEXT_DEC,   M_B),
    E1(0xC3, TEXT_INC,   M_B),
    E1(0xC4, TEXT_INV,   M_B),
    E1(0xC5, TEXT_CLR,   M_B),
    E1(0xC6, TEXT_XCHB,  M_B),
    E1(0xC7, TEXT_SWAP,  M_B),
    E1(0xC8, TEXT_PUSH,  M_B),
    E1(0xC9, TEXT_POP,   M_B),
    E2(0xCA, TEXT_DJNZ,  M_B,    M_REL),
    E1(0xCB, TEXT_COMPL, M_B),
    E1(0xCC, TEXT_RR,    M_B),
    E1(0xCD, TEXT_RRC,   M_B),
    E1(0xCE, TEXT_RL,    M_B),
    E1(0xCF, TEXT_RLC,   M_B),
    E1(0xD2, TEXT_DEC,   M_RN),
    E1(0xD3, TEXT_INC,   M_RN),
    E1(0xD4, TEXT_INV,   M_RN),
    E1(0xD5, TEXT_CLR,   M_RN),
    E1(0xD6, TEXT_XCHB,  M_RN),
    E1(0xD7, TEXT_SWAP,  M_RN),
    E1(0xD8, TEXT_PUSH,  M_RN),
    E1(0xD9, TEXT_POP,   M_RN),
    E2(0xDA, TEXT_DJNZ,  M_RN,   M_REL),
    E1(0xDB, TEXT_COMPL, M_RN),
    E1(0xDC, TEXT_RR,    M_RN),
    E1(0xDD, TEXT_RRC,   M_RN),
    E1(0xDE, TEXT_RL,    M_RN),
    E1(0xDF, TEXT_RLC,   M_RN),
    E1(0x00, TEXT_JMP,   M_REL),
    E1(0x01, TEXT_JN,    M_REL),
    E1(0x02, TEXT_JZ,    M_REL),
    E1(0x02, TEXT_JEQ,   M_REL),
    E1(0x03, TEXT_JC,    M_REL),
    E1(0x04, TEXT_JP,    M_REL),
    E1(0x05, TEXT_JPZ,   M_REL),
    E1(0x06, TEXT_JNZ,   M_REL),
    E1(0x06, TEXT_JNE,   M_REL),
    E1(0x07, TEXT_JNC,   M_REL),
    E1(0x08, TEXT_JV,    M_REL),
    E1(0x09, TEXT_JL,    M_REL),
    E1(0x0A, TEXT_JLE,   M_REL),
    E1(0x0B, TEXT_JHS,   M_REL),
    E1(0x0C, TEXT_JNV,   M_REL),
    E1(0x0D, TEXT_JGE,   M_REL),
    E1(0x0E, TEXT_JG,    M_REL),
    E1(0x0F, TEXT_JLO,   M_REL),
    E1(0xE0, TEXT_TRAP,  M_TVEC),
};

constexpr uint8_t TMS370_INDEX[] PROGMEM = {
     20,  // TEXT_ADC
     35,  // TEXT_ADC
     51,  // TEXT_ADC
     65,  // TEXT_ADC
     79,  // TEXT_ADC
     93,  // TEXT_ADC
    108,  // TEXT_ADC
     19,  // TEXT_ADD
     34,  // TEXT_ADD
     50,  // TEXT_ADD
     64,  // TEXT_ADD
     78,  // TEXT_ADD
     92,  // TEXT_ADD
    107,  // TEXT_ADD
     14,  // TEXT_AND
     29,  // TEXT_AND
     45,  // TEXT_AND
     59,  // TEXT_AND
     73,  // TEXT_AND
     87,  // TEXT_AND
    102,  // TEXT_AND
    121,  // TEXT_AND
    134,  // TEXT_AND
    148,  // TEXT_AND
    130,  // TEXT_BR
    143,  // TEXT_BR
    157,  // TEXT_BR
     17,  // TEXT_BTJO
     32,  // TEXT_BTJO
     48,  // TEXT_BTJO
     62,  // TEXT_BTJO
     76,  // TEXT_BTJO
     90,  // TEXT_BTJO
    105,  // TEXT_BTJO
    124,  // TEXT_BTJO
    137,  // TEXT_BTJO
    151,  // TEXT_BTJO
     18,  // TEXT_BTJZ
     33,  // TEXT_BTJZ
     49,  // TEXT_BTJZ
     63,  // TEXT_BTJZ
     77,  // TEXT_BTJZ
     91,  // TEXT_BTJZ
    106,  // TEXT_BTJZ
    125,  // TEXT_BTJZ
    138,  // TEXT_BTJZ
    152,  // TEXT_BTJZ
    132,  // TEXT_CALL
    145,  // TEXT_CALL
    159,  // TEXT_CALL
    133,  // TEXT_CALLR
    146,  // TEXT_CALLR
    160,  // TEXT_CALLR
    166,  // TEXT_CLR
    181,  // TEXT_CLR
    195,  // TEXT_CLR
    162,  // TEXT_CLRC
     24,  // TEXT_CMP
     39,  // TEXT_CMP
     40,  // TEXT_CMP
     55,  // TEXT_CMP
     69,  // TEXT_CMP
     83,  // TEXT_CMP
     97,  // TEXT_CMP
    112,  // TEXT_CMP
    131,  // TEXT_CMP
    144,  // TEXT_CMP
    158,  // TEXT_CMP
    172,  // TEXT_COMPL
    187,  // TEXT_COMPL
    201,  // TEXT_COMPL
     25,  // TEXT_DAC
     41,  // TEXT_DAC
     56,  // TEXT_DAC
     70,  // TEXT_DAC
     84,  // TEXT_DAC
     98,  // TEXT_DAC
    113,  // TEXT_DAC
    163,  // TEXT_DEC
    178,  // TEXT_DEC
    192,  // TEXT_DEC
    171,  // TEXT_DJNZ
    186,  // TEXT_DJNZ
    200,  // TEXT_DJNZ
     26,  // TEXT_DSB
     42,  // TEXT_DSB
     57,  // TEXT_DSB
     71,  // TEXT_DSB
     85,  // TEXT_DSB
     99,  // TEXT_DSB
    114,  // TEXT_DSB
      1,  // TEXT_IDLE
    164,  // TEXT_INC
    179,  // TEXT_INC
    193,  // TEXT_INC
    100,  // TEXT_INCW
    165,  // TEXT_INV
    180,  // TEXT_INV
    194,  // TEXT_INV
    210,  // TEXT_JC
    209,  // TEXT_JEQ
    222,  // TEXT_JG
    221,  // TEXT_JGE
    219,  // TEXT_JHS
    217,  // TEXT_JL
    218,  // TEXT_JLE
    223,  // TEXT_JLO
    206,  // TEXT_JMP
    127,  // TEXT_JMPL
    140,  // TEXT_JMPL
    154,  // TEXT_JMPL
    207,  // TEXT_JN
    215,  // TEXT_JNC
    214,  // TEXT_JNE
    220,  // TEXT_JNV
    213,  // TEXT_JNZ
    211,  // TEXT_JP
    212,  // TEXT_JPZ
    216,  // TEXT_JV
    208,  // TEXT_JZ
      8,  // TEXT_LDSP
      2,  // TEXT_LDST
     10,  // TEXT_MOV
     11,  // TEXT_MOV
     12,  // TEXT_MOV
     13,  // TEXT_MOV
     27,  // TEXT_MOV
     28,  // TEXT_MOV
     43,  // TEXT_MOV
     44,  // TEXT_MOV
     58,  // TEXT_MOV
     72,  // TEXT_MOV
     86,  // TEXT_MOV
    101,  // TEXT_MOV
    115,  // TEXT_MOV
    116,  // TEXT_MOV
    117,  // TEXT_MOV
    118,  // TEXT_MOV
    119,  // TEXT_MOV
    120,  // TEXT_MOV
    128,  // TEXT_MOV
    129,  // TEXT_MOV
    141,  // TEXT_MOV
    142,  // TEXT_MOV
    147,  // TEXT_MOV
    155,  // TEXT_MOV
    156,  // TEXT_MOV
    126,  // TEXT_MOVW
    139,  // TEXT_MOVW
    153,  // TEXT_MOVW
     23,  // TEXT_MPY
     38,  // TEXT_MPY
     54,  // TEXT_MPY
     68,  // TEXT_MPY
     82,  // TEXT_MPY
     96,  // TEXT_MPY
    111,  // TEXT_MPY
      0,  // TEXT_NOP
     15,  // TEXT_OR
     30,  // TEXT_OR
     46,  // TEXT_OR
     60,  // TEXT_OR
     74,  // TEXT_OR
     88,  // TEXT_OR
    103,  // TEXT_OR
    122,  // TEXT_OR
    135,  // TEXT_OR
    149,  // TEXT_OR
      4,  // TEXT_POP
    170,  // TEXT_POP
    185,  // TEXT_POP
    199,  // TEXT_POP
      9,  // TEXT_PUSH
    169,  // TEXT_PUSH
    184,  // TEXT_PUSH
    198,  // TEXT_PUSH
    175,  // TEXT_RL
    190,  // TEXT_RL
    204,  // TEXT_RL
    176,  // TEXT_RLC
    191,  // TEXT_RLC
    205,  // TEXT_RLC
    173,  // TEXT_RR
    188,  // TEXT_RR
    202,  // TEXT_RR
    174,  // TEXT_RRC
    189,  // TEXT_RRC
    203,  // TEXT_RRC
      7,  // TEXT_RTI
      6,  // TEXT_RTS
     22,  // TEXT_SBB
     37,  // TEXT_SBB
     53,  // TEXT_SBB
     67,  // TEXT_SBB
     81,  // TEXT_SBB
     95,  // TEXT_SBB
    110,  // TEXT_SBB
      3,  // TEXT_SETC
      5,  // TEXT_STSP
     21,  // TEXT_SUB
     36,  // TEXT_SUB
     52,  // TEXT_SUB
     66,  // TEXT_SUB
     80,  // TEXT_SUB
     94,  // TEXT_SUB
    109,  // TEXT_SUB
    168,  // TEXT_SWAP
    183,  // TEXT_SWAP
    197,  // TEXT_SWAP
    224,  // TEXT_TRAP
    161,  // TEXT_TST
    177,  // TEXT_TST
    167,  // TEXT_XCHB
    182,  // TEXT_XCHB
    196,  // TEXT_XCHB
     16,  // TEXT_XOR
     31,  // TEXT_XOR
     47,  // TEXT_XOR
     61,  // TEXT_XOR
     75,  // TEXT_XOR
     89,  // TEXT_XOR
    104,  // TEXT_XOR
    123,  // TEXT_XOR
    136,  // TEXT_XOR
    150,  // TEXT_XOR
};

constexpr Entry TMS370_TF4[] PROGMEM = {
    E2(0xE8, TEXT_MOVW,  M_RIMM, M_RN),
    E2(0xEA, TEXT_MOV,   M_RIDX, M_A),
    E2(0xEB, TEXT_MOV,   M_A,    M_RIDX),
    E2(0xED, TEXT_CMP,   M_RIDX, M_A),
    E2(0xF8, TEXT_DIV,   M_RN,   M_A),
    E1(0xE9, TEXT_JMPL,  M_RIDX),
    E1(0xEC, TEXT_BR,    M_RIDX),
    E1(0xEE, TEXT_CALL,  M_RIDX),
    E1(0xEF, TEXT_CALLR, M_RIDX),
};

constexpr uint8_t TMS370_IF4[] PROGMEM = {
      6,  // TEXT_BR
      7,  // TEXT_CALL
      8,  // TEXT_CALLR
      3,  // TEXT_CMP
      4,  // TEXT_DIV
      5,  // TEXT_JMPL
      1,  // TEXT_MOV
      2,  // TEXT_MOV
      0,  // TEXT_MOVW
};

constexpr Entry TMS370_TF0[] PROGMEM = {
    E0(0x00, TEXT_DINT),
    E0(0x04, TEXT_EINTH),
    E0(0x08, TEXT_EINTL),
    E0(0x0C, TEXT_EINT),
};

constexpr uint8_t TMS370_IF0[] PROGMEM = {
      0,  // TEXT_DINT
      3,  // TEXT_EINT
      1,  // TEXT_EINTH
      2,  // TEXT_EINTL
};

// clang-format on

using EntryPage = entry::PrefixTableBase<Entry>;

constexpr EntryPage TMS370_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(TMS370_TABLE), ARRAY_RANGE(TMS370_INDEX)},
        {0xF4, ARRAY_RANGE(TMS370_TF4), ARRAY_RANGE(TMS370_IF4)},
        {0xF0, ARRAY_RANGE(TMS370_TF0), ARRAY_RANGE(TMS370_IF0)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {TMS370, TEXT_CPU_TMS370, ARRAY_RANGE(TMS370_PAGES)},
};

const Cpu *cpu(CpuType) {
    return &CPU_TABLE[0];
}

bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_A || opr == M_B)
        return table == M_RN;
    if (opr == M_IM16)
        return table == M_IM8 || table == M_SIM8;
    if (opr == M_ADRR)
        return table == M_RN || table == M_REL || table == M_TVEC;
    if (opr == M_ADRP)
        return table == M_PN || table == M_REL;
    if (opr == M_REL)
        return table == M_ABS || table == M_LREL;
    if (opr == M_BIDX)
        return table == M_BREL;
    return false;
}

bool acceptModes(AsmInsn &insn, const Entry *entry) {
    const auto table = entry->readFlags();
    return acceptMode(insn.srcOp.mode, table.src()) && acceptMode(insn.dstOp.mode, table.dst()) &&
           acceptMode(insn.extOp.mode, table.ext());
}

Error searchName(CpuType cpuType, AsmInsn &insn) {
    cpu(cpuType)->searchName(insn, acceptModes);
    return insn.getError();
}

bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *) {
    auto opc = insn.opCode();
    const auto flags = entry->readFlags();
    const auto src = flags.src();
    if (src == M_TVEC)
        opc &= ~0x0F;
    return opc == entry->readOpCode();
}

Error searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) {
    cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    return insn.getError();
}

bool isPrefix(CpuType cpuType, Config::opcode_t code) {
    return cpu(cpuType)->isPrefix(code) && code != 0xF0;
}

const /*PROGMEM*/ char *TableTms370::listCpu_P() const {
    return TEXT_CPU_TMS370;
}

const /*PROGMEM*/ char *TableTms370::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableTms370::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    if (name.iequals(TEXT_CPU_TMS370) || strncasecmp_P(TEXT_CPU_TMS370C, name.str(), 7) == 0) {
        cpuType = TMS370;
        return OK;
    }
    return UNSUPPORTED_CPU;
}

const TableTms370 TABLE;

}  // namespace tms370
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

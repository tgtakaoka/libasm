/*
 * Copyright 2023 Tadashi G. Takaoka
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

#include "table_tms7000.h"

#include "entry_table.h"
#include "entry_tms7000.h"
#include "text_tms7000.h"

using namespace libasm::text::tms7000;

namespace libasm {
namespace tms7000 {

#define E3(_opc, _name, _opr1, _opr2, _opr3) \
    { _opc, Entry::Flags::create(_opr1, _opr2, _opr3), _name }
#define E2(_opc, _name, _opr1, _opr2) E3(_opc, _name, _opr1, _opr2, M_NONE)
#define E1(_opc, _name, _opr1) E2(_opc, _name, _opr1, M_NONE)
#define E0(_opc, _name) E1(_opc, _name, M_NONE)

// clang-format off
static constexpr Entry TMS7000_TABLE[] PROGMEM = {
	E0(0x00, TEXT_NOP),
	E0(0x01, TEXT_IDLE),
	E0(0x05, TEXT_EINT),
	E0(0x06, TEXT_DINT),
	E0(0x07, TEXT_SETC),
	E1(0x08, TEXT_POP,   M_ST),
	E0(0x09, TEXT_STSP),
	E0(0x0A, TEXT_RETS),
	E0(0x0B, TEXT_RETI),
	E0(0x0D, TEXT_LDSP),
	E1(0x0E, TEXT_PUSH,  M_ST),
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
	E2(0x1E, TEXT_DAC,   M_RN,   M_A),
	E2(0x1F, TEXT_DSB,   M_RN,   M_A),
	E2(0x22, TEXT_MOV,   M_IM8,  M_A),
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
	E2(0x80, TEXT_MOVP,  M_PN,   M_A),
	E2(0x82, TEXT_MOVP,  M_A,    M_PN),
	E2(0x83, TEXT_ANDP,  M_A,    M_PN),
	E2(0x84, TEXT_ORP,   M_A,    M_PN),
	E2(0x85, TEXT_XORP,  M_A,    M_PN),
	E3(0x86, TEXT_BTJOP, M_A,    M_PN,  M_REL),
	E3(0x87, TEXT_BTJZP, M_A,    M_PN,  M_REL),
	E2(0x88, TEXT_MOVD,  M_IM16,  M_RN),
	E1(0x8A, TEXT_LDA,   M_ABS),
	E1(0x8B, TEXT_STA,   M_ABS),
	E1(0x8C, TEXT_BR,    M_ABS),
	E1(0x8D, TEXT_CMPA,  M_ABS),
	E1(0x8E, TEXT_CALL,  M_ABS),
	E2(0x91, TEXT_MOVP,  M_PN,   M_B),
	E2(0x92, TEXT_MOVP,  M_B,    M_PN),
	E2(0x93, TEXT_ANDP,  M_B,    M_PN),
	E2(0x94, TEXT_ORP,   M_B,    M_PN),
	E2(0x95, TEXT_XORP,  M_B,    M_PN),
	E3(0x96, TEXT_BTJOP, M_B,    M_PN,  M_REL),
	E3(0x97, TEXT_BTJZP, M_B,    M_PN,  M_REL),
	E2(0x98, TEXT_MOVD,  M_RN,   M_RN),
	E1(0x9A, TEXT_LDA,   M_IDIR),
	E1(0x9B, TEXT_STA,   M_IDIR),
	E1(0x9C, TEXT_BR,    M_IDIR),
	E1(0x9D, TEXT_CMPA,  M_IDIR),
	E1(0x9E, TEXT_CALL,  M_IDIR),
	E2(0xA2, TEXT_MOVP,  M_IM8,  M_PN),
	E2(0xA3, TEXT_ANDP,  M_IM8,  M_PN),
	E2(0xA4, TEXT_ORP,   M_IM8,  M_PN),
	E2(0xA5, TEXT_XORP,  M_IM8,  M_PN),
	E3(0xA6, TEXT_BTJOP, M_IM8,  M_PN,  M_REL),
	E3(0xA7, TEXT_BTJZP, M_IM8,  M_PN,  M_REL),
	E2(0xA8, TEXT_MOVD,  M_BIMM, M_RN),
	E1(0xAA, TEXT_LDA,   M_BIDX),
	E1(0xAB, TEXT_STA,   M_BIDX),
	E1(0xAC, TEXT_BR,    M_BIDX),
	E1(0xAD, TEXT_CMPA,  M_BIDX),
	E1(0xAE, TEXT_CALL,  M_BIDX),
	E0(0xB0, TEXT_TSTA),
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
	E1(0xBB, TEXT_DECD,  M_A),
	E1(0xBC, TEXT_RR,    M_A),
	E1(0xBD, TEXT_RRC,   M_A),
	E1(0xBE, TEXT_RL,    M_A),
	E1(0xBF, TEXT_RLC,   M_A),
	E0(0xC1, TEXT_TSTB),
	E1(0xC2, TEXT_DEC,   M_B),
	E1(0xC3, TEXT_INC,   M_B),
	E1(0xC4, TEXT_INV,   M_B),
	E1(0xC5, TEXT_CLR,   M_B),
	E1(0xC6, TEXT_XCHB,  M_B),
	E1(0xC7, TEXT_SWAP,  M_B),
	E1(0xC8, TEXT_PUSH,  M_B),
	E1(0xC9, TEXT_POP,   M_B),
	E2(0xCA, TEXT_DJNZ,  M_B,    M_REL),
	E1(0xCB, TEXT_DECD,  M_B),
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
	E1(0xDB, TEXT_DECD,  M_RN),
	E1(0xDC, TEXT_RR,    M_RN),
	E1(0xDD, TEXT_RRC,   M_RN),
	E1(0xDE, TEXT_RL,    M_RN),
	E1(0xDF, TEXT_RLC,   M_RN),
	E1(0xE0, TEXT_JMP,   M_REL),
	E1(0xE1, TEXT_JN,    M_REL),
	E1(0xE1, TEXT_JLT,   M_REL),
	E1(0xE2, TEXT_JZ,    M_REL),
	E1(0xE2, TEXT_JEQ,   M_REL),
	E1(0xE3, TEXT_JC,    M_REL),
	E1(0xE3, TEXT_JHS,   M_REL),
	E1(0xE4, TEXT_JP,    M_REL),
	E1(0xE4, TEXT_JGT,   M_REL),
	E1(0xE5, TEXT_JPZ,   M_REL),
	E1(0xE5, TEXT_JGE,   M_REL),
	E1(0xE6, TEXT_JNZ,   M_REL),
	E1(0xE6, TEXT_JNE,   M_REL),
	E1(0xE7, TEXT_JNC,   M_REL),
	E1(0xE7, TEXT_JL,    M_REL),
	E1(0xE8, TEXT_TRAP,  M_TRAP),
};

static constexpr uint8_t TMS7000_INDEX[] PROGMEM = {
     21,  // TEXT_ADC
     35,  // TEXT_ADC
     49,  // TEXT_ADC
     63,  // TEXT_ADC
     77,  // TEXT_ADC
     91,  // TEXT_ADC
    105,  // TEXT_ADC
     20,  // TEXT_ADD
     34,  // TEXT_ADD
     48,  // TEXT_ADD
     62,  // TEXT_ADD
     76,  // TEXT_ADD
     90,  // TEXT_ADD
    104,  // TEXT_ADD
     15,  // TEXT_AND
     29,  // TEXT_AND
     43,  // TEXT_AND
     57,  // TEXT_AND
     71,  // TEXT_AND
     85,  // TEXT_AND
     99,  // TEXT_AND
    114,  // TEXT_ANDP
    127,  // TEXT_ANDP
    139,  // TEXT_ANDP
    122,  // TEXT_BR
    135,  // TEXT_BR
    147,  // TEXT_BR
     18,  // TEXT_BTJO
     32,  // TEXT_BTJO
     46,  // TEXT_BTJO
     60,  // TEXT_BTJO
     74,  // TEXT_BTJO
     88,  // TEXT_BTJO
    102,  // TEXT_BTJO
    117,  // TEXT_BTJOP
    130,  // TEXT_BTJOP
    142,  // TEXT_BTJOP
     19,  // TEXT_BTJZ
     33,  // TEXT_BTJZ
     47,  // TEXT_BTJZ
     61,  // TEXT_BTJZ
     75,  // TEXT_BTJZ
     89,  // TEXT_BTJZ
    103,  // TEXT_BTJZ
    118,  // TEXT_BTJZP
    131,  // TEXT_BTJZP
    143,  // TEXT_BTJZP
    124,  // TEXT_CALL
    137,  // TEXT_CALL
    149,  // TEXT_CALL
    155,  // TEXT_CLR
    170,  // TEXT_CLR
    184,  // TEXT_CLR
    151,  // TEXT_CLRC
     25,  // TEXT_CMP
     39,  // TEXT_CMP
     53,  // TEXT_CMP
     67,  // TEXT_CMP
     81,  // TEXT_CMP
     95,  // TEXT_CMP
    109,  // TEXT_CMP
    123,  // TEXT_CMPA
    136,  // TEXT_CMPA
    148,  // TEXT_CMPA
     26,  // TEXT_DAC
     40,  // TEXT_DAC
     54,  // TEXT_DAC
     68,  // TEXT_DAC
     82,  // TEXT_DAC
     96,  // TEXT_DAC
    110,  // TEXT_DAC
    152,  // TEXT_DEC
    167,  // TEXT_DEC
    181,  // TEXT_DEC
    161,  // TEXT_DECD
    176,  // TEXT_DECD
    190,  // TEXT_DECD
      3,  // TEXT_DINT
    160,  // TEXT_DJNZ
    175,  // TEXT_DJNZ
    189,  // TEXT_DJNZ
     27,  // TEXT_DSB
     41,  // TEXT_DSB
     55,  // TEXT_DSB
     69,  // TEXT_DSB
     83,  // TEXT_DSB
     97,  // TEXT_DSB
    111,  // TEXT_DSB
      2,  // TEXT_EINT
      1,  // TEXT_IDLE
    153,  // TEXT_INC
    168,  // TEXT_INC
    182,  // TEXT_INC
    154,  // TEXT_INV
    169,  // TEXT_INV
    183,  // TEXT_INV
    200,  // TEXT_JC
    199,  // TEXT_JEQ
    205,  // TEXT_JGE
    203,  // TEXT_JGT
    201,  // TEXT_JHS
    209,  // TEXT_JL
    197,  // TEXT_JLT
    195,  // TEXT_JMP
    196,  // TEXT_JN
    208,  // TEXT_JNC
    207,  // TEXT_JNE
    206,  // TEXT_JNZ
    202,  // TEXT_JP
    204,  // TEXT_JPZ
    198,  // TEXT_JZ
    120,  // TEXT_LDA
    133,  // TEXT_LDA
    145,  // TEXT_LDA
      9,  // TEXT_LDSP
     11,  // TEXT_MOV
     12,  // TEXT_MOV
     13,  // TEXT_MOV
     14,  // TEXT_MOV
     28,  // TEXT_MOV
     42,  // TEXT_MOV
     56,  // TEXT_MOV
     70,  // TEXT_MOV
     84,  // TEXT_MOV
     98,  // TEXT_MOV
    119,  // TEXT_MOVD
    132,  // TEXT_MOVD
    144,  // TEXT_MOVD
    112,  // TEXT_MOVP
    113,  // TEXT_MOVP
    125,  // TEXT_MOVP
    126,  // TEXT_MOVP
    138,  // TEXT_MOVP
     24,  // TEXT_MPY
     38,  // TEXT_MPY
     52,  // TEXT_MPY
     66,  // TEXT_MPY
     80,  // TEXT_MPY
     94,  // TEXT_MPY
    108,  // TEXT_MPY
      0,  // TEXT_NOP
     16,  // TEXT_OR
     30,  // TEXT_OR
     44,  // TEXT_OR
     58,  // TEXT_OR
     72,  // TEXT_OR
     86,  // TEXT_OR
    100,  // TEXT_OR
    115,  // TEXT_ORP
    128,  // TEXT_ORP
    140,  // TEXT_ORP
      5,  // TEXT_POP
    159,  // TEXT_POP
    174,  // TEXT_POP
    188,  // TEXT_POP
     10,  // TEXT_PUSH
    158,  // TEXT_PUSH
    173,  // TEXT_PUSH
    187,  // TEXT_PUSH
      8,  // TEXT_RETI
      7,  // TEXT_RETS
    164,  // TEXT_RL
    179,  // TEXT_RL
    193,  // TEXT_RL
    165,  // TEXT_RLC
    180,  // TEXT_RLC
    194,  // TEXT_RLC
    162,  // TEXT_RR
    177,  // TEXT_RR
    191,  // TEXT_RR
    163,  // TEXT_RRC
    178,  // TEXT_RRC
    192,  // TEXT_RRC
     23,  // TEXT_SBB
     37,  // TEXT_SBB
     51,  // TEXT_SBB
     65,  // TEXT_SBB
     79,  // TEXT_SBB
     93,  // TEXT_SBB
    107,  // TEXT_SBB
      4,  // TEXT_SETC
    121,  // TEXT_STA
    134,  // TEXT_STA
    146,  // TEXT_STA
      6,  // TEXT_STSP
     22,  // TEXT_SUB
     36,  // TEXT_SUB
     50,  // TEXT_SUB
     64,  // TEXT_SUB
     78,  // TEXT_SUB
     92,  // TEXT_SUB
    106,  // TEXT_SUB
    157,  // TEXT_SWAP
    172,  // TEXT_SWAP
    186,  // TEXT_SWAP
    210,  // TEXT_TRAP
    150,  // TEXT_TSTA
    166,  // TEXT_TSTB
    156,  // TEXT_XCHB
    171,  // TEXT_XCHB
    185,  // TEXT_XCHB
     17,  // TEXT_XOR
     31,  // TEXT_XOR
     45,  // TEXT_XOR
     59,  // TEXT_XOR
     73,  // TEXT_XOR
     87,  // TEXT_XOR
    101,  // TEXT_XOR
    116,  // TEXT_XORP
    129,  // TEXT_XORP
    141,  // TEXT_XORP
};

// clang-format on

using EntryPage = entry::TableBase<Entry>;

static constexpr EntryPage TMS7000_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TMS7000_TABLE), ARRAY_RANGE(TMS7000_INDEX)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

static constexpr Cpu CPU_TABLE[] PROGMEM = {
        {TMS7000, TEXT_CPU_TMS7000, ARRAY_RANGE(TMS7000_PAGES)},
};

static const Cpu *cpu(CpuType cpuType) {
    UNUSED(cpuType);
    return &CPU_TABLE[0];
}

static bool acceptAll(AsmInsn &insn, const Entry *entry) {
    UNUSED(insn);
    UNUSED(entry);
    return true;
}

bool TableTms7000::hasOperand(CpuType cpuType, AsmInsn &insn) const {
    cpu(cpuType)->searchName(insn, acceptAll);
    return insn.isOK() && insn.src() != M_NONE;
}

static bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_A || opr == M_B)
        return table == M_RN;
    if (opr == M_IM8)
        return table == M_IM16;
    if (opr == M_ADRR)
        return table == M_RN || table == M_REL || table == M_TRAP;
    if (opr == M_ADRP)
        return table == M_PN || table == M_REL;
    return false;
}

static bool acceptModes(AsmInsn &insn, const Entry *entry) {
    const auto table = entry->flags();
    return acceptMode(insn.srcOp.mode, table.src()) && acceptMode(insn.dstOp.mode, table.dst()) &&
           acceptMode(insn.extOp.mode, table.ext());
}

Error TableTms7000::searchName(CpuType cpuType, AsmInsn &insn) const {
    cpu(cpuType)->searchName(insn, acceptModes);
    return insn.getError();
}

static bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *page) {
    UNUSED(page);
    const auto opc = insn.opCode();
    const auto flags = entry->flags();
    const auto src = flags.src();
    if (src == M_TRAP)
        return opc >= entry->opCode();
    return opc == entry->opCode();
}

Error TableTms7000::searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) const {
    cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    return insn.getError();
}

const /*PROGMEM*/ char *TableTms7000::listCpu_P() const {
    return TEXT_CPU_TMS7000;
}

const /*PROGMEM*/ char *TableTms7000::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableTms7000::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    if (name.iequals(TEXT_CPU_TMS7000) || name.iequals(TEXT_CPU_TMS70C00)) {
        cpuType = TMS7000;
        return OK;
    }
    return UNSUPPORTED_CPU;
}

const TableTms7000 TABLE;

}  // namespace tms7000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

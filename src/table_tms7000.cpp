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
	E2(0x62, TEXT_MOV,   M_B,    M_A),
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
	E2(0xC0, TEXT_MOV,   M_A,    M_B),
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
	E2(0xD0, TEXT_MOV,   M_A,    M_RN),
	E2(0xD1, TEXT_MOV,   M_B,    M_RN),
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
     18,  // TEXT_ADC
     32,  // TEXT_ADC
     46,  // TEXT_ADC
     60,  // TEXT_ADC
     74,  // TEXT_ADC
     88,  // TEXT_ADC
    102,  // TEXT_ADC
     17,  // TEXT_ADD
     31,  // TEXT_ADD
     45,  // TEXT_ADD
     59,  // TEXT_ADD
     73,  // TEXT_ADD
     87,  // TEXT_ADD
    101,  // TEXT_ADD
     12,  // TEXT_AND
     26,  // TEXT_AND
     40,  // TEXT_AND
     54,  // TEXT_AND
     68,  // TEXT_AND
     82,  // TEXT_AND
     96,  // TEXT_AND
    111,  // TEXT_ANDP
    124,  // TEXT_ANDP
    136,  // TEXT_ANDP
    119,  // TEXT_BR
    132,  // TEXT_BR
    144,  // TEXT_BR
     15,  // TEXT_BTJO
     29,  // TEXT_BTJO
     43,  // TEXT_BTJO
     57,  // TEXT_BTJO
     71,  // TEXT_BTJO
     85,  // TEXT_BTJO
     99,  // TEXT_BTJO
    114,  // TEXT_BTJOP
    127,  // TEXT_BTJOP
    139,  // TEXT_BTJOP
     16,  // TEXT_BTJZ
     30,  // TEXT_BTJZ
     44,  // TEXT_BTJZ
     58,  // TEXT_BTJZ
     72,  // TEXT_BTJZ
     86,  // TEXT_BTJZ
    100,  // TEXT_BTJZ
    115,  // TEXT_BTJZP
    128,  // TEXT_BTJZP
    140,  // TEXT_BTJZP
    121,  // TEXT_CALL
    134,  // TEXT_CALL
    146,  // TEXT_CALL
    152,  // TEXT_CLR
    168,  // TEXT_CLR
    184,  // TEXT_CLR
    148,  // TEXT_CLRC
     22,  // TEXT_CMP
     36,  // TEXT_CMP
     50,  // TEXT_CMP
     64,  // TEXT_CMP
     78,  // TEXT_CMP
     92,  // TEXT_CMP
    106,  // TEXT_CMP
    120,  // TEXT_CMPA
    133,  // TEXT_CMPA
    145,  // TEXT_CMPA
     23,  // TEXT_DAC
     37,  // TEXT_DAC
     51,  // TEXT_DAC
     65,  // TEXT_DAC
     79,  // TEXT_DAC
     93,  // TEXT_DAC
    107,  // TEXT_DAC
    149,  // TEXT_DEC
    165,  // TEXT_DEC
    181,  // TEXT_DEC
    158,  // TEXT_DECD
    174,  // TEXT_DECD
    190,  // TEXT_DECD
      3,  // TEXT_DINT
    157,  // TEXT_DJNZ
    173,  // TEXT_DJNZ
    189,  // TEXT_DJNZ
     24,  // TEXT_DSB
     38,  // TEXT_DSB
     52,  // TEXT_DSB
     66,  // TEXT_DSB
     80,  // TEXT_DSB
     94,  // TEXT_DSB
    108,  // TEXT_DSB
      2,  // TEXT_EINT
      1,  // TEXT_IDLE
    150,  // TEXT_INC
    166,  // TEXT_INC
    182,  // TEXT_INC
    151,  // TEXT_INV
    167,  // TEXT_INV
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
    117,  // TEXT_LDA
    130,  // TEXT_LDA
    142,  // TEXT_LDA
      9,  // TEXT_LDSP
     11,  // TEXT_MOV
     25,  // TEXT_MOV
     39,  // TEXT_MOV
     53,  // TEXT_MOV
     67,  // TEXT_MOV
     81,  // TEXT_MOV
     95,  // TEXT_MOV
    163,  // TEXT_MOV
    179,  // TEXT_MOV
    180,  // TEXT_MOV
    116,  // TEXT_MOVD
    129,  // TEXT_MOVD
    141,  // TEXT_MOVD
    109,  // TEXT_MOVP
    110,  // TEXT_MOVP
    122,  // TEXT_MOVP
    123,  // TEXT_MOVP
    135,  // TEXT_MOVP
     21,  // TEXT_MPY
     35,  // TEXT_MPY
     49,  // TEXT_MPY
     63,  // TEXT_MPY
     77,  // TEXT_MPY
     91,  // TEXT_MPY
    105,  // TEXT_MPY
      0,  // TEXT_NOP
     13,  // TEXT_OR
     27,  // TEXT_OR
     41,  // TEXT_OR
     55,  // TEXT_OR
     69,  // TEXT_OR
     83,  // TEXT_OR
     97,  // TEXT_OR
    112,  // TEXT_ORP
    125,  // TEXT_ORP
    137,  // TEXT_ORP
      5,  // TEXT_POP
    156,  // TEXT_POP
    172,  // TEXT_POP
    188,  // TEXT_POP
     10,  // TEXT_PUSH
    155,  // TEXT_PUSH
    171,  // TEXT_PUSH
    187,  // TEXT_PUSH
      8,  // TEXT_RETI
      7,  // TEXT_RETS
    161,  // TEXT_RL
    177,  // TEXT_RL
    193,  // TEXT_RL
    162,  // TEXT_RLC
    178,  // TEXT_RLC
    194,  // TEXT_RLC
    159,  // TEXT_RR
    175,  // TEXT_RR
    191,  // TEXT_RR
    160,  // TEXT_RRC
    176,  // TEXT_RRC
    192,  // TEXT_RRC
     20,  // TEXT_SBB
     34,  // TEXT_SBB
     48,  // TEXT_SBB
     62,  // TEXT_SBB
     76,  // TEXT_SBB
     90,  // TEXT_SBB
    104,  // TEXT_SBB
      4,  // TEXT_SETC
    118,  // TEXT_STA
    131,  // TEXT_STA
    143,  // TEXT_STA
      6,  // TEXT_STSP
     19,  // TEXT_SUB
     33,  // TEXT_SUB
     47,  // TEXT_SUB
     61,  // TEXT_SUB
     75,  // TEXT_SUB
     89,  // TEXT_SUB
    103,  // TEXT_SUB
    154,  // TEXT_SWAP
    170,  // TEXT_SWAP
    186,  // TEXT_SWAP
    210,  // TEXT_TRAP
    147,  // TEXT_TSTA
    164,  // TEXT_TSTB
    153,  // TEXT_XCHB
    169,  // TEXT_XCHB
    185,  // TEXT_XCHB
     14,  // TEXT_XOR
     28,  // TEXT_XOR
     42,  // TEXT_XOR
     56,  // TEXT_XOR
     70,  // TEXT_XOR
     84,  // TEXT_XOR
     98,  // TEXT_XOR
    113,  // TEXT_XORP
    126,  // TEXT_XORP
    138,  // TEXT_XORP
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
static constexpr const Cpu &TMS7000_CPU = CPU_TABLE[0];

static bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *page) {
    auto opCode = insn.opCode();
    const auto flags = entry->flags();
    const auto src = flags.src();
    if (src == M_TRAP)
        return opCode >= entry->opCode();
    return opCode == entry->opCode();
}

Error TableTms7000::searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) const {
    TMS7000_CPU.searchOpCode(insn, out, matchOpCode);
    return insn.getError();
}

const /*PROGMEM*/ char *TableTms7000::listCpu_P() const {
    return TEXT_CPU_TMS7000;
}

const /*PROGMEM*/ char *TableTms7000::cpuName_P(CpuType cpuType) const {
    return TMS7000_CPU.name_P();
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

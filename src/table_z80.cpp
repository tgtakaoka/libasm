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
#include "entry_table.h"
#include "entry_z80.h"
#include "text_z80.h"

using namespace libasm::text::z80;

namespace libasm {
namespace z80 {

#define E2(_opc, _mask, _name, _dst, _src) {_opc, Entry::Flags::create(_mask, _dst, _src), _name}
#define E1(_opc, _mask, _name, _dst) E2(_opc, _mask, _name, _dst, M_NONE)
#define E0(_opc, _mask, _name) E1(_opc, _mask, _name, M_NONE)
#define I2(_opc, _mask, _name, _dst, _src) {_opc, Entry::Flags::imcapable(_mask, _dst, _src), _name}
#define I1(_opc, _mask, _name, _dst) I2(_opc, _mask, _name, _dst, M_NONE)
#define I0(_opc, _mask, _name) I1(_opc, _mask, _name, M_NONE)
#define L2(_opc, _mask, _name, _dst, _src) {_opc, Entry::Flags::lmcapable(_mask, _dst, _src), _name}
#define L1(_opc, _mask, _name, _dst) L2(_opc, _mask, _name, _dst, M_NONE)
#define L0(_opc, _mask, _name) L1(_opc, _mask, _name, M_NONE)
#define D2(_opc, _mask, _name, _dst, _src) \
    {_opc, Entry::Flags::ilmcapable(_mask, _dst, _src), _name}
#define D1(_opc, _mask, _name, _dst) D2(_opc, _mask, _name, _dst, M_NONE)
#define D0(_opc, _mask, _name) D1(_opc, _mask, _name, M_NONE)

// clang-format off
constexpr Entry TABLE_Z80[] PROGMEM = {
    E0(0x00, 0000, TEXT_NOP),
    D2(0x01, 0060, TEXT_LD,   M_R16,  M_LM16),
    E2(0x09, 0060, TEXT_ADD,  R_HL,   M_R16),
    E2(0x02, 0020, TEXT_LD,   I_PTR,  R_A),
    E2(0x0A, 0020, TEXT_LD,   R_A,    I_PTR),
    D2(0x22, 0000, TEXT_LD,   M_DABS, R_HL),
    D2(0x2A, 0000, TEXT_LD,   R_HL,   M_DABS),
    I2(0x32, 0000, TEXT_LD,   M_DABS, R_A),
    I2(0x3A, 0000, TEXT_LD,   R_A,    M_DABS),
    E1(0x03, 0060, TEXT_INC,  M_R16),
    E1(0x0B, 0060, TEXT_DEC,  M_R16),
    E1(0x04, 0070, TEXT_INC,  M_DST),
    E1(0x05, 0070, TEXT_DEC,  M_DST),
    E2(0x06, 0070, TEXT_LD,   M_DST, M_IM8),
    E0(0x07, 0000, TEXT_RLCA),
    E0(0x0F, 0000, TEXT_RRCA),
    E0(0x17, 0000, TEXT_RLA),
    E0(0x1F, 0000, TEXT_RRA),
    E0(0x27, 0000, TEXT_DAA),
    E0(0x2F, 0000, TEXT_CPL),
    E0(0x37, 0000, TEXT_SCF),
    E0(0x3F, 0000, TEXT_CCF),
    E0(0x76, 0000, TEXT_HALT),
    E2(0x40, 0077, TEXT_LD,   M_DST, M_SR8),
    E2(0x40, 0077, TEXT_LD,   M_DR8, M_SRC),
    E2(0x80, 0007, TEXT_ADD,  R_A,   M_SRC),
    E2(0x88, 0007, TEXT_ADC,  R_A,   M_SRC),
    E2(0x98, 0007, TEXT_SBC,  R_A,   M_SRC),
    E1(0x90, 0007, TEXT_SUB,  M_SRC),
    E2(0x90, 0007, TEXT_SUB,  R_A,   M_SRC),
    E1(0xA0, 0007, TEXT_AND,  M_SRC),
    E2(0xA0, 0007, TEXT_AND,  R_A,   M_SRC),
    E1(0xA8, 0007, TEXT_XOR,  M_SRC),
    E2(0xA8, 0007, TEXT_XOR,  R_A,   M_SRC),
    E1(0xB0, 0007, TEXT_OR,   M_SRC),
    E2(0xB0, 0007, TEXT_OR,   R_A,   M_SRC),
    E1(0xB8, 0007, TEXT_CP,   M_SRC),
    E2(0xB8, 0007, TEXT_CP,   R_A,   M_SRC),
    E1(0xD6, 0000, TEXT_SUB,  M_IM8),
    E1(0xE6, 0000, TEXT_AND,  M_IM8),
    E1(0xEE, 0000, TEXT_XOR,  M_IM8),
    E1(0xF6, 0000, TEXT_OR,   M_IM8),
    E1(0xFE, 0000, TEXT_CP,   M_IM8),
    E1(0xC0, 0070, TEXT_RET,  M_CC8),
    L1(0xC1, 0020, TEXT_POP,  R_PTRH),
    L1(0xE1, 0000, TEXT_POP,  R_HL),
    L1(0xF1, 0000, TEXT_POP,  R_AF),
    E0(0xC9, 0000, TEXT_RET),
    L2(0xF9, 0000, TEXT_LD,   R_SP,  R_HL),
    I2(0xC2, 0070, TEXT_JP,   M_CC8, M_JABS),
    I1(0xC3, 0000, TEXT_JP,   M_JABS),
    E1(0xE9, 0000, TEXT_JP,   I_HL),
    E2(0xD3, 0000, TEXT_OUT,  M_IOA, R_A),
    E2(0xDB, 0000, TEXT_IN,   R_A,   M_IOA),
    L2(0xE3, 0000, TEXT_EX,   I_SP,  R_HL),
    L2(0xE3, 0000, TEXT_EX,   R_HL,  I_SP),
    L2(0xEB, 0000, TEXT_EX,   R_DE,  R_HL),
    L2(0xEB, 0000, TEXT_EX,   R_HL,  R_DE),
    E0(0xF3, 0000, TEXT_DI),
    E0(0xFB, 0000, TEXT_EI),
    I2(0xC4, 0070, TEXT_CALL, M_CC8, M_JABS),
    L1(0xC5, 0020, TEXT_PUSH, R_PTRH),
    L1(0xE5, 0000, TEXT_PUSH, R_HL),
    L1(0xF5, 0000, TEXT_PUSH, R_AF),
    I1(0xCD, 0000, TEXT_CALL, M_JABS),
    E2(0xC6, 0000, TEXT_ADD,  R_A,   M_IM8),
    E2(0xCE, 0000, TEXT_ADC,  R_A,   M_IM8),
    E2(0xD6, 0000, TEXT_SUB,  R_A,   M_IM8),
    E2(0xDE, 0000, TEXT_SBC,  R_A,   M_IM8),
    E2(0xE6, 0000, TEXT_AND,  R_A,   M_IM8),
    E2(0xEE, 0000, TEXT_XOR,  R_A,   M_IM8),
    E2(0xF6, 0000, TEXT_OR,   R_A,   M_IM8),
    E2(0xFE, 0000, TEXT_CP,   R_A,   M_IM8),
    E1(0xC7, 0070, TEXT_RST,  M_VEC),
    E2(0x08, 0000, TEXT_EX,   R_AF,  R_ALT),
    E1(0x10, 0000, TEXT_DJNZ, M_REL8),
    E2(0x20, 0030, TEXT_JR,   M_CC4, M_REL8),
    E1(0x18, 0000, TEXT_JR,   M_REL8),
    E0(0xD9, 0000, TEXT_EXX),
};

constexpr uint8_t INDEX_Z80[] PROGMEM = {
     26,  // TEXT_ADC
     66,  // TEXT_ADC
      2,  // TEXT_ADD
     25,  // TEXT_ADD
     65,  // TEXT_ADD
     30,  // TEXT_AND
     31,  // TEXT_AND
     39,  // TEXT_AND
     69,  // TEXT_AND
     60,  // TEXT_CALL
     64,  // TEXT_CALL
     21,  // TEXT_CCF
     36,  // TEXT_CP
     37,  // TEXT_CP
     42,  // TEXT_CP
     72,  // TEXT_CP
     19,  // TEXT_CPL
     18,  // TEXT_DAA
     10,  // TEXT_DEC
     12,  // TEXT_DEC
     58,  // TEXT_DI
     75,  // TEXT_DJNZ
     59,  // TEXT_EI
     54,  // TEXT_EX
     55,  // TEXT_EX
     56,  // TEXT_EX
     57,  // TEXT_EX
     74,  // TEXT_EX
     78,  // TEXT_EXX
     22,  // TEXT_HALT
     53,  // TEXT_IN
      9,  // TEXT_INC
     11,  // TEXT_INC
     49,  // TEXT_JP
     50,  // TEXT_JP
     51,  // TEXT_JP
     76,  // TEXT_JR
     77,  // TEXT_JR
      1,  // TEXT_LD
      3,  // TEXT_LD
      4,  // TEXT_LD
      5,  // TEXT_LD
      6,  // TEXT_LD
      7,  // TEXT_LD
      8,  // TEXT_LD
     13,  // TEXT_LD
     23,  // TEXT_LD
     24,  // TEXT_LD
     48,  // TEXT_LD
      0,  // TEXT_NOP
     34,  // TEXT_OR
     35,  // TEXT_OR
     41,  // TEXT_OR
     71,  // TEXT_OR
     52,  // TEXT_OUT
     44,  // TEXT_POP
     45,  // TEXT_POP
     46,  // TEXT_POP
     61,  // TEXT_PUSH
     62,  // TEXT_PUSH
     63,  // TEXT_PUSH
     43,  // TEXT_RET
     47,  // TEXT_RET
     16,  // TEXT_RLA
     14,  // TEXT_RLCA
     17,  // TEXT_RRA
     15,  // TEXT_RRCA
     73,  // TEXT_RST
     27,  // TEXT_SBC
     68,  // TEXT_SBC
     20,  // TEXT_SCF
     28,  // TEXT_SUB
     29,  // TEXT_SUB
     38,  // TEXT_SUB
     67,  // TEXT_SUB
     32,  // TEXT_XOR
     33,  // TEXT_XOR
     40,  // TEXT_XOR
     70,  // TEXT_XOR
};

// CB:xx
constexpr Entry TABLE_BIT[] PROGMEM = {
    E1(0x00, 0007, TEXT_RLC, M_SRC),
    E1(0x08, 0007, TEXT_RRC, M_SRC),
    E1(0x10, 0007, TEXT_RL,  M_SRC),
    E1(0x18, 0007, TEXT_RR,  M_SRC),
    E1(0x20, 0007, TEXT_SLA, M_SRC),
    E1(0x28, 0007, TEXT_SRA, M_SRC),
    E1(0x38, 0007, TEXT_SRL, M_SRC),
    E2(0x40, 0077, TEXT_BIT, M_BIT, M_SRC),
    E2(0x80, 0077, TEXT_RES, M_BIT, M_SRC),
    E2(0xC0, 0077, TEXT_SET, M_BIT, M_SRC),
};

constexpr uint8_t INDEX_BIT[] PROGMEM = {
      7,  // TEXT_BIT
      8,  // TEXT_RES
      2,  // TEXT_RL
      0,  // TEXT_RLC
      3,  // TEXT_RR
      1,  // TEXT_RRC
      9,  // TEXT_SET
      4,  // TEXT_SLA
      5,  // TEXT_SRA
      6,  // TEXT_SRL
};

// ED:xx
constexpr Entry TABLE_EXT[] PROGMEM = {
    E2(0x40, 0070, TEXT_IN,   M_DR8,  I_C),
    E2(0x41, 0070, TEXT_OUT,  I_C,    M_DR8),
    E2(0x42, 0060, TEXT_SBC,  R_HL,   M_R16),
    E2(0x4A, 0060, TEXT_ADC,  R_HL,   M_R16),
    D2(0x43, 0020, TEXT_LD,   M_DABS, R_PTRH),
    D2(0x73, 0000, TEXT_LD,   M_DABS, R_SP),
    D2(0x4B, 0020, TEXT_LD,   R_PTRH, M_DABS),
    D2(0x7B, 0000, TEXT_LD,   R_SP,   M_DABS),
    E0(0x44, 0000, TEXT_NEG),
    E0(0x45, 0000, TEXT_RETN),
    E0(0x4D, 0000, TEXT_RETI),
    E1(0x46, 0030, TEXT_IM,   M_IMMD),
    E2(0x47, 0000, TEXT_LD,   R_I,    R_A),
    E2(0x4F, 0000, TEXT_LD,   R_R,    R_A),
    E2(0x57, 0000, TEXT_LD,   R_A,    R_I),
    E2(0x5F, 0000, TEXT_LD,   R_A,    R_R),
    E0(0x67, 0000, TEXT_RRD),
    E0(0x6F, 0000, TEXT_RLD),
    E0(0xA0, 0000, TEXT_LDI),
    E0(0xA8, 0000, TEXT_LDD),
    E0(0xB0, 0000, TEXT_LDIR),
    E0(0xB8, 0000, TEXT_LDDR),
    E0(0xA1, 0000, TEXT_CPI),
    E0(0xA9, 0000, TEXT_CPD),
    E0(0xB1, 0000, TEXT_CPIR),
    E0(0xB9, 0000, TEXT_CPDR),
    E0(0xA2, 0000, TEXT_INI),
    E0(0xAA, 0000, TEXT_IND),
    E0(0xB2, 0000, TEXT_INIR),
    E0(0xBA, 0000, TEXT_INDR),
    E0(0xA3, 0000, TEXT_OUTI),
    E0(0xAB, 0000, TEXT_OUTD),
    E0(0xB3, 0000, TEXT_OTIR),
    E0(0xBB, 0000, TEXT_OTDR),
};

constexpr uint8_t INDEX_EXT[] PROGMEM = {
      3,  // TEXT_ADC
     23,  // TEXT_CPD
     25,  // TEXT_CPDR
     22,  // TEXT_CPI
     24,  // TEXT_CPIR
     11,  // TEXT_IM
      0,  // TEXT_IN
     27,  // TEXT_IND
     29,  // TEXT_INDR
     26,  // TEXT_INI
     28,  // TEXT_INIR
      4,  // TEXT_LD
      5,  // TEXT_LD
      6,  // TEXT_LD
      7,  // TEXT_LD
     12,  // TEXT_LD
     13,  // TEXT_LD
     14,  // TEXT_LD
     15,  // TEXT_LD
     19,  // TEXT_LDD
     21,  // TEXT_LDDR
     18,  // TEXT_LDI
     20,  // TEXT_LDIR
      8,  // TEXT_NEG
     33,  // TEXT_OTDR
     32,  // TEXT_OTIR
      1,  // TEXT_OUT
     31,  // TEXT_OUTD
     30,  // TEXT_OUTI
     10,  // TEXT_RETI
      9,  // TEXT_RETN
     17,  // TEXT_RLD
     16,  // TEXT_RRD
      2,  // TEXT_SBC
};

// Table for DD:xx, FD:xx
constexpr Entry TABLE_IDX[] PROGMEM = {
    E2(0x09, 0060, TEXT_ADD,  R_IDX,  M_R16X),
    D2(0x21, 0000, TEXT_LD,   R_IDX,  M_LM16),
    D2(0x22, 0000, TEXT_LD,   M_DABS, R_IDX),
    D2(0x2A, 0000, TEXT_LD,   R_IDX,  M_DABS),
    E1(0x23, 0000, TEXT_INC,  R_IDX),
    E1(0x2B, 0000, TEXT_DEC,  R_IDX),
    I1(0x34, 0000, TEXT_INC,  M_IDX8),
    I1(0x35, 0000, TEXT_DEC,  M_IDX8),
    I2(0x36, 0000, TEXT_LD,   M_IDX8, M_IM8),
    I2(0x46, 0070, TEXT_LD,   M_DR8,  M_IDX8),
    I2(0x70, 0007, TEXT_LD,   M_IDX8, M_SR8),
    I2(0x86, 0000, TEXT_ADD,  R_A,    M_IDX8),
    I2(0x8E, 0000, TEXT_ADC,  R_A,    M_IDX8),
    I1(0x96, 0000, TEXT_SUB,  M_IDX8),
    I2(0x96, 0000, TEXT_SUB,  R_A,    M_IDX8),
    I2(0x9E, 0000, TEXT_SBC,  R_A,    M_IDX8),
    I1(0xA6, 0000, TEXT_AND,  M_IDX8),
    I2(0xA6, 0000, TEXT_AND,  R_A,    M_IDX8),
    I1(0xAE, 0000, TEXT_XOR,  M_IDX8),
    I2(0xAE, 0000, TEXT_XOR,  R_A,    M_IDX8),
    I1(0xB6, 0000, TEXT_OR,   M_IDX8),
    I2(0xB6, 0000, TEXT_OR,   R_A,    M_IDX8),
    I1(0xBE, 0000, TEXT_CP,   M_IDX8),
    I2(0xBE, 0000, TEXT_CP,   R_A,    M_IDX8),
    L1(0xE1, 0000, TEXT_POP,  R_IDX),
    E1(0xE9, 0000, TEXT_JP,   I_IDX),
    L2(0xF9, 0000, TEXT_LD,   R_SP,   R_IDX),
    L2(0xE3, 0000, TEXT_EX,   I_SP,   R_IDX),
    L2(0xE3, 0000, TEXT_EX,   R_IDX,  I_SP),
    L1(0xE5, 0000, TEXT_PUSH, R_IDX),
};

constexpr uint8_t INDEX_IDX[] PROGMEM = {
     12,  // TEXT_ADC
      0,  // TEXT_ADD
     11,  // TEXT_ADD
     16,  // TEXT_AND
     17,  // TEXT_AND
     22,  // TEXT_CP
     23,  // TEXT_CP
      5,  // TEXT_DEC
      7,  // TEXT_DEC
     27,  // TEXT_EX
     28,  // TEXT_EX
      4,  // TEXT_INC
      6,  // TEXT_INC
     25,  // TEXT_JP
      1,  // TEXT_LD
      2,  // TEXT_LD
      3,  // TEXT_LD
      8,  // TEXT_LD
      9,  // TEXT_LD
     10,  // TEXT_LD
     26,  // TEXT_LD
     20,  // TEXT_OR
     21,  // TEXT_OR
     24,  // TEXT_POP
     29,  // TEXT_PUSH
     15,  // TEXT_SBC
     13,  // TEXT_SUB
     14,  // TEXT_SUB
     18,  // TEXT_XOR
     19,  // TEXT_XOR
};

// DD:CB:dd:xx, FD:CB:dd:xx
constexpr Entry TABLE_IDXBIT[] PROGMEM = {
    I1(0x06, 0000, TEXT_RLC, M_IDX),
    I1(0x0E, 0000, TEXT_RRC, M_IDX),
    I1(0x16, 0000, TEXT_RL,  M_IDX),
    I1(0x1E, 0000, TEXT_RR,  M_IDX),
    I1(0x26, 0000, TEXT_SLA, M_IDX),
    I1(0x2E, 0000, TEXT_SRA, M_IDX),
    I1(0x3E, 0000, TEXT_SRL, M_IDX),
    I2(0x46, 0070, TEXT_BIT, M_BIT, M_IDX),
    I2(0x86, 0070, TEXT_RES, M_BIT, M_IDX),
    I2(0xC6, 0070, TEXT_SET, M_BIT, M_IDX),
};

constexpr uint8_t INDEX_IDXBIT[] PROGMEM = {
      7,  // TEXT_BIT
      8,  // TEXT_RES
      2,  // TEXT_RL
      0,  // TEXT_RLC
      3,  // TEXT_RR
      1,  // TEXT_RRC
      9,  // TEXT_SET
      4,  // TEXT_SLA
      5,  // TEXT_SRA
      6,  // TEXT_SRL
};

// Z180 ED:xx
constexpr Entry TABLE_Z180_EXT[] PROGMEM = {
    E1(0x4C, 0060, TEXT_MLT,    M_R16),
    E1(0x04, 0070, TEXT_TST,    M_DST),
    E1(0x64, 0000, TEXT_TST,    M_IM8),
    E1(0x74, 0000, TEXT_TSTIO,  M_IM8),
    E2(0x00, 0070, TEXT_IN0,    M_DR8, M_IOA),
    E2(0x01, 0070, TEXT_OUT0,   M_IOA, M_DR8),
    E0(0x83, 0000, TEXT_OTIM),
    E0(0x93, 0000, TEXT_OTIMR),
    E0(0x8B, 0000, TEXT_OTDM),
    E0(0x9B, 0000, TEXT_OTDMR),
    E0(0x76, 0000, TEXT_SLP),
};

constexpr uint8_t INDEX_Z180_EXT[] PROGMEM = {
      4,  // TEXT_IN0
      0,  // TEXT_MLT
      8,  // TEXT_OTDM
      9,  // TEXT_OTDMR
      6,  // TEXT_OTIM
      7,  // TEXT_OTIMR
      5,  // TEXT_OUT0
     10,  // TEXT_SLP
      1,  // TEXT_TST
      2,  // TEXT_TST
      3,  // TEXT_TSTIO
};
// clang-format on

using EntryPage = entry::PrefixTableBase<Entry>;

constexpr EntryPage Z80_PAGES[] PROGMEM = {
        {0x0000, ARRAY_RANGE(TABLE_Z80), ARRAY_RANGE(INDEX_Z80)},
        {TableZ80::EXT, ARRAY_RANGE(TABLE_EXT), ARRAY_RANGE(INDEX_EXT)},
        {TableZ80::BIT, ARRAY_RANGE(TABLE_BIT), ARRAY_RANGE(INDEX_BIT)},
        {TableZ80::IX, ARRAY_RANGE(TABLE_IDX), ARRAY_RANGE(INDEX_IDX)},
        {TableZ80::IY, ARRAY_RANGE(TABLE_IDX), ARRAY_RANGE(INDEX_IDX)},
        {TableZ80::IXBIT, ARRAY_RANGE(TABLE_IDXBIT), ARRAY_RANGE(INDEX_IDXBIT)},
        {TableZ80::IYBIT, ARRAY_RANGE(TABLE_IDXBIT), ARRAY_RANGE(INDEX_IDXBIT)},
};

constexpr EntryPage Z180_PAGES[] PROGMEM = {
        {0x0000, ARRAY_RANGE(TABLE_Z80), ARRAY_RANGE(INDEX_Z80)},
        {TableZ80::EXT, ARRAY_RANGE(TABLE_EXT), ARRAY_RANGE(INDEX_EXT)},
        {TableZ80::EXT, ARRAY_RANGE(TABLE_Z180_EXT), ARRAY_RANGE(INDEX_Z180_EXT)},
        {TableZ80::BIT, ARRAY_RANGE(TABLE_BIT), ARRAY_RANGE(INDEX_BIT)},
        {TableZ80::IX, ARRAY_RANGE(TABLE_IDX), ARRAY_RANGE(INDEX_IDX)},
        {TableZ80::IY, ARRAY_RANGE(TABLE_IDX), ARRAY_RANGE(INDEX_IDX)},
        {TableZ80::IXBIT, ARRAY_RANGE(TABLE_IDXBIT), ARRAY_RANGE(INDEX_IDXBIT)},
        {TableZ80::IYBIT, ARRAY_RANGE(TABLE_IDXBIT), ARRAY_RANGE(INDEX_IDXBIT)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {Z80, TEXT_CPU_Z80, ARRAY_RANGE(Z80_PAGES)},
        {Z180, TEXT_CPU_Z180, ARRAY_RANGE(Z180_PAGES)},
};

const Cpu *cpu(CpuType cpuType) {
    return Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
}

bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_SR8 || opr == R_A)
        return table == M_SRC || table == M_DST || table == M_SR8 || table == M_DR8 ||
               table == R_H || table == R_L;
    if (opr == I_HL)
        return table == M_SRC || table == M_DST;
    if (opr == R_C)
        return table == M_SRC || table == M_DST || table == M_SR8 || table == M_DR8 ||
               table == M_CC4 || table == M_CC8;
    if (opr == R_H || opr == R_L)
        return table == M_SRC || table == M_DST || table == M_SR8 || table == M_DR8;
    if (opr == R_BC || opr == R_DE)
        return table == M_R16 || table == M_R16X || table == R_PTRH;
    if (opr == R_HL)
        return table == M_R16 || table == M_R16X;
    if (opr == R_SP)
        return table == M_R16 || table == M_R16X;
    if (opr == R_IX || opr == R_IY)
        return table == R_IDX || table == M_R16X;
    if (opr == M_IDX8)
        return table == M_IDX;
    if (opr == M_IM16)
        return table == M_IM8 || table == M_REL8 || table == M_BIT || table == M_VEC ||
               table == M_IMMD || table == M_LM16 || table == M_JABS;
    if (opr == M_ABS)
        return table == M_IOA || table == M_DABS;
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
    if (dst == M_DR8 || src == M_DR8) {
        const auto reg = (opc >> 3) & 7;
        if (reg == 6)
            return false;
    }
    if (src == M_SR8 || dst == M_SR8) {
        const auto reg = opc & 7;
        if (reg == 6)
            return false;
    }
    opc &= ~flags.mask();
    return opc == entry->readOpCode();
}

Error searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) {
    cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    return insn.getError();
}

bool isPrefix(CpuType cpuType, uint16_t code) {
    return cpu(cpuType)->isPrefix(code);
}

bool EntryInsn::ixBit() const {
    return (prefix() == TableZ80::IXBIT || prefix() == TableZ80::IYBIT);
}

const /*PROGMEM*/ char *TableZ80::listCpu_P() const {
    return TEXT_Z80_LIST;
}

const /*PROGMEM*/ char *TableZ80::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableZ80::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    auto t = Cpu::search(name, ARRAY_RANGE(CPU_TABLE));
    if (t) {
        cpuType = t->readCpuType();
        return OK;
    }
    return UNSUPPORTED_CPU;
}

const TableZ80 TABLE;

}  // namespace z80
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

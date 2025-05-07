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
    L1(0xC1, 0060, TEXT_POP,  M_STK),
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
    L1(0xC5, 0060, TEXT_PUSH, M_STK),
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
     62,  // TEXT_ADC
      2,  // TEXT_ADD
     25,  // TEXT_ADD
     61,  // TEXT_ADD
     30,  // TEXT_AND
     31,  // TEXT_AND
     39,  // TEXT_AND
     65,  // TEXT_AND
     58,  // TEXT_CALL
     60,  // TEXT_CALL
     21,  // TEXT_CCF
     36,  // TEXT_CP
     37,  // TEXT_CP
     42,  // TEXT_CP
     68,  // TEXT_CP
     19,  // TEXT_CPL
     18,  // TEXT_DAA
     10,  // TEXT_DEC
     12,  // TEXT_DEC
     56,  // TEXT_DI
     71,  // TEXT_DJNZ
     57,  // TEXT_EI
     52,  // TEXT_EX
     53,  // TEXT_EX
     54,  // TEXT_EX
     55,  // TEXT_EX
     70,  // TEXT_EX
     74,  // TEXT_EXX
     22,  // TEXT_HALT
     51,  // TEXT_IN
      9,  // TEXT_INC
     11,  // TEXT_INC
     47,  // TEXT_JP
     48,  // TEXT_JP
     49,  // TEXT_JP
     72,  // TEXT_JR
     73,  // TEXT_JR
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
     46,  // TEXT_LD
      0,  // TEXT_NOP
     34,  // TEXT_OR
     35,  // TEXT_OR
     41,  // TEXT_OR
     67,  // TEXT_OR
     50,  // TEXT_OUT
     44,  // TEXT_POP
     59,  // TEXT_PUSH
     43,  // TEXT_RET
     45,  // TEXT_RET
     16,  // TEXT_RLA
     14,  // TEXT_RLCA
     17,  // TEXT_RRA
     15,  // TEXT_RRCA
     69,  // TEXT_RST
     27,  // TEXT_SBC
     64,  // TEXT_SBC
     20,  // TEXT_SCF
     28,  // TEXT_SUB
     29,  // TEXT_SUB
     38,  // TEXT_SUB
     63,  // TEXT_SUB
     32,  // TEXT_XOR
     33,  // TEXT_XOR
     40,  // TEXT_XOR
     66,  // TEXT_XOR
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
    D2(0x43, 0060, TEXT_LD,   M_DABS, M_NOHL),
    D2(0x4B, 0060, TEXT_LD,   M_NOHL, M_DABS),
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
     21,  // TEXT_CPD
     23,  // TEXT_CPDR
     20,  // TEXT_CPI
     22,  // TEXT_CPIR
      9,  // TEXT_IM
      0,  // TEXT_IN
     25,  // TEXT_IND
     27,  // TEXT_INDR
     24,  // TEXT_INI
     26,  // TEXT_INIR
      4,  // TEXT_LD
      5,  // TEXT_LD
     10,  // TEXT_LD
     11,  // TEXT_LD
     12,  // TEXT_LD
     13,  // TEXT_LD
     17,  // TEXT_LDD
     19,  // TEXT_LDDR
     16,  // TEXT_LDI
     18,  // TEXT_LDIR
      6,  // TEXT_NEG
     31,  // TEXT_OTDR
     30,  // TEXT_OTIR
      1,  // TEXT_OUT
     29,  // TEXT_OUTD
     28,  // TEXT_OUTI
      8,  // TEXT_RETI
      7,  // TEXT_RETN
     15,  // TEXT_RLD
     14,  // TEXT_RRD
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
    E2(0x43, 0060, TEXT_LD ,    M_ABS,  M_NOHL),
    E2(0x43, 0060, TEXT_LDW,    M_ABS,  M_NOHL),
    E1(0x44, 0000, TEXT_NEG,    R_A),
    E2(0x4B, 0060, TEXT_LD,     M_NOHL, M_ABS),
    E2(0x4B, 0060, TEXT_LDW,    M_NOHL, M_ABS),
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
     51,  // TEXT_ADD
     27,  // TEXT_ADDW
     40,  // TEXT_ADDW
     28,  // TEXT_CPW
     41,  // TEXT_CPW
     55,  // TEXT_DI
     25,  // TEXT_DIV
     38,  // TEXT_DIV
     26,  // TEXT_DIVU
     39,  // TEXT_DIVU
     30,  // TEXT_DIVUW
     43,  // TEXT_DIVUW
     29,  // TEXT_DIVW
     42,  // TEXT_DIVW
     56,  // TEXT_EI
     75,  // TEXT_EPUF
     76,  // TEXT_EPUI
     77,  // TEXT_EPUM
     79,  // TEXT_EPUM
     80,  // TEXT_EPUM
      7,  // TEXT_EX
      8,  // TEXT_EX
     32,  // TEXT_EX
     33,  // TEXT_EX
     46,  // TEXT_EXTS
     47,  // TEXT_EXTS
     50,  // TEXT_EXTS
     72,  // TEXT_IN
     68,  // TEXT_INDRW
     61,  // TEXT_INDW
     65,  // TEXT_INIRW
     57,  // TEXT_INIW
     71,  // TEXT_INW
      1,  // TEXT_LD
      3,  // TEXT_LD
      5,  // TEXT_LD
     10,  // TEXT_LD
     12,  // TEXT_LD
     13,  // TEXT_LD
     16,  // TEXT_LD
      0,  // TEXT_LDA
     49,  // TEXT_LDCTL
     52,  // TEXT_LDCTL
     60,  // TEXT_LDCTL
     64,  // TEXT_LDCTL
     59,  // TEXT_LDUD
     63,  // TEXT_LDUD
     67,  // TEXT_LDUP
     70,  // TEXT_LDUP
      2,  // TEXT_LDW
      4,  // TEXT_LDW
      6,  // TEXT_LDW
      9,  // TEXT_LDW
     11,  // TEXT_LDW
     14,  // TEXT_LDW
     17,  // TEXT_LDW
     19,  // TEXT_LDW
     20,  // TEXT_LDW
     78,  // TEXT_MEPU
     81,  // TEXT_MEPU
     82,  // TEXT_MEPU
     21,  // TEXT_MULT
     34,  // TEXT_MULT
     22,  // TEXT_MULTU
     35,  // TEXT_MULTU
     24,  // TEXT_MULTUW
     37,  // TEXT_MULTUW
     23,  // TEXT_MULTW
     36,  // TEXT_MULTW
     15,  // TEXT_NEG
     18,  // TEXT_NEG
     69,  // TEXT_OTDRW
     66,  // TEXT_OTIRW
     74,  // TEXT_OUT
     62,  // TEXT_OUTDW
     58,  // TEXT_OUTIW
     73,  // TEXT_OUTW
     48,  // TEXT_PCACHE
     45,  // TEXT_RETIL
     54,  // TEXT_SC
     31,  // TEXT_SUBW
     44,  // TEXT_SUBW
     53,  // TEXT_TSTI
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

constexpr EntryPage Z280_PAGES[] PROGMEM = {
        {0x0000, ARRAY_RANGE(TABLE_Z80), ARRAY_RANGE(INDEX_Z80)},
        {0x0000, ARRAY_RANGE(TABLE_Z280), ARRAY_RANGE(INDEX_Z280)},
        {TableZ80::IX, ARRAY_RANGE(TABLE_IDX), ARRAY_RANGE(INDEX_IDX)},
        {TableZ80::IY, ARRAY_RANGE(TABLE_IDX), ARRAY_RANGE(INDEX_IDX)},
        {TableZ80::IX, ARRAY_RANGE(TABLE_Z280_IDX), ARRAY_RANGE(INDEX_Z280_IDX)},
        {TableZ80::IY, ARRAY_RANGE(TABLE_Z280_IDX), ARRAY_RANGE(INDEX_Z280_IDX)},
        {TableZ80::IX, ARRAY_RANGE(TABLE_Z280_IX), ARRAY_RANGE(INDEX_Z280_IX)},
        {TableZ80::IY, ARRAY_RANGE(TABLE_Z280_IY), ARRAY_RANGE(INDEX_Z280_IY)},
        {TableZ80::EXT, ARRAY_RANGE(TABLE_EXT), ARRAY_RANGE(INDEX_EXT)},
        {TableZ80::EXT, ARRAY_RANGE(TABLE_Z280_EXT), ARRAY_RANGE(INDEX_Z280_EXT)},
        {TableZ80::IXEXT, ARRAY_RANGE(TABLE_Z280_IDXEXT), ARRAY_RANGE(INDEX_Z280_IDXEXT)},
        {TableZ80::IYEXT, ARRAY_RANGE(TABLE_Z280_IDXEXT), ARRAY_RANGE(INDEX_Z280_IDXEXT)},
        {TableZ80::IXEXT, ARRAY_RANGE(TABLE_Z280_IXEXT), ARRAY_RANGE(INDEX_Z280_IXEXT)},
        {TableZ80::IYEXT, ARRAY_RANGE(TABLE_Z280_IYEXT), ARRAY_RANGE(INDEX_Z280_IYEXT)},
        {TableZ80::BIT, ARRAY_RANGE(TABLE_BIT), ARRAY_RANGE(INDEX_BIT)},
        {TableZ80::BIT, ARRAY_RANGE(TABLE_Z280_BIT), ARRAY_RANGE(INDEX_Z280_BIT)},
        {TableZ80::IXBIT, ARRAY_RANGE(TABLE_IDXBIT), ARRAY_RANGE(INDEX_IDXBIT)},
        {TableZ80::IYBIT, ARRAY_RANGE(TABLE_IDXBIT), ARRAY_RANGE(INDEX_IDXBIT)},
        {TableZ80::IXBIT, ARRAY_RANGE(TABLE_Z280_IDXBIT), ARRAY_RANGE(INDEX_Z280_IDXBIT)},
        {TableZ80::IYBIT, ARRAY_RANGE(TABLE_Z280_IDXBIT), ARRAY_RANGE(INDEX_Z280_IDXBIT)},
};

constexpr EntryPage Z380_PAGES[] PROGMEM = {
        {0x0000, ARRAY_RANGE(TABLE_Z80), ARRAY_RANGE(INDEX_Z80)},
        {0x0000, ARRAY_RANGE(TABLE_Z380), ARRAY_RANGE(INDEX_Z380)},
        {TableZ80::EXT, ARRAY_RANGE(TABLE_EXT), ARRAY_RANGE(INDEX_EXT)},
        {TableZ80::EXT, ARRAY_RANGE(TABLE_Z180_EXT), ARRAY_RANGE(INDEX_Z180_EXT)},
        {TableZ80::EXT, ARRAY_RANGE(TABLE_Z380_EXT), ARRAY_RANGE(INDEX_Z380_EXT)},
        {TableZ80::EXTBIT, ARRAY_RANGE(TABLE_Z380_EXTBIT), ARRAY_RANGE(INDEX_Z380_EXTBIT)},
        {TableZ80::BIT, ARRAY_RANGE(TABLE_BIT), ARRAY_RANGE(INDEX_BIT)},
        {TableZ80::BIT, ARRAY_RANGE(TABLE_Z380_BIT), ARRAY_RANGE(INDEX_Z380_BIT)},
        {TableZ80::IX, ARRAY_RANGE(TABLE_IDX), ARRAY_RANGE(INDEX_IDX)},
        {TableZ80::IY, ARRAY_RANGE(TABLE_IDX), ARRAY_RANGE(INDEX_IDX)},
        {TableZ80::IX, ARRAY_RANGE(TABLE_Z380_IDX), ARRAY_RANGE(INDEX_Z380_IDX)},
        {TableZ80::IY, ARRAY_RANGE(TABLE_Z380_IDX), ARRAY_RANGE(INDEX_Z380_IDX)},
        {TableZ80::IX, ARRAY_RANGE(TABLE_Z380_IX), ARRAY_RANGE(INDEX_Z380_IX)},
        {TableZ80::IY, ARRAY_RANGE(TABLE_Z380_IY), ARRAY_RANGE(INDEX_Z380_IY)},
        {TableZ80::IXBIT, ARRAY_RANGE(TABLE_IDXBIT), ARRAY_RANGE(INDEX_IDXBIT)},
        {TableZ80::IYBIT, ARRAY_RANGE(TABLE_IDXBIT), ARRAY_RANGE(INDEX_IDXBIT)},
        {TableZ80::IXBIT, ARRAY_RANGE(TABLE_Z380_IXBIT), ARRAY_RANGE(INDEX_Z380_IXBIT)},
        {TableZ80::IYBIT, ARRAY_RANGE(TABLE_Z380_IYBIT), ARRAY_RANGE(INDEX_Z380_IYBIT)},
        {TableZ80::IXBIT, ARRAY_RANGE(TABLE_Z380_IDXBIT), ARRAY_RANGE(INDEX_Z380_IDXBIT)},
        {TableZ80::IYBIT, ARRAY_RANGE(TABLE_Z380_IDXBIT), ARRAY_RANGE(INDEX_Z380_IDXBIT)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {Z80, TEXT_CPU_Z80, ARRAY_RANGE(Z80_PAGES)},
        {Z180, TEXT_CPU_Z180, ARRAY_RANGE(Z180_PAGES)},
        {Z280, TEXT_CPU_Z280, ARRAY_RANGE(Z280_PAGES)},
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
        return table == M_SRC || table == M_DST || table == M_MPTR;
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
        return table == M_R16 || table == M_R16X || table == M_NOHL || table == M_STK ||
               table == R_PTRL || table == R_PTRH;
    if (opr == R_HL)
        return table == M_R16 || table == M_R16X || table == M_STK;
    if (opr == R_SP)
        return table == M_R16 || table == M_R16X || table == M_NOHL;
    if (opr == R_AF)
        return table == M_STK;
    if (opr == R_IX || opr == R_IY)
        return table == R_IDX || table == R_IDXL || table == M_R16X;
    if (opr == M_REL16 || opr == M_PCDX)
        return table == M_FIDX || table == M_PIXH || table == M_PIXL;
    if (opr == M_IDX8)
        return table == M_FIDX || table == M_PIXH || table == M_PIXL || table == M_IDX;
    if (opr == M_IDX16)
        return table == M_FIDX || table == M_PIXH || table == M_PIXL || table == M_IDX;
    if (opr == M_PDX)
        return table == M_FIDX || table == M_PIXH || table == M_PIXL;
    if (opr == M_SPDX)
        return table == M_FIDX || table == M_BIXH || table == M_BIXL || table == M_SPX;
    if (opr == M_BDX)
        return table == M_FIDX || table == M_BIXH || table == M_BIXL;
    if (opr == M_IM16)
        return table == M_IM8 || table == M_REL8 || table == M_BIT || table == M_VEC ||
               table == M_IMMD || table == M_EPU || table == M_DM16 || table == M_XM16 ||
               table == M_LM16 || table == M_JABS;
    if (opr == M_ABS)
        return table == M_IOA || table == M_IO16 || table == M_MPTR || table == M_DABS ||
               table == M_XABS;
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
    if (dst == M_NOHL || src == M_NOHL) {
        const auto mode = (opc >> 4) & 3;
        if (mode == 2)
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
    return TEXT_CPU_LIST;
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

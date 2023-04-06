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

#include "config_mos6502.h"

#include "entry_mos6502.h"
#include "table_mos6502.h"
#include "text_mos6502.h"

#include <string.h>

using namespace libasm::text::mos6502;

namespace libasm {
namespace mos6502 {

#define E3(_opc, _name, _opr1, _opr2, _opr3) \
    { _opc, Entry::Flags::create(_opr1, _opr2, _opr3), _name }
#define E2(_opc, _name, _opr1, _opr2) E3(_opc, _name, _opr1, _opr2, M_NONE)
#define E1(_opc, _name, _opr1) E2(_opc, _name, _opr1, M_NONE)
#define E0(_opc, _name) E1(_opc, _name, M_NONE)

// clang-format off
static constexpr Entry MOS6502_TABLE[] PROGMEM = {
    E0(0x00, TEXT_BRK),
    E0(0x40, TEXT_RTI),
    E0(0x60, TEXT_RTS),
    E0(0xEA, TEXT_NOP),
    E0(0x08, TEXT_PHP),
    E0(0x28, TEXT_PLP),
    E0(0x48, TEXT_PHA),
    E0(0x68, TEXT_PLA),
    E0(0x88, TEXT_DEY),
    E0(0xC8, TEXT_INY),
    E0(0xCA, TEXT_DEX),
    E0(0xE8, TEXT_INX),
    E0(0x98, TEXT_TYA),
    E0(0xA8, TEXT_TAY),
    E0(0x8A, TEXT_TXA),
    E0(0xAA, TEXT_TAX),
    E0(0x9A, TEXT_TXS),
    E0(0xBA, TEXT_TSX),
    E0(0x18, TEXT_CLC),
    E0(0x38, TEXT_SEC),
    E0(0x58, TEXT_CLI),
    E0(0x78, TEXT_SEI),
    E0(0xB8, TEXT_CLV),
    E0(0xD8, TEXT_CLD),
    E0(0xF8, TEXT_SED),
    E1(0x0A, TEXT_ASL, M_REGA),
    E1(0x2A, TEXT_ROL, M_REGA),
    E1(0x4A, TEXT_LSR, M_REGA),
    E1(0x6A, TEXT_ROR, M_REGA),
    E1(0xA0, TEXT_LDY, M_IMX),
    E1(0xA2, TEXT_LDX, M_IMX),
    E1(0xC0, TEXT_CPY, M_IMX),
    E1(0xE0, TEXT_CPX, M_IMX),
    E1(0x09, TEXT_ORA, M_IMA),
    E1(0x29, TEXT_AND, M_IMA),
    E1(0x49, TEXT_EOR, M_IMA),
    E1(0x69, TEXT_ADC, M_IMA),
    E1(0xA9, TEXT_LDA, M_IMA),
    E1(0xC9, TEXT_CMP, M_IMA),
    E1(0xE9, TEXT_SBC, M_IMA),
    E1(0x24, TEXT_BIT, M_DPG),
    E1(0x05, TEXT_ORA, M_DPG),
    E1(0x25, TEXT_AND, M_DPG),
    E1(0x45, TEXT_EOR, M_DPG),
    E1(0x65, TEXT_ADC, M_DPG),
    E1(0x85, TEXT_STA, M_DPG),
    E1(0xA5, TEXT_LDA, M_DPG),
    E1(0xC5, TEXT_CMP, M_DPG),
    E1(0xE5, TEXT_SBC, M_DPG),
    E1(0x84, TEXT_STY, M_DPG),
    E1(0xA4, TEXT_LDY, M_DPG),
    E1(0xC4, TEXT_CPY, M_DPG),
    E1(0x86, TEXT_STX, M_DPG),
    E1(0xA6, TEXT_LDX, M_DPG),
    E1(0xE4, TEXT_CPX, M_DPG),
    E1(0xC6, TEXT_DEC, M_DPG),
    E1(0xE6, TEXT_INC, M_DPG),
    E1(0x06, TEXT_ASL, M_DPG),
    E1(0x26, TEXT_ROL, M_DPG),
    E1(0x46, TEXT_LSR, M_DPG),
    E1(0x66, TEXT_ROR, M_DPG),
    E2(0x15, TEXT_ORA, M_DPG, M_REGX),
    E2(0x35, TEXT_AND, M_DPG, M_REGX),
    E2(0x55, TEXT_EOR, M_DPG, M_REGX),
    E2(0x75, TEXT_ADC, M_DPG, M_REGX),
    E2(0x95, TEXT_STA, M_DPG, M_REGX),
    E2(0xB5, TEXT_LDA, M_DPG, M_REGX),
    E2(0xD5, TEXT_CMP, M_DPG, M_REGX),
    E2(0xF5, TEXT_SBC, M_DPG, M_REGX),
    E2(0x94, TEXT_STY, M_DPG, M_REGX),
    E2(0xB4, TEXT_LDY, M_DPG, M_REGX),
    E2(0x96, TEXT_STX, M_DPG, M_REGY),
    E2(0xB6, TEXT_LDX, M_DPG, M_REGY),
    E2(0xD6, TEXT_DEC, M_DPG, M_REGX),
    E2(0xF6, TEXT_INC, M_DPG, M_REGX),
    E2(0x16, TEXT_ASL, M_DPG, M_REGX),
    E2(0x36, TEXT_ROL, M_DPG, M_REGX),
    E2(0x56, TEXT_LSR, M_DPG, M_REGX),
    E2(0x76, TEXT_ROR, M_DPG, M_REGX),
    E1(0x2C, TEXT_BIT, M_ABS),
    E1(0x0D, TEXT_ORA, M_ABS),
    E1(0x2D, TEXT_AND, M_ABS),
    E1(0x4D, TEXT_EOR, M_ABS),
    E1(0x6D, TEXT_ADC, M_ABS),
    E1(0x8D, TEXT_STA, M_ABS),
    E1(0xAD, TEXT_LDA, M_ABS),
    E1(0xCD, TEXT_CMP, M_ABS),
    E1(0xED, TEXT_SBC, M_ABS),
    E1(0x8C, TEXT_STY, M_ABS),
    E1(0xAC, TEXT_LDY, M_ABS),
    E1(0xCC, TEXT_CPY, M_ABS),
    E1(0x8E, TEXT_STX, M_ABS),
    E1(0xAE, TEXT_LDX, M_ABS),
    E1(0xEC, TEXT_CPX, M_ABS),
    E1(0xCE, TEXT_DEC, M_ABS),
    E1(0xEE, TEXT_INC, M_ABS),
    E1(0x0E, TEXT_ASL, M_ABS),
    E1(0x2E, TEXT_ROL, M_ABS),
    E1(0x4E, TEXT_LSR, M_ABS),
    E1(0x6E, TEXT_ROR, M_ABS),
    E1(0x4C, TEXT_JMP, M_ABS),
    E1(0x20, TEXT_JSR, M_ABS),
    E2(0x1D, TEXT_ORA, M_ABS, M_REGX),
    E2(0x3D, TEXT_AND, M_ABS, M_REGX),
    E2(0x5D, TEXT_EOR, M_ABS, M_REGX),
    E2(0x7D, TEXT_ADC, M_ABS, M_REGX),
    E2(0x9D, TEXT_STA, M_ABS, M_REGX),
    E2(0xBD, TEXT_LDA, M_ABS, M_REGX),
    E2(0xDD, TEXT_CMP, M_ABS, M_REGX),
    E2(0xFD, TEXT_SBC, M_ABS, M_REGX),
    E2(0x19, TEXT_ORA, M_ABS, M_REGY),
    E2(0x39, TEXT_AND, M_ABS, M_REGY),
    E2(0x59, TEXT_EOR, M_ABS, M_REGY),
    E2(0x79, TEXT_ADC, M_ABS, M_REGY),
    E2(0x99, TEXT_STA, M_ABS, M_REGY),
    E2(0xB9, TEXT_LDA, M_ABS, M_REGY),
    E2(0xD9, TEXT_CMP, M_ABS, M_REGY),
    E2(0xF9, TEXT_SBC, M_ABS, M_REGY),
    E2(0xBC, TEXT_LDY, M_ABS, M_REGX),
    E2(0xBE, TEXT_LDX, M_ABS, M_REGY),
    E2(0xDE, TEXT_DEC, M_ABS, M_REGX),
    E2(0xFE, TEXT_INC, M_ABS, M_REGX),
    E2(0x1E, TEXT_ASL, M_ABS, M_REGX),
    E2(0x3E, TEXT_ROL, M_ABS, M_REGX),
    E2(0x5E, TEXT_LSR, M_ABS, M_REGX),
    E2(0x7E, TEXT_ROR, M_ABS, M_REGX),
    E1(0x6C, TEXT_JMP, I_ABS),
    E2(0x01, TEXT_ORA, I_DPG, I_REGX),
    E2(0x21, TEXT_AND, I_DPG, I_REGX),
    E2(0x41, TEXT_EOR, I_DPG, I_REGX),
    E2(0x61, TEXT_ADC, I_DPG, I_REGX),
    E2(0x81, TEXT_STA, I_DPG, I_REGX),
    E2(0xA1, TEXT_LDA, I_DPG, I_REGX),
    E2(0xC1, TEXT_CMP, I_DPG, I_REGX),
    E2(0xE1, TEXT_SBC, I_DPG, I_REGX),
    E2(0x11, TEXT_ORA, I_DPG, M_REGY),
    E2(0x31, TEXT_AND, I_DPG, M_REGY),
    E2(0x51, TEXT_EOR, I_DPG, M_REGY),
    E2(0x71, TEXT_ADC, I_DPG, M_REGY),
    E2(0x91, TEXT_STA, I_DPG, M_REGY),
    E2(0xB1, TEXT_LDA, I_DPG, M_REGY),
    E2(0xD1, TEXT_CMP, I_DPG, M_REGY),
    E2(0xF1, TEXT_SBC, I_DPG, M_REGY),
    E1(0x10, TEXT_BPL, M_REL),
    E1(0x30, TEXT_BMI, M_REL),
    E1(0x50, TEXT_BVC, M_REL),
    E1(0x70, TEXT_BVS, M_REL),
    E1(0x90, TEXT_BCC, M_REL),
    E1(0xB0, TEXT_BCS, M_REL),
    E1(0xD0, TEXT_BNE, M_REL),
    E1(0xF0, TEXT_BEQ, M_REL),
};

static constexpr uint8_t MOS6502_INDEX[] PROGMEM = {
     36,  // TEXT_ADC
     44,  // TEXT_ADC
     64,  // TEXT_ADC
     83,  // TEXT_ADC
    105,  // TEXT_ADC
    113,  // TEXT_ADC
    130,  // TEXT_ADC
    138,  // TEXT_ADC
     34,  // TEXT_AND
     42,  // TEXT_AND
     62,  // TEXT_AND
     81,  // TEXT_AND
    103,  // TEXT_AND
    111,  // TEXT_AND
    128,  // TEXT_AND
    136,  // TEXT_AND
     25,  // TEXT_ASL
     57,  // TEXT_ASL
     75,  // TEXT_ASL
     96,  // TEXT_ASL
    122,  // TEXT_ASL
    147,  // TEXT_BCC
    148,  // TEXT_BCS
    150,  // TEXT_BEQ
     40,  // TEXT_BIT
     79,  // TEXT_BIT
    144,  // TEXT_BMI
    149,  // TEXT_BNE
    143,  // TEXT_BPL
      0,  // TEXT_BRK
    145,  // TEXT_BVC
    146,  // TEXT_BVS
     18,  // TEXT_CLC
     23,  // TEXT_CLD
     20,  // TEXT_CLI
     22,  // TEXT_CLV
     38,  // TEXT_CMP
     47,  // TEXT_CMP
     67,  // TEXT_CMP
     86,  // TEXT_CMP
    108,  // TEXT_CMP
    116,  // TEXT_CMP
    133,  // TEXT_CMP
    141,  // TEXT_CMP
     32,  // TEXT_CPX
     54,  // TEXT_CPX
     93,  // TEXT_CPX
     31,  // TEXT_CPY
     51,  // TEXT_CPY
     90,  // TEXT_CPY
     55,  // TEXT_DEC
     73,  // TEXT_DEC
     94,  // TEXT_DEC
    120,  // TEXT_DEC
     10,  // TEXT_DEX
      8,  // TEXT_DEY
     35,  // TEXT_EOR
     43,  // TEXT_EOR
     63,  // TEXT_EOR
     82,  // TEXT_EOR
    104,  // TEXT_EOR
    112,  // TEXT_EOR
    129,  // TEXT_EOR
    137,  // TEXT_EOR
     56,  // TEXT_INC
     74,  // TEXT_INC
     95,  // TEXT_INC
    121,  // TEXT_INC
     11,  // TEXT_INX
      9,  // TEXT_INY
    100,  // TEXT_JMP
    126,  // TEXT_JMP
    101,  // TEXT_JSR
     37,  // TEXT_LDA
     46,  // TEXT_LDA
     66,  // TEXT_LDA
     85,  // TEXT_LDA
    107,  // TEXT_LDA
    115,  // TEXT_LDA
    132,  // TEXT_LDA
    140,  // TEXT_LDA
     30,  // TEXT_LDX
     53,  // TEXT_LDX
     72,  // TEXT_LDX
     92,  // TEXT_LDX
    119,  // TEXT_LDX
     29,  // TEXT_LDY
     50,  // TEXT_LDY
     70,  // TEXT_LDY
     89,  // TEXT_LDY
    118,  // TEXT_LDY
     27,  // TEXT_LSR
     59,  // TEXT_LSR
     77,  // TEXT_LSR
     98,  // TEXT_LSR
    124,  // TEXT_LSR
      3,  // TEXT_NOP
     33,  // TEXT_ORA
     41,  // TEXT_ORA
     61,  // TEXT_ORA
     80,  // TEXT_ORA
    102,  // TEXT_ORA
    110,  // TEXT_ORA
    127,  // TEXT_ORA
    135,  // TEXT_ORA
      6,  // TEXT_PHA
      4,  // TEXT_PHP
      7,  // TEXT_PLA
      5,  // TEXT_PLP
     26,  // TEXT_ROL
     58,  // TEXT_ROL
     76,  // TEXT_ROL
     97,  // TEXT_ROL
    123,  // TEXT_ROL
     28,  // TEXT_ROR
     60,  // TEXT_ROR
     78,  // TEXT_ROR
     99,  // TEXT_ROR
    125,  // TEXT_ROR
      1,  // TEXT_RTI
      2,  // TEXT_RTS
     39,  // TEXT_SBC
     48,  // TEXT_SBC
     68,  // TEXT_SBC
     87,  // TEXT_SBC
    109,  // TEXT_SBC
    117,  // TEXT_SBC
    134,  // TEXT_SBC
    142,  // TEXT_SBC
     19,  // TEXT_SEC
     24,  // TEXT_SED
     21,  // TEXT_SEI
     45,  // TEXT_STA
     65,  // TEXT_STA
     84,  // TEXT_STA
    106,  // TEXT_STA
    114,  // TEXT_STA
    131,  // TEXT_STA
    139,  // TEXT_STA
     52,  // TEXT_STX
     71,  // TEXT_STX
     91,  // TEXT_STX
     49,  // TEXT_STY
     69,  // TEXT_STY
     88,  // TEXT_STY
     15,  // TEXT_TAX
     13,  // TEXT_TAY
     17,  // TEXT_TSX
     14,  // TEXT_TXA
     16,  // TEXT_TXS
     12,  // TEXT_TYA
};

static constexpr Entry G65SC02_TABLE[] PROGMEM = {
    E0(0x5A, TEXT_PHY),
    E0(0x7A, TEXT_PLY),
    E0(0xDA, TEXT_PHX),
    E0(0xFA, TEXT_PLX),
    E1(0x1A, TEXT_INC, M_REGA),
    E1(0x3A, TEXT_DEC, M_REGA),
    E1(0x89, TEXT_BIT, M_IMA),
    E1(0x04, TEXT_TSB, M_DPG),
    E1(0x14, TEXT_TRB, M_DPG),
    E1(0x64, TEXT_STZ, M_DPG),
    E2(0x34, TEXT_BIT, M_DPG, M_REGX),
    E2(0x74, TEXT_STZ, M_DPG, M_REGX),
    E1(0x0C, TEXT_TSB, M_ABS),
    E1(0x1C, TEXT_TRB, M_ABS),
    E1(0x9C, TEXT_STZ, M_ABS),
    E2(0x3C, TEXT_BIT, M_ABS, M_REGX),
    E2(0x9E, TEXT_STZ, M_ABS, M_REGX),
    E2(0x7C, TEXT_JMP, I_ABS, I_REGX),
    E1(0x12, TEXT_ORA, I_DPG),
    E1(0x32, TEXT_AND, I_DPG),
    E1(0x52, TEXT_EOR, I_DPG),
    E1(0x72, TEXT_ADC, I_DPG),
    E1(0x92, TEXT_STA, I_DPG),
    E1(0xB2, TEXT_LDA, I_DPG),
    E1(0xD2, TEXT_CMP, I_DPG),
    E1(0xF2, TEXT_SBC, I_DPG),
    E1(0x80, TEXT_BRA, M_REL),
};

static constexpr uint8_t G65SC02_INDEX[] PROGMEM = {
     21,  // TEXT_ADC
     19,  // TEXT_AND
      6,  // TEXT_BIT
     10,  // TEXT_BIT
     15,  // TEXT_BIT
     26,  // TEXT_BRA
     24,  // TEXT_CMP
      5,  // TEXT_DEC
     20,  // TEXT_EOR
      4,  // TEXT_INC
     17,  // TEXT_JMP
     23,  // TEXT_LDA
     18,  // TEXT_ORA
      2,  // TEXT_PHX
      0,  // TEXT_PHY
      3,  // TEXT_PLX
      1,  // TEXT_PLY
     25,  // TEXT_SBC
     22,  // TEXT_STA
      9,  // TEXT_STZ
     11,  // TEXT_STZ
     14,  // TEXT_STZ
     16,  // TEXT_STZ
      8,  // TEXT_TRB
     13,  // TEXT_TRB
      7,  // TEXT_TSB
     12,  // TEXT_TSB
};

static constexpr Entry R65C02_TABLE[] PROGMEM = {
    E2(0x0F, TEXT_BBR0, M_DPG, M_REL),
    E2(0x1F, TEXT_BBR1, M_DPG, M_REL),
    E2(0x2F, TEXT_BBR2, M_DPG, M_REL),
    E2(0x3F, TEXT_BBR3, M_DPG, M_REL),
    E2(0x4F, TEXT_BBR4, M_DPG, M_REL),
    E2(0x5F, TEXT_BBR5, M_DPG, M_REL),
    E2(0x6F, TEXT_BBR6, M_DPG, M_REL),
    E2(0x7F, TEXT_BBR7, M_DPG, M_REL),
    E2(0x8F, TEXT_BBS0, M_DPG, M_REL),
    E2(0x9F, TEXT_BBS1, M_DPG, M_REL),
    E2(0xAF, TEXT_BBS2, M_DPG, M_REL),
    E2(0xBF, TEXT_BBS3, M_DPG, M_REL),
    E2(0xCF, TEXT_BBS4, M_DPG, M_REL),
    E2(0xDF, TEXT_BBS5, M_DPG, M_REL),
    E2(0xEF, TEXT_BBS6, M_DPG, M_REL),
    E2(0xFF, TEXT_BBS7, M_DPG, M_REL),
    E1(0x07, TEXT_RMB0, M_DPG),
    E1(0x17, TEXT_RMB1, M_DPG),
    E1(0x27, TEXT_RMB2, M_DPG),
    E1(0x37, TEXT_RMB3, M_DPG),
    E1(0x47, TEXT_RMB4, M_DPG),
    E1(0x57, TEXT_RMB5, M_DPG),
    E1(0x67, TEXT_RMB6, M_DPG),
    E1(0x77, TEXT_RMB7, M_DPG),
    E1(0x87, TEXT_SMB0, M_DPG),
    E1(0x97, TEXT_SMB1, M_DPG),
    E1(0xA7, TEXT_SMB2, M_DPG),
    E1(0xB7, TEXT_SMB3, M_DPG),
    E1(0xC7, TEXT_SMB4, M_DPG),
    E1(0xD7, TEXT_SMB5, M_DPG),
    E1(0xE7, TEXT_SMB6, M_DPG),
    E1(0xF7, TEXT_SMB7, M_DPG),
};

static constexpr uint8_t R65C02_INDEX[] PROGMEM = {
      0,  // TEXT_BBR0
      1,  // TEXT_BBR1
      2,  // TEXT_BBR2
      3,  // TEXT_BBR3
      4,  // TEXT_BBR4
      5,  // TEXT_BBR5
      6,  // TEXT_BBR6
      7,  // TEXT_BBR7
      8,  // TEXT_BBS0
      9,  // TEXT_BBS1
     10,  // TEXT_BBS2
     11,  // TEXT_BBS3
     12,  // TEXT_BBS4
     13,  // TEXT_BBS5
     14,  // TEXT_BBS6
     15,  // TEXT_BBS7
     16,  // TEXT_RMB0
     17,  // TEXT_RMB1
     18,  // TEXT_RMB2
     19,  // TEXT_RMB3
     20,  // TEXT_RMB4
     21,  // TEXT_RMB5
     22,  // TEXT_RMB6
     23,  // TEXT_RMB7
     24,  // TEXT_SMB0
     25,  // TEXT_SMB1
     26,  // TEXT_SMB2
     27,  // TEXT_SMB3
     28,  // TEXT_SMB4
     29,  // TEXT_SMB5
     30,  // TEXT_SMB6
     31,  // TEXT_SMB7
};

static constexpr Entry W65C02S_TABLE[] PROGMEM = {
    E0(0xCB, TEXT_WAI),
    E0(0xDB, TEXT_STP),
};

static constexpr uint8_t W65C02S_INDEX[] PROGMEM = {
      1,  // TEXT_STP
      0,  // TEXT_WAI
};

static constexpr Entry W65C816_TABLE[] PROGMEM = {
    E0(0x0B, TEXT_PHD),
    E0(0x1B, TEXT_TCS),
    E0(0x2B, TEXT_PLD),
    E0(0x3B, TEXT_TSC),
    E0(0x4B, TEXT_PHK),
    E0(0x5B, TEXT_TCD),
    E0(0x6B, TEXT_RTL),
    E0(0x7B, TEXT_TDC),
    E0(0x8B, TEXT_PHB),
    E0(0x9B, TEXT_TXY),
    E0(0xAB, TEXT_PLB),
    E0(0xBB, TEXT_TYX),
    E0(0xEB, TEXT_XBA),
    E0(0xFB, TEXT_XCE),
    E1(0x02, TEXT_COP, M_IM8),
    E1(0x42, TEXT_WDM, M_IM8),
    E1(0xC2, TEXT_REP, M_IM8),
    E1(0xE2, TEXT_SEP, M_IM8),
    E1(0x5C, TEXT_JMP, M_ABSL),
    E1(0xDC, TEXT_JMP, L_ABS),
    E2(0xFC, TEXT_JSR, I_ABS,  I_REGX),
    E1(0x22, TEXT_JSL, M_ABSL),
    E1(0x62, TEXT_PER, M_RELL),
    E1(0x82, TEXT_BRL, M_RELL),
    E1(0xD4, TEXT_PEI, I_DPG),
    E1(0xF4, TEXT_PEA, M_ABS),
    E2(0x44, TEXT_MVP, M_BANK, M_BANK),
    E2(0x54, TEXT_MVN, M_BANK, M_BANK),
    E2(0x03, TEXT_ORA, M_DPG,  M_REGS),
    E3(0x13, TEXT_ORA, I_DPG,  I_REGS, M_REGY),
    E1(0x07, TEXT_ORA, L_DPG),
    E2(0x17, TEXT_ORA, L_DPG,  M_REGY),
    E1(0x0F, TEXT_ORA, M_ABSL),
    E2(0x1F, TEXT_ORA, M_ABSL, M_REGX),
    E2(0x23, TEXT_AND, M_DPG,  M_REGS),
    E3(0x33, TEXT_AND, I_DPG,  I_REGS, M_REGY),
    E1(0x27, TEXT_AND, L_DPG),
    E2(0x37, TEXT_AND, L_DPG,  M_REGY),
    E1(0x2F, TEXT_AND, M_ABSL),
    E2(0x3F, TEXT_AND, M_ABSL, M_REGX),
    E2(0x43, TEXT_EOR, M_DPG,  M_REGS),
    E3(0x53, TEXT_EOR, I_DPG,  I_REGS, M_REGY),
    E1(0x47, TEXT_EOR, L_DPG),
    E2(0x57, TEXT_EOR, L_DPG,  M_REGY),
    E1(0x4F, TEXT_EOR, M_ABSL),
    E2(0x5F, TEXT_EOR, M_ABSL, M_REGX),
    E2(0x63, TEXT_ADC, M_DPG,  M_REGS),
    E3(0x73, TEXT_ADC, I_DPG,  I_REGS, M_REGY),
    E1(0x67, TEXT_ADC, L_DPG),
    E2(0x77, TEXT_ADC, L_DPG,  M_REGY),
    E1(0x6F, TEXT_ADC, M_ABSL),
    E2(0x7F, TEXT_ADC, M_ABSL, M_REGX),
    E2(0x83, TEXT_STA, M_DPG,  M_REGS),
    E3(0x93, TEXT_STA, I_DPG,  I_REGS, M_REGY),
    E1(0x87, TEXT_STA, L_DPG),
    E2(0x97, TEXT_STA, L_DPG,  M_REGY),
    E1(0x8F, TEXT_STA, M_ABSL),
    E2(0x9F, TEXT_STA, M_ABSL, M_REGX),
    E2(0xA3, TEXT_LDA, M_DPG,  M_REGS),
    E3(0xB3, TEXT_LDA, I_DPG,  I_REGS, M_REGY),
    E1(0xA7, TEXT_LDA, L_DPG),
    E2(0xB7, TEXT_LDA, L_DPG,  M_REGY),
    E1(0xAF, TEXT_LDA, M_ABSL),
    E2(0xBF, TEXT_LDA, M_ABSL, M_REGX),
    E2(0xC3, TEXT_CMP, M_DPG,  M_REGS),
    E3(0xD3, TEXT_CMP, I_DPG,  I_REGS, M_REGY),
    E1(0xC7, TEXT_CMP, L_DPG),
    E2(0xD7, TEXT_CMP, L_DPG,  M_REGY),
    E1(0xCF, TEXT_CMP, M_ABSL),
    E2(0xDF, TEXT_CMP, M_ABSL, M_REGX),
    E2(0xE3, TEXT_SBC, M_DPG,  M_REGS),
    E3(0xF3, TEXT_SBC, I_DPG,  I_REGS, M_REGY),
    E1(0xE7, TEXT_SBC, L_DPG),
    E2(0xF7, TEXT_SBC, L_DPG,  M_REGY),
    E1(0xEF, TEXT_SBC, M_ABSL),
    E2(0xFF, TEXT_SBC, M_ABSL, M_REGX),
    E1(0xDC, TEXT_JMPL, I_ABS),
    E1(0x07, TEXT_ORAL, I_DPG),
    E2(0x17, TEXT_ORAL, I_DPG, M_REGY),
    E1(0x27, TEXT_ANDL, I_DPG),
    E2(0x37, TEXT_ANDL, I_DPG, M_REGY),
    E1(0x47, TEXT_EORL, I_DPG),
    E2(0x57, TEXT_EORL, I_DPG, M_REGY),
    E1(0x67, TEXT_ADCL, I_DPG),
    E2(0x77, TEXT_ADCL, I_DPG, M_REGY),
    E1(0x87, TEXT_STAL, I_DPG),
    E2(0x97, TEXT_STAL, I_DPG, M_REGY),
    E1(0xA7, TEXT_LDAL, I_DPG),
    E2(0xB7, TEXT_LDAL, I_DPG, M_REGY),
    E1(0xC7, TEXT_CMPL, I_DPG),
    E2(0xD7, TEXT_CMPL, I_DPG, M_REGY),
    E1(0xE7, TEXT_SBCL, I_DPG),
    E2(0xF7, TEXT_SBCL, I_DPG, M_REGY),
};

static constexpr uint8_t W65C816_INDEX[] PROGMEM = {
     46,  // TEXT_ADC
     47,  // TEXT_ADC
     48,  // TEXT_ADC
     49,  // TEXT_ADC
     50,  // TEXT_ADC
     51,  // TEXT_ADC
     83,  // TEXT_ADCL
     84,  // TEXT_ADCL
     34,  // TEXT_AND
     35,  // TEXT_AND
     36,  // TEXT_AND
     37,  // TEXT_AND
     38,  // TEXT_AND
     39,  // TEXT_AND
     79,  // TEXT_ANDL
     80,  // TEXT_ANDL
     23,  // TEXT_BRL
     64,  // TEXT_CMP
     65,  // TEXT_CMP
     66,  // TEXT_CMP
     67,  // TEXT_CMP
     68,  // TEXT_CMP
     69,  // TEXT_CMP
     89,  // TEXT_CMPL
     90,  // TEXT_CMPL
     14,  // TEXT_COP
     40,  // TEXT_EOR
     41,  // TEXT_EOR
     42,  // TEXT_EOR
     43,  // TEXT_EOR
     44,  // TEXT_EOR
     45,  // TEXT_EOR
     81,  // TEXT_EORL
     82,  // TEXT_EORL
     18,  // TEXT_JMP
     19,  // TEXT_JMP
     76,  // TEXT_JMPL
     21,  // TEXT_JSL
     20,  // TEXT_JSR
     58,  // TEXT_LDA
     59,  // TEXT_LDA
     60,  // TEXT_LDA
     61,  // TEXT_LDA
     62,  // TEXT_LDA
     63,  // TEXT_LDA
     87,  // TEXT_LDAL
     88,  // TEXT_LDAL
     27,  // TEXT_MVN
     26,  // TEXT_MVP
     28,  // TEXT_ORA
     29,  // TEXT_ORA
     30,  // TEXT_ORA
     31,  // TEXT_ORA
     32,  // TEXT_ORA
     33,  // TEXT_ORA
     77,  // TEXT_ORAL
     78,  // TEXT_ORAL
     25,  // TEXT_PEA
     24,  // TEXT_PEI
     22,  // TEXT_PER
      8,  // TEXT_PHB
      0,  // TEXT_PHD
      4,  // TEXT_PHK
     10,  // TEXT_PLB
      2,  // TEXT_PLD
     16,  // TEXT_REP
      6,  // TEXT_RTL
     70,  // TEXT_SBC
     71,  // TEXT_SBC
     72,  // TEXT_SBC
     73,  // TEXT_SBC
     74,  // TEXT_SBC
     75,  // TEXT_SBC
     91,  // TEXT_SBCL
     92,  // TEXT_SBCL
     17,  // TEXT_SEP
     52,  // TEXT_STA
     53,  // TEXT_STA
     54,  // TEXT_STA
     55,  // TEXT_STA
     56,  // TEXT_STA
     57,  // TEXT_STA
     85,  // TEXT_STAL
     86,  // TEXT_STAL
      5,  // TEXT_TCD
      1,  // TEXT_TCS
      7,  // TEXT_TDC
      3,  // TEXT_TSC
      9,  // TEXT_TXY
     11,  // TEXT_TYX
     15,  // TEXT_WDM
     12,  // TEXT_XBA
     13,  // TEXT_XCE
};
// clang-format on

static constexpr TableMos6502::EntryPage MOS6502_PAGES[] PROGMEM = {
        {ARRAY_RANGE(MOS6502_TABLE), ARRAY_RANGE(MOS6502_INDEX)},
};
static constexpr TableMos6502::EntryPage G65SC02_PAGES[] PROGMEM = {
        {ARRAY_RANGE(MOS6502_TABLE), ARRAY_RANGE(MOS6502_INDEX)},
        {ARRAY_RANGE(G65SC02_TABLE), ARRAY_RANGE(G65SC02_INDEX)},
};
static constexpr TableMos6502::EntryPage R65C02_PAGES[] PROGMEM = {
        {ARRAY_RANGE(MOS6502_TABLE), ARRAY_RANGE(MOS6502_INDEX)},
        {ARRAY_RANGE(G65SC02_TABLE), ARRAY_RANGE(G65SC02_INDEX)},
        {ARRAY_RANGE(R65C02_TABLE), ARRAY_RANGE(R65C02_INDEX)},
};
static constexpr TableMos6502::EntryPage W65C02S_PAGES[] PROGMEM = {
        {ARRAY_RANGE(MOS6502_TABLE), ARRAY_RANGE(MOS6502_INDEX)},
        {ARRAY_RANGE(G65SC02_TABLE), ARRAY_RANGE(G65SC02_INDEX)},
        {ARRAY_RANGE(R65C02_TABLE), ARRAY_RANGE(R65C02_INDEX)},
        {ARRAY_RANGE(W65C02S_TABLE), ARRAY_RANGE(W65C02S_INDEX)},
};
static constexpr TableMos6502::EntryPage W65C816_PAGES[] PROGMEM = {
        {ARRAY_RANGE(MOS6502_TABLE), ARRAY_RANGE(MOS6502_INDEX)},
        {ARRAY_RANGE(G65SC02_TABLE), ARRAY_RANGE(G65SC02_INDEX)},
        {ARRAY_RANGE(W65C02S_TABLE), ARRAY_RANGE(W65C02S_INDEX)},
        {ARRAY_RANGE(W65C816_TABLE), ARRAY_RANGE(W65C816_INDEX)},
};

static constexpr TableMos6502::Cpu CPU_TABLE[] PROGMEM = {
        {MOS6502, TEXT_CPU_6502, ARRAY_RANGE(MOS6502_PAGES)},
        {G65SC02, TEXT_CPU_65SC02, ARRAY_RANGE(G65SC02_PAGES)},
        {R65C02, TEXT_CPU_65C02, ARRAY_RANGE(R65C02_PAGES)},
        {W65C02S, TEXT_CPU_W65C02S, ARRAY_RANGE(W65C02S_PAGES)},
        {W65C816, TEXT_CPU_65816, ARRAY_RANGE(W65C816_PAGES)},
};

static bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_IMA)
        return table == M_IMX || table == M_IM8;
    if (opr == M_DPG)
        return table == M_ABS || table == M_REL || table == M_ABSL || table == M_RELL ||
               table == M_BANK;
    if (opr == M_ABS)
        return table == M_REL || table == M_ABSL || table == M_RELL || table == M_BANK;
    if (opr == M_ABSL)
        return table == M_REL || table == M_RELL || table == M_BANK;
    if (opr == I_DPG)
        return table == I_ABS;
    if (opr == L_DPG)
        return table == L_ABS;
    return false;
}

static bool acceptModes(InsnMos6502 &insn, const Entry *entry) {
    auto flags = insn.flags();
    auto table = entry->flags();
    return acceptMode(flags.mode1(), table.mode1()) && acceptMode(flags.mode2(), table.mode2()) &&
           acceptMode(flags.mode3(), table.mode3());
}

Error TableMos6502::searchName(InsnMos6502 &insn) const {
    _cpu->searchName(insn, acceptModes);
    return insn.getError();
}

static bool matchOpCode(
        InsnMos6502 &insn, const Entry *entry, const TableMos6502::EntryPage *page) {
    if (insn.opCode() != entry->opCode())
        return false;
    const auto mode = entry->flags().mode1();
    if (mode == L_ABS || mode == L_DPG)
        return insn.allowIndirectLong();
    return true;
}

Error TableMos6502::searchOpCode(InsnMos6502 &insn) const {
    insn.setAllowIndirectLong(_useIndirectLong);
    _cpu->searchOpCode(insn, matchOpCode);
    return insn.getError();
}

void TableMos6502::reset() {
    if (_cpu == nullptr)
        setCpu(MOS6502);
    useIndirectLong(true);
    setLongAccumulator(false);
    setLongIndex(false);
}

bool TableMos6502::setCpu(CpuType cpuType) {
    auto t = Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
    if (t == nullptr)
        return false;
    _cpu = t;
    useIndirectLong(_cpu->cpuType() == W65C816);
    return true;
}

AddressWidth TableMos6502::addressWidth() const {
    return _cpu->cpuType() == W65C816 ? ADDRESS_24BIT : ADDRESS_16BIT;
}

const /* PROGMEM */ char *TableMos6502::listCpu_P() const {
    return TEXT_CPU_LIST;
}

CpuType TableMos6502::cpuType() const {
    return _cpu->cpuType();
}

bool TableMos6502::setCpu(const char *cpu) {
    auto t = Cpu::search(cpu, ARRAY_RANGE(CPU_TABLE));
    if (t)
        return setCpu(t->cpuType());
    if (strcasecmp_P(cpu, TEXT_CPU_MOS6502) == 0)
        return setCpu(MOS6502);
    if (strcasecmp_P(cpu, TEXT_CPU_R65C02) == 0)
        return setCpu(R65C02);
    if (strcasecmp_P(cpu, TEXT_CPU_G65SC02) == 0)
        return setCpu(G65SC02);
    if (toupper(*cpu) == 'W') {
        ++cpu;
        if (strcasecmp_P(cpu, TEXT_CPU_65C02) == 0)
            return setCpu(W65C02S);
        if (strcasecmp_P(cpu, TEXT_CPU_65816) == 0 || strcasecmp_P(cpu, TEXT_CPU_65C816) == 0 ||
                strcasecmp_P(cpu, TEXT_CPU_65C816S) == 0)
            return setCpu(W65C816);
    }
    return false;
}

bool TableMos6502::useIndirectLong(bool enable) {
    if (enable && _cpu->cpuType() != W65C816) {
        _useIndirectLong = false;
        return false;
    }
    _useIndirectLong = enable;
    return true;
}

bool TableMos6502::setLongAccumulator(bool on) {
    if (on && _cpu->cpuType() != W65C816) {
        _longAccumulator = false;
        return false;
    }
    _longAccumulator = on;
    return true;
}

bool TableMos6502::setLongIndex(bool on) {
    if (on && _cpu->cpuType() != W65C816) {
        _longIndex = false;
        return false;
    }
    _longIndex = on;
    return true;
}

bool TableMos6502::longImmediate(AddrMode mode) const {
    if (_cpu->cpuType() == W65C816) {
        if (mode == M_IMA)
            return _longAccumulator;
        if (mode == M_IMX)
            return _longIndex;
    }
    return false;
}

TableMos6502 TableMos6502::TABLE;

}  // namespace mos6502
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

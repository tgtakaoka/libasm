/*
 * Copyright 2021 Tadashi G. Takaoka
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

#include "table_mc6805.h"
#include "entry_mc6805.h"
#include "entry_table.h"
#include "text_mc6805.h"

using namespace libasm::text::mc6805;

namespace libasm {
namespace mc6805 {

#define E3(_opc, _cf, _name, _opr1, _opr2, _opr3) \
    {_opc, Entry::Flags::create(_cf, _opr1, _opr2, _opr3), _name}
#define E2(_opc, _cf, _name, _opr1, _opr2) E3(_opc, _cf, _name, _opr1, _opr2, M_NONE)
#define E1(_opc, _cf, _name, _opr1) E2(_opc, _cf, _name, _opr1, M_NONE)
#define E0(_opc, _cf, _name) E1(_opc, _cf, _name, M_NONE)

// clang-format off
constexpr Entry MC6805_TABLE[] PROGMEM = {
    E1(0x20, CF_00, TEXT_BRA,  M_REL),
    E1(0x21, CF_00, TEXT_BRN,  M_REL),
    E1(0x22, CF_00, TEXT_BHI,  M_REL),
    E1(0x23, CF_00, TEXT_BLS,  M_REL),
    E1(0x24, CF_00, TEXT_BHS,  M_REL),
    E1(0x24, CF_00, TEXT_BCC,  M_REL),
    E1(0x25, CF_00, TEXT_BLO,  M_REL),
    E1(0x25, CF_00, TEXT_BCS,  M_REL),
    E1(0x26, CF_00, TEXT_BNE,  M_REL),
    E1(0x27, CF_00, TEXT_BEQ,  M_REL),
    E1(0x28, CF_00, TEXT_BHCC, M_REL),
    E1(0x29, CF_00, TEXT_BHCS, M_REL),
    E1(0x2A, CF_00, TEXT_BPL,  M_REL),
    E1(0x2B, CF_00, TEXT_BMI,  M_REL),
    E1(0x2C, CF_00, TEXT_BMC,  M_REL),
    E1(0x2D, CF_00, TEXT_BMS,  M_REL),
    E1(0x2E, CF_00, TEXT_BIL,  M_REL),
    E1(0x2F, CF_00, TEXT_BIH,  M_REL),
    E1(0x30, CF_00, TEXT_NEG,  M_DIR),
    E1(0x33, CF_00, TEXT_COM,  M_DIR),
    E1(0x34, CF_00, TEXT_LSR,  M_DIR),
    E1(0x36, CF_00, TEXT_ROR,  M_DIR),
    E1(0x37, CF_00, TEXT_ASR,  M_DIR),
    E1(0x38, CF_00, TEXT_ASL,  M_DIR),
    E1(0x38, CF_00, TEXT_LSL,  M_DIR),
    E1(0x39, CF_00, TEXT_ROL,  M_DIR),
    E1(0x3A, CF_00, TEXT_DEC,  M_DIR),
    E1(0x3C, CF_00, TEXT_INC,  M_DIR),
    E1(0x3D, CF_00, TEXT_TST,  M_DIR),
    E1(0x3F, CF_00, TEXT_CLR,  M_DIR),
    E1(0x60, CF_10, TEXT_NEG,  M_IX10),
    E1(0x63, CF_10, TEXT_COM,  M_IX10),
    E1(0x64, CF_10, TEXT_LSR,  M_IX10),
    E1(0x66, CF_10, TEXT_ROR,  M_IX10),
    E1(0x67, CF_10, TEXT_ASR,  M_IX10),
    E1(0x68, CF_10, TEXT_ASL,  M_IX10),
    E1(0x68, CF_10, TEXT_LSL,  M_IX10),
    E1(0x69, CF_10, TEXT_ROL,  M_IX10),
    E1(0x6A, CF_10, TEXT_DEC,  M_IX10),
    E1(0x6C, CF_10, TEXT_INC,  M_IX10),
    E1(0x6D, CF_10, TEXT_TST,  M_IX10),
    E1(0x6F, CF_10, TEXT_CLR,  M_IX10),
    E0(0x40, CF_00, TEXT_NEGA),
    E0(0x43, CF_00, TEXT_COMA),
    E0(0x44, CF_00, TEXT_LSRA),
    E0(0x46, CF_00, TEXT_RORA),
    E0(0x47, CF_00, TEXT_ASRA),
    E0(0x48, CF_00, TEXT_ASLA),
    E0(0x48, CF_00, TEXT_LSLA),
    E0(0x49, CF_00, TEXT_ROLA),
    E0(0x4A, CF_00, TEXT_DECA),
    E0(0x4C, CF_00, TEXT_INCA),
    E0(0x4D, CF_00, TEXT_TSTA),
    E0(0x4F, CF_00, TEXT_CLRA),
    E0(0x50, CF_00, TEXT_NEGX),
    E0(0x53, CF_00, TEXT_COMX),
    E0(0x54, CF_00, TEXT_LSRX),
    E0(0x56, CF_00, TEXT_RORX),
    E0(0x57, CF_00, TEXT_ASRX),
    E0(0x58, CF_00, TEXT_ASLX),
    E0(0x58, CF_00, TEXT_LSLX),
    E0(0x59, CF_00, TEXT_ROLX),
    E0(0x5A, CF_00, TEXT_DECX),
    E0(0x5C, CF_00, TEXT_INCX),
    E0(0x5D, CF_00, TEXT_TSTX),
    E0(0x5F, CF_00, TEXT_CLRX),
    E1(0xA0, CF_10, TEXT_SUB,  M_GEN1),
    E1(0xA1, CF_10, TEXT_CMP,  M_GEN1),
    E1(0xA2, CF_10, TEXT_SBC,  M_GEN1),
    E1(0xA3, CF_10, TEXT_CPX,  M_GEN1),
    E1(0xA4, CF_10, TEXT_AND,  M_GEN1),
    E1(0xA5, CF_10, TEXT_BIT,  M_GEN1),
    E1(0xA6, CF_10, TEXT_LDA,  M_GEN1),
    E1(0xB7, CF_00, TEXT_STA,  M_DIR),
    E1(0xA8, CF_10, TEXT_EOR,  M_GEN1),
    E1(0xA9, CF_10, TEXT_ADC,  M_GEN1),
    E1(0xAA, CF_10, TEXT_ORA,  M_GEN1),
    E1(0xAB, CF_10, TEXT_ADD,  M_GEN1),
    E1(0xBC, CF_00, TEXT_JMP,  M_DIR),
    E1(0xAE, CF_10, TEXT_LDX,  M_GEN1),
    E1(0xBF, CF_00, TEXT_STX,  M_DIR),
    E1(0xAD, CF_00, TEXT_BSR,  M_REL),
    E1(0xBD, CF_00, TEXT_JSR,  M_DIR),
    E1(0xC0, CF_30, TEXT_SUB,  M_GEN2),
    E1(0xC1, CF_30, TEXT_CMP,  M_GEN2),
    E1(0xC2, CF_30, TEXT_SBC,  M_GEN2),
    E1(0xC3, CF_30, TEXT_CPX,  M_GEN2),
    E1(0xC4, CF_30, TEXT_AND,  M_GEN2),
    E1(0xC5, CF_30, TEXT_BIT,  M_GEN2),
    E1(0xC6, CF_30, TEXT_LDA,  M_GEN2),
    E1(0xC7, CF_30, TEXT_STA,  M_GEN2),
    E1(0xC8, CF_30, TEXT_EOR,  M_GEN2),
    E1(0xC9, CF_30, TEXT_ADC,  M_GEN2),
    E1(0xCA, CF_30, TEXT_ORA,  M_GEN2),
    E1(0xCB, CF_30, TEXT_ADD,  M_GEN2),
    E1(0xCC, CF_30, TEXT_JMP,  M_GEN2),
    E1(0xCE, CF_30, TEXT_LDX,  M_GEN2),
    E1(0xCF, CF_30, TEXT_STX,  M_GEN2),
    E1(0xCD, CF_30, TEXT_JSR,  M_GEN2),
    E0(0x80, CF_00, TEXT_RTI),
    E0(0x81, CF_00, TEXT_RTS),
    E0(0x83, CF_00, TEXT_SWI),
    E0(0x97, CF_00, TEXT_TAX),
    E0(0x98, CF_00, TEXT_CLC),
    E0(0x99, CF_00, TEXT_SEC),
    E0(0x9A, CF_00, TEXT_CLI),
    E0(0x9B, CF_00, TEXT_SEI),
    E0(0x9C, CF_00, TEXT_RSP),
    E0(0x9D, CF_00, TEXT_NOP),
    E0(0x9F, CF_00, TEXT_TXA),
    E2(0x10, CF_00, TEXT_BSET,  M_BNO, M_DIR),
    E2(0x11, CF_00, TEXT_BCLR,  M_BNO, M_DIR),
    E3(0x00, CF_00, TEXT_BRSET, M_BNO, M_DIR, M_REL),
    E3(0x01, CF_00, TEXT_BRCLR, M_BNO, M_DIR, M_REL),
};

constexpr uint8_t MC6805_INDEX[] PROGMEM = {
     75,  // TEXT_ADC
     92,  // TEXT_ADC
     77,  // TEXT_ADD
     94,  // TEXT_ADD
     70,  // TEXT_AND
     87,  // TEXT_AND
     23,  // TEXT_ASL
     35,  // TEXT_ASL
     47,  // TEXT_ASLA
     59,  // TEXT_ASLX
     22,  // TEXT_ASR
     34,  // TEXT_ASR
     46,  // TEXT_ASRA
     58,  // TEXT_ASRX
      5,  // TEXT_BCC
    111,  // TEXT_BCLR
      7,  // TEXT_BCS
      9,  // TEXT_BEQ
     10,  // TEXT_BHCC
     11,  // TEXT_BHCS
      2,  // TEXT_BHI
      4,  // TEXT_BHS
     17,  // TEXT_BIH
     16,  // TEXT_BIL
     71,  // TEXT_BIT
     88,  // TEXT_BIT
      6,  // TEXT_BLO
      3,  // TEXT_BLS
     14,  // TEXT_BMC
     13,  // TEXT_BMI
     15,  // TEXT_BMS
      8,  // TEXT_BNE
     12,  // TEXT_BPL
      0,  // TEXT_BRA
    113,  // TEXT_BRCLR
      1,  // TEXT_BRN
    112,  // TEXT_BRSET
    110,  // TEXT_BSET
     81,  // TEXT_BSR
    103,  // TEXT_CLC
    105,  // TEXT_CLI
     29,  // TEXT_CLR
     41,  // TEXT_CLR
     53,  // TEXT_CLRA
     65,  // TEXT_CLRX
     67,  // TEXT_CMP
     84,  // TEXT_CMP
     19,  // TEXT_COM
     31,  // TEXT_COM
     43,  // TEXT_COMA
     55,  // TEXT_COMX
     69,  // TEXT_CPX
     86,  // TEXT_CPX
     26,  // TEXT_DEC
     38,  // TEXT_DEC
     50,  // TEXT_DECA
     62,  // TEXT_DECX
     74,  // TEXT_EOR
     91,  // TEXT_EOR
     27,  // TEXT_INC
     39,  // TEXT_INC
     51,  // TEXT_INCA
     63,  // TEXT_INCX
     78,  // TEXT_JMP
     95,  // TEXT_JMP
     82,  // TEXT_JSR
     98,  // TEXT_JSR
     72,  // TEXT_LDA
     89,  // TEXT_LDA
     79,  // TEXT_LDX
     96,  // TEXT_LDX
     24,  // TEXT_LSL
     36,  // TEXT_LSL
     48,  // TEXT_LSLA
     60,  // TEXT_LSLX
     20,  // TEXT_LSR
     32,  // TEXT_LSR
     44,  // TEXT_LSRA
     56,  // TEXT_LSRX
     18,  // TEXT_NEG
     30,  // TEXT_NEG
     42,  // TEXT_NEGA
     54,  // TEXT_NEGX
    108,  // TEXT_NOP
     76,  // TEXT_ORA
     93,  // TEXT_ORA
     25,  // TEXT_ROL
     37,  // TEXT_ROL
     49,  // TEXT_ROLA
     61,  // TEXT_ROLX
     21,  // TEXT_ROR
     33,  // TEXT_ROR
     45,  // TEXT_RORA
     57,  // TEXT_RORX
    107,  // TEXT_RSP
     99,  // TEXT_RTI
    100,  // TEXT_RTS
     68,  // TEXT_SBC
     85,  // TEXT_SBC
    104,  // TEXT_SEC
    106,  // TEXT_SEI
     73,  // TEXT_STA
     90,  // TEXT_STA
     80,  // TEXT_STX
     97,  // TEXT_STX
     66,  // TEXT_SUB
     83,  // TEXT_SUB
    101,  // TEXT_SWI
    102,  // TEXT_TAX
     28,  // TEXT_TST
     40,  // TEXT_TST
     52,  // TEXT_TSTA
     64,  // TEXT_TSTX
    109,  // TEXT_TXA
};

constexpr Entry MC146805_TABLE[] PROGMEM = {
    E0(0x8E, CF_00, TEXT_STOP),
    E0(0x8F, CF_00, TEXT_WAIT),
};

constexpr uint8_t MC146805_INDEX[] PROGMEM = {
      0,  // TEXT_STOP
      1,  // TEXT_WAIT
};

constexpr Entry MC68HC05_TABLE[] PROGMEM = {
    E0(0x42, CF_00, TEXT_MUL),
};

constexpr uint8_t MC68HC05_INDEX[] PROGMEM = {
      0,  // TEXT_MUL
};

constexpr Entry MC68HC08_TABLE[] PROGMEM = {
    E1(0x35, CF_00, TEXT_STHX,  M_DIR),
    E1(0x45, CF_00, TEXT_LDHX,  M_IM16),
    E1(0x55, CF_00, TEXT_LDHX,  M_DIR),
    E1(0x65, CF_00, TEXT_CPHX,  M_IM16),
    E1(0x75, CF_00, TEXT_CPHX,  M_DIR),
    E2(0x4E, CF_00, TEXT_MOV,   M_DIR,  M_DIR),
    E2(0x5E, CF_00, TEXT_MOV,   M_DIR,  M_IX0P), // for disassembler
    E1(0x5E, CF_00, TEXT_MOV,   M_IX1P),         // for assembler
    E2(0x6E, CF_00, TEXT_MOV,   M_IMM,  M_DIR),
    E2(0x7E, CF_00, TEXT_MOV,   M_IX0P, M_DIR),
    E1(0xA7, CF_00, TEXT_AIS,   M_SIM8),
    E1(0xAF, CF_00, TEXT_AIX,   M_SIM8),
    E0(0x52, CF_00, TEXT_DIV),
    E0(0x62, CF_00, TEXT_NSA),
    E0(0x72, CF_00, TEXT_DAA),
    E0(0x84, CF_00, TEXT_TAP),
    E0(0x85, CF_00, TEXT_TPA),
    E0(0x86, CF_00, TEXT_PULA),
    E0(0x87, CF_00, TEXT_PSHA),
    E0(0x88, CF_00, TEXT_PULX),
    E0(0x89, CF_00, TEXT_PSHX),
    E0(0x8A, CF_00, TEXT_PULH),
    E0(0x8B, CF_00, TEXT_PSHH),
    E0(0x8C, CF_00, TEXT_CLRH),
    E0(0x94, CF_00, TEXT_TXS),
    E0(0x95, CF_00, TEXT_TSX),
    E1(0x90, CF_00, TEXT_BGE,   M_REL),
    E1(0x91, CF_00, TEXT_BLT,   M_REL),
    E1(0x92, CF_00, TEXT_BGT,   M_REL),
    E1(0x93, CF_00, TEXT_BLE,   M_REL),
    E2(0x3B, CF_00, TEXT_DBNZ,  M_DIR,  M_REL),
    E1(0x4B, CF_00, TEXT_DBNZA, M_REL),
    E1(0x5B, CF_00, TEXT_DBNZX, M_REL),
    E2(0x6B, CF_00, TEXT_DBNZ,  M_IX1,  M_REL),
    E2(0x7B, CF_00, TEXT_DBNZ,  M_IX0,  M_REL),
    E2(0x7B, CF_00, TEXT_DBNZ,  M_REGX, M_REL),
    E2(0x31, CF_00, TEXT_CBEQ,  M_DIR,  M_REL),
    E2(0x41, CF_00, TEXT_CBEQA, M_IMM,  M_REL),
    E2(0x51, CF_00, TEXT_CBEQX, M_IMM,  M_REL),
    E2(0x61, CF_00, TEXT_CBEQ,  M_IX1P, M_REL),
    E2(0x71, CF_00, TEXT_CBEQ,  M_IX0P, M_REL),
};

constexpr uint8_t MC68HC08_INDEX[] PROGMEM = {
     10,  // TEXT_AIS
     11,  // TEXT_AIX
     26,  // TEXT_BGE
     28,  // TEXT_BGT
     29,  // TEXT_BLE
     27,  // TEXT_BLT
     36,  // TEXT_CBEQ
     39,  // TEXT_CBEQ
     40,  // TEXT_CBEQ
     37,  // TEXT_CBEQA
     38,  // TEXT_CBEQX
     23,  // TEXT_CLRH
      3,  // TEXT_CPHX
      4,  // TEXT_CPHX
     14,  // TEXT_DAA
     30,  // TEXT_DBNZ
     33,  // TEXT_DBNZ
     34,  // TEXT_DBNZ
     35,  // TEXT_DBNZ
     31,  // TEXT_DBNZA
     32,  // TEXT_DBNZX
     12,  // TEXT_DIV
      1,  // TEXT_LDHX
      2,  // TEXT_LDHX
      5,  // TEXT_MOV
      6,  // TEXT_MOV
      7,  // TEXT_MOV
      8,  // TEXT_MOV
      9,  // TEXT_MOV
     13,  // TEXT_NSA
     18,  // TEXT_PSHA
     22,  // TEXT_PSHH
     20,  // TEXT_PSHX
     17,  // TEXT_PULA
     21,  // TEXT_PULH
     19,  // TEXT_PULX
      0,  // TEXT_STHX
     15,  // TEXT_TAP
     16,  // TEXT_TPA
     25,  // TEXT_TSX
     24,  // TEXT_TXS
};

constexpr Entry MC68HC08_P9E[] PROGMEM = {
    E1(0x60, CF_00, TEXT_NEG,  M_SP1),
    E2(0x61, CF_00, TEXT_CBEQ, M_SP1,  M_REL),
    E1(0x63, CF_00, TEXT_COM,  M_SP1),
    E1(0x64, CF_00, TEXT_LSR,  M_SP1),
    E1(0x66, CF_00, TEXT_ROR,  M_SP1),
    E1(0x67, CF_00, TEXT_ASR,  M_SP1),
    E1(0x68, CF_00, TEXT_ASL,  M_SP1),
    E1(0x68, CF_00, TEXT_LSL,  M_SP1),
    E1(0x69, CF_00, TEXT_ROL,  M_SP1),
    E1(0x6A, CF_00, TEXT_DEC,  M_SP1),
    E2(0x6B, CF_00, TEXT_DBNZ, M_SP1,  M_REL),
    E1(0x6C, CF_00, TEXT_INC,  M_SP1),
    E1(0x6D, CF_00, TEXT_TST,  M_SP1),
    E1(0x6F, CF_00, TEXT_CLR,  M_SP1),
    E1(0xE0, CF_00, TEXT_SUB,  M_SP1),
    E1(0xE1, CF_00, TEXT_CMP,  M_SP1),
    E1(0xE2, CF_00, TEXT_SBC,  M_SP1),
    E1(0xE3, CF_00, TEXT_CPX,  M_SP1),
    E1(0xE4, CF_00, TEXT_AND,  M_SP1),
    E1(0xE5, CF_00, TEXT_BIT,  M_SP1),
    E1(0xE6, CF_00, TEXT_LDA,  M_SP1),
    E1(0xE7, CF_00, TEXT_STA,  M_SP1),
    E1(0xE8, CF_00, TEXT_EOR,  M_SP1),
    E1(0xE9, CF_00, TEXT_ADC,  M_SP1),
    E1(0xEA, CF_00, TEXT_ORA,  M_SP1),
    E1(0xEB, CF_00, TEXT_ADD,  M_SP1),
    E1(0xEE, CF_00, TEXT_LDX,  M_SP1),
    E1(0xEF, CF_00, TEXT_STX,  M_SP1),
    E1(0xD0, CF_00, TEXT_SUB,  M_SP2),
    E1(0xD1, CF_00, TEXT_CMP,  M_SP2),
    E1(0xD2, CF_00, TEXT_SBC,  M_SP2),
    E1(0xD3, CF_00, TEXT_CPX,  M_SP2),
    E1(0xD4, CF_00, TEXT_AND,  M_SP2),
    E1(0xD5, CF_00, TEXT_BIT,  M_SP2),
    E1(0xD6, CF_00, TEXT_LDA,  M_SP2),
    E1(0xD7, CF_00, TEXT_STA,  M_SP2),
    E1(0xD8, CF_00, TEXT_EOR,  M_SP2),
    E1(0xD9, CF_00, TEXT_ADC,  M_SP2),
    E1(0xDA, CF_00, TEXT_ORA,  M_SP2),
    E1(0xDB, CF_00, TEXT_ADD,  M_SP2),
    E1(0xDE, CF_00, TEXT_LDX,  M_SP2),
    E1(0xDF, CF_00, TEXT_STX,  M_SP2),
};

constexpr uint8_t MC68HC08_I9E[] PROGMEM = {
     23,  // TEXT_ADC
     37,  // TEXT_ADC
     25,  // TEXT_ADD
     39,  // TEXT_ADD
     18,  // TEXT_AND
     32,  // TEXT_AND
      6,  // TEXT_ASL
      5,  // TEXT_ASR
     19,  // TEXT_BIT
     33,  // TEXT_BIT
      1,  // TEXT_CBEQ
     13,  // TEXT_CLR
     15,  // TEXT_CMP
     29,  // TEXT_CMP
      2,  // TEXT_COM
     17,  // TEXT_CPX
     31,  // TEXT_CPX
     10,  // TEXT_DBNZ
      9,  // TEXT_DEC
     22,  // TEXT_EOR
     36,  // TEXT_EOR
     11,  // TEXT_INC
     20,  // TEXT_LDA
     34,  // TEXT_LDA
     26,  // TEXT_LDX
     40,  // TEXT_LDX
      7,  // TEXT_LSL
      3,  // TEXT_LSR
      0,  // TEXT_NEG
     24,  // TEXT_ORA
     38,  // TEXT_ORA
      8,  // TEXT_ROL
      4,  // TEXT_ROR
     16,  // TEXT_SBC
     30,  // TEXT_SBC
     21,  // TEXT_STA
     35,  // TEXT_STA
     27,  // TEXT_STX
     41,  // TEXT_STX
     14,  // TEXT_SUB
     28,  // TEXT_SUB
     12,  // TEXT_TST
};

constexpr Entry MC68HCS08_TABLE[] PROGMEM = {
    E1(0x35, CF_00, TEXT_STHX, M_DIR),
    E1(0x55, CF_00, TEXT_LDHX, M_DIR),
    E1(0x75, CF_00, TEXT_CPHX, M_DIR),
    E1(0x32, CF_00, TEXT_LDHX, M_EXT),
    E1(0x3E, CF_00, TEXT_CPHX, M_EXT),
    E1(0x96, CF_00, TEXT_STHX, M_EXT),
    E0(0x82, CF_00, TEXT_BGND),
};

constexpr uint8_t MC68HCS08_INDEX[] PROGMEM = {
      6,  // TEXT_BGND
      2,  // TEXT_CPHX
      4,  // TEXT_CPHX
      1,  // TEXT_LDHX
      3,  // TEXT_LDHX
      0,  // TEXT_STHX
      5,  // TEXT_STHX
};

constexpr Entry MC68HCS08_P9E[] PROGMEM = {
    E1(0xAE, CF_00, TEXT_LDHX, M_IX0),
    E1(0xBE, CF_00, TEXT_LDHX, M_IX2),
    E1(0xCE, CF_00, TEXT_LDHX, M_IX1),
    E1(0xFE, CF_00, TEXT_LDHX, M_SP1),
    E1(0xFF, CF_00, TEXT_STHX, M_SP1),
    E1(0xF3, CF_00, TEXT_CPHX, M_SP1),
};

constexpr uint8_t MC68HCS08_I9E[] PROGMEM = {
      5,  // TEXT_CPHX
      0,  // TEXT_LDHX
      1,  // TEXT_LDHX
      2,  // TEXT_LDHX
      3,  // TEXT_LDHX
      4,  // TEXT_STHX
};

// clang-format on

using EntryPage = entry::PrefixTableBase<Entry>;

constexpr EntryPage MC6805_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(MC6805_TABLE), ARRAY_RANGE(MC6805_INDEX)},
};

constexpr EntryPage MC146805_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(MC6805_TABLE), ARRAY_RANGE(MC6805_INDEX)},
        {0x00, ARRAY_RANGE(MC146805_TABLE), ARRAY_RANGE(MC146805_INDEX)},
};

constexpr EntryPage MC68HC05_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(MC6805_TABLE), ARRAY_RANGE(MC6805_INDEX)},
        {0x00, ARRAY_RANGE(MC146805_TABLE), ARRAY_RANGE(MC146805_INDEX)},
        {0x00, ARRAY_RANGE(MC68HC05_TABLE), ARRAY_RANGE(MC68HC05_INDEX)},
};

constexpr EntryPage MC68HC08_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(MC68HC08_TABLE), ARRAY_RANGE(MC68HC08_INDEX)},
        // Above definitions overrides unknown instructions defined below.
        {0x00, ARRAY_RANGE(MC6805_TABLE), ARRAY_RANGE(MC6805_INDEX)},
        {0x00, ARRAY_RANGE(MC146805_TABLE), ARRAY_RANGE(MC146805_INDEX)},
        {0x00, ARRAY_RANGE(MC68HC05_TABLE), ARRAY_RANGE(MC68HC05_INDEX)},
        {0x9E, ARRAY_RANGE(MC68HC08_P9E), ARRAY_RANGE(MC68HC08_I9E)},
};

constexpr EntryPage MC68HCS08_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(MC68HCS08_TABLE), ARRAY_RANGE(MC68HCS08_INDEX)},
        {0x00, ARRAY_RANGE(MC68HC08_TABLE), ARRAY_RANGE(MC68HC08_INDEX)},
        // Above definitions overrides unknown instructions defined below.
        {0x00, ARRAY_RANGE(MC6805_TABLE), ARRAY_RANGE(MC6805_INDEX)},
        {0x00, ARRAY_RANGE(MC146805_TABLE), ARRAY_RANGE(MC146805_INDEX)},
        {0x00, ARRAY_RANGE(MC68HC05_TABLE), ARRAY_RANGE(MC68HC05_INDEX)},
        {0x9E, ARRAY_RANGE(MC68HCS08_P9E), ARRAY_RANGE(MC68HCS08_I9E)},
        {0x9E, ARRAY_RANGE(MC68HC08_P9E), ARRAY_RANGE(MC68HC08_I9E)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {MC6805, TEXT_CPU_6805, ARRAY_RANGE(MC6805_PAGES)},
        {MC146805, TEXT_CPU_146805, ARRAY_RANGE(MC146805_PAGES)},
        {MC68HC05, TEXT_CPU_68HC05, ARRAY_RANGE(MC68HC05_PAGES)},
        {MC68HC08, TEXT_CPU_68HC08, ARRAY_RANGE(MC68HC08_PAGES)},
        {MC68HCS08, TEXT_CPU_68HCS08, ARRAY_RANGE(MC68HCS08_PAGES)},
};

const Cpu *cpu(CpuType cpuType) {
    return Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
}

bool hasOperand(CpuType cpuType, AsmInsn &insn) {
    cpu(cpuType)->searchName(insn, Cpu::acceptAll<AsmInsn, Entry>);
    return insn.isOK() && insn.mode1() != M_NONE;
}

bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_EXT)
        return table == M_REL || table == M_GEN2;
    if (opr == M_DIR)
        return table == M_REL || table == M_EXT || table == M_GEN1 || table == M_GEN2;
    if (opr == M_IMM)
        return table == M_IM16 || table == M_SIM8 || table == M_GEN1;
    if (opr == M_BNO)
        return table == M_REL || table == M_DIR || table == M_EXT || table == M_GEN1;
    if (opr == M_IX0 || opr == M_IX1)
        return table == M_GEN2 || table == M_IX10;
    if (opr == M_IX2)
        return table == M_GEN2;
    return false;
}

bool acceptModes(AsmInsn &insn, const Entry *entry) {
    const auto table = entry->readFlags();
    return acceptMode(insn.op1.mode, table.mode1()) && acceptMode(insn.op2.mode, table.mode2()) &&
           acceptMode(insn.op3.mode, table.mode3());
}

Error searchName(CpuType cpuType, AsmInsn &insn) {
    cpu(cpuType)->searchName(insn, acceptModes);
    return insn.getError();
}

bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *) {
    auto opc = insn.opCode();
    const auto flags = entry->readFlags();
    const auto mode1 = flags.mode1();
    if (mode1 == M_IX10 || mode1 == M_GEN1) {
        opc &= ~0x10;
    } else if (mode1 == M_GEN2) {
        opc &= ~0x30;
    } else if (mode1 == M_BNO) {
        opc &= ~0x0E;
    }
    return opc == entry->readOpCode();
}

Error searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) {
    cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    return insn.getError();
}

bool isPrefix(CpuType cpuType, Config::opcode_t code) {
    return cpu(cpuType)->isPrefix(code);
}

const /*PROGMEM*/ char *TableMc6805::listCpu_P() const {
    return TEXT_MC6805_LIST;
}

const /*PROGMEM*/ char *TableMc6805::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableMc6805::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    name.iexpectText_P(TEXT_MC6805_LIST, 2);
    auto t = Cpu::search(name, ARRAY_RANGE(CPU_TABLE));
    if (t) {
        cpuType = t->readCpuType();
        return OK;
    }
    return UNSUPPORTED_CPU;
}

const TableMc6805 TABLE;

}  // namespace mc6805
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

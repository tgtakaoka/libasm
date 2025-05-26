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

#include "table_mc68hc12.h"
#include "entry_mc68hc12.h"
#include "entry_table.h"
#include "text_mc68hc12.h"

using namespace libasm::text::mc68hc12;

namespace libasm {
namespace mc68hc12 {

#define E3(_opc, _cf, _name, _opr1, _opr2, _opr3) \
    {_opc, Entry::Flags::create(_cf, _opr1, _opr2, _opr3), _name}
#define E2(_opc, _cf, _name, _opr1, _opr2) E3(_opc, _cf, _name, _opr1, _opr2, M_NONE)
#define E1(_opc, _cf, _name, _opr1) E2(_opc, _cf, _name, _opr1, M_NONE)
#define E0(_opc, _cf, _name) E1(_opc, _cf, _name, M_NONE)

// clang-format off
constexpr Entry MC68HC12_TABLE[] PROGMEM = {
    E0(0x02, CF_00, TEXT_INY),
    E0(0x03, CF_00, TEXT_DEY),
    E1(0x05, CF_00, TEXT_JMP,  M_IDX),
    E1(0x06, CF_00, TEXT_JMP,  M_EXT),
    E1(0x07, CF_00, TEXT_BSR,  M_RL8),
    E0(0x08, CF_00, TEXT_INX),
    E0(0x09, CF_00, TEXT_DEX),
    E0(0x0A, CF_00, TEXT_RTC),
    E0(0x0B, CF_00, TEXT_RTI),
    E2(0x0C, CF_00, TEXT_BSET,  M_DIDX, M_IM8),
    E2(0x0D, CF_00, TEXT_BCLR,  M_DIDX, M_IM8),
    E3(0x0E, CF_00, TEXT_BRSET, M_DIDX, M_IM8, M_RL8),
    E3(0x0F, CF_00, TEXT_BRCLR, M_DIDX, M_IM8, M_RL8),
    E1(0x10, CF_00, TEXT_ANDCC, M_IM8),
    E0(0x11, CF_00, TEXT_EDIV),
    E0(0x12, CF_00, TEXT_MUL),
    E0(0x13, CF_00, TEXT_EMUL),
    E1(0x14, CF_00, TEXT_ORCC,  M_IM8),
    E1(0x15, CF_00, TEXT_JSR,   M_IDX),
    E1(0x17, CF_00, TEXT_JSR,   M_DIR),
    E1(0x16, CF_00, TEXT_JSR,   M_EXT),
    E1(0x19, CF_00, TEXT_LEAY,  M_DIDX),
    E1(0x1A, CF_00, TEXT_LEAX,  M_DIDX),
    E1(0x1B, CF_00, TEXT_LEAS,  M_DIDX),
    E2(0x4C, CF_00, TEXT_BSET,  M_DIR,  M_IM8),
    E2(0x4D, CF_00, TEXT_BCLR,  M_DIR,  M_IM8),
    E3(0x4E, CF_00, TEXT_BRSET, M_DIR,  M_IM8, M_RL8),
    E3(0x4F, CF_00, TEXT_BRCLR, M_DIR,  M_IM8, M_RL8),
    E2(0x1C, CF_00, TEXT_BSET,  M_EXT, M_IM8),
    E2(0x1D, CF_00, TEXT_BCLR,  M_EXT, M_IM8),
    E3(0x1E, CF_00, TEXT_BRSET, M_EXT, M_IM8, M_RL8),
    E3(0x1F, CF_00, TEXT_BRCLR, M_EXT, M_IM8, M_RL8),
    E1(0x20, CF_00, TEXT_BRA,   M_RL8),
    E1(0x21, CF_00, TEXT_BRN,   M_RL8),
    E1(0x22, CF_00, TEXT_BHI,   M_RL8),
    E1(0x23, CF_00, TEXT_BLS,   M_RL8),
    E1(0x24, CF_00, TEXT_BCC,   M_RL8),
    E1(0x24, CF_00, TEXT_BHS,   M_RL8),
    E1(0x25, CF_00, TEXT_BCS,   M_RL8),
    E1(0x25, CF_00, TEXT_BLO,   M_RL8),
    E1(0x26, CF_00, TEXT_BNE,   M_RL8),
    E1(0x27, CF_00, TEXT_BEQ,   M_RL8),
    E1(0x28, CF_00, TEXT_BVC,   M_RL8),
    E1(0x29, CF_00, TEXT_BVS,   M_RL8),
    E1(0x2A, CF_00, TEXT_BPL,   M_RL8),
    E1(0x2B, CF_00, TEXT_BMI,   M_RL8),
    E1(0x2C, CF_00, TEXT_BGE,   M_RL8),
    E1(0x2D, CF_00, TEXT_BLT,   M_RL8),
    E1(0x2E, CF_00, TEXT_BGT,   M_RL8),
    E1(0x2F, CF_00, TEXT_BLE,   M_RL8),
    E0(0x30, CF_00, TEXT_PULX),
    E0(0x31, CF_00, TEXT_PULY),
    E0(0x32, CF_00, TEXT_PULA),
    E0(0x33, CF_00, TEXT_PULB),
    E0(0x34, CF_00, TEXT_PSHX),
    E0(0x35, CF_00, TEXT_PSHY),
    E0(0x36, CF_00, TEXT_PSHA),
    E0(0x37, CF_00, TEXT_PSHB),
    E0(0x38, CF_00, TEXT_PULC),
    E0(0x39, CF_00, TEXT_PSHC),
    E0(0x3A, CF_00, TEXT_PULD),
    E0(0x3B, CF_00, TEXT_PSHD),
    E0(0x3D, CF_00, TEXT_RTS),
    E0(0x3E, CF_00, TEXT_WAI),
    E0(0x3F, CF_00, TEXT_SWI),
    E0(0x40, CF_00, TEXT_NEGA),
    E0(0x41, CF_00, TEXT_COMA),
    E0(0x42, CF_00, TEXT_INCA),
    E0(0x43, CF_00, TEXT_DECA),
    E0(0x44, CF_00, TEXT_LSRA),
    E0(0x45, CF_00, TEXT_ROLA),
    E0(0x46, CF_00, TEXT_RORA),
    E0(0x47, CF_00, TEXT_ASRA),
    E0(0x48, CF_00, TEXT_ASLA),
    E0(0x48, CF_00, TEXT_LSLA),
    E0(0x49, CF_00, TEXT_LSRD),
    E2(0x4A, CF_00, TEXT_CALL,  M_EXT,  M_PAG),
    E0(0x50, CF_00, TEXT_NEGB),
    E0(0x51, CF_00, TEXT_COMB),
    E0(0x52, CF_00, TEXT_INCB),
    E0(0x53, CF_00, TEXT_DECB),
    E0(0x54, CF_00, TEXT_LSRB),
    E0(0x55, CF_00, TEXT_ROLB),
    E0(0x56, CF_00, TEXT_RORB),
    E0(0x57, CF_00, TEXT_ASRB),
    E0(0x58, CF_00, TEXT_ASLB),
    E0(0x58, CF_00, TEXT_LSLB),
    E0(0x59, CF_00, TEXT_ASLD),
    E0(0x59, CF_00, TEXT_LSLD),
    E1(0x5A, CF_00, TEXT_STAA,  M_DIR),
    E1(0x5B, CF_00, TEXT_STAB,  M_DIR),
    E1(0x5C, CF_00, TEXT_STD,   M_DIR),
    E1(0x5D, CF_00, TEXT_STY,   M_DIR),
    E1(0x5E, CF_00, TEXT_STX,   M_DIR),
    E1(0x5F, CF_00, TEXT_STS,   M_DIR),
    E1(0x60, CF_10, TEXT_NEG,   M_GMEM),
    E1(0x61, CF_10, TEXT_COM,   M_GMEM),
    E1(0x62, CF_10, TEXT_INC,   M_GMEM),
    E1(0x63, CF_10, TEXT_DEC,   M_GMEM),
    E1(0x64, CF_10, TEXT_LSR,   M_GMEM),
    E1(0x65, CF_10, TEXT_ROL,   M_GMEM),
    E1(0x66, CF_10, TEXT_ROR,   M_GMEM),
    E1(0x67, CF_10, TEXT_ASR,   M_GMEM),
    E1(0x68, CF_10, TEXT_ASL,   M_GMEM),
    E1(0x68, CF_10, TEXT_LSL,   M_GMEM),
    E1(0x69, CF_10, TEXT_CLR,   M_GMEM),
    E1(0x6A, CF_10, TEXT_STAA,  M_GMEM),
    E1(0x6B, CF_10, TEXT_STAB,  M_GMEM),
    E1(0x6C, CF_10, TEXT_STD,   M_GMEM),
    E1(0x6D, CF_10, TEXT_STY,   M_GMEM),
    E1(0x6E, CF_10, TEXT_STX,   M_GMEM),
    E1(0x6F, CF_10, TEXT_STS,   M_GMEM),
    E1(0x80, CF_30, TEXT_SUBA,  M_GN8),
    E1(0x81, CF_30, TEXT_CMPA,  M_GN8),
    E1(0x82, CF_30, TEXT_SBCA,  M_GN8),
    E1(0x83, CF_30, TEXT_SUBD,  M_GN16),
    E1(0x84, CF_30, TEXT_ANDA,  M_GN8),
    E1(0x85, CF_30, TEXT_BITA,  M_GN8),
    E1(0x86, CF_30, TEXT_LDAA,  M_GN8),
    E1(0x88, CF_30, TEXT_EORA,  M_GN8),
    E1(0x89, CF_30, TEXT_ADCA,  M_GN8),
    E1(0x8A, CF_30, TEXT_ORAA,  M_GN8),
    E1(0x8B, CF_30, TEXT_ADDA,  M_GN8),
    E1(0x8C, CF_30, TEXT_CPD,   M_GN16),
    E1(0x8D, CF_30, TEXT_CPY,   M_GN16),
    E1(0x8E, CF_30, TEXT_CPX,   M_GN16),
    E1(0x8F, CF_30, TEXT_CPS,   M_GN16),
    E0(0x87, CF_00, TEXT_CLRA),
    E0(0x97, CF_00, TEXT_TSTA),
    E0(0xA7, CF_00, TEXT_NOP),
    E1(0xC0, CF_30, TEXT_SUBB,  M_GN8),
    E1(0xC1, CF_30, TEXT_CMPB,  M_GN8),
    E1(0xC2, CF_30, TEXT_SBCB,  M_GN8),
    E1(0xC3, CF_30, TEXT_ADDD,  M_GN16),
    E1(0xC4, CF_30, TEXT_ANDB,  M_GN8),
    E1(0xC5, CF_30, TEXT_BITB,  M_GN8),
    E1(0xC6, CF_30, TEXT_LDAB,  M_GN8),
    E1(0xC8, CF_30, TEXT_EORB,  M_GN8),
    E1(0xC9, CF_30, TEXT_ADCB,  M_GN8),
    E1(0xCA, CF_30, TEXT_ORAB,  M_GN8),
    E1(0xCB, CF_30, TEXT_ADDB,  M_GN8),
    E1(0xCC, CF_30, TEXT_LDD,   M_GN16),
    E1(0xCD, CF_30, TEXT_LDY,   M_GN16),
    E1(0xCE, CF_30, TEXT_LDX,   M_GN16),
    E1(0xCF, CF_30, TEXT_LDS,   M_GN16),
    E0(0xC7, CF_00, TEXT_CLRB),
    E0(0xD7, CF_00, TEXT_TSTB),
    E1(0xE7, CF_10, TEXT_TST,   M_GMEM),
    E0(0x00, CF_00, TEXT_BGND),
    E0(0x01, CF_00, TEXT_MEM),
};

constexpr uint8_t MC68HC12_INDEX[] PROGMEM = {
    120,  // TEXT_ADCA
    138,  // TEXT_ADCB
    122,  // TEXT_ADDA
    140,  // TEXT_ADDB
    133,  // TEXT_ADDD
    116,  // TEXT_ANDA
    134,  // TEXT_ANDB
     13,  // TEXT_ANDCC
    103,  // TEXT_ASL
     73,  // TEXT_ASLA
     85,  // TEXT_ASLB
     87,  // TEXT_ASLD
    102,  // TEXT_ASR
     72,  // TEXT_ASRA
     84,  // TEXT_ASRB
     36,  // TEXT_BCC
     10,  // TEXT_BCLR
     25,  // TEXT_BCLR
     29,  // TEXT_BCLR
     38,  // TEXT_BCS
     41,  // TEXT_BEQ
     46,  // TEXT_BGE
    148,  // TEXT_BGND
     48,  // TEXT_BGT
     34,  // TEXT_BHI
     37,  // TEXT_BHS
    117,  // TEXT_BITA
    135,  // TEXT_BITB
     49,  // TEXT_BLE
     39,  // TEXT_BLO
     35,  // TEXT_BLS
     47,  // TEXT_BLT
     45,  // TEXT_BMI
     40,  // TEXT_BNE
     44,  // TEXT_BPL
     32,  // TEXT_BRA
     12,  // TEXT_BRCLR
     27,  // TEXT_BRCLR
     31,  // TEXT_BRCLR
     33,  // TEXT_BRN
     11,  // TEXT_BRSET
     26,  // TEXT_BRSET
     30,  // TEXT_BRSET
      9,  // TEXT_BSET
     24,  // TEXT_BSET
     28,  // TEXT_BSET
      4,  // TEXT_BSR
     42,  // TEXT_BVC
     43,  // TEXT_BVS
     76,  // TEXT_CALL
    105,  // TEXT_CLR
    127,  // TEXT_CLRA
    145,  // TEXT_CLRB
    113,  // TEXT_CMPA
    131,  // TEXT_CMPB
     96,  // TEXT_COM
     66,  // TEXT_COMA
     78,  // TEXT_COMB
    123,  // TEXT_CPD
    126,  // TEXT_CPS
    125,  // TEXT_CPX
    124,  // TEXT_CPY
     98,  // TEXT_DEC
     68,  // TEXT_DECA
     80,  // TEXT_DECB
      6,  // TEXT_DEX
      1,  // TEXT_DEY
     14,  // TEXT_EDIV
     16,  // TEXT_EMUL
    119,  // TEXT_EORA
    137,  // TEXT_EORB
     97,  // TEXT_INC
     67,  // TEXT_INCA
     79,  // TEXT_INCB
      5,  // TEXT_INX
      0,  // TEXT_INY
      2,  // TEXT_JMP
      3,  // TEXT_JMP
     18,  // TEXT_JSR
     19,  // TEXT_JSR
     20,  // TEXT_JSR
    118,  // TEXT_LDAA
    136,  // TEXT_LDAB
    141,  // TEXT_LDD
    144,  // TEXT_LDS
    143,  // TEXT_LDX
    142,  // TEXT_LDY
     23,  // TEXT_LEAS
     22,  // TEXT_LEAX
     21,  // TEXT_LEAY
    104,  // TEXT_LSL
     74,  // TEXT_LSLA
     86,  // TEXT_LSLB
     88,  // TEXT_LSLD
     99,  // TEXT_LSR
     69,  // TEXT_LSRA
     81,  // TEXT_LSRB
     75,  // TEXT_LSRD
    149,  // TEXT_MEM
     15,  // TEXT_MUL
     95,  // TEXT_NEG
     65,  // TEXT_NEGA
     77,  // TEXT_NEGB
    129,  // TEXT_NOP
    121,  // TEXT_ORAA
    139,  // TEXT_ORAB
     17,  // TEXT_ORCC
     56,  // TEXT_PSHA
     57,  // TEXT_PSHB
     59,  // TEXT_PSHC
     61,  // TEXT_PSHD
     54,  // TEXT_PSHX
     55,  // TEXT_PSHY
     52,  // TEXT_PULA
     53,  // TEXT_PULB
     58,  // TEXT_PULC
     60,  // TEXT_PULD
     50,  // TEXT_PULX
     51,  // TEXT_PULY
    100,  // TEXT_ROL
     70,  // TEXT_ROLA
     82,  // TEXT_ROLB
    101,  // TEXT_ROR
     71,  // TEXT_RORA
     83,  // TEXT_RORB
      7,  // TEXT_RTC
      8,  // TEXT_RTI
     62,  // TEXT_RTS
    114,  // TEXT_SBCA
    132,  // TEXT_SBCB
     89,  // TEXT_STAA
    106,  // TEXT_STAA
     90,  // TEXT_STAB
    107,  // TEXT_STAB
     91,  // TEXT_STD
    108,  // TEXT_STD
     94,  // TEXT_STS
    111,  // TEXT_STS
     93,  // TEXT_STX
    110,  // TEXT_STX
     92,  // TEXT_STY
    109,  // TEXT_STY
    112,  // TEXT_SUBA
    130,  // TEXT_SUBB
    115,  // TEXT_SUBD
     64,  // TEXT_SWI
    147,  // TEXT_TST
    128,  // TEXT_TSTA
    146,  // TEXT_TSTB
     63,  // TEXT_WAI
};

constexpr Entry MC68HC12_P04[] PROGMEM = {
    E2(0x00, CF_11, TEXT_DBEQ, M_R8L,  M_RL9),
    E2(0x20, CF_11, TEXT_DBNE, M_R8L,  M_RL9),
    E2(0x40, CF_11, TEXT_TBEQ, M_R8L,  M_RL9),
    E2(0x60, CF_11, TEXT_TBNE, M_R8L,  M_RL9),
    E2(0x80, CF_11, TEXT_IBEQ, M_R8L,  M_RL9),
    E2(0xA0, CF_11, TEXT_IBNE, M_R8L,  M_RL9),
    E2(0x04, CF_13, TEXT_DBEQ, M_R16L, M_RL9),
    E2(0x24, CF_13, TEXT_DBNE, M_R16L, M_RL9),
    E2(0x44, CF_13, TEXT_TBEQ, M_R16L, M_RL9),
    E2(0x64, CF_13, TEXT_TBNE, M_R16L, M_RL9),
    E2(0x84, CF_13, TEXT_IBEQ, M_R16L, M_RL9),
    E2(0xA4, CF_13, TEXT_IBNE, M_R16L, M_RL9),
};

constexpr uint8_t MC68HC12_I04[] PROGMEM = {
      0,  // TEXT_DBEQ
      6,  // TEXT_DBEQ
      1,  // TEXT_DBNE
      7,  // TEXT_DBNE
      4,  // TEXT_IBEQ
     10,  // TEXT_IBEQ
      5,  // TEXT_IBNE
     11,  // TEXT_IBNE
      2,  // TEXT_TBEQ
      8,  // TEXT_TBEQ
      3,  // TEXT_TBNE
      9,  // TEXT_TBNE
};

constexpr Entry MC68HC12_P4B[] PROGMEM = {
    E1(0xE3, CF_1C, TEXT_CALL, M_CIDX),
    E2(0x00, CF_FF, TEXT_CALL, M_CIDX, M_PAG),
};

constexpr uint8_t MC68HC12_I4B[] PROGMEM = {
      0,  // TEXT_CALL
      1,  // TEXT_CALL
};

constexpr Entry MC68HC12_PB7[] PROGMEM = {
    E2(0x00, CF_33, TEXT_TFR, M_R8H,  M_R8L),
    E2(0x04, CF_33, TEXT_SEX, M_R8H,  M_R16L),
    E2(0x44, CF_33, TEXT_TFR, M_R16H, M_R16L),
    E2(0x80, CF_33, TEXT_EXG, M_R8H,  M_R8L),
    E2(0xC4, CF_33, TEXT_EXG, M_R16H, M_R16L),
    E2(0x40, CF_33, TEXT_TFR, M_R16H, M_R8L),
    E2(0x84, CF_33, TEXT_EXG, M_R8H,  M_R16L),
    E2(0xC0, CF_33, TEXT_EXG, M_R16H, M_R8L),
    E1(0x00, CF_00, TEXT_TFR, M_PAIR),
    E1(0x04, CF_00, TEXT_SEX, M_PAIR),
    E1(0x80, CF_00, TEXT_EXG, M_PAIR),
    E0(0x02, CF_00, TEXT_TAP),
    E0(0x20, CF_00, TEXT_TPA),
    E0(0x75, CF_00, TEXT_TSX),
    E0(0x57, CF_00, TEXT_TXS),
    E0(0x76, CF_00, TEXT_TSY),
    E0(0x67, CF_00, TEXT_TYS),
    E0(0xC5, CF_00, TEXT_XGDX),
    E0(0xC6, CF_00, TEXT_XGDY),
};

constexpr uint8_t MC68HC12_IB7[] PROGMEM = {
      3,  // TEXT_EXG
      4,  // TEXT_EXG
      6,  // TEXT_EXG
      7,  // TEXT_EXG
     10,  // TEXT_EXG
      1,  // TEXT_SEX
      9,  // TEXT_SEX
     11,  // TEXT_TAP
      0,  // TEXT_TFR
      2,  // TEXT_TFR
      5,  // TEXT_TFR
      8,  // TEXT_TFR
     12,  // TEXT_TPA
     13,  // TEXT_TSX
     15,  // TEXT_TSY
     14,  // TEXT_TXS
     16,  // TEXT_TYS
     17,  // TEXT_XGDX
     18,  // TEXT_XGDY
};

constexpr Entry MC68HC12_P18[] PROGMEM = {
    E2(0x00, CF_00, TEXT_MOVW, M_IM16, M_NIDX),
    E2(0x01, CF_00, TEXT_MOVW, M_EXT,  M_NIDX),
    E2(0x02, CF_00, TEXT_MOVW, M_NIDX, M_NIDX),
    E2(0x03, CF_00, TEXT_MOVW, M_IM16, M_EXT),
    E2(0x04, CF_00, TEXT_MOVW, M_EXT,  M_EXT),
    E2(0x05, CF_00, TEXT_MOVW, M_NIDX, M_EXT),
    E0(0x06, CF_00, TEXT_ABA),
    E0(0x07, CF_00, TEXT_DAA),
    E2(0x08, CF_00, TEXT_MOVB, M_IM8,  M_NIDX),
    E2(0x09, CF_00, TEXT_MOVB, M_EXT,  M_NIDX),
    E2(0x0A, CF_00, TEXT_MOVB, M_NIDX, M_NIDX),
    E2(0x0B, CF_00, TEXT_MOVB, M_IM8,  M_EXT),
    E2(0x0C, CF_00, TEXT_MOVB, M_EXT,  M_EXT),
    E2(0x0D, CF_00, TEXT_MOVB, M_NIDX, M_EXT),
    E0(0x0E, CF_00, TEXT_TAB),
    E0(0x0F, CF_00, TEXT_TBA),
    E0(0x10, CF_00, TEXT_IDIV),
    E0(0x11, CF_00, TEXT_FDIV),
    E1(0x12, CF_00, TEXT_EMACS, M_EXT),
    E0(0x13, CF_00, TEXT_EMULS),
    E0(0x14, CF_00, TEXT_EDIVS),
    E0(0x15, CF_00, TEXT_IDIVS),
    E0(0x16, CF_00, TEXT_SBA),
    E0(0x17, CF_00, TEXT_CBA),
    E1(0x18, CF_00, TEXT_MAXA,  M_IDX),
    E1(0x19, CF_00, TEXT_MINA,  M_IDX),
    E1(0x1A, CF_00, TEXT_EMAXD, M_IDX),
    E1(0x1B, CF_00, TEXT_EMIND, M_IDX),
    E1(0x1C, CF_00, TEXT_MAXM,  M_IDX),
    E1(0x1D, CF_00, TEXT_MINM,  M_IDX),
    E1(0x1E, CF_00, TEXT_EMAXM, M_IDX),
    E1(0x1F, CF_00, TEXT_EMINM, M_IDX),
    E1(0x20, CF_00, TEXT_LBRA,  M_RL16),
    E1(0x21, CF_00, TEXT_LBRN,  M_RL16),
    E1(0x22, CF_00, TEXT_LBHI,  M_RL16),
    E1(0x23, CF_00, TEXT_LBLS,  M_RL16),
    E1(0x24, CF_00, TEXT_LBCC,  M_RL16),
    E1(0x24, CF_00, TEXT_LBHS,  M_RL16),
    E1(0x25, CF_00, TEXT_LBCS,  M_RL16),
    E1(0x25, CF_00, TEXT_LBLO,  M_RL16),
    E1(0x26, CF_00, TEXT_LBNE,  M_RL16),
    E1(0x27, CF_00, TEXT_LBEQ,  M_RL16),
    E1(0x28, CF_00, TEXT_LBVC,  M_RL16),
    E1(0x29, CF_00, TEXT_LBVS,  M_RL16),
    E1(0x2A, CF_00, TEXT_LBPL,  M_RL16),
    E1(0x2B, CF_00, TEXT_LBMI,  M_RL16),
    E1(0x2C, CF_00, TEXT_LBGE,  M_RL16),
    E1(0x2D, CF_00, TEXT_LBLT,  M_RL16),
    E1(0x2E, CF_00, TEXT_LBGT,  M_RL16),
    E1(0x2F, CF_00, TEXT_LBLE,  M_RL16),
    E0(0x3A, CF_00, TEXT_REV),
    E0(0x3B, CF_00, TEXT_REVW),
    E0(0x3C, CF_00, TEXT_WAV),
    E1(0x3D, CF_00, TEXT_TBL,   M_NIDX),
    E0(0x3E, CF_00, TEXT_STOP),
    E1(0x3F, CF_00, TEXT_ETBL,  M_NIDX),
    E1(0x30, CF_07, TEXT_TRAP,  M_TRAP),
    E1(0x38, CF_00, TEXT_TRAP,  M_TRAP),
    E1(0x39, CF_00, TEXT_TRAP,  M_TRAP),
    E1(0x40, CF_3F, TEXT_TRAP,  M_TRAP),
    E1(0x80, CF_7F, TEXT_TRAP,  M_TRAP),
};

constexpr uint8_t MC68HC12_I18[] PROGMEM = {
      6,  // TEXT_ABA
     23,  // TEXT_CBA
      7,  // TEXT_DAA
     20,  // TEXT_EDIVS
     18,  // TEXT_EMACS
     26,  // TEXT_EMAXD
     30,  // TEXT_EMAXM
     27,  // TEXT_EMIND
     31,  // TEXT_EMINM
     19,  // TEXT_EMULS
     55,  // TEXT_ETBL
     17,  // TEXT_FDIV
     16,  // TEXT_IDIV
     21,  // TEXT_IDIVS
     36,  // TEXT_LBCC
     38,  // TEXT_LBCS
     41,  // TEXT_LBEQ
     46,  // TEXT_LBGE
     48,  // TEXT_LBGT
     34,  // TEXT_LBHI
     37,  // TEXT_LBHS
     49,  // TEXT_LBLE
     39,  // TEXT_LBLO
     35,  // TEXT_LBLS
     47,  // TEXT_LBLT
     45,  // TEXT_LBMI
     40,  // TEXT_LBNE
     44,  // TEXT_LBPL
     32,  // TEXT_LBRA
     33,  // TEXT_LBRN
     42,  // TEXT_LBVC
     43,  // TEXT_LBVS
     24,  // TEXT_MAXA
     28,  // TEXT_MAXM
     25,  // TEXT_MINA
     29,  // TEXT_MINM
      8,  // TEXT_MOVB
      9,  // TEXT_MOVB
     10,  // TEXT_MOVB
     11,  // TEXT_MOVB
     12,  // TEXT_MOVB
     13,  // TEXT_MOVB
      0,  // TEXT_MOVW
      1,  // TEXT_MOVW
      2,  // TEXT_MOVW
      3,  // TEXT_MOVW
      4,  // TEXT_MOVW
      5,  // TEXT_MOVW
     50,  // TEXT_REV
     51,  // TEXT_REVW
     22,  // TEXT_SBA
     54,  // TEXT_STOP
     14,  // TEXT_TAB
     15,  // TEXT_TBA
     53,  // TEXT_TBL
     56,  // TEXT_TRAP
     57,  // TEXT_TRAP
     58,  // TEXT_TRAP
     59,  // TEXT_TRAP
     60,  // TEXT_TRAP
     52,  // TEXT_WAV
};

constexpr Entry MC68HC12_P10[] PROGMEM = {
    E0(0xFE, CF_00, TEXT_CLC),
    E0(0xEF, CF_00, TEXT_CLI),
    E0(0xFD, CF_00, TEXT_CLV),
};

constexpr uint8_t MC68HC12_I10[] PROGMEM = {
      0,  // TEXT_CLC
      1,  // TEXT_CLI
      2,  // TEXT_CLV
};

constexpr Entry MC68HC12_P14[] PROGMEM = {
    E0(0x01, CF_00, TEXT_SEC),
    E0(0x10, CF_00, TEXT_SEI),
    E0(0x02, CF_00, TEXT_SEV),
};

constexpr uint8_t MC68HC12_I14[] PROGMEM = {
      0,  // TEXT_SEC
      1,  // TEXT_SEI
      2,  // TEXT_SEV
};

constexpr Entry MC68HC12_P19[] PROGMEM = {
    E0(0xED, CF_00, TEXT_ABY),
};

constexpr uint8_t MC68HC12_I19[] PROGMEM = {
      0,  // TEXT_ABY
};

constexpr Entry MC68HC12_P1A[] PROGMEM = {
    E0(0xE5, CF_00, TEXT_ABX),
};

constexpr uint8_t MC68HC12_I1A[] PROGMEM = {
      0,  // TEXT_ABX
};

constexpr Entry MC68HC12_P1B[] PROGMEM = {
    E0(0x9F, CF_00, TEXT_DES),
    E0(0x81, CF_00, TEXT_INS),
};

constexpr uint8_t MC68HC12_I1B[] PROGMEM = {
      0,  // TEXT_DES
      1,  // TEXT_INS
};
// clang-format on

using EntryPage = entry::PrefixTableBase<Entry>;

constexpr EntryPage MC68HC12_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(MC68HC12_TABLE), ARRAY_RANGE(MC68HC12_INDEX)},
        {0x18, ARRAY_RANGE(MC68HC12_P18), ARRAY_RANGE(MC68HC12_I18)},
        {0x04, ARRAY_RANGE(MC68HC12_P04), ARRAY_RANGE(MC68HC12_I04)},
        {0x4B, ARRAY_RANGE(MC68HC12_P4B), ARRAY_RANGE(MC68HC12_I4B)},
        {0xB7, ARRAY_RANGE(MC68HC12_PB7), ARRAY_RANGE(MC68HC12_IB7)},
        {0x10, ARRAY_RANGE(MC68HC12_P10), ARRAY_RANGE(MC68HC12_I10)},
        {0x14, ARRAY_RANGE(MC68HC12_P14), ARRAY_RANGE(MC68HC12_I14)},
        {0x19, ARRAY_RANGE(MC68HC12_P19), ARRAY_RANGE(MC68HC12_I19)},
        {0x1A, ARRAY_RANGE(MC68HC12_P1A), ARRAY_RANGE(MC68HC12_I1A)},
        {0x1B, ARRAY_RANGE(MC68HC12_P1B), ARRAY_RANGE(MC68HC12_I1B)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {MC68HC12, TEXT_CPU_68HC12, ARRAY_RANGE(MC68HC12_PAGES)},
};

const Cpu *cpu(CpuType) {
    return &CPU_TABLE[0];
}

bool hasOperand(CpuType cpuType, AsmInsn &insn) {
    cpu(cpuType)->searchName(insn, Cpu::acceptAll<AsmInsn, Entry>);
    return insn.isOK() && insn.mode1() != M_NONE;
}

bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (table == M_GN8 || table == M_GN16)
        return opr == M_EXT || opr == M_DIR || opr == M_IM16 || opr == M_IDX || opr == M_PAIR;
    if (table == M_GMEM)
        return opr == M_EXT || opr == M_IDX || opr == M_PAIR;
    if (table == M_IDX || table == M_DIDX || table == M_NIDX || table == M_CIDX)
        return opr == M_IDX || opr == M_PAIR;
    if (table == M_RL8 || table == M_RL9 || table == M_RL16)
        return opr == M_EXT || opr == M_DIR;
    if (opr == M_EXT)
        return table == M_TRAP;
    if (opr == M_DIR)
        return table == M_EXT || table == M_TRAP || table == M_PAG;
    if (opr == M_IM16)
        return table == M_IM8;
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
    opc &= ~entry->readFlags().mask();
    return opc == entry->readOpCode();
}

Error searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) {
    cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    return insn.getError();
}

bool isPrefix(CpuType cpuType, Config::opcode_t code) {
    if (code == 0x10 || code == 0x14 || code == 0x19 || code == 0x1A || code == 0x1B)
        return false;
    return cpu(cpuType)->isPrefix(code);
}

const /*PROGMEM*/ char *TableMc68HC12::listCpu_P() const {
    return TEXT_CPU_MC68HC12;
}

const /*PROGMEM*/ char *TableMc68HC12::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableMc68HC12::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    auto p = name;
    p.iexpectText_P(TEXT_CPU_MC68HC12, 2);  // ignore prefix MC
    auto t = Cpu::search(p, ARRAY_RANGE(CPU_TABLE));
    if (t) {
        cpuType = t->readCpuType();
        return OK;
    }
    return UNSUPPORTED_CPU;
}

const TableMc68HC12 TABLE;

}  // namespace mc68hc12
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

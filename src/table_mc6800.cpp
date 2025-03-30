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

#include "table_mc6800.h"
#include "entry_mc6800.h"
#include "entry_table.h"
#include "text_mc6800.h"

using namespace libasm::text::mc6800;

namespace libasm {
namespace mc6800 {

#define E3(_opc, _cf, _name, _opr1, _opr2, _opr3) \
    {_opc, Entry::Flags::create(_cf, _opr1, _opr2, _opr3), _name}
#define E2(_opc, _cf, _name, _opr1, _opr2) E3(_opc, _cf, _name, _opr1, _opr2, M_NONE)
#define E1(_opc, _cf, _name, _opr1) E2(_opc, _cf, _name, _opr1, M_NONE)
#define E0(_opc, _cf, _name) E1(_opc, _cf, _name, M_NONE)

// clang-format off
constexpr Entry MC6800_TABLE[] PROGMEM = {
    E0(0x01, CF_00, TEXT_NOP),
    E0(0x06, CF_00, TEXT_TAP),
    E0(0x07, CF_00, TEXT_TPA),
    E0(0x08, CF_00, TEXT_INX),
    E0(0x09, CF_00, TEXT_DEX),
    E0(0x0A, CF_00, TEXT_CLV),
    E0(0x0B, CF_00, TEXT_SEV),
    E0(0x0C, CF_00, TEXT_CLC),
    E0(0x0D, CF_00, TEXT_SEC),
    E0(0x0E, CF_00, TEXT_CLI),
    E0(0x0F, CF_00, TEXT_SEI),
    E0(0x10, CF_00, TEXT_SBA),
    E0(0x11, CF_00, TEXT_CBA),
    E0(0x16, CF_00, TEXT_TAB),
    E0(0x17, CF_00, TEXT_TBA),
    E0(0x19, CF_00, TEXT_DAA),
    E0(0x1B, CF_00, TEXT_ABA),
    E1(0x20, CF_00, TEXT_BRA,  M_REL),
    E1(0x22, CF_00, TEXT_BHI,  M_REL),
    E1(0x23, CF_00, TEXT_BLS,  M_REL),
    E1(0x24, CF_00, TEXT_BHS,  M_REL),
    E1(0x24, CF_00, TEXT_BCC,  M_REL),
    E1(0x25, CF_00, TEXT_BLO,  M_REL),
    E1(0x25, CF_00, TEXT_BCS,  M_REL),
    E1(0x26, CF_00, TEXT_BNE,  M_REL),
    E1(0x27, CF_00, TEXT_BEQ,  M_REL),
    E1(0x28, CF_00, TEXT_BVC,  M_REL),
    E1(0x29, CF_00, TEXT_BVS,  M_REL),
    E1(0x2A, CF_00, TEXT_BPL,  M_REL),
    E1(0x2B, CF_00, TEXT_BMI,  M_REL),
    E1(0x2C, CF_00, TEXT_BGE,  M_REL),
    E1(0x2D, CF_00, TEXT_BLT,  M_REL),
    E1(0x2E, CF_00, TEXT_BGT,  M_REL),
    E1(0x2F, CF_00, TEXT_BLE,  M_REL),
    E0(0x30, CF_00, TEXT_TSX),
    E0(0x31, CF_00, TEXT_INS),
    E0(0x32, CF_00, TEXT_PULA),
    E0(0x33, CF_00, TEXT_PULB),
    E0(0x34, CF_00, TEXT_DES),
    E0(0x35, CF_00, TEXT_TXS),
    E0(0x36, CF_00, TEXT_PSHA),
    E0(0x37, CF_00, TEXT_PSHB),
    E0(0x39, CF_00, TEXT_RTS),
    E0(0x3B, CF_00, TEXT_RTI),
    E0(0x3E, CF_00, TEXT_WAI),
    E0(0x3F, CF_00, TEXT_SWI),
    E0(0x40, CF_00, TEXT_NEGA),
    E0(0x50, CF_00, TEXT_NEGB),
    E0(0x43, CF_00, TEXT_COMA),
    E0(0x53, CF_00, TEXT_COMB),
    E0(0x44, CF_00, TEXT_LSRA),
    E0(0x54, CF_00, TEXT_LSRB),
    E0(0x46, CF_00, TEXT_RORA),
    E0(0x56, CF_00, TEXT_RORB),
    E0(0x47, CF_00, TEXT_ASRA),
    E0(0x57, CF_00, TEXT_ASRB),
    E0(0x48, CF_00, TEXT_ASLA),
    E0(0x58, CF_00, TEXT_ASLB),
    E0(0x48, CF_00, TEXT_LSLA),
    E0(0x58, CF_00, TEXT_LSLB),
    E0(0x49, CF_00, TEXT_ROLA),
    E0(0x59, CF_00, TEXT_ROLB),
    E0(0x4A, CF_00, TEXT_DECA),
    E0(0x5A, CF_00, TEXT_DECB),
    E0(0x4C, CF_00, TEXT_INCA),
    E0(0x5C, CF_00, TEXT_INCB),
    E0(0x4D, CF_00, TEXT_TSTA),
    E0(0x5D, CF_00, TEXT_TSTB),
    E0(0x4F, CF_00, TEXT_CLRA),
    E0(0x5F, CF_00, TEXT_CLRB),
    E1(0x60, CF_10, TEXT_NEG,  M_GMEM),
    E1(0x63, CF_10, TEXT_COM,  M_GMEM),
    E1(0x64, CF_10, TEXT_LSR,  M_GMEM),
    E1(0x66, CF_10, TEXT_ROR,  M_GMEM),
    E1(0x67, CF_10, TEXT_ASR,  M_GMEM),
    E1(0x68, CF_10, TEXT_ASL,  M_GMEM),
    E1(0x68, CF_10, TEXT_LSL,  M_GMEM),
    E1(0x69, CF_10, TEXT_ROL,  M_GMEM),
    E1(0x6A, CF_10, TEXT_DEC,  M_GMEM),
    E1(0x6C, CF_10, TEXT_INC,  M_GMEM),
    E1(0x6D, CF_10, TEXT_TST,  M_GMEM),
    E1(0x6E, CF_10, TEXT_JMP,  M_GMEM),
    E1(0x6F, CF_10, TEXT_CLR,  M_GMEM),
    E1(0x80, CF_30, TEXT_SUBA, M_GN8),
    E1(0xC0, CF_30, TEXT_SUBB, M_GN8),
    E1(0x81, CF_30, TEXT_CMPA, M_GN8),
    E1(0xC1, CF_30, TEXT_CMPB, M_GN8),
    E1(0x82, CF_30, TEXT_SBCA, M_GN8),
    E1(0xC2, CF_30, TEXT_SBCB, M_GN8),
    E1(0x84, CF_30, TEXT_ANDA, M_GN8),
    E1(0xC4, CF_30, TEXT_ANDB, M_GN8),
    E1(0x85, CF_30, TEXT_BITA, M_GN8),
    E1(0xC5, CF_30, TEXT_BITB, M_GN8),
    E1(0x86, CF_30, TEXT_LDAA, M_GN8),
    E1(0xC6, CF_30, TEXT_LDAB, M_GN8),
    E1(0x97, CF_00, TEXT_STAA, M_DIR),
    E1(0xA7, CF_00, TEXT_STAA, M_IDX),
    E1(0xB7, CF_00, TEXT_STAA, M_EXT),
    E1(0xD7, CF_00, TEXT_STAB, M_DIR),
    E1(0xE7, CF_00, TEXT_STAB, M_IDX),
    E1(0xF7, CF_00, TEXT_STAB, M_EXT),
    E1(0x88, CF_30, TEXT_EORA, M_GN8),
    E1(0xC8, CF_30, TEXT_EORB, M_GN8),
    E1(0x89, CF_30, TEXT_ADCA, M_GN8),
    E1(0xC9, CF_30, TEXT_ADCB, M_GN8),
    E1(0x8A, CF_30, TEXT_ORAA, M_GN8),
    E1(0xCA, CF_30, TEXT_ORAB, M_GN8),
    E1(0x8B, CF_30, TEXT_ADDA, M_GN8),
    E1(0xCB, CF_30, TEXT_ADDB, M_GN8),
    E1(0x8C, CF_30, TEXT_CPX,  M_GN16),
    E1(0x8D, CF_00, TEXT_BSR,  M_REL),
    E1(0x8E, CF_30, TEXT_LDS,  M_GN16),
    E1(0x9F, CF_00, TEXT_STS,  M_DIR),
    E1(0xAF, CF_00, TEXT_STS,  M_IDX),
    E1(0xBF, CF_00, TEXT_STS,  M_EXT),
    E1(0xAD, CF_00, TEXT_JSR,  M_IDX),
    E1(0xBD, CF_00, TEXT_JSR,  M_EXT),
    E1(0xCE, CF_30, TEXT_LDX,  M_GN16),
    E1(0xDF, CF_00, TEXT_STX,  M_DIR),
    E1(0xEF, CF_00, TEXT_STX,  M_IDX),
    E1(0xFF, CF_00, TEXT_STX,  M_EXT),
};

constexpr uint8_t MC6800_INDEX[] PROGMEM = {
     16,  // TEXT_ABA
    103,  // TEXT_ADCA
    104,  // TEXT_ADCB
    107,  // TEXT_ADDA
    108,  // TEXT_ADDB
     89,  // TEXT_ANDA
     90,  // TEXT_ANDB
     75,  // TEXT_ASL
     56,  // TEXT_ASLA
     57,  // TEXT_ASLB
     74,  // TEXT_ASR
     54,  // TEXT_ASRA
     55,  // TEXT_ASRB
     21,  // TEXT_BCC
     23,  // TEXT_BCS
     25,  // TEXT_BEQ
     30,  // TEXT_BGE
     32,  // TEXT_BGT
     18,  // TEXT_BHI
     20,  // TEXT_BHS
     91,  // TEXT_BITA
     92,  // TEXT_BITB
     33,  // TEXT_BLE
     22,  // TEXT_BLO
     19,  // TEXT_BLS
     31,  // TEXT_BLT
     29,  // TEXT_BMI
     24,  // TEXT_BNE
     28,  // TEXT_BPL
     17,  // TEXT_BRA
    110,  // TEXT_BSR
     26,  // TEXT_BVC
     27,  // TEXT_BVS
     12,  // TEXT_CBA
      7,  // TEXT_CLC
      9,  // TEXT_CLI
     82,  // TEXT_CLR
     68,  // TEXT_CLRA
     69,  // TEXT_CLRB
      5,  // TEXT_CLV
     85,  // TEXT_CMPA
     86,  // TEXT_CMPB
     71,  // TEXT_COM
     48,  // TEXT_COMA
     49,  // TEXT_COMB
    109,  // TEXT_CPX
     15,  // TEXT_DAA
     78,  // TEXT_DEC
     62,  // TEXT_DECA
     63,  // TEXT_DECB
     38,  // TEXT_DES
      4,  // TEXT_DEX
    101,  // TEXT_EORA
    102,  // TEXT_EORB
     79,  // TEXT_INC
     64,  // TEXT_INCA
     65,  // TEXT_INCB
     35,  // TEXT_INS
      3,  // TEXT_INX
     81,  // TEXT_JMP
    115,  // TEXT_JSR
    116,  // TEXT_JSR
     93,  // TEXT_LDAA
     94,  // TEXT_LDAB
    111,  // TEXT_LDS
    117,  // TEXT_LDX
     76,  // TEXT_LSL
     58,  // TEXT_LSLA
     59,  // TEXT_LSLB
     72,  // TEXT_LSR
     50,  // TEXT_LSRA
     51,  // TEXT_LSRB
     70,  // TEXT_NEG
     46,  // TEXT_NEGA
     47,  // TEXT_NEGB
      0,  // TEXT_NOP
    105,  // TEXT_ORAA
    106,  // TEXT_ORAB
     40,  // TEXT_PSHA
     41,  // TEXT_PSHB
     36,  // TEXT_PULA
     37,  // TEXT_PULB
     77,  // TEXT_ROL
     60,  // TEXT_ROLA
     61,  // TEXT_ROLB
     73,  // TEXT_ROR
     52,  // TEXT_RORA
     53,  // TEXT_RORB
     43,  // TEXT_RTI
     42,  // TEXT_RTS
     11,  // TEXT_SBA
     87,  // TEXT_SBCA
     88,  // TEXT_SBCB
      8,  // TEXT_SEC
     10,  // TEXT_SEI
      6,  // TEXT_SEV
     95,  // TEXT_STAA
     96,  // TEXT_STAA
     97,  // TEXT_STAA
     98,  // TEXT_STAB
     99,  // TEXT_STAB
    100,  // TEXT_STAB
    112,  // TEXT_STS
    113,  // TEXT_STS
    114,  // TEXT_STS
    118,  // TEXT_STX
    119,  // TEXT_STX
    120,  // TEXT_STX
     83,  // TEXT_SUBA
     84,  // TEXT_SUBB
     45,  // TEXT_SWI
     13,  // TEXT_TAB
      1,  // TEXT_TAP
     14,  // TEXT_TBA
      2,  // TEXT_TPA
     80,  // TEXT_TST
     66,  // TEXT_TSTA
     67,  // TEXT_TSTB
     34,  // TEXT_TSX
     39,  // TEXT_TXS
     44,  // TEXT_WAI
};

constexpr Entry MB8861_TABLE[] PROGMEM = {
    E2(0x71, CF_00, TEXT_NIM, M_IM8, M_IDX),
    E2(0x72, CF_00, TEXT_OIM, M_IM8, M_IDX),
    E2(0x75, CF_00, TEXT_XIM, M_IM8, M_IDX),
    E2(0x7B, CF_00, TEXT_TMM, M_IM8, M_IDX),
    E1(0xEC, CF_00, TEXT_ADX, M_IM8),
    E1(0xFC, CF_00, TEXT_ADX, M_EXT),
};

constexpr uint8_t MB8861_INDEX[] PROGMEM = {
      4,  // TEXT_ADX
      5,  // TEXT_ADX
      0,  // TEXT_NIM
      1,  // TEXT_OIM
      3,  // TEXT_TMM
      2,  // TEXT_XIM
};

constexpr Entry MC6801_TABLE[] PROGMEM = {
    E0(0x04, CF_00, TEXT_LSRD),
    E0(0x05, CF_00, TEXT_ASLD),
    E0(0x05, CF_00, TEXT_LSLD),
    E1(0x21, CF_00, TEXT_BRN,  M_REL),
    E0(0x38, CF_00, TEXT_PULX),
    E0(0x3A, CF_00, TEXT_ABX),
    E0(0x3C, CF_00, TEXT_PSHX),
    E0(0x3D, CF_00, TEXT_MUL),
    E1(0x83, CF_30, TEXT_SUBD, M_GN16),
    E1(0x9D, CF_00, TEXT_JSR,  M_DIR),
    E1(0xC3, CF_30, TEXT_ADDD, M_GN16),
    E1(0xCC, CF_30, TEXT_LDD,  M_GN16),
    E1(0xDD, CF_00, TEXT_STD,  M_DIR),
    E1(0xED, CF_00, TEXT_STD,  M_IDX),
    E1(0xFD, CF_00, TEXT_STD,  M_EXT),
};

constexpr uint8_t MC6801_INDEX[] PROGMEM = {
      5,  // TEXT_ABX
     10,  // TEXT_ADDD
      1,  // TEXT_ASLD
      3,  // TEXT_BRN
      9,  // TEXT_JSR
     11,  // TEXT_LDD
      2,  // TEXT_LSLD
      0,  // TEXT_LSRD
      7,  // TEXT_MUL
      6,  // TEXT_PSHX
      4,  // TEXT_PULX
     12,  // TEXT_STD
     13,  // TEXT_STD
     14,  // TEXT_STD
      8,  // TEXT_SUBD
};

constexpr Entry HD6301_TABLE[] PROGMEM = {
    E0(0x18, CF_00, TEXT_XGDX),
    E0(0x1A, CF_00, TEXT_SLP),
    E2(0x61, CF_00, TEXT_AIM,  M_BMM, M_IDX),
    E2(0x61, CF_00, TEXT_BCLR, M_BIT, M_IDX),
    E2(0x62, CF_00, TEXT_OIM,  M_BMM, M_IDX),
    E2(0x62, CF_00, TEXT_BSET, M_BIT, M_IDX),
    E2(0x65, CF_00, TEXT_EIM,  M_BMM, M_IDX),
    E2(0x65, CF_00, TEXT_BTGL, M_BIT, M_IDX),
    E2(0x6B, CF_00, TEXT_TIM,  M_BMM, M_IDX),
    E2(0x6B, CF_00, TEXT_BTST, M_BIT, M_IDX),
    E2(0x71, CF_00, TEXT_AIM,  M_BMM, M_DIR),
    E2(0x71, CF_00, TEXT_BCLR, M_BIT, M_DIR),
    E2(0x72, CF_00, TEXT_OIM,  M_BMM, M_DIR),
    E2(0x72, CF_00, TEXT_BSET, M_BIT, M_DIR),
    E2(0x75, CF_00, TEXT_EIM,  M_BMM, M_DIR),
    E2(0x75, CF_00, TEXT_BTGL, M_BIT, M_DIR),
    E2(0x7B, CF_00, TEXT_TIM,  M_BMM, M_DIR),
    E2(0x7B, CF_00, TEXT_BTST, M_BIT, M_DIR),
};

constexpr uint8_t HD6301_INDEX[] PROGMEM = {
      2,  // TEXT_AIM
     10,  // TEXT_AIM
      3,  // TEXT_BCLR
     11,  // TEXT_BCLR
      5,  // TEXT_BSET
     13,  // TEXT_BSET
      7,  // TEXT_BTGL
     15,  // TEXT_BTGL
      9,  // TEXT_BTST
     17,  // TEXT_BTST
      6,  // TEXT_EIM
     14,  // TEXT_EIM
      4,  // TEXT_OIM
     12,  // TEXT_OIM
      1,  // TEXT_SLP
      8,  // TEXT_TIM
     16,  // TEXT_TIM
      0,  // TEXT_XGDX
};

constexpr Entry MC68HC11_P00[] PROGMEM = {
    E0(0x02, CF_00, TEXT_IDIV),
    E0(0x03, CF_00, TEXT_FDIV),
    E3(0x12, CF_00, TEXT_BRSET, M_DIR, M_IM8, M_REL),
    E3(0x13, CF_00, TEXT_BRCLR, M_DIR, M_IM8, M_REL),
    E2(0x14, CF_00, TEXT_BSET,  M_DIR, M_IM8),
    E2(0x15, CF_00, TEXT_BCLR,  M_DIR, M_IM8),
    E2(0x1C, CF_00, TEXT_BSET,  M_IDX, M_IM8),
    E2(0x1D, CF_00, TEXT_BCLR,  M_IDX, M_IM8),
    E3(0x1E, CF_00, TEXT_BRSET, M_IDX, M_IM8, M_REL),
    E3(0x1F, CF_00, TEXT_BRCLR, M_IDX, M_IM8, M_REL),
    E0(0x8F, CF_00, TEXT_XGDX),
    E0(0xCF, CF_00, TEXT_STOP),
};

constexpr uint8_t MC68HC11_I00[] PROGMEM = {
      5,  // TEXT_BCLR
      7,  // TEXT_BCLR
      3,  // TEXT_BRCLR
      9,  // TEXT_BRCLR
      2,  // TEXT_BRSET
      8,  // TEXT_BRSET
      4,  // TEXT_BSET
      6,  // TEXT_BSET
      1,  // TEXT_FDIV
      0,  // TEXT_IDIV
     11,  // TEXT_STOP
     10,  // TEXT_XGDX
};

constexpr Entry MC68HC11_P18[] PROGMEM = {
    E0(0x08, CF_00, TEXT_INY),
    E0(0x09, CF_00, TEXT_DEY),
    E2(0x1C, CF_00, TEXT_BSET,  M_IDY, M_IM8),
    E2(0x1D, CF_00, TEXT_BCLR,  M_IDY, M_IM8),
    E3(0x1E, CF_00, TEXT_BRSET, M_IDY, M_IM8, M_REL),
    E3(0x1F, CF_00, TEXT_BRCLR, M_IDY, M_IM8, M_REL),
    E0(0x30, CF_00, TEXT_TSY),
    E0(0x35, CF_00, TEXT_TYS),
    E0(0x38, CF_00, TEXT_PULY),
    E0(0x3A, CF_00, TEXT_ABY),
    E0(0x3C, CF_00, TEXT_PSHY),
    E1(0x60, CF_00, TEXT_NEG,   M_IDY),
    E1(0x63, CF_00, TEXT_COM,   M_IDY),
    E1(0x64, CF_00, TEXT_LSR,   M_IDY),
    E1(0x66, CF_00, TEXT_ROR,   M_IDY),
    E1(0x67, CF_00, TEXT_ASR,   M_IDY),
    E1(0x68, CF_00, TEXT_ASL,   M_IDY),
    E1(0x68, CF_00, TEXT_LSL,   M_IDY),
    E1(0x69, CF_00, TEXT_ROL,   M_IDY),
    E1(0x6A, CF_00, TEXT_DEC,   M_IDY),
    E1(0x6C, CF_00, TEXT_INC,   M_IDY),
    E1(0x6D, CF_00, TEXT_TST,   M_IDY),
    E1(0x6E, CF_00, TEXT_JMP,   M_IDY),
    E1(0x6F, CF_00, TEXT_CLR,   M_IDY),
    E1(0x8C, CF_00, TEXT_CPY,   M_IM16),
    E0(0x8F, CF_00, TEXT_XGDY),
    E1(0x9C, CF_00, TEXT_CPY,   M_DIR),
    E1(0xA0, CF_00, TEXT_SUBA,  M_IDY),
    E1(0xE0, CF_00, TEXT_SUBB,  M_IDY),
    E1(0xA1, CF_00, TEXT_CMPA,  M_IDY),
    E1(0xE1, CF_00, TEXT_CMPB,  M_IDY),
    E1(0xA2, CF_00, TEXT_SBCA,  M_IDY),
    E1(0xE2, CF_00, TEXT_SBCB,  M_IDY),
    E1(0xA3, CF_00, TEXT_SUBD,  M_IDY),
    E1(0xE3, CF_00, TEXT_ADDD,  M_IDY),
    E1(0xA4, CF_00, TEXT_ANDA,  M_IDY),
    E1(0xE4, CF_00, TEXT_ANDB,  M_IDY),
    E1(0xA5, CF_00, TEXT_BITA,  M_IDY),
    E1(0xE5, CF_00, TEXT_BITB,  M_IDY),
    E1(0xA6, CF_00, TEXT_LDAA,  M_IDY),
    E1(0xE6, CF_00, TEXT_LDAB,  M_IDY),
    E1(0xA7, CF_00, TEXT_STAA,  M_IDY),
    E1(0xE7, CF_00, TEXT_STAB,  M_IDY),
    E1(0xA8, CF_00, TEXT_EORA,  M_IDY),
    E1(0xE8, CF_00, TEXT_EORB,  M_IDY),
    E1(0xA9, CF_00, TEXT_ADCA,  M_IDY),
    E1(0xE9, CF_00, TEXT_ADCB,  M_IDY),
    E1(0xAA, CF_00, TEXT_ORAA,  M_IDY),
    E1(0xEA, CF_00, TEXT_ORAB,  M_IDY),
    E1(0xAB, CF_00, TEXT_ADDA,  M_IDY),
    E1(0xEB, CF_00, TEXT_ADDB,  M_IDY),
    E1(0xAC, CF_00, TEXT_CPY,   M_IDY),
    E1(0xAD, CF_00, TEXT_JSR,   M_IDY),
    E1(0xAE, CF_00, TEXT_LDS,   M_IDY),
    E1(0xAF, CF_00, TEXT_STS,   M_IDY),
    E1(0xBC, CF_00, TEXT_CPY,   M_EXT),
    E1(0xCE, CF_00, TEXT_LDY,   M_IM16),
    E1(0xDE, CF_00, TEXT_LDY,   M_DIR),
    E1(0xDF, CF_00, TEXT_STY,   M_DIR),
    E1(0xEC, CF_00, TEXT_LDD,   M_IDY),
    E1(0xED, CF_00, TEXT_STD,   M_IDY),
    E1(0xEE, CF_00, TEXT_LDY,   M_IDY),
    E1(0xEF, CF_00, TEXT_STY,   M_IDY),
    E1(0xFE, CF_00, TEXT_LDY,   M_EXT),
    E1(0xFF, CF_00, TEXT_STY,   M_EXT),
};

constexpr uint8_t MC68HC11_I18[] PROGMEM = {
      9,  // TEXT_ABY
     45,  // TEXT_ADCA
     46,  // TEXT_ADCB
     49,  // TEXT_ADDA
     50,  // TEXT_ADDB
     34,  // TEXT_ADDD
     35,  // TEXT_ANDA
     36,  // TEXT_ANDB
     16,  // TEXT_ASL
     15,  // TEXT_ASR
      3,  // TEXT_BCLR
     37,  // TEXT_BITA
     38,  // TEXT_BITB
      5,  // TEXT_BRCLR
      4,  // TEXT_BRSET
      2,  // TEXT_BSET
     23,  // TEXT_CLR
     29,  // TEXT_CMPA
     30,  // TEXT_CMPB
     12,  // TEXT_COM
     24,  // TEXT_CPY
     26,  // TEXT_CPY
     51,  // TEXT_CPY
     55,  // TEXT_CPY
     19,  // TEXT_DEC
      1,  // TEXT_DEY
     43,  // TEXT_EORA
     44,  // TEXT_EORB
     20,  // TEXT_INC
      0,  // TEXT_INY
     22,  // TEXT_JMP
     52,  // TEXT_JSR
     39,  // TEXT_LDAA
     40,  // TEXT_LDAB
     59,  // TEXT_LDD
     53,  // TEXT_LDS
     56,  // TEXT_LDY
     57,  // TEXT_LDY
     61,  // TEXT_LDY
     63,  // TEXT_LDY
     17,  // TEXT_LSL
     13,  // TEXT_LSR
     11,  // TEXT_NEG
     47,  // TEXT_ORAA
     48,  // TEXT_ORAB
     10,  // TEXT_PSHY
      8,  // TEXT_PULY
     18,  // TEXT_ROL
     14,  // TEXT_ROR
     31,  // TEXT_SBCA
     32,  // TEXT_SBCB
     41,  // TEXT_STAA
     42,  // TEXT_STAB
     60,  // TEXT_STD
     54,  // TEXT_STS
     58,  // TEXT_STY
     62,  // TEXT_STY
     64,  // TEXT_STY
     27,  // TEXT_SUBA
     28,  // TEXT_SUBB
     33,  // TEXT_SUBD
     21,  // TEXT_TST
      6,  // TEXT_TSY
      7,  // TEXT_TYS
     25,  // TEXT_XGDY
};

constexpr Entry MC68HC11_P1A[] PROGMEM = {
    E1(0x83, CF_30, TEXT_CPD,   M_GN16),
    E1(0xAC, CF_00, TEXT_CPY,   M_IDX),
    E1(0xEE, CF_00, TEXT_LDY,   M_IDX),
    E1(0xEF, CF_00, TEXT_STY,   M_IDX),
};

constexpr uint8_t MC68HC11_I1A[] PROGMEM = {
      0,  // TEXT_CPD
      1,  // TEXT_CPY
      2,  // TEXT_LDY
      3,  // TEXT_STY
};

constexpr Entry MC68HC11_PCD[] PROGMEM = {
    E1(0xA3, CF_00, TEXT_CPD,   M_IDY),
    E1(0xAC, CF_00, TEXT_CPX,   M_IDY),
    E1(0xEE, CF_00, TEXT_LDX,   M_IDY),
    E1(0xEF, CF_00, TEXT_STX,   M_IDY),
};

constexpr uint8_t MC68HC11_ICD[] PROGMEM = {
      0,  // TEXT_CPD
      1,  // TEXT_CPX
      2,  // TEXT_LDX
      3,  // TEXT_STX
};
// clang-format on

using EntryPage = entry::PrefixTableBase<Entry>;

constexpr EntryPage MC6800_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(MC6800_TABLE), ARRAY_RANGE(MC6800_INDEX)},
};

constexpr EntryPage MB8861_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(MC6800_TABLE), ARRAY_RANGE(MC6800_INDEX)},
        {0x00, ARRAY_RANGE(MB8861_TABLE), ARRAY_RANGE(MB8861_INDEX)},
};

constexpr EntryPage MC6801_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(MC6801_TABLE), ARRAY_RANGE(MC6801_INDEX)},
        {0x00, ARRAY_RANGE(MC6800_TABLE), ARRAY_RANGE(MC6800_INDEX)},
};

constexpr EntryPage HD6301_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(HD6301_TABLE), ARRAY_RANGE(HD6301_INDEX)},
        {0x00, ARRAY_RANGE(MC6801_TABLE), ARRAY_RANGE(MC6801_INDEX)},
        {0x00, ARRAY_RANGE(MC6800_TABLE), ARRAY_RANGE(MC6800_INDEX)},
};

constexpr EntryPage MC68HC11_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(MC68HC11_P00), ARRAY_RANGE(MC68HC11_I00)},
        {0x00, ARRAY_RANGE(MC6801_TABLE), ARRAY_RANGE(MC6801_INDEX)},
        {0x00, ARRAY_RANGE(MC6800_TABLE), ARRAY_RANGE(MC6800_INDEX)},
        {0x18, ARRAY_RANGE(MC68HC11_P18), ARRAY_RANGE(MC68HC11_I18)},
        {0x1A, ARRAY_RANGE(MC68HC11_P1A), ARRAY_RANGE(MC68HC11_I1A)},
        {0xCD, ARRAY_RANGE(MC68HC11_PCD), ARRAY_RANGE(MC68HC11_ICD)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {MC6800, TEXT_CPU_6800, ARRAY_RANGE(MC6800_PAGES)},
        {MB8861, TEXT_CPU_MB8861, ARRAY_RANGE(MB8861_PAGES)},
        {MC6801, TEXT_CPU_6801, ARRAY_RANGE(MC6801_PAGES)},
        {HD6301, TEXT_CPU_6301, ARRAY_RANGE(HD6301_PAGES)},
        {HD6301, TEXT_CPU_HD6301, ARRAY_RANGE(HD6301_PAGES)},
        {MC68HC11, TEXT_CPU_6811, ARRAY_RANGE(MC68HC11_PAGES)},
        {MC68HC11, TEXT_CPU_68HC11, ARRAY_RANGE(MC68HC11_PAGES)},
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
    if (table == M_GN8 || table == M_GN16)
        return opr == M_EXT || opr == M_DIR || opr == M_BIT || opr == M_IM16 || opr == M_IDX;
    if (table == M_GMEM)
        return opr == M_EXT || opr == M_DIR || opr == M_BIT || opr == M_IDX;
    if (opr == M_EXT)
        return table == M_REL;
    if (opr == M_DIR)
        return table == M_REL || table == M_EXT;
    if (opr == M_BIT)
        return table == M_REL || table == M_EXT || table == M_DIR;
    if (opr == M_IM16)
        return table == M_IM8 || table == M_BMM;
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

const Entry *searchOpCodeImpl(const Cpu *cpu, DisInsn &insn, StrBuffer &out) {
    return cpu->searchOpCode(insn, out, matchOpCode);
}

Error searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) {
    searchOpCodeImpl(cpu(cpuType), insn, out);
    return insn.getError();
}

Error searchOpCodeAlias(CpuType cpuType, DisInsn &insn, StrBuffer &out) {
    auto entry = searchOpCodeImpl(cpu(cpuType), insn, out);
    if (entry == nullptr)
        return insn.setErrorIf(INTERNAL_ERROR);
    entry += 1;
    if (entry->readOpCode() != insn.opCode())
        return insn.setError(INTERNAL_ERROR);
    Cpu::defaultReadName(insn, entry, out);
    return OK;
}

bool isPrefix(CpuType cpuType, Config::opcode_t code) {
    return cpu(cpuType)->isPrefix(code);
}

const /*PROGMEM*/ char *TableMc6800::listCpu_P() const {
    return TEXT_MC6800_LIST;
}

const /*PROGMEM*/ char *TableMc6800::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableMc6800::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    auto p = name;
    auto t = Cpu::search(p, ARRAY_RANGE(CPU_TABLE));
    if (t == nullptr && p.iexpectText_P(TEXT_MC6800_LIST, 2))
        t = Cpu::search(p, ARRAY_RANGE(CPU_TABLE));
    if (t) {
        cpuType = t->readCpuType();
    } else if (p.iequals_P(TEXT_CPU_68HC11)) {
        cpuType = MC68HC11;
    } else if (name.iequals_P(TEXT_CPU_HD6301)) {
        cpuType = HD6301;
    } else {
        return UNSUPPORTED_CPU;
    }
    return OK;
}

const TableMc6800 TABLE;

}  // namespace mc6800
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

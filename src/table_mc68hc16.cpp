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

#include "table_mc68hc16.h"
#include "entry_mc68hc16.h"
#include "entry_table.h"
#include "text_mc68hc16.h"

using namespace libasm::text::mc68hc16;

namespace libasm {
namespace mc68hc16 {

#define E3(_opc, _cf, _name, _opr1, _opr2, _opr3) \
    {_opc, Entry::Flags::create(_cf, _opr1, _opr2, _opr3), _name}
#define E2(_opc, _cf, _name, _opr1, _opr2) E3(_opc, _cf, _name, _opr1, _opr2, M_NONE)
#define E1(_opc, _cf, _name, _opr1) E2(_opc, _cf, _name, _opr1, M_NONE)
#define E0(_opc, _cf, _name) E1(_opc, _cf, _name, M_NONE)
#define P0(_opc, _post, _name) {_opc, Entry::Flags::postbyte(_post), _name}

// clang-format off
constexpr Entry MC68HC16_TABLE[] PROGMEM = {
    E1(0x00, CF_30, TEXT_COM,   M_IX8),
    E1(0x01, CF_30, TEXT_DEC,   M_IX8),
    E1(0x02, CF_30, TEXT_NEG,   M_IX8),
    E1(0x03, CF_30, TEXT_INC,   M_IX8),
    E1(0x04, CF_30, TEXT_ASL,   M_IX8),
    E1(0x04, CF_30, TEXT_LSL,   M_IX8),
    E1(0x05, CF_30, TEXT_CLR,   M_IX8),
    E1(0x06, CF_30, TEXT_TST,   M_IX8),
    E1(0x0C, CF_30, TEXT_ROL,   M_IX8),
    E1(0x0D, CF_30, TEXT_ASR,   M_IX8),
    E1(0x0E, CF_30, TEXT_ROR,   M_IX8),
    E1(0x0F, CF_30, TEXT_LSR,   M_IX8),
    E2(0x30, CF_00, TEXT_MOVB,  M_IXX8, M_EX16),
    E2(0x31, CF_00, TEXT_MOVW,  M_IXX8, M_EX16),
    E2(0x32, CF_00, TEXT_MOVB,  M_EX16, M_IXX8),
    E2(0x33, CF_00, TEXT_MOVW,  M_EX16, M_IXX8),
    E1(0x34, CF_00, TEXT_PSHM,  M_LIST),
    E1(0x34, CF_00, TEXT_PSHM,  M_IXE),          // E,ix
    E2(0x34, CF_00, TEXT_PSHM,  M_IXE,  M_LIST), // E,ix,...
    E1(0x35, CF_00, TEXT_PULM,  M_LIST),
    E1(0x35, CF_00, TEXT_PULM,  M_IXE),          // E,ix
    E2(0x35, CF_00, TEXT_PULM,  M_IXE,  M_LIST), // Emix,...
    E1(0x36, CF_00, TEXT_BSR,   M_RL8),
    E2(0x38, CF_00, TEXT_BCLR,  M_EX16, M_IM8),
    E2(0x39, CF_00, TEXT_BSET,  M_EX16, M_IM8),
    E3(0x3A, CF_00, TEXT_BRCLR, M_EX16, M_IM8, M_RL16),
    E3(0x3B, CF_00, TEXT_BRSET, M_EX16, M_IM8, M_RL16),
    E1(0x3C, CF_00, TEXT_AIX,   M_IM8),
    E1(0x3D, CF_00, TEXT_AIY,   M_IM8),
    E1(0x3E, CF_00, TEXT_AIZ,   M_IM8),
    E1(0x3F, CF_00, TEXT_AIS,   M_IM8),
    E1(0x40, CF_30, TEXT_SUBA,  M_IX8),
    E1(0x41, CF_30, TEXT_ADDA,  M_IX8),
    E1(0x42, CF_30, TEXT_SBCA,  M_IX8),
    E1(0x43, CF_30, TEXT_ADCA,  M_IX8),
    E1(0x44, CF_30, TEXT_EORA,  M_IX8),
    E1(0x45, CF_30, TEXT_LDAA,  M_IX8),
    E1(0x46, CF_30, TEXT_ANDA,  M_IX8),
    E1(0x47, CF_30, TEXT_ORAA,  M_IX8),
    E1(0x48, CF_30, TEXT_CMPA,  M_IX8),
    E1(0x49, CF_30, TEXT_BITA,  M_IX8),
    E1(0x4A, CF_30, TEXT_STAA,  M_IX8),
    E1(0x4B, CF_30, TEXT_JMP,   M_IX20),
    E1(0x4C, CF_30, TEXT_CPX,   M_IX8),
    E1(0x4D, CF_30, TEXT_CPY,   M_IX8),
    E1(0x4E, CF_30, TEXT_CPZ,   M_IX8),
    E1(0x4F, CF_30, TEXT_CPS,   M_IX8),
    E1(0x70, CF_00, TEXT_SUBA,  M_IM8),
    E1(0x71, CF_00, TEXT_ADDA,  M_IM8),
    E1(0x72, CF_00, TEXT_SBCA,  M_IM8),
    E1(0x73, CF_00, TEXT_ADCA,  M_IM8),
    E1(0x74, CF_00, TEXT_EORA,  M_IM8),
    E1(0x75, CF_00, TEXT_LDAA,  M_IM8),
    E1(0x76, CF_00, TEXT_ANDA,  M_IM8),
    E1(0x77, CF_00, TEXT_ORAA,  M_IM8),
    E1(0x78, CF_00, TEXT_CMPA,  M_IM8),
    E1(0x79, CF_00, TEXT_BITA,  M_IM8),
    E1(0x7A, CF_00, TEXT_JMP,   M_EX20),
    E2(0x7B, CF_00, TEXT_MAC,   M_IM4H, M_IM4L),
    E1(0x7C, CF_00, TEXT_ADDE,  M_IM8),
    E1(0x80, CF_30, TEXT_SUBD,  M_IX8),
    E1(0x81, CF_30, TEXT_ADDD,  M_IX8),
    E1(0x82, CF_30, TEXT_SBCD,  M_IX8),
    E1(0x83, CF_30, TEXT_ADCD,  M_IX8),
    E1(0x84, CF_30, TEXT_EORD,  M_IX8),
    E1(0x85, CF_30, TEXT_LDD,   M_IX8),
    E1(0x86, CF_30, TEXT_ANDD,  M_IX8),
    E1(0x87, CF_30, TEXT_ORD,   M_IX8),
    E1(0x88, CF_30, TEXT_CPD,   M_IX8),
    E1(0x89, CF_30, TEXT_JSR,   M_IX20),
    E1(0x8A, CF_30, TEXT_STD,   M_IX8),
    E3(0x8B, CF_30, TEXT_BRSET, M_IX8, M_IM8, M_RL8),
    E1(0x8C, CF_30, TEXT_STX,   M_IX8),
    E1(0x8D, CF_30, TEXT_STY,   M_IX8),
    E1(0x8E, CF_30, TEXT_STZ,   M_IX8),
    E1(0x8F, CF_30, TEXT_STS,   M_IX8),
    E1(0xB0, CF_00, TEXT_BRA,   M_RL8),
    E1(0xB1, CF_00, TEXT_BRN,   M_RL8),
    E1(0xB2, CF_00, TEXT_BHI,   M_RL8),
    E1(0xB3, CF_00, TEXT_BLS,   M_RL8),
    E1(0xB4, CF_00, TEXT_BCC,   M_RL8),
    E1(0xB5, CF_00, TEXT_BCS,   M_RL8),
    E1(0xB4, CF_00, TEXT_BHS,   M_RL8),
    E1(0xB5, CF_00, TEXT_BLO,   M_RL8),
    E1(0xB6, CF_00, TEXT_BNE,   M_RL8),
    E1(0xB7, CF_00, TEXT_BEQ,   M_RL8),
    E1(0xB8, CF_00, TEXT_BVC,   M_RL8),
    E1(0xB9, CF_00, TEXT_BVS,   M_RL8),
    E1(0xBA, CF_00, TEXT_BPL,   M_RL8),
    E1(0xBB, CF_00, TEXT_BMI,   M_RL8),
    E1(0xBC, CF_00, TEXT_BGE,   M_RL8),
    E1(0xBD, CF_00, TEXT_BLT,   M_RL8),
    E1(0xBE, CF_00, TEXT_BGT,   M_RL8),
    E1(0xBF, CF_00, TEXT_BLE,   M_RL8),
    E1(0xC0, CF_30, TEXT_SUBB,  M_IX8),
    E1(0xC1, CF_30, TEXT_ADDB,  M_IX8),
    E1(0xC2, CF_30, TEXT_SBCB,  M_IX8),
    E1(0xC3, CF_30, TEXT_ADCB,  M_IX8),
    E1(0xC4, CF_30, TEXT_EORB,  M_IX8),
    E1(0xC5, CF_30, TEXT_LDAB,  M_IX8),
    E1(0xC6, CF_30, TEXT_ANDB,  M_IX8),
    E1(0xC7, CF_30, TEXT_ORAB,  M_IX8),
    E1(0xC8, CF_30, TEXT_CMPB,  M_IX8),
    E1(0xC9, CF_30, TEXT_BITB,  M_IX8),
    E1(0xCA, CF_30, TEXT_STAB,  M_IX8),
    E3(0xCB, CF_30, TEXT_BRCLR, M_IX8, M_IM8, M_RL8),
    E1(0xCC, CF_30, TEXT_LDX,   M_IX8),
    E1(0xCD, CF_30, TEXT_LDY,   M_IX8),
    E1(0xCE, CF_30, TEXT_LDZ,   M_IX8),
    E1(0xCF, CF_30, TEXT_LDS,   M_IX8),
    E1(0xF0, CF_00, TEXT_SUBB,  M_IM8),
    E1(0xF1, CF_00, TEXT_ADDB,  M_IM8),
    E1(0xF2, CF_00, TEXT_SBCB,  M_IM8),
    E1(0xF3, CF_00, TEXT_ADCB,  M_IM8),
    E1(0xF4, CF_00, TEXT_EORB,  M_IM8),
    E1(0xF5, CF_00, TEXT_LDAB,  M_IM8),
    E1(0xF6, CF_00, TEXT_ANDB,  M_IM8),
    E1(0xF7, CF_00, TEXT_ORAB,  M_IM8),
    E1(0xF8, CF_00, TEXT_CMPB,  M_IM8),
    E1(0xF9, CF_00, TEXT_BITB,  M_IM8),
    E1(0xFA, CF_00, TEXT_JSR,   M_EX20),
    E2(0xFB, CF_00, TEXT_RMAC,  M_IM4H, M_IM4L),
    E1(0xFC, CF_00, TEXT_ADDD,  M_IM8),
};

constexpr uint8_t MC68HC16_INDEX[] PROGMEM = {
     34,  // TEXT_ADCA
     50,  // TEXT_ADCA
     97,  // TEXT_ADCB
    113,  // TEXT_ADCB
     63,  // TEXT_ADCD
     32,  // TEXT_ADDA
     48,  // TEXT_ADDA
     95,  // TEXT_ADDB
    111,  // TEXT_ADDB
     61,  // TEXT_ADDD
    122,  // TEXT_ADDD
     59,  // TEXT_ADDE
     30,  // TEXT_AIS
     27,  // TEXT_AIX
     28,  // TEXT_AIY
     29,  // TEXT_AIZ
     37,  // TEXT_ANDA
     53,  // TEXT_ANDA
    100,  // TEXT_ANDB
    116,  // TEXT_ANDB
     66,  // TEXT_ANDD
      4,  // TEXT_ASL
      9,  // TEXT_ASR
     80,  // TEXT_BCC
     23,  // TEXT_BCLR
     81,  // TEXT_BCS
     85,  // TEXT_BEQ
     90,  // TEXT_BGE
     92,  // TEXT_BGT
     78,  // TEXT_BHI
     82,  // TEXT_BHS
     40,  // TEXT_BITA
     56,  // TEXT_BITA
    103,  // TEXT_BITB
    119,  // TEXT_BITB
     93,  // TEXT_BLE
     83,  // TEXT_BLO
     79,  // TEXT_BLS
     91,  // TEXT_BLT
     89,  // TEXT_BMI
     84,  // TEXT_BNE
     88,  // TEXT_BPL
     76,  // TEXT_BRA
     25,  // TEXT_BRCLR
    105,  // TEXT_BRCLR
     77,  // TEXT_BRN
     26,  // TEXT_BRSET
     71,  // TEXT_BRSET
     24,  // TEXT_BSET
     22,  // TEXT_BSR
     86,  // TEXT_BVC
     87,  // TEXT_BVS
      6,  // TEXT_CLR
     39,  // TEXT_CMPA
     55,  // TEXT_CMPA
    102,  // TEXT_CMPB
    118,  // TEXT_CMPB
      0,  // TEXT_COM
     68,  // TEXT_CPD
     46,  // TEXT_CPS
     43,  // TEXT_CPX
     44,  // TEXT_CPY
     45,  // TEXT_CPZ
      1,  // TEXT_DEC
     35,  // TEXT_EORA
     51,  // TEXT_EORA
     98,  // TEXT_EORB
    114,  // TEXT_EORB
     64,  // TEXT_EORD
      3,  // TEXT_INC
     42,  // TEXT_JMP
     57,  // TEXT_JMP
     69,  // TEXT_JSR
    120,  // TEXT_JSR
     36,  // TEXT_LDAA
     52,  // TEXT_LDAA
     99,  // TEXT_LDAB
    115,  // TEXT_LDAB
     65,  // TEXT_LDD
    109,  // TEXT_LDS
    106,  // TEXT_LDX
    107,  // TEXT_LDY
    108,  // TEXT_LDZ
      5,  // TEXT_LSL
     11,  // TEXT_LSR
     58,  // TEXT_MAC
     12,  // TEXT_MOVB
     14,  // TEXT_MOVB
     13,  // TEXT_MOVW
     15,  // TEXT_MOVW
      2,  // TEXT_NEG
     38,  // TEXT_ORAA
     54,  // TEXT_ORAA
    101,  // TEXT_ORAB
    117,  // TEXT_ORAB
     67,  // TEXT_ORD
     16,  // TEXT_PSHM
     17,  // TEXT_PSHM
     18,  // TEXT_PSHM
     19,  // TEXT_PULM
     20,  // TEXT_PULM
     21,  // TEXT_PULM
    121,  // TEXT_RMAC
      8,  // TEXT_ROL
     10,  // TEXT_ROR
     33,  // TEXT_SBCA
     49,  // TEXT_SBCA
     96,  // TEXT_SBCB
    112,  // TEXT_SBCB
     62,  // TEXT_SBCD
     41,  // TEXT_STAA
    104,  // TEXT_STAB
     70,  // TEXT_STD
     75,  // TEXT_STS
     72,  // TEXT_STX
     73,  // TEXT_STY
     74,  // TEXT_STZ
     31,  // TEXT_SUBA
     47,  // TEXT_SUBA
     94,  // TEXT_SUBB
    110,  // TEXT_SUBB
     60,  // TEXT_SUBD
      7,  // TEXT_TST
};

constexpr Entry MC68HC16_P17[] PROGMEM = {
    E1(0x00, CF_30, TEXT_COM,   M_IX16),
    E1(0x01, CF_30, TEXT_DEC,   M_IX16),
    E1(0x02, CF_30, TEXT_NEG,   M_IX16),
    E1(0x03, CF_30, TEXT_INC,   M_IX16),
    E1(0x04, CF_30, TEXT_ASL,   M_IX16),
    E1(0x04, CF_30, TEXT_LSL,   M_IX16),
    E1(0x05, CF_30, TEXT_CLR,   M_IX16),
    E1(0x06, CF_30, TEXT_TST,   M_IX16),
    E2(0x08, CF_30, TEXT_BCLR,  M_IX8, M_IM8),
    E2(0x09, CF_30, TEXT_BSET,  M_IX8, M_IM8),
    E1(0x0C, CF_30, TEXT_ROL,   M_IX16),
    E1(0x0D, CF_30, TEXT_ASR,   M_IX16),
    E1(0x0E, CF_30, TEXT_ROR,   M_IX16),
    E1(0x0F, CF_30, TEXT_LSR,   M_IX16),
    E1(0x30, CF_00, TEXT_COM,   M_EX16),
    E1(0x31, CF_00, TEXT_DEC,   M_EX16),
    E1(0x32, CF_00, TEXT_NEG,   M_EX16),
    E1(0x33, CF_00, TEXT_INC,   M_EX16),
    E1(0x34, CF_00, TEXT_ASL,   M_EX16),
    E1(0x34, CF_00, TEXT_LSL,   M_EX16),
    E1(0x35, CF_00, TEXT_CLR,   M_EX16),
    E1(0x36, CF_00, TEXT_TST,   M_EX16),
    E1(0x3C, CF_00, TEXT_ROL,   M_EX16),
    E1(0x3D, CF_00, TEXT_ASR,   M_EX16),
    E1(0x3E, CF_00, TEXT_ROR,   M_EX16),
    E1(0x3F, CF_00, TEXT_LSR,   M_EX16),
    E1(0x40, CF_30, TEXT_SUBA,  M_IX16),
    E1(0x41, CF_30, TEXT_ADDA,  M_IX16),
    E1(0x42, CF_30, TEXT_SBCA,  M_IX16),
    E1(0x43, CF_30, TEXT_ADCA,  M_IX16),
    E1(0x44, CF_30, TEXT_EORA,  M_IX16),
    E1(0x45, CF_30, TEXT_LDAA,  M_IX16),
    E1(0x46, CF_30, TEXT_ANDA,  M_IX16),
    E1(0x47, CF_30, TEXT_ORAA,  M_IX16),
    E1(0x48, CF_30, TEXT_CMPA,  M_IX16),
    E1(0x49, CF_30, TEXT_BITA,  M_IX16),
    E1(0x4A, CF_30, TEXT_STAA,  M_IX16),
    E1(0x4C, CF_30, TEXT_CPX,   M_IX16),
    E1(0x4D, CF_30, TEXT_CPY,   M_IX16),
    E1(0x4E, CF_30, TEXT_CPZ,   M_IX16),
    E1(0x4F, CF_30, TEXT_CPS,   M_IX16),
    E1(0x70, CF_00, TEXT_SUBA,  M_EX16),
    E1(0x71, CF_00, TEXT_ADDA,  M_EX16),
    E1(0x72, CF_00, TEXT_SBCA,  M_EX16),
    E1(0x73, CF_00, TEXT_ADCA,  M_EX16),
    E1(0x74, CF_00, TEXT_EORA,  M_EX16),
    E1(0x75, CF_00, TEXT_LDAA,  M_EX16),
    E1(0x76, CF_00, TEXT_ANDA,  M_EX16),
    E1(0x77, CF_00, TEXT_ORAA,  M_EX16),
    E1(0x78, CF_00, TEXT_CMPA,  M_EX16),
    E1(0x79, CF_00, TEXT_BITA,  M_EX16),
    E1(0x7A, CF_00, TEXT_STAA,  M_EX16),
    E1(0x7C, CF_00, TEXT_CPX,   M_EX16),
    E1(0x7D, CF_00, TEXT_CPY,   M_EX16),
    E1(0x7E, CF_00, TEXT_CPZ,   M_EX16),
    E1(0x7F, CF_00, TEXT_CPS,   M_EX16),
    E1(0x8C, CF_30, TEXT_STX,   M_IX16),
    E1(0x8D, CF_30, TEXT_STY,   M_IX16),
    E1(0x8E, CF_30, TEXT_STZ,   M_IX16),
    E1(0x8F, CF_30, TEXT_STS,   M_IX16),
    E1(0xBC, CF_00, TEXT_STX,   M_EX16),
    E1(0xBD, CF_00, TEXT_STY,   M_EX16),
    E1(0xBE, CF_00, TEXT_STZ,   M_EX16),
    E1(0xBF, CF_00, TEXT_STS,   M_EX16),
    E1(0xC0, CF_30, TEXT_SUBB,  M_IX16),
    E1(0xC1, CF_30, TEXT_ADDB,  M_IX16),
    E1(0xC2, CF_30, TEXT_SBCB,  M_IX16),
    E1(0xC3, CF_30, TEXT_ADCB,  M_IX16),
    E1(0xC4, CF_30, TEXT_EORB,  M_IX16),
    E1(0xC5, CF_30, TEXT_LDAB,  M_IX16),
    E1(0xC6, CF_30, TEXT_ANDB,  M_IX16),
    E1(0xC7, CF_30, TEXT_ORAB,  M_IX16),
    E1(0xC8, CF_30, TEXT_CMPB,  M_IX16),
    E1(0xC9, CF_30, TEXT_BITB,  M_IX16),
    E1(0xCA, CF_30, TEXT_STAB,  M_IX16),
    E1(0xCC, CF_30, TEXT_LDX,   M_IX16),
    E1(0xCD, CF_30, TEXT_LDY,   M_IX16),
    E1(0xCE, CF_30, TEXT_LDZ,   M_IX16),
    E1(0xCF, CF_30, TEXT_LDS,   M_IX16),
    E1(0xF0, CF_00, TEXT_SUBB,  M_EX16),
    E1(0xF1, CF_00, TEXT_ADDB,  M_EX16),
    E1(0xF2, CF_00, TEXT_SBCB,  M_EX16),
    E1(0xF3, CF_00, TEXT_ADCB,  M_EX16),
    E1(0xF4, CF_00, TEXT_EORB,  M_EX16),
    E1(0xF5, CF_00, TEXT_LDAB,  M_EX16),
    E1(0xF6, CF_00, TEXT_ANDB,  M_EX16),
    E1(0xF7, CF_00, TEXT_ORAB,  M_EX16),
    E1(0xF8, CF_00, TEXT_CMPB,  M_EX16),
    E1(0xF9, CF_00, TEXT_BITB,  M_EX16),
    E1(0xFA, CF_00, TEXT_STAB,  M_EX16),
    E1(0xFC, CF_00, TEXT_LDX,   M_EX16),
    E1(0xFD, CF_00, TEXT_LDY,   M_EX16),
    E1(0xFE, CF_00, TEXT_LDZ,   M_EX16),
    E1(0xFF, CF_00, TEXT_LDS,   M_EX16),
};

constexpr uint8_t MC68HC16_I17[] PROGMEM = {
     29,  // TEXT_ADCA
     44,  // TEXT_ADCA
     67,  // TEXT_ADCB
     82,  // TEXT_ADCB
     27,  // TEXT_ADDA
     42,  // TEXT_ADDA
     65,  // TEXT_ADDB
     80,  // TEXT_ADDB
     32,  // TEXT_ANDA
     47,  // TEXT_ANDA
     70,  // TEXT_ANDB
     85,  // TEXT_ANDB
      4,  // TEXT_ASL
     18,  // TEXT_ASL
     11,  // TEXT_ASR
     23,  // TEXT_ASR
      8,  // TEXT_BCLR
     35,  // TEXT_BITA
     50,  // TEXT_BITA
     73,  // TEXT_BITB
     88,  // TEXT_BITB
      9,  // TEXT_BSET
      6,  // TEXT_CLR
     20,  // TEXT_CLR
     34,  // TEXT_CMPA
     49,  // TEXT_CMPA
     72,  // TEXT_CMPB
     87,  // TEXT_CMPB
      0,  // TEXT_COM
     14,  // TEXT_COM
     40,  // TEXT_CPS
     55,  // TEXT_CPS
     37,  // TEXT_CPX
     52,  // TEXT_CPX
     38,  // TEXT_CPY
     53,  // TEXT_CPY
     39,  // TEXT_CPZ
     54,  // TEXT_CPZ
      1,  // TEXT_DEC
     15,  // TEXT_DEC
     30,  // TEXT_EORA
     45,  // TEXT_EORA
     68,  // TEXT_EORB
     83,  // TEXT_EORB
      3,  // TEXT_INC
     17,  // TEXT_INC
     31,  // TEXT_LDAA
     46,  // TEXT_LDAA
     69,  // TEXT_LDAB
     84,  // TEXT_LDAB
     78,  // TEXT_LDS
     93,  // TEXT_LDS
     75,  // TEXT_LDX
     90,  // TEXT_LDX
     76,  // TEXT_LDY
     91,  // TEXT_LDY
     77,  // TEXT_LDZ
     92,  // TEXT_LDZ
      5,  // TEXT_LSL
     19,  // TEXT_LSL
     13,  // TEXT_LSR
     25,  // TEXT_LSR
      2,  // TEXT_NEG
     16,  // TEXT_NEG
     33,  // TEXT_ORAA
     48,  // TEXT_ORAA
     71,  // TEXT_ORAB
     86,  // TEXT_ORAB
     10,  // TEXT_ROL
     22,  // TEXT_ROL
     12,  // TEXT_ROR
     24,  // TEXT_ROR
     28,  // TEXT_SBCA
     43,  // TEXT_SBCA
     66,  // TEXT_SBCB
     81,  // TEXT_SBCB
     36,  // TEXT_STAA
     51,  // TEXT_STAA
     74,  // TEXT_STAB
     89,  // TEXT_STAB
     59,  // TEXT_STS
     63,  // TEXT_STS
     56,  // TEXT_STX
     60,  // TEXT_STX
     57,  // TEXT_STY
     61,  // TEXT_STY
     58,  // TEXT_STZ
     62,  // TEXT_STZ
     26,  // TEXT_SUBA
     41,  // TEXT_SUBA
     64,  // TEXT_SUBB
     79,  // TEXT_SUBB
      7,  // TEXT_TST
     21,  // TEXT_TST
};

constexpr Entry MC68HC16_P27[] PROGMEM = {
    E1(0x00, CF_30, TEXT_COMW,  M_IX16),
    E1(0x01, CF_30, TEXT_DECW,  M_IX16),
    E1(0x02, CF_30, TEXT_NEGW,  M_IX16),
    E1(0x03, CF_30, TEXT_INCW,  M_IX16),
    E1(0x04, CF_30, TEXT_ASLW,  M_IX16),
    E1(0x04, CF_30, TEXT_LSLW,  M_IX16),
    E1(0x05, CF_30, TEXT_CLRW,  M_IX16),
    E1(0x06, CF_30, TEXT_TSTW,  M_IX16),
    E2(0x08, CF_30, TEXT_BCLRW, M_IX16, M_IM16),
    E2(0x09, CF_30, TEXT_BSETW, M_IX16, M_IM16),
    E1(0x0C, CF_30, TEXT_ROLW,  M_IX16),
    E1(0x0D, CF_30, TEXT_ASRW,  M_IX16),
    E1(0x0E, CF_30, TEXT_RORW,  M_IX16),
    E1(0x0F, CF_30, TEXT_LSRW,  M_IX16),
    E1(0x30, CF_00, TEXT_COMW,  M_EX16),
    E1(0x31, CF_00, TEXT_DECW,  M_EX16),
    E1(0x32, CF_00, TEXT_NEGW,  M_EX16),
    E1(0x33, CF_00, TEXT_INCW,  M_EX16),
    E1(0x34, CF_00, TEXT_ASLW,  M_EX16),
    E1(0x34, CF_00, TEXT_LSLW,  M_EX16),
    E1(0x35, CF_00, TEXT_CLRW,  M_EX16),
    E1(0x36, CF_00, TEXT_TSTW,  M_EX16),
    E2(0x38, CF_00, TEXT_BCLRW, M_EX16, M_IM16),
    E2(0x39, CF_00, TEXT_BSETW, M_EX16, M_IM16),
    E1(0x3C, CF_00, TEXT_ROLW,  M_EX16),
    E1(0x3D, CF_00, TEXT_ASRW,  M_EX16),
    E1(0x3E, CF_00, TEXT_RORW,  M_EX16),
    E1(0x3F, CF_00, TEXT_LSRW,  M_EX16),
    E1(0x40, CF_30, TEXT_SUBA,  M_IXE),
    E1(0x41, CF_30, TEXT_ADDA,  M_IXE),
    E1(0x42, CF_30, TEXT_SBCA,  M_IXE),
    E1(0x43, CF_30, TEXT_ADCA,  M_IXE),
    E1(0x44, CF_30, TEXT_EORA,  M_IXE),
    E1(0x45, CF_30, TEXT_LDAA,  M_IXE),
    E1(0x46, CF_30, TEXT_ANDA,  M_IXE),
    E1(0x47, CF_30, TEXT_ORAA,  M_IXE),
    E1(0x48, CF_30, TEXT_CMPA,  M_IXE),
    E1(0x49, CF_30, TEXT_BITA,  M_IXE),
    E1(0x4A, CF_30, TEXT_STAA,  M_IXE),
    E0(0x4C, CF_00, TEXT_NOP),
    E0(0x4D, CF_00, TEXT_TYX),
    E0(0x4E, CF_00, TEXT_TZX),
    E0(0x4F, CF_00, TEXT_TSX),
    E0(0x5C, CF_00, TEXT_TXY),
    E0(0x5E, CF_00, TEXT_TZY),
    E0(0x5F, CF_00, TEXT_TSY),
    E0(0x6C, CF_00, TEXT_TXZ),
    E0(0x6D, CF_00, TEXT_TYZ),
    E0(0x6F, CF_00, TEXT_TSZ),
    E0(0x70, CF_00, TEXT_COME),
    E1(0x71, CF_00, TEXT_LDED,  M_EX16),
    E0(0x72, CF_00, TEXT_NEGE),
    E1(0x73, CF_00, TEXT_STED,  M_EX16),
    E0(0x74, CF_00, TEXT_ASLE),
    E0(0x74, CF_00, TEXT_LSLE),
    E0(0x75, CF_00, TEXT_CLRE),
    E0(0x76, CF_00, TEXT_TSTE),
    E0(0x77, CF_00, TEXT_RTI),
    E0(0x78, CF_00, TEXT_ADE),
    E0(0x79, CF_00, TEXT_SDE),
    E0(0x7A, CF_00, TEXT_XGDE),
    E0(0x7B, CF_00, TEXT_TDE),
    E0(0x7C, CF_00, TEXT_ROLE),
    E0(0x7D, CF_00, TEXT_ASRE),
    E0(0x7E, CF_00, TEXT_RORE),
    E0(0x7F, CF_00, TEXT_LSRE),
    E1(0x80, CF_30, TEXT_SUBD,  M_IXE),
    E1(0x81, CF_30, TEXT_ADDD,  M_IXE),
    E1(0x82, CF_30, TEXT_SBCD,  M_IXE),
    E1(0x83, CF_30, TEXT_ADCD,  M_IXE),
    E1(0x84, CF_30, TEXT_EORD,  M_IXE),
    E1(0x85, CF_30, TEXT_LDD,   M_IXE),
    E1(0x86, CF_30, TEXT_ANDD,  M_IXE),
    E1(0x87, CF_30, TEXT_ORD,   M_IXE),
    E1(0x88, CF_30, TEXT_CPD,   M_IXE),
    E1(0x8A, CF_30, TEXT_STD,   M_IXE),
    E1(0xB0, CF_00, TEXT_LDHI,  M_EX16),
    E0(0xB1, CF_00, TEXT_TEDM),
    E0(0xB2, CF_00, TEXT_TEM),
    E0(0xB3, CF_00, TEXT_TMXED),
    E0(0xB4, CF_00, TEXT_TMER),
    E0(0xB5, CF_00, TEXT_TMET),
    E0(0xB6, CF_00, TEXT_ASLM),
    E0(0xB7, CF_00, TEXT_CLRM),
    E0(0xB8, CF_00, TEXT_PSHMAC),
    E0(0xB9, CF_00, TEXT_PULMAC),
    E0(0xBA, CF_00, TEXT_ASRM),
    E0(0xBB, CF_00, TEXT_TEKB),
    E1(0xC0, CF_30, TEXT_SUBB,  M_IXE),
    E1(0xC1, CF_30, TEXT_ADDB,  M_IXE),
    E1(0xC2, CF_30, TEXT_SBCB,  M_IXE),
    E1(0xC3, CF_30, TEXT_ADCB,  M_IXE),
    E1(0xC4, CF_30, TEXT_EORB,  M_IXE),
    E1(0xC5, CF_30, TEXT_LDAB,  M_IXE),
    E1(0xC6, CF_30, TEXT_ANDB,  M_IXE),
    E1(0xC7, CF_30, TEXT_ORAB,  M_IXE),
    E1(0xC8, CF_30, TEXT_CMPB,  M_IXE),
    E1(0xC9, CF_30, TEXT_BITB,  M_IXE),
    E1(0xCA, CF_30, TEXT_STAB,  M_IXE),
    E0(0xF0, CF_00, TEXT_COMD),
    E0(0xF1, CF_00, TEXT_LPSTOP),
    E0(0xF2, CF_00, TEXT_NEGD),
    E0(0xF3, CF_00, TEXT_WAI),
    E0(0xF4, CF_00, TEXT_ASLD),
    E0(0xF4, CF_00, TEXT_LSLD),
    E0(0xF5, CF_00, TEXT_CLRD),
    E0(0xF6, CF_00, TEXT_TSTD),
    E0(0xF7, CF_00, TEXT_RTS),
    E0(0xF8, CF_00, TEXT_SXT),
    E1(0xF9, CF_00, TEXT_LBSR,  M_RL16),
    E0(0xFA, CF_00, TEXT_TBEK),
    E0(0xFB, CF_00, TEXT_TED),
    E0(0xFC, CF_00, TEXT_ROLD),
    E0(0xFD, CF_00, TEXT_ASRD),
    E0(0xFE, CF_00, TEXT_RORD),
    E0(0xFF, CF_00, TEXT_LSRD),
};

constexpr uint8_t MC68HC16_I27[] PROGMEM = {
     31,  // TEXT_ADCA
     91,  // TEXT_ADCB
     69,  // TEXT_ADCD
     29,  // TEXT_ADDA
     89,  // TEXT_ADDB
     67,  // TEXT_ADDD
     58,  // TEXT_ADE
     34,  // TEXT_ANDA
     94,  // TEXT_ANDB
     72,  // TEXT_ANDD
    103,  // TEXT_ASLD
     53,  // TEXT_ASLE
     82,  // TEXT_ASLM
      4,  // TEXT_ASLW
     18,  // TEXT_ASLW
    113,  // TEXT_ASRD
     63,  // TEXT_ASRE
     86,  // TEXT_ASRM
     11,  // TEXT_ASRW
     25,  // TEXT_ASRW
      8,  // TEXT_BCLRW
     22,  // TEXT_BCLRW
     37,  // TEXT_BITA
     97,  // TEXT_BITB
      9,  // TEXT_BSETW
     23,  // TEXT_BSETW
    105,  // TEXT_CLRD
     55,  // TEXT_CLRE
     83,  // TEXT_CLRM
      6,  // TEXT_CLRW
     20,  // TEXT_CLRW
     36,  // TEXT_CMPA
     96,  // TEXT_CMPB
     99,  // TEXT_COMD
     49,  // TEXT_COME
      0,  // TEXT_COMW
     14,  // TEXT_COMW
     74,  // TEXT_CPD
      1,  // TEXT_DECW
     15,  // TEXT_DECW
     32,  // TEXT_EORA
     92,  // TEXT_EORB
     70,  // TEXT_EORD
      3,  // TEXT_INCW
     17,  // TEXT_INCW
    109,  // TEXT_LBSR
     33,  // TEXT_LDAA
     93,  // TEXT_LDAB
     71,  // TEXT_LDD
     50,  // TEXT_LDED
     76,  // TEXT_LDHI
    100,  // TEXT_LPSTOP
    104,  // TEXT_LSLD
     54,  // TEXT_LSLE
      5,  // TEXT_LSLW
     19,  // TEXT_LSLW
    115,  // TEXT_LSRD
     65,  // TEXT_LSRE
     13,  // TEXT_LSRW
     27,  // TEXT_LSRW
    101,  // TEXT_NEGD
     51,  // TEXT_NEGE
      2,  // TEXT_NEGW
     16,  // TEXT_NEGW
     39,  // TEXT_NOP
     35,  // TEXT_ORAA
     95,  // TEXT_ORAB
     73,  // TEXT_ORD
     84,  // TEXT_PSHMAC
     85,  // TEXT_PULMAC
    112,  // TEXT_ROLD
     62,  // TEXT_ROLE
     10,  // TEXT_ROLW
     24,  // TEXT_ROLW
    114,  // TEXT_RORD
     64,  // TEXT_RORE
     12,  // TEXT_RORW
     26,  // TEXT_RORW
     57,  // TEXT_RTI
    107,  // TEXT_RTS
     30,  // TEXT_SBCA
     90,  // TEXT_SBCB
     68,  // TEXT_SBCD
     59,  // TEXT_SDE
     38,  // TEXT_STAA
     98,  // TEXT_STAB
     75,  // TEXT_STD
     52,  // TEXT_STED
     28,  // TEXT_SUBA
     88,  // TEXT_SUBB
     66,  // TEXT_SUBD
    108,  // TEXT_SXT
    110,  // TEXT_TBEK
     61,  // TEXT_TDE
    111,  // TEXT_TED
     77,  // TEXT_TEDM
     87,  // TEXT_TEKB
     78,  // TEXT_TEM
     80,  // TEXT_TMER
     81,  // TEXT_TMET
     79,  // TEXT_TMXED
    106,  // TEXT_TSTD
     56,  // TEXT_TSTE
      7,  // TEXT_TSTW
     21,  // TEXT_TSTW
     42,  // TEXT_TSX
     45,  // TEXT_TSY
     48,  // TEXT_TSZ
     43,  // TEXT_TXY
     46,  // TEXT_TXZ
     40,  // TEXT_TYX
     47,  // TEXT_TYZ
     41,  // TEXT_TZX
     44,  // TEXT_TZY
    102,  // TEXT_WAI
     60,  // TEXT_XGDE
};

constexpr Entry MC68HC16_P37[] PROGMEM = {
    E0(0x00, CF_00, TEXT_COMA),
    E0(0x01, CF_00, TEXT_DECA),
    E0(0x02, CF_00, TEXT_NEGA),
    E0(0x03, CF_00, TEXT_INCA),
    E0(0x04, CF_00, TEXT_ASLA),
    E0(0x04, CF_00, TEXT_LSLA),
    E0(0x05, CF_00, TEXT_CLRA),
    E0(0x06, CF_00, TEXT_TSTA),
    E0(0x07, CF_00, TEXT_TBA),
    E0(0x08, CF_00, TEXT_PSHA),
    E0(0x09, CF_00, TEXT_PULA),
    E0(0x0A, CF_00, TEXT_SBA),
    E0(0x0B, CF_00, TEXT_ABA),
    E0(0x0C, CF_00, TEXT_ROLA),
    E0(0x0D, CF_00, TEXT_ASRA),
    E0(0x0E, CF_00, TEXT_RORA),
    E0(0x0F, CF_00, TEXT_LSRA),
    E0(0x10, CF_00, TEXT_COMB),
    E0(0x11, CF_00, TEXT_DECB),
    E0(0x12, CF_00, TEXT_NEGB),
    E0(0x13, CF_00, TEXT_INCB),
    E0(0x14, CF_00, TEXT_ASLB),
    E0(0x14, CF_00, TEXT_LSLB),
    E0(0x15, CF_00, TEXT_CLRB),
    E0(0x16, CF_00, TEXT_TSTB),
    E0(0x17, CF_00, TEXT_TAB),
    E0(0x18, CF_00, TEXT_PSHB),
    E0(0x19, CF_00, TEXT_PULB),
    E0(0x1A, CF_00, TEXT_XGAB),
    E0(0x1B, CF_00, TEXT_CBA),
    E0(0x1C, CF_00, TEXT_ROLB),
    E0(0x1D, CF_00, TEXT_ASRB),
    E0(0x1E, CF_00, TEXT_RORB),
    E0(0x1F, CF_00, TEXT_LSRB),
    E0(0x20, CF_00, TEXT_SWI),
    E0(0x21, CF_00, TEXT_DAA),
    E0(0x22, CF_00, TEXT_ACE),
    E0(0x23, CF_00, TEXT_ACED),
    E0(0x24, CF_00, TEXT_MUL),
    E0(0x25, CF_00, TEXT_EMUL),
    E0(0x26, CF_00, TEXT_EMULS),
    E0(0x27, CF_00, TEXT_FMULS),
    E0(0x28, CF_00, TEXT_EDIV),
    E0(0x29, CF_00, TEXT_EDIVS),
    E0(0x2A, CF_00, TEXT_IDIV),
    E0(0x2B, CF_00, TEXT_FDIV),
    E0(0x2C, CF_00, TEXT_TPD),
    E0(0x2D, CF_00, TEXT_TDP),
    E0(0x2F, CF_00, TEXT_TDMSK),
    E1(0x30, CF_00, TEXT_SUBE,  M_IM16),
    E1(0x31, CF_00, TEXT_ADDE,  M_IM16),
    E1(0x32, CF_00, TEXT_SBCE,  M_IM16),
    E1(0x33, CF_00, TEXT_ADCE,  M_IM16),
    E1(0x34, CF_00, TEXT_EORE,  M_IM16),
    E1(0x35, CF_00, TEXT_LDE,   M_IM16),
    E1(0x36, CF_00, TEXT_ANDE,  M_IM16),
    E1(0x37, CF_00, TEXT_ORE,   M_IM16),
    E1(0x38, CF_00, TEXT_CPE,   M_IM16),
    E1(0x3A, CF_00, TEXT_ANDP,  M_IM16),
    E1(0x3B, CF_00, TEXT_ORP,   M_IM16),
    E1(0x3C, CF_00, TEXT_AIX,   M_IM16),
    E1(0x3D, CF_00, TEXT_AIY,   M_IM16),
    E1(0x3E, CF_00, TEXT_AIZ,   M_IM16),
    E1(0x3F, CF_00, TEXT_AIS,   M_IM16),
    E1(0x40, CF_30, TEXT_SUBE,  M_IX16),
    E1(0x41, CF_30, TEXT_ADDE,  M_IX16),
    E1(0x42, CF_30, TEXT_SBCE,  M_IX16),
    E1(0x43, CF_30, TEXT_ADCE,  M_IX16),
    E1(0x44, CF_30, TEXT_EORE,  M_IX16),
    E1(0x45, CF_30, TEXT_LDE,   M_IX16),
    E1(0x46, CF_30, TEXT_ANDE,  M_IX16),
    E1(0x47, CF_30, TEXT_ORE,   M_IX16),
    E1(0x48, CF_30, TEXT_CPE,   M_IX16),
    E1(0x4A, CF_30, TEXT_STE,   M_IX16),
    E0(0x4C, CF_00, TEXT_XGEX),
    E0(0x4D, CF_00, TEXT_AEX),
    E0(0x4E, CF_00, TEXT_TXS),
    E0(0x4F, CF_00, TEXT_ABX),
    E0(0x5C, CF_00, TEXT_XGEY),
    E0(0x5D, CF_00, TEXT_AEY),
    E0(0x5E, CF_00, TEXT_TYS),
    E0(0x5F, CF_00, TEXT_ABY),
    E0(0x6C, CF_00, TEXT_XGEZ),
    E0(0x6D, CF_00, TEXT_AEZ),
    E0(0x6E, CF_00, TEXT_TZS),
    E0(0x6F, CF_00, TEXT_ABZ),
    E1(0x70, CF_00, TEXT_SUBE,  M_EX16),
    E1(0x71, CF_00, TEXT_ADDE,  M_EX16),
    E1(0x72, CF_00, TEXT_SBCE,  M_EX16),
    E1(0x73, CF_00, TEXT_ADCE,  M_EX16),
    E1(0x74, CF_00, TEXT_EORE,  M_EX16),
    E1(0x75, CF_00, TEXT_LDE,   M_EX16),
    E1(0x76, CF_00, TEXT_ANDE,  M_EX16),
    E1(0x77, CF_00, TEXT_ORE,   M_EX16),
    E1(0x78, CF_00, TEXT_CPE,   M_EX16),
    E1(0x7A, CF_00, TEXT_STE,   M_EX16),
    E1(0x7C, CF_00, TEXT_CPX,   M_IM16),
    E1(0x7D, CF_00, TEXT_CPY,   M_IM16),
    E1(0x7E, CF_00, TEXT_CPZ,   M_IM16),
    E1(0x7F, CF_00, TEXT_CPS,   M_IM16),
    E1(0x80, CF_00, TEXT_LBRA,  M_RL16),
    E1(0x81, CF_00, TEXT_LBRN,  M_RL16),
    E1(0x82, CF_00, TEXT_LBHI,  M_RL16),
    E1(0x83, CF_00, TEXT_LBLS,  M_RL16),
    E1(0x84, CF_00, TEXT_LBCC,  M_RL16),
    E1(0x85, CF_00, TEXT_LBCS,  M_RL16),
    E1(0x84, CF_00, TEXT_LBHS,  M_RL16),
    E1(0x85, CF_00, TEXT_LBLO,  M_RL16),
    E1(0x86, CF_00, TEXT_LBNE,  M_RL16),
    E1(0x87, CF_00, TEXT_LBEQ,  M_RL16),
    E1(0x88, CF_00, TEXT_LBVC,  M_RL16),
    E1(0x89, CF_00, TEXT_LBVS,  M_RL16),
    E1(0x8A, CF_00, TEXT_LBPL,  M_RL16),
    E1(0x8B, CF_00, TEXT_LBMI,  M_RL16),
    E1(0x8C, CF_00, TEXT_LBGE,  M_RL16),
    E1(0x8D, CF_00, TEXT_LBLT,  M_RL16),
    E1(0x8E, CF_00, TEXT_LBGT,  M_RL16),
    E1(0x8F, CF_00, TEXT_LBLE,  M_RL16),
    E1(0x90, CF_00, TEXT_LBMV,  M_RL16),
    E1(0x91, CF_00, TEXT_LBEV,  M_RL16),
    E0(0x9C, CF_00, TEXT_TBXK),
    E0(0x9D, CF_00, TEXT_TBYK),
    E0(0x9E, CF_00, TEXT_TBZK),
    E0(0x9F, CF_00, TEXT_TBSK),
    E0(0xA6, CF_00, TEXT_BGND),
    E0(0xAC, CF_00, TEXT_TXKB),
    E0(0xAD, CF_00, TEXT_TYKB),
    E0(0xAE, CF_00, TEXT_TZKB),
    E0(0xAF, CF_00, TEXT_TSKB),
    E1(0xB0, CF_00, TEXT_SUBD,  M_IM16),
    E1(0xB1, CF_00, TEXT_ADDD,  M_IM16),
    E1(0xB2, CF_00, TEXT_SBCD,  M_IM16),
    E1(0xB3, CF_00, TEXT_ADCD,  M_IM16),
    E1(0xB4, CF_00, TEXT_EORD,  M_IM16),
    E1(0xB5, CF_00, TEXT_LDD,   M_IM16),
    E1(0xB6, CF_00, TEXT_ANDD,  M_IM16),
    E1(0xB7, CF_00, TEXT_ORD,   M_IM16),
    E1(0xB8, CF_00, TEXT_CPD,   M_IM16),
    E1(0xBC, CF_00, TEXT_LDX,   M_IM16),
    E1(0xBD, CF_00, TEXT_LDY,   M_IM16),
    E1(0xBE, CF_00, TEXT_LDZ,   M_IM16),
    E1(0xBF, CF_00, TEXT_LDS,   M_IM16),
    E1(0xC0, CF_30, TEXT_SUBD,  M_IX16),
    E1(0xC1, CF_30, TEXT_ADDD,  M_IX16),
    E1(0xC2, CF_30, TEXT_SBCD,  M_IX16),
    E1(0xC3, CF_30, TEXT_ADCD,  M_IX16),
    E1(0xC4, CF_30, TEXT_EORD,  M_IX16),
    E1(0xC5, CF_30, TEXT_LDD,   M_IX16),
    E1(0xC6, CF_30, TEXT_ANDD,  M_IX16),
    E1(0xC7, CF_30, TEXT_ORD,   M_IX16),
    E1(0xC8, CF_30, TEXT_CPD,   M_IX16),
    E1(0xCA, CF_30, TEXT_STD,   M_IX16),
    E0(0xCC, CF_00, TEXT_XGDX),
    E0(0xCD, CF_00, TEXT_ADX),
    E0(0xDC, CF_00, TEXT_XGDY),
    E0(0xDD, CF_00, TEXT_ADY),
    E0(0xEC, CF_00, TEXT_XGDZ),
    E0(0xED, CF_00, TEXT_ADZ),
    E1(0xF0, CF_00, TEXT_SUBD,  M_EX16),
    E1(0xF1, CF_00, TEXT_ADDD,  M_EX16),
    E1(0xF2, CF_00, TEXT_SBCD,  M_EX16),
    E1(0xF3, CF_00, TEXT_ADCD,  M_EX16),
    E1(0xF4, CF_00, TEXT_EORD,  M_EX16),
    E1(0xF5, CF_00, TEXT_LDD,   M_EX16),
    E1(0xF6, CF_00, TEXT_ANDD,  M_EX16),
    E1(0xF7, CF_00, TEXT_ORD,   M_EX16),
    E1(0xF8, CF_00, TEXT_CPD,   M_EX16),
    E1(0xFA, CF_00, TEXT_STD,   M_EX16),
    E0(0xFC, CF_00, TEXT_TPA),
    E0(0xFD, CF_00, TEXT_TAP),
    E2(0xFE, CF_00, TEXT_MOVB,  M_EX16, M_EX16),
    E2(0xFF, CF_00, TEXT_MOVW,  M_EX16, M_EX16),
};

constexpr uint8_t MC68HC16_I37[] PROGMEM = {
     12,  // TEXT_ABA
     77,  // TEXT_ABX
     81,  // TEXT_ABY
     85,  // TEXT_ABZ
     36,  // TEXT_ACE
     37,  // TEXT_ACED
    132,  // TEXT_ADCD
    145,  // TEXT_ADCD
    161,  // TEXT_ADCD
     52,  // TEXT_ADCE
     67,  // TEXT_ADCE
     89,  // TEXT_ADCE
    130,  // TEXT_ADDD
    143,  // TEXT_ADDD
    159,  // TEXT_ADDD
     50,  // TEXT_ADDE
     65,  // TEXT_ADDE
     87,  // TEXT_ADDE
    153,  // TEXT_ADX
    155,  // TEXT_ADY
    157,  // TEXT_ADZ
     75,  // TEXT_AEX
     79,  // TEXT_AEY
     83,  // TEXT_AEZ
     63,  // TEXT_AIS
     60,  // TEXT_AIX
     61,  // TEXT_AIY
     62,  // TEXT_AIZ
    135,  // TEXT_ANDD
    148,  // TEXT_ANDD
    164,  // TEXT_ANDD
     55,  // TEXT_ANDE
     70,  // TEXT_ANDE
     92,  // TEXT_ANDE
     58,  // TEXT_ANDP
      4,  // TEXT_ASLA
     21,  // TEXT_ASLB
     14,  // TEXT_ASRA
     31,  // TEXT_ASRB
    124,  // TEXT_BGND
     29,  // TEXT_CBA
      6,  // TEXT_CLRA
     23,  // TEXT_CLRB
      0,  // TEXT_COMA
     17,  // TEXT_COMB
    137,  // TEXT_CPD
    150,  // TEXT_CPD
    166,  // TEXT_CPD
     57,  // TEXT_CPE
     72,  // TEXT_CPE
     94,  // TEXT_CPE
     99,  // TEXT_CPS
     96,  // TEXT_CPX
     97,  // TEXT_CPY
     98,  // TEXT_CPZ
     35,  // TEXT_DAA
      1,  // TEXT_DECA
     18,  // TEXT_DECB
     42,  // TEXT_EDIV
     43,  // TEXT_EDIVS
     39,  // TEXT_EMUL
     40,  // TEXT_EMULS
    133,  // TEXT_EORD
    146,  // TEXT_EORD
    162,  // TEXT_EORD
     53,  // TEXT_EORE
     68,  // TEXT_EORE
     90,  // TEXT_EORE
     45,  // TEXT_FDIV
     41,  // TEXT_FMULS
     44,  // TEXT_IDIV
      3,  // TEXT_INCA
     20,  // TEXT_INCB
    104,  // TEXT_LBCC
    105,  // TEXT_LBCS
    109,  // TEXT_LBEQ
    119,  // TEXT_LBEV
    114,  // TEXT_LBGE
    116,  // TEXT_LBGT
    102,  // TEXT_LBHI
    106,  // TEXT_LBHS
    117,  // TEXT_LBLE
    107,  // TEXT_LBLO
    103,  // TEXT_LBLS
    115,  // TEXT_LBLT
    113,  // TEXT_LBMI
    118,  // TEXT_LBMV
    108,  // TEXT_LBNE
    112,  // TEXT_LBPL
    100,  // TEXT_LBRA
    101,  // TEXT_LBRN
    110,  // TEXT_LBVC
    111,  // TEXT_LBVS
    134,  // TEXT_LDD
    147,  // TEXT_LDD
    163,  // TEXT_LDD
     54,  // TEXT_LDE
     69,  // TEXT_LDE
     91,  // TEXT_LDE
    141,  // TEXT_LDS
    138,  // TEXT_LDX
    139,  // TEXT_LDY
    140,  // TEXT_LDZ
      5,  // TEXT_LSLA
     22,  // TEXT_LSLB
     16,  // TEXT_LSRA
     33,  // TEXT_LSRB
    170,  // TEXT_MOVB
    171,  // TEXT_MOVW
     38,  // TEXT_MUL
      2,  // TEXT_NEGA
     19,  // TEXT_NEGB
    136,  // TEXT_ORD
    149,  // TEXT_ORD
    165,  // TEXT_ORD
     56,  // TEXT_ORE
     71,  // TEXT_ORE
     93,  // TEXT_ORE
     59,  // TEXT_ORP
      9,  // TEXT_PSHA
     26,  // TEXT_PSHB
     10,  // TEXT_PULA
     27,  // TEXT_PULB
     13,  // TEXT_ROLA
     30,  // TEXT_ROLB
     15,  // TEXT_RORA
     32,  // TEXT_RORB
     11,  // TEXT_SBA
    131,  // TEXT_SBCD
    144,  // TEXT_SBCD
    160,  // TEXT_SBCD
     51,  // TEXT_SBCE
     66,  // TEXT_SBCE
     88,  // TEXT_SBCE
    151,  // TEXT_STD
    167,  // TEXT_STD
     73,  // TEXT_STE
     95,  // TEXT_STE
    129,  // TEXT_SUBD
    142,  // TEXT_SUBD
    158,  // TEXT_SUBD
     49,  // TEXT_SUBE
     64,  // TEXT_SUBE
     86,  // TEXT_SUBE
     34,  // TEXT_SWI
     25,  // TEXT_TAB
    169,  // TEXT_TAP
      8,  // TEXT_TBA
    123,  // TEXT_TBSK
    120,  // TEXT_TBXK
    121,  // TEXT_TBYK
    122,  // TEXT_TBZK
     48,  // TEXT_TDMSK
     47,  // TEXT_TDP
    168,  // TEXT_TPA
     46,  // TEXT_TPD
    128,  // TEXT_TSKB
      7,  // TEXT_TSTA
     24,  // TEXT_TSTB
    125,  // TEXT_TXKB
     76,  // TEXT_TXS
    126,  // TEXT_TYKB
     80,  // TEXT_TYS
    127,  // TEXT_TZKB
     84,  // TEXT_TZS
     28,  // TEXT_XGAB
    152,  // TEXT_XGDX
    154,  // TEXT_XGDY
    156,  // TEXT_XGDZ
     74,  // TEXT_XGEX
     78,  // TEXT_XGEY
     82,  // TEXT_XGEZ
};

constexpr Entry MC68HC16_IX16_TABLE[] PROGMEM = {
    E2(0x08, CF_30, TEXT_BCLR,  M_IX16, M_IM8),
    E2(0x09, CF_30, TEXT_BSET,  M_IX16, M_IM8),
    E3(0x0A, CF_30, TEXT_BRCLR, M_IX16, M_IM8, M_RL16),
    E3(0x0B, CF_30, TEXT_BRSET, M_IX16, M_IM8, M_RL16),
};

constexpr uint8_t MC68HC16_IX16_INDEX[] PROGMEM = {
      0,  // TEXT_BCLR
      2,  // TEXT_BRCLR
      3,  // TEXT_BRSET
      1,  // TEXT_BSET
};

constexpr Entry MC68HC16_PSHM_P34[] PROGMEM = {
    E0(0x01, CF_00, TEXT_PSHD),
    E0(0x04, CF_00, TEXT_PSHX),
    E0(0x08, CF_00, TEXT_PSHY),
};

constexpr uint8_t MC68HC16_PSHM_I34[] PROGMEM = {
      0,  // TEXT_PSHD
      1,  // TEXT_PSHX
      2,  // TEXT_PSHY
};

constexpr Entry MC68HC16_PULM_P35[] PROGMEM = {
    E0(0x40, CF_00, TEXT_PULD),
    E0(0x10, CF_00, TEXT_PULX),
    E0(0x08, CF_00, TEXT_PULY),
};

constexpr uint8_t MC68HC16_PULM_I35[] PROGMEM = {
      0,  // TEXT_PULD
      1,  // TEXT_PULX
      2,  // TEXT_PULY
};

constexpr Entry MC68HC16_AIX_P3C[] PROGMEM = {
    E0(0xFF, CF_00, TEXT_DEX),
    E0(0x01, CF_00, TEXT_INX),
};

constexpr uint8_t MC68HC16_AIX_I3C[] PROGMEM = {
      0,  // TEXT_DEX
      1,  // TEXT_INX
};

constexpr Entry MC68HC16_AIY_P3D[] PROGMEM = {
    E0(0xFF, CF_00, TEXT_DEY),
    E0(0x01, CF_00, TEXT_INY),
};

constexpr uint8_t MC68HC16_AIY_I3D[] PROGMEM = {
      0,  // TEXT_DEY
      1,  // TEXT_INY
};

constexpr Entry MC68HC16_AIS_P3F[] PROGMEM = {
    E0(0xFF, CF_00, TEXT_DES),
    E0(0x01, CF_00, TEXT_INS),
};

constexpr uint8_t MC68HC16_AIS_I3F[] PROGMEM = {
      0,  // TEXT_DES
      1,  // TEXT_INS
};

constexpr Entry MC68HC16_ANDCC_P373A[] PROGMEM = {
    P0(0xFE, 0xFF, TEXT_CLC),
    P0(0xFF, 0x1F, TEXT_CLI),
    P0(0xFD, 0xFF, TEXT_CLV),
};

constexpr uint8_t MC68HC16_ANDCC_I373A[] PROGMEM = {
      0,  // TEXT_CLC
      1,  // TEXT_CLI
      2,  // TEXT_CLV
};

constexpr Entry MC68HC16_ORCC_P373B[] PROGMEM = {
    P0(0x01, 0x00, TEXT_SEC),
    P0(0x00, 0xE0, TEXT_SEI),
    P0(0x02, 0x00, TEXT_SEV),
};

constexpr uint8_t MC68HC16_ORCC_I373B[] PROGMEM = {
      0,  // TEXT_SEC
      1,  // TEXT_SEI
      2,  // TEXT_SEV
};

// clang-format on

using EntryPage = entry::PrefixTableBase<Entry>;

constexpr EntryPage MC68HC16_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(MC68HC16_TABLE), ARRAY_RANGE(MC68HC16_INDEX)},
        {0x17, ARRAY_RANGE(MC68HC16_P17), ARRAY_RANGE(MC68HC16_I17)},
        {0x00, ARRAY_RANGE(MC68HC16_IX16_TABLE), ARRAY_RANGE(MC68HC16_IX16_INDEX)},
        {0x27, ARRAY_RANGE(MC68HC16_P27), ARRAY_RANGE(MC68HC16_I27)},
        {0x3C, ARRAY_RANGE(MC68HC16_AIX_P3C), ARRAY_RANGE(MC68HC16_AIX_I3C)},
        {0x3D, ARRAY_RANGE(MC68HC16_AIY_P3D), ARRAY_RANGE(MC68HC16_AIY_I3D)},
        {0x3F, ARRAY_RANGE(MC68HC16_AIS_P3F), ARRAY_RANGE(MC68HC16_AIS_I3F)},
        {0x37, ARRAY_RANGE(MC68HC16_P37), ARRAY_RANGE(MC68HC16_I37)},
        {0x34, ARRAY_RANGE(MC68HC16_PSHM_P34), ARRAY_RANGE(MC68HC16_PSHM_I34)},
        {0x35, ARRAY_RANGE(MC68HC16_PULM_P35), ARRAY_RANGE(MC68HC16_PULM_I35)},
        {0x373A, ARRAY_RANGE(MC68HC16_ANDCC_P373A), ARRAY_RANGE(MC68HC16_ANDCC_I373A)},
        {0x373B, ARRAY_RANGE(MC68HC16_ORCC_P373B), ARRAY_RANGE(MC68HC16_ORCC_I373B)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {MC68HC16, TEXT_CPU_68HC16, ARRAY_RANGE(MC68HC16_PAGES)},
};

const Cpu *cpu(CpuType cpuType) {
    return &CPU_TABLE[0];
}

bool hasOperand(CpuType cpuType, AsmInsn &insn) {
    cpu(cpuType)->searchName(insn, Cpu::acceptAll<AsmInsn, Entry>);
    return insn.isOK() && insn.mode1() != M_NONE;
}

bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_EX16)
        return table == M_RL8 || table == M_RL16 || table == M_EX16 || table == M_EX20 ||
               table == M_IM4H || table == M_IM4L;
    if (opr == M_IM8)
        return table == M_IM16;
    if (opr == M_IX8)
        return table == M_IX16 || table == M_IX20 || table == M_IXX8;
    if (opr == M_IX16)
        return table == M_IX20;
    if (opr == M_NONE)
        return table == M_POST;
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
    if (flags.format() == CF_30) {
        if ((opc & 0x30) == 0x30)
            return false;
        opc &= ~0x30;
    }
    return opc == entry->readOpCode();
}

Error searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) {
    cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    return insn.getError();
}

bool isPrefix(CpuType cpuType, Config::opcode_t code) {
    return code == 0x17 || code == 0x27 || code == 0x37;
}

const /*PROGMEM*/ char *TableMc68HC16::listCpu_P() const {
    return TEXT_CPU_MC68HC16;
}

const /*PROGMEM*/ char *TableMc68HC16::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableMc68HC16::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    auto p = name;
    if (p.iexpectText_P(TEXT_CPU_MC68HC16, 2))
        ;  // ignore prefix MC
    auto t = Cpu::search(p, ARRAY_RANGE(CPU_TABLE));
    if (t) {
        cpuType = t->readCpuType();
        return OK;
    }
    return UNSUPPORTED_CPU;
}

const TableMc68HC16 TABLE;

}  // namespace mc68hc16
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

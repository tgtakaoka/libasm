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

#include "table_mc6809.h"

#include <string.h>

#include "config_mc6809.h"
#include "entry_mc6809.h"
#include "text_mc6809.h"

namespace libasm {
namespace mc6809 {

#define X(_opc, _name, _op1, _op2) \
    { _opc, Entry::Flags::create(_op1, _op2), _name }
#define E(_opc, _name, _op1) X(_opc, _name, _op1, M_NONE)
#define U(_opc, _name, _op1) X(_opc, _name, _op1, M_LIST)

// clang-format off
static constexpr Entry MC6809_P00[] PROGMEM = {
    E(0x00, TEXT_NEG,   M_GMEM),
    E(0x03, TEXT_COM,   M_GMEM),
    E(0x04, TEXT_LSR,   M_GMEM),
    E(0x06, TEXT_ROR,   M_GMEM),
    E(0x07, TEXT_ASR,   M_GMEM),
    E(0x08, TEXT_ASL,   M_GMEM),
    E(0x08, TEXT_LSL,   M_GMEM),
    E(0x09, TEXT_ROL,   M_GMEM),
    E(0x0A, TEXT_DEC,   M_GMEM),
    E(0x0C, TEXT_INC,   M_GMEM),
    E(0x0D, TEXT_TST,   M_GMEM),
    E(0x0E, TEXT_JMP,   M_GMEM),
    E(0x0F, TEXT_CLR,   M_GMEM),
    E(0x12, TEXT_NOP,   M_NONE),
    E(0x13, TEXT_SYNC,  M_NONE),
    E(0x16, TEXT_LBRA,  M_LREL),
    E(0x17, TEXT_LBSR,  M_LREL),
    E(0x19, TEXT_DAA,   M_NONE),
    E(0x1A, TEXT_ORCC,  M_IM8),
    E(0x1C, TEXT_ANDCC, M_IM8),
    E(0x1D, TEXT_SEX,   M_NONE),
    E(0x1E, TEXT_EXG,   M_PAIR),
    E(0x1F, TEXT_TFR,   M_PAIR),
    E(0x20, TEXT_BRA,   M_REL),
    E(0x21, TEXT_BRN,   M_REL),
    E(0x22, TEXT_BHI,   M_REL),
    E(0x23, TEXT_BLS,   M_REL),
    E(0x24, TEXT_BHS,   M_REL),
    E(0x24, TEXT_BCC,   M_REL),
    E(0x25, TEXT_BLO,   M_REL),
    E(0x25, TEXT_BCS,   M_REL),
    E(0x26, TEXT_BNE,   M_REL),
    E(0x27, TEXT_BEQ,   M_REL),
    E(0x28, TEXT_BVC,   M_REL),
    E(0x29, TEXT_BVS,   M_REL),
    E(0x2A, TEXT_BPL,   M_REL),
    E(0x2B, TEXT_BMI,   M_REL),
    E(0x2C, TEXT_BGE,   M_REL),
    E(0x2D, TEXT_BLT,   M_REL),
    E(0x2E, TEXT_BGT,   M_REL),
    E(0x2F, TEXT_BLE,   M_REL),
    E(0x30, TEXT_LEAX,  M_IDX),
    E(0x31, TEXT_LEAY,  M_IDX),
    E(0x32, TEXT_LEAS,  M_IDX),
    E(0x33, TEXT_LEAU,  M_IDX),
    E(0x34, TEXT_PSHS,  M_LIST),
    E(0x35, TEXT_PULS,  M_LIST),
    E(0x36, TEXT_PSHU,  M_LIST),
    E(0x37, TEXT_PULU,  M_LIST),
    E(0x39, TEXT_RTS,   M_NONE),
    E(0x3A, TEXT_ABX,   M_NONE),
    E(0x3B, TEXT_RTI,   M_NONE),
    E(0x3C, TEXT_CWAI,  M_IM8),
    E(0x3D, TEXT_MUL,   M_NONE),
    E(0x3F, TEXT_SWI,   M_NONE),
    E(0x40, TEXT_NEGA,  M_NONE),
    E(0x43, TEXT_COMA,  M_NONE),
    E(0x44, TEXT_LSRA,  M_NONE),
    E(0x46, TEXT_RORA,  M_NONE),
    E(0x47, TEXT_ASRA,  M_NONE),
    E(0x48, TEXT_ASLA,  M_NONE),
    E(0x48, TEXT_LSLA,  M_NONE),
    E(0x49, TEXT_ROLA,  M_NONE),
    E(0x4A, TEXT_DECA,  M_NONE),
    E(0x4C, TEXT_INCA,  M_NONE),
    E(0x4D, TEXT_TSTA,  M_NONE),
    E(0x4F, TEXT_CLRA,  M_NONE),
    E(0x50, TEXT_NEGB,  M_NONE),
    E(0x53, TEXT_COMB,  M_NONE),
    E(0x54, TEXT_LSRB,  M_NONE),
    E(0x56, TEXT_RORB,  M_NONE),
    E(0x57, TEXT_ASRB,  M_NONE),
    E(0x58, TEXT_ASLB,  M_NONE),
    E(0x58, TEXT_LSLB,  M_NONE),
    E(0x59, TEXT_ROLB,  M_NONE),
    E(0x5A, TEXT_DECB,  M_NONE),
    E(0x5C, TEXT_INCB,  M_NONE),
    E(0x5D, TEXT_TSTB,  M_NONE),
    E(0x5F, TEXT_CLRB,  M_NONE),
    E(0x80, TEXT_SUBA,  M_GEN8),
    E(0x81, TEXT_CMPA,  M_GEN8),
    E(0x82, TEXT_SBCA,  M_GEN8),
    E(0x83, TEXT_SUBD,  M_GEN16),
    E(0x84, TEXT_ANDA,  M_GEN8),
    E(0x85, TEXT_BITA,  M_GEN8),
    E(0x86, TEXT_LDA,   M_GEN8),
    U(0x87, TEXT_STA,   M_IM8),
    E(0x87, TEXT_STA,   M_GEN8),
    E(0x88, TEXT_EORA,  M_GEN8),
    E(0x89, TEXT_ADCA,  M_GEN8),
    E(0x8A, TEXT_ORA,   M_GEN8),
    E(0x8B, TEXT_ADDA,  M_GEN8),
    E(0x8C, TEXT_CMPX,  M_GEN16),
    E(0x8D, TEXT_BSR,   M_REL),
    E(0x8D, TEXT_JSR,   M_GEN16),
    E(0x8E, TEXT_LDX,   M_GEN16),
    U(0x8F, TEXT_STX,   M_IM32),
    E(0x8F, TEXT_STX,   M_GEN16),
    E(0xC0, TEXT_SUBB,  M_GEN8),
    E(0xC1, TEXT_CMPB,  M_GEN8),
    E(0xC2, TEXT_SBCB,  M_GEN8),
    E(0xC3, TEXT_ADDD,  M_GEN16),
    E(0xC4, TEXT_ANDB,  M_GEN8),
    E(0xC5, TEXT_BITB,  M_GEN8),
    E(0xC6, TEXT_LDB,   M_GEN8),
    U(0xC7, TEXT_STB,   M_IM8),
    E(0xC7, TEXT_STB,   M_GEN8),
    E(0xC8, TEXT_EORB,  M_GEN8),
    E(0xC9, TEXT_ADCB,  M_GEN8),
    E(0xCA, TEXT_ORB,   M_GEN8),
    E(0xCB, TEXT_ADDB,  M_GEN8),
    E(0xCC, TEXT_LDD,   M_GEN16),
    U(0xCD, TEXT_STD,   M_IM32),
    E(0xCD, TEXT_STD,   M_GEN16),
    E(0xCE, TEXT_LDU,   M_GEN16),
    U(0xCF, TEXT_STU,   M_IM32),
    E(0xCF, TEXT_STU,   M_GEN16),
};

static constexpr uint8_t MC6809_I00[] PROGMEM = {
     50,  // TEXT_ABX
     89,  // TEXT_ADCA
    108,  // TEXT_ADCB
     91,  // TEXT_ADDA
    110,  // TEXT_ADDB
    101,  // TEXT_ADDD
     83,  // TEXT_ANDA
    102,  // TEXT_ANDB
     19,  // TEXT_ANDCC
      5,  // TEXT_ASL
     60,  // TEXT_ASLA
     72,  // TEXT_ASLB
      4,  // TEXT_ASR
     59,  // TEXT_ASRA
     71,  // TEXT_ASRB
     28,  // TEXT_BCC
     30,  // TEXT_BCS
     32,  // TEXT_BEQ
     37,  // TEXT_BGE
     39,  // TEXT_BGT
     25,  // TEXT_BHI
     27,  // TEXT_BHS
     84,  // TEXT_BITA
    103,  // TEXT_BITB
     40,  // TEXT_BLE
     29,  // TEXT_BLO
     26,  // TEXT_BLS
     38,  // TEXT_BLT
     36,  // TEXT_BMI
     31,  // TEXT_BNE
     35,  // TEXT_BPL
     23,  // TEXT_BRA
     24,  // TEXT_BRN
     93,  // TEXT_BSR
     33,  // TEXT_BVC
     34,  // TEXT_BVS
     12,  // TEXT_CLR
     66,  // TEXT_CLRA
     78,  // TEXT_CLRB
     80,  // TEXT_CMPA
     99,  // TEXT_CMPB
     92,  // TEXT_CMPX
      1,  // TEXT_COM
     56,  // TEXT_COMA
     68,  // TEXT_COMB
     52,  // TEXT_CWAI
     17,  // TEXT_DAA
      8,  // TEXT_DEC
     63,  // TEXT_DECA
     75,  // TEXT_DECB
     88,  // TEXT_EORA
    107,  // TEXT_EORB
     21,  // TEXT_EXG
      9,  // TEXT_INC
     64,  // TEXT_INCA
     76,  // TEXT_INCB
     11,  // TEXT_JMP
     94,  // TEXT_JSR
     15,  // TEXT_LBRA
     16,  // TEXT_LBSR
     85,  // TEXT_LDA
    104,  // TEXT_LDB
    111,  // TEXT_LDD
    114,  // TEXT_LDU
     95,  // TEXT_LDX
     43,  // TEXT_LEAS
     44,  // TEXT_LEAU
     41,  // TEXT_LEAX
     42,  // TEXT_LEAY
      6,  // TEXT_LSL
     61,  // TEXT_LSLA
     73,  // TEXT_LSLB
      2,  // TEXT_LSR
     57,  // TEXT_LSRA
     69,  // TEXT_LSRB
     53,  // TEXT_MUL
      0,  // TEXT_NEG
     55,  // TEXT_NEGA
     67,  // TEXT_NEGB
     13,  // TEXT_NOP
     90,  // TEXT_ORA
    109,  // TEXT_ORB
     18,  // TEXT_ORCC
     45,  // TEXT_PSHS
     47,  // TEXT_PSHU
     46,  // TEXT_PULS
     48,  // TEXT_PULU
      7,  // TEXT_ROL
     62,  // TEXT_ROLA
     74,  // TEXT_ROLB
      3,  // TEXT_ROR
     58,  // TEXT_RORA
     70,  // TEXT_RORB
     51,  // TEXT_RTI
     49,  // TEXT_RTS
     81,  // TEXT_SBCA
    100,  // TEXT_SBCB
     20,  // TEXT_SEX
     86,  // TEXT_STA
     87,  // TEXT_STA
    105,  // TEXT_STB
    106,  // TEXT_STB
    112,  // TEXT_STD
    113,  // TEXT_STD
    115,  // TEXT_STU
    116,  // TEXT_STU
     96,  // TEXT_STX
     97,  // TEXT_STX
     79,  // TEXT_SUBA
     98,  // TEXT_SUBB
     82,  // TEXT_SUBD
     54,  // TEXT_SWI
     14,  // TEXT_SYNC
     22,  // TEXT_TFR
     10,  // TEXT_TST
     65,  // TEXT_TSTA
     77,  // TEXT_TSTB
};

static constexpr Entry MC6809_P10[] PROGMEM = {
    E(0x21, TEXT_LBRN,  M_LREL),
    E(0x22, TEXT_LBHI,  M_LREL),
    E(0x23, TEXT_LBLS,  M_LREL),
    E(0x24, TEXT_LBHS,  M_LREL),
    E(0x24, TEXT_LBCC,  M_LREL),
    E(0x25, TEXT_LBLO,  M_LREL),
    E(0x25, TEXT_LBCS,  M_LREL),
    E(0x26, TEXT_LBNE,  M_LREL),
    E(0x27, TEXT_LBEQ,  M_LREL),
    E(0x28, TEXT_LBVC,  M_LREL),
    E(0x29, TEXT_LBVS,  M_LREL),
    E(0x2A, TEXT_LBPL,  M_LREL),
    E(0x2B, TEXT_LBMI,  M_LREL),
    E(0x2C, TEXT_LBGE,  M_LREL),
    E(0x2D, TEXT_LBLT,  M_LREL),
    E(0x2E, TEXT_LBGT,  M_LREL),
    E(0x2F, TEXT_LBLE,  M_LREL),
    E(0x3F, TEXT_SWI2,  M_NONE),
    E(0x83, TEXT_CMPD,  M_GEN16),
    E(0x8C, TEXT_CMPY,  M_GEN16),
    E(0x8E, TEXT_LDY,   M_GEN16),
    U(0x8F, TEXT_STY,   M_IM32),
    E(0x8F, TEXT_STY,   M_GEN16),
    E(0xCE, TEXT_LDS,   M_GEN16),
    U(0xCF, TEXT_STS,   M_IM32),
    E(0xCF, TEXT_STS,   M_GEN16),
};

static constexpr uint8_t MC6809_I10[] PROGMEM = {
     18,  // TEXT_CMPD
     19,  // TEXT_CMPY
      4,  // TEXT_LBCC
      6,  // TEXT_LBCS
      8,  // TEXT_LBEQ
     13,  // TEXT_LBGE
     15,  // TEXT_LBGT
      1,  // TEXT_LBHI
      3,  // TEXT_LBHS
     16,  // TEXT_LBLE
      5,  // TEXT_LBLO
      2,  // TEXT_LBLS
     14,  // TEXT_LBLT
     12,  // TEXT_LBMI
      7,  // TEXT_LBNE
     11,  // TEXT_LBPL
      0,  // TEXT_LBRN
      9,  // TEXT_LBVC
     10,  // TEXT_LBVS
     23,  // TEXT_LDS
     20,  // TEXT_LDY
     24,  // TEXT_STS
     25,  // TEXT_STS
     21,  // TEXT_STY
     22,  // TEXT_STY
     17,  // TEXT_SWI2
};

static constexpr Entry MC6809_P11[] PROGMEM = {
    E(0x3F, TEXT_SWI3,  M_NONE),
    E(0x83, TEXT_CMPU,  M_GEN16),
    E(0x8C, TEXT_CMPS,  M_GEN16),
};

static constexpr uint8_t MC6809_I11[] PROGMEM = {
      2,  // TEXT_CMPS
      1,  // TEXT_CMPU
      0,  // TEXT_SWI3
};

static constexpr Entry HD6309_P00[] PROGMEM = {
    X(0x01, TEXT_OIM,   M_IM8,  M_GMEM),
    X(0x02, TEXT_AIM,   M_IM8,  M_GMEM),
    X(0x05, TEXT_EIM,   M_IM8,  M_GMEM),
    X(0x0B, TEXT_TIM,   M_IM8,  M_GMEM),
    E(0x14, TEXT_SEXW,  M_NONE),
    E(0xCD, TEXT_LDQ,   M_IM32),
};

static constexpr uint8_t HD6309_I00[] PROGMEM = {
      1,  // TEXT_AIM
      2,  // TEXT_EIM
      5,  // TEXT_LDQ
      0,  // TEXT_OIM
      4,  // TEXT_SEXW
      3,  // TEXT_TIM
};

static constexpr Entry HD6309_P10[] PROGMEM = {
    E(0x30, TEXT_ADDR,  M_PAIR),
    E(0x31, TEXT_ADCR,  M_PAIR),
    E(0x32, TEXT_SUBR,  M_PAIR),
    E(0x33, TEXT_SBCR,  M_PAIR),
    E(0x34, TEXT_ANDR,  M_PAIR),
    E(0x35, TEXT_ORR,   M_PAIR),
    E(0x36, TEXT_EORR,  M_PAIR),
    E(0x37, TEXT_CMPR,  M_PAIR),
    E(0x38, TEXT_PSHSW, M_NONE),
    E(0x39, TEXT_PULSW, M_NONE),
    E(0x3A, TEXT_PSHUW, M_NONE),
    E(0x3B, TEXT_PULUW, M_NONE),
    E(0x40, TEXT_NEGD,  M_NONE),
    E(0x43, TEXT_COMD,  M_NONE),
    E(0x44, TEXT_LSRD,  M_NONE),
    E(0x46, TEXT_RORD,  M_NONE),
    E(0x47, TEXT_ASRD,  M_NONE),
    E(0x48, TEXT_ASLD,  M_NONE),
    E(0x48, TEXT_LSLD,  M_NONE),
    E(0x49, TEXT_ROLD,  M_NONE),
    E(0x4A, TEXT_DECD,  M_NONE),
    E(0x4C, TEXT_INCD,  M_NONE),
    E(0x4D, TEXT_TSTD,  M_NONE),
    E(0x4F, TEXT_CLRD,  M_NONE),
    E(0x53, TEXT_COMW,  M_NONE),
    E(0x54, TEXT_LSRW,  M_NONE),
    E(0x56, TEXT_RORW,  M_NONE),
    E(0x59, TEXT_ROLW,  M_NONE),
    E(0x5A, TEXT_DECW,  M_NONE),
    E(0x5C, TEXT_INCW,  M_NONE),
    E(0x5D, TEXT_TSTW,  M_NONE),
    E(0x5F, TEXT_CLRW,  M_NONE),
    E(0x80, TEXT_SUBW,  M_GEN16),
    E(0x81, TEXT_CMPW,  M_GEN16),
    E(0x82, TEXT_SBCD,  M_GEN16),
    E(0x84, TEXT_ANDD,  M_GEN16),
    E(0x85, TEXT_BITD,  M_GEN16),
    E(0x86, TEXT_LDW,   M_GEN16),
    U(0x87, TEXT_STW,   M_IM32),
    E(0x87, TEXT_STW,   M_GEN16),
    E(0x88, TEXT_EORD,  M_GEN16),
    E(0x89, TEXT_ADCD,  M_GEN16),
    E(0x8A, TEXT_ORD,   M_GEN16),
    E(0x8B, TEXT_ADDW,  M_GEN16),
    U(0xCC, TEXT_LDQ,   M_IM32), // LDQ # is defined in HD6309_P00[]
    E(0xCC, TEXT_LDQ,   M_GEN16),
    U(0xCD, TEXT_STQ,   M_IM32),
    E(0xCD, TEXT_STQ,   M_GEN16),
};

static constexpr uint8_t HD6309_I10[] PROGMEM = {
     41,  // TEXT_ADCD
      1,  // TEXT_ADCR
      0,  // TEXT_ADDR
     43,  // TEXT_ADDW
     35,  // TEXT_ANDD
      4,  // TEXT_ANDR
     17,  // TEXT_ASLD
     16,  // TEXT_ASRD
     36,  // TEXT_BITD
     23,  // TEXT_CLRD
     31,  // TEXT_CLRW
      7,  // TEXT_CMPR
     33,  // TEXT_CMPW
     13,  // TEXT_COMD
     24,  // TEXT_COMW
     20,  // TEXT_DECD
     28,  // TEXT_DECW
     40,  // TEXT_EORD
      6,  // TEXT_EORR
     21,  // TEXT_INCD
     29,  // TEXT_INCW
     44,  // TEXT_LDQ
     45,  // TEXT_LDQ
     37,  // TEXT_LDW
     18,  // TEXT_LSLD
     14,  // TEXT_LSRD
     25,  // TEXT_LSRW
     12,  // TEXT_NEGD
     42,  // TEXT_ORD
      5,  // TEXT_ORR
      8,  // TEXT_PSHSW
     10,  // TEXT_PSHUW
      9,  // TEXT_PULSW
     11,  // TEXT_PULUW
     19,  // TEXT_ROLD
     27,  // TEXT_ROLW
     15,  // TEXT_RORD
     26,  // TEXT_RORW
     34,  // TEXT_SBCD
      3,  // TEXT_SBCR
     46,  // TEXT_STQ
     47,  // TEXT_STQ
     38,  // TEXT_STW
     39,  // TEXT_STW
      2,  // TEXT_SUBR
     32,  // TEXT_SUBW
     22,  // TEXT_TSTD
     30,  // TEXT_TSTW
};

static constexpr Entry HD6309_P11[] PROGMEM = {
    X(0x30, TEXT_BAND,  M_RBIT, M_DBIT),
    X(0x31, TEXT_BIAND, M_RBIT, M_DBIT),
    X(0x32, TEXT_BOR,   M_RBIT, M_DBIT),
    X(0x33, TEXT_BIOR,  M_RBIT, M_DBIT),
    X(0x34, TEXT_BEOR,  M_RBIT, M_DBIT),
    X(0x35, TEXT_BIEOR, M_RBIT, M_DBIT),
    X(0x36, TEXT_LDBT,  M_RBIT, M_DBIT),
    X(0x37, TEXT_STBT,  M_RBIT, M_DBIT),
    X(0x38, TEXT_TFM,   M_RTFM, M_RTFM),  /* R+,R+ */
    X(0x39, TEXT_TFM,   M_RTFM, M_RTFM),  /* R-,R- */
    X(0x3A, TEXT_TFM,   M_RTFM, M_RTFM),  /* R+,R */
    X(0x3B, TEXT_TFM,   M_RTFM, M_RTFM),  /* R,R+ */
    E(0x3C, TEXT_BITMD, M_IM8),
    E(0x3D, TEXT_LDMD,  M_IM8),
    E(0x43, TEXT_COME,  M_NONE),
    E(0x4A, TEXT_DECE,  M_NONE),
    E(0x4C, TEXT_INCE,  M_NONE),
    E(0x4D, TEXT_TSTE,  M_NONE),
    E(0x4F, TEXT_CLRE,  M_NONE),
    E(0x53, TEXT_COMF,  M_NONE),
    E(0x5A, TEXT_DECF,  M_NONE),
    E(0x5C, TEXT_INCF,  M_NONE),
    E(0x5D, TEXT_TSTF,  M_NONE),
    E(0x5F, TEXT_CLRF,  M_NONE),
    E(0x80, TEXT_SUBE,  M_GEN8),
    E(0x81, TEXT_CMPE,  M_GEN8),
    E(0x86, TEXT_LDE,   M_GEN8),
    U(0x87, TEXT_STE,   M_IM8),
    E(0x87, TEXT_STE,   M_GEN8),
    E(0x8B, TEXT_ADDE,  M_GEN8),
    E(0x8D, TEXT_DIVD,  M_GEN16),
    E(0x8E, TEXT_DIVQ,  M_GEN16),
    E(0x8F, TEXT_MULD,  M_GEN16),
    E(0xC0, TEXT_SUBF,  M_GEN8),
    E(0xC1, TEXT_CMPF,  M_GEN8),
    E(0xC6, TEXT_LDF,   M_GEN8),
    U(0xC7, TEXT_STF,   M_IM8),
    E(0xC7, TEXT_STF,   M_GEN8),
    E(0xCB, TEXT_ADDF,  M_GEN8),
};

static constexpr uint8_t HD6309_I11[] PROGMEM = {
     29,  // TEXT_ADDE
     38,  // TEXT_ADDF
      0,  // TEXT_BAND
      4,  // TEXT_BEOR
      1,  // TEXT_BIAND
      5,  // TEXT_BIEOR
      3,  // TEXT_BIOR
     12,  // TEXT_BITMD
      2,  // TEXT_BOR
     18,  // TEXT_CLRE
     23,  // TEXT_CLRF
     25,  // TEXT_CMPE
     34,  // TEXT_CMPF
     14,  // TEXT_COME
     19,  // TEXT_COMF
     15,  // TEXT_DECE
     20,  // TEXT_DECF
     30,  // TEXT_DIVD
     31,  // TEXT_DIVQ
     16,  // TEXT_INCE
     21,  // TEXT_INCF
      6,  // TEXT_LDBT
     26,  // TEXT_LDE
     35,  // TEXT_LDF
     13,  // TEXT_LDMD
     32,  // TEXT_MULD
      7,  // TEXT_STBT
     27,  // TEXT_STE
     28,  // TEXT_STE
     36,  // TEXT_STF
     37,  // TEXT_STF
     24,  // TEXT_SUBE
     33,  // TEXT_SUBF
      8,  // TEXT_TFM
      9,  // TEXT_TFM
     10,  // TEXT_TFM
     11,  // TEXT_TFM
     17,  // TEXT_TSTE
     22,  // TEXT_TSTF
};
// clang-format on

static constexpr Config::opcode_t PREFIX_P00 = 0x00;
static constexpr Config::opcode_t PREFIX_P10 = 0x10;
static constexpr Config::opcode_t PREFIX_P11 = 0x11;

bool TableMc6809::isPrefix(Config::opcode_t opCode) {
    return opCode == PREFIX_P10 || opCode == PREFIX_P11;
}

static constexpr TableMc6809::EntryPage MC6809_PAGES[] PROGMEM = {
        {PREFIX_P00, ARRAY_RANGE(MC6809_P00), ARRAY_RANGE(MC6809_I00)},
        {PREFIX_P10, ARRAY_RANGE(MC6809_P10), ARRAY_RANGE(MC6809_I10)},
        {PREFIX_P11, ARRAY_RANGE(MC6809_P11), ARRAY_RANGE(MC6809_I11)},
};

static constexpr TableMc6809::EntryPage HD6309_PAGES[] PROGMEM = {
        {PREFIX_P00, ARRAY_RANGE(HD6309_P00), ARRAY_RANGE(HD6309_I00)},
        {PREFIX_P00, ARRAY_RANGE(MC6809_P00), ARRAY_RANGE(MC6809_I00)},
        {PREFIX_P10, ARRAY_RANGE(MC6809_P10), ARRAY_RANGE(MC6809_I10)},
        {PREFIX_P11, ARRAY_RANGE(MC6809_P11), ARRAY_RANGE(MC6809_I11)},
        {PREFIX_P10, ARRAY_RANGE(HD6309_P10), ARRAY_RANGE(HD6309_I10)},
        {PREFIX_P11, ARRAY_RANGE(HD6309_P11), ARRAY_RANGE(HD6309_I11)},
};

struct TableMc6809::PostEntry : PostSpec {
    uint8_t mask;
    uint8_t byte;

    constexpr PostEntry(
            RegName _index, RegName _base, int8_t _size, bool _indir, uint8_t _mask, uint8_t _byte)
        : PostSpec(_index, _base, _size, _indir), mask(_mask), byte(_byte) {}
};

static constexpr TableMc6809::PostEntry MC6809_POSTBYTE[] PROGMEM = {
        {REG_UNDEF, REG_X, 0, true, 0x8F, 0x84},       // ,X [,X]
        {REG_UNDEF, REG_X, 5, false, 0x80, 0x00},      // n5,X
        {REG_UNDEF, REG_X, 8, true, 0x8F, 0x88},       // n8,X [n8,X]
        {REG_UNDEF, REG_X, 16, true, 0x8F, 0x89},      // n16,X [n16,X]
        {REG_A, REG_X, 0, true, 0x8F, 0x86},           // A,X [A,X]
        {REG_B, REG_X, 0, true, 0x8F, 0x85},           // B,X [B,X]
        {REG_D, REG_X, 0, true, 0x8F, 0x8B},           // D,X [D,X]
        {REG_UNDEF, REG_X, 1, false, 0x9F, 0x80},      // ,X+
        {REG_UNDEF, REG_X, 2, true, 0x8F, 0x81},       // ,X++ [,X++]
        {REG_UNDEF, REG_X, -1, false, 0x9F, 0x82},     // ,-X
        {REG_UNDEF, REG_X, -2, true, 0x8F, 0x83},      // ,--X [,--X]
        {REG_UNDEF, REG_PCR, 8, true, 0x8F, 0x8C},     // n8,PCR [n8,PCR]
        {REG_UNDEF, REG_PCR, 16, true, 0x8F, 0x8D},    // n16,PCR [n16,PCR]
        {REG_UNDEF, REG_UNDEF, 16, true, 0xFF, 0x9F},  // [n16]
};

static constexpr TableMc6809::PostEntry HD6309_POSTBYTE[] PROGMEM = {
        {REG_E, REG_X, 0, true, 0x8F, 0x87},        // E,X [E,X]
        {REG_F, REG_X, 0, true, 0x8F, 0x8A},        // F,X [F,X]
        {REG_W, REG_X, 0, true, 0x8F, 0x8E},        // W,X [W,X]
        {REG_UNDEF, REG_W, 0, false, 0xFF, 0x8F},   // ,W
        {REG_UNDEF, REG_W, 0, true, 0xFF, 0x90},    // [,W]
        {REG_UNDEF, REG_W, 16, false, 0xFF, 0xAF},  // n16,W
        {REG_UNDEF, REG_W, 16, true, 0xFF, 0xB0},   // [n16,W]
        {REG_UNDEF, REG_W, 2, false, 0xFF, 0xCF},   // ,W++
        {REG_UNDEF, REG_W, 2, true, 0xFF, 0xD0},    // [,W++]
        {REG_UNDEF, REG_W, -2, false, 0xFF, 0xEF},  // ,--W
        {REG_UNDEF, REG_W, -2, true, 0xFF, 0xF0},   // [,--W]
};

typedef PageBase<TableMc6809::PostEntry> PostPage;

static constexpr PostPage MC6809_POSTS[] PROGMEM = {
        {ARRAY_RANGE(MC6809_POSTBYTE)},
};

static constexpr PostPage HD6309_POSTS[] PROGMEM = {
        {ARRAY_RANGE(MC6809_POSTBYTE)},
        {ARRAY_RANGE(HD6309_POSTBYTE)},
};

struct TableMc6809::Cpu : CpuBase<CpuType, TableMc6809::EntryPage> {
    const PostPage *postTable() const {
        return reinterpret_cast<const PostPage *>(pgm_read_ptr(&_post_table));
    }
    const PostPage *postEnd() const {
        return reinterpret_cast<const PostPage *>(pgm_read_ptr(&_post_end));
    }
    constexpr Cpu(CpuType cpuType, const char *name, const TableMc6809::EntryPage *table,
            const TableMc6809::EntryPage *end, const PostPage *postTable, const PostPage *postEnd)
        : CpuBase(cpuType, name, table, end), _post_table(postTable), _post_end(postEnd) {}

    static const Cpu *search(CpuType cpuType, const Cpu *table, const Cpu *end) {
        for (const auto *t = table; t < end; t++) {
            if (cpuType == t->cpuType())
                return t;
        }
        return nullptr;
    }

    static const Cpu *search(const char *name, const Cpu *table, const Cpu *end) {
        for (const auto *t = table; t < end; t++) {
            if (strcasecmp_P(name, t->name_P()) == 0)
                return t;
        }
        return nullptr;
    }

private:
    const PostPage *_post_table;
    const PostPage *_post_end;
};

static constexpr TableMc6809::Cpu CPU_TABLES[] PROGMEM = {
        {MC6809, TEXT_CPU_6809, ARRAY_RANGE(MC6809_PAGES), ARRAY_RANGE(MC6809_POSTS)},
        {HD6309, TEXT_CPU_6309, ARRAY_RANGE(HD6309_PAGES), ARRAY_RANGE(HD6309_POSTS)},
};

static bool matchAddrMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (table == M_GEN8 || table == M_GEN16)
        return opr == M_IM32 || opr == M_IDX || opr == M_LIST || opr == M_PAIR || opr == M_DIR ||
               opr == M_EXT;
    if (table == M_GMEM)
        return opr == M_IDX || opr == M_LIST || opr == M_PAIR || opr == M_DIR || opr == M_EXT;
    if (opr == M_IM32)
        return table == M_IM8 || table == M_LIST;  // immediate register list
    if (opr == M_PAIR)
        return table == M_IDX       // A,X
               || table == M_LIST;  // 2-length register list
    if (opr == M_RBIT)
        return table == M_PAIR;  // A,0
    if (opr == M_DIR)
        return table == M_REL || table == M_LREL;
    if (opr == M_EXT)
        return table == M_REL || table == M_LREL;
    if (opr == M_LIST)
        return table == M_RTFM;  // X
    if (opr == M_NONE)
        return table == M_LIST;  // empty register list
    return false;
}

static bool matchAddrMode(Entry::Flags flags, const Entry *entry) {
    auto table = entry->flags();
    return matchAddrMode(flags.mode1(), table.mode1()) &&
           matchAddrMode(flags.mode2(), table.mode2());
}

Error TableMc6809::searchName(InsnMc6809 &insn) {
    uint8_t count = 0;
    for (auto page = _cpu->table(); page < _cpu->end(); page++) {
        auto entry = TableBase::searchName<EntryPage, Entry, Entry::Flags>(
                insn.name(), insn.flags(), page, matchAddrMode, count);
        if (entry) {
            insn.setOpCode(entry->opCode(), page->prefix());
            insn.setFlags(entry->flags());
            if (insn.undefined())
                return setError(OPERAND_NOT_ALLOWED);
            return setOK();
        }
    }
    return setError(count == 0 ? UNKNOWN_INSTRUCTION : OPERAND_NOT_ALLOWED);
}

static Config::opcode_t maskCode(Config::opcode_t code, const Entry *entry) {
    const auto &flags = entry->flags();
    const auto mode1 = flags.mode1();
    if (mode1 == M_GEN8 || mode1 == M_GEN16)
        return code & ~0x30;
    if (mode1 == M_GMEM || flags.mode2() == M_GMEM) {
        const auto opc = code & 0xF0;
        if (opc == 0x00 || opc == 0x60 || opc == 0x70)
            return code & ~0xF0;
    }
    return code;
}

Error TableMc6809::searchOpCode(InsnMc6809 &insn) {
    for (auto page = _cpu->table(); page < _cpu->end(); page++) {
        if (insn.prefix() != page->prefix())
            continue;
        auto entry = TableBase::searchCode<Entry, Config::opcode_t>(
                insn.opCode(), page->table(), page->end(), maskCode);
        if (entry) {
            insn.setFlags(entry->flags());
            insn.setName_P(entry->name_P());
            if (insn.undefined())
                break;
            return setOK();
        }
    }
    return setError(UNKNOWN_INSTRUCTION);
}

Error TableMc6809::searchPostByte(const uint8_t post, PostSpec &spec) const {
    for (auto page = _cpu->postTable(); page < _cpu->postEnd(); page++) {
        for (auto entry = page->table(); entry < page->end(); entry++) {
            auto mask = pgm_read_byte(&entry->mask);
            auto byte = post & mask;
            if (byte == pgm_read_byte(&entry->byte)) {
                spec.index = RegName(pgm_read_byte(&entry->index));
                spec.base = RegName(pgm_read_byte(&entry->base));
                spec.size = pgm_read_byte(&entry->size);
                spec.indir = (mask != 0x80) && (post & 0x10);
                return OK;
            }
        }
    }
    return UNKNOWN_POSTBYTE;
}

static RegName baseRegName(const RegName base) {
    if (base == REG_X || base == REG_W || base == REG_PCR)
        return base;
    if (base == REG_Y || base == REG_U || base == REG_S)
        return REG_X;
    if (base == REG_PC)
        return REG_PCR;
    return REG_UNDEF;
}

int16_t TableMc6809::searchPostSpec(PostSpec &spec) const {
    auto specBase = baseRegName(spec.base);
    for (auto page = _cpu->postTable(); page < _cpu->postEnd(); page++) {
        for (auto entry = page->table(); entry < page->end(); entry++) {
            auto base = RegName(pgm_read_byte(&entry->base));
            auto size = static_cast<int8_t>(pgm_read_byte(&entry->size));
            if (specBase == base && spec.size == size &&
                    spec.index == RegName(pgm_read_byte(&entry->index))) {
                auto byte = pgm_read_byte(&entry->byte);
                if (spec.indir) {
                    if (pgm_read_byte(&entry->indir) == 0)
                        continue;
                    byte |= 0x10;
                }
                spec.base = base;
                return byte;
            }
        }
    }
    return -1;
}

TableMc6809::TableMc6809() {
    setCpu(MC6809);
}

bool TableMc6809::setCpu(CpuType cpuType) {
    auto t = Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLES));
    if (t == nullptr)
        return false;
    _cpu = t;
    return true;
}

CpuType TableMc6809::cpuType() const {
    return _cpu->cpuType();
}

const /* PROGMEM */ char *TableMc6809::listCpu_P() const {
    return TEXT_CPU_LIST;
}

const /* PROGMEM */ char *TableMc6809::cpu_P() const {
    return _cpu->name_P();
}

bool TableMc6809::setCpu(const char *cpu) {
    auto t = Cpu::search(cpu, ARRAY_RANGE(CPU_TABLES));
    if (t)
        return setCpu(t->cpuType());
    if (strcasecmp_P(cpu, TEXT_CPU_MC6809) == 0)
        return setCpu(MC6809);
    if (strcasecmp_P(cpu, TEXT_CPU_HD6309) == 0)
        return setCpu(HD6309);
    return false;
}

class TableMc6809 TableMc6809;

}  // namespace mc6809
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

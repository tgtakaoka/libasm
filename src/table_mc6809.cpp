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

#define X(_opc, _name, _mode, _extra) \
    { _opc, Entry::Flags::create(_mode, _extra), _name }
#define E(_opc, _name, _mode) X(_opc, _name, _mode, NONE)

// clang-format off
static constexpr Entry MC6809_P00[] PROGMEM = {
    E(0x00, TEXT_NEG,   DIR),
    E(0x03, TEXT_COM,   DIR),
    E(0x04, TEXT_LSR,   DIR),
    E(0x06, TEXT_ROR,   DIR),
    E(0x07, TEXT_ASR,   DIR),
    E(0x08, TEXT_ASL,   DIR),
    E(0x08, TEXT_LSL,   DIR),
    E(0x09, TEXT_ROL,   DIR),
    E(0x0A, TEXT_DEC,   DIR),
    E(0x0C, TEXT_INC,   DIR),
    E(0x0D, TEXT_TST,   DIR),
    E(0x0E, TEXT_JMP,   DIR),
    E(0x0F, TEXT_CLR,   DIR),
    E(0x12, TEXT_NOP,   NONE),
    E(0x13, TEXT_SYNC,  NONE),
    E(0x16, TEXT_LBRA,  LREL),
    E(0x17, TEXT_LBSR,  LREL),
    E(0x19, TEXT_DAA,   NONE),
    E(0x1A, TEXT_ORCC,  IM8),
    E(0x1C, TEXT_ANDCC, IM8),
    E(0x1D, TEXT_SEX,   NONE),
    E(0x1E, TEXT_EXG,   REG_REG),
    E(0x1F, TEXT_TFR,   REG_REG),
    E(0x20, TEXT_BRA,   REL),
    E(0x21, TEXT_BRN,   REL),
    E(0x22, TEXT_BHI,   REL),
    E(0x23, TEXT_BLS,   REL),
    E(0x24, TEXT_BHS,   REL),
    E(0x24, TEXT_BCC,   REL),
    E(0x25, TEXT_BLO,   REL),
    E(0x25, TEXT_BCS,   REL),
    E(0x26, TEXT_BNE,   REL),
    E(0x27, TEXT_BEQ,   REL),
    E(0x28, TEXT_BVC,   REL),
    E(0x29, TEXT_BVS,   REL),
    E(0x2A, TEXT_BPL,   REL),
    E(0x2B, TEXT_BMI,   REL),
    E(0x2C, TEXT_BGE,   REL),
    E(0x2D, TEXT_BLT,   REL),
    E(0x2E, TEXT_BGT,   REL),
    E(0x2F, TEXT_BLE,   REL),
    E(0x30, TEXT_LEAX,  IDX),
    E(0x31, TEXT_LEAY,  IDX),
    E(0x32, TEXT_LEAS,  IDX),
    E(0x33, TEXT_LEAU,  IDX),
    E(0x34, TEXT_PSHS,  REGLIST),
    E(0x35, TEXT_PULS,  REGLIST),
    E(0x36, TEXT_PSHU,  REGLIST),
    E(0x37, TEXT_PULU,  REGLIST),
    E(0x39, TEXT_RTS,   NONE),
    E(0x3A, TEXT_ABX,   NONE),
    E(0x3B, TEXT_RTI,   NONE),
    E(0x3C, TEXT_CWAI,  IM8),
    E(0x3D, TEXT_MUL,   NONE),
    E(0x3F, TEXT_SWI,   NONE),
    E(0x40, TEXT_NEGA,  NONE),
    E(0x43, TEXT_COMA,  NONE),
    E(0x44, TEXT_LSRA,  NONE),
    E(0x46, TEXT_RORA,  NONE),
    E(0x47, TEXT_ASRA,  NONE),
    E(0x48, TEXT_ASLA,  NONE),
    E(0x48, TEXT_LSLA,  NONE),
    E(0x49, TEXT_ROLA,  NONE),
    E(0x4A, TEXT_DECA,  NONE),
    E(0x4C, TEXT_INCA,  NONE),
    E(0x4D, TEXT_TSTA,  NONE),
    E(0x4F, TEXT_CLRA,  NONE),
    E(0x50, TEXT_NEGB,  NONE),
    E(0x53, TEXT_COMB,  NONE),
    E(0x54, TEXT_LSRB,  NONE),
    E(0x56, TEXT_RORB,  NONE),
    E(0x57, TEXT_ASRB,  NONE),
    E(0x58, TEXT_ASLB,  NONE),
    E(0x58, TEXT_LSLB,  NONE),
    E(0x59, TEXT_ROLB,  NONE),
    E(0x5A, TEXT_DECB,  NONE),
    E(0x5C, TEXT_INCB,  NONE),
    E(0x5D, TEXT_TSTB,  NONE),
    E(0x5F, TEXT_CLRB,  NONE),
    E(0x60, TEXT_NEG,   IDX),
    E(0x63, TEXT_COM,   IDX),
    E(0x64, TEXT_LSR,   IDX),
    E(0x66, TEXT_ROR,   IDX),
    E(0x67, TEXT_ASR,   IDX),
    E(0x68, TEXT_ASL,   IDX),
    E(0x68, TEXT_LSL,   IDX),
    E(0x69, TEXT_ROL,   IDX),
    E(0x6A, TEXT_DEC,   IDX),
    E(0x6C, TEXT_INC,   IDX),
    E(0x6D, TEXT_TST,   IDX),
    E(0x6E, TEXT_JMP,   IDX),
    E(0x6F, TEXT_CLR,   IDX),
    E(0x70, TEXT_NEG,   EXT),
    E(0x73, TEXT_COM,   EXT),
    E(0x74, TEXT_LSR,   EXT),
    E(0x76, TEXT_ROR,   EXT),
    E(0x77, TEXT_ASR,   EXT),
    E(0x78, TEXT_ASL,   EXT),
    E(0x78, TEXT_LSL,   EXT),
    E(0x79, TEXT_ROL,   EXT),
    E(0x7A, TEXT_DEC,   EXT),
    E(0x7C, TEXT_INC,   EXT),
    E(0x7D, TEXT_TST,   EXT),
    E(0x7E, TEXT_JMP,   EXT),
    E(0x7F, TEXT_CLR,   EXT),
    E(0x80, TEXT_SUBA,  IM8),
    E(0x81, TEXT_CMPA,  IM8),
    E(0x82, TEXT_SBCA,  IM8),
    E(0x83, TEXT_SUBD,  IM16),
    E(0x84, TEXT_ANDA,  IM8),
    E(0x85, TEXT_BITA,  IM8),
    E(0x86, TEXT_LDA,   IM8),
    E(0x88, TEXT_EORA,  IM8),
    E(0x89, TEXT_ADCA,  IM8),
    E(0x8A, TEXT_ORA,   IM8),
    E(0x8B, TEXT_ADDA,  IM8),
    E(0x8C, TEXT_CMPX,  IM16),
    E(0x8D, TEXT_BSR,   REL),
    E(0x8E, TEXT_LDX,   IM16),
    E(0x90, TEXT_SUBA,  DIR),
    E(0x91, TEXT_CMPA,  DIR),
    E(0x92, TEXT_SBCA,  DIR),
    E(0x93, TEXT_SUBD,  DIR),
    E(0x94, TEXT_ANDA,  DIR),
    E(0x95, TEXT_BITA,  DIR),
    E(0x96, TEXT_LDA,   DIR),
    E(0x97, TEXT_STA,   DIR),
    E(0x98, TEXT_EORA,  DIR),
    E(0x99, TEXT_ADCA,  DIR),
    E(0x9A, TEXT_ORA,   DIR),
    E(0x9B, TEXT_ADDA,  DIR),
    E(0x9C, TEXT_CMPX,  DIR),
    E(0x9D, TEXT_JSR,   DIR),
    E(0x9E, TEXT_LDX,   DIR),
    E(0x9F, TEXT_STX,   DIR),
    E(0xA0, TEXT_SUBA,  IDX),
    E(0xA1, TEXT_CMPA,  IDX),
    E(0xA2, TEXT_SBCA,  IDX),
    E(0xA3, TEXT_SUBD,  IDX),
    E(0xA4, TEXT_ANDA,  IDX),
    E(0xA5, TEXT_BITA,  IDX),
    E(0xA6, TEXT_LDA,   IDX),
    E(0xA7, TEXT_STA,   IDX),
    E(0xA8, TEXT_EORA,  IDX),
    E(0xA9, TEXT_ADCA,  IDX),
    E(0xAA, TEXT_ORA,   IDX),
    E(0xAB, TEXT_ADDA,  IDX),
    E(0xAC, TEXT_CMPX,  IDX),
    E(0xAD, TEXT_JSR,   IDX),
    E(0xAE, TEXT_LDX,   IDX),
    E(0xAF, TEXT_STX,   IDX),
    E(0xB0, TEXT_SUBA,  EXT),
    E(0xB1, TEXT_CMPA,  EXT),
    E(0xB2, TEXT_SBCA,  EXT),
    E(0xB3, TEXT_SUBD,  EXT),
    E(0xB4, TEXT_ANDA,  EXT),
    E(0xB5, TEXT_BITA,  EXT),
    E(0xB6, TEXT_LDA,   EXT),
    E(0xB7, TEXT_STA,   EXT),
    E(0xB8, TEXT_EORA,  EXT),
    E(0xB9, TEXT_ADCA,  EXT),
    E(0xBA, TEXT_ORA,   EXT),
    E(0xBB, TEXT_ADDA,  EXT),
    E(0xBC, TEXT_CMPX,  EXT),
    E(0xBD, TEXT_JSR,   EXT),
    E(0xBE, TEXT_LDX,   EXT),
    E(0xBF, TEXT_STX,   EXT),
    E(0xC0, TEXT_SUBB,  IM8),
    E(0xC1, TEXT_CMPB,  IM8),
    E(0xC2, TEXT_SBCB,  IM8),
    E(0xC3, TEXT_ADDD,  IM16),
    E(0xC4, TEXT_ANDB,  IM8),
    E(0xC5, TEXT_BITB,  IM8),
    E(0xC6, TEXT_LDB,   IM8),
    E(0xC8, TEXT_EORB,  IM8),
    E(0xC9, TEXT_ADCB,  IM8),
    E(0xCA, TEXT_ORB,   IM8),
    E(0xCB, TEXT_ADDB,  IM8),
    E(0xCC, TEXT_LDD,   IM16),
    E(0xCE, TEXT_LDU,   IM16),
    E(0xD0, TEXT_SUBB,  DIR),
    E(0xD1, TEXT_CMPB,  DIR),
    E(0xD2, TEXT_SBCB,  DIR),
    E(0xD3, TEXT_ADDD,  DIR),
    E(0xD4, TEXT_ANDB,  DIR),
    E(0xD5, TEXT_BITB,  DIR),
    E(0xD6, TEXT_LDB,   DIR),
    E(0xD7, TEXT_STB,   DIR),
    E(0xD8, TEXT_EORB,  DIR),
    E(0xD9, TEXT_ADCB,  DIR),
    E(0xDA, TEXT_ORB,   DIR),
    E(0xDB, TEXT_ADDB,  DIR),
    E(0xDC, TEXT_LDD,   DIR),
    E(0xDD, TEXT_STD,   DIR),
    E(0xDE, TEXT_LDU,   DIR),
    E(0xDF, TEXT_STU,   DIR),
    E(0xE0, TEXT_SUBB,  IDX),
    E(0xE1, TEXT_CMPB,  IDX),
    E(0xE2, TEXT_SBCB,  IDX),
    E(0xE3, TEXT_ADDD,  IDX),
    E(0xE4, TEXT_ANDB,  IDX),
    E(0xE5, TEXT_BITB,  IDX),
    E(0xE6, TEXT_LDB,   IDX),
    E(0xE7, TEXT_STB,   IDX),
    E(0xE8, TEXT_EORB,  IDX),
    E(0xE9, TEXT_ADCB,  IDX),
    E(0xEA, TEXT_ORB,   IDX),
    E(0xEB, TEXT_ADDB,  IDX),
    E(0xEC, TEXT_LDD,   IDX),
    E(0xED, TEXT_STD,   IDX),
    E(0xEE, TEXT_LDU,   IDX),
    E(0xEF, TEXT_STU,   IDX),
    E(0xF0, TEXT_SUBB,  EXT),
    E(0xF1, TEXT_CMPB,  EXT),
    E(0xF2, TEXT_SBCB,  EXT),
    E(0xF3, TEXT_ADDD,  EXT),
    E(0xF4, TEXT_ANDB,  EXT),
    E(0xF5, TEXT_BITB,  EXT),
    E(0xF6, TEXT_LDB,   EXT),
    E(0xF7, TEXT_STB,   EXT),
    E(0xF8, TEXT_EORB,  EXT),
    E(0xF9, TEXT_ADCB,  EXT),
    E(0xFA, TEXT_ORB,   EXT),
    E(0xFB, TEXT_ADDB,  EXT),
    E(0xFC, TEXT_LDD,   EXT),
    E(0xFD, TEXT_STD,   EXT),
    E(0xFE, TEXT_LDU,   EXT),
    E(0xFF, TEXT_STU,   EXT),
};

static constexpr Entry MC6809_P10[] PROGMEM = {
    E(0x21, TEXT_LBRN,  LREL),
    E(0x22, TEXT_LBHI,  LREL),
    E(0x23, TEXT_LBLS,  LREL),
    E(0x24, TEXT_LBHS,  LREL),
    E(0x24, TEXT_LBCC,  LREL),
    E(0x25, TEXT_LBLO,  LREL),
    E(0x25, TEXT_LBCS,  LREL),
    E(0x26, TEXT_LBNE,  LREL),
    E(0x27, TEXT_LBEQ,  LREL),
    E(0x28, TEXT_LBVC,  LREL),
    E(0x29, TEXT_LBVS,  LREL),
    E(0x2A, TEXT_LBPL,  LREL),
    E(0x2B, TEXT_LBMI,  LREL),
    E(0x2C, TEXT_LBGE,  LREL),
    E(0x2D, TEXT_LBLT,  LREL),
    E(0x2E, TEXT_LBGT,  LREL),
    E(0x2F, TEXT_LBLE,  LREL),
    E(0x3F, TEXT_SWI2,  NONE),
    E(0x83, TEXT_CMPD,  IM16),
    E(0x8C, TEXT_CMPY,  IM16),
    E(0x8E, TEXT_LDY,   IM16),
    E(0x93, TEXT_CMPD,  DIR),
    E(0x9C, TEXT_CMPY,  DIR),
    E(0x9E, TEXT_LDY,   DIR),
    E(0x9F, TEXT_STY,   DIR),
    E(0xA3, TEXT_CMPD,  IDX),
    E(0xAC, TEXT_CMPY,  IDX),
    E(0xAE, TEXT_LDY,   IDX),
    E(0xAF, TEXT_STY,   IDX),
    E(0xB3, TEXT_CMPD,  EXT),
    E(0xBC, TEXT_CMPY,  EXT),
    E(0xBE, TEXT_LDY,   EXT),
    E(0xBF, TEXT_STY,   EXT),
    E(0xCE, TEXT_LDS,   IM16),
    E(0xDE, TEXT_LDS,   DIR),
    E(0xDF, TEXT_STS,   DIR),
    E(0xEE, TEXT_LDS,   IDX),
    E(0xEF, TEXT_STS,   IDX),
    E(0xFE, TEXT_LDS,   EXT),
    E(0xFF, TEXT_STS,   EXT),
};

static constexpr Entry MC6809_P11[] PROGMEM = {
    E(0x3F, TEXT_SWI3,  NONE),
    E(0x83, TEXT_CMPU,  IM16),
    E(0x8C, TEXT_CMPS,  IM16),
    E(0x93, TEXT_CMPU,  DIR),
    E(0x9C, TEXT_CMPS,  DIR),
    E(0xA3, TEXT_CMPU,  IDX),
    E(0xAC, TEXT_CMPS,  IDX),
    E(0xB3, TEXT_CMPU,  EXT),
    E(0xBC, TEXT_CMPS,  EXT),
};

static constexpr Entry HD6309_P00[] PROGMEM = {
    X(0x01, TEXT_OIM,   IM8,  DIR),
    X(0x02, TEXT_AIM,   IM8,  DIR),
    X(0x05, TEXT_EIM,   IM8,  DIR),
    X(0x0B, TEXT_TIM,   IM8,  DIR),
    E(0x14, TEXT_SEXW,  NONE),
    X(0x61, TEXT_OIM,   IM8,  IDX),
    X(0x62, TEXT_AIM,   IM8,  IDX),
    X(0x65, TEXT_EIM,   IM8,  IDX),
    X(0x6B, TEXT_TIM,   IM8,  IDX),
    X(0x71, TEXT_OIM,   IM8,  EXT),
    X(0x72, TEXT_AIM,   IM8,  EXT),
    X(0x75, TEXT_EIM,   IM8,  EXT),
    X(0x7B, TEXT_TIM,   IM8,  EXT),
    E(0xCD, TEXT_LDQ,   IM32),
};

static constexpr Entry HD6309_P10[] PROGMEM = {
    E(0x30, TEXT_ADDR,  REG_REG),
    E(0x31, TEXT_ADCR,  REG_REG),
    E(0x32, TEXT_SUBR,  REG_REG),
    E(0x33, TEXT_SBCR,  REG_REG),
    E(0x34, TEXT_ANDR,  REG_REG),
    E(0x35, TEXT_ORR,   REG_REG),
    E(0x36, TEXT_EORR,  REG_REG),
    E(0x37, TEXT_CMPR,  REG_REG),
    E(0x38, TEXT_PSHSW, NONE),
    E(0x39, TEXT_PULSW, NONE),
    E(0x3A, TEXT_PSHUW, NONE),
    E(0x3B, TEXT_PULUW, NONE),
    E(0x40, TEXT_NEGD,  NONE),
    E(0x43, TEXT_COMD,  NONE),
    E(0x44, TEXT_LSRD,  NONE),
    E(0x46, TEXT_RORD,  NONE),
    E(0x47, TEXT_ASRD,  NONE),
    E(0x48, TEXT_ASLD,  NONE),
    E(0x48, TEXT_LSLD,  NONE),
    E(0x49, TEXT_ROLD,  NONE),
    E(0x4A, TEXT_DECD,  NONE),
    E(0x4C, TEXT_INCD,  NONE),
    E(0x4D, TEXT_TSTD,  NONE),
    E(0x4F, TEXT_CLRD,  NONE),
    E(0x53, TEXT_COMW,  NONE),
    E(0x54, TEXT_LSRW,  NONE),
    E(0x56, TEXT_RORW,  NONE),
    E(0x59, TEXT_ROLW,  NONE),
    E(0x5A, TEXT_DECW,  NONE),
    E(0x5C, TEXT_INCW,  NONE),
    E(0x5D, TEXT_TSTW,  NONE),
    E(0x5F, TEXT_CLRW,  NONE),
    E(0x80, TEXT_SUBW,  IM16),
    E(0x81, TEXT_CMPW,  IM16),
    E(0x82, TEXT_SBCD,  IM16),
    E(0x84, TEXT_ANDD,  IM16),
    E(0x85, TEXT_BITD,  IM16),
    E(0x86, TEXT_LDW,   IM16),
    E(0x88, TEXT_EORD,  IM16),
    E(0x89, TEXT_ADCD,  IM16),
    E(0x8A, TEXT_ORD,   IM16),
    E(0x8B, TEXT_ADDW,  IM16),
    E(0x90, TEXT_SUBW,  DIR),
    E(0x91, TEXT_CMPW,  DIR),
    E(0x92, TEXT_SBCD,  DIR),
    E(0x94, TEXT_ANDD,  DIR),
    E(0x95, TEXT_BITD,  DIR),
    E(0x96, TEXT_LDW,   DIR),
    E(0x97, TEXT_STW,   DIR),
    E(0x98, TEXT_EORD,  DIR),
    E(0x99, TEXT_ADCD,  DIR),
    E(0x9A, TEXT_ORD,   DIR),
    E(0x9B, TEXT_ADDW,  DIR),
    E(0xA0, TEXT_SUBW,  IDX),
    E(0xA1, TEXT_CMPW,  IDX),
    E(0xA2, TEXT_SBCD,  IDX),
    E(0xA4, TEXT_ANDD,  IDX),
    E(0xA5, TEXT_BITD,  IDX),
    E(0xA6, TEXT_LDW,   IDX),
    E(0xA7, TEXT_STW,   IDX),
    E(0xA8, TEXT_EORD,  IDX),
    E(0xA9, TEXT_ADCD,  IDX),
    E(0xAA, TEXT_ORD,   IDX),
    E(0xAB, TEXT_ADDW,  IDX),
    E(0xB0, TEXT_SUBW,  EXT),
    E(0xB1, TEXT_CMPW,  EXT),
    E(0xB2, TEXT_SBCD,  EXT),
    E(0xB4, TEXT_ANDD,  EXT),
    E(0xB5, TEXT_BITD,  EXT),
    E(0xB6, TEXT_LDW,   EXT),
    E(0xB7, TEXT_STW,   EXT),
    E(0xB8, TEXT_EORD,  EXT),
    E(0xB9, TEXT_ADCD,  EXT),
    E(0xBA, TEXT_ORD,   EXT),
    E(0xBB, TEXT_ADDW,  EXT),
    E(0xDC, TEXT_LDQ,   DIR),
    E(0xDD, TEXT_STQ,   DIR),
    E(0xEC, TEXT_LDQ,   IDX),
    E(0xED, TEXT_STQ,   IDX),
    E(0xFC, TEXT_LDQ,   EXT),
    E(0xFD, TEXT_STQ,   EXT),
};

static constexpr Entry HD6309_P11[] PROGMEM = {
    X(0x30, TEXT_BAND,  REG_BIT, DIR_BIT),
    X(0x31, TEXT_BIAND, REG_BIT, DIR_BIT),
    X(0x32, TEXT_BOR,   REG_BIT, DIR_BIT),
    X(0x33, TEXT_BIOR,  REG_BIT, DIR_BIT),
    X(0x34, TEXT_BEOR,  REG_BIT, DIR_BIT),
    X(0x35, TEXT_BIEOR, REG_BIT, DIR_BIT),
    X(0x36, TEXT_LDBT,  REG_BIT, DIR_BIT),
    X(0x37, TEXT_STBT,  REG_BIT, DIR_BIT),
    X(0x38, TEXT_TFM,   REG_TFM, REG_TFM),  /* R+,R+ */
    X(0x39, TEXT_TFM,   REG_TFM, REG_TFM),  /* R-,R- */
    X(0x3A, TEXT_TFM,   REG_TFM, REG_TFM),  /* R+,R */
    X(0x3B, TEXT_TFM,   REG_TFM, REG_TFM),  /* R,R+ */
    E(0x3C, TEXT_BITMD, IM8),
    E(0x3D, TEXT_LDMD,  IM8),
    E(0x43, TEXT_COME,  NONE),
    E(0x4A, TEXT_DECE,  NONE),
    E(0x4C, TEXT_INCE,  NONE),
    E(0x4D, TEXT_TSTE,  NONE),
    E(0x4F, TEXT_CLRE,  NONE),
    E(0x53, TEXT_COMF,  NONE),
    E(0x5A, TEXT_DECF,  NONE),
    E(0x5C, TEXT_INCF,  NONE),
    E(0x5D, TEXT_TSTF,  NONE),
    E(0x5F, TEXT_CLRF,  NONE),
    E(0x80, TEXT_SUBE,  IM8),
    E(0x81, TEXT_CMPE,  IM8),
    E(0x86, TEXT_LDE,   IM8),
    E(0x8B, TEXT_ADDE,  IM8),
    E(0x8D, TEXT_DIVD,  IM16),
    E(0x8E, TEXT_DIVQ,  IM16),
    E(0x8F, TEXT_MULD,  IM16),
    E(0x90, TEXT_SUBE,  DIR),
    E(0x91, TEXT_CMPE,  DIR),
    E(0x96, TEXT_LDE,   DIR),
    E(0x97, TEXT_STE,   DIR),
    E(0x9B, TEXT_ADDE,  DIR),
    E(0x9D, TEXT_DIVD,  DIR),
    E(0x9E, TEXT_DIVQ,  DIR),
    E(0x9F, TEXT_MULD,  DIR),
    E(0xA0, TEXT_SUBE,  IDX),
    E(0xA1, TEXT_CMPE,  IDX),
    E(0xA6, TEXT_LDE,   IDX),
    E(0xA7, TEXT_STE,   IDX),
    E(0xAB, TEXT_ADDE,  IDX),
    E(0xAD, TEXT_DIVD,  IDX),
    E(0xAE, TEXT_DIVQ,  IDX),
    E(0xAF, TEXT_MULD,  IDX),
    E(0xB0, TEXT_SUBE,  EXT),
    E(0xB1, TEXT_CMPE,  EXT),
    E(0xB6, TEXT_LDE,   EXT),
    E(0xB7, TEXT_STE,   EXT),
    E(0xBB, TEXT_ADDE,  EXT),
    E(0xBD, TEXT_DIVD,  EXT),
    E(0xBE, TEXT_DIVQ,  EXT),
    E(0xBF, TEXT_MULD,  EXT),
    E(0xC0, TEXT_SUBF,  IM8),
    E(0xC1, TEXT_CMPF,  IM8),
    E(0xC6, TEXT_LDF,   IM8),
    E(0xCB, TEXT_ADDF,  IM8),
    E(0xD0, TEXT_SUBF,  DIR),
    E(0xD1, TEXT_CMPF,  DIR),
    E(0xD6, TEXT_LDF,   DIR),
    E(0xD7, TEXT_STF,   DIR),
    E(0xDB, TEXT_ADDF,  DIR),
    E(0xE0, TEXT_SUBF,  IDX),
    E(0xE1, TEXT_CMPF,  IDX),
    E(0xE6, TEXT_LDF,   IDX),
    E(0xE7, TEXT_STF,   IDX),
    E(0xEB, TEXT_ADDF,  IDX),
    E(0xF0, TEXT_SUBF,  EXT),
    E(0xF1, TEXT_CMPF,  EXT),
    E(0xF6, TEXT_LDF,   EXT),
    E(0xF7, TEXT_STF,   EXT),
    E(0xFB, TEXT_ADDF,  EXT),
};
// clang-format on

static constexpr Config::opcode_t PREFIX_P00 = 0x00;
static constexpr Config::opcode_t PREFIX_P10 = 0x10;
static constexpr Config::opcode_t PREFIX_P11 = 0x11;

bool TableMc6809::isPrefix(Config::opcode_t opCode) {
    return opCode == PREFIX_P10 || opCode == PREFIX_P11;
}

class TableMc6809::EntryPage : public EntryPageBase<Entry> {
public:
    constexpr EntryPage(
            Config::opcode_t prefix, const Entry *table, const Entry *end)
        : EntryPageBase(table, end), _prefix(prefix) {}

    Config::opcode_t prefix() const { return pgm_read_byte(&_prefix); }

private:
    Config::opcode_t _prefix;
};

static constexpr TableMc6809::EntryPage MC6809_PAGES[] PROGMEM = {
        {PREFIX_P00, ARRAY_RANGE(MC6809_P00)},
        {PREFIX_P10, ARRAY_RANGE(MC6809_P10)},
        {PREFIX_P11, ARRAY_RANGE(MC6809_P11)},
};

static constexpr TableMc6809::EntryPage HD6309_PAGES[] PROGMEM = {
        {PREFIX_P00, ARRAY_RANGE(MC6809_P00)},
        {PREFIX_P10, ARRAY_RANGE(MC6809_P10)},
        {PREFIX_P11, ARRAY_RANGE(MC6809_P11)},
        {PREFIX_P00, ARRAY_RANGE(HD6309_P00)},
        {PREFIX_P10, ARRAY_RANGE(HD6309_P10)},
        {PREFIX_P11, ARRAY_RANGE(HD6309_P11)},
};

static bool matchAddrMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == IM32)
        return table == IM8 || table == IM16 ||
               table == REGLIST;  // immediate register list
    if (opr == REG_REG)
        return table == IDX          // A,X
               || table == REGLIST;  // 2-length register list
    if (opr == REG_BIT)
        return table == REG_REG;  // A,0
    if (opr == DIR)
        return table == REL || table == LREL;
    if (opr == EXT)
        return table == REL || table == LREL;
    if (opr == REGLIST)
        return table == REG_TFM;  // X
    if (opr == NONE)
        return table == REGLIST;  // empty register list
    return false;
}

static bool matchAddrMode(Entry::Flags flags, const Entry *entry) {
    const Entry::Flags table = entry->flags();
    return matchAddrMode(flags.mode1(), table.mode1()) &&
           matchAddrMode(flags.mode2(), table.mode2());
}

Error TableMc6809::searchName(
        InsnMc6809 &insn, const EntryPage *pages, const EntryPage *end) const {
    uint8_t count = 0;
    for (const EntryPage *page = pages; page < end; page++) {
        const Entry *entry = TableBase::searchName<Entry, Entry::Flags>(
                insn.name(), insn.flags(), page->table(), page->end(),
                matchAddrMode, count);
        if (entry) {
            insn.setOpCode(entry->opCode(), page->prefix());
            insn.setFlags(entry->flags());
            return OK;
        }
    }
    return count == 0 ? UNKNOWN_INSTRUCTION : OPERAND_NOT_ALLOWED;
}

Error TableMc6809::searchOpCode(
        InsnMc6809 &insn, const EntryPage *pages, const EntryPage *end) const {
    for (const EntryPage *page = pages; page < end; page++) {
        if (insn.prefix() != page->prefix())
            continue;
        const Entry *entry = TableBase::searchCode<Entry, Config::opcode_t>(
                insn.opCode(), page->table(), page->end());
        if (entry) {
            insn.setFlags(entry->flags());
            insn.setName_P(entry->name());
            return OK;
        }
    }
    return UNKNOWN_INSTRUCTION;
}

Error TableMc6809::searchName(InsnMc6809 &insn) const {
    return _error.setError(searchName(insn, _table, _end));
}

Error TableMc6809::searchOpCode(InsnMc6809 &insn) const {
    return _error.setError(searchOpCode(insn, _table, _end));
}

struct TableMc6809::PostEntry : public PostSpec {
    uint8_t mask;
    uint8_t byte;

    constexpr PostEntry(RegName _index, RegName _base, int8_t _size,
            bool _indir, uint8_t _mask, uint8_t _byte)
        : PostSpec(_index, _base, _size, _indir), mask(_mask), byte(_byte) {}
};

static const TableMc6809::PostEntry MC6809_POSTBYTE[] PROGMEM = {
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

static const TableMc6809::PostEntry HD6309_POSTBYTE[] PROGMEM = {
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

Error TableMc6809::searchPostByte(const uint8_t post, PostSpec &spec,
        const PostEntry *table, const PostEntry *end) const {
    for (const PostEntry *entry = table; entry < end; entry++) {
        const uint8_t mask = pgm_read_byte(&entry->mask);
        const uint8_t byte = post & mask;
        if (byte == pgm_read_byte(&entry->byte)) {
            spec.index = RegName(pgm_read_byte(&entry->index));
            spec.base = RegName(pgm_read_byte(&entry->base));
            spec.size = pgm_read_byte(&entry->size);
            spec.indir = (mask != 0x80) && (post & 0x10);
            return OK;
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

int16_t TableMc6809::searchPostSpec(
        PostSpec &spec, const PostEntry *table, const PostEntry *end) const {
    const RegName specBase = baseRegName(spec.base);
    for (const PostEntry *entry = table; entry < end; entry++) {
        const RegName base = RegName(pgm_read_byte(&entry->base));
        const int8_t size = static_cast<int8_t>(pgm_read_byte(&entry->size));
        if (specBase == base && spec.size == size &&
                spec.index == RegName(pgm_read_byte(&entry->index))) {
            uint8_t byte = pgm_read_byte(&entry->byte);
            if (spec.indir) {
                if (pgm_read_byte(&entry->indir) == 0)
                    continue;
                byte |= 0x10;
            }
            spec.base = base;
            return byte;
        }
    }
    return -1;
}

Error TableMc6809::searchPostByte(const uint8_t post, PostSpec &spec) const {
    if (_cpuType == HD6309 &&
            searchPostByte(post, spec, ARRAY_RANGE(HD6309_POSTBYTE)) == OK)
        return OK;
    return searchPostByte(post, spec, ARRAY_RANGE(MC6809_POSTBYTE));
}

int16_t TableMc6809::searchPostSpec(PostSpec &spec) const {
    int16_t post = searchPostSpec(spec, ARRAY_RANGE(MC6809_POSTBYTE));
    if (post < 0 && _cpuType == HD6309)
        post = searchPostSpec(spec, ARRAY_RANGE(HD6309_POSTBYTE));
    return post;
}

TableMc6809::TableMc6809() {
    setCpu(MC6809);
}

bool TableMc6809::setCpu(CpuType cpuType) {
    _cpuType = cpuType;
    if (cpuType == MC6809) {
        _table = ARRAY_BEGIN(MC6809_PAGES);
        _end = ARRAY_END(MC6809_PAGES);
        return true;
    }
    _table = ARRAY_BEGIN(HD6309_PAGES);
    _end = ARRAY_END(HD6309_PAGES);
    return true;
}

const char *TableMc6809::listCpu() const {
    return TEXT_CPU_LIST;
}

const char *TableMc6809::getCpu() const {
    return _cpuType == MC6809 ? TEXT_CPU_6809 : TEXT_CPU_6309;
}

bool TableMc6809::setCpu(const char *cpu) {
    if (strcasecmp_P(cpu, TEXT_CPU_MC6809) == 0 ||
            strcasecmp_P(cpu, TEXT_CPU_6809) == 0)
        return setCpu(MC6809);
    if (strcasecmp_P(cpu, TEXT_CPU_HD6309) == 0 ||
            strcasecmp_P(cpu, TEXT_CPU_6309) == 0)
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

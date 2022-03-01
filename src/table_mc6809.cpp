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
#define E(_opc, _name, _mode) X(_opc, _name, _mode, M_NONE)

// clang-format off
static constexpr Entry MC6809_P00[] PROGMEM = {
    E(0x00, TEXT_NEG,   M_DIR),
    E(0x03, TEXT_COM,   M_DIR),
    E(0x04, TEXT_LSR,   M_DIR),
    E(0x06, TEXT_ROR,   M_DIR),
    E(0x07, TEXT_ASR,   M_DIR),
    E(0x08, TEXT_ASL,   M_DIR),
    E(0x08, TEXT_LSL,   M_DIR),
    E(0x09, TEXT_ROL,   M_DIR),
    E(0x0A, TEXT_DEC,   M_DIR),
    E(0x0C, TEXT_INC,   M_DIR),
    E(0x0D, TEXT_TST,   M_DIR),
    E(0x0E, TEXT_JMP,   M_DIR),
    E(0x0F, TEXT_CLR,   M_DIR),
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
    E(0x60, TEXT_NEG,   M_IDX),
    E(0x63, TEXT_COM,   M_IDX),
    E(0x64, TEXT_LSR,   M_IDX),
    E(0x66, TEXT_ROR,   M_IDX),
    E(0x67, TEXT_ASR,   M_IDX),
    E(0x68, TEXT_ASL,   M_IDX),
    E(0x68, TEXT_LSL,   M_IDX),
    E(0x69, TEXT_ROL,   M_IDX),
    E(0x6A, TEXT_DEC,   M_IDX),
    E(0x6C, TEXT_INC,   M_IDX),
    E(0x6D, TEXT_TST,   M_IDX),
    E(0x6E, TEXT_JMP,   M_IDX),
    E(0x6F, TEXT_CLR,   M_IDX),
    E(0x70, TEXT_NEG,   M_EXT),
    E(0x73, TEXT_COM,   M_EXT),
    E(0x74, TEXT_LSR,   M_EXT),
    E(0x76, TEXT_ROR,   M_EXT),
    E(0x77, TEXT_ASR,   M_EXT),
    E(0x78, TEXT_ASL,   M_EXT),
    E(0x78, TEXT_LSL,   M_EXT),
    E(0x79, TEXT_ROL,   M_EXT),
    E(0x7A, TEXT_DEC,   M_EXT),
    E(0x7C, TEXT_INC,   M_EXT),
    E(0x7D, TEXT_TST,   M_EXT),
    E(0x7E, TEXT_JMP,   M_EXT),
    E(0x7F, TEXT_CLR,   M_EXT),
    E(0x80, TEXT_SUBA,  M_IM8),
    E(0x81, TEXT_CMPA,  M_IM8),
    E(0x82, TEXT_SBCA,  M_IM8),
    E(0x83, TEXT_SUBD,  M_IM16),
    E(0x84, TEXT_ANDA,  M_IM8),
    E(0x85, TEXT_BITA,  M_IM8),
    E(0x86, TEXT_LDA,   M_IM8),
    E(0x88, TEXT_EORA,  M_IM8),
    E(0x89, TEXT_ADCA,  M_IM8),
    E(0x8A, TEXT_ORA,   M_IM8),
    E(0x8B, TEXT_ADDA,  M_IM8),
    E(0x8C, TEXT_CMPX,  M_IM16),
    E(0x8D, TEXT_BSR,   M_REL),
    E(0x8E, TEXT_LDX,   M_IM16),
    E(0x90, TEXT_SUBA,  M_DIR),
    E(0x91, TEXT_CMPA,  M_DIR),
    E(0x92, TEXT_SBCA,  M_DIR),
    E(0x93, TEXT_SUBD,  M_DIR),
    E(0x94, TEXT_ANDA,  M_DIR),
    E(0x95, TEXT_BITA,  M_DIR),
    E(0x96, TEXT_LDA,   M_DIR),
    E(0x97, TEXT_STA,   M_DIR),
    E(0x98, TEXT_EORA,  M_DIR),
    E(0x99, TEXT_ADCA,  M_DIR),
    E(0x9A, TEXT_ORA,   M_DIR),
    E(0x9B, TEXT_ADDA,  M_DIR),
    E(0x9C, TEXT_CMPX,  M_DIR),
    E(0x9D, TEXT_JSR,   M_DIR),
    E(0x9E, TEXT_LDX,   M_DIR),
    E(0x9F, TEXT_STX,   M_DIR),
    E(0xA0, TEXT_SUBA,  M_IDX),
    E(0xA1, TEXT_CMPA,  M_IDX),
    E(0xA2, TEXT_SBCA,  M_IDX),
    E(0xA3, TEXT_SUBD,  M_IDX),
    E(0xA4, TEXT_ANDA,  M_IDX),
    E(0xA5, TEXT_BITA,  M_IDX),
    E(0xA6, TEXT_LDA,   M_IDX),
    E(0xA7, TEXT_STA,   M_IDX),
    E(0xA8, TEXT_EORA,  M_IDX),
    E(0xA9, TEXT_ADCA,  M_IDX),
    E(0xAA, TEXT_ORA,   M_IDX),
    E(0xAB, TEXT_ADDA,  M_IDX),
    E(0xAC, TEXT_CMPX,  M_IDX),
    E(0xAD, TEXT_JSR,   M_IDX),
    E(0xAE, TEXT_LDX,   M_IDX),
    E(0xAF, TEXT_STX,   M_IDX),
    E(0xB0, TEXT_SUBA,  M_EXT),
    E(0xB1, TEXT_CMPA,  M_EXT),
    E(0xB2, TEXT_SBCA,  M_EXT),
    E(0xB3, TEXT_SUBD,  M_EXT),
    E(0xB4, TEXT_ANDA,  M_EXT),
    E(0xB5, TEXT_BITA,  M_EXT),
    E(0xB6, TEXT_LDA,   M_EXT),
    E(0xB7, TEXT_STA,   M_EXT),
    E(0xB8, TEXT_EORA,  M_EXT),
    E(0xB9, TEXT_ADCA,  M_EXT),
    E(0xBA, TEXT_ORA,   M_EXT),
    E(0xBB, TEXT_ADDA,  M_EXT),
    E(0xBC, TEXT_CMPX,  M_EXT),
    E(0xBD, TEXT_JSR,   M_EXT),
    E(0xBE, TEXT_LDX,   M_EXT),
    E(0xBF, TEXT_STX,   M_EXT),
    E(0xC0, TEXT_SUBB,  M_IM8),
    E(0xC1, TEXT_CMPB,  M_IM8),
    E(0xC2, TEXT_SBCB,  M_IM8),
    E(0xC3, TEXT_ADDD,  M_IM16),
    E(0xC4, TEXT_ANDB,  M_IM8),
    E(0xC5, TEXT_BITB,  M_IM8),
    E(0xC6, TEXT_LDB,   M_IM8),
    E(0xC8, TEXT_EORB,  M_IM8),
    E(0xC9, TEXT_ADCB,  M_IM8),
    E(0xCA, TEXT_ORB,   M_IM8),
    E(0xCB, TEXT_ADDB,  M_IM8),
    E(0xCC, TEXT_LDD,   M_IM16),
    E(0xCE, TEXT_LDU,   M_IM16),
    E(0xD0, TEXT_SUBB,  M_DIR),
    E(0xD1, TEXT_CMPB,  M_DIR),
    E(0xD2, TEXT_SBCB,  M_DIR),
    E(0xD3, TEXT_ADDD,  M_DIR),
    E(0xD4, TEXT_ANDB,  M_DIR),
    E(0xD5, TEXT_BITB,  M_DIR),
    E(0xD6, TEXT_LDB,   M_DIR),
    E(0xD7, TEXT_STB,   M_DIR),
    E(0xD8, TEXT_EORB,  M_DIR),
    E(0xD9, TEXT_ADCB,  M_DIR),
    E(0xDA, TEXT_ORB,   M_DIR),
    E(0xDB, TEXT_ADDB,  M_DIR),
    E(0xDC, TEXT_LDD,   M_DIR),
    E(0xDD, TEXT_STD,   M_DIR),
    E(0xDE, TEXT_LDU,   M_DIR),
    E(0xDF, TEXT_STU,   M_DIR),
    E(0xE0, TEXT_SUBB,  M_IDX),
    E(0xE1, TEXT_CMPB,  M_IDX),
    E(0xE2, TEXT_SBCB,  M_IDX),
    E(0xE3, TEXT_ADDD,  M_IDX),
    E(0xE4, TEXT_ANDB,  M_IDX),
    E(0xE5, TEXT_BITB,  M_IDX),
    E(0xE6, TEXT_LDB,   M_IDX),
    E(0xE7, TEXT_STB,   M_IDX),
    E(0xE8, TEXT_EORB,  M_IDX),
    E(0xE9, TEXT_ADCB,  M_IDX),
    E(0xEA, TEXT_ORB,   M_IDX),
    E(0xEB, TEXT_ADDB,  M_IDX),
    E(0xEC, TEXT_LDD,   M_IDX),
    E(0xED, TEXT_STD,   M_IDX),
    E(0xEE, TEXT_LDU,   M_IDX),
    E(0xEF, TEXT_STU,   M_IDX),
    E(0xF0, TEXT_SUBB,  M_EXT),
    E(0xF1, TEXT_CMPB,  M_EXT),
    E(0xF2, TEXT_SBCB,  M_EXT),
    E(0xF3, TEXT_ADDD,  M_EXT),
    E(0xF4, TEXT_ANDB,  M_EXT),
    E(0xF5, TEXT_BITB,  M_EXT),
    E(0xF6, TEXT_LDB,   M_EXT),
    E(0xF7, TEXT_STB,   M_EXT),
    E(0xF8, TEXT_EORB,  M_EXT),
    E(0xF9, TEXT_ADCB,  M_EXT),
    E(0xFA, TEXT_ORB,   M_EXT),
    E(0xFB, TEXT_ADDB,  M_EXT),
    E(0xFC, TEXT_LDD,   M_EXT),
    E(0xFD, TEXT_STD,   M_EXT),
    E(0xFE, TEXT_LDU,   M_EXT),
    E(0xFF, TEXT_STU,   M_EXT),
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
    E(0x83, TEXT_CMPD,  M_IM16),
    E(0x8C, TEXT_CMPY,  M_IM16),
    E(0x8E, TEXT_LDY,   M_IM16),
    E(0x93, TEXT_CMPD,  M_DIR),
    E(0x9C, TEXT_CMPY,  M_DIR),
    E(0x9E, TEXT_LDY,   M_DIR),
    E(0x9F, TEXT_STY,   M_DIR),
    E(0xA3, TEXT_CMPD,  M_IDX),
    E(0xAC, TEXT_CMPY,  M_IDX),
    E(0xAE, TEXT_LDY,   M_IDX),
    E(0xAF, TEXT_STY,   M_IDX),
    E(0xB3, TEXT_CMPD,  M_EXT),
    E(0xBC, TEXT_CMPY,  M_EXT),
    E(0xBE, TEXT_LDY,   M_EXT),
    E(0xBF, TEXT_STY,   M_EXT),
    E(0xCE, TEXT_LDS,   M_IM16),
    E(0xDE, TEXT_LDS,   M_DIR),
    E(0xDF, TEXT_STS,   M_DIR),
    E(0xEE, TEXT_LDS,   M_IDX),
    E(0xEF, TEXT_STS,   M_IDX),
    E(0xFE, TEXT_LDS,   M_EXT),
    E(0xFF, TEXT_STS,   M_EXT),
};

static constexpr Entry MC6809_P11[] PROGMEM = {
    E(0x3F, TEXT_SWI3,  M_NONE),
    E(0x83, TEXT_CMPU,  M_IM16),
    E(0x8C, TEXT_CMPS,  M_IM16),
    E(0x93, TEXT_CMPU,  M_DIR),
    E(0x9C, TEXT_CMPS,  M_DIR),
    E(0xA3, TEXT_CMPU,  M_IDX),
    E(0xAC, TEXT_CMPS,  M_IDX),
    E(0xB3, TEXT_CMPU,  M_EXT),
    E(0xBC, TEXT_CMPS,  M_EXT),
};

static constexpr Entry HD6309_P00[] PROGMEM = {
    X(0x01, TEXT_OIM,   M_IM8,  M_DIR),
    X(0x02, TEXT_AIM,   M_IM8,  M_DIR),
    X(0x05, TEXT_EIM,   M_IM8,  M_DIR),
    X(0x0B, TEXT_TIM,   M_IM8,  M_DIR),
    E(0x14, TEXT_SEXW,  M_NONE),
    X(0x61, TEXT_OIM,   M_IM8,  M_IDX),
    X(0x62, TEXT_AIM,   M_IM8,  M_IDX),
    X(0x65, TEXT_EIM,   M_IM8,  M_IDX),
    X(0x6B, TEXT_TIM,   M_IM8,  M_IDX),
    X(0x71, TEXT_OIM,   M_IM8,  M_EXT),
    X(0x72, TEXT_AIM,   M_IM8,  M_EXT),
    X(0x75, TEXT_EIM,   M_IM8,  M_EXT),
    X(0x7B, TEXT_TIM,   M_IM8,  M_EXT),
    E(0xCD, TEXT_LDQ,   M_IM32),
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
    E(0x80, TEXT_SUBW,  M_IM16),
    E(0x81, TEXT_CMPW,  M_IM16),
    E(0x82, TEXT_SBCD,  M_IM16),
    E(0x84, TEXT_ANDD,  M_IM16),
    E(0x85, TEXT_BITD,  M_IM16),
    E(0x86, TEXT_LDW,   M_IM16),
    E(0x88, TEXT_EORD,  M_IM16),
    E(0x89, TEXT_ADCD,  M_IM16),
    E(0x8A, TEXT_ORD,   M_IM16),
    E(0x8B, TEXT_ADDW,  M_IM16),
    E(0x90, TEXT_SUBW,  M_DIR),
    E(0x91, TEXT_CMPW,  M_DIR),
    E(0x92, TEXT_SBCD,  M_DIR),
    E(0x94, TEXT_ANDD,  M_DIR),
    E(0x95, TEXT_BITD,  M_DIR),
    E(0x96, TEXT_LDW,   M_DIR),
    E(0x97, TEXT_STW,   M_DIR),
    E(0x98, TEXT_EORD,  M_DIR),
    E(0x99, TEXT_ADCD,  M_DIR),
    E(0x9A, TEXT_ORD,   M_DIR),
    E(0x9B, TEXT_ADDW,  M_DIR),
    E(0xA0, TEXT_SUBW,  M_IDX),
    E(0xA1, TEXT_CMPW,  M_IDX),
    E(0xA2, TEXT_SBCD,  M_IDX),
    E(0xA4, TEXT_ANDD,  M_IDX),
    E(0xA5, TEXT_BITD,  M_IDX),
    E(0xA6, TEXT_LDW,   M_IDX),
    E(0xA7, TEXT_STW,   M_IDX),
    E(0xA8, TEXT_EORD,  M_IDX),
    E(0xA9, TEXT_ADCD,  M_IDX),
    E(0xAA, TEXT_ORD,   M_IDX),
    E(0xAB, TEXT_ADDW,  M_IDX),
    E(0xB0, TEXT_SUBW,  M_EXT),
    E(0xB1, TEXT_CMPW,  M_EXT),
    E(0xB2, TEXT_SBCD,  M_EXT),
    E(0xB4, TEXT_ANDD,  M_EXT),
    E(0xB5, TEXT_BITD,  M_EXT),
    E(0xB6, TEXT_LDW,   M_EXT),
    E(0xB7, TEXT_STW,   M_EXT),
    E(0xB8, TEXT_EORD,  M_EXT),
    E(0xB9, TEXT_ADCD,  M_EXT),
    E(0xBA, TEXT_ORD,   M_EXT),
    E(0xBB, TEXT_ADDW,  M_EXT),
    E(0xDC, TEXT_LDQ,   M_DIR),
    E(0xDD, TEXT_STQ,   M_DIR),
    E(0xEC, TEXT_LDQ,   M_IDX),
    E(0xED, TEXT_STQ,   M_IDX),
    E(0xFC, TEXT_LDQ,   M_EXT),
    E(0xFD, TEXT_STQ,   M_EXT),
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
    E(0x80, TEXT_SUBE,  M_IM8),
    E(0x81, TEXT_CMPE,  M_IM8),
    E(0x86, TEXT_LDE,   M_IM8),
    E(0x8B, TEXT_ADDE,  M_IM8),
    E(0x8D, TEXT_DIVD,  M_IM16),
    E(0x8E, TEXT_DIVQ,  M_IM16),
    E(0x8F, TEXT_MULD,  M_IM16),
    E(0x90, TEXT_SUBE,  M_DIR),
    E(0x91, TEXT_CMPE,  M_DIR),
    E(0x96, TEXT_LDE,   M_DIR),
    E(0x97, TEXT_STE,   M_DIR),
    E(0x9B, TEXT_ADDE,  M_DIR),
    E(0x9D, TEXT_DIVD,  M_DIR),
    E(0x9E, TEXT_DIVQ,  M_DIR),
    E(0x9F, TEXT_MULD,  M_DIR),
    E(0xA0, TEXT_SUBE,  M_IDX),
    E(0xA1, TEXT_CMPE,  M_IDX),
    E(0xA6, TEXT_LDE,   M_IDX),
    E(0xA7, TEXT_STE,   M_IDX),
    E(0xAB, TEXT_ADDE,  M_IDX),
    E(0xAD, TEXT_DIVD,  M_IDX),
    E(0xAE, TEXT_DIVQ,  M_IDX),
    E(0xAF, TEXT_MULD,  M_IDX),
    E(0xB0, TEXT_SUBE,  M_EXT),
    E(0xB1, TEXT_CMPE,  M_EXT),
    E(0xB6, TEXT_LDE,   M_EXT),
    E(0xB7, TEXT_STE,   M_EXT),
    E(0xBB, TEXT_ADDE,  M_EXT),
    E(0xBD, TEXT_DIVD,  M_EXT),
    E(0xBE, TEXT_DIVQ,  M_EXT),
    E(0xBF, TEXT_MULD,  M_EXT),
    E(0xC0, TEXT_SUBF,  M_IM8),
    E(0xC1, TEXT_CMPF,  M_IM8),
    E(0xC6, TEXT_LDF,   M_IM8),
    E(0xCB, TEXT_ADDF,  M_IM8),
    E(0xD0, TEXT_SUBF,  M_DIR),
    E(0xD1, TEXT_CMPF,  M_DIR),
    E(0xD6, TEXT_LDF,   M_DIR),
    E(0xD7, TEXT_STF,   M_DIR),
    E(0xDB, TEXT_ADDF,  M_DIR),
    E(0xE0, TEXT_SUBF,  M_IDX),
    E(0xE1, TEXT_CMPF,  M_IDX),
    E(0xE6, TEXT_LDF,   M_IDX),
    E(0xE7, TEXT_STF,   M_IDX),
    E(0xEB, TEXT_ADDF,  M_IDX),
    E(0xF0, TEXT_SUBF,  M_EXT),
    E(0xF1, TEXT_CMPF,  M_EXT),
    E(0xF6, TEXT_LDF,   M_EXT),
    E(0xF7, TEXT_STF,   M_EXT),
    E(0xFB, TEXT_ADDF,  M_EXT),
};
// clang-format on

static constexpr Config::opcode_t PREFIX_P00 = 0x00;
static constexpr Config::opcode_t PREFIX_P10 = 0x10;
static constexpr Config::opcode_t PREFIX_P11 = 0x11;

bool TableMc6809::isPrefix(Config::opcode_t opCode) {
    return opCode == PREFIX_P10 || opCode == PREFIX_P11;
}

struct TableMc6809::EntryPage : EntryPageBase<Entry> {
    constexpr EntryPage(Config::opcode_t prefix, const Entry *table, const Entry *end)
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
    if (opr == M_IM32)
        return table == M_IM8 || table == M_IM16 || table == M_LIST;  // immediate register list
    if (opr == M_PAIR)
        return table == M_IDX          // A,X
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

Error TableMc6809::searchName(
        InsnMc6809 &insn, const EntryPage *pages, const EntryPage *end) const {
    uint8_t count = 0;
    for (auto page = pages; page < end; page++) {
        auto entry = TableBase::searchName<Entry, Entry::Flags>(
                insn.name(), insn.flags(), page->table(), page->end(), matchAddrMode, count);
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
    for (auto page = pages; page < end; page++) {
        if (insn.prefix() != page->prefix())
            continue;
        auto entry = TableBase::searchCode<Entry, Config::opcode_t>(
                insn.opCode(), page->table(), page->end());
        if (entry) {
            insn.setFlags(entry->flags());
            insn.setName_P(entry->name_P());
            return OK;
        }
    }
    return UNKNOWN_INSTRUCTION;
}

Error TableMc6809::searchName(InsnMc6809 &insn) const {
    return setError(searchName(insn, _table, _end));
}

Error TableMc6809::searchOpCode(InsnMc6809 &insn) const {
    return setError(searchOpCode(insn, _table, _end));
}

struct TableMc6809::PostEntry : PostSpec {
    uint8_t mask;
    uint8_t byte;

    constexpr PostEntry(
            RegName _index, RegName _base, int8_t _size, bool _indir, uint8_t _mask, uint8_t _byte)
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

Error TableMc6809::searchPostByte(
        const uint8_t post, PostSpec &spec, const PostEntry *table, const PostEntry *end) const {
    for (auto entry = table; entry < end; entry++) {
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
    auto specBase = baseRegName(spec.base);
    for (auto entry = table; entry < end; entry++) {
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
    return -1;
}

Error TableMc6809::searchPostByte(const uint8_t post, PostSpec &spec) const {
    if (_cpuType == HD6309 && searchPostByte(post, spec, ARRAY_RANGE(HD6309_POSTBYTE)) == OK)
        return OK;
    return searchPostByte(post, spec, ARRAY_RANGE(MC6809_POSTBYTE));
}

int16_t TableMc6809::searchPostSpec(PostSpec &spec) const {
    auto post = searchPostSpec(spec, ARRAY_RANGE(MC6809_POSTBYTE));
    if (post < 0 && _cpuType == HD6309)
        post = searchPostSpec(spec, ARRAY_RANGE(HD6309_POSTBYTE));
    return post;
}

class CpuTable : public CpuTableBase<CpuType, TableMc6809::EntryPage> {
public:
    constexpr CpuTable(CpuType cpuType, const char *name, const TableMc6809::EntryPage *table,
            const TableMc6809::EntryPage *end)
        : CpuTableBase(cpuType, name, table, end) {}
};

static constexpr CpuTable CPU_TABLES[] PROGMEM = {
        {MC6809, TEXT_CPU_6809, ARRAY_RANGE(MC6809_PAGES)},
        {HD6309, TEXT_CPU_6309, ARRAY_RANGE(HD6309_PAGES)},
};

TableMc6809::TableMc6809() {
    setCpu(MC6809);
}

bool TableMc6809::setCpu(CpuType cpuType) {
    auto t = CpuTable::search(cpuType, ARRAY_RANGE(CPU_TABLES));
    if (t == nullptr)
        return false;
    _cpuType = cpuType;
    _table = t->table();
    _end = t->end();
    return true;
}

const /* PROGMEM */ char *TableMc6809::listCpu_P() const {
    return TEXT_CPU_LIST;
}

const /* PROGMEM */ char *TableMc6809::cpu_P() const {
    return CpuTable::search(_cpuType, ARRAY_RANGE(CPU_TABLES))->name_P();
}

bool TableMc6809::setCpu(const char *cpu) {
    auto t = CpuTable::search(cpu, ARRAY_RANGE(CPU_TABLES));
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

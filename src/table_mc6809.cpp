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

#include "config_mc6809.h"

#include "entry_mc6809.h"
#include "table_mc6809.h"
#include "text_mc6809.h"

#include <string.h>

namespace libasm {
namespace mc6809 {

#define E(_opc, _name, _sz, _mode)                              \
    { _opc,  Entry::_flags(SZ_##_sz, _mode), TEXT_##_name },

static constexpr Entry MC6809_P00[] PROGMEM = {
    E(0x00, NEG,   BYTE, DIR)
    E(0x03, COM,   BYTE, DIR)
    E(0x04, LSR,   BYTE, DIR)
    E(0x06, ROR,   BYTE, DIR)
    E(0x07, ASR,   BYTE, DIR)
    E(0x08, ASL,   BYTE, DIR)
    E(0x08, LSL,   BYTE, DIR)
    E(0x09, ROL,   BYTE, DIR)
    E(0x0A, DEC,   BYTE, DIR)
    E(0x0C, INC,   BYTE, DIR)
    E(0x0D, TST,   BYTE, DIR)
    E(0x0E, JMP,   NONE, DIR)
    E(0x0F, CLR,   BYTE, DIR)
    E(0x12, NOP,   NONE, INH)
    E(0x13, SYNC,  NONE, INH)
    E(0x16, LBRA,  WORD, REL)
    E(0x17, LBSR,  WORD, REL)
    E(0x19, DAA,   NONE, INH)
    E(0x1A, ORCC,  BYTE, IMM)
    E(0x1C, ANDCC, BYTE, IMM)
    E(0x1D, SEX,   BYTE, INH)
    E(0x1E, EXG,   NONE, REG_REG)
    E(0x1F, TFR,   NONE, REG_REG)
    E(0x20, BRA,   BYTE, REL)
    E(0x21, BRN,   BYTE, REL)
    E(0x22, BHI,   BYTE, REL)
    E(0x23, BLS,   BYTE, REL)
    E(0x24, BHS,   BYTE, REL)
    E(0x24, BCC,   BYTE, REL)
    E(0x25, BLO,   BYTE, REL)
    E(0x25, BCS,   BYTE, REL)
    E(0x26, BNE,   BYTE, REL)
    E(0x27, BEQ,   BYTE, REL)
    E(0x28, BVC,   BYTE, REL)
    E(0x29, BVS,   BYTE, REL)
    E(0x2A, BPL,   BYTE, REL)
    E(0x2B, BMI,   BYTE, REL)
    E(0x2C, BGE,   BYTE, REL)
    E(0x2D, BLT,   BYTE, REL)
    E(0x2E, BGT,   BYTE, REL)
    E(0x2F, BLE,   BYTE, REL)
    E(0x30, LEAX,  NONE, IDX)
    E(0x31, LEAY,  NONE, IDX)
    E(0x32, LEAS,  NONE, IDX)
    E(0x33, LEAU,  NONE, IDX)
    E(0x34, PSHS,  BYTE, PSH_PUL)
    E(0x35, PULS,  BYTE, PSH_PUL)
    E(0x36, PSHU,  BYTE, PSH_PUL)
    E(0x37, PULU,  BYTE, PSH_PUL)
    E(0x39, RTS,   NONE, INH)
    E(0x3A, ABX,   NONE, INH)
    E(0x3B, RTI,   NONE, INH)
    E(0x3C, CWAI,  BYTE, IMM)
    E(0x3D, MUL,   BYTE, INH)
    E(0x3F, SWI,   NONE, INH)
    E(0x40, NEGA,  BYTE, INH)
    E(0x43, COMA,  BYTE, INH)
    E(0x44, LSRA,  BYTE, INH)
    E(0x46, RORA,  BYTE, INH)
    E(0x47, ASRA,  BYTE, INH)
    E(0x48, ASLA,  BYTE, INH)
    E(0x48, LSLA,  BYTE, INH)
    E(0x49, ROLA,  BYTE, INH)
    E(0x4A, DECA,  BYTE, INH)
    E(0x4C, INCA,  BYTE, INH)
    E(0x4D, TSTA,  BYTE, INH)
    E(0x4F, CLRA,  BYTE, INH)
    E(0x50, NEGB,  BYTE, INH)
    E(0x53, COMB,  BYTE, INH)
    E(0x54, LSRB,  BYTE, INH)
    E(0x56, RORB,  BYTE, INH)
    E(0x57, ASRB,  BYTE, INH)
    E(0x58, ASLB,  BYTE, INH)
    E(0x58, LSLB,  BYTE, INH)
    E(0x59, ROLB,  BYTE, INH)
    E(0x5A, DECB,  BYTE, INH)
    E(0x5C, INCB,  BYTE, INH)
    E(0x5D, TSTB,  BYTE, INH)
    E(0x5F, CLRB,  BYTE, INH)
    E(0x60, NEG,   BYTE, IDX)
    E(0x63, COM,   BYTE, IDX)
    E(0x64, LSR,   BYTE, IDX)
    E(0x66, ROR,   BYTE, IDX)
    E(0x67, ASR,   BYTE, IDX)
    E(0x68, ASL,   BYTE, IDX)
    E(0x68, LSL,   BYTE, IDX)
    E(0x69, ROL,   BYTE, IDX)
    E(0x6A, DEC,   BYTE, IDX)
    E(0x6C, INC,   BYTE, IDX)
    E(0x6D, TST,   BYTE, IDX)
    E(0x6E, JMP,   NONE, IDX)
    E(0x6F, CLR,   BYTE, IDX)
    E(0x70, NEG,   BYTE, EXT)
    E(0x73, COM,   BYTE, EXT)
    E(0x74, LSR,   BYTE, EXT)
    E(0x76, ROR,   BYTE, EXT)
    E(0x77, ASR,   BYTE, EXT)
    E(0x78, ASL,   BYTE, EXT)
    E(0x78, LSL,   BYTE, EXT)
    E(0x79, ROL,   BYTE, EXT)
    E(0x7A, DEC,   BYTE, EXT)
    E(0x7C, INC,   BYTE, EXT)
    E(0x7D, TST,   BYTE, EXT)
    E(0x7E, JMP,   NONE, EXT)
    E(0x7F, CLR,   BYTE, EXT)
    E(0x80, SUBA,  BYTE, IMM)
    E(0x81, CMPA,  BYTE, IMM)
    E(0x82, SBCA,  BYTE, IMM)
    E(0x83, SUBD,  WORD, IMM)
    E(0x84, ANDA,  BYTE, IMM)
    E(0x85, BITA,  BYTE, IMM)
    E(0x86, LDA,   BYTE, IMM)
    E(0x88, EORA,  BYTE, IMM)
    E(0x89, ADCA,  BYTE, IMM)
    E(0x8A, ORA,   BYTE, IMM)
    E(0x8B, ADDA,  BYTE, IMM)
    E(0x8C, CMPX,  WORD, IMM)
    E(0x8D, BSR,   BYTE, REL)
    E(0x8E, LDX,   WORD, IMM)
    E(0x90, SUBA,  BYTE, DIR)
    E(0x91, CMPA,  BYTE, DIR)
    E(0x92, SBCA,  BYTE, DIR)
    E(0x93, SUBD,  WORD, DIR)
    E(0x94, ANDA,  BYTE, DIR)
    E(0x95, BITA,  BYTE, DIR)
    E(0x96, LDA,   BYTE, DIR)
    E(0x97, STA,   BYTE, DIR)
    E(0x98, EORA,  BYTE, DIR)
    E(0x99, ADCA,  BYTE, DIR)
    E(0x9A, ORA,   BYTE, DIR)
    E(0x9B, ADDA,  BYTE, DIR)
    E(0x9C, CMPX,  WORD, DIR)
    E(0x9D, JSR,   NONE, DIR)
    E(0x9E, LDX,   WORD, DIR)
    E(0x9F, STX,   WORD, DIR)
    E(0xA0, SUBA,  BYTE, IDX)
    E(0xA1, CMPA,  BYTE, IDX)
    E(0xA2, SBCA,  BYTE, IDX)
    E(0xA3, SUBD,  WORD, IDX)
    E(0xA4, ANDA,  BYTE, IDX)
    E(0xA5, BITA,  BYTE, IDX)
    E(0xA6, LDA,   BYTE, IDX)
    E(0xA7, STA,   BYTE, IDX)
    E(0xA8, EORA,  BYTE, IDX)
    E(0xA9, ADCA,  BYTE, IDX)
    E(0xAA, ORA,   BYTE, IDX)
    E(0xAB, ADDA,  BYTE, IDX)
    E(0xAC, CMPX,  WORD, IDX)
    E(0xAD, JSR,   NONE, IDX)
    E(0xAE, LDX,   WORD, IDX)
    E(0xAF, STX,   WORD, IDX)
    E(0xB0, SUBA,  BYTE, EXT)
    E(0xB1, CMPA,  BYTE, EXT)
    E(0xB2, SBCA,  BYTE, EXT)
    E(0xB3, SUBD,  BYTE, EXT)
    E(0xB4, ANDA,  BYTE, EXT)
    E(0xB5, BITA,  BYTE, EXT)
    E(0xB6, LDA,   BYTE, EXT)
    E(0xB7, STA,   BYTE, EXT)
    E(0xB8, EORA,  BYTE, EXT)
    E(0xB9, ADCA,  BYTE, EXT)
    E(0xBA, ORA,   BYTE, EXT)
    E(0xBB, ADDA,  BYTE, EXT)
    E(0xBC, CMPX,  WORD, EXT)
    E(0xBD, JSR,   NONE, EXT)
    E(0xBE, LDX,   WORD, EXT)
    E(0xBF, STX,   WORD, EXT)
    E(0xC0, SUBB,  BYTE, IMM)
    E(0xC1, CMPB,  BYTE, IMM)
    E(0xC2, SBCB,  BYTE, IMM)
    E(0xC3, ADDD,  WORD, IMM)
    E(0xC4, ANDB,  BYTE, IMM)
    E(0xC5, BITB,  BYTE, IMM)
    E(0xC6, LDB,   BYTE, IMM)
    E(0xC8, EORB,  BYTE, IMM)
    E(0xC9, ADCB,  BYTE, IMM)
    E(0xCA, ORB,   BYTE, IMM)
    E(0xCB, ADDB,  BYTE, IMM)
    E(0xCC, LDD,   WORD, IMM)
    E(0xCE, LDU,   WORD, IMM)
    E(0xD0, SUBB,  BYTE, DIR)
    E(0xD1, CMPB,  BYTE, DIR)
    E(0xD2, SBCB,  BYTE, DIR)
    E(0xD3, ADDD,  WORD, DIR)
    E(0xD4, ANDB,  BYTE, DIR)
    E(0xD5, BITB,  BYTE, DIR)
    E(0xD6, LDB,   BYTE, DIR)
    E(0xD7, STB,   BYTE, DIR)
    E(0xD8, EORB,  BYTE, DIR)
    E(0xD9, ADCB,  BYTE, DIR)
    E(0xDA, ORB,   BYTE, DIR)
    E(0xDB, ADDB,  BYTE, DIR)
    E(0xDC, LDD,   WORD, DIR)
    E(0xDD, STD,   WORD, DIR)
    E(0xDE, LDU,   WORD, DIR)
    E(0xDF, STU,   WORD, DIR)
    E(0xE0, SUBB,  BYTE, IDX)
    E(0xE1, CMPB,  BYTE, IDX)
    E(0xE2, SBCB,  BYTE, IDX)
    E(0xE3, ADDD,  WORD, IDX)
    E(0xE4, ANDB,  BYTE, IDX)
    E(0xE5, BITB,  BYTE, IDX)
    E(0xE6, LDB,   BYTE, IDX)
    E(0xE7, STB,   BYTE, IDX)
    E(0xE8, EORB,  BYTE, IDX)
    E(0xE9, ADCB,  BYTE, IDX)
    E(0xEA, ORB,   BYTE, IDX)
    E(0xEB, ADDB,  BYTE, IDX)
    E(0xEC, LDD,   WORD, IDX)
    E(0xED, STD,   WORD, IDX)
    E(0xEE, LDU,   WORD, IDX)
    E(0xEF, STU,   WORD, IDX)
    E(0xF0, SUBB,  BYTE, EXT)
    E(0xF1, CMPB,  BYTE, EXT)
    E(0xF2, SBCB,  BYTE, EXT)
    E(0xF3, ADDD,  WORD, EXT)
    E(0xF4, ANDB,  BYTE, EXT)
    E(0xF5, BITB,  BYTE, EXT)
    E(0xF6, LDB,   BYTE, EXT)
    E(0xF7, STB,   BYTE, EXT)
    E(0xF8, EORB,  BYTE, EXT)
    E(0xF9, ADCB,  BYTE, EXT)
    E(0xFA, ORB,   BYTE, EXT)
    E(0xFB, ADDB,  BYTE, EXT)
    E(0xFC, LDD,   WORD, EXT)
    E(0xFD, STD,   WORD, EXT)
    E(0xFE, LDU,   WORD, EXT)
    E(0xFF, STU,   WORD, EXT)
};

static constexpr Entry MC6809_P10[] PROGMEM = {
    E(0x21, LBRN,  WORD, REL)
    E(0x22, LBHI,  WORD, REL)
    E(0x23, LBLS,  WORD, REL)
    E(0x24, LBHS,  WORD, REL)
    E(0x24, LBCC,  WORD, REL)
    E(0x25, LBLO,  WORD, REL)
    E(0x25, LBCS,  WORD, REL)
    E(0x26, LBNE,  WORD, REL)
    E(0x27, LBEQ,  WORD, REL)
    E(0x28, LBVC,  WORD, REL)
    E(0x29, LBVS,  WORD, REL)
    E(0x2A, LBPL,  WORD, REL)
    E(0x2B, LBMI,  WORD, REL)
    E(0x2C, LBGE,  WORD, REL)
    E(0x2D, LBLT,  WORD, REL)
    E(0x2E, LBGT,  WORD, REL)
    E(0x2F, LBLE,  WORD, REL)
    E(0x3F, SWI2,  NONE, INH)
    E(0x83, CMPD,  WORD, IMM)
    E(0x8C, CMPY,  WORD, IMM)
    E(0x8E, LDY,   WORD, IMM)
    E(0x93, CMPD,  WORD, DIR)
    E(0x9C, CMPY,  WORD, DIR)
    E(0x9E, LDY,   WORD, DIR)
    E(0x9F, STY,   WORD, DIR)
    E(0xA3, CMPD,  WORD, IDX)
    E(0xAC, CMPY,  WORD, IDX)
    E(0xAE, LDY,   WORD, IDX)
    E(0xAF, STY,   WORD, IDX)
    E(0xB3, CMPD,  WORD, EXT)
    E(0xBC, CMPY,  WORD, EXT)
    E(0xBE, LDY,   WORD, EXT)
    E(0xBF, STY,   WORD, EXT)
    E(0xCE, LDS,   WORD, IMM)
    E(0xDE, LDS,   WORD, DIR)
    E(0xDF, STS,   WORD, DIR)
    E(0xEE, LDS,   WORD, IDX)
    E(0xEF, STS,   WORD, IDX)
    E(0xFE, LDS,   WORD, EXT)
    E(0xFF, STS,   WORD, EXT)
};

static constexpr Entry MC6809_P11[] PROGMEM = {
    E(0x3F, SWI3,  NONE, INH)
    E(0x83, CMPU,  WORD, IMM)
    E(0x8C, CMPS,  WORD, IMM)
    E(0x93, CMPU,  WORD, DIR)
    E(0x9C, CMPS,  WORD, DIR)
    E(0xA3, CMPU,  WORD, IDX)
    E(0xAC, CMPS,  WORD, IDX)
    E(0xB3, CMPU,  WORD, EXT)
    E(0xBC, CMPS,  WORD, EXT)
};

static constexpr Entry HD6309_P00[] PROGMEM = {
    E(0x01, OIM,   BYTE, IMM_DIR)
    E(0x02, AIM,   BYTE, IMM_DIR)
    E(0x05, EIM,   BYTE, IMM_DIR)
    E(0x0B, TIM,   BYTE, IMM_DIR)
    E(0x14, SEXW,  WORD, INH)
    E(0x61, OIM,   BYTE, IMM_IDX)
    E(0x62, AIM,   BYTE, IMM_IDX)
    E(0x65, EIM,   BYTE, IMM_IDX)
    E(0x6B, TIM,   BYTE, IMM_IDX)
    E(0x71, OIM,   BYTE, IMM_EXT)
    E(0x72, AIM,   BYTE, IMM_EXT)
    E(0x75, EIM,   BYTE, IMM_EXT)
    E(0x7B, TIM,   BYTE, IMM_EXT)
    E(0xCD, LDQ,   LONG, IMM)
};

static constexpr Entry HD6309_P10[] PROGMEM = {
    E(0x30, ADDR,  NONE, REG_REG)
    E(0x31, ADCR,  NONE, REG_REG)
    E(0x32, SUBR,  NONE, REG_REG)
    E(0x33, SBCR,  NONE, REG_REG)
    E(0x34, ANDR,  NONE, REG_REG)
    E(0x35, ORR,   NONE, REG_REG)
    E(0x36, EORR,  NONE, REG_REG)
    E(0x37, CMPR,  NONE, REG_REG)
    E(0x38, PSHSW, NONE, INH)
    E(0x39, PULSW, NONE, INH)
    E(0x3A, PSHUW, NONE, INH)
    E(0x3B, PULUW, NONE, INH)
    E(0x40, NEGD,  WORD, INH)
    E(0x43, COMD,  WORD, INH)
    E(0x44, LSRD,  WORD, INH)
    E(0x46, RORD,  WORD, INH)
    E(0x47, ASRD,  WORD, INH)
    E(0x48, ASLD,  WORD, INH)
    E(0x48, LSLD,  WORD, INH)
    E(0x49, ROLD,  WORD, INH)
    E(0x4A, DECD,  WORD, INH)
    E(0x4C, INCD,  WORD, INH)
    E(0x4D, TSTD,  WORD, INH)
    E(0x4F, CLRD,  WORD, INH)
    E(0x53, COMW,  WORD, INH)
    E(0x54, LSRW,  WORD, INH)
    E(0x56, RORW,  WORD, INH)
    E(0x59, ROLW,  WORD, INH)
    E(0x5A, DECW,  WORD, INH)
    E(0x5C, INCW,  WORD, INH)
    E(0x5D, TSTW,  WORD, INH)
    E(0x5F, CLRW,  WORD, INH)
    E(0x80, SUBW,  WORD, IMM)
    E(0x81, CMPW,  WORD, IMM)
    E(0x82, SBCD,  WORD, IMM)
    E(0x84, ANDD,  WORD, IMM)
    E(0x85, BITD,  WORD, IMM)
    E(0x86, LDW,   WORD, IMM)
    E(0x88, EORD,  WORD, IMM)
    E(0x89, ADCD,  WORD, IMM)
    E(0x8A, ORD,   WORD, IMM)
    E(0x8B, ADDW,  WORD, IMM)
    E(0x90, SUBW,  WORD, DIR)
    E(0x91, CMPW,  WORD, DIR)
    E(0x92, SBCD,  WORD, DIR)
    E(0x94, ANDD,  WORD, DIR)
    E(0x95, BITD,  WORD, DIR)
    E(0x96, LDW,   WORD, DIR)
    E(0x97, STW,   WORD, DIR)
    E(0x98, EORD,  WORD, DIR)
    E(0x99, ADCD,  WORD, DIR)
    E(0x9A, ORD,   WORD, DIR)
    E(0x9B, ADDW,  WORD, DIR)
    E(0xA0, SUBW,  WORD, IDX)
    E(0xA1, CMPW,  WORD, IDX)
    E(0xA2, SBCD,  WORD, IDX)
    E(0xA4, ANDD,  WORD, IDX)
    E(0xA5, BITD,  WORD, IDX)
    E(0xA6, LDW,   WORD, IDX)
    E(0xA7, STW,   WORD, IDX)
    E(0xA8, EORD,  WORD, IDX)
    E(0xA9, ADCD,  WORD, IDX)
    E(0xAA, ORD,   WORD, IDX)
    E(0xAB, ADDW,  WORD, IDX)
    E(0xB0, SUBW,  WORD, EXT)
    E(0xB1, CMPW,  WORD, EXT)
    E(0xB2, SBCD,  WORD, EXT)
    E(0xB4, ANDD,  WORD, EXT)
    E(0xB5, BITD,  WORD, EXT)
    E(0xB6, LDW,   WORD, EXT)
    E(0xB7, STW,   WORD, EXT)
    E(0xB8, EORD,  WORD, EXT)
    E(0xB9, ADCD,  WORD, EXT)
    E(0xBA, ORD,   WORD, EXT)
    E(0xBB, ADDW,  WORD, EXT)
    E(0xDC, LDQ,   LONG, DIR)
    E(0xDD, STQ,   LONG, DIR)
    E(0xEC, LDQ,   LONG, IDX)
    E(0xED, STQ,   LONG, IDX)
    E(0xFC, LDQ,   LONG, EXT)
    E(0xFD, STQ,   LONG, EXT)
};

static constexpr Entry HD6309_P11[] PROGMEM = {
    E(0x30, BAND,  BYTE, BITOP)
    E(0x31, BIAND, BYTE, BITOP)
    E(0x32, BOR,   BYTE, BITOP)
    E(0x33, BIOR,  BYTE, BITOP)
    E(0x34, BEOR,  BYTE, BITOP)
    E(0x35, BIEOR, BYTE, BITOP)
    E(0x36, LDBT,  BYTE, BITOP)
    E(0x37, STBT,  BYTE, BITOP)
    E(0x38, TFM,   NONE, TFR_MEM) /* R+,R+ */
    E(0x39, TFM,   NONE, TFR_MEM) /* R-,R- */
    E(0x3A, TFM,   NONE, TFR_MEM) /* R+,R */
    E(0x3B, TFM,   NONE, TFR_MEM) /* R,R+ */
    E(0x3C, BITMD, BYTE, IMM)
    E(0x3D, LDMD,  BYTE, IMM)
    E(0x43, COME,  BYTE, INH)
    E(0x4A, DECE,  BYTE, INH)
    E(0x4C, INCE,  BYTE, INH)
    E(0x4D, TSTE,  BYTE, INH)
    E(0x4F, CLRE,  BYTE, INH)
    E(0x53, COMF,  BYTE, INH)
    E(0x5A, DECF,  BYTE, INH)
    E(0x5C, INCF,  BYTE, INH)
    E(0x5D, TSTF,  BYTE, INH)
    E(0x5F, CLRF,  BYTE, INH)
    E(0x80, SUBE,  BYTE, IMM)
    E(0x81, CMPE,  BYTE, IMM)
    E(0x86, LDE,   BYTE, IMM)
    E(0x8B, ADDE,  BYTE, IMM)
    E(0x8D, DIVD,  WORD, IMM)
    E(0x8E, DIVQ,  WORD, IMM)
    E(0x8F, MULD,  WORD, IMM)
    E(0x90, SUBE,  BYTE, DIR)
    E(0x91, CMPE,  BYTE, DIR)
    E(0x96, LDE,   BYTE, DIR)
    E(0x97, STE,   BYTE, DIR)
    E(0x9B, ADDE,  BYTE, DIR)
    E(0x9D, DIVD,  BYTE, DIR)
    E(0x9E, DIVQ,  WORD, DIR)
    E(0x9F, MULD,  WORD, DIR)
    E(0xA0, SUBE,  BYTE, IDX)
    E(0xA1, CMPE,  BYTE, IDX)
    E(0xA6, LDE,   BYTE, IDX)
    E(0xA7, STE,   BYTE, IDX)
    E(0xAB, ADDE,  BYTE, IDX)
    E(0xAD, DIVD,  BYTE, IDX)
    E(0xAE, DIVQ,  WORD, IDX)
    E(0xAF, MULD,  WORD, IDX)
    E(0xB0, SUBE,  BYTE, EXT)
    E(0xB1, CMPE,  BYTE, EXT)
    E(0xB6, LDE,   BYTE, EXT)
    E(0xB7, STE,   BYTE, EXT)
    E(0xBB, ADDE,  BYTE, EXT)
    E(0xBD, DIVD,  BYTE, EXT)
    E(0xBE, DIVQ,  WORD, EXT)
    E(0xBF, MULD,  WORD, EXT)
    E(0xC0, SUBF,  BYTE, IMM)
    E(0xC1, CMPF,  BYTE, IMM)
    E(0xC6, LDF,   BYTE, IMM)
    E(0xCB, ADDF,  BYTE, IMM)
    E(0xD0, SUBF,  BYTE, DIR)
    E(0xD1, CMPF,  BYTE, DIR)
    E(0xD6, LDF,   BYTE, DIR)
    E(0xD7, STF,   BYTE, DIR)
    E(0xDB, ADDF,  BYTE, DIR)
    E(0xE0, SUBF,  BYTE, IDX)
    E(0xE1, CMPF,  BYTE, IDX)
    E(0xE6, LDF,   BYTE, IDX)
    E(0xE7, STF,   BYTE, IDX)
    E(0xEB, ADDF,  BYTE, IDX)
    E(0xF0, SUBF,  BYTE, EXT)
    E(0xF1, CMPF,  BYTE, EXT)
    E(0xF6, LDF,   BYTE, EXT)
    E(0xF7, STF,   BYTE, EXT)
    E(0xFB, ADDF,  BYTE, EXT)
};

static constexpr Config::opcode_t PREFIX_P00 = 0x00;
static constexpr Config::opcode_t PREFIX_P10 = 0x10;
static constexpr Config::opcode_t PREFIX_P11 = 0x11;

bool TableMc6809::isPrefixCode(Config::opcode_t opCode) {
    return opCode == PREFIX_P10 || opCode == PREFIX_P11;
}

struct TableMc6809::EntryPage {
    const Config::opcode_t prefix;
    const Entry *const table;
    const Entry *const end;
};

static constexpr TableMc6809::EntryPage MC6809_PAGES[] PROGMEM = {
    { PREFIX_P00, ARRAY_RANGE(MC6809_P00) },
    { PREFIX_P10, ARRAY_RANGE(MC6809_P10) },
    { PREFIX_P11, ARRAY_RANGE(MC6809_P11) },
};

static constexpr TableMc6809::EntryPage HD6309_PAGES[] PROGMEM = {
    { PREFIX_P00, ARRAY_RANGE(MC6809_P00) },
    { PREFIX_P10, ARRAY_RANGE(MC6809_P10) },
    { PREFIX_P11, ARRAY_RANGE(MC6809_P11) },
    { PREFIX_P00, ARRAY_RANGE(HD6309_P00) },
    { PREFIX_P10, ARRAY_RANGE(HD6309_P10) },
    { PREFIX_P11, ARRAY_RANGE(HD6309_P11) },
};

Error TableMc6809::searchName(
    InsnMc6809 &insn, const EntryPage *pages, const EntryPage *end) {
    const char *name = insn.name();
    for (const EntryPage *page = pages; page < end; page++) {
        const Entry *table = reinterpret_cast<Entry *>(pgm_read_ptr(&page->table));
        const Entry *end = reinterpret_cast<Entry *>(pgm_read_ptr(&page->end));
        const Entry *entry = TableBase::searchName<Entry>(name, table, end);
        if (entry) {
            const Config::opcode_t prefix = pgm_read_byte(&page->prefix);
            insn.setOpCode(pgm_read_byte(&entry->opCode), prefix);
            insn.setFlags(pgm_read_byte(&entry->flags));
            return OK;
        }
    }
    return UNKNOWN_INSTRUCTION;
}

static bool matchAddrMode(AddrMode addrMode, const Entry *entry) {
    const uint8_t flags = pgm_read_byte(&entry->flags);
    return addrMode == Entry::_addrMode(flags);
}

Error TableMc6809::searchNameAndAddrMode(
    InsnMc6809 &insn, const EntryPage *pages, const EntryPage *end) {
    const AddrMode addrMode = insn.addrMode();
    for (const EntryPage *page = pages; page < end; page++) {
        const Entry *table = reinterpret_cast<Entry *>(pgm_read_ptr(&page->table));
        const Entry *end = reinterpret_cast<Entry *>(pgm_read_ptr(&page->end));
        const Entry *entry = TableBase::searchName<Entry,AddrMode>(
            insn.name(), addrMode, table, end, matchAddrMode);
        if (entry) {
            const Config::opcode_t prefix = pgm_read_byte(&page->prefix);
            insn.setOpCode(pgm_read_byte(&entry->opCode), prefix);
            insn.setFlags(pgm_read_byte(&entry->flags));
            return OK;
        }
    }
    return UNKNOWN_INSTRUCTION;
}

Error TableMc6809::searchOpCode(
    InsnMc6809 &insn, const EntryPage *pages, const EntryPage *end) {
    for (const EntryPage *page = pages; page < end; page++) {
        const Config::opcode_t prefix = pgm_read_byte(&page->prefix);
        if (insn.prefixCode() != prefix) continue;
        const Entry *table = reinterpret_cast<Entry *>(pgm_read_ptr(&page->table));
        const Entry *end = reinterpret_cast<Entry *>(pgm_read_ptr(&page->end));
        const Entry *entry = TableBase::searchCode<Entry,Config::opcode_t>(
            insn.opCode(), table, end);
        if (entry) {
            const char *name =
                reinterpret_cast<const char *>(pgm_read_ptr(&entry->name));
            TableBase::setName(insn.insn(), name, Config::NAME_MAX);
            insn.setFlags(pgm_read_byte(&entry->flags));
            return OK;
        }
    }
    return UNKNOWN_INSTRUCTION;
}

Error TableMc6809::searchName(InsnMc6809 &insn) const {
    return searchName(insn, _table, _end);
}

Error TableMc6809::searchNameAndAddrMode(InsnMc6809 &insn) const {
    return searchNameAndAddrMode(insn, _table, _end);
}

Error TableMc6809::searchOpCode(InsnMc6809 &insn) const {
    return searchOpCode(insn, _table, _end);
}

TableMc6809::TableMc6809() {
    setCpu(MC6809);
}

void TableMc6809::setCpu(CpuType cpuType) {
    _cpuType = cpuType;
    if (cpuType == MC6809) {
        _table = ARRAY_BEGIN(MC6809_PAGES);
        _end = ARRAY_END(MC6809_PAGES);
    } else {
        _table = ARRAY_BEGIN(HD6309_PAGES);
        _end = ARRAY_END(HD6309_PAGES);
    }
}

const char *TableMc6809::getCpu() {
    return _cpuType == MC6809 ? "6809" : "6309";
}

bool TableMc6809::setCpu(const char *cpu) {
    const char *p;
    p = cpu + (strncasecmp(cpu, "MC", 2) ? 0 : 2);
    if (strcmp(p, "6809") == 0) {
        setCpu(MC6809);
        return true;
    }
    p = cpu + (strncasecmp(cpu, "HD", 2) ? 0 : 2);
    if (strcmp(p, "6309") == 0) {
        setCpu(HD6309);
        return true;
    }
    return false;
}

class TableMc6809 TableMc6809;

} // namespace mc6809
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

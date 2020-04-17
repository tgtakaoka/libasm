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
    E(0x00, NEG,   BYTE, DIRP)
    E(0x03, COM,   BYTE, DIRP)
    E(0x04, LSR,   BYTE, DIRP)
    E(0x06, ROR,   BYTE, DIRP)
    E(0x07, ASR,   BYTE, DIRP)
    E(0x08, ASL,   BYTE, DIRP)
    E(0x08, LSL,   BYTE, DIRP)
    E(0x09, ROL,   BYTE, DIRP)
    E(0x0A, DEC,   BYTE, DIRP)
    E(0x0C, INC,   BYTE, DIRP)
    E(0x0D, TST,   BYTE, DIRP)
    E(0x0E, JMP,   NONE, DIRP)
    E(0x0F, CLR,   BYTE, DIRP)
    E(0x12, NOP,   NONE, INHR)
    E(0x13, SYNC,  NONE, INHR)
    E(0x16, LBRA,  WORD, REL)
    E(0x17, LBSR,  WORD, REL)
    E(0x19, DAA,   NONE, INHR)
    E(0x1A, ORCC,  BYTE, IMM)
    E(0x1C, ANDCC, BYTE, IMM)
    E(0x1D, SEX,   BYTE, INHR)
    E(0x1E, EXG,   NONE, REGS)
    E(0x1F, TFR,   NONE, REGS)
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
    E(0x30, LEAX,  NONE, INDX)
    E(0x31, LEAY,  NONE, INDX)
    E(0x32, LEAS,  NONE, INDX)
    E(0x33, LEAU,  NONE, INDX)
    E(0x34, PSHS,  BYTE, STKOP)
    E(0x35, PULS,  BYTE, STKOP)
    E(0x36, PSHU,  BYTE, STKOP)
    E(0x37, PULU,  BYTE, STKOP)
    E(0x39, RTS,   NONE, INHR)
    E(0x3A, ABX,   NONE, INHR)
    E(0x3B, RTI,   NONE, INHR)
    E(0x3C, CWAI,  BYTE, IMM)
    E(0x3D, MUL,   BYTE, INHR)
    E(0x3F, SWI,   NONE, INHR)
    E(0x40, NEGA,  BYTE, INHR)
    E(0x43, COMA,  BYTE, INHR)
    E(0x44, LSRA,  BYTE, INHR)
    E(0x46, RORA,  BYTE, INHR)
    E(0x47, ASRA,  BYTE, INHR)
    E(0x48, ASLA,  BYTE, INHR)
    E(0x48, LSLA,  BYTE, INHR)
    E(0x49, ROLA,  BYTE, INHR)
    E(0x4A, DECA,  BYTE, INHR)
    E(0x4C, INCA,  BYTE, INHR)
    E(0x4D, TSTA,  BYTE, INHR)
    E(0x4F, CLRA,  BYTE, INHR)
    E(0x50, NEGB,  BYTE, INHR)
    E(0x53, COMB,  BYTE, INHR)
    E(0x54, LSRB,  BYTE, INHR)
    E(0x56, RORB,  BYTE, INHR)
    E(0x57, ASRB,  BYTE, INHR)
    E(0x58, ASLB,  BYTE, INHR)
    E(0x58, LSLB,  BYTE, INHR)
    E(0x59, ROLB,  BYTE, INHR)
    E(0x5A, DECB,  BYTE, INHR)
    E(0x5C, INCB,  BYTE, INHR)
    E(0x5D, TSTB,  BYTE, INHR)
    E(0x5F, CLRB,  BYTE, INHR)
    E(0x60, NEG,   BYTE, INDX)
    E(0x63, COM,   BYTE, INDX)
    E(0x64, LSR,   BYTE, INDX)
    E(0x66, ROR,   BYTE, INDX)
    E(0x67, ASR,   BYTE, INDX)
    E(0x68, ASL,   BYTE, INDX)
    E(0x68, LSL,   BYTE, INDX)
    E(0x69, ROL,   BYTE, INDX)
    E(0x6A, DEC,   BYTE, INDX)
    E(0x6C, INC,   BYTE, INDX)
    E(0x6D, TST,   BYTE, INDX)
    E(0x6E, JMP,   NONE, INDX)
    E(0x6F, CLR,   BYTE, INDX)
    E(0x70, NEG,   BYTE, EXTD)
    E(0x73, COM,   BYTE, EXTD)
    E(0x74, LSR,   BYTE, EXTD)
    E(0x76, ROR,   BYTE, EXTD)
    E(0x77, ASR,   BYTE, EXTD)
    E(0x78, ASL,   BYTE, EXTD)
    E(0x78, LSL,   BYTE, EXTD)
    E(0x79, ROL,   BYTE, EXTD)
    E(0x7A, DEC,   BYTE, EXTD)
    E(0x7C, INC,   BYTE, EXTD)
    E(0x7D, TST,   BYTE, EXTD)
    E(0x7E, JMP,   NONE, EXTD)
    E(0x7F, CLR,   BYTE, EXTD)
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
    E(0x90, SUBA,  BYTE, DIRP)
    E(0x91, CMPA,  BYTE, DIRP)
    E(0x92, SBCA,  BYTE, DIRP)
    E(0x93, SUBD,  WORD, DIRP)
    E(0x94, ANDA,  BYTE, DIRP)
    E(0x95, BITA,  BYTE, DIRP)
    E(0x96, LDA,   BYTE, DIRP)
    E(0x97, STA,   BYTE, DIRP)
    E(0x98, EORA,  BYTE, DIRP)
    E(0x99, ADCA,  BYTE, DIRP)
    E(0x9A, ORA,   BYTE, DIRP)
    E(0x9B, ADDA,  BYTE, DIRP)
    E(0x9C, CMPX,  WORD, DIRP)
    E(0x9D, JSR,   NONE, DIRP)
    E(0x9E, LDX,   WORD, DIRP)
    E(0x9F, STX,   WORD, DIRP)
    E(0xA0, SUBA,  BYTE, INDX)
    E(0xA1, CMPA,  BYTE, INDX)
    E(0xA2, SBCA,  BYTE, INDX)
    E(0xA3, SUBD,  WORD, INDX)
    E(0xA4, ANDA,  BYTE, INDX)
    E(0xA5, BITA,  BYTE, INDX)
    E(0xA6, LDA,   BYTE, INDX)
    E(0xA7, STA,   BYTE, INDX)
    E(0xA8, EORA,  BYTE, INDX)
    E(0xA9, ADCA,  BYTE, INDX)
    E(0xAA, ORA,   BYTE, INDX)
    E(0xAB, ADDA,  BYTE, INDX)
    E(0xAC, CMPX,  WORD, INDX)
    E(0xAD, JSR,   NONE, INDX)
    E(0xAE, LDX,   WORD, INDX)
    E(0xAF, STX,   WORD, INDX)
    E(0xB0, SUBA,  BYTE, EXTD)
    E(0xB1, CMPA,  BYTE, EXTD)
    E(0xB2, SBCA,  BYTE, EXTD)
    E(0xB3, SUBD,  BYTE, EXTD)
    E(0xB4, ANDA,  BYTE, EXTD)
    E(0xB5, BITA,  BYTE, EXTD)
    E(0xB6, LDA,   BYTE, EXTD)
    E(0xB7, STA,   BYTE, EXTD)
    E(0xB8, EORA,  BYTE, EXTD)
    E(0xB9, ADCA,  BYTE, EXTD)
    E(0xBA, ORA,   BYTE, EXTD)
    E(0xBB, ADDA,  BYTE, EXTD)
    E(0xBC, CMPX,  WORD, EXTD)
    E(0xBD, JSR,   NONE, EXTD)
    E(0xBE, LDX,   WORD, EXTD)
    E(0xBF, STX,   WORD, EXTD)
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
    E(0xD0, SUBB,  BYTE, DIRP)
    E(0xD1, CMPB,  BYTE, DIRP)
    E(0xD2, SBCB,  BYTE, DIRP)
    E(0xD3, ADDD,  WORD, DIRP)
    E(0xD4, ANDB,  BYTE, DIRP)
    E(0xD5, BITB,  BYTE, DIRP)
    E(0xD6, LDB,   BYTE, DIRP)
    E(0xD7, STB,   BYTE, DIRP)
    E(0xD8, EORB,  BYTE, DIRP)
    E(0xD9, ADCB,  BYTE, DIRP)
    E(0xDA, ORB,   BYTE, DIRP)
    E(0xDB, ADDB,  BYTE, DIRP)
    E(0xDC, LDD,   WORD, DIRP)
    E(0xDD, STD,   WORD, DIRP)
    E(0xDE, LDU,   WORD, DIRP)
    E(0xDF, STU,   WORD, DIRP)
    E(0xE0, SUBB,  BYTE, INDX)
    E(0xE1, CMPB,  BYTE, INDX)
    E(0xE2, SBCB,  BYTE, INDX)
    E(0xE3, ADDD,  WORD, INDX)
    E(0xE4, ANDB,  BYTE, INDX)
    E(0xE5, BITB,  BYTE, INDX)
    E(0xE6, LDB,   BYTE, INDX)
    E(0xE7, STB,   BYTE, INDX)
    E(0xE8, EORB,  BYTE, INDX)
    E(0xE9, ADCB,  BYTE, INDX)
    E(0xEA, ORB,   BYTE, INDX)
    E(0xEB, ADDB,  BYTE, INDX)
    E(0xEC, LDD,   WORD, INDX)
    E(0xED, STD,   WORD, INDX)
    E(0xEE, LDU,   WORD, INDX)
    E(0xEF, STU,   WORD, INDX)
    E(0xF0, SUBB,  BYTE, EXTD)
    E(0xF1, CMPB,  BYTE, EXTD)
    E(0xF2, SBCB,  BYTE, EXTD)
    E(0xF3, ADDD,  WORD, EXTD)
    E(0xF4, ANDB,  BYTE, EXTD)
    E(0xF5, BITB,  BYTE, EXTD)
    E(0xF6, LDB,   BYTE, EXTD)
    E(0xF7, STB,   BYTE, EXTD)
    E(0xF8, EORB,  BYTE, EXTD)
    E(0xF9, ADCB,  BYTE, EXTD)
    E(0xFA, ORB,   BYTE, EXTD)
    E(0xFB, ADDB,  BYTE, EXTD)
    E(0xFC, LDD,   WORD, EXTD)
    E(0xFD, STD,   WORD, EXTD)
    E(0xFE, LDU,   WORD, EXTD)
    E(0xFF, STU,   WORD, EXTD)

    // Pseudo instruction to set DP on assembler
    E(PSEUDO_SETDP,  SETDP,  BYTE, PSEUDO)
    E(PSEUDO_ASSUME, ASSUME, BYTE, PSEUDO)
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
    E(0x3F, SWI2,  NONE, INHR)
    E(0x83, CMPD,  WORD, IMM)
    E(0x8C, CMPY,  WORD, IMM)
    E(0x8E, LDY,   WORD, IMM)
    E(0x93, CMPD,  WORD, DIRP)
    E(0x9C, CMPY,  WORD, DIRP)
    E(0x9E, LDY,   WORD, DIRP)
    E(0x9F, STY,   WORD, DIRP)
    E(0xA3, CMPD,  WORD, INDX)
    E(0xAC, CMPY,  WORD, INDX)
    E(0xAE, LDY,   WORD, INDX)
    E(0xAF, STY,   WORD, INDX)
    E(0xB3, CMPD,  WORD, EXTD)
    E(0xBC, CMPY,  WORD, EXTD)
    E(0xBE, LDY,   WORD, EXTD)
    E(0xBF, STY,   WORD, EXTD)
    E(0xCE, LDS,   WORD, IMM)
    E(0xDE, LDS,   WORD, DIRP)
    E(0xDF, STS,   WORD, DIRP)
    E(0xEE, LDS,   WORD, INDX)
    E(0xEF, STS,   WORD, INDX)
    E(0xFE, LDS,   WORD, EXTD)
    E(0xFF, STS,   WORD, EXTD)
};

static constexpr Entry MC6809_P11[] PROGMEM = {
    E(0x3F, SWI3,  NONE, INHR)
    E(0x83, CMPU,  WORD, IMM)
    E(0x8C, CMPS,  WORD, IMM)
    E(0x93, CMPU,  WORD, DIRP)
    E(0x9C, CMPS,  WORD, DIRP)
    E(0xA3, CMPU,  WORD, INDX)
    E(0xAC, CMPS,  WORD, INDX)
    E(0xB3, CMPU,  WORD, EXTD)
    E(0xBC, CMPS,  WORD, EXTD)
};

static constexpr Entry HD6309_P00[] PROGMEM = {
    E(0x01, OIM,   BYTE, IMMDIR)
    E(0x02, AIM,   BYTE, IMMDIR)
    E(0x05, EIM,   BYTE, IMMDIR)
    E(0x0B, TIM,   BYTE, IMMDIR)
    E(0x14, SEXW,  WORD, INHR)
    E(0x61, OIM,   BYTE, IMMIDX)
    E(0x62, AIM,   BYTE, IMMIDX)
    E(0x65, EIM,   BYTE, IMMIDX)
    E(0x6B, TIM,   BYTE, IMMIDX)
    E(0x71, OIM,   BYTE, IMMEXT)
    E(0x72, AIM,   BYTE, IMMEXT)
    E(0x75, EIM,   BYTE, IMMEXT)
    E(0x7B, TIM,   BYTE, IMMEXT)
    E(0xCD, LDQ,   LONG, IMM)
};

static constexpr Entry HD6309_P10[] PROGMEM = {
    E(0x30, ADDR,  NONE, REGS)
    E(0x31, ADCR,  NONE, REGS)
    E(0x32, SUBR,  NONE, REGS)
    E(0x33, SBCR,  NONE, REGS)
    E(0x34, ANDR,  NONE, REGS)
    E(0x35, ORR,   NONE, REGS)
    E(0x36, EORR,  NONE, REGS)
    E(0x37, CMPR,  NONE, REGS)
    E(0x38, PSHSW, NONE, INHR)
    E(0x39, PULSW, NONE, INHR)
    E(0x3A, PSHUW, NONE, INHR)
    E(0x3B, PULUW, NONE, INHR)
    E(0x40, NEGD,  WORD, INHR)
    E(0x43, COMD,  WORD, INHR)
    E(0x44, LSRD,  WORD, INHR)
    E(0x46, RORD,  WORD, INHR)
    E(0x47, ASRD,  WORD, INHR)
    E(0x48, ASLD,  WORD, INHR)
    E(0x48, LSLD,  WORD, INHR)
    E(0x49, ROLD,  WORD, INHR)
    E(0x4A, DECD,  WORD, INHR)
    E(0x4C, INCD,  WORD, INHR)
    E(0x4D, TSTD,  WORD, INHR)
    E(0x4F, CLRD,  WORD, INHR)
    E(0x53, COMW,  WORD, INHR)
    E(0x54, LSRW,  WORD, INHR)
    E(0x56, RORW,  WORD, INHR)
    E(0x59, ROLW,  WORD, INHR)
    E(0x5A, DECW,  WORD, INHR)
    E(0x5C, INCW,  WORD, INHR)
    E(0x5D, TSTW,  WORD, INHR)
    E(0x5F, CLRW,  WORD, INHR)
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
    E(0x90, SUBW,  WORD, DIRP)
    E(0x91, CMPW,  WORD, DIRP)
    E(0x92, SBCD,  WORD, DIRP)
    E(0x94, ANDD,  WORD, DIRP)
    E(0x95, BITD,  WORD, DIRP)
    E(0x96, LDW,   WORD, DIRP)
    E(0x97, STW,   WORD, DIRP)
    E(0x98, EORD,  WORD, DIRP)
    E(0x99, ADCD,  WORD, DIRP)
    E(0x9A, ORD,   WORD, DIRP)
    E(0x9B, ADDW,  WORD, DIRP)
    E(0xA0, SUBW,  WORD, INDX)
    E(0xA1, CMPW,  WORD, INDX)
    E(0xA2, SBCD,  WORD, INDX)
    E(0xA4, ANDD,  WORD, INDX)
    E(0xA5, BITD,  WORD, INDX)
    E(0xA6, LDW,   WORD, INDX)
    E(0xA7, STW,   WORD, INDX)
    E(0xA8, EORD,  WORD, INDX)
    E(0xA9, ADCD,  WORD, INDX)
    E(0xAA, ORD,   WORD, INDX)
    E(0xAB, ADDW,  WORD, INDX)
    E(0xB0, SUBW,  WORD, EXTD)
    E(0xB1, CMPW,  WORD, EXTD)
    E(0xB2, SBCD,  WORD, EXTD)
    E(0xB4, ANDD,  WORD, EXTD)
    E(0xB5, BITD,  WORD, EXTD)
    E(0xB6, LDW,   WORD, EXTD)
    E(0xB7, STW,   WORD, EXTD)
    E(0xB8, EORD,  WORD, EXTD)
    E(0xB9, ADCD,  WORD, EXTD)
    E(0xBA, ORD,   WORD, EXTD)
    E(0xBB, ADDW,  WORD, EXTD)
    E(0xDC, LDQ,   LONG, DIRP)
    E(0xDD, STQ,   LONG, DIRP)
    E(0xEC, LDQ,   LONG, INDX)
    E(0xED, STQ,   LONG, INDX)
    E(0xFC, LDQ,   LONG, EXTD)
    E(0xFD, STQ,   LONG, EXTD)
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
    E(0x38, TFM,   NONE, TFRM) /* R+,R+ */
    E(0x39, TFM,   NONE, TFRM) /* R-,R- */
    E(0x3A, TFM,   NONE, TFRM) /* R+,R */
    E(0x3B, TFM,   NONE, TFRM) /* R,R+ */
    E(0x3C, BITMD, BYTE, IMM)
    E(0x3D, LDMD,  BYTE, IMM)
    E(0x43, COME,  BYTE, INHR)
    E(0x4A, DECE,  BYTE, INHR)
    E(0x4C, INCE,  BYTE, INHR)
    E(0x4D, TSTE,  BYTE, INHR)
    E(0x4F, CLRE,  BYTE, INHR)
    E(0x53, COMF,  BYTE, INHR)
    E(0x5A, DECF,  BYTE, INHR)
    E(0x5C, INCF,  BYTE, INHR)
    E(0x5D, TSTF,  BYTE, INHR)
    E(0x5F, CLRF,  BYTE, INHR)
    E(0x80, SUBE,  BYTE, IMM)
    E(0x81, CMPE,  BYTE, IMM)
    E(0x86, LDE,   BYTE, IMM)
    E(0x8B, ADDE,  BYTE, IMM)
    E(0x8D, DIVD,  WORD, IMM)
    E(0x8E, DIVQ,  WORD, IMM)
    E(0x8F, MULD,  WORD, IMM)
    E(0x90, SUBE,  BYTE, DIRP)
    E(0x91, CMPE,  BYTE, DIRP)
    E(0x96, LDE,   BYTE, DIRP)
    E(0x97, STE,   BYTE, DIRP)
    E(0x9B, ADDE,  BYTE, DIRP)
    E(0x9D, DIVD,  BYTE, DIRP)
    E(0x9E, DIVQ,  WORD, DIRP)
    E(0x9F, MULD,  WORD, DIRP)
    E(0xA0, SUBE,  BYTE, INDX)
    E(0xA1, CMPE,  BYTE, INDX)
    E(0xA6, LDE,   BYTE, INDX)
    E(0xA7, STE,   BYTE, INDX)
    E(0xAB, ADDE,  BYTE, INDX)
    E(0xAD, DIVD,  BYTE, INDX)
    E(0xAE, DIVQ,  WORD, INDX)
    E(0xAF, MULD,  WORD, INDX)
    E(0xB0, SUBE,  BYTE, EXTD)
    E(0xB1, CMPE,  BYTE, EXTD)
    E(0xB6, LDE,   BYTE, EXTD)
    E(0xB7, STE,   BYTE, EXTD)
    E(0xBB, ADDE,  BYTE, EXTD)
    E(0xBD, DIVD,  BYTE, EXTD)
    E(0xBE, DIVQ,  WORD, EXTD)
    E(0xBF, MULD,  WORD, EXTD)
    E(0xC0, SUBF,  BYTE, IMM)
    E(0xC1, CMPF,  BYTE, IMM)
    E(0xC6, LDF,   BYTE, IMM)
    E(0xCB, ADDF,  BYTE, IMM)
    E(0xD0, SUBF,  BYTE, DIRP)
    E(0xD1, CMPF,  BYTE, DIRP)
    E(0xD6, LDF,   BYTE, DIRP)
    E(0xD7, STF,   BYTE, DIRP)
    E(0xDB, ADDF,  BYTE, DIRP)
    E(0xE0, SUBF,  BYTE, INDX)
    E(0xE1, CMPF,  BYTE, INDX)
    E(0xE6, LDF,   BYTE, INDX)
    E(0xE7, STF,   BYTE, INDX)
    E(0xEB, ADDF,  BYTE, INDX)
    E(0xF0, SUBF,  BYTE, EXTD)
    E(0xF1, CMPF,  BYTE, EXTD)
    E(0xF6, LDF,   BYTE, EXTD)
    E(0xF7, STF,   BYTE, EXTD)
    E(0xFB, ADDF,  BYTE, EXTD)
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
            insn.setInsnCode(prefix, pgm_read_byte(&entry->opCode));
            insn.setFlags(pgm_read_byte(&entry->flags));
            return OK;
        }
    }
    return UNKNOWN_INSTRUCTION;
}

static bool matchAddrMode(AddrMode addrMode, const Entry *entry) {
    const host::uint_t flags = pgm_read_byte(&entry->flags);
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
            insn.setInsnCode(prefix, pgm_read_byte(&entry->opCode));
            insn.setFlags(pgm_read_byte(&entry->flags));
            return OK;
        }
    }
    return UNKNOWN_INSTRUCTION;
}

Error TableMc6809::searchInsnCode(
    InsnMc6809 &insn, const EntryPage *pages, const EntryPage *end) {
    for (const EntryPage *page = pages; page < end; page++) {
        const Config::opcode_t prefix = pgm_read_byte(&page->prefix);
        if (insn.prefixCode() != prefix) continue;
        const Entry *table = reinterpret_cast<Entry *>(pgm_read_ptr(&page->table));
        const Entry *end = reinterpret_cast<Entry *>(pgm_read_ptr(&page->end));
        const Entry *entry = TableBase::searchCode<Entry,Config::opcode_t>(
            insn.opCode(), table, end);
        if (entry) {
            insn.setFlags(pgm_read_byte(&entry->flags));
            TableBase::setName(insn.insn(), entry->name, Config::NAME_MAX);
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

Error TableMc6809::searchInsnCode(InsnMc6809 &insn) const {
    return searchInsnCode(insn, _table, _end);
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

const char *TableMc6809::listCpu() {
    return "6809, 6309";
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

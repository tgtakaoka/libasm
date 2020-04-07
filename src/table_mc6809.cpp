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

#define P00(_opc, _name, _sz, _mcu, _mode)                          \
    { _opc,  Entry::_flags(_mcu, SZ_##_sz, _mode), TEXT_##_name },
#define P10(_opc, _name, _sz, _mcu, _mode)                          \
    { _opc,  Entry::_flags(_mcu, SZ_##_sz, _mode), TEXT_##_name },
#define P11(_opc, _name,  _sz, _mcu, _mode)                         \
    { _opc,  Entry::_flags(_mcu, SZ_##_sz, _mode), TEXT_##_name },

static constexpr Entry MC6809_P00[] PROGMEM = {
    P00(0x00, NEG,   BYTE, MC6809, DIRP)
    P00(0x03, COM,   BYTE, MC6809, DIRP)
    P00(0x04, LSR,   BYTE, MC6809, DIRP)
    P00(0x06, ROR,   BYTE, MC6809, DIRP)
    P00(0x07, ASR,   BYTE, MC6809, DIRP)
    P00(0x08, ASL,   BYTE, MC6809, DIRP)
    P00(0x08, LSL,   BYTE, MC6809, DIRP)
    P00(0x09, ROL,   BYTE, MC6809, DIRP)
    P00(0x0A, DEC,   BYTE, MC6809, DIRP)
    P00(0x0C, INC,   BYTE, MC6809, DIRP)
    P00(0x0D, TST,   BYTE, MC6809, DIRP)
    P00(0x0E, JMP,   NONE, MC6809, DIRP)
    P00(0x0F, CLR,   BYTE, MC6809, DIRP)
    P00(0x12, NOP,   NONE, MC6809, INHR)
    P00(0x13, SYNC,  NONE, MC6809, INHR)
    P00(0x16, LBRA,  WORD, MC6809, REL)
    P00(0x17, LBSR,  WORD, MC6809, REL)
    P00(0x19, DAA,   NONE, MC6809, INHR)
    P00(0x1A, ORCC,  BYTE, MC6809, IMM)
    P00(0x1C, ANDCC, BYTE, MC6809, IMM)
    P00(0x1D, SEX,   BYTE, MC6809, INHR)
    P00(0x1E, EXG,   NONE, MC6809, REGS)
    P00(0x1F, TFR,   NONE, MC6809, REGS)
    P00(0x20, BRA,   BYTE, MC6809, REL)
    P00(0x21, BRN,   BYTE, MC6809, REL)
    P00(0x22, BHI,   BYTE, MC6809, REL)
    P00(0x23, BLS,   BYTE, MC6809, REL)
    P00(0x24, BHS,   BYTE, MC6809, REL)
    P00(0x24, BCC,   BYTE, MC6809, REL)
    P00(0x25, BLO,   BYTE, MC6809, REL)
    P00(0x25, BCS,   BYTE, MC6809, REL)
    P00(0x26, BNE,   BYTE, MC6809, REL)
    P00(0x27, BEQ,   BYTE, MC6809, REL)
    P00(0x28, BVC,   BYTE, MC6809, REL)
    P00(0x29, BVS,   BYTE, MC6809, REL)
    P00(0x2A, BPL,   BYTE, MC6809, REL)
    P00(0x2B, BMI,   BYTE, MC6809, REL)
    P00(0x2C, BGE,   BYTE, MC6809, REL)
    P00(0x2D, BLT,   BYTE, MC6809, REL)
    P00(0x2E, BGT,   BYTE, MC6809, REL)
    P00(0x2F, BLE,   BYTE, MC6809, REL)
    P00(0x30, LEAX,  NONE, MC6809, INDX)
    P00(0x31, LEAY,  NONE, MC6809, INDX)
    P00(0x32, LEAS,  NONE, MC6809, INDX)
    P00(0x33, LEAU,  NONE, MC6809, INDX)
    P00(0x34, PSHS,  BYTE, MC6809, STKOP)
    P00(0x35, PULS,  BYTE, MC6809, STKOP)
    P00(0x36, PSHU,  BYTE, MC6809, STKOP)
    P00(0x37, PULU,  BYTE, MC6809, STKOP)
    P00(0x39, RTS,   NONE, MC6809, INHR)
    P00(0x3A, ABX,   NONE, MC6809, INHR)
    P00(0x3B, RTI,   NONE, MC6809, INHR)
    P00(0x3C, CWAI,  BYTE, MC6809, IMM)
    P00(0x3D, MUL,   BYTE, MC6809, INHR)
    P00(0x3F, SWI,   NONE, MC6809, INHR)
    P00(0x40, NEGA,  BYTE, MC6809, INHR)
    P00(0x43, COMA,  BYTE, MC6809, INHR)
    P00(0x44, LSRA,  BYTE, MC6809, INHR)
    P00(0x46, RORA,  BYTE, MC6809, INHR)
    P00(0x47, ASRA,  BYTE, MC6809, INHR)
    P00(0x48, ASLA,  BYTE, MC6809, INHR)
    P00(0x48, LSLA,  BYTE, MC6809, INHR)
    P00(0x49, ROLA,  BYTE, MC6809, INHR)
    P00(0x4A, DECA,  BYTE, MC6809, INHR)
    P00(0x4C, INCA,  BYTE, MC6809, INHR)
    P00(0x4D, TSTA,  BYTE, MC6809, INHR)
    P00(0x4F, CLRA,  BYTE, MC6809, INHR)
    P00(0x50, NEGB,  BYTE, MC6809, INHR)
    P00(0x53, COMB,  BYTE, MC6809, INHR)
    P00(0x54, LSRB,  BYTE, MC6809, INHR)
    P00(0x56, RORB,  BYTE, MC6809, INHR)
    P00(0x57, ASRB,  BYTE, MC6809, INHR)
    P00(0x58, ASLB,  BYTE, MC6809, INHR)
    P00(0x58, LSLB,  BYTE, MC6809, INHR)
    P00(0x59, ROLB,  BYTE, MC6809, INHR)
    P00(0x5A, DECB,  BYTE, MC6809, INHR)
    P00(0x5C, INCB,  BYTE, MC6809, INHR)
    P00(0x5D, TSTB,  BYTE, MC6809, INHR)
    P00(0x5F, CLRB,  BYTE, MC6809, INHR)
    P00(0x60, NEG,   BYTE, MC6809, INDX)
    P00(0x63, COM,   BYTE, MC6809, INDX)
    P00(0x64, LSR,   BYTE, MC6809, INDX)
    P00(0x66, ROR,   BYTE, MC6809, INDX)
    P00(0x67, ASR,   BYTE, MC6809, INDX)
    P00(0x68, ASL,   BYTE, MC6809, INDX)
    P00(0x68, LSL,   BYTE, MC6809, INDX)
    P00(0x69, ROL,   BYTE, MC6809, INDX)
    P00(0x6A, DEC,   BYTE, MC6809, INDX)
    P00(0x6C, INC,   BYTE, MC6809, INDX)
    P00(0x6D, TST,   BYTE, MC6809, INDX)
    P00(0x6E, JMP,   NONE, MC6809, INDX)
    P00(0x6F, CLR,   BYTE, MC6809, INDX)
    P00(0x70, NEG,   BYTE, MC6809, EXTD)
    P00(0x73, COM,   BYTE, MC6809, EXTD)
    P00(0x74, LSR,   BYTE, MC6809, EXTD)
    P00(0x76, ROR,   BYTE, MC6809, EXTD)
    P00(0x77, ASR,   BYTE, MC6809, EXTD)
    P00(0x78, ASL,   BYTE, MC6809, EXTD)
    P00(0x78, LSL,   BYTE, MC6809, EXTD)
    P00(0x79, ROL,   BYTE, MC6809, EXTD)
    P00(0x7A, DEC,   BYTE, MC6809, EXTD)
    P00(0x7C, INC,   BYTE, MC6809, EXTD)
    P00(0x7D, TST,   BYTE, MC6809, EXTD)
    P00(0x7E, JMP,   NONE, MC6809, EXTD)
    P00(0x7F, CLR,   BYTE, MC6809, EXTD)
    P00(0x80, SUBA,  BYTE, MC6809, IMM)
    P00(0x81, CMPA,  BYTE, MC6809, IMM)
    P00(0x82, SBCA,  BYTE, MC6809, IMM)
    P00(0x83, SUBD,  WORD, MC6809, IMM)
    P00(0x84, ANDA,  BYTE, MC6809, IMM)
    P00(0x85, BITA,  BYTE, MC6809, IMM)
    P00(0x86, LDA,   BYTE, MC6809, IMM)
    P00(0x88, EORA,  BYTE, MC6809, IMM)
    P00(0x89, ADCA,  BYTE, MC6809, IMM)
    P00(0x8A, ORA,   BYTE, MC6809, IMM)
    P00(0x8B, ADDA,  BYTE, MC6809, IMM)
    P00(0x8C, CMPX,  WORD, MC6809, IMM)
    P00(0x8D, BSR,   BYTE, MC6809, REL)
    P00(0x8E, LDX,   WORD, MC6809, IMM)
    P00(0x90, SUBA,  BYTE, MC6809, DIRP)
    P00(0x91, CMPA,  BYTE, MC6809, DIRP)
    P00(0x92, SBCA,  BYTE, MC6809, DIRP)
    P00(0x93, SUBD,  WORD, MC6809, DIRP)
    P00(0x94, ANDA,  BYTE, MC6809, DIRP)
    P00(0x95, BITA,  BYTE, MC6809, DIRP)
    P00(0x96, LDA,   BYTE, MC6809, DIRP)
    P00(0x97, STA,   BYTE, MC6809, DIRP)
    P00(0x98, EORA,  BYTE, MC6809, DIRP)
    P00(0x99, ADCA,  BYTE, MC6809, DIRP)
    P00(0x9A, ORA,   BYTE, MC6809, DIRP)
    P00(0x9B, ADDA,  BYTE, MC6809, DIRP)
    P00(0x9C, CMPX,  WORD, MC6809, DIRP)
    P00(0x9D, JSR,   NONE, MC6809, DIRP)
    P00(0x9E, LDX,   WORD, MC6809, DIRP)
    P00(0x9F, STX,   WORD, MC6809, DIRP)
    P00(0xA0, SUBA,  BYTE, MC6809, INDX)
    P00(0xA1, CMPA,  BYTE, MC6809, INDX)
    P00(0xA2, SBCA,  BYTE, MC6809, INDX)
    P00(0xA3, SUBD,  WORD, MC6809, INDX)
    P00(0xA4, ANDA,  BYTE, MC6809, INDX)
    P00(0xA5, BITA,  BYTE, MC6809, INDX)
    P00(0xA6, LDA,   BYTE, MC6809, INDX)
    P00(0xA7, STA,   BYTE, MC6809, INDX)
    P00(0xA8, EORA,  BYTE, MC6809, INDX)
    P00(0xA9, ADCA,  BYTE, MC6809, INDX)
    P00(0xAA, ORA,   BYTE, MC6809, INDX)
    P00(0xAB, ADDA,  BYTE, MC6809, INDX)
    P00(0xAC, CMPX,  WORD, MC6809, INDX)
    P00(0xAD, JSR,   NONE, MC6809, INDX)
    P00(0xAE, LDX,   WORD, MC6809, INDX)
    P00(0xAF, STX,   WORD, MC6809, INDX)
    P00(0xB0, SUBA,  BYTE, MC6809, EXTD)
    P00(0xB1, CMPA,  BYTE, MC6809, EXTD)
    P00(0xB2, SBCA,  BYTE, MC6809, EXTD)
    P00(0xB3, SUBD,  BYTE, MC6809, EXTD)
    P00(0xB4, ANDA,  BYTE, MC6809, EXTD)
    P00(0xB5, BITA,  BYTE, MC6809, EXTD)
    P00(0xB6, LDA,   BYTE, MC6809, EXTD)
    P00(0xB7, STA,   BYTE, MC6809, EXTD)
    P00(0xB8, EORA,  BYTE, MC6809, EXTD)
    P00(0xB9, ADCA,  BYTE, MC6809, EXTD)
    P00(0xBA, ORA,   BYTE, MC6809, EXTD)
    P00(0xBB, ADDA,  BYTE, MC6809, EXTD)
    P00(0xBC, CMPX,  WORD, MC6809, EXTD)
    P00(0xBD, JSR,   NONE, MC6809, EXTD)
    P00(0xBE, LDX,   WORD, MC6809, EXTD)
    P00(0xBF, STX,   WORD, MC6809, EXTD)
    P00(0xC0, SUBB,  BYTE, MC6809, IMM)
    P00(0xC1, CMPB,  BYTE, MC6809, IMM)
    P00(0xC2, SBCB,  BYTE, MC6809, IMM)
    P00(0xC3, ADDD,  WORD, MC6809, IMM)
    P00(0xC4, ANDB,  BYTE, MC6809, IMM)
    P00(0xC5, BITB,  BYTE, MC6809, IMM)
    P00(0xC6, LDB,   BYTE, MC6809, IMM)
    P00(0xC8, EORB,  BYTE, MC6809, IMM)
    P00(0xC9, ADCB,  BYTE, MC6809, IMM)
    P00(0xCA, ORB,   BYTE, MC6809, IMM)
    P00(0xCB, ADDB,  BYTE, MC6809, IMM)
    P00(0xCC, LDD,   WORD, MC6809, IMM)
    P00(0xCE, LDU,   WORD, MC6809, IMM)
    P00(0xD0, SUBB,  BYTE, MC6809, DIRP)
    P00(0xD1, CMPB,  BYTE, MC6809, DIRP)
    P00(0xD2, SBCB,  BYTE, MC6809, DIRP)
    P00(0xD3, ADDD,  WORD, MC6809, DIRP)
    P00(0xD4, ANDB,  BYTE, MC6809, DIRP)
    P00(0xD5, BITB,  BYTE, MC6809, DIRP)
    P00(0xD6, LDB,   BYTE, MC6809, DIRP)
    P00(0xD7, STB,   BYTE, MC6809, DIRP)
    P00(0xD8, EORB,  BYTE, MC6809, DIRP)
    P00(0xD9, ADCB,  BYTE, MC6809, DIRP)
    P00(0xDA, ORB,   BYTE, MC6809, DIRP)
    P00(0xDB, ADDB,  BYTE, MC6809, DIRP)
    P00(0xDC, LDD,   WORD, MC6809, DIRP)
    P00(0xDD, STD,   WORD, MC6809, DIRP)
    P00(0xDE, LDU,   WORD, MC6809, DIRP)
    P00(0xDF, STU,   WORD, MC6809, DIRP)
    P00(0xE0, SUBB,  BYTE, MC6809, INDX)
    P00(0xE1, CMPB,  BYTE, MC6809, INDX)
    P00(0xE2, SBCB,  BYTE, MC6809, INDX)
    P00(0xE3, ADDD,  WORD, MC6809, INDX)
    P00(0xE4, ANDB,  BYTE, MC6809, INDX)
    P00(0xE5, BITB,  BYTE, MC6809, INDX)
    P00(0xE6, LDB,   BYTE, MC6809, INDX)
    P00(0xE7, STB,   BYTE, MC6809, INDX)
    P00(0xE8, EORB,  BYTE, MC6809, INDX)
    P00(0xE9, ADCB,  BYTE, MC6809, INDX)
    P00(0xEA, ORB,   BYTE, MC6809, INDX)
    P00(0xEB, ADDB,  BYTE, MC6809, INDX)
    P00(0xEC, LDD,   WORD, MC6809, INDX)
    P00(0xED, STD,   WORD, MC6809, INDX)
    P00(0xEE, LDU,   WORD, MC6809, INDX)
    P00(0xEF, STU,   WORD, MC6809, INDX)
    P00(0xF0, SUBB,  BYTE, MC6809, EXTD)
    P00(0xF1, CMPB,  BYTE, MC6809, EXTD)
    P00(0xF2, SBCB,  BYTE, MC6809, EXTD)
    P00(0xF3, ADDD,  WORD, MC6809, EXTD)
    P00(0xF4, ANDB,  BYTE, MC6809, EXTD)
    P00(0xF5, BITB,  BYTE, MC6809, EXTD)
    P00(0xF6, LDB,   BYTE, MC6809, EXTD)
    P00(0xF7, STB,   BYTE, MC6809, EXTD)
    P00(0xF8, EORB,  BYTE, MC6809, EXTD)
    P00(0xF9, ADCB,  BYTE, MC6809, EXTD)
    P00(0xFA, ORB,   BYTE, MC6809, EXTD)
    P00(0xFB, ADDB,  BYTE, MC6809, EXTD)
    P00(0xFC, LDD,   WORD, MC6809, EXTD)
    P00(0xFD, STD,   WORD, MC6809, EXTD)
    P00(0xFE, LDU,   WORD, MC6809, EXTD)
    P00(0xFF, STU,   WORD, MC6809, EXTD)

    // Pseudo instruction to set DP on assembler
    P00(PSEUDO_SETDP,  SETDP,  BYTE, MC6809, PSEUDO)
    P00(PSEUDO_ASSUME, ASSUME, BYTE, MC6809, PSEUDO)
};

static constexpr Entry MC6809_P10[] PROGMEM = {
    P10(0x21, LBRN,  WORD, MC6809, REL)
    P10(0x22, LBHI,  WORD, MC6809, REL)
    P10(0x23, LBLS,  WORD, MC6809, REL)
    P10(0x24, LBHS,  WORD, MC6809, REL)
    P10(0x24, LBCC,  WORD, MC6809, REL)
    P10(0x25, LBLO,  WORD, MC6809, REL)
    P10(0x25, LBCS,  WORD, MC6809, REL)
    P10(0x26, LBNE,  WORD, MC6809, REL)
    P10(0x27, LBEQ,  WORD, MC6809, REL)
    P10(0x28, LBVC,  WORD, MC6809, REL)
    P10(0x29, LBVS,  WORD, MC6809, REL)
    P10(0x2A, LBPL,  WORD, MC6809, REL)
    P10(0x2B, LBMI,  WORD, MC6809, REL)
    P10(0x2C, LBGE,  WORD, MC6809, REL)
    P10(0x2D, LBLT,  WORD, MC6809, REL)
    P10(0x2E, LBGT,  WORD, MC6809, REL)
    P10(0x2F, LBLE,  WORD, MC6809, REL)
    P10(0x3F, SWI2,  NONE, MC6809, INHR)
    P10(0x83, CMPD,  WORD, MC6809, IMM)
    P10(0x8C, CMPY,  WORD, MC6809, IMM)
    P10(0x8E, LDY,   WORD, MC6809, IMM)
    P10(0x93, CMPD,  WORD, MC6809, DIRP)
    P10(0x9C, CMPY,  WORD, MC6809, DIRP)
    P10(0x9E, LDY,   WORD, MC6809, DIRP)
    P10(0x9F, STY,   WORD, MC6809, DIRP)
    P10(0xA3, CMPD,  WORD, MC6809, INDX)
    P10(0xAC, CMPY,  WORD, MC6809, INDX)
    P10(0xAE, LDY,   WORD, MC6809, INDX)
    P10(0xAF, STY,   WORD, MC6809, INDX)
    P10(0xB3, CMPD,  WORD, MC6809, EXTD)
    P10(0xBC, CMPY,  WORD, MC6809, EXTD)
    P10(0xBE, LDY,   WORD, MC6809, EXTD)
    P10(0xBF, STY,   WORD, MC6809, EXTD)
    P10(0xCE, LDS,   WORD, MC6809, IMM)
    P10(0xDE, LDS,   WORD, MC6809, DIRP)
    P10(0xDF, STS,   WORD, MC6809, DIRP)
    P10(0xEE, LDS,   WORD, MC6809, INDX)
    P10(0xEF, STS,   WORD, MC6809, INDX)
    P10(0xFE, LDS,   WORD, MC6809, EXTD)
    P10(0xFF, STS,   WORD, MC6809, EXTD)
};

static constexpr Entry MC6809_P11[] PROGMEM = {
    P11(0x3F, SWI3,  NONE, MC6809, INHR)
    P11(0x83, CMPU,  WORD, MC6809, IMM)
    P11(0x8C, CMPS,  WORD, MC6809, IMM)
    P11(0x93, CMPU,  WORD, MC6809, DIRP)
    P11(0x9C, CMPS,  WORD, MC6809, DIRP)
    P11(0xA3, CMPU,  WORD, MC6809, INDX)
    P11(0xAC, CMPS,  WORD, MC6809, INDX)
    P11(0xB3, CMPU,  WORD, MC6809, EXTD)
    P11(0xBC, CMPS,  WORD, MC6809, EXTD)
};

static constexpr Entry HD6309_P00[] PROGMEM = {
    P00(0x01, OIM,   BYTE, HD6309, IMMDIR)
    P00(0x02, AIM,   BYTE, HD6309, IMMDIR)
    P00(0x05, EIM,   BYTE, HD6309, IMMDIR)
    P00(0x0B, TIM,   BYTE, HD6309, IMMDIR)
    P00(0x14, SEXW,  WORD, HD6309, INHR)
    P00(0x61, OIM,   BYTE, HD6309, IMMIDX)
    P00(0x62, AIM,   BYTE, HD6309, IMMIDX)
    P00(0x65, EIM,   BYTE, HD6309, IMMIDX)
    P00(0x6B, TIM,   BYTE, HD6309, IMMIDX)
    P00(0x71, OIM,   BYTE, HD6309, IMMEXT)
    P00(0x72, AIM,   BYTE, HD6309, IMMEXT)
    P00(0x75, EIM,   BYTE, HD6309, IMMEXT)
    P00(0x7B, TIM,   BYTE, HD6309, IMMEXT)
    P00(0xCD, LDQ,   LONG, HD6309, IMM)
};

static constexpr Entry HD6309_P10[] PROGMEM = {
    P10(0x30, ADDR,  NONE, HD6309, REGS)
    P10(0x31, ADCR,  NONE, HD6309, REGS)
    P10(0x32, SUBR,  NONE, HD6309, REGS)
    P10(0x33, SBCR,  NONE, HD6309, REGS)
    P10(0x34, ANDR,  NONE, HD6309, REGS)
    P10(0x35, ORR,   NONE, HD6309, REGS)
    P10(0x36, EORR,  NONE, HD6309, REGS)
    P10(0x37, CMPR,  NONE, HD6309, REGS)
    P10(0x38, PSHSW, NONE, HD6309, INHR)
    P10(0x39, PULSW, NONE, HD6309, INHR)
    P10(0x3A, PSHUW, NONE, HD6309, INHR)
    P10(0x3B, PULUW, NONE, HD6309, INHR)
    P10(0x40, NEGD,  WORD, HD6309, INHR)
    P10(0x43, COMD,  WORD, HD6309, INHR)
    P10(0x44, LSRD,  WORD, HD6309, INHR)
    P10(0x46, RORD,  WORD, HD6309, INHR)
    P10(0x47, ASRD,  WORD, HD6309, INHR)
    P10(0x48, ASLD,  WORD, HD6309, INHR)
    P10(0x48, LSLD,  WORD, HD6309, INHR)
    P10(0x49, ROLD,  WORD, HD6309, INHR)
    P10(0x4A, DECD,  WORD, HD6309, INHR)
    P10(0x4C, INCD,  WORD, HD6309, INHR)
    P10(0x4D, TSTD,  WORD, HD6309, INHR)
    P10(0x4F, CLRD,  WORD, HD6309, INHR)
    P10(0x53, COMW,  WORD, HD6309, INHR)
    P10(0x54, LSRW,  WORD, HD6309, INHR)
    P10(0x56, RORW,  WORD, HD6309, INHR)
    P10(0x59, ROLW,  WORD, HD6309, INHR)
    P10(0x5A, DECW,  WORD, HD6309, INHR)
    P10(0x5C, INCW,  WORD, HD6309, INHR)
    P10(0x5D, TSTW,  WORD, HD6309, INHR)
    P10(0x5F, CLRW,  WORD, HD6309, INHR)
    P10(0x80, SUBW,  WORD, HD6309, IMM)
    P10(0x81, CMPW,  WORD, HD6309, IMM)
    P10(0x82, SBCD,  WORD, HD6309, IMM)
    P10(0x84, ANDD,  WORD, HD6309, IMM)
    P10(0x85, BITD,  WORD, HD6309, IMM)
    P10(0x86, LDW,   WORD, HD6309, IMM)
    P10(0x88, EORD,  WORD, HD6309, IMM)
    P10(0x89, ADCD,  WORD, HD6309, IMM)
    P10(0x8A, ORD,   WORD, HD6309, IMM)
    P10(0x8B, ADDW,  WORD, HD6309, IMM)
    P10(0x90, SUBW,  WORD, HD6309, DIRP)
    P10(0x91, CMPW,  WORD, HD6309, DIRP)
    P10(0x92, SBCD,  WORD, HD6309, DIRP)
    P10(0x94, ANDD,  WORD, HD6309, DIRP)
    P10(0x95, BITD,  WORD, HD6309, DIRP)
    P10(0x96, LDW,   WORD, HD6309, DIRP)
    P10(0x97, STW,   WORD, HD6309, DIRP)
    P10(0x98, EORD,  WORD, HD6309, DIRP)
    P10(0x99, ADCD,  WORD, HD6309, DIRP)
    P10(0x9A, ORD,   WORD, HD6309, DIRP)
    P10(0x9B, ADDW,  WORD, HD6309, DIRP)
    P10(0xA0, SUBW,  WORD, HD6309, INDX)
    P10(0xA1, CMPW,  WORD, HD6309, INDX)
    P10(0xA2, SBCD,  WORD, HD6309, INDX)
    P10(0xA4, ANDD,  WORD, HD6309, INDX)
    P10(0xA5, BITD,  WORD, HD6309, INDX)
    P10(0xA6, LDW,   WORD, HD6309, INDX)
    P10(0xA7, STW,   WORD, HD6309, INDX)
    P10(0xA8, EORD,  WORD, HD6309, INDX)
    P10(0xA9, ADCD,  WORD, HD6309, INDX)
    P10(0xAA, ORD,   WORD, HD6309, INDX)
    P10(0xAB, ADDW,  WORD, HD6309, INDX)
    P10(0xB0, SUBW,  WORD, HD6309, EXTD)
    P10(0xB1, CMPW,  WORD, HD6309, EXTD)
    P10(0xB2, SBCD,  WORD, HD6309, EXTD)
    P10(0xB4, ANDD,  WORD, HD6309, EXTD)
    P10(0xB5, BITD,  WORD, HD6309, EXTD)
    P10(0xB6, LDW,   WORD, HD6309, EXTD)
    P10(0xB7, STW,   WORD, HD6309, EXTD)
    P10(0xB8, EORD,  WORD, HD6309, EXTD)
    P10(0xB9, ADCD,  WORD, HD6309, EXTD)
    P10(0xBA, ORD,   WORD, HD6309, EXTD)
    P10(0xBB, ADDW,  WORD, HD6309, EXTD)
    P10(0xDC, LDQ,   LONG, HD6309, DIRP)
    P10(0xDD, STQ,   LONG, HD6309, DIRP)
    P10(0xEC, LDQ,   LONG, HD6309, INDX)
    P10(0xED, STQ,   LONG, HD6309, INDX)
    P10(0xFC, LDQ,   LONG, HD6309, EXTD)
    P10(0xFD, STQ,   LONG, HD6309, EXTD)
};

static constexpr Entry HD6309_P11[] PROGMEM = {
    P11(0x30, BAND,  BYTE, HD6309, BITOP)
    P11(0x31, BIAND, BYTE, HD6309, BITOP)
    P11(0x32, BOR,   BYTE, HD6309, BITOP)
    P11(0x33, BIOR,  BYTE, HD6309, BITOP)
    P11(0x34, BEOR,  BYTE, HD6309, BITOP)
    P11(0x35, BIEOR, BYTE, HD6309, BITOP)
    P11(0x36, LDBT,  BYTE, HD6309, BITOP)
    P11(0x37, STBT,  BYTE, HD6309, BITOP)
    P11(0x38, TFM,   NONE, HD6309, TFRM) /* R+,R+ */
    P11(0x39, TFM,   NONE, HD6309, TFRM) /* R-,R- */
    P11(0x3A, TFM,   NONE, HD6309, TFRM) /* R+,R */
    P11(0x3B, TFM,   NONE, HD6309, TFRM) /* R,R+ */
    P11(0x3C, BITMD, BYTE, HD6309, IMM)
    P11(0x3D, LDMD,  BYTE, HD6309, IMM)
    P11(0x43, COME,  BYTE, HD6309, INHR)
    P11(0x4A, DECE,  BYTE, HD6309, INHR)
    P11(0x4C, INCE,  BYTE, HD6309, INHR)
    P11(0x4D, TSTE,  BYTE, HD6309, INHR)
    P11(0x4F, CLRE,  BYTE, HD6309, INHR)
    P11(0x53, COMF,  BYTE, HD6309, INHR)
    P11(0x5A, DECF,  BYTE, HD6309, INHR)
    P11(0x5C, INCF,  BYTE, HD6309, INHR)
    P11(0x5D, TSTF,  BYTE, HD6309, INHR)
    P11(0x5F, CLRF,  BYTE, HD6309, INHR)
    P11(0x80, SUBE,  BYTE, HD6309, IMM)
    P11(0x81, CMPE,  BYTE, HD6309, IMM)
    P11(0x86, LDE,   BYTE, HD6309, IMM)
    P11(0x8B, ADDE,  BYTE, HD6309, IMM)
    P11(0x8D, DIVD,  WORD, HD6309, IMM)
    P11(0x8E, DIVQ,  WORD, HD6309, IMM)
    P11(0x8F, MULD,  WORD, HD6309, IMM)
    P11(0x90, SUBE,  BYTE, HD6309, DIRP)
    P11(0x91, CMPE,  BYTE, HD6309, DIRP)
    P11(0x96, LDE,   BYTE, HD6309, DIRP)
    P11(0x97, STE,   BYTE, HD6309, DIRP)
    P11(0x9B, ADDE,  BYTE, HD6309, DIRP)
    P11(0x9D, DIVD,  BYTE, HD6309, DIRP)
    P11(0x9E, DIVQ,  WORD, HD6309, DIRP)
    P11(0x9F, MULD,  WORD, HD6309, DIRP)
    P11(0xA0, SUBE,  BYTE, HD6309, INDX)
    P11(0xA1, CMPE,  BYTE, HD6309, INDX)
    P11(0xA6, LDE,   BYTE, HD6309, INDX)
    P11(0xA7, STE,   BYTE, HD6309, INDX)
    P11(0xAB, ADDE,  BYTE, HD6309, INDX)
    P11(0xAD, DIVD,  BYTE, HD6309, INDX)
    P11(0xAE, DIVQ,  WORD, HD6309, INDX)
    P11(0xAF, MULD,  WORD, HD6309, INDX)
    P11(0xB0, SUBE,  BYTE, HD6309, EXTD)
    P11(0xB1, CMPE,  BYTE, HD6309, EXTD)
    P11(0xB6, LDE,   BYTE, HD6309, EXTD)
    P11(0xB7, STE,   BYTE, HD6309, EXTD)
    P11(0xBB, ADDE,  BYTE, HD6309, EXTD)
    P11(0xBD, DIVD,  BYTE, HD6309, EXTD)
    P11(0xBE, DIVQ,  WORD, HD6309, EXTD)
    P11(0xBF, MULD,  WORD, HD6309, EXTD)
    P11(0xC0, SUBF,  BYTE, HD6309, IMM)
    P11(0xC1, CMPF,  BYTE, HD6309, IMM)
    P11(0xC6, LDF,   BYTE, HD6309, IMM)
    P11(0xCB, ADDF,  BYTE, HD6309, IMM)
    P11(0xD0, SUBF,  BYTE, HD6309, DIRP)
    P11(0xD1, CMPF,  BYTE, HD6309, DIRP)
    P11(0xD6, LDF,   BYTE, HD6309, DIRP)
    P11(0xD7, STF,   BYTE, HD6309, DIRP)
    P11(0xDB, ADDF,  BYTE, HD6309, DIRP)
    P11(0xE0, SUBF,  BYTE, HD6309, INDX)
    P11(0xE1, CMPF,  BYTE, HD6309, INDX)
    P11(0xE6, LDF,   BYTE, HD6309, INDX)
    P11(0xE7, STF,   BYTE, HD6309, INDX)
    P11(0xEB, ADDF,  BYTE, HD6309, INDX)
    P11(0xF0, SUBF,  BYTE, HD6309, EXTD)
    P11(0xF1, CMPF,  BYTE, HD6309, EXTD)
    P11(0xF6, LDF,   BYTE, HD6309, EXTD)
    P11(0xF7, STF,   BYTE, HD6309, EXTD)
    P11(0xFB, ADDF,  BYTE, HD6309, EXTD)
};

static constexpr Config::opcode_t PREFIX_P00 = 0x00;
static constexpr Config::opcode_t PREFIX_P10 = 0x10;
static constexpr Config::opcode_t PREFIX_P11 = 0x11;

bool TableMc6809::isPrefixCode(Config::opcode_t opCode) {
    return opCode == PREFIX_P10 || opCode == PREFIX_P11;
}

const Entry *TableMc6809::searchEntry(
    const char *name,
    const Entry *table, const Entry *end) {
    for (const Entry *entry = table; entry < end; entry++) {
        if (pgm_strcasecmp(name, entry->name) == 0)
            return entry;
    }
    return nullptr;
}

const Entry *TableMc6809::searchEntry(
    const Config::opcode_t opCode,
    const Entry *table, const Entry *end) {
    for (const Entry *entry = table; entry < end; entry++) {
        if (opCode == pgm_read_byte(&entry->opc))
            return entry;
    }
    return nullptr;
}

Error TableMc6809::searchName(
    InsnMc6809 &insn, const EntryPage *pages, const EntryPage *end) {
    const char *name = insn.name();
    for (const EntryPage *page = pages; page < end; page++) {
        const Entry *entry;
        if ((entry = searchEntry(name, page->table, page->end)) != nullptr) {
            insn.setInsnCode(page->prefix, pgm_read_byte(&entry->opc));
            insn.setFlags(pgm_read_byte(&entry->flags));
            return OK;
        }
    }
    return UNKNOWN_INSTRUCTION;
}

Error TableMc6809::searchNameAndAddrMode(
    InsnMc6809 &insn, const EntryPage *pages, const EntryPage *end) {
    const char *name = insn.name();
    const AddrMode addrMode = insn.addrMode();
    for (const EntryPage *page = pages; page < end; page++) {
        for (const Entry *entry = page->table; entry < page->end
                 && (entry = searchEntry(name, entry, page->end)) != nullptr; entry++) {
            const host::uint_t flags = pgm_read_byte(&entry->flags);
            if (addrMode == Entry::_addrMode(flags)) {
                insn.setInsnCode(page->prefix, pgm_read_byte(&entry->opc));
                insn.setFlags(flags);
                return OK;
            }
        }
    }
    return UNKNOWN_INSTRUCTION;
}

Error TableMc6809::searchInsnCode(
    InsnMc6809 &insn, const EntryPage *pages, const EntryPage *end) {
    for (const EntryPage *page = pages; page < end; page++) {
        if (insn.prefixCode() != page->prefix) continue;
        const Entry *entry = searchEntry(insn.opCode(), page->table, page->end);
        if (entry) {
            insn.setFlags(pgm_read_byte(&entry->flags));
            char name[Config::name_max + 1];
            pgm_strncpy(name, entry->name, sizeof(name));
            insn.setName(name);
            return OK;
        }
    }
    return UNKNOWN_INSTRUCTION;
}

static constexpr EntryPage MC6809_PAGES[] = {
    { PREFIX_P00, &MC6809_P00[0], ARRAY_END(MC6809_P00) },
    { PREFIX_P10, &MC6809_P10[0], ARRAY_END(MC6809_P10) },
    { PREFIX_P11, &MC6809_P11[0], ARRAY_END(MC6809_P11) },
};

static constexpr EntryPage HD6309_PAGES[] = {
    { PREFIX_P00, &HD6309_P00[0], ARRAY_END(HD6309_P00) },
    { PREFIX_P10, &HD6309_P10[0], ARRAY_END(HD6309_P10) },
    { PREFIX_P11, &HD6309_P11[0], ARRAY_END(HD6309_P11) },
};

Error TableMc6809::searchName(InsnMc6809 &insn) const {
    if (searchName(insn, ARRAY_RANGE(MC6809_PAGES)) == OK)
        return OK;
    if (is6309() && searchName(insn, ARRAY_RANGE(HD6309_PAGES)) == OK)
        return OK;
    return UNKNOWN_INSTRUCTION;
}

Error TableMc6809::searchNameAndAddrMode(InsnMc6809 &insn) const {
    if (searchNameAndAddrMode(insn, ARRAY_RANGE(MC6809_PAGES)) == OK)
        return OK;
    if (is6309() && searchNameAndAddrMode(insn, ARRAY_RANGE(HD6309_PAGES)) == OK)
        return OK;
    return UNKNOWN_INSTRUCTION;
}

Error TableMc6809::searchInsnCode(InsnMc6809 &insn) const {
    if (searchInsnCode(insn, ARRAY_RANGE(MC6809_PAGES)) == OK)
        return OK;
    if (is6309() && searchInsnCode(insn, ARRAY_RANGE(HD6309_PAGES)) == OK)
        return OK;
    return UNKNOWN_INSTRUCTION;
}

const char *TableMc6809::listCpu() {
    return "6809, 6309";
}

bool TableMc6809::setCpu(const char *cpu) {
    if (strcmp(cpu, "6809") == 0) {
        _cpuType = MC6809;
        return true;
    }
    if (strcmp(cpu, "6309") == 0) {
        _cpuType = HD6309;
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

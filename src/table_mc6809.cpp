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

#define X(_opc, _name, _mode, _extra)                       \
    { _opc, Entry::Flags::create(_mode, _extra), TEXT_##_name },
#define E(_opc, _name, _mode) X(_opc, _name, _mode, NONE)

static constexpr Entry MC6809_P00[] PROGMEM = {
    E(0x00, NEG,   DIR)
    E(0x03, COM,   DIR)
    E(0x04, LSR,   DIR)
    E(0x06, ROR,   DIR)
    E(0x07, ASR,   DIR)
    E(0x08, ASL,   DIR)
    E(0x08, LSL,   DIR)
    E(0x09, ROL,   DIR)
    E(0x0A, DEC,   DIR)
    E(0x0C, INC,   DIR)
    E(0x0D, TST,   DIR)
    E(0x0E, JMP,   DIR)
    E(0x0F, CLR,   DIR)
    E(0x12, NOP,   NONE)
    E(0x13, SYNC,  NONE)
    E(0x16, LBRA,  LREL)
    E(0x17, LBSR,  LREL)
    E(0x19, DAA,   NONE)
    E(0x1A, ORCC,  IM8)
    E(0x1C, ANDCC, IM8)
    E(0x1D, SEX,   NONE)
    E(0x1E, EXG,   REG_REG)
    E(0x1F, TFR,   REG_REG)
    E(0x20, BRA,   REL)
    E(0x21, BRN,   REL)
    E(0x22, BHI,   REL)
    E(0x23, BLS,   REL)
    E(0x24, BHS,   REL)
    E(0x24, BCC,   REL)
    E(0x25, BLO,   REL)
    E(0x25, BCS,   REL)
    E(0x26, BNE,   REL)
    E(0x27, BEQ,   REL)
    E(0x28, BVC,   REL)
    E(0x29, BVS,   REL)
    E(0x2A, BPL,   REL)
    E(0x2B, BMI,   REL)
    E(0x2C, BGE,   REL)
    E(0x2D, BLT,   REL)
    E(0x2E, BGT,   REL)
    E(0x2F, BLE,   REL)
    E(0x30, LEAX,  IDX)
    E(0x31, LEAY,  IDX)
    E(0x32, LEAS,  IDX)
    E(0x33, LEAU,  IDX)
    E(0x34, PSHS,  REGLIST)
    E(0x35, PULS,  REGLIST)
    E(0x36, PSHU,  REGLIST)
    E(0x37, PULU,  REGLIST)
    E(0x39, RTS,   NONE)
    E(0x3A, ABX,   NONE)
    E(0x3B, RTI,   NONE)
    E(0x3C, CWAI,  IM8)
    E(0x3D, MUL,   NONE)
    E(0x3F, SWI,   NONE)
    E(0x40, NEGA,  NONE)
    E(0x43, COMA,  NONE)
    E(0x44, LSRA,  NONE)
    E(0x46, RORA,  NONE)
    E(0x47, ASRA,  NONE)
    E(0x48, ASLA,  NONE)
    E(0x48, LSLA,  NONE)
    E(0x49, ROLA,  NONE)
    E(0x4A, DECA,  NONE)
    E(0x4C, INCA,  NONE)
    E(0x4D, TSTA,  NONE)
    E(0x4F, CLRA,  NONE)
    E(0x50, NEGB,  NONE)
    E(0x53, COMB,  NONE)
    E(0x54, LSRB,  NONE)
    E(0x56, RORB,  NONE)
    E(0x57, ASRB,  NONE)
    E(0x58, ASLB,  NONE)
    E(0x58, LSLB,  NONE)
    E(0x59, ROLB,  NONE)
    E(0x5A, DECB,  NONE)
    E(0x5C, INCB,  NONE)
    E(0x5D, TSTB,  NONE)
    E(0x5F, CLRB,  NONE)
    E(0x60, NEG,   IDX)
    E(0x63, COM,   IDX)
    E(0x64, LSR,   IDX)
    E(0x66, ROR,   IDX)
    E(0x67, ASR,   IDX)
    E(0x68, ASL,   IDX)
    E(0x68, LSL,   IDX)
    E(0x69, ROL,   IDX)
    E(0x6A, DEC,   IDX)
    E(0x6C, INC,   IDX)
    E(0x6D, TST,   IDX)
    E(0x6E, JMP,   IDX)
    E(0x6F, CLR,   IDX)
    E(0x70, NEG,   EXT)
    E(0x73, COM,   EXT)
    E(0x74, LSR,   EXT)
    E(0x76, ROR,   EXT)
    E(0x77, ASR,   EXT)
    E(0x78, ASL,   EXT)
    E(0x78, LSL,   EXT)
    E(0x79, ROL,   EXT)
    E(0x7A, DEC,   EXT)
    E(0x7C, INC,   EXT)
    E(0x7D, TST,   EXT)
    E(0x7E, JMP,   EXT)
    E(0x7F, CLR,   EXT)
    E(0x80, SUBA,  IM8)
    E(0x81, CMPA,  IM8)
    E(0x82, SBCA,  IM8)
    E(0x83, SUBD,  IM16)
    E(0x84, ANDA,  IM8)
    E(0x85, BITA,  IM8)
    E(0x86, LDA,   IM8)
    E(0x88, EORA,  IM8)
    E(0x89, ADCA,  IM8)
    E(0x8A, ORA,   IM8)
    E(0x8B, ADDA,  IM8)
    E(0x8C, CMPX,  IM16)
    E(0x8D, BSR,   REL)
    E(0x8E, LDX,   IM16)
    E(0x90, SUBA,  DIR)
    E(0x91, CMPA,  DIR)
    E(0x92, SBCA,  DIR)
    E(0x93, SUBD,  DIR)
    E(0x94, ANDA,  DIR)
    E(0x95, BITA,  DIR)
    E(0x96, LDA,   DIR)
    E(0x97, STA,   DIR)
    E(0x98, EORA,  DIR)
    E(0x99, ADCA,  DIR)
    E(0x9A, ORA,   DIR)
    E(0x9B, ADDA,  DIR)
    E(0x9C, CMPX,  DIR)
    E(0x9D, JSR,   DIR)
    E(0x9E, LDX,   DIR)
    E(0x9F, STX,   DIR)
    E(0xA0, SUBA,  IDX)
    E(0xA1, CMPA,  IDX)
    E(0xA2, SBCA,  IDX)
    E(0xA3, SUBD,  IDX)
    E(0xA4, ANDA,  IDX)
    E(0xA5, BITA,  IDX)
    E(0xA6, LDA,   IDX)
    E(0xA7, STA,   IDX)
    E(0xA8, EORA,  IDX)
    E(0xA9, ADCA,  IDX)
    E(0xAA, ORA,   IDX)
    E(0xAB, ADDA,  IDX)
    E(0xAC, CMPX,  IDX)
    E(0xAD, JSR,   IDX)
    E(0xAE, LDX,   IDX)
    E(0xAF, STX,   IDX)
    E(0xB0, SUBA,  EXT)
    E(0xB1, CMPA,  EXT)
    E(0xB2, SBCA,  EXT)
    E(0xB3, SUBD,  EXT)
    E(0xB4, ANDA,  EXT)
    E(0xB5, BITA,  EXT)
    E(0xB6, LDA,   EXT)
    E(0xB7, STA,   EXT)
    E(0xB8, EORA,  EXT)
    E(0xB9, ADCA,  EXT)
    E(0xBA, ORA,   EXT)
    E(0xBB, ADDA,  EXT)
    E(0xBC, CMPX,  EXT)
    E(0xBD, JSR,   EXT)
    E(0xBE, LDX,   EXT)
    E(0xBF, STX,   EXT)
    E(0xC0, SUBB,  IM8)
    E(0xC1, CMPB,  IM8)
    E(0xC2, SBCB,  IM8)
    E(0xC3, ADDD,  IM16)
    E(0xC4, ANDB,  IM8)
    E(0xC5, BITB,  IM8)
    E(0xC6, LDB,   IM8)
    E(0xC8, EORB,  IM8)
    E(0xC9, ADCB,  IM8)
    E(0xCA, ORB,   IM8)
    E(0xCB, ADDB,  IM8)
    E(0xCC, LDD,   IM16)
    E(0xCE, LDU,   IM16)
    E(0xD0, SUBB,  DIR)
    E(0xD1, CMPB,  DIR)
    E(0xD2, SBCB,  DIR)
    E(0xD3, ADDD,  DIR)
    E(0xD4, ANDB,  DIR)
    E(0xD5, BITB,  DIR)
    E(0xD6, LDB,   DIR)
    E(0xD7, STB,   DIR)
    E(0xD8, EORB,  DIR)
    E(0xD9, ADCB,  DIR)
    E(0xDA, ORB,   DIR)
    E(0xDB, ADDB,  DIR)
    E(0xDC, LDD,   DIR)
    E(0xDD, STD,   DIR)
    E(0xDE, LDU,   DIR)
    E(0xDF, STU,   DIR)
    E(0xE0, SUBB,  IDX)
    E(0xE1, CMPB,  IDX)
    E(0xE2, SBCB,  IDX)
    E(0xE3, ADDD,  IDX)
    E(0xE4, ANDB,  IDX)
    E(0xE5, BITB,  IDX)
    E(0xE6, LDB,   IDX)
    E(0xE7, STB,   IDX)
    E(0xE8, EORB,  IDX)
    E(0xE9, ADCB,  IDX)
    E(0xEA, ORB,   IDX)
    E(0xEB, ADDB,  IDX)
    E(0xEC, LDD,   IDX)
    E(0xED, STD,   IDX)
    E(0xEE, LDU,   IDX)
    E(0xEF, STU,   IDX)
    E(0xF0, SUBB,  EXT)
    E(0xF1, CMPB,  EXT)
    E(0xF2, SBCB,  EXT)
    E(0xF3, ADDD,  EXT)
    E(0xF4, ANDB,  EXT)
    E(0xF5, BITB,  EXT)
    E(0xF6, LDB,   EXT)
    E(0xF7, STB,   EXT)
    E(0xF8, EORB,  EXT)
    E(0xF9, ADCB,  EXT)
    E(0xFA, ORB,   EXT)
    E(0xFB, ADDB,  EXT)
    E(0xFC, LDD,   EXT)
    E(0xFD, STD,   EXT)
    E(0xFE, LDU,   EXT)
    E(0xFF, STU,   EXT)
};

static constexpr Entry MC6809_P10[] PROGMEM = {
    E(0x21, LBRN,  LREL)
    E(0x22, LBHI,  LREL)
    E(0x23, LBLS,  LREL)
    E(0x24, LBHS,  LREL)
    E(0x24, LBCC,  LREL)
    E(0x25, LBLO,  LREL)
    E(0x25, LBCS,  LREL)
    E(0x26, LBNE,  LREL)
    E(0x27, LBEQ,  LREL)
    E(0x28, LBVC,  LREL)
    E(0x29, LBVS,  LREL)
    E(0x2A, LBPL,  LREL)
    E(0x2B, LBMI,  LREL)
    E(0x2C, LBGE,  LREL)
    E(0x2D, LBLT,  LREL)
    E(0x2E, LBGT,  LREL)
    E(0x2F, LBLE,  LREL)
    E(0x3F, SWI2,  NONE)
    E(0x83, CMPD,  IM16)
    E(0x8C, CMPY,  IM16)
    E(0x8E, LDY,   IM16)
    E(0x93, CMPD,  DIR)
    E(0x9C, CMPY,  DIR)
    E(0x9E, LDY,   DIR)
    E(0x9F, STY,   DIR)
    E(0xA3, CMPD,  IDX)
    E(0xAC, CMPY,  IDX)
    E(0xAE, LDY,   IDX)
    E(0xAF, STY,   IDX)
    E(0xB3, CMPD,  EXT)
    E(0xBC, CMPY,  EXT)
    E(0xBE, LDY,   EXT)
    E(0xBF, STY,   EXT)
    E(0xCE, LDS,   IM16)
    E(0xDE, LDS,   DIR)
    E(0xDF, STS,   DIR)
    E(0xEE, LDS,   IDX)
    E(0xEF, STS,   IDX)
    E(0xFE, LDS,   EXT)
    E(0xFF, STS,   EXT)
};

static constexpr Entry MC6809_P11[] PROGMEM = {
    E(0x3F, SWI3,  NONE)
    E(0x83, CMPU,  IM16)
    E(0x8C, CMPS,  IM16)
    E(0x93, CMPU,  DIR)
    E(0x9C, CMPS,  DIR)
    E(0xA3, CMPU,  IDX)
    E(0xAC, CMPS,  IDX)
    E(0xB3, CMPU,  EXT)
    E(0xBC, CMPS,  EXT)
};

static constexpr Entry HD6309_P00[] PROGMEM = {
    X(0x01, OIM,   IM8,  DIR)
    X(0x02, AIM,   IM8,  DIR)
    X(0x05, EIM,   IM8,  DIR)
    X(0x0B, TIM,   IM8,  DIR)
    E(0x14, SEXW,  NONE)
    X(0x61, OIM,   IM8,  IDX)
    X(0x62, AIM,   IM8,  IDX)
    X(0x65, EIM,   IM8,  IDX)
    X(0x6B, TIM,   IM8,  IDX)
    X(0x71, OIM,   IM8,  EXT)
    X(0x72, AIM,   IM8,  EXT)
    X(0x75, EIM,   IM8,  EXT)
    X(0x7B, TIM,   IM8,  EXT)
    E(0xCD, LDQ,   IM32)
};

static constexpr Entry HD6309_P10[] PROGMEM = {
    E(0x30, ADDR,  REG_REG)
    E(0x31, ADCR,  REG_REG)
    E(0x32, SUBR,  REG_REG)
    E(0x33, SBCR,  REG_REG)
    E(0x34, ANDR,  REG_REG)
    E(0x35, ORR,   REG_REG)
    E(0x36, EORR,  REG_REG)
    E(0x37, CMPR,  REG_REG)
    E(0x38, PSHSW, NONE)
    E(0x39, PULSW, NONE)
    E(0x3A, PSHUW, NONE)
    E(0x3B, PULUW, NONE)
    E(0x40, NEGD,  NONE)
    E(0x43, COMD,  NONE)
    E(0x44, LSRD,  NONE)
    E(0x46, RORD,  NONE)
    E(0x47, ASRD,  NONE)
    E(0x48, ASLD,  NONE)
    E(0x48, LSLD,  NONE)
    E(0x49, ROLD,  NONE)
    E(0x4A, DECD,  NONE)
    E(0x4C, INCD,  NONE)
    E(0x4D, TSTD,  NONE)
    E(0x4F, CLRD,  NONE)
    E(0x53, COMW,  NONE)
    E(0x54, LSRW,  NONE)
    E(0x56, RORW,  NONE)
    E(0x59, ROLW,  NONE)
    E(0x5A, DECW,  NONE)
    E(0x5C, INCW,  NONE)
    E(0x5D, TSTW,  NONE)
    E(0x5F, CLRW,  NONE)
    E(0x80, SUBW,  IM16)
    E(0x81, CMPW,  IM16)
    E(0x82, SBCD,  IM16)
    E(0x84, ANDD,  IM16)
    E(0x85, BITD,  IM16)
    E(0x86, LDW,   IM16)
    E(0x88, EORD,  IM16)
    E(0x89, ADCD,  IM16)
    E(0x8A, ORD,   IM16)
    E(0x8B, ADDW,  IM16)
    E(0x90, SUBW,  DIR)
    E(0x91, CMPW,  DIR)
    E(0x92, SBCD,  DIR)
    E(0x94, ANDD,  DIR)
    E(0x95, BITD,  DIR)
    E(0x96, LDW,   DIR)
    E(0x97, STW,   DIR)
    E(0x98, EORD,  DIR)
    E(0x99, ADCD,  DIR)
    E(0x9A, ORD,   DIR)
    E(0x9B, ADDW,  DIR)
    E(0xA0, SUBW,  IDX)
    E(0xA1, CMPW,  IDX)
    E(0xA2, SBCD,  IDX)
    E(0xA4, ANDD,  IDX)
    E(0xA5, BITD,  IDX)
    E(0xA6, LDW,   IDX)
    E(0xA7, STW,   IDX)
    E(0xA8, EORD,  IDX)
    E(0xA9, ADCD,  IDX)
    E(0xAA, ORD,   IDX)
    E(0xAB, ADDW,  IDX)
    E(0xB0, SUBW,  EXT)
    E(0xB1, CMPW,  EXT)
    E(0xB2, SBCD,  EXT)
    E(0xB4, ANDD,  EXT)
    E(0xB5, BITD,  EXT)
    E(0xB6, LDW,   EXT)
    E(0xB7, STW,   EXT)
    E(0xB8, EORD,  EXT)
    E(0xB9, ADCD,  EXT)
    E(0xBA, ORD,   EXT)
    E(0xBB, ADDW,  EXT)
    E(0xDC, LDQ,   DIR)
    E(0xDD, STQ,   DIR)
    E(0xEC, LDQ,   IDX)
    E(0xED, STQ,   IDX)
    E(0xFC, LDQ,   EXT)
    E(0xFD, STQ,   EXT)
};

static constexpr Entry HD6309_P11[] PROGMEM = {
    X(0x30, BAND,  REG_BIT, DIR_BIT)
    X(0x31, BIAND, REG_BIT, DIR_BIT)
    X(0x32, BOR,   REG_BIT, DIR_BIT)
    X(0x33, BIOR,  REG_BIT, DIR_BIT)
    X(0x34, BEOR,  REG_BIT, DIR_BIT)
    X(0x35, BIEOR, REG_BIT, DIR_BIT)
    X(0x36, LDBT,  REG_BIT, DIR_BIT)
    X(0x37, STBT,  REG_BIT, DIR_BIT)
    X(0x38, TFM,   REG_TFM, REG_TFM)  /* R+,R+ */
    X(0x39, TFM,   REG_TFM, REG_TFM)  /* R-,R- */
    X(0x3A, TFM,   REG_TFM, REG_TFM)  /* R+,R */
    X(0x3B, TFM,   REG_TFM, REG_TFM)  /* R,R+ */
    E(0x3C, BITMD, IM8)
    E(0x3D, LDMD,  IM8)
    E(0x43, COME,  NONE)
    E(0x4A, DECE,  NONE)
    E(0x4C, INCE,  NONE)
    E(0x4D, TSTE,  NONE)
    E(0x4F, CLRE,  NONE)
    E(0x53, COMF,  NONE)
    E(0x5A, DECF,  NONE)
    E(0x5C, INCF,  NONE)
    E(0x5D, TSTF,  NONE)
    E(0x5F, CLRF,  NONE)
    E(0x80, SUBE,  IM8)
    E(0x81, CMPE,  IM8)
    E(0x86, LDE,   IM8)
    E(0x8B, ADDE,  IM8)
    E(0x8D, DIVD,  IM16)
    E(0x8E, DIVQ,  IM16)
    E(0x8F, MULD,  IM16)
    E(0x90, SUBE,  DIR)
    E(0x91, CMPE,  DIR)
    E(0x96, LDE,   DIR)
    E(0x97, STE,   DIR)
    E(0x9B, ADDE,  DIR)
    E(0x9D, DIVD,  DIR)
    E(0x9E, DIVQ,  DIR)
    E(0x9F, MULD,  DIR)
    E(0xA0, SUBE,  IDX)
    E(0xA1, CMPE,  IDX)
    E(0xA6, LDE,   IDX)
    E(0xA7, STE,   IDX)
    E(0xAB, ADDE,  IDX)
    E(0xAD, DIVD,  IDX)
    E(0xAE, DIVQ,  IDX)
    E(0xAF, MULD,  IDX)
    E(0xB0, SUBE,  EXT)
    E(0xB1, CMPE,  EXT)
    E(0xB6, LDE,   EXT)
    E(0xB7, STE,   EXT)
    E(0xBB, ADDE,  EXT)
    E(0xBD, DIVD,  EXT)
    E(0xBE, DIVQ,  EXT)
    E(0xBF, MULD,  EXT)
    E(0xC0, SUBF,  IM8)
    E(0xC1, CMPF,  IM8)
    E(0xC6, LDF,   IM8)
    E(0xCB, ADDF,  IM8)
    E(0xD0, SUBF,  DIR)
    E(0xD1, CMPF,  DIR)
    E(0xD6, LDF,   DIR)
    E(0xD7, STF,   DIR)
    E(0xDB, ADDF,  DIR)
    E(0xE0, SUBF,  IDX)
    E(0xE1, CMPF,  IDX)
    E(0xE6, LDF,   IDX)
    E(0xE7, STF,   IDX)
    E(0xEB, ADDF,  IDX)
    E(0xF0, SUBF,  EXT)
    E(0xF1, CMPF,  EXT)
    E(0xF6, LDF,   EXT)
    E(0xF7, STF,   EXT)
    E(0xFB, ADDF,  EXT)
};

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

static bool matchAddrMode(AddrMode opr, AddrMode table) {
    if (opr == table) return true;
    if (opr == IM32) return table == IM8 || table == IM16
                         || table == REGLIST; // immediate register list
    if (opr == REG_REG) return table == IDX // A,X
                            || table == REGLIST; // 2-length register list
    if (opr == REG_BIT) return table == REG_REG; // A,0
    if (opr == DIR) return table == REL || table == LREL;
    if (opr == EXT) return table == REL || table == LREL;
    if (opr == REGLIST) return table == REG_TFM; // X
    if (opr == NONE) return table == REGLIST; // empty register list
    return false;
}

static bool matchAddrMode(Entry::Flags flags, const Entry *entry) {
    const Entry::Flags table = entry->flags();
    return matchAddrMode(flags.mode1(), table.mode1())
        && matchAddrMode(flags.mode2(), table.mode2());
}

Error TableMc6809::searchName(
    InsnMc6809 &insn, const EntryPage *pages, const EntryPage *end) const {
    uint8_t count = 0;
    for (const EntryPage *page = pages; page < end; page++) {
        const Entry *entry = TableBase::searchName<Entry, Entry::Flags>(
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
    for (const EntryPage *page = pages; page < end; page++) {
        if (insn.prefix() != page->prefix()) continue;
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

    constexpr PostEntry(
        RegName _index,
        RegName _base,
        int8_t _size,
        bool _indir,
        uint8_t _mask,
        uint8_t _byte)
        : PostSpec(_index, _base, _size, _indir),
          mask(_mask),
          byte(_byte) {}
};

static const TableMc6809::PostEntry MC6809_POSTBYTE[] PROGMEM = {
    { REG_UNDEF, REG_X,      0, true,  0x8F, 0x84 }, // ,X [,X]
    { REG_UNDEF, REG_X,      5, false, 0x80, 0x00 }, // n5,X
    { REG_UNDEF, REG_X,      8, true,  0x8F, 0x88 }, // n8,X [n8,X]
    { REG_UNDEF, REG_X,     16, true,  0x8F, 0x89 }, // n16,X [n16,X]
    { REG_A,     REG_X,      0, true,  0x8F, 0x86 }, // A,X [A,X]
    { REG_B,     REG_X,      0, true,  0x8F, 0x85 }, // B,X [B,X]
    { REG_D,     REG_X,      0, true,  0x8F, 0x8B }, // D,X [D,X]
    { REG_UNDEF, REG_X,      1, false, 0x9F, 0x80 }, // ,X+
    { REG_UNDEF, REG_X,      2, true,  0x8F, 0x81 }, // ,X++ [,X++]
    { REG_UNDEF, REG_X,     -1, false, 0x9F, 0x82 }, // ,-X
    { REG_UNDEF, REG_X,     -2, true,  0x8F, 0x83 }, // ,--X [,--X]
    { REG_UNDEF, REG_PCR,    8, true,  0x8F, 0x8C }, // n8,PCR [n8,PCR]
    { REG_UNDEF, REG_PCR,   16, true,  0x8F, 0x8D }, // n16,PCR [n16,PCR]
    { REG_UNDEF, REG_UNDEF, 16, true,  0xFF, 0x9F }, // [n16]
};

static const TableMc6809::PostEntry HD6309_POSTBYTE[] PROGMEM = {
    { REG_E,     REG_X,   0, true,  0x8F, 0x87 }, // E,X [E,X]
    { REG_F,     REG_X,   0, true,  0x8F, 0x8A }, // F,X [F,X]
    { REG_W,     REG_X,   0, true,  0x8F, 0x8E }, // W,X [W,X]
    { REG_UNDEF, REG_W,   0, false, 0xFF, 0x8F }, // ,W
    { REG_UNDEF, REG_W,   0, true,  0xFF, 0x90 }, // [,W]
    { REG_UNDEF, REG_W,  16, false, 0xFF, 0xAF }, // n16,W
    { REG_UNDEF, REG_W,  16, true,  0xFF, 0xB0 }, // [n16,W]
    { REG_UNDEF, REG_W,   2, false, 0xFF, 0xCF }, // ,W++
    { REG_UNDEF, REG_W,   2, true,  0xFF, 0xD0 }, // [,W++]
    { REG_UNDEF, REG_W,  -2, false, 0xFF, 0xEF }, // ,--W
    { REG_UNDEF, REG_W,  -2, true,  0xFF, 0xF0 }, // [,--W]
};

Error TableMc6809::searchPostByte(
    const uint8_t post, PostSpec &spec,
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
        if (specBase == base
            && spec.size == size
            && spec.index == RegName(pgm_read_byte(&entry->index))) {
            uint8_t byte = pgm_read_byte(&entry->byte);
            if (spec.indir) {
                if (pgm_read_byte(&entry->indir) == 0) continue;
                byte |= 0x10;
            }
            spec.base = base;
            return byte;
        }
    }
    return -1;
}

Error TableMc6809::searchPostByte(
    const uint8_t post, PostSpec &spec) const {
    if (_cpuType == HD6309
        && searchPostByte(post, spec, ARRAY_RANGE(HD6309_POSTBYTE)) == OK)
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
    if (strcasecmp_P(cpu, TEXT_CPU_MC6809) == 0
        || strcasecmp_P(cpu, TEXT_CPU_6809) == 0)
        return setCpu(MC6809);
    if (strcasecmp_P(cpu, TEXT_CPU_HD6309) == 0
        || strcasecmp_P(cpu, TEXT_CPU_6309) == 0)
        return setCpu(HD6309);
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

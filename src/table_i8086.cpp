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

#include "config_i8086.h"
#include "entry_i8086.h"
#include "table_i8086.h"
#include "text_i8086.h"

#include <string.h>

namespace libasm {
namespace i8086 {

#define E(_opc, _name, _sz, _dst, _src, _dpos, _spos)           \
    { _opc,                                                     \
      Entry::_flags(_dst, _src, _dpos, _spos, SZ_##_sz, false), \
      TEXT_##_name },
#define S(_opc, _name, _sz, _dst, _src, _dpos, _spos)           \
    { _opc,                                                     \
      Entry::_flags(_dst, _src, _dpos, _spos, SZ_##_sz, true), \
      TEXT_##_name },

static const Entry TABLE_00[] PROGMEM = {
    E(0x00, ADD,    BYTE, M_BMOD, M_BREG, P_MOD,  P_REG)
    E(0x01, ADD,    WORD, M_WMOD, M_WREG, P_MOD,  P_REG)
    E(0x02, ADD,    BYTE, M_BREG, M_BMOD, P_REG,  P_MOD)
    E(0x03, ADD,    WORD, M_WREG, M_WMOD, P_REG,  P_MOD)
    E(0x04, ADD,    BYTE, M_AL,   M_IMM,  P_NONE, P_OPR)
    E(0x05, ADD,    WORD, M_AX,   M_IMM,  P_NONE, P_OPR)
    E(0x06, PUSH,   WORD, M_SREG, M_NONE, P_OSEG, P_NONE)
    E(0x07, POP,    WORD, M_SREG, M_NONE, P_OSEG, P_NONE)
    E(0x08, OR,     BYTE, M_BMOD, M_BREG, P_MOD,  P_REG)
    E(0x09, OR,     WORD, M_WMOD, M_WREG, P_MOD,  P_REG)
    E(0x0A, OR,     BYTE, M_BREG, M_BMOD, P_REG,  P_MOD)
    E(0x0B, OR,     WORD, M_WREG, M_WMOD, P_REG,  P_MOD)
    E(0x0C, OR,     BYTE, M_AL,   M_IMM,  P_NONE, P_OPR)
    E(0x0D, OR,     WORD, M_AX,   M_IMM,  P_NONE, P_OPR)
    E(0x10, ADC,    BYTE, M_BMOD, M_BREG, P_MOD,  P_REG)
    E(0x11, ADC,    WORD, M_WMOD, M_WREG, P_MOD,  P_REG)
    E(0x12, ADC,    BYTE, M_BREG, M_BMOD, P_REG,  P_MOD)
    E(0x13, ADC,    WORD, M_WREG, M_WMOD, P_REG,  P_MOD)
    E(0x14, ADC,    BYTE, M_AL,   M_IMM,  P_NONE, P_OPR)
    E(0x15, ADC,    WORD, M_AX,   M_IMM,  P_NONE, P_OPR)
    E(0x18, SBB,    BYTE, M_BMOD, M_BREG, P_MOD,  P_REG)
    E(0x19, SBB,    WORD, M_WMOD, M_WREG, P_MOD,  P_REG)
    E(0x1A, SBB,    BYTE, M_BREG, M_BMOD, P_REG,  P_MOD)
    E(0x1B, SBB,    WORD, M_WREG, M_WMOD, P_REG,  P_MOD)
    E(0x1C, SBB,    BYTE, M_AL,   M_IMM,  P_NONE, P_OPR)
    E(0x1D, SBB,    WORD, M_AX,   M_IMM,  P_NONE, P_OPR)
    E(0x20, AND,    BYTE, M_BMOD, M_BREG, P_MOD,  P_REG)
    E(0x21, AND,    WORD, M_WMOD, M_WREG, P_MOD,  P_REG)
    E(0x22, AND,    BYTE, M_BREG, M_BMOD, P_REG,  P_MOD)
    E(0x23, AND,    WORD, M_WREG, M_WMOD, P_REG,  P_MOD)
    E(0x24, AND,    BYTE, M_AL,   M_IMM,  P_NONE, P_OPR)
    E(0x25, AND,    WORD, M_AX,   M_IMM,  P_NONE, P_OPR)
    E(0x27, DAA,    BYTE, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0x28, SUB,    BYTE, M_BMOD, M_BREG, P_MOD,  P_REG)
    E(0x29, SUB,    WORD, M_WMOD, M_WREG, P_MOD,  P_REG)
    E(0x2A, SUB,    BYTE, M_BREG, M_BMOD, P_REG,  P_MOD)
    E(0x2B, SUB,    WORD, M_WREG, M_WMOD, P_REG,  P_MOD)
    E(0x2C, SUB,    BYTE, M_AL,   M_IMM,  P_NONE, P_OPR)
    E(0x2D, SUB,    WORD, M_AX,   M_IMM,  P_NONE, P_OPR)
    E(0x2F, DAS,    BYTE, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0x30, XOR,    BYTE, M_BMOD, M_BREG, P_MOD,  P_REG)
    E(0x31, XOR,    WORD, M_WMOD, M_WREG, P_MOD,  P_REG)
    E(0x32, XOR,    BYTE, M_BREG, M_BMOD, P_REG,  P_MOD)
    E(0x33, XOR,    WORD, M_WREG, M_WMOD, P_REG,  P_MOD)
    E(0x34, XOR,    BYTE, M_AL,   M_IMM,  P_NONE, P_OPR)
    E(0x35, XOR,    WORD, M_AX,   M_IMM,  P_NONE, P_OPR)
    E(0x37, AAA,    BYTE, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0x38, CMP,    BYTE, M_BMOD, M_BREG, P_MOD,  P_REG)
    E(0x39, CMP,    WORD, M_WMOD, M_WREG, P_MOD,  P_REG)
    E(0x3A, CMP,    BYTE, M_BREG, M_BMOD, P_REG,  P_MOD)
    E(0x3B, CMP,    WORD, M_WREG, M_WMOD, P_REG,  P_MOD)
    E(0x3C, CMP,    BYTE, M_AL,   M_IMM,  P_NONE, P_OPR)
    E(0x3D, CMP,    WORD, M_AX,   M_IMM,  P_NONE, P_OPR)
    E(0x3F, AAS,    BYTE, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0x40, INC,    WORD, M_WREG, M_NONE, P_OREG, P_NONE)
    E(0x48, DEC,    WORD, M_WREG, M_NONE, P_OREG, P_NONE)
    E(0x50, PUSH,   WORD, M_WREG, M_NONE, P_OREG, P_NONE)
    E(0x58, POP,    WORD, M_WREG, M_NONE, P_OREG, P_NONE)
    E(0x70, JO,     NONE, M_REL8, M_NONE, P_OPR,  P_NONE)
    E(0x71, JNO,    NONE, M_REL8, M_NONE, P_OPR,  P_NONE)
    E(0x72, JB,     NONE, M_REL8, M_NONE, P_OPR,  P_NONE)
    E(0x72, JC,     NONE, M_REL8, M_NONE, P_OPR,  P_NONE)
    E(0x72, JNAE,   NONE, M_REL8, M_NONE, P_OPR,  P_NONE)
    E(0x73, JAE,    NONE, M_REL8, M_NONE, P_OPR,  P_NONE)
    E(0x73, JNB,    NONE, M_REL8, M_NONE, P_OPR,  P_NONE)
    E(0x73, JNC,    NONE, M_REL8, M_NONE, P_OPR,  P_NONE)
    E(0x74, JE,     NONE, M_REL8, M_NONE, P_OPR,  P_NONE)
    E(0x74, JZ,     NONE, M_REL8, M_NONE, P_OPR,  P_NONE)
    E(0x75, JNE,    NONE, M_REL8, M_NONE, P_OPR,  P_NONE)
    E(0x75, JNZ,    NONE, M_REL8, M_NONE, P_OPR,  P_NONE)
    E(0x76, JBE,    NONE, M_REL8, M_NONE, P_OPR,  P_NONE)
    E(0x76, JNA,    NONE, M_REL8, M_NONE, P_OPR,  P_NONE)
    E(0x77, JA,     NONE, M_REL8, M_NONE, P_OPR,  P_NONE)
    E(0x77, JNBE,   NONE, M_REL8, M_NONE, P_OPR,  P_NONE)
    E(0x78, JS,     NONE, M_REL8, M_NONE, P_OPR,  P_NONE)
    E(0x79, JNS,    NONE, M_REL8, M_NONE, P_OPR,  P_NONE)
    E(0x7A, JPE,    NONE, M_REL8, M_NONE, P_OPR,  P_NONE)
    E(0x7A, JP,     NONE, M_REL8, M_NONE, P_OPR,  P_NONE)
    E(0x7B, JPO,    NONE, M_REL8, M_NONE, P_OPR,  P_NONE)
    E(0x7B, JNP,    NONE, M_REL8, M_NONE, P_OPR,  P_NONE)
    E(0x7C, JL,     NONE, M_REL8, M_NONE, P_OPR,  P_NONE)
    E(0x7C, JNGE,   NONE, M_REL8, M_NONE, P_OPR,  P_NONE)
    E(0x7D, JGE,    NONE, M_REL8, M_NONE, P_OPR,  P_NONE)
    E(0x7D, JNL,    NONE, M_REL8, M_NONE, P_OPR,  P_NONE)
    E(0x7E, JLE,    NONE, M_REL8, M_NONE, P_OPR,  P_NONE)
    E(0x7E, JNG,    NONE, M_REL8, M_NONE, P_OPR,  P_NONE)
    E(0x7F, JG,     NONE, M_REL8, M_NONE, P_OPR,  P_NONE)
    E(0x7F, JNLE,   NONE, M_REL8, M_NONE, P_OPR,  P_NONE)
    E(0x84, TEST,   BYTE, M_BREG, M_BMOD, P_REG,  P_MOD)
    E(0x84, TEST,   BYTE, M_BMOD, M_BREG, P_MOD,  P_REG)
    E(0x85, TEST,   WORD, M_WREG, M_WMOD, P_REG,  P_MOD)
    E(0x85, TEST,   WORD, M_WMOD, M_WREG, P_MOD,  P_REG)
    E(0x86, XCHG,   BYTE, M_BMOD, M_BREG, P_MOD,  P_REG)
    E(0x86, XCHG,   BYTE, M_BREG, M_BMOD, P_REG,  P_MOD)
    E(0x87, XCHG,   WORD, M_WMOD, M_WREG, P_MOD,  P_REG)
    E(0x87, XCHG,   WORD, M_WREG, M_WMOD, P_REG,  P_MOD)
    E(0x88, MOV,    BYTE, M_BMOD, M_BREG, P_MOD,  P_REG)
    E(0x89, MOV,    WORD, M_WMOD, M_WREG, P_MOD,  P_REG)
    E(0x8A, MOV,    BYTE, M_BREG, M_BMOD, P_REG,  P_MOD)
    E(0x8B, MOV,    WORD, M_WREG, M_WMOD, P_REG,  P_MOD)
    E(0x8C, MOV,    WORD, M_WMOD, M_SREG, P_MOD,  P_REG)
    E(0x8D, LEA,    WORD, M_WREG, M_BMEM, P_REG,  P_MOD)
    E(0x8D, LEA,    WORD, M_WREG, M_WMEM, P_REG,  P_MOD)
    E(0x8E, MOV,    WORD, M_SREG, M_WMOD, P_REG,  P_MOD)
    E(0x90, NOP,    NONE, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0x90, XCHG,   WORD, M_AX,   M_WREG, P_NONE, P_OREG)
    E(0x90, XCHG,   WORD, M_WREG, M_AX,   P_OREG, P_NONE)
    E(0x98, CBW,    BYTE, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0x99, CWD,    WORD, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0x9A, CALLF,  NONE, M_FAR,  M_NONE, P_OPR,  P_NONE)
    E(0x9B, WAIT,   NONE, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0x9C, PUSHF,  WORD, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0x9D, POPF,   WORD, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0x9E, SAHF,   BYTE, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0x9F, LAHF,   BYTE, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0xA0, MOV,    BYTE, M_AL,   M_BDIR, P_NONE, P_NONE)
    E(0xA1, MOV,    WORD, M_AX,   M_WDIR, P_NONE, P_NONE)
    E(0xA2, MOV,    BYTE, M_BDIR, M_AL,   P_NONE, P_NONE)
    E(0xA3, MOV,    WORD, M_WDIR, M_AX,   P_NONE, P_NONE)
    S(0xA4, MOVSB,  BYTE, M_NONE, M_NONE, P_NONE, P_NONE)
    S(0xA5, MOVSW,  WORD, M_NONE, M_NONE, P_NONE, P_NONE)
    S(0xA6, CMPSB,  BYTE, M_NONE, M_NONE, P_NONE, P_NONE)
    S(0xA7, CMPSW,  WORD, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0xA8, TEST,   BYTE, M_AL,   M_IMM,  P_NONE, P_OPR)
    E(0xA9, TEST,   WORD, M_AX,   M_IMM,  P_NONE, P_OPR)
    S(0xAA, STOSB,  BYTE, M_NONE, M_NONE, P_NONE, P_NONE)
    S(0xAB, STOSW,  WORD, M_NONE, M_NONE, P_NONE, P_NONE)
    S(0xAC, LODSB,  BYTE, M_NONE, M_NONE, P_NONE, P_NONE)
    S(0xAD, LODSW,  WORD, M_NONE, M_NONE, P_NONE, P_NONE)
    S(0xAE, SCASB,  BYTE, M_NONE, M_NONE, P_NONE, P_NONE)
    S(0xAF, SCASW,  WORD, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0xB0, MOV,    BYTE, M_BREG, M_IMM,  P_OREG, P_OPR)
    E(0xB8, MOV,    WORD, M_WREG, M_IMM,  P_OREG, P_OPR)
    E(0xC2, RET,    WORD, M_IMM,  M_NONE, P_OPR,  P_NONE)
    E(0xC3, RET,    NONE, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0xC4, LES,    WORD, M_WREG, M_WMEM, P_REG,  P_MOD)
    E(0xC5, LDS,    WORD, M_WREG, M_WMEM, P_REG,  P_MOD)
    E(0xCA, RETF,   WORD, M_IMM,  M_NONE, P_OPR,  P_NONE)
    E(0xCB, RETF,   NONE, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0xCC, INT,    NONE, M_VAL3, M_NONE, P_NONE, P_NONE)
    E(0xCD, INT,    BYTE, M_IMM,  M_NONE, P_OPR,  P_NONE)
    E(0xCE, INTO,   NONE, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0xCF, IRET,   NONE, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0xD7, XLAT,   NONE, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0xE0, LOOPNE, NONE, M_REL8, M_NONE, P_OPR,  P_NONE)
    E(0xE0, LOOPNZ, NONE, M_REL8, M_NONE, P_OPR,  P_NONE)
    E(0xE1, LOOPE,  NONE, M_REL8, M_NONE, P_OPR,  P_NONE)
    E(0xE1, LOOPZ,  NONE, M_REL8, M_NONE, P_OPR,  P_NONE)
    E(0xE2, LOOP,   NONE, M_REL8, M_NONE, P_OPR,  P_NONE)
    E(0xE3, JCXZ,   NONE, M_REL8, M_NONE, P_OPR,  P_NONE)
    E(0xE4, IN,     BYTE, M_AL,   M_IOA,  P_NONE, P_OPR)
    E(0xE5, IN,     WORD, M_AX,   M_IOA,  P_NONE, P_OPR)
    E(0xE6, OUT,    BYTE, M_IOA,  M_AL,   P_OPR,  P_NONE)
    E(0xE7, OUT,    WORD, M_IOA,  M_AX,   P_OPR,  P_NONE)
    E(0xE8, CALL,   NONE, M_REL,  M_NONE, P_OPR,  P_NONE)
    E(0xE9, JMP,    NONE, M_REL,  M_NONE, P_OPR,  P_NONE)
    E(0xEA, JMPF,   NONE, M_FAR,  M_NONE, P_OPR,  P_NONE)
    E(0xEB, JMP,    NONE, M_REL8, M_NONE, P_OPR,  P_NONE)
    E(0xEC, IN,     BYTE, M_AL,   M_DX,   P_NONE, P_NONE)
    E(0xED, IN,     WORD, M_AX,   M_DX,   P_NONE, P_NONE)
    E(0xEE, OUT,    BYTE, M_DX,   M_AL,   P_NONE, P_NONE)
    E(0xEF, OUT,    WORD, M_DX,   M_AX,   P_NONE, P_NONE)
    E(0xF0, LOCK,   NONE, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0xF2, REPNE,  NONE, M_ISTR, M_NONE, P_OPR,  P_NONE)
    E(0xF2, REPNZ,  NONE, M_ISTR, M_NONE, P_OPR,  P_NONE)
    E(0xF3, REP,    NONE, M_ISTR, M_NONE, P_OPR,  P_NONE)
    E(0xF3, REPE,   NONE, M_ISTR, M_NONE, P_OPR,  P_NONE)
    E(0xF3, REPZ,   NONE, M_ISTR, M_NONE, P_OPR,  P_NONE)
    E(0xF4, HLT,    NONE, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0xF5, CMC,    NONE, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0xF8, CLC,    NONE, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0xF9, STC,    NONE, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0xFA, CLI,    NONE, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0xFB, STI,    NONE, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0xFC, CLD,    NONE, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0xFD, STD,    NONE, M_NONE, M_NONE, P_NONE, P_NONE)
};

static const Entry TABLE_80[] PROGMEM = {
    E(000, ADD, BYTE, M_BMOD, M_IMM, P_OMOD, P_OPR)
    E(010, OR,  BYTE, M_BMOD, M_IMM, P_OMOD, P_OPR)
    E(020, ADC, BYTE, M_BMOD, M_IMM, P_OMOD, P_OPR)
    E(030, SBB, BYTE, M_BMOD, M_IMM, P_OMOD, P_OPR)
    E(040, AND, BYTE, M_BMOD, M_IMM, P_OMOD, P_OPR)
    E(050, SUB, BYTE, M_BMOD, M_IMM, P_OMOD, P_OPR)
    E(060, XOR, BYTE, M_BMOD, M_IMM, P_OMOD, P_OPR)
    E(070, CMP, BYTE, M_BMOD, M_IMM, P_OMOD, P_OPR)
};

static const Entry TABLE_81[] PROGMEM = {
    E(000, ADD, WORD, M_WMOD, M_IMM, P_OMOD, P_OPR)
    E(010, OR,  WORD, M_WMOD, M_IMM, P_OMOD, P_OPR)
    E(020, ADC, WORD, M_WMOD, M_IMM, P_OMOD, P_OPR)
    E(030, SBB, WORD, M_WMOD, M_IMM, P_OMOD, P_OPR)
    E(040, AND, WORD, M_WMOD, M_IMM, P_OMOD, P_OPR)
    E(050, SUB, WORD, M_WMOD, M_IMM, P_OMOD, P_OPR)
    E(060, XOR, WORD, M_WMOD, M_IMM, P_OMOD, P_OPR)
    E(070, CMP, WORD, M_WMOD, M_IMM, P_OMOD, P_OPR)
};

static const Entry TABLE_83[] PROGMEM = {
    E(000, ADD, WORD, M_WMOD, M_IMM8, P_OMOD, P_OPR)
    E(020, ADC, WORD, M_WMOD, M_IMM8, P_OMOD, P_OPR)
    E(030, SBB, WORD, M_WMOD, M_IMM8, P_OMOD, P_OPR)
    E(050, SUB, WORD, M_WMOD, M_IMM8, P_OMOD, P_OPR)
    E(070, CMP, WORD, M_WMOD, M_IMM8, P_OMOD, P_OPR)
};

static const Entry TABLE_8F[] PROGMEM = {
    E(000, POP, NONE, M_WMOD, M_NONE, P_OMOD, P_NONE)
};

static const Entry TABLE_C6[] PROGMEM = {
    E(000, MOV, BYTE, M_BMOD, M_IMM, P_OMOD, P_OPR)
};

static const Entry TABLE_C7[] PROGMEM = {
    E(000, MOV, WORD, M_WMOD, M_IMM, P_OMOD, P_OPR)
};

static const Entry TABLE_D0[] PROGMEM = {
    E(000, ROL, BYTE, M_BMOD, M_VAL1, P_OMOD, P_NONE)
    E(010, ROR, BYTE, M_BMOD, M_VAL1, P_OMOD, P_NONE)
    E(020, RCL, BYTE, M_BMOD, M_VAL1, P_OMOD, P_NONE)
    E(030, RCR, BYTE, M_BMOD, M_VAL1, P_OMOD, P_NONE)
    E(040, SHL, BYTE, M_BMOD, M_VAL1, P_OMOD, P_NONE)
    E(040, SAL, BYTE, M_BMOD, M_VAL1, P_OMOD, P_NONE)
    E(050, SHR, BYTE, M_BMOD, M_VAL1, P_OMOD, P_NONE)
    E(070, SAR, BYTE, M_BMOD, M_VAL1, P_OMOD, P_NONE)
};

static const Entry TABLE_D1[] PROGMEM = {
    E(000, ROL, WORD, M_WMOD, M_VAL1, P_OMOD, P_NONE)
    E(010, ROR, WORD, M_WMOD, M_VAL1, P_OMOD, P_NONE)
    E(020, RCL, WORD, M_WMOD, M_VAL1, P_OMOD, P_NONE)
    E(030, RCR, WORD, M_WMOD, M_VAL1, P_OMOD, P_NONE)
    E(040, SHL, WORD, M_WMOD, M_VAL1, P_OMOD, P_NONE)
    E(040, SAL, WORD, M_WMOD, M_VAL1, P_OMOD, P_NONE)
    E(050, SHR, WORD, M_WMOD, M_VAL1, P_OMOD, P_NONE)
    E(070, SAR, WORD, M_WMOD, M_VAL1, P_OMOD, P_NONE)
};

static const Entry TABLE_D2[] PROGMEM = {
    E(000, ROL, BYTE, M_BMOD, M_CL, P_OMOD, P_NONE)
    E(010, ROR, BYTE, M_BMOD, M_CL, P_OMOD, P_NONE)
    E(020, RCL, BYTE, M_BMOD, M_CL, P_OMOD, P_NONE)
    E(030, RCR, BYTE, M_BMOD, M_CL, P_OMOD, P_NONE)
    E(040, SHL, BYTE, M_BMOD, M_CL, P_OMOD, P_NONE)
    E(040, SAL, BYTE, M_BMOD, M_CL, P_OMOD, P_NONE)
    E(050, SHR, BYTE, M_BMOD, M_CL, P_OMOD, P_NONE)
    E(070, SAR, BYTE, M_BMOD, M_CL, P_OMOD, P_NONE)
};

static const Entry TABLE_D3[] PROGMEM = {
    E(000, ROL, WORD, M_WMOD, M_CL, P_OMOD, P_NONE)
    E(010, ROR, WORD, M_WMOD, M_CL, P_OMOD, P_NONE)
    E(020, RCL, WORD, M_WMOD, M_CL, P_OMOD, P_NONE)
    E(030, RCR, WORD, M_WMOD, M_CL, P_OMOD, P_NONE)
    E(040, SHL, WORD, M_WMOD, M_CL, P_OMOD, P_NONE)
    E(040, SAL, WORD, M_WMOD, M_CL, P_OMOD, P_NONE)
    E(050, SHR, WORD, M_WMOD, M_CL, P_OMOD, P_NONE)
    E(070, SAR, WORD, M_WMOD, M_CL, P_OMOD, P_NONE)
};

static const Entry TABLE_D4[] PROGMEM = {
    E(0x0A, AAM, BYTE, M_NONE, M_NONE, P_NONE, P_NONE)
};

static const Entry TABLE_D5[] PROGMEM = {
    E(0x0A, AAD, BYTE, M_NONE, M_NONE, P_NONE, P_NONE)
};

static const Entry TABLE_F6[] PROGMEM = {
    E(000, TEST, BYTE, M_BMOD, M_IMM,  P_OMOD, P_OPR)
    E(020, NOT,  BYTE, M_BMOD, M_NONE, P_OMOD, P_NONE)
    E(030, NEG,  BYTE, M_BMOD, M_NONE, P_OMOD, P_NONE)
    E(040, MUL,  BYTE, M_BMOD, M_NONE, P_OMOD, P_NONE)
    E(050, IMUL, BYTE, M_BMOD, M_NONE, P_OMOD, P_NONE)
    E(060, DIV,  BYTE, M_BMOD, M_NONE, P_OMOD, P_NONE)
    E(070, IDIV, BYTE, M_BMOD, M_NONE, P_OMOD, P_NONE)
};

static const Entry TABLE_F7[] PROGMEM = {
    E(000, TEST, WORD, M_WMOD, M_IMM,  P_OMOD, P_OPR)
    E(020, NOT,  WORD, M_WMOD, M_NONE, P_OMOD, P_NONE)
    E(030, NEG,  WORD, M_WMOD, M_NONE, P_OMOD, P_NONE)
    E(040, MUL,  WORD, M_WMOD, M_NONE, P_OMOD, P_NONE)
    E(050, IMUL, WORD, M_WMOD, M_NONE, P_OMOD, P_NONE)
    E(060, DIV,  WORD, M_WMOD, M_NONE, P_OMOD, P_NONE)
    E(070, IDIV, WORD, M_WMOD, M_NONE, P_OMOD, P_NONE)
};

static const Entry TABLE_FE[] PROGMEM = {
    E(000, INC, BYTE, M_BMOD, M_NONE, P_OMOD, P_NONE)
    E(010, DEC, BYTE, M_BMOD, M_NONE, P_OMOD, P_NONE)
};

static const Entry TABLE_FF[] PROGMEM = {
    E(000, INC,   WORD, M_WMOD, M_NONE, P_OMOD, P_NONE)
    E(010, DEC,   WORD, M_WMOD, M_NONE, P_OMOD, P_NONE)
    E(020, CALL,  NONE, M_WMOD, M_NONE, P_OMOD, P_NONE)
    E(030, CALLF, NONE, M_WMEM, M_NONE, P_OMOD, P_NONE)
    E(040, JMP,   NONE, M_WMOD, M_NONE, P_OMOD, P_NONE)
    E(050, JMPF,  NONE, M_WMEM, M_NONE, P_OMOD, P_NONE)
    E(060, PUSH,  NONE, M_WMOD, M_NONE, P_OMOD, P_NONE)
};

struct TableI8086::EntryPage {
    const Config::opcode_t first;
    const Entry *const table;
    const Entry *const end;
};

static const TableI8086::EntryPage I8086_PAGES[] PROGMEM = {
    { 0x00, ARRAY_RANGE(TABLE_00) },
    { 0x80, ARRAY_RANGE(TABLE_80) },
    { 0x81, ARRAY_RANGE(TABLE_81) },
    { 0x83, ARRAY_RANGE(TABLE_83) },
    { 0x8F, ARRAY_RANGE(TABLE_8F) },
    { 0xC6, ARRAY_RANGE(TABLE_C6) },
    { 0xC7, ARRAY_RANGE(TABLE_C7) },
    { 0xD0, ARRAY_RANGE(TABLE_D0) },
    { 0xD1, ARRAY_RANGE(TABLE_D1) },
    { 0xD2, ARRAY_RANGE(TABLE_D2) },
    { 0xD3, ARRAY_RANGE(TABLE_D3) },
    { 0xD4, ARRAY_RANGE(TABLE_D4) },
    { 0xD5, ARRAY_RANGE(TABLE_D5) },
    { 0xF6, ARRAY_RANGE(TABLE_F6) },
    { 0xF7, ARRAY_RANGE(TABLE_F7) },
    { 0xFE, ARRAY_RANGE(TABLE_FE) },
    { 0xFF, ARRAY_RANGE(TABLE_FF) },
};

bool TableI8086::isRepeatPrefix(Config::opcode_t opCode) const {
    return opCode == 0xF2 || opCode == 0xF3;
}

bool TableI8086::isSegmentPrefix(Config::opcode_t opCode) const {
    return overrideSeg(opCode) != REG_UNDEF;
}

RegName TableI8086::overrideSeg(Config::opcode_t opCode) const {
    switch (opCode) {
    case 0x26: return REG_ES;
    case 0x2E: return REG_CS;
    case 0x36: return REG_SS;
    case 0x3E: return REG_DS;
    default:   return REG_UNDEF;
    }
}

Config::opcode_t TableI8086::segOverridePrefix(RegName name) const {
    switch (name) {
    case REG_ES: return 0x26;
    case REG_CS: return 0x2E;
    case REG_SS: return 0x36;
    case REG_DS: return 0x3E;
    default:     return 0;
    }
}

bool TableI8086::isFirstByte(Config::opcode_t opCode) const {
    for (const EntryPage *page = ARRAY_BEGIN(I8086_PAGES);
         page < ARRAY_END(I8086_PAGES); page++) {
        const Config::opcode_t first = pgm_read_byte(&page->first);
        if (first == 0) continue;
        if (first == opCode) return true;
    }
    return false;
}

static Config::opcode_t maskCode(Config::opcode_t opcode, const Entry *entry) {
    const uint8_t pos = Entry::_pos(pgm_read_dword(&entry->flags));
    const OprPos dstPos = Entry::_dstPos(pos);
    const OprPos srcPos = Entry::_srcPos(pos);
    Config::opcode_t mask = 0;
    if (dstPos == P_OREG || srcPos == P_OREG) mask |= 0007;
    if (dstPos == P_OSEG || srcPos == P_OSEG) mask |= 0030;
    if (dstPos == P_OMOD || srcPos == P_OMOD) mask |= 0307;
    return opcode & ~mask;
}

Error TableI8086::searchOpCode(
    InsnI8086 &insn, const EntryPage *pages, const EntryPage *end) const {
    for (const EntryPage *page = pages; page < end; page++) {
        const Config::opcode_t first = pgm_read_byte(&page->first);
        if (insn.first() != first) continue;
        const Entry *table =
            reinterpret_cast<Entry *>(pgm_read_ptr(&page->table));
        const Entry *end = reinterpret_cast<Entry *>(pgm_read_ptr(&page->end));
        const Entry *entry = TableBase::searchCode<Entry,Config::opcode_t>(
            insn.opCode(), table, end, maskCode);
        if (entry) {
            insn.setFlags(pgm_read_dword(&entry->flags));
            const char *name =
                reinterpret_cast<const char *>(pgm_read_ptr(&entry->name));
            TableBase::setName(insn.insn(), name, Config::NAME_MAX);
            return OK;
        }
    }
    return UNKNOWN_INSTRUCTION;
}

Error TableI8086::searchOpCode(InsnI8086 &insn) const {
    return _error.setError(searchOpCode(insn, ARRAY_RANGE(I8086_PAGES)));
}

const char *TableI8086::listCpu() const {
    return TEXT_CPU_I8086;
}

const char *TableI8086::getCpu() const {
    return TEXT_CPU_8086;
}

bool TableI8086::setCpu(const char *cpu) {
    if (toupper(*cpu) == 'I') cpu++;
    return strcmp_P(cpu, TEXT_CPU_8086) == 0;
}

class TableI8086 TableI8086;

} // namespace i8086
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

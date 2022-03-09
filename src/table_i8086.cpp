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

#include "table_i8086.h"

#include <string.h>

#include "config_i8086.h"
#include "entry_i8086.h"
#include "text_i8086.h"

namespace libasm {
namespace i8086 {

#define _ENTRY(_opc, _name, _sz, _dst, _src, _dpos, _spos, _stri) \
    { _opc, Entry::Flags::create(_dst, _src, _dpos, _spos, SZ_##_sz, _stri), _name }
#define E(_opc, _name, _sz, _dst, _src, _dpos, _spos) \
    _ENTRY(_opc, _name, _sz, _dst, _src, _dpos, _spos, false)
#define S(_opc, _name, _sz, _dst, _src, _dpos, _spos) \
    _ENTRY(_opc, _name, _sz, _dst, _src, _dpos, _spos, true)

// clang-format off
static const Entry TABLE_00[] PROGMEM = {
    E(0xA0, TEXT_MOV,    BYTE, M_AL,   M_BDIR, P_NONE, P_OPR),
    E(0xA1, TEXT_MOV,    WORD, M_AX,   M_WDIR, P_NONE, P_OPR),
    E(0xA2, TEXT_MOV,    BYTE, M_BDIR, M_AL,   P_OPR,  P_NONE),
    E(0xA3, TEXT_MOV,    WORD, M_WDIR, M_AX,   P_OPR,  P_NONE),
    E(0x90, TEXT_NOP,    NONE, M_NONE, M_NONE, P_NONE, P_NONE),
    E(0x90, TEXT_XCHG,   WORD, M_AX,   M_WREG, P_NONE, P_OREG),
    E(0x90, TEXT_XCHG,   WORD, M_WREG, M_AX,   P_OREG, P_NONE),
    E(0x04, TEXT_ADD,    BYTE, M_AL,   M_IMM,  P_NONE, P_OPR),
    E(0x05, TEXT_ADD,    WORD, M_AX,   M_IMM,  P_NONE, P_OPR),
    E(0x0C, TEXT_OR,     BYTE, M_AL,   M_IMM,  P_NONE, P_OPR),
    E(0x0D, TEXT_OR,     WORD, M_AX,   M_IMM,  P_NONE, P_OPR),
    E(0x02, TEXT_ADD,    BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    E(0x00, TEXT_ADD,    BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E(0x03, TEXT_ADD,    WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    E(0x01, TEXT_ADD,    WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    E(0x06, TEXT_PUSH,   WORD, M_SREG, M_NONE, P_OSEG, P_NONE),
    E(0x0F, TEXT_POP,    WORD, M_CS,   M_NONE, P_NONE, P_NONE),
    E(0x07, TEXT_POP,    WORD, M_SREG, M_NONE, P_OSEG, P_NONE),
    E(0x0A, TEXT_OR,     BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    E(0x08, TEXT_OR,     BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E(0x0B, TEXT_OR,     WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    E(0x09, TEXT_OR,     WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    E(0x12, TEXT_ADC,    BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    E(0x10, TEXT_ADC,    BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E(0x13, TEXT_ADC,    WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    E(0x11, TEXT_ADC,    WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    E(0x14, TEXT_ADC,    BYTE, M_AL,   M_IMM,  P_NONE, P_OPR),
    E(0x15, TEXT_ADC,    WORD, M_AX,   M_IMM,  P_NONE, P_OPR),
    E(0x1A, TEXT_SBB,    BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    E(0x18, TEXT_SBB,    BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E(0x1B, TEXT_SBB,    WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    E(0x19, TEXT_SBB,    WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    E(0x1C, TEXT_SBB,    BYTE, M_AL,   M_IMM,  P_NONE, P_OPR),
    E(0x1D, TEXT_SBB,    WORD, M_AX,   M_IMM,  P_NONE, P_OPR),
    E(0x22, TEXT_AND,    BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    E(0x20, TEXT_AND,    BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E(0x23, TEXT_AND,    WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    E(0x21, TEXT_AND,    WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    E(0x24, TEXT_AND,    BYTE, M_AL,   M_IMM,  P_NONE, P_OPR),
    E(0x25, TEXT_AND,    WORD, M_AX,   M_IMM,  P_NONE, P_OPR),
    E(0x26, TEXT_SEGES,  BYTE, M_NONE, M_NONE, P_NONE, P_NONE),
    E(0x27, TEXT_DAA,    BYTE, M_NONE, M_NONE, P_NONE, P_NONE),
    E(0x2A, TEXT_SUB,    BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    E(0x28, TEXT_SUB,    BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E(0x2B, TEXT_SUB,    WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    E(0x29, TEXT_SUB,    WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    E(0x2C, TEXT_SUB,    BYTE, M_AL,   M_IMM,  P_NONE, P_OPR),
    E(0x2D, TEXT_SUB,    WORD, M_AX,   M_IMM,  P_NONE, P_OPR),
    E(0x2E, TEXT_SEGCS,  BYTE, M_NONE, M_NONE, P_NONE, P_NONE),
    E(0x2F, TEXT_DAS,    BYTE, M_NONE, M_NONE, P_NONE, P_NONE),
    E(0x32, TEXT_XOR,    BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    E(0x30, TEXT_XOR,    BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E(0x33, TEXT_XOR,    WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    E(0x31, TEXT_XOR,    WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    E(0x34, TEXT_XOR,    BYTE, M_AL,   M_IMM,  P_NONE, P_OPR),
    E(0x35, TEXT_XOR,    WORD, M_AX,   M_IMM,  P_NONE, P_OPR),
    E(0x36, TEXT_SEGSS,  BYTE, M_NONE, M_NONE, P_NONE, P_NONE),
    E(0x37, TEXT_AAA,    BYTE, M_NONE, M_NONE, P_NONE, P_NONE),
    E(0x3A, TEXT_CMP,    BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    E(0x38, TEXT_CMP,    BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E(0x3B, TEXT_CMP,    WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    E(0x39, TEXT_CMP,    WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    E(0x3C, TEXT_CMP,    BYTE, M_AL,   M_IMM,  P_NONE, P_OPR),
    E(0x3D, TEXT_CMP,    WORD, M_AX,   M_IMM,  P_NONE, P_OPR),
    E(0x3E, TEXT_SEGDS,  BYTE, M_NONE, M_NONE, P_NONE, P_NONE),
    E(0x3F, TEXT_AAS,    BYTE, M_NONE, M_NONE, P_NONE, P_NONE),
    E(0x40, TEXT_INC,    WORD, M_WREG, M_NONE, P_OREG, P_NONE),
    E(0x48, TEXT_DEC,    WORD, M_WREG, M_NONE, P_OREG, P_NONE),
    E(0x50, TEXT_PUSH,   WORD, M_WREG, M_NONE, P_OREG, P_NONE),
    E(0x58, TEXT_POP,    WORD, M_WREG, M_NONE, P_OREG, P_NONE),
    E(0x70, TEXT_JO,     NONE, M_REL8, M_NONE, P_OPR,  P_NONE),
    E(0x71, TEXT_JNO,    NONE, M_REL8, M_NONE, P_OPR,  P_NONE),
    E(0x72, TEXT_JB,     NONE, M_REL8, M_NONE, P_OPR,  P_NONE),
    E(0x72, TEXT_JC,     NONE, M_REL8, M_NONE, P_OPR,  P_NONE),
    E(0x72, TEXT_JNAE,   NONE, M_REL8, M_NONE, P_OPR,  P_NONE),
    E(0x73, TEXT_JAE,    NONE, M_REL8, M_NONE, P_OPR,  P_NONE),
    E(0x73, TEXT_JNB,    NONE, M_REL8, M_NONE, P_OPR,  P_NONE),
    E(0x73, TEXT_JNC,    NONE, M_REL8, M_NONE, P_OPR,  P_NONE),
    E(0x74, TEXT_JE,     NONE, M_REL8, M_NONE, P_OPR,  P_NONE),
    E(0x74, TEXT_JZ,     NONE, M_REL8, M_NONE, P_OPR,  P_NONE),
    E(0x75, TEXT_JNE,    NONE, M_REL8, M_NONE, P_OPR,  P_NONE),
    E(0x75, TEXT_JNZ,    NONE, M_REL8, M_NONE, P_OPR,  P_NONE),
    E(0x76, TEXT_JBE,    NONE, M_REL8, M_NONE, P_OPR,  P_NONE),
    E(0x76, TEXT_JNA,    NONE, M_REL8, M_NONE, P_OPR,  P_NONE),
    E(0x77, TEXT_JA,     NONE, M_REL8, M_NONE, P_OPR,  P_NONE),
    E(0x77, TEXT_JNBE,   NONE, M_REL8, M_NONE, P_OPR,  P_NONE),
    E(0x78, TEXT_JS,     NONE, M_REL8, M_NONE, P_OPR,  P_NONE),
    E(0x79, TEXT_JNS,    NONE, M_REL8, M_NONE, P_OPR,  P_NONE),
    E(0x7A, TEXT_JPE,    NONE, M_REL8, M_NONE, P_OPR,  P_NONE),
    E(0x7A, TEXT_JP,     NONE, M_REL8, M_NONE, P_OPR,  P_NONE),
    E(0x7B, TEXT_JPO,    NONE, M_REL8, M_NONE, P_OPR,  P_NONE),
    E(0x7B, TEXT_JNP,    NONE, M_REL8, M_NONE, P_OPR,  P_NONE),
    E(0x7C, TEXT_JL,     NONE, M_REL8, M_NONE, P_OPR,  P_NONE),
    E(0x7C, TEXT_JNGE,   NONE, M_REL8, M_NONE, P_OPR,  P_NONE),
    E(0x7D, TEXT_JGE,    NONE, M_REL8, M_NONE, P_OPR,  P_NONE),
    E(0x7D, TEXT_JNL,    NONE, M_REL8, M_NONE, P_OPR,  P_NONE),
    E(0x7E, TEXT_JLE,    NONE, M_REL8, M_NONE, P_OPR,  P_NONE),
    E(0x7E, TEXT_JNG,    NONE, M_REL8, M_NONE, P_OPR,  P_NONE),
    E(0x7F, TEXT_JG,     NONE, M_REL8, M_NONE, P_OPR,  P_NONE),
    E(0x7F, TEXT_JNLE,   NONE, M_REL8, M_NONE, P_OPR,  P_NONE),
    E(0x84, TEXT_TEST,   BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    E(0x84, TEXT_TEST,   BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E(0x85, TEXT_TEST,   WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    E(0x85, TEXT_TEST,   WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    E(0x86, TEXT_XCHG,   BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    E(0x86, TEXT_XCHG,   BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E(0x87, TEXT_XCHG,   WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    E(0x87, TEXT_XCHG,   WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    E(0x8A, TEXT_MOV,    BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    E(0x88, TEXT_MOV,    BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E(0x8B, TEXT_MOV,    WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    E(0x89, TEXT_MOV,    WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    E(0x8C, TEXT_MOV,    WORD, M_WMOD, M_SREG, P_MOD,  P_REG),
    E(0x8D, TEXT_LEA,    WORD, M_WREG, M_BMEM, P_REG,  P_MOD),
    E(0x8D, TEXT_LEA,    WORD, M_WREG, M_WMEM, P_REG,  P_MOD),
    E(0x8E, TEXT_MOV,    WORD, M_SREG, M_WMOD, P_REG,  P_MOD),
    E(0x98, TEXT_CBW,    BYTE, M_NONE, M_NONE, P_NONE, P_NONE),
    E(0x99, TEXT_CWD,    WORD, M_NONE, M_NONE, P_NONE, P_NONE),
    E(0x9A, TEXT_CALLF,  NONE, M_FAR,  M_NONE, P_OPR,  P_NONE),
    E(0x9B, TEXT_WAIT,   NONE, M_NONE, M_NONE, P_NONE, P_NONE),
    E(0x9C, TEXT_PUSHF,  WORD, M_NONE, M_NONE, P_NONE, P_NONE),
    E(0x9D, TEXT_POPF,   WORD, M_NONE, M_NONE, P_NONE, P_NONE),
    E(0x9E, TEXT_SAHF,   BYTE, M_NONE, M_NONE, P_NONE, P_NONE),
    E(0x9F, TEXT_LAHF,   BYTE, M_NONE, M_NONE, P_NONE, P_NONE),
    S(0xA4, TEXT_MOVSB,  BYTE, M_NONE, M_NONE, P_NONE, P_NONE),
    S(0xA5, TEXT_MOVSW,  WORD, M_NONE, M_NONE, P_NONE, P_NONE),
    S(0xA4, TEXT_MOVSB,  BYTE, M_BMEM, M_BMEM, P_NONE, P_NONE),
    S(0xA5, TEXT_MOVSW,  WORD, M_WMEM, M_WMEM, P_NONE, P_NONE),
    S(0xA4, TEXT_MOVS,   BYTE, M_BMEM, M_BMEM, P_NONE, P_NONE),
    S(0xA5, TEXT_MOVS,   WORD, M_WMEM, M_WMEM, P_NONE, P_NONE),
    S(0xA6, TEXT_CMPSB,  BYTE, M_NONE, M_NONE, P_NONE, P_NONE),
    S(0xA7, TEXT_CMPSW,  WORD, M_NONE, M_NONE, P_NONE, P_NONE),
    S(0xA6, TEXT_CMPSB,  BYTE, M_BMEM, M_BMEM, P_NONE, P_NONE),
    S(0xA7, TEXT_CMPSW,  WORD, M_WMEM, M_WMEM, P_NONE, P_NONE),
    S(0xA6, TEXT_CMPS,   BYTE, M_BMEM, M_BMEM, P_NONE, P_NONE),
    S(0xA7, TEXT_CMPS,   WORD, M_WMEM, M_WMEM, P_NONE, P_NONE),
    E(0xA8, TEXT_TEST,   BYTE, M_AL,   M_IMM,  P_NONE, P_OPR),
    E(0xA9, TEXT_TEST,   WORD, M_AX,   M_IMM,  P_NONE, P_OPR),
    S(0xAA, TEXT_STOSB,  BYTE, M_NONE, M_NONE, P_NONE, P_NONE),
    S(0xAB, TEXT_STOSW,  WORD, M_NONE, M_NONE, P_NONE, P_NONE),
    S(0xAA, TEXT_STOSB,  NONE, M_BMEM, M_NONE, P_NONE, P_NONE),
    S(0xAB, TEXT_STOSW,  NONE, M_WMEM, M_NONE, P_NONE, P_NONE),
    S(0xAA, TEXT_STOS,   BYTE, M_BMEM, M_NONE, P_NONE, P_NONE),
    S(0xAB, TEXT_STOS,   WORD, M_WMEM, M_NONE, P_NONE, P_NONE),
    S(0xAC, TEXT_LODSB,  BYTE, M_NONE, M_NONE, P_NONE, P_NONE),
    S(0xAD, TEXT_LODSW,  WORD, M_NONE, M_NONE, P_NONE, P_NONE),
    S(0xAC, TEXT_LODSB,  NONE, M_BMEM, M_NONE, P_NONE, P_NONE),
    S(0xAD, TEXT_LODSW,  NONE, M_WMEM, M_NONE, P_NONE, P_NONE),
    S(0xAC, TEXT_LODS,   BYTE, M_BMEM, M_NONE, P_NONE, P_NONE),
    S(0xAD, TEXT_LODS,   WORD, M_WMEM, M_NONE, P_NONE, P_NONE),
    S(0xAE, TEXT_SCASB,  BYTE, M_NONE, M_NONE, P_NONE, P_NONE),
    S(0xAF, TEXT_SCASW,  WORD, M_NONE, M_NONE, P_NONE, P_NONE),
    S(0xAE, TEXT_SCASB,  NONE, M_BMEM, M_NONE, P_NONE, P_NONE),
    S(0xAF, TEXT_SCASW,  NONE, M_WMEM, M_NONE, P_NONE, P_NONE),
    S(0xAE, TEXT_SCAS,   BYTE, M_BMEM, M_NONE, P_NONE, P_NONE),
    S(0xAF, TEXT_SCAS,   WORD, M_WMEM, M_NONE, P_NONE, P_NONE),
    E(0xB0, TEXT_MOV,    BYTE, M_BREG, M_IMM,  P_OREG, P_OPR),
    E(0xB8, TEXT_MOV,    WORD, M_WREG, M_IMM,  P_OREG, P_OPR),
    E(0xC2, TEXT_RET,    WORD, M_UI16, M_NONE, P_OPR,  P_NONE),
    E(0xC3, TEXT_RET,    NONE, M_NONE, M_NONE, P_NONE, P_NONE),
    E(0xC4, TEXT_LES,    WORD, M_WREG, M_WMEM, P_REG,  P_MOD),
    E(0xC5, TEXT_LDS,    WORD, M_WREG, M_WMEM, P_REG,  P_MOD),
    E(0xCA, TEXT_RETF,   WORD, M_UI16, M_NONE, P_OPR,  P_NONE),
    E(0xCB, TEXT_RETF,   NONE, M_NONE, M_NONE, P_NONE, P_NONE),
    E(0xCC, TEXT_INT,    NONE, M_VAL3, M_NONE, P_NONE, P_NONE),
    E(0xCD, TEXT_INT,    BYTE, M_IMM,  M_NONE, P_OPR,  P_NONE),
    E(0xCE, TEXT_INTO,   NONE, M_NONE, M_NONE, P_NONE, P_NONE),
    E(0xCF, TEXT_IRET,   NONE, M_NONE, M_NONE, P_NONE, P_NONE),
    E(0xD7, TEXT_XLAT,   NONE, M_NONE, M_NONE, P_NONE, P_NONE),
    E(0xE0, TEXT_LOOPNE, NONE, M_REL8, M_NONE, P_OPR,  P_NONE),
    E(0xE0, TEXT_LOOPNZ, NONE, M_REL8, M_NONE, P_OPR,  P_NONE),
    E(0xE1, TEXT_LOOPE,  NONE, M_REL8, M_NONE, P_OPR,  P_NONE),
    E(0xE1, TEXT_LOOPZ,  NONE, M_REL8, M_NONE, P_OPR,  P_NONE),
    E(0xE2, TEXT_LOOP,   NONE, M_REL8, M_NONE, P_OPR,  P_NONE),
    E(0xE3, TEXT_JCXZ,   NONE, M_REL8, M_NONE, P_OPR,  P_NONE),
    E(0xE4, TEXT_IN,     BYTE, M_AL,   M_IOA,  P_NONE, P_OPR),
    E(0xE5, TEXT_IN,     WORD, M_AX,   M_IOA,  P_NONE, P_OPR),
    E(0xE6, TEXT_OUT,    BYTE, M_IOA,  M_AL,   P_OPR,  P_NONE),
    E(0xE7, TEXT_OUT,    WORD, M_IOA,  M_AX,   P_OPR,  P_NONE),
    E(0xE8, TEXT_CALL,   NONE, M_REL,  M_NONE, P_OPR,  P_NONE),
    E(0xEB, TEXT_JMP,    NONE, M_REL8, M_NONE, P_OPR,  P_NONE),
    E(0xE9, TEXT_JMP,    NONE, M_REL,  M_NONE, P_OPR,  P_NONE),
    E(0xEA, TEXT_JMPF,   NONE, M_FAR,  M_NONE, P_OPR,  P_NONE),
    E(0xEC, TEXT_IN,     BYTE, M_AL,   M_DX,   P_NONE, P_NONE),
    E(0xED, TEXT_IN,     WORD, M_AX,   M_DX,   P_NONE, P_NONE),
    E(0xEE, TEXT_OUT,    BYTE, M_DX,   M_AL,   P_NONE, P_NONE),
    E(0xEF, TEXT_OUT,    WORD, M_DX,   M_AX,   P_NONE, P_NONE),
    E(0xF0, TEXT_LOCK,   NONE, M_NONE, M_NONE, P_NONE, P_NONE),
    E(0xF2, TEXT_REPNE,  NONE, M_ISTR, M_NONE, P_OPR,  P_NONE),
    E(0xF2, TEXT_REPNZ,  NONE, M_ISTR, M_NONE, P_OPR,  P_NONE),
    E(0xF3, TEXT_REP,    NONE, M_ISTR, M_NONE, P_OPR,  P_NONE),
    E(0xF3, TEXT_REPE,   NONE, M_ISTR, M_NONE, P_OPR,  P_NONE),
    E(0xF3, TEXT_REPZ,   NONE, M_ISTR, M_NONE, P_OPR,  P_NONE),
    E(0xF2, TEXT_REPNE,  NONE, M_NONE, M_NONE, P_NONE, P_NONE),
    E(0xF2, TEXT_REPNZ,  NONE, M_NONE, M_NONE, P_NONE, P_NONE),
    E(0xF3, TEXT_REP,    NONE, M_NONE, M_NONE, P_NONE, P_NONE),
    E(0xF3, TEXT_REPE,   NONE, M_NONE, M_NONE, P_NONE, P_NONE),
    E(0xF3, TEXT_REPZ,   NONE, M_NONE, M_NONE, P_NONE, P_NONE),
    E(0xF4, TEXT_HLT,    NONE, M_NONE, M_NONE, P_NONE, P_NONE),
    E(0xF5, TEXT_CMC,    NONE, M_NONE, M_NONE, P_NONE, P_NONE),
    E(0xF8, TEXT_CLC,    NONE, M_NONE, M_NONE, P_NONE, P_NONE),
    E(0xF9, TEXT_STC,    NONE, M_NONE, M_NONE, P_NONE, P_NONE),
    E(0xFA, TEXT_CLI,    NONE, M_NONE, M_NONE, P_NONE, P_NONE),
    E(0xFB, TEXT_STI,    NONE, M_NONE, M_NONE, P_NONE, P_NONE),
    E(0xFC, TEXT_CLD,    NONE, M_NONE, M_NONE, P_NONE, P_NONE),
    E(0xFD, TEXT_STD,    NONE, M_NONE, M_NONE, P_NONE, P_NONE),
};

static const uint8_t INDEX_00[] PROGMEM = {
     57,  // TEXT_AAA
     65,  // TEXT_AAS
     22,  // TEXT_ADC
     23,  // TEXT_ADC
     24,  // TEXT_ADC
     25,  // TEXT_ADC
     26,  // TEXT_ADC
     27,  // TEXT_ADC
      7,  // TEXT_ADD
      8,  // TEXT_ADD
     11,  // TEXT_ADD
     12,  // TEXT_ADD
     13,  // TEXT_ADD
     14,  // TEXT_ADD
     34,  // TEXT_AND
     35,  // TEXT_AND
     36,  // TEXT_AND
     37,  // TEXT_AND
     38,  // TEXT_AND
     39,  // TEXT_AND
    179,  // TEXT_CALL
    118,  // TEXT_CALLF
    116,  // TEXT_CBW
    200,  // TEXT_CLC
    204,  // TEXT_CLD
    202,  // TEXT_CLI
    199,  // TEXT_CMC
     58,  // TEXT_CMP
     59,  // TEXT_CMP
     60,  // TEXT_CMP
     61,  // TEXT_CMP
     62,  // TEXT_CMP
     63,  // TEXT_CMP
    134,  // TEXT_CMPS
    135,  // TEXT_CMPS
    130,  // TEXT_CMPSB
    132,  // TEXT_CMPSB
    131,  // TEXT_CMPSW
    133,  // TEXT_CMPSW
    117,  // TEXT_CWD
     41,  // TEXT_DAA
     49,  // TEXT_DAS
     67,  // TEXT_DEC
    198,  // TEXT_HLT
    175,  // TEXT_IN
    176,  // TEXT_IN
    183,  // TEXT_IN
    184,  // TEXT_IN
     66,  // TEXT_INC
    164,  // TEXT_INT
    165,  // TEXT_INT
    166,  // TEXT_INTO
    167,  // TEXT_IRET
     84,  // TEXT_JA
     75,  // TEXT_JAE
     72,  // TEXT_JB
     82,  // TEXT_JBE
     73,  // TEXT_JC
    174,  // TEXT_JCXZ
     78,  // TEXT_JE
     98,  // TEXT_JG
     94,  // TEXT_JGE
     92,  // TEXT_JL
     96,  // TEXT_JLE
    180,  // TEXT_JMP
    181,  // TEXT_JMP
    182,  // TEXT_JMPF
     83,  // TEXT_JNA
     74,  // TEXT_JNAE
     76,  // TEXT_JNB
     85,  // TEXT_JNBE
     77,  // TEXT_JNC
     80,  // TEXT_JNE
     97,  // TEXT_JNG
     93,  // TEXT_JNGE
     95,  // TEXT_JNL
     99,  // TEXT_JNLE
     71,  // TEXT_JNO
     91,  // TEXT_JNP
     87,  // TEXT_JNS
     81,  // TEXT_JNZ
     70,  // TEXT_JO
     89,  // TEXT_JP
     88,  // TEXT_JPE
     90,  // TEXT_JPO
     86,  // TEXT_JS
     79,  // TEXT_JZ
    123,  // TEXT_LAHF
    161,  // TEXT_LDS
    113,  // TEXT_LEA
    114,  // TEXT_LEA
    160,  // TEXT_LES
    187,  // TEXT_LOCK
    148,  // TEXT_LODS
    149,  // TEXT_LODS
    144,  // TEXT_LODSB
    146,  // TEXT_LODSB
    145,  // TEXT_LODSW
    147,  // TEXT_LODSW
    173,  // TEXT_LOOP
    171,  // TEXT_LOOPE
    169,  // TEXT_LOOPNE
    170,  // TEXT_LOOPNZ
    172,  // TEXT_LOOPZ
      0,  // TEXT_MOV
      1,  // TEXT_MOV
      2,  // TEXT_MOV
      3,  // TEXT_MOV
    108,  // TEXT_MOV
    109,  // TEXT_MOV
    110,  // TEXT_MOV
    111,  // TEXT_MOV
    112,  // TEXT_MOV
    115,  // TEXT_MOV
    156,  // TEXT_MOV
    157,  // TEXT_MOV
    128,  // TEXT_MOVS
    129,  // TEXT_MOVS
    124,  // TEXT_MOVSB
    126,  // TEXT_MOVSB
    125,  // TEXT_MOVSW
    127,  // TEXT_MOVSW
      4,  // TEXT_NOP
      9,  // TEXT_OR
     10,  // TEXT_OR
     18,  // TEXT_OR
     19,  // TEXT_OR
     20,  // TEXT_OR
     21,  // TEXT_OR
    177,  // TEXT_OUT
    178,  // TEXT_OUT
    185,  // TEXT_OUT
    186,  // TEXT_OUT
     16,  // TEXT_POP
     17,  // TEXT_POP
     69,  // TEXT_POP
    121,  // TEXT_POPF
     15,  // TEXT_PUSH
     68,  // TEXT_PUSH
    120,  // TEXT_PUSHF
    190,  // TEXT_REP
    195,  // TEXT_REP
    191,  // TEXT_REPE
    196,  // TEXT_REPE
    188,  // TEXT_REPNE
    193,  // TEXT_REPNE
    189,  // TEXT_REPNZ
    194,  // TEXT_REPNZ
    192,  // TEXT_REPZ
    197,  // TEXT_REPZ
    158,  // TEXT_RET
    159,  // TEXT_RET
    162,  // TEXT_RETF
    163,  // TEXT_RETF
    122,  // TEXT_SAHF
     28,  // TEXT_SBB
     29,  // TEXT_SBB
     30,  // TEXT_SBB
     31,  // TEXT_SBB
     32,  // TEXT_SBB
     33,  // TEXT_SBB
    154,  // TEXT_SCAS
    155,  // TEXT_SCAS
    150,  // TEXT_SCASB
    152,  // TEXT_SCASB
    151,  // TEXT_SCASW
    153,  // TEXT_SCASW
     48,  // TEXT_SEGCS
     64,  // TEXT_SEGDS
     40,  // TEXT_SEGES
     56,  // TEXT_SEGSS
    201,  // TEXT_STC
    205,  // TEXT_STD
    203,  // TEXT_STI
    142,  // TEXT_STOS
    143,  // TEXT_STOS
    138,  // TEXT_STOSB
    140,  // TEXT_STOSB
    139,  // TEXT_STOSW
    141,  // TEXT_STOSW
     42,  // TEXT_SUB
     43,  // TEXT_SUB
     44,  // TEXT_SUB
     45,  // TEXT_SUB
     46,  // TEXT_SUB
     47,  // TEXT_SUB
    100,  // TEXT_TEST
    101,  // TEXT_TEST
    102,  // TEXT_TEST
    103,  // TEXT_TEST
    136,  // TEXT_TEST
    137,  // TEXT_TEST
    119,  // TEXT_WAIT
      5,  // TEXT_XCHG
      6,  // TEXT_XCHG
    104,  // TEXT_XCHG
    105,  // TEXT_XCHG
    106,  // TEXT_XCHG
    107,  // TEXT_XCHG
    168,  // TEXT_XLAT
     50,  // TEXT_XOR
     51,  // TEXT_XOR
     52,  // TEXT_XOR
     53,  // TEXT_XOR
     54,  // TEXT_XOR
     55,  // TEXT_XOR
};

static const Entry TABLE_80[] PROGMEM = {
    E(000, TEXT_ADD, BYTE, M_BMOD, M_IMM, P_OMOD, P_OPR),
    E(010, TEXT_OR,  BYTE, M_BMOD, M_IMM, P_OMOD, P_OPR),
    E(020, TEXT_ADC, BYTE, M_BMOD, M_IMM, P_OMOD, P_OPR),
    E(030, TEXT_SBB, BYTE, M_BMOD, M_IMM, P_OMOD, P_OPR),
    E(040, TEXT_AND, BYTE, M_BMOD, M_IMM, P_OMOD, P_OPR),
    E(050, TEXT_SUB, BYTE, M_BMOD, M_IMM, P_OMOD, P_OPR),
    E(060, TEXT_XOR, BYTE, M_BMOD, M_IMM, P_OMOD, P_OPR),
    E(070, TEXT_CMP, BYTE, M_BMOD, M_IMM, P_OMOD, P_OPR),
};

static const uint8_t INDEX_80[] PROGMEM = {
      2,  // TEXT_ADC
      0,  // TEXT_ADD
      4,  // TEXT_AND
      7,  // TEXT_CMP
      1,  // TEXT_OR
      3,  // TEXT_SBB
      5,  // TEXT_SUB
      6,  // TEXT_XOR
};

static const Entry TABLE_81[] PROGMEM = {
    E(000, TEXT_ADD, WORD, M_WMOD, M_IMM, P_OMOD, P_OPR),
    E(010, TEXT_OR,  WORD, M_WMOD, M_IMM, P_OMOD, P_OPR),
    E(020, TEXT_ADC, WORD, M_WMOD, M_IMM, P_OMOD, P_OPR),
    E(030, TEXT_SBB, WORD, M_WMOD, M_IMM, P_OMOD, P_OPR),
    E(040, TEXT_AND, WORD, M_WMOD, M_IMM, P_OMOD, P_OPR),
    E(050, TEXT_SUB, WORD, M_WMOD, M_IMM, P_OMOD, P_OPR),
    E(060, TEXT_XOR, WORD, M_WMOD, M_IMM, P_OMOD, P_OPR),
    E(070, TEXT_CMP, WORD, M_WMOD, M_IMM, P_OMOD, P_OPR),
};

static const uint8_t INDEX_81[] PROGMEM = {
      2,  // TEXT_ADC
      0,  // TEXT_ADD
      4,  // TEXT_AND
      7,  // TEXT_CMP
      1,  // TEXT_OR
      3,  // TEXT_SBB
      5,  // TEXT_SUB
      6,  // TEXT_XOR
};

static const Entry TABLE_83[] PROGMEM = {
    E(000, TEXT_ADD, WORD, M_WMOD, M_IMM8, P_OMOD, P_OPR),
    E(010, TEXT_OR,  WORD, M_WMOD, M_IMM8, P_OMOD, P_OPR),
    E(020, TEXT_ADC, WORD, M_WMOD, M_IMM8, P_OMOD, P_OPR),
    E(030, TEXT_SBB, WORD, M_WMOD, M_IMM8, P_OMOD, P_OPR),
    E(040, TEXT_AND, WORD, M_WMOD, M_IMM8, P_OMOD, P_OPR),
    E(050, TEXT_SUB, WORD, M_WMOD, M_IMM8, P_OMOD, P_OPR),
    E(060, TEXT_XOR, WORD, M_WMOD, M_IMM8, P_OMOD, P_OPR),
    E(070, TEXT_CMP, WORD, M_WMOD, M_IMM8, P_OMOD, P_OPR),
};

static const uint8_t INDEX_83[] PROGMEM = {
      2,  // TEXT_ADC
      0,  // TEXT_ADD
      4,  // TEXT_AND
      7,  // TEXT_CMP
      1,  // TEXT_OR
      3,  // TEXT_SBB
      5,  // TEXT_SUB
      6,  // TEXT_XOR
};

static const Entry TABLE_8F[] PROGMEM = {
    E(000, TEXT_POP, NONE, M_WMOD, M_NONE, P_OMOD, P_NONE),
};

static const uint8_t INDEX_8F[] PROGMEM = {
      0,  // TEXT_POP
};

static const Entry TABLE_C6[] PROGMEM = {
    E(000, TEXT_MOV, BYTE, M_BMOD, M_IMM, P_OMOD, P_OPR),
};

static const uint8_t INDEX_C6[] PROGMEM = {
      0,  // TEXT_MOV
};

static const Entry TABLE_C7[] PROGMEM = {
    E(000, TEXT_MOV, WORD, M_WMOD, M_IMM, P_OMOD, P_OPR),
};

static const uint8_t INDEX_C7[] PROGMEM = {
      0,  // TEXT_MOV
};

static const Entry TABLE_D0[] PROGMEM = {
    E(000, TEXT_ROL, BYTE, M_BMOD, M_VAL1, P_OMOD, P_NONE),
    E(010, TEXT_ROR, BYTE, M_BMOD, M_VAL1, P_OMOD, P_NONE),
    E(020, TEXT_RCL, BYTE, M_BMOD, M_VAL1, P_OMOD, P_NONE),
    E(030, TEXT_RCR, BYTE, M_BMOD, M_VAL1, P_OMOD, P_NONE),
    E(040, TEXT_SHL, BYTE, M_BMOD, M_VAL1, P_OMOD, P_NONE),
    E(040, TEXT_SAL, BYTE, M_BMOD, M_VAL1, P_OMOD, P_NONE),
    E(050, TEXT_SHR, BYTE, M_BMOD, M_VAL1, P_OMOD, P_NONE),
    E(070, TEXT_SAR, BYTE, M_BMOD, M_VAL1, P_OMOD, P_NONE),
};

static const uint8_t INDEX_D0[] PROGMEM = {
      2,  // TEXT_RCL
      3,  // TEXT_RCR
      0,  // TEXT_ROL
      1,  // TEXT_ROR
      5,  // TEXT_SAL
      7,  // TEXT_SAR
      4,  // TEXT_SHL
      6,  // TEXT_SHR
};

static const Entry TABLE_D1[] PROGMEM = {
    E(000, TEXT_ROL, WORD, M_WMOD, M_VAL1, P_OMOD, P_NONE),
    E(010, TEXT_ROR, WORD, M_WMOD, M_VAL1, P_OMOD, P_NONE),
    E(020, TEXT_RCL, WORD, M_WMOD, M_VAL1, P_OMOD, P_NONE),
    E(030, TEXT_RCR, WORD, M_WMOD, M_VAL1, P_OMOD, P_NONE),
    E(040, TEXT_SHL, WORD, M_WMOD, M_VAL1, P_OMOD, P_NONE),
    E(040, TEXT_SAL, WORD, M_WMOD, M_VAL1, P_OMOD, P_NONE),
    E(050, TEXT_SHR, WORD, M_WMOD, M_VAL1, P_OMOD, P_NONE),
    E(070, TEXT_SAR, WORD, M_WMOD, M_VAL1, P_OMOD, P_NONE),
};

static const uint8_t INDEX_D1[] PROGMEM = {
      2,  // TEXT_RCL
      3,  // TEXT_RCR
      0,  // TEXT_ROL
      1,  // TEXT_ROR
      5,  // TEXT_SAL
      7,  // TEXT_SAR
      4,  // TEXT_SHL
      6,  // TEXT_SHR
};

static const Entry TABLE_D2[] PROGMEM = {
    E(000, TEXT_ROL, BYTE, M_BMOD, M_CL, P_OMOD, P_NONE),
    E(010, TEXT_ROR, BYTE, M_BMOD, M_CL, P_OMOD, P_NONE),
    E(020, TEXT_RCL, BYTE, M_BMOD, M_CL, P_OMOD, P_NONE),
    E(030, TEXT_RCR, BYTE, M_BMOD, M_CL, P_OMOD, P_NONE),
    E(040, TEXT_SHL, BYTE, M_BMOD, M_CL, P_OMOD, P_NONE),
    E(040, TEXT_SAL, BYTE, M_BMOD, M_CL, P_OMOD, P_NONE),
    E(050, TEXT_SHR, BYTE, M_BMOD, M_CL, P_OMOD, P_NONE),
    E(070, TEXT_SAR, BYTE, M_BMOD, M_CL, P_OMOD, P_NONE),
};

static const uint8_t INDEX_D2[] PROGMEM = {
      2,  // TEXT_RCL
      3,  // TEXT_RCR
      0,  // TEXT_ROL
      1,  // TEXT_ROR
      5,  // TEXT_SAL
      7,  // TEXT_SAR
      4,  // TEXT_SHL
      6,  // TEXT_SHR
};

static const Entry TABLE_D3[] PROGMEM = {
    E(000, TEXT_ROL, WORD, M_WMOD, M_CL, P_OMOD, P_NONE),
    E(010, TEXT_ROR, WORD, M_WMOD, M_CL, P_OMOD, P_NONE),
    E(020, TEXT_RCL, WORD, M_WMOD, M_CL, P_OMOD, P_NONE),
    E(030, TEXT_RCR, WORD, M_WMOD, M_CL, P_OMOD, P_NONE),
    E(040, TEXT_SHL, WORD, M_WMOD, M_CL, P_OMOD, P_NONE),
    E(040, TEXT_SAL, WORD, M_WMOD, M_CL, P_OMOD, P_NONE),
    E(050, TEXT_SHR, WORD, M_WMOD, M_CL, P_OMOD, P_NONE),
    E(070, TEXT_SAR, WORD, M_WMOD, M_CL, P_OMOD, P_NONE),
};

static const uint8_t INDEX_D3[] PROGMEM = {
      2,  // TEXT_RCL
      3,  // TEXT_RCR
      0,  // TEXT_ROL
      1,  // TEXT_ROR
      5,  // TEXT_SAL
      7,  // TEXT_SAR
      4,  // TEXT_SHL
      6,  // TEXT_SHR
};

static const Entry TABLE_D4[] PROGMEM = {
    E(0x0A, TEXT_AAM, BYTE, M_NONE, M_NONE, P_NONE, P_NONE),
};

static const uint8_t INDEX_D4[] PROGMEM = {
      0,  // TEXT_AAM
};

static const Entry TABLE_D5[] PROGMEM = {
    E(0x0A, TEXT_AAD, BYTE, M_NONE, M_NONE, P_NONE, P_NONE),
};

static const uint8_t INDEX_D5[] PROGMEM = {
      0,  // TEXT_AAD
};

static const Entry TABLE_F6[] PROGMEM = {
    E(000, TEXT_TEST, BYTE, M_BMOD, M_IMM, P_OMOD, P_OPR),
    E(020, TEXT_NOT,  BYTE, M_BMOD, M_NONE, P_OMOD, P_NONE),
    E(030, TEXT_NEG,  BYTE, M_BMOD, M_NONE, P_OMOD, P_NONE),
    E(040, TEXT_MUL,  BYTE, M_BMOD, M_NONE, P_OMOD, P_NONE),
    E(050, TEXT_IMUL, BYTE, M_BMOD, M_NONE, P_OMOD, P_NONE),
    E(060, TEXT_DIV,  BYTE, M_BMOD, M_NONE, P_OMOD, P_NONE),
    E(070, TEXT_IDIV, BYTE, M_BMOD, M_NONE, P_OMOD, P_NONE),
};

static const uint8_t INDEX_F6[] PROGMEM = {
      5,  // TEXT_DIV
      6,  // TEXT_IDIV
      4,  // TEXT_IMUL
      3,  // TEXT_MUL
      2,  // TEXT_NEG
      1,  // TEXT_NOT
      0,  // TEXT_TEST
};

static const Entry TABLE_F7[] PROGMEM = {
    E(000, TEXT_TEST, WORD, M_WMOD, M_IMM, P_OMOD, P_OPR),
    E(020, TEXT_NOT,  WORD, M_WMOD, M_NONE, P_OMOD, P_NONE),
    E(030, TEXT_NEG,  WORD, M_WMOD, M_NONE, P_OMOD, P_NONE),
    E(040, TEXT_MUL,  WORD, M_WMOD, M_NONE, P_OMOD, P_NONE),
    E(050, TEXT_IMUL, WORD, M_WMOD, M_NONE, P_OMOD, P_NONE),
    E(060, TEXT_DIV,  WORD, M_WMOD, M_NONE, P_OMOD, P_NONE),
    E(070, TEXT_IDIV, WORD, M_WMOD, M_NONE, P_OMOD, P_NONE),
};

static const uint8_t INDEX_F7[] PROGMEM = {
      5,  // TEXT_DIV
      6,  // TEXT_IDIV
      4,  // TEXT_IMUL
      3,  // TEXT_MUL
      2,  // TEXT_NEG
      1,  // TEXT_NOT
      0,  // TEXT_TEST
};

static const Entry TABLE_FE[] PROGMEM = {
    E(000, TEXT_INC, BYTE, M_BMOD, M_NONE, P_OMOD, P_NONE),
    E(010, TEXT_DEC, BYTE, M_BMOD, M_NONE, P_OMOD, P_NONE),
};

static const uint8_t INDEX_FE[] PROGMEM = {
      1,  // TEXT_DEC
      0,  // TEXT_INC
};

static const Entry TABLE_FF[] PROGMEM = {
    E(000, TEXT_INC,   WORD, M_WMOD, M_NONE, P_OMOD, P_NONE),
    E(010, TEXT_DEC,   WORD, M_WMOD, M_NONE, P_OMOD, P_NONE),
    E(020, TEXT_CALL,  NONE, M_WMOD, M_NONE, P_OMOD, P_NONE),
    E(030, TEXT_CALLF, NONE, M_WMEM, M_NONE, P_OMOD, P_NONE),
    E(040, TEXT_JMP,   NONE, M_WMOD, M_NONE, P_OMOD, P_NONE),
    E(050, TEXT_JMPF,  NONE, M_WMEM, M_NONE, P_OMOD, P_NONE),
    E(060, TEXT_PUSH,  NONE, M_WMOD, M_NONE, P_OMOD, P_NONE),
};

static const uint8_t INDEX_FF[] PROGMEM = {
      2,  // TEXT_CALL
      3,  // TEXT_CALLF
      1,  // TEXT_DEC
      0,  // TEXT_INC
      4,  // TEXT_JMP
      5,  // TEXT_JMPF
      6,  // TEXT_PUSH
};
// clang-format on

struct TableI8086::EntryPage : EntryPageBase<Entry> {
    constexpr EntryPage(Config::opcode_t prefix, const Entry *table, const Entry *end,
            const uint8_t *index, const uint8_t *iend)
        : EntryPageBase(table, end, index, iend), _prefix(prefix) {}

    Config::opcode_t prefix() const { return pgm_read_byte(&_prefix); }

private:
    Config::opcode_t _prefix;
};

static const TableI8086::EntryPage I8086_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(TABLE_00), ARRAY_RANGE(INDEX_00)},
        {0x80, ARRAY_RANGE(TABLE_80), ARRAY_RANGE(INDEX_80)},
        {0x83, ARRAY_RANGE(TABLE_83), ARRAY_RANGE(INDEX_83)},  // M_IMM8
        {0x81, ARRAY_RANGE(TABLE_81), ARRAY_RANGE(INDEX_81)},  // M_IMM
        {0x8F, ARRAY_RANGE(TABLE_8F), ARRAY_RANGE(INDEX_8F)},
        {0xC6, ARRAY_RANGE(TABLE_C6), ARRAY_RANGE(INDEX_C6)},
        {0xC7, ARRAY_RANGE(TABLE_C7), ARRAY_RANGE(INDEX_C7)},
        {0xD0, ARRAY_RANGE(TABLE_D0), ARRAY_RANGE(INDEX_D0)},
        {0xD1, ARRAY_RANGE(TABLE_D1), ARRAY_RANGE(INDEX_D1)},
        {0xD2, ARRAY_RANGE(TABLE_D2), ARRAY_RANGE(INDEX_D2)},
        {0xD3, ARRAY_RANGE(TABLE_D3), ARRAY_RANGE(INDEX_D3)},
        {0xD4, ARRAY_RANGE(TABLE_D4), ARRAY_RANGE(INDEX_D4)},
        {0xD5, ARRAY_RANGE(TABLE_D5), ARRAY_RANGE(INDEX_D5)},
        {0xF6, ARRAY_RANGE(TABLE_F6), ARRAY_RANGE(INDEX_F6)},
        {0xF7, ARRAY_RANGE(TABLE_F7), ARRAY_RANGE(INDEX_F7)},
        {0xFE, ARRAY_RANGE(TABLE_FE), ARRAY_RANGE(INDEX_FE)},
        {0xFF, ARRAY_RANGE(TABLE_FF), ARRAY_RANGE(INDEX_FF)},
};

static bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_AL || opr == M_CL)
        return table == M_BREG || table == M_BMOD;
    if (opr == M_AX || opr == M_DX)
        return table == M_WREG || table == M_WMOD;
    if (opr == M_BREG)
        return table == M_BMOD;
    if (opr == M_WREG)
        return table == M_WMOD;
    if (opr == M_MEM)  // checked by |acceptSize| later.
        return table == M_BMOD || table == M_BMEM || table == M_WMOD || table == M_WMEM;
    if (opr == M_DIR)  // checked by |acceptSize| later.
        return table == M_BMOD || table == M_BMEM || table == M_BDIR || table == M_WMOD ||
               table == M_WMEM || table == M_WDIR;
    if (opr == M_IMM || opr == M_IMM8 || opr == M_VAL1 || opr == M_VAL3)
        return table == M_IMM || table == M_IOA || table == M_UI16 || table == M_REL8 ||
               table == M_REL;
    if (opr == M_BMEM)
        return table == M_BMOD;
    if (opr == M_WMEM)
        return table == M_WMOD;
    if (opr == M_BDIR)
        return table == M_BMOD || table == M_BMEM;
    if (opr == M_WDIR)
        return table == M_WMOD || table == M_WMEM;
    if (opr == M_CS)
        return table == M_SREG;
    return false;
}

static bool hasSize(AddrMode mode) {
    return mode == M_AX || mode == M_DX || mode == M_WREG || mode == M_AL || mode == M_CL ||
           mode == M_BREG || mode == M_CS || mode == M_SREG;
}

static bool acceptSize(const InsnI8086 &insn, const Entry *entry) {
    auto dst = insn.dstMode();
    auto src = insn.srcMode();
    auto flags = entry->flags();
    if (dst == M_MEM || dst == M_DIR) {
        if (src == M_NONE)
            return flags.size() == SZ_NONE;
        return hasSize(src) || flags.strInst();
    }
    if (src == M_MEM || src == M_DIR)
        return hasSize(dst) || flags.strInst();
    return true;
}

static bool acceptModes(const InsnI8086 &insn, const Entry *entry) {
    auto table = entry->flags();
    return acceptMode(insn.dstMode(), table.dstMode()) &&
        acceptMode(insn.srcMode(), table.srcMode()) &&
        acceptSize(insn, entry);
}

Error TableI8086::searchName(InsnI8086 &insn, const EntryPage *pages, const EntryPage *end) const {
    uint8_t count = 0;
    for (auto page = pages; page < end; page++) {
        auto entry = TableBase::searchName<EntryPage, Entry, const InsnI8086&>(
                         insn.name(), insn, page, acceptModes, count);
        if (entry) {
            insn.setOpCode(entry->opCode(), page->prefix());
            insn.setFlags(entry->flags());
            return OK;
        }
    }
    return count == 0 ? UNKNOWN_INSTRUCTION : OPERAND_NOT_ALLOWED;
}

bool TableI8086::isRepeatPrefix(Config::opcode_t opCode) const {
    return opCode == 0xF2 || opCode == 0xF3;
}

bool TableI8086::isSegmentPrefix(Config::opcode_t opCode) const {
    return overrideSeg(opCode) != REG_UNDEF;
}

RegName TableI8086::overrideSeg(Config::opcode_t opCode) const {
    switch (opCode) {
    case 0x26:
        return REG_ES;
    case 0x2E:
        return REG_CS;
    case 0x36:
        return REG_SS;
    case 0x3E:
        return REG_DS;
    default:
        return REG_UNDEF;
    }
}

Config::opcode_t TableI8086::segOverridePrefix(RegName name) const {
    switch (name) {
    case REG_ES:
        return 0x26;
    case REG_CS:
        return 0x2E;
    case REG_SS:
        return 0x36;
    case REG_DS:
        return 0x3E;
    default:
        return 0;
    }
}

bool TableI8086::isPrefix(Config::opcode_t opCode) const {
    for (auto page = ARRAY_BEGIN(I8086_PAGES); page < ARRAY_END(I8086_PAGES); page++) {
        auto prefix = page->prefix();
        if (prefix == 0)
            continue;
        if (prefix == opCode)
            return true;
    }
    return false;
}

static Config::opcode_t maskCode(Config::opcode_t opcode, const Entry *entry) {
    auto dstPos = entry->flags().dstPos();
    auto srcPos = entry->flags().srcPos();
    Config::opcode_t mask = 0;
    if (dstPos == P_OREG || srcPos == P_OREG)
        mask |= 0007;
    if (dstPos == P_OSEG || srcPos == P_OSEG)
        mask |= 0030;
    if (dstPos == P_OMOD || srcPos == P_OMOD)
        mask |= 0307;
    return opcode & ~mask;
}

Error TableI8086::searchOpCode(
        InsnI8086 &insn, const EntryPage *pages, const EntryPage *end) const {
    for (auto page = pages; page < end; page++) {
        if (insn.prefix() != page->prefix())
            continue;
        auto entry = TableBase::searchCode<Entry, Config::opcode_t>(
                insn.opCode(), page->table(), page->end(), maskCode);
        if (entry) {
            insn.setFlags(entry->flags());
            insn.setName_P(entry->name_P());
            return OK;
        }
    }
    return UNKNOWN_INSTRUCTION;
}

Error TableI8086::searchName(InsnI8086 &insn) {
    return setError(searchName(insn, ARRAY_RANGE(I8086_PAGES)));
}

Error TableI8086::searchOpCode(InsnI8086 &insn) {
    return setError(searchOpCode(insn, ARRAY_RANGE(I8086_PAGES)));
}

const /* PROGMEM */ char *TableI8086::listCpu_P() const {
    return TEXT_CPU_I8086;
}

const /* PROGMEM */ char *TableI8086::cpu_P() const {
    return TEXT_CPU_8086;
}

bool TableI8086::setCpu(const char *cpu) {
    if (toupper(*cpu) == 'I')
        cpu++;
    return strcmp_P(cpu, TEXT_CPU_8086) == 0;
}

class TableI8086 TableI8086;

}  // namespace i8086
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

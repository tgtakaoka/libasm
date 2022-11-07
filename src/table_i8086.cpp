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

#define E3(_opc, _name, _sz, _dst, _src, _ext, _dpos, _spos, _epos) \
    { _opc, Entry::Flags::create(_dst, _src, _ext, _dpos, _spos, _epos, SZ_##_sz), _name }
#define E2(_opc, _name, _sz, _dst, _src, _dpos, _spos) \
    E3(_opc, _name, _sz, _dst, _src, M_NONE, _dpos, _spos, P_NONE)
#define E1(_opc, _name, _sz, _dst, _dpos) E2(_opc, _name, _sz, _dst, M_NONE, _dpos, P_NONE)
#define E0(_opc, _name, _sz) E1(_opc, _name, _sz, M_NONE, P_NONE)
#define S2(_opc, _name, _sz, _dst, _src) \
    { _opc, Entry::Flags::strInst(_dst, _src, SZ_##_sz), _name }
#define S1(_opc, _name, _sz, _dst) S2(_opc, _name, _sz, _dst, M_NONE)
#define S0(_opc, _name, _sz) S1(_opc, _name, _sz, M_NONE)

// clang-format off
static constexpr Entry TABLE_00[] PROGMEM = {
    E2(0xA0, TEXT_MOV,    BYTE, M_AL,   M_BDIR, P_NONE, P_OPR),
    E2(0xA1, TEXT_MOV,    WORD, M_AX,   M_WDIR, P_NONE, P_OPR),
    E2(0xA2, TEXT_MOV,    BYTE, M_BDIR, M_AL,   P_OPR,  P_NONE),
    E2(0xA3, TEXT_MOV,    WORD, M_WDIR, M_AX,   P_OPR,  P_NONE),
    E0(0x90, TEXT_NOP,    NONE),
    E2(0x90, TEXT_XCHG,   WORD, M_AX,   M_WREG, P_NONE, P_OREG),
    E2(0x90, TEXT_XCHG,   WORD, M_WREG, M_AX,   P_OREG, P_NONE),
    E2(0x04, TEXT_ADD,    BYTE, M_AL,   M_IMM,  P_NONE, P_OPR),
    E2(0x05, TEXT_ADD,    WORD, M_AX,   M_IMM,  P_NONE, P_OPR),
    E2(0x0C, TEXT_OR,     BYTE, M_AL,   M_IMM,  P_NONE, P_OPR),
    E2(0x0D, TEXT_OR,     WORD, M_AX,   M_IMM,  P_NONE, P_OPR),
    E2(0x02, TEXT_ADD,    BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    E2(0x00, TEXT_ADD,    BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E2(0x03, TEXT_ADD,    WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    E2(0x01, TEXT_ADD,    WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    E1(0x06, TEXT_PUSH,   WORD, M_SREG, P_OSEG),
    E1(0x0F, TEXT_POP,    WORD, M_CS,   P_NONE),
    E1(0x07, TEXT_POP,    WORD, M_SREG, P_OSEG),
    E2(0x0A, TEXT_OR,     BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    E2(0x08, TEXT_OR,     BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E2(0x0B, TEXT_OR,     WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    E2(0x09, TEXT_OR,     WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    E2(0x12, TEXT_ADC,    BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    E2(0x10, TEXT_ADC,    BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E2(0x13, TEXT_ADC,    WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    E2(0x11, TEXT_ADC,    WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    E2(0x14, TEXT_ADC,    BYTE, M_AL,   M_IMM,  P_NONE, P_OPR),
    E2(0x15, TEXT_ADC,    WORD, M_AX,   M_IMM,  P_NONE, P_OPR),
    E2(0x1A, TEXT_SBB,    BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    E2(0x18, TEXT_SBB,    BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E2(0x1B, TEXT_SBB,    WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    E2(0x19, TEXT_SBB,    WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    E2(0x1C, TEXT_SBB,    BYTE, M_AL,   M_IMM,  P_NONE, P_OPR),
    E2(0x1D, TEXT_SBB,    WORD, M_AX,   M_IMM,  P_NONE, P_OPR),
    E2(0x22, TEXT_AND,    BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    E2(0x20, TEXT_AND,    BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E2(0x23, TEXT_AND,    WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    E2(0x21, TEXT_AND,    WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    E2(0x24, TEXT_AND,    BYTE, M_AL,   M_IMM,  P_NONE, P_OPR),
    E2(0x25, TEXT_AND,    WORD, M_AX,   M_IMM,  P_NONE, P_OPR),
    E0(0x26, TEXT_SEGES,  BYTE),
    E0(0x27, TEXT_DAA,    BYTE),
    E2(0x2A, TEXT_SUB,    BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    E2(0x28, TEXT_SUB,    BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E2(0x2B, TEXT_SUB,    WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    E2(0x29, TEXT_SUB,    WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    E2(0x2C, TEXT_SUB,    BYTE, M_AL,   M_IMM,  P_NONE, P_OPR),
    E2(0x2D, TEXT_SUB,    WORD, M_AX,   M_IMM,  P_NONE, P_OPR),
    E0(0x2E, TEXT_SEGCS,  BYTE),
    E0(0x2F, TEXT_DAS,    BYTE),
    E2(0x32, TEXT_XOR,    BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    E2(0x30, TEXT_XOR,    BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E2(0x33, TEXT_XOR,    WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    E2(0x31, TEXT_XOR,    WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    E2(0x34, TEXT_XOR,    BYTE, M_AL,   M_IMM,  P_NONE, P_OPR),
    E2(0x35, TEXT_XOR,    WORD, M_AX,   M_IMM,  P_NONE, P_OPR),
    E0(0x36, TEXT_SEGSS,  BYTE),
    E0(0x37, TEXT_AAA,    BYTE),
    E2(0x3A, TEXT_CMP,    BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    E2(0x38, TEXT_CMP,    BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E2(0x3B, TEXT_CMP,    WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    E2(0x39, TEXT_CMP,    WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    E2(0x3C, TEXT_CMP,    BYTE, M_AL,   M_IMM,  P_NONE, P_OPR),
    E2(0x3D, TEXT_CMP,    WORD, M_AX,   M_IMM,  P_NONE, P_OPR),
    E0(0x3E, TEXT_SEGDS,  BYTE),
    E0(0x3F, TEXT_AAS,    BYTE),
    E1(0x40, TEXT_INC,    WORD, M_WREG, P_OREG),
    E1(0x48, TEXT_DEC,    WORD, M_WREG, P_OREG),
    E1(0x50, TEXT_PUSH,   WORD, M_WREG, P_OREG),
    E1(0x58, TEXT_POP,    WORD, M_WREG, P_OREG),
    E1(0x70, TEXT_JO,     NONE, M_REL8, P_OPR),
    E1(0x71, TEXT_JNO,    NONE, M_REL8, P_OPR),
    E1(0x72, TEXT_JB,     NONE, M_REL8, P_OPR),
    E1(0x72, TEXT_JC,     NONE, M_REL8, P_OPR),
    E1(0x72, TEXT_JNAE,   NONE, M_REL8, P_OPR),
    E1(0x73, TEXT_JAE,    NONE, M_REL8, P_OPR),
    E1(0x73, TEXT_JNB,    NONE, M_REL8, P_OPR),
    E1(0x73, TEXT_JNC,    NONE, M_REL8, P_OPR),
    E1(0x74, TEXT_JE,     NONE, M_REL8, P_OPR),
    E1(0x74, TEXT_JZ,     NONE, M_REL8, P_OPR),
    E1(0x75, TEXT_JNE,    NONE, M_REL8, P_OPR),
    E1(0x75, TEXT_JNZ,    NONE, M_REL8, P_OPR),
    E1(0x76, TEXT_JBE,    NONE, M_REL8, P_OPR),
    E1(0x76, TEXT_JNA,    NONE, M_REL8, P_OPR),
    E1(0x77, TEXT_JA,     NONE, M_REL8, P_OPR),
    E1(0x77, TEXT_JNBE,   NONE, M_REL8, P_OPR),
    E1(0x78, TEXT_JS,     NONE, M_REL8, P_OPR),
    E1(0x79, TEXT_JNS,    NONE, M_REL8, P_OPR),
    E1(0x7A, TEXT_JPE,    NONE, M_REL8, P_OPR),
    E1(0x7A, TEXT_JP,     NONE, M_REL8, P_OPR),
    E1(0x7B, TEXT_JPO,    NONE, M_REL8, P_OPR),
    E1(0x7B, TEXT_JNP,    NONE, M_REL8, P_OPR),
    E1(0x7C, TEXT_JL,     NONE, M_REL8, P_OPR),
    E1(0x7C, TEXT_JNGE,   NONE, M_REL8, P_OPR),
    E1(0x7D, TEXT_JGE,    NONE, M_REL8, P_OPR),
    E1(0x7D, TEXT_JNL,    NONE, M_REL8, P_OPR),
    E1(0x7E, TEXT_JLE,    NONE, M_REL8, P_OPR),
    E1(0x7E, TEXT_JNG,    NONE, M_REL8, P_OPR),
    E1(0x7F, TEXT_JG,     NONE, M_REL8, P_OPR),
    E1(0x7F, TEXT_JNLE,   NONE, M_REL8, P_OPR),
    E2(0x84, TEXT_TEST,   BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    E2(0x84, TEXT_TEST,   BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E2(0x85, TEXT_TEST,   WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    E2(0x85, TEXT_TEST,   WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    E2(0x86, TEXT_XCHG,   BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    E2(0x86, TEXT_XCHG,   BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E2(0x87, TEXT_XCHG,   WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    E2(0x87, TEXT_XCHG,   WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    E2(0x8A, TEXT_MOV,    BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    E2(0x88, TEXT_MOV,    BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E2(0x8B, TEXT_MOV,    WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    E2(0x89, TEXT_MOV,    WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    E2(0x8C, TEXT_MOV,    WORD, M_WMOD, M_SREG, P_MOD,  P_REG),
    E2(0x8D, TEXT_LEA,    WORD, M_WREG, M_BMEM, P_REG,  P_MOD),
    E2(0x8D, TEXT_LEA,    WORD, M_WREG, M_WMEM, P_REG,  P_MOD),
    E2(0x8E, TEXT_MOV,    WORD, M_SREG, M_WMOD, P_REG,  P_MOD),
    E0(0x98, TEXT_CBW,    BYTE),
    E0(0x99, TEXT_CWD,    WORD),
    E1(0x9A, TEXT_CALLF,  NONE, M_FAR,  P_OPR),
    E0(0x9B, TEXT_WAIT,   NONE),
    E0(0x9C, TEXT_PUSHF,  WORD),
    E0(0x9D, TEXT_POPF,   WORD),
    E0(0x9E, TEXT_SAHF,   BYTE),
    E0(0x9F, TEXT_LAHF,   BYTE),
    S0(0xA4, TEXT_MOVSB,  BYTE),
    S0(0xA5, TEXT_MOVSW,  WORD),
    S2(0xA4, TEXT_MOVSB,  BYTE, M_BMEM, M_BMEM),
    S2(0xA5, TEXT_MOVSW,  WORD, M_WMEM, M_WMEM),
    S2(0xA4, TEXT_MOVS,   BYTE, M_BMEM, M_BMEM),
    S2(0xA5, TEXT_MOVS,   WORD, M_WMEM, M_WMEM),
    S0(0xA6, TEXT_CMPSB,  BYTE),
    S0(0xA7, TEXT_CMPSW,  WORD),
    S2(0xA6, TEXT_CMPSB,  BYTE, M_BMEM, M_BMEM),
    S2(0xA7, TEXT_CMPSW,  WORD, M_WMEM, M_WMEM),
    S2(0xA6, TEXT_CMPS,   BYTE, M_BMEM, M_BMEM),
    S2(0xA7, TEXT_CMPS,   WORD, M_WMEM, M_WMEM),
    E2(0xA8, TEXT_TEST,   BYTE, M_AL,   M_IMM,  P_NONE, P_OPR),
    E2(0xA9, TEXT_TEST,   WORD, M_AX,   M_IMM,  P_NONE, P_OPR),
    S0(0xAA, TEXT_STOSB,  BYTE),
    S0(0xAB, TEXT_STOSW,  WORD),
    S1(0xAA, TEXT_STOSB,  NONE, M_BMEM),
    S1(0xAB, TEXT_STOSW,  NONE, M_WMEM),
    S1(0xAA, TEXT_STOS,   BYTE, M_BMEM),
    S1(0xAB, TEXT_STOS,   WORD, M_WMEM),
    S0(0xAC, TEXT_LODSB,  BYTE),
    S0(0xAD, TEXT_LODSW,  WORD),
    S1(0xAC, TEXT_LODSB,  NONE, M_BMEM),
    S1(0xAD, TEXT_LODSW,  NONE, M_WMEM),
    S1(0xAC, TEXT_LODS,   BYTE, M_BMEM),
    S1(0xAD, TEXT_LODS,   WORD, M_WMEM),
    S0(0xAE, TEXT_SCASB,  BYTE),
    S0(0xAF, TEXT_SCASW,  WORD),
    S1(0xAE, TEXT_SCASB,  NONE, M_BMEM),
    S1(0xAF, TEXT_SCASW,  NONE, M_WMEM),
    S1(0xAE, TEXT_SCAS,   BYTE, M_BMEM),
    S1(0xAF, TEXT_SCAS,   WORD, M_WMEM),
    E2(0xB0, TEXT_MOV,    BYTE, M_BREG, M_IMM,  P_OREG, P_OPR),
    E2(0xB8, TEXT_MOV,    WORD, M_WREG, M_IMM,  P_OREG, P_OPR),
    E1(0xC2, TEXT_RET,    WORD, M_UI16, P_OPR),
    E0(0xC3, TEXT_RET,    NONE),
    E2(0xC4, TEXT_LES,    WORD, M_WREG, M_WMEM, P_REG,  P_MOD),
    E2(0xC5, TEXT_LDS,    WORD, M_WREG, M_WMEM, P_REG,  P_MOD),
    E1(0xCA, TEXT_RETF,   WORD, M_UI16, P_OPR),
    E0(0xCB, TEXT_RETF,   NONE),
    E1(0xCC, TEXT_INT,    NONE, M_VAL3, P_NONE),
    E1(0xCD, TEXT_INT,    BYTE, M_IMM,  P_OPR),
    E0(0xCE, TEXT_INTO,   NONE),
    E0(0xCF, TEXT_IRET,   NONE),
    E0(0xD7, TEXT_XLAT,   NONE),
    E1(0xE0, TEXT_LOOPNE, NONE, M_REL8, P_OPR),
    E1(0xE0, TEXT_LOOPNZ, NONE, M_REL8, P_OPR),
    E1(0xE1, TEXT_LOOPE,  NONE, M_REL8, P_OPR),
    E1(0xE1, TEXT_LOOPZ,  NONE, M_REL8, P_OPR),
    E1(0xE2, TEXT_LOOP,   NONE, M_REL8, P_OPR),
    E1(0xE3, TEXT_JCXZ,   NONE, M_REL8, P_OPR),
    E2(0xE4, TEXT_IN,     BYTE, M_AL,   M_IOA,  P_NONE, P_OPR),
    E2(0xE5, TEXT_IN,     WORD, M_AX,   M_IOA,  P_NONE, P_OPR),
    E2(0xE6, TEXT_OUT,    BYTE, M_IOA,  M_AL,   P_OPR,  P_NONE),
    E2(0xE7, TEXT_OUT,    WORD, M_IOA,  M_AX,   P_OPR,  P_NONE),
    E1(0xE8, TEXT_CALL,   NONE, M_REL,  P_OPR),
    E1(0xEB, TEXT_JMP,    NONE, M_REL8, P_OPR),
    E1(0xE9, TEXT_JMP,    NONE, M_REL,  P_OPR),
    E1(0xEA, TEXT_JMPF,   NONE, M_FAR,  P_OPR),
    E2(0xEC, TEXT_IN,     BYTE, M_AL,   M_DX,   P_NONE, P_NONE),
    E2(0xED, TEXT_IN,     WORD, M_AX,   M_DX,   P_NONE, P_NONE),
    E2(0xEE, TEXT_OUT,    BYTE, M_DX,   M_AL,   P_NONE, P_NONE),
    E2(0xEF, TEXT_OUT,    WORD, M_DX,   M_AX,   P_NONE, P_NONE),
    E0(0xF0, TEXT_LOCK,   NONE),
    E1(0xF2, TEXT_REPNE,  NONE, M_ISTR, P_OPR),
    E1(0xF2, TEXT_REPNZ,  NONE, M_ISTR, P_OPR),
    E1(0xF3, TEXT_REP,    NONE, M_ISTR, P_OPR),
    E1(0xF3, TEXT_REPE,   NONE, M_ISTR, P_OPR),
    E1(0xF3, TEXT_REPZ,   NONE, M_ISTR, P_OPR),
    E0(0xF2, TEXT_REPNE,  NONE),
    E0(0xF2, TEXT_REPNZ,  NONE),
    E0(0xF3, TEXT_REP,    NONE),
    E0(0xF3, TEXT_REPE,   NONE),
    E0(0xF3, TEXT_REPZ,   NONE),
    E0(0xF4, TEXT_HLT,    NONE),
    E0(0xF5, TEXT_CMC,    NONE),
    E0(0xF8, TEXT_CLC,    NONE),
    E0(0xF9, TEXT_STC,    NONE),
    E0(0xFA, TEXT_CLI,    NONE),
    E0(0xFB, TEXT_STI,    NONE),
    E0(0xFC, TEXT_CLD,    NONE),
    E0(0xFD, TEXT_STD,    NONE),
};

static constexpr uint8_t INDEX_00[] PROGMEM = {
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

static constexpr Entry TABLE_80[] PROGMEM = {
    E2(000, TEXT_ADD, BYTE, M_BMOD, M_IMM, P_OMOD, P_OPR),
    E2(010, TEXT_OR,  BYTE, M_BMOD, M_IMM, P_OMOD, P_OPR),
    E2(020, TEXT_ADC, BYTE, M_BMOD, M_IMM, P_OMOD, P_OPR),
    E2(030, TEXT_SBB, BYTE, M_BMOD, M_IMM, P_OMOD, P_OPR),
    E2(040, TEXT_AND, BYTE, M_BMOD, M_IMM, P_OMOD, P_OPR),
    E2(050, TEXT_SUB, BYTE, M_BMOD, M_IMM, P_OMOD, P_OPR),
    E2(060, TEXT_XOR, BYTE, M_BMOD, M_IMM, P_OMOD, P_OPR),
    E2(070, TEXT_CMP, BYTE, M_BMOD, M_IMM, P_OMOD, P_OPR),
};

static constexpr uint8_t INDEX_80[] PROGMEM = {
      2,  // TEXT_ADC
      0,  // TEXT_ADD
      4,  // TEXT_AND
      7,  // TEXT_CMP
      1,  // TEXT_OR
      3,  // TEXT_SBB
      5,  // TEXT_SUB
      6,  // TEXT_XOR
};

static constexpr Entry TABLE_81[] PROGMEM = {
    E2(000, TEXT_ADD, WORD, M_WMOD, M_IMM, P_OMOD, P_OPR),
    E2(010, TEXT_OR,  WORD, M_WMOD, M_IMM, P_OMOD, P_OPR),
    E2(020, TEXT_ADC, WORD, M_WMOD, M_IMM, P_OMOD, P_OPR),
    E2(030, TEXT_SBB, WORD, M_WMOD, M_IMM, P_OMOD, P_OPR),
    E2(040, TEXT_AND, WORD, M_WMOD, M_IMM, P_OMOD, P_OPR),
    E2(050, TEXT_SUB, WORD, M_WMOD, M_IMM, P_OMOD, P_OPR),
    E2(060, TEXT_XOR, WORD, M_WMOD, M_IMM, P_OMOD, P_OPR),
    E2(070, TEXT_CMP, WORD, M_WMOD, M_IMM, P_OMOD, P_OPR),
};

static constexpr uint8_t INDEX_81[] PROGMEM = {
      2,  // TEXT_ADC
      0,  // TEXT_ADD
      4,  // TEXT_AND
      7,  // TEXT_CMP
      1,  // TEXT_OR
      3,  // TEXT_SBB
      5,  // TEXT_SUB
      6,  // TEXT_XOR
};

static constexpr Entry TABLE_83[] PROGMEM = {
    E2(000, TEXT_ADD, WORD, M_WMOD, M_IMM8, P_OMOD, P_OPR),
    E2(010, TEXT_OR,  WORD, M_WMOD, M_IMM8, P_OMOD, P_OPR),
    E2(020, TEXT_ADC, WORD, M_WMOD, M_IMM8, P_OMOD, P_OPR),
    E2(030, TEXT_SBB, WORD, M_WMOD, M_IMM8, P_OMOD, P_OPR),
    E2(040, TEXT_AND, WORD, M_WMOD, M_IMM8, P_OMOD, P_OPR),
    E2(050, TEXT_SUB, WORD, M_WMOD, M_IMM8, P_OMOD, P_OPR),
    E2(060, TEXT_XOR, WORD, M_WMOD, M_IMM8, P_OMOD, P_OPR),
    E2(070, TEXT_CMP, WORD, M_WMOD, M_IMM8, P_OMOD, P_OPR),
};

static constexpr uint8_t INDEX_83[] PROGMEM = {
      2,  // TEXT_ADC
      0,  // TEXT_ADD
      4,  // TEXT_AND
      7,  // TEXT_CMP
      1,  // TEXT_OR
      3,  // TEXT_SBB
      5,  // TEXT_SUB
      6,  // TEXT_XOR
};

static constexpr Entry TABLE_8F[] PROGMEM = {
    E1(000, TEXT_POP, NONE, M_WMOD, P_OMOD),
};

static constexpr uint8_t INDEX_8F[] PROGMEM = {
      0,  // TEXT_POP
};

static constexpr Entry TABLE_C6[] PROGMEM = {
    E2(000, TEXT_MOV, BYTE, M_BMOD, M_IMM, P_OMOD, P_OPR),
};

static constexpr uint8_t INDEX_C6[] PROGMEM = {
      0,  // TEXT_MOV
};

static constexpr Entry TABLE_C7[] PROGMEM = {
    E2(000, TEXT_MOV, WORD, M_WMOD, M_IMM, P_OMOD, P_OPR),
};

static constexpr uint8_t INDEX_C7[] PROGMEM = {
      0,  // TEXT_MOV
};

static constexpr Entry TABLE_D0[] PROGMEM = {
    E2(000, TEXT_ROL, BYTE, M_BMOD, M_VAL1, P_OMOD, P_NONE),
    E2(010, TEXT_ROR, BYTE, M_BMOD, M_VAL1, P_OMOD, P_NONE),
    E2(020, TEXT_RCL, BYTE, M_BMOD, M_VAL1, P_OMOD, P_NONE),
    E2(030, TEXT_RCR, BYTE, M_BMOD, M_VAL1, P_OMOD, P_NONE),
    E2(040, TEXT_SHL, BYTE, M_BMOD, M_VAL1, P_OMOD, P_NONE),
    E2(040, TEXT_SAL, BYTE, M_BMOD, M_VAL1, P_OMOD, P_NONE),
    E2(050, TEXT_SHR, BYTE, M_BMOD, M_VAL1, P_OMOD, P_NONE),
    E2(070, TEXT_SAR, BYTE, M_BMOD, M_VAL1, P_OMOD, P_NONE),
};

static constexpr uint8_t INDEX_D0[] PROGMEM = {
      2,  // TEXT_RCL
      3,  // TEXT_RCR
      0,  // TEXT_ROL
      1,  // TEXT_ROR
      5,  // TEXT_SAL
      7,  // TEXT_SAR
      4,  // TEXT_SHL
      6,  // TEXT_SHR
};

static constexpr Entry TABLE_D1[] PROGMEM = {
    E2(000, TEXT_ROL, WORD, M_WMOD, M_VAL1, P_OMOD, P_NONE),
    E2(010, TEXT_ROR, WORD, M_WMOD, M_VAL1, P_OMOD, P_NONE),
    E2(020, TEXT_RCL, WORD, M_WMOD, M_VAL1, P_OMOD, P_NONE),
    E2(030, TEXT_RCR, WORD, M_WMOD, M_VAL1, P_OMOD, P_NONE),
    E2(040, TEXT_SHL, WORD, M_WMOD, M_VAL1, P_OMOD, P_NONE),
    E2(040, TEXT_SAL, WORD, M_WMOD, M_VAL1, P_OMOD, P_NONE),
    E2(050, TEXT_SHR, WORD, M_WMOD, M_VAL1, P_OMOD, P_NONE),
    E2(070, TEXT_SAR, WORD, M_WMOD, M_VAL1, P_OMOD, P_NONE),
};

static constexpr uint8_t INDEX_D1[] PROGMEM = {
      2,  // TEXT_RCL
      3,  // TEXT_RCR
      0,  // TEXT_ROL
      1,  // TEXT_ROR
      5,  // TEXT_SAL
      7,  // TEXT_SAR
      4,  // TEXT_SHL
      6,  // TEXT_SHR
};

static constexpr Entry TABLE_D2[] PROGMEM = {
    E2(000, TEXT_ROL, BYTE, M_BMOD, M_CL, P_OMOD, P_NONE),
    E2(010, TEXT_ROR, BYTE, M_BMOD, M_CL, P_OMOD, P_NONE),
    E2(020, TEXT_RCL, BYTE, M_BMOD, M_CL, P_OMOD, P_NONE),
    E2(030, TEXT_RCR, BYTE, M_BMOD, M_CL, P_OMOD, P_NONE),
    E2(040, TEXT_SHL, BYTE, M_BMOD, M_CL, P_OMOD, P_NONE),
    E2(040, TEXT_SAL, BYTE, M_BMOD, M_CL, P_OMOD, P_NONE),
    E2(050, TEXT_SHR, BYTE, M_BMOD, M_CL, P_OMOD, P_NONE),
    E2(070, TEXT_SAR, BYTE, M_BMOD, M_CL, P_OMOD, P_NONE),
};

static constexpr uint8_t INDEX_D2[] PROGMEM = {
      2,  // TEXT_RCL
      3,  // TEXT_RCR
      0,  // TEXT_ROL
      1,  // TEXT_ROR
      5,  // TEXT_SAL
      7,  // TEXT_SAR
      4,  // TEXT_SHL
      6,  // TEXT_SHR
};

static constexpr Entry TABLE_D3[] PROGMEM = {
    E2(000, TEXT_ROL, WORD, M_WMOD, M_CL, P_OMOD, P_NONE),
    E2(010, TEXT_ROR, WORD, M_WMOD, M_CL, P_OMOD, P_NONE),
    E2(020, TEXT_RCL, WORD, M_WMOD, M_CL, P_OMOD, P_NONE),
    E2(030, TEXT_RCR, WORD, M_WMOD, M_CL, P_OMOD, P_NONE),
    E2(040, TEXT_SHL, WORD, M_WMOD, M_CL, P_OMOD, P_NONE),
    E2(040, TEXT_SAL, WORD, M_WMOD, M_CL, P_OMOD, P_NONE),
    E2(050, TEXT_SHR, WORD, M_WMOD, M_CL, P_OMOD, P_NONE),
    E2(070, TEXT_SAR, WORD, M_WMOD, M_CL, P_OMOD, P_NONE),
};

static constexpr uint8_t INDEX_D3[] PROGMEM = {
      2,  // TEXT_RCL
      3,  // TEXT_RCR
      0,  // TEXT_ROL
      1,  // TEXT_ROR
      5,  // TEXT_SAL
      7,  // TEXT_SAR
      4,  // TEXT_SHL
      6,  // TEXT_SHR
};

static constexpr Entry TABLE_D4[] PROGMEM = {
    E0(0x0A, TEXT_AAM, BYTE),
};

static constexpr uint8_t INDEX_D4[] PROGMEM = {
      0,  // TEXT_AAM
};

static constexpr Entry TABLE_D5[] PROGMEM = {
    E0(0x0A, TEXT_AAD, BYTE),
};

static constexpr uint8_t INDEX_D5[] PROGMEM = {
      0,  // TEXT_AAD
};

static constexpr Entry TABLE_F6[] PROGMEM = {
    E2(000, TEXT_TEST, BYTE, M_BMOD, M_IMM, P_OMOD, P_OPR),
    E1(020, TEXT_NOT,  BYTE, M_BMOD, P_OMOD),
    E1(030, TEXT_NEG,  BYTE, M_BMOD, P_OMOD),
    E1(040, TEXT_MUL,  BYTE, M_BMOD, P_OMOD),
    E1(050, TEXT_IMUL, BYTE, M_BMOD, P_OMOD),
    E1(060, TEXT_DIV,  BYTE, M_BMOD, P_OMOD),
    E1(070, TEXT_IDIV, BYTE, M_BMOD, P_OMOD),
};

static constexpr uint8_t INDEX_F6[] PROGMEM = {
      5,  // TEXT_DIV
      6,  // TEXT_IDIV
      4,  // TEXT_IMUL
      3,  // TEXT_MUL
      2,  // TEXT_NEG
      1,  // TEXT_NOT
      0,  // TEXT_TEST
};

static constexpr Entry TABLE_F7[] PROGMEM = {
    E2(000, TEXT_TEST, WORD, M_WMOD, M_IMM, P_OMOD, P_OPR),
    E1(020, TEXT_NOT,  WORD, M_WMOD, P_OMOD),
    E1(030, TEXT_NEG,  WORD, M_WMOD, P_OMOD),
    E1(040, TEXT_MUL,  WORD, M_WMOD, P_OMOD),
    E1(050, TEXT_IMUL, WORD, M_WMOD, P_OMOD),
    E1(060, TEXT_DIV,  WORD, M_WMOD, P_OMOD),
    E1(070, TEXT_IDIV, WORD, M_WMOD, P_OMOD),
};

static constexpr uint8_t INDEX_F7[] PROGMEM = {
      5,  // TEXT_DIV
      6,  // TEXT_IDIV
      4,  // TEXT_IMUL
      3,  // TEXT_MUL
      2,  // TEXT_NEG
      1,  // TEXT_NOT
      0,  // TEXT_TEST
};

static constexpr Entry TABLE_FE[] PROGMEM = {
    E1(000, TEXT_INC, BYTE, M_BMOD, P_OMOD),
    E1(010, TEXT_DEC, BYTE, M_BMOD, P_OMOD),
};

static constexpr uint8_t INDEX_FE[] PROGMEM = {
      1,  // TEXT_DEC
      0,  // TEXT_INC
};

static constexpr Entry TABLE_FF[] PROGMEM = {
    E1(000, TEXT_INC,   WORD, M_WMOD, P_OMOD),
    E1(010, TEXT_DEC,   WORD, M_WMOD, P_OMOD),
    E1(020, TEXT_CALL,  NONE, M_WMOD, P_OMOD),
    E1(030, TEXT_CALLF, NONE, M_WMEM, P_OMOD),
    E1(040, TEXT_JMP,   NONE, M_WMOD, P_OMOD),
    E1(050, TEXT_JMPF,  NONE, M_WMEM, P_OMOD),
    E1(060, TEXT_PUSH,  NONE, M_WMOD, P_OMOD),
};

static constexpr uint8_t INDEX_FF[] PROGMEM = {
      2,  // TEXT_CALL
      3,  // TEXT_CALLF
      1,  // TEXT_DEC
      0,  // TEXT_INC
      4,  // TEXT_JMP
      5,  // TEXT_JMPF
      6,  // TEXT_PUSH
};

// i80186

static constexpr Entry TABLE_I80186[] PROGMEM = {
    E0(0x60, TEXT_PUSHA, NONE),
    E0(0x61, TEXT_POPA,  NONE),
    E2(0x62, TEXT_BOUND, WORD, M_WREG, M_WMEM, P_REG,  P_MOD),
    E1(0x6A, TEXT_PUSH,  BYTE, M_IMM8, P_OPR), // M_IMM8
    E3(0x6B, TEXT_IMUL,  WORD, M_WREG, M_WMOD, M_IMM8, P_REG, P_MOD, P_OPR),
    E2(0x6B, TEXT_IMUL,  WORD, M_WREG, M_IMM8, P_MREG, P_OPR),  // M_IMM8
    E1(0x68, TEXT_PUSH,  WORD, M_IMM,  P_OPR), // M_IMM
    E3(0x69, TEXT_IMUL,  WORD, M_WREG, M_WMOD, M_IMM,  P_REG, P_MOD, P_OPR),
    E2(0x69, TEXT_IMUL,  WORD, M_WREG, M_IMM,  P_MREG, P_OPR),  // M_IMM
    S0(0x6C, TEXT_INSB,  BYTE),
    S2(0x6C, TEXT_INSB,  BYTE, M_MEM,  M_DX),
    S0(0x6D, TEXT_INSW,  WORD),
    S2(0x6D, TEXT_INSW,  WORD, M_MEM,  M_DX),
    S2(0x6C, TEXT_INS,   BYTE, M_BMEM, M_DX),
    S2(0x6D, TEXT_INS,   WORD, M_WMEM, M_DX),
    S0(0x6E, TEXT_OUTSB, BYTE),
    S2(0x6E, TEXT_OUTSB, BYTE, M_DX,   M_BMEM),
    S0(0x6F, TEXT_OUTSW, WORD),
    S2(0x6F, TEXT_OUTSW, WORD, M_DX,   M_WMEM),
    S2(0x6E, TEXT_OUTS,  BYTE, M_DX,   M_BMEM),
    S2(0x6F, TEXT_OUTS,  WORD, M_DX,   M_WMEM),
    E2(0xC8, TEXT_ENTER, NONE, M_UI16, M_UI8,  P_OPR,  P_OPR),
    E0(0xC9, TEXT_LEAVE, NONE),
};

static constexpr uint8_t INDEX_I80186[] PROGMEM = {
      2,  // TEXT_BOUND
     21,  // TEXT_ENTER
      4,  // TEXT_IMUL
      5,  // TEXT_IMUL
      7,  // TEXT_IMUL
      8,  // TEXT_IMUL
     13,  // TEXT_INS
     14,  // TEXT_INS
      9,  // TEXT_INSB
     10,  // TEXT_INSB
     11,  // TEXT_INSW
     12,  // TEXT_INSW
     22,  // TEXT_LEAVE
     19,  // TEXT_OUTS
     20,  // TEXT_OUTS
     15,  // TEXT_OUTSB
     16,  // TEXT_OUTSB
     17,  // TEXT_OUTSW
     18,  // TEXT_OUTSW
      1,  // TEXT_POPA
      3,  // TEXT_PUSH
      6,  // TEXT_PUSH
      0,  // TEXT_PUSHA
};

static constexpr Entry TABLE_C0[] PROGMEM = {
    E2(000, TEXT_ROL, BYTE, M_BMOD, M_BIT, P_OMOD, P_OPR),
    E2(010, TEXT_ROR, BYTE, M_BMOD, M_BIT, P_OMOD, P_OPR),
    E2(020, TEXT_RCL, BYTE, M_BMOD, M_BIT, P_OMOD, P_OPR),
    E2(030, TEXT_RCR, BYTE, M_BMOD, M_BIT, P_OMOD, P_OPR),
    E2(040, TEXT_SHL, BYTE, M_BMOD, M_BIT, P_OMOD, P_OPR),
    E2(040, TEXT_SAL, BYTE, M_BMOD, M_BIT, P_OMOD, P_OPR),
    E2(050, TEXT_SHR, BYTE, M_BMOD, M_BIT, P_OMOD, P_OPR),
    E2(070, TEXT_SAR, BYTE, M_BMOD, M_BIT, P_OMOD, P_OPR),
};

static constexpr uint8_t INDEX_C0[] PROGMEM = {
      2,  // TEXT_RCL
      3,  // TEXT_RCR
      0,  // TEXT_ROL
      1,  // TEXT_ROR
      5,  // TEXT_SAL
      7,  // TEXT_SAR
      4,  // TEXT_SHL
      6,  // TEXT_SHR
};

static constexpr Entry TABLE_C1[] PROGMEM = {
    E2(000, TEXT_ROL, WORD, M_WMOD, M_BIT, P_OMOD, P_OPR),
    E2(010, TEXT_ROR, WORD, M_WMOD, M_BIT, P_OMOD, P_OPR),
    E2(020, TEXT_RCL, WORD, M_WMOD, M_BIT, P_OMOD, P_OPR),
    E2(030, TEXT_RCR, WORD, M_WMOD, M_BIT, P_OMOD, P_OPR),
    E2(040, TEXT_SHL, WORD, M_WMOD, M_BIT, P_OMOD, P_OPR),
    E2(040, TEXT_SAL, WORD, M_WMOD, M_BIT, P_OMOD, P_OPR),
    E2(050, TEXT_SHR, WORD, M_WMOD, M_BIT, P_OMOD, P_OPR),
    E2(070, TEXT_SAR, WORD, M_WMOD, M_BIT, P_OMOD, P_OPR),
};

static constexpr uint8_t INDEX_C1[] PROGMEM = {
      2,  // TEXT_RCL
      3,  // TEXT_RCR
      0,  // TEXT_ROL
      1,  // TEXT_ROR
      5,  // TEXT_SAL
      7,  // TEXT_SAR
      4,  // TEXT_SHL
      6,  // TEXT_SHR
};

// V30

static constexpr Entry V30TABLE_00[] PROGMEM = {
    E1(0x64, TEXT_REPNC,  NONE, M_ISTR, P_OPR),
    E0(0x64, TEXT_REPNC,  NONE),
    E1(0x65, TEXT_REPC,   NONE, M_ISTR, P_OPR),
    E0(0x65, TEXT_REPC,   NONE),
};

static constexpr uint8_t V30INDEX_00[] PROGMEM = {
      2,  // TEXT_REPC
      3,  // TEXT_REPC
      0,  // TEXT_REPNC
      1,  // TEXT_REPNC
};

static constexpr Entry V30TABLE_0F[] PROGMEM = {
    E1(0xFF, TEXT_BRKEM,  BYTE, M_IMM,  P_OPR),
    S0(0x20, TEXT_ADD4S,  BYTE),
    S2(0x20, TEXT_ADD4S,  BYTE, M_BMEM, M_BMEM),
    S0(0x26, TEXT_CMP4S,  BYTE),
    S2(0x26, TEXT_CMP4S,  BYTE, M_BMEM, M_BMEM),
    S0(0x22, TEXT_SUB4S,  BYTE),
    S2(0x22, TEXT_SUB4S,  BYTE, M_BMEM, M_BMEM),
    E1(0x28, TEXT_ROL4,   BYTE, M_BMOD, P_MOD),
    E1(0x2A, TEXT_ROR4,   BYTE, M_BMOD, P_MOD),
    E2(0x31, TEXT_INS,    WORD, M_BREG, M_BREG, P_MOD,  P_REG),
    E2(0x39, TEXT_INS,    WORD, M_BREG, M_BIT,  P_MOD,  P_OPR),
    E2(0x33, TEXT_EXT,    WORD, M_BREG, M_BREG, P_MOD,  P_REG),
    E2(0x3B, TEXT_EXT,    WORD, M_BREG, M_BIT,  P_MOD,  P_OPR),
    E2(0x12, TEXT_CLR1,   BYTE, M_BMOD, M_CL,   P_MOD,  P_NONE),
    E2(0x13, TEXT_CLR1,   WORD, M_WMOD, M_CL,   P_MOD,  P_NONE),
    E2(0x1A, TEXT_CLR1,   BYTE, M_BMOD, M_BIT,  P_MOD,  P_OPR),
    E2(0x1B, TEXT_CLR1,   WORD, M_WMOD, M_BIT,  P_MOD,  P_OPR),
    E2(0x14, TEXT_SET1,   BYTE, M_BMOD, M_CL,   P_MOD,  P_NONE),
    E2(0x15, TEXT_SET1,   WORD, M_WMOD, M_CL,   P_MOD,  P_NONE),
    E2(0x1C, TEXT_SET1,   BYTE, M_BMOD, M_BIT,  P_MOD,  P_OPR),
    E2(0x1D, TEXT_SET1,   WORD, M_WMOD, M_BIT,  P_MOD,  P_OPR),
    E2(0x16, TEXT_NOT1,   BYTE, M_BMOD, M_CL,   P_MOD,  P_NONE),
    E2(0x17, TEXT_NOT1,   WORD, M_WMOD, M_CL,   P_MOD,  P_NONE),
    E2(0x1E, TEXT_NOT1,   BYTE, M_BMOD, M_BIT,  P_MOD,  P_OPR),
    E2(0x1F, TEXT_NOT1,   WORD, M_WMOD, M_BIT,  P_MOD,  P_OPR),
};

static constexpr uint8_t V30INDEX_0F[] PROGMEM = {
      1,  // TEXT_ADD4S
      2,  // TEXT_ADD4S
      0,  // TEXT_BRKEM
     13,  // TEXT_CLR1
     14,  // TEXT_CLR1
     15,  // TEXT_CLR1
     16,  // TEXT_CLR1
      3,  // TEXT_CMP4S
      4,  // TEXT_CMP4S
     11,  // TEXT_EXT
     12,  // TEXT_EXT
      9,  // TEXT_INS
     10,  // TEXT_INS
     21,  // TEXT_NOT1
     22,  // TEXT_NOT1
     23,  // TEXT_NOT1
     24,  // TEXT_NOT1
      7,  // TEXT_ROL4
      8,  // TEXT_ROR4
     17,  // TEXT_SET1
     18,  // TEXT_SET1
     19,  // TEXT_SET1
     20,  // TEXT_SET1
      5,  // TEXT_SUB4S
      6,  // TEXT_SUB4S
};

// clang-format on

static constexpr TableI8086::EntryPage I8086_PAGES[] PROGMEM = {
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

static constexpr TableI8086::EntryPage I80186_PAGES[] PROGMEM = {
        // I80186
        {0x00, ARRAY_RANGE(TABLE_I80186), ARRAY_RANGE(INDEX_I80186)},
        {0xD0, ARRAY_RANGE(TABLE_D0), ARRAY_RANGE(INDEX_D0)},
        {0xD1, ARRAY_RANGE(TABLE_D1), ARRAY_RANGE(INDEX_D1)},
        {0xC0, ARRAY_RANGE(TABLE_C0), ARRAY_RANGE(INDEX_C0)},
        {0xC1, ARRAY_RANGE(TABLE_C1), ARRAY_RANGE(INDEX_C1)},
        // i8086
        {0x00, ARRAY_RANGE(TABLE_00), ARRAY_RANGE(INDEX_00)},
        {0x80, ARRAY_RANGE(TABLE_80), ARRAY_RANGE(INDEX_80)},
        {0x83, ARRAY_RANGE(TABLE_83), ARRAY_RANGE(INDEX_83)},  // M_IMM8
        {0x81, ARRAY_RANGE(TABLE_81), ARRAY_RANGE(INDEX_81)},  // M_IMM
        {0x8F, ARRAY_RANGE(TABLE_8F), ARRAY_RANGE(INDEX_8F)},
        {0xC6, ARRAY_RANGE(TABLE_C6), ARRAY_RANGE(INDEX_C6)},
        {0xC7, ARRAY_RANGE(TABLE_C7), ARRAY_RANGE(INDEX_C7)},
        {0xD2, ARRAY_RANGE(TABLE_D2), ARRAY_RANGE(INDEX_D2)},
        {0xD3, ARRAY_RANGE(TABLE_D3), ARRAY_RANGE(INDEX_D3)},
        {0xD4, ARRAY_RANGE(TABLE_D4), ARRAY_RANGE(INDEX_D4)},
        {0xD5, ARRAY_RANGE(TABLE_D5), ARRAY_RANGE(INDEX_D5)},
        {0xF6, ARRAY_RANGE(TABLE_F6), ARRAY_RANGE(INDEX_F6)},
        {0xF7, ARRAY_RANGE(TABLE_F7), ARRAY_RANGE(INDEX_F7)},
        {0xFE, ARRAY_RANGE(TABLE_FE), ARRAY_RANGE(INDEX_FE)},
        {0xFF, ARRAY_RANGE(TABLE_FF), ARRAY_RANGE(INDEX_FF)},
};

static constexpr TableI8086::EntryPage V30_PAGES[] PROGMEM = {
        // V30
        {0x00, ARRAY_RANGE(V30TABLE_00), ARRAY_RANGE(V30INDEX_00)},
        {0x0F, ARRAY_RANGE(V30TABLE_0F), ARRAY_RANGE(V30INDEX_0F)},
        // I80186
        {0x00, ARRAY_RANGE(TABLE_I80186), ARRAY_RANGE(INDEX_I80186)},
        {0xD0, ARRAY_RANGE(TABLE_D0), ARRAY_RANGE(INDEX_D0)},
        {0xD1, ARRAY_RANGE(TABLE_D1), ARRAY_RANGE(INDEX_D1)},
        {0xC0, ARRAY_RANGE(TABLE_C0), ARRAY_RANGE(INDEX_C0)},
        {0xC1, ARRAY_RANGE(TABLE_C1), ARRAY_RANGE(INDEX_C1)},
        // i8086
        {0x00, ARRAY_RANGE(TABLE_00), ARRAY_RANGE(INDEX_00)},
        {0x80, ARRAY_RANGE(TABLE_80), ARRAY_RANGE(INDEX_80)},
        {0x83, ARRAY_RANGE(TABLE_83), ARRAY_RANGE(INDEX_83)},  // M_IMM8
        {0x81, ARRAY_RANGE(TABLE_81), ARRAY_RANGE(INDEX_81)},  // M_IMM
        {0x8F, ARRAY_RANGE(TABLE_8F), ARRAY_RANGE(INDEX_8F)},
        {0xC6, ARRAY_RANGE(TABLE_C6), ARRAY_RANGE(INDEX_C6)},
        {0xC7, ARRAY_RANGE(TABLE_C7), ARRAY_RANGE(INDEX_C7)},
        {0xD2, ARRAY_RANGE(TABLE_D2), ARRAY_RANGE(INDEX_D2)},
        {0xD3, ARRAY_RANGE(TABLE_D3), ARRAY_RANGE(INDEX_D3)},
        {0xD4, ARRAY_RANGE(TABLE_D4), ARRAY_RANGE(INDEX_D4)},
        {0xD5, ARRAY_RANGE(TABLE_D5), ARRAY_RANGE(INDEX_D5)},
        {0xF6, ARRAY_RANGE(TABLE_F6), ARRAY_RANGE(INDEX_F6)},
        {0xF7, ARRAY_RANGE(TABLE_F7), ARRAY_RANGE(INDEX_F7)},
        {0xFE, ARRAY_RANGE(TABLE_FE), ARRAY_RANGE(INDEX_FE)},
        {0xFF, ARRAY_RANGE(TABLE_FF), ARRAY_RANGE(INDEX_FF)},
};

static constexpr TableI8086::Cpu CPU_TABLE[] PROGMEM = {
        {I8086, TEXT_CPU_8086, ARRAY_RANGE(I8086_PAGES)},
        {I80186, TEXT_CPU_80186, ARRAY_RANGE(I80186_PAGES)},
        {V30, TEXT_CPU_V30, ARRAY_RANGE(V30_PAGES)},
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
    if (opr == M_VAL1 || opr == M_VAL3)
        return table == M_IMM || table == M_IMM8 || table == M_IOA || table == M_UI16 ||
               table == M_UI8 || table == M_BIT || table == M_REL8 || table == M_REL;
    if (opr == M_IMM || opr == M_IMM8)
        return table == M_IMM || table == M_IOA || table == M_UI16 || table == M_UI8 ||
               table == M_BIT || table == M_REL8 || table == M_REL;
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
    auto dst = insn.dst();
    auto src = insn.src();
    auto flags = entry->flags();
    if (dst == M_MEM || dst == M_DIR) {
        if (src == M_NONE)
            return flags.size() == SZ_NONE;
        return hasSize(src) || flags.stringInst();
    }
    if (src == M_MEM || src == M_DIR)
        return hasSize(dst) || flags.stringInst();
    return true;
}

static bool acceptModes(const InsnI8086 &insn, const Entry *entry) {
    auto table = entry->flags();
    return acceptMode(insn.dst(), table.dst()) && acceptMode(insn.src(), table.src()) &&
           acceptMode(insn.ext(), table.ext()) && acceptSize(insn, entry);
}

Error TableI8086::searchName(InsnI8086 &insn) {
    uint8_t count = 0;
    auto entry = _cpu->searchName(insn, acceptModes, count);
    return setError(entry ? OK : (count ? OPERAND_NOT_ALLOWED : UNKNOWN_INSTRUCTION));
}

bool TableI8086::isRepeatPrefix(Config::opcode_t opCode) const {
    if (opCode == 0xF2 || opCode == 0xF3)
        return true;
    if (_cpu->cpuType() == V30)
        return opCode == 0x0F;
    return false;
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

Error TableI8086::searchOpCode(InsnI8086 &insn) {
    for (auto page = _cpu->table(); page < _cpu->end(); page++) {
        if (insn.prefix() != page->prefix())
            continue;
        auto entry = searchEntry(insn.opCode(), page->table(), page->end(), maskCode);
        if (entry) {
            insn.setFlags(entry->flags());
            insn.nameBuffer().text_P(entry->name_P());
            return setOK();
        }
    }
    return setError(UNKNOWN_INSTRUCTION);
}

TableI8086::TableI8086() {
    setCpu(I8086);
}

bool TableI8086::setCpu(CpuType cpuType) {
    auto t = Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
    if (t == nullptr)
        return false;
    _cpu = t;
    return true;
}

const /* PROGMEM */ char *TableI8086::listCpu_P() const {
    return TEXT_CPU_LIST;
}

const /* PROGMEM */ char *TableI8086::cpu_P() const {
    return _cpu->name_P();
}

bool TableI8086::setCpu(const char *cpu) {
    auto t = Cpu::search(cpu, ARRAY_RANGE(CPU_TABLE));
    if (t)
        return setCpu(t->cpuType());
    if (strcasecmp_P(cpu, TEXT_CPU_I8086) == 0)
        return setCpu(I8086);
    if (strcasecmp_P(cpu, TEXT_CPU_I80186) == 0)
        return setCpu(I80186);
    return false;
}

TableI8086 TableI8086::TABLE;

}  // namespace i8086
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

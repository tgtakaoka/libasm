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

#include "entry_i8086.h"
#include "entry_table.h"
#include "text_i8086.h"

using namespace libasm::text::i8086;

namespace libasm {
namespace i8086 {

using namespace reg;

#define E3(_opc, _name, _sz, _dst, _src, _ext, _dpos, _spos, _epos) \
    { _opc, Entry::Flags::create(_dst, _src, _ext, _dpos, _spos, _epos, _sz), _name }
#define E2(_opc, _name, _sz, _dst, _src, _dpos, _spos) \
    E3(_opc, _name, _sz, _dst, _src, M_NONE, _dpos, _spos, P_NONE)
#define E1(_opc, _name, _sz, _dst, _dpos) E2(_opc, _name, _sz, _dst, M_NONE, _dpos, P_NONE)
#define E0(_opc, _name, _sz) E1(_opc, _name, _sz, M_NONE, P_NONE)
#define F2(_opc, _name, _sz, _dst, _src, _dpos, _spos) \
    { _opc, Entry::Flags::fpuInst(_dst, _src, _dpos, _spos, _sz), _name }
#define F1(_opc, _name, _sz, _dst, _dpos) F2(_opc, _name, _sz, _dst, M_NONE, _dpos, P_NONE)
#define F0(_opc, _name, _sz) F1(_opc, _name, _sz, M_NONE, P_NONE)
#define S2(_opc, _name, _sz, _dst, _src) \
    { _opc, Entry::Flags::strInst(_dst, _src, _sz), _name }
#define S1(_opc, _name, _sz, _dst) S2(_opc, _name, _sz, _dst, M_NONE)
#define S0(_opc, _name, _sz) S1(_opc, _name, _sz, M_NONE)

// clang-format off
static constexpr Entry TABLE_00[] PROGMEM = {
    E2(0xA0, TEXT_MOV,    SZ_BYTE, M_AL,   M_BDIR, P_NONE, P_OPR),
    E2(0xA1, TEXT_MOV,    SZ_WORD, M_AX,   M_WDIR, P_NONE, P_OPR),
    E2(0xA2, TEXT_MOV,    SZ_BYTE, M_BDIR, M_AL,   P_OPR,  P_NONE),
    E2(0xA3, TEXT_MOV,    SZ_WORD, M_WDIR, M_AX,   P_OPR,  P_NONE),
    E0(0x90, TEXT_NOP,    SZ_NONE),
    E2(0x90, TEXT_XCHG,   SZ_WORD, M_AX,   M_WREG, P_NONE, P_OREG),
    E2(0x90, TEXT_XCHG,   SZ_WORD, M_WREG, M_AX,   P_OREG, P_NONE),
    E2(0x04, TEXT_ADD,    SZ_BYTE, M_AL,   M_IMM,  P_NONE, P_OPR),
    E2(0x05, TEXT_ADD,    SZ_WORD, M_AX,   M_IMM,  P_NONE, P_OPR),
    E2(0x0C, TEXT_OR,     SZ_BYTE, M_AL,   M_IMM,  P_NONE, P_OPR),
    E2(0x0D, TEXT_OR,     SZ_WORD, M_AX,   M_IMM,  P_NONE, P_OPR),
    E2(0x02, TEXT_ADD,    SZ_BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    E2(0x00, TEXT_ADD,    SZ_BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E2(0x03, TEXT_ADD,    SZ_WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    E2(0x01, TEXT_ADD,    SZ_WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    E1(0x06, TEXT_PUSH,   SZ_WORD, M_SREG, P_OSEG),
    E1(0x0F, TEXT_POP,    SZ_WORD, M_CS,   P_NONE),
    E1(0x07, TEXT_POP,    SZ_WORD, M_SREG, P_OSEG),
    E2(0x0A, TEXT_OR,     SZ_BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    E2(0x08, TEXT_OR,     SZ_BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E2(0x0B, TEXT_OR,     SZ_WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    E2(0x09, TEXT_OR,     SZ_WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    E2(0x12, TEXT_ADC,    SZ_BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    E2(0x10, TEXT_ADC,    SZ_BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E2(0x13, TEXT_ADC,    SZ_WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    E2(0x11, TEXT_ADC,    SZ_WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    E2(0x14, TEXT_ADC,    SZ_BYTE, M_AL,   M_IMM,  P_NONE, P_OPR),
    E2(0x15, TEXT_ADC,    SZ_WORD, M_AX,   M_IMM,  P_NONE, P_OPR),
    E2(0x1A, TEXT_SBB,    SZ_BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    E2(0x18, TEXT_SBB,    SZ_BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E2(0x1B, TEXT_SBB,    SZ_WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    E2(0x19, TEXT_SBB,    SZ_WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    E2(0x1C, TEXT_SBB,    SZ_BYTE, M_AL,   M_IMM,  P_NONE, P_OPR),
    E2(0x1D, TEXT_SBB,    SZ_WORD, M_AX,   M_IMM,  P_NONE, P_OPR),
    E2(0x22, TEXT_AND,    SZ_BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    E2(0x20, TEXT_AND,    SZ_BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E2(0x23, TEXT_AND,    SZ_WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    E2(0x21, TEXT_AND,    SZ_WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    E2(0x24, TEXT_AND,    SZ_BYTE, M_AL,   M_IMM,  P_NONE, P_OPR),
    E2(0x25, TEXT_AND,    SZ_WORD, M_AX,   M_IMM,  P_NONE, P_OPR),
    E0(0x26, TEXT_SEGES,  SZ_BYTE),
    E0(0x27, TEXT_DAA,    SZ_BYTE),
    E2(0x2A, TEXT_SUB,    SZ_BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    E2(0x28, TEXT_SUB,    SZ_BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E2(0x2B, TEXT_SUB,    SZ_WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    E2(0x29, TEXT_SUB,    SZ_WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    E2(0x2C, TEXT_SUB,    SZ_BYTE, M_AL,   M_IMM,  P_NONE, P_OPR),
    E2(0x2D, TEXT_SUB,    SZ_WORD, M_AX,   M_IMM,  P_NONE, P_OPR),
    E0(0x2E, TEXT_SEGCS,  SZ_BYTE),
    E0(0x2F, TEXT_DAS,    SZ_BYTE),
    E2(0x32, TEXT_XOR,    SZ_BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    E2(0x30, TEXT_XOR,    SZ_BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E2(0x33, TEXT_XOR,    SZ_WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    E2(0x31, TEXT_XOR,    SZ_WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    E2(0x34, TEXT_XOR,    SZ_BYTE, M_AL,   M_IMM,  P_NONE, P_OPR),
    E2(0x35, TEXT_XOR,    SZ_WORD, M_AX,   M_IMM,  P_NONE, P_OPR),
    E0(0x36, TEXT_SEGSS,  SZ_BYTE),
    E0(0x37, TEXT_AAA,    SZ_BYTE),
    E2(0x3A, TEXT_CMP,    SZ_BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    E2(0x38, TEXT_CMP,    SZ_BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E2(0x3B, TEXT_CMP,    SZ_WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    E2(0x39, TEXT_CMP,    SZ_WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    E2(0x3C, TEXT_CMP,    SZ_BYTE, M_AL,   M_IMM,  P_NONE, P_OPR),
    E2(0x3D, TEXT_CMP,    SZ_WORD, M_AX,   M_IMM,  P_NONE, P_OPR),
    E0(0x3E, TEXT_SEGDS,  SZ_BYTE),
    E0(0x3F, TEXT_AAS,    SZ_BYTE),
    E1(0x40, TEXT_INC,    SZ_WORD, M_WREG, P_OREG),
    E1(0x48, TEXT_DEC,    SZ_WORD, M_WREG, P_OREG),
    E1(0x50, TEXT_PUSH,   SZ_WORD, M_WREG, P_OREG),
    E1(0x58, TEXT_POP,    SZ_WORD, M_WREG, P_OREG),
    E1(0x70, TEXT_JO,     SZ_NONE, M_REL8, P_OPR),
    E1(0x71, TEXT_JNO,    SZ_NONE, M_REL8, P_OPR),
    E1(0x72, TEXT_JB,     SZ_NONE, M_REL8, P_OPR),
    E1(0x72, TEXT_JC,     SZ_NONE, M_REL8, P_OPR),
    E1(0x72, TEXT_JNAE,   SZ_NONE, M_REL8, P_OPR),
    E1(0x73, TEXT_JAE,    SZ_NONE, M_REL8, P_OPR),
    E1(0x73, TEXT_JNB,    SZ_NONE, M_REL8, P_OPR),
    E1(0x73, TEXT_JNC,    SZ_NONE, M_REL8, P_OPR),
    E1(0x74, TEXT_JE,     SZ_NONE, M_REL8, P_OPR),
    E1(0x74, TEXT_JZ,     SZ_NONE, M_REL8, P_OPR),
    E1(0x75, TEXT_JNE,    SZ_NONE, M_REL8, P_OPR),
    E1(0x75, TEXT_JNZ,    SZ_NONE, M_REL8, P_OPR),
    E1(0x76, TEXT_JBE,    SZ_NONE, M_REL8, P_OPR),
    E1(0x76, TEXT_JNA,    SZ_NONE, M_REL8, P_OPR),
    E1(0x77, TEXT_JA,     SZ_NONE, M_REL8, P_OPR),
    E1(0x77, TEXT_JNBE,   SZ_NONE, M_REL8, P_OPR),
    E1(0x78, TEXT_JS,     SZ_NONE, M_REL8, P_OPR),
    E1(0x79, TEXT_JNS,    SZ_NONE, M_REL8, P_OPR),
    E1(0x7A, TEXT_JPE,    SZ_NONE, M_REL8, P_OPR),
    E1(0x7A, TEXT_JP,     SZ_NONE, M_REL8, P_OPR),
    E1(0x7B, TEXT_JPO,    SZ_NONE, M_REL8, P_OPR),
    E1(0x7B, TEXT_JNP,    SZ_NONE, M_REL8, P_OPR),
    E1(0x7C, TEXT_JL,     SZ_NONE, M_REL8, P_OPR),
    E1(0x7C, TEXT_JNGE,   SZ_NONE, M_REL8, P_OPR),
    E1(0x7D, TEXT_JGE,    SZ_NONE, M_REL8, P_OPR),
    E1(0x7D, TEXT_JNL,    SZ_NONE, M_REL8, P_OPR),
    E1(0x7E, TEXT_JLE,    SZ_NONE, M_REL8, P_OPR),
    E1(0x7E, TEXT_JNG,    SZ_NONE, M_REL8, P_OPR),
    E1(0x7F, TEXT_JG,     SZ_NONE, M_REL8, P_OPR),
    E1(0x7F, TEXT_JNLE,   SZ_NONE, M_REL8, P_OPR),
    E2(0x84, TEXT_TEST,   SZ_BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    E2(0x84, TEXT_TEST,   SZ_BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E2(0x85, TEXT_TEST,   SZ_WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    E2(0x85, TEXT_TEST,   SZ_WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    E2(0x86, TEXT_XCHG,   SZ_BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    E2(0x86, TEXT_XCHG,   SZ_BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E2(0x87, TEXT_XCHG,   SZ_WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    E2(0x87, TEXT_XCHG,   SZ_WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    E2(0x8A, TEXT_MOV,    SZ_BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    E2(0x88, TEXT_MOV,    SZ_BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E2(0x8B, TEXT_MOV,    SZ_WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    E2(0x89, TEXT_MOV,    SZ_WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    E2(0x8C, TEXT_MOV,    SZ_WORD, M_WMOD, M_SREG, P_MOD,  P_REG),
    E2(0x8D, TEXT_LEA,    SZ_WORD, M_WREG, M_BMEM, P_REG,  P_MOD),
    E2(0x8D, TEXT_LEA,    SZ_WORD, M_WREG, M_WMEM, P_REG,  P_MOD),
    E2(0x8E, TEXT_MOV,    SZ_WORD, M_SREG, M_WMOD, P_REG,  P_MOD),
    E0(0x98, TEXT_CBW,    SZ_BYTE),
    E0(0x99, TEXT_CWD,    SZ_WORD),
    E1(0x9A, TEXT_CALLF,  SZ_NONE, M_FAR,  P_OPR),
    E0(0x9B, TEXT_WAIT,   SZ_NONE),
    E0(0x9C, TEXT_PUSHF,  SZ_WORD),
    E0(0x9D, TEXT_POPF,   SZ_WORD),
    E0(0x9E, TEXT_SAHF,   SZ_BYTE),
    E0(0x9F, TEXT_LAHF,   SZ_BYTE),
    S0(0xA4, TEXT_MOVSB,  SZ_BYTE),
    S0(0xA5, TEXT_MOVSW,  SZ_WORD),
    S2(0xA4, TEXT_MOVSB,  SZ_BYTE, M_BMEM, M_BMEM),
    S2(0xA5, TEXT_MOVSW,  SZ_WORD, M_WMEM, M_WMEM),
    S2(0xA4, TEXT_MOVS,   SZ_BYTE, M_BMEM, M_BMEM),
    S2(0xA5, TEXT_MOVS,   SZ_WORD, M_WMEM, M_WMEM),
    S0(0xA6, TEXT_CMPSB,  SZ_BYTE),
    S0(0xA7, TEXT_CMPSW,  SZ_WORD),
    S2(0xA6, TEXT_CMPSB,  SZ_BYTE, M_BMEM, M_BMEM),
    S2(0xA7, TEXT_CMPSW,  SZ_WORD, M_WMEM, M_WMEM),
    S2(0xA6, TEXT_CMPS,   SZ_BYTE, M_BMEM, M_BMEM),
    S2(0xA7, TEXT_CMPS,   SZ_WORD, M_WMEM, M_WMEM),
    E2(0xA8, TEXT_TEST,   SZ_BYTE, M_AL,   M_IMM,  P_NONE, P_OPR),
    E2(0xA9, TEXT_TEST,   SZ_WORD, M_AX,   M_IMM,  P_NONE, P_OPR),
    S0(0xAA, TEXT_STOSB,  SZ_BYTE),
    S0(0xAB, TEXT_STOSW,  SZ_WORD),
    S1(0xAA, TEXT_STOSB,  SZ_NONE, M_BMEM),
    S1(0xAB, TEXT_STOSW,  SZ_NONE, M_WMEM),
    S1(0xAA, TEXT_STOS,   SZ_BYTE, M_BMEM),
    S1(0xAB, TEXT_STOS,   SZ_WORD, M_WMEM),
    S0(0xAC, TEXT_LODSB,  SZ_BYTE),
    S0(0xAD, TEXT_LODSW,  SZ_WORD),
    S1(0xAC, TEXT_LODSB,  SZ_NONE, M_BMEM),
    S1(0xAD, TEXT_LODSW,  SZ_NONE, M_WMEM),
    S1(0xAC, TEXT_LODS,   SZ_BYTE, M_BMEM),
    S1(0xAD, TEXT_LODS,   SZ_WORD, M_WMEM),
    S0(0xAE, TEXT_SCASB,  SZ_BYTE),
    S0(0xAF, TEXT_SCASW,  SZ_WORD),
    S1(0xAE, TEXT_SCASB,  SZ_NONE, M_BMEM),
    S1(0xAF, TEXT_SCASW,  SZ_NONE, M_WMEM),
    S1(0xAE, TEXT_SCAS,   SZ_BYTE, M_BMEM),
    S1(0xAF, TEXT_SCAS,   SZ_WORD, M_WMEM),
    E2(0xB0, TEXT_MOV,    SZ_BYTE, M_BREG, M_IMM,  P_OREG, P_OPR),
    E2(0xB8, TEXT_MOV,    SZ_WORD, M_WREG, M_IMM,  P_OREG, P_OPR),
    E1(0xC2, TEXT_RET,    SZ_WORD, M_UI16, P_OPR),
    E0(0xC3, TEXT_RET,    SZ_NONE),
    E2(0xC4, TEXT_LES,    SZ_WORD, M_WREG, M_WMEM, P_REG,  P_MOD),
    E2(0xC5, TEXT_LDS,    SZ_WORD, M_WREG, M_WMEM, P_REG,  P_MOD),
    E1(0xCA, TEXT_RETF,   SZ_WORD, M_UI16, P_OPR),
    E0(0xCB, TEXT_RETF,   SZ_NONE),
    E1(0xCC, TEXT_INT,    SZ_NONE, M_VAL3, P_NONE),
    E1(0xCD, TEXT_INT,    SZ_BYTE, M_IMM,  P_OPR),
    E0(0xCE, TEXT_INTO,   SZ_NONE),
    E0(0xCF, TEXT_IRET,   SZ_NONE),
    E0(0xD7, TEXT_XLAT,   SZ_NONE),
    E1(0xE0, TEXT_LOOPNE, SZ_NONE, M_REL8, P_OPR),
    E1(0xE0, TEXT_LOOPNZ, SZ_NONE, M_REL8, P_OPR),
    E1(0xE1, TEXT_LOOPE,  SZ_NONE, M_REL8, P_OPR),
    E1(0xE1, TEXT_LOOPZ,  SZ_NONE, M_REL8, P_OPR),
    E1(0xE2, TEXT_LOOP,   SZ_NONE, M_REL8, P_OPR),
    E1(0xE3, TEXT_JCXZ,   SZ_NONE, M_REL8, P_OPR),
    E2(0xE4, TEXT_IN,     SZ_BYTE, M_AL,   M_IOA,  P_NONE, P_OPR),
    E2(0xE5, TEXT_IN,     SZ_WORD, M_AX,   M_IOA,  P_NONE, P_OPR),
    E2(0xE6, TEXT_OUT,    SZ_BYTE, M_IOA,  M_AL,   P_OPR,  P_NONE),
    E2(0xE7, TEXT_OUT,    SZ_WORD, M_IOA,  M_AX,   P_OPR,  P_NONE),
    E1(0xE8, TEXT_CALL,   SZ_NONE, M_REL,  P_OPR),
    E1(0xEB, TEXT_JMP,    SZ_NONE, M_REL8, P_OPR),
    E1(0xE9, TEXT_JMP,    SZ_NONE, M_REL,  P_OPR),
    E1(0xEA, TEXT_JMPF,   SZ_NONE, M_FAR,  P_OPR),
    E2(0xEC, TEXT_IN,     SZ_BYTE, M_AL,   M_DX,   P_NONE, P_NONE),
    E2(0xED, TEXT_IN,     SZ_WORD, M_AX,   M_DX,   P_NONE, P_NONE),
    E2(0xEE, TEXT_OUT,    SZ_BYTE, M_DX,   M_AL,   P_NONE, P_NONE),
    E2(0xEF, TEXT_OUT,    SZ_WORD, M_DX,   M_AX,   P_NONE, P_NONE),
    E0(0xF0, TEXT_LOCK,   SZ_NONE),
    E1(0xF2, TEXT_REPNE,  SZ_NONE, M_ISTR, P_OPR),
    E1(0xF2, TEXT_REPNZ,  SZ_NONE, M_ISTR, P_OPR),
    E1(0xF3, TEXT_REP,    SZ_NONE, M_ISTR, P_OPR),
    E1(0xF3, TEXT_REPE,   SZ_NONE, M_ISTR, P_OPR),
    E1(0xF3, TEXT_REPZ,   SZ_NONE, M_ISTR, P_OPR),
    E0(0xF2, TEXT_REPNE,  SZ_NONE),
    E0(0xF2, TEXT_REPNZ,  SZ_NONE),
    E0(0xF3, TEXT_REP,    SZ_NONE),
    E0(0xF3, TEXT_REPE,   SZ_NONE),
    E0(0xF3, TEXT_REPZ,   SZ_NONE),
    E0(0xF4, TEXT_HLT,    SZ_NONE),
    E0(0xF5, TEXT_CMC,    SZ_NONE),
    E0(0xF8, TEXT_CLC,    SZ_NONE),
    E0(0xF9, TEXT_STC,    SZ_NONE),
    E0(0xFA, TEXT_CLI,    SZ_NONE),
    E0(0xFB, TEXT_STI,    SZ_NONE),
    E0(0xFC, TEXT_CLD,    SZ_NONE),
    E0(0xFD, TEXT_STD,    SZ_NONE),
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
    E2(000, TEXT_ADD, SZ_BYTE, M_BMOD, M_IMM, P_OMOD, P_OPR),
    E2(010, TEXT_OR,  SZ_BYTE, M_BMOD, M_IMM, P_OMOD, P_OPR),
    E2(020, TEXT_ADC, SZ_BYTE, M_BMOD, M_IMM, P_OMOD, P_OPR),
    E2(030, TEXT_SBB, SZ_BYTE, M_BMOD, M_IMM, P_OMOD, P_OPR),
    E2(040, TEXT_AND, SZ_BYTE, M_BMOD, M_IMM, P_OMOD, P_OPR),
    E2(050, TEXT_SUB, SZ_BYTE, M_BMOD, M_IMM, P_OMOD, P_OPR),
    E2(060, TEXT_XOR, SZ_BYTE, M_BMOD, M_IMM, P_OMOD, P_OPR),
    E2(070, TEXT_CMP, SZ_BYTE, M_BMOD, M_IMM, P_OMOD, P_OPR),
};

static constexpr uint8_t INDEX_8X[] PROGMEM = {
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
    E2(000, TEXT_ADD, SZ_WORD, M_WMOD, M_IMM, P_OMOD, P_OPR),
    E2(010, TEXT_OR,  SZ_WORD, M_WMOD, M_IMM, P_OMOD, P_OPR),
    E2(020, TEXT_ADC, SZ_WORD, M_WMOD, M_IMM, P_OMOD, P_OPR),
    E2(030, TEXT_SBB, SZ_WORD, M_WMOD, M_IMM, P_OMOD, P_OPR),
    E2(040, TEXT_AND, SZ_WORD, M_WMOD, M_IMM, P_OMOD, P_OPR),
    E2(050, TEXT_SUB, SZ_WORD, M_WMOD, M_IMM, P_OMOD, P_OPR),
    E2(060, TEXT_XOR, SZ_WORD, M_WMOD, M_IMM, P_OMOD, P_OPR),
    E2(070, TEXT_CMP, SZ_WORD, M_WMOD, M_IMM, P_OMOD, P_OPR),
};

static constexpr Entry TABLE_83[] PROGMEM = {
    E2(000, TEXT_ADD, SZ_WORD, M_WMOD, M_IMM8, P_OMOD, P_OPR),
    E2(010, TEXT_OR,  SZ_WORD, M_WMOD, M_IMM8, P_OMOD, P_OPR),
    E2(020, TEXT_ADC, SZ_WORD, M_WMOD, M_IMM8, P_OMOD, P_OPR),
    E2(030, TEXT_SBB, SZ_WORD, M_WMOD, M_IMM8, P_OMOD, P_OPR),
    E2(040, TEXT_AND, SZ_WORD, M_WMOD, M_IMM8, P_OMOD, P_OPR),
    E2(050, TEXT_SUB, SZ_WORD, M_WMOD, M_IMM8, P_OMOD, P_OPR),
    E2(060, TEXT_XOR, SZ_WORD, M_WMOD, M_IMM8, P_OMOD, P_OPR),
    E2(070, TEXT_CMP, SZ_WORD, M_WMOD, M_IMM8, P_OMOD, P_OPR),
};

static constexpr Entry TABLE_8F[] PROGMEM = {
    E1(000, TEXT_POP, SZ_NONE, M_WMOD, P_OMOD),
};

static constexpr uint8_t INDEX_8F[] PROGMEM = {
      0,  // TEXT_POP
};

static constexpr Entry TABLE_C6[] PROGMEM = {
    E2(000, TEXT_MOV, SZ_BYTE, M_BMOD, M_IMM, P_OMOD, P_OPR),
};

static constexpr uint8_t INDEX_C6[] PROGMEM = {
      0,  // TEXT_MOV
};

static constexpr Entry TABLE_C7[] PROGMEM = {
    E2(000, TEXT_MOV, SZ_WORD, M_WMOD, M_IMM, P_OMOD, P_OPR),
};

static constexpr uint8_t INDEX_C7[] PROGMEM = {
      0,  // TEXT_MOV
};

static constexpr Entry TABLE_D0[] PROGMEM = {
    E2(000, TEXT_ROL, SZ_BYTE, M_BMOD, M_VAL1, P_OMOD, P_NONE),
    E2(010, TEXT_ROR, SZ_BYTE, M_BMOD, M_VAL1, P_OMOD, P_NONE),
    E2(020, TEXT_RCL, SZ_BYTE, M_BMOD, M_VAL1, P_OMOD, P_NONE),
    E2(030, TEXT_RCR, SZ_BYTE, M_BMOD, M_VAL1, P_OMOD, P_NONE),
    E2(040, TEXT_SHL, SZ_BYTE, M_BMOD, M_VAL1, P_OMOD, P_NONE),
    E2(040, TEXT_SAL, SZ_BYTE, M_BMOD, M_VAL1, P_OMOD, P_NONE),
    E2(050, TEXT_SHR, SZ_BYTE, M_BMOD, M_VAL1, P_OMOD, P_NONE),
    E2(070, TEXT_SAR, SZ_BYTE, M_BMOD, M_VAL1, P_OMOD, P_NONE),
};

static constexpr uint8_t INDEX_DX[] PROGMEM = {
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
    E2(000, TEXT_ROL, SZ_WORD, M_WMOD, M_VAL1, P_OMOD, P_NONE),
    E2(010, TEXT_ROR, SZ_WORD, M_WMOD, M_VAL1, P_OMOD, P_NONE),
    E2(020, TEXT_RCL, SZ_WORD, M_WMOD, M_VAL1, P_OMOD, P_NONE),
    E2(030, TEXT_RCR, SZ_WORD, M_WMOD, M_VAL1, P_OMOD, P_NONE),
    E2(040, TEXT_SHL, SZ_WORD, M_WMOD, M_VAL1, P_OMOD, P_NONE),
    E2(040, TEXT_SAL, SZ_WORD, M_WMOD, M_VAL1, P_OMOD, P_NONE),
    E2(050, TEXT_SHR, SZ_WORD, M_WMOD, M_VAL1, P_OMOD, P_NONE),
    E2(070, TEXT_SAR, SZ_WORD, M_WMOD, M_VAL1, P_OMOD, P_NONE),
};

static constexpr Entry TABLE_D2[] PROGMEM = {
    E2(000, TEXT_ROL, SZ_BYTE, M_BMOD, M_CL, P_OMOD, P_NONE),
    E2(010, TEXT_ROR, SZ_BYTE, M_BMOD, M_CL, P_OMOD, P_NONE),
    E2(020, TEXT_RCL, SZ_BYTE, M_BMOD, M_CL, P_OMOD, P_NONE),
    E2(030, TEXT_RCR, SZ_BYTE, M_BMOD, M_CL, P_OMOD, P_NONE),
    E2(040, TEXT_SHL, SZ_BYTE, M_BMOD, M_CL, P_OMOD, P_NONE),
    E2(040, TEXT_SAL, SZ_BYTE, M_BMOD, M_CL, P_OMOD, P_NONE),
    E2(050, TEXT_SHR, SZ_BYTE, M_BMOD, M_CL, P_OMOD, P_NONE),
    E2(070, TEXT_SAR, SZ_BYTE, M_BMOD, M_CL, P_OMOD, P_NONE),
};

static constexpr Entry TABLE_D3[] PROGMEM = {
    E2(000, TEXT_ROL, SZ_WORD, M_WMOD, M_CL, P_OMOD, P_NONE),
    E2(010, TEXT_ROR, SZ_WORD, M_WMOD, M_CL, P_OMOD, P_NONE),
    E2(020, TEXT_RCL, SZ_WORD, M_WMOD, M_CL, P_OMOD, P_NONE),
    E2(030, TEXT_RCR, SZ_WORD, M_WMOD, M_CL, P_OMOD, P_NONE),
    E2(040, TEXT_SHL, SZ_WORD, M_WMOD, M_CL, P_OMOD, P_NONE),
    E2(040, TEXT_SAL, SZ_WORD, M_WMOD, M_CL, P_OMOD, P_NONE),
    E2(050, TEXT_SHR, SZ_WORD, M_WMOD, M_CL, P_OMOD, P_NONE),
    E2(070, TEXT_SAR, SZ_WORD, M_WMOD, M_CL, P_OMOD, P_NONE),
};

static constexpr Entry TABLE_D4[] PROGMEM = {
    E0(0x0A, TEXT_AAM, SZ_BYTE),
};

static constexpr uint8_t INDEX_D4[] PROGMEM = {
      0,  // TEXT_AAM
};

static constexpr Entry TABLE_D5[] PROGMEM = {
    E0(0x0A, TEXT_AAD, SZ_BYTE),
};

static constexpr uint8_t INDEX_D5[] PROGMEM = {
      0,  // TEXT_AAD
};

static constexpr Entry TABLE_F6[] PROGMEM = {
    E2(000, TEXT_TEST, SZ_BYTE, M_BMOD, M_IMM, P_OMOD, P_OPR),
    E1(020, TEXT_NOT,  SZ_BYTE, M_BMOD, P_OMOD),
    E1(030, TEXT_NEG,  SZ_BYTE, M_BMOD, P_OMOD),
    E1(040, TEXT_MUL,  SZ_BYTE, M_BMOD, P_OMOD),
    E1(050, TEXT_IMUL, SZ_BYTE, M_BMOD, P_OMOD),
    E1(060, TEXT_DIV,  SZ_BYTE, M_BMOD, P_OMOD),
    E1(070, TEXT_IDIV, SZ_BYTE, M_BMOD, P_OMOD),
};

static constexpr uint8_t INDEX_FX[] PROGMEM = {
      5,  // TEXT_DIV
      6,  // TEXT_IDIV
      4,  // TEXT_IMUL
      3,  // TEXT_MUL
      2,  // TEXT_NEG
      1,  // TEXT_NOT
      0,  // TEXT_TEST
};

static constexpr Entry TABLE_F7[] PROGMEM = {
    E2(000, TEXT_TEST, SZ_WORD, M_WMOD, M_IMM, P_OMOD, P_OPR),
    E1(020, TEXT_NOT,  SZ_WORD, M_WMOD, P_OMOD),
    E1(030, TEXT_NEG,  SZ_WORD, M_WMOD, P_OMOD),
    E1(040, TEXT_MUL,  SZ_WORD, M_WMOD, P_OMOD),
    E1(050, TEXT_IMUL, SZ_WORD, M_WMOD, P_OMOD),
    E1(060, TEXT_DIV,  SZ_WORD, M_WMOD, P_OMOD),
    E1(070, TEXT_IDIV, SZ_WORD, M_WMOD, P_OMOD),
};

static constexpr Entry TABLE_FE[] PROGMEM = {
    E1(000, TEXT_INC, SZ_BYTE, M_BMOD, P_OMOD),
    E1(010, TEXT_DEC, SZ_BYTE, M_BMOD, P_OMOD),
};

static constexpr uint8_t INDEX_FE[] PROGMEM = {
      1,  // TEXT_DEC
      0,  // TEXT_INC
};

static constexpr Entry TABLE_FF[] PROGMEM = {
    E1(000, TEXT_INC,   SZ_WORD, M_WMOD, P_OMOD),
    E1(010, TEXT_DEC,   SZ_WORD, M_WMOD, P_OMOD),
    E1(020, TEXT_CALL,  SZ_NONE, M_WMOD, P_OMOD),
    E1(030, TEXT_CALLF, SZ_NONE, M_WMEM, P_OMOD),
    E1(040, TEXT_JMP,   SZ_NONE, M_WMOD, P_OMOD),
    E1(050, TEXT_JMPF,  SZ_NONE, M_WMEM, P_OMOD),
    E1(060, TEXT_PUSH,  SZ_NONE, M_WMOD, P_OMOD),
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
    E0(0x60, TEXT_PUSHA, SZ_NONE),
    E0(0x61, TEXT_POPA,  SZ_NONE),
    E2(0x62, TEXT_BOUND, SZ_WORD, M_WREG, M_WMEM, P_REG,  P_MOD),
    E1(0x6A, TEXT_PUSH,  SZ_BYTE, M_IMM8, P_OPR), // M_IMM8
    E3(0x6B, TEXT_IMUL,  SZ_WORD, M_WREG, M_WMOD, M_IMM8, P_REG, P_MOD, P_OPR),
    E2(0x6B, TEXT_IMUL,  SZ_WORD, M_WREG, M_IMM8, P_MREG, P_OPR),  // M_IMM8
    E1(0x68, TEXT_PUSH,  SZ_WORD, M_IMM,  P_OPR), // M_IMM
    E3(0x69, TEXT_IMUL,  SZ_WORD, M_WREG, M_WMOD, M_IMM,  P_REG, P_MOD, P_OPR),
    E2(0x69, TEXT_IMUL,  SZ_WORD, M_WREG, M_IMM,  P_MREG, P_OPR),  // M_IMM
    S0(0x6C, TEXT_INSB,  SZ_BYTE),
    S2(0x6C, TEXT_INSB,  SZ_BYTE, M_MEM,  M_DX),
    S0(0x6D, TEXT_INSW,  SZ_WORD),
    S2(0x6D, TEXT_INSW,  SZ_WORD, M_MEM,  M_DX),
    S2(0x6C, TEXT_INS,   SZ_BYTE, M_BMEM, M_DX),
    S2(0x6D, TEXT_INS,   SZ_WORD, M_WMEM, M_DX),
    S0(0x6E, TEXT_OUTSB, SZ_BYTE),
    S2(0x6E, TEXT_OUTSB, SZ_BYTE, M_DX,   M_BMEM),
    S0(0x6F, TEXT_OUTSW, SZ_WORD),
    S2(0x6F, TEXT_OUTSW, SZ_WORD, M_DX,   M_WMEM),
    S2(0x6E, TEXT_OUTS,  SZ_BYTE, M_DX,   M_BMEM),
    S2(0x6F, TEXT_OUTS,  SZ_WORD, M_DX,   M_WMEM),
    E2(0xC8, TEXT_ENTER, SZ_NONE, M_UI16, M_UI8,  P_OPR,  P_OPR),
    E0(0xC9, TEXT_LEAVE, SZ_NONE),
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
    E2(000, TEXT_ROL, SZ_BYTE, M_BMOD, M_BIT, P_OMOD, P_OPR),
    E2(010, TEXT_ROR, SZ_BYTE, M_BMOD, M_BIT, P_OMOD, P_OPR),
    E2(020, TEXT_RCL, SZ_BYTE, M_BMOD, M_BIT, P_OMOD, P_OPR),
    E2(030, TEXT_RCR, SZ_BYTE, M_BMOD, M_BIT, P_OMOD, P_OPR),
    E2(040, TEXT_SHL, SZ_BYTE, M_BMOD, M_BIT, P_OMOD, P_OPR),
    E2(040, TEXT_SAL, SZ_BYTE, M_BMOD, M_BIT, P_OMOD, P_OPR),
    E2(050, TEXT_SHR, SZ_BYTE, M_BMOD, M_BIT, P_OMOD, P_OPR),
    E2(070, TEXT_SAR, SZ_BYTE, M_BMOD, M_BIT, P_OMOD, P_OPR),
};

static constexpr uint8_t INDEX_CX[] PROGMEM = {
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
    E2(000, TEXT_ROL, SZ_WORD, M_WMOD, M_BIT, P_OMOD, P_OPR),
    E2(010, TEXT_ROR, SZ_WORD, M_WMOD, M_BIT, P_OMOD, P_OPR),
    E2(020, TEXT_RCL, SZ_WORD, M_WMOD, M_BIT, P_OMOD, P_OPR),
    E2(030, TEXT_RCR, SZ_WORD, M_WMOD, M_BIT, P_OMOD, P_OPR),
    E2(040, TEXT_SHL, SZ_WORD, M_WMOD, M_BIT, P_OMOD, P_OPR),
    E2(040, TEXT_SAL, SZ_WORD, M_WMOD, M_BIT, P_OMOD, P_OPR),
    E2(050, TEXT_SHR, SZ_WORD, M_WMOD, M_BIT, P_OMOD, P_OPR),
    E2(070, TEXT_SAR, SZ_WORD, M_WMOD, M_BIT, P_OMOD, P_OPR),
};

// V30

static constexpr Entry V30TABLE_00[] PROGMEM = {
    E1(0x64, TEXT_REPNC,  SZ_NONE, M_ISTR, P_OPR),
    E0(0x64, TEXT_REPNC,  SZ_NONE),
    E1(0x65, TEXT_REPC,   SZ_NONE, M_ISTR, P_OPR),
    E0(0x65, TEXT_REPC,   SZ_NONE),
};

static constexpr uint8_t V30INDEX_00[] PROGMEM = {
      2,  // TEXT_REPC
      3,  // TEXT_REPC
      0,  // TEXT_REPNC
      1,  // TEXT_REPNC
};

static constexpr Entry V30TABLE_0F[] PROGMEM = {
    E1(0xFF, TEXT_BRKEM,  SZ_BYTE, M_IMM,  P_OPR),
    S0(0x20, TEXT_ADD4S,  SZ_BYTE),
    S2(0x20, TEXT_ADD4S,  SZ_BYTE, M_BMEM, M_BMEM),
    S0(0x26, TEXT_CMP4S,  SZ_BYTE),
    S2(0x26, TEXT_CMP4S,  SZ_BYTE, M_BMEM, M_BMEM),
    S0(0x22, TEXT_SUB4S,  SZ_BYTE),
    S2(0x22, TEXT_SUB4S,  SZ_BYTE, M_BMEM, M_BMEM),
    E1(0x28, TEXT_ROL4,   SZ_BYTE, M_BMOD, P_MOD),
    E1(0x2A, TEXT_ROR4,   SZ_BYTE, M_BMOD, P_MOD),
    E2(0x31, TEXT_INS,    SZ_WORD, M_BREG, M_BREG, P_MOD,  P_REG),
    E2(0x39, TEXT_INS,    SZ_WORD, M_BREG, M_BIT,  P_MOD,  P_OPR),
    E2(0x33, TEXT_EXT,    SZ_WORD, M_BREG, M_BREG, P_MOD,  P_REG),
    E2(0x3B, TEXT_EXT,    SZ_WORD, M_BREG, M_BIT,  P_MOD,  P_OPR),
    E2(0x12, TEXT_CLR1,   SZ_BYTE, M_BMOD, M_CL,   P_MOD,  P_NONE),
    E2(0x13, TEXT_CLR1,   SZ_WORD, M_WMOD, M_CL,   P_MOD,  P_NONE),
    E2(0x1A, TEXT_CLR1,   SZ_BYTE, M_BMOD, M_BIT,  P_MOD,  P_OPR),
    E2(0x1B, TEXT_CLR1,   SZ_WORD, M_WMOD, M_BIT,  P_MOD,  P_OPR),
    E2(0x14, TEXT_SET1,   SZ_BYTE, M_BMOD, M_CL,   P_MOD,  P_NONE),
    E2(0x15, TEXT_SET1,   SZ_WORD, M_WMOD, M_CL,   P_MOD,  P_NONE),
    E2(0x1C, TEXT_SET1,   SZ_BYTE, M_BMOD, M_BIT,  P_MOD,  P_OPR),
    E2(0x1D, TEXT_SET1,   SZ_WORD, M_WMOD, M_BIT,  P_MOD,  P_OPR),
    E2(0x16, TEXT_NOT1,   SZ_BYTE, M_BMOD, M_CL,   P_MOD,  P_NONE),
    E2(0x17, TEXT_NOT1,   SZ_WORD, M_WMOD, M_CL,   P_MOD,  P_NONE),
    E2(0x1E, TEXT_NOT1,   SZ_BYTE, M_BMOD, M_BIT,  P_MOD,  P_OPR),
    E2(0x1F, TEXT_NOT1,   SZ_WORD, M_WMOD, M_BIT,  P_MOD,  P_OPR),
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

static constexpr Entry TABLE_D8[] PROGMEM = {
    F2(0xC0, TEXT_FADD,   SZ_NONE,  M_ST0,  M_STI,  P_NONE, P_OREG),
    F2(0xC8, TEXT_FMUL,   SZ_NONE,  M_ST0,  M_STI,  P_NONE, P_OREG),
    F1(0xD0, TEXT_FCOM,   SZ_NONE,  M_STI,  P_OREG),
    F1(0xD8, TEXT_FCOMP,  SZ_NONE,  M_STI,  P_OREG),
    F2(0xE0, TEXT_FSUB,   SZ_NONE,  M_ST0,  M_STI,  P_NONE, P_OREG),
    F2(0xE8, TEXT_FSUBR,  SZ_NONE,  M_ST0,  M_STI,  P_NONE, P_OREG),
    F2(0xF0, TEXT_FDIV,   SZ_NONE,  M_ST0,  M_STI,  P_NONE, P_OREG),
    F2(0xF8, TEXT_FDIVR,  SZ_NONE,  M_ST0,  M_STI,  P_NONE, P_OREG),
    F1(000,  TEXT_FADD,   SZ_DWORD, M_FMOD, P_OMOD),
    F1(010,  TEXT_FMUL,   SZ_DWORD, M_FMOD, P_OMOD),
    F1(020,  TEXT_FCOM,   SZ_DWORD, M_FMOD, P_OMOD),
    F1(030,  TEXT_FCOMP,  SZ_DWORD, M_FMOD, P_OMOD),
    F1(040,  TEXT_FSUB,   SZ_DWORD, M_FMOD, P_OMOD),
    F1(050,  TEXT_FSUBR,  SZ_DWORD, M_FMOD, P_OMOD),
    F1(060,  TEXT_FDIV,   SZ_DWORD, M_FMOD, P_OMOD),
    F1(070,  TEXT_FDIVR,  SZ_DWORD, M_FMOD, P_OMOD),
};

static constexpr uint8_t INDEX_D8[] PROGMEM = {
      0,  // TEXT_FADD
      8,  // TEXT_FADD
      2,  // TEXT_FCOM
     10,  // TEXT_FCOM
      3,  // TEXT_FCOMP
     11,  // TEXT_FCOMP
      6,  // TEXT_FDIV
     14,  // TEXT_FDIV
      7,  // TEXT_FDIVR
     15,  // TEXT_FDIVR
      1,  // TEXT_FMUL
      9,  // TEXT_FMUL
      4,  // TEXT_FSUB
     12,  // TEXT_FSUB
      5,  // TEXT_FSUBR
     13,  // TEXT_FSUBR
};

static constexpr Entry TABLE_D9[] PROGMEM = {
    F1(0xC0, TEXT_FLD,     SZ_NONE,  M_STI,  P_OREG),
    F1(0xC8, TEXT_FXCH,    SZ_NONE,  M_STI,  P_OREG),
    F0(0xD0, TEXT_FNOP,    SZ_NONE),
    F0(0xE0, TEXT_FCHS,    SZ_NONE),
    F0(0xE1, TEXT_FABS,    SZ_NONE),
    F0(0xE4, TEXT_FTST,    SZ_NONE),
    F0(0xE5, TEXT_FXAM,    SZ_NONE),
    F0(0xE8, TEXT_FLD1,    SZ_NONE),
    F0(0xE9, TEXT_FLDL2T,  SZ_NONE),
    F0(0xEA, TEXT_FLDL2E,  SZ_NONE),
    F0(0xEB, TEXT_FLDPI,   SZ_NONE),
    F0(0xEC, TEXT_FLDLG2,  SZ_NONE),
    F0(0xED, TEXT_FLDLN2,  SZ_NONE),
    F0(0xEE, TEXT_FLDZ,    SZ_NONE),
    F0(0xF0, TEXT_F2XM1,   SZ_NONE),
    F0(0xF1, TEXT_FYL2X,   SZ_NONE),
    F0(0xF2, TEXT_FPTAN,   SZ_NONE),
    F0(0xF3, TEXT_FPATAN,  SZ_NONE),
    F0(0xF4, TEXT_FXTRACT, SZ_NONE),
    F0(0xF6, TEXT_FDECSTP, SZ_NONE),
    F0(0xF7, TEXT_FINCSTP, SZ_NONE),
    F0(0xF8, TEXT_FPREM,   SZ_NONE),
    F0(0xF9, TEXT_FYL2XP1, SZ_NONE),
    F0(0xFA, TEXT_FSQRT,   SZ_NONE),
    F0(0xFC, TEXT_FRNDINT, SZ_NONE),
    F0(0xFD, TEXT_FSCALE,  SZ_NONE),
    F1(000,  TEXT_FLD,     SZ_DWORD, M_FMOD, P_OMOD),
    F1(020,  TEXT_FST,     SZ_DWORD, M_FMOD, P_OMOD),
    F1(030,  TEXT_FSTP,    SZ_DWORD, M_FMOD, P_OMOD),
    F1(040,  TEXT_FLDENV,  SZ_NONE,  M_WMOD, P_OMOD),
    F1(050,  TEXT_FLDCW,   SZ_NONE,  M_WMOD, P_OMOD),
    F1(060,  TEXT_FSTENV,  SZ_NONE,  M_WMOD, P_OMOD),
    F1(070,  TEXT_FSTCW,   SZ_NONE,  M_WMOD, P_OMOD),
};

static constexpr uint8_t INDEX_D9[] PROGMEM = {
     14,  // TEXT_F2XM1
      4,  // TEXT_FABS
      3,  // TEXT_FCHS
     19,  // TEXT_FDECSTP
     20,  // TEXT_FINCSTP
      0,  // TEXT_FLD
     26,  // TEXT_FLD
      7,  // TEXT_FLD1
     30,  // TEXT_FLDCW
     29,  // TEXT_FLDENV
      9,  // TEXT_FLDL2E
      8,  // TEXT_FLDL2T
     11,  // TEXT_FLDLG2
     12,  // TEXT_FLDLN2
     10,  // TEXT_FLDPI
     13,  // TEXT_FLDZ
      2,  // TEXT_FNOP
     17,  // TEXT_FPATAN
     21,  // TEXT_FPREM
     16,  // TEXT_FPTAN
     24,  // TEXT_FRNDINT
     25,  // TEXT_FSCALE
     23,  // TEXT_FSQRT
     27,  // TEXT_FST
     32,  // TEXT_FSTCW
     31,  // TEXT_FSTENV
     28,  // TEXT_FSTP
      5,  // TEXT_FTST
      6,  // TEXT_FXAM
      1,  // TEXT_FXCH
     18,  // TEXT_FXTRACT
     15,  // TEXT_FYL2X
     22,  // TEXT_FYL2XP1
};

static constexpr Entry TABLE_DA[] PROGMEM = {
    F1(000,  TEXT_FIADD,  SZ_DWORD, M_FMOD, P_OMOD),
    F1(010,  TEXT_FIMUL,  SZ_DWORD, M_FMOD, P_OMOD),
    F1(020,  TEXT_FICOM,  SZ_DWORD, M_FMOD, P_OMOD),
    F1(030,  TEXT_FICOMP, SZ_DWORD, M_FMOD, P_OMOD),
    F1(040,  TEXT_FISUB,  SZ_DWORD, M_FMOD, P_OMOD),
    F1(050,  TEXT_FISUBR, SZ_DWORD, M_FMOD, P_OMOD),
    F1(060,  TEXT_FIDIV,  SZ_DWORD, M_FMOD, P_OMOD),
    F1(070,  TEXT_FIDIVR, SZ_DWORD, M_FMOD, P_OMOD),
};

static constexpr uint8_t INDEX_DA[] PROGMEM = {
      0,  // TEXT_FIADD
      2,  // TEXT_FICOM
      3,  // TEXT_FICOMP
      6,  // TEXT_FIDIV
      7,  // TEXT_FIDIVR
      1,  // TEXT_FIMUL
      4,  // TEXT_FISUB
      5,  // TEXT_FISUBR
};

static constexpr Entry TABLE_DB[] PROGMEM = {
    F0(0xE0, TEXT_FENI,  SZ_NONE),
    F0(0xE1, TEXT_FDISI, SZ_NONE),
    F0(0xE2, TEXT_FCLEX, SZ_NONE),
    F0(0xE3, TEXT_FINIT, SZ_NONE),
    F1(000,  TEXT_FILD,  SZ_DWORD, M_FMOD, P_OMOD),
    F1(020,  TEXT_FIST,  SZ_DWORD, M_FMOD, P_OMOD),
    F1(030,  TEXT_FISTP, SZ_DWORD, M_FMOD, P_OMOD),
    F1(050,  TEXT_FLD,   SZ_TBYTE, M_FMOD, P_OMOD),
    F1(070,  TEXT_FSTP,  SZ_TBYTE, M_FMOD, P_OMOD),
};

static constexpr uint8_t INDEX_DB[] PROGMEM = {
      2,  // TEXT_FCLEX
      1,  // TEXT_FDISI
      0,  // TEXT_FENI
      4,  // TEXT_FILD
      3,  // TEXT_FINIT
      5,  // TEXT_FIST
      6,  // TEXT_FISTP
      7,  // TEXT_FLD
      8,  // TEXT_FSTP
};

static constexpr Entry TABLE_DC[] PROGMEM = {
    F2(0xC0, TEXT_FADD,   SZ_NONE,  M_STI,  M_ST0,  P_OREG, P_NONE),
    F2(0xC8, TEXT_FMUL,   SZ_NONE,  M_STI,  M_ST0,  P_OREG, P_NONE),
    F2(0xE8, TEXT_FSUB,   SZ_NONE,  M_STI,  M_ST0,  P_OREG, P_NONE),
    F2(0xE0, TEXT_FSUBR,  SZ_NONE,  M_STI,  M_ST0,  P_OREG, P_NONE),
    F2(0xF8, TEXT_FDIV,   SZ_NONE,  M_STI,  M_ST0,  P_OREG, P_NONE),
    F2(0xF0, TEXT_FDIVR,  SZ_NONE,  M_STI,  M_ST0,  P_OREG, P_NONE),
    F1(000,  TEXT_FADD,   SZ_QWORD, M_FMOD, P_OMOD),
    F1(010,  TEXT_FMUL,   SZ_QWORD, M_FMOD, P_OMOD),
    F1(020,  TEXT_FCOM,   SZ_QWORD, M_FMOD, P_OMOD),
    F1(030,  TEXT_FCOMP,  SZ_QWORD, M_FMOD, P_OMOD),
    F1(040,  TEXT_FSUB,   SZ_QWORD, M_FMOD, P_OMOD),
    F1(050,  TEXT_FSUBR,  SZ_QWORD, M_FMOD, P_OMOD),
    F1(060,  TEXT_FDIV,   SZ_QWORD, M_FMOD, P_OMOD),
    F1(070,  TEXT_FDIVR,  SZ_QWORD, M_FMOD, P_OMOD),
};

static constexpr uint8_t INDEX_DC[] PROGMEM = {
      0,  // TEXT_FADD
      6,  // TEXT_FADD
      8,  // TEXT_FCOM
      9,  // TEXT_FCOMP
      4,  // TEXT_FDIV
     12,  // TEXT_FDIV
      5,  // TEXT_FDIVR
     13,  // TEXT_FDIVR
      1,  // TEXT_FMUL
      7,  // TEXT_FMUL
      2,  // TEXT_FSUB
     10,  // TEXT_FSUB
      3,  // TEXT_FSUBR
     11,  // TEXT_FSUBR
};

static constexpr Entry TABLE_DD[] PROGMEM = {
    F1(0xC0, TEXT_FFREE,  SZ_NONE,  M_STI,  P_OREG),
    F1(0xD0, TEXT_FST,    SZ_NONE,  M_STI,  P_OREG),
    F1(0xD8, TEXT_FSTP,   SZ_NONE,  M_STI,  P_OREG),
    F1(000,  TEXT_FLD,    SZ_QWORD, M_FMOD, P_OMOD),
    F1(020,  TEXT_FST,    SZ_QWORD, M_FMOD, P_OMOD),
    F1(030,  TEXT_FSTP,   SZ_QWORD, M_FMOD, P_OMOD),
    F1(040,  TEXT_FRSTOR, SZ_NONE,  M_WMOD, P_OMOD),
    F1(060,  TEXT_FSAVE,  SZ_NONE,  M_WMOD, P_OMOD),
    F1(070,  TEXT_FSTSW,  SZ_NONE,  M_WMOD, P_OMOD),
};

static constexpr uint8_t INDEX_DD[] PROGMEM = {
      0,  // TEXT_FFREE
      3,  // TEXT_FLD
      6,  // TEXT_FRSTOR
      7,  // TEXT_FSAVE
      1,  // TEXT_FST
      4,  // TEXT_FST
      2,  // TEXT_FSTP
      5,  // TEXT_FSTP
      8,  // TEXT_FSTSW
};

static constexpr Entry TABLE_DE[] PROGMEM = {
    F2(0xC0, TEXT_FADDP,  SZ_NONE,  M_STI, M_ST0, P_OREG, P_NONE),
    F2(0xC8, TEXT_FMULP,  SZ_NONE,  M_STI, M_ST0, P_OREG, P_NONE),
    F0(0xD9, TEXT_FCOMPP, SZ_NONE),
    F2(0xE0, TEXT_FSUBRP, SZ_NONE,  M_STI, M_ST0, P_OREG, P_NONE),
    F2(0xE8, TEXT_FSUBP,  SZ_NONE,  M_STI, M_ST0, P_OREG, P_NONE),
    F2(0xF0, TEXT_FDIVRP, SZ_NONE,  M_STI, M_ST0, P_OREG, P_NONE),
    F2(0xF8, TEXT_FDIVP,  SZ_NONE,  M_STI, M_ST0, P_OREG, P_NONE),
    F1(000,  TEXT_FIADD,  SZ_WORD, M_WMOD, P_OMOD),
    F1(010,  TEXT_FIMUL,  SZ_WORD, M_WMOD, P_OMOD),
    F1(020,  TEXT_FICOM,  SZ_WORD, M_WMOD, P_OMOD),
    F1(030,  TEXT_FICOMP, SZ_WORD, M_WMOD, P_OMOD),
    F1(040,  TEXT_FISUB,  SZ_WORD, M_WMOD, P_OMOD),
    F1(050,  TEXT_FISUBR, SZ_WORD, M_WMOD, P_OMOD),
    F1(060,  TEXT_FIDIV,  SZ_WORD, M_WMOD, P_OMOD),
    F1(070,  TEXT_FIDIVR, SZ_WORD, M_WMOD, P_OMOD),
};

static constexpr uint8_t INDEX_DE[] PROGMEM = {
      0,  // TEXT_FADDP
      2,  // TEXT_FCOMPP
      6,  // TEXT_FDIVP
      5,  // TEXT_FDIVRP
      7,  // TEXT_FIADD
      9,  // TEXT_FICOM
     10,  // TEXT_FICOMP
     13,  // TEXT_FIDIV
     14,  // TEXT_FIDIVR
      8,  // TEXT_FIMUL
     11,  // TEXT_FISUB
     12,  // TEXT_FISUBR
      1,  // TEXT_FMULP
      4,  // TEXT_FSUBP
      3,  // TEXT_FSUBRP
};

static constexpr Entry TABLE_DF[] PROGMEM = {
    F1(000,  TEXT_FILD,   SZ_WORD,  M_WMOD, P_OMOD),
    F1(020,  TEXT_FIST,   SZ_WORD,  M_WMOD, P_OMOD),
    F1(030,  TEXT_FISTP,  SZ_WORD,  M_WMOD, P_OMOD),
    F1(040,  TEXT_FBLD,   SZ_TBYTE, M_FMOD, P_OMOD),
    F1(050,  TEXT_FILD,   SZ_QWORD, M_FMOD, P_OMOD),
    F1(060,  TEXT_FBSTP,  SZ_TBYTE, M_FMOD, P_OMOD),
    F1(070,  TEXT_FISTP,  SZ_QWORD, M_FMOD, P_OMOD),
};

static constexpr uint8_t INDEX_DF[] PROGMEM = {
      3,  // TEXT_FBLD
      5,  // TEXT_FBSTP
      0,  // TEXT_FILD
      4,  // TEXT_FILD
      1,  // TEXT_FIST
      2,  // TEXT_FISTP
      6,  // TEXT_FISTP
};

static constexpr Entry TABLE_FPU[] PROGMEM = {
    E0(0x9B, TEXT_FWAIT, SZ_NONE),
};

static constexpr uint8_t INDEX_FPU[] PROGMEM = {
      0,  // TEXT_FWAIT
};

// clang-format on

using EntryPage = entry::PrefixTableBase<Entry>;

static constexpr EntryPage I8086_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(TABLE_00), ARRAY_RANGE(INDEX_00)},
        {0x80, ARRAY_RANGE(TABLE_80), ARRAY_RANGE(INDEX_8X)},
        {0x83, ARRAY_RANGE(TABLE_83), ARRAY_RANGE(INDEX_8X)},  // M_IMM8
        {0x81, ARRAY_RANGE(TABLE_81), ARRAY_RANGE(INDEX_8X)},  // M_IMM
        {0x8F, ARRAY_RANGE(TABLE_8F), ARRAY_RANGE(INDEX_8F)},
        {0xC6, ARRAY_RANGE(TABLE_C6), ARRAY_RANGE(INDEX_C6)},
        {0xC7, ARRAY_RANGE(TABLE_C7), ARRAY_RANGE(INDEX_C7)},
        {0xD0, ARRAY_RANGE(TABLE_D0), ARRAY_RANGE(INDEX_DX)},
        {0xD1, ARRAY_RANGE(TABLE_D1), ARRAY_RANGE(INDEX_DX)},
        {0xD2, ARRAY_RANGE(TABLE_D2), ARRAY_RANGE(INDEX_DX)},
        {0xD3, ARRAY_RANGE(TABLE_D3), ARRAY_RANGE(INDEX_DX)},
        {0xD4, ARRAY_RANGE(TABLE_D4), ARRAY_RANGE(INDEX_D4)},
        {0xD5, ARRAY_RANGE(TABLE_D5), ARRAY_RANGE(INDEX_D5)},
        {0xF6, ARRAY_RANGE(TABLE_F6), ARRAY_RANGE(INDEX_FX)},
        {0xF7, ARRAY_RANGE(TABLE_F7), ARRAY_RANGE(INDEX_FX)},
        {0xFE, ARRAY_RANGE(TABLE_FE), ARRAY_RANGE(INDEX_FE)},
        {0xFF, ARRAY_RANGE(TABLE_FF), ARRAY_RANGE(INDEX_FF)},
};

static constexpr EntryPage I80186_PAGES[] PROGMEM = {
        // I80186
        {0x00, ARRAY_RANGE(TABLE_I80186), ARRAY_RANGE(INDEX_I80186)},
        {0xD0, ARRAY_RANGE(TABLE_D0), ARRAY_RANGE(INDEX_DX)},
        {0xD1, ARRAY_RANGE(TABLE_D1), ARRAY_RANGE(INDEX_DX)},
        {0xC0, ARRAY_RANGE(TABLE_C0), ARRAY_RANGE(INDEX_CX)},
        {0xC1, ARRAY_RANGE(TABLE_C1), ARRAY_RANGE(INDEX_CX)},
        // i8086
        {0x00, ARRAY_RANGE(TABLE_00), ARRAY_RANGE(INDEX_00)},
        {0x80, ARRAY_RANGE(TABLE_80), ARRAY_RANGE(INDEX_8X)},
        {0x83, ARRAY_RANGE(TABLE_83), ARRAY_RANGE(INDEX_8X)},  // M_IMM8
        {0x81, ARRAY_RANGE(TABLE_81), ARRAY_RANGE(INDEX_8X)},  // M_IMM
        {0x8F, ARRAY_RANGE(TABLE_8F), ARRAY_RANGE(INDEX_8F)},
        {0xC6, ARRAY_RANGE(TABLE_C6), ARRAY_RANGE(INDEX_C6)},
        {0xC7, ARRAY_RANGE(TABLE_C7), ARRAY_RANGE(INDEX_C7)},
        {0xD2, ARRAY_RANGE(TABLE_D2), ARRAY_RANGE(INDEX_DX)},
        {0xD3, ARRAY_RANGE(TABLE_D3), ARRAY_RANGE(INDEX_DX)},
        {0xD4, ARRAY_RANGE(TABLE_D4), ARRAY_RANGE(INDEX_D4)},
        {0xD5, ARRAY_RANGE(TABLE_D5), ARRAY_RANGE(INDEX_D5)},
        {0xF6, ARRAY_RANGE(TABLE_F6), ARRAY_RANGE(INDEX_FX)},
        {0xF7, ARRAY_RANGE(TABLE_F7), ARRAY_RANGE(INDEX_FX)},
        {0xFE, ARRAY_RANGE(TABLE_FE), ARRAY_RANGE(INDEX_FE)},
        {0xFF, ARRAY_RANGE(TABLE_FF), ARRAY_RANGE(INDEX_FF)},
};

static constexpr EntryPage V30_PAGES[] PROGMEM = {
        // V30
        {0x00, ARRAY_RANGE(V30TABLE_00), ARRAY_RANGE(V30INDEX_00)},
        {0x0F, ARRAY_RANGE(V30TABLE_0F), ARRAY_RANGE(V30INDEX_0F)},
        // I80186
        {0x00, ARRAY_RANGE(TABLE_I80186), ARRAY_RANGE(INDEX_I80186)},
        {0xD0, ARRAY_RANGE(TABLE_D0), ARRAY_RANGE(INDEX_DX)},
        {0xD1, ARRAY_RANGE(TABLE_D1), ARRAY_RANGE(INDEX_DX)},
        {0xC0, ARRAY_RANGE(TABLE_C0), ARRAY_RANGE(INDEX_CX)},
        {0xC1, ARRAY_RANGE(TABLE_C1), ARRAY_RANGE(INDEX_CX)},
        // i8086
        {0x00, ARRAY_RANGE(TABLE_00), ARRAY_RANGE(INDEX_00)},
        {0x80, ARRAY_RANGE(TABLE_80), ARRAY_RANGE(INDEX_8X)},
        {0x83, ARRAY_RANGE(TABLE_83), ARRAY_RANGE(INDEX_8X)},  // M_IMM8
        {0x81, ARRAY_RANGE(TABLE_81), ARRAY_RANGE(INDEX_8X)},  // M_IMM
        {0x8F, ARRAY_RANGE(TABLE_8F), ARRAY_RANGE(INDEX_8F)},
        {0xC6, ARRAY_RANGE(TABLE_C6), ARRAY_RANGE(INDEX_C6)},
        {0xC7, ARRAY_RANGE(TABLE_C7), ARRAY_RANGE(INDEX_C7)},
        {0xD2, ARRAY_RANGE(TABLE_D2), ARRAY_RANGE(INDEX_DX)},
        {0xD3, ARRAY_RANGE(TABLE_D3), ARRAY_RANGE(INDEX_DX)},
        {0xD4, ARRAY_RANGE(TABLE_D4), ARRAY_RANGE(INDEX_D4)},
        {0xD5, ARRAY_RANGE(TABLE_D5), ARRAY_RANGE(INDEX_D5)},
        {0xF6, ARRAY_RANGE(TABLE_F6), ARRAY_RANGE(INDEX_FX)},
        {0xF7, ARRAY_RANGE(TABLE_F7), ARRAY_RANGE(INDEX_FX)},
        {0xFE, ARRAY_RANGE(TABLE_FE), ARRAY_RANGE(INDEX_FE)},
        {0xFF, ARRAY_RANGE(TABLE_FF), ARRAY_RANGE(INDEX_FF)},
};

static constexpr EntryPage I8087_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(TABLE_FPU), ARRAY_RANGE(INDEX_FPU)},
        {0xD8, ARRAY_RANGE(TABLE_D8), ARRAY_RANGE(INDEX_D8)},
        {0xD9, ARRAY_RANGE(TABLE_D9), ARRAY_RANGE(INDEX_D9)},
        {0xDA, ARRAY_RANGE(TABLE_DA), ARRAY_RANGE(INDEX_DA)},
        {0xDB, ARRAY_RANGE(TABLE_DB), ARRAY_RANGE(INDEX_DB)},
        {0xDC, ARRAY_RANGE(TABLE_DC), ARRAY_RANGE(INDEX_DC)},
        {0xDD, ARRAY_RANGE(TABLE_DD), ARRAY_RANGE(INDEX_DD)},
        {0xDE, ARRAY_RANGE(TABLE_DE), ARRAY_RANGE(INDEX_DE)},
        {0xDF, ARRAY_RANGE(TABLE_DF), ARRAY_RANGE(INDEX_DF)},
};

template <typename CPUTYPE>
using Processor = entry::CpuBase<CPUTYPE, EntryPage>;

struct Cpu : Processor<CpuType> {
    constexpr Cpu(CpuType cpuType, const /* PROGMEM */ char *name_P, const EntryPage *table,
            const EntryPage *end)
        : Processor<CpuType>(cpuType, name_P, table, end) {}
};

struct Fpu : Processor<FpuType> {
    constexpr Fpu(FpuType fpuType, const /* PROGMEM */ char *name_P, const EntryPage *table,
            const EntryPage *end)
        : Processor<FpuType>(fpuType, name_P, table, end) {}
};

static constexpr Cpu CPU_TABLE[] PROGMEM = {
        {I8086, TEXT_CPU_8086, ARRAY_RANGE(I8086_PAGES)},
        {I80186, TEXT_CPU_80186, ARRAY_RANGE(I80186_PAGES)},
        {V30, TEXT_CPU_V30, ARRAY_RANGE(V30_PAGES)},
};

static const Cpu *cpu(CpuType cpuType) {
    return Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
}

#define EMPTY_RANGE(a) ARRAY_BEGIN(a), ARRAY_BEGIN(a)

static constexpr Fpu FPU_TABLE[] PROGMEM = {
        {FPU_I8087, TEXT_FPU_8087, ARRAY_RANGE(I8087_PAGES)},
        {FPU_NONE, TEXT_none, EMPTY_RANGE(I8087_PAGES)},
};

static const Fpu *fpu(FpuType fpuType) {
    return Fpu::search(fpuType, ARRAY_RANGE(FPU_TABLE));
}

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
    if (opr == M_ST0)
        return table == M_STI;
    return false;
}

static bool hasSize(AddrMode mode) {
    return mode == M_AX || mode == M_DX || mode == M_WREG || mode == M_AL || mode == M_CL ||
           mode == M_BREG || mode == M_CS || mode == M_SREG;
}

static bool acceptSize(const AsmInsn &insn, const Entry *entry) {
    const auto dst = insn.dstOp.mode;
    const auto src = insn.srcOp.mode;
    const auto flags = entry->flags();
    if (dst == M_MEM || dst == M_DIR) {
        if (src == M_NONE)
            return flags.size() == SZ_NONE;
        return hasSize(src) || flags.stringInst();
    }
    if (src == M_MEM || src == M_DIR)
        return hasSize(dst) || flags.stringInst();
    if (dst == M_FMOD) {
        const auto ptrSize = OprSize(insn.dstOp.ptr - REG_PTR);
        return ptrSize == flags.size();
    }
    return true;
}

static bool acceptModes(AsmInsn &insn, const Entry *entry) {
    const auto table = entry->flags();
    return acceptMode(insn.dstOp.mode, table.dst()) && acceptMode(insn.srcOp.mode, table.src()) &&
           acceptMode(insn.extOp.mode, table.ext()) && acceptSize(insn, entry);
}

static constexpr char TEXT_FN[] PROGMEM = "FN";

Error TableI8086::searchName(const CpuSpec &cpuSpec, AsmInsn &insn) const {
    fpu(cpuSpec.fpu)->searchName(insn, acceptModes);
    if (insn.getError() == UNKNOWN_INSTRUCTION) {
        // check non-wait version FNxxxx
        if (strncasecmp_P(insn.name(), TEXT_FN, 2) == 0) {
            char name[insn.nameBuffer().len() + 1];
            strcpy(name, insn.name());
            insn.nameBuffer().reset().letter('F').text(name + 2);
            insn.setOK();
            fpu(cpuSpec.fpu)->searchName(insn, acceptModes);
            if (insn.isOK() && insn.fpuInst()) {
                ;  // found non-wait float instruction
            } else {
                insn.setError(UNKNOWN_INSTRUCTION);
            }
            insn.nameBuffer().reset().text(name);
        }
    } else if (insn.fpuInst()) {
        insn.setFwait();
    }
    if (insn.getError() == UNKNOWN_INSTRUCTION) {
        insn.setOK();
        cpu(cpuSpec.cpu)->searchName(insn, acceptModes);
    }
    return insn.getError();
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

static bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *page) {
    UNUSED(page);
    auto opc = insn.opCode();
    const auto flags = entry->flags();
    const auto dstPos = flags.dstPos();
    const auto srcPos = flags.srcPos();
    if (dstPos == P_OREG || srcPos == P_OREG) {
        opc &= ~0007;
    } else if (dstPos == P_OSEG || srcPos == P_OSEG) {
        opc &= ~0030;
    } else if (dstPos == P_OMOD || srcPos == P_OMOD) {
        const auto prefix = page->prefix();
        if (prefix < 0xD8 || prefix >= 0xE0 || (opc >> 6) != 3)
            opc &= ~0307;
    }
    return opc == entry->opCode();
}

Error TableI8086::searchOpCode(const CpuSpec &cpuSpec, DisInsn &insn, StrBuffer &out) const {
    fpu(cpuSpec.fpu)->searchOpCode(insn, out, matchOpCode);
    if (insn.isOK()) {
        if (insn.opCode() == DisInsn::FWAIT)
            insn.setError(UNKNOWN_INSTRUCTION);  // prefer WAIT than FWAIT
        if (insn.fpuInst() && insn.fwait() == 0) {
            // no-wait instruction
            char name[insn.nameBuffer().len() + 2];
            strcpy(name, insn.name());
            auto save{out};
            insn.nameBuffer().reset().over(out).text_P(TEXT_FN).text(name + 1).over(
                    insn.nameBuffer());
            save.over(out);
        }
    }
    if (insn.getError() == UNKNOWN_INSTRUCTION && insn.fwait() == 0) {
        insn.setOK();
        cpu(cpuSpec.cpu)->searchOpCode(insn, out, matchOpCode);
    }
    return insn.getError();
}

bool TableI8086::isPrefix(CpuType cpuType, Config::opcode_t code) const {
    return cpu(cpuType)->isPrefix(code);
}

bool TableI8086::isPrefix(FpuType fpuType, Config::opcode_t code) const {
    return fpu(fpuType)->isPrefix(code);
}

const /*PROGMEM*/ char *TableI8086::listCpu_P() const {
    return TEXT_I8086_LIST;
}

const /*PROGMEM*/ char *TableI8086::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableI8086::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    auto t = Cpu::search(name, ARRAY_RANGE(CPU_TABLE));
    if (t) {
        cpuType = t->cpuType();
    } else {
        name.iexpect('i');
        if (name.iequals(TEXT_CPU_8086)) {
            cpuType = I8086;
        } else if (name.iequals(TEXT_CPU_80186)) {
            cpuType = I80186;
        } else {
            return UNSUPPORTED_CPU;
        }
    }
    return OK;
}

const TableI8086 TABLE;

}  // namespace i8086
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

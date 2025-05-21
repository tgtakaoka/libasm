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
#include "reg_i8086.h"
#include "text_i8086.h"

#include <stdio.h>

namespace libasm {
namespace i8086 {

using namespace reg;
using namespace libasm::text::i8086;

#define X3(_opc, _cf, _name, _sz, _dst, _src, _ext, _dpos, _spos, _epos, _str, _lock, _nsz) \
    {_opc,                                                                                  \
            Entry::Flags::create(                                                           \
                    _cf, _dst, _src, _ext, _dpos, _spos, _epos, _sz, _str, _lock, _nsz),    \
            _name}
#define X2(_opc, _cf, _name, _sz, _dst, _src, _dpos, _spos, _str, _lock, _nsz) \
    X3(_opc, _cf, _name, _sz, _dst, _src, M_NONE, _dpos, _spos, P_NONE, _str, _lock, _nsz)
#define X1(_opc, _cf, _name, _sz, _dst, _dpos, _str, _lock, _nsz) \
    X2(_opc, _cf, _name, _sz, _dst, M_NONE, _dpos, P_NONE, _str, _lock, _nsz)
#define E3(_opc, _cf, _name, _sz, _dst, _src, _ext, _dpos, _spos, _epos) \
    X3(_opc, _cf, _name, _sz, _dst, _src, _ext, _dpos, _spos, _epos, false, false, false)
#define E2(_opc, _cf, _name, _sz, _dst, _src, _dpos, _spos) \
    E3(_opc, _cf, _name, _sz, _dst, _src, M_NONE, _dpos, _spos, P_NONE)
#define E1(_opc, _cf, _name, _sz, _dst, _dpos) \
    E2(_opc, _cf, _name, _sz, _dst, M_NONE, _dpos, P_NONE)
#define E0(_opc, _cf, _name, _sz) E1(_opc, _cf, _name, _sz, M_NONE, P_NONE)
#define L2(_opc, _cf, _name, _sz, _dst, _src, _dpos, _spos) \
    X2(_opc, _cf, _name, _sz, _dst, _src, _dpos, _spos, false, true, false)
#define L1(_opc, _cf, _name, _sz, _dst, _dpos) \
    L2(_opc, _cf, _name, _sz, _dst, M_NONE, _dpos, P_NONE)
#define N2(_opc, _cf, _name, _sz, _dst, _src, _dpos, _spos) \
    X2(_opc, _cf, _name, _sz, _dst, _src, _dpos, _spos, false, false, true)
#define N1(_opc, _cf, _name, _sz, _dst, _dpos) \
    N2(_opc, _cf, _name, _sz, _dst, M_NONE, _dpos, P_NONE)
#define M2(_opc, _cf, _name, _sz, _dst, _src, _dpos, _spos) \
    X2(_opc, _cf, _name, _sz, _dst, _src, _dpos, _spos, false, true, true)
#define M1(_opc, _cf, _name, _sz, _dst, _dpos) \
    L2(_opc, _cf, _name, _sz, _dst, M_NONE, _dpos, P_NONE)
#define S2(_opc, _cf, _name, _sz, _dst, _src) \
    X2(_opc, _cf, _name, _sz, _dst, _src, P_NONE, P_NONE, true, false, false)
#define S1(_opc, _cf, _name, _sz, _dst) S2(_opc, _cf, _name, _sz, _dst, M_NONE)
#define S0(_opc, _cf, _name, _sz) S1(_opc, _cf, _name, _sz, M_NONE)
#define T2(_opc, _cf, _name, _sz, _dst, _src) \
    X2(_opc, _cf, _name, _sz, _dst, _src, P_NONE, P_NONE, true, false, true)
#define T1(_opc, _cf, _name, _sz, _dst) T2(_opc, _cf, _name, _sz, _dst, M_NONE)

// clang-format off
constexpr Entry T8086_00[] PROGMEM = {
    E2(0xA0, CF_00, TEXT_MOV,    SZ_BYTE, M_AL,   M_BDIR, P_NONE, P_OPR),
    E2(0xA1, CF_00, TEXT_MOV,    SZ_DATA, M_AX,   M_WDIR, P_NONE, P_OPR),
    E2(0xA2, CF_00, TEXT_MOV,    SZ_BYTE, M_BDIR, M_AL,   P_OPR,  P_NONE),
    E2(0xA3, CF_00, TEXT_MOV,    SZ_DATA, M_WDIR, M_AX,   P_OPR,  P_NONE),
    E0(0x90, CF_00, TEXT_NOP,    SZ_NONE),
    E2(0x90, CF_07, TEXT_XCHG,   SZ_DATA, M_AX,   M_WREG, P_NONE, P_OREG),
    E2(0x90, CF_07, TEXT_XCHG,   SZ_DATA, M_WREG, M_AX,   P_OREG, P_NONE),
    E2(0x04, CF_00, TEXT_ADD,    SZ_BYTE, M_AL,   M_WIMM, P_NONE, P_OPR),
    E2(0x05, CF_00, TEXT_ADD,    SZ_DATA, M_AX,   M_WIMM, P_NONE, P_OPR),
    E2(0x0C, CF_00, TEXT_OR,     SZ_BYTE, M_AL,   M_WIMM, P_NONE, P_OPR),
    E2(0x0D, CF_00, TEXT_OR,     SZ_WORD, M_AX,   M_WIMM, P_NONE, P_OPR),
    L2(0x00, CF_00, TEXT_ADD,    SZ_BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E2(0x02, CF_00, TEXT_ADD,    SZ_BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    L2(0x01, CF_00, TEXT_ADD,    SZ_WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    E2(0x03, CF_00, TEXT_ADD,    SZ_WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    E1(0x06, CF_30, TEXT_PUSH,   SZ_WORD, M_SREG, P_OSEG),
    E1(0x0F, CF_00, TEXT_POP,    SZ_WORD, M_CS,   P_NONE),
    E1(0x07, CF_30, TEXT_POP,    SZ_WORD, M_SREG, P_OSEG),
    L2(0x08, CF_00, TEXT_OR,     SZ_BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E2(0x0A, CF_00, TEXT_OR,     SZ_BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    L2(0x09, CF_00, TEXT_OR,     SZ_WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    E2(0x0B, CF_00, TEXT_OR,     SZ_WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    L2(0x10, CF_00, TEXT_ADC,    SZ_BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E2(0x12, CF_00, TEXT_ADC,    SZ_BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    L2(0x11, CF_00, TEXT_ADC,    SZ_WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    E2(0x13, CF_00, TEXT_ADC,    SZ_WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    E2(0x14, CF_00, TEXT_ADC,    SZ_BYTE, M_AL,   M_WIMM, P_NONE, P_OPR),
    E2(0x15, CF_00, TEXT_ADC,    SZ_WORD, M_AX,   M_WIMM, P_NONE, P_OPR),
    L2(0x18, CF_00, TEXT_SBB,    SZ_BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E2(0x1A, CF_00, TEXT_SBB,    SZ_BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    L2(0x19, CF_00, TEXT_SBB,    SZ_WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    E2(0x1B, CF_00, TEXT_SBB,    SZ_WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    E2(0x1C, CF_00, TEXT_SBB,    SZ_BYTE, M_AL,   M_WIMM, P_NONE, P_OPR),
    E2(0x1D, CF_00, TEXT_SBB,    SZ_WORD, M_AX,   M_WIMM, P_NONE, P_OPR),
    L2(0x20, CF_00, TEXT_AND,    SZ_BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E2(0x22, CF_00, TEXT_AND,    SZ_BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    L2(0x21, CF_00, TEXT_AND,    SZ_WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    E2(0x23, CF_00, TEXT_AND,    SZ_WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    E2(0x24, CF_00, TEXT_AND,    SZ_BYTE, M_AL,   M_WIMM, P_NONE, P_OPR),
    E2(0x25, CF_00, TEXT_AND,    SZ_WORD, M_AX,   M_WIMM, P_NONE, P_OPR),
    E0(0x26, CF_00, TEXT_SEGES,  SZ_BYTE),
    E0(0x27, CF_00, TEXT_DAA,    SZ_BYTE),
    L2(0x28, CF_00, TEXT_SUB,    SZ_BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E2(0x2A, CF_00, TEXT_SUB,    SZ_BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    L2(0x29, CF_00, TEXT_SUB,    SZ_WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    E2(0x2B, CF_00, TEXT_SUB,    SZ_WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    E2(0x2C, CF_00, TEXT_SUB,    SZ_BYTE, M_AL,   M_WIMM, P_NONE, P_OPR),
    E2(0x2D, CF_00, TEXT_SUB,    SZ_WORD, M_AX,   M_WIMM, P_NONE, P_OPR),
    E0(0x2E, CF_00, TEXT_SEGCS,  SZ_BYTE),
    E0(0x2F, CF_00, TEXT_DAS,    SZ_BYTE),
    L2(0x30, CF_00, TEXT_XOR,    SZ_BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E2(0x32, CF_00, TEXT_XOR,    SZ_BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    L2(0x31, CF_00, TEXT_XOR,    SZ_WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    E2(0x33, CF_00, TEXT_XOR,    SZ_WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    E2(0x34, CF_00, TEXT_XOR,    SZ_BYTE, M_AL,   M_WIMM, P_NONE, P_OPR),
    E2(0x35, CF_00, TEXT_XOR,    SZ_DATA, M_AX,   M_WIMM, P_NONE, P_OPR),
    E0(0x36, CF_00, TEXT_SEGSS,  SZ_NONE),
    E0(0x37, CF_00, TEXT_AAA,    SZ_NONE),
    E2(0x38, CF_00, TEXT_CMP,    SZ_BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E2(0x3A, CF_00, TEXT_CMP,    SZ_BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    E2(0x39, CF_00, TEXT_CMP,    SZ_DATA, M_WMOD, M_WREG, P_MOD,  P_REG),
    E2(0x3B, CF_00, TEXT_CMP,    SZ_DATA, M_WREG, M_WMOD, P_REG,  P_MOD),
    E2(0x3C, CF_00, TEXT_CMP,    SZ_BYTE, M_AL,   M_WIMM, P_NONE, P_OPR),
    E2(0x3D, CF_00, TEXT_CMP,    SZ_DATA, M_AX,   M_WIMM, P_NONE, P_OPR),
    E0(0x3E, CF_00, TEXT_SEGDS,  SZ_NONE),
    E0(0x3F, CF_00, TEXT_AAS,    SZ_NONE),
    E1(0x40, CF_07, TEXT_INC,    SZ_DATA, M_WREG, P_OREG),
    E1(0x48, CF_07, TEXT_DEC,    SZ_DATA, M_WREG, P_OREG),
    E1(0x50, CF_07, TEXT_PUSH,   SZ_DATA, M_WREG, P_OREG),
    E1(0x58, CF_07, TEXT_POP,    SZ_DATA, M_WREG, P_OREG),
    E1(0x70, CF_00, TEXT_JO,     SZ_NONE, M_REL8, P_OPR),
    E1(0x71, CF_00, TEXT_JNO,    SZ_NONE, M_REL8, P_OPR),
    E1(0x72, CF_00, TEXT_JB,     SZ_NONE, M_REL8, P_OPR),
    E1(0x72, CF_00, TEXT_JC,     SZ_NONE, M_REL8, P_OPR),
    E1(0x72, CF_00, TEXT_JNAE,   SZ_NONE, M_REL8, P_OPR),
    E1(0x73, CF_00, TEXT_JAE,    SZ_NONE, M_REL8, P_OPR),
    E1(0x73, CF_00, TEXT_JNB,    SZ_NONE, M_REL8, P_OPR),
    E1(0x73, CF_00, TEXT_JNC,    SZ_NONE, M_REL8, P_OPR),
    E1(0x74, CF_00, TEXT_JE,     SZ_NONE, M_REL8, P_OPR),
    E1(0x74, CF_00, TEXT_JZ,     SZ_NONE, M_REL8, P_OPR),
    E1(0x75, CF_00, TEXT_JNE,    SZ_NONE, M_REL8, P_OPR),
    E1(0x75, CF_00, TEXT_JNZ,    SZ_NONE, M_REL8, P_OPR),
    E1(0x76, CF_00, TEXT_JBE,    SZ_NONE, M_REL8, P_OPR),
    E1(0x76, CF_00, TEXT_JNA,    SZ_NONE, M_REL8, P_OPR),
    E1(0x77, CF_00, TEXT_JA,     SZ_NONE, M_REL8, P_OPR),
    E1(0x77, CF_00, TEXT_JNBE,   SZ_NONE, M_REL8, P_OPR),
    E1(0x78, CF_00, TEXT_JS,     SZ_NONE, M_REL8, P_OPR),
    E1(0x79, CF_00, TEXT_JNS,    SZ_NONE, M_REL8, P_OPR),
    E1(0x7A, CF_00, TEXT_JPE,    SZ_NONE, M_REL8, P_OPR),
    E1(0x7A, CF_00, TEXT_JP,     SZ_NONE, M_REL8, P_OPR),
    E1(0x7B, CF_00, TEXT_JPO,    SZ_NONE, M_REL8, P_OPR),
    E1(0x7B, CF_00, TEXT_JNP,    SZ_NONE, M_REL8, P_OPR),
    E1(0x7C, CF_00, TEXT_JL,     SZ_NONE, M_REL8, P_OPR),
    E1(0x7C, CF_00, TEXT_JNGE,   SZ_NONE, M_REL8, P_OPR),
    E1(0x7D, CF_00, TEXT_JGE,    SZ_NONE, M_REL8, P_OPR),
    E1(0x7D, CF_00, TEXT_JNL,    SZ_NONE, M_REL8, P_OPR),
    E1(0x7E, CF_00, TEXT_JLE,    SZ_NONE, M_REL8, P_OPR),
    E1(0x7E, CF_00, TEXT_JNG,    SZ_NONE, M_REL8, P_OPR),
    E1(0x7F, CF_00, TEXT_JG,     SZ_NONE, M_REL8, P_OPR),
    E1(0x7F, CF_00, TEXT_JNLE,   SZ_NONE, M_REL8, P_OPR),
    E2(0x84, CF_00, TEXT_TEST,   SZ_BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E2(0x84, CF_00, TEXT_TEST,   SZ_BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    E2(0x85, CF_00, TEXT_TEST,   SZ_WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    E2(0x85, CF_00, TEXT_TEST,   SZ_WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    L2(0x86, CF_00, TEXT_XCHG,   SZ_BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    L2(0x86, CF_00, TEXT_XCHG,   SZ_BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    L2(0x87, CF_00, TEXT_XCHG,   SZ_WORD, M_WMOD, M_WREG, P_MOD,  P_REG),
    L2(0x87, CF_00, TEXT_XCHG,   SZ_WORD, M_WREG, M_WMOD, P_REG,  P_MOD),
    E2(0x88, CF_00, TEXT_MOV,    SZ_BYTE, M_BMOD, M_BREG, P_MOD,  P_REG),
    E2(0x8A, CF_00, TEXT_MOV,    SZ_BYTE, M_BREG, M_BMOD, P_REG,  P_MOD),
    E2(0x89, CF_00, TEXT_MOV,    SZ_DATA, M_WMOD, M_WREG, P_MOD,  P_REG),
    E2(0x8B, CF_00, TEXT_MOV,    SZ_DATA, M_WREG, M_WMOD, P_REG,  P_MOD),
    E2(0x8C, CF_00, TEXT_MOV,    SZ_WORD, M_WMOD, M_SREG, P_MOD,  P_REG),
    E2(0x8D, CF_00, TEXT_LEA,    SZ_DATA, M_WREG, M_MEM,  P_REG,  P_MOD),
    E2(0x8E, CF_00, TEXT_MOV,    SZ_WORD, M_SREG, M_WMOD, P_REG,  P_MOD),
    E0(0x98, CF_00, TEXT_CBW,    SZ_NONE),
    E0(0x99, CF_00, TEXT_CWD,    SZ_NONE),
    E1(0x9A, CF_00, TEXT_CALL,   SZ_NONE, M_FAR,  P_OPR), // FMODE_FAR
    E1(0x9A, CF_00, TEXT_CALLF,  SZ_NONE, M_FAR,  P_OPR), // FMODE_F
    E2(0x9A, CF_00, TEXT_LCALL,  SZ_NONE, M_SEG,  M_OFF, P_OPR,  P_OPR), // FMODE_L
    E0(0x9B, CF_00, TEXT_WAIT,   SZ_NONE),
    E0(0x9C, CF_00, TEXT_PUSHF,  SZ_WORD),
    E0(0x9D, CF_00, TEXT_POPF,   SZ_WORD),
    E0(0x9E, CF_00, TEXT_SAHF,   SZ_NONE),
    E0(0x9F, CF_00, TEXT_LAHF,   SZ_NONE),
    S2(0xA4, CF_00, TEXT_MOVSB,  SZ_BYTE, M_BMEM, M_BMEM),
    S2(0xA5, CF_00, TEXT_MOVSW,  SZ_WORD, M_WMEM, M_WMEM),
    T2(0xA4, CF_00, TEXT_MOVS,   SZ_BYTE, M_BMEM, M_BMEM),
    T2(0xA5, CF_00, TEXT_MOVS,   SZ_DATA, M_WMEM, M_WMEM),
    S0(0xA4, CF_00, TEXT_MOVSB,  SZ_NONE),
    S0(0xA5, CF_00, TEXT_MOVSW,  SZ_NONE),
    S2(0xA6, CF_00, TEXT_CMPSB,  SZ_BYTE, M_BMEM, M_BMEM),
    S2(0xA7, CF_00, TEXT_CMPSW,  SZ_WORD, M_WMEM, M_WMEM),
    T2(0xA6, CF_00, TEXT_CMPS,   SZ_BYTE, M_BMEM, M_BMEM),
    T2(0xA7, CF_00, TEXT_CMPS,   SZ_DATA, M_WMEM, M_WMEM),
    S0(0xA6, CF_00, TEXT_CMPSB,  SZ_NONE),
    S0(0xA7, CF_00, TEXT_CMPSW,  SZ_NONE),
    E2(0xA8, CF_00, TEXT_TEST,   SZ_BYTE, M_AL,   M_WIMM,  P_NONE, P_OPR),
    E2(0xA9, CF_00, TEXT_TEST,   SZ_DATA, M_AX,   M_WIMM,  P_NONE, P_OPR),
    S1(0xAA, CF_00, TEXT_STOSB,  SZ_BYTE, M_BMEM),
    S1(0xAB, CF_00, TEXT_STOSW,  SZ_WORD, M_WMEM),
    T1(0xAA, CF_00, TEXT_STOS,   SZ_BYTE, M_BMEM),
    T1(0xAB, CF_00, TEXT_STOS,   SZ_DATA, M_WMEM),
    S0(0xAA, CF_00, TEXT_STOSB,  SZ_NONE),
    S0(0xAB, CF_00, TEXT_STOSW,  SZ_NONE),
    S1(0xAC, CF_00, TEXT_LODSB,  SZ_BYTE, M_BMEM),
    S1(0xAD, CF_00, TEXT_LODSW,  SZ_WORD, M_WMEM),
    T1(0xAC, CF_00, TEXT_LODS,   SZ_BYTE, M_BMEM),
    T1(0xAD, CF_00, TEXT_LODS,   SZ_DATA, M_WMEM),
    S0(0xAC, CF_00, TEXT_LODSB,  SZ_NONE),
    S0(0xAD, CF_00, TEXT_LODSW,  SZ_NONE),
    S1(0xAE, CF_00, TEXT_SCASB,  SZ_BYTE, M_BMEM),
    S1(0xAF, CF_00, TEXT_SCASW,  SZ_WORD, M_WMEM),
    T1(0xAE, CF_00, TEXT_SCAS,   SZ_BYTE, M_BMEM),
    T1(0xAF, CF_00, TEXT_SCAS,   SZ_DATA, M_WMEM),
    S0(0xAE, CF_00, TEXT_SCASB,  SZ_NONE),
    S0(0xAF, CF_00, TEXT_SCASW,  SZ_NONE),
    E2(0xB0, CF_07, TEXT_MOV,    SZ_BYTE, M_BREG, M_WIMM, P_OREG, P_OPR),
    E2(0xB8, CF_07, TEXT_MOV,    SZ_DATA, M_WREG, M_WIMM, P_OREG, P_OPR),
    E1(0xC2, CF_00, TEXT_RET,    SZ_NONE, M_UI16, P_OPR),
    E0(0xC3, CF_00, TEXT_RET,    SZ_NONE),
    E2(0xC4, CF_00, TEXT_LES,    SZ_DATA, M_WREG, M_MEM, P_REG,  P_MOD),
    E2(0xC5, CF_00, TEXT_LDS,    SZ_DATA, M_WREG, M_MEM, P_REG,  P_MOD),
    E1(0xCA, CF_00, TEXT_RETF,   SZ_WORD, M_UI16, P_OPR), // FMODE_FAR
    E1(0xCA, CF_00, TEXT_RETF,   SZ_WORD, M_UI16, P_OPR), // FMODE_F
    E1(0xCA, CF_00, TEXT_LRET,   SZ_WORD, M_UI16, P_OPR), // FMODE_L
    E0(0xCB, CF_00, TEXT_RETF,   SZ_NONE),                // FMODE_FAR
    E0(0xCB, CF_00, TEXT_RETF,   SZ_NONE),                // FMODE_F
    E0(0xCB, CF_00, TEXT_LRET,   SZ_NONE),                // FMODE_L
    E1(0xCC, CF_00, TEXT_INT,    SZ_NONE, M_VAL3, P_NONE),
    E1(0xCD, CF_00, TEXT_INT,    SZ_BYTE, M_WIMM, P_OPR),
    E0(0xCE, CF_00, TEXT_INTO,   SZ_NONE),
    E0(0xCF, CF_00, TEXT_IRET,   SZ_NONE),
    E0(0xD7, CF_00, TEXT_XLAT,   SZ_NONE),
    E1(0xE0, CF_00, TEXT_LOOPNE, SZ_NONE, M_REL8, P_OPR),
    E1(0xE0, CF_00, TEXT_LOOPNZ, SZ_NONE, M_REL8, P_OPR),
    E1(0xE1, CF_00, TEXT_LOOPE,  SZ_NONE, M_REL8, P_OPR),
    E1(0xE1, CF_00, TEXT_LOOPZ,  SZ_NONE, M_REL8, P_OPR),
    E1(0xE2, CF_00, TEXT_LOOP,   SZ_NONE, M_REL8, P_OPR),
    E1(0xE3, CF_00, TEXT_JCXZ,   SZ_NONE, M_REL8, P_OPR),
    E2(0xE4, CF_00, TEXT_IN,     SZ_BYTE, M_AL,   M_IOA,  P_NONE, P_OPR),
    E2(0xE5, CF_00, TEXT_IN,     SZ_DATA, M_AX,   M_IOA,  P_NONE, P_OPR),
    E2(0xE6, CF_00, TEXT_OUT,    SZ_BYTE, M_IOA,  M_AL,   P_OPR,  P_NONE),
    E2(0xE7, CF_00, TEXT_OUT,    SZ_DATA, M_IOA,  M_AX,   P_OPR,  P_NONE),
    E1(0xE8, CF_00, TEXT_CALL,   SZ_NONE, M_REL,  P_OPR),
    E1(0xEB, CF_00, TEXT_JMP,    SZ_NONE, M_REL8, P_OPR),
    E1(0xE9, CF_00, TEXT_JMP,    SZ_NONE, M_REL,  P_OPR),
    E1(0xEA, CF_00, TEXT_JMP,    SZ_NONE, M_FAR,  P_OPR),
    E1(0xEA, CF_00, TEXT_JMPF,   SZ_NONE, M_FAR,  P_OPR),
    E2(0xEA, CF_00, TEXT_LJMP,   SZ_NONE, M_SEG,  M_OFF,  P_OPR,  P_OPR),
    E2(0xEC, CF_00, TEXT_IN,     SZ_BYTE, M_AL,   M_DX,   P_NONE, P_NONE),
    E2(0xED, CF_00, TEXT_IN,     SZ_DATA, M_AX,   M_DX,   P_NONE, P_NONE),
    E2(0xEE, CF_00, TEXT_OUT,    SZ_BYTE, M_DX,   M_AL,   P_NONE, P_NONE),
    E2(0xEF, CF_00, TEXT_OUT,    SZ_DATA, M_DX,   M_AX,   P_NONE, P_NONE),
    E0(0xF0, CF_00, TEXT_LOCK,   SZ_NONE),
    E0(0xF2, CF_00, TEXT_REPNE,  SZ_NONE),
    E0(0xF2, CF_00, TEXT_REPNZ,  SZ_NONE),
    E0(0xF3, CF_00, TEXT_REP,    SZ_NONE),
    E0(0xF3, CF_00, TEXT_REPE,   SZ_NONE),
    E0(0xF3, CF_00, TEXT_REPZ,   SZ_NONE),
    E0(0xF4, CF_00, TEXT_HLT,    SZ_NONE),
    E0(0xF5, CF_00, TEXT_CMC,    SZ_NONE),
    E0(0xF8, CF_00, TEXT_CLC,    SZ_NONE),
    E0(0xF9, CF_00, TEXT_STC,    SZ_NONE),
    E0(0xFA, CF_00, TEXT_CLI,    SZ_NONE),
    E0(0xFB, CF_00, TEXT_STI,    SZ_NONE),
    E0(0xFC, CF_00, TEXT_CLD,    SZ_NONE),
    E0(0xFD, CF_00, TEXT_STD,    SZ_NONE),
};

constexpr uint8_t I8086_00[] PROGMEM = {
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
    117,  // TEXT_CALL
    184,  // TEXT_CALL
    118,  // TEXT_CALLF
    115,  // TEXT_CBW
    202,  // TEXT_CLC
    206,  // TEXT_CLD
    204,  // TEXT_CLI
    201,  // TEXT_CMC
     58,  // TEXT_CMP
     59,  // TEXT_CMP
     60,  // TEXT_CMP
     61,  // TEXT_CMP
     62,  // TEXT_CMP
     63,  // TEXT_CMP
    133,  // TEXT_CMPS
    134,  // TEXT_CMPS
    131,  // TEXT_CMPSB
    135,  // TEXT_CMPSB
    132,  // TEXT_CMPSW
    136,  // TEXT_CMPSW
    116,  // TEXT_CWD
     41,  // TEXT_DAA
     49,  // TEXT_DAS
     67,  // TEXT_DEC
    200,  // TEXT_HLT
    180,  // TEXT_IN
    181,  // TEXT_IN
    190,  // TEXT_IN
    191,  // TEXT_IN
     66,  // TEXT_INC
    169,  // TEXT_INT
    170,  // TEXT_INT
    171,  // TEXT_INTO
    172,  // TEXT_IRET
     84,  // TEXT_JA
     75,  // TEXT_JAE
     72,  // TEXT_JB
     82,  // TEXT_JBE
     73,  // TEXT_JC
    179,  // TEXT_JCXZ
     78,  // TEXT_JE
     98,  // TEXT_JG
     94,  // TEXT_JGE
     92,  // TEXT_JL
     96,  // TEXT_JLE
    185,  // TEXT_JMP
    186,  // TEXT_JMP
    187,  // TEXT_JMP
    188,  // TEXT_JMPF
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
    124,  // TEXT_LAHF
    119,  // TEXT_LCALL
    162,  // TEXT_LDS
    113,  // TEXT_LEA
    161,  // TEXT_LES
    189,  // TEXT_LJMP
    194,  // TEXT_LOCK
    147,  // TEXT_LODS
    148,  // TEXT_LODS
    145,  // TEXT_LODSB
    149,  // TEXT_LODSB
    146,  // TEXT_LODSW
    150,  // TEXT_LODSW
    178,  // TEXT_LOOP
    176,  // TEXT_LOOPE
    174,  // TEXT_LOOPNE
    175,  // TEXT_LOOPNZ
    177,  // TEXT_LOOPZ
    165,  // TEXT_LRET
    168,  // TEXT_LRET
      0,  // TEXT_MOV
      1,  // TEXT_MOV
      2,  // TEXT_MOV
      3,  // TEXT_MOV
    108,  // TEXT_MOV
    109,  // TEXT_MOV
    110,  // TEXT_MOV
    111,  // TEXT_MOV
    112,  // TEXT_MOV
    114,  // TEXT_MOV
    157,  // TEXT_MOV
    158,  // TEXT_MOV
    127,  // TEXT_MOVS
    128,  // TEXT_MOVS
    125,  // TEXT_MOVSB
    129,  // TEXT_MOVSB
    126,  // TEXT_MOVSW
    130,  // TEXT_MOVSW
      4,  // TEXT_NOP
      9,  // TEXT_OR
     10,  // TEXT_OR
     18,  // TEXT_OR
     19,  // TEXT_OR
     20,  // TEXT_OR
     21,  // TEXT_OR
    182,  // TEXT_OUT
    183,  // TEXT_OUT
    192,  // TEXT_OUT
    193,  // TEXT_OUT
     16,  // TEXT_POP
     17,  // TEXT_POP
     69,  // TEXT_POP
    122,  // TEXT_POPF
     15,  // TEXT_PUSH
     68,  // TEXT_PUSH
    121,  // TEXT_PUSHF
    197,  // TEXT_REP
    198,  // TEXT_REPE
    195,  // TEXT_REPNE
    196,  // TEXT_REPNZ
    199,  // TEXT_REPZ
    159,  // TEXT_RET
    160,  // TEXT_RET
    163,  // TEXT_RETF
    164,  // TEXT_RETF
    166,  // TEXT_RETF
    167,  // TEXT_RETF
    123,  // TEXT_SAHF
     28,  // TEXT_SBB
     29,  // TEXT_SBB
     30,  // TEXT_SBB
     31,  // TEXT_SBB
     32,  // TEXT_SBB
     33,  // TEXT_SBB
    153,  // TEXT_SCAS
    154,  // TEXT_SCAS
    151,  // TEXT_SCASB
    155,  // TEXT_SCASB
    152,  // TEXT_SCASW
    156,  // TEXT_SCASW
     48,  // TEXT_SEGCS
     64,  // TEXT_SEGDS
     40,  // TEXT_SEGES
     56,  // TEXT_SEGSS
    203,  // TEXT_STC
    207,  // TEXT_STD
    205,  // TEXT_STI
    141,  // TEXT_STOS
    142,  // TEXT_STOS
    139,  // TEXT_STOSB
    143,  // TEXT_STOSB
    140,  // TEXT_STOSW
    144,  // TEXT_STOSW
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
    137,  // TEXT_TEST
    138,  // TEXT_TEST
    120,  // TEXT_WAIT
      5,  // TEXT_XCHG
      6,  // TEXT_XCHG
    104,  // TEXT_XCHG
    105,  // TEXT_XCHG
    106,  // TEXT_XCHG
    107,  // TEXT_XCHG
    173,  // TEXT_XLAT
     50,  // TEXT_XOR
     51,  // TEXT_XOR
     52,  // TEXT_XOR
     53,  // TEXT_XOR
     54,  // TEXT_XOR
     55,  // TEXT_XOR
};

constexpr Entry T8086_80[] PROGMEM = {
    M2(000, CF_00, TEXT_ADD, SZ_BYTE, M_BMOD, M_WIMM, P_OMOD, P_OPR),
    M2(010, CF_00, TEXT_OR,  SZ_BYTE, M_BMOD, M_WIMM, P_OMOD, P_OPR),
    M2(020, CF_00, TEXT_ADC, SZ_BYTE, M_BMOD, M_WIMM, P_OMOD, P_OPR),
    M2(030, CF_00, TEXT_SBB, SZ_BYTE, M_BMOD, M_WIMM, P_OMOD, P_OPR),
    M2(040, CF_00, TEXT_AND, SZ_BYTE, M_BMOD, M_WIMM, P_OMOD, P_OPR),
    M2(050, CF_00, TEXT_SUB, SZ_BYTE, M_BMOD, M_WIMM, P_OMOD, P_OPR),
    M2(060, CF_00, TEXT_XOR, SZ_BYTE, M_BMOD, M_WIMM, P_OMOD, P_OPR),
    N2(070, CF_00, TEXT_CMP, SZ_BYTE, M_BMOD, M_WIMM, P_OMOD, P_OPR),
};

constexpr uint8_t I8086_8X[] PROGMEM = {
      2,  // TEXT_ADC
      0,  // TEXT_ADD
      4,  // TEXT_AND
      7,  // TEXT_CMP
      1,  // TEXT_OR
      3,  // TEXT_SBB
      5,  // TEXT_SUB
      6,  // TEXT_XOR
};

constexpr Entry T8086_81[] PROGMEM = {
    M2(000, CF_00, TEXT_ADD, SZ_WORD, M_WMOD, M_WIMM, P_OMOD, P_OPR),
    M2(010, CF_00, TEXT_OR,  SZ_WORD, M_WMOD, M_WIMM, P_OMOD, P_OPR),
    M2(020, CF_00, TEXT_ADC, SZ_WORD, M_WMOD, M_WIMM, P_OMOD, P_OPR),
    M2(030, CF_00, TEXT_SBB, SZ_WORD, M_WMOD, M_WIMM, P_OMOD, P_OPR),
    M2(040, CF_00, TEXT_AND, SZ_WORD, M_WMOD, M_WIMM, P_OMOD, P_OPR),
    M2(050, CF_00, TEXT_SUB, SZ_WORD, M_WMOD, M_WIMM, P_OMOD, P_OPR),
    M2(060, CF_00, TEXT_XOR, SZ_WORD, M_WMOD, M_WIMM, P_OMOD, P_OPR),
    N2(070, CF_00, TEXT_CMP, SZ_WORD, M_WMOD, M_WIMM, P_OMOD, P_OPR),
};

constexpr Entry T8086_83[] PROGMEM = {
    M2(000, CF_00, TEXT_ADD, SZ_WORD, M_WMOD, M_BIMM, P_OMOD, P_OPR),
    M2(010, CF_00, TEXT_OR,  SZ_WORD, M_WMOD, M_BIMM, P_OMOD, P_OPR),
    M2(020, CF_00, TEXT_ADC, SZ_WORD, M_WMOD, M_BIMM, P_OMOD, P_OPR),
    M2(030, CF_00, TEXT_SBB, SZ_WORD, M_WMOD, M_BIMM, P_OMOD, P_OPR),
    M2(040, CF_00, TEXT_AND, SZ_WORD, M_WMOD, M_BIMM, P_OMOD, P_OPR),
    M2(050, CF_00, TEXT_SUB, SZ_WORD, M_WMOD, M_BIMM, P_OMOD, P_OPR),
    M2(060, CF_00, TEXT_XOR, SZ_WORD, M_WMOD, M_BIMM, P_OMOD, P_OPR),
    N2(070, CF_00, TEXT_CMP, SZ_WORD, M_WMOD, M_BIMM, P_OMOD, P_OPR),
};

constexpr Entry T8086_8F[] PROGMEM = {
    E1(000, CF_00, TEXT_POP, SZ_WORD, M_WMOD, P_OMOD),
};

constexpr uint8_t I8086_8F[] PROGMEM = {
      0,  // TEXT_POP
};

constexpr Entry T8086_C6[] PROGMEM = {
    N2(000, CF_00, TEXT_MOV, SZ_BYTE, M_BMOD, M_WIMM, P_OMOD, P_OPR),
};

constexpr uint8_t I8086_C6[] PROGMEM = {
      0,  // TEXT_MOV
};

constexpr Entry T8086_C7[] PROGMEM = {
    N2(000, CF_00, TEXT_MOV, SZ_DATA, M_WMOD, M_WIMM, P_OMOD, P_OPR),
};

constexpr uint8_t I8086_C7[] PROGMEM = {
      0,  // TEXT_MOV
};

constexpr Entry T8086_D0[] PROGMEM = {
    N2(000, CF_00, TEXT_ROL, SZ_BYTE, M_BMOD, M_VAL1, P_OMOD, P_NONE),
    N2(010, CF_00, TEXT_ROR, SZ_BYTE, M_BMOD, M_VAL1, P_OMOD, P_NONE),
    N2(020, CF_00, TEXT_RCL, SZ_BYTE, M_BMOD, M_VAL1, P_OMOD, P_NONE),
    N2(030, CF_00, TEXT_RCR, SZ_BYTE, M_BMOD, M_VAL1, P_OMOD, P_NONE),
    N2(040, CF_00, TEXT_SHL, SZ_BYTE, M_BMOD, M_VAL1, P_OMOD, P_NONE),
    N2(040, CF_00, TEXT_SAL, SZ_BYTE, M_BMOD, M_VAL1, P_OMOD, P_NONE),
    N2(050, CF_00, TEXT_SHR, SZ_BYTE, M_BMOD, M_VAL1, P_OMOD, P_NONE),
    N2(070, CF_00, TEXT_SAR, SZ_BYTE, M_BMOD, M_VAL1, P_OMOD, P_NONE),
};

constexpr uint8_t I8086_DX[] PROGMEM = {
      2,  // TEXT_RCL
      3,  // TEXT_RCR
      0,  // TEXT_ROL
      1,  // TEXT_ROR
      5,  // TEXT_SAL
      7,  // TEXT_SAR
      4,  // TEXT_SHL
      6,  // TEXT_SHR
};

constexpr Entry T8086_D1[] PROGMEM = {
    N2(000, CF_00, TEXT_ROL, SZ_DATA, M_WMOD, M_VAL1, P_OMOD, P_NONE),
    N2(010, CF_00, TEXT_ROR, SZ_DATA, M_WMOD, M_VAL1, P_OMOD, P_NONE),
    N2(020, CF_00, TEXT_RCL, SZ_DATA, M_WMOD, M_VAL1, P_OMOD, P_NONE),
    N2(030, CF_00, TEXT_RCR, SZ_DATA, M_WMOD, M_VAL1, P_OMOD, P_NONE),
    N2(040, CF_00, TEXT_SHL, SZ_DATA, M_WMOD, M_VAL1, P_OMOD, P_NONE),
    N2(040, CF_00, TEXT_SAL, SZ_DATA, M_WMOD, M_VAL1, P_OMOD, P_NONE),
    N2(050, CF_00, TEXT_SHR, SZ_DATA, M_WMOD, M_VAL1, P_OMOD, P_NONE),
    N2(070, CF_00, TEXT_SAR, SZ_DATA, M_WMOD, M_VAL1, P_OMOD, P_NONE),
};

constexpr Entry T8086_D2[] PROGMEM = {
    N2(000, CF_00, TEXT_ROL, SZ_BYTE, M_BMOD, M_CL, P_OMOD, P_NONE),
    N2(010, CF_00, TEXT_ROR, SZ_BYTE, M_BMOD, M_CL, P_OMOD, P_NONE),
    N2(020, CF_00, TEXT_RCL, SZ_BYTE, M_BMOD, M_CL, P_OMOD, P_NONE),
    N2(030, CF_00, TEXT_RCR, SZ_BYTE, M_BMOD, M_CL, P_OMOD, P_NONE),
    N2(040, CF_00, TEXT_SHL, SZ_BYTE, M_BMOD, M_CL, P_OMOD, P_NONE),
    N2(040, CF_00, TEXT_SAL, SZ_BYTE, M_BMOD, M_CL, P_OMOD, P_NONE),
    N2(050, CF_00, TEXT_SHR, SZ_BYTE, M_BMOD, M_CL, P_OMOD, P_NONE),
    N2(070, CF_00, TEXT_SAR, SZ_BYTE, M_BMOD, M_CL, P_OMOD, P_NONE),
};

constexpr Entry T8086_D3[] PROGMEM = {
    N2(000, CF_00, TEXT_ROL, SZ_DATA, M_WMOD, M_CL, P_OMOD, P_NONE),
    N2(010, CF_00, TEXT_ROR, SZ_DATA, M_WMOD, M_CL, P_OMOD, P_NONE),
    N2(020, CF_00, TEXT_RCL, SZ_DATA, M_WMOD, M_CL, P_OMOD, P_NONE),
    N2(030, CF_00, TEXT_RCR, SZ_DATA, M_WMOD, M_CL, P_OMOD, P_NONE),
    N2(040, CF_00, TEXT_SHL, SZ_DATA, M_WMOD, M_CL, P_OMOD, P_NONE),
    N2(040, CF_00, TEXT_SAL, SZ_DATA, M_WMOD, M_CL, P_OMOD, P_NONE),
    N2(050, CF_00, TEXT_SHR, SZ_DATA, M_WMOD, M_CL, P_OMOD, P_NONE),
    N2(070, CF_00, TEXT_SAR, SZ_DATA, M_WMOD, M_CL, P_OMOD, P_NONE),
};

constexpr Entry T8086_D4[] PROGMEM = {
    E0(0x0A, CF_00, TEXT_AAM, SZ_NONE),
};

constexpr uint8_t I8086_D4[] PROGMEM = {
      0,  // TEXT_AAM
};

constexpr Entry T8086_D5[] PROGMEM = {
    E0(0x0A, CF_00, TEXT_AAD, SZ_NONE),
};

constexpr uint8_t I8086_D5[] PROGMEM = {
      0,  // TEXT_AAD
};

constexpr Entry T8086_F6[] PROGMEM = {
    N2(000, CF_00, TEXT_TEST, SZ_BYTE, M_BMOD, M_WIMM, P_OMOD, P_OPR),
    M1(020, CF_00, TEXT_NOT,  SZ_BYTE, M_BMOD, P_OMOD),
    M1(030, CF_00, TEXT_NEG,  SZ_BYTE, M_BMOD, P_OMOD),
    N1(040, CF_00, TEXT_MUL,  SZ_BYTE, M_BMOD, P_OMOD),
    N1(050, CF_00, TEXT_IMUL, SZ_BYTE, M_BMOD, P_OMOD),
    N1(060, CF_00, TEXT_DIV,  SZ_BYTE, M_BMOD, P_OMOD),
    N1(070, CF_00, TEXT_IDIV, SZ_BYTE, M_BMOD, P_OMOD),
};

constexpr uint8_t I8086_FX[] PROGMEM = {
      5,  // TEXT_DIV
      6,  // TEXT_IDIV
      4,  // TEXT_IMUL
      3,  // TEXT_MUL
      2,  // TEXT_NEG
      1,  // TEXT_NOT
      0,  // TEXT_TEST
};

constexpr Entry T8086_F7[] PROGMEM = {
    N2(000, CF_00, TEXT_TEST, SZ_DATA, M_WMOD, M_WIMM, P_OMOD, P_OPR),
    M1(020, CF_00, TEXT_NOT,  SZ_DATA, M_WMOD, P_OMOD),
    M1(030, CF_00, TEXT_NEG,  SZ_DATA, M_WMOD, P_OMOD),
    N1(040, CF_00, TEXT_MUL,  SZ_DATA, M_WMOD, P_OMOD),
    N1(050, CF_00, TEXT_IMUL, SZ_DATA, M_WMOD, P_OMOD),
    N1(060, CF_00, TEXT_DIV,  SZ_DATA, M_WMOD, P_OMOD),
    N1(070, CF_00, TEXT_IDIV, SZ_DATA, M_WMOD, P_OMOD),
};

constexpr Entry T8086_FE[] PROGMEM = {
    M1(000, CF_00, TEXT_INC, SZ_BYTE, M_BMOD, P_OMOD),
    M1(010, CF_00, TEXT_DEC, SZ_BYTE, M_BMOD, P_OMOD),
};

constexpr uint8_t I8086_FE[] PROGMEM = {
      1,  // TEXT_DEC
      0,  // TEXT_INC
};

constexpr Entry T8086_FF[] PROGMEM = {
    M1(000, CF_00, TEXT_INC,   SZ_WORD, M_WMOD, P_OMOD),
    M1(010, CF_00, TEXT_DEC,   SZ_WORD, M_WMOD, P_OMOD),
    E1(020, CF_00, TEXT_CALL,  SZ_NONE, M_WMOD, P_OMOD),
    E1(030, CF_00, TEXT_CALL,  SZ_NONE, M_FARM, P_OMOD), // FMODE_FAR
    E1(030, CF_00, TEXT_CALLF, SZ_NONE, M_WMEM, P_OMOD), // FMODE_F
    E1(030, CF_00, TEXT_LCALL, SZ_NONE, M_WMEM, P_OMOD), // FMODE_L
    E1(040, CF_00, TEXT_JMP,   SZ_NONE, M_WMOD, P_OMOD),
    E1(050, CF_00, TEXT_JMP,   SZ_NONE, M_FARM, P_OMOD), // FMODE_FAR
    E1(050, CF_00, TEXT_JMPF,  SZ_NONE, M_WMEM, P_OMOD), // FMODE_F
    E1(050, CF_00, TEXT_LJMP,  SZ_NONE, M_WMEM, P_OMOD), // FMODE_L
    E1(060, CF_00, TEXT_PUSH,  SZ_WORD, M_WMOD, P_OMOD),
};

constexpr uint8_t I8086_FF[] PROGMEM = {
      2,  // TEXT_CALL
      3,  // TEXT_CALL
      4,  // TEXT_CALLF
      1,  // TEXT_DEC
      0,  // TEXT_INC
      6,  // TEXT_JMP
      7,  // TEXT_JMP
      8,  // TEXT_JMPF
      5,  // TEXT_LCALL
      9,  // TEXT_LJMP
     10,  // TEXT_PUSH
};

// i80186

constexpr Entry T80186_00[] PROGMEM = {
    E0(0x60, CF_00, TEXT_PUSHA, SZ_NONE),
    E0(0x61, CF_00, TEXT_POPA,  SZ_NONE),
    E2(0x62, CF_00, TEXT_BOUND, SZ_DATA, M_WREG, M_WMEM, P_REG,  P_MOD),
    E1(0x6A, CF_00, TEXT_PUSH,  SZ_BYTE, M_BIMM, P_OPR),
    E3(0x6B, CF_00, TEXT_IMUL,  SZ_DATA, M_WREG, M_WMOD, M_BIMM, P_REG, P_MOD, P_OPR),
    E2(0x6B, CF_00, TEXT_IMUL,  SZ_DATA, M_WREG, M_BIMM, P_MREG, P_OPR),
    E1(0x68, CF_00, TEXT_PUSH,  SZ_DATA, M_WIMM, P_OPR),
    E3(0x69, CF_00, TEXT_IMUL,  SZ_DATA, M_WREG, M_WMOD, M_WIMM, P_REG, P_MOD, P_OPR),
    E2(0x69, CF_00, TEXT_IMUL,  SZ_DATA, M_WREG, M_WIMM, P_MREG, P_OPR),
    S2(0x6C, CF_00, TEXT_INSB,  SZ_BYTE, M_BMEM, M_DX),
    S2(0x6D, CF_00, TEXT_INSW,  SZ_WORD, M_WMEM, M_DX),
    T2(0x6C, CF_00, TEXT_INS,   SZ_BYTE, M_BMEM, M_DX),
    T2(0x6D, CF_00, TEXT_INS,   SZ_DATA, M_WMEM, M_DX),
    S0(0x6C, CF_00, TEXT_INSB,  SZ_BYTE),
    S0(0x6D, CF_00, TEXT_INSW,  SZ_WORD),
    S2(0x6E, CF_00, TEXT_OUTSB, SZ_BYTE, M_DX,   M_BMEM),
    S2(0x6F, CF_00, TEXT_OUTSW, SZ_WORD, M_DX,   M_WMEM),
    T2(0x6E, CF_00, TEXT_OUTS,  SZ_BYTE, M_DX,   M_BMEM),
    T2(0x6F, CF_00, TEXT_OUTS,  SZ_DATA, M_DX,   M_WMEM),
    S0(0x6E, CF_00, TEXT_OUTSB, SZ_BYTE),
    S0(0x6F, CF_00, TEXT_OUTSW, SZ_WORD),
    E2(0xC8, CF_00, TEXT_ENTER, SZ_NONE, M_UI16, M_UI8,  P_OPR,  P_OPR),
    E0(0xC9, CF_00, TEXT_LEAVE, SZ_NONE),
};

constexpr uint8_t I80186_00[] PROGMEM = {
      2,  // TEXT_BOUND
     21,  // TEXT_ENTER
      4,  // TEXT_IMUL
      5,  // TEXT_IMUL
      7,  // TEXT_IMUL
      8,  // TEXT_IMUL
     11,  // TEXT_INS
     12,  // TEXT_INS
      9,  // TEXT_INSB
     13,  // TEXT_INSB
     10,  // TEXT_INSW
     14,  // TEXT_INSW
     22,  // TEXT_LEAVE
     17,  // TEXT_OUTS
     18,  // TEXT_OUTS
     15,  // TEXT_OUTSB
     19,  // TEXT_OUTSB
     16,  // TEXT_OUTSW
     20,  // TEXT_OUTSW
      1,  // TEXT_POPA
      3,  // TEXT_PUSH
      6,  // TEXT_PUSH
      0,  // TEXT_PUSHA
};

constexpr Entry T80186_C0[] PROGMEM = {
    N2(000, CF_00, TEXT_ROL, SZ_BYTE, M_BMOD, M_BIT, P_OMOD, P_OPR),
    N2(010, CF_00, TEXT_ROR, SZ_BYTE, M_BMOD, M_BIT, P_OMOD, P_OPR),
    N2(020, CF_00, TEXT_RCL, SZ_BYTE, M_BMOD, M_BIT, P_OMOD, P_OPR),
    N2(030, CF_00, TEXT_RCR, SZ_BYTE, M_BMOD, M_BIT, P_OMOD, P_OPR),
    N2(040, CF_00, TEXT_SHL, SZ_BYTE, M_BMOD, M_BIT, P_OMOD, P_OPR),
    N2(040, CF_00, TEXT_SAL, SZ_BYTE, M_BMOD, M_BIT, P_OMOD, P_OPR),
    N2(050, CF_00, TEXT_SHR, SZ_BYTE, M_BMOD, M_BIT, P_OMOD, P_OPR),
    N2(070, CF_00, TEXT_SAR, SZ_BYTE, M_BMOD, M_BIT, P_OMOD, P_OPR),
};

constexpr uint8_t I80186_CX[] PROGMEM = {
      2,  // TEXT_RCL
      3,  // TEXT_RCR
      0,  // TEXT_ROL
      1,  // TEXT_ROR
      5,  // TEXT_SAL
      7,  // TEXT_SAR
      4,  // TEXT_SHL
      6,  // TEXT_SHR
};

constexpr Entry T80186_C1[] PROGMEM = {
    N2(000, CF_00, TEXT_ROL, SZ_DATA, M_WMOD, M_BIT, P_OMOD, P_OPR),
    N2(010, CF_00, TEXT_ROR, SZ_DATA, M_WMOD, M_BIT, P_OMOD, P_OPR),
    N2(020, CF_00, TEXT_RCL, SZ_DATA, M_WMOD, M_BIT, P_OMOD, P_OPR),
    N2(030, CF_00, TEXT_RCR, SZ_DATA, M_WMOD, M_BIT, P_OMOD, P_OPR),
    N2(040, CF_00, TEXT_SHL, SZ_DATA, M_WMOD, M_BIT, P_OMOD, P_OPR),
    N2(040, CF_00, TEXT_SAL, SZ_DATA, M_WMOD, M_BIT, P_OMOD, P_OPR),
    N2(050, CF_00, TEXT_SHR, SZ_DATA, M_WMOD, M_BIT, P_OMOD, P_OPR),
    N2(070, CF_00, TEXT_SAR, SZ_DATA, M_WMOD, M_BIT, P_OMOD, P_OPR),
};

// i80286

constexpr Entry T80286_00[] PROGMEM = {
    E2(0x63, CF_00, TEXT_ARPL,  SZ_WORD, M_WMOD, M_WREG, P_MOD, P_REG),
    E1(0xD7, CF_00, TEXT_XLAT,  SZ_BYTE, M_BMEM, P_NONE),
    E0(0xD7, CF_00, TEXT_XLATB, SZ_NONE),
};

constexpr uint8_t I80286_00[] PROGMEM = {
      0,  // TEXT_ARPL
      1,  // TEXT_XLAT
      2,  // TEXT_XLATB
};

constexpr Entry T80286_0F[] PROGMEM = {
    E0(0x06, CF_00, TEXT_CLTS, SZ_NONE),
    E2(0x02, CF_00, TEXT_LAR,  SZ_DATA, M_WREG, M_WMOD, P_REG, P_MOD),
    E2(0x03, CF_00, TEXT_LSL,  SZ_DATA, M_WREG, M_WMOD, P_REG, P_MOD),
};

constexpr uint8_t I80286_0F[] PROGMEM = {
      0,  // TEXT_CLTS
      1,  // TEXT_LAR
      2,  // TEXT_LSL
};

constexpr Entry T80286_0F00[] PROGMEM = {
    E1(000, CF_00, TEXT_SLDT, SZ_WORD, M_WMOD, P_OMOD),
    E1(010, CF_00, TEXT_STR,  SZ_WORD, M_WMOD, P_OMOD),
    E1(020, CF_00, TEXT_LLDT, SZ_WORD, M_WMOD, P_OMOD),
    E1(030, CF_00, TEXT_LTR,  SZ_WORD, M_WMOD, P_OMOD),
    E1(040, CF_00, TEXT_VERR, SZ_WORD, M_WMOD, P_OMOD),
    E1(050, CF_00, TEXT_VERW, SZ_WORD, M_WMOD, P_OMOD),
};

constexpr uint8_t I80286_0F00[] PROGMEM = {
      2,  // TEXT_LLDT
      3,  // TEXT_LTR
      0,  // TEXT_SLDT
      1,  // TEXT_STR
      4,  // TEXT_VERR
      5,  // TEXT_VERW
};

constexpr Entry T80286_0F01[] PROGMEM = {
    E1(000, CF_00, TEXT_SGDT, SZ_NONE, M_MEM,  P_OMOD),
    E1(010, CF_00, TEXT_SIDT, SZ_NONE, M_MEM,  P_OMOD),
    E1(020, CF_00, TEXT_LGDT, SZ_NONE, M_MEM,  P_OMOD),
    E1(030, CF_00, TEXT_LIDT, SZ_NONE, M_MEM,  P_OMOD),
    E1(040, CF_00, TEXT_SMSW, SZ_WORD, M_WMOD, P_OMOD),
    E1(060, CF_00, TEXT_LMSW, SZ_WORD, M_WMOD, P_OMOD),
};

constexpr uint8_t I80286_0F01[] PROGMEM = {
      2,  // TEXT_LGDT
      3,  // TEXT_LIDT
      5,  // TEXT_LMSW
      0,  // TEXT_SGDT
      1,  // TEXT_SIDT
      4,  // TEXT_SMSW
};

// i80386

constexpr Entry T80386_00[] PROGMEM = {
    E0(0x64, CF_00, TEXT_SEGFS, SZ_NONE),
    E0(0x65, CF_00, TEXT_SEGGS, SZ_NONE),
};

constexpr uint8_t I80386_00[] PROGMEM = {
    /* empty */
};

constexpr Entry T80386_8F[] PROGMEM = {
    N1(000, CF_00, TEXT_POP, SZ_DATA, M_WMOD, P_OMOD),
};

constexpr Entry T80386_FF[] PROGMEM = {
    M1(000, CF_00, TEXT_INC,   SZ_DATA, M_WMOD, P_OMOD),
    M1(010, CF_00, TEXT_DEC,   SZ_DATA, M_WMOD, P_OMOD),
    N1(020, CF_00, TEXT_CALL,  SZ_DATA, M_WMOD, P_OMOD),
    N1(030, CF_00, TEXT_CALL,  SZ_DATA, M_FARM, P_OMOD),
    E1(030, CF_00, TEXT_CALLF, SZ_DATA, M_WMEM, P_OMOD),
    E1(030, CF_00, TEXT_LCALL, SZ_DATA, M_WMEM, P_OMOD),
    N1(040, CF_00, TEXT_JMP,   SZ_DATA, M_WMOD, P_OMOD),
    N1(050, CF_00, TEXT_JMP,   SZ_DATA, M_FARM, P_OMOD),
    E1(050, CF_00, TEXT_JMPF,  SZ_DATA, M_WMEM, P_OMOD),
    E1(050, CF_00, TEXT_LJMP,  SZ_DATA, M_WMEM, P_OMOD),
    N1(060, CF_00, TEXT_PUSH,  SZ_DATA, M_WMOD, P_OMOD),
};

constexpr Entry TCODE16_00[] PROGMEM = {
    E0(0x60, CF_00, TEXT_PUSHA, SZ_NONE),
    E0(0x61, CF_00, TEXT_POPA,  SZ_NONE),
    S2(0x6D, CF_00, TEXT_INSW,  SZ_WORD, M_WMEM, M_DX),
    S2(0x6F, CF_00, TEXT_OUTSW, SZ_WORD, M_DX,   M_WMEM),
    E0(0x98, CF_00, TEXT_CBW,   SZ_NONE),
    E0(0x99, CF_00, TEXT_CWD,   SZ_NONE),
    E0(0x9C, CF_00, TEXT_PUSHF, SZ_NONE),
    E0(0x9D, CF_00, TEXT_POPF,  SZ_NONE),
    S2(0xA5, CF_00, TEXT_MOVSW, SZ_WORD, M_DMEM, M_DMEM),
    S0(0xA5, CF_00, TEXT_MOVSW, SZ_NONE),
    S2(0xA7, CF_00, TEXT_CMPSW, SZ_WORD, M_DMEM, M_DMEM),
    S0(0xA7, CF_00, TEXT_CMPSW, SZ_NONE),
    S1(0xAB, CF_00, TEXT_STOSW, SZ_WORD, M_DMEM),
    S0(0xAB, CF_00, TEXT_STOSW, SZ_NONE),
    S1(0xAD, CF_00, TEXT_LODSW, SZ_WORD, M_DMEM),
    S0(0xAD, CF_00, TEXT_LODSW, SZ_NONE),
    S1(0xAF, CF_00, TEXT_SCASW, SZ_WORD, M_DMEM),
    S0(0xAF, CF_00, TEXT_SCASW, SZ_NONE),
    E0(0xCF, CF_00, TEXT_IRET,  SZ_WORD),
    E1(0xE3, CF_00, TEXT_JCXZ,  SZ_NONE,  M_REL8, P_OPR),
};

constexpr Entry TCODE32_00[] PROGMEM = {
    E0(0x60, CF_00, TEXT_PUSHAD, SZ_NONE),
    E0(0x61, CF_00, TEXT_POPAD,  SZ_NONE),
    S2(0x6D, CF_00, TEXT_INSD,   SZ_DWORD, M_DMEM, M_DX),
    S2(0x6F, CF_00, TEXT_OUTSD,  SZ_DWORD, M_DX,   M_DMEM),
    E0(0x98, CF_00, TEXT_CWDE,   SZ_NONE),
    E0(0x99, CF_00, TEXT_CDQ,    SZ_NONE),
    E0(0x9C, CF_00, TEXT_PUSHFD, SZ_NONE),
    E0(0x9D, CF_00, TEXT_POPFD,  SZ_NONE),
    S2(0xA5, CF_00, TEXT_MOVSD,  SZ_DWORD, M_DMEM, M_DMEM),
    S0(0xA5, CF_00, TEXT_MOVSD,  SZ_NONE),
    S2(0xA7, CF_00, TEXT_CMPSD,  SZ_DWORD, M_DMEM, M_DMEM),
    S0(0xA7, CF_00, TEXT_CMPSD,  SZ_NONE),
    S1(0xAB, CF_00, TEXT_STOSD,  SZ_DWORD, M_DMEM),
    S0(0xAB, CF_00, TEXT_STOSD,  SZ_NONE),
    S1(0xAD, CF_00, TEXT_LODSD,  SZ_DWORD, M_DMEM),
    S0(0xAD, CF_00, TEXT_LODSD,  SZ_NONE),
    S1(0xAF, CF_00, TEXT_SCASD,  SZ_DWORD, M_DMEM),
    S0(0xAF, CF_00, TEXT_SCASD,  SZ_NONE),
    E0(0xCF, CF_00, TEXT_IRETD,  SZ_NONE),
    E1(0xE3, CF_00, TEXT_JECXZ,  SZ_NONE,  M_REL8, P_OPR),
};

constexpr uint8_t ICODE16_00[] PROGMEM = {
    /* empty */
};

constexpr Entry T80386_0F[] PROGMEM = {
    E2(0x20, CF_00, TEXT_MOV,  SZ_DWORD, M_DREG, M_CTLR, P_MOD, P_REG),
    E2(0x21, CF_00, TEXT_MOV,  SZ_DWORD, M_DREG, M_DBGR, P_MOD, P_REG),
    E2(0x22, CF_00, TEXT_MOV,  SZ_DWORD, M_CTLR, M_DREG, P_REG, P_MOD),
    E2(0x23, CF_00, TEXT_MOV,  SZ_DWORD, M_DBGR, M_DREG, P_REG, P_MOD),
    E2(0x24, CF_00, TEXT_MOV,  SZ_DWORD, M_DREG, M_TSTR, P_MOD, P_REG),
    E2(0x26, CF_00, TEXT_MOV,  SZ_DWORD, M_TSTR, M_DREG, P_REG, P_MOD),
    E1(0x80, CF_00, TEXT_JO,   SZ_NONE, M_REL, P_OPR),
    E1(0x81, CF_00, TEXT_JNO,  SZ_NONE, M_REL, P_OPR),
    E1(0x82, CF_00, TEXT_JB,   SZ_NONE, M_REL, P_OPR),
    E1(0x82, CF_00, TEXT_JC,   SZ_NONE, M_REL, P_OPR),
    E1(0x82, CF_00, TEXT_JNAE, SZ_NONE, M_REL, P_OPR),
    E1(0x83, CF_00, TEXT_JAE,  SZ_NONE, M_REL, P_OPR),
    E1(0x83, CF_00, TEXT_JNB,  SZ_NONE, M_REL, P_OPR),
    E1(0x83, CF_00, TEXT_JNC,  SZ_NONE, M_REL, P_OPR),
    E1(0x84, CF_00, TEXT_JE,   SZ_NONE, M_REL, P_OPR),
    E1(0x84, CF_00, TEXT_JZ,   SZ_NONE, M_REL, P_OPR),
    E1(0x85, CF_00, TEXT_JNE,  SZ_NONE, M_REL, P_OPR),
    E1(0x85, CF_00, TEXT_JNZ,  SZ_NONE, M_REL, P_OPR),
    E1(0x86, CF_00, TEXT_JBE,  SZ_NONE, M_REL, P_OPR),
    E1(0x86, CF_00, TEXT_JNA,  SZ_NONE, M_REL, P_OPR),
    E1(0x87, CF_00, TEXT_JA,   SZ_NONE, M_REL, P_OPR),
    E1(0x87, CF_00, TEXT_JNBE, SZ_NONE, M_REL, P_OPR),
    E1(0x88, CF_00, TEXT_JS,   SZ_NONE, M_REL, P_OPR),
    E1(0x89, CF_00, TEXT_JNS,  SZ_NONE, M_REL, P_OPR),
    E1(0x8A, CF_00, TEXT_JPE,  SZ_NONE, M_REL, P_OPR),
    E1(0x8A, CF_00, TEXT_JP,   SZ_NONE, M_REL, P_OPR),
    E1(0x8B, CF_00, TEXT_JPO,  SZ_NONE, M_REL, P_OPR),
    E1(0x8B, CF_00, TEXT_JNP,  SZ_NONE, M_REL, P_OPR),
    E1(0x8C, CF_00, TEXT_JL,   SZ_NONE, M_REL, P_OPR),
    E1(0x8C, CF_00, TEXT_JNGE, SZ_NONE, M_REL, P_OPR),
    E1(0x8D, CF_00, TEXT_JGE,  SZ_NONE, M_REL, P_OPR),
    E1(0x8D, CF_00, TEXT_JNL,  SZ_NONE, M_REL, P_OPR),
    E1(0x8E, CF_00, TEXT_JLE,  SZ_NONE, M_REL, P_OPR),
    E1(0x8E, CF_00, TEXT_JNG,  SZ_NONE, M_REL, P_OPR),
    E1(0x8F, CF_00, TEXT_JG,   SZ_NONE, M_REL, P_OPR),
    E1(0x8F, CF_00, TEXT_JNLE, SZ_NONE, M_REL, P_OPR),
    E1(0x90, CF_00, TEXT_SETO,   SZ_BYTE, M_BMOD, P_MOD),
    E1(0x91, CF_00, TEXT_SETNO,  SZ_BYTE, M_BMOD, P_MOD),
    E1(0x92, CF_00, TEXT_SETB,   SZ_BYTE, M_BMOD, P_MOD),
    E1(0x92, CF_00, TEXT_SETC,   SZ_BYTE, M_BMOD, P_MOD),
    E1(0x92, CF_00, TEXT_SETNAE, SZ_BYTE, M_BMOD, P_MOD),
    E1(0x93, CF_00, TEXT_SETAE,  SZ_BYTE, M_BMOD, P_MOD),
    E1(0x93, CF_00, TEXT_SETNB,  SZ_BYTE, M_BMOD, P_MOD),
    E1(0x93, CF_00, TEXT_SETNC,  SZ_BYTE, M_BMOD, P_MOD),
    E1(0x94, CF_00, TEXT_SETE,   SZ_BYTE, M_BMOD, P_MOD),
    E1(0x94, CF_00, TEXT_SETZ,   SZ_BYTE, M_BMOD, P_MOD),
    E1(0x95, CF_00, TEXT_SETNE,  SZ_BYTE, M_BMOD, P_MOD),
    E1(0x95, CF_00, TEXT_SETNZ,  SZ_BYTE, M_BMOD, P_MOD),
    E1(0x96, CF_00, TEXT_SETBE,  SZ_BYTE, M_BMOD, P_MOD),
    E1(0x96, CF_00, TEXT_SETNA,  SZ_BYTE, M_BMOD, P_MOD),
    E1(0x97, CF_00, TEXT_SETA,   SZ_BYTE, M_BMOD, P_MOD),
    E1(0x97, CF_00, TEXT_SETNBE, SZ_BYTE, M_BMOD, P_MOD),
    E1(0x98, CF_00, TEXT_SETS,   SZ_BYTE, M_BMOD, P_MOD),
    E1(0x99, CF_00, TEXT_SETNS,  SZ_BYTE, M_BMOD, P_MOD),
    E1(0x9A, CF_00, TEXT_SETPE,  SZ_BYTE, M_BMOD, P_MOD),
    E1(0x9A, CF_00, TEXT_SETP,   SZ_BYTE, M_BMOD, P_MOD),
    E1(0x9B, CF_00, TEXT_SETPO,  SZ_BYTE, M_BMOD, P_MOD),
    E1(0x9B, CF_00, TEXT_SETNP,  SZ_BYTE, M_BMOD, P_MOD),
    E1(0x9C, CF_00, TEXT_SETL,   SZ_BYTE, M_BMOD, P_MOD),
    E1(0x9C, CF_00, TEXT_SETNGE, SZ_BYTE, M_BMOD, P_MOD),
    E1(0x9D, CF_00, TEXT_SETGE,  SZ_BYTE, M_BMOD, P_MOD),
    E1(0x9D, CF_00, TEXT_SETNL,  SZ_BYTE, M_BMOD, P_MOD),
    E1(0x9E, CF_00, TEXT_SETLE,  SZ_BYTE, M_BMOD, P_MOD),
    E1(0x9E, CF_00, TEXT_SETNG,  SZ_BYTE, M_BMOD, P_MOD),
    E1(0x9F, CF_00, TEXT_SETG,   SZ_BYTE, M_BMOD, P_MOD),
    E1(0x9F, CF_00, TEXT_SETNLE, SZ_BYTE, M_BMOD, P_MOD),
    E1(0xA0, CF_00, TEXT_PUSH,   SZ_WORD, M_FS,   P_NONE),
    E1(0xA1, CF_00, TEXT_POP,    SZ_WORD, M_FS,   P_NONE),
    E2(0xA3, CF_00, TEXT_BT,     SZ_DATA, M_WMOD, M_WREG, P_MOD,  P_REG),
    E3(0xA4, CF_00, TEXT_SHLD,   SZ_DATA, M_WMOD, M_WREG, M_BIT,  P_MOD, P_REG, P_OPR),
    E3(0xA5, CF_00, TEXT_SHLD,   SZ_DATA, M_WMOD, M_WREG, M_CL,   P_MOD, P_REG, P_NONE),
    E1(0xA8, CF_00, TEXT_PUSH,   SZ_WORD, M_GS,   P_NONE),
    E1(0xA9, CF_00, TEXT_POP,    SZ_WORD, M_GS,   P_NONE),
    E2(0xAB, CF_00, TEXT_BTS,    SZ_DATA, M_WMOD, M_WREG, P_MOD,  P_REG),
    E3(0xAC, CF_00, TEXT_SHRD,   SZ_DATA, M_WMOD, M_WREG, M_BIT,  P_MOD, P_REG, P_OPR),
    E3(0xAD, CF_00, TEXT_SHRD,   SZ_DATA, M_WMOD, M_WREG, M_CL,   P_MOD, P_REG, P_NONE),
    E2(0xAF, CF_00, TEXT_IMUL,   SZ_DATA, M_WREG, M_WMOD, P_REG,  P_MOD),
    E2(0xB2, CF_00, TEXT_LSS,    SZ_DATA, M_WREG, M_MEM, P_REG,  P_MOD),
    E2(0xB3, CF_00, TEXT_BTR,    SZ_DATA, M_WMOD, M_WREG, P_MOD,  P_REG),
    E2(0xB4, CF_00, TEXT_LFS,    SZ_DATA, M_WREG, M_MEM, P_REG,  P_MOD),
    E2(0xB5, CF_00, TEXT_LGS,    SZ_DATA, M_WREG, M_MEM, P_REG,  P_MOD),
    E2(0xBB, CF_00, TEXT_BTC,    SZ_DATA, M_WMOD, M_WREG, P_REG,  P_MOD),
    E2(0xBC, CF_00, TEXT_BSF,    SZ_DATA, M_WREG, M_WMOD, P_REG,  P_MOD),
    E2(0xBD, CF_00, TEXT_BSR,    SZ_DATA, M_WREG, M_WMOD, P_REG,  P_MOD),
    N2(0xB6, CF_00, TEXT_MOVZX,  SZ_DATA, M_WREG, M_BMOD, P_REG,  P_MOD),
    N2(0xB7, CF_00, TEXT_MOVZX,  SZ_WORD, M_DREG, M_WMOD, P_REG,  P_MOD),
    N2(0xBE, CF_00, TEXT_MOVSX,  SZ_DATA, M_WREG, M_BMOD, P_REG,  P_MOD),
    N2(0xBF, CF_00, TEXT_MOVSX,  SZ_WORD, M_DREG, M_WMOD, P_REG,  P_MOD),
};

constexpr uint8_t I80386_0F[] PROGMEM = {
    /* empty */
};

constexpr Entry T80386_0FBA[] PROGMEM = {
    N2(040, CF_00, TEXT_BT,  SZ_DATA, M_WMOD, M_UI8, P_OMOD, P_OPR),
    N2(050, CF_00, TEXT_BTS, SZ_DATA, M_WMOD, M_UI8, P_OMOD, P_OPR),
    N2(060, CF_00, TEXT_BTR, SZ_DATA, M_WMOD, M_UI8, P_OMOD, P_OPR),
    N2(070, CF_00, TEXT_BTC, SZ_DATA, M_WMOD, M_UI8, P_OMOD, P_OPR),
};

constexpr uint8_t I80386_0FBA[] PROGMEM = {
    /* empty */
};

// V30

constexpr Entry TV30_00[] PROGMEM = {
    E0(0x64, CF_00, TEXT_REPNC,  SZ_NONE),
    E0(0x65, CF_00, TEXT_REPC,   SZ_NONE),
};

constexpr uint8_t IV30_00[] PROGMEM = {
      1,  // TEXT_REPC
      0,  // TEXT_REPNC
};

constexpr Entry TV30_0F[] PROGMEM = {
    E1(0xFF, CF_00, TEXT_BRKEM,  SZ_BYTE, M_UI8,  P_OPR),
    S2(0x20, CF_00, TEXT_ADD4S,  SZ_BYTE, M_BMEM, M_BMEM),
    S0(0x20, CF_00, TEXT_ADD4S,  SZ_NONE),
    S2(0x26, CF_00, TEXT_CMP4S,  SZ_BYTE, M_BMEM, M_BMEM),
    S0(0x26, CF_00, TEXT_CMP4S,  SZ_NONE),
    S2(0x22, CF_00, TEXT_SUB4S,  SZ_BYTE, M_BMEM, M_BMEM),
    S0(0x22, CF_00, TEXT_SUB4S,  SZ_NONE),
    E1(0x28, CF_00, TEXT_ROL4,   SZ_BYTE, M_BMOD, P_MOD),
    E1(0x2A, CF_00, TEXT_ROR4,   SZ_BYTE, M_BMOD, P_MOD),
    E2(0x31, CF_00, TEXT_INS,    SZ_WORD, M_BREG, M_BREG, P_MOD,  P_REG),
    E2(0x39, CF_00, TEXT_INS,    SZ_WORD, M_BREG, M_BIT,  P_MOD,  P_OPR),
    E2(0x33, CF_00, TEXT_EXT,    SZ_WORD, M_BREG, M_BREG, P_MOD,  P_REG),
    E2(0x3B, CF_00, TEXT_EXT,    SZ_WORD, M_BREG, M_BIT,  P_MOD,  P_OPR),
    N2(0x12, CF_00, TEXT_CLR1,   SZ_BYTE, M_BMOD, M_CL,   P_MOD,  P_NONE),
    N2(0x13, CF_00, TEXT_CLR1,   SZ_WORD, M_WMOD, M_CL,   P_MOD,  P_NONE),
    N2(0x1A, CF_00, TEXT_CLR1,   SZ_BYTE, M_BMOD, M_BIT,  P_MOD,  P_OPR), 
    N2(0x1B, CF_00, TEXT_CLR1,   SZ_WORD, M_WMOD, M_BIT,  P_MOD,  P_OPR), 
    N2(0x14, CF_00, TEXT_SET1,   SZ_BYTE, M_BMOD, M_CL,   P_MOD,  P_NONE),
    N2(0x15, CF_00, TEXT_SET1,   SZ_WORD, M_WMOD, M_CL,   P_MOD,  P_NONE),
    N2(0x1C, CF_00, TEXT_SET1,   SZ_BYTE, M_BMOD, M_BIT,  P_MOD,  P_OPR), 
    N2(0x1D, CF_00, TEXT_SET1,   SZ_WORD, M_WMOD, M_BIT,  P_MOD,  P_OPR), 
    N2(0x16, CF_00, TEXT_NOT1,   SZ_BYTE, M_BMOD, M_CL,   P_MOD,  P_NONE),
    N2(0x17, CF_00, TEXT_NOT1,   SZ_WORD, M_WMOD, M_CL,   P_MOD,  P_NONE),
    N2(0x1E, CF_00, TEXT_NOT1,   SZ_BYTE, M_BMOD, M_BIT,  P_MOD,  P_OPR), 
    N2(0x1F, CF_00, TEXT_NOT1,   SZ_WORD, M_WMOD, M_BIT,  P_MOD,  P_OPR), 
};

constexpr uint8_t IV30_0F[] PROGMEM = {
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

#if !defined(LIBASM_I8086_NOFPU)
constexpr Entry T8087_00[] PROGMEM = {
    E0(0x9B, CF_00, TEXT_FWAIT,  SZ_NONE),
};

constexpr uint8_t I8087_00[] PROGMEM = {
      0,  // TEXT_FWAIT
};

constexpr Entry T8087_D8[] PROGMEM = {
    E2(0xC0, CF_07, TEXT_FADD,   SZ_NONE,  M_ST0,  M_STI,  P_NONE, P_OREG),
    E2(0xC8, CF_07, TEXT_FMUL,   SZ_NONE,  M_ST0,  M_STI,  P_NONE, P_OREG),
    E1(0xD0, CF_07, TEXT_FCOM,   SZ_NONE,  M_STI,  P_OREG),
    E1(0xD8, CF_07, TEXT_FCOMP,  SZ_NONE,  M_STI,  P_OREG),
    E2(0xE0, CF_07, TEXT_FSUB,   SZ_NONE,  M_ST0,  M_STI,  P_NONE, P_OREG),
    E2(0xE8, CF_07, TEXT_FSUBR,  SZ_NONE,  M_ST0,  M_STI,  P_NONE, P_OREG),
    E2(0xF0, CF_07, TEXT_FDIV,   SZ_NONE,  M_ST0,  M_STI,  P_NONE, P_OREG),
    E2(0xF8, CF_07, TEXT_FDIVR,  SZ_NONE,  M_ST0,  M_STI,  P_NONE, P_OREG),
    N1(000,  CF_00, TEXT_FADD,   SZ_DWORD, M_DMEM, P_OMOD),
    N1(010,  CF_00, TEXT_FMUL,   SZ_DWORD, M_DMEM, P_OMOD),
    N1(020,  CF_00, TEXT_FCOM,   SZ_DWORD, M_DMEM, P_OMOD),
    N1(030,  CF_00, TEXT_FCOMP,  SZ_DWORD, M_DMEM, P_OMOD),
    N1(040,  CF_00, TEXT_FSUB,   SZ_DWORD, M_DMEM, P_OMOD),
    N1(050,  CF_00, TEXT_FSUBR,  SZ_DWORD, M_DMEM, P_OMOD),
    N1(060,  CF_00, TEXT_FDIV,   SZ_DWORD, M_DMEM, P_OMOD),
    N1(070,  CF_00, TEXT_FDIVR,  SZ_DWORD, M_DMEM, P_OMOD),
};

constexpr uint8_t I8087_D8[] PROGMEM = {
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

constexpr Entry T8087_D9[] PROGMEM = {
    E1(0xC0, CF_07, TEXT_FLD,     SZ_NONE,  M_STI,  P_OREG),
    E1(0xC8, CF_07, TEXT_FXCH,    SZ_NONE,  M_STI,  P_OREG),
    E0(0xD0, CF_00, TEXT_FNOP,    SZ_NONE),
    E0(0xE0, CF_00, TEXT_FCHS,    SZ_NONE),
    E0(0xE1, CF_00, TEXT_FABS,    SZ_NONE),
    E0(0xE4, CF_00, TEXT_FTST,    SZ_NONE),
    E0(0xE5, CF_00, TEXT_FXAM,    SZ_NONE),
    E0(0xE8, CF_00, TEXT_FLD1,    SZ_NONE),
    E0(0xE9, CF_00, TEXT_FLDL2T,  SZ_NONE),
    E0(0xEA, CF_00, TEXT_FLDL2E,  SZ_NONE),
    E0(0xEB, CF_00, TEXT_FLDPI,   SZ_NONE),
    E0(0xEC, CF_00, TEXT_FLDLG2,  SZ_NONE),
    E0(0xED, CF_00, TEXT_FLDLN2,  SZ_NONE),
    E0(0xEE, CF_00, TEXT_FLDZ,    SZ_NONE),
    E0(0xF0, CF_00, TEXT_F2XM1,   SZ_NONE),
    E0(0xF1, CF_00, TEXT_FYL2X,   SZ_NONE),
    E0(0xF2, CF_00, TEXT_FPTAN,   SZ_NONE),
    E0(0xF3, CF_00, TEXT_FPATAN,  SZ_NONE),
    E0(0xF4, CF_00, TEXT_FXTRACT, SZ_NONE),
    E0(0xF6, CF_00, TEXT_FDECSTP, SZ_NONE),
    E0(0xF7, CF_00, TEXT_FINCSTP, SZ_NONE),
    E0(0xF8, CF_00, TEXT_FPREM,   SZ_NONE),
    E0(0xF9, CF_00, TEXT_FYL2XP1, SZ_NONE),
    E0(0xFA, CF_00, TEXT_FSQRT,   SZ_NONE),
    E0(0xFC, CF_00, TEXT_FRNDINT, SZ_NONE),
    E0(0xFD, CF_00, TEXT_FSCALE,  SZ_NONE),
    N1(000,  CF_00, TEXT_FLD,     SZ_DWORD, M_DMEM, P_OMOD),
    N1(020,  CF_00, TEXT_FST,     SZ_DWORD, M_DMEM, P_OMOD),
    N1(030,  CF_00, TEXT_FSTP,    SZ_DWORD, M_DMEM, P_OMOD),
    E1(040,  CF_00, TEXT_FLDENV,  SZ_NONE,  M_MEM,  P_OMOD),
    E1(050,  CF_00, TEXT_FLDCW,   SZ_WORD,  M_WMEM, P_OMOD),
    E1(060,  CF_00, TEXT_FNSTENV, SZ_NONE,  M_MEM,  P_OMOD),
    E1(070,  CF_00, TEXT_FNSTCW,  SZ_WORD,  M_WMEM, P_OMOD),
};

constexpr uint8_t I8087_D9[] PROGMEM = {
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
     32,  // TEXT_FNSTCW
     31,  // TEXT_FNSTENV
     17,  // TEXT_FPATAN
     21,  // TEXT_FPREM
     16,  // TEXT_FPTAN
     24,  // TEXT_FRNDINT
     25,  // TEXT_FSCALE
     23,  // TEXT_FSQRT
     27,  // TEXT_FST
     28,  // TEXT_FSTP
      5,  // TEXT_FTST
      6,  // TEXT_FXAM
      1,  // TEXT_FXCH
     18,  // TEXT_FXTRACT
     15,  // TEXT_FYL2X
     22,  // TEXT_FYL2XP1
};

constexpr Entry T8087_DA[] PROGMEM = {
    N1(000,  CF_00, TEXT_FIADD,  SZ_DWORD, M_DMEM, P_OMOD),
    N1(010,  CF_00, TEXT_FIMUL,  SZ_DWORD, M_DMEM, P_OMOD),
    N1(020,  CF_00, TEXT_FICOM,  SZ_DWORD, M_DMEM, P_OMOD),
    N1(030,  CF_00, TEXT_FICOMP, SZ_DWORD, M_DMEM, P_OMOD),
    N1(040,  CF_00, TEXT_FISUB,  SZ_DWORD, M_DMEM, P_OMOD),
    N1(050,  CF_00, TEXT_FISUBR, SZ_DWORD, M_DMEM, P_OMOD),
    N1(060,  CF_00, TEXT_FIDIV,  SZ_DWORD, M_DMEM, P_OMOD),
    N1(070,  CF_00, TEXT_FIDIVR, SZ_DWORD, M_DMEM, P_OMOD),
};

constexpr uint8_t I8087_DA[] PROGMEM = {
      0,  // TEXT_FIADD
      2,  // TEXT_FICOM
      3,  // TEXT_FICOMP
      6,  // TEXT_FIDIV
      7,  // TEXT_FIDIVR
      1,  // TEXT_FIMUL
      4,  // TEXT_FISUB
      5,  // TEXT_FISUBR
};

constexpr Entry T8087_DB[] PROGMEM = {
    E0(0xE0, CF_00, TEXT_FNENI,  SZ_NONE),
    E0(0xE1, CF_00, TEXT_FNDISI, SZ_NONE),
    E0(0xE2, CF_00, TEXT_FNCLEX, SZ_NONE),
    E0(0xE3, CF_00, TEXT_FNINIT, SZ_NONE),
    N1(000,  CF_00, TEXT_FILD,   SZ_DWORD, M_DMEM, P_OMOD),
    N1(020,  CF_00, TEXT_FIST,   SZ_DWORD, M_DMEM, P_OMOD),
    N1(030,  CF_00, TEXT_FISTP,  SZ_DWORD, M_DMEM, P_OMOD),
    N1(050,  CF_00, TEXT_FLD,    SZ_TBYTE, M_FMEM, P_OMOD),
    N1(070,  CF_00, TEXT_FSTP,   SZ_TBYTE, M_FMEM, P_OMOD),
};

constexpr uint8_t I8087_DB[] PROGMEM = {
      4,  // TEXT_FILD
      5,  // TEXT_FIST
      6,  // TEXT_FISTP
      7,  // TEXT_FLD
      2,  // TEXT_FNCLEX
      1,  // TEXT_FNDISI
      0,  // TEXT_FNENI
      3,  // TEXT_FNINIT
      8,  // TEXT_FSTP
};

constexpr Entry T8087_DC[] PROGMEM = {
    E2(0xC0, CF_07, TEXT_FADD,  SZ_NONE,  M_STI,  M_ST0,  P_OREG, P_NONE),
    E2(0xC8, CF_07, TEXT_FMUL,  SZ_NONE,  M_STI,  M_ST0,  P_OREG, P_NONE),
    E2(0xE8, CF_07, TEXT_FSUB,  SZ_NONE,  M_STI,  M_ST0,  P_OREG, P_NONE),
    E2(0xE0, CF_07, TEXT_FSUBR, SZ_NONE,  M_STI,  M_ST0,  P_OREG, P_NONE),
    E2(0xF8, CF_07, TEXT_FDIV,  SZ_NONE,  M_STI,  M_ST0,  P_OREG, P_NONE),
    E2(0xF0, CF_07, TEXT_FDIVR, SZ_NONE,  M_STI,  M_ST0,  P_OREG, P_NONE),
    N1(000,  CF_00, TEXT_FADD,  SZ_QWORD, M_FMEM, P_OMOD),
    N1(010,  CF_00, TEXT_FMUL,  SZ_QWORD, M_FMEM, P_OMOD),
    N1(020,  CF_00, TEXT_FCOM,  SZ_QWORD, M_FMEM, P_OMOD),
    N1(030,  CF_00, TEXT_FCOMP, SZ_QWORD, M_FMEM, P_OMOD),
    N1(040,  CF_00, TEXT_FSUB,  SZ_QWORD, M_FMEM, P_OMOD),
    N1(050,  CF_00, TEXT_FSUBR, SZ_QWORD, M_FMEM, P_OMOD),
    N1(060,  CF_00, TEXT_FDIV,  SZ_QWORD, M_FMEM, P_OMOD),
    N1(070,  CF_00, TEXT_FDIVR, SZ_QWORD, M_FMEM, P_OMOD),
};

constexpr uint8_t I8087_DC[] PROGMEM = {
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

constexpr Entry T8087_DD[] PROGMEM = {
    E1(0xC0, CF_07, TEXT_FFREE,  SZ_NONE,  M_STI,  P_OREG),
    E1(0xD0, CF_07, TEXT_FST,    SZ_NONE,  M_STI,  P_OREG),
    E1(0xD8, CF_07, TEXT_FSTP,   SZ_NONE,  M_STI,  P_OREG),
    N1(000,  CF_00, TEXT_FLD,    SZ_QWORD, M_FMEM, P_OMOD),
    N1(020,  CF_00, TEXT_FST,    SZ_QWORD, M_FMEM, P_OMOD),
    N1(030,  CF_00, TEXT_FSTP,   SZ_QWORD, M_FMEM, P_OMOD),
    E1(040,  CF_00, TEXT_FRSTOR, SZ_NONE,  M_MEM,  P_OMOD),
    E1(060,  CF_00, TEXT_FNSAVE, SZ_NONE,  M_MEM,  P_OMOD),
    E1(070,  CF_00, TEXT_FNSTSW, SZ_WORD,  M_WMEM, P_OMOD),
};

constexpr uint8_t I8087_DD[] PROGMEM = {
      0,  // TEXT_FFREE
      3,  // TEXT_FLD
      7,  // TEXT_FNSAVE
      8,  // TEXT_FNSTSW
      6,  // TEXT_FRSTOR
      1,  // TEXT_FST
      4,  // TEXT_FST
      2,  // TEXT_FSTP
      5,  // TEXT_FSTP
};

constexpr Entry T8087_DE[] PROGMEM = {
    E2(0xC0, CF_07, TEXT_FADDP,  SZ_NONE, M_STI,  M_ST0, P_OREG, P_NONE),
    E2(0xC8, CF_07, TEXT_FMULP,  SZ_NONE, M_STI,  M_ST0, P_OREG, P_NONE),
    E0(0xD9, CF_00, TEXT_FCOMPP, SZ_NONE),
    E2(0xE0, CF_07, TEXT_FSUBRP, SZ_NONE, M_STI,  M_ST0, P_OREG, P_NONE),
    E2(0xE8, CF_07, TEXT_FSUBP,  SZ_NONE, M_STI,  M_ST0, P_OREG, P_NONE),
    E2(0xF0, CF_07, TEXT_FDIVRP, SZ_NONE, M_STI,  M_ST0, P_OREG, P_NONE),
    E2(0xF8, CF_07, TEXT_FDIVP,  SZ_NONE, M_STI,  M_ST0, P_OREG, P_NONE),
    N1(000,  CF_00, TEXT_FIADD,  SZ_WORD, M_WMEM, P_OMOD),
    N1(010,  CF_00, TEXT_FIMUL,  SZ_WORD, M_WMEM, P_OMOD),
    N1(020,  CF_00, TEXT_FICOM,  SZ_WORD, M_WMEM, P_OMOD),
    N1(030,  CF_00, TEXT_FICOMP, SZ_WORD, M_WMEM, P_OMOD),
    N1(040,  CF_00, TEXT_FISUB,  SZ_WORD, M_WMEM, P_OMOD),
    N1(050,  CF_00, TEXT_FISUBR, SZ_WORD, M_WMEM, P_OMOD),
    N1(060,  CF_00, TEXT_FIDIV,  SZ_WORD, M_WMEM, P_OMOD),
    N1(070,  CF_00, TEXT_FIDIVR, SZ_WORD, M_WMEM, P_OMOD),
};

constexpr uint8_t I8087_DE[] PROGMEM = {
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

constexpr Entry T8087_DF[] PROGMEM = {
    N1(000,  CF_00, TEXT_FILD,  SZ_WORD,  M_WMEM, P_OMOD),
    N1(020,  CF_00, TEXT_FIST,  SZ_WORD,  M_WMEM, P_OMOD),
    N1(030,  CF_00, TEXT_FISTP, SZ_WORD,  M_WMEM, P_OMOD),
    N1(040,  CF_00, TEXT_FBLD,  SZ_TBYTE, M_FMEM, P_OMOD),
    N1(050,  CF_00, TEXT_FILD,  SZ_QWORD, M_FMEM, P_OMOD),
    N1(060,  CF_00, TEXT_FBSTP, SZ_TBYTE, M_FMEM, P_OMOD),
    N1(070,  CF_00, TEXT_FISTP, SZ_QWORD, M_FMEM, P_OMOD),
};

constexpr uint8_t I8087_DF[] PROGMEM = {
      3,  // TEXT_FBLD
      5,  // TEXT_FBSTP
      0,  // TEXT_FILD
      4,  // TEXT_FILD
      1,  // TEXT_FIST
      2,  // TEXT_FISTP
      6,  // TEXT_FISTP
};

constexpr Entry T8087_9BD9[] PROGMEM = {
    E1(060,  CF_00, TEXT_FSTENV, SZ_NONE, M_MEM,  P_OMOD),
    E1(070,  CF_00, TEXT_FSTCW,  SZ_WORD, M_WMEM, P_OMOD),
};

constexpr uint8_t I8087_9BD9[] PROGMEM = {
      1,  // TEXT_FSTCW
      0,  // TEXT_FSTENV
};

constexpr Entry T8087_9BDB[] PROGMEM = {
    E0(0xE0, CF_00, TEXT_FENI,  SZ_NONE),
    E0(0xE1, CF_00, TEXT_FDISI, SZ_NONE),
    E0(0xE2, CF_00, TEXT_FCLEX, SZ_NONE),
    E0(0xE3, CF_00, TEXT_FINIT, SZ_NONE),
};

constexpr uint8_t I8087_9BDB[] PROGMEM = {
      2,  // TEXT_FCLEX
      1,  // TEXT_FDISI
      0,  // TEXT_FENI
      3,  // TEXT_FINIT
};

constexpr Entry T8087_9BDD[] PROGMEM = {
    E1(060,  CF_00, TEXT_FSAVE, SZ_NONE, M_MEM,  P_OMOD),
    E1(070,  CF_00, TEXT_FSTSW, SZ_WORD, M_WMEM, P_OMOD),
};

constexpr uint8_t I8087_9BDD[] PROGMEM = {
      0,  // TEXT_FSAVE
      1,  // TEXT_FSTSW
};

constexpr Entry T80287_DB[] PROGMEM = {
    E0(0xE4, CF_00, TEXT_FNSETPM, SZ_NONE),
};

constexpr uint8_t I80287_DB[] PROGMEM = {
      0,  // TEXT_FNSETPM
};

constexpr Entry T80287_DF[] PROGMEM = {
    E1(0xE0, CF_00, TEXT_FNSTSW, SZ_NONE,  M_AX, P_NONE),
};

constexpr uint8_t I80287_DF[] PROGMEM = {
      0,  // TEXT_FNSTSW
};

constexpr Entry T80387_D9[] PROGMEM = {
    E0(0xF5, CF_00, TEXT_FPREM1,  SZ_NONE),
    E0(0xFB, CF_00, TEXT_FSINCOS, SZ_NONE),
    E0(0xFE, CF_00, TEXT_FSIN,    SZ_NONE),
    E0(0xFF, CF_00, TEXT_FCOS,    SZ_NONE),
};

constexpr uint8_t I80387_D9[] PROGMEM = {
      3,  // TEXT_FCOS
      0,  // TEXT_FPREM1
      2,  // TEXT_FSIN
      1,  // TEXT_FSINCOS
};

constexpr Entry T80387_DA[] PROGMEM = {
    E0(0xE9, CF_00, TEXT_FUCOMPP, SZ_NONE),
};

constexpr uint8_t I80387_DA[] PROGMEM = {
      0,  // TEXT_FUCOMPP
};

constexpr Entry T80387_DD[] PROGMEM = {
    E1(0xE0, CF_07, TEXT_FUCOM,  SZ_NONE, M_STI, P_OREG),
    E1(0xE8, CF_07, TEXT_FUCOMP, SZ_NONE, M_STI, P_OREG),
};

constexpr uint8_t I80387_DD[] PROGMEM = {
      0,  // TEXT_FUCOM
      1,  // TEXT_FUCOMP
};
#endif

// clang-format on

using EntryPage = entry::PrefixTableBase<Entry>;

constexpr EntryPage I8086_PAGES[] PROGMEM = {
        {0x83, ARRAY_RANGE(T8086_83), ARRAY_RANGE(I8086_8X)},  // M_BIMM
        {0x00, ARRAY_RANGE(T8086_00), ARRAY_RANGE(I8086_00)},
        {0x80, ARRAY_RANGE(T8086_80), ARRAY_RANGE(I8086_8X)},
        {0x81, ARRAY_RANGE(T8086_81), ARRAY_RANGE(I8086_8X)},  // M_WIMM
        {0x8F, ARRAY_RANGE(T8086_8F), ARRAY_RANGE(I8086_8F)},
        {0xC6, ARRAY_RANGE(T8086_C6), ARRAY_RANGE(I8086_C6)},
        {0xC7, ARRAY_RANGE(T8086_C7), ARRAY_RANGE(I8086_C7)},
        {0xD0, ARRAY_RANGE(T8086_D0), ARRAY_RANGE(I8086_DX)},
        {0xD1, ARRAY_RANGE(T8086_D1), ARRAY_RANGE(I8086_DX)},
        {0xD2, ARRAY_RANGE(T8086_D2), ARRAY_RANGE(I8086_DX)},
        {0xD3, ARRAY_RANGE(T8086_D3), ARRAY_RANGE(I8086_DX)},
        {0xD4, ARRAY_RANGE(T8086_D4), ARRAY_RANGE(I8086_D4)},
        {0xD5, ARRAY_RANGE(T8086_D5), ARRAY_RANGE(I8086_D5)},
        {0xF6, ARRAY_RANGE(T8086_F6), ARRAY_RANGE(I8086_FX)},
        {0xF7, ARRAY_RANGE(T8086_F7), ARRAY_RANGE(I8086_FX)},
        {0xFE, ARRAY_RANGE(T8086_FE), ARRAY_RANGE(I8086_FE)},
        {0xFF, ARRAY_RANGE(T8086_FF), ARRAY_RANGE(I8086_FF)},
};

constexpr EntryPage I80186_PAGES[] PROGMEM = {
        // i80186
        {0x00, ARRAY_RANGE(T80186_00), ARRAY_RANGE(I80186_00)},
        {0xD0, ARRAY_RANGE(T8086_D0), ARRAY_RANGE(I8086_DX)},    // M_VAL1
        {0xD1, ARRAY_RANGE(T8086_D1), ARRAY_RANGE(I8086_DX)},    // M_VAL1
        {0xC0, ARRAY_RANGE(T80186_C0), ARRAY_RANGE(I80186_CX)},  // M_BIT
        {0xC1, ARRAY_RANGE(T80186_C1), ARRAY_RANGE(I80186_CX)},  // M_BIT
        // i8086
        {0x83, ARRAY_RANGE(T8086_83), ARRAY_RANGE(I8086_8X)},  // M_BIMM
        {0x00, ARRAY_RANGE(T8086_00), ARRAY_RANGE(I8086_00)},
        {0x80, ARRAY_RANGE(T8086_80), ARRAY_RANGE(I8086_8X)},
        {0x81, ARRAY_RANGE(T8086_81), ARRAY_RANGE(I8086_8X)},  // M_WIMM
        {0x8F, ARRAY_RANGE(T8086_8F), ARRAY_RANGE(I8086_8F)},
        {0xC6, ARRAY_RANGE(T8086_C6), ARRAY_RANGE(I8086_C6)},
        {0xC7, ARRAY_RANGE(T8086_C7), ARRAY_RANGE(I8086_C7)},
        {0xD2, ARRAY_RANGE(T8086_D2), ARRAY_RANGE(I8086_DX)},
        {0xD3, ARRAY_RANGE(T8086_D3), ARRAY_RANGE(I8086_DX)},
        {0xD4, ARRAY_RANGE(T8086_D4), ARRAY_RANGE(I8086_D4)},
        {0xD5, ARRAY_RANGE(T8086_D5), ARRAY_RANGE(I8086_D5)},
        {0xF6, ARRAY_RANGE(T8086_F6), ARRAY_RANGE(I8086_FX)},
        {0xF7, ARRAY_RANGE(T8086_F7), ARRAY_RANGE(I8086_FX)},
        {0xFE, ARRAY_RANGE(T8086_FE), ARRAY_RANGE(I8086_FE)},
        {0xFF, ARRAY_RANGE(T8086_FF), ARRAY_RANGE(I8086_FF)},
};

constexpr EntryPage I80286_PAGES[] PROGMEM = {
        // i80186
        {0x00, ARRAY_RANGE(T80186_00), ARRAY_RANGE(I80186_00)},
        {0xD0, ARRAY_RANGE(T8086_D0), ARRAY_RANGE(I8086_DX)},    // M_VAL1
        {0xD1, ARRAY_RANGE(T8086_D1), ARRAY_RANGE(I8086_DX)},    // M_VAL1
        {0xC0, ARRAY_RANGE(T80186_C0), ARRAY_RANGE(I80186_CX)},  // M_BIT
        {0xC1, ARRAY_RANGE(T80186_C1), ARRAY_RANGE(I80186_CX)},  // M_BIT
        // i80286
        {0x00, ARRAY_RANGE(T80286_00), ARRAY_RANGE(I80286_00)},
        {0x0F, ARRAY_RANGE(T80286_0F), ARRAY_RANGE(I80286_0F)},
        {0x0F00, ARRAY_RANGE(T80286_0F00), ARRAY_RANGE(I80286_0F00)},
        {0x0F01, ARRAY_RANGE(T80286_0F01), ARRAY_RANGE(I80286_0F01)},
        // i8086
        {0x83, ARRAY_RANGE(T8086_83), ARRAY_RANGE(I8086_8X)},  // M_BIMM
        {0x00, ARRAY_RANGE(T8086_00), ARRAY_RANGE(I8086_00)},
        {0x80, ARRAY_RANGE(T8086_80), ARRAY_RANGE(I8086_8X)},
        {0x81, ARRAY_RANGE(T8086_81), ARRAY_RANGE(I8086_8X)},  // M_WIMM
        {0x8F, ARRAY_RANGE(T8086_8F), ARRAY_RANGE(I8086_8F)},
        {0xC6, ARRAY_RANGE(T8086_C6), ARRAY_RANGE(I8086_C6)},
        {0xC7, ARRAY_RANGE(T8086_C7), ARRAY_RANGE(I8086_C7)},
        {0xD2, ARRAY_RANGE(T8086_D2), ARRAY_RANGE(I8086_DX)},
        {0xD3, ARRAY_RANGE(T8086_D3), ARRAY_RANGE(I8086_DX)},
        {0xD4, ARRAY_RANGE(T8086_D4), ARRAY_RANGE(I8086_D4)},
        {0xD5, ARRAY_RANGE(T8086_D5), ARRAY_RANGE(I8086_D5)},
        {0xF6, ARRAY_RANGE(T8086_F6), ARRAY_RANGE(I8086_FX)},
        {0xF7, ARRAY_RANGE(T8086_F7), ARRAY_RANGE(I8086_FX)},
        {0xFE, ARRAY_RANGE(T8086_FE), ARRAY_RANGE(I8086_FE)},
        {0xFF, ARRAY_RANGE(T8086_FF), ARRAY_RANGE(I8086_FF)},
};

constexpr EntryPage I80386_PAGES[] PROGMEM = {
        // i80386
        {0x00, ARRAY_RANGE(TCODE16_00), ARRAY_RANGE(ICODE16_00)},
        // i80186
        {0x00, ARRAY_RANGE(T80186_00), ARRAY_RANGE(I80186_00)},
        {0xD0, ARRAY_RANGE(T8086_D0), ARRAY_RANGE(I8086_DX)},    // M_VAL1
        {0xD1, ARRAY_RANGE(T8086_D1), ARRAY_RANGE(I8086_DX)},    // M_VAL1
        {0xC0, ARRAY_RANGE(T80186_C0), ARRAY_RANGE(I80186_CX)},  // M_BIT
        {0xC1, ARRAY_RANGE(T80186_C1), ARRAY_RANGE(I80186_CX)},  // M_BIT
        // i80286
        {0x00, ARRAY_RANGE(T80286_00), ARRAY_RANGE(I80286_00)},
        {0x0F, ARRAY_RANGE(T80286_0F), ARRAY_RANGE(I80286_0F)},
        {0x0F00, ARRAY_RANGE(T80286_0F00), ARRAY_RANGE(I80286_0F00)},
        {0x0F01, ARRAY_RANGE(T80286_0F01), ARRAY_RANGE(I80286_0F01)},
        // i8086
        {0x83, ARRAY_RANGE(T8086_83), ARRAY_RANGE(I8086_8X)},  // M_BIMM
        {0x00, ARRAY_RANGE(T8086_00), ARRAY_RANGE(I8086_00)},
        {0x80, ARRAY_RANGE(T8086_80), ARRAY_RANGE(I8086_8X)},
        {0x81, ARRAY_RANGE(T8086_81), ARRAY_RANGE(I8086_8X)},   // M_WIMM
        {0x8F, ARRAY_RANGE(T80386_8F), ARRAY_RANGE(I8086_8F)},  // i80386
        {0xC6, ARRAY_RANGE(T8086_C6), ARRAY_RANGE(I8086_C6)},
        {0xC7, ARRAY_RANGE(T8086_C7), ARRAY_RANGE(I8086_C7)},
        {0xD2, ARRAY_RANGE(T8086_D2), ARRAY_RANGE(I8086_DX)},
        {0xD3, ARRAY_RANGE(T8086_D3), ARRAY_RANGE(I8086_DX)},
        {0xD4, ARRAY_RANGE(T8086_D4), ARRAY_RANGE(I8086_D4)},
        {0xD5, ARRAY_RANGE(T8086_D5), ARRAY_RANGE(I8086_D5)},
        {0xF6, ARRAY_RANGE(T8086_F6), ARRAY_RANGE(I8086_FX)},
        {0xF7, ARRAY_RANGE(T8086_F7), ARRAY_RANGE(I8086_FX)},
        {0xFE, ARRAY_RANGE(T8086_FE), ARRAY_RANGE(I8086_FE)},
        {0xFF, ARRAY_RANGE(T80386_FF), ARRAY_RANGE(I8086_FF)},  // i80386
        // i80386
        {0x00, ARRAY_RANGE(T80386_00), ARRAY_RANGE(I80386_00)},
        {0x0F, ARRAY_RANGE(T80386_0F), ARRAY_RANGE(I80386_0F)},
        {0x0FBA, ARRAY_RANGE(T80386_0FBA), ARRAY_RANGE(I80386_0FBA)},
};

constexpr EntryPage V30_PAGES[] PROGMEM = {
        // V30
        {0x00, ARRAY_RANGE(TV30_00), ARRAY_RANGE(IV30_00)},
        {0x0F, ARRAY_RANGE(TV30_0F), ARRAY_RANGE(IV30_0F)},
        // I80186
        {0x00, ARRAY_RANGE(T80186_00), ARRAY_RANGE(I80186_00)},
        {0xD0, ARRAY_RANGE(T8086_D0), ARRAY_RANGE(I8086_DX)},    // M_VAL1
        {0xD1, ARRAY_RANGE(T8086_D1), ARRAY_RANGE(I8086_DX)},    // M_VAL1
        {0xC0, ARRAY_RANGE(T80186_C0), ARRAY_RANGE(I80186_CX)},  // M_BIT
        {0xC1, ARRAY_RANGE(T80186_C1), ARRAY_RANGE(I80186_CX)},  // M_BIT
        // i8086
        {0x83, ARRAY_RANGE(T8086_83), ARRAY_RANGE(I8086_8X)},  // M_BIMM
        {0x00, ARRAY_RANGE(T8086_00), ARRAY_RANGE(I8086_00)},
        {0x80, ARRAY_RANGE(T8086_80), ARRAY_RANGE(I8086_8X)},
        {0x81, ARRAY_RANGE(T8086_81), ARRAY_RANGE(I8086_8X)},  // M_WIMM
        {0x8F, ARRAY_RANGE(T8086_8F), ARRAY_RANGE(I8086_8F)},
        {0xC6, ARRAY_RANGE(T8086_C6), ARRAY_RANGE(I8086_C6)},
        {0xC7, ARRAY_RANGE(T8086_C7), ARRAY_RANGE(I8086_C7)},
        {0xD2, ARRAY_RANGE(T8086_D2), ARRAY_RANGE(I8086_DX)},
        {0xD3, ARRAY_RANGE(T8086_D3), ARRAY_RANGE(I8086_DX)},
        {0xD4, ARRAY_RANGE(T8086_D4), ARRAY_RANGE(I8086_D4)},
        {0xD5, ARRAY_RANGE(T8086_D5), ARRAY_RANGE(I8086_D5)},
        {0xF6, ARRAY_RANGE(T8086_F6), ARRAY_RANGE(I8086_FX)},
        {0xF7, ARRAY_RANGE(T8086_F7), ARRAY_RANGE(I8086_FX)},
        {0xFE, ARRAY_RANGE(T8086_FE), ARRAY_RANGE(I8086_FE)},
        {0xFF, ARRAY_RANGE(T8086_FF), ARRAY_RANGE(I8086_FF)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {I8086, TEXT_CPU_8086, ARRAY_RANGE(I8086_PAGES)},
        {I80186, TEXT_CPU_80186, ARRAY_RANGE(I80186_PAGES)},
        {I80286, TEXT_CPU_80286, ARRAY_RANGE(I80286_PAGES)},
        {I80386, TEXT_CPU_80386, ARRAY_RANGE(I80386_PAGES)},
        {V30, TEXT_CPU_V30, ARRAY_RANGE(V30_PAGES)},
};

const Cpu *cpu(CpuType cpuType) {
    return Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
}

#define EMPTY_RANGE(a) ARRAY_BEGIN(a), ARRAY_BEGIN(a)

#if !defined(LIBASM_I8086_NOFPU)
constexpr EntryPage I8087_PAGES[] PROGMEM = {
        // i8087
        {0xD8, ARRAY_RANGE(T8087_D8), ARRAY_RANGE(I8087_D8)},
        {0xD9, ARRAY_RANGE(T8087_D9), ARRAY_RANGE(I8087_D9)},
        {0xDA, ARRAY_RANGE(T8087_DA), ARRAY_RANGE(I8087_DA)},
        {0xDB, ARRAY_RANGE(T8087_DB), ARRAY_RANGE(I8087_DB)},
        {0xDC, ARRAY_RANGE(T8087_DC), ARRAY_RANGE(I8087_DC)},
        {0xDD, ARRAY_RANGE(T8087_DD), ARRAY_RANGE(I8087_DD)},
        {0xDE, ARRAY_RANGE(T8087_DE), ARRAY_RANGE(I8087_DE)},
        {0xDF, ARRAY_RANGE(T8087_DF), ARRAY_RANGE(I8087_DF)},
        {0x00, ARRAY_RANGE(T8087_00), ARRAY_RANGE(I8087_00)},
        {0x9BD9, ARRAY_RANGE(T8087_9BD9), ARRAY_RANGE(I8087_9BD9)},
        {0x9BDB, ARRAY_RANGE(T8087_9BDB), ARRAY_RANGE(I8087_9BDB)},
        {0x9BDD, ARRAY_RANGE(T8087_9BDD), ARRAY_RANGE(I8087_9BDD)},
};

constexpr EntryPage I80287_PAGES[] PROGMEM = {
        // i8087
        {0xD8, ARRAY_RANGE(T8087_D8), ARRAY_RANGE(I8087_D8)},
        {0xD9, ARRAY_RANGE(T8087_D9), ARRAY_RANGE(I8087_D9)},
        {0xDA, ARRAY_RANGE(T8087_DA), ARRAY_RANGE(I8087_DA)},
        {0xDB, ARRAY_RANGE(T8087_DB), ARRAY_RANGE(I8087_DB)},
        {0xDC, ARRAY_RANGE(T8087_DC), ARRAY_RANGE(I8087_DC)},
        {0xDD, ARRAY_RANGE(T8087_DD), ARRAY_RANGE(I8087_DD)},
        {0xDE, ARRAY_RANGE(T8087_DE), ARRAY_RANGE(I8087_DE)},
        {0xDF, ARRAY_RANGE(T8087_DF), ARRAY_RANGE(I8087_DF)},
        {0x00, ARRAY_RANGE(T8087_00), ARRAY_RANGE(I8087_00)},
        {0x9BD9, ARRAY_RANGE(T8087_9BD9), ARRAY_RANGE(I8087_9BD9)},
        {0x9BDB, ARRAY_RANGE(T8087_9BDB), ARRAY_RANGE(I8087_9BDB)},
        {0x9BDD, ARRAY_RANGE(T8087_9BDD), ARRAY_RANGE(I8087_9BDD)},
        // i80287
        {0xDB, ARRAY_RANGE(T80287_DB), ARRAY_RANGE(I80287_DB)},
        {0xDF, ARRAY_RANGE(T80287_DF), ARRAY_RANGE(I80287_DF)},
};

constexpr EntryPage I80387_PAGES[] PROGMEM = {
        // i8087
        {0xD8, ARRAY_RANGE(T8087_D8), ARRAY_RANGE(I8087_D8)},
        {0xD9, ARRAY_RANGE(T8087_D9), ARRAY_RANGE(I8087_D9)},
        {0xDA, ARRAY_RANGE(T8087_DA), ARRAY_RANGE(I8087_DA)},
        {0xDB, ARRAY_RANGE(T8087_DB), ARRAY_RANGE(I8087_DB)},
        {0xDC, ARRAY_RANGE(T8087_DC), ARRAY_RANGE(I8087_DC)},
        {0xDD, ARRAY_RANGE(T8087_DD), ARRAY_RANGE(I8087_DD)},
        {0xDE, ARRAY_RANGE(T8087_DE), ARRAY_RANGE(I8087_DE)},
        {0xDF, ARRAY_RANGE(T8087_DF), ARRAY_RANGE(I8087_DF)},
        {0x00, ARRAY_RANGE(T8087_00), ARRAY_RANGE(I8087_00)},
        {0x9BD9, ARRAY_RANGE(T8087_9BD9), ARRAY_RANGE(I8087_9BD9)},
        {0x9BDB, ARRAY_RANGE(T8087_9BDB), ARRAY_RANGE(I8087_9BDB)},
        {0x9BDD, ARRAY_RANGE(T8087_9BDD), ARRAY_RANGE(I8087_9BDD)},
        // i80287
        {0xDB, ARRAY_RANGE(T80287_DB), ARRAY_RANGE(I80287_DB)},
        {0xDF, ARRAY_RANGE(T80287_DF), ARRAY_RANGE(I80287_DF)},
        // i80387
        {0xD9, ARRAY_RANGE(T80387_D9), ARRAY_RANGE(I80387_D9)},
        {0xDA, ARRAY_RANGE(T80387_DA), ARRAY_RANGE(I80387_DA)},
        {0xDD, ARRAY_RANGE(T80387_DD), ARRAY_RANGE(I80387_DD)},
};

using Fpu = entry::CpuBase<FpuType, EntryPage>;

constexpr Fpu FPU_TABLE[] PROGMEM = {
        {FPU_I8087, TEXT_FPU_8087, ARRAY_RANGE(I8087_PAGES)},
        {FPU_I80287, TEXT_FPU_80287, ARRAY_RANGE(I80287_PAGES)},
        {FPU_I80387, TEXT_FPU_80387, ARRAY_RANGE(I80387_PAGES)},
        {FPU_I80C187, TEXT_FPU_80C187, ARRAY_RANGE(I80387_PAGES)},
        {FPU_NONE, TEXT_none, EMPTY_RANGE(I8087_PAGES)},
};

const Fpu *fpu(FpuType fpuType) {
    return Fpu::search(fpuType, ARRAY_RANGE(FPU_TABLE));
}
#endif

bool acceptMode(AddrMode opr, AddrMode table) {
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
               table == M_WMEM || table == M_WDIR || table == M_MEM;
    if (opr == M_VAL1 || opr == M_VAL3)
        return table == M_WIMM || table == M_BIMM || table == M_IOA || table == M_UI16 ||
               table == M_UI8 || table == M_BIT || table == M_REL8 || table == M_REL ||
               table == M_SEG || table == M_OFF;
    if (opr == M_WIMM || opr == M_BIMM)
        return table == M_WIMM || table == M_IOA || table == M_UI16 || table == M_UI8 ||
               table == M_BIT || table == M_REL8 || table == M_REL || table == M_SEG ||
               table == M_OFF;
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

bool hasSize(AddrMode mode) {
    return mode == M_AX || mode == M_DX || mode == M_WREG || mode == M_AL || mode == M_CL ||
           mode == M_BREG || mode == M_CS || mode == M_SREG;
}

bool acceptSize(const AsmInsn &insn, const Entry *entry) {
    const auto dst = insn.dstOp.mode;
    const auto src = insn.srcOp.mode;
    const auto flags = entry->readFlags();
    if (dst == M_MEM || dst == M_DIR) {
        if (src == M_NONE)
            return !flags.needSize();
        return hasSize(src) || flags.stringInsn();
    }
    if (src == M_MEM || src == M_DIR)
        return hasSize(dst) || flags.stringInsn();
    if (dst == M_FMEM) {
        const auto ptrSize = pointerSize(insn.dstOp.ptr);
        return ptrSize == flags.size();
    }
    return true;
}

bool acceptModes(AsmInsn &insn, const Entry *entry) {
    const auto table = entry->readFlags();
    // if (strcasecmp(insn.name(), "CALL") == 0 && strcasecmp_P(entry->name_P(), "CALL") == 0) {
    //     const auto dst = acceptMode(insn.dstOp.mode, table.dst());
    //     const auto src = acceptMode(insn.srcOp.mode, table.src());
    //     const auto ext = acceptMode(insn.extOp.mode, table.ext());
    //     const auto size = acceptSize(insn, entry);
    //     printf("@@ insn: dst=%d src=%d  table: opc=%02X(%04o) dst=%d src=%d size=%d  accept: "
    //            "dst=%d "
    //            "src=%d size=%d\n",
    //             insn.dstOp.mode, insn.srcOp.mode, entry->readOpCode(), entry->readOpCode(),
    //             table.dst(), table.src(), table.size(), dst, src, size);
    //     return dst && src && ext && size;
    // }
    return acceptMode(insn.dstOp.mode, table.dst()) && acceptMode(insn.srcOp.mode, table.src()) &&
           acceptMode(insn.extOp.mode, table.ext()) && acceptSize(insn, entry);
}

Error searchName(const CpuSpec &cpuSpec, AsmInsn &insn) {
    cpu(cpuSpec.cpu)->searchName(insn, acceptModes);
#if !defined(LIBASM_I8086_NOFPU)
    if (insn.getError() == UNKNOWN_INSTRUCTION)
        fpu(cpuSpec.fpu)->searchName(insn, acceptModes);
#endif
    if (insn.getError() == UNKNOWN_INSTRUCTION)
        insn.setError(insn.name(), insn);
    return insn.getError();
}

bool isSegmentPrefix(const CpuSpec &cpuSpec, Config::opcode_t opCode) {
    return overrideSeg(cpuSpec, opCode) != REG_UNDEF;
}

bool isRepeatPrefix(const CpuSpec &cpuSpec, Config::opcode_t opCode) {
    if (opCode == 0xF2 || opCode == 0xF3)
        return true;
    return cpuSpec.cpu == V30 && (opCode == 0x64 || opCode == 0x65);
}

RegName overrideSeg(const CpuSpec &cpuSpec, Config::opcode_t opCode, RegName defSeg) {
    switch (opCode) {
    case 0x26:
        return REG_ES;
    case 0x2E:
        return REG_CS;
    case 0x36:
        return REG_SS;
    case 0x3E:
        return REG_DS;
    case 0x64:
        return cpuSpec.hasExtraSeg() ? REG_FS : defSeg;
    case 0x65:
        return cpuSpec.hasExtraSeg() ? REG_GS : defSeg;
    default:
        return defSeg;
    }
}

Config::opcode_t segOverridePrefix(const CpuSpec &cpuSpec, RegName name) {
    switch (name) {
    case REG_ES:
        return 0x26;
    case REG_CS:
        return 0x2E;
    case REG_SS:
        return 0x36;
    case REG_DS:
        return 0x3E;
    case REG_FS:
        return cpuSpec.hasExtraSeg() ? 0x64 : 0;
    case REG_GS:
        return cpuSpec.hasExtraSeg() ? 0x65 : 0;
    default:
        return 0;
    }
}

bool EntryInsn::repeatInsn(Config::opcode_t opc, CpuType cpuType) {
    switch (opc) {
    case 0xF2:
    case 0xF3:
        return true;
    case 0x64:
    case 0x65:
        return cpuType == V30;
    default:
        return false;
    }
}

bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *page) {
    auto opc = insn.opCode();
    const auto flags = entry->readFlags();
    auto mask = flags.mask();
    if (flags.dstPos() == P_OMOD || flags.srcPos() == P_OMOD) {
        const auto prefix = page->readPrefix();
        // non-FPU or non-register modReg instruction needs to be
        // masked out mode and reg field of |opc|.
        const auto mod = (opc >> 6);
        if (!DisInsn::escapeInsn(prefix) || mod != 3)
            mask |= 0307;
    }
    opc &= ~mask;
    return opc == entry->readOpCode();
}

bool isData16(const Entry *entry) {
    return entry >= TCODE16_00 && entry < ARRAY_END(TCODE16_00);
}

const Entry *toData32(const Entry *entry) {
    return (entry - TCODE16_00) + TCODE32_00;
}

Error searchOpCode(const CpuSpec &cpuSpec, DisInsn &insn, StrBuffer &out) {
    auto entry = cpu(cpuSpec.cpu)->searchOpCode(insn, out, matchOpCode);
    if (isData16(entry) && insn.useData32()) {
        entry = toData32(entry);
        Cpu::defaultReadName(insn, entry, out);
    }
    if (insn.farInsn() && insn.farMode() != FMODE_FAR) {
        entry += (insn.farMode() == FMODE_F ? 1 : 2);
        Cpu::defaultReadName(insn, entry, out);
    }
#if !defined(LIBASM_I8086_NOFPU)
    if (insn.getError() == UNKNOWN_INSTRUCTION)
        fpu(cpuSpec.fpu)->searchOpCode(insn, out, matchOpCode);
#endif
    return insn.getError();
}

bool isPrefix(const CpuSpec &cpuSpec, uint16_t code) {
    return cpu(cpuSpec.cpu)->isPrefix(code)
#if !defined(LIBASM_I8086_NOFPU)
           || fpu(cpuSpec.fpu)->isPrefix(code);
#endif
    ;
}

const /*PROGMEM*/ char *TableI8086::listCpu_P() const {
    return TEXT_I8086_LIST;
}

const /*PROGMEM*/ char *TableI8086::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableI8086::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    auto t = Cpu::search(name, ARRAY_RANGE(CPU_TABLE));
    if (t == nullptr && name.iexpect('i'))
        t = Cpu::search(name, ARRAY_RANGE(CPU_TABLE));
    if (t == nullptr)
        return UNSUPPORTED_CPU;
    cpuType = t->readCpuType();
    return OK;
}

void Config::setCpuType(CpuType cpuType) {
    _cpuSpec.cpu = cpuType;
    ConfigImpl::setCpuType(cpuType);
    setFpuType(_cpuSpec.fpu == FPU_NONE ? FPU_NONE : FPU_ON);
}

const char *Config::fpu_P() const {
    return fpu(_cpuSpec.fpu)->name_P();
}

Error Config::setFpuType(FpuType fpuType) {
    if (fpuType == FPU_NONE) {
        _cpuSpec.fpu = FPU_NONE;
        return OK;
    }
#if defined(LIBASM_I8086_NOFPU)
    return FLOAT_NOT_SUPPORTED;
#else
    const auto cpuType = _cpuSpec.cpu;
    if (fpuType == FPU_ON) {
        if (cpuType == I8086 || cpuType == I80186) {
            _cpuSpec.fpu = FPU_I8087;
            return OK;
        } else if (cpuType == I80286) {
            _cpuSpec.fpu = FPU_I80287;
            return OK;
        } else if (cpuType == I80386) {
            _cpuSpec.fpu = FPU_I80387;
            return OK;
        }
    } else if (fpuType == FPU_I8087) {
        if (cpuType == I8086 || cpuType == I80186) {
            _cpuSpec.fpu = FPU_I8087;
            return OK;
        }
    } else if (fpuType == FPU_I80287) {
        if (cpuType == I80286) {
            _cpuSpec.fpu = FPU_I80287;
            return OK;
        }
    } else if (fpuType == FPU_I80387) {
        if (cpuType == I80386) {
            _cpuSpec.fpu = FPU_I80387;
            return OK;
        }
    } else if (fpuType == FPU_I80C187) {
        if (cpuType == I80186) {
            _cpuSpec.fpu = FPU_I80C187;
            return OK;
        }
    }
    _cpuSpec.fpu = FPU_NONE;
    return FLOAT_NOT_SUPPORTED;
#endif
}

Error Config::setFpuName(StrScanner &scan) {
    if (scan.expectFalse())
        return setFpuType(FPU_NONE);
#if !defined(LIBASM_I8086_NOFPU)
    if (scan.expectTrue())
        return setFpuType(FPU_ON);
    auto p = scan;
    p.iexpect('i');
    if (p.iequals_P(TEXT_FPU_8087))
        return setFpuType(FPU_I8087);
    if (p.iequals_P(TEXT_FPU_80287))
        return setFpuType(FPU_I80287);
    if (p.iequals_P(TEXT_FPU_80387))
        return setFpuType(FPU_I80387);
    if (p.iequals_P(TEXT_FPU_80C187))
        return setFpuType(FPU_I80C187);
#endif
    return UNKNOWN_OPERAND;
}

static constexpr auto LEA = 0x8D;      // LEA
static constexpr auto CALLF = 0x9A;    // CALL far
static constexpr auto JMPF = 0xEA;     // JMPF far
static constexpr auto RETFu16 = 0xCA;  // RETF u16
static constexpr auto RETF = 0xCB;     // RETF
static constexpr auto PUSHiw = 0x68;   // PUSH iw

bool EntryInsn::leaInsn() const {
    return prefix() == 0 && opCode() == LEA;
}

bool EntryInsn::farInsn() const {
    const auto pre = prefix();
    const auto opc = opCode();
    if (pre == 0)
        return opc == CALLF || opc == JMPF || opc == RETFu16 || opc == RETF;
    if (pre == 0xFF) {
        const auto reg = (opc >> 3) & 7;
        return reg == 3 || reg == 5;  // CALLF/JMPF
    }
    return false;
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

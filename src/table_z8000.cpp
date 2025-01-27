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

#include "table_z8000.h"
#include "entry_table.h"
#include "entry_z8000.h"
#include "text_z8000.h"

using namespace libasm::text::z8000;

namespace libasm {
namespace z8000 {

#define X4(_opc, _name, _cf, _sz, _dst, _src, _dstf, _srcf, _ex1, _ex2, _postFormat) \
    {_opc, Entry::Flags::create(_dst, _dstf, _src, _srcf, _ex1, _ex2, _postFormat, _cf, _sz), _name}
#define X3(_opc, _name, _cf, _sz, _dst, _src, _dstf, _srcf, _ex1, _postFormat) \
    X4(_opc, _name, _cf, _sz, _dst, _src, _dstf, _srcf, _ex1, E2_NONE, _postFormat)
#define X2(_opc, _name, _cf, _sz, _dst, _src, _dstf, _srcf, _postFormat) \
    X3(_opc, _name, _cf, _sz, _dst, _src, _dstf, _srcf, E1_NONE, _postFormat)
#define E2(_opc, _name, _cf, _sz, _dst, _src, _dstf, _srcf) \
    X2(_opc, _name, _cf, _sz, _dst, _src, _dstf, _srcf, PF_NONE)
#define E1(_opc, _name, _cf, _sz, _dst, _dstf) E2(_opc, _name, _cf, _sz, _dst, M_NONE, _dstf, OP_NO)
#define E0(_opc, _name, _cf) E1(_opc, _name, _cf, SZ_NONE, M_NONE, OP_NO)

// clang-format off
constexpr Entry TABLE_Z8000[] PROGMEM = {
    E2(0xC000, TEXT_LDB,    CF_0FFF, SZ_BYTE, M_R,    M_IM8,  OP_C8, OP_C0),
    E1(0xD000, TEXT_CALR,   CF_0FFF, SZ_NONE, M_RA12,         OP_C0),
    E2(0xE000, TEXT_JR,     CF_0FFF, SZ_NONE, M_CC,   M_RA8,  OP_C8, OP_C0),
    E1(0xE800, TEXT_JR,     CF_00FF, SZ_NONE, M_RA8,          OP_C0),
    E2(0xF000, TEXT_DBJNZ,  CF_0F7F, SZ_BYTE, M_R,    M_RA7,  OP_C8, OP_C0),
    E2(0xF080, TEXT_DJNZ,   CF_0F7F, SZ_WORD, M_R,    M_RA7,  OP_C8, OP_C0),
    E2(0x3000, TEXT_LDRB,   CF_000F, SZ_BYTE, M_R,    M_RA,   OP_C0, OP_NO),
    E2(0x3000, TEXT_LDB,    CF_00FF, SZ_BYTE, M_R,    M_BA,   OP_C0, OP_C4),
    E2(0x3100, TEXT_LDR,    CF_000F, SZ_WORD, M_R,    M_RA,   OP_C0, OP_NO),
    E2(0x3100, TEXT_LD,     CF_00FF, SZ_WORD, M_R,    M_BA,   OP_C0, OP_C4),
    E2(0x3200, TEXT_LDRB,   CF_000F, SZ_BYTE, M_RA,   M_R,    OP_NO, OP_C0),
    E2(0x3200, TEXT_LDB,    CF_00FF, SZ_BYTE, M_BA,   M_R,    OP_C4, OP_C0),
    E2(0x3300, TEXT_LDR,    CF_000F, SZ_WORD, M_RA,   M_R,    OP_NO, OP_C0),
    E2(0x3300, TEXT_LD,     CF_00FF, SZ_WORD, M_BA,   M_R,    OP_C4, OP_C0),
    E2(0x3400, TEXT_LDAR,   CF_000F, SZ_ADDR, M_R,    M_RA,   OP_C0, OP_NO),
    E2(0x3400, TEXT_LDA,    CF_00FF, SZ_ADDR, M_R,    M_BA,   OP_C0, OP_C4),
    E2(0x3500, TEXT_LDRL,   CF_000F, SZ_QUAD, M_R,    M_RA,   OP_C0, OP_NO),
    E2(0x3500, TEXT_LDL,    CF_00FF, SZ_QUAD, M_R,    M_BA,   OP_C0, OP_C4),
    E2(0x3700, TEXT_LDRL,   CF_000F, SZ_QUAD, M_RA,   M_R,    OP_NO, OP_C0),
    E2(0x3700, TEXT_LDL,    CF_00FF, SZ_QUAD, M_BA,   M_R,    OP_C4, OP_C0),
    X2(0x7000, TEXT_LDB,    CF_00FF, SZ_BYTE, M_R,    M_BX,   OP_C0, OP_C4, PF_0X00),
    X2(0x7100, TEXT_LD,     CF_00FF, SZ_WORD, M_R,    M_BX,   OP_C0, OP_C4, PF_0X00),
    X2(0x7200, TEXT_LDB,    CF_00FF, SZ_BYTE, M_BX,   M_R,    OP_C4, OP_C0, PF_0X00),
    X2(0x7300, TEXT_LD,     CF_00FF, SZ_WORD, M_BX,   M_R,    OP_C4, OP_C0, PF_0X00),
    X2(0x7400, TEXT_LDA,    CF_00FF, SZ_ADDR, M_R,    M_BX,   OP_C0, OP_C4, PF_0X00),
    X2(0x7500, TEXT_LDL,    CF_00FF, SZ_QUAD, M_R,    M_BX,   OP_C0, OP_C4, PF_0X00),
    E2(0x7600, TEXT_LDA,    CF_00FF, SZ_ADDR, M_R,    M_GENA, OP_C0, OP_C4),
    X2(0x7700, TEXT_LDL,    CF_00FF, SZ_QUAD, M_BX,   M_R,    OP_C4, OP_C0, PF_0X00),
    E0(0x7A00, TEXT_HALT,   CF_0000),
    E0(0x7B00, TEXT_IRET,   CF_0000),
    E0(0x7B08, TEXT_MSET,   CF_0000),
    E0(0x7B09, TEXT_MRES,   CF_0000),
    E0(0x7B0A, TEXT_MBIT,   CF_0000),
    E1(0x7B0D, TEXT_MREQ,   CF_00F0, SZ_WORD, M_R,            OP_C4),
    E1(0x7C00, TEXT_DI,     CF_0003, SZ_NONE, M_INTR,         OP_C0),
    E1(0x7C04, TEXT_EI,     CF_0003, SZ_NONE, M_INTR,         OP_C0),
    E2(0x7D00, TEXT_LDCTL,  CF_00F7, SZ_WORD, M_R,    M_CTL,  OP_C4, OP_C0),
    E2(0x7D08, TEXT_LDCTL,  CF_00F7, SZ_WORD, M_CTL,  M_R,    OP_C0, OP_C4),
    E2(0x7F00, TEXT_SC,     CF_00FF, SZ_NONE, M_IM8,  M_NONE, OP_C0, OP_C0),
    E2(0x8C01, TEXT_LDCTLB, CF_00F0, SZ_BYTE, M_R,    M_CTL,  OP_C4, OP_C0),
    E2(0x8C09, TEXT_LDCTLB, CF_00F0, SZ_BYTE, M_CTL,  M_R,    OP_C0, OP_C4),
    E1(0x0C00, TEXT_COMB,   CF_C0F0, SZ_BYTE, M_GEND,         OP_C4),
    E2(0x0C01, TEXT_CPB,    CF_C0F0, SZ_BYTE, M_GENA, M_IM,   OP_C4, OP_NO),
    E1(0x0C02, TEXT_NEGB,   CF_C0F0, SZ_BYTE, M_GEND,         OP_C4), // NEGB/LDCTLB share same bit pattern
    E1(0x0C04, TEXT_TESTB,  CF_C0F0, SZ_BYTE, M_GEND,         OP_C4),
    E2(0x0C05, TEXT_LDB,    CF_C0F0, SZ_BYTE, M_GENA, M_IM,   OP_C4, OP_NO),
    E1(0x0C06, TEXT_TSETB,  CF_C0F0, SZ_BYTE, M_GEND,         OP_C4),
    E1(0x0C08, TEXT_CLRB,   CF_C0F0, SZ_BYTE, M_GEND,         OP_C4), // CLRB/LDCTLB share smae bit pattern
    E1(0x8D01, TEXT_SETFLG, CF_00F0, SZ_NONE, M_FLAG,         OP_C4),
    E1(0x8D03, TEXT_RESFLG, CF_00F0, SZ_NONE, M_FLAG,         OP_C4),
    E1(0x8D05, TEXT_COMFLG, CF_00F0, SZ_NONE, M_FLAG,         OP_C4),
    E0(0x8D07, TEXT_NOP,    CF_0000),
    E1(0x9E00, TEXT_RET,    CF_000F, SZ_NONE, M_CC,           OP_C0),
    E2(0xAE00, TEXT_TCCB,   CF_00FF, SZ_BYTE, M_CC,   M_R,    OP_C0, OP_C4),
    E1(0xAE08, TEXT_TCCB,   CF_00F0, SZ_BYTE, M_R,            OP_C4),
    E2(0xAF00, TEXT_TCC,    CF_00FF, SZ_WORD, M_CC,   M_R,    OP_C0, OP_C4),
    E1(0xAF08, TEXT_TCC,    CF_00F0, SZ_WORD, M_R,            OP_C4),
    E1(0xB000, TEXT_DAB,    CF_00F0, SZ_BYTE, M_R,            OP_C4),
    E1(0xB100, TEXT_EXTSB,  CF_00F0, SZ_BYTE, M_DBLR,         OP_C4),
    E1(0xB107, TEXT_EXTSL,  CF_00F0, SZ_QUAD, M_DBLR,         OP_C4),
    E1(0xB10A, TEXT_EXTS,   CF_00F0, SZ_WORD, M_DBLR,         OP_C4),
    E2(0xB200, TEXT_RLB,    CF_00F2, SZ_BYTE, M_R,    M_QCNT, OP_C4, OP_C0),
    E2(0xB201, TEXT_SLLB,   CF_00F0, SZ_BYTE, M_R,    M_SCNT, OP_C4, OP_NO),
    E2(0xB201, TEXT_SRLB,   CF_00F0, SZ_BYTE, M_R,    M_NCNT, OP_C4, OP_NO),
    X2(0xB203, TEXT_SDLB,   CF_00F0, SZ_BYTE, M_R,    M_WR,   OP_C4, OP_P8, PF_0X00),
    E2(0xB204, TEXT_RRB,    CF_00F2, SZ_BYTE, M_R,    M_QCNT, OP_C4, OP_C0),
    E2(0xB208, TEXT_RLCB,   CF_00F2, SZ_BYTE, M_R,    M_QCNT, OP_C4, OP_C0),
    E2(0xB209, TEXT_SLAB,   CF_00F0, SZ_BYTE, M_R,    M_SCNT, OP_C4, OP_NO),
    E2(0xB209, TEXT_SRAB,   CF_00F0, SZ_BYTE, M_R,    M_NCNT, OP_C4, OP_NO),
    X2(0xB20B, TEXT_SDAB,   CF_00F0, SZ_BYTE, M_R,    M_WR,   OP_C4, OP_P8, PF_0X00),
    E2(0xB20C, TEXT_RRCB,   CF_00F2, SZ_BYTE, M_R,    M_QCNT, OP_C4, OP_C0),
    E2(0xB300, TEXT_RL,     CF_00F2, SZ_WORD, M_R,    M_QCNT, OP_C4, OP_C0),
    E2(0xB301, TEXT_SLL,    CF_00F0, SZ_WORD, M_R,    M_SCNT, OP_C4, OP_NO),
    E2(0xB301, TEXT_SRL,    CF_00F0, SZ_WORD, M_R,    M_NCNT, OP_C4, OP_NO),
    X2(0xB303, TEXT_SDL,    CF_00F0, SZ_WORD, M_R,    M_WR,   OP_C4, OP_P8, PF_0X00),
    E2(0xB304, TEXT_RR,     CF_00F2, SZ_WORD, M_R,    M_QCNT, OP_C4, OP_C0),
    E2(0xB305, TEXT_SLLL,   CF_00F0, SZ_QUAD, M_R,    M_SCNT, OP_C4, OP_NO),
    E2(0xB305, TEXT_SRLL,   CF_00F0, SZ_QUAD, M_R,    M_NCNT, OP_C4, OP_NO),
    X2(0xB307, TEXT_SDLL,   CF_00F0, SZ_QUAD, M_R,    M_WR,   OP_C4, OP_P8, PF_0X00),
    E2(0xB308, TEXT_RLC,    CF_00F2, SZ_WORD, M_R,    M_QCNT, OP_C4, OP_C0),
    E2(0xB309, TEXT_SLA,    CF_00F0, SZ_WORD, M_R,    M_SCNT, OP_C4, OP_NO),
    E2(0xB309, TEXT_SRA,    CF_00F0, SZ_WORD, M_R,    M_NCNT, OP_C4, OP_NO),
    X2(0xB30B, TEXT_SDA,    CF_00F0, SZ_WORD, M_R,    M_WR,   OP_C4, OP_P8, PF_0X00),
    E2(0xB30C, TEXT_RRC,    CF_00F2, SZ_WORD, M_R,    M_QCNT, OP_C4, OP_C0),
    E2(0xB30D, TEXT_SLAL,   CF_00F0, SZ_QUAD, M_R,    M_SCNT, OP_C4, OP_NO),
    E2(0xB30D, TEXT_SRAL,   CF_00F0, SZ_QUAD, M_R,    M_NCNT, OP_C4, OP_NO),
    X2(0xB30F, TEXT_SDAL,   CF_00F0, SZ_QUAD, M_R,    M_WR,   OP_C4, OP_P8, PF_0X00),
    E2(0xB400, TEXT_ADCB,   CF_00FF, SZ_BYTE, M_R,    M_R,    OP_C0, OP_C4),
    E2(0xB500, TEXT_ADC,    CF_00FF, SZ_WORD, M_R,    M_R,    OP_C0, OP_C4),
    E2(0xB600, TEXT_SBCB,   CF_00FF, SZ_BYTE, M_R,    M_R,    OP_C0, OP_C4),
    E2(0xB700, TEXT_SBC,    CF_00FF, SZ_WORD, M_R,    M_R,    OP_C0, OP_C4),
    E2(0xBC00, TEXT_RRDB,   CF_00FF, SZ_BYTE, M_R,    M_R,    OP_C0, OP_C4),
    E2(0xBD00, TEXT_LDK,    CF_00FF, SZ_WORD, M_R,    M_BIT,  OP_C4, OP_C0),
    E2(0xBE00, TEXT_RLDB,   CF_00FF, SZ_BYTE, M_R,    M_R,    OP_C0, OP_C4),
    E2(0x0000, TEXT_ADDB,   CF_C0FF, SZ_BYTE, M_R,    M_GENI, OP_C0, OP_C4),
    E2(0x0100, TEXT_ADD,    CF_C0FF, SZ_WORD, M_R,    M_GENI, OP_C0, OP_C4),
    E2(0x0200, TEXT_SUBB,   CF_C0FF, SZ_BYTE, M_R,    M_GENI, OP_C0, OP_C4),
    E2(0x0300, TEXT_SUB,    CF_C0FF, SZ_WORD, M_R,    M_GENI, OP_C0, OP_C4),
    E2(0x0400, TEXT_ORB,    CF_C0FF, SZ_BYTE, M_R,    M_GENI, OP_C0, OP_C4),
    E2(0x0500, TEXT_OR,     CF_C0FF, SZ_WORD, M_R,    M_GENI, OP_C0, OP_C4),
    E2(0x0600, TEXT_ANDB,   CF_C0FF, SZ_BYTE, M_R,    M_GENI, OP_C0, OP_C4),
    E2(0x0700, TEXT_AND,    CF_C0FF, SZ_WORD, M_R,    M_GENI, OP_C0, OP_C4),
    E2(0x0800, TEXT_XORB,   CF_C0FF, SZ_BYTE, M_R,    M_GENI, OP_C0, OP_C4),
    E2(0x0900, TEXT_XOR,    CF_C0FF, SZ_WORD, M_R,    M_GENI, OP_C0, OP_C4),
    E2(0x0A00, TEXT_CPB,    CF_C0FF, SZ_BYTE, M_R,    M_GENI, OP_C0, OP_C4),
    E2(0x0B00, TEXT_CP,     CF_C0FF, SZ_WORD, M_R,    M_GENI, OP_C0, OP_C4),
    E1(0x0D00, TEXT_COM,    CF_C0F0, SZ_WORD, M_GEND,         OP_C4),
    E2(0x0D01, TEXT_CP,     CF_C0F0, SZ_WORD, M_GENA, M_IM,   OP_C4, OP_NO),
    E1(0x0D02, TEXT_NEG,    CF_C0F0, SZ_WORD, M_GEND,         OP_C4),
    E1(0x0D04, TEXT_TEST,   CF_C0F0, SZ_WORD, M_GEND,         OP_C4),
    E2(0x0D05, TEXT_LD,     CF_C0F0, SZ_WORD, M_GENA, M_IM,   OP_C4, OP_NO),
    E1(0x0D06, TEXT_TSET,   CF_C0F0, SZ_WORD, M_GEND,         OP_C4),
    E1(0x0D08, TEXT_CLR,    CF_C0F0, SZ_WORD, M_GEND,         OP_C4),
    E2(0x0D09, TEXT_PUSH,   CF_C0F0, SZ_WORD, M_IR,   M_IM,   OP_C4, OP_NO),
    E2(0x1000, TEXT_CPL,    CF_C0FF, SZ_QUAD, M_R,    M_GENI, OP_C0, OP_C4),
    E2(0x1100, TEXT_PUSHL,  CF_C0FF, SZ_QUAD, M_IR,   M_GEND, OP_C4, OP_C0),
    E2(0x1200, TEXT_SUBL,   CF_C0FF, SZ_QUAD, M_R,    M_GENI, OP_C0, OP_C4),
    E2(0x1300, TEXT_PUSH,   CF_C0FF, SZ_WORD, M_IR,   M_GEND, OP_C4, OP_C0),
    E2(0x1400, TEXT_LDL,    CF_C0FF, SZ_QUAD, M_R,    M_GENI, OP_C0, OP_C4),
    E2(0x1500, TEXT_POPL,   CF_C0FF, SZ_QUAD, M_GEND, M_IR,   OP_C0, OP_C4),
    E2(0x1600, TEXT_ADDL,   CF_C0FF, SZ_QUAD, M_R,    M_GENI, OP_C0, OP_C4),
    E2(0x1700, TEXT_POP,    CF_C0FF, SZ_WORD, M_GEND, M_IR,   OP_C0, OP_C4),
    E2(0x1800, TEXT_MULTL,  CF_C0FF, SZ_QUAD, M_DBLR, M_GENI, OP_C0, OP_C4),
    E2(0x1900, TEXT_MULT,   CF_C0FF, SZ_WORD, M_DBLR, M_GENI, OP_C0, OP_C4),
    E2(0x1A00, TEXT_DIVL,   CF_C0FF, SZ_QUAD, M_DBLR, M_GENI, OP_C0, OP_C4),
    E2(0x1B00, TEXT_DIV,    CF_C0FF, SZ_WORD, M_DBLR, M_GENI, OP_C0, OP_C4),
    X3(0x1C01, TEXT_LDM,    CF_C0F0, SZ_WORD, M_R,    M_GENA, OP_P8, OP_C4, E1_CNT, PF_0X0X),
    E1(0x1C08, TEXT_TESTL,  CF_C0F0, SZ_QUAD, M_GEND,         OP_C4),
    X3(0x1C09, TEXT_LDM,    CF_C0F0, SZ_WORD, M_GENA, M_R,    OP_C4, OP_P8, E1_CNT, PF_0X0X),
    E2(0x1D00, TEXT_LDL,    CF_C0FF, SZ_QUAD, M_GENA, M_R,    OP_C4, OP_C0),
    E2(0x1E00, TEXT_JP,     CF_C0FF, SZ_WORD, M_CC,   M_GENA, OP_C0, OP_C4),
    E1(0x1E08, TEXT_JP,     CF_C0F0, SZ_WORD, M_GENA,         OP_C4),
    E1(0x1F00, TEXT_CALL,   CF_C0FF, SZ_WORD, M_GENA,         OP_C4),
    E2(0x2000, TEXT_LDB,    CF_C0FF, SZ_BYTE, M_R,    M_GENI, OP_C0, OP_C4),
    E2(0x2100, TEXT_LD,     CF_C0FF, SZ_WORD, M_R,    M_GENI, OP_C0, OP_C4),
    X2(0x2200, TEXT_RESB,   CF_000F, SZ_BYTE, M_R,    M_WR07, OP_P8, OP_C0, PF_0X00),
    E2(0x2200, TEXT_RESB,   CF_X0FF, SZ_BYTE, M_GEND, M_BIT,  OP_C4, OP_C0),
    X2(0x2300, TEXT_RES,    CF_000F, SZ_WORD, M_R,    M_R,    OP_P8, OP_C0, PF_0X00),
    E2(0x2300, TEXT_RES,    CF_X0FF, SZ_WORD, M_GEND, M_BIT,  OP_C4, OP_C0),
    X2(0x2400, TEXT_SETB,   CF_000F, SZ_BYTE, M_R,    M_WR07, OP_P8, OP_C0, PF_0X00),
    E2(0x2400, TEXT_SETB,   CF_X0FF, SZ_BYTE, M_GEND, M_BIT,  OP_C4, OP_C0),
    X2(0x2500, TEXT_SET,    CF_000F, SZ_WORD, M_R,    M_R,    OP_P8, OP_C0, PF_0X00),
    E2(0x2500, TEXT_SET,    CF_X0FF, SZ_WORD, M_GEND, M_BIT,  OP_C4, OP_C0),
    X2(0x2600, TEXT_BITB,   CF_000F, SZ_BYTE, M_R,    M_WR07, OP_P8, OP_C0, PF_0X00),
    E2(0x2600, TEXT_BITB,   CF_X0FF, SZ_BYTE, M_GEND, M_BIT,  OP_C4, OP_C0),
    X2(0x2700, TEXT_BIT,    CF_000F, SZ_WORD, M_R,    M_R,    OP_P8, OP_C0, PF_0X00),
    E2(0x2700, TEXT_BIT,    CF_X0FF, SZ_WORD, M_GEND, M_BIT,  OP_C4, OP_C0),
    E2(0x2800, TEXT_INCB,   CF_C0FF, SZ_BYTE, M_GEND, M_CNT,  OP_C4, OP_C0),
    E2(0x2900, TEXT_INC,    CF_C0FF, SZ_WORD, M_GEND, M_CNT,  OP_C4, OP_C0),
    E2(0x2A00, TEXT_DECB,   CF_C0FF, SZ_BYTE, M_GEND, M_CNT,  OP_C4, OP_C0),
    E2(0x2B00, TEXT_DEC,    CF_C0FF, SZ_WORD, M_GEND, M_CNT,  OP_C4, OP_C0),
    E2(0x2C00, TEXT_EXB,    CF_C0FF, SZ_BYTE, M_R,    M_GEND, OP_C0, OP_C4),
    E2(0x2D00, TEXT_EX,     CF_C0FF, SZ_WORD, M_R,    M_GEND, OP_C0, OP_C4),
    E2(0x2E00, TEXT_LDB,    CF_C0FF, SZ_BYTE, M_GENA, M_R,    OP_C4, OP_C0),
    E2(0x2F00, TEXT_LD,     CF_C0FF, SZ_WORD, M_GENA, M_R,    OP_C4, OP_C0),
    E1(0x3900, TEXT_LDPS,   CF_C0F0, SZ_WORD, M_GENA,         OP_C4),
    E2(0x3A04, TEXT_INB,    CF_00F0, SZ_BYTE, M_R,    M_IO,   OP_C4, OP_NO),
    E2(0x3A05, TEXT_SINB,   CF_00F0, SZ_BYTE, M_R,    M_IO,   OP_C4, OP_NO),
    E2(0x3A06, TEXT_OUTB,   CF_00F0, SZ_BYTE, M_IO,   M_R,    OP_NO, OP_C4),
    E2(0x3A07, TEXT_SOUTB,  CF_00F0, SZ_BYTE, M_IO,   M_R,    OP_NO, OP_C4),
    E2(0x3B04, TEXT_IN,     CF_00F0, SZ_WORD, M_R,    M_IO,   OP_C4, OP_NO),
    E2(0x3B05, TEXT_SIN,    CF_00F0, SZ_WORD, M_R,    M_IO,   OP_C4, OP_NO),
    E2(0x3B06, TEXT_OUT,    CF_00F0, SZ_WORD, M_IO,   M_R,    OP_NO, OP_C4),
    E2(0x3B07, TEXT_SOUT,   CF_00F0, SZ_WORD, M_IO,   M_R,    OP_NO, OP_C4),
    E2(0x3C00, TEXT_INB,    CF_00FF, SZ_BYTE, M_R,    M_IRIO, OP_C0, OP_C4),
    E2(0x3D00, TEXT_IN,     CF_00FF, SZ_WORD, M_R,    M_IRIO, OP_C0, OP_C4),
    E2(0x3E00, TEXT_OUTB,   CF_00FF, SZ_BYTE, M_IRIO, M_R,    OP_C4, OP_C0),
    E2(0x3F00, TEXT_OUT,    CF_00FF, SZ_WORD, M_IRIO, M_R,    OP_C4, OP_C0),
    X3(0x3A00, TEXT_INIRB,  CF_00F0, SZ_BYTE, M_IR,   M_IRIO, OP_P4, OP_C4, E1_WR, PF_0XX0),
    X3(0x3A00, TEXT_INIB,   CF_00F0, SZ_BYTE, M_IR,   M_IRIO, OP_P4, OP_C4, E1_WR, PF_0XX8),
    X3(0x3A01, TEXT_SINIRB, CF_00F0, SZ_BYTE, M_IR,   M_IRIO, OP_P4, OP_C4, E1_WR, PF_0XX0),
    X3(0x3A01, TEXT_SINIB,  CF_00F0, SZ_BYTE, M_IR,   M_IRIO, OP_P4, OP_C4, E1_WR, PF_0XX8),
    X3(0x3A02, TEXT_OTIRB,  CF_00F0, SZ_BYTE, M_IRIO, M_IR,   OP_P4, OP_C4, E1_WR, PF_0XX0),
    X3(0x3A02, TEXT_OUTIB,  CF_00F0, SZ_BYTE, M_IRIO, M_IR,   OP_P4, OP_C4, E1_WR, PF_0XX8),
    X3(0x3A03, TEXT_SOTIRB, CF_00F0, SZ_BYTE, M_IRIO, M_IR,   OP_P4, OP_C4, E1_WR, PF_0XX0),
    X3(0x3A03, TEXT_SOUTIB, CF_00F0, SZ_BYTE, M_IRIO, M_IR,   OP_P4, OP_C4, E1_WR, PF_0XX8),
    X3(0x3A08, TEXT_INDRB,  CF_00F0, SZ_BYTE, M_IR,   M_IRIO, OP_P4, OP_C4, E1_WR, PF_0XX0),
    X3(0x3A08, TEXT_INDB,   CF_00F0, SZ_BYTE, M_IR,   M_IRIO, OP_P4, OP_C4, E1_WR, PF_0XX8),
    X3(0x3A09, TEXT_SINDRB, CF_00F0, SZ_BYTE, M_IR,   M_IRIO, OP_P4, OP_C4, E1_WR, PF_0XX0),
    X3(0x3A09, TEXT_SINDB,  CF_00F0, SZ_BYTE, M_IR,   M_IRIO, OP_P4, OP_C4, E1_WR, PF_0XX8),
    X3(0x3A0A, TEXT_OTDRB,  CF_00F0, SZ_BYTE, M_IRIO, M_IR,   OP_P4, OP_C4, E1_WR, PF_0XX0),
    X3(0x3A0A, TEXT_OUTDB,  CF_00F0, SZ_BYTE, M_IRIO, M_IR,   OP_P4, OP_C4, E1_WR, PF_0XX8),
    X3(0x3A0B, TEXT_SOTDRB, CF_00F0, SZ_BYTE, M_IRIO, M_IR,   OP_P4, OP_C4, E1_WR, PF_0XX0),
    X3(0x3A0B, TEXT_SOUTDB, CF_00F0, SZ_BYTE, M_IRIO, M_IR,   OP_P4, OP_C4, E1_WR, PF_0XX8),
    X3(0x3B00, TEXT_INIR,   CF_00F0, SZ_WORD, M_IR,   M_IRIO, OP_P4, OP_C4, E1_WR, PF_0XX0),
    X3(0x3B00, TEXT_INI,    CF_00F0, SZ_WORD, M_IR,   M_IRIO, OP_P4, OP_C4, E1_WR, PF_0XX8),
    X3(0x3B01, TEXT_SINIR,  CF_00F0, SZ_WORD, M_IR,   M_IRIO, OP_P4, OP_C4, E1_WR, PF_0XX0),
    X3(0x3B01, TEXT_SINI,   CF_00F0, SZ_WORD, M_IR,   M_IRIO, OP_P4, OP_C4, E1_WR, PF_0XX8),
    X3(0x3B02, TEXT_OTIR,   CF_00F0, SZ_WORD, M_IRIO, M_IR,   OP_P4, OP_C4, E1_WR, PF_0XX0),
    X3(0x3B02, TEXT_OUTI,   CF_00F0, SZ_WORD, M_IRIO, M_IR,   OP_P4, OP_C4, E1_WR, PF_0XX8),
    X3(0x3B03, TEXT_SOTIR,  CF_00F0, SZ_WORD, M_IRIO, M_IR,   OP_P4, OP_C4, E1_WR, PF_0XX0),
    X3(0x3B03, TEXT_SOUTI,  CF_00F0, SZ_WORD, M_IRIO, M_IR,   OP_P4, OP_C4, E1_WR, PF_0XX8),
    X3(0x3B08, TEXT_INDR,   CF_00F0, SZ_WORD, M_IR,   M_IRIO, OP_P4, OP_C4, E1_WR, PF_0XX0),
    X3(0x3B08, TEXT_IND,    CF_00F0, SZ_WORD, M_IR,   M_IRIO, OP_P4, OP_C4, E1_WR, PF_0XX8),
    X3(0x3B09, TEXT_SINDR,  CF_00F0, SZ_WORD, M_IR,   M_IRIO, OP_P4, OP_C4, E1_WR, PF_0XX0),
    X3(0x3B09, TEXT_SIND,   CF_00F0, SZ_WORD, M_IR,   M_IRIO, OP_P4, OP_C4, E1_WR, PF_0XX8),
    X3(0x3B0A, TEXT_OTDR,   CF_00F0, SZ_WORD, M_IRIO, M_IR,   OP_P4, OP_C4, E1_WR, PF_0XX0),
    X3(0x3B0A, TEXT_OUTD,   CF_00F0, SZ_WORD, M_IRIO, M_IR,   OP_P4, OP_C4, E1_WR, PF_0XX8),
    X3(0x3B0B, TEXT_SOTDR,  CF_00F0, SZ_WORD, M_IRIO, M_IR,   OP_P4, OP_C4, E1_WR, PF_0XX0),
    X3(0x3B0B, TEXT_SOUTD,  CF_00F0, SZ_WORD, M_IRIO, M_IR,   OP_P4, OP_C4, E1_WR, PF_0XX8),
    X3(0xB800, TEXT_TRIB,   CF_00F0, SZ_BYTE, M_IR,   M_IR,   OP_C4, OP_P4, E1_WR, PF_0XX0),
    X3(0xB802, TEXT_TRTIB,  CF_00F0, SZ_BYTE, M_IR,   M_IR,   OP_C4, OP_P4, E1_WR, PF_0XX0),
    X3(0xB804, TEXT_TRIRB,  CF_00F0, SZ_BYTE, M_IR,   M_IR,   OP_C4, OP_P4, E1_WR, PF_0XX0),
    X3(0xB806, TEXT_TRTIRB, CF_00F0, SZ_BYTE, M_IR,   M_IR,   OP_C4, OP_P4, E1_WR, PF_0XXE),
    X3(0xB808, TEXT_TRDB,   CF_00F0, SZ_BYTE, M_IR,   M_IR,   OP_C4, OP_P4, E1_WR, PF_0XX0),
    X3(0xB80A, TEXT_TRTDB,  CF_00F0, SZ_BYTE, M_IR,   M_IR,   OP_C4, OP_P4, E1_WR, PF_0XX0),
    X3(0xB80C, TEXT_TRDRB,  CF_00F0, SZ_BYTE, M_IR,   M_IR,   OP_C4, OP_P4, E1_WR, PF_0XX0),
    X3(0xB80E, TEXT_TRTDRB, CF_00F0, SZ_BYTE, M_IR,   M_IR,   OP_C4, OP_P4, E1_WR, PF_0XXE),
    X4(0xBA00, TEXT_CPIB,   CF_00F0, SZ_BYTE, M_R,    M_IR,   OP_P4, OP_C4, E1_WR, E2_CC, PF_0XXX),
    X3(0xBA01, TEXT_LDIRB,  CF_00F0, SZ_BYTE, M_IR,   M_IR,   OP_P4, OP_C4, E1_WR,        PF_0XX0),
    X3(0xBA01, TEXT_LDIB,   CF_00F0, SZ_BYTE, M_IR,   M_IR,   OP_P4, OP_C4, E1_WR,        PF_0XX8),
    X4(0xBA02, TEXT_CPSIB,  CF_00F0, SZ_BYTE, M_IR,   M_IR,   OP_P4, OP_C4, E1_WR, E2_CC, PF_0XXX),
    X4(0xBA04, TEXT_CPIRB,  CF_00F0, SZ_BYTE, M_R,    M_IR,   OP_P4, OP_C4, E1_WR, E2_CC, PF_0XXX),
    X4(0xBA06, TEXT_CPSIRB, CF_00F0, SZ_BYTE, M_IR,   M_IR,   OP_P4, OP_C4, E1_WR, E2_CC, PF_0XXX),
    X4(0xBA08, TEXT_CPDB,   CF_00F0, SZ_BYTE, M_R,    M_IR,   OP_P4, OP_C4, E1_WR, E2_CC, PF_0XXX),
    X3(0xBA09, TEXT_LDDRB,  CF_00F0, SZ_BYTE, M_IR,   M_IR,   OP_P4, OP_C4, E1_WR,        PF_0XX0),
    X3(0xBA09, TEXT_LDDB,   CF_00F0, SZ_BYTE, M_IR,   M_IR,   OP_P4, OP_C4, E1_WR,        PF_0XX8),
    X4(0xBA0A, TEXT_CPSDB,  CF_00F0, SZ_BYTE, M_IR,   M_IR,   OP_P4, OP_C4, E1_WR, E2_CC, PF_0XXX),
    X4(0xBA0C, TEXT_CPDRB,  CF_00F0, SZ_BYTE, M_R,    M_IR,   OP_P4, OP_C4, E1_WR, E2_CC, PF_0XXX),
    X4(0xBA0E, TEXT_CPSDRB, CF_00F0, SZ_BYTE, M_IR,   M_IR,   OP_P4, OP_C4, E1_WR, E2_CC, PF_0XXX),
    X4(0xBB00, TEXT_CPI,    CF_00F0, SZ_WORD, M_R,    M_IR,   OP_P4, OP_C4, E1_WR, E2_CC, PF_0XXX),
    X3(0xBB01, TEXT_LDIR,   CF_00F0, SZ_WORD, M_IR,   M_IR,   OP_P4, OP_C4, E1_WR,        PF_0XX0),
    X3(0xBB01, TEXT_LDI,    CF_00F0, SZ_WORD, M_IR,   M_IR,   OP_P4, OP_C4, E1_WR,        PF_0XX8),
    X4(0xBB02, TEXT_CPSI,   CF_00F0, SZ_WORD, M_IR,   M_IR,   OP_P4, OP_C4, E1_WR, E2_CC, PF_0XXX),
    X4(0xBB04, TEXT_CPIR,   CF_00F0, SZ_WORD, M_R,    M_IR,   OP_P4, OP_C4, E1_WR, E2_CC, PF_0XXX),
    X4(0xBB06, TEXT_CPSIR,  CF_00F0, SZ_WORD, M_IR,   M_IR,   OP_P4, OP_C4, E1_WR, E2_CC, PF_0XXX),
    X4(0xBB08, TEXT_CPD,    CF_00F0, SZ_WORD, M_R,    M_IR,   OP_P4, OP_C4, E1_WR, E2_CC, PF_0XXX),
    X3(0xBB09, TEXT_LDDR,   CF_00F0, SZ_WORD, M_IR,   M_IR,   OP_P4, OP_C4, E1_WR,        PF_0XX0),
    X3(0xBB09, TEXT_LDD,    CF_00F0, SZ_WORD, M_IR,   M_IR,   OP_P4, OP_C4, E1_WR,        PF_0XX8),
    X4(0xBB0A, TEXT_CPSD,   CF_00F0, SZ_WORD, M_IR,   M_IR,   OP_P4, OP_C4, E1_WR, E2_CC, PF_0XXX),
    X4(0xBB0C, TEXT_CPDR,   CF_00F0, SZ_WORD, M_R,    M_IR,   OP_P4, OP_C4, E1_WR, E2_CC, PF_0XXX),
    X4(0xBB0E, TEXT_CPSDR,  CF_00F0, SZ_WORD, M_IR,   M_IR,   OP_P4, OP_C4, E1_WR, E2_CC, PF_0XXX),
};

constexpr uint8_t INDEX_Z8000[] PROGMEM = {
     88,  // TEXT_ADC
     87,  // TEXT_ADCB
     95,  // TEXT_ADD
     94,  // TEXT_ADDB
    120,  // TEXT_ADDL
    101,  // TEXT_AND
    100,  // TEXT_ANDB
    145,  // TEXT_BIT
    146,  // TEXT_BIT
    143,  // TEXT_BITB
    144,  // TEXT_BITB
    132,  // TEXT_CALL
      1,  // TEXT_CALR
    112,  // TEXT_CLR
     47,  // TEXT_CLRB
    106,  // TEXT_COM
     41,  // TEXT_COMB
     50,  // TEXT_COMFLG
    105,  // TEXT_CP
    107,  // TEXT_CP
     42,  // TEXT_CPB
    104,  // TEXT_CPB
    226,  // TEXT_CPD
    214,  // TEXT_CPDB
    230,  // TEXT_CPDR
    218,  // TEXT_CPDRB
    220,  // TEXT_CPI
    208,  // TEXT_CPIB
    224,  // TEXT_CPIR
    212,  // TEXT_CPIRB
    114,  // TEXT_CPL
    229,  // TEXT_CPSD
    217,  // TEXT_CPSDB
    231,  // TEXT_CPSDR
    219,  // TEXT_CPSDRB
    223,  // TEXT_CPSI
    211,  // TEXT_CPSIB
    225,  // TEXT_CPSIR
    213,  // TEXT_CPSIRB
     57,  // TEXT_DAB
      4,  // TEXT_DBJNZ
    150,  // TEXT_DEC
    149,  // TEXT_DECB
     34,  // TEXT_DI
    125,  // TEXT_DIV
    124,  // TEXT_DIVL
      5,  // TEXT_DJNZ
     35,  // TEXT_EI
    152,  // TEXT_EX
    151,  // TEXT_EXB
     60,  // TEXT_EXTS
     58,  // TEXT_EXTSB
     59,  // TEXT_EXTSL
     28,  // TEXT_HALT
    160,  // TEXT_IN
    165,  // TEXT_IN
    156,  // TEXT_INB
    164,  // TEXT_INB
    148,  // TEXT_INC
    147,  // TEXT_INCB
    193,  // TEXT_IND
    177,  // TEXT_INDB
    192,  // TEXT_INDR
    176,  // TEXT_INDRB
    185,  // TEXT_INI
    169,  // TEXT_INIB
    184,  // TEXT_INIR
    168,  // TEXT_INIRB
     29,  // TEXT_IRET
    130,  // TEXT_JP
    131,  // TEXT_JP
      2,  // TEXT_JR
      3,  // TEXT_JR
      9,  // TEXT_LD
     13,  // TEXT_LD
     21,  // TEXT_LD
     23,  // TEXT_LD
    110,  // TEXT_LD
    134,  // TEXT_LD
    154,  // TEXT_LD
     15,  // TEXT_LDA
     24,  // TEXT_LDA
     26,  // TEXT_LDA
     14,  // TEXT_LDAR
      0,  // TEXT_LDB
      7,  // TEXT_LDB
     11,  // TEXT_LDB
     20,  // TEXT_LDB
     22,  // TEXT_LDB
     45,  // TEXT_LDB
    133,  // TEXT_LDB
    153,  // TEXT_LDB
     36,  // TEXT_LDCTL
     37,  // TEXT_LDCTL
     39,  // TEXT_LDCTLB
     40,  // TEXT_LDCTLB
    228,  // TEXT_LDD
    216,  // TEXT_LDDB
    227,  // TEXT_LDDR
    215,  // TEXT_LDDRB
    222,  // TEXT_LDI
    210,  // TEXT_LDIB
    221,  // TEXT_LDIR
    209,  // TEXT_LDIRB
     92,  // TEXT_LDK
     17,  // TEXT_LDL
     19,  // TEXT_LDL
     25,  // TEXT_LDL
     27,  // TEXT_LDL
    118,  // TEXT_LDL
    129,  // TEXT_LDL
    126,  // TEXT_LDM
    128,  // TEXT_LDM
    155,  // TEXT_LDPS
      8,  // TEXT_LDR
     12,  // TEXT_LDR
      6,  // TEXT_LDRB
     10,  // TEXT_LDRB
     16,  // TEXT_LDRL
     18,  // TEXT_LDRL
     32,  // TEXT_MBIT
     33,  // TEXT_MREQ
     31,  // TEXT_MRES
     30,  // TEXT_MSET
    123,  // TEXT_MULT
    122,  // TEXT_MULTL
    108,  // TEXT_NEG
     43,  // TEXT_NEGB
     51,  // TEXT_NOP
     99,  // TEXT_OR
     98,  // TEXT_ORB
    196,  // TEXT_OTDR
    180,  // TEXT_OTDRB
    188,  // TEXT_OTIR
    172,  // TEXT_OTIRB
    162,  // TEXT_OUT
    167,  // TEXT_OUT
    158,  // TEXT_OUTB
    166,  // TEXT_OUTB
    197,  // TEXT_OUTD
    181,  // TEXT_OUTDB
    189,  // TEXT_OUTI
    173,  // TEXT_OUTIB
    121,  // TEXT_POP
    119,  // TEXT_POPL
    113,  // TEXT_PUSH
    117,  // TEXT_PUSH
    115,  // TEXT_PUSHL
    137,  // TEXT_RES
    138,  // TEXT_RES
    135,  // TEXT_RESB
    136,  // TEXT_RESB
     49,  // TEXT_RESFLG
     52,  // TEXT_RET
     71,  // TEXT_RL
     61,  // TEXT_RLB
     79,  // TEXT_RLC
     66,  // TEXT_RLCB
     93,  // TEXT_RLDB
     75,  // TEXT_RR
     65,  // TEXT_RRB
     83,  // TEXT_RRC
     70,  // TEXT_RRCB
     91,  // TEXT_RRDB
     90,  // TEXT_SBC
     89,  // TEXT_SBCB
     38,  // TEXT_SC
     82,  // TEXT_SDA
     69,  // TEXT_SDAB
     86,  // TEXT_SDAL
     74,  // TEXT_SDL
     64,  // TEXT_SDLB
     78,  // TEXT_SDLL
    141,  // TEXT_SET
    142,  // TEXT_SET
    139,  // TEXT_SETB
    140,  // TEXT_SETB
     48,  // TEXT_SETFLG
    161,  // TEXT_SIN
    157,  // TEXT_SINB
    195,  // TEXT_SIND
    179,  // TEXT_SINDB
    194,  // TEXT_SINDR
    178,  // TEXT_SINDRB
    187,  // TEXT_SINI
    171,  // TEXT_SINIB
    186,  // TEXT_SINIR
    170,  // TEXT_SINIRB
     80,  // TEXT_SLA
     67,  // TEXT_SLAB
     84,  // TEXT_SLAL
     72,  // TEXT_SLL
     62,  // TEXT_SLLB
     76,  // TEXT_SLLL
    198,  // TEXT_SOTDR
    182,  // TEXT_SOTDRB
    190,  // TEXT_SOTIR
    174,  // TEXT_SOTIRB
    163,  // TEXT_SOUT
    159,  // TEXT_SOUTB
    199,  // TEXT_SOUTD
    183,  // TEXT_SOUTDB
    191,  // TEXT_SOUTI
    175,  // TEXT_SOUTIB
     81,  // TEXT_SRA
     68,  // TEXT_SRAB
     85,  // TEXT_SRAL
     73,  // TEXT_SRL
     63,  // TEXT_SRLB
     77,  // TEXT_SRLL
     97,  // TEXT_SUB
     96,  // TEXT_SUBB
    116,  // TEXT_SUBL
     55,  // TEXT_TCC
     56,  // TEXT_TCC
     53,  // TEXT_TCCB
     54,  // TEXT_TCCB
    109,  // TEXT_TEST
     44,  // TEXT_TESTB
    127,  // TEXT_TESTL
    204,  // TEXT_TRDB
    206,  // TEXT_TRDRB
    200,  // TEXT_TRIB
    202,  // TEXT_TRIRB
    205,  // TEXT_TRTDB
    207,  // TEXT_TRTDRB
    201,  // TEXT_TRTIB
    203,  // TEXT_TRTIRB
    111,  // TEXT_TSET
     46,  // TEXT_TSETB
    103,  // TEXT_XOR
    102,  // TEXT_XORB
};
// clang-format on

using EntryPage = entry::TableBase<Entry>;

constexpr EntryPage Z8000_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_Z8000), ARRAY_RANGE(INDEX_Z8000)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {Z8001, TEXT_CPU_Z8001, ARRAY_RANGE(Z8000_PAGES)},
        {Z8002, TEXT_CPU_Z8002, ARRAY_RANGE(Z8000_PAGES)},
};

const Cpu *cpu(CpuType cpuType) {
    return Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
}

bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_R)
        return table == M_GENI || table == M_GEND || table == M_WR07 || table == M_WR ||
               table == M_DBLR;
    if (opr == M_IM)
        return table == M_GENI || table == M_IM8 || table == M_BIT || table == M_CNT ||
               table == M_QCNT || table == M_SCNT || table == M_NCNT || table == M_IO;
    if (opr == M_IR)
        return table == M_GENI || table == M_GEND || table == M_GENA || table == M_IRIO;
    if (opr == M_DA)
        return table == M_GENI || table == M_GEND || table == M_GENA || table == M_RA ||
               table == M_RA12 || table == M_RA8 || table == M_RA7 || table == M_IO;
    if (opr == M_X)
        return table == M_GENI || table == M_GEND || table == M_GENA;
    if (opr == M_CC)
        return table == M_FLAG;  // C & Z
    if (opr == M_NONE)
        return table == M_CC || table == M_INTR || table == M_FLAG;
    return false;
}

bool acceptModes(AsmInsn &insn, const Entry *entry) {
    const auto table = entry->readFlags();
    return acceptMode(insn.dstOp.mode, table.dst()) && acceptMode(insn.srcOp.mode, table.src()) &&
           acceptMode(insn.ex1Op.mode, table.ex1()) && acceptMode(insn.ex2Op.mode, table.ex2());
}

Error searchName(CpuType cpuType, AsmInsn &insn) {
    cpu(cpuType)->searchName(insn, acceptModes);
    return insn.getError();
}

bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *) {
    const auto flags = entry->readFlags();
    const auto cf = flags.codeFormat();
    const auto opc = insn.opCode();
    if ((cf == CF_C0F0 || cf == CF_C0FF) && (opc & 0xC000) == 0xC000)
        return false;
    if (cf == CF_X0FF && (opc & 0xC000) == 0 && (opc & 0x00F0) == 0)
        return false;
    if ((opc & ~flags.codeMask()) != entry->readOpCode())
        return false;
    if (flags.postFormat() != PF_NONE) {
        insn.readPostfix();
        return (insn.postfix() & flags.postMask()) == flags.postVal();
    }
    return true;
}

Error searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) {
    cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    return insn.getError();
}

Error searchOpCodeAlias(CpuType cpuType, DisInsn &insn, StrBuffer &out) {
    auto entry = cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    if (entry) {
        entry++;
        Cpu::defaultReadName(insn, entry, out);
    }
    return insn.getError();
}

const /*PROGMEM*/ char *TableZ8000::listCpu_P() const {
    return TEXT_Z8000_LIST;
}

const /*PROGMEM*/ char *TableZ8000::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableZ8000::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    auto t = Cpu::search(name, ARRAY_RANGE(CPU_TABLE));
    if (t) {
        cpuType = t->readCpuType();
        return OK;
    }
    return UNSUPPORTED_CPU;
}

const TableZ8000 TABLE;

}  // namespace z8000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

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
#include "config_z8000.h"
#include "entry_z8000.h"
#include "text_z8000.h"

#include <string.h>

namespace libasm {
namespace z8000 {

#define X(_opc, _cm, _sz, _name, _dst, _src, _dstf, _srcf, _ex1, _ex2, _post)               \
    {                                                                                       \
        _opc,                                                                               \
                Entry::Flags::create(_dst, MF_##_dstf, _src, MF_##_srcf, _ex1, _ex2, _post, \
                        CM_##_cm, SZ_##_sz),                                                \
                _name                                                                       \
    }
#define E(_opc, _cm, _sz, _name, _dst, _src, _dstf, _srcf) \
    X(_opc, _cm, _sz, _name, _dst, _src, _dstf, _srcf, M_NO, M_NO, P_NO)

// clang-format off
static const Entry Z8000_TABLE[] PROGMEM = {
    E(0xC000, 0x0FFF, BYTE, TEXT_LDB,    M_R,    M_IM8,  C8, C0),
    E(0xD000, 0x0FFF, NONE, TEXT_CALR,   M_RA12, M_NO,   C0, NO),
    E(0xE000, 0x0FFF, NONE, TEXT_JR,     M_CC,   M_RA8,  C8, C0),
    E(0xE800, 0x00FF, NONE, TEXT_JR,     M_RA8,  M_NO,   C0, NO),
    E(0xF000, 0x0F7F, BYTE, TEXT_DBJNZ,  M_R,    M_RA7,  C8, C0),
    E(0xF080, 0x0F7F, WORD, TEXT_DJNZ,   M_R,    M_RA7,  C8, C0),
    E(0x3000, 0x000F, BYTE, TEXT_LDRB,   M_R,    M_RA,   C0, NO),
    E(0x3000, 0x00FF, BYTE, TEXT_LDB,    M_R,    M_BA,   C0, C4),
    E(0x3100, 0x000F, WORD, TEXT_LDR,    M_R,    M_RA,   C0, NO),
    E(0x3100, 0x00FF, WORD, TEXT_LD,     M_R,    M_BA,   C0, C4),
    E(0x3200, 0x000F, BYTE, TEXT_LDRB,   M_RA,   M_R,    NO, C0),
    E(0x3200, 0x00FF, BYTE, TEXT_LDB,    M_BA,   M_R,    C4, C0),
    E(0x3300, 0x000F, WORD, TEXT_LDR,    M_RA,   M_R,    NO, C0),
    E(0x3300, 0x00FF, WORD, TEXT_LD,     M_BA,   M_R,    C4, C0),
    E(0x3400, 0x000F, ADDR, TEXT_LDAR,   M_R,    M_RA,   C0, NO),
    E(0x3400, 0x00FF, ADDR, TEXT_LDA,    M_R,    M_BA,   C0, C4),
    E(0x3500, 0x000F, LONG, TEXT_LDRL,   M_R,    M_RA,   C0, NO),
    E(0x3500, 0x00FF, LONG, TEXT_LDL,    M_R,    M_BA,   C0, C4),
    E(0x3700, 0x000F, LONG, TEXT_LDRL,   M_RA,   M_R,    NO, C0),
    E(0x3700, 0x00FF, LONG, TEXT_LDL,    M_BA,   M_R,    C4, C0),
    E(0x3C00, 0x00FF, BYTE, TEXT_INB,    M_R,    M_IRIO, C0, C4),
    E(0x3D00, 0x00FF, WORD, TEXT_IN,     M_R,    M_IRIO, C0, C4),
    E(0x3E00, 0x00FF, BYTE, TEXT_OUTB,   M_IRIO, M_R,    C4, C0),
    E(0x3F00, 0x00FF, WORD, TEXT_OUT,    M_IRIO, M_R,    C4, C0),
    X(0x7000, 0x00FF, BYTE, TEXT_LDB,    M_R,    M_BX,   C0, C4, M_NO, M_NO, P_0X00),
    X(0x7100, 0x00FF, WORD, TEXT_LD,     M_R,    M_BX,   C0, C4, M_NO, M_NO, P_0X00),
    X(0x7200, 0x00FF, BYTE, TEXT_LDB,    M_BX,   M_R,    C4, C0, M_NO, M_NO, P_0X00),
    X(0x7300, 0x00FF, WORD, TEXT_LD,     M_BX,   M_R,    C4, C0, M_NO, M_NO, P_0X00),
    X(0x7400, 0x00FF, ADDR, TEXT_LDA,    M_R,    M_BX,   C0, C4, M_NO, M_NO, P_0X00),
    X(0x7500, 0x00FF, LONG, TEXT_LDL,    M_R,    M_BX,   C0, C4, M_NO, M_NO, P_0X00),
    E(0x7600, 0x00FF, ADDR, TEXT_LDA,    M_R,    M_GENA, C0, C4),
    X(0x7700, 0x00FF, LONG, TEXT_LDL,    M_BX,   M_R,    C4, C0, M_NO, M_NO, P_0X00),
    E(0x7A00, 0x0000, NONE, TEXT_HALT,   M_NO,   M_NO,   NO, NO),
    E(0x7B00, 0x0000, NONE, TEXT_IRET,   M_NO,   M_NO,   NO, NO),
    E(0x7B08, 0x0000, NONE, TEXT_MSET,   M_NO,   M_NO,   NO, NO),
    E(0x7B09, 0x0000, NONE, TEXT_MRES,   M_NO,   M_NO,   NO, NO),
    E(0x7B0A, 0x0000, NONE, TEXT_MBIT,   M_NO,   M_NO,   NO, NO),
    E(0x7B0D, 0x00F0, WORD, TEXT_MREQ,   M_R,    M_NO,   C4, NO),
    E(0x7C00, 0x0003, NONE, TEXT_DI,     M_INTR, M_NO,   C0, NO),
    E(0x7C04, 0x0003, NONE, TEXT_EI,     M_INTR, M_NO,   C0, NO),
    E(0x7D00, 0x00F7, WORD, TEXT_LDCTL,  M_R,    M_CTL,  C4, C0),
    E(0x7D08, 0x00F7, WORD, TEXT_LDCTL,  M_CTL,  M_R,    C0, C4),
    E(0x7F00, 0x00FF, NONE, TEXT_SC,     M_IM8,  M_NO,   C0, C0),
    E(0x8C01, 0x00F0, BYTE, TEXT_LDCTLB, M_R,    M_CTL,  C4, C0),
    E(0x8C09, 0x00F0, BYTE, TEXT_LDCTLB, M_CTL,  M_R,    C0, C4),
    E(0x0C00, 0xC0F0, BYTE, TEXT_COMB,   M_GEND, M_NO,   C4, NO),
    E(0x0C01, 0xC0F0, BYTE, TEXT_CPB,    M_GENA, M_IM,   C4, NO),
    E(0x0C02, 0xC0F0, BYTE, TEXT_NEGB,   M_GEND, M_NO,   C4, NO), // NEGB/LDCTLB share same bit pattern
    E(0x0C04, 0xC0F0, BYTE, TEXT_TESTB,  M_GEND, M_NO,   C4, NO),
    E(0x0C05, 0xC0F0, BYTE, TEXT_LDB,    M_GENA, M_IM,   C4, NO),
    E(0x0C06, 0xC0F0, BYTE, TEXT_TSETB,  M_GEND, M_NO,   C4, NO),
    E(0x0C08, 0xC0F0, BYTE, TEXT_CLRB,   M_GEND, M_NO,   C4, NO), // CLRB/LDCTLB share smae bit pattern
    E(0x8D01, 0x00F0, NONE, TEXT_SETFLG, M_FLAG, M_NO,   C4, NO),
    E(0x8D03, 0x00F0, NONE, TEXT_RESFLG, M_FLAG, M_NO,   C4, NO),
    E(0x8D05, 0x00F0, NONE, TEXT_COMFLG, M_FLAG, M_NO,   C4, NO),
    E(0x8D07, 0x0000, NONE, TEXT_NOP,    M_NO,   M_NO,   NO, NO),
    E(0x9E00, 0x000F, NONE, TEXT_RET,    M_CC,   M_NO,   C0, NO),
    E(0xAE00, 0x00FF, BYTE, TEXT_TCCB,   M_CC,   M_R,    C0, C4),
    E(0xAE08, 0x00F0, BYTE, TEXT_TCCB,   M_R,    M_NO,   C4, NO),
    E(0xAF00, 0x00FF, WORD, TEXT_TCC,    M_CC,   M_R,    C0, C4),
    E(0xAF08, 0x00F0, WORD, TEXT_TCC,    M_R,    M_NO,   C4, NO),
    E(0xB000, 0x00F0, BYTE, TEXT_DAB,    M_R,    M_NO,   C4, NO),
    E(0xB100, 0x00F0, BYTE, TEXT_EXTSB,  M_DR,   M_NO,   C4, NO),
    E(0xB107, 0x00F0, LONG, TEXT_EXTSL,  M_DR,   M_NO,   C4, NO),
    E(0xB10A, 0x00F0, WORD, TEXT_EXTS,   M_DR,   M_NO,   C4, NO),
    E(0xB200, 0x00F2, BYTE, TEXT_RLB,    M_R,    M_QCNT, C4, C0),
    E(0xB201, 0x00F0, BYTE, TEXT_SLLB,   M_R,    M_SCNT, C4, NO),
    E(0xB201, 0x00F0, BYTE, TEXT_SRLB,   M_R,    M_NCNT, C4, NO),
    X(0xB203, 0x00F0, BYTE, TEXT_SDLB,   M_R,    M_WR,   C4, P8, M_NO, M_NO, P_0X00),
    E(0xB204, 0x00F2, BYTE, TEXT_RRB,    M_R,    M_QCNT, C4, C0),
    E(0xB208, 0x00F2, BYTE, TEXT_RLCB,   M_R,    M_QCNT, C4, C0),
    E(0xB209, 0x00F0, BYTE, TEXT_SLAB,   M_R,    M_SCNT, C4, NO),
    E(0xB209, 0x00F0, BYTE, TEXT_SRAB,   M_R,    M_NCNT, C4, NO),
    X(0xB20B, 0x00F0, BYTE, TEXT_SDAB,   M_R,    M_WR,   C4, P8, M_NO, M_NO, P_0X00),
    E(0xB20C, 0x00F2, BYTE, TEXT_RRCB,   M_R,    M_QCNT, C4, C0),
    E(0xB300, 0x00F2, WORD, TEXT_RL,     M_R,    M_QCNT, C4, C0),
    E(0xB301, 0x00F0, WORD, TEXT_SLL,    M_R,    M_SCNT, C4, NO),
    E(0xB301, 0x00F0, WORD, TEXT_SRL,    M_R,    M_NCNT, C4, NO),
    X(0xB303, 0x00F0, WORD, TEXT_SDL,    M_R,    M_WR,   C4, P8, M_NO, M_NO, P_0X00),
    E(0xB304, 0x00F2, WORD, TEXT_RR,     M_R,    M_QCNT, C4, C0),
    E(0xB305, 0x00F0, LONG, TEXT_SLLL,   M_R,    M_SCNT, C4, NO),
    E(0xB305, 0x00F0, LONG, TEXT_SRLL,   M_R,    M_NCNT, C4, NO),
    X(0xB307, 0x00F0, LONG, TEXT_SDLL,   M_R,    M_WR,   C4, P8, M_NO, M_NO, P_0X00),
    E(0xB308, 0x00F2, WORD, TEXT_RLC,    M_R,    M_QCNT, C4, C0),
    E(0xB309, 0x00F0, WORD, TEXT_SLA,    M_R,    M_SCNT, C4, NO),
    E(0xB309, 0x00F0, WORD, TEXT_SRA,    M_R,    M_NCNT, C4, NO),
    X(0xB30B, 0x00F0, WORD, TEXT_SDA,    M_R,    M_WR,   C4, P8, M_NO, M_NO, P_0X00),
    E(0xB30C, 0x00F2, WORD, TEXT_RRC,    M_R,    M_QCNT, C4, C0),
    E(0xB30D, 0x00F0, LONG, TEXT_SLAL,   M_R,    M_SCNT, C4, NO),
    E(0xB30D, 0x00F0, LONG, TEXT_SRAL,   M_R,    M_NCNT, C4, NO),
    X(0xB30F, 0x00F0, LONG, TEXT_SDAL,   M_R,    M_WR,   C4, P8, M_NO, M_NO, P_0X00),
    E(0xB400, 0x00FF, BYTE, TEXT_ADCB,   M_R,    M_R,    C0, C4),
    E(0xB500, 0x00FF, WORD, TEXT_ADC,    M_R,    M_R,    C0, C4),
    E(0xB600, 0x00FF, BYTE, TEXT_SBCB,   M_R,    M_R,    C0, C4),
    E(0xB700, 0x00FF, WORD, TEXT_SBC,    M_R,    M_R,    C0, C4),
    E(0xBC00, 0x00FF, BYTE, TEXT_RRDB,   M_R,    M_R,    C0, C4),
    E(0xBD00, 0x00FF, WORD, TEXT_LDK,    M_R,    M_BIT,  C4, C0),
    E(0xBE00, 0x00FF, BYTE, TEXT_RLDB,   M_R,    M_R,    C0, C4),
    E(0x3A04, 0x00F0, BYTE, TEXT_INB,    M_R,    M_IO,   C4, NO),
    E(0x3A05, 0x00F0, BYTE, TEXT_SINB,   M_R,    M_IO,   C4, NO),
    E(0x3A06, 0x00F0, BYTE, TEXT_OUTB,   M_IO,   M_R,    NO, C4),
    E(0x3A07, 0x00F0, BYTE, TEXT_SOUTB,  M_IO,   M_R,    NO, C4),
    E(0x3B04, 0x00F0, WORD, TEXT_IN,     M_R,    M_IO,   C4, NO),
    E(0x3B05, 0x00F0, WORD, TEXT_SIN,    M_R,    M_IO,   C4, NO),
    E(0x3B06, 0x00F0, WORD, TEXT_OUT,    M_IO,   M_R,    NO, C4),
    E(0x3B07, 0x00F0, WORD, TEXT_SOUT,   M_IO,   M_R,    NO, C4),
    X(0x3A00, 0x00F0, BYTE, TEXT_INIRB,  M_IR,   M_IRIO, P4, C4, M_WR, M_NO, P_0XX0),
    X(0x3A00, 0x00F0, BYTE, TEXT_INIB,   M_IR,   M_IRIO, P4, C4, M_WR, M_NO, P_0XX8),
    X(0x3A01, 0x00F0, BYTE, TEXT_SINIRB, M_IR,   M_IRIO, P4, C4, M_WR, M_NO, P_0XX0),
    X(0x3A01, 0x00F0, BYTE, TEXT_SINIB,  M_IR,   M_IRIO, P4, C4, M_WR, M_NO, P_0XX8),
    X(0x3A02, 0x00F0, BYTE, TEXT_OTIRB,  M_IRIO, M_IR,   P4, C4, M_WR, M_NO, P_0XX0),
    X(0x3A02, 0x00F0, BYTE, TEXT_OUTIB,  M_IRIO, M_IR,   P4, C4, M_WR, M_NO, P_0XX8),
    X(0x3A03, 0x00F0, BYTE, TEXT_SOTIRB, M_IRIO, M_IR,   P4, C4, M_WR, M_NO, P_0XX0),
    X(0x3A03, 0x00F0, BYTE, TEXT_SOUTIB, M_IRIO, M_IR,   P4, C4, M_WR, M_NO, P_0XX8),
    X(0x3A08, 0x00F0, BYTE, TEXT_INDRB,  M_IR,   M_IRIO, P4, C4, M_WR, M_NO, P_0XX0),
    X(0x3A08, 0x00F0, BYTE, TEXT_INDB,   M_IR,   M_IRIO, P4, C4, M_WR, M_NO, P_0XX8),
    X(0x3A09, 0x00F0, BYTE, TEXT_SINDRB, M_IR,   M_IRIO, P4, C4, M_WR, M_NO, P_0XX0),
    X(0x3A09, 0x00F0, BYTE, TEXT_SINDB,  M_IR,   M_IRIO, P4, C4, M_WR, M_NO, P_0XX8),
    X(0x3A0A, 0x00F0, BYTE, TEXT_OTDRB,  M_IRIO, M_IR,   P4, C4, M_WR, M_NO, P_0XX0),
    X(0x3A0A, 0x00F0, BYTE, TEXT_OUTDB,  M_IRIO, M_IR,   P4, C4, M_WR, M_NO, P_0XX8),
    X(0x3A0B, 0x00F0, BYTE, TEXT_SOTDRB, M_IRIO, M_IR,   P4, C4, M_WR, M_NO, P_0XX0),
    X(0x3A0B, 0x00F0, BYTE, TEXT_SOUTDB, M_IRIO, M_IR,   P4, C4, M_WR, M_NO, P_0XX8),
    X(0x3B00, 0x00F0, WORD, TEXT_INIR,   M_IR,   M_IRIO, P4, C4, M_WR, M_NO, P_0XX0),
    X(0x3B00, 0x00F0, WORD, TEXT_INI,    M_IR,   M_IRIO, P4, C4, M_WR, M_NO, P_0XX8),
    X(0x3B01, 0x00F0, WORD, TEXT_SINIR,  M_IR,   M_IRIO, P4, C4, M_WR, M_NO, P_0XX0),
    X(0x3B01, 0x00F0, WORD, TEXT_SINI,   M_IR,   M_IRIO, P4, C4, M_WR, M_NO, P_0XX8),
    X(0x3B02, 0x00F0, WORD, TEXT_OTIR,   M_IRIO, M_IR,   P4, C4, M_WR, M_NO, P_0XX0),
    X(0x3B02, 0x00F0, WORD, TEXT_OUTI,   M_IRIO, M_IR,   P4, C4, M_WR, M_NO, P_0XX8),
    X(0x3B03, 0x00F0, WORD, TEXT_SOTIR,  M_IRIO, M_IR,   P4, C4, M_WR, M_NO, P_0XX0),
    X(0x3B03, 0x00F0, WORD, TEXT_SOUTI,  M_IRIO, M_IR,   P4, C4, M_WR, M_NO, P_0XX8),
    X(0x3B08, 0x00F0, WORD, TEXT_INDR,   M_IR,   M_IRIO, P4, C4, M_WR, M_NO, P_0XX0),
    X(0x3B08, 0x00F0, WORD, TEXT_IND,    M_IR,   M_IRIO, P4, C4, M_WR, M_NO, P_0XX8),
    X(0x3B09, 0x00F0, WORD, TEXT_SINDR,  M_IR,   M_IRIO, P4, C4, M_WR, M_NO, P_0XX0),
    X(0x3B09, 0x00F0, WORD, TEXT_SIND,   M_IR,   M_IRIO, P4, C4, M_WR, M_NO, P_0XX8),
    X(0x3B0A, 0x00F0, WORD, TEXT_OTDR,   M_IRIO, M_IR,   P4, C4, M_WR, M_NO, P_0XX0),
    X(0x3B0A, 0x00F0, WORD, TEXT_OUTD,   M_IRIO, M_IR,   P4, C4, M_WR, M_NO, P_0XX8),
    X(0x3B0B, 0x00F0, WORD, TEXT_SOTDR,  M_IRIO, M_IR,   P4, C4, M_WR, M_NO, P_0XX0),
    X(0x3B0B, 0x00F0, WORD, TEXT_SOUTD,  M_IRIO, M_IR,   P4, C4, M_WR, M_NO, P_0XX8),
    X(0xB800, 0x00F0, BYTE, TEXT_TRIB,   M_IR,   M_IR,   C4, P4, M_WR, M_NO, P_0XX0),
    X(0xB802, 0x00F0, BYTE, TEXT_TRTIB,  M_IR,   M_IR,   C4, P4, M_WR, M_NO, P_0XX0),
    X(0xB804, 0x00F0, BYTE, TEXT_TRIRB,  M_IR,   M_IR,   C4, P4, M_WR, M_NO, P_0XX0),
    X(0xB806, 0x00F0, BYTE, TEXT_TRTIRB, M_IR,   M_IR,   C4, P4, M_WR, M_NO, P_0XXE),
    X(0xB808, 0x00F0, BYTE, TEXT_TRDB,   M_IR,   M_IR,   C4, P4, M_WR, M_NO, P_0XX0),
    X(0xB80A, 0x00F0, BYTE, TEXT_TRTDB,  M_IR,   M_IR,   C4, P4, M_WR, M_NO, P_0XX0),
    X(0xB80C, 0x00F0, BYTE, TEXT_TRDRB,  M_IR,   M_IR,   C4, P4, M_WR, M_NO, P_0XX0),
    X(0xB80E, 0x00F0, BYTE, TEXT_TRTDRB, M_IR,   M_IR,   C4, P4, M_WR, M_NO, P_0XXE),
    X(0xBA00, 0x00F0, BYTE, TEXT_CPIB,   M_R,    M_IR,   P4, C4, M_WR, M_CC, P_0XXX),
    X(0xBA01, 0x00F0, BYTE, TEXT_LDIRB,  M_IR,   M_IR,   P4, C4, M_WR, M_NO, P_0XX0),
    X(0xBA01, 0x00F0, BYTE, TEXT_LDIB,   M_IR,   M_IR,   P4, C4, M_WR, M_NO, P_0XX8),
    X(0xBA02, 0x00F0, BYTE, TEXT_CPSIB,  M_IR,   M_IR,   P4, C4, M_WR, M_CC, P_0XXX),
    X(0xBA04, 0x00F0, BYTE, TEXT_CPIRB,  M_R,    M_IR,   P4, C4, M_WR, M_CC, P_0XXX),
    X(0xBA06, 0x00F0, BYTE, TEXT_CPSIRB, M_IR,   M_IR,   P4, C4, M_WR, M_CC, P_0XXX),
    X(0xBA08, 0x00F0, BYTE, TEXT_CPDB,   M_R,    M_IR,   P4, C4, M_WR, M_CC, P_0XXX),
    X(0xBA09, 0x00F0, BYTE, TEXT_LDDRB,  M_IR,   M_IR,   P4, C4, M_WR, M_NO, P_0XX0),
    X(0xBA09, 0x00F0, BYTE, TEXT_LDDB,   M_IR,   M_IR,   P4, C4, M_WR, M_NO, P_0XX8),
    X(0xBA0A, 0x00F0, BYTE, TEXT_CPSDB,  M_IR,   M_IR,   P4, C4, M_WR, M_CC, P_0XXX),
    X(0xBA0C, 0x00F0, BYTE, TEXT_CPDRB,  M_R,    M_IR,   P4, C4, M_WR, M_CC, P_0XXX),
    X(0xBA0E, 0x00F0, BYTE, TEXT_CPSDRB, M_IR,   M_IR,   P4, C4, M_WR, M_CC, P_0XXX),
    X(0xBB00, 0x00F0, WORD, TEXT_CPI,    M_R,    M_IR,   P4, C4, M_WR, M_CC, P_0XXX),
    X(0xBB01, 0x00F0, WORD, TEXT_LDIR,   M_IR,   M_IR,   P4, C4, M_WR, M_NO, P_0XX0),
    X(0xBB01, 0x00F0, WORD, TEXT_LDI,    M_IR,   M_IR,   P4, C4, M_WR, M_NO, P_0XX8),
    X(0xBB02, 0x00F0, WORD, TEXT_CPSI,   M_IR,   M_IR,   P4, C4, M_WR, M_CC, P_0XXX),
    X(0xBB04, 0x00F0, WORD, TEXT_CPIR,   M_R,    M_IR,   P4, C4, M_WR, M_CC, P_0XXX),
    X(0xBB06, 0x00F0, WORD, TEXT_CPSIR,  M_IR,   M_IR,   P4, C4, M_WR, M_CC, P_0XXX),
    X(0xBB08, 0x00F0, WORD, TEXT_CPD,    M_R,    M_IR,   P4, C4, M_WR, M_CC, P_0XXX),
    X(0xBB09, 0x00F0, WORD, TEXT_LDDR,   M_IR,   M_IR,   P4, C4, M_WR, M_NO, P_0XX0),
    X(0xBB09, 0x00F0, WORD, TEXT_LDD,    M_IR,   M_IR,   P4, C4, M_WR, M_NO, P_0XX8),
    X(0xBB0A, 0x00F0, WORD, TEXT_CPSD,   M_IR,   M_IR,   P4, C4, M_WR, M_CC, P_0XXX),
    X(0xBB0C, 0x00F0, WORD, TEXT_CPDR,   M_R,    M_IR,   P4, C4, M_WR, M_CC, P_0XXX),
    X(0xBB0E, 0x00F0, WORD, TEXT_CPSDR,  M_IR,   M_IR,   P4, C4, M_WR, M_CC, P_0XXX),
    E(0x0000, 0xC0FF, BYTE, TEXT_ADDB,   M_R,    M_GENI, C0, C4),
    E(0x0100, 0xC0FF, WORD, TEXT_ADD,    M_R,    M_GENI, C0, C4),
    E(0x0200, 0xC0FF, BYTE, TEXT_SUBB,   M_R,    M_GENI, C0, C4),
    E(0x0300, 0xC0FF, WORD, TEXT_SUB,    M_R,    M_GENI, C0, C4),
    E(0x0400, 0xC0FF, BYTE, TEXT_ORB,    M_R,    M_GENI, C0, C4),
    E(0x0500, 0xC0FF, WORD, TEXT_OR,     M_R,    M_GENI, C0, C4),
    E(0x0600, 0xC0FF, BYTE, TEXT_ANDB,   M_R,    M_GENI, C0, C4),
    E(0x0700, 0xC0FF, WORD, TEXT_AND,    M_R,    M_GENI, C0, C4),
    E(0x0800, 0xC0FF, BYTE, TEXT_XORB,   M_R,    M_GENI, C0, C4),
    E(0x0900, 0xC0FF, WORD, TEXT_XOR,    M_R,    M_GENI, C0, C4),
    E(0x0A00, 0xC0FF, BYTE, TEXT_CPB,    M_R,    M_GENI, C0, C4),
    E(0x0B00, 0xC0FF, WORD, TEXT_CP,     M_R,    M_GENI, C0, C4),
    E(0x0D00, 0xC0F0, WORD, TEXT_COM,    M_GEND, M_NO,   C4, NO),
    E(0x0D01, 0xC0F0, WORD, TEXT_CP,     M_GENA, M_IM,   C4, NO),
    E(0x0D02, 0xC0F0, WORD, TEXT_NEG,    M_GEND, M_NO,   C4, NO),
    E(0x0D04, 0xC0F0, WORD, TEXT_TEST,   M_GEND, M_NO,   C4, NO),
    E(0x0D05, 0xC0F0, WORD, TEXT_LD,     M_GENA, M_IM,   C4, NO),
    E(0x0D06, 0xC0F0, WORD, TEXT_TSET,   M_GEND, M_NO,   C4, NO),
    E(0x0D08, 0xC0F0, WORD, TEXT_CLR,    M_GEND, M_NO,   C4, NO),
    E(0x0D09, 0xC0F0, WORD, TEXT_PUSH,   M_IR,   M_IM,   C4, NO),
    E(0x1000, 0xC0FF, LONG, TEXT_CPL,    M_R,    M_GENI, C0, C4),
    E(0x1100, 0xC0FF, LONG, TEXT_PUSHL,  M_IR,   M_GEND, C4, C0),
    E(0x1200, 0xC0FF, LONG, TEXT_SUBL,   M_R,    M_GENI, C0, C4),
    E(0x1300, 0xC0FF, WORD, TEXT_PUSH,   M_IR,   M_GEND, C4, C0),
    E(0x1400, 0xC0FF, LONG, TEXT_LDL,    M_R,    M_GENI, C0, C4),
    E(0x1500, 0xC0FF, LONG, TEXT_POPL,   M_GEND, M_IR,   C0, C4),
    E(0x1600, 0xC0FF, LONG, TEXT_ADDL,   M_R,    M_GENI, C0, C4),
    E(0x1700, 0xC0FF, WORD, TEXT_POP,    M_GEND, M_IR,   C0, C4),
    E(0x1800, 0xC0FF, LONG, TEXT_MULTL,  M_DR,   M_GENI, C0, C4),
    E(0x1900, 0xC0FF, WORD, TEXT_MULT,   M_DR,   M_GENI, C0, C4),
    E(0x1A00, 0xC0FF, LONG, TEXT_DIVL,   M_DR,   M_GENI, C0, C4),
    E(0x1B00, 0xC0FF, WORD, TEXT_DIV,    M_DR,   M_GENI, C0, C4),
    X(0x1C01, 0xC0F0, WORD, TEXT_LDM,    M_R,    M_GENA, P8, C4, M_CNT, M_NO, P_0X0X),
    E(0x1C08, 0xC0F0, LONG, TEXT_TESTL,  M_GEND, M_NO,   C4, NO),
    X(0x1C09, 0xC0F0, WORD, TEXT_LDM,    M_GENA, M_R,    C4, P8, M_CNT, M_NO, P_0X0X),
    E(0x1D00, 0xC0FF, LONG, TEXT_LDL,    M_GENA, M_R,    C4, C0),
    E(0x1E00, 0xC0FF, NONE, TEXT_JP,     M_CC,   M_GENA, C0, C4),
    E(0x1E08, 0xC0F0, NONE, TEXT_JP,     M_GENA, M_NO,   C4, NO),
    E(0x1F00, 0xC0FF, NONE, TEXT_CALL,   M_GENA, M_NO,   C4, NO),
    E(0x2000, 0xC0FF, BYTE, TEXT_LDB,    M_R,    M_GENI, C0, C4),
    E(0x2100, 0xC0FF, WORD, TEXT_LD,     M_R,    M_GENI, C0, C4),
    X(0x2200, 0x000F, BYTE, TEXT_RESB,   M_R,    M_WR07, P8, C0, M_NO,  M_NO, P_0X00),
    E(0x2200, 0xC0FF, BYTE, TEXT_RESB,   M_GEND, M_BIT,  C4, C0),
    X(0x2300, 0x000F, WORD, TEXT_RES,    M_R,    M_R,    P8, C0, M_NO,  M_NO, P_0X00),
    E(0x2300, 0xC0FF, WORD, TEXT_RES,    M_GEND, M_BIT,  C4, C0),
    X(0x2400, 0x000F, BYTE, TEXT_SETB,   M_R,    M_WR07, P8, C0, M_NO,  M_NO, P_0X00),
    E(0x2400, 0xC0FF, BYTE, TEXT_SETB,   M_GEND, M_BIT,  C4, C0),
    X(0x2500, 0x000F, WORD, TEXT_SET,    M_R,    M_R,    P8, C0, M_NO,  M_NO, P_0X00),
    E(0x2500, 0xC0FF, WORD, TEXT_SET,    M_GEND, M_BIT,  C4, C0),
    X(0x2600, 0x000F, BYTE, TEXT_BITB,   M_R,    M_WR07, P8, C0, M_NO,  M_NO, P_0X00),
    E(0x2600, 0xC0FF, BYTE, TEXT_BITB,   M_GEND, M_BIT,  C4, C0),
    X(0x2700, 0x000F, WORD, TEXT_BIT,    M_R,    M_R,    P8, C0, M_NO,  M_NO, P_0X00),
    E(0x2700, 0xC0FF, WORD, TEXT_BIT,    M_GEND, M_BIT,  C4, C0),
    E(0x2800, 0xC0FF, BYTE, TEXT_INCB,   M_GEND, M_CNT,  C4, C0),
    E(0x2900, 0xC0FF, WORD, TEXT_INC,    M_GEND, M_CNT,  C4, C0),
    E(0x2A00, 0xC0FF, BYTE, TEXT_DECB,   M_GEND, M_CNT,  C4, C0),
    E(0x2B00, 0xC0FF, WORD, TEXT_DEC,    M_GEND, M_CNT,  C4, C0),
    E(0x2C00, 0xC0FF, BYTE, TEXT_EXB,    M_R,    M_GEND, C0, C4),
    E(0x2D00, 0xC0FF, WORD, TEXT_EX,     M_R,    M_GEND, C0, C4),
    E(0x2E00, 0xC0FF, BYTE, TEXT_LDB,    M_GENA, M_R,    C4, C0),
    E(0x2F00, 0xC0FF, WORD, TEXT_LD,     M_GENA, M_R,    C4, C0),
    E(0x3900, 0xC0F0, WORD, TEXT_LDPS,   M_GENA, M_NO,   C4, NO),
};
// clang-format on

static bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_R)
        return table == M_GENI || table == M_GEND || table == M_WR07 || table == M_WR ||
               table == M_DR;
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
    if (opr == M_NO)
        return table == M_CC || table == M_INTR || table == M_FLAG;
    return false;
}

static bool acceptModes(Entry::Flags flags, const Entry *entry) {
    auto table = entry->flags();
    return acceptMode(flags.dstMode(), table.dstMode()) &&
           acceptMode(flags.srcMode(), table.srcMode()) &&
           acceptMode(flags.ex1Mode(), table.ex1Mode()) &&
           acceptMode(flags.ex2Mode(), table.ex2Mode());
}

Error TableZ8000::searchName(InsnZ8000 &insn) const {
    uint8_t count = 0;
    auto entry = TableBase::searchName<Entry, Entry::Flags>(
            insn.name(), insn.flags(), ARRAY_RANGE(Z8000_TABLE), acceptModes, count);
    if (entry) {
        insn.setOpCode(entry->opCode());
        insn.setFlags(entry->flags());
        return setOK();
    }
    return setError(count == 0 ? UNKNOWN_INSTRUCTION : OPERAND_NOT_ALLOWED);
}

static Config::opcode_t tableCode(Config::opcode_t opCode, const Entry *entry) {
    return opCode & ~entry->flags().codeMask();
}

static bool matchPostWord(const InsnZ8000 &insn) {
    return (insn.post() & insn.postMask()) == insn.postVal();
}

const Entry *TableZ8000::searchOpCode(
        InsnZ8000 &insn, DisMemory &memory, const Entry *table, const Entry *end) const {
    for (auto entry = table;
            entry < end && (entry = TableBase::searchCode<Entry, Config::opcode_t>(
                                    insn.opCode(), entry, end, tableCode)) != nullptr;
            entry++) {
        insn.setFlags(entry->flags());
        if (insn.hasPost()) {
            if (insn.length() < 4) {
                insn.readPost(memory);
                if (setError(insn))
                    return nullptr;
            }
            if (!matchPostWord(insn))
                continue;
        }
        insn.setName_P(entry->name());
        return entry;
    }
    return nullptr;
}

Error TableZ8000::searchOpCode(InsnZ8000 &insn, DisMemory &memory) const {
    auto entry = searchOpCode(insn, memory, ARRAY_RANGE(Z8000_TABLE));
    return setError(entry ? OK : UNKNOWN_INSTRUCTION);
}

Error TableZ8000::searchOpCodeAlias(InsnZ8000 &insn, DisMemory &memory) const {
    auto entry = searchOpCode(insn, memory, ARRAY_RANGE(Z8000_TABLE));
    if (entry) {
        entry++;
        insn.setFlags(entry->flags());
        insn.setName_P(entry->name());
    }
    return setError(entry ? OK : UNKNOWN_INSTRUCTION);
}

TableZ8000::TableZ8000() : _cpuType(Z8001) {}

const char *TableZ8000::listCpu() const {
    return TEXT_CPU_LIST;
}

const char *TableZ8000::getCpu() const {
    return _cpuType == Z8001 ? TEXT_CPU_Z8001 : TEXT_CPU_Z8002;
}

bool TableZ8000::setCpu(const char *cpu) {
    if (strcasecmp_P(cpu, TEXT_CPU_Z8001) == 0) {
        _cpuType = Z8001;
        return true;
    }
    if (strcasecmp_P(cpu, TEXT_CPU_Z8002) == 0) {
        _cpuType = Z8002;
        return true;
    }
    return false;
}

AddressWidth TableZ8000::addressWidth() const {
    return segmentedModel() ? ADDRESS_24BIT : ADDRESS_16BIT;
}

bool TableZ8000::segmentedModel() const {
    return _cpuType == Z8001;
}

class TableZ8000 TableZ8000;

}  // namespace z8000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

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

#include "config_z8000.h"
#include "entry_z8000.h"
#include "table_z8000.h"
#include "text_z8000.h"

#include <string.h>

namespace libasm {
namespace z8000 {

#define E(_opc, _cm, _sz, _name, _dst, _src, _dst_mf, _src_mf)  \
    {                                                           \
        _opc,                                                   \
        Entry::_flags(Entry::_opr(_dst, MF_##_dst_mf),          \
                      Entry::_opr(_src, MF_##_src_mf),          \
                      Entry::_ext(M_NO, M_NO, P_NO),            \
                      Entry::_size(CM_##_cm, SZ_##_sz)),        \
        TEXT_##_name                                            \
    },
#define X(_opc, _cm, _sz, _name, _dst, _src, _dst_mf, _src_mf, _ex1, _ex2, _post) \
    {                                                                   \
        _opc,                                                           \
        Entry::_flags(Entry::_opr(_dst, MF_##_dst_mf),                  \
                      Entry::_opr(_src, MF_##_src_mf),                  \
                      Entry::_ext(_ex1, _ex2, _post),                   \
                      Entry::_size(CM_##_cm, SZ_##_sz)),                \
        TEXT_##_name                                                    \
    },

static const Entry Z8000_TABLE[] PROGMEM = {
    E(0xC000, 0x0FFF, BYTE, LDB,    M_R,    M_IM8,  C8, C0)
    E(0xD000, 0x0FFF, NONE, CALR,   M_RA12, M_NO,   C0, NO)
    E(0xE000, 0x0FFF, NONE, JR,     M_CC,   M_RA8,  C8, C0)
    E(0xE800, 0x00FF, NONE, JR,     M_RA8,  M_NO,   C0, NO)
    E(0xF000, 0x0F7F, BYTE, DBJNZ,  M_R,    M_RA7,  C8, C0)
    E(0xF080, 0x0F7F, WORD, DJNZ,   M_R,    M_RA7,  C8, C0)
    E(0x3000, 0x000F, BYTE, LDRB,   M_R,    M_RA,   C0, NO)
    E(0x3000, 0x00FF, BYTE, LDB,    M_R,    M_BA,   C0, C4)
    E(0x3100, 0x000F, WORD, LDR,    M_R,    M_RA,   C0, NO)
    E(0x3100, 0x00FF, WORD, LD,     M_R,    M_BA,   C0, C4)
    E(0x3200, 0x000F, BYTE, LDRB,   M_RA,   M_R,    NO, C0)
    E(0x3200, 0x00FF, BYTE, LDB,    M_BA,   M_R,    C4, C0)
    E(0x3300, 0x000F, WORD, LDR,    M_RA,   M_R,    NO, C0)
    E(0x3300, 0x00FF, WORD, LD,     M_BA,   M_R,    C4, C0)
    E(0x3400, 0x000F, ADDR, LDAR,   M_R,    M_RA,   C0, NO)
    E(0x3400, 0x00FF, ADDR, LDA,    M_R,    M_BA,   C0, C4)
    E(0x3500, 0x000F, LONG, LDRL,   M_R,    M_RA,   C0, NO)
    E(0x3500, 0x00FF, LONG, LDL,    M_R,    M_BA,   C0, C4)
    E(0x3700, 0x000F, LONG, LDRL,   M_RA,   M_R,    NO, C0)
    E(0x3700, 0x00FF, LONG, LDL,    M_BA,   M_R,    C4, C0)
    E(0x3C00, 0x00FF, BYTE, INB,    M_R,    M_IRIO, C0, C4)
    E(0x3D00, 0x00FF, WORD, IN,     M_R,    M_IRIO, C0, C4)
    E(0x3E00, 0x00FF, BYTE, OUTB,   M_IRIO, M_R,    C4, C0)
    E(0x3F00, 0x00FF, WORD, OUT,    M_IRIO, M_R,    C4, C0)
    X(0x7000, 0x00FF, BYTE, LDB,    M_R,    M_BX,   C0, C4, M_NO, M_NO, P_0X00)
    X(0x7100, 0x00FF, WORD, LD,     M_R,    M_BX,   C0, C4, M_NO, M_NO, P_0X00)
    X(0x7200, 0x00FF, BYTE, LDB,    M_BX,   M_R,    C4, C0, M_NO, M_NO, P_0X00)
    X(0x7300, 0x00FF, WORD, LD,     M_BX,   M_R,    C4, C0, M_NO, M_NO, P_0X00)
    X(0x7400, 0x00FF, ADDR, LDA,    M_R,    M_BX,   C0, C4, M_NO, M_NO, P_0X00)
    X(0x7500, 0x00FF, LONG, LDL,    M_R,    M_BX,   C0, C4, M_NO, M_NO, P_0X00)
    E(0x7600, 0x00FF, ADDR, LDA,    M_R,    M_GENA, C0, C4)
    X(0x7700, 0x00FF, LONG, LDL,    M_BX,   M_R,    C4, C0, M_NO, M_NO, P_0X00)
    E(0x7A00, 0x0000, NONE, HALT,   M_NO,   M_NO,   NO, NO)
    E(0x7B00, 0x0000, NONE, IRET,   M_NO,   M_NO,   NO, NO)
    E(0x7B08, 0x0000, NONE, MSET,   M_NO,   M_NO,   NO, NO)
    E(0x7B09, 0x0000, NONE, MRES,   M_NO,   M_NO,   NO, NO)
    E(0x7B0A, 0x0000, NONE, MBIT,   M_NO,   M_NO,   NO, NO)
    E(0x7B0D, 0x00F0, WORD, MREQ,   M_R,    M_NO,   C4, NO)
    E(0x7C00, 0x0003, NONE, DI,     M_INTT, M_NO,   C0, NO)
    E(0x7C04, 0x0003, NONE, EI,     M_INTT, M_NO,   C0, NO)
    E(0x7D00, 0x00F7, WORD, LDCTL,  M_R,    M_CTL,  C4, C0)
    E(0x7D08, 0x00F7, WORD, LDCTL,  M_CTL,  M_R,    C0, C4)
    E(0x7F00, 0x00FF, NONE, SC,     M_IM8,  M_NO,   C0, C0)
    E(0x8C01, 0x00F0, BYTE, LDCTLB, M_R,    M_CTL,  C4, C0)
    E(0x8C09, 0x00F0, BYTE, LDCTLB, M_CTL,  M_R,    C0, C4)
    E(0x0C00, 0xC0F0, BYTE, COMB,   M_GEND, M_NO,   C4, NO)
    E(0x0C01, 0xC0F0, BYTE, CPB,    M_GENA, M_IM,   C4, NO)
    E(0x0C02, 0xC0F0, BYTE, NEGB,   M_GEND, M_NO,   C4, NO) // NEGB/LDCTLB share same bit pattern
    E(0x0C04, 0xC0F0, BYTE, TESTB,  M_GEND, M_NO,   C4, NO)
    E(0x0C05, 0xC0F0, BYTE, LDB,    M_GENA, M_IM,   C4, NO)
    E(0x0C06, 0xC0F0, BYTE, TSETB,  M_GEND, M_NO,   C4, NO)
    E(0x0C08, 0xC0F0, BYTE, CLRB,   M_GEND, M_NO,   C4, NO) // CLRB/LDCTLB share smae bit pattern
    E(0x8D01, 0x00F0, NONE, SETFLG, M_FLAG, M_NO,   C4, NO)
    E(0x8D03, 0x00F0, NONE, RESFLG, M_FLAG, M_NO,   C4, NO)
    E(0x8D05, 0x00F0, NONE, COMFLG, M_FLAG, M_NO,   C4, NO)
    E(0x8D07, 0x0000, NONE, NOP,    M_NO,   M_NO,   NO, NO)
    E(0x9E00, 0x000F, NONE, RET,    M_CC,   M_NO,   C0, NO)
    E(0xAE00, 0x00FF, BYTE, TCCB,   M_CC,   M_R,    C0, C4)
    E(0xAE08, 0x00F0, BYTE, TCCB,   M_R,    M_NO,   C4, NO)
    E(0xAF00, 0x00FF, WORD, TCC,    M_CC,   M_R,    C0, C4)
    E(0xAF08, 0x00F0, WORD, TCC,    M_R,    M_NO,   C4, NO)
    E(0xB000, 0x00F0, BYTE, DAB,    M_R,    M_NO,   C4, NO)
    E(0xB100, 0x00F0, BYTE, EXTSB,  M_DR,   M_NO,   C4, NO)
    E(0xB107, 0x00F0, LONG, EXTSL,  M_DR,   M_NO,   C4, NO)
    E(0xB10A, 0x00F0, WORD, EXTS,   M_DR,   M_NO,   C4, NO)
    E(0xB200, 0x00F2, BYTE, RLB,    M_R,    M_QCNT, C4, C0)
    E(0xB201, 0x00F0, BYTE, SLLB,   M_R,    M_SCNT, C4, NO)
    E(0xB201, 0x00F0, BYTE, SRLB,   M_R,    M_NCNT, C4, NO)
    X(0xB203, 0x00F0, BYTE, SDLB,   M_R,    M_WR,   C4, P8, M_NO, M_NO, P_0X00)
    E(0xB204, 0x00F2, BYTE, RRB,    M_R,    M_QCNT, C4, C0)
    E(0xB208, 0x00F2, BYTE, RLCB,   M_R,    M_QCNT, C4, C0)
    E(0xB209, 0x00F0, BYTE, SLAB,   M_R,    M_SCNT, C4, NO)
    E(0xB209, 0x00F0, BYTE, SRAB,   M_R,    M_NCNT, C4, NO)
    X(0xB20B, 0x00F0, BYTE, SDAB,   M_R,    M_WR,   C4, P8, M_NO, M_NO, P_0X00)
    E(0xB20C, 0x00F2, BYTE, RRCB,   M_R,    M_QCNT, C4, C0)
    E(0xB300, 0x00F2, WORD, RL,     M_R,    M_QCNT, C4, C0)
    E(0xB301, 0x00F0, WORD, SLL,    M_R,    M_SCNT, C4, NO)
    E(0xB301, 0x00F0, WORD, SRL,    M_R,    M_NCNT, C4, NO)
    X(0xB303, 0x00F0, WORD, SDL,    M_R,    M_WR,   C4, P8, M_NO, M_NO, P_0X00)
    E(0xB304, 0x00F2, WORD, RR,     M_R,    M_QCNT, C4, C0)
    E(0xB305, 0x00F0, LONG, SLLL,   M_R,    M_SCNT, C4, NO)
    E(0xB305, 0x00F0, LONG, SRLL,   M_R,    M_NCNT, C4, NO)
    X(0xB307, 0x00F0, LONG, SDLL,   M_R,    M_WR,   C4, P8, M_NO, M_NO, P_0X00)
    E(0xB308, 0x00F2, WORD, RLC,    M_R,    M_QCNT, C4, C0)
    E(0xB309, 0x00F0, WORD, SLA,    M_R,    M_SCNT, C4, NO)
    E(0xB309, 0x00F0, WORD, SRA,    M_R,    M_NCNT, C4, NO)
    X(0xB30B, 0x00F0, WORD, SDA,    M_R,    M_WR,   C4, P8, M_NO, M_NO, P_0X00)
    E(0xB30C, 0x00F2, WORD, RRC,    M_R,    M_QCNT, C4, C0)
    E(0xB30D, 0x00F0, LONG, SLAL,   M_R,    M_SCNT, C4, NO)
    E(0xB30D, 0x00F0, LONG, SRAL,   M_R,    M_NCNT, C4, NO)
    X(0xB30F, 0x00F0, LONG, SDAL,   M_R,    M_WR,   C4, P8, M_NO, M_NO, P_0X00)
    E(0xB400, 0x00FF, BYTE, ADCB,   M_R,    M_R,    C0, C4)
    E(0xB500, 0x00FF, WORD, ADC,    M_R,    M_R,    C0, C4)
    E(0xB600, 0x00FF, BYTE, SBCB,   M_R,    M_R,    C0, C4)
    E(0xB700, 0x00FF, WORD, SBC,    M_R,    M_R,    C0, C4)
    E(0xBC00, 0x00FF, BYTE, RRDB,   M_R,    M_R,    C0, C4)
    E(0xBD00, 0x00FF, WORD, LDK,    M_R,    M_BIT,  C4, C0)
    E(0xBE00, 0x00FF, BYTE, RLDB,   M_R,    M_R,    C0, C4)
    E(0x3A04, 0x00F0, BYTE, INB,    M_R,    M_IO,   C4, NO)
    E(0x3A05, 0x00F0, BYTE, SINB,   M_R,    M_IO,   C4, NO)
    E(0x3A06, 0x00F0, BYTE, OUTB,   M_IO,   M_R,    NO, C4)
    E(0x3A07, 0x00F0, BYTE, SOUTB,  M_IO,   M_R,    NO, C4)
    E(0x3B04, 0x00F0, WORD, IN,     M_R,    M_IO,   C4, NO)
    E(0x3B05, 0x00F0, WORD, SIN,    M_R,    M_IO,   C4, NO)
    E(0x3B06, 0x00F0, WORD, OUT,    M_IO,   M_R,    NO, C4)
    E(0x3B07, 0x00F0, WORD, SOUT,   M_IO,   M_R,    NO, C4)
    X(0x3A00, 0x00F0, BYTE, INIRB,  M_IR,   M_IRIO, P4, C4, M_WR, M_NO, P_0XX0)
    X(0x3A00, 0x00F0, BYTE, INIB,   M_IR,   M_IRIO, P4, C4, M_WR, M_NO, P_0XX8)
    X(0x3A01, 0x00F0, BYTE, SINIRB, M_IR,   M_IRIO, P4, C4, M_WR, M_NO, P_0XX0)
    X(0x3A01, 0x00F0, BYTE, SINIB,  M_IR,   M_IRIO, P4, C4, M_WR, M_NO, P_0XX8)
    X(0x3A02, 0x00F0, BYTE, OTIRB,  M_IRIO, M_IR,   P4, C4, M_WR, M_NO, P_0XX0)
    X(0x3A02, 0x00F0, BYTE, OUTIB,  M_IRIO, M_IR,   P4, C4, M_WR, M_NO, P_0XX8)
    X(0x3A03, 0x00F0, BYTE, SOTIRB, M_IRIO, M_IR,   P4, C4, M_WR, M_NO, P_0XX0)
    X(0x3A03, 0x00F0, BYTE, SOUTIB, M_IRIO, M_IR,   P4, C4, M_WR, M_NO, P_0XX8)
    X(0x3A08, 0x00F0, BYTE, INDRB,  M_IR,   M_IRIO, P4, C4, M_WR, M_NO, P_0XX0)
    X(0x3A08, 0x00F0, BYTE, INDB,   M_IR,   M_IRIO, P4, C4, M_WR, M_NO, P_0XX8)
    X(0x3A09, 0x00F0, BYTE, SINDRB, M_IR,   M_IRIO, P4, C4, M_WR, M_NO, P_0XX0)
    X(0x3A09, 0x00F0, BYTE, SINDB,  M_IR,   M_IRIO, P4, C4, M_WR, M_NO, P_0XX8)
    X(0x3A0A, 0x00F0, BYTE, OTDRB,  M_IRIO, M_IR,   P4, C4, M_WR, M_NO, P_0XX0)
    X(0x3A0A, 0x00F0, BYTE, OUTDB,  M_IRIO, M_IR,   P4, C4, M_WR, M_NO, P_0XX8)
    X(0x3A0B, 0x00F0, BYTE, SOTDRB, M_IRIO, M_IR,   P4, C4, M_WR, M_NO, P_0XX0)
    X(0x3A0B, 0x00F0, BYTE, SOUTDB, M_IRIO, M_IR,   P4, C4, M_WR, M_NO, P_0XX8)
    X(0x3B00, 0x00F0, WORD, INIR,   M_IR,   M_IRIO, P4, C4, M_WR, M_NO, P_0XX0)
    X(0x3B00, 0x00F0, WORD, INI,    M_IR,   M_IRIO, P4, C4, M_WR, M_NO, P_0XX8)
    X(0x3B01, 0x00F0, WORD, SINIR,  M_IR,   M_IRIO, P4, C4, M_WR, M_NO, P_0XX0)
    X(0x3B01, 0x00F0, WORD, SINI,   M_IR,   M_IRIO, P4, C4, M_WR, M_NO, P_0XX8)
    X(0x3B02, 0x00F0, WORD, OTIR,   M_IRIO, M_IR,   P4, C4, M_WR, M_NO, P_0XX0)
    X(0x3B02, 0x00F0, WORD, OUTI,   M_IRIO, M_IR,   P4, C4, M_WR, M_NO, P_0XX8)
    X(0x3B03, 0x00F0, WORD, SOTIR,  M_IRIO, M_IR,   P4, C4, M_WR, M_NO, P_0XX0)
    X(0x3B03, 0x00F0, WORD, SOUTI,  M_IRIO, M_IR,   P4, C4, M_WR, M_NO, P_0XX8)
    X(0x3B08, 0x00F0, WORD, INDR,   M_IR,   M_IRIO, P4, C4, M_WR, M_NO, P_0XX0)
    X(0x3B08, 0x00F0, WORD, IND,    M_IR,   M_IRIO, P4, C4, M_WR, M_NO, P_0XX8)
    X(0x3B09, 0x00F0, WORD, SINDR,  M_IR,   M_IRIO, P4, C4, M_WR, M_NO, P_0XX0)
    X(0x3B09, 0x00F0, WORD, SIND,   M_IR,   M_IRIO, P4, C4, M_WR, M_NO, P_0XX8)
    X(0x3B0A, 0x00F0, WORD, OTDR,   M_IRIO, M_IR,   P4, C4, M_WR, M_NO, P_0XX0)
    X(0x3B0A, 0x00F0, WORD, OUTD,   M_IRIO, M_IR,   P4, C4, M_WR, M_NO, P_0XX8)
    X(0x3B0B, 0x00F0, WORD, SOTDR,  M_IRIO, M_IR,   P4, C4, M_WR, M_NO, P_0XX0)
    X(0x3B0B, 0x00F0, WORD, SOUTD,  M_IRIO, M_IR,   P4, C4, M_WR, M_NO, P_0XX8)
    X(0xB800, 0x00F0, BYTE, TRIB,   M_IR,   M_IR,   C4, P4, M_WR, M_NO, P_0XX0)
    X(0xB802, 0x00F0, BYTE, TRTIB,  M_IR,   M_IR,   C4, P4, M_WR, M_NO, P_0XX0)
    X(0xB804, 0x00F0, BYTE, TRIRB,  M_IR,   M_IR,   C4, P4, M_WR, M_NO, P_0XX0)
    X(0xB806, 0x00F0, BYTE, TRTIRB, M_IR,   M_IR,   C4, P4, M_WR, M_NO, P_0XXE)
    X(0xB808, 0x00F0, BYTE, TRDB,   M_IR,   M_IR,   C4, P4, M_WR, M_NO, P_0XX0)
    X(0xB80A, 0x00F0, BYTE, TRTDB,  M_IR,   M_IR,   C4, P4, M_WR, M_NO, P_0XX0)
    X(0xB80C, 0x00F0, BYTE, TRDRB,  M_IR,   M_IR,   C4, P4, M_WR, M_NO, P_0XX0)
    X(0xB80E, 0x00F0, BYTE, TRTDRB, M_IR,   M_IR,   C4, P4, M_WR, M_NO, P_0XXE)
    X(0xBA00, 0x00F0, BYTE, CPIB,   M_R,    M_IR,   P4, C4, M_WR, M_CC, P_0XXX)
    X(0xBA01, 0x00F0, BYTE, LDIRB,  M_IR,   M_IR,   P4, C4, M_WR, M_NO, P_0XX0)
    X(0xBA01, 0x00F0, BYTE, LDIB,   M_IR,   M_IR,   P4, C4, M_WR, M_NO, P_0XX8)
    X(0xBA02, 0x00F0, BYTE, CPSIB,  M_IR,   M_IR,   P4, C4, M_WR, M_CC, P_0XXX)
    X(0xBA04, 0x00F0, BYTE, CPIRB,  M_R,    M_IR,   P4, C4, M_WR, M_CC, P_0XXX)
    X(0xBA06, 0x00F0, BYTE, CPSIRB, M_IR,   M_IR,   P4, C4, M_WR, M_CC, P_0XXX)
    X(0xBA08, 0x00F0, BYTE, CPDB,   M_R,    M_IR,   P4, C4, M_WR, M_CC, P_0XXX)
    X(0xBA09, 0x00F0, BYTE, LDDRB,  M_IR,   M_IR,   P4, C4, M_WR, M_NO, P_0XX0)
    X(0xBA09, 0x00F0, BYTE, LDDB,   M_IR,   M_IR,   P4, C4, M_WR, M_NO, P_0XX8)
    X(0xBA0A, 0x00F0, BYTE, CPSDB,  M_IR,   M_IR,   P4, C4, M_WR, M_CC, P_0XXX)
    X(0xBA0C, 0x00F0, BYTE, CPDRB,  M_R,    M_IR,   P4, C4, M_WR, M_CC, P_0XXX)
    X(0xBA0E, 0x00F0, BYTE, CPSDRB, M_IR,   M_IR,   P4, C4, M_WR, M_CC, P_0XXX)
    X(0xBB00, 0x00F0, WORD, CPI,    M_R,    M_IR,   P4, C4, M_WR, M_CC, P_0XXX)
    X(0xBB01, 0x00F0, WORD, LDIR,   M_IR,   M_IR,   P4, C4, M_WR, M_NO, P_0XX0)
    X(0xBB01, 0x00F0, WORD, LDI,    M_IR,   M_IR,   P4, C4, M_WR, M_NO, P_0XX8)
    X(0xBB02, 0x00F0, WORD, CPSI,   M_IR,   M_IR,   P4, C4, M_WR, M_CC, P_0XXX)
    X(0xBB04, 0x00F0, WORD, CPIR,   M_R,    M_IR,   P4, C4, M_WR, M_CC, P_0XXX)
    X(0xBB06, 0x00F0, WORD, CPSIR,  M_IR,   M_IR,   P4, C4, M_WR, M_CC, P_0XXX)
    X(0xBB08, 0x00F0, WORD, CPD,    M_R,    M_IR,   P4, C4, M_WR, M_CC, P_0XXX)
    X(0xBB09, 0x00F0, WORD, LDDR,   M_IR,   M_IR,   P4, C4, M_WR, M_NO, P_0XX0)
    X(0xBB09, 0x00F0, WORD, LDD,    M_IR,   M_IR,   P4, C4, M_WR, M_NO, P_0XX8)
    X(0xBB0A, 0x00F0, WORD, CPSD,   M_IR,   M_IR,   P4, C4, M_WR, M_CC, P_0XXX)
    X(0xBB0C, 0x00F0, WORD, CPDR,   M_R,    M_IR,   P4, C4, M_WR, M_CC, P_0XXX)
    X(0xBB0E, 0x00F0, WORD, CPSDR,  M_IR,   M_IR,   P4, C4, M_WR, M_CC, P_0XXX)
    E(0x0000, 0xC0FF, BYTE, ADDB,   M_R,    M_GENI, C0, C4)
    E(0x0100, 0xC0FF, WORD, ADD,    M_R,    M_GENI, C0, C4)
    E(0x0200, 0xC0FF, BYTE, SUBB,   M_R,    M_GENI, C0, C4)
    E(0x0300, 0xC0FF, WORD, SUB,    M_R,    M_GENI, C0, C4)
    E(0x0400, 0xC0FF, BYTE, ORB,    M_R,    M_GENI, C0, C4)
    E(0x0500, 0xC0FF, WORD, OR,     M_R,    M_GENI, C0, C4)
    E(0x0600, 0xC0FF, BYTE, ANDB,   M_R,    M_GENI, C0, C4)
    E(0x0700, 0xC0FF, WORD, AND,    M_R,    M_GENI, C0, C4)
    E(0x0800, 0xC0FF, BYTE, XORB,   M_R,    M_GENI, C0, C4)
    E(0x0900, 0xC0FF, WORD, XOR,    M_R,    M_GENI, C0, C4)
    E(0x0A00, 0xC0FF, BYTE, CPB,    M_R,    M_GENI, C0, C4)
    E(0x0B00, 0xC0FF, WORD, CP,     M_R,    M_GENI, C0, C4)
    E(0x0D00, 0xC0F0, WORD, COM,    M_GEND, M_NO,   C4, NO)
    E(0x0D01, 0xC0F0, WORD, CP,     M_GENA, M_IM,   C4, NO)
    E(0x0D02, 0xC0F0, WORD, NEG,    M_GEND, M_NO,   C4, NO)
    E(0x0D04, 0xC0F0, WORD, TEST,   M_GEND, M_NO,   C4, NO)
    E(0x0D05, 0xC0F0, WORD, LD,     M_GENA, M_IM,   C4, NO)
    E(0x0D06, 0xC0F0, WORD, TSET,   M_GEND, M_NO,   C4, NO)
    E(0x0D08, 0xC0F0, WORD, CLR,    M_GEND, M_NO,   C4, NO)
    E(0x0D09, 0xC0F0, WORD, PUSH,   M_IR,   M_IM,   C4, NO)
    E(0x1000, 0xC0FF, LONG, CPL,    M_R,    M_GENI, C0, C4)
    E(0x1100, 0xC0FF, LONG, PUSHL,  M_IR,   M_GEND, C4, C0)
    E(0x1200, 0xC0FF, LONG, SUBL,   M_R,    M_GENI, C0, C4)
    E(0x1300, 0xC0FF, WORD, PUSH,   M_IR,   M_GEND, C4, C0)
    E(0x1400, 0xC0FF, LONG, LDL,    M_R,    M_GENI, C0, C4)
    E(0x1500, 0xC0FF, LONG, POPL,   M_GEND, M_IR,   C0, C4)
    E(0x1600, 0xC0FF, LONG, ADDL,   M_R,    M_GENI, C0, C4)
    E(0x1700, 0xC0FF, WORD, POP,    M_GEND, M_IR,   C0, C4)
    E(0x1800, 0xC0FF, LONG, MULTL,  M_DR,   M_GENI, C0, C4)
    E(0x1900, 0xC0FF, WORD, MULT,   M_DR,   M_GENI, C0, C4)
    E(0x1A00, 0xC0FF, LONG, DIVL,   M_DR,   M_GENI, C0, C4)
    E(0x1B00, 0xC0FF, WORD, DIV,    M_DR,   M_GENI, C0, C4)
    X(0x1C01, 0xC0F0, WORD, LDM,    M_R,    M_GENA, P8, C4, M_CNT, M_NO, P_0X0X)
    E(0x1C08, 0xC0F0, LONG, TESTL,  M_GEND, M_NO,   C4, NO)
    X(0x1C09, 0xC0F0, WORD, LDM,    M_GENA, M_R,    C4, P8, M_CNT, M_NO, P_0X0X)
    E(0x1D00, 0xC0FF, LONG, LDL,    M_GENA, M_R,    C4, C0)
    E(0x1E00, 0xC0FF, NONE, JP,     M_CC,   M_GENA, C0, C4)
    E(0x1E08, 0xC0F0, NONE, JP,     M_GENA, M_NO,   C4, NO)
    E(0x1F00, 0xC0FF, NONE, CALL,   M_GENA, M_NO,   C4, NO)
    E(0x2000, 0xC0FF, BYTE, LDB,    M_R,    M_GENI, C0, C4)
    E(0x2100, 0xC0FF, WORD, LD,     M_R,    M_GENI, C0, C4)
    X(0x2200, 0x000F, BYTE, RESB,   M_R,    M_WR07, P8, C0, M_NO,  M_NO, P_0X00)
    E(0x2200, 0xC0FF, BYTE, RESB,   M_GEND, M_BIT,  C4, C0)
    X(0x2300, 0x000F, WORD, RES,    M_R,    M_R,    P8, C0, M_NO,  M_NO, P_0X00)
    E(0x2300, 0xC0FF, WORD, RES,    M_GEND, M_BIT,  C4, C0)
    X(0x2400, 0x000F, BYTE, SETB,   M_R,    M_WR07, P8, C0, M_NO,  M_NO, P_0X00)
    E(0x2400, 0xC0FF, BYTE, SETB,   M_GEND, M_BIT,  C4, C0)
    X(0x2500, 0x000F, WORD, SET,    M_R,    M_R,    P8, C0, M_NO,  M_NO, P_0X00)
    E(0x2500, 0xC0FF, WORD, SET,    M_GEND, M_BIT,  C4, C0)
    X(0x2600, 0x000F, BYTE, BITB,   M_R,    M_WR07, P8, C0, M_NO,  M_NO, P_0X00)
    E(0x2600, 0xC0FF, BYTE, BITB,   M_GEND, M_BIT,  C4, C0)
    X(0x2700, 0x000F, WORD, BIT,    M_R,    M_R,    P8, C0, M_NO,  M_NO, P_0X00)
    E(0x2700, 0xC0FF, WORD, BIT,    M_GEND, M_BIT,  C4, C0)
    E(0x2800, 0xC0FF, BYTE, INCB,   M_GEND, M_CNT,  C4, C0)
    E(0x2900, 0xC0FF, WORD, INC,    M_GEND, M_CNT,  C4, C0)
    E(0x2A00, 0xC0FF, BYTE, DECB,   M_GEND, M_CNT,  C4, C0)
    E(0x2B00, 0xC0FF, WORD, DEC,    M_GEND, M_CNT,  C4, C0)
    E(0x2C00, 0xC0FF, BYTE, EXB,    M_R,    M_GEND, C0, C4)
    E(0x2D00, 0xC0FF, WORD, EX,     M_R,    M_GEND, C0, C4)
    E(0x2E00, 0xC0FF, BYTE, LDB,    M_GENA, M_R,    C4, C0)
    E(0x2F00, 0xC0FF, WORD, LD,     M_GENA, M_R,    C4, C0)
    E(0x3900, 0xC0F0, WORD, LDPS,   M_GENA, M_NO,   C4, NO)
};

static bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table) return true;
    if (opr == M_R)
        return table == M_GENI || table == M_GEND || table == M_WR07 || table == M_WR
            || table == M_DR;
    if (opr == M_IM)
        return table == M_GENI || table == M_IO || table == M_IM8 || table == M_BIT
            || table == M_CNT || table == M_QCNT || table == M_SCNT || table == M_NCNT;
    if (opr == M_IR)
        return table == M_GENI || table == M_GEND || table == M_GENA || table == M_IRIO;
    if (opr == M_DA)
        return table == M_GENI || table == M_GEND || table == M_GENA
            || table == M_RA || table == M_RA12 || table == M_RA8 || table == M_RA7;
    if (opr == M_X)
        return table == M_GENI || table == M_GEND || table == M_GENA;
    if (opr == M_CC) return table == M_FLAG; // C & Z
    if (opr == M_NO) return table == M_CC || table == M_INTT || table == M_FLAG;
    return false;
}

static bool acceptModes(uint32_t flags, const Entry *entry) {
    const uint32_t table = pgm_read_dword(&entry->flags);
    return acceptMode(Entry::_mode(Entry::_dst(flags)), Entry::_mode(Entry::_dst(table)))
        && acceptMode(Entry::_mode(Entry::_src(flags)), Entry::_mode(Entry::_src(table)))
        && acceptMode(Entry::_ex1(Entry::_ext(flags)), Entry::_ex1(Entry::_ext(table)))
        && acceptMode(Entry::_ex2(Entry::_ext(flags)), Entry::_ex2(Entry::_ext(table)));
}

Error TableZ8000::searchName(InsnZ8000 &insn) const {
    uint8_t count = 0;
    const uint32_t flags = Entry::_flags(
            Entry::_opr(insn.dstMode(), MF_NO),
            Entry::_opr(insn.srcMode(), MF_NO),
            Entry::_ext(insn.ex1Mode(), insn.ex2Mode(), P_NO),
            0
        );
    const Entry *entry = TableBase::searchName<Entry, uint32_t>(
            insn.name(), flags, ARRAY_RANGE(Z8000_TABLE), acceptModes, count);
    if (entry) {
        insn.setOpCode(pgm_read_word(&entry->opCode));
        insn.setFlags(pgm_read_dword(&entry->flags));
        return _error.setOK();
    }
    return _error.setError(
            count == 0 ? UNKNOWN_INSTRUCTION : OPERAND_NOT_ALLOWED);
}

static Config::opcode_t tableCode(
        Config::opcode_t opCode, const Entry *entry) {
    const uint32_t flags = pgm_read_dword(&entry->flags);
    const uint16_t mask = Entry::_mask(Entry::_size(flags));
    return opCode & ~mask;
}

static bool matchPostWord(const InsnZ8000 &insn) {
    const uint16_t post = insn.post() & insn.postMask();
    return post == insn.postVal();
}

const Entry *TableZ8000::searchOpCode(
    InsnZ8000 &insn, DisMemory &memory,
    const Entry *table, const Entry *end) const {
    const Config::opcode_t opCode = insn.opCode();
    for (const Entry *entry = table;
         entry < end
             && (entry = TableBase::searchCode<Entry, Config::opcode_t>(
                     opCode, entry, end, tableCode))!= nullptr;
         entry++) {
        insn.setFlags(pgm_read_dword(&entry->flags));
        if (insn.hasPost()) {
            if (insn.length() < 4) {
                insn.readPost(memory);
                if (_error.setError(insn))
                    return nullptr;
            }
            if (!matchPostWord(insn)) continue;
        }
        const /*PROGMEM*/ char *name =
            reinterpret_cast<const char *>(pgm_read_ptr(&entry->name));
        insn.setName_P(name);
        return entry;
    }
    return nullptr;
}

Error TableZ8000::searchOpCode(InsnZ8000 &insn, DisMemory &memory) const {
    const Entry *entry = searchOpCode(insn, memory, ARRAY_RANGE(Z8000_TABLE));
    return _error.setError(entry ? OK : UNKNOWN_INSTRUCTION);
}

Error TableZ8000::searchOpCodeAlias(InsnZ8000 &insn, DisMemory &memory) const {
    const Entry *entry = searchOpCode(insn, memory, ARRAY_RANGE(Z8000_TABLE));
    if (entry) {
        entry++;
        insn.setFlags(pgm_read_dword(&entry->flags));
        const /*PROGMEM*/ char *name =
            reinterpret_cast<const char *>(pgm_read_ptr(&entry->name));
        insn.setName_P(name);
    }
    return _error.setError(entry ? OK : UNKNOWN_INSTRUCTION);
}

TableZ8000::TableZ8000()
    : _cpuType(Z8001)
{
}

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

} // namespace z8000
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

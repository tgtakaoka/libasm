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

static constexpr AddrMode ex1Modes[] PROGMEM = {
        M_NO,     // E1_NO
        M_CNT,    // E1_CNT
        M_WR,     // E1_WR
        M_ERROR,  // E1_ERROR
};
AddrMode Entry::toAddrMode(Ex1Mode mode) {
    return AddrMode(pgm_read_byte(ex1Modes + mode));
}

static constexpr AddrMode ex2Modes[] PROGMEM = {
        M_NO,     // E2_NO
        M_CC,     // E2_CC
        M_ERROR,  // E2_ERROR
};
AddrMode Entry::toAddrMode(Ex2Mode mode) {
    return AddrMode(pgm_read_byte(ex2Modes + mode));
}

static constexpr uint8_t postVals[] PROGMEM = {
        0x0,  // P_NO
        0x0,  // P_0XX0
        0x8,  // P_0XX8
        0xE,  // P_0XXE
        0x0,  // P_0X0X
        0x0,  // P_0X00
        0x0,  // P_0XXX
};
uint8_t Entry::postVal(PostMode post) {
    return pgm_read_byte(postVals + post);
}

static constexpr uint8_t postMasks[] PROGMEM = {
        0x0,  // P_NO
        0xF,  // P_0XX0
        0xF,  // P_0XX8
        0xF,  // P_0XXE
        0x0,  // P_0X0X
        0x0,  // P_0X00
        0x0,  // P_0XXX
};
uint8_t Entry::postMask(PostMode post) {
    return pgm_read_byte(postMasks + post);
}

static constexpr uint16_t codeMasks[] PROGMEM = {
        (uint16_t)~0x0000,  // CM_0x0000
        (uint16_t)~0x0003,  // CM_0x0003
        (uint16_t)~0x000F,  // CM_0x000F
        (uint16_t)~0x00F0,  // CM_0x00F0
        (uint16_t)~0x00F2,  // CM_0x00F2
        (uint16_t)~0x00F7,  // CM_0x00F7
        (uint16_t)~0x00FF,  // CM_0x00FF
        (uint16_t)~0x0F7F,  // CM_0x0F7F
        (uint16_t)~0x0FFF,  // CM_0x0FFF
        (uint16_t)~0xC0F0,  // CM_0xC0F0
        (uint16_t)~0xC0FF,  // CM_0xC0FF
};
uint16_t Entry::codeMask(uint8_t size) {
    const CodeMask cm = CodeMask((size >> codeMask_gp) & codeMask_gm);
    return pgm_read_word(codeMasks + cm);
}

#define X(_opc, _name, _cm, _sz, _dst, _src, _dstf, _srcf, _ex1, _ex2, _post)               \
    {                                                                                       \
        _opc,                                                                               \
                Entry::Flags::create(_dst, MF_##_dstf, _src, MF_##_srcf, _ex1, _ex2, _post, \
                        CM_##_cm, SZ_##_sz),                                                \
                _name                                                                       \
    }
#define E(_opc, _name, _cm, _sz, _dst, _src, _dstf, _srcf) \
    X(_opc, _name, _cm, _sz, _dst, _src, _dstf, _srcf, E1_NO, E2_NO, P_NO)

// clang-format off
static const Entry Z8000_TABLE[] PROGMEM = {
    E(0xC000, TEXT_LDB,    0x0FFF, BYTE, M_R,    M_IM8,  C8, C0),
    E(0xD000, TEXT_CALR,   0x0FFF, NONE, M_RA12, M_NO,   C0, NO),
    E(0xE000, TEXT_JR,     0x0FFF, NONE, M_CC,   M_RA8,  C8, C0),
    E(0xE800, TEXT_JR,     0x00FF, NONE, M_RA8,  M_NO,   C0, NO),
    E(0xF000, TEXT_DBJNZ,  0x0F7F, BYTE, M_R,    M_RA7,  C8, C0),
    E(0xF080, TEXT_DJNZ,   0x0F7F, WORD, M_R,    M_RA7,  C8, C0),
    E(0x3000, TEXT_LDRB,   0x000F, BYTE, M_R,    M_RA,   C0, NO),
    E(0x3000, TEXT_LDB,    0x00FF, BYTE, M_R,    M_BA,   C0, C4),
    E(0x3100, TEXT_LDR,    0x000F, WORD, M_R,    M_RA,   C0, NO),
    E(0x3100, TEXT_LD,     0x00FF, WORD, M_R,    M_BA,   C0, C4),
    E(0x3200, TEXT_LDRB,   0x000F, BYTE, M_RA,   M_R,    NO, C0),
    E(0x3200, TEXT_LDB,    0x00FF, BYTE, M_BA,   M_R,    C4, C0),
    E(0x3300, TEXT_LDR,    0x000F, WORD, M_RA,   M_R,    NO, C0),
    E(0x3300, TEXT_LD,     0x00FF, WORD, M_BA,   M_R,    C4, C0),
    E(0x3400, TEXT_LDAR,   0x000F, ADDR, M_R,    M_RA,   C0, NO),
    E(0x3400, TEXT_LDA,    0x00FF, ADDR, M_R,    M_BA,   C0, C4),
    E(0x3500, TEXT_LDRL,   0x000F, LONG, M_R,    M_RA,   C0, NO),
    E(0x3500, TEXT_LDL,    0x00FF, LONG, M_R,    M_BA,   C0, C4),
    E(0x3700, TEXT_LDRL,   0x000F, LONG, M_RA,   M_R,    NO, C0),
    E(0x3700, TEXT_LDL,    0x00FF, LONG, M_BA,   M_R,    C4, C0),
    E(0x3C00, TEXT_INB,    0x00FF, BYTE, M_R,    M_IRIO, C0, C4),
    E(0x3D00, TEXT_IN,     0x00FF, WORD, M_R,    M_IRIO, C0, C4),
    E(0x3E00, TEXT_OUTB,   0x00FF, BYTE, M_IRIO, M_R,    C4, C0),
    E(0x3F00, TEXT_OUT,    0x00FF, WORD, M_IRIO, M_R,    C4, C0),
    X(0x7000, TEXT_LDB,    0x00FF, BYTE, M_R,    M_BX,   C0, C4, E1_NO, E2_NO, P_0X00),
    X(0x7100, TEXT_LD,     0x00FF, WORD, M_R,    M_BX,   C0, C4, E1_NO, E2_NO, P_0X00),
    X(0x7200, TEXT_LDB,    0x00FF, BYTE, M_BX,   M_R,    C4, C0, E1_NO, E2_NO, P_0X00),
    X(0x7300, TEXT_LD,     0x00FF, WORD, M_BX,   M_R,    C4, C0, E1_NO, E2_NO, P_0X00),
    X(0x7400, TEXT_LDA,    0x00FF, ADDR, M_R,    M_BX,   C0, C4, E1_NO, E2_NO, P_0X00),
    X(0x7500, TEXT_LDL,    0x00FF, LONG, M_R,    M_BX,   C0, C4, E1_NO, E2_NO, P_0X00),
    E(0x7600, TEXT_LDA,    0x00FF, ADDR, M_R,    M_GENA, C0, C4),
    X(0x7700, TEXT_LDL,    0x00FF, LONG, M_BX,   M_R,    C4, C0, E1_NO, E2_NO, P_0X00),
    E(0x7A00, TEXT_HALT,   0x0000, NONE, M_NO,   M_NO,   NO, NO),
    E(0x7B00, TEXT_IRET,   0x0000, NONE, M_NO,   M_NO,   NO, NO),
    E(0x7B08, TEXT_MSET,   0x0000, NONE, M_NO,   M_NO,   NO, NO),
    E(0x7B09, TEXT_MRES,   0x0000, NONE, M_NO,   M_NO,   NO, NO),
    E(0x7B0A, TEXT_MBIT,   0x0000, NONE, M_NO,   M_NO,   NO, NO),
    E(0x7B0D, TEXT_MREQ,   0x00F0, WORD, M_R,    M_NO,   C4, NO),
    E(0x7C00, TEXT_DI,     0x0003, NONE, M_INTR, M_NO,   C0, NO),
    E(0x7C04, TEXT_EI,     0x0003, NONE, M_INTR, M_NO,   C0, NO),
    E(0x7D00, TEXT_LDCTL,  0x00F7, WORD, M_R,    M_CTL,  C4, C0),
    E(0x7D08, TEXT_LDCTL,  0x00F7, WORD, M_CTL,  M_R,    C0, C4),
    E(0x7F00, TEXT_SC,     0x00FF, NONE, M_IM8,  M_NO,   C0, C0),
    E(0x8C01, TEXT_LDCTLB, 0x00F0, BYTE, M_R,    M_CTL,  C4, C0),
    E(0x8C09, TEXT_LDCTLB, 0x00F0, BYTE, M_CTL,  M_R,    C0, C4),
    E(0x0C00, TEXT_COMB,   0xC0F0, BYTE, M_GEND, M_NO,   C4, NO),
    E(0x0C01, TEXT_CPB,    0xC0F0, BYTE, M_GENA, M_IM,   C4, NO),
    E(0x0C02, TEXT_NEGB,   0xC0F0, BYTE, M_GEND, M_NO,   C4, NO), // NEGB/LDCTLB share same bit pattern
    E(0x0C04, TEXT_TESTB,  0xC0F0, BYTE, M_GEND, M_NO,   C4, NO),
    E(0x0C05, TEXT_LDB,    0xC0F0, BYTE, M_GENA, M_IM,   C4, NO),
    E(0x0C06, TEXT_TSETB,  0xC0F0, BYTE, M_GEND, M_NO,   C4, NO),
    E(0x0C08, TEXT_CLRB,   0xC0F0, BYTE, M_GEND, M_NO,   C4, NO), // CLRB/LDCTLB share smae bit pattern
    E(0x8D01, TEXT_SETFLG, 0x00F0, NONE, M_FLAG, M_NO,   C4, NO),
    E(0x8D03, TEXT_RESFLG, 0x00F0, NONE, M_FLAG, M_NO,   C4, NO),
    E(0x8D05, TEXT_COMFLG, 0x00F0, NONE, M_FLAG, M_NO,   C4, NO),
    E(0x8D07, TEXT_NOP,    0x0000, NONE, M_NO,   M_NO,   NO, NO),
    E(0x9E00, TEXT_RET,    0x000F, NONE, M_CC,   M_NO,   C0, NO),
    E(0xAE00, TEXT_TCCB,   0x00FF, BYTE, M_CC,   M_R,    C0, C4),
    E(0xAE08, TEXT_TCCB,   0x00F0, BYTE, M_R,    M_NO,   C4, NO),
    E(0xAF00, TEXT_TCC,    0x00FF, WORD, M_CC,   M_R,    C0, C4),
    E(0xAF08, TEXT_TCC,    0x00F0, WORD, M_R,    M_NO,   C4, NO),
    E(0xB000, TEXT_DAB,    0x00F0, BYTE, M_R,    M_NO,   C4, NO),
    E(0xB100, TEXT_EXTSB,  0x00F0, BYTE, M_DR,   M_NO,   C4, NO),
    E(0xB107, TEXT_EXTSL,  0x00F0, LONG, M_DR,   M_NO,   C4, NO),
    E(0xB10A, TEXT_EXTS,   0x00F0, WORD, M_DR,   M_NO,   C4, NO),
    E(0xB200, TEXT_RLB,    0x00F2, BYTE, M_R,    M_QCNT, C4, C0),
    E(0xB201, TEXT_SLLB,   0x00F0, BYTE, M_R,    M_SCNT, C4, NO),
    E(0xB201, TEXT_SRLB,   0x00F0, BYTE, M_R,    M_NCNT, C4, NO),
    X(0xB203, TEXT_SDLB,   0x00F0, BYTE, M_R,    M_WR,   C4, P8, E1_NO, E2_NO, P_0X00),
    E(0xB204, TEXT_RRB,    0x00F2, BYTE, M_R,    M_QCNT, C4, C0),
    E(0xB208, TEXT_RLCB,   0x00F2, BYTE, M_R,    M_QCNT, C4, C0),
    E(0xB209, TEXT_SLAB,   0x00F0, BYTE, M_R,    M_SCNT, C4, NO),
    E(0xB209, TEXT_SRAB,   0x00F0, BYTE, M_R,    M_NCNT, C4, NO),
    X(0xB20B, TEXT_SDAB,   0x00F0, BYTE, M_R,    M_WR,   C4, P8, E1_NO, E2_NO, P_0X00),
    E(0xB20C, TEXT_RRCB,   0x00F2, BYTE, M_R,    M_QCNT, C4, C0),
    E(0xB300, TEXT_RL,     0x00F2, WORD, M_R,    M_QCNT, C4, C0),
    E(0xB301, TEXT_SLL,    0x00F0, WORD, M_R,    M_SCNT, C4, NO),
    E(0xB301, TEXT_SRL,    0x00F0, WORD, M_R,    M_NCNT, C4, NO),
    X(0xB303, TEXT_SDL,    0x00F0, WORD, M_R,    M_WR,   C4, P8, E1_NO, E2_NO, P_0X00),
    E(0xB304, TEXT_RR,     0x00F2, WORD, M_R,    M_QCNT, C4, C0),
    E(0xB305, TEXT_SLLL,   0x00F0, LONG, M_R,    M_SCNT, C4, NO),
    E(0xB305, TEXT_SRLL,   0x00F0, LONG, M_R,    M_NCNT, C4, NO),
    X(0xB307, TEXT_SDLL,   0x00F0, LONG, M_R,    M_WR,   C4, P8, E1_NO, E2_NO, P_0X00),
    E(0xB308, TEXT_RLC,    0x00F2, WORD, M_R,    M_QCNT, C4, C0),
    E(0xB309, TEXT_SLA,    0x00F0, WORD, M_R,    M_SCNT, C4, NO),
    E(0xB309, TEXT_SRA,    0x00F0, WORD, M_R,    M_NCNT, C4, NO),
    X(0xB30B, TEXT_SDA,    0x00F0, WORD, M_R,    M_WR,   C4, P8, E1_NO, E2_NO, P_0X00),
    E(0xB30C, TEXT_RRC,    0x00F2, WORD, M_R,    M_QCNT, C4, C0),
    E(0xB30D, TEXT_SLAL,   0x00F0, LONG, M_R,    M_SCNT, C4, NO),
    E(0xB30D, TEXT_SRAL,   0x00F0, LONG, M_R,    M_NCNT, C4, NO),
    X(0xB30F, TEXT_SDAL,   0x00F0, LONG, M_R,    M_WR,   C4, P8, E1_NO, E2_NO, P_0X00),
    E(0xB400, TEXT_ADCB,   0x00FF, BYTE, M_R,    M_R,    C0, C4),
    E(0xB500, TEXT_ADC,    0x00FF, WORD, M_R,    M_R,    C0, C4),
    E(0xB600, TEXT_SBCB,   0x00FF, BYTE, M_R,    M_R,    C0, C4),
    E(0xB700, TEXT_SBC,    0x00FF, WORD, M_R,    M_R,    C0, C4),
    E(0xBC00, TEXT_RRDB,   0x00FF, BYTE, M_R,    M_R,    C0, C4),
    E(0xBD00, TEXT_LDK,    0x00FF, WORD, M_R,    M_BIT,  C4, C0),
    E(0xBE00, TEXT_RLDB,   0x00FF, BYTE, M_R,    M_R,    C0, C4),
    E(0x3A04, TEXT_INB,    0x00F0, BYTE, M_R,    M_IO,   C4, NO),
    E(0x3A05, TEXT_SINB,   0x00F0, BYTE, M_R,    M_IO,   C4, NO),
    E(0x3A06, TEXT_OUTB,   0x00F0, BYTE, M_IO,   M_R,    NO, C4),
    E(0x3A07, TEXT_SOUTB,  0x00F0, BYTE, M_IO,   M_R,    NO, C4),
    E(0x3B04, TEXT_IN,     0x00F0, WORD, M_R,    M_IO,   C4, NO),
    E(0x3B05, TEXT_SIN,    0x00F0, WORD, M_R,    M_IO,   C4, NO),
    E(0x3B06, TEXT_OUT,    0x00F0, WORD, M_IO,   M_R,    NO, C4),
    E(0x3B07, TEXT_SOUT,   0x00F0, WORD, M_IO,   M_R,    NO, C4),
    X(0x3A00, TEXT_INIRB,  0x00F0, BYTE, M_IR,   M_IRIO, P4, C4, E1_WR, E2_NO, P_0XX0),
    X(0x3A00, TEXT_INIB,   0x00F0, BYTE, M_IR,   M_IRIO, P4, C4, E1_WR, E2_NO, P_0XX8),
    X(0x3A01, TEXT_SINIRB, 0x00F0, BYTE, M_IR,   M_IRIO, P4, C4, E1_WR, E2_NO, P_0XX0),
    X(0x3A01, TEXT_SINIB,  0x00F0, BYTE, M_IR,   M_IRIO, P4, C4, E1_WR, E2_NO, P_0XX8),
    X(0x3A02, TEXT_OTIRB,  0x00F0, BYTE, M_IRIO, M_IR,   P4, C4, E1_WR, E2_NO, P_0XX0),
    X(0x3A02, TEXT_OUTIB,  0x00F0, BYTE, M_IRIO, M_IR,   P4, C4, E1_WR, E2_NO, P_0XX8),
    X(0x3A03, TEXT_SOTIRB, 0x00F0, BYTE, M_IRIO, M_IR,   P4, C4, E1_WR, E2_NO, P_0XX0),
    X(0x3A03, TEXT_SOUTIB, 0x00F0, BYTE, M_IRIO, M_IR,   P4, C4, E1_WR, E2_NO, P_0XX8),
    X(0x3A08, TEXT_INDRB,  0x00F0, BYTE, M_IR,   M_IRIO, P4, C4, E1_WR, E2_NO, P_0XX0),
    X(0x3A08, TEXT_INDB,   0x00F0, BYTE, M_IR,   M_IRIO, P4, C4, E1_WR, E2_NO, P_0XX8),
    X(0x3A09, TEXT_SINDRB, 0x00F0, BYTE, M_IR,   M_IRIO, P4, C4, E1_WR, E2_NO, P_0XX0),
    X(0x3A09, TEXT_SINDB,  0x00F0, BYTE, M_IR,   M_IRIO, P4, C4, E1_WR, E2_NO, P_0XX8),
    X(0x3A0A, TEXT_OTDRB,  0x00F0, BYTE, M_IRIO, M_IR,   P4, C4, E1_WR, E2_NO, P_0XX0),
    X(0x3A0A, TEXT_OUTDB,  0x00F0, BYTE, M_IRIO, M_IR,   P4, C4, E1_WR, E2_NO, P_0XX8),
    X(0x3A0B, TEXT_SOTDRB, 0x00F0, BYTE, M_IRIO, M_IR,   P4, C4, E1_WR, E2_NO, P_0XX0),
    X(0x3A0B, TEXT_SOUTDB, 0x00F0, BYTE, M_IRIO, M_IR,   P4, C4, E1_WR, E2_NO, P_0XX8),
    X(0x3B00, TEXT_INIR,   0x00F0, WORD, M_IR,   M_IRIO, P4, C4, E1_WR, E2_NO, P_0XX0),
    X(0x3B00, TEXT_INI,    0x00F0, WORD, M_IR,   M_IRIO, P4, C4, E1_WR, E2_NO, P_0XX8),
    X(0x3B01, TEXT_SINIR,  0x00F0, WORD, M_IR,   M_IRIO, P4, C4, E1_WR, E2_NO, P_0XX0),
    X(0x3B01, TEXT_SINI,   0x00F0, WORD, M_IR,   M_IRIO, P4, C4, E1_WR, E2_NO, P_0XX8),
    X(0x3B02, TEXT_OTIR,   0x00F0, WORD, M_IRIO, M_IR,   P4, C4, E1_WR, E2_NO, P_0XX0),
    X(0x3B02, TEXT_OUTI,   0x00F0, WORD, M_IRIO, M_IR,   P4, C4, E1_WR, E2_NO, P_0XX8),
    X(0x3B03, TEXT_SOTIR,  0x00F0, WORD, M_IRIO, M_IR,   P4, C4, E1_WR, E2_NO, P_0XX0),
    X(0x3B03, TEXT_SOUTI,  0x00F0, WORD, M_IRIO, M_IR,   P4, C4, E1_WR, E2_NO, P_0XX8),
    X(0x3B08, TEXT_INDR,   0x00F0, WORD, M_IR,   M_IRIO, P4, C4, E1_WR, E2_NO, P_0XX0),
    X(0x3B08, TEXT_IND,    0x00F0, WORD, M_IR,   M_IRIO, P4, C4, E1_WR, E2_NO, P_0XX8),
    X(0x3B09, TEXT_SINDR,  0x00F0, WORD, M_IR,   M_IRIO, P4, C4, E1_WR, E2_NO, P_0XX0),
    X(0x3B09, TEXT_SIND,   0x00F0, WORD, M_IR,   M_IRIO, P4, C4, E1_WR, E2_NO, P_0XX8),
    X(0x3B0A, TEXT_OTDR,   0x00F0, WORD, M_IRIO, M_IR,   P4, C4, E1_WR, E2_NO, P_0XX0),
    X(0x3B0A, TEXT_OUTD,   0x00F0, WORD, M_IRIO, M_IR,   P4, C4, E1_WR, E2_NO, P_0XX8),
    X(0x3B0B, TEXT_SOTDR,  0x00F0, WORD, M_IRIO, M_IR,   P4, C4, E1_WR, E2_NO, P_0XX0),
    X(0x3B0B, TEXT_SOUTD,  0x00F0, WORD, M_IRIO, M_IR,   P4, C4, E1_WR, E2_NO, P_0XX8),
    X(0xB800, TEXT_TRIB,   0x00F0, BYTE, M_IR,   M_IR,   C4, P4, E1_WR, E2_NO, P_0XX0),
    X(0xB802, TEXT_TRTIB,  0x00F0, BYTE, M_IR,   M_IR,   C4, P4, E1_WR, E2_NO, P_0XX0),
    X(0xB804, TEXT_TRIRB,  0x00F0, BYTE, M_IR,   M_IR,   C4, P4, E1_WR, E2_NO, P_0XX0),
    X(0xB806, TEXT_TRTIRB, 0x00F0, BYTE, M_IR,   M_IR,   C4, P4, E1_WR, E2_NO, P_0XXE),
    X(0xB808, TEXT_TRDB,   0x00F0, BYTE, M_IR,   M_IR,   C4, P4, E1_WR, E2_NO, P_0XX0),
    X(0xB80A, TEXT_TRTDB,  0x00F0, BYTE, M_IR,   M_IR,   C4, P4, E1_WR, E2_NO, P_0XX0),
    X(0xB80C, TEXT_TRDRB,  0x00F0, BYTE, M_IR,   M_IR,   C4, P4, E1_WR, E2_NO, P_0XX0),
    X(0xB80E, TEXT_TRTDRB, 0x00F0, BYTE, M_IR,   M_IR,   C4, P4, E1_WR, E2_NO, P_0XXE),
    X(0xBA00, TEXT_CPIB,   0x00F0, BYTE, M_R,    M_IR,   P4, C4, E1_WR, E2_CC, P_0XXX),
    X(0xBA01, TEXT_LDIRB,  0x00F0, BYTE, M_IR,   M_IR,   P4, C4, E1_WR, E2_NO, P_0XX0),
    X(0xBA01, TEXT_LDIB,   0x00F0, BYTE, M_IR,   M_IR,   P4, C4, E1_WR, E2_NO, P_0XX8),
    X(0xBA02, TEXT_CPSIB,  0x00F0, BYTE, M_IR,   M_IR,   P4, C4, E1_WR, E2_CC, P_0XXX),
    X(0xBA04, TEXT_CPIRB,  0x00F0, BYTE, M_R,    M_IR,   P4, C4, E1_WR, E2_CC, P_0XXX),
    X(0xBA06, TEXT_CPSIRB, 0x00F0, BYTE, M_IR,   M_IR,   P4, C4, E1_WR, E2_CC, P_0XXX),
    X(0xBA08, TEXT_CPDB,   0x00F0, BYTE, M_R,    M_IR,   P4, C4, E1_WR, E2_CC, P_0XXX),
    X(0xBA09, TEXT_LDDRB,  0x00F0, BYTE, M_IR,   M_IR,   P4, C4, E1_WR, E2_NO, P_0XX0),
    X(0xBA09, TEXT_LDDB,   0x00F0, BYTE, M_IR,   M_IR,   P4, C4, E1_WR, E2_NO, P_0XX8),
    X(0xBA0A, TEXT_CPSDB,  0x00F0, BYTE, M_IR,   M_IR,   P4, C4, E1_WR, E2_CC, P_0XXX),
    X(0xBA0C, TEXT_CPDRB,  0x00F0, BYTE, M_R,    M_IR,   P4, C4, E1_WR, E2_CC, P_0XXX),
    X(0xBA0E, TEXT_CPSDRB, 0x00F0, BYTE, M_IR,   M_IR,   P4, C4, E1_WR, E2_CC, P_0XXX),
    X(0xBB00, TEXT_CPI,    0x00F0, WORD, M_R,    M_IR,   P4, C4, E1_WR, E2_CC, P_0XXX),
    X(0xBB01, TEXT_LDIR,   0x00F0, WORD, M_IR,   M_IR,   P4, C4, E1_WR, E2_NO, P_0XX0),
    X(0xBB01, TEXT_LDI,    0x00F0, WORD, M_IR,   M_IR,   P4, C4, E1_WR, E2_NO, P_0XX8),
    X(0xBB02, TEXT_CPSI,   0x00F0, WORD, M_IR,   M_IR,   P4, C4, E1_WR, E2_CC, P_0XXX),
    X(0xBB04, TEXT_CPIR,   0x00F0, WORD, M_R,    M_IR,   P4, C4, E1_WR, E2_CC, P_0XXX),
    X(0xBB06, TEXT_CPSIR,  0x00F0, WORD, M_IR,   M_IR,   P4, C4, E1_WR, E2_CC, P_0XXX),
    X(0xBB08, TEXT_CPD,    0x00F0, WORD, M_R,    M_IR,   P4, C4, E1_WR, E2_CC, P_0XXX),
    X(0xBB09, TEXT_LDDR,   0x00F0, WORD, M_IR,   M_IR,   P4, C4, E1_WR, E2_NO, P_0XX0),
    X(0xBB09, TEXT_LDD,    0x00F0, WORD, M_IR,   M_IR,   P4, C4, E1_WR, E2_NO, P_0XX8),
    X(0xBB0A, TEXT_CPSD,   0x00F0, WORD, M_IR,   M_IR,   P4, C4, E1_WR, E2_CC, P_0XXX),
    X(0xBB0C, TEXT_CPDR,   0x00F0, WORD, M_R,    M_IR,   P4, C4, E1_WR, E2_CC, P_0XXX),
    X(0xBB0E, TEXT_CPSDR,  0x00F0, WORD, M_IR,   M_IR,   P4, C4, E1_WR, E2_CC, P_0XXX),
    E(0x0000, TEXT_ADDB,   0xC0FF, BYTE, M_R,    M_GENI, C0, C4),
    E(0x0100, TEXT_ADD,    0xC0FF, WORD, M_R,    M_GENI, C0, C4),
    E(0x0200, TEXT_SUBB,   0xC0FF, BYTE, M_R,    M_GENI, C0, C4),
    E(0x0300, TEXT_SUB,    0xC0FF, WORD, M_R,    M_GENI, C0, C4),
    E(0x0400, TEXT_ORB,    0xC0FF, BYTE, M_R,    M_GENI, C0, C4),
    E(0x0500, TEXT_OR,     0xC0FF, WORD, M_R,    M_GENI, C0, C4),
    E(0x0600, TEXT_ANDB,   0xC0FF, BYTE, M_R,    M_GENI, C0, C4),
    E(0x0700, TEXT_AND,    0xC0FF, WORD, M_R,    M_GENI, C0, C4),
    E(0x0800, TEXT_XORB,   0xC0FF, BYTE, M_R,    M_GENI, C0, C4),
    E(0x0900, TEXT_XOR,    0xC0FF, WORD, M_R,    M_GENI, C0, C4),
    E(0x0A00, TEXT_CPB,    0xC0FF, BYTE, M_R,    M_GENI, C0, C4),
    E(0x0B00, TEXT_CP,     0xC0FF, WORD, M_R,    M_GENI, C0, C4),
    E(0x0D00, TEXT_COM,    0xC0F0, WORD, M_GEND, M_NO,   C4, NO),
    E(0x0D01, TEXT_CP,     0xC0F0, WORD, M_GENA, M_IM,   C4, NO),
    E(0x0D02, TEXT_NEG,    0xC0F0, WORD, M_GEND, M_NO,   C4, NO),
    E(0x0D04, TEXT_TEST,   0xC0F0, WORD, M_GEND, M_NO,   C4, NO),
    E(0x0D05, TEXT_LD,     0xC0F0, WORD, M_GENA, M_IM,   C4, NO),
    E(0x0D06, TEXT_TSET,   0xC0F0, WORD, M_GEND, M_NO,   C4, NO),
    E(0x0D08, TEXT_CLR,    0xC0F0, WORD, M_GEND, M_NO,   C4, NO),
    E(0x0D09, TEXT_PUSH,   0xC0F0, WORD, M_IR,   M_IM,   C4, NO),
    E(0x1000, TEXT_CPL,    0xC0FF, LONG, M_R,    M_GENI, C0, C4),
    E(0x1100, TEXT_PUSHL,  0xC0FF, LONG, M_IR,   M_GEND, C4, C0),
    E(0x1200, TEXT_SUBL,   0xC0FF, LONG, M_R,    M_GENI, C0, C4),
    E(0x1300, TEXT_PUSH,   0xC0FF, WORD, M_IR,   M_GEND, C4, C0),
    E(0x1400, TEXT_LDL,    0xC0FF, LONG, M_R,    M_GENI, C0, C4),
    E(0x1500, TEXT_POPL,   0xC0FF, LONG, M_GEND, M_IR,   C0, C4),
    E(0x1600, TEXT_ADDL,   0xC0FF, LONG, M_R,    M_GENI, C0, C4),
    E(0x1700, TEXT_POP,    0xC0FF, WORD, M_GEND, M_IR,   C0, C4),
    E(0x1800, TEXT_MULTL,  0xC0FF, LONG, M_DR,   M_GENI, C0, C4),
    E(0x1900, TEXT_MULT,   0xC0FF, WORD, M_DR,   M_GENI, C0, C4),
    E(0x1A00, TEXT_DIVL,   0xC0FF, LONG, M_DR,   M_GENI, C0, C4),
    E(0x1B00, TEXT_DIV,    0xC0FF, WORD, M_DR,   M_GENI, C0, C4),
    X(0x1C01, TEXT_LDM,    0xC0F0, WORD, M_R,    M_GENA, P8, C4, E1_CNT, E2_NO, P_0X0X),
    E(0x1C08, TEXT_TESTL,  0xC0F0, LONG, M_GEND, M_NO,   C4, NO),
    X(0x1C09, TEXT_LDM,    0xC0F0, WORD, M_GENA, M_R,    C4, P8, E1_CNT, E2_NO, P_0X0X),
    E(0x1D00, TEXT_LDL,    0xC0FF, LONG, M_GENA, M_R,    C4, C0),
    E(0x1E00, TEXT_JP,     0xC0FF, NONE, M_CC,   M_GENA, C0, C4),
    E(0x1E08, TEXT_JP,     0xC0F0, NONE, M_GENA, M_NO,   C4, NO),
    E(0x1F00, TEXT_CALL,   0xC0FF, NONE, M_GENA, M_NO,   C4, NO),
    E(0x2000, TEXT_LDB,    0xC0FF, BYTE, M_R,    M_GENI, C0, C4),
    E(0x2100, TEXT_LD,     0xC0FF, WORD, M_R,    M_GENI, C0, C4),
    X(0x2200, TEXT_RESB,   0x000F, BYTE, M_R,    M_WR07, P8, C0, E1_NO, E2_NO, P_0X00),
    E(0x2200, TEXT_RESB,   0xC0FF, BYTE, M_GEND, M_BIT,  C4, C0),
    X(0x2300, TEXT_RES,    0x000F, WORD, M_R,    M_R,    P8, C0, E1_NO, E2_NO, P_0X00),
    E(0x2300, TEXT_RES,    0xC0FF, WORD, M_GEND, M_BIT,  C4, C0),
    X(0x2400, TEXT_SETB,   0x000F, BYTE, M_R,    M_WR07, P8, C0, E1_NO, E2_NO, P_0X00),
    E(0x2400, TEXT_SETB,   0xC0FF, BYTE, M_GEND, M_BIT,  C4, C0),
    X(0x2500, TEXT_SET,    0x000F, WORD, M_R,    M_R,    P8, C0, E1_NO, E2_NO, P_0X00),
    E(0x2500, TEXT_SET,    0xC0FF, WORD, M_GEND, M_BIT,  C4, C0),
    X(0x2600, TEXT_BITB,   0x000F, BYTE, M_R,    M_WR07, P8, C0, E1_NO, E2_NO, P_0X00),
    E(0x2600, TEXT_BITB,   0xC0FF, BYTE, M_GEND, M_BIT,  C4, C0),
    X(0x2700, TEXT_BIT,    0x000F, WORD, M_R,    M_R,    P8, C0, E1_NO, E2_NO, P_0X00),
    E(0x2700, TEXT_BIT,    0xC0FF, WORD, M_GEND, M_BIT,  C4, C0),
    E(0x2800, TEXT_INCB,   0xC0FF, BYTE, M_GEND, M_CNT,  C4, C0),
    E(0x2900, TEXT_INC,    0xC0FF, WORD, M_GEND, M_CNT,  C4, C0),
    E(0x2A00, TEXT_DECB,   0xC0FF, BYTE, M_GEND, M_CNT,  C4, C0),
    E(0x2B00, TEXT_DEC,    0xC0FF, WORD, M_GEND, M_CNT,  C4, C0),
    E(0x2C00, TEXT_EXB,    0xC0FF, BYTE, M_R,    M_GEND, C0, C4),
    E(0x2D00, TEXT_EX,     0xC0FF, WORD, M_R,    M_GEND, C0, C4),
    E(0x2E00, TEXT_LDB,    0xC0FF, BYTE, M_GENA, M_R,    C4, C0),
    E(0x2F00, TEXT_LD,     0xC0FF, WORD, M_GENA, M_R,    C4, C0),
    E(0x3900, TEXT_LDPS,   0xC0F0, WORD, M_GENA, M_NO,   C4, NO),
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
    return opCode & entry->flags().codeMask();
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
        insn.setName_P(entry->name_P());
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
        insn.setName_P(entry->name_P());
    }
    return setError(entry ? OK : UNKNOWN_INSTRUCTION);
}

TableZ8000::TableZ8000() : _cpuType(Z8001) {}

const /* PROGMEM */ char *TableZ8000::listCpu_P() const {
    return TEXT_CPU_LIST;
}

const /* PROGMEM */ char *TableZ8000::cpu_P() const {
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

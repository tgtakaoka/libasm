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

#include "config_ns32000.h"
#include "entry_ns32000.h"
#include "table_ns32000.h"
#include "text_ns32000.h"

#include <string.h>

namespace libasm {
namespace ns32000 {

#define E(_opc, _name, _sz, _src, _dst, _spos, _dpos)           \
    {                                                           \
        _opc,                                                   \
        Entry::_flags(Entry::_opr(_src, _spos),                 \
                      Entry::_opr(_dst, _dpos),                 \
                      Entry::_opr(M_NONE, P_NONE),              \
                      Entry::_ex2(M_NONE, P_NONE, SZ_##_sz)),   \
        TEXT_##_name                                            \
    },
#define X(_opc, _name, _sz, _src, _dst, _spos, _dpos, _ex1m, _ex2m, _ex1p, _ex2p) \
    {                                                                   \
        _opc,                                                           \
        Entry::_flags(Entry::_opr(_src, _spos),                         \
                      Entry::_opr(_dst, _dpos),                         \
                      Entry::_opr(_ex1m, _ex1p),                        \
                      Entry::_ex2(_ex2m, _ex2p, SZ_##_sz)),             \
        TEXT_##_name                                                    \
    },

// Format 0: |cond|1010|
static const Entry FORMAT_0[] PROGMEM = {
    E(0x0A, BEQ, NONE, M_REL, M_NONE, P_DISP, P_NONE)
    E(0x1A, BNE, NONE, M_REL, M_NONE, P_DISP, P_NONE)
    E(0x2A, BCS, NONE, M_REL, M_NONE, P_DISP, P_NONE)
    E(0x3A, BCC, NONE, M_REL, M_NONE, P_DISP, P_NONE)
    E(0x4A, BHI, NONE, M_REL, M_NONE, P_DISP, P_NONE)
    E(0x5A, BLS, NONE, M_REL, M_NONE, P_DISP, P_NONE)
    E(0x6A, BGT, NONE, M_REL, M_NONE, P_DISP, P_NONE)
    E(0x7A, BLE, NONE, M_REL, M_NONE, P_DISP, P_NONE)
    E(0x8A, BFS, NONE, M_REL, M_NONE, P_DISP, P_NONE)
    E(0x9A, BFC, NONE, M_REL, M_NONE, P_DISP, P_NONE)
    E(0xAA, BLO, NONE, M_REL, M_NONE, P_DISP, P_NONE)
    E(0xBA, BHS, NONE, M_REL, M_NONE, P_DISP, P_NONE)
    E(0xCA, BLT, NONE, M_REL, M_NONE, P_DISP, P_NONE)
    E(0xDA, BGE, NONE, M_REL, M_NONE, P_DISP, P_NONE)
    E(0xEA, BR,  NONE, M_REL, M_NONE, P_DISP, P_NONE)
};

// Format 1: |_op_|0010|
static const Entry FORMAT_1[] PROGMEM = {
    E(0x02, BSR,     NONE, M_REL,  M_NONE, P_DISP, P_NONE)
    E(0x12, RET,     NONE, M_DISP, M_NONE, P_DISP, P_NONE)
    E(0x22, CXP,     NONE, M_DISP, M_NONE, P_DISP, P_NONE)
    E(0x32, RXP,     NONE, M_DISP, M_NONE, P_DISP, P_NONE)
    E(0x42, RETT,    NONE, M_DISP, M_NONE, P_DISP, P_NONE)
    E(0x52, RETI,    NONE, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0x62, SAVE,    NONE, M_PUSH, M_NONE, P_IMPL, P_NONE)
    E(0x72, RESTORE, NONE, M_POP,  M_NONE, P_IMPL, P_NONE)
    E(0x82, ENTER,   NONE, M_PUSH, M_DISP, P_IMPL, P_DISP)
    E(0x92, EXIT,    NONE, M_POP,  M_NONE, P_IMPL, P_NONE)
    E(0xA2, NOP,     NONE, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0xB2, WAIT,    NONE, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0xC2, DIA,     NONE, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0xD2, FLAG,    NONE, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0xE2, SVC,     NONE, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0xF2, BPT,     NONE, M_NONE, M_NONE, P_NONE, P_NONE)
};

// Format 4: |gen1_|gen| |2_|_op_|ii|
static const Entry FORMAT_4[] PROGMEM = {
    E(0x00, ADDB,   BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x01, ADDW,   WORD, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x03, ADDD,   LONG, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x04, CMPB,   BYTE, M_GENR, M_GENR, P_GEN1, P_GEN2)
    E(0x05, CMPW,   WORD, M_GENR, M_GENR, P_GEN1, P_GEN2)
    E(0x07, CMPD,   LONG, M_GENR, M_GENR, P_GEN1, P_GEN2)
    E(0x08, BICB,   BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x09, BICW,   WORD, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x0B, BICD,   LONG, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x10, ADDCB,  BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x11, ADDCW,  WORD, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x13, ADDCD,  LONG, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x14, MOVB,   BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x15, MOVW,   WORD, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x17, MOVD,   LONG, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x18, ORB,    BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x19, ORW,    WORD, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x1B, ORD,    LONG, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x20, SUBB,   BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x21, SUBW,   WORD, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x23, SUBD,   LONG, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x27, ADDR,   LONG, M_GENA, M_GENW, P_GEN1, P_GEN2)
    E(0x28, ANDB,   BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x29, ANDW,   WORD, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x2B, ANDD,   LONG, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x30, SUBCB,  BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x31, SUBCW,  WORD, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x33, SUBCD,  LONG, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x34, TBITB,  BYTE, M_GENR, M_GENA, P_GEN1, P_GEN2)
    E(0x35, TBITW,  WORD, M_GENR, M_GENA, P_GEN1, P_GEN2)
    E(0x37, TBITD,  LONG, M_GENR, M_GENA, P_GEN1, P_GEN2)
    E(0x38, XORB,   BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x39, XORW,   WORD, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x3B, XORD,   LONG, M_GENR, M_GENW, P_GEN1, P_GEN2)
};

// Format 2: |_gen_|sho| |t|_op|11|ii|
static const Entry FORMAT_2_0[] PROGMEM = {
    E(0x0C, ADDQB, BYTE, M_INT4, M_GENW, P_SHORT, P_GEN1)
    E(0x0D, ADDQW, WORD, M_INT4, M_GENW, P_SHORT, P_GEN1)
    E(0x0F, ADDQD, LONG, M_INT4, M_GENW, P_SHORT, P_GEN1)
    E(0x1C, CMPQB, BYTE, M_INT4, M_GENW, P_SHORT, P_GEN1)
    E(0x1D, CMPQW, WORD, M_INT4, M_GENW, P_SHORT, P_GEN1)
    E(0x1F, CMPQD, LONG, M_INT4, M_GENW, P_SHORT, P_GEN1)
    E(0x2C, SPRB,  BYTE, M_PREG, M_GENW, P_SHORT, P_GEN1)
    E(0x2D, SPRW,  WORD, M_PREG, M_GENW, P_SHORT, P_GEN1)
    E(0x2F, SPRD,  LONG, M_PREG, M_GENW, P_SHORT, P_GEN1)
    X(0x4C, ACBB,  BYTE, M_INT4, M_GENW, P_SHORT, P_GEN1, M_REL, M_NONE, P_DISP, P_NONE)
    X(0x4D, ACBW,  WORD, M_INT4, M_GENW, P_SHORT, P_GEN1, M_REL, M_NONE, P_DISP, P_NONE)
    X(0x4F, ACBD,  LONG, M_INT4, M_GENW, P_SHORT, P_GEN1, M_REL, M_NONE, P_DISP, P_NONE)
    E(0x5C, MOVQB, BYTE, M_INT4, M_GENW, P_SHORT, P_GEN1)
    E(0x5D, MOVQW, WORD, M_INT4, M_GENW, P_SHORT, P_GEN1)
    E(0x5F, MOVQD, LONG, M_INT4, M_GENW, P_SHORT, P_GEN1)
    E(0x6C, LPRB,  BYTE, M_PREG, M_GENR, P_SHORT, P_GEN1)
    E(0x6D, LPRW,  WORD, M_PREG, M_GENR, P_SHORT, P_GEN1)
    E(0x6F, LPRD,  LONG, M_PREG, M_GENR, P_SHORT, P_GEN1)
};
// Format 2: |_gen_|con| |d|011|11|ii|
static const Entry FORMAT_2_1[] PROGMEM = {
    E(0x00, SEQB, BYTE, M_GENW, M_NONE, P_GEN1, P_NONE)
    E(0x01, SCSB, BYTE, M_GENW, M_NONE, P_GEN1, P_NONE)
    E(0x02, SHIB, BYTE, M_GENW, M_NONE, P_GEN1, P_NONE)
    E(0x03, SGTB, BYTE, M_GENW, M_NONE, P_GEN1, P_NONE)
    E(0x04, SFSB, BYTE, M_GENW, M_NONE, P_GEN1, P_NONE)
    E(0x05, SLOB, BYTE, M_GENW, M_NONE, P_GEN1, P_NONE)
    E(0x06, SLTB, BYTE, M_GENW, M_NONE, P_GEN1, P_NONE)
};
static const Entry FORMAT_2_2[] PROGMEM = {
    E(0x00, SNEB, BYTE, M_GENW, M_NONE, P_GEN1, P_NONE)
    E(0x01, SCCB, BYTE, M_GENW, M_NONE, P_GEN1, P_NONE)
    E(0x02, SLSB, BYTE, M_GENW, M_NONE, P_GEN1, P_NONE)
    E(0x03, SLEB, BYTE, M_GENW, M_NONE, P_GEN1, P_NONE)
    E(0x04, SFCB, BYTE, M_GENW, M_NONE, P_GEN1, P_NONE)
    E(0x05, SHSB, BYTE, M_GENW, M_NONE, P_GEN1, P_NONE)
    E(0x06, SGEB, BYTE, M_GENW, M_NONE, P_GEN1, P_NONE)
};
static const Entry FORMAT_2_3[] PROGMEM = {
    E(0x00, SEQW, WORD, M_GENW, M_NONE, P_GEN1, P_NONE)
    E(0x01, SCSW, WORD, M_GENW, M_NONE, P_GEN1, P_NONE)
    E(0x02, SHIW, WORD, M_GENW, M_NONE, P_GEN1, P_NONE)
    E(0x03, SGTW, WORD, M_GENW, M_NONE, P_GEN1, P_NONE)
    E(0x04, SFSW, WORD, M_GENW, M_NONE, P_GEN1, P_NONE)
    E(0x05, SLOW, WORD, M_GENW, M_NONE, P_GEN1, P_NONE)
    E(0x06, SLTW, WORD, M_GENW, M_NONE, P_GEN1, P_NONE)
};
static const Entry FORMAT_2_4[] PROGMEM = {
    E(0x00, SNEW, WORD, M_GENW, M_NONE, P_GEN1, P_NONE)
    E(0x01, SCCW, WORD, M_GENW, M_NONE, P_GEN1, P_NONE)
    E(0x02, SLSW, WORD, M_GENW, M_NONE, P_GEN1, P_NONE)
    E(0x03, SLEW, WORD, M_GENW, M_NONE, P_GEN1, P_NONE)
    E(0x04, SFCW, WORD, M_GENW, M_NONE, P_GEN1, P_NONE)
    E(0x05, SHSW, WORD, M_GENW, M_NONE, P_GEN1, P_NONE)
    E(0x06, SGEW, WORD, M_GENW, M_NONE, P_GEN1, P_NONE)
};
static const Entry FORMAT_2_5[] PROGMEM = {
    E(0x00, SEQD, LONG, M_GENW, M_NONE, P_GEN1, P_NONE)
    E(0x01, SCSD, LONG, M_GENW, M_NONE, P_GEN1, P_NONE)
    E(0x02, SHID, LONG, M_GENW, M_NONE, P_GEN1, P_NONE)
    E(0x03, SGTD, LONG, M_GENW, M_NONE, P_GEN1, P_NONE)
    E(0x04, SFSD, LONG, M_GENW, M_NONE, P_GEN1, P_NONE)
    E(0x05, SLOD, LONG, M_GENW, M_NONE, P_GEN1, P_NONE)
    E(0x06, SLTD, LONG, M_GENW, M_NONE, P_GEN1, P_NONE)
};
static const Entry FORMAT_2_6[] PROGMEM = {
    E(0x00, SNED, LONG, M_GENW, M_NONE, P_GEN1, P_NONE)
    E(0x01, SCCD, LONG, M_GENW, M_NONE, P_GEN1, P_NONE)
    E(0x02, SLSD, LONG, M_GENW, M_NONE, P_GEN1, P_NONE)
    E(0x03, SLED, LONG, M_GENW, M_NONE, P_GEN1, P_NONE)
    E(0x04, SFCD, LONG, M_GENW, M_NONE, P_GEN1, P_NONE)
    E(0x05, SHSD, LONG, M_GENW, M_NONE, P_GEN1, P_NONE)
    E(0x06, SGED, LONG, M_GENW, M_NONE, P_GEN1, P_NONE)
};

// Format 3: |_gen_|_op| |011111|ii|
static const Entry FORMAT_3_1[] PROGMEM = {
    E(0x01, BICPSRB, BYTE, M_GENR, M_NONE, P_GEN1, P_NONE)
    E(0x03, BISPSRB, BYTE, M_GENR, M_NONE, P_GEN1, P_NONE)
    E(0x05, ADJSPB,  BYTE, M_GENR, M_NONE, P_GEN1, P_NONE)
    E(0x07, CASEB,   BYTE, M_GENR, M_NONE, P_GEN1, P_NONE)
};
static const Entry FORMAT_3_2[] PROGMEM = {
    E(0x01, BICPSRW, WORD, M_GENR, M_NONE, P_GEN1, P_NONE)
    E(0x03, BISPSRW, WORD, M_GENR, M_NONE, P_GEN1, P_NONE)
    E(0x05, ADJSPW,  WORD, M_GENR, M_NONE, P_GEN1, P_NONE)
    E(0x07, CASEW,   WORD, M_GENR, M_NONE, P_GEN1, P_NONE)
};
static const Entry FORMAT_3_3[] PROGMEM = {
    E(0x00, CXPD,    LONG, M_GENA, M_NONE, P_GEN1, P_NONE)
    E(0x02, JUMP,    LONG, M_GENA, M_NONE, P_GEN1, P_NONE)
    E(0x05, ADJSPD,  LONG, M_GENR, M_NONE, P_GEN1, P_NONE)
    E(0x06, JSR,     LONG, M_GENA, M_NONE, P_GEN1, P_NONE)
    E(0x07, CASED,   LONG, M_GENR, M_NONE, P_GEN1, P_NONE)
};

// Format 5: |00000|sho| |t|0|_op_|ii| |0000|1110|
static const Entry FORMAT_5_0[] PROGMEM = {
    E(0x0B, SETCFG, NONE, M_CONF, M_NONE, P_SHORT, P_NONE)
};
// Format 5: |00000|flg| |t|0|_op_|ii| |0000|1110|
static const Entry FORMAT_5_1[] PROGMEM = {
    E(0x00, MOVSB,  BYTE, M_SOPT, M_NONE, P_SHORT, P_NONE)
    E(0x01, MOVSW,  WORD, M_SOPT, M_NONE, P_SHORT, P_NONE)
    E(0x03, MOVSD,  LONG, M_SOPT, M_NONE, P_SHORT, P_NONE)
    E(0x80, MOVST,  BYTE, M_SOPT, M_NONE, P_SHORT, P_NONE)
    E(0x04, CMPSB,  BYTE, M_SOPT, M_NONE, P_SHORT, P_NONE)
    E(0x05, CMPSW,  WORD, M_SOPT, M_NONE, P_SHORT, P_NONE)
    E(0x07, CMPSD,  LONG, M_SOPT, M_NONE, P_SHORT, P_NONE)
    E(0x84, CMPST,  BYTE, M_SOPT, M_NONE, P_SHORT, P_NONE)
    E(0x0C, SKPSB,  BYTE, M_SOPT, M_NONE, P_SHORT, P_NONE)
    E(0x0D, SKPSW,  WORD, M_SOPT, M_NONE, P_SHORT, P_NONE)
    E(0x0F, SKPSD,  LONG, M_SOPT, M_NONE, P_SHORT, P_NONE)
    E(0x8C, SKPST,  BYTE, M_SOPT, M_NONE, P_SHORT, P_NONE)
    E(0x00, MOVSB,  BYTE, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0x01, MOVSW,  WORD, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0x03, MOVSD,  LONG, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0x80, MOVST,  BYTE, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0x04, CMPSB,  BYTE, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0x05, CMPSW,  WORD, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0x07, CMPSD,  LONG, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0x84, CMPST,  BYTE, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0x0C, SKPSB,  BYTE, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0x0D, SKPSW,  WORD, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0x0F, SKPSD,  LONG, M_NONE, M_NONE, P_NONE, P_NONE)
    E(0x8C, SKPST,  BYTE, M_NONE, M_NONE, P_NONE, P_NONE)
};

// Format 6: |gen1_|gen| |2_|_op_|ii| |0100|1110|
static const Entry FORMAT_6[] PROGMEM = {
    E(0x00, ROTB,   BYTE, M_GENC, M_GENW, P_GEN1, P_GEN2)
    E(0x01, ROTW,   WORD, M_GENC, M_GENW, P_GEN1, P_GEN2)
    E(0x03, ROTD,   LONG, M_GENC, M_GENW, P_GEN1, P_GEN2)
    E(0x04, ASHB,   BYTE, M_GENC, M_GENW, P_GEN1, P_GEN2)
    E(0x05, ASHW,   WORD, M_GENC, M_GENW, P_GEN1, P_GEN2)
    E(0x07, ASHD,   LONG, M_GENC, M_GENW, P_GEN1, P_GEN2)
    E(0x08, CBITB,  BYTE, M_GENR, M_GENA, P_GEN1, P_GEN2)
    E(0x09, CBITW,  WORD, M_GENR, M_GENA, P_GEN1, P_GEN2)
    E(0x0B, CBITD,  LONG, M_GENR, M_GENA, P_GEN1, P_GEN2)
    E(0x0C, CBITIB, BYTE, M_GENR, M_GENA, P_GEN1, P_GEN2)
    E(0x0D, CBITIW, WORD, M_GENR, M_GENA, P_GEN1, P_GEN2)
    E(0x0F, CBITID, LONG, M_GENR, M_GENA, P_GEN1, P_GEN2)
    E(0x14, LSHB,   BYTE, M_GENC, M_GENW, P_GEN1, P_GEN2)
    E(0x15, LSHW,   WORD, M_GENC, M_GENW, P_GEN1, P_GEN2)
    E(0x17, LSHD,   LONG, M_GENC, M_GENW, P_GEN1, P_GEN2)
    E(0x18, SBITB,  BYTE, M_GENR, M_GENA, P_GEN1, P_GEN2)
    E(0x19, SBITW,  WORD, M_GENR, M_GENA, P_GEN1, P_GEN2)
    E(0x1B, SBITD,  LONG, M_GENR, M_GENA, P_GEN1, P_GEN2)
    E(0x1C, SBITIB, BYTE, M_GENR, M_GENA, P_GEN1, P_GEN2)
    E(0x1D, SBITIW, WORD, M_GENR, M_GENA, P_GEN1, P_GEN2)
    E(0x1F, SBITID, LONG, M_GENR, M_GENA, P_GEN1, P_GEN2)
    E(0x20, NEGB,   BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x21, NEGW,   WORD, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x23, NEGD,   LONG, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x24, NOTB,   BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x25, NOTW,   WORD, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x27, NOTD,   LONG, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x2C, SUBPB,  BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x2D, SUBPW,  WORD, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x2F, SUBPD,  LONG, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x30, ABSB,   BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x31, ABSW,   WORD, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x33, ABSD,   LONG, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x34, COMB,   BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x35, COMW,   WORD, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x37, COMD,   LONG, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x38, IBITB,  BYTE, M_GENR, M_GENA, P_GEN1, P_GEN2)
    E(0x39, IBITW,  WORD, M_GENR, M_GENA, P_GEN1, P_GEN2)
    E(0x3B, IBITD,  LONG, M_GENR, M_GENA, P_GEN1, P_GEN2)
    E(0x3C, ADDPB,  BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x3D, ADDPW,  WORD, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x3F, ADDPD,  LONG, M_GENR, M_GENW, P_GEN1, P_GEN2)
};

// Format 7: |gen1_|gen| |2_|_op_|ii| |1100|1110|
static const Entry FORMAT_7[] PROGMEM = {
    X(0x00, MOVMB,  BYTE, M_GENA, M_GENA, P_GEN1, P_GEN2, M_LEN16, M_NONE,  P_DISP, P_NONE)
    X(0x01, MOVMW,  WORD, M_GENA, M_GENA, P_GEN1, P_GEN2, M_LEN8,  M_NONE,  P_DISP, P_NONE)
    X(0x03, MOVMD,  LONG, M_GENA, M_GENA, P_GEN1, P_GEN2, M_LEN4,  M_NONE,  P_DISP, P_NONE)
    X(0x04, CMPMB,  BYTE, M_GENA, M_GENA, P_GEN1, P_GEN2, M_LEN16, M_NONE,  P_DISP, P_NONE)
    X(0x05, CMPMW,  WORD, M_GENA, M_GENA, P_GEN1, P_GEN2, M_LEN8,  M_NONE,  P_DISP, P_NONE)
    X(0x07, CMPMD,  LONG, M_GENA, M_GENA, P_GEN1, P_GEN2, M_LEN4,  M_NONE,  P_DISP, P_NONE)
    X(0x08, INSSB,  BYTE, M_GENR, M_GENA, P_GEN1, P_GEN2, M_BFOFF, M_BFLEN, P_IMPL, P_IMPL)
    X(0x09, INSSW,  WORD, M_GENR, M_GENA, P_GEN1, P_GEN2, M_BFOFF, M_BFLEN, P_IMPL, P_IMPL)
    X(0x0B, INSSD,  LONG, M_GENR, M_GENA, P_GEN1, P_GEN2, M_BFOFF, M_BFLEN, P_IMPL, P_IMPL)
    X(0x0C, EXTSB,  BYTE, M_GENA, M_GENW, P_GEN1, P_GEN2, M_BFOFF, M_BFLEN, P_IMPL, P_IMPL)
    X(0x0D, EXTSW,  WORD, M_GENA, M_GENW, P_GEN1, P_GEN2, M_BFOFF, M_BFLEN, P_IMPL, P_IMPL)
    X(0x0F, EXTSD,  LONG, M_GENA, M_GENW, P_GEN1, P_GEN2, M_BFOFF, M_BFLEN, P_IMPL, P_IMPL)
    E(0x10, MOVXBW, BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x1C, MOVXBD, BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x1D, MOVXWD, WORD, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x14, MOVZBW, BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x18, MOVZBD, BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x19, MOVZWD, WORD, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x20, MULB,   BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x21, MULW,   WORD, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x23, MULD,   LONG, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x24, MEIB,   BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x25, MEIW,   WORD, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x27, MEID,   LONG, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x2C, DEIB,   BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x2D, DEIW,   WORD, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x2F, DEID,   LONG, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x30, QUOB,   BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x31, QUOW,   WORD, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x33, QUOD,   LONG, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x34, REMB,   BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x35, REMW,   WORD, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x37, REMD,   LONG, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x38, MODB,   BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x39, MODW,   WORD, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x3B, MODD,   LONG, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x3C, DIVB,   BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x3D, DIVW,   WORD, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x3F, DIVD,   LONG, M_GENR, M_GENW, P_GEN1, P_GEN2)
};

// Format 8: |gen1_|gen| |2_|reg|o|ii| |op|10|1110|
static const Entry FORMAT_8_1[] PROGMEM = {
    X(0x00, EXTB,   BYTE, M_GREG, M_GENA, P_REG, P_GEN1, M_GENW, M_LEN32, P_GEN2, P_DISP)
    X(0x01, EXTW,   WORD, M_GREG, M_GENA, P_REG, P_GEN1, M_GENW, M_LEN32, P_GEN2, P_DISP)
    X(0x03, EXTD,   LONG, M_GREG, M_GENA, P_REG, P_GEN1, M_GENW, M_LEN32, P_GEN2, P_DISP)
    X(0x04, INDEXB, BYTE, M_GREG, M_GENR, P_REG, P_GEN1, M_GENR, M_NONE,  P_GEN2, P_NONE)
    X(0x05, INDEXW, WORD, M_GREG, M_GENR, P_REG, P_GEN1, M_GENR, M_NONE,  P_GEN2, P_NONE)
    X(0x07, INDEXD, LONG, M_GREG, M_GENR, P_REG, P_GEN1, M_GENR, M_NONE,  P_GEN2, P_NONE)
};
static const Entry FORMAT_8_2[] PROGMEM = {
    X(0x03, CVTP, LONG, M_GREG, M_GENA, P_REG,  P_GEN1,  M_GENW, M_NONE,  P_GEN2, P_NONE)
    E(0x04, FFSB, BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x05, FFSW, WORD, M_GENR, M_GENW, P_GEN1, P_GEN2)
    E(0x07, FFSD, LONG, M_GENR, M_GENW, P_GEN1, P_GEN2)
};
static const Entry FORMAT_8_3[] PROGMEM = {
    X(0x00, INSB,   BYTE, M_GREG, M_GENR, P_REG, P_GEN1, M_GENA, M_LEN32, P_GEN2, P_DISP)
    X(0x01, INSW,   WORD, M_GREG, M_GENR, P_REG, P_GEN1, M_GENA, M_LEN32, P_GEN2, P_DISP)
    X(0x03, INSD,   LONG, M_GREG, M_GENR, P_REG, P_GEN1, M_GENA, M_LEN32, P_GEN2, P_DISP)
};
static const Entry FORMAT_8_3_1[] PROGMEM = {
    E(0x0C, MOVSUB, BYTE, M_GENA, M_GENA, P_GEN1, P_GEN2)
    E(0x0D, MOVSUW, WORD, M_GENA, M_GENA, P_GEN1, P_GEN2)
    E(0x0F, MOVSUD, LONG, M_GENA, M_GENA, P_GEN1, P_GEN2)
    E(0x1C, MOVUSB, BYTE, M_GENA, M_GENA, P_GEN1, P_GEN2)
    E(0x1D, MOVUSW, WORD, M_GENA, M_GENA, P_GEN1, P_GEN2)
    E(0x1F, MOVUSD, LONG, M_GENA, M_GENA, P_GEN1, P_GEN2)
};
static const Entry FORMAT_8_4[] PROGMEM = {
    X(0x00, CHECKB, BYTE, M_GREG, M_GENA, P_REG, P_GEN1, M_GENR, M_NONE, P_GEN2, P_NONE)
    X(0x01, CHECKW, WORD, M_GREG, M_GENA, P_REG, P_GEN1, M_GENR, M_NONE, P_GEN2, P_NONE)
    X(0x03, CHECKD, LONG, M_GREG, M_GENA, P_REG, P_GEN1, M_GENR, M_NONE, P_GEN2, P_NONE)
};

// Format 9: |gen1_|gen| |2_|_op|f|ii| |0011|1110|
static const Entry FORMAT_9[] PROGMEM = {
    E(0x00, MOVBL,   BYTE,   M_GENR, M_FENW, P_GEN1, P_GEN2)
    E(0x01, MOVWL,   WORD,   M_GENR, M_FENW, P_GEN1, P_GEN2)
    E(0x03, MOVDL,   LONG,   M_GENR, M_FENW, P_GEN1, P_GEN2)
    E(0x04, MOVBF,   BYTE,   M_GENR, M_FENW, P_GEN1, P_GEN2)
    E(0x05, MOVWF,   WORD,   M_GENR, M_FENW, P_GEN1, P_GEN2)
    E(0x07, MOVDF,   LONG,   M_GENR, M_FENW, P_GEN1, P_GEN2)
    E(0x0F, LFSR,    LONG,   M_GENR, M_NONE, P_GEN1, P_NONE)
    E(0x37, SFSR,    LONG,   M_GENW, M_NONE, P_GEN2, P_NONE)
    E(0x16, MOVLF,   DOUBLE, M_FENR, M_FENW, P_GEN1, P_GEN2)
    E(0x1B, MOVFL,   FLOAT,  M_FENR, M_FENW, P_GEN1, P_GEN2)
    E(0x20, ROUNDLB, DOUBLE, M_FENR, M_GENW, P_GEN1, P_GEN2)
    E(0x21, ROUNDLW, DOUBLE, M_FENR, M_GENW, P_GEN1, P_GEN2)
    E(0x23, ROUNDLD, DOUBLE, M_FENR, M_GENW, P_GEN1, P_GEN2)
    E(0x24, ROUNDFB, FLOAT,  M_FENR, M_GENW, P_GEN1, P_GEN2)
    E(0x25, ROUNDFW, FLOAT,  M_FENR, M_GENW, P_GEN1, P_GEN2)
    E(0x27, ROUNDFD, FLOAT,  M_FENR, M_GENW, P_GEN1, P_GEN2)
    E(0x28, TRUNCLB, DOUBLE, M_FENR, M_GENW, P_GEN1, P_GEN2)
    E(0x29, TRUNCLW, DOUBLE, M_FENR, M_GENW, P_GEN1, P_GEN2)
    E(0x2B, TRUNCLD, DOUBLE, M_FENR, M_GENW, P_GEN1, P_GEN2)
    E(0x2C, TRUNCFB, FLOAT,  M_FENR, M_GENW, P_GEN1, P_GEN2)
    E(0x2D, TRUNCFW, FLOAT,  M_FENR, M_GENW, P_GEN1, P_GEN2)
    E(0x2F, TRUNCFD, FLOAT,  M_FENR, M_GENW, P_GEN1, P_GEN2)
    E(0x38, FLOORLB, DOUBLE, M_FENR, M_GENW, P_GEN1, P_GEN2)
    E(0x39, FLOORLW, DOUBLE, M_FENR, M_GENW, P_GEN1, P_GEN2)
    E(0x3B, FLOORLD, DOUBLE, M_FENR, M_GENW, P_GEN1, P_GEN2)
    E(0x3C, FLOORFB, FLOAT,  M_FENR, M_GENW, P_GEN1, P_GEN2)
    E(0x3D, FLOORFW, FLOAT,  M_FENR, M_GENW, P_GEN1, P_GEN2)
    E(0x3F, FLOORFD, FLOAT,  M_FENR, M_GENW, P_GEN1, P_GEN2)
};

// Format 11: |gen1_|gen| |2_|_op_|0f| |1011|1110|
static const Entry FORMAT_11[] PROGMEM = {
    E(0x00, ADDL, DOUBLE, M_FENR, M_FENW, P_GEN1, P_GEN2)
    E(0x01, ADDF, FLOAT,  M_FENR, M_FENW, P_GEN1, P_GEN2)
    E(0x04, MOVL, DOUBLE, M_FENR, M_FENW, P_GEN1, P_GEN2)
    E(0x05, MOVF, FLOAT,  M_FENR, M_FENW, P_GEN1, P_GEN2)
    E(0x08, CMPL, DOUBLE, M_FENR, M_FENW, P_GEN1, P_GEN2)
    E(0x09, CMPF, FLOAT,  M_FENR, M_FENW, P_GEN1, P_GEN2)
    E(0x10, SUBL, DOUBLE, M_FENR, M_FENW, P_GEN1, P_GEN2)
    E(0x11, SUBF, FLOAT,  M_FENR, M_FENW, P_GEN1, P_GEN2)
    E(0x14, NEGL, DOUBLE, M_FENR, M_FENW, P_GEN1, P_GEN2)
    E(0x15, NEGF, FLOAT,  M_FENR, M_FENW, P_GEN1, P_GEN2)
    E(0x20, DIVL, DOUBLE, M_FENR, M_FENW, P_GEN1, P_GEN2)
    E(0x21, DIVF, FLOAT,  M_FENR, M_FENW, P_GEN1, P_GEN2)
    E(0x30, MULL, DOUBLE, M_FENR, M_FENW, P_GEN1, P_GEN2)
    E(0x31, MULF, FLOAT,  M_FENR, M_FENW, P_GEN1, P_GEN2)
    E(0x34, ABSL, DOUBLE, M_FENR, M_FENW, P_GEN1, P_GEN2)
    E(0x35, ABSF, FLOAT,  M_FENR, M_FENW, P_GEN1, P_GEN2)
};

// Format 14: |gen1_|sho| |t|0|_op_|ii| |0001|1110|
static const Entry FORMAT_14_1[] PROGMEM = {
    E(0x03, RDVAL, LONG, M_GENA, M_NONE, P_GEN1,  P_NONE)
    E(0x07, WRVAL, LONG, M_GENA, M_NONE, P_GEN1,  P_NONE)
};
static const Entry FORMAT_14_2[] PROGMEM = {
    E(0x0B, LMR,   LONG, M_MREG, M_GENR, P_SHORT, P_GEN1)
    E(0x0F, SMR,   LONG, M_MREG, M_GENW, P_SHORT, P_GEN1)
};

struct TableNs32000::EntryPage {
    const Config::opcode_t prefix;
    const Config::opcode_t mask;
    const uint8_t post;
    const Entry *const table;
    const Entry *const end;
};

static const TableNs32000::EntryPage NS32000_PAGES[] PROGMEM = {
    { 0x00, 0x00, 0, ARRAY_RANGE(FORMAT_0)   },
    { 0x00, 0x00, 0, ARRAY_RANGE(FORMAT_1)   },
    { 0x00, 0xC0, 1, ARRAY_RANGE(FORMAT_4)   },
    { 0x00, 0x80, 1, ARRAY_RANGE(FORMAT_2_0) },
    { 0x3C, 0xF8, 0, ARRAY_RANGE(FORMAT_2_1) },
    { 0xBC, 0xF8, 0, ARRAY_RANGE(FORMAT_2_2) },
    { 0x3D, 0xF8, 0, ARRAY_RANGE(FORMAT_2_3) },
    { 0xBD, 0xF8, 0, ARRAY_RANGE(FORMAT_2_4) },
    { 0x3F, 0xF8, 0, ARRAY_RANGE(FORMAT_2_5) },
    { 0xBF, 0xF8, 0, ARRAY_RANGE(FORMAT_2_6) },
    { 0x7C, 0xF8, 0, ARRAY_RANGE(FORMAT_3_1) },
    { 0x7D, 0xF8, 0, ARRAY_RANGE(FORMAT_3_2) },
    { 0x7F, 0xF8, 0, ARRAY_RANGE(FORMAT_3_3) },
    { 0x0E, 0x80, 1, ARRAY_RANGE(FORMAT_5_0) },
    { 0x0E, 0x00, 1, ARRAY_RANGE(FORMAT_5_1) },
    { 0x4E, 0xC0, 1, ARRAY_RANGE(FORMAT_6)   },
    { 0xCE, 0xC0, 1, ARRAY_RANGE(FORMAT_7)   },
    { 0x2E, 0xF8, 1, ARRAY_RANGE(FORMAT_8_1) },
    { 0x6E, 0xF8, 1, ARRAY_RANGE(FORMAT_8_2) },
    { 0xAE, 0xF8, 1, ARRAY_RANGE(FORMAT_8_3) },
    { 0xAE, 0xC0, 1, ARRAY_RANGE(FORMAT_8_3_1) },
    { 0xEE, 0xF8, 1, ARRAY_RANGE(FORMAT_8_4) },
    { 0x3E, 0xC0, 1, ARRAY_RANGE(FORMAT_9)   },
    { 0xBE, 0xC0, 1, ARRAY_RANGE(FORMAT_11)  },
    { 0x1E, 0x00, 1, ARRAY_RANGE(FORMAT_14_1) },
    { 0x1E, 0x80, 1, ARRAY_RANGE(FORMAT_14_2) },
};

bool TableNs32000::isPrefixCode(Config::opcode_t opCode) const {
    for (const EntryPage *page = ARRAY_BEGIN(NS32000_PAGES);
         page < ARRAY_END(NS32000_PAGES); page++) {
        const Config::opcode_t prefix = pgm_read_byte(&page->prefix);
        if (prefix == 0) continue;
        if (prefix == opCode) return true;
    }
    return false;
}

static bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table) return true;
    if (opr == M_GREG)
        return table == M_GENR || table == M_GENC || table == M_GENW
            || table == M_PUSH || table == M_POP;
    if (opr == M_RREL || opr == M_MREL || opr == M_ABS || opr == M_EXT
        || opr == M_TOS || opr == M_MEM)
        return table == M_GENR || table == M_GENC || table == M_GENW
            || table == M_FENR || table == M_FENW;
    if (opr == M_IMM)
        return table == M_GENR || table == M_GENC || table == M_FENR
            || table == M_DISP || table == M_INT4 || table == M_REL
            || table == M_BFOFF || table == M_BFLEN || table == M_LEN32
            || table == M_LEN16 || table == M_LEN8 || table == M_LEN4;
    if (opr == M_FREG) return table == M_FENR || table == M_FENW;
    if (opr == M_PUSH) return table == M_POP;
    return false;
}

static bool acceptModes(uint32_t flags, const Entry *entry) {
    const uint32_t table = pgm_read_dword(&entry->flags);
    return acceptMode(Entry::_mode(Entry::_src(flags)), Entry::_mode(Entry::_src(table)))
        && acceptMode(Entry::_mode(Entry::_dst(flags)), Entry::_mode(Entry::_dst(table)))
        && acceptMode(Entry::_mode(Entry::_ex1(flags)), Entry::_mode(Entry::_ex1(table)))
        && acceptMode(Entry::_ex2Mode(Entry::_ex2(flags)), Entry::_ex2Mode(Entry::_ex2(table)));
}

Error TableNs32000::searchName(
    InsnNs32000 &insn, const EntryPage *pages, const EntryPage *end) const {
    uint8_t count = 0;
    const uint32_t flags = Entry::_flags(
            Entry::_opr(insn.srcMode(), P_NONE),
            Entry::_opr(insn.dstMode(), P_NONE),
            Entry::_opr(insn.ex1Mode(), P_NONE),
            Entry::_ex2(insn.ex2Mode(), P_NONE, SZ_NONE));
    for (const EntryPage *page = pages; page < end; page++) {
        const uint8_t post = pgm_read_byte(&page->post);
        const Entry *table = reinterpret_cast<Entry *>(pgm_read_ptr(&page->table));
        const Entry *end = reinterpret_cast<Entry *>(pgm_read_ptr(&page->end));
        const Entry *entry = TableBase::searchName<Entry, uint32_t>(
                insn.name(), flags, table, end, acceptModes, count);
        if (entry) {
            const Config::opcode_t prefix = pgm_read_byte(&page->prefix);
            insn.setOpCode(pgm_read_byte(&entry->opCode), prefix);
            insn.setFlags(pgm_read_dword(&entry->flags));
            if (post) insn.setHasPost();
            return OK;
        }
    }
    return count == 0 ? UNKNOWN_INSTRUCTION : OPERAND_NOT_ALLOWED;
}

Error TableNs32000::searchOpCode(
    InsnNs32000 &insn, DisMemory &memory,
    const EntryPage *pages, const EntryPage *end) const {
    for (const EntryPage *page = pages; page < end; page++) {
        const Config::opcode_t prefix = pgm_read_byte(&page->prefix);
        if (insn.prefix() != prefix) continue;
        const Config::opcode_t mask = ~pgm_read_byte(&page->mask);
        const uint8_t post = pgm_read_byte(&page->post);
        const Entry *table = reinterpret_cast<Entry *>(pgm_read_ptr(&page->table));
        const Entry *end = reinterpret_cast<Entry *>(pgm_read_ptr(&page->end));
        const Entry *entry = TableBase::searchCode<Entry,Config::opcode_t>(
            insn.opCode() & mask, table, end);
        if (entry) {
            insn.setFlags(pgm_read_dword(&entry->flags));
            if (post) {
                if (insn.readPost(memory)) return NO_MEMORY;
                insn.setHasPost();
            }
            const char *name =
                reinterpret_cast<const char *>(pgm_read_ptr(&entry->name));
            TableBase::setName(insn.insn(), name, Config::NAME_MAX);
            return OK;
        }
    }
    return UNKNOWN_INSTRUCTION;
}

Error TableNs32000::searchName(InsnNs32000 &insn) const {
    return _error.setError(searchName(insn, ARRAY_RANGE(NS32000_PAGES)));
}

Error TableNs32000::searchOpCode(InsnNs32000 &insn, DisMemory &memory) const {
    return _error.setError(
        searchOpCode(insn, memory, ARRAY_RANGE(NS32000_PAGES)));
}

const char *TableNs32000::listCpu() {
    return getCpu();
}

const char *TableNs32000::getCpu() {
    return "NS32032";
}

bool TableNs32000::setCpu(const char *cpu) {
    return strcasecmp(cpu, "NS32032") == 0
        || strcasecmp(cpu, "NS32016") == 0
        || strcasecmp(cpu, "NS32008") == 0;
}

class TableNs32000 TableNs32000;

} // namespace ns32000
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

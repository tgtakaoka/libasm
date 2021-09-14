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

#include "table_ns32000.h"
#include "config_ns32000.h"
#include "entry_ns32000.h"
#include "text_ns32000.h"

#include <string.h>

namespace libasm {
namespace ns32000 {

#define X(_opc, _name, _sz, _srcm, _dstm, _srcp, _dstp, _ex1m, _ex2m, _ex1p, _ex2p)        \
    {                                                                                      \
        _opc,                                                                              \
                Entry::Flags::create(                                                      \
                        _srcm, _srcp, _dstm, _dstp, _ex1m, _ex1p, _ex2m, _ex2p, SZ_##_sz), \
                _name                                                                      \
    }
#define E(_opc, _name, _sz, _srcm, _dstm, _srcp, _dstp) \
    X(_opc, _name, _sz, _srcm, _dstm, _srcp, _dstp, M_NONE, M_NONE, P_NONE, P_NONE)

// clang-format off
// Format 0: |cond|1010|
static const Entry FORMAT_0[] PROGMEM = {
    E(0x0A, TEXT_BEQ, NONE, M_REL, M_NONE, P_DISP, P_NONE),
    E(0x1A, TEXT_BNE, NONE, M_REL, M_NONE, P_DISP, P_NONE),
    E(0x2A, TEXT_BCS, NONE, M_REL, M_NONE, P_DISP, P_NONE),
    E(0x3A, TEXT_BCC, NONE, M_REL, M_NONE, P_DISP, P_NONE),
    E(0x4A, TEXT_BHI, NONE, M_REL, M_NONE, P_DISP, P_NONE),
    E(0x5A, TEXT_BLS, NONE, M_REL, M_NONE, P_DISP, P_NONE),
    E(0x6A, TEXT_BGT, NONE, M_REL, M_NONE, P_DISP, P_NONE),
    E(0x7A, TEXT_BLE, NONE, M_REL, M_NONE, P_DISP, P_NONE),
    E(0x8A, TEXT_BFS, NONE, M_REL, M_NONE, P_DISP, P_NONE),
    E(0x9A, TEXT_BFC, NONE, M_REL, M_NONE, P_DISP, P_NONE),
    E(0xAA, TEXT_BLO, NONE, M_REL, M_NONE, P_DISP, P_NONE),
    E(0xBA, TEXT_BHS, NONE, M_REL, M_NONE, P_DISP, P_NONE),
    E(0xCA, TEXT_BLT, NONE, M_REL, M_NONE, P_DISP, P_NONE),
    E(0xDA, TEXT_BGE, NONE, M_REL, M_NONE, P_DISP, P_NONE),
    E(0xEA, TEXT_BR,  NONE, M_REL, M_NONE, P_DISP, P_NONE),
};

// Format 1: |_op_|0010|
static const Entry FORMAT_1[] PROGMEM = {
    E(0x02, TEXT_BSR,     NONE,   M_REL,  M_NONE, P_DISP, P_NONE),
    E(0x12, TEXT_RET,     NONE,   M_DISP, M_NONE, P_DISP, P_NONE),
    E(0x22, TEXT_CXP,     NONE,   M_DISP, M_NONE, P_DISP, P_NONE),
    E(0x32, TEXT_RXP,     NONE,   M_DISP, M_NONE, P_DISP, P_NONE),
    E(0x42, TEXT_RETT,    NONE,   M_DISP, M_NONE, P_DISP, P_NONE),
    E(0x52, TEXT_RETI,    NONE,   M_NONE, M_NONE, P_NONE, P_NONE),
    E(0x62, TEXT_SAVE,    NONE,   M_RLST, M_NONE, P_IMPL, P_NONE),
    E(0x72, TEXT_RESTORE, DOUBLE, M_RLST, M_NONE, P_IMPL, P_NONE),
    E(0x82, TEXT_ENTER,   NONE,   M_RLST, M_DISP, P_IMPL, P_DISP),
    E(0x92, TEXT_EXIT,    DOUBLE, M_RLST, M_NONE, P_IMPL, P_NONE),
    E(0xA2, TEXT_NOP,     NONE,   M_NONE, M_NONE, P_NONE, P_NONE),
    E(0xB2, TEXT_WAIT,    NONE,   M_NONE, M_NONE, P_NONE, P_NONE),
    E(0xC2, TEXT_DIA,     NONE,   M_NONE, M_NONE, P_NONE, P_NONE),
    E(0xD2, TEXT_FLAG,    NONE,   M_NONE, M_NONE, P_NONE, P_NONE),
    E(0xE2, TEXT_SVC,     NONE,   M_NONE, M_NONE, P_NONE, P_NONE),
    E(0xF2, TEXT_BPT,     NONE,   M_NONE, M_NONE, P_NONE, P_NONE),
};

// Format 4: |gen1_|gen| |2_|_op_|ii|
static const Entry FORMAT_4[] PROGMEM = {
    E(0x00, TEXT_ADDB,   BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x01, TEXT_ADDW,   WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x03, TEXT_ADDD,   DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x04, TEXT_CMPB,   BYTE,   M_GENR, M_GENR, P_GEN1, P_GEN2),
    E(0x05, TEXT_CMPW,   WORD,   M_GENR, M_GENR, P_GEN1, P_GEN2),
    E(0x07, TEXT_CMPD,   DOUBLE, M_GENR, M_GENR, P_GEN1, P_GEN2),
    E(0x08, TEXT_BICB,   BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x09, TEXT_BICW,   WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x0B, TEXT_BICD,   DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x10, TEXT_ADDCB,  BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x11, TEXT_ADDCW,  WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x13, TEXT_ADDCD,  DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x14, TEXT_MOVB,   BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x15, TEXT_MOVW,   WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x17, TEXT_MOVD,   DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x18, TEXT_ORB,    BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x19, TEXT_ORW,    WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x1B, TEXT_ORD,    DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x20, TEXT_SUBB,   BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x21, TEXT_SUBW,   WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x23, TEXT_SUBD,   DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x27, TEXT_ADDR,   DOUBLE, M_GENA, M_GENW, P_GEN1, P_GEN2),
    E(0x28, TEXT_ANDB,   BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x29, TEXT_ANDW,   WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x2B, TEXT_ANDD,   DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x30, TEXT_SUBCB,  BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x31, TEXT_SUBCW,  WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x33, TEXT_SUBCD,  DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x34, TEXT_TBITB,  BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x35, TEXT_TBITW,  WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x37, TEXT_TBITD,  DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x38, TEXT_XORB,   BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x39, TEXT_XORW,   WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x3B, TEXT_XORD,   DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
};

// Format 2: |_gen_|sho| |t|_op|11|ii|
static const Entry FORMAT_2_0[] PROGMEM = {
    E(0x0C, TEXT_ADDQB, BYTE,   M_INT4, M_GENW, P_SHORT, P_GEN1),
    E(0x0D, TEXT_ADDQW, WORD,   M_INT4, M_GENW, P_SHORT, P_GEN1),
    E(0x0F, TEXT_ADDQD, DOUBLE, M_INT4, M_GENW, P_SHORT, P_GEN1),
    E(0x1C, TEXT_CMPQB, BYTE,   M_INT4, M_GENR, P_SHORT, P_GEN1),
    E(0x1D, TEXT_CMPQW, WORD,   M_INT4, M_GENR, P_SHORT, P_GEN1),
    E(0x1F, TEXT_CMPQD, DOUBLE, M_INT4, M_GENR, P_SHORT, P_GEN1),
    E(0x2C, TEXT_SPRB,  BYTE,   M_PREG, M_GENW, P_SHORT, P_GEN1),
    E(0x2D, TEXT_SPRW,  WORD,   M_PREG, M_GENW, P_SHORT, P_GEN1),
    E(0x2F, TEXT_SPRD,  DOUBLE, M_PREG, M_GENW, P_SHORT, P_GEN1),
    X(0x4C, TEXT_ACBB,  BYTE,   M_INT4, M_GENW, P_SHORT, P_GEN1, M_REL, M_NONE, P_DISP, P_NONE),
    X(0x4D, TEXT_ACBW,  WORD,   M_INT4, M_GENW, P_SHORT, P_GEN1, M_REL, M_NONE, P_DISP, P_NONE),
    X(0x4F, TEXT_ACBD,  DOUBLE, M_INT4, M_GENW, P_SHORT, P_GEN1, M_REL, M_NONE, P_DISP, P_NONE),
    E(0x5C, TEXT_MOVQB, BYTE,   M_INT4, M_GENW, P_SHORT, P_GEN1),
    E(0x5D, TEXT_MOVQW, WORD,   M_INT4, M_GENW, P_SHORT, P_GEN1),
    E(0x5F, TEXT_MOVQD, DOUBLE, M_INT4, M_GENW, P_SHORT, P_GEN1),
    E(0x6C, TEXT_LPRB,  BYTE,   M_PREG, M_GENR, P_SHORT, P_GEN1),
    E(0x6D, TEXT_LPRW,  WORD,   M_PREG, M_GENR, P_SHORT, P_GEN1),
    E(0x6F, TEXT_LPRD,  DOUBLE, M_PREG, M_GENR, P_SHORT, P_GEN1),
};
// Format 2: |_gen_|con| |d|011|11|ii|
static const Entry FORMAT_2_1[] PROGMEM = {
    E(0x00, TEXT_SEQB, BYTE, M_GENW, M_NONE, P_GEN1, P_NONE),
    E(0x01, TEXT_SCSB, BYTE, M_GENW, M_NONE, P_GEN1, P_NONE),
    E(0x02, TEXT_SHIB, BYTE, M_GENW, M_NONE, P_GEN1, P_NONE),
    E(0x03, TEXT_SGTB, BYTE, M_GENW, M_NONE, P_GEN1, P_NONE),
    E(0x04, TEXT_SFSB, BYTE, M_GENW, M_NONE, P_GEN1, P_NONE),
    E(0x05, TEXT_SLOB, BYTE, M_GENW, M_NONE, P_GEN1, P_NONE),
    E(0x06, TEXT_SLTB, BYTE, M_GENW, M_NONE, P_GEN1, P_NONE),
};
static const Entry FORMAT_2_2[] PROGMEM = {
    E(0x00, TEXT_SNEB, BYTE, M_GENW, M_NONE, P_GEN1, P_NONE),
    E(0x01, TEXT_SCCB, BYTE, M_GENW, M_NONE, P_GEN1, P_NONE),
    E(0x02, TEXT_SLSB, BYTE, M_GENW, M_NONE, P_GEN1, P_NONE),
    E(0x03, TEXT_SLEB, BYTE, M_GENW, M_NONE, P_GEN1, P_NONE),
    E(0x04, TEXT_SFCB, BYTE, M_GENW, M_NONE, P_GEN1, P_NONE),
    E(0x05, TEXT_SHSB, BYTE, M_GENW, M_NONE, P_GEN1, P_NONE),
    E(0x06, TEXT_SGEB, BYTE, M_GENW, M_NONE, P_GEN1, P_NONE),
};
static const Entry FORMAT_2_3[] PROGMEM = {
    E(0x00, TEXT_SEQW, WORD, M_GENW, M_NONE, P_GEN1, P_NONE),
    E(0x01, TEXT_SCSW, WORD, M_GENW, M_NONE, P_GEN1, P_NONE),
    E(0x02, TEXT_SHIW, WORD, M_GENW, M_NONE, P_GEN1, P_NONE),
    E(0x03, TEXT_SGTW, WORD, M_GENW, M_NONE, P_GEN1, P_NONE),
    E(0x04, TEXT_SFSW, WORD, M_GENW, M_NONE, P_GEN1, P_NONE),
    E(0x05, TEXT_SLOW, WORD, M_GENW, M_NONE, P_GEN1, P_NONE),
    E(0x06, TEXT_SLTW, WORD, M_GENW, M_NONE, P_GEN1, P_NONE),
};
static const Entry FORMAT_2_4[] PROGMEM = {
    E(0x00, TEXT_SNEW, WORD, M_GENW, M_NONE, P_GEN1, P_NONE),
    E(0x01, TEXT_SCCW, WORD, M_GENW, M_NONE, P_GEN1, P_NONE),
    E(0x02, TEXT_SLSW, WORD, M_GENW, M_NONE, P_GEN1, P_NONE),
    E(0x03, TEXT_SLEW, WORD, M_GENW, M_NONE, P_GEN1, P_NONE),
    E(0x04, TEXT_SFCW, WORD, M_GENW, M_NONE, P_GEN1, P_NONE),
    E(0x05, TEXT_SHSW, WORD, M_GENW, M_NONE, P_GEN1, P_NONE),
    E(0x06, TEXT_SGEW, WORD, M_GENW, M_NONE, P_GEN1, P_NONE),
};
static const Entry FORMAT_2_5[] PROGMEM = {
    E(0x00, TEXT_SEQD, DOUBLE, M_GENW, M_NONE, P_GEN1, P_NONE),
    E(0x01, TEXT_SCSD, DOUBLE, M_GENW, M_NONE, P_GEN1, P_NONE),
    E(0x02, TEXT_SHID, DOUBLE, M_GENW, M_NONE, P_GEN1, P_NONE),
    E(0x03, TEXT_SGTD, DOUBLE, M_GENW, M_NONE, P_GEN1, P_NONE),
    E(0x04, TEXT_SFSD, DOUBLE, M_GENW, M_NONE, P_GEN1, P_NONE),
    E(0x05, TEXT_SLOD, DOUBLE, M_GENW, M_NONE, P_GEN1, P_NONE),
    E(0x06, TEXT_SLTD, DOUBLE, M_GENW, M_NONE, P_GEN1, P_NONE),
};
static const Entry FORMAT_2_6[] PROGMEM = {
    E(0x00, TEXT_SNED, DOUBLE, M_GENW, M_NONE, P_GEN1, P_NONE),
    E(0x01, TEXT_SCCD, DOUBLE, M_GENW, M_NONE, P_GEN1, P_NONE),
    E(0x02, TEXT_SLSD, DOUBLE, M_GENW, M_NONE, P_GEN1, P_NONE),
    E(0x03, TEXT_SLED, DOUBLE, M_GENW, M_NONE, P_GEN1, P_NONE),
    E(0x04, TEXT_SFCD, DOUBLE, M_GENW, M_NONE, P_GEN1, P_NONE),
    E(0x05, TEXT_SHSD, DOUBLE, M_GENW, M_NONE, P_GEN1, P_NONE),
    E(0x06, TEXT_SGED, DOUBLE, M_GENW, M_NONE, P_GEN1, P_NONE),
};

// Format 3: |_gen_|_op| |011111|ii|
static const Entry FORMAT_3_1[] PROGMEM = {
    E(0x01, TEXT_BICPSRB, BYTE, M_GENR, M_NONE, P_GEN1, P_NONE),
    E(0x03, TEXT_BISPSRB, BYTE, M_GENR, M_NONE, P_GEN1, P_NONE),
    E(0x05, TEXT_ADJSPB,  BYTE, M_GENR, M_NONE, P_GEN1, P_NONE),
    E(0x07, TEXT_CASEB,   BYTE, M_GENR, M_NONE, P_GEN1, P_NONE),
};
static const Entry FORMAT_3_2[] PROGMEM = {
    E(0x01, TEXT_BICPSRW, WORD, M_GENR, M_NONE, P_GEN1, P_NONE),
    E(0x03, TEXT_BISPSRW, WORD, M_GENR, M_NONE, P_GEN1, P_NONE),
    E(0x05, TEXT_ADJSPW,  WORD, M_GENR, M_NONE, P_GEN1, P_NONE),
    E(0x07, TEXT_CASEW,   WORD, M_GENR, M_NONE, P_GEN1, P_NONE),
};
static const Entry FORMAT_3_3[] PROGMEM = {
    E(0x00, TEXT_CXPD,   DOUBLE, M_GENA, M_NONE, P_GEN1, P_NONE),
    E(0x02, TEXT_JUMP,   DOUBLE, M_GENA, M_NONE, P_GEN1, P_NONE),
    E(0x05, TEXT_ADJSPD, DOUBLE, M_GENR, M_NONE, P_GEN1, P_NONE),
    E(0x06, TEXT_JSR,    DOUBLE, M_GENA, M_NONE, P_GEN1, P_NONE),
    E(0x07, TEXT_CASED,  DOUBLE, M_GENR, M_NONE, P_GEN1, P_NONE),
};

// Format 5: |00000|sho| |t|0|_op_|ii| |0000|1110|
static const Entry FORMAT_5_0[] PROGMEM = {
    E(0x0B, TEXT_SETCFG, NONE, M_CONF, M_NONE, P_SHORT, P_NONE),
};
// Format 5: |00000|flg| |t|0|_op_|ii| |0000|1110|
static const Entry FORMAT_5_1[] PROGMEM = {
    E(0x00, TEXT_MOVSB, BYTE,   M_SOPT, M_NONE, P_SHORT, P_NONE),
    E(0x01, TEXT_MOVSW, WORD,   M_SOPT, M_NONE, P_SHORT, P_NONE),
    E(0x03, TEXT_MOVSD, DOUBLE, M_SOPT, M_NONE, P_SHORT, P_NONE),
    E(0x80, TEXT_MOVST, BYTE,   M_SOPT, M_NONE, P_SHORT, P_NONE),
    E(0x04, TEXT_CMPSB, BYTE,   M_SOPT, M_NONE, P_SHORT, P_NONE),
    E(0x05, TEXT_CMPSW, WORD,   M_SOPT, M_NONE, P_SHORT, P_NONE),
    E(0x07, TEXT_CMPSD, DOUBLE, M_SOPT, M_NONE, P_SHORT, P_NONE),
    E(0x84, TEXT_CMPST, BYTE,   M_SOPT, M_NONE, P_SHORT, P_NONE),
    E(0x0C, TEXT_SKPSB, BYTE,   M_SOPT, M_NONE, P_SHORT, P_NONE),
    E(0x0D, TEXT_SKPSW, WORD,   M_SOPT, M_NONE, P_SHORT, P_NONE),
    E(0x0F, TEXT_SKPSD, DOUBLE, M_SOPT, M_NONE, P_SHORT, P_NONE),
    E(0x8C, TEXT_SKPST, BYTE,   M_SOPT, M_NONE, P_SHORT, P_NONE),
};

// Format 6: |gen1_|gen| |2_|_op_|ii| |0100|1110|
static const Entry FORMAT_6[] PROGMEM = {
    E(0x00, TEXT_ROTB,   BYTE,   M_GENC, M_GENW, P_GEN1, P_GEN2),
    E(0x01, TEXT_ROTW,   WORD,   M_GENC, M_GENW, P_GEN1, P_GEN2),
    E(0x03, TEXT_ROTD,   DOUBLE, M_GENC, M_GENW, P_GEN1, P_GEN2),
    E(0x04, TEXT_ASHB,   BYTE,   M_GENC, M_GENW, P_GEN1, P_GEN2),
    E(0x05, TEXT_ASHW,   WORD,   M_GENC, M_GENW, P_GEN1, P_GEN2),
    E(0x07, TEXT_ASHD,   DOUBLE, M_GENC, M_GENW, P_GEN1, P_GEN2),
    E(0x08, TEXT_CBITB,  BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x09, TEXT_CBITW,  WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x0B, TEXT_CBITD,  DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x0C, TEXT_CBITIB, BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x0D, TEXT_CBITIW, WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x0F, TEXT_CBITID, DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x14, TEXT_LSHB,   BYTE,   M_GENC, M_GENW, P_GEN1, P_GEN2),
    E(0x15, TEXT_LSHW,   WORD,   M_GENC, M_GENW, P_GEN1, P_GEN2),
    E(0x17, TEXT_LSHD,   DOUBLE, M_GENC, M_GENW, P_GEN1, P_GEN2),
    E(0x18, TEXT_SBITB,  BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x19, TEXT_SBITW,  WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x1B, TEXT_SBITD,  DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x1C, TEXT_SBITIB, BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x1D, TEXT_SBITIW, WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x1F, TEXT_SBITID, DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x20, TEXT_NEGB,   BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x21, TEXT_NEGW,   WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x23, TEXT_NEGD,   DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x24, TEXT_NOTB,   BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x25, TEXT_NOTW,   WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x27, TEXT_NOTD,   DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x2C, TEXT_SUBPB,  BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x2D, TEXT_SUBPW,  WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x2F, TEXT_SUBPD,  DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x30, TEXT_ABSB,   BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x31, TEXT_ABSW,   WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x33, TEXT_ABSD,   DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x34, TEXT_COMB,   BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x35, TEXT_COMW,   WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x37, TEXT_COMD,   DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x38, TEXT_IBITB,  BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x39, TEXT_IBITW,  WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x3B, TEXT_IBITD,  DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x3C, TEXT_ADDPB,  BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x3D, TEXT_ADDPW,  WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x3F, TEXT_ADDPD,  DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
};

// Format 7: |gen1_|gen| |2_|_op_|ii| |1100|1110|
static const Entry FORMAT_7[] PROGMEM = {
    X(0x00, TEXT_MOVMB,  BYTE,   M_GENA, M_GENA, P_GEN1, P_GEN2, M_LEN16, M_NONE,  P_DISP, P_NONE),
    X(0x01, TEXT_MOVMW,  WORD,   M_GENA, M_GENA, P_GEN1, P_GEN2, M_LEN8,  M_NONE,  P_DISP, P_NONE),
    X(0x03, TEXT_MOVMD,  DOUBLE, M_GENA, M_GENA, P_GEN1, P_GEN2, M_LEN4,  M_NONE,  P_DISP, P_NONE),
    X(0x04, TEXT_CMPMB,  BYTE,   M_GENA, M_GENA, P_GEN1, P_GEN2, M_LEN16, M_NONE,  P_DISP, P_NONE),
    X(0x05, TEXT_CMPMW,  WORD,   M_GENA, M_GENA, P_GEN1, P_GEN2, M_LEN8,  M_NONE,  P_DISP, P_NONE),
    X(0x07, TEXT_CMPMD,  DOUBLE, M_GENA, M_GENA, P_GEN1, P_GEN2, M_LEN4,  M_NONE,  P_DISP, P_NONE),
    X(0x08, TEXT_INSSB,  BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2, M_BFOFF, M_BFLEN, P_IMPL, P_IMPL),
    X(0x09, TEXT_INSSW,  WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2, M_BFOFF, M_BFLEN, P_IMPL, P_IMPL),
    X(0x0B, TEXT_INSSD,  DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2, M_BFOFF, M_BFLEN, P_IMPL, P_IMPL),
    X(0x0C, TEXT_EXTSB,  BYTE,   M_GENW, M_GENW, P_GEN1, P_GEN2, M_BFOFF, M_BFLEN, P_IMPL, P_IMPL),
    X(0x0D, TEXT_EXTSW,  WORD,   M_GENW, M_GENW, P_GEN1, P_GEN2, M_BFOFF, M_BFLEN, P_IMPL, P_IMPL),
    X(0x0F, TEXT_EXTSD,  DOUBLE, M_GENW, M_GENW, P_GEN1, P_GEN2, M_BFOFF, M_BFLEN, P_IMPL, P_IMPL),
    E(0x10, TEXT_MOVXBW, BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x1C, TEXT_MOVXBD, BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x1D, TEXT_MOVXWD, WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x14, TEXT_MOVZBW, BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x18, TEXT_MOVZBD, BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x19, TEXT_MOVZWD, WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x20, TEXT_MULB,   BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x21, TEXT_MULW,   WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x23, TEXT_MULD,   DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    // gen2 must be register pair.
    X(0x24, TEXT_MEIB,   BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2, M_NONE, M_NONE, P_NONE, P_IMPL),
    X(0x25, TEXT_MEIW,   WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2, M_NONE, M_NONE, P_NONE, P_IMPL),
    X(0x27, TEXT_MEID,   DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2, M_NONE, M_NONE, P_NONE, P_IMPL),
    X(0x2C, TEXT_DEIB,   BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2, M_NONE, M_NONE, P_NONE, P_IMPL),
    X(0x2D, TEXT_DEIW,   WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2, M_NONE, M_NONE, P_NONE, P_IMPL),
    X(0x2F, TEXT_DEID,   DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2, M_NONE, M_NONE, P_NONE, P_IMPL),
    E(0x30, TEXT_QUOB,   BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x31, TEXT_QUOW,   WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x33, TEXT_QUOD,   DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x34, TEXT_REMB,   BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x35, TEXT_REMW,   WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x37, TEXT_REMD,   DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x38, TEXT_MODB,   BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x39, TEXT_MODW,   WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x3B, TEXT_MODD,   DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x3C, TEXT_DIVB,   BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x3D, TEXT_DIVW,   WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x3F, TEXT_DIVD,   DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
};

// Format 8: |gen1_|gen| |2_|reg|o|ii| |op|10|1110|
static const Entry FORMAT_8_1[] PROGMEM = {
    X(0x00, TEXT_EXTB,   BYTE,   M_GREG, M_GENW, P_REG, P_GEN1, M_GENW, M_LEN32, P_GEN2, P_DISP),
    X(0x01, TEXT_EXTW,   WORD,   M_GREG, M_GENW, P_REG, P_GEN1, M_GENW, M_LEN32, P_GEN2, P_DISP),
    X(0x03, TEXT_EXTD,   DOUBLE, M_GREG, M_GENW, P_REG, P_GEN1, M_GENW, M_LEN32, P_GEN2, P_DISP),
    X(0x04, TEXT_INDEXB, BYTE,   M_GREG, M_GENR, P_REG, P_GEN1, M_GENR, M_NONE,  P_GEN2, P_NONE),
    X(0x05, TEXT_INDEXW, WORD,   M_GREG, M_GENR, P_REG, P_GEN1, M_GENR, M_NONE,  P_GEN2, P_NONE),
    X(0x07, TEXT_INDEXD, DOUBLE, M_GREG, M_GENR, P_REG, P_GEN1, M_GENR, M_NONE,  P_GEN2, P_NONE),
};
static const Entry FORMAT_8_2[] PROGMEM = {
    X(0x03, TEXT_CVTP, DOUBLE, M_GREG, M_GENA, P_REG,  P_GEN1,  M_GENW, M_NONE,  P_GEN2, P_NONE),
    E(0x04, TEXT_FFSB, BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x05, TEXT_FFSW, WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E(0x07, TEXT_FFSD, DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
};
static const Entry FORMAT_8_3[] PROGMEM = {
    X(0x00, TEXT_INSB, BYTE,   M_GREG, M_GENR, P_REG, P_GEN1, M_GENW, M_LEN32, P_GEN2, P_DISP),
    X(0x01, TEXT_INSW, WORD,   M_GREG, M_GENR, P_REG, P_GEN1, M_GENW, M_LEN32, P_GEN2, P_DISP),
    X(0x03, TEXT_INSD, DOUBLE, M_GREG, M_GENR, P_REG, P_GEN1, M_GENW, M_LEN32, P_GEN2, P_DISP),
};
static const Entry FORMAT_8_3_1[] PROGMEM = {
    E(0x0C, TEXT_MOVSUB, BYTE,   M_GENA, M_GENA, P_GEN1, P_GEN2),
    E(0x0D, TEXT_MOVSUW, WORD,   M_GENA, M_GENA, P_GEN1, P_GEN2),
    E(0x0F, TEXT_MOVSUD, DOUBLE, M_GENA, M_GENA, P_GEN1, P_GEN2),
    E(0x1C, TEXT_MOVUSB, BYTE,   M_GENA, M_GENA, P_GEN1, P_GEN2),
    E(0x1D, TEXT_MOVUSW, WORD,   M_GENA, M_GENA, P_GEN1, P_GEN2),
    E(0x1F, TEXT_MOVUSD, DOUBLE, M_GENA, M_GENA, P_GEN1, P_GEN2),
};
static const Entry FORMAT_8_4[] PROGMEM = {
    X(0x00, TEXT_CHECKB, BYTE,   M_GREG, M_GENA, P_REG, P_GEN1, M_GENR, M_NONE, P_GEN2, P_NONE),
    X(0x01, TEXT_CHECKW, WORD,   M_GREG, M_GENA, P_REG, P_GEN1, M_GENR, M_NONE, P_GEN2, P_NONE),
    X(0x03, TEXT_CHECKD, DOUBLE, M_GREG, M_GENA, P_REG, P_GEN1, M_GENR, M_NONE, P_GEN2, P_NONE),
};

// Format 9: |gen1_|gen| |2_|_op|f|ii| |0011|1110|
static const Entry FORMAT_9[] PROGMEM = {
    // gen2 must be register pair
    X(0x00, TEXT_MOVBL,   BYTE,   M_GENR, M_FENW, P_GEN1, P_GEN2, M_NONE, M_NONE, P_NONE, P_IMPL),
    X(0x01, TEXT_MOVWL,   WORD,   M_GENR, M_FENW, P_GEN1, P_GEN2, M_NONE, M_NONE, P_NONE, P_IMPL),
    X(0x03, TEXT_MOVDL,   DOUBLE, M_GENR, M_FENW, P_GEN1, P_GEN2, M_NONE, M_NONE, P_NONE, P_IMPL),
    E(0x04, TEXT_MOVBF,   BYTE,   M_GENR, M_FENW, P_GEN1, P_GEN2),
    E(0x05, TEXT_MOVWF,   WORD,   M_GENR, M_FENW, P_GEN1, P_GEN2),
    E(0x07, TEXT_MOVDF,   DOUBLE, M_GENR, M_FENW, P_GEN1, P_GEN2),
    E(0x0F, TEXT_LFSR,    DOUBLE, M_GENR, M_NONE, P_GEN1, P_NONE),
    E(0x37, TEXT_SFSR,    DOUBLE, M_GENW, M_NONE, P_GEN2, P_NONE),
    // gen1 must be register pair
    X(0x16, TEXT_MOVLF,   FLOAT,  M_FENR, M_FENW, P_GEN1, P_GEN2, M_NONE, M_NONE, P_IMPL, P_NONE),
    // gen2 must be register pair
    X(0x1B, TEXT_MOVFL,   FLOAT,  M_FENR, M_FENW, P_GEN1, P_GEN2, M_NONE, M_NONE, P_NONE, P_IMPL),
    E(0x20, TEXT_ROUNDLB, LONG,   M_FENR, M_GENW, P_GEN1, P_GEN2),
    E(0x21, TEXT_ROUNDLW, LONG,   M_FENR, M_GENW, P_GEN1, P_GEN2),
    E(0x23, TEXT_ROUNDLD, LONG,   M_FENR, M_GENW, P_GEN1, P_GEN2),
    E(0x24, TEXT_ROUNDFB, FLOAT,  M_FENR, M_GENW, P_GEN1, P_GEN2),
    E(0x25, TEXT_ROUNDFW, FLOAT,  M_FENR, M_GENW, P_GEN1, P_GEN2),
    E(0x27, TEXT_ROUNDFD, FLOAT,  M_FENR, M_GENW, P_GEN1, P_GEN2),
    E(0x28, TEXT_TRUNCLB, LONG,   M_FENR, M_GENW, P_GEN1, P_GEN2),
    E(0x29, TEXT_TRUNCLW, LONG,   M_FENR, M_GENW, P_GEN1, P_GEN2),
    E(0x2B, TEXT_TRUNCLD, LONG,   M_FENR, M_GENW, P_GEN1, P_GEN2),
    E(0x2C, TEXT_TRUNCFB, FLOAT,  M_FENR, M_GENW, P_GEN1, P_GEN2),
    E(0x2D, TEXT_TRUNCFW, FLOAT,  M_FENR, M_GENW, P_GEN1, P_GEN2),
    E(0x2F, TEXT_TRUNCFD, FLOAT,  M_FENR, M_GENW, P_GEN1, P_GEN2),
    E(0x38, TEXT_FLOORLB, LONG,   M_FENR, M_GENW, P_GEN1, P_GEN2),
    E(0x39, TEXT_FLOORLW, LONG,   M_FENR, M_GENW, P_GEN1, P_GEN2),
    E(0x3B, TEXT_FLOORLD, LONG,   M_FENR, M_GENW, P_GEN1, P_GEN2),
    E(0x3C, TEXT_FLOORFB, FLOAT,  M_FENR, M_GENW, P_GEN1, P_GEN2),
    E(0x3D, TEXT_FLOORFW, FLOAT,  M_FENR, M_GENW, P_GEN1, P_GEN2),
    E(0x3F, TEXT_FLOORFD, FLOAT,  M_FENR, M_GENW, P_GEN1, P_GEN2),
};

// Format 11: |gen1_|gen| |2_|_op_|0f| |1011|1110|
static const Entry FORMAT_11[] PROGMEM = {
    E(0x00, TEXT_ADDL, LONG,  M_FENR, M_FENW, P_GEN1, P_GEN2),
    E(0x01, TEXT_ADDF, FLOAT, M_FENR, M_FENW, P_GEN1, P_GEN2),
    E(0x04, TEXT_MOVL, LONG,  M_FENR, M_FENW, P_GEN1, P_GEN2),
    E(0x05, TEXT_MOVF, FLOAT, M_FENR, M_FENW, P_GEN1, P_GEN2),
    E(0x08, TEXT_CMPL, LONG,  M_FENR, M_FENR, P_GEN1, P_GEN2),
    E(0x09, TEXT_CMPF, FLOAT, M_FENR, M_FENR, P_GEN1, P_GEN2),
    E(0x10, TEXT_SUBL, LONG,  M_FENR, M_FENW, P_GEN1, P_GEN2),
    E(0x11, TEXT_SUBF, FLOAT, M_FENR, M_FENW, P_GEN1, P_GEN2),
    E(0x14, TEXT_NEGL, LONG,  M_FENR, M_FENW, P_GEN1, P_GEN2),
    E(0x15, TEXT_NEGF, FLOAT, M_FENR, M_FENW, P_GEN1, P_GEN2),
    E(0x20, TEXT_DIVL, LONG,  M_FENR, M_FENW, P_GEN1, P_GEN2),
    E(0x21, TEXT_DIVF, FLOAT, M_FENR, M_FENW, P_GEN1, P_GEN2),
    E(0x30, TEXT_MULL, LONG,  M_FENR, M_FENW, P_GEN1, P_GEN2),
    E(0x31, TEXT_MULF, FLOAT, M_FENR, M_FENW, P_GEN1, P_GEN2),
    E(0x34, TEXT_ABSL, LONG,  M_FENR, M_FENW, P_GEN1, P_GEN2),
    E(0x35, TEXT_ABSF, FLOAT, M_FENR, M_FENW, P_GEN1, P_GEN2),
};

// Format 14: |gen1_|sho| |t|0|_op_|ii| |0001|1110|
static const Entry FORMAT_14_1[] PROGMEM = {
    E(0x03, TEXT_RDVAL, DOUBLE, M_GENA, M_NONE, P_GEN1,  P_NONE),
    E(0x07, TEXT_WRVAL, DOUBLE, M_GENA, M_NONE, P_GEN1,  P_NONE),
};
static const Entry FORMAT_14_2[] PROGMEM = {
    E(0x0B, TEXT_LMR, DOUBLE, M_MREG, M_GENR, P_SHORT, P_GEN1),
    E(0x0F, TEXT_SMR, DOUBLE, M_MREG, M_GENW, P_SHORT, P_GEN1),
};
// clang-format on

class TableNs32000::EntryPage : public EntryPageBase<Entry> {
public:
    constexpr EntryPage(Config::opcode_t prefix, Config::opcode_t mask, uint8_t post,
            const Entry *table, const Entry *end)
        : EntryPageBase(table, end), _prefix(prefix), _mask(mask), _post(post) {}

    Config::opcode_t prefix() const { return pgm_read_byte(&_prefix); }
    Config::opcode_t mask() const { return pgm_read_byte(&_mask); }
    Config::opcode_t post() const { return pgm_read_byte(&_post); }

private:
    Config::opcode_t _prefix;
    Config::opcode_t _mask;
    uint8_t _post;
};

// Standard Instructions
static const TableNs32000::EntryPage NS32032_PAGES[] PROGMEM = {
        {0x00, 0x00, 0, ARRAY_RANGE(FORMAT_0)},
        {0x00, 0x00, 0, ARRAY_RANGE(FORMAT_1)},
        {0x00, 0xC0, 1, ARRAY_RANGE(FORMAT_4)},
        {0x00, 0x80, 1, ARRAY_RANGE(FORMAT_2_0)},
        {0x3C, 0xF8, 0, ARRAY_RANGE(FORMAT_2_1)},
        {0xBC, 0xF8, 0, ARRAY_RANGE(FORMAT_2_2)},
        {0x3D, 0xF8, 0, ARRAY_RANGE(FORMAT_2_3)},
        {0xBD, 0xF8, 0, ARRAY_RANGE(FORMAT_2_4)},
        {0x3F, 0xF8, 0, ARRAY_RANGE(FORMAT_2_5)},
        {0xBF, 0xF8, 0, ARRAY_RANGE(FORMAT_2_6)},
        {0x7C, 0xF8, 0, ARRAY_RANGE(FORMAT_3_1)},
        {0x7D, 0xF8, 0, ARRAY_RANGE(FORMAT_3_2)},
        {0x7F, 0xF8, 0, ARRAY_RANGE(FORMAT_3_3)},
        {0x0E, 0x80, 1, ARRAY_RANGE(FORMAT_5_0)},
        {0x0E, 0x00, 1, ARRAY_RANGE(FORMAT_5_1)},
        {0x4E, 0xC0, 1, ARRAY_RANGE(FORMAT_6)},
        {0xCE, 0xC0, 1, ARRAY_RANGE(FORMAT_7)},
        {0x2E, 0xF8, 1, ARRAY_RANGE(FORMAT_8_1)},
        {0x6E, 0xF8, 1, ARRAY_RANGE(FORMAT_8_2)},
        {0xAE, 0xF8, 1, ARRAY_RANGE(FORMAT_8_3)},
        {0xEE, 0xF8, 1, ARRAY_RANGE(FORMAT_8_4)},
};

// Floating point instructions
static const TableNs32000::EntryPage NS32081_PAGES[] PROGMEM = {
        {0x3E, 0xC0, 1, ARRAY_RANGE(FORMAT_9)},
        {0xBE, 0xC0, 1, ARRAY_RANGE(FORMAT_11)},
};

// Memory management instructions
static const TableNs32000::EntryPage NS32082_PAGES[] PROGMEM = {
        {0xAE, 0xC0, 1, ARRAY_RANGE(FORMAT_8_3_1)},
        {0x1E, 0x00, 1, ARRAY_RANGE(FORMAT_14_1)},
        {0x1E, 0x80, 1, ARRAY_RANGE(FORMAT_14_2)},
};

static bool isPrefix(Config::opcode_t opCode, const TableNs32000::EntryPage *page,
        const TableNs32000::EntryPage *end) {
    for (const TableNs32000::EntryPage *entry = page; entry < end; entry++) {
        const Config::opcode_t prefix = entry->prefix();
        if (prefix == 0)
            continue;
        if (prefix == opCode)
            return true;
    }
    return false;
}

bool TableNs32000::isPrefixCode(Config::opcode_t opCode) const {
    return isPrefix(opCode, ARRAY_RANGE(NS32032_PAGES)) ||
           isPrefix(opCode, ARRAY_RANGE(NS32081_PAGES)) ||
           isPrefix(opCode, ARRAY_RANGE(NS32082_PAGES));
}

static bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_GREG)
        return table == M_GENR || table == M_GENC || table == M_GENW || table == M_RLST;
    if (opr == M_RREL || opr == M_MREL || opr == M_ABS || opr == M_EXT || opr == M_TOS ||
            opr == M_MEM || opr == M_REL)
        return table == M_GENR || table == M_GENC || table == M_GENW || table == M_GENA ||
               table == M_FENR || table == M_FENW;
    if (opr == M_IMM)
        return table == M_GENR || table == M_GENC || table == M_FENR || table == M_DISP ||
               table == M_INT4 || table == M_REL || table == M_BFOFF || table == M_BFLEN ||
               table == M_LEN32 || table == M_LEN16 || table == M_LEN8 || table == M_LEN4;
    if (opr == M_FREG)
        return table == M_FENR || table == M_FENW;
    if (opr == M_NONE)
        return table == M_RLST || table == M_CONF || table == M_SOPT;
    return false;
}

static bool acceptModes(const Entry::Flags flags, const Entry *entry) {
    const Entry::Flags table = entry->flags();
    return acceptMode(flags.srcMode(), table.srcMode()) &&
           acceptMode(flags.dstMode(), table.dstMode()) &&
           acceptMode(flags.ex1Mode(), table.ex1Mode()) &&
           acceptMode(flags.ex2Mode(), table.ex2Mode());
}

Error TableNs32000::searchName(
        InsnNs32000 &insn, const EntryPage *pages, const EntryPage *end) const {
    uint8_t count = 0;
    for (const EntryPage *page = pages; page < end; page++) {
        const uint8_t post = page->post();
        const Entry *entry = TableBase::searchName<Entry, Entry::Flags>(
                insn.name(), insn.flags(), page->table(), page->end(), acceptModes, count);
        if (entry) {
            insn.setOpCode(entry->opCode(), page->prefix());
            insn.setPost(0, post != 0);
            insn.setFlags(entry->flags());
            return OK;
        }
    }
    return count == 0 ? UNKNOWN_INSTRUCTION : OPERAND_NOT_ALLOWED;
}

Error TableNs32000::searchOpCode(
        InsnNs32000 &insn, DisMemory &memory, const EntryPage *pages, const EntryPage *end) const {
    for (const EntryPage *page = pages; page < end; page++) {
        if (insn.prefix() != page->prefix())
            continue;
        const auto post = page->post();
        const Entry *entry = TableBase::searchCode<Entry, Config::opcode_t>(
                insn.opCode() & ~page->mask(), page->table(), page->end());
        if (entry) {
            insn.setFlags(entry->flags());
            if (post) {
                insn.readPost(memory);
                if (setError(insn))
                    return getError();
            }
            insn.setName_P(entry->name());
            return OK;
        }
    }
    return UNKNOWN_INSTRUCTION;
}

Error TableNs32000::searchName(InsnNs32000 &insn) const {
    Error error = searchName(insn, ARRAY_RANGE(NS32032_PAGES));
    if (error == UNKNOWN_INSTRUCTION && _fpuType == FPU_NS32081)
        error = searchName(insn, ARRAY_RANGE(NS32081_PAGES));
    if (error == UNKNOWN_INSTRUCTION && _mmuType == MMU_NS32082)
        error = searchName(insn, ARRAY_RANGE(NS32082_PAGES));
    return setError(error);
}

Error TableNs32000::searchOpCode(InsnNs32000 &insn, DisMemory &memory) const {
    Error error = searchOpCode(insn, memory, ARRAY_RANGE(NS32032_PAGES));
    if (error != OK)
        error = searchOpCode(insn, memory, ARRAY_RANGE(NS32081_PAGES));
    if (error != OK)
        error = searchOpCode(insn, memory, ARRAY_RANGE(NS32082_PAGES));
    return setError(error);
}

const char *TableNs32000::listCpu() const {
    return TEXT_CPU_LIST;
}

const char *TableNs32000::getCpu() const {
    return TEXT_CPU_32032;
}

bool TableNs32000::setCpu(const char *cpu) {
    if (strncasecmp_P(cpu, TEXT_CPU_NS, 2) == 0)
        cpu += 2;
    return strcasecmp_P(cpu, TEXT_CPU_32032) == 0;
}

class TableNs32000 TableNs32000;

}  // namespace ns32000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

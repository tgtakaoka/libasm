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

static constexpr AddrMode ex2Modes[] PROGMEM = {
        M_NONE,   // EM2_NONE
        M_IMM,    // EM2_IMM
        M_BFLEN,  // EM2_BFLEN
        M_LEN32,  // EM2_LEN32
        M_NONE,   // EM2_ERROR
};
AddrMode Entry::toAddrMode(Ex2Mode mode) {
    return AddrMode(pgm_read_byte(ex2Modes + mode));
}

static constexpr OprPos ex2Pos[] PROGMEM = {
        P_NONE,  // EP2_NONE
        P_IMPL,  // EP2_IMPL
        P_DISP,  // EP2_DISP
        P_NONE,  // EP2_ERROR
};
OprPos Entry::toOprPos(Ex2Pos pos) {
    return OprPos(pgm_read_byte(ex2Pos + pos));
}

#define E4(_opc, _name, _sz, _srcm, _dstm, _srcp, _dstp, _ex1m, _ex2m, _ex1p, _ex2p)       \
    {                                                                                      \
        _opc,                                                                              \
                Entry::Flags::create(                                                      \
                        _srcm, _srcp, _dstm, _dstp, _ex1m, _ex1p, _ex2m, _ex2p, SZ_##_sz), \
                _name                                                                      \
    }
#define E3(_opc, _name, _sz, _srcm, _dstm, _srcp, _dstp, _ex1m, _ex1p) \
    E4(_opc, _name, _sz, _srcm, _dstm, _srcp, _dstp, _ex1m, EM2_NONE, _ex1p, EP2_NONE)
#define E2(_opc, _name, _sz, _srcm, _dstm, _srcp, _dstp) \
    E3(_opc, _name, _sz, _srcm, _dstm, _srcp, _dstp, M_NONE, P_NONE)
#define E1(_opc, _name, _sz, _srcm, _srcp) E2(_opc, _name, _sz, _srcm, M_NONE, _srcp, P_NONE)
#define E0(_opc, _name) E1(_opc, _name, NONE, M_NONE, P_NONE)
#define W2(_opc, _name, _sz, _srcm, _dstm, _srcp, _dstp, _srcw, _dstw)                          \
    E4(_opc, _name, _sz, _srcm, _dstm, _srcp, _dstp, M_NONE, EM2_NONE, _srcw ? P_IMPL : P_NONE, \
            _dstw ? EP2_IMPL : EP2_NONE)

// clang-format off
// Format 0: |cond|1010|
static constexpr Entry FORMAT_0[] PROGMEM = {
    E1(0x0A, TEXT_BEQ, NONE, M_REL, P_DISP),
    E1(0x1A, TEXT_BNE, NONE, M_REL, P_DISP),
    E1(0x2A, TEXT_BCS, NONE, M_REL, P_DISP),
    E1(0x3A, TEXT_BCC, NONE, M_REL, P_DISP),
    E1(0x4A, TEXT_BHI, NONE, M_REL, P_DISP),
    E1(0x5A, TEXT_BLS, NONE, M_REL, P_DISP),
    E1(0x6A, TEXT_BGT, NONE, M_REL, P_DISP),
    E1(0x7A, TEXT_BLE, NONE, M_REL, P_DISP),
    E1(0x8A, TEXT_BFS, NONE, M_REL, P_DISP),
    E1(0x9A, TEXT_BFC, NONE, M_REL, P_DISP),
    E1(0xAA, TEXT_BLO, NONE, M_REL, P_DISP),
    E1(0xBA, TEXT_BHS, NONE, M_REL, P_DISP),
    E1(0xCA, TEXT_BLT, NONE, M_REL, P_DISP),
    E1(0xDA, TEXT_BGE, NONE, M_REL, P_DISP),
    E1(0xEA, TEXT_BR,  NONE, M_REL, P_DISP),
};
static constexpr uint8_t INDEX_0[] PROGMEM = {
      3,  // TEXT_BCC
      2,  // TEXT_BCS
      0,  // TEXT_BEQ
      9,  // TEXT_BFC
      8,  // TEXT_BFS
     13,  // TEXT_BGE
      6,  // TEXT_BGT
      4,  // TEXT_BHI
     11,  // TEXT_BHS
      7,  // TEXT_BLE
     10,  // TEXT_BLO
      5,  // TEXT_BLS
     12,  // TEXT_BLT
      1,  // TEXT_BNE
     14,  // TEXT_BR
};

// Format 1: |_op_|0010|
static constexpr Entry FORMAT_1[] PROGMEM = {
    E1(0x02, TEXT_BSR,     NONE,   M_REL, P_DISP),
    E1(0x12, TEXT_RET,     NONE,   M_DISP, P_DISP),
    E1(0x22, TEXT_CXP,     NONE,   M_DISP, P_DISP),
    E1(0x32, TEXT_RXP,     NONE,   M_DISP, P_DISP),
    E1(0x42, TEXT_RETT,    NONE,   M_DISP, P_DISP),
    E0(0x52, TEXT_RETI),
    E1(0x62, TEXT_SAVE,    NONE,   M_RLST, P_IMPL),
    E1(0x72, TEXT_RESTORE, DOUBLE, M_RLST, P_IMPL),
    E2(0x82, TEXT_ENTER,   NONE,   M_RLST, M_DISP, P_IMPL, P_DISP),
    E1(0x92, TEXT_EXIT,    DOUBLE, M_RLST, P_IMPL),
    E0(0xA2, TEXT_NOP),
    E0(0xB2, TEXT_WAIT),
    E0(0xC2, TEXT_DIA),
    E0(0xD2, TEXT_FLAG),
    E0(0xE2, TEXT_SVC),
    E0(0xF2, TEXT_BPT),
};
static constexpr uint8_t INDEX_1[] PROGMEM = {
     15,  // TEXT_BPT
      0,  // TEXT_BSR
      2,  // TEXT_CXP
     12,  // TEXT_DIA
      8,  // TEXT_ENTER
      9,  // TEXT_EXIT
     13,  // TEXT_FLAG
     10,  // TEXT_NOP
      7,  // TEXT_RESTORE
      1,  // TEXT_RET
      5,  // TEXT_RETI
      4,  // TEXT_RETT
      3,  // TEXT_RXP
      6,  // TEXT_SAVE
     14,  // TEXT_SVC
     11,  // TEXT_WAIT
};

// Format 4: |gen1_|gen| |2_|_op_|ii|
static constexpr Entry FORMAT_4[] PROGMEM = {
    E2(0x00, TEXT_ADDB,   BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x01, TEXT_ADDW,   WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x03, TEXT_ADDD,   DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x04, TEXT_CMPB,   BYTE,   M_GENR, M_GENR, P_GEN1, P_GEN2),
    E2(0x05, TEXT_CMPW,   WORD,   M_GENR, M_GENR, P_GEN1, P_GEN2),
    E2(0x07, TEXT_CMPD,   DOUBLE, M_GENR, M_GENR, P_GEN1, P_GEN2),
    E2(0x08, TEXT_BICB,   BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x09, TEXT_BICW,   WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x0B, TEXT_BICD,   DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x10, TEXT_ADDCB,  BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x11, TEXT_ADDCW,  WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x13, TEXT_ADDCD,  DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x14, TEXT_MOVB,   BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x15, TEXT_MOVW,   WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x17, TEXT_MOVD,   DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x18, TEXT_ORB,    BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x19, TEXT_ORW,    WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x1B, TEXT_ORD,    DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x20, TEXT_SUBB,   BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x21, TEXT_SUBW,   WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x23, TEXT_SUBD,   DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x27, TEXT_ADDR,   DOUBLE, M_GENA, M_GENW, P_GEN1, P_GEN2),
    E2(0x28, TEXT_ANDB,   BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x29, TEXT_ANDW,   WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x2B, TEXT_ANDD,   DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x30, TEXT_SUBCB,  BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x31, TEXT_SUBCW,  WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x33, TEXT_SUBCD,  DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x34, TEXT_TBITB,  BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x35, TEXT_TBITW,  WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x37, TEXT_TBITD,  DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x38, TEXT_XORB,   BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x39, TEXT_XORW,   WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x3B, TEXT_XORD,   DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
};
static constexpr uint8_t INDEX_4[] PROGMEM = {
      0,  // TEXT_ADDB
      9,  // TEXT_ADDCB
     11,  // TEXT_ADDCD
     10,  // TEXT_ADDCW
      2,  // TEXT_ADDD
     21,  // TEXT_ADDR
      1,  // TEXT_ADDW
     22,  // TEXT_ANDB
     24,  // TEXT_ANDD
     23,  // TEXT_ANDW
      6,  // TEXT_BICB
      8,  // TEXT_BICD
      7,  // TEXT_BICW
      3,  // TEXT_CMPB
      5,  // TEXT_CMPD
      4,  // TEXT_CMPW
     12,  // TEXT_MOVB
     14,  // TEXT_MOVD
     13,  // TEXT_MOVW
     15,  // TEXT_ORB
     17,  // TEXT_ORD
     16,  // TEXT_ORW
     18,  // TEXT_SUBB
     25,  // TEXT_SUBCB
     27,  // TEXT_SUBCD
     26,  // TEXT_SUBCW
     20,  // TEXT_SUBD
     19,  // TEXT_SUBW
     28,  // TEXT_TBITB
     30,  // TEXT_TBITD
     29,  // TEXT_TBITW
     31,  // TEXT_XORB
     33,  // TEXT_XORD
     32,  // TEXT_XORW
};

// Format 2: |_gen_|sho| |t|_op|11|ii|
static constexpr Entry FORMAT_2_0[] PROGMEM = {
    E2(0x0C, TEXT_ADDQB, BYTE,   M_INT4, M_GENW, P_SHORT, P_GEN1),
    E2(0x0D, TEXT_ADDQW, WORD,   M_INT4, M_GENW, P_SHORT, P_GEN1),
    E2(0x0F, TEXT_ADDQD, DOUBLE, M_INT4, M_GENW, P_SHORT, P_GEN1),
    E2(0x1C, TEXT_CMPQB, BYTE,   M_INT4, M_GENR, P_SHORT, P_GEN1),
    E2(0x1D, TEXT_CMPQW, WORD,   M_INT4, M_GENR, P_SHORT, P_GEN1),
    E2(0x1F, TEXT_CMPQD, DOUBLE, M_INT4, M_GENR, P_SHORT, P_GEN1),
    E2(0x2C, TEXT_SPRB,  BYTE,   M_PREG, M_GENW, P_SHORT, P_GEN1),
    E2(0x2D, TEXT_SPRW,  WORD,   M_PREG, M_GENW, P_SHORT, P_GEN1),
    E2(0x2F, TEXT_SPRD,  DOUBLE, M_PREG, M_GENW, P_SHORT, P_GEN1),
    E3(0x4C, TEXT_ACBB,  BYTE,   M_INT4, M_GENW, P_SHORT, P_GEN1, M_REL, P_DISP),
    E3(0x4D, TEXT_ACBW,  WORD,   M_INT4, M_GENW, P_SHORT, P_GEN1, M_REL, P_DISP),
    E3(0x4F, TEXT_ACBD,  DOUBLE, M_INT4, M_GENW, P_SHORT, P_GEN1, M_REL, P_DISP),
    E2(0x5C, TEXT_MOVQB, BYTE,   M_INT4, M_GENW, P_SHORT, P_GEN1),
    E2(0x5D, TEXT_MOVQW, WORD,   M_INT4, M_GENW, P_SHORT, P_GEN1),
    E2(0x5F, TEXT_MOVQD, DOUBLE, M_INT4, M_GENW, P_SHORT, P_GEN1),
    E2(0x6C, TEXT_LPRB,  BYTE,   M_PREG, M_GENR, P_SHORT, P_GEN1),
    E2(0x6D, TEXT_LPRW,  WORD,   M_PREG, M_GENR, P_SHORT, P_GEN1),
    E2(0x6F, TEXT_LPRD,  DOUBLE, M_PREG, M_GENR, P_SHORT, P_GEN1),
};
// Format 2: |_gen_|con| |d|011|11|ii|
static constexpr Entry FORMAT_2_1[] PROGMEM = {
    E1(0x00, TEXT_SEQB, BYTE, M_GENW, P_GEN1),
    E1(0x01, TEXT_SCSB, BYTE, M_GENW, P_GEN1),
    E1(0x02, TEXT_SHIB, BYTE, M_GENW, P_GEN1),
    E1(0x03, TEXT_SGTB, BYTE, M_GENW, P_GEN1),
    E1(0x04, TEXT_SFSB, BYTE, M_GENW, P_GEN1),
    E1(0x05, TEXT_SLOB, BYTE, M_GENW, P_GEN1),
    E1(0x06, TEXT_SLTB, BYTE, M_GENW, P_GEN1),
};
static constexpr Entry FORMAT_2_2[] PROGMEM = {
    E1(0x00, TEXT_SNEB, BYTE, M_GENW, P_GEN1),
    E1(0x01, TEXT_SCCB, BYTE, M_GENW, P_GEN1),
    E1(0x02, TEXT_SLSB, BYTE, M_GENW, P_GEN1),
    E1(0x03, TEXT_SLEB, BYTE, M_GENW, P_GEN1),
    E1(0x04, TEXT_SFCB, BYTE, M_GENW, P_GEN1),
    E1(0x05, TEXT_SHSB, BYTE, M_GENW, P_GEN1),
    E1(0x06, TEXT_SGEB, BYTE, M_GENW, P_GEN1),
};
static constexpr Entry FORMAT_2_3[] PROGMEM = {
    E1(0x00, TEXT_SEQW, WORD, M_GENW, P_GEN1),
    E1(0x01, TEXT_SCSW, WORD, M_GENW, P_GEN1),
    E1(0x02, TEXT_SHIW, WORD, M_GENW, P_GEN1),
    E1(0x03, TEXT_SGTW, WORD, M_GENW, P_GEN1),
    E1(0x04, TEXT_SFSW, WORD, M_GENW, P_GEN1),
    E1(0x05, TEXT_SLOW, WORD, M_GENW, P_GEN1),
    E1(0x06, TEXT_SLTW, WORD, M_GENW, P_GEN1),
};
static constexpr Entry FORMAT_2_4[] PROGMEM = {
    E1(0x00, TEXT_SNEW, WORD, M_GENW, P_GEN1),
    E1(0x01, TEXT_SCCW, WORD, M_GENW, P_GEN1),
    E1(0x02, TEXT_SLSW, WORD, M_GENW, P_GEN1),
    E1(0x03, TEXT_SLEW, WORD, M_GENW, P_GEN1),
    E1(0x04, TEXT_SFCW, WORD, M_GENW, P_GEN1),
    E1(0x05, TEXT_SHSW, WORD, M_GENW, P_GEN1),
    E1(0x06, TEXT_SGEW, WORD, M_GENW, P_GEN1),
};
static constexpr Entry FORMAT_2_5[] PROGMEM = {
    E1(0x00, TEXT_SEQD, DOUBLE, M_GENW, P_GEN1),
    E1(0x01, TEXT_SCSD, DOUBLE, M_GENW, P_GEN1),
    E1(0x02, TEXT_SHID, DOUBLE, M_GENW, P_GEN1),
    E1(0x03, TEXT_SGTD, DOUBLE, M_GENW, P_GEN1),
    E1(0x04, TEXT_SFSD, DOUBLE, M_GENW, P_GEN1),
    E1(0x05, TEXT_SLOD, DOUBLE, M_GENW, P_GEN1),
    E1(0x06, TEXT_SLTD, DOUBLE, M_GENW, P_GEN1),
};
static constexpr Entry FORMAT_2_6[] PROGMEM = {
    E1(0x00, TEXT_SNED, DOUBLE, M_GENW, P_GEN1),
    E1(0x01, TEXT_SCCD, DOUBLE, M_GENW, P_GEN1),
    E1(0x02, TEXT_SLSD, DOUBLE, M_GENW, P_GEN1),
    E1(0x03, TEXT_SLED, DOUBLE, M_GENW, P_GEN1),
    E1(0x04, TEXT_SFCD, DOUBLE, M_GENW, P_GEN1),
    E1(0x05, TEXT_SHSD, DOUBLE, M_GENW, P_GEN1),
    E1(0x06, TEXT_SGED, DOUBLE, M_GENW, P_GEN1),
};
static constexpr uint8_t INDEX_2_0[] PROGMEM = {
      9,  // TEXT_ACBB
     11,  // TEXT_ACBD
     10,  // TEXT_ACBW
      0,  // TEXT_ADDQB
      2,  // TEXT_ADDQD
      1,  // TEXT_ADDQW
      3,  // TEXT_CMPQB
      5,  // TEXT_CMPQD
      4,  // TEXT_CMPQW
     15,  // TEXT_LPRB
     17,  // TEXT_LPRD
     16,  // TEXT_LPRW
     12,  // TEXT_MOVQB
     14,  // TEXT_MOVQD
     13,  // TEXT_MOVQW
      6,  // TEXT_SPRB
      8,  // TEXT_SPRD
      7,  // TEXT_SPRW
};
static constexpr uint8_t INDEX_2_1[] PROGMEM = {
      1,  // TEXT_SCSB
      0,  // TEXT_SEQB
      4,  // TEXT_SFSB
      3,  // TEXT_SGTB
      2,  // TEXT_SHIB
      5,  // TEXT_SLOB
      6,  // TEXT_SLTB
};
static constexpr uint8_t INDEX_2_2[] PROGMEM = {
      1,  // TEXT_SCCB
      4,  // TEXT_SFCB
      6,  // TEXT_SGEB
      5,  // TEXT_SHSB
      3,  // TEXT_SLEB
      2,  // TEXT_SLSB
      0,  // TEXT_SNEB
};
static constexpr uint8_t INDEX_2_3[] PROGMEM = {
      1,  // TEXT_SCSW
      0,  // TEXT_SEQW
      4,  // TEXT_SFSW
      3,  // TEXT_SGTW
      2,  // TEXT_SHIW
      5,  // TEXT_SLOW
      6,  // TEXT_SLTW
};
static constexpr uint8_t INDEX_2_4[] PROGMEM = {
      1,  // TEXT_SCCW
      4,  // TEXT_SFCW
      6,  // TEXT_SGEW
      5,  // TEXT_SHSW
      3,  // TEXT_SLEW
      2,  // TEXT_SLSW
      0,  // TEXT_SNEW
};
static constexpr uint8_t INDEX_2_5[] PROGMEM = {
      1,  // TEXT_SCSD
      0,  // TEXT_SEQD
      4,  // TEXT_SFSD
      3,  // TEXT_SGTD
      2,  // TEXT_SHID
      5,  // TEXT_SLOD
      6,  // TEXT_SLTD
};
static constexpr uint8_t INDEX_2_6[] PROGMEM = {
      1,  // TEXT_SCCD
      4,  // TEXT_SFCD
      6,  // TEXT_SGED
      5,  // TEXT_SHSD
      3,  // TEXT_SLED
      2,  // TEXT_SLSD
      0,  // TEXT_SNED
};

// Format 3: |_gen_|_op| |011111|ii|
static constexpr Entry FORMAT_3_1[] PROGMEM = {
    E1(0x01, TEXT_BICPSRB, BYTE, M_GENR, P_GEN1),
    E1(0x03, TEXT_BISPSRB, BYTE, M_GENR, P_GEN1),
    E1(0x05, TEXT_ADJSPB,  BYTE, M_GENR, P_GEN1),
    E1(0x07, TEXT_CASEB,   BYTE, M_GENR, P_GEN1),
};
static constexpr Entry FORMAT_3_2[] PROGMEM = {
    E1(0x01, TEXT_BICPSRW, WORD, M_GENR, P_GEN1),
    E1(0x03, TEXT_BISPSRW, WORD, M_GENR, P_GEN1),
    E1(0x05, TEXT_ADJSPW,  WORD, M_GENR, P_GEN1),
    E1(0x07, TEXT_CASEW,   WORD, M_GENR, P_GEN1),
};
static constexpr Entry FORMAT_3_3[] PROGMEM = {
    E1(0x00, TEXT_CXPD,   DOUBLE, M_GENA, P_GEN1),
    E1(0x02, TEXT_JUMP,   DOUBLE, M_GENA, P_GEN1),
    E1(0x05, TEXT_ADJSPD, DOUBLE, M_GENR, P_GEN1),
    E1(0x06, TEXT_JSR,    DOUBLE, M_GENA, P_GEN1),
    E1(0x07, TEXT_CASED,  DOUBLE, M_GENR, P_GEN1),
};
static constexpr uint8_t INDEX_3_1[] PROGMEM = {
      2,  // TEXT_ADJSPB
      0,  // TEXT_BICPSRB
      1,  // TEXT_BISPSRB
      3,  // TEXT_CASEB
};
static constexpr uint8_t INDEX_3_2[] PROGMEM = {
      2,  // TEXT_ADJSPW
      0,  // TEXT_BICPSRW
      1,  // TEXT_BISPSRW
      3,  // TEXT_CASEW
};
static constexpr uint8_t INDEX_3_3[] PROGMEM = {
      2,  // TEXT_ADJSPD
      4,  // TEXT_CASED
      0,  // TEXT_CXPD
      3,  // TEXT_JSR
      1,  // TEXT_JUMP
};

// Format 5: |00000|sho| |t|0|_op_|ii| |0000|1110|
static constexpr Entry FORMAT_5_0[] PROGMEM = {
    E1(0x0B, TEXT_SETCFG, NONE, M_CONF, P_SHORT),
};
// Format 5: |00000|flg| |t|0|_op_|ii| |0000|1110|
static constexpr Entry FORMAT_5_1[] PROGMEM = {
    E1(0x00, TEXT_MOVSB, BYTE,   M_SOPT, P_SHORT),
    E1(0x01, TEXT_MOVSW, WORD,   M_SOPT, P_SHORT),
    E1(0x03, TEXT_MOVSD, DOUBLE, M_SOPT, P_SHORT),
    E1(0x80, TEXT_MOVST, BYTE,   M_SOPT, P_SHORT),
    E1(0x04, TEXT_CMPSB, BYTE,   M_SOPT, P_SHORT),
    E1(0x05, TEXT_CMPSW, WORD,   M_SOPT, P_SHORT),
    E1(0x07, TEXT_CMPSD, DOUBLE, M_SOPT, P_SHORT),
    E1(0x84, TEXT_CMPST, BYTE,   M_SOPT, P_SHORT),
    E1(0x0C, TEXT_SKPSB, BYTE,   M_SOPT, P_SHORT),
    E1(0x0D, TEXT_SKPSW, WORD,   M_SOPT, P_SHORT),
    E1(0x0F, TEXT_SKPSD, DOUBLE, M_SOPT, P_SHORT),
    E1(0x8C, TEXT_SKPST, BYTE,   M_SOPT, P_SHORT),
};
static constexpr uint8_t INDEX_5_0[] PROGMEM = {
      0,  // TEXT_SETCFG
};
static constexpr uint8_t INDEX_5_1[] PROGMEM = {
      4,  // TEXT_CMPSB
      6,  // TEXT_CMPSD
      7,  // TEXT_CMPST
      5,  // TEXT_CMPSW
      0,  // TEXT_MOVSB
      2,  // TEXT_MOVSD
      3,  // TEXT_MOVST
      1,  // TEXT_MOVSW
      8,  // TEXT_SKPSB
     10,  // TEXT_SKPSD
     11,  // TEXT_SKPST
      9,  // TEXT_SKPSW
};

// Format 6: |gen1_|gen| |2_|_op_|ii| |0100|1110|
static constexpr Entry FORMAT_6[] PROGMEM = {
    E2(0x00, TEXT_ROTB,   BYTE,   M_GENC, M_GENW, P_GEN1, P_GEN2),
    E2(0x01, TEXT_ROTW,   WORD,   M_GENC, M_GENW, P_GEN1, P_GEN2),
    E2(0x03, TEXT_ROTD,   DOUBLE, M_GENC, M_GENW, P_GEN1, P_GEN2),
    E2(0x04, TEXT_ASHB,   BYTE,   M_GENC, M_GENW, P_GEN1, P_GEN2),
    E2(0x05, TEXT_ASHW,   WORD,   M_GENC, M_GENW, P_GEN1, P_GEN2),
    E2(0x07, TEXT_ASHD,   DOUBLE, M_GENC, M_GENW, P_GEN1, P_GEN2),
    E2(0x08, TEXT_CBITB,  BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x09, TEXT_CBITW,  WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x0B, TEXT_CBITD,  DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x0C, TEXT_CBITIB, BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x0D, TEXT_CBITIW, WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x0F, TEXT_CBITID, DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x14, TEXT_LSHB,   BYTE,   M_GENC, M_GENW, P_GEN1, P_GEN2),
    E2(0x15, TEXT_LSHW,   WORD,   M_GENC, M_GENW, P_GEN1, P_GEN2),
    E2(0x17, TEXT_LSHD,   DOUBLE, M_GENC, M_GENW, P_GEN1, P_GEN2),
    E2(0x18, TEXT_SBITB,  BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x19, TEXT_SBITW,  WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x1B, TEXT_SBITD,  DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x1C, TEXT_SBITIB, BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x1D, TEXT_SBITIW, WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x1F, TEXT_SBITID, DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x20, TEXT_NEGB,   BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x21, TEXT_NEGW,   WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x23, TEXT_NEGD,   DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x24, TEXT_NOTB,   BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x25, TEXT_NOTW,   WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x27, TEXT_NOTD,   DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x2C, TEXT_SUBPB,  BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x2D, TEXT_SUBPW,  WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x2F, TEXT_SUBPD,  DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x30, TEXT_ABSB,   BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x31, TEXT_ABSW,   WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x33, TEXT_ABSD,   DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x34, TEXT_COMB,   BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x35, TEXT_COMW,   WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x37, TEXT_COMD,   DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x38, TEXT_IBITB,  BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x39, TEXT_IBITW,  WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x3B, TEXT_IBITD,  DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x3C, TEXT_ADDPB,  BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x3D, TEXT_ADDPW,  WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x3F, TEXT_ADDPD,  DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
};
static constexpr uint8_t INDEX_6[] PROGMEM = {
     30,  // TEXT_ABSB
     32,  // TEXT_ABSD
     31,  // TEXT_ABSW
     39,  // TEXT_ADDPB
     41,  // TEXT_ADDPD
     40,  // TEXT_ADDPW
      3,  // TEXT_ASHB
      5,  // TEXT_ASHD
      4,  // TEXT_ASHW
      6,  // TEXT_CBITB
      8,  // TEXT_CBITD
      9,  // TEXT_CBITIB
     11,  // TEXT_CBITID
     10,  // TEXT_CBITIW
      7,  // TEXT_CBITW
     33,  // TEXT_COMB
     35,  // TEXT_COMD
     34,  // TEXT_COMW
     36,  // TEXT_IBITB
     38,  // TEXT_IBITD
     37,  // TEXT_IBITW
     12,  // TEXT_LSHB
     14,  // TEXT_LSHD
     13,  // TEXT_LSHW
     21,  // TEXT_NEGB
     23,  // TEXT_NEGD
     22,  // TEXT_NEGW
     24,  // TEXT_NOTB
     26,  // TEXT_NOTD
     25,  // TEXT_NOTW
      0,  // TEXT_ROTB
      2,  // TEXT_ROTD
      1,  // TEXT_ROTW
     15,  // TEXT_SBITB
     17,  // TEXT_SBITD
     18,  // TEXT_SBITIB
     20,  // TEXT_SBITID
     19,  // TEXT_SBITIW
     16,  // TEXT_SBITW
     27,  // TEXT_SUBPB
     29,  // TEXT_SUBPD
     28,  // TEXT_SUBPW
};

// Format 7: |gen1_|gen| |2_|_op_|ii| |1100|1110|
static constexpr Entry FORMAT_7[] PROGMEM = {
    E3(0x00, TEXT_MOVMB,  BYTE,   M_GENA, M_GENA, P_GEN1, P_GEN2, M_LEN16,  P_DISP),
    E3(0x01, TEXT_MOVMW,  WORD,   M_GENA, M_GENA, P_GEN1, P_GEN2, M_LEN8,  P_DISP),
    E3(0x03, TEXT_MOVMD,  DOUBLE, M_GENA, M_GENA, P_GEN1, P_GEN2, M_LEN4,  P_DISP),
    E3(0x04, TEXT_CMPMB,  BYTE,   M_GENA, M_GENA, P_GEN1, P_GEN2, M_LEN16,  P_DISP),
    E3(0x05, TEXT_CMPMW,  WORD,   M_GENA, M_GENA, P_GEN1, P_GEN2, M_LEN8,  P_DISP),
    E3(0x07, TEXT_CMPMD,  DOUBLE, M_GENA, M_GENA, P_GEN1, P_GEN2, M_LEN4,  P_DISP),
    E4(0x08, TEXT_INSSB,  BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2, M_BFOFF, EM2_BFLEN, P_IMPL, EP2_IMPL),
    E4(0x09, TEXT_INSSW,  WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2, M_BFOFF, EM2_BFLEN, P_IMPL, EP2_IMPL),
    E4(0x0B, TEXT_INSSD,  DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2, M_BFOFF, EM2_BFLEN, P_IMPL, EP2_IMPL),
    E4(0x0C, TEXT_EXTSB,  BYTE,   M_GENW, M_GENW, P_GEN1, P_GEN2, M_BFOFF, EM2_BFLEN, P_IMPL, EP2_IMPL),
    E4(0x0D, TEXT_EXTSW,  WORD,   M_GENW, M_GENW, P_GEN1, P_GEN2, M_BFOFF, EM2_BFLEN, P_IMPL, EP2_IMPL),
    E4(0x0F, TEXT_EXTSD,  DOUBLE, M_GENW, M_GENW, P_GEN1, P_GEN2, M_BFOFF, EM2_BFLEN, P_IMPL, EP2_IMPL),
    E2(0x10, TEXT_MOVXBW, BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x1C, TEXT_MOVXBD, BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x1D, TEXT_MOVXWD, WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x14, TEXT_MOVZBW, BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x18, TEXT_MOVZBD, BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x19, TEXT_MOVZWD, WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x20, TEXT_MULB,   BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x21, TEXT_MULW,   WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x23, TEXT_MULD,   DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    // gen2 must be register pair.
    W2(0x24, TEXT_MEIB,   BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2, false, true),
    W2(0x25, TEXT_MEIW,   WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2, false, true),
    W2(0x27, TEXT_MEID,   DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2, false, true),
    W2(0x2C, TEXT_DEIB,   BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2, false, true),
    W2(0x2D, TEXT_DEIW,   WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2, false, true),
    W2(0x2F, TEXT_DEID,   DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2, false, true),
    E2(0x30, TEXT_QUOB,   BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x31, TEXT_QUOW,   WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x33, TEXT_QUOD,   DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x34, TEXT_REMB,   BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x35, TEXT_REMW,   WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x37, TEXT_REMD,   DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x38, TEXT_MODB,   BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x39, TEXT_MODW,   WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x3B, TEXT_MODD,   DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x3C, TEXT_DIVB,   BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x3D, TEXT_DIVW,   WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x3F, TEXT_DIVD,   DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
};
static constexpr uint8_t INDEX_7[] PROGMEM = {
      3,  // TEXT_CMPMB
      5,  // TEXT_CMPMD
      4,  // TEXT_CMPMW
     24,  // TEXT_DEIB
     26,  // TEXT_DEID
     25,  // TEXT_DEIW
     36,  // TEXT_DIVB
     38,  // TEXT_DIVD
     37,  // TEXT_DIVW
      9,  // TEXT_EXTSB
     11,  // TEXT_EXTSD
     10,  // TEXT_EXTSW
      6,  // TEXT_INSSB
      8,  // TEXT_INSSD
      7,  // TEXT_INSSW
     21,  // TEXT_MEIB
     23,  // TEXT_MEID
     22,  // TEXT_MEIW
     33,  // TEXT_MODB
     35,  // TEXT_MODD
     34,  // TEXT_MODW
      0,  // TEXT_MOVMB
      2,  // TEXT_MOVMD
      1,  // TEXT_MOVMW
     13,  // TEXT_MOVXBD
     12,  // TEXT_MOVXBW
     14,  // TEXT_MOVXWD
     16,  // TEXT_MOVZBD
     15,  // TEXT_MOVZBW
     17,  // TEXT_MOVZWD
     18,  // TEXT_MULB
     20,  // TEXT_MULD
     19,  // TEXT_MULW
     27,  // TEXT_QUOB
     29,  // TEXT_QUOD
     28,  // TEXT_QUOW
     30,  // TEXT_REMB
     32,  // TEXT_REMD
     31,  // TEXT_REMW
};

// Format 8: |gen1_|gen| |2_|reg|o|ii| |op|10|1110|
static constexpr Entry FORMAT_8_1[] PROGMEM = {
    E4(0x00, TEXT_EXTB,   BYTE,   M_GREG, M_GENW, P_REG, P_GEN1, M_GENW, EM2_LEN32, P_GEN2, EP2_DISP),
    E4(0x01, TEXT_EXTW,   WORD,   M_GREG, M_GENW, P_REG, P_GEN1, M_GENW, EM2_LEN32, P_GEN2, EP2_DISP),
    E4(0x03, TEXT_EXTD,   DOUBLE, M_GREG, M_GENW, P_REG, P_GEN1, M_GENW, EM2_LEN32, P_GEN2, EP2_DISP),
    E3(0x04, TEXT_INDEXB, BYTE,   M_GREG, M_GENR, P_REG, P_GEN1, M_GENR,  P_GEN2),
    E3(0x05, TEXT_INDEXW, WORD,   M_GREG, M_GENR, P_REG, P_GEN1, M_GENR,  P_GEN2),
    E3(0x07, TEXT_INDEXD, DOUBLE, M_GREG, M_GENR, P_REG, P_GEN1, M_GENR,  P_GEN2),
};
static constexpr Entry FORMAT_8_2[] PROGMEM = {
    E3(0x03, TEXT_CVTP, DOUBLE, M_GREG, M_GENA, P_REG,  P_GEN1,  M_GENW, P_GEN2),
    E2(0x04, TEXT_FFSB, BYTE,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x05, TEXT_FFSW, WORD,   M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x07, TEXT_FFSD, DOUBLE, M_GENR, M_GENW, P_GEN1, P_GEN2),
};
static constexpr Entry FORMAT_8_3[] PROGMEM = {
    E4(0x00, TEXT_INSB, BYTE,   M_GREG, M_GENR, P_REG, P_GEN1, M_GENW, EM2_LEN32, P_GEN2, EP2_DISP),
    E4(0x01, TEXT_INSW, WORD,   M_GREG, M_GENR, P_REG, P_GEN1, M_GENW, EM2_LEN32, P_GEN2, EP2_DISP),
    E4(0x03, TEXT_INSD, DOUBLE, M_GREG, M_GENR, P_REG, P_GEN1, M_GENW, EM2_LEN32, P_GEN2, EP2_DISP),
};
static constexpr Entry FORMAT_8_3_1[] PROGMEM = {
    E2(0x0C, TEXT_MOVSUB, BYTE,   M_GENA, M_GENA, P_GEN1, P_GEN2),
    E2(0x0D, TEXT_MOVSUW, WORD,   M_GENA, M_GENA, P_GEN1, P_GEN2),
    E2(0x0F, TEXT_MOVSUD, DOUBLE, M_GENA, M_GENA, P_GEN1, P_GEN2),
    E2(0x1C, TEXT_MOVUSB, BYTE,   M_GENA, M_GENA, P_GEN1, P_GEN2),
    E2(0x1D, TEXT_MOVUSW, WORD,   M_GENA, M_GENA, P_GEN1, P_GEN2),
    E2(0x1F, TEXT_MOVUSD, DOUBLE, M_GENA, M_GENA, P_GEN1, P_GEN2),
};
static constexpr Entry FORMAT_8_4[] PROGMEM = {
    E3(0x00, TEXT_CHECKB, BYTE,   M_GREG, M_GENA, P_REG, P_GEN1, M_GENR, P_GEN2),
    E3(0x01, TEXT_CHECKW, WORD,   M_GREG, M_GENA, P_REG, P_GEN1, M_GENR, P_GEN2),
    E3(0x03, TEXT_CHECKD, DOUBLE, M_GREG, M_GENA, P_REG, P_GEN1, M_GENR, P_GEN2),
};
static constexpr uint8_t INDEX_8_1[] PROGMEM = {
      0,  // TEXT_EXTB
      2,  // TEXT_EXTD
      1,  // TEXT_EXTW
      3,  // TEXT_INDEXB
      5,  // TEXT_INDEXD
      4,  // TEXT_INDEXW
};
static constexpr uint8_t INDEX_8_2[] PROGMEM = {
      0,  // TEXT_CVTP
      1,  // TEXT_FFSB
      3,  // TEXT_FFSD
      2,  // TEXT_FFSW
};
static constexpr uint8_t INDEX_8_3[] PROGMEM = {
      0,  // TEXT_INSB
      2,  // TEXT_INSD
      1,  // TEXT_INSW
};
static constexpr uint8_t INDEX_8_3_1[] PROGMEM = {
      0,  // TEXT_MOVSUB
      2,  // TEXT_MOVSUD
      1,  // TEXT_MOVSUW
      3,  // TEXT_MOVUSB
      5,  // TEXT_MOVUSD
      4,  // TEXT_MOVUSW
};
static constexpr uint8_t INDEX_8_4[] PROGMEM = {
      0,  // TEXT_CHECKB
      2,  // TEXT_CHECKD
      1,  // TEXT_CHECKW
};

// Format 9: |gen1_|gen| |2_|_op|f|ii| |0011|1110|
static constexpr Entry FORMAT_9[] PROGMEM = {
    // gen2 must be register pair
    W2(0x00, TEXT_MOVBL,   BYTE,   M_GENR, M_FENW, P_GEN1, P_GEN2, false, true),
    W2(0x01, TEXT_MOVWL,   WORD,   M_GENR, M_FENW, P_GEN1, P_GEN2, false, true),
    W2(0x03, TEXT_MOVDL,   DOUBLE, M_GENR, M_FENW, P_GEN1, P_GEN2, false, true),
    E2(0x04, TEXT_MOVBF,   BYTE,   M_GENR, M_FENW, P_GEN1, P_GEN2),
    E2(0x05, TEXT_MOVWF,   WORD,   M_GENR, M_FENW, P_GEN1, P_GEN2),
    E2(0x07, TEXT_MOVDF,   DOUBLE, M_GENR, M_FENW, P_GEN1, P_GEN2),
    E1(0x0F, TEXT_LFSR,    DOUBLE, M_GENR, P_GEN1),
    E1(0x37, TEXT_SFSR,    DOUBLE, M_GENW, P_GEN2),
    // gen1 must be register pair
    W2(0x16, TEXT_MOVLF,   FLOAT,  M_FENR, M_FENW, P_GEN1, P_GEN2, true, false),
    // gen2 must be register pair
    W2(0x1B, TEXT_MOVFL,   FLOAT,  M_FENR, M_FENW, P_GEN1, P_GEN2, false, true),
    E2(0x20, TEXT_ROUNDLB, LONG,   M_FENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x21, TEXT_ROUNDLW, LONG,   M_FENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x23, TEXT_ROUNDLD, LONG,   M_FENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x24, TEXT_ROUNDFB, FLOAT,  M_FENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x25, TEXT_ROUNDFW, FLOAT,  M_FENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x27, TEXT_ROUNDFD, FLOAT,  M_FENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x28, TEXT_TRUNCLB, LONG,   M_FENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x29, TEXT_TRUNCLW, LONG,   M_FENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x2B, TEXT_TRUNCLD, LONG,   M_FENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x2C, TEXT_TRUNCFB, FLOAT,  M_FENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x2D, TEXT_TRUNCFW, FLOAT,  M_FENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x2F, TEXT_TRUNCFD, FLOAT,  M_FENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x38, TEXT_FLOORLB, LONG,   M_FENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x39, TEXT_FLOORLW, LONG,   M_FENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x3B, TEXT_FLOORLD, LONG,   M_FENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x3C, TEXT_FLOORFB, FLOAT,  M_FENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x3D, TEXT_FLOORFW, FLOAT,  M_FENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x3F, TEXT_FLOORFD, FLOAT,  M_FENR, M_GENW, P_GEN1, P_GEN2),
};
static constexpr uint8_t INDEX_9[] PROGMEM = {
     25,  // TEXT_FLOORFB
     27,  // TEXT_FLOORFD
     26,  // TEXT_FLOORFW
     22,  // TEXT_FLOORLB
     24,  // TEXT_FLOORLD
     23,  // TEXT_FLOORLW
      6,  // TEXT_LFSR
      3,  // TEXT_MOVBF
      0,  // TEXT_MOVBL
      5,  // TEXT_MOVDF
      2,  // TEXT_MOVDL
      9,  // TEXT_MOVFL
      8,  // TEXT_MOVLF
      4,  // TEXT_MOVWF
      1,  // TEXT_MOVWL
     13,  // TEXT_ROUNDFB
     15,  // TEXT_ROUNDFD
     14,  // TEXT_ROUNDFW
     10,  // TEXT_ROUNDLB
     12,  // TEXT_ROUNDLD
     11,  // TEXT_ROUNDLW
      7,  // TEXT_SFSR
     19,  // TEXT_TRUNCFB
     21,  // TEXT_TRUNCFD
     20,  // TEXT_TRUNCFW
     16,  // TEXT_TRUNCLB
     18,  // TEXT_TRUNCLD
     17,  // TEXT_TRUNCLW
};

// Format 11: |gen1_|gen| |2_|_op_|0f| |1011|1110|
static constexpr Entry FORMAT_11[] PROGMEM = {
    E2(0x00, TEXT_ADDL, LONG,  M_FENR, M_FENW, P_GEN1, P_GEN2),
    E2(0x01, TEXT_ADDF, FLOAT, M_FENR, M_FENW, P_GEN1, P_GEN2),
    E2(0x04, TEXT_MOVL, LONG,  M_FENR, M_FENW, P_GEN1, P_GEN2),
    E2(0x05, TEXT_MOVF, FLOAT, M_FENR, M_FENW, P_GEN1, P_GEN2),
    E2(0x08, TEXT_CMPL, LONG,  M_FENR, M_FENR, P_GEN1, P_GEN2),
    E2(0x09, TEXT_CMPF, FLOAT, M_FENR, M_FENR, P_GEN1, P_GEN2),
    E2(0x10, TEXT_SUBL, LONG,  M_FENR, M_FENW, P_GEN1, P_GEN2),
    E2(0x11, TEXT_SUBF, FLOAT, M_FENR, M_FENW, P_GEN1, P_GEN2),
    E2(0x14, TEXT_NEGL, LONG,  M_FENR, M_FENW, P_GEN1, P_GEN2),
    E2(0x15, TEXT_NEGF, FLOAT, M_FENR, M_FENW, P_GEN1, P_GEN2),
    E2(0x20, TEXT_DIVL, LONG,  M_FENR, M_FENW, P_GEN1, P_GEN2),
    E2(0x21, TEXT_DIVF, FLOAT, M_FENR, M_FENW, P_GEN1, P_GEN2),
    E2(0x30, TEXT_MULL, LONG,  M_FENR, M_FENW, P_GEN1, P_GEN2),
    E2(0x31, TEXT_MULF, FLOAT, M_FENR, M_FENW, P_GEN1, P_GEN2),
    E2(0x34, TEXT_ABSL, LONG,  M_FENR, M_FENW, P_GEN1, P_GEN2),
    E2(0x35, TEXT_ABSF, FLOAT, M_FENR, M_FENW, P_GEN1, P_GEN2),
};
static constexpr uint8_t INDEX_11[] PROGMEM = {
     15,  // TEXT_ABSF
     14,  // TEXT_ABSL
      1,  // TEXT_ADDF
      0,  // TEXT_ADDL
      5,  // TEXT_CMPF
      4,  // TEXT_CMPL
     11,  // TEXT_DIVF
     10,  // TEXT_DIVL
      3,  // TEXT_MOVF
      2,  // TEXT_MOVL
     13,  // TEXT_MULF
     12,  // TEXT_MULL
      9,  // TEXT_NEGF
      8,  // TEXT_NEGL
      7,  // TEXT_SUBF
      6,  // TEXT_SUBL
};

// Format 14: |gen1_|sho| |t|0|_op_|ii| |0001|1110|
static constexpr Entry FORMAT_14_1[] PROGMEM = {
    E1(0x03, TEXT_RDVAL, DOUBLE, M_GENA, P_GEN1),
    E1(0x07, TEXT_WRVAL, DOUBLE, M_GENA, P_GEN1),
};
static constexpr Entry FORMAT_14_2[] PROGMEM = {
    E2(0x0B, TEXT_LMR, DOUBLE, M_MREG, M_GENR, P_SHORT, P_GEN1),
    E2(0x0F, TEXT_SMR, DOUBLE, M_MREG, M_GENW, P_SHORT, P_GEN1),
};
static constexpr uint8_t INDEX_14_1[] PROGMEM = {
      0,  // TEXT_RDVAL
      1,  // TEXT_WRVAL
};
static constexpr uint8_t INDEX_14_2[] PROGMEM = {
      0,  // TEXT_LMR
      1,  // TEXT_SMR
};
// clang-format on

struct EntryPage : EntryTableBase<Entry> {
    Config::opcode_t mask() const { return pgm_read_byte(&_mask); }
    Config::opcode_t post() const { return pgm_read_byte(&_post); }

    constexpr EntryPage(Config::opcode_t prefix, Config::opcode_t mask, uint8_t post,
            const Entry *table, const Entry *end, const uint8_t *index, const uint8_t *iend)
        : EntryTableBase(prefix, table, end, index, iend), _mask(mask), _post(post) {}

private:
    const Config::opcode_t _mask;
    const uint8_t _post;
};

// Standard Instructions
static constexpr EntryPage NS32032_PAGES[] PROGMEM = {
        {0x00, 0x00, 0, ARRAY_RANGE(FORMAT_0), ARRAY_RANGE(INDEX_0)},
        {0x00, 0x00, 0, ARRAY_RANGE(FORMAT_1), ARRAY_RANGE(INDEX_1)},
        {0x00, 0xC0, 1, ARRAY_RANGE(FORMAT_4), ARRAY_RANGE(INDEX_4)},
        {0x00, 0x80, 1, ARRAY_RANGE(FORMAT_2_0), ARRAY_RANGE(INDEX_2_0)},
        {0x3C, 0xF8, 0, ARRAY_RANGE(FORMAT_2_1), ARRAY_RANGE(INDEX_2_1)},
        {0xBC, 0xF8, 0, ARRAY_RANGE(FORMAT_2_2), ARRAY_RANGE(INDEX_2_2)},
        {0x3D, 0xF8, 0, ARRAY_RANGE(FORMAT_2_3), ARRAY_RANGE(INDEX_2_3)},
        {0xBD, 0xF8, 0, ARRAY_RANGE(FORMAT_2_4), ARRAY_RANGE(INDEX_2_4)},
        {0x3F, 0xF8, 0, ARRAY_RANGE(FORMAT_2_5), ARRAY_RANGE(INDEX_2_5)},
        {0xBF, 0xF8, 0, ARRAY_RANGE(FORMAT_2_6), ARRAY_RANGE(INDEX_2_6)},
        {0x7C, 0xF8, 0, ARRAY_RANGE(FORMAT_3_1), ARRAY_RANGE(INDEX_3_1)},
        {0x7D, 0xF8, 0, ARRAY_RANGE(FORMAT_3_2), ARRAY_RANGE(INDEX_3_2)},
        {0x7F, 0xF8, 0, ARRAY_RANGE(FORMAT_3_3), ARRAY_RANGE(INDEX_3_3)},
        {0x0E, 0x80, 1, ARRAY_RANGE(FORMAT_5_0), ARRAY_RANGE(INDEX_5_0)},
        {0x0E, 0x00, 1, ARRAY_RANGE(FORMAT_5_1), ARRAY_RANGE(INDEX_5_1)},
        {0x4E, 0xC0, 1, ARRAY_RANGE(FORMAT_6), ARRAY_RANGE(INDEX_6)},
        {0xCE, 0xC0, 1, ARRAY_RANGE(FORMAT_7), ARRAY_RANGE(INDEX_7)},
        {0x2E, 0xF8, 1, ARRAY_RANGE(FORMAT_8_1), ARRAY_RANGE(INDEX_8_1)},
        {0x6E, 0xF8, 1, ARRAY_RANGE(FORMAT_8_2), ARRAY_RANGE(INDEX_8_2)},
        {0xAE, 0xF8, 1, ARRAY_RANGE(FORMAT_8_3), ARRAY_RANGE(INDEX_8_3)},
        {0xEE, 0xF8, 1, ARRAY_RANGE(FORMAT_8_4), ARRAY_RANGE(INDEX_8_4)},
};

// Floating point instructions
static constexpr EntryPage NS32081_PAGES[] PROGMEM = {
        {0x3E, 0xC0, 1, ARRAY_RANGE(FORMAT_9), ARRAY_RANGE(INDEX_9)},
        {0xBE, 0xC0, 1, ARRAY_RANGE(FORMAT_11), ARRAY_RANGE(INDEX_11)},
};

// Memory management instructions
static constexpr EntryPage NS32082_PAGES[] PROGMEM = {
        {0xAE, 0xC0, 1, ARRAY_RANGE(FORMAT_8_3_1), ARRAY_RANGE(INDEX_8_3_1)},
        {0x1E, 0x00, 1, ARRAY_RANGE(FORMAT_14_1), ARRAY_RANGE(INDEX_14_1)},
        {0x1E, 0x80, 1, ARRAY_RANGE(FORMAT_14_2), ARRAY_RANGE(INDEX_14_2)},
};

template <typename CPUTYPE>
struct ProcessorCpuCommon : CpuBase<CPUTYPE, EntryPage> {
    constexpr ProcessorCpuCommon(CPUTYPE cpuType, const /* PROGMEM */ char *name_P,
            const EntryPage *table, const EntryPage *end)
        : CpuBase<CPUTYPE, EntryPage>(cpuType, name_P, table, end) {}

    Error searchName(InsnNs32000 &insn, bool (*accept)(InsnNs32000 &, const Entry *),
            void (*pageSetup)(InsnNs32000 &, const EntryPage *)) const {
        CpuBase<CPUTYPE, EntryPage>::searchName(insn, accept, pageSetup);
        return insn.getError();
    }

    Error searchOpCode(InsnNs32000 &insn, DisMemory &memory,
            bool (*matchOpCode)(InsnNs32000 &, const Entry *, const EntryPage *),
            void (*readEntryName)(InsnNs32000 &, const Entry *, const EntryPage *)) const {
        const auto entry =
                CpuBase<CPUTYPE, EntryPage>::searchOpCode(insn, matchOpCode, readEntryName);
        if (entry && insn.hasPost())
            insn.readPost(memory);
        return insn.getError();
    }
};

struct TableNs32000::Cpu : ProcessorCpuCommon<CpuType> {
    constexpr Cpu(CpuType cpuType, const /* PROGMEM */ char *name_P, const EntryPage *table,
            const EntryPage *end)
        : ProcessorCpuCommon<CpuType>(cpuType, name_P, table, end) {}
};
struct TableNs32000::Fpu : ProcessorCpuCommon<FpuType> {
    constexpr Fpu(FpuType fpuType, const /* PROGMEM */ char *name_P, const EntryPage *table,
            const EntryPage *end)
        : ProcessorCpuCommon<FpuType>(fpuType, name_P, table, end) {}
};
struct TableNs32000::Mmu : ProcessorCpuCommon<MmuType> {
    constexpr Mmu(MmuType mmuType, const /* PROGMEM */ char *name_P, const EntryPage *table,
            const EntryPage *end)
        : ProcessorCpuCommon<MmuType>(mmuType, name_P, table, end) {}
};

static constexpr TableNs32000::Cpu CPU_TABLE[] PROGMEM = {
        {NS32032, TEXT_CPU_32032, ARRAY_RANGE(NS32032_PAGES)},
};
static constexpr const TableNs32000::Cpu &NS32032_CPU = CPU_TABLE[0];

#define EMPTY_RANGE(a) ARRAY_BEGIN(a), ARRAY_BEGIN(a)

static constexpr TableNs32000::Fpu FPU_TABLE[] PROGMEM = {
        {FPU_NS32081, TEXT_FPU_NS32081, ARRAY_RANGE(NS32081_PAGES)},
        {FPU_NONE, TEXT_none, EMPTY_RANGE(NS32081_PAGES)},
};
static constexpr const TableNs32000::Fpu &NS32081_FPU = FPU_TABLE[0];

static constexpr TableNs32000::Mmu MMU_TABLE[] PROGMEM = {
        {MMU_NS32082, TEXT_MMU_NS32082, ARRAY_RANGE(NS32082_PAGES)},
        {MMU_NONE, TEXT_none, EMPTY_RANGE(NS32082_PAGES)},
};
static constexpr const TableNs32000::Mmu &NS32082_MMU = MMU_TABLE[0];

bool TableNs32000::isPrefixCode(uint8_t code) const {
    return NS32032_CPU.isPrefix(code) || NS32081_FPU.isPrefix(code) || NS32082_MMU.isPrefix(code);
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

static void searchPageSetup(InsnNs32000 &insn, const EntryPage *page) {
    insn.setPost(0, page->post() != 0);
}

static bool acceptModes(InsnNs32000 &insn, const Entry *entry) {
    auto flags = insn.flags();
    auto table = entry->flags();
    return acceptMode(flags.src(), table.src()) && acceptMode(flags.dst(), table.dst()) &&
           acceptMode(flags.ex1(), table.ex1()) && acceptMode(flags.ex2(), table.ex2());
}

Error TableNs32000::searchName(InsnNs32000 &insn) const {
    _cpu->searchName(insn, acceptModes, searchPageSetup);
    if (insn.getError() == UNKNOWN_INSTRUCTION) {
        insn.setOK();
        _fpu->searchName(insn, acceptModes, searchPageSetup);
    }
    if (insn.getError() == UNKNOWN_INSTRUCTION) {
        insn.setOK();
        _mmu->searchName(insn, acceptModes, searchPageSetup);
    }
    return insn.getError();
}

static bool matchOpCode(InsnNs32000 &insn, const Entry *entry, const EntryPage *page) {
    auto opCode = insn.opCode();
    opCode &= ~page->mask();
    return opCode == entry->opCode();
}

static void readEntryName(InsnNs32000 &insn, const Entry *entry, const EntryPage *page) {
    TableNs32000::Cpu::defaultReadEntryName(insn, entry, page);
    insn.setPost(0, page->post() != 0);
}

Error TableNs32000::searchOpCode(InsnNs32000 &insn, DisMemory &memory) const {
    NS32032_CPU.searchOpCode(insn, memory, matchOpCode, readEntryName);
    if (insn.getError() == UNKNOWN_INSTRUCTION) {
        insn.setOK();
        NS32081_FPU.searchOpCode(insn, memory, matchOpCode, readEntryName);
    }
    if (insn.getError() == UNKNOWN_INSTRUCTION) {
        insn.setOK();
        NS32082_MMU.searchOpCode(insn, memory, matchOpCode, readEntryName);
    }
    return insn.getError();
}

TableNs32000::TableNs32000() : _cpu(&NS32032_CPU) {
    reset();
}

void TableNs32000::reset() {
    setFpu(FPU_NONE);
    setMmu(MMU_NONE);
}

bool TableNs32000::setFpu(FpuType fpuType) {
    auto t = Fpu::search(fpuType, ARRAY_RANGE(FPU_TABLE));
    if (t == nullptr)
        return false;
    _fpu = t;
    return true;
}

bool TableNs32000::setMmu(MmuType mmuType) {
    auto t = Mmu::search(mmuType, ARRAY_RANGE(MMU_TABLE));
    if (t == nullptr)
        return false;
    _mmu = t;
    return true;
}

const /* PROGMEM */ char *TableNs32000::listCpu_P() const {
    return TEXT_CPU_LIST;
}

const /* PROGMEM */ char *TableNs32000::cpu_P() const {
    return _cpu->name_P();
}

bool TableNs32000::setCpu(const char *cpu) {
    if (strncasecmp_P(cpu, TEXT_CPU_NS, 2) == 0)
        cpu += 2;
    return strcasecmp_P(cpu, TEXT_CPU_32032) == 0;
}

bool TableNs32000::setFpu(const StrScanner &fpu) {
    if (fpu.iequals_P(TEXT_FPU_NS32081))
        return setFpu(FPU_NS32081);
    if (fpu.iequals_P(TEXT_none))
        return setFpu(FPU_NONE);
    return false;
}

bool TableNs32000::setMmu(const StrScanner &mmu) {
    if (mmu.iequals_P(TEXT_MMU_NS32082))
        return setMmu(MMU_NS32082);
    if (mmu.iequals_P(TEXT_none))
        return setMmu(MMU_NONE);
    return false;
}

TableNs32000 TableNs32000::TABLE;

}  // namespace ns32000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

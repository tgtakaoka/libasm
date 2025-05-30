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
#include "entry_ns32000.h"
#include "entry_table.h"
#include "text_ns32000.h"

using namespace libasm::text::ns32000;

namespace libasm {
namespace ns32000 {

#define E4(_opc, _name, _sz, _srcm, _dstm, _srcp, _dstp, _ex1m, _ex2m, _ex1p, _ex2p) \
    {_opc, Entry::Flags::create(_srcm, _srcp, _dstm, _dstp, _ex1m, _ex1p, _ex2m, _ex2p, _sz), _name}
#define E3(_opc, _name, _sz, _srcm, _dstm, _srcp, _dstp, _ex1m, _ex1p) \
    E4(_opc, _name, _sz, _srcm, _dstm, _srcp, _dstp, _ex1m, EM2_NONE, _ex1p, EP2_NONE)
#define E2(_opc, _name, _sz, _srcm, _dstm, _srcp, _dstp) \
    E3(_opc, _name, _sz, _srcm, _dstm, _srcp, _dstp, M_NONE, P_NONE)
#define E1(_opc, _name, _sz, _srcm, _srcp) E2(_opc, _name, _sz, _srcm, M_NONE, _srcp, P_NONE)
#define E0(_opc, _name) E1(_opc, _name, SZ_NONE, M_NONE, P_NONE)
#define W2(_opc, _name, _sz, _srcm, _dstm, _srcp, _dstp, _socta, _docta)                         \
    E4(_opc, _name, _sz, _srcm, _dstm, _srcp, _dstp, M_NONE, EM2_NONE, _socta ? P_IMPL : P_NONE, \
            _docta ? EP2_IMPL : EP2_NONE)

// clang-format off
// Format 0: |cond|1010|
constexpr Entry FORMAT_0[] PROGMEM = {
    E1(0x0A, TEXT_BEQ, SZ_NONE, M_REL, P_DISP),
    E1(0x1A, TEXT_BNE, SZ_NONE, M_REL, P_DISP),
    E1(0x2A, TEXT_BCS, SZ_NONE, M_REL, P_DISP),
    E1(0x3A, TEXT_BCC, SZ_NONE, M_REL, P_DISP),
    E1(0x4A, TEXT_BHI, SZ_NONE, M_REL, P_DISP),
    E1(0x5A, TEXT_BLS, SZ_NONE, M_REL, P_DISP),
    E1(0x6A, TEXT_BGT, SZ_NONE, M_REL, P_DISP),
    E1(0x7A, TEXT_BLE, SZ_NONE, M_REL, P_DISP),
    E1(0x8A, TEXT_BFS, SZ_NONE, M_REL, P_DISP),
    E1(0x9A, TEXT_BFC, SZ_NONE, M_REL, P_DISP),
    E1(0xAA, TEXT_BLO, SZ_NONE, M_REL, P_DISP),
    E1(0xBA, TEXT_BHS, SZ_NONE, M_REL, P_DISP),
    E1(0xCA, TEXT_BLT, SZ_NONE, M_REL, P_DISP),
    E1(0xDA, TEXT_BGE, SZ_NONE, M_REL, P_DISP),
    E1(0xEA, TEXT_BR,  SZ_NONE, M_REL, P_DISP),
};
constexpr uint8_t INDEX_0[] PROGMEM = {
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
constexpr Entry FORMAT_1[] PROGMEM = {
    E1(0x02, TEXT_BSR,     SZ_NONE,   M_REL, P_DISP),
    E1(0x12, TEXT_RET,     SZ_NONE,   M_DISP, P_DISP),
    E1(0x22, TEXT_CXP,     SZ_NONE,   M_DISP, P_DISP),
    E1(0x32, TEXT_RXP,     SZ_NONE,   M_DISP, P_DISP),
    E1(0x42, TEXT_RETT,    SZ_NONE,   M_DISP, P_DISP),
    E0(0x52, TEXT_RETI),
    E1(0x62, TEXT_SAVE,    SZ_NONE,   M_RLST, P_IMPL),
    E1(0x72, TEXT_RESTORE, SZ_QUAD, M_RLST, P_IMPL),
    E2(0x82, TEXT_ENTER,   SZ_NONE,   M_RLST, M_DISP, P_IMPL, P_DISP),
    E1(0x92, TEXT_EXIT,    SZ_QUAD, M_RLST, P_IMPL),
    E0(0xA2, TEXT_NOP),
    E0(0xB2, TEXT_WAIT),
    E0(0xC2, TEXT_DIA),
    E0(0xD2, TEXT_FLAG),
    E0(0xE2, TEXT_SVC),
    E0(0xF2, TEXT_BPT),
};
constexpr uint8_t INDEX_1[] PROGMEM = {
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
constexpr Entry FORMAT_4[] PROGMEM = {
    E2(0x00, TEXT_ADDB,   SZ_BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x01, TEXT_ADDW,   SZ_WORD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x03, TEXT_ADDD,   SZ_QUAD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x04, TEXT_CMPB,   SZ_BYTE, M_GENR, M_GENR, P_GEN1, P_GEN2),
    E2(0x05, TEXT_CMPW,   SZ_WORD, M_GENR, M_GENR, P_GEN1, P_GEN2),
    E2(0x07, TEXT_CMPD,   SZ_QUAD, M_GENR, M_GENR, P_GEN1, P_GEN2),
    E2(0x08, TEXT_BICB,   SZ_BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x09, TEXT_BICW,   SZ_WORD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x0B, TEXT_BICD,   SZ_QUAD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x10, TEXT_ADDCB,  SZ_BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x11, TEXT_ADDCW,  SZ_WORD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x13, TEXT_ADDCD,  SZ_QUAD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x14, TEXT_MOVB,   SZ_BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x15, TEXT_MOVW,   SZ_WORD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x17, TEXT_MOVD,   SZ_QUAD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x18, TEXT_ORB,    SZ_BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x19, TEXT_ORW,    SZ_WORD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x1B, TEXT_ORD,    SZ_QUAD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x20, TEXT_SUBB,   SZ_BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x21, TEXT_SUBW,   SZ_WORD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x23, TEXT_SUBD,   SZ_QUAD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x27, TEXT_ADDR,   SZ_QUAD, M_GENA, M_GENW, P_GEN1, P_GEN2),
    E2(0x28, TEXT_ANDB,   SZ_BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x29, TEXT_ANDW,   SZ_WORD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x2B, TEXT_ANDD,   SZ_QUAD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x30, TEXT_SUBCB,  SZ_BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x31, TEXT_SUBCW,  SZ_WORD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x33, TEXT_SUBCD,  SZ_QUAD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x34, TEXT_TBITB,  SZ_BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x35, TEXT_TBITW,  SZ_WORD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x37, TEXT_TBITD,  SZ_QUAD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x38, TEXT_XORB,   SZ_BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x39, TEXT_XORW,   SZ_WORD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x3B, TEXT_XORD,   SZ_QUAD, M_GENR, M_GENW, P_GEN1, P_GEN2),
};
constexpr uint8_t INDEX_4[] PROGMEM = {
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
constexpr Entry FORMAT_2_0[] PROGMEM = {
    E2(0x0C, TEXT_ADDQB, SZ_BYTE, M_INT4, M_GENW, P_SHORT, P_GEN1),
    E2(0x0D, TEXT_ADDQW, SZ_WORD, M_INT4, M_GENW, P_SHORT, P_GEN1),
    E2(0x0F, TEXT_ADDQD, SZ_QUAD, M_INT4, M_GENW, P_SHORT, P_GEN1),
    E2(0x1C, TEXT_CMPQB, SZ_BYTE, M_INT4, M_GENR, P_SHORT, P_GEN1),
    E2(0x1D, TEXT_CMPQW, SZ_WORD, M_INT4, M_GENR, P_SHORT, P_GEN1),
    E2(0x1F, TEXT_CMPQD, SZ_QUAD, M_INT4, M_GENR, P_SHORT, P_GEN1),
    E2(0x2C, TEXT_SPRB,  SZ_BYTE, M_PREG, M_GENW, P_SHORT, P_GEN1),
    E2(0x2D, TEXT_SPRW,  SZ_WORD, M_PREG, M_GENW, P_SHORT, P_GEN1),
    E2(0x2F, TEXT_SPRD,  SZ_QUAD, M_PREG, M_GENW, P_SHORT, P_GEN1),
    E3(0x4C, TEXT_ACBB,  SZ_BYTE, M_INT4, M_GENW, P_SHORT, P_GEN1, M_REL, P_DISP),
    E3(0x4D, TEXT_ACBW,  SZ_WORD, M_INT4, M_GENW, P_SHORT, P_GEN1, M_REL, P_DISP),
    E3(0x4F, TEXT_ACBD,  SZ_QUAD, M_INT4, M_GENW, P_SHORT, P_GEN1, M_REL, P_DISP),
    E2(0x5C, TEXT_MOVQB, SZ_BYTE, M_INT4, M_GENW, P_SHORT, P_GEN1),
    E2(0x5D, TEXT_MOVQW, SZ_WORD, M_INT4, M_GENW, P_SHORT, P_GEN1),
    E2(0x5F, TEXT_MOVQD, SZ_QUAD, M_INT4, M_GENW, P_SHORT, P_GEN1),
    E2(0x6C, TEXT_LPRB,  SZ_BYTE, M_PREG, M_GENR, P_SHORT, P_GEN1),
    E2(0x6D, TEXT_LPRW,  SZ_WORD, M_PREG, M_GENR, P_SHORT, P_GEN1),
    E2(0x6F, TEXT_LPRD,  SZ_QUAD, M_PREG, M_GENR, P_SHORT, P_GEN1),
};
// Format 2: |_gen_|con| |d|011|11|ii|
constexpr Entry FORMAT_2_1[] PROGMEM = {
    E1(0x00, TEXT_SEQB, SZ_BYTE, M_GENW, P_GEN1),
    E1(0x01, TEXT_SCSB, SZ_BYTE, M_GENW, P_GEN1),
    E1(0x02, TEXT_SHIB, SZ_BYTE, M_GENW, P_GEN1),
    E1(0x03, TEXT_SGTB, SZ_BYTE, M_GENW, P_GEN1),
    E1(0x04, TEXT_SFSB, SZ_BYTE, M_GENW, P_GEN1),
    E1(0x05, TEXT_SLOB, SZ_BYTE, M_GENW, P_GEN1),
    E1(0x06, TEXT_SLTB, SZ_BYTE, M_GENW, P_GEN1),
};
constexpr Entry FORMAT_2_2[] PROGMEM = {
    E1(0x00, TEXT_SNEB, SZ_BYTE, M_GENW, P_GEN1),
    E1(0x01, TEXT_SCCB, SZ_BYTE, M_GENW, P_GEN1),
    E1(0x02, TEXT_SLSB, SZ_BYTE, M_GENW, P_GEN1),
    E1(0x03, TEXT_SLEB, SZ_BYTE, M_GENW, P_GEN1),
    E1(0x04, TEXT_SFCB, SZ_BYTE, M_GENW, P_GEN1),
    E1(0x05, TEXT_SHSB, SZ_BYTE, M_GENW, P_GEN1),
    E1(0x06, TEXT_SGEB, SZ_BYTE, M_GENW, P_GEN1),
};
constexpr Entry FORMAT_2_3[] PROGMEM = {
    E1(0x00, TEXT_SEQW, SZ_WORD, M_GENW, P_GEN1),
    E1(0x01, TEXT_SCSW, SZ_WORD, M_GENW, P_GEN1),
    E1(0x02, TEXT_SHIW, SZ_WORD, M_GENW, P_GEN1),
    E1(0x03, TEXT_SGTW, SZ_WORD, M_GENW, P_GEN1),
    E1(0x04, TEXT_SFSW, SZ_WORD, M_GENW, P_GEN1),
    E1(0x05, TEXT_SLOW, SZ_WORD, M_GENW, P_GEN1),
    E1(0x06, TEXT_SLTW, SZ_WORD, M_GENW, P_GEN1),
};
constexpr Entry FORMAT_2_4[] PROGMEM = {
    E1(0x00, TEXT_SNEW, SZ_WORD, M_GENW, P_GEN1),
    E1(0x01, TEXT_SCCW, SZ_WORD, M_GENW, P_GEN1),
    E1(0x02, TEXT_SLSW, SZ_WORD, M_GENW, P_GEN1),
    E1(0x03, TEXT_SLEW, SZ_WORD, M_GENW, P_GEN1),
    E1(0x04, TEXT_SFCW, SZ_WORD, M_GENW, P_GEN1),
    E1(0x05, TEXT_SHSW, SZ_WORD, M_GENW, P_GEN1),
    E1(0x06, TEXT_SGEW, SZ_WORD, M_GENW, P_GEN1),
};
constexpr Entry FORMAT_2_5[] PROGMEM = {
    E1(0x00, TEXT_SEQD, SZ_QUAD, M_GENW, P_GEN1),
    E1(0x01, TEXT_SCSD, SZ_QUAD, M_GENW, P_GEN1),
    E1(0x02, TEXT_SHID, SZ_QUAD, M_GENW, P_GEN1),
    E1(0x03, TEXT_SGTD, SZ_QUAD, M_GENW, P_GEN1),
    E1(0x04, TEXT_SFSD, SZ_QUAD, M_GENW, P_GEN1),
    E1(0x05, TEXT_SLOD, SZ_QUAD, M_GENW, P_GEN1),
    E1(0x06, TEXT_SLTD, SZ_QUAD, M_GENW, P_GEN1),
};
constexpr Entry FORMAT_2_6[] PROGMEM = {
    E1(0x00, TEXT_SNED, SZ_QUAD, M_GENW, P_GEN1),
    E1(0x01, TEXT_SCCD, SZ_QUAD, M_GENW, P_GEN1),
    E1(0x02, TEXT_SLSD, SZ_QUAD, M_GENW, P_GEN1),
    E1(0x03, TEXT_SLED, SZ_QUAD, M_GENW, P_GEN1),
    E1(0x04, TEXT_SFCD, SZ_QUAD, M_GENW, P_GEN1),
    E1(0x05, TEXT_SHSD, SZ_QUAD, M_GENW, P_GEN1),
    E1(0x06, TEXT_SGED, SZ_QUAD, M_GENW, P_GEN1),
};
constexpr uint8_t INDEX_2_0[] PROGMEM = {
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
constexpr uint8_t INDEX_2_1[] PROGMEM = {
      1,  // TEXT_SCSB
      0,  // TEXT_SEQB
      4,  // TEXT_SFSB
      3,  // TEXT_SGTB
      2,  // TEXT_SHIB
      5,  // TEXT_SLOB
      6,  // TEXT_SLTB
};
constexpr uint8_t INDEX_2_2[] PROGMEM = {
      1,  // TEXT_SCCB
      4,  // TEXT_SFCB
      6,  // TEXT_SGEB
      5,  // TEXT_SHSB
      3,  // TEXT_SLEB
      2,  // TEXT_SLSB
      0,  // TEXT_SNEB
};
constexpr uint8_t INDEX_2_3[] PROGMEM = {
      1,  // TEXT_SCSW
      0,  // TEXT_SEQW
      4,  // TEXT_SFSW
      3,  // TEXT_SGTW
      2,  // TEXT_SHIW
      5,  // TEXT_SLOW
      6,  // TEXT_SLTW
};
constexpr uint8_t INDEX_2_4[] PROGMEM = {
      1,  // TEXT_SCCW
      4,  // TEXT_SFCW
      6,  // TEXT_SGEW
      5,  // TEXT_SHSW
      3,  // TEXT_SLEW
      2,  // TEXT_SLSW
      0,  // TEXT_SNEW
};
constexpr uint8_t INDEX_2_5[] PROGMEM = {
      1,  // TEXT_SCSD
      0,  // TEXT_SEQD
      4,  // TEXT_SFSD
      3,  // TEXT_SGTD
      2,  // TEXT_SHID
      5,  // TEXT_SLOD
      6,  // TEXT_SLTD
};
constexpr uint8_t INDEX_2_6[] PROGMEM = {
      1,  // TEXT_SCCD
      4,  // TEXT_SFCD
      6,  // TEXT_SGED
      5,  // TEXT_SHSD
      3,  // TEXT_SLED
      2,  // TEXT_SLSD
      0,  // TEXT_SNED
};

// Format 3: |_gen_|_op| |011111|ii|
constexpr Entry FORMAT_3_1[] PROGMEM = {
    E1(0x01, TEXT_BICPSRB, SZ_BYTE, M_GENR, P_GEN1),
    E1(0x03, TEXT_BISPSRB, SZ_BYTE, M_GENR, P_GEN1),
    E1(0x05, TEXT_ADJSPB,  SZ_BYTE, M_GENR, P_GEN1),
    E1(0x07, TEXT_CASEB,   SZ_BYTE, M_GENR, P_GEN1),
};
constexpr Entry FORMAT_3_2[] PROGMEM = {
    E1(0x01, TEXT_BICPSRW, SZ_WORD, M_GENR, P_GEN1),
    E1(0x03, TEXT_BISPSRW, SZ_WORD, M_GENR, P_GEN1),
    E1(0x05, TEXT_ADJSPW,  SZ_WORD, M_GENR, P_GEN1),
    E1(0x07, TEXT_CASEW,   SZ_WORD, M_GENR, P_GEN1),
};
constexpr Entry FORMAT_3_3[] PROGMEM = {
    E1(0x00, TEXT_CXPD,   SZ_QUAD, M_GENA, P_GEN1),
    E1(0x02, TEXT_JUMP,   SZ_QUAD, M_GENA, P_GEN1),
    E1(0x05, TEXT_ADJSPD, SZ_QUAD, M_GENR, P_GEN1),
    E1(0x06, TEXT_JSR,    SZ_QUAD, M_GENA, P_GEN1),
    E1(0x07, TEXT_CASED,  SZ_QUAD, M_GENR, P_GEN1),
};
constexpr uint8_t INDEX_3_1[] PROGMEM = {
      2,  // TEXT_ADJSPB
      0,  // TEXT_BICPSRB
      1,  // TEXT_BISPSRB
      3,  // TEXT_CASEB
};
constexpr uint8_t INDEX_3_2[] PROGMEM = {
      2,  // TEXT_ADJSPW
      0,  // TEXT_BICPSRW
      1,  // TEXT_BISPSRW
      3,  // TEXT_CASEW
};
constexpr uint8_t INDEX_3_3[] PROGMEM = {
      2,  // TEXT_ADJSPD
      4,  // TEXT_CASED
      0,  // TEXT_CXPD
      3,  // TEXT_JSR
      1,  // TEXT_JUMP
};

// Format 5: |00000|sho| |t|0|_op_|ii| |0000|1110|
constexpr Entry FORMAT_5_0[] PROGMEM = {
    E2(0x0B, TEXT_SETCFG, SZ_NONE, M_CONF, M_ZERO, P_SHORT, P_GEN1),
};
// Format 5: |00000|fla| |g|0|_op_|ii| |0000|1110|
constexpr Entry FORMAT_5_1[] PROGMEM = {
    E2(0x00, TEXT_MOVSB, SZ_BYTE, M_SOPT, M_ZERO, P_SHORT, P_GEN1),
    E2(0x01, TEXT_MOVSW, SZ_WORD, M_SOPT, M_ZERO, P_SHORT, P_GEN1),
    E2(0x03, TEXT_MOVSD, SZ_QUAD, M_SOPT, M_ZERO, P_SHORT, P_GEN1),
    E2(0x80, TEXT_MOVST, SZ_BYTE, M_SOPT, M_ZERO, P_SHORT, P_GEN1),
    E2(0x04, TEXT_CMPSB, SZ_BYTE, M_SOPT, M_ZERO, P_SHORT, P_GEN1),
    E2(0x05, TEXT_CMPSW, SZ_WORD, M_SOPT, M_ZERO, P_SHORT, P_GEN1),
    E2(0x07, TEXT_CMPSD, SZ_QUAD, M_SOPT, M_ZERO, P_SHORT, P_GEN1),
    E2(0x84, TEXT_CMPST, SZ_BYTE, M_SOPT, M_ZERO, P_SHORT, P_GEN1),
    E2(0x0C, TEXT_SKPSB, SZ_BYTE, M_SOPT, M_ZERO, P_SHORT, P_GEN1),
    E2(0x0D, TEXT_SKPSW, SZ_WORD, M_SOPT, M_ZERO, P_SHORT, P_GEN1),
    E2(0x0F, TEXT_SKPSD, SZ_QUAD, M_SOPT, M_ZERO, P_SHORT, P_GEN1),
    E2(0x8C, TEXT_SKPST, SZ_BYTE, M_SOPT, M_ZERO, P_SHORT, P_GEN1),
};
constexpr uint8_t INDEX_5_0[] PROGMEM = {
      0,  // TEXT_SETCFG
};
constexpr uint8_t INDEX_5_1[] PROGMEM = {
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
constexpr Entry FORMAT_6[] PROGMEM = {
    E2(0x00, TEXT_ROTB,   SZ_BYTE, M_GENC, M_GENW, P_GEN1, P_GEN2),
    E2(0x01, TEXT_ROTW,   SZ_WORD, M_GENC, M_GENW, P_GEN1, P_GEN2),
    E2(0x03, TEXT_ROTD,   SZ_QUAD, M_GENC, M_GENW, P_GEN1, P_GEN2),
    E2(0x04, TEXT_ASHB,   SZ_BYTE, M_GENC, M_GENW, P_GEN1, P_GEN2),
    E2(0x05, TEXT_ASHW,   SZ_WORD, M_GENC, M_GENW, P_GEN1, P_GEN2),
    E2(0x07, TEXT_ASHD,   SZ_QUAD, M_GENC, M_GENW, P_GEN1, P_GEN2),
    E2(0x08, TEXT_CBITB,  SZ_BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x09, TEXT_CBITW,  SZ_WORD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x0B, TEXT_CBITD,  SZ_QUAD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x0C, TEXT_CBITIB, SZ_BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x0D, TEXT_CBITIW, SZ_WORD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x0F, TEXT_CBITID, SZ_QUAD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x14, TEXT_LSHB,   SZ_BYTE, M_GENC, M_GENW, P_GEN1, P_GEN2),
    E2(0x15, TEXT_LSHW,   SZ_WORD, M_GENC, M_GENW, P_GEN1, P_GEN2),
    E2(0x17, TEXT_LSHD,   SZ_QUAD, M_GENC, M_GENW, P_GEN1, P_GEN2),
    E2(0x18, TEXT_SBITB,  SZ_BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x19, TEXT_SBITW,  SZ_WORD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x1B, TEXT_SBITD,  SZ_QUAD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x1C, TEXT_SBITIB, SZ_BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x1D, TEXT_SBITIW, SZ_WORD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x1F, TEXT_SBITID, SZ_QUAD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x20, TEXT_NEGB,   SZ_BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x21, TEXT_NEGW,   SZ_WORD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x23, TEXT_NEGD,   SZ_QUAD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x24, TEXT_NOTB,   SZ_BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x25, TEXT_NOTW,   SZ_WORD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x27, TEXT_NOTD,   SZ_QUAD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x2C, TEXT_SUBPB,  SZ_BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x2D, TEXT_SUBPW,  SZ_WORD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x2F, TEXT_SUBPD,  SZ_QUAD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x30, TEXT_ABSB,   SZ_BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x31, TEXT_ABSW,   SZ_WORD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x33, TEXT_ABSD,   SZ_QUAD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x34, TEXT_COMB,   SZ_BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x35, TEXT_COMW,   SZ_WORD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x37, TEXT_COMD,   SZ_QUAD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x38, TEXT_IBITB,  SZ_BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x39, TEXT_IBITW,  SZ_WORD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x3B, TEXT_IBITD,  SZ_QUAD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x3C, TEXT_ADDPB,  SZ_BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x3D, TEXT_ADDPW,  SZ_WORD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x3F, TEXT_ADDPD,  SZ_QUAD, M_GENR, M_GENW, P_GEN1, P_GEN2),
};
constexpr uint8_t INDEX_6[] PROGMEM = {
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
constexpr Entry FORMAT_7[] PROGMEM = {
    E3(0x00, TEXT_MOVMB,  SZ_BYTE, M_GENA, M_GENA, P_GEN1, P_GEN2, M_LEN16,  P_DISP),
    E3(0x01, TEXT_MOVMW,  SZ_WORD, M_GENA, M_GENA, P_GEN1, P_GEN2, M_LEN16,  P_DISP),
    E3(0x03, TEXT_MOVMD,  SZ_QUAD, M_GENA, M_GENA, P_GEN1, P_GEN2, M_LEN16,  P_DISP),
    E3(0x04, TEXT_CMPMB,  SZ_BYTE, M_GENA, M_GENA, P_GEN1, P_GEN2, M_LEN16,  P_DISP),
    E3(0x05, TEXT_CMPMW,  SZ_WORD, M_GENA, M_GENA, P_GEN1, P_GEN2, M_LEN16,  P_DISP),
    E3(0x07, TEXT_CMPMD,  SZ_QUAD, M_GENA, M_GENA, P_GEN1, P_GEN2, M_LEN16,  P_DISP),
    E4(0x08, TEXT_INSSB,  SZ_BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2, M_BFOFF, EM2_BFLEN, P_IMPL, EP2_IMPL),
    E4(0x09, TEXT_INSSW,  SZ_WORD, M_GENR, M_GENW, P_GEN1, P_GEN2, M_BFOFF, EM2_BFLEN, P_IMPL, EP2_IMPL),
    E4(0x0B, TEXT_INSSD,  SZ_QUAD, M_GENR, M_GENW, P_GEN1, P_GEN2, M_BFOFF, EM2_BFLEN, P_IMPL, EP2_IMPL),
    E4(0x0C, TEXT_EXTSB,  SZ_BYTE, M_GENW, M_GENW, P_GEN1, P_GEN2, M_BFOFF, EM2_BFLEN, P_IMPL, EP2_IMPL),
    E4(0x0D, TEXT_EXTSW,  SZ_WORD, M_GENW, M_GENW, P_GEN1, P_GEN2, M_BFOFF, EM2_BFLEN, P_IMPL, EP2_IMPL),
    E4(0x0F, TEXT_EXTSD,  SZ_QUAD, M_GENW, M_GENW, P_GEN1, P_GEN2, M_BFOFF, EM2_BFLEN, P_IMPL, EP2_IMPL),
    E2(0x10, TEXT_MOVXBW, SZ_BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x1C, TEXT_MOVXBD, SZ_BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x1D, TEXT_MOVXWD, SZ_WORD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x14, TEXT_MOVZBW, SZ_BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x18, TEXT_MOVZBD, SZ_BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x19, TEXT_MOVZWD, SZ_WORD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x20, TEXT_MULB,   SZ_BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x21, TEXT_MULW,   SZ_WORD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x23, TEXT_MULD,   SZ_QUAD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    // gen2 must be register pair.
    W2(0x24, TEXT_MEIB,   SZ_BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2, false, true),
    W2(0x25, TEXT_MEIW,   SZ_WORD, M_GENR, M_GENW, P_GEN1, P_GEN2, false, true),
    W2(0x27, TEXT_MEID,   SZ_QUAD, M_GENR, M_GENW, P_GEN1, P_GEN2, false, true),
    W2(0x2C, TEXT_DEIB,   SZ_BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2, false, true),
    W2(0x2D, TEXT_DEIW,   SZ_WORD, M_GENR, M_GENW, P_GEN1, P_GEN2, false, true),
    W2(0x2F, TEXT_DEID,   SZ_QUAD, M_GENR, M_GENW, P_GEN1, P_GEN2, false, true),
    E2(0x30, TEXT_QUOB,   SZ_BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x31, TEXT_QUOW,   SZ_WORD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x33, TEXT_QUOD,   SZ_QUAD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x34, TEXT_REMB,   SZ_BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x35, TEXT_REMW,   SZ_WORD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x37, TEXT_REMD,   SZ_QUAD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x38, TEXT_MODB,   SZ_BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x39, TEXT_MODW,   SZ_WORD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x3B, TEXT_MODD,   SZ_QUAD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x3C, TEXT_DIVB,   SZ_BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x3D, TEXT_DIVW,   SZ_WORD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x3F, TEXT_DIVD,   SZ_QUAD, M_GENR, M_GENW, P_GEN1, P_GEN2),
};
constexpr uint8_t INDEX_7[] PROGMEM = {
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
constexpr Entry FORMAT_8_1[] PROGMEM = {
    E4(0x00, TEXT_EXTB,   SZ_BYTE, M_GREG, M_GENW, P_REG, P_GEN1, M_GENW, EM2_LEN32, P_GEN2, EP2_DISP),
    E4(0x01, TEXT_EXTW,   SZ_WORD, M_GREG, M_GENW, P_REG, P_GEN1, M_GENW, EM2_LEN32, P_GEN2, EP2_DISP),
    E4(0x03, TEXT_EXTD,   SZ_QUAD, M_GREG, M_GENW, P_REG, P_GEN1, M_GENW, EM2_LEN32, P_GEN2, EP2_DISP),
    E3(0x04, TEXT_INDEXB, SZ_BYTE, M_GREG, M_GENR, P_REG, P_GEN1, M_GENR,  P_GEN2),
    E3(0x05, TEXT_INDEXW, SZ_WORD, M_GREG, M_GENR, P_REG, P_GEN1, M_GENR,  P_GEN2),
    E3(0x07, TEXT_INDEXD, SZ_QUAD, M_GREG, M_GENR, P_REG, P_GEN1, M_GENR,  P_GEN2),
};
constexpr Entry FORMAT_8_2[] PROGMEM = {
    E3(0x03, TEXT_CVTP, SZ_QUAD, M_GREG, M_GENA, P_REG,  P_GEN1,  M_GENW, P_GEN2),
};
constexpr Entry FORMAT_8_2_1[] PROGMEM = {
    E2(0x04, TEXT_FFSB, SZ_BYTE, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x05, TEXT_FFSW, SZ_WORD, M_GENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x07, TEXT_FFSD, SZ_QUAD, M_GENR, M_GENW, P_GEN1, P_GEN2),
};
constexpr Entry FORMAT_8_3[] PROGMEM = {
    E4(0x00, TEXT_INSB, SZ_BYTE, M_GREG, M_GENR, P_REG, P_GEN1, M_GENW, EM2_LEN32, P_GEN2, EP2_DISP),
    E4(0x01, TEXT_INSW, SZ_WORD, M_GREG, M_GENR, P_REG, P_GEN1, M_GENW, EM2_LEN32, P_GEN2, EP2_DISP),
    E4(0x03, TEXT_INSD, SZ_QUAD, M_GREG, M_GENR, P_REG, P_GEN1, M_GENW, EM2_LEN32, P_GEN2, EP2_DISP),
};
constexpr Entry FORMAT_8_4[] PROGMEM = {
    E3(0x00, TEXT_CHECKB, SZ_BYTE, M_GREG, M_GENA, P_REG, P_GEN1, M_GENR, P_GEN2),
    E3(0x01, TEXT_CHECKW, SZ_WORD, M_GREG, M_GENA, P_REG, P_GEN1, M_GENR, P_GEN2),
    E3(0x03, TEXT_CHECKD, SZ_QUAD, M_GREG, M_GENA, P_REG, P_GEN1, M_GENR, P_GEN2),
};
constexpr uint8_t INDEX_8_1[] PROGMEM = {
      0,  // TEXT_EXTB
      2,  // TEXT_EXTD
      1,  // TEXT_EXTW
      3,  // TEXT_INDEXB
      5,  // TEXT_INDEXD
      4,  // TEXT_INDEXW
};
constexpr uint8_t INDEX_8_2[] PROGMEM = {
      0,  // TEXT_CVTP
};
constexpr uint8_t INDEX_8_2_1[] PROGMEM = {
      0,  // TEXT_FFSB
      2,  // TEXT_FFSD
      1,  // TEXT_FFSW
};
constexpr uint8_t INDEX_8_3[] PROGMEM = {
      0,  // TEXT_INSB
      2,  // TEXT_INSD
      1,  // TEXT_INSW
};
constexpr uint8_t INDEX_8_4[] PROGMEM = {
      0,  // TEXT_CHECKB
      2,  // TEXT_CHECKD
      1,  // TEXT_CHECKW
};

#if !defined(LIBASM_NS32000_NOPMMU)
constexpr Entry FORMAT_8_3_1_PMMU[] PROGMEM = {
    E2(0x0C, TEXT_MOVSUB, SZ_BYTE, M_GENA, M_GENA, P_GEN1, P_GEN2),
    E2(0x0D, TEXT_MOVSUW, SZ_WORD, M_GENA, M_GENA, P_GEN1, P_GEN2),
    E2(0x0F, TEXT_MOVSUD, SZ_QUAD, M_GENA, M_GENA, P_GEN1, P_GEN2),
    E2(0x1C, TEXT_MOVUSB, SZ_BYTE, M_GENA, M_GENA, P_GEN1, P_GEN2),
    E2(0x1D, TEXT_MOVUSW, SZ_WORD, M_GENA, M_GENA, P_GEN1, P_GEN2),
    E2(0x1F, TEXT_MOVUSD, SZ_QUAD, M_GENA, M_GENA, P_GEN1, P_GEN2),
};
constexpr uint8_t INDEX_8_3_1_PMMU[] PROGMEM = {
      0,  // TEXT_MOVSUB
      2,  // TEXT_MOVSUD
      1,  // TEXT_MOVSUW
      3,  // TEXT_MOVUSB
      5,  // TEXT_MOVUSD
      4,  // TEXT_MOVUSW
};
#endif

#if !defined(LIBASM_NS32000_NOFPU)

// Format 9: |gen1_|gen| |2_|_op|f|ii| |0011|1110|
constexpr Entry FORMAT_9_FPU[] PROGMEM = {
    // gen2 must be register pair
    W2(0x00, TEXT_MOVBL,   SZ_BYTE, M_GENR, M_FENW, P_GEN1, P_GEN2, false, true),
    W2(0x01, TEXT_MOVWL,   SZ_WORD, M_GENR, M_FENW, P_GEN1, P_GEN2, false, true),
    W2(0x03, TEXT_MOVDL,   SZ_QUAD, M_GENR, M_FENW, P_GEN1, P_GEN2, false, true),
    E2(0x04, TEXT_MOVBF,   SZ_BYTE, M_GENR, M_FENW, P_GEN1, P_GEN2),
    E2(0x05, TEXT_MOVWF,   SZ_WORD, M_GENR, M_FENW, P_GEN1, P_GEN2),
    E2(0x07, TEXT_MOVDF,   SZ_QUAD, M_GENR, M_FENW, P_GEN1, P_GEN2),
    E2(0x0F, TEXT_LFSR,    SZ_QUAD, M_GENR, M_ZERO, P_GEN1, P_GEN2),
    E2(0x37, TEXT_SFSR,    SZ_QUAD, M_GENW, M_ZERO, P_GEN2, P_GEN1),
    // gen1 must be register pair
    W2(0x16, TEXT_MOVLF,   SZ_QUAD, M_FENR, M_FENW, P_GEN1, P_GEN2, true, false),
    // gen2 must be register pair
    W2(0x1B, TEXT_MOVFL,   SZ_QUAD, M_FENR, M_FENW, P_GEN1, P_GEN2, false, true),
    E2(0x20, TEXT_ROUNDLB, SZ_OCTA, M_FENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x21, TEXT_ROUNDLW, SZ_OCTA, M_FENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x23, TEXT_ROUNDLD, SZ_OCTA, M_FENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x24, TEXT_ROUNDFB, SZ_QUAD, M_FENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x25, TEXT_ROUNDFW, SZ_QUAD, M_FENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x27, TEXT_ROUNDFD, SZ_QUAD, M_FENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x28, TEXT_TRUNCLB, SZ_OCTA, M_FENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x29, TEXT_TRUNCLW, SZ_OCTA, M_FENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x2B, TEXT_TRUNCLD, SZ_OCTA, M_FENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x2C, TEXT_TRUNCFB, SZ_QUAD, M_FENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x2D, TEXT_TRUNCFW, SZ_QUAD, M_FENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x2F, TEXT_TRUNCFD, SZ_QUAD, M_FENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x38, TEXT_FLOORLB, SZ_OCTA, M_FENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x39, TEXT_FLOORLW, SZ_OCTA, M_FENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x3B, TEXT_FLOORLD, SZ_OCTA, M_FENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x3C, TEXT_FLOORFB, SZ_QUAD, M_FENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x3D, TEXT_FLOORFW, SZ_QUAD, M_FENR, M_GENW, P_GEN1, P_GEN2),
    E2(0x3F, TEXT_FLOORFD, SZ_QUAD, M_FENR, M_GENW, P_GEN1, P_GEN2),
};
constexpr uint8_t INDEX_9_FPU[] PROGMEM = {
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
constexpr Entry FORMAT_11_FPU[] PROGMEM = {
    E2(0x00, TEXT_ADDL, SZ_OCTA, M_FENR, M_FENW, P_GEN1, P_GEN2),
    E2(0x01, TEXT_ADDF, SZ_QUAD, M_FENR, M_FENW, P_GEN1, P_GEN2),
    E2(0x04, TEXT_MOVL, SZ_OCTA, M_FENR, M_FENW, P_GEN1, P_GEN2),
    E2(0x05, TEXT_MOVF, SZ_QUAD, M_FENR, M_FENW, P_GEN1, P_GEN2),
    E2(0x08, TEXT_CMPL, SZ_OCTA, M_FENR, M_FENR, P_GEN1, P_GEN2),
    E2(0x09, TEXT_CMPF, SZ_QUAD, M_FENR, M_FENR, P_GEN1, P_GEN2),
    E2(0x10, TEXT_SUBL, SZ_OCTA, M_FENR, M_FENW, P_GEN1, P_GEN2),
    E2(0x11, TEXT_SUBF, SZ_QUAD, M_FENR, M_FENW, P_GEN1, P_GEN2),
    E2(0x14, TEXT_NEGL, SZ_OCTA, M_FENR, M_FENW, P_GEN1, P_GEN2),
    E2(0x15, TEXT_NEGF, SZ_QUAD, M_FENR, M_FENW, P_GEN1, P_GEN2),
    E2(0x20, TEXT_DIVL, SZ_OCTA, M_FENR, M_FENW, P_GEN1, P_GEN2),
    E2(0x21, TEXT_DIVF, SZ_QUAD, M_FENR, M_FENW, P_GEN1, P_GEN2),
    E2(0x30, TEXT_MULL, SZ_OCTA, M_FENR, M_FENW, P_GEN1, P_GEN2),
    E2(0x31, TEXT_MULF, SZ_QUAD, M_FENR, M_FENW, P_GEN1, P_GEN2),
    E2(0x34, TEXT_ABSL, SZ_OCTA, M_FENR, M_FENW, P_GEN1, P_GEN2),
    E2(0x35, TEXT_ABSF, SZ_QUAD, M_FENR, M_FENW, P_GEN1, P_GEN2),
};
constexpr uint8_t INDEX_11_FPU[] PROGMEM = {
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

#endif

#if !defined(LIBASM_NS32000_NOPMMU)
// Format 14: |gen1_|sho| |t|0|_op_|ii| |0001|1110|
constexpr Entry FORMAT_14_1_PMMU[] PROGMEM = {
    E2(0x03, TEXT_RDVAL, SZ_QUAD, M_GENA, M_ZERO, P_GEN1, P_GEN2),
    E2(0x07, TEXT_WRVAL, SZ_QUAD, M_GENA, M_ZERO, P_GEN1, P_GEN2),
};
constexpr Entry FORMAT_14_2_PMMU[] PROGMEM = {
    E2(0x0B, TEXT_LMR, SZ_QUAD, M_MREG, M_GENR, P_SHORT, P_GEN1),
    E2(0x0F, TEXT_SMR, SZ_QUAD, M_MREG, M_GENW, P_SHORT, P_GEN1),
};
constexpr uint8_t INDEX_14_1_PMMU[] PROGMEM = {
      0,  // TEXT_RDVAL
      1,  // TEXT_WRVAL
};
constexpr uint8_t INDEX_14_2_PMMU[] PROGMEM = {
      0,  // TEXT_LMR
      1,  // TEXT_SMR
};
#endif
// clang-format on

struct EntryPage : entry::PrefixTableBase<Entry> {
    Config::opcode_t readMask() const { return pgm_read_byte(&_mask_P); }
    Config::opcode_t readPost() const { return pgm_read_byte(&_post_P); }

    constexpr EntryPage(Config::opcode_t prefix, Config::opcode_t mask, uint8_t post,
            const Entry *head_P, const Entry *tail_P, const uint8_t *index_P,
            const uint8_t *itail_P)
        : PrefixTableBase(prefix, head_P, tail_P, index_P, itail_P), _mask_P(mask), _post_P(post) {}

private:
    const Config::opcode_t _mask_P;
    const uint8_t _post_P;
};

// Standard Instructions
constexpr EntryPage NS32032_PAGES[] PROGMEM = {
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
        {0x6E, 0xC0, 1, ARRAY_RANGE(FORMAT_8_2_1), ARRAY_RANGE(INDEX_8_2_1)},
        {0xAE, 0xF8, 1, ARRAY_RANGE(FORMAT_8_3), ARRAY_RANGE(INDEX_8_3)},
        {0xEE, 0xF8, 1, ARRAY_RANGE(FORMAT_8_4), ARRAY_RANGE(INDEX_8_4)},
};

#if !defined(LIBASM_NS32000_NOFPU)
// Floating point instructions
constexpr EntryPage NS32081_PAGES[] PROGMEM = {
        {0x3E, 0xC0, 1, ARRAY_RANGE(FORMAT_9_FPU), ARRAY_RANGE(INDEX_9_FPU)},
        {0xBE, 0xC0, 1, ARRAY_RANGE(FORMAT_11_FPU), ARRAY_RANGE(INDEX_11_FPU)},
};
#endif

#if !defined(LIBASM_NS32000_NOPMMU)
// Memory management instructions
constexpr EntryPage NS32082_PAGES[] PROGMEM = {
        {0xAE, 0xC0, 1, ARRAY_RANGE(FORMAT_8_3_1_PMMU), ARRAY_RANGE(INDEX_8_3_1_PMMU)},
        {0x1E, 0x00, 1, ARRAY_RANGE(FORMAT_14_1_PMMU), ARRAY_RANGE(INDEX_14_1_PMMU)},
        {0x1E, 0x80, 1, ARRAY_RANGE(FORMAT_14_2_PMMU), ARRAY_RANGE(INDEX_14_2_PMMU)},
};
#endif

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {NS32032, TEXT_CPU_32032, ARRAY_RANGE(NS32032_PAGES)},
};

const Cpu *cpu(CpuType) {
    return &CPU_TABLE[0];
}

#define EMPTY_RANGE(a) ARRAY_BEGIN(a), ARRAY_BEGIN(a)

#if !defined(LIBASM_NS32000_NOFPU)
using Fpu = entry::CpuBase<FpuType, EntryPage>;

constexpr Fpu FPU_TABLE[] PROGMEM = {
        {FPU_NS32081, TEXT_FPU_NS32081, ARRAY_RANGE(NS32081_PAGES)},
        {FPU_NONE, TEXT_none, EMPTY_RANGE(NS32081_PAGES)},
};

const Fpu *fpu(FpuType fpuType) {
    return Fpu::search(fpuType, ARRAY_RANGE(FPU_TABLE));
}
#endif

#if !defined(LIBASM_NS32000_NOPMMU)
using Pmmu = entry::CpuBase<PmmuType, EntryPage>;

constexpr Pmmu PMMU_TABLE[] PROGMEM = {
        {PMMU_NS32082, TEXT_PMMU_NS32082, ARRAY_RANGE(NS32082_PAGES)},
        {PMMU_NONE, TEXT_none, EMPTY_RANGE(NS32082_PAGES)},
};

const Pmmu *pmmu(PmmuType pmmuType) {
    return Pmmu::search(pmmuType, ARRAY_RANGE(PMMU_TABLE));
}
#endif

bool acceptMode(AddrMode opr, AddrMode table) {
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
               table == M_LEN32 || table == M_LEN16;
    if (opr == M_FREG)
        return table == M_FENR || table == M_FENW;
    if (opr == M_NONE)
        return table == M_RLST || table == M_CONF || table == M_SOPT || table == M_ZERO;
    return false;
}

bool acceptModes(AsmInsn &insn, const Entry *entry) {
    const auto table = entry->readFlags();
    return acceptMode(insn.srcOp.mode, table.src()) && acceptMode(insn.dstOp.mode, table.dst()) &&
           acceptMode(insn.ex1Op.mode, table.ex1()) && acceptMode(insn.ex2Op.mode, table.ex2());
}

void pageSetup(AsmInsn &insn, const EntryPage *page) {
    insn.setPostfix(0, page->readPost() != 0);
}

Error searchName(const CpuSpec &cpuSpec, AsmInsn &insn) {
    cpu(cpuSpec.cpu)->searchName(insn, acceptModes, pageSetup);
#if !defined(LIBASM_NS32000_NOFPU)
    if (insn.getError() == UNKNOWN_INSTRUCTION)
        fpu(cpuSpec.fpu)->searchName(insn, acceptModes, pageSetup);
#endif
#if !defined(LIBASM_NS32000_NOPMMU)
    if (insn.getError() == UNKNOWN_INSTRUCTION)
        pmmu(cpuSpec.pmmu)->searchName(insn, acceptModes, pageSetup);
#endif
    return insn.getError();
}

bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *page) {
    return (insn.opCode() & ~page->readMask()) == entry->readOpCode();
}

void readEntryName(DisInsn &insn, const Entry *entry, StrBuffer &out, const EntryPage *page) {
    Cpu::defaultReadName(insn, entry, out, page);
    insn.setPostfix(0, page->readPost() != 0);
}

template <typename CPU>
Error searchCode(const CPU *cpu, DisInsn &insn, StrBuffer &out) {
    const auto entry =
            cpu->searchOpCode(insn, out, matchOpCode, Cpu::defaultPageMatcher, readEntryName);
    if (entry && insn.hasPostfix())
        insn.setPostfix(insn.readByte());
    return insn.getError();
}

Error searchOpCode(const CpuSpec &cpuSpec, DisInsn &insn, StrBuffer &out) {
    searchCode(cpu(cpuSpec.cpu), insn, out);
#if !defined(LIBASM_NS32000_NOFPU)
    if (insn.getError() == UNKNOWN_INSTRUCTION)
        searchCode(fpu(cpuSpec.fpu), insn, out);
#endif
#if !defined(LIBASM_NS32000_NOPMMU)
    if (insn.getError() == UNKNOWN_INSTRUCTION)
        searchCode(pmmu(cpuSpec.pmmu), insn, out);
#endif
    return insn.getError();
}

bool isPrefixCode(const CpuSpec &cpuSpec, uint8_t code) {
    return
#if !defined(LIBASM_NS32000_NOFPU)
            fpu(cpuSpec.fpu)->isPrefix(code) ||
#endif
#if !defined(LIBASM_NS32000_NOPMMU)
            pmmu(cpuSpec.pmmu)->isPrefix(code) ||
#endif
            cpu(cpuSpec.cpu)->isPrefix(code);
}

const /*PROGMEM*/ char *TableNs32000::listCpu_P() const {
    return TEXT_CPU_NS32032;
}

const /*PROGMEM*/ char *TableNs32000::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableNs32000::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    if (name.iequals(TEXT_CPU_NS32032) || name.iequals(TEXT_CPU_32032)) {
        cpuType = NS32032;
        return OK;
    }
    return UNSUPPORTED_CPU;
}

const /*PROGMEM*/ char *Config::fpu_P() const {
    return fpu(_cpuSpec.fpu)->name_P();
}

Error Config::setFpuType(FpuType fpuType) {
    if (fpuType == FPU_NONE) {
        _cpuSpec.fpu = FPU_NONE;
        return OK;
    }
#if !defined(LIBASM_NS32000_NOFPU)
    if (fpuType == FPU_ON || fpuType == FPU_NS32081) {
        _cpuSpec.fpu = FPU_NS32081;
        return OK;
    }
#endif
    return UNKNOWN_OPERAND;
}

Error Config::setFpuName(StrScanner &scan) {
    if (scan.expectFalse())
        return setFpuType(FPU_NONE);
#if !defined(LIBASM_NS32000_NOFPU)
    if (scan.expectTrue())
        return setFpuType(FPU_ON);
    if (scan.iequals_P(TEXT_FPU_NS32081))
        return setFpuType(FPU_NS32081);
#endif
    return UNKNOWN_OPERAND;
}

const /*PROGMEM*/ char *Config::pmmu_P() const {
    return pmmu(_cpuSpec.pmmu)->name_P();
}

Error Config::setPmmuType(PmmuType pmmuType) {
    if (pmmuType == PMMU_NONE) {
        _cpuSpec.pmmu = PMMU_NONE;
        return OK;
    }
#if !defined(LIBASM_NS32000_NOPMMU)
    if (pmmuType == PMMU_ON || pmmuType == PMMU_NS32082) {
        _cpuSpec.pmmu = PMMU_NS32082;
        return OK;
    }
#endif
    return UNKNOWN_OPERAND;
}

Error Config::setPmmuName(StrScanner &scan) {
    if (scan.expectFalse())
        return setPmmuType(PMMU_NONE);
#if !defined(LIBASM_NS32000_NOPMMU)
    if (scan.expectTrue())
        return setPmmuType(PMMU_ON);
    if (scan.iequals_P(TEXT_PMMU_NS32082))
        return setPmmuType(PMMU_NS32082);
#endif
    return UNKNOWN_OPERAND;
}

const TableNs32000 TABLE;

}  // namespace ns32000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

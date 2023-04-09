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

#include "table_mc68000.h"

#include <string.h>

#include "config_mc68000.h"
#include "entry_mc68000.h"
#include "text_mc68000.h"

using namespace libasm::text::mc68000;

namespace libasm {
namespace mc68000 {

#define E(_opc, _name, _isize, _src, _dst, _srcp, _dstp, _osize, _with, _alias)       \
    {                                                                                 \
        _opc,                                                                         \
                Entry::Flags::create(_src, _dst, OP_##_srcp, OP_##_dstp, SZ_##_osize, \
                        ISZ_##_isize, _with, _alias),                                 \
                _name                                                                 \
    }
#define E2(_opc, _name, _isize, _src, _dst, _srcp, _dstp, _osize) \
    E(_opc, _name, _isize, _src, _dst, _srcp, _dstp, _osize, false, false)
#define E1(_opc, _name, _isize, _src, _srcp, _osize) \
    E2(_opc, _name, _isize, _src, M_NONE, _srcp, __, _osize)
#define E0(_opc, _name) E1(_opc, _name, NONE, M_NONE, __, NONE)
#define W2(_opc, _name, _isize, _src, _dst, _srcp, _dstp, _osize) \
    E(_opc, _name, _isize, _src, _dst, _srcp, _dstp, _osize, true, false)
#define W1(_opc, _name, _isize, _src, _srcp, _osize) \
    W2(_opc, _name, _isize, _src, M_NONE, _srcp, __, _osize)
#define A2(_opc, _name, _isize, _src, _dst, _srcp, _dstp, _osize) \
    E(_opc, _name, _isize, _src, _dst, _srcp, _dstp, _osize, false, true)

// clang-format off
static constexpr Entry MC68000_TABLE[] PROGMEM = {
    E2(0000074, TEXT_ORI,   NONE, M_IMDAT, M_CCR,   __, __, BYTE),
    E2(0000174, TEXT_ORI,   NONE, M_IMDAT, M_SR,    __, __, WORD),
    E2(0000000, TEXT_ORI,   DATA, M_IMDAT, M_WDATA, __, 10, DATA),
    E2(0001074, TEXT_ANDI,  NONE, M_IMDAT, M_CCR,   __, __, BYTE),
    E2(0001174, TEXT_ANDI,  NONE, M_IMDAT, M_SR,    __, __, WORD),
    E2(0001000, TEXT_ANDI,  DATA, M_IMDAT, M_WDATA, __, 10, DATA),
    E2(0002000, TEXT_SUBI,  DATA, M_IMDAT, M_WDATA, __, 10, DATA),
    E2(0003000, TEXT_ADDI,  DATA, M_IMDAT, M_WDATA, __, 10, DATA),
    E2(0005074, TEXT_EORI,  NONE, M_IMDAT, M_CCR,   __, __, BYTE),
    E2(0005174, TEXT_EORI,  NONE, M_IMDAT, M_SR,    __, __, WORD),
    E2(0005000, TEXT_EORI,  DATA, M_IMDAT, M_WDATA, __, 10, DATA),
    E2(0006000, TEXT_CMPI,  DATA, M_IMDAT, M_WDATA, __, 10, DATA),
    W2(0004000, TEXT_BTST,  NONE, M_IMBIT, M_DREG,  __, _0, LONG),
    W2(0004000, TEXT_BTST,  NONE, M_IMBIT, M_RMEM,  __, 10, BYTE),
    W2(0004100, TEXT_BCHG,  NONE, M_IMBIT, M_DREG,  __, _0, LONG),
    W2(0004100, TEXT_BCHG,  NONE, M_IMBIT, M_WMEM,  __, 10, BYTE),
    W2(0004200, TEXT_BCLR,  NONE, M_IMBIT, M_DREG,  __, _0, LONG),
    W2(0004200, TEXT_BCLR,  NONE, M_IMBIT, M_WMEM,  __, 10, BYTE),
    W2(0004300, TEXT_BSET,  NONE, M_IMBIT, M_DREG,  __, _0, LONG),
    W2(0004300, TEXT_BSET,  NONE, M_IMBIT, M_WMEM,  __, 10, BYTE),
    E2(0000410, TEXT_MOVEP, DATA, M_DISP,  M_DREG,  _0, _3, ADR6),
    E2(0000610, TEXT_MOVEP, DATA, M_DREG,  M_DISP,  _3, _0, ADR6),
    W2(0000400, TEXT_BTST,  NONE, M_DREG,  M_DREG,  _3, _0, LONG),
    W2(0000400, TEXT_BTST,  NONE, M_DREG,  M_RMEM,  _3, 10, BYTE),
    W2(0000500, TEXT_BCHG,  NONE, M_DREG,  M_DREG,  _3, _0, LONG),
    W2(0000500, TEXT_BCHG,  NONE, M_DREG,  M_WMEM,  _3, 10, BYTE),
    W2(0000600, TEXT_BCLR,  NONE, M_DREG,  M_DREG,  _3, _0, LONG),
    W2(0000600, TEXT_BCLR,  NONE, M_DREG,  M_WMEM,  _3, 10, BYTE),
    W2(0000700, TEXT_BSET,  NONE, M_DREG,  M_DREG,  _3, _0, LONG),
    W2(0000700, TEXT_BSET,  NONE, M_DREG,  M_WMEM,  _3, 10, BYTE),
    E2(0020100, TEXT_MOVEA, LONG, M_RADDR, M_AREG,  10, _3, LONG),
    E2(0030100, TEXT_MOVEA, WORD, M_RADDR, M_AREG,  10, _3, WORD),
    E2(0030100, TEXT_MOVEA, NONE, M_RADDR, M_AREG,  10, _3, WORD),
    W2(0042300, TEXT_MOVE,  NONE, M_RDATA, M_CCR,   10, __, WORD),
    W2(0040300, TEXT_MOVE,  NONE, M_SR,    M_WDATA, __, 10, WORD),
    W2(0043300, TEXT_MOVE,  NONE, M_RDATA, M_SR,    10, __, WORD),
    E2(0010000, TEXT_MOVE,  BYTE, M_RDATA, M_WDATA, 10, 23, BYTE),
    E2(0020000, TEXT_MOVE,  LONG, M_RADDR, M_WDATA, 10, 23, LONG),
    E2(0030000, TEXT_MOVE,  WORD, M_RADDR, M_WDATA, 10, 23, WORD),
    E1(0040000, TEXT_NEGX,  DATA, M_WDATA,  10, DATA),
    E1(0041000, TEXT_CLR,   DATA, M_WDATA,  10, DATA),
    E1(0042000, TEXT_NEG,   DATA, M_WDATA,  10, DATA),
    E1(0043000, TEXT_NOT,   DATA, M_WDATA,  10, DATA),
    E1(0044200, TEXT_EXT,   DATA, M_DREG,  _0, ADR6),
    E1(0044000, TEXT_NBCD,  NONE, M_WDATA,  10, BYTE),
    E1(0044100, TEXT_SWAP,  NONE, M_DREG,  _0, LONG),
    E1(0044100, TEXT_PEA,   NONE, M_JADDR,  10, LONG),
    E0(0045374, TEXT_ILLEG),
    E1(0045300, TEXT_TAS,   NONE, M_WDATA,  10, BYTE),
    E1(0045000, TEXT_TST,   DATA, M_WDATA,  10, DATA),
    E1(0047100, TEXT_TRAP,  NONE, M_IMVEC,  __, NONE),
    E2(0047120, TEXT_LINK,  NONE, M_AREG,  M_IMDSP, _0, __, WORD),
    E1(0047130, TEXT_UNLK,  NONE, M_AREG,  _0, NONE),
    E2(0047140, TEXT_MOVE,  NONE, M_AREG,  M_USP,   _0, __, LONG),
    E2(0047150, TEXT_MOVE,  NONE, M_USP,   M_AREG,  __, _0, LONG),
    E0(0047160, TEXT_RESET),
    E0(0047161, TEXT_NOP),
    E1(0047162, TEXT_STOP,  NONE, M_IMDAT,  __, WORD),
    E0(0047163, TEXT_RTE),
    E0(0047165, TEXT_RTS),
    E0(0047166, TEXT_TRAPV),
    E0(0047167, TEXT_RTR),
    E1(0047200, TEXT_JSR,   NONE, M_JADDR,  10, WORD),
    E1(0047300, TEXT_JMP,   NONE, M_JADDR,  10, WORD),
    E2(0044200, TEXT_MOVEM, DATA, M_MULT,  M_DADDR, __, 10, ADR6),
    E2(0046200, TEXT_MOVEM, DATA, M_IADDR, M_MULT,  10, __, ADR6),
    E2(0040700, TEXT_LEA,   NONE, M_JADDR, M_AREG,  10, _3, LONG),
    E2(0040600, TEXT_CHK,   WORD, M_RDATA, M_DREG,  10, _3, WORD),
    E2(0050310, TEXT_DBT,   NONE, M_DREG,  M_REL16, _0, __, WORD),
    E2(0050710, TEXT_DBRA,  NONE, M_DREG,  M_REL16, _0, __, WORD),
    E2(0050710, TEXT_DBF,   NONE, M_DREG,  M_REL16, _0, __, WORD),
    E2(0051310, TEXT_DBHI,  NONE, M_DREG,  M_REL16, _0, __, WORD),
    E2(0051710, TEXT_DBLS,  NONE, M_DREG,  M_REL16, _0, __, WORD),
    E2(0052310, TEXT_DBCC,  NONE, M_DREG,  M_REL16, _0, __, WORD),
    E2(0052310, TEXT_DBHS,  NONE, M_DREG,  M_REL16, _0, __, WORD),
    E2(0052710, TEXT_DBCS,  NONE, M_DREG,  M_REL16, _0, __, WORD),
    E2(0052710, TEXT_DBLO,  NONE, M_DREG,  M_REL16, _0, __, WORD),
    E2(0053310, TEXT_DBNE,  NONE, M_DREG,  M_REL16, _0, __, WORD),
    E2(0053710, TEXT_DBEQ,  NONE, M_DREG,  M_REL16, _0, __, WORD),
    E2(0054310, TEXT_DBVC,  NONE, M_DREG,  M_REL16, _0, __, WORD),
    E2(0054710, TEXT_DBVS,  NONE, M_DREG,  M_REL16, _0, __, WORD),
    E2(0055310, TEXT_DBPL,  NONE, M_DREG,  M_REL16, _0, __, WORD),
    E2(0055710, TEXT_DBMI,  NONE, M_DREG,  M_REL16, _0, __, WORD),
    E2(0056310, TEXT_DBGE,  NONE, M_DREG,  M_REL16, _0, __, WORD),
    E2(0056710, TEXT_DBLT,  NONE, M_DREG,  M_REL16, _0, __, WORD),
    E2(0057310, TEXT_DBGT,  NONE, M_DREG,  M_REL16, _0, __, WORD),
    E2(0057710, TEXT_DBLE,  NONE, M_DREG,  M_REL16, _0, __, WORD),
    E1(0050300, TEXT_ST,    NONE, M_WDATA,  10, BYTE),
    E1(0050700, TEXT_SF,    NONE, M_WDATA,  10, BYTE),
    E1(0051300, TEXT_SHI,   NONE, M_WDATA,  10, BYTE),
    E1(0051700, TEXT_SLS,   NONE, M_WDATA,  10, BYTE),
    E1(0052300, TEXT_SCC,   NONE, M_WDATA,  10, BYTE),
    E1(0052300, TEXT_SHS,   NONE, M_WDATA,  10, BYTE),
    E1(0052700, TEXT_SCS,   NONE, M_WDATA,  10, BYTE),
    E1(0052700, TEXT_SLO,   NONE, M_WDATA,  10, BYTE),
    E1(0053300, TEXT_SNE,   NONE, M_WDATA,  10, BYTE),
    E1(0053700, TEXT_SEQ,   NONE, M_WDATA,  10, BYTE),
    E1(0054300, TEXT_SVC,   NONE, M_WDATA,  10, BYTE),
    E1(0054700, TEXT_SVS,   NONE, M_WDATA,  10, BYTE),
    E1(0055300, TEXT_SPL,   NONE, M_WDATA,  10, BYTE),
    E1(0055700, TEXT_SMI,   NONE, M_WDATA,  10, BYTE),
    E1(0056300, TEXT_SGE,   NONE, M_WDATA,  10, BYTE),
    E1(0056700, TEXT_SLT,   NONE, M_WDATA,  10, BYTE),
    E1(0057300, TEXT_SGT,   NONE, M_WDATA,  10, BYTE),
    E1(0057700, TEXT_SLE,   NONE, M_WDATA,  10, BYTE),
    E2(0050000, TEXT_ADDQ,  DATA, M_IM3,   M_WADDR, __, 10, DATA),
    E2(0050400, TEXT_SUBQ,  DATA, M_IM3,   M_WADDR, __, 10, DATA),
    E1(0060000, TEXT_BRA,   NONE, M_REL8,  __, BYTE),
    E1(0060000, TEXT_BRA,   NONE, M_REL16,  __, WORD),
    E1(0060000, TEXT_BT,    NONE, M_REL8,  __, BYTE),
    E1(0060000, TEXT_BT,    NONE, M_REL16,  __, WORD),
    E1(0060400, TEXT_BSR,   NONE, M_REL8,  __, BYTE),
    E1(0060400, TEXT_BSR,   NONE, M_REL16,  __, WORD),
    E1(0061000, TEXT_BHI,   NONE, M_REL8,  __, BYTE),
    E1(0061000, TEXT_BHI,   NONE, M_REL16,  __, WORD),
    E1(0061400, TEXT_BLS,   NONE, M_REL8,  __, BYTE),
    E1(0061400, TEXT_BLS,   NONE, M_REL16,  __, WORD),
    E1(0062000, TEXT_BCC,   NONE, M_REL8,  __, BYTE),
    E1(0062000, TEXT_BCC,   NONE, M_REL16,  __, WORD),
    E1(0062000, TEXT_BHS,   NONE, M_REL8,  __, BYTE),
    E1(0062000, TEXT_BHS,   NONE, M_REL16,  __, WORD),
    E1(0062400, TEXT_BCS,   NONE, M_REL8,  __, BYTE),
    E1(0062400, TEXT_BCS,   NONE, M_REL16,  __, WORD),
    E1(0062400, TEXT_BLO,   NONE, M_REL8,  __, BYTE),
    E1(0062400, TEXT_BLO,   NONE, M_REL16,  __, WORD),
    E1(0063000, TEXT_BNE,   NONE, M_REL8,  __, BYTE),
    E1(0063000, TEXT_BNE,   NONE, M_REL16,  __, WORD),
    E1(0063400, TEXT_BEQ,   NONE, M_REL8,  __, BYTE),
    E1(0063400, TEXT_BEQ,   NONE, M_REL16,  __, WORD),
    E1(0064000, TEXT_BVC,   NONE, M_REL8,  __, BYTE),
    E1(0064000, TEXT_BVC,   NONE, M_REL16,  __, WORD),
    E1(0064400, TEXT_BVS,   NONE, M_REL8,  __, BYTE),
    E1(0064400, TEXT_BVS,   NONE, M_REL16,  __, WORD),
    E1(0065000, TEXT_BPL,   NONE, M_REL8,  __, BYTE),
    E1(0065000, TEXT_BPL,   NONE, M_REL16,  __, WORD),
    E1(0065400, TEXT_BMI,   NONE, M_REL8,  __, BYTE),
    E1(0065400, TEXT_BMI,   NONE, M_REL16,  __, WORD),
    E1(0066000, TEXT_BGE,   NONE, M_REL8,  __, BYTE),
    E1(0066000, TEXT_BGE,   NONE, M_REL16,  __, WORD),
    E1(0066400, TEXT_BLT,   NONE, M_REL8,  __, BYTE),
    E1(0066400, TEXT_BLT,   NONE, M_REL16,  __, WORD),
    E1(0067000, TEXT_BGT,   NONE, M_REL8,  __, BYTE),
    E1(0067000, TEXT_BGT,   NONE, M_REL16,  __, WORD),
    E1(0067400, TEXT_BLE,   NONE, M_REL8,  __, BYTE),
    E1(0067400, TEXT_BLE,   NONE, M_REL16,  __, WORD),
    E2(0070000, TEXT_MOVEQ, NONE, M_IM8,   M_DREG,  __, _3, LONG),
    E2(0100300, TEXT_DIVU,  DATA, M_RDATA, M_DREG,  10, _3, WORD),
    E2(0100700, TEXT_DIVS,  DATA, M_RDATA, M_DREG,  10, _3, WORD),
    E2(0100000, TEXT_OR,    DATA, M_RDATA, M_DREG,  10, _3, DATA),
    E2(0100400, TEXT_SBCD,  NONE, M_DREG,  M_DREG,  _0, _3, BYTE),
    E2(0100410, TEXT_SBCD,  NONE, M_PDEC,  M_PDEC,  _0, _3, BYTE),
    E2(0100400, TEXT_OR,    DATA, M_DREG,  M_WMEM,  _3, 10, DATA),
    E2(0110300, TEXT_SUBA,  DATA, M_RADDR, M_AREG,  10, _3, ADR8),
    E2(0110400, TEXT_SUBX,  DATA, M_DREG,  M_DREG,  _0, _3, DATA),
    E2(0110410, TEXT_SUBX,  DATA, M_PDEC,  M_PDEC,  _0, _3, DATA),
    E2(0110000, TEXT_SUB,   DATA, M_RADDR, M_DREG,  10, _3, DATA),
    E2(0110400, TEXT_SUB,   DATA, M_DREG,  M_WMEM,  _3, 10, DATA),
    E2(0130300, TEXT_CMPA,  DATA, M_RADDR, M_AREG,  10, _3, ADR8),
    E2(0130000, TEXT_CMP,   DATA, M_RADDR, M_DREG,  10, _3, DATA),
    E2(0130410, TEXT_CMPM,  DATA, M_PINC,  M_PINC,  _0, _3, DATA),
    E2(0130400, TEXT_EOR,   DATA, M_DREG,  M_WDATA, _3, 10, DATA),
    E2(0140300, TEXT_MULU,  DATA, M_RDATA, M_DREG,  10, _3, WORD),
    E2(0140700, TEXT_MULS,  DATA, M_RDATA, M_DREG,  10, _3, WORD),
    E2(0140000, TEXT_AND,   DATA, M_RDATA, M_DREG,  10, _3, DATA),
    E2(0140400, TEXT_ABCD,  NONE, M_DREG,  M_DREG,  _0, _3, BYTE),
    E2(0140410, TEXT_ABCD,  NONE, M_PDEC,  M_PDEC,  _0, _3, BYTE),
    E2(0140500, TEXT_EXG,   NONE, M_DREG,  M_DREG,  _3, _0, LONG),
    E2(0140510, TEXT_EXG,   NONE, M_AREG,  M_AREG,  _3, _0, LONG),
    E2(0140610, TEXT_EXG,   NONE, M_DREG,  M_AREG,  _3, _0, LONG),
    E2(0140610, TEXT_EXG,   NONE, M_AREG,  M_DREG,  _0, _3, LONG),
    E2(0140400, TEXT_AND,   DATA, M_DREG,  M_WMEM,  _3, 10, DATA),
    E2(0150300, TEXT_ADDA,  DATA, M_RADDR, M_AREG,  10, _3, ADR8),
    E2(0150400, TEXT_ADDX,  DATA, M_DREG,  M_DREG,  _0, _3, DATA),
    E2(0150410, TEXT_ADDX,  DATA, M_PDEC,  M_PDEC,  _0, _3, DATA),
    E2(0150000, TEXT_ADD,   DATA, M_RADDR, M_DREG,  10, _3, DATA),
    E2(0150400, TEXT_ADD,   DATA, M_DREG,  M_WMEM,  _3, 10, DATA),
    W1(0160300, TEXT_ASR,   NONE, M_WMEM,  10, WORD),
    W1(0160700, TEXT_ASL,   NONE, M_WMEM,  10, WORD),
    W1(0161300, TEXT_LSR,   NONE, M_WMEM,  10, WORD),
    W1(0161700, TEXT_LSL,   NONE, M_WMEM,  10, WORD),
    W1(0162300, TEXT_ROXR,  NONE, M_WMEM,  10, WORD),
    W1(0162700, TEXT_ROXL,  NONE, M_WMEM,  10, WORD),
    W1(0163300, TEXT_ROR,   NONE, M_WMEM,  10, WORD),
    W1(0163700, TEXT_ROL,   NONE, M_WMEM,  10, WORD),
    E2(0160000, TEXT_ASR,   DATA, M_IM3,   M_DREG,  __, _0, DATA),
    E2(0160040, TEXT_ASR,   DATA, M_DREG,  M_DREG,  _3, _0, DATA),
    E2(0160400, TEXT_ASL,   DATA, M_IM3,   M_DREG,  __, _0, DATA),
    E2(0160440, TEXT_ASL,   DATA, M_DREG,  M_DREG,  _3, _0, DATA),
    E2(0160010, TEXT_LSR,   DATA, M_IM3,   M_DREG,  __, _0, DATA),
    E2(0160050, TEXT_LSR,   DATA, M_DREG,  M_DREG,  _3, _0, DATA),
    E2(0160410, TEXT_LSL,   DATA, M_IM3,   M_DREG,  __, _0, DATA),
    E2(0160450, TEXT_LSL,   DATA, M_DREG,  M_DREG,  _3, _0, DATA),
    E2(0160020, TEXT_ROXR,  DATA, M_IM3,   M_DREG,  __, _0, DATA),
    E2(0160060, TEXT_ROXR,  DATA, M_DREG,  M_DREG,  _3, _0, DATA),
    E2(0160420, TEXT_ROXL,  DATA, M_IM3,   M_DREG,  __, _0, DATA),
    E2(0160460, TEXT_ROXL,  DATA, M_DREG,  M_DREG,  _3, _0, DATA),
    E2(0160030, TEXT_ROR,   DATA, M_IM3,   M_DREG,  __, _0, DATA),
    E2(0160070, TEXT_ROR,   DATA, M_DREG,  M_DREG,  _3, _0, DATA),
    E2(0160430, TEXT_ROL,   DATA, M_IM3,   M_DREG,  __, _0, DATA),
    E2(0160470, TEXT_ROL,   DATA, M_DREG,  M_DREG,  _3, _0, DATA),
};

static constexpr uint8_t MC68000_INDEX[] PROGMEM = {
    164,  // TEXT_ABCD
    165,  // TEXT_ABCD
    174,  // TEXT_ADD
    175,  // TEXT_ADD
    171,  // TEXT_ADDA
      7,  // TEXT_ADDI
    105,  // TEXT_ADDQ
    172,  // TEXT_ADDX
    173,  // TEXT_ADDX
    163,  // TEXT_AND
    170,  // TEXT_AND
      3,  // TEXT_ANDI
      4,  // TEXT_ANDI
      5,  // TEXT_ANDI
    177,  // TEXT_ASL
    186,  // TEXT_ASL
    187,  // TEXT_ASL
    176,  // TEXT_ASR
    184,  // TEXT_ASR
    185,  // TEXT_ASR
    117,  // TEXT_BCC
    118,  // TEXT_BCC
     14,  // TEXT_BCHG
     15,  // TEXT_BCHG
     24,  // TEXT_BCHG
     25,  // TEXT_BCHG
     16,  // TEXT_BCLR
     17,  // TEXT_BCLR
     26,  // TEXT_BCLR
     27,  // TEXT_BCLR
    121,  // TEXT_BCS
    122,  // TEXT_BCS
    127,  // TEXT_BEQ
    128,  // TEXT_BEQ
    137,  // TEXT_BGE
    138,  // TEXT_BGE
    141,  // TEXT_BGT
    142,  // TEXT_BGT
    113,  // TEXT_BHI
    114,  // TEXT_BHI
    119,  // TEXT_BHS
    120,  // TEXT_BHS
    143,  // TEXT_BLE
    144,  // TEXT_BLE
    123,  // TEXT_BLO
    124,  // TEXT_BLO
    115,  // TEXT_BLS
    116,  // TEXT_BLS
    139,  // TEXT_BLT
    140,  // TEXT_BLT
    135,  // TEXT_BMI
    136,  // TEXT_BMI
    125,  // TEXT_BNE
    126,  // TEXT_BNE
    133,  // TEXT_BPL
    134,  // TEXT_BPL
    107,  // TEXT_BRA
    108,  // TEXT_BRA
     18,  // TEXT_BSET
     19,  // TEXT_BSET
     28,  // TEXT_BSET
     29,  // TEXT_BSET
    111,  // TEXT_BSR
    112,  // TEXT_BSR
    109,  // TEXT_BT
    110,  // TEXT_BT
     12,  // TEXT_BTST
     13,  // TEXT_BTST
     22,  // TEXT_BTST
     23,  // TEXT_BTST
    129,  // TEXT_BVC
    130,  // TEXT_BVC
    131,  // TEXT_BVS
    132,  // TEXT_BVS
     67,  // TEXT_CHK
     40,  // TEXT_CLR
    158,  // TEXT_CMP
    157,  // TEXT_CMPA
     11,  // TEXT_CMPI
    159,  // TEXT_CMPM
     73,  // TEXT_DBCC
     75,  // TEXT_DBCS
     78,  // TEXT_DBEQ
     70,  // TEXT_DBF
     83,  // TEXT_DBGE
     85,  // TEXT_DBGT
     71,  // TEXT_DBHI
     74,  // TEXT_DBHS
     86,  // TEXT_DBLE
     76,  // TEXT_DBLO
     72,  // TEXT_DBLS
     84,  // TEXT_DBLT
     82,  // TEXT_DBMI
     77,  // TEXT_DBNE
     81,  // TEXT_DBPL
     69,  // TEXT_DBRA
     68,  // TEXT_DBT
     79,  // TEXT_DBVC
     80,  // TEXT_DBVS
    147,  // TEXT_DIVS
    146,  // TEXT_DIVU
    160,  // TEXT_EOR
      8,  // TEXT_EORI
      9,  // TEXT_EORI
     10,  // TEXT_EORI
    166,  // TEXT_EXG
    167,  // TEXT_EXG
    168,  // TEXT_EXG
    169,  // TEXT_EXG
     43,  // TEXT_EXT
     47,  // TEXT_ILLEG
     63,  // TEXT_JMP
     62,  // TEXT_JSR
     66,  // TEXT_LEA
     51,  // TEXT_LINK
    179,  // TEXT_LSL
    190,  // TEXT_LSL
    191,  // TEXT_LSL
    178,  // TEXT_LSR
    188,  // TEXT_LSR
    189,  // TEXT_LSR
     33,  // TEXT_MOVE
     34,  // TEXT_MOVE
     35,  // TEXT_MOVE
     36,  // TEXT_MOVE
     37,  // TEXT_MOVE
     38,  // TEXT_MOVE
     53,  // TEXT_MOVE
     54,  // TEXT_MOVE
     30,  // TEXT_MOVEA
     31,  // TEXT_MOVEA
     32,  // TEXT_MOVEA
     64,  // TEXT_MOVEM
     65,  // TEXT_MOVEM
     20,  // TEXT_MOVEP
     21,  // TEXT_MOVEP
    145,  // TEXT_MOVEQ
    162,  // TEXT_MULS
    161,  // TEXT_MULU
     44,  // TEXT_NBCD
     41,  // TEXT_NEG
     39,  // TEXT_NEGX
     56,  // TEXT_NOP
     42,  // TEXT_NOT
    148,  // TEXT_OR
    151,  // TEXT_OR
      0,  // TEXT_ORI
      1,  // TEXT_ORI
      2,  // TEXT_ORI
     46,  // TEXT_PEA
     55,  // TEXT_RESET
    183,  // TEXT_ROL
    198,  // TEXT_ROL
    199,  // TEXT_ROL
    182,  // TEXT_ROR
    196,  // TEXT_ROR
    197,  // TEXT_ROR
    181,  // TEXT_ROXL
    194,  // TEXT_ROXL
    195,  // TEXT_ROXL
    180,  // TEXT_ROXR
    192,  // TEXT_ROXR
    193,  // TEXT_ROXR
     58,  // TEXT_RTE
     61,  // TEXT_RTR
     59,  // TEXT_RTS
    149,  // TEXT_SBCD
    150,  // TEXT_SBCD
     91,  // TEXT_SCC
     93,  // TEXT_SCS
     96,  // TEXT_SEQ
     88,  // TEXT_SF
    101,  // TEXT_SGE
    103,  // TEXT_SGT
     89,  // TEXT_SHI
     92,  // TEXT_SHS
    104,  // TEXT_SLE
     94,  // TEXT_SLO
     90,  // TEXT_SLS
    102,  // TEXT_SLT
    100,  // TEXT_SMI
     95,  // TEXT_SNE
     99,  // TEXT_SPL
     87,  // TEXT_ST
     57,  // TEXT_STOP
    155,  // TEXT_SUB
    156,  // TEXT_SUB
    152,  // TEXT_SUBA
      6,  // TEXT_SUBI
    106,  // TEXT_SUBQ
    153,  // TEXT_SUBX
    154,  // TEXT_SUBX
     97,  // TEXT_SVC
     98,  // TEXT_SVS
     45,  // TEXT_SWAP
     48,  // TEXT_TAS
     50,  // TEXT_TRAP
     60,  // TEXT_TRAPV
     49,  // TEXT_TST
     52,  // TEXT_UNLK
};

static constexpr Entry ALIAS_TABLE[] PROGMEM = {
    A2(0020100, TEXT_MOVE,  LONG, M_RADDR, M_AREG,  10, _3, LONG),
    A2(0030100, TEXT_MOVE,  WORD, M_RADDR, M_AREG,  10, _3, WORD),
    A2(0110300, TEXT_SUB,   DATA, M_RADDR, M_AREG,  10, _3, ADR8),
    A2(0110374, TEXT_SUBI,  DATA, M_IMDAT, M_AREG,  __, _3, ADR8),
    A2(0130374, TEXT_CMPI,  DATA, M_IMDAT, M_AREG,  __, _3, ADR8),
    A2(0150300, TEXT_ADD,   DATA, M_RADDR, M_AREG,  10, _3, ADR8),
    A2(0150374, TEXT_ADDI,  DATA, M_IMDAT, M_AREG,  __, _3, ADR8),
};

static constexpr uint8_t ALIAS_INDEX[] PROGMEM = {
      5,  // TEXT_ADD
      6,  // TEXT_ADDI
      4,  // TEXT_CMPI
      0,  // TEXT_MOVE
      1,  // TEXT_MOVE
      2,  // TEXT_SUB
      3,  // TEXT_SUBI
};
// clang-format on

static constexpr TableMc68000::EntryPage MC68000_PAGES[] PROGMEM = {
        {ARRAY_RANGE(MC68000_TABLE), ARRAY_RANGE(MC68000_INDEX)},
};

static constexpr TableMc68000::EntryPage ALIAS_PAGES[] PROGMEM = {
        {ARRAY_RANGE(ALIAS_TABLE), ARRAY_RANGE(ALIAS_INDEX)},
        {ARRAY_RANGE(MC68000_TABLE), ARRAY_RANGE(MC68000_INDEX)},
};

static constexpr TableMc68000::Cpu CPU_TABLE[] PROGMEM = {
        {MC68000, TEXT_CPU_68000, ARRAY_RANGE(MC68000_PAGES)},
        {MC68000, TEXT_CPU_68000, ARRAY_RANGE(ALIAS_PAGES)},
};
static constexpr const TableMc68000::Cpu &MC68000_CPU = CPU_TABLE[0];
static constexpr const TableMc68000::Cpu &MC68000_CPU_WITH_ALIAS = CPU_TABLE[1];

static bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_DREG)
        return table == M_RADDR || table == M_RDATA || table == M_WADDR || table == M_WDATA ||
               table == M_MULT;
    if (opr == M_AREG)
        return table == M_RADDR || table == M_WADDR || table == M_MULT;
    if (opr == M_AIND || opr == M_DISP || opr == M_INDX || opr == M_AWORD || opr == M_ALONG)
        return table == M_RADDR || table == M_RDATA || table == M_WADDR || table == M_WDATA ||
               table == M_RMEM || table == M_WMEM || table == M_JADDR || table == M_IADDR ||
               table == M_DADDR;
    if (opr == M_PINC)
        return table == M_RADDR || table == M_RDATA || table == M_WADDR || table == M_WDATA ||
               table == M_RMEM || table == M_WMEM || table == M_IADDR;
    if (opr == M_PDEC)
        return table == M_RADDR || table == M_RDATA || table == M_WADDR || table == M_WDATA ||
               table == M_RMEM || table == M_WMEM || table == M_DADDR;
    if (opr == M_PCDSP || opr == M_PCIDX)
        return table == M_RADDR || table == M_RDATA || table == M_RMEM || table == M_JADDR ||
               table == M_IADDR;
    if (opr == M_IMDAT)
        return table == M_RADDR || table == M_RDATA || table == M_IMBIT || table == M_IM3 ||
               table == M_IM8 || table == M_IMVEC || table == M_IMDSP;
    if (opr == M_LABEL)
        return table == M_REL16 || table == M_REL8;
    return false;
}

static bool acceptSize(InsnSize insn, OprSize table, InsnSize isize) {
    const OprSize opr = OprSize(insn);
    if (opr == table)
        return true;
    if (opr == SZ_BYTE)
        return table == SZ_DATA;
    if (opr == SZ_WORD)
        return table == SZ_DATA || table == SZ_ADR6 || table == SZ_ADR8;
    if (opr == SZ_LONG)
        return table == SZ_DATA || table == SZ_ADR6 || table == SZ_ADR8;
    if (opr == SZ_NONE)
        return table == SZ_WORD || table == SZ_DATA || isize == ISZ_NONE || table == SZ_ADR6 ||
               table == SZ_ADR8;
    return false;
}

static bool acceptModes(InsnMc68000 &insn, const Entry *entry) {
    auto flags = insn.flags();
    auto table = entry->flags();
    return acceptMode(flags.src(), table.src()) && acceptMode(flags.dst(), table.dst()) &&
           acceptSize(flags.insnSize(), table.oprSize(), table.insnSize());
}

Error TableMc68000::searchName(InsnMc68000 &insn) const {
    _cpu->searchName(insn, acceptModes);
    return insn.getError();
}

static Config::opcode_t getInsnMask(AddrMode src) {
    if (src == M_IM8 || src == M_REL8)
        return 0xFF;
    if (src == M_IMVEC)
        return 0xF;
    if (src == M_IM3)
        return 07000;
    return 0;
}

static Config::opcode_t getInsnMask(OprPos pos) {
    switch (pos) {
    case OP_10:
        return 00077;
    case OP_23:
        return 07700;
    case OP__0:
        return 00007;
    case OP__3:
        return 07000;
    default:
        return 0;
    }
}

static Config::opcode_t getInsnMask(OprSize size) {
    switch (size) {
    case SZ_DATA:
        return (3 << 6);
    case SZ_ADR6:
        return (1 << 6);
    case SZ_ADR8:
        return (1 << 8);
    default:
        return 0;
    }
}

static Config::opcode_t getInsnMask(Entry::Flags flags) {
    return getInsnMask(flags.src()) | getInsnMask(flags.srcPos()) | getInsnMask(flags.dstPos()) |
           getInsnMask(flags.oprSize());
}

static bool matchOpCode(
        InsnMc68000 &insn, const Entry *entry, const TableMc68000::EntryPage *page) {
    auto opCode = insn.opCode();
    opCode &= ~getInsnMask(entry->flags());
    return opCode == entry->opCode();
}

Error TableMc68000::searchOpCode(InsnMc68000 &insn, StrBuffer &out) const {
    _cpu->searchOpCode(insn, out, matchOpCode);
    return insn.getError();
}

TableMc68000::TableMc68000() {
    setAlias(false);
}

void TableMc68000::setAlias(bool enable) {
    _cpu = enable ? &MC68000_CPU_WITH_ALIAS : &MC68000_CPU;
}

bool TableMc68000::setCpu(const char *cpu) {
    auto p = cpu;
    if (strncasecmp_P(cpu, TEXT_CPU_MC68000, 2) == 0)
        p += 2;
    return strcmp_P(p, TEXT_CPU_68000) == 0;
}

TableMc68000 TableMc68000::TABLE;

}  // namespace mc68000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

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

#include "config_mc68000.h"
#include "entry_mc68000.h"
#include "table_mc68000.h"
#include "text_mc68000.h"

#include <string.h>

namespace libasm {
namespace mc68000 {

#if defined(SPL)
#undef SPL
#endif

#define X(_opc, _name, _isize, _msrc, _mdst, _spos, _dpos, _osize, _alias) \
    {                                                                   \
        _opc,                                                           \
        Entry::_flags(Entry::_opr(_msrc),                               \
                      Entry::_opr(_mdst),                               \
                      Entry::_pos(OP_##_spos, OP_##_dpos, _alias),      \
                      Entry::_size(SZ_##_osize, ISZ_##_isize)),         \
        TEXT_##_name,                                                   \
    },
#define E(_opc, _name, _isize, _msrc, _mdst, _spos, _dpos, _osize)      \
    X(_opc, _name, _isize, _msrc, _mdst, _spos, _dpos, _osize, false)
#define A(_opc, _name, _isize, _msrc, _mdst, _spos, _dpos, _osize)      \
    X(_opc, _name, _isize, _msrc, _mdst, _spos, _dpos, _osize, true)

static constexpr Entry MC68000_TABLE[] PROGMEM = {
    E(0000074, ORI,   NONE, M_IMDAT, M_CCR,   __, __, WORD)
    E(0000174, ORI,   NONE, M_IMDAT, M_SR,    __, __, WORD)
    E(0000000, ORI,   DATA, M_IMDAT, M_WDATA, __, 10, DATA)
    E(0001074, ANDI,  NONE, M_IMDAT, M_CCR,   __, __, WORD)
    E(0001174, ANDI,  NONE, M_IMDAT, M_SR,    __, __, WORD)
    E(0001000, ANDI,  DATA, M_IMDAT, M_WDATA, __, 10, DATA)
    E(0002000, SUBI,  DATA, M_IMDAT, M_WDATA, __, 10, DATA)
    E(0003000, ADDI,  DATA, M_IMDAT, M_WDATA, __, 10, DATA)
    E(0005074, EORI,  NONE, M_IMDAT, M_CCR,   __, __, WORD)
    E(0005174, EORI,  NONE, M_IMDAT, M_SR,    __, __, WORD)
    E(0005000, EORI,  DATA, M_IMDAT, M_WDATA, __, 10, DATA)
    E(0006000, CMPI,  DATA, M_IMDAT, M_WDATA, __, 10, DATA)
    E(0004000, BTST,  LONG, M_IMBIT, M_DREG,  __, _0, LONG)
    E(0004000, BTST,  BYTE, M_IMBIT, M_RMEM,  __, 10, BYTE)
    E(0004100, BCHG,  LONG, M_IMBIT, M_DREG,  __, _0, LONG)
    E(0004100, BCHG,  BYTE, M_IMBIT, M_WDATA, __, 10, BYTE)
    E(0004200, BCLR,  LONG, M_IMBIT, M_DREG,  __, _0, LONG)
    E(0004200, BCLR,  BYTE, M_IMBIT, M_WDATA, __, 10, BYTE)
    E(0004300, BSET,  LONG, M_IMBIT, M_DREG,  __, _0, LONG)
    E(0004300, BSET,  BYTE, M_IMBIT, M_WDATA, __, 10, BYTE)
    E(0000410, MOVEP, DATA, M_DISP,  M_DREG,  _0, _3, ADR6)
    E(0000610, MOVEP, DATA, M_DREG,  M_DISP,  _3, _0, ADR6)
    E(0000400, BTST,  LONG, M_DREG,  M_DREG,  _3, _0, LONG)
    E(0000400, BTST,  BYTE, M_DREG,  M_RMEM,  _3, 10, BYTE)
    E(0000500, BCHG,  LONG, M_DREG,  M_DREG,  _3, _0, LONG)
    E(0000500, BCHG,  BYTE, M_DREG,  M_WDATA, _3, 10, BYTE)
    E(0000600, BCLR,  LONG, M_DREG,  M_DREG,  _3, _0, LONG)
    E(0000600, BCLR,  BYTE, M_DREG,  M_WDATA, _3, 10, BYTE)
    E(0000700, BSET,  LONG, M_DREG,  M_DREG,  _3, _0, LONG)
    E(0000700, BSET,  BYTE, M_DREG,  M_WDATA, _3, 10, BYTE)
    E(0020100, MOVEA, LONG, M_RADDR, M_AREG,  10, _3, LONG)
    E(0030100, MOVEA, WORD, M_RADDR, M_AREG,  10, _3, WORD)
    E(0042300, MOVE,  NONE, M_RDATA, M_CCR,   10, __, WORD)
    E(0040300, MOVE,  NONE, M_SR,    M_WDATA, __, 10, WORD)
    E(0043300, MOVE,  NONE, M_RDATA, M_SR,    10, __, WORD)
    E(0010000, MOVE,  BYTE, M_RDATA, M_WDATA, 10, 23, BYTE)
    E(0020000, MOVE,  LONG, M_RADDR, M_WDATA, 10, 23, LONG)
    E(0030000, MOVE,  WORD, M_RADDR, M_WDATA, 10, 23, WORD)
    E(0040000, NEGX,  DATA, M_WDATA, M_NONE,  10, __, DATA)
    E(0041000, CLR,   DATA, M_WDATA, M_NONE,  10, __, DATA)
    E(0042000, NEG,   DATA, M_WDATA, M_NONE,  10, __, DATA)
    E(0043000, NOT,   DATA, M_WDATA, M_NONE,  10, __, DATA)
    E(0044200, EXT,   DATA, M_DREG,  M_NONE,  _0, __, ADR6)
    E(0044000, NBCD,  NONE, M_WDATA, M_NONE,  10, __, BYTE)
    E(0044100, SWAP,  NONE, M_DREG,  M_NONE,  _0, __, NONE)
    E(0044100, PEA,   NONE, M_JADDR, M_NONE,  10, __, NONE)
    E(0045374, ILLEG, NONE, M_NONE,  M_NONE,  __, __, NONE)
    E(0045300, TAS,   NONE, M_WDATA, M_NONE,  10, __, BYTE)
    E(0045000, TST,   DATA, M_RMEM,  M_NONE,  10, __, DATA)
    E(0047100, TRAP,  NONE, M_IMVEC, M_NONE,  __, __, NONE)
    E(0047120, LINK,  NONE, M_AREG,  M_IMDSP, _0, __, WORD)
    E(0047130, UNLK,  NONE, M_AREG,  M_NONE,  _0, __, NONE)
    E(0047140, MOVE,  NONE, M_AREG,  M_USP,   _0, __, NONE)
    E(0047150, MOVE,  NONE, M_USP,   M_AREG,  __, _0, NONE)
    E(0047160, RESET, NONE, M_NONE,  M_NONE,  __, __, NONE)
    E(0047161, NOP,   NONE, M_NONE,  M_NONE,  __, __, NONE)
    E(0047162, STOP,  NONE, M_IMDAT, M_NONE,  __, __, WORD)
    E(0047163, RTE,   NONE, M_NONE,  M_NONE,  __, __, NONE)
    E(0047165, RTS,   NONE, M_NONE,  M_NONE,  __, __, NONE)
    E(0047166, TRAPV, NONE, M_NONE,  M_NONE,  __, __, NONE)
    E(0047167, RTR,   NONE, M_NONE,  M_NONE,  __, __, NONE)
    E(0047200, JSR,   NONE, M_JADDR, M_NONE,  10, __, WORD)
    E(0047300, JMP,   NONE, M_JADDR, M_NONE,  10, __, WORD)
    E(0044200, MOVEM, DATA, M_MULT,  M_DADDR, __, 10, ADR6)
    E(0046200, MOVEM, DATA, M_IADDR, M_MULT,  10, __, ADR6)
    E(0040700, LEA,   NONE, M_JADDR, M_AREG,  10, _3, NONE)
    E(0040600, CHK,   WORD, M_RDATA, M_DREG,  10, _3, WORD)
    E(0050310, DBT,   NONE, M_DREG,  M_REL16, _0, __, WORD)
    E(0050710, DBRA,  NONE, M_DREG,  M_REL16, _0, __, WORD)
    E(0050710, DBF,   NONE, M_DREG,  M_REL16, _0, __, WORD)
    E(0051310, DBHI,  NONE, M_DREG,  M_REL16, _0, __, WORD)
    E(0051710, DBLS,  NONE, M_DREG,  M_REL16, _0, __, WORD)
    E(0052310, DBCC,  NONE, M_DREG,  M_REL16, _0, __, WORD)
    E(0052310, DBHS,  NONE, M_DREG,  M_REL16, _0, __, WORD)
    E(0052710, DBCS,  NONE, M_DREG,  M_REL16, _0, __, WORD)
    E(0052710, DBLO,  NONE, M_DREG,  M_REL16, _0, __, WORD)
    E(0053310, DBNE,  NONE, M_DREG,  M_REL16, _0, __, WORD)
    E(0053710, DBEQ,  NONE, M_DREG,  M_REL16, _0, __, WORD)
    E(0054310, DBVC,  NONE, M_DREG,  M_REL16, _0, __, WORD)
    E(0054710, DBVS,  NONE, M_DREG,  M_REL16, _0, __, WORD)
    E(0055310, DBPL,  NONE, M_DREG,  M_REL16, _0, __, WORD)
    E(0055710, DBMI,  NONE, M_DREG,  M_REL16, _0, __, WORD)
    E(0056310, DBGE,  NONE, M_DREG,  M_REL16, _0, __, WORD)
    E(0056710, DBLT,  NONE, M_DREG,  M_REL16, _0, __, WORD)
    E(0057310, DBGT,  NONE, M_DREG,  M_REL16, _0, __, WORD)
    E(0057710, DBLE,  NONE, M_DREG,  M_REL16, _0, __, WORD)
    E(0050300, ST,    NONE, M_WDATA, M_NONE,  10, __, BYTE)
    E(0050700, SF,    NONE, M_WDATA, M_NONE,  10, __, BYTE)
    E(0051300, SHI,   NONE, M_WDATA, M_NONE,  10, __, BYTE)
    E(0051700, SLS,   NONE, M_WDATA, M_NONE,  10, __, BYTE)
    E(0052300, SCC,   NONE, M_WDATA, M_NONE,  10, __, BYTE)
    E(0052300, SHS,   NONE, M_WDATA, M_NONE,  10, __, BYTE)
    E(0052700, SCS,   NONE, M_WDATA, M_NONE,  10, __, BYTE)
    E(0052700, SLO,   NONE, M_WDATA, M_NONE,  10, __, BYTE)
    E(0053300, SNE,   NONE, M_WDATA, M_NONE,  10, __, BYTE)
    E(0053700, SEQ,   NONE, M_WDATA, M_NONE,  10, __, BYTE)
    E(0054300, SVC,   NONE, M_WDATA, M_NONE,  10, __, BYTE)
    E(0054700, SVS,   NONE, M_WDATA, M_NONE,  10, __, BYTE)
    E(0055300, SPL,   NONE, M_WDATA, M_NONE,  10, __, BYTE)
    E(0055700, SMI,   NONE, M_WDATA, M_NONE,  10, __, BYTE)
    E(0056300, SGE,   NONE, M_WDATA, M_NONE,  10, __, BYTE)
    E(0056700, SLT,   NONE, M_WDATA, M_NONE,  10, __, BYTE)
    E(0057300, SGT,   NONE, M_WDATA, M_NONE,  10, __, BYTE)
    E(0057700, SLE,   NONE, M_WDATA, M_NONE,  10, __, BYTE)
    E(0050000, ADDQ,  DATA, M_IM3,   M_WADDR, __, 10, DATA)
    E(0050400, SUBQ,  DATA, M_IM3,   M_WADDR, __, 10, DATA)
    E(0060000, BRA,   NONE, M_REL8,  M_NONE,  __, __, WORD)
    E(0060000, BT,    NONE, M_REL8,  M_NONE,  __, __, WORD)
    E(0060400, BSR,   NONE, M_REL8,  M_NONE,  __, __, WORD)
    E(0061000, BHI,   NONE, M_REL8,  M_NONE,  __, __, WORD)
    E(0061400, BLS,   NONE, M_REL8,  M_NONE,  __, __, WORD)
    E(0062000, BCC,   NONE, M_REL8,  M_NONE,  __, __, WORD)
    E(0062000, BHS,   NONE, M_REL8,  M_NONE,  __, __, WORD)
    E(0062400, BCS,   NONE, M_REL8,  M_NONE,  __, __, WORD)
    E(0062400, BLO,   NONE, M_REL8,  M_NONE,  __, __, WORD)
    E(0063000, BNE,   NONE, M_REL8,  M_NONE,  __, __, WORD)
    E(0063400, BEQ,   NONE, M_REL8,  M_NONE,  __, __, WORD)
    E(0064000, BVC,   NONE, M_REL8,  M_NONE,  __, __, WORD)
    E(0064400, BVS,   NONE, M_REL8,  M_NONE,  __, __, WORD)
    E(0065000, BPL,   NONE, M_REL8,  M_NONE,  __, __, WORD)
    E(0065400, BMI,   NONE, M_REL8,  M_NONE,  __, __, WORD)
    E(0066000, BGE,   NONE, M_REL8,  M_NONE,  __, __, WORD)
    E(0066400, BLT,   NONE, M_REL8,  M_NONE,  __, __, WORD)
    E(0067000, BGT,   NONE, M_REL8,  M_NONE,  __, __, WORD)
    E(0067400, BLE,   NONE, M_REL8,  M_NONE,  __, __, WORD)
    E(0070000, MOVEQ, NONE, M_IM8,   M_DREG,  __, _3, NONE)
    E(0100300, DIVU,  DATA, M_RDATA, M_DREG,  10, _3, WORD)
    E(0100700, DIVS,  DATA, M_RDATA, M_DREG,  10, _3, WORD)
    E(0100000, OR,    DATA, M_RDATA, M_DREG,  10, _3, DATA)
    E(0100400, SBCD,  NONE, M_DREG,  M_DREG,  _0, _3, BYTE)
    E(0100410, SBCD,  NONE, M_PDEC,  M_PDEC,  _0, _3, BYTE)
    E(0100400, OR,    DATA, M_DREG,  M_WMEM,  _3, 10, DATA)
    E(0110300, SUBA,  DATA, M_RADDR, M_AREG,  10, _3, ADR8)
    A(0110300, SUB,   DATA, M_RADDR, M_AREG,  10, _3, ADR8)
    A(0110374, SUBI,  DATA, M_IMDAT, M_AREG,  __, _3, ADR8)
    E(0110400, SUBX,  DATA, M_DREG,  M_DREG,  _0, _3, DATA)
    E(0110410, SUBX,  DATA, M_PDEC,  M_PDEC,  _0, _3, DATA)
    E(0110000, SUB,   DATA, M_RADDR, M_DREG,  10, _3, DATA)
    E(0110400, SUB,   DATA, M_DREG,  M_WMEM,  _3, 10, DATA)
    E(0130300, CMPA,  DATA, M_RADDR, M_AREG,  10, _3, ADR8)
    A(0130374, CMPI,  DATA, M_IMDAT, M_AREG,  __, _3, ADR8)
    E(0130000, CMP,   DATA, M_RADDR, M_DREG,  10, _3, DATA)
    E(0130410, CMPM,  DATA, M_PINC,  M_PINC,  _0, _3, DATA)
    E(0130400, EOR,   DATA, M_DREG,  M_WDATA, _3, 10, DATA)
    E(0140300, MULU,  DATA, M_RDATA, M_DREG,  10, _3, WORD)
    E(0140700, MULS,  DATA, M_RDATA, M_DREG,  10, _3, WORD)
    E(0140000, AND,   DATA, M_RDATA, M_DREG,  10, _3, DATA)
    E(0140400, ABCD,  NONE, M_DREG,  M_DREG,  _0, _3, BYTE)
    E(0140410, ABCD,  NONE, M_PDEC,  M_PDEC,  _0, _3, BYTE)
    E(0140500, EXG,   NONE, M_DREG,  M_DREG,  _3, _0, NONE)
    E(0140510, EXG,   NONE, M_AREG,  M_AREG,  _3, _0, NONE)
    E(0140610, EXG,   NONE, M_DREG,  M_AREG,  _3, _0, NONE)
    E(0140610, EXG,   NONE, M_AREG,  M_DREG,  _0, _3, NONE)
    E(0140400, AND,   DATA, M_DREG,  M_WMEM,  _3, 10, DATA)
    E(0150300, ADDA,  DATA, M_RADDR, M_AREG,  10, _3, ADR8)
    A(0150300, ADD,   DATA, M_RADDR, M_AREG,  10, _3, ADR8)
    A(0150374, ADDI,  DATA, M_IMDAT, M_AREG,  __, _3, ADR8)
    E(0150400, ADDX,  DATA, M_DREG,  M_DREG,  _0, _3, DATA)
    E(0150410, ADDX,  DATA, M_PDEC,  M_PDEC,  _0, _3, DATA)
    E(0150000, ADD,   DATA, M_RADDR, M_DREG,  10, _3, DATA)
    E(0150400, ADD,   DATA, M_DREG,  M_WMEM,  _3, 10, DATA)
    E(0160300, ASR,   NONE, M_WMEM,  M_NONE,  10, __, BYTE)
    E(0160700, ASL,   NONE, M_WMEM,  M_NONE,  10, __, BYTE)
    E(0161300, LSR,   NONE, M_WMEM,  M_NONE,  10, __, BYTE)
    E(0161700, LSL,   NONE, M_WMEM,  M_NONE,  10, __, BYTE)
    E(0162300, ROXR,  NONE, M_WMEM,  M_NONE,  10, __, BYTE)
    E(0162700, ROXL,  NONE, M_WMEM,  M_NONE,  10, __, BYTE)
    E(0163300, ROR,   NONE, M_WMEM,  M_NONE,  10, __, BYTE)
    E(0163700, ROL,   NONE, M_WMEM,  M_NONE,  10, __, BYTE)
    E(0160000, ASR,   DATA, M_IM3,   M_DREG,  __, _0, DATA)
    E(0160040, ASR,   DATA, M_DREG,  M_DREG,  _3, _0, DATA)
    E(0160400, ASL,   DATA, M_IM3,   M_DREG,  __, _0, DATA)
    E(0160440, ASL,   DATA, M_DREG,  M_DREG,  _3, _0, DATA)
    E(0160010, LSR,   DATA, M_IM3,   M_DREG,  __, _0, DATA)
    E(0160050, LSR,   DATA, M_DREG,  M_DREG,  _3, _0, DATA)
    E(0160410, LSL,   DATA, M_IM3,   M_DREG,  __, _0, DATA)
    E(0160450, LSL,   DATA, M_DREG,  M_DREG,  _3, _0, DATA)
    E(0160020, ROXR,  DATA, M_IM3,   M_DREG,  __, _0, DATA)
    E(0160060, ROXR,  DATA, M_DREG,  M_DREG,  _3, _0, DATA)
    E(0160420, ROXL,  DATA, M_IM3,   M_DREG,  __, _0, DATA)
    E(0160460, ROXL,  DATA, M_DREG,  M_DREG,  _3, _0, DATA)
    E(0160030, ROR,   DATA, M_IM3,   M_DREG,  __, _0, DATA)
    E(0160070, ROR,   DATA, M_DREG,  M_DREG,  _3, _0, DATA)
    E(0160430, ROL,   DATA, M_IM3,   M_DREG,  __, _0, DATA)
    E(0160470, ROL,   DATA, M_DREG,  M_DREG,  _3, _0, DATA)
};

static bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table) return true;
    if (opr == M_DREG)
        return table == M_RADDR || table == M_RDATA
            || table == M_WADDR || table == M_WDATA
            || table == M_RMEM  || table == M_MULT;
    if (opr == M_AREG)
        return table == M_RADDR || table == M_WADDR || table == M_MULT;
    if (opr == M_AIND || opr == M_DISP || opr == M_INDX
        || opr == M_AWORD || opr == M_ALONG)
        return table == M_RADDR || table == M_RDATA
            || table == M_WADDR || table == M_WDATA
            || table == M_RMEM  || table == M_WMEM
            || table == M_JADDR || table == M_IADDR
            || table == M_DADDR;
    if (opr == M_PINC)
        return table == M_RADDR || table == M_RDATA
            || table == M_WADDR || table == M_WDATA
            || table == M_RMEM  || table == M_WMEM
            || table == M_IADDR;
    if (opr == M_PDEC)        
        return table == M_RADDR || table == M_RDATA
            || table == M_WADDR || table == M_WDATA
            || table == M_RMEM  || table == M_WMEM
            || table == M_DADDR;
    if (opr == M_PCDSP || opr == M_PCIDX)
        return table == M_RADDR || table == M_RDATA
            || table == M_RMEM  || table == M_JADDR
            || table == M_IADDR;
    if (opr == M_IMDAT)
        return table == M_RADDR || table == M_RDATA
            || table == M_IMBIT || table == M_IM3
            || table == M_IM8   || table == M_IMVEC
            || table == M_IMDSP;
    if (opr == M_LABEL)
        return table == M_REL16 || table == M_REL8;
    return false;
}

static bool acceptSize(OprSize opr, OprSize table, InsnSize isize) {
    if (opr == table) return true;
    if (opr == SZ_BYTE)
        return table == SZ_DATA;
    if (opr == SZ_WORD)
        return table == SZ_DATA || table == SZ_ADR6 || table == SZ_ADR8;
    if (opr == SZ_LONG)
        return table == SZ_DATA || table == SZ_ADR6 || table == SZ_ADR8;
    if (opr == SZ_NONE)
        return table == SZ_WORD || table == SZ_DATA || isize == ISZ_NONE
            || table == SZ_ADR6 || table == SZ_ADR8;
    return false;
}

static bool matchAddrMode(uint32_t flags, const Entry *entry) {
    const uint32_t table = pgm_read_dword(&entry->flags);
    return acceptMode(Entry::_mode(Entry::_src(flags)), Entry::_mode(Entry::_src(table)))
        && acceptMode(Entry::_mode(Entry::_dst(flags)), Entry::_mode(Entry::_dst(table)))
        && acceptSize(
            Entry::_oprSize(Entry::_size(flags)),
            Entry::_oprSize(Entry::_size(table)),
            Entry::_insnSize(Entry::_size(table)));
}

Error TableMc68000::searchName(InsnMc68000 &insn) const {
    const uint32_t flags = Entry::_flags(
        Entry::_opr(insn.srcMode()), Entry::_opr(insn.dstMode()),
        OP___, Entry::_size(insn.oprSize(), ISZ_NONE));
    uint8_t count = 0;
    const Entry *const end = ARRAY_END(MC68000_TABLE);
    for (const Entry *entry = ARRAY_BEGIN(MC68000_TABLE);
         entry < end
             && (entry = TableBase::searchName<Entry,uint32_t>(
                     insn.name(), flags, entry, end, matchAddrMode, count));
         entry++) {
        insn.setFlags(pgm_read_dword(&entry->flags));
        if (insn.alias() && !_aliasEnabled)
            continue;
        insn.setOpCode(pgm_read_word(&entry->opCode));
        return _error.setOK();
    }
    return _error.setError(
        count == 0 ? UNKNOWN_INSTRUCTION : OPERAND_NOT_ALLOWED);
}

static Config::opcode_t getInsnMask(AddrMode srcMode) {
    if (srcMode == M_IM8 || srcMode == M_REL8)
        return 0xFF;
    if (srcMode == M_IMVEC)
        return 0xF;
    if (srcMode == M_IM3)
        return 07000;
    return 0;
}

static Config::opcode_t getInsnMask(OprPos pos) {
    switch (pos) {
    case OP_10: return 00077;
    case OP_23: return 07700;
    case OP__0: return 00007;
    case OP__3: return 07000;
    default:    return 0;
    }
}

static Config::opcode_t getInsnMask(OprSize size) {
    switch (size) {
    case SZ_DATA: return (3 << 6);
    case SZ_ADR6: return (1 << 6);
    case SZ_ADR8: return (1 << 8);
    default: return 0;
    }
}

static Config::opcode_t getInsnMask(uint32_t flags) {
    return getInsnMask(Entry::_mode(Entry::_src(flags)))
        | getInsnMask(Entry::_srcPos(Entry::_pos(flags)))
        | getInsnMask(Entry::_dstPos(Entry::_pos(flags)))
        | getInsnMask(Entry::_oprSize(Entry::_size(flags)));
}

static Config::opcode_t maskCode(Config::opcode_t opCode, const Entry *entry) {
    const uint32_t flags = pgm_read_dword(&entry->flags);
    const Config::opcode_t mask = getInsnMask(flags);
    return opCode & ~mask;
}

Error TableMc68000::searchOpCode(InsnMc68000 &insn) const {
    const Config::opcode_t opCode = insn.opCode();
    const Entry *entry = TableBase::searchCode<Entry, Config::opcode_t>(
        opCode, ARRAY_RANGE(MC68000_TABLE), maskCode);
    if (entry) {
        insn.setFlags(pgm_read_dword(&entry->flags));
        const char *name =
            reinterpret_cast<const char *>(pgm_read_ptr(&entry->name));
        TableBase::setName(insn.insn(), name, Config::NAME_MAX);
        return _error.setOK();
    }
    return _error.setError(UNKNOWN_INSTRUCTION);
}

TableMc68000::TableMc68000()
    : _aliasEnabled(false)
{}

bool TableMc68000::setCpu(const char *cpu) {
    const char *p = cpu + (strncasecmp(cpu, "MC", 2) ? 0 : 2);
    return strcmp(p, "68000") == 0
        || strcmp(p, "68008") == 0;
}

class TableMc68000 TableMc68000;

} // namespace mc68000
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

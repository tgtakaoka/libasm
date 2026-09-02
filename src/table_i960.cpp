/*
 * Copyright 2026 Tadashi G. Takaoka
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

#include "table_i960.h"

#include "entry_i960.h"
#include "entry_table.h"
#include "text_i960.h"

using namespace libasm::text::i960;

namespace libasm {
namespace i960 {

#define E0(_opc, _name) {_opc, Entry::Flags::create(), _name}
#define E1(_opc, _name, _o1) {_opc, Entry::Flags::create(_o1), _name}
#define E2(_opc, _name, _o1, _o2) {_opc, Entry::Flags::create(_o1, _o2), _name}
#define E3(_opc, _name, _o1, _o2, _o3) {_opc, Entry::Flags::create(_o1, _o2, _o3), _name}
// The 80960KB extensions, which the fpu option enables.
#define F1(_opc, _name, _o1) {_opc, Entry::Flags::create(_o1, M_NONE, M_NONE, true), _name}
#define F2(_opc, _name, _o1, _o2) {_opc, Entry::Flags::create(_o1, _o2, M_NONE, true), _name}
#define F3(_opc, _name, _o1, _o2, _o3) {_opc, Entry::Flags::create(_o1, _o2, _o3, true), _name}

// clang-format off
// An entry holds the whole 32-bit instruction with its operand fields zeroed.
// The mode bit of a field the instruction does not use is part of the
// operation code and reads one, which is the form GNU as and the original
// Intel assembler emit; an encoding which leaves such a bit zero does not
// match, so a disassembled instruction assembles back to the same word.
constexpr Entry TABLE_I960[] PROGMEM = {
    // REG format
    E3(0x58000000, TEXT_NOTBIT,  M_SRC1, M_SRC2, M_DST),
    E3(0x58000080, TEXT_AND,     M_SRC1, M_SRC2, M_DST),
    E3(0x58000100, TEXT_ANDNOT,  M_SRC1, M_SRC2, M_DST),
    E3(0x58000180, TEXT_SETBIT,  M_SRC1, M_SRC2, M_DST),
    E3(0x58000200, TEXT_NOTAND,  M_SRC1, M_SRC2, M_DST),
    E3(0x58000300, TEXT_XOR,     M_SRC1, M_SRC2, M_DST),
    E3(0x58000380, TEXT_OR,      M_SRC1, M_SRC2, M_DST),
    E3(0x58000400, TEXT_NOR,     M_SRC1, M_SRC2, M_DST),
    E3(0x58000480, TEXT_XNOR,    M_SRC1, M_SRC2, M_DST),
    E3(0x58000580, TEXT_ORNOT,   M_SRC1, M_SRC2, M_DST),
    E3(0x58000600, TEXT_CLRBIT,  M_SRC1, M_SRC2, M_DST),
    E3(0x58000680, TEXT_NOTOR,   M_SRC1, M_SRC2, M_DST),
    E3(0x58000700, TEXT_NAND,    M_SRC1, M_SRC2, M_DST),
    E3(0x58000780, TEXT_ALTERBIT, M_SRC1, M_SRC2, M_DST),
    E2(0x58001500, TEXT_NOT,     M_SRC1, M_DST),
    E3(0x59000000, TEXT_ADDO,    M_SRC1, M_SRC2, M_DST),
    E3(0x59000080, TEXT_ADDI,    M_SRC1, M_SRC2, M_DST),
    E3(0x59000100, TEXT_SUBO,    M_SRC1, M_SRC2, M_DST),
    E3(0x59000180, TEXT_SUBI,    M_SRC1, M_SRC2, M_DST),
    E3(0x59000400, TEXT_SHRO,    M_SRC1, M_SRC2, M_DST),
    E3(0x59000500, TEXT_SHRDI,   M_SRC1, M_SRC2, M_DST),
    E3(0x59000580, TEXT_SHRI,    M_SRC1, M_SRC2, M_DST),
    E3(0x59000600, TEXT_SHLO,    M_SRC1, M_SRC2, M_DST),
    E3(0x59000680, TEXT_ROTATE,  M_SRC1, M_SRC2, M_DST),
    E3(0x59000700, TEXT_SHLI,    M_SRC1, M_SRC2, M_DST),
    E3(0x5A000200, TEXT_CMPINCO, M_SRC1, M_SRC2, M_DST),
    E3(0x5A000280, TEXT_CMPINCI, M_SRC1, M_SRC2, M_DST),
    E3(0x5A000300, TEXT_CMPDECO, M_SRC1, M_SRC2, M_DST),
    E3(0x5A000380, TEXT_CMPDECI, M_SRC1, M_SRC2, M_DST),
    E2(0x5A002000, TEXT_CMPO,    M_SRC1, M_SRC2),
    E2(0x5A002080, TEXT_CMPI,    M_SRC1, M_SRC2),
    E2(0x5A002100, TEXT_CONCMPO, M_SRC1, M_SRC2),
    E2(0x5A002180, TEXT_CONCMPI, M_SRC1, M_SRC2),
    E2(0x5A002600, TEXT_SCANBYTE, M_SRC1, M_SRC2),
    E2(0x5A002700, TEXT_CHKBIT,  M_SRC1, M_SRC2),
    E3(0x5B000000, TEXT_ADDC,    M_SRC1, M_SRC2, M_DST),
    E3(0x5B000100, TEXT_SUBC,    M_SRC1, M_SRC2, M_DST),
    E2(0x5C001600, TEXT_MOV,     M_SRC1, M_DST),
    E2(0x5D001600, TEXT_MOVL,    M_SRC1L, M_DSTL),
    E2(0x5E001600, TEXT_MOVT,    M_SRC1Q, M_DSTQ),
    E2(0x5F001600, TEXT_MOVQ,    M_SRC1Q, M_DSTQ),
    E2(0x60002000, TEXT_SYNMOV,  M_REG1, M_REG2),
    E2(0x60002080, TEXT_SYNMOVL, M_REG1, M_REG2),
    E2(0x60002100, TEXT_SYNMOVQ, M_REG1, M_REG2),
    E3(0x61000000, TEXT_ATMOD,   M_REG1, M_SRC2, M_DST),
    E3(0x61000100, TEXT_ATADD,   M_REG1, M_SRC2, M_DST),
    E2(0x61001280, TEXT_SYNLD,   M_REG1, M_DST),
    E3(0x64000280, TEXT_MODAC,   M_SRC1, M_SRC2, M_DST),
    E2(0x64001000, TEXT_SPANBIT, M_SRC1, M_DST),
    E2(0x64001080, TEXT_SCANBIT, M_SRC1, M_DST),
    E3(0x65000000, TEXT_MODIFY,  M_SRC1, M_SRC2, M_DST),
    E3(0x65000080, TEXT_EXTRACT, M_SRC1, M_SRC2, M_DST),
    E3(0x65000200, TEXT_MODTC,   M_SRC1, M_SRC2, M_DST),
    E3(0x65000280, TEXT_MODPC,   M_SRC1, M_SRC2, M_DST),
    E1(0x66003000, TEXT_CALLS,   M_SRC1),
    E0(0x66003D80, TEXT_MARK),
    E0(0x66003E00, TEXT_FMARK),
    E0(0x66003E80, TEXT_FLUSHREG),
    E0(0x66003F80, TEXT_SYNCF),
    E3(0x67000000, TEXT_EMUL,    M_SRC1, M_SRC2, M_DSTL),
    E3(0x67000080, TEXT_EDIV,    M_SRC1, M_SRC2L, M_DST),
    E3(0x70000080, TEXT_MULO,    M_SRC1, M_SRC2, M_DST),
    E3(0x70000400, TEXT_REMO,    M_SRC1, M_SRC2, M_DST),
    E3(0x70000580, TEXT_DIVO,    M_SRC1, M_SRC2, M_DST),
    E3(0x74000080, TEXT_MULI,    M_SRC1, M_SRC2, M_DST),
    E3(0x74000400, TEXT_REMI,    M_SRC1, M_SRC2, M_DST),
    E3(0x74000480, TEXT_MODI,    M_SRC1, M_SRC2, M_DST),
    E3(0x74000580, TEXT_DIVI,    M_SRC1, M_SRC2, M_DST),
    // COBR format: compare and branch, and test
    E1(0x20000000, TEXT_TESTNO,  M_CREG1),
    E1(0x21000000, TEXT_TESTG,   M_CREG1),
    E1(0x22000000, TEXT_TESTE,   M_CREG1),
    E1(0x23000000, TEXT_TESTGE,  M_CREG1),
    E1(0x24000000, TEXT_TESTL,   M_CREG1),
    E1(0x25000000, TEXT_TESTNE,  M_CREG1),
    E1(0x26000000, TEXT_TESTLE,  M_CREG1),
    E1(0x27000000, TEXT_TESTO,   M_CREG1),
    E3(0x30000000, TEXT_BBC,     M_CSRC1, M_CSRC2, M_CDISP),
    E3(0x31000000, TEXT_CMPOBG,  M_CSRC1, M_CSRC2, M_CDISP),
    E3(0x32000000, TEXT_CMPOBE,  M_CSRC1, M_CSRC2, M_CDISP),
    E3(0x33000000, TEXT_CMPOBGE, M_CSRC1, M_CSRC2, M_CDISP),
    E3(0x34000000, TEXT_CMPOBL,  M_CSRC1, M_CSRC2, M_CDISP),
    E3(0x35000000, TEXT_CMPOBNE, M_CSRC1, M_CSRC2, M_CDISP),
    E3(0x36000000, TEXT_CMPOBLE, M_CSRC1, M_CSRC2, M_CDISP),
    E3(0x37000000, TEXT_BBS,     M_CSRC1, M_CSRC2, M_CDISP),
    E3(0x38000000, TEXT_CMPIBNO, M_CSRC1, M_CSRC2, M_CDISP),
    E3(0x39000000, TEXT_CMPIBG,  M_CSRC1, M_CSRC2, M_CDISP),
    E3(0x3A000000, TEXT_CMPIBE,  M_CSRC1, M_CSRC2, M_CDISP),
    E3(0x3B000000, TEXT_CMPIBGE, M_CSRC1, M_CSRC2, M_CDISP),
    E3(0x3C000000, TEXT_CMPIBL,  M_CSRC1, M_CSRC2, M_CDISP),
    E3(0x3D000000, TEXT_CMPIBNE, M_CSRC1, M_CSRC2, M_CDISP),
    E3(0x3E000000, TEXT_CMPIBLE, M_CSRC1, M_CSRC2, M_CDISP),
    E3(0x3F000000, TEXT_CMPIBO,  M_CSRC1, M_CSRC2, M_CDISP),
    // CTRL format: branch, call and fault
    E1(0x08000000, TEXT_B,       M_TDISP),
    E1(0x09000000, TEXT_CALL,    M_TDISP),
    E0(0x0A000000, TEXT_RET),
    E1(0x0B000000, TEXT_BAL,     M_TDISP),
    E1(0x10000000, TEXT_BNO,     M_TDISP),
    E1(0x11000000, TEXT_BG,      M_TDISP),
    E1(0x12000000, TEXT_BE,      M_TDISP),
    E1(0x13000000, TEXT_BGE,     M_TDISP),
    E1(0x14000000, TEXT_BL,      M_TDISP),
    E1(0x15000000, TEXT_BNE,     M_TDISP),
    E1(0x16000000, TEXT_BLE,     M_TDISP),
    E1(0x17000000, TEXT_BO,      M_TDISP),
    E0(0x18000000, TEXT_FAULTNO),
    E0(0x19000000, TEXT_FAULTG),
    E0(0x1A000000, TEXT_FAULTE),
    E0(0x1B000000, TEXT_FAULTGE),
    E0(0x1C000000, TEXT_FAULTL),
    E0(0x1D000000, TEXT_FAULTNE),
    E0(0x1E000000, TEXT_FAULTLE),
    E0(0x1F000000, TEXT_FAULTO),
    // MEM format: load, store and address
    E2(0x80000000, TEXT_LDOB,    M_MEM, M_MDST),
    E2(0x82000000, TEXT_STOB,    M_MDST, M_MEM),
    E1(0x84000000, TEXT_BX,      M_MEM),
    E2(0x85000000, TEXT_BALX,    M_MEM, M_MDST),
    E1(0x86000000, TEXT_CALLX,   M_MEM),
    E2(0x88000000, TEXT_LDOS,    M_MEM, M_MDST),
    E2(0x8A000000, TEXT_STOS,    M_MDST, M_MEM),
    E2(0x8C000000, TEXT_LDA,     M_MEM, M_MDST),
    E2(0x90000000, TEXT_LD,      M_MEM, M_MDST),
    E2(0x92000000, TEXT_ST,      M_MDST, M_MEM),
    E2(0x98000000, TEXT_LDL,     M_MEM, M_MDSTL),
    E2(0x9A000000, TEXT_STL,     M_MDSTL, M_MEM),
    E2(0xA0000000, TEXT_LDT,     M_MEM, M_MDSTQ),
    E2(0xA2000000, TEXT_STT,     M_MDSTQ, M_MEM),
    E2(0xB0000000, TEXT_LDQ,     M_MEM, M_MDSTQ),
    E2(0xB2000000, TEXT_STQ,     M_MDSTQ, M_MEM),
    E2(0xC0000000, TEXT_LDIB,    M_MEM, M_MDST),
    E2(0xC2000000, TEXT_STIB,    M_MDST, M_MEM),
    E2(0xC8000000, TEXT_LDIS,    M_MEM, M_MDST),
    E2(0xCA000000, TEXT_STIS,    M_MDST, M_MEM),
    // 80960KB floating-point and decimal extensions
    F3(0x64000100, TEXT_DADDC,   M_SRC1, M_SRC2, M_DST),
    F3(0x64000180, TEXT_DSUBC,   M_SRC1, M_SRC2, M_DST),
    F2(0x64001200, TEXT_DMOVT,   M_SRC1, M_DST),
    F3(0x67000300, TEXT_SCALERL, M_SRC1, M_FSRC2L, M_FDSTL),
    F3(0x67000380, TEXT_SCALER,  M_SRC1, M_FSRC2, M_FDST),
    F2(0x67001200, TEXT_CVTIR,   M_SRC1, M_FDST),
    F2(0x67001280, TEXT_CVTILR,  M_SRC1, M_FDST),
    F3(0x68000000, TEXT_ATANR,   M_FSRC1, M_FSRC2, M_FDST),
    F3(0x68000080, TEXT_LOGEPR,  M_FSRC1, M_FSRC2, M_FDST),
    F3(0x68000100, TEXT_LOGR,    M_FSRC1, M_FSRC2, M_FDST),
    F3(0x68000180, TEXT_REMR,    M_FSRC1, M_FSRC2, M_FDST),
    F2(0x68001400, TEXT_SQRTR,   M_FSRC1, M_FDST),
    F2(0x68001480, TEXT_EXPR,    M_FSRC1, M_FDST),
    F2(0x68001500, TEXT_LOGBNR,  M_FSRC1, M_FDST),
    F2(0x68001580, TEXT_ROUNDR,  M_FSRC1, M_FDST),
    F2(0x68001600, TEXT_SINR,    M_FSRC1, M_FDST),
    F2(0x68001680, TEXT_COSR,    M_FSRC1, M_FDST),
    F2(0x68001700, TEXT_TANR,    M_FSRC1, M_FDST),
    F2(0x68002200, TEXT_CMPOR,   M_FSRC1, M_FSRC2),
    F2(0x68002280, TEXT_CMPR,    M_FSRC1, M_FSRC2),
    F1(0x68003780, TEXT_CLASSR,  M_FSRC1),
    F3(0x69000000, TEXT_ATANRL,  M_FSRC1L, M_FSRC2L, M_FDSTL),
    F3(0x69000080, TEXT_LOGEPRL, M_FSRC1L, M_FSRC2L, M_FDSTL),
    F3(0x69000100, TEXT_LOGRL,   M_FSRC1L, M_FSRC2L, M_FDSTL),
    F3(0x69000180, TEXT_REMRL,   M_FSRC1L, M_FSRC2L, M_FDSTL),
    F2(0x69001400, TEXT_SQRTRL,  M_FSRC1L, M_FDSTL),
    F2(0x69001480, TEXT_EXPRL,   M_FSRC1L, M_FDSTL),
    F2(0x69001500, TEXT_LOGBNRL, M_FSRC1L, M_FDSTL),
    F2(0x69001580, TEXT_ROUNDRL, M_FSRC1L, M_FDSTL),
    F2(0x69001600, TEXT_SINRL,   M_FSRC1L, M_FDSTL),
    F2(0x69001680, TEXT_COSRL,   M_FSRC1L, M_FDSTL),
    F2(0x69001700, TEXT_TANRL,   M_FSRC1L, M_FDSTL),
    F2(0x69002200, TEXT_CMPORL,  M_FSRC1L, M_FSRC2L),
    F2(0x69002280, TEXT_CMPRL,   M_FSRC1L, M_FSRC2L),
    F1(0x69003780, TEXT_CLASSRL, M_FSRC1L),
    F2(0x6C001000, TEXT_CVTRI,   M_FSRC1, M_DST),
    F2(0x6C001080, TEXT_CVTRIL,  M_FSRC1, M_DSTL),
    F2(0x6C001100, TEXT_CVTZRI,  M_FSRC1, M_DST),
    F2(0x6C001180, TEXT_CVTZRIL, M_FSRC1, M_DSTL),
    F2(0x6C001480, TEXT_MOVR,    M_FSRC1, M_FDST),
    F2(0x6D001480, TEXT_MOVRL,   M_FSRC1L, M_FDSTL),
    F3(0x6E000100, TEXT_CPYSRE,  M_FSRC1Q, M_FSRC2Q, M_FDSTQ),
    F3(0x6E000180, TEXT_CPYRSRE, M_FSRC1Q, M_FSRC2Q, M_FDSTQ),
    F2(0x6E001080, TEXT_MOVRE,   M_FSRC1Q, M_FDSTQ),
    F3(0x78000580, TEXT_DIVR,    M_FSRC1, M_FSRC2, M_FDST),
    F3(0x78000600, TEXT_MULR,    M_FSRC1, M_FSRC2, M_FDST),
    F3(0x78000680, TEXT_SUBR,    M_FSRC1, M_FSRC2, M_FDST),
    F3(0x78000780, TEXT_ADDR,    M_FSRC1, M_FSRC2, M_FDST),
    F3(0x79000580, TEXT_DIVRL,   M_FSRC1L, M_FSRC2L, M_FDSTL),
    F3(0x79000600, TEXT_MULRL,   M_FSRC1L, M_FSRC2L, M_FDSTL),
    F3(0x79000680, TEXT_SUBRL,   M_FSRC1L, M_FSRC2L, M_FDSTL),
    F3(0x79000780, TEXT_ADDRL,   M_FSRC1L, M_FSRC2L, M_FDSTL),
};

static constexpr uint8_t INDEX_I960[] PROGMEM = {
     35,  // TEXT_ADDC
     16,  // TEXT_ADDI
     15,  // TEXT_ADDO
    179,  // TEXT_ADDR
    183,  // TEXT_ADDRL
     13,  // TEXT_ALTERBIT
      1,  // TEXT_AND
      2,  // TEXT_ANDNOT
     45,  // TEXT_ATADD
    139,  // TEXT_ATANR
    153,  // TEXT_ATANRL
     44,  // TEXT_ATMOD
     92,  // TEXT_B
     95,  // TEXT_BAL
    115,  // TEXT_BALX
     76,  // TEXT_BBC
     83,  // TEXT_BBS
     98,  // TEXT_BE
     97,  // TEXT_BG
     99,  // TEXT_BGE
    100,  // TEXT_BL
    102,  // TEXT_BLE
    101,  // TEXT_BNE
     96,  // TEXT_BNO
    103,  // TEXT_BO
    114,  // TEXT_BX
     93,  // TEXT_CALL
     54,  // TEXT_CALLS
    116,  // TEXT_CALLX
     34,  // TEXT_CHKBIT
    152,  // TEXT_CLASSR
    166,  // TEXT_CLASSRL
     10,  // TEXT_CLRBIT
     28,  // TEXT_CMPDECI
     27,  // TEXT_CMPDECO
     30,  // TEXT_CMPI
     86,  // TEXT_CMPIBE
     85,  // TEXT_CMPIBG
     87,  // TEXT_CMPIBGE
     88,  // TEXT_CMPIBL
     90,  // TEXT_CMPIBLE
     89,  // TEXT_CMPIBNE
     84,  // TEXT_CMPIBNO
     91,  // TEXT_CMPIBO
     26,  // TEXT_CMPINCI
     25,  // TEXT_CMPINCO
     29,  // TEXT_CMPO
     78,  // TEXT_CMPOBE
     77,  // TEXT_CMPOBG
     79,  // TEXT_CMPOBGE
     80,  // TEXT_CMPOBL
     82,  // TEXT_CMPOBLE
     81,  // TEXT_CMPOBNE
    150,  // TEXT_CMPOR
    164,  // TEXT_CMPORL
    151,  // TEXT_CMPR
    165,  // TEXT_CMPRL
     32,  // TEXT_CONCMPI
     31,  // TEXT_CONCMPO
    148,  // TEXT_COSR
    162,  // TEXT_COSRL
    174,  // TEXT_CPYRSRE
    173,  // TEXT_CPYSRE
    138,  // TEXT_CVTILR
    137,  // TEXT_CVTIR
    167,  // TEXT_CVTRI
    168,  // TEXT_CVTRIL
    169,  // TEXT_CVTZRI
    170,  // TEXT_CVTZRIL
    132,  // TEXT_DADDC
     67,  // TEXT_DIVI
     63,  // TEXT_DIVO
    176,  // TEXT_DIVR
    180,  // TEXT_DIVRL
    134,  // TEXT_DMOVT
    133,  // TEXT_DSUBC
     60,  // TEXT_EDIV
     59,  // TEXT_EMUL
    144,  // TEXT_EXPR
    158,  // TEXT_EXPRL
     51,  // TEXT_EXTRACT
    106,  // TEXT_FAULTE
    105,  // TEXT_FAULTG
    107,  // TEXT_FAULTGE
    108,  // TEXT_FAULTL
    110,  // TEXT_FAULTLE
    109,  // TEXT_FAULTNE
    104,  // TEXT_FAULTNO
    111,  // TEXT_FAULTO
     57,  // TEXT_FLUSHREG
     56,  // TEXT_FMARK
    120,  // TEXT_LD
    119,  // TEXT_LDA
    128,  // TEXT_LDIB
    130,  // TEXT_LDIS
    122,  // TEXT_LDL
    112,  // TEXT_LDOB
    117,  // TEXT_LDOS
    126,  // TEXT_LDQ
    124,  // TEXT_LDT
    145,  // TEXT_LOGBNR
    159,  // TEXT_LOGBNRL
    140,  // TEXT_LOGEPR
    154,  // TEXT_LOGEPRL
    141,  // TEXT_LOGR
    155,  // TEXT_LOGRL
     55,  // TEXT_MARK
     47,  // TEXT_MODAC
     66,  // TEXT_MODI
     50,  // TEXT_MODIFY
     53,  // TEXT_MODPC
     52,  // TEXT_MODTC
     37,  // TEXT_MOV
     38,  // TEXT_MOVL
     40,  // TEXT_MOVQ
    171,  // TEXT_MOVR
    175,  // TEXT_MOVRE
    172,  // TEXT_MOVRL
     39,  // TEXT_MOVT
     64,  // TEXT_MULI
     61,  // TEXT_MULO
    177,  // TEXT_MULR
    181,  // TEXT_MULRL
     12,  // TEXT_NAND
      7,  // TEXT_NOR
     14,  // TEXT_NOT
      4,  // TEXT_NOTAND
      0,  // TEXT_NOTBIT
     11,  // TEXT_NOTOR
      6,  // TEXT_OR
      9,  // TEXT_ORNOT
     65,  // TEXT_REMI
     62,  // TEXT_REMO
    142,  // TEXT_REMR
    156,  // TEXT_REMRL
     94,  // TEXT_RET
     23,  // TEXT_ROTATE
    146,  // TEXT_ROUNDR
    160,  // TEXT_ROUNDRL
    136,  // TEXT_SCALER
    135,  // TEXT_SCALERL
     49,  // TEXT_SCANBIT
     33,  // TEXT_SCANBYTE
      3,  // TEXT_SETBIT
     24,  // TEXT_SHLI
     22,  // TEXT_SHLO
     20,  // TEXT_SHRDI
     21,  // TEXT_SHRI
     19,  // TEXT_SHRO
    147,  // TEXT_SINR
    161,  // TEXT_SINRL
     48,  // TEXT_SPANBIT
    143,  // TEXT_SQRTR
    157,  // TEXT_SQRTRL
    121,  // TEXT_ST
    129,  // TEXT_STIB
    131,  // TEXT_STIS
    123,  // TEXT_STL
    113,  // TEXT_STOB
    118,  // TEXT_STOS
    127,  // TEXT_STQ
    125,  // TEXT_STT
     36,  // TEXT_SUBC
     18,  // TEXT_SUBI
     17,  // TEXT_SUBO
    178,  // TEXT_SUBR
    182,  // TEXT_SUBRL
     58,  // TEXT_SYNCF
     46,  // TEXT_SYNLD
     41,  // TEXT_SYNMOV
     42,  // TEXT_SYNMOVL
     43,  // TEXT_SYNMOVQ
    149,  // TEXT_TANR
    163,  // TEXT_TANRL
     70,  // TEXT_TESTE
     69,  // TEXT_TESTG
     71,  // TEXT_TESTGE
     72,  // TEXT_TESTL
     74,  // TEXT_TESTLE
     73,  // TEXT_TESTNE
     68,  // TEXT_TESTNO
     75,  // TEXT_TESTO
      8,  // TEXT_XNOR
      5,  // TEXT_XOR
};

// clang-format on

using EntryPage = entry::TableBase<Entry>;

constexpr EntryPage I960_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_I960), ARRAY_RANGE(INDEX_I960)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {I80960, TEXT_CPU_80960, ARRAY_RANGE(I960_PAGES)},
};

namespace {

const Cpu *cpu(CpuType cpuType) {
    return Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
}

// A register pair must name an even register and a quad one which is a
// multiple of four; the manual leaves an unaligned operand undefined, so an
// encoding which uses one is turned down rather than disassembled into an
// instruction which would not assemble back.
bool registersAligned(const DisInsn &insn, const Entry::Flags &flags) {
    const auto opc = insn.opCode();
    for (uint_fast8_t pos = 0; pos < MAX_OPERANDS; pos++) {
        const auto mode = flags.mode(pos);
        const auto regs = operandRegs(mode);
        if (regs == 1)
            continue;
        uint8_t regno;
        switch (mode) {
        case M_SRC1L:
        case M_SRC1Q:
        case M_FSRC1L:
        case M_FSRC1Q:
            // A set mode bit makes the field a five-bit literal, or on a
            // floating-point operand a floating-point register or a real
            // literal.  None of those is a register pair, so the alignment
            // rule does not reach them.
            if (opc & BIT_M1)
                continue;
            regno = opc & 0x1F;
            break;
        case M_SRC2L:
        case M_FSRC2L:
        case M_FSRC2Q:
            if (opc & BIT_M2)
                continue;
            regno = (opc >> 14) & 0x1F;
            break;
        case M_FDSTL:
        case M_FDSTQ:
            if (opc & BIT_M3)
                continue;
            regno = (opc >> 19) & 0x1F;
            break;
        default:
            regno = (opc >> 19) & 0x1F;
            break;
        }
        if (regno % regs)
            return false;
    }
    return true;
}

bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *) {
    const auto flags = entry->readFlags();
    if (!insn.allows(flags))
        return false;
    if (!registersAligned(insn, flags))
        return false;
    // The table carries a set mode bit for every field the instruction leaves
    // unused.  A field which is used has its bit either in the operand mask or
    // required to be zero, so clearing all three reaches exactly the unused
    // ones.
    auto opCode = entry->readOpCode();
    if (!insn.hasModeBits())
        opCode &= ~MODE_BITS;
    return (insn.opCode() & ~flags.mask()) == opCode;
}

}  // namespace

Error searchOpCode(CpuType cpuType, ModeBits modeBits, bool fpu, DisInsn &insn, StrBuffer &out) {
    insn.setModeBits(modeBits);
    insn.setFpu(fpu);
    cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    return insn.getError();
}

const /*PROGMEM*/ char *TableI960::listCpu_P() const {
    return TEXT_I960_LIST;
}

const /*PROGMEM*/ char *TableI960::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableI960::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    const auto *t = Cpu::search(name, ARRAY_RANGE(CPU_TABLE));
    if (t == nullptr)
        return UNSUPPORTED_CPU;
    cpuType = t->readCpuType();
    return OK;
}

const TableI960 TABLE;

}  // namespace i960
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

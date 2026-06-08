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

#include "table_h8300.h"
#include "entry_h8300.h"
#include "entry_table.h"
#include "text_h8300.h"

using namespace libasm::text::h8300;

namespace libasm {
namespace h8300 {

#define E2(_opc, _name, _isz, _osz, _src, _dst, _spos, _dpos) \
    {_opc, Entry::Flags::create(_isz, _osz, _src, _dst, _spos, _dpos), _name}
#define E1(_opc, _name, _isz, _osz, _src, _spos) \
    E2(_opc, _name, _isz, _osz, _src, M_NONE, _spos, POS____)
#define E0(_opc, _name) E1(_opc, _name, ISZ_NONE, SZ_NONE, M_NONE, POS____)

// clang-format off

constexpr Entry H8300_TABLE[] PROGMEM = {
    E2(0xF000, TEXT_MOV,   ISZ_DATA, SZ_BYTE, M_IMM8,  M_REG8,  POS__FF, POS_F__), // MOV.B #xx:8, Rd
    E2(0x7900, TEXT_MOV,   ISZ_DATA, SZ_WORD, M_IMM16, M_REG16, POS____, POS___F), // MOV.W #xx:16, Rd
    E2(0x8000, TEXT_ADD,   ISZ_DATA, SZ_BYTE, M_IMM8,  M_REG8,  POS__FF, POS_F__), // ADD.B #xx:8, Rd
    E2(0xA000, TEXT_CMP,   ISZ_DATA, SZ_BYTE, M_IMM8,  M_REG8,  POS__FF, POS_F__), // CMP.B #xx:8, Rd
    E2(0xC000, TEXT_OR,    ISZ_NONE, SZ_BYTE, M_IMM8,  M_REG8,  POS__FF, POS_F__), // OR.B  #xx:8, Rd
    E2(0xD000, TEXT_XOR,   ISZ_NONE, SZ_BYTE, M_IMM8,  M_REG8,  POS__FF, POS_F__), // XOR   #xx:8, Rd
    E2(0xE000, TEXT_AND,   ISZ_NONE, SZ_BYTE, M_IMM8,  M_REG8,  POS__FF, POS_F__), // AND   #xx:8, Rd
    E2(0x9000, TEXT_ADDX,  ISZ_NONE, SZ_BYTE, M_IMM8,  M_REG8,  POS__FF, POS_F__), // ADDX  #xx:8, Rd
    E2(0xB000, TEXT_SUBX,  ISZ_NONE, SZ_BYTE, M_IMM8,  M_REG8,  POS__FF, POS_F__), // SUBX  #xx:8, Rd
    E2(0x0C00, TEXT_MOV,   ISZ_DATA, SZ_BYTE, M_REG8,  M_REG8,  POS__F_, POS___F), // MOV.B Rs, Rd
    E2(0x0D00, TEXT_MOV,   ISZ_DATA, SZ_WORD, M_REG16, M_REG16, POS__F_, POS___F), // MOV.W Rs, Rd
    E2(0x2000, TEXT_MOV,   ISZ_DATA, SZ_BYTE, M_ABS8,  M_REG8,  POS__FF, POS_F__), // MOV.B @aa:8, Rd
    E2(0x3000, TEXT_MOV,   ISZ_DATA, SZ_BYTE, M_REG8,  M_ABS8,  POS_F__, POS__FF), // MOV.B Rs, @aa:8
    E2(0x6800, TEXT_MOV,   ISZ_DATA, SZ_BYTE, M_INDIR, M_REG8,  POS__7_, POS___F), // MOV.B @Rs, Rd
    E2(0x6880, TEXT_MOV,   ISZ_DATA, SZ_BYTE, M_REG8,  M_INDIR, POS___F, POS__7_), // MOV.B Rs, @Rd
    E2(0x6900, TEXT_MOV,   ISZ_DATA, SZ_WORD, M_INDIR, M_REG16, POS__7_, POS___F), // MOV.W @Rs, Rd
    E2(0x6980, TEXT_MOV,   ISZ_DATA, SZ_WORD, M_REG16, M_INDIR, POS___F, POS__7_), // MOV.W Rd, @Rd
    E2(0x6A00, TEXT_MOV,   ISZ_DATA, SZ_BYTE, M_ABS16, M_REG8,  POS____, POS___F), // MOV.B @aa:16, Rd
    E2(0x6A80, TEXT_MOV,   ISZ_DATA, SZ_BYTE, M_REG8,  M_ABS16, POS___F, POS____), // MOV,B Rs, @aa:16
    E2(0x6B00, TEXT_MOV,   ISZ_DATA, SZ_WORD, M_ABS16, M_REG16, POS____, POS___F), // MOV.W @aa:16, Rd
    E2(0x6B80, TEXT_MOV,   ISZ_DATA, SZ_WORD, M_REG16, M_ABS16, POS___F, POS____), // MOV.W Rs, @aa:16
    E2(0x6C00, TEXT_MOV,   ISZ_DATA, SZ_BYTE, M_PINC,  M_REG8,  POS__7_, POS___F), // MOV.B @Rs+, Rd
    E2(0x6C80, TEXT_MOV,   ISZ_DATA, SZ_BYTE, M_REG8,  M_PDEC,  POS___F, POS__7_), // MOV.B Rs, @-Rd
    E1(0x6D70, TEXT_POP,   ISZ_NONE, SZ_WORD, M_REG16, POS___F), // POP   Rd
    E1(0x6DF0, TEXT_PUSH,  ISZ_NONE, SZ_WORD, M_REG16, POS___F), // PUSH  Rs
    E2(0x6D00, TEXT_MOV,   ISZ_DATA, SZ_WORD, M_PINC,  M_REG16, POS__7_, POS___F), // MOV.W @Rs+, Rd
    E2(0x6D80, TEXT_MOV,   ISZ_DATA, SZ_WORD, M_REG16, M_PDEC,  POS___F, POS__7_), // MOV.W Rs, @-Rd
    E2(0x6E00, TEXT_MOV,   ISZ_DATA, SZ_BYTE, M_IDX16, M_REG8,  POS__7_, POS___F), // MOV.B #(d:16,Rs), Rd
    E2(0x6E80, TEXT_MOV,   ISZ_DATA, SZ_BYTE, M_REG8,  M_IDX16, POS___F, POS__7_), // MOV.B Rs, @(d:16,Rd)
    E2(0x6F00, TEXT_MOV,   ISZ_DATA, SZ_WORD, M_IDX16, M_REG16, POS__7_, POS___F), // MOV.W @(d:16,Rs), Rd
    E2(0x6F80, TEXT_MOV,   ISZ_DATA, SZ_WORD, M_REG16, M_IDX16, POS___F, POS__7_), // MOV.W Rs, @(d:16,Rd)
    E2(0x0800, TEXT_ADD,   ISZ_DATA, SZ_BYTE, M_REG8,  M_REG8,  POS__F_, POS___F), // ADD.B Rs, Rd
    E2(0x0900, TEXT_ADD,   ISZ_DATA, SZ_WORD, M_REG16, M_REG16, POS__F_, POS___F), // ADD.W Rs, Rd
    E2(0x1800, TEXT_SUB,   ISZ_DATA, SZ_BYTE, M_REG8,  M_REG8,  POS__F_, POS___F), // SUB.B Rs, Rd
    E2(0x1900, TEXT_SUB,   ISZ_DATA, SZ_WORD, M_REG16, M_REG16, POS__F_, POS___F), // SUB.W Rs, Rd
    E2(0x1C00, TEXT_CMP,   ISZ_DATA, SZ_BYTE, M_REG8,  M_REG8,  POS__F_, POS___F), // CMP.B Rs, Rd
    E2(0x1D00, TEXT_CMP,   ISZ_DATA, SZ_WORD, M_REG16, M_REG16, POS__F_, POS___F), // CMP.W Rs, Rd
    E2(0x1400, TEXT_OR,    ISZ_NONE, SZ_BYTE, M_REG8,  M_REG8,  POS__F_, POS___F), // OR    Rs, Rd
    E2(0x1500, TEXT_XOR,   ISZ_NONE, SZ_BYTE, M_REG8,  M_REG8,  POS__F_, POS___F), // XOR   Rs, Rd
    E2(0x1600, TEXT_AND,   ISZ_NONE, SZ_BYTE, M_REG8,  M_REG8,  POS__F_, POS___F), // AND   Rs, Rd
    E2(0x0E00, TEXT_ADDX,  ISZ_NONE, SZ_BYTE, M_REG8,  M_REG8,  POS__F_, POS___F), // ADDX  Rs, Rd
    E2(0x1E00, TEXT_SUBX,  ISZ_NONE, SZ_BYTE, M_REG8,  M_REG8,  POS__F_, POS___F), // SUBX  Rs, Rd
    E2(0x0B00, TEXT_ADDS,  ISZ_ADDR, SZ_WORD, M_VAL1,  M_ADREG, POS____, POS___F), // ADDS  #1, Rd/ERd
    E2(0x0B80, TEXT_ADDS,  ISZ_ADDR, SZ_WORD, M_VAL2,  M_ADREG, POS____, POS___F), // ADDS  #2, Rd/ERd
    E2(0x1B00, TEXT_SUBS,  ISZ_ADDR, SZ_WORD, M_VAL1,  M_ADREG, POS____, POS___F), // SUBS  #1, Rd/ERd
    E2(0x1B80, TEXT_SUBS,  ISZ_ADDR, SZ_WORD, M_VAL2,  M_ADREG, POS____, POS___F), // SUBS  #2, Rd/ERd
    E2(0x5000, TEXT_MULXU, ISZ_NONE, SZ_BYTE, M_REG8,  M_REG16, POS__F_, POS___F), // MULXU Rs, Rd
    E2(0x5100, TEXT_DIVXU, ISZ_NONE, SZ_BYTE, M_REG8,  M_REG16, POS__F_, POS___F), // DIVXU Rs, Rd
    E1(0x0A00, TEXT_INC,   ISZ_NONE, SZ_BYTE, M_REG8,  POS___F), // INC   Rd
    E1(0x1A00, TEXT_DEC,   ISZ_NONE, SZ_BYTE, M_REG8,  POS___F), // DEC   Rd
    E1(0x1700, TEXT_NOT,   ISZ_NONE, SZ_BYTE, M_REG8,  POS___F), // NOT   Rd
    E1(0x1780, TEXT_NEG,   ISZ_NONE, SZ_BYTE, M_REG8,  POS___F), // NEG   Rd
    E1(0x0F00, TEXT_DAA,   ISZ_NONE, SZ_BYTE, M_REG8,  POS___F), // DAA   Rd
    E1(0x1F00, TEXT_DAS,   ISZ_NONE, SZ_BYTE, M_REG8,  POS___F), // DAS   Rd
    E1(0x1000, TEXT_SHLL,  ISZ_NONE, SZ_BYTE, M_REG8,  POS___F), // SHLL  Rd
    E1(0x1080, TEXT_SHAL,  ISZ_NONE, SZ_BYTE, M_REG8,  POS___F), // SHAL  Rd
    E1(0x1100, TEXT_SHLR,  ISZ_NONE, SZ_BYTE, M_REG8,  POS___F), // SHLR  Rd
    E1(0x1180, TEXT_SHAR,  ISZ_NONE, SZ_BYTE, M_REG8,  POS___F), // SHAR  Rd
    E1(0x1200, TEXT_ROTXL, ISZ_NONE, SZ_BYTE, M_REG8,  POS___F), // ROTXL Rd
    E1(0x1280, TEXT_ROTL,  ISZ_NONE, SZ_BYTE, M_REG8,  POS___F), // ROTL  Rd
    E1(0x1300, TEXT_ROTXR, ISZ_NONE, SZ_BYTE, M_REG8,  POS___F), // ROTXR Rd
    E1(0x1380, TEXT_ROTR,  ISZ_NONE, SZ_BYTE, M_REG8,  POS___F), // ROTR  Rd
    E1(0x4000, TEXT_BRA,   ISZ_NONE, SZ_BYTE, M_REL8,  POS__FF), // BRA d:8
    E1(0x4000, TEXT_BT,    ISZ_NONE, SZ_BYTE, M_REL8,  POS__FF), // BT  d:8
    E1(0x4100, TEXT_BRN,   ISZ_NONE, SZ_BYTE, M_REL8,  POS__FF), // BRN d:8
    E1(0x4100, TEXT_BF,    ISZ_NONE, SZ_BYTE, M_REL8,  POS__FF), // BF  d:8
    E1(0x4200, TEXT_BHI,   ISZ_NONE, SZ_BYTE, M_REL8,  POS__FF), // BHI d:8
    E1(0x4300, TEXT_BLS,   ISZ_NONE, SZ_BYTE, M_REL8,  POS__FF), // BLS d:8
    E1(0x4400, TEXT_BCC,   ISZ_NONE, SZ_BYTE, M_REL8,  POS__FF), // BCC d:8
    E1(0x4400, TEXT_BHS,   ISZ_NONE, SZ_BYTE, M_REL8,  POS__FF), // BHS d:8
    E1(0x4500, TEXT_BCS,   ISZ_NONE, SZ_BYTE, M_REL8,  POS__FF), // BCS d:8
    E1(0x4500, TEXT_BLO,   ISZ_NONE, SZ_BYTE, M_REL8,  POS__FF), // BLO d:8
    E1(0x4600, TEXT_BNE,   ISZ_NONE, SZ_BYTE, M_REL8,  POS__FF), // BNE d:8
    E1(0x4700, TEXT_BEQ,   ISZ_NONE, SZ_BYTE, M_REL8,  POS__FF), // BEQ d:8
    E1(0x4800, TEXT_BVC,   ISZ_NONE, SZ_BYTE, M_REL8,  POS__FF), // BVC d:8
    E1(0x4900, TEXT_BVS,   ISZ_NONE, SZ_BYTE, M_REL8,  POS__FF), // BVS d:8
    E1(0x4A00, TEXT_BPL,   ISZ_NONE, SZ_BYTE, M_REL8,  POS__FF), // BPL d:8
    E1(0x4B00, TEXT_BMI,   ISZ_NONE, SZ_BYTE, M_REL8,  POS__FF), // BMI d:8
    E1(0x4C00, TEXT_BGE,   ISZ_NONE, SZ_BYTE, M_REL8,  POS__FF), // BGE d:8
    E1(0x4D00, TEXT_BLT,   ISZ_NONE, SZ_BYTE, M_REL8,  POS__FF), // BLT d:8
    E1(0x4E00, TEXT_BGT,   ISZ_NONE, SZ_BYTE, M_REL8,  POS__FF), // BGT d:8
    E1(0x4F00, TEXT_BLE,   ISZ_NONE, SZ_BYTE, M_REL8,  POS__FF), // BGE d:8
    E1(0x5500, TEXT_BSR,   ISZ_NONE, SZ_BYTE, M_REL8,  POS__FF), // BSR d:8
    E0(0x5470, TEXT_RTS),                                // RTS
    E0(0x5670, TEXT_RTE),                                // RTE
    E1(0x5900, TEXT_JMP,   ISZ_NONE, SZ_NONE, M_INDIR, POS__7_), // JMP @Rn
    E1(0x5A00, TEXT_JMP,   ISZ_NONE, SZ_WORD, M_ABS16, POS____), // JMP @aa:16
    E1(0x5B00, TEXT_JMP,   ISZ_NONE, SZ_NONE, M_MIND8, POS__FF), // JMP @@aa:8
    E1(0x5D00, TEXT_JSR,   ISZ_NONE, SZ_NONE, M_INDIR, POS__7_), // JSR @Rn
    E1(0x5E00, TEXT_JSR,   ISZ_NONE, SZ_WORD, M_ABS16, POS____), // JSR @aa:16
    E1(0x5F00, TEXT_JSR,   ISZ_NONE, SZ_NONE, M_MIND8, POS__FF), // JSR @@aa:8
    E2(0x6000, TEXT_BSET,  ISZ_NONE, SZ_BYTE, M_REG8,  M_REG8,  POS__F_, POS___F), // BSET  Rn, Rd
    E2(0x6100, TEXT_BNOT,  ISZ_NONE, SZ_BYTE, M_REG8,  M_REG8,  POS__F_, POS___F), // BNOT  Rn, Rd
    E2(0x6200, TEXT_BCLR,  ISZ_NONE, SZ_BYTE, M_REG8,  M_REG8,  POS__F_, POS___F), // BCLR  Rn, Rd
    E2(0x6700, TEXT_BST,   ISZ_NONE, SZ_BYTE, M_IMM3,  M_REG8,  POS__7_, POS___F), // BST   #xx:3, Rd
    E2(0x6780, TEXT_BIST,  ISZ_NONE, SZ_BYTE, M_IMM3,  M_REG8,  POS__7_, POS___F), // BIST  #xx:3, Rd
    E2(0x7000, TEXT_BSET,  ISZ_NONE, SZ_BYTE, M_IMM3,  M_REG8,  POS__7_, POS___F), // BSET  #xx:3, Rd
    E2(0x7100, TEXT_BNOT,  ISZ_NONE, SZ_BYTE, M_IMM3,  M_REG8,  POS__7_, POS___F), // BNOT  #xx:3, Rd
    E2(0x7200, TEXT_BCLR,  ISZ_NONE, SZ_BYTE, M_IMM3,  M_REG8,  POS__7_, POS___F), // BCLR  #xx:3, Rd
    E2(0x6300, TEXT_BTST,  ISZ_NONE, SZ_BYTE, M_REG8,  M_REG8,  POS__F_, POS___F), // BTST  Rn, Rd
    E2(0x7300, TEXT_BTST,  ISZ_NONE, SZ_BYTE, M_IMM3,  M_REG8,  POS__7_, POS___F), // BTST  #xx:3, Rd
    E2(0x7400, TEXT_BOR,   ISZ_NONE, SZ_BYTE, M_IMM3,  M_REG8,  POS__7_, POS___F), // BOR   #xx:3, Rd
    E2(0x7480, TEXT_BIOR,  ISZ_NONE, SZ_BYTE, M_IMM3,  M_REG8,  POS__7_, POS___F), // BIOR  #xx:3, Rd
    E2(0x7500, TEXT_BXOR,  ISZ_NONE, SZ_BYTE, M_IMM3,  M_REG8,  POS__7_, POS___F), // BXOR  #xx:3, Rd
    E2(0x7580, TEXT_BIXOR, ISZ_NONE, SZ_BYTE, M_IMM3,  M_REG8,  POS__7_, POS___F), // BIXOR #xx:3, Rd
    E2(0x7600, TEXT_BAND,  ISZ_NONE, SZ_BYTE, M_IMM3,  M_REG8,  POS__7_, POS___F), // BAND  #xx:3, Rd
    E2(0x7680, TEXT_BIAND, ISZ_NONE, SZ_BYTE, M_IMM3,  M_REG8,  POS__7_, POS___F), // BIAND #xx:3, Rd
    E2(0x7700, TEXT_BLD,   ISZ_NONE, SZ_BYTE, M_IMM3,  M_REG8,  POS__7_, POS___F), // BLD   #xx:3, Rd
    E2(0x7780, TEXT_BILD,  ISZ_NONE, SZ_BYTE, M_IMM3,  M_REG8,  POS__7_, POS___F), // BILD  #xx:3, Rd
    E2(0x6A40, TEXT_MOVFPE, ISZ_NONE,SZ_BYTE, M_ABS16, M_REG8,  POS____, POS___F), // MOVFPE @aa:16, Rd
    E2(0x6AC0, TEXT_MOVTPE, ISZ_NONE,SZ_BYTE, M_REG8,  M_ABS16, POS___F, POS____), // MOVVTPE Rs, @aa:16
    E0(0x0000, TEXT_NOP),       // NOP
    E0(0x0180, TEXT_SLEEP),     // SLEEP
    E2(0x0200, TEXT_STC,   ISZ_NONE, SZ_BYTE, M_CCR,   M_REG8,  POS____, POS___F), // STC  CCR,   Rd
    E2(0x0300, TEXT_LDC,   ISZ_NONE, SZ_BYTE, M_REG8,  M_CCR,   POS___F, POS____), // LDC  Rs,    CCR
    E2(0x0400, TEXT_ORC,   ISZ_NONE, SZ_BYTE, M_IMM8,  M_CCR,   POS__FF, POS____), // ORC  #xx:8, CCR
    E2(0x0500, TEXT_XORC,  ISZ_NONE, SZ_BYTE, M_IMM8,  M_CCR,   POS__FF, POS____), // XORC #xx:8, CCR
    E2(0x0600, TEXT_ANDC,  ISZ_NONE, SZ_BYTE, M_IMM8,  M_CCR,   POS__FF, POS____), // ANDC #xx:8, CCR
    E2(0x0700, TEXT_LDC,   ISZ_NONE, SZ_BYTE, M_IMM8,  M_CCR,   POS__FF, POS____), // LDC  #xx:8, CCR
};

constexpr uint8_t H8300_INDEX[] PROGMEM = {
      2,  // TEXT_ADD
     31,  // TEXT_ADD
     32,  // TEXT_ADD
     42,  // TEXT_ADDS
     43,  // TEXT_ADDS
      7,  // TEXT_ADDX
     40,  // TEXT_ADDX
      6,  // TEXT_AND
     39,  // TEXT_AND
    117,  // TEXT_ANDC
    105,  // TEXT_BAND
     68,  // TEXT_BCC
     93,  // TEXT_BCLR
     98,  // TEXT_BCLR
     70,  // TEXT_BCS
     73,  // TEXT_BEQ
     65,  // TEXT_BF
     78,  // TEXT_BGE
     80,  // TEXT_BGT
     66,  // TEXT_BHI
     69,  // TEXT_BHS
    106,  // TEXT_BIAND
    108,  // TEXT_BILD
    102,  // TEXT_BIOR
     95,  // TEXT_BIST
    104,  // TEXT_BIXOR
    107,  // TEXT_BLD
     81,  // TEXT_BLE
     71,  // TEXT_BLO
     67,  // TEXT_BLS
     79,  // TEXT_BLT
     77,  // TEXT_BMI
     72,  // TEXT_BNE
     92,  // TEXT_BNOT
     97,  // TEXT_BNOT
    101,  // TEXT_BOR
     76,  // TEXT_BPL
     62,  // TEXT_BRA
     64,  // TEXT_BRN
     91,  // TEXT_BSET
     96,  // TEXT_BSET
     82,  // TEXT_BSR
     94,  // TEXT_BST
     63,  // TEXT_BT
     99,  // TEXT_BTST
    100,  // TEXT_BTST
     74,  // TEXT_BVC
     75,  // TEXT_BVS
    103,  // TEXT_BXOR
      3,  // TEXT_CMP
     35,  // TEXT_CMP
     36,  // TEXT_CMP
     52,  // TEXT_DAA
     53,  // TEXT_DAS
     49,  // TEXT_DEC
     47,  // TEXT_DIVXU
     48,  // TEXT_INC
     85,  // TEXT_JMP
     86,  // TEXT_JMP
     87,  // TEXT_JMP
     88,  // TEXT_JSR
     89,  // TEXT_JSR
     90,  // TEXT_JSR
    114,  // TEXT_LDC
    118,  // TEXT_LDC
      0,  // TEXT_MOV
      1,  // TEXT_MOV
      9,  // TEXT_MOV
     10,  // TEXT_MOV
     11,  // TEXT_MOV
     12,  // TEXT_MOV
     13,  // TEXT_MOV
     14,  // TEXT_MOV
     15,  // TEXT_MOV
     16,  // TEXT_MOV
     17,  // TEXT_MOV
     18,  // TEXT_MOV
     19,  // TEXT_MOV
     20,  // TEXT_MOV
     21,  // TEXT_MOV
     22,  // TEXT_MOV
     25,  // TEXT_MOV
     26,  // TEXT_MOV
     27,  // TEXT_MOV
     28,  // TEXT_MOV
     29,  // TEXT_MOV
     30,  // TEXT_MOV
    109,  // TEXT_MOVFPE
    110,  // TEXT_MOVTPE
     46,  // TEXT_MULXU
     51,  // TEXT_NEG
    111,  // TEXT_NOP
     50,  // TEXT_NOT
      4,  // TEXT_OR
     37,  // TEXT_OR
    115,  // TEXT_ORC
     23,  // TEXT_POP
     24,  // TEXT_PUSH
     59,  // TEXT_ROTL
     61,  // TEXT_ROTR
     58,  // TEXT_ROTXL
     60,  // TEXT_ROTXR
     84,  // TEXT_RTE
     83,  // TEXT_RTS
     55,  // TEXT_SHAL
     57,  // TEXT_SHAR
     54,  // TEXT_SHLL
     56,  // TEXT_SHLR
    112,  // TEXT_SLEEP
    113,  // TEXT_STC
     33,  // TEXT_SUB
     34,  // TEXT_SUB
     44,  // TEXT_SUBS
     45,  // TEXT_SUBS
      8,  // TEXT_SUBX
     41,  // TEXT_SUBX
      5,  // TEXT_XOR
     38,  // TEXT_XOR
    116,  // TEXT_XORC
};

// 0x7B5C, CF_0000
constexpr Entry H8300_7B5C_TABLE[] PROGMEM = {
    E1(0x598F, TEXT_EEPMOV, ISZ_NONE, SZ_BYTE, M_NONE, POS____),  // EEPMOV / EEPMOV.B
};

constexpr uint8_t H8300_7B5C_INDEX[] PROGMEM = {
      0,  // TEXT_EEPMOV
};

// 0x7C00, M_INDIR, POS__7_
// 0x7E00, M_ABS8,  POS__FF
constexpr Entry H8300_7C00_TABLE[] = {
    E2(0x6300, TEXT_BTST,  ISZ_NONE, SZ_BYTE, M_REG8, M_INDIR, POS__F_, POS_PRX), // BTST  Rn,    @Rd/@aa:8
    E2(0x7300, TEXT_BTST,  ISZ_NONE, SZ_BYTE, M_IMM3, M_INDIR, POS__7_, POS_PRX), // BTST  #xx:3, @Rd/@aa:8
    E2(0x7400, TEXT_BOR,   ISZ_NONE, SZ_BYTE, M_IMM3, M_INDIR, POS__7_, POS_PRX), // BOR   #xx:3, @Rd/@aa:8
    E2(0x7480, TEXT_BIOR,  ISZ_NONE, SZ_BYTE, M_IMM3, M_INDIR, POS__7_, POS_PRX), // BIOR  #xx:3, @Rd/@aa:8
    E2(0x7500, TEXT_BXOR,  ISZ_NONE, SZ_BYTE, M_IMM3, M_INDIR, POS__7_, POS_PRX), // BXOR  #xx:3, @Rd/@aa:8
    E2(0x7580, TEXT_BIXOR, ISZ_NONE, SZ_BYTE, M_IMM3, M_INDIR, POS__7_, POS_PRX), // BIXOR #xx:3, @Rd/@aa:8
    E2(0x7600, TEXT_BAND,  ISZ_NONE, SZ_BYTE, M_IMM3, M_INDIR, POS__7_, POS_PRX), // BAND  #xx:3, @Rd/@aa:8
    E2(0x7680, TEXT_BIAND, ISZ_NONE, SZ_BYTE, M_IMM3, M_INDIR, POS__7_, POS_PRX), // BIAND #xx:3, @Rd/@aa:8
    E2(0x7700, TEXT_BLD,   ISZ_NONE, SZ_BYTE, M_IMM3, M_INDIR, POS__7_, POS_PRX), // BLD   #xx:3, @Rd/@aa:8
    E2(0x7780, TEXT_BILD,  ISZ_NONE, SZ_BYTE, M_IMM3, M_INDIR, POS__7_, POS_PRX), // BILD  #xx:3, @Rd/@aa:8
};

constexpr uint8_t H8300_7C00_INDEX[] PROGMEM = {
      6,  // TEXT_BAND
      7,  // TEXT_BIAND
      9,  // TEXT_BILD
      3,  // TEXT_BIOR
      5,  // TEXT_BIXOR
      8,  // TEXT_BLD
      2,  // TEXT_BOR
      0,  // TEXT_BTST
      1,  // TEXT_BTST
      4,  // TEXT_BXOR
};

// 0x7D00, M_INDIR, POS__7_
// 0x7F00, M_ABS8,  POS__FF
constexpr Entry H8300_7D00_TABLE[] = {
    E2(0x6000, TEXT_BSET, ISZ_NONE, SZ_BYTE, M_REG8, M_INDIR, POS__F_, POS_PRX), // BSET Rn,    @Rd/@aa:8
    E2(0x6100, TEXT_BNOT, ISZ_NONE, SZ_BYTE, M_REG8, M_INDIR, POS__F_, POS_PRX), // BNOT Rn,    @Rd/@aa:8
    E2(0x6200, TEXT_BCLR, ISZ_NONE, SZ_BYTE, M_REG8, M_INDIR, POS__F_, POS_PRX), // BCLR Rn,    @Rd/@aa:8
    E2(0x6700, TEXT_BST,  ISZ_NONE, SZ_BYTE, M_IMM3, M_INDIR, POS__7_, POS_PRX), // BST  #xx:3, @Rd/@aa:8
    E2(0x6780, TEXT_BIST, ISZ_NONE, SZ_BYTE, M_IMM3, M_INDIR, POS__7_, POS_PRX), // BIST #xx:3, @Rd/@aa:8
    E2(0x7000, TEXT_BSET, ISZ_NONE, SZ_BYTE, M_IMM3, M_INDIR, POS__7_, POS_PRX), // BSET #xx:3, @Rd/@aa:8
    E2(0x7100, TEXT_BNOT, ISZ_NONE, SZ_BYTE, M_IMM3, M_INDIR, POS__7_, POS_PRX), // BNOT #xx:3, @Rd/@aa:8
    E2(0x7200, TEXT_BCLR, ISZ_NONE, SZ_BYTE, M_IMM3, M_INDIR, POS__7_, POS_PRX), // BCLR #xx:3, @Rd/@aa:8
};

// 0x0100 prefix: MOV.L addressing modes (reuses MOV.W opcode patterns).
// POP.L / PUSH.L entries come first so they win over MOV.L @SP+/@-SP for
// the ER7 register slot.
constexpr Entry H8300H_0100_TABLE[] PROGMEM = {
    E1(0x6D70, TEXT_POP,  ISZ_DATA, SZ_LONG, M_REG32, POS___7),                   // POP.L  ERd
    E1(0x6DF0, TEXT_PUSH, ISZ_DATA, SZ_LONG, M_REG32, POS___7),                   // PUSH.L ERs
    E2(0x6900, TEXT_MOV,  ISZ_DATA, SZ_LONG, M_INDIR, M_REG32, POS__7_, POS___7), // MOV.L @ERs, ERd
    E2(0x6980, TEXT_MOV,  ISZ_DATA, SZ_LONG, M_REG32, M_INDIR, POS___7, POS__7_), // MOV.L ERs, @ERd
    E2(0x6D00, TEXT_MOV,  ISZ_DATA, SZ_LONG, M_PINC,  M_REG32, POS__7_, POS___7), // MOV.L @ERs+, ERd
    E2(0x6D80, TEXT_MOV,  ISZ_DATA, SZ_LONG, M_REG32, M_PDEC,  POS___7, POS__7_), // MOV.L ERs, @-ERd
    E2(0x6B00, TEXT_MOV,  ISZ_DATA, SZ_LONG, M_ABS16, M_REG32, POS____, POS___7), // MOV.L @aa:16, ERd
    E2(0x6B80, TEXT_MOV,  ISZ_DATA, SZ_LONG, M_REG32, M_ABS16, POS___7, POS____), // MOV.L ERs, @aa:16
    E2(0x6F00, TEXT_MOV,  ISZ_DATA, SZ_LONG, M_IDX16, M_REG32, POS__7_, POS___7), // MOV.L @(d:16,ERs), ERd
    E2(0x6F80, TEXT_MOV,  ISZ_DATA, SZ_LONG, M_REG32, M_IDX16, POS___7, POS__7_), // MOV.L ERs, @(d:16,ERd)
    E2(0x6B20, TEXT_MOV,  ISZ_DATA, SZ_LONG, M_ABS24, M_REG32, POS____, POS___7), // MOV.L @aa:24, ERd
    E2(0x6BA0, TEXT_MOV,  ISZ_DATA, SZ_LONG, M_REG32, M_ABS24, POS___7, POS____), // MOV.L ERs, @aa:24
};

constexpr uint8_t H8300H_0100_INDEX[] PROGMEM = {
      2,  // TEXT_MOV
      3,  // TEXT_MOV
      4,  // TEXT_MOV
      5,  // TEXT_MOV
      6,  // TEXT_MOV
      7,  // TEXT_MOV
      8,  // TEXT_MOV
      9,  // TEXT_MOV
     10,  // TEXT_MOV
     11,  // TEXT_MOV
      0,  // TEXT_POP
      1,  // TEXT_PUSH
};

// 0x01F0 prefix: OR.L / XOR.L / AND.L reg-reg.
constexpr Entry H8300H_01F0_TABLE[] PROGMEM = {
    E2(0x6400, TEXT_OR,  ISZ_DATA, SZ_LONG, M_REG32, M_REG32, POS__7_, POS___7), // OR.L  ERs, ERd
    E2(0x6500, TEXT_XOR, ISZ_DATA, SZ_LONG, M_REG32, M_REG32, POS__7_, POS___7), // XOR.L ERs, ERd
    E2(0x6600, TEXT_AND, ISZ_DATA, SZ_LONG, M_REG32, M_REG32, POS__7_, POS___7), // AND.L ERs, ERd
};

constexpr uint8_t H8300H_01F0_INDEX[] PROGMEM = {
      2,  // TEXT_AND
      0,  // TEXT_OR
      1,  // TEXT_XOR
};

// 0x0140 prefix: LDC/STC ccr with memory operand (reuses MOV.W byte patterns
// with the register operand rewritten to CCR).
constexpr Entry H8300H_0140_TABLE[] PROGMEM = {
    E2(0x6900, TEXT_LDC, ISZ_NONE, SZ_WORD, M_INDIR, M_CCR,  POS__7_, POS____), // LDC @ER, CCR
    E2(0x6980, TEXT_STC, ISZ_NONE, SZ_WORD, M_CCR,   M_INDIR,POS____, POS__7_), // STC CCR, @ER
    E2(0x6D00, TEXT_LDC, ISZ_NONE, SZ_WORD, M_PINC,  M_CCR,  POS__7_, POS____), // LDC @ER+, CCR
    E2(0x6D80, TEXT_STC, ISZ_NONE, SZ_WORD, M_CCR,   M_PDEC, POS____, POS__7_), // STC CCR, @-ER
    E2(0x6B00, TEXT_LDC, ISZ_NONE, SZ_WORD, M_ABS16, M_CCR,  POS____, POS____), // LDC @aa:16, CCR
    E2(0x6B80, TEXT_STC, ISZ_NONE, SZ_WORD, M_CCR,   M_ABS16,POS____, POS____), // STC CCR, @aa:16
    E2(0x6F00, TEXT_LDC, ISZ_NONE, SZ_WORD, M_IDX16, M_CCR,  POS__7_, POS____), // LDC @(d:16,ER), CCR
    E2(0x6F80, TEXT_STC, ISZ_NONE, SZ_WORD, M_CCR,   M_IDX16,POS____, POS__7_), // STC CCR, @(d:16,ER)
    E2(0x6B20, TEXT_LDC, ISZ_NONE, SZ_WORD, M_ABS24, M_CCR,  POS____, POS____), // LDC @aa:24, CCR
    E2(0x6BA0, TEXT_STC, ISZ_NONE, SZ_WORD, M_CCR,   M_ABS24,POS____, POS____), // STC CCR, @aa:24
};

constexpr uint8_t H8300H_0140_INDEX[] PROGMEM = {
      0,  // TEXT_LDC
      2,  // TEXT_LDC
      4,  // TEXT_LDC
      6,  // TEXT_LDC
      8,  // TEXT_LDC
      1,  // TEXT_STC
      3,  // TEXT_STC
      5,  // TEXT_STC
      7,  // TEXT_STC
      9,  // TEXT_STC
};

// 0x7800 prefix: ERn pointer for @(d:24,ERn) MOV.B / MOV.W variants.
// ERn is encoded at bits 6:4 of the prefix word; disp24 follows the opcode.
constexpr Entry H8300H_7800_TABLE[] PROGMEM = {
    E2(0x6A20, TEXT_MOV, ISZ_DATA, SZ_BYTE, M_IDX24, M_REG8,  POS_PRX, POS___F), // MOV.B @(d:24,ER), Rd
    E2(0x6AA0, TEXT_MOV, ISZ_DATA, SZ_BYTE, M_REG8,  M_IDX24, POS___F, POS_PRX), // MOV.B Rs, @(d:24,ER)
    E2(0x6B20, TEXT_MOV, ISZ_DATA, SZ_WORD, M_IDX24, M_REG16, POS_PRX, POS___F), // MOV.W @(d:24,ER), Rd
    E2(0x6BA0, TEXT_MOV, ISZ_DATA, SZ_WORD, M_REG16, M_IDX24, POS___F, POS_PRX), // MOV.W Rs, @(d:24,ER)
};

constexpr uint8_t H8300H_7800_INDEX[] PROGMEM = {
      0,  // TEXT_MOV
      1,  // TEXT_MOV
      2,  // TEXT_MOV
      3,  // TEXT_MOV
};

// 0x0140 super-prefix + 0x7800 normal prefix: LDC/STC ccr,@(d:24,ERn).
constexpr Entry H8300H_0140_7800_TABLE[] PROGMEM = {
    E2(0x6B20, TEXT_LDC, ISZ_NONE, SZ_WORD, M_IDX24, M_CCR,   POS_PRX, POS____), // LDC @(d:24,ER), CCR
    E2(0x6BA0, TEXT_STC, ISZ_NONE, SZ_WORD, M_CCR,   M_IDX24, POS____, POS_PRX), // STC CCR, @(d:24,ER)
};

constexpr uint8_t H8300H_0140_7800_INDEX[] PROGMEM = {
      0,  // TEXT_LDC
      1,  // TEXT_STC
};

// 0x0100 super-prefix + 0x7800 normal prefix: MOV.L @(d:24,ERs), ERd (load).
constexpr Entry H8300H_0100_7800_TABLE[] PROGMEM = {
    E2(0x6B20, TEXT_MOV, ISZ_DATA, SZ_LONG, M_IDX24, M_REG32, POS_PRX, POS___7), // MOV.L @(d:24,ERs), ERd
};

constexpr uint8_t H8300H_0100_7800_INDEX[] PROGMEM = {
      0,  // TEXT_MOV
};

// 0x0100 super-prefix + 0x7880 normal prefix: MOV.L ERs, @(d:24,ERd) (store).
constexpr Entry H8300H_0100_7880_TABLE[] PROGMEM = {
    E2(0x6BA0, TEXT_MOV, ISZ_DATA, SZ_LONG, M_REG32, M_IDX24, POS___7, POS_PRX), // MOV.L ERs, @(d:24,ERd)
};

constexpr uint8_t H8300H_0100_7880_INDEX[] PROGMEM = {
      0,  // TEXT_MOV
};

// 0x01C0 prefix: MULXS (signed multiply).
constexpr Entry H8300H_01C0_TABLE[] PROGMEM = {
    E2(0x5000, TEXT_MULXS, ISZ_DATA, SZ_BYTE, M_REG8,  M_REG16, POS__F_, POS___F), // MULXS.B Rs, Rd
    E2(0x5200, TEXT_MULXS, ISZ_DATA, SZ_WORD, M_REG16, M_REG32, POS__F_, POS___7), // MULXS.W Rs, ERd
};

constexpr uint8_t H8300H_01C0_INDEX[] PROGMEM = {
      0,  // TEXT_MULXS
      1,  // TEXT_MULXS
};

// 0x7BD4 prefix: EEPMOV.W (the 0x7B5C prefix is EEPMOV / EEPMOV.B).
constexpr Entry H8300H_7BD4_TABLE[] PROGMEM = {
    E1(0x598F, TEXT_EEPMOV, ISZ_DATA, SZ_WORD, M_NONE, POS____),  // EEPMOV.W
};

constexpr uint8_t H8300H_7BD4_INDEX[] PROGMEM = {
      0,  // TEXT_EEPMOV
};

// 0x01D0 prefix: DIVXS (signed divide).
constexpr Entry H8300H_01D0_TABLE[] PROGMEM = {
    E2(0x5100, TEXT_DIVXS, ISZ_DATA, SZ_BYTE, M_REG8,  M_REG16, POS__F_, POS___F), // DIVXS.B Rs, Rd
    E2(0x5300, TEXT_DIVXS, ISZ_DATA, SZ_WORD, M_REG16, M_REG32, POS__F_, POS___7), // DIVXS.W Rs, ERd
};

constexpr uint8_t H8300H_01D0_INDEX[] PROGMEM = {
      0,  // TEXT_DIVXS
      1,  // TEXT_DIVXS
};

constexpr uint8_t H8300_7D00_INDEX[] PROGMEM = {
      2,  // TEXT_BCLR
      7,  // TEXT_BCLR
      4,  // TEXT_BIST
      1,  // TEXT_BNOT
      6,  // TEXT_BNOT
      0,  // TEXT_BSET
      5,  // TEXT_BSET
      3,  // TEXT_BST
};

// H8/300H-only instructions.
constexpr Entry H8300H_TABLE[] PROGMEM = {
    E2(0x0F80, TEXT_MOV,   ISZ_DATA, SZ_LONG, M_REG32, M_REG32, POS__7_, POS___7), // MOV.L  ERs, ERd
    E2(0x0A80, TEXT_ADD,   ISZ_DATA, SZ_LONG, M_REG32, M_REG32, POS__7_, POS___7), // ADD.L  ERs, ERd
    E2(0x1A80, TEXT_SUB,   ISZ_DATA, SZ_LONG, M_REG32, M_REG32, POS__7_, POS___7), // SUB.L  ERs, ERd
    E2(0x1F80, TEXT_CMP,   ISZ_DATA, SZ_LONG, M_REG32, M_REG32, POS__7_, POS___7), // CMP.L  ERs, ERd
    E2(0x5200, TEXT_MULXU, ISZ_DATA, SZ_WORD, M_REG16, M_REG32, POS__F_, POS___7), // MULXU.W Rs, ERd
    E2(0x5300, TEXT_DIVXU, ISZ_DATA, SZ_WORD, M_REG16, M_REG32, POS__F_, POS___7), // DIVXU.W Rs, ERd
    E1(0x1710, TEXT_NOT,   ISZ_DATA, SZ_WORD, M_REG16, POS___F),                   // NOT.W  Rd
    E1(0x1730, TEXT_NOT,   ISZ_DATA, SZ_LONG, M_REG32, POS___7),                   // NOT.L  ERd
    E1(0x1790, TEXT_NEG,   ISZ_DATA, SZ_WORD, M_REG16, POS___F),                   // NEG.W  Rd
    E1(0x17B0, TEXT_NEG,   ISZ_DATA, SZ_LONG, M_REG32, POS___7),                   // NEG.L  ERd
    E1(0x1750, TEXT_EXTU,  ISZ_DATA, SZ_WORD, M_REG16, POS___F),                   // EXTU.W Rd
    E1(0x1770, TEXT_EXTU,  ISZ_DATA, SZ_LONG, M_REG32, POS___7),                   // EXTU.L ERd
    E1(0x17D0, TEXT_EXTS,  ISZ_DATA, SZ_WORD, M_REG16, POS___F),                   // EXTS.W Rd
    E1(0x17F0, TEXT_EXTS,  ISZ_DATA, SZ_LONG, M_REG32, POS___7),                   // EXTS.L ERd
    E1(0x1010, TEXT_SHLL,  ISZ_DATA, SZ_WORD, M_REG16, POS___F),                   // SHLL.W Rd
    E1(0x1030, TEXT_SHLL,  ISZ_DATA, SZ_LONG, M_REG32, POS___7),                   // SHLL.L ERd
    E1(0x1090, TEXT_SHAL,  ISZ_DATA, SZ_WORD, M_REG16, POS___F),                   // SHAL.W Rd
    E1(0x10B0, TEXT_SHAL,  ISZ_DATA, SZ_LONG, M_REG32, POS___7),                   // SHAL.L ERd
    E1(0x1110, TEXT_SHLR,  ISZ_DATA, SZ_WORD, M_REG16, POS___F),                   // SHLR.W Rd
    E1(0x1130, TEXT_SHLR,  ISZ_DATA, SZ_LONG, M_REG32, POS___7),                   // SHLR.L ERd
    E1(0x1190, TEXT_SHAR,  ISZ_DATA, SZ_WORD, M_REG16, POS___F),                   // SHAR.W Rd
    E1(0x11B0, TEXT_SHAR,  ISZ_DATA, SZ_LONG, M_REG32, POS___7),                   // SHAR.L ERd
    E1(0x1210, TEXT_ROTXL, ISZ_DATA, SZ_WORD, M_REG16, POS___F),                   // ROTXL.W Rd
    E1(0x1230, TEXT_ROTXL, ISZ_DATA, SZ_LONG, M_REG32, POS___7),                   // ROTXL.L ERd
    E1(0x1290, TEXT_ROTL,  ISZ_DATA, SZ_WORD, M_REG16, POS___F),                   // ROTL.W Rd
    E1(0x12B0, TEXT_ROTL,  ISZ_DATA, SZ_LONG, M_REG32, POS___7),                   // ROTL.L ERd
    E1(0x1310, TEXT_ROTXR, ISZ_DATA, SZ_WORD, M_REG16, POS___F),                   // ROTXR.W Rd
    E1(0x1330, TEXT_ROTXR, ISZ_DATA, SZ_LONG, M_REG32, POS___7),                   // ROTXR.L ERd
    E1(0x1390, TEXT_ROTR,  ISZ_DATA, SZ_WORD, M_REG16, POS___F),                   // ROTR.W Rd
    E1(0x13B0, TEXT_ROTR,  ISZ_DATA, SZ_LONG, M_REG32, POS___7),                   // ROTR.L ERd
    E2(0x0B50, TEXT_INC,   ISZ_DATA, SZ_WORD, M_VAL1, M_REG16, POS____, POS___F),  // INC.W  #1, Rd
    E2(0x0BD0, TEXT_INC,   ISZ_DATA, SZ_WORD, M_VAL2, M_REG16, POS____, POS___F),  // INC.W  #2, Rd
    E2(0x0B70, TEXT_INC,   ISZ_DATA, SZ_LONG, M_VAL1, M_REG32, POS____, POS___7),  // INC.L  #1, ERd
    E2(0x0BF0, TEXT_INC,   ISZ_DATA, SZ_LONG, M_VAL2, M_REG32, POS____, POS___7),  // INC.L  #2, ERd
    E2(0x1B50, TEXT_DEC,   ISZ_DATA, SZ_WORD, M_VAL1, M_REG16, POS____, POS___F),  // DEC.W  #1, Rd
    E2(0x1BD0, TEXT_DEC,   ISZ_DATA, SZ_WORD, M_VAL2, M_REG16, POS____, POS___F),  // DEC.W  #2, Rd
    E2(0x1B70, TEXT_DEC,   ISZ_DATA, SZ_LONG, M_VAL1, M_REG32, POS____, POS___7),  // DEC.L  #1, ERd
    E2(0x1BF0, TEXT_DEC,   ISZ_DATA, SZ_LONG, M_VAL2, M_REG32, POS____, POS___7),  // DEC.L  #2, ERd
    E2(0x7A00, TEXT_MOV,   ISZ_DATA, SZ_LONG, M_IMM32, M_REG32, POS____, POS___7), // MOV.L  #imm32, ERd
    E2(0x7A10, TEXT_ADD,   ISZ_DATA, SZ_LONG, M_IMM32, M_REG32, POS____, POS___7), // ADD.L  #imm32, ERd
    E2(0x7A20, TEXT_CMP,   ISZ_DATA, SZ_LONG, M_IMM32, M_REG32, POS____, POS___7), // CMP.L  #imm32, ERd
    E2(0x7A30, TEXT_SUB,   ISZ_DATA, SZ_LONG, M_IMM32, M_REG32, POS____, POS___7), // SUB.L  #imm32, ERd
    E2(0x7A40, TEXT_OR,    ISZ_DATA, SZ_LONG, M_IMM32, M_REG32, POS____, POS___7), // OR.L   #imm32, ERd
    E2(0x7A50, TEXT_XOR,   ISZ_DATA, SZ_LONG, M_IMM32, M_REG32, POS____, POS___7), // XOR.L  #imm32, ERd
    E2(0x7A60, TEXT_AND,   ISZ_DATA, SZ_LONG, M_IMM32, M_REG32, POS____, POS___7), // AND.L  #imm32, ERd
    E2(0x6400, TEXT_OR,    ISZ_DATA, SZ_WORD, M_REG16, M_REG16, POS__F_, POS___F), // OR.W   Rs, Rd
    E2(0x6500, TEXT_XOR,   ISZ_DATA, SZ_WORD, M_REG16, M_REG16, POS__F_, POS___F), // XOR.W  Rs, Rd
    E2(0x6600, TEXT_AND,   ISZ_DATA, SZ_WORD, M_REG16, M_REG16, POS__F_, POS___F), // AND.W  Rs, Rd
    E2(0x7940, TEXT_OR,    ISZ_DATA, SZ_WORD, M_IMM16, M_REG16, POS____, POS___F), // OR.W   #imm16, Rd
    E2(0x7950, TEXT_XOR,   ISZ_DATA, SZ_WORD, M_IMM16, M_REG16, POS____, POS___F), // XOR.W  #imm16, Rd
    E2(0x7960, TEXT_AND,   ISZ_DATA, SZ_WORD, M_IMM16, M_REG16, POS____, POS___F), // AND.W  #imm16, Rd
    E2(0x7910, TEXT_ADD,   ISZ_DATA, SZ_WORD, M_IMM16, M_REG16, POS____, POS___F), // ADD.W  #imm16, Rd
    E2(0x7920, TEXT_CMP,   ISZ_DATA, SZ_WORD, M_IMM16, M_REG16, POS____, POS___F), // CMP.W  #imm16, Rd
    E2(0x7930, TEXT_SUB,   ISZ_DATA, SZ_WORD, M_IMM16, M_REG16, POS____, POS___F), // SUB.W  #imm16, Rd
    E1(0x5800, TEXT_BRA,   ISZ_NONE, SZ_WORD, M_REL16, POS____),                   // BRA :16
    E1(0x5800, TEXT_BT,    ISZ_NONE, SZ_WORD, M_REL16, POS____),                   // BT  :16 (alias)
    E1(0x5810, TEXT_BRN,   ISZ_NONE, SZ_WORD, M_REL16, POS____),                   // BRN :16
    E1(0x5810, TEXT_BF,    ISZ_NONE, SZ_WORD, M_REL16, POS____),                   // BF  :16 (alias)
    E1(0x5820, TEXT_BHI,   ISZ_NONE, SZ_WORD, M_REL16, POS____),                   // BHI :16
    E1(0x5830, TEXT_BLS,   ISZ_NONE, SZ_WORD, M_REL16, POS____),                   // BLS :16
    E1(0x5840, TEXT_BCC,   ISZ_NONE, SZ_WORD, M_REL16, POS____),                   // BCC :16
    E1(0x5840, TEXT_BHS,   ISZ_NONE, SZ_WORD, M_REL16, POS____),                   // BHS :16 (alias)
    E1(0x5850, TEXT_BCS,   ISZ_NONE, SZ_WORD, M_REL16, POS____),                   // BCS :16
    E1(0x5850, TEXT_BLO,   ISZ_NONE, SZ_WORD, M_REL16, POS____),                   // BLO :16 (alias)
    E1(0x5860, TEXT_BNE,   ISZ_NONE, SZ_WORD, M_REL16, POS____),                   // BNE :16
    E1(0x5870, TEXT_BEQ,   ISZ_NONE, SZ_WORD, M_REL16, POS____),                   // BEQ :16
    E1(0x5880, TEXT_BVC,   ISZ_NONE, SZ_WORD, M_REL16, POS____),                   // BVC :16
    E1(0x5890, TEXT_BVS,   ISZ_NONE, SZ_WORD, M_REL16, POS____),                   // BVS :16
    E1(0x58A0, TEXT_BPL,   ISZ_NONE, SZ_WORD, M_REL16, POS____),                   // BPL :16
    E1(0x58B0, TEXT_BMI,   ISZ_NONE, SZ_WORD, M_REL16, POS____),                   // BMI :16
    E1(0x58C0, TEXT_BGE,   ISZ_NONE, SZ_WORD, M_REL16, POS____),                   // BGE :16
    E1(0x58D0, TEXT_BLT,   ISZ_NONE, SZ_WORD, M_REL16, POS____),                   // BLT :16
    E1(0x58E0, TEXT_BGT,   ISZ_NONE, SZ_WORD, M_REL16, POS____),                   // BGT :16
    E1(0x58F0, TEXT_BLE,   ISZ_NONE, SZ_WORD, M_REL16, POS____),                   // BLE :16
    E1(0x5C00, TEXT_BSR,   ISZ_NONE, SZ_WORD, M_REL16, POS____),                   // BSR :16
    E2(0x0B90, TEXT_ADDS,  ISZ_ADDR, SZ_WORD, M_VAL4, M_ADREG, POS____, POS___7),  // ADDS #4, ERd
    E2(0x1B90, TEXT_SUBS,  ISZ_ADDR, SZ_WORD, M_VAL4, M_ADREG, POS____, POS___7),  // SUBS #4, ERd
    E1(0x5700, TEXT_TRAPA, ISZ_NONE, SZ_NONE, M_IMM3, POS__7_),                    // TRAPA #vec (2-bit)
    E2(0x6A20, TEXT_MOV,   ISZ_DATA, SZ_BYTE, M_ABS24, M_REG8,  POS____, POS___F), // MOV.B @aa:24, Rd
    E2(0x6AA0, TEXT_MOV,   ISZ_DATA, SZ_BYTE, M_REG8,  M_ABS24, POS___F, POS____), // MOV.B Rs, @aa:24
    E2(0x6B20, TEXT_MOV,   ISZ_DATA, SZ_WORD, M_ABS24, M_REG16, POS____, POS___F), // MOV.W @aa:24, Rd
    E2(0x6BA0, TEXT_MOV,   ISZ_DATA, SZ_WORD, M_REG16, M_ABS24, POS___F, POS____), // MOV.W Rs, @aa:24
    E1(0x5A00, TEXT_JMP,   ISZ_NONE, SZ_NONE, M_ABS24, POS__FF),                   // JMP @aa:24
    E1(0x5E00, TEXT_JSR,   ISZ_NONE, SZ_NONE, M_ABS24, POS__FF),                   // JSR @aa:24
};

constexpr uint8_t H8300H_INDEX[] PROGMEM = {
      1,  // TEXT_ADD
     39,  // TEXT_ADD
     51,  // TEXT_ADD
     75,  // TEXT_ADDS
     44,  // TEXT_AND
     47,  // TEXT_AND
     50,  // TEXT_AND
     60,  // TEXT_BCC
     62,  // TEXT_BCS
     65,  // TEXT_BEQ
     57,  // TEXT_BF
     70,  // TEXT_BGE
     72,  // TEXT_BGT
     58,  // TEXT_BHI
     61,  // TEXT_BHS
     73,  // TEXT_BLE
     63,  // TEXT_BLO
     59,  // TEXT_BLS
     71,  // TEXT_BLT
     69,  // TEXT_BMI
     64,  // TEXT_BNE
     68,  // TEXT_BPL
     54,  // TEXT_BRA
     56,  // TEXT_BRN
     74,  // TEXT_BSR
     55,  // TEXT_BT
     66,  // TEXT_BVC
     67,  // TEXT_BVS
      3,  // TEXT_CMP
     40,  // TEXT_CMP
     52,  // TEXT_CMP
     34,  // TEXT_DEC
     35,  // TEXT_DEC
     36,  // TEXT_DEC
     37,  // TEXT_DEC
      5,  // TEXT_DIVXU
     12,  // TEXT_EXTS
     13,  // TEXT_EXTS
     10,  // TEXT_EXTU
     11,  // TEXT_EXTU
     30,  // TEXT_INC
     31,  // TEXT_INC
     32,  // TEXT_INC
     33,  // TEXT_INC
     82,  // TEXT_JMP
     83,  // TEXT_JSR
      0,  // TEXT_MOV
     38,  // TEXT_MOV
     78,  // TEXT_MOV
     79,  // TEXT_MOV
     80,  // TEXT_MOV
     81,  // TEXT_MOV
      4,  // TEXT_MULXU
      8,  // TEXT_NEG
      9,  // TEXT_NEG
      6,  // TEXT_NOT
      7,  // TEXT_NOT
     42,  // TEXT_OR
     45,  // TEXT_OR
     48,  // TEXT_OR
     24,  // TEXT_ROTL
     25,  // TEXT_ROTL
     28,  // TEXT_ROTR
     29,  // TEXT_ROTR
     22,  // TEXT_ROTXL
     23,  // TEXT_ROTXL
     26,  // TEXT_ROTXR
     27,  // TEXT_ROTXR
     16,  // TEXT_SHAL
     17,  // TEXT_SHAL
     20,  // TEXT_SHAR
     21,  // TEXT_SHAR
     14,  // TEXT_SHLL
     15,  // TEXT_SHLL
     18,  // TEXT_SHLR
     19,  // TEXT_SHLR
      2,  // TEXT_SUB
     41,  // TEXT_SUB
     53,  // TEXT_SUB
     76,  // TEXT_SUBS
     77,  // TEXT_TRAPA
     43,  // TEXT_XOR
     46,  // TEXT_XOR
     49,  // TEXT_XOR
};

// clang-format on

struct EntryPage : entry::PrefixTableBase<Entry> {
    constexpr EntryPage(Config::opcode_t prefix, SuperPrefix rprx, AddrMode dmode, OprPos dpos,
            const Entry *head_P, const Entry *tail_P, const uint8_t *index_P,
            const uint8_t *itail_P)
        : PrefixTableBase(prefix, head_P, tail_P, index_P, itail_P),
          _dmode_P(dmode),
          _attr_P(static_cast<uint8_t>((dpos << dpos_gp) | (rprx << rprx_gp))) {}

    Config::opcode_t prefixMask() const { return Entry::posMask(readDstPos()); }
    AddrMode readDstMode() const { return static_cast<AddrMode>(pgm_read_byte(&_dmode_P)); }
    OprPos readDstPos() const {
        return static_cast<OprPos>((pgm_read_byte(&_attr_P) >> dpos_gp) & dpos_gm);
    }
    SuperPrefix readSuperPrefix() const {
        return static_cast<SuperPrefix>((pgm_read_byte(&_attr_P) >> rprx_gp) & rprx_gm);
    }

private:
    const AddrMode _dmode_P;
    const uint8_t _attr_P;
    static constexpr int dpos_gp = 0;
    static constexpr uint_fast8_t dpos_gm = 0x7;
    static constexpr int rprx_gp = 4;
    static constexpr uint_fast8_t rprx_gm = 0x3;
};

// clang-format off
constexpr EntryPage H8300_PAGES[] PROGMEM = {
        {0x0000, SPRX_NONE, M_NONE,  POS____, ARRAY_RANGE(H8300_TABLE),      ARRAY_RANGE(H8300_INDEX)},
        {0x7C00, SPRX_NONE, M_INDIR, POS__7_, ARRAY_RANGE(H8300_7C00_TABLE), ARRAY_RANGE(H8300_7C00_INDEX)},
        {0x7E00, SPRX_NONE, M_ABS8,  POS__FF, ARRAY_RANGE(H8300_7C00_TABLE), ARRAY_RANGE(H8300_7C00_INDEX)},
        {0x7D00, SPRX_NONE, M_INDIR, POS__7_, ARRAY_RANGE(H8300_7D00_TABLE), ARRAY_RANGE(H8300_7D00_INDEX)},
        {0x7F00, SPRX_NONE, M_ABS8,  POS__FF, ARRAY_RANGE(H8300_7D00_TABLE), ARRAY_RANGE(H8300_7D00_INDEX)},
        {0x7B5C, SPRX_NONE, M_NONE,  POS____, ARRAY_RANGE(H8300_7B5C_TABLE), ARRAY_RANGE(H8300_7B5C_INDEX)},
};

constexpr EntryPage H8300H_PAGES[] PROGMEM = {
        {0x0000, SPRX_NONE, M_NONE,  POS____, ARRAY_RANGE(H8300_TABLE),       ARRAY_RANGE(H8300_INDEX)},
        {0x7C00, SPRX_NONE, M_INDIR, POS__7_, ARRAY_RANGE(H8300_7C00_TABLE),  ARRAY_RANGE(H8300_7C00_INDEX)},
        {0x7E00, SPRX_NONE, M_ABS8,  POS__FF, ARRAY_RANGE(H8300_7C00_TABLE),  ARRAY_RANGE(H8300_7C00_INDEX)},
        {0x7D00, SPRX_NONE, M_INDIR, POS__7_, ARRAY_RANGE(H8300_7D00_TABLE),  ARRAY_RANGE(H8300_7D00_INDEX)},
        {0x7F00, SPRX_NONE, M_ABS8,  POS__FF, ARRAY_RANGE(H8300_7D00_TABLE),  ARRAY_RANGE(H8300_7D00_INDEX)},
        {0x7B5C, SPRX_NONE, M_NONE,  POS____, ARRAY_RANGE(H8300_7B5C_TABLE),  ARRAY_RANGE(H8300_7B5C_INDEX)},
        {0x0000, SPRX_NONE, M_NONE,  POS____, ARRAY_RANGE(H8300H_TABLE),      ARRAY_RANGE(H8300H_INDEX)},
        {0x0000, SPRX_0100, M_NONE,  POS____, ARRAY_RANGE(H8300H_0100_TABLE), ARRAY_RANGE(H8300H_0100_INDEX)},
        {0x0000, SPRX_0140, M_NONE,  POS____, ARRAY_RANGE(H8300H_0140_TABLE), ARRAY_RANGE(H8300H_0140_INDEX)},
        {0x7800, SPRX_NONE, M_IDX24, POS__7_, ARRAY_RANGE(H8300H_7800_TABLE), ARRAY_RANGE(H8300H_7800_INDEX)},
        {0x7800, SPRX_0140, M_IDX24, POS__7_, ARRAY_RANGE(H8300H_0140_7800_TABLE), ARRAY_RANGE(H8300H_0140_7800_INDEX)},
        {0x7800, SPRX_0100, M_IDX24, POS__7_, ARRAY_RANGE(H8300H_0100_7800_TABLE), ARRAY_RANGE(H8300H_0100_7800_INDEX)},
        {0x7880, SPRX_0100, M_IDX24, POS__7_, ARRAY_RANGE(H8300H_0100_7880_TABLE), ARRAY_RANGE(H8300H_0100_7880_INDEX)},
        {0x01C0, SPRX_NONE, M_NONE,  POS____, ARRAY_RANGE(H8300H_01C0_TABLE), ARRAY_RANGE(H8300H_01C0_INDEX)},
        {0x01D0, SPRX_NONE, M_NONE,  POS____, ARRAY_RANGE(H8300H_01D0_TABLE), ARRAY_RANGE(H8300H_01D0_INDEX)},
        {0x01F0, SPRX_NONE, M_NONE,  POS____, ARRAY_RANGE(H8300H_01F0_TABLE), ARRAY_RANGE(H8300H_01F0_INDEX)},
        {0x7BD4, SPRX_NONE, M_NONE,  POS____, ARRAY_RANGE(H8300H_7BD4_TABLE), ARRAY_RANGE(H8300H_7BD4_INDEX)},
};
// clang-format on

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {H8300, TEXT_CPU_H8300, ARRAY_RANGE(H8300_PAGES)},
        {H8300H, TEXT_CPU_H8300H, ARRAY_RANGE(H8300H_PAGES)},
};

const Cpu *cpu(CpuType cpuType) {
    return Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
}

bool pageMatcher(DisInsn &insn, const EntryPage *page_P) {
    return insn.superPrefix == page_P->readSuperPrefix() &&
           (insn.prefix() & ~page_P->prefixMask()) == page_P->readPrefix();
}

SuperPrefix toSuperPrefix(CpuType cpuType, Config::opcode_t code) {
    if (cpuType == H8300)
        return SPRX_NONE;
    switch (code) {
    case 0x0100:
        return SPRX_0100;
    case 0x0140:
        return SPRX_0140;
    default:
        return SPRX_NONE;
    }
}

Config::opcode_t fromSuperPrefix(SuperPrefix rprx) {
    switch (rprx) {
    case SPRX_0100:
        return 0x0100;
    case SPRX_0140:
        return 0x0140;
    default:
        return 0;
    }
}

bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *) {
    const auto opc = insn.opCode();
    const auto flags = entry->readFlags();
    return (opc & ~flags.opCodeMask()) == entry->readOpCode();
}

void readName(DisInsn &insn, const Entry *entry, StrBuffer &out, const EntryPage *page_P) {
    Cpu::defaultReadName(insn, entry, out, page_P);
    insn.prefixMode = page_P->readDstMode();
    insn.prefixPos = page_P->readDstPos();
}

Error searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) {
    cpu(cpuType)->searchOpCode(insn, out, matchOpCode, pageMatcher, readName);
    if (insn.getError() == UNKNOWN_INSTRUCTION)
        insn.nameBuffer().reset();
    return insn.getError();
}

bool acceptMode(const Operand &op, AddrMode table, bool hasReg32) {
    if (op.mode == table)
        return true;
    if (op.mode == M_VAL1 || op.mode == M_VAL2 || op.mode == M_VAL4 || op.mode == M_IMM3 ||
            op.mode == M_IMM8)
        return table == M_IMM3 || table == M_IMM8 || table == M_IMM16 || table == M_IMM32;
    if (op.mode == M_IMM16)
        return table == M_IMM32;
    if (op.mode == M_ABS8)
        return table == M_ABS16 || table == M_ABS24;
    if (op.mode == M_ABS16)
        return table == M_ABS24;
    if (table == M_ADREG) {
        if (hasReg32)
            return op.mode == M_REG32 || op.mode == M_REG16;
        return op.mode == M_REG16;
    }
    return false;
}

bool acceptModes(AsmInsn &insn, const Entry *entry) {
    const auto flags = entry->readFlags();
    // The prefix page can override either operand, but only the one that
    // declares POS_PRX in its position field.
    const auto src = (flags.srcPos() == POS_PRX) ? insn.prefixMode : flags.src();
    const auto dst = (flags.dstPos() == POS_PRX) ? insn.prefixMode : flags.dst();
    const auto reg32 = insn.hasReg32();
    return acceptMode(insn.srcOp, src, reg32) && acceptMode(insn.dstOp, dst, reg32);
}

bool acceptModesSized(AsmInsn &insn, const Entry *entry) {
    const auto flags = entry->readFlags();
    if (insn.sizeSuffix != SZ_NONE) {
        // ISZ_ADDR entries take their size from the CpuType: SZ_WORD on
        // H8/300, SZ_LONG on H8/300H. Everything else compares to the
        // entry's static oprSize.
        const auto expected = (flags.insnSize() == ISZ_ADDR) ? (insn.hasReg32() ? SZ_LONG : SZ_WORD)
                                                             : flags.oprSize();
        if (insn.sizeSuffix != expected)
            return false;
    }
    return acceptModes(insn, entry);
}

void pageSetup(AsmInsn &insn, const EntryPage *page_P) {
    insn.setPrefix(page_P->readPrefix());
    insn.prefixMode = page_P->readDstMode();
    insn.prefixPos = page_P->readDstPos();
    insn.superPrefix = page_P->readSuperPrefix();
}

Error searchName(CpuType cpuType, AsmInsn &insn) {
    // Pass 1: require the user's size suffix to match the entry's oprSize so
    // multi-size mnemonics (EEPMOV / EEPMOV.W, MOV.B / MOV.W / MOV.L, etc.)
    // can each find their canonical entry. If that fails, fall back to the
    // size-ignoring search so a true size mismatch surfaces as ILLEGAL_SIZE
    // via the post-search check in encodeImpl.
    cpu(cpuType)->searchName(insn, acceptModesSized, pageSetup);
    if (insn.getError() == OK)
        return OK;
    insn.setOK();
    cpu(cpuType)->searchName(insn, acceptModes, pageSetup);
    return insn.getError();
}

bool prefixMatcher(uint16_t code, const EntryPage *page_P) {
    const auto prefix = page_P->readPrefix();
    return prefix && ((code & ~page_P->prefixMask()) == prefix);
}

bool isPrefix(CpuType cpuType, Config::opcode_t code) {
    // SuperPrefix codes (0x0140) are consumed separately in decodeImpl; don't
    // double-count them here.
    if (toSuperPrefix(cpuType, code) != SPRX_NONE)
        return false;
    return cpu(cpuType)->isPrefix(code, prefixMatcher);
}

const /*PROGMEM*/ char *TableH8300::listCpu_P() const {
    return TEXT_H8300_LIST;
}

const /*PROGMEM*/ char *TableH8300::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableH8300::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    auto p = name;
    const auto *t = Cpu::search(p, ARRAY_RANGE(CPU_TABLE));
    if (t) {
        name = p;
        cpuType = t->readCpuType();
        return OK;
    }
    return UNSUPPORTED_CPU;
}

const TableH8300 TABLE;

}  // namespace h8300
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

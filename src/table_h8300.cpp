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
    E2(0x0B00, TEXT_ADDS,  ISZ_NONE, SZ_WORD, M_VAL1,  M_ADREG, POS____, POS___F), // ADDS  #1, Rd
    E2(0x0B80, TEXT_ADDS,  ISZ_NONE, SZ_WORD, M_VAL2,  M_ADREG, POS____, POS___F), // ADDS  #2, Rd
    E2(0x1B00, TEXT_SUBS,  ISZ_NONE, SZ_WORD, M_VAL1,  M_ADREG, POS____, POS___F), // SUBS  #1, Rd
    E2(0x1B80, TEXT_SUBS,  ISZ_NONE, SZ_WORD, M_VAL2,  M_ADREG, POS____, POS___F), // SUBS  #2, Rd
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
    E0(0x598F, TEXT_EEPMOV), // EEPMOV
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

// clang-format on

struct EntryPage : entry::PrefixTableBase<Entry> {
    constexpr EntryPage(Config::opcode_t prefix, AddrMode dmode, OprPos dpos, const Entry *head_P,
            const Entry *tail_P, const uint8_t *index_P, const uint8_t *itail_P)
        : PrefixTableBase(prefix, head_P, tail_P, index_P, itail_P),
          _dmode_P(dmode),
          _dpos_P(dpos) {}

    Config::opcode_t prefixMask() const { return Entry::posMask(readDstPos()); }
    AddrMode readDstMode() const { return static_cast<AddrMode>(pgm_read_byte(&_dmode_P)); }
    OprPos readDstPos() const { return static_cast<OprPos>(pgm_read_byte(&_dpos_P)); }

private:
    const AddrMode _dmode_P;
    const OprPos _dpos_P;
};

// clang-format off
constexpr EntryPage H8300_PAGES[] PROGMEM = {
        {0x0000, M_NONE,  POS____, ARRAY_RANGE(H8300_TABLE),      ARRAY_RANGE(H8300_INDEX)},
        {0x7C00, M_INDIR, POS__7_, ARRAY_RANGE(H8300_7C00_TABLE), ARRAY_RANGE(H8300_7C00_INDEX)},
        {0x7E00, M_ABS8,  POS__FF, ARRAY_RANGE(H8300_7C00_TABLE), ARRAY_RANGE(H8300_7C00_INDEX)},
        {0x7D00, M_INDIR, POS__7_, ARRAY_RANGE(H8300_7D00_TABLE), ARRAY_RANGE(H8300_7D00_INDEX)},
        {0x7F00, M_ABS8,  POS__FF, ARRAY_RANGE(H8300_7D00_TABLE), ARRAY_RANGE(H8300_7D00_INDEX)},
        {0x7B5C, M_NONE,  POS____, ARRAY_RANGE(H8300_7B5C_TABLE), ARRAY_RANGE(H8300_7B5C_INDEX)},
};
// clang-format on

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {H8300, TEXT_CPU_H8300, ARRAY_RANGE(H8300_PAGES)},
};

const Cpu *cpu(CpuType cpuType) {
    return Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
}

bool pageMatcher(DisInsn &insn, const EntryPage *page_P) {
    return (insn.prefix() & ~page_P->prefixMask()) == page_P->readPrefix();
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

bool acceptMode(const Operand &op, AddrMode table) {
    if (op.mode == table)
        return true;
    if (op.mode == M_VAL1 || op.mode == M_VAL2 || op.mode == M_IMM3 || op.mode == M_IMM8)
        return table == M_IMM3 || table == M_IMM8 || table == M_IMM16;
    if (op.mode == M_ABS8)
        return table == M_ABS16;
    if (op.mode == M_REG16)
        return table == M_ADREG;
    return false;
}

bool acceptModes(AsmInsn &insn, const Entry *entry) {
    const auto flags = entry->readFlags();
    const auto dst = (insn.prefixMode != M_NONE) ? insn.prefixMode : flags.dst();
    return acceptMode(insn.srcOp, flags.src()) && acceptMode(insn.dstOp, dst);
}

void pageSetup(AsmInsn &insn, const EntryPage *page_P) {
    insn.setPrefix(page_P->readPrefix());
    insn.prefixMode = page_P->readDstMode();
    insn.prefixPos = page_P->readDstPos();
}

Error searchName(CpuType cpuType, AsmInsn &insn) {
    cpu(cpuType)->searchName(insn, acceptModes, pageSetup);
    return insn.getError();
}

bool prefixMatcher(uint16_t code, const EntryPage *page_P) {
    const auto prefix = page_P->readPrefix();
    return prefix && ((code & ~page_P->prefixMask()) == prefix);
}

bool isPrefix(CpuType cpuType, Config::opcode_t code) {
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

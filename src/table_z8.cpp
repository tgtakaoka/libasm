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

#include "config_z8.h"

#include "entry_z8.h"
#include "table_z8.h"
#include "text_z8.h"

#include <ctype.h>
#include <string.h>

namespace libasm {
namespace z8 {

#define P3(_opc, _name, _post, _dst, _src, _dstSrc, _ext) \
    { _opc, Entry::Flags::create(_dst, _src, _ext, _dstSrc, _post), _name }
#define P2(_opc, _name, _post, _dst, _src, _dstSrc) \
    P3(_opc, _name, _post, _dst, _src, _dstSrc, M_NONE)
#define P1(_opc, _name, _post, _dst) P2(_opc, _name, _post, _dst, M_NONE, DS_NO)
#define E2(_opc, _name, _dst, _src, _dstSrc) P2(_opc, _name, P0, _dst, _src, _dstSrc)
#define E1(_opc, _name, _dst) P1(_opc, _name, P0, _dst)
#define E0(_opc, _name) E1(_opc, _name, M_NONE)

// clang-format off
static constexpr Entry TABLE_COMMON[] PROGMEM = {
    E2(0x08, TEXT_LD,   M_r,   M_R,  DST_SRC),
    E2(0x09, TEXT_LD,   M_R,   M_r,  SRC_DST),
    E2(0x0C, TEXT_LD,   M_r,   M_IM, DST_SRC),
    E2(0xE4, TEXT_LD,   M_R,   M_R,  SRC_DST),
    E2(0xE5, TEXT_LD,   M_R,   M_IR, SRC_DST),
    E2(0xE6, TEXT_LD,   M_R,   M_IM, DST_SRC),
    E2(0xF5, TEXT_LD,   M_IR,  M_R,  SRC_DST),
    E2(0x0A, TEXT_DJNZ, M_r,   M_RA, DST_SRC),
    E1(0x8B, TEXT_JR,   M_RA),
    E2(0x0B, TEXT_JR,   M_cc,  M_RA, DST_SRC),
    E1(0x8D, TEXT_JP,   M_DA),
    E2(0x0D, TEXT_JP,   M_cc,  M_DA, DST_SRC),
    E1(0x30, TEXT_JP,   M_IRR),
    E2(0x02, TEXT_ADD,  M_r,   M_r,  DST_SRC),
    E2(0x03, TEXT_ADD,  M_r,   M_Ir, DST_SRC),
    E2(0x04, TEXT_ADD,  M_R,   M_R,  SRC_DST),
    E2(0x05, TEXT_ADD,  M_R,   M_IR, SRC_DST),
    E2(0x06, TEXT_ADD,  M_R,   M_IM, DST_SRC),
    E2(0x12, TEXT_ADC,  M_r,   M_r,  DST_SRC),
    E2(0x13, TEXT_ADC,  M_r,   M_Ir, DST_SRC),
    E2(0x14, TEXT_ADC,  M_R,   M_R,  SRC_DST),
    E2(0x15, TEXT_ADC,  M_R,   M_IR, SRC_DST),
    E2(0x16, TEXT_ADC,  M_R,   M_IM, DST_SRC),
    E2(0x22, TEXT_SUB,  M_r,   M_r,  DST_SRC),
    E2(0x23, TEXT_SUB,  M_r,   M_Ir, DST_SRC),
    E2(0x24, TEXT_SUB,  M_R,   M_R,  SRC_DST),
    E2(0x25, TEXT_SUB,  M_R,   M_IR, SRC_DST),
    E2(0x26, TEXT_SUB,  M_R,   M_IM, DST_SRC),
    E2(0x32, TEXT_SBC,  M_r,   M_r,  DST_SRC),
    E2(0x33, TEXT_SBC,  M_r,   M_Ir, DST_SRC),
    E2(0x34, TEXT_SBC,  M_R,   M_R,  SRC_DST),
    E2(0x35, TEXT_SBC,  M_R,   M_IR, SRC_DST),
    E2(0x36, TEXT_SBC,  M_R,   M_IM, DST_SRC),
    E2(0x42, TEXT_OR,   M_r,   M_r,  DST_SRC),
    E2(0x43, TEXT_OR,   M_r,   M_Ir, DST_SRC),
    E2(0x44, TEXT_OR,   M_R,   M_R,  SRC_DST),
    E2(0x45, TEXT_OR,   M_R,   M_IR, SRC_DST),
    E2(0x46, TEXT_OR,   M_R,   M_IM, DST_SRC),
    E2(0x52, TEXT_AND,  M_r,   M_r,  DST_SRC),
    E2(0x53, TEXT_AND,  M_r,   M_Ir, DST_SRC),
    E2(0x54, TEXT_AND,  M_R,   M_R,  SRC_DST),
    E2(0x55, TEXT_AND,  M_R,   M_IR, SRC_DST),
    E2(0x56, TEXT_AND,  M_R,   M_IM, DST_SRC),
    E2(0x62, TEXT_TCM,  M_r,   M_r,  DST_SRC),
    E2(0x63, TEXT_TCM,  M_r,   M_Ir, DST_SRC),
    E2(0x64, TEXT_TCM,  M_R,   M_R,  SRC_DST),
    E2(0x65, TEXT_TCM,  M_R,   M_IR, SRC_DST),
    E2(0x66, TEXT_TCM,  M_R,   M_IM, DST_SRC),
    E2(0x72, TEXT_TM,   M_r,   M_r,  DST_SRC),
    E2(0x73, TEXT_TM,   M_r,   M_Ir, DST_SRC),
    E2(0x74, TEXT_TM,   M_R,   M_R,  SRC_DST),
    E2(0x75, TEXT_TM,   M_R,   M_IR, SRC_DST),
    E2(0x76, TEXT_TM,   M_R,   M_IM, DST_SRC),
    E2(0xA2, TEXT_CP,   M_r,   M_r,  DST_SRC),
    E2(0xA3, TEXT_CP,   M_r,   M_Ir, DST_SRC),
    E2(0xA4, TEXT_CP,   M_R,   M_R,  SRC_DST),
    E2(0xA5, TEXT_CP,   M_R,   M_IR, SRC_DST),
    E2(0xA6, TEXT_CP,   M_R,   M_IM, DST_SRC),
    E2(0xB2, TEXT_XOR,  M_r,   M_r,  DST_SRC),
    E2(0xB3, TEXT_XOR,  M_r,   M_Ir, DST_SRC),
    E2(0xB4, TEXT_XOR,  M_R,   M_R,  SRC_DST),
    E2(0xB5, TEXT_XOR,  M_R,   M_IR, SRC_DST),
    E2(0xB6, TEXT_XOR,  M_R,   M_IM, DST_SRC),
    E1(0x00, TEXT_DEC,  M_R),
    E1(0x01, TEXT_DEC,  M_IR),
    E1(0x10, TEXT_RLC,  M_R),
    E1(0x11, TEXT_RLC,  M_IR),
    E1(0x0E, TEXT_INC,  M_r),
    E1(0x20, TEXT_INC,  M_R),
    E1(0x21, TEXT_INC,  M_IR),
    E1(0x40, TEXT_DA,   M_R),
    E1(0x41, TEXT_DA,   M_IR),
    E1(0x50, TEXT_POP,  M_R),
    E1(0x51, TEXT_POP,  M_IR),
    E1(0x60, TEXT_COM,  M_R),
    E1(0x61, TEXT_COM,  M_IR),
    E1(0x70, TEXT_PUSH, M_R),
    E1(0x71, TEXT_PUSH, M_IR),
    E1(0x80, TEXT_DECW, M_RR),
    E1(0x81, TEXT_DECW, M_IR),
    E1(0x90, TEXT_RL,   M_R),
    E1(0x91, TEXT_RL,   M_IR),
    E1(0xA0, TEXT_INCW, M_RR),
    E1(0xA1, TEXT_INCW, M_IR),
    E1(0xB0, TEXT_CLR,  M_R),
    E1(0xB1, TEXT_CLR,  M_IR),
    E1(0xC0, TEXT_RRC,  M_R),
    E1(0xC1, TEXT_RRC,  M_IR),
    E1(0xD0, TEXT_SRA,  M_R),
    E1(0xD1, TEXT_SRA,  M_IR),
    E1(0xE0, TEXT_RR,   M_R),
    E1(0xE1, TEXT_RR,   M_IR),
    E1(0xF0, TEXT_SWAP, M_R),
    E1(0xF1, TEXT_SWAP, M_IR),
    E0(0x8F, TEXT_DI),
    E0(0x9F, TEXT_EI),
    E0(0xAF, TEXT_RET),
    E0(0xBF, TEXT_IRET),
    E0(0xCF, TEXT_RCF),
    E0(0xDF, TEXT_SCF),
    E0(0xEF, TEXT_CCF),
    E0(0xFF, TEXT_NOP),
};

static constexpr uint8_t INDEX_COMMON[] PROGMEM = {
     18,  // TEXT_ADC
     19,  // TEXT_ADC
     20,  // TEXT_ADC
     21,  // TEXT_ADC
     22,  // TEXT_ADC
     13,  // TEXT_ADD
     14,  // TEXT_ADD
     15,  // TEXT_ADD
     16,  // TEXT_ADD
     17,  // TEXT_ADD
     38,  // TEXT_AND
     39,  // TEXT_AND
     40,  // TEXT_AND
     41,  // TEXT_AND
     42,  // TEXT_AND
    100,  // TEXT_CCF
     84,  // TEXT_CLR
     85,  // TEXT_CLR
     74,  // TEXT_COM
     75,  // TEXT_COM
     53,  // TEXT_CP
     54,  // TEXT_CP
     55,  // TEXT_CP
     56,  // TEXT_CP
     57,  // TEXT_CP
     70,  // TEXT_DA
     71,  // TEXT_DA
     63,  // TEXT_DEC
     64,  // TEXT_DEC
     78,  // TEXT_DECW
     79,  // TEXT_DECW
     94,  // TEXT_DI
      7,  // TEXT_DJNZ
     95,  // TEXT_EI
     67,  // TEXT_INC
     68,  // TEXT_INC
     69,  // TEXT_INC
     82,  // TEXT_INCW
     83,  // TEXT_INCW
     97,  // TEXT_IRET
     10,  // TEXT_JP
     11,  // TEXT_JP
     12,  // TEXT_JP
      8,  // TEXT_JR
      9,  // TEXT_JR
      0,  // TEXT_LD
      1,  // TEXT_LD
      2,  // TEXT_LD
      3,  // TEXT_LD
      4,  // TEXT_LD
      5,  // TEXT_LD
      6,  // TEXT_LD
    101,  // TEXT_NOP
     33,  // TEXT_OR
     34,  // TEXT_OR
     35,  // TEXT_OR
     36,  // TEXT_OR
     37,  // TEXT_OR
     72,  // TEXT_POP
     73,  // TEXT_POP
     76,  // TEXT_PUSH
     77,  // TEXT_PUSH
     98,  // TEXT_RCF
     96,  // TEXT_RET
     80,  // TEXT_RL
     81,  // TEXT_RL
     65,  // TEXT_RLC
     66,  // TEXT_RLC
     90,  // TEXT_RR
     91,  // TEXT_RR
     86,  // TEXT_RRC
     87,  // TEXT_RRC
     28,  // TEXT_SBC
     29,  // TEXT_SBC
     30,  // TEXT_SBC
     31,  // TEXT_SBC
     32,  // TEXT_SBC
     99,  // TEXT_SCF
     88,  // TEXT_SRA
     89,  // TEXT_SRA
     23,  // TEXT_SUB
     24,  // TEXT_SUB
     25,  // TEXT_SUB
     26,  // TEXT_SUB
     27,  // TEXT_SUB
     92,  // TEXT_SWAP
     93,  // TEXT_SWAP
     43,  // TEXT_TCM
     44,  // TEXT_TCM
     45,  // TEXT_TCM
     46,  // TEXT_TCM
     47,  // TEXT_TCM
     48,  // TEXT_TM
     49,  // TEXT_TM
     50,  // TEXT_TM
     51,  // TEXT_TM
     52,  // TEXT_TM
     58,  // TEXT_XOR
     59,  // TEXT_XOR
     60,  // TEXT_XOR
     61,  // TEXT_XOR
     62,  // TEXT_XOR
};

static constexpr Entry TABLE_Z8[] PROGMEM = {
    E1(0x31, TEXT_SRP,  M_IM),
    E2(0x07, TEXT_ADD,  M_IR,  M_IM,  DST_SRC),
    E2(0x17, TEXT_ADC,  M_IR,  M_IM,  DST_SRC),
    E2(0x27, TEXT_SUB,  M_IR,  M_IM,  DST_SRC),
    E2(0x37, TEXT_SBC,  M_IR,  M_IM,  DST_SRC),
    E2(0x47, TEXT_OR,   M_IR,  M_IM,  DST_SRC),
    E2(0x57, TEXT_AND,  M_IR,  M_IM,  DST_SRC),
    E2(0x67, TEXT_TCM,  M_IR,  M_IM,  DST_SRC),
    E2(0x77, TEXT_TM,   M_IR,  M_IM,  DST_SRC),
    E2(0xA7, TEXT_CP,   M_IR,  M_IM,  DST_SRC),
    E2(0xB7, TEXT_XOR,  M_IR,  M_IM,  DST_SRC),
    E2(0xC7, TEXT_LD,   M_r,   M_X,   DST_SRC),
    E2(0xD7, TEXT_LD,   M_X,   M_r,   SRC_DST),
    E2(0xE3, TEXT_LD,   M_r,   M_Ir,  DST_SRC),
    E2(0xE7, TEXT_LD,   M_IR,  M_IM,  DST_SRC),
    E2(0xF3, TEXT_LD,   M_Ir,  M_r,   DST_SRC),
    E2(0x82, TEXT_LDE,  M_r,   M_Irr, DST_SRC),
    E2(0x92, TEXT_LDE,  M_Irr, M_r,   SRC_DST),
    E2(0x83, TEXT_LDEI, M_Ir,  M_Irr, DST_SRC),
    E2(0x93, TEXT_LDEI, M_Irr, M_Ir,  SRC_DST),
    E2(0xC2, TEXT_LDC,  M_r,   M_Irr, DST_SRC),
    E2(0xD2, TEXT_LDC,  M_Irr, M_r,   SRC_DST),
    E2(0xC3, TEXT_LDCI, M_Ir,  M_Irr, DST_SRC),
    E2(0xD3, TEXT_LDCI, M_Irr, M_Ir,  SRC_DST),
    E1(0xD6, TEXT_CALL, M_DA),
    E1(0xD4, TEXT_CALL, M_IRR),
};

static constexpr uint8_t INDEX_Z8[] PROGMEM = {
      2,  // TEXT_ADC
      1,  // TEXT_ADD
      6,  // TEXT_AND
     24,  // TEXT_CALL
     25,  // TEXT_CALL
      9,  // TEXT_CP
     11,  // TEXT_LD
     12,  // TEXT_LD
     13,  // TEXT_LD
     14,  // TEXT_LD
     15,  // TEXT_LD
     20,  // TEXT_LDC
     21,  // TEXT_LDC
     22,  // TEXT_LDCI
     23,  // TEXT_LDCI
     16,  // TEXT_LDE
     17,  // TEXT_LDE
     18,  // TEXT_LDEI
     19,  // TEXT_LDEI
      5,  // TEXT_OR
      4,  // TEXT_SBC
      0,  // TEXT_SRP
      3,  // TEXT_SUB
      7,  // TEXT_TCM
      8,  // TEXT_TM
     10,  // TEXT_XOR
};

static constexpr Entry TABLE_Z86C[] PROGMEM = {
    E0(0x6F, TEXT_STOP),
    E0(0x7F, TEXT_HALT),
};

static constexpr uint8_t INDEX_Z86C[] PROGMEM = {
      1,  // TEXT_HALT
      0,  // TEXT_STOP
};

static constexpr Entry TABLE_SUPER8[] PROGMEM = {
    E0(0x0F, TEXT_NEXT),
    E0(0x1F, TEXT_ENTER),
    E0(0x2F, TEXT_EXIT),
    E0(0x3F, TEXT_WFI),
    E0(0x4F, TEXT_SB0),
    E0(0x5F, TEXT_SB1),
    E1(0xF6, TEXT_CALL, M_DA),
    E1(0xF4, TEXT_CALL, M_IRR),
    E1(0xD4, TEXT_CALL, M_IM),
    E2(0x87, TEXT_LD,   M_r,   M_X,   DST_SRC),
    E2(0x97, TEXT_LD,   M_X,   M_r,   SRC_DST),
    E2(0xC7, TEXT_LD,   M_r,   M_Ir,  DST_SRC),
    E2(0xD7, TEXT_LD,   M_Ir,  M_r,   DST_SRC),
    E2(0xD6, TEXT_LD,   M_IR,  M_IM,  DST_SRC),
    E2(0xC4, TEXT_LDW,  M_RR,  M_RR,  SRC_DST),
    E2(0xC5, TEXT_LDW,  M_RR,  M_IR,  SRC_DST),
    E2(0xC6, TEXT_LDW,  M_RR,  M_IML, DST_SRC),
    E2(0x94, TEXT_DIV,  M_RR,  M_R,   SRC_DST),
    E2(0x95, TEXT_DIV,  M_RR,  M_IR,  SRC_DST),
    E2(0x96, TEXT_DIV,  M_RR,  M_IM,  SRC_DST),
    E2(0x84, TEXT_MULT, M_RR,  M_R,   SRC_DST),
    E2(0x85, TEXT_MULT, M_RR,  M_IR,  SRC_DST),
    E2(0x86, TEXT_MULT, M_RR,  M_IM,  SRC_DST),
    E2(0x82, TEXT_PUSHUD, M_IR, M_R,  DST_SRC),
    E2(0x83, TEXT_PUSHUI, M_IR, M_R,  DST_SRC),
    E2(0x92, TEXT_POPUD,  M_R,  M_IR, SRC_DST),
    E2(0x93, TEXT_POPUI,  M_R,  M_IR, SRC_DST),
    P3(0xC2, TEXT_CPIJE,  P0, M_r, M_Ir, SRC_DST, M_RA),
    P3(0xD2, TEXT_CPIJNE, P0, M_r, M_Ir, SRC_DST, M_RA),
};

static constexpr uint8_t INDEX_SUPER8[] PROGMEM = {
      6,  // TEXT_CALL
      7,  // TEXT_CALL
      8,  // TEXT_CALL
     27,  // TEXT_CPIJE
     28,  // TEXT_CPIJNE
     17,  // TEXT_DIV
     18,  // TEXT_DIV
     19,  // TEXT_DIV
      1,  // TEXT_ENTER
      2,  // TEXT_EXIT
      9,  // TEXT_LD
     10,  // TEXT_LD
     11,  // TEXT_LD
     12,  // TEXT_LD
     13,  // TEXT_LD
     14,  // TEXT_LDW
     15,  // TEXT_LDW
     16,  // TEXT_LDW
     20,  // TEXT_MULT
     21,  // TEXT_MULT
     22,  // TEXT_MULT
      0,  // TEXT_NEXT
     25,  // TEXT_POPUD
     26,  // TEXT_POPUI
     23,  // TEXT_PUSHUD
     24,  // TEXT_PUSHUI
      4,  // TEXT_SB0
      5,  // TEXT_SB1
      3,  // TEXT_WFI
};

static constexpr Entry TABLE_SUPER8_POST[] PROGMEM {
    P2(0xA7, TEXT_LDC,   P4_0, M_r,   M_DA,  DST_SRC),
    P2(0xA7, TEXT_LDE,   P4_1, M_r,   M_DA,  DST_SRC),
    P2(0xB7, TEXT_LDC,   P4_0, M_DA,  M_r,   SRC_DST),
    P2(0xB7, TEXT_LDE,   P4_1, M_DA,  M_r,   SRC_DST),
    P2(0xE7, TEXT_LDC,   P1_0, M_r,   M_XS,  DST_SRC),
    P2(0xE7, TEXT_LDE,   P1_1, M_r,   M_XS,  DST_SRC),
    P2(0xF7, TEXT_LDC,   P1_0, M_XS,  M_r,   SRC_DST),
    P2(0xF7, TEXT_LDE,   P1_1, M_XS,  M_r,   SRC_DST),
    P2(0xA7, TEXT_LDC,   P1_0, M_r,   M_XL,  DST_SRC),
    P2(0xA7, TEXT_LDE,   P1_1, M_r,   M_XL,  DST_SRC),
    P2(0xB7, TEXT_LDC,   P1_0, M_XL,  M_r,   SRC_DST),
    P2(0xB7, TEXT_LDE,   P1_1, M_XL,  M_r,   SRC_DST),
    P1(0x31, TEXT_SRP,   P2_0, M_IM),
    P1(0x31, TEXT_SRP0,  P2_2, M_IM),
    P1(0x31, TEXT_SRP1,  P2_1, M_IM),
    P2(0xC3, TEXT_LDC,   P1_0, M_r,   M_Irr, DST_SRC),
    P2(0xC3, TEXT_LDE,   P1_1, M_r,   M_Irr, DST_SRC),
    P2(0xD3, TEXT_LDC,   P1_0, M_Irr, M_r,   SRC_DST),
    P2(0xD3, TEXT_LDE,   P1_1, M_Irr, M_r,   SRC_DST),
    P2(0xE2, TEXT_LDCD,  P1_0, M_r,   M_Irr, DST_SRC),
    P2(0xE2, TEXT_LDED,  P1_1, M_r,   M_Irr, DST_SRC),
    P2(0xE3, TEXT_LDCI,  P1_0, M_r,   M_Irr, DST_SRC),
    P2(0xE3, TEXT_LDEI,  P1_1, M_r,   M_Irr, DST_SRC),
    P2(0xF2, TEXT_LDCPD, P1_0, M_Irr, M_r,   SRC_DST),
    P2(0xF2, TEXT_LDEPD, P1_1, M_Irr, M_r,   SRC_DST),
    P2(0xF3, TEXT_LDCPI, P1_0, M_Irr, M_r,   SRC_DST),
    P2(0xF3, TEXT_LDEPI, P1_1, M_Irr, M_r,   SRC_DST),
    P3(0x17, TEXT_BCP,   P1_0, M_r,   M_R,   DST_SRC, M_IMb),
    P2(0x57, TEXT_BITC,  P1_0, M_r,   M_IMb, DS_NO),
    P2(0x77, TEXT_BITR,  P1_0, M_r,   M_IMb, DS_NO),
    P2(0x77, TEXT_BITS,  P1_1, M_r,   M_IMb, DS_NO),
    P3(0x07, TEXT_BOR,   P1_0, M_r,   M_R,   DST_SRC, M_IMb),
    P3(0x07, TEXT_BOR,   P1_1, M_R,   M_IMb, SRC_DST, M_r),
    P3(0x27, TEXT_BXOR,  P1_0, M_r,   M_R,   DST_SRC, M_IMb),
    P3(0x27, TEXT_BXOR,  P1_1, M_R,   M_IMb, SRC_DST, M_r),
    P3(0x47, TEXT_LDB,   P1_0, M_r,   M_R,   DST_SRC, M_IMb),
    P3(0x47, TEXT_LDB,   P1_1, M_R,   M_IMb, SRC_DST, M_r),
    P3(0x67, TEXT_BAND,  P1_0, M_r,   M_R,   DST_SRC, M_IMb),
    P3(0x67, TEXT_BAND,  P1_1, M_R,   M_IMb, SRC_DST, M_r),
    P3(0x37, TEXT_BTJRF, P1_0, M_RA,  M_r,   SRC_DST, M_IMb),
    P3(0x37, TEXT_BTJRT, P1_1, M_RA,  M_r,   SRC_DST, M_IMb),
};

static constexpr uint8_t INDEX_SUPER8_POST[] PROGMEM {
     37,  // TEXT_BAND
     38,  // TEXT_BAND
     27,  // TEXT_BCP
     28,  // TEXT_BITC
     29,  // TEXT_BITR
     30,  // TEXT_BITS
     31,  // TEXT_BOR
     32,  // TEXT_BOR
     39,  // TEXT_BTJRF
     40,  // TEXT_BTJRT
     33,  // TEXT_BXOR
     34,  // TEXT_BXOR
     35,  // TEXT_LDB
     36,  // TEXT_LDB
      0,  // TEXT_LDC
      2,  // TEXT_LDC
      4,  // TEXT_LDC
      6,  // TEXT_LDC
      8,  // TEXT_LDC
     10,  // TEXT_LDC
     15,  // TEXT_LDC
     17,  // TEXT_LDC
     19,  // TEXT_LDCD
     21,  // TEXT_LDCI
     23,  // TEXT_LDCPD
     25,  // TEXT_LDCPI
      1,  // TEXT_LDE
      3,  // TEXT_LDE
      5,  // TEXT_LDE
      7,  // TEXT_LDE
      9,  // TEXT_LDE
     11,  // TEXT_LDE
     16,  // TEXT_LDE
     18,  // TEXT_LDE
     20,  // TEXT_LDED
     22,  // TEXT_LDEI
     24,  // TEXT_LDEPD
     26,  // TEXT_LDEPI
     12,  // TEXT_SRP
     13,  // TEXT_SRP0
     14,  // TEXT_SRP1
};
// clang-format on

static constexpr TableZ8::EntryPage Z8_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_Z8), ARRAY_RANGE(INDEX_Z8)},
        {ARRAY_RANGE(TABLE_COMMON), ARRAY_RANGE(INDEX_COMMON)},
};

static constexpr TableZ8::EntryPage Z86C_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_Z8), ARRAY_RANGE(INDEX_Z8)},
        {ARRAY_RANGE(TABLE_COMMON), ARRAY_RANGE(INDEX_COMMON)},
        {ARRAY_RANGE(TABLE_Z86C), ARRAY_RANGE(INDEX_Z86C)},
};

static constexpr TableZ8::EntryPage SUPER8_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_SUPER8), ARRAY_RANGE(INDEX_SUPER8)},
        {ARRAY_RANGE(TABLE_SUPER8_POST), ARRAY_RANGE(INDEX_SUPER8_POST)},
        {ARRAY_RANGE(TABLE_COMMON), ARRAY_RANGE(INDEX_COMMON)},
};

static constexpr TableZ8::Cpu CPU_TABLES[] PROGMEM = {
        {Z8, TEXT_CPU_Z8, ARRAY_RANGE(Z8_PAGES)},
        {Z86C, TEXT_CPU_Z86C, ARRAY_RANGE(Z86C_PAGES)},
        {SUPER8, TEXT_CPU_Z88, ARRAY_RANGE(SUPER8_PAGES)},
};

static bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_IM)
        return table == M_IMb || table == M_IML;
    if (opr == M_DA)
        return table == M_RA;
    if (opr == M_R)
        return table == M_DA || table == M_RA;
    if (opr == M_RR)
        return table == M_DA || table == M_RA || table == M_R;
    if (opr == M_IRR)
        return table == M_IR;
    if (opr == M_r)
        return table == M_R;
    if (opr == M_rr)
        return table == M_RR;
    if (opr == M_Ir)
        return table == M_IR;
    if (opr == M_Irr)
        return table == M_IRR;
    if (opr == M_W)
        return table == M_DA || table == M_RA || table == M_r || table == M_R;
    if (opr == M_WW)
        return table == M_DA || table == M_RA || table == M_rr || table == M_RR || table == M_r ||
               table == M_R;
    if (opr == M_IW)
        return table == M_Ir || table == M_IR;
    if (opr == M_IWW)
        return table == M_Irr || table == M_IRR || table == M_Ir || table == M_IR;
    if (opr == M_XS)
        return table == M_X;
    if (opr == M_XL)
        return table == M_X;
    return false;
}

static bool acceptModes(Entry::Flags flags, const Entry *entry) {
    auto table = entry->flags();
    return acceptMode(flags.dstMode(), table.dstMode()) &&
           acceptMode(flags.srcMode(), table.srcMode()) &&
           acceptMode(flags.extMode(), table.extMode());
}

Error TableZ8::searchName(InsnZ8 &insn) {
    uint8_t count = 0;
    for (auto page = _cpu->table(); page < _cpu->end(); page++) {
        auto entry = searchEntry(insn.name(), insn.flags(), page, acceptModes, count);
        if (entry) {
            insn.setOpCode(entry->opCode());
            insn.setFlags(entry->flags());
            return setOK();
        }
    }
    return setError(count == 0 ? UNKNOWN_INSTRUCTION : OPERAND_NOT_ALLOWED);
}

static Config::opcode_t maskCode(Config::opcode_t opCode, const Entry *entry) {
    auto table = entry->opCode();
    return InsnZ8::operandInOpCode(table) ? opCode & 0x0f : opCode;
}

static bool matchPostByte(const InsnZ8 &insn) {
    auto post = insn.post();
    switch (insn.postFormat()) {
    case P1_0:
        return (post & 1) == 0;
    case P1_1:
        return (post & 1) == 1;
    case P2_0:
        return (post & 3) == 0;
    case P2_1:
        return (post & 3) == 1;
    case P2_2:
        return (post & 3) == 2;
    case P4_0:
        return (post & 0xF) == 0;
    case P4_1:
        return (post & 0xF) == 1;
    default:
        return true;
    }
}

Error TableZ8::searchOpCode(InsnZ8 &insn, DisMemory &memory) {
    for (auto page = _cpu->table(); page < _cpu->end(); page++) {
        auto end = page->end();
        for (auto entry = page->table(); entry < end; entry++) {
            entry = searchEntry(insn.opCode(), entry, end, maskCode);
            if (entry == nullptr)
                break;
            insn.setFlags(entry->flags());
            if (insn.postFormat()) {
                if (insn.length() < 2) {
                    insn.readPost(memory);
                    if (insn.getError())
                        return setError(NO_MEMORY);
                }
                if (!matchPostByte(insn))
                    continue;
            }
            insn.nameBuffer().text_P(entry->name_P());
            return setOK();
        }
    }
    return setError(UNKNOWN_INSTRUCTION);
}

TableZ8::TableZ8() {
    setCpu(Z8);
}

bool TableZ8::isSuper8() const {
    return _cpu->cpuType() == SUPER8;
}

bool TableZ8::setCpu(CpuType cpuType) {
    auto t = Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLES));
    if (t == nullptr)
        return false;
    _cpu = t;
    return true;
}

const /* PROGMEM */ char *TableZ8::listCpu_P() const {
    return TEXT_CPU_LIST;
}

const /* PROGMEM */ char *TableZ8::cpu_P() const {
    return _cpu->name_P();
}

bool TableZ8::setCpu(const char *cpu) {
    auto t = Cpu::search(cpu, ARRAY_RANGE(CPU_TABLES));
    if (t)
        return setCpu(t->cpuType());
    if (strncasecmp_P(cpu, TEXT_CPU_Z86C, 4) == 0)
        return setCpu(Z86C);
    if (strncasecmp_P(cpu, TEXT_CPU_Z86, 3) == 0)
        return setCpu(Z8);
    if (strncasecmp_P(cpu, TEXT_CPU_Z88, 3) == 0 || strcasecmp_P(cpu, TEXT_CPU_SUPER8) == 0)
        return setCpu(SUPER8);
    return false;
}

TableZ8 TableZ8::TABLE;

}  // namespace z8
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

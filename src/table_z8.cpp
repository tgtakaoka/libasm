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

#include "table_z8.h"
#include "entry_table.h"
#include "entry_z8.h"
#include "text_z8.h"

using namespace libasm::text::z8;

namespace libasm {
namespace z8 {

#define E3(_opc, _name, _postFormat, _dst, _src, _ext, _dstPos, _srcPos, _extPos) \
    {_opc, Entry::Flags::create(_dst, _src, _ext, _postFormat, _dstPos, _srcPos, _extPos), _name}
#define E2(_opc, _name, _postFormat, _dst, _src, _dstPos, _srcPos) \
    E3(_opc, _name, _postFormat, _dst, _src, M_NONE, _dstPos, _srcPos, OP_NONE)
#define E1(_opc, _name, _postFormat, _dst, _dstPos) \
    E2(_opc, _name, _postFormat, _dst, M_NONE, _dstPos, OP_NONE)
#define E0(_opc, _name, _postFormat) E1(_opc, _name, _postFormat, M_NONE, OP_NONE)

// clang-format off
constexpr Entry TABLE_COMMON[] PROGMEM = {
    E2(0x08, TEXT_LD,   PF_NONE, M_r,   M_R,  OP_CODE, OP_BYT1),
    E2(0x09, TEXT_LD,   PF_NONE, M_R,   M_r,  OP_BYT1, OP_CODE),
    E2(0x0C, TEXT_LD,   PF_NONE, M_r,   M_IM, OP_CODE, OP_BYT1),
    E2(0xE4, TEXT_LD,   PF_NONE, M_R,   M_R,  OP_BYT2, OP_BYT1),
    E2(0xE5, TEXT_LD,   PF_NONE, M_R,   M_IR, OP_BYT2, OP_BYT1),
    E2(0xE6, TEXT_LD,   PF_NONE, M_R,   M_IM, OP_BYT1, OP_BYT2),
    E2(0xF5, TEXT_LD,   PF_NONE, M_IR,  M_R,  OP_BYT2, OP_BYT1),
    E2(0x0A, TEXT_DJNZ, PF_NONE, M_r,   M_RA, OP_CODE, OP_BYT1),
    E1(0x8B, TEXT_JR,   PF_NONE, M_RA,  OP_BYT1),
    E2(0x0B, TEXT_JR,   PF_NONE, M_cc,  M_RA, OP_CODE, OP_BYT1),
    E1(0x8D, TEXT_JP,   PF_NONE, M_DA,  OP_W1BE),
    E2(0x0D, TEXT_JP,   PF_NONE, M_cc,  M_DA, OP_CODE, OP_W1BE),
    E1(0x30, TEXT_JP,   PF_NONE, M_IRR, OP_BYT1),
    E2(0x02, TEXT_ADD,  PF_NONE, M_r,   M_r,  OP_B1HI, OP_B1LO),
    E2(0x03, TEXT_ADD,  PF_NONE, M_r,   M_Ir, OP_B1HI, OP_B1LO),
    E2(0x04, TEXT_ADD,  PF_NONE, M_R,   M_R,  OP_BYT2, OP_BYT1),
    E2(0x05, TEXT_ADD,  PF_NONE, M_R,   M_IR, OP_BYT2, OP_BYT1),
    E2(0x06, TEXT_ADD,  PF_NONE, M_R,   M_IM, OP_BYT1, OP_BYT2),
    E2(0x12, TEXT_ADC,  PF_NONE, M_r,   M_r,  OP_B1HI, OP_B1LO),
    E2(0x13, TEXT_ADC,  PF_NONE, M_r,   M_Ir, OP_B1HI, OP_B1LO),
    E2(0x14, TEXT_ADC,  PF_NONE, M_R,   M_R,  OP_BYT2, OP_BYT1),
    E2(0x15, TEXT_ADC,  PF_NONE, M_R,   M_IR, OP_BYT2, OP_BYT1),
    E2(0x16, TEXT_ADC,  PF_NONE, M_R,   M_IM, OP_BYT1, OP_BYT2),
    E2(0x22, TEXT_SUB,  PF_NONE, M_r,   M_r,  OP_B1HI, OP_B1LO),
    E2(0x23, TEXT_SUB,  PF_NONE, M_r,   M_Ir, OP_B1HI, OP_B1LO),
    E2(0x24, TEXT_SUB,  PF_NONE, M_R,   M_R,  OP_BYT2, OP_BYT1),
    E2(0x25, TEXT_SUB,  PF_NONE, M_R,   M_IR, OP_BYT2, OP_BYT1),
    E2(0x26, TEXT_SUB,  PF_NONE, M_R,   M_IM, OP_BYT1, OP_BYT2),
    E2(0x32, TEXT_SBC,  PF_NONE, M_r,   M_r,  OP_B1HI, OP_B1LO),
    E2(0x33, TEXT_SBC,  PF_NONE, M_r,   M_Ir, OP_B1HI, OP_B1LO),
    E2(0x34, TEXT_SBC,  PF_NONE, M_R,   M_R,  OP_BYT2, OP_BYT1),
    E2(0x35, TEXT_SBC,  PF_NONE, M_R,   M_IR, OP_BYT2, OP_BYT1),
    E2(0x36, TEXT_SBC,  PF_NONE, M_R,   M_IM, OP_BYT1, OP_BYT2),
    E2(0x42, TEXT_OR,   PF_NONE, M_r,   M_r,  OP_B1HI, OP_B1LO),
    E2(0x43, TEXT_OR,   PF_NONE, M_r,   M_Ir, OP_B1HI, OP_B1LO),
    E2(0x44, TEXT_OR,   PF_NONE, M_R,   M_R,  OP_BYT2, OP_BYT1),
    E2(0x45, TEXT_OR,   PF_NONE, M_R,   M_IR, OP_BYT2, OP_BYT1),
    E2(0x46, TEXT_OR,   PF_NONE, M_R,   M_IM, OP_BYT1, OP_BYT2),
    E2(0x52, TEXT_AND,  PF_NONE, M_r,   M_r,  OP_B1HI, OP_B1LO),
    E2(0x53, TEXT_AND,  PF_NONE, M_r,   M_Ir, OP_B1HI, OP_B1LO),
    E2(0x54, TEXT_AND,  PF_NONE, M_R,   M_R,  OP_BYT2, OP_BYT1),
    E2(0x55, TEXT_AND,  PF_NONE, M_R,   M_IR, OP_BYT2, OP_BYT1),
    E2(0x56, TEXT_AND,  PF_NONE, M_R,   M_IM, OP_BYT1, OP_BYT2),
    E2(0x62, TEXT_TCM,  PF_NONE, M_r,   M_r,  OP_B1HI, OP_B1LO),
    E2(0x63, TEXT_TCM,  PF_NONE, M_r,   M_Ir, OP_B1HI, OP_B1LO),
    E2(0x64, TEXT_TCM,  PF_NONE, M_R,   M_R,  OP_BYT2, OP_BYT1),
    E2(0x65, TEXT_TCM,  PF_NONE, M_R,   M_IR, OP_BYT2, OP_BYT1),
    E2(0x66, TEXT_TCM,  PF_NONE, M_R,   M_IM, OP_BYT1, OP_BYT2),
    E2(0x72, TEXT_TM,   PF_NONE, M_r,   M_r,  OP_B1HI, OP_B1LO),
    E2(0x73, TEXT_TM,   PF_NONE, M_r,   M_Ir, OP_B1HI, OP_B1LO),
    E2(0x74, TEXT_TM,   PF_NONE, M_R,   M_R,  OP_BYT2, OP_BYT1),
    E2(0x75, TEXT_TM,   PF_NONE, M_R,   M_IR, OP_BYT2, OP_BYT1),
    E2(0x76, TEXT_TM,   PF_NONE, M_R,   M_IM, OP_BYT1, OP_BYT2),
    E2(0xA2, TEXT_CP,   PF_NONE, M_r,   M_r,  OP_B1HI, OP_B1LO),
    E2(0xA3, TEXT_CP,   PF_NONE, M_r,   M_Ir, OP_B1HI, OP_B1LO),
    E2(0xA4, TEXT_CP,   PF_NONE, M_R,   M_R,  OP_BYT2, OP_BYT1),
    E2(0xA5, TEXT_CP,   PF_NONE, M_R,   M_IR, OP_BYT2, OP_BYT1),
    E2(0xA6, TEXT_CP,   PF_NONE, M_R,   M_IM, OP_BYT1, OP_BYT2),
    E2(0xB2, TEXT_XOR,  PF_NONE, M_r,   M_r,  OP_B1HI, OP_B1LO),
    E2(0xB3, TEXT_XOR,  PF_NONE, M_r,   M_Ir, OP_B1HI, OP_B1LO),
    E2(0xB4, TEXT_XOR,  PF_NONE, M_R,   M_R,  OP_BYT2, OP_BYT1),
    E2(0xB5, TEXT_XOR,  PF_NONE, M_R,   M_IR, OP_BYT2, OP_BYT1),
    E2(0xB6, TEXT_XOR,  PF_NONE, M_R,   M_IM, OP_BYT1, OP_BYT2),
    E1(0x00, TEXT_DEC,  PF_NONE, M_R,   OP_BYT1),
    E1(0x01, TEXT_DEC,  PF_NONE, M_IR,  OP_BYT1),
    E1(0x10, TEXT_RLC,  PF_NONE, M_R,   OP_BYT1),
    E1(0x11, TEXT_RLC,  PF_NONE, M_IR,  OP_BYT1),
    E1(0x0E, TEXT_INC,  PF_NONE, M_r,   OP_CODE),
    E1(0x20, TEXT_INC,  PF_NONE, M_R,   OP_BYT1),
    E1(0x21, TEXT_INC,  PF_NONE, M_IR,  OP_BYT1),
    E1(0x40, TEXT_DA,   PF_NONE, M_R,   OP_BYT1),
    E1(0x41, TEXT_DA,   PF_NONE, M_IR,  OP_BYT1),
    E1(0x50, TEXT_POP,  PF_NONE, M_R,   OP_BYT1),
    E1(0x51, TEXT_POP,  PF_NONE, M_IR,  OP_BYT1),
    E1(0x60, TEXT_COM,  PF_NONE, M_R,   OP_BYT1),
    E1(0x61, TEXT_COM,  PF_NONE, M_IR,  OP_BYT1),
    E1(0x70, TEXT_PUSH, PF_NONE, M_R,   OP_BYT1),
    E1(0x71, TEXT_PUSH, PF_NONE, M_IR,  OP_BYT1),
    E1(0x80, TEXT_DECW, PF_NONE, M_RR,  OP_BYT1),
    E1(0x81, TEXT_DECW, PF_NONE, M_IR,  OP_BYT1),
    E1(0x90, TEXT_RL,   PF_NONE, M_R,   OP_BYT1),
    E1(0x91, TEXT_RL,   PF_NONE, M_IR,  OP_BYT1),
    E1(0xA0, TEXT_INCW, PF_NONE, M_RR,  OP_BYT1),
    E1(0xA1, TEXT_INCW, PF_NONE, M_IR,  OP_BYT1),
    E1(0xB0, TEXT_CLR,  PF_NONE, M_R,   OP_BYT1),
    E1(0xB1, TEXT_CLR,  PF_NONE, M_IR,  OP_BYT1),
    E1(0xC0, TEXT_RRC,  PF_NONE, M_R,   OP_BYT1),
    E1(0xC1, TEXT_RRC,  PF_NONE, M_IR,  OP_BYT1),
    E1(0xD0, TEXT_SRA,  PF_NONE, M_R,   OP_BYT1),
    E1(0xD1, TEXT_SRA,  PF_NONE, M_IR,  OP_BYT1),
    E1(0xE0, TEXT_RR,   PF_NONE, M_R,   OP_BYT1),
    E1(0xE1, TEXT_RR,   PF_NONE, M_IR,  OP_BYT1),
    E1(0xF0, TEXT_SWAP, PF_NONE, M_R,   OP_BYT1),
    E1(0xF1, TEXT_SWAP, PF_NONE, M_IR,  OP_BYT1),
    E0(0x8F, TEXT_DI,   PF_NONE),
    E0(0x9F, TEXT_EI,   PF_NONE),
    E0(0xAF, TEXT_RET,  PF_NONE),
    E0(0xBF, TEXT_IRET, PF_NONE),
    E0(0xCF, TEXT_RCF,  PF_NONE),
    E0(0xDF, TEXT_SCF,  PF_NONE),
    E0(0xEF, TEXT_CCF,  PF_NONE),
    E0(0xFF, TEXT_NOP,  PF_NONE),
};

constexpr uint8_t INDEX_COMMON[] PROGMEM = {
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

constexpr Entry TABLE_Z8[] PROGMEM = {
    E1(0x31, TEXT_SRP,  PF_NONE, M_IM,  OP_BYT1),
    E2(0x07, TEXT_ADD,  PF_NONE, M_IR,  M_IM,  OP_BYT1, OP_BYT2),
    E2(0x17, TEXT_ADC,  PF_NONE, M_IR,  M_IM,  OP_BYT1, OP_BYT2),
    E2(0x27, TEXT_SUB,  PF_NONE, M_IR,  M_IM,  OP_BYT1, OP_BYT2),
    E2(0x37, TEXT_SBC,  PF_NONE, M_IR,  M_IM,  OP_BYT1, OP_BYT2),
    E2(0x47, TEXT_OR,   PF_NONE, M_IR,  M_IM,  OP_BYT1, OP_BYT2),
    E2(0x57, TEXT_AND,  PF_NONE, M_IR,  M_IM,  OP_BYT1, OP_BYT2),
    E2(0x67, TEXT_TCM,  PF_NONE, M_IR,  M_IM,  OP_BYT1, OP_BYT2),
    E2(0x77, TEXT_TM,   PF_NONE, M_IR,  M_IM,  OP_BYT1, OP_BYT2),
    E2(0xA7, TEXT_CP,   PF_NONE, M_IR,  M_IM,  OP_BYT1, OP_BYT2),
    E2(0xB7, TEXT_XOR,  PF_NONE, M_IR,  M_IM,  OP_BYT1, OP_BYT2),
    E2(0xC7, TEXT_LD,   PF_NONE, M_r,   M_X,   OP_B1HI, OP_BYT2), // x: OP_B1LO
    E2(0xD7, TEXT_LD,   PF_NONE, M_X,   M_r,   OP_BYT2, OP_B1HI), // x: OP_B1LO
    E2(0xE3, TEXT_LD,   PF_NONE, M_r,   M_Ir,  OP_B1HI, OP_B1LO),
    E2(0xE7, TEXT_LD,   PF_NONE, M_IR,  M_IM,  OP_BYT1, OP_BYT2),
    E2(0xF3, TEXT_LD,   PF_NONE, M_Ir,  M_r,   OP_B1HI, OP_B1LO),
    E2(0x82, TEXT_LDE,  PF_NONE, M_r,   M_Irr, OP_B1HI, OP_B1LO),
    E2(0x92, TEXT_LDE,  PF_NONE, M_Irr, M_r,   OP_B1LO, OP_B1HI),
    E2(0x83, TEXT_LDEI, PF_NONE, M_Ir,  M_Irr, OP_B1HI, OP_B1LO),
    E2(0x93, TEXT_LDEI, PF_NONE, M_Irr, M_Ir,  OP_B1LO, OP_B1HI),
    E2(0xC2, TEXT_LDC,  PF_NONE, M_r,   M_Irr, OP_B1HI, OP_B1LO),
    E2(0xD2, TEXT_LDC,  PF_NONE, M_Irr, M_r,   OP_B1LO, OP_B1HI),
    E2(0xC3, TEXT_LDCI, PF_NONE, M_Ir,  M_Irr, OP_B1HI, OP_B1LO),
    E2(0xD3, TEXT_LDCI, PF_NONE, M_Irr, M_Ir,  OP_B1LO, OP_B1HI),
    E1(0xD6, TEXT_CALL, PF_NONE, M_DA,  OP_W1BE),
    E1(0xD4, TEXT_CALL, PF_NONE, M_IRR, OP_BYT1),
};

constexpr uint8_t INDEX_Z8[] PROGMEM = {
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

constexpr Entry TABLE_Z86C[] PROGMEM = {
    E0(0x6F, TEXT_STOP, PF_NONE),
    E0(0x7F, TEXT_HALT, PF_NONE),
};

constexpr uint8_t INDEX_Z86C[] PROGMEM = {
      1,  // TEXT_HALT
      0,  // TEXT_STOP
};

constexpr Entry TABLE_SUPER8[] PROGMEM = {
    E0(0x0F, TEXT_NEXT,   PF_NONE),
    E0(0x1F, TEXT_ENTER,  PF_NONE),
    E0(0x2F, TEXT_EXIT,   PF_NONE),
    E0(0x3F, TEXT_WFI,    PF_NONE),
    E0(0x4F, TEXT_SB0,    PF_NONE),
    E0(0x5F, TEXT_SB1,    PF_NONE),
    E1(0xF6, TEXT_CALL,   PF_NONE, M_DA,  OP_W1BE),
    E1(0xF4, TEXT_CALL,   PF_NONE, M_IRR, OP_BYT1),
    E1(0xD4, TEXT_CALL,   PF_NONE, M_IA,  OP_BYT1),
    E2(0x87, TEXT_LD,     PF_NONE, M_r,   M_X,   OP_B1HI, OP_BYT2), // x: OP_B1LO
    E2(0x97, TEXT_LD,     PF_NONE, M_X,   M_r,   OP_BYT2, OP_B1HI), // x: OP_B1LO
    E2(0xC7, TEXT_LD,     PF_NONE, M_r,   M_Ir,  OP_B1HI, OP_B1LO),
    E2(0xD7, TEXT_LD,     PF_NONE, M_Ir,  M_r,   OP_B1HI, OP_B1LO),
    E2(0xD6, TEXT_LD,     PF_NONE, M_IR,  M_IM,  OP_BYT1, OP_BYT2),
    E2(0xC4, TEXT_LDW,    PF_NONE, M_RR,  M_RR,  OP_BYT2, OP_BYT1),
    E2(0xC5, TEXT_LDW,    PF_NONE, M_RR,  M_IR,  OP_BYT2, OP_BYT1),
    E2(0xC6, TEXT_LDW,    PF_NONE, M_RR,  M_IML, OP_BYT1, OP_W2BE),
    E2(0x94, TEXT_DIV,    PF_NONE, M_RR,  M_R,   OP_BYT2, OP_BYT1),
    E2(0x95, TEXT_DIV,    PF_NONE, M_RR,  M_IR,  OP_BYT2, OP_BYT1),
    E2(0x96, TEXT_DIV,    PF_NONE, M_RR,  M_IM,  OP_BYT2, OP_BYT1),
    E2(0x84, TEXT_MULT,   PF_NONE, M_RR,  M_R,   OP_BYT2, OP_BYT1),
    E2(0x85, TEXT_MULT,   PF_NONE, M_RR,  M_IR,  OP_BYT2, OP_BYT1),
    E2(0x86, TEXT_MULT,   PF_NONE, M_RR,  M_IM,  OP_BYT2, OP_BYT1),
    E2(0x82, TEXT_PUSHUD, PF_NONE, M_IR,  M_R,   OP_BYT1, OP_BYT2),
    E2(0x83, TEXT_PUSHUI, PF_NONE, M_IR,  M_R,   OP_BYT1, OP_BYT2),
    E2(0x92, TEXT_POPUD,  PF_NONE, M_R,   M_IR,  OP_BYT2, OP_BYT1),
    E2(0x93, TEXT_POPUI,  PF_NONE, M_R,   M_IR,  OP_BYT2, OP_BYT1),
    E3(0xC2, TEXT_CPIJE,  PF_NONE, M_r,   M_Ir,  M_RA, OP_B1LO, OP_B1HI, OP_BYT2),
    E3(0xD2, TEXT_CPIJNE, PF_NONE, M_r,   M_Ir,  M_RA, OP_B1LO, OP_B1HI, OP_BYT2),
};

constexpr uint8_t INDEX_SUPER8[] PROGMEM = {
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

constexpr Entry TABLE_SUPER8_POST[] PROGMEM {
    E2(0xA7, TEXT_LDC,   PF4_0, M_r,   M_DA,  OP_B1HI, OP_W2LE), // dst: r0 not allowed
    E2(0xA7, TEXT_LDE,   PF4_1, M_r,   M_DA,  OP_B1HI, OP_W2LE), // dst: r0 not allowed
    E2(0xB7, TEXT_LDC,   PF4_0, M_DA,  M_r,   OP_W2LE, OP_B1HI), // src: r0 not allowed
    E2(0xB7, TEXT_LDE,   PF4_1, M_DA,  M_r,   OP_W2LE, OP_B1HI), // src: r0 not allowed
    E2(0xE7, TEXT_LDC,   PF1_0, M_r,   M_XS,  OP_B1HI, OP_BYT2), // x: OP_B1LO
    E2(0xE7, TEXT_LDE,   PF1_1, M_r,   M_XS,  OP_B1HI, OP_BYT2), // x: OP_B1LO
    E2(0xF7, TEXT_LDC,   PF1_0, M_XS,  M_r,   OP_BYT2, OP_B1HI), // x: OP_B1LO
    E2(0xF7, TEXT_LDE,   PF1_1, M_XS,  M_r,   OP_BYT2, OP_B1HI), // x: OP_B1LO
    E2(0xA7, TEXT_LDC,   PF1_0, M_r,   M_XL,  OP_B1HI, OP_W2LE), // x: OP_B1LO
    E2(0xA7, TEXT_LDE,   PF1_1, M_r,   M_XL,  OP_B1HI, OP_W2LE), // x: OP_B1LO
    E2(0xB7, TEXT_LDC,   PF1_0, M_XL,  M_r,   OP_W2LE, OP_B1HI), // x: OP_B1LO
    E2(0xB7, TEXT_LDE,   PF1_1, M_XL,  M_r,   OP_W2LE, OP_B1HI), // x: OP_B1LO
    E1(0x31, TEXT_SRP,   PF2_0, M_IM,  OP_BYT1),
    E1(0x31, TEXT_SRP0,  PF2_2, M_IM,  OP_BYT1),
    E1(0x31, TEXT_SRP1,  PF2_1, M_IM,  OP_BYT1),
    E2(0xC3, TEXT_LDC,   PF1_0, M_r,   M_Irr, OP_B1HI, OP_B1LO),
    E2(0xC3, TEXT_LDE,   PF1_1, M_r,   M_Irr, OP_B1HI, OP_B1LO),
    E2(0xD3, TEXT_LDC,   PF1_0, M_Irr, M_r,   OP_B1LO, OP_B1HI),
    E2(0xD3, TEXT_LDE,   PF1_1, M_Irr, M_r,   OP_B1LO, OP_B1HI),
    E2(0xE2, TEXT_LDCD,  PF1_0, M_r,   M_Irr, OP_B1HI, OP_B1LO),
    E2(0xE2, TEXT_LDED,  PF1_1, M_r,   M_Irr, OP_B1HI, OP_B1LO),
    E2(0xE3, TEXT_LDCI,  PF1_0, M_r,   M_Irr, OP_B1HI, OP_B1LO),
    E2(0xE3, TEXT_LDEI,  PF1_1, M_r,   M_Irr, OP_B1HI, OP_B1LO),
    E2(0xF2, TEXT_LDCPD, PF1_0, M_Irr, M_r,   OP_B1LO, OP_B1HI),
    E2(0xF2, TEXT_LDEPD, PF1_1, M_Irr, M_r,   OP_B1LO, OP_B1HI),
    E2(0xF3, TEXT_LDCPI, PF1_0, M_Irr, M_r,   OP_B1LO, OP_B1HI),
    E2(0xF3, TEXT_LDEPI, PF1_1, M_Irr, M_r,   OP_B1LO, OP_B1HI),
    E3(0x17, TEXT_BCP,   PF1_0, M_r,   M_R,   M_IMb, OP_B1HI, OP_BYT2, OP_B1LO),
    E2(0x57, TEXT_BITC,  PF1_0, M_r,   M_IMb, OP_B1HI, OP_B1LO),
    E2(0x77, TEXT_BITR,  PF1_0, M_r,   M_IMb, OP_B1HI, OP_B1LO),
    E2(0x77, TEXT_BITS,  PF1_1, M_r,   M_IMb, OP_B1HI, OP_B1LO),
    E3(0x07, TEXT_BOR,   PF1_0, M_r,   M_R,   M_IMb, OP_B1HI, OP_BYT2, OP_B1LO),
    E3(0x07, TEXT_BOR,   PF1_1, M_R,   M_IMb, M_r,   OP_BYT2, OP_B1LO, OP_B1HI),
    E3(0x27, TEXT_BXOR,  PF1_0, M_r,   M_R,   M_IMb, OP_B1HI, OP_BYT2, OP_B1LO),
    E3(0x27, TEXT_BXOR,  PF1_1, M_R,   M_IMb, M_r,   OP_BYT2, OP_B1LO, OP_B1HI),
    E3(0x47, TEXT_LDB,   PF1_0, M_r,   M_R,   M_IMb, OP_B1HI, OP_BYT2, OP_B1LO),
    E3(0x47, TEXT_LDB,   PF1_1, M_R,   M_IMb, M_r,   OP_BYT2, OP_B1LO, OP_B1HI),
    E3(0x67, TEXT_BAND,  PF1_0, M_r,   M_R,   M_IMb, OP_B1HI, OP_BYT2, OP_B1LO),
    E3(0x67, TEXT_BAND,  PF1_1, M_R,   M_IMb, M_r,   OP_BYT2, OP_B1LO, OP_B1HI),
    E3(0x37, TEXT_BTJRF, PF1_0, M_RA,  M_r,   M_IMb, OP_BYT2, OP_B1HI, OP_B1LO),
    E3(0x37, TEXT_BTJRT, PF1_1, M_RA,  M_r,   M_IMb, OP_BYT2, OP_B1HI, OP_B1LO),
};

constexpr uint8_t INDEX_SUPER8_POST[] PROGMEM {
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

using EntryPage = entry::TableBase<Entry>;

constexpr EntryPage Z8_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_Z8), ARRAY_RANGE(INDEX_Z8)},
        {ARRAY_RANGE(TABLE_COMMON), ARRAY_RANGE(INDEX_COMMON)},
};

constexpr EntryPage Z86C_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_Z8), ARRAY_RANGE(INDEX_Z8)},
        {ARRAY_RANGE(TABLE_COMMON), ARRAY_RANGE(INDEX_COMMON)},
        {ARRAY_RANGE(TABLE_Z86C), ARRAY_RANGE(INDEX_Z86C)},
};

constexpr EntryPage SUPER8_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_SUPER8), ARRAY_RANGE(INDEX_SUPER8)},
        {ARRAY_RANGE(TABLE_SUPER8_POST), ARRAY_RANGE(INDEX_SUPER8_POST)},
        {ARRAY_RANGE(TABLE_COMMON), ARRAY_RANGE(INDEX_COMMON)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {Z8, TEXT_CPU_Z8, ARRAY_RANGE(Z8_PAGES)},
        {Z86C, TEXT_CPU_Z86C, ARRAY_RANGE(Z86C_PAGES)},
        {SUPER8, TEXT_CPU_Z88, ARRAY_RANGE(SUPER8_PAGES)},
};

const Cpu *cpu(CpuType cpuType) {
    return Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
}

bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_IM)
        return table == M_IMb || table == M_IML || table == M_IA;
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

bool acceptModes(AsmInsn &insn, const Entry *entry) {
    const auto table = entry->readFlags();
    return acceptMode(insn.dstOp.mode, table.dst()) && acceptMode(insn.srcOp.mode, table.src()) &&
           acceptMode(insn.extOp.mode, table.ext());
}

Error searchName(CpuType cpuType, AsmInsn &insn) {
    cpu(cpuType)->searchName(insn, acceptModes);
    return insn.getError();
}

bool matchPostByte(Config::opcode_t post, PostFormat format) {
    switch (format) {
    case PF1_0:
        return (post & 1) == 0;
    case PF1_1:
        return (post & 1) == 1;
    case PF2_0:
        return (post & 3) == 0;
    case PF2_1:
        return (post & 3) == 1;
    case PF2_2:
        return (post & 3) == 2;
    case PF4_0:
        return (post & 0xF) == 0;
    case PF4_1:
        return (post & 0xF) == 1;
    default:
        return true;
    }
}

bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *) {
    auto opc = insn.opCode();
    const auto flags = entry->readFlags();
    if (flags.dstPos() == OP_CODE || flags.srcPos() == OP_CODE)
        opc &= 0x0f;
    if (opc == entry->readOpCode()) {
        const auto postFormat = flags.postFormat();
        if (postFormat == PF_NONE)
            return true;
        insn.readPostfix();
        return matchPostByte(insn.postfix(), postFormat);
    }
    return false;
}

Error searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) {
    cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    return insn.getError();
}

const /*PROGMEM*/ char *TableZ8::listCpu_P() const {
    return TEXT_Z8_LIST;
}

const /*PROGMEM*/ char *TableZ8::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableZ8::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    auto t = Cpu::search(name, ARRAY_RANGE(CPU_TABLE));
    if (t) {
        cpuType = t->readCpuType();
    } else if (name.iexpectText_P(TEXT_CPU_Z86C)) {
        cpuType = Z86C;
    } else if (name.iexpectText_P(TEXT_CPU_Z86)) {
        cpuType = Z8;
    } else if (name.iexpectText_P(TEXT_CPU_Z88) || name.iequals_P(TEXT_CPU_SUPER8)) {
        cpuType = SUPER8;
    } else {
        return UNSUPPORTED_CPU;
    }
    return OK;
}

const TableZ8 TABLE;

}  // namespace z8
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

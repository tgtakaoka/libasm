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

#include "table_z80.h"
#include "entry_table.h"
#include "entry_z80.h"
#include "text_z80.h"

using namespace libasm::text::z80;

namespace libasm {
namespace z80 {

#define E2(_opc, _name, _dst, _src) {_opc, Entry::Flags::create(_dst, _src), _name}
#define E1(_opc, _name, _dst) E2(_opc, _name, _dst, M_NONE)
#define E0(_opc, _name) E1(_opc, _name, M_NONE)

// clang-format off
constexpr Entry TABLE_Z80[] PROGMEM = {
    E0(0x00, TEXT_NOP),
    E2(0x01, TEXT_LD,   M_R16, M_IM16),
    E2(0x09, TEXT_ADD,  R_HL,  M_R16),
    E2(0x02, TEXT_LD,   I_PTR, R_A),
    E2(0x0A, TEXT_LD,   R_A,   I_PTR),
    E2(0x22, TEXT_LD,   M_ABS, R_HL),
    E2(0x2A, TEXT_LD,   R_HL,  M_ABS),
    E2(0x32, TEXT_LD,   M_ABS, R_A),
    E2(0x3A, TEXT_LD,   R_A,   M_ABS),
    E1(0x03, TEXT_INC,  M_R16),
    E1(0x0B, TEXT_DEC,  M_R16),
    E1(0x04, TEXT_INC,  M_DST),
    E1(0x05, TEXT_DEC,  M_DST),
    E2(0x06, TEXT_LD,   M_DST, M_IM8),
    E0(0x07, TEXT_RLCA),
    E0(0x0F, TEXT_RRCA),
    E0(0x17, TEXT_RLA),
    E0(0x1F, TEXT_RRA),
    E0(0x27, TEXT_DAA),
    E0(0x2F, TEXT_CPL),
    E0(0x37, TEXT_SCF),
    E0(0x3F, TEXT_CCF),
    E0(0x76, TEXT_HALT),
    E2(0x40, TEXT_LD,   M_DST, M_SR8),
    E2(0x40, TEXT_LD,   M_DR8, M_SRC),
    E2(0x80, TEXT_ADD,  R_A,   M_SRC),
    E2(0x88, TEXT_ADC,  R_A,   M_SRC),
    E1(0x90, TEXT_SUB,  M_SRC),
    E2(0x90, TEXT_SUB,  R_A,   M_SRC),
    E2(0x98, TEXT_SBC,  R_A,   M_SRC),
    E1(0xA0, TEXT_AND,  M_SRC),
    E2(0xA0, TEXT_AND,  R_A,   M_SRC),
    E1(0xA8, TEXT_XOR,  M_SRC),
    E2(0xA8, TEXT_XOR,  R_A,   M_SRC),
    E1(0xB0, TEXT_OR,   M_SRC),
    E2(0xB0, TEXT_OR,   R_A,   M_SRC),
    E1(0xB8, TEXT_CP,   M_SRC),
    E2(0xB8, TEXT_CP,   R_A,   M_SRC),
    E1(0xC0, TEXT_RET,  M_CC8),
    E1(0xC1, TEXT_POP,  M_STK),
    E0(0xC9, TEXT_RET),
    E2(0xF9, TEXT_LD,   R_SP,  R_HL),
    E2(0xC2, TEXT_JP,   M_CC8, M_IM16),
    E1(0xC3, TEXT_JP,   M_IM16),
    E1(0xE9, TEXT_JP,   I_HL),
    E2(0xD3, TEXT_OUT,  M_IOA, R_A),
    E2(0xDB, TEXT_IN,   R_A,   M_IOA),
    E2(0xE3, TEXT_EX,   I_SP,  R_HL),
    E2(0xE3, TEXT_EX,   R_HL,  I_SP),
    E2(0xEB, TEXT_EX,   R_DE,  R_HL),
    E2(0xEB, TEXT_EX,   R_HL,  R_DE),
    E0(0xF3, TEXT_DI),
    E0(0xFB, TEXT_EI),
    E2(0xC4, TEXT_CALL, M_CC8, M_IM16),
    E1(0xC5, TEXT_PUSH, M_STK),
    E1(0xCD, TEXT_CALL, M_IM16),
    E2(0xC6, TEXT_ADD,  R_A,   M_IM8),
    E2(0xCE, TEXT_ADC,  R_A,   M_IM8),
    E1(0xD6, TEXT_SUB,  M_IM8),
    E2(0xD6, TEXT_SUB,  R_A,   M_IM8),
    E2(0xDE, TEXT_SBC,  R_A,   M_IM8),
    E1(0xE6, TEXT_AND,  M_IM8),
    E2(0xE6, TEXT_AND,  R_A,   M_IM8),
    E1(0xEE, TEXT_XOR,  M_IM8),
    E2(0xEE, TEXT_XOR,  R_A,   M_IM8),
    E1(0xF6, TEXT_OR,   M_IM8),
    E2(0xF6, TEXT_OR,   R_A,   M_IM8),
    E1(0xFE, TEXT_CP,   M_IM8),
    E2(0xFE, TEXT_CP,   R_A,   M_IM8),
    E1(0xC7, TEXT_RST,  M_VEC),
    E2(0x08, TEXT_EX,   R_AF,  R_AFP),
    E1(0x10, TEXT_DJNZ, M_REL8),
    E2(0x20, TEXT_JR,   M_CC4, M_REL8),
    E1(0x18, TEXT_JR,   M_REL8),
    E0(0xD9, TEXT_EXX),
};

constexpr uint8_t INDEX_Z80[] PROGMEM = {
     26,  // TEXT_ADC
     57,  // TEXT_ADC
      2,  // TEXT_ADD
     25,  // TEXT_ADD
     56,  // TEXT_ADD
     30,  // TEXT_AND
     31,  // TEXT_AND
     61,  // TEXT_AND
     62,  // TEXT_AND
     53,  // TEXT_CALL
     55,  // TEXT_CALL
     21,  // TEXT_CCF
     36,  // TEXT_CP
     37,  // TEXT_CP
     67,  // TEXT_CP
     68,  // TEXT_CP
     19,  // TEXT_CPL
     18,  // TEXT_DAA
     10,  // TEXT_DEC
     12,  // TEXT_DEC
     51,  // TEXT_DI
     71,  // TEXT_DJNZ
     52,  // TEXT_EI
     47,  // TEXT_EX
     48,  // TEXT_EX
     49,  // TEXT_EX
     50,  // TEXT_EX
     70,  // TEXT_EX
     74,  // TEXT_EXX
     22,  // TEXT_HALT
     46,  // TEXT_IN
      9,  // TEXT_INC
     11,  // TEXT_INC
     42,  // TEXT_JP
     43,  // TEXT_JP
     44,  // TEXT_JP
     72,  // TEXT_JR
     73,  // TEXT_JR
      1,  // TEXT_LD
      3,  // TEXT_LD
      4,  // TEXT_LD
      5,  // TEXT_LD
      6,  // TEXT_LD
      7,  // TEXT_LD
      8,  // TEXT_LD
     13,  // TEXT_LD
     23,  // TEXT_LD
     24,  // TEXT_LD
     41,  // TEXT_LD
      0,  // TEXT_NOP
     34,  // TEXT_OR
     35,  // TEXT_OR
     65,  // TEXT_OR
     66,  // TEXT_OR
     45,  // TEXT_OUT
     39,  // TEXT_POP
     54,  // TEXT_PUSH
     38,  // TEXT_RET
     40,  // TEXT_RET
     16,  // TEXT_RLA
     14,  // TEXT_RLCA
     17,  // TEXT_RRA
     15,  // TEXT_RRCA
     69,  // TEXT_RST
     29,  // TEXT_SBC
     60,  // TEXT_SBC
     20,  // TEXT_SCF
     27,  // TEXT_SUB
     28,  // TEXT_SUB
     58,  // TEXT_SUB
     59,  // TEXT_SUB
     32,  // TEXT_XOR
     33,  // TEXT_XOR
     63,  // TEXT_XOR
     64,  // TEXT_XOR
};

// CB:xx
constexpr Entry TABLE_BIT[] PROGMEM = {
    E1(0x00, TEXT_RLC, M_SRC),
    E1(0x08, TEXT_RRC, M_SRC),
    E1(0x10, TEXT_RL,  M_SRC),
    E1(0x18, TEXT_RR,  M_SRC),
    E1(0x20, TEXT_SLA, M_SRC),
    E1(0x28, TEXT_SRA, M_SRC),
    E1(0x38, TEXT_SRL, M_SRC),
    E2(0x40, TEXT_BIT, M_BIT, M_SRC),
    E2(0x80, TEXT_RES, M_BIT, M_SRC),
    E2(0xC0, TEXT_SET, M_BIT, M_SRC),
};

constexpr uint8_t INDEX_BIT[] PROGMEM = {
      7,  // TEXT_BIT
      8,  // TEXT_RES
      2,  // TEXT_RL
      0,  // TEXT_RLC
      3,  // TEXT_RR
      1,  // TEXT_RRC
      9,  // TEXT_SET
      4,  // TEXT_SLA
      5,  // TEXT_SRA
      6,  // TEXT_SRL
};

// ED:xx
constexpr Entry TABLE_EXT[] PROGMEM = {
    E2(0x40, TEXT_IN,   M_DR8,  I_C),
    E2(0x41, TEXT_OUT,  I_C,    M_DR8),
    E2(0x42, TEXT_SBC,  R_HL,   M_R16),
    E2(0x4A, TEXT_ADC,  R_HL,   M_R16),
    E2(0x43, TEXT_LD,   M_ABS,  M_NOHL),
    E2(0x4B, TEXT_LD,   M_NOHL, M_ABS),
    E0(0x44, TEXT_NEG),
    E0(0x45, TEXT_RETN),
    E0(0x4D, TEXT_RETI),
    E1(0x46, TEXT_IM,   M_IMMD),
    E2(0x47, TEXT_LD,   R_I,    R_A),
    E2(0x4F, TEXT_LD,   R_R,    R_A),
    E2(0x57, TEXT_LD,   R_A,    R_I),
    E2(0x5F, TEXT_LD,   R_A,    R_R),
    E0(0x67, TEXT_RRD),
    E0(0x6F, TEXT_RLD),
    E0(0xA0, TEXT_LDI),
    E0(0xA8, TEXT_LDD),
    E0(0xB0, TEXT_LDIR),
    E0(0xB8, TEXT_LDDR),
    E0(0xA1, TEXT_CPI),
    E0(0xA9, TEXT_CPD),
    E0(0xB1, TEXT_CPIR),
    E0(0xB9, TEXT_CPDR),
    E0(0xA2, TEXT_INI),
    E0(0xAA, TEXT_IND),
    E0(0xB2, TEXT_INIR),
    E0(0xBA, TEXT_INDR),
    E0(0xA3, TEXT_OUTI),
    E0(0xAB, TEXT_OUTD),
    E0(0xB3, TEXT_OTIR),
    E0(0xBB, TEXT_OTDR),
};

constexpr uint8_t INDEX_EXT[] PROGMEM = {
      3,  // TEXT_ADC
     21,  // TEXT_CPD
     23,  // TEXT_CPDR
     20,  // TEXT_CPI
     22,  // TEXT_CPIR
      9,  // TEXT_IM
      0,  // TEXT_IN
     25,  // TEXT_IND
     27,  // TEXT_INDR
     24,  // TEXT_INI
     26,  // TEXT_INIR
      4,  // TEXT_LD
      5,  // TEXT_LD
     10,  // TEXT_LD
     11,  // TEXT_LD
     12,  // TEXT_LD
     13,  // TEXT_LD
     17,  // TEXT_LDD
     19,  // TEXT_LDDR
     16,  // TEXT_LDI
     18,  // TEXT_LDIR
      6,  // TEXT_NEG
     31,  // TEXT_OTDR
     30,  // TEXT_OTIR
      1,  // TEXT_OUT
     29,  // TEXT_OUTD
     28,  // TEXT_OUTI
      8,  // TEXT_RETI
      7,  // TEXT_RETN
     15,  // TEXT_RLD
     14,  // TEXT_RRD
      2,  // TEXT_SBC
};

// Table for DD:xx, FD:xx
constexpr Entry TABLE_IDX[] PROGMEM = {
    E2(0x09, TEXT_ADD,  R_IDX,  M_R16X),
    E2(0x21, TEXT_LD,   R_IDX,  M_IM16),
    E2(0x22, TEXT_LD,   M_ABS,  R_IDX),
    E2(0x2A, TEXT_LD,   R_IDX,  M_ABS),
    E1(0x23, TEXT_INC,  R_IDX),
    E1(0x2B, TEXT_DEC,  R_IDX),
    E1(0x34, TEXT_INC,  M_IDX8),
    E1(0x35, TEXT_DEC,  M_IDX8),
    E2(0x36, TEXT_LD,   M_IDX8, M_IM8),
    E2(0x46, TEXT_LD,   M_DR8,  M_IDX8),
    E2(0x70, TEXT_LD,   M_IDX8, M_SR8),
    E2(0x86, TEXT_ADD,  R_A,    M_IDX8),
    E2(0x8E, TEXT_ADC,  R_A,    M_IDX8),
    E1(0x96, TEXT_SUB,  M_IDX8),
    E2(0x96, TEXT_SUB,  R_A,    M_IDX8),
    E2(0x9E, TEXT_SBC,  R_A,    M_IDX8),
    E1(0xA6, TEXT_AND,  M_IDX8),
    E2(0xA6, TEXT_AND,  R_A,    M_IDX8),
    E1(0xAE, TEXT_XOR,  M_IDX8),
    E2(0xAE, TEXT_XOR,  R_A,    M_IDX8),
    E1(0xB6, TEXT_OR,   M_IDX8),
    E2(0xB6, TEXT_OR,   R_A,    M_IDX8),
    E1(0xBE, TEXT_CP,   M_IDX8),
    E2(0xBE, TEXT_CP,   R_A,    M_IDX8),
    E1(0xE1, TEXT_POP,  R_IDX),
    E1(0xE9, TEXT_JP,   I_IDX),
    E2(0xF9, TEXT_LD,   R_SP,   R_IDX),
    E2(0xE3, TEXT_EX,   I_SP,   R_IDX),
    E2(0xE3, TEXT_EX,   R_IDX,  I_SP),
    E1(0xE5, TEXT_PUSH, R_IDX),
};

constexpr uint8_t INDEX_IDX[] PROGMEM = {
     12,  // TEXT_ADC
      0,  // TEXT_ADD
     11,  // TEXT_ADD
     16,  // TEXT_AND
     17,  // TEXT_AND
     22,  // TEXT_CP
     23,  // TEXT_CP
      5,  // TEXT_DEC
      7,  // TEXT_DEC
     27,  // TEXT_EX
     28,  // TEXT_EX
      4,  // TEXT_INC
      6,  // TEXT_INC
     25,  // TEXT_JP
      1,  // TEXT_LD
      2,  // TEXT_LD
      3,  // TEXT_LD
      8,  // TEXT_LD
      9,  // TEXT_LD
     10,  // TEXT_LD
     26,  // TEXT_LD
     20,  // TEXT_OR
     21,  // TEXT_OR
     24,  // TEXT_POP
     29,  // TEXT_PUSH
     15,  // TEXT_SBC
     13,  // TEXT_SUB
     14,  // TEXT_SUB
     18,  // TEXT_XOR
     19,  // TEXT_XOR
};

// DD:CB:dd:xx, FD:CB:dd:xx
constexpr Entry TABLE_IDXBIT[] PROGMEM = {
    E1(0x06, TEXT_RLC, M_IDX),
    E1(0x0E, TEXT_RRC, M_IDX),
    E1(0x16, TEXT_RL,  M_IDX),
    E1(0x1E, TEXT_RR,  M_IDX),
    E1(0x26, TEXT_SLA, M_IDX),
    E1(0x2E, TEXT_SRA, M_IDX),
    E1(0x3E, TEXT_SRL, M_IDX),
    E2(0x46, TEXT_BIT, M_BIT, M_IDX),
    E2(0x86, TEXT_RES, M_BIT, M_IDX),
    E2(0xC6, TEXT_SET, M_BIT, M_IDX),
};

constexpr uint8_t INDEX_IDXBIT[] PROGMEM = {
      7,  // TEXT_BIT
      8,  // TEXT_RES
      2,  // TEXT_RL
      0,  // TEXT_RLC
      3,  // TEXT_RR
      1,  // TEXT_RRC
      9,  // TEXT_SET
      4,  // TEXT_SLA
      5,  // TEXT_SRA
      6,  // TEXT_SRL
};

// Z180 ED:xx
constexpr Entry TABLE_Z180_EXT[] PROGMEM = {
    E1(0x4C, TEXT_MLT,    M_R16),
    E1(0x04, TEXT_TST,    M_DST),
    E1(0x64, TEXT_TST,    M_IM8),
    E1(0x74, TEXT_TSTIO,  M_IM8),
    E2(0x00, TEXT_IN0,    M_DR8, M_IOA),
    E2(0x01, TEXT_OUT0,   M_IOA, M_DR8),
    E0(0x83, TEXT_OTIM),
    E0(0x93, TEXT_OTIMR),
    E0(0x8B, TEXT_OTDM),
    E0(0x9B, TEXT_OTDMR),
    E0(0x76, TEXT_SLP),
};

constexpr uint8_t INDEX_Z180_EXT[] PROGMEM = {
      4,  // TEXT_IN0
      0,  // TEXT_MLT
      8,  // TEXT_OTDM
      9,  // TEXT_OTDMR
      6,  // TEXT_OTIM
      7,  // TEXT_OTIMR
      5,  // TEXT_OUT0
     10,  // TEXT_SLP
      1,  // TEXT_TST
      2,  // TEXT_TST
      3,  // TEXT_TSTIO
};

constexpr Entry TABLE_Z280[] PROGMEM = {
    E1(0x03, TEXT_INCW, M_R16),
    E1(0x0B, TEXT_DECW, M_R16),
    E1(0x80, TEXT_ADD,  M_SRC),
    E1(0x88, TEXT_ADC,  M_SRC),
    E1(0x98, TEXT_SBC,  M_SRC),
    E1(0xC6, TEXT_ADD,  M_IM8),
    E1(0xCE, TEXT_ADC,  M_IM8),
    E1(0xDE, TEXT_SBC,  M_IM8),
    E1(0x27, TEXT_DAA,  R_A),
    E1(0x2F, TEXT_CPL,  R_A),
    E2(0xF9, TEXT_LDW,  R_SP,  R_HL),
};

constexpr uint8_t INDEX_Z280[] PROGMEM = {
      3,  // TEXT_ADC
      6,  // TEXT_ADC
      2,  // TEXT_ADD
      5,  // TEXT_ADD
      9,  // TEXT_CPL
      8,  // TEXT_DAA
      1,  // TEXT_DECW
      0,  // TEXT_INCW
     10,  // TEXT_LDW
      4,  // TEXT_SBC
      7,  // TEXT_SBC
};

// Z280 ED:xx
constexpr Entry TABLE_Z280_EXT[] PROGMEM = {
    E2(0x02, TEXT_LDA,    R_HL,   M_FIDX),
    E2(0x03, TEXT_LD,     M_FIDX, R_A),
    E2(0x04, TEXT_LDW,    R_HL,   M_FIDX),
    E2(0x04, TEXT_LD,     R_HL,   M_FIDX),
    E2(0x05, TEXT_LDW,    M_FIDX, R_HL),
    E2(0x05, TEXT_LD,     M_FIDX, R_HL),
    E2(0x06, TEXT_LDW,    M_R16,  I_HL),
    E2(0x07, TEXT_EX,     R_A,    M_DST),
    E2(0x07, TEXT_EX,     M_DST,  R_A),
    E2(0x06, TEXT_LDW,    M_R16,  I_HL),
    E2(0x06, TEXT_LD,     M_R16,  I_HL),
    E2(0x0E, TEXT_LDW,    I_HL,   M_R16),
    E2(0x0E, TEXT_LD,     I_HL,   M_R16),
    E2(0x43, TEXT_LD ,    M_ABS,  M_NOHL),
    E2(0x43, TEXT_LDW,    M_ABS,  M_NOHL),
    E1(0x44, TEXT_NEG,    R_A),
    E2(0x4B, TEXT_LD,     M_NOHL, M_ABS),
    E2(0x4B, TEXT_LDW,    M_NOHL, M_ABS),
    E1(0x4C, TEXT_NEG,    R_HL),
    E2(0x63, TEXT_LDW,    M_ABS,  R_HL),
    E2(0x6B, TEXT_LDW,    R_HL,   M_ABS),
    E2(0xC0, TEXT_MULT,   R_A,    M_DST),
    E2(0xC1, TEXT_MULTU,  R_A,    M_DST),
    E2(0xC2, TEXT_MULTW,  R_HL,   M_R16),
    E2(0xC3, TEXT_MULTUW, R_HL,   M_R16),
    E2(0xC4, TEXT_DIV,    R_HL,   M_DST),
    E2(0xC5, TEXT_DIVU,   R_HL,   M_DST),
    E2(0xC6, TEXT_ADDW,   R_HL,   M_R16),
    E2(0xC7, TEXT_CPW,    R_HL,   M_R16),
    E2(0xCA, TEXT_DIVW,   R_DEHL, M_R16),
    E2(0xCB, TEXT_DIVUW,  R_DEHL, M_R16),
    E2(0xCE, TEXT_SUBW,   R_HL,   M_R16),
    E2(0xEF, TEXT_EX,     R_H,    R_L),
    E2(0xEF, TEXT_EX,     R_L,    R_H),
    E1(0xC0, TEXT_MULT,   M_DST),
    E1(0xC1, TEXT_MULTU,  M_DST),
    E1(0xC2, TEXT_MULTW,  M_R16),
    E1(0xC3, TEXT_MULTUW, M_R16),
    E1(0xC4, TEXT_DIV,    M_DST),
    E1(0xC5, TEXT_DIVU,   M_DST),
    E1(0xC6, TEXT_ADDW,   M_R16),
    E1(0xC7, TEXT_CPW,    M_R16),
    E1(0xCA, TEXT_DIVW,   M_R16),
    E1(0xCB, TEXT_DIVUW,  M_R16),
    E1(0xCE, TEXT_SUBW,   M_R16),
    E0(0x55, TEXT_RETIL),
    E1(0x64, TEXT_EXTS,   R_A),
    E0(0x64, TEXT_EXTS),
    E0(0x65, TEXT_PCACHE),
    E2(0x66, TEXT_LDCTL,  R_HL,   I_C),
    E1(0x6C, TEXT_EXTS,   R_HL),
    E2(0x6D, TEXT_ADD,    R_HL,   R_A),
    E2(0x6E, TEXT_LDCTL,  I_C,    R_HL),
    E1(0x70, TEXT_TSTI,   I_C),
    E1(0x71, TEXT_SC,     M_IM16),
    E1(0x77, TEXT_DI,     M_IM8),
    E1(0x7F, TEXT_EI,     M_IM8),
    E0(0x82, TEXT_INIW),
    E0(0x83, TEXT_OUTIW),
    E2(0x86, TEXT_LDUD,   R_A,    I_HL),
    E2(0x87, TEXT_LDCTL,  R_HL,   R_USP),
    E0(0x8A, TEXT_INDW),
    E0(0x8B, TEXT_OUTDW),
    E2(0x8E, TEXT_LDUD,   I_HL,   R_A),
    E2(0x8F, TEXT_LDCTL,  R_USP,  R_HL),
    E0(0x92, TEXT_INIRW),
    E0(0x93, TEXT_OTIRW),
    E2(0x96, TEXT_LDUP,   R_A,    I_HL),
    E0(0x9A, TEXT_INDRW),
    E0(0x9B, TEXT_OTDRW),
    E2(0x9E, TEXT_LDUP,   I_HL,   R_A),
    E2(0xB7, TEXT_INW,    R_HL,   I_C),
    E2(0xB7, TEXT_IN,     R_HL,   I_C),
    E2(0xBF, TEXT_OUTW,   I_C,    R_HL),
    E2(0xBF, TEXT_OUT,    I_C,    R_HL),
    E1(0x97, TEXT_EPUF,   M_EPU),
    E1(0x9F, TEXT_EPUI,   M_EPU),
    E2(0x84, TEXT_EPUM,   M_FIDX, M_EPU),
    E2(0x85, TEXT_MEPU,   M_FIDX, M_EPU),
    E2(0xA6, TEXT_EPUM,   I_HL,   M_EPU),
    E2(0xA7, TEXT_EPUM,   M_ABS,  M_EPU),
    E2(0xAE, TEXT_MEPU,   I_HL,   M_EPU),
    E2(0xAF, TEXT_MEPU,   M_ABS,  M_EPU),
};

constexpr uint8_t INDEX_Z280_EXT[] PROGMEM = {
     51,  // TEXT_ADD
     27,  // TEXT_ADDW
     40,  // TEXT_ADDW
     28,  // TEXT_CPW
     41,  // TEXT_CPW
     55,  // TEXT_DI
     25,  // TEXT_DIV
     38,  // TEXT_DIV
     26,  // TEXT_DIVU
     39,  // TEXT_DIVU
     30,  // TEXT_DIVUW
     43,  // TEXT_DIVUW
     29,  // TEXT_DIVW
     42,  // TEXT_DIVW
     56,  // TEXT_EI
     75,  // TEXT_EPUF
     76,  // TEXT_EPUI
     77,  // TEXT_EPUM
     79,  // TEXT_EPUM
     80,  // TEXT_EPUM
      7,  // TEXT_EX
      8,  // TEXT_EX
     32,  // TEXT_EX
     33,  // TEXT_EX
     46,  // TEXT_EXTS
     47,  // TEXT_EXTS
     50,  // TEXT_EXTS
     72,  // TEXT_IN
     68,  // TEXT_INDRW
     61,  // TEXT_INDW
     65,  // TEXT_INIRW
     57,  // TEXT_INIW
     71,  // TEXT_INW
      1,  // TEXT_LD
      3,  // TEXT_LD
      5,  // TEXT_LD
     10,  // TEXT_LD
     12,  // TEXT_LD
     13,  // TEXT_LD
     16,  // TEXT_LD
      0,  // TEXT_LDA
     49,  // TEXT_LDCTL
     52,  // TEXT_LDCTL
     60,  // TEXT_LDCTL
     64,  // TEXT_LDCTL
     59,  // TEXT_LDUD
     63,  // TEXT_LDUD
     67,  // TEXT_LDUP
     70,  // TEXT_LDUP
      2,  // TEXT_LDW
      4,  // TEXT_LDW
      6,  // TEXT_LDW
      9,  // TEXT_LDW
     11,  // TEXT_LDW
     14,  // TEXT_LDW
     17,  // TEXT_LDW
     19,  // TEXT_LDW
     20,  // TEXT_LDW
     78,  // TEXT_MEPU
     81,  // TEXT_MEPU
     82,  // TEXT_MEPU
     21,  // TEXT_MULT
     34,  // TEXT_MULT
     22,  // TEXT_MULTU
     35,  // TEXT_MULTU
     24,  // TEXT_MULTUW
     37,  // TEXT_MULTUW
     23,  // TEXT_MULTW
     36,  // TEXT_MULTW
     15,  // TEXT_NEG
     18,  // TEXT_NEG
     69,  // TEXT_OTDRW
     66,  // TEXT_OTIRW
     74,  // TEXT_OUT
     62,  // TEXT_OUTDW
     58,  // TEXT_OUTIW
     73,  // TEXT_OUTW
     48,  // TEXT_PCACHE
     45,  // TEXT_RETIL
     54,  // TEXT_SC
     31,  // TEXT_SUBW
     44,  // TEXT_SUBW
     53,  // TEXT_TSTI
};

// Z280 DD:xx
constexpr Entry TABLE_Z280_IX[] PROGMEM = {
    E2(0x01, TEXT_LDW,  M_MPTR, M_IM16),
    E1(0x03, TEXT_INCW, M_MPTR),
    E1(0x04, TEXT_INC,  M_BIXH),
    E1(0x05, TEXT_DEC,  M_BIXH),
    E2(0x06, TEXT_LD,   M_BIXH,  M_IM8),
    E1(0x0B, TEXT_DECW, M_MPTR),
    E1(0x20, TEXT_JAR,  M_REL8),
    E1(0x28, TEXT_JAF,  M_REL8),
    E2(0x31, TEXT_LDW,  M_REL16, M_IM16),
    E1(0x33, TEXT_INCW, M_REL16),
    E1(0x3B, TEXT_DECW, M_REL16),
    E1(0x3C, TEXT_INC,  M_ABS),
    E1(0x3D, TEXT_DEC,  M_ABS),
    E2(0x3E, TEXT_LD,   M_ABS,   M_IM8),
    E2(0x78, TEXT_LD,   R_A,     M_BIXL),
    E2(0x80, TEXT_ADD,  R_A,     M_BIXL),
    E2(0x88, TEXT_ADC,  R_A,     M_BIXL),
    E2(0x90, TEXT_SUB,  R_A,     M_BIXL),
    E2(0x98, TEXT_SBC,  R_A,     M_BIXL),
    E2(0xA0, TEXT_AND,  R_A,     M_BIXL),
    E2(0xA8, TEXT_XOR,  R_A,     M_BIXL),
    E2(0xB0, TEXT_OR,   R_A,     M_BIXL),
    E2(0xB8, TEXT_CP,   R_A,     M_BIXL),
    E1(0x78, TEXT_LD,   M_BIXL),
    E1(0x80, TEXT_ADD,  M_BIXL),
    E1(0x88, TEXT_ADC,  M_BIXL),
    E1(0x90, TEXT_SUB,  M_BIXL),
    E1(0x98, TEXT_SBC,  M_BIXL),
    E1(0xA0, TEXT_AND,  M_BIXL),
    E1(0xA8, TEXT_XOR,  M_BIXL),
    E1(0xB0, TEXT_OR,   M_BIXL),
    E1(0xB8, TEXT_CP,   M_BIXL),
    E1(0xC1, TEXT_POP,  M_MPTR),
    E2(0xC2, TEXT_JP,   M_CC8,   I_HL),
    E2(0xC4, TEXT_CALL, M_CC8,   I_HL),
    E1(0xC5, TEXT_PUSH, M_MPTR),
    E1(0xCD, TEXT_CALL, I_HL),
    E1(0xF1, TEXT_POP,  M_REL16),
    E1(0xF5, TEXT_PUSH, M_REL16),
};

constexpr uint8_t INDEX_Z280_IX[] PROGMEM = {
     16,  // TEXT_ADC
     25,  // TEXT_ADC
     15,  // TEXT_ADD
     24,  // TEXT_ADD
     19,  // TEXT_AND
     28,  // TEXT_AND
     34,  // TEXT_CALL
     36,  // TEXT_CALL
     22,  // TEXT_CP
     31,  // TEXT_CP
      3,  // TEXT_DEC
     12,  // TEXT_DEC
      5,  // TEXT_DECW
     10,  // TEXT_DECW
      2,  // TEXT_INC
     11,  // TEXT_INC
      1,  // TEXT_INCW
      9,  // TEXT_INCW
      7,  // TEXT_JAF
      6,  // TEXT_JAR
     33,  // TEXT_JP
      4,  // TEXT_LD
     13,  // TEXT_LD
     14,  // TEXT_LD
     23,  // TEXT_LD
      0,  // TEXT_LDW
      8,  // TEXT_LDW
     21,  // TEXT_OR
     30,  // TEXT_OR
     32,  // TEXT_POP
     37,  // TEXT_POP
     35,  // TEXT_PUSH
     38,  // TEXT_PUSH
     18,  // TEXT_SBC
     27,  // TEXT_SBC
     17,  // TEXT_SUB
     26,  // TEXT_SUB
     20,  // TEXT_XOR
     29,  // TEXT_XOR
};

// Z280 FD:xx
constexpr Entry TABLE_Z280_IY[] PROGMEM = {
    E1(0x03, TEXT_INCW, M_IDX16),
    E1(0x04, TEXT_INC,  M_PIXH),
    E1(0x05, TEXT_DEC,  M_PIXH),
    E2(0x06, TEXT_LD,   M_PIXH, M_IM8),
    E1(0x0B, TEXT_DECW, M_IDX16),
    E2(0x78, TEXT_LD,   R_A,    M_PIXL),
    E2(0x80, TEXT_ADD,  R_A,    M_PIXL),
    E2(0x88, TEXT_ADC,  R_A,    M_PIXL),
    E2(0x90, TEXT_SUB,  R_A,    M_PIXL),
    E2(0x98, TEXT_SBC,  R_A,    M_PIXL),
    E2(0xA0, TEXT_AND,  R_A,    M_PIXL),
    E2(0xA8, TEXT_XOR,  R_A,    M_PIXL),
    E2(0xB0, TEXT_OR,   R_A,    M_PIXL),
    E2(0xB8, TEXT_CP,   R_A,    M_PIXL),
    E1(0x78, TEXT_LD,   M_PIXL),
    E1(0x80, TEXT_ADD,  M_PIXL),
    E1(0x88, TEXT_ADC,  M_PIXL),
    E1(0x90, TEXT_SUB,  M_PIXL),
    E1(0x98, TEXT_SBC,  M_PIXL),
    E1(0xA0, TEXT_AND,  M_PIXL),
    E1(0xA8, TEXT_XOR,  M_PIXL),
    E1(0xB0, TEXT_OR,   M_PIXL),
    E1(0xB8, TEXT_CP,   M_PIXL),
    E2(0xC2, TEXT_JP,   M_CC8,  M_REL16),
    E1(0xC3, TEXT_JP,   M_REL16),
    E2(0xC4, TEXT_CALL, M_CC8,  M_REL16),
    E1(0xCD, TEXT_CALL, M_REL16),
    E1(0xF5, TEXT_PUSH, M_IM16),
};

constexpr uint8_t INDEX_Z280_IY[] PROGMEM = {
      7,  // TEXT_ADC
     16,  // TEXT_ADC
      6,  // TEXT_ADD
     15,  // TEXT_ADD
     10,  // TEXT_AND
     19,  // TEXT_AND
     25,  // TEXT_CALL
     26,  // TEXT_CALL
     13,  // TEXT_CP
     22,  // TEXT_CP
      2,  // TEXT_DEC
      4,  // TEXT_DECW
      1,  // TEXT_INC
      0,  // TEXT_INCW
     23,  // TEXT_JP
     24,  // TEXT_JP
      3,  // TEXT_LD
      5,  // TEXT_LD
     14,  // TEXT_LD
     12,  // TEXT_OR
     21,  // TEXT_OR
     27,  // TEXT_PUSH
      9,  // TEXT_SBC
     18,  // TEXT_SBC
      8,  // TEXT_SUB
     17,  // TEXT_SUB
     11,  // TEXT_XOR
     20,  // TEXT_XOR
};

// Z280 CB:xx
constexpr Entry TABLE_Z280_BIT[] PROGMEM = {
    E1(0x30, TEXT_TSET, M_SRC),
};

constexpr uint8_t INDEX_Z280_BIT[] PROGMEM = {
      0,  // TEXT_TSET
};

// Z280 DD:xx, FD:xx
constexpr Entry TABLE_Z280_IDX[] PROGMEM = {
    E1(0x24, TEXT_INC,  R_DXY),
    E1(0x25, TEXT_DEC,  R_DXY),
    E1(0x23, TEXT_INCW, R_IDX),
    E1(0x2B, TEXT_DECW, R_IDX),
    E2(0x26, TEXT_LD,   R_DXY,  M_IM8),
    E2(0x64, TEXT_LD,   R_DXY,  R_SXY),
    E2(0x44, TEXT_LD,   M_DR8X, R_SXY),
    E2(0x60, TEXT_LD,   R_DXY,  M_SR8X),
    E2(0xEB, TEXT_EX,   R_IDX,  R_HL),
    E2(0x84, TEXT_ADD,  R_A,    R_SXY),
    E1(0x86, TEXT_ADD,  M_IDX8),
    E1(0x8E, TEXT_ADC,  M_IDX8),
    E1(0x9E, TEXT_SBC,  M_IDX8),
    E2(0x8C, TEXT_ADC,  R_A,    R_SXY),
    E2(0x94, TEXT_SUB,  R_A,    R_SXY),
    E2(0x9C, TEXT_SBC,  R_A,    R_SXY),
    E2(0xA4, TEXT_AND,  R_A,    R_SXY),
    E2(0xAC, TEXT_XOR,  R_A,    R_SXY),
    E2(0xB4, TEXT_OR,   R_A,    R_SXY),
    E2(0xBC, TEXT_CP,   R_A,    R_SXY),
    E1(0x84, TEXT_ADD,  R_SXY),
    E1(0x8C, TEXT_ADC,  R_SXY),
    E1(0x94, TEXT_SUB,  R_SXY),
    E1(0x9C, TEXT_SBC,  R_SXY),
    E1(0xA4, TEXT_AND,  R_SXY),
    E1(0xAC, TEXT_XOR,  R_SXY),
    E1(0xB4, TEXT_OR,   R_SXY),
    E1(0xBC, TEXT_CP,   R_SXY),
    E2(0xEB, TEXT_EX,   R_HL,   R_IDX),
    E2(0xF9, TEXT_LDW,  R_SP,   R_IDX),
};

constexpr uint8_t INDEX_Z280_IDX[] PROGMEM = {
     11,  // TEXT_ADC
     13,  // TEXT_ADC
     21,  // TEXT_ADC
      9,  // TEXT_ADD
     10,  // TEXT_ADD
     20,  // TEXT_ADD
     16,  // TEXT_AND
     24,  // TEXT_AND
     19,  // TEXT_CP
     27,  // TEXT_CP
      1,  // TEXT_DEC
      3,  // TEXT_DECW
      8,  // TEXT_EX
     28,  // TEXT_EX
      0,  // TEXT_INC
      2,  // TEXT_INCW
      4,  // TEXT_LD
      5,  // TEXT_LD
      6,  // TEXT_LD
      7,  // TEXT_LD
     29,  // TEXT_LDW
     18,  // TEXT_OR
     26,  // TEXT_OR
     12,  // TEXT_SBC
     15,  // TEXT_SBC
     23,  // TEXT_SBC
     14,  // TEXT_SUB
     22,  // TEXT_SUB
     17,  // TEXT_XOR
     25,  // TEXT_XOR
};

// Z280 DD:ED:xx, FD:ED:xx
constexpr Entry TABLE_Z280_IDXEXT[] PROGMEM = {
    E2(0x02, TEXT_LDA,    R_IDX,  M_FIDX),
    E2(0x04, TEXT_LDW,    R_IDX,  M_FIDX),
    E2(0x04, TEXT_LD,     R_IDX,  M_FIDX),
    E2(0x05, TEXT_LDW,    M_FIDX, R_IDX),
    E2(0x05, TEXT_LD,     M_FIDX, R_IDX),
    E2(0x06, TEXT_LDW,    M_R16,  M_IDX8),
    E2(0x06, TEXT_LD,     M_R16,  M_IDX8),
    E2(0x0E, TEXT_LDW,    M_IDX8, M_R16),
    E2(0x0E, TEXT_LD,     M_IDX8, M_R16),
    E2(0x27, TEXT_EX,     R_A,    R_DXY),
    E2(0x27, TEXT_EX,     R_DXY,  R_A),
    E2(0x37, TEXT_EX,     R_A,    M_IDX8),
    E2(0x37, TEXT_EX,     M_IDX8, R_A),
    E2(0x42, TEXT_SBC,    R_IDX,  M_R16X),
    E2(0x4A, TEXT_ADC,    R_IDX,  M_R16X),
    E2(0x60, TEXT_IN,     R_DXY,  I_C),
    E2(0x61, TEXT_OUT,    I_C,    R_DXY),
    E2(0x66, TEXT_LDCTL,  R_IDX,  I_C),
    E2(0x6D, TEXT_ADD,    R_IDX,  R_A),
    E2(0x6E, TEXT_LDCTL,  I_C,    R_IDX),
    E2(0x86, TEXT_LDUD,   R_A,    M_IDX8),
    E2(0x87, TEXT_LDCTL,  R_IDX,  R_USP),
    E2(0x8E, TEXT_LDUD,   M_IDX8, R_A),
    E2(0x8F, TEXT_LDCTL,  R_USP,  R_IDX),
    E2(0x96, TEXT_LDUP,   R_A,    M_IDX8),
    E2(0x9E, TEXT_LDUP,   M_IDX8, R_A),
    E2(0xE0, TEXT_MULT,   R_A,    R_DXY),
    E2(0xE1, TEXT_MULTU,  R_A,    R_DXY),
    E2(0xE2, TEXT_MULTW,  R_HL,   R_IDX),
    E2(0xE3, TEXT_MULTUW, R_HL,   R_IDX),
    E2(0xE4, TEXT_DIV,    R_HL,   R_DXY),
    E2(0xE5, TEXT_DIVU,   R_HL,   R_DXY),
    E2(0xE6, TEXT_ADDW,   R_HL,   R_IDX),
    E2(0xE7, TEXT_CPW,    R_HL,   R_IDX),
    E2(0xEA, TEXT_DIVW,   R_DEHL, R_IDX),
    E2(0xEB, TEXT_DIVUW,  R_DEHL, R_IDX),
    E2(0xEE, TEXT_SUBW,   R_HL,   R_IDX),
    E2(0xF0, TEXT_MULT,   R_A,    M_IDX8),
    E2(0xF1, TEXT_MULTU,  R_A,    M_IDX8),
    E2(0xF4, TEXT_DIV,    R_HL,   M_IDX8),
    E2(0xF5, TEXT_DIVU,   R_HL,   M_IDX8),
    E1(0xE0, TEXT_MULT,   R_DXY),
    E1(0xE1, TEXT_MULTU,  R_DXY),
    E1(0xE2, TEXT_MULTW,  R_IDX),
    E1(0xE3, TEXT_MULTUW, R_IDX),
    E1(0xE4, TEXT_DIV,    R_DXY),
    E1(0xE5, TEXT_DIVU,   R_DXY),
    E1(0xE6, TEXT_ADDW,   R_IDX),
    E1(0xE7, TEXT_CPW,    R_IDX),
    E1(0xEA, TEXT_DIVW,   R_IDX),
    E1(0xEB, TEXT_DIVUW,  R_IDX),
    E1(0xEE, TEXT_SUBW,   R_IDX),
    E1(0xF0, TEXT_MULT,   M_IDX8),
    E1(0xF1, TEXT_MULTU,  M_IDX8),
    E1(0xF4, TEXT_DIV,    M_IDX8),
    E1(0xF5, TEXT_DIVU,   M_IDX8),
};

constexpr uint8_t INDEX_Z280_IDXEXT[] PROGMEM = {
     14,  // TEXT_ADC
     18,  // TEXT_ADD
     32,  // TEXT_ADDW
     47,  // TEXT_ADDW
     33,  // TEXT_CPW
     48,  // TEXT_CPW
     30,  // TEXT_DIV
     39,  // TEXT_DIV
     45,  // TEXT_DIV
     54,  // TEXT_DIV
     31,  // TEXT_DIVU
     40,  // TEXT_DIVU
     46,  // TEXT_DIVU
     55,  // TEXT_DIVU
     35,  // TEXT_DIVUW
     50,  // TEXT_DIVUW
     34,  // TEXT_DIVW
     49,  // TEXT_DIVW
      9,  // TEXT_EX
     10,  // TEXT_EX
     11,  // TEXT_EX
     12,  // TEXT_EX
     15,  // TEXT_IN
      2,  // TEXT_LD
      4,  // TEXT_LD
      6,  // TEXT_LD
      8,  // TEXT_LD
      0,  // TEXT_LDA
     17,  // TEXT_LDCTL
     19,  // TEXT_LDCTL
     21,  // TEXT_LDCTL
     23,  // TEXT_LDCTL
     20,  // TEXT_LDUD
     22,  // TEXT_LDUD
     24,  // TEXT_LDUP
     25,  // TEXT_LDUP
      1,  // TEXT_LDW
      3,  // TEXT_LDW
      5,  // TEXT_LDW
      7,  // TEXT_LDW
     26,  // TEXT_MULT
     37,  // TEXT_MULT
     41,  // TEXT_MULT
     52,  // TEXT_MULT
     27,  // TEXT_MULTU
     38,  // TEXT_MULTU
     42,  // TEXT_MULTU
     53,  // TEXT_MULTU
     29,  // TEXT_MULTUW
     44,  // TEXT_MULTUW
     28,  // TEXT_MULTW
     43,  // TEXT_MULTW
     16,  // TEXT_OUT
     13,  // TEXT_SBC
     36,  // TEXT_SUBW
     51,  // TEXT_SUBW
};

constexpr Entry TABLE_Z280_IXEXT[] PROGMEM = {
    E2(0x07, TEXT_EX,     R_A,    M_BIXH),
    E2(0x07, TEXT_EX,     M_BIXH, R_A),
    E2(0x3F, TEXT_EX,     R_A,    M_ABS),
    E2(0x3F, TEXT_EX,     M_ABS,  R_A),
    E2(0x40, TEXT_IN,     M_BIXH, I_C),
    E2(0x41, TEXT_OUT,    I_C,    M_BIXH),
    E2(0x78, TEXT_IN,     M_ABS,  I_C),
    E2(0x79, TEXT_OUT,    I_C,    M_ABS),
    E2(0xC0, TEXT_MULT,   R_A,    M_BIXH),
    E2(0xC1, TEXT_MULTU,  R_A,    M_BIXH),
    E2(0xC2, TEXT_MULTW,  R_HL,   M_MPTR),
    E2(0xC3, TEXT_MULTUW, R_HL,   M_MPTR),
    E2(0xC4, TEXT_DIV,    R_HL,   M_BIXH),
    E2(0xC5, TEXT_DIVU,   R_HL,   M_BIXH),
    E2(0xC6, TEXT_ADDW,   R_HL,   M_MPTR),
    E2(0xC7, TEXT_CPW,    R_HL,   M_MPTR),
    E2(0xCA, TEXT_DIVW,   R_DEHL, M_MPTR),
    E2(0xCB, TEXT_DIVUW,  R_DEHL, M_MPTR),
    E2(0xCE, TEXT_SUBW,   R_HL,   M_MPTR),
    E2(0xF2, TEXT_MULTW,  R_HL,   M_REL16),
    E2(0xF3, TEXT_MULTUW, R_HL,   M_REL16),
    E2(0xF6, TEXT_ADDW,   R_HL,   M_REL16),
    E2(0xF7, TEXT_CPW,    R_HL,   M_REL16),
    E2(0xF8, TEXT_MULT,   R_A,    M_ABS),
    E2(0xF9, TEXT_MULTU,  R_A,    M_ABS),
    E2(0xFA, TEXT_DIVW,   R_DEHL, M_REL16),
    E2(0xFB, TEXT_DIVUW,  R_DEHL, M_REL16),
    E2(0xFC, TEXT_DIV,    R_HL,   M_ABS),
    E2(0xFD, TEXT_DIVU,   R_HL,   M_ABS),
    E2(0xFE, TEXT_SUBW,   R_HL,   M_REL16),
    E1(0xC0, TEXT_MULT,   M_BIXH),
    E1(0xC1, TEXT_MULTU,  M_BIXH),
    E1(0xC2, TEXT_MULTW,  M_MPTR),
    E1(0xC3, TEXT_MULTUW, M_MPTR),
    E1(0xC4, TEXT_DIV,    M_BIXH),
    E1(0xC5, TEXT_DIVU,   M_BIXH),
    E1(0xC6, TEXT_ADDW,   M_MPTR),
    E1(0xC7, TEXT_CPW,    M_MPTR),
    E1(0xCA, TEXT_DIVW,   M_MPTR),
    E1(0xCB, TEXT_DIVUW,  M_MPTR),
    E1(0xCE, TEXT_SUBW,   M_MPTR),
    E1(0xF2, TEXT_MULTW,  M_REL16),
    E1(0xF3, TEXT_MULTUW, M_REL16),
    E1(0xF6, TEXT_ADDW,   M_REL16),
    E1(0xF7, TEXT_CPW,    M_REL16),
    E1(0xF8, TEXT_MULT,   M_ABS),
    E1(0xF9, TEXT_MULTU,  M_ABS),
    E1(0xFA, TEXT_DIVW,   M_REL16),
    E1(0xFB, TEXT_DIVUW,  M_REL16),
    E1(0xFC, TEXT_DIV,    M_ABS),
    E1(0xFD, TEXT_DIVU,   M_ABS),
    E1(0xFE, TEXT_SUBW,   M_REL16),
};

constexpr uint8_t INDEX_Z280_IXEXT[] PROGMEM = {
     14,  // TEXT_ADDW
     21,  // TEXT_ADDW
     36,  // TEXT_ADDW
     43,  // TEXT_ADDW
     15,  // TEXT_CPW
     22,  // TEXT_CPW
     37,  // TEXT_CPW
     44,  // TEXT_CPW
     12,  // TEXT_DIV
     27,  // TEXT_DIV
     34,  // TEXT_DIV
     49,  // TEXT_DIV
     13,  // TEXT_DIVU
     28,  // TEXT_DIVU
     35,  // TEXT_DIVU
     50,  // TEXT_DIVU
     17,  // TEXT_DIVUW
     26,  // TEXT_DIVUW
     39,  // TEXT_DIVUW
     48,  // TEXT_DIVUW
     16,  // TEXT_DIVW
     25,  // TEXT_DIVW
     38,  // TEXT_DIVW
     47,  // TEXT_DIVW
      0,  // TEXT_EX
      1,  // TEXT_EX
      2,  // TEXT_EX
      3,  // TEXT_EX
      4,  // TEXT_IN
      6,  // TEXT_IN
      8,  // TEXT_MULT
     23,  // TEXT_MULT
     30,  // TEXT_MULT
     45,  // TEXT_MULT
      9,  // TEXT_MULTU
     24,  // TEXT_MULTU
     31,  // TEXT_MULTU
     46,  // TEXT_MULTU
     11,  // TEXT_MULTUW
     20,  // TEXT_MULTUW
     33,  // TEXT_MULTUW
     42,  // TEXT_MULTUW
     10,  // TEXT_MULTW
     19,  // TEXT_MULTW
     32,  // TEXT_MULTW
     41,  // TEXT_MULTW
      5,  // TEXT_OUT
      7,  // TEXT_OUT
     18,  // TEXT_SUBW
     29,  // TEXT_SUBW
     40,  // TEXT_SUBW
     51,  // TEXT_SUBW
};

constexpr Entry TABLE_Z280_IYEXT[] PROGMEM = {
    E2(0x07, TEXT_EX,     R_A,    M_PIXH),
    E2(0x40, TEXT_IN,     M_PIXH, I_C),
    E2(0x41, TEXT_OUT,    I_C,    M_PIXH),
    E2(0xC0, TEXT_MULT,   R_A,    M_PIXH),
    E2(0xC1, TEXT_MULTU,  R_A,    M_PIXH),
    E2(0xC2, TEXT_MULTW,  R_HL,   M_IDX16),
    E2(0xC3, TEXT_MULTUW, R_HL,   M_IDX16),
    E2(0xC4, TEXT_DIV,    R_HL,   M_PIXH),
    E2(0xC5, TEXT_DIVU,   R_HL,   M_PIXH),
    E2(0xC6, TEXT_ADDW,   R_HL,   M_IDX16),
    E2(0xC7, TEXT_CPW,    R_HL,   M_IDX16),
    E2(0xCA, TEXT_DIVW,   R_DEHL, M_IDX16),
    E2(0xCB, TEXT_DIVUW,  R_DEHL, M_IDX16),
    E2(0xCE, TEXT_SUBW,   R_HL,   M_IDX16),
    E2(0xF2, TEXT_MULTW,  R_HL,   M_IM16),
    E2(0xF3, TEXT_MULTUW, R_HL,   M_IM16),
    E2(0xF6, TEXT_ADDW,   R_HL,   M_IM16),
    E2(0xF7, TEXT_CPW,    R_HL,   M_IM16),
    E2(0xF8, TEXT_MULT,   R_A,    M_IM8),
    E2(0xF9, TEXT_MULTU,  R_A,    M_IM8),
    E2(0xFA, TEXT_DIVW,   R_DEHL, M_IM16),
    E2(0xFB, TEXT_DIVUW,  R_DEHL, M_IM16),
    E2(0xFC, TEXT_DIV,    R_HL,   M_IM8),
    E2(0xFD, TEXT_DIVU,   R_HL,   M_IM8),
    E2(0xFE, TEXT_SUBW,   R_HL,   M_IM16),
    E1(0xC0, TEXT_MULT,   M_PIXH),
    E1(0xC1, TEXT_MULTU,  M_PIXH),
    E1(0xC2, TEXT_MULTW,  M_IDX16),
    E1(0xC3, TEXT_MULTUW, M_IDX16),
    E1(0xC4, TEXT_DIV,    M_PIXH),
    E1(0xC5, TEXT_DIVU,   M_PIXH),
    E1(0xC6, TEXT_ADDW,   M_IDX16),
    E1(0xC7, TEXT_CPW,    M_IDX16),
    E1(0xCA, TEXT_DIVW,   M_IDX16),
    E1(0xCB, TEXT_DIVUW,  M_IDX16),
    E1(0xCE, TEXT_SUBW,   M_IDX16),
    E1(0xF2, TEXT_MULTW,  M_IM16),
    E1(0xF3, TEXT_MULTUW, M_IM16),
    E1(0xF6, TEXT_ADDW,   M_IM16),
    E1(0xF7, TEXT_CPW,    M_IM16),
    E1(0xF8, TEXT_MULT,   M_IM8),
    E1(0xF9, TEXT_MULTU,  M_IM8),
    E1(0xFA, TEXT_DIVW,   M_IM16),
    E1(0xFB, TEXT_DIVUW,  M_IM16),
    E1(0xFC, TEXT_DIV,    M_IM8),
    E1(0xFD, TEXT_DIVU,   M_IM8),
    E1(0xFE, TEXT_SUBW,   M_IM16),
};

constexpr uint8_t INDEX_Z280_IYEXT[] PROGMEM = {
      9,  // TEXT_ADDW
     16,  // TEXT_ADDW
     31,  // TEXT_ADDW
     38,  // TEXT_ADDW
     10,  // TEXT_CPW
     17,  // TEXT_CPW
     32,  // TEXT_CPW
     39,  // TEXT_CPW
      7,  // TEXT_DIV
     22,  // TEXT_DIV
     29,  // TEXT_DIV
     44,  // TEXT_DIV
      8,  // TEXT_DIVU
     23,  // TEXT_DIVU
     30,  // TEXT_DIVU
     45,  // TEXT_DIVU
     12,  // TEXT_DIVUW
     21,  // TEXT_DIVUW
     34,  // TEXT_DIVUW
     43,  // TEXT_DIVUW
     11,  // TEXT_DIVW
     20,  // TEXT_DIVW
     33,  // TEXT_DIVW
     42,  // TEXT_DIVW
      0,  // TEXT_EX
      1,  // TEXT_IN
      3,  // TEXT_MULT
     18,  // TEXT_MULT
     25,  // TEXT_MULT
     40,  // TEXT_MULT
      4,  // TEXT_MULTU
     19,  // TEXT_MULTU
     26,  // TEXT_MULTU
     41,  // TEXT_MULTU
      6,  // TEXT_MULTUW
     15,  // TEXT_MULTUW
     28,  // TEXT_MULTUW
     37,  // TEXT_MULTUW
      5,  // TEXT_MULTW
     14,  // TEXT_MULTW
     27,  // TEXT_MULTW
     36,  // TEXT_MULTW
      2,  // TEXT_OUT
     13,  // TEXT_SUBW
     24,  // TEXT_SUBW
     35,  // TEXT_SUBW
     46,  // TEXT_SUBW
};

// Z280 DD:CB:dd:xx, FD:CB:dd:xx
constexpr Entry TABLE_Z280_IDXBIT[] PROGMEM = {
    E1(0x36, TEXT_TSET, M_IDX),
};

constexpr uint8_t INDEX_Z280_IDXBIT[] PROGMEM = {
      0,  // TEXT_TSET
};

// clang-format on

using EntryPage = entry::PrefixTableBase<Entry>;

constexpr EntryPage Z80_PAGES[] PROGMEM = {
        {0x0000, ARRAY_RANGE(TABLE_Z80), ARRAY_RANGE(INDEX_Z80)},
        {TableZ80::EXT, ARRAY_RANGE(TABLE_EXT), ARRAY_RANGE(INDEX_EXT)},
        {TableZ80::BIT, ARRAY_RANGE(TABLE_BIT), ARRAY_RANGE(INDEX_BIT)},
        {TableZ80::IX, ARRAY_RANGE(TABLE_IDX), ARRAY_RANGE(INDEX_IDX)},
        {TableZ80::IY, ARRAY_RANGE(TABLE_IDX), ARRAY_RANGE(INDEX_IDX)},
        {TableZ80::IXBIT, ARRAY_RANGE(TABLE_IDXBIT), ARRAY_RANGE(INDEX_IDXBIT)},
        {TableZ80::IYBIT, ARRAY_RANGE(TABLE_IDXBIT), ARRAY_RANGE(INDEX_IDXBIT)},
};

constexpr EntryPage Z180_PAGES[] PROGMEM = {
        {0x0000, ARRAY_RANGE(TABLE_Z80), ARRAY_RANGE(INDEX_Z80)},
        {TableZ80::EXT, ARRAY_RANGE(TABLE_EXT), ARRAY_RANGE(INDEX_EXT)},
        {TableZ80::EXT, ARRAY_RANGE(TABLE_Z180_EXT), ARRAY_RANGE(INDEX_Z180_EXT)},
        {TableZ80::BIT, ARRAY_RANGE(TABLE_BIT), ARRAY_RANGE(INDEX_BIT)},
        {TableZ80::IX, ARRAY_RANGE(TABLE_IDX), ARRAY_RANGE(INDEX_IDX)},
        {TableZ80::IY, ARRAY_RANGE(TABLE_IDX), ARRAY_RANGE(INDEX_IDX)},
        {TableZ80::IXBIT, ARRAY_RANGE(TABLE_IDXBIT), ARRAY_RANGE(INDEX_IDXBIT)},
        {TableZ80::IYBIT, ARRAY_RANGE(TABLE_IDXBIT), ARRAY_RANGE(INDEX_IDXBIT)},
};

constexpr EntryPage Z280_PAGES[] PROGMEM = {
        {0x0000, ARRAY_RANGE(TABLE_Z80), ARRAY_RANGE(INDEX_Z80)},
        {0x0000, ARRAY_RANGE(TABLE_Z280), ARRAY_RANGE(INDEX_Z280)},
        {TableZ80::IX, ARRAY_RANGE(TABLE_IDX), ARRAY_RANGE(INDEX_IDX)},
        {TableZ80::IY, ARRAY_RANGE(TABLE_IDX), ARRAY_RANGE(INDEX_IDX)},
        {TableZ80::IX, ARRAY_RANGE(TABLE_Z280_IDX), ARRAY_RANGE(INDEX_Z280_IDX)},
        {TableZ80::IY, ARRAY_RANGE(TABLE_Z280_IDX), ARRAY_RANGE(INDEX_Z280_IDX)},
        {TableZ80::IX, ARRAY_RANGE(TABLE_Z280_IX), ARRAY_RANGE(INDEX_Z280_IX)},
        {TableZ80::IY, ARRAY_RANGE(TABLE_Z280_IY), ARRAY_RANGE(INDEX_Z280_IY)},
        {TableZ80::EXT, ARRAY_RANGE(TABLE_EXT), ARRAY_RANGE(INDEX_EXT)},
        {TableZ80::EXT, ARRAY_RANGE(TABLE_Z280_EXT), ARRAY_RANGE(INDEX_Z280_EXT)},
        {TableZ80::IXEXT, ARRAY_RANGE(TABLE_Z280_IDXEXT), ARRAY_RANGE(INDEX_Z280_IDXEXT)},
        {TableZ80::IYEXT, ARRAY_RANGE(TABLE_Z280_IDXEXT), ARRAY_RANGE(INDEX_Z280_IDXEXT)},
        {TableZ80::IXEXT, ARRAY_RANGE(TABLE_Z280_IXEXT), ARRAY_RANGE(INDEX_Z280_IXEXT)},
        {TableZ80::IYEXT, ARRAY_RANGE(TABLE_Z280_IYEXT), ARRAY_RANGE(INDEX_Z280_IYEXT)},
        {TableZ80::BIT, ARRAY_RANGE(TABLE_BIT), ARRAY_RANGE(INDEX_BIT)},
        {TableZ80::BIT, ARRAY_RANGE(TABLE_Z280_BIT), ARRAY_RANGE(INDEX_Z280_BIT)},
        {TableZ80::IXBIT, ARRAY_RANGE(TABLE_IDXBIT), ARRAY_RANGE(INDEX_IDXBIT)},
        {TableZ80::IYBIT, ARRAY_RANGE(TABLE_IDXBIT), ARRAY_RANGE(INDEX_IDXBIT)},
        {TableZ80::IXBIT, ARRAY_RANGE(TABLE_Z280_IDXBIT), ARRAY_RANGE(INDEX_Z280_IDXBIT)},
        {TableZ80::IYBIT, ARRAY_RANGE(TABLE_Z280_IDXBIT), ARRAY_RANGE(INDEX_Z280_IDXBIT)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {Z80, TEXT_CPU_Z80, ARRAY_RANGE(Z80_PAGES)},
        {Z180, TEXT_CPU_Z180, ARRAY_RANGE(Z180_PAGES)},
        {Z280, TEXT_CPU_Z280, ARRAY_RANGE(Z280_PAGES)},
};

const Cpu *cpu(CpuType cpuType) {
    return Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
}

bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_SR8 || opr == R_A)
        return table == M_SRC || table == M_DST || table == M_SR8 || table == M_DR8 ||
               table == M_SR8X || table == M_DR8X || table == R_H || table == R_L;
    if (opr == I_HL)
        return table == M_SRC || table == M_DST || table == M_MPTR;
    if (opr == R_C)
        return table == M_SRC || table == M_DST || table == M_SR8 || table == M_DR8 ||
               table == M_SR8X || table == M_DR8X || table == M_CC4 || table == M_CC8;
    if (opr == R_H || opr == R_L)
        return table == M_SRC || table == M_DST || table == M_SR8 || table == M_DR8 ||
               table == M_SR8X || table == M_DR8X;
    if (opr == R_DXY)
        return table == R_SXY;
    if (opr == R_BC || opr == R_DE)
        return table == M_R16 || table == M_R16X || table == M_NOHL || table == M_STK;
    if (opr == R_HL)
        return table == M_R16 || table == M_R16X || table == M_STK;
    if (opr == R_SP)
        return table == M_R16 || table == M_R16X || table == M_NOHL;
    if (opr == R_AF)
        return table == M_STK;
    if (opr == R_IDX)
        return table == M_R16X;
    if (opr == M_REL16 || opr == M_PCDX)
        return table == M_FIDX || table == M_PIXH || table == M_PIXL;
    if (opr == M_IDX8)
        return table == M_FIDX || table == M_PIXH || table == M_PIXL || table == M_IDX;
    if (opr == M_IDX16)
        return table == M_FIDX || table == M_PIXH || table == M_PIXL || table == M_IDX;
    if (opr == M_PDX)
        return table == M_FIDX || table == M_PIXH || table == M_PIXL;
    if (opr == M_SPDX)
        return table == M_FIDX || table == M_BIXH || table == M_BIXL;
    if (opr == M_BDX)
        return table == M_FIDX || table == M_BIXH || table == M_BIXL;
    if (opr == M_IM16)
        return table == M_IM8 || table == M_REL8 || table == M_BIT || table == M_VEC ||
               table == M_IMMD || table == M_EPU;
    if (opr == M_ABS)
        return table == M_IOA || table == M_MPTR;
    if (opr == M_CC4)
        return table == M_CC8;
    return false;
}

bool acceptModes(AsmInsn &insn, const Entry *entry) {
    const auto table = entry->readFlags();
    return acceptMode(insn.dstOp.mode, table.dst()) && acceptMode(insn.srcOp.mode, table.src());
}

Error searchName(CpuType cpuType, AsmInsn &insn) {
    cpu(cpuType)->searchName(insn, acceptModes);
    return insn.getError();
}

bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *) {
    auto opc = insn.opCode();
    const auto flags = entry->readFlags();
    const auto dst = flags.dst();
    const auto src = flags.src();

    if (dst == M_DR8 || src == M_DR8 || dst == M_DR8X) {
        const auto reg = (opc >> 3) & 7;
        if (reg == 6 || (dst == M_DR8X && (reg == 4 || reg == 5)))
            return false;
        opc &= ~(7 << 3);
    }
    if (dst == M_DST || src == M_DST || dst == M_FIDX || src == M_FIDX)
        opc &= ~(7 << 3);
    if (src == M_SR8 || src == M_SR8X) {
        const auto reg = opc & 7;
        if (reg == 6 || (src == M_SR8X && (reg == 4 || reg == 5)))
            return false;
        opc &= ~7;
    }
    if (dst == M_SRC || src == M_SRC)
        opc &= ~7;
    if (dst == R_DXY || src == R_DXY)
        opc &= ~8;
    if (dst == M_R16L || src == M_R16L)
        opc &= ~3;
    if (src == R_SXY)
        opc &= ~1;
    if (dst == M_BIXH || src == M_BIXH || dst == M_PIXH || src == M_PIXH)
        opc &= ~(3 << 3);
    if (dst == M_BIXL || src == M_BIXL || dst == M_PIXL || src == M_PIXL)
        opc &= ~3;
    if (dst == M_MPTR || src == M_MPTR || dst == M_IDX16 || src == M_IDX16)
        opc &= ~(1 << 4);

    if (dst == M_CC8 || dst == M_VEC || dst == M_BIT) {
        opc &= ~(7 << 3);
    } else if (dst == M_R16 || src == M_R16 || dst == M_R16X || src == M_R16X || dst == M_STK ||
               src == M_STK) {
        opc &= ~(3 << 4);
    } else if (dst == M_NOHL || src == M_NOHL) {
        const auto mode = (opc >> 4) & 3;
        if (mode == 2)
            return false;
        opc &= ~(3 << 4);
    } else if (dst == I_PTR || src == I_PTR) {
        opc &= ~(1 << 4);
    } else if (dst == M_CC4 || dst == M_IMMD) {
        opc &= ~(3 << 3);
    }
    return opc == entry->readOpCode();
}

Error searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) {
    cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    return insn.getError();
}

bool isPrefix(CpuType cpuType, uint16_t code) {
    return cpu(cpuType)->isPrefix(code);
}

bool EntryInsn::ixBit() const {
    return (prefix() == TableZ80::IXBIT || prefix() == TableZ80::IYBIT);
}

const /*PROGMEM*/ char *TableZ80::listCpu_P() const {
    return TEXT_CPU_LIST;
}

const /*PROGMEM*/ char *TableZ80::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableZ80::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    auto t = Cpu::search(name, ARRAY_RANGE(CPU_TABLE));
    if (t) {
        cpuType = t->readCpuType();
        return OK;
    }
    return UNSUPPORTED_CPU;
}

const TableZ80 TABLE;

}  // namespace z80
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

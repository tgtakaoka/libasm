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

#include "table_tlcs900.h"
#include "entry_table.h"
#include "entry_tlcs900.h"
#include "insn_tlcs900.h"
#include "reg_tlcs900.h"
#include "text_tlcs900.h"

using namespace libasm::text::tlcs900;
using namespace libasm::tlcs900::reg;

namespace libasm {
namespace tlcs900 {

#define E2(_opc, _cf, _name, _sz, _dst, _src) \
    {_opc, Entry::Flags::create(_cf, _dst, _src, _sz), _name}
#define E1(_opc, _cf, _name, _sz, _dst) E2(_opc, _cf, _name, _sz, _dst, M_NONE)
#define E0(_opc, _cf, _name, _sz) E1(_opc, _cf, _name, _sz, M_NONE)

// clang-format off

constexpr Entry TABLE_OPC[] PROGMEM = {
    E0(0x00,  CF_00, TEXT_NOP,   SM_ANY),                       // NOP
    E1(0x02,  CF_00, TEXT_PUSH,  SM_ANY, R_SR),                 // PUSH SR
    E1(0x03,  CF_00, TEXT_POP,   SM_ANY, R_SR),                 // POP SR
    E0(0x05,  CF_00, TEXT_HALT,  SM_ANY),                       // HALT
    E1(0x06,  CF_00, TEXT_EI,    SM_ANY, M_INTLVL),             // EI #
    E0(0x07,  CF_00, TEXT_RETI,  SM_ANY),                       // RETI
    E2(0x08,  CF_00, TEXT_LD,    SM_ANY, M_ABS8,   M_IMM8),     // LD (n),#
    E1(0x09,  CF_00, TEXT_PUSH,  SM_ANY, M_IMM8),               // PUSH #
    E2(0x0A,  CF_00, TEXT_LDW,   SM_ANY, M_ABS8,   M_IMM16),    // LDW (n),#
    E1(0x0B,  CF_00, TEXT_PUSHW, SM_ANY, M_IMM16),              // PUSHW #
    E0(0x0C,  CF_00, TEXT_INCF,  SM_ANY),                       // INCF
    E0(0x0D,  CF_00, TEXT_DECF,  SM_ANY),                       // DECF
    E0(0x0E,  CF_00, TEXT_RET,   SM_ANY),                       // RET
    E1(0x0E,  CF_00, TEXT_RET,   SM_ANY, M_CCT),                // RET T
    E1(0x0F,  CF_00, TEXT_RETD,  SM_ANY, M_IMM16),              // RETD #
    E0(0x10,  CF_00, TEXT_RCF,   SM_ANY),                       // RCF
    E0(0x11,  CF_00, TEXT_SCF,   SM_ANY),                       // SCF
    E0(0x12,  CF_00, TEXT_CCF,   SM_ANY),                       // CCF
    E0(0x13,  CF_00, TEXT_ZCF,   SM_ANY),                       // ZCF
    E1(0x14,  CF_00, TEXT_PUSH,  SM_ANY, R_A),                  // PUSH A
    E1(0x15,  CF_00, TEXT_POP,   SM_ANY, R_A),                  // POP A
    E2(0x16,  CF_00, TEXT_EX,    SM_ANY, R_F,      R_FP),       // EX F,F'
    E1(0x17,  CF_00, TEXT_LDF,   SM_ANY, M_LDF),                // LDF #
    E1(0x18,  CF_00, TEXT_PUSH,  SM_ANY, R_F),                  // PUSH F
    E1(0x19,  CF_00, TEXT_POP,   SM_ANY, R_F),                  // POP F
    E1(0x1A,  CF_00, TEXT_JP,    SM_ANY, M_ABS16),              // JP (nn)
    E1(0x1B,  CF_00, TEXT_JP,    SM_ANY, M_ABS24),              // JP (nnn)
    E1(0x1C,  CF_00, TEXT_CALL,  SM_ANY, M_ABS16),              // CALL (nn)
    E1(0x1D,  CF_00, TEXT_CALL,  SM_ANY, M_ABS24),              // CALL (nnn)
    E2(0x1A,  CF_00, TEXT_JP,    SM_ANY, M_CCT,    M_ABS16),    // JP T,(nn)
    E2(0x1B,  CF_00, TEXT_JP,    SM_ANY, M_CCT,    M_ABS24),    // JP T,(nnn)
    E2(0x1C,  CF_00, TEXT_CALL,  SM_ANY, M_CCT,    M_ABS16),    // CALL T,(nn)
    E2(0x1D,  CF_00, TEXT_CALL,  SM_ANY, M_CCT,    M_ABS24),    // CALL T,(nnn)
    E1(0x1E,  CF_00, TEXT_CALR,  SM_ANY, M_REL16),              // CALR $
    E2(0x20,  CF_07, TEXT_LD,    SM_ANY, M_REG8,   M_IMM8),     // LD r,#
    E1(0x28,  CF_07, TEXT_PUSH,  SM_ANY, M_REG16),              // PUSH r
    E2(0x30,  CF_07, TEXT_LD,    SM_ANY, M_REG16,  M_IMM16),    // LD r,#
    E1(0x38,  CF_07, TEXT_PUSH,  SM_ANY, M_REG32),              // PUSH r
    E2(0x40,  CF_07, TEXT_LD,    SM_ANY, M_REG32,  M_IMM32),    // LD r,#
    E1(0x48,  CF_07, TEXT_POP,   SM_ANY, M_REG16),              // POP r
    E1(0x58,  CF_07, TEXT_POP,   SM_ANY, M_REG32),              // POP r
    E2(0x60,  CF_0F, TEXT_JR,    SM_ANY, M_CC,     M_REL8),     // JR cc,$
    E2(0x70,  CF_0F, TEXT_JRL,   SM_ANY, M_CC,     M_REL16),    // JRL cc,$
    E1(0xF8,  CF_07, TEXT_SWI,   SM_ANY, M_SWI),                // SWI #v
    E0(0xFF,  CF_00, TEXT_SWI,   SM_ANY),                       // SWI
    E1(0x06,  CF_00, TEXT_EI,    SM_ANY, M_ZERO),               // EI
    E1(0x06,  CF_00, TEXT_DI,    SM_ANY, M_DISUF),              // DI
};
constexpr uint8_t INDEX_OPC[] PROGMEM = {
     27,  // TEXT_CALL
     28,  // TEXT_CALL
     31,  // TEXT_CALL
     32,  // TEXT_CALL
     33,  // TEXT_CALR
     17,  // TEXT_CCF
     11,  // TEXT_DECF
     46,  // TEXT_DI
      4,  // TEXT_EI
     45,  // TEXT_EI
     21,  // TEXT_EX
      3,  // TEXT_HALT
     10,  // TEXT_INCF
     25,  // TEXT_JP
     26,  // TEXT_JP
     29,  // TEXT_JP
     30,  // TEXT_JP
     41,  // TEXT_JR
     42,  // TEXT_JRL
      6,  // TEXT_LD
     34,  // TEXT_LD
     36,  // TEXT_LD
     38,  // TEXT_LD
     22,  // TEXT_LDF
      8,  // TEXT_LDW
      0,  // TEXT_NOP
      2,  // TEXT_POP
     20,  // TEXT_POP
     24,  // TEXT_POP
     39,  // TEXT_POP
     40,  // TEXT_POP
      1,  // TEXT_PUSH
      7,  // TEXT_PUSH
     19,  // TEXT_PUSH
     23,  // TEXT_PUSH
     35,  // TEXT_PUSH
     37,  // TEXT_PUSH
      9,  // TEXT_PUSHW
     15,  // TEXT_RCF
     12,  // TEXT_RET
     13,  // TEXT_RET
     14,  // TEXT_RETD
      5,  // TEXT_RETI
     16,  // TEXT_SCF
     43,  // TEXT_SWI
     44,  // TEXT_SWI
     18,  // TEXT_ZCF
};

constexpr Entry TABLE_OPC_TLCS900[] PROGMEM = {
        E0(0x01,  CF_00, TEXT_NORMAL, SM_ANY),                  // NORMAL
        E0(0x04,  CF_00, TEXT_MAX,   SM_ANY),                   // MAX
};
constexpr uint8_t INDEX_OPC_TLCS900[] PROGMEM = {
        1,  // TEXT_MAX
        0,  // TEXT_NORMAL
};

constexpr Entry TABLE_OPC_TLCS900L[] PROGMEM = {
    E0(0x04,  CF_00, TEXT_MIN,   SM_ANY),                       // MIN
};
constexpr uint8_t INDEX_OPC_TLCS900L[] PROGMEM = {
      0,  // TEXT_MIN
};

constexpr Entry TABLE_SRC_REG[] PROGMEM = {
    E1(0x06,  CF_00, TEXT_CPL,   SM_BW,  M_SRC),                // CPL (mem)
    E1(0x07,  CF_00, TEXT_NEG,   SM_BW,  M_SRC),                // NEG (mem)
    E2(0x08,  CF_00, TEXT_MUL,   SM_ANY, M_MULDSTP, M_IMMX),    // MUL RR,#
    E2(0x09,  CF_00, TEXT_MULS,  SM_ANY, M_MULDSTP, M_IMMX),    // MULS RR,#
    E2(0x0A,  CF_00, TEXT_DIV,   SM_ANY, M_MULDSTP, M_IMMX),    // DIV RR,#
    E2(0x0B,  CF_00, TEXT_DIVS,  SM_ANY, M_MULDSTP, M_IMMX),    // DIVS RR,#
    E2(0x0C,  CF_00, TEXT_LINK,  SM_L,   M_SRC,    M_IMM16),    // LINK (mem),#
    E1(0x0D,  CF_00, TEXT_UNLK,  SM_L,   M_SRC),                // UNLK (mem)
    E2(0x0E,  CF_00, TEXT_BS1F,  SM_W,   R_A,      M_SRC),      // BS1F A,(mem)
    E2(0x0F,  CF_00, TEXT_BS1B,  SM_W,   R_A,      M_SRC),      // BS1B A,(mem)
    E1(0x10,  CF_00, TEXT_DAA,   SM_B,   M_SRC),                // DAA (mem)
    E1(0x12,  CF_00, TEXT_EXTZ,  SM_WL,  M_SRC),                // EXTZ (mem)
    E1(0x13,  CF_00, TEXT_EXTS,  SM_WL,  M_SRC),                // EXTS (mem)
    E1(0x14,  CF_00, TEXT_PAA,   SM_WL,  M_SRC),                // PAA (mem)
    E1(0x16,  CF_00, TEXT_MIRR,  SM_W,   M_SRC),                // MIRR (mem)
    E1(0x19,  CF_00, TEXT_MULA,  SM_ANY, M_R32SRC),             // MULA xrr
    E2(0x1C,  CF_00, TEXT_DJNZ,  SM_BW,  M_SRC,    M_REL8),     // DJNZ (mem),$
    E2(0x20,  CF_00, TEXT_ANDCF, SM_BW,  M_BIT,    M_SRC),      // ANDCF b,(mem)
    E2(0x21,  CF_00, TEXT_ORCF,  SM_BW,  M_BIT,    M_SRC),      // ORCF b,(mem)
    E2(0x22,  CF_00, TEXT_XORCF, SM_BW,  M_BIT,    M_SRC),      // XORCF b,(mem)
    E2(0x23,  CF_00, TEXT_LDCF,  SM_BW,  M_BIT,    M_SRC),      // LDCF b,(mem)
    E2(0x24,  CF_00, TEXT_STCF,  SM_BW,  M_BIT,    M_SRC),      // STCF b,(mem)
    E2(0x2E,  CF_00, TEXT_LDC,   SM_ANY, M_CREG,   M_SRC),      // LDC cr,(mem)
    E2(0x2F,  CF_00, TEXT_LDC,   SM_ANY, M_SRC,    M_CREG),     // LDC (mem),cr
    E2(0x30,  CF_00, TEXT_RES,   SM_BW,  M_BIT,    M_SRC),      // RES b,(mem)
    E2(0x31,  CF_00, TEXT_SET,   SM_BW,  M_BIT,    M_SRC),      // SET b,(mem)
    E2(0x32,  CF_00, TEXT_CHG,   SM_BW,  M_BIT,    M_SRC),      // CHG b,(mem)
    E2(0x33,  CF_00, TEXT_BIT,   SM_BW,  M_BIT,    M_SRC),      // BIT b,(mem)
    E2(0x34,  CF_00, TEXT_TSET,  SM_BW,  M_BIT,    M_SRC),      // TSET b,(mem)
    E2(0x40,  CF_07, TEXT_MUL,   SM_ANY, M_MULDST, M_SRC),      // MUL RR,(mem)
    E2(0x48,  CF_07, TEXT_MULS,  SM_ANY, M_MULDST, M_SRC),      // MULS RR,(mem)
    E2(0x50,  CF_07, TEXT_DIV,   SM_ANY, M_MULDST, M_SRC),      // DIV RR,(mem)
    E2(0x58,  CF_07, TEXT_DIVS,  SM_ANY, M_MULDST, M_SRC),      // DIVS RR,(mem)
    E2(0x60,  CF_07, TEXT_INC,   SM_ANY, M_STEP,   M_SRC),      // INC #3,(mem)
    E1(0x61,  CF_00, TEXT_INC,   SM_ANY, M_SRC),                // INC (mem)
    E2(0x68,  CF_07, TEXT_DEC,   SM_ANY, M_STEP,   M_SRC),      // DEC #3,(mem)
    E1(0x69,  CF_00, TEXT_DEC,   SM_ANY, M_SRC),                // DEC (mem)
    E2(0x70,  CF_0F, TEXT_SCC,   SM_BW,  M_CC,     M_SRC),      // SCC cc,(mem)
    E2(0x80,  CF_07, TEXT_ADD,   SM_ANY, M_DST,    M_SRC),      // ADD R,(mem)
    E2(0x88,  CF_07, TEXT_LD,    SM_ANY, M_DST,    M_SRC),      // LD R,(mem)
    E2(0x90,  CF_07, TEXT_ADC,   SM_ANY, M_DST,    M_SRC),      // ADC R,(mem)
    E2(0xA0,  CF_07, TEXT_SUB,   SM_ANY, M_DST,    M_SRC),      // SUB R,(mem)
    E2(0xB0,  CF_07, TEXT_SBC,   SM_ANY, M_DST,    M_SRC),      // SBC R,(mem)
    E2(0xB8,  CF_07, TEXT_EX,    SM_BW,  M_DST,    M_SRC),      // EX R,(mem)
    E2(0xC0,  CF_07, TEXT_AND,   SM_ANY, M_DST,    M_SRC),      // AND R,(mem)
    E2(0xC8,  CF_00, TEXT_ADD,   SM_ANY, M_SRC,    M_IMMX),     // ADD (mem),#
    E2(0xC9,  CF_00, TEXT_ADC,   SM_ANY, M_SRC,    M_IMMX),     // ADC (mem),#
    E2(0xCA,  CF_00, TEXT_SUB,   SM_ANY, M_SRC,    M_IMMX),     // SUB (mem),#
    E2(0xCB,  CF_00, TEXT_SBC,   SM_ANY, M_SRC,    M_IMMX),     // SBC (mem),#
    E2(0xCC,  CF_00, TEXT_AND,   SM_ANY, M_SRC,    M_IMMX),     // AND (mem),#
    E2(0xCD,  CF_00, TEXT_XOR,   SM_ANY, M_SRC,    M_IMMX),     // XOR (mem),#
    E2(0xCE,  CF_00, TEXT_OR,    SM_ANY, M_SRC,    M_IMMX),     // OR (mem),#
    E2(0xCF,  CF_00, TEXT_CP,    SM_ANY, M_SRC,    M_IMMX),     // CP (mem),#
    E2(0xD0,  CF_07, TEXT_XOR,   SM_ANY, M_DST,    M_SRC),      // XOR R,(mem)
    E2(0xE0,  CF_07, TEXT_OR,    SM_ANY, M_DST,    M_SRC),      // OR R,(mem)
    E2(0xE8,  CF_00, TEXT_RLC,   SM_ANY, M_RCOUNT, M_SRC),      // RLC #n,(mem)
    E2(0xE9,  CF_00, TEXT_RRC,   SM_ANY, M_RCOUNT, M_SRC),      // RRC #n,(mem)
    E2(0xEA,  CF_00, TEXT_RL,    SM_ANY, M_RCOUNT, M_SRC),      // RL #n,(mem)
    E2(0xEB,  CF_00, TEXT_RR,    SM_ANY, M_RCOUNT, M_SRC),      // RR #n,(mem)
    E2(0xEC,  CF_00, TEXT_SLA,   SM_ANY, M_RCOUNT, M_SRC),      // SLA #n,(mem)
    E2(0xED,  CF_00, TEXT_SRA,   SM_ANY, M_RCOUNT, M_SRC),      // SRA #n,(mem)
    E2(0xEE,  CF_00, TEXT_SLL,   SM_ANY, M_RCOUNT, M_SRC),      // SLL #n,(mem)
    E2(0xEF,  CF_00, TEXT_SRL,   SM_ANY, M_RCOUNT, M_SRC),      // SRL #n,(mem)
    // Count omitted defaults to 1: "RLC r" == "RLC 1,r" (asm-only aliases).
    E2(0xE8,  CF_00, TEXT_RLC,   SM_ANY, M_SRC,    M_RC1),      // RLC r
    E2(0xE9,  CF_00, TEXT_RRC,   SM_ANY, M_SRC,    M_RC1),      // RRC r
    E2(0xEA,  CF_00, TEXT_RL,    SM_ANY, M_SRC,    M_RC1),      // RL r
    E2(0xEB,  CF_00, TEXT_RR,    SM_ANY, M_SRC,    M_RC1),      // RR r
    E2(0xEC,  CF_00, TEXT_SLA,   SM_ANY, M_SRC,    M_RC1),      // SLA r
    E2(0xED,  CF_00, TEXT_SRA,   SM_ANY, M_SRC,    M_RC1),      // SRA r
    E2(0xEE,  CF_00, TEXT_SLL,   SM_ANY, M_SRC,    M_RC1),      // SLL r
    E2(0xEF,  CF_00, TEXT_SRL,   SM_ANY, M_SRC,    M_RC1),      // SRL r
    E2(0xF0,  CF_07, TEXT_CP,    SM_ANY, M_DST,    M_SRC),      // CP R,(mem)
    E2(0x38,  CF_00, TEXT_MINC1, SM_W,   M_BUF,    M_SRC),      // MINC1 #buf,(mem)
    E2(0x39,  CF_00, TEXT_MINC2, SM_W,   M_BUF,    M_SRC),      // MINC2 #buf,(mem)
    E2(0x3A,  CF_00, TEXT_MINC4, SM_W,   M_BUF,    M_SRC),      // MINC4 #buf,(mem)
    E2(0x3C,  CF_00, TEXT_MDEC1, SM_W,   M_BUF,    M_SRC),      // MDEC1 #buf,(mem)
    E2(0x3D,  CF_00, TEXT_MDEC2, SM_W,   M_BUF,    M_SRC),      // MDEC2 #buf,(mem)
    E2(0x3E,  CF_00, TEXT_MDEC4, SM_W,   M_BUF,    M_SRC),      // MDEC4 #buf,(mem)
};
constexpr uint8_t INDEX_SRC_REG[] PROGMEM = {
     40,  // TEXT_ADC
     46,  // TEXT_ADC
     38,  // TEXT_ADD
     45,  // TEXT_ADD
     44,  // TEXT_AND
     49,  // TEXT_AND
     17,  // TEXT_ANDCF
     27,  // TEXT_BIT
      9,  // TEXT_BS1B
      8,  // TEXT_BS1F
     26,  // TEXT_CHG
     52,  // TEXT_CP
     71,  // TEXT_CP
      0,  // TEXT_CPL
     10,  // TEXT_DAA
     35,  // TEXT_DEC
     36,  // TEXT_DEC
      4,  // TEXT_DIV
     31,  // TEXT_DIV
      5,  // TEXT_DIVS
     32,  // TEXT_DIVS
     16,  // TEXT_DJNZ
     43,  // TEXT_EX
     12,  // TEXT_EXTS
     11,  // TEXT_EXTZ
     33,  // TEXT_INC
     34,  // TEXT_INC
     39,  // TEXT_LD
     22,  // TEXT_LDC
     23,  // TEXT_LDC
     20,  // TEXT_LDCF
      6,  // TEXT_LINK
     75,  // TEXT_MDEC1
     76,  // TEXT_MDEC2
     77,  // TEXT_MDEC4
     72,  // TEXT_MINC1
     73,  // TEXT_MINC2
     74,  // TEXT_MINC4
     14,  // TEXT_MIRR
      2,  // TEXT_MUL
     29,  // TEXT_MUL
     15,  // TEXT_MULA
      3,  // TEXT_MULS
     30,  // TEXT_MULS
      1,  // TEXT_NEG
     51,  // TEXT_OR
     54,  // TEXT_OR
     18,  // TEXT_ORCF
     13,  // TEXT_PAA
     24,  // TEXT_RES
     57,  // TEXT_RL
     65,  // TEXT_RL
     55,  // TEXT_RLC
     63,  // TEXT_RLC
     58,  // TEXT_RR
     66,  // TEXT_RR
     56,  // TEXT_RRC
     64,  // TEXT_RRC
     42,  // TEXT_SBC
     48,  // TEXT_SBC
     37,  // TEXT_SCC
     25,  // TEXT_SET
     59,  // TEXT_SLA
     67,  // TEXT_SLA
     61,  // TEXT_SLL
     69,  // TEXT_SLL
     60,  // TEXT_SRA
     68,  // TEXT_SRA
     62,  // TEXT_SRL
     70,  // TEXT_SRL
     21,  // TEXT_STCF
     41,  // TEXT_SUB
     47,  // TEXT_SUB
     28,  // TEXT_TSET
      7,  // TEXT_UNLK
     50,  // TEXT_XOR
     53,  // TEXT_XOR
     19,  // TEXT_XORCF
};

constexpr Entry TABLE_SRC_ABREG[] PROGMEM = {
    E2(0x03,  CF_00, TEXT_LD,    SM_ANY, M_ABSDST, M_IMMX),     // LD (R),#
    E2(0x98,  CF_07, TEXT_LD,    SM_ANY, M_ABSDST, M_DST),      // LD (R),R
};
constexpr uint8_t INDEX_SRC_ABREG[] PROGMEM = {
      0,  // TEXT_LD
      1,  // TEXT_LD
};

constexpr Entry TABLE_SRC_MEM[] PROGMEM = {
    E1(0x04,  CF_00, TEXT_PUSH,  SM_B,   M_SRC),                // PUSH (mem)
    E1(0x04,  CF_00, TEXT_PUSHW, SM_W,   M_SRC),                // PUSHW (mem)
    E2(0x06,  CF_00, TEXT_RLD,   SM_ANY, R_A,      M_SRC),      // RLD A,(mem)
    E1(0x06,  CF_00, TEXT_RLD,   SM_ANY, M_SRC),                // RLD (mem)
    E2(0x07,  CF_00, TEXT_RRD,   SM_ANY, R_A,      M_SRC),      // RRD A,(mem)
    E1(0x07,  CF_00, TEXT_RRD,   SM_ANY, M_SRC),                // RRD (mem)
    E2(0x40,  CF_07, TEXT_MUL,   SM_ANY, M_MULDST, M_SRC),      // MUL RR,(mem)
    E2(0x48,  CF_07, TEXT_MULS,  SM_ANY, M_MULDST, M_SRC),      // MULS RR,(mem)
    E2(0x50,  CF_07, TEXT_DIV,   SM_ANY, M_MULDST, M_SRC),      // DIV RR,(mem)
    E2(0x58,  CF_07, TEXT_DIVS,  SM_ANY, M_MULDST, M_SRC),      // DIVS RR,(mem)
    E2(0x60,  CF_07, TEXT_INC,   SM_B,   M_STEP,   M_SRC),      // INC #3,(mem)
    E1(0x61,  CF_00, TEXT_INC,   SM_B,   M_SRC),                // INC (mem)
    E2(0x60,  CF_07, TEXT_INCW,  SM_W,   M_STEP,   M_SRC),      // INCW #3,(mem)
    E1(0x61,  CF_00, TEXT_INCW,  SM_W,   M_SRC),                // INCW (mem)
    E2(0x68,  CF_07, TEXT_DEC,   SM_B,   M_STEP,   M_SRC),      // DEC #3,(mem)
    E1(0x69,  CF_00, TEXT_DEC,   SM_B,   M_SRC),                // DEC (mem)
    E2(0x68,  CF_07, TEXT_DECW,  SM_W,   M_STEP,   M_SRC),      // DECW #3,(mem)
    E1(0x69,  CF_00, TEXT_DECW,  SM_W,   M_SRC),                // DECW (mem)
    E2(0x19,  CF_00, TEXT_LD,    SM_B,   M_ABS16,  M_SRC),      // LD (nn),(mem)
    E2(0x19,  CF_00, TEXT_LDW,   SM_W,   M_ABS16,  M_SRC),      // LDW (nn),(mem)
    E2(0x20,  CF_07, TEXT_LD,    SM_ANY, M_DST,    M_SRC),      // LD R,(mem)
    E2(0x80,  CF_07, TEXT_ADD,   SM_ANY, M_DST,    M_SRC),      // ADD R,(mem)
    E2(0x90,  CF_07, TEXT_ADC,   SM_ANY, M_DST,    M_SRC),      // ADC R,(mem)
    E2(0xA0,  CF_07, TEXT_SUB,   SM_ANY, M_DST,    M_SRC),      // SUB R,(mem)
    E2(0xB0,  CF_07, TEXT_SBC,   SM_ANY, M_DST,    M_SRC),      // SBC R,(mem)
    E2(0xC0,  CF_07, TEXT_AND,   SM_ANY, M_DST,    M_SRC),      // AND R,(mem)
    E2(0x88,  CF_07, TEXT_ADD,   SM_ANY, M_SRC,    M_DST),      // ADD (mem),R
    E2(0x98,  CF_07, TEXT_ADC,   SM_ANY, M_SRC,    M_DST),      // ADC (mem),R
    E2(0xA8,  CF_07, TEXT_SUB,   SM_ANY, M_SRC,    M_DST),      // SUB (mem),R
    E2(0xB8,  CF_07, TEXT_SBC,   SM_ANY, M_SRC,    M_DST),      // SBC (mem),R
    E2(0xC8,  CF_07, TEXT_AND,   SM_ANY, M_SRC,    M_DST),      // AND (mem),R
    E2(0xD8,  CF_07, TEXT_XOR,   SM_ANY, M_SRC,    M_DST),      // XOR (mem),R
    E2(0xE8,  CF_07, TEXT_OR,    SM_ANY, M_SRC,    M_DST),      // OR (mem),R
    E2(0xF8,  CF_07, TEXT_CP,    SM_ANY, M_SRC,    M_DST),      // CP (mem),R
    E2(0x30,  CF_07, TEXT_EX,    SM_BW,  M_DST,    M_SRC),      // EX R,(mem)
    E2(0x38,  CF_00, TEXT_ADD,   SM_B,   M_SRC,    M_IMM8),     // ADD (mem),#
    E2(0x38,  CF_00, TEXT_ADDW,  SM_W,   M_SRC,    M_IMM16),    // ADDW (mem),#
    E2(0x39,  CF_00, TEXT_ADC,   SM_B,   M_SRC,    M_IMM8),     // ADC (mem),#
    E2(0x39,  CF_00, TEXT_ADCW,  SM_W,   M_SRC,    M_IMM16),    // ADCW (mem),#
    E2(0x3A,  CF_00, TEXT_SUB,   SM_B,   M_SRC,    M_IMM8),     // SUB (mem),#
    E2(0x3A,  CF_00, TEXT_SUBW,  SM_W,   M_SRC,    M_IMM16),    // SUBW (mem),#
    E2(0x3B,  CF_00, TEXT_SBC,   SM_B,   M_SRC,    M_IMM8),     // SBC (mem),#
    E2(0x3B,  CF_00, TEXT_SBCW,  SM_W,   M_SRC,    M_IMM16),    // SBCW (mem),#
    E2(0x3C,  CF_00, TEXT_AND,   SM_B,   M_SRC,    M_IMM8),     // AND (mem),#
    E2(0x3C,  CF_00, TEXT_ANDW,  SM_W,   M_SRC,    M_IMM16),    // ANDW (mem),#
    E2(0x3D,  CF_00, TEXT_XOR,   SM_B,   M_SRC,    M_IMM8),     // XOR (mem),#
    E2(0x3D,  CF_00, TEXT_XORW,  SM_W,   M_SRC,    M_IMM16),    // XORW (mem),#
    E2(0x3E,  CF_00, TEXT_OR,    SM_B,   M_SRC,    M_IMM8),     // OR (mem),#
    E2(0x3E,  CF_00, TEXT_ORW,   SM_W,   M_SRC,    M_IMM16),    // ORW (mem),#
    E2(0x3F,  CF_00, TEXT_CP,    SM_B,   M_SRC,    M_IMM8),     // CP (mem),#
    E2(0x3F,  CF_00, TEXT_CPW,   SM_W,   M_SRC,    M_IMM16),    // CPW (mem),#
    E2(0xD0,  CF_07, TEXT_XOR,   SM_ANY, M_DST,    M_SRC),      // XOR R,(mem)
    E2(0xE0,  CF_07, TEXT_OR,    SM_ANY, M_DST,    M_SRC),      // OR R,(mem)
    E1(0x78,  CF_00, TEXT_RLC,   SM_B,   M_SRC),                // RLC (mem)
    E1(0x78,  CF_00, TEXT_RLCW,  SM_W,   M_SRC),                // RLCW (mem)
    E1(0x79,  CF_00, TEXT_RRC,   SM_B,   M_SRC),                // RRC (mem)
    E1(0x79,  CF_00, TEXT_RRCW,  SM_W,   M_SRC),                // RRCW (mem)
    E1(0x7A,  CF_00, TEXT_RL,    SM_B,   M_SRC),                // RL (mem)
    E1(0x7A,  CF_00, TEXT_RLW,   SM_W,   M_SRC),                // RLW (mem)
    E1(0x7B,  CF_00, TEXT_RR,    SM_B,   M_SRC),                // RR (mem)
    E1(0x7B,  CF_00, TEXT_RRW,   SM_W,   M_SRC),                // RRW (mem)
    E1(0x7C,  CF_00, TEXT_SLA,   SM_B,   M_SRC),                // SLA (mem)
    E1(0x7C,  CF_00, TEXT_SLAW,  SM_W,   M_SRC),                // SLAW (mem)
    E1(0x7D,  CF_00, TEXT_SRA,   SM_B,   M_SRC),                // SRA (mem)
    E1(0x7D,  CF_00, TEXT_SRAW,  SM_W,   M_SRC),                // SRAW (mem)
    E1(0x7E,  CF_00, TEXT_SLL,   SM_B,   M_SRC),                // SLL (mem)
    E1(0x7E,  CF_00, TEXT_SLLW,  SM_W,   M_SRC),                // SLLW (mem)
    E1(0x7F,  CF_00, TEXT_SRL,   SM_B,   M_SRC),                // SRL (mem)
    E1(0x7F,  CF_00, TEXT_SRLW,  SM_W,   M_SRC),                // SRLW (mem)
    E2(0xF0,  CF_07, TEXT_CP,    SM_ANY, M_DST,    M_SRC),      // CP R,(mem)
};
constexpr uint8_t INDEX_SRC_MEM[] PROGMEM = {
     22,  // TEXT_ADC
     27,  // TEXT_ADC
     37,  // TEXT_ADC
     38,  // TEXT_ADCW
     21,  // TEXT_ADD
     26,  // TEXT_ADD
     35,  // TEXT_ADD
     36,  // TEXT_ADDW
     25,  // TEXT_AND
     30,  // TEXT_AND
     43,  // TEXT_AND
     44,  // TEXT_ANDW
     33,  // TEXT_CP
     49,  // TEXT_CP
     69,  // TEXT_CP
     50,  // TEXT_CPW
     14,  // TEXT_DEC
     15,  // TEXT_DEC
     16,  // TEXT_DECW
     17,  // TEXT_DECW
      8,  // TEXT_DIV
      9,  // TEXT_DIVS
     34,  // TEXT_EX
     10,  // TEXT_INC
     11,  // TEXT_INC
     12,  // TEXT_INCW
     13,  // TEXT_INCW
     18,  // TEXT_LD
     20,  // TEXT_LD
     19,  // TEXT_LDW
      6,  // TEXT_MUL
      7,  // TEXT_MULS
     32,  // TEXT_OR
     47,  // TEXT_OR
     52,  // TEXT_OR
     48,  // TEXT_ORW
      0,  // TEXT_PUSH
      1,  // TEXT_PUSHW
     57,  // TEXT_RL
     53,  // TEXT_RLC
     54,  // TEXT_RLCW
      2,  // TEXT_RLD
      3,  // TEXT_RLD
     58,  // TEXT_RLW
     59,  // TEXT_RR
     55,  // TEXT_RRC
     56,  // TEXT_RRCW
      4,  // TEXT_RRD
      5,  // TEXT_RRD
     60,  // TEXT_RRW
     24,  // TEXT_SBC
     29,  // TEXT_SBC
     41,  // TEXT_SBC
     42,  // TEXT_SBCW
     61,  // TEXT_SLA
     62,  // TEXT_SLAW
     65,  // TEXT_SLL
     66,  // TEXT_SLLW
     63,  // TEXT_SRA
     64,  // TEXT_SRAW
     67,  // TEXT_SRL
     68,  // TEXT_SRLW
     23,  // TEXT_SUB
     28,  // TEXT_SUB
     39,  // TEXT_SUB
     40,  // TEXT_SUBW
     31,  // TEXT_XOR
     45,  // TEXT_XOR
     51,  // TEXT_XOR
     46,  // TEXT_XORW
};


constexpr Entry TABLE_DST_MEM[] PROGMEM = {
    E2(0x00,  CF_00, TEXT_LD,    SM_B,   M_DST,    M_IMM8),     // LD R,#
    E2(0x02,  CF_00, TEXT_LDW,   SM_W,   M_DST,    M_IMM16),    // LDW R,#
    E1(0x04,  CF_00, TEXT_POP,   SM_ANY, M_DST),                // POP R
    E1(0x06,  CF_00, TEXT_POPW,  SM_ANY, M_DST),                // POPW R
    E2(0x14,  CF_00, TEXT_LD,    SM_ANY, M_DST,    M_ABS16),    // LD R,(nn)
    E2(0x16,  CF_00, TEXT_LDW,   SM_ANY, M_DST,    M_ABS16),    // LDW R,(nn)
    E2(0x20,  CF_07, TEXT_LDA,   SM_ANY, M_REG16,  M_DST),      // LDA r,R
    E2(0x30,  CF_07, TEXT_LDA,   SM_ANY, M_REG32,  M_DST),      // LDA r,R
    E2(0x40,  CF_07, TEXT_LD,    SM_ANY, M_DST,    M_REG8),     // LD R,r
    E2(0x50,  CF_07, TEXT_LD,    SM_ANY, M_DST,    M_REG16),    // LD R,r
    E2(0x60,  CF_07, TEXT_LD,    SM_ANY, M_DST,    M_REG32),    // LD R,r
    E2(0x80,  CF_07, TEXT_ANDCF, SM_B,   M_BIT,    M_DST),      // ANDCF b,R
    E2(0x88,  CF_07, TEXT_ORCF,  SM_B,   M_BIT,    M_DST),      // ORCF b,R
    E2(0x90,  CF_07, TEXT_XORCF, SM_B,   M_BIT,    M_DST),      // XORCF b,R
    E2(0x98,  CF_07, TEXT_LDCF,  SM_B,   M_BIT,    M_DST),      // LDCF b,R
    E2(0xA0,  CF_07, TEXT_STCF,  SM_B,   M_BIT,    M_DST),      // STCF b,R
    E2(0xA8,  CF_07, TEXT_TSET,  SM_B,   M_BIT,    M_DST),      // TSET b,R
    E2(0xB0,  CF_07, TEXT_RES,   SM_B,   M_BIT,    M_DST),      // RES b,R
    E2(0xB8,  CF_07, TEXT_SET,   SM_B,   M_BIT,    M_DST),      // SET b,R
    E2(0xC0,  CF_07, TEXT_CHG,   SM_B,   M_BIT,    M_DST),      // CHG b,R
    E2(0xC8,  CF_07, TEXT_BIT,   SM_B,   M_BIT,    M_DST),      // BIT b,R
    E2(0xD0,  CF_0F, TEXT_JP,    SM_ANY, M_CC,     M_DST),      // JP cc,R
    E2(0xE0,  CF_0F, TEXT_CALL,  SM_ANY, M_CC,     M_DST),      // CALL cc,R
};
constexpr uint8_t INDEX_DST_MEM[] PROGMEM = {
     11,  // TEXT_ANDCF
     20,  // TEXT_BIT
     22,  // TEXT_CALL
     19,  // TEXT_CHG
     21,  // TEXT_JP
      0,  // TEXT_LD
      4,  // TEXT_LD
      8,  // TEXT_LD
      9,  // TEXT_LD
     10,  // TEXT_LD
      6,  // TEXT_LDA
      7,  // TEXT_LDA
     14,  // TEXT_LDCF
      1,  // TEXT_LDW
      5,  // TEXT_LDW
     12,  // TEXT_ORCF
      2,  // TEXT_POP
      3,  // TEXT_POPW
     17,  // TEXT_RES
     18,  // TEXT_SET
     15,  // TEXT_STCF
     16,  // TEXT_TSET
     13,  // TEXT_XORCF
};

constexpr Entry TABLE_LDX[] PROGMEM = {
    E2(0xF7,  CF_00, TEXT_LDX,   SM_ANY, M_LDXDST, M_IMM16),    // LDX (nn),#
};
constexpr uint8_t INDEX_LDX[] PROGMEM = {
      0,  // TEXT_LDX
};


constexpr Entry TABLE_BLOCK[] PROGMEM = {
    // Explicit-operand forms first so the disassembler emits them; the bare
    // aliases follow so the assembler still accepts the operand-less spelling.
    E2(0x10,  CF_00, TEXT_LDI,   SM_B,   M_BLKDST, M_BLKSRC),    // LDI (XDE+),(XHL+)
    E2(0x10,  CF_00, TEXT_LDIW,  SM_W,   M_BLKDST, M_BLKSRC),    // LDIW (XDE+),(XHL+)
    E2(0x11,  CF_00, TEXT_LDIR,  SM_B,   M_BLKDST, M_BLKSRC),    // LDIR (XDE+),(XHL+)
    E2(0x11,  CF_00, TEXT_LDIRW, SM_W,   M_BLKDST, M_BLKSRC),    // LDIRW (XDE+),(XHL+)
    E2(0x12,  CF_00, TEXT_LDD,   SM_B,   M_BLKDST, M_BLKSRC),    // LDD (XDE-),(XHL-)
    E2(0x12,  CF_00, TEXT_LDDW,  SM_W,   M_BLKDST, M_BLKSRC),    // LDDW (XDE-),(XHL-)
    E2(0x13,  CF_00, TEXT_LDDR,  SM_B,   M_BLKDST, M_BLKSRC),    // LDDR (XDE-),(XHL-)
    E2(0x13,  CF_00, TEXT_LDDRW, SM_W,   M_BLKDST, M_BLKSRC),    // LDDRW (XDE-),(XHL-)
    E2(0x14,  CF_00, TEXT_CPI,   SM_B,   R_A,      M_BLKSRC),    // CPI A,(R+)
    E2(0x15,  CF_00, TEXT_CPIR,  SM_B,   R_A,      M_BLKSRC),    // CPIR A,(R+)
    E2(0x16,  CF_00, TEXT_CPD,   SM_B,   R_A,      M_BLKSRC),    // CPD A,(R-)
    E2(0x17,  CF_00, TEXT_CPDR,  SM_B,   R_A,      M_BLKSRC),    // CPDR A,(R-)
    // Word block compare: WA accumulator carries the size (no CP<W> mnemonic).
    E2(0x14,  CF_00, TEXT_CPI,   SM_W,   R_WA,     M_BLKSRC),    // CPI WA,(R+)
    E2(0x15,  CF_00, TEXT_CPIR,  SM_W,   R_WA,     M_BLKSRC),    // CPIR WA,(R+)
    E2(0x16,  CF_00, TEXT_CPD,   SM_W,   R_WA,     M_BLKSRC),    // CPD WA,(R-)
    E2(0x17,  CF_00, TEXT_CPDR,  SM_W,   R_WA,     M_BLKSRC),    // CPDR WA,(R-)
    E0(0x10,  CF_00, TEXT_LDI,   SM_B),                         // LDI
    E0(0x10,  CF_00, TEXT_LDIW,  SM_W),                         // LDIW
    E0(0x11,  CF_00, TEXT_LDIR,  SM_B),                         // LDIR
    E0(0x11,  CF_00, TEXT_LDIRW, SM_W),                         // LDIRW
    E0(0x12,  CF_00, TEXT_LDD,   SM_B),                         // LDD
    E0(0x12,  CF_00, TEXT_LDDW,  SM_W),                         // LDDW
    E0(0x13,  CF_00, TEXT_LDDR,  SM_B),                         // LDDR
    E0(0x13,  CF_00, TEXT_LDDRW, SM_W),                         // LDDRW
    E0(0x14,  CF_00, TEXT_CPI,   SM_B),                         // CPI
    E0(0x15,  CF_00, TEXT_CPIR,  SM_B),                         // CPIR
    E0(0x16,  CF_00, TEXT_CPD,   SM_B),                         // CPD
    E0(0x17,  CF_00, TEXT_CPDR,  SM_B),                         // CPDR
};

constexpr uint8_t INDEX_BLOCK[] PROGMEM = {
     10,  // TEXT_CPD
     14,  // TEXT_CPD
     26,  // TEXT_CPD
     11,  // TEXT_CPDR
     15,  // TEXT_CPDR
     27,  // TEXT_CPDR
      8,  // TEXT_CPI
     12,  // TEXT_CPI
     24,  // TEXT_CPI
      9,  // TEXT_CPIR
     13,  // TEXT_CPIR
     25,  // TEXT_CPIR
      4,  // TEXT_LDD
     20,  // TEXT_LDD
      6,  // TEXT_LDDR
     22,  // TEXT_LDDR
      7,  // TEXT_LDDRW
     23,  // TEXT_LDDRW
      5,  // TEXT_LDDW
     21,  // TEXT_LDDW
      0,  // TEXT_LDI
     16,  // TEXT_LDI
      2,  // TEXT_LDIR
     18,  // TEXT_LDIR
      3,  // TEXT_LDIRW
     19,  // TEXT_LDIRW
      1,  // TEXT_LDIW
     17,  // TEXT_LDIW
};

constexpr Entry TABLE_RETCC[] PROGMEM = {
    E1(0xF0,  CF_0F, TEXT_RET,   SM_ANY, M_CC),                 // RET cc
};

constexpr uint8_t INDEX_RETCC[] PROGMEM = {
      0,  // TEXT_RET
};

// LDAR layout: F3 13 rel16 reg_byte (rel16 between prefix and opcode).
constexpr Entry TABLE_LDAR[] PROGMEM = {
    E2(0x20,  CF_07, TEXT_LDAR,  SM_ANY, M_REG16,  M_LDARREL),  // LDAR r,$
    E2(0x30,  CF_07, TEXT_LDAR,  SM_ANY, M_REG32,  M_LDARREL),  // LDAR r,$
};

constexpr uint8_t INDEX_LDAR[] PROGMEM = {
      0,  // TEXT_LDAR
      1,  // TEXT_LDAR
};

// clang-format on

static constexpr uint16_t PM_BIT(PrefixMode pm) {
    return uint16_t(1) << pm;
}
static constexpr uint16_t PM_BITS_REG = PM_BIT(PM_REG8) | PM_BIT(PM_REG16) | PM_BIT(PM_REG32);
static constexpr uint16_t PM_BITS_ABREG =
        PM_BIT(PM_ABREG8) | PM_BIT(PM_ABREG16) | PM_BIT(PM_ABREG32);
static constexpr uint16_t PM_BITS_MEM = PM_BIT(PM_MEM8) | PM_BIT(PM_MEM16) | PM_BIT(PM_MEM32);

// EntryPage carries a PrefixMode bitmask so a single page can match multiple
// PMs (e.g. PM_REG8/16/32 share the same table) and inherits PrefixTableBase
// for an optional fixed prefix byte.
struct EntryPage : entry::PrefixTableBase<Entry> {
    uint16_t readPmMask() const { return pgm_read_word(&_pm_mask_P); }
    bool prefixMatcher(PrefixMode pm) const { return readPmMask() & PM_BIT(pm); }
    constexpr EntryPage(uint16_t pm_mask, uint16_t prefix, const Entry *head_P, const Entry *tail_P,
            const uint8_t *index_P, const uint8_t *itail_P)
        : PrefixTableBase(prefix, head_P, tail_P, index_P, itail_P), _pm_mask_P(pm_mask) {}

private:
    const uint16_t _pm_mask_P;
};

struct Cpu : entry::CpuBase<CpuType, EntryPage> {
    constexpr Cpu(CpuType cpuType, const /*PROGMEM*/ char *name_P, const EntryPage *head_P,
            const EntryPage *tail_P)
        : CpuBase(cpuType, name_P, head_P, tail_P) {}
    const EntryPage *pagesBegin() const { return _pages.readHead(); }
    const EntryPage *pagesEnd() const { return _pages.readTail(); }
};

// clang-format off
constexpr EntryPage TLCS900_PAGES[] PROGMEM = {
    {PM_BIT(PM_NONE),  0,    ARRAY_RANGE(TABLE_OPC_TLCS900), ARRAY_RANGE(INDEX_OPC_TLCS900)},
    {PM_BIT(PM_NONE),  0,    ARRAY_RANGE(TABLE_LDX),         ARRAY_RANGE(INDEX_LDX)},
    {PM_BIT(PM_NONE),  0,    ARRAY_RANGE(TABLE_OPC),         ARRAY_RANGE(INDEX_OPC)},
    {PM_BITS_REG | PM_BITS_ABREG, 0,
                             ARRAY_RANGE(TABLE_SRC_REG),     ARRAY_RANGE(INDEX_SRC_REG)},
    {PM_BITS_ABREG,    0,    ARRAY_RANGE(TABLE_SRC_ABREG),   ARRAY_RANGE(INDEX_SRC_ABREG)},
    // TABLE_DST_MEM precedes TABLE_SRC_MEM so the encoder prefers the ASL-style
    // PM_MEMD (destination-prefixed) form for LD (mem),(mem) ambiguities.
    {PM_BIT(PM_MEMD),  0,    ARRAY_RANGE(TABLE_DST_MEM),     ARRAY_RANGE(INDEX_DST_MEM)},
    {PM_BITS_MEM,      0,    ARRAY_RANGE(TABLE_SRC_MEM),     ARRAY_RANGE(INDEX_SRC_MEM)},
    {PM_BIT(PM_BLOCK) | PM_BIT(PM_BLOCKW), 0x83,   ARRAY_RANGE(TABLE_BLOCK),     ARRAY_RANGE(INDEX_BLOCK)},
    {PM_BIT(PM_RETCC), 0xB0,   ARRAY_RANGE(TABLE_RETCC),     ARRAY_RANGE(INDEX_RETCC)},
    {PM_BIT(PM_LDAR),  0xF313, ARRAY_RANGE(TABLE_LDAR),      ARRAY_RANGE(INDEX_LDAR)},
};

constexpr EntryPage TLCS900L_PAGES[] PROGMEM = {
    {PM_BIT(PM_NONE),  0,    ARRAY_RANGE(TABLE_OPC_TLCS900L), ARRAY_RANGE(INDEX_OPC_TLCS900L)},
    {PM_BIT(PM_NONE),  0,    ARRAY_RANGE(TABLE_LDX),          ARRAY_RANGE(INDEX_LDX)},
    {PM_BIT(PM_NONE),  0,    ARRAY_RANGE(TABLE_OPC),          ARRAY_RANGE(INDEX_OPC)},
    {PM_BITS_REG | PM_BITS_ABREG, 0,
                             ARRAY_RANGE(TABLE_SRC_REG),      ARRAY_RANGE(INDEX_SRC_REG)},
    {PM_BITS_ABREG,    0,    ARRAY_RANGE(TABLE_SRC_ABREG),    ARRAY_RANGE(INDEX_SRC_ABREG)},
    {PM_BIT(PM_MEMD),  0,    ARRAY_RANGE(TABLE_DST_MEM),      ARRAY_RANGE(INDEX_DST_MEM)},
    {PM_BITS_MEM,      0,    ARRAY_RANGE(TABLE_SRC_MEM),      ARRAY_RANGE(INDEX_SRC_MEM)},
    {PM_BIT(PM_BLOCK) | PM_BIT(PM_BLOCKW), 0x83,   ARRAY_RANGE(TABLE_BLOCK),      ARRAY_RANGE(INDEX_BLOCK)},
    {PM_BIT(PM_RETCC), 0xB0,   ARRAY_RANGE(TABLE_RETCC),      ARRAY_RANGE(INDEX_RETCC)},
    {PM_BIT(PM_LDAR),  0xF313, ARRAY_RANGE(TABLE_LDAR),       ARRAY_RANGE(INDEX_LDAR)},
};

constexpr EntryPage TLCS900H_PAGES[] PROGMEM = {
    {PM_BIT(PM_NONE),  0,      ARRAY_RANGE(TABLE_LDX),       ARRAY_RANGE(INDEX_LDX)},
    {PM_BIT(PM_NONE),  0,      ARRAY_RANGE(TABLE_OPC),       ARRAY_RANGE(INDEX_OPC)},
    {PM_BITS_REG | PM_BITS_ABREG, 0,
                               ARRAY_RANGE(TABLE_SRC_REG),   ARRAY_RANGE(INDEX_SRC_REG)},
    {PM_BITS_ABREG,    0,      ARRAY_RANGE(TABLE_SRC_ABREG), ARRAY_RANGE(INDEX_SRC_ABREG)},
    {PM_BIT(PM_MEMD),  0,      ARRAY_RANGE(TABLE_DST_MEM),   ARRAY_RANGE(INDEX_DST_MEM)},
    {PM_BITS_MEM,      0,      ARRAY_RANGE(TABLE_SRC_MEM),   ARRAY_RANGE(INDEX_SRC_MEM)},
    {PM_BIT(PM_BLOCK) | PM_BIT(PM_BLOCKW), 0x83,   ARRAY_RANGE(TABLE_BLOCK),     ARRAY_RANGE(INDEX_BLOCK)},
    {PM_BIT(PM_RETCC), 0xB0,   ARRAY_RANGE(TABLE_RETCC),     ARRAY_RANGE(INDEX_RETCC)},
    {PM_BIT(PM_LDAR),  0xF313, ARRAY_RANGE(TABLE_LDAR),      ARRAY_RANGE(INDEX_LDAR)},
};

// TLCS900/H2 is TLCS900/H minus LDX.
constexpr EntryPage TLCS900H2_PAGES[] PROGMEM = {
    {PM_BIT(PM_NONE),  0,      ARRAY_RANGE(TABLE_OPC),       ARRAY_RANGE(INDEX_OPC)},
    {PM_BITS_REG | PM_BITS_ABREG, 0,
                               ARRAY_RANGE(TABLE_SRC_REG),   ARRAY_RANGE(INDEX_SRC_REG)},
    {PM_BITS_ABREG,    0,      ARRAY_RANGE(TABLE_SRC_ABREG), ARRAY_RANGE(INDEX_SRC_ABREG)},
    {PM_BIT(PM_MEMD),  0,      ARRAY_RANGE(TABLE_DST_MEM),   ARRAY_RANGE(INDEX_DST_MEM)},
    {PM_BITS_MEM,      0,      ARRAY_RANGE(TABLE_SRC_MEM),   ARRAY_RANGE(INDEX_SRC_MEM)},
    {PM_BIT(PM_BLOCK) | PM_BIT(PM_BLOCKW), 0x83,   ARRAY_RANGE(TABLE_BLOCK),     ARRAY_RANGE(INDEX_BLOCK)},
    {PM_BIT(PM_RETCC), 0xB0,   ARRAY_RANGE(TABLE_RETCC),     ARRAY_RANGE(INDEX_RETCC)},
    {PM_BIT(PM_LDAR),  0xF313, ARRAY_RANGE(TABLE_LDAR),      ARRAY_RANGE(INDEX_LDAR)},
};

constexpr Cpu CPU_TABLE[] PROGMEM = {
    {TLCS900,   TEXT_CPU_TLCS900,   ARRAY_RANGE(TLCS900_PAGES)},
    {TLCS900L,  TEXT_CPU_TLCS900L,  ARRAY_RANGE(TLCS900L_PAGES)},
    {TLCS900H,  TEXT_CPU_TLCS900H,  ARRAY_RANGE(TLCS900H_PAGES)},
    {TLCS900H2, TEXT_CPU_TLCS900H2, ARRAY_RANGE(TLCS900H2_PAGES)},
};
// clang-format on

static const Cpu *cpu(CpuType cpuType) {
    return Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
}

struct PrefixPage {
    uint8_t lo_P;
    uint8_t hi_P;
    PrefixMode pm_P;

    bool contains(Config::opcode_t code) const {
        return pgm_read_byte(&lo_P) <= code && code <= pgm_read_byte(&hi_P);
    }
    PrefixMode readPm() const { return static_cast<PrefixMode>(pgm_read_byte(&pm_P)); }
};

// clang-format off
// Maps prefix byte ranges to PrefixMode. Single-byte specials (0x83, 0xB0)
// must precede the ranges that contain them (0x80-0x8F, 0xB1-0xBF).
constexpr PrefixPage PREFIX_PAGES[] PROGMEM = {
    // 0x83 ((XHL) byte) and 0xB0 ((XWA) dest) are ordinary memory prefixes;
    // they double as the block-transfer / conditional-return prefixes, which
    // the block/retcc pages claim by matching their fixed page prefix byte.
    {0x80, 0x8F, PM_MEM8},
    {0x90, 0x9F, PM_MEM16},
    {0xA0, 0xAF, PM_MEM32},
    {0xB0, 0xBF, PM_MEMD},
    {0xC0, 0xC5, PM_MEM8},
    {0xC7, 0xC7, PM_ABREG8},
    {0xC8, 0xCF, PM_REG8},
    {0xD0, 0xD5, PM_MEM16},
    {0xD7, 0xD7, PM_ABREG16},
    {0xD8, 0xDF, PM_REG16},
    {0xE0, 0xE5, PM_MEM32},
    {0xE7, 0xE7, PM_ABREG32},
    {0xE8, 0xEF, PM_REG32},
    {0xF0, 0xF5, PM_MEMD},
};
// clang-format on

bool isPrefix(CpuType /*cpuType*/, Config::opcode_t code, PrefixMode &pm) {
    for (const auto *p = ARRAY_BEGIN(PREFIX_PAGES); p < ARRAY_END(PREFIX_PAGES); p++) {
        if (p->contains(code)) {
            pm = p->readPm();
            return true;
        }
    }
    return false;
}

static bool matchOpCode(const Entry *entry, Config::opcode_t opc) {
    const auto flags = entry->readFlags();
    return (opc & ~flags.mask()) == entry->readOpCode();
}

static bool isMem(AddrMode mode) {
    return mode == M_IND || mode == M_PDEC || mode == M_PINC || mode == M_IDX8 || mode == M_IDX16 ||
           mode == M_IDXR || mode == M_ABS8 || mode == M_ABS16 || mode == M_ABS24;
}

static bool modeMatch(AddrMode actual, AddrMode expected, RegName reg) {
    if (actual == expected)
        return true;
    if (expected == M_NONE)
        return actual == M_NONE;
    if (actual == R_C)
        return expected == M_REG8 || expected == M_CC;
    if (expected == M_IMM16 && actual == M_IMM8)
        return true;
    if (expected == M_IMM32 && (actual == M_IMM8 || actual == M_IMM16))
        return true;
    if (expected == M_REL16 && actual == M_REL8)
        return true;
    if (expected == R_WA && actual == M_REG16 && reg == REG_WA)
        return true;
    if (expected == R_A && actual == M_REG8 && reg == REG_A)
        return true;
    if (expected == R_SR && actual == R_SR)
        return true;
    if ((expected == M_LDF || expected == M_INTLVL) && actual == M_IMM8)
        return true;
    if (expected == M_SWI && actual == M_IMM8)
        return true;
    if (expected == M_BIT && actual == M_IMM8)
        return true;
    if (expected == M_RCOUNT && actual == M_IMM8)
        return true;
    if (expected == M_BUF)
        return actual == M_IMM8 || actual == M_IMM16;
    if (expected == M_LDXDST)
        return actual == M_ABS8 || actual == M_ABS16;
    if (expected == M_ZERO)
        return actual == M_NONE;
    if (expected == M_DISUF)
        return actual == M_NONE;
    if (expected == M_RC1)
        return actual == M_NONE;
    if (expected == M_BLKDST || expected == M_BLKSRC)
        return actual == M_PINC || actual == M_BPDEC;
    if (expected == M_R32SRC)
        return actual == M_REG32;
    if (expected == M_LDARREG)
        return actual == M_REG16 || actual == M_REG32;
    if (expected == M_LDARREL)
        return actual == M_IMM8 || actual == M_IMM16 || actual == M_IMM32;
    if (expected == M_SRC || expected == M_DST)
        return isMem(actual);
    if (expected == M_REL8 || expected == M_REL16)
        return actual == M_IMM8 || actual == M_IMM16 || actual == M_IMM32;
    if (expected == M_STEP)
        return actual == M_IMM8;
    return false;
}

// Resolve a context-relative AddrMode (M_DST/M_SRC/M_ABSDST/M_IMMX) to a
// concrete AddrMode for the current PrefixMode. Returns expected unchanged
// when no resolution applies.
static AddrMode resolveMode(AddrMode expected, PrefixMode pm, bool dstSlot, const Operand &op) {
    switch (expected) {
    case M_DST:
        switch (pm) {
        case PM_REG8:
        case PM_MEM8:
        case PM_ABREG8:
            return M_REG8;
        case PM_REG16:
        case PM_MEM16:
        case PM_ABREG16:
            return M_REG16;
        case PM_REG32:
        case PM_MEM32:
        case PM_ABREG32:
            return M_REG32;
        default:
            return expected;
        }
    case M_SRC:
        switch (pm) {
        case PM_REG8:
            return M_REG8;
        case PM_REG16:
            return M_REG16;
        case PM_REG32:
            return M_REG32;
        case PM_ABREG8:
            return M_ABREG8;
        case PM_ABREG16:
            return M_ABREG16;
        case PM_ABREG32:
            return M_ABREG32;
        case PM_MEM8:
        case PM_MEM16:
        case PM_MEM32:
            // src slot keeps the operand's actual memory mode; dst slot
            // never reaches PM_MEM* with M_SRC in real entries.
            return dstSlot ? expected : (isMem(op.mode) ? op.mode : M_NONE);
        default:
            return expected;
        }
    case M_ABSDST:
        switch (pm) {
        case PM_ABREG8:
            return M_ABREG8;
        case PM_ABREG16:
            return M_ABREG16;
        case PM_ABREG32:
            return M_ABREG32;
        default:
            return expected;
        }
    case M_R32SRC:
        // MULA xrr32 alias: a 32-bit register operand carried by the
        // PM_REG16 prefix (D8+r). Legal only at PM_REG16.
        return pm == PM_REG16 ? M_REG32 : M_NONE;
    case M_CCT:  // the always-true condition; the cc==T check is in acceptModes
        return M_CC;
    case M_MULDST:  // MUL/DIV dest: double the source size (no long-op)
        if (pm == PM_REG8 || pm == PM_ABREG8 || pm == PM_MEM8)
            return M_REG16;
        if (pm == PM_REG16 || pm == PM_ABREG16 || pm == PM_MEM16)
            return M_REG32;
        return M_NONE;
    case M_MULDSTP:  // MUL/DIV rr,# dest in the prefix: 16-bit (byte op) / 32-bit (word op)
        return pm == PM_REG8 ? M_REG16 : pm == PM_REG16 ? M_REG32 : M_NONE;
    case M_IMMX:
        switch (pm) {
        case PM_REG8:
        case PM_MEM8:
        case PM_ABREG8:
            return M_IMM8;
        case PM_REG16:
        case PM_MEM16:
        case PM_ABREG16:
            return M_IMM16;
        case PM_REG32:
        case PM_MEM32:
        case PM_ABREG32:
            return M_IMM32;
        default:
            return M_IMM8;
        }
    default:
        return expected;
    }
}

static bool acceptMode(const Operand &op, AddrMode expected, PrefixMode pm, bool dstSlot) {
    if (op.mode == expected)
        return true;
    const auto resolved = resolveMode(expected, pm, dstSlot, op);
    if (resolved == R_F)
        return op.mode == M_CC && op.cc == CC_F;
    return modeMatch(op.mode, resolved, op.reg);
}

// The operation size implied by a PrefixMode, as a one-hot SizeMask. Prefix
// modes that carry no size (PM_MEMD dest, PM_NONE, block/retcc/ldar) return
// SM_BWL so they intersect any entry's allowed set.
static SizeMask sizeMaskOf(PrefixMode pm) {
    switch (pm) {
    case PM_REG8:
    case PM_MEM8:
    case PM_ABREG8:
    case PM_BLOCK:  // 0x83 byte block transfer
        return SM_B;
    case PM_REG16:
    case PM_MEM16:
    case PM_ABREG16:
    case PM_BLOCKW:  // 0x93 word block transfer
        return SM_W;
    case PM_REG32:
    case PM_MEM32:
    case PM_ABREG32:
        return SM_L;
    default:
        return SM_BWL;
    }
}

static bool acceptModes(AsmInsn &insn, const Entry *entry, PrefixMode pm) {
    const auto flags = entry->readFlags();
    if (!acceptMode(insn.dstOp, flags.dst(), pm, /*dstSlot=*/true))
        return false;
    if (!acceptMode(insn.srcOp, flags.src(), pm, /*dstSlot=*/false))
        return false;
    const auto dst = flags.dst();
    if (dst == M_CCT && insn.dstOp.cc != CC_T)
        return false;  // the cc=T fold only matches the always-true condition
    return true;
}

// searchName callbacks (CpuBase::searchName).

// Store the page's PrefixMode mask on insn so acceptOperands can iterate it.
static void pageSetup(AsmInsn &insn, const EntryPage *page) {
    insn.setPmMask(page->readPmMask());
}

// Try each PrefixMode in the page's mask; on first match, record the PM on
// insn so readCode can finalize the entry. Called by binarySearch once per
// candidate entry with the current page's mask already stored on insn.
static bool acceptOperands(AsmInsn &insn, const Entry *entry) {
    const auto mask = insn.pmMask();
    const auto sizes = entry->readFlags().sizes();
    for (uint8_t bit = 0; bit <= PM_BLOCKW; bit++) {
        const auto pm = PrefixMode(bit);
        if (!(mask & PM_BIT(pm)))
            continue;
        if (!acceptModes(insn, entry, pm))
            continue;
        if (sizeMaskOf(pm) & sizes) {
            insn.setPrefixMode(pm);
            return true;
        }
        // Operands fit but the operation size is outside the entry's set; keep
        // looking, and remember so a failed search reports ILLEGAL_SIZE.
        insn.setSizeError();
    }
    return false;
}

Error searchName(CpuType cpuType, AsmInsn &insn) {
    cpu(cpuType)->searchName(insn, acceptOperands, pageSetup);
    // A candidate matched the operands but not the size: report ILLEGAL_SIZE
    // rather than the generic UNKNOWN_INSTRUCTION / OPERAND_NOT_ALLOWED.
    if (insn.getError() != OK && insn.sizeError())
        insn.setError(ILLEGAL_SIZE);
    return insn.getError();
}

// searchOpCode callbacks (CpuBase::searchOpCode).

// Set the page's PrefixMode bitmask on insn and pick the PM (derived from
// the recorded prefix byte) that this page accepts. Returns false if no
// PM in the page's mask matches.
static bool prefixMatcher(DisInsn &insn, const EntryPage *page) {
    const auto mask = page->readPmMask();
    insn.setPmMask(mask);
    const auto prefix = insn.prefix();
    // Block-transfer (0x83) and conditional-return (0xB0) pages carry a fixed
    // page prefix and are matched by that byte directly. These bytes also serve
    // as (XHL)/(XWA) memory prefixes on the mem pages, which are earlier in the
    // page list, so only the block/retcc opcodes ever reach these pages.
    if (mask & PM_BIT(PM_BLOCK)) {
        // Block page: byte prefix 0x8x, word prefix 0x9x, where the low nibble
        // (0-7) is the source pointer register. Block transfer restricts it to
        // the XDE/XHL (3) or XIX/XIY (5) pair; block compare accepts any pointer.
        if ((prefix & 0x0F) > 7)
            return false;
        if ((prefix & 0xF0) == 0x80)
            insn.setPrefixMode(PM_BLOCK);
        else if ((prefix & 0xF0) == 0x90)
            insn.setPrefixMode(PM_BLOCKW);
        else
            return false;
        return true;
    }
    if (mask & PM_BIT(PM_RETCC)) {
        if (prefix != page->readPrefix())
            return false;
        insn.setPrefixMode(PM_RETCC);
        return true;
    }
    PrefixMode pm = PM_NONE;
    if (prefix > UINT8_MAX) {
        // Only PM_LDAR uses a 2-byte prefix (F3 13).
        pm = PM_LDAR;
    } else if (prefix != 0 && !isPrefix(TLCS900, static_cast<Config::opcode_t>(prefix), pm)) {
        return false;
    }
    if ((mask & PM_BIT(pm)) == 0)
        return false;
    insn.setPrefixMode(pm);
    return true;
}

static bool opcMatchCode(DisInsn &insn, const Entry *entry, const EntryPage *) {
    if (!matchOpCode(entry, insn.opCode()))
        return false;
    // Reject encodings whose prefix size is outside the entry's allowed set
    // (e.g. a long-size prefix on a byte/word-only memory op), and pick the
    // right same-opcode row (PUSH vs PUSHW) from the decoded prefix size.
    return sizeMaskOf(insn.pre()) & entry->readFlags().sizes();
}

Error searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) {
    cpu(cpuType)->searchOpCode(insn, out, opcMatchCode, prefixMatcher);
    return insn.getError();
}

const /*PROGMEM*/ char *TableTlcs900::listCpu_P() const {
    return TEXT_TLCS900_LIST;
}

const /*PROGMEM*/ char *TableTlcs900::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableTlcs900::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    const auto *t = Cpu::search(name, ARRAY_RANGE(CPU_TABLE));
    if (t) {
        cpuType = t->readCpuType();
        return OK;
    }
    return UNSUPPORTED_CPU;
}

const TableTlcs900 TABLE;

}  // namespace tlcs900
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

/*
 * Copyright 2025 Tadashi G. Takaoka
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

#include "table_h8500.h"
#include "entry_h8500.h"
#include "entry_table.h"
#include "text_h8500.h"

using namespace libasm::text::h8500;

namespace libasm {
namespace h8500 {

// clang-format off

// Entry layout mirrors the assembly form "name[:class[.size]] [src[,dst]]":
// the class follows the name and the operand size follows the class.
#define E2(opc, cf, name, cls, isz, osz, src, dst) \
    {opc, Entry::Flags::create(cf, osz, isz, cls, src, dst), name}
#define E1(opc, cf, name, cls, isz, osz, src) E2(opc, cf, name, cls, isz, osz, src, M_NONE)
#define E0(opc, name) E2(opc, CF_00, name, IC_N, ISZ_NONE, SZ_NONE, M_NONE, M_NONE)

// General format (PM_GEN): [EA byte][ext 0-2][OP byte]. The EA operand is the
// M_EASRC/M_EADST/M_IMM* slot; matchCode validates the resolved EA mode against
// it, which is how immediate-vs-reg/mem restriction (ANDC vs BCLR at 0x58) is
// expressed without an eaByte special case.
constexpr Entry TABLE_GEN[] PROGMEM = {
    // ADD:Q precedes ADD:G so a bare "ADD #1" selects the shorter quick form;
    // acceptMode rejects ADD:Q when the value is not +/-1/+/-2, falling to ADD:G.
    E2(0x08, CF_00, TEXT_ADD,    IC_Q, ISZ_DATA, SZ_DATA, M_IMM2,    M_EADST),    // ADD:Q.B/.W #1/2,<EAd>
    E2(0x09, CF_00, TEXT_ADD,    IC_Q, ISZ_DATA, SZ_DATA, M_IMM2,    M_EADST),    // ADD:Q.B/.W #1/2,<EAd>
    E2(0x0C, CF_00, TEXT_ADD,    IC_Q, ISZ_DATA, SZ_DATA, M_IMM2,    M_EADST),    // ADD:Q.B/.W #1/2,<EAd>
    E2(0x0D, CF_00, TEXT_ADD,    IC_Q, ISZ_DATA, SZ_DATA, M_IMM2,    M_EADST),    // ADD:Q.B/.W #1/2,<EAd>
    E2(0x20, CF_07, TEXT_ADD,    IC_G, ISZ_DATA, SZ_DATA, M_EASRC,   M_REG),      // ADD:G.B/.W <EAs>,Rd
    E2(0x28, CF_07, TEXT_ADDS,   IC_N, ISZ_DATA, SZ_DATA, M_EASRC,   M_REG),      // ADDS.B/.W <EAs>,Rd
    E2(0xA0, CF_07, TEXT_ADDX,   IC_N, ISZ_DATA, SZ_DATA, M_EASRC,   M_REG),      // ADDX.B/.W <EAs>,Rd
    E2(0x58, CF_07, TEXT_ANDC,   IC_N, ISZ_DATA, SZ_DATA, M_IMM8,    M_CR),       // ANDC.B/.W #xx:8,<CR>
    E2(0x58, CF_07, TEXT_BCLR,   IC_N, ISZ_DATA, SZ_DATA, M_REG,     M_EADST),    // BCLR.B/.W Rs,<EAd>
    E2(0xD0, CF_0F, TEXT_BCLR,   IC_N, ISZ_DATA, SZ_DATA, M_BIT,     M_EADST),    // BCLR.B/.W #bit,<EAd>
    E2(0x68, CF_07, TEXT_BNOT,   IC_N, ISZ_DATA, SZ_DATA, M_REG,     M_EADST),    // BNOT.B/.W Rs,<EAd>
    E2(0xE0, CF_0F, TEXT_BNOT,   IC_N, ISZ_DATA, SZ_DATA, M_BIT,     M_EADST),    // BNOT.B/.W #bit,<EAd>
    E2(0x48, CF_07, TEXT_BSET,   IC_N, ISZ_DATA, SZ_DATA, M_REG,     M_EADST),    // BSET.B/.W Rs,<EAd>
    E2(0xC0, CF_0F, TEXT_BSET,   IC_N, ISZ_DATA, SZ_DATA, M_BIT,     M_EADST),    // BSET.B/.W #bit,<EAd>
    E2(0x78, CF_07, TEXT_BTST,   IC_N, ISZ_DATA, SZ_DATA, M_REG,     M_EADST),    // BTST.B/.W Rs,<EAd>
    E2(0xF0, CF_0F, TEXT_BTST,   IC_N, ISZ_DATA, SZ_DATA, M_BIT,     M_EADST),    // BTST.B/.W #bit,<EAd>
    E1(0x13, CF_00, TEXT_CLR,    IC_N, ISZ_DATA, SZ_DATA, M_EADST),               // CLR.B/.W <EAd>
    E2(0x70, CF_07, TEXT_CMP,    IC_G, ISZ_DATA, SZ_DATA, M_EASRC,   M_REG),      // CMP:G.B/.W <EAs>,Rd
    E2(0x04, CF_00, TEXT_CMP,    IC_G, ISZ_DATA, SZ_BYTE, M_IMM8,    M_EAMEM),    // CMP:G.B #xx:8,<EAd>
    E2(0x05, CF_00, TEXT_CMP,    IC_G, ISZ_DATA, SZ_WORD, M_IMM16,   M_EAMEM),    // CMP:G.W #xx:16,<EAd>
    E2(0xB8, CF_07, TEXT_DIVXU,  IC_N, ISZ_DATA, SZ_DATA, M_EASRC,   M_REGP),     // DIVXU.B/.W <EAs>,Rd
    E1(0x11, CF_00, TEXT_EXTS,   IC_N, ISZ_NONE, SZ_BYTE, M_EAREG),               // EXTS Rn
    E1(0x12, CF_00, TEXT_EXTU,   IC_N, ISZ_NONE, SZ_BYTE, M_EAREG),               // EXTU Rn
    E2(0x88, CF_07, TEXT_LDC,    IC_N, ISZ_NONE, SZ_DATA, M_EASRC,   M_CR),       // LDC <EAs>,<CR>
    E2(0x80, CF_07, TEXT_MOV,    IC_G, ISZ_DATA, SZ_DATA, M_EASRC,   M_REG),      // MOV:G.B/.W <EAs>,Rd
    E2(0x90, CF_07, TEXT_MOV,    IC_G, ISZ_DATA, SZ_DATA, M_REG,     M_EADST),    // MOV:G.B/.W Rs,<EAd>
    E2(0x06, CF_00, TEXT_MOV,    IC_G, ISZ_DATA, SZ_BYTE, M_IMM8,    M_EAMEM),    // MOV:G.B #xx:8,<EAd>
    E2(0x07, CF_00, TEXT_MOV,    IC_G, ISZ_DATA, SZ_WORD, M_IMM16,   M_EAMEM),    // MOV:G.W #xx:16,<EAd>
    E2(0xA8, CF_07, TEXT_MULXU,  IC_N, ISZ_DATA, SZ_DATA, M_EASRC,   M_REGP),     // MULXU.B/.W <EAs>,Rd
    E1(0x14, CF_00, TEXT_NEG,    IC_N, ISZ_DATA, SZ_DATA, M_EADST),               // NEG.B/.W <EAd>
    E1(0x15, CF_00, TEXT_NOT,    IC_N, ISZ_DATA, SZ_DATA, M_EADST),               // NOT.B/.W <EAd>
    E2(0x40, CF_07, TEXT_OR,     IC_N, ISZ_DATA, SZ_DATA, M_EASRC,   M_REG),      // OR.B/.W <EAs>,Rd
    E2(0x48, CF_07, TEXT_ORC,    IC_N, ISZ_DATA, SZ_DATA, M_IMM8,    M_CR),       // ORC.B/.W #xx:8,<CR>
    E1(0x1C, CF_00, TEXT_ROTL,   IC_N, ISZ_DATA, SZ_DATA, M_EADST),               // ROTL.B/.W <EAd>
    E1(0x1D, CF_00, TEXT_ROTR,   IC_N, ISZ_DATA, SZ_DATA, M_EADST),               // ROTR.B/.W <EAd>
    E1(0x1E, CF_00, TEXT_ROTXL,  IC_N, ISZ_DATA, SZ_DATA, M_EADST),               // ROTXL.B/.W <EAd>
    E1(0x1F, CF_00, TEXT_ROTXR,  IC_N, ISZ_DATA, SZ_DATA, M_EADST),               // ROTXR.B/.W <EAd>
    E1(0x18, CF_00, TEXT_SHAL,   IC_N, ISZ_DATA, SZ_DATA, M_EADST),               // SHAL.B/.W <EAd>
    E1(0x19, CF_00, TEXT_SHAR,   IC_N, ISZ_DATA, SZ_DATA, M_EADST),               // SHAR.B/.W <EAd>
    E1(0x1A, CF_00, TEXT_SHLL,   IC_N, ISZ_DATA, SZ_DATA, M_EADST),               // SHLL.B/.W <EAd>
    E1(0x1B, CF_00, TEXT_SHLR,   IC_N, ISZ_DATA, SZ_DATA, M_EADST),               // SHLR.B/.W <EAd>
    E2(0x98, CF_07, TEXT_STC,    IC_N, ISZ_NONE, SZ_DATA, M_CR,      M_EADST),    // STC <CR>,<EAd>
    E2(0x30, CF_07, TEXT_SUB,    IC_N, ISZ_DATA, SZ_DATA, M_EASRC,   M_REG),      // SUB.B/.W <EAs>,Rd
    E2(0xB0, CF_07, TEXT_SUBX,   IC_N, ISZ_DATA, SZ_DATA, M_EASRC,   M_REG),      // SUBX.B/.W <EAs>,Rd
    E2(0x38, CF_07, TEXT_SUBS,   IC_N, ISZ_DATA, SZ_DATA, M_EASRC,   M_REG),      // SUBS.B/.W <EAs>,Rd
    E1(0x10, CF_00, TEXT_SWAP,   IC_N, ISZ_NONE, SZ_BYTE, M_EAREG),               // SWAP Rn
    E1(0x17, CF_00, TEXT_TAS,    IC_N, ISZ_NONE, SZ_BYTE, M_EADST),               // TAS <EAd>
    E1(0x16, CF_00, TEXT_TST,    IC_N, ISZ_DATA, SZ_DATA, M_EADST),               // TST.B/.W <EAd>
    E2(0x50, CF_07, TEXT_AND,    IC_N, ISZ_DATA, SZ_DATA, M_EASRC,   M_REG),      // AND.B/.W <EAs>,Rd
    E2(0x60, CF_07, TEXT_XOR,    IC_N, ISZ_DATA, SZ_DATA, M_EASRC,   M_REG),      // XOR.B/.W <EAs>,Rd
    E2(0x68, CF_07, TEXT_XORC,   IC_N, ISZ_DATA, SZ_DATA, M_IMM8,    M_CR),       // XORC.B/.W #xx:8,<CR>
};

constexpr uint8_t INDEX_GEN[] PROGMEM = {
      0,  // TEXT_ADD
      1,  // TEXT_ADD
      2,  // TEXT_ADD
      3,  // TEXT_ADD
      4,  // TEXT_ADD
      5,  // TEXT_ADDS
      6,  // TEXT_ADDX
     48,  // TEXT_AND
      7,  // TEXT_ANDC
      8,  // TEXT_BCLR
      9,  // TEXT_BCLR
     10,  // TEXT_BNOT
     11,  // TEXT_BNOT
     12,  // TEXT_BSET
     13,  // TEXT_BSET
     14,  // TEXT_BTST
     15,  // TEXT_BTST
     16,  // TEXT_CLR
     17,  // TEXT_CMP
     18,  // TEXT_CMP
     19,  // TEXT_CMP
     20,  // TEXT_DIVXU
     21,  // TEXT_EXTS
     22,  // TEXT_EXTU
     23,  // TEXT_LDC
     24,  // TEXT_MOV
     25,  // TEXT_MOV
     26,  // TEXT_MOV
     27,  // TEXT_MOV
     28,  // TEXT_MULXU
     29,  // TEXT_NEG
     30,  // TEXT_NOT
     31,  // TEXT_OR
     32,  // TEXT_ORC
     33,  // TEXT_ROTL
     34,  // TEXT_ROTR
     35,  // TEXT_ROTXL
     36,  // TEXT_ROTXR
     37,  // TEXT_SHAL
     38,  // TEXT_SHAR
     39,  // TEXT_SHLL
     40,  // TEXT_SHLR
     41,  // TEXT_STC
     42,  // TEXT_SUB
     44,  // TEXT_SUBS
     43,  // TEXT_SUBX
     45,  // TEXT_SWAP
     46,  // TEXT_TAS
     47,  // TEXT_TST
     49,  // TEXT_XOR
     50,  // TEXT_XORC
};

// XCH Rs,Rd shares OP byte 0x90 with MOV:G Rs,<EAd> but takes a register EA only.
// It lives in its own one-entry table whose page (register EA, searched before
// the shared TABLE_GEN page) claims 0x90 for a register EA; a memory EA at 0x90
// is left to MOV:G.
constexpr Entry TABLE_XCH[] PROGMEM = {
    E2(0x90, CF_07, TEXT_XCH,    IC_N, ISZ_NONE, SZ_WORD, M_EAREG,   M_REG),      // XCH Rn,Rd
};

constexpr uint8_t INDEX_XCH[] PROGMEM = {
      0,  // TEXT_XCH
};

// Extended format (PM_EXT): [EA byte][ext 0-2][00][OP byte]. DADD/DSUB are valid
// on every variant; MOVFPE/MOVTPE need the E-clock pin, so they live in a
// separate table that only the E-clock page lists include (no array slicing).
constexpr Entry TABLE_EXT[] PROGMEM = {
    E2(0xA0, CF_07, TEXT_DADD,   IC_N, ISZ_NONE, SZ_BYTE, M_EAREG,   M_REG),      // DADD Rn,Rd
    E2(0xB0, CF_07, TEXT_DSUB,   IC_N, ISZ_NONE, SZ_BYTE, M_EAREG,   M_REG),      // DSUB Rn,Rd
};

constexpr uint8_t INDEX_EXT[] PROGMEM = {
      0,  // TEXT_DADD
      1,  // TEXT_DSUB
};

// E-clock peripheral transfer (H8/500 and H8/530 only).
constexpr Entry TABLE_ECLK[] PROGMEM = {
    E2(0x80, CF_07, TEXT_MOVFPE, IC_N, ISZ_NONE, SZ_BYTE, M_EAMEM,   M_REG),      // MOVFPE <EA>,Rd
    E2(0x90, CF_07, TEXT_MOVTPE, IC_N, ISZ_NONE, SZ_BYTE, M_REG,     M_EAMEM),    // MOVTPE Rs,<EA>
};

constexpr uint8_t INDEX_ECLK[] PROGMEM = {
      0,  // TEXT_MOVFPE
      1,  // TEXT_MOVTPE
};

// Special format (PM_SPC): [OP byte][...]. No EA byte.
constexpr Entry TABLE_SPC[] PROGMEM = {
    E1(0x24, CF_00, TEXT_BCC,    IC_N, ISZ_NONE, SZ_NONE, M_REL8),                // BCC d:8
    E1(0x25, CF_00, TEXT_BCS,    IC_N, ISZ_NONE, SZ_NONE, M_REL8),                // BCS d:8
    E1(0x27, CF_00, TEXT_BEQ,    IC_N, ISZ_NONE, SZ_NONE, M_REL8),                // BEQ d:8
    E1(0x21, CF_00, TEXT_BF,     IC_N, ISZ_NONE, SZ_NONE, M_REL8),                // BF d:8
    E1(0x2C, CF_00, TEXT_BGE,    IC_N, ISZ_NONE, SZ_NONE, M_REL8),                // BGE d:8
    E1(0x2E, CF_00, TEXT_BGT,    IC_N, ISZ_NONE, SZ_NONE, M_REL8),                // BGT d:8
    E1(0x22, CF_00, TEXT_BHI,    IC_N, ISZ_NONE, SZ_NONE, M_REL8),                // BHI d:8
    E1(0x24, CF_00, TEXT_BHS,    IC_N, ISZ_NONE, SZ_NONE, M_REL8),                // BHS d:8
    E1(0x2F, CF_00, TEXT_BLE,    IC_N, ISZ_NONE, SZ_NONE, M_REL8),                // BLE d:8
    E1(0x25, CF_00, TEXT_BLO,    IC_N, ISZ_NONE, SZ_NONE, M_REL8),                // BLO d:8
    E1(0x23, CF_00, TEXT_BLS,    IC_N, ISZ_NONE, SZ_NONE, M_REL8),                // BLS d:8
    E1(0x2D, CF_00, TEXT_BLT,    IC_N, ISZ_NONE, SZ_NONE, M_REL8),                // BLT d:8
    E1(0x2B, CF_00, TEXT_BMI,    IC_N, ISZ_NONE, SZ_NONE, M_REL8),                // BMI d:8
    E1(0x26, CF_00, TEXT_BNE,    IC_N, ISZ_NONE, SZ_NONE, M_REL8),                // BNE d:8
    E1(0x2A, CF_00, TEXT_BPL,    IC_N, ISZ_NONE, SZ_NONE, M_REL8),                // BPL d:8
    E1(0x20, CF_00, TEXT_BRA,    IC_N, ISZ_NONE, SZ_NONE, M_REL8),                // BRA d:8
    E1(0x21, CF_00, TEXT_BRN,    IC_N, ISZ_NONE, SZ_NONE, M_REL8),                // BRN d:8
    E1(0x0E, CF_00, TEXT_BSR,    IC_N, ISZ_NONE, SZ_NONE, M_REL8),                // BSR d:8
    E1(0x20, CF_00, TEXT_BT,     IC_N, ISZ_NONE, SZ_NONE, M_REL8),                // BT d:8
    E1(0x28, CF_00, TEXT_BVC,    IC_N, ISZ_NONE, SZ_NONE, M_REL8),                // BVC d:8
    E1(0x29, CF_00, TEXT_BVS,    IC_N, ISZ_NONE, SZ_NONE, M_REL8),                // BVS d:8
    E1(0x34, CF_00, TEXT_BCC,    IC_N, ISZ_NONE, SZ_NONE, M_REL16),               // BCC d:16
    E1(0x35, CF_00, TEXT_BCS,    IC_N, ISZ_NONE, SZ_NONE, M_REL16),               // BCS d:16
    E1(0x37, CF_00, TEXT_BEQ,    IC_N, ISZ_NONE, SZ_NONE, M_REL16),               // BEQ d:16
    E1(0x34, CF_00, TEXT_BHS,    IC_N, ISZ_NONE, SZ_NONE, M_REL16),               // BHS d:16
    E1(0x35, CF_00, TEXT_BLO,    IC_N, ISZ_NONE, SZ_NONE, M_REL16),               // BLO d:16
    E1(0x33, CF_00, TEXT_BLS,    IC_N, ISZ_NONE, SZ_NONE, M_REL16),               // BLS d:16
    E1(0x3D, CF_00, TEXT_BLT,    IC_N, ISZ_NONE, SZ_NONE, M_REL16),               // BLT d:16
    E1(0x3B, CF_00, TEXT_BMI,    IC_N, ISZ_NONE, SZ_NONE, M_REL16),               // BMI d:16
    E1(0x36, CF_00, TEXT_BNE,    IC_N, ISZ_NONE, SZ_NONE, M_REL16),               // BNE d:16
    E1(0x3A, CF_00, TEXT_BPL,    IC_N, ISZ_NONE, SZ_NONE, M_REL16),               // BPL d:16
    E1(0x30, CF_00, TEXT_BRA,    IC_N, ISZ_NONE, SZ_NONE, M_REL16),               // BRA d:16
    E1(0x1E, CF_00, TEXT_BSR,    IC_N, ISZ_NONE, SZ_NONE, M_REL16),               // BSR d:16
    E1(0x30, CF_00, TEXT_BT,     IC_N, ISZ_NONE, SZ_NONE, M_REL16),               // BT d:16
    E1(0x38, CF_00, TEXT_BVC,    IC_N, ISZ_NONE, SZ_NONE, M_REL16),               // BVC d:16
    E1(0x39, CF_00, TEXT_BVS,    IC_N, ISZ_NONE, SZ_NONE, M_REL16),               // BVS d:16
    E1(0x31, CF_00, TEXT_BF,     IC_N, ISZ_NONE, SZ_NONE, M_REL16),               // BF d:16
    E1(0x32, CF_00, TEXT_BHI,    IC_N, ISZ_NONE, SZ_NONE, M_REL16),               // BHI d:16
    E1(0x3C, CF_00, TEXT_BGE,    IC_N, ISZ_NONE, SZ_NONE, M_REL16),               // BGE d:16
    E1(0x3E, CF_00, TEXT_BGT,    IC_N, ISZ_NONE, SZ_NONE, M_REL16),               // BGT d:16
    E1(0x3F, CF_00, TEXT_BLE,    IC_N, ISZ_NONE, SZ_NONE, M_REL16),               // BLE d:16
    E1(0x31, CF_00, TEXT_BRN,    IC_N, ISZ_NONE, SZ_NONE, M_REL16),               // BRN d:16
    E2(0x40, CF_07, TEXT_CMP,    IC_E, ISZ_NONE, SZ_BYTE, M_IMM8,    M_REG),      // CMP:E #xx:8,Rd
    E2(0x48, CF_07, TEXT_CMP,    IC_I, ISZ_NONE, SZ_WORD, M_IMM16,   M_REG),      // CMP:I #xx:16,Rd
    E1(0x10, CF_00, TEXT_JMP,    IC_N, ISZ_NONE, SZ_NONE, M_ABS16),               // JMP @aa:16
    E1(0x18, CF_00, TEXT_JSR,    IC_N, ISZ_NONE, SZ_NONE, M_ABS16),               // JSR @aa:16
    E2(0x02, CF_00, TEXT_LDM,    IC_N, ISZ_NONE, SZ_WORD, M_SP,      M_REGLIST),  // LDM @SP+,(rlist)
    E2(0x17, CF_00, TEXT_LINK,   IC_N, ISZ_NONE, SZ_NONE, M_FP,      M_IMM8),     // LINK FP,#xx:8
    E2(0x1F, CF_00, TEXT_LINK,   IC_N, ISZ_NONE, SZ_NONE, M_FP,      M_IMM16),    // LINK FP,#xx:16
    E2(0x50, CF_07, TEXT_MOV,    IC_E, ISZ_NONE, SZ_BYTE, M_IMM8,    M_REG),      // MOV:E #xx:8,Rd
    E2(0x80, CF_0F, TEXT_MOV,    IC_F, ISZ_DATA, SZ_DATA, M_FPX8,    M_REG),      // MOV:F.B/.W @(d:8,R6),Rd
    E2(0x90, CF_0F, TEXT_MOV,    IC_F, ISZ_DATA, SZ_DATA, M_REG,     M_FPX8),     // MOV:F.B/.W Rs,@(d:8,R6)
    E2(0x58, CF_07, TEXT_MOV,    IC_I, ISZ_NONE, SZ_WORD, M_IMM16,   M_REG),      // MOV:I #xx:16,Rd
    E2(0x60, CF_0F, TEXT_MOV,    IC_L, ISZ_DATA, SZ_DATA, M_ABS8,    M_REG),      // MOV:L.B/.W @aa:8,Rd
    E2(0x70, CF_0F, TEXT_MOV,    IC_S, ISZ_DATA, SZ_DATA, M_REG,     M_ABS8),     // MOV:S.B/.W Rs,@aa:8
    E0(0x00,                 TEXT_NOP),                                           // NOP
    E1(0x14, CF_00, TEXT_RTD,    IC_N, ISZ_NONE, SZ_NONE, M_IMM8),                // RTD #xx:8
    E1(0x1C, CF_00, TEXT_RTD,    IC_N, ISZ_NONE, SZ_NONE, M_IMM16),               // RTD #xx:16
    E0(0x0A,                 TEXT_RTE),                                           // RTE
    E0(0x19,                 TEXT_RTS),                                           // RTS
    E2(0x07, CF_00, TEXT_SCB_EQ, IC_N, ISZ_NONE, SZ_NONE, M_SCB,     M_REL8),     // SCB/EQ Rn,d:8
    E2(0x01, CF_00, TEXT_SCB_F,  IC_N, ISZ_NONE, SZ_NONE, M_SCB,     M_REL8),     // SCB/F Rn,d:8
    E2(0x06, CF_00, TEXT_SCB_NE, IC_N, ISZ_NONE, SZ_NONE, M_SCB,     M_REL8),     // SCB/NE Rn,d:8
    E0(0x1A,                 TEXT_SLEEP),                                         // SLEEP
    E2(0x12, CF_00, TEXT_STM,    IC_N, ISZ_NONE, SZ_WORD, M_REGLIST, M_SP),       // STM (rlist),@-SP
    E0(0x09,                 TEXT_TRAP_VS),                                       // TRAP/VS
    E1(0x08, CF_00, TEXT_TRAPA,  IC_N, ISZ_NONE, SZ_NONE, M_TRAPV),               // TRAPA #vec
    E1(0x0F, CF_00, TEXT_UNLK,   IC_N, ISZ_NONE, SZ_NONE, M_FP),                  // UNLK FP
};

constexpr uint8_t INDEX_SPC[] PROGMEM = {
      0,  // TEXT_BCC
     21,  // TEXT_BCC
      1,  // TEXT_BCS
     22,  // TEXT_BCS
      2,  // TEXT_BEQ
     23,  // TEXT_BEQ
      3,  // TEXT_BF
     36,  // TEXT_BF
      4,  // TEXT_BGE
     38,  // TEXT_BGE
      5,  // TEXT_BGT
     39,  // TEXT_BGT
      6,  // TEXT_BHI
     37,  // TEXT_BHI
      7,  // TEXT_BHS
     24,  // TEXT_BHS
      8,  // TEXT_BLE
     40,  // TEXT_BLE
      9,  // TEXT_BLO
     25,  // TEXT_BLO
     10,  // TEXT_BLS
     26,  // TEXT_BLS
     11,  // TEXT_BLT
     27,  // TEXT_BLT
     12,  // TEXT_BMI
     28,  // TEXT_BMI
     13,  // TEXT_BNE
     29,  // TEXT_BNE
     14,  // TEXT_BPL
     30,  // TEXT_BPL
     15,  // TEXT_BRA
     31,  // TEXT_BRA
     16,  // TEXT_BRN
     41,  // TEXT_BRN
     17,  // TEXT_BSR
     32,  // TEXT_BSR
     18,  // TEXT_BT
     33,  // TEXT_BT
     19,  // TEXT_BVC
     34,  // TEXT_BVC
     20,  // TEXT_BVS
     35,  // TEXT_BVS
     42,  // TEXT_CMP
     43,  // TEXT_CMP
     44,  // TEXT_JMP
     45,  // TEXT_JSR
     46,  // TEXT_LDM
     47,  // TEXT_LINK
     48,  // TEXT_LINK
     49,  // TEXT_MOV
     50,  // TEXT_MOV
     51,  // TEXT_MOV
     52,  // TEXT_MOV
     53,  // TEXT_MOV
     54,  // TEXT_MOV
     55,  // TEXT_NOP
     56,  // TEXT_RTD
     57,  // TEXT_RTD
     58,  // TEXT_RTE
     59,  // TEXT_RTS
     60,  // TEXT_SCB
     61,  // TEXT_SCB
     62,  // TEXT_SCB
     63,  // TEXT_SLEEP
     64,  // TEXT_STM
     65,  // TEXT_TRAP
     66,  // TEXT_TRAPA
     67,  // TEXT_UNLK
};

// Secondary (0x11-prefix) instructions, all CPUs: JMP/JSR @Rn / @(d,Rn).
constexpr Entry TABLE_SEC[] PROGMEM = {
    E1(0xD0, CF_07, TEXT_JMP,    IC_N, ISZ_NONE, SZ_NONE, M_IND),                 // JMP @Rn
    E1(0xE0, CF_07, TEXT_JMP,    IC_N, ISZ_NONE, SZ_NONE, M_IDX8),                // JMP @(d:8,Rn)
    E1(0xF0, CF_07, TEXT_JMP,    IC_N, ISZ_NONE, SZ_NONE, M_IDX16),               // JMP @(d:16,Rn)
    E1(0xD8, CF_07, TEXT_JSR,    IC_N, ISZ_NONE, SZ_NONE, M_IND),                 // JSR @Rn
    E1(0xE8, CF_07, TEXT_JSR,    IC_N, ISZ_NONE, SZ_NONE, M_IDX8),                // JSR @(d:8,Rn)
    E1(0xF8, CF_07, TEXT_JSR,    IC_N, ISZ_NONE, SZ_NONE, M_IDX16),               // JSR @(d:16,Rn)
};

constexpr uint8_t INDEX_SEC[] PROGMEM = {
      0,  // TEXT_JMP
      1,  // TEXT_JMP
      2,  // TEXT_JMP
      3,  // TEXT_JSR
      4,  // TEXT_JSR
      5,  // TEXT_JSR
};

// Page-jump SPC instructions (maximum-mode, all variants): PJMP/PJSR @aa:24.
constexpr Entry TABLE_SPC_PJ[] PROGMEM = {
    E1(0x03, CF_00, TEXT_PJSR,   IC_N, ISZ_NONE, SZ_NONE, M_ABS24),               // PJSR @aa:24
    E1(0x13, CF_00, TEXT_PJMP,   IC_N, ISZ_NONE, SZ_NONE, M_ABS24),               // PJMP @aa:24
};

constexpr uint8_t INDEX_SPC_PJ[] PROGMEM = {
      1,  // TEXT_PJMP
      0,  // TEXT_PJSR
};

// Page-jump SEC instructions (maximum-mode, all variants): PJMP/PJSR @Rn, PRTS, PRTD.
constexpr Entry TABLE_SEC_PJ[] PROGMEM = {
    E1(0xC0, CF_07, TEXT_PJMP,   IC_N, ISZ_NONE, SZ_NONE, M_INDP),                // PJMP @Rn
    E1(0xC8, CF_07, TEXT_PJSR,   IC_N, ISZ_NONE, SZ_NONE, M_INDP),                // PJSR @Rn
    E1(0x14, CF_00, TEXT_PRTD,   IC_N, ISZ_NONE, SZ_NONE, M_IMM8),                // PRTD #xx:8
    E1(0x1C, CF_00, TEXT_PRTD,   IC_N, ISZ_NONE, SZ_NONE, M_IMM16),               // PRTD #xx:16
    E0(0x19,                 TEXT_PRTS),                                          // PRTS
};

constexpr uint8_t INDEX_SEC_PJ[] PROGMEM = {
      0,  // TEXT_PJMP
      1,  // TEXT_PJSR
      2,  // TEXT_PRTD
      3,  // TEXT_PRTD
      4,  // TEXT_PRTS
};

#undef E2
#undef E1
#undef E0

// clang-format on

// EntryPage stores the EA/prefix classifier entirely in the generic 16-bit
// prefix slot; the constructor takes the fields explicitly and packs them with
// the named position/mask constants below (no LEAD macro, no extra field). The
// EA-byte mask is not stored -- it is a pure function of the resolved mode.
struct EntryPage : entry::PrefixTableBase<Entry> {
private:
    // prefix slot layout: [15:8]=eaMatch [7:6]=pm [4]=maxModeOnly [3:0]=eaMode
    static constexpr int match_gp = 8;
    static constexpr int pm_gp = 6;
    static constexpr int mmo_gp = 4;
    static constexpr int mode_gp = 0;
    static constexpr uint_fast16_t match_gm = 0xFF;
    static constexpr uint_fast16_t pm_gm = 0x3;
    static constexpr uint_fast16_t mmo_gm = 0x1;
    static constexpr uint_fast16_t mode_gm = 0xF;

    static constexpr uint16_t pack(
            uint8_t eaMatch, PrefixMode pm, AddrMode eaMode, bool maxModeOnly) {
        return static_cast<uint16_t>((static_cast<uint16_t>(eaMatch) << match_gp) |
                                     (static_cast<uint16_t>(pm) << pm_gp) |
                                     (static_cast<uint16_t>(maxModeOnly ? 1 : 0) << mmo_gp) |
                                     (static_cast<uint16_t>(eaMode) << mode_gp));
    }

public:
    constexpr EntryPage(uint8_t eaMatch, PrefixMode pm, AddrMode eaMode, const Entry *head_P,
            const Entry *tail_P, const uint8_t *index_P, const uint8_t *itail_P,
            bool maxModeOnly = false)
        : PrefixTableBase(
                  pack(eaMatch, pm, eaMode, maxModeOnly), head_P, tail_P, index_P, itail_P) {}

    uint8_t eaMatch() const { return (readPrefix() >> match_gp) & match_gm; }
    PrefixMode pm() const { return PrefixMode((readPrefix() >> pm_gp) & pm_gm); }
    AddrMode eaMode() const { return AddrMode((readPrefix() >> mode_gp) & mode_gm); }
    // Page-jump pages are valid only in maximum mode; both the assembler and
    // the disassembler hide them in minimum mode.
    bool maxModeOnly() const { return (readPrefix() >> mmo_gp) & mmo_gm; }

    // The EA-byte mask follows the resolved mode: reg-field for register/
    // indirect/displacement modes, Sz for @aa, exact for #imm / no-EA.
    CodeFormat eaMaskSel() const {
        switch (eaMode()) {
        case M_ABS8:
        case M_ABS16:
            return CF_08;
        case M_IMM8:
        case M_IMM16:
        case M_NONE:
            return CF_00;
        default:
            return CF_0F;
        }
    }
    bool eaMatches(uint8_t b) const { return (b & ~CODE_MASK[eaMaskSel()]) == eaMatch(); }
};

// clang-format off

// One row per EA mode for GEN; all share TABLE_GEN. Reused by every CPU.
#define GEN_PAGES                                                                      \
    {0xA0, PM_GEN, M_REG,     ARRAY_RANGE(TABLE_GEN), ARRAY_RANGE(INDEX_GEN)},  \
    {0xB0, PM_GEN, M_PDEC,    ARRAY_RANGE(TABLE_GEN), ARRAY_RANGE(INDEX_GEN)},  \
    {0xC0, PM_GEN, M_PINC,    ARRAY_RANGE(TABLE_GEN), ARRAY_RANGE(INDEX_GEN)},  \
    {0xD0, PM_GEN, M_IND,     ARRAY_RANGE(TABLE_GEN), ARRAY_RANGE(INDEX_GEN)},  \
    {0xE0, PM_GEN, M_IDX8,    ARRAY_RANGE(TABLE_GEN), ARRAY_RANGE(INDEX_GEN)},  \
    {0xF0, PM_GEN, M_IDX16,   ARRAY_RANGE(TABLE_GEN), ARRAY_RANGE(INDEX_GEN)},  \
    {0x05, PM_GEN, M_ABS8,    ARRAY_RANGE(TABLE_GEN), ARRAY_RANGE(INDEX_GEN)},  \
    {0x15, PM_GEN, M_ABS16,   ARRAY_RANGE(TABLE_GEN), ARRAY_RANGE(INDEX_GEN)},  \
    {0x04, PM_GEN, M_IMM8,    ARRAY_RANGE(TABLE_GEN), ARRAY_RANGE(INDEX_GEN)},  \
    {0x0C, PM_GEN, M_IMM16,   ARRAY_RANGE(TABLE_GEN), ARRAY_RANGE(INDEX_GEN)}

// One row per (reg/mem) EA mode for EXT, pointing at |table|/|index|.
#define EXT_PAGES(table, index)                                                   \
    {0xA0, PM_EXT, M_REG,     ARRAY_RANGE(table), ARRAY_RANGE(index)},     \
    {0xB0, PM_EXT, M_PDEC,    ARRAY_RANGE(table), ARRAY_RANGE(index)},     \
    {0xC0, PM_EXT, M_PINC,    ARRAY_RANGE(table), ARRAY_RANGE(index)},     \
    {0xD0, PM_EXT, M_IND,     ARRAY_RANGE(table), ARRAY_RANGE(index)},     \
    {0xE0, PM_EXT, M_IDX8,    ARRAY_RANGE(table), ARRAY_RANGE(index)},     \
    {0xF0, PM_EXT, M_IDX16,   ARRAY_RANGE(table), ARRAY_RANGE(index)},     \
    {0x05, PM_EXT, M_ABS8,    ARRAY_RANGE(table), ARRAY_RANGE(index)},     \
    {0x15, PM_EXT, M_ABS16,   ARRAY_RANGE(table), ARRAY_RANGE(index)}

// Page jumps (PJMP/PJSR/PRTS/PRTD) are a maximum-mode feature available on every
// variant; both the assembler and the disassembler gate them by operating mode,
// so every page list carries the *_PJ pages (maxModeOnly=true).
#define MAXMODE_PAGES                                                                          \
    {0x00, PM_SPC, M_NONE, ARRAY_RANGE(TABLE_SPC_PJ), ARRAY_RANGE(INDEX_SPC_PJ), true},   \
    {0x11, PM_SEC, M_NONE, ARRAY_RANGE(TABLE_SEC_PJ), ARRAY_RANGE(INDEX_SEC_PJ), true}

// The register-EA page for XCH precedes GEN so it claims OP 0x90 for a register
// EA; every other register-EA opcode falls through to the shared TABLE_GEN.
#define XCH_PAGE \
    {0xA0, PM_GEN, M_REG, ARRAY_RANGE(TABLE_XCH), ARRAY_RANGE(INDEX_XCH)}

// The special-format page precedes the general page so a bare stem assembles to
// the shorter special form (MOV:E/F/I/L/S, CMP:E/I) before falling to MOV:G/
// CMP:G. Page order is name-search only; disassembly filters pages by prefix
// mode, so the special page never shadows a general-format opcode.
#define SPC_PAGE {0x00, PM_SPC, M_NONE, ARRAY_RANGE(TABLE_SPC), ARRAY_RANGE(INDEX_SPC)}
#define SEC_PAGE {0x11, PM_SEC, M_NONE, ARRAY_RANGE(TABLE_SEC), ARRAY_RANGE(INDEX_SEC)}

// ORDER-SENSITIVE: page order is load-bearing for the assembler's name search.
// SPC must precede the GEN pages (bare-stem picks the short MOV:E/F/.. over
// MOV:G), XCH must precede GEN (claims OP 0x90 for a register EA), and within
// TABLE_GEN the ADD:Q rows precede ADD:G. Disassembly is unaffected (pages are
// filtered by prefix mode). Do not reorder these for tidiness.
constexpr EntryPage H8500_PAGES[] PROGMEM = {
        SPC_PAGE,
        XCH_PAGE,
        GEN_PAGES,
        EXT_PAGES(TABLE_EXT, INDEX_EXT),
        EXT_PAGES(TABLE_ECLK, INDEX_ECLK),
        SEC_PAGE,
        MAXMODE_PAGES,
};

// H8/520: no E-clock, so no TABLE_ECLK pages.
constexpr EntryPage H8520_PAGES[] PROGMEM = {
        SPC_PAGE,
        XCH_PAGE,
        GEN_PAGES,
        EXT_PAGES(TABLE_EXT, INDEX_EXT),
        SEC_PAGE,
        MAXMODE_PAGES,
};

constexpr EntryPage H8530_PAGES[] PROGMEM = {
        SPC_PAGE,
        XCH_PAGE,
        GEN_PAGES,
        EXT_PAGES(TABLE_EXT, INDEX_EXT),
        EXT_PAGES(TABLE_ECLK, INDEX_ECLK),
        SEC_PAGE,
        MAXMODE_PAGES,
};
#undef SPC_PAGE
#undef SEC_PAGE
#undef XCH_PAGE
#undef GEN_PAGES
#undef EXT_PAGES
#undef MAXMODE_PAGES

// clang-format on

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {H8_500, TEXT_CPU_H8500, ARRAY_RANGE(H8500_PAGES)},
        {H8_520, TEXT_CPU_H8520, ARRAY_RANGE(H8520_PAGES)},
        {H8_530, TEXT_CPU_H8530, ARRAY_RANGE(H8530_PAGES)},
};

static const Cpu *cpu(CpuType cpuType) {
    return Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
}

// Number of EA extension bytes for the resolved EA mode.
static uint8_t extOf(AddrMode mode) {
    switch (mode) {
    case M_IDX16:
    case M_ABS16:
    case M_IMM16:
        return 2;
    case M_IDX8:
    case M_FPX8:
    case M_ABS8:
    case M_IMM8:
        return 1;
    default:
        return 0;  // M_REG, M_IND, M_PDEC, M_PINC
    }
}

int classifyLead(CpuType cpuType, uint8_t b, AddrMode &mode) {
    const auto *c = cpu(cpuType);
    for (const auto *page = c->pagesBegin(); page < c->pagesEnd(); page++) {
        const auto pm = page->pm();
        if ((pm == PM_GEN || pm == PM_EXT) && page->eaMatches(b)) {
            mode = page->eaMode();
            return extOf(mode);
        }
    }
    return -1;
}

bool isPrefix(CpuType /*cpuType*/, Config::opcode_t code) {
    return code == 0x11;
}

// The EA-operand slot of a GEN/EXT entry (M_EASRC/M_EADST/M_IMM*), else M_NONE.
static AddrMode eaSlotOf(Entry::Flags f) {
    const auto s = f.src();
    const auto d = f.dst();
    // The M_EASRC/M_EADST placeholder is the EA; prefer it (MOV:G #xx,<EAd> has
    // an EA dst and an immediate src that is trailing data, not the EA). Only
    // when neither side is a placeholder does an immediate act as the EA (ANDC).
    if (d == M_EASRC || d == M_EADST || d == M_EAMEM)
        return d;
    if (s == M_EASRC || s == M_EADST || s == M_EAREG || s == M_EAMEM)
        return s;
    if (s == M_IMM8 || s == M_IMM16)
        return s;
    if (d == M_IMM8 || d == M_IMM16)
        return d;
    return M_NONE;
}

// Does the entry's EA slot accept the resolved EA mode? This is where the
// immediate-vs-reg/mem restriction lives (e.g. ANDC needs #imm, BCLR can't).
static bool eaSlotAccepts(AddrMode slot, AddrMode eaMode) {
    switch (slot) {
    case M_EASRC:
        return true;
    case M_EAREG:
        return eaMode == M_REG;
    case M_EAMEM:
        return eaMode >= M_IND && eaMode <= M_ABS16;
    case M_EADST:
        return eaMode != M_IMM8 && eaMode != M_IMM16;
    case M_IMM8:
    case M_IMM16:
        return eaMode == M_IMM8 || eaMode == M_IMM16;
    default:
        return false;
    }
}

static bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *page) {
    const auto flags = entry->readFlags();
    const auto pm = page->pm();
    if (pm == PM_GEN || pm == PM_EXT) {
        if (!eaSlotAccepts(eaSlotOf(flags), insn.eaMode))
            return false;
        return (insn.opByte & ~flags.mask()) == entry->readOpCode();
    }
    // PM_SPC / PM_SEC: no EA byte.
    return (insn.opCode() & ~flags.mask()) == entry->readOpCode();
}

static bool pageMatcher(DisInsn &insn, const EntryPage *page) {
    // Page jumps are maxMode-only; skip those pages in minimum mode (-> unknown).
    if (page->maxModeOnly() && !insn.maxMode)
        return false;
    if (page->pm() != insn.prefixMode())
        return false;
    const auto pm = insn.prefixMode();
    if (pm == PM_GEN || pm == PM_EXT)
        return page->eaMatches(insn.eaByte);
    return true;
}

Error searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) {
    cpu(cpuType)->searchOpCode(insn, out, matchOpCode, pageMatcher);
    return insn.getError();
}

static bool isEaMode(AddrMode mode) {
    return mode >= M_REG && mode <= M_IMM16;
}

// Accept |op| as a value for the |table|-declared mode (assembler). |insnSz| is
// the parsed .B/.W suffix, used to widen an EA-immediate slot to word.
static bool acceptMode(const Operand &op, AddrMode table, OprSize insnSz) {
    if (op.mode == table)
        return true;
    // A no-suffix absolute that overflowed 16 bits (parsed M_ABS24) has no
    // @aa:24 data form; it is accepted by the EA / @aa:16 / @aa:8 slots and
    // shortened (high byte dropped, page-validated at emit). An explicit :size
    // (absSize != 0) is honored as written.
    const bool absDowngrade = op.mode == M_ABS24 && op.absSize == 0;
    switch (table) {
    case M_EASRC:
        return isEaMode(op.mode) || absDowngrade;
    case M_EAREG:
        return op.mode == M_REG;
    case M_EAMEM:
        return (op.mode >= M_IND && op.mode <= M_ABS16) || absDowngrade;
    case M_EADST:
        return (isEaMode(op.mode) || absDowngrade) && op.mode != M_IMM8 && op.mode != M_IMM16;
    case M_ABS8:
        // MOV:L/MOV:S @aa:8: a no-suffix absolute is re-formed to it (BR-paged).
        return op.absSize == 0 && (op.mode == M_ABS16 || op.mode == M_ABS24);
    case M_ABS16:
        // JMP/JSR @aa:16: a no-suffix >64K address shortens to it (CP-paged).
        return absDowngrade;
    case M_IMM8:
        // ANDC/ORC/XORC .w widens the immediate-EA operand to 16 bits.
        return op.mode == M_IMM16 && insnSz == SZ_WORD;
    case M_IMM16:
        // MOV:I/CMP:I are the explicit 16-bit forms: a value that parsed as
        // imm8 (e.g. a small or negative #-1) still encodes as a word.
        return op.mode == M_IMM8;
    case M_BIT:
        // A .W bit op widens the (bit-number) immediate to M_IMM16; accept it.
        return op.mode == M_IMM8 || op.mode == M_IMM16;
    case M_IMM2:
        // ADD:Q encodes the quick value (+/-1, +/-2) in the OP byte, so a .W that
        // widened the immediate to M_IMM16 is still acceptable; the value range
        // is checked in acceptOperands (bare stem) or the encoder (explicit :Q).
        return op.mode == M_IMM8 || op.mode == M_IMM16;
    case M_TRAPV:
        return op.mode == M_IMM8;
    case M_SCB:
        return op.mode == M_REG;
    case M_FP:
        return op.mode == M_REG && op.reg == REG_FP;
    case M_SP:
        // LDM @SP+ (post-inc) / STM @-SP (pre-dec): the stack pointer R7.
        return (op.mode == M_PINC || op.mode == M_PDEC) && op.reg == REG_SP;
    case M_IDX8:
        return op.mode == M_FPX8;
    case M_REGP:
        // MULXU/DIVXU.W Rd register pair: parsed as a plain register; the encoder
        // flags an odd (misaligned) register as REGISTER_NOT_ALIGNED.
        return op.mode == M_REG;
    case M_INDP:
        // PJMP/PJSR @Rn register pair: parsed as @Rn; the encoder flags an odd
        // (misaligned) register as REGISTER_NOT_ALIGNED.
        return op.mode == M_IND;
    default:
        return false;
    }
}

static bool acceptOperands(AsmInsn &insn, const Entry *entry) {
    // Page jumps live on maxMode-only pages; reject them in minimum mode.
    if (insn.currentPageMaxOnly && !insn.maxMode)
        return false;
    const auto flags = entry->readFlags();
    // An explicit ":x" class (MOV:F etc) matches only that class; a bare stem
    // (IC_N) matches every class and the page/index order picks the optimum.
    if (insn.insnCls != IC_N && flags.insnClass() != insn.insnCls)
        return false;
    // A written .B/.W must agree with a fixed byte/word operand size. This
    // selects the right MOV:G immediate (0x06/0x07) and, for a bare stem, skips
    // wrong-sized classes so the matching one is reached (mov.w -> MOV:I not
    // MOV:E). With an explicit class a size mismatch is left to encodeImpl's
    // ILLEGAL_SIZE check instead of silently skipping the entry.
    const auto osz = flags.oprSize();
    if (insn.insnSz != SZ_NONE && (osz == SZ_BYTE || osz == SZ_WORD) && insn.insnSz != osz &&
            (insn.insnCls == IC_N || flags.insnSize() == ISZ_DATA))
        return false;
    // A bare stem with no size defaults to word like ASL/GAS: skip a byte-fixed
    // class form (MOV:E/CMP:E, MOV:G.B #imm) so the word form (MOV:I/CMP:I,
    // MOV:G.W) is chosen. Byte-only ops with no class (EXTS/DADD/...) are kept.
    if (insn.insnSz == SZ_NONE && insn.insnCls == IC_N && osz == SZ_BYTE &&
            flags.insnClass() != IC_N)
        return false;
    // Optimal selection for a bare stem: a short form that can't encode the
    // operand value is skipped so the search reaches the general form. ADD:Q
    // only encodes +/-1, +/-2; a larger quick value falls through to ADD:G. An
    // explicit ADD:Q keeps its OVERFLOW_RANGE error from the encoder instead.
    if (insn.insnCls == IC_N && (flags.src() == M_IMM2 || flags.dst() == M_IMM2)) {
        const auto &q = (flags.src() == M_IMM2) ? insn.op1 : insn.op2;
        const auto v = q.val.getSigned();
        if (q.hasError() || (v != 1 && v != 2 && v != -1 && v != -2))
            return false;
    }
    return acceptMode(insn.op1, flags.src(), insn.insnSz) &&
           acceptMode(insn.op2, flags.dst(), insn.insnSz);
}

// pageSetup runs before each page's name search; stash its maxMode-only flag so
// acceptOperands can reject page-jump entries in minimum mode.
static void pageSetup(AsmInsn &insn, const EntryPage *page) {
    insn.currentPageMaxOnly = page->maxModeOnly();
}

// readCode runs once for the matched entry; record the page's PrefixMode (the
// EA byte / 0x11 prefix is rebuilt from operands by the encoder, so the generic
// prefix-setting readCode is not used).
static void readCodeName(AsmInsn &insn, const Entry *entry, const EntryPage *page) {
    insn.setPrefixMode(page->pm());
    insn.setOpCode(entry->readOpCode());
    insn.setFlags(entry->readFlags());
}

Error searchName(CpuType cpuType, AsmInsn &insn) {
    cpu(cpuType)->searchName(insn, acceptOperands, pageSetup, readCodeName);
    return insn.getError();
}

const /*PROGMEM*/ char *TableH8500::listCpu_P() const {
    return text::h8500::TEXT_H8500_LIST;
}

const /*PROGMEM*/ char *TableH8500::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableH8500::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    const auto *e = Cpu::search(name, ARRAY_RANGE(CPU_TABLE));
    if (e) {
        cpuType = e->readCpuType();
        return OK;
    }
    return UNSUPPORTED_CPU;
}

const TableH8500 TABLE;

}  // namespace h8500
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

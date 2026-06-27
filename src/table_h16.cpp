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

#include "table_h16.h"
#include "entry_h16.h"
#include "entry_table.h"
#include "text_h16.h"

using namespace libasm::text::h16;

namespace libasm {
namespace h16 {

#define E3(opc, name, cls, isz, mode1, mode2, mode3) \
    {opc, Entry::Flags::create(isz, cls, mode1, mode2, mode3), name}
#define E2(opc, name, cls, isz, mode1, mode2) E3(opc, name, cls, isz, mode1, mode2, M_NONE)
#define E1(opc, name, cls, isz, mode1) E2(opc, name, cls, isz, mode1, M_NONE)
#define E0(opc, name) E2(opc, name, IC_N, ISZ_NONE, M_NONE, M_NONE)
// ISZ_FIXED forms: fixed operand size |osz| accepted as an optional suffix.
#define F2(opc, name, cls, osz, mode1, mode2) \
    {opc, Entry::Flags::create(ISZ_FIXED, cls, mode1, mode2, M_NONE, osz), name}
#define F1(opc, name, cls, osz, mode1) F2(opc, name, cls, osz, mode1, M_NONE)
#define F0(opc, name, osz) F2(opc, name, IC_N, osz, M_NONE, M_NONE)

// ============================================================================
// Main page: single-byte opcodes.  Sized entries have Sz in the low 2 bits of
// the opcode; matchOpCode masks those bits.
// ============================================================================
constexpr Entry TABLE_MAIN[] PROGMEM = {
    // ADD/SUB/CMP/MOV families: same stem, class disambiguates.  Listed per
    // stem in optimal-preference order RQ > R > Q > G (smallest first) so a
    // class-less mnemonic picks the smallest fitting form.
    E1(0x30, TEXT_ADD,    IC_RQ, ISZ_DATA,  M_IRQ),             // ADD:RQ imm4u,Rnd
    E1(0x20, TEXT_ADD,    IC_R,  ISZ_DATA,  M_RR),              // ADD:R  Rns,Rnd
    E2(0x10, TEXT_ADD,    IC_Q,  ISZ_DATA,  M_IQ,    M_EADST),  // ADD:Q  imm8s,EAd
    E2(0x00, TEXT_ADD,    IC_G,  ISZ_DATA,  M_EASRC, M_EADST),  // ADD:G  .B/.W/.L
    E1(0x34, TEXT_SUB,    IC_RQ, ISZ_DATA,  M_IRQ),             // SUB:RQ (no :Q)
    E1(0x24, TEXT_SUB,    IC_R,  ISZ_DATA,  M_RR),              // SUB:R
    E2(0x04, TEXT_SUB,    IC_G,  ISZ_DATA,  M_EASRC, M_EADST),  // SUB:G
    E1(0x38, TEXT_CMP,    IC_RQ, ISZ_DATA,  M_SIRQ),            // CMP:RQ imm4s,Rnd
    E1(0x28, TEXT_CMP,    IC_R,  ISZ_DATA,  M_RR),              // CMP:R
    E2(0x18, TEXT_CMP,    IC_Q,  ISZ_DATA,  M_IQ,    M_EASRC),  // CMP:Q
    E2(0x08, TEXT_CMP,    IC_G,  ISZ_DATA,  M_EASRC, M_EASRC),  // CMP:G (no writeback)
    E1(0x3C, TEXT_MOV,    IC_RQ, ISZ_DATA,  M_SIRQ),            // MOV:RQ imm4s,Rnd
    E1(0x2C, TEXT_MOV,    IC_R,  ISZ_DATA,  M_RR),              // MOV:R
    E2(0x1C, TEXT_MOV,    IC_Q,  ISZ_DATA,  M_IQ,    M_EADST),  // MOV:Q
    E2(0x0C, TEXT_MOV,    IC_G,  ISZ_DATA,  M_EASRC, M_EADST),  // MOV:G
    E1(0x14, TEXT_CLR,    IC_N,  ISZ_DATA,  M_EADST),           // CLR
    E2(0x40, TEXT_ADDS,   IC_N,  ISZ_DATA,  M_EASRC, M_EADST),  // ADDS   sign-extend
    E2(0x44, TEXT_SUBS,   IC_N,  ISZ_DATA,  M_EASRC, M_EADST),  // SUBS
    E2(0x48, TEXT_CMPS,   IC_N,  ISZ_DATA,  M_EASRC, M_EASRC),  // CMPS
    E2(0x4C, TEXT_MOVS,   IC_N,  ISZ_DATA,  M_EASRC, M_EADST),  // MOVS
    E2(0x50, TEXT_ADDX,   IC_N,  ISZ_DATA,  M_EASRC, M_EADST),  // ADDX   add with CX
    E2(0x54, TEXT_SUBX,   IC_N,  ISZ_DATA,  M_EASRC, M_EADST),  // SUBX
    E1(0x58, TEXT_TST,    IC_N,  ISZ_DATA,  M_EASRC),           // TST    read-only
    E1(0x5C, TEXT_MOVF,   IC_N,  ISZ_DATA,  M_EADST),           // MOVF   from R0
    E2(0x70, TEXT_STM,    IC_N,  ISZ_DATA,  M_RLIST, M_EADST),  // STM <reglist>,<EAd> (.B/.W/.L)
    E2(0x74, TEXT_LDM,    IC_N,  ISZ_DATA,  M_EASRC, M_RLIST),  // LDM <EAs>,<reglist> (.B/.W/.L)
    E2(0x78, TEXT_MOVTPE, IC_N,  ISZ_DATA,  M_EASRC, M_EAPERI), // MOVTPE EAs,<periEAd> (.B/.W/.L)
    E2(0x7C, TEXT_MOVFPE, IC_N,  ISZ_DATA,  M_EAPERI, M_EADST), // MOVFPE <periEAs>,EAd (.B/.W/.L)
    E2(0x80, TEXT_AND,    IC_N,  ISZ_DATA,  M_EASRC, M_EADST),  // AND
    F2(0x83, TEXT_DADD,   IC_N,  SZ_BYTE,   M_EASRC, M_EADST),  // DADD   decimal add (byte)
    E2(0x84, TEXT_XOR,    IC_N,  ISZ_DATA,  M_EASRC, M_EADST),  // XOR
    F2(0x87, TEXT_DSUB,   IC_N,  SZ_BYTE,   M_EASRC, M_EADST),  // DSUB   decimal sub (byte)
    E2(0x88, TEXT_OR,     IC_N,  ISZ_DATA,  M_EASRC, M_EADST),  // OR
    E1(0x8C, TEXT_NEG,    IC_N,  ISZ_DATA,  M_EADST),           // NEG
    E1(0x90, TEXT_NOT,    IC_N,  ISZ_DATA,  M_EADST),           // NOT
    E1(0x98, TEXT_BRA,    IC_N,  ISZ_DATA,  M_DISP),            // BRA    disp .B/.W/.L
    E1(0x9B, TEXT_JMP,    IC_N,  ISZ_NONE,  M_EAMEM),          // JMP    via EA (no Rn/imm)
    E1(0x9C, TEXT_NEGX,   IC_N,  ISZ_DATA,  M_EADST),           // NEGX   negate with CX
    E1(0xA0, TEXT_BEQ,    IC_N,  ISZ_DATA,  M_DISP),            // BEQ    (short)
    E1(0xA8, TEXT_BSR,    IC_N,  ISZ_DATA,  M_DISP),            // BSR
    E1(0xAB, TEXT_JSR,    IC_N,  ISZ_NONE,  M_EAMEM),          // JSR    via EA (no Rn/imm)
    E1(0xAC, TEXT_EXTU,   IC_N,  ISZ_EXTU,  M_RNNO),            // EXTU   Sz: 0=W,1=L,2=B
    F1(0xAF, TEXT_DNEG,   IC_N,  SZ_BYTE,   M_EADST),           // DNEG   decimal neg (byte)
    E1(0xB0, TEXT_BNE,    IC_N,  ISZ_DATA,  M_DISP),            // BNE    (short)
    F1(0xB3, TEXT_XCH,    IC_N,  SZ_LONG,   M_RR),              // XCH    exchange regs (long)
    E2(0xB4, TEXT_SCB,    IC_N,  ISZ_DATA,  M_RCC,   M_DISP),   // SCB/cc Rn, disp (.B/.W/.L disp)
    F1(0xB7, TEXT_SET,    IC_CC, SZ_BYTE,   M_EADST),           // SET/cc <EAd> (byte)
    E1(0xB8, TEXT_RTD,    IC_N,  ISZ_DATA,  M_FRMSZ),           // RTD    deallocate (.B/.W/.L)
    E0(0xBB, TEXT_RTS),                                         // RTS
    E1(0xBC, TEXT_EXTS,   IC_N,  ISZ_EXTU,  M_RNNO),            // EXTS   Sz: 0=W,1=L,2=B
    F2(0xBF, TEXT_MOVA,   IC_N,  SZ_LONG,   M_EASRC, M_EADST),  // MOVA   move EA to R0 (long)
    E2(0xD0, TEXT_LINK,   IC_N,  ISZ_DATA,  M_RNNO,  M_FRMSZ),  // LINK   Rn,#frame (.B/.W/.L)
    E1(0xD3, TEXT_UNLK,   IC_N,  ISZ_NONE,  M_RNNO),            // UNLK
    // BFEXT/BFINS/BFSCH: Rnb,Rnf then EAs,EAd.  Three forms by how many of the
    // two EAs are memory vs register-direct (folded into M_BITF).
    E1(0xD4, TEXT_BFEXT,  IC_N,  ISZ_NONE,  M_BITF),                    // EAs,EAd reg
    E2(0xD4, TEXT_BFEXT,  IC_N,  ISZ_NONE,  M_BITF, M_EADST),          // EAd memory
    E3(0xD4, TEXT_BFEXT,  IC_N,  ISZ_NONE,  M_BITF, M_EASRC, M_EADST), // EAs,EAd memory
    E1(0xD5, TEXT_BFINS,  IC_N,  ISZ_NONE,  M_BITF),
    E2(0xD5, TEXT_BFINS,  IC_N,  ISZ_NONE,  M_BITF, M_EADST),
    E3(0xD5, TEXT_BFINS,  IC_N,  ISZ_NONE,  M_BITF, M_EASRC, M_EADST),
    E1(0xD6, TEXT_BFSCH,  IC_N,  ISZ_NONE,  M_BITF),
    E2(0xD6, TEXT_BFSCH,  IC_N,  ISZ_NONE,  M_BITF, M_EADST),
    E3(0xD6, TEXT_BFSCH,  IC_N,  ISZ_NONE,  M_BITF, M_EASRC, M_EADST),
    E1(0xD7, TEXT_BFMOV,  IC_N,  ISZ_NONE,  M_BFMOV),           // BFMOV 6 registers
    E2(0xE0, TEXT_MOVTP,  IC_N,  ISZ_OWL,   M_EASRC, M_EAPERI), // MOVTP  EAs,<periEAd> (.W/.L, bit0)
    E2(0xE2, TEXT_MOVFP,  IC_N,  ISZ_OWL,   M_EAPERI, M_EADST), // MOVFP  <periEAs>,EAd (.W/.L, bit0)
    E1(0xE4, TEXT_CGBN,   IC_N,  ISZ_NONE,  M_RNNO),            // CGBN
    F0(0xE8, TEXT_PGBN,          SZ_BYTE),                      // PGBN (no list, opt .B)
    F1(0xE9, TEXT_PGBN,   IC_N,  SZ_BYTE,   M_RLIST),           // PGBN <reglist> (bit0 set)
    E1(0xEA, TEXT_SWAP,   IC_N,  ISZ_OBIT0, M_EADST),           // SWAP   swap halves (.B/.W, bit0)
    F1(0xEC, TEXT_TAS,    IC_N,  SZ_BYTE,   M_EADST),           // TAS    test and set (byte)
    E0(0xF0, TEXT_RESET),                                       // RESET
    E0(0xF1, TEXT_RTE),                                         // RTE
    E1(0xF2, TEXT_TRAPA,  IC_N,  ISZ_NONE,  M_TRAPV),           // TRAPA  #imm4
    E2(0xF3, TEXT_TRAP,   IC_CC, ISZ_NONE,  M_NONE,  M_NONE),   // TRAP/cc (cc byte)
    E0(0xF4, TEXT_RTR),                                         // RTR
    E0(0xF5, TEXT_SLEEP),                                       // SLEEP
    E2(0xF8, TEXT_ANDC,   IC_N,  ISZ_NONE,  M_EASRC, M_CR),     // ANDC   EA,CR
    E2(0xF9, TEXT_ORC,    IC_N,  ISZ_NONE,  M_EASRC, M_CR),     // ORC
    E2(0xFA, TEXT_XORC,   IC_N,  ISZ_NONE,  M_EASRC, M_CR),     // XORC
    E2(0xFB, TEXT_LDC,    IC_N,  ISZ_NONE,  M_EASRC, M_CR),     // LDC
    E2(0xFC, TEXT_STC,    IC_N,  ISZ_NONE,  M_CR,    M_EADST),  // STC
    F0(0xFD, TEXT_ICBN,          SZ_LONG),                      // ICBN (long)
    F0(0xFE, TEXT_DCBN,          SZ_LONG),                      // DCBN (long)
    E0(0xFF, TEXT_NOP),                                         // NOP
};

// ============================================================================
// SFT (shift/rotate) dynamic.  Prefix 0x60/0x61/0x62 = .B/.W/.L (Sz in prefix
// low 2 bits).  Opcode [MODE(3)|*|Rn(4)]: MODE bits 7-5 select the mnemonic.
// ============================================================================
constexpr Entry TABLE_SFT_DYN[] PROGMEM = {
    E2(0x00, TEXT_SHAR,   IC_N,  ISZ_PRSZ,  M_SCNTR, M_EADST),  // MODE=000 SHAR
    E2(0x20, TEXT_SHLR,   IC_N,  ISZ_PRSZ,  M_SCNTR, M_EADST),  // MODE=001 SHLR
    E2(0x40, TEXT_ROTR,   IC_N,  ISZ_PRSZ,  M_SCNTR, M_EADST),  // MODE=010 ROTR
    E2(0x60, TEXT_ROTXR,  IC_N,  ISZ_PRSZ,  M_SCNTR, M_EADST),  // MODE=011 ROTXR
    E2(0x80, TEXT_SHAL,   IC_N,  ISZ_PRSZ,  M_SCNTR, M_EADST),  // MODE=100 SHAL
    E2(0xA0, TEXT_SHLL,   IC_N,  ISZ_PRSZ,  M_SCNTR, M_EADST),  // MODE=101 SHLL
    E2(0xC0, TEXT_ROTL,   IC_N,  ISZ_PRSZ,  M_SCNTR, M_EADST),  // MODE=110 ROTL
    E2(0xE0, TEXT_ROTXL,  IC_N,  ISZ_PRSZ,  M_SCNTR, M_EADST),  // MODE=111 ROTXL
};

// SFT static.  Prefix 0x64/0x65/0x66.  Opcode is [MODE(3)|Imm5(5)].
constexpr Entry TABLE_SFT_STAT[] PROGMEM = {
    E2(0x00, TEXT_SHAR,   IC_N,  ISZ_PRSZ,  M_SCNTI, M_EADST),
    E2(0x20, TEXT_SHLR,   IC_N,  ISZ_PRSZ,  M_SCNTI, M_EADST),
    E2(0x40, TEXT_ROTR,   IC_N,  ISZ_PRSZ,  M_SCNTI, M_EADST),
    E2(0x60, TEXT_ROTXR,  IC_N,  ISZ_PRSZ,  M_SCNTI, M_EADST),
    E2(0x80, TEXT_SHAL,   IC_N,  ISZ_PRSZ,  M_SCNTI, M_EADST),
    E2(0xA0, TEXT_SHLL,   IC_N,  ISZ_PRSZ,  M_SCNTI, M_EADST),
    E2(0xC0, TEXT_ROTL,   IC_N,  ISZ_PRSZ,  M_SCNTI, M_EADST),
    E2(0xE0, TEXT_ROTXL,  IC_N,  ISZ_PRSZ,  M_SCNTI, M_EADST),
};

// BIT dynamic.  Prefix 0x68/0x69/0x6A.  Opcode is [*|MODE(2)|*|Rn(4)].
constexpr Entry TABLE_BIT_DYN[] PROGMEM = {
    E2(0x00, TEXT_BSET,   IC_N,  ISZ_PRSZ,  M_BNUMR, M_EADST),  // MODE=00 BSET
    E2(0x20, TEXT_BNOT,   IC_N,  ISZ_PRSZ,  M_BNUMR, M_EADST),  // MODE=01 BNOT
    E2(0x40, TEXT_BCLR,   IC_N,  ISZ_PRSZ,  M_BNUMR, M_EADST),  // MODE=10 BCLR
    E2(0x60, TEXT_BTST,   IC_N,  ISZ_PRSZ,  M_BNUMR, M_EADST),  // MODE=11 BTST
};

// BIT static.  Prefix 0x6C/0x6D/0x6E.  Opcode is [*|MODE(2)|Imm5(5)].
constexpr Entry TABLE_BIT_STAT[] PROGMEM = {
    E2(0x00, TEXT_BSET,   IC_N,  ISZ_PRSZ,  M_BNUMI, M_EADST),
    E2(0x20, TEXT_BNOT,   IC_N,  ISZ_PRSZ,  M_BNUMI, M_EADST),
    E2(0x40, TEXT_BCLR,   IC_N,  ISZ_PRSZ,  M_BNUMI, M_EADST),
    E2(0x60, TEXT_BTST,   IC_N,  ISZ_PRSZ,  M_BNUMI, M_EADST),
};

// MUL.  Prefix 0xEE.  Opcode (second byte) is [MODE(4)|*4]: bit 6 = signed(0)
// or unsigned(1); bit 4 = byte(0) or word(1).
constexpr Entry TABLE_MUL[] PROGMEM = {
    E2(0x00, TEXT_MULXS,  IC_N,  ISZ_OBIT4, M_EASRC, M_EADST),  // MULXS (bit 6 = 0)
    E2(0x40, TEXT_MULXU,  IC_N,  ISZ_OBIT4, M_EASRC, M_EADST),  // MULXU (bit 6 = 1)
};

// DIV.  Prefix 0xEF.  Same MODE layout as MUL.
constexpr Entry TABLE_DIV[] PROGMEM = {
    E2(0x00, TEXT_DIVXS,  IC_N,  ISZ_OBIT4, M_EASRC, M_EADST),
    E2(0x40, TEXT_DIVXU,  IC_N,  ISZ_OBIT4, M_EASRC, M_EADST),
};

// Bcc:G.  Prefix 0xA4/0xA5/0xA6.  Opcode [*4|cc(4)]: low 4 bits select the
// condition.  All class IC_G; the disassembler reconstructs the ":G" suffix.
constexpr Entry TABLE_BCC_G[] PROGMEM = {
    E1(0x00, TEXT_BT,     IC_G,  ISZ_PRSZ,  M_DISP),            // cc=0 T
    E1(0x01, TEXT_BF,     IC_G,  ISZ_PRSZ,  M_DISP),            // cc=1 F
    E1(0x02, TEXT_BHI,    IC_G,  ISZ_PRSZ,  M_DISP),            // cc=2 HI
    E1(0x03, TEXT_BLS,    IC_G,  ISZ_PRSZ,  M_DISP),            // cc=3 LS
    E1(0x04, TEXT_BCC,    IC_G,  ISZ_PRSZ,  M_DISP),            // cc=4 CC/HS
    E1(0x05, TEXT_BCS,    IC_G,  ISZ_PRSZ,  M_DISP),            // cc=5 CS/LO
    E1(0x06, TEXT_BNE,    IC_G,  ISZ_PRSZ,  M_DISP),            // cc=6 NE
    E1(0x07, TEXT_BEQ,    IC_G,  ISZ_PRSZ,  M_DISP),            // cc=7 EQ
    E1(0x08, TEXT_BVC,    IC_G,  ISZ_PRSZ,  M_DISP),            // cc=8 VC
    E1(0x09, TEXT_BVS,    IC_G,  ISZ_PRSZ,  M_DISP),            // cc=9 VS
    E1(0x0A, TEXT_BPL,    IC_G,  ISZ_PRSZ,  M_DISP),            // cc=A PL
    E1(0x0B, TEXT_BMI,    IC_G,  ISZ_PRSZ,  M_DISP),            // cc=B MI
    E1(0x0C, TEXT_BGE,    IC_G,  ISZ_PRSZ,  M_DISP),            // cc=C GE
    E1(0x0D, TEXT_BLT,    IC_G,  ISZ_PRSZ,  M_DISP),            // cc=D LT
    E1(0x0E, TEXT_BGT,    IC_G,  ISZ_PRSZ,  M_DISP),            // cc=E GT
    E1(0x0F, TEXT_BLE,    IC_G,  ISZ_PRSZ,  M_DISP),            // cc=F LE
    // HS/LO are Table 16-8 aliases for CC/CS; the assembler accepts them, the
    // disassembler emits the canonical CC/CS (matched earlier in table order).
    E1(0x04, TEXT_BHS,    IC_G,  ISZ_PRSZ,  M_DISP),            // cc=4 HS == CC
    E1(0x05, TEXT_BLO,    IC_G,  ISZ_PRSZ,  M_DISP),            // cc=5 LO == CS
};

// STRING family.  Prefix 0x94/0x95 = .B/.W (Sz in prefix low bit).  Second
// byte is [flag|TYP(3)|Rnc(4)]: flag (bit 7) = 4-register form (SCMP/SSCH),
// bit 6 = direction (0=F, 1=B).  Opcode here is selector<<4 (Rnc page-masked).
constexpr Entry TABLE_STRING[] PROGMEM = {
    E1(0x00, TEXT_SSTR, IC_N, ISZ_PRSZ, M_STROP),               // SSTR/F (TYP=000)
    E1(0x40, TEXT_SSTR, IC_N, ISZ_PRSZ, M_STROP),               // SSTR/B (TYP=100)
    E1(0x20, TEXT_SMOV, IC_N, ISZ_PRSZ, M_STROP),               // SMOV/F (TYP=010)
    E1(0x50, TEXT_SMOV, IC_N, ISZ_PRSZ, M_STROP),               // SMOV/B (TYP=101)
    E1(0x80, TEXT_SSCH, IC_N, ISZ_PRSZ, M_STROP),               // SSCH/F (flag=1, TYP=000)
    E1(0xC0, TEXT_SSCH, IC_N, ISZ_PRSZ, M_STROP),               // SSCH/B (flag=1, TYP=100)
    E1(0xA0, TEXT_SCMP, IC_N, ISZ_PRSZ, M_STROP),               // SCMP/F (flag=1, TYP=010)
    E1(0xD0, TEXT_SCMP, IC_N, ISZ_PRSZ, M_STROP),               // SCMP/B (flag=1, TYP=101)
};

// Sorted name -> entry-index arrays, used by searchName's binary search.
// Generated literally by scripts/make-index-array.sh.
constexpr uint8_t INDEX_MAIN[] PROGMEM = {
      0,  // TEXT_ADD
      1,  // TEXT_ADD
      2,  // TEXT_ADD
      3,  // TEXT_ADD
     16,  // TEXT_ADDS
     20,  // TEXT_ADDX
     28,  // TEXT_AND
     76,  // TEXT_ANDC
     38,  // TEXT_BEQ
     53,  // TEXT_BFEXT
     54,  // TEXT_BFEXT
     55,  // TEXT_BFEXT
     56,  // TEXT_BFINS
     57,  // TEXT_BFINS
     58,  // TEXT_BFINS
     62,  // TEXT_BFMOV
     59,  // TEXT_BFSCH
     60,  // TEXT_BFSCH
     61,  // TEXT_BFSCH
     43,  // TEXT_BNE
     35,  // TEXT_BRA
     39,  // TEXT_BSR
     65,  // TEXT_CGBN
     15,  // TEXT_CLR
      7,  // TEXT_CMP
      8,  // TEXT_CMP
      9,  // TEXT_CMP
     10,  // TEXT_CMP
     18,  // TEXT_CMPS
     29,  // TEXT_DADD
     82,  // TEXT_DCBN
     42,  // TEXT_DNEG
     31,  // TEXT_DSUB
     49,  // TEXT_EXTS
     41,  // TEXT_EXTU
     81,  // TEXT_ICBN
     36,  // TEXT_JMP
     40,  // TEXT_JSR
     79,  // TEXT_LDC
     25,  // TEXT_LDM
     51,  // TEXT_LINK
     11,  // TEXT_MOV
     12,  // TEXT_MOV
     13,  // TEXT_MOV
     14,  // TEXT_MOV
     50,  // TEXT_MOVA
     23,  // TEXT_MOVF
     64,  // TEXT_MOVFP
     27,  // TEXT_MOVFPE
     19,  // TEXT_MOVS
     63,  // TEXT_MOVTP
     26,  // TEXT_MOVTPE
     33,  // TEXT_NEG
     37,  // TEXT_NEGX
     83,  // TEXT_NOP
     34,  // TEXT_NOT
     32,  // TEXT_OR
     77,  // TEXT_ORC
     66,  // TEXT_PGBN
     67,  // TEXT_PGBN
     70,  // TEXT_RESET
     47,  // TEXT_RTD
     71,  // TEXT_RTE
     74,  // TEXT_RTR
     48,  // TEXT_RTS
     45,  // TEXT_SCB
     46,  // TEXT_SET
     75,  // TEXT_SLEEP
     80,  // TEXT_STC
     24,  // TEXT_STM
      4,  // TEXT_SUB
      5,  // TEXT_SUB
      6,  // TEXT_SUB
     17,  // TEXT_SUBS
     21,  // TEXT_SUBX
     68,  // TEXT_SWAP
     69,  // TEXT_TAS
     73,  // TEXT_TRAP
     72,  // TEXT_TRAPA
     22,  // TEXT_TST
     52,  // TEXT_UNLK
     44,  // TEXT_XCH
     30,  // TEXT_XOR
     78,  // TEXT_XORC
};
constexpr uint8_t INDEX_SFT[] PROGMEM = {
      6,  // TEXT_ROTL
      2,  // TEXT_ROTR
      7,  // TEXT_ROTXL
      3,  // TEXT_ROTXR
      4,  // TEXT_SHAL
      0,  // TEXT_SHAR
      5,  // TEXT_SHLL
      1,  // TEXT_SHLR
};
constexpr uint8_t INDEX_BIT[] PROGMEM = {
      2,  // TEXT_BCLR
      1,  // TEXT_BNOT
      0,  // TEXT_BSET
      3,  // TEXT_BTST
};
constexpr uint8_t INDEX_MUL_DIV[] PROGMEM = {
      0,  // TEXT_MULXS
      1,  // TEXT_MULXU
};
constexpr uint8_t INDEX_BCC_G[] PROGMEM = {
      4,  // TEXT_BCC
      5,  // TEXT_BCS
      7,  // TEXT_BEQ
      1,  // TEXT_BF
     12,  // TEXT_BGE
     14,  // TEXT_BGT
      2,  // TEXT_BHI
     16,  // TEXT_BHS
     15,  // TEXT_BLE
     17,  // TEXT_BLO
      3,  // TEXT_BLS
     13,  // TEXT_BLT
     11,  // TEXT_BMI
      6,  // TEXT_BNE
     10,  // TEXT_BPL
      0,  // TEXT_BT
      8,  // TEXT_BVC
      9,  // TEXT_BVS
};
constexpr uint8_t INDEX_STRING[] PROGMEM = {
    6,  // TEXT_SCMP
    7,  // TEXT_SCMP
    2,  // TEXT_SMOV
    3,  // TEXT_SMOV
    4,  // TEXT_SSCH
    5,  // TEXT_SSCH
    0,  // TEXT_SSTR
    1,  // TEXT_SSTR
};
#undef E0
#undef E1
#undef E2

// Each prefix page carries three matching masks:
//   prefixMask - one page matches all consecutive prefixes differing only in
//                Sz (e.g. 0x60-0x62 for SFT dyn .B/.W/.L)
//   opMask     - which bits of the second-byte opcode are operand data vs
//                MODE selector (ignored when matching)
//   zeroMask   - bits inside opMask that the manual marks "don't care" but
//                that the disassembler requires to be zero (else
//                ILLEGAL_OPERAND_MODE)
// Only a few distinct (prefixMask, opMask, zeroMask) triples occur, so each is
// named a PageFormat and EntryPage stores a single format byte, looking the
// three masks up from PAGE_MASKS (the CodeFormat idiom, cf. entry_i8096.h).
enum PageFormat : uint8_t {
    PF_MAIN = 0,   // 0x00,0x00,0x00  no prefix; entry-level Sz mask applies
    PF_SFT_DYN,    // 0x03,0x1F,0x10  SFT dynamic: ignore Rn+bit4; bit4 must be 0
    PF_SFT_STAT,   // 0x03,0x1F,0x00  SFT static: ignore MODE+Imm5
    PF_BIT_DYN,    // 0x03,0x9F,0x90  BIT dynamic: bits 7,4 must be 0
    PF_BIT_STAT,   // 0x03,0x9F,0x80  BIT static: bit 7 must be 0
    PF_MULDIV,     // 0x00,0xBF,0xAF  MUL/DIV: ignore size+spares; spares must be 0
    PF_BCC_G,      // 0x03,0xF0,0xF0  Bcc:G: ignore high nibble; must be 0
    PF_STRING,     // 0x01,0x0F,0x00  STRING: ignore Rnc
};

struct PageMask {
    uint8_t prefixMask;
    uint8_t opMask;
    uint8_t zeroMask;
};

constexpr PageMask PAGE_MASKS[] PROGMEM = {
    {0x00, 0x00, 0x00},  // PF_MAIN
    {0x03, 0x1F, 0x10},  // PF_SFT_DYN
    {0x03, 0x1F, 0x00},  // PF_SFT_STAT
    {0x03, 0x9F, 0x90},  // PF_BIT_DYN
    {0x03, 0x9F, 0x80},  // PF_BIT_STAT
    {0x00, 0xBF, 0xAF},  // PF_MULDIV
    {0x03, 0xF0, 0xF0},  // PF_BCC_G
    {0x01, 0x0F, 0x00},  // PF_STRING
};

struct EntryPage : entry::PrefixTableBase<Entry> {
    constexpr EntryPage(uint16_t prefix, PageFormat format, const Entry *head_P,
            const Entry *tail_P, const uint8_t *index_P, const uint8_t *itail_P)
        : PrefixTableBase(prefix, head_P, tail_P, index_P, itail_P),
          _format(format) {}

    uint8_t prefixMask() const { return pgm_read_byte(&PAGE_MASKS[fmt()].prefixMask); }
    uint8_t opMask() const { return pgm_read_byte(&PAGE_MASKS[fmt()].opMask); }
    uint8_t zeroMask() const { return pgm_read_byte(&PAGE_MASKS[fmt()].zeroMask); }

private:
    uint8_t fmt() const { return pgm_read_byte(&_format); }
    const uint8_t _format;
};

constexpr EntryPage H16_PAGES[] PROGMEM = {
    // main page: no prefix; entry-level mask applies; no zero-required bits
    {0x0000, PF_MAIN,     ARRAY_RANGE(TABLE_MAIN),     ARRAY_RANGE(INDEX_MAIN)},
    // SFT dynamic 0x60-0x62; opcode [MODE(3)|*|Rn(4)]
    {0x0060, PF_SFT_DYN,  ARRAY_RANGE(TABLE_SFT_DYN),  ARRAY_RANGE(INDEX_SFT)},
    // SFT static 0x64-0x66; opcode [MODE(3)|Imm5(5)]
    {0x0064, PF_SFT_STAT, ARRAY_RANGE(TABLE_SFT_STAT), ARRAY_RANGE(INDEX_SFT)},
    // BIT dynamic 0x68-0x6A; opcode [*|MODE(2)|*|Rn(4)]
    {0x0068, PF_BIT_DYN,  ARRAY_RANGE(TABLE_BIT_DYN),  ARRAY_RANGE(INDEX_BIT)},
    // BIT static 0x6C-0x6E; opcode [*|MODE(2)|Imm5(5)]
    {0x006C, PF_BIT_STAT, ARRAY_RANGE(TABLE_BIT_STAT), ARRAY_RANGE(INDEX_BIT)},
    // MUL 0xEE; opcode [MODE(4)|****]
    {0x00EE, PF_MULDIV,   ARRAY_RANGE(TABLE_MUL),      ARRAY_RANGE(INDEX_MUL_DIV)},
    // DIV 0xEF; same layout
    {0x00EF, PF_MULDIV,   ARRAY_RANGE(TABLE_DIV),      ARRAY_RANGE(INDEX_MUL_DIV)},
    // Bcc:G 0xA4-0xA6; opcode [****|cc(4)]
    {0x00A4, PF_BCC_G,    ARRAY_RANGE(TABLE_BCC_G),    ARRAY_RANGE(INDEX_BCC_G)},
    // STRING 0x94-0x95; opcode [flag|TYP|Rnc]
    {0x0094, PF_STRING,   ARRAY_RANGE(TABLE_STRING),   ARRAY_RANGE(INDEX_STRING)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
    {H16, TEXT_CPU_H16, ARRAY_RANGE(H16_PAGES)},
};

static const Cpu *cpu(CpuType cpuType) {
    return Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
}

// prefixMatcher: |code| is a prefix when it falls in a page's prefix range
// (page prefix with its Sz/variant bits, given by prefixMask, masked out).
static bool prefixMatcher(uint16_t code, const EntryPage *page) {
    const auto prefix = page->readPrefix();
    return prefix && (code & ~page->prefixMask()) == prefix;
}

bool isPrefix(CpuType cpuType, Config::opcode_t code) {
    return cpu(cpuType)->isPrefix(code, prefixMatcher);
}

// pageMatcher: a page matches when (insn.prefix() & ~prefixMask) == page.prefix
// and (for sized prefixes) the masked-out bits don't encode Sz=11 (reserved).
static bool pageMatcher(DisInsn &insn, const EntryPage *page) {
    const auto pmask = page->prefixMask();
    if ((insn.prefix() & ~pmask) != page->readPrefix())
        return false;
    if (pmask == 0x03 && (insn.prefix() & 0x03) == 0x03)
        return false;  // reserved Sz=11
    return true;
}

// matchOpCode: the prefix pages supply a non-zero opMask that names the
// operand-data bits to ignore when matching.  After a match, page-level
// zeroMask bits must be zero or we flag ILLEGAL_OPERAND_MODE.  The main page
// has opMask 0 and falls back to the entry's own Sz mask.
static bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *page) {
    const auto pmask = page->opMask();
    if (pmask != 0) {
        if ((insn.opCode() & ~pmask) != entry->readOpCode())
            return false;
        if (insn.opCode() & page->zeroMask())
            insn.setErrorIf(ILLEGAL_OPERAND_MODE);
        return true;
    }
    const auto emask = entry->readFlags().opCodeMask();
    // Only a 2-bit Sz field has a reserved combination (Sz=11); a 1-bit
    // byte/word field (emask 0x01) uses both values.
    if (emask == 0x03 && (insn.opCode() & 0x03) == 0x03)
        return false;
    return (insn.opCode() & ~emask) == entry->readOpCode();
}

Error searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) {
    cpu(cpuType)->searchOpCode(insn, out, matchOpCode, pageMatcher);
    return insn.getError();
}

// True if op's parsed immediate is defined and within [lo, hi].  An undefined
// value (forward reference) is treated as not fitting, so optimal selection
// falls back to the general form rather than risk a too-small encoding.
static bool immFits(const Operand &op, int32_t lo, int32_t hi) {
    if (op.val.isUndefined())
        return false;
    const auto v = op.val.getSigned();
    return v >= lo && v <= hi;
}


// Lead-with-equality acceptMode helper; the parser sets the exact AddrMode the
// table expects so the switch only carries fit-relaxations.
static bool acceptMode(const Operand &op, AddrMode table) {
    if (op.mode == table)
        return true;
    switch (table) {
    case M_EASRC:
        return isEaMode(op.mode);
    case M_EADST:
        // A write destination can be any EA except immediate (you can't store
        // into a literal).  The disassembler enforces the same via destOnly,
        // so allowing it here would assemble bytes the disassembler rejects.
        return isEaMode(op.mode) && op.mode != M_IMM;
    case M_EAMEM:
        // JMP/JSR target: a memory EA (manual Available EA forbids Rn and #imm).
        return isMemEaMode(op.mode);
    case M_EAPERI:
        // MOVTP/MOVFP peripheral operand: a memory EA without auto-inc/dec.
        return isPeriEaMode(op.mode);
    case M_RNNO:
        // CGBN/UNLK Rn: parser produces register-direct M_DREG.
        return op.mode == M_DREG;
    case M_IQ:
        // ADD:Q / CMP:Q / MOV:Q: imm8 signed.  The range gate both rejects an
        // out-of-range explicit :Q and lets class-less selection skip :Q in
        // favor of :G when the value needs more than 8 bits.
        return op.mode == M_IMM && immFits(op, -0x80, 0x7F);
    case M_TRAPV:
    case M_FRMSZ:
    case M_SCNTI:
    case M_BNUMI:
        // TRAPA / RTD / SFT-stat / BIT-stat: parser produces M_IMM.
        return op.mode == M_IMM;
    case M_SCNTR:
    case M_BNUMR:
    case M_RCC:
        // SFT-dyn / BIT-dyn dynamic count, and SCB's Rn: a global register
        // (parser sees M_DREG).
        return op.mode == M_DREG && isGlobalReg(op.reg);
    case M_DISP:
        // Branch target written as a bare expression.
        return op.mode == M_IMM;
    case M_CR:
        // ANDC/ORC/XORC/LDC/STC: M_CR operand parsed as itself (M_CR).
        return op.mode == M_CR;
    default:
        return false;
    }
}

static bool acceptOperands(AsmInsn &insn, const Entry *entry) {
    const auto flags = entry->readFlags();
    // An explicit ":X" class must match the entry's class.  A class-less
    // mnemonic (IC_N) matches any class; the first acceptable entry in
    // optimal-preference (INDEX) order then wins -> smallest fitting form.
    if (insn.classSuffix != IC_N && insn.classSuffix != flags.insnClass())
        return false;
    // The list/tuple families below validate insn.regList (folded into a single
    // M_RLIST slot by the parser) directly.

    // String ops (SMOV/SSTR/SCMP/SSCH): 3 or 4 global registers plus a required
    // /F|/B direction matching the entry's selector (opcode bit 6).
    if (flags.mode1() == M_STROP) {
        const auto opc = entry->readOpCode();
        if (insn.dirSuffix == DIR_NONE)
            return false;
        if ((insn.dirSuffix == DIR_B) != ((opc & 0x40) != 0))
            return false;
        const uint8_t n = (opc & 0x80) ? 4 : 3;  // SCMP/SSCH=4, SMOV/SSTR=3
        if (n == 4 && insn.ccSuffix == reg::CC_UNDEF)
            return false;  // SCMP/SSCH require a /cc (SMOV/SSTR's spurious cc is
                           // rejected as UNKNOWN_INSTRUCTION after searchName)
        return insn.regCount == n && insn.regGlobal && !insn.regRange &&
               insn.op2.mode == M_NONE && insn.op3.mode == M_NONE;
    }
    // BFMOV: six global registers, no EA operand.
    if (flags.mode1() == M_BFMOV)
        return insn.regCount == 6 && insn.regGlobal && !insn.regRange &&
               insn.op2.mode == M_NONE && insn.op3.mode == M_NONE;
    // Bit-field ops BFEXT/BFINS/BFSCH: Rnb, Rnf (regList[0..1]) then EAs, EAd.
    // Each entry form names its memory EAs via mode2/mode3; the rest are
    // register-direct and folded into the run, so regCount = 2 + (register-direct
    // EAs) = 4 - (memory EAs).
    if (flags.mode1() == M_BITF) {
        if (!insn.regGlobal || insn.regRange)
            return false;
        const bool mem2 = flags.mode2() != M_NONE;
        const bool mem3 = flags.mode3() != M_NONE;
        if ((insn.op2.mode != M_NONE) != mem2 || (insn.op3.mode != M_NONE) != mem3)
            return false;
        if (insn.regCount != uint8_t(4 - mem2 - mem3))
            return false;
        if (mem2 && !isBitFieldEaMode(insn.op2.mode))
            return false;
        if (mem3 && !isBitFieldEaMode(insn.op3.mode))
            return false;
        return true;
    }
    // STM/LDM/PGBN: a global-register list, plus a positional memory EA -- STM:
    // list..., EAd (the last operand); LDM: EAs (op1), list...; PGBN: list only.
    if (flags.mode1() == M_RLIST || flags.mode2() == M_RLIST) {
        if (insn.regCount < 1 || !insn.regGlobal)
            return false;
        if (flags.mode2() == M_EADST) {  // STM
            const auto &ea = insn.op3.mode != M_NONE ? insn.op3 : insn.op2;
            return isMemEaMode(ea.mode);
        }
        if (flags.mode1() == M_EASRC)    // LDM
            return isMemEaMode(insn.op1.mode);
        // PGBN: no EA, so no op slot may hold a non-list operand.
        return insn.op3.mode == M_NONE &&
               (insn.op2.mode == M_NONE || insn.op2.mode == M_DREG) &&
               (insn.op1.mode == M_RLIST || insn.op1.mode == M_DREG);
    }
    // M_RR / M_IRQ / M_SIRQ combine two user operands into a single byte; the
    // table marks them as src-only with M_NONE dst, but the parsed operand
    // pair must both be register-direct (M_RR) or imm4+register.  The :RQ
    // immediate is unsigned 0..15 for ADD/SUB (zero-extended, M_IRQ) but
    // signed -8..7 for CMP/MOV (sign-extended, M_SIRQ) per the manual.
    if (flags.mode2() == M_NONE) {
        if (flags.mode1() == M_RR)
            return insn.op1.mode == M_DREG && isGlobalReg(insn.op1.reg) &&
                   insn.op2.mode == M_DREG && isGlobalReg(insn.op2.reg);
        if (flags.mode1() == M_IRQ || flags.mode1() == M_SIRQ) {
            const auto lo = flags.mode1() == M_SIRQ ? -8 : 0;
            const auto hi = flags.mode1() == M_SIRQ ? 7 : 15;
            return insn.op1.mode == M_IMM && immFits(insn.op1, lo, hi) &&
                   insn.op2.mode == M_DREG && isGlobalReg(insn.op2.reg);
        }
    }
    return acceptMode(insn.op1, flags.mode1()) && acceptMode(insn.op2, flags.mode2());
}

Error searchName(CpuType cpuType, AsmInsn &insn) {
    cpu(cpuType)->searchName(insn, acceptOperands);
    return insn.getError();
}

const /*PROGMEM*/ char *TableH16::listCpu_P() const {
    return TEXT_H16_LIST;
}

const /*PROGMEM*/ char *TableH16::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableH16::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    if (name.iequals_P(TEXT_CPU_H16)) {
        cpuType = H16;
        return OK;
    }
    return UNSUPPORTED_CPU;
}

const TableH16 TABLE;

}  // namespace h16
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

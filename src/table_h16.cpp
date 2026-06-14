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

#define E2(opc, isz, name, src, dst) \
    {opc, Entry::Flags::create(isz, src, dst), name}
#define E1(opc, isz, name, src) E2(opc, isz, name, src, M_NONE)
#define E0(opc, name) E2(opc, ISZ_NONE, name, M_NONE, M_NONE)

// ============================================================================
// Main page: single-byte opcodes.  Sized entries (ISZ_DATA/ISZ_EXTU) have Sz
// in the low 2 bits of the opcode; matchOpCode masks those bits.
// ============================================================================
constexpr Entry TABLE_MAIN[] PROGMEM = {
    E2(0x00, ISZ_DATA, TEXT_ADD_G,  M_EA,   M_EAD),  // ADD:G  .B/.W/.L
    E2(0x04, ISZ_DATA, TEXT_SUB_G,  M_EA,   M_EAD),  // SUB:G
    E2(0x08, ISZ_DATA, TEXT_CMP_G,  M_EA,   M_EA),   // CMP:G  (no writeback)
    E2(0x0C, ISZ_DATA, TEXT_MOV_G,  M_EA,   M_EAD),  // MOV:G
    E2(0x10, ISZ_DATA, TEXT_ADD_Q,  M_IQ,   M_EAD),  // ADD:Q  imm8s, EAd
    E1(0x14, ISZ_DATA, TEXT_CLR,    M_EAD),          // CLR
    E2(0x18, ISZ_DATA, TEXT_CMP_Q,  M_IQ,   M_EA),   // CMP:Q
    E2(0x1C, ISZ_DATA, TEXT_MOV_Q,  M_IQ,   M_EAD),  // MOV:Q
    E1(0x20, ISZ_DATA, TEXT_ADD_R,  M_RR),           // ADD:R  Rns,Rnd
    E1(0x24, ISZ_DATA, TEXT_SUB_R,  M_RR),           // SUB:R
    E1(0x28, ISZ_DATA, TEXT_CMP_R,  M_RR),           // CMP:R
    E1(0x2C, ISZ_DATA, TEXT_MOV_R,  M_RR),           // MOV:R
    E1(0x30, ISZ_DATA, TEXT_ADD_RQ, M_IRQ),          // ADD:RQ imm4u,Rnd
    E1(0x34, ISZ_DATA, TEXT_SUB_RQ, M_IRQ),          // SUB:RQ
    E1(0x38, ISZ_DATA, TEXT_CMP_RQ, M_IRQ),          // CMP:RQ
    E1(0x3C, ISZ_DATA, TEXT_MOV_RQ, M_IRQ),          // MOV:RQ
    E2(0x40, ISZ_DATA, TEXT_ADDS,   M_EA,   M_EAD),  // ADDS   sign-extend
    E2(0x44, ISZ_DATA, TEXT_SUBS,   M_EA,   M_EAD),  // SUBS
    E2(0x48, ISZ_DATA, TEXT_CMPS,   M_EA,   M_EA),   // CMPS
    E2(0x4C, ISZ_DATA, TEXT_MOVS,   M_EA,   M_EAD),  // MOVS
    E2(0x50, ISZ_DATA, TEXT_ADDX,   M_EA,   M_EAD),  // ADDX   add with CX
    E2(0x54, ISZ_DATA, TEXT_SUBX,   M_EA,   M_EAD),  // SUBX
    E1(0x58, ISZ_DATA, TEXT_TST,    M_EA),           // TST    read-only
    E1(0x5C, ISZ_DATA, TEXT_MOVF,   M_EAD),          // MOVF   from R0
    E2(0x78, ISZ_WORD, TEXT_MOVTPE, M_EA,   M_EAD),  // MOVTPE peripheral+E
    E2(0x7C, ISZ_WORD, TEXT_MOVFPE, M_EA,   M_EAD),  // MOVFPE peripheral+E
    E2(0x80, ISZ_DATA, TEXT_AND,    M_EA,   M_EAD),  // AND
    E2(0x83, ISZ_NONE, TEXT_DADD,   M_EA,   M_EAD),  // DADD   decimal add
    E2(0x84, ISZ_DATA, TEXT_XOR,    M_EA,   M_EAD),  // XOR
    E2(0x87, ISZ_NONE, TEXT_DSUB,   M_EA,   M_EAD),  // DSUB   decimal sub
    E2(0x88, ISZ_DATA, TEXT_OR,     M_EA,   M_EAD),  // OR
    E1(0x8C, ISZ_DATA, TEXT_NEG,    M_EAD),          // NEG
    E1(0x90, ISZ_DATA, TEXT_NOT,    M_EAD),          // NOT
    E1(0x98, ISZ_DATA, TEXT_BRA,    M_DISP),         // BRA    disp .B/.W/.L
    E1(0x9B, ISZ_NONE, TEXT_JMP,    M_EA),           // JMP    via EA
    E1(0x9C, ISZ_DATA, TEXT_NEGX,   M_EAD),          // NEGX   negate with CX
    E1(0xA0, ISZ_DATA, TEXT_BEQ,    M_DISP),         // BEQ
    E1(0xA8, ISZ_DATA, TEXT_BSR,    M_DISP),         // BSR
    E1(0xAB, ISZ_NONE, TEXT_JSR,    M_EA),           // JSR
    E1(0xAC, ISZ_EXTU, TEXT_EXTU,   M_RNNO),         // EXTU   Sz: 0=W,1=L,2=B
    E1(0xAF, ISZ_FIXB, TEXT_DNEG,   M_EAD),          // DNEG.B decimal neg
    E1(0xB0, ISZ_DATA, TEXT_BNE,    M_DISP),         // BNE
    E1(0xB3, ISZ_NONE, TEXT_XCH,    M_RR),           // XCH    exchange regs
    E1(0xB8, ISZ_DATA, TEXT_RTD,    M_FRMSZ),        // RTD    deallocate
    E0(0xBB,           TEXT_RTS),                    // RTS
    E1(0xBC, ISZ_EXTU, TEXT_EXTS,   M_RNNO),         // EXTS   Sz: 0=W,1=L,2=B
    E2(0xBF, ISZ_NONE, TEXT_MOVA,   M_EA,   M_EAD),  // MOVA   move EA to R0
    E1(0xD0, ISZ_DATA, TEXT_LINK,   M_RFRM),         // LINK   Rn,#frame
    E1(0xD3, ISZ_NONE, TEXT_UNLK,   M_RNNO),         // UNLK
    E2(0xE0, ISZ_WORD, TEXT_MOVTP,  M_EA,   M_EAD),  // MOVTP  peripheral
    E2(0xE2, ISZ_WORD, TEXT_MOVFP,  M_EA,   M_EAD),  // MOVFP  peripheral
    E1(0xE4, ISZ_NONE, TEXT_CGBN,   M_RNNO),         // CGBN
    E1(0xEB, ISZ_WORD, TEXT_SWAP,   M_EAD),          // SWAP via EA
    E0(0xF0,           TEXT_RESET),                  // RESET
    E0(0xF1,           TEXT_RTE),                    // RTE
    E1(0xF2, ISZ_NONE, TEXT_TRAPA,  M_ITRPN),        // TRAPA  #imm4
    E0(0xF4,           TEXT_RTR),                    // RTR
    E0(0xF5,           TEXT_SLEEP),                  // SLEEP
    E2(0xF8, ISZ_NONE, TEXT_ANDC,   M_EA,   M_CR),   // ANDC   EA,CR
    E2(0xF9, ISZ_NONE, TEXT_ORC,    M_EA,   M_CR),   // ORC
    E2(0xFA, ISZ_NONE, TEXT_XORC,   M_EA,   M_CR),   // XORC
    E2(0xFB, ISZ_NONE, TEXT_LDC,    M_EA,   M_CR),   // LDC
    E2(0xFC, ISZ_NONE, TEXT_STC,    M_CR,   M_EAD),  // STC
    E0(0xFD,           TEXT_ICBN),                   // ICBN
    E0(0xFE,           TEXT_DCBN),                   // DCBN
    E0(0xFF,           TEXT_NOP),                    // NOP
};

// ============================================================================
// SFT (shift/rotate) dynamic.  Prefix 0x60/0x61/0x62 = .B/.W/.L (Sz in low 2
// bits of prefix).  Opcode (second byte) is [MODE(3)|*|Rn(4)]: MODE bits 7-5
// select mnemonic; bits 4-0 carry don't-care + Rn.
// ============================================================================
constexpr Entry TABLE_SFT_DYN[] PROGMEM = {
    E2(0x00, ISZ_PRSZ, TEXT_SHAR,  M_SCNTR, M_EAD),  // MODE=000 SHAR
    E2(0x20, ISZ_PRSZ, TEXT_SHLR,  M_SCNTR, M_EAD),  // MODE=001 SHLR
    E2(0x40, ISZ_PRSZ, TEXT_ROTR,  M_SCNTR, M_EAD),  // MODE=010 ROTR
    E2(0x60, ISZ_PRSZ, TEXT_ROTXR, M_SCNTR, M_EAD),  // MODE=011 ROTXR
    E2(0x80, ISZ_PRSZ, TEXT_SHAL,  M_SCNTR, M_EAD),  // MODE=100 SHAL
    E2(0xA0, ISZ_PRSZ, TEXT_SHLL,  M_SCNTR, M_EAD),  // MODE=101 SHLL
    E2(0xC0, ISZ_PRSZ, TEXT_ROTL,  M_SCNTR, M_EAD),  // MODE=110 ROTL
    E2(0xE0, ISZ_PRSZ, TEXT_ROTXL, M_SCNTR, M_EAD),  // MODE=111 ROTXL
};

// SFT static.  Prefix 0x64/0x65/0x66.  Opcode is [MODE(3)|Imm5(5)].
constexpr Entry TABLE_SFT_STAT[] PROGMEM = {
    E2(0x00, ISZ_PRSZ, TEXT_SHAR,  M_SCNTI, M_EAD),
    E2(0x20, ISZ_PRSZ, TEXT_SHLR,  M_SCNTI, M_EAD),
    E2(0x40, ISZ_PRSZ, TEXT_ROTR,  M_SCNTI, M_EAD),
    E2(0x60, ISZ_PRSZ, TEXT_ROTXR, M_SCNTI, M_EAD),
    E2(0x80, ISZ_PRSZ, TEXT_SHAL,  M_SCNTI, M_EAD),
    E2(0xA0, ISZ_PRSZ, TEXT_SHLL,  M_SCNTI, M_EAD),
    E2(0xC0, ISZ_PRSZ, TEXT_ROTL,  M_SCNTI, M_EAD),
    E2(0xE0, ISZ_PRSZ, TEXT_ROTXL, M_SCNTI, M_EAD),
};

// BIT dynamic.  Prefix 0x68/0x69/0x6A.  Opcode is [*|MODE(2)|*|Rn(4)].
constexpr Entry TABLE_BIT_DYN[] PROGMEM = {
    E2(0x00, ISZ_PRSZ, TEXT_BSET,  M_BNUMR, M_EAD),  // MODE=00 BSET
    E2(0x20, ISZ_PRSZ, TEXT_BNOT,  M_BNUMR, M_EAD),  // MODE=01 BNOT
    E2(0x40, ISZ_PRSZ, TEXT_BCLR,  M_BNUMR, M_EAD),  // MODE=10 BCLR
    E2(0x60, ISZ_PRSZ, TEXT_BTST,  M_BNUMR, M_EAD),  // MODE=11 BTST
};

// BIT static.  Prefix 0x6C/0x6D/0x6E.  Opcode is [*|MODE(2)|Imm5(5)].
constexpr Entry TABLE_BIT_STAT[] PROGMEM = {
    E2(0x00, ISZ_PRSZ, TEXT_BSET,  M_BNUMI, M_EAD),
    E2(0x20, ISZ_PRSZ, TEXT_BNOT,  M_BNUMI, M_EAD),
    E2(0x40, ISZ_PRSZ, TEXT_BCLR,  M_BNUMI, M_EAD),
    E2(0x60, ISZ_PRSZ, TEXT_BTST,  M_BNUMI, M_EAD),
};

// MUL.  Prefix 0xEE.  Opcode (second byte) is [MODE(4)|*4]: bit 6 = signed(0)
// or unsigned(1); bit 4 = byte(0) or word(1).
constexpr Entry TABLE_MUL[] PROGMEM = {
    E2(0x00, ISZ_OBIT4, TEXT_MULXS, M_EA, M_EAD),    // MULXS (bit 6 = 0)
    E2(0x40, ISZ_OBIT4, TEXT_MULXU, M_EA, M_EAD),    // MULXU (bit 6 = 1)
};

// DIV.  Prefix 0xEF.  Same MODE layout as MUL.
constexpr Entry TABLE_DIV[] PROGMEM = {
    E2(0x00, ISZ_OBIT4, TEXT_DIVXS, M_EA, M_EAD),
    E2(0x40, ISZ_OBIT4, TEXT_DIVXU, M_EA, M_EAD),
};

// Bcc:G.  Prefix 0xA4/0xA5/0xA6.  Opcode (second byte) is [*4|cc(4)]: low 4
// bits select the condition.
constexpr Entry TABLE_BCC_G[] PROGMEM = {
    E1(0x00, ISZ_PRSZ, TEXT_BT_G,  M_DISP),          // cc=0 T
    E1(0x01, ISZ_PRSZ, TEXT_BF_G,  M_DISP),          // cc=1 F
    E1(0x02, ISZ_PRSZ, TEXT_BHI_G, M_DISP),          // cc=2 HI
    E1(0x03, ISZ_PRSZ, TEXT_BLS_G, M_DISP),          // cc=3 LS
    E1(0x04, ISZ_PRSZ, TEXT_BCC_G, M_DISP),          // cc=4 CC/HS
    E1(0x05, ISZ_PRSZ, TEXT_BCS_G, M_DISP),          // cc=5 CS/LO
    E1(0x06, ISZ_PRSZ, TEXT_BNE_G, M_DISP),          // cc=6 NE
    E1(0x07, ISZ_PRSZ, TEXT_BEQ_G, M_DISP),          // cc=7 EQ
    E1(0x08, ISZ_PRSZ, TEXT_BVC_G, M_DISP),          // cc=8 VC
    E1(0x09, ISZ_PRSZ, TEXT_BVS_G, M_DISP),          // cc=9 VS
    E1(0x0A, ISZ_PRSZ, TEXT_BPL_G, M_DISP),          // cc=A PL
    E1(0x0B, ISZ_PRSZ, TEXT_BMI_G, M_DISP),          // cc=B MI
    E1(0x0C, ISZ_PRSZ, TEXT_BGE_G, M_DISP),          // cc=C GE
    E1(0x0D, ISZ_PRSZ, TEXT_BLT_G, M_DISP),          // cc=D LT
    E1(0x0E, ISZ_PRSZ, TEXT_BGT_G, M_DISP),          // cc=E GT
    E1(0x0F, ISZ_PRSZ, TEXT_BLE_G, M_DISP),          // cc=F LE
};

// Sorted INDEX arrays are regenerated via scripts/make-index-array.sh when the
// assembler lands.  Disassembly uses linear scan and so does not need them.
constexpr uint8_t INDEX_EMPTY[] PROGMEM = {};

#undef E0
#undef E1
#undef E2

// EntryPage extends PrefixTableBase with:
//   prefixMask - one page matches all consecutive prefixes differing only in
//                Sz (e.g. 0x60-0x62 for SFT dyn .B/.W/.L)
//   opMask     - which bits of the second-byte opcode are operand data vs
//                MODE selector (ignored when matching)
//   zeroMask   - bits inside opMask that the manual marks "don't care" but
//                that the disassembler requires to be zero (else
//                ILLEGAL_OPERAND_MODE)
struct EntryPage : entry::PrefixTableBase<Entry> {
    constexpr EntryPage(uint16_t prefix, uint8_t prefixMask, uint8_t opMask,
            uint8_t zeroMask, const Entry *head_P, const Entry *tail_P,
            const uint8_t *index_P, const uint8_t *itail_P)
        : PrefixTableBase(prefix, head_P, tail_P, index_P, itail_P),
          _prefixMask(prefixMask),
          _opMask(opMask),
          _zeroMask(zeroMask) {}

    uint8_t prefixMask() const { return pgm_read_byte(&_prefixMask); }
    uint8_t opMask() const { return pgm_read_byte(&_opMask); }
    uint8_t zeroMask() const { return pgm_read_byte(&_zeroMask); }

private:
    const uint8_t _prefixMask;
    const uint8_t _opMask;
    const uint8_t _zeroMask;
};

// opMask ignores operand-data bits when matching the entry's opcode;
// zeroMask names the "*" bits that the manual marks don't-care but that the
// disassembler treats as zero-required (reports ILLEGAL_OPERAND_MODE if
// they're non-zero).
constexpr EntryPage H16_PAGES[] PROGMEM = {
    // main page: no prefix; entry-level mask applies; no zero-required bits
    {0x0000, 0x00, 0x00, 0x00, ARRAY_RANGE(TABLE_MAIN),     ARRAY_RANGE(INDEX_EMPTY)},
    // SFT dynamic 0x60-0x62; opcode [MODE(3)|*|Rn(4)]:
    //   opMask 0x1F = ignore Rn + bit 4 for matching
    //   zeroMask 0x10 = bit 4 ("*") must be 0
    {0x0060, 0x03, 0x1F, 0x10, ARRAY_RANGE(TABLE_SFT_DYN),  ARRAY_RANGE(INDEX_EMPTY)},
    // SFT static 0x64-0x66; opcode [MODE(3)|Imm5(5)]:
    {0x0064, 0x03, 0x1F, 0x00, ARRAY_RANGE(TABLE_SFT_STAT), ARRAY_RANGE(INDEX_EMPTY)},
    // BIT dynamic 0x68-0x6A; opcode [*|MODE(2)|*|Rn(4)]:
    //   zeroMask 0x90 = bits 7 and 4 must be 0
    {0x0068, 0x03, 0x9F, 0x90, ARRAY_RANGE(TABLE_BIT_DYN),  ARRAY_RANGE(INDEX_EMPTY)},
    // BIT static 0x6C-0x6E; opcode [*|MODE(2)|Imm5(5)]:
    //   zeroMask 0x80 = bit 7 must be 0
    {0x006C, 0x03, 0x9F, 0x80, ARRAY_RANGE(TABLE_BIT_STAT), ARRAY_RANGE(INDEX_EMPTY)},
    // MUL 0xEE; opcode [MODE(4)|****]:
    //   opMask 0xBF = ignore bit 4 (size) + bits 7,5,3-0 for matching
    //   zeroMask 0xAF = bits 7,5,3-0 must be 0
    {0x00EE, 0x00, 0xBF, 0xAF, ARRAY_RANGE(TABLE_MUL),      ARRAY_RANGE(INDEX_EMPTY)},
    // DIV 0xEF; same layout
    {0x00EF, 0x00, 0xBF, 0xAF, ARRAY_RANGE(TABLE_DIV),      ARRAY_RANGE(INDEX_EMPTY)},
    // Bcc:G 0xA4-0xA6; opcode [****|cc(4)]:
    //   opMask 0xF0 = ignore high nibble for matching
    //   zeroMask 0xF0 = high nibble must be 0
    {0x00A4, 0x03, 0xF0, 0xF0, ARRAY_RANGE(TABLE_BCC_G),    ARRAY_RANGE(INDEX_EMPTY)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
    {HD641016, TEXT_CPU_HD641016, ARRAY_RANGE(H16_PAGES)},
};

static const Cpu *cpu(CpuType cpuType) {
    return Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
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
    if (emask != 0 && (insn.opCode() & emask) == emask)
        return false;
    return (insn.opCode() & ~emask) == entry->readOpCode();
}

Error searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) {
    cpu(cpuType)->searchOpCode(insn, out, matchOpCode, pageMatcher);
    return insn.getError();
}

static bool acceptMode(AsmInsn &, const Entry *) {
    return false;
}

Error searchName(CpuType cpuType, AsmInsn &insn) {
    cpu(cpuType)->searchName(insn, acceptMode);
    return insn.getError();
}

const /*PROGMEM*/ char *TableH16::listCpu_P() const {
    return TEXT_H16_LIST;
}

const /*PROGMEM*/ char *TableH16::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableH16::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    if (name.iequals_P(TEXT_CPU_HD641016) || name.iequals_P(TEXT_CPU_HD641016 + 2)) {
        cpuType = HD641016;
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

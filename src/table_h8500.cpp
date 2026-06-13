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

#define E2(opc, fmt, esz, name, src, dst) \
    {opc, Entry::Flags::create(fmt, esz, src, dst), name}
#define E1(opc, fmt, esz, name, src) E2(opc, fmt, esz, name, src, M_NONE)
#define E0(opc, fmt, name)           E1(opc, fmt, EA_WORD, name, M_NONE)

// General/Extended format instructions, valid on all CPU variants.
constexpr Entry TABLE_GEN[] PROGMEM = {
    E2(0x20, FMT_GEN, EA_WORD, TEXT_ADD_G,  M_EA,   M_REG),
    E2(0x08, FMT_GEN, EA_WORD, TEXT_ADD_Q,  M_EA,   M_NONE),
    E2(0x09, FMT_GEN, EA_WORD, TEXT_ADD_Q,  M_EA,   M_NONE),
    E2(0x0C, FMT_GEN, EA_WORD, TEXT_ADD_Q,  M_EA,   M_NONE),
    E2(0x0D, FMT_GEN, EA_WORD, TEXT_ADD_Q,  M_EA,   M_NONE),
    E2(0x28, FMT_GEN, EA_WORD, TEXT_ADDS,   M_EA,   M_REG),
    E2(0xA0, FMT_GEN, EA_BYTE, TEXT_ADDX,   M_EA,   M_REG),
    E2(0x58, FMT_GEN, EA_BYTE, TEXT_ANDC,   M_IMM8, M_CR),
    E2(0x58, FMT_GEN, EA_BYTE, TEXT_BCLR,   M_REG,  M_EA),
    E2(0xD0, FMT_GEN, EA_BYTE, TEXT_BCLR,   M_BIT,  M_EA),
    E2(0x68, FMT_GEN, EA_BYTE, TEXT_BNOT,   M_REG,  M_EA),
    E2(0xE0, FMT_GEN, EA_BYTE, TEXT_BNOT,   M_BIT,  M_EA),
    E2(0x48, FMT_GEN, EA_BYTE, TEXT_BSET,   M_REG,  M_EA),
    E2(0xC0, FMT_GEN, EA_BYTE, TEXT_BSET,   M_BIT,  M_EA),
    E2(0x78, FMT_GEN, EA_BYTE, TEXT_BTST,   M_REG,  M_EA),
    E2(0xF0, FMT_GEN, EA_BYTE, TEXT_BTST,   M_BIT,  M_EA),
    E2(0x13, FMT_GEN, EA_WORD, TEXT_CLR,    M_EA,   M_NONE),
    E2(0x70, FMT_GEN, EA_WORD, TEXT_CMP_G,  M_EA,   M_REG),
    E2(0xA0, FMT_EXT, EA_BYTE, TEXT_DADD,   M_EA,   M_REG),
    E2(0xB8, FMT_GEN, EA_BYTE, TEXT_DIVXU,  M_EA,   M_REG),
    E2(0xB0, FMT_EXT, EA_BYTE, TEXT_DSUB,   M_EA,   M_REG),
    E2(0x11, FMT_GEN, EA_BYTE, TEXT_EXTS,   M_EA,   M_NONE),
    E2(0x12, FMT_GEN, EA_BYTE, TEXT_EXTU,   M_EA,   M_NONE),
    E2(0x88, FMT_GEN, EA_WORD, TEXT_LDC,    M_EA,   M_CR),
    E2(0x80, FMT_GEN, EA_WORD, TEXT_MOV_G,  M_EA,   M_REG),
    E2(0x90, FMT_GEN, EA_WORD, TEXT_MOV_G,  M_REG,  M_EA),
    E2(0xA8, FMT_GEN, EA_BYTE, TEXT_MULXU,  M_EA,   M_REG),
    E2(0x14, FMT_GEN, EA_WORD, TEXT_NEG,    M_EA,   M_NONE),
    E2(0x15, FMT_GEN, EA_WORD, TEXT_NOT,    M_EA,   M_NONE),
    E2(0x40, FMT_GEN, EA_WORD, TEXT_OR,     M_EA,   M_REG),
    E2(0x78, FMT_GEN, EA_BYTE, TEXT_ORC,    M_IMM8, M_CR),
    E2(0x3C, FMT_GEN, EA_WORD, TEXT_ROTL,   M_EA,   M_NONE),
    E2(0x3D, FMT_GEN, EA_WORD, TEXT_ROTR,   M_EA,   M_NONE),
    E2(0x3E, FMT_GEN, EA_WORD, TEXT_ROTXL,  M_EA,   M_NONE),
    E2(0x3F, FMT_GEN, EA_WORD, TEXT_ROTXR,  M_EA,   M_NONE),
    E2(0x18, FMT_GEN, EA_WORD, TEXT_SHAL,   M_EA,   M_NONE),
    E2(0x19, FMT_GEN, EA_WORD, TEXT_SHAR,   M_EA,   M_NONE),
    E2(0x1A, FMT_GEN, EA_WORD, TEXT_SHLL,   M_EA,   M_NONE),
    E2(0x1B, FMT_GEN, EA_WORD, TEXT_SHLR,   M_EA,   M_NONE),
    E2(0x98, FMT_GEN, EA_WORD, TEXT_STC,    M_CR,   M_EA),
    E2(0x30, FMT_GEN, EA_WORD, TEXT_SUB,    M_EA,   M_REG),
    E2(0xB0, FMT_GEN, EA_BYTE, TEXT_SUBX,   M_EA,   M_REG),
    E2(0x38, FMT_GEN, EA_WORD, TEXT_SUBS,   M_EA,   M_REG),
    E2(0x10, FMT_GEN, EA_BYTE, TEXT_SWAP,   M_EA,   M_NONE),
    E2(0x17, FMT_GEN, EA_BYTE, TEXT_TAS,    M_EA,   M_NONE),
    E2(0x16, FMT_GEN, EA_WORD, TEXT_TST,    M_EA,   M_NONE),
    E2(0x90, FMT_GEN, EA_WORD, TEXT_XCH,    M_REG,  M_EA),
    E2(0x50, FMT_GEN, EA_WORD, TEXT_AND,    M_EA,   M_REG),
    E2(0x60, FMT_GEN, EA_WORD, TEXT_XOR,    M_EA,   M_REG),
    E2(0x68, FMT_GEN, EA_BYTE, TEXT_XORC,   M_IMM8, M_CR),
};

constexpr uint8_t INDEX_GEN[] PROGMEM = {
      0,  // TEXT_ADD_G
      1,  // TEXT_ADD_Q
      2,  // TEXT_ADD_Q
      3,  // TEXT_ADD_Q
      4,  // TEXT_ADD_Q
      5,  // TEXT_ADDS
      6,  // TEXT_ADDX
     47,  // TEXT_AND
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
     17,  // TEXT_CMP_G
     18,  // TEXT_DADD
     19,  // TEXT_DIVXU
     20,  // TEXT_DSUB
     21,  // TEXT_EXTS
     22,  // TEXT_EXTU
     23,  // TEXT_LDC
     24,  // TEXT_MOV_G
     25,  // TEXT_MOV_G
     26,  // TEXT_MULXU
     27,  // TEXT_NEG
     28,  // TEXT_NOT
     29,  // TEXT_OR
     30,  // TEXT_ORC
     31,  // TEXT_ROTL
     32,  // TEXT_ROTR
     33,  // TEXT_ROTXL
     34,  // TEXT_ROTXR
     35,  // TEXT_SHAL
     36,  // TEXT_SHAR
     37,  // TEXT_SHLL
     38,  // TEXT_SHLR
     39,  // TEXT_STC
     40,  // TEXT_SUB
     42,  // TEXT_SUBS
     41,  // TEXT_SUBX
     43,  // TEXT_SWAP
     44,  // TEXT_TAS
     45,  // TEXT_TST
     46,  // TEXT_XCH
     48,  // TEXT_XOR
     49,  // TEXT_XORC
};

// GEN/EXT instructions for H8/500 only (E-clock peripheral bus).
constexpr Entry TABLE_GEN_ECLOCK[] PROGMEM = {
    E2(0x80, FMT_EXT, EA_BYTE, TEXT_MOVFPE, M_EA,  M_REG),
    E2(0x90, FMT_EXT, EA_WORD, TEXT_MOVTPE, M_REG, M_EA),
};

constexpr uint8_t INDEX_GEN_ECLOCK[] PROGMEM = {
      0,  // TEXT_MOVFPE
      1,  // TEXT_MOVTPE
};

// Special (Format B) instructions, valid on all CPU variants.
// PJMP/PJSR @aa:24 are max-mode only.
constexpr Entry TABLE_SPC[] PROGMEM = {
    E2(0x09, FMT_SPC, EA_WORD, TEXT_ADD_Q,  M_IMM2,   M_REG),
    E1(0x24, FMT_SPC, EA_WORD, TEXT_BCC,    M_PCREL8),
    E1(0x25, FMT_SPC, EA_WORD, TEXT_BCS,    M_PCREL8),
    E1(0x27, FMT_SPC, EA_WORD, TEXT_BEQ,    M_PCREL8),
    E1(0x21, FMT_SPC, EA_WORD, TEXT_BF,     M_PCREL8),
    E1(0x2C, FMT_SPC, EA_WORD, TEXT_BGE,    M_PCREL8),
    E1(0x2E, FMT_SPC, EA_WORD, TEXT_BGT,    M_PCREL8),
    E1(0x22, FMT_SPC, EA_WORD, TEXT_BHI,    M_PCREL8),
    E1(0x24, FMT_SPC, EA_WORD, TEXT_BHS,    M_PCREL8),
    E1(0x2F, FMT_SPC, EA_WORD, TEXT_BLE,    M_PCREL8),
    E1(0x25, FMT_SPC, EA_WORD, TEXT_BLO,    M_PCREL8),
    E1(0x23, FMT_SPC, EA_WORD, TEXT_BLS,    M_PCREL8),
    E1(0x2D, FMT_SPC, EA_WORD, TEXT_BLT,    M_PCREL8),
    E1(0x2B, FMT_SPC, EA_WORD, TEXT_BMI,    M_PCREL8),
    E1(0x26, FMT_SPC, EA_WORD, TEXT_BNE,    M_PCREL8),
    E1(0x2A, FMT_SPC, EA_WORD, TEXT_BPL,    M_PCREL8),
    E1(0x20, FMT_SPC, EA_WORD, TEXT_BRA,    M_PCREL8),
    E1(0x21, FMT_SPC, EA_WORD, TEXT_BRN,    M_PCREL8),
    E1(0x0E, FMT_SPC, EA_WORD, TEXT_BSR,    M_PCREL8),
    E1(0x20, FMT_SPC, EA_WORD, TEXT_BT,     M_PCREL8),
    E1(0x28, FMT_SPC, EA_WORD, TEXT_BVC,    M_PCREL8),
    E1(0x29, FMT_SPC, EA_WORD, TEXT_BVS,    M_PCREL8),
    E1(0x34, FMT_SPC, EA_WORD, TEXT_BCC,    M_PCREL16),
    E1(0x35, FMT_SPC, EA_WORD, TEXT_BCS,    M_PCREL16),
    E1(0x37, FMT_SPC, EA_WORD, TEXT_BEQ,    M_PCREL16),
    E1(0x34, FMT_SPC, EA_WORD, TEXT_BHS,    M_PCREL16),
    E1(0x35, FMT_SPC, EA_WORD, TEXT_BLO,    M_PCREL16),
    E1(0x33, FMT_SPC, EA_WORD, TEXT_BLS,    M_PCREL16),
    E1(0x3D, FMT_SPC, EA_WORD, TEXT_BLT,    M_PCREL16),
    E1(0x3B, FMT_SPC, EA_WORD, TEXT_BMI,    M_PCREL16),
    E1(0x36, FMT_SPC, EA_WORD, TEXT_BNE,    M_PCREL16),
    E1(0x3A, FMT_SPC, EA_WORD, TEXT_BPL,    M_PCREL16),
    E1(0x30, FMT_SPC, EA_WORD, TEXT_BRA,    M_PCREL16),
    E1(0x1E, FMT_SPC, EA_WORD, TEXT_BSR,    M_PCREL16),
    E1(0x30, FMT_SPC, EA_WORD, TEXT_BT,     M_PCREL16),
    E1(0x38, FMT_SPC, EA_WORD, TEXT_BVC,    M_PCREL16),
    E1(0x39, FMT_SPC, EA_WORD, TEXT_BVS,    M_PCREL16),
    E2(0x40, FMT_SPC, EA_BYTE, TEXT_CMP_E,  M_IMM8,   M_REG),
    E2(0x48, FMT_SPC, EA_WORD, TEXT_CMP_I,  M_IMM16,  M_REG),
    E1(0x10, FMT_SPC, EA_WORD, TEXT_JMP,    M_ABS16),
    E1(0x18, FMT_SPC, EA_WORD, TEXT_JSR,    M_ABS16),
    E1(0x02, FMT_SPC, EA_WORD, TEXT_LDM,    M_REGLIST),
    E2(0x17, FMT_SPC, EA_WORD, TEXT_LINK,   M_FP,     M_IMM8),
    E2(0x1F, FMT_SPC, EA_WORD, TEXT_LINK,   M_FP,     M_IMM16),
    E2(0x50, FMT_SPC, EA_BYTE, TEXT_MOV_E,  M_IMM8,   M_REG),
    E2(0x80, FMT_SPC, EA_WORD, TEXT_MOV_F,  M_DISP8F, M_REG),
    E2(0x90, FMT_SPC, EA_WORD, TEXT_MOV_F,  M_REG,    M_DISP8F),
    E2(0x58, FMT_SPC, EA_WORD, TEXT_MOV_I,  M_IMM16,  M_REG),
    E2(0x60, FMT_SPC, EA_WORD, TEXT_MOV_L,  M_ABS8,   M_REG),
    E2(0x70, FMT_SPC, EA_WORD, TEXT_MOV_S,  M_REG,    M_ABS8),
    E0(0x00, FMT_SPC,           TEXT_NOP),
    E1(0x14, FMT_SPC, EA_WORD, TEXT_RTD,    M_IMM8),
    E1(0x1C, FMT_SPC, EA_WORD, TEXT_RTD,    M_IMM16),
    E0(0x0A, FMT_SPC,           TEXT_RTE),
    E0(0x19, FMT_SPC,           TEXT_RTS),
    E2(0x07, FMT_SPC, EA_WORD, TEXT_SCB_EQ, M_SCB,    M_PCREL8),
    E2(0x01, FMT_SPC, EA_WORD, TEXT_SCB_F,  M_SCB,    M_PCREL8),
    E2(0x06, FMT_SPC, EA_WORD, TEXT_SCB_NE, M_SCB,    M_PCREL8),
    E0(0x1A, FMT_SPC,           TEXT_SLEEP),
    E1(0x12, FMT_SPC, EA_WORD, TEXT_STM,    M_REGLIST),
    E0(0x09, FMT_SPC,           TEXT_TRAP_VS),
    E1(0x08, FMT_SPC, EA_WORD, TEXT_TRAPA,  M_TRAPV),
    E1(0x0F, FMT_SPC, EA_WORD, TEXT_UNLK,   M_FP),
};

constexpr uint8_t INDEX_SPC[] PROGMEM = {
      0,  // TEXT_ADD_Q
      1,  // TEXT_BCC
     22,  // TEXT_BCC
      2,  // TEXT_BCS
     23,  // TEXT_BCS
      3,  // TEXT_BEQ
     24,  // TEXT_BEQ
      4,  // TEXT_BF
      5,  // TEXT_BGE
      6,  // TEXT_BGT
      7,  // TEXT_BHI
      8,  // TEXT_BHS
     25,  // TEXT_BHS
      9,  // TEXT_BLE
     10,  // TEXT_BLO
     26,  // TEXT_BLO
     11,  // TEXT_BLS
     27,  // TEXT_BLS
     12,  // TEXT_BLT
     28,  // TEXT_BLT
     13,  // TEXT_BMI
     29,  // TEXT_BMI
     14,  // TEXT_BNE
     30,  // TEXT_BNE
     15,  // TEXT_BPL
     31,  // TEXT_BPL
     16,  // TEXT_BRA
     32,  // TEXT_BRA
     17,  // TEXT_BRN
     18,  // TEXT_BSR
     33,  // TEXT_BSR
     19,  // TEXT_BT
     34,  // TEXT_BT
     20,  // TEXT_BVC
     35,  // TEXT_BVC
     21,  // TEXT_BVS
     36,  // TEXT_BVS
     37,  // TEXT_CMP_E
     38,  // TEXT_CMP_I
     39,  // TEXT_JMP
     40,  // TEXT_JSR
     41,  // TEXT_LDM
     42,  // TEXT_LINK
     43,  // TEXT_LINK
     44,  // TEXT_MOV_E
     45,  // TEXT_MOV_F
     46,  // TEXT_MOV_F
     47,  // TEXT_MOV_I
     48,  // TEXT_MOV_L
     49,  // TEXT_MOV_S
     50,  // TEXT_NOP
     51,  // TEXT_RTD
     52,  // TEXT_RTD
     53,  // TEXT_RTE
     54,  // TEXT_RTS
     55,  // TEXT_SCB_EQ
     56,  // TEXT_SCB_F
     57,  // TEXT_SCB_NE
     58,  // TEXT_SLEEP
     59,  // TEXT_STM
     60,  // TEXT_TRAP_VS
     61,  // TEXT_TRAPA
     62,  // TEXT_UNLK
};

// Secondary (0x11-prefix) instructions, valid on all CPU variants.
// PJMP @Rn / PJSR @Rn / PRTS / PRTD are max-mode only at runtime.
constexpr Entry TABLE_SEC[] PROGMEM = {
    E1(0xD0, FMT_SEC, EA_WORD, TEXT_JMP,    M_IND),
    E1(0xE0, FMT_SEC, EA_WORD, TEXT_JMP,    M_DISP8),
    E1(0xF0, FMT_SEC, EA_WORD, TEXT_JMP,    M_DISP16),
    E1(0xD8, FMT_SEC, EA_WORD, TEXT_JSR,    M_IND),
    E1(0xE8, FMT_SEC, EA_WORD, TEXT_JSR,    M_DISP8),
    E1(0xF8, FMT_SEC, EA_WORD, TEXT_JSR,    M_DISP16),
};

constexpr uint8_t INDEX_SEC[] PROGMEM = {
      0,  // TEXT_JMP
      1,  // TEXT_JMP
      2,  // TEXT_JMP
      3,  // TEXT_JSR
      4,  // TEXT_JSR
      5,  // TEXT_JSR
};

// Page-jump SPC instructions (max-mode only): PJMP/PJSR with absolute 24-bit
// address. Hosted in a separate page that is only searched when MAXMODE is on.
constexpr Entry TABLE_SPC_MAX[] PROGMEM = {
    E1(0x03, FMT_SPC, EA_WORD, TEXT_PJSR,   M_ABS24),
    E1(0x13, FMT_SPC, EA_WORD, TEXT_PJMP,   M_ABS24),
};

constexpr uint8_t INDEX_SPC_MAX[] PROGMEM = {
      1,  // TEXT_PJMP
      0,  // TEXT_PJSR
};

// Page-jump SEC instructions (max-mode only): PJMP/PJSR @Rn, PRTS, PRTD.
constexpr Entry TABLE_SEC_MAX[] PROGMEM = {
    E1(0xC0, FMT_SEC, EA_WORD, TEXT_PJMP,   M_IND),
    E1(0xC8, FMT_SEC, EA_WORD, TEXT_PJSR,   M_IND),
    E1(0x14, FMT_SEC, EA_WORD, TEXT_PRTD,   M_IMM8),
    E1(0x1C, FMT_SEC, EA_WORD, TEXT_PRTD,   M_IMM16),
    E0(0x19, FMT_SEC,           TEXT_PRTS),
};

constexpr uint8_t INDEX_SEC_MAX[] PROGMEM = {
      0,  // TEXT_PJMP
      1,  // TEXT_PJSR
      2,  // TEXT_PRTD
      3,  // TEXT_PRTD
      4,  // TEXT_PRTS
};

#undef E2
#undef E1
#undef E0
#undef E2M
#undef E1M
#undef E0M

// clang-format on

// EntryPage: extends PrefixTableBase with a maxModeOnly bit so the page
// matcher can skip pages whose instructions are only valid in maximum mode.
// The framework's defaultPageMatcher checks insn.prefix() against
// page->readPrefix(); pageMatcher below adds the maxMode guard.
struct EntryPage : entry::PrefixTableBase<Entry> {
    constexpr EntryPage(Config::opcode_t prefix, bool maxModeOnly, const Entry *head_P,
            const Entry *tail_P, const uint8_t *index_P, const uint8_t *itail_P)
        : PrefixTableBase(prefix, head_P, tail_P, index_P, itail_P),
          _maxModeOnly(maxModeOnly) {}

    bool maxModeOnly() const { return pgm_read_byte(&_maxModeOnly) != 0; }

private:
    const uint8_t _maxModeOnly;
};

constexpr EntryPage H8500_PAGES[] PROGMEM = {
    {0x00, false, ARRAY_RANGE(TABLE_GEN),        ARRAY_RANGE(INDEX_GEN)},
    {0x00, false, ARRAY_RANGE(TABLE_GEN_ECLOCK), ARRAY_RANGE(INDEX_GEN_ECLOCK)},
    {0x00, false, ARRAY_RANGE(TABLE_SPC),        ARRAY_RANGE(INDEX_SPC)},
    {0x00, true,  ARRAY_RANGE(TABLE_SPC_MAX),    ARRAY_RANGE(INDEX_SPC_MAX)},
    {0x11, false, ARRAY_RANGE(TABLE_SEC),        ARRAY_RANGE(INDEX_SEC)},
    {0x11, true,  ARRAY_RANGE(TABLE_SEC_MAX),    ARRAY_RANGE(INDEX_SEC_MAX)},
};

// H8/520 has no E-clock peripheral interface.
constexpr EntryPage H8520_PAGES[] PROGMEM = {
    {0x00, false, ARRAY_RANGE(TABLE_GEN),     ARRAY_RANGE(INDEX_GEN)},
    {0x00, false, ARRAY_RANGE(TABLE_SPC),     ARRAY_RANGE(INDEX_SPC)},
    {0x00, true,  ARRAY_RANGE(TABLE_SPC_MAX), ARRAY_RANGE(INDEX_SPC_MAX)},
    {0x11, false, ARRAY_RANGE(TABLE_SEC),     ARRAY_RANGE(INDEX_SEC)},
    {0x11, true,  ARRAY_RANGE(TABLE_SEC_MAX), ARRAY_RANGE(INDEX_SEC_MAX)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
    {H8_500, TEXT_CPU_H8500, ARRAY_RANGE(H8500_PAGES)},
    {H8_520, TEXT_CPU_H8520, ARRAY_RANGE(H8520_PAGES)},
};

static const Cpu *cpu(CpuType cpuType) {
    return Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
}

// Mask of variable bits in the OP byte for GEN/EXT instructions.
static uint8_t opMaskOf(const Entry::Flags &flags) {
    const auto src = flags.src();
    const auto dst = flags.dst();
    if (src == M_BIT)
        return 0x0F;
    if (src == M_REG || src == M_CR || dst == M_REG || dst == M_CR)
        return 0x07;
    return 0x00;
}

// Mask of variable bits in the SEC opcode byte. Register-bearing forms
// (JMP/JSR/PJMP/PJSR with @Rn or @(d,Rn)) embed the register in low 3 bits.
static uint8_t secMaskOf(const Entry::Flags &flags) {
    const auto src = flags.src();
    if (src == M_IND || src == M_DISP8 || src == M_DISP16)
        return 0x07;
    return 0x00;
}

bool isEaByte(uint8_t b) {
    if (b >= 0xA0)
        return true;
    return b == 0x04 || b == 0x05 || b == 0x0C || b == 0x0D || b == 0x15 || b == 0x1D;
}

// matchOpCode: used by searchOpCode for disassembly.
// insn.insnFmt must be set before calling searchOpCode.
static bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *) {
    const auto flags = entry->readFlags();
    if (flags.fmt() != insn.insnFmt)
        return false;
    if (insn.insnFmt == FMT_SPC) {
        if (flags.src() == M_IMM2)
            return false;  // ADD:Q SPC entry is for assembly only
        const auto mask = opMaskOf(flags);
        if (mask && (entry->readOpCode() & mask))
            return insn.opCode() == entry->readOpCode();
        return (insn.opCode() & ~mask) == entry->readOpCode();
    }
    if (insn.insnFmt == FMT_SEC) {
        const auto mask = secMaskOf(flags);
        return (insn.opCode() & ~mask) == entry->readOpCode();
    }
    // GEN/EXT: disambiguate M_IMM8-src (ANDC/ORC/XORC) from M_REG-src
    // (BCLR/BTST/BNOT) instructions sharing the same base OP byte.
    if (flags.src() == M_IMM8 && insn.eaByte != 0x04)
        return false;
    if (flags.src() == M_REG && insn.eaByte == 0x04)
        return false;
    return (insn.opByte & ~opMaskOf(flags)) == entry->readOpCode();
}

bool isPrefix(CpuType /*cpuType*/, Config::opcode_t code) {
    return code == 0x11;
}

// The disassembler decodes page-jump opcodes unconditionally - the byte
// sequences uniquely identify them, so emitting "PJMP" against an unknown
// max-mode setting is more useful than reporting "unknown instruction".
// The MAXMODE-only gating is enforced only at assembly time (acceptModes).

Error searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) {
    cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    return insn.getError();
}

// EA-encodable modes occupy a contiguous range in AddrMode (see entry_h8500.h)
// so the check reduces to a single comparison.
static bool isEaMode(AddrMode mode) {
    return mode >= M_REG && mode <= M_IMM16;
}

// Accept |op| as a value for the |table|-declared mode. The fast path is the
// exact-match check; the switch only covers the H8/500's "fits-into-this-mode"
// relaxations (M_DISP8F satisfies M_DISP8, etc.) and the catch-all table modes.
static bool acceptMode(const Operand &op, AddrMode table) {
    if (op.mode == table)
        return true;
    switch (table) {
    case M_EA:
        return isEaMode(op.mode);
    case M_BIT:
        return op.mode == M_IMM8;
    case M_IMM2:
    case M_TRAPV:
        return op.mode == M_IMM8;
    case M_SCB:
        return op.mode == M_REG;
    case M_FP:
        // LINK/UNLK take the frame pointer (R6) as a fixed register.
        return op.mode == M_REG && op.reg == REG_FP;
    case M_DISP8:
        // M_DISP8 also accepts @(d:8,FP); the parser produces M_DISP8F when
        // the base register is FP, but the encoding is the same as @(d:8,Rn).
        return op.mode == M_DISP8F;
    default:
        return false;
    }
}

static bool acceptModes(AsmInsn &insn, const Entry *entry) {
    // The framework has no built-in page reject; pageSetup stashes the page's
    // maxModeOnly bit on insn so acceptModes can drop entries from a MAXMODE-
    // only page when MIN-mode is active.
    if (insn.currentPageMaxOnly && !insn.maxMode)
        return false;
    const auto flags = entry->readFlags();
    return acceptMode(insn.op1, flags.src()) && acceptMode(insn.op2, flags.dst());
}

static void pageSetup(AsmInsn &insn, const EntryPage *page) {
    insn.currentPageMaxOnly = page->maxModeOnly();
}

Error searchName(CpuType cpuType, AsmInsn &insn) {
    cpu(cpuType)->searchName(insn, acceptModes, pageSetup);
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

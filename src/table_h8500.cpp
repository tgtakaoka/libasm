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

// General/Extended format instructions, valid on all 3 CPU variants.
// Index 0–49 (50 entries).
constexpr Entry TABLE_GEN[] PROGMEM = {
    E2(0x20, FMT_GEN, EA_WORD, TEXT_ADD_G,  M_EA,   M_REG),   //  0
    E2(0x08, FMT_GEN, EA_WORD, TEXT_ADD_Q,  M_EA,   M_NONE),  //  1
    E2(0x09, FMT_GEN, EA_WORD, TEXT_ADD_Q,  M_EA,   M_NONE),  //  2
    E2(0x0C, FMT_GEN, EA_WORD, TEXT_ADD_Q,  M_EA,   M_NONE),  //  3
    E2(0x0D, FMT_GEN, EA_WORD, TEXT_ADD_Q,  M_EA,   M_NONE),  //  4
    E2(0x28, FMT_GEN, EA_WORD, TEXT_ADDS,   M_EA,   M_REG),   //  5
    E2(0xA0, FMT_GEN, EA_BYTE, TEXT_ADDX,   M_EA,   M_REG),   //  6
    E2(0x58, FMT_GEN, EA_BYTE, TEXT_ANDC,   M_IMM8, M_CR),    //  7
    E2(0x58, FMT_GEN, EA_BYTE, TEXT_BCLR,   M_REG,  M_EA),    //  8
    E2(0xD0, FMT_GEN, EA_BYTE, TEXT_BCLR,   M_BIT,  M_EA),    //  9
    E2(0x68, FMT_GEN, EA_BYTE, TEXT_BNOT,   M_REG,  M_EA),    // 10
    E2(0xE0, FMT_GEN, EA_BYTE, TEXT_BNOT,   M_BIT,  M_EA),    // 11
    E2(0x48, FMT_GEN, EA_BYTE, TEXT_BSET,   M_REG,  M_EA),    // 12
    E2(0xC0, FMT_GEN, EA_BYTE, TEXT_BSET,   M_BIT,  M_EA),    // 13
    E2(0x78, FMT_GEN, EA_BYTE, TEXT_BTST,   M_REG,  M_EA),    // 14
    E2(0xF0, FMT_GEN, EA_BYTE, TEXT_BTST,   M_BIT,  M_EA),    // 15
    E2(0x13, FMT_GEN, EA_WORD, TEXT_CLR,    M_EA,   M_NONE),  // 16
    E2(0x70, FMT_GEN, EA_WORD, TEXT_CMP_G,  M_EA,   M_REG),   // 17
    E2(0xA0, FMT_EXT, EA_BYTE, TEXT_DADD,   M_EA,   M_REG),   // 18
    E2(0xB8, FMT_GEN, EA_BYTE, TEXT_DIVXU,  M_EA,   M_REG),   // 19
    E2(0xB0, FMT_EXT, EA_BYTE, TEXT_DSUB,   M_EA,   M_REG),   // 20
    E2(0x11, FMT_GEN, EA_BYTE, TEXT_EXTS,   M_EA,   M_NONE),  // 21
    E2(0x12, FMT_GEN, EA_BYTE, TEXT_EXTU,   M_EA,   M_NONE),  // 22
    E2(0x88, FMT_GEN, EA_WORD, TEXT_LDC,    M_EA,   M_CR),    // 23
    E2(0x80, FMT_GEN, EA_WORD, TEXT_MOV_G,  M_EA,   M_REG),   // 24
    E2(0x90, FMT_GEN, EA_WORD, TEXT_MOV_G,  M_REG,  M_EA),    // 25
    E2(0xA8, FMT_GEN, EA_BYTE, TEXT_MULXU,  M_EA,   M_REG),   // 26
    E2(0x14, FMT_GEN, EA_WORD, TEXT_NEG,    M_EA,   M_NONE),  // 27
    E2(0x15, FMT_GEN, EA_WORD, TEXT_NOT,    M_EA,   M_NONE),  // 28
    E2(0x40, FMT_GEN, EA_WORD, TEXT_OR,     M_EA,   M_REG),   // 29
    E2(0x78, FMT_GEN, EA_BYTE, TEXT_ORC,    M_IMM8, M_CR),    // 30
    E2(0x3C, FMT_GEN, EA_WORD, TEXT_ROTL,   M_EA,   M_NONE),  // 31
    E2(0x3D, FMT_GEN, EA_WORD, TEXT_ROTR,   M_EA,   M_NONE),  // 32
    E2(0x3E, FMT_GEN, EA_WORD, TEXT_ROTXL,  M_EA,   M_NONE),  // 33
    E2(0x3F, FMT_GEN, EA_WORD, TEXT_ROTXR,  M_EA,   M_NONE),  // 34
    E2(0x18, FMT_GEN, EA_WORD, TEXT_SHAL,   M_EA,   M_NONE),  // 35
    E2(0x19, FMT_GEN, EA_WORD, TEXT_SHAR,   M_EA,   M_NONE),  // 36
    E2(0x1A, FMT_GEN, EA_WORD, TEXT_SHLL,   M_EA,   M_NONE),  // 37
    E2(0x1B, FMT_GEN, EA_WORD, TEXT_SHLR,   M_EA,   M_NONE),  // 38
    E2(0x98, FMT_GEN, EA_WORD, TEXT_STC,    M_CR,   M_EA),    // 39
    E2(0x30, FMT_GEN, EA_WORD, TEXT_SUB,    M_EA,   M_REG),   // 40
    E2(0xB0, FMT_GEN, EA_BYTE, TEXT_SUBX,   M_EA,   M_REG),   // 41
    E2(0x38, FMT_GEN, EA_WORD, TEXT_SUBS,   M_EA,   M_REG),   // 42
    E2(0x10, FMT_GEN, EA_BYTE, TEXT_SWAP,   M_EA,   M_NONE),  // 43
    E2(0x17, FMT_GEN, EA_BYTE, TEXT_TAS,    M_EA,   M_NONE),  // 44
    E2(0x16, FMT_GEN, EA_WORD, TEXT_TST,    M_EA,   M_NONE),  // 45
    E2(0x90, FMT_GEN, EA_WORD, TEXT_XCH,    M_REG,  M_EA),    // 46
    E2(0x50, FMT_GEN, EA_WORD, TEXT_AND,    M_EA,   M_REG),   // 47
    E2(0x60, FMT_GEN, EA_WORD, TEXT_XOR,    M_EA,   M_REG),   // 48
    E2(0x68, FMT_GEN, EA_BYTE, TEXT_XORC,   M_IMM8, M_CR),    // 49
};

// Sorted by name: ADD:G(0) ADD:Q(1,2,3,4) ADDS(5) ADDX(6) AND(47) ANDC(7)
//   BCLR(8,9) BNOT(10,11) BSET(12,13) BTST(14,15) CLR(16) CMP:G(17)
//   DADD(18) DIVXU(19) DSUB(20) EXTS(21) EXTU(22) LDC(23) MOV:G(24,25)
//   MULXU(26) NEG(27) NOT(28) OR(29) ORC(30) ROTL(31) ROTR(32) ROTXL(33)
//   ROTXR(34) SHAL(35) SHAR(36) SHLL(37) SHLR(38) STC(39) SUB(40)
//   SUBX(41) SUBS(42) SWAP(43) TAS(44) TST(45) XCH(46) XOR(48) XORC(49)
constexpr uint8_t INDEX_GEN[] PROGMEM = {
     0,  1,  2,  3,  4,  5,  6, 47,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
    31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 42, 41, 43, 44, 45, 46,
    48, 49,
};

// GEN/EXT instructions for H8/500 and H8/532 only (E-clock peripheral bus).
constexpr Entry TABLE_GEN_ECLOCK[] PROGMEM = {
    E2(0x80, FMT_EXT, EA_BYTE, TEXT_MOVFPE, M_EA,  M_REG),   // 0
    E2(0x90, FMT_EXT, EA_BYTE, TEXT_MOVTPE, M_REG, M_EA),    // 1
};
// Sorted: MOVFPE(0) MOVTPE(1)
constexpr uint8_t INDEX_GEN_ECLOCK[] PROGMEM = {0, 1};

// Special (Format B) instructions, valid on all 3 CPU variants.
// BCC/BHI/BGE/BGT/BLE/BRN have d:8 form only; remaining Bcc have both d:8 and d:16.
// 0x31=RTS 0x32=SLEEP 0x3C=RTD:16 0x3E=BSR:16 0x3F=LINK:16 occupy the d:16 Bcc slots.
// Index 0–61 (62 entries).
constexpr Entry TABLE_SPC[] PROGMEM = {
    E2(0x09, FMT_SPC, EA_WORD, TEXT_ADD_Q,  M_IMM2,   M_REG),   //  0
    // Bcc d:8 (opcode = 0x20 | cc)
    E1(0x24, FMT_SPC, EA_WORD, TEXT_BCC,    M_PCREL8),           //  1
    E1(0x25, FMT_SPC, EA_WORD, TEXT_BCS,    M_PCREL8),           //  2
    E1(0x27, FMT_SPC, EA_WORD, TEXT_BEQ,    M_PCREL8),           //  3
    E1(0x21, FMT_SPC, EA_WORD, TEXT_BF,     M_PCREL8),           //  4
    E1(0x2C, FMT_SPC, EA_WORD, TEXT_BGE,    M_PCREL8),           //  5
    E1(0x2E, FMT_SPC, EA_WORD, TEXT_BGT,    M_PCREL8),           //  6
    E1(0x22, FMT_SPC, EA_WORD, TEXT_BHI,    M_PCREL8),           //  7
    E1(0x24, FMT_SPC, EA_WORD, TEXT_BHS,    M_PCREL8),           //  8
    E1(0x2F, FMT_SPC, EA_WORD, TEXT_BLE,    M_PCREL8),           //  9
    E1(0x25, FMT_SPC, EA_WORD, TEXT_BLO,    M_PCREL8),           // 10
    E1(0x23, FMT_SPC, EA_WORD, TEXT_BLS,    M_PCREL8),           // 11
    E1(0x2D, FMT_SPC, EA_WORD, TEXT_BLT,    M_PCREL8),           // 12
    E1(0x2B, FMT_SPC, EA_WORD, TEXT_BMI,    M_PCREL8),           // 13
    E1(0x26, FMT_SPC, EA_WORD, TEXT_BNE,    M_PCREL8),           // 14
    E1(0x2A, FMT_SPC, EA_WORD, TEXT_BPL,    M_PCREL8),           // 15
    E1(0x20, FMT_SPC, EA_WORD, TEXT_BRA,    M_PCREL8),           // 16
    E1(0x21, FMT_SPC, EA_WORD, TEXT_BRN,    M_PCREL8),           // 17
    E1(0x1E, FMT_SPC, EA_WORD, TEXT_BSR,    M_PCREL8),           // 18
    E1(0x20, FMT_SPC, EA_WORD, TEXT_BT,     M_PCREL8),           // 19
    E1(0x28, FMT_SPC, EA_WORD, TEXT_BVC,    M_PCREL8),           // 20
    E1(0x29, FMT_SPC, EA_WORD, TEXT_BVS,    M_PCREL8),           // 21
    // Bcc d:16 (cc=0,3,4,5,6,7,8,9,10,11,13 only; opcode = 0x30 | cc)
    E1(0x34, FMT_SPC, EA_WORD, TEXT_BCC,    M_PCREL16),          // 22
    E1(0x35, FMT_SPC, EA_WORD, TEXT_BCS,    M_PCREL16),          // 23
    E1(0x37, FMT_SPC, EA_WORD, TEXT_BEQ,    M_PCREL16),          // 24
    E1(0x34, FMT_SPC, EA_WORD, TEXT_BHS,    M_PCREL16),          // 25
    E1(0x35, FMT_SPC, EA_WORD, TEXT_BLO,    M_PCREL16),          // 26
    E1(0x33, FMT_SPC, EA_WORD, TEXT_BLS,    M_PCREL16),          // 27
    E1(0x3D, FMT_SPC, EA_WORD, TEXT_BLT,    M_PCREL16),          // 28
    E1(0x3B, FMT_SPC, EA_WORD, TEXT_BMI,    M_PCREL16),          // 29
    E1(0x36, FMT_SPC, EA_WORD, TEXT_BNE,    M_PCREL16),          // 30
    E1(0x3A, FMT_SPC, EA_WORD, TEXT_BPL,    M_PCREL16),          // 31
    E1(0x30, FMT_SPC, EA_WORD, TEXT_BRA,    M_PCREL16),          // 32
    E1(0x3E, FMT_SPC, EA_WORD, TEXT_BSR,    M_PCREL16),          // 33
    E1(0x30, FMT_SPC, EA_WORD, TEXT_BT,     M_PCREL16),          // 34
    E1(0x38, FMT_SPC, EA_WORD, TEXT_BVC,    M_PCREL16),          // 35
    E1(0x39, FMT_SPC, EA_WORD, TEXT_BVS,    M_PCREL16),          // 36
    E2(0x40, FMT_SPC, EA_BYTE, TEXT_CMP_E,  M_IMM8,   M_REG),   // 37
    E2(0x48, FMT_SPC, EA_WORD, TEXT_CMP_I,  M_IMM16,  M_REG),   // 38
    E1(0x10, FMT_SPC, EA_WORD, TEXT_JMP,    M_ABS16),            // 39
    E1(0x18, FMT_SPC, EA_WORD, TEXT_JSR,    M_ABS16),            // 40
    E1(0x02, FMT_SPC, EA_WORD, TEXT_LDM,    M_REGLIST),          // 41
    E2(0x17, FMT_SPC, EA_WORD, TEXT_LINK,   M_REG,    M_IMM8),   // 42
    E2(0x3F, FMT_SPC, EA_WORD, TEXT_LINK,   M_REG,    M_IMM16),  // 43
    E2(0x50, FMT_SPC, EA_BYTE, TEXT_MOV_E,  M_IMM8,   M_REG),   // 44
    E2(0x80, FMT_SPC, EA_WORD, TEXT_MOV_F,  M_DISP8F, M_REG),   // 45
    E2(0x90, FMT_SPC, EA_WORD, TEXT_MOV_F,  M_REG,    M_DISP8F), // 46
    E2(0x58, FMT_SPC, EA_WORD, TEXT_MOV_I,  M_IMM16,  M_REG),   // 47
    E2(0x60, FMT_SPC, EA_WORD, TEXT_MOV_L,  M_ABS8,   M_REG),   // 48
    E2(0x70, FMT_SPC, EA_WORD, TEXT_MOV_S,  M_REG,    M_ABS8),   // 49
    E0(0x00, FMT_SPC,           TEXT_NOP),                        // 50
    E1(0x14, FMT_SPC, EA_WORD, TEXT_RTD,    M_IMM8),             // 51
    E1(0x3C, FMT_SPC, EA_WORD, TEXT_RTD,    M_IMM16),            // 52
    E0(0x0A, FMT_SPC,           TEXT_RTE),                        // 53
    E0(0x31, FMT_SPC,           TEXT_RTS),                        // 54
    E2(0x07, FMT_SPC, EA_WORD, TEXT_SCB_EQ, M_SCB,    M_PCREL8), // 55
    E2(0x01, FMT_SPC, EA_WORD, TEXT_SCB_F,  M_SCB,    M_PCREL8), // 56
    E2(0x06, FMT_SPC, EA_WORD, TEXT_SCB_NE, M_SCB,    M_PCREL8), // 57
    E0(0x32, FMT_SPC,           TEXT_SLEEP),                      // 58
    E0(0x09, FMT_SPC,           TEXT_TRAP_VS),                    // 59
    E1(0x08, FMT_SPC, EA_WORD, TEXT_TRAPA,  M_TRAPV),            // 60
    E1(0x0B, FMT_SPC, EA_WORD, TEXT_UNLK,   M_REG),              // 61
};

// Sorted by name: ADD:Q(0) BCC(1,22) BCS(2,23) BEQ(3,24) BF(4) BGE(5) BGT(6)
//   BHI(7) BHS(8,25) BLE(9) BLO(10,26) BLS(11,27) BLT(12,28) BMI(13,29)
//   BNE(14,30) BPL(15,31) BRA(16,32) BRN(17) BSR(18,33) BT(19,34) BVC(20,35)
//   BVS(21,36) CMP:E(37) CMP:I(38) JMP(39) JSR(40) LDM(41) LINK(42,43)
//   MOV:E(44) MOV:F(45,46) MOV:I(47) MOV:L(48) MOV:S(49) NOP(50) RTD(51,52)
//   RTE(53) RTS(54) SCB/EQ(55) SCB/F(56) SCB/NE(57) SLEEP(58) TRAP/VS(59)
//   TRAPA(60) UNLK(61)
constexpr uint8_t INDEX_SPC[] PROGMEM = {
     0,  1, 22,  2, 23,  3, 24,  4,  5,  6,  7,  8, 25,  9, 10, 26,
    11, 27, 12, 28, 13, 29, 14, 30, 15, 31, 16, 32, 17, 18, 33, 19,
    34, 20, 35, 21, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
    48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61,
};

// H8/500 and H8/520 only: STM at opcode 0x03.
// H8/532 reuses opcode 0x03 for PJSR @aa:24.
constexpr Entry TABLE_SPC_NO_H8532[] PROGMEM = {
    E1(0x03, FMT_SPC, EA_WORD, TEXT_STM, M_REGLIST),  // 0
};
constexpr uint8_t INDEX_SPC_NO_H8532[] PROGMEM = {0};

// H8/532-only Format B instructions (absolute-address page jumps).
constexpr Entry TABLE_SPC_H8532[] PROGMEM = {
    E1(0x13, FMT_SPC, EA_WORD, TEXT_PJMP, M_ABS24),  // 0
    E1(0x03, FMT_SPC, EA_WORD, TEXT_PJSR, M_ABS24),  // 1
};
// Sorted: PJMP(0) PJSR(1)
constexpr uint8_t INDEX_SPC_H8532[] PROGMEM = {0, 1};

#undef E2
#undef E1
#undef E0

// clang-format on

using EntryPage = entry::TableBase<Entry>;

// H8/500: GEN + ECLOCK + SPC + STM (opcode 0x03)
constexpr EntryPage H8500_PAGES[] PROGMEM = {
    {ARRAY_RANGE(TABLE_GEN),          ARRAY_RANGE(INDEX_GEN)},
    {ARRAY_RANGE(TABLE_GEN_ECLOCK),   ARRAY_RANGE(INDEX_GEN_ECLOCK)},
    {ARRAY_RANGE(TABLE_SPC),          ARRAY_RANGE(INDEX_SPC)},
    {ARRAY_RANGE(TABLE_SPC_NO_H8532), ARRAY_RANGE(INDEX_SPC_NO_H8532)},
};

// H8/520: GEN + SPC + STM (no E-clock interface)
constexpr EntryPage H8520_PAGES[] PROGMEM = {
    {ARRAY_RANGE(TABLE_GEN),          ARRAY_RANGE(INDEX_GEN)},
    {ARRAY_RANGE(TABLE_SPC),          ARRAY_RANGE(INDEX_SPC)},
    {ARRAY_RANGE(TABLE_SPC_NO_H8532), ARRAY_RANGE(INDEX_SPC_NO_H8532)},
};

// H8/532: GEN + ECLOCK + SPC + H8532-specific
constexpr EntryPage H8532_PAGES[] PROGMEM = {
    {ARRAY_RANGE(TABLE_GEN),        ARRAY_RANGE(INDEX_GEN)},
    {ARRAY_RANGE(TABLE_GEN_ECLOCK), ARRAY_RANGE(INDEX_GEN_ECLOCK)},
    {ARRAY_RANGE(TABLE_SPC),        ARRAY_RANGE(INDEX_SPC)},
    {ARRAY_RANGE(TABLE_SPC_H8532),  ARRAY_RANGE(INDEX_SPC_H8532)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
    {H8_500, TEXT_CPU_H8500, ARRAY_RANGE(H8500_PAGES)},
    {H8_520, TEXT_CPU_H8520, ARRAY_RANGE(H8520_PAGES)},
    {H8_532, TEXT_CPU_H8532, ARRAY_RANGE(H8532_PAGES)},
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
        // Use exact match when base opcode has non-zero bits in the masked field
        // (LINK/UNLK have fixed-register operands, not range-encoded).
        if (mask && (entry->readOpCode() & mask))
            return insn.opCode() == entry->readOpCode();
        return (insn.opCode() & ~mask) == entry->readOpCode();
    }
    // GEN/EXT: disambiguate M_IMM8-src (ANDC/ORC/XORC) from M_REG-src
    // (BCLR/BTST/BNOT) instructions sharing the same base OP byte.
    // EA=0x04 is the IMM8 encoding; M_REG-as-source uses EA as destination.
    if (flags.src() == M_IMM8 && insn.eaByte != 0x04)
        return false;
    if (flags.src() == M_REG && insn.eaByte == 0x04)
        return false;
    return (insn.opByte & ~opMaskOf(flags)) == entry->readOpCode();
}

static bool isEaMode(AddrMode mode) {
    switch (mode) {
    case M_REG:
    case M_IND:
    case M_PREDEC:
    case M_POSTINC:
    case M_DISP8:
    case M_DISP8F:
    case M_DISP16:
    case M_ABS8:
    case M_ABS16:
    case M_IMM8:
    case M_IMM16:
        return true;
    default:
        return false;
    }
}

static bool modeMatch(const AsmInsn &insn, const Entry *e) {
    const auto flags = e->readFlags();
    const auto esrc = flags.src();
    const auto edst = flags.dst();
    const auto &op1 = insn.op1;
    const auto &op2 = insn.op2;
    if (esrc == M_NONE) {
        if (op1.mode != M_NONE)
            return false;
    } else if (esrc == M_EA) {
        if (!isEaMode(op1.mode))
            return false;
    } else if (esrc == M_BIT) {
        if (op1.mode != M_IMM8 && op1.mode != M_BIT)
            return false;
    } else if (esrc == M_IMM2 || esrc == M_TRAPV) {
        // Accept M_IMM8 for quick-increment or trap vector
        if (op1.mode != M_IMM8)
            return false;
    } else if (esrc == M_SCB) {
        // Count register is a general register
        if (op1.mode != M_REG)
            return false;
    } else if (esrc == M_PCREL8 || esrc == M_PCREL16) {
        if (op1.mode != M_PCREL8 && op1.mode != M_PCREL16)
            return false;
    } else if (esrc != op1.mode) {
        return false;
    }
    if (edst == M_NONE) {
        if (op2.mode != M_NONE)
            return false;
    } else if (edst == M_EA) {
        if (!isEaMode(op2.mode))
            return false;
    } else if (edst == M_PCREL8 || edst == M_PCREL16) {
        if (op2.mode != M_PCREL8 && op2.mode != M_PCREL16)
            return false;
    } else if (edst != op2.mode) {
        return false;
    }
    return true;
}

static bool acceptModes(AsmInsn &insn, const Entry *entry) {
    return modeMatch(insn, entry);
}

// Search all pages for the CPU to disassemble a GEN/EXT instruction.
// insn.insnFmt and insn.opCode() must be set before calling.
Error searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) {
    cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    return insn.getError();
}

// Search all pages for the CPU to assemble an instruction by name.
Error searchName(CpuType cpuType, AsmInsn &insn) {
    cpu(cpuType)->searchName(insn, acceptModes);
    return insn.getError();
}

// TableH8500 implementation

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

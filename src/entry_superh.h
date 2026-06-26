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

#ifndef __LIBASM_ENTRY_SUPERH_H__
#define __LIBASM_ENTRY_SUPERH_H__

#include <stdint.h>

#include "config_superh.h"
#include "entry_base.h"

namespace libasm {
namespace superh {

// SH-1/SH-2 addressing modes with their instruction-word bit positions.
// Each mode determines which bits in the 16-bit instruction word are variable;
// opcodeMask() ORs these together so the table search can mask them out.
//
// The GBR-relative @(disp,GBR) and PC-relative @(disp,PC) operands use a single
// mode each (M_D8, M_PCREL); the access width (and thus the displacement scale)
// comes from the InsnSize attribute, not from a per-width mode.
// Modes are ordered so that all "no variable bits" modes come first (M_NONE..
// M_PULL) and every mode that masks instruction-word bits is contiguous from
// M_RN onward, grouped by which bits it varies. modeMask() exploits this: a
// mode below M_RN has no mask, otherwise a small table indexed by (mode - M_RN)
// selects the variation. M_NONE must stay 0 (used as the empty-operand sentinel).
enum AddrMode : uint8_t {
    M_NONE = 0,    // no operand
    // No variable bits: implicit operands, control/system/DSP/FPU registers.
    M_R0 = 1,      // R0 implied
    M_SR = 2,      // SR control register
    M_GBR = 3,     // GBR control register
    M_VBR = 4,     // VBR control register
    M_MACH = 5,    // MACH system register
    M_MACL = 6,    // MACL system register
    M_PR = 7,      // PR system register
    M_IGBR = 8,    // @(R0,GBR) implied (bitwise .B forms)
    M_DSR = 9,     // DSR DSP status register
    M_A0 = 10,     // A0 DSP accumulator low
    M_X0 = 11,     // X0 DSP data register
    M_X1 = 12,     // X1 DSP data register
    M_Y0 = 13,     // Y0 DSP data register
    M_Y1 = 14,     // Y1 DSP data register
    M_MOD = 15,    // MOD DSP modulo register
    M_RS = 16,     // RS DSP repeat-start register
    M_RE = 17,     // RE DSP repeat-end register
    M_FPUL = 18,   // FPUL FPU communication register
    M_FPSCR = 19,  // FPSCR FPU status/control register
    M_BANK = 20,   // implicit R0 destination for STBANK/LDBANK
    M_PUSH = 21,   // implicit @-R15 (MOVML.L/MOVMU.L save)
    M_PULL = 22,   // implicit @R15+ (MOVML.L/MOVMU.L restore)
    // --- masked modes start here (M_RN); keep contiguous, grouped by mask ---
    // V_N: n-field bits[11:8], 0x0F00
    M_RN = 23,     // Rn
    M_IRN = 24,    // @Rn
    M_INCN = 25,   // @Rn+
    M_DECN = 26,   // @-Rn
    M_IDXN = 27,   // @(R0,Rn)
    M_FRN = 28,    // FRn
    M_D12N = 29,   // @(disp12,Rn), disp in 2nd word
    // V_M: m-field bits[7:4], 0x00F0
    M_RM = 30,     // Rm
    M_IRM = 31,    // @Rm
    M_INCM = 32,   // @Rm+
    M_IDXM = 33,   // @(R0,Rm)
    M_FRM = 34,    // FRm
    M_D12M = 35,   // @(disp12,Rm), disp in 2nd word
    M_IMM20 = 36,  // 20-bit signed imm, hi nibble in bits[7:4], lo16 in 2nd word
    M_IMM20S = 37, // 20-bit signed imm shifted-left 8 (MOVI20S)
    // V_D8: 8-bit field bits[7:0], 0x00FF
    M_D4M = 38,    // @(d4,Rm), m=bits[7:4], d=bits[3:0] -> 0x00FF
    M_D8 = 39,     // @(disp,GBR), d=bits[7:0]; scale from size
    M_PCREL = 40,  // @(disp,PC),  d=bits[7:0]; scale from size
    M_IMM8 = 41,   // #imm8 sign-extended, bits[7:0]
    M_REL8 = 42,   // 8-bit signed PC-rel branch, bits[7:0]
    M_REL8P = 43,  // 8-bit signed PC-rel as "@(*+N,PC)"
    M_TVEC = 44,   // TRAPA vector, bits[7:0]
    // singleton variations
    M_D4N = 45,    // @(d4,Rn), n=bits[11:8],d=bits[3:0] -> 0x0F0F
    M_REL12 = 46,  // 12-bit signed PC-rel branch, bits[11:0] -> 0x0FFF
    M_IMM3 = 47,   // 3-bit unsigned imm, bits[3:1] -> 0x000E
    M_DRN = 48,    // DRn (even FRn pair), bits[11:9] -> 0x0E00
    M_DRM = 49,    // DRm (even FRm pair), bits[7:5]  -> 0x00E0
};

// Integer access size from the mnemonic suffix (.B/.W/.L). ISZ_BYTE/WORD/LONG
// are fixed single-size entries (the suffix is appended on disassembly and
// required, or defaulted, on assembly). ISZ_DATA marks an entry whose size is
// encoded in the opcode itself: sizeFieldOf(src,dst) says which bits hold it
// and how field values map to sizes. The base opcode stored in the table holds
// the *default* size, so a bare (no-suffix) mnemonic assembles to it. Float
// precision suffixes (FMOV.S) stay part of the mnemonic name, not encoded here.
enum InsnSize : uint8_t {
    ISZ_NONE = 0,  // no size suffix (ADD, register FMOV, ...)
    ISZ_BYTE = 1,  // fixed .B
    ISZ_WORD = 2,  // fixed .W
    ISZ_LONG = 3,  // fixed .L
    ISZ_DATA = 4,  // size embedded in opcode; field from (src,dst) pair
};

// Describes the opcode bits that hold the access size of an ISZ_DATA entry.
// mask==0 marks "no embedded size" (a defensive default). sizes[] maps the
// right-justified field value to an InsnSize; ISZ_NONE marks an unused or
// illegal slot. The mapping is per (src,dst) pair so it absorbs the SH quirks:
// PC-relative starts at .W (no .B), and MAC runs .L/.W (bit set means .W).
struct SizeField {
    Config::opcode_t mask;
    uint8_t shift;
    InsnSize sizes[4];
};

// B/W/L in opcode bits[1:0] (MOV register-indirect family).
inline SizeField sizeFieldOf(AddrMode src, AddrMode dst) {
    constexpr SizeField NONE_FIELD = {0, 0, {ISZ_NONE, ISZ_NONE, ISZ_NONE, ISZ_NONE}};
    constexpr InsnSize BWL[4] = {ISZ_BYTE, ISZ_WORD, ISZ_LONG, ISZ_NONE};
    constexpr InsnSize BW[4] = {ISZ_BYTE, ISZ_WORD, ISZ_NONE, ISZ_NONE};
    switch (src) {
    case M_RM:
        if (dst == M_IDXN || dst == M_IRN || dst == M_DECN)
            return {0x0003, 0, {BWL[0], BWL[1], BWL[2], BWL[3]}};
        if (dst == M_RN)  // SWAP/EXTS/EXTU, bit[0]
            return {0x0001, 0, {BW[0], BW[1], BW[2], BW[3]}};
        break;
    case M_IDXM:
    case M_IRM:
    case M_INCM:
        if (dst == M_RN)
            return {0x0003, 0, {BWL[0], BWL[1], BWL[2], BWL[3]}};
        break;
    case M_R0:
        if (dst == M_D8)  // MOV.{B,W,L} R0,@(disp,GBR), bits[9:8]
            return {0x0300, 8, {BWL[0], BWL[1], BWL[2], BWL[3]}};
        if (dst == M_D4M)  // MOV.{B,W} R0,@(d4,Rn), bit[8]
            return {0x0100, 8, {BW[0], BW[1], BW[2], BW[3]}};
        break;
    case M_D8:
        if (dst == M_R0)
            return {0x0300, 8, {BWL[0], BWL[1], BWL[2], BWL[3]}};
        break;
    case M_D4M:
        if (dst == M_R0)
            return {0x0100, 8, {BW[0], BW[1], BW[2], BW[3]}};
        break;
    case M_RN:
        if (dst == M_NONE)  // CLIPS/CLIPU, bit[2]
            return {0x0004, 2, {BW[0], BW[1], BW[2], BW[3]}};
        break;
    case M_PCREL:
        if (dst == M_RN)  // MOV.W/MOV.L @(disp,PC),Rn, bit[14], base .W
            return {0x4000, 14, {ISZ_WORD, ISZ_LONG, ISZ_NONE, ISZ_NONE}};
        break;
    default:
        break;
    }
    return NONE_FIELD;
}

// Decode the access size carried in an ISZ_DATA opcode.
inline InsnSize decodeDataSize(AddrMode src, AddrMode dst, Config::opcode_t opc) {
    const auto sf = sizeFieldOf(src, dst);
    return sf.sizes[(opc & sf.mask) >> sf.shift];
}

// Operand modes whose data lives in a second 16-bit word (SH-2A 32-bit forms).
// Their presence is what makes an entry "long form", so no flag bit is needed.
inline bool isLongMode(AddrMode mode) {
    return mode == M_IMM20 || mode == M_IMM20S || mode == M_D12N || mode == M_D12M;
}

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        // _attr bit layout: [14:12]=InsnSize, [11:6]=src(6), [5:0]=dst(6).
        // Long-form (SH-2A 32-bit) is inferred from the operand modes, so no
        // bit is stored. bp_=bit position of each field, gm_=its group mask.
        static constexpr int bp_dst = 0;
        static constexpr int bp_src = 6;
        static constexpr int bp_size = 12;
        static constexpr uint16_t gm_dst = uint16_t(0x3F) << bp_dst;   // [5:0]
        static constexpr uint16_t gm_src = uint16_t(0x3F) << bp_src;   // [11:6]
        static constexpr uint16_t gm_size = uint16_t(0x07) << bp_size; // [14:12]

        uint16_t _attr;

        static constexpr Flags create(InsnSize isz, AddrMode src, AddrMode dst) {
            return Flags{static_cast<uint16_t>(
                    ((static_cast<uint16_t>(isz) << bp_size) & gm_size) |
                    ((static_cast<uint16_t>(src) << bp_src) & gm_src) |
                    ((static_cast<uint16_t>(dst) << bp_dst) & gm_dst))};
        }

        InsnSize insnSize() const { return InsnSize((_attr & gm_size) >> bp_size); }
        AddrMode src() const { return AddrMode((_attr & gm_src) >> bp_src); }
        AddrMode dst() const { return AddrMode((_attr & gm_dst) >> bp_dst); }
        bool longForm() const { return isLongMode(src()) || isLongMode(dst()); }

        // Returns the OR of all variable bits in the 16-bit instruction word,
        // including the embedded size field for ISZ_DATA entries.
        Config::opcode_t opcodeMask() const {
            auto m = modeMask(src()) | modeMask(dst());
            if (insnSize() == ISZ_DATA)
                m |= sizeFieldOf(src(), dst()).mask;
            return m;
        }

    private:
        // Double-indexed lookup over the contiguous masked-mode range [M_RN..]:
        // each mode selects a bit-mask variation, and the variation maps to the
        // actual instruction-word mask. Modes below M_RN have no variable bits.
        // Function-local PROGMEM tables keep the header ODR-clean.
        static Config::opcode_t modeMask(AddrMode mode) {
            if (mode < M_RN)
                return 0;
            enum : uint8_t {
                V_N,     // 0x0F00  n-field bits[11:8]
                V_M,     // 0x00F0  m-field bits[7:4]
                V_D8,    // 0x00FF  8-bit disp/imm/rel
                V_D4N,   // 0x0F0F  n-field + 4-bit disp
                V_REL12, // 0x0FFF  12-bit rel
                V_IMM3,  // 0x000E  bits[3:1]
                V_DRN,   // 0x0E00  bits[11:9]
                V_DRM,   // 0x00E0  bits[7:5]
            };
            static constexpr uint8_t MODE_VAR[] PROGMEM = {  // indexed by mode - M_RN
                V_N, V_N, V_N, V_N, V_N, V_N, V_N,            // M_RN M_IRN M_INCN M_DECN M_IDXN M_FRN M_D12N
                V_M, V_M, V_M, V_M, V_M, V_M, V_M, V_M,       // M_RM M_IRM M_INCM M_IDXM M_FRM M_D12M M_IMM20 M_IMM20S
                V_D8, V_D8, V_D8, V_D8, V_D8, V_D8, V_D8,     // M_D4M M_D8 M_PCREL M_IMM8 M_REL8 M_REL8P M_TVEC
                V_D4N,                                        // M_D4N
                V_REL12,                                      // M_REL12
                V_IMM3,                                       // M_IMM3
                V_DRN,                                        // M_DRN
                V_DRM,                                        // M_DRM
            };
            static constexpr Config::opcode_t VAR_MASK[] PROGMEM = {
                0x0F00, 0x00F0, 0x00FF, 0x0F0F, 0x0FFF, 0x000E, 0x0E00, 0x00E0,
            };
            return pgm_read_word(&VAR_MASK[pgm_read_byte(&MODE_VAR[mode - M_RN])]);
        }
    };

    constexpr Entry(Config::opcode_t opc, Flags flags, const /* PROGMEM */ char *name_P)
        : Base(name_P, opc), _flags_P(flags) {}

    Flags readFlags() const { return Flags{pgm_read_word(&_flags_P._attr)}; }

private:
    const Flags _flags_P;
};

}  // namespace superh
}  // namespace libasm

#endif  // __LIBASM_ENTRY_SUPERH_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

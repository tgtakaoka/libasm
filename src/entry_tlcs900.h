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

#ifndef __LIBASM_ENTRY_TLCS900_H__
#define __LIBASM_ENTRY_TLCS900_H__

#include <stdint.h>

#include "config_tlcs900.h"
#include "entry_base.h"
#include "reg_tlcs900.h"

namespace libasm {
namespace tlcs900 {

// clang-format off
// AddrMode is ordered so the Flags fields stay narrow (see Entry::Flags):
//   0-18  : usable in the SRC (2nd) operand -> the 5-bit src field.
//   19-39 : only ever the DST (1st) operand -> the 6-bit dst field.
//   40+   : never stored in a Flags field (parser results / disassembler /
//           resolveMode targets); value is unconstrained.
enum AddrMode : uint8_t {
    // -- SRC-usable (0-18); most are usable as DST too --
    M_NONE  = 0,   // no operand
    M_REG8  = 1,   // 8-bit current bank register (W,A,B,C,D,E,H,L)
    M_REG16 = 2,   // 16-bit current bank register (WA,BC,DE,HL,IX,IY,IZ,SP)
    M_REG32 = 3,   // 32-bit current bank register (XWA,...,XSP)
    M_ABS16 = 4,   // (nn) 16-bit absolute address
    M_ABS24 = 5,   // (nnn) 24-bit absolute address
    M_IMM8  = 6,   // 8-bit immediate
    M_IMM16 = 7,   // 16-bit immediate
    M_IMM32 = 8,   // 32-bit immediate
    M_IMMX  = 9,   // immediate of context-matching size (8/16/32-bit)
    M_REL8  = 10,  // 8-bit PC-relative displacement
    M_REL16 = 11,  // 16-bit PC-relative displacement
    M_SRC   = 12,  // operand is the prefix-encoded source
    M_DST   = 13,  // operand is the prefix-encoded destination
    M_CREG  = 14,  // DMA/NSP control register (RegName in op.reg)
    M_LDARREL = 15,  // LDAR rel16 target with base = instruction_start + 4
    R_FP    = 16,  // F' (alternate flag register, EX F,F' only)
    M_RC1   = 17,  // implicit rotate/shift count 1: matches M_NONE; emits 0x01
    M_BLKSRC = 18,  // block source pointer (XHL/XIY+/-): prefix low nibble, dir from opcode
    // -- DST-only (19-40); asm-only matcher modes at the top --
    M_ABS8  = 19,  // (n) 8-bit absolute address
    M_BIT   = 20,  // bit number 0-7 (in following byte)
    M_CC    = 21,  // condition code CC (in opcode low nibble)
    M_STEP  = 22,  // INC/DEC step 1-8 in opcode low 3 bits (8 encoded as 0)
    M_LDF   = 23,  // LDF RFP bank 0-3 (in following byte, low 2 bits)
    M_SWI   = 24,  // SWI vector 0-7 (in opcode low 3 bits)
    M_RCOUNT = 25,  // rotate/shift count 1-16 (in following byte, 16 encoded as 0)
    M_BUF   = 26,  // MINC/MDEC buffer size (power of 2, following word)
    R_A     = 27,  // register A (implicit; byte block-compare accumulator)
    R_SR    = 28,  // status register SR
    R_F     = 29,  // flag byte F (implicit, PUSH F / POP F)
    R_WA    = 30,  // register WA (implicit; word block-compare accumulator, MUL result)
    M_LDXDST = 31,  // LDX destination: 0x00 sub-byte, then 16-bit absolute address
    M_ABSDST = 32,  // absolute bank register in dst, resolves to M_ABREG{8,16,32} by PM
    M_INTLVL = 33,  // EI interrupt level 0-7 (in following byte, low 3 bits)
    M_MULDST = 34,  // MUL/DIV dest in opcode (0x40+R): double the prefix(src) size
    M_MULDSTP = 35,  // MUL/DIV rr,# dest carried IN the prefix (C8+code / D8+code)
    M_R32SRC = 36,  // 32-bit register passed as PM_REG16 prefix (MULA xrr32 alias)
    M_BLKDST = 37,  // block dest pointer (XDE/XIX+/-): src pointer - 1, dir from opcode
    // asm-only matcher modes (never emitted by the disassembler):
    M_CCT   = 38,  // always-true condition (cc=T): folds JP/CALL T,(abs) (asm-only)
    M_ZERO  = 39,  // implicit zero: matches M_NONE in asm; emits 0x00 trailing byte
    M_DISUF = 40,  // DI suffix: matches M_NONE in asm; emits 0x07 trailing byte
    // -- Never a Flags field: parser results / disassembler / resolveMode --
    M_IND   = 41,  // (xrr) indirect via 32-bit register
    M_PDEC  = 42,  // (-xrr) pre-decrement
    M_PINC  = 43,  // (xrr+) post-increment
    M_IDX8  = 44,  // (xrr+d8) 8-bit displacement indexed
    M_IDX16 = 45,  // (xrr+d16) 16-bit displacement indexed
    M_IDXR  = 46,  // (xrr+r) register indexed (r is an 8-bit register)
    M_ABREG8  = 47,  // 8-bit absolute bank register (register code in op.val)
    M_ABREG16 = 48,  // 16-bit absolute bank register
    M_ABREG32 = 49,  // 32-bit absolute bank register
    M_LDARREG = 50,  // LDAR destination register (resolved form)
    R_C     = 51,  // 'C' dual-use: matches M_REG8 (reg C) or M_CC (carry)
    M_BPDEC = 52,  // (xrr-) block post-decrement; parser result, block ops only
};
// clang-format on

enum CodeFormat : uint8_t {
    CF_00 = 0,   // exact opcode match (mask 0x00)
    CF_07 = 1,   // low 3 bits variable (mask 0x07)
    CF_0F = 2,   // low 4 bits variable (mask 0x0F)
    CF_07L = 3,  // bits[3] + bits[2:0]: mask 0x0F but grouped as 0x08|reg
};

// Set of operation sizes an instruction permits. The operand's actual size
// (inferred from the register name or the prefix mode) is a one-hot member;
// intersecting it with the entry's mask decides legality (empty -> illegal),
// and the intersection is the operation size. SM_ANY (the E0/E1/E2 default)
// means unrestricted.
enum SizeMask : uint8_t {
    SM_ANY = 0,  // no restriction (treated as B|W|L)
    SM_B = 1,    // byte only
    SM_W = 2,    // word only
    SM_L = 4,    // long only
    SM_BW = 3,   // byte or word (no long)
    SM_WL = 6,   // word or long (no byte)
    SM_BWL = 7,  // all three
};

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint16_t _attr;

        static constexpr Flags create(CodeFormat cf, AddrMode dst, AddrMode src, SizeMask sz) {
            return Flags{static_cast<uint16_t>((static_cast<uint16_t>(dst) << dst_gp) |
                                               (static_cast<uint16_t>(src) << src_gp) |
                                               (static_cast<uint16_t>(cf) << cf_gp) |
                                               (static_cast<uint16_t>(sz) << sz_gp))};
        }

        AddrMode dst() const { return AddrMode((_attr >> dst_gp) & dst_gm); }
        AddrMode src() const { return AddrMode((_attr >> src_gp) & src_gm); }
        CodeFormat format() const { return CodeFormat((_attr >> cf_gp) & cf_gm); }
        SizeMask sizes() const {
            const auto sz = SizeMask((_attr >> sz_gp) & sz_gm);
            return sz == SM_ANY ? SM_BWL : sz;
        }
        uint8_t mask() const {
            static constexpr uint8_t MASK[] PROGMEM = {
                    0x00,  // CF_00
                    0x07,  // CF_07
                    0x0F,  // CF_0F
                    0x0F,  // CF_07L (0x0F but low nibble split as 8|reg)
            };
            return pgm_read_byte(&MASK[format()]);
        }

    private:
        // dst 6 bits, src 5 bits (only 17 modes ever appear there), cf 2 bits,
        // size 3 bits = 16 bits total.
        static constexpr int dst_gp = 0;
        static constexpr int src_gp = 6;
        static constexpr int cf_gp = 11;
        static constexpr int sz_gp = 13;
        static constexpr uint_fast8_t dst_gm = 0x3F;
        static constexpr uint_fast8_t src_gm = 0x1F;
        static constexpr uint_fast8_t cf_gm = 0x3;
        static constexpr uint_fast8_t sz_gm = 0x7;
    };

    constexpr Entry(Config::opcode_t opc, Flags flags, const /* PROGMEM */ char *name_P)
        : Base(name_P, opc), _flags_P(flags) {}

    Flags readFlags() const { return Flags{pgm_read_word(&_flags_P._attr)}; }

private:
    const Flags _flags_P;
};

}  // namespace tlcs900
}  // namespace libasm

#endif  // __LIBASM_ENTRY_TLCS900_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

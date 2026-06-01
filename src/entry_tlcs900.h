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
enum AddrMode : uint8_t {
    M_NONE  = 0,
    // Register operands (actual register from prefix byte or opcode field)
    M_REG8  = 1,   // 8-bit current bank register (W,A,B,C,D,E,H,L)
    M_REG16 = 2,   // 16-bit current bank register (WA,BC,DE,HL,IX,IY,IZ,SP)
    M_REG32 = 3,   // 32-bit current bank register (XWA,...,XSP)
    // Memory addressing modes (prefix byte determines the memory address)
    M_IND   = 4,   // (xrr) indirect via 32-bit register
    M_PRDC  = 5,   // (-xrr) pre-decrement
    M_PINC  = 6,   // (xrr+) post-increment
    M_IDX8  = 7,   // (xrr+d8) 8-bit displacement indexed
    M_IDX16 = 8,   // (xrr+d16) 16-bit displacement indexed
    M_IDXR  = 9,   // (xrr+r) register indexed (r is an 8-bit register)
    M_ABS8  = 10,  // (n) 8-bit absolute address
    M_ABS16 = 11,  // (nn) 16-bit absolute address
    M_ABS24 = 12,  // (nnn) 24-bit absolute address
    // Immediate / literal
    M_IMM8  = 13,  // 8-bit immediate
    M_IMM16 = 14,  // 16-bit immediate
    M_IMM32 = 15,  // 32-bit immediate
    // Encoded in opcode byte
    M_BIT   = 16,  // bit number 0-7 (in following byte)
    M_CC    = 17,  // condition code CC (in opcode low nibble)
    M_REL8  = 18,  // 8-bit PC-relative displacement
    M_REL16 = 19,  // 16-bit PC-relative displacement
    M_STEP1 = 20,  // INC/DEC step=1 (opcode encodes this)
    M_STEP2 = 21,  // INC/DEC step=2
    M_STEP4 = 22,  // INC/DEC step=4
    M_LDF   = 23,  // LDF RFP bank 0-3 (in following byte, low 2 bits)
    M_SWI   = 24,  // SWI vector 0-7 (in opcode low 3 bits)
    M_RCOUNT = 25, // rotate/shift count (in following byte)
    M_BUF   = 26,  // MINC/MDEC buffer size (power of 2, following word)
    // Placeholder: resolved from prefix context
    M_SRC   = 27,  // operand is the prefix-encoded source
    M_DST   = 28,  // operand is the prefix-encoded destination
    // Implicit / hard-coded registers
    R_A     = 29,  // register A (implicit)
    R_WA    = 30,  // register WA (implicit, e.g. MUL result)
    R_SR    = 31,  // status register SR
    R_F     = 32,  // flag byte F (implicit, PUSH F / POP F)
    R_FP    = 33,  // F' (alternate flag register, EX F,F' only)
    R_C     = 34,  // 'C' dual-use: matches M_REG8 (reg C) or M_CC (carry)
    M_IMMX    = 35,  // immediate of context-matching size (8/16/32-bit)
    M_ABREG8  = 36,  // 8-bit absolute bank register (register code in op.val)
    M_ABREG16 = 37,  // 16-bit absolute bank register
    M_ABREG32 = 38,  // 32-bit absolute bank register
    M_CREG    = 39,  // DMA/NSP control register (RegName in op.reg)
    M_LDXDST  = 40,  // LDX destination: emits/expects 0x00 sub-byte, then 16-bit absolute address
    M_ABSDST  = 41,  // absolute bank register in dst (prefix-encoded), resolves to M_ABREG{8,16,32} by PM
    M_ZERO    = 42,  // implicit zero operand: matches M_NONE in asm; emits 0x00 trailing byte
    M_INTLVL  = 43,  // EI interrupt level 0-7 (in following byte, low 3 bits)
};
// clang-format on

enum CodeFormat : uint8_t {
    CF_00 = 0,   // exact opcode match (mask 0x00)
    CF_07 = 1,   // low 3 bits variable (mask 0x07)
    CF_0F = 2,   // low 4 bits variable (mask 0x0F)
    CF_07L = 3,  // bits[3] + bits[2:0]: mask 0x0F but grouped as 0x08|reg
};

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint16_t _attr;

        static constexpr Flags create(CodeFormat cf, AddrMode dst, AddrMode src) {
            return Flags{static_cast<uint16_t>((dst << dst_gp) | (src << src_gp) | (cf << cf_gp))};
        }

        AddrMode dst() const { return AddrMode((_attr >> dst_gp) & dst_gm); }
        AddrMode src() const { return AddrMode((_attr >> src_gp) & src_gm); }
        CodeFormat format() const { return CodeFormat((_attr >> cf_gp) & cf_gm); }
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
        static constexpr int dst_gp = 0;
        static constexpr int src_gp = 6;
        static constexpr int cf_gp = 12;
        static constexpr uint_fast8_t dst_gm = 0x3F;
        static constexpr uint_fast8_t src_gm = 0x3F;
        static constexpr uint_fast8_t cf_gm = 0x3;
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

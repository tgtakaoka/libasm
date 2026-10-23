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

#ifndef __LIBASM_ENTRY_C166_H__
#define __LIBASM_ENTRY_C166_H__

#include <stdint.h>

#include "config_c166.h"
#include "entry_base.h"

namespace libasm {
namespace c166 {

// Every instruction is one or two words.  The first word is the opcode byte
// and its operand byte; operand fields are named by where they sit.
enum AddrMode : uint8_t {
    M_NONE = 0,
    // Operands in the first word.
    M_RWH = 1,    // Rw in the high nibble
    M_RWL = 2,    // Rw in the low nibble
    M_RBH = 3,    // Rb in the high nibble
    M_RBL = 4,    // Rb in the low nibble
    M_IRH = 5,    // [Rw] in the high nibble
    M_IRL = 6,    // [Rw] in the low nibble
    M_PIH = 7,    // [Rw+] in the high nibble
    M_PIL = 8,    // [Rw+] in the low nibble
    M_PDL = 9,    // [-Rw] in the low nibble
    M_IR2 = 10,   // [Rw] in bits 1:0, R0-R3 only
    M_PI2 = 11,   // [Rw+] in bits 1:0, R0-R3 only
    M_D3 = 12,    // #data3 in bits 2:0
    M_D4 = 13,    // #data4 in the high nibble
    M_TRAP = 14,  // #trap7 in bits 7:1
    M_REGW = 15,  // word reg
    M_REGB = 16,  // byte reg
    M_CC = 17,    // cc in the high nibble
    M_CCO = 18,   // cc in the high nibble of the opcode
    M_BITO = 19,  // bitaddr; bitoff, and the bit in the high nibble of the opcode
    M_BOFF = 20,  // bitoff
    M_REL = 21,   // rel
    M_SEG = 22,   // seg
    M_RNG = 23,   // #irang2 in bits 5:4
    M_RNGR = 24,  // #irang2 in bits 5:4, which also starts an ESFR sequence
    // Operands which need the second word.
    M_IXL = 25,   // [Rw+#data16]; Rw in the low nibble
    M_IM16 = 26,  // #data16
    M_IM8 = 27,   // #data8 in the low byte; the high byte is not significant
    M_MEM = 28,   // mem
    M_CADR = 29,  // caddr
    M_BITQ = 30,  // bitaddr; bitoff, and the bit in the high nibble of the 4th byte
    M_BITZ = 31,  // bitaddr; bitoff in the 3rd byte, the bit in the low nibble of the 4th byte
    M_MDL = 32,   // #mask8 in the 3rd byte, #data8 in the 4th byte
    M_MDH = 33,   // #data8 in the 3rd byte, #mask8 in the 4th byte
    M_RELW = 34,  // rel in the 3rd byte; the low nibble of the 4th byte is unused
    M_PAG = 35,   // #pag10
    M_SEG8 = 36,  // #seg8
    // Assembler operands which several of the above accept.
    M_RW = 37,    // Rw
    M_RB = 38,    // Rb
    M_IDIR = 39,  // [Rw]
    M_PINC = 40,  // [Rw+]
    M_IMM = 41,   // #imm
    M_BIT = 42,   // bitaddr
    M_ADDR = 43,  // address
};

constexpr auto M_WORD2 = M_IXL;

// Which bits of the operand byte carry operands; the rest must equal the
// entry's post byte.
enum CodeFormat : uint8_t {
    CF_00 = 0,   // none
    CF_FF = 1,   // all
    CF_F0 = 2,   // high nibble
    CF_0F = 3,   // low nibble
    CF_F3 = 4,   // high nibble and bits 1:0
    CF_F7 = 5,   // high nibble and bits 2:0
    CF_FE = 6,   // bits 7:1
    CF_30 = 7,   // bits 5:4
    CF_3F = 8,   // bits 5:4 and low nibble
    CF_NN = 9,   // the same register in both nibbles
    CF_PD = 10,  // none; the second word repeats the opcode
    CF_OP = 11,  // all, and the high nibble of the opcode
};

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint16_t _attr;

        static constexpr Flags create(CodeFormat cf, AddrMode dst, AddrMode src) {
            return Flags{static_cast<uint16_t>((static_cast<uint16_t>(dst) << dst_gp) |
                                               (static_cast<uint16_t>(src) << src_gp) |
                                               (static_cast<uint16_t>(cf) << cf_gp))};
        }

        AddrMode dst() const { return AddrMode((_attr >> dst_gp) & mode_gm); }
        AddrMode src() const { return AddrMode((_attr >> src_gp) & mode_gm); }
        CodeFormat cf() const { return CodeFormat((_attr >> cf_gp) & cf_gm); }
        uint8_t opCodeMask() const { return cf() == CF_OP ? 0xF0 : 0; }
        uint8_t postMask() const {
            static constexpr uint8_t MASK[] PROGMEM = {
                    0x00,  // CF_00
                    0xFF,  // CF_FF
                    0xF0,  // CF_F0
                    0x0F,  // CF_0F
                    0xF3,  // CF_F3
                    0xF7,  // CF_F7
                    0xFE,  // CF_FE
                    0x30,  // CF_30
                    0x3F,  // CF_3F
                    0xFF,  // CF_NN
                    0x00,  // CF_PD
                    0xFF,  // CF_OP
            };
            return pgm_read_byte(&MASK[cf()]);
        }
        bool word2() const { return dst() >= M_WORD2 || src() >= M_WORD2 || cf() == CF_PD; }

    private:
        static constexpr int dst_gp = 0;
        static constexpr int src_gp = 6;
        static constexpr int cf_gp = 12;
        static constexpr uint_fast8_t mode_gm = 0x3F;
        static constexpr uint_fast8_t cf_gm = 0xF;
    };

    constexpr Entry(Config::opcode_t opc, Config::opcode_t post, Flags flags,
            const /* PROGMEM */ char *name_P)
        : Base(name_P, opc), _flags_P(flags), _post_P(post) {}

    Flags readFlags() const { return Flags{pgm_read_word(&_flags_P._attr)}; }
    Config::opcode_t readPost() const { return pgm_read_byte(&_post_P); }

private:
    const Flags _flags_P;
    const Config::opcode_t _post_P;
};

}  // namespace c166
}  // namespace libasm

#endif  // __LIBASM_ENTRY_C166_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

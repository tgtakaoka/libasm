/*
 * Copyright 2020 Tadashi G. Takaoka
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

#ifndef __LIBASM_ENTRY_TLCS90_H__
#define __LIBASM_ENTRY_TLCS90_H__

#include <stdint.h>

#include "config_tlcs90.h"
#include "entry_base.h"
#include "reg_tlcs90.h"

namespace libasm {
namespace tlcs90 {

enum AddrMode : uint8_t {
    M_NONE = 0,
    M_IMM8 = 1,    // Immediate 8-bit: nn
    M_IMM16 = 2,   // immediate 16-bit: nnnn
    M_BIT = 3,     // Bit number: 0-7
    M_EXT = 4,     // Extended address 16-bit: (nnnn)
    M_DIR = 5,     // Direct address 8-bit: (FFnn)
    M_REL8 = 6,    // Relative 8-bit: nnnn
    M_REL16 = 7,   // Relative 16-bit: nnnn
    M_IND = 8,     // Indirect: (M_REG16)
    M_IDX = 9,     // Indexed: (M_REGIX+nn)
    M_BASE = 10,   // Based indexed: (HL+A)
    M_CC = 11,     // Condition: F/LT/LE/ULE/OV/MI/Z/C/GE/GT/UGT/NOV/PL/NZ/NC
    M_STACK = 12,  // Register on stack: BC/DE/HL/IX/IY/AF
    M_REG8 = 13,   // Register 8-bit: B/C/D/E/H/L/A
    M_REG16 = 14,  // Register 16-bit: BC/DE/HL/IX/IY/SP
    M_REGIX = 15,  // Register index: IX/IY/SP
    M_HLPA = 16,   // HL+A
    M_IXPD = 17,   // ix+disp
    M_SRC16 = 18,  // M_REG16
    M_SRC = 19,
    M_DST = 20,
    M_SYM = 21,  // Undefined symbol: M_EXT/M_DIR
    R_A = 22,    // Check |mode2reg| in dis_tlcs90.cpp
    R_C = 23,    // Check |reg2mode| in asm_tlcs90.cpp
    R_AFP = 24,
    R_BC = 25,
    R_DE = 26,
    R_HL = 27,
    R_SP = 28,
    R_AF = 29,
};

enum CodeFormat : uint8_t {
    CF_00 = 0,  // 0x00
    CF_03 = 1,  // 0x03
    CF_07 = 2,  // 0x07
    CF_0F = 3,  // 0x0F
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
                    0x00,  // CF_00 = 0
                    0x03,  // CF_03 = 1
                    0x07,  // CF_07 = 2
                    0x0F,  // CF_0F = 3
            };
            return pgm_read_byte(&MASK[format()]);
        }

    private:
        static constexpr int dst_gp = 0;
        static constexpr int src_gp = 5;
        static constexpr int cf_gp = 10;
        static constexpr uint_fast8_t dst_gm = 0x1F;
        static constexpr uint_fast8_t src_gm = 0x1F;
        static constexpr uint_fast8_t cf_gm = 0x3;
    };

    constexpr Entry(Config::opcode_t opc, Flags flags, const /* PROGMEM */ char *name_P)
        : Base(name_P, opc), _flags_P(flags) {}

    Flags readFlags() const { return Flags{pgm_read_word(&_flags_P._attr)}; }

private:
    const Flags _flags_P;
};

}  // namespace tlcs90
}  // namespace libasm

#endif  // __LIBASM_ENTRY_TLCS90_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

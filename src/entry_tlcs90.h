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

#ifndef __ENTRY_TLCS90_H__
#define __ENTRY_TLCS90_H__

#include "config_tlcs90.h"
#include "entry_base.h"

namespace libasm {
namespace tlcs90 {

enum AddrMode : uint8_t {
    M_NO = 0,
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
    R_BC = 16 + 0 + 0,   // REG_BC
    R_DE = 16 + 0 + 1,   // REG_DE
    R_HL = 16 + 0 + 2,   // REG_HL
    R_SP = 16 + 4 + 2,   // REG_SP
    R_AF = 16 + 8 + 6,   // REG_AF
    R_AFP = 16 + 8 + 7,  // REG_AFP
    R_C = 16 + 16 + 1,   // REG_C or CC_C
    R_A = 16 + 16 + 6,   // REG_A
    M_SRC16 = 60,        // M_REG16
    M_SRC = 61,
    M_DST = 62,
    M_UNKI = 63,  // Undefined instruction
};

class Entry : public EntryBase<Config> {
public:
    struct Flags {
        uint8_t _dst;
        uint8_t _src;

        static constexpr Flags create(AddrMode dst, AddrMode src, bool emit) {
            return Flags{static_cast<uint8_t>(
                                 uint8_t(dst) | (emit ? (1 << emit_bp) : 0)),
                    static_cast<uint8_t>(src)};
        }
        Flags read() const {
            return Flags{pgm_read_byte(&_dst), pgm_read_byte(&_src)};
        }

        AddrMode dstMode() const { return AddrMode(_dst & mode_gm); }
        AddrMode srcMode() const { return AddrMode(_src); }
        bool emit() const { return _dst & (1 << emit_bp); }
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const char *name)
        : EntryBase(name, opCode), _flags(flags) {}

    Flags flags() const { return _flags.read(); }

private:
    Flags _flags;

    static constexpr uint8_t mode_gm = 0x3f;
    static constexpr int8_t emit_bp = 7;
};

}  // namespace tlcs90
}  // namespace libasm

#endif  // __ENTRY_TLCS90_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

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

#ifndef __ENTRY_I8051_H__
#define __ENTRY_I8051_H__

#include "config_i8051.h"

namespace libasm {
namespace i8051 {

enum AddrMode : uint8_t {
    NONE  = 0,
    REL   = 1,  // Relative
    AREG  = 2,  // A register
    RREG  = 3,  // Rn register
    CREG  = 4,  // C register
    DREG  = 5,  // DPTR register
    ABREG = 6,  // AB register
    ADR8  = 7,  // Internal 8-bit RAM address
    ADR11 = 8,  // 11-bit program code address
    ADR16 = 9,  // 16-bot program code address
    BITAD = 10, // 8-bit bit address
    NOTAD = 11, // not(/) 8-bit address
    IMM8  = 12, // 8-bit Immediate data
    IMM16 = 13, // 16-bit Immediate data
    IDIRR = 14, // @Rn indirect
    IDIRD = 15, // @DPTR indirect
    INDXD = 16, // @A+DPTR
    INDXP = 17, // @A+PC
};

struct Entry {
    const Config::opcode_t opCode;
    const uint16_t flags;
    const char *name;

    static inline AddrMode _dstMode(uint16_t flags) {
        return AddrMode((flags >> dstMode_gp) & addrMode_gm);
    }
    static inline AddrMode _srcMode(uint16_t flags) {
        return AddrMode((flags >> srcMode_gp) & addrMode_gm);
    }
    static inline AddrMode _extMode(uint16_t flags) {
        return AddrMode((flags >> extMode_gp) & addrMode_gm);
    }
    static constexpr uint16_t _flags(AddrMode dst, AddrMode src, AddrMode ext) {
        return (static_cast<uint16_t>(dst) << dstMode_gp)
            | (static_cast<uint16_t>(src) << srcMode_gp)
            | (static_cast<uint16_t>(ext) << extMode_gp);
    }

private:
    static constexpr uint8_t addrMode_gm = 0x1f;
    static constexpr uint8_t dstMode_gp = 0;
    static constexpr uint8_t srcMode_gp = 5;
    static constexpr uint8_t extMode_gp = 10;
};

} // namespace i8051
} // namespace libasm

#endif // __ENTRY_I8051_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

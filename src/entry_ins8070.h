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

#ifndef __ENTRY_INS8070_H__
#define __ENTRY_INS8070_H__

#include "config_ins8070.h"

namespace libasm {
namespace ins8070 {

enum OprSize : host::uint_t {
    SZ_NONE,                    // unknown
    SZ_BYTE = 1,
    SZ_WORD = 2,
};

enum AddrMode : host::uint_t {
    IMPLIED,
    GENERIC,
    ABSOLUTE,
    RELATIVE,
    IMMEDIATE,
    UNDEF, // Undefined instruction
};

enum OprFormat : host::uint_t {
    OPR_NO,
    OPR_A,   // A register
    OPR_E,   // E register
    OPR_S,   // S register
    OPR_EA,  // EA register
    OPR_IX,  // P2/P3 register
    OPR_SP,  // SP register
    OPR_PN,  // PC/SP/P2/P3 register
    OPR_T,   // T register
    OPR_4,   // 4 bit constant
    OPR_IM,  // Immediate constant
    OPR_16,  // 16 bit constant
    OPR_GN,  // nn/nn,PC/nn,SP/nn,Pn/@nn,Pn/nn,@Pn
};

struct Entry {
    const Config::opcode_t opCode;
    const uint16_t flags;
    const char *name;

    static inline AddrMode _addrMode(uint16_t flags) {
        return AddrMode(flags & addrMode_gm);
    }
    static inline OprFormat _leftOpr(uint16_t flags) {
        return OprFormat((flags >> leftOpr_gp) & oprFormat_gm);
    }
    static inline OprFormat _rightOpr(uint16_t flags) {
        return OprFormat((flags >> rightOpr_gp) & oprFormat_gm);
    }
    static inline OprSize _oprSize(uint16_t flags) {
        return OprSize((flags >> oprSize_gp) & oprSize_gm);
    }
    static constexpr uint16_t _flags(
        AddrMode addrMode, OprFormat left, OprFormat right, OprSize size) {
        return uint16_t(addrMode)
            | (uint16_t(left) << leftOpr_gp)
            | (uint16_t(right) << rightOpr_gp)
            | (uint16_t(size) << oprSize_gp);
    }
    static uint16_t _set(uint16_t flags, AddrMode addrMode) {
        return (flags & ~addrMode_gm) | uint16_t(addrMode);
    }

private:
    static constexpr host::uint_t addrMode_gm = 0x07;
    static constexpr host::uint_t oprFormat_gm = 0x0F;
    static constexpr host::uint_t oprSize_gm = 0x3;
    static constexpr host::uint_t leftOpr_gp = 8;
    static constexpr host::uint_t rightOpr_gp = 12;
    static constexpr host::uint_t oprSize_gp = 4;
};

} // namespace ins8070
} // namespace libasm

#endif // __ENTRY_INS8070_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

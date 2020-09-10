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

enum OprSize : uint8_t {
    SZ_NONE = 0,  // unknown
    SZ_BYTE = 1,
    SZ_WORD = 2,
};

enum AddrMode : uint8_t {
    IMPLIED   = 1,  // Implied
    GENERIC   = 2,  // Generic; nnnn,PC/nn,SP/nn,Pn/@nn,Pn/0xFFnn/
    ABSOLUTE  = 3,  // Absolute; nnnn
    RELATIVE  = 4,  // Relative; nnnn

    IMMEDIATE = 5,  // Immediate; #nn
    UNDEF     = 0,  // Undefined instruction
};

enum OprFormat : uint8_t {
    OPR_NO = 0,
    OPR_A  = 1,  // Accumulator: A
    OPR_E  = 2,  // Extension register: E
    OPR_EA = 3,  // Double precision Accumulator: EA
    OPR_T  = 4,  // Temporary register: T
    OPR_S  = 5,  // Status register: S
    OPR_PN = 6,  // Pointer Register: P2/P3
    OPR_SP = 7,  // SP register
    OPR_BR = 8,  // Base Register: PC/SP/P2/P3
    OPR_IM = 9,  // Immediate constant
    OPR_16 = 10, // 16 bit constant
    OPR_4  = 11, // 4 bit constant
    OPR_PR = 12, // Pointer relative: nn,Pn
    OPR_RL = 13, // PC Relative
    OPR_GN = 14, // nnnn/nnnn,PC/nn,SP/nn,Pn/@nn,Pn/nn,@Pn/0xFFnn
};

struct Entry {
    const Config::opcode_t opCode;
    const uint16_t flags;
    const char *name;

    static inline AddrMode _addrMode(uint16_t flags) {
        return AddrMode(flags & addrMode_gm);
    }
    static inline OprFormat _dstOpr(uint16_t flags) {
        return OprFormat((flags >> dstOpr_gp) & oprFormat_gm);
    }
    static inline OprFormat _srcOpr(uint16_t flags) {
        return OprFormat((flags >> srcOpr_gp) & oprFormat_gm);
    }
    static inline OprSize _oprSize(uint16_t flags) {
        return OprSize((flags >> oprSize_gp) & oprSize_gm);
    }
    static constexpr uint16_t _flags(
        AddrMode addrMode, OprFormat dst, OprFormat src, OprSize size) {
        return static_cast<uint16_t>(addrMode)
            | (static_cast<uint16_t>(dst) << dstOpr_gp)
            | (static_cast<uint16_t>(src) << srcOpr_gp)
            | (static_cast<uint16_t>(size) << oprSize_gp);
    }

private:
    static constexpr uint8_t addrMode_gm  = 0x7;
    static constexpr uint8_t oprFormat_gm = 0xF;
    static constexpr uint8_t oprSize_gm   = 0x3;
    static constexpr int oprSize_gp = 4;
    static constexpr int dstOpr_gp = 8;
    static constexpr int srcOpr_gp = 12;
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

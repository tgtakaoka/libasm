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

#ifndef __ENTRY_TMS9900_H__
#define __ENTRY_TMS9900_H__

#include "config_tms9900.h"

namespace libasm {
namespace tms9900 {

enum CpuType : uint8_t {
    TMS9900,
    TMS9995,
    TMS99105,
};

enum AddrMode : uint8_t {
    M_NO   = 0,
    M_IMM  = 1,   // 16-bit Immediate data
    M_REG  = 2,   // ---- ---- ---- wwww: Work Register Addressing
    M_SCNT = 3,   // ---- ---- cccc ----
    M_SRC  = 4,   // ---- ---- --SS ssss
    M_CNT  = 5,   // ---- --cc cc-- ----
    M_XOP  = 6,   // ---- --vv vv-- ----
    M_DREG = 7,   // ---- --dd dd-- ----
    M_DST  = 8,   // ---- DDdd dd-- ----
    M_REL  = 10,  // ---- ---- nnnn nnnn
    M_CRU  = 11,  // ---- ---- nnnn nnnn
    M_RTWP = 12,  // ---- ---- ---- -nnn RTWP mode
    M_DST2 = 13,  // 0100 DDdd dd-- ---- in 2nd word
    M_CNT2 = 14,  // 0100 00cc cc-- ---- in 2nd word
    M_BIT2 = 15,  // 0000 00bb bb-- ---- in 2nd word
    M_SRC2 = 16,  // ---- ---- --SS ssss in 2nd word
    //Only for assembler.
    M_IREG = 17,  // Work Register Indirect Addressing: *Rn
    M_INCR = 18,  // Work Register Indirect Auto Increment Addressing: *Rn+
    M_SYBL = 19,  // Symbol (Direct) Addressing: @LABEL
    M_INDX = 20,  // Indexed Addressing: @TABLE(Rn)
};

struct Entry {
    const Config::opcode_t opCode;
    const uint16_t flags;
    const char *name;

    static inline AddrMode _srcMode(uint16_t flags) {
        return AddrMode((flags >> srcMode_gp) & addrMode_gm);
    }

    static inline AddrMode _dstMode(uint16_t flags) {
        return AddrMode((flags >> dstMode_gp) & addrMode_gm);
    }

    static constexpr uint16_t _flags(AddrMode src, AddrMode dst) {
        return (static_cast<uint16_t>(src) << srcMode_gp)
            | (static_cast<uint16_t>(dst) << dstMode_gp);
    }

private:
    static constexpr int srcMode_gp = 0;
    static constexpr int dstMode_gp = 8;
    static constexpr uint8_t addrMode_gm = 0x1F;
};

} // namespace tms9900
} // namespace libasm

#endif // __ENTRY_TMS9900_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

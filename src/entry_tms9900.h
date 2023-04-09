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

#include <stdint.h>

#include "config_tms9900.h"
#include "entry_base.h"

namespace libasm {
namespace tms9900 {

enum AddrMode : uint8_t {
    M_NONE = 0,
    M_IMM = 1,    // 16-bit Immediate data
    M_REG = 2,    // ---- ---- ---- wwww: Work Register Addressing
    M_SCNT = 3,   // ---- ---- cccc ----
    M_SRC = 4,    // ---- ---- --SS ssss
    M_CNT = 5,    // ---- --cc cc-- ----
    M_XOP = 6,    // ---- --vv vv-- ----
    M_DREG = 7,   // ---- --dd dd-- ----
    M_DST = 8,    // ---- DDdd dd-- ----
    M_REL = 10,   // ---- ---- nnnn nnnn
    M_CRU = 11,   // ---- ---- nnnn nnnn
    M_RTWP = 12,  // ---- ---- ---- -nnn RTWP mode
    M_DST2 = 13,  // 0100 DDdd dd-- ---- in 2nd word
    M_CNT2 = 14,  // 0100 00cc cc-- ---- in 2nd word
    M_BIT2 = 15,  // 0000 00bb bb-- ---- in 2nd word
    M_SRC2 = 16,  // ---- ---- --SS ssss in 2nd word
    // Only for assembler.
    M_IREG = 17,  // Work Register Indirect Addressing: *Rn
    M_INCR = 18,  // Work Register Indirect Auto Increment Addressing: *Rn+
    M_SYBL = 19,  // Symbol (Direct) Addressing: @LABEL
    M_INDX = 20,  // Indexed Addressing: @TABLE(Rn)
};

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint8_t _src;
        uint8_t _dst;

        static constexpr Flags create(AddrMode src, AddrMode dst) {
            return Flags{static_cast<uint8_t>(src), static_cast<uint8_t>(dst)};
        }
        Flags read() const { return Flags{pgm_read_byte(&_src), pgm_read_byte(&_dst)}; }

        AddrMode src() const { return AddrMode(_src); }
        AddrMode dst() const { return AddrMode(_dst); }
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const char *name)
        : Base(name, opCode), _flags(flags) {}

    Flags flags() const { return _flags.read(); }

private:
    Flags _flags;
};

}  // namespace tms9900
}  // namespace libasm

#endif  // __ENTRY_TMS9900_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

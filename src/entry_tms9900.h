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

enum CpuType {
    TMS9900,
    TMS9995,
};

enum AddrMode {
    INH     = 0,   // ---- ---- ---- ----
    IMM     = 1,   // ---- ---- ---- ---- + nnnn
    REG     = 2,   // ---- ---- ---- wwww
    REG_IMM = 3,   // ---- ---- ---- wwww + nnnn
    CNT_REG = 4,   // ---- ---- cccc wwww
    SRC     = 5,   // ---- ---- --SS ssss
    CNT_SRC = 6,   // ---- --cc ccSS ssss
    XOP_SRC = 7,   // ---- --vv vvSS ssss
    REG_SRC = 8,   // ---- --dd ddSS ssss
    DST_SRC = 9,   // ---- DDdd ddSS ssss
    REL     = 10,  // ---- ---- nnnn nnnn
    CRU_OFF = 11,  // ---- ---- nnnn nnnn
};

struct Entry {
    const Config::opcode_t opCode;
    const uint8_t flags;
    const char *name;

    static inline CpuType _cpuType(uint8_t flags) {
        return (flags & tms9995_bm) == 0 ? TMS9900 : TMS9995;
    }

    static inline AddrMode _addrMode(uint8_t flags) {
        return AddrMode(flags & addrMode_gm);
    }
    static constexpr uint8_t _flags(CpuType cpuType, AddrMode addrMode) {
        return (cpuType == TMS9995 ? tms9995_bm : 0)
            | static_cast<uint8_t>(addrMode);
    }

private:
    static constexpr uint8_t tms9995_bm = 0x80;
    static constexpr uint8_t addrMode_gm = 0xf;
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

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

enum CpuType : host::uint_t {
    TMS9900,
    TMS9995,
};

enum AddrMode : host::uint_t {
    INH,                        // ---- ---- ---- ----
    IMM,                        // ---- ---- ---- ---- + nnnn
    REG,                        // ---- ---- ---- wwww
    REG_IMM,                    // ---- ---- ---- wwww + nnnn
    CNT_REG,                    // ---- ---- cccc wwww
    SRC,                        // ---- ---- --SS ssss
    CNT_SRC,                    // ---- --cc ccSS ssss
    XOP_SRC,                    // ---- --vv vvSS ssss
    REG_SRC,                    // ---- --dd ddSS ssss
    DST_SRC,                    // ---- DDdd ddSS ssss
    REL,                        // ---- ---- nnnn nnnn
    CRU_OFF,                    // ---- ---- nnnn nnnn
};

struct Entry {
    const target::insn_t insnCode;
    const host::uint_t flags;
    static constexpr host::uint_t code_max = 6;
    static constexpr host::uint_t name_max = 4;
    const char *name;

    static inline CpuType _cpuType(host::uint_t flags) {
        return (flags & tms9995_bm) == 0 ? TMS9900 : TMS9995;
    }

    static inline AddrMode _addrMode(host::uint_t flags) {
        return AddrMode(flags & addrMode_gm);
    }
    static constexpr host::uint_t _flags(CpuType cpuType, AddrMode addrMode) {
        return (cpuType == TMS9995 ? tms9995_bm : 0)
            | host::uint_t(addrMode);
    }

private:
    static constexpr host::uint_t tms9995_bm = 0x80;
    static constexpr host::uint_t addrMode_gm = 0x0f;
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

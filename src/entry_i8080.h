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

#ifndef __ENTRY_I8080_H__
#define __ENTRY_I8080_H__

#include "config_i8080.h"

namespace libasm {
namespace i8080 {

enum CpuType : uint8_t {
    I8080,
    I8085,
};

enum AddrMode : uint8_t {
    M_NO   = 0,
    M_IM8  = 1,
    M_IM16 = 2,
    M_ABS  = 3,
    M_IOA  = 4,
    M_PTR  = 5,   // |..|pp|....|: B/D/H/SP
    M_STK  = 6,   // |..|pp|....|: B/D/H/PSW
    M_IDX  = 7,   // |...|i|....|: B/D
    M_REG  = 8,   // |......|rrr|: B/C/D/E/H/L/M/A
    M_DST  = 9,   // |..|rrr|...|: B/C/D/E/H/L/M/A
    M_VEC  = 10,  // |..|vvv|...|: 0~7
    M_REGH = 11,  // H register
};

struct Entry {
    const Config::opcode_t opCode;
    const uint8_t flags;
    const /*PROGMEM*/ char *name;

    static inline AddrMode _dstMode(uint8_t flags) {
        return AddrMode((flags >> dstMode_gp) & addrMode_gm);
    }
    static inline AddrMode _srcMode(uint8_t flags) {
        return AddrMode((flags >> srcMode_gp) & addrMode_gm);
    }
    static constexpr uint8_t _flags(AddrMode dst, AddrMode src) {
        return (static_cast<uint8_t>(dst) << dstMode_gp)
            | (static_cast<uint8_t>(src) << srcMode_gp);
    }

private:
    static constexpr int dstMode_gp = 0;
    static constexpr int srcMode_gp = 4;
    static constexpr uint8_t addrMode_gm = 0xF;
};

} // namespace i8080
} // namespace libasm

#endif // __ENTRY_I8080_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

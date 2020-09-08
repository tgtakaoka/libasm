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
    INHR   = 0,
    IMM8   = 1,
    IMM16  = 2,
    DIRECT = 3,
    IOADR  = 4,
};

enum InsnFormat : uint8_t {
    NO_FORMAT     = 0,
    POINTER_REG   = 1,  // **pp_****: B/D/H/SP
    STACK_REG     = 2,  // **PP_****: B/D/H/PSW
    INDEX_REG     = 3,  // ***I_****: B/D
    DATA_REG      = 4,  // **DD_D***: B/C/D/E/H/L/M/A
    LOW_DATA_REG  = 5,  // ****_*DDD: B/C/D/E/H/L/M/A
    DATA_DATA_REG = 6,  // **DD_DSSS: B/C/D/E/H/L/M/A
    VECTOR_NO     = 7,  // **VV_V***: 0~7
};

struct Entry {
    const Config::opcode_t opCode;
    const uint8_t flags;
    const char *name;

    static inline InsnFormat _insnFormat(uint8_t flags) {
        return InsnFormat((flags >> insnFormat_gp) & insnFormat_gm);
    }
    static inline AddrMode _addrMode(uint8_t flags) {
        return AddrMode(flags & addrMode_gm);
    }
    static constexpr uint8_t _flags(AddrMode addrMode, InsnFormat iformat) {
        return (static_cast<uint8_t>(iformat) << insnFormat_gp)
            | static_cast<uint8_t>(addrMode);
    }

private:
    static constexpr int     insnFormat_gp = 4;
    static constexpr uint8_t insnFormat_gm = 0x7;
    static constexpr uint8_t addrMode_gm   = 0x07;
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

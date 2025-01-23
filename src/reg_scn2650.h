/*
 * Copyright 2022 Tadashi G. Takaoka
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

#ifndef __LIBASM_REG_SCN2650_H__
#define __LIBASM_REG_SCN2650_H__

#include <stdint.h>
#include "config_scn2650.h"
#include "str_buffer.h"
#include "str_scanner.h"

namespace libasm {

struct ValueParser;

namespace scn2650 {

enum RegName : int8_t {
    REG_UNDEF = -1,
    REG_R0 = 0,
    REG_R1 = 1,
    REG_R2 = 2,
    REG_R3 = 3,
};

enum CcName : int8_t {
    CC_UNDEF = -1,
    CC_EQ = 0,  // Equal, Zero
    CC_GT = 1,  // Greater Than, Positive
    CC_LT = 2,  // Less Than, Negative
    CC_UN = 3,  // Unconditional, Always
};

namespace reg {

constexpr Config::uintptr_t page(const Config::uintptr_t addr) {
    return addr & ~0x1FFF;  // 8k bytes per page
}

constexpr Config::uintptr_t offset(const Config::uintptr_t addr) {
    return addr & 0x1FFF;
}

constexpr Config::uintptr_t inpage(const Config::uintptr_t addr, const Config::ptrdiff_t delta) {
    return page(addr) | offset(addr + delta);
}

RegName parseRegName(StrScanner &scan, const ValueParser &parser);
uint8_t encodeRegName(RegName name);
RegName decodeRegName(uint8_t opc);
StrBuffer &outRegName(StrBuffer &out, RegName name);

CcName parseCcName(StrScanner &scan, const ValueParser &parser);
uint8_t encodeCcName(CcName name);
CcName decodeCcName(uint8_t opc);
StrBuffer &outCcName(StrBuffer &out, const CcName name);

}  // namespace reg
}  // namespace scn2650
}  // namespace libasm

#endif  // __LIBASM_REG_SCN2650_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

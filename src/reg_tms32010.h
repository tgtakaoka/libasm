/*
 * Copyright 2021 Tadashi G. Takaoka
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

#ifndef __LIBASM_REG_TMS32010_H__
#define __LIBASM_REG_TMS32010_H__

#include <stdint.h>

#include "str_buffer.h"
#include "str_scanner.h"

namespace libasm {
namespace tms32010 {

enum RegName : int8_t {
    REG_UNDEF = -1,
    REG_AR0 = 0,   // Auxiliary Register 0
    REG_AR1 = 1,   // Auxiliary Register 1
    REG_PA0 = 8,   // Port address 0
    REG_PA1 = 9,   // Port address 1
    REG_PA2 = 10,  // Port address 2
    REG_PA3 = 11,  // Port address 3
    REG_PA4 = 12,  // Port address 4
    REG_PA5 = 13,  // Port address 5
    REG_PA6 = 14,  // Port address 6
    REG_PA7 = 15,  // Port address 7
};

namespace reg {

RegName parseRegName(StrScanner &scan);
StrBuffer &outRegName(StrBuffer &out, RegName name);
bool isAuxiliary(RegName name);
bool isPortAddress(RegName name);

}  // namespace reg
}  // namespace tms32010
}  // namespace libasm

#endif  // __REG_TMS32010_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

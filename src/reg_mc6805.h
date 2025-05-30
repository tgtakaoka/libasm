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

#ifndef __LIBASM_REG_MC6805_H__
#define __LIBASM_REG_MC6805_H__

#include <stdint.h>

#include "str_buffer.h"
#include "str_scanner.h"

namespace libasm {
namespace mc6805 {

enum RegName : int8_t {
    REG_UNDEF = 0,
    REG_X = 'X',
    REG_SP = 'S',
};

namespace reg {

RegName parseRegName(StrScanner &scan);
StrBuffer &outRegName(StrBuffer &out, RegName name);

}  // namespace reg
}  // namespace mc6805
}  // namespace libasm

#endif  // __LIBASM_REG_MC6805_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

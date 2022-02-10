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

#ifndef __REG_I8096_H__
#define __REG_I8096_H__

#include "entry_i8096.h"
#include "reg_base.h"

namespace libasm {
namespace i8096 {

class RegI8096 : public RegBase {
public:
    static bool isWreg(uint8_t regno);
    static bool isLreg(uint8_t regno);
};

}  // namespace i8096
}  // namespace libasm

#endif  // __REG_I8096_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
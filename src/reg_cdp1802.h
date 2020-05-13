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

#include "reg_base.h"

#ifndef __REGISTER_CDP1802_H__
#define __REGISTER_CDP1802_H__

namespace libasm {
namespace cdp1802 {

/*
 * SCRT (Standard CALL and RETURN Techinique.
 * R0: Direct Memory Access
 * R1: Interrupt
 * R2: Stack Pointer
 * R3: Program Counter
 * R4: Subroutine Call address
 * R5: Link register
 * R6: Argument pointer
 * R7-R15: Working register
 */

class RegCdp1802 : public RegBase {
};

} // namespace cdp1802
} // namespace libasm

#endif // __REGISTER_CDP1802_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

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

#ifndef __INSN_I8096_H__
#define __INSN_I8096_H__

#include "config_i8096.h"
#include "entry_i8096.h"
#include "insn_base.h"

namespace libasm {
namespace i8096 {

class InsnI8096 : public InsnImpl<Config, Entry> {
public:
    InsnI8096(Insn &insn) : InsnImpl(insn) {}

    AaMode aa() const { return AaMode(opCode() & 3); }
    AddrMode dst() const { return flags().dst(); }
    AddrMode src1() const { return flags().src1(); }
    AddrMode src2() const { return flags().src2(); }
};

}  // namespace i8096
}  // namespace libasm

#endif  // __INSN_I8096_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

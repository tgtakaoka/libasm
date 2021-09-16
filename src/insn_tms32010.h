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

#ifndef __INSN_TMS32010_H__
#define __INSN_TMS32010_H__

#include "config_tms32010.h"
#include "entry_tms32010.h"
#include "insn_base.h"

namespace libasm {
namespace tms32010 {

class InsnTms32010 : public InsnImpl<Config, Entry> {
public:
    InsnTms32010(Insn &insn) : InsnImpl(insn) {}

    AddrMode op1() const { return flags().op1(); }
    AddrMode op2() const { return flags().op2(); }
    AddrMode op3() const { return flags().op3(); }
};

}  // namespace tms32010
}  // namespace libasm

#endif  // __INSN_TMS32010_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

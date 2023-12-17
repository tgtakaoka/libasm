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

#ifndef __LIBASM_DIS_TMS9900_H__
#define __LIBASM_DIS_TMS9900_H__

#include "config_tms9900.h"
#include "dis_base.h"
#include "insn_tms9900.h"

namespace libasm {
namespace tms9900 {

struct DisTms9900 final : Disassembler, Config {
    DisTms9900(const ValueFormatter::Plugins &plugins = defaultPlugins());

private:
    Error checkPostWord(DisInsn &insn) const;
    void decodeRelative(DisInsn &insn, StrBuffer &out) const;
    void decodeModeReg(DisInsn &insn, StrBuffer &out, uint8_t modeReg) const;
    void decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode) const;

    Error decodeImpl(DisMemory &memory, Insn &insn, StrBuffer &out) const override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueFormatter::Plugins &defaultPlugins();
};

}  // namespace tms9900
}  // namespace libasm

#endif  // __DIS_TMS9900_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

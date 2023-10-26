/*
 * Copyright 2023 Tadashi G. Takaoka
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

#ifndef __DIS_F3850_H__
#define __DIS_F3850_H__

#include "config_f3850.h"
#include "dis_base.h"
#include "insn_f3850.h"

namespace libasm {
namespace f3850 {

struct DisF3850 final : Disassembler, Config {
    DisF3850(const ValueFormatter::Plugins &plugins = defaultPlugins());

    void reset() override;

    Error setUseScratchpadName(bool enable);

private:
    const BoolOption<DisF3850> _opt_useScratchpad;

    bool _useScratchpad;

    void decodeRelative(DisInsn &insn, StrBuffer &out) const;
    void decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode) const;

    Error decodeImpl(DisMemory &memory, Insn &insn, StrBuffer &out) const override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueFormatter::Plugins &defaultPlugins();
};

}  // namespace f3850
}  // namespace libasm

#endif  // __DIS_F3850_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

/*
 * Copyright 2026 Tadashi G. Takaoka
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

#ifndef __LIBASM_DIS_AM29000_H__
#define __LIBASM_DIS_AM29000_H__

#include "config_am29000.h"
#include "dis_base.h"
#include "insn_am29000.h"

namespace libasm {
namespace am29000 {

struct DisAm29000 final : Disassembler, Config {
    DisAm29000(const ValueFormatter::Plugins &plugins = defaultPlugins());

    void reset() override;

    Error setExternSymbol(bool enable);

private:
    const BoolOption<Config> _opt_fpu;
    const BoolOption<Config> _opt_intMul;
    const BoolOption<DisAm29000> _opt_externSymbol;

    bool _externSymbol;

    void decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode) const;

    Error decodeImpl(DisMemory &memory, Insn &insn, StrBuffer &out) const override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueFormatter::Plugins &defaultPlugins();
};

}  // namespace am29000
}  // namespace libasm

#endif  // __LIBASM_DIS_AM29000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

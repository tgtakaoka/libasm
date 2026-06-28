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

#ifndef __LIBASM_DIS_CP1600_H__
#define __LIBASM_DIS_CP1600_H__

#include "config_cp1600.h"
#include "dis_base.h"
#include "insn_cp1600.h"

namespace libasm {
namespace cp1600 {

struct DisCp1600 final : Disassembler, Config {
    DisCp1600(const ValueFormatter::Plugins &plugins = defaultPlugins());

    void reset() override;

    Error setSpAlias(bool enable);
    Error setPcAlias(bool enable);

private:
    const BoolOption<DisCp1600> _opt_spAlias;
    const BoolOption<DisCp1600> _opt_pcAlias;

    bool _spAlias;
    bool _pcAlias;

    StrBuffer &outReg(StrBuffer &out, uint_fast8_t num) const;
    void decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode) const;
    void decodeBody(DisInsn &insn, StrBuffer &out) const;

    Error decodeImpl(DisMemory &memory, Insn &insn, StrBuffer &out) const override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueFormatter::Plugins &defaultPlugins();
};

}  // namespace cp1600
}  // namespace libasm

#endif  // __LIBASM_DIS_CP1600_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

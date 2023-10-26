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

#ifndef __DIS_SCN2650_H__
#define __DIS_SCN2650_H__

#include "config_scn2650.h"
#include "dis_base.h"
#include "insn_scn2650.h"

namespace libasm {
namespace scn2650 {

struct DisScn2650 final : Disassembler, Config {
    DisScn2650(const ValueFormatter::Plugins &plugins = defaultPlugins());

private:
    void decodeAbsolute(DisInsn &insn, StrBuffer &out, AddrMode mode) const;
    void decodeIndexed(DisInsn &insn, StrBuffer &out) const;
    void decodeRelative(DisInsn &insn, StrBuffer &out, AddrMode mode) const;
    void decodeOperand(DisInsn &insn, StrBuffer &out, const AddrMode mode) const;

    Error decodeImpl(DisMemory &memory, Insn &insn, StrBuffer &out) const override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueFormatter::Plugins &defaultPlugins();
};

}  // namespace scn2650
}  // namespace libasm

#endif  // __DIS_SCN2650_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

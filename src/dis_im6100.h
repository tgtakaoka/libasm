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

#ifndef __DIS_IM6100_H__
#define __DIS_IM6100_H__

#include "config_im6100.h"
#include "dis_base.h"
#include "insn_im6100.h"

namespace libasm {
namespace im6100 {

struct DisIm6100 final : Disassembler, Config {
    DisIm6100(const ValueFormatter::Plugins &plugins = defaultPlugins());

    void reset() override;

    Error setIgnoreliteral(bool enable);

private:
    const BoolOption<DisIm6100> _opt_ignoreliteral;

    bool _ignoreliteral;

    Error decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode);

    Error decodeImpl(DisMemory &memory, Insn &insn, StrBuffer &out) override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueFormatter::Plugins &defaultPlugins();
};

}  // namespace im6100
}  // namespace libasm

#endif  // __DIS_IM6100_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

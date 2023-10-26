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

#ifndef __DIS_MC6805_H__
#define __DIS_MC6805_H__

#include "config_mc6805.h"
#include "dis_base.h"
#include "insn_mc6805.h"

namespace libasm {
namespace mc6805 {

struct DisMc6805 final : Disassembler, Config {
    DisMc6805(const ValueFormatter::Plugins &plugins = defaultPlugins());

    AddressWidth addressWidth() const override;
    void reset() override;

    Error setPcBits(int32_t val);

private:
    const IntOption<DisMc6805> _opt_pc_bits;

    uint8_t _pc_bits;

    void decodeDirectPage(DisInsn &insn, StrBuffer &out) const;
    void decodeExtended(DisInsn &insn, StrBuffer &out) const;
    void decodeIndexed(DisInsn &insn, StrBuffer &out, AddrMode mode) const;
    void decodeRelative(DisInsn &insn, StrBuffer &out) const;
    void decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode) const;

    Error decodeImpl(DisMemory &memory, Insn &insn, StrBuffer &out) const override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueFormatter::Plugins &defaultPlugins();
};

}  // namespace mc6805
}  // namespace libasm

#endif  // __DIS_MC6805_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

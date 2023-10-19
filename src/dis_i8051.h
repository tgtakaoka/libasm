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

#ifndef __DIS_I8051_H__
#define __DIS_I8051_H__

#include "config_i8051.h"
#include "dis_base.h"
#include "insn_i8051.h"

namespace libasm {
namespace i8051 {

struct DisI8051 final : Disassembler, Config {
    DisI8051(const ValueFormatter::Plugins &plugins = defaultPlugins());

private:
    void decodeRelative(DisInsn &insn, StrBuffer &out) const;
    void decodeBitAddr(DisInsn &insn, StrBuffer &out, AddrMode mode) const;
    void decodeRReg(DisInsn &insn, StrBuffer &out, AddrMode mode) const;
    void decodeAddress(DisInsn &insn, StrBuffer &out, AddrMode mode) const;
    void decodeImmediate(DisInsn &insn, StrBuffer &out, AddrMode mode) const;
    void decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode) const;

    Error decodeImpl(DisMemory &memory, Insn &insn, StrBuffer &out) override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueFormatter::Plugins &defaultPlugins();
};

}  // namespace i8051
}  // namespace libasm

#endif  // __DIS_I8051_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

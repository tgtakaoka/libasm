/*
 * Copyright 2025 Tadashi G. Takaoka
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

#ifndef __LIBASM_DIS_Z380_H__
#define __LIBASM_DIS_Z380_H__

#include "config_z380.h"
#include "dis_base.h"
#include "insn_z380.h"
#include "reg_z380.h"

namespace libasm {
namespace z380 {

struct DisZ380 final : Disassembler, Config {
    DisZ380(const ValueFormatter::Plugins &plugins = defaultPlugins());

    void reset() override;

private:
    const BoolOption<DisZ380> _opt_extmode;
    const BoolOption<DisZ380> _opt_lwordmode;

    StrBuffer &outIndirectReg(StrBuffer &out, RegName reg) const;
    StrBuffer &outDataReg(StrBuffer &out, RegName reg) const;
    StrBuffer &outAlternateReg(StrBuffer &out, const DisInsn &insn, AddrMode other) const;

    bool wordMode(const Ddir &ddir) const;
    bool lwordMode(const Ddir &ddir) const;
    void decodeImmediate16(DisInsn &insn, StrBuffer &out, AddrMode mode) const;
    void decodeAbsolute(DisInsn &insn, StrBuffer &out, AddrMode mode) const;
    void decodeRelative(DisInsn &insn, StrBuffer &out, AddrMode mode) const;
    void decodeShortIndex(DisInsn &insn, StrBuffer &out, RegName base) const;
    void decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode, AddrMode other) const;

    StrBuffer &outAbsAddr(StrBuffer &out, uint32_t val, uint8_t addrWidth = 0) const override;
    Error decodeImpl(DisMemory &memory, Insn &insn, StrBuffer &out) const override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueFormatter::Plugins &defaultPlugins();
};

}  // namespace z380
}  // namespace libasm

#endif  // __LIBASM_DIS_Z380_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

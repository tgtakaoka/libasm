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

#ifndef __LIBASM_DIS_Z280_H__
#define __LIBASM_DIS_Z280_H__

#include "config_z280.h"
#include "dis_base.h"
#include "insn_z280.h"
#include "reg_z280.h"

namespace libasm {
namespace z280 {

struct DisZ280 final : Disassembler, Config {
    DisZ280(const ValueFormatter::Plugins &plugins = defaultPlugins());

private:
    StrBuffer &outIndirectReg(StrBuffer &out, RegName reg) const;
    StrBuffer &outDataReg(StrBuffer &out, RegName reg) const;
    StrBuffer &outAlternateReg(StrBuffer &out, const DisInsn &insn, AddrMode other) const;

    void decodeImmediate16(DisInsn &insn, StrBuffer &out) const;
    void decodeAbsolute(DisInsn &insn, StrBuffer &out) const;
    void decodeRelative(DisInsn &insn, StrBuffer &out, AddrMode mode) const;
    void decodeShortIndex(DisInsn &insn, StrBuffer &out, RegName base) const;
    void decodeLongIndex(DisInsn &insn, StrBuffer &out, RegName base) const;
    void decodeMemoryPointer(DisInsn &insn, StrBuffer &out) const;
    void decodeBaseIndex(DisInsn &insn, StrBuffer &out, AddrMode mode) const;
    void decodePointerIndex(DisInsn &insn, StrBuffer &out, AddrMode mode) const;
    void decodeFullIndex(DisInsn &insn, StrBuffer &out) const;
    void decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode, AddrMode other) const;

    StrBuffer &outAbsAddr(StrBuffer &out, uint32_t val, uint8_t addrWidth = 0) const override;
    Error decodeImpl(DisMemory &memory, Insn &insn, StrBuffer &out) const override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueFormatter::Plugins &defaultPlugins();
};

}  // namespace z280
}  // namespace libasm

#endif  // __LIBASM_DIS_Z280_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

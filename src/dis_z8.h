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

#ifndef __LIBASM_DIS_Z8_H__
#define __LIBASM_DIS_Z8_H__

#include "config_z8.h"
#include "dis_base.h"
#include "insn_z8.h"

namespace libasm {
namespace z8 {

struct DisZ8 final : Disassembler, Config {
    DisZ8(const ValueFormatter::Plugins &plugins = defaultPlugins());

    void reset() override;

    Error setUseWorkRegister(bool enable);

private:
    const BoolOption<DisZ8> _opt_workRegister;

    bool _useWorkRegister;

    struct Operand;

    StrBuffer &outRegAddr(StrBuffer &out, uint8_t regAddr, bool indir = false) const;
    StrBuffer &outPairAddr(StrBuffer &out, uint8_t regAddr, bool indir = false) const;
    StrBuffer &outBitPos(StrBuffer &out, uint8_t bitPos) const;

    void readOperands(DisInsn &insn, Operand &dstOp, Operand &srcOp, Operand &extOp) const;
    void decodeWorkReg(DisInsn &insn, StrBuffer &out, Operand &op) const;
    void decodeRegAddr(DisInsn &insn, StrBuffer &out, Operand &op) const;
    void decodeImmediate(DisInsn &insn, StrBuffer &out, Operand &op) const;
    void decodeRelative(DisInsn &insn, StrBuffer &out, Operand &op) const;
    void decodeIndexed(DisInsn &insn, StrBuffer &out, Operand &op) const;
    void decodeOperand(DisInsn &insn, StrBuffer &out, Operand &) const;

    Error decodeImpl(DisMemory &memory, Insn &insn, StrBuffer &out) const override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueFormatter::Plugins &defaultPlugins();
};

}  // namespace z8
}  // namespace libasm

#endif  // __DIS_Z8_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

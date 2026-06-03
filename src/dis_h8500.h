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

#ifndef __LIBASM_DIS_H8500_H__
#define __LIBASM_DIS_H8500_H__

#include "config_h8500.h"
#include "dis_base.h"
#include "insn_h8500.h"

namespace libasm {
namespace h8500 {

struct DisH8500 final : Disassembler, Config {
    DisH8500(const ValueFormatter::Plugins &plugins = defaultPlugins());

private:
    // EA byte → operand output helpers
    void outEaOperand(DisInsn &insn, StrBuffer &out, uint8_t ea) const;
    void outReg(StrBuffer &out, RegName reg) const;
    void outCr(StrBuffer &out, CrName cr) const;
    void outImm8(StrBuffer &out, uint8_t val) const;
    void outImm16(DisInsn &insn, StrBuffer &out) const;
    void outAbs8(StrBuffer &out, uint8_t addr) const;
    void outAbs16(DisInsn &insn, StrBuffer &out) const;
    void outPcRel8(DisInsn &insn, StrBuffer &out) const;
    void outPcRel16(DisInsn &insn, StrBuffer &out) const;
    void outBit(StrBuffer &out, uint8_t bit) const;

    // Decode helpers
    Error decodeGenFmt(DisInsn &insn, StrBuffer &out) const;
    Error decodeSpcFmt(DisInsn &insn, StrBuffer &out) const;
    Error decodeSecondary(DisInsn &insn, StrBuffer &out) const;

    Error decodeImpl(DisMemory &memory, Insn &insn, StrBuffer &out) const override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueFormatter::Plugins &defaultPlugins();
};

}  // namespace h8500
}  // namespace libasm

#endif  // __LIBASM_DIS_H8500_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

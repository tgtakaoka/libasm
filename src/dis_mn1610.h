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

#ifndef __DIS_MN1610_H__
#define __DIS_MN1610_H__

#include "config_mn1610.h"
#include "dis_base.h"
#include "insn_mn1610.h"
#include "reg_mn1610.h"

namespace libasm {
namespace mn1610 {

struct DisMn1610 final : Disassembler, Config {
    DisMn1610(const ValueFormatter::Plugins &plugins = defaultPlugins());

private:
    Error outRegister(StrBuffer &out, RegName reg, AddrMode mode);
    Error outConditionCode(StrBuffer &out, CcName cc);
    Error outGenericAddr(StrBuffer &out, Config::opcode_t opc, Config::uintptr_t base);
    Error outIndirect(StrBuffer &out, Config::opcode_t opc);
    StrBuffer &outComma(StrBuffer &out, Config::opcode_t opc, AddrMode mode);
    Error decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode);

    Error decodeImpl(DisMemory &memory, Insn &insn, StrBuffer &out) override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueFormatter::Plugins &defaultPlugins();
};

}  // namespace mn1610
}  // namespace libasm

#endif  // __DIS_MN1610_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

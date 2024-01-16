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

#ifndef __LIBASM_ASM_I8096_H__
#define __LIBASM_ASM_I8096_H__

#include "asm_base.h"
#include "config_i8096.h"
#include "insn_i8096.h"

namespace libasm {
namespace i8096 {

struct AsmI8096 final : Assembler, Config {
    AsmI8096(const ValueParser::Plugins &plugins = defaultPlugins());

private:
    Error parseIndirect(StrScanner &scan, Operand &opr) const;
    Error parseOperand(StrScanner &scan, Operand &opr) const;
    void emitAop(AsmInsn &insn, AddrMode mode, const Operand &op) const;
    void emitRelative(AsmInsn &insn, AddrMode mode, const Operand &op) const;
    void emitOperand(AsmInsn &insn, AddrMode mode, const Operand &op) const;

    Error encodeImpl(StrScanner &scan, Insn &insn) const override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueParser::Plugins &defaultPlugins();
};

}  // namespace i8096
}  // namespace libasm

#endif  // __LIBASM_ASM_I8096_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

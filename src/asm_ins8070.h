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

#ifndef __LIBASM_ASM_INS8070_H__
#define __LIBASM_ASM_INS8070_H__

#include "asm_base.h"
#include "config_ins8070.h"
#include "insn_ins8070.h"

namespace libasm {
namespace ins8070 {

struct AsmIns8070 final : Assembler, Config {
    AsmIns8070(const ValueParser::Plugins &plugins = defaultPlugins());

private:
    Error parseOperand(StrScanner &scan, Operand &op) const;

    void emitAbsolute(AsmInsn &insn, const Operand &op) const;
    void emitImmediate(AsmInsn &insn, const Operand &op) const;
    void emitRelative(AsmInsn &insn, const Operand &op) const;
    void emitGeneric(AsmInsn &insn, const Operand &op) const;
    void emitOperand(AsmInsn &insn, AddrMode mode, const Operand &op) const;

    Error defineAddrConstant(StrScanner &scan, Insn &insn);
    Error processPseudo(StrScanner &scan, Insn &insn) override;
    Error encodeImpl(StrScanner &scan, Insn &insn) const override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueParser::Plugins &defaultPlugins();
};

}  // namespace ins8070
}  // namespace libasm

#endif  // __LIBASM_ASM_INS8070_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

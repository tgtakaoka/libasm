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

#ifndef __LIBASM_ASM_I8080_H__
#define __LIBASM_ASM_I8080_H__

#include "asm_base.h"
#include "config_i8080.h"
#include "insn_i8080.h"

namespace libasm {
namespace i8080 {

struct AsmI8080 final : Assembler, Config {
    AsmI8080(const ValueParser::Plugins &plugins = defaultPlugins());

    void reset() override;

    Error setZilogSyntax(bool enable);

private:
    const BoolOption<AsmI8080> _opt_zilog;

    bool _zilogSyntax;

    Error parseIntelOperand(StrScanner &scan, Operand &op) const;
    Error parseZilogOperand(StrScanner &scan, Operand &op) const;
    Error parseOperand(StrScanner &scan, Operand &op) const;

    void encodeOperand(AsmInsn &insn, const Operand &op, AddrMode mode) const;

    Error processPseudo(StrScanner &scan, Insn &insn) override;
    Error encodeImpl(StrScanner &scan, Insn &insn) const override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueParser::Plugins &defaultPlugins();
};

}  // namespace i8080
}  // namespace libasm

#endif  // __LIBASM_ASM_I8080_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

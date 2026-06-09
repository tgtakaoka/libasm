/*
 * Copyright 2026 Tadashi G. Takaoka
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

#ifndef __LIBASM_ASM_H8300_H__
#define __LIBASM_ASM_H8300_H__

#include "asm_base.h"
#include "config_h8300.h"
#include "insn_h8300.h"

namespace libasm {
namespace h8300 {

struct AsmH8300 final : Assembler, Config {
    AsmH8300(const ValueParser::Plugins &plugins = defaultPlugins());

private:
    const BoolOption<Config> _opt_advancedMode;

    RegName parseRegOperand(StrScanner &scan) const;
    Error parseBitSuffix(StrScanner &scan, Operand &op) const;
    Error parseOperand(StrScanner &scan, Operand &op) const;
    void encodeOprReg8(AsmInsn &insn, const Operand &op, OprPos pos) const;
    void encodeOprReg16(AsmInsn &insn, const Operand &op, OprPos pos) const;
    void encodeOprReg32(AsmInsn &insn, const Operand &op, OprPos pos) const;
    void encodeOprAddrReg(AsmInsn &insn, const Operand &op, OprPos pos) const;
    void encodeAbsolute(AsmInsn &insn, const Operand &op, AddrMode mode, OprPos pos) const;
    void encodeRelative(AsmInsn &insn, const Operand &op) const;
    void encodeOperand(AsmInsn &insn, const Operand &op, AddrMode mode, OprPos pos) const;

    Error encodeImpl(StrScanner &scan, Insn &insn) const override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueParser::Plugins &defaultPlugins();
};

}  // namespace h8300
}  // namespace libasm

#endif  // __LIBASM_ASM_H8300_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

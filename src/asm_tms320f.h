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

#ifndef __LIBASM_ASM_TMS320F_H__
#define __LIBASM_ASM_TMS320F_H__

#include "asm_base.h"
#include "config_tms320f.h"
#include "insn_tms320f.h"

namespace libasm {
namespace tms320f {

struct AsmTms320f final : Assembler, Config {
    AsmTms320f(const ValueParser::Plugins &plugins = defaultPlugins());

private:
    mutable Insn _prevInsn;
    mutable AsmInsn _prev;

    Error parseOperand(StrScanner &scan, Operand &op) const;
    Error encodeRelative(AsmInsn &insn, const Operand &op, AddrMode mode) const;
    Error encodeIndirect(AsmInsn &insn, const Operand &op, AddrMode mode, OprPos pos) const;
    Error encodeInteger(AsmInsn &insn, const Operand &op, AddrMode mode, OprPos pos) const;
    Error encodeFloat(AsmInsn &insn, const Operand &op, AddrMode mode, OprPos pos) const;
    Error encodeOperand(
            AsmInsn &out, const AsmInsn &insn, const Operand &op, AddrMode mode, OprPos pos) const;

    Error alignOrigin(StrScanner &scan, Insn &insn, uint16_t);
    Error defineFloat(StrScanner &scan, Insn &insn, uint16_t bits);
    Error defineInteger(StrScanner &scan, Insn &insn, uint16_t max);
    Error processPseudo(StrScanner &scan, Insn &insn) override;
    Error encodeParallel(StrScanner &scan, Insn &insn, uint16_t);
    Error encodeImpl(StrScanner &scan, Insn &insn) const override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueParser::Plugins &defaultPlugins();

    using PseudoTms320F = pseudo::__Pseudo<AsmTms320f, Insn>;
    using PseudosTms320F = pseudo::__Pseudos<PseudoTms320F>;
    static const PseudoTms320F PSEUDO_TMS320F_TABLE[] PROGMEM;
    static const PseudosTms320F PSEUDOS_TMS320F PROGMEM;
};

}  // namespace tms320f
}  // namespace libasm

#endif  // __LIBASM_ASM_TMS320F_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

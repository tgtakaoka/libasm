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

#ifndef __LIBASM_ASM_H8500_H__
#define __LIBASM_ASM_H8500_H__

#include "asm_base.h"
#include "config_h8500.h"
#include "insn_h8500.h"

namespace libasm {
namespace h8500 {

struct AsmH8500 final : Assembler, Config {
    AsmH8500(const ValueParser::Plugins &plugins = defaultPlugins());

private:
    const BoolOption<Config> _opt_maxMode;

    Error parseOperand(StrScanner &scan, Operand &op) const;

    void emitPcRel8(AsmInsn &insn, const Operand &op) const;
    void emitPcRel16(AsmInsn &insn, const Operand &op) const;
    // Low 16/8 bits of an absolute operand, validated against the assumed page
    // register (DP/CP for @aa:16, BR for @aa:8): undefined -> silently drop the
    // high byte, defined -> OVERFLOW_RANGE on a page mismatch.
    uint16_t pageLow16(AsmInsn &insn, const Operand &op, PageReg page) const;
    uint8_t pageLow8(AsmInsn &insn, const Operand &op) const;
    void emitEaExt(AsmInsn &insn, const Operand &op, AddrMode mode) const;
    Error emitOperand(AsmInsn &insn, const Operand &op, AddrMode mode) const;

    Error encodeGenFmt(AsmInsn &insn) const;
    Error encodeSpcFmt(AsmInsn &insn) const;
    Error encodeSecFmt(AsmInsn &insn) const;

    Error processMaxMode(StrScanner &scan, Insn &insn, uint16_t extra);
    Error processSet(StrScanner &scan, Insn &insn, uint16_t extra);

    void reset() override;
    Error processPseudo(StrScanner &scan, Insn &insn) override;
    Error encodeImpl(StrScanner &scan, Insn &insn) const override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueParser::Plugins &defaultPlugins();

    using PseudoH8500 = pseudo::__Pseudo<AsmH8500, Insn>;
    using PseudosH8500 = pseudo::__Pseudos<PseudoH8500>;
    static const PseudoH8500 PSEUDO_H8500_TABLE[] PROGMEM;
    static const PseudosH8500 PSEUDOS_H8500 PROGMEM;
};

}  // namespace h8500
}  // namespace libasm

#endif  // __LIBASM_ASM_H8500_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

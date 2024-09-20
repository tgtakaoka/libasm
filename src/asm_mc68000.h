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

#ifndef __LIBASM_ASM_MC68000_H__
#define __LIBASM_ASM_MC68000_H__

#include "asm_base.h"
#include "config_mc68000.h"
#include "insn_mc68000.h"

namespace libasm {
namespace mc68000 {

struct AsmMc68000 final : Assembler, Config {
    AsmMc68000(const ValueParser::Plugins &plugins = defaultPlugins());

    void reset() override;

    Error setFpu(StrScanner &scan) override;

private:
    Error parseOperand(StrScanner &scan, Operand &op) const;
    Error parseKFactor(StrScanner &scan, Operand &op) const;
    Error checkAlignment(AsmInsn &insn, OprSize size, const Operand &op) const;

    void encodeBriefExtension(AsmInsn &insn, const Operand &op, Config::ptrdiff_t disp) const;
    void encodeDisplacement(AsmInsn &insn, const Operand &op, Config::ptrdiff_t disp) const;
    void encodeRelativeAddr(AsmInsn &insn, AddrMode mode, const Operand &op) const;
    void encodeImmediate(AsmInsn &insn, const Operand &op, OprSize size) const;
    void encodeFloatControlList(AsmInsn &insn, const Operand &o) const;
    void encodeFloatRegisterList(AsmInsn &insn, const Operand &o) const;
    void encodeRegisterList(AsmInsn &insn, const Operand &op, bool reverse = false) const;
    Error encodeOperand(
            AsmInsn &insn, OprSize size, const Operand &op, AddrMode mode, OprPos pos) const;

    Error processPseudo(StrScanner &scan, Insn &insn) override;
    Error encodeImpl(StrScanner &scan, Insn &insn) const override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueParser::Plugins &defaultPlugins();
};

}  // namespace mc68000
}  // namespace libasm

#endif  // __LIBASM_ASM_MC68000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

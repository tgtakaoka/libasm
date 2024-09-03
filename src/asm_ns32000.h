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

#ifndef __LIBASM_ASM_NS32000_H__
#define __LIBASM_ASM_NS32000_H__

#include "asm_base.h"
#include "config_ns32000.h"
#include "insn_ns32000.h"
#include "reg_ns32000.h"

namespace libasm {
namespace ns32000 {

struct AsmNs32000 final : Assembler, Config {
    AsmNs32000(const ValueParser::Plugins &plugins = defaultPlugins());

    void reset() override;

    Error setFpu(StrScanner &scan) override;
    Error setPmmu(StrScanner &scan);

private:
    const TextOption<AsmNs32000> _opt_pmmu;

    Error parseStrOptNames(StrScanner &scan, Operand &op, bool braket = false) const;
    Error parseConfigNames(StrScanner &scan, Operand &op) const;
    Error parseRegisterList(StrScanner &scan, Operand &op) const;
    Error parseBaseOperand(StrScanner &scan, Operand &op) const;
    Error parseOperand(StrScanner &scan, Operand &op) const;
    void emitDisplacement(AsmInsn &insn, const Operand &op, const Value &disp,
            Error error = OVERFLOW_RANGE) const;
    void emitLength(AsmInsn &insn, const Operand &op) const;
    void emitBitField(AsmInsn &insn, AddrMode mode, const Operand &off, const Operand &len) const;
    void emitImmediate(AsmInsn &insn, AddrMode mode, const Operand &op) const;
    void emitIndexByte(AsmInsn &insn, const Operand &op) const;
    uint8_t encodeGenericField(AddrMode mode, RegName reg) const;
    void emitGeneric(AsmInsn &insn, AddrMode mode, const Operand &op, OprPos pos) const;
    void emitRelative(AsmInsn &insn, const Operand &op) const;
    void emitOperand(AsmInsn &insn, AddrMode mode, OprSize size, const Operand &op, OprPos pos,
            const Operand &prevOp) const;

    Error processPseudo(StrScanner &scan, Insn &insn) override;
    Error encodeImpl(StrScanner &scan, Insn &insn) const override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueParser::Plugins &defaultPlugins();
};

}  // namespace ns32000
}  // namespace libasm

#endif  // __LIBASM_ASM_NS32000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

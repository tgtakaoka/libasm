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

#ifndef __LIBASM_ASM_I8086_H__
#define __LIBASM_ASM_I8086_H__

#include "asm_base.h"
#include "config_i8086.h"
#include "insn_i8086.h"
#include "reg_i8086.h"

namespace libasm {
namespace i8086 {

struct AsmI8086 final : Assembler, Config {
    AsmI8086(const ValueParser::Plugins &plugins = defaultPlugins());

    void reset() override;

    Error setOptimizeSegment(bool enable);

private:
    const TextOption<Config> _opt_fpu;
    const BoolOption<AsmI8086> _opt_optimizeSegment;

    bool _optimizeSegment;

    Error parsePointerSize(StrScanner &scan, Operand &op) const;
    void parseSegmentOverride(StrScanner &scan, Operand &op) const;
    void parseBaseRegister(StrScanner &scan, Operand &op) const;
    void parseIndexRegister(StrScanner &scan, Operand &op) const;
    Error parseDisplacement(StrScanner &scan, Operand &op) const;
    Error parseOperand(StrScanner &scan, Operand &opr) const;

    void emitImmediate(
            AsmInsn &insn, AddrMode mode, OprSize size, const Value &val, const ErrorAt &at) const;
    void emitRelative(AsmInsn &insn, const Operand &op, AddrMode mode) const;
    void emitRegister(AsmInsn &insn, const Operand &op, OprPos pos) const;
    Config::opcode_t encodeSegmentOverride(RegName seg, RegName base) const;
    void emitModReg(AsmInsn &insn, const Operand &op, OprPos pos) const;
    void emitDirect(AsmInsn &insn, const Operand &op, OprPos pos) const;
    void emitOperand(AsmInsn &insn, AddrMode mode, const Operand &op, OprPos pos) const;
    void emitStringOperand(AsmInsn &insn, const Operand &op, RegName seg, RegName index) const;
    void emitStringInst(AsmInsn &insn, const Operand &src, const Operand &dst) const;

    Error setCoprocessor(StrScanner &scan, AsmInsn &insn, uint16_t);
    enum I8087Type : uint16_t {
        DATA_DD = 'D',  // 32-bit binary or IEEE 754 floating point number
        DATA_DQ = 'Q',  // 64-bit binary or IEEE 754 floating point number
        DATA_DT = 'T'   // 80-bit BCD or i8087 Temporary Real
    };
    Error defineConstant(StrScanner &scan, AsmInsn &insn, uint16_t i8087Type);
    Error encodeRepeatInsn(StrScanner &scan, AsmInsn &insn, uint16_t opc);
    Error processPseudo(StrScanner &scan, Insn &insn) override;
    Error encodeImpl(StrScanner &scan, AsmInsn &insn) const;
    Error encodeImpl(StrScanner &scan, Insn &insn) const override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueParser::Plugins &defaultPlugins();

    using PseudoI8086 = pseudo::__Pseudo<AsmI8086, AsmInsn>;
    using PseudosI8086 = pseudo::__Pseudos<PseudoI8086>;
    static const PseudoI8086 PSEUDO_I8086_TABLE[] PROGMEM;
    static const PseudosI8086 PSEUDOS_I8086 PROGMEM;
};

}  // namespace i8086
}  // namespace libasm

#endif  // __LIBASM_ASM_I8086_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

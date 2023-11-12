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

#ifndef __ASM_Z8_H__
#define __ASM_Z8_H__

#include "asm_base.h"
#include "config_z8.h"
#include "insn_z8.h"

namespace libasm {
namespace z8 {

struct AsmZ8 final : Assembler, Config {
    AsmZ8(const ValueParser::Plugins &plugins = defaultPlugins());

    void reset() override;

    Error setRegAlias(bool enable);
    Error setRegPointer(int32_t rp);
    Error setRegPointer0(int32_t rp);
    Error setRegPointer1(int32_t rp);
    Error setOptimizeIndex(bool enable);

private:
    const BoolOption<AsmZ8> _opt_reg_alias;
    const IntOption<AsmZ8> _opt_setrp;
    const IntOption<AsmZ8> _opt_setrp0;
    const IntOption<AsmZ8> _opt_setrp1;
    const BoolOption<AsmZ8> _opt_optimize_index;

    bool _regAlias;
    int16_t _regPointer0;
    int16_t _regPointer1;
    bool _optimizeIndex;

    bool isWorkReg(uint8_t regAddr) const;

    Error parseOperand(StrScanner &scan, Operand &op) const;

    void encodeRegImmOperand(AsmInsn &insn, const AddrMode mode, const Operand &op) const;
    void encodeAbsolute(AsmInsn &insn, const Operand &dstOp, const Operand &srcOp) const;
    void encodeRelative(AsmInsn &insn, const Operand &op) const;
    void encodeIndexed(AsmInsn &insn, const Operand &dstOp, const Operand &srcOp) const;
    void encodeIndirectRegPair(AsmInsn &insn, const Operand &dstOp, const Operand &srcOp) const;
    void encodeInOpCode(AsmInsn &insn, const Operand &dstOp, const Operand &srcOp) const;
    void encodeMultiOperands(
            AsmInsn &insn, const Operand &dstOp, const Operand &srcOp, const Operand &extOp) const;
    void encodePostByte(
            AsmInsn &insn, const Operand &dstOp, const Operand &srcOp, const Operand &extOp) const;
    void encodeRelative(AsmInsn &insn, OprPos pos, const Operand &op) const;
    void encodeIndexed(AsmInsn &insn, AddrMode mode, OprPos pos, const Operand &srcOp) const;
    void encodeRegAddr(AsmInsn &insn, OprPos pos, const Operand &op) const;
    void encodeImmediate(AsmInsn &insn, AddrMode mode, OprPos pos, const Operand &op) const;
    void encodeOperand(AsmInsn &insn, AddrMode mode, OprPos pos, const Operand &op) const;

    Error processPseudo(StrScanner &scan, Insn &insn) override;
    Error encodeImpl(StrScanner &scan, Insn &insn) const override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueParser::Plugins &defaultPlugins();
};

}  // namespace z8
}  // namespace libasm

#endif  // __ASM_Z8_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

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

private:
    const BoolOption<AsmZ8> _opt_reg_alias;
    const IntOption<AsmZ8> _opt_setrp;
    const IntOption<AsmZ8> _opt_setrp0;
    const IntOption<AsmZ8> _opt_setrp1;

    bool _reg_alias;
    int16_t _regPointer0;
    int16_t _regPointer1;

    bool isWorkReg(uint8_t regAddr) const;
    Error setRp(StrScanner &scan, IntOption<AsmZ8>::Setter setter);

    struct Operand;
    Error parseOperand(StrScanner &scan, Operand &op) const;

    void encodeRegImmOperand(AsmInsn &insn, const AddrMode mode, const Operand &op);
    void encodeAbsolute(AsmInsn &insn, const Operand &dstOp, const Operand &srcOp);
    void encodeRelative(AsmInsn &insn, const Operand &op);
    void encodeIndexed(AsmInsn &insn, const Operand &dstOp, const Operand &srcOp);
    void encodeIndirectRegPair(AsmInsn &insn, const Operand &dstOp, const Operand &srcOp);
    void encodeInOpCode(AsmInsn &insn, const Operand &dstOp, const Operand &srcOp);
    void encodeMultiOperands(
            AsmInsn &insn, const Operand &dstOp, const Operand &srcOp, const Operand &extOp);
    void encodePostByte(
            AsmInsn &insn, const Operand &dstOp, const Operand &srcOp, const Operand &extOp);
    void encodeRelative(AsmInsn &insn, OprPos pos, const Operand &op);
    void encodeIndexed(AsmInsn &insn, AddrMode mode, OprPos pos, const Operand &srcOp);
    void encodeRegAddr(AsmInsn &insn, OprPos pos, const Operand &op);
    void encodeImmediate(AsmInsn &insn, AddrMode mode, OprPos pos, const Operand &op);
    void encodeOperand(AsmInsn &insn, AddrMode mode, OprPos pos, const Operand &op);

    Error processPseudo(StrScanner &scan, Insn &insn) override;
    Error encodeImpl(StrScanner &scan, Insn &insn) override;
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

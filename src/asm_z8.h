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

class AsmZ8 final : public Assembler, public Config {
public:
    AsmZ8();

    const ConfigBase &config() const override { return *this; }
    void reset() override;

    Error setRegPointer(int32_t rp);
    Error setRegPointer0(int32_t rp);
    Error setRegPointer1(int32_t rp);

private:
    const ZilogNumberParser _number;
    const SemicolonCommentParser _comment;
    const SimpleSymbolParser _symbol{SymbolParser::DOLLAR_DOT_QUESTION_UNDER};
    const ZilogLetterParser _letter;
    const DollarLocationParser _location;
    const IntOption<AsmZ8> _opt_setrp;
    const IntOption<AsmZ8> _opt_setrp0;
    const IntOption<AsmZ8> _opt_setrp1;

    struct PseudoZ8 : PseudoBase {
        Error processPseudo(StrScanner &scan, Insn &insn, Assembler *assembler) override;

    private:
        Error setRp(StrScanner &scan, AsmZ8 *asmZ8, IntOption<AsmZ8>::Setter setter) const;
    } _pseudos;

    int16_t _regPointer0;
    int16_t _regPointer1;

    bool isWorkReg(uint8_t regAddr) const;

    struct Operand;
    Error parseOperand(StrScanner &scan, Operand &op) const;

    void encodeOperand(InsnZ8 &insn, const AddrMode mode, const Operand &op);
    void encodeAbsolute(InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp);
    void encodeRelative(InsnZ8 &insn, const Operand &op);
    void encodeIndexed(InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp);
    void encodeIndirectRegPair(InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp);
    void encodeInOpCode(InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp);
    void encodeMultiOperands(
            InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp, const Operand &extOp);
    void encodePostByte(
            InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp, const Operand &extOp);

    Error encodeImpl(StrScanner &scan, Insn &insn) override;
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

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

#ifndef __ASM_NS32000_H__
#define __ASM_NS32000_H__

#include "asm_base.h"
#include "config_ns32000.h"
#include "insn_ns32000.h"
#include "reg_ns32000.h"

namespace libasm {
namespace ns32000 {

class AsmNs32000 : public Assembler, public Config {
public:
    AsmNs32000();

    const ConfigBase &config() const override { return *this; }
    void reset() override;

    Error setFpu(StrScanner &scan);
    Error setPmmu(StrScanner &scan);

private:
    ValueParser _parser;
    const NationalNumberParser _number{/*'X' or 'H'*/ 0, 'B', /*'O' or*/ 'Q'};
    const SharpCommentParser _comment;
    const SimpleSymbolParser _symbol{SymbolParser::DOT_UNDER};
    const CStyleLetterParser _letter;
    const NationalLocationParser _location{'*'};
    const TextOption<AsmNs32000> _opt_fpu;
    const TextOption<AsmNs32000> _opt_pmmu;

    struct PseudoNs32000 : PseudoBase {
        Error processPseudo(StrScanner &scan, Insn &insn, Assembler *assembler) override;
    } _pseudos;

    struct Operand;
    Error parseStrOptNames(StrScanner &scan, Operand &op, bool braket = false) const;
    Error parseConfigNames(StrScanner &scan, Operand &op) const;
    Error parseRegisterList(StrScanner &scan, Operand &op) const;
    Error parseBaseOperand(StrScanner &scan, Operand &op);
    Error parseOperand(StrScanner &scan, Operand &op);
    void emitDisplacement(
            InsnNs32000 &insn, const Operand &op, int32_t val32, Error error = OVERFLOW_RANGE);
    void emitLength(InsnNs32000 &insn, AddrMode mode, const Operand &op);
    void emitBitField(InsnNs32000 &insn, AddrMode mode, const Operand &off, const Operand &len);
    void emitImmediate(InsnNs32000 &insn, const Operand &op, OprSize size);
    void emitIndexByte(InsnNs32000 &insn, const Operand &op) const;
    uint8_t encodeGenericField(AddrMode mode, RegName reg) const;
    void emitGeneric(InsnNs32000 &insn, AddrMode mode, const Operand &op, OprPos pos);
    void emitRelative(InsnNs32000 &insn, const Operand &op);
    void emitOperand(InsnNs32000 &insn, AddrMode mode, OprSize size, const Operand &op, OprPos pos,
            const Operand &prevOp);

    Error encodeImpl(StrScanner &scan, Insn &insn) override;
};

}  // namespace ns32000
}  // namespace libasm

#endif  // __ASM_NS32000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

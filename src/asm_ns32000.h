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
#include "table_ns32000.h"

namespace libasm {
namespace ns32000 {

class AsmNs32000 : public Assembler, public Config {
public:
    AsmNs32000()
        : Assembler(_parser, TableNs32000::TABLE, _pseudos),
          _parser(_number, _comment, _symbol, _letter, _location),
          _pseudos() {
        reset();
    }

    const ConfigBase &config() const override { return *this; }
    void reset() override { TableNs32000::TABLE.reset(); }
    const Options &options() const override { return _options; }

private:
    ValueParser _parser;
    const NationalNumberParser _number{/*'X' or 'H'*/ 0, 'B', /*'O' or*/ 'Q'};
    const SharpCommentParser _comment;
    const SimpleSymbolParser _symbol{SymbolParser::DOT_UNDER};
    const CStyleLetterParser _letter;
    const NationalLocationParser _location{'*'};
    struct PseudoNs32000 : PseudoBase {
        Error processPseudo(StrScanner &scan, Insn &insn, Assembler &assembler) override;

        Error setFpu(const StrScanner &scan) const;
        Error setPmmu(const StrScanner &scan) const;
    } _pseudos;

    const struct OptPmmu : public OptionBase {
        OptPmmu(PseudoNs32000 &pseudos);
        Error set(StrScanner &scan) const override;
        PseudoNs32000 &_pseudos;
    } _opt_pmmu{_pseudos};
    const struct OptFpu : public OptionBase {
        OptFpu(PseudoNs32000 &pseudos, const OptionBase &next);
        Error set(StrScanner &scan) const override;
        PseudoNs32000 &_pseudos;
    } _opt_fpu{_pseudos, _opt_pmmu};
    const Options _options{_opt_fpu};

    struct Operand : public OperandBase {
        AddrMode mode;
        RegName reg;
        uint32_t val32;
        uint32_t disp2;
        double float64;
        RegName index;
        OprSize size;
        Operand()
            : mode(M_NONE),
              reg(REG_UNDEF),
              val32(0),
              disp2(0),
              float64(0),
              index(REG_UNDEF),
              size(SZ_NONE) {}
    };

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

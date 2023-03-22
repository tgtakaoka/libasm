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
#include "reg_z8.h"
#include "table_z8.h"

namespace libasm {
namespace z8 {

class AsmZ8 : public Assembler, public Config {
public:
    AsmZ8() : Assembler(_parser, TableZ8::TABLE, _pseudos), _parser(), _pseudos() { reset(); }

    const ConfigBase &config() const override { return *this; }
    void reset() override { _pseudos.setRegPointer(-1); }
    const Options &options() const override { return _options; }

private:
    IntelValueParser _parser;
    struct PseudoZ8 : PseudoBase {
        Error processPseudo(StrScanner &scan, Insn &insn, Assembler &assembler) override;

        bool isWorkReg(uint8_t regAddr) const;
        Error setRegPointer(int32_t rp);
        Error setRegPointer0(int32_t rp);
        Error setRegPointer1(int32_t rp);

    private:
        Error setRp(StrScanner &scan, Assembler &assembler, Error (AsmZ8::PseudoZ8::*)(int32_t));

        int16_t _regPointer0;
        int16_t _regPointer1;
    } _pseudos;

    struct OptSetrp1 : public IntOptionBase {
        OptSetrp1(PseudoZ8 &pseudos);
        void set(int32_t value) const override { _pseudos.setRegPointer1(value); }
        PseudoZ8 &_pseudos;
    } _opt_setrp1{_pseudos};
    struct OptSetrp0 : public IntOptionBase {
        OptSetrp0(PseudoZ8 &pseudos, const OptionBase &next);
        void set(int32_t value) const override { _pseudos.setRegPointer0(value); }
        PseudoZ8 &_pseudos;
    } _opt_setrp0{_pseudos, _opt_setrp1};
    struct OptSetrp : public IntOptionBase {
        OptSetrp(PseudoZ8 &pseudos, const OptionBase &next);
        void set(int32_t value) const override { _pseudos.setRegPointer(value); }
        PseudoZ8 &_pseudos;
    } _opt_setrp{_pseudos, _opt_setrp0};
    const Options _options{_opt_setrp};

    struct Operand : public OperandBase {
        AddrMode mode;
        RegName reg;
        CcName cc;
        uint16_t val16;
        Operand() : mode(M_NONE), reg(REG_UNDEF), cc(CC_UNDEF), val16(0) {}
    };

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

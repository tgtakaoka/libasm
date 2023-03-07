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
    AsmNs32000() : Assembler(_parser, TableNs32000::TABLE), _parser('*') { reset(); }

    const ConfigBase &config() const override { return *this; }
    void reset() override { TableNs32000::TABLE.reset(); }
    const Options &options() const override { return _options; }

private:
    NationalValueParser _parser;
    struct OptCoprocessor : public OptionBase {
        OptCoprocessor(const /*PROGMEM*/ char *name_P, const /*PROGMEM*/ char *desc_P,
                Error (*set)(const StrScanner &))
            : OptionBase(name_P, desc_P, OPT_TEXT), _set(set) {}
        OptCoprocessor(const /*PROGMEM*/ char *name_P, const /*PROGMEM*/ char *desc_P,
                Error (*set)(const StrScanner &), const OptionBase &next)
            : OptionBase(name_P, desc_P, OPT_TEXT, next), _set(set) {}
        Error set(StrScanner &scan) const override { return (*_set)(scan); }
        Error (*_set)(const StrScanner &);
    };
    const OptCoprocessor _opt_pmmu{OPT_TEXT_PMMU, OPT_DESC_PMMU, &AsmNs32000::setPmmu};
    const OptCoprocessor _opt_fpu{OPT_TEXT_FPU, OPT_DESC_FPU, &AsmNs32000::setFpu, _opt_pmmu};
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

    static Error setFpu(const StrScanner &scan);
    static Error setPmmu(const StrScanner &scan);

    Error parseStrOptNames(StrScanner &scan, Operand &op, bool braket = false) const;
    Error parseConfigNames(StrScanner &scan, Operand &op) const;
    Error parseRegisterList(StrScanner &scan, Operand &op) const;
    Error parseBaseOperand(StrScanner &scan, Operand &op);
    Error parseOperand(StrScanner &scan, Operand &op);
    void emitDisplacement(
            InsnNs32000 &insn, const Operand &op, uint32_t val32, Error error = OVERFLOW_RANGE);
    void emitLength(InsnNs32000 &insn, AddrMode mode, const Operand &op);
    void emitBitField(InsnNs32000 &insn, AddrMode mode, const Operand &off, const Operand &len);
    void emitImmediate(InsnNs32000 &insn, const Operand &op, OprSize size);
    void emitIndexByte(InsnNs32000 &insn, const Operand &op) const;
    uint8_t encodeGenericField(AddrMode mode, RegName reg) const;
    void emitGeneric(InsnNs32000 &insn, AddrMode mode, const Operand &op, OprPos pos);
    void emitRelative(InsnNs32000 &insn, const Operand &op);
    void emitOperand(InsnNs32000 &insn, AddrMode mode, OprSize size, const Operand &op, OprPos pos,
            const Operand &prevOp);

    Error processPseudo(StrScanner &scan, Insn &insn) override;;
    Error encodeImpl(StrScanner &scan, Insn &insn) override;

    static const char OPT_TEXT_FPU[] PROGMEM;
    static const char OPT_DESC_FPU[] PROGMEM;
    static const char OPT_TEXT_PMMU[] PROGMEM;
    static const char OPT_DESC_PMMU[] PROGMEM;
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

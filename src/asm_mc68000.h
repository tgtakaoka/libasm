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

#ifndef __ASM_MC68000_H__
#define __ASM_MC68000_H__

#include "asm_base.h"
#include "config_mc68000.h"
#include "insn_mc68000.h"
#include "reg_mc68000.h"
#include "table_mc68000.h"

namespace libasm {
namespace mc68000 {

class AsmMc68000 : public Assembler, public Config {
public:
    AsmMc68000() : Assembler(_parser, TableMc68000::TABLE), _parser() { reset(); }

    const ConfigBase &config() const override { return *this; }
    void reset() override { setAlias(true); }
    const Options &options() const override { return _options; }

private:
    MotorolaValueParser _parser;
    const struct OptAlias : public BoolOptionBase {
        OptAlias(AsmMc68000 *assembler)
            : BoolOptionBase(OPT_BOOL_ALIAS, OPT_DESC_ALIAS), _assembler(assembler) {}
        Error set(bool value) const override {
            _assembler->setAlias(value);
            return OK;
        }
        AsmMc68000 *_assembler;
    } _opt_alias{this};
    const Options _options{_opt_alias};

    struct Operand : public OperandBase {
        AddrMode mode;
        RegName reg;
        RegName indexReg;
        OprSize indexSize;
        uint32_t val32;
        StrScanner list;
        Operand()
            : mode(M_NONE), reg(REG_UNDEF), indexReg(REG_UNDEF), indexSize(SZ_NONE), val32(0), list() {}
        Config::uintptr_t offset(const InsnMc68000 &insn) const;
    };

    Error parseOperand(StrScanner &scan, Operand &op) const;
    Error checkAlignment(OprSize size, const Operand &op);

    void emitBriefExtension(InsnMc68000 &insn, const Operand &op, Config::ptrdiff_t disp);
    void emitDisplacement(InsnMc68000 &insn, const Operand &op, Config::ptrdiff_t disp);
    void emitRelativeAddr(InsnMc68000 &insn, AddrMode mode, const Operand &op);
    void emitImmediateData(InsnMc68000 &insn, const Operand &op, OprSize size, uint32_t data);
    void emitRegisterList(InsnMc68000 &insn, const Operand &op, bool reverse = false);
    Error emitEffectiveAddr(
            InsnMc68000 &insn, OprSize size, const Operand &op, AddrMode mode, OprPos pos);
    Error encodeImpl(StrScanner &scan, Insn &insn) override;

    void setAlias(bool enable) { TableMc68000::TABLE.setAlias(enable); }

    static const char OPT_BOOL_ALIAS[] PROGMEM;
    static const char OPT_DESC_ALIAS[] PROGMEM;
};

}  // namespace mc68000
}  // namespace libasm

#endif  // __ASM_MC68000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

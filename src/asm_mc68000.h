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
    AsmMc68000() : Assembler(_parser, TableMc68000), _parser() { reset(); }

    const ConfigBase &config() const override { return *this; }
    void reset() override { setAlias(true); }

    static const char OPT_BOOL_ALIAS[] PROGMEM;

private:
    MotorolaValueParser _parser;
    const struct OptAlias : public BoolOptionBase {
        OptAlias(AsmMc68000 *assembler)
            : BoolOptionBase(OPT_BOOL_ALIAS, assembler->_options), _assembler(assembler) {}
        Error set(bool value) const override {
            _assembler->setAlias(value);
            return OK;
        }
        AsmMc68000 *_assembler;
    } _opt_alias{this};

    struct Operand : public ErrorAt {
        AddrMode mode;
        RegName reg;
        RegName indexReg;
        OprSize indexSize;
        uint32_t val32;
        Operand()
            : ErrorAt(),
              mode(M_NONE),
              reg(REG_UNDEF),
              indexReg(REG_UNDEF),
              indexSize(SZ_NONE),
              val32(0) {}
        void fixupMultiRegister();
        Config::uintptr_t offset(const InsnMc68000 &insn) const;
    };

    Error parseMoveMultiRegList(StrScanner &scan, Operand &op);
    Error parseOperand(StrScanner &scan, Operand &op);
    Error checkAlignment(OprSize size, const Operand &op);

    Error emitBriefExtension(
            InsnMc68000 &insn, RegName index, OprSize size, Config::ptrdiff_t disp);
    Error emitDisplacement(InsnMc68000 &insn, Config::ptrdiff_t disp);
    Error emitRelativeAddr(InsnMc68000 &insn, AddrMode mode, const Operand &op);
    Error emitImmediateData(InsnMc68000 &insn, OprSize size, uint32_t data);
    Error emitEffectiveAddr(
            InsnMc68000 &insn, OprSize size, const Operand &op, AddrMode mode, OprPos pos);
    Error encode(StrScanner &scan, Insn &insn) override;

    void setAlias(bool enable) { TableMc68000.setAlias(enable); }
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

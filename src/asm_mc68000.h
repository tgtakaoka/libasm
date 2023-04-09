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

namespace libasm {
namespace mc68000 {

class AsmMc68000 final : public Assembler, public Config {
public:
    AsmMc68000();

    const ConfigBase &config() const override { return *this; }
    void reset() override;

    Error setAlias(bool enable);

private:
    const MotorolaNumberParser _number;
    const AsteriskCommentParser _comment;
    const SimpleSymbolParser _symbol{SymbolParser::DOT, SymbolParser::DOLLAR_DOT_UNDER};
    const MotorolaLetterParser _letter{/*closingQuote*/ true};
    const AsteriskLocationParser _location;
    const BoolOption<AsmMc68000> _opt_alias;

    struct Operand;
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

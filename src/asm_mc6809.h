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

#ifndef __ASM_MC6809_H__
#define __ASM_MC6809_H__

#include "asm_base.h"
#include "config_mc6809.h"
#include "insn_mc6809.h"

namespace libasm {
namespace mc6809 {

class AsmMc6809 : public Assembler, public Config {
public:
    AsmMc6809();

    const ConfigBase &config() const override { return *this; }
    void reset() override;

    Error setDirectPage(int32_t val);

private:
    ValueParser _parser;
    const MotorolaNumberParser _number;
    const AsteriskCommentParser _comment;
    const SimpleSymbolParser _symbol{SymbolParser::DOT, SymbolParser::DOLLAR_DOT_UNDER};
    const MotorolaLetterParser _letter;
    const AsteriskLocationParser _location;
    const Mc68xxOperatorParser _operators;
    const IntOption<AsmMc6809> _opt_setdp;

    struct PseudoMc6809 : PseudoBase {
        Error processPseudo(StrScanner &scan, Insn &insn, Assembler *assembler) override;
    } _pseudos;
    uint8_t _direct_page;

    bool onDirectPage(Config::uintptr_t addr) const;
    
    struct Operand;
    bool parseBitPosition(StrScanner &scan, Operand &op) const;
    bool parseMemBit(StrScanner &scan, Operand &op) const;
    Error parseOperand(StrScanner &scan, Operand &op, AddrMode hint) const;

    void encodeRegisterList(InsnMc6809 &insn, const Operand &op);
    void encodeRegisterPair(InsnMc6809 &insn, const Operand &op);
    void encodeRelative(InsnMc6809 &insn, const Operand &op, AddrMode mode);
    Config::ptrdiff_t calculateDisplacement(const InsnMc6809 &insn, const Operand &op) const;
    void encodeIndexed(InsnMc6809 &insn, const Operand &op);
    char transferMemoryMode(Operand &op) const;
    void encodeTransferMemory(InsnMc6809 &insn, Operand &op1, Operand &op2);
    void encodeOperand(InsnMc6809 &insn, const Operand &op, AddrMode mode);

    Error encodeImpl(StrScanner &scan, Insn &insn) override;
};

}  // namespace mc6809
}  // namespace libasm

#endif  // __ASM_MC6809_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

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

#ifndef __ASM_INS8070_H__
#define __ASM_INS8070_H__

#include "asm_base.h"
#include "config_ins8070.h"
#include "insn_ins8070.h"

namespace libasm {
namespace ins8070 {

class AsmIns8070 : public Assembler, public Config {
public:
    AsmIns8070();

    const ConfigBase &config() const override { return *this; }

private:
    ValueParser _parser;
    const NationalNumberParser _number{'X'};
    const SemicolonCommentParser _comment;
    const SimpleSymbolParser _symbol{SymbolParser::DOLLAR, SymbolParser::NONE};
    const DefaultLetterParser _letter;
    const NationalLocationParser _location{'$'};
    const struct Ins8070FunctionParser : FunctionParser {
        const Functor *parseFunction(StrScanner &scan, ErrorAt &error) const override;
    } _function;
    PseudoBase _pseudos;

    struct Operand;
    Error parseOperand(StrScanner &scan, Operand &op) const;

    void emitAbsolute(InsnIns8070 &insn, const Operand &op);
    void emitImmediate(InsnIns8070 &insn, const Operand &op);
    void emitRelative(InsnIns8070 &insn, const Operand &op);
    void emitGeneric(InsnIns8070 &insn, const Operand &op);
    void emitOperand(InsnIns8070 &insn, AddrMode mode, const Operand &op);
    Error encodeImpl(StrScanner &scan, Insn &insn) override;
};

}  // namespace ins8070
}  // namespace libasm

#endif  // __ASM_INS8070_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

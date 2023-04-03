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

#ifndef __ASM_MC6800_H__
#define __ASM_MC6800_H__

#include "asm_base.h"
#include "config_mc6800.h"
#include "insn_mc6800.h"
#include "reg_mc6800.h"
#include "table_mc6800.h"

namespace libasm {
namespace mc6800 {

class AsmMc6800 : public Assembler, public Config {
public:
    AsmMc6800()
        : Assembler(_parser, TableMc6800::TABLE, _pseudos),
          _parser(_number, _comment, _symbol, _letter, _location),
          _pseudos() {}

    const ConfigBase &config() const override { return *this; }

private:
    ValueParser _parser;
    const MotorolaNumberParser _number;
    const AsteriskCommentParser _comment;
    const SimpleSymbolParser _symbol{SymbolParser::DOT, SymbolParser::DOLLAR_DOT_UNDER};
    const MotorolaLetterParser _letter;
    const AsteriskLocationParser _location;
    struct PseudoBase _pseudos;

    struct Operand : public OperandBase {
        AddrMode mode;
        uint8_t size;
        uint16_t val16;
        Operand() : mode(M_NONE), size(0), val16(0) {}
    };

    Error parseOperand(StrScanner &scan, Operand &op) const;
    void emitRelative(InsnMc6800 &insn, const Operand &op);
    void emitImmediate(InsnMc6800 &insn, AddrMode mode, const Operand &op);
    void emitBitNumber(InsnMc6800 &insn, const Operand &op);
    void emitOperand(InsnMc6800 &insn, AddrMode mode, const Operand &op);
    Error encodeImpl(StrScanner &scan, Insn &insn) override;
};

}  // namespace mc6800
}  // namespace libasm

#endif  // __ASM_MC6800_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

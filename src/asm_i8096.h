/*
 * Copyright 2022 Tadashi G. Takaoka
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

#ifndef __ASM_I8096_H__
#define __ASM_I8096_H__

#include "asm_base.h"
#include "config_i8096.h"
#include "insn_i8096.h"
#include "reg_i8096.h"
#include "table_i8096.h"

namespace libasm {
namespace i8096 {

class AsmI8096 : public Assembler, public Config {
public:
    AsmI8096()
        : Assembler(_parser, TableI8096::TABLE, _pseudos),
          _parser(_number, _comment, _symbol, _letter, _location),
          _pseudos() {}

    const ConfigBase &config() const override { return *this; }

private:
    ValueParser _parser;
    const IntelNumberParser _number;
    const SemicolonCommentParser _comment;
    const DefaultSymbolParser _symbol;
    const DefaultLetterParser _letter;
    const DollarLocationParser _location;
    PseudoBase _pseudos;

    struct Operand : public OperandBase {
        AddrMode mode;
        uint8_t regno;
        Error regerr;
        uint16_t val16;
        Operand() : mode(M_NONE), regno(0), regerr(OK), val16(0) {}
    };

    Error parseIndirect(StrScanner &scan, Operand &opr) const;
    Error parseOperand(StrScanner &scan, Operand &opr) const;
    void emitAop(InsnI8096 &insn, AddrMode mode, const Operand &op);
    void emitRelative(InsnI8096 &insn, AddrMode mode, const Operand &op);
    void emitOperand(InsnI8096 &insn, AddrMode mode, const Operand &op);
    Error encodeImpl(StrScanner &scan, Insn &insn) override;
};

}  // namespace i8096
}  // namespace libasm

#endif  // __ASM_I8096_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

/*
 * Copyright 2021 Tadashi G. Takaoka
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

#ifndef __ASM_TMS32010_H__
#define __ASM_TMS32010_H__

#include "asm_base.h"
#include "config_tms32010.h"
#include "insn_tms32010.h"

namespace libasm {
namespace tms32010 {

class AsmTms32010 final : public Assembler, public Config {
public:
    AsmTms32010();

    const ConfigBase &config() const override { return *this; }

private:
    const IntelNumberParser _number;
    const AsteriskCommentParser _comment;
    const SimpleSymbolParser _symbol{SymbolParser::DOLLAR_UNDER};
    const DefaultLetterParser _letter;
    const DollarLocationParser _location;

    struct Operand;
    Error parseOperand(StrScanner &scan, Operand &op) const;

    void encodeOperand(InsnTms32010 &insn, const Operand &op, AddrMode mode);
    Error encodeImpl(StrScanner &scan, Insn &insn) override;
};

}  // namespace tms32010
}  // namespace libasm

#endif  // __ASM_TMS32010_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

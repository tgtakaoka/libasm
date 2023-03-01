/*
 * Copyright 2023 Tadashi G. Takaoka
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

#ifndef __ASM_F3850_H__
#define __ASM_F3850_H__

#include "asm_base.h"
#include "config_f3850.h"
#include "insn_f3850.h"
#include "reg_f3850.h"
#include "table_f3850.h"

namespace libasm {
namespace f3850 {

class AsmF3850 : public Assembler, public Config {
public:
    AsmF3850() : Assembler(_parser, TableF3850::TABLE), _parser() {}

    const ConfigBase &config() const override { return *this; }

private:
    FairchildValueParser _parser;
    RegF3850 _regs;

    struct Operand : public OperandBase {
        AddrMode mode;
        uint16_t val16;
        Operand() : mode(M_NONE), val16(0) {}
    };

    Error parseOperand(StrScanner &scan, Operand &op) const;

    void emitImmediate(InsnF3850 &insn, const Operand &op, AddrMode mode);
    void emitRelative(InsnF3850 &insn, const Operand &op);
    void encodeOperand(InsnF3850 &insn, const Operand &op, AddrMode mode);
    Error encodeImpl(StrScanner &scan, Insn &insn) override;
};

}  // namespace f3850
}  // namespace libasm

#endif  // __ASM_F3850_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
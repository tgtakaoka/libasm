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

#ifndef __ASM_TMS9900_H__
#define __ASM_TMS9900_H__

#include "asm_base.h"
#include "config_tms9900.h"
#include "insn_tms9900.h"
#include "reg_tms9900.h"
#include "table_tms9900.h"

namespace libasm {
namespace tms9900 {

class AsmTms9900 : public Assembler, public Config {
public:
    AsmTms9900() : Assembler(_parser, TableTms9900::TABLE, _pseudos), _parser(_commentParser), _pseudos() {}

    const ConfigBase &config() const override { return *this; }

private:
    IntelValueParser _parser;
    const struct : CommentParser {
        bool commentLine(const StrScanner &scan) const override {
            return *scan == '*' || endOfLine(scan);
        }
        bool endOfLine(const StrScanner &scan) const override { return *scan == 0 || *scan == ';'; }
    } _commentParser;
    PseudoBase _pseudos;

    struct Operand : public OperandBase {
        AddrMode mode;
        RegName reg;
        uint16_t val16;
        Operand() : mode(M_NONE), reg(REG_UNDEF), val16(0) {}
    };

    Error parseOperand(StrScanner &scan, Operand &op) const;

    void encodeRelative(InsnTms9900 &insn, const Operand &op);
    void encodeCruOffset(InsnTms9900 &insn, const Operand &op);
    void encodeModeReg(InsnTms9900 &insn, const Operand &op, AddrMode mode);
    void encodeOperand(InsnTms9900 &insn, const Operand &op, AddrMode mode);
    Error encodeImpl(StrScanner &scan, Insn &insn) override;
};

}  // namespace tms9900
}  // namespace libasm

#endif  // __ASM_TMS9900_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

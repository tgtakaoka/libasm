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
#include "reg_ins8070.h"
#include "table_ins8070.h"

namespace libasm {
namespace ins8070 {

class AsmIns8070
    : public Assembler,
      public Config {
public:
    ValueParser *getParser() override { return &_parser; }

    // Config
    const char *listCpu() const override { return TableIns8070.listCpu(); }
    bool setCpu(const char *cpu) override { return TableIns8070.setCpu(cpu); }
    const char *getCpu() const override { return TableIns8070.getCpu(); }

private:
    class _ValueParser : public ValueParser {
    protected:
        bool isCurrentOriginSymbol(char c) const override { return c == '$'; }
    } _parser;
    RegIns8070 _regs;

    struct Operand : public ErrorReporter {
        OprFormat dst;
        OprFormat src;
        RegName reg;
        bool autoIndex;
        bool hasVal;
        uint16_t val;
    };

    Error nextToken(Operand &op, OprFormat &opr);
    Error parseOperand(Operand &op);

    Error encodeImplied(InsnIns8070 &insn, const Operand &op);
    Error encodeAbsolute(InsnIns8070 &insn, const Operand &op);
    Error encodeImmediate(InsnIns8070 &insn, const Operand &op);
    Error encodeRelative(InsnIns8070 &insn, const Operand &op);
    Error encodeIndexed(InsnIns8070 &insn, const Operand &op);
    Error encodeGeneric(InsnIns8070 &insn, const Operand &op);
    Error encode(Insn &insn) override;
};

} // namespace ins8070
} // namespace libasm

#endif // __ASM_INS8070_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

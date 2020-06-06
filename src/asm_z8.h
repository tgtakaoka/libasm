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

#ifndef __ASM_Z8_H__
#define __ASM_Z8_H__

#include "asm_base.h"
#include "config_z8.h"
#include "insn_z8.h"
#include "reg_z8.h"
#include "table_z8.h"

namespace libasm {
namespace z8 {

class AsmZ8
    : public Assembler,
      public Config {
public:
    ValueParser *getParser() override { return &_parser; }

    // Config
    const char *listCpu() const override { return TableZ8.listCpu(); }
    bool setCpu(const char *cpu) override { return TableZ8.setCpu(cpu); }
    const char *getCpu() const override { return TableZ8.getCpu(); }

    void reset() override { _register_pointer = 0; }
    Error setRegisterPointer(uint8_t rp);

private:
    IntelValueParser _parser;
    RegZ8 _regs;
    uint8_t _register_pointer = 0;

    struct Operand : public ErrorReporter {
        AddrMode mode;
        RegName reg;
        CcName  cc;
        uint16_t val;
    };

    Error parseOperand(Operand &op);

    Error encodeOperand(
        InsnZ8 &insn, const AddrMode mode, const Operand &op);
    Error encodeAbsolute(
        InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp);
    Error encodeRelative(
        InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp);
    Error encodeIndexed(
        InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp);
    Error encodeIndirectRegPair(
        InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp);
    Error encodeInOpCode(
        InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp);
    Error encodeTwoOperands(
        InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp);
    Error processPseudo(InsnZ8 &insn, const char *line);
    Error encode(Insn &insn) override;
};

} // namespace z8
} // namespace libasm

#endif // __ASM_Z8_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

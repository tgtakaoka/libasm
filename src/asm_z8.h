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

    void reset() override { setRegPointer(-1); }
    bool setRegPointer(int16_t rp);
    bool setRegPointer0(int16_t rp0);
    bool setRegPointer1(int16_t rp1);

private:
    IntelValueParser _parser;
    RegZ8 _regs;
    int16_t _regPointer0;
    int16_t _regPointer1;

    TableBase &getTable() const override { return TableZ8; }

    bool isWorkReg(uint8_t regAddr) const;

    struct Operand : public ErrorReporter {
        AddrMode mode;
        RegName reg;
        CcName  cc;
        uint16_t val;
        Operand()
            : ErrorReporter(),
              mode(M_NO),
              reg(REG_UNDEF),
              cc(CC_UNDEF),
              val(0)
        {}
    };

    Error parseOperand(Operand &op);

    Error encodeOperand(
        InsnZ8 &insn, const AddrMode mode, const Operand &op);
    Error encodeAbsolute(
        InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp);
    Error encodeRelative(InsnZ8 &insn, const Operand &op);
    Error encodeIndexed(
        InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp);
    Error encodeIndirectRegPair(
        InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp);
    Error encodeInOpCode(
        InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp);
    Error encodeMultiOperands(
        InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp,
        const Operand &extOp);
    Error encodePostByte(
        InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp,
        const Operand &extOp);
    Error setRp(
        InsnZ8 &insn, const char *line, const char *name,
        bool (AsmZ8::*)(int16_t));
    Error assumeRp(
        const char *line, const char *name, bool (AsmZ8::*)(int16_t));
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

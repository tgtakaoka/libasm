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

#ifndef __ASM_Z8000_H__
#define __ASM_Z8000_H__

#include "asm_base.h"
#include "config_z8000.h"
#include "insn_z8000.h"
#include "reg_z8000.h"
#include "table_z8000.h"

namespace libasm {
namespace z8000 {

class AsmZ8000
    : public Assembler,
      public Config {
public:
    AsmZ8000() : Assembler(_parser, TableZ8000) {}

    bool endOfLine(const char *scan) const override;

    AddressWidth addressWidth() const override { return TableZ8000.addressWidth(); }

private:
    ValueParser _parser;

    struct Operand : public ErrorReporter {
        AddrMode mode;
        RegName reg;    // M_R/M_IR/M_X/M_BX/M_CTL
        RegName base;   // M_BA/M_BX
        CcName cc;      // M_CC
        uint32_t val32; // M_IM/M_DA/M_X/M_BA/M_INTT/M_FLAG
        Operand()
            : ErrorReporter(),
              mode(M_NO),
              reg(REG_UNDEF),
              base(REG_UNDEF),
              cc(CC_UNDEF),
              val32(0)
        {}
    };

    int8_t parseIntrNames(const char *scan);
    int8_t parseFlagNames(const char *scan);
    Error parseOperand(const char *scan, Operand &opr);

    Error emitData(InsnZ8000 &insn, ModeField field, Config::opcode_t data);
    Error emitRegister(InsnZ8000 &insn, ModeField field, RegName reg);
    Error emitIndirectRegister(InsnZ8000 &insn, ModeField field, RegName reg);
    Error emitImmediate(
            InsnZ8000 &insn, ModeField field, AddrMode mode, const Operand &op);
    Error emitDirectAddress(InsnZ8000 &insn, uint32_t addr);
    Error emitRelative(InsnZ8000 &insn, AddrMode mode, const Operand &op);
    Error emitIndexed(InsnZ8000 &insn, ModeField field, const Operand &op);
    Error emitBaseAddress(InsnZ8000 &insn, ModeField field, const Operand &op);
    Error emitBaseIndexed(InsnZ8000 &insn, ModeField field, const Operand &op);
    Error emitFlags(InsnZ8000 &insn, ModeField field, const Operand &op);
    Error emitCtlRegister(InsnZ8000 &insn, ModeField field, const Operand &op);
    Error emitOperand(
            InsnZ8000 &insn, AddrMode mode, const Operand &op, ModeField field);
    Error encode(Insn &insn) override;
};

} // namespace z8000
} // namespace libasm

#endif // __ASM_Z8000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

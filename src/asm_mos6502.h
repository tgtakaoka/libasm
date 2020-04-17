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

#ifndef __ASM_MOS6502_H__
#define __ASM_MOS6502_H__

#include "asm_base.h"
#include "config_mos6502.h"
#include "insn_mos6502.h"
#include "reg_mos6502.h"
#include "table_mos6502.h"

namespace libasm {
namespace mos6502 {

class AsmMos6502
    : public Assembler,
      public Config {
public:
    AsmOperand &getParser() override { return _parser; }

    // Config
    const char *listCpu() const override { return TableMos6502.listCpu(); }
    bool setCpu(const char *cpu) override { return TableMos6502.setCpu(cpu); }

private:
    AsmMotoOperand _parser;
    RegMos6502 _regs;

    struct Operand : public ErrorReporter {
        AddrMode mode;
        uint16_t val16;
    };

    Error selectMode(
        char modifier, Operand &op, /* AddrMode labs,*/ AddrMode abs, AddrMode zp);
    Error parseOperand(Operand &op);

    Error encodeRelative(InsnMos6502 &insn, bool emitInsn);
    Error encodeZeroPageRelative(InsnMos6502 &insn);

    Error encode(Insn &insn) override;
};

} // namespace mos6502
} // namespace libasm

#endif // __ASM_MOS6502_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

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

#ifndef __ASM_M6502_H__
#define __ASM_M6502_H__

#include "config_m6502.h"

#include "insn_m6502.h"
#include "reg_m6502.h"
#include "table_m6502.h"
#include "asm_interface.h"

namespace libasm {
namespace m6502 {

class AsmM6502 : public Assembler<Config> {
public:
    AsmOperand &getParser() override { return _parser; }
    bool setCpu(const char *cpu) override { return TableM6502.setCpu(cpu); }
    const char *listCpu() const override { return TableM6502::listCpu(); }

private:
    AsmMotoOperand _parser;
    RegM6502 _regs;

    struct Operand : public ErrorReporter {
        AddrMode mode;
        uint32_t val32;
    };

    Error selectMode(
        char modifier, Operand &op, AddrMode labs, AddrMode abs, AddrMode zp);
    Error parseOperand(Operand &op);

    Error encodeLongRelative(InsnM6502 &insn);
    Error encodeRelative(InsnM6502 &insn, bool emitInsn);
    Error encodeZeroPageRelative(InsnM6502 &insn);
    Error encodeBlockMove(InsnM6502 &insn);

    Error encode(Insn &insn) override;
};

} // namespace m6502
} // namespace libasm

#endif // __ASM_M6502_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

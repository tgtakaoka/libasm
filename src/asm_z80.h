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

#ifndef __ASM_Z80_H__
#define __ASM_Z80_H__

#include "config_z80.h"

#include "insn_z80.h"
#include "reg_z80.h"
#include "table_z80.h"
#include "asm_interface.h"

class AsmZ80 : public Assembler<target::uintptr_t> {
public:
    AsmOperand &getParser() override { return _parser; }
    bool setCpu(const char *cpu) override { return TableZ80.setCpu(cpu); }
    const char *listCpu() const override { return TableZ80::listCpu(); }

private:
    AsmIntelOperand _parser;
    RegZ80 _regs;

    struct Operand : public ErrorReporter {
        OprFormat format;
        RegName reg;
        uint16_t val;
        OprSize size;
    };

    Error parseOperand(const Insn &insn, Operand &opr);

    Error encodeImmediate(
        Insn &insn, const Operand &left, const Operand &right);
    Error encodeInherent(
        Insn &insn, const Operand &left, const Operand &right);
    Error encodeDirect(
        Insn &insn, const Operand &left, const Operand &right);
    Error encodeIoaddr(
        Insn &insn, const Operand &left, const Operand &right);
    Error encodeRelative(
        Insn &insn, const Operand &left, const Operand &right);
    Error encodeIndexed(
        Insn &insn, const Operand &left, const Operand &right);
    Error encodeIndexedImmediate8(
        Insn &insn, const Operand &left, const Operand &right);

    Error encode(Insn &insn) override;
};

#endif // __ASM_Z80_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

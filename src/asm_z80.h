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

namespace libasm {
namespace z80 {

class AsmZ80 : public Assembler<Config::uintptr_t> {
public:
    AsmOperand &getParser() override { return _parser; }
    bool setCpu(const char *cpu) override { return TableZ80.setCpu(cpu); }
    const char *listCpu() const override { return TableZ80::listCpu(); }
    Endian endian() const override { return Config::endian; }
    host::uint_t maxBytes() const override { return Config::code_max; }
    host::uint_t maxName() const override { return Config::name_max; }

private:
    AsmIntelOperand _parser;
    RegZ80 _regs;

    struct Operand : public ErrorReporter {
        OprFormat format;
        RegName reg;
        uint16_t val;
        OprSize size;
    };

    Error parseOperand(const InsnZ80 &insn, Operand &opr);

    Error encodeImmediate(
        InsnZ80 &insn, const Operand &left, const Operand &right);
    Error encodeInherent(
        InsnZ80 &insn, const Operand &left, const Operand &right);
    Error encodeDirect(
        InsnZ80 &insn, const Operand &left, const Operand &right);
    Error encodeIoaddr(
        InsnZ80 &insn, const Operand &left, const Operand &right);
    Error encodeRelative(
        InsnZ80 &insn, const Operand &left, const Operand &right);
    Error encodeIndexed(
        InsnZ80 &insn, const Operand &left, const Operand &right);
    Error encodeIndexedImmediate8(
        InsnZ80 &insn, const Operand &left, const Operand &right);

    Error encode(Insn<Config::uintptr_t> &insn) override;
};

} // namespace z80
} // namespace libasm

#endif // __ASM_Z80_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

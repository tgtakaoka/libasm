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

#ifndef __ASM_W65C816_H__
#define __ASM_W65C816_H__

#include "asm_base.h"
#include "config_w65c816.h"
#include "insn_w65c816.h"
#include "reg_w65c816.h"
#include "table_w65c816.h"

namespace libasm {
namespace w65c816 {

class AsmW65C816
    : public Assembler,
      public Config {
public:
    ValueParser *getParser() override { return &_parser; }

    // Config
    const char *listCpu() const override { return TableW65C816.listCpu(); }
    bool setCpu(const char *cpu) override { return TableW65C816.setCpu(cpu); }
    const char *getCpu() const override { return TableW65C816.getCpu(); }

    void reset() override { _long_acc = _long_idx = false; }

private:
    MotoValueParser _parser;
    RegW65C816 _regs;
    bool _long_acc = false;
    bool _long_idx = false;

    struct Operand : public ErrorReporter {
        AddrMode mode;
        uint32_t val32;
    };

    Error parseOnOff(const char *line, bool &val);
    Error parseZeroOne(const char *line, bool &val);
    Error selectMode(
        char size, Operand &op, AddrMode labs, AddrMode abs, AddrMode zp);
    Error parseOperand(Operand &op, Operand &extra);

    Error encodeLongRelative(InsnW65C816 &insn, const Operand &op);
    Error encodeRelative(InsnW65C816 &insn, const Operand &op);
    Error encodeZeroPageRelative(
        InsnW65C816 &insn, const Operand &op, const Operand &extra);
    Error processPseudo(InsnW65C816 &insn, const char *line);
    Error encode(Insn &insn) override;
};

} // namespace w65c816
} // namespace libasm

#endif // __ASM_W65C816_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

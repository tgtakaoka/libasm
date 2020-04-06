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

#ifndef __ASM_MC6800_H__
#define __ASM_MC6800_H__

#include "config_mc6800.h"

#include "insn_mc6800.h"
#include "reg_mc6800.h"
#include "table_mc6800.h"
#include "asm_interface.h"

class AsmMc6800 : public Assembler<target::uintptr_t> {
public:
    AsmOperand &getParser() override { return _parser; }
    bool setCpu(const char *cpu) override { return TableMc6800.setCpu(cpu); }
    const char *listCpu() const override { return TableMc6800::listCpu(); }
    Endian endian() const override { return ENDIAN_BIG; }
    host::uint_t maxBytes() const override { return Entry::code_max; }
    host::uint_t maxName() const override { return Entry::name_max; }

private:
    AsmMotoOperand _parser;
    RegMc6800 _regs;

    bool adjustAccumulator(Insn &insn);
    Error determineAddrMode(const char *line, Insn &insn);

    Error encodeInherent(Insn &insn);
    Error encodeDirect(Insn &insn);
    Error encodeExtended(Insn &insn);
    Error encodeIndexed(Insn &insn);
    Error encodeRelative(Insn &insn);
    Error encodeImmediate(Insn &insn);

    Error encode(Insn &insn) override;
};

#endif // __ASM_MC6800_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

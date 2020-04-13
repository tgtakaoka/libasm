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

#include "asm_base.h"
#include "config_mc6800.h"
#include "insn_mc6800.h"
#include "reg_mc6800.h"
#include "table_mc6800.h"

namespace libasm {
namespace mc6800 {

class AsmMc6800
    : public Assembler,
      public Config {
public:
    AsmOperand &getParser() override { return _parser; }

    // Config
    const char *listCpu() const override { return TableMc6800::listCpu(); }
    bool setCpu(const char *cpu) override { return TableMc6800.setCpu(cpu); }

private:
    AsmMotoOperand _parser;
    RegMc6800 _regs;

    bool adjustAccumulator(InsnMc6800 &insn);
    Error determineAddrMode(const char *line, InsnMc6800 &insn);

    Error encodeInherent(InsnMc6800 &insn);
    Error encodeDirect(InsnMc6800 &insn);
    Error encodeExtended(InsnMc6800 &insn);
    Error encodeIndexed(InsnMc6800 &insn);
    Error encodeRelative(InsnMc6800 &insn);
    Error encodeImmediate(InsnMc6800 &insn);

    Error encode(Insn &insn) override;
};

} // namespace mc6800
} // namespace libasm

#endif // __ASM_MC6800_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

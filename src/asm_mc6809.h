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

#ifndef __ASM_MC6809_H__
#define __ASM_MC6809_H__

#include "asm_base.h"
#include "config_mc6809.h"
#include "insn_mc6809.h"
#include "reg_mc6809.h"
#include "table_mc6809.h"

namespace libasm {
namespace mc6809 {

class AsmMc6809
    : public Assembler,
      public Config {
public:
    ValueParser *getParser() override { return &_parser; }

    // Config
    const char *listCpu() const override { return TableMc6809.listCpu(); }
    bool setCpu(const char *cpu) override { return TableMc6809.setCpu(cpu); }
    const char *getCpu() const override { return TableMc6809.getCpu(); }

    void reset() override { _direct_page = 0; }

private:
    MotoValueParser _parser;
    RegMc6809 _regs;
    uint8_t _direct_page;

    Error determineAddrMode(const char *line, InsnMc6809 &insn);

    // MC6809
    Error encodePushPull(InsnMc6809 &insn);
    Error encodeRegisters(InsnMc6809 &insn);
    Error encodeRelative(InsnMc6809 &insn);
    Error encodeImmediate(InsnMc6809 &insn);
    Error encodeDirect(InsnMc6809 &insn, bool emitInsn = true);
    Error encodeExtended(InsnMc6809 &insn, bool emitInsn = true);
    Error encodeIndexed(InsnMc6809 &insn, bool emitInsn = true);
    // MC6809
    Error encodeBitOperation(InsnMc6809 &insn);
    Error encodeImmediatePlus(InsnMc6809 &insn);
    Error encodeTransferMemory(InsnMc6809 &insn);
    // Pseudo instruction
    Error processPseudo(InsnMc6809 &insn, const char *line);
    Error encode(Insn &insn) override;
};

} // namespace mc6809
} // namespace libasm

#endif // __ASM_MC6809_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

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

#include "asm_base.h"
#include "config_z80.h"
#include "insn_z80.h"
#include "reg_z80.h"
#include "table_z80.h"

namespace libasm {
namespace z80 {

class AsmZ80
    : public Assembler,
      public Config {
public:
    ValueParser *getParser() override { return &_parser; }

private:
    IntelValueParser _parser;

    TableBase &getTable() const override { return TableZ80; }

    struct Operand : public ErrorReporter {
        OprFormat format;
        RegName reg;
        uint16_t val;
        OprSize size;
        Operand()
            : ErrorReporter(),
              format(NO_OPR),
              reg(REG_UNDEF),
              val(0),
              size(SZ_NONE)
        {}
    };

    Error parseOperand(Operand &opr);

    Error encodeImmediate(
        InsnZ80 &insn, const Operand &dst, const Operand &src);
    Error encodeInherent(
        InsnZ80 &insn, const Operand &dst, const Operand &src);
    Error encodeDirect(
        InsnZ80 &insn, const Operand &dst, const Operand &src);
    Error encodeIoaddr(
        InsnZ80 &insn, const Operand &dst, const Operand &src);
    Error encodeRelative(
        InsnZ80 &insn, const Operand &dst, const Operand &src);
    Error encodeIndexed(
        InsnZ80 &insn, const Operand &dst, const Operand &src);
    Error encodeIndexedImmediate8(
        InsnZ80 &insn, const Operand &dst, const Operand &src);

    Error encode(Insn &insn) override;
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

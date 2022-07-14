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

class AsmZ80 : public Assembler, public Config {
public:
    AsmZ80() : Assembler(_parser, TableZ80::TABLE), _parser() {}

    const ConfigBase &config() const override { return *this; }

private:
    IntelValueParser _parser;

    struct Operand : public ErrorAt {
        AddrMode mode;
        RegName reg;
        uint16_t val16;
        Operand() : ErrorAt(), mode(M_NO), reg(REG_UNDEF), val16(0) {}
    };

    Error parseOperand(StrScanner &scan, Operand &op);

    Error encodeRelative(InsnZ80 &insn, const Operand &op);
    Error encodeIndexedBitOp(InsnZ80 &insn, const Operand &op);
    Error encodeOperand(InsnZ80 &insn, const Operand &op, AddrMode mode, const Operand &other);
    Error encode(StrScanner &scan, Insn &insn) override;
};

}  // namespace z80
}  // namespace libasm

#endif  // __ASM_Z80_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

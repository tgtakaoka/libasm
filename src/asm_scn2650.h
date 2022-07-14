/*
 * Copyright 2022 Tadashi G. Takaoka
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

#ifndef __ASM_SCN2650_H__
#define __ASM_SCN2650_H__

#include "asm_base.h"
#include "config_scn2650.h"
#include "insn_scn2650.h"
#include "reg_scn2650.h"
#include "table_scn2650.h"

namespace libasm {
namespace scn2650 {

class AsmScn2650 : public Assembler, public Config {
public:
    AsmScn2650() : Assembler(_parser, TableScn2650::TABLE), _parser() {}

    const ConfigBase &config() const override { return *this; }

private:
    IntelValueParser _parser;
    RegScn2650 _regs;

    struct Operand : public ErrorAt {
        AddrMode mode;
        RegName reg;
        CcName cc;
        bool indir;
        char sign;
        uint16_t val16;
        Operand()
            : ErrorAt(),
              mode(NONE),
              reg(REG_UNDEF),
              cc(CC_UNDEF),
              indir(false),
              sign(0),
              val16(0) {}
    };

    Error parseOperand(StrScanner &scan, Operand &op);

    Error emitAbsolute(InsnScn2650 &insn, const Operand &op, AddrMode mode);
    Error emitIndexed(InsnScn2650 &insn, const Operand &op, AddrMode mode);
    Error emitZeroPage(InsnScn2650 &insn, const Operand &op);
    Error emitRelative(InsnScn2650 &insn, const Operand &op);
    Error encodeOperand(InsnScn2650 &insn, const Operand &op, AddrMode mode);
    Error encode(StrScanner &scan, Insn &insn) override;
};

}  // namespace scn2650
}  // namespace libasm

#endif  // __ASM_SCN2650_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

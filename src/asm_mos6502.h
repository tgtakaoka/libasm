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

#ifndef __ASM_MOS6502_H__
#define __ASM_MOS6502_H__

#include "asm_base.h"
#include "config_mos6502.h"
#include "insn_mos6502.h"
#include "reg_mos6502.h"
#include "table_mos6502.h"

namespace libasm {
namespace mos6502 {

class AsmMos6502 : public Assembler, public Config {
public:
    AsmMos6502() : Assembler(_parser, TableMos6502) {}
    ConfigBase &config() override { return *this; }
    AddressWidth addressWidth() const override { return TableMos6502.addressWidth(); }
    void reset() override { _long_acc = _long_idx = false; }

private:
    MotoValueParser _parser;
    bool _long_acc = false;
    bool _long_idx = false;

    struct Operand : public ErrorReporter {
        AddrMode mode;
        uint32_t val32;
        Operand() : ErrorReporter(), mode(IMPL), val32(0) {}
    };

    Error parseOnOff(const char *scan, bool &val);
    Error processPseudo(const char *scan, InsnMos6502 &insn);
    Error selectMode(char size, Operand &op, AddrMode zp, AddrMode abs, AddrMode labs = IMPL);
    Error parseOperand(const char *scan, Operand &op, Operand &extra);

    Error encodeRelative(InsnMos6502 &insn, AddrMode mode, const Operand &op);
    Error encode(Insn &insn) override;
};

}  // namespace mos6502
}  // namespace libasm

#endif  // __ASM_MOS6502_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

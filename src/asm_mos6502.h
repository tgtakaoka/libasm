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
    AsmMos6502() : Assembler(_parser, TableMos6502::TABLE), _parser() { reset(); }

    const ConfigBase &config() const override { return *this; }
    AddressWidth addressWidth() const override { return TableMos6502::TABLE.addressWidth(); }
    void reset() override { TableMos6502::TABLE.reset(); }

    static const char OPT_BOOL_LONGA[] PROGMEM;
    static const char OPT_BOOL_LONGI[] PROGMEM;

private:
    MotorolaValueParser _parser;
    const struct OptLongA : public BoolOptionBase {
        OptLongA(Options &options) : BoolOptionBase(OPT_BOOL_LONGA, options) {}
        Error set(bool value) const override {
            return TableMos6502::TABLE.setLongAccumulator(value) ? OK : OPERAND_NOT_ALLOWED;
        }
    } _opt_longa{_options};
    const struct OptLongI : public BoolOptionBase {
        OptLongI(Options &options) : BoolOptionBase(OPT_BOOL_LONGI, options) {}
        Error set(bool value) const override {
            return TableMos6502::TABLE.setLongIndex(value) ? OK : OPERAND_NOT_ALLOWED;
        }
    } _opt_longi{_options};

    struct Operand : public ErrorAt {
        AddrMode mode;
        uint32_t val32;
        Operand() : ErrorAt(), mode(IMPL), val32(0) {}
    };

    Error parseTableOnOff(StrScanner &scan, bool (TableMos6502::*set)(bool val));
    Error processPseudo(StrScanner &scan, const char *name);
    Error selectMode(char size, Operand &op, AddrMode zp, AddrMode abs, AddrMode labs = IMPL);
    Error parseOperand(StrScanner &scan, Operand &op, Operand &extra);

    Error encodeRelative(InsnMos6502 &insn, AddrMode mode, const Operand &op);
    Error encode(StrScanner &scan, Insn &insn) override;
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

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

#ifndef __DIS_MOS6502_H__
#define __DIS_MOS6502_H__

#include "config_mos6502.h"
#include "dis_base.h"
#include "insn_mos6502.h"
#include "reg_mos6502.h"
#include "table_mos6502.h"

namespace libasm {
namespace mos6502 {

class DisMos6502 : public Disassembler, public Config {
public:
    DisMos6502() : Disassembler(_formatter, _regs, TableMos6502, '*'), _formatter(), _regs() {
        reset();
    }

    const ConfigBase &config() const override { return *this; }
    AddressWidth addressWidth() const override { return TableMos6502.addressWidth(); }
    void reset() override;

    static const char OPT_BOOL_INDIRECT_LONG[] PROGMEM;
    static const char OPT_BOOL_LONGA[] PROGMEM;
    static const char OPT_BOOL_LONGI[] PROGMEM;

private:
    MotorolaValueFormatter _formatter;
    RegMos6502 _regs;
    const struct OptIndirectLong : public BoolOptionBase{
        OptIndirectLong(Options &options) : BoolOptionBase(OPT_BOOL_INDIRECT_LONG, options) {}
        Error set(bool value) const override {
            TableMos6502.useIndirectLong(value);
            return OK;
        }
    } _opt_indirectLong{_options};
    const struct OptLongA : public BoolOptionBase{
        OptLongA(Options &options) : BoolOptionBase(OPT_BOOL_LONGA, options) {}
        Error set(bool value) const override {
            TableMos6502.longAccumulator(value);
            return OK;
        }
    } _opt_longa{_options};
    const struct OptLongI : public BoolOptionBase{
        OptLongI(Options &options) : BoolOptionBase(OPT_BOOL_LONGI, options) {}
        Error set(bool value) const override {
            TableMos6502.longIndex(value);
            return OK;
        }
    } _opt_longi{_options};

    Error decodeImmediate(DisMemory &memory, InsnMos6502 &insn, StrBuffer &out, AddrMode mode);
    Error decodeAbsolute(DisMemory &memory, InsnMos6502 &insn, StrBuffer &out, AddrMode mode);
    Error decodeZeroPage(DisMemory &memory, InsnMos6502 &insn, StrBuffer &out, AddrMode mode);
    Error decodeRelative(DisMemory &memory, InsnMos6502 &insn, StrBuffer &out, AddrMode mode);
    Error decodeBlockMove(DisMemory &memory, InsnMos6502 &insn, StrBuffer &out);
    Error decode(DisMemory &memory, Insn &insn, StrBuffer &out) override;
};

}  // namespace mos6502
}  // namespace libasm

#endif  // __DIS_MOS6502_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

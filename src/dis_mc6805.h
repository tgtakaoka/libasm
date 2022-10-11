/*
 * Copyright 2021 Tadashi G. Takaoka
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

#ifndef __DIS_MC6805_H__
#define __DIS_MC6805_H__

#include "config_mc6805.h"
#include "dis_base.h"
#include "insn_mc6805.h"
#include "reg_mc6805.h"
#include "table_mc6805.h"

namespace libasm {
namespace mc6805 {

class DisMc6805 : public Disassembler, public Config {
public:
    DisMc6805() : Disassembler(_formatter, _regs, TableMc6805::TABLE, '*'), _formatter(), _regs() {
        reset();
    }

    const ConfigBase &config() const override { return *this; }
    AddressWidth addressWidth() const override { return _addrWidth; }
    void reset() override { _addrWidth = ADDRESS_13BIT; }
    const Options &options() const override { return _options; }

private:
    MotorolaValueFormatter _formatter;
    RegMc6805 _regs;
    AddressWidth _addrWidth;
    const struct OptPcBits : public IntOptionBase {
        OptPcBits(AddressWidth &value)
            : IntOptionBase(OPT_INT_PCBITS, OPT_DESC_PCBITS), _width(value) {}
        Error check(int32_t value) const override {
            return value >= 0 && value <= 16 ? OK : OVERFLOW_RANGE;
        }
        void set(int32_t value) const override { _width = AddressWidth(value ?: 13); }
        AddressWidth &_width;
    } _opt_pc_bits{_addrWidth};
    const Options _options{_opt_pc_bits};

    StrBuffer &outRegister(StrBuffer &out, RegName regName);

    Error checkAddressRange(Config::uintptr_t addr);
    Error decodeDirectPage(DisMemory &memory, InsnMc6805 &insn, StrBuffer &out);
    Error decodeExtended(DisMemory &memory, InsnMc6805 &insn, StrBuffer &out);
    Error decodeIndexed(DisMemory &memory, InsnMc6805 &insn, StrBuffer &out, AddrMode mode);
    Error decodeRelative(DisMemory &memory, InsnMc6805 &insn, StrBuffer &out);
    Error decodeOperand(DisMemory &memory, InsnMc6805 &insn, StrBuffer &out, AddrMode mode);
    Error decodeImpl(DisMemory &memory, Insn &insn, StrBuffer &out) override;

    static const char OPT_INT_PCBITS[] PROGMEM;
    static const char OPT_DESC_PCBITS[] PROGMEM;
};

}  // namespace mc6805
}  // namespace libasm

#endif  // __DIS_MC6805_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

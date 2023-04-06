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

#ifndef __DIS_Z8000_H__
#define __DIS_Z8000_H__

#include "config_z8000.h"
#include "dis_base.h"
#include "insn_z8000.h"
#include "reg_z8000.h"
#include "table_z8000.h"

namespace libasm {
namespace z8000 {

class DisZ8000 : public Disassembler, public Config {
public:
    DisZ8000() : Disassembler(_formatter, _regs, TableZ8000::TABLE, '$'), _formatter(), _regs() {
        reset();
    }

    const ConfigBase &config() const override { return *this; }
    AddressWidth addressWidth() const override { return TableZ8000::TABLE.addressWidth(); }
    void reset() override;
    const Options &options() const override { return _options; }

private:
    IntelValueFormatter _formatter;
    RegZ8000 _regs;
    bool _ioAddressPrefix;
    bool _shortDirect;
    const struct OptIoaddrPrefix : public BoolOption {
        OptIoaddrPrefix(bool &var);
    } _opt_ioaddrPrefix{_ioAddressPrefix};
    const struct OptShortDirect : public BoolOption {
        OptShortDirect(bool &var, const OptionBase &next);
    } _opt_shortDirect{_shortDirect, _opt_ioaddrPrefix};
    const Options _options{_opt_shortDirect};

    StrBuffer &outRegister(StrBuffer &out, RegName regName);
    StrBuffer &outConditionCode(StrBuffer &out, uint8_t ccNum);
    StrBuffer &outImmediate(StrBuffer &out, uint8_t data, AddrMode mode);
    StrBuffer &outComma(StrBuffer &out, const InsnZ8000 &insn, AddrMode mode, ModeField field);
    Error decodeImmediate(
            DisMemory &memory, InsnZ8000 &insn, StrBuffer &out, AddrMode mode, OprSize size);
    Error decodeFlags(StrBuffer &out, uint8_t flags);
    Error decodeGeneralRegister(StrBuffer &out, uint8_t num, OprSize size, bool indirect = false);
    Error decodeDoubleSizedRegister(StrBuffer &out, uint8_t num, OprSize size);
    Error decodeControlRegister(StrBuffer &out, uint8_t ctlNum, OprSize size);
    Error decodeBaseAddressing(
            DisMemory &memory, InsnZ8000 &insn, StrBuffer &out, AddrMode mode, uint8_t num);
    Error decodeGenericAddressing(
            DisMemory &memory, InsnZ8000 &insn, StrBuffer &out, AddrMode mode, uint8_t num);
    Error decodeDirectAddress(DisMemory &memory, InsnZ8000 &insn, StrBuffer &out);
    Error decodeRelativeAddressing(
            DisMemory &memory, InsnZ8000 &insn, StrBuffer &out, AddrMode mode);
    Error decodeOperand(
            DisMemory &memory, InsnZ8000 &insn, StrBuffer &out, AddrMode mode, ModeField field);
    Error checkPostWord(const InsnZ8000 &insn);
    Error checkRegisterOverlap(const InsnZ8000 &insn);
    Error decodeImpl(DisMemory &memory, Insn &insn, StrBuffer &out) override;
};

}  // namespace z8000
}  // namespace libasm

#endif  // __DIS_Z8000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

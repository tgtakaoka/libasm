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

#ifndef __DIS_CDP1802_H__
#define __DIS_CDP1802_H__

#include "config_cdp1802.h"
#include "dis_base.h"
#include "insn_cdp1802.h"
#include "reg_cdp1802.h"
#include "table_cdp1802.h"

namespace libasm {
namespace cdp1802 {

class DisCdp1802 : public Disassembler, public Config {
public:
    DisCdp1802()
        : Disassembler(_formatter, _regs, TableCdp1802::TABLE, '$'), _formatter(), _regs() {
        reset();
    }

    const ConfigBase &config() const override { return *this; }
    void reset() override;
    const Options &options() const override { return _options; }

private:
    IntelValueFormatter _formatter;
    RegCdp1802 _regs;
    bool _useReg;
    const struct OptUseRegister : public BoolOption {
        OptUseRegister(bool &var);
    } _opt_useReg{_useReg};
    const Options _options{_opt_useReg};

    Error decodeOperand(DisMemory &memory, InsnCdp1802 &insn, StrBuffer &out, AddrMode mode);
    Error decodeImpl(DisMemory &memory, Insn &insn, StrBuffer &out) override;
};

}  // namespace cdp1802
}  // namespace libasm

#endif  // __DIS_CDP1802_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

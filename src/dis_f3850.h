/*
 * Copyright 2023 Tadashi G. Takaoka
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

#ifndef __DIS_F3850_H__
#define __DIS_F3850_H__

#include "config_f3850.h"
#include "dis_base.h"
#include "insn_f3850.h"
#include "reg_f3850.h"
#include "table_f3850.h"

namespace libasm {
namespace f3850 {

class DisF3850 : public Disassembler, public Config {
public:
    DisF3850()
        : Disassembler(_formatter, _regs, TableF3850::TABLE, '$'), _formatter(), _regs() {
        reset();
    }

    const ConfigBase &config() const override { return *this; }
    void reset() override { _useScratchpad = false; }
    const Options &options() const override { return _options; }

private:
    FairchildValueFormatter _formatter;
    RegF3850 _regs;
    bool _useScratchpad;
    const BoolOption _opt_useScratchpad{
            OPT_BOOL_USE_SCRATCHPAD, OPT_DESC_USE_SCRATCHPAD, _useScratchpad};
    const Options _options{_opt_useScratchpad};

    Error decodeRelative(DisMemory &memory, InsnF3850 &insn, StrBuffer &out);
    Error decodeOperand(DisMemory &memory, InsnF3850 &insn, StrBuffer &out, AddrMode mode);
    Error decodeImpl(DisMemory &memory, Insn &insn, StrBuffer &out) override;

    static const char OPT_BOOL_USE_SCRATCHPAD[] PROGMEM;
    static const char OPT_DESC_USE_SCRATCHPAD[] PROGMEM;
};

}  // namespace f3850
}  // namespace libasm

#endif  // __DIS_F3850_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

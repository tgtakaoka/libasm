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

#ifndef __DIS_TMS9900_H__
#define __DIS_TMS9900_H__

#include "config_tms9900.h"
#include "dis_base.h"
#include "insn_tms9900.h"
#include "reg_tms9900.h"
#include "table_tms9900.h"

namespace libasm {
namespace tms9900 {

class DisTms9900
    : public Disassembler,
      public Config {
public:
    ValueFormatter &getFormatter() override { return _formatter; }

    // Config
    const char *listCpu() const override { return TableTms9900.listCpu(); }
    bool setCpu(const char *cpu) override { return TableTms9900.setCpu(cpu); }
    const char *getCpu() const override { return TableTms9900.getCpu(); }

private:
    IntelValueFormatter _formatter;
    RegTms9900 _regs;

    RegBase &getRegister() override { return _regs; }

    Error decodeOperand(
        DisMemory &memory, InsnTms9900 &insn, host::uint_t opr);
    Error decodeImmediate(DisMemory &memory, InsnTms9900 &insn);
    Error decodeRelative(InsnTms9900 &insn);
    Error decode(DisMemory &memory, Insn &insn) override;
};

} // namespace tms9900
} // namespace libasm

#endif // __DIS_TMS9900_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

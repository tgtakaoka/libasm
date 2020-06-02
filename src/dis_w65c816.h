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

#ifndef __DIS_W65C816_H__
#define __DIS_W65C816_H__

#include "config_w65c816.h"
#include "dis_base.h"
#include "insn_w65c816.h"
#include "reg_w65c816.h"
#include "table_w65c816.h"

namespace libasm {
namespace w65c816 {

class DisW65C816
    : public Disassembler,
      public Config {
public:
    ValueFormatter &getFormatter() override { return _formatter; }

    // Config
    bool setCpu(const char *cpu) override { return TableW65C816.setCpu(cpu); }
    const char *listCpu() const override { return TableW65C816.listCpu(); }
    const char *getCpu() const override { return TableW65C816.getCpu(); }

    void acceptIndirectLong(bool accept) { _acceptIndirectLong = accept; }
    void longAccumlator(bool on) { _long_acc = on; }
    void longIndex(bool on) { _long_idx = on; }

private:
    MotoValueFormatter _formatter;
    RegW65C816 _regs;
    bool _acceptIndirectLong = true;
    bool _long_acc = false;
    bool _long_idx = false;

    RegBase &getRegister() override { return _regs; }

    Error decodeImmediate(DisMemory &memory, InsnW65C816 &insn);
    Error decodeAbsolute(DisMemory &memory, InsnW65C816 &insn);
    Error decodeZeroPage(DisMemory &memory, InsnW65C816 &insn);
    Error decodeRelative(DisMemory &memory, InsnW65C816 &insn);
    Error decodeBlockMove(DisMemory &memory, InsnW65C816 &insn);
    Error decode(DisMemory &memory, Insn &insn) override;
};

} // namespace w65c816
} // namespace libasm

#endif // __DIS_W65C816_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

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

#include "insn_tms9900.h"
#include "reg_tms9900.h"
#include "table_tms9900.h"
#include "dis_interface.h"

namespace libasm {
namespace tms9900 {

class DisTms9900 : public Disassembler<target::uintptr_t> {
public:
    DisOperand &getFormatter() override { return _formatter; }
    bool setCpu(const char *cpu) override { return TableTms9900.setCpu(cpu); }
    const char *listCpu() const override { return TableTms9900::listCpu(); }
    Endian endian() const override { return ENDIAN_BIG; }
    host::uint_t maxBytes() const override { return Entry::code_max; }
    host::uint_t maxName() const override { return Entry::name_max; }

private:
    DisIntelOperand _formatter;
    RegTms9900 _regs;

    RegBase &getRegister() override { return _regs; }

    void outAddress(target::uintptr_t addr, bool relax = true);

    Error decodeOperand(
        DisMemory<target::uintptr_t> &memory, InsnTms9900 &insn,
        host::uint_t opr);
    Error decodeImmediate(
        DisMemory<target::uintptr_t> &memory, InsnTms9900 &insn);
    Error decodeRelative(InsnTms9900 &insn);
    Error decode(
        DisMemory<target::uintptr_t> &memory, Insn& insn) override;
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

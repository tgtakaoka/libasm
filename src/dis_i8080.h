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

#ifndef __DIS_I8080_H__
#define __DIS_I8080_H__

#include "config_i8080.h"

#include "insn_i8080.h"
#include "reg_i8080.h"
#include "table_i8080.h"
#include "dis_interface.h"

class DisI8080 : public Disassembler<target::uintptr_t> {
public:
    DisOperand &getFormatter() override { return _formatter; }
    bool setCpu(const char *cpu) override { return TableI8080.setCpu(cpu); }
    const char *listCpu() const override { return TableI8080::listCpu(); }
    Endian endian() const override { return ENDIAN_LITTLE; }
    host::uint_t maxBytes() const override { return Entry::code_max; }
    host::uint_t maxName() const override { return Entry::name_max; }

private:
    DisIntelOperand _formatter;
    RegI8080 _regs;

    RegBase &getRegister() override { return _regs; }
    void outRegister(RegName regName);

    Error decodeImmediate8(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeImmediate16(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeDirect(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeIoaddr(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decode(
        DisMemory<target::uintptr_t> &memory, Insn& insn) override;
};

#endif // __DIS_I8080_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

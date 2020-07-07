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

class DisZ8000
    : public Disassembler,
      public Config {
public:
    ValueFormatter &getFormatter() override { return _formatter; }

    // Config
    AddressWidth addressWidth() const override { return TableZ8000.addressWidth(); }
    int8_t addressBits() const override { return TableZ8000.addressBits(); }
    const char *listCpu() const override { return TableZ8000.listCpu(); }
    bool setCpu(const char *cpu) override { return TableZ8000.setCpu(cpu); }
    const char *getCpu() const override { return TableZ8000.getCpu(); }

    void reset() override { preferWorkRegister(true); }
    void preferWorkRegister(bool enabled) {
        _preferWorkRegister = enabled;
    }

private:
    ValueFormatter _formatter;
    RegZ8000 _regs;
    bool _preferWorkRegister = true;

    RegBase &getRegister() override { return _regs; }

    void outRegister(RegName regName);
    void outConditionCode(uint8_t ccNum);
    void outImmediate(uint8_t data, AddrMode mode);
    void outComma(const InsnZ8000 &insn, AddrMode mode, ModeField field);
    Error decodeImmediate(
        DisMemory &memory, InsnZ8000 &insn, AddrMode mode, OprSize size);
    Error decodeFlags(uint8_t flags);
    Error decodeGeneralRegister(
        uint8_t num, OprSize size, bool indirect = false);
    Error decodeDoubleSizedRegister(uint8_t num, OprSize size);
    Error decodeControlRegister(uint8_t ctlNum, OprSize size);
    Error decodeBaseAddressing(
        DisMemory &memory, InsnZ8000 &insn, AddrMode mode, uint8_t num);
    Error decodeGenericAddressing(
        DisMemory &memory, InsnZ8000 &insn, AddrMode mode, uint8_t num);
    Error decodeDirectAddress(DisMemory &memory, InsnZ8000 &insn);
    Error decodeRelativeAddressing(
        DisMemory &memory, InsnZ8000 &insn, AddrMode mode);
    Error decodeOperand(
        DisMemory &memory, InsnZ8000 &insn, AddrMode mode, ModeField field);
    Error checkPostWord(const InsnZ8000 &insn);
    Error decode(DisMemory &memory, Insn &insn) override;
};

} // namespace z8000
} // namespace libasm

#endif // __DIS_Z8000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

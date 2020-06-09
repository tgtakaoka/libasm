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

#ifndef __DIS_Z8_H__
#define __DIS_Z8_H__

#include "config_z8.h"
#include "dis_base.h"
#include "insn_z8.h"
#include "reg_z8.h"
#include "table_z8.h"

namespace libasm {
namespace z8 {

class DisZ8
    : public Disassembler,
      public Config {
public:
    ValueFormatter &getFormatter() override { return _formatter; }

    // Config
    const char *listCpu() const override { return TableZ8.listCpu(); }
    bool setCpu(const char *cpu) override { return TableZ8.setCpu(cpu); }
    const char *getCpu() const override { return TableZ8.getCpu(); }

    void preferWorkRegister(bool enabled) {
        _preferWorkRegister = enabled;
    }

private:
    IntelValueFormatter _formatter;
    RegZ8 _regs;
    bool _preferWorkRegister = true;

    RegBase &getRegister() override { return _regs; }
    void outCcName(Config::opcode_t opCode);
    void outIndexed(uint8_t base, RegName idx);
    Error outWorkReg(uint8_t regNum, bool indir = false, bool pair = false);
    Error outRegAddr(uint8_t regAddr, bool indir = false, bool pair = false);

    Error decodeOperand(DisMemory &memory, InsnZ8 &insn, AddrMode mode);
    Error decodeAbsolute(DisMemory &memory, InsnZ8 &insn);
    Error decodeRelative(DisMemory &memory, InsnZ8 &insn);
    Error decodeIndexed(DisMemory &memory, InsnZ8 &insn);
    Error decodeIndirectRegPair(DisMemory &memory, InsnZ8 &insn);
    Error decodeInOpCode(DisMemory &memory, InsnZ8 &insn);
    Error decodeTwoOperands(DisMemory &memory, InsnZ8 &insn);
    Error decode(DisMemory &memory, Insn &insn) override;
};

} // namespace z8
} // namespace libasm

#endif // __DIS_Z8_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

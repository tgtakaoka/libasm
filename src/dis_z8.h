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

class DisZ8 : public Disassembler, public Config {
public:
    DisZ8() : Disassembler(_formatter, _regs, TableZ8::TABLE, '$'), _formatter(), _regs() {
        reset();
    }

    const ConfigBase &config() const override { return *this; }
    void reset() override { _useWorkRegister = true; }
    const Options &options() const override { return _options; }

private:
    IntelValueFormatter _formatter;
    RegZ8 _regs;
    bool _useWorkRegister;
    const struct OptWorkRegister : public BoolOption {
        OptWorkRegister(bool &var);
    } _opt_workRegister{_useWorkRegister};
    const Options _options{_opt_workRegister};

    StrBuffer &outCcName(StrBuffer &out, Config::opcode_t opCode);
    StrBuffer &outIndexed(StrBuffer &out, uint16_t base, RegName idx, AddrMode mode);
    StrBuffer &outWorkReg(StrBuffer &out, uint8_t regNum, bool indir = false);
    StrBuffer &outPairReg(StrBuffer &out, uint8_t regNum, bool indir = false);
    StrBuffer &outRegAddr(StrBuffer &out, uint8_t regAddr, bool indir = false);
    StrBuffer &outPairAddr(StrBuffer &out, uint8_t regAddr, bool indir = false);
    StrBuffer &outBitPos(StrBuffer &out, uint8_t bitPos);

    Error decodeOperand(DisMemory &memory, InsnZ8 &insn, StrBuffer &out, AddrMode mode);
    Error decodeAbsolute(
            DisMemory &memory, InsnZ8 &insn, StrBuffer &out, Endian endian = ENDIAN_BIG);
    Error decodeRelative(DisMemory &memory, InsnZ8 &insn, StrBuffer &out);
    Error decodeIndexed(DisMemory &memory, InsnZ8 &insn, StrBuffer &out, uint8_t opr1);
    Error decodeIndirectRegPair(DisMemory &memory, InsnZ8 &insn, StrBuffer &out);
    Error decodeInOpCode(DisMemory &memory, InsnZ8 &insn, StrBuffer &out);
    Error decodeTwoOperands(DisMemory &memory, InsnZ8 &insn, StrBuffer &out);
    Error decodePostByte(DisMemory &memory, InsnZ8 &insn, StrBuffer &out);
    Error decodeImpl(DisMemory &memory, Insn &insn, StrBuffer &out) override;
};

}  // namespace z8
}  // namespace libasm

#endif  // __DIS_Z8_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

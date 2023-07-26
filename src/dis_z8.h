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

namespace libasm {
namespace z8 {

class DisZ8 final : public Disassembler, public Config {
public:
    DisZ8();

    void reset() override;

    Error setUseWorkRegister(bool enable);

private:
    const PrefixHexFormatter _hexFormatter{HexFormatter::PERCENT};
    const BoolOption<DisZ8> _opt_workRegister;

    bool _useWorkRegister;

    StrBuffer &outConditionCode(StrBuffer &out, Config::opcode_t opCode);
    StrBuffer &outIndexed(StrBuffer &out, uint16_t base, RegName idx, AddrMode mode);
    StrBuffer &outRegAddr(StrBuffer &out, uint8_t regAddr, bool indir = false);
    StrBuffer &outPairAddr(StrBuffer &out, uint8_t regAddr, bool indir = false);
    StrBuffer &outBitPos(StrBuffer &out, uint8_t bitPos);

    Error decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode);
    Error decodeAbsolute(DisInsn &insn, StrBuffer &out, Endian endian = ENDIAN_BIG);
    Error decodeRelative(DisInsn &insn, StrBuffer &out);
    Error decodeIndexed(DisInsn &insn, StrBuffer &out, uint8_t opr1);
    Error decodeIndirectRegPair(DisInsn &insn, StrBuffer &out);
    Error decodeInOpCode(DisInsn &insn, StrBuffer &out);
    Error decodeTwoOperands(DisInsn &insn, StrBuffer &out);
    Error decodePostByte(DisInsn &insn, StrBuffer &out);

    Error decodeImpl(DisMemory &memory, Insn &insn, StrBuffer &out) override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
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

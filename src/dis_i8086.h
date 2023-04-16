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

#ifndef __DIS_I8086_H__
#define __DIS_I8086_H__

#include "config_i8086.h"
#include "dis_base.h"
#include "insn_i8086.h"
#include "reg_i8086.h"

namespace libasm {
namespace i8086 {

class DisI8086 final : public Disassembler, public Config {
public:
    DisI8086();

    void reset() override;

    Error setStringInsn(bool enable);
    Error setSegmentInsn(bool enable);

private:
    const SuffixHexFormatter _hexFormatter{'h'};
    const BoolOption<DisI8086> _opt_segmentInsn;
    const BoolOption<DisI8086> _opt_stringInsn;

    bool _segOverrideInsn;
    bool _repeatHasStringInst;

    StrBuffer &outRegister(StrBuffer &out, RegName name, const char prefix = 0);
    Error outMemReg(DisInsn &insn, StrBuffer &out, RegName seg, uint8_t mode, uint8_t r_m);

    RegName decodeRegister(const DisInsn &insn, AddrMode mode, OprPos pos);
    Error decodeRelative(DisInsn &insn, StrBuffer &out, AddrMode mode);
    Error decodeImmediate(DisInsn &insn, StrBuffer &out, AddrMode mode);
    Error decodeRepeatStr(DisInsn &insn, StrBuffer &out);
    Error decodeMemReg(DisInsn &insn, StrBuffer &out, AddrMode mode, OprPos pos);
    Error decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode, OprPos pos);
    Error readCodes(DisInsn &insn);
    Error decodeStringInst(DisInsn &insn, StrBuffer &out);

    Error decodeImpl(DisMemory &memory, Insn &insn, StrBuffer &out) override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
};

}  // namespace i8086
}  // namespace libasm

#endif  // __DIS_I8086_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

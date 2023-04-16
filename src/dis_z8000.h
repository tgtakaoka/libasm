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

namespace libasm {
namespace z8000 {

class DisZ8000 final : public Disassembler, public Config {
public:
    DisZ8000();

    void reset() override;

    Error setShortDirect(bool enable);
    Error setIoAddressPrefix(bool enable);

private:
    const SuffixHexFormatter _hexFormatter{'h'};
    const BoolOption<DisZ8000> _opt_shortDirect;
    const BoolOption<DisZ8000> _opt_ioaddrPrefix;

    bool _shortDirect;
    bool _ioAddressPrefix;

    StrBuffer &outImmediate(StrBuffer &out, uint8_t data, AddrMode mode);
    StrBuffer &outComma(StrBuffer &out, const DisInsn &insn, AddrMode mode, ModeField field);
    Error decodeImmediate(DisInsn &insn, StrBuffer &out, AddrMode mode, OprSize size);
    Error decodeFlags(StrBuffer &out, uint8_t flags);
    Error decodeGeneralRegister(StrBuffer &out, uint8_t num, OprSize size, bool indirect = false);
    Error decodeDoubleSizedRegister(StrBuffer &out, uint8_t num, OprSize size);
    Error decodeControlRegister(StrBuffer &out, uint8_t ctlNum, OprSize size);
    Error decodeBaseAddressing(DisInsn &insn, StrBuffer &out, AddrMode mode, uint8_t num);
    Error decodeGenericAddressing(DisInsn &insn, StrBuffer &out, AddrMode mode, uint8_t num);
    Error decodeDirectAddress(DisInsn &insn, StrBuffer &out);
    Error decodeRelativeAddressing(DisInsn &insn, StrBuffer &out, AddrMode mode);
    Error decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode, ModeField field);
    Error checkPostWord(const DisInsn &insn);
    Error checkRegisterOverlap(const DisInsn &insn);

    Error decodeImpl(DisMemory &memory, Insn &insn, StrBuffer &out) override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
};

}  // namespace z8000
}  // namespace libasm

#endif  // __DIS_Z8000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

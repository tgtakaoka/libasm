/*
 * Copyright 2022 Tadashi G. Takaoka
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

#ifndef __DIS_I8096_H__
#define __DIS_I8096_H__

#include "config_i8096.h"
#include "dis_base.h"
#include "insn_i8096.h"

namespace libasm {
namespace i8096 {

class DisI8096 final : public Disassembler, public Config {
public:
    DisI8096();

    void reset() override;

    Error setUseAbsolute(bool enable);

private:
    const SuffixHexFormatter _hexFormatter{'h'};
    const BoolOption<DisI8096> _opt_absolute;

    bool _useAbsolute;

    struct Operand : public ErrorReporter {
        AddrMode mode;
        uint8_t regno;
        uint16_t val16;
        int16_t int16() const { return static_cast<int16_t>(val16); }
        int8_t int8() const { return static_cast<int8_t>(val16); }
        Operand() : mode(M_NONE), regno(0), val16(0) {}
        Error read(DisInsn &insn, AddrMode mode);
    };

    StrBuffer &outRegister(StrBuffer &out, uint8_t regno, bool indir = false) const;
    StrBuffer &outRelative(StrBuffer &out, const DisInsn &insn, const Operand &op);
    StrBuffer &outOperand(StrBuffer &out, const DisInsn &insn, const Operand &op);

    Error decodeImpl(DisMemory &memory, Insn &insn, StrBuffer &out) override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
};

}  // namespace i8096
}  // namespace libasm

#endif  // __DIS_I8096_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

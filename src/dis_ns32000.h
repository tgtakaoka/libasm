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

#ifndef __DIS_NS32000_H__
#define __DIS_NS32000_H__

#include "config_ns32000.h"
#include "dis_base.h"
#include "insn_ns32000.h"

namespace libasm {
namespace ns32000 {

struct DisNs32000 final : Disassembler, Config {
    DisNs32000(const ValueFormatter::Plugins &plugins = defaultPlugins());

    void reset() override;

    Error setPcRelativeParen(bool enable);
    Error setExternalParen(bool enable);
    Error setStringOptionBracket(bool enable);
    Error setFloatPrefix(bool enable);

private:
    const BoolOption<DisNs32000> _opt_pcrelParen;
    const BoolOption<DisNs32000> _opt_externalParen;
    const BoolOption<DisNs32000> _opt_stroptBracket;
    const BoolOption<DisNs32000> _opt_floatPrefix;

    bool _stringOptionBracket;
    bool _pcRelativeParen;
    bool _externalParen;
    bool _floatPrefix;

    struct Displacement : ErrorReporter {
        int32_t val32;
        uint8_t bits;
    };

    StrBuffer &outDisplacement(StrBuffer &out, const Displacement &disp) const;

    Error readIndexByte(DisInsn &insn, AddrMode mode, OprPos pos) const;
    Error readDisplacement(DisInsn &insn, Displacement &disp) const;

    void decodeLength(DisInsn &insn, StrBuffer &out, AddrMode mode) const;
    void decodeBitField(DisInsn &insn, StrBuffer &out) const;
    void decodeImmediate(DisInsn &insn, StrBuffer &out, AddrMode mode) const;
    void decodeDisplacement(DisInsn &insn, StrBuffer &out, AddrMode mode) const;
    void decodeRelative(DisInsn &insn, StrBuffer &out) const;
    void decodeConfig(DisInsn &insn, StrBuffer &out, OprPos pos) const;
    void decodeStrOpt(DisInsn &insn, StrBuffer &out, OprPos pos) const;
    void decodeRegisterList(DisInsn &insn, StrBuffer &out) const;
    void decodeGeneric(DisInsn &insn, StrBuffer &out, AddrMode mode, OprPos pos, OprSize size,
            Error idxError) const;
    void decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode, OprPos pos, OprSize size,
            Error idxError = OK) const;

    Error decodeImpl(DisMemory &memory, Insn &insn, StrBuffer &out) const override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueFormatter::Plugins &defaultPlugins();
};

}  // namespace ns32000
}  // namespace libasm

#endif  // __DIS_NS32000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

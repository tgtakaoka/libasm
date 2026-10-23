/*
 * Copyright 2026 Tadashi G. Takaoka
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

#ifndef __LIBASM_DIS_C166_H__
#define __LIBASM_DIS_C166_H__

#include "config_c166.h"
#include "dis_base.h"
#include "insn_c166.h"

namespace libasm {
namespace c166 {

struct DisC166 final : Disassembler, Config {
    DisC166(const ValueFormatter::Plugins &plugins = defaultPlugins());

    void reset() override;

    Error setCcPrefix(bool enable);
    Error setIndexSharp(bool enable);
    Error setCanonicalMem(bool enable);
    Error setExtPrefix(bool enable);
    Error setSfrName(bool enable);
    Error setGnuAs(bool enable) override;

private:
    const BoolOption<DisC166> _opt_ccPrefix;
    const BoolOption<DisC166> _opt_indexSharp;
    const BoolOption<DisC166> _opt_canonicalMem;
    const BoolOption<DisC166> _opt_extPrefix;
    const BoolOption<DisC166> _opt_sfrName;

    bool _ccPrefix;
    bool _indexSharp;
    bool _canonicalMem;
    bool _extPrefix;
    bool _sfrName;

    void decodeShortReg(DisInsn &insn, StrBuffer &out, AddrMode mode) const;
    void decodeBitOff(DisInsn &insn, StrBuffer &out, uint8_t bitoff) const;
    void decodeSfr(DisInsn &insn, StrBuffer &out, uint16_t addr) const;
    void decodeMem(StrBuffer &out, uint16_t addr) const;
    void decodeRelative(DisInsn &insn, StrBuffer &out, uint8_t rel) const;
    void decodeCodeAddress(DisInsn &insn, StrBuffer &out) const;
    void decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode) const;
    Error decodeInsn(DisInsn &insn, StrBuffer &out, bool follower = false) const;
    Error decodeFollower(Insn &insn, StrBuffer &out) const;

    Error decodeImpl(DisMemory &memory, Insn &insn, StrBuffer &out) const override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueFormatter::Plugins &defaultPlugins();
};

}  // namespace c166
}  // namespace libasm

#endif  // __LIBASM_DIS_C166_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

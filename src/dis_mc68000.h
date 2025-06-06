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

#ifndef __LIBASM_DIS_MC68000_H__
#define __LIBASM_DIS_MC68000_H__

#include "config_mc68000.h"
#include "dis_base.h"
#include "insn_mc68000.h"

namespace libasm {
namespace mc68000 {

struct DisMc68000 final : Disassembler, Config {
    DisMc68000(const ValueFormatter::Plugins &plugins = defaultPlugins());

    void reset() override;

private:
    const TextOption<Config> _opt_fpu;
    const TextOption<Config> _opt_pmmu;

    void outCacheList(StrBuffer &out, CacheName cache) const;
    void outBitField(DisInsn &insn, StrBuffer &out) const;
    void outDataRegPair(DisInsn &insn, StrBuffer &out, OprPos pos) const;
    void outPointerPair(DisInsn &insn, StrBuffer &out) const;
    void outControlReg(DisInsn &insn, StrBuffer &out) const;
    void outImmediateData(DisInsn &insn, StrBuffer &out, OprSize eaSize) const;
    void outEffectiveAddr(
            DisInsn &insn, StrBuffer &out, AddrMode mode, RegName reg, OprSize size) const;
#if !defined(LIBASM_MC68000_NOFPU)
    StrBuffer &outExtendedReal(StrBuffer &buf, const ExtendedReal &v) const;
    StrBuffer &outDecimalString(StrBuffer &buf, const DecimalString &v) const;
#endif
    Config::uintptr_t readDisplacement(DisInsn &insn, RegName base, uint_fast8_t bdSize) const;
    const char *outDisplacement(DisInsn &insn, StrBuffer &out, RegName base, Config::uintptr_t disp,
            uint_fast8_t bdSize, const char *delim) const;
    const char *outIndexReg(StrBuffer &out, uint16_t ext, const char *delim) const;
    void decodeBriefExtension(DisInsn &insn, StrBuffer &out, uint16_t ext, RegName base) const;
    void decodeFullExtension(
            DisInsn &insn, StrBuffer &out, uint16_t ext, RegName base, OprSize size) const;
    void decodeRelative(DisInsn &insn, StrBuffer &out, AddrMode mode) const;
    void decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode, OprPos pos, OprSize size,
            uint16_t opr16 = 0, Error opr16Error = OK) const;
    char decodeInsnSize(const DisInsn &insn, OprSize size) const;

    Error decodeImpl(DisMemory &memory, Insn &insn, StrBuffer &out) const override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueFormatter::Plugins &defaultPlugins();
};

}  // namespace mc68000
}  // namespace libasm

#endif  // __LIBASM_DIS_MC68000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

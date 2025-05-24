/*
 * Copyright 2021 Tadashi G. Takaoka
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

#ifndef __LIBASM_DIS_TMS320_H__
#define __LIBASM_DIS_TMS320_H__

#include "config_tms320.h"
#include "dis_base.h"
#include "insn_tms320.h"

namespace libasm {
namespace tms320 {

struct DisTms320 final : Disassembler, Config {
    DisTms320(const ValueFormatter::Plugins &plugins = defaultPlugins());

    void reset() override;

    Error setUseAuxName(bool enable);
    Error setUsePortName(bool enable);

private:
    const BoolOption<DisTms320> _opt_useAuxName;
    const BoolOption<DisTms320> _opt_usePortName;

    bool _useAuxName;
    bool _usePortName;

    StrBuffer &outAuxiliary(StrBuffer &out, uint_fast8_t no) const;
    StrBuffer &outPort(StrBuffer &out, uint_fast8_t no) const;
    StrBuffer &outDirect(StrBuffer &out, DisInsn &insn) const;
    StrBuffer &outIndirect(StrBuffer &out, uint8_t mam) const;
    StrBuffer &outModifyAR(StrBuffer &out, uint8_t mam) const;
    StrBuffer &outNextArp(StrBuffer &out, uint8_t mam) const;
    StrBuffer &outShiftCount(StrBuffer &out, uint8_t count, uint8_t mam) const;
    StrBuffer &outProgramAddress(StrBuffer &out, DisInsn &insn) const;
    void decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode) const;
    bool hasValue(DisInsn &insn, AddrMode mode) const;

    Error decodeImpl(DisMemory &memory, Insn &insn, StrBuffer &out) const override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueFormatter::Plugins &defaultPlugins();
};

}  // namespace tms320
}  // namespace libasm

#endif  // __LIBASM_DIS_TMS320_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

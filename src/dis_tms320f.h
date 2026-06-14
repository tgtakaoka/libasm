/*
 * Copyright 2025 Tadashi G. Takaoka
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

#ifndef __LIBASM_DIS_TMS320F_H__
#define __LIBASM_DIS_TMS320F_H__

#include "config_tms320f.h"
#include "dis_base.h"
#include "insn_tms320f.h"

namespace libasm {
namespace tms320f {

struct DisTms320f final : Disassembler, Config {
    DisTms320f(const ValueFormatter::Plugins &plugins = defaultPlugins());

private:
    // Cross-instruction state carried on the Insn (per Insn::state<T>).
    // hasParaDst: set during the first half of a parallel pair, consumed by
    // the second to flag DUPLICATE_REGISTER. All-zero (hasParaDst == false)
    // is the natural fresh state.
    struct State {
        bool hasParaDst;
        int8_t paraDstReg;  // valid only when hasParaDst is true; RegName fits in int8_t
    };

    void decodeRelative(DisInsn &insn, StrBuffer &out, AddrMode mode) const;
    void decodeAbsolute(DisInsn &insn, StrBuffer &out) const;
    RegName decodeIndirect(DisInsn &insn, StrBuffer &out, uint_fast8_t mod, uint_fast8_t ar,
            int_fast16_t disp) const;
    void decodeGeneric(DisInsn &insn, StrBuffer &out, AddrMode mode, uint_fast8_t pos) const;
    RegName decode3OpGeneric(
            DisInsn &insn, StrBuffer &out, AddrMode mode, bool indir, uint_fast8_t modar) const;
    RegName decodeRegister(DisInsn &insn, OprPos pos) const;
    void decodeOperand(
            DisInsn &insn, StrBuffer &out, AddrMode mode, OprPos pos, uint_fast8_t no) const;

    Error decodeImpl(DisMemory &memory, Insn &insn, StrBuffer &out) const override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueFormatter::Plugins &defaultPlugins();
};

}  // namespace tms320f
}  // namespace libasm

#endif  // __LIBASM_DIS_TMS320F_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

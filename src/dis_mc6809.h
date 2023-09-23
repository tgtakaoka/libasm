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

#ifndef __DIS_MC6809_H__
#define __DIS_MC6809_H__

#include "config_mc6809.h"
#include "dis_base.h"
#include "insn_mc6809.h"

namespace libasm {
namespace mc6809 {

struct DisMc6809 final : Disassembler, Config {
    DisMc6809(const ValueFormatter::Plugins &plugins = defaultPlugins());

private:
    // MC6809
    Error decodeDirectPage(DisInsn &insn, StrBuffer &out);
    Error decodeIndexed(DisInsn &insn, StrBuffer &out);
    Error decodeExtended(DisInsn &insn, StrBuffer &out);
    Error decodeRelative(DisInsn &insn, StrBuffer &out, AddrMode mode);
    Error decodeImmediate(DisInsn &insn, StrBuffer &out, AddrMode mode);
    Error decodePushPull(DisInsn &insn, StrBuffer &out);
    Error decodeRegisters(DisInsn &insn, StrBuffer &out);
    // HD6309
    Error decodeRegBit(DisInsn &insn, StrBuffer &out);
    Error decodeDirBit(DisInsn &insn, StrBuffer &out);
    Error decodeTransferMemory(DisInsn &insn, StrBuffer &out);

    Error decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode);
    Error decodeImpl(DisMemory &memory, Insn &insn, StrBuffer &out) override;

    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueFormatter::Plugins &defaultPlugins();
};

}  // namespace mc6809
}  // namespace libasm

#endif  // __DIS_MC6809_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

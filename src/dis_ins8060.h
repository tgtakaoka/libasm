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

#ifndef __DIS_INS8060_H__
#define __DIS_INS8060_H__

#include "config_ins8060.h"
#include "dis_base.h"
#include "insn_ins8060.h"

namespace libasm {
namespace ins8060 {

class DisIns8060 final : public Disassembler, public Config {
public:
    DisIns8060();

private:
    const PrefixHexFormatter _hexFormatter{HexFormatter::X_DASH};

    Error decodePntr(InsnIns8060 &insn, StrBuffer &out);
    Error decodeImm8(DisMemory &memory, InsnIns8060 &insn, StrBuffer &out);
    Error decodeIndx(DisMemory &memory, InsnIns8060 &insn, StrBuffer &out, bool hasMode);
    Error decodeImpl(DisMemory &memory, Insn &insn, StrBuffer &out) override;

    static Config::uintptr_t page(Config::uintptr_t addr) { return addr & ~0xFFF; }
    static Config::uintptr_t offset(Config::uintptr_t addr) { return addr & 0xFFF; }

    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
};

}  // namespace ins8060
}  // namespace libasm

#endif  // __DIS_INS8060_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

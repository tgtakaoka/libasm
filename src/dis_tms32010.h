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

#ifndef __DIS_TMS32010_H__
#define __DIS_TMS32010_H__

#include "config_tms32010.h"
#include "dis_base.h"
#include "insn_tms32010.h"

namespace libasm {
namespace tms32010 {

class DisTms32010 final : public Disassembler, public Config {
public:
    DisTms32010();

private:
    const SuffixHexFormatter _hexFormatter{'h'};

    Error decodeDirect(StrBuffer &out, Config::opcode_t opc);
    Error decodeIndirect(StrBuffer &out, uint8_t mam);
    Error decodeNextArp(StrBuffer &out, uint8_t mam);
    Error decodeShiftCount(StrBuffer &out, uint8_t count, uint8_t mam, AddrMode mnode);
    Error decodeOperand(DisMemory &memory, InsnTms32010 &insn, StrBuffer &out, AddrMode mode);
    Error decodeImpl(DisMemory &memory, Insn &insn, StrBuffer &out) override;

    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
};

}  // namespace tms32010
}  // namespace libasm

#endif  // __DIS_TMS32010_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

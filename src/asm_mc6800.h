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

#ifndef __ASM_MC6800_H__
#define __ASM_MC6800_H__

#include "asm_base.h"
#include "config_mc6800.h"
#include "insn_mc6800.h"

namespace libasm {
namespace mc6800 {

class AsmMc6800 final : public Assembler, public Config {
public:
    AsmMc6800(const ValueParser::Plugins &plugins = defaultPlugins());

private:
    struct Operand;
    Error parseOperand(StrScanner &scan, Operand &op) const;
    void emitRelative(AsmInsn &insn, const Operand &op);
    void emitImmediate(AsmInsn &insn, AddrMode mode, const Operand &op);
    void emitBitNumber(AsmInsn &insn, const Operand &op);
    void emitOperand(AsmInsn &insn, AddrMode mode, const Operand &op);

    Error encodeImpl(StrScanner &scan, Insn &insn) override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueParser::Plugins &defaultPlugins();
};

}  // namespace mc6800
}  // namespace libasm

#endif  // __ASM_MC6800_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

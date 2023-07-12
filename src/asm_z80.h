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

#ifndef __ASM_Z80_H__
#define __ASM_Z80_H__

#include "asm_base.h"
#include "config_z80.h"
#include "insn_z80.h"

namespace libasm {
namespace z80 {

class AsmZ80 final : public Assembler, public Config {
public:
    AsmZ80(const ValueParser::Plugins &plugins = defaultPlugins());

    bool hasSetInstruction() const override { return true; }

private:
    struct Operand;
    Error parseOperand(StrScanner &scan, Operand &op) const;

    void encodeRelative(AsmInsn &insn, const Operand &op);
    void encodeIndexedBitOp(AsmInsn &insn, const Operand &op);
    void encodeOperand(AsmInsn &insn, const Operand &op, AddrMode mode, const Operand &other);

    Error processPseudo(StrScanner &scan, Insn &insn) override;
    Error encodeImpl(StrScanner &scan, Insn &insn) override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueParser::Plugins &defaultPlugins();
};

}  // namespace z80
}  // namespace libasm

#endif  // __ASM_Z80_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

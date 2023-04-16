/*
 * Copyright 2022 Tadashi G. Takaoka
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

#ifndef __ASM_SCN2650_H__
#define __ASM_SCN2650_H__

#include "asm_base.h"
#include "config_scn2650.h"
#include "insn_scn2650.h"

namespace libasm {
namespace scn2650 {

class AsmScn2650 final : public Assembler, public Config {
public:
    AsmScn2650(const ValueParser::Plugins &plugins = defaultPlugins());

private:
    struct Operand;
    Error parseOperand(StrScanner &scan, Operand &op) const;

    void emitAbsolute(AsmInsn &insn, const Operand &op, AddrMode mode);
    void emitIndexed(AsmInsn &insn, const Operand &op, AddrMode mode);
    void emitZeroPage(AsmInsn &insn, const Operand &op);
    void emitRelative(AsmInsn &insn, const Operand &op);
    void encodeOperand(AsmInsn &insn, const Operand &op, AddrMode mode);

    Error encodeImpl(StrScanner &scan, Insn &insn) override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueParser::Plugins &defaultPlugins();
};

}  // namespace scn2650
}  // namespace libasm

#endif  // __ASM_SCN2650_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

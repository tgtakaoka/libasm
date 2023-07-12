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

#ifndef __ASM_CDP1802_H__
#define __ASM_CDP1802_H__

#include "asm_base.h"
#include "config_cdp1802.h"
#include "insn_cdp1802.h"

namespace libasm {
namespace cdp1802 {

class AsmCdp1802 final : public Assembler, public Config {
public:
    AsmCdp1802(const ValueParser::Plugins &plugins = defaultPlugins());

    void reset() override;

    Error setUseReg(bool enable);
    Error setSmartBranch(bool enable);

private:
    const BoolOption<AsmCdp1802> _opt_useReg;
    const BoolOption<AsmCdp1802> _opt_smartBranch;

    bool _useReg;
    bool _smartBranch;

    struct Operand;
    Error parseOperand(StrScanner &scan, Operand &op) const;

    void emitOperand(AsmInsn &insn, AddrMode mode, const Operand &op);
    void encodePage(AsmInsn &insn, AddrMode mode, const Operand &op);

    Error processPseudo(StrScanner &scan, Insn &insn) override;
    Error encodeImpl(StrScanner &scan, Insn &insn) override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueParser::Plugins &defaultPlugins();
};

}  // namespace cdp1802
}  // namespace libasm

#endif  // __ASM_CDP1802_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

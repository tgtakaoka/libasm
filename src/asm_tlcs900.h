/*
 * Copyright 2026 Tadashi G. Takaoka
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

#ifndef __LIBASM_ASM_TLCS900_H__
#define __LIBASM_ASM_TLCS900_H__

#include "asm_base.h"
#include "config_tlcs900.h"
#include "insn_tlcs900.h"
#include "option_base.h"
#include "pseudos.h"

namespace libasm {
namespace tlcs900 {

struct Operand;

struct AsmTlcs900 final : Assembler, Config {
    AsmTlcs900(const ValueParser::Plugins &plugins = defaultPlugins());

    void reset() override;

private:
    const BoolOption<Config> _opt_maxMode;

    Error parseOperand(StrScanner &scan, Operand &op) const;
    void encodePrefixAddr(AsmInsn &insn) const;
    void encodeOperand(AsmInsn &insn, const Operand &op, AddrMode mode) const;
    Error processMaxMode(StrScanner &scan, Insn &insn, uint16_t extra);

    Error processPseudo(StrScanner &scan, Insn &insn) override;
    Error encodeImpl(StrScanner &scan, Insn &insn) const override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueParser::Plugins &defaultPlugins();

    using PseudoTlcs900 = pseudo::__Pseudo<AsmTlcs900, Insn>;
    using PseudosTlcs900 = pseudo::__Pseudos<PseudoTlcs900>;
    static const PseudoTlcs900 PSEUDO_TLCS900_TABLE[] PROGMEM;
    static const PseudosTlcs900 PSEUDOS_TLCS900 PROGMEM;
};

}  // namespace tlcs900
}  // namespace libasm

#endif  // __LIBASM_ASM_TLCS900_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

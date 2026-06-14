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

#ifndef __LIBASM_ASM_CP1600_H__
#define __LIBASM_ASM_CP1600_H__

#include "asm_base.h"
#include "config_cp1600.h"
#include "insn_cp1600.h"

namespace libasm {
namespace cp1600 {

struct AsmCp1600 final : Assembler, Config {
    AsmCp1600(const ValueParser::Plugins &plugins = defaultPlugins());

    void reset() override;

private:
    Error parseOperand(StrScanner &scan, Operand &op) const;

    void encodeOperand(AsmInsn &insn, const Operand &op, AddrMode mode) const;
    void encodeBranch(AsmInsn &insn, const Operand &op) const;
    void emitBranch(AsmInsn &insn, const Operand &op) const;

    Error defineByte(StrScanner &scan, Insn &insn, uint16_t extra);
    Error defineZero(StrScanner &scan, Insn &insn, uint16_t extra);
    Error defineText(StrScanner &scan, Insn &insn, uint16_t extra);
    Error setRadix(StrScanner &scan, Insn &insn, uint16_t extra);

    Error processPseudo(StrScanner &scan, Insn &insn) override;

    Error encodeImpl(StrScanner &scan, Insn &insn) const override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueParser::Plugins &defaultPlugins();

    using PseudoCp1600 = pseudo::__Pseudo<AsmCp1600, Insn>;
    using PseudosCp1600 = pseudo::__Pseudos<PseudoCp1600>;
    static const PseudoCp1600 PSEUDO_CP1600_TABLE[] PROGMEM;
    static const PseudosCp1600 PSEUDOS_CP1600 PROGMEM;

public:
    // Cross-instruction state carried on the Insn (per Insn::state<T>).
    // sdbdPrefix: true if the previously assembled instruction was SDBD
    // (0x0001). All-zero (sdbdPrefix == false) is the natural fresh state.
    struct State { bool sdbdPrefix; };
};

}  // namespace cp1600
}  // namespace libasm

#endif  // __LIBASM_ASM_CP1600_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

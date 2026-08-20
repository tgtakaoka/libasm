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

#ifndef __LIBASM_ASM_AM29000_H__
#define __LIBASM_ASM_AM29000_H__

#include "asm_base.h"
#include "config_am29000.h"
#include "insn_am29000.h"

namespace libasm {
namespace am29000 {

struct AsmAm29000 final : Assembler, Config {
    AsmAm29000(const ValueParser::Plugins &plugins = defaultPlugins());

    void reset() override;

private:
    const BoolOption<AsmAm29000> _opt_fpu;
    const BoolOption<AsmAm29000> _opt_intMul;

    Error parseOperand(StrScanner &scan, Operand &op) const;

    void encodeTarget(AsmInsn &insn, const Operand &op) const;
    void encodeControl(AsmInsn &insn, const Operand &op, uint_fast8_t pos, uint32_t max,
            AddrMode mode = M_NONE) const;
    void encodeOperand(AsmInsn &insn, const Operand &op, AddrMode mode) const;

    Error setFpuOption(bool enable);
    Error setIntMulOption(bool enable);

    Error processPseudo(StrScanner &scan, Insn &insn) override;
    Error encodeImpl(StrScanner &scan, Insn &insn) const override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueParser::Plugins &defaultPlugins();

    using PseudoAm29000 = pseudo::__Pseudo<AsmAm29000, Insn>;
    using PseudosAm29000 = pseudo::__Pseudos<PseudoAm29000>;
    static const PseudoAm29000 PSEUDO_AM29000_TABLE[] PROGMEM;
    static const PseudosAm29000 PSEUDOS_AM29000 PROGMEM;
    Error processFpu(StrScanner &scan, Insn &insn, uint16_t extra);
    Error processIntMul(StrScanner &scan, Insn &insn, uint16_t extra);
};

}  // namespace am29000
}  // namespace libasm

#endif  // __LIBASM_ASM_AM29000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

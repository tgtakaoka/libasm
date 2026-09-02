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

#ifndef __LIBASM_ASM_I960_H__
#define __LIBASM_ASM_I960_H__

#include "asm_base.h"
#include "config_i960.h"
#include "insn_i960.h"

namespace libasm {
namespace i960 {

struct AsmI960 final : Assembler, Config {
    AsmI960(const ValueParser::Plugins &plugins = defaultPlugins());

    void reset() override;

private:
    const IntOption<AsmI960> _opt_modeBits;
    const TextOption<Config> _opt_ipOperand;
    const BoolOption<AsmI960> _opt_fpu;

    Error parseMemory(StrScanner &scan, Operand &op) const;
    Error parseOperand(StrScanner &scan, Operand &op) const;

    void encodeRegister(AsmInsn &insn, const Operand &op, AddrMode mode, uint_fast8_t pos) const;
    void encodeLiteral(AsmInsn &insn, const Operand &op, Config::opcode_t modeBit,
            uint_fast8_t pos) const;
    void encodeDisplacement(AsmInsn &insn, const Operand &op, uint_fast8_t bits) const;
    void encodeMemory(AsmInsn &insn, const Operand &op) const;
    void encodeFloat(AsmInsn &insn, const Operand &op, AddrMode mode, Config::opcode_t modeBit,
            uint_fast8_t pos) const;
    void encodeOperand(AsmInsn &insn, const Operand &op, AddrMode mode) const;

    Error setModeBitsOption(int32_t value);
    Error setFpuOption(bool enable);

    Error processPseudo(StrScanner &scan, Insn &insn) override;
    Error encodeImpl(StrScanner &scan, Insn &insn) const override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueParser::Plugins &defaultPlugins();

    using PseudoI960 = pseudo::__Pseudo<AsmI960, Insn>;
    using PseudosI960 = pseudo::__Pseudos<PseudoI960>;
    static const PseudoI960 PSEUDO_I960_TABLE[] PROGMEM;
    static const PseudosI960 PSEUDOS_I960 PROGMEM;
    Error processModeBits(StrScanner &scan, Insn &insn, uint16_t extra);
    Error processIpOperand(StrScanner &scan, Insn &insn, uint16_t extra);
    Error processFpu(StrScanner &scan, Insn &insn, uint16_t extra);
    Error defineExtended(StrScanner &scan, Insn &insn, uint16_t extra);
};

}  // namespace i960
}  // namespace libasm

#endif  // __LIBASM_ASM_I960_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

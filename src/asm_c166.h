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

#ifndef __LIBASM_ASM_C166_H__
#define __LIBASM_ASM_C166_H__

#include "asm_base.h"
#include "config_c166.h"
#include "insn_c166.h"

namespace libasm {
namespace c166 {

struct AsmC166 final : Assembler, Config {
    AsmC166(const ValueParser::Plugins &plugins = defaultPlugins());

    void reset() override;

private:
    // The page each DPP register is assumed to hold, -1 if none.
    int16_t _dpp[4];

    Error processPseudo(StrScanner &scan, Insn &insn) override;
    Error processDpp(StrScanner &scan, Insn &insn, uint8_t num);

    bool parseCoIndirect(StrScanner &scan, Operand &op) const;
    Error parseRepeat(StrScanner &scan, AsmInsn &insn) const;
    Error parseIndirect(StrScanner &scan, Operand &op) const;
    Error parseOperand(StrScanner &scan, Operand &op, bool first) const;

    void encodeShortReg(AsmInsn &insn, const Operand &op) const;
    uint8_t encodeBitOff(AsmInsn &insn, const Operand &op) const;
    uint8_t encodeRelative(AsmInsn &insn, const Operand &op) const;
    void encodeCodeAddress(AsmInsn &insn, const Operand &op) const;
    void encodeMemAddress(AsmInsn &insn, const Operand &op) const;
    void encodeImmediate(AsmInsn &insn, AddrMode mode, const Operand &op) const;
    void encodeOperand(AsmInsn &insn, AddrMode mode, const Operand &op) const;

    Error encodeImpl(StrScanner &scan, Insn &insn) const override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueParser::Plugins &defaultPlugins();
};

}  // namespace c166
}  // namespace libasm

#endif  // __LIBASM_ASM_C166_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

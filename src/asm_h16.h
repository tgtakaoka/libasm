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

#ifndef __LIBASM_ASM_H16_H__
#define __LIBASM_ASM_H16_H__

#include "asm_base.h"
#include "config_h16.h"
#include "insn_h16.h"

namespace libasm {
namespace h16 {

struct AsmH16 final : Assembler, Config {
    AsmH16(const ValueParser::Plugins &plugins = defaultPlugins());

private:
    Error parseOperand(StrScanner &scan, Operand &op) const;
    Error parseIndirect(StrScanner &scan, Operand &op) const;
    Error parseParen(StrScanner &scan, Operand &op) const;
    Error parseRegList(StrScanner &scan, AsmInsn &insn) const;
    void encodeOperand(AsmInsn &insn, const Operand &op, AddrMode mode) const;
    void emitEa(AsmInsn &insn, const Operand &op, uint8_t accBit) const;
    void emitPcRelDisp(AsmInsn &insn, const Operand &op, uint8_t sd) const;

    Error encodeImpl(StrScanner &scan, Insn &insn) const override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueParser::Plugins &defaultPlugins();
};

}  // namespace h16
}  // namespace libasm

#endif  // __LIBASM_ASM_H16_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

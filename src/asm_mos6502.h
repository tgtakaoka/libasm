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

#ifndef __LIBASM_ASM_MOS6502_H__
#define __LIBASM_ASM_MOS6502_H__

#include "asm_base.h"
#include "config_mos6502.h"
#include "insn_mos6502.h"

namespace libasm {
namespace mos6502 {

struct AsmMos6502 final : Assembler, Config {
    AsmMos6502(const ValueParser::Plugins &plugins = defaultPlugins());

    void reset() override;

    Error setLongAccumulator(bool enable);
    Error setLongIndex(bool enable);

private:
    const BoolOption<AsmMos6502> _opt_longa;
    const BoolOption<AsmMos6502> _opt_longi;

    bool _longAccumulator;
    bool _longIndex;

    Error selectMode(char size, Operand &op, AddrMode zp, AddrMode abs, AddrMode labs) const;
    Error parseOpenIndirect(StrScanner &scan, Operand &op, char &indirect) const;
    Error parseCloseIndirect(StrScanner &scan, Operand &op, char &indirect) const;
    Error parseOperand(StrScanner &scan, Operand &op, char &indirect) const;

    void emitImmediate(AsmInsn &insn, const Operand &op, bool imm16) const;
    void encodeRelative(AsmInsn &insn, AddrMode mode, const Operand &op) const;
    void encodeOperand(AsmInsn &insn, AddrMode mode, const Operand &op) const;

    Error processPseudo(StrScanner &scan, Insn &insn) override;
    Error encodeImpl(StrScanner &scan, Insn &insn) const override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueParser::Plugins &defaultPlugins();
};

}  // namespace mos6502
}  // namespace libasm

#endif  // __LIBASM_ASM_MOS6502_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

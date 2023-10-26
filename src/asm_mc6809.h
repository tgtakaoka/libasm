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

#ifndef __ASM_MC6809_H__
#define __ASM_MC6809_H__

#include "asm_base.h"
#include "config_mc6809.h"
#include "insn_mc6809.h"

namespace libasm {
namespace mc6809 {

struct AsmMc6809 final : Assembler, Config {
    AsmMc6809(const ValueParser::Plugins &plugins = defaultPlugins());

    void reset() override;

    Error setDirectPage(int32_t val);

private:
    const IntOption<AsmMc6809> _opt_setdp;

    uint8_t _direct_page;

    bool onDirectPage(Config::uintptr_t addr) const;

    bool parseBitPosition(StrScanner &scan, Operand &op) const;
    bool parseMemBit(StrScanner &scan, Operand &op) const;
    Error parseOperand(StrScanner &scan, Operand &op, AddrMode hint) const;

    void encodeRegisterList(AsmInsn &insn, const Operand &op) const;
    void encodeRegisterPair(AsmInsn &insn, const Operand &op) const;
    void encodeRelative(AsmInsn &insn, const Operand &op, AddrMode mode) const;
    Config::ptrdiff_t calculateDisplacement(const AsmInsn &insn, const Operand &op) const;
    void encodeIndexed(AsmInsn &insn, const Operand &op) const;
    char transferMemoryMode(Operand &op) const;
    void encodeTransferMemory(AsmInsn &insn, Operand &op1, Operand &op2) const;
    void encodeOperand(AsmInsn &insn, const Operand &op, AddrMode mode) const;

    Error processPseudo(StrScanner &scan, Insn &insn) override;
    Error encodeImpl(StrScanner &scan, Insn &insn) const override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueParser::Plugins &defaultPlugins();
};

}  // namespace mc6809
}  // namespace libasm

#endif  // __ASM_MC6809_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

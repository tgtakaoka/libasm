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

#ifndef __LIBASM_ASM_PDP11_H__
#define __LIBASM_ASM_PDP11_H__

#include "asm_base.h"
#include "config_pdp11.h"
#include "insn_pdp11.h"

namespace libasm {
namespace pdp11 {

struct AsmPdp11 final : Assembler, Config {
    AsmPdp11(const ValueParser::Plugins &plugins = defaultPlugins());

    void reset() override;

    Error setImplicitWord(bool enable);

private:
    const BoolOption<AsmPdp11> _opt_implicitWord;

    bool _implicitWord;

    RegName parseRegExpr(StrScanner &scan, Operand &op, char delim = 0) const;
    Error isRegDeferred(StrScanner &scan, Operand &op) const;
    Error parseRegMode(StrScanner &scan, Operand &op) const;
    Error parseOperand(StrScanner &scan, Operand &op) const;
    void encodeRelative(AsmInsn &insn, const Operand &op, AddrMode mode, OprPos pos) const;
    void encodeImmediate(AsmInsn &insn, const Operand &op, AddrMode mode, OprPos pos) const;
    void encodeGeneral(AsmInsn &insn, const Operand &op, AddrMode mode, OprPos pos) const;
    void encodeRegister(AsmInsn &insn, const Operand &op, AddrMode mode, OprPos pos) const;
    void encodeOperand(AsmInsn &insn, const Operand &op, AddrMode mode, OprPos pos) const;

    Error setRadix(StrScanner &scan, Insn &insn, uint16_t);
    enum DecType : char {
        DATA_FLT2 = 'F',  // 32-bit DEC Single Precision floating point
        DATA_FLT4 = 'D',  // 64-bit DEC Double Precision floating point
    };
    Error defineDecFloat(StrScanner &scan, Insn &insn, uint16_t type);
    Error defineAscii(StrScanner &scan, Insn &insn, uint16_t);
    Error processPseudo(StrScanner &scan, Insn &insn) override;
    Error encodeImpl(StrScanner &scan, Insn &insn) const override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueParser::Plugins &defaultPlugins();

    using PseudoPdp11 = pseudo::__Pseudo<AsmPdp11>;
    using PseudosPdp11 = pseudo::__Pseudos<PseudoPdp11>;
    static const PseudoPdp11 PSEUDO_PDP11_TABLE[] PROGMEM;
    static const PseudosPdp11 PSEUDOS_PDP11 PROGMEM;
};

}  // namespace pdp11
}  // namespace libasm

#endif  // __LIBASM_ASM_PDP11_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

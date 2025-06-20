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

#ifndef __LIBASM_ASM_MC68000_H__
#define __LIBASM_ASM_MC68000_H__

#include "asm_base.h"
#include "config_mc68000.h"
#include "insn_mc68000.h"

namespace libasm {
namespace mc68000 {

struct AsmMc68000 final : Assembler, Config {
    AsmMc68000(const ValueParser::Plugins &plugins = defaultPlugins());

    void reset() override;

private:
    const TextOption<Config> _opt_fpu;
    const TextOption<Config> _opt_pmmu;

    Error parseCacheList(StrScanner &scan, Operand &op) const;
    Error parseDisplacement(StrScanner &scan, Displacement &disp, ErrorAt &error, char delim) const;
    Error parseIndexScale(StrScanner &scan, Indexing &index, ErrorAt &error, char delim) const;
    Error parseIndexSuffix(StrScanner &scan, Indexing &index, ErrorAt &error, char delim) const;
    Error parseAddressing(StrScanner &scan, Addressing &addr, ErrorAt &error, char delim) const;
    Error parsePointerPair(StrScanner &scan, Operand &op) const;
    Error parseOperand(StrScanner &scan, Operand &op) const;
    Error parseBitField(StrScanner &scan, Operand &op) const;
    Error checkAlignment(AsmInsn &insn, OprSize size, const Value &val, const ErrorAt &) const;

    void encodeBriefExtension(AsmInsn &insn, const Addressing &addr) const;
    void encodeFullExtension(AsmInsn &insn, const Addressing &addr) const;
    void encodeEffectiveAddress(AsmInsn &insn, const Operand &op, OprPos pos) const;
    AddrMode branchType(AddrMode mode, InsnSize size, Config::ptrdiff_t delta) const;
    void encodeRelativeAddr(AsmInsn &insn, AddrMode mode, const Operand &op) const;
    bool encodeMiscImmediate(
            AsmInsn &insn, const Operand &op, AddrMode mode, OprPos pos, OprSize size) const;
    void encodeImmediate(AsmInsn &insn, const Operand &op, OprSize size) const;
    void encodeAbsoluteMode(AsmInsn &insn, OprSize size, OprPos pos, const Addressing &addr,
            const ErrorAt &at) const;
    void encodeCacheList(AsmInsn &insn, const Operand &op) const;
    void encodeFloatControlList(AsmInsn &insn, const Operand &o) const;
    void encodeFloatRegisterList(AsmInsn &insn, const Operand &o) const;
    void encodePmove(AsmInsn &insn, const Operand &op, AddrMode mode) const;
    void encodeRegisterList(AsmInsn &insn, const Operand &op, bool reverse = false) const;
    void encodeBitField(AsmInsn &insn, const Operand &op) const;
    void encodeDataRegPair(AsmInsn &insn, const Operand &op, OprPos pos) const;
    void encodePointerPair(AsmInsn &insn, const Operand &op) const;
    int_fast8_t encodeModeReg(AsmInsn &insn, AddrMode mode, RegName reg, OprPos pos) const;
    Error encodeOperand(
            AsmInsn &insn, OprSize size, const Operand &op, AddrMode mode, OprPos pos) const;

    enum Mc68881Type : char {
        DATA_DCX = 'X',  // 96-bit MC68881 Extended Binary Real
        DATA_DCP = 'P',  // 96-bit MC68881 Packed Decimal String
    };
    Error defineDataConstant(
            AsmInsn &insn, StrScanner &scan, Mc68881Type type, ErrorAt &error) const;
    Error processPseudo(StrScanner &scan, Insn &insn) override;
    Error encodeImpl(StrScanner &scan, Insn &insn) const override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueParser::Plugins &defaultPlugins();
};

}  // namespace mc68000
}  // namespace libasm

#endif  // __LIBASM_ASM_MC68000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

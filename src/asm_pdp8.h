/*
 * Copyright 2023 Tadashi G. Takaoka
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

#ifndef __LIBASM_ASM_PDP8_H__
#define __LIBASM_ASM_PDP8_H__

#include "asm_base.h"
#include "config_pdp8.h"
#include "insn_pdp8.h"

namespace libasm {
namespace pdp8 {

struct AsmPdp8 final : Assembler, Config {
    AsmPdp8(const ValueParser::Plugins &plugins = defaultPlugins());

    void reset() override;

private:
    Error setInputRadix(StrScanner &scan, Insn &insn, uint8_t extra);
    Error defineDoubleDecimal(StrScanner &scan, Insn &insn, uint8_t extra = 0);
    Error alignOnPage(StrScanner &scan, Insn &insn, uint8_t extra = 0);
    Error defineField(StrScanner &scan, Insn &insn, uint8_t extra = 0);

    Error parseMemReferenceOperand(StrScanner &scan, AsmInsn &insn) const;
    Error parseIoTransferOperand(StrScanner &scan, AsmInsn &insn) const;
    Error parseField(StrScanner &scan, AsmInsn &insn) const;
    Error parseMemExtensionOperand(StrScanner &scan, AsmInsn &insn) const;
    Error parseOperateOperand(StrScanner &scan, AsmInsn &insn) const;
    Error encodeMicro(AsmInsn &insn, const AsmInsn &micro, Config::opcode_t &done) const;

    Error processPseudo(StrScanner &scan, Insn &insn) override;
    Error encodeImpl(StrScanner &scan, Insn &insn) const override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueParser::Plugins &defaultPlugins();
    static const pseudo::Pseudo PSEUDOS[] PROGMEM;
};

}  // namespace pdp8
}  // namespace libasm

#endif  // __LIBASM_ASM_PDP8_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

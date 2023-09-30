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

#ifndef __ASM_IM6100_H__
#define __ASM_IM6100_H__

#include "asm_base.h"
#include "config_im6100.h"
#include "insn_im6100.h"

namespace libasm {
namespace im6100 {

struct AsmIm6100 final : Assembler, Config {
    AsmIm6100(const ValueParser::Plugins &plugins = defaultPlugins());

    void reset() override;

private:
    Error parseMemReferenceOperand(StrScanner &scan, AsmInsn &insn);
    Error parseIoTransferOperand(StrScanner &scan, AsmInsn &insn);
    Error parseOperateOperand(StrScanner &scan, AsmInsn &insn);

    Error encodeMicro(AsmInsn &insn, const AsmInsn &micro, Config::opcode_t &done);

    Error setInputRadix(StrScanner &scan, Insn &insn, uint8_t extra);
    Error defineDoubleDecimal(StrScanner &scan, Insn &insn, uint8_t extra = 0);
    Error alignOnPage(StrScanner &scan, Insn &insn, uint8_t extra = 0);
    Error processPseudo(StrScanner &scan, Insn &insn) override;

    Error encodeImpl(StrScanner &scan, Insn &insn) override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueParser::Plugins &defaultPlugins();
    static const pseudo::Pseudo PSEUDOS[] PROGMEM;
};

}  // namespace im6100
}  // namespace libasm

#endif  // __ASM_IM6100_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

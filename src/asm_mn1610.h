/*
 * Copyright 2021 Tadashi G. Takaoka
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

#ifndef __ASM_MN1610_H__
#define __ASM_MN1610_H__

#include "asm_base.h"
#include "config_mn1610.h"
#include "insn_mn1610.h"
#include "reg_mn1610.h"
#include "table_mn1610.h"

namespace libasm {
namespace mn1610 {

class AsmMn1610 : public Assembler, public Config {
public:
    AsmMn1610() : Assembler(_parser, TableMn1610), _parser('*') {}

    const ConfigBase &config() const override { return *this; }
    AddressWidth addressWidth() const override { return TableMn1610.addressWidth(); }

private:
    NationalValueParser _parser;

    struct Operand : public ErrorAt {
        AddrMode mode;
        RegName reg;
        CcName cc;
        uint32_t val32;
        Operand() : ErrorAt(), mode(M_NO), reg(REG_UNDEF), cc(CC_UNDEF), val32(0) {}
    };

    Error parseOperand(StrScanner &scan, Operand &op);

    Error checkAddressRange(Config::uintptr_t addr);
    Error encodeIcRelative(InsnMn1610 &insn, const Operand &op);
    Error encodeGenericAddress(InsnMn1610 &insn, const Operand &op);
    Error encodeOperand(InsnMn1610 &insn, const Operand &op, AddrMode mode);
    Error encode(StrScanner &scan, Insn &insn) override;
};

}  // namespace mn1610
}  // namespace libasm

#endif  // __ASM_MN1610_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

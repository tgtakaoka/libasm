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

#ifndef __ASM_INS8070_H__
#define __ASM_INS8070_H__

#include "asm_base.h"
#include "config_ins8070.h"
#include "insn_ins8070.h"
#include "reg_ins8070.h"
#include "table_ins8070.h"

namespace libasm {
namespace ins8070 {

class AsmIns8070 : public Assembler, public Config {
public:
    AsmIns8070();

    const ConfigBase &config() const override { return *this; }

private:
    NationalValueParser _parser;

    struct Operand : public ErrorAt {
        OprFormat format;
        RegName reg;
        bool autoIndex;
        uint16_t val16;
        Operand() : ErrorAt(), format(OPR_NO), reg(REG_UNDEF), autoIndex(false), val16(0) {}
    };

    Error parseOperand(StrScanner &scan, Operand &op);

    Error emitAbsolute(InsnIns8070 &insn, const Operand &op);
    Error emitImmediate(InsnIns8070 &insn, const Operand &op);
    Error emitRelative(InsnIns8070 &insn, const Operand &op);
    Error emitGeneric(InsnIns8070 &insn, const Operand &op);
    Error emitOperand(InsnIns8070 &insn, OprFormat format, const Operand &op);
    Error encode(StrScanner &scan, Insn &insn) override;
};

}  // namespace ins8070
}  // namespace libasm

#endif  // __ASM_INS8070_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

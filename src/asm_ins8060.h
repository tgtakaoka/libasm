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

#ifndef __ASM_INS8060_H__
#define __ASM_INS8060_H__

#include "asm_base.h"
#include "config_ins8060.h"
#include "insn_ins8060.h"
#include "reg_ins8060.h"
#include "table_ins8060.h"

namespace libasm {
namespace ins8060 {

class AsmIns8060 : public Assembler, public Config {
public:
    AsmIns8060() : Assembler(_parser, TableIns8060) {}

private:
    ValueParser _parser{'$'};

    struct Operand : public ErrorReporter {
        AddrMode mode;
        RegName reg;
        uint16_t val16;
        Operand() : ErrorReporter(), mode(UNDEF), reg(REG_UNDEF), val16(0) {}
    };

    Error parseOperand(const char *scan, Operand &opr);

    Error encodeRel8(InsnIns8060 &insn, const Operand &opr);
    Error encodeIndx(InsnIns8060 &insn, const Operand &opr);

    Error encode(Insn &insn) override;
};

}  // namespace ins8060
}  // namespace libasm

#endif  // __ASM_INS8060_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

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

#ifndef __ASM_TMS32010_H__
#define __ASM_TMS32010_H__

#include "asm_base.h"
#include "config_tms32010.h"
#include "insn_tms32010.h"
#include "reg_tms32010.h"
#include "table_tms32010.h"

namespace libasm {
namespace tms32010 {

class AsmTms32010 : public Assembler, public Config {
public:
    AsmTms32010() : Assembler(_parser, TableTms32010) {}
    ConfigBase &config() override { return *this; }

private:
    IntelValueParser _parser;

    struct Operand : public ErrorReporter {
        AddrMode mode;
        RegName reg;
        uint16_t val16;
        Operand() : ErrorReporter(), mode(M_NO), reg(REG_UNDEF), val16(0) {}
    };

    Error parseOperand(const char *scan, Operand &op);

    Error encodeOperand(InsnTms32010 &insn, const Operand &op, AddrMode mode);
    Error encode(Insn &insn) override;
};

}  // namespace tms32010
}  // namespace libasm

#endif  // __ASM_TMS32010_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

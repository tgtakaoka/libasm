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

#ifndef __ASM_I8051_H__
#define __ASM_I8051_H__

#include "asm_base.h"
#include "config_i8051.h"
#include "insn_i8051.h"
#include "reg_i8051.h"
#include "table_i8051.h"

namespace libasm {
namespace i8051 {

class AsmI8051 : public Assembler, public Config {
public:
    AsmI8051() : Assembler(_parser, TableI8051) {}

private:
    IntelValueParser _parser;

    struct Operand : public ErrorReporter {
        AddrMode mode;
        RegName reg;
        uint16_t val16;
        Operand() : ErrorReporter(), mode(NONE), reg(REG_UNDEF), val16(0) {}
    };

    Error parseOperand(const char *scan, Operand &op);

    Error encodeOperand(InsnI8051 &insn, const AddrMode mode, const Operand &op);
    Error encode(Insn &insn) override;
};

}  // namespace i8051
}  // namespace libasm

#endif  // __ASM_I8051_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

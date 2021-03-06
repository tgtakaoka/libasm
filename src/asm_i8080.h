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

#ifndef __ASM_I8080_H__
#define __ASM_I8080_H__

#include "asm_base.h"
#include "config_i8080.h"
#include "insn_i8080.h"
#include "reg_i8080.h"
#include "table_i8080.h"

namespace libasm {
namespace i8080 {

class AsmI8080 : public Assembler, public Config {
public:
    AsmI8080() : Assembler(_parser, TableI8080) {}

private:
    IntelValueParser _parser;

    struct Operand : public ErrorReporter {
        AddrMode mode;
        RegName reg;
        uint16_t val16;
        Operand() : ErrorReporter(), mode(M_NO), reg(REG_UNDEF), val16(0) {}
    };

    Error parseOperand(const char *scan, Operand &op);

    Error encodeOperand(InsnI8080 &insn, const Operand &op, AddrMode mode);
    Error encode(Insn &insn) override;
};

}  // namespace i8080
}  // namespace libasm

#endif  // __ASM_I8080_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

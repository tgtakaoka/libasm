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

#ifndef __ASM_MC6800_H__
#define __ASM_MC6800_H__

#include "asm_base.h"
#include "config_mc6800.h"
#include "insn_mc6800.h"
#include "reg_mc6800.h"
#include "table_mc6800.h"

namespace libasm {
namespace mc6800 {

class AsmMc6800
    : public Assembler,
      public Config {
public:
    ValueParser *getParser() override { return &_parser; }

    // Config
    const char *listCpu() const override { return TableMc6800.listCpu(); }
    bool setCpu(const char *cpu) override { return TableMc6800.setCpu(cpu); }
    const char *getCpu() const override { return TableMc6800.getCpu(); }

private:
    MotoValueParser _parser;
    RegMc6800 _regs;

    enum Token : char {
        EOL = 0,
        ERROR = '?',
        COMMA = ',',
        REG_ACC = 'A',
        REG_IDX = 'x',
        VAL_IMM = '#',
        VAL_ADR = 'v',
    };
    Token _token;
    RegName _reg;
    uint16_t _val;
    OprSize _valSize;
    Token nextToken();

    struct Operand : public ErrorReporter {
        AddrMode mode;
        RegName reg;
        uint16_t imm;
        uint16_t opr;
        uint16_t addr;
        Error addrError;
        Operand()
            : ErrorReporter(),
              mode(INH),
              reg(REG_UNDEF),
              imm(0),
              opr(0),
              addr(0),
              addrError(OK)
        {}
    };

    Error adjustAccumulator(InsnMc6800 &insn, const Operand &op);
    Error parseOperand(Operand &op);
    Error encodeRelative(
        InsnMc6800 &insn, Config::uintptr_t target, Error error);
    Error encode(Insn &insn) override;
};

} // namespace mc6800
} // namespace libasm

#endif // __ASM_MC6800_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

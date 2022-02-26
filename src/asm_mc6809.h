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

#ifndef __ASM_MC6809_H__
#define __ASM_MC6809_H__

#include "asm_base.h"
#include "config_mc6809.h"
#include "insn_mc6809.h"
#include "reg_mc6809.h"
#include "table_mc6809.h"

namespace libasm {
namespace mc6809 {

class AsmMc6809 : public Assembler, public Config {
public:
    AsmMc6809() : Assembler(_parser, TableMc6809), _parser(), _direct_page(0) {}

    const ConfigBase &config() const override { return *this; }
    void reset() override { _direct_page = 0; }

private:
    MotorolaValueParser _parser;
    uint8_t _direct_page;

    struct Operand : public ErrorReporter {
        AddrMode mode;
        RegName index;
        RegName base;
        bool indir;
        int8_t extra;
        uint32_t val32;
        Operand()
            : ErrorReporter(),
              mode(NONE),
              index(REG_UNDEF),
              base(REG_UNDEF),
              indir(false),
              extra(0),
              val32(0) {}
    };

    bool parsePointerMode(StrScanner &scan, Operand &op, bool indir) __attribute__((noinline));
    bool parseIndexedMode(StrScanner &scan, Operand &op, bool indir);
    bool parseBitPosition(StrScanner &scan, Operand &op);
    bool parseRegisterList(StrScanner &scan, Operand &op, bool indir);
    Error parseOperand(StrScanner &scan, Operand &op);
    Error processPseudo(StrScanner &scan, InsnMc6809 &insn);

    Error encodePushPull(InsnMc6809 &insn, const Operand &op);
    Error encodeRegisters(InsnMc6809 &insn, const Operand &op);
    Error encodeRelative(InsnMc6809 &insn, const Operand &op, AddrMode mode);
    Config::ptrdiff_t calculateDisplacement(const InsnMc6809 &insn, const Operand &op) const;
    Error encodeIndexed(InsnMc6809 &insn, const Operand &op);
    char transferMemoryMode(const Operand &op);
    Error encodeTransferMemory(InsnMc6809 &insn, const Operand &op1, const Operand &op2);
    Error encodeOperand(InsnMc6809 &insn, const Operand &op, AddrMode mode);
    Error encode(StrScanner &scan, Insn &insn) override;
};

}  // namespace mc6809
}  // namespace libasm

#endif  // __ASM_MC6809_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

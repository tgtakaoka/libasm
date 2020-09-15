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

class AsmMc6809
    : public Assembler,
      public Config {
public:
    ValueParser *getParser() override { return &_parser; }
    void reset() override { _direct_page = 0; }

private:
    MotoValueParser _parser;
    RegMc6809 _regs;
    uint8_t _direct_page;

    TableBase &getTable() const override { return TableMc6809; }

    struct Operand : public ErrorReporter {
        AddrMode mode;
        IndexedSubMode sub;
        RegName index;
        RegName base;
        bool indir;
        int8_t extra;
        uint32_t val32;
        Operand()
            : ErrorReporter(),
              mode(NONE),
              sub(PNTR_IDX),
              index(REG_UNDEF),
              base(REG_UNDEF),
              indir(false),
              extra(0),
              val32(0)
        {}
    };

    bool parsePointerMode(Operand &op, const char *scan, bool indir);
    bool parseIndexedMode(Operand &op, const char *scan, bool indir);
    bool parseBitRegister(Operand &op, const char *scan);
    bool parseBitPosition(Operand &op, const char *scan);
    bool parseRegisterList(Operand &op, const char *scan, bool indir);
    Error parseOperand(Operand &op);

    Error encodePushPull(InsnMc6809 &insn, const Operand &op);
    Error encodeRegisters(InsnMc6809 &insn, const Operand &op);
    Error encodeRelative(InsnMc6809 &insn, const Operand &op);
    Error encodeImmediate(
        InsnMc6809 &insn, const Operand &op, Operand &extra);
    Config::ptrdiff_t calculateDisplacement(
        const InsnMc6809 &insn, const Operand &op) const;
    Error encodeIndexed(
        InsnMc6809 &insn, Operand &op, bool emitInsn = true);
    Error encodeBitOperation(
        InsnMc6809 &insn, const Operand &op, const Operand &extra);
    Error encodeTransferMemory(
        InsnMc6809 &insn, const Operand &op, const Operand &extra);

    Error processPseudo(InsnMc6809 &insn);
    Error encode(Insn &insn) override;
};

} // namespace mc6809
} // namespace libasm

#endif // __ASM_MC6809_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

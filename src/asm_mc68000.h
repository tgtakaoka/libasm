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

#ifndef __ASM_MC68000_H__
#define __ASM_MC68000_H__

#include "asm_base.h"
#include "config_mc68000.h"
#include "insn_mc68000.h"
#include "reg_mc68000.h"
#include "table_mc68000.h"

namespace libasm {
namespace mc68000 {

class AsmMc68000
    : public Assembler,
      public Config {
public:
    ValueParser *getParser() override { return &_parser; }

    // Config
    const char *listCpu() const override { return TableMc68000.listCpu(); }
    bool setCpu(const char *cpu) override { return TableMc68000.setCpu(cpu); }
    const char *getCpu() const override { return TableMc68000.getCpu(); }

    void setOptimize(bool enabled) { _optimize = enabled; }

private:
    MotoValueParser _parser;
    bool _optimize = false;

public:
    struct Operand : public ErrorReporter {
        EaMode mode;
        RegName reg;
        RegName index;          // index register
        EaSize size;            // index size
        uint32_t val32;
        bool satisfy(uint8_t categories) const {
            return EaMc68000::satisfy(mode, categories);
        }
        void reset() {
            setOK();
            mode = M_NONE;
            reg = index = REG_UNDEF;
            size = SZ_NONE;
            val32 = 0;
        }
    };
private:

    Error checkSize(InsnMc68000 &insn, const EaSize size);
    Error checkSize(const uint32_t val32, const EaSize size, bool uint);
    Error parseOperand(Operand &opr);
    Error parseMoveMultiRegList(Operand &opr);

    Error emitImmediateData(
        InsnMc68000 &insn, EaSize size, uint32_t val, Error error);
    Error emitEffectiveAddr(
        InsnMc68000 &insn,
        const Operand &ea,
        int8_t size_gp = 6,
        int8_t mode_gp = 3,
        uint8_t reg_gp = 0);

    Error encodeImplied(
        InsnMc68000 &insn, const Operand &op1, const Operand &op2);
    Error encodeDestSiz(
        InsnMc68000 &insn, const Operand &op1, const Operand &op2);
    Error encodeAddrReg(
        InsnMc68000 &insn, const Operand &op1, const Operand &op2);
    Error encodeDataReg(
        InsnMc68000 &insn, const Operand &op1, const Operand &op2);
    Error encodeTrapVec(
        InsnMc68000 &insn, const Operand &op1, const Operand &op2);
    Error encodeDataDst(
        InsnMc68000 &insn, const Operand &op1, const Operand &op2);
    Error encodeDestOpr(
        InsnMc68000 &insn, const Operand &op1, const Operand &op2);
    Error encodeSignExt(
        InsnMc68000 &insn, const Operand &op1, const Operand &op2);
    Error encodeRelative(
        InsnMc68000 &insn, const Operand &op1, const Operand &op2);
    Error encodeMoveMlt(
        InsnMc68000 &insn, const Operand &op1, const Operand &op2);
    Error encodeMoveQic(
        InsnMc68000 &insn, const Operand &op1, const Operand &op2);
    Error encodeMovePer(
        InsnMc68000 &insn, const Operand &op1, const Operand &op2);
    Error encodeAregSiz(
        InsnMc68000 &insn, const Operand &op1, const Operand &op2);
    Error encodeDregDst(
        InsnMc68000 &insn, const Operand &op1, const Operand &op2);
    Error encodeDataQic(
        InsnMc68000 &insn, const Operand &op1, const Operand &op2);
    Error encodeDmemOpr(
        InsnMc68000 &insn, const Operand &op1, const Operand &op2);
    Error encodeDmemSiz(
        InsnMc68000 &insn, const Operand &op1, const Operand &op2);
    Error encodeRegsExg(
        InsnMc68000 &insn, const Operand &op1, const Operand &op2);
    Error encodeMoveOpr(
        InsnMc68000 &insn, const Operand &op1, const Operand &op2);

    Error encode(Insn &insn) override;
};

} // namespace mc68000
} // namespace libasm

#endif // __ASM_MC68000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

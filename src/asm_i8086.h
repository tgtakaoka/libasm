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

#ifndef __ASM_I8086_H__
#define __ASM_I8086_H__

#include "asm_base.h"
#include "config_i8086.h"
#include "insn_i8086.h"
#include "reg_i8086.h"
#include "table_i8086.h"

namespace libasm {
namespace i8086 {

class AsmI8086 : public Assembler, public Config {
public:
    AsmI8086() : Assembler(_parser, TableI8086::TABLE, _pseudos), _parser(), _pseudos() { reset(); }

    const ConfigBase &config() const override { return *this; }
    void reset() override { _optimizeSegment = false; }
    const Options &options() const override { return _options; }

private:
    IntelValueParser _parser;
    PseudoBase _pseudos;

    bool _optimizeSegment;
    const struct OptOptimizeSeg : public BoolOption {
        OptOptimizeSeg(bool &var);
    } _opt_optimizeSegment{_optimizeSegment};
    const Options _options{_opt_optimizeSegment};

    struct Operand : public OperandBase {
        AddrMode mode;
        RegName ptr;
        RegName seg;
        RegName reg;
        RegName index;
        bool hasVal;
        uint32_t val32;
        uint16_t seg16;
        Operand()
            : mode(M_NONE),
              ptr(REG_UNDEF),
              seg(REG_UNDEF),
              reg(REG_UNDEF),
              index(REG_UNDEF),
              hasVal(false),
              val32(0),
              seg16(0) {}
        uint8_t encodeMod() const;
        uint8_t encodeR_m() const;
        AddrMode immediateMode() const;
        void print(const char *) const;
    };

    Error parseStringInst(StrScanner &scan, Operand &op) const;
    Error parsePointerSize(StrScanner &scan, Operand &op) const;
    void parseSegmentOverride(StrScanner &scan, Operand &op) const;
    void parseBaseRegister(StrScanner &scan, Operand &op) const;
    void parseIndexRegister(StrScanner &scan, Operand &op) const;
    Error parseDisplacement(StrScanner &scan, Operand &op) const;
    Error parseOperand(StrScanner &scan, Operand &opr) const;

    void emitImmediate(InsnI8086 &insn, const Operand &op, OprSize size, uint32_t val);
    void emitRelative(InsnI8086 &insn, const Operand &op, AddrMode mode);
    void emitRegister(InsnI8086 &insn, const Operand &op, OprPos pos);
    Config::opcode_t encodeSegmentOverride(RegName seg, RegName base);
    void emitModReg(InsnI8086 &insn, const Operand &op, OprPos pos);
    void emitDirect(InsnI8086 &insn, const Operand &op, OprPos pos);
    void emitOperand(InsnI8086 &insn, AddrMode mode, const Operand &op, OprPos pos);
    void emitStringOperand(InsnI8086 &insn, const Operand &op, RegName seg, RegName index);
    void emitStringInst(InsnI8086 &insn, const Operand &src, const Operand &dst);
    Error encodeImpl(StrScanner &scan, Insn &insn) override;
};

}  // namespace i8086
}  // namespace libasm

#endif  // __ASM_I8086_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

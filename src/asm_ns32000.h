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

#ifndef __ASM_NS32000_H__
#define __ASM_NS32000_H__

#include "asm_base.h"
#include "config_ns32000.h"
#include "insn_ns32000.h"
#include "reg_ns32000.h"
#include "table_ns32000.h"

namespace libasm {
namespace ns32000 {

class AsmNs32000 : public Assembler, public Config {
public:
    AsmNs32000() : Assembler(_parser, TableNs32000) {}

    void reset() override {
        setFpu(FPU_NONE);
        setMmu(MMU_NONE);
    }
    void setFpu(FpuType fpuType) { TableNs32000.setFpu(fpuType); }
    void setMmu(MmuType mmuType) { TableNs32000.setMmu(mmuType); }

private:
    ValueParser _parser{'*'};

    struct Operand : public ErrorReporter {
        AddrMode mode;
        RegName reg;
        union {
            struct {
                uint32_t val32;
                uint32_t disp2;
            };
            double float64;
        };
        RegName index;
        OprSize size;
        Operand()
            : ErrorReporter(),
              mode(M_NONE),
              reg(REG_UNDEF),
              val32(0),
              disp2(0),
              index(REG_UNDEF),
              size(SZ_NONE) {}
    };

    Error parseStrOptNames(const char *scan, Operand &op, bool braket = false);
    Error parseConfigNames(const char *scan, Operand &op);
    Error parseRegisterList(const char *scan, Operand &op);
    Error parseBaseOperand(const char *scan, Operand &op);
    Error parseOperand(const char *scan, Operand &op);
    Error processPseudo(const char *scan, const InsnNs32000 &insn);
    Error emitDisplacement(InsnNs32000 &insn, uint32_t val32);
    Error emitLength(InsnNs32000 &insn, AddrMode mode, const Operand &op);
    Error emitBitField(InsnNs32000 &insn, AddrMode mode, const Operand *off,
            const Operand &len);
    Error emitImmediate(InsnNs32000 &insn, const Operand &op, OprSize size);
    Error emitIndexByte(InsnNs32000 &insn, const Operand &op) const;
    uint8_t encodeGenericField(AddrMode mode, RegName reg) const;
    Error emitGeneric(
            InsnNs32000 &insn, AddrMode mode, const Operand &op, OprPos pos);
    Error emitRelative(InsnNs32000 &insn, const Operand &op);
    Error emitOperand(InsnNs32000 &insn, AddrMode mode, OprSize size,
            const Operand &op, OprPos pos, const Operand *prevOp = nullptr);
    Error encode(Insn &insn) override;
};

}  // namespace ns32000
}  // namespace libasm

#endif  // __ASM_NS32000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

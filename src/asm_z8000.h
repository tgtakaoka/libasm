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
#ifndef __ASM_Z8000_H__
#define __ASM_Z8000_H__

#include "asm_base.h"
#include "config_z8000.h"
#include "insn_z8000.h"
#include "reg_z8000.h"

namespace libasm {
namespace z8000 {

class AsmZ8000 final : public Assembler, public Config {
public:
    AsmZ8000(const ValueParser::Plugins &plugins = defaultPlugins());

    void reset() override;

    Error setShortDirect(bool enable);

    bool hasSetInstruction() const override { return true; }

private:
    const BoolOption<AsmZ8000> _opt_shortDitrect;

    bool _autoShortDirect;

    int8_t parseIntrNames(StrScanner &scan) const;
    int8_t parseFlagNames(StrScanner &scan) const;

    struct Operand;
    Error parseOperand(StrScanner &scan, Operand &op);

    void emitData(AsmInsn &insn, ModeField field, Config::opcode_t data);
    void emitRegister(AsmInsn &insn, ModeField field, RegName reg);
    void emitIndirectRegister(AsmInsn &insn, const Operand &op, ModeField field, RegName reg);
    void emitImmediate(AsmInsn &insn, ModeField field, AddrMode mode, const Operand &op);
    void emitDirectAddress(AsmInsn &insn, const Operand &op);
    void emitRelative(AsmInsn &insn, AddrMode mode, const Operand &op);
    void emitIndexed(AsmInsn &insn, ModeField field, const Operand &op);
    void emitBaseAddress(AsmInsn &insn, ModeField field, const Operand &op);
    void emitBaseIndexed(AsmInsn &insn, ModeField field, const Operand &op);
    void emitFlags(AsmInsn &insn, ModeField field, const Operand &op);
    void emitCtlRegister(AsmInsn &insn, ModeField field, const Operand &op);
    void emitOperand(AsmInsn &insn, AddrMode mode, const Operand &op, ModeField field);
    void checkRegisterOverlap(const Operand &dstOp, const Operand &srcOp, RegName cnt = REG_UNDEF);
    void checkRegisterOverlap(
            const AsmInsn &insn, const Operand &dstOp, const Operand &srcOp, const Operand &cntOp);

    Error encodeImpl(StrScanner &scan, Insn &insn) override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueParser::Plugins &defaultPlugins();
};

}  // namespace z8000
}  // namespace libasm

#endif  // __ASM_Z8000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

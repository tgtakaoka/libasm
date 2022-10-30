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
#include "table_z8000.h"

namespace libasm {
namespace z8000 {

class AsmZ8000 : public Assembler, public Config {
public:
    AsmZ8000() : Assembler(_parser, TableZ8000::TABLE), _parser() {
        reset();
        //_options.registerOption(_opt_shortDitrect);
    }

    const ConfigBase &config() const override { return *this; }
    AddressWidth addressWidth() const override { return TableZ8000::TABLE.addressWidth(); }
    void reset() override { _autoShortDirect = false; }
    const Options &options() const override { return _options; }

private:
    IntelValueParser _parser;
    bool _autoShortDirect;
    const BoolOption _opt_shortDitrect{
            OPT_BOOL_SHORT_DIRECT, OPT_DESC_SHORT_DIRECT, _autoShortDirect};
    const Options _options{_opt_shortDitrect};

    struct Operand : public OperandBase {
        AddrMode mode;
        RegName reg;     // M_R/M_IR/M_X/M_BX/M_CTL
        RegName base;    // M_BA/M_BX
        CcName cc;       // M_CC/M_DA/M_X
        uint32_t val32;  // M_IM/M_DA/M_X/M_BA/M_INTT/M_FLAG
        Operand() : mode(M_NONE), reg(REG_UNDEF), base(REG_UNDEF), cc(CC_UNDEF), val32(0) {}
    };

    int8_t parseIntrNames(StrScanner &scan) const;
    int8_t parseFlagNames(StrScanner &scan) const;
    Error parseOperand(StrScanner &scan, Operand &op);

    Error emitData(InsnZ8000 &insn, ModeField field, Config::opcode_t data);
    Error emitRegister(InsnZ8000 &insn, ModeField field, RegName reg);
    Error emitIndirectRegister(InsnZ8000 &insn, const Operand &op, ModeField field, RegName reg);
    Error emitImmediate(InsnZ8000 &insn, ModeField field, AddrMode mode, const Operand &op);
    Error emitDirectAddress(InsnZ8000 &insn, const Operand &op);
    Error emitRelative(InsnZ8000 &insn, AddrMode mode, const Operand &op);
    Error emitIndexed(InsnZ8000 &insn, ModeField field, const Operand &op);
    Error emitBaseAddress(InsnZ8000 &insn, ModeField field, const Operand &op);
    Error emitBaseIndexed(InsnZ8000 &insn, ModeField field, const Operand &op);
    Error emitFlags(InsnZ8000 &insn, ModeField field, const Operand &op);
    Error emitCtlRegister(InsnZ8000 &insn, ModeField field, const Operand &op);
    Error emitOperand(InsnZ8000 &insn, AddrMode mode, const Operand &op, ModeField field);
    Error checkRegisterOverlap(const Operand &dstOp, const Operand &srcOp, RegName cnt = REG_UNDEF);
    Error checkRegisterOverlap(const InsnZ8000 &insn, const Operand &dstOp, const Operand &srcOp,
            const Operand &cntOp);
    Error encodeImpl(StrScanner &scan, Insn &insn) override;

    static const char OPT_BOOL_SHORT_DIRECT[] PROGMEM;
    static const char OPT_DESC_SHORT_DIRECT[] PROGMEM;
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

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

#ifndef __ASM_Z8_H__
#define __ASM_Z8_H__

#include "asm_base.h"
#include "config_z8.h"
#include "insn_z8.h"
#include "reg_z8.h"
#include "table_z8.h"

namespace libasm {
namespace z8 {

class AsmZ8 : public Assembler, public Config {
public:
    AsmZ8() : Assembler(_parser, TableZ8::TABLE), _parser() { reset(); }

    const ConfigBase &config() const override { return *this; }
    void reset() override { setRegPointer(-1); }

    static const char OPT_INT_SETRP[] PROGMEM;
    static const char OPT_INT_SETRP0[] PROGMEM;
    static const char OPT_INT_SETRP1[] PROGMEM;

private:
    IntelValueParser _parser;
    int16_t _regPointer0;
    int16_t _regPointer1;
    struct OptSetrp : public IntOptionBase {
        OptSetrp(const /* PROGMEM */ char *name_P, AsmZ8 *assembler, bool (AsmZ8::*set)(int16_t))
            : IntOptionBase(name_P, assembler->_options), _assembler(assembler), _set(set) {}
        Error check(int32_t value) const override {
            return (_assembler->*_set)(value) ? OK : OPERAND_NOT_ALLOWED;
        }
        void set(int32_t value) const override { (void)value; }
        AsmZ8 *_assembler;
        bool (AsmZ8::*_set)(int16_t);
    };
    const OptSetrp _opt_setrp{OPT_INT_SETRP, this, &AsmZ8::setRegPointer};
    const OptSetrp _opt_setrp0{OPT_INT_SETRP0, this, &AsmZ8::setRegPointer0};
    const OptSetrp _opt_setrp1{OPT_INT_SETRP1, this, &AsmZ8::setRegPointer1};

    bool setRegPointer(int16_t rp);
    bool setRegPointer0(int16_t rp);
    bool setRegPointer1(int16_t rp);
    bool isWorkReg(uint8_t regAddr) const;

    struct Operand : public ErrorAt {
        AddrMode mode;
        RegName reg;
        CcName cc;
        uint16_t val16;
        Operand() : ErrorAt(), mode(M_NO), reg(REG_UNDEF), cc(CC_UNDEF), val16(0) {}
    };

    Error parseOperand(StrScanner &scan, Operand &op);
    Error setRp(StrScanner &scan, bool (AsmZ8::*)(int16_t));
    Error processPseudo(StrScanner &scan, const char *name);

    Error encodeOperand(InsnZ8 &insn, const AddrMode mode, const Operand &op);
    Error encodeAbsolute(InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp);
    Error encodeRelative(InsnZ8 &insn, const Operand &op);
    Error encodeIndexed(InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp);
    Error encodeIndirectRegPair(InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp);
    Error encodeInOpCode(InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp);
    Error encodeMultiOperands(
            InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp, const Operand &extOp);
    Error encodePostByte(
            InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp, const Operand &extOp);
    Error encode(StrScanner &scan, Insn &insn) override;
};

}  // namespace z8
}  // namespace libasm

#endif  // __ASM_Z8_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

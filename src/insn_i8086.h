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

#ifndef __LIBASM_INSN_I8086_H__
#define __LIBASM_INSN_I8086_H__

#include "config_i8086.h"
#include "entry_i8086.h"
#include "insn_base.h"
#include "reg_i8086.h"
#include "value.h"

namespace libasm {
namespace i8086 {

struct EntryInsn : EntryInsnPrefix<Config, Entry> {
    EntryInsn() : _fwait(0), _segment(0) {}

    AddrMode dst() const { return flags().dst(); }
    AddrMode src() const { return flags().src(); }
    AddrMode ext() const { return flags().ext(); }
    OprPos dstPos() const { return flags().dstPos(); }
    OprPos srcPos() const { return flags().srcPos(); }
    OprPos extPos() const { return flags().extPos(); }
    OprSize size() const { return flags().size(); }
    bool stringInst() const { return flags().stringInst(); }
    bool fpuInst() const { return flags().fpuInst(); }

    void setSegment(Config::opcode_t segment) { _segment = segment; }
    Config::opcode_t segment() const { return _segment; }
    void setFwait(bool enable = true) { _fwait = enable ? FWAIT : 0; }
    Config::opcode_t fwait() const { return _fwait; }

    static constexpr Config::opcode_t FWAIT = 0x9B;

protected:
    Config::opcode_t _fwait;
    Config::opcode_t _segment;
};

struct Operand final : ErrorAt {
    AddrMode mode;
    RegName ptr;
    RegName seg;
    RegName reg;
    RegName index;
    bool hasDisp;
    Value val;
    Value segval;
    Operand()
        : mode(M_NONE),
          ptr(REG_UNDEF),
          seg(REG_UNDEF),
          reg(REG_UNDEF),
          index(REG_UNDEF),
          hasDisp(false),
          val(),
          segval() {}
    uint8_t encodeMod() const;
    uint8_t encodeR_m() const;
    AddrMode immediateMode() const;
    void print(const char *) const;
};

struct AsmInsn final : AsmInsnImpl<Config>, EntryInsn {
    AsmInsn(Insn &insn) : AsmInsnImpl(insn), _modReg(0), _hasModReg(false) {}

    Operand dstOp, srcOp, extOp;

    void embedModReg(Config::opcode_t data) {
        _modReg |= data;
        _hasModReg = true;
    }
    Config::opcode_t modReg() const { return _modReg; }

    void prepairModReg() {
        if (hasPrefix())
            return;
        const OprPos dst = dstPos();
        const OprPos src = srcPos();
        if (dst == P_MOD || dst == P_REG || dst == P_MREG || src == P_MOD || src == P_REG)
            embedModReg(0);
    }

    void emitInsn() {
        uint8_t pos = 0;
        if (_fwait)
            emitByte(_fwait, pos++);
        if (_segment)
            emitByte(_segment, pos++);
        if (hasPrefix())
            emitByte(prefix(), pos++);
        emitByte(opCode(), pos++);
        if (_hasModReg)
            emitByte(_modReg, pos);
    }
    void emitOperand8(uint8_t val8) { emitByte(val8, operandPos()); }
    void emitOperand16(uint16_t val16) { emitUint16(val16, operandPos()); }

private:
    Config::opcode_t _modReg;
    bool _hasModReg;

    uint8_t operandPos() const {
        uint8_t pos = length();
        if (pos == 0) {
            if (_fwait)
                pos++;
            if (_segment)
                pos++;
            if (hasPrefix())
                pos++;
            pos++;
            if (_hasModReg)
                pos++;
        }
        return pos;
    }
};

struct DisInsn final : DisInsnImpl<Config>, EntryInsn {
    DisInsn(Insn &insn, DisMemory &memory, const StrBuffer &out)
        : DisInsnImpl(insn, memory, out), _modReg(0), _hasPushBack(false), _pushBack(0) {}
    DisInsn(Insn &insn, DisInsn &o, const StrBuffer &out)
        : DisInsnImpl(insn, o, out), _modReg(0), _hasPushBack(false), _pushBack(0) {}

    uint8_t readByte() override {
        if (_hasPushBack) {
            _hasPushBack = false;
            return _pushBack;
        }
        return DisInsnBase::readByte();
    }

    void pushBack(uint8_t code) {
        _pushBack = code;
        _hasPushBack = true;
    }

    void readModReg() {
        const OprPos dst = dstPos();
        const OprPos src = srcPos();
        if (dst == P_MOD || dst == P_REG || src == P_MOD || src == P_REG)
            _modReg = readByte();
        else if (dst == P_OMOD || src == P_OMOD)
            _modReg = opCode();
    }
    Config::opcode_t modReg() const { return _modReg; }

private:
    Config::opcode_t _modReg;
    bool _hasPushBack;
    Config::opcode_t _pushBack;
};

}  // namespace i8086
}  // namespace libasm

#endif  // __LIBASM_INSN_I8086_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

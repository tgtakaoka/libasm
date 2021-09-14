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

#ifndef __INSN_I8086_H__
#define __INSN_I8086_H__

#include "config_i8086.h"
#include "entry_i8086.h"
#include "insn_base.h"

namespace libasm {
namespace i8086 {

class InsnI8086 : public InsnImpl<Config, Entry> {
public:
    InsnI8086(Insn &insn) : InsnImpl(insn), _segment(0), _modReg(0), _hasModReg(false) {}

    AddrMode dstMode() const { return flags().dstMode(); }
    AddrMode srcMode() const { return flags().srcMode(); }
    OprPos dstPos() const { return flags().dstPos(); }
    OprPos srcPos() const { return flags().srcPos(); }
    OprSize oprSize() const { return flags().size(); }
    bool stringInst() const { return flags().strInst(); }
    void setAddrMode(AddrMode dst, AddrMode src) {
        setFlags(Entry::Flags::create(dst, src, P_NONE, P_NONE, SZ_NONE, false));
    }

    void setSegment(Config::opcode_t segment) { _segment = segment; }

    void setOpCode(Config::opcode_t opCode, Config::opcode_t prefix) {
        _prefix = prefix;
        _opCode = opCode;
    }

    void readModReg(DisMemory &memory) {
        if (_prefix) {
            _modReg = _opCode;
        } else {
            const OprPos dst = dstPos();
            const OprPos src = srcPos();
            if (dst == P_MOD || dst == P_REG || src == P_MOD || src == P_REG)
                _modReg = readByte(memory);
        }
    }

    Config::opcode_t segment() const { return _segment; }
    Config::opcode_t prefix() const { return _prefix; }
    Config::opcode_t opCode() const { return _opCode; }
    Config::opcode_t modReg() const { return _modReg; }

    void embed(Config::opcode_t data) { _opCode |= data; }

    void embedModReg(Config::opcode_t data) {
        _modReg |= data;
        _hasModReg = true;
    }

    void prepairModReg() {
        if (_prefix)
            return;
        const OprPos dst = dstPos();
        const OprPos src = srcPos();
        if (dst == P_MOD || dst == P_REG || src == P_MOD || src == P_REG)
            embedModReg(0);
    }

    void emitInsn() {
        uint8_t pos = 0;
        if (_segment)
            emitByte(_segment, pos++);
        if (_prefix)
            emitByte(_prefix, pos++);
        emitByte(_opCode, pos++);
        if (_hasModReg)
            emitByte(_modReg, pos);
    }

    void emitOperand8(uint8_t val8) { emitByte(val8, operandPos()); }

    void emitOperand16(uint16_t val16) { emitUint16(val16, operandPos()); }

private:
    Config::opcode_t _segment;
    Config::opcode_t _prefix;
    Config::opcode_t _opCode;
    Config::opcode_t _modReg;
    bool _hasModReg;

    uint8_t operandPos() const {
        uint8_t pos = length();
        if (pos == 0) {
            if (_segment)
                pos++;
            if (_prefix)
                pos++;
            pos++;
            if (_hasModReg)
                pos++;
        }
        return pos;
    }
};

}  // namespace i8086
}  // namespace libasm

#endif  // __INSN_I8086_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

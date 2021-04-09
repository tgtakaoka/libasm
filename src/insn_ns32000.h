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

#ifndef __INSN_NS32000_H__
#define __INSN_NS32000_H__

#include "config_ns32000.h"
#include "entry_ns32000.h"
#include "insn_base.h"
#include "reg_ns32000.h"

namespace libasm {
namespace ns32000 {

class InsnNs32000 : public InsnImpl<Config> {
public:
    InsnNs32000(Insn &insn) : InsnImpl(insn) {}

    AddrMode srcMode() const { return _flags.srcMode(); }
    AddrMode dstMode() const { return _flags.dstMode(); }
    AddrMode ex1Mode() const { return _flags.ex1Mode(); }
    AddrMode ex2Mode() const { return _flags.ex2Mode(); }
    OprPos srcPos() const { return _flags.srcPos(); }
    OprPos dstPos() const { return _flags.dstPos(); }
    OprPos ex1Pos() const { return _flags.ex1Pos(); }
    OprPos ex2Pos() const { return _flags.ex2Pos(); }
    OprSize oprSize() const { return _flags.oprSize(); }

    void setFlags(Entry::Flags flags) {
        _flags = flags;
        _hasPost = false;
    }
    Entry::Flags flags() const { return _flags; }

    void setAddrMode(AddrMode src, AddrMode dst, AddrMode ex1, AddrMode ex2) {
        _flags = Entry::Flags::create(
                src, P_NONE, dst, P_NONE, ex1, P_NONE, ex2, P_NONE, SZ_NONE);
    }

    void setOpCode(Config::opcode_t opCode, Config::opcode_t prefix = 0) {
        _opCode = opCode;
        _prefix = prefix;
        _post = 0;
    }

    void embed(Config::opcode_t data) { _opCode |= data; }

    void embedPost(Config::opcode_t data) { _post |= data; }

    void setIndexByte(uint8_t data, OprPos pos) {
        if (pos == P_GEN1)
            _indexByte1 = data;
        if (pos == P_GEN2)
            _indexByte2 = data;
    }

    void readPost(DisMemory &memory) { _post = readByte(memory); }

    void setHasPost() { _hasPost = true; }

    Config::opcode_t opCode() const { return _opCode; }
    bool hasPrefix() const { return _prefix != 0; }
    Config::opcode_t prefix() const { return _prefix; }
    bool hasPost() const { return _hasPost; }
    uint8_t post() const { return _post; }
    uint8_t indexByte(OprPos pos) const {
        return pos == P_GEN1 ? _indexByte1 : _indexByte2;
    }

    void emitInsn() {
        uint8_t pos = 0;
        if (hasPrefix())
            emitByte(_prefix, pos++);
        emitByte(_opCode, pos++);
        if (hasPost())
            emitByte(_post, pos);
    }

    void emitOperand8(uint8_t val8) { emitByte(val8, operandPos()); }

    void emitOperand16(uint16_t val16) { emitUint16(val16, operandPos()); }

    void emitOperand32(uint32_t val32) { emitUint32(val32, operandPos()); }

    void emitOpFloat32(float float32) { emitFloat32(float32, operandPos()); }

    void emitOpFloat64(double float64) { emitFloat64(float64, operandPos()); }

private:
    Config::opcode_t _opCode;
    Config::opcode_t _prefix;
    Config::opcode_t _post;
    Entry::Flags _flags;
    bool _hasPost;
    uint8_t _indexByte1;
    uint8_t _indexByte2;

    uint8_t operandPos() {
        uint8_t pos = length();
        if (pos == 0) {
            if (hasPrefix())
                pos++;
            pos++;
            if (hasPost())
                pos++;
        }
        return pos;
    }
};

}  // namespace ns32000
}  // namespace libasm

#endif  // __INSN_NS32000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

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

namespace libasm {
namespace ns32000 {

class InsnNs32000 : public InsnImpl<Config, Entry> {
public:
    InsnNs32000(Insn &insn) : InsnImpl(insn) {}

    AddrMode srcMode() const { return flags().srcMode(); }
    AddrMode dstMode() const { return flags().dstMode(); }
    AddrMode ex1Mode() const { return flags().ex1Mode(); }
    AddrMode ex2Mode() const { return flags().ex2Mode(); }
    OprPos srcPos() const { return flags().srcPos(); }
    OprPos dstPos() const { return flags().dstPos(); }
    OprPos ex1Pos() const { return flags().ex1Pos(); }
    OprPos ex2Pos() const { return flags().ex2Pos(); }
    OprSize oprSize() const { return flags().oprSize(); }
    void setAddrMode(AddrMode src, AddrMode dst, AddrMode ex1, AddrMode ex2) {
        setFlags(Entry::Flags::create(src, P_NONE, dst, P_NONE, ex1, P_NONE, ex2, P_NONE, SZ_NONE));
    }

    void readPost(DisMemory &memory) { setPost(readByte(memory)); }

    void setIndexByte(uint8_t data, OprPos pos) {
        if (pos == P_GEN1)
            _indexByte1 = data;
        if (pos == P_GEN2)
            _indexByte2 = data;
    }
    uint8_t indexByte(OprPos pos) const { return pos == P_GEN1 ? _indexByte1 : _indexByte2; }

    void emitInsn() {
        uint8_t pos = 0;
        if (hasPrefix())
            emitByte(prefix(), pos++);
        emitByte(opCode(), pos++);
        if (hasPost())
            emitByte(post(), pos);
    }

    void emitOperand8(uint8_t val8) { emitByte(val8, operandPos()); }
    void emitOperand16(uint16_t val16) { emitUint16(val16, operandPos()); }
    void emitOperand32(uint32_t val32) { emitUint32(val32, operandPos()); }
    void emitOpFloat32(float float32) { emitFloat32(float32, operandPos()); }
    void emitOpFloat64(double float64) { emitFloat64(float64, operandPos()); }

private:
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

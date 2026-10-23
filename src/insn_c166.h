/*
 * Copyright 2026 Tadashi G. Takaoka
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

#ifndef __LIBASM_INSN_C166_H__
#define __LIBASM_INSN_C166_H__

#include "config_c166.h"
#include "entry_c166.h"
#include "insn_base.h"
#include "reg_c166.h"
#include "value.h"

namespace libasm {
namespace c166 {

struct EntryInsn : EntryInsnBase<Config, Entry> {
    AddrMode dst() const { return flags().dst(); }
    AddrMode src() const { return flags().src(); }
    CodeFormat cf() const { return flags().cf(); }
    bool hasWord2() const { return flags().word2(); }
};

struct Operand final : ErrorAt {
    AddrMode mode;
    RegName reg;
    CcName cc;
    Value val;
    Value bitpos;
    Operand() : mode(M_NONE), reg(REG_UNDEF), cc(CC_UNDEF), val(), bitpos() {}
};

struct AsmInsn final : AsmInsnImpl<Config>, EntryInsn {
    AsmInsn(Insn &insn) : AsmInsnImpl(insn), _post(0), _word2(0) {}

    Operand dstOp, srcOp, extOp;

    void setPost(uint8_t post) { _post = post; }
    void embedPost(uint8_t data) { _post |= data; }
    uint8_t post() const { return _post; }
    void embedByte3(uint8_t data) { _word2 |= data; }
    void embedByte4(uint8_t data) { _word2 |= static_cast<uint16_t>(data) << 8; }
    void setWord2(uint16_t data) { _word2 = data; }

    // The instruction's length before its operands are encoded.
    uint8_t insnLength() const { return hasWord2() ? 4 : 2; }

    void emitInsn() {
        emitByte(opCode(), 0);
        emitByte(_post, 1);
        if (hasWord2())
            emitUint16(_word2, 2);
    }

private:
    uint8_t _post;
    uint16_t _word2;
};

struct DisInsn final : DisInsnImpl<Config>, EntryInsn {
    DisInsn(Insn &insn, DisMemory &memory, const StrBuffer &out)
        : DisInsnImpl(insn, memory, out), _post(0), _byte3(0), _byte4(0) {}

    void readPost() { _post = readByte(); }
    void readWord2() {
        _byte3 = readByte();
        _byte4 = readByte();
    }
    uint8_t post() const { return _post; }
    uint8_t byte3() const { return _byte3; }
    uint8_t byte4() const { return _byte4; }
    uint16_t word2() const { return _byte3 | (static_cast<uint16_t>(_byte4) << 8); }

private:
    uint8_t _post;
    uint8_t _byte3;
    uint8_t _byte4;
};

}  // namespace c166
}  // namespace libasm

#endif  // __LIBASM_INSN_C166_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

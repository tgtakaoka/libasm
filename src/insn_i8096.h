/*
 * Copyright 2022 Tadashi G. Takaoka
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

#ifndef __LIBASM_INSN_I8096_H__
#define __LIBASM_INSN_I8096_H__

#include "config_i8096.h"
#include "entry_i8096.h"
#include "insn_base.h"
#include "value.h"

namespace libasm {
namespace i8096 {

struct EntryInsn : EntryInsnPrePostfix<Config, Entry> {
    AaMode aa() const { return AaMode(opCode() & 3); }
    AddrMode dst() const { return flags().dst(); }
    AddrMode src1() const { return flags().src1(); }
    AddrMode src2() const { return flags().src2(); }
    // Whether |src2| names a DSP selector instead of a third operand.
    bool hasPost() const { return src2() >= M_PMAC; }
    void embedAa(AaMode aa) { embed(uint8_t(aa)); }
    bool mulDivInsn() const;
    // Whether this is a word multiply, whose destination can be a MAC
    // selector; see Config::macSelector.
    bool mulWordInsn() const;
};

struct Operand final : ErrorAt {
    AddrMode mode;
    Value regno;
    Value val;
    StrScanner regAt;
    Operand() : mode(M_NONE), regno(), val() {}
};

struct AsmInsn final : AsmInsnImpl<Config>, EntryInsn {
    AsmInsn(Insn &insn) : AsmInsnImpl(insn), _signedPrefix(false) {}

    Operand dstOp, src1Op, src2Op;

    // Whether a signed multiply or divide must take the FE prefix; see
    // M_SLREG.
    void setSignedPrefix(bool enable) { _signedPrefix = enable; }
    bool signedPrefix() const { return _signedPrefix; }

    void emitInsn() {
        uint8_t pos = 0;
        if (hasPrefix())
            emitByte(prefix(), pos++);
        emitByte(opCode(), pos);
    }
    // The DSP selector trails the operands; RPT's is followed by the
    // RPT_CNT register address.
    void emitPost() {
        if (src2() == M_PMAC || src2() == M_PRPT)
            emitOperand8(postfix());
        if (src2() == M_PRPT)
            emitOperand8(RPT_CNT);
    }
    void emitOperand8(uint8_t val8) { emitByte(val8, operandPos()); }
    void emitOperand16(uint16_t val16) { emitUint16(val16, operandPos()); }
    void emitOperand24(uint32_t val24) {
        emitUint16(val24, operandPos());
        emitByte(val24 >> 16, operandPos());
    }

private:
    bool _signedPrefix;

    uint8_t operandPos() {
        uint8_t pos = length();
        if (pos == 0)
            pos = hasPrefix() ? 2 : 1;
        return pos;
    }
};

struct DisInsn final : DisInsnImpl<Config>, EntryInsn {
    DisInsn(Insn &insn, DisMemory &memory, const StrBuffer &out)
        : DisInsnImpl(insn, memory, out), _read(0), _taken(0) {}

    // The table search has to look at operand bytes: BR and EBR differ only
    // in bit 0 of the first one, and the DSP selector trails the whole
    // operand.  What the search reads ahead is cached and handed to the
    // operand reader in turn, so a candidate that does not match leaves the
    // reader where it was.
    uint8_t peekOperand(uint8_t pos) {
        while (_read <= pos && _read < MAX_AHEAD && isOK())
            _ahead[_read++] = readByte();
        return pos < _read ? _ahead[pos] : 0;
    }
    uint8_t nextByte() { return _taken < _read ? _ahead[_taken++] : readByte(); }
    uint16_t nextUint16() {
        const auto lsb = nextByte();
        return static_cast<uint16_t>(nextByte()) << 8 | lsb;
    }

private:
    // The furthest look-ahead is a 3-operand MAC indexed by a 16-bit
    // displacement: three operand bytes, then the register, then the selector.
    static constexpr uint8_t MAX_AHEAD = 5;
    uint8_t _ahead[MAX_AHEAD];
    uint8_t _read;
    uint8_t _taken;
};

}  // namespace i8096
}  // namespace libasm

#endif  // __LIBASM_INSN_I8096_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

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

#ifndef __LIBASM_INSN_Z80_H__
#define __LIBASM_INSN_Z80_H__

#include "config_z80.h"
#include "entry_z80.h"
#include "insn_base.h"
#include "reg_z80.h"
#include "value.h"

namespace libasm {
namespace z80 {

struct EntryInsn : EntryInsnPrefix<Config, Entry> {
    AddrMode dst() const { return flags().dst(); }
    AddrMode src() const { return flags().src(); }
    bool ixBit() const;
    bool imCapable() const { return flags().imCapable(); }
    bool lmCapable() const { return flags().lmCapable(); }
};

struct AsmInsn;
struct DisInsn;

/** Z380 Decoder Directive */
struct Ddir final {
    Ddir() : _prefix(0), _opc(0), _addr(0) {}
    void operator=(const AsmInsn &insn);
    void operator=(const DisInsn &insn);
    explicit operator bool() const { return _prefix != 0; }
    void clear() { _prefix = 0; }
    void setImmediate(DdName dd);
    bool setMode(DdName dd);

    bool noImmediate() const { return *this && (_opc & 3) == 0; }
    bool byteImmediate() const { return *this && ((wim() & 3) == 1 || wim() == 0x03); }
    bool wordImmediate() const { return *this && ((wim() & 3) == 2 || wim() == 0x23); }
    bool noMode() const { return *this && (_opc & 3) == 3; }
    bool wordMode() const { return _prefix == 0xDD && (_opc & 3) < 3; }
    bool lwordMode() const { return _prefix == 0xFD && (_opc & 3) < 3; }

    Config::opcode_t prefix() const { return _prefix; }
    Config::opcode_t opc() const { return _opc; }
    Config::uintptr_t addr() const { return _addr; }

private:
    Config::opcode_t _prefix;
    Config::opcode_t _opc;
    Config::uintptr_t _addr;

    Config::opcode_t wim() const { return (_prefix & 0x20) | (_opc & 3); }
};

struct Operand final : ErrorAt {
    AddrMode mode;
    RegName reg;
    RegName idx;
    Value val;
    Operand() : mode(M_NONE), reg(REG_UNDEF), idx(REG_UNDEF), val() {}
};

struct AsmInsn final : AsmInsnImpl<Config>, EntryInsn {
    AsmInsn(Insn &insn) : AsmInsnImpl(insn), _dd(0) {}

    Operand dstOp, srcOp;
    Ddir ddir;

    Error fixup(const Ddir &ddir);
    Error embedDd(DdName dd);
    void emitInsn();
    void emitOperand8(uint8_t val8) { emitByte(val8, operandPos()); }
    void emitOperand16(uint16_t val16) { emitUint16(val16, operandPos()); }
    void emitOperand24(uint32_t val24) {
        emitOperand16(val24);
        emitByte((val24 >> 16) & UINT8_MAX);
    }
    void emitOperand32(uint32_t val32) { emitUint32(val32, operandPos()); }
    uint_fast8_t operandPos() const;

private:
    Config::opcode_t _dd;
    void fixupDd();
};

struct DisInsn final : DisInsnImpl<Config>, EntryInsn {
    DisInsn(Insn &insn, DisMemory &memory, const StrBuffer &out) : DisInsnImpl(insn, memory, out) {}
    DisInsn(DisInsn &o) : DisInsnImpl(o) {}

    uint32_t readUint24() {
        const auto val16 = readUint16();
        return (static_cast<uint32_t>(readByte()) << 16) | val16;
    }

    int32_t readDisp8() {
        if (ddir.byteImmediate())
            return static_cast<int16_t>(readUint16());
        if (ddir.wordImmediate())
            return Config::signExtend(readUint24(), 24);
        return static_cast<int8_t>(readByte());
    }

    int32_t ixoff;  // index offset for ixBit() instruction
    Ddir ddir;      // Z380 Decoder Directive
};

}  // namespace z80
}  // namespace libasm

#endif  // __LIBASM_INSN_Z80_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

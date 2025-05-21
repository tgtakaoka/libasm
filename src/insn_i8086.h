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
    EntryInsn() : _segment(0) {}

    AddrMode dst() const { return flags().dst(); }
    AddrMode src() const { return flags().src(); }
    AddrMode ext() const { return flags().ext(); }
    OprPos dstPos() const { return flags().dstPos(); }
    OprPos srcPos() const { return flags().srcPos(); }
    OprPos extPos() const { return flags().extPos(); }
    OprSize size() const { return flags().size(); }
    bool stringInst() const { return flags().stringInst(); }
    bool leaInsn() const { return prefix() == 0 && opCode() == 0x8D; }

    void setSegment(Config::opcode_t segment) { _segment = segment; }
    Config::opcode_t segment() const { return _segment; }

    static constexpr Config::opcode_t DATA32 = 0x66;
    static constexpr Config::opcode_t ADDR32 = 0x67;
    static constexpr Config::opcode_t FWAIT = 0x9B;
    static bool escapeInsn(Config::opcode_t opc) { return opc >= 0xD8 && opc < 0xE0; }

protected:
    Config::opcode_t _segment;
};

struct Operand final : ErrorAt {
    AddrMode mode;
    PrefixName ptr;
    RegName seg;
    RegName reg;
    RegName index;
    bool hasDisp;
    Value val;
    Value segval;
    Operand()
        : mode(M_NONE),
          ptr(PRE_UNDEF),
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

    bool farOperand() const { return flags().dst() == M_SEG && flags().src() == M_OFF; }

    void prepairModReg();
    void embedModReg(Config::opcode_t data);
    Config::opcode_t modReg() const { return _modReg; }

    void emitInsn();
    Error emitOperand8(uint8_t val8) { return emitByte(val8, operandPos()); }
    Error emitOperand16(uint16_t val16) { return emitUint16(val16, operandPos()); }
#if !defined(LIBASM_ASM_NOFLOAT)
    Error emitFloat32(const float80_t &value) { return emitFloat32Le(value); }
    Error emitFloat64(const float80_t &value) { return emitFloat64Le(value); }
    Error emitPackedDecimal(int64_t val64);
    Error emitTemporaryReal(const float80_t &val80);
#endif

private:
    Config::opcode_t _modReg;
    bool _hasModReg;

    uint_fast8_t operandPos() const;
};

struct DisInsn final : DisInsnImpl<Config>, EntryInsn {
    DisInsn(Insn &insn, DisMemory &memory, const StrBuffer &out)
        : DisInsnImpl(insn, memory, out),
          _modReg(0),
          _hasData32(false),
          _hasAddr32(false),
          _hasUnusedData32(false),
          _hasUnusedAddr32(false) {}
    DisInsn(Insn &insn, DisInsn &o, const StrBuffer &out)
        : DisInsnImpl(insn, o, out),
          _modReg(0),
          _hasData32(o._hasData32),
          _hasAddr32(o._hasAddr32),
          _hasUnusedData32(o._hasUnusedData32),
          _hasUnusedAddr32(o._hasUnusedAddr32) {}

    void addSuffix(char suffix, StrBuffer &out);
    void addGnuPrefix(char prefix, StrBuffer &out);
    void addPrefix(const /*PROGMEM*/ char *prefix_P, StrBuffer &out);

    Config::opcode_t readSizePrefix(Config::opcode_t opc);
    void setModel(bool model32) { _model32 = model32; }
    bool data32() const { return _model32 ^ _hasData32; };
    bool addr32() const { return _model32 ^ _hasAddr32; }
    bool useData32() {
        _hasUnusedData32 = false;
        return data32();
    }

    bool useAddr32() {
        _hasUnusedAddr32 = false;
        return addr32();
    }
    bool hasUnusedData32() const { return _hasUnusedData32; }
    bool hasUnusedAddr32() const { return _hasUnusedAddr32; }
    void setUsage(const DisInsn &o) {
        _hasUnusedData32 = o._hasUnusedData32;
        _hasUnusedAddr32 = o._hasUnusedAddr32;
    }

    uint16_t farseg;
    bool farInsn() const;
    bool needData32() const;
    bool fpuInsn() const { return escapeInsn(prefix() & UINT8_MAX); }

    void setGnuAs(bool enable) { _gnuAs = enable; }
    bool gnuAs() const { return _gnuAs; }

    void readModReg();
    Config::opcode_t modReg() const { return _modReg; }

private:
    Config::opcode_t _modReg;
    bool _hasData32;
    bool _hasAddr32;
    mutable bool _hasUnusedData32;
    mutable bool _hasUnusedAddr32;
    bool _model32;
    bool _gnuAs;
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

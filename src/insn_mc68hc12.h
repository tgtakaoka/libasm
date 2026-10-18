/*
 * Copyright 2025 Tadashi G. Takaoka
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

#ifndef __LIBASM_INSN_MC68HC12_H__
#define __LIBASM_INSN_MC68HC12_H__

#include "config_mc68hc12.h"
#include "entry_mc68hc12.h"
#include "insn_base.h"
#include "reg_mc68hc12.h"
#include "value.h"

namespace libasm {
namespace mc68hc12 {

struct EntryInsn : EntryInsnPrefix<Config, Entry> {
    AddrMode mode1() const { return flags().mode1(); }
    AddrMode mode2() const { return flags().mode2(); }
    AddrMode mode3() const { return flags().mode3(); }

    /** MOVB/MOVW */
    bool isMove() const {
        if (indexed(mode1()))
            return indexed(mode2()) || mode2() == M_EXT;
        return indexed(mode2()) && (mode1() == M_IM8 || mode1() == M_IM16 || mode1() == M_EXT);
    }
    /** MOVB/MOVW whose destination index post byte precedes the source operand */
    bool moveDestFirst() const { return isMove() && !indexed(mode1()); }

    /** M68HC12 PC-relative base of MOVB/MOVW operand |pos|, relative to the instruction address */
    int_fast8_t hc12MovePcBase(uint_fast8_t pos) const {
        const auto len = 2 + operandSize(mode1()) + operandSize(mode2());
        // M68HC12 PC offsets for MOVE instructions
        if (mode2() == M_EXT)
            return len - 2;  // IDX => EXT
        if (indexed(mode1()))
            return pos == 1 ? len - 1 : len + 1;  // IDX => IDX
        return mode1() == M_IM8 ? len + 1 : len + 2;  // IMM/EXT => IDX
    }

    /**
     * Bytes after the index post byte of MOVB/MOVW operand |pos| which the PC-relative base
     * counts. A source never counts the destination's index extension.
     */
    uint_fast8_t moveTrailer(uint_fast8_t pos) const {
        if (pos == 2)
            return indexed(mode1()) ? 0 : operandSize(mode1());
        return mode2() == M_EXT ? 2 : 1;
    }

private:
    static bool indexed(AddrMode mode) { return mode == M_NIDX || mode == M_IDX; }
    static uint_fast8_t operandSize(AddrMode mode) {
        return (mode == M_IM16 || mode == M_EXT) ? 2 : 1;
    }
};

struct Operand final : ErrorAt {
    AddrMode mode;
    RegName index;
    RegName base;
    StrScanner baseAt;
    bool indir;
    int8_t size;
    int8_t update;
    uint8_t pos;
    Value val;
    Operand()
        : mode(M_NONE),
          index(REG_UNDEF),
          base(REG_UNDEF),
          indir(false),
          size(0),
          update(-1),
          pos(0),
          val() {}
};

struct AsmInsn final : AsmInsnImpl<Config>, EntryInsn {
    AsmInsn(Insn &insn)
        : AsmInsnImpl(insn), pagedModel(false), cpuLocation(insn.address()), locationPage(-1) {}

    Operand op1, op2, op3;
    bool pagedModel;
    /** The CPU address of the instruction, and its program page or -1 */
    uint16_t cpuLocation;
    int16_t locationPage;

    void emitInsn() {
        uint8_t pos = 0;
        if (hasPrefix())
            emitByte(prefix(), pos++);
        emitByte(opCode(), pos);
    }
    void emitOperand8(uint8_t val8) { emitByte(val8, operandPos()); }
    void emitOperand16(uint16_t val16) { emitUint16(val16, operandPos()); }

private:
    uint8_t operandPos() const {
        uint8_t pos = length();
        if (pos == 0)
            pos = hasPrefix() ? 2 : 1;
        return pos;
    }
};

struct DisInsn final : DisInsnImpl<Config>, EntryInsn {
    DisInsn(Insn &insn, DisMemory &memory, const StrBuffer &out) : DisInsnImpl(insn, memory, out) {}

    uint8_t pos;  // operand position
    Config::opcode_t post;
    int16_t postExt;
};

}  // namespace mc68hc12
}  // namespace libasm

#endif  // __LIBASM_INSN_MC68HC12_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

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

#ifndef __LIBASM_INSN_H16_H__
#define __LIBASM_INSN_H16_H__

#include "config_h16.h"
#include "entry_h16.h"
#include "insn_base.h"
#include "reg_h16.h"
#include "value.h"

namespace libasm {
namespace h16 {

struct EntryInsn : EntryInsnPrefix<Config, Entry> {
    AddrMode mode1() const { return flags().mode1(); }
    AddrMode mode2() const { return flags().mode2(); }
    AddrMode mode3() const { return flags().mode3(); }
    InsnSize insnSize() const { return flags().insnSize(); }
    InsnClass insnClass() const { return flags().insnClass(); }
    // Fixed operand size from the table (ISZ_FIXED ops); SZ_NONE otherwise.
    OprSize fixedSize() const { return flags().oprSize(); }

    // Set by encode/decode from the Sz field in the opcode byte.
    OprSize oprSize = SZ_NONE;
};

struct Operand final : ErrorAt {
    AddrMode mode;
    reg::RegName reg;       // base register (Rn) or for M_DREG the register itself
    reg::RegName indexReg;  // index register Xm (M_INDEX, M_PCIDX)
    reg::CrName creg;       // M_CR
    reg::CcName cc;         // M_CC
    uint8_t scale;          // 1, 2, 4, or 8 (M_SCALE, M_INDEX, M_PCIDX)
    uint8_t indexSize;      // L bit: 0=word, 1=long (M_INDEX, M_PCIDX)
    uint8_t dispSize;       // requested displacement size (8/16/32, 0=auto)
    uint8_t immSize;        // requested immediate size (8/16/32, 0=auto)
    uint8_t absSize;        // requested absolute size (8/16/32, 0=auto)
    Value val;              // displacement / immediate / absolute / ds1
    Value val2;             // ds2 for M_DIND
    uint8_t ds1Size;        // ds1 size for M_DIND (1 or 4 bytes)
    uint8_t ds2Size;        // ds2 size for M_DIND
    Operand()
        : mode(M_NONE),
          reg(reg::REG_UNDEF),
          indexReg(reg::REG_UNDEF),
          creg(reg::CR_UNDEF),
          cc(reg::CC_UNDEF),
          scale(1),
          indexSize(0),
          dispSize(0),
          immSize(0),
          absSize(0),
          val(),
          val2(),
          ds1Size(0),
          ds2Size(0) {}
};

struct AsmInsn final : AsmInsnImpl<Config>, EntryInsn {
    AsmInsn(Insn &insn) : AsmInsnImpl(insn) {}

    // At most three operands; a register run folds into one M_RLIST slot.
    Operand op1, op2, op3;

    // Every register of a run (STM/LDM/PGBN list or BFMOV/string/bit-field
    // tuple), ranges expanded; the single source of truth regardless of how it
    // folds into op1..op3 (see parseRegList).
    uint8_t regList[16];    // register numbers (0..15) in parse order
    uint8_t regCount = 0;
    bool regRange = false;  // a "Rn-Rm" range appeared -> always M_RLIST
    bool regGlobal = true;  // every run register is a global Rn (lists forbid CR/PR)

    void addReg(reg::RegName r) {
        if (regCount < 16)
            regList[regCount++] = reg::regNum(r);
    }
    void addRegNum(uint8_t n) {
        if (regCount < 16)
            regList[regCount++] = n;
    }
    // 16-bit list mask (bit n = Rn present), for STM/LDM/PGBN.
    uint16_t regMask() const {
        uint16_t mask = 0;
        for (uint8_t i = 0; i < regCount; ++i)
            mask |= uint16_t(1u << regList[i]);
        return mask;
    }

    using AsmInsnImpl::emitByte;

    // Emit the prefix byte (if any) and the opcode byte at the start of the
    // instruction.  Call after encodeOperand has filled the trailing bytes.
    void emitInsn() {
        uint8_t pos = 0;
        if (hasPrefix())
            emitByte(prefix(), pos++);
        emitByte(opCode(), pos);
    }

    // Position past the opcode/prefix; operand bytes append at length() once
    // any have been emitted.
    uint8_t operandPos() const {
        uint8_t pos = length();
        if (pos == 0)
            pos = hasPrefix() ? 2 : 1;
        return pos;
    }

    void emitOperand8(uint8_t v)  { emitByte(v, operandPos()); }
    void emitOperand16(uint16_t v) { emitUint16(v, operandPos()); }
    void emitOperand32(uint32_t v) { emitUint32(v, operandPos()); }

    // Split the mnemonic token STEM[/cc][:CLASS][.SIZE]: record the "/cc" in
    // ccSuffix, ":CLASS" in classSuffix, ".SIZE" in sizeSuffix, leaving
    // name() = stem.
    void parseClassAndSize(const ValueParser &parser);

    OprSize sizeSuffix = SZ_NONE;
    InsnClass classSuffix = IC_N;
    reg::CcName ccSuffix = reg::CC_UNDEF;  // /cc condition (SCB/SET/TRAP/SCMP/SSCH)
    StrDir dirSuffix = DIR_NONE;           // /F or /B direction (string ops)
    bool ccFromLoneF = false;              // ccSuffix is a tentative lone "/F"
};

struct DisInsn final : DisInsnImpl<Config>, EntryInsn {
    DisInsn(Insn &insn, DisMemory &memory, const StrBuffer &out)
        : DisInsnImpl(insn, memory, out) {}

    // accumulatorForm: an EA byte had bit 7 set (dst is implicitly R0, no EAd
    // byte).  accConsumed: a dst EA honored it; set-but-not-consumed is illegal.
    bool accumulatorForm = false;
    bool accConsumed = false;

    // CR register pre-read before operand dispatch (byte order [opcode][CR][EA],
    // mnemonic operand order varies) so M_CR just outputs it.
    reg::CrName crReg = reg::CR_UNDEF;
    reg::CcName ccReg = reg::CC_UNDEF;       // /cc condition (SCB/SET/TRAP)
    reg::RegName scbReg = reg::REG_UNDEF;    // SCB's Rn (pre-read with cc)
};

}  // namespace h16
}  // namespace libasm

#endif  // __LIBASM_INSN_H16_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

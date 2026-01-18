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

#ifndef __LIBASM_INSN_TMS320F_H__
#define __LIBASM_INSN_TMS320F_H__

#include "config_tms320f.h"
#include "entry_tms320f.h"
#include "insn_base.h"
#include "reg_tms320f.h"
#include "str_scanner.h"
#include "value.h"

namespace libasm {
namespace tms320f {

struct EntryInsn : EntryInsnBase<Config, Entry> {
    AddrMode mode1() const { return flags().mode1(); }
    AddrMode mode2() const { return flags().mode2(); }
    AddrMode mode3() const { return flags().mode3(); }
    OprPos pos1() const { return flags().pos1(); }
    OprPos pos2() const { return flags().pos2(); }
    OprPos pos3() const { return flags().pos3(); }
    CodeFormat codeFormat() const { return flags().codeFormat(); }
    LswFormat lswFormat() const { return flags().lswFormat(); }
    bool maybeUnary() const { return flags().maybeUnary(); }
    bool isParallel() const { return flags().isParallel(); }
};

enum SubMode : uint8_t {
    SUB_NONE = 0,
    SUB_DISP = 1,
    SUB_PRE = 2,
    SUB_POST = 3,
    SUB_BASE_MASK = 0x3,
    SUB_CIRC = 0x10,
    SUB_BITR = 0x20,
};

struct Operand final : ErrorAt {
    uint_fast8_t pos;
    AddrMode mode;
    RegName reg;
    SubMode subMode;
    bool indexSign;
    RegName index;
    bool hasDisp;
    StrScanner dispAt;
    Value val;
    Operand(uint_fast8_t _pos)
        : pos(_pos),
          mode(M_NONE),
          reg(REG_UNDEF),
          subMode(SUB_NONE),
          indexSign(false),
          index(REG_UNDEF),
          hasDisp(false),
          dispAt(),
          val() {}
    void copyFrom(const Operand &from);
};

struct AsmInsn final : AsmInsnImpl<Config>, EntryInsn {
    AsmInsn(Insn &insn) : AsmInsnImpl(insn) {}

    Operand op1{1}, op2{2}, op3{3};
    AsmInsn *para;

    void emitInsn() { emitUint32(opCode(), 0); }
    bool hasContinue() const { return _insn.hasContinue(); }
    void clearContinueMark() const { _insn.setContinueMark_P(nullptr); }
    void copyFrom(const AsmInsn &from);
};

struct DisInsn final : DisInsnImpl<Config>, EntryInsn {
    DisInsn(Insn &insn, DisMemory &memory, const StrBuffer &out) : DisInsnImpl(insn, memory, out) {}
    void bytesToOpCode();
    bool hasContinue() const { return _insn.hasContinue(); }
    void setContinueMark_P(const /*PROGMEM*/ char *mark_P) const {
        _insn.setContinueMark_P(mark_P);
    }

    RegName srcReg = REG_UNDEF;
    RegName src2AReg = REG_UNDEF;
};

}  // namespace tms320f
}  // namespace libasm

#endif  // __LIBASM_INSN_TMS320F_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

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

#ifndef __LIBASM_INSN_TLCS900_H__
#define __LIBASM_INSN_TLCS900_H__

#include "config_tlcs900.h"
#include "entry_tlcs900.h"
#include "insn_base.h"
#include "reg_tlcs900.h"
#include "value.h"

namespace libasm {
namespace tlcs900 {

// Prefix context modes: determines which instruction table to use
// and how to interpret the prefix byte operand.
enum PrefixMode : uint8_t {
    PM_NONE = 0,   // no prefix: standalone instruction
    PM_REG8 = 1,   // prefix 0xC8-0xCF: 8-bit current-bank register context
    PM_REG16 = 2,  // prefix 0xD8-0xDF: 16-bit current-bank register context
    PM_REG32 = 3,  // prefix 0xE8-0xEF: 32-bit current-bank register context
    PM_MEM8 = 4,   // memory byte-source context (84-87, 8C-8F, C0-C5 + sub)
    PM_MEM16 = 5,  // memory word-source context (94-97, 9C-9F, D0-D5 + sub)
    PM_MEM32 = 6,  // memory lword-source context (A4-A7, AC-AF, E0-E5 + sub)
    PM_MEMD = 7,   // memory destination context (B4-B7, BC-BF, F0-F5 + sub)
    PM_BLOCK = 8,   // 0x83 prefix: block transfer/compare instructions
    PM_RETCC = 9,   // 0xB0 prefix: conditional return
    PM_ABREG8 = 10,   // prefix 0xC7: 8-bit absolute bank register context
    PM_ABREG16 = 11,  // prefix 0xD7: 16-bit absolute bank register context
    PM_ABREG32 = 12,  // prefix 0xE7: 32-bit absolute bank register context
};

struct EntryInsn : EntryInsnPrefix<Config, Entry> {
    AddrMode dst() const { return flags().dst(); }
    AddrMode src() const { return flags().src(); }
    void setAddrMode(AddrMode dst, AddrMode src) {
        setFlags(Entry::Flags::create(flags().format(), dst, src));
    }
    PrefixMode pre() const { return _prefixMode; }
    void setPrefixMode(PrefixMode mode) { _prefixMode = mode; }

private:
    PrefixMode _prefixMode = PM_NONE;
};

// Decoded operand
struct Operand final : ErrorAt {
    AddrMode mode;  // addressing mode
    RegName reg;    // base register (for indirect, indexed, or src/dst reg)
    RegName idx;    // index register (for M_IDXR only)
    CcName cc;      // condition code (for M_CC)
    Value val;      // immediate, displacement, or address value
    uint8_t size;   // operand size from prefix: 0=byte, 1=word, 2=lword
    Operand()
        : ErrorAt(), mode(M_NONE), reg(REG_UNDEF), idx(REG_UNDEF), cc(CC_UNDEF), val(), size(0) {}
};

struct AsmInsn final : AsmInsnImpl<Config>, EntryInsn {
    AsmInsn(Insn &insn) : AsmInsnImpl(insn) {}

    Operand dstOp, srcOp;

    void setEmitInsn() { _emitInsn = true; }
    void emitInsn(Config::opcode_t opc) {
        if (_emitInsn) {
            emitByte(opc);
            _emitInsn = false;
        }
    }

    // Per-page PrefixMode bitmask, set by searchName's pageSetup callback so
    // that acceptOperands can iterate the page's valid PMs.
    uint16_t pmMask() const { return _pmMask; }
    void setPmMask(uint16_t mask) { _pmMask = mask; }

private:
    bool _emitInsn = false;
    uint16_t _pmMask = 0;
};

struct DisInsn final : DisInsnImpl<Config>, EntryInsn {
    DisInsn(Insn &insn, DisMemory &memory, const StrBuffer &out) : DisInsnImpl(insn, memory, out) {}

    // Per-page PrefixMode bitmask, set by searchOpCode's pageMatcher callback
    // so downstream decode code can consult the page's accepted PMs.
    uint16_t pmMask() const { return _pmMask; }
    void setPmMask(uint16_t mask) { _pmMask = mask; }

private:
    uint16_t _pmMask = 0;
};

}  // namespace tlcs900
}  // namespace libasm

#endif  // __LIBASM_INSN_TLCS900_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

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
    PM_NONE = 0,      // no prefix: standalone instruction
    PM_REG8 = 1,      // prefix 0xC8-0xCF: 8-bit current-bank register context
    PM_REG16 = 2,     // prefix 0xD8-0xDF: 16-bit current-bank register context
    PM_REG32 = 3,     // prefix 0xE8-0xEF: 32-bit current-bank register context
    PM_MEM8 = 4,      // memory byte-source context (84-87, 8C-8F, C0-C5 + sub)
    PM_MEM16 = 5,     // memory word-source context (94-97, 9C-9F, D0-D5 + sub)
    PM_MEM32 = 6,     // memory lword-source context (A4-A7, AC-AF, E0-E5 + sub)
    PM_MEMD = 7,      // memory destination context (B4-B7, BC-BF, F0-F5 + sub)
    PM_BLOCK = 8,     // 0x83 prefix: block transfer/compare instructions
    PM_RETCC = 9,     // 0xB0 prefix: conditional return
    PM_ABREG8 = 10,   // prefix 0xC7: 8-bit absolute bank register context
    PM_ABREG16 = 11,  // prefix 0xD7: 16-bit absolute bank register context
    PM_ABREG32 = 12,  // prefix 0xE7: 32-bit absolute bank register context
    PM_LDAR = 13,     // 2-byte prefix 0xF313: LDAR, rel16 src sits between prefix and opcode
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

    // Stashed by AsmTlcs900 before encoding so prefixSize() can decide
    // between the compact (1-byte) and complex (2-byte) M_PRDC/M_PINC
    // prefix forms without having to consult the assembler instance.
    bool complexIndir = false;

    // Opcode position computed from the resolved AddrMode / PrefixMode /
    // prefix() state -- independent of the current emit length.
    uint_fast8_t prefixSize() const;
    // First byte past the opcode slot, growing past prior operand trailings.
    uint_fast8_t operandPos() const;
    // Write the final opcode at prefixSize(). Skipped when encodeOperand
    // reset bytes for a structural error so failures leave no encoded output.
    void emitInsn();
    void emitOperand8(uint8_t val) { emitByte(val, operandPos()); }
    void emitOperand16(uint16_t val) { emitUint16(val, operandPos()); }
    void emitOperand24(uint32_t val) {
        const auto pos = operandPos();
        emitUint16(static_cast<uint16_t>(val), pos);
        emitByte(static_cast<uint8_t>(val >> 16), pos + 2);
    }
    void emitOperand32(uint32_t val) { emitUint32(val, operandPos()); }

    // Per-page PrefixMode bitmask, set by searchName's pageSetup callback so
    // that acceptOperands can iterate the page's valid PMs.
    uint16_t pmMask() const { return _pmMask; }
    void setPmMask(uint16_t mask) { _pmMask = mask; }

private:
    uint16_t _pmMask = 0;
};

struct DisInsn final : DisInsnImpl<Config>, EntryInsn {
    DisInsn(Insn &insn, DisMemory &memory, const StrBuffer &out) : DisInsnImpl(insn, memory, out) {}

    // Sub-byte read after the prefix byte, kept on the insn so the addressing
    // helpers can look at it without re-reading. Set by decodeImpl when the
    // sub-byte must be inspected up front (e.g. 0xF3 routing between LDAR's
    // 2-byte prefix and PM_MEMD's complex addressing).
    uint8_t sub = 0;

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

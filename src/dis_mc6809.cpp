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

#include "dis_mc6809.h"

#include "reg_mc6809.h"
#include "table_mc6809.h"

namespace libasm {
namespace mc6809 {

using namespace reg;

const ValueFormatter::Plugins &DisMc6809::defaultPlugins() {
    return ValueFormatter::Plugins::motorola();
}

DisMc6809::DisMc6809(const ValueFormatter::Plugins &plugins)
    : Disassembler(plugins), Config(TABLE) {
    reset();
}

void DisMc6809::decodeDirectPage(DisInsn &insn, StrBuffer &out) const {
    const uint8_t dir = insn.readByte();
    const auto label = lookup(dir);
    if (label) {
        out.letter('<').rtext(label);
    } else {
        outAbsAddr(out, dir, 8);
    }
}

void DisMc6809::decodeExtended(DisInsn &insn, StrBuffer &out) const {
    const Config::uintptr_t addr = insn.readUint16();
    const auto label = lookup(addr);
    if (label) {
        out.letter('>').rtext(label);
    } else {
        if (addr < 0x100)
            out.letter('>');
        outAbsAddr(out, addr);
    }
}

void DisMc6809::decodeIndexed(DisInsn &insn, StrBuffer &out) const {
    const auto post = insn.readPost();
    PostSpec spec;
    if (TABLE.searchPostByte(cpuType(), post, spec)) {
        insn.setErrorIf(out, UNKNOWN_POSTBYTE);
        return;
    }
    if (spec.indir)
        out.letter('[');
    if (spec.base != REG_UNDEF && spec.size > 2) {  // n,X
        Config::ptrdiff_t offset = 0;
        char prefix = 0;
        if (spec.size == 5) {
            // Sign extends 5-bit number as 0x10 is a sign bit.
            offset = signExtend(post, 5);  //
            if (offset == 0)
                prefix = '{';
        } else if (spec.size == 8) {
            offset = static_cast<int8_t>(insn.readByte());
            if (spec.indir && offset == 0)
                prefix = '<';
            // Check offset is in 5-bit integer range.
            if (!spec.indir && !overflowInt(offset, 5))
                prefix = '<';
        } else {
            offset = static_cast<int16_t>(insn.readUint16());
            // Check offset is in 8-bit integer range.
            if (!overflowInt8(offset))
                prefix = '>';
        }
        if (spec.base == REG_PCR) {
            const auto base = insn.address() + insn.length();
            Error error;
            const auto target = branchTarget(base, offset, error);
            if (error)
                insn.setErrorIf(out, error);
            if (spec.size == 16 && !overflowInt8(offset))
                out.letter('>');
            outRelAddr(out, target, insn.address(), spec.size);
        } else {
            if (prefix) {
                if (prefix == '{')
                    out.letter(prefix = '<');
                out.letter(prefix);
            }
            if (spec.size == 16) {
                outHex(out, offset, -16);
            } else {
                outDec(out, offset, -spec.size);
            }
        }
    }
    if (spec.index != REG_UNDEF)  // A,X
        outRegName(out, spec.index);
    if (spec.base == REG_UNDEF) {  // [nnnn]
        outAbsAddr(out, insn.readUint16());
    } else if (insn.isOK() || out.len()) {
        out.letter(',');
    }
    if (spec.size < 0) {  // ,-X ,--X
        out.letter('-');
        if (spec.size == -2)
            out.letter('-');
    }
    if (spec.base == REG_X) {
        const auto base = decodeBaseReg(post >> 5);
        outRegName(out, base);
    } else if (spec.base != REG_UNDEF) {
        outRegName(out, spec.base);
    }
    if (spec.size == 1 || spec.size == 2) {  // ,X+ ,X++
        out.letter('+');
        if (spec.size == 2)
            out.letter('+');
    }
    if (spec.indir)
        out.letter(']');
}

void DisMc6809::decodeRelative(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    const auto delta = (mode == M_REL) ? static_cast<int8_t>(insn.readByte())
                                       : static_cast<int16_t>(insn.readUint16());
    const auto base = insn.address() + insn.length();
    Error error;
    const auto target = branchTarget(base, delta, error);
    if (error)
        insn.setErrorIf(out, error);
    outRelAddr(out, target, insn.address(), mode == M_REL ? 8 : 16);
}

void DisMc6809::decodeImmediate(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    out.letter('#');
    uint32_t val;
    uint8_t bits;
    if (mode == M_IM8 || mode == M_GEN8) {
        val = insn.readByte();
        bits = 8;
    } else if (mode == M_GEN16) {
        val = insn.readUint16();
        bits = 16;
    } else {
        val = insn.readUint32();
        bits = 32;
    }
    outHex(out, val, bits);
}

void DisMc6809::decodePushPull(DisInsn &insn, StrBuffer &out) const {
    auto post = insn.readPost();
    const bool hasDreg = (post & 0x06) == 0x06;
    if (hasDreg)
        post &= ~0x02;  // clear REG_A
    const bool userStack = (insn.opCode() & 2) != 0;
    const bool push = (insn.opCode() & 1) == 0;
    for (uint8_t i = 0, n = 0; i < 8; i++) {
        const uint8_t bitPos = push ? 7 - i : i;
        if (post & (1U << bitPos)) {
            if (n != 0)
                out.letter(',');
            auto reg = decodeStackReg(bitPos, userStack);
            if (reg == REG_B && hasDreg)
                reg = REG_D;
            outRegName(out, reg);
            n++;
        }
    }
}

void DisMc6809::decodeRegisters(DisInsn &insn, StrBuffer &out) const {
    const auto post = insn.readPost();
    const auto cpu = cpuType();
    const auto dst = decodeDataReg(cpu, post);
    const auto src = decodeDataReg(cpu, post >> 4);
    const auto size2 = regSize(dst);
    const auto size1 = regSize(src);
    if (src == REG_UNDEF)
        insn.setErrorIf(out, ILLEGAL_REGISTER);
    outRegName(out, src, true).comma();
    if (dst == REG_UNDEF)
        insn.setErrorIf(out, ILLEGAL_REGISTER);
    if (size1 != SZ_NONE && size2 != SZ_NONE && size1 != size2)
        insn.setErrorIf(out, ILLEGAL_SIZE);
    outRegName(out, dst, true);
}

void DisMc6809::decodeRegBit(DisInsn &insn, StrBuffer &out) const {
    const auto post = insn.readPost();
    const auto reg = decodeBitOpReg(post >> 6);
    if (reg == REG_UNDEF)
        insn.setErrorIf(out, ILLEGAL_REGISTER);
    outRegName(out, reg).letter('.');
    outHex(out, post & 7, 3);
}

void DisMc6809::decodeDirBit(DisInsn &insn, StrBuffer &out) const {
    decodeDirectPage(insn, out);
    out.letter('.');
    outHex(out, (insn.post() >> 3) & 7, 3);
}

void DisMc6809::decodeTransferMemory(DisInsn &insn, StrBuffer &out) const {
    const auto post = insn.readPost();
    const auto cpu = cpuType();
    const auto dst = decodeDataReg(cpu, post);
    const auto src = decodeDataReg(cpu, post >> 4);
    const auto mode = insn.opCode() & 0x3;
    if (!isTfmBaseReg(src))
        insn.setErrorIf(out, ILLEGAL_REGISTER);
    outRegName(out, src);
    const auto srcModeChar = tfmSrcModeChar(mode);
    if (srcModeChar)
        out.letter(srcModeChar);
    out.comma();
    if (!isTfmBaseReg(dst))
        insn.setErrorIf(out, ILLEGAL_REGISTER);
    outRegName(out, dst);
    const auto dstModeChar = tfmDstModeChar(mode);
    if (dstModeChar)
        out.letter(dstModeChar);
}

void DisMc6809::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    switch (mode) {
    case M_DIR:
        decodeDirectPage(insn, out);
        return;
    case M_EXT:
        decodeExtended(insn, out);
        return;
    case M_IDX:
        decodeIndexed(insn, out);
        return;
    case M_REL:
    case M_LREL:
        decodeRelative(insn, out, mode);
        return;
    case M_GMEM:
        switch (insn.opCode() & 0xF0) {
        case 0x60:
            decodeIndexed(insn, out);
            return;
        case 0x70:
            decodeExtended(insn, out);
            return;
        default:
            decodeDirectPage(insn, out);
            return;
        }
    case M_GEN8:
    case M_GEN16:
        switch (insn.opCode() & 0x30) {
        case 0x10:
            decodeDirectPage(insn, out);
            return;
        case 0x20:
            decodeIndexed(insn, out);
            return;
        case 0x30:
            decodeExtended(insn, out);
            return;
        default:
            break;
        }
        // Fall-through
    case M_IM8:
    case M_IM32:
        decodeImmediate(insn, out, mode);
        return;
    case M_LIST:
        decodePushPull(insn, out);
        return;
    case M_PAIR:
        decodeRegisters(insn, out);
        return;
    case M_RBIT:
        decodeRegBit(insn, out);
        return;
    case M_DBIT:
        decodeDirBit(insn, out);
        return;
    case M_RTFM:
        decodeTransferMemory(insn, out);
        return;
    default:
        break;
    }
}

Error DisMc6809::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) {
    DisInsn insn(_insn, memory, out);
    const auto opc = insn.readByte();
    insn.setOpCode(opc);
    if (TABLE.isPrefix(cpuType(), opc)) {
        insn.setOpCode(insn.readByte(), opc);
        if (insn.getError())
            return setError(insn);
    }
    if (TABLE.searchOpCode(cpuType(), insn, out))
        return setError(insn);

    decodeOperand(insn, out, insn.mode1());
    const auto mode2 = insn.mode2();
    if (mode2 != M_NONE && mode2 != M_RTFM)
        decodeOperand(insn, out.comma(), mode2);
    return setError(insn);
}

}  // namespace mc6809
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

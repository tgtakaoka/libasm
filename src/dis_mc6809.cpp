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

Error DisMc6809::decodeDirectPage(DisInsn &insn, StrBuffer &out) {
    const uint8_t dir = insn.readByte();
    const auto label = lookup(dir);
    if (label) {
        out.letter('<').rtext(label);
    } else {
        outAbsAddr(out, dir, 8);
    }
    return setError(insn);
}

Error DisMc6809::decodeExtended(DisInsn &insn, StrBuffer &out) {
    const Config::uintptr_t addr = insn.readUint16();
    const auto label = lookup(addr);
    if (label) {
        out.letter('>').rtext(label);
    } else {
        if (addr < 0x100)
            out.letter('>');
        outAbsAddr(out, addr);
    }
    return setError(insn);
}

Error DisMc6809::decodeIndexed(DisInsn &insn, StrBuffer &out) {
    const uint8_t post = insn.readPost();
    PostSpec spec;
    if (TABLE.searchPostByte(cpuType(), post, spec))
        return setError(UNKNOWN_POSTBYTE);
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
            const auto target = branchTarget(base, offset);
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
    } else {
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
    return setErrorIf(insn);
}

Error DisMc6809::decodeRelative(
        DisInsn &insn, StrBuffer &out, AddrMode mode) {
    Config::ptrdiff_t delta;
    if (mode == M_REL) {
        delta = static_cast<int8_t>(insn.readByte());
    } else {
        delta = static_cast<Config::ptrdiff_t>(insn.readUint16());
    }
    const auto base = insn.address() + insn.length();
    const auto target = branchTarget(base, delta);
    outRelAddr(out, target, insn.address(), mode == M_REL ? 8 : 16);
    return setErrorIf(insn);
}

Error DisMc6809::decodeImmediate(
        DisInsn &insn, StrBuffer &out, AddrMode mode) {
    out.letter('#');
    if (mode == M_IM8 || mode == M_GEN8) {
        outHex(out, insn.readByte(), 8);
    } else if (mode == M_GEN16) {
        outHex(out, insn.readUint16(), 16);
    } else {
        outHex(out, insn.readUint32(), 32);
    }
    return setError(insn);
}

Error DisMc6809::decodePushPull(DisInsn &insn, StrBuffer &out) {
    uint8_t post = insn.readPost();
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
    return setError(insn);
}

Error DisMc6809::decodeRegisters(DisInsn &insn, StrBuffer &out) {
    const uint8_t post = insn.readPost();
    const auto cpu = cpuType();
    const auto dst = decodeDataReg(cpu, post);
    const auto src = decodeDataReg(cpu, post >> 4);
    if (src == REG_UNDEF || dst == REG_UNDEF)
        return setError(ILLEGAL_REGISTER);
    const auto size2 = regSize(dst);
    const auto size1 = regSize(src);
    if (size1 != SZ_NONE && size2 != SZ_NONE && size1 != size2)
        return setError(ILLEGAL_SIZE);
    outRegName(out, src).comma();
    outRegName(out, dst);
    return setError(insn);
}

Error DisMc6809::decodeRegBit(DisInsn &insn, StrBuffer &out) {
    const uint8_t post = insn.readPost();
    const auto reg = decodeBitOpReg(post >> 6);
    if (reg == REG_UNDEF)
        return setError(ILLEGAL_REGISTER);
    outRegName(out, reg).letter('.');
    outHex(out, post & 7, 3);
    return OK;
}

Error DisMc6809::decodeDirBit(DisInsn &insn, StrBuffer &out) {
    if (decodeDirectPage(insn, out))
        return getError();
    out.letter('.');
    outHex(out, (insn.readPost() >> 3) & 7, 3);
    return OK;
}

Error DisMc6809::decodeTransferMemory(DisInsn &insn, StrBuffer &out) {
    const uint8_t post = insn.readPost();
    const auto src = decodeTfmBaseReg(post >> 4);
    const auto dst = decodeTfmBaseReg(post);
    const uint8_t mode = insn.opCode() & 0x3;
    if (src == REG_UNDEF || dst == REG_UNDEF)
        return setError(ILLEGAL_REGISTER);
    outRegName(out, src);
    const char srcModeChar = tfmSrcModeChar(mode);
    if (srcModeChar)
        out.letter(srcModeChar);
    out.comma();
    outRegName(out, dst);
    const char dstModeChar = tfmDstModeChar(mode);
    if (dstModeChar)
        out.letter(dstModeChar);
    return setError(insn);
}

Error DisMc6809::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode) {
    switch (mode) {
    case M_DIR:
        return decodeDirectPage(insn, out);
    case M_EXT:
        return decodeExtended(insn, out);
    case M_IDX:
        return decodeIndexed(insn, out);
    case M_REL:
    case M_LREL:
        return decodeRelative(insn, out, mode);
    case M_GMEM:
        switch (insn.opCode() & 0xF0) {
        case 0x60:
            return decodeIndexed(insn, out);
        case 0x70:
            return decodeExtended(insn, out);
        default:
            return decodeDirectPage(insn, out);
        }
    case M_GEN8:
    case M_GEN16:
        switch (insn.opCode() & 0x30) {
        case 0x10:
            return decodeDirectPage(insn, out);
        case 0x20:
            return decodeIndexed(insn, out);
        case 0x30:
            return decodeExtended(insn, out);
        default:
            break;
        }
        // Fall-through
    case M_IM8:
    case M_IM32:
        return decodeImmediate(insn, out, mode);
    case M_LIST:
        return decodePushPull(insn, out);
    case M_PAIR:
        return decodeRegisters(insn, out);
    case M_RBIT:
        return decodeRegBit(insn, out);
    case M_DBIT:
        return decodeDirBit(insn, out);
    case M_RTFM:
        return decodeTransferMemory(insn, out);
    default:
        return OK;
    }
}

Error DisMc6809::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) {
    DisInsn insn(_insn, memory);
    Config::opcode_t opCode = insn.readByte();
    insn.setOpCode(opCode);
    if (TABLE.isPrefix(cpuType(), opCode)) {
        const Config::opcode_t prefix = opCode;
        opCode = insn.readByte();
        insn.setOpCode(opCode, prefix);
    }
    if (setError(insn))
        return getError();

    if (TABLE.searchOpCode(cpuType(), insn, out))
        return setError(insn);

    if (decodeOperand(insn, out, insn.mode1()))
        return getError();
    const auto mode2 = insn.mode2();
    if (mode2 == M_NONE)
        return OK;
    if (mode2 == M_RTFM)
        return OK;
    out.comma();
    return decodeOperand(insn, out, mode2);
}

}  // namespace mc6809
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

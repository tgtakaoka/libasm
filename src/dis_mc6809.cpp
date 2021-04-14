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

#include "table_mc6809.h"

namespace libasm {
namespace mc6809 {

char *DisMc6809::outRegister(char *out, RegName regName) {
    return _regs.outRegName(out, regName);
}

Error DisMc6809::decodeDirectPage(DisMemory &memory, InsnMc6809 &insn, char *out) {
    const uint8_t dir = insn.readByte(memory);
    outAbsAddr(out, dir, 8, PSTR("<"));
    return setError(insn);
}

Error DisMc6809::decodeExtended(DisMemory &memory, InsnMc6809 &insn, char *out) {
    const Config::uintptr_t addr = insn.readUint16(memory);
    outAbsAddr(out, addr, 16, PSTR(">"), addr < 0x100);
    return setError(insn);
}

Error DisMc6809::decodeIndexed(DisMemory &memory, InsnMc6809 &insn, char *out) {
    const uint8_t post = insn.readPost(memory);
    PostSpec spec;
    if (TableMc6809.searchPostByte(post, spec))
        return setError(UNKNOWN_POSTBYTE);
    if (spec.indir)
        *out++ = '[';
    if (spec.base != REG_UNDEF && spec.size > 2) {  // n,X
        Config::ptrdiff_t offset = 0;
        char prefix = 0;
        if (spec.size == 5) {
            offset = post & 0x1F;
            if (post & 0x10)
                offset |= 0xFFE0;
            if (offset == 0)
                prefix = '{';
        } else if (spec.size == 8) {
            offset = static_cast<int8_t>(insn.readByte(memory));
            if (spec.indir && offset == 0)
                prefix = '<';
            if (!spec.indir && offset >= -16 && offset < 16)
                prefix = '<';
        } else {
            offset = static_cast<int16_t>(insn.readUint16(memory));
            if (offset >= -128 && offset < 128)
                prefix = '>';
        }
        if (spec.base == REG_PCR) {
            const Config::uintptr_t target = insn.address() + insn.length() + offset;
            out = outRelAddr(out, target, insn.address(), spec.size);
        } else {
            if (prefix) {
                if (prefix == '{')
                    *out++ = (prefix = '<');
                *out++ = prefix;
            }
            if (spec.size == 16) {
                out = outHex(out, offset, -16);
            } else {
                out = outDec(out, offset, -spec.size);
            }
        }
    }
    if (spec.index != REG_UNDEF)  // A,X
        out = outRegister(out, spec.index);
    if (spec.base == REG_UNDEF) {  // [nnnn]
        out = outAbsAddr(out, insn.readUint16(memory));
    } else {
        *out++ = ',';
    }
    if (spec.size < 0) {  // ,-X ,--X
        *out++ = '-';
        if (spec.size == -2)
            *out++ = '-';
    }
    if (spec.base == REG_X) {
        const RegName base = _regs.decodeBaseReg(post >> 5);
        out = outRegister(out, base);
    } else if (spec.base != REG_UNDEF) {
        out = outRegister(out, spec.base);
    }
    if (spec.size == 1 || spec.size == 2) {  // ,X+ ,X++
        *out++ = '+';
        if (spec.size == 2)
            *out++ = '+';
    }
    if (spec.indir)
        *out++ = ']';
    *out = 0;
    return setError(insn);
}

Error DisMc6809::decodeRelative(DisMemory &memory, InsnMc6809 &insn, char *out, AddrMode mode) {
    Config::ptrdiff_t delta;
    if (mode == REL) {
        delta = static_cast<int8_t>(insn.readByte(memory));
    } else {
        delta = static_cast<Config::ptrdiff_t>(insn.readUint16(memory));
    }
    const Config::uintptr_t base = insn.address() + insn.length();
    const Config::uintptr_t target = base + delta;
    if ((delta >= 0 && target < base) || (delta < 0 && target >= base))
        return setError(OPERAND_TOO_FAR);
    const uint8_t deltaWidth = mode == REL ? 8 : 16;
    outRelAddr(out, target, insn.address(), deltaWidth);
    return setError(insn);
}

Error DisMc6809::decodeImmediate(DisMemory &memory, InsnMc6809 &insn, char *out, AddrMode mode) {
    *out++ = '#';
    if (mode == IM8) {
        outHex(out, insn.readByte(memory), 8);
    } else if (mode == IM16) {
        outHex(out, insn.readUint16(memory), 16);
    } else {
        outHex(out, insn.readUint32(memory), 32);
    }
    return setError(insn);
}

Error DisMc6809::decodePushPull(DisMemory &memory, InsnMc6809 &insn, char *out) {
    uint8_t post = insn.readPost(memory);
    const bool hasDreg = (post & 0x06) == 0x06;
    if (hasDreg)
        post &= ~0x02;  // clear REG_A
    const bool onUserStack = (insn.opCode() & 2) != 0;
    const bool push = (insn.opCode() & 1) == 0;
    for (uint8_t i = 0, n = 0; i < 8; i++) {
        const uint8_t bitPos = push ? 7 - i : i;
        if (post & (1 << bitPos)) {
            if (n != 0)
                *out++ = ',';
            RegName reg = _regs.decodeStackReg(bitPos);
            if (reg == REG_U && onUserStack)
                reg = REG_S;
            if (reg == REG_B && hasDreg)
                reg = REG_D;
            out = outRegister(out, reg);
            n++;
        }
    }
    return setError(insn);
}

Error DisMc6809::decodeRegisters(DisMemory &memory, InsnMc6809 &insn, char *out) {
    const uint8_t post = insn.readPost(memory);
    const RegName dst = _regs.decodeDataReg(post);
    const RegName src = _regs.decodeDataReg(post >> 4);
    if (src == REG_UNDEF || dst == REG_UNDEF)
        return setError(ILLEGAL_REGISTER);
    const RegSize size2 = _regs.regSize(dst);
    const RegSize size1 = _regs.regSize(src);
    if (size1 != SZ_NONE && size2 != SZ_NONE && size1 != size2)
        return setError(ILLEGAL_SIZE);
    out = outRegister(out, src);
    *out++ = ',';
    outRegister(out, dst);
    return setError(insn);
}

Error DisMc6809::decodeRegBit(DisMemory &memory, InsnMc6809 &insn, char *out) {
    const uint8_t post = insn.readPost(memory);
    const RegName reg = _regs.decodeBitOpReg(post >> 6);
    if (reg == REG_UNDEF)
        return setError(ILLEGAL_REGISTER);
    out = outRegister(out, reg);
    *out++ = '.';
    out = outHex(out, post & 7, 3);
    return OK;
}

Error DisMc6809::decodeDirBit(DisMemory &memory, InsnMc6809 &insn, char *out) {
    if (decodeDirectPage(memory, insn, out))
        return getError();
    out += strlen(out);
    *out++ = '.';
    outHex(out, (insn.readPost(memory) >> 3) & 7, 3);
    return OK;
}

Error DisMc6809::decodeTransferMemory(DisMemory &memory, InsnMc6809 &insn, char *out) {
    const uint8_t post = insn.readPost(memory);
    const RegName src = _regs.decodeTfmBaseReg(post >> 4);
    const RegName dst = _regs.decodeTfmBaseReg(post);
    const uint8_t mode = insn.opCode() & 0x3;
    if (src == REG_UNDEF || dst == REG_UNDEF)
        return setError(ILLEGAL_REGISTER);
    out = outRegister(out, src);
    const char srcModeChar = _regs.tfmSrcModeChar(mode);
    if (srcModeChar)
        *out++ = srcModeChar;
    *out++ = ',';
    out = outRegister(out, dst);
    const char dstModeChar = _regs.tfmDstModeChar(mode);
    if (dstModeChar)
        *out++ = dstModeChar;
    *out = 0;
    return setError(insn);
}

Error DisMc6809::decodeOperand(DisMemory &memory, InsnMc6809 &insn, char *out, AddrMode mode) {
    switch (mode) {
    case DIR:
        return decodeDirectPage(memory, insn, out);
    case EXT:
        return decodeExtended(memory, insn, out);
    case IDX:
        return decodeIndexed(memory, insn, out);
    case REL:
    case LREL:
        return decodeRelative(memory, insn, out, mode);
    case IM8:
    case IM16:
    case IM32:
        return decodeImmediate(memory, insn, out, mode);
    case REGLIST:
        return decodePushPull(memory, insn, out);
    case REG_REG:
        return decodeRegisters(memory, insn, out);
    case REG_BIT:
        return decodeRegBit(memory, insn, out);
    case DIR_BIT:
        return decodeDirBit(memory, insn, out);
    case REG_TFM:
        return decodeTransferMemory(memory, insn, out);
    default:
        return OK;
    }
}

Error DisMc6809::decode(DisMemory &memory, Insn &_insn, char *out) {
    InsnMc6809 insn(_insn);
    Config::opcode_t opCode = insn.readByte(memory);
    insn.setOpCode(opCode);
    if (TableMc6809::isPrefix(opCode)) {
        const Config::opcode_t prefix = opCode;
        opCode = insn.readByte(memory);
        insn.setOpCode(opCode, prefix);
    }
    if (setError(insn))
        return getError();

    if (TableMc6809.searchOpCode(insn))
        return setError(TableMc6809.getError());

    if (decodeOperand(memory, insn, out, insn.mode1()))
        return getError();
    const AddrMode mode2 = insn.mode2();
    if (mode2 == NONE)
        return OK;
    if (mode2 == REG_TFM)
        return OK;
    out += strlen(out);
    *out++ = ',';
    return decodeOperand(memory, insn, out, mode2);
}

}  // namespace mc6809
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

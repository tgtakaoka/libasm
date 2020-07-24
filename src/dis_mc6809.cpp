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

void DisMc6809::outRegister(RegName regName) {
    _operands = _regs.outRegName(_operands, regName);
}

Error DisMc6809::decodeDirectPage(DisMemory &memory, InsnMc6809 &insn) {
    uint8_t dir;
    if (insn.readByte(memory, dir)) return setError(NO_MEMORY);
    const char *label = lookup(dir);
    if (label) {
        *_operands ++ = '<';
        outText(label);
    } else {
        outConstant(dir, 16, false);
    }
    return setOK();
}

Error DisMc6809::decodeExtended(DisMemory &memory, InsnMc6809 &insn) {
    Config::uintptr_t addr;
    if (insn.readUint16(memory, addr)) return setError(NO_MEMORY);
    if (addr < 0x100) *_operands++ = '>';
    outConstant(addr, 16, false);
    return setOK();
}

Error DisMc6809::decodeIndexed(DisMemory &memory, InsnMc6809 &insn) {
    uint8_t post = 0;
    if (insn.readByte(memory, post)) return setError(NO_MEMORY);
    PostSpec spec;
    if (TableMc6809.searchPostByte(post, spec)) return setError(UNKNOWN_POSTBYTE);
    if (spec.indir) *_operands++ = '[';
    if (spec.mode == DISP_IDX || spec.mode == PNTR_IDX) {
        Config::ptrdiff_t offset = 0;
        const char *force = nullptr;
        if (spec.size == 5) {
            offset = post & 0x1F;
            if (post & 0x10) offset |= 0xFFE0;
            if (offset == 0) force = "<<";
        } else if (spec.size == 8) {
            uint8_t val;
            if (insn.readByte(memory, val)) return setError(NO_MEMORY);
            offset = static_cast<int8_t>(val);
            if (spec.indir && offset == 0) force = "<";
            if (!spec.indir && offset >= -16 && offset < 16) force = "<";
        } else if (spec.size == 16) {
            uint16_t val;
            if (insn.readUint16(memory, val)) return setError(NO_MEMORY);
            offset = static_cast<int16_t>(val);
            if (offset >= -128 && offset < 128) force = ">";
        }
        if (spec.size) {
            if (spec.base == REG_PCR) {
                const Config::uintptr_t target =
                    insn.address() + insn.length() + offset;
                outRelativeAddr(target, insn.address(), spec.size);
            } else {
                if (force) outText(force);
                outConstant(offset, 10);
            }
        }
    }
    if (spec.mode == ACCM_IDX)
        outRegister(spec.index);
    if (spec.mode == ABS_IDIR) {
        Config::uintptr_t addr;
        if (insn.readUint16(memory, addr)) return setError(NO_MEMORY);
        outConstant(addr, 16, false);
    } else {
        *_operands++ = ',';
    }
    if (spec.mode == AUTO_IDX && spec.size < 0) {
        *_operands++ = '-';
        if (spec.size == -2) *_operands++ = '-';
    }
    if (spec.base == REG_X) {
        const RegName base = _regs.decodeBaseReg((post >> 5) & 3);
        outRegister(base);
    } else if (spec.base != REG_UNDEF) {
        outRegister(spec.base);
    }
    if (spec.mode == AUTO_IDX && spec.size > 0) {
        *_operands++ = '+';
        if (spec.size == 2) *_operands++ = '+';
    }
    if (spec.indir) *_operands++ = ']';
    *_operands = 0;
    return setOK();
}

Error DisMc6809::decodeRelative(DisMemory &memory, InsnMc6809 &insn) {
    Config::ptrdiff_t delta;
    if (insn.addrMode() == REL) {
        uint8_t val;
        if (insn.readByte(memory, val)) return setError(NO_MEMORY);
        delta = static_cast<int8_t>(val);
    } else {
        uint16_t val;
        if (insn.readUint16(memory, val)) return setError(NO_MEMORY);
        delta = static_cast<Config::ptrdiff_t>(val);
    }
    const Config::uintptr_t target = insn.address() + insn.length() + delta;
    outRelativeAddr(target, insn.address(), insn.addrMode() == REL ? 8 : 16);
    return setOK();
}

Error DisMc6809::decodeImmediate(DisMemory &memory, InsnMc6809 &insn) {
    *_operands++ = '#';
    if (insn.addrMode() == IM8) {
        uint8_t val;
        if (insn.readByte(memory, val)) return setError(NO_MEMORY);
        const Config::opcode_t opCode = insn.opCode();
        constexpr uint8_t ORCC = 0x1A;
        constexpr uint8_t ANDCC = 0x1C;
        constexpr uint8_t CWAI_BITMD = 0x3C;
        constexpr uint8_t LDMD = 0x3D;
        if (opCode == ORCC || opCode == ANDCC
            || opCode == CWAI_BITMD || opCode == LDMD) {
            outConstant(val, 2, false);
        } else {
            outConstant(val);
        }
    } else if (insn.addrMode() == IM16) {
        uint16_t val;
        if (insn.readUint16(memory, val)) return setError(NO_MEMORY);
        outConstant(val);
    } else if (insn.addrMode() == IM32) {
        uint32_t val;
        if (insn.readUint32(memory, val)) return setError(NO_MEMORY);
        outConstant(val);
    } else {
        return setError(UNKNOWN_INSTRUCTION);
    }
    return setOK();
}

Error DisMc6809::decodePushPull(DisMemory &memory, InsnMc6809 &insn) {
    uint8_t post;
    if (insn.readByte(memory, post)) return setError(NO_MEMORY);
    const bool push = (insn.opCode() & 1) == 0;
    const bool hasDreg = (post & 0x06) == 0x06;
    if (hasDreg) post &= ~0x02; // clear REG_A
    const bool onUserStack = (insn.opCode() & 2) != 0;
    for (uint8_t i = 0, n = 0; i < 8; i++) {
        const uint8_t bitPos = push ? 7 - i : i;
        if (post & (1 << bitPos)) {
            if (n != 0) *_operands++ = ',';
            const RegName regName = _regs.decodeStackReg(bitPos, onUserStack);
            outRegister((hasDreg && regName == REG_B) ? REG_D : regName);
            n++;
        }
    }
    return setOK();
}

Error DisMc6809::decodeRegisters(DisMemory &memory, InsnMc6809 &insn) {
    uint8_t post;
    if (insn.readByte(memory, post)) return setError(NO_MEMORY);
    const RegName src = _regs.decodeRegName(post >> 4);
    const RegName dst = _regs.decodeRegName(post & 0xf);
    if (src == REG_UNDEF || dst == REG_UNDEF)
        return setError(ILLEGAL_REGISTER);
    const RegSize size1 = RegMc6809::regSize(src);
    const RegSize size2 = RegMc6809::regSize(dst);
    if (size1 != SZ_NONE && size2 != SZ_NONE && size1 != size2)
        return setError(ILLEGAL_SIZE);
    outRegister(src);
    *_operands++ = ',';
    outRegister(dst);
    return setOK();
}

Error DisMc6809::decodeBitOperation(DisMemory &memory, InsnMc6809 &insn) {
    uint8_t post;
    if (insn.readByte(memory, post)) return setError(NO_MEMORY);
    const RegName reg = _regs.decodeBitOpReg(post >> 6);
    if (reg == REG_UNDEF) return setError(ILLEGAL_REGISTER);
    outRegister(reg);
    *_operands++ = '.';
    outConstant(static_cast<uint8_t>(post & 7), 10);
    *_operands++ = ',';
    if (decodeDirectPage(memory, insn)) return getError();
    *_operands++ = '.';
    outConstant(static_cast<uint8_t>((post >> 3) & 7));
    return setOK();
}

Error DisMc6809::decodeTransferMemory(DisMemory &memory, InsnMc6809 &insn) {
    uint8_t post;
    if (insn.readByte(memory, post)) return setError(NO_MEMORY);
    const RegName src = _regs.decodeTfmBaseReg(post >> 4);
    const RegName dst = _regs.decodeTfmBaseReg(post & 0xf);
    const uint8_t mode = insn.opCode() & 0x3;
    if (src == REG_UNDEF || dst == REG_UNDEF) return setError(ILLEGAL_REGISTER);
    outRegister(src);
    const char srcModeChar = _regs.tfmSrcModeChar(mode);
    if (srcModeChar) *_operands++ = srcModeChar;
    *_operands++ = ',';
    outRegister(dst);
    const char dstModeChar = _regs.tfmDstModeChar(mode);
    if (dstModeChar) *_operands++ = dstModeChar;
    *_operands = 0;
    return setOK();
}

Error DisMc6809::decode(DisMemory &memory, Insn &_insn) {
    InsnMc6809 insn(_insn);
    Config::opcode_t opCode;
    if (insn.readByte(memory, opCode)) return setError(NO_MEMORY);
    insn.setOpCode(opCode);
    if (TableMc6809::isPrefixCode(opCode)) {
        const Config::opcode_t prefix = opCode;
        if (insn.readByte(memory, opCode)) return setError(NO_MEMORY);
        insn.setOpCode(opCode, prefix);
    }

    if (TableMc6809.searchOpCode(insn))
        return setError(TableMc6809.getError());

    switch (insn.addrMode()) {
    case NONE: return setOK();
    case DIR: return decodeDirectPage(memory, insn);
    case EXT: return decodeExtended(memory, insn);
    case IDX: return decodeIndexed(memory, insn);
    case REL:
    case LREL: return decodeRelative(memory, insn);
    case IM8:
    case IM16:
    case IM32:
        decodeImmediate(memory, insn);
        break;
    case REGLIST: return decodePushPull(memory, insn);
    case REG_REG: return decodeRegisters(memory, insn);
    case REG_BIT:  return decodeBitOperation(memory, insn);
    case REG_TFM: return decodeTransferMemory(memory, insn);
    default:     return setError(INTERNAL_ERROR);
    }
    if (insn.extraMode() != NONE) *_operands++ = ',';
    switch (insn.extraMode()) {
    case DIR: return decodeDirectPage(memory, insn);
    case EXT: return decodeExtended(memory, insn);
    case IDX: return decodeIndexed(memory, insn);
    default: break;
    }
    return setOK();
}

} // namespace mc6809
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

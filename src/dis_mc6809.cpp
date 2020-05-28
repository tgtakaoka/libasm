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
    uint8_t post;
    if (insn.readByte(memory, post)) return setError(NO_MEMORY);
    const uint8_t mode = post & 0x8F;

    uint8_t indir = (post & 0x10);
    const char *label = nullptr;
    Config::uintptr_t addr = 0;
    Config::ptrdiff_t offset = 0;
    host::int_t offSize = 0;
    RegName base = _regs.decodeBaseReg((post >> 5) & 3);
    RegName index = REG_UNDEF;
    host::int_t incr = 0;

    if (mode == 0x84) {
        // ,R [,R]
        ;
    } else if (TableMc6809.is6309() && (post == 0x8F || post == 0x90)) {
        // ,W [,W]
        base = REG_W;
    } else if (TableMc6809.is6309() && (post == 0xAF || post == 0xB0)) {
        // n16,W [n16,W]
        base = REG_W;
        offSize = 16;
        if (insn.readUint16(memory, addr)) return setError(NO_MEMORY);
        offset = static_cast<int16_t>(addr);
    } else if (TableMc6809.is6309()
               && (post == 0xCF || post == 0xD0 || post == 0xEF || post == 0xF0)) {
        // ,W++ ,--W [,W++] [,--W]
        base = REG_W;
        incr = (post < 0xE0) ? 2 : -2;
    } else if ((post & 0x80) == 0) {
        // n5,R
        offSize = 5;
        addr = post & 0x1F;
        if (post & 0x10) addr |= 0xFFE0;
        offset = static_cast<int16_t>(addr);
        indir = 0;
    } else if (mode == 0x88) {
        // n8,R [n8,R]
        offSize = 8;
        uint8_t val;
        if (insn.readByte(memory, val)) return setError(NO_MEMORY);
        addr = val;
        offset = static_cast<int8_t>(val);
    } else if (mode == 0x89) {
        // n16,R [n16,R]
        offSize = 16;
        if (insn.readUint16(memory, addr)) return setError(NO_MEMORY);
        offset = static_cast<int16_t>(addr);
    } else if (_regs.decodeIndexReg(post & 0xf) != REG_UNDEF) {
        // R,R [R,R]
        index = _regs.decodeIndexReg(mode & 0xf);
    } else if (mode == 0x80) {
        // ,R+
        if (indir) return setError(UNKNOWN_POSTBYTE);
        incr = 1;
    } else if (mode == 0x82) {
        // ,-R
        if (indir) return setError(UNKNOWN_POSTBYTE);
        incr = -1;
    } else if (mode == 0x81 || mode == 0x83) {
        // ,R++ ,--R, [,R++] [,--R]
        incr = (mode == 0x81) ? 2 : -2;
    } else if (post == 0x8C || post == 0x8D || post == 0x9C || post == 0x9D) {
        // n8,PCR n16,PCR [n8,PCR] [n16,PCR]
        base = REG_PCR;
        offSize = -1;
        if (mode == 0x8C) {
            uint8_t val;
            if (insn.readByte(memory, val)) return setError(NO_MEMORY);
            offset = static_cast<int8_t>(val);
        } else {
            uint16_t val;
            if (insn.readUint16(memory, val)) return setError(NO_MEMORY);
            offset = static_cast<int16_t>(val);
        }
        addr = insn.address() + insn.length() + offset;
        label = lookup(addr);
    } else if (post == 0x9F) {
        // [n16]
        base = REG_UNDEF;
        if (insn.readUint16(memory, addr)) return setError(NO_MEMORY);
        label = lookup(addr);
        offSize = -1;
    } else {
        return setError(UNKNOWN_POSTBYTE);
    }

    if (indir) *_operands++ = '[';
    if (label) {
        outText(label);
    } else {
        if (base) {
            if (index) {
                outRegister(index);
            } else if (offSize < 0) {
                outConstant(addr, 16, false);
            } else if (offSize != 0) {
                if (offSize == 16 && offset >= -128 && offset < 128)
                    outText(">");
                if (offSize == 8) {
                    if (!indir && offset >= -16 && offset < 16)
                        outText("<");
                    if (indir && offset == 0)
                        outText("<");
                }
                if (offSize == 5 && offset == 0)
                    outText("<<");
                outConstant(offset, 10);
            }
        } else {
            outConstant(addr, 16, false);
        }
    }
    if (base) {
        *_operands++ = ',';
        for (; incr < 0; incr++) *_operands++ = '-';
        outRegister(base);
        for (; incr > 0; incr--) *_operands++ = '+';
    }
    if (indir) *_operands++ = ']';
    *_operands = 0;
    return setOK();
}

Error DisMc6809::decodeRelative(DisMemory &memory, InsnMc6809 &insn) {
    Config::ptrdiff_t delta;
    if (insn.oprSize() == SZ_BYTE) {
        uint8_t val;
        if (insn.readByte(memory, val)) return setError(NO_MEMORY);
        delta = static_cast<int8_t>(val);
    } else {
        uint16_t val;
        if (insn.readUint16(memory, val)) return setError(NO_MEMORY);
        delta = static_cast<Config::ptrdiff_t>(val);
    }
    const Config::uintptr_t addr = insn.address() + insn.length() + delta;
    outConstant(addr, 16, false);
    return setOK();
}

Error DisMc6809::decodeImmediate(DisMemory &memory, InsnMc6809 &insn) {
    *_operands++ = '#';
    if (insn.oprSize() == SZ_BYTE) {
        uint8_t val;
        if (insn.readByte(memory, val)) return setError(NO_MEMORY);
        const Config::opcode_t opCode = insn.opCode();
        constexpr uint8_t ORCC = 0x1A;
        constexpr uint8_t ANDCC = 0x1C;
        constexpr uint8_t CWAI_BITMD = 0x3C;
        constexpr uint8_t LDMD = 0x3D;
        if (opCode == ORCC || opCode == ANDCC
            || opCode == CWAI_BITMD || opCode == LDMD) {
            outConstant(val, 2);
        } else {
            outConstant(val);
        }
    } else if (insn.oprSize() == SZ_WORD) {
        uint16_t val;
        if (insn.readUint16(memory, val)) return setError(NO_MEMORY);
        outConstant(val);
    } else if (insn.oprSize() == SZ_LONG) {
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
    for (host::uint_t i = 0, n = 0; i < 8; i++) {
        const host::uint_t bitPos = push ? 7 - i : i;
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
    const OprSize size1 = RegMc6809::regSize(src);
    const OprSize size2 = RegMc6809::regSize(dst);
    if (size1 != SZ_NONE && size2 != SZ_NONE && size1 != size2)
        return setError(ILLEGAL_SIZE);
    outRegister(src);
    *_operands++ = ',';
    outRegister(dst);
    return setOK();
}

Error DisMc6809::decodeImmediatePlus(DisMemory &memory, InsnMc6809 &insn) {
    *_operands++ = '#';
    uint8_t val;
    if (insn.readByte(memory, val)) return setError(NO_MEMORY);
    outConstant(val);
    *_operands++ = ',';
    switch (insn.addrMode()) {
    case IMM_DIR: return decodeDirectPage(memory, insn);
    case IMM_EXT: return decodeExtended(memory, insn);
    case IMM_IDX: return decodeIndexed(memory, insn);
    default:     return setError(INTERNAL_ERROR);
    }
}

Error DisMc6809::decodeBitOperation(DisMemory &memory, InsnMc6809 &insn) {
    uint8_t post;
    if (insn.readByte(memory, post)) return setError(NO_MEMORY);
    const RegName reg = _regs.decodeBitOpReg(post >> 6);
    if (reg == REG_UNDEF) return setError(ILLEGAL_REGISTER);
    outRegister(reg);
    *_operands++ = '.';
    outConstant(uint8_t(post & 7), 10);
    *_operands++ = ',';
    if (decodeDirectPage(memory, insn)) return getError();
    *_operands++ = '.';
    outConstant(uint8_t((post >> 3) & 7));
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
        return setError(UNKNOWN_INSTRUCTION);

    switch (insn.addrMode()) {
    case INH: return setOK();
    case DIR: return decodeDirectPage(memory, insn);
    case EXT: return decodeExtended(memory, insn);
    case IDX: return decodeIndexed(memory, insn);
    case REL: return decodeRelative(memory, insn);
    case IMM: return decodeImmediate(memory, insn);
    case PSH_PUL: return decodePushPull(memory, insn);
    case REG_REG: return decodeRegisters(memory, insn);
    case IMM_DIR:
    case IMM_EXT:
    case IMM_IDX: return decodeImmediatePlus(memory, insn);
    case BITOP:   return decodeBitOperation(memory, insn);
    case TFR_MEM: return decodeTransferMemory(memory, insn);
    default:      return setError(INTERNAL_ERROR);
    }
}

} // namespace mc6809
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

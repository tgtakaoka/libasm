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

#include "dis_z80.h"
#include "table_z80.h"

namespace libasm {
namespace z80 {

template<typename T>
char *DisZ80::outAbsolute(char *out, T addr) {
    *out++ = '(';
    out = outAddress(out, addr);
    *out++ = ')';
    *out = 0;
    return out;
}

char *DisZ80::outIndexOffset(const InsnZ80 &insn, char *out, int8_t offset) {
    *out++ = '(';
    out = outRegister(out, RegZ80::decodeIndexReg(insn));
    if (offset >= 0) *out++ = '+';
    out = outConstant(out, offset, 10);
    *out++ = ')';
    *out = 0;
    return out;
}

char *DisZ80::outRegister(char *out, RegName regName) {
    return _regs.outRegName(out, regName);
}

char *DisZ80::outPointer(char *out, RegName regName) {
    *out++ = '(';
    out = _regs.outRegName(out, regName);
    *out++ = ')';
    *out = 0;
    return out;
}

char *DisZ80::outDataRegister(char *out, RegName regName) {
    return (regName == REG_UNDEF) ? outPointer(out, REG_HL)
        : outRegister(out, regName);
}

char *DisZ80::outConditionName(char *out, Config::opcode_t cc, bool cc8) {
    return cc8 ? _regs.outCc8Name(out, cc) : _regs.outCc4Name(out, cc);
}

Error DisZ80::decodeInherent(InsnZ80 &insn, char *out) {
    const Config::opcode_t opc = insn.opCode();
    switch (insn.dstFormat()) {
    case A_REG:
        out = outRegister(out, REG_A);
        break;
    case REG_8:
        if (insn.insnFormat() == DST_FMT || insn.insnFormat() == DST_SRC_FMT) {
            const RegName regName = RegZ80::decodeDataReg(opc >> 3);
            if (regName == REG_UNDEF && insn.srcFormat() == C_PTR)
                return setError(UNKNOWN_INSTRUCTION);
            out = outDataRegister(out, regName);
        } else if (insn.insnFormat() == SRC_FMT) {
            out = outDataRegister(out, RegZ80::decodeDataReg(opc));
        } else return setError(INTERNAL_ERROR);
        break;
    case IR_REG:
        out = outRegister(out, RegZ80::decodeIrReg(opc >> 3));
        break;
    case BC_REG:
        out = outRegister(out, REG_BC);
        break;
    case DE_REG:
        out = outRegister(out, REG_DE);
        break;
    case HL_REG:
        out = outRegister(out, REG_HL);
        break;
    case AF_REG:
        out = outRegister(out, REG_AF);
        break;
    case SP_REG:
        out = outRegister(out, REG_SP);
        break;
    case REG_16:
        out = outRegister(out, RegZ80::decodePointerReg(opc >> 4));
        break;
    case STK_16:
        out = outRegister(out, RegZ80::decodeStackReg(opc >> 4));
        break;
    case IX_REG:
        out = outRegister(out, RegZ80::decodeIndexReg(insn));
        break;
    case COND_8:
        out = outConditionName(out, (opc >> 3) & 7);
        break;
    case C_PTR:
        if (insn.srcFormat() == REG_8 && insn.insnFormat() == DST_FMT
            && RegZ80::decodeDataReg(opc >> 3) == REG_UNDEF)
            return setError(UNKNOWN_INSTRUCTION);
        out = outPointer(out, REG_C);
        break;
    case BC_PTR:
        out = outPointer(out, RegZ80::decodeIndirectBase(opc >> 4));
        break;
    case HL_PTR:
        out = outPointer(out, REG_HL);
        break;
    case SP_PTR:
        out = outPointer(out, REG_SP);
        break;
    case IX_PTR:
        out = outPointer(out, RegZ80::decodeIndexReg(insn));
        break;
    case VEC_NO:
        out = outConstant(out, static_cast<uint8_t>(opc & 0x38), 16, false);
        break;
    case BIT_NO:
        out = outConstant(out, static_cast<uint8_t>((opc >> 3) & 7));
        break;
    case IMM_NO:
        switch (opc) {
        case 0x46: *out++ = '0'; break;
        case 0x56: *out++ = '1'; break;
        case 0x5E: *out++ = '2'; break;
        default: return setError(UNKNOWN_INSTRUCTION);
        }
        *out = 0;
        break;
    case IM_REG:
        out = outRegister(out, REG_IM);
        break;
    default:
        break;
    }

    if (insn.dstFormat() != NO_OPR && insn.srcFormat() != NO_OPR)
        *out++ = ',';

    switch (insn.srcFormat()) {
    case A_REG:
        out = outRegister(out, REG_A);
        break;
    case REG_8:
        if (insn.insnFormat() == SRC_FMT || insn.insnFormat() == DST_SRC_FMT) {
            out = outDataRegister(out, RegZ80::decodeDataReg(opc));
        } else if (insn.insnFormat() == DST_FMT) {
            out = outDataRegister(out, RegZ80::decodeDataReg(opc >> 3));
        } else return setError(INTERNAL_ERROR);
        break;
    case IR_REG:
        out = outRegister(out, RegZ80::decodeIrReg(opc >> 3));
        break;
    case AFPREG:
        out = outRegister(out, REG_AFP);
        break;
    case HL_REG:
        out = outRegister(out, REG_HL);
        break;
    case REG_16:
        out = outRegister(out, RegZ80::decodePointerReg(opc >> 4));
        break;
    case IX_REG:
        out = outRegister(out, RegZ80::decodeIndexReg(insn));
        break;
    case REG_16X:
        out = outRegister(out, RegZ80::decodePointerReg(opc >> 4, &insn));
        break;
    case C_PTR:
        out = outPointer(out, REG_C);
        break;
    case BC_PTR:
        out = outPointer(out, RegZ80::decodeIndirectBase(opc >> 4));
        break;
    case IM_REG:
        out = outRegister(out, REG_IM);
    default:
        break;
    }

    *out = 0;
    return setOK();
}

Error DisZ80::decodeImmediate8(InsnZ80 &insn, char *out, uint8_t val) {
    const Config::opcode_t opc = insn.opCode();
    switch (insn.dstFormat()) {
    case A_REG:
        out = outRegister(out, REG_A);
        break;
    case IMM_8:
        out = outConstant(out, val);
        break;
    case REG_8:
        out = outDataRegister(out, RegZ80::decodeDataReg(opc >> 3));
        break;
    default:
        break;
    }
    if (insn.srcFormat() == IMM_8) {
        *out++ = ',';
        outConstant(out, val);
    }
    return setOK();
}

Error DisZ80::decodeImmediate16(InsnZ80 &insn, char *out, uint16_t val) {
    const Config::opcode_t opc = insn.opCode();
    switch (insn.dstFormat()) {
    case REG_16:
        out = outRegister(out, RegZ80::decodePointerReg((opc >> 4) & 3));
        break;
    case IX_REG:
        out = outRegister(out, RegZ80::decodeIndexReg(insn));
        break;
    default:
        break;
    }
    *out++ = ',';
    outConstant(out, val);
    return setOK();
}

Error DisZ80::decodeDirect(InsnZ80 &insn, char *out, Config::uintptr_t addr) {
    const Config::opcode_t opc = insn.opCode();
    RegName regName;
    switch (insn.dstFormat()) {
    case ADDR_16:
        out = outAbsolute(out, addr);
        break;
    case HL_REG:
        out = outRegister(out, REG_HL);
        break;
    case A_REG:
        out = outRegister(out, REG_A);
        break;
    case COND_8:
        out = outConditionName(out, (opc >> 3) & 7);
        break;
    case IMM_16:
        out = outAddress(out, addr);
        break;
    case REG_16:
        regName = RegZ80::decodePointerReg((opc >> 4) & 3);
        if (regName == REG_HL && insn.insnFormat() == PTR_FMT)
            return setError(UNKNOWN_INSTRUCTION);
        out = outRegister(out, regName);
        break;
    case IX_REG:
        out = outRegister(out, RegZ80::decodeIndexReg(insn));
        break;
    default:
        break;
    }
    if (insn.srcFormat() != NO_OPR) *out++ = ',';
    switch (insn.srcFormat()) {
    case HL_REG:
        out = outRegister(out, REG_HL);
        break;
    case ADDR_16:
        out = outAbsolute(out, addr);
        break;
    case A_REG:
        out = outRegister(out, REG_A);
        break;
    case IMM_16:
        out = outAddress(out, addr);
        break;
    case REG_16:
        regName = RegZ80::decodePointerReg((opc >> 4) & 3);
        if (regName == REG_HL && insn.insnFormat() == PTR_FMT)
            return setError(UNKNOWN_INSTRUCTION);
        out = outRegister(out, regName);
        break;
    case IX_REG:
        out = outRegister(out, RegZ80::decodeIndexReg(insn));
        break;
    default:
        break;
    }
    return setOK();
}

Error DisZ80::decodeIoaddr(InsnZ80 &insn, char *out, uint8_t ioaddr) {
    switch (insn.dstFormat()) {
    case ADDR_8:
        out = outAbsolute(out, ioaddr);
        break;
    case A_REG:
        out = outRegister(out, REG_A);
        break;
    default:
        break;
    }
    *out++ = ',';
    switch (insn.srcFormat()) {
    case ADDR_8:
        out = outAbsolute(out, ioaddr);
        break;
    case A_REG:
        out = outRegister(out, REG_A);
        break;
    default:
        break;
    }
    return setOK();
}

Error DisZ80::decodeRelative(InsnZ80 &insn, char *out, int8_t delta) {
    if (insn.dstFormat() == COND_4) {
        const Config::opcode_t opc = insn.opCode();
        out = outConditionName(out, (opc >> 3) & 3, false);
        *out++ = ',';
    }
    const Config::uintptr_t target = insn.address() + 2 + delta;
    outRelativeAddr(out, target, insn.address(), 8);
    return setOK();
}

Error DisZ80::decodeIndexed(InsnZ80 &insn, char *out, int8_t offset) {
    const Config::opcode_t opc = insn.opCode();
    RegName regName;
    switch (insn.dstFormat()) {
    case IX_OFF:
        if (insn.srcFormat() == REG_8 && RegZ80::decodeDataReg(opc) == REG_UNDEF)
            return setError(ILLEGAL_OPERAND);
        out = outIndexOffset(insn, out, offset);
        break;
    case REG_8:
        regName = RegZ80::decodeDataReg(opc >> 3);
        if (regName == REG_UNDEF) return setError(UNKNOWN_INSTRUCTION);
        out = outDataRegister(out, regName);
        break;
    case A_REG:
        out = outRegister(out, REG_A);
        break;
    default:
        break;
    }
    if (insn.srcFormat() != NO_OPR) *out++ = ',';
    switch (insn.srcFormat()) {
    case IX_OFF:
        out = outIndexOffset(insn, out, offset);
        break;
    case REG_8:
        regName = RegZ80::decodeDataReg(opc);
        if (regName == REG_UNDEF) return setError(UNKNOWN_INSTRUCTION);
        out = outDataRegister(out, regName);
        break;
    default:
        break;
    }
    return setOK();
}

Error DisZ80::decodeIndexedImmediate8(
    InsnZ80 &insn, char *out, int8_t offset, uint8_t val) {
    out = outIndexOffset(insn, out, offset);
    *out++ = ',';
    outConstant(out, val);
    return setOK();
}

Error DisZ80::decodeIndexedBitOp(
    InsnZ80 &insn, char *out, int8_t offset, Config::opcode_t opCode) {
    InsnZ80 ixBit(insn);
    ixBit.setInsnCode(insn.opCode(), opCode);
    if (TableZ80.searchOpCode(ixBit)) 
        return setError(TableZ80.getError());
    insn.setName(ixBit.name());

    const RegName regName = RegZ80::decodeDataReg(opCode);
    if (ixBit.dstFormat() == BIT_NO
        && ixBit.srcFormat() == REG_8 && regName == REG_UNDEF) {
        out = outConstant(out, static_cast<uint8_t>((opCode >> 3) & 7));
        *out++ = ',';
        out = outIndexOffset(insn, out, offset);
    } else if (ixBit.dstFormat() == REG_8 && regName == REG_UNDEF
               && ixBit.srcFormat() == NO_OPR) {
        out = outIndexOffset(insn, out, offset);
    } else {
        return setError(UNKNOWN_INSTRUCTION);
    }
    return setOK();
}

Error DisZ80::decode(DisMemory &memory, Insn &_insn, char *out) {
    InsnZ80 insn(_insn);
    Config::opcode_t opCode;
    if (insn.readByte(memory, opCode)) return setError(NO_MEMORY);
    insn.setInsnCode(0, opCode);
    if (TableZ80.cpuType() == Z80 && TableZ80::isPrefixCode(opCode)) {
        const Config::opcode_t prefix = opCode;
        if (insn.readByte(memory, opCode)) return setError(NO_MEMORY);
        insn.setInsnCode(prefix, opCode);
    }

    if (TableZ80.searchOpCode(insn))
        return setError(TableZ80.getError());

    uint8_t u8;
    uint16_t u16;
    uint8_t offset;

    switch (insn.addrMode()) {
    case INHR:
        return decodeInherent(insn, out);
    case IMM8:
        if (insn.readByte(memory, u8)) return setError(NO_MEMORY);
        return decodeImmediate8(insn, out, u8);
    case IMM16:
        if (insn.readUint16(memory, u16)) return setError(NO_MEMORY);
        return decodeImmediate16(insn, out, u16);
    case DIRECT:
        if (insn.readUint16(memory, u16)) return setError(NO_MEMORY);
        return decodeDirect(insn, out, u16);
    case IOADR:
        if (insn.readByte(memory, u8)) return setError(NO_MEMORY);
        return decodeIoaddr(insn, out, u8);
    case REL8:
        if (insn.readByte(memory, u8)) return setError(NO_MEMORY);
        return decodeRelative(insn, out, u8);
    case INDX:
        if (insn.readByte(memory, u8)) return setError(NO_MEMORY);
        return decodeIndexed(insn, out, u8);
    case INDX_IMM8:
        if (insn.readByte(memory, offset)) return setError(NO_MEMORY);
        if (insn.readByte(memory, u8)) return setError(NO_MEMORY);
        if (insn.dstFormat() == IX_BIT)
            return decodeIndexedBitOp(insn, out, offset, u8);
        return decodeIndexedImmediate8(insn, out, offset, u8);
    default:
        return setError(INTERNAL_ERROR);
    }
}

} // namespace z80
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

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

#include "dis_i8051.h"
#include "table_i8051.h"

namespace libasm {
namespace i8051 {

void DisI8051::outRegister(RegName regName) {
    _operands = _regs.outRegName(_operands, regName);
}

Error DisI8051::decodeRelative(DisMemory &memory, InsnI8051 &insn) {
    uint8_t val8;
    if (insn.readByte(memory, val8)) return setError(NO_MEMORY);
    const Config::ptrdiff_t delta = static_cast<int8_t>(val8);
    const Config::uintptr_t target = insn.address() + insn.length() + delta;
    outRelativeAddr(target, insn.address(), 8);
    return setOK();
}

Error DisI8051::decodeBitAddr(DisMemory &memory, InsnI8051 &insn) {
    uint8_t val8;
    if (insn.readByte(memory, val8)) return setError(NO_MEMORY);
    const uint8_t addr8 = (val8 & 0x80) ? (val8 & ~7) : ((val8 >> 3) + 0x20);
    val8 &= 7;
    outConstant(addr8, 16, false, true);
    *_operands++ = '.';
    outConstant(val8, 10);
    return setOK();
}

Error DisI8051::decodeRReg(
    DisMemory &memory, InsnI8051 &insn, const AddrMode mode) {
    const RegName reg = _regs.decodeRReg(insn.opCode() & (mode == IDIRR ? 1 : 7));
    if (mode == IDIRR) *_operands++ = '@';
    _operands = _regs.outRegName(_operands, reg);
    return setOK();
}

Error DisI8051::decodeAddress(
    DisMemory &memory, InsnI8051 &insn, const AddrMode mode) {
    if (mode == ADR8) {
        uint8_t addr8;
        if (insn.readByte(memory, addr8)) return setError(NO_MEMORY);
        outConstant(addr8, 16, false, true);
    } else if (mode == ADR11) {
        uint8_t val8;
        if (insn.readByte(memory, val8)) return setError(NO_MEMORY);
        Config::uintptr_t addr = (insn.address() + insn.length()) & 0xF800;
        addr |= (insn.opCode() & 0xE0) << 3;
        addr |= val8;
        outConstant(addr, 16, false, true);
    } else {
        Config::uintptr_t addr16;
        if (insn.readUint16(memory, addr16)) return setError(NO_MEMORY);
        outConstant(addr16, 16, false, true);
    }
    return setOK();
}

Error DisI8051::decodeImmediate(
    DisMemory &memory, InsnI8051 &insn, const AddrMode mode) {
    *_operands++ = '#';
    if (mode == IMM8) {
        uint8_t val8;
        if (insn.readByte(memory, val8)) return setError(NO_MEMORY);
        outConstant(val8, 16);
    } else {
        uint16_t val16;
        if (insn.readUint16(memory, val16)) return setError(NO_MEMORY);
        outConstant(val16, 16);
    }
    return setOK();
}

Error DisI8051::decodeOperand(
    DisMemory &memory, InsnI8051 &insn, const AddrMode mode) {
    switch (mode) {
    case NONE:
        break;
    case REL:
        return decodeRelative(memory, insn);
    case AREG:
        outRegister(REG_A);
        break;
    case RREG:
    case IDIRR:
        return decodeRReg(memory, insn, mode);
    case CREG:
        outRegister(REG_C);
        break;
    case IDIRD:
        *_operands++ = '@';
        /* Fall-through */
    case DREG:
        outRegister(REG_DPTR);
        break;
    case ABREG:
        outRegister(REG_AB);
        break;
    case ADR8:
    case ADR11:
    case ADR16:
        return decodeAddress(memory, insn, mode);
    case NOTAD:
        *_operands++ = '/';
        /* Fall-through */
    case BITAD:
        return decodeBitAddr(memory, insn);
    case IMM8:
    case IMM16:
        return decodeImmediate(memory, insn, mode);
    case INDXD:
        *_operands++ = '@';
        outRegister(REG_A);
        *_operands++ = '+';
        outRegister(REG_DPTR);
        break;
    case INDXP:
        *_operands++ = '@';
        outRegister(REG_A);
        *_operands++ = '+';
        outRegister(REG_PC);
        break;
    }
    return setOK();
}

Error DisI8051::decode(
    DisMemory &memory, Insn &_insn) {
    InsnI8051 insn(_insn);
    Config::opcode_t opCode;
    if (insn.readByte(memory, opCode)) return setError(NO_MEMORY);
    insn.setOpCode(opCode);
    if (TableI8051.searchOpCode(insn))
        return setError(TableI8051.getError());

    const AddrMode dst = insn.dstMode();
    const AddrMode src = insn.srcMode();
    if (dst == ADR8 && src == ADR8) { // MOV dst,src
        uint8_t src8, dst8;
        if (insn.readByte(memory, src8)) return setError(NO_MEMORY);
        if (insn.readByte(memory, dst8)) return setError(NO_MEMORY);
        outConstant(dst8, 16, false);
        *_operands++ = ',';
        outConstant(src8, 16, false);
    } else {
        if (dst != NONE)
            if (decodeOperand(memory, insn, dst)) return getError();
        if (src != NONE) {
            *_operands++ = ',';
            if (decodeOperand(memory, insn, src)) return getError();
        }
    }
    const AddrMode ext = insn.extMode();
    if (ext != NONE) {
        *_operands++ = ',';
        if (decodeOperand(memory, insn, ext)) return getError();
    }
    return setOK();
}

} // namespace i8051
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

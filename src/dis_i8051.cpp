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

StrBuffer &DisI8051::outRegister(StrBuffer &out, RegName regName) {
    return _regs.outRegName(out, regName);
}

Error DisI8051::decodeRelative(DisMemory &memory, InsnI8051 &insn, StrBuffer &out) {
    const Config::ptrdiff_t delta = static_cast<int8_t>(insn.readByte(memory));
    const Config::uintptr_t target = insn.address() + insn.length() + delta;
    outRelAddr(out, target, insn.address(), 8);
    return setError(insn);
}

Error DisI8051::decodeBitAddr(DisMemory &memory, InsnI8051 &insn, StrBuffer &out) {
    uint8_t val8 = insn.readByte(memory);
    const uint8_t addr8 = (val8 & 0x80) ? (val8 & ~7) : ((val8 >> 3) + 0x20);
    val8 &= 7;
    outAbsAddr(out, addr8, 8).letter('.');
    outHex(out, val8, 3);
    return setError(insn);
}

Error DisI8051::decodeRReg(InsnI8051 &insn, StrBuffer &out, const AddrMode mode) {
    const RegName reg = _regs.decodeRReg(insn.opCode() & (mode == IDIRR ? 1 : 7));
    if (mode == IDIRR)
        out.letter('@');
    outRegister(out, reg);
    return setOK();
}

Error DisI8051::decodeAddress(DisMemory &memory, InsnI8051 &insn, StrBuffer &out, const AddrMode mode) {
    if (mode == ADR8) {
        outAbsAddr(out, insn.readByte(memory), 8);
    } else if (mode == ADR11) {
        const uint8_t val8 = insn.readByte(memory);
        Config::uintptr_t addr = (insn.address() + insn.length()) & 0xF800;
        addr |= (insn.opCode() & 0xE0) << 3;
        addr |= val8;
        outAbsAddr(out, addr);
    } else {
        outAbsAddr(out, insn.readUint16(memory));
    }
    return setError(insn);
}

Error DisI8051::decodeImmediate(
        DisMemory &memory, InsnI8051 &insn, StrBuffer &out, const AddrMode mode) {
    out.letter('#');
    if (mode == IMM8) {
        outHex(out, insn.readByte(memory), 8);
    } else {
        outHex(out, insn.readUint16(memory), 16);
    }
    return setError(insn);
}

Error DisI8051::decodeOperand(DisMemory &memory, InsnI8051 &insn, StrBuffer &out, const AddrMode mode) {
    switch (mode) {
    case NONE:
        break;
    case REL:
        return decodeRelative(memory, insn, out);
    case AREG:
        outRegister(out, REG_A);
        break;
    case RREG:
    case IDIRR:
        return decodeRReg(insn, out, mode);
    case CREG:
        outRegister(out, REG_C);
        break;
    case IDIRD:
        out.letter('@');
        /* Fall-through */
    case DREG:
        outRegister(out, REG_DPTR);
        break;
    case ABREG:
        outRegister(out, REG_AB);
        break;
    case ADR8:
    case ADR11:
    case ADR16:
        return decodeAddress(memory, insn, out, mode);
    case NOTAD:
        out.letter('/');
        /* Fall-through */
    case BITAD:
        return decodeBitAddr(memory, insn, out);
    case IMM8:
    case IMM16:
        return decodeImmediate(memory, insn, out, mode);
    case INDXD:
        outRegister(out.letter('@'), REG_A).letter('+');
        outRegister(out, REG_DPTR);
        break;
    case INDXP:
        outRegister(out.letter('@'), REG_A).letter('+');
        outRegister(out, REG_PC);
        break;
    }
    return setOK();
}

Error DisI8051::decode(DisMemory &memory, Insn &_insn, StrBuffer &out) {
    InsnI8051 insn(_insn);
    const Config::opcode_t opCode = insn.readByte(memory);
    insn.setOpCode(opCode);
    if (setError(insn))
        return getError();

    if (TableI8051::TABLE.searchOpCode(insn))
        return setError(TableI8051::TABLE.getError());

    const AddrMode dst = insn.dstMode();
    const AddrMode src = insn.srcMode();
    if (dst == ADR8 && src == ADR8) {  // MOV dst,src
        const uint8_t src8 = insn.readByte(memory);
        const uint8_t dst8 = insn.readByte(memory);
        outAbsAddr(out, dst8, 8).comma();
        outAbsAddr(out, src8, 8);
    } else {
        if (dst != NONE) {
            if (decodeOperand(memory, insn, out, dst))
                return getError();
        }
        if (src != NONE) {
            out.comma();
            if (decodeOperand(memory, insn, out, src))
                return getError();
        }
    }
    const AddrMode ext = insn.extMode();
    if (ext != NONE) {
        out.comma();
        if (decodeOperand(memory, insn, out, ext))
            return getError();
    }
    return setError(insn);
}

}  // namespace i8051
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

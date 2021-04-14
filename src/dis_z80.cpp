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

char *DisZ80::outIndirectAddr(char *out, uint16_t addr, uint8_t bits) {
    *out++ = '(';
    out = outHex(out, addr, bits);
    *out++ = ')';
    *out = 0;
    return out;
}

char *DisZ80::outRegister(char *out, RegName reg) {
    return _regs.outRegName(out, reg);
}

char *DisZ80::outIndirectReg(char *out, RegName reg) {
    *out++ = '(';
    out = outRegister(out, reg);
    *out++ = ')';
    *out = 0;
    return out;
}

char *DisZ80::outIndexOffset(char *out, RegName reg, int8_t offset) {
    *out++ = '(';
    out = outRegister(out, reg);
    if (offset >= 0)
        *out++ = '+';
    out = outDec(out, offset, -8);
    *out++ = ')';
    *out = 0;
    return out;
}

char *DisZ80::outDataReg(char *out, RegName reg) {
    if (reg == REG_HL)
        return outIndirectReg(out, reg);
    return outRegister(out, reg);
}

Error DisZ80::decodeIndexedBitOp(DisMemory &memory, InsnZ80 &insn, char *out) {
    const int8_t offset = insn.readByte(memory);
    const Config::opcode_t opc = insn.readByte(memory);

    InsnZ80 ixBit(insn);
    ixBit.setOpCode(opc, insn.opCode());
    if (TableZ80.searchOpCode(ixBit))
        return setError(TableZ80.getError());
    const char *name = ixBit.name();
    insn.setName(name, name + strlen(name));

    const RegName reg = RegZ80::decodeDataReg(opc);
    if (reg != REG_HL)
        return setError(UNKNOWN_INSTRUCTION);
    const AddrMode dst = ixBit.dstMode();
    if (dst == M_BIT) {
        out = outHex(out, (opc >> 3) & 7, 3);
        *out++ = ',';
    }
    outIndexOffset(out, RegZ80::decodeIndexReg(insn), offset);
    return setError(insn);
}

Error DisZ80::decodeRelative(DisMemory &memory, InsnZ80 &insn, char *out) {
    const int8_t delta = static_cast<int8_t>(insn.readByte(memory));
    const Config::uintptr_t target = insn.address() + insn.length() + delta;
    outRelAddr(out, target, insn.address(), 8);
    return setError(insn);
}

Error DisZ80::decodeOperand(DisMemory &memory, InsnZ80 &insn, char *out, AddrMode mode) {
    Config::opcode_t opc = insn.opCode();
    switch (mode) {
    case M_IM8:
        outHex(out, insn.readByte(memory), 8);
        break;
    case M_IM16:
        outHex(out, insn.readUint16(memory), 16);
        break;
    case M_ABS:
        outIndirectAddr(out, insn.readUint16(memory), 16);
        break;
    case M_IOA:
        outIndirectAddr(out, insn.readByte(memory), 8);
        break;
    case M_INDX:
        outIndexOffset(out, RegZ80::decodeIndexReg(insn), insn.readByte(memory));
        break;
    case M_CC4:
        _regs.outCcName(out, RegZ80::decodeCcName((opc >> 3) & 3));
        return OK;
    case M_CC8:
        _regs.outCcName(out, RegZ80::decodeCcName(opc >> 3));
        return OK;
    case M_REL:
        return decodeRelative(memory, insn, out);
    case M_PTR:
    case M_PIX:
        outRegister(out, RegZ80::decodePointerReg(opc >> 4, insn));
        return OK;
    case M_STK:
        outRegister(out, RegZ80::decodeStackReg(opc >> 4));
        return OK;
    case I_BCDE:
        outIndirectReg(out, RegZ80::decodeIndirectBase(opc >> 4));
        return OK;
    case M_REG:
        outDataReg(out, RegZ80::decodeDataReg(opc));
        return OK;
    case M_DST:
        outDataReg(out, RegZ80::decodeDataReg(opc >> 3));
        return OK;
    case M_VEC:
        outHex(out, opc & 0x38, 8);
        return OK;
    case M_BIT:
        outHex(out, (opc >> 3) & 7, 3);
        return OK;
    case M_IMMD:
        opc = (opc >> 3) & 3;
        if (opc == 1)
            return setError(UNKNOWN_INSTRUCTION);
        if (opc)
            opc--;
        *out++ = opc + '0';
        *out = 0;
        return OK;
    case R_IR:
        outRegister(out, RegZ80::decodeIrReg(opc >> 3));
        return OK;
    case R_IXIY:
        outRegister(out, RegZ80::decodeIndexReg(insn));
        return OK;
    case R_A:
        outRegister(out, REG_A);
        return OK;
    case R_DE:
        outRegister(out, REG_DE);
        return OK;
    case R_HL:
        outRegister(out, REG_HL);
        return OK;
    case R_SP:
        outRegister(out, REG_SP);
        return OK;
    case R_AF:
        outRegister(out, REG_AF);
        return OK;
    case R_AFP:
        outRegister(out, REG_AFP);
        return OK;
    case R_IM:
        outRegister(out, REG_IM);
        return OK;
    case I_IXIY:
        outIndirectReg(out, RegZ80::decodeIndexReg(insn));
        return OK;
    case I_C:
        outIndirectReg(out, REG_C);
        return OK;
    case I_HL:
        outIndirectReg(out, REG_HL);
        return OK;
    case I_SP:
        outIndirectReg(out, REG_SP);
        return OK;
    default:
        return OK;
    }
    return setError(insn);
}

Error DisZ80::decode(DisMemory &memory, Insn &_insn, char *out) {
    InsnZ80 insn(_insn);
    Config::opcode_t opCode = insn.readByte(memory);
    insn.setOpCode(opCode);
    if (TableZ80.isPrefix(opCode)) {
        const Config::opcode_t prefix = opCode;
        opCode = insn.readByte(memory);
        insn.setOpCode(opCode, prefix);
    }
    if (setError(insn))
        return getError();

    if (TableZ80.searchOpCode(insn))
        return setError(TableZ80.getError());

    const AddrMode dst = insn.dstMode();
    if (dst == M_UNKI)
        return setError(UNKNOWN_INSTRUCTION);
    if (dst == M_NO)
        return OK;
    if (dst == T_IXB)
        return decodeIndexedBitOp(memory, insn, out);
    if (decodeOperand(memory, insn, out, dst))
        return getError();
    const AddrMode src = insn.srcMode();
    if (src == M_NO)
        return OK;
    out += strlen(out);
    *out++ = ',';
    return decodeOperand(memory, insn, out, src);
}

}  // namespace z80
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

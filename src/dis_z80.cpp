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

#include "reg_z80.h"
#include "table_z80.h"

namespace libasm {
namespace z80 {

using namespace reg;

const ValueFormatter::Plugins &DisZ80::defaultPlugins() {
    return ValueFormatter::Plugins::intel();
}

DisZ80::DisZ80(const ValueFormatter::Plugins &plugins) : Disassembler(plugins), Config(TABLE) {
    reset();
}

StrBuffer &DisZ80::outIndirectAddr(StrBuffer &out, uint16_t addr, uint8_t bits) {
    return outHex(out.letter('('), addr, bits).letter(')');
}

StrBuffer &DisZ80::outIndirectReg(StrBuffer &out, RegName reg) {
    return outRegName(out.letter('('), reg).letter(')');
}

StrBuffer &DisZ80::outIndexOffset(StrBuffer &out, RegName reg, int8_t offset) {
    outRegName(out.letter('('), reg);
    if (offset >= 0)
        out.letter('+');
    return outDec(out, offset, -8).letter(')');
}

StrBuffer &DisZ80::outDataReg(StrBuffer &out, RegName reg) {
    if (reg == REG_HL)
        return outIndirectReg(out, reg);
    return outRegName(out, reg);
}

Error DisZ80::decodeIndexedBitOp(DisInsn &insn, StrBuffer &out) {
    const int8_t offset = insn.readByte();
    const Config::opcode_t opc = insn.readByte();

    DisInsn ixBit(insn);  // |ixBit| will share internal implementation with |insn|
    ixBit.setOpCode(opc, insn.opCode());
    ixBit.nameBuffer().reset();
    if (TABLE.searchOpCode(cpuType(), ixBit, out))
        return setError(ixBit);

    const auto reg = decodeDataReg(opc);
    if (reg != REG_HL)
        return setError(UNKNOWN_INSTRUCTION);
    const auto dst = ixBit.dst();
    if (dst == M_BIT)
        outHex(out, (opc >> 3) & 7, 3).comma();
    outIndexOffset(out, decodeIndexReg(insn), offset);
    return setError(insn);
}

Error DisZ80::decodeRelative(DisInsn &insn, StrBuffer &out) {
    const auto delta = static_cast<int8_t>(insn.readByte());
    const auto base = insn.address() + insn.length();
    const auto target = branchTarget(base, delta);
    outRelAddr(out, target, insn.address(), 8);
    return setError(insn);
}

Error DisZ80::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode) {
    Config::opcode_t opc = insn.opCode();
    switch (mode) {
    case M_IM8:
        outHex(out, insn.readByte(), 8);
        break;
    case M_IM16:
        outHex(out, insn.readUint16(), 16);
        break;
    case M_ABS:
        outIndirectAddr(out, insn.readUint16(), 16);
        break;
    case M_IOA:
        outIndirectAddr(out, insn.readByte(), 8);
        break;
    case M_INDX:
        outIndexOffset(out, decodeIndexReg(insn), insn.readByte());
        break;
    case M_CC4:
        outCcName(out, decodeCcName((opc >> 3) & 3));
        return OK;
    case M_CC8:
        outCcName(out, decodeCcName(opc >> 3));
        return OK;
    case M_REL:
        return decodeRelative(insn, out);
    case M_PTR:
    case M_PIX:
        outRegName(out, decodePointerReg(opc >> 4, insn));
        return OK;
    case M_STK:
        outRegName(out, decodeStackReg(opc >> 4));
        return OK;
    case I_BCDE:
        outIndirectReg(out, decodeIndirectBase(opc >> 4));
        return OK;
    case M_REG:
        outDataReg(out, decodeDataReg(opc));
        return OK;
    case M_DST:
        outDataReg(out, decodeDataReg(opc >> 3));
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
        out.letter(opc + '0');
        return OK;
    case R_IR:
        outRegName(out, decodeIrReg(opc >> 3));
        return OK;
    case R_IXIY:
        outRegName(out, decodeIndexReg(insn));
        return OK;
    case R_A:
        outRegName(out, REG_A);
        return OK;
    case R_DE:
        outRegName(out, REG_DE);
        return OK;
    case R_HL:
        outRegName(out, REG_HL);
        return OK;
    case R_SP:
        outRegName(out, REG_SP);
        return OK;
    case R_AF:
        outRegName(out, REG_AF);
        return OK;
    case R_AFP:
        outRegName(out, REG_AFP);
        return OK;
    case R_IM:
        outRegName(out, REG_IM);
        return OK;
    case I_IXIY:
        outIndirectReg(out, decodeIndexReg(insn));
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

Error DisZ80::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) {
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

    const auto dst = insn.dst();
    if (dst == M_NONE)
        return OK;
    if (dst == T_IXB)
        return decodeIndexedBitOp(insn, out);
    if (decodeOperand(insn, out, dst))
        return getError();
    const auto src = insn.src();
    if (src == M_NONE)
        return OK;
    out.comma();
    return decodeOperand(insn, out, src);
}

}  // namespace z80
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

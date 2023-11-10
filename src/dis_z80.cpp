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

StrBuffer &DisZ80::outIndirectReg(StrBuffer &out, RegName reg) const {
    return outRegName(out.letter('('), reg).letter(')');
}

StrBuffer &DisZ80::outIndexOffset(StrBuffer &out, int8_t offset) const {
    if (offset >= 0)
        out.letter('+');
    return outDec(out, offset, -8);
}

StrBuffer &DisZ80::outDataReg(StrBuffer &out, RegName reg) const {
    if (reg == REG_HL)
        return outIndirectReg(out, reg);
    return outRegName(out, reg);
}

void DisZ80::decodeIndexedBitOp(DisInsn &insn, StrBuffer &out) const {
    const auto offset = static_cast<int8_t>(insn.readByte());
    const auto offsetError = insn.getError();
    const auto opc = insn.readByte();

    DisInsn ixBit(insn);  // |ixBit| will share internal implementation with |insn|
    ixBit.setOpCode(opc, insn.opCode());
    ixBit.nameBuffer().reset();
    if (TABLE.searchOpCode(cpuType(), ixBit, out)) {
        insn.nameBuffer().reset();
        insn.setErrorIf(out, UNKNOWN_INSTRUCTION);
        return;
    }

    if (decodeDataReg(opc) == REG_HL) {
        if (ixBit.dst() == M_BIT)
            outHex(out, (opc >> 3) & 7, 3).comma();
        outRegName(out.letter('('), decodeIndexReg(insn.prefix()));
        if (offsetError)
            insn.setErrorIf(out, offsetError);
        outIndexOffset(out, offset).letter(')');
    } else {
        insn.nameBuffer().reset();
        insn.setErrorIf(out, UNKNOWN_INSTRUCTION);
    }
}

void DisZ80::decodeRelative(DisInsn &insn, StrBuffer &out) const {
    const auto delta = static_cast<int8_t>(insn.readByte());
    const auto base = insn.address() + insn.length();
    const auto target = base + delta;
    insn.setErrorIf(out, checkAddr(target));
    outRelAddr(out, target, insn.address(), 8);
}

void DisZ80::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    auto opc = insn.opCode();
    switch (mode) {
    case M_IM8:
        outHex(out, insn.readByte(), 8);
        break;
    case M_IM16:
        outHex(out, insn.readUint16(), 16);
        break;
    case M_ABS:
        out.letter('(');
        outAbsAddr(out, insn.readUint16()).letter(')');
        break;
    case M_IOA:
        out.letter('(');
        outHex(out, insn.readByte(), 8).letter(')');
        break;
    case M_IDX:
        outRegName(out.letter('('), decodeIndexReg(insn.prefix()));
        outIndexOffset(out, insn.readByte()).letter(')');
        break;
    case M_CC4:
        outCcName(out, decodeCcName((opc >> 3) & 3));
        break;
    case M_CC8:
        outCcName(out, decodeCcName(opc >> 3));
        break;
    case M_REL:
        decodeRelative(insn, out);
        break;
    case M_R16:
    case M_NOHL:
    case M_R16X:
        outRegName(out, decodePointerReg(opc >> 4, insn.prefix()));
        break;
    case M_STK:
        outRegName(out, decodeStackReg(opc >> 4));
        break;
    case M_SRC:
    case M_SR8:
        outDataReg(out, decodeDataReg(opc));
        break;
    case M_DST:
    case M_DR8:
        outDataReg(out, decodeDataReg(opc >> 3));
        break;
    case M_VEC:
        outHex(out, opc & 0x38, 8);
        break;
    case M_BIT:
        outHex(out, (opc >> 3) & 7, 3);
        break;
    case M_IMMD:
        opc = (opc >> 3) & 3;
        if (opc)
            opc--;
        out.letter(opc + '0');
        break;
    case R_IDX:
        outRegName(out, decodeIndexReg(insn.prefix()));
        break;
    case R_A:
    case R_DE:
    case R_HL:
    case R_SP:
    case R_AF:
    case R_AFP:
    case R_I:
    case R_R:
    case R_IM:
        outRegName(out, RegName(mode - R_BASE));
        break;
    case I_PTR:
        outIndirectReg(out, decodeIndirectBase(opc >> 4));
        break;
    case I_IDX:
        outIndirectReg(out, decodeIndexReg(insn.prefix()));
        break;
    case I_HL:
    case I_SP:
    case I_C:
        outIndirectReg(out,RegName(mode - I_BASE));
        break;
    default:
        break;
    }
}

Error DisZ80::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) const {
    DisInsn insn(_insn, memory, out);
    const auto opc = insn.readByte();
    insn.setOpCode(opc);
    if (TABLE.isPrefix(cpuType(), opc)) {
        insn.setOpCode(insn.readByte(), opc);
        if (insn.getError())
            return _insn.setError(insn);
    }
    if (TABLE.searchOpCode(cpuType(), insn, out))
        return _insn.setError(insn);

    const auto dst = insn.dst();
    if (dst == T_IXB) {
        decodeIndexedBitOp(insn, out);
    } else if (dst != M_NONE) {
        decodeOperand(insn, out, dst);
        const auto src = insn.src();
        if (src != M_NONE)
            decodeOperand(insn, out.comma(), src);
    }
    return _insn.setError(insn);
}

}  // namespace z80
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

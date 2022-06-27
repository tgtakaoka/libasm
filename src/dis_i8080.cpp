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

#include "dis_i8080.h"

#include "table_i8080.h"

namespace libasm {
namespace i8080 {

StrBuffer &DisI8080::outRegister(StrBuffer &out, RegName regName) {
    return _regs.outRegName(out, regName);
}

Error DisI8080::decodeOperand(DisMemory &memory, InsnI8080 &insn, StrBuffer &out, AddrMode mode) {
    switch (mode) {
    case M_IM8:
    case M_IOA:
        outHex(out, insn.readByte(memory), 8);
        break;
    case M_IM16:
    case M_ABS:
        outHex(out, insn.readUint16(memory), 16);
        break;
    case M_PTR:
        outRegister(out, RegI8080::decodePointerReg(insn.opCode() >> 4));
        return OK;
    case M_STK:
        outRegister(out, RegI8080::decodeStackReg(insn.opCode() >> 4));
        return OK;
    case M_IDX:
        outRegister(out, RegI8080::decodeIndexReg(insn.opCode() >> 4));
        return OK;
    case M_REG:
        outRegister(out, RegI8080::decodeDataReg(insn.opCode()));
        return OK;
    case M_DST:
        outRegister(out, RegI8080::decodeDataReg(insn.opCode() >> 3));
        return OK;
    case M_VEC:
        outHex(out, (insn.opCode() >> 3) & 7, 3);
        return OK;
    default:
        return OK;
    }
    return setError(insn);
}

Error DisI8080::decode(DisMemory &memory, Insn &_insn, StrBuffer &out) {
    InsnI8080 insn(_insn);
    Config::opcode_t opCode = insn.readByte(memory);
    insn.setOpCode(opCode);
    if (TableI8080.isPrefix(opCode)) {
        const auto prefix = opCode;
        opCode = insn.readByte(memory);
        insn.setOpCode(opCode, prefix);
    }
    if (setError(insn))
        return getError();

    if (TableI8080.searchOpCode(insn))
        return setError(TableI8080.getError());

    const AddrMode dst = insn.dstMode();
    if (dst == M_NO)
        return OK;
    if (decodeOperand(memory, insn, out, dst))
        return getError();
    const AddrMode src = insn.srcMode();
    if (src == M_NO)
        return OK;
    out.comma();
    return decodeOperand(memory, insn, out, src);
}

}  // namespace i8080
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

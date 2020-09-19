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

char *DisI8080::outRegister(char *out, RegName regName) {
    return _regs.outRegName(out, regName);
}

Error DisI8080::decode(DisMemory &memory, Insn &_insn, char *out) {
    InsnI8080 insn(_insn);
    const Config::opcode_t opCode = insn.readByte(memory);
    if (setError(insn)) return getError();

    insn.setOpCode(opCode);
    if (TableI8080.searchOpCode(insn))
        return setError(TableI8080.getError());

    switch (insn.insnFormat()) {
    case POINTER_REG:
        out = outRegister(out, RegI8080::decodePointerReg(opCode >> 4));
        break;
    case STACK_REG:
        out = outRegister(out, RegI8080::decodeStackReg(opCode >> 4));
        break;
    case INDEX_REG:
        out = outRegister(out, RegI8080::decodeIndexReg(opCode >> 4));
        break;
    case DATA_REG:
        out = outRegister(out, RegI8080::decodeDataReg(opCode >> 3));
        break;
    case LOW_DATA_REG:
        out = outRegister(out, RegI8080::decodeDataReg(opCode));
        break;
    case DATA_DATA_REG:
        out = outRegister(out, RegI8080::decodeDataReg(opCode >> 3));
        *out++ = ',';
        out = outRegister(out, RegI8080::decodeDataReg(opCode));
        break;
    case VECTOR_NO:
        out = outHex(out, (opCode >> 3) & 7, 3);
        break;
    default:
        break;
    }

    switch (insn.addrMode()) {
    case IMM8:
        if (insn.insnFormat() != NO_FORMAT) *out++ = ',';
        outHex(out, insn.readByte(memory), 8);
        break;
    case IMM16:
        if (insn.insnFormat() != NO_FORMAT) *out++ = ',';
        outHex(out, insn.readUint16(memory), 16);
        break;
    case DIRECT:
        outAbsAddr(out, insn.readUint16(memory));
        break;
    case IOADR:
        outAbsAddr(out, insn.readByte(memory));
        break;
    default:
        break;
    }
    return setError(insn);
}

} // namespace i8080
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

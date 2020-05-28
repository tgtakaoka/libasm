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

void DisI8080::outRegister(RegName regName) {
    _operands = _regs.outRegName(_operands, regName);
}

Error DisI8080::decodeImmediate8(
    DisMemory& memory, InsnI8080 &insn) {
    uint8_t val;
    if (insn.readByte(memory, val)) return setError(NO_MEMORY);
    if (insn.insnFormat() != NO_FORMAT) *_operands++ = ',';
    outConstant(val);
    return setOK();
}

Error DisI8080::decodeImmediate16(
    DisMemory& memory, InsnI8080 &insn) {
    uint16_t val;
    if (insn.readUint16(memory, val)) return setError(NO_MEMORY);
    if (insn.insnFormat() != NO_FORMAT) *_operands++ = ',';
    outConstant(val);
    return setOK();
}

Error DisI8080::decodeDirect(
    DisMemory &memory, InsnI8080& insn) {
    Config::uintptr_t addr;
    if (insn.readUint16(memory, addr)) return setError(NO_MEMORY);
    outConstant(addr, 16, false);
    return setOK();
}

Error DisI8080::decodeIoaddr(
    DisMemory &memory, InsnI8080& insn) {
    uint8_t ioaddr;
    if (insn.readByte(memory, ioaddr)) return setError(NO_MEMORY);
    outConstant(ioaddr, 16, false);
    return setOK();
}

Error DisI8080::decode(
    DisMemory &memory, Insn &_insn) {
    InsnI8080 insn(_insn);
    Config::opcode_t opCode;
    if (insn.readByte(memory, opCode)) return setError(NO_MEMORY);
    insn.setOpCode(opCode);
    if (TableI8080.searchOpCode(insn))
        return setError(UNKNOWN_INSTRUCTION);

    switch (insn.insnFormat()) {
    case POINTER_REG:
        outRegister(RegI8080::decodePointerReg((opCode >> 4) & 3));
        break;
    case STACK_REG:
        outRegister(RegI8080::decodeStackReg((opCode >> 4) & 3));
        break;
    case INDEX_REG:
        outRegister(RegI8080::decodeIndexReg((opCode >> 4) & 1));
        break;
    case DATA_REG:
        outRegister(RegI8080::decodeDataReg((opCode >> 3) & 7));
        break;
    case LOW_DATA_REG:
        outRegister(RegI8080::decodeDataReg(opCode & 7));
        break;
    case DATA_DATA_REG:
        outRegister(RegI8080::decodeDataReg((opCode >> 3) & 7));
        *_operands++ = ',';
        outRegister(RegI8080::decodeDataReg(opCode & 7));
        break;
    case VECTOR_NO:
        outConstant(static_cast<uint8_t>((opCode >> 3) & 7));
        break;
    default:
        break;
    }

    switch (insn.addrMode()) {
    case INHR:
        return setOK();
    case IMM8:
        return decodeImmediate8(memory, insn);
    case IMM16:
        return decodeImmediate16(memory, insn);
    case DIRECT:
        return decodeDirect(memory, insn);
    case IOADR:
        return decodeIoaddr(memory, insn);
    default:
        return setError(INTERNAL_ERROR);
    }
}

} // namespace i8080
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

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
    DisMemory<Config>& memory, InsnI8080 &insn) {
    uint8_t val;
    if (insn.readByte(memory, val)) return setError(NO_MEMORY);
    if (insn.insnFormat() != NO_FORMAT) *_operands++ = ',';
    outConstant(val);
    return setError(OK);
}

Error DisI8080::decodeImmediate16(
    DisMemory<Config>& memory, InsnI8080 &insn) {
    uint16_t val;
    if (insn.readUint16(memory, val)) return setError(NO_MEMORY);
    if (insn.insnFormat() != NO_FORMAT) *_operands++ = ',';
    const char *label = lookup(val);
    if (label) {
        outText(label);
    } else {
        outConstant(val);
    }
    return setError(OK);
}

Error DisI8080::decodeDirect(
    DisMemory<Config> &memory, InsnI8080& insn) {
    Config::uintptr_t addr;
    if (insn.readUint16(memory, addr)) return setError(NO_MEMORY);
    const char *label = lookup(addr);
    if (label) {
        outText(label);
    } else {
        outConstant(addr, 16, false);
    }
    return setError(OK);
}

Error DisI8080::decodeIoaddr(
    DisMemory<Config> &memory, InsnI8080& insn) {
    uint8_t ioaddr;
    if (insn.readByte(memory, ioaddr)) return setError(NO_MEMORY);
    outConstant(ioaddr, 16, false);
    return setError(OK);
}

Error DisI8080::decode(
    DisMemory<Config> &memory, Insn &_insn) {
    InsnI8080 insn(_insn);
    Config::insn_t insnCode;
    if (insn.readByte(memory, insnCode)) return setError(NO_MEMORY);
    insn.setInsnCode(insnCode);
    if (TableI8080.searchInsnCode(insn))
        return setError(UNKNOWN_INSTRUCTION);

    switch (insn.insnFormat()) {
    case POINTER_REG:
        outRegister(RegI8080::decodePointerReg((insnCode >> 4) & 3));
        break;
    case STACK_REG:
        outRegister(RegI8080::decodeStackReg((insnCode >> 4) & 3));
        break;
    case INDEX_REG:
        outRegister(RegI8080::decodeIndexReg((insnCode >> 4) & 1));
        break;
    case DATA_REG:
        outRegister(RegI8080::decodeDataReg((insnCode >> 3) & 7));
        break;
    case LOW_DATA_REG:
        outRegister(RegI8080::decodeDataReg(insnCode & 7));
        break;
    case DATA_DATA_REG:
        outRegister(RegI8080::decodeDataReg((insnCode >> 3) & 7));
        *_operands++ = ',';
        outRegister(RegI8080::decodeDataReg(insnCode & 7));
        break;
    case VECTOR_NO:
        outConstant((insnCode >> 3) & 7);
        break;
    default:
        break;
    }

    switch (insn.addrMode()) {
    case INHR:
        return setError(OK);
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

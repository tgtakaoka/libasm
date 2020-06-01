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

#include "dis_tms9900.h"
#include "table_tms9900.h"

namespace libasm {
namespace tms9900 {

Error DisTms9900::decodeOperand(
    DisMemory &memory, InsnTms9900 &insn, const uint8_t opr) {
    const uint8_t regno = opr & 0xf;
    const uint8_t mode = (opr >> 4) & 0x3;
    if (mode == 1 || mode == 3) *_operands++ = '*';
    if (mode == 2) {
        uint16_t val;
        if (insn.readUint16(memory, val)) return setError(NO_MEMORY);
        *_operands++ = '@';
        outConstant(val, 16);
        if (regno) {
            *_operands++ = '(';
            _operands = _regs.outRegName(_operands, regno);
            *_operands++ = ')';
        }
    } else {
        _operands = _regs.outRegName(_operands, regno);
        if (mode == 3)
            *_operands++ = '+';
    }
    *_operands = 0;
    return setOK();
}

Error DisTms9900::decodeImmediate(
    DisMemory& memory, InsnTms9900 &insn) {
    uint16_t val;
    if (insn.readUint16(memory, val)) return setError(NO_MEMORY);
    outConstant(val, 16);
    return setOK();
}

Error DisTms9900::decodeRelative(InsnTms9900 &insn) {
    int16_t delta = static_cast<int8_t>(insn.opCode() & 0xff);
    delta <<= 1;
    const Config::uintptr_t addr = insn.address() + 2 + delta;
    outConstant(addr, 16, false);
    return setOK();
}

Error DisTms9900::decode(
    DisMemory &memory, Insn &_insn) {
    InsnTms9900 insn(_insn);
    Config::opcode_t opCode;
    if (insn.readUint16(memory, opCode)) return setError(NO_MEMORY);
    insn.setOpCode(opCode);
    if (TableTms9900.searchOpCode(insn)) {
        insn.setName("MID");
        return setError(UNKNOWN_INSTRUCTION);
    }

    switch (insn.addrMode()) {
    case INH:
        return setOK();
    case IMM:
        return decodeImmediate(memory, insn);
    case REG:
        _operands = _regs.outRegName(_operands, opCode);
        return setOK();
    case REG_IMM:
        _operands = _regs.outRegName(_operands, opCode);
        *_operands++ = ',';
        return decodeImmediate(memory, insn);
    case CNT_REG: {
        _operands = _regs.outRegName(_operands, opCode);
        *_operands++ = ',';
        const uint8_t count = (opCode >> 4) & 0x0f;
        if (count == 0) _operands = _regs.outRegName(_operands, 0);
        else outConstant(static_cast<uint8_t>(count), 10);
        return setOK();
    }
    case SRC:
        return decodeOperand(memory, insn, opCode);
    case REG_SRC:
        if (decodeOperand(memory, insn, opCode))
            return getError();
        *_operands++ = ',';
        _operands = _regs.outRegName(_operands, opCode >> 6);
        return setOK();
    case CNT_SRC:
    case XOP_SRC: {
        if (decodeOperand(memory, insn, opCode))
            return getError();
        *_operands++ = ',';
        uint8_t count = (opCode >> 6) & 0xf;
        if (insn.addrMode() == CNT_SRC && count == 0)
            count = 16;
        outConstant(static_cast<uint8_t>(count), 10);
        return setOK();
    }
    case DST_SRC: {
        if (decodeOperand(memory, insn, opCode))
            return getError();
        *_operands++ = ',';
        decodeOperand(memory, insn, opCode >> 6);
        return getError();
    }
    case REL:
        decodeRelative(insn);
        return setOK();
    case CRU_OFF: {
        const int8_t offset = static_cast<int8_t>(opCode & 0xff);
        outConstant(offset, 10);
        return setOK();
    }
    default:
        return setError(INTERNAL_ERROR);
    }
}

} // namespace tms9900
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

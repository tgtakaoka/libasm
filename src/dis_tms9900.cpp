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
        outConstant(val);
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
    const Config::uintptr_t target = insn.address() + 2 + delta;
    outRelativeAddr(target, insn.address(), 9);
    return setOK();
}

Error DisTms9900::decode(
    DisMemory &memory, Insn &_insn) {
    InsnTms9900 insn(_insn);
    Config::opcode_t opCode;
    if (insn.readUint16(memory, opCode)) return setError(NO_MEMORY);
    insn.setOpCode(opCode);
    if (TableTms9900.searchOpCode(insn)) {
    mid:
        insn.setName("MID");
        return setError(UNKNOWN_INSTRUCTION);
    }

    uint8_t count;
    switch (insn.addrMode()) {
    case IMM_MOD:
        count = insn.opCode() & 7;
        if (count == 1 || count == 2 || count == 4)
            outConstant(count, 10);
        return setOK();
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
    case CNT_REG:
        _operands = _regs.outRegName(_operands, opCode);
        *_operands++ = ',';
        count = (opCode >> 4) & 0x0f;
        if (count == 0) _operands = _regs.outRegName(_operands, 0);
        else outConstant(count, 10);
        return setOK();
    case DW_BIT_SRC:
        if (insn.readUint16(memory, opCode))
            return setError(NO_MEMORY);
        count = (opCode >> 6) & 0x0f;
        if ((opCode & 0xFC00) != 0x0000) goto mid;
        if ((opCode & 0x0030) == 0x0030) goto mid; // no auto increment mode
        if (decodeOperand(memory, insn, opCode)) return getError();
        *_operands++ = ',';
        outConstant(count, 10);
        return setOK();
    case SRC:
        return decodeOperand(memory, insn, opCode);
    case REG_SRC:
        if (decodeOperand(memory, insn, opCode))
            return getError();
        *_operands++ = ',';
        _operands = _regs.outRegName(_operands, opCode >> 6);
        return setOK();
    case DW_CNT_SRC:
        if (insn.readUint16(memory, opCode))
            return setError(NO_MEMORY);
        if ((opCode & 0xFC00) != 0x4000) goto mid;
        if (decodeOperand(memory, insn, opCode))
            return getError();
        *_operands++ = ',';
        count = (opCode >> 6) & 0xf;
        if (count == 0) _operands = _regs.outRegName(_operands, 0);
        else outConstant(count, 10);
        return setOK();
    case CNT_SRC:
    case XOP_SRC:
        if (decodeOperand(memory, insn, opCode))
            return getError();
        *_operands++ = ',';
        count = (opCode >> 6) & 0xf;
        if (insn.addrMode() == CNT_SRC && count == 0)
            count = 16;
        outConstant(count, 10);
        return setOK();
    case DW_DST_SRC:
        if (insn.readUint16(memory, opCode))
            return setError(NO_MEMORY);
        if ((opCode & 0xF000) != 0x4000) goto mid;
        /* Fall-through */
    case DST_SRC:
        if (decodeOperand(memory, insn, opCode))
            return getError();
        *_operands++ = ',';
        decodeOperand(memory, insn, opCode >> 6);
        return getError();
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

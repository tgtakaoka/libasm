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

void DisTms9900::outAddress(target::uintptr_t addr, bool relax) {
    const char *label = lookup(addr);
    if (label) {
        outText(label);
    } else {
        outConstant(addr, 16, relax);
    }
}

Error DisTms9900::decodeOperand(
    DisMemory<target::uintptr_t> &memory, Insn &insn, const host::uint_t opr) {
    const host::uint_t regno = opr & 0xf;
    const host::uint_t mode = (opr >> 4) & 0x3;
    if (mode == 1 || mode == 3) *_operands++ = '*';
    if (mode == 2) {
        uint16_t val;
        if (insn.readUint16(memory, val)) return setError(NO_MEMORY);
        *_operands++ = '@';
        outAddress(val);
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
    return setError(OK);
}

Error DisTms9900::decodeImmediate(
    DisMemory<target::uintptr_t>& memory, Insn &insn) {
    uint16_t val;
    if (insn.readUint16(memory, val)) return setError(NO_MEMORY);
    outAddress(val);
    return setError(OK);
}

Error DisTms9900::decodeRelative(Insn& insn) {
    int16_t delta = static_cast<int8_t>(insn.insnCode() & 0xff);
    delta <<= 1;
    const target::uintptr_t addr = insn.address() + 2 + delta;
    outAddress(addr, false);
    return setError(OK);
}

Error DisTms9900::decode(
    DisMemory<target::uintptr_t> &memory, Insn &insn) {
    target::insn_t insnCode;
    if (insn.readUint16(memory, insnCode)) return setError(NO_MEMORY);
    insn.setInsnCode(insnCode);
    if (TableTms9900.searchInsnCode(insn)) {
        insn.setName("MID");
        return setError(UNKNOWN_INSTRUCTION);
    }

    switch (insn.addrMode()) {
    case INH:
        return setError(OK);
    case IMM:
        return decodeImmediate(memory, insn);
    case REG:
        _operands = _regs.outRegName(_operands, insnCode);
        return setError(OK);
    case REG_IMM:
        _operands = _regs.outRegName(_operands, insnCode);
        *_operands++ = ',';
        return decodeImmediate(memory, insn);
    case CNT_REG: {
        _operands = _regs.outRegName(_operands, insnCode);
        *_operands++ = ',';
        const host::uint_t count = (insnCode >> 4) & 0x0f;
        if (count == 0) _operands = _regs.outRegName(_operands, 0);
        else outConstant(static_cast<uint8_t>(count), 10);
        return setError(OK);
    }
    case SRC:
        return decodeOperand(memory, insn, insnCode);
    case REG_SRC:
        if (decodeOperand(memory, insn, insnCode))
            return getError();
        *_operands++ = ',';
        _operands = _regs.outRegName(_operands, insnCode >> 6);
        return setError(OK);
    case CNT_SRC:
    case XOP_SRC: {
        if (decodeOperand(memory, insn, insnCode))
            return getError();
        *_operands++ = ',';
        host::uint_t count = (insnCode >> 6) & 0xf;
        if (insn.addrMode() == CNT_SRC && count == 0)
            count = 16;
        const char *label = lookup(count);
        if (label) {
            outText(label);
        } else {
            outConstant(static_cast<uint8_t>(count), 10);
        }
        return setError(OK);
    }
    case DST_SRC: {
        if (decodeOperand(memory, insn, insnCode))
            return getError();
        *_operands++ = ',';
        decodeOperand(memory, insn, insnCode >> 6);
        return getError();
    }
    case REL:
        decodeRelative(insn);
        return setError(OK);
    case CRU_OFF: {
        const int8_t offset = static_cast<int8_t>(insnCode & 0xff);
        const char *label = lookup(offset);
        if (label) {
            outText(label);
        } else {
            outConstant(offset, 10);
        }
        return setError(OK);
    }
    default:
        return setError(INTERNAL_ERROR);
    }
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

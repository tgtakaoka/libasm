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

#include "dis_m6502.h"
#include "table_m6502.h"

Error DisM6502::decodeImmediate(
    DisMemory<target::uintptr_t>& memory, Insn &insn) {
    uint8_t val;
    if (insn.readByte(memory, val)) return setError(NO_MEMORY);
    *_operands++ = '#';
    const char *label = lookup(val);
    if (label) {
        outText(label);
    } else {
        outConstant(val);
    }
    return setError(OK);
}

Error DisM6502::decodeAbsolute(
    DisMemory<target::uintptr_t>& memory, Insn &insn) {
    const bool indirect = (insn.addrMode() == IDX_ABS_IND)
        || (insn.addrMode() == ABS_INDIRECT);
    RegName index;
    switch (insn.addrMode()) {
    case ABS_IDX_X:
    case IDX_ABS_IND:
        index = REG_X;
        break;
    case ABS_IDX_Y:
        index = REG_Y;
        break;
    default:
        index = REG_UNDEF;
        break;
    }
    target::uintptr_t addr;
    if (insn.readUint16(memory, addr)) return setError(NO_MEMORY);
    if (indirect) *_operands++ = '(';
    const char *label = lookup(addr);
    if (label) {
        *_operands++ = '>';
        outText(label);
    } else {
        if (addr < 0x100) *_operands++ = '>';
        outConstant(addr, 16, false);
    }
    if (index != REG_UNDEF) {
        *_operands++ = ',';
        _operands = _regs.outRegName(_operands, index);
    }
    if (indirect) *_operands++ = ')';
    *_operands = 0;
    return setError(OK);
}

Error DisM6502::decodeZeroPage(
    DisMemory<target::uintptr_t> &memory, Insn& insn) {
    const bool indirect = insn.addrMode() == INDX_IND
        || insn.addrMode() == INDIRECT_IDX
        || insn.addrMode() == ZP_INDIRECT;
    RegName index;
    switch (insn.addrMode()) {
    case ZP_IDX_X:
    case INDX_IND:
        index = REG_X;
        break;
    case ZP_IDX_Y:
    case INDIRECT_IDX:
        index = REG_Y;
        break;
    default:
        index = REG_UNDEF;
        break;
    }
    uint8_t zp;
    if (insn.readByte(memory, zp)) return setError(NO_MEMORY);
    if (indirect) *_operands++ = '(';
    const char *label = lookup(zp);
    if (label) {
        *_operands++ = '<';
        outText(label);
    } else {
        outConstant(zp, 16, false);
    }
    if (indirect && index == REG_Y) *_operands++ = ')';
    if (index != REG_UNDEF) {
        *_operands++ = ',';
        _operands = _regs.outRegName(_operands, index);
    }
    if (indirect && index != REG_Y) *_operands++ = ')';
    *_operands = 0;
#ifdef W65C02_ENABLE_BITOPS
    if (insn.addrMode() == ZP_REL8) {
        *_operands++ = ',';
        return decodeRelative(memory, insn);
    }
#endif
    return setError(OK);
}

Error DisM6502::decodeRelative(
    DisMemory<target::uintptr_t> &memory, Insn &insn) {
    target::ptrdiff_t delta;
    uint8_t val;
    if (insn.readByte(memory, val)) return setError(NO_MEMORY);
    delta = static_cast<int8_t>(val);
#ifdef W65C02_ENABLE_BITOPS
    const host::uint_t insnLen = (insn.addrMode() == ZP_REL8 ? 3 : 2);
#else
    const host::uint_t insnLen = 2;
#endif
    const target::uintptr_t addr = insn.address() + insnLen + delta;
    const char *label = lookup(addr);
    if (label) {
        outText(label);
    } else {
        outConstant(addr, 16, false);
    }
    return setError(OK);
}

Error DisM6502::decode(
    DisMemory<target::uintptr_t> &memory, Insn &insn) {
    target::insn_t insnCode;
    if (insn.readByte(memory, insnCode)) return setError(NO_MEMORY);
    insn.setInsnCode(insnCode);

    if (TableM6502.searchInsnCode(insn))
        return setError(UNKNOWN_INSTRUCTION);

    if (insn.is65c02() && !TableM6502.is65c02())
        return setError(UNKNOWN_INSTRUCTION);

    switch (insn.addrMode()) {
    case IMPLIED:
        return setError(OK);
    case ACCUMULATOR:
        _operands = _regs.outRegName(_operands, REG_A);
        *_operands = 0;
        return setError(OK);
    case IMMEDIATE:
        return decodeImmediate(memory, insn);
    case ABSOLUTE:
    case ABS_IDX_X:
    case ABS_IDX_Y:
    case ABS_INDIRECT:
    case IDX_ABS_IND:
        return decodeAbsolute(memory, insn);
    case ZEROPAGE:
    case ZP_IDX_X:
    case ZP_IDX_Y:
    case INDX_IND:
    case INDIRECT_IDX:
    case ZP_INDIRECT:
#ifdef W65C02_ENABLE_BITOPS
    case ZP_REL8:
#endif
        return decodeZeroPage(memory, insn);
    case REL8:
        return decodeRelative(memory, insn);
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

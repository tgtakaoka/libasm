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
    const AddrMode addrMode = insn.addrMode();
    const bool indirect = addrMode == ABS_IDX_IDIR
        || addrMode == ABS_IDIR;
    RegName index;
    switch (addrMode) {
    case ABS_IDX:
    case ABS_IDX_IDIR:
        index = REG_X;
        break;
    case ABS_IDY:
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
    const AddrMode addrMode = insn.addrMode();
    const bool indirect = addrMode == ZPG_IDX_IDIR
        || addrMode == ZPG_IDIR_IDY
        || addrMode == ZPG_IDIR;
    RegName index;
    switch (addrMode) {
    case ZPG_IDX:
    case ZPG_IDX_IDIR:
        index = REG_X;
        break;
    case ZPG_IDY:
    case ZPG_IDIR_IDY:
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
    if (addrMode == ZPG_REL) {
        *_operands++ = ',';
        return decodeRelative(memory, insn);
    }
    return setError(OK);
}

Error DisM6502::decodeRelative(
    DisMemory<target::uintptr_t> &memory, Insn &insn) {
    uint8_t val;
    if (insn.readByte(memory, val)) return setError(NO_MEMORY);
    const host::uint_t insnLen = (insn.addrMode() == ZPG_REL ? 3 : 2);
    const target::uintptr_t addr = insn.address() + insnLen
        + static_cast<int8_t>(val);
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

    switch (insn.addrMode()) {
    case IMPL:
        return setError(OK);
    case ACCM:
        _operands = _regs.outRegName(_operands, REG_A);
        *_operands = 0;
        return setError(OK);
    case IMM:
        return decodeImmediate(memory, insn);
    case ABS:
    case ABS_IDX:
    case ABS_IDY:
    case ABS_IDIR:
    case ABS_IDX_IDIR:
        return decodeAbsolute(memory, insn);
    case ZPG:
    case ZPG_IDX:
    case ZPG_IDY:
    case ZPG_IDX_IDIR:
    case ZPG_IDIR_IDY:
    case ZPG_IDIR:
    case ZPG_REL:
        return decodeZeroPage(memory, insn);
    case REL:
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

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

namespace libasm {
namespace m6502 {

Error DisM6502::decodeImmediate(
    DisMemory<Config::uintptr_t>& memory, InsnM6502 &insn) {
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
    DisMemory<Config::uintptr_t>& memory, InsnM6502 &insn) {
    const AddrMode addrMode = insn.addrMode();
    const bool indirect = addrMode == ABS_IDX_IDIR
        || addrMode == ABS_IDIR
        || addrMode == ABS_IDIR_LONG;
    const bool absLong =
        (addrMode == ABS_LONG || addrMode == ABS_LONG_IDX);
    const bool idirLong = addrMode == ABS_IDIR_LONG;
    RegName index;
    switch (addrMode) {
    case ABS_IDX:
    case ABS_IDX_IDIR:
    case ABS_LONG_IDX:
        index = REG_X;
        break;
    case ABS_IDY:
        index = REG_Y;
        break;
    default:
        index = REG_UNDEF;
        break;
    }
    Config::uintptr_t addr;
    uint32_t target;
    if (insn.readUint16(memory, addr)) return setError(NO_MEMORY);
    target = addr;
    if (absLong) {
        uint8_t bank;
        if (insn.readByte(memory, bank)) return setError(NO_MEMORY);
        target |= static_cast<uint32_t>(bank) << 16;
    }
    if (indirect) *_operands++ = idirLong ? '[' : '(';
    const char *label = lookup(target);
    if (label) {
        *_operands++ = '>';
        if (absLong) *_operands++ = '>';
        outText(label);
    } else if (!absLong) {
        if (target < 0x100) *_operands++ = '>';
        outConstant(addr, 16, false);
    } else {
        if (target < 0x10000) {
            *_operands++ = '>';
            *_operands++ = '>';
        }
        outConstant(target, 16);
    }
    if (index != REG_UNDEF) {
        *_operands++ = ',';
        _operands = _regs.outRegName(_operands, index);
    }
    if (indirect) *_operands++ = idirLong ? ']' : ')';
    *_operands = 0;
    return setError(OK);
}

Error DisM6502::decodeZeroPage(
    DisMemory<Config::uintptr_t> &memory, InsnM6502 &insn) {
    const AddrMode addrMode = insn.addrMode();
    const bool indirect = addrMode == ZPG_IDX_IDIR
        || addrMode == ZPG_IDIR_IDY
        || addrMode == ZPG_IDIR
        || addrMode == SP_REL_IDIR_IDY
        || addrMode == ZPG_IDIR_LONG
        || addrMode == ZPG_IDIR_LONG_IDY;
    const bool zpLong = addrMode == ZPG_IDIR_LONG
        || addrMode == ZPG_IDIR_LONG_IDY;
    RegName index;
    switch (addrMode) {
    case ZPG_IDX:
    case ZPG_IDX_IDIR:
        index = REG_X;
        break;
    case ZPG_IDY:
    case ZPG_IDIR_IDY:
    case ZPG_IDIR_LONG_IDY:
        index = REG_Y;
        break;
    case SP_REL:
    case SP_REL_IDIR_IDY:
        index = REG_S;
        break;
    default:
        index = REG_UNDEF;
        break;
    }
    uint8_t zp;
    if (insn.readByte(memory, zp)) return setError(NO_MEMORY);
    if (indirect) *_operands++ = zpLong ? '[' : '(';
    const char *label = lookup(zp);
    if (label) {
        *_operands++ = '<';
        outText(label);
    } else {
        outConstant(zp, 16, false);
    }
    if (indirect && index == REG_Y) *_operands++ = zpLong ? ']' : ')';
    if (index != REG_UNDEF) {
        *_operands++ = ',';
        _operands = _regs.outRegName(_operands, index);
    }
    if (indirect && index != REG_Y) *_operands++ = zpLong ? ']' : ')';
    if (addrMode == SP_REL_IDIR_IDY) {
        *_operands++ = ',';
        _operands = _regs.outRegName(_operands, REG_Y);
    }
    *_operands = 0;
    if (addrMode == ZPG_REL) {
        *_operands++ = ',';
        return decodeRelative(memory, insn);
    }
    return setError(OK);
}

Error DisM6502::decodeRelative(
    DisMemory<Config::uintptr_t> &memory, InsnM6502 &insn) {
    Config::uintptr_t addr;
    if (insn.addrMode() == REL_LONG) {
        uint16_t val;
        if (insn.readUint16(memory, val)) return setError(NO_MEMORY);
        addr = insn.address() + 3 + static_cast<int16_t>(val);
    } else {
        uint8_t val;
        if (insn.readByte(memory, val)) return setError(NO_MEMORY);
        addr = insn.address() + (insn.addrMode() == ZPG_REL ? 3 : 2)
            + static_cast<int8_t>(val);
    }
    const char *label = lookup(addr);
    if (label) {
        outText(label);
    } else {
        outConstant(addr, 16, false);
    }
    return setError(OK);
}

Error DisM6502::decodeBlockMove(
    DisMemory<Config::uintptr_t> &memory, InsnM6502 &insn) {
    uint8_t srcpg, dstpg;
    if (insn.readByte(memory, srcpg)) return setError(NO_MEMORY);
    if (insn.readByte(memory, dstpg)) return setError(NO_MEMORY);
    outConstant(srcpg, 16, false);
    *_operands++ = ',';
    outConstant(dstpg, 16, false);
    return setError(OK);
}

Error DisM6502::decode(
    DisMemory<Config::uintptr_t> &memory, Insn<Config::uintptr_t> &_insn) {
    InsnM6502 insn(_insn);
    Config::insn_t insnCode;
    if (insn.readByte(memory, insnCode)) return setError(NO_MEMORY);
    insn.setInsnCode(insnCode);

    if (TableM6502.searchInsnCode(insn, _acceptIndirectLong))
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
    case ABS_LONG:
    case ABS_LONG_IDX:
    case ABS_IDIR_LONG:
        return decodeAbsolute(memory, insn);
    case ZPG:
    case ZPG_IDX:
    case ZPG_IDY:
    case ZPG_IDX_IDIR:
    case ZPG_IDIR_IDY:
    case ZPG_IDIR:
    case ZPG_REL:
    case SP_REL:
    case SP_REL_IDIR_IDY:
    case ZPG_IDIR_LONG:
    case ZPG_IDIR_LONG_IDY:
        return decodeZeroPage(memory, insn);
    case REL:
    case REL_LONG:
        return decodeRelative(memory, insn);
    case BLOCK_MOVE:
        return decodeBlockMove(memory, insn);
    default:
        return setError(INTERNAL_ERROR);
    }
}

} // namespace m6502
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

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

#include "dis_w65c816.h"
#include "table_w65c816.h"

namespace libasm {
namespace w65c816 {

using mos6502::RegName;
using mos6502::RegName::REG_UNDEF;
using mos6502::RegName::REG_A;
using mos6502::RegName::REG_X;
using mos6502::RegName::REG_Y;
#define REG_S mos6502::RegName('S')

Error DisW65C816::decodeImmediate(
    DisMemory& memory, InsnW65C816 &insn) {
    const bool imm16 = (insn.addrMode() == IMMA && _long_acc)
        || (insn.addrMode() == IMMX && _long_idx);
    uint16_t val;
    if (imm16) {
        if (insn.readUint16(memory, val)) return setError(NO_MEMORY);
    } else {
        uint8_t val8;
        if (insn.readByte(memory, val8)) return setError(NO_MEMORY);
        val = val8;
    }
    *_operands++ = '#';
    const char *label = lookup(val);
    if (label) {
        outText(label);
    } else if (imm16) {
        outConstant(val);
    } else {
        outConstant(static_cast<uint8_t>(val));
    }
    if (insn.opCode() == TableW65C816::WDM)
        return setError(UNKNOWN_INSTRUCTION);
    return setOK();
}

Error DisW65C816::decodeAbsolute(
    DisMemory& memory, InsnW65C816 &insn) {
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
    uint16_t addr;
    Config::uintptr_t target;
    if (insn.readUint16(memory, addr)) return setError(NO_MEMORY);
    target = addr;
    if (absLong) {
        uint8_t bank;
        if (insn.readByte(memory, bank)) return setError(NO_MEMORY);
        target |= static_cast<Config::uintptr_t>(bank) << 16;
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
        outConstant(target, 16, false, ADDRESS_24BIT);
    }
    if (index != REG_UNDEF) {
        *_operands++ = ',';
        _operands = _regs.outRegName(_operands, index);
    }
    if (indirect) *_operands++ = idirLong ? ']' : ')';
    *_operands = 0;
    return setOK();
}

Error DisW65C816::decodeZeroPage(
    DisMemory &memory, InsnW65C816 &insn) {
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
    return setOK();
}

Error DisW65C816::decodeRelative(
    DisMemory &memory, InsnW65C816 &insn) {
    const uint8_t bank = static_cast<uint8_t>(insn.address() >> 16);
    uint16_t addr = static_cast<uint16_t>(insn.address());
    if (insn.addrMode() == REL_LONG) {
        uint16_t val;
        if (insn.readUint16(memory, val)) return setError(NO_MEMORY);
        addr += 3 + static_cast<int16_t>(val);
    } else {
        uint8_t val;
        if (insn.readByte(memory, val)) return setError(NO_MEMORY);
        addr += (insn.addrMode() == ZPG_REL ? 3 : 2);
        addr += static_cast<int8_t>(val);
    }
    const Config::uintptr_t target =
        (static_cast<Config::uintptr_t>(bank) << 16) + addr;
    const char *label = lookup(target);
    if (label) {
        outText(label);
    } else {
        outConstant(target, 16, false, Config::addressBits());
    }
    return setOK();
}

Error DisW65C816::decodeBlockMove(
    DisMemory &memory, InsnW65C816 &insn) {
    uint8_t sbank, dbank;
    if (insn.readByte(memory, dbank)) return setError(NO_MEMORY);
    if (insn.readByte(memory, sbank)) return setError(NO_MEMORY);
    const Config::uintptr_t src = static_cast<Config::uintptr_t>(sbank) << 16;
    const Config::uintptr_t dst = static_cast<Config::uintptr_t>(dbank) << 16;
    const char *label = lookup(src);
    if (label) {
        outText(label);
    } else {
        outConstant(src, 16, false, Config::addressBits());
    }
    *_operands++ = ',';
    label = lookup(dst);
    if (label) {
        outText(label);
    } else {
        outConstant(dst, 16, false, Config::addressBits());
    }
    return setOK();
}

Error DisW65C816::decode(
    DisMemory &memory, Insn &_insn) {
    InsnW65C816 insn(_insn);
    Config::opcode_t opCode;
    if (insn.readByte(memory, opCode)) return setError(NO_MEMORY);
    insn.setOpCode(opCode);

    if (TableW65C816.searchOpCode(insn, _acceptIndirectLong))
        return setError(UNKNOWN_INSTRUCTION);

    switch (insn.addrMode()) {
    case IMPL:
        return setOK();
    case ACCM:
        _operands = _regs.outRegName(_operands, REG_A);
        *_operands = 0;
        return setOK();
    case IMM8:
    case IMMA:
    case IMMX:
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

} // namespace w65c816
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

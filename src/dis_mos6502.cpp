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

#include "dis_mos6502.h"
#include "table_mos6502.h"

namespace libasm {
namespace mos6502 {

Error DisMos6502::decodeImmediate(
    DisMemory& memory, InsnMos6502 &insn) {
    uint8_t val;
    if (insn.readByte(memory, val)) return setError(NO_MEMORY);
    *_operands++ = '#';
    outConstant(val);
    return setOK();
}

Error DisMos6502::decodeAbsolute(
    DisMemory& memory, InsnMos6502 &insn) {
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
    Config::uintptr_t addr;
    uint32_t target;
    if (insn.readUint16(memory, addr)) return setError(NO_MEMORY);
    target = addr;
    if (indirect) *_operands++ = '(';
    const char *label = lookup(target);
    if (label) {
        *_operands++ = '>';
        outText(label);
    } else {
        if (target < 0x100) *_operands++ = '>';
        outConstant(addr, 16, false);
    }
    if (index != REG_UNDEF) {
        *_operands++ = ',';
        _operands = _regs.outRegName(_operands, index);
    }
    if (indirect) *_operands++ = ')';
    *_operands = 0;
    return setOK();
}

Error DisMos6502::decodeZeroPage(
    DisMemory &memory, InsnMos6502 &insn) {
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
    return setOK();
}

Error DisMos6502::decodeRelative(
    DisMemory &memory, InsnMos6502 &insn) {
    Config::uintptr_t addr;
    uint8_t val;
    if (insn.readByte(memory, val)) return setError(NO_MEMORY);
    addr = insn.address() + (insn.addrMode() == ZPG_REL ? 3 : 2)
        + static_cast<int8_t>(val);
    outConstant(addr, 16, false);
    return setOK();
}

Error DisMos6502::decode(
    DisMemory &memory, Insn &_insn) {
    InsnMos6502 insn(_insn);
    Config::opcode_t opCode;
    if (insn.readByte(memory, opCode)) return setError(NO_MEMORY);
    insn.setOpCode(opCode);

    if (TableMos6502.searchOpCode(insn))
        return setError(TableMos6502.getError());

    switch (insn.addrMode()) {
    case IMPL:
        return setOK();
    case ACCM:
        _operands = _regs.outRegName(_operands, REG_A);
        *_operands = 0;
        return setOK();
    case IMMA:
    case IMMX:
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

} // namespace mos6502
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

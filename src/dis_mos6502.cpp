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

void DisMos6502::reset() {
    useIndirectLong(true);
    longAccumlator(false);
    longIndex(false);
}

Error DisMos6502::decodeImmediate(
    DisMemory& memory, InsnMos6502 &insn, char *out) {
    *out++ = '#';
    if (TableMos6502.longImmediate(insn.addrMode())) {
        uint16_t val16;
        if (insn.readUint16(memory, val16)) return setError(NO_MEMORY);
        outConstant(out, val16);
    } else {
        uint8_t val8;
        if (insn.readByte(memory, val8)) return setError(NO_MEMORY);
        outConstant(out, val8);
    }
    if (insn.opCode() == TableMos6502::WDM)
        return setError(UNKNOWN_INSTRUCTION);
    return setOK();
}

Error DisMos6502::decodeAbsolute(
    DisMemory& memory, InsnMos6502 &insn, char *out) {
    const AddrMode addrMode = insn.addrMode();
    const bool indirect = addrMode == ABS_IDX_IDIR
        || addrMode == ABS_IDIR
        || addrMode == ABS_IDIR_LONG;
    const bool absLong = addrMode == ABS_LONG
        || addrMode == ABS_LONG_IDX;
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
    if (indirect) *out++ = idirLong ? '[' : '(';
    uint16_t addr;
    if (insn.readUint16(memory, addr)) return setError(NO_MEMORY);
    if (!absLong) {
        out = outAddress(out, addr, PSTR(">"), addr < 0x100);
    } else {
        uint32_t target = addr;
        uint8_t bank;
        if (insn.readByte(memory, bank)) return setError(NO_MEMORY);
        target |= static_cast<uint32_t>(bank) << 16;
        out = outAddress(
            out, target, PSTR(">>"), target < 0x10000, addressWidth());
    }
    if (index != REG_UNDEF) {
        *out++ = ',';
        out = _regs.outRegName(out, index);
    }
    if (indirect) *out++ = idirLong ? ']' : ')';
    *out = 0;
    return setOK();
}

Error DisMos6502::decodeZeroPage(
    DisMemory &memory, InsnMos6502 &insn, char *out) {
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
    if (indirect) *out++ = zpLong ? '[' : '(';
    out = outAddress(out, zp, PSTR("<"));
    if (indirect && index == REG_Y) *out++ = zpLong ? ']' : ')';
    if (index != REG_UNDEF) {
        *out++ = ',';
        out = _regs.outRegName(out, index);
    }
    if (indirect && index != REG_Y) *out++ = zpLong ? ']' : ')';
    if (addrMode == SP_REL_IDIR_IDY) {
        *out++ = ',';
        out = _regs.outRegName(out, REG_Y);
    }
    *out = 0;
    if (addrMode == ZPG_REL) {
        *out++ = ',';
        return decodeRelative(memory, insn, out);
    }
    return setOK();
}

Error DisMos6502::decodeRelative(
    DisMemory &memory, InsnMos6502 &insn, char *out) {
    const uint8_t bank = static_cast<uint8_t>(insn.address() >> 16);
    uint16_t addr = static_cast<uint16_t>(insn.address());
    const AddrMode addrMode = insn.addrMode();
    if (addrMode == REL_LONG) {
        uint16_t val16;
        if (insn.readUint16(memory, val16)) return setError(NO_MEMORY);
        addr += 3 + static_cast<int16_t>(val16);
    } else {
        uint8_t val8;
        if (insn.readByte(memory, val8)) return setError(NO_MEMORY);
        addr += ((addrMode == ZPG_REL) ? 3 : 2) + static_cast<int8_t>(val8);
    }
    const uint32_t target = (static_cast<uint32_t>(bank) << 16) + addr;
    outRelativeAddr(out, target, insn.address(), addrMode == REL_LONG ? 16 : 8);
    return setOK();
}

Error DisMos6502::decodeBlockMove(
    DisMemory &memory, InsnMos6502 &insn, char *out) {
    uint8_t sbank, dbank;
    if (insn.readByte(memory, dbank)) return setError(NO_MEMORY);
    if (insn.readByte(memory, sbank)) return setError(NO_MEMORY);
    const uint32_t src = static_cast<uint32_t>(sbank) << 16;
    const uint32_t dst = static_cast<uint32_t>(dbank) << 16;
    out = outAddress(out, src, nullptr, false, addressWidth());
    *out++ = ',';
    outAddress(out, dst, nullptr, false, addressWidth());
    return setOK();
}

Error DisMos6502::decode(DisMemory &memory, Insn &_insn, char *out) {
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
        out = _regs.outRegName(out, REG_A);
        *out = 0;
        return setOK();
    case IMM8:
    case IMMA:
    case IMMX:
        return decodeImmediate(memory, insn, out);
    case ABS:
    case ABS_IDX:
    case ABS_IDY:
    case ABS_IDIR:
    case ABS_IDX_IDIR:
    case ABS_LONG:
    case ABS_LONG_IDX:
    case ABS_IDIR_LONG:
        return decodeAbsolute(memory, insn, out);
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
        return decodeZeroPage(memory, insn, out);
    case REL:
    case REL_LONG:
        return decodeRelative(memory, insn, out);
    case BLOCK_MOVE:
        return decodeBlockMove(memory, insn, out);
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

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
        DisMemory &memory, InsnMos6502 &insn, char *out) {
    *out++ = '#';
    if (TableMos6502.longImmediate(insn.addrMode())) {
        outHex(out, insn.readUint16(memory), 16);
    } else {
        outHex(out, insn.readByte(memory), 8);
    }
    if (insn.opCode() == TableMos6502::WDM)
        return setError(UNKNOWN_INSTRUCTION);
    return setError(insn);
}

Error DisMos6502::decodeAbsolute(
        DisMemory &memory, InsnMos6502 &insn, char *out) {
    const AddrMode addrMode = insn.addrMode();
    const bool indirect = addrMode == ABS_IDX_IDIR || addrMode == ABS_IDIR ||
                          addrMode == ABS_IDIR_LONG;
    const bool absLong = addrMode == ABS_LONG || addrMode == ABS_LONG_IDX;
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
    if (indirect)
        *out++ = idirLong ? '[' : '(';
    const uint16_t addr = insn.readUint16(memory);
    if (!absLong) {
        out = outAbsAddr(out, addr, 16, PSTR(">"), addr < 0x100);
    } else {
        uint32_t target = addr;
        const uint8_t bank = insn.readByte(memory);
        target |= static_cast<uint32_t>(bank) << 16;
        // JSL has only ABS_LONG addressing
        if (insn.opCode() == TableMos6502::JSL) {
            out = outAbsAddr(out, target, addressWidth());
        } else {
            out = outAbsAddr(
                    out, target, addressWidth(), PSTR(">>"), target < 0x10000);
        }
    }
    if (index != REG_UNDEF) {
        *out++ = ',';
        out = _regs.outRegName(out, index);
    }
    if (indirect)
        *out++ = idirLong ? ']' : ')';
    *out = 0;
    return setError(insn);
}

Error DisMos6502::decodeZeroPage(
        DisMemory &memory, InsnMos6502 &insn, char *out) {
    const AddrMode addrMode = insn.addrMode();
    const bool indirect =
            addrMode == ZPG_IDX_IDIR || addrMode == ZPG_IDIR_IDY ||
            addrMode == ZPG_IDIR || addrMode == SP_REL_IDIR_IDY ||
            addrMode == ZPG_IDIR_LONG || addrMode == ZPG_IDIR_LONG_IDY;
    const bool zpLong =
            addrMode == ZPG_IDIR_LONG || addrMode == ZPG_IDIR_LONG_IDY;
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
    const uint8_t zp = insn.readByte(memory);
    if (indirect)
        *out++ = zpLong ? '[' : '(';
    out = outAbsAddr(out, zp, 8, PSTR("<"));
    if (indirect && index == REG_Y)
        *out++ = zpLong ? ']' : ')';
    if (index != REG_UNDEF) {
        *out++ = ',';
        out = _regs.outRegName(out, index);
    }
    if (indirect && index != REG_Y)
        *out++ = zpLong ? ']' : ')';
    if (addrMode == SP_REL_IDIR_IDY) {
        *out++ = ',';
        out = _regs.outRegName(out, REG_Y);
    }
    *out = 0;
    if (addrMode == ZPG_REL) {
        *out++ = ',';
        return decodeRelative(memory, insn, out);
    }
    return setError(insn);
}

Error DisMos6502::decodeRelative(
        DisMemory &memory, InsnMos6502 &insn, char *out) {
    const uint8_t deltaBits = (insn.addrMode() == REL_LONG) ? 16 : 8;
    const int16_t delta =
            (deltaBits == 16) ? static_cast<int16_t>(insn.readUint16(memory))
                              : static_cast<int8_t>(insn.readByte(memory));
    const uint16_t origin = insn.address();
    const uint16_t base = origin + insn.length();
    const uint16_t target = base + delta;
    Config::uintptr_t addr, orig;
    if (addressWidth() == ADDRESS_24BIT) {
        const Config::uintptr_t bank = insn.address() & ~0xFFFF;
        addr = bank | target;
        orig = addr - delta - insn.length();
        if ((delta >= 0 && target <= origin) || (delta < 0 && target > base)) {
            outAbsAddr(out, addr, addressWidth());
            return setError(insn);
        }
    } else {
        addr = static_cast<int16_t>(target);
        orig = static_cast<int16_t>(origin);
    }
    outRelAddr(out, addr, orig, deltaBits);
    return setError(insn);
}

Error DisMos6502::decodeBlockMove(
        DisMemory &memory, InsnMos6502 &insn, char *out) {
    const uint8_t dbank = insn.readByte(memory);
    const uint8_t sbank = insn.readByte(memory);
    const uint32_t dst = static_cast<uint32_t>(dbank) << 16;
    const uint32_t src = static_cast<uint32_t>(sbank) << 16;
    out = outAbsAddr(out, src, addressWidth());
    *out++ = ',';
    outAbsAddr(out, dst, addressWidth());
    return setOK();
}

Error DisMos6502::decode(DisMemory &memory, Insn &_insn, char *out) {
    InsnMos6502 insn(_insn);
    const Config::opcode_t opCode = insn.readByte(memory);
    if (setError(insn))
        return getError();
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

}  // namespace mos6502
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

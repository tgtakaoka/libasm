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

const char DisMos6502::OPT_BOOL_INDIRECT_LONG[] PROGMEM = "indirect-long";
const char DisMos6502::OPT_DESC_INDIRECT_LONG[] PROGMEM = "[] for indirect long operand";
const char DisMos6502::OPT_BOOL_LONGA[] PROGMEM = "longa";
const char DisMos6502::OPT_DESC_LONGA[] PROGMEM = "enable 16-bit accumulator";
const char DisMos6502::OPT_BOOL_LONGI[] PROGMEM = "longi";
const char DisMos6502::OPT_DESC_LONGI[] PROGMEM = "enable 16-bit index registers";

Error DisMos6502::decodeImmediate(
        DisMemory &memory, InsnMos6502 &insn, StrBuffer &out, AddrMode mode) {
    out.letter('#');
    if (TableMos6502::TABLE.longImmediate(mode)) {
        outHex(out, insn.readUint16(memory), 16);
    } else {
        outHex(out, insn.readByte(memory), 8);
    }
    if (insn.opCode() == TableMos6502::WDM)
        return setError(UNKNOWN_INSTRUCTION);
    return setError(insn);
}

Error DisMos6502::decodeAbsolute(
        DisMemory &memory, InsnMos6502 &insn, StrBuffer &out, AddrMode mode) {
    const RegName index = (mode == ABS_IDX || mode == ABS_IDX_IDIR || mode == ABS_LONG_IDX)
                                  ? REG_X
                                  : (mode == ABS_IDY ? REG_Y : REG_UNDEF);
    const bool indirect = (mode == ABS_IDX_IDIR || mode == ABS_IDIR || mode == ABS_IDIR_LONG);
    const bool idirLong = (mode == ABS_IDIR_LONG);
    if (indirect)
        out.letter(idirLong ? '[' : '(');
    const uint16_t addr = insn.readUint16(memory);
    if (mode == ABS_LONG || mode == ABS_LONG_IDX) {
        uint32_t target = addr;
        const uint8_t bank = insn.readByte(memory);
        target |= static_cast<uint32_t>(bank) << 16;
        // JSL has only ABS_LONG addressing
        if (insn.opCode() == TableMos6502::JSL) {
            outAbsAddr(out, target);
        } else {
            const char *label = lookup(target);
            if (label) {
                out.letter('>').letter('>').text(label);
            } else {
                if (target < 0x10000)
                    out.letter('>').letter('>');
                outAbsAddr(out, target);
            }
        }
    } else {
        const char *label = lookup(addr);
        if (label) {
            out.letter('>').text(label);
        } else {
            if (addr < 0x100)
                out.letter('>');
            outAbsAddr(out, addr, 16);
        }
    }
    if (index != REG_UNDEF) {
        out.letter(',');
        _regs.outRegName(out, index);
    }
    if (indirect)
        out.letter(idirLong ? ']' : ')');
    return setError(insn);
}

Error DisMos6502::decodeZeroPage(
        DisMemory &memory, InsnMos6502 &insn, StrBuffer &out, AddrMode mode) {
    const bool indirect =
            (mode == ZPG_IDX_IDIR || mode == ZPG_IDIR_IDY || mode == ZPG_IDIR ||
                    mode == SP_REL_IDIR_IDY || mode == ZPG_IDIR_LONG || mode == ZPG_IDIR_LONG_IDY);
    const bool zpLong = (mode == ZPG_IDIR_LONG || mode == ZPG_IDIR_LONG_IDY);
    RegName index;
    switch (mode) {
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
        out.letter(zpLong ? '[' : '(');
    const char *label = lookup(zp);
    if (label) {
        out.letter('<').text(label);
    } else {
        outAbsAddr(out, zp, 8);
    }
    if (indirect && index == REG_Y)
        out.letter(zpLong ? ']' : ')');
    if (index != REG_UNDEF) {
        out.letter(',');
        _regs.outRegName(out, index);
    }
    if (indirect && index != REG_Y)
        out.letter(zpLong ? ']' : ')');
    if (mode == SP_REL_IDIR_IDY) {
        out.letter(',');
        _regs.outRegName(out, REG_Y);
    }
    if (mode == ZPG_REL) {
        out.letter(',');
        return decodeRelative(memory, insn, out, REL);
    }
    return setError(insn);
}

Error DisMos6502::decodeRelative(
        DisMemory &memory, InsnMos6502 &insn, StrBuffer &out, AddrMode mode) {
    Config::ptrdiff_t delta;
    if (mode == REL_LONG) {
        delta = static_cast<int16_t>(insn.readUint16(memory));
    } else {
        delta = static_cast<int8_t>(insn.readByte(memory));
    }
    const uint16_t origin = insn.address();
    const uint16_t base = origin + insn.length();
    const uint16_t target = base + delta;
    if ((delta >= 0 && target < base) || (delta < 0 && target >= base))
        return setError(OPERAND_TOO_FAR);
    if (addressWidth() == ADDRESS_24BIT) {
        const Config::uintptr_t orig = insn.address();
        const Config::uintptr_t addr = orig + insn.length() + delta;
        if (orig >> 16 != addr >> 16)
            return setError(OPERAND_TOO_FAR);
        const uint8_t deltaBits = (mode == REL_LONG) ? 16 : 8;
        outRelAddr(out, addr, orig, deltaBits);
    } else {
        outRelAddr(out, target, origin, 8);
    }
    return setError(insn);
}

Error DisMos6502::decodeBlockMove(DisMemory &memory, InsnMos6502 &insn, StrBuffer &out) {
    const uint8_t dbank = insn.readByte(memory);
    const uint8_t sbank = insn.readByte(memory);
    const uint32_t dst = static_cast<uint32_t>(dbank) << 16;
    const uint32_t src = static_cast<uint32_t>(sbank) << 16;
    outAbsAddr(out, src).comma();
    outAbsAddr(out, dst);
    return setOK();
}

Error DisMos6502::decode(DisMemory &memory, Insn &_insn, StrBuffer &out) {
    InsnMos6502 insn(_insn);
    const Config::opcode_t opCode = insn.readByte(memory);
    if (setError(insn))
        return getError();
    insn.setOpCode(opCode);

    if (TableMos6502::TABLE.searchOpCode(insn))
        return setError(TableMos6502::TABLE.getError());

    const AddrMode mode = insn.addrMode();
    switch (mode) {
    case IMPL:
        return setOK();
    case ACCM:
        _regs.outRegName(out, REG_A);
        return setOK();
    case IMM8:
    case IMMA:
    case IMMX:
        return decodeImmediate(memory, insn, out, mode);
    case ABS:
    case ABS_IDX:
    case ABS_IDY:
    case ABS_IDIR:
    case ABS_IDX_IDIR:
    case ABS_LONG:
    case ABS_LONG_IDX:
    case ABS_IDIR_LONG:
        return decodeAbsolute(memory, insn, out, mode);
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
        return decodeZeroPage(memory, insn, out, mode);
    case REL:
    case REL_LONG:
        return decodeRelative(memory, insn, out, mode);
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

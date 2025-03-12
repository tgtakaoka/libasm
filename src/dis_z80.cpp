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

#include "dis_z80.h"
#include "reg_z80.h"
#include "table_z80.h"

namespace libasm {
namespace z80 {

using namespace reg;

const ValueFormatter::Plugins &DisZ80::defaultPlugins() {
    return ValueFormatter::Plugins::intel();
}

DisZ80::DisZ80(const ValueFormatter::Plugins &plugins) : Disassembler(plugins), Config(TABLE) {
    reset();
}

StrBuffer &DisZ80::outIndirectReg(StrBuffer &out, RegName reg) const {
    return outRegName(out.letter('('), reg).letter(')');
}

StrBuffer &DisZ80::outDataReg(StrBuffer &out, RegName reg) const {
    if (reg == REG_HL)
        return outIndirectReg(out, reg);
    return outRegName(out, reg);
}

void DisZ80::decodeAbsolute(DisInsn &insn, StrBuffer &out) const {
    outAbsAddr(out.letter('('), insn.readUint16()).letter(')');
}

void DisZ80::decodeRelative(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    int_fast8_t bits = 0;
    int32_t delta = 0;
    if (mode == M_REL8) {
        bits = 8;
        delta = static_cast<int8_t>(insn.readByte());
    } else if (mode == M_REL16) {
        bits = 16;
        delta = static_cast<int16_t>(insn.readUint16());
    }
    uint32_t base = insn.address() + insn.length();
    if (insn.src() == M_IM8) {
        base += 1;              // <rel16>, im8
    } else if (insn.src() == M_IM16) {
        base += 2;              // <rel16>, im16
    } else if (insn.src() == M_EPU) {
        base += 4;              // <rel16>, EPU template
    }
    uint32_t target = base + delta;
    if ((base & ~UINT16_MAX) != (target & ~UINT16_MAX)) {
        insn.setErrorIf(out, OVERFLOW_RANGE);
        if (z280()) {
            target &= UINT32_C(0xFFFFFF);
        } else if (z180()) {
            target &= UINT32_C(0xFFFFF);
        } else {
            target &= UINT16_MAX;
        }
    }
    insn.setErrorIf(out, checkAddr(target));
    if (z280() && mode == M_REL16)
        out.letter('<');
    outRelAddr(out, target, insn.address(), bits);
    if (z280() && mode == M_REL16)
        out.letter('>');
}

namespace {
RegName decodeIndexReg(uint16_t prefix) {
    if (prefix == TableZ80::IX || prefix == TableZ80::IXEXT || prefix == TableZ80::IXBIT)
        return REG_IX;
    if (prefix == TableZ80::IY || prefix == TableZ80::IYEXT || prefix == TableZ80::IYBIT)
        return REG_IY;
    return REG_UNDEF;
}

RegName decodePointerReg(uint_fast8_t num, uint16_t prefix) {
    const auto name = RegName(num & 3);
    if (name == REG_HL) {
        const auto ix = decodeIndexReg(prefix);
        return ix == REG_UNDEF ? name : ix;
    }
    return name;
}

uint_fast8_t decodeInterruptMode(uint_fast8_t opc) {
    static const uint8_t MODE[] = {0, 3, 1, 2};
    return MODE[(opc >> 3) & 3];
}
}  // namespace

void DisZ80::decodeShortIndex(DisInsn &insn, StrBuffer &out, RegName base) const {
    const auto disp = insn.ixBit() ? insn.ixoff : static_cast<int8_t>(insn.readByte());
    outRegName(out.letter('('), base);
    if (disp >= 0)
        out.letter('+');
    outDec(out, disp, -8).letter(')');
}

void DisZ80::decodeLongIndex(DisInsn &insn, StrBuffer &out, RegName base) const {
    const auto disp = static_cast<int16_t>(insn.readUint16());
    outRegName(out.letter('('), base);
    if (disp >= 0)
        out.letter('+');
    outHex(out, disp, -16).letter(')');
}

void DisZ80::decodeMemoryPointer(DisInsn &insn, StrBuffer &out) const {
    if (insn.opCode() & 0x10) {
        decodeAbsolute(insn, out);
    } else {
        outIndirectReg(out, REG_HL);
    }
}

void DisZ80::decodeBaseIndex(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    const auto opc = insn.opCode();
    const auto type = (mode == M_BIXL ? opc : (opc >> 3)) & 3;
    if (type == 0) {
        decodeLongIndex(insn, out, REG_SP);
    } else {
        static constexpr RegName BASE[] = {REG_SP, REG_HL, REG_HL, REG_IX};
        static constexpr RegName INDEX[] = {REG_UNDEF, REG_IX, REG_IY, REG_IY};
        outRegName(out.letter('('), BASE[type]);
        outRegName(out.letter('+'), INDEX[type]).letter(')');
    }
}

void DisZ80::decodePointerIndex(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    const auto opc = insn.opCode();
    const auto type = (mode == M_PIXL ? opc : (opc >> 3)) & 3;
    if (type == 0) {
        decodeRelative(insn, out, M_REL16);
    } else {
        static constexpr RegName BASE[] = {REG_UNDEF, REG_IX, REG_IY, REG_HL};
        decodeLongIndex(insn, out, BASE[type]);
    }
}

void DisZ80::decodeFullIndex(DisInsn &insn, StrBuffer &out) const {
    if (insn.opCode() & 0x20) {
        decodePointerIndex(insn, out, M_PIXH);
    } else {
        decodeBaseIndex(insn, out, M_BIXH);
    }
}

void DisZ80::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    auto opc = insn.opCode();
    switch (mode) {
    case M_IM8:
        outHex(out, insn.readByte(), 8);
        break;
    case M_IM16:
        outHex(out, insn.readUint16(), 16);
        break;
    case M_EPU:
        outHex(out, insn.readUint32(), 32);
        break;
    case M_ABS:
        decodeAbsolute(insn, out);
        break;
    case M_IOA:
        out.letter('(');
        outHex(out, insn.readByte(), 8).letter(')');
        break;
    case M_IDX:
    case M_IDX8:
        decodeShortIndex(insn, out, decodeIndexReg(insn.prefix()));
        break;
    case M_IDX16:
        decodeLongIndex(insn, out, (opc & 0x10) == 0 ? REG_IX : REG_IY);
        break;
    case M_MPTR:
        decodeMemoryPointer(insn, out);
        break;
    case M_BIXH:
    case M_BIXL:
        decodeBaseIndex(insn, out, mode);
        break;
    case M_PIXH:
    case M_PIXL:
        decodePointerIndex(insn, out, mode);
        break;
    case M_FIDX:
        decodeFullIndex(insn, out);
        break;
    case M_CC4:
        outCcName(out, decodeCcName((opc >> 3) & 3));
        break;
    case M_CC8:
        outCcName(out, decodeCcName(opc >> 3));
        break;
    case M_REL8:
    case M_REL16:
        decodeRelative(insn, out, mode);
        break;
    case M_R16:
        outRegName(out, decodePointerReg(opc >> 4, 0));
        break;
    case M_NOHL:
    case M_R16X:
        outRegName(out, decodePointerReg(opc >> 4, insn.prefix()));
        break;
    case M_STK:
        outRegName(out, decodeStackReg(opc >> 4));
        break;
    case M_SRC:
    case M_SR8:
    case M_SR8X:
        outDataReg(out, decodeDataReg(opc));
        break;
    case M_DST:
    case M_DR8:
    case M_DR8X:
        outDataReg(out, decodeDataReg(opc >> 3));
        break;
    case M_VEC:
        outHex(out, opc & 0x38, 8);
        break;
    case M_BIT:
        outHex(out, (opc >> 3) & 7, 3);
        break;
    case M_IMMD:
        opc = decodeInterruptMode(opc);
        if (z80() && opc == 3) {
            insn.setErrorIf(UNKNOWN_INSTRUCTION);
        } else {
            out.letter(opc + '0');
        }
        break;
    case R_IDX:
        outRegName(out, decodeIndexReg(insn.prefix()));
        break;
    case R_DXY:
    case R_SXY: {
        const auto ix = decodeIndexReg(insn.prefix());
        const auto hilo = (opc & (mode == R_DXY ? 8 : 1)) ? 'L' : 'H';
        outRegName(out, ix).letter(hilo);
        break;
    }
    case R_A:
    case R_H:
    case R_L:
    case R_DE:
    case R_HL:
    case R_SP:
    case R_AF:
    case R_AFP:
    case R_DEHL:
    case R_I:
    case R_R:
    case R_USP:
        outRegName(out, RegName(mode - R_BASE));
        break;
    case I_PTR:
        outIndirectReg(out, decodeIndirectBase(opc >> 4));
        break;
    case I_IDX:
        outIndirectReg(out, decodeIndexReg(insn.prefix()));
        break;
    case I_HL:
    case I_SP:
    case I_C:
        outIndirectReg(out, RegName(mode - I_BASE));
        break;
    default:
        break;
    }
}

StrBuffer &DisZ80::outAbsAddr(StrBuffer &out, uint32_t val, uint8_t addrWidth) const {
    if (val <= UINT16_MAX)
        addrWidth = ADDRESS_16BIT;
    return Disassembler::outAbsAddr(out, val, addrWidth);
}

Error DisZ80::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) const {
    DisInsn insn(_insn, memory, out);
    auto opc = insn.readByte();
    if (isPrefix(cpuType(), opc)) {
        insn.setPrefix(opc);
        opc = insn.readByte();
        auto prefix = (insn.prefix() << 8) | opc;
        if (isPrefix(cpuType(), prefix)) {
            insn.setPrefix(prefix);
            opc = insn.readByte();
            if (insn.ixBit()) {
                insn.ixoff = opc;
                opc = insn.readByte();
            }
        }
    }
    insn.setOpCode(opc);
    if (insn.getError())
        return _insn.setError(insn);
    if (searchOpCode(cpuType(), insn, out))
        return _insn.setError(insn);

    decodeOperand(insn, out, insn.dst());
    const auto src = insn.src();
    if (src != M_NONE)
        decodeOperand(insn, out.comma(), src);
    if (insn.getError() == UNKNOWN_INSTRUCTION)
        insn.nameBuffer().reset();
    return _insn.setError(insn);
}

}  // namespace z80
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

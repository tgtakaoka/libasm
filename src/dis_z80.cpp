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
using namespace text::option;

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

StrBuffer &DisZ80::outAlternateReg(StrBuffer &out, const DisInsn &insn, AddrMode other) const {
    auto reg = REG_UNDEF;
    const auto opc = insn.opCode();
    switch (other) {
    case R_AF:
        reg = REG_AF;
        break;
    case M_SR8:
        reg = decodeDataReg(opc);
        break;
    case R_HL:
        reg = REG_HL;
        break;
    default:
        break;
    }
    if (reg != REG_UNDEF)
        outRegName(out, reg).letter('\'');
    return out;
}

void DisZ80::decodeImmediate16(DisInsn &insn, StrBuffer &out) const {
    outAbsAddr(out, insn.readUint16(), 16);
}

void DisZ80::decodeAbsolute(DisInsn &insn, StrBuffer &out) const {
    const auto abs = insn.readUint16();
    outAbsAddr(out.letter('('), abs, 16).letter(')');
}

void DisZ80::decodeRelative(DisInsn &insn, StrBuffer &out) const {
    const int16_t delta = static_cast<int8_t>(insn.readByte());
    const auto base = insn.address() + insn.length();
    auto target = base + delta;
    if ((base & ~UINT16_MAX) != (target & ~UINT16_MAX)) {
        insn.setErrorIf(out, OVERFLOW_RANGE);
        const auto width = addressWidth();
        if (width == ADDRESS_20BIT) {
            target &= UINT32_C(0xFFFFF);
        } else {  //  ADDRESS_16BIT
            target &= UINT16_MAX;
        }
    }
    insn.setErrorIf(out, checkAddr(target));
    outRelAddr(out, target, insn.address(), 8);
}

RegName decodePointerReg(uint_fast8_t num, Config::prefix_t prefix) {
    const auto name = RegName(num & 3);
    if (name == REG_HL) {
        const auto ix = Entry::decodeIndex(prefix);
        return ix == REG_UNDEF ? name : ix;
    }
    return name;
}

uint_fast8_t decodeInterruptMode(uint_fast8_t opc) {
    static const uint8_t MODE[] = {0, 3, 1, 2};
    return MODE[(opc >> 3) & 3];
}

void DisZ80::decodeShortIndex(DisInsn &insn, StrBuffer &out, RegName base) const {
    const auto disp = insn.ixBit() ? insn.ixoff : insn.readDisp8();
    outRegName(out.letter('('), base);
    if (disp >= 0)
        out.letter('+');
    outDec(out, disp, -8);
    out.letter(')');
}

void DisZ80::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode, AddrMode other) const {
    auto opc = insn.opCode();
    switch (mode) {
    case M_IM8:
        outHex(out, insn.readByte(), 8);
        break;
    case M_IM16:
    case M_LM16:
    case M_JABS:
        decodeImmediate16(insn, out);
        break;
    case M_ABS:
    case M_DABS:
        decodeAbsolute(insn, out);
        break;
    case M_IOA:
        outHex(out.letter('('), insn.readByte(), 8).letter(')');
        break;
        break;
    case M_IDX:
    case M_IDX8:
        decodeShortIndex(insn, out, Entry::decodeIndex(insn.prefix()));
        break;
    case M_CC4:
        outCcName(out, decodeCcName((opc >> 3) & 3));
        break;
    case M_CC8:
        outCcName(out, decodeCcName(opc >> 3));
        break;
    case M_REL8:
        decodeRelative(insn, out);
        break;
    case M_R16:
        outRegName(out, decodePointerReg(opc >> 4, 0));
        break;
    case M_R16X:
        outRegName(out, decodePointerReg(opc >> 4, insn.prefix()));
        break;
    case M_SRC:
    case M_SR8:
        outDataReg(out, decodeDataReg(opc));
        break;
    case M_DST:
    case M_DR8:
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
        if (opc == 3) {
            insn.setErrorIf(UNKNOWN_INSTRUCTION);
        } else {
            out.letter(opc + '0');
        }
        break;
    case R_IDX:
        outRegName(out, Entry::decodeIndex(insn.prefix()));
        break;
    case R_PTRH:
        outRegName(out, (opc & 0x10) ? REG_DE : REG_BC);
        break;
    case I_PTR:
        outIndirectReg(out, decodeIndirectBase(opc >> 4));
        break;
    case I_IDX:
        outIndirectReg(out, Entry::decodeIndex(insn.prefix()));
        break;
    case I_HL:
    case I_SP:
    case I_C:
        outIndirectReg(out, RegName(mode - I_BASE));
        break;
    case R_ALT:
        outAlternateReg(out, insn, other);
        break;
    default:
        if (mode >= R_BASE)
            outRegName(out, RegName(mode - R_BASE));
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
            if (insn.ixBit())
                insn.ixoff = insn.readDisp8();
            opc = insn.readByte();
        }
    }
    insn.setOpCode(opc);
    if (insn.getError())
        return _insn.setError(insn);

    if (searchOpCode(cpuType(), insn, out))
        return _insn.setError(insn);

    const auto dst = insn.dst();
    const auto src = insn.src();
    decodeOperand(insn, out, dst, src);
    if (src != M_NONE)
        decodeOperand(insn, out.comma(), src, dst);
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

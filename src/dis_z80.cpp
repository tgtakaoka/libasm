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
#include "text_common.h"

namespace libasm {
namespace z80 {

using namespace reg;
using namespace text::common;
using namespace text::option;

const ValueFormatter::Plugins &DisZ80::defaultPlugins() {
    return ValueFormatter::Plugins::intel();
}

DisZ80::DisZ80(const ValueFormatter::Plugins &plugins)
    : Disassembler(plugins, &_opt_extmode),
      Config(TABLE),
      _opt_extmode(
              this, &DisZ80::setExtendedMode, OPT_BOOL_EXTMODE, OPT_DESC_EXTMODE, &_opt_lwordmode),
      _opt_lwordmode(this, &DisZ80::setLongWordMode, OPT_BOOL_LWORDMODE, OPT_DESC_LWORDMODE) {
    reset();
}

void DisZ80::reset() {
    Disassembler::reset();
    setExtendedMode(false);
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
    case R_PTRL:
        reg = (opc & 1) ? REG_DE : REG_BC;
        break;
    case R_HL:
        reg = REG_HL;
        break;
    case R_IDXL:
        reg = (opc & 1) ? REG_IY : REG_IX;
        break;
    default:
        break;
    }
    if (reg != REG_UNDEF)
        outRegName(out, reg).letter('\'');
    return out;
}

bool DisZ80::wordMode(const Ddir &ddir) const {
    return (!_lwordmode && !ddir.lwordMode()) || (_lwordmode && ddir.wordMode());
}

bool DisZ80::lwordMode(const Ddir &ddir) const {
    return (_lwordmode && !ddir.wordMode()) || (!_lwordmode && ddir.lwordMode());
}

void DisZ80::decodeImmediate16(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    if (insn.ddir) {
        if ((mode == M_XM16 && !_extmode) || (mode == M_LM16 && !lwordMode(insn.ddir))) {
            insn.setErrorIf(out, PREFIX_HAS_NO_EFFECT);
            mode = M_IM16;
        }
        if (!insn.lmCapable() && insn.ddir.noImmediate())
            insn.setErrorIf(out, PREFIX_HAS_NO_EFFECT);
        if (!insn.lmCapable() && !insn.ddir.noMode())
            insn.setErrorIf(out, SUBOPTIMAL_INSTRUCTION);
    }
    if (mode == M_IM16 || !insn.ddir || insn.ddir.noImmediate()) {
        outAbsAddr(out, insn.readUint16(), 16);
    } else if (insn.ddir.byteImmediate()) {
        outAbsAddr(out, insn.readUint24(), 24);
    } else if (insn.ddir.wordImmediate()) {
        outAbsAddr(out, insn.readUint32(), 32);
    }
}

void DisZ80::decodeAbsolute(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    const auto *prefix = (mode == M_JABS) ? "" : "(";
    if (insn.ddir) {
        if ((mode == M_XABS || mode == M_JABS) && !_extmode) {
            insn.setErrorIf(out, PREFIX_HAS_NO_EFFECT);
            mode = M_ABS;
        }
        if (!insn.lmCapable() && insn.ddir.noImmediate())
            insn.setErrorIf(out, PREFIX_HAS_NO_EFFECT);
        if (!insn.lmCapable() && !insn.ddir.noMode())
            insn.setErrorIf(out, SUBOPTIMAL_INSTRUCTION);
    }
    if (mode == M_ABS || !insn.ddir || insn.ddir.noImmediate()) {
        outAbsAddr(out.text(prefix), insn.readUint16(), 16);
    } else if (insn.ddir.byteImmediate()) {
        outAbsAddr(out.text(prefix), insn.readUint24(), 24);
    } else if (insn.ddir.wordImmediate()) {
        outAbsAddr(out.text(prefix), insn.readUint32(), 32);
    }
    if (*prefix)
        out.letter(')');
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
    } else if (mode == M_REL24) {
        bits = 24;
        const uint16_t lsw = insn.readUint16();
        const int8_t msb = insn.readByte();
        delta = (static_cast<int32_t>(msb) << 16) | lsw;
    }
    auto base = insn.address() + insn.length();
    if (insn.src() == M_IM8) {
        base += 1;  // <rel16>, im8
    } else if (insn.src() == M_IM16) {
        base += 2;  // <rel16>, im16
    } else if (insn.src() == M_EPU) {
        base += 4;  // <rel16>, EPU template
    }
    auto target = base + delta;
    if (z380() && _extmode) {
        ;
    } else if ((base & ~UINT16_MAX) != (target & ~UINT16_MAX)) {
        insn.setErrorIf(out, OVERFLOW_RANGE);
        const auto width = addressWidth();
        if (width == ADDRESS_24BIT) {
            target &= UINT32_C(0xFFFFFF);
        } else if (width == ADDRESS_20BIT) {
            target &= UINT32_C(0xFFFFF);
        } else {  // ADDRESS_32BIT, ADDRESS_16BIT
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
    if (insn.ddir && !insn.lmCapable()) {
        if (insn.ddir.noImmediate())
            insn.setErrorIf(out, PREFIX_HAS_NO_EFFECT);
        if (!insn.ddir.noMode())
            insn.setErrorIf(out, SUBOPTIMAL_INSTRUCTION);
    }
    const auto disp = insn.ixBit() ? insn.ixoff : insn.readDisp8();
    outRegName(out.letter('('), base);
    if (disp >= 0)
        out.letter('+');
    if (insn.ddir.byteImmediate()) {
        outHex(out, disp, -16);
    } else if (insn.ddir.wordImmediate()) {
        outHex(out, disp, -24);
    } else {
        outDec(out, disp, -8);
    }
    out.letter(')');
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
        decodeAbsolute(insn, out, M_ABS);
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

void DisZ80::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode, AddrMode other) const {
    auto opc = insn.opCode();
    switch (mode) {
    case M_IM8:
        outHex(out, insn.readByte(), 8);
        break;
    case M_IM16:
    case M_DM16:
    case M_XM16:
    case M_LM16:
        decodeImmediate16(insn, out, mode);
        break;
    case M_EPU:
        outHex(out, insn.readUint32(), 32);
        break;
    case M_ABS:
    case M_JABS:
    case M_DABS:
    case M_XABS:
    case M_IO16:
        decodeAbsolute(insn, out, mode);
        break;
    case M_IOA:
        outHex(out.letter('('), insn.readByte(), 8).letter(')');
        break;
        break;
    case M_IDX:
    case M_IDX8:
        decodeShortIndex(insn, out, decodeIndexReg(insn.prefix()));
        break;
    case M_SPX:
        decodeShortIndex(insn, out, REG_SP);
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
    case M_REL24:
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
    case R_IDXL:
        outRegName(out, (opc & 1) ? REG_IY : REG_IX);
        break;
    case R_DXY:
    case R_SXY: {
        const auto ix = decodeIndexReg(insn.prefix());
        const auto hi = z380() ? 'U' : 'H';
        const auto hilo = (opc & (mode == R_DXY ? 8 : 1)) ? 'L' : hi;
        outRegName(out, ix).letter(hilo);
        break;
    }
    case R_PTRH:
        outRegName(out, (opc & 0x10) ? REG_DE : REG_BC);
        break;
    case R_PTRL:
        outRegName(out, (opc & 0x01) ? REG_DE : REG_BC);
        break;
    case I_PTR:
        opc >>= 4;
        // Fall-through
    case I_PTRL:
        outIndirectReg(out, decodeIndirectBase(opc));
        break;
    case I_IDX:
        outIndirectReg(out, decodeIndexReg(insn.prefix()));
        break;
    case I_HL:
    case I_SP:
    case I_C:
        outIndirectReg(out, RegName(mode - I_BASE));
        break;
    case R_ALT:
        outAlternateReg(out, insn, other);
        break;
    case M_LW:
    case M_LCK:
    case M_XM:
        outCtlName(out, insn);
        break;
    default:
        if (mode >= R_BASE)
            outRegName(out, RegName(mode - R_BASE));
        break;
    }
}

StrBuffer &DisZ80::outAbsAddr(StrBuffer &out, uint32_t val, uint8_t addrWidth) const {
    if (val <= UINT16_MAX && (!z380() || !_extmode))
        addrWidth = ADDRESS_16BIT;
    return Disassembler::outAbsAddr(out, val, addrWidth);
}

void Ddir::operator=(const DisInsn &insn) {
    _prefix = insn.prefix();
    _opc = insn.opCode();
}

Error DisZ80::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) const {
    DisInsn insn(_insn, memory, out);
retry:
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
    if (dst == M_DD) {
        if (insn.ddir)
            return _insn.setError(INVALID_INSTRUCTION);
        insn.ddir = insn;
        insn.setPrefix(0);
        goto retry;
    }
    if (insn.ddir && !insn.imCapable()) {
        if (!insn.lmCapable() || !insn.ddir.noImmediate() || insn.ddir.noMode())
            insn.setErrorIf(out, PREFIX_HAS_NO_EFFECT);
    }
    decodeOperand(insn, out, dst, src);
    if (src != M_NONE)
        decodeOperand(insn, out.comma(), src, dst);
    if (insn.getError() == UNKNOWN_INSTRUCTION)
        insn.nameBuffer().reset();
    insn.ddir.clear();
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

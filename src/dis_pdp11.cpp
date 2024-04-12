/*
 * Copyright 2024 Tadashi G. Takaoka
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

#include "dec_float.h"
#include "dis_pdp11.h"
#include "reg_pdp11.h"
#include "table_pdp11.h"

namespace libasm {
namespace pdp11 {

using namespace reg;

const ValueFormatter::Plugins &DisPdp11::defaultPlugins() {
    static const struct final : ValueFormatter::Plugins {
        const DecFormatter &dec() const override { return Pdp11DecFormatter::singleton(); }
        const BinFormatter &bin() const override { return Pdp11BinFormatter::singleton(); }
        char locationSymbol() const override { return '.'; }
    } PLUGINS{};
    return PLUGINS;
}

DisPdp11::DisPdp11(const ValueFormatter::Plugins &plugins) : Disassembler(plugins), Config(TABLE) {
    reset();
}

void DisPdp11::reset() {
    Disassembler::reset();
    setListRadix(RADIX_8);
}

uint32_t DisInsn::readUint32Mix() {
    const auto msw = readUint16();
    const auto lsw = readUint16();
    return (static_cast<uint32_t>(msw) << 16) | lsw;
}

uint64_t DisInsn::readUint64Mix() {
    const auto msdw = readUint32Mix();
    const auto lsdw = readUint32Mix();
    return (static_cast<uint64_t>(msdw) << 32) | lsdw;
}

#if !defined(LIBASM_DIS_NOFLOAT)

float80_t DisInsn::readDecFloat32() {
    dec_float32_t f32;
    return static_cast<float80_t>(f32.set(readUint32Mix()));
}

float80_t DisInsn::readDecFloat64() {
    dec_float64_t f64;
    return static_cast<float80_t>(f64.set(readUint64Mix()));
}

#endif

void DisPdp11::decodeImmediate(DisInsn &insn, StrBuffer &out) const {
#if defined(LIBASM_DIS_NOFLOAT)
    const auto at = out.mark();
#endif
    out.letter('#');
    switch (insn.size()) {
    case SZ_BYTE: {
        const auto val = insn.readUint16();
        if (val > UINT8_MAX) {
            insn.setErrorIf(out, OVERFLOW_RANGE);
            outHex(out, val, 16);
        } else {
            outHex(out, val, 8);
        }
        break;
    }
    case SZ_WORD:
        outHex(out, insn.readUint16(), 16);
        break;
    case SZ_FLOT:
#if defined(LIBASM_DIS_NOFLOAT)
        insn.setErrorIf(at, FLOAT_NOT_SUPPORTED);
        out.oct(insn.readUint32Mix());
#else
        out.float32(insn.readDecFloat32());
#endif
        break;
    case SZ_DUBL:
#if defined(LIBASM_DIS_NOFLOAT)
        insn.setErrorIf(at, FLOAT_NOT_SUPPORTED);
        out.oct(insn.readUint64Mix());
#else
        out.float80(insn.readDecFloat64());
#endif
        break;
    case SZ_INTG:
        outDec(out, insn.readUint16(), -16);
        break;
    case SZ_LONG:
        outDec(out, insn.readUint32Mix(), -32);
        break;
    default:
        break;
    }
}

void DisPdp11::decodeGeneralMode(DisInsn &insn, StrBuffer &out, AddrMode mode, uint8_t opr) const {
    const auto r = opr & 07;
    const auto m = (opr >> 3) & 07;
    if (mode == M_GEND || mode == M_GENA || mode == M_GENG) {
        if (m == 2 && r == 7)  // #immediate
            insn.setErrorIf(out, ILLEGAL_OPERAND_MODE);
    }
    if (mode == M_GENA && m == 0)  // Rn
        insn.setErrorIf(out, REGISTER_NOT_ALLOWED);
    auto reg = decodeGeneralReg(r);
    if ((mode == M_GENF || mode == M_GENG) && m == 0) {
        if (r < 6) {  // AC0~AC5
            reg = decodeFloatReg(r);
        } else {
            insn.setErrorIf(out, ILLEGAL_REGISTER);
        }
    }
    const auto errpos = out.mark();
    switch (m) {
    case 0:  // Rn, ACn
        outRegName(out, reg);
        break;
    case 1:  // (Rn)
        outRegName(out.letter('('), reg).letter(')');
        break;
    case 3:  // @(Rn)+
        out.letter('@');
        // Fall-through
    case 2:  // (Rn)+
        if (reg == REG_PC) {
            if (m == 3) {
                const auto addr = insn.readUint16();
                insn.setErrorIf(errpos, checkAddr(addr, insn.size() != SZ_BYTE));
                outAbsAddr(out.letter('#'), addr);
            } else {
                decodeImmediate(insn, out);
            }
        } else {
            outRegName(out.letter('('), reg).letter(')').letter('+');
        }
        break;
    case 5:  // @-(Rn)
        out.letter('@');
        // Fall-through
    case 4:  // -(Rn)
        if (reg == REG_PC)
            insn.setErrorIf(errpos, NOT_AN_EXPECTED);
        outRegName(out.letter('-').letter('('), reg).letter(')');
        break;
    case 7:  // @X(Rn)
        out.letter('@');
        // Fall-through
    case 6:  // X(Rn)
        if (reg == REG_PC) {
            const auto base = insn.address() + insn.length() + 2;
            const auto delta = static_cast<int16_t>(insn.readUint16());
            const auto target = base + delta;
            insn.setErrorIf(errpos, checkAddr(target, m == 7 || insn.size() != SZ_BYTE));
            outRelAddr(out, target, insn.address(), 16);
        } else {
            outHex(out, insn.readUint16(), 16);
            outRegName(out.letter('('), reg).letter(')');
        }
        break;
    }
}

void DisPdp11::decodeRelative(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    const auto base = insn.address() + 2;
    const auto opc = insn.opCode();
    const auto delta = 2 * static_cast<int8_t>(mode == M_REL8 ? opc : -(opc & 077));
    const auto deltaBits = mode == M_REL8 ? 9 : 7;
    const auto target = base + delta;
    insn.setErrorIf(out, checkAddr(target, true));
    outRelAddr(out, target, insn.address(), deltaBits);
}

namespace {
uint8_t getOperand(Config::opcode_t opc, OprPos pos) {
    switch (pos) {
    case P_0077:
        return opc & 077;
    case P_7700:
        return (opc >> 6) & 077;
    case P_0700:
        return (opc >> 6) & 07;
    case P_0007:
        return opc & 07;
    case P_0377:
        return opc & 0xFF;
    case P_0300:
        return (opc >> 6) & 03;
    default:
        return 0;
    }
}
}  // namespace

void DisPdp11::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode, OprPos pos) const {
    const auto opr = getOperand(insn.opCode(), pos);
    switch (mode) {
    case M_GENR:
        outRegName(out, decodeGeneralReg(opr));
        break;
    case M_GENS:
    case M_GEND:
    case M_GENA:
    case M_GENF:
    case M_GENG:
        decodeGeneralMode(insn, out, mode, opr);
        break;
    case M_AC03:
        outRegName(out, decodeFloatReg(opr));
        break;
    case M_REL8:
    case M_REL6:
        decodeRelative(insn, out, mode);
        break;
    case M_IMM8:
        outHex(out, opr, 8);
        break;
    case M_IMM6:
        outHex(out, opr, 6);
        break;
    case M_IMM3:
        outHex(out, opr, 3);
        break;
    default:
        break;
    }
}

Error DisPdp11::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) const {
    DisInsn insn(_insn, memory, out);
    insn.setOpCode(insn.readUint16());
    if (TABLE.searchOpCode(cpuType(), insn, out))
        return _insn.setError(insn);

    decodeOperand(insn, out, insn.src(), insn.srcPos());
    if (insn.dst() != M_NONE)
        out.comma();
    decodeOperand(insn, out, insn.dst(), insn.dstPos());
    return _insn.setError(insn);
}

}  // namespace pdp11
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

/*
 * Copyright 2025 Tadashi G. Takaoka
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

#include "dis_tms320f.h"
#include "reg_tms320f.h"
#include "table_tms320f.h"
#include "text_tms320f.h"
#include "ti_float.h"

namespace libasm {
namespace tms320f {

using namespace reg;
using namespace text::common;
using namespace text::tms320f;

const ValueFormatter::Plugins &DisTms320f::defaultPlugins() {
    static const struct final : ValueFormatter::Plugins {
        const HexFormatter &hex() const override { return _hex; }
        char locationSymbol() const override { return '$'; }
        const /*PROGMEM*/ char *lineComment_P() const override { return PSTR_STAR; }
        const SuffixHexFormatter _hex{'h'};
    } PLUGINS{};
    return PLUGINS;
}

DisTms320f::DisTms320f(const ValueFormatter::Plugins &plugins)
    : Disassembler(plugins), Config(TABLE) {
    reset();
}

void DisTms320f::decodeRelative(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    if ((insn.opCode() & (UINT32_C(1) << 25)) == 0) {
        const auto reg = decodeRegName(insn.opCode() & UINT16_MAX);
        if (reg == REG_UNDEF) {
            insn.setErrorIf(out, ILLEGAL_REGISTER);
        } else {
            outRegName(out, reg);
        }
        return;
    }
    const auto delta = static_cast<int16_t>(insn.opCode());
    const auto base = insn.address() + (mode == M_DREL ? 3 : 1);
    const auto target = base + delta;
    insn.setErrorIf(out, checkAddr(target));
    outRelAddr(out, target, insn.address(), 16);
}

void DisTms320f::decodeAbsolute(DisInsn &insn, StrBuffer &out) const {
    const auto target = insn.opCode() & UINT32_C(0x00FFFFFF);
    insn.setErrorIf(out, checkAddr(target));
    outAbsAddr(out, target);
}

RegName DisTms320f::decodeIndirect(DisInsn &insn, StrBuffer &out, uint_fast8_t mod,
        uint_fast8_t arn, int_fast16_t disp) const {
    const auto ar = RegName(REG_AR0 + arn);
    const auto defdisp = disp < 0;
    const auto update = mod < 0x18 && (mod & 7) >= 2;
    if (mod < 0x18) {
        const auto index = mod < 0x08 ? REG_UNDEF : (mod < 0x10 ? REG_IR0 : REG_IR1);
        out.letter('*');
        if (mod & 4)
            outRegName(out, ar);
        const auto incdec = (mod & 1) == 0 ? '+' : '-';
        out.letter(incdec);
        if (update)
            out.letter(incdec);
        if ((mod & 4) == 0)
            outRegName(out, ar);
        if (index != REG_UNDEF) {
            outRegName(out.letter('('), index).letter(')');
        } else if (!defdisp && disp != 1) {
            // omit *+ARn(1), *-ARn(1), *ARn++(1), *ARn--(1), *++ARn(1), *--ARn(1)
            out.letter('(').dec(uint8_t(disp)).letter(')');
        }
        if ((mod & 7) >= 6)
            out.letter('%');
    } else if (mod == 0x18) {
        outRegName(out.letter('*'), ar);
    } else if (mod == 0x19) {
        outRegName(out.letter('*'), ar).text_P(PSTR("++(IR0)B"));
    } else {
        insn.setErrorIf(out, ILLEGAL_OPERAND_MODE);
    }
    return update ? ar : REG_UNDEF;
}

void DisTms320f::decodeGeneric(
        DisInsn &insn, StrBuffer &out, AddrMode mode, uint_fast8_t pos) const {
    const auto opr = static_cast<uint16_t>(insn.opCode());
    const auto gg = (insn.opCode() >> 21) & 3;
    auto reg = REG_UNDEF;
    switch (gg) {
    case 0:
        if (mode == M_MEM)
            insn.setErrorIf(out, REGISTER_NOT_ALLOWED);
        if (opr & ~0x1F) {
            insn.setErrorIf(out, ILLEGAL_REGISTER);
        } else if ((mode == M_FGEN || mode == M_FLDM) && opr >= 8) {
            insn.setErrorIf(out, ILLEGAL_REGISTER);
        }
        reg = decodeRegName(opr & 0x1F);
        outRegName(out, reg);
        if (pos == 1)
            insn.srcReg = reg;
        break;
    case 1:
        if (mode == M_MREG)
            insn.setErrorIf(out, ILLEGAL_OPERAND_MODE);
        outAbsAddr(out.letter('@'), opr);
        break;
    case 2:
        decodeIndirect(insn, out, (opr >> 11) & 0x1F, (opr >> 8) & 7, opr & 0xFF);
        break;
    case 3:
        if (mode == M_MREG) {
            insn.setErrorIf(out, ILLEGAL_OPERAND_MODE);
            mode = M_IGEN;
        }
        if (mode == M_MEM) {
            insn.setErrorIf(out, ILLEGAL_OPERAND_MODE);
            mode = (insn.mode1() == M_FREG || insn.mode2() == M_FREG) ? M_FGEN : M_IGEN;
        }
        if (mode == M_FGEN || mode == M_FLDM) {
            if (mode == M_FLDM && (opr & ~0xFFF) != 0)
                insn.setErrorIf(out, ILLEGAL_OPERAND);
            ti_float16_t f16;
            f16.set(opr);
            const auto f80 = static_cast<float80_t>(f16);
            out.float80(f80, 5);
        } else if (mode == M_GCNT) {
            outDec(out, opr, -7);
        } else {
            const auto bit = (mode == M_UGEN) ? 16 : -16;
            outHex(out, opr, bit);
        }
        break;
    }
}

RegName DisTms320f::decode3OpGeneric(
        DisInsn &insn, StrBuffer &out, AddrMode mode, bool indir, uint_fast8_t modar) const {
    if (indir) {
        const auto mod = modar >> 3;
        const auto ar = modar & 7;
        return decodeIndirect(insn, out, mod, ar, /* default */ -1);
    }
    auto reg = decodeRegName(modar);
    if (reg == REG_UNDEF) {
        insn.setErrorIf(out, ILLEGAL_REGISTER);
    } else if (mode == M_FDAT && reg >= REG_AR0) {
        insn.setErrorIf(out, ILLEGAL_REGISTER);
    }
    outRegName(out, reg);
    return REG_UNDEF;
}

RegName DisTms320f::decodeRegister(DisInsn &insn, OprPos pos) const {
    const auto opc = insn.opCode();
    if (insn.isParallel()) {
        switch (pos) {
        case P_0040:
            return decodeRegName(((opc >> 22) & 1) + 2);
        case P_0080:
            return decodeRegName((opc >> 23) & 1);
        case P_0007:
            return decodeRegName((opc >> 16) & 7);
        case P_0038:
            return decodeRegName((opc >> 19) & 7);
        case P_01C0:
            return decodeRegName((opc >> 22) & 7);
        case P_00FF:  // M_IIDR/M_FIDR
            return ((opc >> 5) & 7) == 7 ? decodeRegName((opc >> 0) & 0x1F) : REG_UNDEF;
        case P_FF00:  // M_IIDR/M_FIDR
            return ((opc >> 13) & 7) == 7 ? decodeRegName((opc >> 8) & 0x1F) : REG_UNDEF;
        default:
            return REG_UNDEF;
        }
    }
    return decodeRegName((opc >> 16) & 0x1F);
}

void DisTms320f::decodeOperand(
        DisInsn &insn, StrBuffer &out, AddrMode mode, OprPos pos, uint_fast8_t no) const {
    const auto opc = insn.opCode();
    auto opr = opc & UINT16_MAX;
    const auto reg = decodeRegister(insn, pos);
    switch (mode) {
    case R_R01:
    case R_R23:
    case M_FREG:
        if (reg > REG_R7)
            insn.setErrorIf(out, ILLEGAL_REGISTER);
        // Fall-through
    case R_DP:
    case M_IREG:
        if (reg == REG_UNDEF) {
            insn.setErrorIf(out, ILLEGAL_REGISTER);
        } else if (!_gnuAs && insn.maybeUnary() && no == 2 && reg == insn.srcReg) {
            ;  // omit destination register
        } else {
            if (no == 2)
                out.comma();
            if (_paraDstReg != REG_UNDEF) {
                if (no == 2 && insn.mode3() == M_NONE && _paraDstReg == reg)
                    insn.setErrorIf(out, DUPLICATE_REGISTER);
            }
            outRegName(out, reg);
        }
        break;
    case M_MEM:
    case M_IGEN:
    case M_UGEN:
    case M_FGEN:
    case M_FLDM:
    case M_GCNT:
    case M_MREG:
        if (no == 2)
            out.comma();
        decodeGeneric(insn, out, mode, no);
        break;
    case M_IDAT:
    case M_FDAT:
        if (no == 1) {
            // src2
            insn.src2AReg = decode3OpGeneric(insn, out, mode, opc & (UINT32_C(1) << 22), opr);
        } else if (no == 2) {
            // src1
            const auto at = out.comma().mark();
            const auto ar = decode3OpGeneric(insn, out, mode, opc & (UINT32_C(1) << 21), opr >> 8);
            if (ar != REG_UNDEF && ar == insn.src2AReg)
                insn.setErrorIf(at, REGISTERS_OVERLAPPED);
        }
        break;
    case M_IDIR:
    case M_IIDR:
    case M_FIDR:
        if (no == 2)
            out.comma();
        if (pos == P_FF00) {
            if (mode != M_IDIR && ((opc >> 13) & 7) == 7) {
                if (reg == REG_UNDEF)
                    insn.setErrorIf(out, ILLEGAL_REGISTER);
                if (mode == M_FIDR && !isFloatReg(reg))
                    insn.setErrorIf(out, REGISTER_NOT_ALLOWED);
                outRegName(out, reg);
                break;
            }
            decodeIndirect(insn, out, (opc >> 11) & 0x1F, (opc >> 8) & 7, -1);
        } else {  // P_00FF
            if (mode != M_IDIR && ((opc >> 5) & 7) == 7) {
                if (reg == REG_UNDEF)
                    insn.setErrorIf(out, ILLEGAL_REGISTER);
                if (mode == M_FIDR && !isFloatReg(reg))
                    insn.setErrorIf(out, REGISTER_NOT_ALLOWED);
                outRegName(out, reg);
                break;
            }
            decodeIndirect(insn, out, (opc >> 3) & 0x1F, opc & 7, -1);
        }
        break;
    case M_MSBA:
        if ((opr & ~UINT8_MAX) != 0)
            insn.setErrorIf(out, OVERFLOW_RANGE);
        if (no == 2)
            out.comma();
        outHex(out, opr << 16, 24);
        break;
    case M_IREL:
    case M_DREL:
        if (no == 2)
            out.comma();
        decodeRelative(insn, out, mode);
        break;
    case M_AD24:
        decodeAbsolute(insn, out);
        break;
    case R_ARN:
        outRegName(out, RegName(REG_AR0 + ((opc >> 22) & 7)));
        break;
    case M_TVEC:
        if ((opr & 0xFFE0) == 0x0020) {
            opr &= ~0x20;
        } else {
            insn.setErrorIf(out, ILLEGAL_OPERAND);
        }
        outDec(out, opr, 16);
        break;
    default:
        break;
    }
}

void DisInsn::bytesToOpCode() {
    const auto *ptr = bytes();
    Config::opcode_t opc = *ptr++;
    opc |= static_cast<uint16_t>(*ptr++) << 8;
    opc |= static_cast<uint32_t>(*ptr++) << 16;
    opc |= static_cast<uint32_t>(*ptr++) << 24;
    setOpCode(opc);
}

Error DisTms320f::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) const {
    DisInsn insn(_insn, memory, out);
    if (insn.hasContinue()) {
        insn.bytesToOpCode();
    } else {
        insn.setOpCode(insn.readUint32());
    }
    if (searchOpCode(cpuType(), insn, out))
        return _insn.setError(insn);

    if (insn.codeFormat() != CF_00 && insn.lswFormat() == LF_00 &&
            (insn.opCode() & UINT16_MAX) != 0)
        insn.setErrorIf(out, ILLEGAL_OPERAND);
    decodeOperand(insn, out, insn.mode1(), insn.pos1(), 1);
    const auto mode2 = insn.mode2();
    if (mode2 != M_NONE) {
        decodeOperand(insn, out, mode2, insn.pos2(), 2);
        const auto mode3 = insn.mode3();
        if (mode3 != M_NONE)
            decodeOperand(insn, out.comma(), mode3, insn.pos3(), 3);
    }

    if (insn.hasContinue()) {
        insn.setContinueMark_P(nullptr);
        _paraDstReg = REG_UNDEF;
    } else if (insn.isParallel()) {
        insn.setContinueMark_P(TEXT_PARALLEL);
        if (insn.mode3() == M_NONE && mode2 == M_FREG)
            _paraDstReg = decodeRegister(insn, insn.pos2());
    } else {
        _paraDstReg = REG_UNDEF;
    }
    return _insn.setError(insn);
}

}  // namespace tms320f
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

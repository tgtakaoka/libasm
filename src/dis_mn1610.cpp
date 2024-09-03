/*
 * Copyright 2021 Tadashi G. Takaoka
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

#include "dis_mn1610.h"

#include "reg_mn1610.h"
#include "table_mn1610.h"
#include "text_common.h"

namespace libasm {
namespace mn1610 {

using namespace reg;
using namespace text::common;

const ValueFormatter::Plugins &DisMn1610::defaultPlugins() {
    static const struct final : ValueFormatter::Plugins {
        const HexFormatter &hex() const override { return _hex; }
        char locationSymbol() const override { return '*'; }
        const /*PROGMEM*/ char *lineComment_P() const override { return PSTR_STAR; }
        const SurroundHexFormatter _hex{PSTR_X_DASH, '\''};
    } PLUGINS{};
    return PLUGINS;
}

DisMn1610::DisMn1610(const ValueFormatter::Plugins &plugins)
    : Disassembler(plugins), Config(TABLE) {
    reset();
}

void DisMn1610::outConditionCode(StrBuffer &out, CcName cc) const {
    if (cc != CC_NONE)
        outCcName(out, cc);
}

StrBuffer &DisMn1610::outRegister(StrBuffer &out, RegName reg, AddrMode mode) const {
    if (reg == REG_CSBR && mode == M_SB)
        return out;
    if (reg == REG_SIR && mode == M_RHW)
        reg = REG_SOR;
    return outRegName(out, reg);
}

void DisMn1610::outGenericAddr(StrBuffer &out, Config::opcode_t opc, Config::uintptr_t base) const {
    const uint8_t addr = opc & 0xFF;
    const auto mode = (opc >> 11) & 7;
    const Config::uintptr_t target = base + static_cast<int8_t>(addr);
    switch (mode) {
    case 2:
        out.letter('(');
        // Fall-through
    case 0:
        outAbsAddr(out, addr, 8);
        if (mode == 2)
            out.letter(')');
        break;
    case 3:
        out.letter('(');
        // Fall-through
    case 1:
        if (_relativeTarget) {
            outRelAddr(out, target, base, 8);
        } else {
            outAbsAddr(out, target).letter('-').letter(_curSym);
            outRegName(out.letter('('), REG_IC).letter(')');
        }
        if (mode == 3)
            out.letter(')');
        break;
    case 4:
    case 5:
        outDec(out, addr, 8);
        goto print_index;
    case 6:
    case 7:
        outAbsAddr(out.letter('('), addr, 8).letter(')');
    print_index:
        outRegName(out.letter('('), (mode == 4 || mode == 6) ? REG_X0 : REG_X1).letter(')');
        break;
    }
}

void DisMn1610::outIndirect(StrBuffer &out, Config::opcode_t opc) const {
    const auto mode = (opc >> 6) & 3;
    if (mode == 2)
        out.letter('-');
    outRegName(out.letter('('), decodeIndirect(opc)).letter(')');
    if (mode == 3)
        out.letter('+');
}

void DisMn1610::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode) const {
    auto opc = insn.opCode();
    switch (mode) {
    case M_SKIP:
        outConditionCode(out, decodeSkip(opc >> 4));
        break;
    case M_RD:
    case M_RDG:
        outRegister(out, decodeRegNum(opc >> 8), mode);
        break;
    case M_RS:
    case M_RSG:
        outRegister(out, decodeRegNum(opc), mode);
        break;
    case M_GEN:
        outGenericAddr(out, opc, insn.address());
        break;
    case M_EOP:
        outConditionCode(out, decodeEop(opc));
        break;
    case M_IM4:
    case M_BIT:
        outDec(out, opc & 0xF, 4);
        break;
    case M_IM8W:
        opc = insn.readUint16();
        // Fall-through
    case M_IM8:
        outDec(out, opc & 0xFF, 8);
        break;
    case M_IOA:
        outAbsAddr(out, opc & 0xFF, 8);
        break;
    case M_ILVL:
        outDec(out, opc & 3, 2);
        break;
    case M_IM16:
        outHex(out, insn.readUint16(), 16);
        break;
    case M_ABS:
        outAbsAddr(out, insn.readUint16(), 16);
        break;
    case M_R0:
        outRegister(out, REG_R0, mode);
        break;
    case M_DR0:
        outRegister(out, REG_DR0, mode);
        break;
    case M_RI1:
    case M_RI2:
        outRegister(out.letter('('), mode == M_RI1 ? REG_R1 : REG_R2, mode).letter(')');
        break;
    case M_RI:
        outRegister(out.letter('('), decodeIndirect(opc), mode).letter(')');
        break;
    case M_RIAU:
        outIndirect(out, opc);
        break;
    case M_SB:
        if (decodeSegment((opc >> 4) & 3) != REG_CSBR)
            outRegister(out, decodeSegment((opc >> 4) & 3), mode);
        break;
    case M_IABS:
        out.letter('(');
        outAbsAddr(out, insn.readUint16(), 16).letter(')');
        break;
    case M_COP:
        outConditionCode(out, decodeCop(opc >> 3));
        break;
    case M_RBW:
    case M_RB:
        outRegister(out, decodeSegment(opc >> 4), mode);
        break;
    case M_RHW:
    case M_RHR:
        outRegister(out, decodeHardware(opc >> 4), mode);
        break;
    case M_RP:
        outRegister(out, decodeSpecial(opc >> 4), mode);
        break;
    default:
        break;
    }
}

StrBuffer &DisMn1610::outComma(StrBuffer &out, Config::opcode_t opc, AddrMode mode) const {
    if (mode == M_SKIP && decodeSkip(opc >> 4) == CC_NONE)
        return out;
    if (mode == M_EOP && decodeEop(opc) == CC_NONE)
        return out;
    if (mode == M_COP && decodeCop(opc >> 3) == CC_NONE)
        return out;
    if (mode == M_SB && decodeSegment((opc >> 4) & 3) == REG_CSBR)
        return out;
    return out.comma();
}

Error DisMn1610::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) const {
    DisInsn insn(_insn, memory, out);
    const auto opc = insn.readUint16();
    insn.setOpCode(opc);
    if (TABLE.searchOpCode(cpuType(), insn, out))
        return _insn.setError(insn);

    decodeOperand(insn, out, insn.mode1());
    const auto mode2 = insn.mode2();
    if (mode2 != M_NONE) {
        decodeOperand(insn, outComma(out, opc, mode2), mode2);
        const auto mode3 = insn.mode3();
        if (mode3 != M_NONE) {
            decodeOperand(insn, outComma(out, opc, mode3), mode3);
            const auto mode4 = insn.mode4();
            if (mode4 != M_NONE)
                decodeOperand(insn, outComma(out, opc, mode4), mode4);
        }
    }
    return _insn.setError(insn);
}

}  // namespace mn1610
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

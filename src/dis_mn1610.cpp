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

namespace libasm {
namespace mn1610 {

using namespace reg;

DisMn1610::DisMn1610() : Disassembler(_formatter, TableMn1610::TABLE, '*'), _formatter(true) {
    reset();
}

AddressWidth DisMn1610::addressWidth() const {
    return TableMn1610::TABLE.addressWidth();
}

Error DisMn1610::outConditionCode(StrBuffer &out, CcName cc) {
    if (cc == CC_UNDEF)
        return setError(ILLEGAL_OPERAND);
    if (cc != CC_NONE)
        outCcName(out, cc);
    return OK;
}

Error DisMn1610::outRegister(StrBuffer &out, RegName reg, AddrMode mode) {
    if (reg == REG_UNDEF)
        return setError(ILLEGAL_REGISTER);
    if (reg == REG_STR && (mode == M_RDG || mode == M_RSG))
        return setError(ILLEGAL_REGISTER);
    if (reg == REG_CSBR && mode == M_RBW)
        return setError(REGISTER_NOT_ALLOWED);
    if (reg == REG_CSBR && mode == M_SB)
        return OK;
    if (reg == REG_SIR && mode == M_RHW)
        reg = REG_SOR;
    outRegName(out, reg);
    return OK;
}

Error DisMn1610::outGenericAddr(StrBuffer &out, Config::opcode_t opc, Config::uintptr_t base) {
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
    return OK;
}

Error DisMn1610::decodeOperand(DisMemory &memory, InsnMn1610 &insn, StrBuffer &out, AddrMode mode) {
    auto opc = insn.opCode();
    switch (mode) {
    case M_SKIP:
        return outConditionCode(out, decodeSkip(opc >> 4));
    case M_RD:
    case M_RDG:
        return outRegister(out, decodeRegNum(opc >> 8), mode);
    case M_RS:
    case M_RSG:
        return outRegister(out, decodeRegNum(opc), mode);
    case M_GEN:
        return outGenericAddr(out, opc, insn.address());
    case M_EOP:
        return outConditionCode(out, decodeEop(opc));
    case M_IM4:
    case M_BIT:
        outDec(out, opc & 0xF, 4);
        return OK;
    case M_IM8W:
        opc = insn.readUint16(memory);
        // Fall-through
    case M_IM8:
        outDec(out, opc & 0xFF, 8);
        return OK;
    case M_IOA:
        outAbsAddr(out, opc & 0xFF, 8);
        return OK;
    case M_ILVL:
        outDec(out, opc & 3, 2);
        return OK;
    case M_IM16:
    case M_ABS:
        outAbsAddr(out, insn.readUint16(memory), 16);
        return OK;
    case M_R0:
        return outRegister(out, REG_R0, mode);
    case M_DR0:
        return outRegister(out, REG_DR0, mode);
    case M_RI1:
    case M_RI2:
        outRegister(out.letter('('), mode == M_RI1 ? REG_R1 : REG_R2, mode);
        out.letter(')');
        return OK;
    case M_RI:
        outRegister(out.letter('('), decodeIndirect(opc), mode);
        out.letter(')');
        return OK;
    case M_RIAU:
        return outIndirect(out, opc);
    case M_SB:
        if (decodeSegment((opc >> 4) & 3) == REG_CSBR)
            return OK;  // Can be omitted.
        return outRegister(out, decodeSegment((opc >> 4) & 3), mode);
    case M_IABS:
        outAbsAddr(out.letter('('), insn.readUint16(memory), 16);
        out.letter(')');
        return OK;
    case M_COP:
        return outConditionCode(out, decodeCop(opc >> 3));
    case M_RBW:
    case M_RB:
        return outRegister(out, decodeSegment(opc >> 4), mode);
    case M_RHW:
    case M_RHR:
        return outRegister(out, decodeHardware(opc >> 4), mode);
    case M_RP:
        return outRegister(out, decodeSpecial(opc >> 4), mode);
    default:
        return OK;
    }
}

Error DisMn1610::outIndirect(StrBuffer &out, Config::opcode_t opc) {
    const auto mode = (opc >> 6) & 3;
    if (mode == 0)
        return setError(ILLEGAL_OPERAND_MODE);
    if (mode == 2)
        out.letter('-');
    outRegName(out.letter('('), decodeIndirect(opc)).letter(')');
    if (mode == 3)
        out.letter('+');
    return OK;
}

StrBuffer &DisMn1610::outComma(StrBuffer &out, Config::opcode_t opc, AddrMode mode) {
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

Error DisMn1610::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) {
    InsnMn1610 insn(_insn);
    const auto opc = insn.readUint16(memory);

    insn.setOpCode(opc);
    if (TableMn1610::TABLE.searchOpCode(insn, out))
        return setError(insn);
    if (setError(insn))
        return getError();

    const auto mode1 = insn.mode1();
    if (mode1 == M_NONE)
        return setOK();
    if (decodeOperand(memory, insn, out, mode1))
        return getError();
    const auto mode2 = insn.mode2();
    if (mode2 == M_NONE)
        return setOK();
    if (decodeOperand(memory, insn, outComma(out, opc, mode2), mode2))
        return getError();
    const auto mode3 = insn.mode3();
    if (mode3 == M_NONE)
        return setOK();
    if (decodeOperand(memory, insn, outComma(out, opc, mode3), mode3))
        return getError();
    const auto mode4 = insn.mode4();
    if (mode4 == M_NONE)
        return setOK();
    return decodeOperand(memory, insn, outComma(out, opc, mode4), mode4);
}

}  // namespace mn1610
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

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

#include "table_mn1610.h"

namespace libasm {
namespace mn1610 {

Error DisMn1610::outCcName(StrBuffer &out, CcName cc) {
    if (cc == CC_UNDEF)
        return setError(ILLEGAL_OPERAND);
    if (cc != CC_NONE)
        _regs.outCcName(out, cc);
    return OK;
}

Error DisMn1610::outRegName(StrBuffer &out, RegName reg, AddrMode mode) {
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
    _regs.outRegName(out, reg);
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
            outAbsAddr(out, target).letter('-').letter('*');
            _regs.outRegName(out.letter('('), REG_IC).letter(')');
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
        _regs.outRegName(out.letter('('), (mode == 4 || mode == 6) ? REG_X0 : REG_X1).letter(')');
        break;
    }
    return OK;
}

Error DisMn1610::decodeOperand(DisMemory &memory, InsnMn1610 &insn, StrBuffer &out, AddrMode mode) {
    auto opc = insn.opCode();
    switch (mode) {
    case M_SKIP:
        return outCcName(out, RegMn1610::decodeSkip(opc >> 4));
    case M_RD:
    case M_RDG:
        return outRegName(out, RegMn1610::decodeRegNum(opc >> 8), mode);
    case M_RS:
    case M_RSG:
        return outRegName(out, RegMn1610::decodeRegNum(opc), mode);
    case M_GEN:
        return outGenericAddr(out, opc, insn.address());
    case M_EOP:
        return outCcName(out, RegMn1610::decodeEop(opc));
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
        return outRegName(out, REG_R0, mode);
    case M_DR0:
        return outRegName(out, REG_DR0, mode);
    case M_RI1:
    case M_RI2:
        outRegName(out.letter('('), mode == M_RI1 ? REG_R1 : REG_R2, mode);
        out.letter(')');
        return OK;
    case M_RI:
        outRegName(out.letter('('), RegMn1610::decodeIndirect(opc), mode);
        out.letter(')');
        return OK;
    case M_RIAU:
        return outIndirect(out, opc);
    case M_SB:
        if (RegMn1610::decodeSegment((opc >> 4) & 3) == REG_CSBR)
            return OK;  // Can be omitted.
        return outRegName(out, RegMn1610::decodeSegment((opc >> 4) & 3), mode);
    case M_IABS:
        outAbsAddr(out.letter('('), insn.readUint16(memory), 16);
        out.letter(')');
        return OK;
    case M_COP:
        return outCcName(out, RegMn1610::decodeCop(opc >> 3));
    case M_RBW:
    case M_RB:
        return outRegName(out, RegMn1610::decodeSegment(opc >> 4), mode);
    case M_RHW:
    case M_RHR:
        return outRegName(out, RegMn1610::decodeHardware(opc >> 4), mode);
    case M_RP:
        return outRegName(out, RegMn1610::decodeSpecial(opc >> 4), mode);
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
    _regs.outRegName(out.letter('('), RegMn1610::decodeIndirect(opc)).letter(')');
    if (mode == 3)
        out.letter('+');
    return OK;
}

StrBuffer &DisMn1610::outComma(StrBuffer &out, Config::opcode_t opc, AddrMode mode) {
    if (mode == M_SKIP && RegMn1610::decodeSkip(opc >> 4) == CC_NONE)
        return out;
    if (mode == M_EOP && RegMn1610::decodeEop(opc) == CC_NONE)
        return out;
    if (mode == M_COP && RegMn1610::decodeCop(opc >> 3) == CC_NONE)
        return out;
    if (mode == M_SB && RegMn1610::decodeSegment((opc >> 4) & 3) == REG_CSBR)
        return out;
    return out.comma();
}

Error DisMn1610::decode(DisMemory &memory, Insn &_insn, StrBuffer &out) {
    InsnMn1610 insn(_insn);
    const auto opc = insn.readUint16(memory);

    insn.setOpCode(opc);
    if (TableMn1610.searchOpCode(insn))
        return setError(TableMn1610.getError());
    if (setError(insn))
        return getError();

    const auto op1 = insn.op1();
    if (op1 == M_NO)
        return setOK();
    if (decodeOperand(memory, insn, out, op1))
        return getError();
    const auto op2 = insn.op2();
    if (op2 == M_NO)
        return setOK();
    if (decodeOperand(memory, insn, outComma(out, opc, op2), op2))
        return getError();
    const auto op3 = insn.op3();
    if (op3 == M_NO)
        return setOK();
    if (decodeOperand(memory, insn, outComma(out, opc, op3), op3))
        return getError();
    const auto op4 = insn.op4();
    if (op4 == M_NO)
        return setOK();
    return decodeOperand(memory, insn, outComma(out, opc, op4), op4);
}

}  // namespace mn1610
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

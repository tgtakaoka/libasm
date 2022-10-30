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

#include "dis_tlcs90.h"

#include "reg_tlcs90.h"
#include "table_tlcs90.h"

namespace libasm {
namespace tlcs90 {

Error DisTlcs90::readOperand(DisMemory &memory, InsnTlcs90 &insn, AddrMode mode, Operand &op) {
    const Config::opcode_t opc = insn.opCode();
    op.mode = mode;
    switch (mode) {
    case M_BIT:
        op.val16 = opc & 7;
        break;
    case M_IMM8:
    case M_REL8:
    case M_DIR:
        op.val16 = insn.readByte(memory);
        break;
    case M_IMM16:
    case M_REL16:
    case M_EXT:
        op.val16 = insn.readUint16(memory);
        break;
    case M_CC:
        op.cc = RegTlcs90::decodeCcName(opc);
        break;
    case M_STACK:
        op.reg = RegTlcs90::decodeStackReg(opc);
        if (op.reg == REG_UNDEF)
            return setError(UNKNOWN_INSTRUCTION);
        break;
    case M_REG8:
        op.reg = RegTlcs90::decodeReg8(opc);
        if (op.reg == REG_UNDEF)
            return setError(UNKNOWN_INSTRUCTION);
        break;
    case M_REG16:
        op.reg = RegTlcs90::decodeReg16(opc);
        if (op.reg == REG_UNDEF)
            return setError(UNKNOWN_INSTRUCTION);
        break;
    case M_REGIX:
        op.reg = RegTlcs90::decodeIndexReg(opc);
        if (op.reg == REG_UNDEF)
            return setError(UNKNOWN_INSTRUCTION);
        break;
    default:
        break;
    }
    return setError(insn);
}

Error DisTlcs90::decodeRelative(
        InsnTlcs90 &insn, StrBuffer &out, AddrMode mode, const Operand &op) {
    Config::ptrdiff_t delta;
    if (mode == M_REL8) {
        delta = static_cast<int8_t>(op.val16);
    } else {
        delta = static_cast<int16_t>(op.val16);
    }
    const Config::uintptr_t target = insn.address() + 2 + delta;
    const uint8_t deltaWidth = mode == M_REL8 ? 8 : 16;
    outRelAddr(out, target, insn.address(), deltaWidth);
    return OK;
}

Error DisTlcs90::decodeOperand(InsnTlcs90 &insn, StrBuffer &out, AddrMode mode, const Operand &op) {
    uint16_t val16 = op.val16;
    int8_t val8 = static_cast<int8_t>(val16);
    switch (mode) {
    case M_IMM8:
    case M_BIT:
        outHex(out, val16, 8);
        break;
    case M_IMM16:
        outHex(out, val16, 16);
        break;
    case M_DIR:
        val16 |= 0xFF00;
        /* Fall-through */
    case M_EXT:
        outHex(out.letter('('), val16, 16).letter(')');
        break;
    case M_REL8:
    case M_REL16:
        return decodeRelative(insn, out, mode, op);
    case M_IND:
        _regs.outRegName(out.letter('('), op.reg).letter(')');
        break;
    case M_IDX:
        _regs.outRegName(out.letter('('), op.reg);
        if (val8 >= 0)
            out.letter('+');
        outHex(out, val8, -8).letter(')');
        break;
    case M_BASE:
        _regs.outRegName(out.letter('('), REG_HL).letter('+');
        _regs.outRegName(out, REG_A).letter(')');
        break;
    case M_CC:
        _regs.outCcName(out, op.cc);
        break;
    case M_STACK:
    case M_REG8:
    case M_REG16:
    case M_REGIX:
        _regs.outRegName(out, op.reg);
        break;
    case R_BC:
    case R_DE:
    case R_HL:
    case R_SP:
    case R_AF:
    case R_AFP:
    case R_A:
        _regs.outRegName(out, RegName(uint8_t(mode) - 16));
        break;
    default:
        break;
    }
    return OK;
}

Error DisTlcs90::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) {
    InsnTlcs90 insn(_insn);
    Operand preOp;
    if (TableTlcs90::TABLE.readInsn(memory, insn, preOp))
        return setError(TableTlcs90::TABLE.getError());
    if (TableTlcs90::TABLE.searchOpCode(insn))
        return setError(TableTlcs90::TABLE.getError());
    const AddrMode dst = insn.dstMode();
    if (dst == M_NONE)
        return OK;
    const char *start = out.mark();
    if (dst == M_DST) {
        decodeOperand(insn, out, preOp.mode, preOp);
    } else {
        Operand op;
        if (readOperand(memory, insn, dst, op) == OK)
            decodeOperand(insn, out, dst, op);
    }
    if (getError())
        return getError();
    const AddrMode src = insn.srcMode();
    if (src == M_NONE)
        return OK;
    if (src == M_SRC16) {
        if (insn.prefix() == 0xFB)
            return setError(UNKNOWN_INSTRUCTION);
        preOp.mode = M_REG16;
        preOp.reg = RegTlcs90::decodeReg16(insn.prefix());
    }
    if (out.mark() != start)  // skip CC_T because it's empty.
        out.comma();
    if (src == M_SRC || src == M_SRC16) {
        decodeOperand(insn, out, preOp.mode, preOp);
    } else {
        Operand op;
        if (readOperand(memory, insn, src, op) == OK)
            decodeOperand(insn, out, src, op);
    }
    return getError();
}

}  // namespace tlcs90
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

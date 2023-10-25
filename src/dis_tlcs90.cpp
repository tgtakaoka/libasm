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

using namespace reg;

const ValueFormatter::Plugins &DisTlcs90::defaultPlugins() {
    return ValueFormatter::Plugins::intel();
}

DisTlcs90::DisTlcs90(const ValueFormatter::Plugins &plugins)
    : Disassembler(plugins), Config(TABLE) {
    reset();
}

Error DisTlcs90::readOperand(DisInsn &insn, AddrMode mode, Operand &op) const {
    const auto opc = insn.opCode();
    op.mode = mode;
    switch (mode) {
    case M_BIT:
        op.val16 = opc & 7;
        break;
    case M_IMM8:
    case M_REL8:
    case M_DIR:
        op.val16 = insn.readByte();
        break;
    case M_IMM16:
    case M_REL16:
    case M_EXT:
        op.val16 = insn.readUint16();
        break;
    case M_CC:
        op.cc = decodeCcName(opc);
        break;
    case M_STACK:
        op.reg = decodeStackReg(opc);
        if (op.reg == REG_UNDEF)
            op.setError(UNKNOWN_INSTRUCTION);
        break;
    case M_REG8:
        op.reg = decodeReg8(opc);
        if (op.reg == REG_UNDEF)
            op.setError(UNKNOWN_INSTRUCTION);
        break;
    case M_REG16:
        op.reg = decodeReg16(opc);
        if (op.reg == REG_UNDEF)
            op.setError(UNKNOWN_INSTRUCTION);
        break;
    case M_REGIX:
        op.reg = decodeIndexReg(opc);
        if (op.reg == REG_UNDEF)
            op.setError(UNKNOWN_INSTRUCTION);
        break;
    default:
        break;
    }
    return op.getError();
}

void DisTlcs90::decodeRelative(
        DisInsn &insn, StrBuffer &out, AddrMode mode, const Operand &op) const {
    const auto delta =
            (mode == M_REL8) ? static_cast<int8_t>(op.val16) : static_cast<int16_t>(op.val16);
    const auto base = insn.address() + 2;
    Error error;
    const auto target = branchTarget(base, delta, error);
    if (error)
        insn.setErrorIf(out, error);
    outRelAddr(out, target, insn.address(), mode == M_REL8 ? 8 : 16);
}

void DisTlcs90::decodeOperand(
        DisInsn &insn, StrBuffer &out, AddrMode mode, const Operand &op) const {
    auto val16 = op.val16;
    auto val8 = static_cast<int8_t>(val16);
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
        decodeRelative(insn, out, mode, op);
        break;
    case M_IND:
        outRegName(out.letter('('), op.reg).letter(')');
        break;
    case M_IDX:
        outRegName(out.letter('('), op.reg);
        if (val8 >= 0)
            out.letter('+');
        outHex(out, val8, -8).letter(')');
        break;
    case M_BASE:
        outRegName(out.letter('('), REG_HL).letter('+');
        outRegName(out, REG_A).letter(')');
        break;
    case M_CC:
        outCcName(out, op.cc);
        break;
    case M_STACK:
    case M_REG8:
    case M_REG16:
    case M_REGIX:
        outRegName(out, op.reg);
        break;
    case R_BC:
    case R_DE:
    case R_HL:
    case R_SP:
    case R_AF:
    case R_AFP:
    case R_A:
        outRegName(out, RegName(uint8_t(mode) - 16));
        break;
    default:
        break;
    }
}

Error DisTlcs90::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) {
    DisInsn insn(_insn, memory, out);
    const auto opc = insn.readByte();
    insn.setOpCode(opc);
    Operand prefixOp;
    if (TABLE.isPrefix(cpuType(), opc, prefixOp.mode)) {
        if (insn.readOpCode(prefixOp))
            return setError(insn);
    }
    if (TABLE.searchOpCode(cpuType(), insn, prefixOp, out))
        return setError(insn);

    const auto dst = insn.dst();
    if (dst != M_NONE) {
        const auto *start = out.mark();
        if (dst == M_DST) {
            decodeOperand(insn, out, prefixOp.mode, prefixOp);
        } else {
            Operand op;
            if (readOperand(insn, dst, op))
                insn.setErrorIf(out, op);
            decodeOperand(insn, out, dst, op);
        }
        const auto src = insn.src();
        if (src != M_NONE) {
            if (out.mark() != start)  // skip CC_T because it's empty.
                out.comma();
            if (src == M_SRC) {
                decodeOperand(insn, out, prefixOp.mode, prefixOp);
            } else {
                Operand op;
                if (readOperand(insn, src, op))
                    insn.setErrorIf(out, op);
                decodeOperand(insn, out, src, op);
            }
        }
    }
    return setError(insn);
}

}  // namespace tlcs90
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

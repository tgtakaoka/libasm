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

#include "asm_z8.h"
#include "table_z8.h"
#include "reg_z8.h"

namespace libasm {
namespace z8 {

Error AsmZ8::encodeOperand(InsnZ8 &insn, const Operand &op) {
    if (op.mode == M_NO) return getError();
    if (op.reg != REG_UNDEF
        && (op.mode == M_R || op.mode == M_IR || op.mode == M_IRR)) {
        insn.emitByte(RegZ8::encodeRegName(op.reg) | 0xE0);
        return getError();
    }
    if (op.val >= 0x100) return setError(OVERFLOW_RANGE);
    insn.emitByte(static_cast<uint8_t>(op.val));
    return getError();
}

Error AsmZ8::encodeAbsolute(
    InsnZ8 &insn, const Operand &dst, const Operand &src) {
    const Operand &op = (dst.mode == M_DA) ? dst : src;
    if (dst.mode == M_cc)
        insn.embed(RegZ8::encodeCcName(dst.cc) << 4);
    insn.emitInsn();
    insn.emitUint16(op.val);
    return op.getError();
}

Error AsmZ8::encodeRelative(
    InsnZ8 &insn, const Operand &dst, const Operand &src) {
    const Operand& op = (dst.mode == M_RA) ? dst : src;
    if (dst.mode == M_cc) {
        insn.embed(RegZ8::encodeCcName(dst.cc) << 4);
    } else if (dst.mode == M_r) {
        insn.embed(RegZ8::encodeRegName(dst.reg) << 4);
    }
    const Config::uintptr_t target = op.getError() ? insn.address() : op.val;
    const Config::uintptr_t base = insn.address() + 2;
    const Config::ptrdiff_t delta = target - base;
    if (delta < -128 || delta >= 128) return setErrorIf(OPERAND_TOO_FAR);
    insn.emitInsn();
    insn.emitByte(static_cast<uint8_t>(delta));
    return op.getError();
}

Error AsmZ8::encodeIndexed(
    InsnZ8 &insn, const Operand &dst, const Operand &src) {
    const RegName index = (dst.mode == M_X) ? dst.reg : src.reg;
    const uint16_t disp = (dst.mode == M_X) ? dst.val : src.val;
    const RegName reg = (dst.mode == M_X) ? src.reg : dst.reg;
    if (disp >= 0x100) return setError(OVERFLOW_RANGE);
    const uint8_t op1 = RegZ8::encodeRegName(index)
        | (RegZ8::encodeRegName(reg) << 4);
    insn.emitInsn();
    insn.emitByte(op1);
    insn.emitByte(static_cast<uint8_t>(disp));
    return getError();
}

Error AsmZ8::encodeIndirectRegPair(
    InsnZ8 &insn, const Operand &dst, const Operand &src) {
    const RegName pair = (dst.mode == M_Irr) ? dst.reg : src.reg;
    const RegName reg = (dst.mode == M_Irr) ? src.reg : dst.reg;
    const uint8_t opr = RegZ8::encodeRegName(pair)
        | (RegZ8::encodeRegName(reg) << 4);
    insn.emitInsn();
    insn.emitByte(opr);
    return getError();
}

Error AsmZ8::encodeInOpCode(
    InsnZ8 &insn, const Operand &dst, const Operand &src) {
    const RegName reg = (dst.mode == M_r) ? dst.reg : src.reg;
    const Operand &op = (dst.mode == M_r) ? src : dst;
    insn.embed(RegZ8::encodeRegName(reg) << 4);

    // In "LD r,R", Only a full 8-bit register address can be used
    if (op.mode == M_R && (op.val & 0xF0) == 0xE0)
        return setError(ILLEGAL_REGISTER);

    insn.emitInsn();
    return encodeOperand(insn, op);
}

Error AsmZ8::encodeTwoOperands(
    InsnZ8 &insn, const Operand &dst, const Operand &src) {
    insn.emitInsn();
    if (dst.mode == M_r || dst.mode == M_Ir) {
        if (src.mode == M_r || (dst.mode == M_r && src.mode == M_Ir)) {
            insn.emitByte(RegZ8::encodeRegName(src.reg)
                          | (RegZ8::encodeRegName(dst.reg) << 4));
            return getError();
        }
        return setError(INTERNAL_ERROR);
    }
    const Error error1 = encodeOperand(insn, (src.mode == M_IM) ? dst : src);
    const Error error2 = encodeOperand(insn, (src.mode == M_IM) ? src : dst);
    setErrorIf(error1);
    setErrorIf(error2);
    return getError();
}

Error AsmZ8::parseOperand(Operand &op) {
    setError(op.setOK());
    op.mode = M_NO;

    const char *p = _scan;
    if (endOfLine(p)) {
        op.mode = M_NO;
        return OK;
    }

    op.cc = _regs.parseCcName(p);
    if (op.cc != CC_UNDEF) {
        _scan = p + RegZ8::ccNameLen(op.cc);
        op.mode = M_cc;
        return OK;
    }

    if (*p == '#') {
        _scan = skipSpaces(p + 1);
        if (getOperand(op.val)) return getError();
        op.setError(getError());
        op.mode = M_IM;
        return OK;
    }

    const bool indir = (*p == '@');
    if (indir) p++;

    op.reg = _regs.parseRegName(p);
    if (op.reg != REG_UNDEF) {
        _scan = p + RegZ8::regNameLen(op.reg);
        const bool pair = RegZ8::isRegPair(op.reg);
        if (indir) {
            op.mode = pair ? M_Irr : M_Ir;
        } else {
            if (pair) return setError(UNKNOWN_OPERAND);
            op.mode = M_r;
        }
        return OK;
    }

    const bool regAddr = (*p == '>');
    if (regAddr) p++;
    _scan = p;
    if (getOperand(op.val)) return getError();
    op.setError(getError());
    p = skipSpaces(_scan);
    if (*p == '(') {
        if (indir || regAddr) setError(UNKNOWN_OPERAND);
        p++;
        op.reg = _regs.parseRegName(p);
        if (op.reg == REG_UNDEF || RegZ8::isRegPair(op.reg))
            return setError(UNKNOWN_OPERAND);
        p = skipSpaces(p + RegZ8::regNameLen(op.reg));
        if (*p != ')') return setError(MISSING_CLOSING_PAREN);
        _scan = p + 1;
        op.mode = M_X;
        return OK;
    }
    if (indir) {
        if (regAddr) return setError(UNKNOWN_OPERAND); // unnecessary
        if (op.val >= 0x100) return setError(OVERFLOW_RANGE);
        op.mode = (op.val & 1) == 0 ? M_IRR : M_IR;
        return OK;
    }
    if (regAddr) {
        if (op.val >= 0x100) return setError(OVERFLOW_RANGE);
        op.mode = M_R;
        return OK;
    }
    op.mode = M_DA;
    return OK;
}

Error AsmZ8::encode(Insn &_insn) {
    InsnZ8 insn(_insn);
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);
    _scan = skipSpaces(endName);

    Operand dstOp, srcOp;
    if (parseOperand(dstOp)) return getError();
    _scan = skipSpaces(_scan);
    if (*_scan == ',') {
        _scan = skipSpaces(_scan + 1);
        if (parseOperand(srcOp)) return getError();
    } else {
        srcOp.setOK();
        srcOp.mode = M_NO;
    }
    if (!endOfLine(skipSpaces(_scan))) return setError(GARBAGE_AT_END);
    setError(dstOp.getError());
    setErrorIf(srcOp.getError());

    insn.setAddrMode(dstOp.mode, srcOp.mode);
    if (TableZ8.searchName(insn))
        return setError(UNKNOWN_INSTRUCTION);
    const AddrMode dst = dstOp.mode = insn.dstMode();
    const AddrMode src = srcOp.mode = insn.srcMode();

    if (dst == M_NO) {
        insn.emitInsn();
        return setOK();
    }
    if (dst == M_DA || src == M_DA)
        return encodeAbsolute(insn, dstOp, srcOp);
    if (dst == M_RA || src == M_RA)
        return encodeRelative(insn, dstOp, srcOp);
    if (dst == M_X || src == M_X)
        return encodeIndexed(insn, dstOp, srcOp);
    if (dst == M_Irr || src == M_Irr)
        return encodeIndirectRegPair(insn, dstOp, srcOp);
    if (InsnZ8::operandInOpCode(insn.opCode()))
        return encodeInOpCode(insn, dstOp, srcOp);
    if (src == M_NO) {
        insn.emitInsn();
        return encodeOperand(insn, dstOp);
    }
    return encodeTwoOperands(insn, dstOp, srcOp);
}

} // namespace z8
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

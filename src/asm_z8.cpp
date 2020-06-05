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

Error AsmZ8::encodeOperand(
    InsnZ8 &insn, const AddrMode mode, const Operand &op) {
    if (mode == M_NO) return getError();
    if ((mode == M_R || mode == M_IR || mode == M_IRR)
        && op.reg != REG_UNDEF) {
        insn.emitByte(RegZ8::encodeRegName(op.reg) | 0xE0);
        return getError();
    }
    if (op.val >= 0x100) return setError(OVERFLOW_RANGE);
    insn.emitByte(static_cast<uint8_t>(op.val));
    return getError();
}

Error AsmZ8::encodeAbsolute(
    InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp) {
    const AddrMode dst = insn.dstMode();
    const Operand &op = (dst == M_DA) ? dstOp : srcOp;
    if (dst == M_cc)
        insn.embed(RegZ8::encodeCcName(dstOp.cc) << 4);
    insn.emitInsn();
    insn.emitUint16(op.val);
    return op.getError();
}

Error AsmZ8::encodeRelative(
    InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp) {
    const AddrMode dst = insn.dstMode();
    const Operand& op = (dst== M_RA) ? dstOp : srcOp;
    if (dst == M_cc) {
        insn.embed(RegZ8::encodeCcName(dstOp.cc) << 4);
    } else if (dst == M_r) {
        insn.embed(RegZ8::encodeRegName(dstOp.reg) << 4);
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
    InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp) {
    const AddrMode dst = insn.dstMode();
    const RegName index = (dst == M_X) ? dstOp.reg : srcOp.reg;
    const uint16_t disp = (dst == M_X) ? dstOp.val : srcOp.val;
    const RegName reg = (dst == M_X) ? srcOp.reg : dstOp.reg;
    if (disp >= 0x100) return setError(OVERFLOW_RANGE);
    const uint8_t op1 =
        RegZ8::encodeRegName(index) | (RegZ8::encodeRegName(reg) << 4);
    insn.emitInsn();
    insn.emitByte(op1);
    insn.emitByte(static_cast<uint8_t>(disp));
    return getError();
}

Error AsmZ8::encodeIndirectRegPair(
    InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp) {
    const AddrMode dst = insn.dstMode();
    const RegName pair = (dst == M_Irr) ? dstOp.reg : srcOp.reg;
    const RegName reg = (dst == M_Irr) ? srcOp.reg : dstOp.reg;
    const uint8_t opr =
        RegZ8::encodeRegName(pair) | (RegZ8::encodeRegName(reg) << 4);
    insn.emitInsn();
    insn.emitByte(opr);
    return getError();
}

Error AsmZ8::encodeInOpCode(
    InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp) {
    const AddrMode dst = insn.dstMode();
    const AddrMode src = insn.srcMode();
    const RegName reg = (dst == M_r) ? dstOp.reg : srcOp.reg;
    const Operand &op = (dst == M_r) ? srcOp : dstOp;
    const AddrMode mode = (dst == M_r) ? src : dst;
    insn.embed(RegZ8::encodeRegName(reg) << 4);
    insn.emitInsn();
    return encodeOperand(insn, mode, op);
}

#include <stdio.h>
Error AsmZ8::encodeTwoOperands(
    InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp) {
    insn.emitInsn();
    const AddrMode dst = insn.dstMode();
    const AddrMode src = insn.srcMode();
    if (dst == M_r || dst == M_Ir) {
        if (src == M_r || (dst == M_r && src == M_Ir)) {
            const uint8_t opr = RegZ8::encodeRegName(srcOp.reg)
                | (RegZ8::encodeRegName(dstOp.reg) << 4);
            insn.emitByte(opr);
            return getError();
        }
        return setError(INTERNAL_ERROR);
    }
    Error error1, error2;
    if (src == M_IM) {
        error1 = encodeOperand(insn, dst, dstOp);
        error2 = encodeOperand(insn, src, srcOp);
    } else {
        error1 = encodeOperand(insn, src, srcOp);
        error2 = encodeOperand(insn, dst, dstOp);
    }
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
        op.mode = M_IR;
        return OK;
    }
    if (regAddr) {
        if (op.val >= 0x100) return setError(OVERFLOW_RANGE);
        op.mode = M_R;
        return OK;
    }
    if (op.val >= 0x100) {
        op.mode = M_DA;
    } else if ((op.val & 0xF0) != /* RP */ 0x00) {
        op.mode = M_R;
    } else {
        op.mode = M_Rr;
        op.reg = _regs.decodeRegNum(op.val & 0xF);
    }
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
        return setError(TableZ8.getError());
    const AddrMode dst = insn.dstMode();
    const AddrMode src = insn.srcMode();

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
        if (dst == M_IRR && (dstOp.val & 1) != 0)
            return setError(ILLEGAL_REGISTER);
        insn.emitInsn();
        return encodeOperand(insn, dst, dstOp);
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

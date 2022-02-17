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

#include "asm_z80.h"

namespace libasm {
namespace z80 {

Error AsmZ80::encodeRelative(InsnZ80 &insn, const Operand &op) {
    const Config::uintptr_t base = insn.address() + 2;
    const Config::uintptr_t target = op.getError() ? base : op.val16;
    const Config::ptrdiff_t delta = target - base;
    if (overflowRel8(delta))
        return setError(OPERAND_TOO_FAR);
    insn.emitOperand8(delta);
    return OK;
}

Error AsmZ80::encodeIndexedBitOp(InsnZ80 &insn, const Operand &op) {
    const Config::opcode_t opc = insn.opCode();  // Bit opcode.
    insn.setOpCode(insn.prefix());               // Make 0xCB prefix as opcode.
    RegZ80::encodeIndexReg(insn, op.reg);        // Add 0xDD/0xFD prefix
    insn.emitOperand8(op.val16);                 // Index offset.
    insn.emitOperand8(opc);                      // Bit opcode.
    insn.emitInsn();
    return OK;
}

Error AsmZ80::encodeOperand(InsnZ80 &insn, const Operand &op, AddrMode mode, const Operand &other) {
    uint16_t val16 = op.val16;
    switch (mode) {
    case M_IM8:
        if (overflowUint8(val16))
            return setError(OVERFLOW_RANGE);
        insn.emitOperand8(val16);
        return OK;
    case M_IOA:
        if (val16 >= 0x100)
            return setError(OVERFLOW_RANGE);
        insn.emitOperand8(val16);
        return OK;
    case M_INDX:
        if (overflowRel8(static_cast<int16_t>(val16)))
            return setError(OVERFLOW_RANGE);
        if (insn.indexBit())
            return encodeIndexedBitOp(insn, op);
        insn.emitOperand8(val16);
        /* Fall-through */
    case R_IXIY:
    case I_IXIY:
        RegZ80::encodeIndexReg(insn, op.reg);
        return OK;
    case M_IM16:
    case M_ABS:
        insn.emitOperand16(val16);
        return OK;
    case M_REL:
        return encodeRelative(insn, op);
    case M_CC4:
    case M_CC8:
        insn.embed(static_cast<uint8_t>(val16) << 3);
        return OK;
    case M_PTR:
        insn.embed(RegZ80::encodePointerReg(op.reg) << 4);
        return OK;
    case M_PIX:
        insn.embed(RegZ80::encodePointerRegIx(op.reg, other.reg) << 4);
        return OK;
    case M_STK:
        insn.embed(RegZ80::encodeStackReg(op.reg) << 4);
        return OK;
    case I_BCDE:
        insn.embed(RegZ80::encodeIndirectBase(op.reg) << 4);
        return OK;
    case M_REG:
        insn.embed(RegZ80::encodeDataReg(op.reg));
        return OK;
    case M_DST:
        insn.embed(RegZ80::encodeDataReg(op.reg) << 3);
        return OK;
    case R_IR:
        insn.embed(RegZ80::encodeIrReg(op.reg) << 3);
        return OK;
    case M_VEC:
        if ((val16 & ~0x38) != 0)
            return setError(ILLEGAL_OPERAND);
        insn.embed(val16);
        return OK;
    case M_BIT:
        if (val16 >= 8)
            return setError(ILLEGAL_BIT_NUMBER);
        insn.embed(static_cast<uint8_t>(val16) << 3);
        return OK;
    case M_IMMD:
        switch (val16) {
        case 0:
            return OK;
        case 1:
            insn.embed(2 << 3);
            return OK;
        case 2:
            insn.embed(3 << 3);
            return OK;
        default:
            break;
        }
        return setError(ILLEGAL_OPERAND);
    default:
        return OK;
    }
}

Error AsmZ80::parseOperand(StrScanner &scan, Operand &op) {
    StrScanner p(scan.skipSpaces());
    if (endOfLine(*p))
        return OK;

    // 'C' is either C-reg or C-condition
    StrScanner a(p);
    const RegName reg = RegZ80::parseRegName(a);
    if (reg == REG_C) {
        op.mode = R_C;
        op.reg = REG_C;
        op.val16 = RegZ80::encodeCcName(CC_C);
        scan = a;
        return OK;
    }

    const CcName cc = RegZ80::parseCcName(p);
    if (cc != CC_UNDEF) {
        op.mode = RegZ80::isCc4Name(cc) ? M_CC4 : M_CC8;
        op.val16 = RegZ80::encodeCcName(cc);
        scan = p;
        return OK;
    }

    op.reg = RegZ80::parseRegName(p);
    if (op.reg != REG_UNDEF) {
        switch (op.reg) {
        case REG_IX:
        case REG_IY:
            op.mode = R_IXIY;
            break;
        case REG_I:
        case REG_R:
            op.mode = R_IR;
            break;
        case REG_B:
        case REG_D:
        case REG_E:
        case REG_H:
        case REG_L:
            op.mode = M_REG;
            break;
        default:
            op.mode = AddrMode(int8_t(op.reg) + 26);
            break;
        }
        scan = p;
        return OK;
    }
    if (p.expect('(')) {
        op.reg = RegZ80::parseRegName(p.skipSpaces());
        if (op.reg == REG_UNDEF) {
            op.val16 = parseExpr16(p, op);
            if (parserError())
                return getError();
            if (!p.skipSpaces().expect(')'))
                return setError(p, MISSING_CLOSING_PAREN);
            op.mode = M_ABS;
            scan = p;
            return OK;
        }
        if (p.skipSpaces().expect(')')) {
            switch (op.reg) {
            case REG_BC:
            case REG_DE:
                op.mode = I_BCDE;
                break;
            case REG_IX:
            case REG_IY:
                op.mode = I_IXIY;
                break;
            case REG_HL:
                op.mode = I_HL;
                break;
            case REG_SP:
                op.mode = I_SP;
                break;
            case REG_C:
                op.mode = I_C;
                break;
            default:
                return setError(REGISTER_NOT_ALLOWED);
            }
            scan = p;
            return OK;
        }
        if (*p == '+' || *p == '-') {
            if (op.reg == REG_IX || op.reg == REG_IY) {
                op.val16 = parseExpr16(p, op);
                if (parserError())
                    return getError();
                if (!p.skipSpaces().expect(')'))
                    return setError(MISSING_CLOSING_PAREN);
                scan = p;
                op.mode = M_INDX;
                return OK;
            }
        }
        return setError(UNKNOWN_OPERAND);
    }
    op.val16 = parseExpr16(p, op);
    if (parserError())
        return getError();
    op.mode = M_IM16;
    scan = p;
    return OK;
}

Error AsmZ80::encode(StrScanner &scan, Insn &_insn) {
    InsnZ80 insn(_insn);
    insn.setName(_parser.readSymbol(scan));

    Operand dstOp, srcOp;
    if (parseOperand(scan, dstOp))
        return getError();
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, srcOp))
            return getError();
        scan.skipSpaces();
    }
    if (!endOfLine(*scan))
        return setError(GARBAGE_AT_END);
    setErrorIf(dstOp);
    setErrorIf(srcOp);

    insn.setAddrMode(dstOp.mode, srcOp.mode);
    if (TableZ80.searchName(insn))
        return setError(TableZ80.getError());

    const AddrMode dst = insn.dstMode();
    if (dst != M_NO) {
        if (encodeOperand(insn, dstOp, dst, srcOp))
            return getError();
    }
    const AddrMode src = insn.srcMode();
    if (src != M_NO) {
        if (encodeOperand(insn, srcOp, src, dstOp))
            return getError();
    }
    if (!insn.indexBit())
        insn.emitInsn();
    return getError();
}

}  // namespace z80
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

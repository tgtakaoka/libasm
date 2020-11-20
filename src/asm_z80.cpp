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

#include "reg_z80.h"
#include "table_z80.h"

namespace libasm {
namespace z80 {

Error AsmZ80::encodeRelative(InsnZ80 &insn, const Operand &op) {
    const Config::uintptr_t base = insn.address() + 2;
    const Config::uintptr_t target = op.getError() ? base : op.val16;
    const Config::ptrdiff_t delta = target - base;
    if (delta < -128 || delta >= 128)
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

Error AsmZ80::encodeOperand(
        InsnZ80 &insn, const Operand &op, AddrMode mode, const Operand &other) {
    uint16_t val16 = op.val16;
    switch (mode) {
    case M_IM8:
        if (static_cast<int16_t>(val16) < -128 ||
                (static_cast<int16_t>(val16) >= 0 && val16 >= 256))
            return setError(OVERFLOW_RANGE);
        insn.emitOperand8(val16);
        return OK;
    case M_IOA:
        if (val16 >= 256)
            return setError(OVERFLOW_RANGE);
        insn.emitOperand8(val16);
        return OK;
    case M_INDX:
        if (static_cast<int16_t>(val16) < -128 ||
                static_cast<int16_t>(val16) >= 128)
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

Error AsmZ80::parseOperand(const char *scan, Operand &op) {
    const char *p = skipSpaces(scan);
    _scan = p;
    if (endOfLine(p))
        return OK;

    // 'C' is either C-reg or C-condition
    const RegName reg = RegZ80::parseRegName(p);
    if (reg == REG_C) {
        _scan = p + RegZ80::regNameLen(REG_C);
        op.mode = R_C;
        op.reg = REG_C;
        op.val16 = RegZ80::encodeCcName(CC_C);
        return OK;
    }

    const CcName cc = RegZ80::parseCcName(p);
    if (cc != CC_UNDEF) {
        _scan = p + RegZ80::ccNameLen(cc);
        op.mode = RegZ80::isCc4Name(cc) ? M_CC4 : M_CC8;
        op.val16 = RegZ80::encodeCcName(cc);
        return OK;
    }

    op.reg = RegZ80::parseRegName(p);
    if (op.reg != REG_UNDEF) {
        _scan = p + RegZ80::regNameLen(op.reg);
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
        return OK;
    }
    if (*p == '(') {
        p = skipSpaces(p + 1);
        op.reg = RegZ80::parseRegName(p);
        if (op.reg == REG_UNDEF) {
            op.val16 = parseExpr16(p);
            if (parserError())
                return getError();
            op.setError(getError());
            p = skipSpaces(_scan);
            if (*p != ')')
                return setError(MISSING_CLOSING_PAREN);
            op.mode = M_ABS;
            _scan = p + 1;
            return OK;
        }
        p = skipSpaces(p + RegZ80::regNameLen(op.reg));
        if (*p == ')') {
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
            _scan = p + 1;
            return OK;
        }
        if (*p == '+' || *p == '-') {
            if (op.reg == REG_IX || op.reg == REG_IY) {
                op.val16 = parseExpr16(p);
                if (parserError())
                    return getError();
                op.setError(getError());
                p = skipSpaces(_scan);
                if (*p != ')')
                    return setError(MISSING_CLOSING_PAREN);
                _scan = p + 1;
                op.mode = M_INDX;
                return OK;
            }
        }
        return setError(UNKNOWN_OPERAND);
    }
    op.val16 = parseExpr16(p);
    if (parserError())
        return getError();
    op.setError(getError());
    op.mode = M_IM16;
    return OK;
    ;
}

Error AsmZ80::encode(Insn &_insn) {
    InsnZ80 insn(_insn);
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);

    Operand dstOp, srcOp;
    if (parseOperand(endName, dstOp))
        return getError();
    const char *p = skipSpaces(_scan);
    if (*p == ',') {
        if (parseOperand(p + 1, srcOp))
            return getError();
        p = skipSpaces(_scan);
    }
    if (!endOfLine(p))
        return setError(GARBAGE_AT_END);
    setErrorIf(dstOp.getError());
    setErrorIf(srcOp.getError());

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

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

#include "asm_i8048.h"

#include "error_reporter.h"
#include "table_i8048.h"

namespace libasm {
namespace i8048 {

Error AsmI8048::parseOperand(const char *scan, Operand &op) {
    const char *p = skipSpaces(scan);
    _scan = p;
    if (endOfLine(p))
        return OK;

    if (*p == '#') {
        op.val16 = parseExpr16(p + 1);
        if (parserError())
            return getError();
        op.setError(getError());
        op.mode = M_IMM8;
        return OK;
    }

    const bool indir = (*p == '@');
    if (indir) {
        p++;
        if (isspace(*p))
            return setError(UNKNOWN_OPERAND);
    }

    op.reg = RegI8048::parseRegName(p);
    if (op.reg != REG_UNDEF) {
        p += RegI8048::regNameLen(op.reg);
        if (indir) {
            switch (op.reg) {
            case REG_A:
                op.mode = M_IA;
                break;
            case REG_R0:
            case REG_R1:
                op.mode = M_IR;
                break;
            case REG_R3:
                op.mode = M_IR3;
                break;
            default:
                return setError(REGISTER_NOT_ALLOWED);
            }
            _scan = p;
            return OK;
        }
        switch (op.reg) {
        case REG_BUS:
            op.mode = M_BUS;
            break;
        case REG_P:
            op.mode = M_P;
            break;
        case REG_P1:
            op.mode = M_P1;
            break;
        case REG_P2:
            op.mode = M_P2;
            break;
        case REG_P4:
        case REG_P5:
        case REG_P6:
        case REG_P7:
            op.mode = M_PEXT;
            break;
        case REG_A:
            op.mode = M_A;
            break;
        case REG_PSW:
            op.mode = M_PSW;
            break;
        case REG_F0:
        case REG_F1:
            op.mode = M_F;
            break;
        case REG_C:
            op.mode = M_C;
            break;
        case REG_I:
            op.mode = M_I;
            break;
        case REG_RB0:
        case REG_RB1:
            op.mode = M_RB;
            break;
        case REG_MB0:
        case REG_MB1:
            op.mode = M_MB;
            break;
        case REG_T:
            op.mode = M_T;
            break;
        case REG_CLK:
            op.mode = M_CLK;
            break;
        case REG_CNT:
            op.mode = M_CNT;
            break;
        case REG_TCNT:
            op.mode = M_TCNT;
            break;
        case REG_TCNTI:
            op.mode = M_TCNTI;
            break;
        default:
            if (RegI8048::isRReg(op.reg)) {
                op.mode = M_R;
            } else {
                return setError(UNKNOWN_OPERAND);
            }
        }
        _scan = p;
        return OK;
    }
    if (indir)
        return setError(UNKNOWN_OPERAND);

    op.val16 = parseExpr16(p);
    if (parserError())
        return getError();
    op.setError(getError());
    p = _scan;
    op.mode = M_AD11;
    return OK;
}

Error AsmI8048::encodeAddress(InsnI8048 &insn, const AddrMode mode, const Operand &op) {
    if (mode == M_AD08) {
        const Config::uintptr_t page = (insn.address() + 1) & ~0xFF;
        if ((op.val16 & ~0xFF) != page)
            return setError(OPERAND_TOO_FAR);
        insn.emitOperand8(op.val16);
        return setOK();
    }
    const Config::uintptr_t max = 1UL << uint8_t(config().addressWidth());
    if (op.val16 >= max)
        return setError(OVERFLOW_RANGE);
    insn.embed((op.val16 >> 3) & 0xE0);
    insn.emitOperand8(op.val16);
    return setOK();
}

Error AsmI8048::encodeOperand(InsnI8048 &insn, const AddrMode mode, const Operand &op) {
    switch (mode) {
    case M_IR:
    case M_IR3:
    case M_R:
        insn.embed(RegI8048::encodeRReg(op.reg));
        return OK;
    case M_P12:
    case M_PEXT:
        insn.embed(RegI8048::encodePort(op.reg));
        return OK;
    case M_AD08:
    case M_AD11:
        return encodeAddress(insn, mode, op);
    case M_IMM8:
    case M_BIT8:
        if (overflowUint8(op.val16))
            return setError(OVERFLOW_RANGE);
        insn.emitOperand8(op.val16);
        return OK;
    case M_BITN:
        if (op.val16 >= 8)
            return setError(ILLEGAL_BIT_NUMBER);
        insn.embed(op.val16 << 5);
        return OK;
    case M_F:
        insn.embed(op.reg == REG_F1 ? 0x20 : 0);
        return OK;
    case M_RB:
        insn.embed(op.reg == REG_RB1 ? 0x10 : 0);
        return OK;
    case M_MB:
        insn.embed(op.reg == REG_MB1 ? 0x10 : 0);
        return OK;
    default:
        return OK;
    }
}

Error AsmI8048::encode(Insn &_insn) {
    InsnI8048 insn(_insn);
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
    setError(dstOp.getError());
    setErrorIf(srcOp.getError());

    insn.setAddrMode(dstOp.mode, srcOp.mode);
    if (TableI8048.searchName(insn))
        return setError(TableI8048.getError());

    if (encodeOperand(insn, insn.dstMode(), dstOp))
        return getError();
    if (encodeOperand(insn, insn.srcMode(), srcOp)) {
        insn.reset();
        return getError();
    }
    insn.emitInsn();
    return getError();
}

}  // namespace i8048
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

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

#include "asm_ins8070.h"

#include "table_ins8070.h"

namespace libasm {
namespace ins8070 {

Error AsmIns8070::emitAbsolute(InsnIns8070 &insn, const Operand &op) {
    // PC will be +1 before fetching instruction.
    const Config::uintptr_t target = op.getError() ? 0 : op.val16 - 1;
    insn.emitOperand16(target);
    return OK;
}

Error AsmIns8070::emitImmediate(InsnIns8070 &insn, const Operand &op) {
    if (insn.oprSize() == SZ_WORD)
        insn.emitOperand16(op.val16);
    if (insn.oprSize() == SZ_BYTE)
        insn.emitOperand8(op.val16);
    return OK;
}

Error AsmIns8070::emitRelative(InsnIns8070 &insn, const Operand &op) {
    const Config::uintptr_t base = insn.address() + 1;
    // PC will be +1 before feting instruction
    const uint8_t fetch = (insn.addrMode() == RELATIVE) ? 1 : 0;
    const Config::uintptr_t target =
            (op.getError() ? base + fetch : op.val16) - fetch;
    const Config::ptrdiff_t offset = target - base;
    if (offset < -128 || offset >= 128)
        return setError(OPERAND_TOO_FAR);
    insn.emitOperand8(static_cast<uint8_t>(offset));
    return OK;
}

Error AsmIns8070::emitGeneric(InsnIns8070 &insn, const Operand &op) {
    if (op.format == OPR_IM) {
        insn.embed(4);
        return emitImmediate(insn, op);
    }
    if ((op.format == OPR_16 || op.format == OPR_4) && op.reg == REG_UNDEF) {
        const Config::uintptr_t target = op.getError() ? 0xFF00 : op.val16;
        if (target < 0xFF00)
            return setError(OVERFLOW_RANGE);
        insn.embed(5);
        insn.emitOperand8(static_cast<uint8_t>(target));
        return OK;
    }
    if (op.reg == REG_PC)
        return emitRelative(insn, op);

    const Config::ptrdiff_t offset = static_cast<Config::uintptr_t>(op.val16);
    if (offset < -128 || offset >= 128)
        return setError(OVERFLOW_RANGE);
    insn.embed(RegIns8070::encodePointerReg(op.reg));
    if (op.autoIndex)
        insn.embed(4);
    insn.emitOperand8(static_cast<uint8_t>(offset));
    return OK;
}

Error AsmIns8070::emitOperand(
        InsnIns8070 &insn, OprFormat format, const Operand &op) {
    switch (format) {
    case OPR_PN:
    case OPR_BR:
        insn.embed(RegIns8070::encodePointerReg(op.reg));
        break;
    case OPR_4:
        insn.embed(op.val16 & 0x0F);
        break;
    case OPR_IM:
        return emitImmediate(insn, op);
    case OPR_16:
        return emitAbsolute(insn, op);
    case OPR_PR:
    case OPR_GN:
        return emitGeneric(insn, op);
    case OPR_RL:
        return emitRelative(insn, op);
    default:
        break;
    }
    return OK;
}

Error AsmIns8070::parseOperand(const char *scan, Operand &op) {
    const char *p = skipSpaces(scan);
    _scan = p;
    if (endOfLine(p) || *p == ',')
        return OK;

    if (*p == '#' || *p == '=') {
        op.val16 = parseExpr16(p + 1);
        if (parserError())
            return getError();
        op.setError(getError());
        op.format = OPR_IM;
        return OK;
    }

    const RegName reg = RegIns8070::parseRegName(p);
    if (reg != REG_UNDEF) {
        _scan = p + RegIns8070::regNameLen(reg);
        OprFormat opr;
        switch (reg) {
        case REG_A:
            opr = OPR_A;
            break;
        case REG_E:
            opr = OPR_E;
            break;
        case REG_S:
            opr = OPR_S;
            break;
        case REG_EA:
            opr = OPR_EA;
            break;
        case REG_T:
            opr = OPR_T;
            break;
        case REG_SP:
            opr = OPR_SP;
            break;
        case REG_PC:
            opr = OPR_BR;
            break;
        default:
            opr = OPR_PN;
            break;
        }
        op.reg = reg;
        op.format = opr;
        return OK;
    }

    bool autoIndex = (*p == '@');
    if (autoIndex)
        p++;
    op.val16 = parseExpr16(p);
    if (parserError())
        return getError();
    op.setError(getError());
    p = skipSpaces(_scan);

    if (*p == '(')  // SC/MP style
        return setError(MISSING_COMMA);
    if (*p == ',') {
        p = skipSpaces(p + 1);
        if (!autoIndex && *p == '@') {
            autoIndex = true;
            p++;
        }
        const RegName ptr = RegIns8070::parseRegName(p);
        if (!RegIns8070::isPointerReg(ptr))
            setError(UNKNOWN_OPERAND);
        if (autoIndex && (ptr == REG_PC || ptr == REG_SP))
            return setError(REGISTER_NOT_ALLOWED);
        _scan = p + RegIns8070::regNameLen(ptr);
        op.reg = ptr;
        op.autoIndex = autoIndex;
        op.format =
                (autoIndex || ptr == REG_SP || ptr == REG_PC) ? OPR_GN : OPR_PR;
        return OK;
    }

    op.format = (op.val16 < 0x10) ? OPR_4 : OPR_16;
    return OK;
}

Error AsmIns8070::encode(Insn &_insn) {
    InsnIns8070 insn(_insn);
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);

    Operand dst, src;
    if (parseOperand(endName, dst))
        return getError();
    const char *p = skipSpaces(_scan);
    if (*p == ',') {
        if (parseOperand(p + 1, src))
            return getError();
        p = skipSpaces(_scan);
    }
    if (!endOfLine(p))
        return setError(GARBAGE_AT_END);
    setErrorIf(dst.getError());
    setErrorIf(src.getError());

    insn.setOprFormats(dst.format, src.format);
    if (TableIns8070.searchName(insn))
        return setError(TableIns8070.getError());

    if (emitOperand(insn, insn.dstOpr(), dst))
        return getError();
    if (emitOperand(insn, insn.srcOpr(), src))
        return getError();
    insn.emitInsn();
    return getError();
}

}  // namespace ins8070
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

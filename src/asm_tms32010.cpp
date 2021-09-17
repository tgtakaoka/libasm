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

#include "asm_tms32010.h"

#include "table_tms32010.h"

namespace libasm {
namespace tms32010 {

static AddrMode constantType(uint16_t val) {
    if (val == 0)
        return M_LS0;
    if (val == 1)
        return M_DPK;
    if (val == 4)
        return M_LS3;
    if (val < 8)
        return M_IM3;
    if (val < 16)
        return M_LS4;
    if (val < 0x100)
        return M_IM8;
    if (val < 0x1000)
        return M_PMA;
    const int16_t k13 = static_cast<int16_t>(val);
    if (k13 >= -0x1000 && k13 < 0x1000)
        return M_IM13;
    return M_NO;
}

Error AsmTms32010::encodeOperand(InsnTms32010 &insn, const Operand &op, AddrMode mode) {
    static constexpr Config::opcode_t SST = 0x7C00;
    switch (mode) {
    case M_MAM:
        switch (op.mode) {
        case M_ARP:
            insn.embed(0x88);
            break;
        case M_INC:
            insn.embed(0xA8);
            break;
        case M_DEC:
            insn.embed(0x98);
            break;
        default:
            if (insn.opCode() == SST && op.val16 < 0x80)
                return setError(OVERFLOW_RANGE);
            insn.embed(op.val16 & 0x7F);
            break;
        }
        break;
    case M_LS3:
    case M_LS4:
    case M_PA:
        insn.embed(op.val16 << 8);
        break;
    case M_NARP:
        if (op.mode == M_NO)
            break;
        insn.setOpCode(insn.opCode() & ~8);
        insn.embed(op.val16);
        break;
    case M_AR:
        insn.embed(op.val16 << 8);
        break;
    case M_ARK:
    case M_DPK:
        insn.embed(op.val16);
        break;
    case M_IM8:
        insn.embed(op.val16);
        break;
    case M_IM13:
        insn.embed(op.val16 & 0x1FFF);
        break;
    case M_PMA:
        insn.emitOperand16(op.val16);
        break;
    default:
        break;
    }
    return OK;
}

Error AsmTms32010::parseOperand(const char *scan, Operand &op) {
    const char *p = skipSpaces(scan);
    _scan = p;
    if (endOfLine(p))
        return OK;

    if (*p == '*') {
        p++;
        if (*p == '+') {
            p++;
            op.mode = M_INC;
        } else if (*p == '-') {
            p++;
            op.mode = M_DEC;
        } else {
            op.mode = M_ARP;
        }
        _scan = p;
        return OK;
    }
    op.reg = RegTms32010::parseRegName(p);
    if (op.reg != REG_UNDEF) {
        _scan = p + RegTms32010::regNameLen(op.reg);
        if (RegTms32010::isAuxiliary(op.reg)) {
            op.mode = M_AR;
            op.val16 = int8_t(op.reg) - int8_t(REG_AR0);
        } else {
            op.mode = M_PA;
            op.val16 = int8_t(op.reg) - int8_t(REG_PA0);
        }
        return OK;
    }

    op.val16 = parseExpr16(p);
    if (parserError())
        return getError();
    op.setError(getError());
    op.mode = constantType(op.val16);
    if (op.mode == M_NO)
        return setError(OVERFLOW_RANGE);
    return OK;
}

Error AsmTms32010::encode(Insn &_insn) {
    InsnTms32010 insn(_insn);
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);

    Operand op1, op2, op3;
    if (parseOperand(endName, op1))
        return getError();
    const char *p = skipSpaces(_scan);
    if (*p == ',') {
        if (parseOperand(p + 1, op2))
            return getError();
        p = skipSpaces(_scan);
    }
    if (*p == ',') {
        if (parseOperand(p + 1, op3))
            return getError();
        p = skipSpaces(_scan);
    }
    if (!endOfLine(p))
        return setError(GARBAGE_AT_END);
    setErrorIf(op1.getError());
    setErrorIf(op2.getError());
    setErrorIf(op3.getError());

    insn.setAddrMode(op1.mode, op2.mode, op3.mode);
    if (TableTms32010.searchName(insn))
        return setError(TableTms32010.getError());

    const AddrMode mode1 = insn.op1();
    if (mode1 != M_NO && encodeOperand(insn, op1, mode1))
        return getError();
    const AddrMode mode2 = insn.op2();
    if (mode2 != M_NO && encodeOperand(insn, op2, mode2)) {
        insn.resetAddress(insn.address());
        return getError();
    }
    const AddrMode mode3 = insn.op3();
    if (mode3 != M_NO && encodeOperand(insn, op3, mode3)) {
        insn.resetAddress(insn.address());
        return getError();
    }
    insn.emitInsn();
    return getError();
}

}  // namespace tms32010
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

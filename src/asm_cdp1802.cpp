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

#include "asm_cdp1802.h"

#include "reg_cdp1802.h"
#include "table_cdp1802.h"

namespace libasm {
namespace cdp1802 {

Error AsmCdp1802::encodePage(InsnCdp1802 &insn, AddrMode mode, const Operand &op) {
    const Config::uintptr_t base = insn.address() + 2;
    const Config::uintptr_t target = op.getError() ? base : op.val16;
    const Config::uintptr_t page = base & ~0xFF;
    if (mode == PAGE) {
        if ((target & ~0xFF) != page)
            return setError(OPERAND_TOO_FAR);
        insn.emitInsn();
        insn.emitByte(target);
        return OK;
    }
    if ((target & ~0xFF) == page && _smartBranch) {
        const auto opc = insn.opCode();
        insn.setOpCode(0x30 | (opc & 0xF)); // convert to in-page branch
        insn.emitInsn();
        insn.emitByte(target);
        return OK;
    }
    insn.emitInsn();
    insn.emitUint16(op.val16);
    return OK;
}

Error AsmCdp1802::emitOperand(InsnCdp1802 &insn, AddrMode mode, const Operand &op) {
    uint16_t val16 = op.val16;
    switch (mode) {
    case REG1:
        if (op.getError())
            val16 = 7;  // default work register.
        if (val16 == 0)
            return setError(REGISTER_NOT_ALLOWED);
        /* Fall-through */
    case REGN:
        if (op.getError())
            val16 = 7;  // default work register.
        if (val16 >= 16)
            return setError(ILLEGAL_REGISTER);
        insn.embed(val16);
        insn.emitInsn();
        return OK;
    case IMM8:
        if (overflowUint8(val16))
            return setError(OVERFLOW_RANGE);
        insn.emitInsn();
        insn.emitByte(val16);
        return OK;
    case PAGE:
    case ADDR:
        return encodePage(insn, mode, op);
        return OK;
    case IOAD:
        if (op.getError())
            val16 = 1;  // default IO address
        if (val16 == 0 || val16 >= 8)
            return setError(OPERAND_NOT_ALLOWED);
        insn.embed(val16);
        insn.emitInsn();
        return OK;
    default:
        insn.emitInsn();
        return OK;
    }
}

Error AsmCdp1802::parseOperand(const char *scan, Operand &op) {
    const char *p = skipSpaces(scan);
    _scan = p;
    if (endOfLine(p))
        return OK;

    if (_useReg) {
        const RegName reg = RegCdp1802::parseRegName(p);
        if (reg != REG_UNDEF) {
            _scan = p + RegCdp1802::regNameLen(reg);
            op.val16 = int8_t(reg);
            op.mode = REGN;
            return OK;
        }
    }
    op.val16 = parseExpr16(p);
    if (parserError())
        return getError();
    op.setError(getError());
    op.mode = ADDR;
    return OK;
}

Error AsmCdp1802::encode(Insn &_insn) {
    InsnCdp1802 insn(_insn);
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);

    Operand op1, op2;
    if (parseOperand(endName, op1))
        return getError();
    const char *p = skipSpaces(_scan);
    if (*p == ',') {
        if (parseOperand(p + 1, op2))
            return getError();
        p = skipSpaces(_scan);
    }
    if (!endOfLine(p))
        return setError(GARBAGE_AT_END);
    setErrorIf(op1.getError());
    setErrorIf(op2.getError());

    insn.setAddrMode(op1.mode, op2.mode);
    if (TableCdp1802.searchName(insn))
        return setError(TableCdp1802.getError());

    if (emitOperand(insn, insn.mode1(), op1))
        return getError();
    if (insn.mode2() == ADDR) {
        insn.emitUint16(op2.val16);
    }
    return OK;
}

}  // namespace cdp1802
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

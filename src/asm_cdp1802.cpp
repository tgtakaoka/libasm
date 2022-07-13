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

namespace libasm {
namespace cdp1802 {

const char AsmCdp1802::OPT_BOOL_USE_REGISTER[] PROGMEM = "use-register";
const char AsmCdp1802::OPT_BOOL_SMART_BRANCH[] PROGMEM = "smart-branch";

Error AsmCdp1802::encodePage(InsnCdp1802 &insn, AddrMode mode, const Operand &op) {
    const Config::uintptr_t base = insn.address() + 2;
    const Config::uintptr_t target = op.getError() ? base : op.val16;
    const Config::uintptr_t page = base & ~0xFF;
    if (mode == PAGE) {
        if ((target & ~0xFF) != page)
            return setError(op, OPERAND_TOO_FAR);
        insn.emitInsn();
        insn.emitByte(target);
        return OK;
    }
    if ((target & ~0xFF) == page && _smartBranch) {
        const auto opc = insn.opCode();
        insn.setOpCode(0x30 | (opc & 0xF));  // convert to in-page branch
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
            return setError(op, REGISTER_NOT_ALLOWED);
        /* Fall-through */
    case REGN:
        if (op.getError())
            val16 = 7;  // default work register.
        if (val16 >= 16)
            return setError(op, ILLEGAL_REGISTER);
        insn.embed(val16);
        insn.emitInsn();
        break;
    case IMM8:
        if (overflowUint8(val16))
            return setError(op, OVERFLOW_RANGE);
        insn.emitInsn();
        insn.emitByte(val16);
        break;
    case PAGE:
    case ADDR:
        return encodePage(insn, mode, op);
    case IOAD:
        if (op.getError())
            val16 = 1;  // default IO address
        if (val16 == 0 || val16 >= 8)
            return setError(op, OPERAND_NOT_ALLOWED);
        insn.embed(val16);
        insn.emitInsn();
        break;
    default:
        insn.emitInsn();
        break;
    }
    return getError();
}

Error AsmCdp1802::parseOperand(StrScanner &scan, Operand &op) {
    StrScanner p(scan.skipSpaces());
    op.setAt(p);
    if (endOfLine(*p))
        return OK;

    if (_useReg) {
        const RegName reg = RegCdp1802::parseRegName(p);
        if (reg != REG_UNDEF) {
            op.val16 = int8_t(reg);
            op.mode = REGN;
            scan = p;
            return OK;
        }
    }
    op.val16 = parseExpr16(p, op);
    if (parserError())
        return getError();
    op.mode = ADDR;
    scan = p;
    return OK;
}

Error AsmCdp1802::encode(StrScanner &scan, Insn &_insn) {
    InsnCdp1802 insn(_insn);
    insn.setName(_parser.readSymbol(scan));

    Operand op1, op2;
    if (parseOperand(scan, op1))
        return getError();
    const StrScanner op1p(scan);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, op2))
            return getError();
        scan.skipSpaces();
    }
    if (!endOfLine(*scan))
        return setError(scan, GARBAGE_AT_END);
    setErrorIf(op1);
    setErrorIf(op2);

    insn.setAddrMode(op1.mode, op2.mode);
    if (TableCdp1802.searchName(insn))
        return setError(TableCdp1802.getError());

    emitOperand(insn, insn.mode1(), op1);
    if (insn.mode2() == ADDR)
        insn.emitUint16(op2.val16);
    return getError();
}

}  // namespace cdp1802
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

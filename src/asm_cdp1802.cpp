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

#include "table_cdp1802.h"

namespace libasm {
namespace cdp1802 {

Error AsmCdp1802::encodePage(InsnCdp1802 &insn, const Operand &op) {
    const Config::uintptr_t base = insn.address() + 2;
    const Config::uintptr_t target = op.getError() ? base : op.val16;
    const Config::uintptr_t page = base & ~0xFF;
    if ((target & ~0xFF) != page)
        return setError(OPERAND_TOO_FAR);
    insn.emitInsn();
    insn.emitByte(target);
    return OK;
}

Error AsmCdp1802::parseOperand(const char *scan, Operand &op) {
    const char *p = skipSpaces(scan);
    _scan = p;
    if (endOfLine(p))
        return OK;

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

    Operand op;
    if (parseOperand(endName, op))
        return getError();
    const char *p = skipSpaces(_scan);
    if (!endOfLine(p))
        return setError(GARBAGE_AT_END);
    setErrorIf(op.getError());

    insn.setAddrMode(op.mode);
    if (TableCdp1802.searchName(insn))
        return setError(TableCdp1802.getError());

    uint16_t val16 = op.val16;
    switch (insn.addrMode()) {
    case REG1:
        if (op.getError())
            val16 = 7;  // default work register.
        if (val16 == 0)
            return setError(REGISTER_NOT_ALLOWED);
        /* Fall-through */
    case REGN:
        if (val16 >= 16)
            return setError(ILLEGAL_REGISTER);
        insn.embed(val16);
        insn.emitInsn();
        return OK;
    case IMM8:
        if (static_cast<int16_t>(val16) < -128 ||
                (static_cast<int16_t>(val16) >= 0 && val16 >= 0x100))
            return setError(OVERFLOW_RANGE);
        insn.emitInsn();
        insn.emitByte(val16);
        return OK;
    case PAGE:
        return encodePage(insn, op);
    case ADDR:
        insn.emitInsn();
        insn.emitUint16(val16);
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

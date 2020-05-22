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

Error AsmCdp1802::encodeRegn(InsnCdp1802 &insn) {
    uint8_t regNo;
    if (getOperand(regNo)) return getError();
    if (getError() == UNDEFINED_SYMBOL)
        regNo = 7; // first working register
    if (regNo >= 16 || (insn.addrMode() == REG1 && regNo == 0))
        return setError(ILLEGAL_REGISTER);
    insn.embed(regNo);
    insn.emitInsn();
    return OK;
}

Error AsmCdp1802::encodeImm8(InsnCdp1802 &insn) {
    uint8_t val = 0;
    if (getOperand(val)) return getError();
    insn.emitInsn();
    insn.emitByte(val);
    return OK;
}

Error AsmCdp1802::encodePage(InsnCdp1802 &insn) {
    Config::uintptr_t addr;
    if (getOperand(addr)) return getError();
    if (getError() == UNDEFINED_SYMBOL)
        addr = insn.address() + 2;
    const Config::uintptr_t base = (insn.address() + 2) & ~0xFF;
    if ((addr & ~0xFF) != base)
        return setError(OPERAND_TOO_FAR);
    insn.emitInsn();
    insn.emitByte(static_cast<uint8_t>(addr));
    return OK;
}

Error AsmCdp1802::encodeAddr(InsnCdp1802 &insn) {
    Config::uintptr_t addr;
    if (getOperand(addr)) return getError();
    insn.emitInsn();
    insn.emitUint16(addr);
    return OK;
}

Error AsmCdp1802::encodeIoad(InsnCdp1802 &insn) {
    uint8_t ioAddr;
    if (getOperand(ioAddr)) return getError();
    if (getError() == UNDEFINED_SYMBOL)
        ioAddr = 1;
    if (ioAddr == 0 || ioAddr >= 8)
        return setError(OVERFLOW_RANGE);
    insn.embed(ioAddr);
    insn.emitInsn();
    return OK;
}

Error AsmCdp1802::encode(Insn &_insn) {
    InsnCdp1802 insn(_insn);
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);
    if (TableCdp1802.searchName(insn))
        return setError(UNKNOWN_INSTRUCTION);
    _scan = skipSpaces(endName);

    switch (insn.addrMode()) {
    case IMPL: insn.emitInsn(); break;
    case REGN:
    case REG1: encodeRegn(insn); break;
    case IMM8: encodeImm8(insn); break;
    case PAGE: encodePage(insn); break;
    case ADDR: encodeAddr(insn); break;
    case IOAD: encodeIoad(insn); break;
    default: return setError(INTERNAL_ERROR);
    }
    return checkLineEnd();
}

} // namespace cdp1802
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

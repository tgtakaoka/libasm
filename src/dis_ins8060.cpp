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

#include "dis_ins8060.h"
#include "table_ins8060.h"

namespace libasm {
namespace ins8060 {

char *DisIns8060::outRegister(char *out, RegName regName) {
    return _regs.outRegName(out, regName);
}

Error DisIns8060::decodePntr(InsnIns8060 &insn, char *out) {
    outRegister(out, _regs.decodePointerReg(insn.opCode() & 3));
    return setOK();
}

Error DisIns8060::decodeImm8(
        DisMemory& memory, InsnIns8060 &insn, char *out) {
    uint8_t val;
    if (insn.readByte(memory, val)) return setError(NO_MEMORY);
    outConstant(out, val);
    return setOK();
}

Error DisIns8060::decodeIndx(
    DisMemory &memory, InsnIns8060& insn, char *out, bool hasMode) {
    const RegName reg = _regs.decodePointerReg(insn.opCode() & 3);
    uint8_t opr;
    if (insn.readByte(memory, opr)) return setError(NO_MEMORY);
    if (hasMode && (insn.opCode() & 4) != 0)
        *out++ = '@';
    if (reg == REG_PC && opr != 0x80) { // PC relative
        // PC points the last byte of instruction.
        const Config::uintptr_t base = insn.address() + 1;
        const Config::uintptr_t page = (base & ~0xFFF);
        const int8_t disp = static_cast<int8_t>(opr);
        // PC will be incremented before fetching next instruction.
        const int8_t fetch = insn.addrMode() == REL8 ? 1 : 0;
        // Program space is paged by 4kB.
        Config::uintptr_t target = base + disp + fetch;
        if ((target & ~0xFFF) == page) {
            outRelativeAddr(out, target, insn.address(), 8);
        } else {
            target &= 0xFFF;
            target |= page;
            outAddress(out, target);
        }
        return setOK();
    }
    if (opr == 0x80) {         // E(Pn)
        out = outRegister(out, REG_E);
    } else {
        const int8_t disp = static_cast<int8_t>(opr);
        out = outConstant(out, disp, 10);
    }
    *out++ = '(';
    out = outRegister(out, reg);
    *out++ = ')';
    *out = 0;
    return setOK();
}

Error DisIns8060::decode(DisMemory &memory, Insn &_insn, char *out) {
    InsnIns8060 insn(_insn);
    Config::opcode_t opCode;
    if (insn.readByte(memory, opCode)) return setError(NO_MEMORY);
    insn.setOpCode(opCode);
    if (TableIns8060.searchOpCode(insn))
        return setError(TableIns8060.getError());

    switch (insn.addrMode()) {
    case INHR:
        return setOK();
    case PNTR:
        return decodePntr(insn, out);
    case IMM8:
        return decodeImm8(memory, insn, out);
    case REL8:
    case DISP:
        return decodeIndx(memory, insn, out, false);
    case INDX:
        return decodeIndx(memory, insn, out, true);
    default:
        return setError(INTERNAL_ERROR);
    }
}

} // namespace ins8060
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

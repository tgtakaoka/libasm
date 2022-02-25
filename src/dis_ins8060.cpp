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

StrBuffer &DisIns8060::outRegister(StrBuffer &out, RegName regName) {
    return _regs.outRegName(out, regName);
}

Error DisIns8060::decodePntr(InsnIns8060 &insn, StrBuffer &out) {
    outRegister(out, _regs.decodePointerReg(insn.opCode()));
    return setOK();
}

Error DisIns8060::decodeImm8(DisMemory &memory, InsnIns8060 &insn, StrBuffer &out) {
    outHex(out, insn.readByte(memory), 8);
    return setError(insn);
}

Error DisIns8060::decodeIndx(DisMemory &memory, InsnIns8060 &insn, StrBuffer &out, bool hasMode) {
    const RegName reg = _regs.decodePointerReg(insn.opCode());
    const uint8_t opr = insn.readByte(memory);
    if (hasMode && (insn.opCode() & 4) != 0)
        out.letter('@');
    if (reg == REG_PC && opr != 0x80) {  // PC relative
        // PC points the last byte of instruction.
        const auto base = insn.address() + 1;
        const auto disp = static_cast<int8_t>(opr);
        // PC will be incremented before fetching next instruction.
        const int8_t fetch = insn.addrMode() == REL8 ? 1 : 0;
        // Program space is paged by 4kB.
        const auto toff = offset(offset(base) + disp + fetch);
        const Config::uintptr_t target = page(base) | toff;
        const Config::ptrdiff_t diff = target - base - fetch;
        if (diff >= -127 && diff < 128) {
            outRelAddr(out, target, insn.address(), 8);
        } else {
            outAbsAddr(out, target);
        }
    } else {
        if (opr == 0x80) {  // E(Pn)
            outRegister(out, REG_E);
        } else {
            const int8_t disp = static_cast<int8_t>(opr);
            outDec(out, disp, -8);
        }
        outRegister(out.letter('('), reg).letter(')');
    }
    return setError(insn);
}

Error DisIns8060::decode(DisMemory &memory, Insn &_insn, StrBuffer &out) {
    InsnIns8060 insn(_insn);
    const Config::opcode_t opCode = insn.readByte(memory);
    if (setError(insn))
        return getError();
    insn.setOpCode(opCode);

    if (TableIns8060.searchOpCode(insn))
        return setError(TableIns8060.getError());

    switch (insn.addrMode()) {
    case PNTR:
        decodePntr(insn, out);
        break;
    case IMM8:
        decodeImm8(memory, insn, out);
        break;
    case REL8:
    case DISP:
        decodeIndx(memory, insn, out, false);
        break;
    case INDX:
        decodeIndx(memory, insn, out, true);
        break;
    default:
        setOK();
        break;
    }
    if (page(insn.address()) != page(insn.address() + insn.length() - 1))
        setError(OVERWRAP_PAGE);
    return getError();
}

}  // namespace ins8060
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

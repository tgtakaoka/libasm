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

#include "dis_ins8070.h"
#include "table_ins8070.h"

namespace libasm {
namespace ins8070 {

char *DisIns8070::outRegister(char *out, RegName regName) {
    return _regs.outRegName(out, regName);
}

char *DisIns8070::outOperand(char *out, OprFormat opr, uint8_t value) {
    switch (opr) {
    case OPR_A:  return outRegister(out, REG_A);
    case OPR_E:  return outRegister(out, REG_E);
    case OPR_S:  return outRegister(out, REG_S);
    case OPR_EA: return outRegister(out, REG_EA);
    case OPR_PN: return outRegister(out, (value & 1) ? REG_P3 : REG_P2);
    case OPR_BR: return outRegister(out, RegIns8070::decodePointerReg(value & 3));
    case OPR_T:  return outRegister(out, REG_T);
    case OPR_4:  return outConstant(out, static_cast<uint8_t>(value & 15), 10);
    default:     return out;
    }
}

Error DisIns8070::decodeImplied(InsnIns8070 &insn, char *out) {
    out = outOperand(out, insn.dstOpr(), insn.opCode());
    if (insn.srcOpr() != OPR_NO) {
        *out++ = ',';
        outOperand(out, insn.srcOpr(), insn.opCode());
    }
    return setOK();
}

Error DisIns8070::decodeImmediate(
    DisMemory &memory, InsnIns8070 &insn, char *out) {
    out = outOperand(out, insn.dstOpr(), insn.opCode());
    *out++ = ',';
    *out++ = _immSym ? '#' : '=';
    if (insn.oprSize() == SZ_WORD)
        return decodeAbsolute(memory, insn, out);

    outConstant(out, insn.readByte(memory));
    return setError(insn);
}

Error DisIns8070::decodeAbsolute(
    DisMemory &memory, InsnIns8070 &insn, char *out) {
    const uint8_t fetch = (insn.addrMode() == ABSOLUTE) ? 1 : 0;
    const Config::uintptr_t target = insn.readUint16(memory) + fetch;
    outAddress(out, target);
    return setError(insn);
}

Error DisIns8070::decodeDirect(
    DisMemory &memory, InsnIns8070 &insn, char *out) {
    const Config::uintptr_t target = 0xFF00 | insn.readByte(memory);
    outAddress(out, target);
    return setError(insn);
}

Error DisIns8070::decodeRelative(
        DisMemory &memory, InsnIns8070 &insn, char *out) {
    const Config::ptrdiff_t disp = static_cast<int8_t>(insn.readByte(memory));
    const OprFormat src = insn.srcOpr();
    const RegName base = _regs.decodePointerReg(insn.opCode() & 3);
    if (insn.dstOpr() == OPR_RL
        || (src == OPR_GN && base == REG_PC)) {
        const uint8_t fetch = (insn.addrMode() == RELATIVE) ? 1 : 0;
        const Config::uintptr_t target = insn.address() + 1 + disp + fetch;
        out = outRelativeAddr(out, target, insn.address(), 8);
        if (src == OPR_GN) {
            *out++ = ',';
            outRegister(out, REG_PC);
        }
    } else {
        out = outConstant(out, disp, 10);
        *out++ = ',';
        if (src == OPR_PR) {
            outOperand(out, src, insn.opCode());
        } else {
            outOperand(out, OPR_BR, insn.opCode());
        }
    }
    return setError(insn);
}

Error DisIns8070::decodeGeneric(
    DisMemory &memory, InsnIns8070 &insn, char *out) {
    const uint8_t mode = insn.opCode() & 7;
    if (mode != 4) {
       out = outOperand(out, insn.dstOpr());
       *out++ = ',';
    }
    switch (mode) {
    case 4:
        return decodeImmediate(memory, insn, out);
    case 5:
        return decodeDirect(memory, insn, out);
    case 6: case 7:
        *out++ = '@';
        return decodeRelative(memory, insn, out);
    default:
        return decodeRelative(memory, insn, out);
    }
}

Error DisIns8070::decode(DisMemory &memory, Insn &_insn, char *out) {
    InsnIns8070 insn(_insn);
    const Config::opcode_t opCode = insn.readByte(memory);
    if (setError(insn)) return getError();
    insn.setOpCode(opCode);

    if (TableIns8070.searchOpCode(insn))
        return setError(TableIns8070.getError());

    switch (insn.addrMode()) {
    case IMPLIED:
        return decodeImplied(insn, out);
    case IMMEDIATE:
        return decodeImmediate(memory, insn, out);
    case ABSOLUTE:
        return decodeAbsolute(memory, insn, out);
    case RELATIVE:
        return decodeRelative(memory, insn, out);
    case GENERIC:
        return decodeGeneric(memory, insn, out);
    default:
        return setError(INTERNAL_ERROR);
    }
}

} // namespace ins8070
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

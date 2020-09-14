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

bool DisIns8070::outOperand(char *out, OprFormat opr, uint8_t value) {
    switch (opr) {
    case OPR_A: outRegister(out, REG_A); break;
    case OPR_E: outRegister(out, REG_E); break;
    case OPR_S: outRegister(out, REG_S); break;
    case OPR_EA: outRegister(out, REG_EA); break;
    case OPR_PN:
        outRegister(out, (value & 1) ? REG_P3 : REG_P2);
        break;
    case OPR_BR:
        switch (value & 3) {
        case 0: outRegister(out, REG_PC); break;
        case 1: outRegister(out, REG_SP); break;
        case 2: outRegister(out, REG_P2); break;
        case 3: outRegister(out, REG_P3); break;
        }
        break;
    case OPR_T: outRegister(out, REG_T); break;
    case OPR_4:
        outConstant(out, static_cast<uint8_t>(value & 15), 10);
        break;
    case OPR_NO: break;
    default:
        return false;
    }
    return true;
}

Error DisIns8070::decodeImplied(InsnIns8070 &insn, char *out) {
    if (outOperand(out, insn.dstOpr(), insn.opCode())) {
        out += strlen(out);
        if (insn.srcOpr() != OPR_NO) {
            *out++ = ',';
            outOperand(out, insn.srcOpr(), insn.opCode());
        }
        return setOK();
    }
    return setError(ILLEGAL_OPERAND);
}

Error DisIns8070::decodeImmediate(
    DisMemory &memory, InsnIns8070 &insn, char *out) {
    outOperand(out, insn.dstOpr(), insn.opCode());
    out += strlen(out);
    *out++ = ',';
    *out++ = _immSym ? '#' : '=';
    if (insn.oprSize() == SZ_WORD)
        return decodeAbsolute(memory, insn, out);

    uint8_t val;
    if (insn.readByte(memory, val)) return setError(NO_MEMORY);
    outConstant(out, val, 16);
    return setOK();
}

Error DisIns8070::decodeAbsolute(
    DisMemory &memory, InsnIns8070 &insn, char *out) {
    uint16_t val;
    if (insn.readUint16(memory, val)) return setError(NO_MEMORY);
    const uint8_t fetch = (insn.addrMode() == ABSOLUTE) ? 1 : 0;
    const Config::uintptr_t target = val + fetch;
    outAddress(out, target);
    return setOK();
}

Error DisIns8070::decodeRelative(
        DisMemory &memory, InsnIns8070 &insn, char *out) {
    uint8_t val;
    if (insn.readByte(memory, val)) return setError(NO_MEMORY);
    const Config::ptrdiff_t disp = static_cast<int8_t>(val);
    const OprFormat src = insn.srcOpr();
    const RegName base = _regs.decodePointerReg(insn.opCode());
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
    return setOK();
}

Error DisIns8070::decodeGeneric(
    DisMemory &memory, InsnIns8070 &insn, char *out) {
    const uint8_t mode = insn.opCode() & 7;
    if (mode == 4) return decodeImmediate(memory, insn, out);

    outOperand(out, insn.dstOpr());
    out += strlen(out);
    *out++ = ',';
    if (mode < 4) return decodeRelative(memory, insn, out);
    if (mode >= 6) {
        *out++ = '@';
        return decodeRelative(memory, insn, out);
    }
    if (mode == 5) {            // DIRECT
        uint8_t val;
        if (insn.readByte(memory, val)) return setError(NO_MEMORY);
        const Config::uintptr_t addr = 0xFF00 | val;
        outAddress(out, addr);
        return setOK();
    }
    return UNKNOWN_INSTRUCTION;
}

Error DisIns8070::decode(DisMemory &memory, Insn &_insn, char *out) {
    InsnIns8070 insn(_insn);
    Config::opcode_t opCode;
    if (insn.readByte(memory, opCode)) return setError(NO_MEMORY);
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

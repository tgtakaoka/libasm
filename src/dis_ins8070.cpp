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

static const char OPT_BOOL_IMM_PREFIX[] PROGMEM = "imm-prefix";
static const char OPT_DESC_IMM_PREFIX[] PROGMEM = "immediate prefix # (default =)";

DisIns8070::OptImmediatePrefix::OptImmediatePrefix(bool &var)
    : BoolOption(OPT_BOOL_IMM_PREFIX, OPT_DESC_IMM_PREFIX, var) {}

StrBuffer &DisIns8070::outRegister(StrBuffer &out, RegName regName) {
    return _regs.outRegName(out, regName);
}

Error DisIns8070::decodeImmediate(DisMemory &memory, InsnIns8070 &insn, StrBuffer &out) {
    out.letter(_immediatePrefix ? '#' : '=');
    if (insn.oprSize() == SZ_WORD) {
        outHex(out, insn.readUint16(memory), 16);
    } else {
        outHex(out, insn.readByte(memory), 8);
    }
    return setError(insn);
}

Error DisIns8070::decodeAbsolute(DisMemory &memory, InsnIns8070 &insn, StrBuffer &out) {
    const uint8_t fetch = insn.execute() ? 1 : 0;
    const Config::uintptr_t target = insn.readUint16(memory) + fetch;
    outAbsAddr(out, target);
    return setError(insn);
}

Error DisIns8070::decodeDirect(DisMemory &memory, InsnIns8070 &insn, StrBuffer &out) {
    const Config::uintptr_t target = 0xFF00 | insn.readByte(memory);
    outAbsAddr(out, target);
    return setError(insn);
}

Error DisIns8070::decodeRelative(
        DisMemory &memory, InsnIns8070 &insn, StrBuffer &out, AddrMode mode) {
    const Config::ptrdiff_t disp = static_cast<int8_t>(insn.readByte(memory));
    const RegName base = _regs.decodePointerReg(insn.opCode());
    if (mode == M_PCR) {
        const uint8_t fetch = insn.execute() ? 1 : 0;
        const Config::uintptr_t target = insn.address() + 1 + disp + fetch;
        outRelAddr(out, target, insn.address(), 8);
        if (fetch == 0)
            outRegister(out.letter(','), REG_PC);
    } else {
        outDec(out, disp, -8).letter(',');
        outRegister(out, base);
    }
    return setError(insn);
}

Error DisIns8070::decodeGeneric(DisMemory &memory, InsnIns8070 &insn, StrBuffer &out) {
    const uint8_t mode = insn.opCode() & 7;
    switch (mode) {
    case 0:
        return decodeRelative(memory, insn, out, M_PCR);
    case 1:
    case 2:
    case 3:
        return decodeRelative(memory, insn, out, M_IDX);
    case 4:
        return decodeImmediate(memory, insn, out);
    case 5:
        return decodeDirect(memory, insn, out);
    default:  // 6, 7
        return decodeRelative(memory, insn, out.letter('@'), M_IDX);
    }
}

Error DisIns8070::decodeOperand(
        DisMemory &memory, InsnIns8070 &insn, StrBuffer &out, AddrMode mode) {
    switch (mode) {
    case M_AR:
        outRegister(out, REG_A);
        break;
    case M_ER:
        outRegister(out, REG_E);
        break;
    case M_SR:
        outRegister(out, REG_S);
        break;
    case M_EA:
        outRegister(out, REG_EA);
        break;
    case M_P23:
        outRegister(out, (insn.opCode() & 1) ? REG_P3 : REG_P2);
        break;
    case M_PTR:
        outRegister(out, RegIns8070::decodePointerReg(insn.opCode()));
        break;
    case M_TR:
        outRegister(out, REG_T);
        break;
    case M_IMM:
        decodeImmediate(memory, insn, out);
        break;
    case M_ADR:
        decodeAbsolute(memory, insn, out);
        break;
    case M_VEC:
        outDec(out, insn.opCode() & 15, 4);
        break;
    case M_IDX:
    case M_PCR:
        decodeRelative(memory, insn, out, mode);
        break;
    case M_GEN:
        decodeGeneric(memory, insn, out);
        break;
    default:
        return OK;
    }
    if (isOK())
        setError(insn);
    return getError();
}

Error DisIns8070::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) {
    InsnIns8070 insn(_insn);
    const Config::opcode_t opCode = insn.readByte(memory);
    if (setError(insn))
        return getError();
    insn.setOpCode(opCode);

    if (TableIns8070::TABLE.searchOpCode(insn))
        return setError(insn);

    const AddrMode dst = insn.dst();
    if (dst == M_NONE)
        return setOK();
    if (decodeOperand(memory, insn, out, dst))
        return getError();

    const AddrMode src = insn.src();
    if (src == M_NONE)
        return setOK();
    out.comma();
    return decodeOperand(memory, insn, out, src);
}

}  // namespace ins8070
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

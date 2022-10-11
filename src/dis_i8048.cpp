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

#include "dis_i8048.h"

#include "entry_i8048.h"
#include "error_reporter.h"
#include "table_i8048.h"

namespace libasm {
namespace i8048 {

Error DisI8048::decodeOperand(
        DisMemory &memory, InsnI8048 &insn, StrBuffer &out, const AddrMode mode) {
    const Config::opcode_t opc = insn.opCode();
    switch (mode) {
    case M_IA:
        out.letter('@');
        // Fall-through
    case M_A:
        _regs.outRegName(out, REG_A);
        break;
    case M_IR:
    case M_IR3:
        out.letter('@');
        // Fall-through
    case M_R:
        _regs.outRegName(out, _regs.decodeRReg(opc & 7));
        break;
    case M_BUS:
        _regs.outRegName(out, REG_BUS);
        break;
    case M_P12:
        if ((opc & 7) == 0 || (opc & 7) == 3)
            return setError(UNKNOWN_INSTRUCTION);
        // Fall-through
    case M_PEXT:
        _regs.outRegName(out, _regs.decodePort(opc & 7));
        break;
    case M_P:
        _regs.outRegName(out, REG_P);
        break;
    case M_P1:
        _regs.outRegName(out, REG_P1);
        break;
    case M_P2:
        _regs.outRegName(out, REG_P2);
        break;
    case M_AD08:
        outAbsAddr(out, ((insn.address() + 1) & ~0xFF) | insn.readByte(memory));
        break;
    case M_AD11:
        outAbsAddr(out, (static_cast<Config::uintptr_t>(opc & 0xE0) << 3) | insn.readByte(memory));
        break;
    case M_BITN:
        outDec(out, (opc & 0xE0) >> 5, 3);
        break;
    case M_IMM8:
        outDec(out.letter('#'), insn.readByte(memory), 8);
        break;
    case M_BIT8:
        outHex(out.letter('#'), insn.readByte(memory), 8, false);
        break;
    case M_PSW:
        _regs.outRegName(out, REG_PSW);
        break;
    case M_C:
        _regs.outRegName(out, REG_C);
        break;
    case M_F:
        _regs.outRegName(out, (opc & 0x20) ? REG_F1 : REG_F0);
        break;
    case M_RB:
        _regs.outRegName(out, (opc & 0x10) ? REG_RB1 : REG_RB0);
        break;
    case M_MB:
        _regs.outRegName(out, (opc & 0x10) ? REG_MB1 : REG_MB0);
        break;
    case M_I:
        _regs.outRegName(out, REG_I);
        break;
    case M_T:
        _regs.outRegName(out, REG_T);
        break;
    case M_CNT:
        _regs.outRegName(out, REG_CNT);
        break;
    case M_TCNT:
        _regs.outRegName(out, REG_TCNT);
        break;
    case M_TCNTI:
        _regs.outRegName(out, REG_TCNTI);
        break;
    case M_CLK:
        _regs.outRegName(out, REG_CLK);
        break;
    default:
        break;
    }
    return setOK();
}

Error DisI8048::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) {
    InsnI8048 insn(_insn);
    const Config::opcode_t opCode = insn.readByte(memory);
    insn.setOpCode(opCode);
    if (setError(insn))
        return getError();

    if (TableI8048::TABLE.searchOpCode(insn))
        return setError(TableI8048::TABLE.getError());

    const AddrMode dst = insn.dstMode();
    if (dst != M_NONE) {
        if (decodeOperand(memory, insn, out, dst))
            return getError();
    }
    const AddrMode src = insn.srcMode();
    if (src != M_NONE) {
        out.comma();
        if (decodeOperand(memory, insn, out, src))
            return getError();
    }
    insn.emitInsn();
    return setError(insn);
}

}  // namespace i8048
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

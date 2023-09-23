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

#include "reg_i8048.h"
#include "table_i8048.h"

namespace libasm {
namespace i8048 {

using namespace reg;

const ValueFormatter::Plugins &DisI8048::defaultPlugins() {
    return ValueFormatter::Plugins::intel();
}

DisI8048::DisI8048(const ValueFormatter::Plugins &plugins) : Disassembler(plugins), Config(TABLE) {
    reset();
}

Error DisI8048::decodeOperand(DisInsn &insn, StrBuffer &out, const AddrMode mode) {
    const auto opc = insn.opCode();
    switch (mode) {
    case M_IA:
        out.letter('@');
        // Fall-through
    case M_A:
        outRegName(out, REG_A);
        break;
    case M_IR:
    case M_IR3:
        out.letter('@');
        // Fall-through
    case M_R:
        outRegName(out, decodeRRegNum(opc & 7));
        break;
    case M_BUS:
        outRegName(out, REG_BUS);
        break;
    case M_P12:
        if ((opc & 7) == 0 || (opc & 7) == 3)
            return setError(UNKNOWN_INSTRUCTION);
        // Fall-through
    case M_PEXT:
        outRegName(out, decodePortNum(opc & 7));
        break;
    case M_P:
        outRegName(out, REG_P);
        break;
    case M_P1:
        outRegName(out, REG_P1);
        break;
    case M_P2:
        outRegName(out, REG_P2);
        break;
    case M_AD08:
        outAbsAddr(out, ((insn.address() + 1) & ~0xFF) | insn.readByte());
        break;
    case M_AD11:
        outAbsAddr(out, (static_cast<Config::uintptr_t>(opc & 0xE0) << 3) | insn.readByte());
        break;
    case M_BITN:
        outDec(out, (opc & 0xE0) >> 5, 3);
        break;
    case M_IMM8:
        outDec(out.letter('#'), insn.readByte(), 8);
        break;
    case M_BIT8:
        outHex(out.letter('#'), insn.readByte(), 8, false);
        break;
    case M_PSW:
        outRegName(out, REG_PSW);
        break;
    case M_C:
        outRegName(out, REG_C);
        break;
    case M_F:
        outRegName(out, (opc & 0x20) ? REG_F1 : REG_F0);
        break;
    case M_RB:
        outRegName(out, (opc & 0x10) ? REG_RB1 : REG_RB0);
        break;
    case M_MB:
        outRegName(out, (opc & 0x10) ? REG_MB1 : REG_MB0);
        break;
    case M_I:
        outRegName(out, REG_I);
        break;
    case M_T:
        outRegName(out, REG_T);
        break;
    case M_CNT:
        outRegName(out, REG_CNT);
        break;
    case M_TCNT:
        outRegName(out, REG_TCNT);
        break;
    case M_TCNTI:
        outRegName(out, REG_TCNTI);
        break;
    case M_CLK:
        outRegName(out, REG_CLK);
        break;
    default:
        break;
    }
    return setOK();
}

Error DisI8048::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) {
    DisInsn insn(_insn, memory);
    const auto opCode = insn.readByte();
    insn.setOpCode(opCode);
    if (setError(insn))
        return getError();

    if (TABLE.searchOpCode(cpuType(), insn, out))
        return setError(insn);

    const auto dst = insn.dst();
    if (dst != M_NONE) {
        if (decodeOperand(insn, out, dst))
            return getError();
    }
    const auto src = insn.src();
    if (src != M_NONE) {
        out.comma();
        if (decodeOperand(insn, out, src))
            return getError();
    }
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

/*
 * Copyright 2023 Tadashi G. Takaoka
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

#include "dis_f3850.h"
#include "reg_f3850.h"
#include "table_f3850.h"
#include "text_common.h"

namespace libasm {
namespace f3850 {

using namespace reg;
using namespace text::common;

const ValueFormatter::Plugins& DisF3850::defaultPlugins() {
    return ValueFormatter::Plugins::fairchild();
}

DisF3850::DisF3850(const ValueFormatter::Plugins& plugins) : Disassembler(plugins), Config(TABLE) {
    reset();
}

void DisF3850::decodeRelative(DisInsn& insn, StrBuffer& out) const {
    const auto delta = static_cast<int8_t>(insn.readByte());
    const auto base = insn.address() + 1;
    const auto target = base + delta;
    insn.setErrorIf(out, checkAddr(target));
    outRelAddr(out, target, insn.address(), 8);
}

void DisF3850::decodeOperand(DisInsn& insn, StrBuffer& out, AddrMode mode) const {
    const auto opCode = insn.opCode();
    switch (mode) {
    case M_REG: {
        if ((opCode & 0xF) == 15)
            insn.setErrorIf(out, OPERAND_NOT_ALLOWED);
        const auto name = decodeRegName(opCode);
        if (name != REG_UNDEF) {
            outRegName(out, name);
            break;
        }
        outDec(out, opCode & 0xF, 4);
        break;
    }
    case M_C1:
        outDec(out, 1, 1);
        break;
    case M_C4:
        outDec(out, 4, 3);
        break;
    case M_IM3:
        outHex(out, opCode & 7, 3);
        break;
    case M_IOS:
        if ((opCode & 0xF) == 2 || (opCode & 0xF) == 3)
            insn.setErrorIf(out, OPERAND_NOT_ALLOWED);
        /* Fall-through */
    case M_IM4:
        outHex(out, opCode & 0xF, 4);
        break;
    case M_IOA: {
        const auto ioa = insn.readByte();
        if (ioa < 4)
            insn.setErrorIf(out, OPERAND_NOT_ALLOWED);
        outHex(out, ioa, 8);
        break;
    }
    case M_IM8:
        outHex(out, insn.readByte(), 8);
        break;
    case M_ADDR:
        outAbsAddr(out, insn.readUint16());
        break;
    case M_REL:
        return decodeRelative(insn, out);
    case M_NONE:
        break;
    case M_J:
        outRegName(out, REG_J);
        break;
    default:
        if (uint8_t(mode) < uint8_t(M_REG))
            outRegName(out, RegName(uint8_t(mode)));
        break;
    }
}

Error DisF3850::decodeImpl(DisMemory& memory, Insn& _insn, StrBuffer& out) const {
    DisInsn insn(_insn, memory, out);
    insn.setOpCode(insn.readByte());
    if (searchOpCode(cpuType(), insn, out))
        return _insn.setError(insn);

    const auto mode1 = insn.mode1();
    if (mode1 != M_NONE) {
        decodeOperand(insn, out, mode1);
        const auto mode2 = insn.mode2();
        if (mode2 != M_NONE)
            decodeOperand(insn, out.comma(), mode2);
    }
    return _insn.setError(insn);
}

}  // namespace f3850
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
